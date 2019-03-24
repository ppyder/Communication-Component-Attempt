using System;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Threading;
using System.IO.Ports;

namespace TempUpper
{
    //实现通信协议
    public class InterfaceToSlave : ComEntity  //继承通信实体类
    {
        #region 通用组件

        //数据接收缓冲区
        private byte[] Rx_Buffer;
        private int BufferIndex = 0;

        //数据接收计数器
        private UInt64 DataCounter_Byte = 0;
        private UInt64 DataCounter_Pack = 0;
        private UInt64 DataCounter_ErrorPack = 0;

        //接收到的字符数据缓冲区
        private string ReceiveStrBuffer = "";
        private int SrtBufferLength = 20000; //文本缓冲区的大小

        //标志
        private bool isShowInHex = true;
        private bool isTimeValid = true;
        private bool isRandomDataValid = true;

        //状态属性：接收到的包数量
        public UInt64 PackReceived
        {
            get
            {
                return DataCounter_Pack;
            }
            set
            {
                DataCounter_Pack = value;
            }
        }

        //状态属性：接收到的字节数
        public UInt64 ByteReceived
        {
            get
            {
                return DataCounter_Byte;
            }
            set
            {
                DataCounter_Byte = value;
            }
        }

        //状态属性：接收到的错误包数量
        public UInt64 ErrorPackReceived
        {
            get
            {
                return DataCounter_ErrorPack;
            }
            set
            {
                DataCounter_ErrorPack = value;
            }
        }

        //设置属性：接收字符串数据缓冲区大小，单位为字符个数
        public int RcvBufferSize
        {
            get
            {
                return SrtBufferLength;
            }
            set
            {
                SrtBufferLength = value;
            }
        }

        //设置属性：是否十六进制解析非格式化数据
        public bool IsShowInhex
        {
            get
            {
                return isShowInHex;
            }
            set
            {
                isShowInHex = value;
            }
        }

        //设置属性：是否在每个读取间隔加上时间标记
        public bool IsTimeMarkValid
        {
            get
            {
                return isTimeValid;
            }
            set
            {
                isTimeValid = value;
            }
        }

        //设置属性：接收到的非格式化数据是否有效
        public bool IsRandomDataValid
        {
            get
            {
                return isRandomDataValid;
            }
            set
            {
                isRandomDataValid = value;
            }
        }

        //内容属性：文本缓冲区(信息接收时，用于存储非致命通知的缓冲区,读取后自动清空)
        public string RcvStringBuffer
        {
            get
            {
                string temp = ReceiveStrBuffer;

                ReceiveStrBuffer = "";

                return temp;
            }
        }

        //动作属性:获取文本缓冲区的内容（受保护）
        protected string DataStrBuffer_Read
        {
            get
            {
                return ReceiveStrBuffer;
            }
        }

        //构造函数
        public InterfaceToSlave()
        {

        }

        //将数据写入字符串数据缓冲区
        protected void WriteDataInBuffer(string Str)
        {
            if (ReceiveStrBuffer.Length <= SrtBufferLength)
            {
                ReceiveStrBuffer += Str;
            }
            else
            {
                throw new Exception("<文本缓冲区>:保存的字符已经达到规定的上限！");
            }
        }
        #endregion

        #region 具体通信协议实现相关

        //用户数据
        public UserDatas Datas;

        //实例化用户数据，同时初始化通信参数
        public void InitUserDatas(ref UserDatas UserData)
        {
            Datas = UserData;

            //由于类在构造时先构造成员类，因此此处的用法是安全的
            //即Datas的RxBufferSize此时已经被初始化完成了，不是0值。
            Rx_Buffer = new byte[Datas.RxBufferSize];

            //初始化发送请求队列
            SendFlags.Request0 = UserDatas.MsgTypesID.TestCtrl;
            SendFlags.Request1 = UserDatas.MsgTypesID.TestCtrl;
            SendFlags.Request2 = UserDatas.MsgTypesID.TestCtrl;

            //允许上位机先发数据
            COM_log.isCouldSendMessage = true;            
        }

        #region 模拟半双工机制

        //结构体：发送状态指示
        struct SendIDlog
        {
            //标识是否可以在触发发送请求时发送数据，只有在接收到下位机回复的数据时才会被置一
            public bool isCouldSendMessage;

            //本机刚刚发出的指令类型
            public UserDatas.MsgTypesID HaveSendID;

            //对方反馈到本机：接收到的数据类型（指明上次从本机发出的指令）       
            public UserDatas.MsgTypesID MsgFeedback_FromSlave;

            //本机反馈到对方：接收到的数据类型（向对方指明本次接收到的指令）
            public UserDatas.MsgTypesID MsgFeedback_ToSlave;
        }

        //发送状态记录器
        SendIDlog COM_log = new SendIDlog();

        //重置发送标志以重新激活发送动作
        public void ReConnectSlave()
        {
            COM_log.MsgFeedback_ToSlave = UserDatas.MsgTypesID.Data_Error;
            COM_log.isCouldSendMessage = true;

            SendData();
        }
        #endregion

        #region 发送队列机制

        /* 结构体：发送标志队列
         * 发送标志队列，允许有三个请求排队。
         * 如果在特殊请求没有处理完成前又触发了特殊指令请求，
         * 那么新触发的请求将会被丢弃。
         * 按照时间顺序，依次按照先来后到进行响应。
         * 目前还没有规定指令优先级。
         * 其内容由指令标识常量枚举类型给出。                       
         */
        struct SendFlagsQueue
        {
            public UserDatas.MsgTypesID Request0;     //请求0：将最先被响应的请求
            public UserDatas.MsgTypesID Request1;     //请求0：将较先被响应的请求
            public UserDatas.MsgTypesID Request2;     //请求0：将最后被响应的请求
        }

        //指令请求队列：特殊指令排队（按先后顺序），一般指令将被覆盖（按优先级）
        private SendFlagsQueue SendFlags;

        //动作属性：赋值以产生发送请求
        public UserDatas.MsgTypesID SendRequest
        {
            set
            {
                //查询请求队列并赋值
                if (SendFlags.Request0 == UserDatas.MsgTypesID.TestCtrl)
                {
                    SendFlags.Request0 = value;
                }
                else if (SendFlags.Request1 == UserDatas.MsgTypesID.TestCtrl)
                {
                    SendFlags.Request1 = value;
                }
                else if (SendFlags.Request2 == UserDatas.MsgTypesID.TestCtrl)
                {
                    SendFlags.Request2 = value;
                }
                else  //如果请求队列已满，则忽略并驳回发送请求
                {
                    throw new Exception("<InterfaceToSlave>:特殊指令请求队列已满！请稍后再请求发送！\r\n本次驳回：" + value);
                }
            }
        }

        //检索并更新特殊指令请求队列
        UserDatas.MsgTypesID CheckRequestQueue()
        {
            UserDatas.MsgTypesID MsgToSend = COM_log.HaveSendID;

            //如果上次的指令被有效接收，就发送新的指令，否则重发
            if (COM_log.HaveSendID == COM_log.MsgFeedback_FromSlave)
            {
                MsgToSend = SendFlags.Request0;

                //前移发送请求
                SendFlags.Request0 = SendFlags.Request1;
                SendFlags.Request1 = SendFlags.Request2;
                SendFlags.Request2 = UserDatas.MsgTypesID.TestCtrl;

                //记录将被发送的数据类型
                COM_log.HaveSendID = MsgToSend;
            }

            return MsgToSend;
        }
        #endregion

        #region 数据处理部分

        //处理待发送的数据
        protected override byte[] DealDataToSend()
        {
            byte[] DataToSend;

            if(COM_log.isCouldSendMessage)
            {
                DataToSend = new byte[Datas.TxBufferSize];

                //置零数组
                Array.Clear(DataToSend, 0, DataToSend.Length);

                //标定数据头
                Datas.UpdateTxDataHead(CheckRequestQueue(), COM_log.MsgFeedback_ToSlave);

                //写入数据头信息
                Datas.StructToBytes(Datas.Tx_Datahead, ref DataToSend, 0);

                //写入数据内容
                Datas.StructToBytes(Datas.GetDataToSend((UserDatas.MsgTypesID)Datas.Tx_Datahead.CMD_type), ref DataToSend, Datas.DataHeadSize);

                //获取校验值
                Datas.Tx_Datatail.verify = Datas.GetCheckValue(DataToSend, 0, (uint)(Datas.DataHeadSize + Datas.TxDataContentSize));

                //写入数据尾
                Datas.StructToBytes(Datas.Tx_Datatail, ref DataToSend, Datas.DataHeadSize + Datas.TxDataContentSize);
            }
            else
            {
                //返回空数组
                DataToSend = new byte[0];
            }

            return DataToSend;
        }

        //接收组件缓冲区中的数据
        protected override void ReceiveData(byte[] DataBuffer)
        {
            //记录接收到的字符数
            DataCounter_Byte += (UInt64)DataBuffer.Length;

            //按照是否是格式化数据的类别区分处理
            foreach(byte Data in DataBuffer)
            {
                //单个字符检索
                GetFormatData(Data);
            }
                        
            //如果使能任意数据接收
            if (isRandomDataValid)
            {
                GetRandomData(DataBuffer);
            }
        }

        //处理格式化数据
        private void GetFormatData(byte Data)
        {
            bool isInvalid = false;    //标识本次接受的数据是否非法

            //在数据头处逐字检查以确保数据头被合理捕获
            switch(BufferIndex)
            {
                case 0:
                    isInvalid = (Data != UserDatas.Datahead1);
                    break;

                case 1:
                    isInvalid = (Data != UserDatas.Datahead2);
                    break;

                case 2:
                    isInvalid = (Data >= (byte)UserDatas.MsgTypesID.RxMaxCode);
                    break;

                case 3:
                    isInvalid = (Data >= (byte)UserDatas.MsgTypesID.RxMaxCode);
                    break;

                case 7:
                    //如果数据头校验不通过
                    if(!Datas.CheckDataHead(Rx_Buffer))
                    {
                        Rx_Buffer[BufferIndex] = Data;

                        BufferIndex = 0;

                        //检查中间接收的第4.5.6.7个字节是否隐藏着真正的数据头
                        for(int i = 4; i < 8; i++)
                        {
                            GetFormatData(Rx_Buffer[i]);
                        }
                        return;
                    }
                    break;

                default:break;
            }

            //发现是非法数据就舍弃
            if (isInvalid)
            {
                BufferIndex = 0;

                //针对在非法处理中刚好遇到数据头的情况
                if(Data == UserDatas.Datahead1)
                {
                    GetFormatData(Data);
                }

                return;
            }

            //将接收到的数据写入接收缓冲区
            Rx_Buffer[BufferIndex] = Data;

            //更新数组索引
            BufferIndex++;

            //数据包长度符合要求时再做拆包操作
            //至此，数据包以及具备的特征是，长度符合要求，数据头正确。
            if (Datas.RxBufferSize == BufferIndex)
            {
                DealFormatReceived(Rx_Buffer);
                BufferIndex = 0;
            }
        }

        //处理任意数据
        private void GetRandomData(byte[] unFrmtData)
        {
            //如果使能了时间戳功能
            if (isTimeValid && DataStrBuffer_Read == "")
            {
                //输出当前时间
                DateTime dt = DateTime.Now;
                WriteDataInBuffer(dt.GetDateTimeFormats('f')[0].ToString() + "\r\n");
            }

            string strRcv = null;

            //解析字符数据
            if (!isShowInHex)
            {
                for (int i = 0; i < unFrmtData.Length; i++)       //窗体显示
                {
                    strRcv += (char)unFrmtData[i];           //按字符格式显示
                }
            }
            else
            {
                for (int i = 0; i < unFrmtData.Length; i++)
                {
                    strRcv += (unFrmtData[i].ToString("X2") + " "); //16进制显示
                }
            }

            WriteDataInBuffer(strRcv);            
        }

        //解析接收到的数据
        private void DealFormatReceived(byte[] Buffer)
        {
            bool iserror = true;

            //获取数据头相应的信息
            UserDatas.Globle_Datahead tempHead = Datas.GetHeadData(Rx_Buffer, 0);

            if(Datas.CheckRxData(Rx_Buffer))
            {
                //记录下位机反馈的上次接收到的数据
                COM_log.MsgFeedback_FromSlave = (UserDatas.MsgTypesID)tempHead.DataFeedback;

                if(tempHead.CMD_type < (byte)UserDatas.MsgTypesID.RxMaxCode)
                {
                    //处理数据并向下位机标识本次接收到的数据类型
                    COM_log.MsgFeedback_ToSlave = Datas.DataDealFuncs[tempHead.CMD_type](Buffer);

                    //记录正确的包数量
                    DataCounter_Pack++;

                    iserror = false;
                }
            }
            
            //如果数据接收有问题
            if(iserror)
            {
                //输出当前时间
                DateTime dt = DateTime.Now;
                WriteDataInBuffer(dt.GetDateTimeFormats('f')[0].ToString() + "\r\n");

                string strRcv = null;

                //记录错误的包数量
                DataCounter_ErrorPack++;

                //解析字符数据
                for (int i = 0; i < Rx_Buffer.Length; i++)
                {
                    strRcv += (Rx_Buffer[i].ToString("X2") + " "); //16进制显示
                }

                SaveInfo("错误包：" + strRcv + "\r\n");

                //向下位机标识本次接收到的数据类型
                COM_log.MsgFeedback_ToSlave = UserDatas.MsgTypesID.Data_Error;
                COM_log.MsgFeedback_FromSlave = UserDatas.MsgTypesID.Data_Error;
            }

            //接收到数据包，意味着下位机完成了一次有效发送，即进入等待回应的状态，允许上位机发送数据
            COM_log.isCouldSendMessage = true;
        }
        #endregion
        #endregion
    }
}
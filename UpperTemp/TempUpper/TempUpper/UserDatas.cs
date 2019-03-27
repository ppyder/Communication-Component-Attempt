using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TempUpper
{
    //意图以后可以在UI上定制通信协议
    public partial class UserDatas : DataTools
    {
        //获取数据头类型
        public Type DataHeadType
        {
            get
            {
                return typeof(Globle_Datahead);
            }
        }
        //获取数据尾类型
        public Type DataTailType
        {
            get
            {
                return typeof(Globle_Datatail);
            }
        }
        //获取数据头大小
        public int DataHeadSize
        {
            get
            {
                return GetSize(typeof(Globle_Datahead));
            }
        }
        //获取数据尾大小
        public int DataTailSize
        {
            get
            {
                return GetSize(typeof(Globle_Datatail));
            }
        }
        //获取发送缓冲区总长
        public int RxBufferSize
        {
            get
            {
                return MaxRxBufferSize;
            }
        }
        //获取发送缓冲区总长
        public int TxBufferSize
        {
            get
            {
                return MaxTxBufferSize;
            }
        }
        //获取发送内容缓冲区长度
        public int TxDataContentSize
        {
            get
            {
                return (MaxTxBufferSize - DataHeadSize - DataTailSize);
            }
        }
        //获取接收内容缓冲区长度
        public int RxDataContentSize
        {
            get
            {
                return (MaxRxBufferSize - DataHeadSize - DataTailSize);
            }
        }

        //数据处理方法委托类型
        public delegate MsgTypesID DealFunc(byte[] Buffer);

        //委托数组，用于处理接收到的数据
        public DealFunc[] DataDealFuncs;

        #region 处理数据的函数

        #region 校验相关

        //获取数据头缓冲区中的数据
        //起始位置指定：数据头数据在数据缓冲区中的位置
        public Globle_Datahead GetHeadData(byte[] Buffer, int StartIndex)
        {
            if ((StartIndex + GetSize(typeof(Globle_Datahead)) > Buffer.Length))
            {
                throw new Exception("<获取数据头信息>:输入参数非法！");
            }

            return (Globle_Datahead)BytesToStruct(Buffer, StartIndex, DataHeadType);
        }

        //获取数据尾缓冲区中的数据
        //起始位置指定：数据尾数据在数据缓冲区中的位置
        public Globle_Datatail GetTailData(byte[] Buffer, int StartIndex)
        {
            if ((StartIndex + GetSize(typeof(Globle_Datatail)) > Buffer.Length))
            {
                throw new Exception("<获取数据尾信息>:输入参数非法！");
            }

            return (Globle_Datatail)BytesToStruct(Buffer, StartIndex, DataTailType);
        }

        //校验接收到的数据
        public bool CheckRxData(byte[] Bytes)
        {
            uint check = 0;

            //获取数据尾数据
            Globle_Datatail tempTail = GetTailData(Bytes, RxBufferSize - DataTailSize);

            //获取校验值
            check = GetCheckValue(Bytes, 0, (uint)(RxBufferSize - DataTailSize));

            //若不满足校验
            if (check != tempTail.verify)
                return false;

            return true;
        }

        //对数据头进行校验，是决定一组数据是不是数据头的关键判据
        //此处没有安全检查，因为它只在数据校验环节被调用了，可以确保其安全性。
        public bool CheckDataHead(byte[] Bytes)
        {
            int HeadVerify = (int)BytesToStruct(Bytes, 4, typeof(int));

            int check = Bytes[0] * Bytes[1] * Bytes[2] * Bytes[3];

            return (check == HeadVerify);
        }
        #endregion

        #region 发送数据处理

        //更新数据头信息
        //参数用于选定本次发送的数据类型和指定反馈到下位机的数据类型
        public void UpdateTxDataHead(MsgTypesID TxDataID, MsgTypesID FeedbackID)
        {
            Tx_Datahead.Data_Head1 = UserDatas.Datahead1;
            Tx_Datahead.Data_Head2 = UserDatas.Datahead2;
            Tx_Datahead.CMD_type = (byte)TxDataID;
            Tx_Datahead.DataFeedback = (byte)FeedbackID;

            Tx_Datahead.HeadVerify = Tx_Datahead.Data_Head1 * Tx_Datahead.Data_Head2
                                   * Tx_Datahead.CMD_type * Tx_Datahead.DataFeedback;
        }

        //由数据类型标签到发送缓冲区的映射
        public object GetDataToSend(MsgTypesID TxDataID)
        {
            object Msg = null;

            //获取待发送的数据内容
            switch (TxDataID)
            {
                case MsgTypesID.Tx_ErrorData:
                    Msg = EmptyTxBuffer;
                    break;

                case MsgTypesID.Tx_EmptyCMD:
                    Msg = EmptyTxBuffer;
                    break;

                case MsgTypesID.Tx_SampleCMD:
                    Msg = Tx_TestCtrlCMD;
                    break;

                default:
                    Msg = null;
                    break;
            }

            return Msg;
        }
        #endregion

        #region 自定义数据解析
        //处理下位机传上来的异常数据
        private MsgTypesID GetErrorData(byte[] Buffer)
        {
            //TODO：写入异常处理函数

            return MsgTypesID.Rx_ErrorData;
        }

        //处理下位机传上来的空数据，此处为无处理。
        private MsgTypesID GetEmptyData(byte[] Buffer)
        {
            return MsgTypesID.Rx_EmptyData;
        }
        
        //处理下位机传上来的采样数据
        private MsgTypesID GetTestData(byte[] Buffer)
        {
            double[] Datas = new double[4];
            TestData Rx_TestData = new TestData();

            //将数据写入缓冲区
            Rx_TestData = (TestData)BytesToStruct(Buffer, GetSize(typeof(Globle_Datahead)), typeof(TestData));

            Datas[0] = Rx_TestData.Time;

            Datas[1] = Rx_TestData.InputData;

            Datas[2] = Rx_TestData.OutputData;

            Datas[3] = Rx_TestData.OutputUc;

            //调用写入数据的方法
            //ParentForm.WriteinData(Datas);

            return MsgTypesID.Rx_SampleData;
        }
        #endregion
        #endregion

        //构造函数
        public UserDatas()
        {
            //初始化缓冲区
            InitBuffers();

            DataDealFuncs = new DealFunc[(int)MsgTypesID.RxMaxCode]
            {
                new DealFunc(GetErrorData),
                new DealFunc(GetEmptyData),
                new DealFunc(GetTestData),
            };

            return;
        }

        ~UserDatas()
        {
            return;
        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TempUpper
{
    //意图以后可以在UI上定制通信协议
    public class UserDatas : DataTools
    {
        #region 数据格式

        //数据包的格式化数据头
        public struct Globle_Datahead
        {
            public byte Data_Head1;    //固定数据头1（0xaa）
            public byte Data_Head2;    //固定数据头2（0xbb）
            public byte CMD_type;      //指令类型（标识数据类型）@ref enum ROV_MsgTypesID
            public byte DataFeedback;  //下位机接收到的数据类型反馈 @ref enum DataFeedbackTypes

            public int HeadVerify;    //数据头校验，相乘校验。用于辅助辨识数据头
        };

        //数据包的格式化数据尾
        public struct Globle_Datatail
        {
            public uint verify;        //校验（求和）
        };
        #endregion

        #region 数据内容

        //反馈测试数据
        public struct TestData
        {
            public ushort Time;         //每组参量对应的时刻
            public short InputData;    //输入参量
            public short OutputData;   //输出参量(系统输出)
            public short OutputUc;     //控制器输出
        }

        //测试控制指令
        public struct TestCtrlCMD
        {
            public byte CMD;        //指令
            public byte Reserve1;   //预留
            public byte Reserve2;   //预留
            public byte Reserve3;   //预留
        }
        #endregion

        #region 常量和枚举标识

        public const byte Datahead1 = 0xaa, Datahead2 = 0xbb;

        //通信数据类型标识集合（发送和接收分别从0开始）
        public enum MsgTypesID
        {
            //异常标识
            Data_Error = 0xff,    //数据解析异常

            //发送组
            TestCtrl = 0x00,     //测试控制指令

            //接收组
            TestData = 0x00,      //测试数据接收
            RxMaxCode             //接收到的数据的编码最大值，用做安全检查

        };
        #endregion

        #region 数据缓冲区及其大小

        //数据头
        public Globle_Datahead Tx_Datahead = new Globle_Datahead();
        //数据尾
        public Globle_Datatail Tx_Datatail = new Globle_Datatail();
        //数据内容缓冲区
        public TestCtrlCMD Tx_TestCtrlCMD = new TestCtrlCMD();

        //接收缓冲区长度的最大值
        private int MaxRxBufferSize = 0;
        //发送缓冲区长度的最大值
        private int MaxTxBufferSize = 0;
        #endregion

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
            check = GetCheckValue(Bytes, 0, (uint)(RxDataContentSize));

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
                case MsgTypesID.TestCtrl:
                    Msg = Tx_TestCtrlCMD;
                    break;

                default:
                    Msg = Tx_TestCtrlCMD;
                    break;
            }

            return Msg;
        }
        #endregion

        #region 自定义数据解析

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

            return MsgTypesID.TestData;
        }
        #endregion
        #endregion

        //构造函数
        public UserDatas()
        {
            //初始化通用接收缓冲区长度
            MaxRxBufferSize = GetSize(typeof(Globle_Datahead)) 
                + GetSize(typeof(Globle_Datatail))
                + GetMaxSize(typeof(TestData));

            //初始化通用发送缓冲区长度
            MaxTxBufferSize = GetSize(typeof(Globle_Datahead))
                + GetSize(typeof(Globle_Datatail))
                + GetMaxSize(typeof(TestCtrlCMD));

            DataDealFuncs = new DealFunc[(int)MsgTypesID.RxMaxCode]
            {
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

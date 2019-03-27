

namespace TempUpper
{
    //意图以后可以在UI上定制通信协议
    public partial class UserDatas : DataTools
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
            /***********  发送组  *************/
            //异常标识
            Tx_ErrorData = 0x00,
            //发送空指令
            Tx_EmptyCMD,
            //发送采样指令
            Tx_SampleCMD,
            //待发送的数据的编码最大值，用做安全检查
            TxMaxCode,

            /************  接收组  *************/
            //接收到错误数据
            Rx_ErrorData = 0x00,
            //空数据指令
            Rx_EmptyData,
            //采样数据
            Rx_SampleData,
            //接收到的数据的编码最大值，用做安全检查
            RxMaxCode             

        };
        #endregion

        #region 数据缓冲区及其大小

        //数据头
        public Globle_Datahead Tx_Datahead = new Globle_Datahead();
        //数据尾
        public Globle_Datatail Tx_Datatail = new Globle_Datatail();
        //数据内容缓冲区
        public TestCtrlCMD Tx_TestCtrlCMD = new TestCtrlCMD();
        private byte EmptyTxBuffer = new byte();

        //接收缓冲区长度的最大值
        private int MaxRxBufferSize = 0;
        //发送缓冲区长度的最大值
        private int MaxTxBufferSize = 0;
        #endregion

        //初始化数据缓冲区及其相关信息
        private void InitBuffers()
        {
            //初始化通用接收缓冲区长度
            MaxRxBufferSize = GetSize(typeof(Globle_Datahead))
                + GetSize(typeof(Globle_Datatail))
                + GetMaxSize(typeof(TestData));

            //初始化通用发送缓冲区长度
            MaxTxBufferSize = GetSize(typeof(Globle_Datahead))
                + GetSize(typeof(Globle_Datatail))
                + GetMaxSize(typeof(TestCtrlCMD));
        }

    }
}

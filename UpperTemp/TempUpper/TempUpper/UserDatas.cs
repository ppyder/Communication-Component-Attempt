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

                case MsgTypesID.Tx_BaseMotionCMD:
                    Msg = Tx_BaseMotionCMD;
                    break;

                case MsgTypesID.Tx_PlanMotionTestCMD:
                    Msg = Tx_PlanCtrlCMD;
                    break;

                case MsgTypesID.Tx_AutoProcessCMD:
                    Msg = Tx_MR1AutoCtrlCMD;
                    break;

                case MsgTypesID.Tx_UpperActionDebugCMD:
                    Msg = Tx_ActionDebugCMD;
                    break;

                default:
                    Msg = null;
                    break;
            }

            return Msg;
        }
        #endregion

        #region 自定义数据解析

        #region 由回调操作产生的委托集合

        //采样数据处理委托
        public delegate void SampleDataHandlerTypedef(SampleData Data);
        //委托实体字段
        private SampleDataHandlerTypedef SampleDataHandler = null;
        //初始化委托
        public void SampleHandlerInit(SampleDataHandlerTypedef Handler)
        {
            SampleDataHandler = Handler;
        }

        //PID数据回传处理委托
        public delegate void PIDDataHandlerTypedef(PIDDataGetTypedef Data);
        //委托实体字段
        private PIDDataHandlerTypedef PIDDataHandler = null;
        //初始化委托
        public void PIDDataHandlerInit(PIDDataHandlerTypedef Handler)
        {
            PIDDataHandler = Handler;
        }

        #endregion

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
        private MsgTypesID GetSampleData(byte[] Buffer)
        {
            //将数据写入缓冲区
            Rx_SampleData = (SampleData)BytesToStruct(Buffer, GetSize(typeof(Globle_Datahead)), typeof(SampleData));

            //调用写入数据的方法
            if (null == SampleDataHandler)
            {
                throw new Exception("<调用采样数据处理函数>：未初始化采样数据处理委托！请先调用函数\"SampleHandlerInit\"以初始化！");
            }
            else
            {
                SampleDataHandler(Rx_SampleData);
            }

            return MsgTypesID.Rx_SampleData;
        }

        //处理下位机传上来的基本运动状态数据
        private MsgTypesID GetBaseMotionData(byte[] Buffer)
        {
            BaseMotionDataTypedef Rx_BaseMotionMsg;

            //将数据更新
            Rx_BaseMotionMsg = (BaseMotionDataTypedef)BytesToStruct(Buffer, GetSize(typeof(Globle_Datahead)), typeof(BaseMotionDataTypedef));

            Rx_MR1State.Speed = Rx_BaseMotionMsg.Speed;
            Rx_MR1State.SpeedDir = Rx_BaseMotionMsg.SpeedDir;
            Rx_MR1State.RotateSpeed = Rx_BaseMotionMsg.RotateSpeed;

            Rx_MR1State.CoordinateX = Rx_BaseMotionMsg.CoordinateX;
            Rx_MR1State.CoordinateY = Rx_BaseMotionMsg.CoordinateY;
            Rx_MR1State.PoseAngle = Rx_BaseMotionMsg.PoseAngle;

            return MsgTypesID.Rx_BaseMotionData;
        }

        //处理下位机传上来的路径规划测试状态参量
        private MsgTypesID GetPlanTestData(byte[] Buffer)
        {
            BasePlanDataTypedef Rx_PlanMotionMsg;

            //将数据更新
            Rx_PlanMotionMsg = (BasePlanDataTypedef)BytesToStruct(Buffer, GetSize(typeof(Globle_Datahead)), typeof(BasePlanDataTypedef));

            Rx_MR1State.isPlanSuccessfully = (0 != Rx_PlanMotionMsg.isPlanSuccessfully);
            Rx_MR1State.SectionFlag = Rx_PlanMotionMsg.SectionFlag;

            Rx_MR1State.DistanceHaveMoved = Rx_PlanMotionMsg.DistanceHaveMoved;
            Rx_MR1State.FullDistance = Rx_PlanMotionMsg.FullDistance;

            Rx_MR1State.CoordinateX = Rx_PlanMotionMsg.CoordinateX;
            Rx_MR1State.CoordinateY = Rx_PlanMotionMsg.CoordinateY;
            Rx_MR1State.PoseAngle = Rx_PlanMotionMsg.PoseAngle;

            return MsgTypesID.Rx_PlanMotionTestData;
        }

        //处理下位机传上来的自动流程状态参量
        private MsgTypesID GetAutoProcessData(byte[] Buffer)
        {
            MR1CtrlDataTypedef Rx_MR1AutoMotionMsg;

            //将数据更新
            Rx_MR1AutoMotionMsg = (MR1CtrlDataTypedef)BytesToStruct(Buffer, GetSize(typeof(Globle_Datahead)), typeof(MR1CtrlDataTypedef));

            Rx_MR1State.isAutoMode = (0 != Rx_MR1AutoMotionMsg.isAutoMode);
            Rx_MR1State.CurrentState = Rx_MR1AutoMotionMsg.CurrentState;
            Rx_MR1State.isActionDoing = (0 != Rx_MR1AutoMotionMsg.isActionDoing);

            Rx_MR1State.CoordinateX = Rx_MR1AutoMotionMsg.CoordinateX;
            Rx_MR1State.CoordinateY = Rx_MR1AutoMotionMsg.CoordinateY;
            Rx_MR1State.PoseAngle = Rx_MR1AutoMotionMsg.PoseAngle;

            return MsgTypesID.Rx_AutoProcessData;
        }

        //处理下位机传上来的动作Debug状态数据
        private MsgTypesID GetActionTestData(byte[] Buffer)
        {
            ActionDebugDataTypedef Rx_ActionDebugMsg;

            //将数据更新
            Rx_ActionDebugMsg = (ActionDebugDataTypedef)BytesToStruct(Buffer, GetSize(typeof(Globle_Datahead)), typeof(ActionDebugDataTypedef));

            Rx_MR1State.PickMotorPos = Rx_ActionDebugMsg.PickMotorPos;
            Rx_MR1State.ShootMotorSpeed = Rx_ActionDebugMsg.ShootMotorSpeed;

            return MsgTypesID.Rx_UpperActionDebugData;
        }

        //处理下位机传上来的PID数据
        private MsgTypesID GetPIDData(byte[] Buffer)
        {
            PIDDataGetTypedef Rx_PIDData = new PIDDataGetTypedef();

            //将数据写入缓冲区
            Rx_PIDData = (PIDDataGetTypedef)BytesToStruct(Buffer, GetSize(typeof(Globle_Datahead)), typeof(PIDDataGetTypedef));

            //调用写入数据的方法
            if (null == PIDDataHandler)
            {
                throw new Exception("<调用PID数据处理函数>：未初始化数据处理委托！请先调用函数\"PIDDataHandlerInit\"以初始化！");
            }
            else
            {
                PIDDataHandler(Rx_PIDData);
            }

            return MsgTypesID.Rx_PIDData;
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
                new DealFunc(GetErrorData),     //接收到错误数据
                new DealFunc(GetEmptyData),     //接收到空数据
                new DealFunc(GetSampleData),    //接收到采样数据
                new DealFunc(GetBaseMotionData),//接收到基本运动数据
                new DealFunc(GetPlanTestData),  //接收到规划测试数据
                new DealFunc(GetAutoProcessData),//接收到自动流程控制数据
                new DealFunc(GetActionTestData),//上层动作测试反馈数据
                new DealFunc(GetPIDData),       //接收到下位机回传的PID数据
            };

            return;
        }

        ~UserDatas()
        {
            return;
        }
    }
}

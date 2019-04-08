

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

        //注意，C#中bool类型占四个字，而单片机中占一个字，枚举类型同理。
        //因此，需要注意把协议中涉及的逻辑变量改成byte型。

        #region 采样控制

        //反馈测试数据
        public struct SampleData
        {
            public ushort Time;         //每组参量对应的时刻
            public short InputData;    //输入参量
            public short OutputData;   //输出参量(系统输出)
            public short OutputUc;     //控制器输出
        }

        //测试控制指令
        public struct SampleCtrlCMD
        {
            public byte CMD;        //指令
            byte Reserve1;   //预留
            byte Reserve2;   //预留
            byte Reserve3;   //预留
        }

        #endregion

        #region 底盘基本运动控制

        //结构体定义：底盘基本运动遥控指令（兼容全手动操作模式）
        public struct BaseMotionCMDTypedef
        {
            //底盘速度大小（模，单位mm/s）
            public float Speed;
            //底盘速度方向（基于全局定位坐标系，单位°）
            public float SpeedDir;
            //底盘自旋角速度，从上往下看逆时针为正
            public float RotateSpeed;

            //上层控制指令                                                     
            public byte PickCMD;    //抓取指令（无动作0，先放下来开爪1，对位，再合爪归位释放2）
            public byte ShootCMD;   //发射指令(取兽骨的爪释放后自动后撤，发射完自动复位。无动作0，发射1，未完成后撤或复位时发射指令)无效)
            public byte HandoverCMD;//交接指令(无动作0，大气缸伸出1，缩回2，释放令牌3)
            byte Reserve;    //预留占位

        };

        //结构体定义：底盘基本运动状态参量
        public struct BaseMotionDataTypedef
        {
            //底盘速度大小（模，单位mm/s）
            public float Speed;
            //底盘速度方向（基于全局定位坐标系，单位°）
            public float SpeedDir;
            //底盘自旋角速度，从上往下看逆时针为正
            public float RotateSpeed;

            //返回坐标X
            public float CoordinateX;
            //返回坐标Y
            public float CoordinateY;
            //基于全局定位坐标系的姿态角
            public float PoseAngle;

        };

        #endregion

        #region 底盘规划运动测试

        //结构体定义：底盘规划运动测试指令
        public struct BasePlanCtrlCMDTypedef
        {
            //规划运动控制参量
            public byte MotionCMD;      //运动控制指令（1为开始运动，0为停止运动）
            public byte isParamsValid;  //指示本次发送下述参量是否有效
            public byte isLockHead;     //标记运动过程中是否锁头
            byte Reserve;        //预留占位符

            //规划运动输入参量
            public int CoordinateX;    //目标坐标X
            public int CoordinateY;    //目标坐标Y

            public int CircleOrigin1X; //挤压圆1圆心坐标X
            public int CircleOrigin1Y; //挤压圆1圆心坐标Y

            public int Circle1DirX;     //挤压方向1的方向向量坐标X
            public int Circle1DirY;     //挤压方向1的方向向量坐标Y

            public int CircleOrigin2X;  //挤压圆2圆心坐标X
            public int CircleOrigin2Y;  //挤压圆2圆心坐标Y

            public int Circle2DirX;     //挤压方向2的方向向量坐标X
            public int Circle2DirY;     //挤压方向2的方向向量坐标Y

        };

        //结构体定义：底盘规划运动返回数据
        public struct BasePlanDataTypedef
        {
            //规划运动返回参量
            public byte isPlanSuccessfully; //标记是否生成路径成功
            public byte SectionFlag;        //路段标志

            //运动状态参量
            public float DistanceHaveMoved; //已经走过的路程
            public float FullDistance;      //路段全长

            //返回实时坐标X
            public float CoordinateX;
            //返回实时坐标Y
            public float CoordinateY;
            //基于全局定位坐标系的姿态角
            public float PoseAngle;

        };

        #endregion

        #region MR1半/全自动流程控制

        //底盘半自动流程控制
        public struct MR1CtrlCMDTypedef
        {
            //运动干预部分
            public float Speed;        //底盘速度大小（模，单位mm/s）
            public float SpeedDir;     //底盘速度方向（基于全局定位坐标系，单位°）
            public float RotateSpeed;  //底盘自旋角速度，从上往下看逆时针为正

            //流程控制部分
            public bool isStopProcess;     //停止流程
            public bool ToNextState;       //进入流程控制中下一个状态
            public bool StateReset;        //复位状态以重新启动流程（此时还未起动）
            public byte SetCurrentState;//设置当前状态以方便从某一个流程节点处重试

            //上层控制指令                                                     
            public byte PickCMD;    //抓取指令（无动作0，先放下来开爪1，对位，再合爪归位释放2）
            public byte ShootCMD;   //发射指令(取兽骨的爪释放后自动后撤，发射完自动复位。无动作0，发射1，未完成后撤或复位时发射指令无效)
            public byte HandoverCMD;//交接指令(无动作0，大气缸伸出1，缩回2，释放令牌3)
            public byte Mode;       //指定是全自动流程0（仅使能开始流程指令和运动干预部分）还是半自动1（使能流程控制部分和上层控制指令）

        };

        //底盘半自动流程控制
        public struct MR1CtrlDataTypedef
        {
            //流程控制部分
            public byte isAutoMode;     //标记是否是全自动模式
            public byte CurrentState;   //标记当前的流程号
            public byte isActionDoing;  //标记上层动作是否在进行中
            byte Reserve;     //保留占位

            //返回实时坐标X
            public float CoordinateX;
            //返回实时坐标Y
            public float CoordinateY;
            //基于全局定位坐标系的姿态角
            public float PoseAngle;

        };

        #endregion

        #region 上层动作Debug

        //上层动作调试指令
        public struct ActionDebugCMDTypedef
        {
            public byte SetPickMotorPos;    //设定抓取电机的位置(待标定)
            public byte SetShootMotorSpeed; //设定发射电机的速度（机构向前推为负，单位mm/s）
            public byte SetSteerPos;        //设定交接舵机的角位置（0-180，单位°）
            public byte CylinderCMD;        //气缸控制指令（第0位大气缸，第1位小气缸，1为推出）

        };

        //上层动作调试信息
        public struct ActionDebugDataTypedef
        {
            public ushort PickMotorPos;    //抓取电机的位置(待标定)
            public ushort ShootMotorSpeed; //发射电机的速度（机构向前推为负，单位mm/s）

        };

        #endregion

        #region PID设置指令

        //枚举定义：PID的ID列表
        enum PID_IDList
        {
            PID_SingleMotor = 0x00,    //单电机PID

        };

        //PID设置指令
        public struct PIDDataSetTypedef
        {
            public byte isSetParam;    //标记是否是设置参数(若为假，则是向下位机索取参数)
            public byte SetTarget;     //标记被设置/索取的目标
            byte Reserve1;              //预留占位符1
            byte Reserve2;              //预留占位符1

            public float Kp;
            public float Ki;
            public float Kd;

            public float I_e_Threshold;    //决定是否使用积分的误差上限

            public float I_Max;            //积分限幅
            public float OutMax;           //输出限幅

        };

        //获取PID设置值
        public struct PIDDataGetTypedef
        {
            public byte PID_ID;     //标记所返回的目标对应的标签
            byte Reserve1;          //预留占位符1
            byte Reserve2;          //预留占位符2
            byte Reserve3;          //预留占位符3

            public float Kp;
            public float Ki;
            public float Kd;

            public float I_e_Threshold;    //决定是否使用积分的误差上限

            public float I_Max;            //积分限幅
            public float OutMax;           //输出限幅

        };

        #endregion
        #endregion

        #region 公示的状态参量

        public struct Mr1State
        {
            /*****************  通用  ******************/
            //底盘速度大小（模，单位mm/s）
            public float Speed;
            //底盘速度方向（基于全局定位坐标系，单位°）
            public float SpeedDir;
            //底盘自旋角速度，从上往下看逆时针为正
            public float RotateSpeed;

            //返回坐标X
            public float CoordinateX;
            //返回坐标Y
            public float CoordinateY;
            //基于全局定位坐标系的姿态角
            public float PoseAngle;

            /*****************  规划运动  ******************/
            //规划运动返回参量
            public bool isPlanSuccessfully; //标记是否生成路径成功
            public byte SectionFlag;     //路段标志

            //运动状态参量
            public float DistanceHaveMoved; //已经走过的路程
            public float FullDistance;      //路段全长

            /*****************  流程控制  ******************/
            public bool isAutoMode;     //标记是否是全自动模式
            public byte CurrentState;//标记当前的流程号
            public bool isActionDoing;  //标记上层动作是否在进行中

            /*****************  上层测试  ******************/
            public ushort PickMotorPos;    //抓取电机的位置(待标定)
            public ushort ShootMotorSpeed; //发射电机的速度（机构向前推为负，单位mm/s）
        };

        #endregion

        #region 常量和枚举标识

        public const byte Datahead1 = 0xaa, Datahead2 = 0xbb;

        //通信数据类型标识集合（发送和接收分别从0开始）
        public enum MsgTypesID
        {
            /***********  发送组  *************/
            //0 - 异常标识
            Tx_ErrorData = 0x00,
            //1 - 发送空指令
            Tx_EmptyCMD,
            //2 - 发送采样指令
            Tx_SampleCMD,
            //3 - 基本运动状态参量
            Tx_BaseMotionCMD,
            //4 - 规划运动状态参量
            Tx_PlanMotionTestCMD,
            //5 - 自动流程状态参量
            Tx_AutoProcessCMD,
            //6 - 上层动作debug状态参量
            Tx_UpperActionDebugCMD,
            //7 - 设置PID参数的指令
            Tx_PIDSetCMD,
            //待发送的数据的编码最大值，用做安全检查
            TxMaxCode,

            /************  接收组  *************/
            //0 - 接收到错误数据
            Rx_ErrorData = 0x00,
            //1 - 空数据指令
            Rx_EmptyData,
            //2 - 采样数据
            Rx_SampleData,
            //3 - 基本运动状态参量
            Rx_BaseMotionData,
            //4 - 规划运动状态参量
            Rx_PlanMotionTestData,
            //5 - 自动流程状态参量
            Rx_AutoProcessData,
            //6 - 上层动作debug状态参量
            Rx_UpperActionDebugData,
            //7 - PID参数返回值
            Rx_PIDData,
            //接收到的数据的编码最大值，用做安全检查
            RxMaxCode             

        };
        #endregion

        #region 数据缓冲区及其大小

        //数据头
        public Globle_Datahead Tx_Datahead = new Globle_Datahead();
        //数据尾
        public Globle_Datatail Tx_Datatail = new Globle_Datatail();
        //发送数据内容缓冲区
        public SampleCtrlCMD            Tx_TestCtrlCMD = new SampleCtrlCMD();
        public BaseMotionCMDTypedef     Tx_BaseMotionCMD = new BaseMotionCMDTypedef();
        public BasePlanCtrlCMDTypedef   Tx_PlanCtrlCMD = new BasePlanCtrlCMDTypedef();
        public MR1CtrlCMDTypedef        Tx_MR1AutoCtrlCMD = new MR1CtrlCMDTypedef();
        public ActionDebugCMDTypedef    Tx_ActionDebugCMD = new ActionDebugCMDTypedef();
        public PIDDataSetTypedef        Tx_PIDSetCMD = new PIDDataSetTypedef();
        //接收到的状态参量公示(未公示的接收参量将在接收回调中得到处理)
        public SampleData               Rx_SampleData = new SampleData(); //此状态量也会在回调中被处理
        public Mr1State                 Rx_MR1State = new Mr1State();

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
                + GetMaxSize(   typeof(SampleData), 
                                typeof(BaseMotionDataTypedef),
                                typeof(BasePlanDataTypedef),
                                typeof(MR1CtrlDataTypedef),
                                typeof(ActionDebugDataTypedef),
                                typeof(PIDDataGetTypedef));

            //初始化通用发送缓冲区长度
            MaxTxBufferSize = GetSize(typeof(Globle_Datahead))
                + GetSize(typeof(Globle_Datatail))
                + GetMaxSize(   typeof(SampleCtrlCMD),
                                typeof(BaseMotionCMDTypedef),
                                typeof(BasePlanCtrlCMDTypedef),
                                typeof(MR1CtrlCMDTypedef),
                                typeof(ActionDebugCMDTypedef),
                                typeof(PIDDataSetTypedef));
        }

    }
}

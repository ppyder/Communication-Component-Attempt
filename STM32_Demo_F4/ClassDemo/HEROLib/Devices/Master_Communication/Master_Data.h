#ifndef MASTER_DATA_H
#define MASTER_DATA_H

//此文件中定义用户通信相关的数据类型，各类型要求大小为整型变量的整数倍
//并约定：0号指令为空指令或指示错误的指令

#include <stdint.h>
#include <stdbool.h>

//结构体定义：通用数据头
typedef struct
{
    uint8_t DataHead1;  //固定数据头1:0xaa
    uint8_t DataHead2;  //固定数据头2:0xbb
    uint8_t MsgTypeID;  //数据类型标识符：指明数据类型
    uint8_t IDFeedback; //向上位机反馈刚刚所接收到的指令类型
    
    uint32_t Verification;  //用于向上位机标识头身份的头校验（相乘校验）
    
}MstrData_DataHead; 

//结构体定义：通用数据尾
typedef struct
{
    uint32_t Verification;  //校验，此时为求和
    
}MstrData_DataTail; 

/***************  以下定义用户数据内容  ********************/

//枚举定义：发送数据类型ID
enum Mstr_TxMsgTypes
{
    //0 - 接收指令错误
    MstrTx_ErrorData = 0,
    
    //1 - 空指令反馈
    MstrTx_EmptyData,
    
    //2 - 采样数据
    MstrTx_SampleData,
    
    //3 - 基本运动状态参量
    MstrTx_BaseMotionData,
    
    //4 - 规划运动状态参量
    MstrTx_PlanMotionTestData,
    
    //5 - 自动流程状态参量
    MstrTx_AutoProcessData,
    
    //6 - 上层动作debug状态参量
    MstrTx_UpperActionDebugData,
    
    //7 - PID参数返回值
    MstrTx_PIDData,
    
    //发送ID的数值上限（辅助合理性校验和数组定义等）
    MstrTx_Num
};

//枚举定义：接收数据类型ID
enum MstrRxMsgTypes
{
    //0 - 接收指令错误
    MstrRx_ErrorCMD = 0,
    
    //1 - 空指令
    MstrRx_EmptyCMD,
    
    //2 - 采样指令
    MstrRx_SampleCMD,
    
    //3 - 基本运动状态参量
    MstrRx_BaseMotionCMD,
    
    //4 - 规划运动状态参量
    MstrRx_PlanMotionTestCMD,
    
    //5 - 自动流程状态参量
    MstrRx_AutoProcessCMD,
    
    //6 - 上层动作debug状态参量
    MstrRx_UpperActionDebugCMD,
    
    //7 - 设置PID参数的指令
    MstrRx_PIDSetCMD,
            
    //接收ID的数值上限（辅助合理性校验和数组定义等）
    MstrRx_Num
};

/****************************  数据1 采样指令与数据  ********************************/

//结构体定义：采样数据包
typedef struct
{
    uint16_t Time_10ms;             //时刻
    int16_t Input;                  //激励
    int16_t Output;                 //响应
    int16_t OutputUc;               //控制器输出
    
}SampleDataTypedef;
 
//结构体定义：采样控制指令
typedef struct
{
    uint8_t CMD;                    //指令
    uint8_t Reserve1;               //预留
    uint8_t Reserve2;               //预留
    uint8_t Reserve3;               //预留
    
}SampleCMDTypedef;

/****************************  END -> 采样指令与数据  ********************************/

/****************************  数据2 底盘基本运动指令与数据  ********************************/

//结构体定义：底盘基本运动遥控指令（兼容全手动操作模式）
typedef struct
{
    //底盘速度大小（模，单位mm/s）
    float Speed;
    //底盘速度方向（基于全局定位坐标系，单位°）
    float SpeedDir;
    //底盘自旋角速度，从上往下看逆时针为正
    float RotateSpeed;
    
    //上层控制指令                                                     
    uint8_t PickCMD;    //抓取指令（无动作0，先放下来开爪1，对位，再合爪归位释放2）
    uint8_t ShootCMD;   //发射指令(取兽骨的爪释放后自动后撤，发射完自动复位。无动作0，发射1，未完成后撤或复位时发射指令)无效)
    uint8_t HandoverCMD;//交接指令(无动作0，大气缸伸出1，缩回2，释放令牌3)
    uint8_t Reserve;    //预留占位
    
}BaseMotionCMDTypedef;

//结构体定义：底盘基本运动状态参量
typedef struct
{
    //底盘速度大小（模，单位mm/s）
    float Speed;
    //底盘速度方向（基于全局定位坐标系，单位°）
    float SpeedDir;
    //底盘自旋角速度，从上往下看逆时针为正
    float RotateSpeed;
    
    //返回坐标X
    float CoordinateX;
    //返回坐标Y
    float CoordinateY;
    //基于全局定位坐标系的姿态角
    float PoseAngle;
    
}BaseMotionDataTypedef;

/****************************  END -> 底盘基本运动指令与数据  ********************************/

/****************************  数据3 规划运动测试指令与数据  ********************************/

//结构体定义：底盘规划运动测试指令
typedef struct
{
    //规划运动控制参量
    uint8_t MotionCMD;      //运动控制指令（1为开始运动，0为停止运动）
    bool    isParamsValid;  //指示本次发送下述参量是否有效
    bool    isLockHead;     //标记运动过程中是否锁头
    uint8_t Reserve;        //预留占位符
    
    //规划运动输入参量
    int32_t CoordinateX;    //目标坐标X
    int32_t CoordinateY;    //目标坐标Y
    
    int32_t CircleOrigin1X; //挤压圆1圆心坐标X
    int32_t CircleOrigin1Y; //挤压圆1圆心坐标Y
    
    int32_t Circle1DirX;     //挤压方向1的方向向量坐标X
    int32_t Circle1DirY;     //挤压方向1的方向向量坐标Y
    
    int32_t CircleOrigin2X;  //挤压圆2圆心坐标X
    int32_t CircleOrigin2Y;  //挤压圆2圆心坐标Y
    
    int32_t Circle2DirX;     //挤压方向2的方向向量坐标X
    int32_t Circle2DirY;     //挤压方向2的方向向量坐标Y
    
}BasePlanCtrlCMDTypedef;

//结构体定义：底盘规划运动返回数据
typedef struct
{
    //规划运动返回参量
    bool isPlanSuccessfully; //标记是否生成路径成功
    uint8_t SectionFlag;     //路段标志
    uint8_t Reserve[2];      //预留占位符
    
    //运动状态参量
    float DistanceHaveMoved; //已经走过的路程
    float FullDistance;      //路段全长
    
    //返回实时坐标X
    float CoordinateX;
    //返回实时坐标Y
    float CoordinateY;
    //基于全局定位坐标系的姿态角
    float PoseAngle;
    
}BasePlanDataTypedef;

/****************************  END -> 规划运动测试指令与数据  ********************************/

/****************************  数据4 自动流程控制指令与数据  ********************************/

//底盘半自动流程控制
typedef struct
{
    //运动干预部分
    float Speed;        //底盘速度大小（模，单位mm/s）
    float SpeedDir;     //底盘速度方向（基于全局定位坐标系，单位°）
    float RotateSpeed;  //底盘自旋角速度，从上往下看逆时针为正
    
    //流程控制部分
    bool isStopProcess;     //停止流程
    bool ToNextState;       //进入流程控制中下一个状态
    bool StateReset;        //复位状态以重新启动流程（此时还未起动）
    uint8_t SetCurrentState;//设置当前状态以方便从某一个流程节点处重试
    
    //上层控制指令                                                     
    uint8_t PickCMD;    //抓取指令（无动作0，先放下来开爪1，对位，再合爪归位释放2）
    uint8_t ShootCMD;   //发射指令(取兽骨的爪释放后自动后撤，发射完自动复位。无动作0，发射1，未完成后撤或复位时发射指令无效)
    uint8_t HandoverCMD;//交接指令(无动作0，大气缸伸出1，缩回2，释放令牌3)
    uint8_t Mode;       //指定是全自动流程0（仅使能开始流程指令和运动干预部分）还是半自动1（使能流程控制部分和上层控制指令）
    
}MR1CtrlCMDTypedef;

//底盘半自动流程控制
typedef struct
{
    //流程控制部分
    bool isAutoMode;     //标记是否是全自动模式
    uint8_t CurrentState;//标记当前的流程号
    bool isActionDoing;  //标记上层动作是否在进行中
    uint8_t Reserve;     //保留占位
    
    //返回实时坐标X
    float CoordinateX;
    //返回实时坐标Y
    float CoordinateY;
    //基于全局定位坐标系的姿态角
    float PoseAngle;
    
}MR1CtrlDataTypedef;

/****************************  END -> 自动流程控制指令与数据  ********************************/

/****************************  数据5 上层动作调试指令与数据  ********************************/

//上层动作调试指令
typedef struct
{
    uint8_t SetSteerPos;        //设定交接舵机的角位置（0-180，单位°）
    uint8_t CylinderCMD;        //气缸控制指令（第0位大气缸，第1位小气缸，1为推出）
    uint8_t SetHandoverMotorPos; //设定交接电机的位置（0-180，单位°）
    uint8_t Reserve;             //预留占位符

    int16_t SetPickMotorPos;    //设定抓取电机的位置(待标定)
    int16_t SetShootMotorSpeed; //设定发射电机的速度（机构向前推为负，单位mm/s）
    
}ActionDebugCMDTypedef;

//上层动作调试信息
typedef struct
{
    uint16_t HandoverMotorPos; //交接电机的位置（（0-180，单位°）
    uint16_t HandoverSteerPos; //交接舵机的位置（（0-180，单位°）
    
    int16_t PickMotorPos;    //抓取电机的位置(待标定)
    int16_t ShootMotorSpeed; //发射电机的速度（机构向前推为负，单位mm/s）
    
}ActionDebugDataTypedef;

/****************************  END -> 上层动作调试指令与数据  ********************************/

/****************************  数据6 PID参数设置指令  ********************************/

//枚举定义：PID的ID列表
typedef enum
{
    PID_SingleMotor = 0x00,     //单电机转速PID
    PID_SingleMotorAngle,       //单电机位置PID
    PID_RobAccelerated,         //整车加速度PID
    PID_RobLocation,            //整车位置PID
    PID_RobEnd,                 //整车终点PID
    PID_RobTime,                //整车时间PID

    PID_PickAngle,          //抓取角度PID
    PID_PickSpeed,          //抓取速度PID
    PID_LaunchAngle,        //发射角度PID
    PID_LaunchSpeed,        //发射速度PID
    
}PID_IDList;

//PID设置指令
typedef struct
{
    bool isSetParam;    //标记是否是设置参数(若为假，则是向下位机索取参数)
    uint8_t SetTarget;  //标记被设置/索取的目标
    uint8_t Reserve[2]; //预留占位符
    
    float Kp;
    float Ki;
    float Kd;
    
    float I_e_Threshold;    //决定是否使用积分的误差上限
    
    float I_Max;            //积分限幅
    float OutMax;           //输出限幅
    
}PIDDataSetTypedef;

//获取PID设置值
typedef struct
{
    PID_IDList PID_ID;     //标记所返回的目标对应的标签
    uint8_t    Reserve[3]; //预留占位符
    
    float Kp;
    float Ki;
    float Kd;
    
    float I_e_Threshold;    //决定是否使用积分的误差上限
    
    float I_Max;            //积分限幅
    float OutMax;           //输出限幅
    
}PIDDataGetTypedef;

/****************************  END -> PID参数设置指令  ********************************/

#endif

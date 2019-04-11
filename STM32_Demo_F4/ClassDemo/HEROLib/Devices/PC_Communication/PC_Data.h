#ifndef PC_DATA_H
#define PC_DATA_H

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
    uint8_t Reserve;    //预留占位符使得该结构体大小为4字节的整数倍
    
}PCData_DataHead; 

//结构体定义：通用数据尾
typedef struct
{
    uint32_t Verification;  //校验，此时为求和
    
}PCData_DataTail; 

/***************  以下定义用户数据内容  ********************/

 //!!!注意：坐标系和正方向定义可以另行商量

//枚举定义：发送数据类型ID
enum PC_TxMsgTypes
{
    //循线模式
    PCTx_FollowMode = 0,
    
    //十字模式
    PCTx_CrossMode,
    
    //抓取模式
    PCTx_PickMode,
    
    //爬坡越障模式
    PCTx_ObstacleMode,
    
    //发送ID的数值上限（辅助合理性校验和数组定义等）
    PCTx_Num
};

//枚举定义：接收数据类型ID
enum PC_RxMsgTypes
{
    //循线模式
    PCRx_FollowMode = 0,
    
    //十字模式
    PCRx_CrossMode,
    
    //抓取模式
    PCRx_PickMode,
        
    //接收ID的数值上限（辅助合理性校验和数组定义等）
    PCRx_Num
};

//结构体定义：巡线数据
typedef struct
{
    float Deflection;   //底盘姿态角偏差
    float Distance;     //与被辨识线的位置偏差（有正负）
    
}FollowLineType; 


//结构体定义：十字数据
typedef struct
{
    float Deflection;   //底盘姿态角偏差
    float AxisX;        //十字中心在相对坐标系中的坐标X
    float AxisY;        //十字中心在相对坐标系中的坐标Y
    
}CrossDataType; 

//结构体定义：抓取对接数据
typedef struct
{
    float Argument;     //偏差向量辐角
    float Modulus;      //偏差向量模
    float Deflection;   //底盘姿态角偏差
    
}PickDataType; 

#endif

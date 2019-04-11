#ifndef BSP_DATA_H
#define BSP_DATA_H

//此文件中定义用户通信相关的数据类型，各类型要求大小为整型变量的整数倍
#include <stdint.h>
#include <stdbool.h>

//结构体定义：通用数据头
typedef struct
{
    uint8_t DataHead1;  //固定数据头1:0xaa
    uint8_t DataHead2;  //固定数据头2:0xbb
    uint8_t MsgTypeID;  //数据类型标识符：指明数据类型
    uint8_t FeedbackID; //向对方反馈刚刚所接收到的指令类型
}BSP_DataHead; 

//结构体定义：通用数据尾
typedef struct
{
    uint32_t Verification;  //校验，此时为求和
    
}BSP_DataTail; 

/***************  以下定义用户数据内容  ********************/

//枚举定义：发送数据类型ID
enum BSP_TxMsgTypes
{
    //0 - 接收指令错误
    BSPTx_ErrorCMD = 0,
    
    //1 - 空指令
    BSPTx_EmptyCMD,
    
    //2 - 测试指令
    BSPTx_TestCMD,

    //发送ID的数值上限（辅助合理性校验和数组定义等）
    BSPTx_Num
};

//枚举定义：接收数据类型ID
enum BSP_RxMsgTypes
{
    //0 - 接收指令错误
    BSPRx_ErrorData = 0,
    
    //1 - 空指令反馈
    BSPRx_EmptyData,
    
    //2 - 测试数据
    BSPRx_TestData,
    
    //接收ID的数值上限（辅助合理性校验和数组定义等）
    BSPRx_Num
};

//结构体定义：测试数据反馈
typedef struct
{
    float Data;   //测试数据反馈
    
}TestDataType; 

//结构体定义：测试控制指令
typedef struct
{
    float CMD;   //测试指令
    
}TestCMDType; 

#endif

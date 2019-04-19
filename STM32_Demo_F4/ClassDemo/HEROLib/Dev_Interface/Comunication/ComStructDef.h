#ifndef COMSTRUCTDEF_H
#define COMSTRUCTDEF_H

#include <stdbool.h>
#include <stdint.h>

#include "stm32f4xx_hal.h"

//针对组件使用过程中的错误进行集中管理
typedef enum 
{
    //无错误
    COM_NoError = 0,
    
    //未初始化通信组件
    COM_Error_UnInited,
    
    //未在通信组件映射函数中找到对应的组件标号
    COM_Error_UnFindModule,
    
    COM_ErrorCodeNum
    
}COM_ErrorCode;

//这里定义了通信组件结构体以及通信载体枚举

typedef enum
{
    SPPRTR_UART,    //通信支持（supporter） - UART
    SPPRTR_CAN,     //通信支持（supporter） - CAN
    
}COM_Supporter;

//通信组件结构体定义
typedef struct
{
    /***************  外设相关  ****************/
    
    //建立外设关联 - UART
    UART_HandleTypeDef *UartHandle;
    
    //建立外设关联 - CAN
    CAN_HandleTypeDef  *CanHandle;
    
    //标记所选取的通信载体
    COM_Supporter   COM_type;
    
    /*************  END -> 外设相关  ************/
    
    /***************  异常处理  ****************/
    //错误码
    COM_ErrorCode ErrorCode;
    
    //对本组件异常的文字性描述
    char* ErrorDescription;
    
    /*************  END -> 异常处理  ************/
    
    /***************  通信组件通用  ****************/
    //标记本组件是否被初始化过，用以做安全保护，不然极易因访问禁止区域而进入HardFault。
    bool isInited;
    
    //接收缓冲区地址(预留双缓冲)
    void *pRxBuffer[2];
    
    //接收缓冲区标志（指示当前正在接收数据的缓冲区标号）
    uint8_t RxBufFlag;
    
    //发送缓冲区地址
    void *pTxBuffer;
    
    //发送缓冲区大小
    int32_t TxBufSize;
    
    //数据处理方法(函数指针)
    bool (*DealData)(void *pRxBuffer);
    
    //数据发送方法(函数指针)
    bool (*SendData)(uint8_t SendCMD);
    
    //发送次数计数器
    uint32_t SendCnt;
    
    //数据包计数器（正确）
    uint32_t RxPackRcvCnt;
    
    //接收到的错误包计数器
    uint32_t RxErrorPackCnt;
    
    /*********  END -> 通信组件通用  *********/
    
    /************  串口通信专用  **************/
    
    //当前的接收缓冲区长度
    int32_t RxBufSize;
    
    //下次应当接受的数据长度（定长缓冲区数据头对齐）
    int32_t Rx_NextRcvLength;
    
    //标志是否正在进行矫正数据头操作
    bool isCorrectHead;
    
    /*********  END -> 串口通信专用  *********/

    /************  CAN通信专用  **************/
    
    //CAN的发送数据帧头
    CAN_TxHeaderTypeDef *pTxHeader;
    
    //CAN的接收数据帧头
    CAN_RxHeaderTypeDef *pRxHeader;
    
    //标记是否正在接收非头数据(标记是否使用头校验函数)
    bool IsRcvingUnhead;
    
    //CAN判断某一帧数据是否为数据包的头的方法(函数指针)
    bool (*Can_IsHeadData)(void *pData, uint32_t DataLength);
    
    //接收数据长度计数器
    uint32_t RxlengthCnt;
    
    /*********  END -> CAN通信专用  *********/
    
}COMInfoTypedef;

#endif

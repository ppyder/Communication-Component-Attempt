#ifndef COMTOBSP_H
#define COMTOBSP_H

#include "BSP_Data.h"
#include "Com_Functions.h"
#include "Half_Duplex.h"

#define USING_BSP_UART
//#define USING_BSP_CAN

//结构体定义：接收数据缓冲区
typedef struct
{
    //数据头
    BSP_DataHead Head;
    
    //数据空间
    union
    {
          TestDataType  Test;
    }Data;
    
    //数据尾
    BSP_DataTail Tail;
    
}BSP_RxBufTypedef; 

//结构体定义：发送数据缓冲区
typedef struct
{
    //数据头
    BSP_DataHead Head;
    
    //数据空间
    union
    {
          TestCMDType  Test;
    }Data;
    
    //数据尾
    BSP_DataTail Tail;
    
}BSP_TxBufTypedef; 

#define BSP_RX_BUFFERSIZE sizeof(BSP_RxBufTypedef)
#define BSP_REQUEST_ERROR_MAX 0

//如果使用板间串口
#if defined(USING_BSP_UART)

#define BSP_COM_UART  USART2    //这里是范例，待改
#define BSP_COM_HUART (huart2)

//如果使用板间CAN
#elif defined(USING_BSP_CAN)

#define BSP_COM_CAN  CAN1
#define BSP_COM_HCAN (hcan1)

#endif /*  IS_USING_BSP_CAN  */

extern Hf_DuplexTypedef BSP_HfCOM ;

//与辅控的通信组件操作接口（使用之前一定要初始化！！！）
extern COMInfoTypedef BSP_COM_Module;

//初始化BSP通信组件的模块初始化函数
void BSP_COM_ModuleInit(COMInfoTypedef *pModule);

#endif




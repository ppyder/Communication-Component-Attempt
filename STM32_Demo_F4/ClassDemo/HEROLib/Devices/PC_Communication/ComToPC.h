#ifndef COMTOPC_H
#define COMTOPC_H

#include "PC_Data.h"
#include "Comunication.h"

//结构体定义：接收数据缓冲区
typedef struct
{
    //数据头
    PCData_DataHead Head;
    
    //数据空间
    union
    {
        FollowLineType  Line;   //循线
        CrossDataType   Cross;  //十字
        PickDataType    Pick;   //拾取
    }Data;
    
    //数据尾
    PCData_DataTail Tail;
    
}PC_RxBufTypedef; 

//结构体定义：发送数据缓冲区
typedef struct
{
    //数据头
    PCData_DataHead Head;
    
    //数据尾
    PCData_DataTail Tail;
    
}PC_TxBufTypedef; 

#define PC_COM_UART USART2
#define PC_COM_HUART (huart2)
#define PC_RX_BUFFERSIZE sizeof(PC_RxBufTypedef)

//与小电脑的通信组件操作接口（使用之前一定要初始化！！！）
extern COMInfoTypedef PC_COM_Module;

//获取到的数据公示
extern FollowLineType  LineData;
extern CrossDataType   CrossData;
extern PickDataType    PickData;

//初始化PC通信组件的模块初始化函数
void PC_COM_ModuleInit(volatile COMInfoTypedef *pModule);

#endif

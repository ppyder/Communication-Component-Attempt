#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "ComStructDef.h"
#include "usart.h"
#include "can.h"

/*  通信组件接口，使用双缓冲区接收，单缓冲区发送，支持定长数据接收，定长/不定长数据发送  */

//组件实体列表，对应于顺序指针数组赋值，便于管理
typedef enum 
{
    //小电脑通信组件
    COM_PC = 0x00,
    
    //控制端通信组件
    COM_MSTR,
    
    //板间通信组件
    COM_BSP,
    
    COM_ModulesNum
    
}COM_ModuleID;

//空指令，用于作为各种通信组件的指令占位
#define NULL_MESSAGE 0

//便于集中管理的通信组件集合
extern COMInfoTypedef *COM_Modules[COM_ModulesNum];

//CAN接收滤波器初始化函数
void CAN_Filter_Config(CAN_HandleTypeDef* hcan);

//数据发送函数
bool COM_SendData(COMInfoTypedef *pModule);
bool COM_SendUnformatData(COMInfoTypedef *pModule, uint8_t *pData, uint32_t DataSzie);
bool COM_CANSendUnformatData(COMInfoTypedef *pModule, CAN_TxHeaderTypeDef *pHeader, uint8_t *pData, uint16_t Size);

#endif

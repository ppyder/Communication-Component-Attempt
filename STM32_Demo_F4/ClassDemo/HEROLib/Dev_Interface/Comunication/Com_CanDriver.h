#ifndef COM_CANDRIVER_H
#define COM_CANDRIVER_H

#include <stdbool.h>

#include "Comunication.h"

//针对can的数据类型映射，包含格式化数据和非格式化数据
typedef enum 
{
    //自定义通协议数据帧
    CAN_FormatData,
    
    //M3508返回数据1
    CAN_M3508Data1,
    
    //M3508返回数据2
    CAN_M3508Data2,
    
    //M3508返回数据3
    CAN_M3508Data3,
    
    CAN_IDsNum
    
}CAN_DataID;

CAN_DataID CAN_StdIDMap(CAN_RxHeaderTypeDef *pHeader);
void RcvFormatCanData(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type);
bool Can_SendData(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *pHeader, uint8_t *pData, uint16_t Size);

#endif

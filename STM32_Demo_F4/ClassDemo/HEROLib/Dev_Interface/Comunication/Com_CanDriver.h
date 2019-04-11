#ifndef COM_CANDRIVER_H
#define COM_CANDRIVER_H

#include <stdbool.h>

#include "Comunication.h"

//���can����������ӳ�䣬������ʽ�����ݺͷǸ�ʽ������
typedef enum 
{
    //�Զ���ͨЭ������֡
    CAN_FormatData,
    
    //M3508��������1
    CAN_M3508Data1,
    
    //M3508��������2
    CAN_M3508Data2,
    
    //M3508��������3
    CAN_M3508Data3,
    
    CAN_IDsNum
    
}CAN_DataID;

CAN_DataID CAN_StdIDMap(CAN_RxHeaderTypeDef *pHeader);
void RcvFormatCanData(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type);
bool Can_SendData(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *pHeader, uint8_t *pData, uint16_t Size);

#endif

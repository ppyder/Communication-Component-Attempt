#ifndef COM_CANDRIVER_H
#define COM_CANDRIVER_H

#include <stdbool.h>

#include "Com_Functions.h"
#include "Com_UserModules.h"

//����CAN��Ϣ�ķ�������
extern void (*pCANDataDealFuncs[CAN_IDsNum])(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type);

//CAN��ص��շ�����
void RcvUnformatCanData(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type);
void RcvFormatCanData(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type);
bool Can_SendData(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *pHeader, uint8_t *pData, uint16_t Size);

#endif

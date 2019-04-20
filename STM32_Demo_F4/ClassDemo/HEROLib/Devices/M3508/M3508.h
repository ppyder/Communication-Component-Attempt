#ifndef _M3508_H_
#define _M3508_H_

#include "stm32f4xx_hal.h"
#include "Com_Functions.h"

extern short Real_C_Value[3];      //�����ʵ����ֵ
extern short Real_V_Value[3];      //�����ʵ�ٶ�ֵ
extern long Real_A_Value[3];      //�����ʵ�Ƕ�ֵ

typedef enum
{
	CAN_3510Moto1_ID = 0x201,
	CAN_3510Moto2_ID = 0x202,
	CAN_3510Moto3_ID = 0x203,
	CAN_3510Moto4_ID = 0x204,
}CAN_Message_ID;

//��CAN��ͨ����������ӿ��������൱��3508�ǹ�����BSPͨ������½����շ���
extern COMInfoTypedef BSP_COM_Module;

void M3508SetCurrent(uint16_t id, int16_t Current1, int16_t Current2, int16_t Current3, int16_t Current4);

void Motor1DataDeal(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type);
void Motor2DataDeal(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type);
void Motor3DataDeal(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type);

#endif 








#ifndef _M3508_H_
#define _M3508_H_

#include "stm32f4xx_hal.h"
#include "ComToCAN_Dev.h"

extern short Real_C_Value[3];      //�����ʵ����ֵ
extern short Real_V_Value[3];      //�����ʵ�ٶ�ֵ
extern short Real_A_Value[3];      //�����ʵ�Ƕ�ֵ

void M3508SetCurrent(uint16_t id, int16_t Current1, int16_t Current2, int16_t Current3, int16_t Current4);

//������ݴ�����
void Motor1DataDeal(uint8_t *pData);
void Motor2DataDeal(uint8_t *pData);
void Motor3DataDeal(uint8_t *pData);

#endif 








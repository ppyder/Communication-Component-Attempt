#ifndef _M3508_H_
#define _M3508_H_

#include "stm32f4xx_hal.h"
#include "ComToCAN_Dev.h"

extern short Real_C_Value[3];      //电机真实电流值
extern short Real_V_Value[3];      //电机真实速度值
extern short Real_A_Value[3];      //电机真实角度值

void M3508SetCurrent(uint16_t id, int16_t Current1, int16_t Current2, int16_t Current3, int16_t Current4);

//电机数据处理函数
void Motor1DataDeal(uint8_t *pData);
void Motor2DataDeal(uint8_t *pData);
void Motor3DataDeal(uint8_t *pData);

#endif 








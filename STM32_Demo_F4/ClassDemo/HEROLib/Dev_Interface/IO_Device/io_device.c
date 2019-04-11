/**
 ******************************************************************************
 * @file    io_device.c
 * @brief   板级IO外设接口：中间层与底层的抽象接口，包含注册和操作函数.
 ******************************************************************************
 *
 * Copyright (C) HITwh Excellent Robot Organization(HERO). 2015-2018. All rights reserved.
 *
 * @version 1.0 示例版本
 * @author  杨亦凡
 * @contact 17863107058(手机)   942041771(qq)
 * @date    2018/10/10
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "io_device.h"

/**
 * @brief	    注册一个输出设备
 * @param[out]  pDevice		指向输出设备的指针
 * @param[in]   ID			该设备的编号（句柄）
 * @param[in]   Name		对该设备的文字性描述
 * @param[in]   pIOStruct	指向该设备对应IO实体的指针
 * @retval	    是否成功注册(bool)
 */
_Bool RegisterIO_OutDevice(IO_OutDev* pDevice, uint32_t ID, char* Name, IO_Out* pIOStruct)
{
	if(pDevice == 0)
	{
	  return false;
	}
	
	//初始化数据结构体
	pDevice -> Basic.ID = ID;
	pDevice -> Basic.Name = Name;
	
	pDevice -> pCtrlPin = pIOStruct;
	
	return true;
}

/**
 * @brief	    注册一个输入设备
 * @param[out]  pDevice		指向输出设备的指针
 * @param[in]   ID			该设备的编号（句柄）
 * @param[in]   Name		对该设备的文字性描述
 * @param[in]   pIOStruct	指向该设备对应IO实体的指针
 * @retval	    是否成功注册(bool)
 */
bool RegisterIO_InDevice(IO_InDev* pDevice, uint32_t ID, char* Name, IO_In* pIOStruct)
{
	if(pDevice == 0)
	{
	  return false;
	}
	
	//初始化数据结构体
	pDevice -> Basic.ID = ID;
	pDevice -> Basic.Name = Name;
	
	pDevice -> pReadPin = pIOStruct;
	
	return true;
}

/**
 * @brief	    输出设备动作修改
 * @param[in]   pDevice		指向输出设备的指针
 * @param[in]   NewState	期望输出的状态
 * @retval	    None
 */
void IO_OutDevCtrl(IO_OutDev* pDevice, GPIO_PinState NewState)
{
	*(pDevice->pCtrlPin->Operator) = NewState;
}

/**
 * @brief	    获取输入设备引脚瞬时电平状态(未滤波)
 * @param[in]   pDevice		指向输出设备的指针
 * @retval	    该电平在此时刻的电平值
 */
uint8_t IO_InDevRead(IO_InDev* pDevice)
{
	return *(pDevice->pReadPin->Operator);
}

/************************ (C) COPYRIGHT HITwh Excellent Robot Organization(HERO). *****END OF FILE****/

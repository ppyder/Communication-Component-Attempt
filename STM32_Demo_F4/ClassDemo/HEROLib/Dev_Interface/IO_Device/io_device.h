/**
 ******************************************************************************
 * @file    io_device.h
 * @brief   ������ϵ�м����ײ��IO����ĳ���ӿڣ�������صĺꡢ���ݽṹ�ͺ�������.
 ******************************************************************************
 *
 * Copyright (C) HITwh Excellent Robot Organization(HERO). 2015-2018. All rights reserved.
 *
 * @version 1.0 ʾ���汾
 * @author  ���ෲ
 * @contact 17863107058(�ֻ�)   942041771(qq)
 * @date    2018/10/10
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _IO_DEVICE_H
#define _IO_DEVICE_H

/* Includes ------------------------------------------------------------------*/
#include "bsp_gpio.h"

/** @addtogroup HeroLibrary HERO�ŶӴ����
  * @{
  */

/** @addtogroup Low_Level_Library Low Level Library(�ײ��)
  * @{
  */

/** @addtogroup Interface_To_MiddleTier Interface(���м��ĳ���ӿ�)
  * @{
  */

/** @addtogroup Interface_IO_Device IO���Ƶ��豸
  * @{
  */
  
/* Exported types ------------------------------------------------------------*/
/** @defgroup IO_Device_Exported_Types IO_Device��������
  * @{
  */

/**
  * @brief �������IO�豸 �ṹ�嶨��
  */
typedef struct 
{
	BasicMsg Basic;		//!< ������Ϣ
	
	IO_Out* pCtrlPin;   //!< ָ�����IOʵ���ָ��
	
}IO_OutDev;

/**
  * @brief ��������IO�豸 �ṹ�嶨��
  */
typedef struct
{
	BasicMsg Basic;		//!< ������Ϣ
	
	IO_In* pReadPin;    //!< ָ������IOʵ���ָ��
	
}IO_InDev;
/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup IO_Device_Exported_Macros IO_Device��
  * @{
  */

/** @defgroup IO_DeviceAction_Group1 IO�����ı���_���� 
  * @{
  */

#define Cylinder_Push	GPIO_PIN_SET 
#define Cylinder_Pull	GPIO_PIN_RESET
/**
  * @}
  */

/** @defgroup IO_Device_Action_Group2 IO�����ı���_LED
  * @{
  */

#define LED_ON	GPIO_PIN_SET 
#define LED_OFF	GPIO_PIN_RESET
/**
  * @}
  */
  
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup IO_Device_Register_Functions IO_Deviceע�ắ��
  * @{
  */
/* ע�ắ�� *****************************/
bool RegisterIO_OutDevice(IO_OutDev* pDevice, uint32_t ID, char* Name, IO_Out* pIOStruct);
bool RegisterIO_InDevice(IO_InDev* pDevice, uint32_t ID, char* Name, IO_In* pIOStruct);/**
  * @}
  */

/** @addtogroup IO_Device_Operate_Functions IO_Device��������
  * @{
  */
/* �������� *****************************/
void IO_OutDevCtrl(IO_OutDev* pDevice, GPIO_PinState NewState);
uint8_t IO_InDevRead(IO_InDev* pDevice);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* __io_device_H */

/************************ (C) COPYRIGHT HITwh Excellent Robot Organization(HERO). *****END OF FILE****/

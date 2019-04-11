/**
 ******************************************************************************
 * @file    io_device.h
 * @brief   包含联系中间层与底层的IO外设的抽象接口，及其相关的宏、数据结构和函数声明.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _IO_DEVICE_H
#define _IO_DEVICE_H

/* Includes ------------------------------------------------------------------*/
#include "bsp_gpio.h"

/** @addtogroup HeroLibrary HERO团队代码库
  * @{
  */

/** @addtogroup Low_Level_Library Low Level Library(底层库)
  * @{
  */

/** @addtogroup Interface_To_MiddleTier Interface(与中间层的抽象接口)
  * @{
  */

/** @addtogroup Interface_IO_Device IO控制的设备
  * @{
  */
  
/* Exported types ------------------------------------------------------------*/
/** @defgroup IO_Device_Exported_Types IO_Device数据类型
  * @{
  */

/**
  * @brief 输出类型IO设备 结构体定义
  */
typedef struct 
{
	BasicMsg Basic;		//!< 基本信息
	
	IO_Out* pCtrlPin;   //!< 指向输出IO实体的指针
	
}IO_OutDev;

/**
  * @brief 输入类型IO设备 结构体定义
  */
typedef struct
{
	BasicMsg Basic;		//!< 基本信息
	
	IO_In* pReadPin;    //!< 指向输入IO实体的指针
	
}IO_InDev;
/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup IO_Device_Exported_Macros IO_Device宏
  * @{
  */

/** @defgroup IO_DeviceAction_Group1 IO操作的别名_气缸 
  * @{
  */

#define Cylinder_Push	GPIO_PIN_SET 
#define Cylinder_Pull	GPIO_PIN_RESET
/**
  * @}
  */

/** @defgroup IO_Device_Action_Group2 IO操作的别名_LED
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
/** @addtogroup IO_Device_Register_Functions IO_Device注册函数
  * @{
  */
/* 注册函数 *****************************/
bool RegisterIO_OutDevice(IO_OutDev* pDevice, uint32_t ID, char* Name, IO_Out* pIOStruct);
bool RegisterIO_InDevice(IO_InDev* pDevice, uint32_t ID, char* Name, IO_In* pIOStruct);/**
  * @}
  */

/** @addtogroup IO_Device_Operate_Functions IO_Device操作函数
  * @{
  */
/* 操作函数 *****************************/
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

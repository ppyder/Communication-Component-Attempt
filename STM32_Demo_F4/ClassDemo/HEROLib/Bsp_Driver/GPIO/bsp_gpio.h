/**
 ******************************************************************************
 * @file    bsp_gpio.h
 * @brief   包含板级IO相关的宏、数据结构和函数声明.
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
#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

/* Includes ------------------------------------------------------------------*/
#include "io_bit_strip_F103.h"
#include "base_class.h"
#include "filter.h"
#include "macros.h"

/** @addtogroup HeroLibrary HERO团队代码库
  * @{
  */

/** @addtogroup Low_Level_Library  Low Level Library(底层库)
  * @{
  */

/** @addtogroup BSP_Driver_Library BSP Driver(板级驱动)
  * @{
  */

/** @addtogroup BSP_IO_Device IO板级驱动
  * @{
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup BSP_IO_Exported_Macros BSP_IO宏
  * @{
  */

/**
  * @brief  将变量转换为同义字符串
  * @param  x: 某变量的变量名
  * @retval 该变量名的同义字符串.
  */
#define VariableName(x)	#x

/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup BSP_IO_Exported_Types BSP_IO数据类型
  * @{
  */

/**
  * @brief 输出类型IO 结构体定义
  */
typedef struct
{
	BasicMsg		Basic;          //!< 基本信息
	
	GPIO_TypeDef	*GPIOx;         //!< 外设地址
    	
	uint32_t 		GPIO_Pin_Num;   //!< 引脚号（编号，非按位指示）
    
	BIT_STRIP       Operator;	    //!< 写位带地址（可读可写）

}IO_Out;


/**
  * @brief 输入类型IO 结构体定义
  */
typedef struct
{

	BasicMsg		Basic;          //!< 基本信息
	
	StateFilter	    filter;         //!< 滤波器
	
	GPIO_TypeDef	*GPIOx;         //!< 外设地址
    
	uint32_t 		GPIO_Pin_Num;   //!< 引脚号（编号，非按位指示）	
	
	BIT_STRIP       Operator;       //!< 读位带地址（可读不可写）

}IO_In;

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup BSP_IO_Register_Functions BSP_IO注册函数
  * @{
  */
/* 一般初始化(连带数据结构体) *****************************/
void Register_InputIO(IO_In* pDevice, uint32_t ID, char* Name,
						GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t InputMode, uint32_t Pull);
void Register_OutputIO(IO_Out* pDevice, uint32_t ID, char* Name,
						GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t OutputMode, uint32_t Speed, GPIO_PinState Initstate);
/**
  * @}
  */

/** @addtogroup BSP_IO_Init_Functions BSP_IO硬件初始化函数
  * @{
  */
/* 纯外设初始化 *****************************/
void Input_GPIO_Init(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t InputMode, uint32_t Pull);
void Output_GPIO_Init(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t OutputMode, uint32_t Speed, GPIO_PinState Initstate);
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
  
#endif /* __bsp_gpio_H */

/************************ (C) COPYRIGHT HITwh Excellent Robot Organization(HERO). *****END OF FILE****/

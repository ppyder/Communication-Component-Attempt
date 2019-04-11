/**
 ******************************************************************************
 * @file    bsp_gpio.h
 * @brief   �����弶IO��صĺꡢ���ݽṹ�ͺ�������.
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
#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

/* Includes ------------------------------------------------------------------*/
#include "io_bit_strip_F103.h"
#include "base_class.h"
#include "filter.h"
#include "macros.h"

/** @addtogroup HeroLibrary HERO�ŶӴ����
  * @{
  */

/** @addtogroup Low_Level_Library  Low Level Library(�ײ��)
  * @{
  */

/** @addtogroup BSP_Driver_Library BSP Driver(�弶����)
  * @{
  */

/** @addtogroup BSP_IO_Device IO�弶����
  * @{
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup BSP_IO_Exported_Macros BSP_IO��
  * @{
  */

/**
  * @brief  ������ת��Ϊͬ���ַ���
  * @param  x: ĳ�����ı�����
  * @retval �ñ�������ͬ���ַ���.
  */
#define VariableName(x)	#x

/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup BSP_IO_Exported_Types BSP_IO��������
  * @{
  */

/**
  * @brief �������IO �ṹ�嶨��
  */
typedef struct
{
	BasicMsg		Basic;          //!< ������Ϣ
	
	GPIO_TypeDef	*GPIOx;         //!< �����ַ
    	
	uint32_t 		GPIO_Pin_Num;   //!< ���źţ���ţ��ǰ�λָʾ��
    
	BIT_STRIP       Operator;	    //!< дλ����ַ���ɶ���д��

}IO_Out;


/**
  * @brief ��������IO �ṹ�嶨��
  */
typedef struct
{

	BasicMsg		Basic;          //!< ������Ϣ
	
	StateFilter	    filter;         //!< �˲���
	
	GPIO_TypeDef	*GPIOx;         //!< �����ַ
    
	uint32_t 		GPIO_Pin_Num;   //!< ���źţ���ţ��ǰ�λָʾ��	
	
	BIT_STRIP       Operator;       //!< ��λ����ַ���ɶ�����д��

}IO_In;

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup BSP_IO_Register_Functions BSP_IOע�ắ��
  * @{
  */
/* һ���ʼ��(�������ݽṹ��) *****************************/
void Register_InputIO(IO_In* pDevice, uint32_t ID, char* Name,
						GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t InputMode, uint32_t Pull);
void Register_OutputIO(IO_Out* pDevice, uint32_t ID, char* Name,
						GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin, uint32_t OutputMode, uint32_t Speed, GPIO_PinState Initstate);
/**
  * @}
  */

/** @addtogroup BSP_IO_Init_Functions BSP_IOӲ����ʼ������
  * @{
  */
/* �������ʼ�� *****************************/
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

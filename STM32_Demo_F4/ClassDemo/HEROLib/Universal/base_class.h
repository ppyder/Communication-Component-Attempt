/**
 ******************************************************************************
 * @file    base_class.h
 * @brief   �������������ݽṹ�������ͷ�������.
 *              + �����˻�����Ϣ���ݽṹ
 *              + �������ڴ������صĺ�������
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
#ifndef BASE_CLASS_H
#define BASE_CLASS_H

/* Includes ------------------------------------------------------------------*/
#include "macros.h"

/** @addtogroup HeroLibrary HERO�ŶӴ����
  * @{
  */

/** @addtogroup Low_Level_Library Low Level Library(�ײ��)
  * @{
  */

/** @addtogroup Universal_Software_Support Universal(���ͨ��֧��)
  * @{
  */
  
/** @addtogroup Universal_Base_Class Base:���༰�䷽��
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup Base_Class_Exported_Types Base��������
  * @{
  */
  
/**
  * @brief ������Ϣ �ṹ�嶨��
  */
typedef struct
{
	uint32_t ID;    //!< �����Ϣ�������
    
	char*	Name;   //!< ����������
	
}BasicMsg;
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup Base_Class_Functions Base�ڴ������������
  * @{
  */

bool VariableInit(void *pVariable, const uint32_t MemorySize);
bool DataCopy(void *pAimAddress, const void* const pSourceAddress, const uint32_t MemorySize);
bool VariableSet(void *pVariable, const int8_t SetNumber, const uint32_t MemorySize);
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

#endif /* __base_class_H */

/************************ (C) COPYRIGHT HITwh Excellent Robot Organization(HERO). *****END OF FILE****/

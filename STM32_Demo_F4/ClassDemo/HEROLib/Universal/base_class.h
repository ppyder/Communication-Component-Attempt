/**
 ******************************************************************************
 * @file    base_class.h
 * @brief   包含基本的数据结构、常量和方法声明.
 *              + 定义了基本信息数据结构
 *              + 声明了内存填充相关的函数集合
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
#ifndef BASE_CLASS_H
#define BASE_CLASS_H

/* Includes ------------------------------------------------------------------*/
#include "macros.h"

/** @addtogroup HeroLibrary HERO团队代码库
  * @{
  */

/** @addtogroup Low_Level_Library Low Level Library(底层库)
  * @{
  */

/** @addtogroup Universal_Software_Support Universal(软件通用支持)
  * @{
  */
  
/** @addtogroup Universal_Base_Class Base:基类及其方法
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup Base_Class_Exported_Types Base数据类型
  * @{
  */
  
/**
  * @brief 基本信息 结构体定义
  */
typedef struct
{
	uint32_t ID;    //!< 编号信息（句柄）
    
	char*	Name;   //!< 文字性描述
	
}BasicMsg;
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup Base_Class_Functions Base内存基本操作函数
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

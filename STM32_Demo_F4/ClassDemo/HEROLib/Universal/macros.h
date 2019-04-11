/**
 ******************************************************************************
 * @file    macros.h
 * @brief   包含一些通用的宏和枚举类型.
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
#ifndef MACROS_H
#define MACROS_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>     //兼容类型声明
#include <stdbool.h>    //C99中的新类型：bool的定义

/** @addtogroup HeroLibrary HERO团队代码库
  * @{
  */

/** @addtogroup Low_Level_Library Low Level Library(底层库)
  * @{
  */

/** @addtogroup Universal_Software_Support Universal(软件通用支持)
  * @{
  */
  
/** @addtogroup Universal_Macros_enums Macros&enums通用宏和枚举定义
  * @{
  */

/**
  * @brief 浮点数的精度，即浮点数能表示的最小值。
  *         注意：浮点运算的“零”应与该值在绝对值层面比较大小，而非匹配；
  */
#define ZERO_FLOAT 1*10e-31f

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

#endif /* __macros_H */

/************************ (C) COPYRIGHT HITwh Excellent Robot Organization(HERO). *****END OF FILE****/

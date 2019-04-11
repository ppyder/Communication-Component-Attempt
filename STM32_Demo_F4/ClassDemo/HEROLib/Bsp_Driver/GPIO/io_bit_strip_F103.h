/**
 ******************************************************************************
 * @file    io_bit_strip_f103.h
 * @brief   包含了一些位带操作相关的宏定义.
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
#ifndef _IO_BITSTRIP_F103_H
#define _IO_BITSTRIP_F103_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
 
/** @addtogroup BSP_IO_Device 板级IO
  * @{
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup BSP_IO_BitStrip_Macros BSP_IO位带宏
  * @note 关于位带的说明：
  *
  *     addr & 0xF0000000，取地址的高4位，看看是2还是4，用于区分SRAM和外设地址，
  *     如果是2，+0x02000000则=0X2200 0000，即是SRAM，如果是4，+0x02000000则=0X4200 0000，即是外设
  *
  *     SRAM 位带区:    0X2000 0000~0X2010 0000
  *     SRAM 位带别名区:0X2200 0000~0X23FF FFFF
  *
  *     外设 位带区:    0X4000 0000~0X4010 0000
  *     外设 位带别名区:0X4200 0000~0X43FF FFFF
  *
  *     addr & 0x000FFFFFF，屏蔽掉高两位，相当于-0X2000 0000或者-0X4000 0000，结果表示偏移位带区多少个字节
  *     <<5  等于*8*4，因为位带区一个地址表示一个字节，一个字节有8个bit，一个bit可以膨胀成一个字，即4个字节
  *     <<2 等于*4，因为一个位可以膨胀成一个字，即4个字节
  *
  *     分解成两条公式应该就是这样：
  *     SRAM位带别名地址
  *     AliasAddr = 0x22000000 + ((A-0x20000000)*8 + n) * 4 = 0x22000000 + (A-0x20000000)*8*4 + n*4
  *     外设位带别名地址
  *     AliasAddr = 0x22000000 + ((A-0x20000000)*8 + n) * 4 = 0x22000000 + (A-0x20000000)*8*4 + n*4
  *
  * @{
  */

/**
  * @brief 位带操作变量对应的类型封装
  */
#define BIT_STRIP volatile unsigned long *

/**
  * @brief  把“位带地址+位序号”转换成别名地址的宏
  * @param  addr    位带地址
  * @param  bitnum  位序号
  * @retval 别名地址
  */
#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x02000000 + ((addr & 0x00FFFFFF)<<5) + (bitnum<<2)) 

/**
  * @brief  把数值类型的地址转换成指针再取其指向内容
  * @param  addr    位带地址值
  * @retval 目标地址指向的内容
  */
#define MEM_ADDR(addr)  *((BIT_STRIP)(addr)) 

/**
  * @brief  把位带别名区地址转换成指针
  * @param  addr    位带地址值
  * @param  bitnum  位序号
  * @retval 目标地址指向的内容
  */
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))   

/**
  * @brief ODR寄存器相对于基地址的偏移量
  */
#define ODR_Addr_Offset	12

/**
  * @brief IDR寄存器相对于基地址的偏移量
  */
#define IDR_Addr_Offset	8

/** @defgroup BSP_IO_BitStrip_Address GPIO ODR 和 IDR 寄存器地址映射
  * @{
  */
#define GPIOA_ODR_Addr    (GPIOA_BASE + ODR_Addr_Offset)  //!< 0x4001080C   
#define GPIOB_ODR_Addr    (GPIOB_BASE + ODR_Addr_Offset)  //!< 0x40010C0C   
#define GPIOC_ODR_Addr    (GPIOC_BASE + ODR_Addr_Offset)  //!< 0x4001100C   
#define GPIOD_ODR_Addr    (GPIOD_BASE + ODR_Addr_Offset)  //!< 0x4001140C   
#define GPIOE_ODR_Addr    (GPIOE_BASE + ODR_Addr_Offset)  //!< 0x4001180C   
#define GPIOF_ODR_Addr    (GPIOF_BASE + ODR_Addr_Offset)  //!< 0x40011A0C      
#define GPIOG_ODR_Addr    (GPIOG_BASE + ODR_Addr_Offset)  //!< 0x40011E0C      
  
#define GPIOA_IDR_Addr    (GPIOA_BASE + IDR_Addr_Offset)  //!< 0x40010808   
#define GPIOB_IDR_Addr    (GPIOB_BASE + IDR_Addr_Offset)  //!< 0x40010C08   
#define GPIOC_IDR_Addr    (GPIOC_BASE + IDR_Addr_Offset)  //!< 0x40011008   
#define GPIOD_IDR_Addr    (GPIOD_BASE + IDR_Addr_Offset)  //!< 0x40011408   
#define GPIOE_IDR_Addr    (GPIOE_BASE + IDR_Addr_Offset)  //!< 0x40011808   
#define GPIOF_IDR_Addr    (GPIOF_BASE + IDR_Addr_Offset)  //!< 0x40011A08   
#define GPIOG_IDR_Addr    (GPIOG_BASE + IDR_Addr_Offset)  //!< 0x40011E08 
/**
  * @}
  */

/** @defgroup BSP_IO_BitStrip_Operator GPIO 位带操作符集合
  * @note    单独操作 GPIO的某一个IO口，n(0,1,2...16),n表示具体是哪一个IO口
  * @{
  */
 
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //!< 输出   
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //!< 输入   
  
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //!< 输出   
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //!< 输入   
  
#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //!< 输出   
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //!< 输入   
  
#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //!< 输出   
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //!< 输入   
  
#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //!< 输出   
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //!< 输入  
  
#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //!< 输出   
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //!< 输入  
  
#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //!< 输出   
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //!< 输入
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* __io_bit_strip_f103_H */

/************************ (C) COPYRIGHT HITwh Excellent Robot Organization(HERO). *****END OF FILE****/

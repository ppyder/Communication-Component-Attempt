/**
 ******************************************************************************
 * @file    io_bit_strip_f103.h
 * @brief   ������һЩλ��������صĺ궨��.
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
#ifndef _IO_BITSTRIP_F103_H
#define _IO_BITSTRIP_F103_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
 
/** @addtogroup BSP_IO_Device �弶IO
  * @{
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup BSP_IO_BitStrip_Macros BSP_IOλ����
  * @note ����λ����˵����
  *
  *     addr & 0xF0000000��ȡ��ַ�ĸ�4λ��������2����4����������SRAM�������ַ��
  *     �����2��+0x02000000��=0X2200 0000������SRAM�������4��+0x02000000��=0X4200 0000����������
  *
  *     SRAM λ����:    0X2000 0000~0X2010 0000
  *     SRAM λ��������:0X2200 0000~0X23FF FFFF
  *
  *     ���� λ����:    0X4000 0000~0X4010 0000
  *     ���� λ��������:0X4200 0000~0X43FF FFFF
  *
  *     addr & 0x000FFFFFF�����ε�����λ���൱��-0X2000 0000����-0X4000 0000�������ʾƫ��λ�������ٸ��ֽ�
  *     <<5  ����*8*4����Ϊλ����һ����ַ��ʾһ���ֽڣ�һ���ֽ���8��bit��һ��bit�������ͳ�һ���֣���4���ֽ�
  *     <<2 ����*4����Ϊһ��λ�������ͳ�һ���֣���4���ֽ�
  *
  *     �ֽ��������ʽӦ�þ���������
  *     SRAMλ��������ַ
  *     AliasAddr = 0x22000000 + ((A-0x20000000)*8 + n) * 4 = 0x22000000 + (A-0x20000000)*8*4 + n*4
  *     ����λ��������ַ
  *     AliasAddr = 0x22000000 + ((A-0x20000000)*8 + n) * 4 = 0x22000000 + (A-0x20000000)*8*4 + n*4
  *
  * @{
  */

/**
  * @brief λ������������Ӧ�����ͷ�װ
  */
#define BIT_STRIP volatile unsigned long *

/**
  * @brief  �ѡ�λ����ַ+λ��š�ת���ɱ�����ַ�ĺ�
  * @param  addr    λ����ַ
  * @param  bitnum  λ���
  * @retval ������ַ
  */
#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x02000000 + ((addr & 0x00FFFFFF)<<5) + (bitnum<<2)) 

/**
  * @brief  ����ֵ���͵ĵ�ַת����ָ����ȡ��ָ������
  * @param  addr    λ����ֵַ
  * @retval Ŀ���ַָ�������
  */
#define MEM_ADDR(addr)  *((BIT_STRIP)(addr)) 

/**
  * @brief  ��λ����������ַת����ָ��
  * @param  addr    λ����ֵַ
  * @param  bitnum  λ���
  * @retval Ŀ���ַָ�������
  */
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))   

/**
  * @brief ODR�Ĵ�������ڻ���ַ��ƫ����
  */
#define ODR_Addr_Offset	12

/**
  * @brief IDR�Ĵ�������ڻ���ַ��ƫ����
  */
#define IDR_Addr_Offset	8

/** @defgroup BSP_IO_BitStrip_Address GPIO ODR �� IDR �Ĵ�����ַӳ��
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

/** @defgroup BSP_IO_BitStrip_Operator GPIO λ������������
  * @note    �������� GPIO��ĳһ��IO�ڣ�n(0,1,2...16),n��ʾ��������һ��IO��
  * @{
  */
 
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //!< ���   
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //!< ����   
  
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //!< ���   
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //!< ����   
  
#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //!< ���   
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //!< ����   
  
#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //!< ���   
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //!< ����   
  
#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //!< ���   
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //!< ����  
  
#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //!< ���   
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //!< ����  
  
#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //!< ���   
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //!< ����
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

/**
 ******************************************************************************
 * @file    base_class.c
 * @brief   定义基类行为、定义常量以及基本的内存操作.
 *              + 数据内存拷贝
 *              + 数据初始化（零初始化、指定值的初始化）
 ******************************************************************************
 *
 * Copyright (C) HITwh Excellent Robot Organization(HERO). 2015-2018. All rights reserved.
 *
 * @version 1.0 示例版本
 * @author  杨亦凡 17863107058(手机)   942041771(qq)
 * @date    2018/10/10
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "base_class.h"
#include <string.h>

/**
 * @brief	    数据拷贝函数，可用于结构体的简单复制操作和数据复制
 * @param[out]  pAimAddress     指向目标变量地址的指针.
 * @param[in]   pSourceAddress  指向数据源变量地址的指针.
 * @param[in]   MemorySize      该变量的大小，通常使用sizeof()关键字获取.
 * @note        若MemorySize的值为0，则视为没有内存拷贝请求，不会进行任何操作。依旧返回true.
 *              注意：pAimAddress不能是指向常量的指针.
 * @retval	    is Copy Successfully: 若拷贝成功，则返回true,反之返回false.
 */
bool DataCopy(void *pAimAddress, const void* const pSourceAddress, const uint32_t MemorySize)
{
    if(pSourceAddress == NULL || pAimAddress == NULL || pAimAddress == pSourceAddress)
        return false;
    
    if(MemorySize == 0)
        return true;
    
    memcpy(pAimAddress, pSourceAddress, MemorySize);
    
    return true;
}

/**
 * @brief	    设置变量的前MemorySize个字节的值
 * @param[out]  pVariable   指向目标变量地址的指针.
 * @param[in]   SetNumber   指定该变量的每个字节将被设置成的值.
 * @param[in]   MemorySize  该变量的大小，通常使用sizeof()关键字获取.
 * @note        若MemorySize的值为0，则视为被设置的字节数为0，不会进行任何操作。依旧返回true。
 *              注意：PVariable不能是指向常量的指针.
 * @retval	    is Set Successfully: 若设置成功，则返回true,反之返回false.
 */
bool VariableSet(void *pVariable, const int8_t SetNumber, const uint32_t MemorySize)
{
    if(pVariable == NULL)
        return false;
    
    if(MemorySize == 0)
        return true;
    
    memset(pVariable, SetNumber, MemorySize);
    
    return true;
}

/**
 * @brief	    初始化变量，将其内容置零
 * @param[out]  pVariable   指向目标变量地址的指针.
 * @param[in]   MemorySize  该变量的大小，通常使用sizeof()关键字获取.
 * @note        若MemorySize的值为0，则视为被设置的字节数为0，不会进行任何操作。返回false。
 *              注意：PVariable不能是指向常量的指针。
 * @retval	    is Set Successfully: 若初始化成功，则返回true,反之返回false.
 */
bool VariableInit(void *pVariable, const uint32_t MemorySize)
{
    if(pVariable == NULL || MemorySize == 0)
        return false;
    
    memset(pVariable, 0, MemorySize);
    
    return true;
}

/************************ (C) COPYRIGHT HITwh Excellent Robot Organization(HERO). *****END OF FILE****/

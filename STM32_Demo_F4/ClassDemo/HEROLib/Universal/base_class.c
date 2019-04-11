/**
 ******************************************************************************
 * @file    base_class.c
 * @brief   ���������Ϊ�����峣���Լ��������ڴ����.
 *              + �����ڴ濽��
 *              + ���ݳ�ʼ�������ʼ����ָ��ֵ�ĳ�ʼ����
 ******************************************************************************
 *
 * Copyright (C) HITwh Excellent Robot Organization(HERO). 2015-2018. All rights reserved.
 *
 * @version 1.0 ʾ���汾
 * @author  ���ෲ 17863107058(�ֻ�)   942041771(qq)
 * @date    2018/10/10
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "base_class.h"
#include <string.h>

/**
 * @brief	    ���ݿ��������������ڽṹ��ļ򵥸��Ʋ��������ݸ���
 * @param[out]  pAimAddress     ָ��Ŀ�������ַ��ָ��.
 * @param[in]   pSourceAddress  ָ������Դ������ַ��ָ��.
 * @param[in]   MemorySize      �ñ����Ĵ�С��ͨ��ʹ��sizeof()�ؼ��ֻ�ȡ.
 * @note        ��MemorySize��ֵΪ0������Ϊû���ڴ濽�����󣬲�������κβ��������ɷ���true.
 *              ע�⣺pAimAddress������ָ������ָ��.
 * @retval	    is Copy Successfully: �������ɹ����򷵻�true,��֮����false.
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
 * @brief	    ���ñ�����ǰMemorySize���ֽڵ�ֵ
 * @param[out]  pVariable   ָ��Ŀ�������ַ��ָ��.
 * @param[in]   SetNumber   ָ���ñ�����ÿ���ֽڽ������óɵ�ֵ.
 * @param[in]   MemorySize  �ñ����Ĵ�С��ͨ��ʹ��sizeof()�ؼ��ֻ�ȡ.
 * @note        ��MemorySize��ֵΪ0������Ϊ�����õ��ֽ���Ϊ0����������κβ��������ɷ���true��
 *              ע�⣺PVariable������ָ������ָ��.
 * @retval	    is Set Successfully: �����óɹ����򷵻�true,��֮����false.
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
 * @brief	    ��ʼ��������������������
 * @param[out]  pVariable   ָ��Ŀ�������ַ��ָ��.
 * @param[in]   MemorySize  �ñ����Ĵ�С��ͨ��ʹ��sizeof()�ؼ��ֻ�ȡ.
 * @note        ��MemorySize��ֵΪ0������Ϊ�����õ��ֽ���Ϊ0����������κβ���������false��
 *              ע�⣺PVariable������ָ������ָ�롣
 * @retval	    is Set Successfully: ����ʼ���ɹ����򷵻�true,��֮����false.
 */
bool VariableInit(void *pVariable, const uint32_t MemorySize)
{
    if(pVariable == NULL || MemorySize == 0)
        return false;
    
    memset(pVariable, 0, MemorySize);
    
    return true;
}

/************************ (C) COPYRIGHT HITwh Excellent Robot Organization(HERO). *****END OF FILE****/

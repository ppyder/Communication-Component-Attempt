/**
 ******************************************************************************
 * @file    filter.c
 * @brief   软件底层：滤波器相关的基本操作和方法.
 *              + 提供了状态滤波器相关的方法
 *                  ++ 滤波跳变状态的获取函数
 *                  ++ 按位滤波的函数
 *              + TODO:提供数据滤波器相关的方法
 * @version 1.0 示例版本
 * @author  杨亦凡
 * @contact 17863107058(手机)   942041771(qq)
 * @date    2018/10/18
 *
 ******************************************************************************
 *
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (C) HITwh Excellent Robot Organization(HERO). 2015-2018.</center></h2>
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "filter.h"
#include <string.h>     //使用其中的NULL宏

/* Private constants --------------------------------------------------------*/
/**
  * @brief 常量数表 滤波掩码 用于决定滤波位数
  */
static const uint32_t FILTER_MASK_CODE[33] = 
{
    0X00,
    0x01,       0x03,       0x07,       0x0F,
    0x1F,       0x3F,       0x7F,       0xFF,
    0x1FF,      0x3FF,      0x7FF,      0xFFF,
    0x1FFF,     0x3FFF,     0x7FFF,     0xFFFF,
    0x1FFFF,    0x3FFFF,    0x7FFFF,    0xFFFFF,
    0x1FFFFF,   0x3FFFFF,   0x7FFFFF,   0xFFFFFF,
    0x1FFFFFF,  0x3FFFFFF,  0x7FFFFFF,  0xFFFFFFF,
    0x1FFFFFFF, 0x3FFFFFFF, 0x7FFFFFFF, 0xFFFFFFFF
};

/**
 * @brief	    输入滤波函数
 * @param[out]  pFilter     指向本次参与滤波的滤波器结构体
 * @param[in]   NowState    当前该滤波器所关心对象的瞬态采样状态
 * @retval	    当前的状态（bool），无效状态为false.
 */
bool InputFilter(StateFilter *pFilter, bool NowState)
{
    uint32_t temp = 0;
    
    if(pFilter == NULL)
        return false;
    
    NowState &= FILTER_MASK_CODE[1];    //取末位，将非法输入的危害降到最小
    
    pFilter->History <<= 1;
    pFilter->History |= NowState;       //更新历史
    
    //取最新的 滤波位数 位
    temp = (pFilter->History) & FILTER_MASK_CODE[pFilter->FilteringBits];
    
    if(temp == FILTER_MASK_CODE[pFilter->FilteringBits])    //被关注的位都是1，说明进入所谓“高电平状态”
    {
        if(pFilter->State != true)
        {
            pFilter->Jumping = RISING_EDGE; //标记触发上升沿
            pFilter->State = true;
        }
    }
    else if(temp == FILTER_MASK_CODE[0])                    //进入“低电平状态”
    {
        if(pFilter->State != false)
        {
            pFilter->Jumping = RISING_EDGE; //标记触发下降沿
            pFilter->State = false;
        }
    }
    
    return pFilter->State;
}

/**
 * @brief	    获取跳变状态
 * @param[in]   pFilter     指向期望获取状态的滤波器结构体
 * @retval	    跳变状态（JumpingState）,无效状态为NO_JUMPING.
 */
JumpingState GetJumpingState(StateFilter *pFilter)
{
    JumpingState result = NO_JUMPING;
    
    if(pFilter == NULL)
        return NO_JUMPING;
    
    result = pFilter->Jumping;
    
    pFilter->Jumping = NO_JUMPING;      //读走一次便使之失效，防止重复触发相应操作
    
    return result;
}

//Z变换分子、分母向量，
//注意这是在运算频率为100Hz的情况下使用双线性变换法计算出来的。
FillterParamType Fill_Params[] = 
{
    {{0.969150966612274, 0.969150966612274}, {1.000000000000000, 0.938301933224547}, 0, 0}, //1000Hz截止，反馈值滤波
    {{0.992970639, 0.992970639}, {1, 0.9859412784}, 0, 0},                                  //4496.11Hz截止，宽带反馈值滤波
    {{0.611015470351657, 0.611015470351657}, {1.000000000000000, 0.222030940703315}, 0, 0}, //50Hz截止，输入前馈滤波，抑制震荡
};

/**
 * @brief	    一阶低通数据滤波器，注意调用频率
 * @param[in]   Value     当前采样值
 * @param[in]   pParams   指向对应的滤波器参量结构体
 * @retval	    滤波后的输出数据
 */
double DataFillter(double Value, FillterParamType *pParams)
{
    double result;
    
    result = (1 / pParams->den[0]) 
            * (pParams->num[0] * Value + pParams->num[1] * pParams->LastInput 
                - pParams->den[1] * pParams->LastOutput);
    
    //记录历史值
    pParams->LastInput = Value;
    pParams->LastOutput = result;
    
    return result;
}   

/************************ (C) COPYRIGHT HITwh Excellent Robot Organization(HERO). *****END OF FILE****/

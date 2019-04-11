/**
 ******************************************************************************
 * @file    filter.h
 * @brief   包含关于滤波器的类型定义和函数声明.
 *              + 包含状态滤波器的相关定义的声明
 *                  ++ 包含状态滤波器跳变沿的枚举类型定义
 *                  ++ 包含状态滤波器的数据结构定义
 *                  ++ 包含状态滤波器的操作函数声明
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
#ifndef FILTER_H
#define FILTER_H

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
  
/** @addtogroup Universal_filter fillter滤波器
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup Filter_Exported_Types Filter数据类型
  * @{
  */

/**
  * @brief 滤波器跳变状态 枚举定义
  */
typedef enum 
{
    NO_JUMPING,     //!< 无跳变
    RISING_EDGE,    //!< 上升沿跳变
    FALLING_EDGE,   //!< 下降沿跳变
    
}JumpingState;

/**
  * @brief 一阶低通数据滤波器可选参数 枚举定义
  */
enum FillterParams
{
    Fill_1000Hz = 0,//1000Hz截止，反馈值滤波
    Fill_4496Hz,    //4496.11Hz截止，宽带反馈值滤波
    Fill_50Hz,      //50Hz截止，输入前馈滤波，抑制震荡
    Fill_Num
};   

/**
  * @brief 状态滤波器 结构定义
  */
typedef struct
{
    uint32_t        CheckPeriod;    //!< 检测周期（单位为us）
    
    uint32_t        History;	    //!< 所记录的历史值
    
    uint8_t         FilteringBits;  //!< 滤波位数
    
	bool            State;		    //!< 滤波后确认的状态
    
    JumpingState    Jumping;        //!< 所监测出的跳变状态
		
}StateFilter;

/**
  * @brief 一阶低通数据滤波器 结构定义
  */
typedef struct 
{
    double num[2];         //!< Z变换后滤波器脉冲传函的分子向量
    double den[2];         //!< Z变换后滤波器脉冲传函的分母向量
    
    double LastInput;      //!< 记录上次滤波器的输入值
    double LastOutput;     //!< 记录上次滤波器的输出
    
}FillterParamType;
/**
  * @}
  */

/* Exported variable --------------------------------------------------------*/
/** @addtogroup Filter_Variable Filter公共变量
  * @{
  */

extern FillterParamType Fill_Params[Fill_Num];
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup Filter_Functions Filter基本操作函数
  * @{
  */

bool InputFilter(StateFilter *pFilter, bool NowState);
JumpingState GetJumpingState(StateFilter *pFilter);
double DataFillter(double Value, FillterParamType *Params);
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

#endif /* __filter_H */

/************************ (C) COPYRIGHT HITwh Excellent Robot Organization(HERO). *****END OF FILE****/

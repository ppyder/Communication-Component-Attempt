/**
 ******************************************************************************
 * @file    filter.h
 * @brief   ���������˲��������Ͷ���ͺ�������.
 *              + ����״̬�˲�������ض��������
 *                  ++ ����״̬�˲��������ص�ö�����Ͷ���
 *                  ++ ����״̬�˲��������ݽṹ����
 *                  ++ ����״̬�˲����Ĳ�����������
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
#ifndef FILTER_H
#define FILTER_H

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
  
/** @addtogroup Universal_filter fillter�˲���
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup Filter_Exported_Types Filter��������
  * @{
  */

/**
  * @brief �˲�������״̬ ö�ٶ���
  */
typedef enum 
{
    NO_JUMPING,     //!< ������
    RISING_EDGE,    //!< ����������
    FALLING_EDGE,   //!< �½�������
    
}JumpingState;

/**
  * @brief һ�׵�ͨ�����˲�����ѡ���� ö�ٶ���
  */
enum FillterParams
{
    Fill_1000Hz = 0,//1000Hz��ֹ������ֵ�˲�
    Fill_4496Hz,    //4496.11Hz��ֹ���������ֵ�˲�
    Fill_50Hz,      //50Hz��ֹ������ǰ���˲���������
    Fill_Num
};   

/**
  * @brief ״̬�˲��� �ṹ����
  */
typedef struct
{
    uint32_t        CheckPeriod;    //!< ������ڣ���λΪus��
    
    uint32_t        History;	    //!< ����¼����ʷֵ
    
    uint8_t         FilteringBits;  //!< �˲�λ��
    
	bool            State;		    //!< �˲���ȷ�ϵ�״̬
    
    JumpingState    Jumping;        //!< ������������״̬
		
}StateFilter;

/**
  * @brief һ�׵�ͨ�����˲��� �ṹ����
  */
typedef struct 
{
    double num[2];         //!< Z�任���˲������崫���ķ�������
    double den[2];         //!< Z�任���˲������崫���ķ�ĸ����
    
    double LastInput;      //!< ��¼�ϴ��˲���������ֵ
    double LastOutput;     //!< ��¼�ϴ��˲��������
    
}FillterParamType;
/**
  * @}
  */

/* Exported variable --------------------------------------------------------*/
/** @addtogroup Filter_Variable Filter��������
  * @{
  */

extern FillterParamType Fill_Params[Fill_Num];
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup Filter_Functions Filter������������
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

/**
 ******************************************************************************
 * @file    filter.c
 * @brief   ����ײ㣺�˲�����صĻ��������ͷ���.
 *              + �ṩ��״̬�˲�����صķ���
 *                  ++ �˲�����״̬�Ļ�ȡ����
 *                  ++ ��λ�˲��ĺ���
 *              + TODO:�ṩ�����˲�����صķ���
 * @version 1.0 ʾ���汾
 * @author  ���ෲ
 * @contact 17863107058(�ֻ�)   942041771(qq)
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
#include <string.h>     //ʹ�����е�NULL��

/* Private constants --------------------------------------------------------*/
/**
  * @brief �������� �˲����� ���ھ����˲�λ��
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
 * @brief	    �����˲�����
 * @param[out]  pFilter     ָ�򱾴β����˲����˲����ṹ��
 * @param[in]   NowState    ��ǰ���˲��������Ķ����˲̬����״̬
 * @retval	    ��ǰ��״̬��bool������Ч״̬Ϊfalse.
 */
bool InputFilter(StateFilter *pFilter, bool NowState)
{
    uint32_t temp = 0;
    
    if(pFilter == NULL)
        return false;
    
    NowState &= FILTER_MASK_CODE[1];    //ȡĩλ�����Ƿ������Σ��������С
    
    pFilter->History <<= 1;
    pFilter->History |= NowState;       //������ʷ
    
    //ȡ���µ� �˲�λ�� λ
    temp = (pFilter->History) & FILTER_MASK_CODE[pFilter->FilteringBits];
    
    if(temp == FILTER_MASK_CODE[pFilter->FilteringBits])    //����ע��λ����1��˵��������ν���ߵ�ƽ״̬��
    {
        if(pFilter->State != true)
        {
            pFilter->Jumping = RISING_EDGE; //��Ǵ���������
            pFilter->State = true;
        }
    }
    else if(temp == FILTER_MASK_CODE[0])                    //���롰�͵�ƽ״̬��
    {
        if(pFilter->State != false)
        {
            pFilter->Jumping = RISING_EDGE; //��Ǵ����½���
            pFilter->State = false;
        }
    }
    
    return pFilter->State;
}

/**
 * @brief	    ��ȡ����״̬
 * @param[in]   pFilter     ָ��������ȡ״̬���˲����ṹ��
 * @retval	    ����״̬��JumpingState��,��Ч״̬ΪNO_JUMPING.
 */
JumpingState GetJumpingState(StateFilter *pFilter)
{
    JumpingState result = NO_JUMPING;
    
    if(pFilter == NULL)
        return NO_JUMPING;
    
    result = pFilter->Jumping;
    
    pFilter->Jumping = NO_JUMPING;      //����һ�α�ʹ֮ʧЧ����ֹ�ظ�������Ӧ����
    
    return result;
}

//Z�任���ӡ���ĸ������
//ע������������Ƶ��Ϊ100Hz�������ʹ��˫���Ա任����������ġ�
FillterParamType Fill_Params[] = 
{
    {{0.969150966612274, 0.969150966612274}, {1.000000000000000, 0.938301933224547}, 0, 0}, //1000Hz��ֹ������ֵ�˲�
    {{0.992970639, 0.992970639}, {1, 0.9859412784}, 0, 0},                                  //4496.11Hz��ֹ���������ֵ�˲�
    {{0.611015470351657, 0.611015470351657}, {1.000000000000000, 0.222030940703315}, 0, 0}, //50Hz��ֹ������ǰ���˲���������
};

/**
 * @brief	    һ�׵�ͨ�����˲�����ע�����Ƶ��
 * @param[in]   Value     ��ǰ����ֵ
 * @param[in]   pParams   ָ���Ӧ���˲��������ṹ��
 * @retval	    �˲�����������
 */
double DataFillter(double Value, FillterParamType *pParams)
{
    double result;
    
    result = (1 / pParams->den[0]) 
            * (pParams->num[0] * Value + pParams->num[1] * pParams->LastInput 
                - pParams->den[1] * pParams->LastOutput);
    
    //��¼��ʷֵ
    pParams->LastInput = Value;
    pParams->LastOutput = result;
    
    return result;
}   

/************************ (C) COPYRIGHT HITwh Excellent Robot Organization(HERO). *****END OF FILE****/

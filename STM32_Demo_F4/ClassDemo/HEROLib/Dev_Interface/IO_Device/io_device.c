/**
 ******************************************************************************
 * @file    io_device.c
 * @brief   �弶IO����ӿڣ��м����ײ�ĳ���ӿڣ�����ע��Ͳ�������.
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

/* Includes ------------------------------------------------------------------*/
#include "io_device.h"

/**
 * @brief	    ע��һ������豸
 * @param[out]  pDevice		ָ������豸��ָ��
 * @param[in]   ID			���豸�ı�ţ������
 * @param[in]   Name		�Ը��豸������������
 * @param[in]   pIOStruct	ָ����豸��ӦIOʵ���ָ��
 * @retval	    �Ƿ�ɹ�ע��(bool)
 */
_Bool RegisterIO_OutDevice(IO_OutDev* pDevice, uint32_t ID, char* Name, IO_Out* pIOStruct)
{
	if(pDevice == 0)
	{
	  return false;
	}
	
	//��ʼ�����ݽṹ��
	pDevice -> Basic.ID = ID;
	pDevice -> Basic.Name = Name;
	
	pDevice -> pCtrlPin = pIOStruct;
	
	return true;
}

/**
 * @brief	    ע��һ�������豸
 * @param[out]  pDevice		ָ������豸��ָ��
 * @param[in]   ID			���豸�ı�ţ������
 * @param[in]   Name		�Ը��豸������������
 * @param[in]   pIOStruct	ָ����豸��ӦIOʵ���ָ��
 * @retval	    �Ƿ�ɹ�ע��(bool)
 */
bool RegisterIO_InDevice(IO_InDev* pDevice, uint32_t ID, char* Name, IO_In* pIOStruct)
{
	if(pDevice == 0)
	{
	  return false;
	}
	
	//��ʼ�����ݽṹ��
	pDevice -> Basic.ID = ID;
	pDevice -> Basic.Name = Name;
	
	pDevice -> pReadPin = pIOStruct;
	
	return true;
}

/**
 * @brief	    ����豸�����޸�
 * @param[in]   pDevice		ָ������豸��ָ��
 * @param[in]   NewState	���������״̬
 * @retval	    None
 */
void IO_OutDevCtrl(IO_OutDev* pDevice, GPIO_PinState NewState)
{
	*(pDevice->pCtrlPin->Operator) = NewState;
}

/**
 * @brief	    ��ȡ�����豸����˲ʱ��ƽ״̬(δ�˲�)
 * @param[in]   pDevice		ָ������豸��ָ��
 * @retval	    �õ�ƽ�ڴ�ʱ�̵ĵ�ƽֵ
 */
uint8_t IO_InDevRead(IO_InDev* pDevice)
{
	return *(pDevice->pReadPin->Operator);
}

/************************ (C) COPYRIGHT HITwh Excellent Robot Organization(HERO). *****END OF FILE****/

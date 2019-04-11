/**
 ******************************************************************************
 * @file    Half_Duplex.c
 * @brief   ʵ�����������Լ����ģ���˫��ͨ�Ż���.
 *              + ��˫����־ϵͳ
 *              + ��˫����������
 *              + ���ݷ��Ͷ���(�Ӻ�ʵ��)
 ******************************************************************************
 *
 * Copyright (C) HITwh Excellent Robot Organization(HERO). 2015-2018. All rights reserved.
 *
 * @version 1.0 ʾ���汾
 * @author  ���ෲ 17863107058(�ֻ�)   942041771(qq)
 * @date    2019/03/21
 *
 ******************************************************************************
 */

#include "Half_Duplex.h"

//˽�к�������
void BlockErrorHandler(void);

//����ȱʡ��ʼ���Ŀ�ͨ������ṹ��
static COMInfoTypedef NULL_COM = {0};

//�����˫��ͨѶ�ṹ��ȱʡֵ
void Hf_DuplexStructDeInit(Hf_DuplexTypedef *pModule)
{
    //���ͨ��δ����
    pModule->isBlocked = false;
    
    //ʹ�ܵ�һ�η���
    pModule->isGetMsg = true;
        
    pModule->RqstErrorCnt = 0;
    pModule->ErrorCntMax = 0;
    
    pModule->pCOM = &NULL_COM;
    pModule->pBlockError = BlockErrorHandler;
}

//���պ���(���ڽ�����ɺ���ã����ڸ�λ���ձ�־)
void Hf_HaveReceived(Hf_DuplexTypedef *pModule)
{
    pModule->isGetMsg = true;
    pModule->isBlocked  = false;
}

uint32_t testCnt = 0;

//���ͺ���
void Hf_SendData(Hf_DuplexTypedef *pModule, uint8_t CMD)
{
    //����Ѿ����յ����ݣ�����ζ�ſ��Է���������
    if(pModule->isGetMsg)
    {
        //��λ��־
        pModule->isGetMsg = false;
        
        //�����쳣������
        pModule->RqstErrorCnt = 0;
        
        //����ָ�����Ϣ
        pModule->pCOM->SendData(CMD);
    }
    else
    {
        //�������������ȴ����ն������أ����¼����
        pModule->RqstErrorCnt++;
        
        if(testCnt < pModule->RqstErrorCnt)
        {
            testCnt = pModule->RqstErrorCnt;
        }
        
        //���ش�����������ô�������
        if(pModule->ErrorCntMax > 0 && pModule->RqstErrorCnt > pModule->ErrorCntMax)
        {
            pModule->isBlocked = true;
            
            pModule->pBlockError();
        }
    }
}

//����������������λ��־��ʹ�ܷ��ͣ�
void Hf_Reconnect(Hf_DuplexTypedef *pModule)
{
    pModule->isGetMsg = true;
    
    //���Ͷ�Ӧ�����0��ָ�Լ��0��Ϊ��ָ��
    Hf_SendData(pModule, 0);
}

//�������ͨ������������
__weak void BlockErrorHandler(void)
{
    while(1)
    {
        
    }
}


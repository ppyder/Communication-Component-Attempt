/**
  ******************************************************************************
  * @file           : Com_Functions.h
  * @brief          : ������ͨ������Ĳ���������
  ******************************************************************************
  ** ˵����
  *     ͨ������ṹ���д���һЩ�ɵ��õĺ���ָ�룬�����ҽ���ʹ�ñ��ļ��еĺ�����
  * �õ�ʱ��ſ��Ա�֤��ʹ�ð�ȫ�ԡ�
  *     ��Ȼ����ȷ����ȫ������£���Ҳ��ȫ����ֱ�ӵ������еĺ�����
  *
  *     ���⣬ͨ��������ض�����fputc����������ζ�������ͨ��ĳ��������ַ�������
  * ͨ�Žӿڽ�����Ϣ��ӡ���ں� @COM_PRINTF_ID ��ָ�����ڴ�ӡ��ͨ������ż��ɡ�����
  * Լ����ÿ���ַ��������Ի��з� '\n'��Ϊ������־����Ϊ������߱����͸�ʽ�����ݵ�
  * ���ܡ�����fputc��һ���ַ�һ���ַ�����ȥ�ģ�Ϊ�˷�ֹ�����Ĵ�ӡ�ַ����ʽ����Ϣ
  * ����һ���ͣ�������˷��ͻ����־����ʶ���ַ��������ı�־��������'\n'�ַ���
  *
  * <h2><center>&copy; Copyright (C) HITwh Excellent Robot Organization(HERO). 2015-2018.</center></h2>
  *
  * + ����Ҫ�õ�ͨ�����ʱ����Ҫ�������²�����
  *
  *   1. �ڶ�Ӧģ���ͷ�ļ��а�����ͷ�ļ�("Com_Functions.h")��
  *   2. �ڶ�Ӧģ���Դ�ļ��а���ͷ�ļ�:"Comunication.h"��
  *   3. �ڶ�Ӧģ���Դ�ļ��ж����ڸ�ģ���������õ�ͨ������ṹ�壬����ͷ�ļ�����������
  *   4. �ڶ�Ӧģ���Դ�ļ��а��ճ�ʼ����������Ҫ����Ϣ���к����ͻ�����׼����
  *   5. �ڶ�Ӧģ���Դ�ļ��ж���ģ���ʼ������������ͷ�ļ�����������
  *   5. ��ͷ�ļ�"Com_UserModules.h"�е�COM_IDö��������������IDö�٣�
  *      ����Դ�ļ�"Com_UserModules.c"�е�COM_Modules�ṹ��ָ�������а�
  *      �ն�Ӧö�ٵ�˳������µ������ַ��
  *   6. ��Դ�ļ�"Com_UserModules.c"�е�ģ��ӳ�亯������Ӷ�Ӧ������Լ�IDӳ�䡣
  *   7. ���ʹ����CAN������Ҫ��CAN��صĴ�������������Ӷ�Ӧ�ĺ�����ַ��
  *
  * + ����Ҫ�����Ѿ������õ�ͨ�����ʱ����������²�����
  *
  *   1.����ͷ�ļ�"Com_Functions.h"��
  *   2.����������ģ���ͷ�ļ���
  *   3.ʹ�ñ��ļ��������ĺ���������ṹ����в�����
  *
  * + ���ļ��а��������º�����
  *
  *   1. CAN�˲�����ʼ��������
  *   2. ������ͨ�������ʼ��ͨ������ĳ�ʼ��������
  *   3. ���ݷ��ͺ����������������ݷ��͡���ʽ�����ݷ��ͣ�������ID�������ݰ�����
  *      ��������can_ID�����ݷ��͡�
  *
  ******************************************************************************
  */

#ifndef COM_FUNCTIONS_H
#define COM_FUNCTIONS_H

#include "ComStructDef.h"
#include "Com_UserModules.h"

//CAN�����˲�����ʼ������
void CAN_Filter_Config(CAN_HandleTypeDef* hcan);

//�ṹ���ʼ������
void COM_UART_StructInit(COMInfoTypedef *pModule, UART_HandleTypeDef *pHuart,
                            bool (*pDealData)(void*), bool (*pSendData)(uint8_t),
                            void *pRcvBuffer, uint32_t RcvBufferSize,
                            void *pSendBuffer, uint32_t SendBufferSize);
                            
void COM_CAN_StructInit(COMInfoTypedef *pModule, CAN_HandleTypeDef *pHCAN,
                            bool (*pDealData)(void*), bool (*pSendData)(uint8_t),
                            bool (*pCan_IsHeadData)(void*, uint32_t),
                            CAN_RxHeaderTypeDef *pRxHeader,
                            CAN_TxHeaderTypeDef *pTxHeader,
                            void *pRcvBuffer, uint32_t RcvBufferSize,
                            void *pSendBuffer, uint32_t SendBufferSize);

//���ݷ��ͺ���
bool COM_SendUnformatData(COMInfoTypedef *pModule, uint8_t *pData, uint32_t DataSzie);
bool COM_CANSendUnformatIDData(COMInfoTypedef *pModule, CAN_TxHeaderTypeDef *pHeader, uint8_t *pData, uint16_t Size);
void SendData_Safely(COMInfoTypedef *pModule, uint8_t SendCMD);

#endif

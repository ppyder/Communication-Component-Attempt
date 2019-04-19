#ifndef COMSTRUCTDEF_H
#define COMSTRUCTDEF_H

#include <stdbool.h>
#include <stdint.h>

#include "stm32f4xx_hal.h"

//������ʹ�ù����еĴ�����м��й���
typedef enum 
{
    //�޴���
    COM_NoError = 0,
    
    //δ��ʼ��ͨ�����
    COM_Error_UnInited,
    
    //δ��ͨ�����ӳ�亯�����ҵ���Ӧ��������
    COM_Error_UnFindModule,
    
    COM_ErrorCodeNum
    
}COM_ErrorCode;

//���ﶨ����ͨ������ṹ���Լ�ͨ������ö��

typedef enum
{
    SPPRTR_UART,    //ͨ��֧�֣�supporter�� - UART
    SPPRTR_CAN,     //ͨ��֧�֣�supporter�� - CAN
    
}COM_Supporter;

//ͨ������ṹ�嶨��
typedef struct
{
    /***************  �������  ****************/
    
    //����������� - UART
    UART_HandleTypeDef *UartHandle;
    
    //����������� - CAN
    CAN_HandleTypeDef  *CanHandle;
    
    //�����ѡȡ��ͨ������
    COM_Supporter   COM_type;
    
    /*************  END -> �������  ************/
    
    /***************  �쳣����  ****************/
    //������
    COM_ErrorCode ErrorCode;
    
    //�Ա�����쳣������������
    char* ErrorDescription;
    
    /*************  END -> �쳣����  ************/
    
    /***************  ͨ�����ͨ��  ****************/
    //��Ǳ�����Ƿ񱻳�ʼ��������������ȫ��������Ȼ��������ʽ�ֹ���������HardFault��
    bool isInited;
    
    //���ջ�������ַ(Ԥ��˫����)
    void *pRxBuffer[2];
    
    //���ջ�������־��ָʾ��ǰ���ڽ������ݵĻ�������ţ�
    uint8_t RxBufFlag;
    
    //���ͻ�������ַ
    void *pTxBuffer;
    
    //���ͻ�������С
    int32_t TxBufSize;
    
    //���ݴ�����(����ָ��)
    bool (*DealData)(void *pRxBuffer);
    
    //���ݷ��ͷ���(����ָ��)
    bool (*SendData)(uint8_t SendCMD);
    
    //���ʹ���������
    uint32_t SendCnt;
    
    //���ݰ�����������ȷ��
    uint32_t RxPackRcvCnt;
    
    //���յ��Ĵ����������
    uint32_t RxErrorPackCnt;
    
    /*********  END -> ͨ�����ͨ��  *********/
    
    /************  ����ͨ��ר��  **************/
    
    //��ǰ�Ľ��ջ���������
    int32_t RxBufSize;
    
    //�´�Ӧ�����ܵ����ݳ��ȣ���������������ͷ���룩
    int32_t Rx_NextRcvLength;
    
    //��־�Ƿ����ڽ��н�������ͷ����
    bool isCorrectHead;
    
    /*********  END -> ����ͨ��ר��  *********/

    /************  CANͨ��ר��  **************/
    
    //CAN�ķ�������֡ͷ
    CAN_TxHeaderTypeDef *pTxHeader;
    
    //CAN�Ľ�������֡ͷ
    CAN_RxHeaderTypeDef *pRxHeader;
    
    //����Ƿ����ڽ��շ�ͷ����(����Ƿ�ʹ��ͷУ�麯��)
    bool IsRcvingUnhead;
    
    //CAN�ж�ĳһ֡�����Ƿ�Ϊ���ݰ���ͷ�ķ���(����ָ��)
    bool (*Can_IsHeadData)(void *pData, uint32_t DataLength);
    
    //�������ݳ��ȼ�����
    uint32_t RxlengthCnt;
    
    /*********  END -> CANͨ��ר��  *********/
    
}COMInfoTypedef;

#endif

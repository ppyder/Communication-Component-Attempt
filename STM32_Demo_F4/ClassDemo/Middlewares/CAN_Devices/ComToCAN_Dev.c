#include "ComToCAN_Dev.h"
#include "Comunication.h"
#include "M3508.h"

//��DevGroup1ͨ�ŵ�ͨ�����ʵ�嶨��
COMInfoTypedef DevGroup1_COM_Module = {0};
//��DevGroup2ͨ�ŵ�ͨ�����ʵ�嶨��
COMInfoTypedef DevGroup2_COM_Module = {0};

//�豸��1_ID����
uint32_t CAN_DevGroup1StdIDs[] = 
{
    0x200 + 0x01,   //M3508�ķ�������_���1
    0x200 + 0x02,   //M3508�ķ�������_���2
    0x200 + 0x03,   //M3508�ķ�������_���3
};

uint32_t DevGroup1StdIDNum = sizeof(CAN_DevGroup1StdIDs) / sizeof(uint32_t);

//�豸��2_ID����
uint32_t CAN_DevGroup2StdIDs[] = 
{
    0x00,   //û�У��˴�����ռλ
};

uint32_t DevGroup2StdIDNum = sizeof(CAN_DevGroup2StdIDs) / sizeof(uint32_t);

//�豸���ݴ���������
void (*pDev_DealFuncs[CAN_DevDataNum])(uint8_t *pData) = 
{
    Motor1DataDeal,     //M3508�ķ�������_���1
    Motor2DataDeal,     //M3508�ķ�������_���2
    Motor3DataDeal,     //M3508�ķ�������_���3
};

/*  �ڲ���������  */
static bool DealDevGroup1Data(void *pRxBuffer);
static bool SendDataToDevGroup1(uint8_t SendCMD);

static bool DealDevGroup2Data(void *pRxBuffer);
static bool SendDataToDevGroup2(uint8_t SendCMD);

/*  �ڲ���������  */
//����ͷ
CAN_TxHeaderTypeDef DevGroup1_TxHeader;
CAN_TxHeaderTypeDef DevGroup2_TxHeader;
//���ͻ�����
static Dev_TxBufTypedef DevGroup1_TxBuffer;
static Dev_TxBufTypedef DevGroup2_TxBuffer;
//����ͷ
CAN_RxHeaderTypeDef DevGroup1_RxHeader;
CAN_RxHeaderTypeDef DevGroup2_RxHeader;
//���ջ�����
Dev_RxBufTypedef DevGroup1_RxBuffer;
Dev_RxBufTypedef DevGroup2_RxBuffer;

/************  ��ʼ������  ***********/

//��ʼ��CAN�ӿڶ�Ӧ��ͨ�����
void DevGroup1_COM_ModuleInit(COMInfoTypedef *pModule)
{        
    //��CAN��ʽ��ʼ��ͨ�����
    COM_CAN_StructInit(pModule, &DevGroup1_COM_HCAN,
                        DealDevGroup1Data, SendDataToDevGroup1,
                        NULL,
                        &DevGroup1_RxHeader,
                        &DevGroup1_TxHeader,
                        &DevGroup1_RxBuffer, Dev_RX_BUFFERSIZE,
                        &DevGroup1_TxBuffer, sizeof(Dev_TxBufTypedef));
    
    /* ʹ�ܽ��գ������жϻص����� */
    CAN_Filter_Config(&DevGroup1_COM_HCAN);
    HAL_CAN_ActivateNotification(&DevGroup1_COM_HCAN, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY);
}

//��ʼ��CAN�ӿڶ�Ӧ��ͨ�����
void DevGroup2_COM_ModuleInit(COMInfoTypedef *pModule)
{        
    //��CAN��ʽ��ʼ��ͨ�����
    COM_CAN_StructInit(pModule, &DevGroup2_COM_HCAN,
                        DealDevGroup2Data, SendDataToDevGroup2,
                        NULL,
                        &DevGroup2_RxHeader,
                        &DevGroup2_TxHeader,
                        &DevGroup2_RxBuffer, Dev_RX_BUFFERSIZE,
                        &DevGroup2_TxBuffer, sizeof(Dev_TxBufTypedef));
    
    /* ʹ�ܽ��գ������жϻص����� */
    CAN_Filter_Config(&DevGroup2_COM_HCAN);
    HAL_CAN_ActivateNotification(&DevGroup2_COM_HCAN, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY);
}

/************  ���ݴ�����ӳ��  ***********/

//��ID��ô��������±�
CAN_DevDataID GetDevGroup1DealID(uint32_t ID)
{
    CAN_DevDataID Result = CAN_M3508Data1;
    uint32_t Cnt = 0, StdID = ID;
    
    //������ѯID
    for(Cnt = 0; Cnt < CAN_IDsNum; Cnt++)
    {
        if(CAN_DevGroup1StdIDs[Cnt] == StdID)
        {
            Result = (CAN_DevDataID)Cnt;
            break;
        }
    }
    
    return Result;
}

//��ID��ô��������±�
CAN_DevDataID GetDevGroup2DealID(uint32_t ID)
{
    CAN_DevDataID Result = CAN_M3508Data1;
    uint32_t Cnt = 0, StdID = ID;
    
    //������ѯID
    for(Cnt = 0; Cnt < CAN_IDsNum; Cnt++)
    {
        if(CAN_DevGroup2StdIDs[Cnt] == StdID)
        {
            Result = (CAN_DevDataID)(Cnt + CAN_DevGroup1Num);
            break;
        }
    }
    
    return Result;
}

/************  �Խ��յ������ݽ��д���  ***********/

//������յ��豸������
static bool DealDevGroup1Data(void *pRxBuffer)
{
    //��ȡ��������ַ
    Dev_RxBufTypedef *pBuffer = (Dev_RxBufTypedef *)pRxBuffer;
    //��ȡ���ݴ�������ID
    CAN_DevDataID DataID = GetDevGroup1DealID(pBuffer->DevID);
    
    //����ID���ö�Ӧ�����ݴ�����
    pDev_DealFuncs[DataID](pRxBuffer);
        
    return true;
}

//������յ��豸������
static bool DealDevGroup2Data(void *pRxBuffer)
{
    //��ȡ��������ַ
    Dev_RxBufTypedef *pBuffer = (Dev_RxBufTypedef *)pRxBuffer;
    //��ȡ���ݴ�������ID
    CAN_DevDataID DataID = GetDevGroup2DealID(pBuffer->DevID);
    
    //����ID���ö�Ӧ�����ݴ�����
    pDev_DealFuncs[DataID](pRxBuffer);
        
    return true;
}

/************  ���ݷ��ͺ���  ***********/

//�������豸��1���豸�������ݣ�Ĭ�Ϸ���ǰ�Ѿ������ݸ���ֵ�ˡ�
//����Ĵ��������Ч������һ����ʶ����
static bool SendDataToDevGroup1(uint8_t SendCMD)
{
    //�����Ƿ��ͳɹ�
    return COM_SendDataInTxBuffer(&DevGroup1_COM_Module);
}

//�������豸��2���豸�������ݣ�Ĭ�Ϸ���ǰ�Ѿ������ݸ���ֵ�ˡ�
//����Ĵ��������Ч������һ����ʶ����
static bool SendDataToDevGroup2(uint8_t SendCMD)
{
    //�����Ƿ��ͳɹ�
    return COM_SendDataInTxBuffer(&DevGroup2_COM_Module);
}


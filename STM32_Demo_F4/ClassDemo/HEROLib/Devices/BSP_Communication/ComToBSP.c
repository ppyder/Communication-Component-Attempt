#include "ComToBSP.h"

//��CANͨ�ŵ�ͨ�����ʵ�嶨��
COMInfoTypedef BSP_COM_Module = {0};

//��˫��ͨ�Ż���ʵ�����
Hf_DuplexTypedef BSP_HfCOM = {0};

enum BSP_RxMsgTypes BSP_RxMsgTypeFeedBack;
enum BSP_TxMsgTypes BSP_TxMsgTypeSaved;

/*  �ڲ���������  */
//����ͷ
CAN_TxHeaderTypeDef BSP_TxHeader = {0x2ff, 0x00, CAN_ID_STD, CAN_RTR_DATA, 8, DISABLE};
//���ͻ�����
static BSP_TxBufTypedef BSP_TxBuffer;
//����ͷ
CAN_RxHeaderTypeDef BSP_RxHeader;
//���ջ�����
BSP_RxBufTypedef BSP_RxBuffer;

/*  �ڲ���������  */
static bool DealBSPData(void *pRxBuffer);
static bool SendDataToBSP(uint8_t SendCMD);
static bool GetMsgFromBSPBuffer(BSP_RxBufTypedef *pBuffer);
static bool CAN_IsBSP_PackHead(void *pData, uint32_t DataLength);

uint8_t Rece_Date1[8] = {0};

//��ʼ��CAN���ӿڶ�Ӧ��ͨ�����
void BSP_COM_ModuleInit(COMInfoTypedef *pModule)
{
    //��λ��˫���ṹ��
    Hf_DuplexStructDeInit(&BSP_HfCOM);
    
    BSP_HfCOM.ErrorCntMax = BSP_REQUEST_ERROR_MAX;
    
    //��ͨ�����
    BSP_HfCOM.pCOM = pModule;
    
    //��Ӳ���ӿ�
    pModule->CanHandle = &BSP_COM_HCAN;
    pModule->COM_type = SPPRTR_CAN;
    
    //�����ݷ��ͽ���ͷ�ṹ��
    pModule->pRxHeader = &BSP_RxHeader;
    pModule->pTxHeader = &BSP_TxHeader;
    
    //�����ݴ�����
    pModule->DealData = DealBSPData;
    
    //�����ݷ��ͷ���
    pModule->SendData = SendDataToBSP;
    
    //����֡���ݴ�����
    pModule->Can_IsHeadData = CAN_IsBSP_PackHead;
    
    //�󶨽��ջ�������canΪ�����壩
    pModule->pRxBuffer[0]   = &BSP_RxBuffer;
    pModule->pRxBuffer[1]   = &BSP_RxBuffer;
    pModule->RxBufSize      = BSP_RX_BUFFERSIZE;
    pModule->IsRcvingUnhead = false;
    pModule->RxlengthCnt    = 0;
    
    pModule->RxBufFlag      = false;
    pModule->RxPackRcvCnt   = 0;
    pModule->RxErrorPackCnt = 0;
    
    //�󶨷��ͻ�����
    pModule->pTxBuffer = &BSP_TxBuffer;
    pModule->TxBufSize = sizeof(BSP_TxBufTypedef);
    pModule->SendCnt   = 0;
    
    /* ʹ�ܽ��գ������жϻص����� */
    CAN_Filter_Config(&BSP_COM_HCAN);
    HAL_CAN_ActivateNotification(&BSP_COM_HCAN, CAN_IT_RX_FIFO0_MSG_PENDING);
}

//��ȡ����У��ֵ
static uint32_t GetCheck(uint8_t *pData, uint32_t DataSize)
{
    uint32_t check = 0;
    
    //���У��
    for(uint32_t i = 0; i < DataSize; i++)
    {
        check += pData[i];
    }
    return check;
}

//�ж�һ֡�����ǲ��ǰ�ͷ
static bool CAN_IsBSP_PackHead(void *pData, uint32_t DataLength)
{
    uint8_t *pBuffer = pData;
    bool Result = false;
    
    //�Ƿ�������(һ֡�������8���ֽ�)
    if(DataLength < sizeof(BSP_DataHead) && DataLength < 8)
    {
        return false;
    }
    
    //ͷƥ��
    Result = (0xAA == pBuffer[0]) && (0xBB == pBuffer[1]);
    //���ݱ�ǩ��ֵ��Χ��
    Result = Result && (pBuffer[2] < BSPRx_Num) && (pBuffer[3] < BSPTx_Num);
    
    return Result;
}

//������յ����ذ������
//���ô˺�����ʱ����Ѿ��ܹ���֤�����ݳ��Ⱥ����͡�����ͷλ������ǰ�ˡ�������������
static bool DealBSPData(void *pRxBuffer)
{
    //��ȡ��������ַ
    BSP_RxBufTypedef *pBuffer = (BSP_RxBufTypedef *)pRxBuffer;
    uint8_t *pData = (uint8_t *)pRxBuffer;
    
    /*  ��������  */
    
    //��ȡУ��ֵ
    uint32_t check = GetCheck(pData, sizeof(BSP_RxBufTypedef) - sizeof(BSP_DataTail));
    
    //���Ĺؿ���У��ֵ
    if(pBuffer->Tail.Verification == check)				
    {
        Hf_HaveReceived(&BSP_HfCOM);
        
        //�������ݴ�����
        if(true == GetMsgFromBSPBuffer(pBuffer))
        {
            //��¼���λ�õ����ݷ���ֵ
            BSP_RxMsgTypeFeedBack = (enum BSP_RxMsgTypes)pBuffer->Head.MsgTypeID;
        }
        else
        {
            //��Ǳ��ν��յ�������Ϊ��������
            BSP_RxMsgTypeFeedBack = BSPRx_ErrorData;
        }
        
        return true;
    }    
    return false;
}

//��ͨ��У������ݻ������л������
static bool GetMsgFromBSPBuffer(BSP_RxBufTypedef *pBuffer)
{
    switch(pBuffer->Head.MsgTypeID)
    {
        case BSPRx_ErrorData:
            break;
        
        case BSPRx_EmptyData:
            break;
        
        case BSPRx_TestData:
            break;
        
        default: break;
    }
    
    return false;
}

//��CAN��������
static bool SendDataToBSP(uint8_t SendCMD)
{
    bool isSendThisCMD = true;    
    //��ȡ���ͻ�������ַ
    BSP_TxBufTypedef *TxBuffer = BSP_COM_Module.pTxBuffer;
    
    TxBuffer->Head.DataHead1 = 0xAA;
    TxBuffer->Head.DataHead2  =0xBB;
    TxBuffer->Head.MsgTypeID = SendCMD;
    TxBuffer->Head.FeedbackID = BSP_RxMsgTypeFeedBack;
    
    //д����������ݲ���Ϥ���η��������Ƿ����ϴεİ��Է�û�ӵ���������
    //isSendThisCMD = GetTxData(&(TxBuffer->Data), TxBuffer->Head.MsgTypeID);
    
    TxBuffer->Data.Test.CMD = 0.123456;

    //��ȡУ��ֵ
    TxBuffer->Tail.Verification = GetCheck((uint8_t*)TxBuffer, 
                                            BSP_COM_Module.TxBufSize - sizeof(TxBuffer->Tail));
    
    //���ν���Ϣ���������ȴ�״̬
    if(true == COM_SendData(&BSP_COM_Module))
    {
        //��¼���η�������Ϣ����
        BSP_TxMsgTypeSaved = (enum BSP_TxMsgTypes)SendCMD;
        
        return isSendThisCMD;
    }
    //�����Ƿ��ͳɹ�
    return false;
}

#include "ComToBSP.h"

//��BSPͨ�ŵ�ͨ�����ʵ�嶨��
COMInfoTypedef BSP_COM_Module = {0};

//��˫��ͨ�Ż���ʵ�����
Hf_DuplexTypedef BSP_HfCOM = {0};

enum BSP_RxMsgTypes BSP_RxMsgTypeFeedBack;
enum BSP_TxMsgTypes BSP_TxMsgTypeSaved;

/*  �ڲ���������  */
static bool DealBSPData(void *pRxBuffer);
static bool SendDataToBSP(uint8_t SendCMD);
static bool GetMsgFromBSPBuffer(BSP_RxBufTypedef *pBuffer);

//���ʹ�ð�䴮��
#if defined(USING_BSP_UART)

/*  �ڲ���������  */
//���ͻ�����
static BSP_TxBufTypedef BSP_TxBuffer;
//���ջ�����
BSP_RxBufTypedef BSP_RxBuffer[2];

//��ʼ��CAN���ӿڶ�Ӧ��ͨ�����
void BSP_COM_ModuleInit(COMInfoTypedef *pModule)
{
    //��λ��˫���ṹ��
    Hf_DuplexStructDeInit(&BSP_HfCOM);
    
    //�趨��˫�����󲵻ش�������
    BSP_HfCOM.ErrorCntMax = BSP_REQUEST_ERROR_MAX;
    
    //��ͨ�����
    BSP_HfCOM.pCOM = pModule;
    
    //��Ӳ���ӿ�
    pModule->UartHandle = &BSP_COM_HUART;
    
    //�����ʹ�õ�ͨ�������Ǵ���
    pModule->COM_type = SPPRTR_UART;
    
    //�����ݴ�����
    pModule->DealData = DealBSPData;
    
    //�����ݷ��ͷ���
    pModule->SendData = SendDataToBSP;
    
    //�󶨽��ջ�����
    pModule->pRxBuffer[0] = BSP_RxBuffer;
    pModule->pRxBuffer[1] = BSP_RxBuffer + 1;
    pModule->RxBufSize = BSP_RX_BUFFERSIZE;
    pModule->Rx_NextRcvLength = BSP_RX_BUFFERSIZE;
    pModule->isCorrectHead = false;
    pModule->RxBufFlag = false;
    pModule->RxPackRcvCnt = 0;
    pModule->RxErrorPackCnt = 0;
    
    //�󶨷��ͻ�����
    pModule->pTxBuffer = &BSP_TxBuffer;
    pModule->TxBufSize = sizeof(BSP_TxBufTypedef);
    pModule->SendCnt = 0;
    
    /* ʹ�ܽ��գ������жϻص����� */
    HAL_UART_Receive_IT(&BSP_COM_HUART, 
                        (uint8_t*)pModule->pRxBuffer[pModule->RxBufFlag], 
                        BSP_COM_Module.RxBufSize);
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

//У�鲢�������ͷ
static inline bool Check_FindHead(uint8_t *pData, uint32_t DataSize, 
                                        uint32_t TailSize, uint32_t *pHeadPos, uint32_t *pCheck)
{
    //������
    int32_t i;
    bool isFindDataHead = false;
    
    (*pCheck) = 0;
    
    //У�鲢Ѱ������ͷ
    for(i = 0, (*pHeadPos) = 0; i < DataSize; i++)	
    {
        //��ͷ�Χ�ǳ�����β�����������
        if(i < DataSize - TailSize)
        {
            (*pCheck) += pData[i];			    //���У��
        }
        
        if(0xaa == pData[i])				//��������ͷλ��
        {
            (*pHeadPos) = i;
        }
        if((0xbb == pData[i]) && ((*pHeadPos) == (i-1)))	//�����һλ��0xBB,��ǰһλ��0xAA��
        {											//��ô����ͷ�ڻ�������λ�þͿ���ȷ��
            isFindDataHead = true;
        }
    }
    
    return isFindDataHead;
}

//������յ����ذ������
static bool DealBSPData(void *pRxBuffer)
{
    //��ȡ��������ַ
    BSP_RxBufTypedef *pBuffer = (BSP_RxBufTypedef *)pRxBuffer;
    uint8_t *pData = (uint8_t *)pRxBuffer;
    
    /*  ��������  */
    
    //����ͷλ�á�У��ֵ
    uint32_t HeadPos = 0;
    uint32_t check = 0;

	if(BSP_COM_Module.RxBufSize != BSP_RX_BUFFERSIZE)
	{	
        //����������ڽ��в�����������������ͷλ��
		BSP_COM_Module.Rx_NextRcvLength = BSP_RX_BUFFERSIZE;
	}
	else
	{
        //�����λ�ͺ�һ�����ĵ�����
        if(BSP_COM_Module.isCorrectHead)
        {
            //����������ڽ��в�����������������ͷλ��
            BSP_COM_Module.Rx_NextRcvLength = BSP_RX_BUFFERSIZE;
            
            BSP_COM_Module.isCorrectHead = false;
            
            return false;
        }
        
        //��У��ֵ��Ѱ������ͷ
        bool isFindDataHead = Check_FindHead(pData, BSP_RX_BUFFERSIZE, sizeof(pBuffer->Tail), 
                                &HeadPos, &check);
        
        //��������ͷλ���쳣����������������Ӧ������ǰ�ˣ�
        if(!isFindDataHead)
        {
            //��ֹ�����if���Ż���
            volatile uint8_t BufTail = pData[BSP_RX_BUFFERSIZE - 1];
            
            if(0xAA == BufTail)
            {
                //���������ͷβ��λ�����Σ���ʱ����ͷ������β��
                BSP_COM_Module.Rx_NextRcvLength = BSP_RX_BUFFERSIZE - 1;
            }
            else
            {
                //������ѹ���Ͳ���������ͷ,�ͼ�����ԭ�����գ�ֱ���������г���������ͷ
                BSP_COM_Module.Rx_NextRcvLength = BSP_RX_BUFFERSIZE;
            }
        }
        else
        {
            //������ͷ������λ
            if(0 != HeadPos)
            {
                //��һ�ν�����ô��λ�Ϳ����������ݴ�λ
                BSP_COM_Module.Rx_NextRcvLength = HeadPos;
                
                BSP_COM_Module.isCorrectHead = true;
            }
            else
            {            
                //��������¾�Ӧ������ԭ������
                BSP_COM_Module.Rx_NextRcvLength = BSP_RX_BUFFERSIZE;
                
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
            }
        }
    }
    
    //��Ǳ��ν��յ�������Ϊ��Ч����
    BSP_RxMsgTypeFeedBack = BSPRx_ErrorData;
    
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

//��BSP��������
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

/* IS_USING_BSP_CAN  */ //END

//���ʹ�ð��CAN
#elif defined(USING_BSP_CAN)
/*  �ڲ���������  */
static bool CAN_IsBSP_PackHead(void *pData, uint32_t DataLength);

/*  �ڲ���������  */
//����ͷ
CAN_TxHeaderTypeDef BSP_TxHeader = {0x2ff, 0x00, CAN_ID_STD, CAN_RTR_DATA, 8, DISABLE};
//���ͻ�����
static BSP_TxBufTypedef BSP_TxBuffer;
//����ͷ
CAN_RxHeaderTypeDef BSP_RxHeader;
//���ջ�����
BSP_RxBufTypedef BSP_RxBuffer;

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
    HAL_CAN_ActivateNotification(&BSP_COM_HCAN, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY);
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

#endif /*  IS_USING_BSP_CAN  */

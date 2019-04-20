#include "Comunication.h"
#include "Com_CanDriver.h"
#include "Com_UserModules.h"

//���쳣������,������������ʹ��
char *COM_ErrorDescriptions[COM_ErrorCodeNum] = 
{
    "δ����",                                      // @COM_NoError
    "ʹ��ǰδ��ʼ����ͨ�������\n",                // @COM_Error_UnInited,
    "δ�����Map�������ҵ���������ݵ�����ţ�\n", // @COM_Error_UnFindModule
};

/* ˽�к������� */
void COM_ModuleErrorHandler(COM_ErrorCode ErrorCode, COMInfoTypedef *pModule);

//CAN��������ͷ����
CAN_RxHeaderTypeDef RxHeader;
//CAN�������ݽ��ջ���
uint8_t CANDataBuf[8] = {0};

/*******************  Ĭ�ϳ�ʼ������  ***********************/
/**
 * @brief	    �����ڷ�ʽ��ʼ��ͨ������ṹ��
 * @param[out]  pModule         ָ������ͨ�ŵ�ͨѶ����ṹ��(COMInfoTypedef*)
 * @param[in]   pHuart          ��Cube���ɵĴ�����ʹ�õĿڵĽṹ�����(UART_HandleTypeDef*)
 * @param[in]   pDealData       ���û�ʵ�ֵ����ݽ��մ�����ָ��(bool (*)(void*))
 * @param[in]   pSendData       ���û�ʵ�ֵ����ݷ��ʹ�����ָ��(bool (*)(uint8_t))
 * @param[in]   pRcvBuffer      ���ݽ��ջ�������ַ(void *)
 * @param[in]   RcvBufferSize   ���ݽ��ջ���������(uint32_t)
 * @param[in]   pSendBuffer     ���ݷ��ͻ�������ַ(void *)
 * @param[in]   SendBufferSize  ���ݷ��ͻ���������(uint32_t)
 * @retval	    None.
 */
void COM_UART_StructInit(COMInfoTypedef *pModule, UART_HandleTypeDef *pHuart,
                            bool (*pDealData)(void*), bool (*pSendData)(uint8_t),
                            void *pRcvBuffer, uint32_t RcvBufferSize,
                            void *pSendBuffer, uint32_t SendBufferSize)
{
    //�м���������ڽ�������ָ��
    uint8_t *pTemp = pRcvBuffer;
    
    //��Ӳ���ӿ�
    pModule->UartHandle = pHuart;
    
    //�����ʹ�õ�ͨ�������Ǵ���
    pModule->COM_type = SPPRTR_UART;
    
    //�����ݴ�����
    pModule->DealData = pDealData;
    
    //�����ݷ��ͷ���
    pModule->SendData = pSendData;
    
    //�󶨽��ջ�����,����Ҫ��˫����������
    pModule->pRxBuffer[0] = pTemp;
    pModule->pRxBuffer[1] = (pTemp + RcvBufferSize);
    pModule->RxBufSize = RcvBufferSize;
    pModule->Rx_NextRcvLength = RcvBufferSize;
    pModule->isCorrectHead = false;
    pModule->RxBufFlag = false;
    pModule->RxPackRcvCnt = 0;
    pModule->RxErrorPackCnt = 0;
    
    //�󶨷��ͻ�����
    pModule->pTxBuffer = pSendBuffer;
    pModule->TxBufSize = SendBufferSize;
    pModule->SendCnt = 0;
    
    //����Ѿ���ʼ�����
    pModule->isInited = true;
    
    //��λ�����־��������
    pModule->ErrorCode = COM_NoError;
    pModule->ErrorDescription = COM_ErrorDescriptions[COM_NoError];
    
}

/**
 * @brief	    ��CAN��ʽ��ʼ��ͨ������ṹ��
 * @param[out]  pModule         ָ������ͨ�ŵ�ͨѶ����ṹ��(COMInfoTypedef*)
 * @param[in]   pHuart          ��Cube���ɵĴ�����ʹ�õĿڵĽṹ�����(CAN_HandleTypeDef*)
 * @param[in]   pDealData       ���û�ʵ�ֵ����ݽ��մ�����ָ��(bool (*)(void*))
 * @param[in]   pSendData       ���û�ʵ�ֵ����ݷ��ʹ�����ָ��(bool (*)(uint8_t))
 * @param[in]   pCan_IsHeadData ���û�ʵ�ֵı�ʶĳ֡�����Ƿ������ݰ�ͷ�ĺ���ָ��(bool (*)(void*, uint32_t))
 * @param[in]   pRxHeader       ָ���������֡ͷ(CAN_RxHeaderTypeDef*)
 * @param[in]   pTxHeader       ָ��������֡ͷ(CAN_TxHeaderTypeDef*)
 * @param[in]   pRcvBuffer      ���ݽ��ջ�������ַ(void *)
 * @param[in]   RcvBufferSize   ���ݽ��ջ���������(uint32_t)
 * @param[in]   pSendBuffer     ���ݷ��ͻ�������ַ(void *)
 * @param[in]   SendBufferSize  ���ݷ��ͻ���������(uint32_t)
 * @retval	    None.
 */
void COM_CAN_StructInit(COMInfoTypedef *pModule, CAN_HandleTypeDef *pHCAN,
                            bool (*pDealData)(void*), bool (*pSendData)(uint8_t),
                            bool (*pCan_IsHeadData)(void*, uint32_t),
                            CAN_RxHeaderTypeDef *pRxHeader,
                            CAN_TxHeaderTypeDef *pTxHeader,
                            void *pRcvBuffer, uint32_t RcvBufferSize,
                            void *pSendBuffer, uint32_t SendBufferSize)
{
    //��Ӳ���ӿ�
    pModule->CanHandle = pHCAN;
    pModule->COM_type = SPPRTR_CAN;
    
    //�����ݷ��ͽ���ͷ�ṹ��
    pModule->pRxHeader = pRxHeader;
    pModule->pTxHeader = pTxHeader;
    
    //�����ݴ�����
    pModule->DealData = pDealData;
    
    //�����ݷ��ͷ���
    pModule->SendData = pSendData;
    
    //����֡���ݴ�����
    pModule->Can_IsHeadData = pCan_IsHeadData;
    
    //�󶨽��ջ�������canΪ�����壩
    pModule->pRxBuffer[0]   = pRcvBuffer;
    pModule->pRxBuffer[1]   = pRcvBuffer;
    pModule->RxBufSize      = RcvBufferSize;
    pModule->IsRcvingUnhead = false;
    pModule->RxlengthCnt    = 0;
    
    pModule->RxBufFlag      = false;
    pModule->RxPackRcvCnt   = 0;
    pModule->RxErrorPackCnt = 0;
    
    //�󶨷��ͻ�����
    pModule->pTxBuffer = pSendBuffer;
    pModule->TxBufSize = SendBufferSize;
    pModule->SendCnt   = 0;
    
    //����Ѿ���ʼ�����
    pModule->isInited = true;
    
    //��λ�����־��������
    pModule->ErrorCode = COM_NoError;
    pModule->ErrorDescription = COM_ErrorDescriptions[COM_NoError];
    
}

/*******************  END -> Ĭ�ϳ�ʼ������  ***********************/

/*******************  ����ķ��ͽӿ�  ***********************/
//��ȫ�ص������ֱ�ӷ��ͺ���
void SendData_Safely(COMInfoTypedef *pModule, uint8_t SendCMD)
{
    //����ͨ�������δ��ʼ����
    if(false == pModule->isInited)
    {
        COM_ModuleErrorHandler(COM_Error_UnInited, pModule);
    }
    
    //���÷��ͺ���
    pModule->SendData(SendCMD);
}

//CAN��������ID�ķǸ�ʽ������
bool COM_CANSendUnformatData(COMInfoTypedef *pModule, CAN_TxHeaderTypeDef *pHeader, uint8_t *pData, uint16_t Size)
{
    //����ͨ�������δ��ʼ����
    if(false == pModule->isInited)
    {
        COM_ModuleErrorHandler(COM_Error_UnInited, pModule);
    }
    
    if(Can_SendData(pModule->CanHandle, pModule->pTxHeader, pData, Size))
    {
        //��¼���ʹ���
        pModule->SendCnt++;
        
        return true;
    }
    return false;
}

//��ָ��ID���ͷǸ�ʽ������
bool COM_SendUnformatData(COMInfoTypedef *pModule, uint8_t *pData, uint32_t DataSzie)
{
    //����ͨ�������δ��ʼ����
    if(false == pModule->isInited)
    {
        COM_ModuleErrorHandler(COM_Error_UnInited, pModule);
    }
    
    //���õײ�ⷢ�ͺ���
    switch(pModule->COM_type)
    {
        case SPPRTR_UART:
            if(HAL_UART_Transmit_DMA(pModule->UartHandle, pData, DataSzie))
            {
                //��¼���ʹ���
                pModule->SendCnt++;
                
                return true;
            }
            break;
        
        case SPPRTR_CAN:
            if(Can_SendData(pModule->CanHandle, pModule->pTxHeader, pData, DataSzie))
            {
                //��¼���ʹ���
                pModule->SendCnt++;
                
                return true;
            }
            break;
        
        default:break;
    }
    
    return false;
}

//��Ϣ���ͺ��������Ѿ����������ݻ������е����ݽ��з���
bool COM_SendData(COMInfoTypedef *pModule)
{    
    volatile bool isSendOK = false;
    
    //����ͨ�������δ��ʼ����
    if(false == pModule->isInited)
    {
        COM_ModuleErrorHandler(COM_Error_UnInited, pModule);
    }
    
    //���õײ�ⷢ�ͺ���
    switch(pModule->COM_type)
    {
        case SPPRTR_UART:
            isSendOK = (HAL_OK == HAL_UART_Transmit_DMA(pModule->UartHandle, 
                                    pModule->pTxBuffer,
                                    pModule->TxBufSize));
            break;
        
        case SPPRTR_CAN:
            isSendOK = Can_SendData(pModule->CanHandle,
                            pModule->pTxHeader,
                            pModule->pTxBuffer,  
                            pModule->TxBufSize);
            
            break;
        
        default:break;
    }
    
    if(isSendOK)
    {
        //��¼���ʹ���
        pModule->SendCnt++;
        
        return true;
    }
    
    return false;
}

/*******************  END -> ����ķ��ͽӿ�  ***********************/

/*******************  ���ջص�����  ***********************/

//���ڽ��ջص�����
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    //��ȡ����Ҫ�����ͨ��������������Ϣ
    COM_ModuleID COM_Type = UART_DealFuncMap(UartHandle);
    uint8_t BufferFlag = COM_Modules[COM_Type]->RxBufFlag;
    void *pDataBuffer = COM_Modules[COM_Type]->pRxBuffer[BufferFlag];
    uint32_t DataSize = COM_Modules[COM_Type]->Rx_NextRcvLength;
    
    if(COM_NULL == COM_Type)
    {
        //��δ�ҵ����������յ����ݵ�ͨ������ṹ��
        COM_ModuleErrorHandler(COM_Error_UnFindModule, COM_Modules[COM_Type]);
    }
    else if(false == COM_Modules[COM_Type]->isInited)
    {
        //����ͨ�������δ��ʼ����
        COM_ModuleErrorHandler(COM_Error_UnInited, COM_Modules[COM_Type]);
    }
    
    //�л���������׼���´εĽ���
    HAL_UART_Receive_IT(UartHandle, COM_Modules[COM_Type]->pRxBuffer[!BufferFlag], DataSize);
    
    //�������ݴ����������½��ռ�����
    if(true == COM_Modules[COM_Type]->DealData(pDataBuffer))
    {
        COM_Modules[COM_Type]->RxPackRcvCnt++;
    }
    else
    {
        COM_Modules[COM_Type]->RxErrorPackCnt++;
    }
    
    //���µ�ǰ����������
    COM_Modules[COM_Type]->RxBufSize = DataSize;
    
    //��ת���ݱ�־
    COM_Modules[COM_Type]->RxBufFlag = !COM_Modules[COM_Type]->RxBufFlag;
}

//CAN������ɵĻص�����
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle)
{
    //��ȡͨ�������
    COM_ModuleID COM_Type = CAN_DealFuncMap(CanHandle);
    CAN_DataID DataID;
    
    //�����账��ȡ���ݱ��ν��յ�����֡
    HAL_CAN_GetRxMessage(CanHandle, CAN_RX_FIFO0, &RxHeader, CANDataBuf);
    
    //��ȡ���յ�����Ϣ����
    DataID = CAN_StdIDMap(&RxHeader);
    
    if(COM_NULL == COM_Type)
    {
        //��δ�ҵ����������յ����ݵ�ͨ������ṹ��
        COM_ModuleErrorHandler(COM_Error_UnFindModule, COM_Modules[COM_Type]);
    }
    else if(false == COM_Modules[COM_Type]->isInited)
    {
        //����ͨ�������δ��ʼ����
        COM_ModuleErrorHandler(COM_Error_UnInited, COM_Modules[COM_Type]);
    }
    
    //��ID���ദ������
    pCANDataDealFuncs[DataID](CANDataBuf, RxHeader.DLC, COM_Type);
}

//ͨ������쳣�ص�����
void COM_ModuleErrorHandler(COM_ErrorCode ErrorCode, COMInfoTypedef *pModule)
{
    //�����������״̬
    while(COM_NoError != ErrorCode)
    {
        //��������˵�������õ����ͨ������������⣬���ڶ�Ӧ����Ĵ��������в鿴���������ַ�Ѵ��뱾������
        pModule->ErrorCode = ErrorCode;
        pModule->ErrorDescription = COM_ErrorDescriptions[ErrorCode];
    }
}

/*******************  END -> ���ջص�����  ***********************/

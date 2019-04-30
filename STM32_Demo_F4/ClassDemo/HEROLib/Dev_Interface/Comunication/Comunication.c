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
static bool COM_SendData_Unsafe(COMInfoTypedef *pModule, void *pData, uint32_t DataSzie);

//CAN��������ͷ����
CAN_RxHeaderTypeDef RxHeader;
//CAN�������ݽ��ջ���,ǰ8�������ڴ洢���ݣ����ĸ������ڴ洢����ID
uint8_t CANDataBuf[12] = {0};

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
    
    //��λ״̬��־
    pModule->isPrintingData = false;
    
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
    
    //��λ״̬��־
    pModule->isPrintingData = false;
    
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
    else if(pModule->isPrintingData)  //������ڴ�ӡ����
    {
        //ʲôҲ����
    }
    else
    {
        //���÷��ͺ���
        pModule->SendData(SendCMD);
    }
}

//CAN��������ID�ķǸ�ʽ������
bool COM_CANSendUnformatIDData(COMInfoTypedef *pModule, CAN_TxHeaderTypeDef *pHeader, uint8_t *pData, uint16_t Size)
{
    //����ͨ�������δ��ʼ����
    if(false == pModule->isInited)
    {
        COM_ModuleErrorHandler(COM_Error_UnInited, pModule);
    }
    else if(pModule->isPrintingData)  //������ڴ�ӡ����
    {
        //ʲôҲ����
    }
    else if(Can_SendData(pModule->CanHandle, pModule->pTxHeader, pData, Size))
    {
        //��¼���ʹ���
        pModule->SendCnt++;
        
        return true;
    }
    return false;
}

//���ͷǸ�ʽ������
bool COM_SendUnformatData(COMInfoTypedef *pModule, uint8_t *pData, uint32_t DataSzie)
{
    //����ͨ�������δ��ʼ����
    if(false == pModule->isInited)
    {
        COM_ModuleErrorHandler(COM_Error_UnInited, pModule);
    }
    else if(pModule->isPrintingData)  //������ڴ�ӡ����
    {
        //ʲôҲ����
    }
    else if(true == COM_SendData_Unsafe(pModule, pData, DataSzie))
    {
        //��¼���ʹ���
        pModule->SendCnt++;
        
        return true;
    }
    
    return false;
}

//�ڲ�����Ƿ��ʼ����ǰ���½������ݷ���,�����Ƿ��ͳɹ�
static bool COM_SendData_Unsafe(COMInfoTypedef *pModule, void *pData, uint32_t DataSzie)
{
    //���õײ�ⷢ�ͺ���
    switch(pModule->COM_type)
    {
        case SPPRTR_UART:
            return HAL_OK == HAL_UART_Transmit_DMA(pModule->UartHandle, pData, DataSzie);
        
        case SPPRTR_CAN:
            return true == Can_SendData(pModule->CanHandle, pModule->pTxHeader, pData, DataSzie);
        
        default:break;
    }
    
    return false;
}

//��Ϣ���ͺ��������Ѿ����������ݻ������е����ݽ��з���,������������ڲ������ݷ��ͺ����б�����
bool COM_SendDataInTxBuffer(COMInfoTypedef *pModule)
{        
    //����ͨ�������δ��ʼ����
    if(false == pModule->isInited)
    {
        COM_ModuleErrorHandler(COM_Error_UnInited, pModule);
    }
    
    //���÷��ͺ���
    if(true == COM_SendData_Unsafe(pModule, pModule->pTxBuffer, pModule->TxBufSize))
    {
        //��¼���ʹ���
        pModule->SendCnt++;
        
        return true;
    }
        
    return false;
}

uint32_t isBlockedCnt = 0;

//�ض���fputc������ʹ��printf����
int fputc(int ch, FILE* stream) 
{
    volatile bool isBlocked = false;
    uint8_t Send_ch = (uint8_t)ch;
    
    //����ͨ�������δ��ʼ����
    if(false == COM_Modules[COM_PRINTF_ID]->isInited)
    {
        COM_ModuleErrorHandler(COM_Error_UnInited, COM_Modules[COM_PRINTF_ID]);
    }
    
    COM_Modules[COM_PRINTF_ID]->isPrintingData = true;
    
    //�ȴ�ֱ�����Ͳ�����
    while(HAL_UART_STATE_BUSY_TX == (HAL_UART_STATE_BUSY_TX & COM_Modules[COM_PRINTF_ID]->UartHandle->gState))
    {
        isBlockedCnt++;
    }
    
    //���ַ���
    if(true == COM_SendData_Unsafe(COM_Modules[COM_PRINTF_ID], &Send_ch, 1))
    {
        //��¼���ʹ���
        COM_Modules[COM_PRINTF_ID]->SendCnt++;
    }
    
    //�ж���û�з�����,Լ���Ի��з�'\n'Ϊ�ַ��������ı�־��
    COM_Modules[COM_PRINTF_ID]->isPrintingData = ('\n' != Send_ch);
    
    return ch; 
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
    COM_ModuleID COM_Type = COM_NULL;
    CAN_DataID DataID;
    uint32_t *pIDBuffer = (uint32_t *)(CANDataBuf + 8);
    
    //�����账��ȡ���ݱ��ν��յ�����֡
    HAL_CAN_GetRxMessage(CanHandle, CAN_RX_FIFO0, &RxHeader, CANDataBuf);
    
    //��IDд�뻺����
    (*pIDBuffer) = RxHeader.StdId;
    
    //��ȡͨ�������
    COM_Type = CAN_DealFuncMap(CanHandle, &RxHeader, &DataID);
    
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

#include "Comunication.h"
#include "Com_CanDriver.h"
#include "ComToPC.h"
#include "ComToMaster.h"
#include "ComToBSP.h"
#include "M3508.h"

//ͨ���������
COMInfoTypedef *COM_Modules[COM_ModulesNum] = 
{
    //��С���Ե�ͨ������ӿ�
    &PC_COM_Module,
    
    //����ƶ˵�ͨ������ӿ�
    &Mstr_COM_Module,
    
    //���������ӵ�ͨ������ӿ�
    &BSP_COM_Module,
};

//can���ݴ���������
void (*pCANDataDealFuncs[CAN_IDsNum])(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type) = 
{
    RcvFormatCanData,   //��ʽ�����ݽ���
    Motor1DataDeal,     //M3508�ķ�������_���1
    Motor2DataDeal,     //M3508�ķ�������_���2
    Motor3DataDeal,     //M3508�ķ�������_���3
};

//CAN��������ͷ����
CAN_RxHeaderTypeDef RxHeader;
//CAN�������ݽ��ջ���
uint8_t CANDataBuf[8] = {0};

/*******************  Map Of Modules  ***********************/

//ѡ������
static inline COM_ModuleID UART_DealFuncMap(UART_HandleTypeDef *UartHandle)
{
    COM_ModuleID result;
    
    if(PC_COM_UART == UartHandle->Instance)
    {
        result = COM_MSTR;//COM_PC;
    }
    
//���ʹ�ð�䴮��
#if defined(USING_BSP_UART)

    if(BSP_COM_UART == UartHandle->Instance)
    {
        result = COM_BSP;
    }

#endif /*  IS_USING_BSP_CAN  */
    
    return result;
}

//ѡ������
static inline COM_ModuleID CAN_DealFuncMap(CAN_HandleTypeDef *CanHandle)
{
    COM_ModuleID result;
    
//���ʹ�ð��CAN
#if defined(USING_BSP_CAN)

    if(BSP_COM_CAN == CanHandle->Instance)
    {
        result = COM_BSP;
    }

#endif /*  IS_USING_BSP_CAN  */
    
    return result;
}

/*******************  END -> Map Of Modules  ***********************/

/*******************  ����ķ��ͽӿ�  ***********************/

//CAN��������ID�ķǸ�ʽ������
bool COM_CANSendUnformatData(COMInfoTypedef *pModule, CAN_TxHeaderTypeDef *pHeader, uint8_t *pData, uint16_t Size)
{
    if(Can_SendData(pModule->CanHandle, pModule->pTxHeader, pData, Size))
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

//��Ϣ���ͺ���
bool COM_SendData(COMInfoTypedef *pModule)
{    
    volatile bool isSendOK = false;
    
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
    
    //��ID���ദ������
    pCANDataDealFuncs[DataID](CANDataBuf, RxHeader.DLC, COM_Type);
}

/*******************  END -> ���ջص�����  ***********************/

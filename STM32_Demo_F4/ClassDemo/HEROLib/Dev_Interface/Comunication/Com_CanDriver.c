#include "Com_CanDriver.h"

/****************  CAN���е����ݴ���������  ***********************/

//CAN ��׼ID ӳ�䣨�����Ӧͷ�ļ��е�ö�٣�

/**
 *  ע�⣺�Զ���ͨ��Э���ID���Լ�ָ���ġ�
 *      ָ����ʱ����Ҫע�⣬����ʹ���˰���ʶ�����ͣ��������ȼ��������
 *      ��ʶ��ֵ��С����Ϣ���и��ߵ����ȼ�������ʶ��ֵ���ʱ�����Ȱ���
 *      ��Ÿ�С�����䷢�͡���ˣ����ǵ��豸ָ��ֻ��Ҫ��֡���͡���Э��
 *      ָ����Ҫ�����֡���ͣ����������Э��ָ���IDȡ�ñ��豸ָ���
 *
 *      ���⣬����Ҫע�⣬ʹ��ͬһ��can���з��͵�ʱ����Ҫע��Ƶ����ϡ�
 *      ���������֪������1Mbps�Ĳ����ʼ��㣬����һ֡���������Ҫ���ѵ�
 *      ��Լ0.2ms��ʱ�䣬�ҿ��ǵ��������������Ϳ�λ�������Ƽ�ʹ�õ����
 *      ����Ƶ���ǵ�֡����1ms����һ�Σ��������>10msһ�Ρ�
 */

//���Զ���ͨ��Э�鷢�͵�����ID����
const uint32_t CAN_FormatStdIDs[] = 
{
    0x2FF,           //���ͨ��ID
};

//�Է��Զ���ͨ��Э�鷢�͵�����ID����
const uint32_t CAN_UnformatStdIDs[] = 
{
    0x200 + 0x01,   //M3508�ķ�������_���1
    0x200 + 0x02,   //M3508�ķ�������_���2
    0x200 + 0x03,   //M3508�ķ�������_���3
};

//CAN��������״ָ̬ʾ(�Ƿ����)
bool isTxMailBoxFree[3] = {true, true, true};

//CAN�����˲�����ʼ������
void CAN_Filter_Config(CAN_HandleTypeDef* hcan)
{
	CAN_FilterTypeDef  sFilterConfig;

	sFilterConfig.FilterBank = 0;                       //��������0
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;   //�����ڱ�ʶ������λģʽ
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;  //������λ��Ϊ����32λ��
    
    /* ʹ�ܱ��ı�ʾ�����������ձ�ʾ�������ݽ��бȶԹ��ˣ���չID�������µľ����������ǵĻ��������FIFO0�� */
    sFilterConfig.FilterIdHigh         = 0x0000;            //Ҫ���˵�ID��λ 
    sFilterConfig.FilterIdLow          = 0x0000;            //Ҫ���˵�ID��λ 
    sFilterConfig.FilterMaskIdHigh     = 0x0000;	        //��������16λÿλ����ƥ��
    sFilterConfig.FilterMaskIdLow      = 0x0000;	        //��������16λÿλ����ƥ��
    sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;  //��������������FIFO 0
    sFilterConfig.FilterActivation = ENABLE;                //ʹ�ܹ�����
    
    //���ǵ�һ�������������can
	if( hcan->Instance == CAN1)
	{
		sFilterConfig.SlaveStartFilterBank = 0;
	}
	else
	{
		sFilterConfig.SlaveStartFilterBank = 14;
	}
    
    HAL_CAN_ConfigFilter(hcan, &sFilterConfig);
  
    HAL_CAN_Start(hcan);
}

/**************************  �������ݴ���  ******************************/

//CAN���͵�֡���ݣ���ʱSizeӦ����С�ڵ���8��
bool CanSendSingleData(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *pHeader, uint8_t *pData, uint16_t Size)
{
    //�ȴ���ֱ������һ��������У����߹��˳�ʱʱ�䣬��ʱʱ��δ�趨��
    while(!isTxMailBoxFree[0] && !isTxMailBoxFree[1] && !isTxMailBoxFree[1]);
    
    //��Ƿ��ͳ���
    pHeader->DLC = Size;
    
    //��ӵ����յ��Ǹ���������
    if(isTxMailBoxFree[0])
    {
        if (HAL_CAN_AddTxMessage(hcan, pHeader, pData, (uint32_t *)CAN_TX_MAILBOX0) != HAL_OK)
        {
              /* Transmission request Error */
              Error_Handler();
        }
        isTxMailBoxFree[0] = false;
    }
    else if(isTxMailBoxFree[1])
    {
        if (HAL_CAN_AddTxMessage(hcan, pHeader, pData, (uint32_t *)CAN_TX_MAILBOX1) != HAL_OK)
        {
              /* Transmission request Error */
              Error_Handler();
        }
        isTxMailBoxFree[1] = false;
    }
    else if(isTxMailBoxFree[2])
    {
        if (HAL_CAN_AddTxMessage(hcan, pHeader, pData, (uint32_t *)CAN_TX_MAILBOX2) != HAL_OK)
        {
              /* Transmission request Error */
              Error_Handler();
        }
        isTxMailBoxFree[1] = false;
    }
    else
    {
        return false;
    }
    
    return true;
}

//CAN������ͺ���(��������)
bool Can_SendData(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *pHeader, uint8_t *pData, uint16_t Size)
{
    //���һ֡���ܷ���
    if(Size <= 8)
    {
        //ֱ�ӷ���
        CanSendSingleData(hcan, pHeader, pData, Size);
    }
    else
    {
        //����Ҫ�ּ��η���ȥ����ֵһ���Ǵ���һ�ġ�
        uint32_t SendCnt = (Size / 8) + ((Size % 8 == 0) ? 0 : 1);
        //��ַƫ����
        uint32_t IndexOffset = 0;
        
        //�ְ�����
        for( ;SendCnt > 1; SendCnt--)
        {
            CanSendSingleData(hcan, pHeader, pData + IndexOffset, 8);
            
            IndexOffset += 8;
        }
        
        //���һ����������Դ�����Ϊ��һ����8���ֽ���ô��
        if(Size % 8 == 0)
        {
            CanSendSingleData(hcan, pHeader, pData + IndexOffset, 8);
        }
        else
        {
            CanSendSingleData(hcan, pHeader, pData + IndexOffset, Size % 8);
        }
    }
    
    return true;
}

/***********************  END -> �������ݴ���  ***************************/

/**************************  �������ݴ���  ******************************/

//���ݿ�������
void DataCopy(uint8_t *pTarget, const uint8_t *pSource, uint32_t DataSize)
{
    uint32_t Cnt = 0;
    for(Cnt = 0; Cnt < DataSize; Cnt++)
    {
        pTarget[Cnt] = pSource[Cnt];
    }
    return;
}

//��������յ���������(��ʽ������/�Ǹ�ʽ������һ��)
CAN_DataID CAN_StdIDMap(CAN_RxHeaderTypeDef *pHeader)
{
    CAN_DataID Result;
    uint32_t Cnt = 0, StdID = pHeader->StdId;
    
    //������ѯID
    for(Cnt = 0; Cnt < CAN_IDsNum; Cnt++)
    {
        if(CAN_FormatStdIDs[Cnt] == StdID)
        {
            Result = CAN_FormatData;
            break;
        }
        else if(CAN_UnformatStdIDs[Cnt] == StdID)
        {
            // @CAN_DataID �� CAN_FormatData ���Ϊ0��
            //��˷Ǹ�ʽ�����ݶ�Ӧ��ö��ֵӦΪ��ѯ���õ������±��һ
            Result = (CAN_DataID)(Cnt + 1);
            break;
        }
    }
    
    return Result;
}

//��һ��������ͷ������֡���д���
void DealHeadData(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type)
{
    //�жϵ�ǰ�����յ��������ǲ��Ǻ�ͷ������
    COM_Modules[COM_Type]->IsRcvingUnhead = COM_Modules[COM_Type]->Can_IsHeadData(pData, DataLength);

    if(COM_Modules[COM_Type]->IsRcvingUnhead)
    {
        //������д����ջ�����
        DataCopy((uint8_t *)COM_Modules[COM_Type]->pRxBuffer[COM_Modules[COM_Type]->RxBufFlag] 
                    + COM_Modules[COM_Type]->RxlengthCnt, pData, DataLength);
        
        COM_Modules[COM_Type]->RxlengthCnt += DataLength;
    }
}

//��ĳ���Զ����ʽ��ͨ��Э����յ�����
void RcvFormatCanData(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type)
{
    //��ȡ��ʹ�õ����ݻ��������
    uint8_t BufferFlag = COM_Modules[COM_Type]->RxBufFlag;
    //��ȡ���ݽ��ջ�������ַ
    void *pDataBuffer = COM_Modules[COM_Type]->pRxBuffer[BufferFlag];
    
    //������ڽ��յ��Ƿ�ͷ���ݣ��Ѿ���������ͷ��
    if(COM_Modules[COM_Type]->IsRcvingUnhead)
    {
        //���зǷ���飨��������Խ�磩
        if(COM_Modules[COM_Type]->RxlengthCnt + DataLength > COM_Modules[COM_Type]->RxBufSize)
        {
            COM_Modules[COM_Type]->RxlengthCnt = 0;
            
            //���ǿ��ܳ�����һ֡������������ͷ�����
            DealHeadData(pData, DataLength, COM_Type);
            
            return;
        }
        
        //������д����ջ�����
        DataCopy((uint8_t *)COM_Modules[COM_Type]->pRxBuffer[COM_Modules[COM_Type]->RxBufFlag] 
                    + COM_Modules[COM_Type]->RxlengthCnt, pData, DataLength);
        
        COM_Modules[COM_Type]->RxlengthCnt += DataLength;
        
        //��������Ѿ�����
        if(COM_Modules[COM_Type]->RxlengthCnt >= COM_Modules[COM_Type]->RxBufSize)
        {
            COM_Modules[COM_Type]->RxlengthCnt = 0;
            
            //�������ݴ����������½��ռ�����
            if(true == COM_Modules[COM_Type]->DealData(pDataBuffer))
            {
                COM_Modules[COM_Type]->RxPackRcvCnt++;
            }
            else
            {
                COM_Modules[COM_Type]->RxErrorPackCnt++;
            }
        }
        else
        {
            //��ʱ����δ��
            return;
        }
    }
    else
    {
        DealHeadData(pData, DataLength, COM_Type);
        return;
    }        
}

/***********************  END -> �������ݴ���  ***************************/

//CAN����0������ɻص�����
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
    isTxMailBoxFree[0] = true;
}

//CAN����1������ɻص�����
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
    isTxMailBoxFree[1] = true;
}

//CAN����2������ɻص�����
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
    isTxMailBoxFree[2] = true;
}

/*************  END -> CAN���е����ݴ���������  ********************/

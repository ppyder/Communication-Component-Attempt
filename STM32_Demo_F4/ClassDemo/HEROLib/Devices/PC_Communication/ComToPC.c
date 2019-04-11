#include "ComToPC.h"

//��С����ͨ�ŵ�ͨ�����ʵ�嶨��
COMInfoTypedef PC_COM_Module = {0};

//��ȡ�������ݹ�ʾ
FollowLineType  LineData = {0};
CrossDataType   CrossData = {0};
PickDataType    PickData = {0};

/*  �ڲ���������  */
//���ͻ�����
static PC_TxBufTypedef PC_TxBuffer;
//���ջ�����
//static PC_RxBufTypedef PC_RxBuffer[2];
PC_RxBufTypedef PC_RxBuffer[2];

/*  �ڲ���������  */
static bool DealPCData(void *pRxBuffer);
static bool SendDataToPC(uint8_t SendCMD);
static void GetMsgFromPCBuffer(PC_RxBufTypedef *pBuffer);

//��ʼ�����ӿڶ�Ӧ��ͨ�����
void PC_COM_ModuleInit(volatile COMInfoTypedef *pModule)
{
    //��Ӳ���ӿ�
    pModule->UartHandle = &PC_COM_HUART;
    
    //�����ʹ�õ��Ǵ���
    pModule->COM_type = SPPRTR_UART;
    
    //�����ݴ�����
    pModule->DealData = DealPCData;
    
    //�����ݷ��ͷ���
    pModule->SendData = SendDataToPC;
    
    //�󶨽��ջ�����
    pModule->pRxBuffer[0] = PC_RxBuffer;
    pModule->pRxBuffer[1] = PC_RxBuffer + 1;
    pModule->RxBufSize = PC_RX_BUFFERSIZE;
    pModule->Rx_NextRcvLength = PC_RX_BUFFERSIZE;
    pModule->isCorrectHead = false;
    pModule->RxBufFlag = false;
    pModule->RxPackRcvCnt = 0;
    pModule->RxErrorPackCnt = 0;
    
    //�󶨷��ͻ�����
    pModule->pTxBuffer = &PC_TxBuffer;
    pModule->TxBufSize = sizeof(PC_TxBufTypedef);
    pModule->SendCnt = 0;
    
    /* ʹ�ܽ��գ������жϻص����� */
//    HAL_UART_Receive_IT(&PC_COM_HUART, 
//                        (uint8_t*)pModule->pRxBuffer[pModule->RxBufFlag], 
//                        PC_RX_BUFFERSIZE);
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

//������յ�С���Ե�����
static bool DealPCData(void *pRxBuffer)
{
    //��ȡ��������ַ
    PC_RxBufTypedef *pBuffer = (PC_RxBufTypedef *)pRxBuffer;
    uint8_t *pData = (uint8_t *)pRxBuffer;
    
    /*  ��������  */
    
    //����ͷλ�á�У��ֵ
    uint32_t HeadPos = 0;
    uint32_t check = 0;

	if(PC_COM_Module.RxBufSize != PC_RX_BUFFERSIZE)
	{	
        //����������ڽ��в�����������������ͷλ��
		PC_COM_Module.Rx_NextRcvLength = PC_RX_BUFFERSIZE;
	}
	else
	{
        //�����λ�ͺ�����
        if(PC_COM_Module.isCorrectHead)
        {
            //����������ڽ��в�����������������ͷλ��
            PC_COM_Module.Rx_NextRcvLength = PC_RX_BUFFERSIZE;
            
            PC_COM_Module.isCorrectHead = false;
            
            return false;
        }
        
        //��У��ֵ��Ѱ������ͷ
        volatile bool isFindDataHead = Check_FindHead(pData, PC_RX_BUFFERSIZE, sizeof(pBuffer->Tail), 
                                &HeadPos, &check);
        
        //��������ͷλ���쳣����������������Ӧ������ǰ�ˣ�
        if(!isFindDataHead)
        {
            //��ֹ�����if���Ż���
            volatile uint8_t BufTail = pData[PC_RX_BUFFERSIZE - 1];
            
            if(0xAA == BufTail)
            {
                //���������ͷβ��λ�����Σ���ʱ����ͷ������β��
                PC_COM_Module.Rx_NextRcvLength = PC_RX_BUFFERSIZE - 1;
            }
            else
            {
                //������ѹ���Ͳ���������ͷ,�ͼ�����ԭ�����գ�ֱ���������г���������ͷ
                PC_COM_Module.Rx_NextRcvLength = PC_RX_BUFFERSIZE;
            }
        }
        else
        {
            //������ͷ������λ
            if(0 != HeadPos)
            {
                //��һ�ν�����ô��λ�Ϳ����������ݴ�λ
                PC_COM_Module.Rx_NextRcvLength = HeadPos;
                
                PC_COM_Module.isCorrectHead = true;
            }
            else
            {            
                //��������¾�Ӧ������ԭ������
                PC_COM_Module.Rx_NextRcvLength = PC_RX_BUFFERSIZE;
                
                //���Ĺؿ���У��ֵ
                if(pBuffer->Tail.Verification == check)				
                {                
                    //�������ݴ�����
                    GetMsgFromPCBuffer(pBuffer);
                    
                    return true;
                }
            }
        }
    }
    
    return false;
}

//��ͨ��У������ݻ������л������
static void GetMsgFromPCBuffer(PC_RxBufTypedef *pBuffer)
{
    switch(pBuffer->Head.MsgTypeID)
    {
        case PCRx_FollowMode: LineData = pBuffer->Data.Line;
                            break;
        
        case PCRx_CrossMode:  CrossData = pBuffer->Data.Cross;
                            break;
        
        case PCRx_PickMode:   PickData = pBuffer->Data.Pick;
                            break;
                
        default: break;
    }
}

//��С���Է�������
static bool SendDataToPC(uint8_t SendCMD)
{
    //��ȡ���ͻ�������ַ
    PC_TxBufTypedef *TxBuffer = PC_COM_Module.pTxBuffer;
    
    TxBuffer->Head.DataHead1 = 0xAA;
    TxBuffer->Head.DataHead2  =0xBB;
    TxBuffer->Head.MsgTypeID = SendCMD;
    TxBuffer->Head.Reserve = 0x00;
    
    //��ȡУ��ֵ
    TxBuffer->Tail.Verification = GetCheck((uint8_t*)TxBuffer, 
                                            PC_COM_Module.TxBufSize - sizeof(TxBuffer->Tail));
    
    //�����Ƿ��ͳɹ�
    return COM_SendData(&PC_COM_Module);
}

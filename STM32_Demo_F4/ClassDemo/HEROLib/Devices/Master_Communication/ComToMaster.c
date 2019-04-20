#include "ComToMaster.h"
#include "Comunication.h"
#include "MasterDataDeal.h"

//����ƶ�ͨ�ŵ�ͨ�����ʵ�嶨��
COMInfoTypedef Mstr_COM_Module = {0};
//��˫��ͨ�Ż���ʵ�����
Hf_DuplexTypedef Mstr_HfCOM = {0};

/*  �ڲ���������  */
//���ͻ�����
Mstr_TxBufTypedef Mstr_TxBuffer;
//���ջ�����
Mstr_RxBufTypedef Mstr_RxBuffer[2];

/*  �ڲ���������  */
static bool DealMstrData(void *pRxBuffer);
static bool SendDataToMstr(uint8_t SendCMD);

//��ʼ�����ӿڶ�Ӧ��ͨ�����
void Mstr_COM_ModuleInit(COMInfoTypedef *pModule)
{
    //��ʼ����˫���ṹ�壬��ͨ���������ʹ����������
    Hf_DuplexStructInit(&Mstr_HfCOM, pModule, Mstr_REQUEST_ERROR_MAX, NULL);
        
    //�Դ��ڷ�ʽ��ʼ��ͨ�����
    COM_UART_StructInit(pModule, &Mstr_COM_HUART,
                        DealMstrData, SendDataToMstr,
                        Mstr_RxBuffer, Mstr_RX_BUFFERSIZE,
                        &Mstr_TxBuffer, sizeof(Mstr_TxBufTypedef));
    
    /* ʹ�ܽ��գ������жϻص����� */
    HAL_UART_Receive_IT(&Mstr_COM_HUART, 
                        (uint8_t*)pModule->pRxBuffer[pModule->RxBufFlag], 
                        Mstr_COM_Module.RxBufSize);
}


//��ȡ�������У��ֵ
static uint32_t GetAddCheck(uint8_t *pData, uint32_t DataSize)
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

//������յ����ƶ˵�����
static bool DealMstrData(void *pRxBuffer)
{
    //��ȡ��������ַ
    Mstr_RxBufTypedef *pBuffer = (Mstr_RxBufTypedef *)pRxBuffer;
    uint8_t *pData = (uint8_t *)pRxBuffer;
    
    /*  ��������  */
    
    //����ͷλ�á�У��ֵ
    uint32_t HeadPos = 0;
    uint32_t check = 0;

	if(Mstr_COM_Module.RxBufSize != Mstr_RX_BUFFERSIZE)
	{	
        //����������ڽ��в�����������������ͷλ��
		Mstr_COM_Module.Rx_NextRcvLength = Mstr_RX_BUFFERSIZE;
	}
	else
	{
        //�����λ�ͺ�һ�����ĵ�����
        if(Mstr_COM_Module.isCorrectHead)
        {
            //����������ڽ��в�����������������ͷλ��
            Mstr_COM_Module.Rx_NextRcvLength = Mstr_RX_BUFFERSIZE;
            
            Mstr_COM_Module.isCorrectHead = false;
            
            return false;
        }
        
        //��У��ֵ��Ѱ������ͷ
        bool isFindDataHead = Check_FindHead(pData, Mstr_RX_BUFFERSIZE, sizeof(pBuffer->Tail), 
                                &HeadPos, &check);
        
        //��������ͷλ���쳣����������������Ӧ������ǰ�ˣ�
        if(!isFindDataHead)
        {
            //��ֹ�����if���Ż���
            volatile uint8_t BufTail = pData[Mstr_RX_BUFFERSIZE - 1];
            
            if(0xAA == BufTail)
            {
                //���������ͷβ��λ�����Σ���ʱ����ͷ������β��
                Mstr_COM_Module.Rx_NextRcvLength = Mstr_RX_BUFFERSIZE - 1;
            }
            else
            {
                //������ѹ���Ͳ���������ͷ,�ͼ�����ԭ�����գ�ֱ���������г���������ͷ
                Mstr_COM_Module.Rx_NextRcvLength = Mstr_RX_BUFFERSIZE;
            }
        }
        else
        {
            //������ͷ������λ
            if(0 != HeadPos)
            {
                //��һ�ν�����ô��λ�Ϳ����������ݴ�λ
                Mstr_COM_Module.Rx_NextRcvLength = HeadPos;
                
                Mstr_COM_Module.isCorrectHead = true;
            }
            else
            {            
                //��������¾�Ӧ������ԭ������
                Mstr_COM_Module.Rx_NextRcvLength = Mstr_RX_BUFFERSIZE;
                
                //���Ĺؿ���У��ֵ
                if(pBuffer->Tail.Verification == check)				
                {
                    Hf_HaveReceived(&Mstr_HfCOM);
                    
                    //�������ݴ�����
                    if(true == GetMsgFromMstrBuffer(pBuffer))
                    {
                        //��¼���λ�õ����ݷ���ֵ
                        RxMsgTypeFeedBack = (enum MstrRxMsgTypes)pBuffer->Head.MsgTypeID;
                    }
                    else
                    {
                        //��Ǳ��ν��յ�������Ϊ��������
                        RxMsgTypeFeedBack = MstrRx_ErrorCMD;
                    }
                    
                    return true;
                }
            }
        }
    }
    
    //��Ǳ��ν��յ�������Ϊ��Ч����
    RxMsgTypeFeedBack = MstrRx_ErrorCMD;
    
    return false;
}

//��С���Է�������
static bool SendDataToMstr(uint8_t SendCMD)
{
    bool isSendThisCMD = true;
    //��ȡ���ͻ�������ַ
    Mstr_TxBufTypedef *TxBuffer = Mstr_COM_Module.pTxBuffer;

    TxBuffer->Head.DataHead1 = 0xAA;
    TxBuffer->Head.DataHead2 = 0xBB;
    TxBuffer->Head.MsgTypeID = SendCMD;
    TxBuffer->Head.IDFeedback = RxMsgTypeFeedBack;

    //���ͷУ��ֵ(���У��)
    TxBuffer->Head.Verification = TxBuffer->Head.DataHead1 
                                 * TxBuffer->Head.DataHead2
                                 * TxBuffer->Head.MsgTypeID
                                 * TxBuffer->Head.IDFeedback;

    //д����������ݲ���Ϥ���η��������Ƿ����ϴεİ��Է�û�ӵ���������
    isSendThisCMD = GetTxData(&(TxBuffer->Data), &(TxBuffer->Head.MsgTypeID));

    //��ȡУ��ֵ
    TxBuffer->Tail.Verification = GetAddCheck((uint8_t*)TxBuffer, 
                                            Mstr_COM_Module.TxBufSize - sizeof(TxBuffer->Tail));

    //���ν���Ϣ���������ȴ�״̬
    if(true == COM_SendData(&Mstr_COM_Module))
    {
        //��¼���η�������Ϣ����
        TxMsgTypeSaved = (enum Mstr_TxMsgTypes)SendCMD;
        
        return isSendThisCMD;
    }
    return false;
}

#include "ComToMaster.h"
#include "MasterDataDeal.h"

//与控制端通信的通信组件实体定义
COMInfoTypedef Mstr_COM_Module = {0};
//半双工通信机制实现组件
Hf_DuplexTypedef Mstr_HfCOM = {0};

/*  内部变量声明  */
//发送缓冲区
Mstr_TxBufTypedef Mstr_TxBuffer;
//接收缓冲区
Mstr_RxBufTypedef Mstr_RxBuffer[2];

/*  内部函数声明  */
static bool DealMstrData(void *pRxBuffer);
static bool SendDataToMstr(uint8_t SendCMD);

//初始化本接口对应的通信组件
void Mstr_COM_ModuleInit(COMInfoTypedef *pModule)
{
    //复位半双工结构体
    Hf_DuplexStructDeInit(&Mstr_HfCOM);
    
    //设定半双工请求驳回次数上限
    Mstr_HfCOM.ErrorCntMax = Mstr_REQUEST_ERROR_MAX;
    
    //绑定通信组件
    Mstr_HfCOM.pCOM = pModule;
    
    //绑定硬件接口
    pModule->UartHandle = &Mstr_COM_HUART;
    
    //标记所使用的通信载体是串口
    pModule->COM_type = SPPRTR_UART;
    
    //绑定数据处理方法
    pModule->DealData = DealMstrData;
    
    //绑定数据发送方法
    pModule->SendData = SendDataToMstr;
    
    //绑定接收缓冲区
    pModule->pRxBuffer[0] = Mstr_RxBuffer;
    pModule->pRxBuffer[1] = Mstr_RxBuffer + 1;
    pModule->RxBufSize = Mstr_RX_BUFFERSIZE;
    pModule->Rx_NextRcvLength = Mstr_RX_BUFFERSIZE;
    pModule->isCorrectHead = false;
    pModule->RxBufFlag = false;
    pModule->RxPackRcvCnt = 0;
    pModule->RxErrorPackCnt = 0;
    
    //绑定发送缓冲区
    pModule->pTxBuffer = &Mstr_TxBuffer;
    pModule->TxBufSize = sizeof(Mstr_TxBufTypedef);
    pModule->SendCnt = 0;
    
    /* 使能接收，进入中断回调函数 */
    HAL_UART_Receive_IT(&Mstr_COM_HUART, 
                        (uint8_t*)pModule->pRxBuffer[pModule->RxBufFlag], 
                        Mstr_COM_Module.RxBufSize);
}


//获取数据求和校验值
static uint32_t GetAddCheck(uint8_t *pData, uint32_t DataSize)
{
    uint32_t check = 0;
    
    //求和校验
    for(uint32_t i = 0; i < DataSize; i++)
    {
        check += pData[i];
    }
    return check;
}

//校验并获得数据头
static inline bool Check_FindHead(uint8_t *pData, uint32_t DataSize, 
                                        uint32_t TailSize, uint32_t *pHeadPos, uint32_t *pCheck)
{
    //计数器
    int32_t i;
    bool isFindDataHead = false;
    
    (*pCheck) = 0;
    
    //校验并寻找数据头
    for(i = 0, (*pHeadPos) = 0; i < DataSize; i++)	
    {
        //求和范围是除数据尾外的所有数据
        if(i < DataSize - TailSize)
        {
            (*pCheck) += pData[i];			    //求和校验
        }
        
        if(0xaa == pData[i])				//检索数据头位置
        {
            (*pHeadPos) = i;
        }
        if((0xbb == pData[i]) && ((*pHeadPos) == (i-1)))	//如果这一位是0xBB,且前一位是0xAA，
        {											//那么数据头在缓冲区的位置就可以确定
            isFindDataHead = true;
        }
    }
    
    return isFindDataHead;
}

//处理接收到控制端的数据
static bool DealMstrData(void *pRxBuffer)
{
    //获取缓冲区地址
    Mstr_RxBufTypedef *pBuffer = (Mstr_RxBufTypedef *)pRxBuffer;
    uint8_t *pData = (uint8_t *)pRxBuffer;
    
    /*  处理数据  */
    
    //数据头位置、校验值
    uint32_t HeadPos = 0;
    uint32_t check = 0;

	if(Mstr_COM_Module.RxBufSize != Mstr_RX_BUFFERSIZE)
	{	
        //此种情况是在进行补偿操作，纠正数据头位置
		Mstr_COM_Module.Rx_NextRcvLength = Mstr_RX_BUFFERSIZE;
	}
	else
	{
        //解决相位滞后一个节拍的问题
        if(Mstr_COM_Module.isCorrectHead)
        {
            //此种情况是在进行补偿操作，纠正数据头位置
            Mstr_COM_Module.Rx_NextRcvLength = Mstr_RX_BUFFERSIZE;
            
            Mstr_COM_Module.isCorrectHead = false;
            
            return false;
        }
        
        //求校验值并寻找数据头
        bool isFindDataHead = Check_FindHead(pData, Mstr_RX_BUFFERSIZE, sizeof(pBuffer->Tail), 
                                &HeadPos, &check);
        
        //处理数据头位置异常的情况（如果正常，应当在最前端）
        if(!isFindDataHead)
        {
            //防止下面的if被优化掉
            volatile uint8_t BufTail = pData[Mstr_RX_BUFFERSIZE - 1];
            
            if(0xAA == BufTail)
            {
                //如果出现了头尾错位的情形，此时数据头在数据尾处
                Mstr_COM_Module.Rx_NextRcvLength = Mstr_RX_BUFFERSIZE - 1;
            }
            else
            {
                //数据中压根就不存在数据头,就继续按原长接收，直到缓冲区中出现了数据头
                Mstr_COM_Module.Rx_NextRcvLength = Mstr_RX_BUFFERSIZE;
            }
        }
        else
        {
            //若数据头不在首位
            if(0 != HeadPos)
            {
                //下一次接收这么多位就可以消除数据错位
                Mstr_COM_Module.Rx_NextRcvLength = HeadPos;
                
                Mstr_COM_Module.isCorrectHead = true;
            }
            else
            {            
                //正常情况下就应当按照原长接收
                Mstr_COM_Module.Rx_NextRcvLength = Mstr_RX_BUFFERSIZE;
                
                //最后的关卡：校验值
                if(pBuffer->Tail.Verification == check)				
                {
                    Hf_HaveReceived(&Mstr_HfCOM);
                    
                    //调用数据处理函数
                    if(true == GetMsgFromMstrBuffer(pBuffer))
                    {
                        //记录本次获得的数据反馈值
                        RxMsgTypeFeedBack = (enum MstrRxMsgTypes)pBuffer->Head.MsgTypeID;
                    }
                    else
                    {
                        //标记本次接收到的数据为错误数据
                        RxMsgTypeFeedBack = MstrRx_ErrorCMD;
                    }
                    
                    return true;
                }
            }
        }
    }
    
    //标记本次接收到的数据为无效数据
    RxMsgTypeFeedBack = MstrRx_ErrorCMD;
    
    return false;
}

//向小电脑发送数据
static bool SendDataToMstr(uint8_t SendCMD)
{
    bool isSendThisCMD = true;
    //获取发送缓冲区地址
    Mstr_TxBufTypedef *TxBuffer = Mstr_COM_Module.pTxBuffer;

    TxBuffer->Head.DataHead1 = 0xAA;
    TxBuffer->Head.DataHead2 = 0xBB;
    TxBuffer->Head.MsgTypeID = SendCMD;
    TxBuffer->Head.IDFeedback = RxMsgTypeFeedBack;

    //获得头校验值(相乘校验)
    TxBuffer->Head.Verification = TxBuffer->Head.DataHead1 
                                 * TxBuffer->Head.DataHead2
                                 * TxBuffer->Head.MsgTypeID
                                 * TxBuffer->Head.IDFeedback;

    //写入待发送数据并获悉本次发送请求是否因上次的包对方没接到而被驳回
    isSendThisCMD = GetTxData(&(TxBuffer->Data), &(TxBuffer->Head.MsgTypeID));

    //获取校验值
    TxBuffer->Tail.Verification = GetAddCheck((uint8_t*)TxBuffer, 
                                            Mstr_COM_Module.TxBufSize - sizeof(TxBuffer->Tail));

    //本次将消息发出后进入等待状态
    if(true == COM_SendData(&Mstr_COM_Module))
    {
        //记录本次发出的信息类型
        TxMsgTypeSaved = (enum Mstr_TxMsgTypes)SendCMD;
        
        return isSendThisCMD;
    }
    return false;
}

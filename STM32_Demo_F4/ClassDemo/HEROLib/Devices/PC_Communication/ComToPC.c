#include "ComToPC.h"

//与小电脑通信的通信组件实体定义
COMInfoTypedef PC_COM_Module = {0};

//获取到的数据公示
FollowLineType  LineData = {0};
CrossDataType   CrossData = {0};
PickDataType    PickData = {0};

/*  内部变量声明  */
//发送缓冲区
static PC_TxBufTypedef PC_TxBuffer;
//接收缓冲区
//static PC_RxBufTypedef PC_RxBuffer[2];
PC_RxBufTypedef PC_RxBuffer[2];

/*  内部函数声明  */
static bool DealPCData(void *pRxBuffer);
static bool SendDataToPC(uint8_t SendCMD);
static void GetMsgFromPCBuffer(PC_RxBufTypedef *pBuffer);

//初始化本接口对应的通信组件
void PC_COM_ModuleInit(volatile COMInfoTypedef *pModule)
{
    //绑定硬件接口
    pModule->UartHandle = &PC_COM_HUART;
    
    //标记所使用的是串口
    pModule->COM_type = SPPRTR_UART;
    
    //绑定数据处理方法
    pModule->DealData = DealPCData;
    
    //绑定数据发送方法
    pModule->SendData = SendDataToPC;
    
    //绑定接收缓冲区
    pModule->pRxBuffer[0] = PC_RxBuffer;
    pModule->pRxBuffer[1] = PC_RxBuffer + 1;
    pModule->RxBufSize = PC_RX_BUFFERSIZE;
    pModule->Rx_NextRcvLength = PC_RX_BUFFERSIZE;
    pModule->isCorrectHead = false;
    pModule->RxBufFlag = false;
    pModule->RxPackRcvCnt = 0;
    pModule->RxErrorPackCnt = 0;
    
    //绑定发送缓冲区
    pModule->pTxBuffer = &PC_TxBuffer;
    pModule->TxBufSize = sizeof(PC_TxBufTypedef);
    pModule->SendCnt = 0;
    
    /* 使能接收，进入中断回调函数 */
//    HAL_UART_Receive_IT(&PC_COM_HUART, 
//                        (uint8_t*)pModule->pRxBuffer[pModule->RxBufFlag], 
//                        PC_RX_BUFFERSIZE);
}


//获取数据校验值
static uint32_t GetCheck(uint8_t *pData, uint32_t DataSize)
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

//处理接收到小电脑的数据
static bool DealPCData(void *pRxBuffer)
{
    //获取缓冲区地址
    PC_RxBufTypedef *pBuffer = (PC_RxBufTypedef *)pRxBuffer;
    uint8_t *pData = (uint8_t *)pRxBuffer;
    
    /*  处理数据  */
    
    //数据头位置、校验值
    uint32_t HeadPos = 0;
    uint32_t check = 0;

	if(PC_COM_Module.RxBufSize != PC_RX_BUFFERSIZE)
	{	
        //此种情况是在进行补偿操作，纠正数据头位置
		PC_COM_Module.Rx_NextRcvLength = PC_RX_BUFFERSIZE;
	}
	else
	{
        //解决相位滞后问题
        if(PC_COM_Module.isCorrectHead)
        {
            //此种情况是在进行补偿操作，纠正数据头位置
            PC_COM_Module.Rx_NextRcvLength = PC_RX_BUFFERSIZE;
            
            PC_COM_Module.isCorrectHead = false;
            
            return false;
        }
        
        //求校验值并寻找数据头
        volatile bool isFindDataHead = Check_FindHead(pData, PC_RX_BUFFERSIZE, sizeof(pBuffer->Tail), 
                                &HeadPos, &check);
        
        //处理数据头位置异常的情况（如果正常，应当在最前端）
        if(!isFindDataHead)
        {
            //防止下面的if被优化掉
            volatile uint8_t BufTail = pData[PC_RX_BUFFERSIZE - 1];
            
            if(0xAA == BufTail)
            {
                //如果出现了头尾错位的情形，此时数据头在数据尾处
                PC_COM_Module.Rx_NextRcvLength = PC_RX_BUFFERSIZE - 1;
            }
            else
            {
                //数据中压根就不存在数据头,就继续按原长接收，直到缓冲区中出现了数据头
                PC_COM_Module.Rx_NextRcvLength = PC_RX_BUFFERSIZE;
            }
        }
        else
        {
            //若数据头不在首位
            if(0 != HeadPos)
            {
                //下一次接收这么多位就可以消除数据错位
                PC_COM_Module.Rx_NextRcvLength = HeadPos;
                
                PC_COM_Module.isCorrectHead = true;
            }
            else
            {            
                //正常情况下就应当按照原长接收
                PC_COM_Module.Rx_NextRcvLength = PC_RX_BUFFERSIZE;
                
                //最后的关卡：校验值
                if(pBuffer->Tail.Verification == check)				
                {                
                    //调用数据处理函数
                    GetMsgFromPCBuffer(pBuffer);
                    
                    return true;
                }
            }
        }
    }
    
    return false;
}

//从通过校验的数据缓冲区中获得数据
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

//向小电脑发送数据
static bool SendDataToPC(uint8_t SendCMD)
{
    //获取发送缓冲区地址
    PC_TxBufTypedef *TxBuffer = PC_COM_Module.pTxBuffer;
    
    TxBuffer->Head.DataHead1 = 0xAA;
    TxBuffer->Head.DataHead2  =0xBB;
    TxBuffer->Head.MsgTypeID = SendCMD;
    TxBuffer->Head.Reserve = 0x00;
    
    //获取校验值
    TxBuffer->Tail.Verification = GetCheck((uint8_t*)TxBuffer, 
                                            PC_COM_Module.TxBufSize - sizeof(TxBuffer->Tail));
    
    //返回是否发送成功
    return COM_SendData(&PC_COM_Module);
}

#include "ComToBSP.h"

//与BSP通信的通信组件实体定义
COMInfoTypedef BSP_COM_Module = {0};

//半双工通信机制实现组件
Hf_DuplexTypedef BSP_HfCOM = {0};

enum BSP_RxMsgTypes BSP_RxMsgTypeFeedBack;
enum BSP_TxMsgTypes BSP_TxMsgTypeSaved;

/*  内部函数声明  */
static bool DealBSPData(void *pRxBuffer);
static bool SendDataToBSP(uint8_t SendCMD);
static bool GetMsgFromBSPBuffer(BSP_RxBufTypedef *pBuffer);

//如果使用板间串口
#if defined(USING_BSP_UART)

/*  内部变量声明  */
//发送缓冲区
static BSP_TxBufTypedef BSP_TxBuffer;
//接收缓冲区
BSP_RxBufTypedef BSP_RxBuffer[2];

//初始化CAN本接口对应的通信组件
void BSP_COM_ModuleInit(COMInfoTypedef *pModule)
{
    //复位半双工结构体
    Hf_DuplexStructDeInit(&BSP_HfCOM);
    
    //设定半双工请求驳回次数上限
    BSP_HfCOM.ErrorCntMax = BSP_REQUEST_ERROR_MAX;
    
    //绑定通信组件
    BSP_HfCOM.pCOM = pModule;
    
    //绑定硬件接口
    pModule->UartHandle = &BSP_COM_HUART;
    
    //标记所使用的通信载体是串口
    pModule->COM_type = SPPRTR_UART;
    
    //绑定数据处理方法
    pModule->DealData = DealBSPData;
    
    //绑定数据发送方法
    pModule->SendData = SendDataToBSP;
    
    //绑定接收缓冲区
    pModule->pRxBuffer[0] = BSP_RxBuffer;
    pModule->pRxBuffer[1] = BSP_RxBuffer + 1;
    pModule->RxBufSize = BSP_RX_BUFFERSIZE;
    pModule->Rx_NextRcvLength = BSP_RX_BUFFERSIZE;
    pModule->isCorrectHead = false;
    pModule->RxBufFlag = false;
    pModule->RxPackRcvCnt = 0;
    pModule->RxErrorPackCnt = 0;
    
    //绑定发送缓冲区
    pModule->pTxBuffer = &BSP_TxBuffer;
    pModule->TxBufSize = sizeof(BSP_TxBufTypedef);
    pModule->SendCnt = 0;
    
    /* 使能接收，进入中断回调函数 */
    HAL_UART_Receive_IT(&BSP_COM_HUART, 
                        (uint8_t*)pModule->pRxBuffer[pModule->RxBufFlag], 
                        BSP_COM_Module.RxBufSize);
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

//处理接收到辅控板的数据
static bool DealBSPData(void *pRxBuffer)
{
    //获取缓冲区地址
    BSP_RxBufTypedef *pBuffer = (BSP_RxBufTypedef *)pRxBuffer;
    uint8_t *pData = (uint8_t *)pRxBuffer;
    
    /*  处理数据  */
    
    //数据头位置、校验值
    uint32_t HeadPos = 0;
    uint32_t check = 0;

	if(BSP_COM_Module.RxBufSize != BSP_RX_BUFFERSIZE)
	{	
        //此种情况是在进行补偿操作，纠正数据头位置
		BSP_COM_Module.Rx_NextRcvLength = BSP_RX_BUFFERSIZE;
	}
	else
	{
        //解决相位滞后一个节拍的问题
        if(BSP_COM_Module.isCorrectHead)
        {
            //此种情况是在进行补偿操作，纠正数据头位置
            BSP_COM_Module.Rx_NextRcvLength = BSP_RX_BUFFERSIZE;
            
            BSP_COM_Module.isCorrectHead = false;
            
            return false;
        }
        
        //求校验值并寻找数据头
        bool isFindDataHead = Check_FindHead(pData, BSP_RX_BUFFERSIZE, sizeof(pBuffer->Tail), 
                                &HeadPos, &check);
        
        //处理数据头位置异常的情况（如果正常，应当在最前端）
        if(!isFindDataHead)
        {
            //防止下面的if被优化掉
            volatile uint8_t BufTail = pData[BSP_RX_BUFFERSIZE - 1];
            
            if(0xAA == BufTail)
            {
                //如果出现了头尾错位的情形，此时数据头在数据尾处
                BSP_COM_Module.Rx_NextRcvLength = BSP_RX_BUFFERSIZE - 1;
            }
            else
            {
                //数据中压根就不存在数据头,就继续按原长接收，直到缓冲区中出现了数据头
                BSP_COM_Module.Rx_NextRcvLength = BSP_RX_BUFFERSIZE;
            }
        }
        else
        {
            //若数据头不在首位
            if(0 != HeadPos)
            {
                //下一次接收这么多位就可以消除数据错位
                BSP_COM_Module.Rx_NextRcvLength = HeadPos;
                
                BSP_COM_Module.isCorrectHead = true;
            }
            else
            {            
                //正常情况下就应当按照原长接收
                BSP_COM_Module.Rx_NextRcvLength = BSP_RX_BUFFERSIZE;
                
                //最后的关卡：校验值
                if(pBuffer->Tail.Verification == check)				
                {
                    Hf_HaveReceived(&BSP_HfCOM);
                    
                    //调用数据处理函数
                    if(true == GetMsgFromBSPBuffer(pBuffer))
                    {
                        //记录本次获得的数据反馈值
                        BSP_RxMsgTypeFeedBack = (enum BSP_RxMsgTypes)pBuffer->Head.MsgTypeID;
                    }
                    else
                    {
                        //标记本次接收到的数据为错误数据
                        BSP_RxMsgTypeFeedBack = BSPRx_ErrorData;
                    }
                    
                    return true;
                }
            }
        }
    }
    
    //标记本次接收到的数据为无效数据
    BSP_RxMsgTypeFeedBack = BSPRx_ErrorData;
    
    return false;
}

//从通过校验的数据缓冲区中获得数据
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

//向BSP发送数据
static bool SendDataToBSP(uint8_t SendCMD)
{
    bool isSendThisCMD = true;    
    //获取发送缓冲区地址
    BSP_TxBufTypedef *TxBuffer = BSP_COM_Module.pTxBuffer;
    
    TxBuffer->Head.DataHead1 = 0xAA;
    TxBuffer->Head.DataHead2  =0xBB;
    TxBuffer->Head.MsgTypeID = SendCMD;
    TxBuffer->Head.FeedbackID = BSP_RxMsgTypeFeedBack;
    
    //写入待发送数据并获悉本次发送请求是否因上次的包对方没接到而被驳回
    //isSendThisCMD = GetTxData(&(TxBuffer->Data), TxBuffer->Head.MsgTypeID);
    
    TxBuffer->Data.Test.CMD = 0.123456;

    //获取校验值
    TxBuffer->Tail.Verification = GetCheck((uint8_t*)TxBuffer, 
                                            BSP_COM_Module.TxBufSize - sizeof(TxBuffer->Tail));
    
    //本次将消息发出后进入等待状态
    if(true == COM_SendData(&BSP_COM_Module))
    {
        //记录本次发出的信息类型
        BSP_TxMsgTypeSaved = (enum BSP_TxMsgTypes)SendCMD;
        
        return isSendThisCMD;
    }
    //返回是否发送成功
    return false;
}

/* IS_USING_BSP_CAN  */ //END

//如果使用板间CAN
#elif defined(USING_BSP_CAN)
/*  内部函数声明  */
static bool CAN_IsBSP_PackHead(void *pData, uint32_t DataLength);

/*  内部变量声明  */
//发送头
CAN_TxHeaderTypeDef BSP_TxHeader = {0x2ff, 0x00, CAN_ID_STD, CAN_RTR_DATA, 8, DISABLE};
//发送缓冲区
static BSP_TxBufTypedef BSP_TxBuffer;
//接收头
CAN_RxHeaderTypeDef BSP_RxHeader;
//接收缓冲区
BSP_RxBufTypedef BSP_RxBuffer;

uint8_t Rece_Date1[8] = {0};

//初始化CAN本接口对应的通信组件
void BSP_COM_ModuleInit(COMInfoTypedef *pModule)
{
    //复位半双工结构体
    Hf_DuplexStructDeInit(&BSP_HfCOM);
    
    BSP_HfCOM.ErrorCntMax = BSP_REQUEST_ERROR_MAX;
    
    //绑定通信组件
    BSP_HfCOM.pCOM = pModule;
    
    //绑定硬件接口
    pModule->CanHandle = &BSP_COM_HCAN;
    pModule->COM_type = SPPRTR_CAN;
    
    //绑定数据发送接收头结构体
    pModule->pRxHeader = &BSP_RxHeader;
    pModule->pTxHeader = &BSP_TxHeader;
    
    //绑定数据处理方法
    pModule->DealData = DealBSPData;
    
    //绑定数据发送方法
    pModule->SendData = SendDataToBSP;
    
    //绑定首帧数据处理方法
    pModule->Can_IsHeadData = CAN_IsBSP_PackHead;
    
    //绑定接收缓冲区（can为单缓冲）
    pModule->pRxBuffer[0]   = &BSP_RxBuffer;
    pModule->pRxBuffer[1]   = &BSP_RxBuffer;
    pModule->RxBufSize      = BSP_RX_BUFFERSIZE;
    pModule->IsRcvingUnhead = false;
    pModule->RxlengthCnt    = 0;
    
    pModule->RxBufFlag      = false;
    pModule->RxPackRcvCnt   = 0;
    pModule->RxErrorPackCnt = 0;
    
    //绑定发送缓冲区
    pModule->pTxBuffer = &BSP_TxBuffer;
    pModule->TxBufSize = sizeof(BSP_TxBufTypedef);
    pModule->SendCnt   = 0;
    
    /* 使能接收，进入中断回调函数 */
    CAN_Filter_Config(&BSP_COM_HCAN);
    HAL_CAN_ActivateNotification(&BSP_COM_HCAN, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY);
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

//判断一帧数据是不是包头
static bool CAN_IsBSP_PackHead(void *pData, uint32_t DataLength)
{
    uint8_t *pBuffer = pData;
    bool Result = false;
    
    //非法输入检查(一帧数据最多8个字节)
    if(DataLength < sizeof(BSP_DataHead) && DataLength < 8)
    {
        return false;
    }
    
    //头匹配
    Result = (0xAA == pBuffer[0]) && (0xBB == pBuffer[1]);
    //数据标签在值域范围内
    Result = Result && (pBuffer[2] < BSPRx_Num) && (pBuffer[3] < BSPTx_Num);
    
    return Result;
}

//处理接收到辅控板的数据
//调用此函数的时候就已经能够保证“数据长度合理”和“数据头位置在最前端”这两个条件了
static bool DealBSPData(void *pRxBuffer)
{
    //获取缓冲区地址
    BSP_RxBufTypedef *pBuffer = (BSP_RxBufTypedef *)pRxBuffer;
    uint8_t *pData = (uint8_t *)pRxBuffer;
    
    /*  处理数据  */
    
    //获取校验值
    uint32_t check = GetCheck(pData, sizeof(BSP_RxBufTypedef) - sizeof(BSP_DataTail));
    
    //最后的关卡：校验值
    if(pBuffer->Tail.Verification == check)				
    {
        Hf_HaveReceived(&BSP_HfCOM);
        
        //调用数据处理函数
        if(true == GetMsgFromBSPBuffer(pBuffer))
        {
            //记录本次获得的数据反馈值
            BSP_RxMsgTypeFeedBack = (enum BSP_RxMsgTypes)pBuffer->Head.MsgTypeID;
        }
        else
        {
            //标记本次接收到的数据为错误数据
            BSP_RxMsgTypeFeedBack = BSPRx_ErrorData;
        }
        
        return true;
    }    
    return false;
}

//从通过校验的数据缓冲区中获得数据
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

//向CAN发送数据
static bool SendDataToBSP(uint8_t SendCMD)
{
    bool isSendThisCMD = true;    
    //获取发送缓冲区地址
    BSP_TxBufTypedef *TxBuffer = BSP_COM_Module.pTxBuffer;
    
    TxBuffer->Head.DataHead1 = 0xAA;
    TxBuffer->Head.DataHead2  =0xBB;
    TxBuffer->Head.MsgTypeID = SendCMD;
    TxBuffer->Head.FeedbackID = BSP_RxMsgTypeFeedBack;
    
    //写入待发送数据并获悉本次发送请求是否因上次的包对方没接到而被驳回
    //isSendThisCMD = GetTxData(&(TxBuffer->Data), TxBuffer->Head.MsgTypeID);
    
    TxBuffer->Data.Test.CMD = 0.123456;

    //获取校验值
    TxBuffer->Tail.Verification = GetCheck((uint8_t*)TxBuffer, 
                                            BSP_COM_Module.TxBufSize - sizeof(TxBuffer->Tail));
    
    //本次将消息发出后进入等待状态
    if(true == COM_SendData(&BSP_COM_Module))
    {
        //记录本次发出的信息类型
        BSP_TxMsgTypeSaved = (enum BSP_TxMsgTypes)SendCMD;
        
        return isSendThisCMD;
    }
    //返回是否发送成功
    return false;
}

#endif /*  IS_USING_BSP_CAN  */

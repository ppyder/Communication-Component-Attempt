#include "Comunication.h"
#include "Com_CanDriver.h"
#include "Com_UserModules.h"

//对异常的描述,配合组件错误码使用
char *COM_ErrorDescriptions[COM_ErrorCodeNum] = 
{
    "未出错",                                      // @COM_NoError
    "使用前未初始化该通信组件！\n",                // @COM_Error_UnInited,
    "未在组件Map函数中找到处理该数据的组件号！\n", // @COM_Error_UnFindModule
};

/* 私有函数声明 */
void COM_ModuleErrorHandler(COM_ErrorCode ErrorCode, COMInfoTypedef *pModule);
static bool COM_SendData_Unsafe(COMInfoTypedef *pModule, void *pData, uint32_t DataSzie);

//CAN单次数据头缓存
CAN_RxHeaderTypeDef RxHeader;
//CAN单次数据接收缓存,前8个字用于存储数据，后四个字用于存储数据ID
uint8_t CANDataBuf[12] = {0};

/*******************  默认初始化函数  ***********************/
/**
 * @brief	    按串口方式初始化通信组件结构体
 * @param[out]  pModule         指向用于通信的通讯组件结构体(COMInfoTypedef*)
 * @param[in]   pHuart          由Cube生成的代表所使用的口的结构体变量(UART_HandleTypeDef*)
 * @param[in]   pDealData       由用户实现的数据接收处理函数指针(bool (*)(void*))
 * @param[in]   pSendData       由用户实现的数据发送处理函数指针(bool (*)(uint8_t))
 * @param[in]   pRcvBuffer      数据接收缓冲区地址(void *)
 * @param[in]   RcvBufferSize   数据接收缓冲区长度(uint32_t)
 * @param[in]   pSendBuffer     数据发送缓冲区地址(void *)
 * @param[in]   SendBufferSize  数据发送缓冲区长度(uint32_t)
 * @retval	    None.
 */
void COM_UART_StructInit(COMInfoTypedef *pModule, UART_HandleTypeDef *pHuart,
                            bool (*pDealData)(void*), bool (*pSendData)(uint8_t),
                            void *pRcvBuffer, uint32_t RcvBufferSize,
                            void *pSendBuffer, uint32_t SendBufferSize)
{
    //中间变量，用于接收数据指针
    uint8_t *pTemp = pRcvBuffer;
    
    //绑定硬件接口
    pModule->UartHandle = pHuart;
    
    //标记所使用的通信载体是串口
    pModule->COM_type = SPPRTR_UART;
    
    //绑定数据处理方法
    pModule->DealData = pDealData;
    
    //绑定数据发送方法
    pModule->SendData = pSendData;
    
    //绑定接收缓冲区,串口要求双缓冲区发送
    pModule->pRxBuffer[0] = pTemp;
    pModule->pRxBuffer[1] = (pTemp + RcvBufferSize);
    pModule->RxBufSize = RcvBufferSize;
    pModule->Rx_NextRcvLength = RcvBufferSize;
    pModule->isCorrectHead = false;
    pModule->RxBufFlag = false;
    pModule->RxPackRcvCnt = 0;
    pModule->RxErrorPackCnt = 0;
    
    //绑定发送缓冲区
    pModule->pTxBuffer = pSendBuffer;
    pModule->TxBufSize = SendBufferSize;
    pModule->SendCnt = 0;
    
    //标记已经初始化完成
    pModule->isInited = true;
    
    //复位状态标志
    pModule->isPrintingData = false;
    
    //复位错误标志及其描述
    pModule->ErrorCode = COM_NoError;
    pModule->ErrorDescription = COM_ErrorDescriptions[COM_NoError];
    
}

/**
 * @brief	    按CAN方式初始化通信组件结构体
 * @param[out]  pModule         指向用于通信的通讯组件结构体(COMInfoTypedef*)
 * @param[in]   pHuart          由Cube生成的代表所使用的口的结构体变量(CAN_HandleTypeDef*)
 * @param[in]   pDealData       由用户实现的数据接收处理函数指针(bool (*)(void*))
 * @param[in]   pSendData       由用户实现的数据发送处理函数指针(bool (*)(uint8_t))
 * @param[in]   pCan_IsHeadData 由用户实现的辨识某帧数据是否是数据包头的函数指针(bool (*)(void*, uint32_t))
 * @param[in]   pRxHeader       指向接收数据帧头(CAN_RxHeaderTypeDef*)
 * @param[in]   pTxHeader       指向发送数据帧头(CAN_TxHeaderTypeDef*)
 * @param[in]   pRcvBuffer      数据接收缓冲区地址(void *)
 * @param[in]   RcvBufferSize   数据接收缓冲区长度(uint32_t)
 * @param[in]   pSendBuffer     数据发送缓冲区地址(void *)
 * @param[in]   SendBufferSize  数据发送缓冲区长度(uint32_t)
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
    //绑定硬件接口
    pModule->CanHandle = pHCAN;
    pModule->COM_type = SPPRTR_CAN;
    
    //绑定数据发送接收头结构体
    pModule->pRxHeader = pRxHeader;
    pModule->pTxHeader = pTxHeader;
    
    //绑定数据处理方法
    pModule->DealData = pDealData;
    
    //绑定数据发送方法
    pModule->SendData = pSendData;
    
    //绑定首帧数据处理方法
    pModule->Can_IsHeadData = pCan_IsHeadData;
    
    //绑定接收缓冲区（can为单缓冲）
    pModule->pRxBuffer[0]   = pRcvBuffer;
    pModule->pRxBuffer[1]   = pRcvBuffer;
    pModule->RxBufSize      = RcvBufferSize;
    pModule->IsRcvingUnhead = false;
    pModule->RxlengthCnt    = 0;
    
    pModule->RxBufFlag      = false;
    pModule->RxPackRcvCnt   = 0;
    pModule->RxErrorPackCnt = 0;
    
    //绑定发送缓冲区
    pModule->pTxBuffer = pSendBuffer;
    pModule->TxBufSize = SendBufferSize;
    pModule->SendCnt   = 0;
    
    //标记已经初始化完成
    pModule->isInited = true;
    
    //复位状态标志
    pModule->isPrintingData = false;
    
    //复位错误标志及其描述
    pModule->ErrorCode = COM_NoError;
    pModule->ErrorDescription = COM_ErrorDescriptions[COM_NoError];
    
}

/*******************  END -> 默认初始化函数  ***********************/

/*******************  对外的发送接口  ***********************/
//安全地调用组件直接发送函数
void SendData_Safely(COMInfoTypedef *pModule, uint8_t SendCMD)
{
    //若本通信组件还未初始化过
    if(false == pModule->isInited)
    {
        COM_ModuleErrorHandler(COM_Error_UnInited, pModule);
    }
    else if(pModule->isPrintingData)  //如果正在打印数据
    {
        //什么也不做
    }
    else
    {
        //调用发送函数
        pModule->SendData(SendCMD);
    }
}

//CAN发送任意ID的非格式化数据
bool COM_CANSendUnformatIDData(COMInfoTypedef *pModule, CAN_TxHeaderTypeDef *pHeader, uint8_t *pData, uint16_t Size)
{
    //若本通信组件还未初始化过
    if(false == pModule->isInited)
    {
        COM_ModuleErrorHandler(COM_Error_UnInited, pModule);
    }
    else if(pModule->isPrintingData)  //如果正在打印数据
    {
        //什么也不做
    }
    else if(Can_SendData(pModule->CanHandle, pModule->pTxHeader, pData, Size))
    {
        //记录发送次数
        pModule->SendCnt++;
        
        return true;
    }
    return false;
}

//发送非格式化数据
bool COM_SendUnformatData(COMInfoTypedef *pModule, uint8_t *pData, uint32_t DataSzie)
{
    //若本通信组件还未初始化过
    if(false == pModule->isInited)
    {
        COM_ModuleErrorHandler(COM_Error_UnInited, pModule);
    }
    else if(pModule->isPrintingData)  //如果正在打印数据
    {
        //什么也不做
    }
    else if(true == COM_SendData_Unsafe(pModule, pData, DataSzie))
    {
        //记录发送次数
        pModule->SendCnt++;
        
        return true;
    }
    
    return false;
}

//在不检查是否初始化的前提下进行数据发送,返回是否发送成功
static bool COM_SendData_Unsafe(COMInfoTypedef *pModule, void *pData, uint32_t DataSzie)
{
    //调用底层库发送函数
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

//信息发送函数，把已经存在于数据缓冲区中的数据进行发送,仅允许在组件内部的数据发送函数中被调用
bool COM_SendDataInTxBuffer(COMInfoTypedef *pModule)
{        
    //若本通信组件还未初始化过
    if(false == pModule->isInited)
    {
        COM_ModuleErrorHandler(COM_Error_UnInited, pModule);
    }
    
    //调用发送函数
    if(true == COM_SendData_Unsafe(pModule, pModule->pTxBuffer, pModule->TxBufSize))
    {
        //记录发送次数
        pModule->SendCnt++;
        
        return true;
    }
        
    return false;
}

uint32_t isBlockedCnt = 0;

//重定向fputc函数以使用printf函数
int fputc(int ch, FILE* stream) 
{
    volatile bool isBlocked = false;
    uint8_t Send_ch = (uint8_t)ch;
    
    //若本通信组件还未初始化过
    if(false == COM_Modules[COM_PRINTF_ID]->isInited)
    {
        COM_ModuleErrorHandler(COM_Error_UnInited, COM_Modules[COM_PRINTF_ID]);
    }
    
    COM_Modules[COM_PRINTF_ID]->isPrintingData = true;
    
    //等待直到发送不阻塞
    while(HAL_UART_STATE_BUSY_TX == (HAL_UART_STATE_BUSY_TX & COM_Modules[COM_PRINTF_ID]->UartHandle->gState))
    {
        isBlockedCnt++;
    }
    
    //单字发送
    if(true == COM_SendData_Unsafe(COM_Modules[COM_PRINTF_ID], &Send_ch, 1))
    {
        //记录发送次数
        COM_Modules[COM_PRINTF_ID]->SendCnt++;
    }
    
    //判断有没有发送完,约定以换行符'\n'为字符串结束的标志。
    COM_Modules[COM_PRINTF_ID]->isPrintingData = ('\n' != Send_ch);
    
    return ch; 
}

/*******************  END -> 对外的发送接口  ***********************/

/*******************  接收回调函数  ***********************/

//串口接收回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    //获取本次要处理的通信组件及其相关信息
    COM_ModuleID COM_Type = UART_DealFuncMap(UartHandle);
    uint8_t BufferFlag = COM_Modules[COM_Type]->RxBufFlag;
    void *pDataBuffer = COM_Modules[COM_Type]->pRxBuffer[BufferFlag];
    uint32_t DataSize = COM_Modules[COM_Type]->Rx_NextRcvLength;
    
    if(COM_NULL == COM_Type)
    {
        //若未找到处理所接收到数据的通信组件结构体
        COM_ModuleErrorHandler(COM_Error_UnFindModule, COM_Modules[COM_Type]);
    }
    else if(false == COM_Modules[COM_Type]->isInited)
    {
        //若本通信组件还未初始化过
        COM_ModuleErrorHandler(COM_Error_UnInited, COM_Modules[COM_Type]);
    }
    
    //切换缓冲区并准备下次的接收
    HAL_UART_Receive_IT(UartHandle, COM_Modules[COM_Type]->pRxBuffer[!BufferFlag], DataSize);
    
    //调用数据处理函数并更新接收计数器
    if(true == COM_Modules[COM_Type]->DealData(pDataBuffer))
    {
        COM_Modules[COM_Type]->RxPackRcvCnt++;
    }
    else
    {
        COM_Modules[COM_Type]->RxErrorPackCnt++;
    }
    
    //更新当前缓冲区长度
    COM_Modules[COM_Type]->RxBufSize = DataSize;
    
    //翻转数据标志
    COM_Modules[COM_Type]->RxBufFlag = !COM_Modules[COM_Type]->RxBufFlag;
}

//CAN接收完成的回调函数
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle)
{
    COM_ModuleID COM_Type = COM_NULL;
    CAN_DataID DataID;
    uint32_t *pIDBuffer = (uint32_t *)(CANDataBuf + 8);
    
    //从外设处获取数据本次接收的数据帧
    HAL_CAN_GetRxMessage(CanHandle, CAN_RX_FIFO0, &RxHeader, CANDataBuf);
    
    //将ID写入缓冲区
    (*pIDBuffer) = RxHeader.StdId;
    
    //获取通信组件号
    COM_Type = CAN_DealFuncMap(CanHandle, &RxHeader, &DataID);
    
    if(COM_NULL == COM_Type)
    {
        //若未找到处理所接收到数据的通信组件结构体
        COM_ModuleErrorHandler(COM_Error_UnFindModule, COM_Modules[COM_Type]);
    }
    else if(false == COM_Modules[COM_Type]->isInited)
    {
        //若本通信组件还未初始化过
        COM_ModuleErrorHandler(COM_Error_UnInited, COM_Modules[COM_Type]);
    }
    
    //按ID分类处理数据
    pCANDataDealFuncs[DataID](CANDataBuf, RxHeader.DLC, COM_Type);
}

//通信组件异常回调函数
void COM_ModuleErrorHandler(COM_ErrorCode ErrorCode, COMInfoTypedef *pModule)
{
    //如果不是正常状态
    while(COM_NoError != ErrorCode)
    {
        //卡在这里说明被调用的这个通信组件出了问题，请在对应组件的错误描述中查看，该组件地址已传入本函数。
        pModule->ErrorCode = ErrorCode;
        pModule->ErrorDescription = COM_ErrorDescriptions[ErrorCode];
    }
}

/*******************  END -> 接收回调函数  ***********************/

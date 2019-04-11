#include "ComToBSP.h"

//与CAN通信的通信组件实体定义
COMInfoTypedef BSP_COM_Module = {0};

//半双工通信机制实现组件
Hf_DuplexTypedef BSP_HfCOM = {0};

enum BSP_RxMsgTypes BSP_RxMsgTypeFeedBack;
enum BSP_TxMsgTypes BSP_TxMsgTypeSaved;

/*  内部变量声明  */
//发送头
CAN_TxHeaderTypeDef BSP_TxHeader = {0x2ff, 0x00, CAN_ID_STD, CAN_RTR_DATA, 8, DISABLE};
//发送缓冲区
static BSP_TxBufTypedef BSP_TxBuffer;
//接收头
CAN_RxHeaderTypeDef BSP_RxHeader;
//接收缓冲区
BSP_RxBufTypedef BSP_RxBuffer;

/*  内部函数声明  */
static bool DealBSPData(void *pRxBuffer);
static bool SendDataToBSP(uint8_t SendCMD);
static bool GetMsgFromBSPBuffer(BSP_RxBufTypedef *pBuffer);
static bool CAN_IsBSP_PackHead(void *pData, uint32_t DataLength);

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
    HAL_CAN_ActivateNotification(&BSP_COM_HCAN, CAN_IT_RX_FIFO0_MSG_PENDING);
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

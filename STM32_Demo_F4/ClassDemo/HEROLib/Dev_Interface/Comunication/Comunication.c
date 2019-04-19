#include "Comunication.h"
#include "Com_CanDriver.h"
#include "ComToPC.h"
#include "ComToMaster.h"
#include "ComToBSP.h"
#include "M3508.h"

//内部声明的空组件结构体，用于标识未找到对应组件的情况
COMInfoTypedef Null_COM_Module;

//对异常的描述,配合组件错误码使用
char *COM_ErrorDescriptions[COM_ErrorCodeNum] = 
{
    "未出错",                                      // @COM_NoError
    "使用前未初始化该通信组件！\n",                // @COM_Error_UnInited,
    "未在组件Map函数中找到处理该数据的组件号！\n",   // @COM_Error_UnFindModule
};

//通信组件集合
COMInfoTypedef *COM_Modules[COM_ModulesNum] = 
{
    //空组件结构体
    &Null_COM_Module,
    
    //与小电脑的通信组件接口
    &PC_COM_Module,
    
    //与控制端的通信组件接口
    &Mstr_COM_Module,
    
    //与其他板子的通信组件接口
    &BSP_COM_Module,
};

//can数据处理函数集合
void (*pCANDataDealFuncs[CAN_IDsNum])(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type) = 
{
    RcvFormatCanData,   //格式化数据接收
    Motor1DataDeal,     //M3508的返回数据_电机1
    Motor2DataDeal,     //M3508的返回数据_电机2
    Motor3DataDeal,     //M3508的返回数据_电机3
};

/* 私有函数声明 */
void COM_ModuleErrorHandler(COM_ErrorCode ErrorCode, COMInfoTypedef *pModule);

//CAN单次数据头缓存
CAN_RxHeaderTypeDef RxHeader;
//CAN单次数据接收缓存
uint8_t CANDataBuf[8] = {0};

/*******************  Map Of Modules  ***********************/

//选择处理函数
static inline COM_ModuleID UART_DealFuncMap(UART_HandleTypeDef *UartHandle)
{
    COM_ModuleID result = COM_NULL;
    
    if(PC_COM_UART == UartHandle->Instance)
    {
        result = COM_MSTR;//COM_PC;
    }
    
//如果使用板间串口
#if defined(USING_BSP_UART)

    else if(BSP_COM_UART == UartHandle->Instance)
    {
        result = COM_BSP;
    }

#endif /*  IS_USING_BSP_CAN  */
    
    return result;
}

//选择处理函数
static inline COM_ModuleID CAN_DealFuncMap(CAN_HandleTypeDef *CanHandle)
{
    COM_ModuleID result = COM_NULL;
    
//如果使用板间CAN
#if defined(USING_BSP_CAN)

    if(BSP_COM_CAN == CanHandle->Instance)
    {
        result = COM_BSP;
    }

#endif /*  IS_USING_BSP_CAN  */
    
    return result;
}

/*******************  END -> Map Of Modules  ***********************/

/*******************  对外的发送接口  ***********************/

//CAN发送任意ID的非格式化数据
bool COM_CANSendUnformatData(COMInfoTypedef *pModule, CAN_TxHeaderTypeDef *pHeader, uint8_t *pData, uint16_t Size)
{
    //若本通信组件还未初始化过
    if(false == pModule->isInited)
    {
        COM_ModuleErrorHandler(COM_Error_UnInited, pModule);
    }
    
    if(Can_SendData(pModule->CanHandle, pModule->pTxHeader, pData, Size))
    {
        //记录发送次数
        pModule->SendCnt++;
        
        return true;
    }
    return false;
}

//以指定ID发送非格式化数据
bool COM_SendUnformatData(COMInfoTypedef *pModule, uint8_t *pData, uint32_t DataSzie)
{
    //若本通信组件还未初始化过
    if(false == pModule->isInited)
    {
        COM_ModuleErrorHandler(COM_Error_UnInited, pModule);
    }
    
    //调用底层库发送函数
    switch(pModule->COM_type)
    {
        case SPPRTR_UART:
            if(HAL_UART_Transmit_DMA(pModule->UartHandle, pData, DataSzie))
            {
                //记录发送次数
                pModule->SendCnt++;
                
                return true;
            }
            break;
        
        case SPPRTR_CAN:
            if(Can_SendData(pModule->CanHandle, pModule->pTxHeader, pData, DataSzie))
            {
                //记录发送次数
                pModule->SendCnt++;
                
                return true;
            }
            break;
        
        default:break;
    }
    
    return false;
}

//信息发送函数
bool COM_SendData(COMInfoTypedef *pModule)
{    
    volatile bool isSendOK = false;
    
    //若本通信组件还未初始化过
    if(false == pModule->isInited)
    {
        COM_ModuleErrorHandler(COM_Error_UnInited, pModule);
    }
    
    //调用底层库发送函数
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
        //记录发送次数
        pModule->SendCnt++;
        
        return true;
    }
    
    return false;
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
    //获取通信组件号
    COM_ModuleID COM_Type = CAN_DealFuncMap(CanHandle);
    CAN_DataID DataID;
    
    //从外设处获取数据本次接收的数据帧
    HAL_CAN_GetRxMessage(CanHandle, CAN_RX_FIFO0, &RxHeader, CANDataBuf);
    
    //获取接收到的信息类型
    DataID = CAN_StdIDMap(&RxHeader);
    
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

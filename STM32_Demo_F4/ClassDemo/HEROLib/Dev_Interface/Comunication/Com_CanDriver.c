#include "Com_CanDriver.h"

/****************  CAN独有的数据处理及其配置  ***********************/

//CAN 标准ID 映射（含义对应头文件中的枚举）

/**
 *  注意：自定义通信协议的ID是自己指定的。
 *      指定的时候需要注意，由于使用了按标识符发送（发送优先级），因此
 *      标识符值更小的消息具有更高的优先级。当标识符值相等时，首先安排
 *      编号更小的邮箱发送。因此，考虑到设备指令只需要单帧发送、而协议
 *      指令需要拆包多帧发送，因此往往将协议指令的ID取得比设备指令都大。
 *
 *      另外，还需要注意，使用同一个can进行发送的时候需要注意频率配合。
 *      经计算可以知道，按1Mbps的波特率计算，发送一帧数据最多需要花费掉
 *      大约0.2ms的时间，且考虑到邮箱有三个发送空位，所以推荐使用的最高
 *      发送频率是单帧数据1ms发送一次，打包数据>10ms一次。
 */

//以自定义通信协议发送的数据ID集合
const uint32_t CAN_FormatStdIDs[] = 
{
    0x2FF,           //板间通信ID
};

//以非自定义通信协议发送的数据ID集合
const uint32_t CAN_UnformatStdIDs[] = 
{
    0x200 + 0x01,   //M3508的返回数据_电机1
    0x200 + 0x02,   //M3508的返回数据_电机2
    0x200 + 0x03,   //M3508的返回数据_电机3
};

//CAN发送邮箱状态指示(是否空闲)
bool isTxMailBoxFree[3] = {true, true, true};

//CAN接收滤波器初始化函数
void CAN_Filter_Config(CAN_HandleTypeDef* hcan)
{
	CAN_FilterTypeDef  sFilterConfig;

	sFilterConfig.FilterBank = 0;                       //过滤器组0
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;   //工作在标识符屏蔽位模式
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;  //过滤器位宽为单个32位。
    
    /* 使能报文标示符过滤器按照标示符的内容进行比对过滤，扩展ID不是如下的就抛弃掉，是的话，会存入FIFO0。 */
    sFilterConfig.FilterIdHigh         = 0x0000;            //要过滤的ID高位 
    sFilterConfig.FilterIdLow          = 0x0000;            //要过滤的ID低位 
    sFilterConfig.FilterMaskIdHigh     = 0x0000;	        //过滤器高16位每位必须匹配
    sFilterConfig.FilterMaskIdLow      = 0x0000;	        //过滤器低16位每位必须匹配
    sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;  //过滤器被关联到FIFO 0
    sFilterConfig.FilterActivation = ENABLE;                //使能过滤器
    
    //考虑到一个板子最多两个can
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

/**************************  发送数据处理  ******************************/

//CAN发送单帧数据，此时Size应当是小于等于8的
bool CanSendSingleData(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *pHeader, uint8_t *pData, uint16_t Size)
{
    //等待，直到任意一个邮箱空闲（或者过了超时时间，超时时间未设定）
    while(!isTxMailBoxFree[0] && !isTxMailBoxFree[1] && !isTxMailBoxFree[1]);
    
    //标记发送长度
    pHeader->DLC = Size;
    
    //添加到不空的那个邮箱里面
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

//CAN拆包发送函数(阻塞发送)
bool Can_SendData(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *pHeader, uint8_t *pData, uint16_t Size)
{
    //如果一帧就能发完
    if(Size <= 8)
    {
        //直接发送
        CanSendSingleData(hcan, pHeader, pData, Size);
    }
    else
    {
        //计算要分几次发出去，该值一定是大于一的。
        uint32_t SendCnt = (Size / 8) + ((Size % 8 == 0) ? 0 : 1);
        //地址偏移量
        uint32_t IndexOffset = 0;
        
        //分包发送
        for( ;SendCnt > 1; SendCnt--)
        {
            CanSendSingleData(hcan, pHeader, pData + IndexOffset, 8);
            
            IndexOffset += 8;
        }
        
        //最后一包数据特殊对待，因为不一定是8个字节那么长
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

/***********************  END -> 发送数据处理  ***************************/

/**************************  接收数据处理  ******************************/

//数据拷贝函数
void DataCopy(uint8_t *pTarget, const uint8_t *pSource, uint32_t DataSize)
{
    uint32_t Cnt = 0;
    for(Cnt = 0; Cnt < DataSize; Cnt++)
    {
        pTarget[Cnt] = pSource[Cnt];
    }
    return;
}

//获得所接收的数据类型(格式化数据/非格式化的哪一类)
CAN_DataID CAN_StdIDMap(CAN_RxHeaderTypeDef *pHeader)
{
    CAN_DataID Result;
    uint32_t Cnt = 0, StdID = pHeader->StdId;
    
    //遍历查询ID
    for(Cnt = 0; Cnt < CAN_IDsNum; Cnt++)
    {
        if(CAN_FormatStdIDs[Cnt] == StdID)
        {
            Result = CAN_FormatData;
            break;
        }
        else if(CAN_UnformatStdIDs[Cnt] == StdID)
        {
            // @CAN_DataID 中 CAN_FormatData 标号为0，
            //因此非格式化数据对应的枚举值应为查询所得的数组下标加一
            Result = (CAN_DataID)(Cnt + 1);
            break;
        }
    }
    
    return Result;
}

//对一个可能是头的数据帧进行处理
void DealHeadData(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type)
{
    //判断当前所接收到的数据是不是含头的数据
    COM_Modules[COM_Type]->IsRcvingUnhead = COM_Modules[COM_Type]->Can_IsHeadData(pData, DataLength);

    if(COM_Modules[COM_Type]->IsRcvingUnhead)
    {
        //将数据写入接收缓冲区
        DataCopy((uint8_t *)COM_Modules[COM_Type]->pRxBuffer[COM_Modules[COM_Type]->RxBufFlag] 
                    + COM_Modules[COM_Type]->RxlengthCnt, pData, DataLength);
        
        COM_Modules[COM_Type]->RxlengthCnt += DataLength;
    }
}

//以某种自定义格式化通信协议接收的数据
void RcvFormatCanData(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type)
{
    //获取所使用的数据缓冲区标号
    uint8_t BufferFlag = COM_Modules[COM_Type]->RxBufFlag;
    //获取数据接收缓冲区地址
    void *pDataBuffer = COM_Modules[COM_Type]->pRxBuffer[BufferFlag];
    
    //如果正在接收的是非头数据（已经有了数据头）
    if(COM_Modules[COM_Type]->IsRcvingUnhead)
    {
        //进行非法检查（避免数组越界）
        if(COM_Modules[COM_Type]->RxlengthCnt + DataLength > COM_Modules[COM_Type]->RxBufSize)
        {
            COM_Modules[COM_Type]->RxlengthCnt = 0;
            
            //考虑可能出现这一帧数据中正好有头的情况
            DealHeadData(pData, DataLength, COM_Type);
            
            return;
        }
        
        //将数据写入接收缓冲区
        DataCopy((uint8_t *)COM_Modules[COM_Type]->pRxBuffer[COM_Modules[COM_Type]->RxBufFlag] 
                    + COM_Modules[COM_Type]->RxlengthCnt, pData, DataLength);
        
        COM_Modules[COM_Type]->RxlengthCnt += DataLength;
        
        //如果数据已经接满
        if(COM_Modules[COM_Type]->RxlengthCnt >= COM_Modules[COM_Type]->RxBufSize)
        {
            COM_Modules[COM_Type]->RxlengthCnt = 0;
            
            //调用数据处理函数并更新接收计数器
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
            //此时接收未满
            return;
        }
    }
    else
    {
        DealHeadData(pData, DataLength, COM_Type);
        return;
    }        
}

/***********************  END -> 接收数据处理  ***************************/

//CAN邮箱0发送完成回调函数
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
    isTxMailBoxFree[0] = true;
}

//CAN邮箱1发送完成回调函数
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
    isTxMailBoxFree[1] = true;
}

//CAN邮箱2发送完成回调函数
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
    isTxMailBoxFree[2] = true;
}

/*************  END -> CAN独有的数据处理及其配置  ********************/

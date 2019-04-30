#include "ComToCAN_Dev.h"
#include "Comunication.h"
#include "M3508.h"

//与DevGroup1通信的通信组件实体定义
COMInfoTypedef DevGroup1_COM_Module = {0};
//与DevGroup2通信的通信组件实体定义
COMInfoTypedef DevGroup2_COM_Module = {0};

//设备组1_ID集合
uint32_t CAN_DevGroup1StdIDs[] = 
{
    0x200 + 0x01,   //M3508的返回数据_电机1
    0x200 + 0x02,   //M3508的返回数据_电机2
    0x200 + 0x03,   //M3508的返回数据_电机3
};

uint32_t DevGroup1StdIDNum = sizeof(CAN_DevGroup1StdIDs) / sizeof(uint32_t);

//设备组2_ID集合
uint32_t CAN_DevGroup2StdIDs[] = 
{
    0x00,   //没有，此处用作占位
};

uint32_t DevGroup2StdIDNum = sizeof(CAN_DevGroup2StdIDs) / sizeof(uint32_t);

//设备数据处理函数集合
void (*pDev_DealFuncs[CAN_DevDataNum])(uint8_t *pData) = 
{
    Motor1DataDeal,     //M3508的返回数据_电机1
    Motor2DataDeal,     //M3508的返回数据_电机2
    Motor3DataDeal,     //M3508的返回数据_电机3
};

/*  内部函数声明  */
static bool DealDevGroup1Data(void *pRxBuffer);
static bool SendDataToDevGroup1(uint8_t SendCMD);

static bool DealDevGroup2Data(void *pRxBuffer);
static bool SendDataToDevGroup2(uint8_t SendCMD);

/*  内部变量声明  */
//发送头
CAN_TxHeaderTypeDef DevGroup1_TxHeader;
CAN_TxHeaderTypeDef DevGroup2_TxHeader;
//发送缓冲区
static Dev_TxBufTypedef DevGroup1_TxBuffer;
static Dev_TxBufTypedef DevGroup2_TxBuffer;
//接收头
CAN_RxHeaderTypeDef DevGroup1_RxHeader;
CAN_RxHeaderTypeDef DevGroup2_RxHeader;
//接收缓冲区
Dev_RxBufTypedef DevGroup1_RxBuffer;
Dev_RxBufTypedef DevGroup2_RxBuffer;

/************  初始化函数  ***********/

//初始化CAN接口对应的通信组件
void DevGroup1_COM_ModuleInit(COMInfoTypedef *pModule)
{        
    //以CAN方式初始化通信组件
    COM_CAN_StructInit(pModule, &DevGroup1_COM_HCAN,
                        DealDevGroup1Data, SendDataToDevGroup1,
                        NULL,
                        &DevGroup1_RxHeader,
                        &DevGroup1_TxHeader,
                        &DevGroup1_RxBuffer, Dev_RX_BUFFERSIZE,
                        &DevGroup1_TxBuffer, sizeof(Dev_TxBufTypedef));
    
    /* 使能接收，进入中断回调函数 */
    CAN_Filter_Config(&DevGroup1_COM_HCAN);
    HAL_CAN_ActivateNotification(&DevGroup1_COM_HCAN, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY);
}

//初始化CAN接口对应的通信组件
void DevGroup2_COM_ModuleInit(COMInfoTypedef *pModule)
{        
    //以CAN方式初始化通信组件
    COM_CAN_StructInit(pModule, &DevGroup2_COM_HCAN,
                        DealDevGroup2Data, SendDataToDevGroup2,
                        NULL,
                        &DevGroup2_RxHeader,
                        &DevGroup2_TxHeader,
                        &DevGroup2_RxBuffer, Dev_RX_BUFFERSIZE,
                        &DevGroup2_TxBuffer, sizeof(Dev_TxBufTypedef));
    
    /* 使能接收，进入中断回调函数 */
    CAN_Filter_Config(&DevGroup2_COM_HCAN);
    HAL_CAN_ActivateNotification(&DevGroup2_COM_HCAN, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY);
}

/************  数据处理函数映射  ***********/

//从ID获得处理函数的下标
CAN_DevDataID GetDevGroup1DealID(uint32_t ID)
{
    CAN_DevDataID Result = CAN_M3508Data1;
    uint32_t Cnt = 0, StdID = ID;
    
    //遍历查询ID
    for(Cnt = 0; Cnt < CAN_IDsNum; Cnt++)
    {
        if(CAN_DevGroup1StdIDs[Cnt] == StdID)
        {
            Result = (CAN_DevDataID)Cnt;
            break;
        }
    }
    
    return Result;
}

//从ID获得处理函数的下标
CAN_DevDataID GetDevGroup2DealID(uint32_t ID)
{
    CAN_DevDataID Result = CAN_M3508Data1;
    uint32_t Cnt = 0, StdID = ID;
    
    //遍历查询ID
    for(Cnt = 0; Cnt < CAN_IDsNum; Cnt++)
    {
        if(CAN_DevGroup2StdIDs[Cnt] == StdID)
        {
            Result = (CAN_DevDataID)(Cnt + CAN_DevGroup1Num);
            break;
        }
    }
    
    return Result;
}

/************  对接收到的数据进行处理  ***********/

//处理接收到设备的数据
static bool DealDevGroup1Data(void *pRxBuffer)
{
    //获取缓冲区地址
    Dev_RxBufTypedef *pBuffer = (Dev_RxBufTypedef *)pRxBuffer;
    //获取数据处理函数的ID
    CAN_DevDataID DataID = GetDevGroup1DealID(pBuffer->DevID);
    
    //根据ID调用对应的数据处理函数
    pDev_DealFuncs[DataID](pRxBuffer);
        
    return true;
}

//处理接收到设备的数据
static bool DealDevGroup2Data(void *pRxBuffer)
{
    //获取缓冲区地址
    Dev_RxBufTypedef *pBuffer = (Dev_RxBufTypedef *)pRxBuffer;
    //获取数据处理函数的ID
    CAN_DevDataID DataID = GetDevGroup2DealID(pBuffer->DevID);
    
    //根据ID调用对应的数据处理函数
    pDev_DealFuncs[DataID](pRxBuffer);
        
    return true;
}

/************  数据发送函数  ***********/

//向属于设备组1的设备发送数据，默认发送前已经把数据赋好值了。
//这里的传入参数无效，仅起一个标识作用
static bool SendDataToDevGroup1(uint8_t SendCMD)
{
    //返回是否发送成功
    return COM_SendDataInTxBuffer(&DevGroup1_COM_Module);
}

//向属于设备组2的设备发送数据，默认发送前已经把数据赋好值了。
//这里的传入参数无效，仅起一个标识作用
static bool SendDataToDevGroup2(uint8_t SendCMD)
{
    //返回是否发送成功
    return COM_SendDataInTxBuffer(&DevGroup2_COM_Module);
}


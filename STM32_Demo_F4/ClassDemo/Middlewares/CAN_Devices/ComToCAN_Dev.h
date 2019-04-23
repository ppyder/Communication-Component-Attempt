#ifndef COMTOCAN_DEV_H
#define COMTOCAN_DEV_H

#include "Dev_Data.h"
#include "Com_Functions.h"

//共用体定义：接收数据缓冲区
typedef struct
{
    //数据内容
    union
    {
        uint8_t Datas[8];
        M3508RxType M3508;
    }Data;
    
    //数据标签ID
    CAN_DataID DevID;
    
}Dev_RxBufTypedef;

//共用体定义：发送数据缓冲区
typedef union
{
    M3508TxType M3508;
    
}Dev_TxBufTypedef;

//针对can的数据类型映射，包含格式化数据和非格式化数据
typedef enum 
{
    /*  设备组1  */
    //M3508返回数据1
    CAN_M3508Data1 = 0,
    
    //M3508返回数据2
    CAN_M3508Data2,
    
    //M3508返回数据3
    CAN_M3508Data3,
        
    //设备组1的设备数量
    CAN_DevGroup1Num,
    
    /*  设备组2  */
    
    
    CAN_DevDataNum
    
}CAN_DevDataID;

#define DevGroup1_COM_CAN  CAN1
#define DevGroup1_COM_HCAN (hcan1)

#define DevGroup2_COM_CAN  CAN1
#define DevGroup2_COM_HCAN (hcan1)

#define Dev_RX_BUFFERSIZE sizeof(Dev_RxBufTypedef)
#define Dev_REQUEST_ERROR_MAX 0

//通信组件定义
extern COMInfoTypedef DevGroup1_COM_Module;
extern COMInfoTypedef DevGroup2_COM_Module;

//设备组ID集合
extern uint32_t CAN_DevGroup1StdIDs[];
extern uint32_t CAN_DevGroup2StdIDs[];
extern uint32_t DevGroup1StdIDNum;
extern uint32_t DevGroup2StdIDNum;

void DevGroup1_COM_ModuleInit(COMInfoTypedef *pModule);
void DevGroup2_COM_ModuleInit(COMInfoTypedef *pModule);

#endif

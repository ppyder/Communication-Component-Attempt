#ifndef COMTOCAN_DEV_H
#define COMTOCAN_DEV_H

#include "Dev_Data.h"
#include "Com_Functions.h"

//�����嶨�壺�������ݻ�����
typedef struct
{
    //��������
    union
    {
        uint8_t Datas[8];
        M3508RxType M3508;
    }Data;
    
    //���ݱ�ǩID
    CAN_DataID DevID;
    
}Dev_RxBufTypedef;

//�����嶨�壺�������ݻ�����
typedef union
{
    M3508TxType M3508;
    
}Dev_TxBufTypedef;

//���can����������ӳ�䣬������ʽ�����ݺͷǸ�ʽ������
typedef enum 
{
    /*  �豸��1  */
    //M3508��������1
    CAN_M3508Data1 = 0,
    
    //M3508��������2
    CAN_M3508Data2,
    
    //M3508��������3
    CAN_M3508Data3,
        
    //�豸��1���豸����
    CAN_DevGroup1Num,
    
    /*  �豸��2  */
    
    
    CAN_DevDataNum
    
}CAN_DevDataID;

#define DevGroup1_COM_CAN  CAN1
#define DevGroup1_COM_HCAN (hcan1)

#define DevGroup2_COM_CAN  CAN1
#define DevGroup2_COM_HCAN (hcan1)

#define Dev_RX_BUFFERSIZE sizeof(Dev_RxBufTypedef)
#define Dev_REQUEST_ERROR_MAX 0

//ͨ���������
extern COMInfoTypedef DevGroup1_COM_Module;
extern COMInfoTypedef DevGroup2_COM_Module;

//�豸��ID����
extern uint32_t CAN_DevGroup1StdIDs[];
extern uint32_t CAN_DevGroup2StdIDs[];
extern uint32_t DevGroup1StdIDNum;
extern uint32_t DevGroup2StdIDNum;

void DevGroup1_COM_ModuleInit(COMInfoTypedef *pModule);
void DevGroup2_COM_ModuleInit(COMInfoTypedef *pModule);

#endif

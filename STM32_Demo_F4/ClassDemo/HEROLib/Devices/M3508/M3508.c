#include "M3508.h"

short id = 0x200;

short Real_C_Value[3] = {0};        //电机真实电流值
short Real_V_Value[3] = {0};        //电机真实速度值
short Real_A_Value[3] = {0};         //电机真实角度值

//向电调发送电流值
void M3508SetCurrent(uint16_t id, int16_t Current1, int16_t Current2, int16_t Current3, int16_t Current4)
{
    //获取发送缓冲区地址
	CAN_TxHeaderTypeDef *pTx_Header = DevGroup1_COM_Module.pTxHeader;
    M3508TxType *pTxData = DevGroup1_COM_Module.pTxBuffer;
    
    //赋值
	pTx_Header->StdId = id;          //帧ID为传入参数的CAN_ID
	pTx_Header->IDE = CAN_ID_STD;    //标准帧
	pTx_Header->RTR = CAN_RTR_DATA;  //数据帧
	pTx_Header->DLC = 0x08;          //帧长度为8
	 
    pTxData->Current1 = Current1;
    pTxData->Current2 = Current2;
    pTxData->Current3 = Current3;
    pTxData->Current4 = Current4;

    //发送数据
    SendData_Safely(&DevGroup1_COM_Module, DevGroup1Tx_M3508);
}

//接收数据处理函数
void Motor1DataDeal(uint8_t *pBuffer)
{
    M3508RxType *pData = (M3508RxType *)pBuffer;
    Real_A_Value[0] = pData->Angle;  //角度值
    Real_V_Value[0] = pData->Speed;  //速度值
    Real_C_Value[0] = pData->Current;  //电流值
}

//接收数据处理函数
void Motor2DataDeal(uint8_t *pBuffer)
{
    M3508RxType *pData = (M3508RxType *)pBuffer;
    
    Real_A_Value[1] = pData->Angle;  //角度值
    Real_V_Value[1] = pData->Speed;  //速度值
    Real_C_Value[1] = pData->Current;  //电流值
}

//接收数据处理函数
void Motor3DataDeal(uint8_t *pBuffer)
{
    M3508RxType *pData = (M3508RxType *)pBuffer;
    
    Real_A_Value[2] = pData->Angle;  //角度值
    Real_V_Value[2] = pData->Speed;  //速度值
    Real_C_Value[2] = pData->Current;  //电流值
}

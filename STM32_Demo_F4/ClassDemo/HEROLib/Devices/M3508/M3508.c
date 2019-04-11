#include "M3508.h"

short id = 0x200;

/*  内部变量声明  */
uint8_t   TxData[8]; 
uint8_t   RxData[8];

short Real_C_Value[3] = {0};        //电机真实电流值
short Real_V_Value[3] = {0};        //电机真实速度值
long Real_A_Value[3] = {0};         //电机真实角度值

//向电调发送电流值
void M3508SetCurrent(uint16_t id, int16_t Current1, int16_t Current2, int16_t Current3, int16_t Current4)
{
	CAN_TxHeaderTypeDef tx_message;
		
	tx_message.StdId = id;          //帧ID为传入参数的CAN_ID
	tx_message.IDE = CAN_ID_STD;    //标准帧
	tx_message.RTR = CAN_RTR_DATA;  //数据帧
	tx_message.DLC = 0x08;          //帧长度为8
	 
	TxData[0] = Current1 >> 8;
	TxData[1] = Current1;
	TxData[2] = Current2 >> 8;
	TxData[3] = Current2;
	TxData[4] = Current3 >> 8;
	TxData[5] = Current3;
	TxData[6] = Current4 >> 8;
	TxData[7] = Current4;

    COM_CANSendUnformatData(&BSP_COM_Module, &tx_message, TxData, 8);
}

//接收数据处理函数
void Motor1DataDeal(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type)
{
    Real_A_Value[0] = (RxData[0]<<8)|(RxData[1]);  //角度值
    Real_V_Value[0] = (RxData[2]<<8)|(RxData[3]);  //速度值
    Real_C_Value[0] = (RxData[4]<<8)|(RxData[5]);  //电流值
}

//接收数据处理函数
void Motor2DataDeal(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type)
{
    Real_A_Value[1] = (RxData[0]<<8)|(RxData[1]);  //角度值
    Real_V_Value[1] = (RxData[2]<<8)|(RxData[3]);  //速度值
    Real_C_Value[1] = (RxData[4]<<8)|(RxData[5]);  //电流值
}

//接收数据处理函数
void Motor3DataDeal(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type)
{
    Real_A_Value[2] = (RxData[0]<<8)|(RxData[1]);  //角度值
    Real_V_Value[2] = (RxData[2]<<8)|(RxData[3]);  //速度值
    Real_C_Value[2] = (RxData[4]<<8)|(RxData[5]);  //电流值
}

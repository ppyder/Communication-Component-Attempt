#include "M3508.h"

short id = 0x200;

/*  �ڲ���������  */
uint8_t   TxData[8]; 
uint8_t   RxData[8];

short Real_C_Value[3] = {0};        //�����ʵ����ֵ
short Real_V_Value[3] = {0};        //�����ʵ�ٶ�ֵ
long Real_A_Value[3] = {0};         //�����ʵ�Ƕ�ֵ

//�������͵���ֵ
void M3508SetCurrent(uint16_t id, int16_t Current1, int16_t Current2, int16_t Current3, int16_t Current4)
{
	CAN_TxHeaderTypeDef tx_message;
		
	tx_message.StdId = id;          //֡IDΪ���������CAN_ID
	tx_message.IDE = CAN_ID_STD;    //��׼֡
	tx_message.RTR = CAN_RTR_DATA;  //����֡
	tx_message.DLC = 0x08;          //֡����Ϊ8
	 
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

//�������ݴ�����
void Motor1DataDeal(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type)
{
    Real_A_Value[0] = (RxData[0]<<8)|(RxData[1]);  //�Ƕ�ֵ
    Real_V_Value[0] = (RxData[2]<<8)|(RxData[3]);  //�ٶ�ֵ
    Real_C_Value[0] = (RxData[4]<<8)|(RxData[5]);  //����ֵ
}

//�������ݴ�����
void Motor2DataDeal(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type)
{
    Real_A_Value[1] = (RxData[0]<<8)|(RxData[1]);  //�Ƕ�ֵ
    Real_V_Value[1] = (RxData[2]<<8)|(RxData[3]);  //�ٶ�ֵ
    Real_C_Value[1] = (RxData[4]<<8)|(RxData[5]);  //����ֵ
}

//�������ݴ�����
void Motor3DataDeal(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type)
{
    Real_A_Value[2] = (RxData[0]<<8)|(RxData[1]);  //�Ƕ�ֵ
    Real_V_Value[2] = (RxData[2]<<8)|(RxData[3]);  //�ٶ�ֵ
    Real_C_Value[2] = (RxData[4]<<8)|(RxData[5]);  //����ֵ
}

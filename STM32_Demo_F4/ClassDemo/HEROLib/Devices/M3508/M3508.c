#include "M3508.h"

short id = 0x200;

short Real_C_Value[3] = {0};        //�����ʵ����ֵ
short Real_V_Value[3] = {0};        //�����ʵ�ٶ�ֵ
short Real_A_Value[3] = {0};         //�����ʵ�Ƕ�ֵ

//�������͵���ֵ
void M3508SetCurrent(uint16_t id, int16_t Current1, int16_t Current2, int16_t Current3, int16_t Current4)
{
    //��ȡ���ͻ�������ַ
	CAN_TxHeaderTypeDef *pTx_Header = DevGroup1_COM_Module.pTxHeader;
    M3508TxType *pTxData = DevGroup1_COM_Module.pTxBuffer;
    
    //��ֵ
	pTx_Header->StdId = id;          //֡IDΪ���������CAN_ID
	pTx_Header->IDE = CAN_ID_STD;    //��׼֡
	pTx_Header->RTR = CAN_RTR_DATA;  //����֡
	pTx_Header->DLC = 0x08;          //֡����Ϊ8
	 
    pTxData->Current1 = Current1;
    pTxData->Current2 = Current2;
    pTxData->Current3 = Current3;
    pTxData->Current4 = Current4;

    //��������
    SendData_Safely(&DevGroup1_COM_Module, DevGroup1Tx_M3508);
}

//�������ݴ�����
void Motor1DataDeal(uint8_t *pBuffer)
{
    M3508RxType *pData = (M3508RxType *)pBuffer;
    Real_A_Value[0] = pData->Angle;  //�Ƕ�ֵ
    Real_V_Value[0] = pData->Speed;  //�ٶ�ֵ
    Real_C_Value[0] = pData->Current;  //����ֵ
}

//�������ݴ�����
void Motor2DataDeal(uint8_t *pBuffer)
{
    M3508RxType *pData = (M3508RxType *)pBuffer;
    
    Real_A_Value[1] = pData->Angle;  //�Ƕ�ֵ
    Real_V_Value[1] = pData->Speed;  //�ٶ�ֵ
    Real_C_Value[1] = pData->Current;  //����ֵ
}

//�������ݴ�����
void Motor3DataDeal(uint8_t *pBuffer)
{
    M3508RxType *pData = (M3508RxType *)pBuffer;
    
    Real_A_Value[2] = pData->Angle;  //�Ƕ�ֵ
    Real_V_Value[2] = pData->Speed;  //�ٶ�ֵ
    Real_C_Value[2] = pData->Current;  //����ֵ
}

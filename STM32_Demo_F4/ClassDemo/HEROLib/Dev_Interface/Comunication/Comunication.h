#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "ComStructDef.h"
#include "usart.h"
#include "can.h"

/*  ͨ������ӿڣ�ʹ��˫���������գ������������ͣ�֧�ֶ������ݽ��գ�����/���������ݷ���  */

//���ʵ���б���Ӧ��˳��ָ�����鸳ֵ�����ڹ���
typedef enum 
{
    //С����ͨ�����
    COM_PC = 0x00,
    
    //���ƶ�ͨ�����
    COM_MSTR,
    
    //���ͨ�����
    COM_BSP,
    
    COM_ModulesNum
    
}COM_ModuleID;

//��ָ�������Ϊ����ͨ�������ָ��ռλ
#define NULL_MESSAGE 0

//���ڼ��й����ͨ���������
extern COMInfoTypedef *COM_Modules[COM_ModulesNum];

//CAN�����˲�����ʼ������
void CAN_Filter_Config(CAN_HandleTypeDef* hcan);

//���ݷ��ͺ���
bool COM_SendData(COMInfoTypedef *pModule);
bool COM_SendUnformatData(COMInfoTypedef *pModule, uint8_t *pData, uint32_t DataSzie);
bool COM_CANSendUnformatData(COMInfoTypedef *pModule, CAN_TxHeaderTypeDef *pHeader, uint8_t *pData, uint16_t Size);

#endif

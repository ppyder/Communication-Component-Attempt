#ifndef COM_USERMODULES_H
#define COM_USERMODULES_H

#include "ComStructDef.h"

//���ʵ���б���Ӧ��˳��ָ�����鸳ֵ�����ڹ���
typedef enum 
{
    //��ͨ������ṹ�壬���ڱ�ʶ��Map������δ�ҵ���������
    COM_NULL = 0x00,
    
    //С����ͨ�����
    COM_PC,
    
    //���ƶ�ͨ�����
    COM_MSTR,
    
    //���ͨ�����
    COM_BSP,
    
    COM_ModulesNum
    
}COM_ModuleID;

//���can����������ӳ�䣬������ʽ�����ݺͷǸ�ʽ������
typedef enum 
{
    //�Զ���ͨЭ������֡
    CAN_FormatData,
    
    //M3508��������1
    CAN_M3508Data1,
    
    //M3508��������2
    CAN_M3508Data2,
    
    //M3508��������3
    CAN_M3508Data3,
    
    CAN_IDsNum
    
}CAN_DataID;

//���ڼ��й����ͨ���������
extern COMInfoTypedef *COM_Modules[COM_ModulesNum];

//����CAN��Ϣ�ķ�������
extern void (*pCANDataDealFuncs[CAN_IDsNum])(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type);

//���Զ���ͨ��Э�鷢�͵�����ID����
extern const uint32_t CAN_FormatStdIDs[];
//�Է��Զ���ͨ��Э�鷢�͵�����ID����
extern const uint32_t CAN_UnformatStdIDs[];

//�û����ӳ��
COM_ModuleID UART_DealFuncMap(UART_HandleTypeDef *UartHandle);
COM_ModuleID CAN_DealFuncMap(CAN_HandleTypeDef *CanHandle);

CAN_DataID CAN_StdIDMap(CAN_RxHeaderTypeDef *pHeader);

#endif

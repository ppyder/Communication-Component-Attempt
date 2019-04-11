#ifndef COMTOPC_H
#define COMTOPC_H

#include "PC_Data.h"
#include "Comunication.h"

//�ṹ�嶨�壺�������ݻ�����
typedef struct
{
    //����ͷ
    PCData_DataHead Head;
    
    //���ݿռ�
    union
    {
        FollowLineType  Line;   //ѭ��
        CrossDataType   Cross;  //ʮ��
        PickDataType    Pick;   //ʰȡ
    }Data;
    
    //����β
    PCData_DataTail Tail;
    
}PC_RxBufTypedef; 

//�ṹ�嶨�壺�������ݻ�����
typedef struct
{
    //����ͷ
    PCData_DataHead Head;
    
    //����β
    PCData_DataTail Tail;
    
}PC_TxBufTypedef; 

#define PC_COM_UART USART2
#define PC_COM_HUART (huart2)
#define PC_RX_BUFFERSIZE sizeof(PC_RxBufTypedef)

//��С���Ե�ͨ����������ӿڣ�ʹ��֮ǰһ��Ҫ��ʼ����������
extern COMInfoTypedef PC_COM_Module;

//��ȡ�������ݹ�ʾ
extern FollowLineType  LineData;
extern CrossDataType   CrossData;
extern PickDataType    PickData;

//��ʼ��PCͨ�������ģ���ʼ������
void PC_COM_ModuleInit(volatile COMInfoTypedef *pModule);

#endif

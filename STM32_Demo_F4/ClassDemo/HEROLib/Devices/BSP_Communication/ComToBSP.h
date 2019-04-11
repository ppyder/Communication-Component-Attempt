#ifndef COMTOBSP_H
#define COMTOBSP_H

#include "BSP_Data.h"
#include "Comunication.h"
#include "Half_Duplex.h"

//�ṹ�嶨�壺�������ݻ�����
typedef struct
{
    //����ͷ
    BSP_DataHead Head;
    
    //���ݿռ�
    union
    {
          TestDataType  Test;
    }Data;
    
    //����β
    BSP_DataTail Tail;
    
}BSP_RxBufTypedef; 

//�ṹ�嶨�壺�������ݻ�����
typedef struct
{
    //����ͷ
    BSP_DataHead Head;
    
    //���ݿռ�
    union
    {
          TestCMDType  Test;
    }Data;
    
    //����β
    BSP_DataTail Tail;
    
}BSP_TxBufTypedef; 

#define BSP_COM_CAN  CAN1
#define BSP_COM_HCAN (hcan1)
#define BSP_RX_BUFFERSIZE sizeof(BSP_RxBufTypedef)
    
#define BSP_REQUEST_ERROR_MAX 0

extern Hf_DuplexTypedef BSP_HfCOM ;

//��CAN��ͨ����������ӿڣ�ʹ��֮ǰһ��Ҫ��ʼ����������
extern COMInfoTypedef BSP_COM_Module;

//��ʼ��CANͨ�������ģ���ʼ������
void BSP_COM_ModuleInit(COMInfoTypedef *pModule);

#endif




#ifndef COMTOBSP_H
#define COMTOBSP_H

#include "BSP_Data.h"
#include "Com_Functions.h"
#include "Half_Duplex.h"

#define USING_BSP_UART
//#define USING_BSP_CAN

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

#define BSP_RX_BUFFERSIZE sizeof(BSP_RxBufTypedef)
#define BSP_REQUEST_ERROR_MAX 0

//���ʹ�ð�䴮��
#if defined(USING_BSP_UART)

#define BSP_COM_UART  USART2    //�����Ƿ���������
#define BSP_COM_HUART (huart2)

//���ʹ�ð��CAN
#elif defined(USING_BSP_CAN)

#define BSP_COM_CAN  CAN1
#define BSP_COM_HCAN (hcan1)

#endif /*  IS_USING_BSP_CAN  */

extern Hf_DuplexTypedef BSP_HfCOM ;

//�븨�ص�ͨ����������ӿڣ�ʹ��֮ǰһ��Ҫ��ʼ����������
extern COMInfoTypedef BSP_COM_Module;

//��ʼ��BSPͨ�������ģ���ʼ������
void BSP_COM_ModuleInit(COMInfoTypedef *pModule);

#endif




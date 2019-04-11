#ifndef COMTOMASTER_H
#define COMTOMASTER_H

#include "Master_Data.h"
#include "Comunication.h"
#include "Half_Duplex.h"

//�ṹ�嶨�壺�������ݻ�����
typedef struct
{
    //����ͷ
    MstrData_DataHead Head;
    
    //���ݿռ�
    union MstrRxUnionType
    {
        SampleCMDTypedef        Sample;         //����
        
        BaseMotionCMDTypedef    BaseMotionCMD;  //�����˶�����ָ��
        
        BasePlanCtrlCMDTypedef  PlanTestCMD;    //�滮�˶�����ָ��
        
        MR1CtrlCMDTypedef       AutoCtrlCMD;    //��/ȫ�Զ����̿���ָ��
        
        ActionDebugCMDTypedef   ActionDebugCMD; //�ϲ㶯��debugָ��
        
        PIDDataSetTypedef       PIDData;        //PID����ָ��
        
    }Data;
    
    //����β
    MstrData_DataTail Tail;
    
}Mstr_RxBufTypedef; 

//�ṹ�嶨�壺�������ݻ�����
typedef struct
{
    //����ͷ
    MstrData_DataHead Head;
    
    //���ݿռ�
    union MstrTxUnionType
    {
        SampleDataTypedef       Sample;             //�������ݷ���
        
        BaseMotionDataTypedef   BaseMotionData;     //�����˶���صķ�������
        
        BasePlanDataTypedef     PlanTestData;       //�滮�˶����ݷ���
        
        MR1CtrlDataTypedef      AutoCtrlData;       //ȫ/���Զ��������ݷ���
        
        ActionDebugDataTypedef  ActionDebugData;    //�ϲ㶯��debug���ݷ���
        
        PIDDataGetTypedef       PIDData;            //PID����
        
    }Data;
    
    //����β
    MstrData_DataTail Tail;
    
}Mstr_TxBufTypedef; 

#define Mstr_COM_UART USART2
#define Mstr_COM_HUART (huart2)
#define Mstr_RX_BUFFERSIZE sizeof(Mstr_RxBufTypedef)
    
#define Mstr_REQUEST_ERROR_MAX 0

extern COMInfoTypedef Mstr_COM_Module;
extern Hf_DuplexTypedef Mstr_HfCOM;

//��ʼ��PCͨ�������ģ���ʼ������
void Mstr_COM_ModuleInit(COMInfoTypedef *pModule);

#endif

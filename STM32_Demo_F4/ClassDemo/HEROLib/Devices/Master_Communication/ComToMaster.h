#ifndef COMTOMASTER_H
#define COMTOMASTER_H

#include "Master_Data.h"
#include "Comunication.h"
#include "Half_Duplex.h"

//结构体定义：接收数据缓冲区
typedef struct
{
    //数据头
    MstrData_DataHead Head;
    
    //数据空间
    union MstrRxUnionType
    {
        SampleCMDTypedef        Sample;         //采样
        
        BaseMotionCMDTypedef    BaseMotionCMD;  //基本运动控制指令
        
        BasePlanCtrlCMDTypedef  PlanTestCMD;    //规划运动测试指令
        
        MR1CtrlCMDTypedef       AutoCtrlCMD;    //半/全自动流程控制指令
        
        ActionDebugCMDTypedef   ActionDebugCMD; //上层动作debug指令
        
        PIDDataSetTypedef       PIDData;        //PID设置指令
        
    }Data;
    
    //数据尾
    MstrData_DataTail Tail;
    
}Mstr_RxBufTypedef; 

//结构体定义：发送数据缓冲区
typedef struct
{
    //数据头
    MstrData_DataHead Head;
    
    //数据空间
    union MstrTxUnionType
    {
        SampleDataTypedef       Sample;             //采样数据反馈
        
        BaseMotionDataTypedef   BaseMotionData;     //基本运动相关的反馈数据
        
        BasePlanDataTypedef     PlanTestData;       //规划运动数据反馈
        
        MR1CtrlDataTypedef      AutoCtrlData;       //全/半自动流程数据反馈
        
        ActionDebugDataTypedef  ActionDebugData;    //上层动作debug数据反馈
        
        PIDDataGetTypedef       PIDData;            //PID数据
        
    }Data;
    
    //数据尾
    MstrData_DataTail Tail;
    
}Mstr_TxBufTypedef; 

#define Mstr_COM_UART USART2
#define Mstr_COM_HUART (huart2)
#define Mstr_RX_BUFFERSIZE sizeof(Mstr_RxBufTypedef)
    
#define Mstr_REQUEST_ERROR_MAX 0

extern COMInfoTypedef Mstr_COM_Module;
extern Hf_DuplexTypedef Mstr_HfCOM;

//初始化PC通信组件的模块初始化函数
void Mstr_COM_ModuleInit(COMInfoTypedef *pModule);

#endif

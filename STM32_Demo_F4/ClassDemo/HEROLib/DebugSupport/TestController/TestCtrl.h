#ifndef TESTCTRL_H
#define TESTCTRL_H

#include <stdint.h>
#include <stdbool.h>
#include "SignalGenerator.h"

//!!!˵����Ĭ��10ms����һ����غ���

typedef struct
{
    //����ʱ������ֵ
    float AimValue;
    
    //������ʱ��ȱʡ����ֵ
    float DefaultAim;
    
    //δ�˲�ʱ�Ĳ���ֵ
    float Feedback;
    
    //�����˲���Ľ��
    float Feedback_Smooth;
    
    //�����Ŀ��������ֵ
    float OutputValue;
    
    //����ʹ�ܱ�־
    bool isSampleEnabled;
    
    //�Ƿ�ʹ�ÿ�����
    //��������ʱ�ֶ������趨ֵ����п������ԣ�
    bool isUsingController;
    
    //ָ������ļ����ź�����
    enum SignalFunctionType SignalType;
    
    //������ʱ��
    uint8_t ms_10;
    uint16_t second;
    
}TestCtrlStructDef;

//�飺��Ҫ����ʵ��
void FeedbackSampleData(uint16_t Time_ms, int16_t Input, int16_t Output, int16_t OutputUc);

extern TestCtrlStructDef TestCtrlParam;

//�������ⲿ��������
void StartSample(void);
void StopSample(void);
void DuringSampling(void);

#endif

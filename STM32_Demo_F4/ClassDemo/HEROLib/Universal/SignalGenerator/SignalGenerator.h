#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H

#include <stdint.h>

//�ź����ɵĲ������ڣ�����õ���һ���ź����ɺ�����ms��
#define SAMPLE_SIGNAL_T 10

enum SignalFunctionType
{
    F_Zero = 0,     //�����
    F_Step,         //��Ծ�ź�
    F_Sin,          //�����ź�
    FunctionCnt
};

//�źź�������
extern double (*SignalFunctions[])(uint32_t time);

#endif

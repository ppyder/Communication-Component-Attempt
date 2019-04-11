#include "SignalGenerator.h"
#include <math.h>

#define Pi 3.1415926536

/*       �������еĺ�������ʱ��(ms)�ĺ����������ɳ���ָ��       */

//˽�к�������
static double ZeroOutput(uint32_t time);
static double StepFunction(uint32_t time);
static double SinFunction(uint32_t time);

//�����źź����ļ���
double (*SignalFunctions[])(uint32_t time) = 
{
    ZeroOutput,
    StepFunction,
    SinFunction,
};

//���������ķ�ֵ���������ⲿ����ʵ�־�����
int16_t Amplitude = 2000;

//������ź�
static double ZeroOutput(uint32_t time)
{
    return 0;
}

//�����������ӳ�ʱ�䣨��λΪms��
uint16_t DelayTime = 0;

//��Ծ����
static double StepFunction(uint32_t time)
{
    double result = 0;
    
    //ת��ʱ��ĵ�λ��ms
    time *= SAMPLE_SIGNAL_T;
    
    //�ӳټ���
    if(time >= DelayTime)
    {
        result = Amplitude * 1;
    }
    else
    {
        result = 0;
    }
    return result;
}

//�����źŵ�Ƶ�ʣ�Hz������ʼ��λ�������ƣ���ƫ�ã�ƫ������
double frequency = 1;
double Theta = 0;
double Offset = 0;

//���Һ���
static double SinFunction(uint32_t time)
{
    double result = 0;
    
    //ת��ʱ��ĵ�λ��ms
    time *= SAMPLE_SIGNAL_T;
    
    result = Amplitude * sin(1e-3 * 2 * Pi * frequency * time + Theta) + Offset;
    
    return result;
}



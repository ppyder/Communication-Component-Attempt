#include "Controller.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//������������ֵ����Ӧ10A����
#define OUTPUT_MAX (16384 * 0.8)

bool isUsingF_Channel = false;

//����޷�
inline double LimitOutput(double Output)
{
    double result = 0;
    
    if(fabs(Output) > OUTPUT_MAX)
    {
        result = Output > 0 ?  OUTPUT_MAX : (-OUTPUT_MAX);
    }
    else
    {
        result = Output;
    }
    
    return result;
}

//���崫�����ӷ�ĸϵ������(�����ҽ״��ɸߵ���)
double num_F[] = {249.6639, -745.8028, 743.7976, -247.6572};
double den_F[] = {1.0000, -0.9874, -0.9951, 0.9923};

//���崫�����ӽ���(��ע�⣬���������鳤��С1)
#define Z_FuncNumOrder (sizeof(num_F) / sizeof(double) - 1)
//���崫����ĸ����(��ע�⣬���������鳤��С1)
#define Z_FuncDenOrder (sizeof(den_F) / sizeof(double) - 1) 

//ǰ��ͨ������ʵ��
double Link_F(double Input)
{
    //��¼��ʷ�α����ڵ���������
    static double LastInput[Z_FuncNumOrder] = {0};
    static double LastOutput[Z_FuncDenOrder] = {0};
    double result = 0;
    uint8_t cnt = 0;
    
    //ģʽʶ��õ���ģ����
    result = num_F[0] * Input;
    
    for(cnt = 1; cnt <= Z_FuncNumOrder; cnt++)
    {
        //��(K-cnt)����ʷ�����Ӱ��
        result += (num_F[cnt] * LastInput[cnt - 1]);
    }
    
    for(cnt = 1; cnt <= Z_FuncDenOrder; cnt++)
    {
        //��(K-cnt)����ʷ�����Ӱ��
        result -= (den_F[cnt] * LastOutput[cnt - 1]);
    }
    
    result *= (1 / den_F[0]);

    //��¼��ʷֵ
    for(cnt = Z_FuncNumOrder;cnt > 0; cnt--)
    {
        LastInput[cnt] = LastInput[cnt - 1];
    }
    LastInput[0] = Input;
    
    for(cnt = Z_FuncDenOrder;cnt > 0; cnt--)
    {
        LastOutput[cnt] = LastOutput[cnt - 1];
    }
    LastOutput[0] = result;
        
    return result;
}

double Kp = 10;
double Ki = 0.3;
double Kd = 0;

//������
double I_deviation = 0;

//������ֵ
double Threshold_I = 200;

//ǰ��ͨ��PID�ջ�����������ʵ�֣�����ƫ��
double Link_G0(double deviation)
{
    static double LastDeviation = 0;
    double result = 0;
    double Kl = 0;
    
    //���ַ���
    if(fabs(deviation) < Threshold_I)
    {
        I_deviation += deviation;
        Kl = 1;
    }
    else  
    {
        Kl = 0;
    }
    
    result = Kp * deviation + Kl * Ki * I_deviation
            + Kd * (deviation - LastDeviation) * 1000 / (double)CALCULATE_T;
    
    LastDeviation = deviation;
    
    return result;
}

//����������ʵ�֣�ʵ�ָ���ͨ�����ź�����
double Controller(double Input, double Feedback)
{
    double result = 0;
    double deviation = Input - Feedback;  //ƫ��
    
    //�����������Ϊ��ͨ���źŵ���
    //�������ǰ��ͨ��
    if(isUsingF_Channel)
    {
        result = Link_F(Input) + Link_G0(deviation);
    }
    else
    {
        result = Link_G0(deviation);
    }
    
    result = LimitOutput(result);

    return result;
}

 


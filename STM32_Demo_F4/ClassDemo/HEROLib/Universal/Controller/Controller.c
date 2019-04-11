#include "Controller.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//限制输出的最大值，对应10A电流
#define OUTPUT_MAX (16384 * 0.8)

bool isUsingF_Channel = false;

//输出限幅
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

//脉冲传函分子分母系数向量(从左到右阶次由高到低)
double num_F[] = {249.6639, -745.8028, 743.7976, -247.6572};
double den_F[] = {1.0000, -0.9874, -0.9951, 0.9923};

//脉冲传函分子阶数(请注意，阶数比数组长度小1)
#define Z_FuncNumOrder (sizeof(num_F) / sizeof(double) - 1)
//脉冲传函分母阶数(请注意，阶数比数组长度小1)
#define Z_FuncDenOrder (sizeof(den_F) / sizeof(double) - 1) 

//前馈通道函数实现
double Link_F(double Input)
{
    //记录历史次本环节的输入和输出
    static double LastInput[Z_FuncNumOrder] = {0};
    static double LastOutput[Z_FuncDenOrder] = {0};
    double result = 0;
    uint8_t cnt = 0;
    
    //模式识别得到的模型逆
    result = num_F[0] * Input;
    
    for(cnt = 1; cnt <= Z_FuncNumOrder; cnt++)
    {
        //第(K-cnt)次历史输入的影响
        result += (num_F[cnt] * LastInput[cnt - 1]);
    }
    
    for(cnt = 1; cnt <= Z_FuncDenOrder; cnt++)
    {
        //第(K-cnt)次历史输出的影响
        result -= (den_F[cnt] * LastOutput[cnt - 1]);
    }
    
    result *= (1 / den_F[0]);

    //记录历史值
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

//积分器
double I_deviation = 0;

//积分阈值
double Threshold_I = 200;

//前向通道PID闭环控制器函数实现，输入偏差
double Link_G0(double deviation)
{
    static double LastDeviation = 0;
    double result = 0;
    double Kl = 0;
    
    //积分分离
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

//控制器总体实现（实现各个通道的信号流）
double Controller(double Input, double Feedback)
{
    double result = 0;
    double deviation = Input - Feedback;  //偏差
    
    //控制器输出即为各通道信号叠加
    //如果采用前馈通道
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

 


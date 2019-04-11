#include "SignalGenerator.h"
#include <math.h>

#define Pi 3.1415926536

/*       这里所有的函数都是时间(ms)的函数，参量由常量指定       */

//私有函数声明
static double ZeroOutput(uint32_t time);
static double StepFunction(uint32_t time);
static double SinFunction(uint32_t time);

//输入信号函数的集合
double (*SignalFunctions[])(uint32_t time) = 
{
    ZeroOutput,
    StepFunction,
    SinFunction,
};

//给定激励的幅值（量纲由外部函数实现决定）
int16_t Amplitude = 2000;

//零输出信号
static double ZeroOutput(uint32_t time)
{
    return 0;
}

//给定激励的延迟时间（单位为ms）
uint16_t DelayTime = 0;

//阶跃函数
static double StepFunction(uint32_t time)
{
    double result = 0;
    
    //转换时间的单位到ms
    time *= SAMPLE_SIGNAL_T;
    
    //延迟激励
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

//正弦信号的频率（Hz）、初始相位（弧度制）、偏置（偏移量）
double frequency = 1;
double Theta = 0;
double Offset = 0;

//正弦函数
static double SinFunction(uint32_t time)
{
    double result = 0;
    
    //转换时间的单位到ms
    time *= SAMPLE_SIGNAL_T;
    
    result = Amplitude * sin(1e-3 * 2 * Pi * frequency * time + Theta) + Offset;
    
    return result;
}



#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H

#include <stdint.h>

//信号生成的采样周期（即多久调用一次信号生成函数，ms）
#define SAMPLE_SIGNAL_T 10

enum SignalFunctionType
{
    F_Zero = 0,     //零输出
    F_Step,         //阶跃信号
    F_Sin,          //正弦信号
    FunctionCnt
};

//信号函数集合
extern double (*SignalFunctions[])(uint32_t time);

#endif

#ifndef TESTCTRL_H
#define TESTCTRL_H

#include <stdint.h>
#include <stdbool.h>
#include "SignalGenerator.h"

//!!!说明：默认10ms调用一次相关函数

typedef struct
{
    //采样时的输入值
    float AimValue;
    
    //不采样时的缺省输入值
    float DefaultAim;
    
    //未滤波时的采样值
    float Feedback;
    
    //采样滤波后的结果
    float Feedback_Smooth;
    
    //计算后的控制器输出值
    float OutputValue;
    
    //采样使能标志
    bool isSampleEnabled;
    
    //是否使用控制器
    //（方便有时手动输入设定值或进行开环测试）
    bool isUsingController;
    
    //指定输入的激励信号类型
    enum SignalFunctionType SignalType;
    
    //采样计时器
    uint8_t ms_10;
    uint16_t second;
    
}TestCtrlStructDef;

//虚：需要另外实现
void FeedbackSampleData(uint16_t Time_ms, int16_t Input, int16_t Output, int16_t OutputUc);

extern TestCtrlStructDef TestCtrlParam;

//正常的外部声明函数
void StartSample(void);
void StopSample(void);
void DuringSampling(void);

#endif

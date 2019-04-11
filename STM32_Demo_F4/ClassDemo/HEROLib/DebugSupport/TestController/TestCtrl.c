#include "filter.h"
#include "TestCtrl.h"
#include "Controller.h"

//测试参量
TestCtrlStructDef TestCtrlParam = 
{
    0, 0, 0, 0, 0, false, true, F_Step, 0, 0,
};

//开始采样和收集数据
void StartSample(void)
{
    //清零计时器
    TestCtrlParam.ms_10 = 0;
    TestCtrlParam.second = 0;
    
    TestCtrlParam.isSampleEnabled = true;
    
    return;
}

//结束采样和收集数据
void StopSample(void)
{
    TestCtrlParam.isSampleEnabled = false;
    
    TestCtrlParam.AimValue = TestCtrlParam.DefaultAim;
    TestCtrlParam.OutputValue = 0;
    
    return;
}

//采样器件的数据处理和行为,10ms被调用一次
void DuringSampling(void)
{
    uint16_t TempTime = 0;
    double TempUc_out = 0;
    
    //如果开始采样
    if(TestCtrlParam.isSampleEnabled)
    {
        TestCtrlParam.ms_10++;
        
        //待发送的时间单位是10ms
        TempTime = TestCtrlParam.second * 100 + TestCtrlParam.ms_10;

        //计算此次应该给出的目标值
        TestCtrlParam.AimValue = SignalFunctions[TestCtrlParam.SignalType](TempTime);
        
        if(TestCtrlParam.ms_10 > 99)
        {
            TestCtrlParam.ms_10 = 0;
            TestCtrlParam.second++;
        }
    }
    
    //计算滤波后的反馈值
    TestCtrlParam.Feedback_Smooth = DataFillter(TestCtrlParam.Feedback, &Fill_Params[Fill_1000Hz]);
    
    if(TestCtrlParam.isUsingController)
    {
        TempUc_out = Controller(TestCtrlParam.AimValue, TestCtrlParam.Feedback_Smooth);
    }
    else
    {
        TempUc_out = TestCtrlParam.AimValue;
    }

    TestCtrlParam.OutputValue = TempUc_out;
                
    FeedbackSampleData(TempTime, (int16_t)TestCtrlParam.AimValue, (int16_t)TestCtrlParam.Feedback_Smooth, 0);
    
    return;
}

//弱定义的数据反馈函数
__weak void FeedbackSampleData(uint16_t Time_ms, int16_t Input, int16_t Output, int16_t OutputUc)
{
    return;
}

#include "filter.h"
#include "TestCtrl.h"
#include "Controller.h"

//���Բ���
TestCtrlStructDef TestCtrlParam = 
{
    0, 0, 0, 0, 0, false, true, F_Step, 0, 0,
};

//��ʼ�������ռ�����
void StartSample(void)
{
    //�����ʱ��
    TestCtrlParam.ms_10 = 0;
    TestCtrlParam.second = 0;
    
    TestCtrlParam.isSampleEnabled = true;
    
    return;
}

//�����������ռ�����
void StopSample(void)
{
    TestCtrlParam.isSampleEnabled = false;
    
    TestCtrlParam.AimValue = TestCtrlParam.DefaultAim;
    TestCtrlParam.OutputValue = 0;
    
    return;
}

//�������������ݴ������Ϊ,10ms������һ��
void DuringSampling(void)
{
    uint16_t TempTime = 0;
    double TempUc_out = 0;
    
    //�����ʼ����
    if(TestCtrlParam.isSampleEnabled)
    {
        TestCtrlParam.ms_10++;
        
        //�����͵�ʱ�䵥λ��10ms
        TempTime = TestCtrlParam.second * 100 + TestCtrlParam.ms_10;

        //����˴�Ӧ�ø�����Ŀ��ֵ
        TestCtrlParam.AimValue = SignalFunctions[TestCtrlParam.SignalType](TempTime);
        
        if(TestCtrlParam.ms_10 > 99)
        {
            TestCtrlParam.ms_10 = 0;
            TestCtrlParam.second++;
        }
    }
    
    //�����˲���ķ���ֵ
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

//����������ݷ�������
__weak void FeedbackSampleData(uint16_t Time_ms, int16_t Input, int16_t Output, int16_t OutputUc)
{
    return;
}

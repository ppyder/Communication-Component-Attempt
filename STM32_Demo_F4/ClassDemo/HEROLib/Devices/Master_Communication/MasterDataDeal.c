#include "MasterDataDeal.h"
#include "TestCtrl.h"

//��ȡ�������ݹ�ʾ
SampleCMDTypedef SampleCMD;
//ʵ�ʲ���ʱ���µ�ֵ
SampleDataTypedef SampelData;
BaseMotionDataTypedef BaseMotionData;
BasePlanDataTypedef    BasePlanData;
MR1CtrlDataTypedef    Mr1CtrlData;
ActionDebugDataTypedef  ActionebugData;
PIDDataGetTypedef       PIDDataFeedback;

//���յ������ݷ���ֵ����ʼ��Ϊ��ָ��
enum MstrRxMsgTypes RxMsgTypeFeedBack = MstrRx_EmptyCMD;
//�ϴη��͵����ݼ�¼,��ʼ��Ϊ��ָ��
enum Mstr_TxMsgTypes TxMsgTypeSaved = MstrTx_EmptyData;

//��ͨ��У������ݻ������л������
bool GetMsgFromMstrBuffer(Mstr_RxBufTypedef *pBuffer)
{
    bool IsValidID = true;
     switch(pBuffer->Head.MsgTypeID)
    {
        case MstrRx_ErrorCMD: 
            break;
        
        case MstrRx_EmptyCMD: 
            break;
        
        case MstrRx_SampleCMD: 
            SampleCMD = pBuffer->Data.Sample;
            //��ʼ����
            if(1 == SampleCMD.CMD)
            {
                StartSample();
            }
            //��������
            else if(0 == SampleCMD.CMD)
            {
                StopSample();
            }
            break;
            
        case MstrRx_BaseMotionCMD: 
            break;
        
        case MstrRx_PlanMotionTestCMD: 
            break;
        
        case MstrRx_AutoProcessCMD: 
            break;
        
        case MstrRx_UpperActionDebugCMD: 
            break;
        
        case MstrRx_PIDSetCMD: 
            //PIDCMD = pBuffer->Data.PIDData;
            break;
                
        default: IsValidID = false;
                break;
    }

    return IsValidID;
}

//��ȡ����������
bool GetTxData(union MstrTxUnionType *pUnion, uint8_t *pDataType)
{
    switch(*pDataType)
    {
        case MstrTx_ErrorData: 
                            break;
        
        case MstrTx_EmptyData: 
                            break;
        
        case MstrTx_SampleData: pUnion->Sample = SampelData;
                            break;
        
        case MstrTx_BaseMotionData: pUnion->BaseMotionData = BaseMotionData;
                            break;
        
        case MstrTx_PlanMotionTestData: pUnion->PlanTestData = BasePlanData;
                            break;
        
        case MstrTx_AutoProcessData: pUnion->AutoCtrlData = Mr1CtrlData;
                            break;
        
        case MstrTx_UpperActionDebugData: pUnion->ActionDebugData = ActionebugData;
                            break;
        
        case MstrTx_PIDData: pUnion->PIDData = PIDDataFeedback;
                            break;
                
        default: break;
    }
    
    return true;
}

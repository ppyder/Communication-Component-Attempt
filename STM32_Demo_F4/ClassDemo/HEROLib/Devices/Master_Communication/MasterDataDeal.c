#include "MasterDataDeal.h"
#include "TestCtrl.h"

//获取到的数据公示
SampleCMDTypedef SampleCMD;
//实际采样时更新的值
SampleDataTypedef SampelData;
BaseMotionDataTypedef BaseMotionData;
BasePlanDataTypedef    BasePlanData;
MR1CtrlDataTypedef    Mr1CtrlData;
ActionDebugDataTypedef  ActionebugData;
PIDDataGetTypedef       PIDDataFeedback;

//接收到的数据反馈值，初始化为空指令
enum MstrRxMsgTypes RxMsgTypeFeedBack = MstrRx_EmptyCMD;
//上次发送的数据记录,初始化为空指令
enum Mstr_TxMsgTypes TxMsgTypeSaved = MstrTx_EmptyData;

//从通过校验的数据缓冲区中获得数据
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
            //开始采样
            if(1 == SampleCMD.CMD)
            {
                StartSample();
            }
            //结束采样
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

//获取待发送数据
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

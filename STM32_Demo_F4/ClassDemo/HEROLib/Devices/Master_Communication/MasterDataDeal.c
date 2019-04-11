#include "MasterDataDeal.h"
#include "TestCtrl.h"

//获取到的数据公示
SampleCMDTypedef SampleCMD = {0};
//实际采样时更新的值
SampleDataTypedef SampelData = {0};

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
        case MstrRx_SampleCMD: 
            SampleCMD = pBuffer->Data.Sample;
            //如果要开始记录了
            if(1 == SampleCMD.CMD)
            {
                StartSample();
            }
            //如果停止记录了
            else if(0 == SampleCMD.CMD)
            {
                StopSample();
            }
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
        case MstrRx_SampleCMD: pUnion->Sample = SampelData;
                            break;
                
        default: break;
    }
    
    return true;
}

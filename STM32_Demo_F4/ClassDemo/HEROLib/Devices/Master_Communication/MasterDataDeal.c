#include "MasterDataDeal.h"
#include "TestCtrl.h"

//��ȡ�������ݹ�ʾ
SampleCMDTypedef SampleCMD = {0};
//ʵ�ʲ���ʱ���µ�ֵ
SampleDataTypedef SampelData = {0};

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
        case MstrRx_SampleCMD: 
            SampleCMD = pBuffer->Data.Sample;
            //���Ҫ��ʼ��¼��
            if(1 == SampleCMD.CMD)
            {
                StartSample();
            }
            //���ֹͣ��¼��
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

//��ȡ����������
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

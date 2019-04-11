#ifndef MASTERDATADEAL_H
#define MASTERDATADEAL_H

#include "ComToMaster.h"

extern SampleCMDTypedef SampleCMD;
extern SampleDataTypedef SampelData;

extern enum MstrRxMsgTypes RxMsgTypeFeedBack;
extern enum Mstr_TxMsgTypes TxMsgTypeSaved;

bool GetTxData(union MstrTxUnionType *pUnion, uint8_t *pDataType);
bool GetMsgFromMstrBuffer(Mstr_RxBufTypedef *pBuffer);

#endif

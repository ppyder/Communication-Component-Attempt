#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "ComStructDef.h"
#include "usart.h"
#include "can.h"

//关于该组件的说明，见头文件 @"Com_Functions.h"

/*  通信组件接口，使用双缓冲区接收，单缓冲区发送，支持定长数据接收，定长/不定长数据发送  */

//错误描述集合
extern char *COM_ErrorDescriptions[COM_ErrorCodeNum];

//空指令，用于作为各种通信组件的指令占位
#define NULL_MESSAGE 0

//数据发送函数（直接发送已经存在于缓冲区中的数据）
bool COM_SendData(COMInfoTypedef *pModule);

#endif

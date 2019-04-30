/**
  ******************************************************************************
  * @file           : Com_Functions.h
  * @brief          : 声明了通信组件的操作函数。
  ******************************************************************************
  ** 说明：
  *     通信组件结构体中存在一些可调用的函数指针，但当且仅当使用本文件中的函数调
  * 用的时候才可以保证其使用安全性。
  *     当然，在确保安全的情况下，你也完全可以直接调用其中的函数。
  *
  *     另外，通信组件中重定向了fputc函数，这意味着你可以通过某个组件对字符串基于
  * 通信接口进行信息打印，在宏 @COM_PRINTF_ID 处指定用于打印的通信组件号即可。并且
  * 约定，每个字符串必须以换行符 '\n'作为结束标志，因为组件还具备发送格式化数据的
  * 功能。由于fputc是一个字符一个字符发出去的，为了防止单个的打印字符与格式化信息
  * 混在一起发送，因此做了发送互斥标志，而识别字符串结束的标志就是遇到'\n'字符。
  *
  * <h2><center>&copy; Copyright (C) HITwh Excellent Robot Organization(HERO). 2015-2018.</center></h2>
  *
  * + 当需要用到通信组件时，需要进行以下操作：
  *
  *   1. 在对应模块的头文件中包含本头文件("Com_Functions.h")。
  *   2. 在对应模块的源文件中包含头文件:"Comunication.h"。
  *   3. 在对应模块的源文件中定义在该模块中起作用的通信组件结构体，并在头文件中声明它。
  *   4. 在对应模块的源文件中按照初始化函数中需要的信息进行函数和缓冲区准备。
  *   5. 在对应模块的源文件中定义模块初始化函数，并在头文件中声明它。
  *   5. 在头文件"Com_UserModules.h"中的COM_ID枚举中添加新组件的ID枚举，
  *      并在源文件"Com_UserModules.c"中的COM_Modules结构体指针数组中按
  *      照对应枚举的顺序添加新的组件地址。
  *   6. 在源文件"Com_UserModules.c"中的模块映射函数中添加对应的组件以及ID映射。
  *   7. 如果使用了CAN，还需要在CAN相关的处理函数集合中添加对应的函数地址。
  *
  * + 当需要调用已经声明好的通信组件时，需进行以下操作：
  *
  *   1.包含头文件"Com_Functions.h"。
  *   2.包含被调用模块的头文件。
  *   3.使用本文件中声明的函数对组件结构体进行操作。
  *
  * + 本文件中包含了以下函数：
  *
  *   1. CAN滤波器初始化函数；
  *   2. 按两种通信载体初始化通信组件的初始化函数。
  *   3. 数据发送函数：包括任意数据发送、格式化数据发送（按数据ID区分数据包）、
  *      基于任意can_ID的数据发送。
  *
  ******************************************************************************
  */

#ifndef COM_FUNCTIONS_H
#define COM_FUNCTIONS_H

#include "ComStructDef.h"
#include "Com_UserModules.h"

//CAN接收滤波器初始化函数
void CAN_Filter_Config(CAN_HandleTypeDef* hcan);

//结构体初始化函数
void COM_UART_StructInit(COMInfoTypedef *pModule, UART_HandleTypeDef *pHuart,
                            bool (*pDealData)(void*), bool (*pSendData)(uint8_t),
                            void *pRcvBuffer, uint32_t RcvBufferSize,
                            void *pSendBuffer, uint32_t SendBufferSize);
                            
void COM_CAN_StructInit(COMInfoTypedef *pModule, CAN_HandleTypeDef *pHCAN,
                            bool (*pDealData)(void*), bool (*pSendData)(uint8_t),
                            bool (*pCan_IsHeadData)(void*, uint32_t),
                            CAN_RxHeaderTypeDef *pRxHeader,
                            CAN_TxHeaderTypeDef *pTxHeader,
                            void *pRcvBuffer, uint32_t RcvBufferSize,
                            void *pSendBuffer, uint32_t SendBufferSize);

//数据发送函数
bool COM_SendUnformatData(COMInfoTypedef *pModule, uint8_t *pData, uint32_t DataSzie);
bool COM_CANSendUnformatIDData(COMInfoTypedef *pModule, CAN_TxHeaderTypeDef *pHeader, uint8_t *pData, uint16_t Size);
void SendData_Safely(COMInfoTypedef *pModule, uint8_t SendCMD);

#endif

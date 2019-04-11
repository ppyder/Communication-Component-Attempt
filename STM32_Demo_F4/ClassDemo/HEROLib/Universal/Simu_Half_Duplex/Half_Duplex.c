/**
 ******************************************************************************
 * @file    Half_Duplex.c
 * @brief   实现了软件层面约束的模拟半双工通信机制.
 *              + 半双工标志系统
 *              + 半双工操作方法
 *              + 数据发送队列(延后实现)
 ******************************************************************************
 *
 * Copyright (C) HITwh Excellent Robot Organization(HERO). 2015-2018. All rights reserved.
 *
 * @version 1.0 示例版本
 * @author  杨亦凡 17863107058(手机)   942041771(qq)
 * @date    2019/03/21
 *
 ******************************************************************************
 */

#include "Half_Duplex.h"

//私有函数声明
void BlockErrorHandler(void);

//用于缺省初始化的空通信组件结构体
static COMInfoTypedef NULL_COM = {0};

//赋予半双工通讯结构体缺省值
void Hf_DuplexStructDeInit(Hf_DuplexTypedef *pModule)
{
    //标记通信未堵塞
    pModule->isBlocked = false;
    
    //使能第一次发送
    pModule->isGetMsg = true;
        
    pModule->RqstErrorCnt = 0;
    pModule->ErrorCntMax = 0;
    
    pModule->pCOM = &NULL_COM;
    pModule->pBlockError = BlockErrorHandler;
}

//接收函数(需在接收完成后调用，用于复位接收标志)
void Hf_HaveReceived(Hf_DuplexTypedef *pModule)
{
    pModule->isGetMsg = true;
    pModule->isBlocked  = false;
}

uint32_t testCnt = 0;

//发送函数
void Hf_SendData(Hf_DuplexTypedef *pModule, uint8_t CMD)
{
    //如果已经接收到数据，便意味着可以发送数据了
    if(pModule->isGetMsg)
    {
        //复位标志
        pModule->isGetMsg = false;
        
        //清零异常计数器
        pModule->RqstErrorCnt = 0;
        
        //按照指令发送消息
        pModule->pCOM->SendData(CMD);
    }
    else
    {
        //如果发送请求因等待接收而被驳回，便记录下来
        pModule->RqstErrorCnt++;
        
        if(testCnt < pModule->RqstErrorCnt)
        {
            testCnt = pModule->RqstErrorCnt;
        }
        
        //驳回次数过多则调用错误处理函数
        if(pModule->ErrorCntMax > 0 && pModule->RqstErrorCnt > pModule->ErrorCntMax)
        {
            pModule->isBlocked = true;
            
            pModule->pBlockError();
        }
    }
}

//断线重连函数（复位标志以使能发送）
void Hf_Reconnect(Hf_DuplexTypedef *pModule)
{
    pModule->isGetMsg = true;
    
    //发送对应组件的0号指令，约定0号为空指令
    Hf_SendData(pModule, 0);
}

//弱定义的通信阻塞处理函数
__weak void BlockErrorHandler(void)
{
    while(1)
    {
        
    }
}


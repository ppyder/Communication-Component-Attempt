#ifndef DEV_DATA_H
#define DEV_DATA_H

//此文件中定义CAN设备要求的数据帧通信协议内容，数据结构长度不能超过八个字节。

#include <stdint.h>
#include <stdbool.h>

/***************  以下定义设备数据缓冲区  ********************/

//枚举定义：设备组1发送数据类型ID
enum DevGroup1_TxMsgTypes
{
    //循线模式
    DevGroup1Tx_M3508 = 0,
    
    //发送ID的数值上限（辅助合理性校验和数组定义等）
    DevGroup1Tx_Num
};

//枚举定义：设备组1接收数据类型ID
enum DevGroup1_RxMsgTypes
{
    //循线模式
    DevGroup1Rx_M3508 = 0,
        
    //接收ID的数值上限（辅助合理性校验和数组定义等）
    DevGroup1Rx_Num
};

//结构体定义：M3508电机的电流发送数据结构体
typedef struct
{
    int16_t Current1;
    int16_t Current2;
    int16_t Current3;
    int16_t Current4;
    
}M3508TxType; 


//结构体定义：M3508电机的电流接收数据结构体
typedef struct
{
    int16_t Angle;      //转子机械角位置
    int16_t Speed;      //转子转速（RPM）
    int16_t Current;    //母线电流
    int16_t Temperature;//电机温度
    
}M3508RxType;  

#endif

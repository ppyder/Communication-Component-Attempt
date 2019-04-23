#ifndef COM_USERMODULES_H
#define COM_USERMODULES_H

#include "ComStructDef.h"

//组件实体列表，对应于顺序指针数组赋值，便于管理
typedef enum 
{
    //空通信组件结构体，用于标识在Map函数中未找到组件的情况
    COM_NULL = 0x00,
    
    /*  使用串口的通信组件  */
    //小电脑通信组件
    COM_PC,
    
    //控制端通信组件
    COM_MSTR,
    
    //使用串口的通信组件数量
    COM_UART_Num,
    
    /*  使用CAN的通信组件，顺序按先非格式化后格式化  */
    //CAN设备组1
    COM_DevGroup1 = COM_UART_Num,
    
    //CAN设备组2
    COM_DevGroup2,
    
    //板间通信组件
    COM_BSP,
    
    COM_ModulesNum
    
}COM_ModuleID;

//针对can的数据类型映射，包含格式化数据和非格式化数据
typedef enum 
{
    //自定义通协议数据
    CAN_FormatData,
    
    //设备回传数据
    CAN_DevData,
    
    CAN_IDsNum
    
}CAN_DataID;

//便于集中管理的通信组件集合
extern COMInfoTypedef *COM_Modules[COM_ModulesNum];

//以自定义通信协议发送的数据ID集合
extern const uint32_t CAN_FormatStdIDs[];
//以非自定义通信协议发送的数据ID集合
extern const uint32_t CAN_UnformatStdIDs[];

//用户组件映射
COM_ModuleID UART_DealFuncMap(UART_HandleTypeDef *UartHandle);
COM_ModuleID CAN_DealFuncMap(CAN_HandleTypeDef *CanHandle, CAN_RxHeaderTypeDef *pHeader, CAN_DataID *pDataID);

#endif

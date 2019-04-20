#include "Com_UserModules.h"
#include "Com_CanDriver.h"
#include "ComToPC.h"
#include "ComToMaster.h"
#include "ComToBSP.h"
#include "M3508.h"

//内部声明的空组件结构体，用于标识未找到对应组件的情况
COMInfoTypedef Null_COM_Module;

//通信组件集合
COMInfoTypedef *COM_Modules[COM_ModulesNum] = 
{
    //空组件结构体
    &Null_COM_Module,
    
    //与小电脑的通信组件接口
    &PC_COM_Module,
    
    //与控制端的通信组件接口
    &Mstr_COM_Module,
    
    //与其他板子的通信组件接口
    &BSP_COM_Module,
};

//以自定义通信协议发送的数据ID集合
const uint32_t CAN_FormatStdIDs[] = 
{
    0x2FF,           //板间通信ID
};

//以非自定义通信协议发送的数据ID集合
const uint32_t CAN_UnformatStdIDs[] = 
{
    0x200 + 0x01,   //M3508的返回数据_电机1
    0x200 + 0x02,   //M3508的返回数据_电机2
    0x200 + 0x03,   //M3508的返回数据_电机3
};

//can数据处理函数集合
void (*pCANDataDealFuncs[CAN_IDsNum])(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type) = 
{
    RcvFormatCanData,   //格式化数据接收
    Motor1DataDeal,     //M3508的返回数据_电机1
    Motor2DataDeal,     //M3508的返回数据_电机2
    Motor3DataDeal,     //M3508的返回数据_电机3
};

/*******************  Map Of Modules  ***********************/

//选择处理函数
inline COM_ModuleID UART_DealFuncMap(UART_HandleTypeDef *UartHandle)
{
    COM_ModuleID result = COM_NULL;
    
    if(PC_COM_UART == UartHandle->Instance)
    {
        result = COM_MSTR;//COM_PC;
    }
    
//如果使用板间串口
#if defined(USING_BSP_UART)

    else if(BSP_COM_UART == UartHandle->Instance)
    {
        result = COM_BSP;
    }

#endif /*  IS_USING_BSP_CAN  */
    
    return result;
}

//选择处理函数
inline COM_ModuleID CAN_DealFuncMap(CAN_HandleTypeDef *CanHandle)
{
    COM_ModuleID result = COM_NULL;
    
//如果使用板间CAN
#if defined(USING_BSP_CAN)

    if(BSP_COM_CAN == CanHandle->Instance)
    {
        result = COM_BSP;
    }

#endif /*  IS_USING_BSP_CAN  */
    
    return result;
}

/*******************  END -> Map Of Modules  ***********************/

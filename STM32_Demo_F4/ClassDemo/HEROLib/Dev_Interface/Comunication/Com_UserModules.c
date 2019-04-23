#include "Com_UserModules.h"
#include "Com_CanDriver.h"
#include "ComToPC.h"
#include "ComToMaster.h"
#include "ComToBSP.h"
#include "ComToCAN_Dev.h"

//内部声明的空组件结构体，用于标识未找到对应组件的情况
COMInfoTypedef Null_COM_Module;

//通信组件集合
COMInfoTypedef *COM_Modules[COM_ModulesNum] = 
{
    //空组件结构体
    &Null_COM_Module,
    
    /*  使用串口的通信组件  */
    //与小电脑的通信组件接口
    &PC_COM_Module,
    
    //与控制端的通信组件接口
    &Mstr_COM_Module,
    
    /*  使用CAN的通信组件,顺序按先非格式化后格式化,先CAN1后CAN2  */
    //设备组1的通信组件
    &DevGroup1_COM_Module,
    
    //设备组2的通信组件
    &DevGroup2_COM_Module,
    
    /* CAN1组件 */
    //与其他板子的通信组件接口
    &BSP_COM_Module,
    
    /* CAN2组件 */
};

//以自定义通信协议发送的数据ID集合，与上面“使用CAN的通信组件”顺序一一对应。
const uint32_t CAN1_FormatStdIDs[] = 
{
    0x2FF,           //板间通信ID
};
uint32_t CAN1_FormatStdIDNum = (sizeof(CAN1_FormatStdIDs) / sizeof(uint32_t));

const uint32_t CAN2_FormatStdIDs[] = 
{
    0x2FF,           //板间通信ID
};
uint32_t CAN2_FormatStdIDNum = (sizeof(CAN2_FormatStdIDs) / sizeof(uint32_t));

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

//选择处理函数,最后一个传入参数获取处理方式
inline COM_ModuleID CAN_DealFuncMap(CAN_HandleTypeDef *CanHandle, CAN_RxHeaderTypeDef *pHeader, CAN_DataID *pDataID)
{
    COM_ModuleID result = COM_NULL;
    uint32_t Cnt = 0, StdID = pHeader->StdId;
    bool isFindID = false;
    
    //挂载在CAN1下的组件
    if(CAN1 == CanHandle->Instance)
    {
        //遍历查询ID
        for(Cnt = 0; Cnt < CAN1_FormatStdIDNum; Cnt++)
        {
            if(CAN1_FormatStdIDs[Cnt] == StdID)
            {
                //标记为格式化数据
                (*pDataID) = CAN_FormatData;
                
                isFindID = true;
                
                break;
            }
        }
        //如果在格式化数据ID集合中没有找到匹配项
        if(isFindID)
        {
            //如果找到了格式化数据ID
            //其标号应是串口号加两个非格式化组件再加上格式化ID下标
            result = (COM_ModuleID)(Cnt + 2 + COM_UART_Num);
        }
        else
        {
            //遍历查询ID
            for(Cnt = 0; Cnt < DevGroup1StdIDNum; Cnt++)
            {
                if(CAN_DevGroup1StdIDs[Cnt] == StdID)
                {
                    //标记为设备数据
                    (*pDataID) = CAN_DevData;
                    result = COM_DevGroup1;
                    isFindID = true;
                    break;
                }
            }
        }
        
    }
    else if(CAN2 == CanHandle->Instance)
    {
        //遍历查询ID
        for(Cnt = 0; Cnt < CAN2_FormatStdIDNum; Cnt++)
        {
            if(CAN2_FormatStdIDs[Cnt] == StdID)
            {
                //标记为格式化数据
                (*pDataID) = CAN_FormatData;
                
                isFindID = true;
                
                break;
            }
        }
        //如果在格式化数据ID集合中没有找到匹配项
        if(isFindID)
        {
            //如果找到了格式化数据ID
            //其标号应是串口号加两个非格式化组件加CAN1格式化组件数目再加上格式化ID下标
            result = (COM_ModuleID)(Cnt + 2 + COM_UART_Num + CAN1_FormatStdIDNum);
        }
        else
        {
            //遍历查询ID
            for(Cnt = 0; Cnt < DevGroup2StdIDNum; Cnt++)
            {
                if(CAN_DevGroup2StdIDs[Cnt] == StdID)
                {
                    //标记为设备数据
                    (*pDataID) = CAN_DevData;
                    result = COM_DevGroup1;
                    isFindID = true;
                    break;
                }
            }
        }
    }
    else
    {
        //不可能到这儿
    }
    
    return result;
}

/*******************  END -> Map Of Modules  ***********************/

#include "middleInit.h"
#include "Com_Functions.h"
#include "ComToPC.h"
#include "ComToMaster.h"
#include "ComToBSP.h"

//初始化通信组件
void COM_ModulesInit(void)
{
    //初始化与小电脑的通信组件
    PC_COM_ModuleInit(COM_Modules[COM_PC]);
    
    //初始化与控制端的通信组件
    Mstr_COM_ModuleInit(COM_Modules[COM_MSTR]);
    
    //初始化与其他控制板的通信组件
    //BSP_COM_ModuleInit(COM_Modules[COM_BSP]);
}

#include "middleInit.h"
#include "Com_Functions.h"
#include "ComToPC.h"
#include "ComToMaster.h"
#include "ComToBSP.h"

//��ʼ��ͨ�����
void COM_ModulesInit(void)
{
    //��ʼ����С���Ե�ͨ�����
    PC_COM_ModuleInit(COM_Modules[COM_PC]);
    
    //��ʼ������ƶ˵�ͨ�����
    Mstr_COM_ModuleInit(COM_Modules[COM_MSTR]);
    
    //��ʼ�����������ư��ͨ�����
    //BSP_COM_ModuleInit(COM_Modules[COM_BSP]);
}

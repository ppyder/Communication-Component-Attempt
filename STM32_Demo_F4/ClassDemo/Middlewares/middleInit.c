#include "middleInit.h"
#include "ComToCAN_Dev.h"
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
    
    //��ʼ����CAN1��֡ͨ���豸��ͨ�����
    DevGroup1_COM_ModuleInit(COM_Modules[COM_DevGroup1]);
    
    //��ʼ�����������ư��ͨ�����
    //BSP_COM_ModuleInit(COM_Modules[COM_BSP]);
}

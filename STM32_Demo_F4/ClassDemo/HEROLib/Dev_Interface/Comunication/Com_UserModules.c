#include "Com_UserModules.h"
#include "Com_CanDriver.h"
#include "ComToPC.h"
#include "ComToMaster.h"
#include "ComToBSP.h"
#include "M3508.h"

//�ڲ������Ŀ�����ṹ�壬���ڱ�ʶδ�ҵ���Ӧ��������
COMInfoTypedef Null_COM_Module;

//ͨ���������
COMInfoTypedef *COM_Modules[COM_ModulesNum] = 
{
    //������ṹ��
    &Null_COM_Module,
    
    //��С���Ե�ͨ������ӿ�
    &PC_COM_Module,
    
    //����ƶ˵�ͨ������ӿ�
    &Mstr_COM_Module,
    
    //���������ӵ�ͨ������ӿ�
    &BSP_COM_Module,
};

//���Զ���ͨ��Э�鷢�͵�����ID����
const uint32_t CAN_FormatStdIDs[] = 
{
    0x2FF,           //���ͨ��ID
};

//�Է��Զ���ͨ��Э�鷢�͵�����ID����
const uint32_t CAN_UnformatStdIDs[] = 
{
    0x200 + 0x01,   //M3508�ķ�������_���1
    0x200 + 0x02,   //M3508�ķ�������_���2
    0x200 + 0x03,   //M3508�ķ�������_���3
};

//can���ݴ���������
void (*pCANDataDealFuncs[CAN_IDsNum])(uint8_t *pData, uint32_t DataLength, COM_ModuleID COM_Type) = 
{
    RcvFormatCanData,   //��ʽ�����ݽ���
    Motor1DataDeal,     //M3508�ķ�������_���1
    Motor2DataDeal,     //M3508�ķ�������_���2
    Motor3DataDeal,     //M3508�ķ�������_���3
};

/*******************  Map Of Modules  ***********************/

//ѡ������
inline COM_ModuleID UART_DealFuncMap(UART_HandleTypeDef *UartHandle)
{
    COM_ModuleID result = COM_NULL;
    
    if(PC_COM_UART == UartHandle->Instance)
    {
        result = COM_MSTR;//COM_PC;
    }
    
//���ʹ�ð�䴮��
#if defined(USING_BSP_UART)

    else if(BSP_COM_UART == UartHandle->Instance)
    {
        result = COM_BSP;
    }

#endif /*  IS_USING_BSP_CAN  */
    
    return result;
}

//ѡ������
inline COM_ModuleID CAN_DealFuncMap(CAN_HandleTypeDef *CanHandle)
{
    COM_ModuleID result = COM_NULL;
    
//���ʹ�ð��CAN
#if defined(USING_BSP_CAN)

    if(BSP_COM_CAN == CanHandle->Instance)
    {
        result = COM_BSP;
    }

#endif /*  IS_USING_BSP_CAN  */
    
    return result;
}

/*******************  END -> Map Of Modules  ***********************/

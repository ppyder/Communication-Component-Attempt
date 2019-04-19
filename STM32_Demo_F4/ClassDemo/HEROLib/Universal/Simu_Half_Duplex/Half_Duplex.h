#ifndef HALF_DUPLEX_H
#define HALF_DUPLEX_H

#include "ComStructDef.h"

#include <stdint.h>
#include <stdbool.h>

typedef enum 
{
    //�޴���
    Hf_NoError = 0,
    
    //δ��ʼ���ṹ��
    Hf_Error_UnInited,
    
    Hf_ErrorCodeNum
    
}Hf_ErrorCode;

//�ṹ�嶨�壺��˫������ʵ�ֽṹ��
typedef struct 
{
    //��ʶ�Ƿ��Ѿ���ʼ���ýṹ��
    bool isInited;
    
    /***************  �쳣����  ****************/
    //������
    Hf_ErrorCode ErrorCode;
    
    //�Ա�����쳣������������
    char* ErrorDescription;
    
    /*************  END -> �쳣����  ************/
    
    /************  ����״̬��ʶ  ************/
    //��ʶ��ǰ������Ƿ�������״̬
    bool isBlocked;
    
    /*******  ���ͺͽ��ձ�־  *******/
    //(is Get Message)    
    //��ʶ�����Ƿ��Ѿ����յ�����Ϣ
    bool isGetMsg;
    
    /******  ͨ���������  ******/
    //ָ��ͨ������Ľṹ��ָ��
    COMInfoTypedef *pCOM;
    
    /******  ������  ******/    
    //(Request Error Count)
    //���󲵻ؼ�����
    //�������󱻾ܾ����ü�������һ��
    //���ۼӵ�ErrorCntMax�󴥷��쳣������
    uint32_t RqstErrorCnt; 
    
    //(Error Count Max)
    //���ؼ����������ֵ�������ж�ͨѶ�ж�
    //��ʼ��Ϊ0���������쳣������
    uint32_t ErrorCntMax;

    /******  ��������  ******/    
    //ͨ�Ŷ������쳣������
    void (*pBlockError)(void);
    
}Hf_DuplexTypedef;

void Hf_DuplexStructDeInit(Hf_DuplexTypedef *pModule);
void Hf_HaveReceived(Hf_DuplexTypedef *pModule);
void Hf_SendData(Hf_DuplexTypedef *pModule, uint8_t CMD);
void Hf_Reconnect(Hf_DuplexTypedef *pModule);

#endif


#ifndef BSP_DATA_H
#define BSP_DATA_H

//���ļ��ж����û�ͨ����ص��������ͣ�������Ҫ���СΪ���ͱ�����������
#include <stdint.h>
#include <stdbool.h>

//�ṹ�嶨�壺ͨ������ͷ
typedef struct
{
    uint8_t DataHead1;  //�̶�����ͷ1:0xaa
    uint8_t DataHead2;  //�̶�����ͷ2:0xbb
    uint8_t MsgTypeID;  //�������ͱ�ʶ����ָ����������
    uint8_t FeedbackID; //��Է������ո������յ���ָ������
}BSP_DataHead; 

//�ṹ�嶨�壺ͨ������β
typedef struct
{
    uint32_t Verification;  //У�飬��ʱΪ���
    
}BSP_DataTail; 

/***************  ���¶����û���������  ********************/

//ö�ٶ��壺������������ID
enum BSP_TxMsgTypes
{
    //0 - ����ָ�����
    BSPTx_ErrorCMD = 0,
    
    //1 - ��ָ��
    BSPTx_EmptyCMD,
    
    //2 - ����ָ��
    BSPTx_TestCMD,

    //����ID����ֵ���ޣ�����������У������鶨��ȣ�
    BSPTx_Num
};

//ö�ٶ��壺������������ID
enum BSP_RxMsgTypes
{
    //0 - ����ָ�����
    BSPRx_ErrorData = 0,
    
    //1 - ��ָ���
    BSPRx_EmptyData,
    
    //2 - ��������
    BSPRx_TestData,
    
    //����ID����ֵ���ޣ�����������У������鶨��ȣ�
    BSPRx_Num
};

//�ṹ�嶨�壺�������ݷ���
typedef struct
{
    float Data;   //�������ݷ���
    
}TestDataType; 

//�ṹ�嶨�壺���Կ���ָ��
typedef struct
{
    float CMD;   //����ָ��
    
}TestCMDType; 

#endif

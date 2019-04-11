#ifndef PC_DATA_H
#define PC_DATA_H

//���ļ��ж����û�ͨ����ص��������ͣ�������Ҫ���СΪ���ͱ�����������
//��Լ����0��ָ��Ϊ��ָ���ָʾ�����ָ��

#include <stdint.h>
#include <stdbool.h>

//�ṹ�嶨�壺ͨ������ͷ
typedef struct
{
    uint8_t DataHead1;  //�̶�����ͷ1:0xaa
    uint8_t DataHead2;  //�̶�����ͷ2:0xbb
    uint8_t MsgTypeID;  //�������ͱ�ʶ����ָ����������
    uint8_t Reserve;    //Ԥ��ռλ��ʹ�øýṹ���СΪ4�ֽڵ�������
    
}PCData_DataHead; 

//�ṹ�嶨�壺ͨ������β
typedef struct
{
    uint32_t Verification;  //У�飬��ʱΪ���
    
}PCData_DataTail; 

/***************  ���¶����û���������  ********************/

 //!!!ע�⣺����ϵ���������������������

//ö�ٶ��壺������������ID
enum PC_TxMsgTypes
{
    //ѭ��ģʽ
    PCTx_FollowMode = 0,
    
    //ʮ��ģʽ
    PCTx_CrossMode,
    
    //ץȡģʽ
    PCTx_PickMode,
    
    //����Խ��ģʽ
    PCTx_ObstacleMode,
    
    //����ID����ֵ���ޣ�����������У������鶨��ȣ�
    PCTx_Num
};

//ö�ٶ��壺������������ID
enum PC_RxMsgTypes
{
    //ѭ��ģʽ
    PCRx_FollowMode = 0,
    
    //ʮ��ģʽ
    PCRx_CrossMode,
    
    //ץȡģʽ
    PCRx_PickMode,
        
    //����ID����ֵ���ޣ�����������У������鶨��ȣ�
    PCRx_Num
};

//�ṹ�嶨�壺Ѳ������
typedef struct
{
    float Deflection;   //������̬��ƫ��
    float Distance;     //�뱻��ʶ�ߵ�λ��ƫ���������
    
}FollowLineType; 


//�ṹ�嶨�壺ʮ������
typedef struct
{
    float Deflection;   //������̬��ƫ��
    float AxisX;        //ʮ���������������ϵ�е�����X
    float AxisY;        //ʮ���������������ϵ�е�����Y
    
}CrossDataType; 

//�ṹ�嶨�壺ץȡ�Խ�����
typedef struct
{
    float Argument;     //ƫ����������
    float Modulus;      //ƫ������ģ
    float Deflection;   //������̬��ƫ��
    
}PickDataType; 

#endif

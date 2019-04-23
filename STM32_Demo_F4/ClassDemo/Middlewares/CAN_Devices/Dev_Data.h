#ifndef DEV_DATA_H
#define DEV_DATA_H

//���ļ��ж���CAN�豸Ҫ�������֡ͨ��Э�����ݣ����ݽṹ���Ȳ��ܳ����˸��ֽڡ�

#include <stdint.h>
#include <stdbool.h>

/***************  ���¶����豸���ݻ�����  ********************/

//ö�ٶ��壺�豸��1������������ID
enum DevGroup1_TxMsgTypes
{
    //ѭ��ģʽ
    DevGroup1Tx_M3508 = 0,
    
    //����ID����ֵ���ޣ�����������У������鶨��ȣ�
    DevGroup1Tx_Num
};

//ö�ٶ��壺�豸��1������������ID
enum DevGroup1_RxMsgTypes
{
    //ѭ��ģʽ
    DevGroup1Rx_M3508 = 0,
        
    //����ID����ֵ���ޣ�����������У������鶨��ȣ�
    DevGroup1Rx_Num
};

//�ṹ�嶨�壺M3508����ĵ����������ݽṹ��
typedef struct
{
    int16_t Current1;
    int16_t Current2;
    int16_t Current3;
    int16_t Current4;
    
}M3508TxType; 


//�ṹ�嶨�壺M3508����ĵ����������ݽṹ��
typedef struct
{
    int16_t Angle;      //ת�ӻ�е��λ��
    int16_t Speed;      //ת��ת�٣�RPM��
    int16_t Current;    //ĸ�ߵ���
    int16_t Temperature;//����¶�
    
}M3508RxType;  

#endif

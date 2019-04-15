#ifndef MASTER_DATA_H
#define MASTER_DATA_H

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
    uint8_t IDFeedback; //����λ�������ո������յ���ָ������
    
    uint32_t Verification;  //��������λ����ʶͷ��ݵ�ͷУ�飨���У�飩
    
}MstrData_DataHead; 

//�ṹ�嶨�壺ͨ������β
typedef struct
{
    uint32_t Verification;  //У�飬��ʱΪ���
    
}MstrData_DataTail; 

/***************  ���¶����û���������  ********************/

//ö�ٶ��壺������������ID
enum Mstr_TxMsgTypes
{
    //0 - ����ָ�����
    MstrTx_ErrorData = 0,
    
    //1 - ��ָ���
    MstrTx_EmptyData,
    
    //2 - ��������
    MstrTx_SampleData,
    
    //3 - �����˶�״̬����
    MstrTx_BaseMotionData,
    
    //4 - �滮�˶�״̬����
    MstrTx_PlanMotionTestData,
    
    //5 - �Զ�����״̬����
    MstrTx_AutoProcessData,
    
    //6 - �ϲ㶯��debug״̬����
    MstrTx_UpperActionDebugData,
    
    //7 - PID��������ֵ
    MstrTx_PIDData,
    
    //����ID����ֵ���ޣ�����������У������鶨��ȣ�
    MstrTx_Num
};

//ö�ٶ��壺������������ID
enum MstrRxMsgTypes
{
    //0 - ����ָ�����
    MstrRx_ErrorCMD = 0,
    
    //1 - ��ָ��
    MstrRx_EmptyCMD,
    
    //2 - ����ָ��
    MstrRx_SampleCMD,
    
    //3 - �����˶�״̬����
    MstrRx_BaseMotionCMD,
    
    //4 - �滮�˶�״̬����
    MstrRx_PlanMotionTestCMD,
    
    //5 - �Զ�����״̬����
    MstrRx_AutoProcessCMD,
    
    //6 - �ϲ㶯��debug״̬����
    MstrRx_UpperActionDebugCMD,
    
    //7 - ����PID������ָ��
    MstrRx_PIDSetCMD,
            
    //����ID����ֵ���ޣ�����������У������鶨��ȣ�
    MstrRx_Num
};

/****************************  ����1 ����ָ��������  ********************************/

//�ṹ�嶨�壺�������ݰ�
typedef struct
{
    uint16_t Time_10ms;             //ʱ��
    int16_t Input;                  //����
    int16_t Output;                 //��Ӧ
    int16_t OutputUc;               //���������
    
}SampleDataTypedef;
 
//�ṹ�嶨�壺��������ָ��
typedef struct
{
    uint8_t CMD;                    //ָ��
    uint8_t Reserve1;               //Ԥ��
    uint8_t Reserve2;               //Ԥ��
    uint8_t Reserve3;               //Ԥ��
    
}SampleCMDTypedef;

/****************************  END -> ����ָ��������  ********************************/

/****************************  ����2 ���̻����˶�ָ��������  ********************************/

//�ṹ�嶨�壺���̻����˶�ң��ָ�����ȫ�ֶ�����ģʽ��
typedef struct
{
    //�����ٶȴ�С��ģ����λmm/s��
    float Speed;
    //�����ٶȷ��򣨻���ȫ�ֶ�λ����ϵ����λ�㣩
    float SpeedDir;
    //�����������ٶȣ��������¿���ʱ��Ϊ��
    float RotateSpeed;
    
    //�ϲ����ָ��                                                     
    uint8_t PickCMD;    //ץȡָ��޶���0���ȷ�������צ1����λ���ٺ�צ��λ�ͷ�2��
    uint8_t ShootCMD;   //����ָ��(ȡ�޹ǵ�צ�ͷź��Զ��󳷣��������Զ���λ���޶���0������1��δ��ɺ󳷻�λʱ����ָ��)��Ч)
    uint8_t HandoverCMD;//����ָ��(�޶���0�����������1������2���ͷ�����3)
    uint8_t Reserve;    //Ԥ��ռλ
    
}BaseMotionCMDTypedef;

//�ṹ�嶨�壺���̻����˶�״̬����
typedef struct
{
    //�����ٶȴ�С��ģ����λmm/s��
    float Speed;
    //�����ٶȷ��򣨻���ȫ�ֶ�λ����ϵ����λ�㣩
    float SpeedDir;
    //�����������ٶȣ��������¿���ʱ��Ϊ��
    float RotateSpeed;
    
    //��������X
    float CoordinateX;
    //��������Y
    float CoordinateY;
    //����ȫ�ֶ�λ����ϵ����̬��
    float PoseAngle;
    
}BaseMotionDataTypedef;

/****************************  END -> ���̻����˶�ָ��������  ********************************/

/****************************  ����3 �滮�˶�����ָ��������  ********************************/

//�ṹ�嶨�壺���̹滮�˶�����ָ��
typedef struct
{
    //�滮�˶����Ʋ���
    uint8_t MotionCMD;      //�˶�����ָ�1Ϊ��ʼ�˶���0Ϊֹͣ�˶���
    bool    isParamsValid;  //ָʾ���η������������Ƿ���Ч
    bool    isLockHead;     //����˶��������Ƿ���ͷ
    uint8_t Reserve;        //Ԥ��ռλ��
    
    //�滮�˶��������
    int32_t CoordinateX;    //Ŀ������X
    int32_t CoordinateY;    //Ŀ������Y
    
    int32_t CircleOrigin1X; //��ѹԲ1Բ������X
    int32_t CircleOrigin1Y; //��ѹԲ1Բ������Y
    
    int32_t Circle1DirX;     //��ѹ����1�ķ�����������X
    int32_t Circle1DirY;     //��ѹ����1�ķ�����������Y
    
    int32_t CircleOrigin2X;  //��ѹԲ2Բ������X
    int32_t CircleOrigin2Y;  //��ѹԲ2Բ������Y
    
    int32_t Circle2DirX;     //��ѹ����2�ķ�����������X
    int32_t Circle2DirY;     //��ѹ����2�ķ�����������Y
    
}BasePlanCtrlCMDTypedef;

//�ṹ�嶨�壺���̹滮�˶���������
typedef struct
{
    //�滮�˶����ز���
    bool isPlanSuccessfully; //����Ƿ�����·���ɹ�
    uint8_t SectionFlag;     //·�α�־
    uint8_t Reserve[2];      //Ԥ��ռλ��
    
    //�˶�״̬����
    float DistanceHaveMoved; //�Ѿ��߹���·��
    float FullDistance;      //·��ȫ��
    
    //����ʵʱ����X
    float CoordinateX;
    //����ʵʱ����Y
    float CoordinateY;
    //����ȫ�ֶ�λ����ϵ����̬��
    float PoseAngle;
    
}BasePlanDataTypedef;

/****************************  END -> �滮�˶�����ָ��������  ********************************/

/****************************  ����4 �Զ����̿���ָ��������  ********************************/

//���̰��Զ����̿���
typedef struct
{
    //�˶���Ԥ����
    float Speed;        //�����ٶȴ�С��ģ����λmm/s��
    float SpeedDir;     //�����ٶȷ��򣨻���ȫ�ֶ�λ����ϵ����λ�㣩
    float RotateSpeed;  //�����������ٶȣ��������¿���ʱ��Ϊ��
    
    //���̿��Ʋ���
    bool isStopProcess;     //ֹͣ����
    bool ToNextState;       //�������̿�������һ��״̬
    bool StateReset;        //��λ״̬�������������̣���ʱ��δ�𶯣�
    uint8_t SetCurrentState;//���õ�ǰ״̬�Է����ĳһ�����̽ڵ㴦����
    
    //�ϲ����ָ��                                                     
    uint8_t PickCMD;    //ץȡָ��޶���0���ȷ�������צ1����λ���ٺ�צ��λ�ͷ�2��
    uint8_t ShootCMD;   //����ָ��(ȡ�޹ǵ�צ�ͷź��Զ��󳷣��������Զ���λ���޶���0������1��δ��ɺ󳷻�λʱ����ָ����Ч)
    uint8_t HandoverCMD;//����ָ��(�޶���0�����������1������2���ͷ�����3)
    uint8_t Mode;       //ָ����ȫ�Զ�����0����ʹ�ܿ�ʼ����ָ����˶���Ԥ���֣����ǰ��Զ�1��ʹ�����̿��Ʋ��ֺ��ϲ����ָ�
    
}MR1CtrlCMDTypedef;

//���̰��Զ����̿���
typedef struct
{
    //���̿��Ʋ���
    bool isAutoMode;     //����Ƿ���ȫ�Զ�ģʽ
    uint8_t CurrentState;//��ǵ�ǰ�����̺�
    bool isActionDoing;  //����ϲ㶯���Ƿ��ڽ�����
    uint8_t Reserve;     //����ռλ
    
    //����ʵʱ����X
    float CoordinateX;
    //����ʵʱ����Y
    float CoordinateY;
    //����ȫ�ֶ�λ����ϵ����̬��
    float PoseAngle;
    
}MR1CtrlDataTypedef;

/****************************  END -> �Զ����̿���ָ��������  ********************************/

/****************************  ����5 �ϲ㶯������ָ��������  ********************************/

//�ϲ㶯������ָ��
typedef struct
{
    uint8_t SetSteerPos;        //�趨���Ӷ���Ľ�λ�ã�0-180����λ�㣩
    uint8_t CylinderCMD;        //���׿���ָ���0λ�����ף���1λС���ף�1Ϊ�Ƴ���
    uint8_t SetHandoverMotorPos; //�趨���ӵ����λ�ã�0-180����λ�㣩
    uint8_t Reserve;             //Ԥ��ռλ��

    int16_t SetPickMotorPos;    //�趨ץȡ�����λ��(���궨)
    int16_t SetShootMotorSpeed; //�趨���������ٶȣ�������ǰ��Ϊ������λmm/s��
    
}ActionDebugCMDTypedef;

//�ϲ㶯��������Ϣ
typedef struct
{
    uint16_t HandoverMotorPos; //���ӵ����λ�ã���0-180����λ�㣩
    uint16_t HandoverSteerPos; //���Ӷ����λ�ã���0-180����λ�㣩
    
    int16_t PickMotorPos;    //ץȡ�����λ��(���궨)
    int16_t ShootMotorSpeed; //���������ٶȣ�������ǰ��Ϊ������λmm/s��
    
}ActionDebugDataTypedef;

/****************************  END -> �ϲ㶯������ָ��������  ********************************/

/****************************  ����6 PID��������ָ��  ********************************/

//ö�ٶ��壺PID��ID�б�
typedef enum
{
    PID_SingleMotor = 0x00,     //�����ת��PID
    PID_SingleMotorAngle,       //�����λ��PID
    PID_RobAccelerated,         //�������ٶ�PID
    PID_RobLocation,            //����λ��PID
    PID_RobEnd,                 //�����յ�PID
    PID_RobTime,                //����ʱ��PID

    PID_PickAngle,          //ץȡ�Ƕ�PID
    PID_PickSpeed,          //ץȡ�ٶ�PID
    PID_LaunchAngle,        //����Ƕ�PID
    PID_LaunchSpeed,        //�����ٶ�PID
    
}PID_IDList;

//PID����ָ��
typedef struct
{
    bool isSetParam;    //����Ƿ������ò���(��Ϊ�٣���������λ����ȡ����)
    uint8_t SetTarget;  //��Ǳ�����/��ȡ��Ŀ��
    uint8_t Reserve[2]; //Ԥ��ռλ��
    
    float Kp;
    float Ki;
    float Kd;
    
    float I_e_Threshold;    //�����Ƿ�ʹ�û��ֵ��������
    
    float I_Max;            //�����޷�
    float OutMax;           //����޷�
    
}PIDDataSetTypedef;

//��ȡPID����ֵ
typedef struct
{
    PID_IDList PID_ID;     //��������ص�Ŀ���Ӧ�ı�ǩ
    uint8_t    Reserve[3]; //Ԥ��ռλ��
    
    float Kp;
    float Ki;
    float Kd;
    
    float I_e_Threshold;    //�����Ƿ�ʹ�û��ֵ��������
    
    float I_Max;            //�����޷�
    float OutMax;           //����޷�
    
}PIDDataGetTypedef;

/****************************  END -> PID��������ָ��  ********************************/

#endif

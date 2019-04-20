#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "ComStructDef.h"
#include "usart.h"
#include "can.h"

//���ڸ������˵������ͷ�ļ� @"Com_Functions.h"

/*  ͨ������ӿڣ�ʹ��˫���������գ������������ͣ�֧�ֶ������ݽ��գ�����/���������ݷ���  */

//������������
extern char *COM_ErrorDescriptions[COM_ErrorCodeNum];

//��ָ�������Ϊ����ͨ�������ָ��ռλ
#define NULL_MESSAGE 0

//���ݷ��ͺ�����ֱ�ӷ����Ѿ������ڻ������е����ݣ�
bool COM_SendData(COMInfoTypedef *pModule);

#endif

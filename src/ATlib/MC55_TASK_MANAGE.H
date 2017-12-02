/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : MC55_TASK_MANAGE.h
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2011-09-23      azh           the original version
******************************************************************************/
#ifndef		_SCI_TASK_H
#define		_SCI_TASK_H

#include	"MC55_module.h"

typedef struct									//����������������
{
	unsigned short	usType;
	unsigned char	ucChNo;						//ͨ�����룬����AT���ݷ���
//	unsigned char	ucRetryTimes;				//�ط�����
//	unsigned short	usTimeOutCnt;				//��ʱ������
	UN_CHANNEL_ADDR	unAddrInfo;	
	unsigned short	usDataLength;				//����������
	unsigned char	ucData[CON_FRAMEDATA_LENGTH_MAX];
}ST_TASKDATA,*PST_TASKDATA;			

typedef struct									//����������������
{
	unsigned short	usHeadPtr;		//ͷָ��
	unsigned short	usTailPtr;		//βָ��
	unsigned short	usMaxDepth;		//���������
	PST_TASKDATA	pstTask;
}ST_TASK_LIST,*PST_TASK_LIST;	

__EXTERN void TaskInit(void);
__EXTERN unsigned char TaskListFill(PST_TASK_LIST pTaskList , PST_TASKDATA pTaskData);
__EXTERN unsigned char TaskListGet(PST_TASK_LIST pTaskList , PST_TASKDATA pTaskData);
__EXTERN unsigned char TaskListGetValidCount(PST_TASK_LIST pTaskList);

__EXTERN unsigned char TaskListPopTask(PST_TASK_LIST pTaskList);
__EXTERN PST_TASKDATA TaskListGetHeadTaskPtr(PST_TASK_LIST pTaskList);


#endif




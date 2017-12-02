/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : MC55_TASK_MANAGE.h
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2011-09-23      azh           the original version
******************************************************************************/
#ifndef		_SCI_TASK_H
#define		_SCI_TASK_H

#include	"MC55_module.h"

typedef struct									//任务数据数据内容
{
	unsigned short	usType;
	unsigned char	ucChNo;						//通道号码，用于AT数据发送
//	unsigned char	ucRetryTimes;				//重发次数
//	unsigned short	usTimeOutCnt;				//超时计数器
	UN_CHANNEL_ADDR	unAddrInfo;	
	unsigned short	usDataLength;				//数据区长度
	unsigned char	ucData[CON_FRAMEDATA_LENGTH_MAX];
}ST_TASKDATA,*PST_TASKDATA;			

typedef struct									//任务数据数据内容
{
	unsigned short	usHeadPtr;		//头指针
	unsigned short	usTailPtr;		//尾指针
	unsigned short	usMaxDepth;		//缓冲区深度
	PST_TASKDATA	pstTask;
}ST_TASK_LIST,*PST_TASK_LIST;	

__EXTERN void TaskInit(void);
__EXTERN unsigned char TaskListFill(PST_TASK_LIST pTaskList , PST_TASKDATA pTaskData);
__EXTERN unsigned char TaskListGet(PST_TASK_LIST pTaskList , PST_TASKDATA pTaskData);
__EXTERN unsigned char TaskListGetValidCount(PST_TASK_LIST pTaskList);

__EXTERN unsigned char TaskListPopTask(PST_TASK_LIST pTaskList);
__EXTERN PST_TASKDATA TaskListGetHeadTaskPtr(PST_TASK_LIST pTaskList);


#endif





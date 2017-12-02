/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : MC55_TASK_MANAGE.C
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2011-09-23      azh           the original version
******************************************************************************/
#include "..\inc\global_config.h"

#include "MC55_TASK_MANAGE.h"

/********************************************************************
	函数名称:	TaskListGetValidCount	
	功能	:	获取任务缓冲区长度	
	入口参数:	pTaskList  任务队列指针
	出口参数:	有效任务数量
*********************************************************************/

unsigned char TaskListGetValidCount(PST_TASK_LIST pTaskList)
{
	unsigned char	ucCnt = 0;
	
	if(pTaskList->usHeadPtr == pTaskList->usTailPtr)
		ucCnt = 0;
	else
	{
		if(pTaskList->usHeadPtr > pTaskList->usTailPtr)//尾指针回绕
			ucCnt = pTaskList->usTailPtr + pTaskList->usMaxDepth -
						pTaskList->usHeadPtr;
		else
			ucCnt = pTaskList->usTailPtr - pTaskList->usHeadPtr;
	}
	return ucCnt;
}

/********************************************************************
	函数名称:	TaskListRead	
	功能	:	读取缓冲区数据，但头指针不移动
	入口参数:	pSciBuffer  缓冲区指针
				pDstBuf		读取数据存放缓冲区指针
				usLen		读取长度
	出口参数:	
*********************************************************************/

unsigned char TaskListGet(PST_TASK_LIST pTaskList , PST_TASKDATA pTaskData)
{
	unsigned char	ucLen = 0;

	ucLen = TaskListGetValidCount(pTaskList);
	if(ucLen == 0 )
		return 0;
	
	*pTaskData = pTaskList->pstTask[pTaskList->usHeadPtr];
	
	pTaskList->usHeadPtr++;

	if(pTaskList->usHeadPtr >= pTaskList->usMaxDepth)
		pTaskList->usHeadPtr = 0;
	
	return 1;
}

/********************************************************************
	函数名称:		TaskListGetHeadTaskPtr
	功能	:		获取最早进入队列的任务指针
	入口参数:		pTaskList	任务队列指针
	出口参数:		任务数据指针
*********************************************************************/

PST_TASKDATA TaskListGetHeadTaskPtr(PST_TASK_LIST pTaskList)
{
	PST_TASKDATA	pTask = NULL;
	unsigned char	ucLen = 0;
	
	ucLen = TaskListGetValidCount(pTaskList);
	if(ucLen == 0 )
		return NULL;
	
	pTask = &pTaskList->pstTask[pTaskList->usHeadPtr];
	return pTask;
}


/********************************************************************
	函数名称:		TaskListPopTask
	功能	:		任务处理完成后, 舍弃队列中最早的任务
	入口参数:		pTaskList		任务队列指针
	出口参数:		0	错误
					1	成功
*********************************************************************/

unsigned char TaskListPopTask(PST_TASK_LIST pTaskList)
{
	unsigned char	ucLen = 0;
	
	ucLen = TaskListGetValidCount(pTaskList);
	
	if(ucLen == 0 )
		return 0;
	
	pTaskList->usHeadPtr ++;
	if(pTaskList->usHeadPtr >= pTaskList->usMaxDepth)
		pTaskList->usHeadPtr = 0;
	
	return 1;
}

/********************************************************************
	函数名称:	SCIBufferReadByte	
	功能	:	从缓冲区数据读取一个字节，头指针不移动移动
	入口参数:	pSciBuffer  缓冲区指针
				ucOffset	读取的偏移量
				pch			数据存放指针
	出口参数:	实际读取的数据长度
*********************************************************************/

unsigned char TaskListFill(PST_TASK_LIST pTaskList , PST_TASKDATA pTaskData)
{
	unsigned char	ucLen = 0;

	ucLen = TaskListGetValidCount(pTaskList);
	if(ucLen >= pTaskList->usMaxDepth-1 )	//队列满
		return 0;
	
	pTaskList->pstTask[pTaskList->usTailPtr] = *pTaskData;

	pTaskList->usTailPtr++;
	if(pTaskList->usTailPtr >= pTaskList->usMaxDepth)
		pTaskList->usTailPtr = 0;
	
	return 1;
}



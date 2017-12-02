/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : MC55_TASK_MANAGE.C
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2011-09-23      azh           the original version
******************************************************************************/
#include "..\inc\global_config.h"

#include "MC55_TASK_MANAGE.h"

/********************************************************************
	��������:	TaskListGetValidCount	
	����	:	��ȡ���񻺳�������	
	��ڲ���:	pTaskList  �������ָ��
	���ڲ���:	��Ч��������
*********************************************************************/

unsigned char TaskListGetValidCount(PST_TASK_LIST pTaskList)
{
	unsigned char	ucCnt = 0;
	
	if(pTaskList->usHeadPtr == pTaskList->usTailPtr)
		ucCnt = 0;
	else
	{
		if(pTaskList->usHeadPtr > pTaskList->usTailPtr)//βָ�����
			ucCnt = pTaskList->usTailPtr + pTaskList->usMaxDepth -
						pTaskList->usHeadPtr;
		else
			ucCnt = pTaskList->usTailPtr - pTaskList->usHeadPtr;
	}
	return ucCnt;
}

/********************************************************************
	��������:	TaskListRead	
	����	:	��ȡ���������ݣ���ͷָ�벻�ƶ�
	��ڲ���:	pSciBuffer  ������ָ��
				pDstBuf		��ȡ���ݴ�Ż�����ָ��
				usLen		��ȡ����
	���ڲ���:	
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
	��������:		TaskListGetHeadTaskPtr
	����	:		��ȡ���������е�����ָ��
	��ڲ���:		pTaskList	�������ָ��
	���ڲ���:		��������ָ��
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
	��������:		TaskListPopTask
	����	:		��������ɺ�, �������������������
	��ڲ���:		pTaskList		�������ָ��
	���ڲ���:		0	����
					1	�ɹ�
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
	��������:	SCIBufferReadByte	
	����	:	�ӻ��������ݶ�ȡһ���ֽڣ�ͷָ�벻�ƶ��ƶ�
	��ڲ���:	pSciBuffer  ������ָ��
				ucOffset	��ȡ��ƫ����
				pch			���ݴ��ָ��
	���ڲ���:	ʵ�ʶ�ȡ�����ݳ���
*********************************************************************/

unsigned char TaskListFill(PST_TASK_LIST pTaskList , PST_TASKDATA pTaskData)
{
	unsigned char	ucLen = 0;

	ucLen = TaskListGetValidCount(pTaskList);
	if(ucLen >= pTaskList->usMaxDepth-1 )	//������
		return 0;
	
	pTaskList->pstTask[pTaskList->usTailPtr] = *pTaskData;

	pTaskList->usTailPtr++;
	if(pTaskList->usTailPtr >= pTaskList->usMaxDepth)
		pTaskList->usTailPtr = 0;
	
	return 1;
}


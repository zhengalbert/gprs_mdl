/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Job_Manage.h
 Description    : 1类数据/2类数据的任务上送、数据查询管理
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/
#ifndef _JOB_MANAGE_H_
#define _JOB_MANAGE_H_

__EXTERN unsigned char gucJob1HoursFlag;
__EXTERN unsigned char gucJobMonthUpFlag;			//按月为周期上送数据
//-------------------------------------------------------------------------------------------
__EXTERN unsigned char Job_ProcData(unsigned char ucJobClassNo);
__EXTERN void SaveJobRunInfo(unsigned char ucJobClassNo, unsigned char ucJobNo);
__EXTERN void GetJobRunInfoFormEE(unsigned char ucJobClassNo, unsigned char ucJobNo);

#endif

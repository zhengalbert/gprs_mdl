/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Job_Manage.h
 Description    : 1������/2�����ݵ��������͡����ݲ�ѯ����
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-02-05    azh     the original version
******************************************************************************/
#ifndef _JOB_MANAGE_H_
#define _JOB_MANAGE_H_

__EXTERN unsigned char gucJob1HoursFlag;
__EXTERN unsigned char gucJobMonthUpFlag;			//����Ϊ������������
//-------------------------------------------------------------------------------------------
__EXTERN unsigned char Job_ProcData(unsigned char ucJobClassNo);
__EXTERN void SaveJobRunInfo(unsigned char ucJobClassNo, unsigned char ucJobNo);
__EXTERN void GetJobRunInfoFormEE(unsigned char ucJobClassNo, unsigned char ucJobNo);

#endif

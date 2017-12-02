/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : CTask_Meter_Respose.h
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-08-15    azh     the original version
******************************************************************************/
#ifndef _CTASK_METER_RESPONSE_H_
#define _CTASK_METER_RESPONSE_H_

__EXTERN unsigned char CTask_Proc_Init(void);		//���ߴ���ģ���ʼ��
__EXTERN unsigned char CTask_Proc_Down_Timer(void);	//���������б��е��������ҵ����ʵ���Ҫ���������ʱ������д���
//__EXTERN unsigned char CTask_Proc_Up_Timer(void);	//��ʱ��������Ӧ�����е����ݣ�����Ҫ����ʱ������Ӧ�����ݼ�������ϴ�����վ
//__EXTERN void TimeChange_CTaskProc(STDATETIME stOldTime,STDATETIME stNewTime);	//�ⲿʱ��仯��Ҫ����
__EXTERN void CTask_Rebuild_Task(void);
//__EXTERN unsigned char CTask_Check_Task(void);
__EXTERN unsigned char Fill_TaskData_Manage_Invalid(unsigned short usDILabel,unsigned char ucTaskSour,unsigned char ucMPNo);
__EXTERN unsigned char Fill_TaskData_Manage(unsigned short usDILabel,unsigned char ucTaskSour,unsigned char ucMPNo);
__EXTERN unsigned char Get_TaskData(unsigned char *pucDes,unsigned char *pucSrc,unsigned char ucTaskNo,unsigned short usDI);
__EXTERN unsigned char Convert_Hex_To_TaskData(unsigned short usDI_Sml,unsigned char *pucSrc,unsigned char *pucDes);

#endif


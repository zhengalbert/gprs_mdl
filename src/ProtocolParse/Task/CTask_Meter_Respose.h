/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : CTask_Meter_Respose.h
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-08-15    azh     the original version
******************************************************************************/
#ifndef _CTASK_METER_RESPONSE_H_
#define _CTASK_METER_RESPONSE_H_

__EXTERN unsigned char CTask_Proc_Init(void);		//曲线处理模块初始化
__EXTERN unsigned char CTask_Proc_Down_Timer(void);	//调度任务列表中的任务，在找到合适的需要处理的任务时对其进行处理
//__EXTERN unsigned char CTask_Proc_Up_Timer(void);	//定时检查任务回应数据中的数据，当需要上行时，则将相应的数据检出，并上传给主站
//__EXTERN void TimeChange_CTaskProc(STDATETIME stOldTime,STDATETIME stNewTime);	//外部时间变化需要修正
__EXTERN void CTask_Rebuild_Task(void);
//__EXTERN unsigned char CTask_Check_Task(void);
__EXTERN unsigned char Fill_TaskData_Manage_Invalid(unsigned short usDILabel,unsigned char ucTaskSour,unsigned char ucMPNo);
__EXTERN unsigned char Fill_TaskData_Manage(unsigned short usDILabel,unsigned char ucTaskSour,unsigned char ucMPNo);
__EXTERN unsigned char Get_TaskData(unsigned char *pucDes,unsigned char *pucSrc,unsigned char ucTaskNo,unsigned short usDI);
__EXTERN unsigned char Convert_Hex_To_TaskData(unsigned short usDI_Sml,unsigned char *pucSrc,unsigned char *pucDes);

#endif


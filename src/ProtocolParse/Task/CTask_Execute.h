/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : CTask_Execute.h
 Description    : 曲线数据模块任务执行
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-09-03    azh     the original version
******************************************************************************/

#ifndef _CTASK_EXECUTE_H
#define _CTASK_EXECUTE_H

__EXTERN unsigned char Task_Proc(void);
__EXTERN unsigned char Task_RequireProc(unsigned char ucTaskNo);
//__EXTERN unsigned char Task_Resolve(STSENDDILIST *pstSendDIList,unsigned char ucTaskNo);
//__EXTERN unsigned char Task_RequireUp(unsigned char ucTaskNo);
//__EXTERN unsigned char Task_UpSendRecord(unsigned char ucTaskNo);
__EXTERN unsigned char Task_Proc_Relay(unsigned char ucTaskNo);
__EXTERN unsigned char Task_Proc_Normal(unsigned char ucTaskNo);

__EXTERN unsigned char Fill_TaskData(void);
//__EXTERN unsigned char Fill_MeterTask(unsigned short usDI,unsigned char ucTaskSour,unsigned char ucMPNo,unsigned char ucRS485TaskGroupListNo);
//__EXTERN unsigned char GetData_From_Ter_SumG(void);

//__EXTERN unsigned char Fill_TaskData_to_TerMsg(STTERMSG *pstTerMsg,unsigned char ucTaskNo,unsigned short usPos,unsigned char ucSendTaskType);
__EXTERN unsigned short Make_TaskRet_String(STGRTASKRETVAL *pstGRTaskRetVal,unsigned char	*pucGRTaskRetBuf);
//---------------------------------------------------------------
__EXTERN void TimTaskCmdRevcFrameByDi(STPROTOCOL_ANALYSE *pstProtocolAnalyse, unsigned char *rev_msg, unsigned char TimeOutFlag);
__EXTERN void TaskCmdRevcFrameByDi(STPROTOCOL_ANALYSE *pstProtocolAnalyse, unsigned char *rev_msg, unsigned char TimeOutFlag);
__EXTERN unsigned char TaskCmdSendHandle(unsigned char com, unsigned char *revmbx);
__EXTERN void Timer_FillRS485MeterDi(void);
#endif

/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Alarm_Record.h
 Description    : 告警日志相关处理
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/

#ifndef _ALARM_RECORD_H_
#define _ALARM_RECORD_H_

//__EXTERN unsigned char Alarm_GetAlrRecordList(void);
__EXTERN unsigned char AddToAlrRecordList(STALRENSUREJUDGE *pstAlrEnsureJudge,unsigned char ucUpFlag);
__EXTERN unsigned char Alarm_UpSend_Manage(unsigned char ucAlrType);
__EXTERN unsigned char Alarm_GetRecordMaxNum(unsigned char ucAlrType);
__EXTERN void Alarm_SaveManageInfo(unsigned char ucAlrType);
__EXTERN void Alarm_InitManageInfo(unsigned char ucFlag);
__EXTERN unsigned char Alarm_SaveRecordAuxInfo(unsigned char ucAlrRecordNo,STPERALRAUXINFO *pstAlarmAuxInfo,unsigned char ucAlrType);
__EXTERN unsigned char ClearAlrRecordAuxInfo(STPERALRAUXINFO *pstAlarmAuxInfo);
__EXTERN unsigned char Alarm_QueryRecordAuxInfo(unsigned char ucAlrRecordNo,STPERALRAUXINFO *pstAlarmAuxInfo,unsigned char ucAlrType);
#endif

/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Alarm_Manage_Meter.h
 Description    : 测量点告警数据处理
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2013-01-08    azh     the original version
******************************************************************************/

#ifndef _ALARM_MANAGE_METER_H_
#define _ALARM_MANAGE_METER_H_

__EXTERN unsigned char Alarm_ForeJudge_Meter(void);
__EXTERN unsigned char Alarm_EnsureJudge_Meter(unsigned char ucAlrForeNo);
__EXTERN unsigned char Alarm_GetForeJudgeInfo(unsigned short usAlrCode,unsigned char ucMPNo,STALRFOREJUDGE *pstAlrForeJudge);
__EXTERN unsigned char Alarm_GetEnsureJudgeInfo(unsigned short usAlrCode,unsigned char ucMPNo,STALRENSUREJUDGE *pstAlrEnsureJudge);

#endif

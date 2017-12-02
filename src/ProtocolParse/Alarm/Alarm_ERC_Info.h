/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Alarm_ERC_Info.c
 Description    : 告警数据转换成国网上行格式
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2013-01-08    azh     the original version
******************************************************************************/

#ifndef _ALARM_ERC_INFO_H_
#define _ALARM_ERC_INFO_H_

__EXTERN unsigned char Alarm_DataConvert(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);

#endif

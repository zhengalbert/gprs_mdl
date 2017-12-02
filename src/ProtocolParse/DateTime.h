/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : DateTime.h
 Description    : 时间管理程序
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/
#ifndef _DATETIME_H_
#define _DATETIME_H_

__EXTERN unsigned short ymd_into_day(unsigned short year,unsigned char month,unsigned char day);
__EXTERN void AddTime(STDATETIME *pstDateTime,unsigned long ulTimeInter,unsigned char ucTimeUnit);
__EXTERN STDATETIME day_into_ymd(unsigned short all_day);
//__EXTERN unsigned long Get_DateTime_Diff(STDATETIME	*pstDateTime0,STDATETIME	*pstDateTime1);
__EXTERN STDATETIME	GetCurrentTime(void);
__EXTERN unsigned char GetCurrentWeek(STDATETIME	*pstDateTime);
//__EXTERN unsigned char GetDateTime_From_Timer(void);
//__EXTERN unsigned long	SubDateTime(STDATETIME *pstLDateTime,STDATETIME	*pstRDateTime);
__EXTERN unsigned long	SubDayTime(STDATETIME*	pstLDateTime,STDATETIME* pstRDateTime);
__EXTERN void SubTime(STDATETIME *pstDateTime,unsigned long ulTimeInter,unsigned char ucTimeUnit);
__EXTERN unsigned long TimeConver(STDATETIME *pstDateTime,unsigned char ucTimeUnit);
__EXTERN STDATETIME TimeConverToDataTime(unsigned long ulTimeSecond);
__EXTERN unsigned long TimeConverToSecond(unsigned char ucTimeInter,unsigned char ucTimeUnit);
//__EXTERN unsigned long TimeInfoConver( unsigned char ucTimeInter,unsigned char ucTimeUnit);
__EXTERN unsigned char TimeIsOver(STDATETIME *pstWorkDateTime,STDATETIME *pstBaseDateTime);
__EXTERN unsigned char Is_DateTime_Availble(STDATETIME *pstDateTime);
__EXTERN void SetDateTime(STDATETIME	*pstDateTime);
//__EXTERN unsigned char	Is_DateTime_Equel(STDATETIME	*pstDateTime1,STDATETIME	*pstDateTime2);

#endif//_DATETIME_H_

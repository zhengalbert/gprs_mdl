/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : DateTime.c
 Description    : 时间管理程序
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

static const unsigned short days_table[52]=
{
 0,365,730,1096,1461,1826,2191,2557,2922,3287,3652,4018,4383,4748,5113,5479,5844,
 6209,6574,6940,7305,7670,8035,8401,8766,9131,9496,9862,10227,10592,10957,11323,
 11688,12053,12418,12784,13149,13514,13879,14245,14610,14975,15340,15706,16071,
 16436,16801,17167,17532,17897,18262,18628	
};


unsigned short ymd_into_day(unsigned short year,unsigned char month,unsigned char day)
{
	unsigned short allday;
	if( (year-1990)>=2 )
	{
		allday=365*2+(year-1992)*365+1+(year-1992)/4;
	}else
	{
		allday=(year-1990)*365;
	}
	switch(month)
	{
	case 1:
		break;
	case 2:
		allday+=31;
		break;
	case 3:
		allday+=31+28;
		break;
	case 4:
		allday+=31+28+31;
		break;
	case 5:
		allday+=31+28+31+30;
		break;
	case 6:
		allday+=31+28+31+30+31;
		break;
	case 7:
		allday+=31+28+31+30+31+30;
		break;
	case 8:
		allday+=31+28+31+30+31+30+31;
		break;
	case 9:
		allday+=31+28+31+30+31+30+31+31;
		break;
	case 10:
		allday+=31+28+31+30+31+30+31+31+30;
		break;
	case 11:	
		allday+=31+28+31+30+31+30+31+31+30+31;
		break;
	case 12:
		allday+=31+28+31+30+31+30+31+31+30+31+30;
		break;
	default:
		break;
	}
	if( (year%4)==0 && month<3 )	allday-=1;/*??????*/
	allday=allday+day;				/*相对(1990.1.1)*/
	return allday;
}

STDATETIME day_into_ymd(unsigned short all_day)
{
	unsigned char month=0,aday,i;
	unsigned short year=0,day=0;
	STDATETIME	stRetDateTime;

	for(i=0;i<52;i++)
	{
		if(all_day<=days_table[i]) break;
	}
	
	if(i>=1)
	{
		year=1990+i-1;
		day=all_day-days_table[i-1];
		if(year%4 != 0 || (year%100==0 && year%400!=0))
		{
			if(day<=31)month=1;
			else if(day<=31+28){month=2;day=day-31;}
			else if(day<=31+28+31){month=3;day=day-31-28;}
			else if(day<=31+28+31+30){month=4;day=day-31-28-31;}
			else if(day<=31+28+31+30+31){month=5;day=day-31-28-31-30;}
			else if(day<=31+28+31+30+31+30){month=6;day=day-31-28-31-30-31;}
			else if(day<=31+28+31+30+31+30+31){month=7;day=day-31-28-31-30-31-30;}
			else if(day<=31+28+31+30+31+30+31+31)
					{month=8;day=day-31-28-31-30-31-30-31;}
			else if(day<=31+28+31+30+31+30+31+31+30)
					{month=9;day=day-31-28-31-30-31-30-31-31;}
			else if(day<=31+28+31+30+31+30+31+31+30+31)
					{month=10;day=day-31-28-31-30-31-30-31-31-30;}
			else if(day<=31+28+31+30+31+30+31+31+30+31+30)
					{month=11;day=day-31-28-31-30-31-30-31-31-30-31;}
			else if(day<=31+28+31+30+31+30+31+31+30+31+30+31)
					{month=12;day=day-31-28-31-30-31-30-31-31-30-31-30;}
		}else 
		{
			if(day<=31)month=1;
			else if(day<=31+29){month=2;day=day-31;}
			else if(day<=31+29+31){month=3;day=day-31-29;}
			else if(day<=31+29+31+30){month=4;day=day-31-29-31;}
			else if(day<=31+29+31+30+31){month=5;day=day-31-29-31-30;}
			else if(day<=31+29+31+30+31+30){month=6;day=day-31-29-31-30-31;}
			else if(day<=31+29+31+30+31+30+31){month=7;day=day-31-29-31-30-31-30;}
			else if(day<=31+29+31+30+31+30+31+31)
					{month=8;day=day-31-29-31-30-31-30-31;}
			else if(day<=31+29+31+30+31+30+31+31+30)
					{month=9;day=day-31-29-31-30-31-30-31-31;}
			else if(day<=31+29+31+30+31+30+31+31+30+31)
					{month=10;day=day-31-29-31-30-31-30-31-31-30;}
			else if(day<=31+29+31+30+31+30+31+31+30+31+30)
					{month=11;day=day-31-29-31-30-31-30-31-31-30-31;}
			else if(day<=31+29+31+30+31+30+31+31+30+31+30+31)
					{month=12;day=day-31-29-31-30-31-30-31-31-30-31-30;}
		}
	}	
	aday=(unsigned char)(day);
	stRetDateTime.ucYear	=year%100;
	stRetDateTime.ucMonth	=month%100;
	stRetDateTime.ucDay		=aday%100;
	stRetDateTime.ucHour	=0;
	stRetDateTime.ucMinute	=0;
	stRetDateTime.ucSecond	=0;
	return stRetDateTime;
}

unsigned char Is_DateTime_Availble(STDATETIME *pstDateTime)
{
	unsigned char ucRet=RET_ERROR;

	//对于某些需量数据，年的信息有可能小于02
	if( pstDateTime->ucHour < 24 && 
		pstDateTime->ucMinute < 60 && 
		pstDateTime->ucSecond < 60 &&
		pstDateTime->ucDay < 32 && 
		pstDateTime->ucMonth < 13 &&  
		pstDateTime->ucYear < 80 &&
		(pstDateTime->ucDay != 0) &&
		(pstDateTime->ucMonth) != 0 
		
		)
		ucRet = RET_SUCCESS;

	return ucRet;
}

STDATETIME	GetCurrentTime(void)
{
    STDATETIME	stCurrTime;    
    
    GetDateTime_SoftRTC(&stCurrTime);
    return stCurrTime;
//azh
//	return gstCurrDateTime;
}

unsigned char GetCurrentWeek(STDATETIME	*pstDateTime)
{
	STDATETIME	stTmpDateTime;
	unsigned long	laDay;
	unsigned char	ucWeekNo=0;

	stTmpDateTime.ucYear	= 2;
	stTmpDateTime.ucMonth	= 4;
	stTmpDateTime.ucDay		= 1;
	stTmpDateTime.ucHour	= 0;
	stTmpDateTime.ucMinute	= 0;
	stTmpDateTime.ucSecond	= 0;		//02.4.1 周1
	laDay=SubDayTime(&stTmpDateTime,pstDateTime);
	ucWeekNo=(unsigned char)((laDay%7)+1);
//azh 376规定1~7:周一~周日
//	if(ucWeekNo == 7)
//		ucWeekNo =0;		//周日以0表示		

	return ucWeekNo;
}

void SetDateTime(STDATETIME	*pstDateTime)
{
	unsigned char ucCount=0;
	STDATETIME	stCurrTime={0,0,0,0,0,0};
	
	if( Is_DateTime_Availble(pstDateTime) == RET_SUCCESS )
	{
		while( pstDateTime->ucMinute!=stCurrTime.ucMinute || 
			   pstDateTime->ucHour!=pstDateTime->ucHour ||
			   pstDateTime->ucDay!=stCurrTime.ucDay || 
			   pstDateTime->ucMonth!=stCurrTime.ucMonth || 
			   pstDateTime->ucYear!=stCurrTime.ucYear )
		{
//azh
//			SetDateTime_RX8025(pstDateTime);
//			GetDateTime_RX8025(&stCurrTime);
#ifdef HARD_RTC
			SetDateTime_ToRTC(pstDateTime);
#else
			SetDateTime_SoftRTC(pstDateTime);
#endif
			ucCount++;
			if(ucCount>3)
				break;
		}
	}
//	if(ucCount<=3)
//		gstCurrDateTime=(*pstDateTime);
}

//unsigned char GetDateTime_From_Timer(void)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	STDATETIME stDateTime;
//	unsigned char ucCount=0;
//	
////	GetDateTime_RX8025(&stDateTime);
//    GetDateTime_SoftRTC(&stDateTime);
//	while(Is_DateTime_Availble(&stDateTime)!=RET_SUCCESS)
//	{
////		GetDateTime_RX8025(&stDateTime);
//        GetDateTime_SoftRTC(&stDateTime);
//		ucCount++;
//		if(ucCount>3)
//		{
//			ucRet=RET_ERROR;
//			break;
//		}
//	}
//	if(ucCount>3)
//	{
//		if(Is_DateTime_Availble(&gstCurrDateTime)==RET_SUCCESS)
//			//SetDateTime_RX8025(&gstCurrDateTime);
//			SetDateTime_SoftRTC(&gstCurrDateTime);
//		else
//		{
//			stDateTime.ucYear	=4;
//			stDateTime.ucMonth	=9;			
//			stDateTime.ucDay	=6;			
//			stDateTime.ucHour	=10;			
//			stDateTime.ucMinute	=30;			
//			stDateTime.ucSecond	=20;					
////			SetDateTime_RX8025(&stDateTime);
////azh
//			SetDateTime_SoftRTC(&stDateTime);
//
//			gstCurrDateTime=stDateTime;
//		}
//	}
//	else
//		gstCurrDateTime=stDateTime;
//	return ucRet;
//}
//
//unsigned long	SubDateTime(STDATETIME *pstLDateTime,STDATETIME	*pstRDateTime)
//{
//	unsigned long	lDayL,lDayR;
//	unsigned long	lMinL,lMinR;
//	unsigned long	lRet;
//	unsigned short	usTmp=0;
//	STDATETIME	stLDateTime,stRDateTime;
//	stLDateTime=(*pstLDateTime);
//	stRDateTime=(*pstRDateTime);
//	usTmp=(unsigned short)((stLDateTime.ucYear+2000)%0x10000);
//	lDayL=(ymd_into_day(usTmp,stLDateTime.ucMonth,stLDateTime.ucDay));
//	usTmp=(unsigned short)((stRDateTime.ucYear+2000)%0x10000);
//	lDayR=(ymd_into_day(usTmp,stRDateTime.ucMonth,stRDateTime.ucDay));
//	lMinL=stLDateTime.ucHour*60+stLDateTime.ucMinute;
//	lMinR=stRDateTime.ucHour*60+stRDateTime.ucMinute;
//	lDayL=lDayL*24*60+lMinL;
//	lDayR=lDayR*24*60+lMinR;
//	if(lDayR > lDayL)
//		lRet = lDayR - lDayL;
//	else
//		lRet = lDayL - lDayR;
//	return lRet;
//}

unsigned long	SubDayTime(STDATETIME*	pstLDateTime,STDATETIME* pstRDateTime)
{
	unsigned long	lDayL,lDayR;
	unsigned long	lRet;
	unsigned short usTmp=0;
	STDATETIME	stLDateTime,stRDateTime;
	stLDateTime=(*pstLDateTime);
	stRDateTime=(*pstRDateTime);
	usTmp=(unsigned short)((stLDateTime.ucYear+2000)%0x10000);
	lDayL=(ymd_into_day(usTmp,stLDateTime.ucMonth,stLDateTime.ucDay));
	usTmp=(unsigned short)((stRDateTime.ucYear+2000)%0x10000);
	lDayR=(ymd_into_day(usTmp,stRDateTime.ucMonth,stRDateTime.ucDay));
	if(lDayR > lDayL)
		lRet = lDayR - lDayL;
	else
		lRet = lDayL - lDayR;
	return lRet;
}

//unsigned long TimeInfoConver( unsigned char ucTimeInter,unsigned char ucTimeUnit)
//{
//	unsigned long ulaDay=0;
//	switch(ucTimeUnit)
//	{
//	case CON_TIMEUNIT_SEC://秒
//		ulaDay=ucTimeInter;
//		break;
//	case CON_TIMEUNIT_MIN://分
//		ulaDay=ucTimeInter*60;
//		break;
//	case CON_TIMEUNIT_HOUR://时
//		ulaDay=ucTimeInter*3600;
//		break;
//	case CON_TIMEUNIT_DAY://日
//		ulaDay=ucTimeInter*3600*24;
//		break;
//	default :
//		ulaDay=1;
//		break;
//	}
//	return ulaDay;
//}

unsigned long TimeConver(STDATETIME *pstDateTime,unsigned char ucTimeUnit)
{
	unsigned long ulaDay=0;
	unsigned short usTmp=0;
	usTmp=(unsigned short)((pstDateTime->ucYear+2000)%0x10000);
	
	ulaDay=ymd_into_day(usTmp,pstDateTime->ucMonth,pstDateTime->ucDay);
	switch(ucTimeUnit)
	{
	case CON_TIMEUNIT_SEC://秒
		ulaDay=ulaDay*24*60*60+pstDateTime->ucHour*60*60+pstDateTime->ucMinute*60+pstDateTime->ucSecond;
		break;
	case CON_TIMEUNIT_MIN://分
		ulaDay=ulaDay*24*60+pstDateTime->ucHour*60+pstDateTime->ucMinute;
		break;
	case CON_TIMEUNIT_HOUR://时
		ulaDay=ulaDay*24+pstDateTime->ucHour;
		break;
	case CON_TIMEUNIT_DAY://日
		break;
	case CON_TIMEUNIT_MON:
		ulaDay = (pstDateTime->ucYear+2000 - 1990) *12 + pstDateTime->ucMonth;
		break;
	default:	break;
	}
	return ulaDay;
}
//unsigned long Get_DateTime_Diff(STDATETIME	*pstDateTime0,STDATETIME	*pstDateTime1)
//{
//	unsigned long lTimeVal0=0,lTimeVal1=0;
//	unsigned long lTimeDiffVal=0;
//	lTimeVal0=TimeConver( pstDateTime0,CON_TIMEUNIT_MIN );
//	lTimeVal1=TimeConver( pstDateTime1,CON_TIMEUNIT_MIN );
//	
//	if( lTimeVal0>=lTimeVal1 )
//		lTimeDiffVal=lTimeVal0-lTimeVal1;
//	else
//		lTimeDiffVal=lTimeVal1-lTimeVal0;
//	
//	return lTimeDiffVal;
//}
unsigned long TimeConverToSecond(unsigned char ucTimeInter,unsigned char ucTimeUnit)
{
	unsigned long ulSecond=0;
	switch(ucTimeUnit)
	{
	case CON_TIMEUNIT_SEC://秒
		ulSecond=ucTimeInter;
		break;
	case CON_TIMEUNIT_MIN://分
		ulSecond=ucTimeInter*60;
		break;
	case CON_TIMEUNIT_HOUR://时
		ulSecond=ucTimeInter*3600;
		break;
	case CON_TIMEUNIT_DAY://日
		ulSecond=ucTimeInter*86400;
		break;
	default:	break;
	}
	return ulSecond;
}

//从1990年1月1日开始秒计数转换为时间
STDATETIME TimeConverToDataTime(unsigned long ulTimeSecond)
{
	unsigned long ulaDay=0,ulDayTmp=0;
	unsigned short usTmp=0;
	STDATETIME stDateTime;

	ulDayTmp=ulTimeSecond/(60*60*24);
	ulaDay=ulTimeSecond%(60*60*24);
	usTmp=(unsigned short)(ulDayTmp%0x10000);
	stDateTime=day_into_ymd(usTmp);
	stDateTime.ucHour=(unsigned char)(ulaDay/3600);
	ulaDay=ulaDay%3600;
	stDateTime.ucMinute=(unsigned char)(ulaDay/60);
	stDateTime.ucSecond=(unsigned char)(ulaDay%60);

	return stDateTime;
}

void AddTime(STDATETIME *pstDateTime,unsigned long ulTimeInter,unsigned char ucTimeUnit)
{
	unsigned long	ulaDay=0;
	unsigned short	usTmp=0;
	if(ucTimeUnit==CON_TIMEUNIT_MON)
	{
		pstDateTime->ucMonth++;
		if(pstDateTime->ucMonth>12)
		{
			pstDateTime->ucMonth=1;
			pstDateTime->ucYear++;
		}
	}else
	{
		usTmp=(unsigned short)((pstDateTime->ucYear+2000)%0x10000);
		ulaDay=ymd_into_day(usTmp,pstDateTime->ucMonth,pstDateTime->ucDay);
		ulaDay=ulaDay*24*60*60+pstDateTime->ucHour*60*60+pstDateTime->ucMinute*60+pstDateTime->ucSecond;
		switch(ucTimeUnit)
		{
		case CON_TIMEUNIT_SEC:
			ulaDay+=ulTimeInter;
			break;
		case CON_TIMEUNIT_MIN:
			ulaDay+=ulTimeInter*60;
			break;
		case CON_TIMEUNIT_HOUR:
			ulaDay+=ulTimeInter*60*60;
			break;
		case CON_TIMEUNIT_DAY:
			ulaDay+=ulTimeInter*24*60*60;
			break;
		default:	
			break;
		}
		(*pstDateTime)=TimeConverToDataTime(ulaDay);
	}
}

void SubTime(STDATETIME *pstDateTime,unsigned long ulTimeInter,unsigned char ucTimeUnit)
{
	unsigned long	ulaDay=0;
	unsigned short	usTmp=0;
	if(ucTimeUnit==CON_TIMEUNIT_MON)
	{
		pstDateTime->ucMonth--;
		if(pstDateTime->ucMonth==0)
		{
			pstDateTime->ucMonth=12;
			pstDateTime->ucYear--;
		}
	}else
	{
		usTmp=(unsigned short)((pstDateTime->ucYear+2000)%0x10000);
		ulaDay=ymd_into_day(usTmp,pstDateTime->ucMonth,pstDateTime->ucDay);
		ulaDay=ulaDay*24*60*60+pstDateTime->ucHour*60*60+pstDateTime->ucMinute*60+pstDateTime->ucSecond;
		switch(ucTimeUnit)
		{
		case CON_TIMEUNIT_SEC:
			ulaDay-=ulTimeInter;
			break;
		case CON_TIMEUNIT_MIN:
			ulaDay-=ulTimeInter*60;
			break;
		case CON_TIMEUNIT_HOUR:
			ulaDay-=ulTimeInter*60*60;
			break;
		case CON_TIMEUNIT_DAY:
			ulaDay-=ulTimeInter*24*60*60;
			break;
		default:	
			break;
		}
		(*pstDateTime)=TimeConverToDataTime(ulaDay);
	}
}

unsigned char TimeIsOver(STDATETIME *pstWorkDateTime,STDATETIME *pstBaseDateTime)
{
	unsigned char ucRet=RET_ERROR;
	unsigned long ulWorkTime=0,ulBaseTime=0;
	
	if( Is_DateTime_Availble(pstWorkDateTime)==RET_SUCCESS && 
		Is_DateTime_Availble(pstBaseDateTime)==RET_SUCCESS )
	{
		ulWorkTime=TimeConver(pstWorkDateTime,CON_TIMEUNIT_SEC);
		ulBaseTime=TimeConver(pstBaseDateTime,CON_TIMEUNIT_SEC);
		if(ulWorkTime>=ulBaseTime)
			ucRet=RET_SUCCESS;
	}
	return ucRet;
}

//unsigned char	Is_DateTime_Equel(STDATETIME	*pstDateTime1,STDATETIME	*pstDateTime2)
//{//两个时间是否相同
//
//	unsigned char ucRet=RET_ERROR;
//	if(	pstDateTime1->ucYear == pstDateTime2->ucYear  &&
//		pstDateTime1->ucMonth== pstDateTime2->ucMonth &&
//		pstDateTime1->ucDay  == pstDateTime2->ucDay   &&
//		pstDateTime1->ucHour == pstDateTime2->ucHour  &&
//		pstDateTime1->ucMinute == pstDateTime2->ucMinute &&
//		pstDateTime1->ucSecond == pstDateTime2->ucSecond )
//		ucRet=RET_SUCCESS;
//	return ucRet;
//}

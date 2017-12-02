/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_DataProc.c
 Description    : 数据处理：数据统计，数据转存
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-08-15    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

unsigned char	ucTimerDataProc1MinFlag;

static void DataProc_Adjust_Frosen_Index(STDAYFROZENLISTINDEX *pstIndex);
static void DataProc_Adjust_Day_Index(STDAYLISTINDEX *pstIndex);
static void DataProc_Adjust_Month_Index(STMONTHLISTINDEX *pstIndex);
static void save_daymonth_data_info(unsigned char ucMeterNo, unsigned char type, unsigned char maxnum);
static void init_daymonth_data_info(unsigned char ucMeterNo, unsigned char type, unsigned char id);
static void save_daymonth_lasttime(unsigned char type);
static void init_daymonth_lasttime(unsigned char type, unsigned char id);

static void init_day_month_data(unsigned char type);

//---------------------------------------------------------------------------------------------------------
void DataProc_Init_All(void)
{
	unsigned char i=0;
//	ClearDateTime(&g_stTime_DayFrozenData);
//	ClearDateTime(&g_stTime_DayFrozenCopy);	
//	g_stTime_DayData=GetCurrentTime();
//	g_stTime_MonthData=GetCurrentTime();
    init_daymonth_lasttime(CON_DAYDATA_TYPE, 0xa5);
    init_daymonth_lasttime(CON_MONTHDATA_TYPE, 0xa5);
    init_daymonth_lasttime(CON_FROZEDATA_TYPE, 0xa5);
    init_daymonth_lasttime(CON_FROZE_METERCOPY_TYPE, 0xa5);
    
	ucTimerDataProc1MinFlag=0xFF;
	for(i=0;i<CON_METER_NUM;i++)
	{
//		DataProc_Adjust_Frosen_Index(&(stHisRunDataIndex[i].stDayFrozenDataIndex));
//		DataProc_Adjust_Day_Index(&(stHisRunDataIndex[i].stDayDataIndex));
//		DataProc_Adjust_Month_Index(&(stHisRunDataIndex[i].stMonthDataIndex));
//azh
        init_daymonth_data_info(i, CON_DAYDATA_TYPE, 0xa5);//日冻结
        init_daymonth_data_info(i, CON_MONTHDATA_TYPE, 0xa5);//月冻结
        init_daymonth_data_info(i, CON_FROZEDATA_TYPE, 0xa5);//抄表日冻结
	}
	DataProc_DayData_Stat_Clear();
	DataProc_MonthData_Stat_Clear();

//	for(i=0;i<CON_SUMGROUP_NUM;i++)
//	{
//		//HdConvertWatchDog();
//		DataProc_Adjust_Day_Index(&(stHisSumGroupDataIndex[i].stSumGDayDataIndex));
//		DataProc_Adjust_Month_Index(&(stHisSumGroupDataIndex[i].stSumGMonthDataIndex));
//	}
//	DataProc_SumGDayData_Stat_Clear();
//	DataProc_SumGMonthData_Stat_Clear();

//	DataProc_Adjust_Frosen_Index(&(stHisTerDataIndex.stDayFrozenDataIndex));
//	DataProc_Adjust_Day_Index(&(stHisTerDataIndex.stTerDayDataIndex));
//	DataProc_Adjust_Month_Index(&(stHisTerDataIndex.stTerMonthDataIndex));
//	DataProc_TerDayData_Stat_Clear();
//	DataProc_TerMonthData_Stat_Clear();

//	for(i=0;i<CON_PULSE_NUM;i++)
//	{
//		DataProc_Adjust_Day_Index(&(stHisPulseDataIndex[i].stPulseDayDataIndex));
//		DataProc_Adjust_Month_Index(&(stHisPulseDataIndex[i].stPulseMonthDataIndex));
//	}
//	DataProc_PulseDayData_Stat_Clear();
//	DataProc_PulseMonthData_Stat_Clear();
//	//HdConvertWatchDog();
}

void DataProc_Init(void)
{
	unsigned char i=0;
	for(i=0;i<CON_METER_NUM;i++)
	{
//		if(stHisRunDataIndex[i].stDayFrozenDataIndex.ucHead>=CON_DAYFROZENDATA_NUM || stHisRunDataIndex[i].stDayFrozenDataIndex.ucTail>=CON_DAYFROZENDATA_NUM)
//			DataProc_Adjust_Frosen_Index(&(stHisRunDataIndex[i].stDayFrozenDataIndex));
//		if(stHisRunDataIndex[i].stDayDataIndex.ucHead>=CON_DAYDATA_NUM || stHisRunDataIndex[i].stDayDataIndex.ucTail>=CON_DAYDATA_NUM)
//			DataProc_Adjust_Day_Index(&(stHisRunDataIndex[i].stDayDataIndex));
//		if(stHisRunDataIndex[i].stMonthDataIndex.ucHead>=CON_MONTHDATA_NUM || stHisRunDataIndex[i].stMonthDataIndex.ucTail>=CON_MONTHDATA_NUM)
//			DataProc_Adjust_Month_Index(&(stHisRunDataIndex[i].stMonthDataIndex));
//azh
        init_daymonth_data_info(i, CON_DAYDATA_TYPE, 0);//日冻结
        init_daymonth_data_info(i, CON_MONTHDATA_TYPE, 0);//月冻结
        init_daymonth_data_info(i, CON_FROZEDATA_TYPE, 0);//抄表日冻结
	}
// 	for(i=0;i<CON_SUMGROUP_NUM;i++)
// 	{
// 		if(stHisSumGroupDataIndex[i].stSumGDayDataIndex.ucHead>=CON_DAYDATA_NUM || stHisSumGroupDataIndex[i].stSumGDayDataIndex.ucTail>=CON_DAYDATA_NUM)
// 			DataProc_Adjust_Day_Index(&(stHisSumGroupDataIndex[i].stSumGDayDataIndex));
// 		if(stHisSumGroupDataIndex[i].stSumGMonthDataIndex.ucHead>=CON_MONTHDATA_NUM || stHisSumGroupDataIndex[i].stSumGMonthDataIndex.ucTail>=CON_MONTHDATA_NUM)
// 			DataProc_Adjust_Month_Index(&(stHisSumGroupDataIndex[i].stSumGMonthDataIndex));
// 	}
													//终端数据
// 	if(stHisTerDataIndex.stDayFrozenDataIndex.ucHead>=CON_DAYFROZENDATA_NUM || stHisTerDataIndex.stDayFrozenDataIndex.ucTail>=CON_DAYFROZENDATA_NUM)
// 		DataProc_Adjust_Frosen_Index(&(stHisTerDataIndex.stDayFrozenDataIndex));
// 	if(stHisTerDataIndex.stTerDayDataIndex.ucHead>=CON_DAYDATA_NUM || stHisTerDataIndex.stTerDayDataIndex.ucTail>=CON_DAYDATA_NUM)
// 		DataProc_Adjust_Day_Index(&(stHisTerDataIndex.stTerDayDataIndex));
// 	if(stHisTerDataIndex.stTerMonthDataIndex.ucHead>=CON_MONTHDATA_NUM || stHisTerDataIndex.stTerMonthDataIndex.ucTail>=CON_MONTHDATA_NUM)
// 		DataProc_Adjust_Month_Index(&(stHisTerDataIndex.stTerMonthDataIndex));

//	for(i=0;i<CON_PULSE_NUM;i++)					//脉冲数据
//	{
//		if(stHisPulseDataIndex[i].stPulseDayDataIndex.ucHead>=CON_DAYDATA_NUM || stHisPulseDataIndex[i].stPulseDayDataIndex.ucTail>=CON_DAYDATA_NUM)
//			DataProc_Adjust_Day_Index(&(stHisPulseDataIndex[i].stPulseDayDataIndex));
//		if(stHisPulseDataIndex[i].stPulseMonthDataIndex.ucHead>=CON_MONTHDATA_NUM || stHisPulseDataIndex[i].stPulseMonthDataIndex.ucTail>=CON_MONTHDATA_NUM)
//			DataProc_Adjust_Month_Index(&(stHisPulseDataIndex[i].stPulseMonthDataIndex));
//	}
    init_daymonth_lasttime(CON_DAYDATA_TYPE, 0);//azh 初始化上次操作时间
    init_daymonth_lasttime(CON_MONTHDATA_TYPE, 0);
    init_daymonth_lasttime(CON_FROZEDATA_TYPE, 0);
    init_daymonth_lasttime(CON_FROZE_METERCOPY_TYPE, 0);
//azh 已保存当前 上电读出    
    init_day_month_data(CON_DAYDATA_TYPE);
    init_day_month_data(CON_MONTHDATA_TYPE);
//	DataProc_DayData_Stat_Clear();
//	DataProc_MonthData_Stat_Clear();    
}

void DataProc_Adjust_Frosen_Index(STDAYFROZENLISTINDEX *pstIndex)
{
	unsigned char j;
	pstIndex->ucHead=0;
	pstIndex->ucTail=0;
	for(j=0;j<CON_DAYFROZENDATA_NUM;j++)
		ClearDateTime(&(pstIndex->stDateTimeList[j]));
}

void DataProc_Adjust_Day_Index(STDAYLISTINDEX *pstIndex)
{
	unsigned char j;
	pstIndex->ucHead=0;
	pstIndex->ucTail=0;
	for(j=0;j<CON_DAYDATA_NUM;j++)
		ClearDateTime(&(pstIndex->stDateTimeList[j]));
}

void DataProc_Adjust_Month_Index(STMONTHLISTINDEX *pstIndex)
{
	unsigned char j;
	pstIndex->ucHead=0;
	pstIndex->ucTail=0;
	for(j=0;j<CON_MONTHDATA_NUM;j++)
		ClearDateTime(&(pstIndex->stDateTimeList[j]));
}

void DataProc_Timer(void)
{//日末或者月末计算，计算方法有月时间处理限制
	STDATETIME  stCurrentTime=GetCurrentTime();
	PSTFREEZEDAY pstFreezeDayPara;
	unsigned char ucMPNo=1;
	unsigned char i=0;
	if(stCurrentTime.ucMinute != ucTimerDataProc1MinFlag)
	{
		ucTimerDataProc1MinFlag=stCurrentTime.ucMinute;
		DataProc_DayData_Stat(stCurrentTime);							//更新日统计，每分钟处理

		DataProc_DayFrozenData_Stat(stCurrentTime);

//		DataProc_SumGDayData_Stat(stCurrentTime);
//		DataProc_TerDayData_Stat(stCurrentTime);						//终端

//		DataProc_TerDayFrozenData_Stat(stCurrentTime);
//		DataProc_PulseDayData_Stat(stCurrentTime);						//脉冲

		DataProc_MonthData_View_Stat(stCurrentTime);					//电量信息每分钟刷新一次

		DataProc_MonthData_FlashLast(stCurrentTime);						//更新月初数据

//		DataProc_SumGMonthData_FlashLast(stCurrentTime);
//		DataProc_TerMonthData_FlashLast(stCurrentTime);
//		DataProc_PulseMonthData_FlashLast(stCurrentTime);

	}
	
	pstFreezeDayPara=RunPara_GetMPFreezeDay(ucMPNo);
	for(i=0;i<3;i++)		//处理最多三个抄表日的日期，准备即时抄表
	{
//		//HdConvertWatchDog();
		if(Is_DateTime_Availble(&(pstFreezeDayPara->stFreezeDay[i]))==RET_SUCCESS)			//抄表日的时分是一致的
		{//取有效的抄表日
			if(pstFreezeDayPara->stFreezeDay[i].ucDay==stCurrentTime.ucDay)
			{//判断当天是否抄表日
				if(g_stTime_DayFrozenCopy.ucDay!=stCurrentTime.ucDay 
					|| g_stTime_DayFrozenCopy.ucMonth!=stCurrentTime.ucMonth)
				{//记录的不是本次抄表日，可以进行计算
					if(stCurrentTime.ucHour==pstFreezeDayPara->stFreezeDay[0].ucHour
						&& stCurrentTime.ucMinute==pstFreezeDayPara->stFreezeDay[0].ucMinute)
					{//抄表日数据处理
						g_stTime_DayFrozenCopy=stCurrentTime;
						DataProc_DayFrozenData_MeterCopy(g_stTime_DayFrozenCopy);		//即时抄表
						save_daymonth_lasttime(CON_FROZE_METERCOPY_TYPE);
					}
				}
			}
		}
	}
	
	for(i=0;i<3;i++)		//处理最多三个抄表日的日期，延迟1分钟，认为抄表已经完成，然后数据转存
	{
//		//HdConvertWatchDog();
		if(Is_DateTime_Availble(&(pstFreezeDayPara->stFreezeDay[i]))==RET_SUCCESS)			//抄表日的时分是一致的
		{//取有效的抄表日
			if(pstFreezeDayPara->stFreezeDay[i].ucDay==stCurrentTime.ucDay)
			{//判断当天是否抄表日
				if(g_stTime_DayFrozenData.ucDay!=stCurrentTime.ucDay 
					|| g_stTime_DayFrozenData.ucMonth!=stCurrentTime.ucMonth)
				{//记录的不是本次抄表日，可以进行计算
					if(stCurrentTime.ucHour==pstFreezeDayPara->stFreezeDay[0].ucHour
						&& stCurrentTime.ucMinute>=(pstFreezeDayPara->stFreezeDay[0].ucMinute+1)
						&& stCurrentTime.ucMinute<=(pstFreezeDayPara->stFreezeDay[0].ucMinute+CON_MINUTE_DIFF))
					{//抄表日数据处理
						g_stTime_DayFrozenData=stCurrentTime;
						DataProc_DayFrozenData_Move(g_stTime_DayFrozenData);		//数据转存
//						DataProc_TerDayFrozenData_Move(g_stTime_DayFrozenData);
                        save_daymonth_lasttime(CON_FROZEDATA_TYPE);
					}
				}
			}
		}
	}

	if(g_stTime_DayData.ucDay!=stCurrentTime.ucDay || g_stTime_DayData.ucMonth!=stCurrentTime.ucMonth)
	{//日数据处理
		g_stTime_DayData=stCurrentTime;
		DataProc_DayData_Move(g_stTime_DayData);
//		DataProc_SumGDayData_Move(g_stTime_DayData);
//		DataProc_TerDayData_Move(g_stTime_DayData);
//		DataProc_PulseDayData_Move(g_stTime_DayData);

		DataProc_MonthData_Stat(g_stTime_DayData);					//更新月统计，必须在日统计初始化之前
//		DataProc_SumGMonthData_Stat(g_stTime_DayData);
//		DataProc_TerMonthData_Stat(g_stTime_DayData);
//		DataProc_PulseMonthData_Stat(g_stTime_DayData);

		DataProc_DayData_Stat_Clear();								//初始化日统计
//		DataProc_SumGDayData_Stat_Clear();
//		DataProc_TerDayData_Stat_Clear();
//		DataProc_PulseDayData_Stat_Clear();

		DataProc_DayData_Stat(stCurrentTime);							//更新日统计
		DataProc_DayFrozenData_Stat(stCurrentTime);
//		DataProc_SumGDayData_Stat(stCurrentTime);
//		DataProc_TerDayData_Stat(stCurrentTime);						//终端
//		DataProc_TerDayFrozenData_Stat(stCurrentTime);
//		DataProc_PulseDayData_Stat(stCurrentTime);						//脉冲

		DataProc_Stat_FluxBytes_Inc(0);								//通信流量保护刷新
		save_daymonth_lasttime(CON_DAYDATA_TYPE);
	}
	if(g_stTime_MonthData.ucMonth!=stCurrentTime.ucMonth)
	{//月数据处理
		g_stTime_MonthData=stCurrentTime;
		DataProc_MonthData_Move(g_stTime_MonthData);
//		DataProc_SumGMonthData_Move(g_stTime_MonthData);
//		DataProc_TerMonthData_Move(g_stTime_MonthData);
//		DataProc_PulseMonthData_Move(g_stTime_MonthData);

		DataProc_MonthData_Stat_Clear();							//初始化月统计
//		DataProc_SumGMonthData_Stat_Clear();
//		DataProc_TerMonthData_Stat_Clear();
//		DataProc_PulseMonthData_Stat_Clear();

		DataProc_MonthData_Stat(g_stTime_MonthData);
//		DataProc_SumGMonthData_Stat(g_stTime_MonthData);
//		DataProc_TerMonthData_Stat(g_stTime_MonthData);
//		DataProc_PulseMonthData_Stat(g_stTime_MonthData);
        save_daymonth_lasttime(CON_MONTHDATA_TYPE);
	}
}

//--- 函数 ---
unsigned char DataProc_DayFrozenData_Move(STDATETIME stDateTime)
{//抄表日数据处理
	unsigned char ucRet=RET_SUCCESS,i=0;
	for(i=0;i<CON_METER_NUM;i++)
	{
		DataProc_Update_MeterData(i,stDateTime,&(stHisDayFrozenData[i]),CON_STATUS_NO);
		DataProc_SaveData_DayFrozen(i,stDateTime,&(stHisDayFrozenData[i]));
	}
	return ucRet;
}

//unsigned char DataProc_TerDayFrozenData_Move(STDATETIME stDateTime)
//{//终端抄表日数据处理
//	unsigned char ucRet=RET_SUCCESS;
//	DataProc_Update_TerData(stDateTime,&(stHisTerDayFrozenData),CON_STATUS_NO);
//	DataProc_SaveData_TerDayFrozen(stDateTime,&(stHisTerDayFrozenData));
//	return ucRet;
//}

unsigned char DataProc_DayData_Move(STDATETIME stDateTime)
{//日数据处理
	unsigned char ucRet=RET_SUCCESS,i=0;
	for(i=0;i<CON_METER_NUM;i++)
	{
		DataProc_Update_MeterData(i,stDateTime,&(stHisDayData[i]),CON_STATUS_NO);
		//HdConvertWatchDog();
		DataProc_SaveData_Day(i,stDateTime,&(stHisDayData[i]));
	}
	return ucRet;
}

unsigned char DataProc_MonthData_Move(STDATETIME stDateTime)
{//月数据处理
	unsigned char ucRet=RET_SUCCESS,i=0;
	for(i=0;i<CON_METER_NUM;i++)
	{
		DataProc_Update_MeterData(i,stDateTime,&(stHisMonthData[i]),CON_STATUS_NO);
		//HdConvertWatchDog();
		DataProc_SaveData_Month(i,stDateTime,&(stHisMonthData[i]));
	}
	return ucRet;
}

//unsigned char DataProc_SumGDayData_Move(STDATETIME stDateTime)
//{//总加日数据处理
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char i=0;
//	for(i=0;i<CON_SUMGROUP_NUM;i++)
//	{
//		SumGDataProc_Update_SumGData(i,stDateTime,&(stHisSumGroupThisDay.stHisSumGroupRunData[i]),CON_STATUS_NO);
//		//HdConvertWatchDog();
//		DataProc_SumGSaveData_Day(i,stDateTime,&(stHisSumGroupThisDay.stHisSumGroupRunData[i]));
//	}
//	return ucRet;
//}

//unsigned char DataProc_SumGMonthData_Move(STDATETIME stDateTime)
//{//总加月数据处理
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<CON_SUMGROUP_NUM;i++)
//	{
//		SumGDataProc_Update_SumGData(i,stDateTime,&(stHisSumGroupThisMonth.stHisSumGroupRunData[i]),CON_STATUS_NO);
//		//HdConvertWatchDog();
//		DataProc_SumGSaveData_Month(i,stDateTime,&(stHisSumGroupThisMonth.stHisSumGroupRunData[i]));
//	}
//	return ucRet;
//}

unsigned char DataProc_DayFrozenData_Stat(STDATETIME stDateTime)
{//冻结日数据统计
	unsigned char ucRet=RET_SUCCESS,i=0;
	for(i=0;i<CON_METER_NUM;i++)
	{
		DataProc_Update_MeterData(i,stDateTime,&(stHisDayFrozenData[i]),CON_STATUS_NO);
		//HdConvertWatchDog();
	}
	return ucRet;
}
//unsigned char DataProc_TerDayFrozenData_Stat(STDATETIME stDateTime)
//{//终端抄表日
//	unsigned char ucRet=RET_SUCCESS;
//	DataProc_Update_TerData(stDateTime,&(stHisTerDayFrozenData),CON_STATUS_NO);
//	return ucRet;
//}

unsigned char DataProc_DayData_Stat(STDATETIME stDateTime)
{//日数据统计
	unsigned char ucRet=RET_SUCCESS,i=0;
	for(i=0;i<CON_METER_NUM;i++)
	{
		//HdConvertWatchDog();
		DataProc_Update_MeterData(i,stDateTime,&(stHisDayData[i]),CON_STATUS_NO);

		DayData_Stat_InstantPow(i,stDateTime);
		DayData_Stat_Demand(i,stDateTime);
		DayData_Stat_VoltStat(i,stDateTime);
		DayData_Stat_Unbalance(i,stDateTime);
		DayData_Stat_CurrOver(i,stDateTime);
		DayData_Stat_SPowOver(i,stDateTime);
		//HdConvertWatchDog();
		DayData_Stat_VoltBreak(i,stDateTime);
		DayData_Stat_PowRate(i,stDateTime);
		DayData_Stat_TerRunState(i,stDateTime);
		DayData_Stat_CtrlNum(i,stDateTime);
	}
	return ucRet;
}

unsigned char DataProc_MonthData_Stat(STDATETIME stDateTime)
{//月数据统计
	unsigned char ucRet=RET_SUCCESS,i=0;
	for(i=0;i<CON_METER_NUM;i++)
	{
		//HdConvertWatchDog();
		DataProc_Update_MeterData(i,stDateTime,&(stHisMonthData[i]),CON_STATUS_NO);

		MonthData_Stat_InstantPow(i,stDateTime);
		MonthData_Stat_Demand(i,stDateTime);
		MonthData_Stat_VoltStat(i,stDateTime);
		MonthData_Stat_Unbalance(i,stDateTime);
		MonthData_Stat_CurrOver(i,stDateTime);
		//HdConvertWatchDog();
		MonthData_Stat_SPowOver(i,stDateTime);
		MonthData_Stat_VoltBreak(i,stDateTime);
		MonthData_Stat_PowRate(i,stDateTime);
		MonthData_Stat_TerRunState(i,stDateTime);
//		MonthData_Stat_CtrlNum(i,stDateTime);
	}
	return ucRet;
}

unsigned char DataProc_MonthData_View_Stat(STDATETIME stDateTime)
{//月示值数据统计
	unsigned char ucRet=RET_SUCCESS,i=0;
	for(i=0;i<CON_METER_NUM;i++)
	{
		DataProc_Update_MeterData(i,stDateTime,&(stHisMonthData[i]),CON_STATUS_NO);
		//HdConvertWatchDog();
	}
//终端交采
//	DataProc_Update_TerData(stDateTime,&stHisTerDataThisMonth,CON_STATUS_NO);
	//脉冲
//	for(i=0;i<2;i++)                //处理前面两路
//	{
//	       DataProc_Update_PulseData(i,stDateTime,&(stHisPulseDayDataThisMonth.stHisPulseRunData[i]),CON_STATUS_NO);
//	}

	return ucRet;
}

unsigned char DataProc_MonthData_FlashLast(STDATETIME stDateTime)
{//月数据统计
	unsigned char ucRet=RET_SUCCESS,i=0;
	STHISRUNDATA *pstHisRunData;
	unsigned char ucMPNo;
	STHISVAL stHisVal;
	for(i=0;i<CON_METER_NUM;i++)
	{
		ucMPNo=RunPara_GetMeter_MPNo(i);
		pstHisRunData=&(stHisMonthData[i]);

		if(pstHisRunData->ulHisLastDayDirPowTotal==CONINVALIDVAL)
		{//上一天的数据无效则刷新之
			stHisVal=Get_History_DirPower_byMPNo(ucMPNo,0,0);	pstHisRunData->ulHisLastDayDirPowTotal=stHisVal.lVal;
			stHisVal=Get_History_DirPower_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisLastDayDirPowFee1=stHisVal.lVal;
			stHisVal=Get_History_DirPower_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisLastDayDirPowFee2=stHisVal.lVal;
			stHisVal=Get_History_DirPower_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisLastDayDirPowFee3=stHisVal.lVal;
			stHisVal=Get_History_DirPower_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisLastDayDirPowFee4=stHisVal.lVal;
		}

		if(pstHisRunData->ulHisLastDayRevPowTotal==CONINVALIDVAL)
		{//上一天的数据无效则刷新之
			stHisVal=Get_History_UnDirPower_byMPNo(ucMPNo,0,0);	pstHisRunData->ulHisLastDayRevPowTotal=stHisVal.lVal;
			stHisVal=Get_History_UnDirPower_byMPNo(ucMPNo,0,1);	pstHisRunData->ulHisLastDayRevPowFee1=stHisVal.lVal;
			stHisVal=Get_History_UnDirPower_byMPNo(ucMPNo,0,2);	pstHisRunData->ulHisLastDayRevPowFee2=stHisVal.lVal;
			stHisVal=Get_History_UnDirPower_byMPNo(ucMPNo,0,3);	pstHisRunData->ulHisLastDayRevPowFee3=stHisVal.lVal;
			stHisVal=Get_History_UnDirPower_byMPNo(ucMPNo,0,4);	pstHisRunData->ulHisLastDayRevPowFee4=stHisVal.lVal;
		}

		if(pstHisRunData->ulHisLastDayDirVarTotal==CONINVALIDVAL)
		{//上一天的数据无效则刷新之
			stHisVal=Get_History_DirVar_byMPNo(ucMPNo,0,0);		pstHisRunData->ulHisLastDayDirVarTotal=stHisVal.lVal;
			stHisVal=Get_History_DirVar_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisLastDayDirVarFee1=stHisVal.lVal;
			stHisVal=Get_History_DirVar_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisLastDayDirVarFee2=stHisVal.lVal;
			stHisVal=Get_History_DirVar_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisLastDayDirVarFee3=stHisVal.lVal;
			stHisVal=Get_History_DirVar_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisLastDayDirVarFee4=stHisVal.lVal;
		}

		if(pstHisRunData->ulHisLastDayRevVarTotal==CONINVALIDVAL)
		{//上一天的数据无效则刷新之
			stHisVal=Get_History_UnDirVar_byMPNo(ucMPNo,0,0);	pstHisRunData->ulHisLastDayRevVarTotal=stHisVal.lVal;
			stHisVal=Get_History_UnDirVar_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisLastDayRevVarFee1=stHisVal.lVal;
			stHisVal=Get_History_UnDirVar_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisLastDayRevVarFee2=stHisVal.lVal;
			stHisVal=Get_History_UnDirVar_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisLastDayRevVarFee3=stHisVal.lVal;
			stHisVal=Get_History_UnDirVar_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisLastDayRevVarFee4=stHisVal.lVal;
		}
	}
	return ucRet;
}

//unsigned char DataProc_SumGDayData_Stat(STDATETIME stDateTime)
//{//总加日数据统计
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char i=0;
//	for(i=0;i<CON_SUMGROUP_NUM;i++)
//	{
//		//HdConvertWatchDog();
//		SumGDataProc_Update_SumGData(i,stDateTime,&(stHisSumGroupThisDay.stHisSumGroupRunData[i]),CON_STATUS_NO);
//		SumGDayData_Stat_InstantPow(i,stDateTime);
//		SumGDayData_Stat_Overflow(i,stDateTime);
//	}
//	return ucRet;
//}

//unsigned char DataProc_SumGMonthData_Stat(STDATETIME stDateTime)
//{//总加月数据统计
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<CON_SUMGROUP_NUM;i++)
//	{
//		//HdConvertWatchDog();
//		SumGDataProc_Update_SumGData(i,stDateTime,&(stHisSumGroupThisMonth.stHisSumGroupRunData[i]),CON_STATUS_NO);
//		SumGMonthData_Stat_InstantPow(i,stDateTime);
//		SumGMonthData_Stat_Overflow(i,stDateTime);
//	}
//	return ucRet;
//}

// unsigned char DataProc_SumGMonthData_FlashLast(STDATETIME stDateTime)
// {//总加月数据统计
// 	unsigned char ucRet=RET_SUCCESS,i=0;
// 	STHISSUMGROUPRUNDATA *pstHisSumGroupRunData;
// 	STHISSUMVALGROUP stHisSumValGroup;
// 	for(i=0;i<CON_SUMGROUP_NUM;i++)
// 	{
// 		pstHisSumGroupRunData=&(stHisSumGroupThisMonth.stHisSumGroupRunData[i]);
// 		if(pstHisSumGroupRunData->fSumGroupLastDayPowTotal==CONINVALIDVAL)
// 		{//上一天的数据无效则刷新之
// 			stHisSumValGroup=Get_History_PowValSumVal(0,0);		pstHisSumGroupRunData->fSumGroupLastDayPowTotal=stHisSumValGroup.stSumValGroup.fSumVal[i];;
// 			stHisSumValGroup=Get_History_PowValSumVal(0,1);		pstHisSumGroupRunData->fSumGroupLastDayPowFee1=stHisSumValGroup.stSumValGroup.fSumVal[i];
// 			stHisSumValGroup=Get_History_PowValSumVal(0,2);		pstHisSumGroupRunData->fSumGroupLastDayPowFee2=stHisSumValGroup.stSumValGroup.fSumVal[i];
// 			stHisSumValGroup=Get_History_PowValSumVal(0,3);		pstHisSumGroupRunData->fSumGroupLastDayPowFee3=stHisSumValGroup.stSumValGroup.fSumVal[i];
// 			stHisSumValGroup=Get_History_PowValSumVal(0,4);		pstHisSumGroupRunData->fSumGroupLastDayPowFee4=stHisSumValGroup.stSumValGroup.fSumVal[i];
// 		}

// 		if(pstHisSumGroupRunData->fSumGroupLastDayVarTotal==CONINVALIDVAL)
// 		{//上一天的数据无效则刷新之
// 			stHisSumValGroup=Get_History_VarValSumVal(0,0);		pstHisSumGroupRunData->fSumGroupLastDayVarTotal=stHisSumValGroup.stSumValGroup.fSumVal[i];
// 			stHisSumValGroup=Get_History_VarValSumVal(0,1);		pstHisSumGroupRunData->fSumGroupLastDayVarFee1=stHisSumValGroup.stSumValGroup.fSumVal[i];
// 			stHisSumValGroup=Get_History_VarValSumVal(0,2);		pstHisSumGroupRunData->fSumGroupLastDayVarFee2=stHisSumValGroup.stSumValGroup.fSumVal[i];
// 			stHisSumValGroup=Get_History_VarValSumVal(0,3);		pstHisSumGroupRunData->fSumGroupLastDayVarFee3=stHisSumValGroup.stSumValGroup.fSumVal[i];
// 			stHisSumValGroup=Get_History_VarValSumVal(0,4);		pstHisSumGroupRunData->fSumGroupLastDayVarFee4=stHisSumValGroup.stSumValGroup.fSumVal[i];
// 		}
// 	}
// 	return ucRet;
// }

unsigned char DataProc_DayData_Stat_Clear(void)
{//初始化日数据统计
	unsigned char ucRet=RET_SUCCESS,i=0;
	for(i=0;i<CON_METER_NUM;i++)
	{
		//HdConvertWatchDog();
		DataProc_Stat_Clear_MPData(&(stHisDayData[i]));
		DataProc_Stat_Clear_InstantPow(&(stHisDayData[i]));
		DataProc_Stat_Clear_Demand(&(stHisDayData[i]));
		DataProc_Stat_Clear_VoltStat(&(stHisDayData[i]));
		DataProc_Stat_Clear_Unbalance(&(stHisDayData[i]));
		DataProc_Stat_Clear_CurrOver(&(stHisDayData[i]));
		DataProc_Stat_Clear_SPowOver(&(stHisDayData[i]));
		//HdConvertWatchDog();
		DataProc_Stat_Clear_VoltBreak(&(stHisDayData[i]));
		DataProc_Stat_Clear_PowRate(&(stHisDayData[i]));
		DataProc_Stat_Clear_TerRunState(&(stHisDayData[i]));
		DataProc_Stat_Clear_CtrlNum(&(stHisDayData[i]));
	}
	return ucRet;
}

unsigned char DataProc_MonthData_Stat_Clear(void)
{//初始化月数据统计
	unsigned char ucRet=RET_SUCCESS,i=0;
	for(i=0;i<CON_METER_NUM;i++)
	{
		//HdConvertWatchDog();
		DataProc_Stat_Clear_MPData(&(stHisMonthData[i]));
		DataProc_Stat_Clear_InstantPow(&(stHisMonthData[i]));
		DataProc_Stat_Clear_Demand(&(stHisMonthData[i]));
		DataProc_Stat_Clear_VoltStat(&(stHisMonthData[i]));
		DataProc_Stat_Clear_Unbalance(&(stHisMonthData[i]));
		DataProc_Stat_Clear_CurrOver(&(stHisMonthData[i]));
		//HdConvertWatchDog();
		DataProc_Stat_Clear_SPowOver(&(stHisMonthData[i]));
		DataProc_Stat_Clear_VoltBreak(&(stHisMonthData[i]));
		DataProc_Stat_Clear_PowRate(&(stHisMonthData[i]));
		DataProc_Stat_Clear_TerRunState(&(stHisMonthData[i]));
		DataProc_Stat_Clear_CtrlNum(&(stHisMonthData[i]));
	}
	return ucRet;
}

//unsigned char DataProc_SumGDayData_Stat_Clear(void)
//{//初始化总加日数据统计
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char i=0;
//	STHISSUMGROUPRUNDATA *pstHisSumGroupRunData;
//	for(i=0;i<CON_SUMGROUP_NUM;i++)
//	{
//		//HdConvertWatchDog();
//		pstHisSumGroupRunData=&(stHisSumGroupThisDay.stHisSumGroupRunData[i]);
//		pstHisSumGroupRunData->fSumGroupPowTotal=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupPowFee1=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupPowFee2=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupPowFee3=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupPowFee4=CONINVALIDVAL;
//
//		pstHisSumGroupRunData->fSumGroupVarTotal=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupVarFee1=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupVarFee2=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupVarFee3=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupVarFee4=CONINVALIDVAL;
//
//		pstHisSumGroupRunData->fSumGroupLastDayPowTotal=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupLastDayPowFee1=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupLastDayPowFee2=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupLastDayPowFee3=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupLastDayPowFee4=CONINVALIDVAL;
//
//		pstHisSumGroupRunData->fSumGroupLastDayVarTotal=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupLastDayVarFee1=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupLastDayVarFee2=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupLastDayVarFee3=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupLastDayVarFee4=CONINVALIDVAL;
//
//		SumGDataProc_Stat_Clear_InstantPow(pstHisSumGroupRunData);
//		SumGDataProc_Stat_Clear_Overflow(pstHisSumGroupRunData);
//	}
//	return ucRet;
//}
//unsigned char DataProc_SumGMonthData_Stat_Clear(void)
//{//初始化总加月数据统计
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	STHISSUMGROUPRUNDATA *pstHisSumGroupRunData;
//	for(i=0;i<CON_SUMGROUP_NUM;i++)
//	{
//		//HdConvertWatchDog();
//		pstHisSumGroupRunData=&(stHisSumGroupThisMonth.stHisSumGroupRunData[i]);
//		pstHisSumGroupRunData->fSumGroupPowTotal=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupPowFee1=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupPowFee2=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupPowFee3=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupPowFee4=CONINVALIDVAL;
//
//		pstHisSumGroupRunData->fSumGroupVarTotal=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupVarFee1=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupVarFee2=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupVarFee3=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupVarFee4=CONINVALIDVAL;
//
//		pstHisSumGroupRunData->fSumGroupLastDayPowTotal=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupLastDayPowFee1=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupLastDayPowFee2=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupLastDayPowFee3=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupLastDayPowFee4=CONINVALIDVAL;
//
//		pstHisSumGroupRunData->fSumGroupLastDayVarTotal=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupLastDayVarFee1=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupLastDayVarFee2=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupLastDayVarFee3=CONINVALIDVAL;
//		pstHisSumGroupRunData->fSumGroupLastDayVarFee4=CONINVALIDVAL;
//
//		SumGDataProc_Stat_Clear_InstantPow(pstHisSumGroupRunData);
//		SumGDataProc_Stat_Clear_Overflow(pstHisSumGroupRunData);		
//	}
//	return ucRet;
//}

//unsigned char DataProc_SaveData_TerDayFrozen(STDATETIME stDateTime,STHISRUNDATA *pstHisRunData)
//{//保存终端抄表日数据
//	unsigned char ucRet=RET_SUCCESS;
// 	unsigned char ucPos=0xFF,i;
// 	unsigned short usBeginPage,usBeginPos;
//     unsigned long ulDataAddr;
// 	unsigned long ulDataTime,ulQueryTime;
// 	STDAYFROZENLISTINDEX *pstDataIndex=NULL;
// 	if(DP_Get_TerDP_State()==CON_TERDP_STATE_DOWNLOAD)			//数据块用于程序下装数据存储
// 		return ucRet;
// 	pstDataIndex=&(stHisTerDataIndex.stDayFrozenDataIndex);
// 	if(pstDataIndex->ucHead != pstDataIndex->ucTail)
// 	{//非空
// 		for(i=0;i<CON_DAYFROZENDATA_NUM;i++)
// 		{
// 			ucPos=(pstDataIndex->ucTail+i) % CON_DAYFROZENDATA_NUM;
// 			if(ucPos==pstDataIndex->ucHead)
// 			{
// 				ucPos=0xFF;
// 				break;
// 			}
// 			ulDataTime=TimeConver(&(pstDataIndex->stDateTimeList[ucPos]),CON_TIMEUNIT_SEC);
// 			ulQueryTime=TimeConver(&stDateTime,CON_TIMEUNIT_SEC);
// 			if((ulQueryTime<=ulDataTime && ulDataTime<ulQueryTime+3600) || (ulQueryTime>=ulDataTime && ulQueryTime<ulDataTime+3600))
// 				break;
// 		}
// 	}
// 	if(ucPos<CON_DAYFROZENDATA_NUM)
// 	{//更新
// 		ulDataAddr	=sizeof(STHISRUNDATA)*ucPos;
// 	}else
// 	{//增加
// 		ulDataAddr	=sizeof(STHISRUNDATA)*pstDataIndex->ucHead;
// 		//更新索引
// 		pstDataIndex->stDateTimeList[pstDataIndex->ucHead]=stDateTime;
// 		pstDataIndex->ucHead++;
// 		if(pstDataIndex->ucHead>=CON_DAYFROZENDATA_NUM)
// 			pstDataIndex->ucHead=0;
// 		if(pstDataIndex->ucHead==pstDataIndex->ucTail)
// 			pstDataIndex->ucTail++;
// 		if(pstDataIndex->ucTail>=CON_DAYFROZENDATA_NUM)
// 			pstDataIndex->ucTail=0;
// 	}
// 	//计算位置
//  	usBeginPage	= CON_DFPAGE_HEAD_TER_HISDATA;
// 	usBeginPage	=usBeginPage+(unsigned short)(ulDataAddr/FPAGE_SIZE);
// 	usBeginPos	=(unsigned short)(ulDataAddr % FPAGE_SIZE);
// 	ucRet=Flash_SetMainMemory((unsigned char *)pstHisRunData,sizeof(STHISRUNDATA),usBeginPage,usBeginPos);
// 	if(ucRet==EF_NORMAL)
// 		ucRet=RET_SUCCESS;
// 	else
// 		ucRet=RET_ERROR;
//	return ucRet;
//}

//unsigned char DataProc_SaveData_Day(unsigned char ucMeterNo,STDATETIME stDateTime,STHISRUNDATA *pstHisRunData)
//{//保存日数据
//	unsigned char ucRet=RET_SUCCESS;
// 	unsigned char ucPos=0xFF,i;
// 	unsigned short usBeginPage,usBeginPos;
//     unsigned long ulDataAddr;
// 	unsigned long ulDataTime,ulQueryTime;
// 	STDATETIME stTempTime;
// 	unsigned char ucMPNo,ucMPType;
// 	STDAYLISTINDEX *pstDataIndex=NULL;
// 	if(ucMeterNo>=CON_METER_NUM)
// 		return RET_ERROR;
// 	ucMPNo=RunPara_GetMeter_MPNo(ucMeterNo);
// 	ucMPType=RunPara_GetMPType(ucMPNo);
// 	if(ucMPType!=CON_MPTYPE_METER)
// 		return RET_ERROR;
//// 	if(DP_Get_TerDP_State()==CON_TERDP_STATE_DOWNLOAD)			//数据块用于程序下装数据存储
//// 		return ucRet;
// 	stTempTime=stDateTime;						//stDateTime为日初或者日期变化后的时间，数据要作为上一天的统计结果
// 	stTempTime.ucHour=0;
// 	stTempTime.ucMinute=0;
// 	stTempTime.ucSecond=0;
// 	pstDataIndex=&(stHisRunDataIndex[ucMeterNo].stDayDataIndex);
// 	if(pstDataIndex->ucHead != pstDataIndex->ucTail)
// 	{//非空
// 		for(i=0;i<CON_DAYDATA_NUM;i++)
// 		{
// 			ucPos=(pstDataIndex->ucTail+i) % CON_DAYDATA_NUM;
// 			if(ucPos==pstDataIndex->ucHead)
// 			{
// 				ucPos=0xFF;
// 				break;
// 			}
// 			ulDataTime=TimeConver(&(pstDataIndex->stDateTimeList[ucPos]),CON_TIMEUNIT_SEC);
// 			ulQueryTime=TimeConver(&stTempTime,CON_TIMEUNIT_SEC);
// 			if((ulQueryTime<=ulDataTime && ulDataTime<ulQueryTime+3600) || (ulQueryTime>=ulDataTime && ulQueryTime<ulDataTime+3600))
// 				break;
// 		}
// 	}
// 	if(ucPos<CON_DAYDATA_NUM)
// 	{//更新
// 		ulDataAddr	=sizeof(STHISRUNDATA)*(ucPos+CON_DAYFROZENDATA_NUM);
// 	}else
// 	{//增加
// 		ulDataAddr	=sizeof(STHISRUNDATA)*(pstDataIndex->ucHead+CON_DAYFROZENDATA_NUM);
// 		//更新索引
// 		pstDataIndex->stDateTimeList[pstDataIndex->ucHead]=stTempTime;
// 		pstDataIndex->ucHead++;
// 		if(pstDataIndex->ucHead>=CON_DAYDATA_NUM)
// 			pstDataIndex->ucHead=0;
// 		if(pstDataIndex->ucHead==pstDataIndex->ucTail)
// 			pstDataIndex->ucTail++;
// 		if(pstDataIndex->ucTail>=CON_DAYDATA_NUM)
// 			pstDataIndex->ucTail=0;
// 	}
// 	//计算位置
//  	usBeginPage	= CON_DFPAGE_HEAD_HISDATA + CON_DFPAGE_HISDATA_PAGENUM*ucMeterNo;
// 	usBeginPage	=usBeginPage+(unsigned short)(ulDataAddr/FPAGE_SIZE);
// 	usBeginPos	=(unsigned short)(ulDataAddr % FPAGE_SIZE);
// 	ucRet=Flash_SetMainMemory((unsigned char *)pstHisRunData,sizeof(STHISRUNDATA),usBeginPage,usBeginPos);
// 	if(ucRet==EF_NORMAL)
// 		ucRet=RET_SUCCESS;
// 	else
// 		ucRet=RET_ERROR;
//	return ucRet;
//}
//azh
unsigned char DataProc_SaveData_Day(unsigned char ucMeterNo,STDATETIME stDateTime,STHISRUNDATA *pstHisRunData)
{//保存日数据
	unsigned char ucRet=RET_SUCCESS;
 	unsigned char ucPos=0xFF,i;
// 	unsigned short usBeginPage,usBeginPos;
    unsigned short uspointnum_perblock,uspoint_totnum,blockpos;

     unsigned long ulDataAddr;
 	unsigned long ulDataTime,ulQueryTime;
 	STDATETIME stTempTime;
 	unsigned char ucMPNo,ucMPType;
 	STDAYLISTINDEX *pstDataIndex=NULL;
 	
 	if(ucMeterNo>=CON_METER_NUM)
 		return RET_ERROR;
 	ucMPNo=RunPara_GetMeter_MPNo(ucMeterNo);
 	ucMPType=RunPara_GetMPType(ucMPNo);
 	if(ucMPType!=CON_MPTYPE_METER)
 		return RET_ERROR;
// 	if(DP_Get_TerDP_State()==CON_TERDP_STATE_DOWNLOAD)			//数据块用于程序下装数据存储
// 		return ucRet;
 	stTempTime=stDateTime;						//stDateTime为日初或者日期变化后的时间，数据要作为上一天的统计结果
 	stTempTime.ucHour=0;
 	stTempTime.ucMinute=0;
 	stTempTime.ucSecond=0;
 	pstDataIndex=&(stHisRunDataIndex[ucMeterNo].stDayDataIndex);

    uspointnum_perblock = MCP_FLASH_BLOCK_SIZE/sizeof(STHISRUNDATA);
    uspoint_totnum = CON_DAYDATA_BLOCKNUM_PER_MP*uspointnum_perblock;

 	if(pstDataIndex->ucHead != pstDataIndex->ucTail)
 	{//非空
 		for(i=0;i<uspoint_totnum;i++)
 		{
 			ucPos=(pstDataIndex->ucTail+i) % uspoint_totnum;
 			if(ucPos==pstDataIndex->ucHead)
 			{
 				ucPos=0xFF;
 				break;
 			}
 			ulDataTime=TimeConver(&(pstDataIndex->stDateTimeList[ucPos]),CON_TIMEUNIT_SEC);
 			ulQueryTime=TimeConver(&stTempTime,CON_TIMEUNIT_SEC);
 			if((ulQueryTime<=ulDataTime && ulDataTime<ulQueryTime+3600) || (ulQueryTime>=ulDataTime && ulQueryTime<ulDataTime+3600))
 				break;
 		}
 	}
 	if(ucPos<uspoint_totnum)
 	{//更新
// 		ulDataAddr	=sizeof(STHISRUNDATA)*(ucPos+CON_DAYFROZENDATA_NUM);//azh 这个时间保存过 就不更新保存了 因为要擦除才行
 	}else
 	{//增加
        if(ucMPNo < CON_MAX_MPNUM)
        {    
            blockpos = CON_DFPAGE_HEAD_HISDATA + ucMPNo*(CON_DAYDATA_BLOCKNUM_PER_MP+CON_MONTHDATA_BLOCKNUM_PER_MP+CON_FROZEDATA_BLOCKNUM_PER_MP);
//            blockpos += 0;//日数据、月数据、结算日数据偏移
            ulDataAddr = blockpos + pstDataIndex->ucHead/uspointnum_perblock;
            ulDataAddr *= MCP_FLASH_BLOCK_SIZE;

            ulDataAddr += sizeof(STHISRUNDATA)*(pstDataIndex->ucHead%uspointnum_perblock);

        	ucRet = MCP_Flash_Write(ulDataAddr,(unsigned char *)pstHisRunData,sizeof(STHISRUNDATA));	
         	if(ucRet==NO_ERROR)
         		ucRet=RET_SUCCESS;
         	else
         		ucRet=RET_ERROR;

    		//更新索引
     		pstDataIndex->stDateTimeList[pstDataIndex->ucHead]=stTempTime;
     		pstDataIndex->ucHead++;
     		if(pstDataIndex->ucHead>=uspoint_totnum)
     		{    
     			pstDataIndex->ucHead=0;
     		}
            if(pstDataIndex->ucHead%uspointnum_perblock == 0)
            {
                blockpos += pstDataIndex->ucHead/uspointnum_perblock;
                ulDataAddr = blockpos*MCP_FLASH_BLOCK_SIZE;
                MCP_Flash_Sector_Erase(ulDataAddr);

         		if(pstDataIndex->ucHead==pstDataIndex->ucTail)
         			pstDataIndex->ucTail += uspointnum_perblock;
         		if(pstDataIndex->ucTail>=uspoint_totnum)
         			pstDataIndex->ucTail=0;
            }      
            save_daymonth_data_info(ucMeterNo, CON_DAYDATA_TYPE, uspoint_totnum);//azh 还得把索引保存到EEPROM
 		}
 	}
	return ucRet;
}

//unsigned char DataProc_SaveData_Month(unsigned char ucMeterNo,STDATETIME stDateTime,STHISRUNDATA *pstHisRunData)
//{//保存月数据
//	unsigned char ucRet=RET_SUCCESS;
// 	unsigned char ucPos=0xFF,i;
// 	unsigned short usBeginPage,usBeginPos;
//     unsigned long ulDataAddr;
// 	unsigned long ulDataTime,ulQueryTime;
// 	STDATETIME stTempTime;
// 	unsigned char ucMPNo,ucMPType;
// 	STMONTHLISTINDEX *pstDataIndex=NULL;
// 	if(ucMeterNo>=CON_METER_NUM)
// 		return RET_ERROR;
// 	ucMPNo=RunPara_GetMeter_MPNo(ucMeterNo);
// 	ucMPType=RunPara_GetMPType(ucMPNo);
// 	if(ucMPType!=CON_MPTYPE_METER)
// 		return RET_ERROR;
// 	if(DP_Get_TerDP_State()==CON_TERDP_STATE_DOWNLOAD)			//数据块用于程序下装数据存储
// 		return ucRet;
// 	stTempTime=stDateTime;
// 	stTempTime.ucDay=1;
// 	stTempTime.ucHour=0;
// 	stTempTime.ucMinute=0;
// 	stTempTime.ucSecond=0;
// 	pstDataIndex=&(stHisRunDataIndex[ucMeterNo].stMonthDataIndex);
// 	if(pstDataIndex->ucHead != pstDataIndex->ucTail)
// 	{//非空
// 		for(i=0;i<CON_MONTHDATA_NUM;i++)
// 		{
// 			ucPos=(pstDataIndex->ucTail+i) % CON_MONTHDATA_NUM;
// 			if(ucPos==pstDataIndex->ucHead)
// 			{
// 				ucPos=0xFF;
// 				break;
// 			}
// 			ulDataTime=TimeConver(&(pstDataIndex->stDateTimeList[ucPos]),CON_TIMEUNIT_SEC);
// 			ulQueryTime=TimeConver(&stTempTime,CON_TIMEUNIT_SEC);
// 			if((ulQueryTime<=ulDataTime && ulDataTime<ulQueryTime+432000) || (ulQueryTime>=ulDataTime && ulQueryTime<ulDataTime+432000))	//偏差5天，考虑月末28日~31日
// 				break;
// 		}
// 	}
// 	if(ucPos<CON_MONTHDATA_NUM)
// 	{//更新
// 		ulDataAddr	=sizeof(STHISRUNDATA)*(ucPos+CON_DAYFROZENDATA_NUM+CON_DAYDATA_NUM);
// 	}else
// 	{//增加
// 		ulDataAddr	=sizeof(STHISRUNDATA)*(pstDataIndex->ucHead+CON_DAYFROZENDATA_NUM+CON_DAYDATA_NUM);
// 		//更新索引
// 		pstDataIndex->stDateTimeList[pstDataIndex->ucHead]=stTempTime;
// 		pstDataIndex->ucHead++;
// 		if(pstDataIndex->ucHead>=CON_MONTHDATA_NUM)
// 			pstDataIndex->ucHead=0;
// 		if(pstDataIndex->ucHead==pstDataIndex->ucTail)
// 			pstDataIndex->ucTail++;
// 		if(pstDataIndex->ucTail>=CON_MONTHDATA_NUM)
// 			pstDataIndex->ucTail=0;
// 	}
// 	//计算位置
//  	usBeginPage	= CON_DFPAGE_HEAD_HISDATA + CON_DFPAGE_HISDATA_PAGENUM*ucMeterNo;
// 	usBeginPage	=usBeginPage+(unsigned short)(ulDataAddr/FPAGE_SIZE);
// 	usBeginPos	=(unsigned short)(ulDataAddr % FPAGE_SIZE);
// 	ucRet=Flash_SetMainMemory((unsigned char *)pstHisRunData,sizeof(STHISRUNDATA),usBeginPage,usBeginPos);
// 	if(ucRet==EF_NORMAL)
// 		ucRet=RET_SUCCESS;
// 	else
// 		ucRet=RET_ERROR;
//	return ucRet;
//}
unsigned char DataProc_SaveData_Month(unsigned char ucMeterNo,STDATETIME stDateTime,STHISRUNDATA *pstHisRunData)
{//保存月数据
	unsigned char ucRet=RET_SUCCESS;
 	unsigned char ucPos=0xFF,i;
// 	unsigned short usBeginPage,usBeginPos;
    unsigned short uspointnum_perblock,uspoint_totnum,blockpos;

     unsigned long ulDataAddr;
 	unsigned long ulDataTime,ulQueryTime;
 	STDATETIME stTempTime;
 	unsigned char ucMPNo,ucMPType;
 	STMONTHLISTINDEX *pstDataIndex=NULL;
 	
 	if(ucMeterNo>=CON_METER_NUM)
 		return RET_ERROR;
 	ucMPNo=RunPara_GetMeter_MPNo(ucMeterNo);
 	ucMPType=RunPara_GetMPType(ucMPNo);
 	if(ucMPType!=CON_MPTYPE_METER)
 		return RET_ERROR;
// 	if(DP_Get_TerDP_State()==CON_TERDP_STATE_DOWNLOAD)			//数据块用于程序下装数据存储
// 		return ucRet;
 	stTempTime=stDateTime;						//stDateTime为日初或者日期变化后的时间，数据要作为上一天的统计结果
 	stTempTime.ucHour=0;
 	stTempTime.ucMinute=0;
 	stTempTime.ucSecond=0;
 	pstDataIndex=&(stHisRunDataIndex[ucMeterNo].stMonthDataIndex);

    uspointnum_perblock = MCP_FLASH_BLOCK_SIZE/sizeof(STHISRUNDATA);
    uspoint_totnum = CON_MONTHDATA_BLOCKNUM_PER_MP*uspointnum_perblock;

 	if(pstDataIndex->ucHead != pstDataIndex->ucTail)
 	{//非空
 		for(i=0;i<uspoint_totnum;i++)
 		{
 			ucPos=(pstDataIndex->ucTail+i) % uspoint_totnum;
 			if(ucPos==pstDataIndex->ucHead)
 			{
 				ucPos=0xFF;
 				break;
 			}
 			ulDataTime=TimeConver(&(pstDataIndex->stDateTimeList[ucPos]),CON_TIMEUNIT_SEC);
 			ulQueryTime=TimeConver(&stTempTime,CON_TIMEUNIT_SEC);
 			if((ulQueryTime<=ulDataTime && ulDataTime<ulQueryTime+3600) || (ulQueryTime>=ulDataTime && ulQueryTime<ulDataTime+3600))
 				break;
 		}
 	}
 	if(ucPos<uspoint_totnum)
 	{//更新
// 		ulDataAddr	=sizeof(STHISRUNDATA)*(ucPos+CON_DAYFROZENDATA_NUM);//azh 这个时间保存过 就不更新保存了 因为要擦除才行
 	}else
 	{//增加
        if(ucMPNo < CON_MAX_MPNUM)
        {    
            blockpos = CON_DFPAGE_HEAD_HISDATA + ucMPNo*(CON_DAYDATA_BLOCKNUM_PER_MP+CON_MONTHDATA_BLOCKNUM_PER_MP+CON_FROZEDATA_BLOCKNUM_PER_MP);
            blockpos += CON_DAYDATA_BLOCKNUM_PER_MP;//日数据、月数据、结算日数据偏移
            ulDataAddr = blockpos + pstDataIndex->ucHead/uspointnum_perblock;
            ulDataAddr *= MCP_FLASH_BLOCK_SIZE;

            ulDataAddr += sizeof(STHISRUNDATA)*(pstDataIndex->ucHead%uspointnum_perblock);

        	ucRet = MCP_Flash_Write(ulDataAddr,(unsigned char *)pstHisRunData,sizeof(STHISRUNDATA));	
         	if(ucRet==NO_ERROR)
         		ucRet=RET_SUCCESS;
         	else
         		ucRet=RET_ERROR;

    		//更新索引
     		pstDataIndex->stDateTimeList[pstDataIndex->ucHead]=stTempTime;
     		pstDataIndex->ucHead++;
     		if(pstDataIndex->ucHead>=uspoint_totnum)
     		{    
     			pstDataIndex->ucHead=0;
     		}
            if(pstDataIndex->ucHead%uspointnum_perblock == 0)
            {
                blockpos += pstDataIndex->ucHead/uspointnum_perblock;
                ulDataAddr = blockpos*MCP_FLASH_BLOCK_SIZE;
                MCP_Flash_Sector_Erase(ulDataAddr);

         		if(pstDataIndex->ucHead==pstDataIndex->ucTail)
         			pstDataIndex->ucTail += uspointnum_perblock;
         		if(pstDataIndex->ucTail>=uspoint_totnum)
         			pstDataIndex->ucTail=0;
            }      
            save_daymonth_data_info(ucMeterNo, CON_MONTHDATA_TYPE, uspoint_totnum);//azh 还得把索引保存到EEPROM
 		}
 	}
	return ucRet;
}
//unsigned char DataProc_SaveData_DayFrozen(unsigned char ucMeterNo,STDATETIME stDateTime,STHISRUNDATA *pstHisRunData)
//{//保存抄表日数据
//	unsigned char ucRet=RET_SUCCESS;
// 	unsigned char ucPos=0xFF,i;
// 	unsigned short usBeginPage,usBeginPos;
//     unsigned long ulDataAddr;
// 	unsigned long ulDataTime,ulQueryTime;
// 	STDAYFROZENLISTINDEX *pstDataIndex=NULL;
// 	if(ucMeterNo>=CON_METER_NUM)
// 		return RET_ERROR;
// 	if(DP_Get_TerDP_State()==CON_TERDP_STATE_DOWNLOAD)			//数据块用于程序下装数据存储
// 		return ucRet;
// 	pstDataIndex=&(stHisRunDataIndex[ucMeterNo].stDayFrozenDataIndex);
// 	if(pstDataIndex->ucHead != pstDataIndex->ucTail)
// 	{//非空
// 		for(i=0;i<CON_DAYFROZENDATA_NUM;i++)
// 		{
// 			ucPos=(pstDataIndex->ucTail+i) % CON_DAYFROZENDATA_NUM;
// 			if(ucPos==pstDataIndex->ucHead)
// 			{
// 				ucPos=0xFF;
// 				break;
// 			}
// 			ulDataTime=TimeConver(&(pstDataIndex->stDateTimeList[ucPos]),CON_TIMEUNIT_SEC);
// 			ulQueryTime=TimeConver(&stDateTime,CON_TIMEUNIT_SEC);
// 			if((ulQueryTime<=ulDataTime && ulDataTime<ulQueryTime+3600) || (ulQueryTime>=ulDataTime && ulQueryTime<ulDataTime+3600))
// 				break;
// 		}
// 	}
// 	if(ucPos<CON_DAYFROZENDATA_NUM)
// 	{//更新
// 		ulDataAddr	=sizeof(STHISRUNDATA)*ucPos;
// 	}else
// 	{//增加
// 		ulDataAddr	=sizeof(STHISRUNDATA)*pstDataIndex->ucHead;
// 		//更新索引
// 		pstDataIndex->stDateTimeList[pstDataIndex->ucHead]=stDateTime;
// 		pstDataIndex->ucHead++;
// 		if(pstDataIndex->ucHead>=CON_DAYFROZENDATA_NUM)
// 			pstDataIndex->ucHead=0;
// 		if(pstDataIndex->ucHead==pstDataIndex->ucTail)
// 			pstDataIndex->ucTail++;
// 		if(pstDataIndex->ucTail>=CON_DAYFROZENDATA_NUM)
// 			pstDataIndex->ucTail=0;
// 	}
// 	//计算位置
// 	usBeginPage	= CON_DFPAGE_HEAD_HISDATA + CON_DFPAGE_HISDATA_PAGENUM*ucMeterNo;
// 	usBeginPage	=usBeginPage+(unsigned short)(ulDataAddr/FPAGE_SIZE);
// 	usBeginPos	=(unsigned short)(ulDataAddr % FPAGE_SIZE);
// 	ucRet=Flash_SetMainMemory((unsigned char *)pstHisRunData,sizeof(STHISRUNDATA),usBeginPage,usBeginPos);
// 	if(ucRet==EF_NORMAL)
// 		ucRet=RET_SUCCESS;
// 	else
// 		ucRet=RET_ERROR;
//	return ucRet;
//}
unsigned char DataProc_SaveData_DayFrozen(unsigned char ucMeterNo,STDATETIME stDateTime,STHISRUNDATA *pstHisRunData)
{//保存抄表日数据
	unsigned char ucRet=RET_SUCCESS;
 	unsigned char ucPos=0xFF,i;
// 	unsigned short usBeginPage,usBeginPos;
    unsigned short uspointnum_perblock,uspoint_totnum,blockpos;

     unsigned long ulDataAddr;
 	unsigned long ulDataTime,ulQueryTime;
 	STDATETIME stTempTime;
 	unsigned char ucMPNo,ucMPType;
 	STDAYFROZENLISTINDEX *pstDataIndex=NULL;
 	
 	if(ucMeterNo>=CON_METER_NUM)
 		return RET_ERROR;
 	ucMPNo=RunPara_GetMeter_MPNo(ucMeterNo);
 	ucMPType=RunPara_GetMPType(ucMPNo);
 	if(ucMPType!=CON_MPTYPE_METER)
 		return RET_ERROR;
// 	if(DP_Get_TerDP_State()==CON_TERDP_STATE_DOWNLOAD)			//数据块用于程序下装数据存储
// 		return ucRet;
 	stTempTime=stDateTime;						//stDateTime为日初或者日期变化后的时间，数据要作为上一天的统计结果
 	stTempTime.ucHour=0;
 	stTempTime.ucMinute=0;
 	stTempTime.ucSecond=0;
 	pstDataIndex=&(stHisRunDataIndex[ucMeterNo].stDayFrozenDataIndex);

    uspointnum_perblock = MCP_FLASH_BLOCK_SIZE/sizeof(STHISRUNDATA);
    uspoint_totnum = CON_FROZEDATA_BLOCKNUM_PER_MP*uspointnum_perblock;

 	if(pstDataIndex->ucHead != pstDataIndex->ucTail)
 	{//非空
 		for(i=0;i<uspoint_totnum;i++)
 		{
 			ucPos=(pstDataIndex->ucTail+i) % uspoint_totnum;
 			if(ucPos==pstDataIndex->ucHead)
 			{
 				ucPos=0xFF;
 				break;
 			}
 			ulDataTime=TimeConver(&(pstDataIndex->stDateTimeList[ucPos]),CON_TIMEUNIT_SEC);
 			ulQueryTime=TimeConver(&stTempTime,CON_TIMEUNIT_SEC);
 			if((ulQueryTime<=ulDataTime && ulDataTime<ulQueryTime+3600) || (ulQueryTime>=ulDataTime && ulQueryTime<ulDataTime+3600))
 				break;
 		}
 	}
 	if(ucPos<uspoint_totnum)
 	{//更新
// 		ulDataAddr	=sizeof(STHISRUNDATA)**ucPos;//azh 这个时间保存过 就不更新保存了 因为要擦除才行
 	}else
 	{//增加
        if(ucMPNo < CON_MAX_MPNUM)
        {    
            blockpos = CON_DFPAGE_HEAD_HISDATA + ucMPNo*(CON_DAYDATA_BLOCKNUM_PER_MP+CON_MONTHDATA_BLOCKNUM_PER_MP+CON_FROZEDATA_BLOCKNUM_PER_MP);
            blockpos += CON_DAYDATA_BLOCKNUM_PER_MP + CON_MONTHDATA_BLOCKNUM_PER_MP;//日数据、月数据、结算日数据偏移
            ulDataAddr = blockpos + pstDataIndex->ucHead/uspointnum_perblock;
            ulDataAddr *= MCP_FLASH_BLOCK_SIZE;

            ulDataAddr += sizeof(STHISRUNDATA)*(pstDataIndex->ucHead%uspointnum_perblock);

        	ucRet = MCP_Flash_Write(ulDataAddr,(unsigned char *)pstHisRunData,sizeof(STHISRUNDATA));	
         	if(ucRet==NO_ERROR)
         		ucRet=RET_SUCCESS;
         	else
         		ucRet=RET_ERROR;

    		//更新索引
     		pstDataIndex->stDateTimeList[pstDataIndex->ucHead]=stTempTime;
     		pstDataIndex->ucHead++;
     		if(pstDataIndex->ucHead>=uspoint_totnum)
     		{    
     			pstDataIndex->ucHead=0;
     		}
            if(pstDataIndex->ucHead%uspointnum_perblock == 0)
            {
                blockpos += pstDataIndex->ucHead/uspointnum_perblock;
                ulDataAddr = blockpos*MCP_FLASH_BLOCK_SIZE;
                MCP_Flash_Sector_Erase(ulDataAddr);

         		if(pstDataIndex->ucHead==pstDataIndex->ucTail)
         			pstDataIndex->ucTail += uspointnum_perblock;
         		if(pstDataIndex->ucTail>=uspoint_totnum)
         			pstDataIndex->ucTail=0;
            }      
            save_daymonth_data_info(ucMeterNo, CON_FROZEDATA_TYPE, uspoint_totnum);//azh 还得把索引保存到EEPROM
 		}
 	}
	return ucRet;
}
//unsigned char DataProc_SumGSaveData_Day(unsigned char ucSumGNo,STDATETIME stDateTime,STHISSUMGROUPRUNDATA *pstHisSumGroupRunData)
//{//保存总加日数据
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucPos=0xFF,i;
//	unsigned short usBeginPage,usBeginPos;
//	unsigned long ulDataAddr;
//	STDATETIME stTempTime;
//	unsigned long ulDataTime,ulQueryTime;
//	STDAYLISTINDEX *pstDataIndex=NULL;
//	if(ucSumGNo>=CON_SUMGROUP_NUM)
//		return RET_ERROR;
//	if(DP_Get_TerDP_State()==CON_TERDP_STATE_DOWNLOAD)			//数据块用于程序下装数据存储
//		return ucRet;
//	stTempTime=stDateTime;
//	stTempTime.ucHour=0;
//	stTempTime.ucMinute=0;
//	stTempTime.ucSecond=0;
//	pstDataIndex=&(stHisSumGroupDataIndex[ucSumGNo].stSumGDayDataIndex);
//	if(pstDataIndex->ucHead != pstDataIndex->ucTail)
//	{//非空
//		for(i=0;i<CON_DAYDATA_NUM;i++)
//		{
//			ucPos=(pstDataIndex->ucTail+i) % CON_DAYDATA_NUM;
//			if(ucPos==pstDataIndex->ucHead)
//			{
//				ucPos=0xFF;
//				break;
//			}
//			ulDataTime=TimeConver(&(pstDataIndex->stDateTimeList[ucPos]),CON_TIMEUNIT_SEC);
//			ulQueryTime=TimeConver(&stTempTime,CON_TIMEUNIT_SEC);
//			if((ulQueryTime<=ulDataTime && ulDataTime<ulQueryTime+3600) || (ulQueryTime>=ulDataTime && ulQueryTime<ulDataTime+3600))
//				break;
//		}
//	}
//	if(ucPos<CON_DAYDATA_NUM)
//	{//更新
//		ulDataAddr	=sizeof(STHISSUMGROUPRUNDATA)*ucPos;
//	}else
//	{//增加
//		ulDataAddr	=sizeof(STHISSUMGROUPRUNDATA)*pstDataIndex->ucHead;
//		//更新索引
//		pstDataIndex->stDateTimeList[pstDataIndex->ucHead]=stTempTime;
//		pstDataIndex->ucHead++;
//		if(pstDataIndex->ucHead>=CON_DAYDATA_NUM)
//			pstDataIndex->ucHead=0;
//		if(pstDataIndex->ucHead==pstDataIndex->ucTail)
//			pstDataIndex->ucTail++;
//		if(pstDataIndex->ucTail>=CON_DAYDATA_NUM)
//			pstDataIndex->ucTail=0;
//	}
//	//计算位置
//	usBeginPage	= CON_DFPAGE_HEAD_SUMG_HISDATA + CON_DFPAGE_SUMG_HISDATA_PAGENUM*ucSumGNo;
//	usBeginPage	=usBeginPage+(unsigned short)(ulDataAddr/FPAGE_SIZE);
//	usBeginPos	=(unsigned short)(ulDataAddr % FPAGE_SIZE);
//	ucRet=Flash_SetMainMemory((unsigned char *)pstHisSumGroupRunData,sizeof(STHISSUMGROUPRUNDATA),usBeginPage,usBeginPos);
//	if(ucRet==EF_NORMAL)
//		ucRet=RET_SUCCESS;
//	else
//		ucRet=RET_ERROR;
//	return ucRet;
//}

//unsigned char DataProc_SumGSaveData_Month(unsigned char ucSumGNo,STDATETIME stDateTime,STHISSUMGROUPRUNDATA *pstHisSumGroupRunData)
//{//保存总加月数据
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucPos=0xFF,i;
//	unsigned short usBeginPage,usBeginPos;
//	unsigned long ulDataAddr;
//	STDATETIME stTempTime;
//	unsigned long ulDataTime,ulQueryTime;
//	STMONTHLISTINDEX *pstDataIndex=NULL;
//	if(ucSumGNo>=CON_SUMGROUP_NUM)
//		return RET_ERROR;
//	if(DP_Get_TerDP_State()==CON_TERDP_STATE_DOWNLOAD)			//数据块用于程序下装数据存储
//		return ucRet;
//	stTempTime=stDateTime;
//	stTempTime.ucDay=1;
//	stTempTime.ucHour=0;
//	stTempTime.ucMinute=0;
//	stTempTime.ucSecond=0;
//	pstDataIndex=&(stHisSumGroupDataIndex[ucSumGNo].stSumGMonthDataIndex);
//	if(pstDataIndex->ucHead != pstDataIndex->ucTail)
//	{//非空
//		for(i=0;i<CON_MONTHDATA_NUM;i++)
//		{
//			ucPos=(pstDataIndex->ucTail+i) % CON_MONTHDATA_NUM;
//			if(ucPos==pstDataIndex->ucHead)
//			{
//				ucPos=0xFF;
//				break;
//			}
//			ulDataTime=TimeConver(&(pstDataIndex->stDateTimeList[ucPos]),CON_TIMEUNIT_SEC);
//			ulQueryTime=TimeConver(&stTempTime,CON_TIMEUNIT_SEC);
//			if((ulQueryTime<=ulDataTime && ulDataTime<ulQueryTime+432000) || (ulQueryTime>=ulDataTime && ulQueryTime<ulDataTime+432000))	//偏差5天，考虑月末28日~31日
//				break;
//		}
//	}
//	if(ucPos<CON_MONTHDATA_NUM)
//	{//更新
//		ulDataAddr	=sizeof(STHISSUMGROUPRUNDATA)*(ucPos+CON_DAYDATA_NUM);
//	}else
//	{//增加
//		ulDataAddr	=sizeof(STHISSUMGROUPRUNDATA)*(pstDataIndex->ucHead+CON_DAYDATA_NUM);
//		//更新索引
//		pstDataIndex->stDateTimeList[pstDataIndex->ucHead]=stTempTime;
//		pstDataIndex->ucHead++;
//		if(pstDataIndex->ucHead>=CON_MONTHDATA_NUM)
//			pstDataIndex->ucHead=0;
//		if(pstDataIndex->ucHead==pstDataIndex->ucTail)
//			pstDataIndex->ucTail++;
//		if(pstDataIndex->ucTail>=CON_MONTHDATA_NUM)
//			pstDataIndex->ucTail=0;
//	}
//	//计算位置
//	usBeginPage	= CON_DFPAGE_HEAD_SUMG_HISDATA + CON_DFPAGE_SUMG_HISDATA_PAGENUM*ucSumGNo;
//	usBeginPage	=usBeginPage+(unsigned short)(ulDataAddr/FPAGE_SIZE);
//	usBeginPos	=(unsigned short)(ulDataAddr % FPAGE_SIZE);
//	ucRet=Flash_SetMainMemory((unsigned char *)pstHisSumGroupRunData,sizeof(STHISSUMGROUPRUNDATA),usBeginPage,usBeginPos);
//	if(ucRet==EF_NORMAL)
//		ucRet=RET_SUCCESS;
//	else
//		ucRet=RET_ERROR;
//	return ucRet;
//}

unsigned char DataProc_GetData_DayFrozen(unsigned char ucMeterNo,STDATETIME stDateTime,STHISRUNDATA *pstHisRunData)
{//读抄表日数据
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucPos=0xFF,i=0;
//	unsigned short usBeginPage,usBeginPos;
    unsigned long ulDataAddr;
    unsigned short uspointnum_perblock,uspoint_totnum;
	unsigned long ulDataTime,ulQueryTime;
//	long lTmp=0;
	unsigned char ucMPNo=1;
	STDATETIME stThisTime=GetCurrentTime();
	STDATETIME stTempTime;
	PSTFREEZEDAY pstFreezeDayPara;
	STDAYFROZENLISTINDEX *pstDataIndex=NULL;
	if(ucMeterNo>=CON_METER_NUM)
		return RET_ERROR;
	pstFreezeDayPara=RunPara_GetMPFreezeDay(ucMPNo);
	if(Is_DateTime_Availble(&(pstFreezeDayPara->stFreezeDay[0]))==RET_SUCCESS)
	{
		stTempTime=stDateTime;
		stTempTime.ucHour=pstFreezeDayPara->stFreezeDay[0].ucHour;
		stTempTime.ucMinute=pstFreezeDayPara->stFreezeDay[0].ucMinute;
		stTempTime.ucSecond=0;
		ulQueryTime=TimeConver(&stTempTime,CON_TIMEUNIT_SEC);
		if(stDateTime.ucYear==stThisTime.ucYear && stDateTime.ucMonth==stThisTime.ucMonth && stDateTime.ucDay==stThisTime.ucDay
			&& (stThisTime.ucHour*60+stThisTime.ucMinute) < (pstFreezeDayPara->stFreezeDay[0].ucHour*60+pstFreezeDayPara->stFreezeDay[0].ucMinute))
		{//当日统计数据取当前数据
			memcpy(pstHisRunData,&(stHisDayFrozenData[ucMeterNo]),sizeof(STHISRUNDATA));
			pstHisRunData->stRecordTime=GetCurrentTime();
		}else
		{
            uspointnum_perblock = MCP_FLASH_BLOCK_SIZE/sizeof(STHISRUNDATA);//azh
            uspoint_totnum = CON_FROZEDATA_BLOCKNUM_PER_MP*uspointnum_perblock;
            
			pstDataIndex=&(stHisRunDataIndex[ucMeterNo].stDayFrozenDataIndex);
			if(pstDataIndex->ucHead != pstDataIndex->ucTail)
			{//非空
				for(i=0;i<uspoint_totnum;i++)
				{
					ucPos=(pstDataIndex->ucTail+i) % uspoint_totnum;
					if(ucPos==pstDataIndex->ucHead)
					{
						ucPos=0xFF;
						break;
					}
					ulDataTime=TimeConver(&(pstDataIndex->stDateTimeList[ucPos]),CON_TIMEUNIT_SEC);
					if((ulQueryTime<=ulDataTime && ulDataTime<ulQueryTime+3600) || (ulQueryTime>=ulDataTime && ulQueryTime<ulDataTime+3600))
						break;
				}
			}
			if(ucPos<uspoint_totnum)
			{
				//计算位置
//azh 				usBeginPage	=CON_DFPAGE_HEAD_HISDATA + CON_DFPAGE_HISDATA_PAGENUM*ucMeterNo;
// 				ulDataAddr	=sizeof(STHISRUNDATA)*ucPos;
// 				usBeginPage	=usBeginPage+(unsigned short)(ulDataAddr/FPAGE_SIZE);
// 				usBeginPos	=(unsigned short)(ulDataAddr % FPAGE_SIZE);
// 				ucRet=Flash_GetMainMemory(usBeginPage,usBeginPos,sizeof(STHISRUNDATA),(unsigned char *)pstHisRunData);
// 				if(ucRet==EF_NORMAL)
// 					ucRet=RET_SUCCESS;
// 				else
// 					ucRet=RET_ERROR;
//azh
                ulDataAddr = CON_DFPAGE_HEAD_HISDATA + ucMeterNo*(CON_DAYDATA_BLOCKNUM_PER_MP+CON_MONTHDATA_BLOCKNUM_PER_MP+CON_FROZEDATA_BLOCKNUM_PER_MP);
                ulDataAddr += CON_DAYDATA_BLOCKNUM_PER_MP + CON_MONTHDATA_BLOCKNUM_PER_MP;//日数据、月数据、结算日数据偏移
                ulDataAddr += ucPos/uspointnum_perblock;
                ulDataAddr *= MCP_FLASH_BLOCK_SIZE;
                ulDataAddr += sizeof(STHISRUNDATA)*(ucPos%uspointnum_perblock);
    
            	ucRet = MCP_Flash_Read(ulDataAddr,(unsigned char *)pstHisRunData,sizeof(STHISRUNDATA));	
             	if(ucRet==NO_ERROR)
             		ucRet=RET_SUCCESS;
             	else
             		ucRet=RET_ERROR;
			}else
				ucRet=RET_ERROR;
		}
	}else
		ucRet=RET_ERROR;
	return ucRet;
}
//unsigned char DataProc_GetData_TerDayFrozen(STDATETIME stDateTime,STHISRUNDATA *pstHisRunData)
//{//终端抄表日
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucPos=0xFF,i=0;
////	unsigned short usBeginPage,usBeginPos;
////    unsigned long ulDataAddr;
//	unsigned long ulDataTime,ulQueryTime;
//	unsigned char ucMPNo=1;
//	STDATETIME stThisTime=GetCurrentTime();
//	STDATETIME stTempTime;
//	STFREEZEDAY stFreezeDayPara;
//	STDAYFROZENLISTINDEX *pstDataIndex=NULL;
//	stFreezeDayPara=RunPara_GetMPFreezeDay(ucMPNo);
//	if(Is_DateTime_Availble(&(stFreezeDayPara.stFreezeDay[0]))==RET_SUCCESS)
//	{//主要取日的时与分，从生成的数据中查找指定日期加时与分的数据
//		stTempTime=stDateTime;
//		stTempTime.ucHour=stFreezeDayPara.stFreezeDay[0].ucHour;
//		stTempTime.ucMinute=stFreezeDayPara.stFreezeDay[0].ucMinute;
//		stTempTime.ucSecond=0;
//		ulQueryTime=TimeConver(&stTempTime,CON_TIMEUNIT_SEC);
//		if(stDateTime.ucYear==stThisTime.ucYear && stDateTime.ucMonth==stThisTime.ucMonth && stDateTime.ucDay==stThisTime.ucDay
//			&& (stThisTime.ucHour*60+stThisTime.ucMinute) < (stFreezeDayPara.stFreezeDay[0].ucHour*60+stFreezeDayPara.stFreezeDay[0].ucMinute))
//		{//当日统计数据取当前数据
//			memcpy(pstHisRunData,&(stHisTerDayFrozenData),sizeof(STHISRUNDATA));
//			pstHisRunData->stRecordTime=GetCurrentTime();
//		}else
//		{
//			pstDataIndex=&(stHisTerDataIndex.stDayFrozenDataIndex);
//			if(pstDataIndex->ucHead != pstDataIndex->ucTail)
//			{//非空
//				for(i=0;i<CON_DAYFROZENDATA_NUM;i++)
//				{
//					ucPos=(pstDataIndex->ucTail+i) % CON_DAYFROZENDATA_NUM;
//					if(ucPos==pstDataIndex->ucHead)
//					{
//						ucPos=0xFF;
//						break;
//					}
//					ulDataTime=TimeConver(&(pstDataIndex->stDateTimeList[ucPos]),CON_TIMEUNIT_SEC);
//					if((ulQueryTime<=ulDataTime && ulDataTime<ulQueryTime+3600) || (ulQueryTime>=ulDataTime && ulQueryTime<ulDataTime+3600))
//						break;
//				}
//			}
//			if(ucPos<CON_DAYFROZENDATA_NUM)
//			{//计算位置
//// 				usBeginPage	=CON_DFPAGE_HEAD_TER_HISDATA;
//// 				ulDataAddr	=sizeof(STHISRUNDATA)*ucPos;
//// 				usBeginPage	=usBeginPage+(unsigned short)(ulDataAddr/FPAGE_SIZE);
//// 				usBeginPos	=(unsigned short)(ulDataAddr % FPAGE_SIZE);
//// 				ucRet=Flash_GetMainMemory(usBeginPage,usBeginPos,sizeof(STHISRUNDATA),(unsigned char *)pstHisRunData);
//// 				if(ucRet==EF_NORMAL)
//// 					ucRet=RET_SUCCESS;
//// 				else
//// 					ucRet=RET_ERROR;
//			}else
//				ucRet=RET_ERROR;
//		}
//	}else
//		ucRet=RET_ERROR;
//	return ucRet;
//}

unsigned char DataProc_GetData_Day(unsigned char ucMeterNo,STDATETIME stDateTime,STHISRUNDATA *pstHisRunData)
{//读日数据
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucPos=0xFF,i=0;
// 	unsigned short usBeginPage,usBeginPos;
 	unsigned long ulDataAddr;
    unsigned short uspointnum_perblock,uspoint_totnum;
	unsigned long ulQueryTime,ulDataTime,ulThisTime;

	STDATETIME stThisTime=GetCurrentTime();
	STDAYLISTINDEX *pstDataIndex=NULL;
	if(ucMeterNo>=CON_METER_NUM)
		return RET_ERROR;
	stThisTime.ucSecond=59;
	stThisTime.ucMinute=59;
	stThisTime.ucHour=23;
	ulThisTime=TimeConver(&stThisTime,CON_TIMEUNIT_SEC);
	ulQueryTime=TimeConver(&stDateTime,CON_TIMEUNIT_SEC);
	if((ulQueryTime<=ulThisTime && ulThisTime<ulQueryTime+3600) || (ulQueryTime>=ulThisTime && ulQueryTime<ulThisTime+3600))
	{//当日统计数据取当前数据
		memcpy(pstHisRunData,&(stHisDayData[ucMeterNo]),sizeof(STHISRUNDATA));
		pstHisRunData->stRecordTime=GetCurrentTime();
	}else
	{
        uspointnum_perblock = MCP_FLASH_BLOCK_SIZE/sizeof(STHISRUNDATA);//azh
        uspoint_totnum = CON_DAYDATA_BLOCKNUM_PER_MP*uspointnum_perblock;
        
		pstDataIndex=&(stHisRunDataIndex[ucMeterNo].stDayDataIndex);
		if(pstDataIndex->ucHead != pstDataIndex->ucTail)
		{//非空
			for(i=0;i<uspoint_totnum;i++)
			{
				ucPos=(pstDataIndex->ucTail+i) % uspoint_totnum;
				if(ucPos==pstDataIndex->ucHead)
				{
					ucPos=0xFF;
					break;
				}
				ulDataTime=TimeConver(&(pstDataIndex->stDateTimeList[ucPos]),CON_TIMEUNIT_SEC);
				if((ulQueryTime<=ulDataTime && ulDataTime<ulQueryTime+3600) || (ulQueryTime>=ulDataTime && ulQueryTime<ulDataTime+3600))
					break;
			}
		}
		if(ucPos<uspoint_totnum)
		{//计算位置
// 			usBeginPage	=CON_DFPAGE_HEAD_HISDATA + CON_DFPAGE_HISDATA_PAGENUM*ucMeterNo;
// 			ulDataAddr	=sizeof(STHISRUNDATA)*(ucPos+CON_DAYFROZENDATA_NUM);
// 			usBeginPage	=usBeginPage+(unsigned short)(ulDataAddr/FPAGE_SIZE);
// 			usBeginPos	=(unsigned short)(ulDataAddr % FPAGE_SIZE);
// 			ucRet=Flash_GetMainMemory(usBeginPage,usBeginPos,sizeof(STHISRUNDATA),(unsigned char *)pstHisRunData);
// 			if(ucRet==EF_NORMAL)
// 				ucRet=RET_SUCCESS;
// 			else
// 				ucRet=RET_ERROR;
//azh
            ulDataAddr = CON_DFPAGE_HEAD_HISDATA + ucMeterNo*(CON_DAYDATA_BLOCKNUM_PER_MP+CON_MONTHDATA_BLOCKNUM_PER_MP+CON_FROZEDATA_BLOCKNUM_PER_MP);
            ulDataAddr += 0;//日数据、月数据、结算日数据偏移
            ulDataAddr += ucPos/uspointnum_perblock;
            ulDataAddr *= MCP_FLASH_BLOCK_SIZE;
            ulDataAddr += sizeof(STHISRUNDATA)*(ucPos%uspointnum_perblock);

        	ucRet = MCP_Flash_Read(ulDataAddr,(unsigned char *)pstHisRunData,sizeof(STHISRUNDATA));	
         	if(ucRet==NO_ERROR)
         		ucRet=RET_SUCCESS;
         	else
         		ucRet=RET_ERROR;
		}else
			ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char DataProc_GetData_Month(unsigned char ucMeterNo,STDATETIME stDateTime,STHISRUNDATA *pstHisRunData)
{//读月数据
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucPos=0xFF,i=0;
// 	unsigned short usBeginPage,usBeginPos;
 	unsigned long ulDataAddr;
    unsigned short uspointnum_perblock,uspoint_totnum;
	unsigned long ulQueryTime,ulDataTime,ulThisTime;

	STDATETIME stThisTime=GetCurrentTime();
	STMONTHLISTINDEX *pstDataIndex=NULL;
	if(ucMeterNo>=CON_METER_NUM)
		return RET_ERROR;
	stThisTime.ucSecond=59;
	stThisTime.ucMinute=59;
	stThisTime.ucHour=23;
	stThisTime.ucDay=28;
	ulThisTime=TimeConver(&stThisTime,CON_TIMEUNIT_SEC);
	ulQueryTime=TimeConver(&stDateTime,CON_TIMEUNIT_SEC);
	if((ulQueryTime<=ulThisTime && ulThisTime<ulQueryTime+432000) || (ulQueryTime>=ulThisTime && ulQueryTime<ulThisTime+432000))
	{//当月统计数据取当前数据
		memcpy(pstHisRunData,&(stHisMonthData[ucMeterNo]),sizeof(STHISRUNDATA));
		pstHisRunData->stRecordTime=GetCurrentTime();
	}else
	{
        uspointnum_perblock = MCP_FLASH_BLOCK_SIZE/sizeof(STHISRUNDATA);//azh
        uspoint_totnum = CON_MONTHDATA_BLOCKNUM_PER_MP*uspointnum_perblock;
        
		pstDataIndex=&(stHisRunDataIndex[ucMeterNo].stMonthDataIndex);
		if(pstDataIndex->ucHead != pstDataIndex->ucTail)
		{//非空
			for(i=0;i<uspoint_totnum;i++)
			{
				ucPos=(pstDataIndex->ucTail+i) % uspoint_totnum;
				if(ucPos==pstDataIndex->ucHead)
				{
					ucPos=0xFF;
					break;
				}
				ulDataTime=TimeConver(&(pstDataIndex->stDateTimeList[ucPos]),CON_TIMEUNIT_SEC);
				if((ulQueryTime<=ulDataTime && ulDataTime<ulQueryTime+432000) || (ulQueryTime>=ulDataTime && ulQueryTime<ulDataTime+432000))	//偏差5天，考虑月末28日~31日
					break;
			}
		}
		if(ucPos<uspoint_totnum)
		{
			//计算位置
// 			usBeginPage	=CON_DFPAGE_HEAD_HISDATA + CON_DFPAGE_HISDATA_PAGENUM*ucMeterNo;
// 			ulDataAddr	=sizeof(STHISRUNDATA)*(ucPos+CON_DAYFROZENDATA_NUM+CON_DAYDATA_NUM);
// 			usBeginPage	=usBeginPage+(unsigned short)(ulDataAddr/FPAGE_SIZE);
// 			usBeginPos	=(unsigned short)(ulDataAddr % FPAGE_SIZE);
// 			ucRet=Flash_GetMainMemory(usBeginPage,usBeginPos,sizeof(STHISRUNDATA),(unsigned char *)pstHisRunData);
// 			if(ucRet==EF_NORMAL)
// 				ucRet=RET_SUCCESS;
// 			else
// 				ucRet=RET_ERROR;
//azh
            ulDataAddr = CON_DFPAGE_HEAD_HISDATA + ucMeterNo*(CON_DAYDATA_BLOCKNUM_PER_MP+CON_MONTHDATA_BLOCKNUM_PER_MP+CON_FROZEDATA_BLOCKNUM_PER_MP);
            ulDataAddr += CON_DAYDATA_BLOCKNUM_PER_MP;//日数据、月数据、结算日数据偏移
            ulDataAddr += ucPos/uspointnum_perblock;
            ulDataAddr *= MCP_FLASH_BLOCK_SIZE;
            ulDataAddr += sizeof(STHISRUNDATA)*(ucPos%uspointnum_perblock);

        	ucRet = MCP_Flash_Read(ulDataAddr,(unsigned char *)pstHisRunData,sizeof(STHISRUNDATA));	
         	if(ucRet==NO_ERROR)
         		ucRet=RET_SUCCESS;
         	else
         		ucRet=RET_ERROR;
		}else
			ucRet=RET_ERROR;
	}
	return ucRet;
}

//unsigned char DataProc_GetSumGData_Month(unsigned char ucSumGNo,STDATETIME stDateTime,STHISSUMGROUPRUNDATA *pstHisSumGRunData)
//{//读总加月数据
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucPos=0xFF,i=0;
//	unsigned short usBeginPage,usBeginPos;
//	unsigned long ulDataAddr;
//	unsigned long ulQueryTime,ulDataTime,ulThisTime;
//	long lTmp=0;
//	STDATETIME stThisTime=GetCurrentTime();
//	STMONTHLISTINDEX *pstDataIndex=NULL;
//	if(ucSumGNo>=CON_SUMGROUP_NUM)
//		return RET_ERROR;
//	stThisTime.ucSecond=59;
//	stThisTime.ucMinute=59;
//	stThisTime.ucHour=23;
//	stThisTime.ucDay=28;
//	ulThisTime=TimeConver(&stThisTime,CON_TIMEUNIT_SEC);
//	ulQueryTime=TimeConver(&stDateTime,CON_TIMEUNIT_SEC);
//	if((ulQueryTime<=ulThisTime && ulThisTime<ulQueryTime+432000) || (ulQueryTime>=ulThisTime && ulQueryTime<ulThisTime+432000))
//	{//当月统计数据取当前数据
//		memcpy(pstHisSumGRunData,&(stHisSumGroupThisMonth.stHisSumGroupRunData[ucSumGNo]),sizeof(STHISSUMGROUPRUNDATA));
//		pstHisSumGRunData->stRecordTime=GetCurrentTime();
//	}else
//	{
//		pstDataIndex=&(stHisSumGroupDataIndex[ucSumGNo].stSumGMonthDataIndex);
//		if(pstDataIndex->ucHead != pstDataIndex->ucTail)
//		{//非空
//			for(i=0;i<CON_MONTHDATA_NUM;i++)
//			{
//				ucPos=(pstDataIndex->ucTail+i) % CON_MONTHDATA_NUM;
//				if(ucPos==pstDataIndex->ucHead)
//				{
//					ucPos=0xFF;
//					break;
//				}
//				ulDataTime=TimeConver(&(pstDataIndex->stDateTimeList[ucPos]),CON_TIMEUNIT_SEC);
//				if((ulQueryTime<=ulDataTime && ulDataTime<ulQueryTime+432000) || (ulQueryTime>=ulDataTime && ulQueryTime<ulDataTime+432000))	//偏差5天，考虑月末28日~31日
//					break;
//			}
//		}
//		if(ucPos<CON_MONTHDATA_NUM)
//		{//计算位置
//			usBeginPage	=CON_DFPAGE_HEAD_SUMG_HISDATA + CON_DFPAGE_SUMG_HISDATA_PAGENUM*ucSumGNo;
//			ulDataAddr	=sizeof(STHISSUMGROUPRUNDATA)*(ucPos+CON_DAYDATA_NUM);
//			usBeginPage	=usBeginPage+(unsigned short)(ulDataAddr/FPAGE_SIZE);
//			usBeginPos	=(unsigned short)(ulDataAddr % FPAGE_SIZE);
//			ucRet=Flash_GetMainMemory(usBeginPage,usBeginPos,sizeof(STHISSUMGROUPRUNDATA),(unsigned char *)pstHisSumGRunData);
//			if(ucRet==EF_NORMAL)
//				ucRet=RET_SUCCESS;
//			else
//				ucRet=RET_ERROR;
//		}else
//			ucRet=RET_ERROR;
//	}
//	return ucRet;
//}

//---------------- 日统计 1分钟处理一次----------------------
unsigned char DayData_Stat_InstantPow(unsigned char ucMeterNo,STDATETIME stDateTime)
{//最大功率日统计
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo;
	STHISVAL stHisVal;
	ucMPNo=RunPara_GetMeter_MPNo(ucMeterNo);
	stHisVal=Get_History_InstantPow_byMPNo(ucMPNo,0,0);
	DataProc_Stat_GetMax(stHisVal,&(stHisDayData[ucMeterNo].stHisMaxInstantPow),stDateTime);
	if(stHisVal.lVal==0)
		stHisDayData[ucMeterNo].usHisZeroTimeInstantPow++;			//1分钟检测一次
	stHisVal=Get_History_InstantPow_byMPNo(ucMPNo,0,1);
	DataProc_Stat_GetMax(stHisVal,&(stHisDayData[ucMeterNo].stHisMaxInstantPowA),stDateTime);
	if(stHisVal.lVal==0)
		stHisDayData[ucMeterNo].usHisZeroTimeInstantPowA++;
	stHisVal=Get_History_InstantPow_byMPNo(ucMPNo,0,2);
	DataProc_Stat_GetMax(stHisVal,&(stHisDayData[ucMeterNo].stHisMaxInstantPowB),stDateTime);
	if(stHisVal.lVal==0)
		stHisDayData[ucMeterNo].usHisZeroTimeInstantPowB++;
	stHisVal=Get_History_InstantPow_byMPNo(ucMPNo,0,3);
	DataProc_Stat_GetMax(stHisVal,&(stHisDayData[ucMeterNo].stHisMaxInstantPowC),stDateTime);
	if(stHisVal.lVal==0)
		stHisDayData[ucMeterNo].usHisZeroTimeInstantPowC++;
	return ucRet;
}

unsigned char DataProc_Stat_GetMax(STHISVAL stBaseVal,STHISVAL *pstRetVal,STDATETIME stDateTime)
{
	unsigned char ucRet=RET_SUCCESS;
	if(Is_DateTime_Availble(&(stBaseVal.stDateTime))==RET_SUCCESS && stBaseVal.lVal!=CONINVALIDVAL)
	{
		if(Is_DateTime_Availble(&(pstRetVal->stDateTime))==RET_SUCCESS && pstRetVal->lVal!=CONINVALIDVAL)
		{
			if(pstRetVal->lVal<stBaseVal.lVal)
			{
				pstRetVal->lVal=stBaseVal.lVal;
				pstRetVal->stDateTime=stDateTime;
				pstRetVal->ucSign=0;
			}
		}else
		{
			pstRetVal->lVal=stBaseVal.lVal;
			pstRetVal->stDateTime=stDateTime;
			pstRetVal->ucSign=0;
		}
	}
	return ucRet;
}

unsigned char DataProc_Stat_GetMin(STHISVAL stBaseVal,STHISVAL *pstRetVal,STDATETIME stDateTime)
{
	unsigned char ucRet=RET_SUCCESS;
	if(Is_DateTime_Availble(&(stBaseVal.stDateTime))==RET_SUCCESS && stBaseVal.lVal!=CONINVALIDVAL)
	{
		if(Is_DateTime_Availble(&(pstRetVal->stDateTime))==RET_SUCCESS && pstRetVal->lVal!=CONINVALIDVAL)
		{
			if(pstRetVal->lVal>stBaseVal.lVal)
			{
				pstRetVal->lVal=stBaseVal.lVal;
				pstRetVal->stDateTime=stDateTime;
				pstRetVal->ucSign=0;
			}
		}else
		{
			pstRetVal->lVal=stBaseVal.lVal;
			pstRetVal->stDateTime=stDateTime;
			pstRetVal->ucSign=0;
		}
	}
	return ucRet;
}

//unsigned char SumGDataProc_Stat_GetMax(float *pfValHis,STDATETIME *pstDataHis,float fValNow,STDATETIME stDateTime)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	float fValHis=(*pfValHis);
//	if(fValNow!=CONINVALIDVAL)
//	{
//		if(Is_DateTime_Availble(pstDataHis)==RET_SUCCESS && fValHis!=CONINVALIDVAL)
//		{
//			if(fValHis<fValNow)
//			{
//				(*pfValHis)=fValNow;
//				(*pstDataHis)=stDateTime;
//			}
//		}else
//		{
//			(*pfValHis)=fValNow;
//			(*pstDataHis)=stDateTime;
//		}
//	}
//	return ucRet;
//}

//unsigned char SumGDataProc_Stat_GetMin(float *pfValHis,STDATETIME *pstDataHis,float fValNow,STDATETIME stDateTime)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	float fValHis=(*pfValHis);
//	if(fValNow!=CONINVALIDVAL)
//	{
//		if(Is_DateTime_Availble(pstDataHis)==RET_SUCCESS && fValHis!=CONINVALIDVAL)
//		{
//			if(fValHis>fValNow)
//			{
//				(*pfValHis)=fValNow;
//				(*pstDataHis)=stDateTime;
//			}
//		}else
//		{
//			(*pfValHis)=fValNow;
//			(*pstDataHis)=stDateTime;
//		}
//	}
//	return ucRet;
//}

unsigned char DayData_Stat_Demand(unsigned char ucMeterNo,STDATETIME stDateTime)
{//最大需量日统计
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo;
	STHISVAL stHisVal;
	ucMPNo=RunPara_GetMeter_MPNo(ucMeterNo);
	stHisVal=Get_History_InstantPow_byMPNo(ucMPNo,0,0);
	DataProc_Stat_GetMax(stHisVal,&(stHisDayData[ucMeterNo].stHisMaxDemand),stDateTime);
	stHisVal=Get_History_InstantPow_byMPNo(ucMPNo,0,1);
	DataProc_Stat_GetMax(stHisVal,&(stHisDayData[ucMeterNo].stHisMaxDemandA),stDateTime);
	stHisVal=Get_History_InstantPow_byMPNo(ucMPNo,0,2);
	DataProc_Stat_GetMax(stHisVal,&(stHisDayData[ucMeterNo].stHisMaxDemandB),stDateTime);
	stHisVal=Get_History_InstantPow_byMPNo(ucMPNo,0,3);
	DataProc_Stat_GetMax(stHisVal,&(stHisDayData[ucMeterNo].stHisMaxDemandC),stDateTime);
	return ucRet;
}

unsigned char DayData_Stat_VoltStat(unsigned char ucMeterNo,STDATETIME stDateTime)
{//电压日统计
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo;
	STMPLIMIT		stMPLimit;
	STHISVAL stHisVol;
	ucMPNo=RunPara_GetMeter_MPNo(ucMeterNo);
	stMPLimit = RunPara_GetMPLimit(ucMPNo);
	stHisVol=Get_History_V_byMPNo(ucMPNo,0,1);
	stHisVol.lVal = stHisVol.lVal/10;//取整数
	DataProc_Stat_GetMax(stHisVol,&(stHisDayData[ucMeterNo].stHisMaxVolA),stDateTime);
	DataProc_Stat_GetMin(stHisVol,&(stHisDayData[ucMeterNo].stHisMinVolA),stDateTime);
	if(Is_DateTime_Availble(&(stHisVol.stDateTime))==RET_SUCCESS && stHisVol.lVal!=CONINVALIDVAL)
	{
		stHisDayData[ucMeterNo].ulHisSumVolA+=stHisVol.lVal;
		if(stHisVol.lVal>stMPLimit.usVolErrUpLimit)
			stHisDayData[ucMeterNo].usHisErrUpTimeVolA++;			//1分钟检测一次
		if(stHisVol.lVal<stMPLimit.usVolErrDownLimit)
			stHisDayData[ucMeterNo].usHisErrDownTimeVolA++;
		if(stHisVol.lVal>stMPLimit.usVolUpLimit)
			stHisDayData[ucMeterNo].usHisNorUpTimeVolA++;
		else
		{
			if(stHisVol.lVal<stMPLimit.usVolDownLimit)
				stHisDayData[ucMeterNo].usHisNorDownTimeVolA++;
			else
				stHisDayData[ucMeterNo].usHisNormalTimeVolA++;
		}
	}
	stHisVol=Get_History_V_byMPNo(ucMPNo,0,2);
	stHisVol.lVal = stHisVol.lVal/10;//取整数
	DataProc_Stat_GetMax(stHisVol,&(stHisDayData[ucMeterNo].stHisMaxVolB),stDateTime);
	DataProc_Stat_GetMin(stHisVol,&(stHisDayData[ucMeterNo].stHisMinVolB),stDateTime);
	if(Is_DateTime_Availble(&(stHisVol.stDateTime))==RET_SUCCESS && stHisVol.lVal!=CONINVALIDVAL)
	{
		stHisDayData[ucMeterNo].ulHisSumVolB+=stHisVol.lVal;
		if(stHisVol.lVal>stMPLimit.usVolErrUpLimit)
			stHisDayData[ucMeterNo].usHisErrUpTimeVolB++;			//1分钟检测一次
		if(stHisVol.lVal<stMPLimit.usVolErrDownLimit)
			stHisDayData[ucMeterNo].usHisErrDownTimeVolB++;
		if(stHisVol.lVal>stMPLimit.usVolUpLimit)
			stHisDayData[ucMeterNo].usHisNorUpTimeVolB++;
		else
		{
			if(stHisVol.lVal<stMPLimit.usVolDownLimit)
				stHisDayData[ucMeterNo].usHisNorDownTimeVolB++;
			else
				stHisDayData[ucMeterNo].usHisNormalTimeVolB++;
		}
	}
	stHisVol=Get_History_V_byMPNo(ucMPNo,0,3);
	stHisVol.lVal = stHisVol.lVal/10;//取整数
	DataProc_Stat_GetMax(stHisVol,&(stHisDayData[ucMeterNo].stHisMaxVolC),stDateTime);
	DataProc_Stat_GetMin(stHisVol,&(stHisDayData[ucMeterNo].stHisMinVolC),stDateTime);
	if(Is_DateTime_Availble(&(stHisVol.stDateTime))==RET_SUCCESS && stHisVol.lVal!=CONINVALIDVAL)
	{
		stHisDayData[ucMeterNo].ulHisSumVolC+=stHisVol.lVal;
		if(stHisVol.lVal>stMPLimit.usVolErrUpLimit)
			stHisDayData[ucMeterNo].usHisErrUpTimeVolC++;			//1分钟检测一次
		if(stHisVol.lVal<stMPLimit.usVolErrDownLimit)
			stHisDayData[ucMeterNo].usHisErrDownTimeVolC++;
		if(stHisVol.lVal>stMPLimit.usVolUpLimit)
			stHisDayData[ucMeterNo].usHisNorUpTimeVolC++;
		else
		{
			if(stHisVol.lVal<stMPLimit.usVolDownLimit)
				stHisDayData[ucMeterNo].usHisNorDownTimeVolC++;
			else
				stHisDayData[ucMeterNo].usHisNormalTimeVolC++;
		}
	}
	return ucRet;
}

unsigned char DayData_Stat_Unbalance(unsigned char ucMeterNo,STDATETIME stDateTime)
{//不平衡度日统计
	unsigned char ucRet=RET_SUCCESS;
// 	unsigned char ucMPNo;
// 	unsigned long ulVal;
// 	STMPLIMIT	stMPLimit;
// 	STMPRATEVAL	stMPRateVal;
// 	STHISVAL stHisValA,stHisValB,stHisValC;
// 	ucMPNo=RunPara_GetMeter_MPNo(ucMeterNo);
// 	stMPLimit=RunPara_GetMPLimit(ucMPNo);
// 	stMPRateVal=RunPara_GetMPRateVal(ucMPNo);
// 	stHisValA=Get_History_I_byMPNo(ucMPNo,0,1);
// 	stHisValB=Get_History_I_byMPNo(ucMPNo,0,2);
// 	stHisValC=Get_History_I_byMPNo(ucMPNo,0,3);
// 	ulVal=GetUnBalanceVal_byVal(stHisValA.lVal,stHisValB.lVal,stHisValC.lVal,stMPRateVal.ucMPLineType);
// 	if(ulVal!=0xFFFFFFFF)
// 	{
// 		if(ulVal>stMPLimit.usCurUnBalance)
// 			stHisDayData[ucMeterNo].usHisUnbalanceTimeCurr++;
// 	}
// 	stHisValA.lVal = ulVal;
// 	DataProc_Stat_GetMax(stHisValA,&(stHisDayData[ucMeterNo].stHisMaxUnBalanceCur),stDateTime);
// 	stHisValA=Get_History_V_byMPNo(ucMPNo,0,1);
// 	stHisValB=Get_History_V_byMPNo(ucMPNo,0,2);
// 	stHisValC=Get_History_V_byMPNo(ucMPNo,0,3);
// 	ulVal=GetUnBalanceVal_byVal(stHisValA.lVal,stHisValB.lVal,stHisValC.lVal,stMPRateVal.ucMPLineType);
// 	if(ulVal!=0xFFFFFFFF)
// 	{
// 		if(ulVal>stMPLimit.usVolUnBalance)
// 			stHisDayData[ucMeterNo].usHisUnbalanceTimeVolt++;
// 	}
// 	stHisValA.lVal = ulVal;
// 	DataProc_Stat_GetMax(stHisValA,&(stHisDayData[ucMeterNo].stHisMaxUnBalanceVol),stDateTime);
	return ucRet;
}

unsigned char DayData_Stat_CurrOver(unsigned char ucMeterNo,STDATETIME stDateTime)
{//过流日统计
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo;
	STMPLIMIT		stMPLimit;
	STHISVAL stHisVol;
	ucMPNo=RunPara_GetMeter_MPNo(ucMeterNo);
	stMPLimit = RunPara_GetMPLimit(ucMPNo);
	stHisVol=Get_History_I_byMPNo(ucMPNo,0,1);
	stHisVol.lVal /= 10;
	DataProc_Stat_GetMax(stHisVol,&(stHisDayData[ucMeterNo].stHisMaxCurrA),stDateTime);
	if(Is_DateTime_Availble(&(stHisVol.stDateTime))==RET_SUCCESS)
	{
		if(stHisVol.lVal>stMPLimit.usCurErrUpLimit)
			stHisDayData[ucMeterNo].usHisErrUpTimeCurrA++;
		if(stHisVol.lVal>stMPLimit.usCurUpLimit)
			stHisDayData[ucMeterNo].usHisNorUpTimeCurrA++;
	}
	stHisVol=Get_History_I_byMPNo(ucMPNo,0,2);
	stHisVol.lVal /= 10;
	DataProc_Stat_GetMax(stHisVol,&(stHisDayData[ucMeterNo].stHisMaxCurrB),stDateTime);
	if(Is_DateTime_Availble(&(stHisVol.stDateTime))==RET_SUCCESS)
	{
		if(stHisVol.lVal>stMPLimit.usCurErrUpLimit)
			stHisDayData[ucMeterNo].usHisErrUpTimeCurrB++;
		if(stHisVol.lVal>stMPLimit.usCurUpLimit)
			stHisDayData[ucMeterNo].usHisNorUpTimeCurrB++;
	}
	stHisVol=Get_History_I_byMPNo(ucMPNo,0,3);
	stHisVol.lVal /= 10;
	DataProc_Stat_GetMax(stHisVol,&(stHisDayData[ucMeterNo].stHisMaxCurrC),stDateTime);
	if(Is_DateTime_Availble(&(stHisVol.stDateTime))==RET_SUCCESS)
	{
		if(stHisVol.lVal>stMPLimit.usCurErrUpLimit)
			stHisDayData[ucMeterNo].usHisErrUpTimeCurrC++;
		if(stHisVol.lVal>stMPLimit.usCurUpLimit)
			stHisDayData[ucMeterNo].usHisNorUpTimeCurrC++;
	}
	//零序电流
	return ucRet;
}

unsigned char DayData_Stat_SPowOver(unsigned char ucMeterNo,STDATETIME stDateTime)
{//视在功率越限日统计
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo;
	STMPLIMIT		stMPLimit;
	STHISVAL stHisValP,stHisValQ,stHisValS;
	ucMPNo=RunPara_GetMeter_MPNo(ucMeterNo);
	stMPLimit = RunPara_GetMPLimit(ucMPNo);
	stHisValP=Get_History_InstantPow_byMPNo(ucMPNo,0,0);
	stHisValQ=Get_History_InstantVar_byMPNo(ucMPNo,0,0);
	stHisValS=stHisValP;
	if(stHisValP.lVal!=CONINVALIDVAL && stHisValQ.lVal!=CONINVALIDVAL)
		stHisValS.lVal = GetSPow(stHisValP.lVal,stHisValQ.lVal);
	else
		ClearHisVal(&stHisValS);
	if(Is_DateTime_Availble(&(stHisValS.stDateTime))==RET_SUCCESS)
	{
		if(stHisValS.lVal>stMPLimit.ulPowErrUpLimit)
			stHisDayData[ucMeterNo].usHisErrUpTimeSPower++;
		if(stHisValS.lVal>stMPLimit.ulPowUpLimit)
			stHisDayData[ucMeterNo].usHisNorUpTimeSPower++;
	}
	return ucRet;
}

unsigned char DayData_Stat_VoltBreak(unsigned char ucMeterNo,STDATETIME stDateTime)
{//断相日统计
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo;
	STHISVAL stHisVol;
	STHISTIME stHisTime;
	ucMPNo=RunPara_GetMeter_MPNo(ucMeterNo);
	stHisVol=Get_History_VolBreakNum_byMPNo(ucMPNo,0,0);
	stHisDayData[ucMeterNo].usHisVolBreakNum=(unsigned short)stHisVol.lVal;
	stHisVol=Get_History_VolBreakNum_byMPNo(ucMPNo,0,1);
	stHisDayData[ucMeterNo].usHisVolABreakNum=(unsigned short)stHisVol.lVal;
	stHisVol=Get_History_VolBreakNum_byMPNo(ucMPNo,0,2);
	stHisDayData[ucMeterNo].usHisVolBBreakNum=(unsigned short)stHisVol.lVal;
	stHisVol=Get_History_VolBreakNum_byMPNo(ucMPNo,0,3);
	stHisDayData[ucMeterNo].usHisVolCBreakNum=(unsigned short)stHisVol.lVal;

	stHisVol=Get_History_VolBreakTime_byMPNo(ucMPNo,0,0);
	stHisDayData[ucMeterNo].usHisVolBreakTime=(unsigned short)stHisVol.lVal;
	stHisVol=Get_History_VolBreakTime_byMPNo(ucMPNo,0,1);
	stHisDayData[ucMeterNo].usHisVolABreakTime=(unsigned short)stHisVol.lVal;
	stHisVol=Get_History_VolBreakTime_byMPNo(ucMPNo,0,2);
	stHisDayData[ucMeterNo].usHisVolBBreakTime=(unsigned short)stHisVol.lVal;
	stHisVol=Get_History_VolBreakTime_byMPNo(ucMPNo,0,3);
	stHisDayData[ucMeterNo].usHisVolCBreakTime=(unsigned short)stHisVol.lVal;
	
	stHisTime=Get_History_LastVolBreakBegTime_byMPNo(ucMPNo,0,0);
	stHisDayData[ucMeterNo].stHisLastVolBreakBegTime=stHisTime.stHisTime;
	stHisTime=Get_History_LastVolBreakBegTime_byMPNo(ucMPNo,0,1);
	stHisDayData[ucMeterNo].stHisLastVolABreakBegTime=stHisTime.stHisTime;
	stHisTime=Get_History_LastVolBreakBegTime_byMPNo(ucMPNo,0,2);
	stHisDayData[ucMeterNo].stHisLastVolBBreakBegTime=stHisTime.stHisTime;
	stHisTime=Get_History_LastVolBreakBegTime_byMPNo(ucMPNo,0,3);
	stHisDayData[ucMeterNo].stHisLastVolCBreakBegTime=stHisTime.stHisTime;
	
	stHisTime=Get_History_LastVolBreakEndTime_byMPNo(ucMPNo,0,0);
	stHisDayData[ucMeterNo].stHisLastVolBreakEndTime=stHisTime.stHisTime;
	stHisTime=Get_History_LastVolBreakEndTime_byMPNo(ucMPNo,0,1);
	stHisDayData[ucMeterNo].stHisLastVolABreakEndTime=stHisTime.stHisTime;
	stHisTime=Get_History_LastVolBreakEndTime_byMPNo(ucMPNo,0,2);
	stHisDayData[ucMeterNo].stHisLastVolBBreakEndTime=stHisTime.stHisTime;
	stHisTime=Get_History_LastVolBreakEndTime_byMPNo(ucMPNo,0,3);
	stHisDayData[ucMeterNo].stHisLastVolCBreakEndTime=stHisTime.stHisTime;
	return ucRet;
}

unsigned char DayData_Stat_PowRate(unsigned char ucMeterNo,STDATETIME stDateTime)
{//功率因数分布日统计
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo;
	unsigned long ulTmp=0;
	STPOWRATEPARA	stPowRateParat;
	STHISVAL stHisValP,stHisValQ,stHisVal_Result;
	ucMPNo=RunPara_GetMeter_MPNo(ucMeterNo);
	stPowRateParat=RunPara_GetMPPowRatePara(ucMPNo);
	stHisValP=Get_History_InstantPow_byMPNo(ucMPNo,0,0);
	stHisValQ=Get_History_InstantVar_byMPNo(ucMPNo,0,0);
	stHisVal_Result=stHisValP;
	if(stHisValP.lVal!=CONINVALIDVAL && stHisValQ.lVal!=CONINVALIDVAL)
		stHisVal_Result.lVal = GetPQRate(stHisValP.lVal,stHisValQ.lVal);
	else
		ClearHisVal(&stHisVal_Result);
	if(Is_DateTime_Availble(&(stHisVal_Result.stDateTime))==RET_SUCCESS)
	{
		ulTmp=stHisVal_Result.lVal*10;								//0.85->85->850
		if(ulTmp<stPowRateParat.usPowRateLimit1)
			stHisDayData[ucMeterNo].usHisPowRateSect1Time++;		//功率因数分布区域1累计时间（功率因数 < 定值1）
		else 
		{
			if(ulTmp<stPowRateParat.usPowRateLimit2)
				stHisDayData[ucMeterNo].usHisPowRateSect2Time++;	//功率因数分布区域2累计时间（定值1≤功率因数<定值2） 
			else
				stHisDayData[ucMeterNo].usHisPowRateSect3Time++;	//功率因数分布区域3累计时间（功率因数≥定值2） 
		}
	}
	return ucRet;
}

unsigned char DayData_Stat_TerRunState(unsigned char ucMeterNo,STDATETIME stDateTime)
{//终端运行状态日统计
	unsigned char ucRet=RET_SUCCESS;
	if(ucMeterNo==0)			//保存在0号位置
	{
		stHisDayData[ucMeterNo].usHisTerPowerOnTime++;			//终端日供电时间 
		//外部统计
		//usHisTerResetNum;			//终端日复位累计次数
		//ulHisFluxBytes;			//流量字节数
	}
	return ucRet;
}

unsigned char DayData_Stat_CtrlNum(unsigned char ucMeterNo,STDATETIME stDateTime)
{//控制跳闸日统计
	unsigned char ucRet=RET_SUCCESS;
	if(ucMeterNo==0)			//保存在0号位置
	{
		;//外部统计
		//usHisMonthEnergyCtrlTripNum;	//月电控跳闸次数
		//usHisBuyEnergyCtrlTripNum;	//购电控跳闸次数
		//usHisPowerCtrlTripNum;		//功控跳闸次数
		//usHisRemoteCtrlTripNum;		//遥控跳闸次数
	}
	return ucRet;
}

//---------------- 月统计 ----------------------
unsigned char MonthData_Stat_InstantPow(unsigned char ucMeterNo,STDATETIME stDateTime)
{//功率月统计
	unsigned char ucRet=RET_SUCCESS;
	DataProc_Stat_GetMax(stHisDayData[ucMeterNo].stHisMaxInstantPow,&(stHisMonthData[ucMeterNo].stHisMaxInstantPow),stHisDayData[ucMeterNo].stHisMaxInstantPow.stDateTime);
	DataProc_Stat_GetMax(stHisDayData[ucMeterNo].stHisMaxInstantPowA,&(stHisMonthData[ucMeterNo].stHisMaxInstantPowA),stHisDayData[ucMeterNo].stHisMaxInstantPowA.stDateTime);
	DataProc_Stat_GetMax(stHisDayData[ucMeterNo].stHisMaxInstantPowB,&(stHisMonthData[ucMeterNo].stHisMaxInstantPowB),stHisDayData[ucMeterNo].stHisMaxInstantPowB.stDateTime);
	DataProc_Stat_GetMax(stHisDayData[ucMeterNo].stHisMaxInstantPowC,&(stHisMonthData[ucMeterNo].stHisMaxInstantPowC),stHisDayData[ucMeterNo].stHisMaxInstantPowC.stDateTime);
	stHisMonthData[ucMeterNo].usHisZeroTimeInstantPow+=stHisDayData[ucMeterNo].usHisZeroTimeInstantPow;
	stHisMonthData[ucMeterNo].usHisZeroTimeInstantPowA+=stHisDayData[ucMeterNo].usHisZeroTimeInstantPowA;
	stHisMonthData[ucMeterNo].usHisZeroTimeInstantPowB+=stHisDayData[ucMeterNo].usHisZeroTimeInstantPowB;
	stHisMonthData[ucMeterNo].usHisZeroTimeInstantPowC+=stHisDayData[ucMeterNo].usHisZeroTimeInstantPowC;
	return ucRet;
}

unsigned char MonthData_Stat_Demand(unsigned char ucMeterNo,STDATETIME stDateTime)
{//最大需量月统计
	unsigned char ucRet=RET_SUCCESS;
	DataProc_Stat_GetMax(stHisDayData[ucMeterNo].stHisMaxDemand,&(stHisMonthData[ucMeterNo].stHisMaxDemand),stHisDayData[ucMeterNo].stHisMaxDemand.stDateTime);
	DataProc_Stat_GetMax(stHisDayData[ucMeterNo].stHisMaxDemandA,&(stHisMonthData[ucMeterNo].stHisMaxDemandA),stHisDayData[ucMeterNo].stHisMaxDemandA.stDateTime);
	DataProc_Stat_GetMax(stHisDayData[ucMeterNo].stHisMaxDemandB,&(stHisMonthData[ucMeterNo].stHisMaxDemandB),stHisDayData[ucMeterNo].stHisMaxDemandB.stDateTime);
	DataProc_Stat_GetMax(stHisDayData[ucMeterNo].stHisMaxDemandC,&(stHisMonthData[ucMeterNo].stHisMaxDemandC),stHisDayData[ucMeterNo].stHisMaxDemandC.stDateTime);
	return ucRet;
}

unsigned char MonthData_Stat_VoltStat(unsigned char ucMeterNo,STDATETIME stDateTime)
{//电压月统计
	unsigned char ucRet=RET_SUCCESS;
	DataProc_Stat_GetMax(stHisDayData[ucMeterNo].stHisMaxVolA,&(stHisMonthData[ucMeterNo].stHisMaxVolA),stHisDayData[ucMeterNo].stHisMaxVolA.stDateTime);
	DataProc_Stat_GetMax(stHisDayData[ucMeterNo].stHisMaxVolB,&(stHisMonthData[ucMeterNo].stHisMaxVolB),stHisDayData[ucMeterNo].stHisMaxVolB.stDateTime);
	DataProc_Stat_GetMax(stHisDayData[ucMeterNo].stHisMaxVolC,&(stHisMonthData[ucMeterNo].stHisMaxVolC),stHisDayData[ucMeterNo].stHisMaxVolC.stDateTime);
	DataProc_Stat_GetMin(stHisDayData[ucMeterNo].stHisMinVolA,&(stHisMonthData[ucMeterNo].stHisMinVolA),stHisDayData[ucMeterNo].stHisMinVolA.stDateTime);
	DataProc_Stat_GetMin(stHisDayData[ucMeterNo].stHisMinVolB,&(stHisMonthData[ucMeterNo].stHisMinVolB),stHisDayData[ucMeterNo].stHisMinVolB.stDateTime);
	DataProc_Stat_GetMin(stHisDayData[ucMeterNo].stHisMinVolC,&(stHisMonthData[ucMeterNo].stHisMinVolC),stHisDayData[ucMeterNo].stHisMinVolC.stDateTime);
	stHisMonthData[ucMeterNo].ulHisSumVolA+=stHisDayData[ucMeterNo].ulHisSumVolA;
	stHisMonthData[ucMeterNo].ulHisSumVolB+=stHisDayData[ucMeterNo].ulHisSumVolB;
	stHisMonthData[ucMeterNo].ulHisSumVolC+=stHisDayData[ucMeterNo].ulHisSumVolC;
	stHisMonthData[ucMeterNo].usHisErrUpTimeVolA+=stHisDayData[ucMeterNo].usHisErrUpTimeVolA;
	stHisMonthData[ucMeterNo].usHisErrUpTimeVolB+=stHisDayData[ucMeterNo].usHisErrUpTimeVolB;
	stHisMonthData[ucMeterNo].usHisErrUpTimeVolC+=stHisDayData[ucMeterNo].usHisErrUpTimeVolC;
	stHisMonthData[ucMeterNo].usHisErrDownTimeVolA+=stHisDayData[ucMeterNo].usHisErrDownTimeVolA;
	stHisMonthData[ucMeterNo].usHisErrDownTimeVolB+=stHisDayData[ucMeterNo].usHisErrDownTimeVolB;
	stHisMonthData[ucMeterNo].usHisErrDownTimeVolC+=stHisDayData[ucMeterNo].usHisErrDownTimeVolC;
	stHisMonthData[ucMeterNo].usHisNorUpTimeVolA+=stHisDayData[ucMeterNo].usHisNorUpTimeVolA;
	stHisMonthData[ucMeterNo].usHisNorUpTimeVolB+=stHisDayData[ucMeterNo].usHisNorUpTimeVolB;
	stHisMonthData[ucMeterNo].usHisNorUpTimeVolC+=stHisDayData[ucMeterNo].usHisNorUpTimeVolC;
	stHisMonthData[ucMeterNo].usHisNorDownTimeVolA+=stHisDayData[ucMeterNo].usHisNorDownTimeVolA;
	stHisMonthData[ucMeterNo].usHisNorDownTimeVolB+=stHisDayData[ucMeterNo].usHisNorDownTimeVolB;
	stHisMonthData[ucMeterNo].usHisNorDownTimeVolC+=stHisDayData[ucMeterNo].usHisNorDownTimeVolC;
	stHisMonthData[ucMeterNo].usHisNormalTimeVolA+=stHisDayData[ucMeterNo].usHisNormalTimeVolA;
	stHisMonthData[ucMeterNo].usHisNormalTimeVolB+=stHisDayData[ucMeterNo].usHisNormalTimeVolB;
	stHisMonthData[ucMeterNo].usHisNormalTimeVolC+=stHisDayData[ucMeterNo].usHisNormalTimeVolC;
	return ucRet;
}

unsigned char MonthData_Stat_Unbalance(unsigned char ucMeterNo,STDATETIME stDateTime)
{//不平衡月统计
	unsigned char ucRet=RET_SUCCESS;
	stHisMonthData[ucMeterNo].usHisUnbalanceTimeVolt+=stHisDayData[ucMeterNo].usHisUnbalanceTimeVolt;
	stHisMonthData[ucMeterNo].usHisUnbalanceTimeCurr+=stHisDayData[ucMeterNo].usHisUnbalanceTimeCurr;
	return ucRet;
}

unsigned char MonthData_Stat_CurrOver(unsigned char ucMeterNo,STDATETIME stDateTime)
{//过流月统计
	unsigned char ucRet=RET_SUCCESS;
	DataProc_Stat_GetMax(stHisDayData[ucMeterNo].stHisMaxCurrA,&(stHisMonthData[ucMeterNo].stHisMaxCurrA),stHisDayData[ucMeterNo].stHisMaxCurrA.stDateTime);
	DataProc_Stat_GetMax(stHisDayData[ucMeterNo].stHisMaxCurrB,&(stHisMonthData[ucMeterNo].stHisMaxCurrB),stHisDayData[ucMeterNo].stHisMaxCurrB.stDateTime);
	DataProc_Stat_GetMax(stHisDayData[ucMeterNo].stHisMaxCurrC,&(stHisMonthData[ucMeterNo].stHisMaxCurrC),stHisDayData[ucMeterNo].stHisMaxCurrC.stDateTime);
	DataProc_Stat_GetMax(stHisDayData[ucMeterNo].stHisMaxCurrN,&(stHisMonthData[ucMeterNo].stHisMaxCurrN),stHisDayData[ucMeterNo].stHisMaxCurrN.stDateTime);
	stHisMonthData[ucMeterNo].usHisErrUpTimeCurrA+=stHisDayData[ucMeterNo].usHisErrUpTimeCurrA;
	stHisMonthData[ucMeterNo].usHisErrUpTimeCurrB+=stHisDayData[ucMeterNo].usHisErrUpTimeCurrB;
	stHisMonthData[ucMeterNo].usHisErrUpTimeCurrC+=stHisDayData[ucMeterNo].usHisErrUpTimeCurrC;
	stHisMonthData[ucMeterNo].usHisNorUpTimeCurrA+=stHisDayData[ucMeterNo].usHisNorUpTimeCurrA;
	stHisMonthData[ucMeterNo].usHisNorUpTimeCurrB+=stHisDayData[ucMeterNo].usHisNorUpTimeCurrB;
	stHisMonthData[ucMeterNo].usHisNorUpTimeCurrC+=stHisDayData[ucMeterNo].usHisNorUpTimeCurrC;
	stHisMonthData[ucMeterNo].usHisNorUpTimeCurrN+=stHisDayData[ucMeterNo].usHisNorUpTimeCurrN;
	return ucRet;
}

unsigned char MonthData_Stat_SPowOver(unsigned char ucMeterNo,STDATETIME stDateTime)
{//视在功率越限月统计
	unsigned char ucRet=RET_SUCCESS;
	stHisMonthData[ucMeterNo].usHisErrUpTimeSPower+=stHisDayData[ucMeterNo].usHisErrUpTimeSPower;
	stHisMonthData[ucMeterNo].usHisNorUpTimeSPower+=stHisDayData[ucMeterNo].usHisNorUpTimeSPower;
	return ucRet;
}

unsigned char MonthData_Stat_VoltBreak(unsigned char ucMeterNo,STDATETIME stDateTime)
{//断相月统计，目前次数与日数据直接相等，认为是当前数据
	unsigned char ucRet=RET_SUCCESS;
	stHisMonthData[ucMeterNo].usHisVolBreakNum=stHisDayData[ucMeterNo].usHisVolBreakNum;
	stHisMonthData[ucMeterNo].usHisVolABreakNum=stHisDayData[ucMeterNo].usHisVolABreakNum;
	stHisMonthData[ucMeterNo].usHisVolBBreakNum=stHisDayData[ucMeterNo].usHisVolBBreakNum;
	stHisMonthData[ucMeterNo].usHisVolCBreakNum=stHisDayData[ucMeterNo].usHisVolCBreakNum;
	stHisMonthData[ucMeterNo].usHisVolBreakTime=stHisDayData[ucMeterNo].usHisVolBreakTime;
	stHisMonthData[ucMeterNo].usHisVolABreakTime=stHisDayData[ucMeterNo].usHisVolABreakTime;
	stHisMonthData[ucMeterNo].usHisVolBBreakTime=stHisDayData[ucMeterNo].usHisVolBBreakTime;
	stHisMonthData[ucMeterNo].usHisVolCBreakTime=stHisDayData[ucMeterNo].usHisVolCBreakTime;
	stHisMonthData[ucMeterNo].stHisLastVolBreakBegTime=stHisDayData[ucMeterNo].stHisLastVolBreakBegTime;
	stHisMonthData[ucMeterNo].stHisLastVolABreakBegTime=stHisDayData[ucMeterNo].stHisLastVolABreakBegTime;
	stHisMonthData[ucMeterNo].stHisLastVolBBreakBegTime=stHisDayData[ucMeterNo].stHisLastVolBBreakBegTime;
	stHisMonthData[ucMeterNo].stHisLastVolCBreakBegTime=stHisDayData[ucMeterNo].stHisLastVolCBreakBegTime;
	stHisMonthData[ucMeterNo].stHisLastVolBreakEndTime=stHisDayData[ucMeterNo].stHisLastVolBreakEndTime;
	stHisMonthData[ucMeterNo].stHisLastVolABreakEndTime=stHisDayData[ucMeterNo].stHisLastVolABreakEndTime;
	stHisMonthData[ucMeterNo].stHisLastVolBBreakEndTime=stHisDayData[ucMeterNo].stHisLastVolBBreakEndTime;
	stHisMonthData[ucMeterNo].stHisLastVolCBreakEndTime=stHisDayData[ucMeterNo].stHisLastVolCBreakEndTime;
	return ucRet;
}

unsigned char MonthData_Stat_PowRate(unsigned char ucMeterNo,STDATETIME stDateTime)
{//功率因数分布月统计
	unsigned char ucRet=RET_SUCCESS;
	stHisMonthData[ucMeterNo].usHisPowRateSect1Time+=stHisDayData[ucMeterNo].usHisPowRateSect1Time;
	stHisMonthData[ucMeterNo].usHisPowRateSect2Time+=stHisDayData[ucMeterNo].usHisPowRateSect2Time;
	stHisMonthData[ucMeterNo].usHisPowRateSect3Time+=stHisDayData[ucMeterNo].usHisPowRateSect3Time;
	return ucRet;
}

//每天处理一次
unsigned char MonthData_Stat_TerRunState(unsigned char ucMeterNo,STDATETIME stDateTime)
{//终端运行状态月统计
	unsigned char ucRet=RET_SUCCESS;
	if(ucMeterNo==0)			//保存在0号位置
	{
		stHisMonthData[ucMeterNo].usHisTerPowerOnTime+=stHisDayData[ucMeterNo].usHisTerPowerOnTime;
		stHisMonthData[ucMeterNo].usHisTerResetNum+=stHisDayData[ucMeterNo].usHisTerResetNum;
		stHisMonthData[ucMeterNo].ulHisFluxBytes+=stHisDayData[ucMeterNo].ulHisFluxBytes;
	}
	return ucRet;
}

//unsigned char MonthData_Stat_CtrlNum(unsigned char ucMeterNo,STDATETIME stDateTime)
//{//控制跳闸月统计
//	unsigned char ucRet=RET_SUCCESS;
//	if(ucMeterNo==0)			//保存在0号位置
//	{
//		stHisMonthData[ucMeterNo].usHisMonthEnergyCtrlTripNum+=stHisDayData[ucMeterNo].usHisMonthEnergyCtrlTripNum;
//		stHisMonthData[ucMeterNo].usHisBuyEnergyCtrlTripNum+=stHisDayData[ucMeterNo].usHisBuyEnergyCtrlTripNum;
//		stHisMonthData[ucMeterNo].usHisPowerCtrlTripNum+=stHisDayData[ucMeterNo].usHisPowerCtrlTripNum;
//		stHisMonthData[ucMeterNo].usHisRemoteCtrlTripNum+=stHisDayData[ucMeterNo].usHisRemoteCtrlTripNum;
//	}
//	return ucRet;
//}
//-------------- 统计初始化 ----------------
unsigned char DataProc_Stat_Clear_InstantPow(STHISRUNDATA *pstHisRunData)
{
	unsigned char ucRet=RET_SUCCESS;
	ClearHisVal(&(pstHisRunData->stHisMaxInstantPow));
	ClearHisVal(&(pstHisRunData->stHisMaxInstantPowA));
	ClearHisVal(&(pstHisRunData->stHisMaxInstantPowB));
	ClearHisVal(&(pstHisRunData->stHisMaxInstantPowC));
	pstHisRunData->usHisZeroTimeInstantPow=0;
	pstHisRunData->usHisZeroTimeInstantPowA=0;
	pstHisRunData->usHisZeroTimeInstantPowB=0;
	pstHisRunData->usHisZeroTimeInstantPowC=0;
	return ucRet;
}

unsigned char DataProc_Stat_Clear_Demand(STHISRUNDATA *pstHisRunData)
{
	unsigned char ucRet=RET_SUCCESS;
	ClearHisVal(&(pstHisRunData->stHisMaxDemand));
	ClearHisVal(&(pstHisRunData->stHisMaxDemandA));
	ClearHisVal(&(pstHisRunData->stHisMaxDemandB));
	ClearHisVal(&(pstHisRunData->stHisMaxDemandC));
	return ucRet;
}

unsigned char DataProc_Stat_Clear_VoltStat(STHISRUNDATA *pstHisRunData)
{
	unsigned char ucRet=RET_SUCCESS;
	ClearHisVal(&(pstHisRunData->stHisMaxVolA));
	ClearHisVal(&(pstHisRunData->stHisMaxVolB));
	ClearHisVal(&(pstHisRunData->stHisMaxVolC));
	ClearHisVal(&(pstHisRunData->stHisMinVolA));
	ClearHisVal(&(pstHisRunData->stHisMinVolB));
	ClearHisVal(&(pstHisRunData->stHisMinVolC));
	pstHisRunData->usHisErrUpTimeVolA=0;
	pstHisRunData->usHisErrUpTimeVolB=0;
	pstHisRunData->usHisErrUpTimeVolC=0;
	pstHisRunData->usHisErrDownTimeVolA=0;
	pstHisRunData->usHisErrDownTimeVolB=0;
	pstHisRunData->usHisErrDownTimeVolC=0;
	pstHisRunData->usHisNorUpTimeVolA=0;
	pstHisRunData->usHisNorUpTimeVolB=0;
	pstHisRunData->usHisNorUpTimeVolC=0;
	pstHisRunData->usHisNorDownTimeVolA=0;
	pstHisRunData->usHisNorDownTimeVolB=0;
	pstHisRunData->usHisNorDownTimeVolC=0;
	pstHisRunData->usHisNormalTimeVolA=0;
	pstHisRunData->usHisNormalTimeVolB=0;
	pstHisRunData->usHisNormalTimeVolC=0;
	pstHisRunData->ulHisSumVolA=0;
	pstHisRunData->ulHisSumVolB=0;
	pstHisRunData->ulHisSumVolC=0;
	return ucRet;
}

unsigned char DataProc_Stat_Clear_Unbalance(STHISRUNDATA *pstHisRunData)
{
	unsigned char ucRet=RET_SUCCESS;
	pstHisRunData->usHisUnbalanceTimeVolt=0;
	pstHisRunData->usHisUnbalanceTimeCurr=0;
	ClearHisVal(&(pstHisRunData->stHisMaxUnBalanceVol));
	ClearHisVal(&(pstHisRunData->stHisMaxUnBalanceCur));
	return ucRet;
}

unsigned char DataProc_Stat_Clear_CurrOver(STHISRUNDATA *pstHisRunData)
{
	unsigned char ucRet=RET_SUCCESS;
	ClearHisVal(&(pstHisRunData->stHisMaxCurrA));
	ClearHisVal(&(pstHisRunData->stHisMaxCurrB));
	ClearHisVal(&(pstHisRunData->stHisMaxCurrC));
	ClearHisVal(&(pstHisRunData->stHisMaxCurrN));
	pstHisRunData->usHisErrUpTimeCurrA=0;
	pstHisRunData->usHisErrUpTimeCurrB=0;
	pstHisRunData->usHisErrUpTimeCurrC=0;
	pstHisRunData->usHisNorUpTimeCurrA=0;
	pstHisRunData->usHisNorUpTimeCurrB=0;
	pstHisRunData->usHisNorUpTimeCurrC=0;
	pstHisRunData->usHisNorUpTimeCurrN=0;
	return ucRet;
}

unsigned char DataProc_Stat_Clear_SPowOver(STHISRUNDATA *pstHisRunData)
{
	unsigned char ucRet=RET_SUCCESS;
	pstHisRunData->usHisErrUpTimeSPower=0;
	pstHisRunData->usHisNorUpTimeSPower=0;
	return ucRet;
}

unsigned char DataProc_Stat_Clear_VoltBreak(STHISRUNDATA *pstHisRunData)
{
	unsigned char ucRet=RET_SUCCESS;
	pstHisRunData->usHisVolBreakNum=0;
	pstHisRunData->usHisVolABreakNum=0;
	pstHisRunData->usHisVolBBreakNum=0;
	pstHisRunData->usHisVolCBreakNum=0;
	pstHisRunData->usHisVolBreakTime=0;
	pstHisRunData->usHisVolABreakTime=0;
	pstHisRunData->usHisVolBBreakTime=0;
	pstHisRunData->usHisVolCBreakTime=0;
	ClearDateTime(&(pstHisRunData->stHisLastVolBreakBegTime));//  最近一次断相起始时刻
	ClearDateTime(&(pstHisRunData->stHisLastVolABreakBegTime));//  A相最近一次断相起始时刻
	ClearDateTime(&(pstHisRunData->stHisLastVolBBreakBegTime));//  B相最近一次断相起始时刻
	ClearDateTime(&(pstHisRunData->stHisLastVolCBreakBegTime));//  C相最近一次断相起始时刻
	ClearDateTime(&(pstHisRunData->stHisLastVolBreakEndTime));//  最近一次断相结束时刻
	ClearDateTime(&(pstHisRunData->stHisLastVolABreakEndTime));//  A相最近一次断相结束时刻
	ClearDateTime(&(pstHisRunData->stHisLastVolBBreakEndTime));//  B相最近一次断相结束时刻
	ClearDateTime(&(pstHisRunData->stHisLastVolCBreakEndTime));//  C相最近一次断相结束时刻
	return ucRet;
}

unsigned char DataProc_Stat_Clear_PowRate(STHISRUNDATA *pstHisRunData)
{
	unsigned char ucRet=RET_SUCCESS;
	pstHisRunData->usHisPowRateSect1Time=0;
	pstHisRunData->usHisPowRateSect2Time=0;
	pstHisRunData->usHisPowRateSect3Time=0;
	return ucRet;
}

unsigned char DataProc_Stat_Clear_TerRunState(STHISRUNDATA *pstHisRunData)
{
	unsigned char ucRet=RET_SUCCESS;
	pstHisRunData->usHisTerPowerOnTime=0;
	pstHisRunData->usHisTerResetNum=0;
	pstHisRunData->ulHisFluxBytes=0;
	return ucRet;
}

unsigned char DataProc_Stat_Clear_CtrlNum(STHISRUNDATA *pstHisRunData)
{
	unsigned char ucRet=RET_SUCCESS;
	pstHisRunData->usHisMonthEnergyCtrlTripNum=0;
	pstHisRunData->usHisBuyEnergyCtrlTripNum=0;
	pstHisRunData->usHisPowerCtrlTripNum=0;
	pstHisRunData->usHisRemoteCtrlTripNum=0;
	return ucRet;
}

unsigned char DataProc_Update_MeterData(unsigned char ucMeterNo,STDATETIME stDateTime,STHISRUNDATA *pstHisRunData,unsigned char ucReflashLastDay)
{//抄表数据更新
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo;

	ucMPNo=RunPara_GetMeter_MPNo(ucMeterNo);
	if(ucMPNo!=CON_MPNO_INVALID)
		DataProc_Update_MPData(ucMPNo,stDateTime,pstHisRunData,ucReflashLastDay);

	return ucRet;
}

//unsigned char SumGDataProc_Update_SumGData(unsigned char ucSumGNo,STDATETIME stDateTime,STHISSUMGROUPRUNDATA *pstHisSumGroupRunData,unsigned char ucReflashLastDay)
//{//总加数据更新
//	unsigned char ucRet=RET_SUCCESS;
//	STHISSUMVALGROUP stHisSumValGroup;
//	pstHisSumGroupRunData->stRecordTime=stDateTime;
//
//	stHisSumValGroup=Get_History_PowValSumVal(0,0);			pstHisSumGroupRunData->fSumGroupPowTotal=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//	stHisSumValGroup=Get_History_PowValSumVal(0,1);			pstHisSumGroupRunData->fSumGroupPowFee1=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//	stHisSumValGroup=Get_History_PowValSumVal(0,2);			pstHisSumGroupRunData->fSumGroupPowFee2=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//	stHisSumValGroup=Get_History_PowValSumVal(0,3);			pstHisSumGroupRunData->fSumGroupPowFee3=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//	stHisSumValGroup=Get_History_PowValSumVal(0,4);			pstHisSumGroupRunData->fSumGroupPowFee4=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//
//	stHisSumValGroup=Get_History_VarValSumVal(0,0);			pstHisSumGroupRunData->fSumGroupVarTotal=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//	stHisSumValGroup=Get_History_VarValSumVal(0,1);			pstHisSumGroupRunData->fSumGroupVarFee1=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//	stHisSumValGroup=Get_History_VarValSumVal(0,2);			pstHisSumGroupRunData->fSumGroupVarFee2=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//	stHisSumValGroup=Get_History_VarValSumVal(0,3);			pstHisSumGroupRunData->fSumGroupVarFee3=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//	stHisSumValGroup=Get_History_VarValSumVal(0,4);			pstHisSumGroupRunData->fSumGroupVarFee4=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//
//	if(ucReflashLastDay==CON_STATUS_YES || pstHisSumGroupRunData->fSumGroupLastDayPowTotal==CONINVALIDVAL)
//	{//上一天的数据无效则刷新之
//		pstHisSumGroupRunData->fSumGroupLastDayPowTotal=pstHisSumGroupRunData->fSumGroupPowTotal;
//		pstHisSumGroupRunData->fSumGroupLastDayPowFee1=pstHisSumGroupRunData->fSumGroupPowFee1;
//		pstHisSumGroupRunData->fSumGroupLastDayPowFee2=pstHisSumGroupRunData->fSumGroupPowFee2;
//		pstHisSumGroupRunData->fSumGroupLastDayPowFee3=pstHisSumGroupRunData->fSumGroupPowFee3;
//		pstHisSumGroupRunData->fSumGroupLastDayPowFee4=pstHisSumGroupRunData->fSumGroupPowFee4;
//	}
//
//	if(ucReflashLastDay==CON_STATUS_YES || pstHisSumGroupRunData->fSumGroupLastDayVarTotal==CONINVALIDVAL)
//	{//上一天的数据无效则刷新之
//		pstHisSumGroupRunData->fSumGroupLastDayVarTotal=pstHisSumGroupRunData->fSumGroupVarTotal;
//		pstHisSumGroupRunData->fSumGroupLastDayVarFee1=pstHisSumGroupRunData->fSumGroupVarFee1;
//		pstHisSumGroupRunData->fSumGroupLastDayVarFee2=pstHisSumGroupRunData->fSumGroupVarFee2;
//		pstHisSumGroupRunData->fSumGroupLastDayVarFee3=pstHisSumGroupRunData->fSumGroupVarFee3;
//		pstHisSumGroupRunData->fSumGroupLastDayVarFee4=pstHisSumGroupRunData->fSumGroupVarFee4;
//	}
//	return ucRet;
//}

//unsigned char SumGDataProc_Stat_Clear_InstantPow(STHISSUMGROUPRUNDATA *pstHisSumGroupRunData)
//{//总加功率统计初始化
//	unsigned char ucRet=RET_SUCCESS;
//	ClearDateTime(&(pstHisSumGroupRunData->stSumGroupMaxTimeInstantPow));
//	pstHisSumGroupRunData->fSumGroupMaxInstantPow=CONINVALIDVAL;
//	ClearDateTime(&(pstHisSumGroupRunData->stSumGroupMinTimeInstantPow));
//	pstHisSumGroupRunData->fSumGroupMinInstantPow=CONINVALIDVAL;
//	pstHisSumGroupRunData->usSumGroupZeroTimeInstantPow=0;
//	return ucRet;
//}

//unsigned char SumGDataProc_Stat_Clear_Overflow(STHISSUMGROUPRUNDATA *pstHisSumGroupRunData)
//{//总加越限初始化
//	unsigned char ucRet=RET_SUCCESS;
//	pstHisSumGroupRunData->usSumGroupPowerOverTime=0;
//	pstHisSumGroupRunData->usSumGroupEnergyOverTime=0;
//	pstHisSumGroupRunData->fSumGroupPowerOverAccValTotal=0;
//	pstHisSumGroupRunData->fSumGroupEnergyOverAccValTotal=0;
//	pstHisSumGroupRunData->fSumGroupPowerOverStartValTotal=pstHisSumGroupRunData->fSumGroupPowTotal;
//	pstHisSumGroupRunData->fSumGroupEnergyOverStartValTotal=pstHisSumGroupRunData->fSumGroupPowTotal;
//	return ucRet;
//}
//
//unsigned char SumGDayData_Stat_InstantPow(unsigned char ucSumGNo,STDATETIME stDateTime)
//{//总加功率日统计
//	unsigned char ucRet=RET_SUCCESS;
//	STHISSUMVALGROUP stHisSumValGroup;
//	stHisSumValGroup=Get_History_InstantPowSumVal(0);
//	SumGDataProc_Stat_GetMax(&(stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].fSumGroupMaxInstantPow),
//							&(stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].stSumGroupMaxTimeInstantPow),
//							stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo],
//							stDateTime);
//	SumGDataProc_Stat_GetMin(&(stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].fSumGroupMinInstantPow),
//							&(stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].stSumGroupMinTimeInstantPow),
//							stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo],
//							stDateTime);
//	if(stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo]==0)
//		stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].usSumGroupZeroTimeInstantPow++;			//1分钟检测一次
//	return ucRet;
//}

//unsigned char SumGDayData_Stat_Overflow(unsigned char ucSumGNo,STDATETIME stDateTime)
//{//总加越限日统计
//	unsigned char ucRet=RET_SUCCESS;
//	STLOADFIXVAL	stLoadFixVal;
//	double	dCurrVal = 0;
//	STLOADFIXVAL  stSafeFixVal;
//	double	dCurrFixVal;
//	unsigned char ucPowCurveNo=0,ucEndHour=0,ucEndMinute=0;
//	//功率越限
//	stSafeFixVal=RunPara_GetCtrlSafeFixVal();
//	stLoadFixVal=RunPara_GetPowFixVal(&ucPowCurveNo,&ucEndHour,&ucEndMinute,ucSumGNo);
//	dCurrVal=Load_GetCurrentInstantPow(ucSumGNo);
//	if(stLoadFixVal.dFixVal != CONINVALIDVAL)
//	{
//		if(stLoadFixVal.dFixVal!=CON_POWER_BIGVAL)
//			dCurrFixVal=stLoadFixVal.dFixVal * stLoadFixVal.usFloatCoef/100;
//		else
//			dCurrFixVal=stLoadFixVal.dFixVal;
//		if(dCurrFixVal<stSafeFixVal.dFixVal)
//			dCurrFixVal=stSafeFixVal.dFixVal;			//保安定值，没有浮动系数
//		if(dCurrVal != CONINVALIDVAL)
//		{
//			if( dCurrVal > dCurrFixVal)
//			{
//				stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].usSumGroupPowerOverTime++;			//1分钟检测一次
//				if((stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].fSumGroupPowTotal>stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].fSumGroupPowerOverStartValTotal)
//					&& (stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].fSumGroupPowerOverStartValTotal!=CONINVALIDVAL))
//				{
//					stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].fSumGroupPowerOverAccValTotal+=
//						(stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].fSumGroupPowTotal-stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].fSumGroupPowerOverStartValTotal);
//				}
//				stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].fSumGroupPowerOverStartValTotal=stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].fSumGroupPowTotal;
//			}
//		}
//	}
//	//电量越限
//	dCurrVal=Load_Energy_MonthLeftPowVal(ucSumGNo);
//	if(dCurrVal<0)
//	{
//		stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].usSumGroupEnergyOverTime++;			//1分钟检测一次
//		if((stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].fSumGroupPowTotal>stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].fSumGroupEnergyOverStartValTotal)
//			&& (stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].fSumGroupEnergyOverStartValTotal!=CONINVALIDVAL))
//		{
//			stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].fSumGroupPowerOverAccValTotal+=
//				(stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].fSumGroupPowTotal-stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].fSumGroupEnergyOverStartValTotal);
//		}
//		stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].fSumGroupEnergyOverStartValTotal=stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].fSumGroupPowTotal;
//	}
//	return ucRet;
//}

//unsigned char SumGMonthData_Stat_InstantPow(unsigned char ucSumGNo,STDATETIME stDateTime)
//{//总加功率月统计
//	unsigned char ucRet=RET_SUCCESS;
//	SumGDataProc_Stat_GetMax(&(stHisSumGroupThisMonth.stHisSumGroupRunData[ucSumGNo].fSumGroupMaxInstantPow),
//							&(stHisSumGroupThisMonth.stHisSumGroupRunData[ucSumGNo].stSumGroupMaxTimeInstantPow),
//							stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].fSumGroupMaxInstantPow,
//							stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].stSumGroupMaxTimeInstantPow);
//	SumGDataProc_Stat_GetMin(&(stHisSumGroupThisMonth.stHisSumGroupRunData[ucSumGNo].fSumGroupMinInstantPow),
//							&(stHisSumGroupThisMonth.stHisSumGroupRunData[ucSumGNo].stSumGroupMinTimeInstantPow),
//							stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].fSumGroupMinInstantPow,
//							stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].stSumGroupMinTimeInstantPow);
//	stHisSumGroupThisMonth.stHisSumGroupRunData[ucSumGNo].usSumGroupZeroTimeInstantPow+=stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].usSumGroupZeroTimeInstantPow;
// 	return ucRet;
// }

//unsigned char SumGMonthData_Stat_Overflow(unsigned char ucSumGNo,STDATETIME stDateTime)
//{//总加越限月统计
//	unsigned char ucRet=RET_SUCCESS;
//	stHisSumGroupThisMonth.stHisSumGroupRunData[ucSumGNo].fSumGroupPowerOverAccValTotal+=stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].fSumGroupPowerOverAccValTotal;
//	stHisSumGroupThisMonth.stHisSumGroupRunData[ucSumGNo].fSumGroupEnergyOverAccValTotal+=stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].fSumGroupEnergyOverAccValTotal;
//	stHisSumGroupThisMonth.stHisSumGroupRunData[ucSumGNo].usSumGroupPowerOverTime+=stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].usSumGroupPowerOverTime;
//	stHisSumGroupThisMonth.stHisSumGroupRunData[ucSumGNo].usSumGroupEnergyOverTime+=stHisSumGroupThisDay.stHisSumGroupRunData[ucSumGNo].usSumGroupEnergyOverTime;
//	return ucRet;
//}

//------------------- 其他辅助函数 -------------------
void DataProc_Stat_TerResetNum_Inc(void)
{
	stHisDayData[0].usHisTerResetNum++;
//	stHisTerDataThisDay.usHisTerResetNum++;
}

//void DataProc_Stat_MonthEnerCtrlNum_Inc(void)
//{//月电控跳闸次数
//	stHisDayData[0].usHisMonthEnergyCtrlTripNum++;
//	stHisTerDataThisDay.usHisMonthEnergyCtrlTripNum++;
//}

//void DataProc_Stat_BuyEnerCtrlNum_Inc(void)
//{//购电控跳闸次数
//	stHisDayData[0].usHisBuyEnergyCtrlTripNum++;
//	stHisTerDataThisDay.usHisBuyEnergyCtrlTripNum++;
//}

//void DataProc_Stat_PowCtrlNum_Inc(void)
//{//功控跳闸次数
//	stHisDayData[0].usHisPowerCtrlTripNum++;
//	stHisTerDataThisDay.usHisPowerCtrlTripNum++;
//}

//void DataProc_Stat_RemoteCtrlNum_Inc(void)
//{//遥控跳闸次数
//	stHisDayData[0].usHisRemoteCtrlTripNum++;
//	stHisTerDataThisDay.usHisRemoteCtrlTripNum++;
//}

void DataProc_Stat_FluxBytes_Inc(unsigned long ulBytes)
{//流量增加
	unsigned long ulLimitBytes,ulLastBytes,ulCurrBytes;
//	ulLastBytes=stHisTerDataThisMonth.ulHisFluxBytes+stHisTerDataThisDay.ulHisFluxBytes;	//日流量改为月流量
////	ulLastBytes=stHisTerDataThisDay.ulHisFluxBytes;//月流量改为日流量
//azh
    ulLastBytes=stHisMonthData[0].ulHisFluxBytes+stHisDayData[0].ulHisFluxBytes;	//日流量改为月流量
	ulCurrBytes=ulLastBytes+ulBytes;
	stHisDayData[0].ulHisFluxBytes += ulBytes;
//	stHisTerDataThisDay.ulHisFluxBytes += ulBytes;
	ulLimitBytes=RunPara_GetTerMisc_FluxLimit();
	if(ulLimitBytes!=0)
	{
		if(Main_RComm_Get_OverFlux_AlrStatus()==CON_STATUS_YES)
		{//已经处于流量越限告警状态
			if(ulCurrBytes<ulLimitBytes)
			{//越限告警恢复，如跨日，如定值改变
				Main_RComm_Set_OverFlux_Logout(0xFF);			//0xFF表示无告警状态
				Main_RComm_UpStart();
			}
		}

		if((ulLastBytes<=ulLimitBytes) && (ulCurrBytes>ulLimitBytes))
		{//超限要下线。要告警，上送后延迟1分钟下线
			//告警
//azh			Alarm_EnsureJudge_OverFluxLimit(ulCurrBytes,ulLimitBytes);
			//设置标志，延时下线
//			Main_RComm_Set_OverFlux_Logout(100);				//50秒后下线
		}
	}
}

void DataProc_Stat_FluxBytes_LimitChange(unsigned long ulOldLimit,unsigned long ulNewLimit)
{//流量增加
	unsigned long ulCurrBytes;
//	ulCurrBytes=stHisTerDataThisDay.ulHisFluxBytes;
//	ulCurrBytes=stHisTerDataThisMonth.ulHisFluxBytes;
//azh
	ulCurrBytes=stHisMonthData[0].ulHisFluxBytes;

	if(ulNewLimit!=0)
	{
		if(Main_RComm_Get_OverFlux_AlrStatus()==CON_STATUS_YES)
		{//已经处于流量越限告警状态
			if(ulCurrBytes<ulNewLimit)
			{//越限告警恢复，如跨日，如定值改变
				Main_RComm_Set_OverFlux_Logout(0xFF);			//0xFF表示无告警状态
				Main_RComm_UpStart();
			}
		}

		if((ulCurrBytes<=ulOldLimit) && (ulCurrBytes>ulNewLimit))
		{//超限要下线。要告警，上送后延迟1分钟下线
			//告警
//azh			Alarm_EnsureJudge_OverFluxLimit(ulCurrBytes,ulNewLimit);
			//设置标志，延时下线
//			Main_RComm_Set_OverFlux_Logout(100);				//50秒后下线
		}
	}
}

//------------------------- 终端 ---------------
//unsigned char DayData_Stat_InstantPow_Ter(STDATETIME stDateTime)
//{//最大功率日统计
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucMPNo;
//	STHISVAL stHisVal;
//	ucMPNo=RunPara_GetTer_MPNo();
//	stHisVal=Get_History_InstantPow_byMPNo(ucMPNo,0,0);
//	DataProc_Stat_GetMax(stHisVal,&(stHisTerDataThisDay.stHisMaxInstantPow),stDateTime);
//	if(stHisVal.lVal==0)
//		stHisTerDataThisDay.usHisZeroTimeInstantPow++;			//1分钟检测一次
//	stHisVal=Get_History_InstantPow_byMPNo(ucMPNo,0,1);
//	DataProc_Stat_GetMax(stHisVal,&(stHisTerDataThisDay.stHisMaxInstantPowA),stDateTime);
//	if(stHisVal.lVal==0)
//		stHisTerDataThisDay.usHisZeroTimeInstantPowA++;
//	stHisVal=Get_History_InstantPow_byMPNo(ucMPNo,0,2);
//	DataProc_Stat_GetMax(stHisVal,&(stHisTerDataThisDay.stHisMaxInstantPowB),stDateTime);
//	if(stHisVal.lVal==0)
//		stHisTerDataThisDay.usHisZeroTimeInstantPowB++;
//	stHisVal=Get_History_InstantPow_byMPNo(ucMPNo,0,3);
//	DataProc_Stat_GetMax(stHisVal,&(stHisTerDataThisDay.stHisMaxInstantPowC),stDateTime);
//	if(stHisVal.lVal==0)
//		stHisTerDataThisDay.usHisZeroTimeInstantPowC++;
//	return ucRet;
//}

//unsigned char DayData_Stat_Demand_Ter(STDATETIME stDateTime)
//{//最大需量日统计
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucMPNo;
//	STHISVAL stHisVal;
//	ucMPNo=RunPara_GetTer_MPNo();
//	stHisVal=Get_History_InstantPow_byMPNo(ucMPNo,0,0);
//	DataProc_Stat_GetMax(stHisVal,&(stHisTerDataThisDay.stHisMaxDemand),stDateTime);
//	stHisVal=Get_History_InstantPow_byMPNo(ucMPNo,0,1);
//	DataProc_Stat_GetMax(stHisVal,&(stHisTerDataThisDay.stHisMaxDemandA),stDateTime);
//	stHisVal=Get_History_InstantPow_byMPNo(ucMPNo,0,2);
//	DataProc_Stat_GetMax(stHisVal,&(stHisTerDataThisDay.stHisMaxDemandB),stDateTime);
//	stHisVal=Get_History_InstantPow_byMPNo(ucMPNo,0,3);
//	DataProc_Stat_GetMax(stHisVal,&(stHisTerDataThisDay.stHisMaxDemandC),stDateTime);
//	return ucRet;
//}

//unsigned char DayData_Stat_VoltStat_Ter(STDATETIME stDateTime)
//{//电压日统计
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucMPNo;
//	STMPLIMIT		stMPLimit;
//	STHISVAL stHisVol;
//	ucMPNo=RunPara_GetTer_MPNo();
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	stHisVol=Get_History_V_byMPNo(ucMPNo,0,1);
//	stHisVol.lVal = stHisVol.lVal/10;//取整数
//	DataProc_Stat_GetMax(stHisVol,&(stHisTerDataThisDay.stHisMaxVolA),stDateTime);
//	DataProc_Stat_GetMin(stHisVol,&(stHisTerDataThisDay.stHisMinVolA),stDateTime);
//	if(Is_DateTime_Availble(&(stHisVol.stDateTime))==RET_SUCCESS && stHisVol.lVal!=CONINVALIDVAL)
//	{
//		stHisTerDataThisDay.ulHisSumVolA+=stHisVol.lVal;
//		if(stHisVol.lVal>stMPLimit.usVolErrUpLimit)
//			stHisTerDataThisDay.usHisErrUpTimeVolA++;			//1分钟检测一次
//		if(stHisVol.lVal<stMPLimit.usVolErrDownLimit)
//			stHisTerDataThisDay.usHisErrDownTimeVolA++;
//		if(stHisVol.lVal>stMPLimit.usVolUpLimit)
//			stHisTerDataThisDay.usHisNorUpTimeVolA++;
//		else
//		{
//			if(stHisVol.lVal<stMPLimit.usVolDownLimit)
//				stHisTerDataThisDay.usHisNorDownTimeVolA++;
//			else
//				stHisTerDataThisDay.usHisNormalTimeVolA++;
//		}
//	}
//	stHisVol=Get_History_V_byMPNo(ucMPNo,0,2);
//	stHisVol.lVal = stHisVol.lVal/10;//取整数
//	DataProc_Stat_GetMax(stHisVol,&(stHisTerDataThisDay.stHisMaxVolB),stDateTime);
//	DataProc_Stat_GetMin(stHisVol,&(stHisTerDataThisDay.stHisMinVolB),stDateTime);
//	if(Is_DateTime_Availble(&(stHisVol.stDateTime))==RET_SUCCESS && stHisVol.lVal!=CONINVALIDVAL)
//	{
//		stHisTerDataThisDay.ulHisSumVolB+=stHisVol.lVal;
//		if(stHisVol.lVal>stMPLimit.usVolErrUpLimit)
//			stHisTerDataThisDay.usHisErrUpTimeVolB++;			//1分钟检测一次
//		if(stHisVol.lVal<stMPLimit.usVolErrDownLimit)
//			stHisTerDataThisDay.usHisErrDownTimeVolB++;
//		if(stHisVol.lVal>stMPLimit.usVolUpLimit)
//			stHisTerDataThisDay.usHisNorUpTimeVolB++;
//		else
//		{
//			if(stHisVol.lVal<stMPLimit.usVolDownLimit)
//				stHisTerDataThisDay.usHisNorDownTimeVolB++;
//			else
//				stHisTerDataThisDay.usHisNormalTimeVolB++;
//		}
//	}
//	stHisVol=Get_History_V_byMPNo(ucMPNo,0,3);
//	stHisVol.lVal = stHisVol.lVal/10;//取整数
//	DataProc_Stat_GetMax(stHisVol,&(stHisTerDataThisDay.stHisMaxVolC),stDateTime);
//	DataProc_Stat_GetMin(stHisVol,&(stHisTerDataThisDay.stHisMinVolC),stDateTime);
//	if(Is_DateTime_Availble(&(stHisVol.stDateTime))==RET_SUCCESS && stHisVol.lVal!=CONINVALIDVAL)
//	{
//		stHisTerDataThisDay.ulHisSumVolC+=stHisVol.lVal;
//		if(stHisVol.lVal>stMPLimit.usVolErrUpLimit)
//			stHisTerDataThisDay.usHisErrUpTimeVolC++;			//1分钟检测一次
//		if(stHisVol.lVal<stMPLimit.usVolErrDownLimit)
//			stHisTerDataThisDay.usHisErrDownTimeVolC++;
//		if(stHisVol.lVal>stMPLimit.usVolUpLimit)
//			stHisTerDataThisDay.usHisNorUpTimeVolC++;
//		else
//		{
//			if(stHisVol.lVal<stMPLimit.usVolDownLimit)
//				stHisTerDataThisDay.usHisNorDownTimeVolC++;
//			else
//				stHisTerDataThisDay.usHisNormalTimeVolC++;
//		}
//	}
//	return ucRet;
//}

//unsigned char DayData_Stat_Unbalance_Ter(STDATETIME stDateTime)
//{//不平衡度日统计
//	unsigned char ucRet=RET_SUCCESS;
//azh 	unsigned char ucMPNo;
// 	unsigned long ulVal;
// 	STMPLIMIT	stMPLimit;
// 	STMPRATEVAL	stMPRateVal;
// 	STHISVAL stHisValA,stHisValB,stHisValC;
// 	ucMPNo=RunPara_GetTer_MPNo();
// 	stMPLimit=RunPara_GetMPLimit(ucMPNo);
// 	stMPRateVal=RunPara_GetMPRateVal(ucMPNo);
// 	stHisValA=Get_History_I_byMPNo(ucMPNo,0,1);
// 	stHisValB=Get_History_I_byMPNo(ucMPNo,0,2);
// 	stHisValC=Get_History_I_byMPNo(ucMPNo,0,3);
// 	ulVal=GetUnBalanceVal_byVal(stHisValA.lVal,stHisValB.lVal,stHisValC.lVal,stMPRateVal.ucMPLineType);
// 	if(ulVal!=0xFFFFFFFF)
// 	{
// 		if(ulVal>stMPLimit.usCurUnBalance)
// 			stHisTerDataThisDay.usHisUnbalanceTimeCurr++;
// 	}
// 	stHisValA=Get_History_V_byMPNo(ucMPNo,0,1);
// 	stHisValB=Get_History_V_byMPNo(ucMPNo,0,2);
// 	stHisValC=Get_History_V_byMPNo(ucMPNo,0,3);
// 	ulVal=GetUnBalanceVal_byVal(stHisValA.lVal,stHisValB.lVal,stHisValC.lVal,stMPRateVal.ucMPLineType);
// 	if(ulVal!=0xFFFFFFFF)
// 	{
// 		if(ulVal>stMPLimit.usVolUnBalance)
// 			stHisTerDataThisDay.usHisUnbalanceTimeVolt++;
// 	}
//	return ucRet;
//}

//unsigned char DayData_Stat_CurrOver_Ter(STDATETIME stDateTime)
//{//过流日统计
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucMPNo;
//	STMPLIMIT		stMPLimit;
//	STHISVAL stHisVol;
//	ucMPNo=RunPara_GetTer_MPNo();
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	stHisVol=Get_History_I_byMPNo(ucMPNo,0,1);
//	DataProc_Stat_GetMax(stHisVol,&(stHisTerDataThisDay.stHisMaxCurrA),stDateTime);
//	if(Is_DateTime_Availble(&(stHisVol.stDateTime))==RET_SUCCESS)
//	{
//		if(stHisVol.lVal>stMPLimit.usCurErrUpLimit)
//			stHisTerDataThisDay.usHisErrUpTimeCurrA++;
//		if(stHisVol.lVal>stMPLimit.usCurUpLimit)
//			stHisTerDataThisDay.usHisNorUpTimeCurrA++;
//	}
//	stHisVol=Get_History_I_byMPNo(ucMPNo,0,2);
//	DataProc_Stat_GetMax(stHisVol,&(stHisTerDataThisDay.stHisMaxCurrB),stDateTime);
//	if(Is_DateTime_Availble(&(stHisVol.stDateTime))==RET_SUCCESS)
//	{
//		if(stHisVol.lVal>stMPLimit.usCurErrUpLimit)
//			stHisTerDataThisDay.usHisErrUpTimeCurrB++;
//		if(stHisVol.lVal>stMPLimit.usCurUpLimit)
//			stHisTerDataThisDay.usHisNorUpTimeCurrB++;
//	}
//	stHisVol=Get_History_I_byMPNo(ucMPNo,0,3);
//	DataProc_Stat_GetMax(stHisVol,&(stHisTerDataThisDay.stHisMaxCurrC),stDateTime);
//	if(Is_DateTime_Availble(&(stHisVol.stDateTime))==RET_SUCCESS)
//	{
//		if(stHisVol.lVal>stMPLimit.usCurErrUpLimit)
//			stHisTerDataThisDay.usHisErrUpTimeCurrC++;
//		if(stHisVol.lVal>stMPLimit.usCurUpLimit)
//			stHisTerDataThisDay.usHisNorUpTimeCurrC++;
//	}
//	//零序电流
//	return ucRet;
//}
//
//unsigned char DayData_Stat_SPowOver_Ter(STDATETIME stDateTime)
//{//视在功率越限日统计
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucMPNo;
//	STMPLIMIT		stMPLimit;
//	STHISVAL stHisValP,stHisValQ,stHisValS;
//	ucMPNo=RunPara_GetTer_MPNo();
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	stHisValP=Get_History_InstantPow_byMPNo(ucMPNo,0,0);
//	stHisValQ=Get_History_InstantVar_byMPNo(ucMPNo,0,0);
//	stHisValS=stHisValP;
//	if(stHisValP.lVal!=CONINVALIDVAL && stHisValQ.lVal!=CONINVALIDVAL)
//		stHisValS.lVal = GetSPow(stHisValP.lVal,stHisValQ.lVal);
//	else
//		ClearHisVal(&stHisValS);
//	if(Is_DateTime_Availble(&(stHisValS.stDateTime))==RET_SUCCESS)
//	{
//		if(stHisValS.lVal>stMPLimit.ulPowErrUpLimit)
//			stHisTerDataThisDay.usHisErrUpTimeSPower++;
//		if(stHisValS.lVal>stMPLimit.ulPowUpLimit)
//			stHisTerDataThisDay.usHisNorUpTimeSPower++;
//	}
//	return ucRet;
//}
//
//unsigned char DayData_Stat_VoltBreak_Ter(STDATETIME stDateTime)
//{//断相日统计
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucMPNo;
//	STHISVAL stHisVol;
//	STHISTIME stHisTime;
//	ucMPNo=RunPara_GetTer_MPNo();
//	stHisVol=Get_History_VolBreakNum_byMPNo(ucMPNo,0,0);
//	stHisTerDataThisDay.usHisVolBreakNum=(unsigned short)stHisVol.lVal;
//	stHisVol=Get_History_VolBreakNum_byMPNo(ucMPNo,0,1);
//	stHisTerDataThisDay.usHisVolABreakNum=(unsigned short)stHisVol.lVal;
//	stHisVol=Get_History_VolBreakNum_byMPNo(ucMPNo,0,2);
//	stHisTerDataThisDay.usHisVolBBreakNum=(unsigned short)stHisVol.lVal;
//	stHisVol=Get_History_VolBreakNum_byMPNo(ucMPNo,0,3);
//	stHisTerDataThisDay.usHisVolCBreakNum=(unsigned short)stHisVol.lVal;
//
//	stHisVol=Get_History_VolBreakTime_byMPNo(ucMPNo,0,0);
//	stHisTerDataThisDay.usHisVolBreakTime=(unsigned short)stHisVol.lVal;
//	stHisVol=Get_History_VolBreakTime_byMPNo(ucMPNo,0,1);
//	stHisTerDataThisDay.usHisVolABreakTime=(unsigned short)stHisVol.lVal;
//	stHisVol=Get_History_VolBreakTime_byMPNo(ucMPNo,0,2);
//	stHisTerDataThisDay.usHisVolBBreakTime=(unsigned short)stHisVol.lVal;
//	stHisVol=Get_History_VolBreakTime_byMPNo(ucMPNo,0,3);
//	stHisTerDataThisDay.usHisVolCBreakTime=(unsigned short)stHisVol.lVal;
//	
//	stHisTime=Get_History_LastVolBreakBegTime_byMPNo(ucMPNo,0,0);
//	stHisTerDataThisDay.stHisLastVolBreakBegTime=stHisTime.stHisTime;
//	stHisTime=Get_History_LastVolBreakBegTime_byMPNo(ucMPNo,0,1);
//	stHisTerDataThisDay.stHisLastVolABreakBegTime=stHisTime.stHisTime;
//	stHisTime=Get_History_LastVolBreakBegTime_byMPNo(ucMPNo,0,2);
//	stHisTerDataThisDay.stHisLastVolBBreakBegTime=stHisTime.stHisTime;
//	stHisTime=Get_History_LastVolBreakBegTime_byMPNo(ucMPNo,0,3);
//	stHisTerDataThisDay.stHisLastVolCBreakBegTime=stHisTime.stHisTime;
//	
//	stHisTime=Get_History_LastVolBreakEndTime_byMPNo(ucMPNo,0,0);
//	stHisTerDataThisDay.stHisLastVolBreakEndTime=stHisTime.stHisTime;
//	stHisTime=Get_History_LastVolBreakEndTime_byMPNo(ucMPNo,0,1);
//	stHisTerDataThisDay.stHisLastVolABreakEndTime=stHisTime.stHisTime;
//	stHisTime=Get_History_LastVolBreakEndTime_byMPNo(ucMPNo,0,2);
//	stHisTerDataThisDay.stHisLastVolBBreakEndTime=stHisTime.stHisTime;
//	stHisTime=Get_History_LastVolBreakEndTime_byMPNo(ucMPNo,0,3);
//	stHisTerDataThisDay.stHisLastVolCBreakEndTime=stHisTime.stHisTime;
//	return ucRet;
//}
//
//unsigned char DayData_Stat_PowRate_Ter(STDATETIME stDateTime)
//{//功率因数分布日统计
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucMPNo;
//	unsigned long ulTmp=0;
//	STPOWRATEPARA	stPowRateParat;
//	STHISVAL stHisValP,stHisValQ,stHisVal_Result;
//	ucMPNo=RunPara_GetTer_MPNo();
//	stPowRateParat=RunPara_GetMPPowRatePara(ucMPNo);
//	stHisValP=Get_History_InstantPow_byMPNo(ucMPNo,0,0);
//	stHisValQ=Get_History_InstantVar_byMPNo(ucMPNo,0,0);
//	stHisVal_Result=stHisValP;
//	if(stHisValP.lVal!=CONINVALIDVAL && stHisValQ.lVal!=CONINVALIDVAL)
//		stHisVal_Result.lVal = GetPQRate(stHisValP.lVal,stHisValQ.lVal);
//	else
//		ClearHisVal(&stHisVal_Result);
//	if(Is_DateTime_Availble(&(stHisVal_Result.stDateTime))==RET_SUCCESS)
//	{
//		ulTmp=stHisVal_Result.lVal*10;								//0.85->85->850
//		if(ulTmp<stPowRateParat.usPowRateLimit1)
//			stHisTerDataThisDay.usHisPowRateSect1Time++;		//功率因数分布区域1累计时间（功率因数 < 定值1）
//		else 
//		{
//			if(ulTmp<stPowRateParat.usPowRateLimit2)
//				stHisTerDataThisDay.usHisPowRateSect2Time++;	//功率因数分布区域2累计时间（定值1≤功率因数<定值2） 
//			else
//				stHisTerDataThisDay.usHisPowRateSect3Time++;	//功率因数分布区域3累计时间（功率因数≥定值2） 
//		}
//	}
//	return ucRet;
//}

//unsigned char DayData_Stat_TerRunState_Ter(STDATETIME stDateTime)
//{//终端运行状态日统计
//	unsigned char ucRet=RET_SUCCESS;
//	stHisTerDataThisDay.usHisTerPowerOnTime++;			//终端日供电时间 
//	//外部统计
//	//usHisTerResetNum;			//终端日复位累计次数
//	//ulHisFluxBytes;			//流量字节数
//	return ucRet;
//}

//unsigned char DayData_Stat_CtrlNum_Ter(STDATETIME stDateTime)
//{//控制跳闸日统计
//	unsigned char ucRet=RET_SUCCESS;
//	;//外部统计
//	//usHisMonthEnergyCtrlTripNum;	//月电控跳闸次数
//	//usHisBuyEnergyCtrlTripNum;	//购电控跳闸次数
//	//usHisPowerCtrlTripNum;		//功控跳闸次数
//	//usHisRemoteCtrlTripNum;		//遥控跳闸次数
//	return ucRet;
//}

//unsigned char MonthData_Stat_InstantPow_Ter(STDATETIME stDateTime)
//{//功率月统计
//	unsigned char ucRet=RET_SUCCESS;
//	DataProc_Stat_GetMax(stHisTerDataThisDay.stHisMaxInstantPow,&(stHisTerDataThisMonth.stHisMaxInstantPow),stHisTerDataThisDay.stHisMaxInstantPow.stDateTime);
//	DataProc_Stat_GetMax(stHisTerDataThisDay.stHisMaxInstantPowA,&(stHisTerDataThisMonth.stHisMaxInstantPowA),stHisTerDataThisDay.stHisMaxInstantPowA.stDateTime);
//	DataProc_Stat_GetMax(stHisTerDataThisDay.stHisMaxInstantPowB,&(stHisTerDataThisMonth.stHisMaxInstantPowB),stHisTerDataThisDay.stHisMaxInstantPowB.stDateTime);
//	DataProc_Stat_GetMax(stHisTerDataThisDay.stHisMaxInstantPowC,&(stHisTerDataThisMonth.stHisMaxInstantPowC),stHisTerDataThisDay.stHisMaxInstantPowC.stDateTime);
//	stHisTerDataThisMonth.usHisZeroTimeInstantPow+=stHisTerDataThisDay.usHisZeroTimeInstantPow;
//	stHisTerDataThisMonth.usHisZeroTimeInstantPowA+=stHisTerDataThisDay.usHisZeroTimeInstantPowA;
//	stHisTerDataThisMonth.usHisZeroTimeInstantPowB+=stHisTerDataThisDay.usHisZeroTimeInstantPowB;
//	stHisTerDataThisMonth.usHisZeroTimeInstantPowC+=stHisTerDataThisDay.usHisZeroTimeInstantPowC;
//	return ucRet;
//}
//
//unsigned char MonthData_Stat_Demand_Ter(STDATETIME stDateTime)
//{//最大需量月统计
//	unsigned char ucRet=RET_SUCCESS;
//	DataProc_Stat_GetMax(stHisTerDataThisDay.stHisMaxDemand,&(stHisTerDataThisMonth.stHisMaxDemand),stHisTerDataThisDay.stHisMaxDemand.stDateTime);
//	DataProc_Stat_GetMax(stHisTerDataThisDay.stHisMaxDemandA,&(stHisTerDataThisMonth.stHisMaxDemandA),stHisTerDataThisDay.stHisMaxDemandA.stDateTime);
//	DataProc_Stat_GetMax(stHisTerDataThisDay.stHisMaxDemandB,&(stHisTerDataThisMonth.stHisMaxDemandB),stHisTerDataThisDay.stHisMaxDemandB.stDateTime);
//	DataProc_Stat_GetMax(stHisTerDataThisDay.stHisMaxDemandC,&(stHisTerDataThisMonth.stHisMaxDemandC),stHisTerDataThisDay.stHisMaxDemandC.stDateTime);
//	return ucRet;
//}
//
//unsigned char MonthData_Stat_VoltStat_Ter(STDATETIME stDateTime)
//{//电压月统计
//	unsigned char ucRet=RET_SUCCESS;
//	DataProc_Stat_GetMax(stHisTerDataThisDay.stHisMaxVolA,&(stHisTerDataThisMonth.stHisMaxVolA),stHisTerDataThisDay.stHisMaxVolA.stDateTime);
//	DataProc_Stat_GetMax(stHisTerDataThisDay.stHisMaxVolB,&(stHisTerDataThisMonth.stHisMaxVolB),stHisTerDataThisDay.stHisMaxVolB.stDateTime);
//	DataProc_Stat_GetMax(stHisTerDataThisDay.stHisMaxVolC,&(stHisTerDataThisMonth.stHisMaxVolC),stHisTerDataThisDay.stHisMaxVolC.stDateTime);
//	DataProc_Stat_GetMin(stHisTerDataThisDay.stHisMinVolA,&(stHisTerDataThisMonth.stHisMinVolA),stHisTerDataThisDay.stHisMinVolA.stDateTime);
//	DataProc_Stat_GetMin(stHisTerDataThisDay.stHisMinVolB,&(stHisTerDataThisMonth.stHisMinVolB),stHisTerDataThisDay.stHisMinVolB.stDateTime);
//	DataProc_Stat_GetMin(stHisTerDataThisDay.stHisMinVolC,&(stHisTerDataThisMonth.stHisMinVolC),stHisTerDataThisDay.stHisMinVolC.stDateTime);
//	stHisTerDataThisMonth.ulHisSumVolA+=stHisTerDataThisDay.ulHisSumVolA;
//	stHisTerDataThisMonth.ulHisSumVolB+=stHisTerDataThisDay.ulHisSumVolB;
//	stHisTerDataThisMonth.ulHisSumVolC+=stHisTerDataThisDay.ulHisSumVolC;
//	stHisTerDataThisMonth.usHisErrUpTimeVolA+=stHisTerDataThisDay.usHisErrUpTimeVolA;
//	stHisTerDataThisMonth.usHisErrUpTimeVolB+=stHisTerDataThisDay.usHisErrUpTimeVolB;
//	stHisTerDataThisMonth.usHisErrUpTimeVolC+=stHisTerDataThisDay.usHisErrUpTimeVolC;
//	stHisTerDataThisMonth.usHisErrDownTimeVolA+=stHisTerDataThisDay.usHisErrDownTimeVolA;
//	stHisTerDataThisMonth.usHisErrDownTimeVolB+=stHisTerDataThisDay.usHisErrDownTimeVolB;
//	stHisTerDataThisMonth.usHisErrDownTimeVolC+=stHisTerDataThisDay.usHisErrDownTimeVolC;
//	stHisTerDataThisMonth.usHisNorUpTimeVolA+=stHisTerDataThisDay.usHisNorUpTimeVolA;
//	stHisTerDataThisMonth.usHisNorUpTimeVolB+=stHisTerDataThisDay.usHisNorUpTimeVolB;
//	stHisTerDataThisMonth.usHisNorUpTimeVolC+=stHisTerDataThisDay.usHisNorUpTimeVolC;
//	stHisTerDataThisMonth.usHisNorDownTimeVolA+=stHisTerDataThisDay.usHisNorDownTimeVolA;
//	stHisTerDataThisMonth.usHisNorDownTimeVolB+=stHisTerDataThisDay.usHisNorDownTimeVolB;
//	stHisTerDataThisMonth.usHisNorDownTimeVolC+=stHisTerDataThisDay.usHisNorDownTimeVolC;
//	stHisTerDataThisMonth.usHisNormalTimeVolA+=stHisTerDataThisDay.usHisNormalTimeVolA;
//	stHisTerDataThisMonth.usHisNormalTimeVolB+=stHisTerDataThisDay.usHisNormalTimeVolB;
//	stHisTerDataThisMonth.usHisNormalTimeVolC+=stHisTerDataThisDay.usHisNormalTimeVolC;
//	return ucRet;
//}
//
//unsigned char MonthData_Stat_Unbalance_Ter(STDATETIME stDateTime)
//{//不平衡月统计
//	unsigned char ucRet=RET_SUCCESS;
//	stHisTerDataThisMonth.usHisUnbalanceTimeVolt+=stHisTerDataThisDay.usHisUnbalanceTimeVolt;
//	stHisTerDataThisMonth.usHisUnbalanceTimeCurr+=stHisTerDataThisDay.usHisUnbalanceTimeCurr;
//	return ucRet;
//}
//
//unsigned char MonthData_Stat_CurrOver_Ter(STDATETIME stDateTime)
//{//过流月统计
//	unsigned char ucRet=RET_SUCCESS;
//	DataProc_Stat_GetMax(stHisTerDataThisDay.stHisMaxCurrA,&(stHisTerDataThisMonth.stHisMaxCurrA),stHisTerDataThisDay.stHisMaxCurrA.stDateTime);
//	DataProc_Stat_GetMax(stHisTerDataThisDay.stHisMaxCurrB,&(stHisTerDataThisMonth.stHisMaxCurrB),stHisTerDataThisDay.stHisMaxCurrB.stDateTime);
//	DataProc_Stat_GetMax(stHisTerDataThisDay.stHisMaxCurrC,&(stHisTerDataThisMonth.stHisMaxCurrC),stHisTerDataThisDay.stHisMaxCurrC.stDateTime);
//	DataProc_Stat_GetMax(stHisTerDataThisDay.stHisMaxCurrN,&(stHisTerDataThisMonth.stHisMaxCurrN),stHisTerDataThisDay.stHisMaxCurrN.stDateTime);
//	stHisTerDataThisMonth.usHisErrUpTimeCurrA+=stHisTerDataThisDay.usHisErrUpTimeCurrA;
//	stHisTerDataThisMonth.usHisErrUpTimeCurrB+=stHisTerDataThisDay.usHisErrUpTimeCurrB;
//	stHisTerDataThisMonth.usHisErrUpTimeCurrC+=stHisTerDataThisDay.usHisErrUpTimeCurrC;
//	stHisTerDataThisMonth.usHisNorUpTimeCurrA+=stHisTerDataThisDay.usHisNorUpTimeCurrA;
//	stHisTerDataThisMonth.usHisNorUpTimeCurrB+=stHisTerDataThisDay.usHisNorUpTimeCurrB;
//	stHisTerDataThisMonth.usHisNorUpTimeCurrC+=stHisTerDataThisDay.usHisNorUpTimeCurrC;
//	stHisTerDataThisMonth.usHisNorUpTimeCurrN+=stHisTerDataThisDay.usHisNorUpTimeCurrN;
//	return ucRet;
//}
//
//unsigned char MonthData_Stat_SPowOver_Ter(STDATETIME stDateTime)
//{//视在功率越限月统计
//	unsigned char ucRet=RET_SUCCESS;
//	stHisTerDataThisMonth.usHisErrUpTimeSPower+=stHisTerDataThisDay.usHisErrUpTimeSPower;
//	stHisTerDataThisMonth.usHisNorUpTimeSPower+=stHisTerDataThisDay.usHisNorUpTimeSPower;
//	return ucRet;
//}
//
//unsigned char MonthData_Stat_VoltBreak_Ter(STDATETIME stDateTime)
//{//断相月统计，目前次数与日数据直接相等，认为是当前数据
//	unsigned char ucRet=RET_SUCCESS;
//	stHisTerDataThisMonth.usHisVolBreakNum=stHisTerDataThisDay.usHisVolBreakNum;
//	stHisTerDataThisMonth.usHisVolABreakNum=stHisTerDataThisDay.usHisVolABreakNum;
//	stHisTerDataThisMonth.usHisVolBBreakNum=stHisTerDataThisDay.usHisVolBBreakNum;
//	stHisTerDataThisMonth.usHisVolCBreakNum=stHisTerDataThisDay.usHisVolCBreakNum;
//	stHisTerDataThisMonth.usHisVolBreakTime=stHisTerDataThisDay.usHisVolBreakTime;
//	stHisTerDataThisMonth.usHisVolABreakTime=stHisTerDataThisDay.usHisVolABreakTime;
//	stHisTerDataThisMonth.usHisVolBBreakTime=stHisTerDataThisDay.usHisVolBBreakTime;
//	stHisTerDataThisMonth.usHisVolCBreakTime=stHisTerDataThisDay.usHisVolCBreakTime;
//	stHisTerDataThisMonth.stHisLastVolBreakBegTime=stHisTerDataThisDay.stHisLastVolBreakBegTime;
//	stHisTerDataThisMonth.stHisLastVolABreakBegTime=stHisTerDataThisDay.stHisLastVolABreakBegTime;
//	stHisTerDataThisMonth.stHisLastVolBBreakBegTime=stHisTerDataThisDay.stHisLastVolBBreakBegTime;
//	stHisTerDataThisMonth.stHisLastVolCBreakBegTime=stHisTerDataThisDay.stHisLastVolCBreakBegTime;
//	stHisTerDataThisMonth.stHisLastVolBreakEndTime=stHisTerDataThisDay.stHisLastVolBreakEndTime;
//	stHisTerDataThisMonth.stHisLastVolABreakEndTime=stHisTerDataThisDay.stHisLastVolABreakEndTime;
//	stHisTerDataThisMonth.stHisLastVolBBreakEndTime=stHisTerDataThisDay.stHisLastVolBBreakEndTime;
//	stHisTerDataThisMonth.stHisLastVolCBreakEndTime=stHisTerDataThisDay.stHisLastVolCBreakEndTime;
//	return ucRet;
//}
//
//unsigned char MonthData_Stat_PowRate_Ter(STDATETIME stDateTime)
//{//功率因数分布月统计
//	unsigned char ucRet=RET_SUCCESS;
//	stHisTerDataThisMonth.usHisPowRateSect1Time+=stHisTerDataThisDay.usHisPowRateSect1Time;
//	stHisTerDataThisMonth.usHisPowRateSect2Time+=stHisTerDataThisDay.usHisPowRateSect2Time;
//	stHisTerDataThisMonth.usHisPowRateSect3Time+=stHisTerDataThisDay.usHisPowRateSect3Time;
//	return ucRet;
//}

//unsigned char MonthData_Stat_TerRunState_Ter(STDATETIME stDateTime)
//{//终端运行状态月统计
//	unsigned char ucRet=RET_SUCCESS;
//	stHisTerDataThisMonth.usHisTerPowerOnTime+=stHisTerDataThisDay.usHisTerPowerOnTime;
//	stHisTerDataThisMonth.usHisTerResetNum+=stHisTerDataThisDay.usHisTerResetNum;
//	stHisTerDataThisMonth.ulHisFluxBytes+=stHisTerDataThisDay.ulHisFluxBytes;
//	return ucRet;
//}

//unsigned char MonthData_Stat_CtrlNum_Ter(STDATETIME stDateTime)
//{//控制跳闸月统计
//	unsigned char ucRet=RET_SUCCESS;
//	stHisTerDataThisMonth.usHisMonthEnergyCtrlTripNum+=stHisTerDataThisDay.usHisMonthEnergyCtrlTripNum;
//	stHisTerDataThisMonth.usHisBuyEnergyCtrlTripNum+=stHisTerDataThisDay.usHisBuyEnergyCtrlTripNum;
//	stHisTerDataThisMonth.usHisPowerCtrlTripNum+=stHisTerDataThisDay.usHisPowerCtrlTripNum;
//	stHisTerDataThisMonth.usHisRemoteCtrlTripNum+=stHisTerDataThisDay.usHisRemoteCtrlTripNum;
//	return ucRet;
//}

//unsigned char DataProc_Update_TerData(STDATETIME stDateTime,STHISRUNDATA *pstHisRunData,unsigned char ucReflashLastDay)
//{//终端数据更新
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucMPNo;
//
//	ucMPNo=RunPara_GetTer_MPNo();
//	DataProc_Update_MPData(ucMPNo,stDateTime,pstHisRunData,ucReflashLastDay);
//
//	return ucRet;
//}

unsigned char DataProc_Update_MPData(unsigned char ucMPNo,STDATETIME stDateTime,STHISRUNDATA *pstHisRunData,unsigned char ucReflashLastDay)
{//测量点数据更新，适用于表和终端
	unsigned char ucRet=RET_SUCCESS;
	STHISVAL stHisVal;
	STHISTIME stHisTime;

	pstHisRunData->stRecordTime=stDateTime;

	stHisVal=Get_History_DirPower_byMPNo(ucMPNo,0,0);	pstHisRunData->ulHisDirPowTotal=stHisVal.lVal;	//	正向有功电量
	stHisVal=Get_History_DirPower_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisDirPowFee1=stHisVal.lVal;
	stHisVal=Get_History_DirPower_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisDirPowFee2=stHisVal.lVal;
	stHisVal=Get_History_DirPower_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisDirPowFee3=stHisVal.lVal;
	stHisVal=Get_History_DirPower_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisDirPowFee4=stHisVal.lVal;

	stHisVal=Get_History_UnDirPower_byMPNo(ucMPNo,0,0);	pstHisRunData->ulHisRevPowTotal=stHisVal.lVal;	//	反向有功电量
	stHisVal=Get_History_UnDirPower_byMPNo(ucMPNo,0,1);	pstHisRunData->ulHisRevPowFee1=stHisVal.lVal;
	stHisVal=Get_History_UnDirPower_byMPNo(ucMPNo,0,2);	pstHisRunData->ulHisRevPowFee2=stHisVal.lVal;
	stHisVal=Get_History_UnDirPower_byMPNo(ucMPNo,0,3);	pstHisRunData->ulHisRevPowFee3=stHisVal.lVal;
	stHisVal=Get_History_UnDirPower_byMPNo(ucMPNo,0,4);	pstHisRunData->ulHisRevPowFee4=stHisVal.lVal;
	
	stHisVal=Get_History_DirVar_byMPNo(ucMPNo,0,0);		pstHisRunData->ulHisDirVarTotal=stHisVal.lVal;	//	正向无功电量
	stHisVal=Get_History_DirVar_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisDirVarFee1=stHisVal.lVal;
	stHisVal=Get_History_DirVar_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisDirVarFee2=stHisVal.lVal;
	stHisVal=Get_History_DirVar_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisDirVarFee3=stHisVal.lVal;
	stHisVal=Get_History_DirVar_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisDirVarFee4=stHisVal.lVal;
	
	stHisVal=Get_History_UnDirVar_byMPNo(ucMPNo,0,0);	pstHisRunData->ulHisRevVarTotal=stHisVal.lVal;	//	反向无功电量
	stHisVal=Get_History_UnDirVar_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisRevVarFee1=stHisVal.lVal;
	stHisVal=Get_History_UnDirVar_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisRevVarFee2=stHisVal.lVal;
	stHisVal=Get_History_UnDirVar_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisRevVarFee3=stHisVal.lVal;
	stHisVal=Get_History_UnDirVar_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisRevVarFee4=stHisVal.lVal;

	stHisVal=Get_History_DirVar1_byMPNo(ucMPNo,0,0);			pstHisRunData->ulHisDirVar1=stHisVal.lVal;		//	1象限无功电量
	stHisVal=Get_History_DirVar1_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisDirVar1Fee1=stHisVal.lVal;
	stHisVal=Get_History_DirVar1_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisDirVar1Fee2=stHisVal.lVal;
	stHisVal=Get_History_DirVar1_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisDirVar1Fee3=stHisVal.lVal;
	stHisVal=Get_History_DirVar1_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisDirVar1Fee4=stHisVal.lVal;

	stHisVal=Get_History_DirVar2_byMPNo(ucMPNo,0,0);			pstHisRunData->ulHisDirVar2=stHisVal.lVal;		//	2象限无功电量
	stHisVal=Get_History_DirVar2_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisDirVar2Fee1=stHisVal.lVal;
	stHisVal=Get_History_DirVar2_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisDirVar2Fee2=stHisVal.lVal;
	stHisVal=Get_History_DirVar2_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisDirVar2Fee3=stHisVal.lVal;
	stHisVal=Get_History_DirVar2_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisDirVar2Fee4=stHisVal.lVal;

	stHisVal=Get_History_DirVar3_byMPNo(ucMPNo,0,0);			pstHisRunData->ulHisDirVar3=stHisVal.lVal;		//	3象限无功电量
	stHisVal=Get_History_DirVar3_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisDirVar3Fee1=stHisVal.lVal;
	stHisVal=Get_History_DirVar3_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisDirVar3Fee2=stHisVal.lVal;
	stHisVal=Get_History_DirVar3_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisDirVar3Fee3=stHisVal.lVal;
	stHisVal=Get_History_DirVar3_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisDirVar3Fee4=stHisVal.lVal;

	stHisVal=Get_History_DirVar4_byMPNo(ucMPNo,0,0);			pstHisRunData->ulHisDirVar4=stHisVal.lVal;		//	4象限无功电量
	stHisVal=Get_History_DirVar4_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisDirVar4Fee1=stHisVal.lVal;
	stHisVal=Get_History_DirVar4_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisDirVar4Fee2=stHisVal.lVal;
	stHisVal=Get_History_DirVar4_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisDirVar4Fee3=stHisVal.lVal;
	stHisVal=Get_History_DirVar4_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisDirVar4Fee4=stHisVal.lVal;

	stHisVal=Get_History_DirPowMaxDemand_byMPNo(ucMPNo,0,0);			pstHisRunData->ulHisDirPowMaxDemand=stHisVal.lVal;		//	正向有功最大需量
	stHisVal=Get_History_DirPowMaxDemand_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisDirPowMaxDemandFee1=stHisVal.lVal;
	stHisVal=Get_History_DirPowMaxDemand_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisDirPowMaxDemandFee2=stHisVal.lVal;
	stHisVal=Get_History_DirPowMaxDemand_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisDirPowMaxDemandFee3=stHisVal.lVal;
	stHisVal=Get_History_DirPowMaxDemand_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisDirPowMaxDemandFee4=stHisVal.lVal;

	stHisTime=Get_History_DirPowMaxDemandTime_byMPNo(ucMPNo,0,0);			pstHisRunData->stHisDirPowMaxDemandTime=stHisTime.stHisTime;
	stHisTime=Get_History_DirPowMaxDemandTime_byMPNo(ucMPNo,0,1);		pstHisRunData->stHisDirPowMaxDemandTimeFee1=stHisTime.stHisTime;
	stHisTime=Get_History_DirPowMaxDemandTime_byMPNo(ucMPNo,0,2);		pstHisRunData->stHisDirPowMaxDemandTimeFee2=stHisTime.stHisTime;
	stHisTime=Get_History_DirPowMaxDemandTime_byMPNo(ucMPNo,0,3);		pstHisRunData->stHisDirPowMaxDemandTimeFee3=stHisTime.stHisTime;
	stHisTime=Get_History_DirPowMaxDemandTime_byMPNo(ucMPNo,0,4);		pstHisRunData->stHisDirPowMaxDemandTimeFee4=stHisTime.stHisTime;

	stHisVal=Get_History_DirVarMaxDemand_byMPNo(ucMPNo,0,0);			pstHisRunData->ulHisDirVarMaxDemand=stHisVal.lVal;		//	正向无功最大需量
	stHisVal=Get_History_DirVarMaxDemand_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisDirVarMaxDemandFee1=stHisVal.lVal;
	stHisVal=Get_History_DirVarMaxDemand_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisDirVarMaxDemandFee2=stHisVal.lVal;
	stHisVal=Get_History_DirVarMaxDemand_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisDirVarMaxDemandFee3=stHisVal.lVal;
	stHisVal=Get_History_DirVarMaxDemand_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisDirVarMaxDemandFee4=stHisVal.lVal;

	stHisTime=Get_History_DirVarMaxDemandTime_byMPNo(ucMPNo,0,0);			pstHisRunData->stHisDirVarMaxDemandTime=stHisTime.stHisTime;
	stHisTime=Get_History_DirVarMaxDemandTime_byMPNo(ucMPNo,0,1);		pstHisRunData->stHisDirVarMaxDemandTimeFee1=stHisTime.stHisTime;
	stHisTime=Get_History_DirVarMaxDemandTime_byMPNo(ucMPNo,0,2);		pstHisRunData->stHisDirVarMaxDemandTimeFee2=stHisTime.stHisTime;
	stHisTime=Get_History_DirVarMaxDemandTime_byMPNo(ucMPNo,0,3);		pstHisRunData->stHisDirVarMaxDemandTimeFee3=stHisTime.stHisTime;
	stHisTime=Get_History_DirVarMaxDemandTime_byMPNo(ucMPNo,0,4);		pstHisRunData->stHisDirVarMaxDemandTimeFee4=stHisTime.stHisTime;

	stHisVal=Get_History_UnDirPowMaxDemand_byMPNo(ucMPNo,0,0);			pstHisRunData->ulHisRevPowMaxDemand=stHisVal.lVal;		//	反向有功最大需量
	stHisVal=Get_History_UnDirPowMaxDemand_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisRevPowMaxDemandFee1=stHisVal.lVal;
	stHisVal=Get_History_UnDirPowMaxDemand_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisRevPowMaxDemandFee2=stHisVal.lVal;
	stHisVal=Get_History_UnDirPowMaxDemand_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisRevPowMaxDemandFee3=stHisVal.lVal;
	stHisVal=Get_History_UnDirPowMaxDemand_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisRevPowMaxDemandFee4=stHisVal.lVal;

	stHisVal = Get_History_DirPowA_byMPNo(ucMPNo, 0, 0);					pstHisRunData->ulHisDirPowA = stHisVal.lVal;
	stHisVal = Get_History_DirPowA_byMPNo(ucMPNo, 0, 1);					pstHisRunData->ulHisUnDirPowA = stHisVal.lVal;
	stHisVal = Get_History_DirPowA_byMPNo(ucMPNo, 0, 2);					pstHisRunData->ulHisDirVarA = stHisVal.lVal;
	stHisVal = Get_History_DirPowA_byMPNo(ucMPNo, 0, 3);					pstHisRunData->ulHisUnDirVarA = stHisVal.lVal;
	
	stHisVal = Get_History_DirPowB_byMPNo(ucMPNo, 0, 0);					pstHisRunData->ulHisDirPowB = stHisVal.lVal;
	stHisVal = Get_History_DirPowB_byMPNo(ucMPNo, 0, 1);					pstHisRunData->ulHisUnDirPowB = stHisVal.lVal;
	stHisVal = Get_History_DirPowB_byMPNo(ucMPNo, 0, 2);					pstHisRunData->ulHisDirVarB = stHisVal.lVal;
	stHisVal = Get_History_DirPowB_byMPNo(ucMPNo, 0, 3);					pstHisRunData->ulHisUnDirVarB = stHisVal.lVal;
	
	stHisVal = Get_History_DirPowC_byMPNo(ucMPNo, 0, 0);					pstHisRunData->ulHisDirPowC = stHisVal.lVal;
	stHisVal = Get_History_DirPowC_byMPNo(ucMPNo, 0, 1);					pstHisRunData->ulHisUnDirPowC = stHisVal.lVal;
	stHisVal = Get_History_DirPowC_byMPNo(ucMPNo, 0, 2);					pstHisRunData->ulHisDirVarC = stHisVal.lVal;
	stHisVal = Get_History_DirPowC_byMPNo(ucMPNo, 0, 3);					pstHisRunData->ulHisUnDirVarC = stHisVal.lVal;
	
	stHisTime=Get_History_UnDirPowMaxDemandTime_byMPNo(ucMPNo,0,0);		pstHisRunData->stHisRevPowMaxDemandTime=stHisTime.stHisTime;
	stHisTime=Get_History_UnDirPowMaxDemandTime_byMPNo(ucMPNo,0,1);		pstHisRunData->stHisRevPowMaxDemandTimeFee1=stHisTime.stHisTime;
	stHisTime=Get_History_UnDirPowMaxDemandTime_byMPNo(ucMPNo,0,2);		pstHisRunData->stHisRevPowMaxDemandTimeFee2=stHisTime.stHisTime;
	stHisTime=Get_History_UnDirPowMaxDemandTime_byMPNo(ucMPNo,0,3);		pstHisRunData->stHisRevPowMaxDemandTimeFee3=stHisTime.stHisTime;
	stHisTime=Get_History_UnDirPowMaxDemandTime_byMPNo(ucMPNo,0,4);		pstHisRunData->stHisRevPowMaxDemandTimeFee4=stHisTime.stHisTime;

	stHisVal=Get_History_UnDirVarMaxDemand_byMPNo(ucMPNo,0,0);		pstHisRunData->ulHisRevVarMaxDemand=stHisVal.lVal;		//	反向无功最大需量
	stHisVal=Get_History_UnDirVarMaxDemand_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisRevVarMaxDemandFee1=stHisVal.lVal;
	stHisVal=Get_History_UnDirVarMaxDemand_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisRevVarMaxDemandFee2=stHisVal.lVal;
	stHisVal=Get_History_UnDirVarMaxDemand_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisRevVarMaxDemandFee3=stHisVal.lVal;
	stHisVal=Get_History_UnDirVarMaxDemand_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisRevVarMaxDemandFee4=stHisVal.lVal;

	stHisTime=Get_History_UnDirVarMaxDemandTime_byMPNo(ucMPNo,0,0);			pstHisRunData->stHisRevVarMaxDemandTime=stHisTime.stHisTime;
	stHisTime=Get_History_UnDirVarMaxDemandTime_byMPNo(ucMPNo,0,1);		pstHisRunData->stHisRevVarMaxDemandTimeFee1=stHisTime.stHisTime;
	stHisTime=Get_History_UnDirVarMaxDemandTime_byMPNo(ucMPNo,0,2);		pstHisRunData->stHisRevVarMaxDemandTimeFee2=stHisTime.stHisTime;
	stHisTime=Get_History_UnDirVarMaxDemandTime_byMPNo(ucMPNo,0,3);		pstHisRunData->stHisRevVarMaxDemandTimeFee3=stHisTime.stHisTime;
	stHisTime=Get_History_UnDirVarMaxDemandTime_byMPNo(ucMPNo,0,4);		pstHisRunData->stHisRevVarMaxDemandTimeFee4=stHisTime.stHisTime;

	if(ucReflashLastDay==CON_STATUS_YES || pstHisRunData->ulHisLastDayDirPowTotal==CONINVALIDVAL)
	{//上一天的数据无效则刷新之
		pstHisRunData->ulHisLastDayDirPowTotal=pstHisRunData->ulHisDirPowTotal;
		pstHisRunData->ulHisLastDayDirPowFee1=pstHisRunData->ulHisDirPowFee1;
		pstHisRunData->ulHisLastDayDirPowFee2=pstHisRunData->ulHisDirPowFee2;
		pstHisRunData->ulHisLastDayDirPowFee3=pstHisRunData->ulHisDirPowFee3;
		pstHisRunData->ulHisLastDayDirPowFee4=pstHisRunData->ulHisDirPowFee4;
	}

	if(ucReflashLastDay==CON_STATUS_YES || pstHisRunData->ulHisLastDayRevPowTotal==CONINVALIDVAL)
	{//上一天的数据无效则刷新之
		pstHisRunData->ulHisLastDayRevPowTotal=pstHisRunData->ulHisRevPowTotal;
		pstHisRunData->ulHisLastDayRevPowFee1=pstHisRunData->ulHisRevPowFee1;
		pstHisRunData->ulHisLastDayRevPowFee2=pstHisRunData->ulHisRevPowFee2;
		pstHisRunData->ulHisLastDayRevPowFee3=pstHisRunData->ulHisRevPowFee3;
		pstHisRunData->ulHisLastDayRevPowFee4=pstHisRunData->ulHisRevPowFee4;
	}

	if(ucReflashLastDay==CON_STATUS_YES || pstHisRunData->ulHisLastDayDirVarTotal==CONINVALIDVAL)
	{//上一天的数据无效则刷新之
		pstHisRunData->ulHisLastDayDirVarTotal=pstHisRunData->ulHisDirVarTotal;
		pstHisRunData->ulHisLastDayDirVarFee1=pstHisRunData->ulHisDirVarFee1;
		pstHisRunData->ulHisLastDayDirVarFee2=pstHisRunData->ulHisDirVarFee2;
		pstHisRunData->ulHisLastDayDirVarFee3=pstHisRunData->ulHisDirVarFee3;
		pstHisRunData->ulHisLastDayDirVarFee4=pstHisRunData->ulHisDirVarFee4;
	}

	if(ucReflashLastDay==CON_STATUS_YES || pstHisRunData->ulHisLastDayRevVarTotal==CONINVALIDVAL)
	{//上一天的数据无效则刷新之
		pstHisRunData->ulHisLastDayRevVarTotal=pstHisRunData->ulHisRevVarTotal;
		pstHisRunData->ulHisLastDayRevVarFee1=pstHisRunData->ulHisRevVarFee1;
		pstHisRunData->ulHisLastDayRevVarFee2=pstHisRunData->ulHisRevVarFee2;
		pstHisRunData->ulHisLastDayRevVarFee3=pstHisRunData->ulHisRevVarFee3;
		pstHisRunData->ulHisLastDayRevVarFee4=pstHisRunData->ulHisRevVarFee4;
	}

	return ucRet;
}

unsigned char DataProc_Stat_Clear_MPData(STHISRUNDATA *pstHisRunData)
{
	unsigned char ucRet=RET_SUCCESS;
	pstHisRunData->ulHisDirPowTotal=CONINVALIDVAL;
	pstHisRunData->ulHisDirPowFee1=CONINVALIDVAL;
	pstHisRunData->ulHisDirPowFee2=CONINVALIDVAL;
	pstHisRunData->ulHisDirPowFee3=CONINVALIDVAL;
	pstHisRunData->ulHisDirPowFee4=CONINVALIDVAL;

	pstHisRunData->ulHisRevPowTotal=CONINVALIDVAL;
	pstHisRunData->ulHisRevPowFee1=CONINVALIDVAL;
	pstHisRunData->ulHisRevPowFee2=CONINVALIDVAL;
	pstHisRunData->ulHisRevPowFee3=CONINVALIDVAL;
	pstHisRunData->ulHisRevPowFee4=CONINVALIDVAL;

	pstHisRunData->ulHisDirVarTotal=CONINVALIDVAL;
	pstHisRunData->ulHisDirVarFee1=CONINVALIDVAL;
	pstHisRunData->ulHisDirVarFee2=CONINVALIDVAL;
	pstHisRunData->ulHisDirVarFee3=CONINVALIDVAL;
	pstHisRunData->ulHisDirVarFee4=CONINVALIDVAL;

	pstHisRunData->ulHisRevVarTotal=CONINVALIDVAL;
	pstHisRunData->ulHisRevVarFee1=CONINVALIDVAL;
	pstHisRunData->ulHisRevVarFee2=CONINVALIDVAL;
	pstHisRunData->ulHisRevVarFee3=CONINVALIDVAL;
	pstHisRunData->ulHisRevVarFee4=CONINVALIDVAL;

	pstHisRunData->ulHisLastDayDirPowTotal=CONINVALIDVAL;
	pstHisRunData->ulHisLastDayDirPowFee1=CONINVALIDVAL;
	pstHisRunData->ulHisLastDayDirPowFee2=CONINVALIDVAL;
	pstHisRunData->ulHisLastDayDirPowFee3=CONINVALIDVAL;
	pstHisRunData->ulHisLastDayDirPowFee4=CONINVALIDVAL;

	pstHisRunData->ulHisLastDayRevPowTotal=CONINVALIDVAL;
	pstHisRunData->ulHisLastDayRevPowFee1=CONINVALIDVAL;
	pstHisRunData->ulHisLastDayRevPowFee2=CONINVALIDVAL;
	pstHisRunData->ulHisLastDayRevPowFee3=CONINVALIDVAL;
	pstHisRunData->ulHisLastDayRevPowFee4=CONINVALIDVAL;

	pstHisRunData->ulHisLastDayDirVarTotal=CONINVALIDVAL;
	pstHisRunData->ulHisLastDayDirVarFee1=CONINVALIDVAL;
	pstHisRunData->ulHisLastDayDirVarFee2=CONINVALIDVAL;
	pstHisRunData->ulHisLastDayDirVarFee3=CONINVALIDVAL;
	pstHisRunData->ulHisLastDayDirVarFee4=CONINVALIDVAL;

	pstHisRunData->ulHisLastDayRevVarTotal=CONINVALIDVAL;
	pstHisRunData->ulHisLastDayRevVarFee1=CONINVALIDVAL;
	pstHisRunData->ulHisLastDayRevVarFee2=CONINVALIDVAL;
	pstHisRunData->ulHisLastDayRevVarFee3=CONINVALIDVAL;
	pstHisRunData->ulHisLastDayRevVarFee4=CONINVALIDVAL;

	pstHisRunData->ulHisDirVar1=CONINVALIDVAL;
	pstHisRunData->ulHisDirVar1Fee1=CONINVALIDVAL;
	pstHisRunData->ulHisDirVar1Fee2=CONINVALIDVAL;
	pstHisRunData->ulHisDirVar1Fee3=CONINVALIDVAL;
	pstHisRunData->ulHisDirVar1Fee4=CONINVALIDVAL;

	pstHisRunData->ulHisDirVar2=CONINVALIDVAL;
	pstHisRunData->ulHisDirVar2Fee1=CONINVALIDVAL;
	pstHisRunData->ulHisDirVar2Fee2=CONINVALIDVAL;
	pstHisRunData->ulHisDirVar2Fee3=CONINVALIDVAL;
	pstHisRunData->ulHisDirVar2Fee4=CONINVALIDVAL;

	pstHisRunData->ulHisDirVar3=CONINVALIDVAL;
	pstHisRunData->ulHisDirVar3Fee1=CONINVALIDVAL;
	pstHisRunData->ulHisDirVar3Fee2=CONINVALIDVAL;
	pstHisRunData->ulHisDirVar3Fee3=CONINVALIDVAL;
	pstHisRunData->ulHisDirVar3Fee4=CONINVALIDVAL;

	pstHisRunData->ulHisDirVar4=CONINVALIDVAL;
	pstHisRunData->ulHisDirVar4Fee1=CONINVALIDVAL;
	pstHisRunData->ulHisDirVar4Fee2=CONINVALIDVAL;
	pstHisRunData->ulHisDirVar4Fee3=CONINVALIDVAL;
	pstHisRunData->ulHisDirVar4Fee4=CONINVALIDVAL;

	pstHisRunData->ulHisDirPowA = CONINVALIDVAL;
	pstHisRunData->ulHisUnDirPowA = CONINVALIDVAL;
	pstHisRunData->ulHisDirVarA = CONINVALIDVAL;
	pstHisRunData->ulHisUnDirVarA = CONINVALIDVAL;

	pstHisRunData->ulHisDirPowB= CONINVALIDVAL;
	pstHisRunData->ulHisUnDirPowB= CONINVALIDVAL;
	pstHisRunData->ulHisDirVarB= CONINVALIDVAL;
	pstHisRunData->ulHisUnDirVarB= CONINVALIDVAL;

	pstHisRunData->ulHisDirPowC= CONINVALIDVAL;
	pstHisRunData->ulHisUnDirPowC= CONINVALIDVAL;
	pstHisRunData->ulHisDirVarC= CONINVALIDVAL;
	pstHisRunData->ulHisUnDirVarC= CONINVALIDVAL;

	pstHisRunData->ulHisDirPowMaxDemand=CONINVALIDVAL;
	pstHisRunData->ulHisDirPowMaxDemandFee1=CONINVALIDVAL;
	pstHisRunData->ulHisDirPowMaxDemandFee2=CONINVALIDVAL;
	pstHisRunData->ulHisDirPowMaxDemandFee3=CONINVALIDVAL;
	pstHisRunData->ulHisDirPowMaxDemandFee4=CONINVALIDVAL;

	pstHisRunData->ulHisDirVarMaxDemand=CONINVALIDVAL;
	pstHisRunData->ulHisDirVarMaxDemandFee1=CONINVALIDVAL;
	pstHisRunData->ulHisDirVarMaxDemandFee2=CONINVALIDVAL;
	pstHisRunData->ulHisDirVarMaxDemandFee3=CONINVALIDVAL;
	pstHisRunData->ulHisDirVarMaxDemandFee4=CONINVALIDVAL;

	pstHisRunData->ulHisRevPowMaxDemand=CONINVALIDVAL;
	pstHisRunData->ulHisRevPowMaxDemandFee1=CONINVALIDVAL;
	pstHisRunData->ulHisRevPowMaxDemandFee2=CONINVALIDVAL;
	pstHisRunData->ulHisRevPowMaxDemandFee3=CONINVALIDVAL;
	pstHisRunData->ulHisRevPowMaxDemandFee4=CONINVALIDVAL;

	pstHisRunData->ulHisRevVarMaxDemand=CONINVALIDVAL;
	pstHisRunData->ulHisRevVarMaxDemandFee1=CONINVALIDVAL;
	pstHisRunData->ulHisRevVarMaxDemandFee2=CONINVALIDVAL;
	pstHisRunData->ulHisRevVarMaxDemandFee3=CONINVALIDVAL;
	pstHisRunData->ulHisRevVarMaxDemandFee4=CONINVALIDVAL;

	ClearDateTime(&(pstHisRunData->stHisDirPowMaxDemandTime));
	ClearDateTime(&(pstHisRunData->stHisDirPowMaxDemandTimeFee1));
	ClearDateTime(&(pstHisRunData->stHisDirPowMaxDemandTimeFee2));
	ClearDateTime(&(pstHisRunData->stHisDirPowMaxDemandTimeFee3));
	ClearDateTime(&(pstHisRunData->stHisDirPowMaxDemandTimeFee4));

	ClearDateTime(&(pstHisRunData->stHisDirVarMaxDemandTime));
	ClearDateTime(&(pstHisRunData->stHisDirVarMaxDemandTimeFee1));
	ClearDateTime(&(pstHisRunData->stHisDirVarMaxDemandTimeFee2));
	ClearDateTime(&(pstHisRunData->stHisDirVarMaxDemandTimeFee3));
	ClearDateTime(&(pstHisRunData->stHisDirVarMaxDemandTimeFee4));

	ClearDateTime(&(pstHisRunData->stHisRevPowMaxDemandTime));
	ClearDateTime(&(pstHisRunData->stHisRevPowMaxDemandTimeFee1));
	ClearDateTime(&(pstHisRunData->stHisRevPowMaxDemandTimeFee2));
	ClearDateTime(&(pstHisRunData->stHisRevPowMaxDemandTimeFee3));
	ClearDateTime(&(pstHisRunData->stHisRevPowMaxDemandTimeFee4));

	ClearDateTime(&(pstHisRunData->stHisRevVarMaxDemandTime));
	ClearDateTime(&(pstHisRunData->stHisRevVarMaxDemandTimeFee1));
	ClearDateTime(&(pstHisRunData->stHisRevVarMaxDemandTimeFee2));
	ClearDateTime(&(pstHisRunData->stHisRevVarMaxDemandTimeFee3));
	ClearDateTime(&(pstHisRunData->stHisRevVarMaxDemandTimeFee4));

	return ucRet;
}

//unsigned char DataProc_TerDayData_Move(STDATETIME stDateTime)
//{//日数据处理
//	unsigned char ucRet=RET_SUCCESS;
//	//HdConvertWatchDog();
//	DataProc_Update_TerData(stDateTime,&(stHisTerDataThisDay),CON_STATUS_NO);
//	DataProc_TerSaveData_Day(stDateTime,&(stHisTerDataThisDay));
//	return ucRet;
//}

//unsigned char DataProc_TerMonthData_Move(STDATETIME stDateTime)
//{//月数据处理
//	unsigned char ucRet=RET_SUCCESS;
//	//HdConvertWatchDog();
//	DataProc_Update_TerData(stDateTime,&(stHisTerDataThisMonth),CON_STATUS_NO);
//	DataProc_TerSaveData_Month(stDateTime,&(stHisTerDataThisMonth));
//	return ucRet;
//}

//unsigned char DataProc_TerSaveData_Day(STDATETIME stDateTime,STHISRUNDATA *pstHisRunData)
//{//保存日数据
//	unsigned char ucRet=RET_SUCCESS;
// 	unsigned char ucPos=0xFF,i;
// 	unsigned short usBeginPage,usBeginPos;
// 	unsigned long ulDataAddr,ulDataTime,ulQueryTime;
// 	STDATETIME stTempTime;
// 	STDAYLISTINDEX *pstDataIndex=NULL;

// 	if(DP_Get_TerDP_State()==CON_TERDP_STATE_DOWNLOAD)			//数据块用于程序下装数据存储
// 		return ucRet;
// 	stTempTime=stDateTime;						//stDateTime为日初或者日期变化后的时间，数据要作为上一天的统计结果
// 	stTempTime.ucHour=0;
// 	stTempTime.ucMinute=0;
// 	stTempTime.ucSecond=0;
// 	pstDataIndex=&(stHisTerDataIndex.stTerDayDataIndex);
// 	if(pstDataIndex->ucHead != pstDataIndex->ucTail)
// 	{//非空
// 		for(i=0;i<CON_DAYDATA_NUM;i++)
// 		{
// 			ucPos=(pstDataIndex->ucTail+i) % CON_DAYDATA_NUM;
// 			if(ucPos==pstDataIndex->ucHead)
// 			{
// 				ucPos=0xFF;
// 				break;
// 			}
// 			ulDataTime=TimeConver(&(pstDataIndex->stDateTimeList[ucPos]),CON_TIMEUNIT_SEC);
// 			ulQueryTime=TimeConver(&stTempTime,CON_TIMEUNIT_SEC);
// 			if((ulQueryTime<=ulDataTime && ulDataTime<ulQueryTime+3600) || (ulQueryTime>=ulDataTime && ulQueryTime<ulDataTime+3600))
// 				break;
// 		}
// 	}
// 	if(ucPos<CON_DAYDATA_NUM)
// 	{//更新
// 		ulDataAddr	=sizeof(STHISRUNDATA)*(ucPos+CON_DAYFROZENDATA_NUM);
// 	}else
// 	{//增加
// 		ulDataAddr	=sizeof(STHISRUNDATA)*(pstDataIndex->ucHead+CON_DAYFROZENDATA_NUM);
// 		//更新索引
// 		pstDataIndex->stDateTimeList[pstDataIndex->ucHead]=stTempTime;
// 		pstDataIndex->ucHead++;
// 		if(pstDataIndex->ucHead>=CON_DAYDATA_NUM)
// 			pstDataIndex->ucHead=0;
// 		if(pstDataIndex->ucHead==pstDataIndex->ucTail)
// 			pstDataIndex->ucTail++;
// 		if(pstDataIndex->ucTail>=CON_DAYDATA_NUM)
// 			pstDataIndex->ucTail=0;
// 	}
// 	//计算位置
//  	usBeginPage	= CON_DFPAGE_HEAD_TER_HISDATA;
// 	usBeginPage	=usBeginPage+(unsigned short)(ulDataAddr/FPAGE_SIZE);
// 	usBeginPos	=(unsigned short)(ulDataAddr % FPAGE_SIZE);
// 	ucRet=Flash_SetMainMemory((unsigned char *)pstHisRunData,sizeof(STHISRUNDATA),usBeginPage,usBeginPos);
// 	if(ucRet==EF_NORMAL)
// 		ucRet=RET_SUCCESS;
// 	else
// 		ucRet=RET_ERROR;
//	return ucRet;
//}

//unsigned char DataProc_TerSaveData_Month(STDATETIME stDateTime,STHISRUNDATA *pstHisRunData)
//{//保存月数据
//	unsigned char ucRet=RET_SUCCESS;
// 	unsigned char ucPos=0xFF,i;
// 	unsigned short usBeginPage,usBeginPos;
// 	unsigned long ulDataAddr,ulDataTime,ulQueryTime;
// 	STDATETIME stTempTime;
// 	STMONTHLISTINDEX *pstDataIndex=NULL;

// 	if(DP_Get_TerDP_State()==CON_TERDP_STATE_DOWNLOAD)			//数据块用于程序下装数据存储
// 		return ucRet;

// 	stTempTime=stDateTime;
// 	stTempTime.ucDay=1;
// 	stTempTime.ucHour=0;
// 	stTempTime.ucMinute=0;
// 	stTempTime.ucSecond=0;
// 	pstDataIndex=&(stHisTerDataIndex.stTerMonthDataIndex);
// 	if(pstDataIndex->ucHead != pstDataIndex->ucTail)
// 	{//非空
// 		for(i=0;i<CON_MONTHDATA_NUM;i++)
// 		{
// 			ucPos=(pstDataIndex->ucTail+i) % CON_MONTHDATA_NUM;
// 			if(ucPos==pstDataIndex->ucHead)
// 			{
// 				ucPos=0xFF;
// 				break;
// 			}
// 			ulDataTime=TimeConver(&(pstDataIndex->stDateTimeList[ucPos]),CON_TIMEUNIT_SEC);
// 			ulQueryTime=TimeConver(&stTempTime,CON_TIMEUNIT_SEC);
// 			if((ulQueryTime<=ulDataTime && ulDataTime<ulQueryTime+432000) || (ulQueryTime>=ulDataTime && ulQueryTime<ulDataTime+432000))	//偏差5天，考虑月末28日~31日
// 				break;
// 		}
// 	}
// 	if(ucPos<CON_MONTHDATA_NUM)
// 	{//更新
// 		ulDataAddr	=sizeof(STHISRUNDATA)*(ucPos+CON_DAYFROZENDATA_NUM+CON_DAYDATA_NUM);
// 	}else
// 	{//增加
// 		ulDataAddr	=sizeof(STHISRUNDATA)*(pstDataIndex->ucHead+CON_DAYFROZENDATA_NUM+CON_DAYDATA_NUM);
// 		//更新索引
// 		pstDataIndex->stDateTimeList[pstDataIndex->ucHead]=stTempTime;
// 		pstDataIndex->ucHead++;
// 		if(pstDataIndex->ucHead>=CON_MONTHDATA_NUM)
// 			pstDataIndex->ucHead=0;
// 		if(pstDataIndex->ucHead==pstDataIndex->ucTail)
// 			pstDataIndex->ucTail++;
// 		if(pstDataIndex->ucTail>=CON_MONTHDATA_NUM)
// 			pstDataIndex->ucTail=0;
// 	}
// 	//计算位置
//  	usBeginPage	= CON_DFPAGE_HEAD_TER_HISDATA;
// 	usBeginPage	=usBeginPage+(unsigned short)(ulDataAddr/FPAGE_SIZE);
// 	usBeginPos	=(unsigned short)(ulDataAddr % FPAGE_SIZE);
// 	ucRet=Flash_SetMainMemory((unsigned char *)pstHisRunData,sizeof(STHISRUNDATA),usBeginPage,usBeginPos);
// 	if(ucRet==EF_NORMAL)
// 		ucRet=RET_SUCCESS;
// 	else
// 		ucRet=RET_ERROR;
//	return ucRet;
//}

//unsigned char DataProc_TerDayData_Stat_Clear(void)
//{//初始化日数据统计
//	unsigned char ucRet=RET_SUCCESS;
//	DataProc_Stat_Clear_MPData(&(stHisTerDataThisDay));
//	DataProc_Stat_Clear_InstantPow(&(stHisTerDataThisDay));
//	DataProc_Stat_Clear_Demand(&(stHisTerDataThisDay));
//	DataProc_Stat_Clear_VoltStat(&(stHisTerDataThisDay));
//	DataProc_Stat_Clear_Unbalance(&(stHisTerDataThisDay));
//	DataProc_Stat_Clear_CurrOver(&(stHisTerDataThisDay));
//	//HdConvertWatchDog();
//	DataProc_Stat_Clear_SPowOver(&(stHisTerDataThisDay));
//	DataProc_Stat_Clear_VoltBreak(&(stHisTerDataThisDay));
//	DataProc_Stat_Clear_PowRate(&(stHisTerDataThisDay));
//	DataProc_Stat_Clear_TerRunState(&(stHisTerDataThisDay));
//	DataProc_Stat_Clear_CtrlNum(&(stHisTerDataThisDay));
//	return ucRet;
//}

//unsigned char DataProc_TerMonthData_Stat_Clear(void)
//{//初始化月数据统计
//	unsigned char ucRet=RET_SUCCESS;
//	DataProc_Stat_Clear_MPData(&(stHisTerDataThisMonth));
//	DataProc_Stat_Clear_InstantPow(&(stHisTerDataThisMonth));
//	DataProc_Stat_Clear_Demand(&(stHisTerDataThisMonth));
//	DataProc_Stat_Clear_VoltStat(&(stHisTerDataThisMonth));
//	DataProc_Stat_Clear_Unbalance(&(stHisTerDataThisMonth));
//	DataProc_Stat_Clear_CurrOver(&(stHisTerDataThisMonth));
//	DataProc_Stat_Clear_SPowOver(&(stHisTerDataThisMonth));
//	DataProc_Stat_Clear_VoltBreak(&(stHisTerDataThisMonth));
//	DataProc_Stat_Clear_PowRate(&(stHisTerDataThisMonth));
//	DataProc_Stat_Clear_TerRunState(&(stHisTerDataThisMonth));
//	DataProc_Stat_Clear_CtrlNum(&(stHisTerDataThisMonth));
//	return ucRet;
//}

//unsigned char DataProc_TerDayData_Stat(STDATETIME stDateTime)
//{//日数据统计
//	unsigned char ucRet=RET_SUCCESS;
//	DataProc_Update_TerData(stDateTime,&(stHisTerDataThisDay),CON_STATUS_NO);

//	DayData_Stat_InstantPow_Ter(stDateTime);
//	DayData_Stat_Demand_Ter(stDateTime);
//	DayData_Stat_VoltStat_Ter(stDateTime);
//	DayData_Stat_Unbalance_Ter(stDateTime);

//	DayData_Stat_CurrOver_Ter(stDateTime);
//	DayData_Stat_SPowOver_Ter(stDateTime);
//	DayData_Stat_VoltBreak_Ter(stDateTime);
//	DayData_Stat_PowRate_Ter(stDateTime);
//	DayData_Stat_TerRunState_Ter(stDateTime);
//	DayData_Stat_CtrlNum_Ter(stDateTime);
	//HdConvertWatchDog();
//	return ucRet;
//}

//unsigned char DataProc_TerMonthData_Stat(STDATETIME stDateTime)
//{//月数据统计
//	unsigned char ucRet=RET_SUCCESS;
//	DataProc_Update_TerData(stDateTime,&(stHisTerDataThisMonth),CON_STATUS_NO);
//
//	MonthData_Stat_InstantPow_Ter(stDateTime);
//	MonthData_Stat_Demand_Ter(stDateTime);
//	MonthData_Stat_VoltStat_Ter(stDateTime);
//	MonthData_Stat_Unbalance_Ter(stDateTime);
//	MonthData_Stat_CurrOver_Ter(stDateTime);

//	MonthData_Stat_SPowOver_Ter(stDateTime);
//	MonthData_Stat_VoltBreak_Ter(stDateTime);
//	MonthData_Stat_PowRate_Ter(stDateTime);
//	MonthData_Stat_TerRunState_Ter(stDateTime);
//	MonthData_Stat_CtrlNum_Ter(stDateTime);
//	return ucRet;
//}

//unsigned char DataProc_TerMonthData_FlashLast(STDATETIME stDateTime)
//{//月数据统计
//	unsigned char ucRet=RET_SUCCESS;
//	STHISRUNDATA *pstHisRunData;
//	unsigned char ucMPNo;
//	STHISVAL stHisVal;
//
//	ucMPNo=RunPara_GetTer_MPNo();
//	pstHisRunData=&(stHisTerDataThisMonth);
//
//	if(pstHisRunData->ulHisLastDayDirPowTotal==CONINVALIDVAL)
//	{//上一天的数据无效则刷新之
//		stHisVal=Get_History_DirPower_byMPNo(ucMPNo,0,0);	pstHisRunData->ulHisLastDayDirPowTotal=stHisVal.lVal;
//		stHisVal=Get_History_DirPower_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisLastDayDirPowFee1=stHisVal.lVal;
//		stHisVal=Get_History_DirPower_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisLastDayDirPowFee2=stHisVal.lVal;
//		stHisVal=Get_History_DirPower_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisLastDayDirPowFee3=stHisVal.lVal;
//		stHisVal=Get_History_DirPower_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisLastDayDirPowFee4=stHisVal.lVal;
//	}
//
//	if(pstHisRunData->ulHisLastDayRevPowTotal==CONINVALIDVAL)
//	{//上一天的数据无效则刷新之
//		stHisVal=Get_History_UnDirPower_byMPNo(ucMPNo,0,0);	pstHisRunData->ulHisLastDayRevPowTotal=stHisVal.lVal;
//		stHisVal=Get_History_UnDirPower_byMPNo(ucMPNo,0,1);	pstHisRunData->ulHisLastDayRevPowFee1=stHisVal.lVal;
//		stHisVal=Get_History_UnDirPower_byMPNo(ucMPNo,0,2);	pstHisRunData->ulHisLastDayRevPowFee2=stHisVal.lVal;
//		stHisVal=Get_History_UnDirPower_byMPNo(ucMPNo,0,3);	pstHisRunData->ulHisLastDayRevPowFee3=stHisVal.lVal;
//		stHisVal=Get_History_UnDirPower_byMPNo(ucMPNo,0,4);	pstHisRunData->ulHisLastDayRevPowFee4=stHisVal.lVal;
//	}
//
//	if(pstHisRunData->ulHisLastDayDirVarTotal==CONINVALIDVAL)
//	{//上一天的数据无效则刷新之
//		stHisVal=Get_History_DirVar_byMPNo(ucMPNo,0,0);		pstHisRunData->ulHisLastDayDirVarTotal=stHisVal.lVal;
//		stHisVal=Get_History_DirVar_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisLastDayDirVarFee1=stHisVal.lVal;
//		stHisVal=Get_History_DirVar_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisLastDayDirVarFee2=stHisVal.lVal;
//		stHisVal=Get_History_DirVar_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisLastDayDirVarFee3=stHisVal.lVal;
//		stHisVal=Get_History_DirVar_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisLastDayDirVarFee4=stHisVal.lVal;
//	}
//
//	if(pstHisRunData->ulHisLastDayRevVarTotal==CONINVALIDVAL)
//	{//上一天的数据无效则刷新之
//		stHisVal=Get_History_UnDirVar_byMPNo(ucMPNo,0,0);	pstHisRunData->ulHisLastDayRevVarTotal=stHisVal.lVal;
//		stHisVal=Get_History_UnDirVar_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisLastDayRevVarFee1=stHisVal.lVal;
//		stHisVal=Get_History_UnDirVar_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisLastDayRevVarFee2=stHisVal.lVal;
//		stHisVal=Get_History_UnDirVar_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisLastDayRevVarFee3=stHisVal.lVal;
//		stHisVal=Get_History_UnDirVar_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisLastDayRevVarFee4=stHisVal.lVal;
//	}
//
//	return ucRet;
//}

unsigned char DataProc_GetData_Day_Ter(STDATETIME stDateTime,STHISRUNDATA *pstHisRunData)
{//读日数据
	unsigned char ucRet=RET_SUCCESS;
// 	unsigned char ucPos=0xFF,i=0;
// 	unsigned short usBeginPage,usBeginPos;
// 	unsigned long ulDataAddr;
// 	unsigned long ulQueryTime,ulDataTime,ulThisTime;
// 	STDATETIME stThisTime=GetCurrentTime();
// 	STDAYLISTINDEX *pstDataIndex=NULL;
// 	stThisTime.ucSecond=59;
// 	stThisTime.ucMinute=59;
// 	stThisTime.ucHour=23;
// 	ulThisTime=TimeConver(&stThisTime,CON_TIMEUNIT_SEC);
// 	ulQueryTime=TimeConver(&stDateTime,CON_TIMEUNIT_SEC);
// 	if((ulQueryTime<=ulThisTime && ulThisTime<ulQueryTime+3600) || (ulQueryTime>=ulThisTime && ulQueryTime<ulThisTime+3600))
// 	{//当日统计数据取当前数据
// 		memcpy(pstHisRunData,&(stHisTerDataThisDay),sizeof(STHISRUNDATA));
// 		pstHisRunData->stRecordTime=GetCurrentTime();
// 	}else
// 	{
// 		pstDataIndex=&(stHisTerDataIndex.stTerDayDataIndex);
// 		if(pstDataIndex->ucHead != pstDataIndex->ucTail)
// 		{//非空
// 			for(i=0;i<CON_DAYDATA_NUM;i++)
// 			{
// 				ucPos=(pstDataIndex->ucTail+i) % CON_DAYDATA_NUM;
// 				if(ucPos==pstDataIndex->ucHead)
// 				{
// 					ucPos=0xFF;
// 					break;
// 				}
// 				ulDataTime=TimeConver(&(pstDataIndex->stDateTimeList[ucPos]),CON_TIMEUNIT_SEC);
// 				if((ulQueryTime<=ulDataTime && ulDataTime<ulQueryTime+3600) || (ulQueryTime>=ulDataTime && ulQueryTime<ulDataTime+3600))
// 					break;
// 			}
// 		}
// 		if(ucPos<CON_DAYDATA_NUM)
// 		{//计算位置
//  			usBeginPage	=CON_DFPAGE_HEAD_TER_HISDATA;
// 			ulDataAddr	=sizeof(STHISRUNDATA)*(ucPos+CON_DAYFROZENDATA_NUM);
// 			usBeginPage	=usBeginPage+(unsigned short)(ulDataAddr/FPAGE_SIZE);
// 			usBeginPos	=(unsigned short)(ulDataAddr % FPAGE_SIZE);
// 			ucRet=Flash_GetMainMemory(usBeginPage,usBeginPos,sizeof(STHISRUNDATA),(unsigned char *)pstHisRunData);
// 			if(ucRet==EF_NORMAL)
// 				ucRet=RET_SUCCESS;
// 			else
// 				ucRet=RET_ERROR;
// 		}else
// 			ucRet=RET_ERROR;
// 	}
	return ucRet;
}

unsigned char DataProc_GetData_Month_Ter(STDATETIME stDateTime,STHISRUNDATA *pstHisRunData)
{//读月数据
	unsigned char ucRet=RET_SUCCESS;
// 	unsigned char ucPos=0xFF,i=0;
// 	unsigned short usBeginPage,usBeginPos;
// 	unsigned long ulDataAddr;
// 	unsigned long ulQueryTime,ulDataTime,ulThisTime;

// 	STDATETIME stThisTime=GetCurrentTime();
// 	STMONTHLISTINDEX *pstDataIndex=NULL;
// 	stThisTime.ucSecond=59;
// 	stThisTime.ucMinute=59;
// 	stThisTime.ucHour=23;
// 	stThisTime.ucDay=28;
// 	ulThisTime=TimeConver(&stThisTime,CON_TIMEUNIT_SEC);
// 	ulQueryTime=TimeConver(&stDateTime,CON_TIMEUNIT_SEC);
// 	if((ulQueryTime<=ulThisTime && ulThisTime<ulQueryTime+432000) || (ulQueryTime>=ulThisTime && ulQueryTime<ulThisTime+432000))
// 	{//当月统计数据取当前数据
// 		memcpy(pstHisRunData,&(stHisTerDataThisMonth),sizeof(STHISRUNDATA));
// 		pstHisRunData->stRecordTime=GetCurrentTime();
// 	}else
// 	{
// 		pstDataIndex=&(stHisTerDataIndex.stTerMonthDataIndex);
// 		if(pstDataIndex->ucHead != pstDataIndex->ucTail)
// 		{//非空
// 			for(i=0;i<CON_MONTHDATA_NUM;i++)
// 			{
// 				ucPos=(pstDataIndex->ucTail+i) % CON_MONTHDATA_NUM;
// 				if(ucPos==pstDataIndex->ucHead)
// 				{
// 					ucPos=0xFF;
// 					break;
// 				}
// 				ulDataTime=TimeConver(&(pstDataIndex->stDateTimeList[ucPos]),CON_TIMEUNIT_SEC);
// 				if((ulQueryTime<=ulDataTime && ulDataTime<ulQueryTime+432000) || (ulQueryTime>=ulDataTime && ulQueryTime<ulDataTime+432000))	//偏差5天，考虑月末28日~31日
// 					break;
// 			}
// 		}
// 		if(ucPos<CON_MONTHDATA_NUM)
// 		{//计算位置
// 			usBeginPage	=CON_DFPAGE_HEAD_TER_HISDATA;
// 			ulDataAddr	=sizeof(STHISRUNDATA)*(ucPos+CON_DAYFROZENDATA_NUM+CON_DAYDATA_NUM);
// 			usBeginPage	=usBeginPage+(unsigned short)(ulDataAddr/FPAGE_SIZE);
// 			usBeginPos	=(unsigned short)(ulDataAddr % FPAGE_SIZE);
// 			ucRet=Flash_GetMainMemory(usBeginPage,usBeginPos,sizeof(STHISRUNDATA),(unsigned char *)pstHisRunData);
// 			if(ucRet==EF_NORMAL)
// 				ucRet=RET_SUCCESS;
// 			else
// 				ucRet=RET_ERROR;
// 		}else
// 			ucRet=RET_ERROR;
// 	}
	return ucRet;
}

//------------- 脉冲 -----------------
//unsigned char DataProc_PulseDayData_Move(STDATETIME stDateTime)
//{//日数据处理
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char i=0;
//	for(i=0;i<CON_PULSE_NUM;i++)
//	{
//		//HdConvertWatchDog();
//		DataProc_Update_PulseData(i,stDateTime,&(stHisPulseDayDataThisDay.stHisPulseRunData[i]),CON_STATUS_NO);
//		DataProc_PulseSaveData_Day(i,stDateTime,&(stHisPulseDayDataThisDay.stHisPulseRunData[i]));
//	}
//	return ucRet;
//}

//unsigned char DataProc_PulseMonthData_Move(STDATETIME stDateTime)
//{//月数据处理
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char i=0;
//	for(i=0;i<CON_PULSE_NUM;i++)
//	{
//		//HdConvertWatchDog();
//		DataProc_Update_PulseData(i,stDateTime,&(stHisPulseDayDataThisMonth.stHisPulseRunData[i]),CON_STATUS_NO);
//		DataProc_PulseSaveData_Month(i,stDateTime,&(stHisPulseDayDataThisMonth.stHisPulseRunData[i]));
//	}
//	return ucRet;
//}

//unsigned char DataProc_PulseSaveData_Day(unsigned char ucPulseNo,STDATETIME stDateTime,STHISPULSERUNDATA *pstHisRunData)
//{//保存日数据
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucPos=0xFF,i;
//	unsigned short usBeginPage,usBeginPos;
//	unsigned long ulDataAddr,ulDataTime,ulQueryTime;
//	STDATETIME stTempTime;
//	STDAYLISTINDEX *pstDataIndex=NULL;
//
//	if(ucPulseNo>=CON_PULSE_NUM)
//		return RET_ERROR;
//	if(DP_Get_TerDP_State()==CON_TERDP_STATE_DOWNLOAD)			//数据块用于程序下装数据存储
//		return ucRet;
//	stTempTime=stDateTime;						//stDateTime为日初或者日期变化后的时间，数据要作为上一天的统计结果
//	stTempTime.ucHour=0;
//	stTempTime.ucMinute=0;
//	stTempTime.ucSecond=0;
//	pstDataIndex=&(stHisPulseDataIndex[ucPulseNo].stPulseDayDataIndex);
//	if(pstDataIndex->ucHead != pstDataIndex->ucTail)
//	{//非空
//		for(i=0;i<CON_DAYDATA_NUM;i++)
//		{
//			ucPos=(pstDataIndex->ucTail+i) % CON_DAYDATA_NUM;
//			if(ucPos==pstDataIndex->ucHead)
//			{
//				ucPos=0xFF;
//				break;
//			}
//			ulDataTime=TimeConver(&(pstDataIndex->stDateTimeList[ucPos]),CON_TIMEUNIT_SEC);
//			ulQueryTime=TimeConver(&stTempTime,CON_TIMEUNIT_SEC);
//			if((ulQueryTime<=ulDataTime && ulDataTime<ulQueryTime+3600) || (ulQueryTime>=ulDataTime && ulQueryTime<ulDataTime+3600))
//				break;
//		}
//	}
//	if(ucPos<CON_DAYDATA_NUM)
//	{//更新
//		ulDataAddr	=sizeof(STHISPULSERUNDATA)*ucPos;
//	}else
//	{//增加
//		ulDataAddr	=sizeof(STHISPULSERUNDATA)*pstDataIndex->ucHead;
//		//更新索引
//		pstDataIndex->stDateTimeList[pstDataIndex->ucHead]=stTempTime;
//		pstDataIndex->ucHead++;
//		if(pstDataIndex->ucHead>=CON_DAYDATA_NUM)
//			pstDataIndex->ucHead=0;
//		if(pstDataIndex->ucHead==pstDataIndex->ucTail)
//			pstDataIndex->ucTail++;
//		if(pstDataIndex->ucTail>=CON_DAYDATA_NUM)
//			pstDataIndex->ucTail=0;
//	}
//	//计算位置
//	usBeginPage	= CON_DFPAGE_HEAD_PULSE_HISDATA + CON_DFPAGE_PULSE_HISDATA_PAGENUM*ucPulseNo;
//	usBeginPage	=usBeginPage+(unsigned short)(ulDataAddr/FPAGE_SIZE);
//	usBeginPos	=(unsigned short)(ulDataAddr % FPAGE_SIZE);
//	ucRet=Flash_SetMainMemory((unsigned char *)pstHisRunData,sizeof(STHISPULSERUNDATA),usBeginPage,usBeginPos);
//	if(ucRet==EF_NORMAL)
//		ucRet=RET_SUCCESS;
//	else
//		ucRet=RET_ERROR;
//	return ucRet;
//}

//unsigned char DataProc_PulseSaveData_Month(unsigned char ucPulseNo,STDATETIME stDateTime,STHISPULSERUNDATA *pstHisRunData)
//{//保存月数据
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucPos=0xFF,i;
//	unsigned short usBeginPage,usBeginPos;
//	unsigned long ulDataAddr,ulDataTime,ulQueryTime;
//	STDATETIME stTempTime;
//	STMONTHLISTINDEX *pstDataIndex=NULL;
//
//	if(ucPulseNo>=CON_PULSE_NUM)
//		return RET_ERROR;
//	if(DP_Get_TerDP_State()==CON_TERDP_STATE_DOWNLOAD)			//数据块用于程序下装数据存储
//		return ucRet;
//	stTempTime=stDateTime;
//	stTempTime.ucDay=1;
//	stTempTime.ucHour=0;
//	stTempTime.ucMinute=0;
//	stTempTime.ucSecond=0;
//	pstDataIndex=&(stHisPulseDataIndex[ucPulseNo].stPulseMonthDataIndex);
//	if(pstDataIndex->ucHead != pstDataIndex->ucTail)
//	{//非空
//		for(i=0;i<CON_MONTHDATA_NUM;i++)
//		{
//			ucPos=(pstDataIndex->ucTail+i) % CON_MONTHDATA_NUM;
//			if(ucPos==pstDataIndex->ucHead)
//			{
//				ucPos=0xFF;
//				break;
//			}
//			ulDataTime=TimeConver(&(pstDataIndex->stDateTimeList[ucPos]),CON_TIMEUNIT_SEC);
//			ulQueryTime=TimeConver(&stTempTime,CON_TIMEUNIT_SEC);
//			if((ulQueryTime<=ulDataTime && ulDataTime<ulQueryTime+432000) || (ulQueryTime>=ulDataTime && ulQueryTime<ulDataTime+432000))	//偏差5天，考虑月末28日~31日
//				break;
//		}
//	}
//	if(ucPos<CON_MONTHDATA_NUM)
//	{//更新
//		ulDataAddr	=sizeof(STHISPULSERUNDATA)*(ucPos+CON_DAYDATA_NUM);
//	}else
//	{//增加
//		ulDataAddr	=sizeof(STHISPULSERUNDATA)*(pstDataIndex->ucHead+CON_DAYDATA_NUM);
//		//更新索引
//		pstDataIndex->stDateTimeList[pstDataIndex->ucHead]=stTempTime;
//		pstDataIndex->ucHead++;
//		if(pstDataIndex->ucHead>=CON_MONTHDATA_NUM)
//			pstDataIndex->ucHead=0;
//		if(pstDataIndex->ucHead==pstDataIndex->ucTail)
//			pstDataIndex->ucTail++;
//		if(pstDataIndex->ucTail>=CON_MONTHDATA_NUM)
//			pstDataIndex->ucTail=0;
//	}
//	//计算位置
//	usBeginPage	= CON_DFPAGE_HEAD_PULSE_HISDATA + CON_DFPAGE_PULSE_HISDATA_PAGENUM*ucPulseNo;
//	usBeginPage	=usBeginPage+(unsigned short)(ulDataAddr/FPAGE_SIZE);
//	usBeginPos	=(unsigned short)(ulDataAddr % FPAGE_SIZE);
//	ucRet=Flash_SetMainMemory((unsigned char *)pstHisRunData,sizeof(STHISPULSERUNDATA),usBeginPage,usBeginPos);
//	if(ucRet==EF_NORMAL)
//		ucRet=RET_SUCCESS;
//	else
//		ucRet=RET_ERROR;
//	return ucRet;
//}

//unsigned char DataProc_GetData_Day_Pulse(unsigned char ucPulseNo,STDATETIME stDateTime,STHISPULSERUNDATA *pstHisRunData)
//{//读脉冲日数据
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucPos=0xFF,i=0;
//	unsigned short usBeginPage,usBeginPos;
//	unsigned long ulDataAddr;
//	unsigned long ulQueryTime,ulDataTime,ulThisTime;
//	long lTmp=0;
//	STDATETIME stThisTime=GetCurrentTime();
//	STDAYLISTINDEX *pstDataIndex=NULL;
//	if(ucPulseNo>=CON_PULSE_NUM)
//		return RET_ERROR;
//	stThisTime.ucSecond=59;
//	stThisTime.ucMinute=59;
//	stThisTime.ucHour=23;
//	ulThisTime=TimeConver(&stThisTime,CON_TIMEUNIT_SEC);
//	ulQueryTime=TimeConver(&stDateTime,CON_TIMEUNIT_SEC);
//	if((ulQueryTime<=ulThisTime && ulThisTime<ulQueryTime+3600) || (ulQueryTime>=ulThisTime && ulQueryTime<ulThisTime+3600))
//	{//当日统计数据取当前数据
//		memcpy(pstHisRunData,&(stHisPulseDayDataThisDay.stHisPulseRunData[ucPulseNo]),sizeof(STHISPULSERUNDATA));
//		pstHisRunData->stRecordTime=GetCurrentTime();
//	}else
//	{
//		pstDataIndex=&(stHisPulseDataIndex[ucPulseNo].stPulseDayDataIndex);
//		if(pstDataIndex->ucHead != pstDataIndex->ucTail)
//		{//非空
//			for(i=0;i<CON_DAYDATA_NUM;i++)
//			{
//				ucPos=(pstDataIndex->ucTail+i) % CON_DAYDATA_NUM;
//				if(ucPos==pstDataIndex->ucHead)
//				{
//					ucPos=0xFF;
//					break;
//				}
//				ulDataTime=TimeConver(&(pstDataIndex->stDateTimeList[ucPos]),CON_TIMEUNIT_SEC);
//				if((ulQueryTime<=ulDataTime && ulDataTime<ulQueryTime+3600) || (ulQueryTime>=ulDataTime && ulQueryTime<ulDataTime+3600))
//					break;
//			}
//		}
//		if(ucPos<CON_DAYDATA_NUM)
//		{//计算位置
//			usBeginPage	=CON_DFPAGE_HEAD_PULSE_HISDATA + CON_DFPAGE_PULSE_HISDATA_PAGENUM*ucPulseNo;
//			ulDataAddr	=sizeof(STHISPULSERUNDATA)*ucPos;
//			usBeginPage	=usBeginPage+(unsigned short)(ulDataAddr/FPAGE_SIZE);
//			usBeginPos	=(unsigned short)(ulDataAddr % FPAGE_SIZE);
//			ucRet=Flash_GetMainMemory(usBeginPage,usBeginPos,sizeof(STHISPULSERUNDATA),(unsigned char *)pstHisRunData);
//			if(ucRet==EF_NORMAL)
//				ucRet=RET_SUCCESS;
//			else
//				ucRet=RET_ERROR;
//		}else
//			ucRet=RET_ERROR;
//	}
//	return ucRet;
//}

//unsigned char DataProc_GetData_Month_Pulse(unsigned char ucPulseNo,STDATETIME stDateTime,STHISPULSERUNDATA *pstHisRunData)
//{//读脉冲月数据
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucPos=0xFF,i=0;
//	unsigned short usBeginPage,usBeginPos;
//	unsigned long ulDataAddr;
//	unsigned long ulQueryTime,ulDataTime,ulThisTime;
//	long lTmp=0;
//	STDATETIME stThisTime=GetCurrentTime();
//	STMONTHLISTINDEX *pstDataIndex=NULL;
//	if(ucPulseNo>=CON_PULSE_NUM)
//		return RET_ERROR;
//	stThisTime.ucSecond=59;
//	stThisTime.ucMinute=59;
//	stThisTime.ucHour=23;
//	stThisTime.ucDay=28;
//	ulThisTime=TimeConver(&stThisTime,CON_TIMEUNIT_SEC);
//	ulQueryTime=TimeConver(&stDateTime,CON_TIMEUNIT_SEC);
//	if((ulQueryTime<=ulThisTime && ulThisTime<ulQueryTime+432000) || (ulQueryTime>=ulThisTime && ulQueryTime<ulThisTime+432000))
//	{//当月统计数据取当前数据
//		memcpy(pstHisRunData,&(stHisPulseDayDataThisMonth.stHisPulseRunData[ucPulseNo]),sizeof(STHISPULSERUNDATA));
//		pstHisRunData->stRecordTime=GetCurrentTime();
//	}else
//	{
//		pstDataIndex=&(stHisPulseDataIndex[ucPulseNo].stPulseMonthDataIndex);
//		if(pstDataIndex->ucHead != pstDataIndex->ucTail)
//		{//非空
//			for(i=0;i<CON_MONTHDATA_NUM;i++)
//			{
//				ucPos=(pstDataIndex->ucTail+i) % CON_MONTHDATA_NUM;
//				if(ucPos==pstDataIndex->ucHead)
//				{
//					ucPos=0xFF;
//					break;
//				}
//				ulDataTime=TimeConver(&(pstDataIndex->stDateTimeList[ucPos]),CON_TIMEUNIT_SEC);
//				if((ulQueryTime<=ulDataTime && ulDataTime<ulQueryTime+432000) || (ulQueryTime>=ulDataTime && ulQueryTime<ulDataTime+432000))	//偏差5天，考虑月末28日~31日
//					break;
//			}
//		}
//		if(ucPos<CON_MONTHDATA_NUM)
//		{//计算位置
//			usBeginPage	=CON_DFPAGE_HEAD_PULSE_HISDATA + CON_DFPAGE_PULSE_HISDATA_PAGENUM*ucPulseNo;
//			ulDataAddr	=sizeof(STHISPULSERUNDATA)*(ucPos+CON_DAYDATA_NUM);
//			usBeginPage	=usBeginPage+(unsigned short)(ulDataAddr/FPAGE_SIZE);
//			usBeginPos	=(unsigned short)(ulDataAddr % FPAGE_SIZE);
//			ucRet=Flash_GetMainMemory(usBeginPage,usBeginPos,sizeof(STHISPULSERUNDATA),(unsigned char *)pstHisRunData);
//			if(ucRet==EF_NORMAL)
//				ucRet=RET_SUCCESS;
//			else
//				ucRet=RET_ERROR;
//		}else
//			ucRet=RET_ERROR;
//	}
//	return ucRet;
//}

//unsigned char DataProc_PulseDayData_Stat_Clear(void)
//{//初始化脉冲日数据统计
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char i=0;
//	for(i=0;i<CON_PULSE_NUM;i++)
//	{
//		//HdConvertWatchDog();
//		stHisPulseDayDataThisDay.stHisPulseRunData[i].ulHisPowTotal=CONINVALIDVAL;
//		stHisPulseDayDataThisDay.stHisPulseRunData[i].ulHisPowFee1=CONINVALIDVAL;
//		stHisPulseDayDataThisDay.stHisPulseRunData[i].ulHisPowFee2=CONINVALIDVAL;
//		stHisPulseDayDataThisDay.stHisPulseRunData[i].ulHisPowFee3=CONINVALIDVAL;
//		stHisPulseDayDataThisDay.stHisPulseRunData[i].ulHisPowFee4=CONINVALIDVAL;
//
//		stHisPulseDayDataThisDay.stHisPulseRunData[i].ulHisLastDayPowTotal=CONINVALIDVAL;
//		stHisPulseDayDataThisDay.stHisPulseRunData[i].ulHisLastDayPowFee1=CONINVALIDVAL;
//		stHisPulseDayDataThisDay.stHisPulseRunData[i].ulHisLastDayPowFee2=CONINVALIDVAL;
//		stHisPulseDayDataThisDay.stHisPulseRunData[i].ulHisLastDayPowFee3=CONINVALIDVAL;
//		stHisPulseDayDataThisDay.stHisPulseRunData[i].ulHisLastDayPowFee4=CONINVALIDVAL;
//
//		DataProc_Pulse_Stat_Clear_InstantPow(&(stHisPulseDayDataThisDay.stHisPulseRunData[i]));
//		DataProc_Pulse_Stat_Clear_Demand(&(stHisPulseDayDataThisDay.stHisPulseRunData[i]));
//	}
//	return ucRet;
//}
//unsigned char DataProc_PulseMonthData_Stat_Clear(void)
//{//初始化脉冲月数据统计
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char i=0;
//	for(i=0;i<CON_PULSE_NUM;i++)
//	{
//		//HdConvertWatchDog();
//		stHisPulseDayDataThisMonth.stHisPulseRunData[i].ulHisPowTotal=CONINVALIDVAL;
//		stHisPulseDayDataThisMonth.stHisPulseRunData[i].ulHisPowFee1=CONINVALIDVAL;
//		stHisPulseDayDataThisMonth.stHisPulseRunData[i].ulHisPowFee2=CONINVALIDVAL;
//		stHisPulseDayDataThisMonth.stHisPulseRunData[i].ulHisPowFee3=CONINVALIDVAL;
//		stHisPulseDayDataThisMonth.stHisPulseRunData[i].ulHisPowFee4=CONINVALIDVAL;
//
//		stHisPulseDayDataThisMonth.stHisPulseRunData[i].ulHisLastDayPowTotal=CONINVALIDVAL;
//		stHisPulseDayDataThisMonth.stHisPulseRunData[i].ulHisLastDayPowFee1=CONINVALIDVAL;
//		stHisPulseDayDataThisMonth.stHisPulseRunData[i].ulHisLastDayPowFee2=CONINVALIDVAL;
//		stHisPulseDayDataThisMonth.stHisPulseRunData[i].ulHisLastDayPowFee3=CONINVALIDVAL;
//		stHisPulseDayDataThisMonth.stHisPulseRunData[i].ulHisLastDayPowFee4=CONINVALIDVAL;
//
//		DataProc_Pulse_Stat_Clear_InstantPow(&(stHisPulseDayDataThisMonth.stHisPulseRunData[i]));
//		DataProc_Pulse_Stat_Clear_Demand(&(stHisPulseDayDataThisMonth.stHisPulseRunData[i]));
//	}
//	return ucRet;
//}

//unsigned char DataProc_Pulse_Stat_Clear_InstantPow(STHISPULSERUNDATA *pstHisRunData)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	ClearHisVal(&(pstHisRunData->stHisMaxInstantPow));
//	pstHisRunData->usHisZeroTimeInstantPow=0;
//	return ucRet;
//}

//unsigned char DataProc_Pulse_Stat_Clear_Demand(STHISPULSERUNDATA *pstHisRunData)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	ClearHisVal(&(pstHisRunData->stHisMaxDemand));
//	return ucRet;
//}
//
//unsigned char DataProc_PulseDayData_Stat(STDATETIME stDateTime)
//{//日数据统计
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char i=0;
//	for(i=0;i<CON_PULSE_NUM;i++)
//	{
//		//HdConvertWatchDog();
//		DataProc_Update_PulseData(i,stDateTime,&(stHisPulseDayDataThisDay.stHisPulseRunData[i]),CON_STATUS_NO);
//
//		DayData_Stat_InstantPow_Pulse(i,stDateTime);
//		DayData_Stat_Demand_Pulse(i,stDateTime);
//	}
//	return ucRet;
//}
//
//unsigned char DataProc_PulseMonthData_Stat(STDATETIME stDateTime)
//{//月数据统计
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<CON_PULSE_NUM;i++)
//	{
//		//HdConvertWatchDog();
//		DataProc_Update_PulseData(i,stDateTime,&(stHisPulseDayDataThisMonth.stHisPulseRunData[i]),CON_STATUS_NO);
//
//		MonthData_Stat_InstantPow_Pulse(i,stDateTime);
//		MonthData_Stat_Demand_Pulse(i,stDateTime);
//	}
//	return ucRet;
//}

//unsigned char DataProc_PulseMonthData_FlashLast(STDATETIME stDateTime)
//{//月数据统计
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char i=0;
//	STHISPULSERUNDATA *pstHisRunData;
//	unsigned char ucMPNo;
//	STHISVAL stHisVal0,stHisVal1,stHisVal2,stHisVal3,stHisVal4;
//	STPULSEPARA		stPulsePara;
//	for(i=0;i<CON_PULSE_NUM;i++)
//	{
//		ucMPNo=RunPara_GetPulse_MPNo(i);
//		stPulsePara = RunPara_GetPulsePara(i);
//		switch(stPulsePara.ucAttr)
//		{
//		case CON_PULSEATTR_DIRPOW:
//			stHisVal0=Get_History_DirPower_byMPNo(ucMPNo,0,0);
//			stHisVal1=Get_History_DirPower_byMPNo(ucMPNo,0,1);
//			stHisVal2=Get_History_DirPower_byMPNo(ucMPNo,0,2);
//			stHisVal3=Get_History_DirPower_byMPNo(ucMPNo,0,3);
//			stHisVal4=Get_History_DirPower_byMPNo(ucMPNo,0,4);
//			break;
//		case CON_PULSEATTR_DIRVAR:
//			stHisVal0=Get_History_DirVar_byMPNo(ucMPNo,0,0);
//			stHisVal1=Get_History_DirVar_byMPNo(ucMPNo,0,1);
//			stHisVal2=Get_History_DirVar_byMPNo(ucMPNo,0,2);
//			stHisVal3=Get_History_DirVar_byMPNo(ucMPNo,0,3);
//			stHisVal4=Get_History_DirVar_byMPNo(ucMPNo,0,4);
//			break;
//		case CON_PULSEATTR_REVPOW:
//			stHisVal0=Get_History_UnDirPower_byMPNo(ucMPNo,0,0);
//			stHisVal1=Get_History_UnDirPower_byMPNo(ucMPNo,0,1);
//			stHisVal2=Get_History_UnDirPower_byMPNo(ucMPNo,0,2);
//			stHisVal3=Get_History_UnDirPower_byMPNo(ucMPNo,0,3);
//			stHisVal4=Get_History_UnDirPower_byMPNo(ucMPNo,0,4);
//			break;
//		case CON_PULSEATTR_REVVAR:
//			stHisVal0=Get_History_UnDirVar_byMPNo(ucMPNo,0,0);
//			stHisVal1=Get_History_UnDirVar_byMPNo(ucMPNo,0,1);
//			stHisVal2=Get_History_UnDirVar_byMPNo(ucMPNo,0,2);
//			stHisVal3=Get_History_UnDirVar_byMPNo(ucMPNo,0,3);
//			stHisVal4=Get_History_UnDirVar_byMPNo(ucMPNo,0,4);
//			break;
//		default:
//			stHisVal0=Get_History_DirPower_byMPNo(ucMPNo,0,0);
//			stHisVal1=Get_History_DirPower_byMPNo(ucMPNo,0,1);
//			stHisVal2=Get_History_DirPower_byMPNo(ucMPNo,0,2);
//			stHisVal3=Get_History_DirPower_byMPNo(ucMPNo,0,3);
//			stHisVal4=Get_History_DirPower_byMPNo(ucMPNo,0,4);
//			break;
//		}
//		pstHisRunData=&(stHisPulseDayDataThisMonth.stHisPulseRunData[i]);
//		if(pstHisRunData->ulHisLastDayPowTotal==CONINVALIDVAL)
//		{//上一天的数据无效则刷新之
//			pstHisRunData->ulHisLastDayPowTotal=stHisVal0.lVal;
//			pstHisRunData->ulHisLastDayPowFee1=stHisVal1.lVal;
//			pstHisRunData->ulHisLastDayPowFee2=stHisVal2.lVal;
//			pstHisRunData->ulHisLastDayPowFee3=stHisVal3.lVal;
//			pstHisRunData->ulHisLastDayPowFee4=stHisVal4.lVal;
//		}
//	}
//	return ucRet;
//}

//unsigned char DataProc_Update_PulseData(unsigned char ucPulseNo,STDATETIME stDateTime,STHISPULSERUNDATA *pstHisRunData,unsigned char ucReflashLastDay)
//{//数据更新
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucMPNo;
//	STHISVAL stHisVal;
//	STHISTIME stHisTime;
//	STPULSEPARA		stPulsePara;
//
//	ucMPNo=RunPara_GetPulse_MPNo(ucPulseNo);
//	pstHisRunData->stRecordTime=stDateTime;
//
//	stPulsePara = RunPara_GetPulsePara(ucPulseNo);
//	switch(stPulsePara.ucAttr)
//	{
//	case CON_PULSEATTR_DIRPOW:
//		stHisVal=Get_History_DirPower_byMPNo(ucMPNo,0,0);	pstHisRunData->ulHisPowTotal=stHisVal.lVal;	//	电量
//		stHisVal=Get_History_DirPower_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisPowFee1=stHisVal.lVal;
//		stHisVal=Get_History_DirPower_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisPowFee2=stHisVal.lVal;
//		stHisVal=Get_History_DirPower_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisPowFee3=stHisVal.lVal;
//		stHisVal=Get_History_DirPower_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisPowFee4=stHisVal.lVal;
//
//		stHisVal=Get_History_DirPowMaxDemand_byMPNo(ucMPNo,0,0);			pstHisRunData->ulHisPowMaxDemand=stHisVal.lVal;		//	正向有功最大需量
//		stHisVal=Get_History_DirPowMaxDemand_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisPowMaxDemandFee1=stHisVal.lVal;
//		stHisVal=Get_History_DirPowMaxDemand_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisPowMaxDemandFee2=stHisVal.lVal;
//		stHisVal=Get_History_DirPowMaxDemand_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisPowMaxDemandFee3=stHisVal.lVal;
//		stHisVal=Get_History_DirPowMaxDemand_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisPowMaxDemandFee4=stHisVal.lVal;
//
//		stHisTime=Get_History_DirPowMaxDemandTime_byMPNo(ucMPNo,0,0);			pstHisRunData->stHisPowMaxDemandTime=stHisTime.stHisTime;
//		stHisTime=Get_History_DirPowMaxDemandTime_byMPNo(ucMPNo,0,1);		pstHisRunData->stHisPowMaxDemandTimeFee1=stHisTime.stHisTime;
//		stHisTime=Get_History_DirPowMaxDemandTime_byMPNo(ucMPNo,0,2);		pstHisRunData->stHisPowMaxDemandTimeFee2=stHisTime.stHisTime;
//		stHisTime=Get_History_DirPowMaxDemandTime_byMPNo(ucMPNo,0,3);		pstHisRunData->stHisPowMaxDemandTimeFee3=stHisTime.stHisTime;
//		stHisTime=Get_History_DirPowMaxDemandTime_byMPNo(ucMPNo,0,4);		pstHisRunData->stHisPowMaxDemandTimeFee4=stHisTime.stHisTime;
//		break;
//	case CON_PULSEATTR_DIRVAR:
//		stHisVal=Get_History_DirVar_byMPNo(ucMPNo,0,0);		pstHisRunData->ulHisPowTotal=stHisVal.lVal;	//	电量
//		stHisVal=Get_History_DirVar_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisPowFee1=stHisVal.lVal;
//		stHisVal=Get_History_DirVar_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisPowFee2=stHisVal.lVal;
//		stHisVal=Get_History_DirVar_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisPowFee3=stHisVal.lVal;
//		stHisVal=Get_History_DirVar_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisPowFee4=stHisVal.lVal;
//
//		stHisVal=Get_History_DirVarMaxDemand_byMPNo(ucMPNo,0,0);			pstHisRunData->ulHisPowMaxDemand=stHisVal.lVal;		//	正向有功最大需量
//		stHisVal=Get_History_DirVarMaxDemand_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisPowMaxDemandFee1=stHisVal.lVal;
//		stHisVal=Get_History_DirVarMaxDemand_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisPowMaxDemandFee2=stHisVal.lVal;
//		stHisVal=Get_History_DirVarMaxDemand_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisPowMaxDemandFee3=stHisVal.lVal;
//		stHisVal=Get_History_DirVarMaxDemand_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisPowMaxDemandFee4=stHisVal.lVal;
//
//		stHisTime=Get_History_DirVarMaxDemandTime_byMPNo(ucMPNo,0,0);			pstHisRunData->stHisPowMaxDemandTime=stHisTime.stHisTime;
//		stHisTime=Get_History_DirVarMaxDemandTime_byMPNo(ucMPNo,0,1);		pstHisRunData->stHisPowMaxDemandTimeFee1=stHisTime.stHisTime;
//		stHisTime=Get_History_DirVarMaxDemandTime_byMPNo(ucMPNo,0,2);		pstHisRunData->stHisPowMaxDemandTimeFee2=stHisTime.stHisTime;
//		stHisTime=Get_History_DirVarMaxDemandTime_byMPNo(ucMPNo,0,3);		pstHisRunData->stHisPowMaxDemandTimeFee3=stHisTime.stHisTime;
//		stHisTime=Get_History_DirVarMaxDemandTime_byMPNo(ucMPNo,0,4);		pstHisRunData->stHisPowMaxDemandTimeFee4=stHisTime.stHisTime;
//		break;
//	case CON_PULSEATTR_REVPOW:
//		stHisVal=Get_History_UnDirPower_byMPNo(ucMPNo,0,0);		pstHisRunData->ulHisPowTotal=stHisVal.lVal;	//	电量
//		stHisVal=Get_History_UnDirPower_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisPowFee1=stHisVal.lVal;
//		stHisVal=Get_History_UnDirPower_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisPowFee2=stHisVal.lVal;
//		stHisVal=Get_History_UnDirPower_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisPowFee3=stHisVal.lVal;
//		stHisVal=Get_History_UnDirPower_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisPowFee4=stHisVal.lVal;
//
//		stHisVal=Get_History_UnDirPowMaxDemand_byMPNo(ucMPNo,0,0);			pstHisRunData->ulHisPowMaxDemand=stHisVal.lVal;		//	正向有功最大需量
//		stHisVal=Get_History_UnDirPowMaxDemand_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisPowMaxDemandFee1=stHisVal.lVal;
//		stHisVal=Get_History_UnDirPowMaxDemand_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisPowMaxDemandFee2=stHisVal.lVal;
//		stHisVal=Get_History_UnDirPowMaxDemand_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisPowMaxDemandFee3=stHisVal.lVal;
//		stHisVal=Get_History_UnDirPowMaxDemand_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisPowMaxDemandFee4=stHisVal.lVal;
//
//		stHisTime=Get_History_UnDirPowMaxDemandTime_byMPNo(ucMPNo,0,0);			pstHisRunData->stHisPowMaxDemandTime=stHisTime.stHisTime;
//		stHisTime=Get_History_UnDirPowMaxDemandTime_byMPNo(ucMPNo,0,1);		pstHisRunData->stHisPowMaxDemandTimeFee1=stHisTime.stHisTime;
//		stHisTime=Get_History_UnDirPowMaxDemandTime_byMPNo(ucMPNo,0,2);		pstHisRunData->stHisPowMaxDemandTimeFee2=stHisTime.stHisTime;
//		stHisTime=Get_History_UnDirPowMaxDemandTime_byMPNo(ucMPNo,0,3);		pstHisRunData->stHisPowMaxDemandTimeFee3=stHisTime.stHisTime;
//		stHisTime=Get_History_UnDirPowMaxDemandTime_byMPNo(ucMPNo,0,4);		pstHisRunData->stHisPowMaxDemandTimeFee4=stHisTime.stHisTime;
//		break;
//	case CON_PULSEATTR_REVVAR:
//		stHisVal=Get_History_UnDirVar_byMPNo(ucMPNo,0,0);	pstHisRunData->ulHisPowTotal=stHisVal.lVal;	//	电量
//		stHisVal=Get_History_UnDirVar_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisPowFee1=stHisVal.lVal;
//		stHisVal=Get_History_UnDirVar_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisPowFee2=stHisVal.lVal;
//		stHisVal=Get_History_UnDirVar_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisPowFee3=stHisVal.lVal;
//		stHisVal=Get_History_UnDirVar_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisPowFee4=stHisVal.lVal;
//
//		stHisVal=Get_History_UnDirVarMaxDemand_byMPNo(ucMPNo,0,0);			pstHisRunData->ulHisPowMaxDemand=stHisVal.lVal;		//	正向有功最大需量
//		stHisVal=Get_History_UnDirVarMaxDemand_byMPNo(ucMPNo,0,1);		pstHisRunData->ulHisPowMaxDemandFee1=stHisVal.lVal;
//		stHisVal=Get_History_UnDirVarMaxDemand_byMPNo(ucMPNo,0,2);		pstHisRunData->ulHisPowMaxDemandFee2=stHisVal.lVal;
//		stHisVal=Get_History_UnDirVarMaxDemand_byMPNo(ucMPNo,0,3);		pstHisRunData->ulHisPowMaxDemandFee3=stHisVal.lVal;
//		stHisVal=Get_History_UnDirVarMaxDemand_byMPNo(ucMPNo,0,4);		pstHisRunData->ulHisPowMaxDemandFee4=stHisVal.lVal;
//
//		stHisTime=Get_History_UnDirVarMaxDemandTime_byMPNo(ucMPNo,0,0);			pstHisRunData->stHisPowMaxDemandTime=stHisTime.stHisTime;
//		stHisTime=Get_History_UnDirVarMaxDemandTime_byMPNo(ucMPNo,0,1);		pstHisRunData->stHisPowMaxDemandTimeFee1=stHisTime.stHisTime;
//		stHisTime=Get_History_UnDirVarMaxDemandTime_byMPNo(ucMPNo,0,2);		pstHisRunData->stHisPowMaxDemandTimeFee2=stHisTime.stHisTime;
//		stHisTime=Get_History_UnDirVarMaxDemandTime_byMPNo(ucMPNo,0,3);		pstHisRunData->stHisPowMaxDemandTimeFee3=stHisTime.stHisTime;
//		stHisTime=Get_History_UnDirVarMaxDemandTime_byMPNo(ucMPNo,0,4);		pstHisRunData->stHisPowMaxDemandTimeFee4=stHisTime.stHisTime;
//		break;
//	default:
//		break;
//	}
//	if(ucReflashLastDay==CON_STATUS_YES || pstHisRunData->ulHisLastDayPowTotal==CONINVALIDVAL)
//	{//上一天的数据无效则刷新之
//		pstHisRunData->ulHisLastDayPowTotal=pstHisRunData->ulHisPowTotal;
//		pstHisRunData->ulHisLastDayPowFee1=pstHisRunData->ulHisPowFee1;
//		pstHisRunData->ulHisLastDayPowFee2=pstHisRunData->ulHisPowFee2;
//		pstHisRunData->ulHisLastDayPowFee3=pstHisRunData->ulHisPowFee3;
//		pstHisRunData->ulHisLastDayPowFee4=pstHisRunData->ulHisPowFee4;
//	}
//	return ucRet;
//}

//unsigned char DayData_Stat_InstantPow_Pulse(unsigned char ucPulseNo,STDATETIME stDateTime)
//{//最大功率日统计
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucMPNo;
//	STHISVAL stHisVal;
//	ucMPNo=RunPara_GetPulse_MPNo(ucPulseNo);
//	stHisVal=Get_History_InstantPow_byMPNo(ucMPNo,0,0);
//	DataProc_Stat_GetMax(stHisVal,&(stHisPulseDayDataThisDay.stHisPulseRunData[ucPulseNo].stHisMaxInstantPow),stDateTime);
//	if(stHisVal.lVal==0)
//		stHisPulseDayDataThisDay.stHisPulseRunData[ucPulseNo].usHisZeroTimeInstantPow++;			//1分钟检测一次
//	return ucRet;
//}
//
//unsigned char MonthData_Stat_InstantPow_Pulse(unsigned char ucPulseNo,STDATETIME stDateTime)
//{//功率月统计
//	unsigned char ucRet=RET_SUCCESS;
//	DataProc_Stat_GetMax(stHisPulseDayDataThisDay.stHisPulseRunData[ucPulseNo].stHisMaxInstantPow,
//		&(stHisPulseDayDataThisMonth.stHisPulseRunData[ucPulseNo].stHisMaxInstantPow),
//		stHisPulseDayDataThisDay.stHisPulseRunData[ucPulseNo].stHisMaxInstantPow.stDateTime);
//	stHisPulseDayDataThisMonth.stHisPulseRunData[ucPulseNo].usHisZeroTimeInstantPow+=stHisPulseDayDataThisDay.stHisPulseRunData[ucPulseNo].usHisZeroTimeInstantPow;
//	return ucRet;
//}
//
//unsigned char DayData_Stat_Demand_Pulse(unsigned char ucPulseNo,STDATETIME stDateTime)
//{//最大需量日统计
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucMPNo;
//	STHISVAL stHisVal;
//	ucMPNo=RunPara_GetPulse_MPNo(ucPulseNo);
//	stHisVal=Get_History_InstantPow_byMPNo(ucMPNo,0,0);
//	DataProc_Stat_GetMax(stHisVal,&(stHisPulseDayDataThisDay.stHisPulseRunData[ucPulseNo].stHisMaxDemand),stDateTime);
//	return ucRet;
//}

//unsigned char MonthData_Stat_Demand_Pulse(unsigned char ucPulseNo,STDATETIME stDateTime)
//{//最大需量月统计
//	unsigned char ucRet=RET_SUCCESS;
//	DataProc_Stat_GetMax(stHisPulseDayDataThisDay.stHisPulseRunData[ucPulseNo].stHisMaxDemand,
//		&(stHisPulseDayDataThisMonth.stHisPulseRunData[ucPulseNo].stHisMaxDemand),
//		stHisPulseDayDataThisDay.stHisPulseRunData[ucPulseNo].stHisMaxDemand.stDateTime);
//	return ucRet;
//}

unsigned char DataProc_DayFrozenData_MeterCopy(STDATETIME stDateTime)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char i=0;
	unsigned char ucMPNo;
//	STRS485TASK		stRS485Task;
//	unsigned char ucPortNo;
//
	for(i=0;i<CON_METER_NUM;i++)
	{
		ucMPNo=RunPara_GetMeter_MPNo(i);
//		//HdConvertWatchDog();
		if(ucMPNo!=CON_MPNO_INVALID)
		{//填发抄表命令
            usTimerCopyFlag[i] = CON_STATUS_YES_2BYTE;//统一出口填数据项
//            Meter_FillTimerCommand(CONRS485SOURTIMER, ucMPNo);//azh

//			//HdConvertWatchDog();
//			ucPortNo=RunPara_GetMeterPara_PortNo_byMPNo(ucMPNo);
//
//			SetRS485TaskPer(&stRS485Task,
//				CONRS485SOURTIMER,CON_SEQNO_NULL,CON_RS485_REPORT_TYPE_GET,CON_DI_TYPE_MPUNDIRPOW_BLOCK,ucMPNo,NULL);
//			ucRet=RS485_FillCommand(&stRS485Task,2,ucPortNo);
//			SetRS485TaskPer(&stRS485Task,
//				CONRS485SOURTIMER,CON_SEQNO_NULL,CON_RS485_REPORT_TYPE_GET,CON_DI_TYPE_MPDIRPOW_BLOCK,ucMPNo,NULL);
//			ucRet=RS485_FillCommand(&stRS485Task,2,ucPortNo);
//			SetRS485TaskPer(&stRS485Task,
//				CONRS485SOURTIMER,CON_SEQNO_NULL,CON_RS485_REPORT_TYPE_GET,CON_DI_TYPE_MPDIRVAR_BLOCK,ucMPNo,NULL);
//			ucRet=RS485_FillCommand(&stRS485Task,2,ucPortNo);
//			SetRS485TaskPer(&stRS485Task,
//				CONRS485SOURTIMER,CON_SEQNO_NULL,CON_RS485_REPORT_TYPE_GET,CON_DI_TYPE_MPUNDIRVAR_BLOCK,ucMPNo,NULL);
//			ucRet=RS485_FillCommand(&stRS485Task,2,ucPortNo);
//
//			SetRS485TaskPer(&stRS485Task,
//				CONRS485SOURTIMER,CON_SEQNO_NULL,CON_RS485_REPORT_TYPE_GET,CON_DI_TYPE_MPVAR1_BLOCK,ucMPNo,NULL);
//			ucRet=RS485_FillCommand(&stRS485Task,2,ucPortNo);
//			SetRS485TaskPer(&stRS485Task,
//				CONRS485SOURTIMER,CON_SEQNO_NULL,CON_RS485_REPORT_TYPE_GET,CON_DI_TYPE_MPVAR4_BLOCK,ucMPNo,NULL);
//			ucRet=RS485_FillCommand(&stRS485Task,2,ucPortNo);
//			SetRS485TaskPer(&stRS485Task,
//				CONRS485SOURTIMER,CON_SEQNO_NULL,CON_RS485_REPORT_TYPE_GET,CON_DI_TYPE_MPVAR2_BLOCK,ucMPNo,NULL);
//			ucRet=RS485_FillCommand(&stRS485Task,2,ucPortNo);
//			SetRS485TaskPer(&stRS485Task,
//				CONRS485SOURTIMER,CON_SEQNO_NULL,CON_RS485_REPORT_TYPE_GET,CON_DI_TYPE_MPVAR3_BLOCK,ucMPNo,NULL);
//			ucRet=RS485_FillCommand(&stRS485Task,2,ucPortNo);
//
//			SetRS485TaskPer(&stRS485Task,
//				CONRS485SOURTIMER,CON_SEQNO_NULL,CON_RS485_REPORT_TYPE_GET,CON_DI_TYPE_MPDIRPOWMAXDEMAND_BLOCK,ucMPNo,NULL);
//			ucRet=RS485_FillCommand(&stRS485Task,2,ucPortNo);
//			SetRS485TaskPer(&stRS485Task,
//				CONRS485SOURTIMER,CON_SEQNO_NULL,CON_RS485_REPORT_TYPE_GET,CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_BLOCK,ucMPNo,NULL);
//			ucRet=RS485_FillCommand(&stRS485Task,2,ucPortNo);
//			SetRS485TaskPer(&stRS485Task,
//				CONRS485SOURTIMER,CON_SEQNO_NULL,CON_RS485_REPORT_TYPE_GET,CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_BLOCK,ucMPNo,NULL);
//			ucRet=RS485_FillCommand(&stRS485Task,2,ucPortNo);
//			SetRS485TaskPer(&stRS485Task,
//				CONRS485SOURTIMER,CON_SEQNO_NULL,CON_RS485_REPORT_TYPE_GET,CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_BLOCK,ucMPNo,NULL);
//			ucRet=RS485_FillCommand(&stRS485Task,2,ucPortNo);
//			//HdConvertWatchDog();
//			SetRS485TaskPer(&stRS485Task,
//				CONRS485SOURTIMER,CON_SEQNO_NULL,CON_RS485_REPORT_TYPE_GET,CON_DI_TYPE_MPDIRVARMAXDEMAND_BLOCK,ucMPNo,NULL);
//			ucRet=RS485_FillCommand(&stRS485Task,2,ucPortNo);
//			SetRS485TaskPer(&stRS485Task,
//				CONRS485SOURTIMER,CON_SEQNO_NULL,CON_RS485_REPORT_TYPE_GET,CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_BLOCK,ucMPNo,NULL);
//			ucRet=RS485_FillCommand(&stRS485Task,2,ucPortNo);
//			SetRS485TaskPer(&stRS485Task,
//				CONRS485SOURTIMER,CON_SEQNO_NULL,CON_RS485_REPORT_TYPE_GET,CON_DI_TYPE_MPUNDIRVARMAXDEMAND_BLOCK,ucMPNo,NULL);
//			ucRet=RS485_FillCommand(&stRS485Task,2,ucPortNo);
//			SetRS485TaskPer(&stRS485Task,
//				CONRS485SOURTIMER,CON_SEQNO_NULL,CON_RS485_REPORT_TYPE_GET,CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_BLOCK,ucMPNo,NULL);
//			ucRet=RS485_FillCommand(&stRS485Task,2,ucPortNo);
//			//HdConvertWatchDog();
		}
	}
	return ucRet;
}
/******************************************************************************
** 函数名称: void save_daymonth_data_info(unsigned char meterno, unsigned char type, unsigned char maxnum)
** 函数功能: 保存日月抄表日冻结数据相关的信息：指针及每条的时间
** 入口参数: ucMeterNo:电表序号 type:日、月、抄表日冻结 maxnum：最大条数
** 返 回 值: 无
** 说    明: 
**           
******************************************************************************/
void save_daymonth_data_info (unsigned char ucMeterNo, unsigned char type, unsigned char maxnum)
{
 	unsigned char buf[8],m=0;
 	unsigned long ucAddr=0;
 	STDAYLISTINDEX *pstDayDataIndex=NULL;
 	STMONTHLISTINDEX *pstMonDataIndex=NULL;
 	STDAYFROZENLISTINDEX *pstFroDataIndex=NULL;
 	 	
    if(ucMeterNo >= CON_METER_NUM)
    {
        return;
    }    
    ucAddr = ucMeterNo * CON_DAYMONTH_DATA_LEN_MP + CON_DAY_MONTH_INFO_BASE_ADDR;
    switch(type)
    {
        case CON_DAYDATA_TYPE://日冻结
            pstDayDataIndex=&(stHisRunDataIndex[ucMeterNo].stDayDataIndex);
            if((pstDayDataIndex->ucHead < maxnum) && (pstDayDataIndex->ucTail < maxnum))
            {
                buf[0] = pstDayDataIndex->ucHead;
                buf[1] = pstDayDataIndex->ucTail;
                buf[CON_DAYMONTH_INDEX_LEN-1] = addverify(buf, (CON_DAYMONTH_INDEX_LEN-1));
                file_write(ucAddr+CON_DAYDATA_INDEX_ADDR, buf, CON_DAYMONTH_INDEX_LEN);                 
                if(pstDayDataIndex->ucHead == 0)//这边调用函数的时候指针已经+1了
                {
                    m = maxnum;
                }
                else
                {
                    m = pstDayDataIndex->ucHead;
                } 
                m--;    
                   
                ucAddr += m*CON_DAYMONTH_TIME_LEN + CON_DAYDATA_TIME_ADDR;
                memcpy(buf, &pstDayDataIndex->stDateTimeList[m], (CON_DAYMONTH_TIME_LEN-1));
                buf[CON_DAYMONTH_TIME_LEN-1] = addverify(buf, (CON_DAYMONTH_TIME_LEN-1));
                file_write(ucAddr, buf, CON_DAYMONTH_TIME_LEN);  
            }    
            
            break;
        case CON_MONTHDATA_TYPE://月冻结
            pstMonDataIndex=&(stHisRunDataIndex[ucMeterNo].stMonthDataIndex);
            if((pstMonDataIndex->ucHead < maxnum) && (pstMonDataIndex->ucTail < maxnum))
            {
                buf[0] = pstMonDataIndex->ucHead;
                buf[1] = pstMonDataIndex->ucTail;
                buf[CON_DAYMONTH_INDEX_LEN-1] = addverify(buf, (CON_DAYMONTH_INDEX_LEN-1));
                file_write(ucAddr+CON_MONTHDATA_INDEX_ADDR, buf, CON_DAYMONTH_INDEX_LEN);                 
                if(pstMonDataIndex->ucHead == 0)//这边调用函数的时候指针已经+1了
                {
                    m = maxnum;
                }
                else
                {
                    m = pstMonDataIndex->ucHead;
                } 
                m--;    
                   
                ucAddr += m*CON_DAYMONTH_TIME_LEN + CON_MONTHDATA_TIME_ADDR;
                memcpy(buf, &pstMonDataIndex->stDateTimeList[m], (CON_DAYMONTH_TIME_LEN-1));
                buf[CON_DAYMONTH_TIME_LEN-1] = addverify(buf, (CON_DAYMONTH_TIME_LEN-1));
                file_write(ucAddr, buf, CON_DAYMONTH_TIME_LEN);  
            }                
            break;        
        case CON_FROZEDATA_TYPE://抄表日冻结
            pstFroDataIndex=&(stHisRunDataIndex[ucMeterNo].stDayFrozenDataIndex);
            if((pstFroDataIndex->ucHead < maxnum) && (pstFroDataIndex->ucTail < maxnum))
            {
                buf[0] = pstFroDataIndex->ucHead;
                buf[1] = pstFroDataIndex->ucTail;
                buf[CON_DAYMONTH_INDEX_LEN-1] = addverify(buf, (CON_DAYMONTH_INDEX_LEN-1));
                file_write(ucAddr+CON_DAYFROZENDATA_INDEX_ADDR, buf, CON_DAYMONTH_INDEX_LEN);                 
                if(pstFroDataIndex->ucHead == 0)//这边调用函数的时候指针已经+1了
                {
                    m = maxnum;
                }
                else
                {
                    m = pstFroDataIndex->ucHead;
                } 
                m--;    
                   
                ucAddr += m*CON_DAYMONTH_TIME_LEN + CON_DAYFROZENDATA_TIME_ADDR;
                memcpy(buf, &pstFroDataIndex->stDateTimeList[m], (CON_DAYMONTH_TIME_LEN-1));
                buf[CON_DAYMONTH_TIME_LEN-1] = addverify(buf, (CON_DAYMONTH_TIME_LEN-1));
                file_write(ucAddr, buf, CON_DAYMONTH_TIME_LEN);  
            }                
            break;  
        default:
            return;          
    }

}
/******************************************************************************
** 函数名称: void init_daymonth_data_info(unsigned char meterno, unsigned char type, unsigned char maxnum)
** 函数功能: 上电初始化或者清零日月抄表日冻结数据相关的信息：指针及每条的时间
** 入口参数: ucMeterNo:电表序号 type:日、月、抄表日冻结 id：=0xa5是数据总清时调用 否则就是上电初始化调用
** 返 回 值: 无
** 说    明: 
**           
******************************************************************************/
void init_daymonth_data_info (unsigned char ucMeterNo, unsigned char type, unsigned char id)
{	
 	unsigned char buf[8],maxnum=0,tail=0;
 	unsigned char flag=0;
 	unsigned long ucAddr=0,offset=0;
 	STDAYLISTINDEX *pstDayDataIndex=NULL;
 	STMONTHLISTINDEX *pstMonDataIndex=NULL;
 	STDAYFROZENLISTINDEX *pstFroDataIndex=NULL;
 	
    if(ucMeterNo >= CON_METER_NUM)
    {
        return;
    }    
    ucAddr = ucMeterNo * CON_DAYMONTH_DATA_LEN_MP + CON_DAY_MONTH_INFO_BASE_ADDR;
    switch(type)
    {
        case CON_DAYDATA_TYPE://日冻结
            pstDayDataIndex=&(stHisRunDataIndex[ucMeterNo].stDayDataIndex);
            DataProc_Adjust_Day_Index(pstDayDataIndex);//清零
            maxnum = CON_DAYDATA_BLOCKNUM_PER_MP*(MCP_FLASH_BLOCK_SIZE/sizeof(STHISRUNDATA));
            file_read(ucAddr+CON_DAYDATA_INDEX_ADDR, buf, CON_DAYMONTH_INDEX_LEN); 
            if((buf[CON_DAYMONTH_INDEX_LEN-1] == addverify(buf, (CON_DAYMONTH_INDEX_LEN-1)))
                && (id != 0xa5))
            {    
                if((buf[0] < maxnum) && (buf[1] < maxnum))
                {
                    flag = 1;//上次数据有效
                    pstDayDataIndex->ucHead = buf[0];
                    pstDayDataIndex->ucTail = buf[1];
                    tail = buf[1];
                    while(pstDayDataIndex->ucHead != tail)
                    {
                        offset = tail*CON_DAYMONTH_TIME_LEN + CON_DAYDATA_TIME_ADDR;
                        file_read(ucAddr+offset, buf, CON_DAYMONTH_TIME_LEN); 
                        if(buf[CON_DAYMONTH_TIME_LEN-1] == addverify(buf, (CON_DAYMONTH_TIME_LEN-1)))
                        {
                            memcpy(&pstDayDataIndex->stDateTimeList[tail], buf, (CON_DAYMONTH_TIME_LEN-1));
                        } 
                        tail++;
                        if(tail>=maxnum)
                        {
                            tail = 0;
                        }       
                    }
                }    
            }
            if(flag == 0)//需要重新初始化eeprom的相关信息, 也有可能是总清
            {
                memset(buf, 0, (CON_DAYMONTH_INDEX_LEN-1));
                buf[CON_DAYMONTH_INDEX_LEN-1] = addverify(buf, (CON_DAYMONTH_INDEX_LEN-1));
                file_write(ucAddr+CON_DAYDATA_INDEX_ADDR, buf, CON_DAYMONTH_INDEX_LEN); 
                //清除flash中首块记录
                ucAddr = CON_DFPAGE_HEAD_HISDATA + ucMeterNo*(CON_DAYDATA_BLOCKNUM_PER_MP+CON_MONTHDATA_BLOCKNUM_PER_MP+CON_FROZEDATA_BLOCKNUM_PER_MP);
                ucAddr += 0;//日数据、月数据、结算日数据偏移
                ucAddr *= MCP_FLASH_BLOCK_SIZE;   
                MCP_Flash_Sector_Erase(ucAddr);                
            }    
            break;
        case CON_MONTHDATA_TYPE://月冻结
            pstMonDataIndex=&(stHisRunDataIndex[ucMeterNo].stMonthDataIndex);
            DataProc_Adjust_Month_Index(pstMonDataIndex);//清零
            maxnum = CON_MONTHDATA_BLOCKNUM_PER_MP*(MCP_FLASH_BLOCK_SIZE/sizeof(STHISRUNDATA));
            file_read(ucAddr+CON_MONTHDATA_INDEX_ADDR, buf, CON_DAYMONTH_INDEX_LEN); 
            if((buf[CON_DAYMONTH_INDEX_LEN-1] == addverify(buf, (CON_DAYMONTH_INDEX_LEN-1)))
                && (id != 0xa5))
            {    
                if((buf[0] < maxnum) && (buf[1] < maxnum))
                {
                    flag = 1;//上次数据有效
                    pstMonDataIndex->ucHead = buf[0];
                    pstMonDataIndex->ucTail = buf[1]; 
                    tail = buf[1]; 
                    while(pstMonDataIndex->ucHead != tail)
                    {
                        offset = tail*CON_DAYMONTH_TIME_LEN + CON_MONTHDATA_TIME_ADDR;
                        file_read(ucAddr+offset, buf, CON_DAYMONTH_TIME_LEN); 
                        if(buf[CON_DAYMONTH_TIME_LEN-1] == addverify(buf, (CON_DAYMONTH_TIME_LEN-1)))
                        {
                            memcpy(&pstMonDataIndex->stDateTimeList[tail], buf, (CON_DAYMONTH_TIME_LEN-1));
                        } 
                        tail++;
                        if(tail>=maxnum)
                        {
                            tail = 0;
                        }       
                    }
                }    
            }
            if(flag == 0)//需要重新初始化eeprom的相关信息, 也有可能是总清
            {
                memset(buf, 0, (CON_DAYMONTH_INDEX_LEN-1));
                buf[CON_DAYMONTH_INDEX_LEN-1] = addverify(buf, (CON_DAYMONTH_INDEX_LEN-1));
                file_write(ucAddr+CON_MONTHDATA_INDEX_ADDR, buf, CON_DAYMONTH_INDEX_LEN); 
                //清除flash中首块记录
                ucAddr = CON_DFPAGE_HEAD_HISDATA + ucMeterNo*(CON_DAYDATA_BLOCKNUM_PER_MP+CON_MONTHDATA_BLOCKNUM_PER_MP+CON_FROZEDATA_BLOCKNUM_PER_MP);
                ucAddr += CON_DAYDATA_BLOCKNUM_PER_MP;//日数据、月数据、结算日数据偏移
                ucAddr *= MCP_FLASH_BLOCK_SIZE;   
                MCP_Flash_Sector_Erase(ucAddr);                
            }    
            break;        
        case CON_FROZEDATA_TYPE://抄表日冻结
            pstFroDataIndex=&(stHisRunDataIndex[ucMeterNo].stDayFrozenDataIndex);
            DataProc_Adjust_Frosen_Index(pstFroDataIndex);//清零
            maxnum = CON_FROZEDATA_BLOCKNUM_PER_MP*(MCP_FLASH_BLOCK_SIZE/sizeof(STHISRUNDATA));
            file_read(ucAddr+CON_DAYFROZENDATA_INDEX_ADDR, buf, CON_DAYMONTH_INDEX_LEN); 
            if((buf[CON_DAYMONTH_INDEX_LEN-1] == addverify(buf, (CON_DAYMONTH_INDEX_LEN-1)))
                && (id != 0xa5))
            {    
                if((buf[0] < maxnum) && (buf[1] < maxnum))
                {
                    flag = 1;//上次数据有效
                    pstFroDataIndex->ucHead = buf[0];
                    pstFroDataIndex->ucTail = buf[1];  
                    tail = buf[1];
                    while(pstFroDataIndex->ucHead != tail)
                    {
                        offset = tail*CON_DAYMONTH_TIME_LEN + CON_DAYFROZENDATA_TIME_ADDR;
                        file_read(ucAddr+offset, buf, CON_DAYMONTH_TIME_LEN); 
                        if(buf[CON_DAYMONTH_TIME_LEN-1] == addverify(buf, (CON_DAYMONTH_TIME_LEN-1)))
                        {
                            memcpy(&pstFroDataIndex->stDateTimeList[tail], buf, (CON_DAYMONTH_TIME_LEN-1));
                        } 
                        tail++;
                        if(tail>=maxnum)
                        {
                            tail = 0;
                        }       
                    }
                }    
            }
            if(flag == 0)//需要重新初始化eeprom的相关信息, 也有可能是总清
            {
                memset(buf, 0, (CON_DAYMONTH_INDEX_LEN-1));
                buf[CON_DAYMONTH_INDEX_LEN-1] = addverify(buf, (CON_DAYMONTH_INDEX_LEN-1));
                file_write(ucAddr+CON_DAYFROZENDATA_INDEX_ADDR, buf, CON_DAYMONTH_INDEX_LEN); 
                //清除flash中首块记录
                ucAddr = CON_DFPAGE_HEAD_HISDATA + ucMeterNo*(CON_DAYDATA_BLOCKNUM_PER_MP+CON_MONTHDATA_BLOCKNUM_PER_MP+CON_FROZEDATA_BLOCKNUM_PER_MP);
                ucAddr += CON_DAYDATA_BLOCKNUM_PER_MP + CON_MONTHDATA_BLOCKNUM_PER_MP;//日数据、月数据、结算日数据偏移
                ucAddr *= MCP_FLASH_BLOCK_SIZE;   
                MCP_Flash_Sector_Erase(ucAddr);                
            }    
            break;  
        default:
            return;          
    }    
}
/******************************************************************************
** 函数名称: void init_daymonth_lasttime(unsigned char type, unsigned char id)
** 函数功能: 上电初始化或者清零日月抄表日冻结上次执行时间
** 入口参数: type:日、月、抄表日冻结时间 
** 返 回 值: 无
** 说    明: 
**           
******************************************************************************/   
void init_daymonth_lasttime (unsigned char type, unsigned char id)
{	
    unsigned char buf[8];
    
    switch(type)
    {
        case CON_DAYDATA_TYPE://日冻结
            ClearDateTime(&g_stTime_DayData);
            if(id==0xa5)
            {
                ClearDateTime(&g_stTime_DayData);
                memcpy(buf, &g_stTime_DayData, (CON_DAYMONTH_TIME_LEN-1));
                buf[CON_DAYMONTH_TIME_LEN-1] = addverify(buf, (CON_DAYMONTH_TIME_LEN-1));
                file_write(CON_DAY_LAST_TIME_ADDR, buf, CON_DAYMONTH_TIME_LEN); 
            }
            else
            {
                file_read(CON_DAY_LAST_TIME_ADDR, buf, CON_DAYMONTH_TIME_LEN);  
                if(buf[CON_DAYMONTH_TIME_LEN-1] == addverify(buf, (CON_DAYMONTH_TIME_LEN-1)))
                {
                    memcpy(&g_stTime_DayData, buf, (CON_DAYMONTH_TIME_LEN-1));
                }
                else
                {
                   ClearDateTime(&g_stTime_DayData); 
                }        
            }        
            
            break;
         case CON_MONTHDATA_TYPE://月冻结
            ClearDateTime(&g_stTime_MonthData);
            if(id==0xa5)
            {
                ClearDateTime(&g_stTime_MonthData);
                memcpy(buf, &g_stTime_MonthData, (CON_DAYMONTH_TIME_LEN-1));
                buf[CON_DAYMONTH_TIME_LEN-1] = addverify(buf, (CON_DAYMONTH_TIME_LEN-1));
                file_write(CON_MONTH_LAST_TIME_ADDR, buf, CON_DAYMONTH_TIME_LEN); 
            }
            else
            {
                file_read(CON_MONTH_LAST_TIME_ADDR, buf, CON_DAYMONTH_TIME_LEN);  
                if(buf[CON_DAYMONTH_TIME_LEN-1] == addverify(buf, (CON_DAYMONTH_TIME_LEN-1)))
                {
                    memcpy(&g_stTime_MonthData, buf, (CON_DAYMONTH_TIME_LEN-1));
                }
                else
                {
                   ClearDateTime(&g_stTime_MonthData); 
                }        
            }        
            break;       
        case CON_FROZEDATA_TYPE://抄表日冻结
            ClearDateTime(&g_stTime_DayFrozenData);
            if(id==0xa5)
            {
                ClearDateTime(&g_stTime_DayFrozenData);
                memcpy(buf, &g_stTime_DayFrozenData, (CON_DAYMONTH_TIME_LEN-1));
                buf[CON_DAYMONTH_TIME_LEN-1] = addverify(buf, (CON_DAYMONTH_TIME_LEN-1));
                file_write(CON_FROZE_LAST_TIME_ADDR, buf, CON_DAYMONTH_TIME_LEN); 
            }
            else
            {
                file_read(CON_FROZE_LAST_TIME_ADDR, buf, CON_DAYMONTH_TIME_LEN);  
                if(buf[CON_DAYMONTH_TIME_LEN-1] == addverify(buf, (CON_DAYMONTH_TIME_LEN-1)))
                {
                    memcpy(&g_stTime_DayFrozenData, buf, (CON_DAYMONTH_TIME_LEN-1));
                }
                else
                {
                   ClearDateTime(&g_stTime_DayFrozenData); 
                }        
            }        
            break;
        case CON_FROZE_METERCOPY_TYPE://抄表日抄表时间
            ClearDateTime(&g_stTime_DayFrozenCopy);
            if(id==0xa5)
            {
                ClearDateTime(&g_stTime_DayFrozenCopy);
                memcpy(buf, &g_stTime_DayFrozenCopy, (CON_DAYMONTH_TIME_LEN-1));
                buf[CON_DAYMONTH_TIME_LEN-1] = addverify(buf, (CON_DAYMONTH_TIME_LEN-1));
                file_write(CON_FROZE_COPY_LAST_TIME_ADDR, buf, CON_DAYMONTH_TIME_LEN); 
            }
            else
            {
                file_read(CON_FROZE_COPY_LAST_TIME_ADDR, buf, CON_DAYMONTH_TIME_LEN);  
                if(buf[CON_DAYMONTH_TIME_LEN-1] == addverify(buf, (CON_DAYMONTH_TIME_LEN-1)))
                {
                    memcpy(&g_stTime_DayFrozenCopy, buf, (CON_DAYMONTH_TIME_LEN-1));
                }
                else
                {
                   ClearDateTime(&g_stTime_DayFrozenCopy); 
                }        
            }        
            break;        
    }
}   
/******************************************************************************
** 函数名称: void save_daymonth_lasttime(unsigned char type)
** 函数功能: 保存日月抄表日冻结上次执行时间
** 入口参数: type:日、月、抄表日冻结时间 
** 返 回 值: 无
** 说    明: 
**           
******************************************************************************/
void save_daymonth_lasttime (unsigned char type)
{	
    unsigned char buf[8];
    
    memset(buf, 0, 8);
    switch(type)
    {
        case CON_DAYDATA_TYPE://日冻结
            memcpy(buf, &g_stTime_DayData, (CON_DAYMONTH_TIME_LEN-1));
            buf[CON_DAYMONTH_TIME_LEN-1] = addverify(buf, (CON_DAYMONTH_TIME_LEN-1));
            file_write(CON_DAY_LAST_TIME_ADDR, buf, CON_DAYMONTH_TIME_LEN); 
            break;
         case CON_MONTHDATA_TYPE://月冻结
            memcpy(buf, &g_stTime_MonthData, (CON_DAYMONTH_TIME_LEN-1));
            buf[CON_DAYMONTH_TIME_LEN-1] = addverify(buf, (CON_DAYMONTH_TIME_LEN-1));
            file_write(CON_MONTH_LAST_TIME_ADDR, buf, CON_DAYMONTH_TIME_LEN); 
            break;       
        case CON_FROZEDATA_TYPE://抄表日冻结
            memcpy(buf, &g_stTime_DayFrozenData, (CON_DAYMONTH_TIME_LEN-1));
            buf[CON_DAYMONTH_TIME_LEN-1] = addverify(buf, (CON_DAYMONTH_TIME_LEN-1));
            file_write(CON_FROZE_LAST_TIME_ADDR, buf, CON_DAYMONTH_TIME_LEN); 
            break;
        case CON_FROZE_METERCOPY_TYPE://抄表日抄表时间
            memcpy(buf, &g_stTime_DayFrozenCopy, (CON_DAYMONTH_TIME_LEN-1));
            buf[CON_DAYMONTH_TIME_LEN-1] = addverify(buf, (CON_DAYMONTH_TIME_LEN-1));
            file_write(CON_FROZE_COPY_LAST_TIME_ADDR, buf, CON_DAYMONTH_TIME_LEN); 
            break;        
    }
}
#define CON_EE_WR_BLOCK     256
/******************************************************************************
** 函数名称: void init_day_month_data(unsigned char type)
** 函数功能: 上电初始化当前日月数据结构
** 入口参数: type:日、月、抄表日冻结时间 
** 返 回 值: 无
** 说    明: 
**           
******************************************************************************/
void init_day_month_data (unsigned char type)
{
    unsigned char mem_type,i=0;
    unsigned char *mem_ptr;
    unsigned short len=0,cnt=0,lencnt=0;

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR31);
    if(mem_ptr == NULL)
    {
        setmemgetnullid(GET_MEM_ERR31, gusCurMemErrFunName);
        return;
    }
    len = sizeof(STHISRUNDATA)+1;
    switch(type)
    {
        case CON_DAYDATA_TYPE://日冻结
            for(i=0,cnt=0; i<4; i++)
            {
                cnt = i*CON_EE_WR_BLOCK;
                if(len > cnt)
                {
                    lencnt = len - cnt;
                    if(lencnt > CON_EE_WR_BLOCK)
                    {
                        lencnt = CON_EE_WR_BLOCK;
                    }
                    file_read(CON_DAY_DATA_BASE_ADDR+cnt, mem_ptr+cnt, lencnt); 
                }
                else
                {
                    break;
                }
            }
            if(mem_ptr[len-1] == addverify(mem_ptr, (len-1)))
            {
                memcpy((unsigned char*)&stHisDayData[0], mem_ptr, (len-1));
            }
            else
            {
                DataProc_DayData_Stat_Clear();
            } 
            break;
         case CON_MONTHDATA_TYPE://月冻结
            for(i=0,cnt=0; i<4; i++)
            {
                cnt = i*CON_EE_WR_BLOCK;
                if(len > cnt)
                {
                    lencnt = len - cnt;
                    if(lencnt > CON_EE_WR_BLOCK)
                    {
                        lencnt = CON_EE_WR_BLOCK;
                    }
                    file_read(CON_MON_DATA_BASE_ADDR+cnt, mem_ptr+cnt, lencnt); 
                }
                else
                {
                    break;
                }
            }
            if(mem_ptr[len-1] == addverify(mem_ptr, (len-1)))
            {
                memcpy((unsigned char*)&stHisMonthData[0], mem_ptr, (len-1));
            }
            else
            {
                DataProc_MonthData_Stat_Clear();
            } 
            break;  
    }
    free_mem_block(mem_ptr);      
}

/******************************************************************************
** 函数名称: void powdn_save_day_month_data(unsigned char type)
** 函数功能: 下电保存当前日月数据结构
** 入口参数: type:日、月、抄表日冻结时间 
** 返 回 值: 无
** 说    明: 
**           
******************************************************************************/
void powdn_save_day_month_data (unsigned char type)
{
    unsigned char mem_type,i=0;
    unsigned char *mem_ptr;
    unsigned short len=0,cnt=0,lencnt=0;

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR31);
    if(mem_ptr == NULL)
    {
        setmemgetnullid(GET_MEM_ERR31, gusCurMemErrFunName);
        return;
    }
    len = sizeof(STHISRUNDATA);
    switch(type)
    {
        case CON_DAYDATA_TYPE://日冻结
            memcpy(mem_ptr, (unsigned char*)&stHisDayData[0], len);
            mem_ptr[len] = addverify(mem_ptr, len);
            len++;
            for(i=0,cnt=0; i<4; i++)
            {
                cnt = i*CON_EE_WR_BLOCK;
                if(len > cnt)
                {
                    lencnt = len - cnt;
                    if(lencnt > CON_EE_WR_BLOCK)
                    {
                        lencnt = CON_EE_WR_BLOCK;
                    }
                    file_write(CON_DAY_DATA_BASE_ADDR+cnt, mem_ptr+cnt, lencnt); 
                }
                else
                {
                    break;
                }
            }
            break;
         case CON_MONTHDATA_TYPE://月冻结
            memcpy(mem_ptr, (unsigned char*)&stHisMonthData[0], len);
            mem_ptr[len] = addverify(mem_ptr, len);
            len++;
            for(i=0,cnt=0; i<4; i++)
            {
                cnt = i*CON_EE_WR_BLOCK;
                if(len > cnt)
                {
                    lencnt = len - cnt;
                    if(lencnt > CON_EE_WR_BLOCK)
                    {
                        lencnt = CON_EE_WR_BLOCK;
                    }
                    file_write(CON_MON_DATA_BASE_ADDR+cnt, mem_ptr+cnt, lencnt); 
                }
                else
                {
                    break;
                }
            }
    }
    free_mem_block(mem_ptr);      
}

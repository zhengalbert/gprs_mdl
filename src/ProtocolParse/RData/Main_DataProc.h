/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_DataProc.h
 Description    : 数据处理：数据统计，数据转存
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-08-15    azh     the original version
******************************************************************************/
#ifndef _MAIN_DATAPROC_H_
#define _MAIN_DATAPROC_H_

__EXTERN unsigned char DataProc_Update_MeterData(unsigned char ucMeterNo,STDATETIME stDateTime,STHISRUNDATA *pstHisRunData,unsigned char ucReflashLastDay);
__EXTERN unsigned char DataProc_Update_MPData(unsigned char ucMPNo,STDATETIME stDateTime,STHISRUNDATA *pstHisRunData,unsigned char ucReflashLastDay);
__EXTERN unsigned char DataProc_Stat_Clear_MPData(STHISRUNDATA *pstHisRunData);

__EXTERN unsigned char DataProc_DayFrozenData_Move(STDATETIME stDateTime);
__EXTERN unsigned char DataProc_DayData_Move(STDATETIME stDateTime);
__EXTERN unsigned char DataProc_MonthData_Move(STDATETIME stDateTime);
__EXTERN unsigned char DataProc_DayData_Stat_Clear(void);
__EXTERN unsigned char DataProc_MonthData_Stat_Clear(void);
__EXTERN unsigned char DataProc_SaveData_DayFrozen(unsigned char ucMeterNo,STDATETIME stDateTime,STHISRUNDATA *pstHisRunData);
__EXTERN unsigned char DataProc_SaveData_Day(unsigned char ucMeterNo,STDATETIME stDateTime,STHISRUNDATA *pstHisRunData);
__EXTERN unsigned char DataProc_SaveData_Month(unsigned char ucMeterNo,STDATETIME stDateTime,STHISRUNDATA *pstHisRunData);

__EXTERN unsigned char DataProc_Stat_GetMax(STHISVAL stBaseVal,STHISVAL *pstRetVal,STDATETIME stDateTime);
__EXTERN unsigned char DataProc_Stat_GetMin(STHISVAL stBaseVal,STHISVAL *pstRetVal,STDATETIME stDateTime);

__EXTERN unsigned char DayData_Stat_InstantPow(unsigned char ucMeterNo,STDATETIME stDateTime);
__EXTERN unsigned char DayData_Stat_Demand(unsigned char ucMeterNo,STDATETIME stDateTime);
__EXTERN unsigned char DayData_Stat_VoltStat(unsigned char ucMeterNo,STDATETIME stDateTime);
__EXTERN unsigned char DayData_Stat_Unbalance(unsigned char ucMeterNo,STDATETIME stDateTime);
__EXTERN unsigned char DayData_Stat_CurrOver(unsigned char ucMeterNo,STDATETIME stDateTime);
__EXTERN unsigned char DayData_Stat_SPowOver(unsigned char ucMeterNo,STDATETIME stDateTime);
__EXTERN unsigned char DayData_Stat_VoltBreak(unsigned char ucMeterNo,STDATETIME stDateTime);
__EXTERN unsigned char DayData_Stat_PowRate(unsigned char ucMeterNo,STDATETIME stDateTime);
__EXTERN unsigned char DayData_Stat_TerRunState(unsigned char ucMeterNo,STDATETIME stDateTime);
__EXTERN unsigned char DayData_Stat_CtrlNum(unsigned char ucMeterNo,STDATETIME stDateTime);

__EXTERN unsigned char MonthData_Stat_InstantPow(unsigned char ucMeterNo,STDATETIME stDateTime);
__EXTERN unsigned char MonthData_Stat_Demand(unsigned char ucMeterNo,STDATETIME stDateTime);
__EXTERN unsigned char MonthData_Stat_VoltStat(unsigned char ucMeterNo,STDATETIME stDateTime);
__EXTERN unsigned char MonthData_Stat_Unbalance(unsigned char ucMeterNo,STDATETIME stDateTime);
__EXTERN unsigned char MonthData_Stat_CurrOver(unsigned char ucMeterNo,STDATETIME stDateTime);
__EXTERN unsigned char MonthData_Stat_SPowOver(unsigned char ucMeterNo,STDATETIME stDateTime);
__EXTERN unsigned char MonthData_Stat_VoltBreak(unsigned char ucMeterNo,STDATETIME stDateTime);
__EXTERN unsigned char MonthData_Stat_PowRate(unsigned char ucMeterNo,STDATETIME stDateTime);
__EXTERN unsigned char MonthData_Stat_TerRunState(unsigned char ucMeterNo,STDATETIME stDateTime);
//__EXTERN unsigned char MonthData_Stat_CtrlNum(unsigned char ucMeterNo,STDATETIME stDateTime);

__EXTERN unsigned char DataProc_Stat_Clear_InstantPow(STHISRUNDATA *pstHisRunData);
__EXTERN unsigned char DataProc_Stat_Clear_Demand(STHISRUNDATA *pstHisRunData);
__EXTERN unsigned char DataProc_Stat_Clear_VoltStat(STHISRUNDATA *pstHisRunData);
__EXTERN unsigned char DataProc_Stat_Clear_Unbalance(STHISRUNDATA *pstHisRunData);
__EXTERN unsigned char DataProc_Stat_Clear_CurrOver(STHISRUNDATA *pstHisRunData);
__EXTERN unsigned char DataProc_Stat_Clear_SPowOver(STHISRUNDATA *pstHisRunData);
__EXTERN unsigned char DataProc_Stat_Clear_VoltBreak(STHISRUNDATA *pstHisRunData);
__EXTERN unsigned char DataProc_Stat_Clear_PowRate(STHISRUNDATA *pstHisRunData);
__EXTERN unsigned char DataProc_Stat_Clear_TerRunState(STHISRUNDATA *pstHisRunData);
__EXTERN unsigned char DataProc_Stat_Clear_CtrlNum(STHISRUNDATA *pstHisRunData);

__EXTERN unsigned char DataProc_DayFrozenData_MeterCopy(STDATETIME stDateTime);
//总加
// __EXTERN unsigned char SumGDataProc_Update_SumGData(unsigned char ucSumGNo,STDATETIME stDateTime,STHISSUMGROUPRUNDATA *pstHisSumGroupRunData,unsigned char ucReflashLastDay);
// __EXTERN unsigned char DataProc_SumGDayData_Move(STDATETIME stDateTime);
// __EXTERN unsigned char DataProc_SumGMonthData_Move(STDATETIME stDateTime);
// __EXTERN unsigned char DataProc_SumGDayData_Stat_Clear(void);
// __EXTERN unsigned char DataProc_SumGMonthData_Stat_Clear(void);
// __EXTERN unsigned char DataProc_SumGSaveData_Day(unsigned char ucSumGNo,STDATETIME stDateTime,STHISSUMGROUPRUNDATA *pstHisSumGroupRunData);
// __EXTERN unsigned char DataProc_SumGSaveData_Month(unsigned char ucSumGNo,STDATETIME stDateTime,STHISSUMGROUPRUNDATA *pstHisSumGroupRunData);

// __EXTERN unsigned char SumGDataProc_Stat_Clear_InstantPow(STHISSUMGROUPRUNDATA *pstHisSumGroupRunData);
// __EXTERN unsigned char SumGDataProc_Stat_Clear_Overflow(STHISSUMGROUPRUNDATA *pstHisSumGroupRunData);
// __EXTERN unsigned char SumGDayData_Stat_InstantPow(unsigned char ucSumGNo,STDATETIME stDateTime);
// __EXTERN unsigned char SumGDayData_Stat_Overflow(unsigned char ucSumGNo,STDATETIME stDateTime);
// __EXTERN unsigned char SumGMonthData_Stat_InstantPow(unsigned char ucSumGNo,STDATETIME stDateTime);
// __EXTERN unsigned char SumGMonthData_Stat_Overflow(unsigned char ucSumGNo,STDATETIME stDateTime);

// __EXTERN unsigned char SumGDataProc_Stat_GetMax(float *pfValHis,STDATETIME *pstDataHis,float fValNow,STDATETIME stDateTime);
// __EXTERN unsigned char SumGDataProc_Stat_GetMin(float *pfValHis,STDATETIME *pstDataHis,float fValNow,STDATETIME stDateTime);

//终端
//__EXTERN unsigned char DataProc_Update_TerData(STDATETIME stDateTime,STHISRUNDATA *pstHisRunData,unsigned char ucReflashLastDay);
//__EXTERN unsigned char DataProc_TerDayData_Move(STDATETIME stDateTime);
//__EXTERN unsigned char DataProc_TerMonthData_Move(STDATETIME stDateTime);
//__EXTERN unsigned char DataProc_TerSaveData_Day(STDATETIME stDateTime,STHISRUNDATA *pstHisRunData);
//__EXTERN unsigned char DataProc_TerSaveData_Month(STDATETIME stDateTime,STHISRUNDATA *pstHisRunData);
//__EXTERN unsigned char DataProc_TerDayData_Stat_Clear(void);
//__EXTERN unsigned char DataProc_TerMonthData_Stat_Clear(void);
//__EXTERN unsigned char DayData_Stat_InstantPow_Ter(STDATETIME stDateTime);
//__EXTERN unsigned char DayData_Stat_Demand_Ter(STDATETIME stDateTime);
//__EXTERN unsigned char DayData_Stat_VoltStat_Ter(STDATETIME stDateTime);
//__EXTERN unsigned char DayData_Stat_Unbalance_Ter(STDATETIME stDateTime);
//__EXTERN unsigned char DayData_Stat_CurrOver_Ter(STDATETIME stDateTime);
//__EXTERN unsigned char DayData_Stat_SPowOver_Ter(STDATETIME stDateTime);
//__EXTERN unsigned char DayData_Stat_VoltBreak_Ter(STDATETIME stDateTime);
//__EXTERN unsigned char DayData_Stat_PowRate_Ter(STDATETIME stDateTime);
//__EXTERN unsigned char DayData_Stat_TerRunState_Ter(STDATETIME stDateTime);
//__EXTERN unsigned char DayData_Stat_CtrlNum_Ter(STDATETIME stDateTime);
//__EXTERN unsigned char MonthData_Stat_InstantPow_Ter(STDATETIME stDateTime);
//__EXTERN unsigned char MonthData_Stat_Demand_Ter(STDATETIME stDateTime);
//__EXTERN unsigned char MonthData_Stat_VoltStat_Ter(STDATETIME stDateTime);
//__EXTERN unsigned char MonthData_Stat_Unbalance_Ter(STDATETIME stDateTime);
//__EXTERN unsigned char MonthData_Stat_CurrOver_Ter(STDATETIME stDateTime);
//__EXTERN unsigned char MonthData_Stat_SPowOver_Ter(STDATETIME stDateTime);
//__EXTERN unsigned char MonthData_Stat_VoltBreak_Ter(STDATETIME stDateTime);
//__EXTERN unsigned char MonthData_Stat_PowRate_Ter(STDATETIME stDateTime);
__EXTERN unsigned char MonthData_Stat_TerRunState_Ter(STDATETIME stDateTime);
//__EXTERN unsigned char MonthData_Stat_CtrlNum_Ter(STDATETIME stDateTime);

//__EXTERN unsigned char DataProc_TerDayFrozenData_Move(STDATETIME stDateTime);
//__EXTERN unsigned char DataProc_SaveData_TerDayFrozen(STDATETIME stDateTime,STHISRUNDATA *pstHisRunData);

//脉冲
// __EXTERN unsigned char DataProc_Update_PulseData(unsigned char ucPulseNo,STDATETIME stDateTime,STHISPULSERUNDATA *pstHisRunData,unsigned char ucReflashLastDay);
// __EXTERN unsigned char DataProc_PulseDayData_Move(STDATETIME stDateTime);
// __EXTERN unsigned char DataProc_PulseMonthData_Move(STDATETIME stDateTime);
// __EXTERN unsigned char DataProc_PulseSaveData_Day(unsigned char ucPulseNo,STDATETIME stDateTime,STHISPULSERUNDATA *pstHisRunData);
// __EXTERN unsigned char DataProc_PulseSaveData_Month(unsigned char ucPulseNo,STDATETIME stDateTime,STHISPULSERUNDATA *pstHisRunData);
// __EXTERN unsigned char DataProc_PulseDayData_Stat_Clear(void);
// __EXTERN unsigned char DataProc_PulseMonthData_Stat_Clear(void);
// __EXTERN unsigned char DataProc_Pulse_Stat_Clear_InstantPow(STHISPULSERUNDATA *pstHisRunData);
// __EXTERN unsigned char DataProc_Pulse_Stat_Clear_Demand(STHISPULSERUNDATA *pstHisRunData);
// __EXTERN unsigned char DayData_Stat_InstantPow_Pulse(unsigned char ucPulseNo,STDATETIME stDateTime);
// __EXTERN unsigned char DayData_Stat_Demand_Pulse(unsigned char ucPulseNo,STDATETIME stDateTime);
// __EXTERN unsigned char MonthData_Stat_InstantPow_Pulse(unsigned char ucPulseNo,STDATETIME stDateTime);
// __EXTERN unsigned char MonthData_Stat_Demand_Pulse(unsigned char ucPulseNo,STDATETIME stDateTime);
__EXTERN void powdn_save_day_month_data(unsigned char type);

#endif

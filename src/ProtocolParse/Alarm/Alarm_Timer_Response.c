/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Alarm_Timer_Response.c
 Description    : 定时刷新数据更新到全局数组
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

static unsigned char Fill_AlarmTimer_Vol_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_Cur_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_InstantPow_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_InstantVar_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);

static unsigned char Fill_AlarmTimer_DirPower_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_UnDirPower_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_DirVar_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_UnDirVar_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_DirVar1_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_DirVar2_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_DirVar3_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_DirVar4_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_DirPowMaxDemand_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_DirPowMaxDemandTime_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_UnDirPowMaxDemand_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_UnDirPowMaxDemandTime_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_DirVarMaxDemand_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_DirVarMaxDemandTime_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_UnDirVarMaxDemand_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_UnDirVarMaxDemandTime_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);

static unsigned char Fill_AlarmTimer_DirPowerLm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_UnDirPowerLm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_DirVarLm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_UnDirVarLm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_DirVar1Lm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_DirVar2Lm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_DirVar3Lm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_DirVar4Lm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_DirPowMaxDemandLm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_DirPowMaxDemandTimeLm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_UnDirPowMaxDemandLm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_UnDirPowMaxDemandTimeLm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_DirVarMaxDemandLm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_DirVarMaxDemandTimeLm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_UnDirVarMaxDemandLm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_UnDirVarMaxDemandTimeLm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);

static unsigned char Fill_AlarmTimer_PulseConst_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_FeeRate_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_ProgramTime_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_MeterStatFlag_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_CopyDay_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_LostVolNum_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_LostVolTime_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_LastVolOffBegTime_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_LastVolOffEndTime_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_DateTime_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_Event_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);

//static unsigned char	Is_FeeRateList_Equel(STFEERATELIST *pstFeeRateList1,STFEERATELIST *pstFeeRateList2);
static unsigned char Fill_AlarmTimer_DirPowA(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_DirPowB(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_DirPowC(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_DirPowALm(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_DirPowBLm(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTimer_DirPowCLm(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid);
/*****************************************************************************
**    paramer:
**    ucMPNo: 测量点编号
**    usDILabel: 数据标识
**    ucIsValid: 有效标志
**
*****************************************************************************/

unsigned char Fill_AlarmTimer_Manage(unsigned char ucMPNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMeterNo=0;
	unsigned char ucMPType = CON_MPTYPE_NULL;

	ucMPType = RunPara_GetMPType(ucMPNo);
	if( ucMPType == CON_MPTYPE_METER)
	{
		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);

		Fill_AlarmTimer_Vol_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_Cur_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_InstantPow_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_InstantVar_Manage(ucMeterNo,usDILabel,ucIsValid);

		Fill_AlarmTimer_DirPower_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_UnDirPower_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DirVar_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_UnDirVar_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DirVar1_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DirVar2_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DirVar3_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DirVar4_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DirPowMaxDemand_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DirPowMaxDemandTime_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_UnDirPowMaxDemand_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_UnDirPowMaxDemandTime_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DirVarMaxDemand_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DirVarMaxDemandTime_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_UnDirVarMaxDemand_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_UnDirVarMaxDemandTime_Manage(ucMeterNo,usDILabel,ucIsValid);

		Fill_AlarmTimer_DirPowerLm_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_UnDirPowerLm_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DirVarLm_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_UnDirVarLm_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DirVar1Lm_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DirVar2Lm_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DirVar3Lm_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DirVar4Lm_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DirPowMaxDemandLm_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DirPowMaxDemandTimeLm_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_UnDirPowMaxDemandLm_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_UnDirPowMaxDemandTimeLm_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DirVarMaxDemandLm_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DirVarMaxDemandTimeLm_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_UnDirVarMaxDemandLm_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_UnDirVarMaxDemandTimeLm_Manage(ucMeterNo,usDILabel,ucIsValid);

		Fill_AlarmTimer_PulseConst_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_FeeRate_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_ProgramTime_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_MeterStatFlag_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_CopyDay_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_LostVolNum_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_LostVolTime_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_LastVolOffBegTime_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_LastVolOffEndTime_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DateTime_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_Event_Manage(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DirPowA(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DirPowB(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DirPowC(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DirPowALm(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DirPowBLm(ucMeterNo,usDILabel,ucIsValid);
		Fill_AlarmTimer_DirPowCLm(ucMeterNo,usDILabel,ucIsValid);
		
//		Fill_AlarmTimer_CuFe_Manage(ucMeterNo,usDILabel,ucIsValid);
}
	return ucRet;
}

//unsigned char Fill_AlarmTer_Manage(STADEMETERVAR	*pstADEMeterVal,STADEMETERENERGYVAR *pstADEMeterEnergyVal,PSTADEERRSTAT	pstADEErrStat,STADEDEMAND *pstAdeDemand)
//{
//	unsigned char ucRet = RET_SUCCESS;
//	long lTmp;
//	unsigned char ucSign;
//	STDATETIME	stTmpTime;
//
//	stTmpTime.ucYear=6;
//	stTmpTime.ucSecond=0;
//
//	Add_History_DirPower_Ter(pstADEMeterEnergyVal->stTWattHrD.ulTotal,0);//正向有功总
//	Add_History_DirPower_Ter(pstADEMeterEnergyVal->stTWattHrD.ulPlus ,1);
//	Add_History_DirPower_Ter(pstADEMeterEnergyVal->stTWattHrD.ulApex ,2);
//	Add_History_DirPower_Ter(pstADEMeterEnergyVal->stTWattHrD.ulFlat ,3);
//	Add_History_DirPower_Ter(pstADEMeterEnergyVal->stTWattHrD.ulVale ,4);
//
//
//	Add_History_UnDirPower_Ter(pstADEMeterEnergyVal->stTWattHrR.ulTotal,0);
//	Add_History_UnDirPower_Ter(pstADEMeterEnergyVal->stTWattHrR.ulPlus ,1);
//	Add_History_UnDirPower_Ter(pstADEMeterEnergyVal->stTWattHrR.ulApex ,2);
//	Add_History_UnDirPower_Ter(pstADEMeterEnergyVal->stTWattHrR.ulFlat ,3);
//	Add_History_UnDirPower_Ter(pstADEMeterEnergyVal->stTWattHrR.ulVale ,4);
//
//	Add_History_DirVar_Ter(pstADEMeterEnergyVal->stTVarHrD.ulTotal,0);
//	Add_History_DirVar_Ter(pstADEMeterEnergyVal->stTVarHrD.ulPlus ,1);
//	Add_History_DirVar_Ter(pstADEMeterEnergyVal->stTVarHrD.ulApex ,2);
//	Add_History_DirVar_Ter(pstADEMeterEnergyVal->stTVarHrD.ulFlat ,3);
//	Add_History_DirVar_Ter(pstADEMeterEnergyVal->stTVarHrD.ulVale ,4);
//
//	Add_History_UnDirVar_Ter(pstADEMeterEnergyVal->stTVarHrR.ulTotal,0);
//	Add_History_UnDirVar_Ter(pstADEMeterEnergyVal->stTVarHrR.ulPlus ,1);
//	Add_History_UnDirVar_Ter(pstADEMeterEnergyVal->stTVarHrR.ulApex ,2);
//	Add_History_UnDirVar_Ter(pstADEMeterEnergyVal->stTVarHrR.ulFlat ,3);
//	Add_History_UnDirVar_Ter(pstADEMeterEnergyVal->stTVarHrR.ulVale ,4);
//
//	Add_History_DirVar1_Ter(pstADEMeterEnergyVal->stVarHr1.ulTotal,0);
//	Add_History_DirVar2_Ter(pstADEMeterEnergyVal->stVarHr2.ulTotal,0);
//	Add_History_DirVar3_Ter(pstADEMeterEnergyVal->stVarHr3.ulTotal,0);
//	Add_History_DirVar4_Ter(pstADEMeterEnergyVal->stVarHr4.ulTotal,0);
//
//	//正向有功最大需量
//	Add_History_DirPowMaxDemand_Ter(pstAdeDemand->stWattHrDMaxDemand.stDemandValTotal.ulDemandVal,0);
//	Add_History_DirPowMaxDemand_Ter(pstAdeDemand->stWattHrDMaxDemand.stDemandValPlus.ulDemandVal,1);
//	Add_History_DirPowMaxDemand_Ter(pstAdeDemand->stWattHrDMaxDemand.stDemandValApex.ulDemandVal,2);
//	Add_History_DirPowMaxDemand_Ter(pstAdeDemand->stWattHrDMaxDemand.stDemandValFlat.ulDemandVal,3);
//	Add_History_DirPowMaxDemand_Ter(pstAdeDemand->stWattHrDMaxDemand.stDemandValVale.ulDemandVal,4);
//	//正向有功最大需量发生时间
//	stTmpTime.ucMonth =pstAdeDemand->stWattHrDMaxDemand.stDemandValTotal.ucMonth;
//	stTmpTime.ucDay   =pstAdeDemand->stWattHrDMaxDemand.stDemandValTotal.ucDay;
//	stTmpTime.ucHour  =pstAdeDemand->stWattHrDMaxDemand.stDemandValTotal.ucHour;
//	stTmpTime.ucMinute=pstAdeDemand->stWattHrDMaxDemand.stDemandValTotal.ucMinute;
//	Add_History_DirPowMaxDemandTime_Ter(stTmpTime,0);
//
//	stTmpTime.ucMonth =pstAdeDemand->stWattHrDMaxDemand.stDemandValPlus.ucMonth;
//	stTmpTime.ucDay   =pstAdeDemand->stWattHrDMaxDemand.stDemandValPlus.ucDay;
//	stTmpTime.ucHour  =pstAdeDemand->stWattHrDMaxDemand.stDemandValPlus.ucHour;
//	stTmpTime.ucMinute=pstAdeDemand->stWattHrDMaxDemand.stDemandValPlus.ucMinute;
//	Add_History_DirPowMaxDemandTime_Ter(stTmpTime,1);
//
//	stTmpTime.ucMonth =pstAdeDemand->stWattHrDMaxDemand.stDemandValApex.ucMonth;
//	stTmpTime.ucDay   =pstAdeDemand->stWattHrDMaxDemand.stDemandValApex.ucDay;
//	stTmpTime.ucHour  =pstAdeDemand->stWattHrDMaxDemand.stDemandValApex.ucHour;
//	stTmpTime.ucMinute=pstAdeDemand->stWattHrDMaxDemand.stDemandValApex.ucMinute;
//	Add_History_DirPowMaxDemandTime_Ter(stTmpTime,2);
//
//	stTmpTime.ucMonth =pstAdeDemand->stWattHrDMaxDemand.stDemandValFlat.ucMonth;
//	stTmpTime.ucDay   =pstAdeDemand->stWattHrDMaxDemand.stDemandValFlat.ucDay;
//	stTmpTime.ucHour  =pstAdeDemand->stWattHrDMaxDemand.stDemandValFlat.ucHour;
//	stTmpTime.ucMinute=pstAdeDemand->stWattHrDMaxDemand.stDemandValFlat.ucMinute;
//	Add_History_DirPowMaxDemandTime_Ter(stTmpTime,3);
//
//	stTmpTime.ucMonth =pstAdeDemand->stWattHrDMaxDemand.stDemandValVale.ucMonth;
//	stTmpTime.ucDay   =pstAdeDemand->stWattHrDMaxDemand.stDemandValVale.ucDay;
//	stTmpTime.ucHour  =pstAdeDemand->stWattHrDMaxDemand.stDemandValVale.ucHour;
//	stTmpTime.ucMinute=pstAdeDemand->stWattHrDMaxDemand.stDemandValVale.ucMinute;
//	Add_History_DirPowMaxDemandTime_Ter(stTmpTime,4);
//	//正向无功最大需量
//	Add_History_DirVarMaxDemand_Ter(pstAdeDemand->stVarHrDMaxDemand.stDemandValTotal.ulDemandVal,0);
//	Add_History_DirVarMaxDemand_Ter(pstAdeDemand->stVarHrDMaxDemand.stDemandValPlus.ulDemandVal,1);
//	Add_History_DirVarMaxDemand_Ter(pstAdeDemand->stVarHrDMaxDemand.stDemandValApex.ulDemandVal,2);
//	Add_History_DirVarMaxDemand_Ter(pstAdeDemand->stVarHrDMaxDemand.stDemandValFlat.ulDemandVal,3);
//	Add_History_DirVarMaxDemand_Ter(pstAdeDemand->stVarHrDMaxDemand.stDemandValVale.ulDemandVal,4);
//	//正向无功最大需量发生时间
//	stTmpTime.ucMonth =pstAdeDemand->stVarHrDMaxDemand.stDemandValTotal.ucMonth;
//	stTmpTime.ucDay   =pstAdeDemand->stVarHrDMaxDemand.stDemandValTotal.ucDay;
//	stTmpTime.ucHour  =pstAdeDemand->stVarHrDMaxDemand.stDemandValTotal.ucHour;
//	stTmpTime.ucMinute=pstAdeDemand->stVarHrDMaxDemand.stDemandValTotal.ucMinute;
//	Add_History_DirVarMaxDemandTime_Ter(stTmpTime,0);
//
//	stTmpTime.ucMonth =pstAdeDemand->stVarHrDMaxDemand.stDemandValPlus.ucMonth;
//	stTmpTime.ucDay   =pstAdeDemand->stVarHrDMaxDemand.stDemandValPlus.ucDay;
//	stTmpTime.ucHour  =pstAdeDemand->stVarHrDMaxDemand.stDemandValPlus.ucHour;
//	stTmpTime.ucMinute=pstAdeDemand->stVarHrDMaxDemand.stDemandValPlus.ucMinute;
//	Add_History_DirVarMaxDemandTime_Ter(stTmpTime,1);
//
//	stTmpTime.ucMonth =pstAdeDemand->stVarHrDMaxDemand.stDemandValApex.ucMonth;
//	stTmpTime.ucDay   =pstAdeDemand->stVarHrDMaxDemand.stDemandValApex.ucDay;
//	stTmpTime.ucHour  =pstAdeDemand->stVarHrDMaxDemand.stDemandValApex.ucHour;
//	stTmpTime.ucMinute=pstAdeDemand->stVarHrDMaxDemand.stDemandValApex.ucMinute;
//	Add_History_DirVarMaxDemandTime_Ter(stTmpTime,2);
//
//	stTmpTime.ucMonth =pstAdeDemand->stVarHrDMaxDemand.stDemandValFlat.ucMonth;
//	stTmpTime.ucDay   =pstAdeDemand->stVarHrDMaxDemand.stDemandValFlat.ucDay;
//	stTmpTime.ucHour  =pstAdeDemand->stVarHrDMaxDemand.stDemandValFlat.ucHour;
//	stTmpTime.ucMinute=pstAdeDemand->stVarHrDMaxDemand.stDemandValFlat.ucMinute;
//	Add_History_DirVarMaxDemandTime_Ter(stTmpTime,3);
//
//	stTmpTime.ucMonth =pstAdeDemand->stVarHrDMaxDemand.stDemandValVale.ucMonth;
//	stTmpTime.ucDay   =pstAdeDemand->stVarHrDMaxDemand.stDemandValVale.ucDay;
//	stTmpTime.ucHour  =pstAdeDemand->stVarHrDMaxDemand.stDemandValVale.ucHour;
//	stTmpTime.ucMinute=pstAdeDemand->stVarHrDMaxDemand.stDemandValVale.ucMinute;
//	Add_History_DirVarMaxDemandTime_Ter(stTmpTime,4);
//	//反向有功最大需量
//	Add_History_UnDirPowMaxDemand_Ter(pstAdeDemand->stWattHrRMaxDemand.stDemandValTotal.ulDemandVal,0);
//	Add_History_UnDirPowMaxDemand_Ter(pstAdeDemand->stWattHrRMaxDemand.stDemandValPlus.ulDemandVal,1);
//	Add_History_UnDirPowMaxDemand_Ter(pstAdeDemand->stWattHrRMaxDemand.stDemandValApex.ulDemandVal,2);
//	Add_History_UnDirPowMaxDemand_Ter(pstAdeDemand->stWattHrRMaxDemand.stDemandValFlat.ulDemandVal,3);
//	Add_History_UnDirPowMaxDemand_Ter(pstAdeDemand->stWattHrRMaxDemand.stDemandValVale.ulDemandVal,4);
//	//反向有功最大需量发生时间
//	stTmpTime.ucMonth =pstAdeDemand->stWattHrRMaxDemand.stDemandValTotal.ucMonth;
//	stTmpTime.ucDay   =pstAdeDemand->stWattHrRMaxDemand.stDemandValTotal.ucDay;
//	stTmpTime.ucHour  =pstAdeDemand->stWattHrRMaxDemand.stDemandValTotal.ucHour;
//	stTmpTime.ucMinute=pstAdeDemand->stWattHrRMaxDemand.stDemandValTotal.ucMinute;
//	Add_History_UnDirPowMaxDemandTime_Ter(stTmpTime,0);
//
//	stTmpTime.ucMonth =pstAdeDemand->stWattHrRMaxDemand.stDemandValPlus.ucMonth;
//	stTmpTime.ucDay   =pstAdeDemand->stWattHrRMaxDemand.stDemandValPlus.ucDay;
//	stTmpTime.ucHour  =pstAdeDemand->stWattHrRMaxDemand.stDemandValPlus.ucHour;
//	stTmpTime.ucMinute=pstAdeDemand->stWattHrRMaxDemand.stDemandValPlus.ucMinute;
//	Add_History_UnDirPowMaxDemandTime_Ter(stTmpTime,1);
//
//	stTmpTime.ucMonth =pstAdeDemand->stWattHrRMaxDemand.stDemandValApex.ucMonth;
//	stTmpTime.ucDay   =pstAdeDemand->stWattHrRMaxDemand.stDemandValApex.ucDay;
//	stTmpTime.ucHour  =pstAdeDemand->stWattHrRMaxDemand.stDemandValApex.ucHour;
//	stTmpTime.ucMinute=pstAdeDemand->stWattHrRMaxDemand.stDemandValApex.ucMinute;
//	Add_History_UnDirPowMaxDemandTime_Ter(stTmpTime,2);
//
//	stTmpTime.ucMonth =pstAdeDemand->stWattHrRMaxDemand.stDemandValFlat.ucMonth;
//	stTmpTime.ucDay   =pstAdeDemand->stWattHrRMaxDemand.stDemandValFlat.ucDay;
//	stTmpTime.ucHour  =pstAdeDemand->stWattHrRMaxDemand.stDemandValFlat.ucHour;
//	stTmpTime.ucMinute=pstAdeDemand->stWattHrRMaxDemand.stDemandValFlat.ucMinute;
//	Add_History_UnDirPowMaxDemandTime_Ter(stTmpTime,3);
//
//	stTmpTime.ucMonth =pstAdeDemand->stWattHrRMaxDemand.stDemandValVale.ucMonth;
//	stTmpTime.ucDay   =pstAdeDemand->stWattHrRMaxDemand.stDemandValVale.ucDay;
//	stTmpTime.ucHour  =pstAdeDemand->stWattHrRMaxDemand.stDemandValVale.ucHour;
//	stTmpTime.ucMinute=pstAdeDemand->stWattHrRMaxDemand.stDemandValVale.ucMinute;
//	Add_History_UnDirPowMaxDemandTime_Ter(stTmpTime,4);
//	//反向无功最大需量
//	Add_History_UnDirVarMaxDemand_Ter(pstAdeDemand->stVarHrRMaxDemand.stDemandValTotal.ulDemandVal,0);
//	Add_History_UnDirVarMaxDemand_Ter(pstAdeDemand->stVarHrRMaxDemand.stDemandValPlus.ulDemandVal,1);
//	Add_History_UnDirVarMaxDemand_Ter(pstAdeDemand->stVarHrRMaxDemand.stDemandValApex.ulDemandVal,2);
//	Add_History_UnDirVarMaxDemand_Ter(pstAdeDemand->stVarHrRMaxDemand.stDemandValFlat.ulDemandVal,3);
//	Add_History_UnDirVarMaxDemand_Ter(pstAdeDemand->stVarHrRMaxDemand.stDemandValVale.ulDemandVal,4);
//	//反向无功最大需量发生时间
//	stTmpTime.ucMonth =pstAdeDemand->stVarHrRMaxDemand.stDemandValTotal.ucMonth;
//	stTmpTime.ucDay   =pstAdeDemand->stVarHrRMaxDemand.stDemandValTotal.ucDay;
//	stTmpTime.ucHour  =pstAdeDemand->stVarHrRMaxDemand.stDemandValTotal.ucHour;
//	stTmpTime.ucMinute=pstAdeDemand->stVarHrRMaxDemand.stDemandValTotal.ucMinute;
//	Add_History_UnDirVarMaxDemandTime_Ter(stTmpTime,0);
//
//	stTmpTime.ucMonth =pstAdeDemand->stVarHrRMaxDemand.stDemandValPlus.ucMonth;
//	stTmpTime.ucDay   =pstAdeDemand->stVarHrRMaxDemand.stDemandValPlus.ucDay;
//	stTmpTime.ucHour  =pstAdeDemand->stVarHrRMaxDemand.stDemandValPlus.ucHour;
//	stTmpTime.ucMinute=pstAdeDemand->stVarHrRMaxDemand.stDemandValPlus.ucMinute;
//	Add_History_UnDirVarMaxDemandTime_Ter(stTmpTime,1);
//
//	stTmpTime.ucMonth =pstAdeDemand->stVarHrRMaxDemand.stDemandValApex.ucMonth;
//	stTmpTime.ucDay   =pstAdeDemand->stVarHrRMaxDemand.stDemandValApex.ucDay;
//	stTmpTime.ucHour  =pstAdeDemand->stVarHrRMaxDemand.stDemandValApex.ucHour;
//	stTmpTime.ucMinute=pstAdeDemand->stVarHrRMaxDemand.stDemandValApex.ucMinute;
//	Add_History_UnDirVarMaxDemandTime_Ter(stTmpTime,2);
//
//	stTmpTime.ucMonth =pstAdeDemand->stVarHrRMaxDemand.stDemandValFlat.ucMonth;
//	stTmpTime.ucDay   =pstAdeDemand->stVarHrRMaxDemand.stDemandValFlat.ucDay;
//	stTmpTime.ucHour  =pstAdeDemand->stVarHrRMaxDemand.stDemandValFlat.ucHour;
//	stTmpTime.ucMinute=pstAdeDemand->stVarHrRMaxDemand.stDemandValFlat.ucMinute;
//	Add_History_UnDirVarMaxDemandTime_Ter(stTmpTime,3);
//
//	stTmpTime.ucMonth =pstAdeDemand->stVarHrRMaxDemand.stDemandValVale.ucMonth;
//	stTmpTime.ucDay   =pstAdeDemand->stVarHrRMaxDemand.stDemandValVale.ucDay;
//	stTmpTime.ucHour  =pstAdeDemand->stVarHrRMaxDemand.stDemandValVale.ucHour;
//	stTmpTime.ucMinute=pstAdeDemand->stVarHrRMaxDemand.stDemandValVale.ucMinute;
//	Add_History_UnDirVarMaxDemandTime_Ter(stTmpTime,4);
//
//	Add_History_V_Ter(pstADEMeterVal->AvRms,1);
//	Add_History_V_Ter(pstADEMeterVal->BvRms,2);
//	Add_History_V_Ter(pstADEMeterVal->CvRms,3);
//
//	Add_History_V_Phase_Ter(pstADEMeterVal->Vaphase,1);
//	Add_History_V_Phase_Ter(pstADEMeterVal->Vbphase,2);
//	Add_History_V_Phase_Ter(pstADEMeterVal->Vcphase,3);
//
//	Add_History_I_Ter(pstADEMeterVal->AiRms,1);
//	Add_History_I_Ter(pstADEMeterVal->BiRms,2);
//	Add_History_I_Ter(pstADEMeterVal->CiRms,3);
//
//	Add_History_I_Phase_Ter(pstADEMeterVal->Iaphase,1);
//	Add_History_I_Phase_Ter(pstADEMeterVal->Ibphase,2);
//	Add_History_I_Phase_Ter(pstADEMeterVal->Icphase,3);
//
//	if(pstADEMeterVal->TWattD >= pstADEMeterVal->TWattR)
//	{
//		lTmp = pstADEMeterVal->TWattD - pstADEMeterVal->TWattR;
//		ucSign = 0;
//	}
//	else
//	{
//		lTmp = pstADEMeterVal->TWattR - pstADEMeterVal->TWattD;
//		ucSign = 1;
//	}
//	Add_History_InstantPow_Ter(lTmp,ucSign,0);											//正向加反向，其中一个为0
//
//	if(pstADEMeterVal->AWattD >= pstADEMeterVal->AWattR)
//	{
//		lTmp = pstADEMeterVal->AWattD - pstADEMeterVal->AWattR;
//		ucSign = 0;
//	}
//	else
//	{
//		lTmp = pstADEMeterVal->AWattR - pstADEMeterVal->AWattD;
//		ucSign = 1;
//	}
//	Add_History_InstantPow_Ter(lTmp,ucSign,1);
//
//	if(pstADEMeterVal->BWattD >= pstADEMeterVal->BWattR)
//	{
//		lTmp = pstADEMeterVal->BWattD - pstADEMeterVal->BWattR;
//		ucSign = 0;
//	}
//	else
//	{
//		lTmp = pstADEMeterVal->BWattR - pstADEMeterVal->BWattD;
//		ucSign = 1;
//	}
//	Add_History_InstantPow_Ter(lTmp,ucSign,2);
//
//	if(pstADEMeterVal->CWattD >= pstADEMeterVal->CWattR)
//	{
//		lTmp = pstADEMeterVal->CWattD - pstADEMeterVal->CWattR;
//		ucSign = 0;
//	}
//	else
//	{
//		lTmp = pstADEMeterVal->CWattR - pstADEMeterVal->CWattD;
//		ucSign = 1;
//	}
//	Add_History_InstantPow_Ter(lTmp,ucSign,3);
//
//	if(pstADEMeterVal->TVarD >= pstADEMeterVal->TVarR)
//	{
//		lTmp = pstADEMeterVal->TVarD - pstADEMeterVal->TVarR;
//		ucSign = 0;
//	}
//	else
//	{
//		lTmp = pstADEMeterVal->TVarR - pstADEMeterVal->TVarD;
//		ucSign = 1;
//	}
//	Add_History_InstantVar_Ter(lTmp,ucSign,0);
//
//	if(pstADEMeterVal->AVarD >= pstADEMeterVal->AVarR)
//	{
//		lTmp = pstADEMeterVal->AVarD - pstADEMeterVal->AVarR;
//		ucSign = 0;
//	}
//	else
//	{
//		lTmp = pstADEMeterVal->AVarR - pstADEMeterVal->AVarD;
//		ucSign = 1;
//	}
//	Add_History_InstantVar_Ter(lTmp,ucSign,1);
//
//	if(pstADEMeterVal->BVarD >= pstADEMeterVal->BVarR)
//	{
//		lTmp = pstADEMeterVal->BVarD - pstADEMeterVal->BVarR;
//		ucSign = 0;
//	}
//	else
//	{
//		lTmp = pstADEMeterVal->BVarR - pstADEMeterVal->BVarD;
//		ucSign = 1;
//	}
//	Add_History_InstantVar_Ter(lTmp,ucSign,2);
//
//	if(pstADEMeterVal->CVarD >= pstADEMeterVal->CVarR)
//	{
//		lTmp = pstADEMeterVal->CVarD - pstADEMeterVal->CVarR;
//		ucSign = 0;
//	}
//	else
//	{
//		lTmp = pstADEMeterVal->CVarR - pstADEMeterVal->CVarD;
//		ucSign = 1;
//	}
//	Add_History_InstantVar_Ter(lTmp,ucSign,3);
//
//	Add_History_PolReverseStat_Ter(pstADEErrStat->ucAPolReveStat,1);
//	Add_History_PolReverseStat_Ter(pstADEErrStat->ucBPolReveStat,2);
//	Add_History_PolReverseStat_Ter(pstADEErrStat->ucCPolReveStat,3);
//
//	Add_History_VolCirReveStat_Ter(pstADEErrStat->ucVolCirReveStat);
//	return ucRet;
//}
/************************************************************************************************
*
*    ucMeterNo:设备号
*  
*    usDILabel: 数据标识
*    ucIsValid: 有效标志
*
******************************三相电压：下发目前全国规约是小项命令/但也不可避免大项命令********/
unsigned char Fill_AlarmTimer_Vol_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPVOLA:					//A相电压
	case CON_DI_TYPE_MPVOLB:					//B相电压
	case CON_DI_TYPE_MPVOLC:					//C相电压
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_V(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_V(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPVOL_BLOCK:				//三相电压
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_V(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_V(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_V(ucMeterNo,CONINVALIDVAL,3);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

/******************************三相电流：下发目前全国规约是小项命令/但也不可避免大项命令********/
unsigned char Fill_AlarmTimer_Cur_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPCURA:					//A相电流
	case CON_DI_TYPE_MPCURB:					//B相电流
	case CON_DI_TYPE_MPCURC:					//C相电流
			if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS) 
				ucRet=Add_History_I(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
			else
				ucRet=Add_History_I(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPCUR_BLOCK:				//电流
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_I(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_I(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_I(ucMeterNo,CONINVALIDVAL,3);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}
/******************************瞬时有功：下发目前全国规约是小项命令/但也不可避免大项命令********/
unsigned char Fill_AlarmTimer_InstantPow_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPINSTANTPOW:					//瞬时有功
	case CON_DI_TYPE_MPINSTANTPOWA:					//A相瞬时有功
	case CON_DI_TYPE_MPINSTANTPOWB:					//B相瞬时有功
	case CON_DI_TYPE_MPINSTANTPOWC:					//C相瞬时有功
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_InstantPow(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_InstantPow(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPINSTANTPOW_BLOCK:			//瞬时有功数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_InstantPow(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_InstantPow(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_InstantPow(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_InstantPow(ucMeterNo,CONINVALIDVAL,3);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}
/******************************瞬时无功：下发目前全国规约是小项命令/但也不可避免大项命令********/
unsigned char Fill_AlarmTimer_InstantVar_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPINSTANTVAR:					//瞬时无功
	case CON_DI_TYPE_MPINSTANTVARA:					//A相瞬时无功
	case CON_DI_TYPE_MPINSTANTVARB:					//B相瞬时无功
	case CON_DI_TYPE_MPINSTANTVARC:					//C相瞬时无功
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_InstantVar(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_InstantVar(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPINSTANTVAR_BLOCK:			//瞬时无功数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_InstantVar(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_InstantVar(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_InstantVar(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_InstantVar(ucMeterNo,CONINVALIDVAL,3);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}
/******************************正向有功电量：下发目前全国规约是小项命令/但也不可避免大项命令********/
unsigned char Fill_AlarmTimer_DirPower_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPDIRPOWTOTAL:					//正向有功总电量
	case CON_DI_TYPE_MPDIRPOWPLUS:					//正向有功尖电量
	case CON_DI_TYPE_MPDIRPOWAPEX:					//正向有功峰电量
	case CON_DI_TYPE_MPDIRPOWFLAT:					//正向有功平电量
	case CON_DI_TYPE_MPDIRPOWVALE:					//正向有功谷电量
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_DirPower(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
		{
			if(lVal==CONINVALIDVAL)
				lVal=lVal-1;
			ucRet=Add_History_DirPower(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
			ucRet=Add_History_DirPower_LastValid(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		}
		break;
	case CON_DI_TYPE_MPDIRPOW_BLOCK:				//正向有功数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_DirPower(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_DirPower(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_DirPower(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_DirPower(ucMeterNo,CONINVALIDVAL,3);
			ucRet=Add_History_DirPower(ucMeterNo,CONINVALIDVAL,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}
/******************************反向有功电量：下发目前全国规约是小项命令/但也不可避免大项命令********/
unsigned char Fill_AlarmTimer_UnDirPower_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPUNDIRPOWTOTAL:					//反向有功总电量
	case CON_DI_TYPE_MPUNDIRPOWPLUS:					//反向有功尖电量
	case CON_DI_TYPE_MPUNDIRPOWAPEX:					//反向有功峰电量
	case CON_DI_TYPE_MPUNDIRPOWFLAT:					//反向有功平电量
	case CON_DI_TYPE_MPUNDIRPOWVALE:					//反向有功谷电量
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_UnDirPower(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
		{
			if(lVal==CONINVALIDVAL)
				lVal=lVal-1;
			ucRet=Add_History_UnDirPower(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
			ucRet=Add_History_UnDirPower_LastValid(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		}
		break;
	case CON_DI_TYPE_MPUNDIRPOW_BLOCK:				//反向有功数据块
	case CON_DI_TYPE_MPALLDI:						//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_UnDirPower(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_UnDirPower(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_UnDirPower(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_UnDirPower(ucMeterNo,CONINVALIDVAL,3);
			ucRet=Add_History_UnDirPower(ucMeterNo,CONINVALIDVAL,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

/******************************正向无功电量：下发目前全国规约是小项命令/但也不可避免大项命令********/
unsigned char Fill_AlarmTimer_DirVar_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPDIRVARTOTAL:					//正向无功总电量
	case CON_DI_TYPE_MPDIRVARPLUS:					//正向无功尖电量
	case CON_DI_TYPE_MPDIRVARAPEX:					//正向无功峰电量
	case CON_DI_TYPE_MPDIRVARFLAT:					//正向无功平电量
	case CON_DI_TYPE_MPDIRVARVALE:					//正向无功谷电量
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_DirVar(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
		{
			if(lVal==CONINVALIDVAL)
				lVal=lVal-1;
			ucRet=Add_History_DirVar(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
			ucRet=Add_History_DirVar_LastValid(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		}
		break;
	case CON_DI_TYPE_MPDIRVAR_BLOCK:				//正向无功数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_DirVar(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_DirVar(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_DirVar(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_DirVar(ucMeterNo,CONINVALIDVAL,3);
			ucRet=Add_History_DirVar(ucMeterNo,CONINVALIDVAL,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

/******************************反向无功电量：下发目前全国规约是小项命令/但也不可避免大项命令********/
unsigned char Fill_AlarmTimer_UnDirVar_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPUNDIRVARTOTAL:					//反向无功总电量
	case CON_DI_TYPE_MPUNDIRVARPLUS:					//反向无功尖电量
	case CON_DI_TYPE_MPUNDIRVARAPEX:					//反向无功峰电量
	case CON_DI_TYPE_MPUNDIRVARFLAT:					//反向无功平电量
	case CON_DI_TYPE_MPUNDIRVARVALE:					//反向无功谷电量
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_UnDirVar(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
		{
			if(lVal==CONINVALIDVAL)
				lVal=lVal-1;
			ucRet=Add_History_UnDirVar(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		}
		break;
	case CON_DI_TYPE_MPUNDIRVAR_BLOCK:					//反向无功数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_UnDirVar(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_UnDirVar(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_UnDirVar(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_UnDirVar(ucMeterNo,CONINVALIDVAL,3);
			ucRet=Add_History_UnDirVar(ucMeterNo,CONINVALIDVAL,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

/******************************1~4象限无功：下发目前全国规约是小项命令/但也不可避免大项命令********/
unsigned char Fill_AlarmTimer_DirVar1_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPVAR1:						//1象限无功总电量
	case CON_DI_TYPE_MPVAR1_FEE1:					//1象限无功尖电量
	case CON_DI_TYPE_MPVAR1_FEE2:					//1象限无功峰电量
	case CON_DI_TYPE_MPVAR1_FEE3:					//1象限无功平电量
	case CON_DI_TYPE_MPVAR1_FEE4:					//1象限无功谷电量
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_DirVar1(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_DirVar1(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPVAR1_BLOCK:					//1象限无功数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_DirVar1(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_DirVar1(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_DirVar1(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_DirVar1(ucMeterNo,CONINVALIDVAL,3);
			ucRet=Add_History_DirVar1(ucMeterNo,CONINVALIDVAL,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_DirVar2_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPVAR2:						//2象限无功总电量
	case CON_DI_TYPE_MPVAR2_FEE1:					//2象限无功尖电量
	case CON_DI_TYPE_MPVAR2_FEE2:					//2象限无功峰电量
	case CON_DI_TYPE_MPVAR2_FEE3:					//2象限无功平电量
	case CON_DI_TYPE_MPVAR2_FEE4:					//2象限无功谷电量
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_DirVar2(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_DirVar2(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPVAR2_BLOCK:					//2象限无功数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_DirVar2(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_DirVar2(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_DirVar2(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_DirVar2(ucMeterNo,CONINVALIDVAL,3);
			ucRet=Add_History_DirVar2(ucMeterNo,CONINVALIDVAL,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_DirVar3_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPVAR3:						//3象限无功总电量
	case CON_DI_TYPE_MPVAR3_FEE1:					//3象限无功尖电量
	case CON_DI_TYPE_MPVAR3_FEE2:					//3象限无功峰电量
	case CON_DI_TYPE_MPVAR3_FEE3:					//3象限无功平电量
	case CON_DI_TYPE_MPVAR3_FEE4:					//3象限无功谷电量
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_DirVar3(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_DirVar3(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPVAR3_BLOCK:					//3象限无功数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_DirVar3(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_DirVar3(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_DirVar3(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_DirVar3(ucMeterNo,CONINVALIDVAL,3);
			ucRet=Add_History_DirVar3(ucMeterNo,CONINVALIDVAL,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_DirVar4_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPVAR4:						//4象限无功总电量
	case CON_DI_TYPE_MPVAR4_FEE1:					//4象限无功尖电量
	case CON_DI_TYPE_MPVAR4_FEE2:					//4象限无功峰电量
	case CON_DI_TYPE_MPVAR4_FEE3:					//4象限无功平电量
	case CON_DI_TYPE_MPVAR4_FEE4:					//4象限无功谷电量
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_DirVar4(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_DirVar4(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPVAR4_BLOCK:					//4象限无功数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_DirVar4(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_DirVar4(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_DirVar4(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_DirVar4(ucMeterNo,CONINVALIDVAL,3);
			ucRet=Add_History_DirVar4(ucMeterNo,CONINVALIDVAL,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_DirPowMaxDemand_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPDIRPOWMAXDEMAND:							//正向有功总最大需量
	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE1:					//费率1
	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE2:					//费率2
	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE3:					//费率3
	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE4:					//费率4
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_DirPowMaxDemand(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_DirPowMaxDemand(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_BLOCK:					//正向有功最大需量数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_DirPowMaxDemand(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_DirPowMaxDemand(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_DirPowMaxDemand(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_DirPowMaxDemand(ucMeterNo,CONINVALIDVAL,3);
			ucRet=Add_History_DirPowMaxDemand(ucMeterNo,CONINVALIDVAL,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

/******************************最大需量发生时间：下发目前全国规约是小项命令/但也不可避免大项命令********/
unsigned char Fill_AlarmTimer_DirPowMaxDemandTime_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME	stDateTime,stAviDateTime;
	ClearDateTime(&stDateTime);
	
	stAviDateTime = BYTE_TO_DATETIME(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME:						//正向有功总最大需量发生时间
	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE1:					//费率1
	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE2:					//费率2
	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE3:					//费率3
	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE4:					//费率4
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_DirPowMaxDemandTime(ucMeterNo,stDateTime,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_DirPowMaxDemandTime(ucMeterNo,stAviDateTime,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_BLOCK:					//正向有功最大需量发生时间数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_DirPowMaxDemandTime(ucMeterNo,stDateTime,0);
			ucRet=Add_History_DirPowMaxDemandTime(ucMeterNo,stDateTime,1);
			ucRet=Add_History_DirPowMaxDemandTime(ucMeterNo,stDateTime,2);
			ucRet=Add_History_DirPowMaxDemandTime(ucMeterNo,stDateTime,3);
			ucRet=Add_History_DirPowMaxDemandTime(ucMeterNo,stDateTime,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_UnDirPowMaxDemand_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND:						//反向有功总最大需量
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE1:					//费率1
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE2:					//费率2
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE3:					//费率3
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE4:					//费率4
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_UnDirPowMaxDemand(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_UnDirPowMaxDemand(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_BLOCK:					//正向有功最大需量数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_UnDirPowMaxDemand(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_UnDirPowMaxDemand(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_UnDirPowMaxDemand(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_UnDirPowMaxDemand(ucMeterNo,CONINVALIDVAL,3);
			ucRet=Add_History_UnDirPowMaxDemand(ucMeterNo,CONINVALIDVAL,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_UnDirPowMaxDemandTime_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME	stDateTime,stAviDateTime;
	ClearDateTime(&stDateTime);
	
	stAviDateTime = BYTE_TO_DATETIME(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME:						//反向有功总最大需量发生时间
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE1:					//费率1
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE2:					//费率2
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE3:					//费率3
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE4:					//费率4
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_UnDirPowMaxDemandTime(ucMeterNo,stDateTime,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_UnDirPowMaxDemandTime(ucMeterNo,stAviDateTime,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_BLOCK:					//反向有功最大需量发生时间数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_UnDirPowMaxDemandTime(ucMeterNo,stDateTime,0);
			ucRet=Add_History_UnDirPowMaxDemandTime(ucMeterNo,stDateTime,1);
			ucRet=Add_History_UnDirPowMaxDemandTime(ucMeterNo,stDateTime,2);
			ucRet=Add_History_UnDirPowMaxDemandTime(ucMeterNo,stDateTime,3);
			ucRet=Add_History_UnDirPowMaxDemandTime(ucMeterNo,stDateTime,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_DirVarMaxDemand_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPDIRVARMAXDEMAND:							//正向有功总最大需量
	case CON_DI_TYPE_MPDIRVARMAXDEMAND_FEE1:					//费率1
	case CON_DI_TYPE_MPDIRVARMAXDEMAND_FEE2:					//费率2
	case CON_DI_TYPE_MPDIRVARMAXDEMAND_FEE3:					//费率3
	case CON_DI_TYPE_MPDIRVARMAXDEMAND_FEE4:					//费率4
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_DirVarMaxDemand(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_DirVarMaxDemand(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPDIRVARMAXDEMAND_BLOCK:					//正向有功最大需量数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_DirVarMaxDemand(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_DirVarMaxDemand(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_DirVarMaxDemand(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_DirVarMaxDemand(ucMeterNo,CONINVALIDVAL,3);
			ucRet=Add_History_DirVarMaxDemand(ucMeterNo,CONINVALIDVAL,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_DirVarMaxDemandTime_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME	stDateTime,stAviDateTime;
	ClearDateTime(&stDateTime);
	
	stAviDateTime = BYTE_TO_DATETIME(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPDIRVARMAXDEMANDTIME:						//正向有功总最大需量发生时间
	case CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_FEE1:					//费率1
	case CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_FEE2:					//费率2
	case CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_FEE3:					//费率3
	case CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_FEE4:					//费率4
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_DirVarMaxDemandTime(ucMeterNo,stDateTime,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_DirVarMaxDemandTime(ucMeterNo,stAviDateTime,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_BLOCK:					//正向有功最大需量发生时间数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_DirVarMaxDemandTime(ucMeterNo,stDateTime,0);
			ucRet=Add_History_DirVarMaxDemandTime(ucMeterNo,stDateTime,1);
			ucRet=Add_History_DirVarMaxDemandTime(ucMeterNo,stDateTime,2);
			ucRet=Add_History_DirVarMaxDemandTime(ucMeterNo,stDateTime,3);
			ucRet=Add_History_DirVarMaxDemandTime(ucMeterNo,stDateTime,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_UnDirVarMaxDemand_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPUNDIRVARMAXDEMAND:						//反向有功总最大需量
	case CON_DI_TYPE_MPUNDIRVARMAXDEMAND_FEE1:					//费率1
	case CON_DI_TYPE_MPUNDIRVARMAXDEMAND_FEE2:					//费率2
	case CON_DI_TYPE_MPUNDIRVARMAXDEMAND_FEE3:					//费率3
	case CON_DI_TYPE_MPUNDIRVARMAXDEMAND_FEE4:					//费率4
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_UnDirVarMaxDemand(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_UnDirVarMaxDemand(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPUNDIRVARMAXDEMAND_BLOCK:					//正向无功最大需量数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_UnDirVarMaxDemand(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_UnDirVarMaxDemand(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_UnDirVarMaxDemand(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_UnDirVarMaxDemand(ucMeterNo,CONINVALIDVAL,3);
			ucRet=Add_History_UnDirVarMaxDemand(ucMeterNo,CONINVALIDVAL,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_UnDirVarMaxDemandTime_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME	stDateTime,stAviDateTime;
	ClearDateTime(&stDateTime);
	
	stAviDateTime = BYTE_TO_DATETIME(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME:						//反向有功总最大需量发生时间
	case CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_FEE1:					//费率1
	case CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_FEE2:					//费率2
	case CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_FEE3:					//费率3
	case CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_FEE4:					//费率4
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_UnDirVarMaxDemandTime(ucMeterNo,stDateTime,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_UnDirVarMaxDemandTime(ucMeterNo,stAviDateTime,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_BLOCK:					//反向有功最大需量发生时间数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_UnDirVarMaxDemandTime(ucMeterNo,stDateTime,0);
			ucRet=Add_History_UnDirVarMaxDemandTime(ucMeterNo,stDateTime,1);
			ucRet=Add_History_UnDirVarMaxDemandTime(ucMeterNo,stDateTime,2);
			ucRet=Add_History_UnDirVarMaxDemandTime(ucMeterNo,stDateTime,3);
			ucRet=Add_History_UnDirVarMaxDemandTime(ucMeterNo,stDateTime,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

/************************************************上月数据*******************************************************/
/******************************(上月)正向有功电量：下发目前全国规约是小项命令/但也不可避免大项命令********/
unsigned char Fill_AlarmTimer_DirPowerLm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPDIRPOWTOTAL_LM:					//正向有功总电量
	case CON_DI_TYPE_MPDIRPOWPLUS_LM:					//正向有功尖电量
	case CON_DI_TYPE_MPDIRPOWAPEX_LM:					//正向有功峰电量
	case CON_DI_TYPE_MPDIRPOWFLAT_LM:					//正向有功平电量
	case CON_DI_TYPE_MPDIRPOWVALE_LM:					//正向有功谷电量
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_DirPowerLm(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_DirPowerLm(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPDIRPOW_LM_BLOCK:				//正向有功数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_DirPowerLm(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_DirPowerLm(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_DirPowerLm(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_DirPowerLm(ucMeterNo,CONINVALIDVAL,3);
			ucRet=Add_History_DirPowerLm(ucMeterNo,CONINVALIDVAL,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}
/******************************反向有功电量(上月)：下发目前全国规约是小项命令/但也不可避免大项命令********/
unsigned char Fill_AlarmTimer_UnDirPowerLm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPUNDIRPOWTOTAL_LM:				//反向有功总电量
	case CON_DI_TYPE_MPUNDIRPOWPLUS_LM:					//反向有功尖电量
	case CON_DI_TYPE_MPUNDIRPOWAPEX_LM:					//反向有功峰电量
	case CON_DI_TYPE_MPUNDIRPOWFLAT_LM:					//反向有功平电量
	case CON_DI_TYPE_MPUNDIRPOWVALE_LM:					//反向有功谷电量
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_UnDirPowerLm(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_UnDirPowerLm(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPUNDIRPOW_LM_BLOCK:				//反向有功数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_UnDirPowerLm(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_UnDirPowerLm(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_UnDirPowerLm(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_UnDirPowerLm(ucMeterNo,CONINVALIDVAL,3);
			ucRet=Add_History_UnDirPowerLm(ucMeterNo,CONINVALIDVAL,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

/******************************正向无功电量(上月)：下发目前全国规约是小项命令/但也不可避免大项命令********/
unsigned char Fill_AlarmTimer_DirVarLm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPDIRVARTOTAL_LM:					//正向无功总电量
	case CON_DI_TYPE_MPDIRVARPLUS_LM:					//正向无功尖电量
	case CON_DI_TYPE_MPDIRVARAPEX_LM:					//正向无功峰电量
	case CON_DI_TYPE_MPDIRVARFLAT_LM:					//正向无功平电量
	case CON_DI_TYPE_MPDIRVARVALE_LM:					//正向无功谷电量
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_DirVarLm(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_DirVarLm(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPDIRVAR_LM_BLOCK:					//正向无功数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_DirVarLm(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_DirVarLm(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_DirVarLm(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_DirVarLm(ucMeterNo,CONINVALIDVAL,3);
			ucRet=Add_History_DirVarLm(ucMeterNo,CONINVALIDVAL,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

/******************************反向无功电量(上月)：下发目前全国规约是小项命令/但也不可避免大项命令********/
unsigned char Fill_AlarmTimer_UnDirVarLm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPUNDIRVARTOTAL_LM:				//反向无功总电量
	case CON_DI_TYPE_MPUNDIRVARPLUS_LM:					//反向无功尖电量
	case CON_DI_TYPE_MPUNDIRVARAPEX_LM:					//反向无功峰电量
	case CON_DI_TYPE_MPUNDIRVARFLAT_LM:					//反向无功平电量
	case CON_DI_TYPE_MPUNDIRVARVALE_LM:					//反向无功谷电量
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_UnDirVarLm(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_UnDirVarLm(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPUNDIRVAR_LM_BLOCK:				//反向无功数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_UnDirVarLm(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_UnDirVarLm(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_UnDirVarLm(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_UnDirVarLm(ucMeterNo,CONINVALIDVAL,3);
			ucRet=Add_History_UnDirVarLm(ucMeterNo,CONINVALIDVAL,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

/******************************1~4象限无功(上月)：下发目前全国规约是小项命令/但也不可避免大项命令********/
unsigned char Fill_AlarmTimer_DirVar1Lm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPVAR1_LM:							//1象限无功总电量
	case CON_DI_TYPE_MPVAR1_LM_FEE1:					//1象限无功尖电量
	case CON_DI_TYPE_MPVAR1_LM_FEE2:					//1象限无功峰电量
	case CON_DI_TYPE_MPVAR1_LM_FEE3:					//1象限无功平电量
	case CON_DI_TYPE_MPVAR1_LM_FEE4:					//1象限无功谷电量
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_DirVar1Lm(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_DirVar1Lm(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPVAR1_LM_BLOCK:					//1象限无功数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_DirVar1Lm(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_DirVar1Lm(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_DirVar1Lm(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_DirVar1Lm(ucMeterNo,CONINVALIDVAL,3);
			ucRet=Add_History_DirVar1Lm(ucMeterNo,CONINVALIDVAL,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_DirVar2Lm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPVAR2_LM:						//2象限无功总电量
	case CON_DI_TYPE_MPVAR2_LM_FEE1:					//2象限无功尖电量
	case CON_DI_TYPE_MPVAR2_LM_FEE2:					//2象限无功峰电量
	case CON_DI_TYPE_MPVAR2_LM_FEE3:					//2象限无功平电量
	case CON_DI_TYPE_MPVAR2_LM_FEE4:					//2象限无功谷电量
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_DirVar2Lm(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_DirVar2Lm(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPVAR2_LM_BLOCK:					//2象限无功数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_DirVar2Lm(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_DirVar2Lm(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_DirVar2Lm(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_DirVar2Lm(ucMeterNo,CONINVALIDVAL,3);
			ucRet=Add_History_DirVar2Lm(ucMeterNo,CONINVALIDVAL,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_DirVar3Lm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPVAR3_LM:						//3象限无功总电量
	case CON_DI_TYPE_MPVAR3_LM_FEE1:					//3象限无功尖电量
	case CON_DI_TYPE_MPVAR3_LM_FEE2:					//3象限无功峰电量
	case CON_DI_TYPE_MPVAR3_LM_FEE3:					//3象限无功平电量
	case CON_DI_TYPE_MPVAR3_LM_FEE4:					//3象限无功谷电量
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
    			ucRet=Add_History_DirVar3Lm(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
    			ucRet=Add_History_DirVar3Lm(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPVAR3_LM_BLOCK:					//3象限无功数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_DirVar3Lm(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_DirVar3Lm(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_DirVar3Lm(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_DirVar3Lm(ucMeterNo,CONINVALIDVAL,3);
			ucRet=Add_History_DirVar3Lm(ucMeterNo,CONINVALIDVAL,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_DirVar4Lm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPVAR4_LM:						//4象限无功总电量
	case CON_DI_TYPE_MPVAR4_LM_FEE1:					//4象限无功尖电量
	case CON_DI_TYPE_MPVAR4_LM_FEE2:					//4象限无功峰电量
	case CON_DI_TYPE_MPVAR4_LM_FEE3:					//4象限无功平电量
	case CON_DI_TYPE_MPVAR4_LM_FEE4:					//4象限无功谷电量
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_DirVar4Lm(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_DirVar4Lm(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPVAR4_LM_BLOCK:					//4象限无功数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_DirVar4Lm(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_DirVar4Lm(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_DirVar4Lm(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_DirVar4Lm(ucMeterNo,CONINVALIDVAL,3);
			ucRet=Add_History_DirVar4Lm(ucMeterNo,CONINVALIDVAL,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_DirPowMaxDemandLm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM:							//正向有功总最大需量
	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE1:					//费率1
	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE2:					//费率2
	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE3:					//费率3
	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE4:					//费率4
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_DirPowMaxDemandLm(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_DirPowMaxDemandLm(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_BLOCK:					//正向有功最大需量数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_DirPowMaxDemandLm(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_DirPowMaxDemandLm(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_DirPowMaxDemandLm(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_DirPowMaxDemandLm(ucMeterNo,CONINVALIDVAL,3);
			ucRet=Add_History_DirPowMaxDemandLm(ucMeterNo,CONINVALIDVAL,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

/******************************最大需量发生时间(上月)：下发目前全国规约是小项命令/但也不可避免大项命令********/
unsigned char Fill_AlarmTimer_DirPowMaxDemandTimeLm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME	stDateTime,stAviDateTime;
	ClearDateTime(&stDateTime);
	
	stAviDateTime = BYTE_TO_DATETIME(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM:						//正向有功总最大需量发生时间
	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE1:					//费率1
	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE2:					//费率2
	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE3:					//费率3
	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE4:					//费率4
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_DirPowMaxDemandTimeLm(ucMeterNo,stDateTime,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_DirPowMaxDemandTimeLm(ucMeterNo,stAviDateTime,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_BLOCK:					//正向有功最大需量发生时间数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_DirPowMaxDemandTimeLm(ucMeterNo,stDateTime,0);
			ucRet=Add_History_DirPowMaxDemandTimeLm(ucMeterNo,stDateTime,1);
			ucRet=Add_History_DirPowMaxDemandTimeLm(ucMeterNo,stDateTime,2);
			ucRet=Add_History_DirPowMaxDemandTimeLm(ucMeterNo,stDateTime,3);
			ucRet=Add_History_DirPowMaxDemandTimeLm(ucMeterNo,stDateTime,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_UnDirPowMaxDemandLm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM:						//反向有功总最大需量
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE1:					//费率1
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE2:					//费率2
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE3:					//费率3
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE4:					//费率4
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_UnDirPowMaxDemandLm(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_UnDirPowMaxDemandLm(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_BLOCK:					//反向有功最大需量数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_UnDirPowMaxDemandLm(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_UnDirPowMaxDemandLm(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_UnDirPowMaxDemandLm(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_UnDirPowMaxDemandLm(ucMeterNo,CONINVALIDVAL,3);
			ucRet=Add_History_UnDirPowMaxDemandLm(ucMeterNo,CONINVALIDVAL,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_UnDirPowMaxDemandTimeLm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME	stDateTime,stAviDateTime;
	ClearDateTime(&stDateTime);
	
	stAviDateTime = BYTE_TO_DATETIME(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM:						//反向有功总最大需量发生时间
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE1:					//费率1
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE2:					//费率2
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE3:					//费率3
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE4:					//费率4
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_UnDirPowMaxDemandTimeLm(ucMeterNo,stDateTime,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_UnDirPowMaxDemandTimeLm(ucMeterNo,stAviDateTime,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_BLOCK:					//反向有功最大需量发生时间数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_UnDirPowMaxDemandTimeLm(ucMeterNo,stDateTime,0);
			ucRet=Add_History_UnDirPowMaxDemandTimeLm(ucMeterNo,stDateTime,1);
			ucRet=Add_History_UnDirPowMaxDemandTimeLm(ucMeterNo,stDateTime,2);
			ucRet=Add_History_UnDirPowMaxDemandTimeLm(ucMeterNo,stDateTime,3);
			ucRet=Add_History_UnDirPowMaxDemandTimeLm(ucMeterNo,stDateTime,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_DirVarMaxDemandLm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPDIRVARMAXDEMAND_LM:							//正向有功总最大需量
	case CON_DI_TYPE_MPDIRVARMAXDEMAND_LM_FEE1:					//费率1
	case CON_DI_TYPE_MPDIRVARMAXDEMAND_LM_FEE2:					//费率2
	case CON_DI_TYPE_MPDIRVARMAXDEMAND_LM_FEE3:					//费率3
	case CON_DI_TYPE_MPDIRVARMAXDEMAND_LM_FEE4:					//费率4
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_DirVarMaxDemandLm(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_DirVarMaxDemandLm(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPDIRVARMAXDEMAND_LM_BLOCK:					//正向有功最大需量数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_DirVarMaxDemandLm(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_DirVarMaxDemandLm(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_DirVarMaxDemandLm(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_DirVarMaxDemandLm(ucMeterNo,CONINVALIDVAL,3);
			ucRet=Add_History_DirVarMaxDemandLm(ucMeterNo,CONINVALIDVAL,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_DirVarMaxDemandTimeLm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME	stDateTime,stAviDateTime;
	ClearDateTime(&stDateTime);
	
	stAviDateTime = BYTE_TO_DATETIME(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_LM:						//正向无功总最大需量发生时间
	case CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_LM_FEE1:					//费率1
	case CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_LM_FEE2:					//费率2
	case CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_LM_FEE3:					//费率3
	case CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_LM_FEE4:					//费率4
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_DirVarMaxDemandTimeLm(ucMeterNo,stDateTime,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_DirVarMaxDemandTimeLm(ucMeterNo,stAviDateTime,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_LM_BLOCK:					//正向无功最大需量发生时间数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_DirVarMaxDemandTimeLm(ucMeterNo,stDateTime,0);
			ucRet=Add_History_DirVarMaxDemandTimeLm(ucMeterNo,stDateTime,1);
			ucRet=Add_History_DirVarMaxDemandTimeLm(ucMeterNo,stDateTime,2);
			ucRet=Add_History_DirVarMaxDemandTimeLm(ucMeterNo,stDateTime,3);
			ucRet=Add_History_DirVarMaxDemandTimeLm(ucMeterNo,stDateTime,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_UnDirVarMaxDemandLm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPUNDIRVARMAXDEMAND_LM:						//反向无功总最大需量
	case CON_DI_TYPE_MPUNDIRVARMAXDEMAND_LM_FEE1:					//费率1
	case CON_DI_TYPE_MPUNDIRVARMAXDEMAND_LM_FEE2:					//费率2
	case CON_DI_TYPE_MPUNDIRVARMAXDEMAND_LM_FEE3:					//费率3
	case CON_DI_TYPE_MPUNDIRVARMAXDEMAND_LM_FEE4:					//费率4
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_UnDirVarMaxDemandLm(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_UnDirVarMaxDemandLm(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPUNDIRVARMAXDEMAND_LM_BLOCK:					//反向无功最大需量数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_UnDirVarMaxDemandLm(ucMeterNo,CONINVALIDVAL,0);
			ucRet=Add_History_UnDirVarMaxDemandLm(ucMeterNo,CONINVALIDVAL,1);
			ucRet=Add_History_UnDirVarMaxDemandLm(ucMeterNo,CONINVALIDVAL,2);
			ucRet=Add_History_UnDirVarMaxDemandLm(ucMeterNo,CONINVALIDVAL,3);
			ucRet=Add_History_UnDirVarMaxDemandLm(ucMeterNo,CONINVALIDVAL,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_UnDirVarMaxDemandTimeLm_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME	stDateTime,stAviDateTime;
	ClearDateTime(&stDateTime);
	
	stAviDateTime = BYTE_TO_DATETIME(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_LM:						//反向有功总最大需量发生时间
	case CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_LM_FEE1:					//费率1
	case CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_LM_FEE2:					//费率2
	case CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_LM_FEE3:					//费率3
	case CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_LM_FEE4:					//费率4
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_UnDirVarMaxDemandTimeLm(ucMeterNo,stDateTime,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_UnDirVarMaxDemandTimeLm(ucMeterNo,stAviDateTime,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_LM_BLOCK:					//反向有功最大需量发生时间数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_UnDirVarMaxDemandTimeLm(ucMeterNo,stDateTime,0);
			ucRet=Add_History_UnDirVarMaxDemandTimeLm(ucMeterNo,stDateTime,1);
			ucRet=Add_History_UnDirVarMaxDemandTimeLm(ucMeterNo,stDateTime,2);
			ucRet=Add_History_UnDirVarMaxDemandTimeLm(ucMeterNo,stDateTime,3);
			ucRet=Add_History_UnDirVarMaxDemandTimeLm(ucMeterNo,stDateTime,4);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_PulseConst_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPPOWCONST:					//有功脉冲常数
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_PowPulseConst(ucMeterNo,CONINVALIDVAL);
		else
			ucRet=Add_History_PowPulseConst(ucMeterNo,lVal);
		break;
	case CON_DI_TYPE_MPVARCONST:					//无功脉冲常数
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_VarPulseConst(ucMeterNo,CONINVALIDVAL);
		else
			ucRet=Add_History_VarPulseConst(ucMeterNo,lVal);
		break;
	case CON_DI_TYPE_MPCONST_BLOCK:					//脉冲常数
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_PowPulseConst(ucMeterNo,CONINVALIDVAL);
			ucRet=Add_History_VarPulseConst(ucMeterNo,CONINVALIDVAL);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_FeeRate_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	STFEERATELIST	stFeeRateList,stAviRateList;

	ClearFeeRateList(&stFeeRateList);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPFEERATE_BLOCK:				//费率数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STFEERATELIST))==RET_SUCCESS)
			ucRet=Add_History_FeeRate(ucMeterNo,stFeeRateList);
		else
		{
			stAviRateList = BYTE_TO_FEERATELIST(stRS485RecvExpainVal.ucDataArea);
			ucRet=Add_History_FeeRate(ucMeterNo,stAviRateList);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_ProgramTime_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME	stDateTime,stAviDateTime;
	unsigned long lVal=0;

	ClearDateTime(&stDateTime);
	
	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	stAviDateTime = BYTE_TO_DATETIME(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPLATELYPROGTIME:				//编程时间
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_ProgramTime(ucMeterNo,stDateTime);
		else
			ucRet=Add_History_ProgramTime(ucMeterNo,stAviDateTime);
		break;
	case CON_DI_TYPE_MPLATELYMAXDEMANDCLEARTIME:	//需量清零时间
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_DemandClrTime(ucMeterNo,stDateTime);
		else
			ucRet=Add_History_DemandClrTime(ucMeterNo,stAviDateTime);
		break;
	case CON_DI_TYPE_MPPROGTIMETIMES:				//编程次数
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_ProgramCount(ucMeterNo,CONINVALIDVAL);
		else
			ucRet=Add_History_ProgramCount(ucMeterNo,lVal);
		break;
	case CON_DI_TYPE_MPMAXDEMANDCLEARTIMES:			//需量清零次数
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_DemandClrCount(ucMeterNo,CONINVALIDVAL);
		else
			ucRet=Add_History_DemandClrCount(ucMeterNo,lVal);
		break;
	case CON_DI_TYPE_MPBATWORKTIME:					//电池工作时间
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_BatWorkTime(ucMeterNo,CONINVALIDVAL);
		else
			ucRet=Add_History_BatWorkTime(ucMeterNo,lVal);
		break;
	case CON_DI_TYPE_MPB21F_BLOCK:					//编程时间等
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_ProgramTime(ucMeterNo,stDateTime);
			ucRet=Add_History_DemandClrTime(ucMeterNo,stDateTime);
			ucRet=Add_History_ProgramCount(ucMeterNo,CONINVALIDVAL);
			ucRet=Add_History_DemandClrCount(ucMeterNo,CONINVALIDVAL);
			ucRet=Add_History_BatWorkTime(ucMeterNo,CONINVALIDVAL);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_MeterStatFlag_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucByte=0;
	unsigned char i;
	ucByte=stRS485RecvExpainVal.ucDataArea[0];
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPMETERSTATFLAG:				//电表状态字
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,1)==RET_SUCCESS)
			ucRet=Add_History_MeterStatFlag(ucMeterNo,(unsigned char)CONINVALIDVAL,1);
		else
			ucRet=Add_History_MeterStatFlag(ucMeterNo,ucByte,1);
	//	for(i=2;i<8;i++)
	//	{
	//		ucRet=Add_History_MeterStatFlag(ucMeterNo,CONINVALIDVAL,i);
	//	}
		break;
	case CON_DI_TYPE_MPNETSTATFLAG:					//电网状态字
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,1)==RET_SUCCESS)
			ucRet=Add_History_MeterStatFlag(ucMeterNo,(unsigned char)CONINVALIDVAL,2);
		else
			ucRet=Add_History_MeterStatFlag(ucMeterNo,ucByte,2);
		break;
	case CON_DI_TYPE_MPCO2F_BLOCK:	
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,14)==RET_SUCCESS)
		{
			for(i=1;i<8;i++)
			{
				ucRet=Add_History_MeterStatFlag(ucMeterNo,(unsigned char)CONINVALIDVAL,i);
			}
		}
		else
		{
			for(i=1;i<8;i++)
			{
				ucByte = stRS485RecvExpainVal.ucDataArea[i*2-2];
				ucRet=Add_History_MeterStatFlag(ucMeterNo,ucByte,i);
			}
		}
		break;
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			for(i=1;i<8;i++)
			{
				ucRet=Add_History_MeterStatFlag(ucMeterNo,(unsigned char)CONINVALIDVAL,i);
			}
		//	ucRet=Add_History_MeterStatFlag(ucMeterNo,CONINVALIDVAL);
		//	ucRet=Add_History_NetStatFlag(ucMeterNo,CONINVALIDVAL);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_CopyDay_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME	stDateTime,stAviDateTime;
	ClearDateTime(&stDateTime);
	
	stAviDateTime = BYTE_TO_DATETIME(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPCOPYDAYTIME:				//抄表日
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_CopyDayTime(ucMeterNo,stDateTime);
		else
			ucRet=Add_History_CopyDayTime(ucMeterNo,stAviDateTime);
		break;
	case CON_DI_TYPE_MPC11F_BLOCK:				//抄表日
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
			ucRet=Add_History_CopyDayTime(ucMeterNo,stDateTime);
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_LostVolNum_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS,i;
	unsigned long lVal=0;
	STHISVAL stHistmp;
	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPLOSTVOLNUM:				//电表断相次数
	//	if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
	//		ucRet=Add_History_LostVolNum(ucMeterNo,CONINVALIDVAL,0);
	//	else
	//		ucRet=Add_History_LostVolNum(ucMeterNo,lVal,0);
		lVal = 0;
		for(i=1;i<4;i++)
		{
			stHistmp = Get_History_VolBreakNum_byMPNo(ucMeterNo, 0, i);
			if(Is_DateTime_Availble(&stHistmp.stDateTime) == RET_SUCCESS && stHistmp.lVal != 0xBB)
			{
				lVal += stHistmp.lVal;
			}
		}
		ucRet=Add_History_LostVolNum(ucMeterNo,lVal,0);
		break;
	case CON_DI_TYPE_MPLOSTVOLANUM:				//A相电表断相次数
	case CON_DI_TYPE_MPLOSTVOLBNUM:				//B相电表断相次数
	case CON_DI_TYPE_MPLOSTVOLCNUM:				//C相电表断相次数
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_LostVolNum(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_LostVolNum(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPB31F_BLOCK:				//电表断相次数
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			for(i=0;i<4;i++)
				ucRet=Add_History_LostVolNum(ucMeterNo,CONINVALIDVAL,i);
		//	ucRet=Add_History_LostVolNum(ucMeterNo,CONINVALIDVAL,0);
		//	ucRet=Add_History_LostVolNum(ucMeterNo,CONINVALIDVAL,1);
		//	ucRet=Add_History_LostVolNum(ucMeterNo,CONINVALIDVAL,2);
		//	ucRet=Add_History_LostVolNum(ucMeterNo,CONINVALIDVAL,3);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_LostVolTime_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS,i;
	unsigned long lVal=0;
	STHISVAL stHistmp;
	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPLOSTVOLTIME:					//断相累计时间
		lVal = 0;
		for(i=1;i<4;i++)
		{
			stHistmp = Get_History_VolBreakTime_byMPNo(ucMeterNo, 0, i);
			if(Is_DateTime_Availble(&stHistmp.stDateTime) == RET_SUCCESS && stHistmp.lVal != 0xBB)
			{
				lVal += stHistmp.lVal;
			}
		}
		ucRet=Add_History_LostVolTime(ucMeterNo,lVal,0);
		break;
	case CON_DI_TYPE_MPLOSTVOLATIME:				//A相断相累计时间
	case CON_DI_TYPE_MPLOSTVOLBTIME:				//B相断相累计时间
	case CON_DI_TYPE_MPLOSTVOLCTIME:				//C相断相累计时间
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_History_LostVolTime(ucMeterNo,CONINVALIDVAL,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_LostVolTime(ucMeterNo,lVal,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPB32F_BLOCK:					//断相累计时间
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			for(i=0;i<4;i++)
				ucRet=Add_History_LostVolTime(ucMeterNo,CONINVALIDVAL,i);
		//	ucRet=Add_History_LostVolTime(ucMeterNo,CONINVALIDVAL,0);
		//	ucRet=Add_History_LostVolTime(ucMeterNo,CONINVALIDVAL,1);
		//	ucRet=Add_History_LostVolTime(ucMeterNo,CONINVALIDVAL,2);
		//	ucRet=Add_History_LostVolTime(ucMeterNo,CONINVALIDVAL,3);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_LastVolOffBegTime_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME	stDateTimetmp,stAviDateTime;
	STDATETIME	stDateTime[3];
	STHISTIME	stHistime_tmp;
	unsigned char i,num=0;
	ClearDateTime(&stDateTimetmp);
	
	stAviDateTime = BYTE_TO_DATETIME(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPLASTVOLOFFBEGTIME:				//最近一次断相起始时间
	//	if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
	//	{
			for(i=1;i<4;i++)
			{
				stHistime_tmp = Get_History_LastVolBreakBegTime_byMPNo(ucMeterNo, 0, i);
				if(Is_DateTime_Availble(&stHistime_tmp.stHisTime) == RET_SUCCESS)
				{
					stDateTime[num] = stHistime_tmp.stHisTime;
					num ++ ;
				}
			}
			if(num == 0)
			{
				ucRet=Add_History_LastVolOffBegTime(ucMeterNo,stDateTimetmp,0);
			}
			else if(num == 1)
			{
				ucRet=Add_History_LastVolOffBegTime(ucMeterNo,stDateTime[0],0);
			}
			else if(num == 2)
			{
				if(TimeIsOver(&stDateTime[0],&stDateTime[1]) == RET_SUCCESS)
					ucRet=Add_History_LastVolOffBegTime(ucMeterNo,stDateTime[1],0);
				else
					ucRet=Add_History_LastVolOffBegTime(ucMeterNo,stDateTime[0],0);
			}
			else if(num == 3)
			{
				if(TimeIsOver(&stDateTime[0],&stDateTime[1]) == RET_SUCCESS)
				{
					if(TimeIsOver(&stDateTime[1],&stDateTime[2]) == RET_SUCCESS)
						ucRet=Add_History_LastVolOffBegTime(ucMeterNo,stDateTime[2],0);
					else
						ucRet=Add_History_LastVolOffBegTime(ucMeterNo,stDateTime[1],0);
				}
				else
				{
					if(TimeIsOver(&stDateTime[0],&stDateTime[2]) == RET_SUCCESS)
						ucRet=Add_History_LastVolOffBegTime(ucMeterNo,stDateTime[2],0);
					else
						ucRet=Add_History_LastVolOffBegTime(ucMeterNo,stDateTime[0],0);
				}
			}
	//	}
		break;
	case CON_DI_TYPE_MPLASTVOLAOFFBEGTIME:				//A相最近一次断相起始时间
	case CON_DI_TYPE_MPLASTVOLBOFFBEGTIME:				//B相最近一次断相起始时间
	case CON_DI_TYPE_MPLASTVOLCOFFBEGTIME:				//C相最近一次断相起始时间
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_LastVolOffBegTime(ucMeterNo,stDateTimetmp,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_LastVolOffBegTime(ucMeterNo,stAviDateTime,(unsigned char)(usDILabel&0x0F));
		break;
	case CON_DI_TYPE_MPB33F_BLOCK:				//最近一次断相起始时间
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_LastVolOffBegTime(ucMeterNo,stDateTimetmp,0);
			ucRet=Add_History_LastVolOffBegTime(ucMeterNo,stDateTimetmp,1);
			ucRet=Add_History_LastVolOffBegTime(ucMeterNo,stDateTimetmp,2);
			ucRet=Add_History_LastVolOffBegTime(ucMeterNo,stDateTimetmp,3);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_LastVolOffEndTime_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME	stDateTime_tmp,stAviDateTime;
	STDATETIME	stDateTime[3];
	STHISTIME	stHistime_tmp;
	unsigned char i,num=0;
//	ClearDateTime(&stDateTimetmp);
	ClearDateTime(&stDateTime_tmp);
	
	stAviDateTime = BYTE_TO_DATETIME(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPLASTVOLOFFENDTIME:				//最近一次断相结束时间
	//	if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
	//		ucRet=Add_History_LastVolOffEndTime(ucMeterNo,stDateTime,0);
	//	else
	//		ucRet=Add_History_LastVolOffEndTime(ucMeterNo,stAviDateTime,0);
		for(i=1;i<4;i++)
		{
			stHistime_tmp = Get_History_LastVolBreakEndTime_byMPNo(ucMeterNo, 0, i);
			if(Is_DateTime_Availble(&stHistime_tmp.stHisTime) == RET_SUCCESS)
			{
				stDateTime[num] = stHistime_tmp.stHisTime;
				num ++ ;
			}
		}
		if(num == 0)
		{
			ucRet=Add_History_LastVolOffEndTime(ucMeterNo,stDateTime_tmp,0);
		}
		else if(num == 1)
		{
			ucRet=Add_History_LastVolOffEndTime(ucMeterNo,stDateTime[0],0);
		}
		else if(num == 2)
		{
			if(TimeIsOver(&stDateTime[0],&stDateTime[1]) == RET_SUCCESS)
				ucRet=Add_History_LastVolOffEndTime(ucMeterNo,stDateTime[0],0);
			else
				ucRet=Add_History_LastVolOffEndTime(ucMeterNo,stDateTime[1],0);
		}
		else if(num == 3)
		{
			if(TimeIsOver(&stDateTime[0],&stDateTime[1]) == RET_SUCCESS)
			{
				if(TimeIsOver(&stDateTime[0],&stDateTime[2]) == RET_SUCCESS)
					ucRet=Add_History_LastVolOffEndTime(ucMeterNo,stDateTime[0],0);
				else
					ucRet=Add_History_LastVolOffEndTime(ucMeterNo,stDateTime[2],0);
			}
			else
			{
				if(TimeIsOver(&stDateTime[1],&stDateTime[2]) == RET_SUCCESS)
					ucRet=Add_History_LastVolOffEndTime(ucMeterNo,stDateTime[1],0);
				else
					ucRet=Add_History_LastVolOffEndTime(ucMeterNo,stDateTime[2],0);
			}
		}
		break;
	case CON_DI_TYPE_MPLASTVOLAOFFENDTIME:				//A相最近一次断相结束时间
	case CON_DI_TYPE_MPLASTVOLBOFFENDTIME:				//B相最近一次断相结束时间
	case CON_DI_TYPE_MPLASTVOLCOFFENDTIME:				//C相最近一次断相结束时间
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_LastVolOffEndTime(ucMeterNo,stDateTime_tmp,(unsigned char)(usDILabel&0x0F));
		else
			ucRet=Add_History_LastVolOffEndTime(ucMeterNo,stAviDateTime,(unsigned char)(usDILabel&0x0F));
		break;
//	case CON_DI_TYPE_MPLASTVOLBOFFENDTIME:				//B相最近一次断相结束时间
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
//			ucRet=Add_History_LastVolOffEndTime(ucMeterNo,stDateTime,2);
//		else
//			ucRet=Add_History_LastVolOffEndTime(ucMeterNo,stAviDateTime,2);
//		break;
//	case CON_DI_TYPE_MPLASTVOLCOFFENDTIME:				//C相最近一次断相结束时间
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
//			ucRet=Add_History_LastVolOffEndTime(ucMeterNo,stDateTime,3);
//		else
//			ucRet=Add_History_LastVolOffEndTime(ucMeterNo,stAviDateTime,3);
//		break;
	case CON_DI_TYPE_MPB34F_BLOCK:				//最近一次断相结束时间
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_LastVolOffEndTime(ucMeterNo,stDateTime_tmp,0);
			ucRet=Add_History_LastVolOffEndTime(ucMeterNo,stDateTime_tmp,1);
			ucRet=Add_History_LastVolOffEndTime(ucMeterNo,stDateTime_tmp,2);
			ucRet=Add_History_LastVolOffEndTime(ucMeterNo,stDateTime_tmp,3);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTimer_DateTime_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME	stDateTime,stAviDateTime;
	ClearDateTime(&stDateTime);
	
	stAviDateTime = BYTE_TO_DATETIME(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPDATEANDWEEK:				//年月日
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_MeterDate(ucMeterNo,stDateTime);
		else
			ucRet=Add_History_MeterDate(ucMeterNo,stAviDateTime);
		break;
	case CON_DI_TYPE_MPTIME:					//时分秒
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_MeterTime(ucMeterNo,stDateTime);
		else
			ucRet=Add_History_MeterTime(ucMeterNo,stAviDateTime);
		break;
	case CON_DI_TYPE_MPDATETIME_BLOCK:			//时钟数据块
	case CON_DI_TYPE_MPALLDI:					//
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_History_MeterDate(ucMeterNo,stDateTime);
			ucRet=Add_History_MeterTime(ucMeterNo,stDateTime);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}


unsigned char Fill_AlarmTimer_Event_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME	stDateTime,stAviDateTime;
	unsigned long lVal=0;	
	ClearDateTime(&stDateTime);
	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	stAviDateTime = BYTE_TO_DATETIME(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPPROGRAMNUM:				//编程次数
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_ProgramCount(ucMeterNo,CONINVALIDVAL);
		else
			ucRet=Add_History_ProgramCount(ucMeterNo,lVal);
		break;
	case CON_DI_TYPE_MPMETERCLRNUM:				//电表清0次数
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_MeterClrCount(ucMeterNo,CONINVALIDVAL);
		else
			ucRet=Add_History_MeterClrCount(ucMeterNo,lVal);
		break;
	case CON_DI_TYPE_MPDEMODECLRNUM:			//需量清0次数
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_DemandClrCount(ucMeterNo,CONINVALIDVAL);
		else
			ucRet=Add_History_DemandClrCount(ucMeterNo,lVal);
		break;
	case CON_DI_TYPE_MPEVENTCLRNUM:				//事件清0次数
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_EventClrCount(ucMeterNo,CONINVALIDVAL);
		else
			ucRet=Add_History_EventClrCount(ucMeterNo,lVal);
		break;
	case CON_DI_TYPE_MPJIAOSHINUM:				//校时次数
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_JiaoshiCount(ucMeterNo,CONINVALIDVAL);
		else
			ucRet=Add_History_JiaoshiCount(ucMeterNo,lVal);
		break;
	case CON_DI_TYPE_MPPROGRAMTIME:				//编程时间
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_ProgramTime(ucMeterNo,stDateTime);
		else
			ucRet=Add_History_ProgramTime(ucMeterNo,stAviDateTime);
		break;
	case CON_DI_TYPE_MPMETERCLRTIME:				//电表清0时间
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_MeterClrTime(ucMeterNo,stDateTime);
		else
			ucRet=Add_History_MeterClrTime(ucMeterNo,stAviDateTime);
		break;
	case CON_DI_TYPE_MPDEMODECLRTIME:			//需量清0时间
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_DemandClrTime(ucMeterNo,stDateTime);
		else
			ucRet=Add_History_DemandClrTime(ucMeterNo,stAviDateTime);
		break;
	case CON_DI_TYPE_MPEVENTCLRTIME:				//事件清0时间
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_EventClrTime(ucMeterNo,stDateTime);
		else
			ucRet=Add_History_EventClrTime(ucMeterNo,stAviDateTime);
		break;
	case CON_DI_TYPE_MPJIAOSHITIME:				//校时时间
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
			ucRet=Add_History_JiaoshiTime(ucMeterNo,stDateTime);
		else
			ucRet=Add_History_JiaoshiTime(ucMeterNo,stAviDateTime);
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}
unsigned char Fill_AlarmTimer_DirPowA(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
		case CON_DI_TYPE_MPDIRPOWA:
		case CON_DI_TYPE_MPUNDIRPOWA:
		case CON_DI_TYPE_MPCOMVAR1A:
		case CON_DI_TYPE_MPCOMVAR2A:
			if(ucIsValid == CONALRTME_INVALID_FLAG ||Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
				ucRet = Add_History_DirPowA(ucMeterNo, CONINVALIDVAL, (unsigned char)(usDILabel&0x0F));
			else 
				ucRet = Add_History_DirPowA(ucMeterNo, lVal, (unsigned char)(usDILabel&0x0F));
		default:
			ucRet=RET_ERROR;
			break;
	}
	return ucRet;
}
unsigned char Fill_AlarmTimer_DirPowB(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
		case CON_DI_TYPE_MPDIRPOWB:
		case CON_DI_TYPE_MPUNDIRPOWB:
		case CON_DI_TYPE_MPCOMVAR1B:
		case CON_DI_TYPE_MPCOMVAR2B:
			if(ucIsValid == CONALRTME_INVALID_FLAG ||Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
				ucRet = Add_History_DirPowB(ucMeterNo, CONINVALIDVAL, (unsigned char)(usDILabel-CON_DI_TYPE_MPDIRPOWB));
			else 
				ucRet = Add_History_DirPowB(ucMeterNo, lVal, (unsigned char)(usDILabel-CON_DI_TYPE_MPDIRPOWB));
		default:
			ucRet=RET_ERROR;
			break;
	}
	return ucRet;
}
unsigned char Fill_AlarmTimer_DirPowC(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
		case CON_DI_TYPE_MPDIRPOWC:
		case CON_DI_TYPE_MPUNDIRPOWC:
		case CON_DI_TYPE_MPCOMVAR1C:
		case CON_DI_TYPE_MPCOMVAR2C:
			if(ucIsValid == CONALRTME_INVALID_FLAG ||Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
				ucRet = Add_History_DirPowC(ucMeterNo, CONINVALIDVAL, (unsigned char)(usDILabel-CON_DI_TYPE_MPDIRPOWC));
			else 
				ucRet = Add_History_DirPowC(ucMeterNo, lVal, (unsigned char)(usDILabel-CON_DI_TYPE_MPDIRPOWC));
		default:
			ucRet=RET_ERROR;
			break;
	}
	return ucRet;
}
unsigned char Fill_AlarmTimer_DirPowALm(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
		case CON_DI_TYPE_MPDIRPOWA_LM:
		case CON_DI_TYPE_MPUNDIRPOWA_LM:
		case CON_DI_TYPE_MPCOMVAR1A_LM:
		case CON_DI_TYPE_MPCOMVAR2A_LM:
			if(ucIsValid == CONALRTME_INVALID_FLAG ||Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
				ucRet = Add_History_DirPowALm(ucMeterNo, CONINVALIDVAL, (unsigned char)(usDILabel&0x0F));
			else 
				ucRet = Add_History_DirPowALm(ucMeterNo, lVal, (unsigned char)(usDILabel&0x0F));
		default:
			ucRet=RET_ERROR;
			break;
	}
	return ucRet;
}
unsigned char Fill_AlarmTimer_DirPowBLm(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
		case CON_DI_TYPE_MPDIRPOWB_LM:
		case CON_DI_TYPE_MPUNDIRPOWB_LM:
		case CON_DI_TYPE_MPCOMVAR1B_LM:
		case CON_DI_TYPE_MPCOMVAR2B_LM:
			if(ucIsValid == CONALRTME_INVALID_FLAG ||Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
				ucRet = Add_History_DirPowBLm(ucMeterNo, CONINVALIDVAL, (unsigned char)(usDILabel-CON_DI_TYPE_MPDIRPOWB_LM));
			else 
				ucRet = Add_History_DirPowBLm(ucMeterNo, lVal, (unsigned char)(usDILabel-CON_DI_TYPE_MPDIRPOWB_LM));
		default:
			ucRet=RET_ERROR;
			break;
	}
	return ucRet;
}
unsigned char Fill_AlarmTimer_DirPowCLm(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
		case CON_DI_TYPE_MPDIRPOWC_LM:
		case CON_DI_TYPE_MPUNDIRPOWC_LM:
		case CON_DI_TYPE_MPCOMVAR1C_LM:
		case CON_DI_TYPE_MPCOMVAR2C_LM:
			if(ucIsValid == CONALRTME_INVALID_FLAG ||Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
				ucRet = Add_History_DirPowCLm(ucMeterNo, CONINVALIDVAL, (unsigned char)(usDILabel-CON_DI_TYPE_MPDIRPOWC_LM));
			else 
				ucRet = Add_History_DirPowCLm(ucMeterNo, lVal, (unsigned char)(usDILabel-CON_DI_TYPE_MPDIRPOWC_LM));
		default:
			ucRet=RET_ERROR;
			break;
	}
	return ucRet;
}

//unsigned char Fill_AlarmTimer_CuFe_Manage(unsigned char ucMeterNo,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned long lVal=0;
//
//	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
//	switch(usDILabel)
//	{
//		case CON_DI_TYPE_MPCUFE_GA:
//		case CON_DI_TYPE_MPCUFE_BA:
//		case CON_DI_TYPE_MPCUFE_RA:
//		case CON_DI_TYPE_MPCUFE_XA:
//		case CON_DI_TYPE_MPCUFE_GB:
//		case CON_DI_TYPE_MPCUFE_BB:
//		case CON_DI_TYPE_MPCUFE_RB:
//		case CON_DI_TYPE_MPCUFE_XB:
//		case CON_DI_TYPE_MPCUFE_GC:
//		case CON_DI_TYPE_MPCUFE_BC:
//		case CON_DI_TYPE_MPCUFE_RC:
//		case CON_DI_TYPE_MPCUFE_XC:
//			if(ucIsValid==CONALRTME_VALID_FLAG && Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))!=RET_SUCCESS)
//			Add_Para_CuFe(ucMeterNo,lVal);
//			break;
//		default:break;
//		
//	}
//}

STFEERATELIST	BYTE_TO_FEERATELIST(unsigned char *pucByte)
{
	unsigned char	i=0,m=0;
	STFEERATELIST	stFeeRateList;

	ClearFeeRateList(&stFeeRateList);

	stFeeRateList.ucFeeRateNum = pucByte[m++];
	if(stFeeRateList.ucFeeRateNum > 8)
		stFeeRateList.ucFeeRateNum = 8;
	for(i=0;i<stFeeRateList.ucFeeRateNum ;i++)
	{
		stFeeRateList.stFeeRate[i].ucFeeRate[0]	= pucByte[m++];
		stFeeRateList.stFeeRate[i].ucFeeRate[1]	= pucByte[m++];
		stFeeRateList.stFeeRate[i].ucFeeRate[2]	= pucByte[m++];
	}

	return stFeeRateList;
}

unsigned char Is_FeeRateList_Equel(STFEERATELIST *pstFeeRateList0,STFEERATELIST *pstFeeRateList1)
{
	unsigned char ucRet=RET_ERROR,i=0;
	if(pstFeeRateList0->ucFeeRateNum==pstFeeRateList1->ucFeeRateNum)
	{
		if(pstFeeRateList0->ucFeeRateNum <= 8)
		{
			for(i=0;i<pstFeeRateList0->ucFeeRateNum;i++)
			{
				if(	pstFeeRateList0->stFeeRate[i].ucFeeRate[0] != pstFeeRateList1->stFeeRate[i].ucFeeRate[0] ||
					pstFeeRateList0->stFeeRate[i].ucFeeRate[1] != pstFeeRateList1->stFeeRate[i].ucFeeRate[1] ||
					pstFeeRateList0->stFeeRate[i].ucFeeRate[2] != pstFeeRateList1->stFeeRate[i].ucFeeRate[2] )
					break;
			}
			if(i>=pstFeeRateList0->ucFeeRateNum)
				ucRet=RET_SUCCESS;
		}else
			ucRet = RET_SUCCESS;		//原应=RET_ERROR，但为避免费率更改的误报警，设为RET_SUCCESS
	}
	return ucRet;
}

/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Alarm_Data_Manage.c
 Description    : 填写告警相关附带数据项
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2013-01-08    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

static void Alarm_SetCurr_ForeJudge(unsigned char ucMPNo,unsigned char ucNum,unsigned char ucCount,unsigned char ucListNo,STDATAITEM_ALR *pstDataItem_alr);
//static unsigned char Add_AlarmForeJudge_stFeeRateValue(unsigned char ucMPNo,unsigned char ucNum,STFEERATELIST stFeeRateList,unsigned short usItemLablePara);
//static unsigned char Add_AlarmForeJudge_ucValue(unsigned char ucMPNo,unsigned char ucNum,unsigned char ucState,unsigned short usItemLablePara);
//static unsigned char Add_AlarmEnsureJudge_stFeeRateValue(unsigned char ucMPNo,unsigned char ucNum,STFEERATELIST stFeeRateList,unsigned short usItemLablePara);
//static unsigned char Add_AlarmEnsureJudge_ucValue(unsigned char ucMPNo,unsigned char ucNum,unsigned char ucStatFlag,unsigned short usItemLablePara);

/*
//正向有功电量（上月）
unsigned char Add_AlarmForeJudge_DirPowerLmTotal(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRPOWTOTAL_LM);
}
//正向有功电量
unsigned char Add_AlarmForeJudge_DirPowerTotal(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRPOWTOTAL);
}
unsigned char Add_AlarmForeJudge_DirPowerPlus(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRPOWPLUS);
}
unsigned char Add_AlarmForeJudge_DirPowerApex(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRPOWAPEX);
}
unsigned char Add_AlarmForeJudge_DirPowerFlat(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRPOWFLAT);
}
unsigned char Add_AlarmForeJudge_DirPowerVale(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRPOWVALE);
}
//反向有功电量
unsigned char Add_AlarmForeJudge_UnDirPowerTotal(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRPOWTOTAL);
}
unsigned char Add_AlarmForeJudge_UnDirPowerPlus(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRPOWPLUS);
}
unsigned char Add_AlarmForeJudge_UnDirPowerApex(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRPOWAPEX);
}
unsigned char Add_AlarmForeJudge_UnDirPowerFlat(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRPOWFLAT);
}
unsigned char Add_AlarmForeJudge_UnDirPowerVale(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRPOWVALE);
}
//正向无功电量
unsigned char Add_AlarmForeJudge_DirVarTotal(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRVARTOTAL);
}
unsigned char Add_AlarmForeJudge_DirVarPlus(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRVARPLUS);
}
unsigned char Add_AlarmForeJudge_DirVarApex(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRVARAPEX);
}
unsigned char Add_AlarmForeJudge_DirVarFlat(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRVARFLAT);
}
unsigned char Add_AlarmForeJudge_DirVarVale(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRVARVALE);
}
//反向无功电量
unsigned char Add_AlarmForeJudge_UnDirVarTotal(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRVARTOTAL);
}
unsigned char Add_AlarmForeJudge_UnDirVarPlus(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRVARPLUS);
}
unsigned char Add_AlarmForeJudge_UnDirVarApex(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRVARAPEX);
}
unsigned char Add_AlarmForeJudge_UnDirVarFlat(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRVARFLAT);
}
unsigned char Add_AlarmForeJudge_UnDirVarVale(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRVARVALE);
}
//1象限无功电量
unsigned char Add_AlarmForeJudge_DirVar1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR1);
}
unsigned char Add_AlarmForeJudge_DirVar1Fee1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR1_FEE1);
}
unsigned char Add_AlarmForeJudge_DirVar1Fee2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR1_FEE2);
}
unsigned char Add_AlarmForeJudge_DirVar1Fee3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR1_FEE3);
}
unsigned char Add_AlarmForeJudge_DirVar1Fee4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR1_FEE4);
}
//2象限无功电量
unsigned char Add_AlarmForeJudge_DirVar2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR2);
}
unsigned char Add_AlarmForeJudge_DirVar2Fee1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR2_FEE1);
}
unsigned char Add_AlarmForeJudge_DirVar2Fee2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR2_FEE2);
}
unsigned char Add_AlarmForeJudge_DirVar2Fee3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR2_FEE3);
}
unsigned char Add_AlarmForeJudge_DirVar2Fee4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR2_FEE4);
}
//3象限无功电量
unsigned char Add_AlarmForeJudge_DirVar3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR3);
}
unsigned char Add_AlarmForeJudge_DirVar3Fee1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR3_FEE1);
}
unsigned char Add_AlarmForeJudge_DirVar3Fee2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR3_FEE2);
}
unsigned char Add_AlarmForeJudge_DirVar3Fee3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR3_FEE3);
}
unsigned char Add_AlarmForeJudge_DirVar3Fee4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR3_FEE4);
}
//4象限无功电量
unsigned char Add_AlarmForeJudge_DirVar4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR4);
}
unsigned char Add_AlarmForeJudge_DirVar4Fee1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR4_FEE1);
}
unsigned char Add_AlarmForeJudge_DirVar4Fee2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR4_FEE2);
}
unsigned char Add_AlarmForeJudge_DirVar4Fee3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR4_FEE3);
}
unsigned char Add_AlarmForeJudge_DirVar4Fee4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR4_FEE4);
}
//电流
unsigned char Add_AlarmForeJudge_Ia(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPCURA);
}
unsigned char Add_AlarmForeJudge_Ib(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPCURB);
}
unsigned char Add_AlarmForeJudge_Ic(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPCURC);
}
//电压
unsigned char Add_AlarmForeJudge_Va(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVOLA);
}
unsigned char Add_AlarmForeJudge_Vb(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVOLB);
}
unsigned char Add_AlarmForeJudge_Vc(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVOLC);
}
//瞬时有功、无功
unsigned char Add_AlarmForeJudge_InstantPow(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPINSTANTPOW);
}
unsigned char Add_AlarmForeJudge_InstantVar(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPINSTANTVAR);
}
//正向有功最大需量
unsigned char Add_AlarmForeJudge_DirPowMaxDemand(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRPOWMAXDEMAND);
}
unsigned char Add_AlarmForeJudge_DirPowMaxDemandFee1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE1);
}
unsigned char Add_AlarmForeJudge_DirPowMaxDemandFee2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE2);
}
unsigned char Add_AlarmForeJudge_DirPowMaxDemandFee3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE3);
}
unsigned char Add_AlarmForeJudge_DirPowMaxDemandFee4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE4);
}
//正向有功最大需量发生时间
unsigned char Add_AlarmForeJudge_DirPowMaxDemandTime(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
{
	return Add_AlarmForeJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME);
}
unsigned char Add_AlarmForeJudge_DirPowMaxDemandTimeFee1(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
{
	return Add_AlarmForeJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE1);
}
unsigned char Add_AlarmForeJudge_DirPowMaxDemandTimeFee2(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
{
	return Add_AlarmForeJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE2);
}
unsigned char Add_AlarmForeJudge_DirPowMaxDemandTimeFee3(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
{
	return Add_AlarmForeJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE3);
}
unsigned char Add_AlarmForeJudge_DirPowMaxDemandTimeFee4(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
{
	return Add_AlarmForeJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE4);
}
//反向有功最大需量
unsigned char Add_AlarmForeJudge_UnDirPowMaxDemand(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRPOWMAXDEMAND);
}
unsigned char Add_AlarmForeJudge_UnDirPowMaxDemandFee1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE1);
}
unsigned char Add_AlarmForeJudge_UnDirPowMaxDemandFee2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE2);
}
unsigned char Add_AlarmForeJudge_UnDirPowMaxDemandFee3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE3);
}
unsigned char Add_AlarmForeJudge_UnDirPowMaxDemandFee4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE4);
}
//反向有功最大需量发生时间
unsigned char Add_AlarmForeJudge_UnDirPowMaxDemandTime(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
{
	return Add_AlarmForeJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME);
}
unsigned char Add_AlarmForeJudge_UnDirPowMaxDemandTimeFee1(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
{
	return Add_AlarmForeJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE1);
}
unsigned char Add_AlarmForeJudge_UnDirPowMaxDemandTimeFee2(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
{
	return Add_AlarmForeJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE2);
}
unsigned char Add_AlarmForeJudge_UnDirPowMaxDemandTimeFee3(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
{
	return Add_AlarmForeJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE3);
}
unsigned char Add_AlarmForeJudge_UnDirPowMaxDemandTimeFee4(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
{
	return Add_AlarmForeJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE4);
}
//有功、无功脉冲常数
unsigned char Add_AlarmForeJudge_PowPulseConst(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPPOWCONST);
}
unsigned char Add_AlarmForeJudge_VarPulseConst(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVARCONST);
}*/
//费率
//unsigned char Add_AlarmForeJudge_FeeRate(unsigned char ucMPNo,unsigned char ucNum,STFEERATELIST	stFeeRateList)
//{
//	return Add_AlarmForeJudge_stFeeRateValue(ucMPNo,ucNum,stFeeRateList,CON_DI_TYPE_MPFEERATE_BLOCK);
//}
////编程时间
//unsigned char Add_AlarmForeJudge_ProgramTime(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
//{
//	return Add_AlarmForeJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPLATELYPROGTIME);
//}
////编程次数
//unsigned char Add_AlarmForeJudge_ProgramCount(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
//{
//	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPPROGTIMETIMES);
//}
////电表运行状态字
//unsigned char Add_AlarmForeJudge_MeterStatFlag(unsigned char ucMPNo,unsigned char ucNum,unsigned char	ucStateFlag)
//{
//	return Add_AlarmForeJudge_ucValue(ucMPNo,ucNum,ucStateFlag,CON_DI_TYPE_MPMETERSTATFLAG);
//}
////抄表日
//unsigned char Add_AlarmForeJudge_CopyDayTime(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
//{
//	return Add_AlarmForeJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPCOPYDAYTIME);
//}
////断相次数
//unsigned char Add_AlarmForeJudge_LostVolNum(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
//{
//	return Add_AlarmForeJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPLOSTVOLNUM);
//}
////日月年
//unsigned char Add_AlarmForeJudge_Date(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
//{
//	return Add_AlarmForeJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPDATEANDWEEK);
//}
////时分秒
//unsigned char Add_AlarmForeJudge_Time(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
//{
//	return Add_AlarmForeJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPTIME);
//}


void Alarm_SetCurr_ForeJudge(unsigned char ucMPNo,unsigned char ucNum,unsigned char ucCount,unsigned char ucListNo,STDATAITEM_ALR *pstDataItem_alr)
{
	if(ucCount < CONALRFOREREPLYNUM)
	{
		stAlrForeJudgeList[ucNum].stAlrDataItem[ucCount].stDateTime =GetCurrentTime();
		stAlrForeJudgeList[ucNum].stAlrDataItem[ucCount].stDataItem_alr[ucListNo]=(*pstDataItem_alr);
	}
}

unsigned char Add_AlarmForeJudge_lValue(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal,unsigned short usItemLablePara)
{
	unsigned char  ucRet=RET_SUCCESS,i=0;
	unsigned short usItemLabel=0;
	unsigned char  ucTmpMPNo=0,ucCount=0,ucDataItemNum=0;
	STDATAITEM_ALR	   stDataItem_alr;

	ucCount = stAlrForeJudgeList[ucNum].ucAlrJudgeCount;
	if(stAlrForeJudgeList[ucNum].ucAlrDataItemNum  < CONALRFORELISTNUM) 
		ucDataItemNum=stAlrForeJudgeList[ucNum].ucAlrDataItemNum;
	else
		ucDataItemNum=CONALRFORELISTNUM;
	
	if(ucCount < CONALRFOREREPLYNUM)
	{
		for(i=0;i<ucDataItemNum;i++)
		{
			usItemLabel = stAlrForeJudgeList[ucNum].stAlrDataItem[ucCount].stDataItem_alr[i].usItemLabel;
			ucTmpMPNo	= stAlrForeJudgeList[ucNum].stAlrDataItem[ucCount].stDataItem_alr[i].ucMPNo;

			if(usItemLabel==usItemLablePara && ucTmpMPNo == ucMPNo)
			{
				SetDataItemPer_alr(&stDataItem_alr,ucMPNo,usItemLablePara,CON_ERRCODE_INVALID,(unsigned char*)(&lVal),4,0);
				Alarm_SetCurr_ForeJudge(ucMPNo,ucNum,ucCount,i,&stDataItem_alr);
				break;
			}
		}
	}
	return ucRet;
}

//unsigned char Add_AlarmForeJudge_stTimeValue(unsigned char ucMPNo,unsigned char ucNum,STDATETIME stDateTime,unsigned short usItemLablePara)
//{
//	unsigned char  ucRet=RET_SUCCESS,i=0;
//	unsigned short usItemLabel=0;
//	unsigned char  ucTmpMPNo=0,ucCount=0,ucDataItemNum=0;
//	STDATAITEM_ALR	   stDataItem_alr;
//
//	ucCount = stAlrForeJudgeList[ucNum].ucAlrJudgeCount;
//	if(stAlrForeJudgeList[ucNum].ucAlrDataItemNum  < CONALRFORELISTNUM) 
//		ucDataItemNum=stAlrForeJudgeList[ucNum].ucAlrDataItemNum;
//	else
//		ucDataItemNum=CONALRFORELISTNUM;
//	
//	if(ucCount < CONALRFOREREPLYNUM)
//	{
//		for(i=0;i<ucDataItemNum;i++)
//		{
//			usItemLabel = stAlrForeJudgeList[ucNum].stAlrDataItem[ucCount].stDataItem_alr[i].usItemLabel;
//			ucTmpMPNo	= stAlrForeJudgeList[ucNum].stAlrDataItem[ucCount].stDataItem_alr[i].ucMPNo;
//
//			if(usItemLabel==usItemLablePara && ucTmpMPNo == ucMPNo)
//			{
//				SetDataItemPer_alr(&stDataItem_alr,ucMPNo,usItemLablePara,CON_ERRCODE_INVALID,(unsigned char*)(&stDateTime),sizeof(stDateTime),0);
//				Alarm_SetCurr_ForeJudge(ucMPNo,ucNum,ucCount,i,&stDataItem_alr);
//				break;
//			}
//		}
//	}
//	return ucRet;
//}
//
//unsigned char Add_AlarmForeJudge_stFeeRateValue(unsigned char ucMPNo,unsigned char ucNum,STFEERATELIST stFeeRateList,unsigned short usItemLablePara)
//{
//	unsigned char  ucRet=RET_SUCCESS,i=0;
//	unsigned short usItemLabel=0;
//	unsigned char  ucTmpMPNo=0,ucCount=0,ucDataItemNum=0;
//	STDATAITEM_ALR	   stDataItem_alr;
//
//	ucCount = stAlrForeJudgeList[ucNum].ucAlrJudgeCount;
//	if(stAlrForeJudgeList[ucNum].ucAlrDataItemNum  < CONALRFORELISTNUM) 
//		ucDataItemNum=stAlrForeJudgeList[ucNum].ucAlrDataItemNum;
//	else
//		ucDataItemNum=CONALRFORELISTNUM;
//	
//	if(ucCount < CONALRFOREREPLYNUM)
//	{
//		for(i=0;i<ucDataItemNum;i++)
//		{
//			usItemLabel = stAlrForeJudgeList[ucNum].stAlrDataItem[ucCount].stDataItem_alr[i].usItemLabel;
//			ucTmpMPNo	= stAlrForeJudgeList[ucNum].stAlrDataItem[ucCount].stDataItem_alr[i].ucMPNo;
//
//			if(usItemLabel==usItemLablePara && ucTmpMPNo == ucMPNo)
//			{
//				SetDataItemPer_alr(&stDataItem_alr,ucMPNo,usItemLablePara,CON_ERRCODE_INVALID,(unsigned char*)(&stFeeRateList),sizeof(stFeeRateList),0);
//				Alarm_SetCurr_ForeJudge(ucMPNo,ucNum,ucCount,i,&stDataItem_alr);
//				break;
//			}
//		}
//	}
//	return ucRet;
//}
//
//unsigned char Add_AlarmForeJudge_ucValue(unsigned char ucMPNo,unsigned char ucNum,unsigned char ucState,unsigned short usItemLablePara)
//{
//	unsigned char  ucRet=RET_SUCCESS,i=0;
//	unsigned short usItemLabel=0;
//	unsigned char  ucTmpMPNo=0,ucCount=0,ucDataItemNum=0;
//	STDATAITEM_ALR	   stDataItem_alr;
//
//	ucCount = stAlrForeJudgeList[ucNum].ucAlrJudgeCount;
//	if(stAlrForeJudgeList[ucNum].ucAlrDataItemNum  < CONALRFORELISTNUM) 
//		ucDataItemNum=stAlrForeJudgeList[ucNum].ucAlrDataItemNum;
//	else
//		ucDataItemNum=CONALRFORELISTNUM;
//	
//	if(ucCount < CONALRFOREREPLYNUM)
//	{
//		for(i=0;i<ucDataItemNum;i++)
//		{
//			usItemLabel = stAlrForeJudgeList[ucNum].stAlrDataItem[ucCount].stDataItem_alr[i].usItemLabel;
//			ucTmpMPNo	= stAlrForeJudgeList[ucNum].stAlrDataItem[ucCount].stDataItem_alr[i].ucMPNo;
//
//			if(usItemLabel==usItemLablePara && ucTmpMPNo == ucMPNo)
//			{
//				SetDataItemPer_alr(&stDataItem_alr,ucMPNo,usItemLablePara,CON_ERRCODE_INVALID,(unsigned char*)(&ucState),1,0);
//				Alarm_SetCurr_ForeJudge(ucMPNo,ucNum,ucCount,i,&stDataItem_alr);
//				break;
//			}
//		}
//	}
//	return ucRet;
//}

unsigned char Alarm_GetHisState_ForeJudge(unsigned char ucNum,unsigned short usItemLabel,unsigned char ucListNo,STHISSTATE *pstHisState)
{
	unsigned char ucRet=RET_ERROR,i=0;
	STDATAITEM_ALR	stDataItem_alr;
	ClearHisState(pstHisState);

	for(i=0;i<stAlrForeJudgeList[ucNum].ucAlrDataItemNum;i++)
	{
		stDataItem_alr =stAlrForeJudgeList[ucNum].stAlrDataItem[ucListNo].stDataItem_alr[i];
		if( stDataItem_alr.usItemLabel == usItemLabel)
		{
			pstHisState->stDateTime = stAlrForeJudgeList[ucNum].stAlrDataItem[ucListNo].stDateTime;
			pstHisState->ucStateFlag = stDataItem_alr.ucDataContent[0];
			ucRet=RET_SUCCESS;
			break;
		}
	}
	return ucRet;
}

//unsigned char Alarm_GetHisVal_ForeJudge(unsigned char ucNum,unsigned short usItemLabel,unsigned char ucListNo,STHISVAL *pstHisVal)
//{
//	unsigned char ucRet=RET_ERROR,i=0;
//	STDATAITEM_ALR	stDataItem_alr;
//	ClearHisVal(pstHisVal);
//
//	for(i=0;i<stAlrForeJudgeList[ucNum].ucAlrDataItemNum;i++)
//	{
//		stDataItem_alr =stAlrForeJudgeList[ucNum].stAlrDataItem[ucListNo].stDataItem_alr[i];
//		if( stDataItem_alr.usItemLabel == usItemLabel)
//		{
//			pstHisVal->stDateTime = stAlrForeJudgeList[ucNum].stAlrDataItem[ucListNo].stDateTime;
//			if( stDataItem_alr.usDataLen == 1)
//				pstHisVal->lVal = stDataItem_alr.ucDataContent[0];
//
//			if( stDataItem_alr.usDataLen == 2)
//			{
//				pstHisVal->lVal = stDataItem_alr.ucDataContent[1];
//				pstHisVal->lVal = pstHisVal->lVal*0x100 + stDataItem_alr.ucDataContent[0];
//			}
//
//			if( stDataItem_alr.usDataLen == 4)
//			{
//				pstHisVal->lVal = stDataItem_alr.ucDataContent[3];
//				pstHisVal->lVal = pstHisVal->lVal*0x100 + stDataItem_alr.ucDataContent[2];
//				pstHisVal->lVal = pstHisVal->lVal*0x100 + stDataItem_alr.ucDataContent[1];
//				pstHisVal->lVal = pstHisVal->lVal*0x100 + stDataItem_alr.ucDataContent[0];
//			}
//
//			ucRet=RET_SUCCESS;
//			break;
//		}
//	}
//
//	return ucRet;
//}

unsigned char Alarm_GetHisTime_ForeJudge(unsigned char ucNum,unsigned short usItemLabel,unsigned char ucListNo,STHISTIME *pstHisTime)
{
	unsigned char ucRet=RET_ERROR,i=0;
	STDATAITEM_ALR	stDataItem_alr;
	ClearHisTime(pstHisTime);

	for(i=0;i<stAlrForeJudgeList[ucNum].ucAlrDataItemNum;i++)
	{
		stDataItem_alr =stAlrForeJudgeList[ucNum].stAlrDataItem[ucListNo].stDataItem_alr[i];
		if( stDataItem_alr.usItemLabel == usItemLabel)
		{
			pstHisTime->stDateTime = stAlrForeJudgeList[ucNum].stAlrDataItem[ucListNo].stDateTime;
			pstHisTime->stHisTime  = BYTE_TO_DATETIME(stDataItem_alr.ucDataContent);
			ucRet=RET_SUCCESS;
			break;
		}
	}
	return ucRet;
}

//unsigned char Alarm_GetHisFeeRate_ForeJudge(unsigned char ucNum,unsigned short usItemLabel,unsigned char ucListNo,STHISFEERATE *pstHisFeeRate)
//{
//	unsigned char ucRet=RET_ERROR,i=0;
//	STDATAITEM_ALR	stDataItem_alr;
//	ClearHisFeeRate(pstHisFeeRate);
//
//	for(i=0;i<stAlrForeJudgeList[ucNum].ucAlrDataItemNum;i++)
//	{
//		stDataItem_alr =stAlrForeJudgeList[ucNum].stAlrDataItem[ucListNo].stDataItem_alr[i];
//		if( stDataItem_alr.usItemLabel == usItemLabel)
//		{
//			pstHisFeeRate->stDateTime = stAlrForeJudgeList[ucNum].stAlrDataItem[ucListNo].stDateTime;
//			pstHisFeeRate->stFeeRateList = BYTE_TO_FEERATELIST(stDataItem_alr.ucDataContent);
//			ucRet=RET_SUCCESS;
//			break;
//		}
//	}
//	return ucRet;
//}
/*
//填写确认队列
//正向有功电量（上月）
unsigned char Add_AlarmEnsureJudge_DirPowerLmTotal(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRPOWTOTAL_LM);
}
//正向有功电量
unsigned char Add_AlarmEnsureJudge_DirPowerTotal(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRPOWTOTAL);
}
unsigned char Add_AlarmEnsureJudge_DirPowerPlus(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRPOWPLUS);
}
unsigned char Add_AlarmEnsureJudge_DirPowerApex(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRPOWAPEX);
}
unsigned char Add_AlarmEnsureJudge_DirPowerFlat(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRPOWFLAT);
}
unsigned char Add_AlarmEnsureJudge_DirPowerVale(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRPOWVALE);
}
//反向有功电量
unsigned char Add_AlarmEnsureJudge_UnDirPowerTotal(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRPOWTOTAL);
}
unsigned char Add_AlarmEnsureJudge_UnDirPowerPlus(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRPOWPLUS);
}
unsigned char Add_AlarmEnsureJudge_UnDirPowerApex(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRPOWAPEX);
}
unsigned char Add_AlarmEnsureJudge_UnDirPowerFlat(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRPOWFLAT);
}
unsigned char Add_AlarmEnsureJudge_UnDirPowerVale(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRPOWVALE);
}
//正向无功电量
unsigned char Add_AlarmEnsureJudge_DirVarTotal(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRVARTOTAL);
}
unsigned char Add_AlarmEnsureJudge_DirVarPlus(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRVARPLUS);
}
unsigned char Add_AlarmEnsureJudge_DirVarApex(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRVARAPEX);
}
unsigned char Add_AlarmEnsureJudge_DirVarFlat(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRVARFLAT);
}
unsigned char Add_AlarmEnsureJudge_DirVarVale(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRVARVALE);
}
//反向无功电量
unsigned char Add_AlarmEnsureJudge_UnDirVarTotal(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRVARTOTAL);
}
unsigned char Add_AlarmEnsureJudge_UnDirVarPlus(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRVARPLUS);
}
unsigned char Add_AlarmEnsureJudge_UnDirVarApex(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRVARAPEX);
}
unsigned char Add_AlarmEnsureJudge_UnDirVarFlat(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRVARFLAT);
}
unsigned char Add_AlarmEnsureJudge_UnDirVarVale(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRVARVALE);
}
//1象限无功电量
unsigned char Add_AlarmEnsureJudge_DirVar1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR1);
}
unsigned char Add_AlarmEnsureJudge_DirVar1Fee1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR1_FEE1);
}
unsigned char Add_AlarmEnsureJudge_DirVar1Fee2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR1_FEE2);
}
unsigned char Add_AlarmEnsureJudge_DirVar1Fee3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR1_FEE3);
}
unsigned char Add_AlarmEnsureJudge_DirVar1Fee4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR1_FEE4);
}
//2象限无功电量
unsigned char Add_AlarmEnsureJudge_DirVar2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR2);
}
unsigned char Add_AlarmEnsureJudge_DirVar2Fee1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR2_FEE1);
}
unsigned char Add_AlarmEnsureJudge_DirVar2Fee2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR2_FEE2);
}
unsigned char Add_AlarmEnsureJudge_DirVar2Fee3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR2_FEE3);
}
unsigned char Add_AlarmEnsureJudge_DirVar2Fee4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR2_FEE4);
}
//3象限无功电量
unsigned char Add_AlarmEnsureJudge_DirVar3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR3);
}
unsigned char Add_AlarmEnsureJudge_DirVar3Fee1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR3_FEE1);
}
unsigned char Add_AlarmEnsureJudge_DirVar3Fee2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR3_FEE2);
}
unsigned char Add_AlarmEnsureJudge_DirVar3Fee3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR3_FEE3);
}
unsigned char Add_AlarmEnsureJudge_DirVar3Fee4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR3_FEE4);
}
//4象限无功电量
unsigned char Add_AlarmEnsureJudge_DirVar4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR4);
}
unsigned char Add_AlarmEnsureJudge_DirVar4Fee1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR4_FEE1);
}
unsigned char Add_AlarmEnsureJudge_DirVar4Fee2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR4_FEE2);
}
unsigned char Add_AlarmEnsureJudge_DirVar4Fee3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR4_FEE3);
}
unsigned char Add_AlarmEnsureJudge_DirVar4Fee4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVAR4_FEE4);
}
//电压
unsigned char Add_AlarmEnsureJudge_Va(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVOLA);
}
unsigned char Add_AlarmEnsureJudge_Vb(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVOLB);
}
unsigned char Add_AlarmEnsureJudge_Vc(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVOLC);
}
//电流
unsigned char Add_AlarmEnsureJudge_Ia(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPCURA);
}
unsigned char Add_AlarmEnsureJudge_Ib(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPCURB);
}
unsigned char Add_AlarmEnsureJudge_Ic(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPCURC);
}
//瞬时有功、无功
unsigned char Add_AlarmEnsureJudge_InstantPow(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPINSTANTPOW);
}
unsigned char Add_AlarmEnsureJudge_InstantVar(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPINSTANTVAR);
}
//正向有功最大需量
unsigned char Add_AlarmEnsureJudge_DirPowMaxDemand(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRPOWMAXDEMAND);
}
unsigned char Add_AlarmEnsureJudge_DirPowMaxDemandFee1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE1);
}
unsigned char Add_AlarmEnsureJudge_DirPowMaxDemandFee2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE2);
}
unsigned char Add_AlarmEnsureJudge_DirPowMaxDemandFee3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE3);
}
unsigned char Add_AlarmEnsureJudge_DirPowMaxDemandFee4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE4);
}
//正向有功最大需量发生时间
unsigned char Add_AlarmEnsureJudge_DirPowMaxDemandTime(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
{
	return Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME);
}
unsigned char Add_AlarmEnsureJudge_DirPowMaxDemandTimeFee1(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
{
	return Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE1);
}
unsigned char Add_AlarmEnsureJudge_DirPowMaxDemandTimeFee2(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
{
	return Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE2);
}
unsigned char Add_AlarmEnsureJudge_DirPowMaxDemandTimeFee3(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
{
	return Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE3);
}
unsigned char Add_AlarmEnsureJudge_DirPowMaxDemandTimeFee4(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
{
	return Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE4);
}
//反向有功最大需量
unsigned char Add_AlarmEnsureJudge_UnDirPowMaxDemand(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRPOWMAXDEMAND);
}
unsigned char Add_AlarmEnsureJudge_UnDirPowMaxDemandFee1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE1);
}
unsigned char Add_AlarmEnsureJudge_UnDirPowMaxDemandFee2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE2);
}
unsigned char Add_AlarmEnsureJudge_UnDirPowMaxDemandFee3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE3);
}
unsigned char Add_AlarmEnsureJudge_UnDirPowMaxDemandFee4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE4);
}
//反向有功最大需量发生时间
unsigned char Add_AlarmEnsureJudge_UnDirPowMaxDemandTime(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
{
	return Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME);
}
unsigned char Add_AlarmEnsureJudge_UnDirPowMaxDemandTimeFee1(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
{
	return Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE1);
}
unsigned char Add_AlarmEnsureJudge_UnDirPowMaxDemandTimeFee2(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
{
	return Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE2);
}
unsigned char Add_AlarmEnsureJudge_UnDirPowMaxDemandTimeFee3(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
{
	return Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE3);
}
unsigned char Add_AlarmEnsureJudge_UnDirPowMaxDemandTimeFee4(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
{
	return Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE4);
}
//有功、无功脉冲常数
unsigned char Add_AlarmEnsureJudge_PowPulseConst(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPPOWCONST);
}
unsigned char Add_AlarmEnsureJudge_VarPulseConst(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
{
	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPVARCONST);
}*/
////费率
//unsigned char Add_AlarmEnsureJudge_FeeRate(unsigned char ucMPNo,unsigned char ucNum,STFEERATELIST	stFeeRateList)
//{
//	return Add_AlarmEnsureJudge_stFeeRateValue(ucMPNo,ucNum,stFeeRateList,CON_DI_TYPE_MPFEERATE_BLOCK);
//}
////编程时间
//unsigned char Add_AlarmEnsureJudge_ProgramTime(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
//{
//	return Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPLATELYPROGTIME);
//}
////编程次数
//unsigned char Add_AlarmEnsureJudge_ProgramCount(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
//{
//	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPPROGTIMETIMES);
//}
////电表运行状态字
//unsigned char Add_AlarmEnsureJudge_MeterStatFlag(unsigned char ucMPNo,unsigned char ucNum,unsigned char	ucStateFlag)
//{
//	return Add_AlarmEnsureJudge_ucValue(ucMPNo,ucNum,ucStateFlag,CON_DI_TYPE_MPMETERSTATFLAG);
//}
////抄表日
//unsigned char Add_AlarmEnsureJudge_CopyDayTime(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
//{
//	return Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPCOPYDAYTIME);
//}
////断相次数
//unsigned char Add_AlarmEnsureJudge_LostVolNum(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal)
//{
//	return Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,CON_DI_TYPE_MPLOSTVOLNUM);
//}
////年月日
//unsigned char Add_AlarmEnsureJudge_Date(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
//{
//	return Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPDATEANDWEEK);
//}
////时分秒
//unsigned char Add_AlarmEnsureJudge_Time(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime)
//{
//	return Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPTIME);
//}

unsigned char Add_AlarmEnsureJudge_lValue(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal,unsigned short usItemLablePara)
{
	unsigned char  ucRet=RET_SUCCESS,i=0;
	unsigned short usItemLabel=0;
	unsigned char  ucTmpMPNo=0,ucDataItemNum=0;
	STDATAITEM_ALR	   stDataItem_alr;

	if(stAlrEnsureJudgeList[ucNum].ucAlrDataItemNum < CONALRENSURELISTNUM)
		ucDataItemNum = stAlrEnsureJudgeList[ucNum].ucAlrDataItemNum;
	else
		ucDataItemNum = CONALRENSURELISTNUM;

	for(i=0;i<ucDataItemNum;i++)
	{
		usItemLabel = stAlrEnsureJudgeList[ucNum].stAlrDataItem[i].usItemLabel;
		ucTmpMPNo	= stAlrEnsureJudgeList[ucNum].stAlrDataItem[i].ucMPNo;

		if(usItemLabel==usItemLablePara && ucTmpMPNo == ucMPNo)
		{
			SetDataItemPer_alr(&stDataItem_alr,ucMPNo,usItemLablePara,CON_ERRCODE_INVALID,(unsigned char*)(&lVal),4,0);
			stAlrEnsureJudgeList[ucNum].stAlrDataItem[i]=stDataItem_alr;
			break;
		}
	}
	return ucRet;
}

//unsigned char Add_AlarmEnsureJudge_stTimeValue(unsigned char ucMPNo,unsigned char ucNum,STDATETIME stDateTime,unsigned short usItemLablePara)
//{
//	unsigned char  ucRet=RET_SUCCESS,i=0;
//	unsigned short usItemLabel=0;
//	unsigned char  ucTmpMPNo=0,ucDataItemNum=0;
//	STDATAITEM_ALR	   stDataItem_alr;
//
//	if(stAlrEnsureJudgeList[ucNum].ucAlrDataItemNum < CONALRENSURELISTNUM)
//		ucDataItemNum = stAlrEnsureJudgeList[ucNum].ucAlrDataItemNum;
//	else
//		ucDataItemNum = CONALRENSURELISTNUM;
//
//	for(i=0;i<ucDataItemNum;i++)
//	{
//		usItemLabel = stAlrEnsureJudgeList[ucNum].stAlrDataItem[i].usItemLabel;
//		ucTmpMPNo	= stAlrEnsureJudgeList[ucNum].stAlrDataItem[i].ucMPNo;
//
//		if(usItemLabel==usItemLablePara && ucTmpMPNo == ucMPNo)
//		{
//			SetDataItemPer_alr(&stDataItem_alr,ucMPNo,usItemLablePara,CON_ERRCODE_INVALID,(unsigned char*)(&stDateTime),sizeof(stDateTime),0);
//			stAlrEnsureJudgeList[ucNum].stAlrDataItem[i]=stDataItem_alr;
//			break;
//		}
//	}
//	return ucRet;
//}
//
//unsigned char Add_AlarmEnsureJudge_stFeeRateValue(unsigned char ucMPNo,unsigned char ucNum,STFEERATELIST stFeeRateList,unsigned short usItemLablePara)
//{
//	unsigned char  ucRet=RET_SUCCESS,i=0;
//	unsigned short usItemLabel=0;
//	unsigned char  ucTmpMPNo=0,ucDataItemNum=0;
//	STDATAITEM_ALR	   stDataItem_alr;
//
//	if(stAlrEnsureJudgeList[ucNum].ucAlrDataItemNum < CONALRENSURELISTNUM)
//		ucDataItemNum = stAlrEnsureJudgeList[ucNum].ucAlrDataItemNum;
//	else
//		ucDataItemNum = CONALRENSURELISTNUM;
//
//	for(i=0;i<ucDataItemNum;i++)
//	{
//		usItemLabel = stAlrEnsureJudgeList[ucNum].stAlrDataItem[i].usItemLabel;
//		ucTmpMPNo	= stAlrEnsureJudgeList[ucNum].stAlrDataItem[i].ucMPNo;
//
//		if(usItemLabel==usItemLablePara && ucTmpMPNo == ucMPNo)
//		{
//			SetDataItemPer_alr(&stDataItem_alr,ucMPNo,usItemLablePara,CON_ERRCODE_INVALID,(unsigned char*)(&stFeeRateList),sizeof(stFeeRateList),0);
//			stAlrEnsureJudgeList[ucNum].stAlrDataItem[i]=stDataItem_alr;
//			break;
//		}
//	}
//	return ucRet;
//}
//
//unsigned char Add_AlarmEnsureJudge_ucValue(unsigned char ucMPNo,unsigned char ucNum,unsigned char ucStatFlag,unsigned short usItemLablePara)
//{
//	unsigned char  ucRet=RET_SUCCESS,i=0;
//	unsigned short usItemLabel=0;
//	unsigned char  ucTmpMPNo=0,ucDataItemNum=0;
//	STDATAITEM_ALR	   stDataItem_alr;
//
//	if(stAlrEnsureJudgeList[ucNum].ucAlrDataItemNum < CONALRENSURELISTNUM)
//		ucDataItemNum = stAlrEnsureJudgeList[ucNum].ucAlrDataItemNum;
//	else
//		ucDataItemNum = CONALRENSURELISTNUM;
//
//	for(i=0;i<ucDataItemNum;i++)
//	{
//		usItemLabel = stAlrEnsureJudgeList[ucNum].stAlrDataItem[i].usItemLabel;
//		ucTmpMPNo	= stAlrEnsureJudgeList[ucNum].stAlrDataItem[i].ucMPNo;
//
//		if(usItemLabel==usItemLablePara && ucTmpMPNo == ucMPNo)
//		{
//			SetDataItemPer_alr(&stDataItem_alr,ucMPNo,usItemLablePara,CON_ERRCODE_INVALID,(unsigned char*)(&ucStatFlag),1,0);
//			stAlrEnsureJudgeList[ucNum].stAlrDataItem[i]=stDataItem_alr;
//			break;
//		}
//	}
//	return ucRet;
//}

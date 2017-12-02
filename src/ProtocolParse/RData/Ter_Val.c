/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Ter_Val.c
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-08-15    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

//void	Init_TerHisData_All(STTERTIMERVAL *pstTerTimerVal)
//{
//	unsigned char i=0;
////	Init_TerHisData(pstTerTimerVal);
// 	for( i=0;i<CONHISTERSWITCHNUM;i++ )
// 	{
//// 		ClearHisState( &(pstTerTimerVal->stHisDoorStat[i]));		//为保证停电期间的监测，门节点无须进行初始化
//// 		ClearHisState( &(pstTerTimerVal->stHisPowerStat[i]));		//供电状态无须进行初始化
// 	}
//}

void	Init_TerHisData(STTERTIMERVAL *pstTerTimerVal)
{
	unsigned char i=0;
//	for( i=0;i<CONHISTERDATANUM;i++ )
//	{
//		ClearHisVal( &(pstTerTimerVal->stHisDirPowTotal[i]));
//		ClearHisVal( &(pstTerTimerVal->stHisDirPowFee1[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisDirPowFee2[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisDirPowFee3[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisDirPowFee4[i]) );
//		
//		ClearHisVal( &(pstTerTimerVal->stHisRevPowTotal[i]));
//		ClearHisVal( &(pstTerTimerVal->stHisRevPowFee1[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisRevPowFee2[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisRevPowFee3[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisRevPowFee4[i]) );
//	}
//	//正向（反向）无功电量
//	for( i=0;i<CONHISTERDATANUM;i++ )
//	{
//		ClearHisVal( &(pstTerTimerVal->stHisDirVarTotal[i]));
//		ClearHisVal( &(pstTerTimerVal->stHisDirVarFee1[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisDirVarFee2[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisDirVarFee3[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisDirVarFee4[i]) );
//		
//		ClearHisVal( &(pstTerTimerVal->stHisRevVarTotal[i]));
//		ClearHisVal( &(pstTerTimerVal->stHisRevVarFee1[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisRevVarFee2[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisRevVarFee3[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisRevVarFee4[i]) );
//	}
//	//1234象限无功电量
//	for( i=0;i<CONHISTERDATANUM;i++ )
//	{
//		ClearHisVal( &(pstTerTimerVal->stHisDirVar1[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisDirVar1Fee1[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisDirVar1Fee2[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisDirVar1Fee3[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisDirVar1Fee4[i]) );
//		
//		ClearHisVal( &(pstTerTimerVal->stHisDirVar2[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisDirVar2Fee1[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisDirVar2Fee2[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisDirVar2Fee3[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisDirVar2Fee4[i]) );
//		
//		ClearHisVal( &(pstTerTimerVal->stHisDirVar3[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisDirVar3Fee1[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisDirVar3Fee2[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisDirVar3Fee3[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisDirVar3Fee4[i]) );
//		
//		ClearHisVal( &(pstTerTimerVal->stHisDirVar4[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisDirVar4Fee1[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisDirVar4Fee2[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisDirVar4Fee3[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisDirVar4Fee4[i]) );
//	}
//	//电压、电流
//	for( i=0;i<CONHISTERDATANUM;i++ )
//	{
//		ClearHisVal( &(pstTerTimerVal->stHisVolA[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisVolB[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisVolC[i]) );
//
//		ClearHisVal( &(pstTerTimerVal->stHisVolA_Phase[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisVolB_Phase[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisVolC_Phase[i]) );
//		
//		ClearHisVal( &(pstTerTimerVal->stHisIa[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisIb[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisIc[i]) );
//
//		ClearHisVal( &(pstTerTimerVal->stHisIa_Phase[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisIb_Phase[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisIc_Phase[i]) );
//		
//		ClearHisVal( &(pstTerTimerVal->stHisInstantPow[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisInstantPowA[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisInstantPowB[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisInstantPowC[i]) );
//		
//		ClearHisVal( &(pstTerTimerVal->stHisInstantVar[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisInstantVarA[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisInstantVarB[i]) );
//		ClearHisVal( &(pstTerTimerVal->stHisInstantVarC[i]) );
//	}
//	//需量及其发生时间
//	for( i=0;i<CONHISTERDATANUM;i++ )
//	{
//		ClearHisVal( &(pstTerTimerVal->stHisDirPowMaxDemand[i]));
//		ClearHisVal( &(pstTerTimerVal->stHisDirPowMaxDemandFee1[i]));
//		ClearHisVal( &(pstTerTimerVal->stHisDirPowMaxDemandFee2[i]));
//		ClearHisVal( &(pstTerTimerVal->stHisDirPowMaxDemandFee3[i]));
//		ClearHisVal( &(pstTerTimerVal->stHisDirPowMaxDemandFee4[i]));
//		
//		ClearHisTime(&(pstTerTimerVal->stHisDirPowMaxDemandTime[i]));
//		ClearHisTime(&(pstTerTimerVal->stHisDirPowMaxDemandTimeFee1[i]));
//		ClearHisTime(&(pstTerTimerVal->stHisDirPowMaxDemandTimeFee2[i]));
//		ClearHisTime(&(pstTerTimerVal->stHisDirPowMaxDemandTimeFee3[i]));
//		ClearHisTime(&(pstTerTimerVal->stHisDirPowMaxDemandTimeFee4[i]));
//		
//		ClearHisVal( &(pstTerTimerVal->stHisRevPowMaxDemand[i]));
//		ClearHisVal( &(pstTerTimerVal->stHisRevPowMaxDemandFee1[i]));
//		ClearHisVal( &(pstTerTimerVal->stHisRevPowMaxDemandFee2[i]));
//		ClearHisVal( &(pstTerTimerVal->stHisRevPowMaxDemandFee3[i]));
//		ClearHisVal( &(pstTerTimerVal->stHisRevPowMaxDemandFee4[i]));
//		
//		ClearHisTime(&(pstTerTimerVal->stHisRevPowMaxDemandTime[i]));
//		ClearHisTime(&(pstTerTimerVal->stHisRevPowMaxDemandTimeFee1[i]));
//		ClearHisTime(&(pstTerTimerVal->stHisRevPowMaxDemandTimeFee2[i]));
//		ClearHisTime(&(pstTerTimerVal->stHisRevPowMaxDemandTimeFee3[i]));
//		ClearHisTime(&(pstTerTimerVal->stHisRevPowMaxDemandTimeFee4[i]));
//		
//		ClearHisVal( &(pstTerTimerVal->stHisDirVarMaxDemand[i]));
//		ClearHisVal( &(pstTerTimerVal->stHisDirVarMaxDemandFee1[i]));
//		ClearHisVal( &(pstTerTimerVal->stHisDirVarMaxDemandFee2[i]));
//		ClearHisVal( &(pstTerTimerVal->stHisDirVarMaxDemandFee3[i]));
//		ClearHisVal( &(pstTerTimerVal->stHisDirVarMaxDemandFee4[i]));
//		
//		ClearHisTime(&(pstTerTimerVal->stHisDirVarMaxDemandTime[i]));
//		ClearHisTime(&(pstTerTimerVal->stHisDirVarMaxDemandTimeFee1[i]));
//		ClearHisTime(&(pstTerTimerVal->stHisDirVarMaxDemandTimeFee2[i]));
//		ClearHisTime(&(pstTerTimerVal->stHisDirVarMaxDemandTimeFee3[i]));
//		ClearHisTime(&(pstTerTimerVal->stHisDirVarMaxDemandTimeFee4[i]));
//		
//		ClearHisVal( &(pstTerTimerVal->stHisRevVarMaxDemand[i]));
//		ClearHisVal( &(pstTerTimerVal->stHisRevVarMaxDemandFee1[i]));
//		ClearHisVal( &(pstTerTimerVal->stHisRevVarMaxDemandFee2[i]));
//		ClearHisVal( &(pstTerTimerVal->stHisRevVarMaxDemandFee3[i]));
//		ClearHisVal( &(pstTerTimerVal->stHisRevVarMaxDemandFee4[i]));
//		
//		ClearHisTime(&(pstTerTimerVal->stHisRevVarMaxDemandTime[i]));
//		ClearHisTime(&(pstTerTimerVal->stHisRevVarMaxDemandTimeFee1[i]));
//		ClearHisTime(&(pstTerTimerVal->stHisRevVarMaxDemandTimeFee2[i]));
//		ClearHisTime(&(pstTerTimerVal->stHisRevVarMaxDemandTimeFee3[i]));
//		ClearHisTime(&(pstTerTimerVal->stHisRevVarMaxDemandTimeFee4[i]));
//	}
	
	//开关量等
// 	for( i=0;i<CONHISTERSWITCHNUM;i++ )
// 	{
// 		ClearHisState( &(pstTerTimerVal->stHisSwitch0Stat[i]));
// 		ClearHisState( &(pstTerTimerVal->stHisSwitch1Stat[i]));
// 		ClearHisState( &(pstTerTimerVal->stHisSwitch2Stat[i]));
// 		ClearHisState( &(pstTerTimerVal->stHisSwitch3Stat[i]));
// 	}
	//告警信息
//	for( i=0;i<CONHISTERDATANUM;i++ )
//	{
//		ClearHisState( &(pstTerTimerVal->stHisPolReverseAStat[i]));
//		ClearHisState( &(pstTerTimerVal->stHisPolReverseBStat[i]));
//		ClearHisState( &(pstTerTimerVal->stHisPolReverseCStat[i]));
//		
// 		ClearHisState( &(pstTerTimerVal->stHisShortCirA1Stat[i]));
// 		ClearHisState( &(pstTerTimerVal->stHisShortCirB1Stat[i]));
// 		ClearHisState( &(pstTerTimerVal->stHisShortCirC1Stat[i]));
// 		
// 		ClearHisState( &(pstTerTimerVal->stHisOpenCirA2Stat[i]));
// 		ClearHisState( &(pstTerTimerVal->stHisOpenCirB2Stat[i]));
// 		ClearHisState( &(pstTerTimerVal->stHisOpenCirC2Stat[i]));
// 		
// 		ClearHisState( &(pstTerTimerVal->stHisShortCirA2Stat[i]));
// 		ClearHisState( &(pstTerTimerVal->stHisShortCirB2Stat[i]));
// 		ClearHisState( &(pstTerTimerVal->stHisShortCirC2Stat[i]));
		
//		ClearHisState( &(pstTerTimerVal->stHisVolCirReveStat[i]));
//	}
 	for( i=0;i<CONHISTERSWITCHNUM;i++ )//azh 上电初始化成停电状态 就不用保存上次状态了
 	{
 		ClearHisState( &(pstTerTimerVal->stHisPowerStat[i]));		//供电状态无须进行初始化
 		pstTerTimerVal->stHisPowerStat[i].ucStateFlag = CON_SWITCH_STAT_CLOSE;

 	}
}

/*************************************************************************************************/
/*
STHISVAL Get_HistoryVal_Ter_byDI(unsigned short usDI,unsigned char ucPos)
{
	STHISVAL stHisVal;
	switch(usDI)
	{
	case CON_DI_TYPE_MPDIRPOWTOTAL:
	case CON_DI_TYPE_MPDIRPOWAPEX:
	case CON_DI_TYPE_MPDIRPOWFLAT:
	case CON_DI_TYPE_MPDIRPOWVALE:
	case CON_DI_TYPE_MPDIRPOWFLAT:
		stHisVal = Get_History_DirPower_Ter(ucPos,(unsigned char)(usDI&0x0F));
	}
		
}*/
////正向有功电量
//STHISVAL Get_History_DirPower_Ter(unsigned char ucPos,unsigned char ucFeeNo)
//{
//	STHISVAL	stHisVal;
//	if(ucPos<CONHISTERDATANUM)
//	{
//		switch(ucFeeNo)
//		{
//		case	CON_FEE_TOTAL:		return stTerTimerVal.stHisDirPowTotal[ucPos];	
//		case	CON_FEE_PLUS:		return stTerTimerVal.stHisDirPowFee1[ucPos];	
// 		case	CON_FEE_APEX:		return stTerTimerVal.stHisDirPowFee2[ucPos];	
// 		case	CON_FEE_FLAT:		return stTerTimerVal.stHisDirPowFee3[ucPos];	
// 		case	CON_FEE_VALE:		return stTerTimerVal.stHisDirPowFee4[ucPos];	
//		}
//	}
//	ClearHisVal(&stHisVal);
//	return(stHisVal);
//}
////反向有功电量
//STHISVAL Get_History_UnDirPower_Ter(unsigned char ucPos,unsigned char ucFeeNo)
//{
//	STHISVAL	stHisVal;
//	if(ucPos<CONHISTERDATANUM)
//	{
//		switch(ucFeeNo)
//		{
//		case	CON_FEE_TOTAL:		return stTerTimerVal.stHisRevPowTotal[ucPos];	
//		case	CON_FEE_PLUS:		return stTerTimerVal.stHisRevPowFee1[ucPos];	
// 		case	CON_FEE_APEX:		return stTerTimerVal.stHisRevPowFee2[ucPos];	
// 		case	CON_FEE_FLAT:		return stTerTimerVal.stHisRevPowFee3[ucPos];	
// 		case	CON_FEE_VALE:		return stTerTimerVal.stHisRevPowFee4[ucPos];	
//		}
//	}
//	ClearHisVal(&stHisVal);
//	return(stHisVal);
//}
////正向无功电量
//STHISVAL Get_History_DirVar_Ter(unsigned char ucPos,unsigned char ucFeeNo)
//{
//	STHISVAL	stHisVal;
//	if(ucPos<CONHISTERDATANUM)
//	{
//		switch(ucFeeNo)
//		{
//		case	CON_FEE_TOTAL:		return stTerTimerVal.stHisDirVarTotal[ucPos];	
//		case	CON_FEE_PLUS:		return stTerTimerVal.stHisDirVarFee1[ucPos];	
// 		case	CON_FEE_APEX:		return stTerTimerVal.stHisDirVarFee2[ucPos];	
// 		case	CON_FEE_FLAT:		return stTerTimerVal.stHisDirVarFee3[ucPos];	
// 		case	CON_FEE_VALE:		return stTerTimerVal.stHisDirVarFee4[ucPos];	
//		}
//	}
//	ClearHisVal(&stHisVal);
//	return(stHisVal);
//}
////反向无功电量
//STHISVAL Get_History_UnDirVar_Ter(unsigned char ucPos,unsigned char ucFeeNo)
//{
//	STHISVAL	stHisVal;
//	if(ucPos<CONHISTERDATANUM)
//	{
//		switch(ucFeeNo)
//		{
//		case	CON_FEE_TOTAL:		return stTerTimerVal.stHisRevVarTotal[ucPos];	
//		case	CON_FEE_PLUS:		return stTerTimerVal.stHisRevVarFee1[ucPos];	
// 		case	CON_FEE_APEX:		return stTerTimerVal.stHisRevVarFee2[ucPos];	
// 		case	CON_FEE_FLAT:		return stTerTimerVal.stHisRevVarFee3[ucPos];	
// 		case	CON_FEE_VALE:		return stTerTimerVal.stHisRevVarFee4[ucPos];	
//		}
//	}
//	ClearHisVal(&stHisVal);
//	return(stHisVal);
//}
////1象限无功电量
//STHISVAL Get_History_DirVar1_Ter(unsigned char ucPos,unsigned char ucFeeNo)
//{
//	STHISVAL	stHisVal;
//	if(ucPos<CONHISTERDATANUM)
//	{
//		switch(ucFeeNo)
//		{
//		case	CON_FEE_TOTAL:		return stTerTimerVal.stHisDirVar1[ucPos];	
//		case	CON_FEE_PLUS:		return stTerTimerVal.stHisDirVar1Fee1[ucPos];	
// 		case	CON_FEE_APEX:		return stTerTimerVal.stHisDirVar1Fee2[ucPos];	
// 		case	CON_FEE_FLAT:		return stTerTimerVal.stHisDirVar1Fee3[ucPos];	
// 		case	CON_FEE_VALE:		return stTerTimerVal.stHisDirVar1Fee4[ucPos];	
//		}
//	}
//	ClearHisVal(&stHisVal);
//	return(stHisVal);
//}
////2象限无功电量
//STHISVAL Get_History_DirVar2_Ter(unsigned char ucPos,unsigned char ucFeeNo)
//{
//	STHISVAL	stHisVal;
//	if(ucPos<CONHISTERDATANUM)
//	{
//		switch(ucFeeNo)
//		{
//		case	CON_FEE_TOTAL:		return stTerTimerVal.stHisDirVar2[ucPos];	
//		case	CON_FEE_PLUS:		return stTerTimerVal.stHisDirVar2Fee1[ucPos];	
// 		case	CON_FEE_APEX:		return stTerTimerVal.stHisDirVar2Fee2[ucPos];	
// 		case	CON_FEE_FLAT:		return stTerTimerVal.stHisDirVar2Fee3[ucPos];	
// 		case	CON_FEE_VALE:		return stTerTimerVal.stHisDirVar2Fee4[ucPos];	
//		}
//	}
//	ClearHisVal(&stHisVal);
//	return(stHisVal);
//}
////3象限无功电量
//STHISVAL Get_History_DirVar3_Ter(unsigned char ucPos,unsigned char ucFeeNo)
//{
//	STHISVAL	stHisVal;
//	if(ucPos<CONHISTERDATANUM)
//	{
//		switch(ucFeeNo)
//		{
//		case	CON_FEE_TOTAL:		return stTerTimerVal.stHisDirVar3[ucPos];	
//		case	CON_FEE_PLUS:		return stTerTimerVal.stHisDirVar3Fee1[ucPos];	
// 		case	CON_FEE_APEX:		return stTerTimerVal.stHisDirVar3Fee2[ucPos];	
// 		case	CON_FEE_FLAT:		return stTerTimerVal.stHisDirVar3Fee3[ucPos];	
// 		case	CON_FEE_VALE:		return stTerTimerVal.stHisDirVar3Fee4[ucPos];	
//		}
//	}
//	ClearHisVal(&stHisVal);
//	return(stHisVal);
//}
////4象限无功电量
//STHISVAL Get_History_DirVar4_Ter(unsigned char ucPos,unsigned char ucFeeNo)
//{
//	STHISVAL	stHisVal;
//	if(ucPos<CONHISTERDATANUM)
//	{
//		switch(ucFeeNo)
//		{
//		case	CON_FEE_TOTAL:		return stTerTimerVal.stHisDirVar4[ucPos];	
//		case	CON_FEE_PLUS:		return stTerTimerVal.stHisDirVar4Fee1[ucPos];	
// 		case	CON_FEE_APEX:		return stTerTimerVal.stHisDirVar4Fee2[ucPos];	
// 		case	CON_FEE_FLAT:		return stTerTimerVal.stHisDirVar4Fee3[ucPos];	
// 		case	CON_FEE_VALE:		return stTerTimerVal.stHisDirVar4Fee4[ucPos];	
//		}
//	}
//	ClearHisVal(&stHisVal);
//	return(stHisVal);
//}
////正向有功最大需量
//STHISVAL Get_History_DirPowMaxDemand_Ter(unsigned char ucPos,unsigned char ucFeeNo)
//{
//	STHISVAL	stHisVal;
//	if(ucPos<CONHISTERDATANUM)
//	{
//		switch(ucFeeNo)
//		{
//		case	CON_FEE_TOTAL:		return stTerTimerVal.stHisDirPowMaxDemand[ucPos];	
//		case	CON_FEE_PLUS:		return stTerTimerVal.stHisDirPowMaxDemandFee1[ucPos];	
// 		case	CON_FEE_APEX:		return stTerTimerVal.stHisDirPowMaxDemandFee2[ucPos];	
// 		case	CON_FEE_FLAT:		return stTerTimerVal.stHisDirPowMaxDemandFee3[ucPos];	
// 		case	CON_FEE_VALE:		return stTerTimerVal.stHisDirPowMaxDemandFee4[ucPos];	
//		}
//	}
//	ClearHisVal(&stHisVal);
//	return(stHisVal);
//}
////正向有功最大需量发生时间
//STHISTIME Get_History_DirPowMaxDemandTime_Ter(unsigned char ucPos,unsigned char ucFeeNo)
//{
//	STHISTIME	stHisTime;
//	if(ucPos<CONHISTERDATANUM)
//	{
//		switch(ucFeeNo)
//		{
//		case	CON_FEE_TOTAL:		return stTerTimerVal.stHisDirPowMaxDemandTime[ucPos];	
//		case	CON_FEE_PLUS:		return stTerTimerVal.stHisDirPowMaxDemandTimeFee1[ucPos];	
// 		case	CON_FEE_APEX:		return stTerTimerVal.stHisDirPowMaxDemandTimeFee2[ucPos];	
// 		case	CON_FEE_FLAT:		return stTerTimerVal.stHisDirPowMaxDemandTimeFee3[ucPos];	
// 		case	CON_FEE_VALE:		return stTerTimerVal.stHisDirPowMaxDemandTimeFee4[ucPos];	
//		}
//	}
//	ClearHisTime(&stHisTime);
//	return(stHisTime);
//}
////正向无功最大需量
//STHISVAL Get_History_DirVarMaxDemand_Ter(unsigned char ucPos,unsigned char ucFeeNo)
//{
//	STHISVAL	stHisVal;
//	if(ucPos<CONHISTERDATANUM)
//	{
//		switch(ucFeeNo)
//		{
//		case	CON_FEE_TOTAL:		return stTerTimerVal.stHisDirVarMaxDemand[ucPos];	
//		case	CON_FEE_PLUS:		return stTerTimerVal.stHisDirVarMaxDemandFee1[ucPos];	
// 		case	CON_FEE_APEX:		return stTerTimerVal.stHisDirVarMaxDemandFee2[ucPos];	
// 		case	CON_FEE_FLAT:		return stTerTimerVal.stHisDirVarMaxDemandFee3[ucPos];	
// 		case	CON_FEE_VALE:		return stTerTimerVal.stHisDirVarMaxDemandFee4[ucPos];	
//		}
//	}
//	ClearHisVal(&stHisVal);
//	return(stHisVal);
//}
////正向无功最大需量发生时间
//STHISTIME Get_History_DirVarMaxDemandTime_Ter(unsigned char ucPos,unsigned char ucFeeNo)
//{
//	STHISTIME	stHisTime;
//	if(ucPos<CONHISTERDATANUM)
//	{
//		switch(ucFeeNo)
//		{
//		case	CON_FEE_TOTAL:		return stTerTimerVal.stHisDirVarMaxDemandTime[ucPos];	
//		case	CON_FEE_PLUS:		return stTerTimerVal.stHisDirVarMaxDemandTimeFee1[ucPos];	
// 		case	CON_FEE_APEX:		return stTerTimerVal.stHisDirVarMaxDemandTimeFee2[ucPos];	
// 		case	CON_FEE_FLAT:		return stTerTimerVal.stHisDirVarMaxDemandTimeFee3[ucPos];	
// 		case	CON_FEE_VALE:		return stTerTimerVal.stHisDirVarMaxDemandTimeFee4[ucPos];	
//		}
//	}
//	ClearHisTime(&stHisTime);
//	return(stHisTime);
//}
////反向有功最大需量
//STHISVAL Get_History_UnDirPowMaxDemand_Ter(unsigned char ucPos,unsigned char ucFeeNo)
//{
//	STHISVAL	stHisVal;
//	if(ucPos<CONHISTERDATANUM)
//	{
//		switch(ucFeeNo)
//		{
//		case	CON_FEE_TOTAL:		return stTerTimerVal.stHisRevPowMaxDemand[ucPos];	
//		case	CON_FEE_PLUS:		return stTerTimerVal.stHisRevPowMaxDemandFee1[ucPos];	
// 		case	CON_FEE_APEX:		return stTerTimerVal.stHisRevPowMaxDemandFee2[ucPos];	
// 		case	CON_FEE_FLAT:		return stTerTimerVal.stHisRevPowMaxDemandFee3[ucPos];	
// 		case	CON_FEE_VALE:		return stTerTimerVal.stHisRevPowMaxDemandFee4[ucPos];	
//		}
//	}
//	ClearHisVal(&stHisVal);
//	return(stHisVal);
//}
////反向有功最大需量发生时间
//STHISTIME Get_History_UnDirPowMaxDemandTime_Ter(unsigned char ucPos,unsigned char ucFeeNo)
//{
//	STHISTIME	stHisTime;
//	if(ucPos<CONHISTERDATANUM)
//	{
//		switch(ucFeeNo)
//		{
//		case	CON_FEE_TOTAL:		return stTerTimerVal.stHisRevPowMaxDemandTime[ucPos];	
//		case	CON_FEE_PLUS:		return stTerTimerVal.stHisRevPowMaxDemandTimeFee1[ucPos];	
// 		case	CON_FEE_APEX:		return stTerTimerVal.stHisRevPowMaxDemandTimeFee2[ucPos];	
// 		case	CON_FEE_FLAT:		return stTerTimerVal.stHisRevPowMaxDemandTimeFee3[ucPos];	
// 		case	CON_FEE_VALE:		return stTerTimerVal.stHisRevPowMaxDemandTimeFee4[ucPos];	
//		}
//	}
//	ClearHisTime(&stHisTime);
//	return(stHisTime);
//}
////反向无功最大需量
//STHISVAL Get_History_UnDirVarMaxDemand_Ter(unsigned char ucPos,unsigned char ucFeeNo)
//{
//	STHISVAL	stHisVal;
//	if(ucPos<CONHISTERDATANUM)
//	{
//		switch(ucFeeNo)
//		{
//		case	CON_FEE_TOTAL:		return stTerTimerVal.stHisRevVarMaxDemand[ucPos];	
//		case	CON_FEE_PLUS:		return stTerTimerVal.stHisRevVarMaxDemandFee1[ucPos];	
// 		case	CON_FEE_APEX:		return stTerTimerVal.stHisRevVarMaxDemandFee2[ucPos];	
// 		case	CON_FEE_FLAT:		return stTerTimerVal.stHisRevVarMaxDemandFee3[ucPos];	
// 		case	CON_FEE_VALE:		return stTerTimerVal.stHisRevVarMaxDemandFee4[ucPos];	
//		}
//	}
//	ClearHisVal(&stHisVal);
//	return(stHisVal);
//}
////反向无功最大需量发生时间
//STHISTIME Get_History_UnDirVarMaxDemandTime_Ter(unsigned char ucPos,unsigned char ucFeeNo)
//{
//	STHISTIME	stHisTime;
//	if(ucPos<CONHISTERDATANUM)
//	{
//		switch(ucFeeNo)
//		{
//		case	CON_FEE_TOTAL:		return stTerTimerVal.stHisRevVarMaxDemandTime[ucPos];	
//		case	CON_FEE_PLUS:		return stTerTimerVal.stHisRevVarMaxDemandTimeFee1[ucPos];	
// 		case	CON_FEE_APEX:		return stTerTimerVal.stHisRevVarMaxDemandTimeFee2[ucPos];	
// 		case	CON_FEE_FLAT:		return stTerTimerVal.stHisRevVarMaxDemandTimeFee3[ucPos];	
// 		case	CON_FEE_VALE:		return stTerTimerVal.stHisRevVarMaxDemandTimeFee4[ucPos];	
//		}
//	}
//	ClearHisTime(&stHisTime);
//	return(stHisTime);
//}
////瞬时有功、无功
//STHISVAL Get_History_InstantPow_Ter(unsigned char ucPos,unsigned char ucNo)
//{
//	STHISVAL	stHisVal;
//	ClearHisVal(&stHisVal);
//	if(ucPos<CONHISTERDATANUM)
//	{
//        switch(ucNo)
//        {
//		case 0:	return stTerTimerVal.stHisInstantPow[ucPos];
//		case 1:	return stTerTimerVal.stHisInstantPowA[ucPos];
// 		case 2:	return stTerTimerVal.stHisInstantPowB[ucPos];
// 		case 3:	return stTerTimerVal.stHisInstantPowC[ucPos];
//        } 
//	}
//	return(stHisVal);
//}
//开关量
//STHISSTATE Get_History_DoorStat_Ter(unsigned char ucPos)
//{
//	STHISSTATE	stHisState;
// 	if(ucPos<CONHISTERSWITCHNUM)
// 		return stTerTimerVal.stHisDoorStat[ucPos];
//	ClearHisState(&stHisState);
//	return(stHisState);
//}
STHISSTATE Get_History_PowerStat_Ter(unsigned char ucPos)
{
	STHISSTATE	stHisState;
 	if(ucPos<CONHISTERSWITCHNUM)
 		return stTerTimerVal.stHisPowerStat[ucPos];
	ClearHisState(&stHisState);
	return(stHisState);
}
//STHISSTATE Get_History_SwitchStat_Ter(unsigned char ucPos,unsigned char ucSwitchNo)
//{
//	STHISSTATE	stHisState;
//	ClearHisState(&stHisState);
// 	if(ucPos<CONHISTERSWITCHNUM)
// 	{
// 		switch(ucSwitchNo)
// 		{
// 		case 0: return stTerTimerVal.stHisSwitch0Stat[ucPos];
// 		case 1: return stTerTimerVal.stHisSwitch1Stat[ucPos];
// 		case 2: return stTerTimerVal.stHisSwitch2Stat[ucPos];
// 		case 3: return stTerTimerVal.stHisSwitch3Stat[ucPos];
// 		case 4: return stTerTimerVal.stHisSwitch4Stat[ucPos];
// 		case 5: return stTerTimerVal.stHisSwitch5Stat[ucPos];
// 		case 6: return stTerTimerVal.stHisSwitch6Stat[ucPos];
// 		}
// 	}
//	return(stHisState);
//}

////告警信息……电流反极性
//STHISSTATE Get_History_PolReverseStat_Ter(unsigned char ucPos,unsigned char ucNo)
//{
//	STHISSTATE	stHisState;
//	ClearHisState(&stHisState);
//	if(ucPos<CONHISTERDATANUM)
//	{
//		switch(ucNo)
//		{
//		case 1:	return stTerTimerVal.stHisPolReverseAStat[ucPos];
//		case 2:return stTerTimerVal.stHisPolReverseBStat[ucPos];
//		case 3:return stTerTimerVal.stHisPolReverseCStat[ucPos];
//		}
//		
//	}
//	return(stHisState);
//}
////告警信息……CT一次侧短路
//STHISSTATE Get_History_ShortCir1Stat_Ter(unsigned char ucPos,unsigned char ucNo)
//{
//	STHISSTATE	stHisState;
//	ClearHisState(&stHisState);
//// 	if(ucPos<CONHISTERDATANUM)
//// 	{
//// 		switch(ucNo)
//// 		{
//// 		case 1:	return stTerTimerVal.stHisShortCirA1Stat[ucPos];
////         case 2:	return stTerTimerVal.stHisShortCirB1Stat[ucPos];
////         case 3:	return stTerTimerVal.stHisShortCirC1Stat[ucPos];
////         }
//// 	}
//	return(stHisState);
//}
////告警信息……CT二次侧开路
//STHISSTATE Get_History_OpenCir2Stat_Ter(unsigned char ucPos,unsigned char ucNo)
//{
//	STHISSTATE	stHisState;
//	ClearHisState(&stHisState);
//// 	if(ucPos<CONHISTERDATANUM)
//// 	{
//// 		switch(ucNo)
//// 		{
//// 		case 1:	return stTerTimerVal.stHisOpenCirA2Stat[ucPos];
////         case 2:	return stTerTimerVal.stHisOpenCirB2Stat[ucPos];
////         case 3:	return stTerTimerVal.stHisOpenCirC2Stat[ucPos];
////         }
//// 	}
//	return(stHisState);
//}
////告警信息……CT二次侧短路
//STHISSTATE Get_History_ShortCir2Stat_Ter(unsigned char ucPos,unsigned char ucNo)
//{
//	STHISSTATE	stHisState;
//	ClearHisState(&stHisState);
//// 	if(ucPos<CONHISTERDATANUM)
//// 		switch(ucNo)
//// 	{
//// 	case 1:	return stTerTimerVal.stHisShortCirA2Stat[ucPos];
//// 	case 2:	return stTerTimerVal.stHisShortCirB2Stat[ucPos];
//// 	case 3:	return stTerTimerVal.stHisShortCirC2Stat[ucPos];
//// 	}
//	
//	return(stHisState);
//}
////告警信息……逆相序
//STHISSTATE Get_History_VolCirReveStat_Ter(unsigned char ucPos)
//{
//	STHISSTATE	stHisState;
//	if(ucPos<CONHISTERDATANUM)
//		return stTerTimerVal.stHisVolCirReveStat[ucPos];
//	ClearHisState(&stHisState);
//	return(stHisState);
//}
////显示电压
//STHISVAL Get_History_V_Disp_Ter(unsigned char ucPos,unsigned char ucNo)
//{
//	STHISVAL	stHisVal;
//	ClearHisVal(&stHisVal);
//	if(ucPos<CONHISTERDATANUM)
//	{
//        switch(ucNo)
//        {
//		case 1:	return stTerTimerVal.stHisVolA[ucPos];
//		case 2:	return stTerTimerVal.stHisVolB[ucPos];
//		case 3:	return stTerTimerVal.stHisVolC[ucPos];
//		}
//	}
//	return(stHisVal);
//}
////显示电压相角
//STHISVAL Get_History_V_Phase_Disp_Ter(unsigned char ucPos,unsigned char ucNo)
//{
//	STHISVAL	stHisVal;
//	ClearHisVal(&stHisVal);
//	if(ucPos<CONHISTERDATANUM)
//	{
//        switch(ucNo)
//        {
//		case 1:	return stTerTimerVal.stHisVolA_Phase[ucPos];
//		case 2:	return stTerTimerVal.stHisVolB_Phase[ucPos];
//		case 3:	return stTerTimerVal.stHisVolC_Phase[ucPos];
//		}
//	}
//	return(stHisVal);
//}
////显示电流
//STHISVAL Get_History_I_Disp_Ter(unsigned char ucPos,unsigned char ucNo)
//{
//	STHISVAL	stHisVal;
//	ClearHisVal(&stHisVal);
//	if(ucPos<CONHISTERDATANUM)
//	{
//        switch(ucNo)
//        {
//		case 1:	return stTerTimerVal.stHisIa[ucPos];
//		case 2:	return stTerTimerVal.stHisIb[ucPos];
//		case 3:	return stTerTimerVal.stHisIc[ucPos];
//		}
//	}
//	return(stHisVal);
//}
////显示电流相角
//STHISVAL Get_History_I_Phase_Disp_Ter(unsigned char ucPos,unsigned char ucNo)
//{
//	STHISVAL	stHisVal;
//	ClearHisVal(&stHisVal);
//	if(ucPos<CONHISTERDATANUM)
//	{
//        switch(ucNo)
//        {
//		case 1:	return stTerTimerVal.stHisIa_Phase[ucPos];
//		case 2:	return stTerTimerVal.stHisIb_Phase[ucPos];
//		case 3:	return stTerTimerVal.stHisIc_Phase[ucPos];
//		}
//	}
//	return(stHisVal);
//}
////显示无功功率
//STHISVAL Get_History_InstantVar_Disp_Ter(unsigned char ucPos,unsigned char ucNo)
//{
//	STHISVAL	stHisVal;
//	ClearHisVal(&stHisVal);
//	if(ucPos<CONHISTERDATANUM)
//	{
//		switch(ucNo)
//		{
//        case 0:	return stTerTimerVal.stHisInstantVar[ucPos];
//		case 1:	return stTerTimerVal.stHisInstantVarA[ucPos];
//		case 2:	return stTerTimerVal.stHisInstantVarB[ucPos];
//		case 3:	return stTerTimerVal.stHisInstantVarC[ucPos];
//        }
//	}
//	return(stHisVal);
//}
//
///*************************************************************************************************/
////正向有功电量
//unsigned char Add_History_DirPower_Ter(unsigned long lVal,unsigned char ucNo)
//{
//	switch(ucNo)
//	{
//	case 0:Add_History_HisVal(stTerTimerVal.stHisDirPowTotal,CONHISTERDATANUM,lVal);break;
//	case 1:Add_History_HisVal(stTerTimerVal.stHisDirPowFee1,CONHISTERDATANUM,lVal);break;
//	case 2:Add_History_HisVal(stTerTimerVal.stHisDirPowFee2,CONHISTERDATANUM,lVal);break;
//	case 3:Add_History_HisVal(stTerTimerVal.stHisDirPowFee3,CONHISTERDATANUM,lVal);break;
//	case 4:Add_History_HisVal(stTerTimerVal.stHisDirPowFee4,CONHISTERDATANUM,lVal);break;
//	}
//	return RET_SUCCESS;
//}
//
//
////反向有功电量
//unsigned char Add_History_UnDirPower_Ter(unsigned long lVal,unsigned char ucNo)
//{
//	switch(ucNo)
//	{
//	case 0:Add_History_HisVal(stTerTimerVal.stHisRevPowTotal,CONHISTERDATANUM,lVal);break;
//	case 1:Add_History_HisVal(stTerTimerVal.stHisRevPowFee1,CONHISTERDATANUM,lVal);break;
//	case 2:Add_History_HisVal(stTerTimerVal.stHisRevPowFee2,CONHISTERDATANUM,lVal);break;
//	case 3:Add_History_HisVal(stTerTimerVal.stHisRevPowFee3,CONHISTERDATANUM,lVal);break;
//	case 4:Add_History_HisVal(stTerTimerVal.stHisRevPowFee4,CONHISTERDATANUM,lVal);break;
//	}
//	return RET_SUCCESS;
//}
//
////正向无功电量
//unsigned char Add_History_DirVar_Ter(unsigned long lVal,unsigned char ucNo)
//{
//	switch(ucNo)
//	{
//	case 0:Add_History_HisVal(stTerTimerVal.stHisDirVarTotal,CONHISTERDATANUM,lVal);break;
//	case 1:Add_History_HisVal(stTerTimerVal.stHisDirVarFee1,CONHISTERDATANUM,lVal);break;
//	case 2:Add_History_HisVal(stTerTimerVal.stHisDirVarFee2,CONHISTERDATANUM,lVal);break;
//	case 3:Add_History_HisVal(stTerTimerVal.stHisDirVarFee3,CONHISTERDATANUM,lVal);break;
//	case 4:Add_History_HisVal(stTerTimerVal.stHisDirVarFee4,CONHISTERDATANUM,lVal);break;
//	}
//	return RET_SUCCESS;
//}
////反向无功电量
//unsigned char Add_History_UnDirVar_Ter(unsigned long lVal,unsigned char ucNo)
//{
//	switch(ucNo)
//	{
//	case 0:Add_History_HisVal(stTerTimerVal.stHisRevVarTotal,CONHISTERDATANUM,lVal);break;
//	case 1:Add_History_HisVal(stTerTimerVal.stHisRevVarFee1,CONHISTERDATANUM,lVal);break;
//	case 2:Add_History_HisVal(stTerTimerVal.stHisRevVarFee2,CONHISTERDATANUM,lVal);break;
//	case 3:Add_History_HisVal(stTerTimerVal.stHisRevVarFee3,CONHISTERDATANUM,lVal);break;
//	case 4:Add_History_HisVal(stTerTimerVal.stHisRevVarFee4,CONHISTERDATANUM,lVal);break;
//	}
//	return RET_SUCCESS;
//}
//
////一象限无功
//unsigned char Add_History_DirVar1_Ter(unsigned long lVal,unsigned char ucNo)
//{
//	switch(ucNo)
//	{
//	case 0:Add_History_HisVal(stTerTimerVal.stHisDirVar1,CONHISTERDATANUM,lVal);break;
//	case 1:Add_History_HisVal(stTerTimerVal.stHisDirVar1Fee1,CONHISTERDATANUM,lVal);break;
//	case 2:Add_History_HisVal(stTerTimerVal.stHisDirVar1Fee2,CONHISTERDATANUM,lVal);break;
//	case 3:Add_History_HisVal(stTerTimerVal.stHisDirVar1Fee3,CONHISTERDATANUM,lVal);break;
//	case 4:Add_History_HisVal(stTerTimerVal.stHisDirVar1Fee4,CONHISTERDATANUM,lVal);break;
//	}
//	return RET_SUCCESS;
//}
////二象限无功
//unsigned char Add_History_DirVar2_Ter(unsigned long lVal,unsigned char ucNo)
//{
//	switch(ucNo)
//	{
//	case 0:Add_History_HisVal(stTerTimerVal.stHisDirVar2,CONHISTERDATANUM,lVal);break;
//	case 1:Add_History_HisVal(stTerTimerVal.stHisDirVar2Fee1,CONHISTERDATANUM,lVal);break;
//	case 2:Add_History_HisVal(stTerTimerVal.stHisDirVar2Fee2,CONHISTERDATANUM,lVal);break;
//	case 3:Add_History_HisVal(stTerTimerVal.stHisDirVar2Fee3,CONHISTERDATANUM,lVal);break;
//	case 4:Add_History_HisVal(stTerTimerVal.stHisDirVar2Fee4,CONHISTERDATANUM,lVal);break;
//	}
//	return RET_SUCCESS;
//}
////三象限无功
//unsigned char Add_History_DirVar3_Ter(unsigned long lVal,unsigned char ucNo)
//{
//	switch(ucNo)
//	{
//	case 0:Add_History_HisVal(stTerTimerVal.stHisDirVar3,CONHISTERDATANUM,lVal);break;
//	case 1:Add_History_HisVal(stTerTimerVal.stHisDirVar3Fee1,CONHISTERDATANUM,lVal);break;
//	case 2:Add_History_HisVal(stTerTimerVal.stHisDirVar3Fee2,CONHISTERDATANUM,lVal);break;
//	case 3:Add_History_HisVal(stTerTimerVal.stHisDirVar3Fee3,CONHISTERDATANUM,lVal);break;
//	case 4:Add_History_HisVal(stTerTimerVal.stHisDirVar3Fee4,CONHISTERDATANUM,lVal);break;
//	}
//	return RET_SUCCESS;
//}
////四象限无功
//unsigned char Add_History_DirVar4_Ter(unsigned long lVal,unsigned char ucNo)
//{
//	switch(ucNo)
//	{
//	case 0:Add_History_HisVal(stTerTimerVal.stHisDirVar4,CONHISTERDATANUM,lVal);break;
//	case 1:Add_History_HisVal(stTerTimerVal.stHisDirVar4Fee1,CONHISTERDATANUM,lVal);break;
//	case 2:Add_History_HisVal(stTerTimerVal.stHisDirVar4Fee2,CONHISTERDATANUM,lVal);break;
//	case 3:Add_History_HisVal(stTerTimerVal.stHisDirVar4Fee3,CONHISTERDATANUM,lVal);break;
//	case 4:Add_History_HisVal(stTerTimerVal.stHisDirVar4Fee4,CONHISTERDATANUM,lVal);break;
//	}
//	return RET_SUCCESS;
//}
////正向有功最大需量
//unsigned char Add_History_DirPowMaxDemand_Ter(unsigned long lVal,unsigned char ucNo)
//{
//	switch(ucNo)
//	{
//	case 0:Add_History_HisVal(stTerTimerVal.stHisDirPowMaxDemand,CONHISTERDATANUM,lVal);break;
//	case 1:Add_History_HisVal(stTerTimerVal.stHisDirPowMaxDemandFee1,CONHISTERDATANUM,lVal);break;
//	case 2:Add_History_HisVal(stTerTimerVal.stHisDirPowMaxDemandFee2,CONHISTERDATANUM,lVal);break;
//	case 3:Add_History_HisVal(stTerTimerVal.stHisDirPowMaxDemandFee3,CONHISTERDATANUM,lVal);break;
//	case 4:Add_History_HisVal(stTerTimerVal.stHisDirPowMaxDemandFee4,CONHISTERDATANUM,lVal);break;
//	}
//	return RET_SUCCESS;
//}
//
////正向有功最大需量发生时间
//unsigned char Add_History_DirPowMaxDemandTime_Ter(STDATETIME	stDateTime,unsigned char ucNo)
//{
//	switch(ucNo)
//	{
//	case 0:Add_History_HisTime(stTerTimerVal.stHisDirPowMaxDemandTime,CONHISTERDATANUM,stDateTime);break;
//	case 1:Add_History_HisTime(stTerTimerVal.stHisDirPowMaxDemandTimeFee1,CONHISTERDATANUM,stDateTime);break;
//	case 2:Add_History_HisTime(stTerTimerVal.stHisDirPowMaxDemandTimeFee2,CONHISTERDATANUM,stDateTime);break;
//	case 3:Add_History_HisTime(stTerTimerVal.stHisDirPowMaxDemandTimeFee3,CONHISTERDATANUM,stDateTime);break;
//	case 4:Add_History_HisTime(stTerTimerVal.stHisDirPowMaxDemandTimeFee4,CONHISTERDATANUM,stDateTime);break;
//	}
//	return RET_SUCCESS;
//}
////正向无功最大需量
//unsigned char Add_History_DirVarMaxDemand_Ter(unsigned long lVal,unsigned char ucNo)
//{
//	switch(ucNo)
//	{
//	case 0:Add_History_HisVal(stTerTimerVal.stHisDirVarMaxDemand,CONHISTERDATANUM,lVal);break;
//	case 1:Add_History_HisVal(stTerTimerVal.stHisDirVarMaxDemandFee1,CONHISTERDATANUM,lVal);break;
//	case 2:Add_History_HisVal(stTerTimerVal.stHisDirVarMaxDemandFee2,CONHISTERDATANUM,lVal);break;
//	case 3:Add_History_HisVal(stTerTimerVal.stHisDirVarMaxDemandFee3,CONHISTERDATANUM,lVal);break;
//	case 4:Add_History_HisVal(stTerTimerVal.stHisDirVarMaxDemandFee4,CONHISTERDATANUM,lVal);break;
//	}
//	return RET_SUCCESS;
//}
////正向无功最大需量发生时间
//unsigned char Add_History_DirVarMaxDemandTime_Ter(STDATETIME	stDateTime,unsigned char ucNo)
//{
//	switch(ucNo)
//	{
//	case 0:Add_History_HisTime(stTerTimerVal.stHisDirVarMaxDemandTime,CONHISTERDATANUM,stDateTime);break;
//	case 1:Add_History_HisTime(stTerTimerVal.stHisDirVarMaxDemandTimeFee1,CONHISTERDATANUM,stDateTime);break;
//	case 2:Add_History_HisTime(stTerTimerVal.stHisDirVarMaxDemandTimeFee1,CONHISTERDATANUM,stDateTime);break;
//	case 3:Add_History_HisTime(stTerTimerVal.stHisDirVarMaxDemandTimeFee1,CONHISTERDATANUM,stDateTime);break;
//	case 4:Add_History_HisTime(stTerTimerVal.stHisDirVarMaxDemandTimeFee1,CONHISTERDATANUM,stDateTime);break;
//	}
//	return RET_SUCCESS;
//}
//
//
////反向有功最大需量
//unsigned char Add_History_UnDirPowMaxDemand_Ter(unsigned long lVal,unsigned char ucNo)
//{
//	switch(ucNo)
//	{
//	case 0:Add_History_HisVal(stTerTimerVal.stHisRevPowMaxDemand,CONHISTERDATANUM,lVal);break;
//	case 1:Add_History_HisVal(stTerTimerVal.stHisRevPowMaxDemandFee1,CONHISTERDATANUM,lVal);break;
//	case 2:Add_History_HisVal(stTerTimerVal.stHisRevPowMaxDemandFee2,CONHISTERDATANUM,lVal);break;
//	case 3:Add_History_HisVal(stTerTimerVal.stHisRevPowMaxDemandFee3,CONHISTERDATANUM,lVal);break;
//	case 4:Add_History_HisVal(stTerTimerVal.stHisRevPowMaxDemandFee4,CONHISTERDATANUM,lVal);break;
//	}
//	return RET_SUCCESS;
//}
////反向有功最大需量发生时间
//unsigned char Add_History_UnDirPowMaxDemandTime_Ter(STDATETIME	stDateTime,unsigned char ucNo)
//{
//	switch(ucNo)
//	{
//	case 0:Add_History_HisTime(stTerTimerVal.stHisRevPowMaxDemandTime,CONHISTERDATANUM,stDateTime);break;
//	case 1:Add_History_HisTime(stTerTimerVal.stHisRevPowMaxDemandTimeFee1,CONHISTERDATANUM,stDateTime);break;
//	case 2:Add_History_HisTime(stTerTimerVal.stHisRevPowMaxDemandTimeFee2,CONHISTERDATANUM,stDateTime);break;
//	case 3:Add_History_HisTime(stTerTimerVal.stHisRevPowMaxDemandTimeFee3,CONHISTERDATANUM,stDateTime);break;
//	case 4:Add_History_HisTime(stTerTimerVal.stHisRevPowMaxDemandTimeFee4,CONHISTERDATANUM,stDateTime);break;
//	}
//	return RET_SUCCESS;
//}
////反向无功最大需量
//unsigned char Add_History_UnDirVarMaxDemand_Ter(unsigned long lVal,unsigned char ucNo)
//{
//	switch(ucNo)
//	{
//	case 0:Add_History_HisVal(stTerTimerVal.stHisRevVarMaxDemand,CONHISTERDATANUM,lVal);break;
//	case 1:Add_History_HisVal(stTerTimerVal.stHisRevVarMaxDemandFee1,CONHISTERDATANUM,lVal);break;
//	case 2:Add_History_HisVal(stTerTimerVal.stHisRevVarMaxDemandFee2,CONHISTERDATANUM,lVal);break;
//	case 3:Add_History_HisVal(stTerTimerVal.stHisRevVarMaxDemandFee3,CONHISTERDATANUM,lVal);break;
//	case 4:Add_History_HisVal(stTerTimerVal.stHisRevVarMaxDemandFee4,CONHISTERDATANUM,lVal);break;
//	}
//	return RET_SUCCESS;
//}
//
////反向无功最大需量发生时间
//unsigned char Add_History_UnDirVarMaxDemandTime_Ter(STDATETIME	stDateTime,unsigned char ucNo)
//{
//	switch(ucNo)
//	{
//	case 0:Add_History_HisTime(stTerTimerVal.stHisRevVarMaxDemandTime,CONHISTERDATANUM,stDateTime);break;
//	case 1:Add_History_HisTime(stTerTimerVal.stHisRevVarMaxDemandTimeFee1,CONHISTERDATANUM,stDateTime);break;
//	case 2:Add_History_HisTime(stTerTimerVal.stHisRevVarMaxDemandTimeFee2,CONHISTERDATANUM,stDateTime);break;
//	case 3:Add_History_HisTime(stTerTimerVal.stHisRevVarMaxDemandTimeFee3,CONHISTERDATANUM,stDateTime);break;
//	case 4:Add_History_HisTime(stTerTimerVal.stHisRevVarMaxDemandTimeFee4,CONHISTERDATANUM,stDateTime);break;
//	}
//	return RET_SUCCESS;
//}
//
////电压
//unsigned char Add_History_V_Ter(unsigned long lVal,unsigned char ucNo)
//{
//	STHISVAL	*pstHisVal=NULL;
//	unsigned long ulTmp;
//	switch(ucNo)
//	{
//	case 1:pstHisVal = &stTerTimerVal.stHisVolA[0];break;
//	case 2:pstHisVal = &stTerTimerVal.stHisVolB[0];break;
//	case 3:pstHisVal = &stTerTimerVal.stHisVolC[0];break;
//	}
//	if(lVal==CONINVALIDVAL || pstHisVal->lVal==CONINVALIDVAL)
//		Add_History_HisVal(pstHisVal,CONHISTERDATANUM,lVal);
//	else
//	{
//		ulTmp=pstHisVal->lVal+pstHisVal->lVal;
//		if(ulTmp<lVal)
//			ulTmp=lVal;
//		else
//			ulTmp=(pstHisVal->lVal+lVal+lVal+lVal)/4;
//		Add_History_HisVal(pstHisVal,CONHISTERDATANUM,ulTmp);
//	}
//	return RET_SUCCESS;
//}
////电压相角
//unsigned char Add_History_V_Phase_Ter(unsigned long lVal,unsigned char ucNo)
//{
//	switch(ucNo)
//	{
//	case 1:Add_History_HisVal(stTerTimerVal.stHisVolA_Phase,CONHISTERDATANUM,lVal);break;
//	case 2:Add_History_HisVal(stTerTimerVal.stHisVolB_Phase,CONHISTERDATANUM,lVal);break;
//	case 3:Add_History_HisVal(stTerTimerVal.stHisVolC_Phase,CONHISTERDATANUM,lVal);break;
//	}
//	return RET_SUCCESS;
//}
////电流
//unsigned char Add_History_I_Ter(unsigned long lVal,unsigned char ucNo)
//{
//	switch(ucNo)
//	{
//	case 1:Add_History_HisVal(stTerTimerVal.stHisIa,CONHISTERDATANUM,lVal);break;
//	case 2:Add_History_HisVal(stTerTimerVal.stHisIb,CONHISTERDATANUM,lVal);break;
//	case 3:Add_History_HisVal(stTerTimerVal.stHisIc,CONHISTERDATANUM,lVal);break;
//	}
//	return RET_SUCCESS;
//}
////电流相角
//unsigned char Add_History_I_Phase_Ter(unsigned long lVal,unsigned char ucNo)
//{
//	switch(ucNo)
//	{
//	case 1:Add_History_HisVal(stTerTimerVal.stHisIa_Phase,CONHISTERDATANUM,lVal);break;
//	case 2:Add_History_HisVal(stTerTimerVal.stHisIb_Phase,CONHISTERDATANUM,lVal);break;
//	case 3:Add_History_HisVal(stTerTimerVal.stHisIc_Phase,CONHISTERDATANUM,lVal);break;
//	}
//	return RET_SUCCESS;
//}
////瞬时有功
//unsigned char Add_History_InstantPow_Ter(unsigned long lVal,unsigned char ucSign,unsigned char ucNo)
//{
//	switch(ucNo)
//	{
//	case 0:Add_History_HisVal(stTerTimerVal.stHisInstantPow,CONHISTERDATANUM,lVal); 
//		stTerTimerVal.stHisInstantPow[0].ucSign=ucSign;break;
//	case 1:Add_History_HisVal(stTerTimerVal.stHisInstantPowA,CONHISTERDATANUM,lVal); 
//		stTerTimerVal.stHisInstantPowA[0].ucSign=ucSign;break;
//	case 2:Add_History_HisVal(stTerTimerVal.stHisInstantPowB,CONHISTERDATANUM,lVal);
//		stTerTimerVal.stHisInstantPowB[0].ucSign=ucSign;break;
//	case 3:Add_History_HisVal(stTerTimerVal.stHisInstantPowC,CONHISTERDATANUM,lVal); 
//		stTerTimerVal.stHisInstantPowC[0].ucSign=ucSign;break;
//	}
//	
//	return RET_SUCCESS;
//}
////瞬时无功
//unsigned char Add_History_InstantVar_Ter(unsigned long lVal,unsigned char ucSign,unsigned char ucNo)
//{
//	switch(ucNo)
//	{
//	case 0:Add_History_HisVal(stTerTimerVal.stHisInstantVar,CONHISTERDATANUM,lVal);
//		stTerTimerVal.stHisInstantVar[0].ucSign=ucSign; break;
//	case 1:Add_History_HisVal(stTerTimerVal.stHisInstantVarA,CONHISTERDATANUM,lVal); 
//		stTerTimerVal.stHisInstantVarA[0].ucSign=ucSign;break;
//	case 2:Add_History_HisVal(stTerTimerVal.stHisInstantVarB,CONHISTERDATANUM,lVal);
//		stTerTimerVal.stHisInstantVarB[0].ucSign=ucSign;  break;
//	case 3:Add_History_HisVal(stTerTimerVal.stHisInstantVarC,CONHISTERDATANUM,lVal);
//		stTerTimerVal.stHisInstantVarC[0].ucSign=ucSign; break;
//	}	
//	return RET_SUCCESS;
//}
//门节点状态
//unsigned char Add_History_DoorStat_Ter(unsigned char	ucStateFlag)
//{
//	Add_History_HisState(stTerTimerVal.stHisDoorStat,CONHISTERSWITCHNUM,ucStateFlag);
//	return RET_SUCCESS;
//}
//供电状态
unsigned char Add_History_PowerStat_Ter(unsigned char	ucStateFlag)
{
	Add_History_HisState(stTerTimerVal.stHisPowerStat,CONHISTERSWITCHNUM,ucStateFlag);
	return RET_SUCCESS;
}
//开关1状态
//unsigned char Add_History_SwitchStat_Ter(unsigned char	ucStateFlag,unsigned char ucSwitchNo)
//{
// 	switch(ucSwitchNo)
// 	{
//	case 0:	Add_History_HisState(stTerTimerVal.stHisSwitch0Stat,CONHISTERSWITCHNUM,ucStateFlag);break;
// 	case 1:	Add_History_HisState(stTerTimerVal.stHisSwitch1Stat,CONHISTERSWITCHNUM,ucStateFlag);break;
// 	case 2:	Add_History_HisState(stTerTimerVal.stHisSwitch2Stat,CONHISTERSWITCHNUM,ucStateFlag);break;
// 	case 3:	Add_History_HisState(stTerTimerVal.stHisSwitch3Stat,CONHISTERSWITCHNUM,ucStateFlag);break;
// 	case 4:	Add_History_HisState(stTerTimerVal.stHisSwitch4Stat,CONHISTERSWITCHNUM,ucStateFlag);break;
// 	case 5:	Add_History_HisState(stTerTimerVal.stHisSwitch5Stat,CONHISTERSWITCHNUM,ucStateFlag);break;
// 	case 6:	Add_History_HisState(stTerTimerVal.stHisSwitch6Stat,CONHISTERSWITCHNUM,ucStateFlag);break;
// 	}
//	return RET_SUCCESS;
//}

////告警信息……电流反极性
//unsigned char  Add_History_PolReverseStat_Ter(unsigned char ucStateFlag,unsigned char ucNo)
//{
//	switch(ucNo)
//	{
//	case 1:Add_History_HisState(stTerTimerVal.stHisPolReverseAStat,CONHISTERDATANUM,ucStateFlag);break;
//	case 2:Add_History_HisState(stTerTimerVal.stHisPolReverseBStat,CONHISTERDATANUM,ucStateFlag);break;
//	case 3:Add_History_HisState(stTerTimerVal.stHisPolReverseCStat,CONHISTERDATANUM,ucStateFlag);break;
//	}
//	return RET_SUCCESS;
//}
////告警信息……CT一次侧短路
//unsigned char  Add_History_ShortCir1Stat_Ter(unsigned char ucStateFlag,unsigned char ucNo)
//{
//// 	switch(ucNo)
//// 	{
//// 	case 1:Add_History_HisState(stTerTimerVal.stHisShortCirA1Stat,CONHISTERDATANUM,ucStateFlag);break;
//// 	case 2:Add_History_HisState(stTerTimerVal.stHisShortCirB1Stat,CONHISTERDATANUM,ucStateFlag);break;
//// 	case 3:Add_History_HisState(stTerTimerVal.stHisShortCirC1Stat,CONHISTERDATANUM,ucStateFlag);break;
//// 	}
//	return RET_SUCCESS;
//}
////告警信息……CT二次侧开路
//unsigned char  Add_History_OpenCir2Stat_Ter(unsigned char ucStateFlag,unsigned char ucNo)
//{
//// 	switch(ucNo)
//// 	{
//// 	case 1:Add_History_HisState(stTerTimerVal.stHisOpenCirA2Stat,CONHISTERDATANUM,ucStateFlag);break;
//// 	case 2:Add_History_HisState(stTerTimerVal.stHisOpenCirB2Stat,CONHISTERDATANUM,ucStateFlag);break;
//// 	case 3:Add_History_HisState(stTerTimerVal.stHisOpenCirC2Stat,CONHISTERDATANUM,ucStateFlag);break;
//// 	}
//	return RET_SUCCESS;
//}
////告警信息……CT二次侧短路
//unsigned char  Add_History_ShortCir2Stat_Ter(unsigned char ucStateFlag,unsigned char ucNo)
//{
//// 	switch(ucNo)
//// 	{
//// 	case 1:	Add_History_HisState(stTerTimerVal.stHisShortCirA2Stat,CONHISTERDATANUM,ucStateFlag);break;
//// 	case 2:	Add_History_HisState(stTerTimerVal.stHisShortCirB2Stat,CONHISTERDATANUM,ucStateFlag);break;
//// 	case 3:	Add_History_HisState(stTerTimerVal.stHisShortCirC2Stat,CONHISTERDATANUM,ucStateFlag);break;
//// 	}
//	return RET_SUCCESS;
//}
////告警信息……逆相序
//unsigned char  Add_History_VolCirReveStat_Ter(unsigned char ucStateFlag)
//{
//	Add_History_HisState(stTerTimerVal.stHisVolCirReveStat,CONHISTERDATANUM,ucStateFlag);
//	return RET_SUCCESS;
//}
//
//unsigned char  Get_Status_TerPowLow()
//{
//	/*
//	unsigned long ulVoltHigh,ulVoltLow;
//	unsigned long ulVoltA=0,ulVoltB=0,ulVoltC=0;
//	unsigned char ucErrCount=0;
//	STHISVAL	stHisVal;
//	if(ADECheckLineType_Is3P3L()==CON_STATUS_YES)				//需要保证先ADEInit获得正确的接线类型
//		ulVoltHigh=10000;
//	else
//		ulVoltHigh=22000;
//	ulVoltHigh=ulVoltHigh*9/10;
//	ulVoltLow=ulVoltHigh/2;
//	
//	stHisVal=Get_History_V_Disp_Ter(0,1);
//	if(stHisVal.lVal!=CONINVALIDVAL)
//		ulVoltA=stHisVal.lVal;
//	stHisVal=Get_History_V_Disp_Ter(0,2);
//	if(stHisVal.lVal!=CONINVALIDVAL)
//		ulVoltB=stHisVal.lVal;
//	stHisVal=Get_History_V_Disp_Ter(0,3);
//	if(stHisVal.lVal!=CONINVALIDVAL)
//		ulVoltC=stHisVal.lVal;
//	
//	//处理A最大，B中间
//	if(ulVoltA<ulVoltB)
//	{//A>B
//		stHisVal.lVal=ulVoltA;
//		ulVoltA=ulVoltB;
//		ulVoltB=stHisVal.lVal;
//	}
//	if(ulVoltA<ulVoltC)
//	{//C MAX
//		ulVoltB=ulVoltA;
//		ulVoltA=ulVoltC;
//	}
//	else if(ulVoltB>ulVoltC)
//	{//C MIN
//		;
//	}
//	else
//	{//C MIDD
//		ulVoltB=ulVoltC;
//	}
//	
//	if(ulVoltA<ulVoltHigh && ulVoltA>ulVoltLow && ulVoltB<ulVoltLow)
//		return CON_STATUS_YES;
//	else
//		return CON_STATUS_NO;
//	*/
//	return CON_STATUS_NO;
//}

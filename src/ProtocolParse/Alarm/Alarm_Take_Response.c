/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Alarm_Take_Response.c
 Description    : 告警附带数据抄读返回解析
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

//static unsigned char Fill_AlarmTake_Vol_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
//static unsigned char Fill_AlarmTake_Cur_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
//static unsigned char Fill_AlarmTake_InstantPow_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
//static unsigned char Fill_AlarmTake_InstantVar_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTake_DirPower_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTake_UnDirPower_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
//static unsigned char Fill_AlarmTake_DirPowerLm_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTake_DirVar_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
static unsigned char Fill_AlarmTake_UnDirVar_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
//static unsigned char Fill_AlarmTake_DirVar1_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
//static unsigned char Fill_AlarmTake_DirVar2_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
//static unsigned char Fill_AlarmTake_DirVar3_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
//static unsigned char Fill_AlarmTake_DirVar4_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
//static unsigned char Fill_AlarmTake_DirPowMaxDemand_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
//static unsigned char Fill_AlarmTake_DirPowMaxDemandTime_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
//static unsigned char Fill_AlarmTake_UnDirPowMaxDemand_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
//static unsigned char Fill_AlarmTake_UnDirPowMaxDemandTime_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
//static unsigned char Fill_AlarmTake_PulseConst_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
//static unsigned char Fill_AlarmTake_FeeRate_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
//static unsigned char Fill_AlarmTake_ProgramTime_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
//static unsigned char Fill_AlarmTake_MeterStatFlag_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
//static unsigned char Fill_AlarmTake_CopyDay_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
//static unsigned char Fill_AlarmTake_LostVolNum_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
//static unsigned char Fill_AlarmTake_DateTime_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);


unsigned char Fill_AlarmTake_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
//	Fill_AlarmTake_Vol_Manage(ucMPNo,ucNum,usDILabel,ucIsValid);
//	Fill_AlarmTake_Cur_Manage(ucMPNo,ucNum,usDILabel,ucIsValid);
//	Fill_AlarmTake_InstantPow_Manage(ucMPNo,ucNum,usDILabel,ucIsValid);
//	Fill_AlarmTake_InstantVar_Manage(ucMPNo,ucNum,usDILabel,ucIsValid);
	Fill_AlarmTake_DirPower_Manage(ucMPNo,ucNum,usDILabel,ucIsValid);
	Fill_AlarmTake_UnDirPower_Manage(ucMPNo,ucNum, usDILabel,ucIsValid);
//	Fill_AlarmTake_DirPowerLm_Manage(ucMPNo,ucNum, usDILabel,ucIsValid);
	Fill_AlarmTake_DirVar_Manage(ucMPNo,ucNum, usDILabel,ucIsValid);
	Fill_AlarmTake_UnDirVar_Manage(ucMPNo,ucNum, usDILabel,ucIsValid);
//	Fill_AlarmTake_DirVar1_Manage(ucMPNo,ucNum, usDILabel,ucIsValid);
//	Fill_AlarmTake_DirVar2_Manage(ucMPNo,ucNum, usDILabel,ucIsValid);
//	Fill_AlarmTake_DirVar3_Manage(ucMPNo,ucNum, usDILabel,ucIsValid);
//	Fill_AlarmTake_DirVar4_Manage(ucMPNo,ucNum, usDILabel,ucIsValid);
//	Fill_AlarmTake_DirPowMaxDemand_Manage(ucMPNo,ucNum, usDILabel,ucIsValid);
//	Fill_AlarmTake_DirPowMaxDemandTime_Manage(ucMPNo,ucNum, usDILabel,ucIsValid);
//	Fill_AlarmTake_UnDirPowMaxDemand_Manage(ucMPNo,ucNum, usDILabel,ucIsValid);
//	Fill_AlarmTake_UnDirPowMaxDemandTime_Manage(ucMPNo,ucNum, usDILabel,ucIsValid);
//	Fill_AlarmTake_PulseConst_Manage(ucMPNo,ucNum, usDILabel,ucIsValid);
//	Fill_AlarmTake_FeeRate_Manage(ucMPNo,ucNum, usDILabel,ucIsValid);
//	Fill_AlarmTake_ProgramTime_Manage(ucMPNo,ucNum, usDILabel,ucIsValid);
//	Fill_AlarmTake_MeterStatFlag_Manage(ucMPNo,ucNum, usDILabel,ucIsValid);
//	Fill_AlarmTake_CopyDay_Manage(ucMPNo,ucNum, usDILabel,ucIsValid);
//	Fill_AlarmTake_LostVolNum_Manage(ucMPNo,ucNum, usDILabel,ucIsValid);
//	Fill_AlarmTake_DateTime_Manage(ucMPNo,ucNum, usDILabel,ucIsValid);
	return ucRet;
}

///******************************三相电压：下发目前全国规约是小项命令/但也不可避免大项命令********/
////告警数据补召
//unsigned char Fill_AlarmTake_Vol_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned long lVal=0;
//
//	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPVOLA:					//A相电压
//	case CON_DI_TYPE_MPVOLB:					//B相电压
//	case CON_DI_TYPE_MPVOLC:					//C相电压
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
//		break;
//	case CON_DI_TYPE_MPVOL_BLOCK:				//三相电压
//	case CON_DI_TYPE_MPALLDI:
//		if(ucIsValid==CONALRTME_INVALID_FLAG)
//		{
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPVOLA);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPVOLB);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPVOLC);
//		}
//		break;
//	default: 
//		ucRet=RET_ERROR;
//	}
//	return ucRet;
//}
//
///******************************三相电流：下发目前全国规约是小项命令/但也不可避免大项命令********/
//unsigned char Fill_AlarmTake_Cur_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned long lVal=0;
//
//	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPCURA:					//A相电流
//	case CON_DI_TYPE_MPCURB:					//B相电流
//	case CON_DI_TYPE_MPCURC:					//C相电流
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
//		break;
//	case CON_DI_TYPE_MPCUR_BLOCK:				//三相电流
//	case CON_DI_TYPE_MPALLDI:
//		if(ucIsValid==CONALRTME_INVALID_FLAG)
//		{
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPCURA);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPCURB);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPCURC);
//		}
//		break;
//	default: 
//		ucRet=RET_ERROR;
//	}
//	return ucRet;
//}
//
///******************************瞬时有功：下发目前全国规约是小项命令/但也不可避免大项命令********/
//unsigned char Fill_AlarmTake_InstantPow_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned long lVal=0;
//
//	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPINSTANTPOW:					//瞬时有功
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
//		break;
//	case CON_DI_TYPE_MPINSTANTPOW_BLOCK:			//瞬时有功数据块
//	case CON_DI_TYPE_MPALLDI:
//		if(ucIsValid==CONALRTME_INVALID_FLAG)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPINSTANTPOW);
//		break;
//	default: 
//		ucRet=RET_ERROR;
//	}
//	return ucRet;
//}
//
///******************************瞬时无功：下发目前全国规约是小项命令/但也不可避免大项命令********/
//unsigned char Fill_AlarmTake_InstantVar_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned long lVal=0;
//
//	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPINSTANTVAR:					//瞬时无功
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
//		break;
//	case CON_DI_TYPE_MPINSTANTVAR_BLOCK:			//瞬时无功数据块
//	case CON_DI_TYPE_MPALLDI:
//		if(ucIsValid==CONALRTME_INVALID_FLAG)	
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPINSTANTVAR);
//		break;
//	default: 
//		ucRet=RET_ERROR;
//	}
//	return ucRet;
//}

/******************************正向有功电量：下发目前全国规约是小项命令/但也不可避免大项命令********/
unsigned char Fill_AlarmTake_DirPower_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
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
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
		else
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
		break;
	case CON_DI_TYPE_MPDIRPOW_BLOCK:				//正向有功数据块
	case CON_DI_TYPE_MPALLDI:
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPDIRPOWTOTAL);
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPDIRPOWPLUS);
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPDIRPOWAPEX);
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPDIRPOWFLAT);
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPDIRPOWVALE);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTake_UnDirPower_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
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
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
		else
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
		break;
	case CON_DI_TYPE_MPUNDIRPOW_BLOCK:					//反向有功数据块
	case CON_DI_TYPE_MPALLDI:
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPUNDIRPOWTOTAL);
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPUNDIRPOWPLUS);
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPUNDIRPOWAPEX);
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPUNDIRPOWFLAT);
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPUNDIRPOWVALE);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

//unsigned char Fill_AlarmTake_DirPowerLm_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned long lVal=0;
//
//	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPDIRPOWTOTAL_LM:					//正向有功电量(上月)
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
//		break;
//	case CON_DI_TYPE_MPDIRPOW_LM_BLOCK:					//正向有功数据块
//	case CON_DI_TYPE_MPALLDI:
//		if(ucIsValid==CONALRTME_INVALID_FLAG)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPDIRPOWTOTAL_LM);
//		break;
//	default: 
//		ucRet=RET_ERROR;
//	}
//	return ucRet;
//}

unsigned char Fill_AlarmTake_DirVar_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
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
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
		else
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
		break;
	case CON_DI_TYPE_MPDIRVAR_BLOCK:				//正向无功数据块
	case CON_DI_TYPE_MPALLDI:
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPDIRVARTOTAL);
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPDIRVARPLUS);
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPDIRVARAPEX);
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPDIRVARFLAT);
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPDIRVARVALE);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

unsigned char Fill_AlarmTake_UnDirVar_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
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
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
		else
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
		break;
	case CON_DI_TYPE_MPUNDIRVAR_BLOCK:					//反向无功数据块
	case CON_DI_TYPE_MPALLDI:
		if(ucIsValid==CONALRTME_INVALID_FLAG)
		{
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPUNDIRVARTOTAL);
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPUNDIRVARPLUS);
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPUNDIRVARAPEX);
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPUNDIRVARFLAT);
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPUNDIRVARVALE);
		}
		break;
	default: 
		ucRet=RET_ERROR;
	}
	return ucRet;
}

//unsigned char Fill_AlarmTake_DirVar1_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned long lVal=0;
//
//	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPVAR1:					//一象限无功总电量
//	case CON_DI_TYPE_MPVAR1_FEE1:					//一象限无功尖电量
//	case CON_DI_TYPE_MPVAR1_FEE2:					//一象限无功峰电量
//	case CON_DI_TYPE_MPVAR1_FEE3:					//一象限无功平电量
//	case CON_DI_TYPE_MPVAR1_FEE4:					//一象限无功谷电量
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
//		break;
//	case CON_DI_TYPE_MPVAR1_BLOCK:					//一象限无功数据块
//	case CON_DI_TYPE_MPALLDI:
//		if(ucIsValid==CONALRTME_INVALID_FLAG)
//		{
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPVAR1);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPVAR1_FEE1);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPVAR1_FEE2);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPVAR1_FEE3);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPVAR1_FEE4);
//		}
//		break;
//	default: 
//		ucRet=RET_ERROR;
//	}
//	return ucRet;
//}
//
//unsigned char Fill_AlarmTake_DirVar2_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned long lVal=0;
//
//	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPVAR2:						//二象限无功总电量
//	case CON_DI_TYPE_MPVAR2_FEE1:					//二象限无功尖电量
//	case CON_DI_TYPE_MPVAR2_FEE2:					//二象限无功峰电量
//	case CON_DI_TYPE_MPVAR2_FEE3:					//二象限无功平电量
//	case CON_DI_TYPE_MPVAR2_FEE4:					//二象限无功谷电量
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
//		break;
//	case CON_DI_TYPE_MPVAR2_BLOCK:					//二象限无功数据块
//	case CON_DI_TYPE_MPALLDI:
//		if(ucIsValid==CONALRTME_INVALID_FLAG)
//		{
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPVAR2);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPVAR2_FEE1);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPVAR2_FEE2);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPVAR2_FEE3);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPVAR2_FEE4);
//		}
//		break;
//	default: 
//		ucRet=RET_ERROR;
//	}
//	return ucRet;
//}
//
//unsigned char Fill_AlarmTake_DirVar3_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned long lVal=0;
//
//	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPVAR3:					//三象限无功总电量
//	case CON_DI_TYPE_MPVAR3_FEE1:					//三象限无功尖电量
//	case CON_DI_TYPE_MPVAR3_FEE2:					//三象限无功峰电量
//	case CON_DI_TYPE_MPVAR3_FEE3:					//三象限无功平电量
//	case CON_DI_TYPE_MPVAR3_FEE4:					//三象限无功谷电量
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
//		break;
//	case CON_DI_TYPE_MPVAR3_BLOCK:					//三象限无功数据块
//	case CON_DI_TYPE_MPALLDI:
//		if(ucIsValid==CONALRTME_INVALID_FLAG)
//		{
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPVAR3);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPVAR3_FEE1);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPVAR3_FEE2);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPVAR3_FEE3);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPVAR3_FEE4);
//		}
//		break;
//	default: 
//		ucRet=RET_ERROR;
//	}
//	return ucRet;
//}
//
//unsigned char Fill_AlarmTake_DirVar4_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned long lVal=0;
//
//	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPVAR4:					//四象限无功总电量
//	case CON_DI_TYPE_MPVAR4_FEE1:					//四象限无功尖电量
//	case CON_DI_TYPE_MPVAR4_FEE2:					//四象限无功峰电量
//	case CON_DI_TYPE_MPVAR4_FEE3:					//四象限无功平电量
//	case CON_DI_TYPE_MPVAR4_FEE4:					//四象限无功谷电量
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
//		break;
//	case CON_DI_TYPE_MPVAR4_BLOCK:					//四象限无功数据块
//	case CON_DI_TYPE_MPALLDI:
//		if(ucIsValid==CONALRTME_INVALID_FLAG)
//		{
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPVAR4);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPVAR4_FEE1);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPVAR4_FEE2);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPVAR4_FEE3);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPVAR4_FEE4);
//		}
//		break;
//	default: 
//		ucRet=RET_ERROR;
//	}
//	return ucRet;
//}
//
//unsigned char Fill_AlarmTake_DirPowMaxDemand_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned long lVal=0;
//
//	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND:						//正向有功总最大需量
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE1:					//正向有功尖最大需量
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE2:					//正向有功峰最大需量
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE3:					//正向有功平最大需量
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE4:					//正向有功谷最大需量
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
//		break;
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_BLOCK:					//正向有功最大需量数据块
//	case CON_DI_TYPE_MPALLDI:
//		if(ucIsValid==CONALRTME_INVALID_FLAG)
//		{
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPDIRPOWMAXDEMAND);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE1);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE2);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE3);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE4);
//		}
//		break;
//	default: 
//		ucRet=RET_ERROR;
//	}
//	return ucRet;
//}
//
//unsigned char Fill_AlarmTake_DirPowMaxDemandTime_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	STDATETIME	stDateTime,stAviDateTime;
//	ClearDateTime(&stDateTime);
//	
//	stAviDateTime = BYTE_TO_DATETIME(stRS485RecvExpainVal.ucDataArea);
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME:						//正向有功总最大需量发生时间
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE1:					//正向有功尖最大需量发生时间
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE2:					//正向有功峰最大需量发生时间
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE3:					//正向有功平最大需量发生时间
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE4:					//正向有功谷最大需量发生时间
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stAviDateTime,usDILabel);
//		break;
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_BLOCK:					//正向有功最大需量发生时间数据块
//	case CON_DI_TYPE_MPALLDI:
//		if(ucIsValid==CONALRTME_INVALID_FLAG)
//		{
//			ucRet=Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME);
//			ucRet=Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE1);
//			ucRet=Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE2);
//			ucRet=Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE3);
//			ucRet=Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE4);
//		}
//		break;
//	default: 
//		ucRet=RET_ERROR;
//	}
//	return ucRet;
//}
//
//unsigned char Fill_AlarmTake_UnDirPowMaxDemand_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned long lVal=0;
//
//	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND:						//反向有功总最大需量
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE1:					//反向有功尖最大需量
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE2:					//反向有功峰最大需量
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE3:					//反向有功平最大需量
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE4:					//反向有功谷最大需量
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
//		break;
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_BLOCK:					//反向有功最大需量数据块
//	case CON_DI_TYPE_MPALLDI:
//		if(ucIsValid==CONALRTME_INVALID_FLAG)
//		{
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPUNDIRPOWMAXDEMAND);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE1);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE2);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE3);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE4);
//		}
//		break;
//	default: 
//		ucRet=RET_ERROR;
//	}
//	return ucRet;
//}
//
//unsigned char Fill_AlarmTake_UnDirPowMaxDemandTime_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	STDATETIME	stDateTime,stAviDateTime;
//	ClearDateTime(&stDateTime);
//	
//	stAviDateTime = BYTE_TO_DATETIME(stRS485RecvExpainVal.ucDataArea);
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME:						//反向有功总最大需量发生时间
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE1:					//反向有功尖最大需量发生时间
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE2:					//反向有功峰最大需量发生时间
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE3:					//反向有功平最大需量发生时间
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE4:					//反向有功谷最大需量发生时间
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stAviDateTime,usDILabel);
//		break;
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_BLOCK:					//反向有功最大需量发生时间数据块
//	case CON_DI_TYPE_MPALLDI:
//		if(ucIsValid==CONALRTME_INVALID_FLAG)
//		{
//			ucRet=Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME);
//			ucRet=Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE1);
//			ucRet=Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE2);
//			ucRet=Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE3);
//			ucRet=Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE4);
//		}
//		break;
//	default: 
//		ucRet=RET_ERROR;
//	}
//	return ucRet;
//}
//
//unsigned char Fill_AlarmTake_PulseConst_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned long lVal=0;
//
//	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPPOWCONST:					//有功脉冲常数
//	case CON_DI_TYPE_MPVARCONST:					//无功脉冲常数
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
//		break;
//	case CON_DI_TYPE_MPCONST_BLOCK:					//脉冲常数
//	case CON_DI_TYPE_MPALLDI:
//		if(ucIsValid==CONALRTME_INVALID_FLAG)
//		{
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPPOWCONST);
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPVARCONST);
//		}
//		break;
//	default: 
//		ucRet=RET_ERROR;
//	}
//	return ucRet;
//}
//
//unsigned char Fill_AlarmTake_FeeRate_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	STFEERATELIST	stFeeRateList,stAviRateList;
//
//	ClearFeeRateList(&stFeeRateList);
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPFEERATE_BLOCK:				//费率数据块
//	case CON_DI_TYPE_MPALLDI:
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STFEERATELIST))==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_FeeRate(ucMPNo,ucNum,stFeeRateList);
//		else
//		{
//			stAviRateList = BYTE_TO_FEERATELIST(stRS485RecvExpainVal.ucDataArea);
//			ucRet=Add_AlarmEnsureJudge_FeeRate(ucMPNo,ucNum,stAviRateList);
//		}
//		break;
//	default: 
//		ucRet=RET_ERROR;
//	}
//	return ucRet;
//}
//
//unsigned char Fill_AlarmTake_ProgramTime_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	STDATETIME	stDateTime,stAviDateTime;
//	unsigned long lVal=0;
//
//	ClearDateTime(&stDateTime);
//	
//	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
//	stAviDateTime = BYTE_TO_DATETIME(stRS485RecvExpainVal.ucDataArea);
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPLATELYPROGTIME:			//编程时间
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_ProgramTime(ucMPNo,ucNum,stDateTime);
//		else
//			ucRet=Add_AlarmEnsureJudge_ProgramTime(ucMPNo,ucNum,stAviDateTime);
//		break;
//	case CON_DI_TYPE_MPPROGTIMETIMES:			//编程次数
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_ProgramCount(ucMPNo,ucNum,CONINVALIDVAL);
//		else
//			ucRet=Add_AlarmEnsureJudge_ProgramCount(ucMPNo,ucNum,lVal);
//		break;
//	case CON_DI_TYPE_MPB21F_BLOCK:				//编程时间
//	case CON_DI_TYPE_MPALLDI:
//		if(ucIsValid==CONALRTME_INVALID_FLAG)
//		{
//			ucRet=Add_AlarmEnsureJudge_ProgramTime(ucMPNo,ucNum,stDateTime);
//			ucRet=Add_AlarmEnsureJudge_ProgramCount(ucMPNo,ucNum,CONINVALIDVAL);
//		}
//		break;
//	default: 
//		ucRet=RET_ERROR;
//	}
//	return ucRet;
//}
//
//unsigned char Fill_AlarmTake_MeterStatFlag_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucByte=0;
//
//	ucByte=stRS485RecvExpainVal.ucDataArea[0];
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPMETERSTATFLAG:				//电表状态字
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,1)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_MeterStatFlag(ucMPNo,ucNum,CONINVALIDVAL);
//		else
//			ucRet=Add_AlarmEnsureJudge_MeterStatFlag(ucMPNo,ucNum,ucByte);
//		break;
//	case CON_DI_TYPE_MPB21F_BLOCK:				//编程时间
//	case CON_DI_TYPE_MPALLDI:
//		if(ucIsValid==CONALRTME_INVALID_FLAG)
//			ucRet=Add_AlarmEnsureJudge_MeterStatFlag(ucMPNo,ucNum,CONINVALIDVAL);
//		break;
//	default: 
//		ucRet=RET_ERROR;
//	}
//	return ucRet;
//}
//
//unsigned char Fill_AlarmTake_CopyDay_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	STDATETIME	stDateTime,stAviDateTime;
//	ClearDateTime(&stDateTime);
//	
//	stAviDateTime = BYTE_TO_DATETIME(stRS485RecvExpainVal.ucDataArea);
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPCOPYDAYTIME:				//抄表日
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_CopyDayTime(ucMPNo,ucNum,stDateTime);
//		else
//			ucRet=Add_AlarmEnsureJudge_CopyDayTime(ucMPNo,ucNum,stAviDateTime);
//		break;
//	case CON_DI_TYPE_MPC11F_BLOCK:				//抄表日
//	case CON_DI_TYPE_MPALLDI:
//		if(ucIsValid==CONALRTME_INVALID_FLAG)
//			ucRet=Add_AlarmEnsureJudge_CopyDayTime(ucMPNo,ucNum,stDateTime);
//		break;
//	default: 
//		ucRet=RET_ERROR;
//	}
//	return ucRet;
//}
//
//unsigned char Fill_AlarmTake_LostVolNum_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned long lVal=0;
//
//	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPLOSTVOLNUM:				//电表断相次数
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_LostVolNum(ucMPNo,ucNum,CONINVALIDVAL);
//		else
//			ucRet=Add_AlarmEnsureJudge_LostVolNum(ucMPNo,ucNum,lVal);
//		break;
//	case CON_DI_TYPE_MPB31F_BLOCK:				//电表断相次数
//	case CON_DI_TYPE_MPALLDI:
//		if(ucIsValid==CONALRTME_INVALID_FLAG)
//			ucRet=Add_AlarmEnsureJudge_LostVolNum(ucMPNo,ucNum,CONINVALIDVAL);
//		break;
//	default: 
//		ucRet=RET_ERROR;
//	}
//	return ucRet;
//}
//
//unsigned char Fill_AlarmTake_DateTime_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	STDATETIME	stDateTime,stAviDateTime;
//	ClearDateTime(&stDateTime);
//	
//	stAviDateTime = BYTE_TO_DATETIME(stRS485RecvExpainVal.ucDataArea);
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPDATEANDWEEK:				//年月日
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_Date(ucMPNo,ucNum,stDateTime);
//		else
//			ucRet=Add_AlarmEnsureJudge_Date(ucMPNo,ucNum,stAviDateTime);
//		break;
//	case CON_DI_TYPE_MPTIME:					//时分秒
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_Time(ucMPNo,ucNum,stDateTime);
//		else
//			ucRet=Add_AlarmEnsureJudge_Time(ucMPNo,ucNum,stAviDateTime);
//		break;
//	case CON_DI_TYPE_MPDATETIME_BLOCK:			
//	case CON_DI_TYPE_MPALLDI:
//		if(ucIsValid==CONALRTME_INVALID_FLAG)
//		{
//			ucRet=Add_AlarmEnsureJudge_Date(ucMPNo,ucNum,stDateTime);
//			ucRet=Add_AlarmEnsureJudge_Time(ucMPNo,ucNum,stDateTime);
//		}
//		break;
//	default: 
//		ucRet=RET_ERROR;
//	}
//	return ucRet;
//}

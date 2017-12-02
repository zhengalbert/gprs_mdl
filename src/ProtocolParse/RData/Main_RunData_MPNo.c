/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_RunData_MPNo.c
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-08-15    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

STHISVAL Get_History_DirPower_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_DirPower(ucMeterNo,ucPos,ucFeeNo);
	}
//	else if(ucMPType == CON_MPTYPE_TER)
//	{
//		if(ucPos == CONHISDATA_LASTVALID_POS)
//			stHisVal = Get_History_DirPower_Ter(0,ucFeeNo);
//		else
//			stHisVal = Get_History_DirPower_Ter(ucPos,ucFeeNo);
//	}
//	else if(ucMPType == CON_MPTYPE_PULSE)
// 	{
// 		ucPulseNo	= RunPara_GetDirPowPulseNo_byMPNo(ucMPNo);
// 		if(ucPos == CONHISDATA_LASTVALID_POS)
// 			stHisVal=Get_History_DirPower_byPulseNo(ucPulseNo,0,ucFeeNo);
// 		else
// 			stHisVal=Get_History_DirPower_byPulseNo(ucPulseNo,ucPos,ucFeeNo);
// 	}
    else
		ClearHisVal(&stHisVal);

	return stHisVal;
}

//读高精度电量0.000KWH
STHISVAL Get_History_DirPower_byMPNo_High(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_DirPower(ucMeterNo,ucPos,ucFeeNo);
//		if((stHisVal.lVal&0xff) != CONINVALIDVAL)
        if(stHisVal.lVal != CONINVALIDVAL)//azh 140918  

			stHisVal.lVal *= 10;
	}
//	else if(ucMPType == CON_MPTYPE_TER)
//	{
//		if(ucPos == CONHISDATA_LASTVALID_POS)
//			stHisVal = Get_History_DirPower_Ter(0,ucFeeNo);
//		else
//			stHisVal = Get_History_DirPower_Ter(ucPos,ucFeeNo);
//		if(stHisVal.lVal != CONINVALIDVAL)
//			stHisVal.lVal *= 10;
//	}
// 	else if(ucMPType == CON_MPTYPE_PULSE)
// 	{
// 		ucPulseNo	= RunPara_GetDirPowPulseNo_byMPNo(ucMPNo);
// 		if(ucPos == CONHISDATA_LASTVALID_POS)
// 			stHisVal=Get_P1000_History_DirPower_byMPNo(ucMPNo,0,ucFeeNo);
// 		else
// 			stHisVal=Get_P1000_History_DirPower_byMPNo(ucMPNo,ucPos,ucFeeNo);
// 		if(stHisVal.lVal != CONINVALIDVAL)
// 			stHisVal.lVal *= 10;
// 	}
    else
		ClearHisVal(&stHisVal);

	return stHisVal;
}

STHISVAL Get_History_UnDirPower_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_UnDirPower(ucMeterNo,ucPos,ucFeeNo);
	}
//	else if(ucMPType == CON_MPTYPE_TER)
//	{
//		if(ucPos == CONHISDATA_LASTVALID_POS)
//			stHisVal = Get_History_UnDirPower_Ter(0,ucFeeNo);				//只有表有最近有效一点数据
//		else
//			stHisVal = Get_History_UnDirPower_Ter(ucPos,ucFeeNo);
//	}
// 	else if(ucMPType == CON_MPTYPE_PULSE)
// 	{
// 		ucPulseNo	= RunPara_GetUnDirPowPulseNo_byMPNo(ucMPNo);
// 		if(ucPos == CONHISDATA_LASTVALID_POS)
// 			stHisVal=Get_History_UnDirPower_byPulseNo(ucPulseNo,0,ucFeeNo);	//只有表有最近有效一点数据
// 		else
// 			stHisVal=Get_History_UnDirPower_byPulseNo(ucPulseNo,ucPos,ucFeeNo);
// 	}
    else
		ClearHisVal(&stHisVal);

	return stHisVal;
}
STHISVAL Get_History_DirVar_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_DirVar(ucMeterNo,ucPos,ucFeeNo);
	}
//	else if(ucMPType == CON_MPTYPE_TER)
//	{
//		if(ucPos == CONHISDATA_LASTVALID_POS)
//			stHisVal = Get_History_DirVar_Ter(0,ucFeeNo);				//只有表有最近有效一点数据
//		else
//			stHisVal = Get_History_DirVar_Ter(ucPos,ucFeeNo);
//	}
// 	else if(ucMPType == CON_MPTYPE_PULSE)
// 	{
// 		ucPulseNo	= RunPara_GetDirVarPulseNo_byMPNo(ucMPNo);
// 		if(ucPos == CONHISDATA_LASTVALID_POS)
// 			stHisVal=Get_History_DirVar_byPulseNo(ucPulseNo,0,ucFeeNo);	//只有表有最近有效一点数据
// 		else
// 			stHisVal=Get_History_DirVar_byPulseNo(ucPulseNo,ucPos,ucFeeNo);
// 	}
    else
		ClearHisVal(&stHisVal);

	return stHisVal;
}

STHISVAL Get_History_UnDirVar_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_UnDirVar(ucMeterNo,ucPos,ucFeeNo);
	}
//	else if(ucMPType == CON_MPTYPE_TER)
//	{
//		stHisVal = Get_History_UnDirVar_Ter(ucPos,ucFeeNo);
//	}
//     else if(ucMPType == CON_MPTYPE_PULSE)
// 	{
// 		ucPulseNo	= RunPara_GetUnDirVarPulseNo_byMPNo(ucMPNo);
// 		stHisVal=Get_History_UnDirVar_byPulseNo(ucPulseNo,ucPos,ucFeeNo);
// 	}
    else
		ClearHisVal(&stHisVal);

	return stHisVal;
}

//1象限无功电量
STHISVAL Get_History_DirVar1_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_DirVar1(ucMeterNo,ucPos,ucFeeNo);
	}
//	else if(ucMPType == CON_MPTYPE_TER)
//	{
//		stHisVal = Get_History_DirVar1_Ter(ucPos,ucFeeNo);
//	}
	else
		ClearHisVal(&stHisVal);

	return stHisVal;
}
//2象限无功电量
STHISVAL Get_History_DirVar2_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_DirVar2(ucMeterNo,ucPos,ucFeeNo);
	}
//	else if(ucMPType == CON_MPTYPE_TER)
//	{
//		stHisVal = Get_History_DirVar2_Ter(ucPos,ucFeeNo);
//	}
	else
		ClearHisVal(&stHisVal);

	return stHisVal;
}
//3象限无功电量
STHISVAL Get_History_DirVar3_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_DirVar3(ucMeterNo,ucPos,ucFeeNo);
	}
//	else if(ucMPType == CON_MPTYPE_TER)
//	{
//		stHisVal = Get_History_DirVar3_Ter(ucPos,ucFeeNo);
//	}
	else
		ClearHisVal(&stHisVal);

	return stHisVal;
}
//4象限无功电量
STHISVAL Get_History_DirVar4_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_DirVar4(ucMeterNo,ucPos,ucFeeNo);
	}
//	else if(ucMPType == CON_MPTYPE_TER)
//	{
//		stHisVal = Get_History_DirVar4_Ter(ucPos,ucFeeNo);
//	}
	else
		ClearHisVal(&stHisVal);

	return stHisVal;
}
//正向有功最大需量
STHISVAL Get_History_DirPowMaxDemand_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_DirPowMaxDemand(ucMeterNo,ucPos,ucFeeNo);
	}
//	else if(ucMPType == CON_MPTYPE_TER)
//	{
//		stHisVal = Get_History_DirPowMaxDemand_Ter(ucPos,ucFeeNo);
//	}
//     else if(ucMPType == CON_MPTYPE_PULSE)
// 	{
// 		ucPulseNo	= RunPara_GetDirPowPulseNo_byMPNo(ucMPNo);
// 		stHisVal=Get_History_DirPowMaxDemand_byPulseNo(ucPulseNo,ucPos,ucFeeNo);
// 	}
    else
		ClearHisVal(&stHisVal);

	return stHisVal;
}
//正向有功最大需量发生时间
STHISTIME Get_History_DirPowMaxDemandTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char  ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	STHISTIME	stHisTime;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisTime=Get_History_DirPowMaxDemandTime(ucMeterNo,ucPos,ucFeeNo);
	}
//	else if(ucMPType == CON_MPTYPE_TER)
//	{
//		stHisTime = Get_History_DirPowMaxDemandTime_Ter(ucPos,ucFeeNo);
//	}
//     else if(ucMPType == CON_MPTYPE_PULSE)
// 	{
// 		ucPulseNo	= RunPara_GetDirPowPulseNo_byMPNo(ucMPNo);
// 		stHisTime=Get_History_DirPowMaxDemandTime_byPulseNo(ucPulseNo,ucPos,ucFeeNo);
// 	}
    else
		ClearHisTime(&stHisTime);

	return stHisTime;
}
//正向无功最大需量
STHISVAL Get_History_DirVarMaxDemand_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_DirVarMaxDemand(ucMeterNo,ucPos,ucFeeNo);
	}
//    else if(ucMPType == CON_MPTYPE_TER)
//	{
//		stHisVal = Get_History_DirVarMaxDemand_Ter(ucPos,ucFeeNo);
//	}
//     else if(ucMPType == CON_MPTYPE_PULSE)
// 	{
// 		ucPulseNo	= RunPara_GetDirVarPulseNo_byMPNo(ucMPNo);
// 		stHisVal=Get_History_DirVarMaxDemand_byPulseNo(ucPulseNo,ucPos,ucFeeNo);
// 	}
    else
		ClearHisVal(&stHisVal);

	return stHisVal;
}
STHISVAL Get_History_DirPowA_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo = 0;
	STHISVAL stHisVal;
	ucMPType	= RunPara_GetMPType(ucMPNo);	
	if(ucMPType == CON_MPTYPE_METER)
	{
		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal = Get_History_DirPowA(ucMeterNo, ucPos,ucNo);
	}else
		ClearHisVal(&stHisVal);
	return stHisVal;
}
STHISVAL Get_History_DirPowB_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo = 0;
	STHISVAL stHisVal;
	
	ucMPType	= RunPara_GetMPType(ucMPNo);	
	if(ucMPType == CON_MPTYPE_METER)
	{
		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal = Get_History_DirPowB(ucMeterNo, ucPos,ucNo);
	}else
		ClearHisVal(&stHisVal);
	return stHisVal;
}
STHISVAL Get_History_DirPowC_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo = 0;
	STHISVAL stHisVal;
	ucMPType	= RunPara_GetMPType(ucMPNo);
	
	if(ucMPType == CON_MPTYPE_METER)
	{
		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal = Get_History_DirPowC(ucMeterNo, ucPos,ucNo);
	}else
		ClearHisVal(&stHisVal);
	return stHisVal;
}
STHISVAL Get_History_DirPowALm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo = 0;
	STHISVAL stHisVal;
	ucMPType	= RunPara_GetMPType(ucMPNo);	
	if(ucMPType == CON_MPTYPE_METER)
	{
		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal = Get_History_DirPowALm(ucMeterNo, ucPos,ucNo);
	}else
		ClearHisVal(&stHisVal);
	return stHisVal;
}
STHISVAL Get_History_DirPowBLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo = 0;
	STHISVAL stHisVal;
	
	ucMPType	= RunPara_GetMPType(ucMPNo);	
	if(ucMPType == CON_MPTYPE_METER)
	{
		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal = Get_History_DirPowBLm(ucMeterNo, ucPos,ucNo);
	}else
		ClearHisVal(&stHisVal);
	return stHisVal;
}
STHISVAL Get_History_DirPowCLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo = 0;
	STHISVAL stHisVal;
	ucMPType	= RunPara_GetMPType(ucMPNo);
	
	if(ucMPType == CON_MPTYPE_METER)
	{
		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal = Get_History_DirPowCLm(ucMeterNo, ucPos,ucNo);
	}else
		ClearHisVal(&stHisVal);
	return stHisVal;
}

//正向无功最大需量发生时间
STHISTIME Get_History_DirVarMaxDemandTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	STHISTIME	stHisTime;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisTime=Get_History_DirVarMaxDemandTime(ucMeterNo,ucPos,ucFeeNo);
	}
//	else if(ucMPType == CON_MPTYPE_TER)
//	{
//		stHisTime = Get_History_DirVarMaxDemandTime_Ter(ucPos,ucFeeNo);
//	}
//     else if(ucMPType == CON_MPTYPE_PULSE)
// 	{
// 		ucPulseNo	= RunPara_GetDirVarPulseNo_byMPNo(ucMPNo);
// 		stHisTime=Get_History_DirVarMaxDemandTime_byPulseNo(ucPulseNo,ucPos,ucFeeNo);
// 	}
    else
		ClearHisTime(&stHisTime);

	return stHisTime;
}
//反向有功最大需量
STHISVAL Get_History_UnDirPowMaxDemand_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_UnDirPowMaxDemand(ucMeterNo,ucPos,ucFeeNo);
	}
//	else if(ucMPType == CON_MPTYPE_TER)
//	{
//		stHisVal = Get_History_UnDirPowMaxDemand_Ter(ucPos,ucFeeNo);
//	}
//     else if(ucMPType == CON_MPTYPE_PULSE)
// 	{
// 		ucPulseNo	= RunPara_GetUnDirPowPulseNo_byMPNo(ucMPNo);
// 		stHisVal=Get_History_UnDirPowMaxDemand_byPulseNo(ucPulseNo,ucPos,ucFeeNo);
// 	}
    else
		ClearHisVal(&stHisVal);

	return stHisVal;
}
//反向有功最大需量发生时间
STHISTIME Get_History_UnDirPowMaxDemandTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	STHISTIME	stHisTime;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisTime=Get_History_UnDirPowMaxDemandTime(ucMeterNo,ucPos,ucFeeNo);
	}
//    else if(ucMPType == CON_MPTYPE_TER)
//	{
//		stHisTime = Get_History_UnDirPowMaxDemandTime_Ter(ucPos,ucFeeNo);
//	}
//     else if(ucMPType == CON_MPTYPE_PULSE)
// 	{
// 		ucPulseNo	= RunPara_GetUnDirPowPulseNo_byMPNo(ucMPNo);
// 		stHisTime=Get_History_UnDirPowMaxDemandTime_byPulseNo(ucPulseNo,ucPos,ucFeeNo);
// 	}
    else
		ClearHisTime(&stHisTime);

	return stHisTime;
}
//反向无功最大需量
STHISVAL Get_History_UnDirVarMaxDemand_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_UnDirVarMaxDemand(ucMeterNo,ucPos,ucFeeNo);
	}
//	else if(ucMPType == CON_MPTYPE_TER)
//	{
//		stHisVal = Get_History_UnDirVarMaxDemand_Ter(ucPos,ucFeeNo);
//	}
//     else if(ucMPType == CON_MPTYPE_PULSE)
// 	{
// 		ucPulseNo	= RunPara_GetUnDirVarPulseNo_byMPNo(ucMPNo);
// 		stHisVal=Get_History_UnDirVarMaxDemand_byPulseNo(ucPulseNo,ucPos,ucFeeNo);
// 	}
    else
		ClearHisVal(&stHisVal);

	return stHisVal;
}
//反向无功最大需量发生时间
STHISTIME Get_History_UnDirVarMaxDemandTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	STHISTIME	stHisTime;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisTime=Get_History_UnDirVarMaxDemandTime(ucMeterNo,ucPos,ucFeeNo);
	}
//	else if(ucMPType == CON_MPTYPE_TER)
//	{
//		stHisTime = Get_History_UnDirVarMaxDemandTime_Ter(ucPos,ucFeeNo);
//	}
//     else if(ucMPType == CON_MPTYPE_PULSE)
// 	{
// 		ucPulseNo	= RunPara_GetUnDirVarPulseNo_byMPNo(ucMPNo);
// 		stHisTime=Get_History_UnDirVarMaxDemandTime_byPulseNo(ucPulseNo,ucPos,ucFeeNo);
// 	}
    else
		ClearHisTime(&stHisTime);

	return stHisTime;
}
//正向有功电量（上月）
STHISVAL Get_History_DirPowerLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_DirPowerLm(ucMeterNo,ucPos,ucFeeNo);
	}
//     else if(ucMPType == CON_MPTYPE_PULSE)
// 	{
// 		ucPulseNo	= RunPara_GetDirPowPulseNo_byMPNo(ucMPNo);
// 		stHisVal=Get_History_DirPower_LM_byPulseNo(ucPulseNo,ucFeeNo);
// 	}
    else
		ClearHisVal(&stHisVal);

	return stHisVal;
}
//反向有功电量(上月)
STHISVAL Get_History_UnDirPowerLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_UnDirPowerLm(ucMeterNo,ucPos,ucFeeNo);
	}
//     else if(ucMPType == CON_MPTYPE_PULSE)
// 	{
// 		ucPulseNo	= RunPara_GetUnDirPowPulseNo_byMPNo(ucMPNo);
// 		stHisVal=Get_History_UnDirPower_LM_byPulseNo(ucPulseNo,ucFeeNo);
// 	}
    else
		ClearHisVal(&stHisVal);

	return stHisVal;
}
//正向无功电量(上月)
STHISVAL Get_History_DirVarLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_DirVarLm(ucMeterNo,ucPos,ucFeeNo);
	}
//     else if(ucMPType == CON_MPTYPE_PULSE)
// 	{
// 		ucPulseNo	= RunPara_GetDirVarPulseNo_byMPNo(ucMPNo);
// 		stHisVal=Get_History_DirVar_LM_byPulseNo(ucPulseNo,ucFeeNo);
// 	}
    else
		ClearHisVal(&stHisVal);

	return stHisVal;
}
//反向无功电量(上月)
STHISVAL Get_History_UnDirVarLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_UnDirVarLm(ucMeterNo,ucPos,ucFeeNo);
	}
//     else if(ucMPType == CON_MPTYPE_PULSE)
// 	{
// 		ucPulseNo	= RunPara_GetUnDirVarPulseNo_byMPNo(ucMPNo);
// 		stHisVal=Get_History_UnDirVar_LM_byPulseNo(ucPulseNo,ucFeeNo);
// 	}
    else
		ClearHisVal(&stHisVal);

	return stHisVal;
}
//1象限无功电量(上月)
STHISVAL Get_History_DirVar1Lm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_DirVar1Lm(ucMeterNo,ucPos,ucFeeNo);
	}else
		ClearHisVal(&stHisVal);

	return stHisVal;
}
//2象限无功电量(上月)
STHISVAL Get_History_DirVar2Lm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_DirVar2Lm(ucMeterNo,ucPos,ucFeeNo);
	}else
		ClearHisVal(&stHisVal);

	return stHisVal;
}
//3象限无功电量(上月)
STHISVAL Get_History_DirVar3Lm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_DirVar3Lm(ucMeterNo,ucPos,ucFeeNo);
	}else
		ClearHisVal(&stHisVal);

	return stHisVal;
}
//4象限无功电量(上月)
STHISVAL Get_History_DirVar4Lm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_DirVar4Lm(ucMeterNo,ucPos,ucFeeNo);
	}else
		ClearHisVal(&stHisVal);

	return stHisVal;
}
//正向有功最大需量(上月)
STHISVAL Get_History_DirPowMaxDemandLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_DirPowMaxDemandLm(ucMeterNo,ucPos,ucFeeNo);
	}
//     else if(ucMPType == CON_MPTYPE_PULSE)
// 	{
// 		ucPulseNo	= RunPara_GetDirPowPulseNo_byMPNo(ucMPNo);
// 		stHisVal=Get_History_DirPowMaxDemand_LM_byPulseNo(ucPulseNo,ucFeeNo);
// 	}
    else
		ClearHisVal(&stHisVal);

	return stHisVal;
}
//正向有功最大需量发生时间(上月)
STHISTIME Get_History_DirPowMaxDemandTimeLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	STHISTIME	stHisTime;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisTime=Get_History_DirPowMaxDemandTimeLm(ucMeterNo,ucPos,ucFeeNo);
	}
//	else if(ucMPType == CON_MPTYPE_PULSE)
//	{
//		ucPulseNo	= RunPara_GetDirPowPulseNo_byMPNo(ucMPNo);
//		stHisTime=Get_History_DirPowMaxDemandTime_LM_byPulseNo(ucPulseNo,ucFeeNo);
//	}
	else
		ClearHisTime(&stHisTime);

	return stHisTime;
}
//正向无功最大需量(上月)
STHISVAL Get_History_DirVarMaxDemandLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_DirVarMaxDemandLm(ucMeterNo,ucPos,ucFeeNo);
	}
//	else if(ucMPType == CON_MPTYPE_PULSE)
//	{
//		ucPulseNo	= RunPara_GetDirVarPulseNo_byMPNo(ucMPNo);
//		stHisVal=Get_History_DirVarMaxDemand_LM_byPulseNo(ucPulseNo,ucFeeNo);
//	}
	else
		ClearHisVal(&stHisVal);

	return stHisVal;
}
//正向无功最大需量发生时间(上月)
STHISTIME Get_History_DirVarMaxDemandTimeLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	STHISTIME	stHisTime;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisTime=Get_History_DirVarMaxDemandTimeLm(ucMeterNo,ucPos,ucFeeNo);
	}
//	else if(ucMPType == CON_MPTYPE_PULSE)
//	{
//		ucPulseNo	= RunPara_GetDirVarPulseNo_byMPNo(ucMPNo);
//		stHisTime=Get_History_DirVarMaxDemandTime_LM_byPulseNo(ucPulseNo,ucFeeNo);
//	}
	else
		ClearHisTime(&stHisTime);

	return stHisTime;
}
//反向有功最大需量(上月)
STHISVAL Get_History_UnDirPowMaxDemandLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_UnDirPowMaxDemandLm(ucMeterNo,ucPos,ucFeeNo);
	}
//	else if(ucMPType == CON_MPTYPE_PULSE)
//	{
//		ucPulseNo	= RunPara_GetUnDirPowPulseNo_byMPNo(ucMPNo);
//		stHisVal=Get_History_UnDirPowMaxDemand_LM_byPulseNo(ucPulseNo,ucFeeNo);
//	}
	else
		ClearHisVal(&stHisVal);

	return stHisVal;
}
//反向有功最大需量发生时间(上月)
STHISTIME Get_History_UnDirPowMaxDemandTimeLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	STHISTIME	stHisTime;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisTime=Get_History_UnDirPowMaxDemandTimeLm(ucMeterNo,ucPos,ucFeeNo);
	}
//	else if(ucMPType == CON_MPTYPE_PULSE)
//	{
//		ucPulseNo	= RunPara_GetUnDirPowPulseNo_byMPNo(ucMPNo);
//		stHisTime=Get_History_UnDirPowMaxDemandTime_LM_byPulseNo(ucPulseNo,ucFeeNo);
//	}
	else
		ClearHisTime(&stHisTime);

	return stHisTime;
}
//反向无功最大需量(上月)
STHISVAL Get_History_UnDirVarMaxDemandLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_UnDirVarMaxDemandLm(ucMeterNo,ucPos,ucFeeNo);
	}
//	else if(ucMPType == CON_MPTYPE_PULSE)
//	{
//		ucPulseNo	= RunPara_GetUnDirVarPulseNo_byMPNo(ucMPNo);
//		stHisVal=Get_History_UnDirVarMaxDemand_LM_byPulseNo(ucPulseNo,ucFeeNo);
//	}
	else
		ClearHisVal(&stHisVal);

	return stHisVal;
}
//反向无功最大需量发生时间(上月)
STHISTIME Get_History_UnDirVarMaxDemandTimeLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	STHISTIME	stHisTime;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisTime=Get_History_UnDirVarMaxDemandTimeLm(ucMeterNo,ucPos,ucFeeNo);
	}
//	else if(ucMPType == CON_MPTYPE_PULSE)
//	{
//		ucPulseNo	= RunPara_GetUnDirVarPulseNo_byMPNo(ucMPNo);
//		stHisTime=Get_History_UnDirVarMaxDemandTime_LM_byPulseNo(ucPulseNo,ucFeeNo);
//	}
	else
		ClearHisTime(&stHisTime);

	return stHisTime;
}



//读取测量点的电压值，都统一到一位小数XXX.X
STHISVAL Get_History_V_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo)
{
	unsigned char ucMeterNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_V(ucMeterNo,ucPos,ucNo);
	}
//	else if(ucMPType == CON_MPTYPE_TER)
//	{
////		stHisVal = Get_History_V_Ter(ucPos,ucNo);
//		stHisVal = Get_History_V_Disp_Ter(ucPos,ucNo);
//		if(stHisVal.lVal!=CONINVALIDVAL)
//			stHisVal.lVal /= 10;
//	}
	else
		ClearHisVal(&stHisVal);
	return stHisVal;
}

STHISVAL Get_History_V_Phase_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo)
{
	unsigned char ucMeterNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_V_Phase(ucMeterNo,ucPos,ucNo);
	}
//	else if(ucMPType == CON_MPTYPE_TER)
//	{
////		stHisVal = Get_History_V_Ter(ucPos,ucNo);
//		stHisVal = Get_History_V_Phase_Disp_Ter(ucPos,ucNo);
//	//	if(stHisVal.lVal!=CONINVALIDVAL)
//	}
	else
		ClearHisVal(&stHisVal);
	return stHisVal;
}
//小数.XXX
STHISVAL Get_History_I_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo)
{
	unsigned char ucMeterNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_I(ucMeterNo,ucPos,ucNo);
	}
//	else if(ucMPType == CON_MPTYPE_TER)
//	{
////		stHisVal = Get_History_I_Ter(ucPos,ucNo);
//		stHisVal = Get_History_I_Disp_Ter(ucPos,ucNo);
//		if(stHisVal.lVal!=CONINVALIDVAL)
//			stHisVal.lVal /= 10;
//	}
	else
		ClearHisVal(&stHisVal);
	return stHisVal;
}

STHISVAL Get_History_I_Phase_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo)
{
	unsigned char ucMeterNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_I_Phase(ucMeterNo,ucPos,ucNo);
	}
//	else if(ucMPType == CON_MPTYPE_TER)
//	{
////		stHisVal = Get_History_I_Ter(ucPos,ucNo);
//		stHisVal = Get_History_I_Phase_Disp_Ter(ucPos,ucNo);
//	//	if(stHisVal.lVal!=CONINVALIDVAL)
//	//		stHisVal.lVal /= 100;
//	}
	else
		ClearHisVal(&stHisVal);
	return stHisVal;
}

STHISVAL Get_History_InstantPow_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo)
{
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_InstantPow(ucMeterNo,ucPos,ucNo);
	}
//	else if(ucMPType == CON_MPTYPE_TER)
//	{
//		stHisVal = Get_History_InstantPow_Ter(ucPos,ucNo);
//	}
//	else if(ucMPType == CON_MPTYPE_PULSE)
//	{
//		ucPulseNo	= RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRPOW);
//		stHisVal = Get_History_InstantPow_byPulseNo(ucPulseNo,ucPos,ucNo);
//	}
    else
		ClearHisVal(&stHisVal);
	return stHisVal;
}

STHISVAL Get_History_InstantVar_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo)
{
	unsigned char ucMeterNo;
//	unsigned char ucPulseNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_InstantVar(ucMeterNo,ucPos,ucNo);
	}
//	else if(ucMPType == CON_MPTYPE_TER)
//	{
////		stHisVal = Get_History_InstantVar_Ter(ucPos,ucNo);
//		stHisVal = Get_History_InstantVar_Disp_Ter(ucPos,ucNo);
//		if(stHisVal.lVal!=CONINVALIDVAL)
//			stHisVal.lVal /= 100;		
//	}
//	else if(ucMPType == CON_MPTYPE_PULSE)
//	{
//		ucPulseNo	= RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
//		stHisVal=Get_History_InstantVar_byPulseNo(ucPulseNo,ucPos,ucNo);
//	}
	else
		ClearHisVal(&stHisVal);
	return stHisVal;
}


STHISVAL Get_History_RevInstantPow_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
{
//	unsigned char ucPulseNo;
//	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISVAL	stHisVal;

//	ucMPType	= RunPara_GetMPType(ucMPNo);
//	if(ucMPType == CON_MPTYPE_PULSE)
//	{
//		ucPulseNo	= RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVPOW);
//		stHisVal=Get_History_RevInstantPow_byPulseNo(ucPulseNo,ucPos);
//	}else
		ClearHisVal(&stHisVal);
	return stHisVal;
}

STHISVAL Get_History_RevInstantVar_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
{
//	unsigned char ucPulseNo;
//	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISVAL	stHisVal;

//	ucMPType	= RunPara_GetMPType(ucMPNo);
//	if(ucMPType == CON_MPTYPE_PULSE)
//	{
//		ucPulseNo	= RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVVAR);
//		stHisVal=Get_History_RevInstantVar_byPulseNo(ucPulseNo,ucPos);
//	}else
		ClearHisVal(&stHisVal);
	return stHisVal;
}

//STHISVAL Get_History_AvePow15_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
//{
//	unsigned long ulTimeDiff=0;
//	unsigned char ucMeterNo=0;
//	unsigned char ucMPType = CON_MPTYPE_NULL;
//	STHISVAL	stHisVal1;
//	STHISVAL	stHisVal2;
//	STHISVAL	stHisVal;
//
//	ucMPType	= RunPara_GetMPType(ucMPNo);
//	if(ucMPType ==CON_MPTYPE_METER)
//	{
//		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
//		stHisVal1	= stMeterTimerVal[ucMeterNo].stHisDirPowTotal[0];
//		stHisVal2	= stMeterTimerVal[ucMeterNo].stHisDirPowTotal[1];
//
//		ulTimeDiff	= SubDateTime(&(stHisVal1.stDateTime),&(stHisVal2.stDateTime));
//		if(	stHisVal1.lVal!=CONINVALIDVAL && stHisVal2.lVal!=CONINVALIDVAL &&
//			(ulTimeDiff<17 && ulTimeDiff>13))
//		{
//			stHisVal = stHisVal1;
//			if(stHisVal1.lVal > stHisVal2.lVal)
//				stHisVal.lVal = stHisVal1.lVal-stHisVal2.lVal;
//			else
//				stHisVal.lVal = stHisVal2.lVal-stHisVal1.lVal;
//			stHisVal.lVal = stHisVal.lVal * 400;
//		}
//	}else
//		ClearHisVal(&stHisVal);
//	return stHisVal;
//}
//STHISVAL Get_History_AveVar15_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
//{
//	unsigned long ulTimeDiff=0;
//	unsigned char ucMeterNo=0;
//	unsigned char ucMPType = CON_MPTYPE_NULL;
//	STHISVAL	stHisVal1;
//	STHISVAL	stHisVal2;
//	STHISVAL	stHisVal;
//
//	ucMPType	= RunPara_GetMPType(ucMPNo);
//	if(ucMPType ==CON_MPTYPE_METER)
//	{
//		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
//		stHisVal1	= stMeterTimerVal[ucMeterNo].stHisDirVarTotal[0];
//		stHisVal2	= stMeterTimerVal[ucMeterNo].stHisDirVarTotal[1];
//
//		ulTimeDiff	= SubDateTime(&(stHisVal1.stDateTime),&(stHisVal2.stDateTime));
//		if(	stHisVal1.lVal!=CONINVALIDVAL && stHisVal2.lVal!=CONINVALIDVAL &&
//			(ulTimeDiff<17 && ulTimeDiff>13))
//		{
//			stHisVal = stHisVal1;
//			if(stHisVal1.lVal > stHisVal2.lVal)
//				stHisVal.lVal = stHisVal1.lVal-stHisVal2.lVal;
//			else
//				stHisVal.lVal = stHisVal2.lVal-stHisVal1.lVal;
//			stHisVal.lVal = stHisVal.lVal * 4;
//		}
//	}else
//		ClearHisVal(&stHisVal);
//	return stHisVal;
//}
//STHISVAL Get_History_RevAvePow15_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
//{
//	unsigned long ulTimeDiff=0;
//	unsigned char ucMeterNo=0;
//	unsigned char ucMPType = CON_MPTYPE_NULL;
//	STHISVAL	stHisVal1;
//	STHISVAL	stHisVal2;
//	STHISVAL	stHisVal;
//
//	ucMPType	= RunPara_GetMPType(ucMPNo);
//	if(ucMPType == CON_MPTYPE_METER)
//	{
//		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
//		stHisVal1	= stMeterTimerVal[ucMeterNo].stHisRevPowTotal[0];
//		stHisVal2	= stMeterTimerVal[ucMeterNo].stHisRevPowTotal[1];
//
//		ulTimeDiff	= SubDateTime(&(stHisVal1.stDateTime),&(stHisVal2.stDateTime));
//		if(	stHisVal1.lVal!=CONINVALIDVAL && stHisVal2.lVal!=CONINVALIDVAL &&
//			(ulTimeDiff<17 && ulTimeDiff>13))
//		{
//			stHisVal = stHisVal1;
//			if(stHisVal1.lVal > stHisVal2.lVal)
//				stHisVal.lVal = stHisVal1.lVal-stHisVal2.lVal;
//			else
//				stHisVal.lVal = stHisVal2.lVal-stHisVal1.lVal;
//			stHisVal.lVal = stHisVal.lVal * 400;
//		}
//	}else
//		ClearHisVal(&stHisVal);
//	return stHisVal;
//}
//STHISVAL Get_History_RevAveVar15_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
//{
//	unsigned long ulTimeDiff=0;
//	unsigned char ucMeterNo=0;
//	unsigned char ucMPType = CON_MPTYPE_NULL;
//	STHISVAL	stHisVal1;
//	STHISVAL	stHisVal2;
//	STHISVAL	stHisVal;
//
//	ucMPType	= RunPara_GetMPType(ucMPNo);
//	if(ucMPType ==CON_MPTYPE_METER)
//	{
//		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
//		stHisVal1	= stMeterTimerVal[ucMeterNo].stHisRevVarTotal[0];
//		stHisVal2	= stMeterTimerVal[ucMeterNo].stHisRevVarTotal[1];
//
//		ulTimeDiff	= SubDateTime(&(stHisVal1.stDateTime),&(stHisVal2.stDateTime));
//		if(	stHisVal1.lVal!=CONINVALIDVAL && stHisVal2.lVal!=CONINVALIDVAL &&
//			(ulTimeDiff<17 && ulTimeDiff>13))
//		{
//			stHisVal = stHisVal1;
//			if(stHisVal1.lVal > stHisVal2.lVal)
//				stHisVal.lVal = stHisVal1.lVal-stHisVal2.lVal;
//			else
//				stHisVal.lVal = stHisVal2.lVal-stHisVal1.lVal;
//			stHisVal.lVal = stHisVal.lVal * 4;
//		}
//	}else
//		ClearHisVal(&stHisVal);
//	return stHisVal;
//}
//
////有功、无功脉冲常数
//STHISVAL Get_History_PowPulseConst_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
//{
//	unsigned char ucMeterNo;
//	unsigned char ucMPType = CON_MPTYPE_NULL;
//	STHISVAL	stHisVal;
//
//	ucMPType	= RunPara_GetMPType(ucMPNo);
//	if(ucMPType ==CON_MPTYPE_METER)
//	{
//		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
//		stHisVal=Get_History_PowPulseConst(ucMeterNo,ucPos);
//	}else
//		ClearHisVal(&stHisVal);
//	return stHisVal;
//}
//
//STHISVAL Get_History_VarPulseConst_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
//{
//	unsigned char ucMeterNo;
//	unsigned char ucMPType = CON_MPTYPE_NULL;
//	STHISVAL	stHisVal;
//
//	ucMPType	= RunPara_GetMPType(ucMPNo);
//	if(ucMPType ==CON_MPTYPE_METER)
//	{
//		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
//		stHisVal=Get_History_VarPulseConst(ucMeterNo,ucPos);
//	}else
//		ClearHisVal(&stHisVal);
//	return stHisVal;
//}
//
////费率
//STHISFEERATE Get_History_FeeRate_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
//{
//	unsigned char ucMeterNo;
//	unsigned char ucMPType = CON_MPTYPE_NULL;
//	STHISFEERATE stHisFeeRate;
//
//	ucMPType	= RunPara_GetMPType(ucMPNo);
//	if(ucMPType ==CON_MPTYPE_METER)
//	{
//		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
//		stHisFeeRate=Get_History_FeeRate(ucMeterNo,ucPos);
//	}else
//		ClearHisFeeRate(&stHisFeeRate);
//	return stHisFeeRate;
//}

//编程时间
STHISTIME Get_History_ProgramTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
{
	unsigned char ucMeterNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISTIME	stHisTime;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisTime=Get_History_ProgramTime(ucMeterNo,ucPos);
	}else
		ClearHisTime(&stHisTime);
	return stHisTime;
}

//电表清零时间
STHISTIME Get_History_MeterClrTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
{
	unsigned char ucMeterNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISTIME	stHisTime;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= 	RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisTime	=	Get_History_MeterClrTime(ucMeterNo,ucPos);
	}else
		ClearHisTime(&stHisTime);
	return stHisTime;
}

//需量清零时间
STHISTIME Get_History_DemandClrTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
{
	unsigned char ucMeterNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISTIME	stHisTime;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisTime=Get_History_DemandClrTime(ucMeterNo,ucPos);
	}else
		ClearHisTime(&stHisTime);
	return stHisTime;
}

//事件清零时间
STHISTIME Get_History_EventClrTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
{
	unsigned char ucMeterNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISTIME	stHisTime;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisTime	= Get_History_EventClrTime(ucMeterNo,ucPos);
	}else
		ClearHisTime(&stHisTime);
	return stHisTime;
}

//校时时间
STHISTIME Get_History_JiaoshiTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
{
	unsigned char ucMeterNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISTIME	stHisTime;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisTime	= Get_History_JiaoshiTime(ucMeterNo,ucPos);
	}else
		ClearHisTime(&stHisTime);
	return stHisTime;
}

//编程次数
STHISVAL Get_History_ProgramCount_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
{
	unsigned char ucMeterNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_ProgramCount(ucMeterNo,ucPos);
	}else
		ClearHisVal(&stHisVal);
	return stHisVal;
}

//电表清零次数
STHISVAL Get_History_MeterClrCount_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
{
	unsigned char ucMeterNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal	= Get_History_MeterClrCount(ucMeterNo,ucPos);
	}else
		ClearHisVal(&stHisVal);
	return stHisVal;
}

//需量清零次数
STHISVAL Get_History_DemandClrCount_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
{
	unsigned char ucMeterNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_DemandClrCount(ucMeterNo,ucPos);
	}else
		ClearHisVal(&stHisVal);
	return stHisVal;
}

//事件清零次数
STHISVAL Get_History_EventClrCount_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
{
	unsigned char ucMeterNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal	= Get_History_EventClrCount(ucMeterNo,ucPos);
	}else
		ClearHisVal(&stHisVal);
	return stHisVal;
}

//校时次数
STHISVAL Get_History_JiaoshiCount_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
{
	unsigned char ucMeterNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal	= Get_History_JiaoshiCount(ucMeterNo,ucPos);
	}else
		ClearHisVal(&stHisVal);
	return stHisVal;
}

//电池工作时间
STHISVAL Get_History_BatWorkTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
{
	unsigned char ucMeterNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_BatWorkTime(ucMeterNo,ucPos);
	}else
		ClearHisVal(&stHisVal);
	return stHisVal;
}

//电表运行状态字
STHISSTATE Get_History_MeterStatFlag_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFlagNum)
{
	unsigned char ucMeterNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISSTATE	stHisState;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisState=Get_History_MeterStatFlag(ucMeterNo,ucPos,ucFlagNum);
	}else
		ClearHisState(&stHisState);
	return stHisState;
}

////电网运行状态字
//STHISSTATE Get_History_NetStatFlag_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
//{
//	unsigned char ucMeterNo;
//	unsigned char ucMPType = CON_MPTYPE_NULL;
//	STHISSTATE	stHisState;
//
//	ucMPType	= RunPara_GetMPType(ucMPNo);
//	if(ucMPType ==CON_MPTYPE_METER)
//	{
//		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
//		stHisState=Get_History_NetStatFlag(ucMeterNo,ucPos);
//	}else
//		ClearHisState(&stHisState);
//	return stHisState;
//}
//
////抄表日
//STHISTIME Get_History_CopyDayTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
//{
//	unsigned char ucMeterNo;
//	unsigned char ucMPType = CON_MPTYPE_NULL;
//	STHISTIME	stHisTime;
//
//	ucMPType	= RunPara_GetMPType(ucMPNo);
//	if(ucMPType ==CON_MPTYPE_METER)
//	{
//		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
//		stHisTime=Get_History_CopyDayTime(ucMeterNo,ucPos);
//	}else
//		ClearHisTime(&stHisTime);
//	return stHisTime;
//}

//断相次数
STHISVAL Get_History_VolBreakNum_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo)
{
	unsigned char ucMeterNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_VolBreakNum(ucMeterNo,ucPos,ucNo);
	}else
		ClearHisVal(&stHisVal);
	return stHisVal;
}

//断相时间累计值
STHISVAL Get_History_VolBreakTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo)
{
	unsigned char ucMeterNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISVAL	stHisVal;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisVal=Get_History_VolBreakTime(ucMeterNo,ucPos,ucNo);
	}else
		ClearHisVal(&stHisVal);
	return stHisVal;
}

//最近一次断相起始时刻
STHISTIME Get_History_LastVolBreakBegTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo)
{
	unsigned char ucMeterNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISTIME	stHisTime;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisTime=Get_History_LastVolBreakBegTime(ucMeterNo,ucPos,ucNo);
	}else
		ClearHisTime(&stHisTime);
	return stHisTime;
}


//最近一次断相结束时刻
STHISTIME Get_History_LastVolBreakEndTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo)
{
	unsigned char ucMeterNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISTIME	stHisTime;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisTime=Get_History_LastVolBreakEndTime(ucMeterNo,ucPos,ucNo);
	}else
		ClearHisTime(&stHisTime);
	return stHisTime;
}


//年月日
STHISTIME Get_History_Date_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
{
	unsigned char ucMeterNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISTIME	stHisTime;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisTime=Get_History_Date(ucMeterNo,ucPos);
	}else
		ClearHisTime(&stHisTime);
	return stHisTime;
}

//时分秒
STHISTIME Get_History_Time_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
{
	unsigned char ucMeterNo;
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISTIME	stHisTime;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_METER)
	{
		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisTime=Get_History_Time(ucMeterNo,ucPos);
	}else
		ClearHisTime(&stHisTime);
	return stHisTime;
}

//门节点状态
//STHISSTATE Get_History_DoorStat_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
//{
////	unsigned char ucMPType = CON_MPTYPE_NULL;
//	STHISSTATE	stHisState;
//
////	ucMPType	= RunPara_GetMPType(ucMPNo);
////	if(ucMPType ==CON_MPTYPE_TER)
////		stHisState=Get_History_DoorStat_Ter(ucPos);
////	else
//		ClearHisState(&stHisState);
//	return stHisState;
//}

//终端停上电状态
STHISSTATE Get_History_PowerStat_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
{
	unsigned char ucMPType = CON_MPTYPE_NULL;
	STHISSTATE	stHisState;

	ucMPType	= RunPara_GetMPType(ucMPNo);
	if(ucMPType ==CON_MPTYPE_TER)
		stHisState=Get_History_PowerStat_Ter(ucPos);
	else
		ClearHisState(&stHisState);
	return stHisState;
}

////开关状态
//STHISSTATE Get_History_SwitchStat_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucSwitchNo)
//{
////	unsigned char ucMPType = CON_MPTYPE_NULL;
//	STHISSTATE	stHisState;
//
////	ucMPType	= RunPara_GetMPType(ucMPNo);
////	if(ucMPType ==CON_MPTYPE_TER)
////		stHisState=Get_History_SwitchStat_Ter(ucPos,ucSwitchNo);
////	else
//		ClearHisState(&stHisState);
//	return stHisState;
//}

//STHISSTATE Get_History_PolReverseStat_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo)
//{
////	unsigned char ucMPType = CON_MPTYPE_NULL;
//	STHISSTATE	stHisState;
//
////	ucMPType	= RunPara_GetMPType(ucMPNo);
////	if(ucMPType ==CON_MPTYPE_TER)
////		stHisState=Get_History_PolReverseStat_Ter(ucPos,ucNo);
////	else
//		ClearHisState(&stHisState);
//	return stHisState;
//}
//STHISSTATE Get_History_ShortCir1Stat_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo)
//{
////	unsigned char ucMPType = CON_MPTYPE_NULL;
//	STHISSTATE	stHisState;
//
////	ucMPType	= RunPara_GetMPType(ucMPNo);
////	if(ucMPType ==CON_MPTYPE_TER)
////		stHisState=Get_History_ShortCir1Stat_Ter(ucPos,ucNo);
////	else
//		ClearHisState(&stHisState);
//	return stHisState;
//}
//
//STHISSTATE Get_History_ShortCir2Stat_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo)
//{
////	unsigned char ucMPType = CON_MPTYPE_NULL;
//	STHISSTATE	stHisState;
//
////	ucMPType	= RunPara_GetMPType(ucMPNo);
////	if(ucMPType ==CON_MPTYPE_TER)
////		stHisState=Get_History_ShortCir2Stat_Ter(ucPos,ucNo);
////	else
//		ClearHisState(&stHisState);
//	return stHisState;
//}


//STHISSTATE Get_History_OpenCir2Stat_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo)
//{
////	unsigned char ucMPType = CON_MPTYPE_NULL;
//	STHISSTATE	stHisState;
//
////	ucMPType	= RunPara_GetMPType(ucMPNo);
////	if(ucMPType ==CON_MPTYPE_TER)
////		stHisState=Get_History_OpenCir2Stat_Ter(ucPos,ucNo);
////	else
//		ClearHisState(&stHisState);
//	return stHisState;
//}


//STHISSTATE Get_History_VolCirReveStat_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
//{
////	unsigned char ucMPType = CON_MPTYPE_NULL;
//	STHISSTATE	stHisState;
//
////	ucMPType	= RunPara_GetMPType(ucMPNo);
////	if(ucMPType ==CON_MPTYPE_TER)
////		stHisState=Get_History_VolCirReveStat_Ter(ucPos);
////	else
//		ClearHisState(&stHisState);
//	return stHisState;
//}

//得到电压值，数据为小数点后两位XXXXXX.XX
//STHISVAL Get_History_V_Disp_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo)
//{
//	unsigned char ucMeterNo;
//	unsigned char ucMPType = CON_MPTYPE_NULL;
//	STHISVAL	stHisVal;
//
//	ucMPType	= RunPara_GetMPType(ucMPNo);
//	if(ucMPType ==CON_MPTYPE_METER)
//	{
//		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
//		stHisVal=Get_History_V(ucMeterNo,ucPos,ucNo);
//		if(stHisVal.lVal!=CONINVALIDVAL)
//			stHisVal.lVal = stHisVal.lVal * 10;
//	}
////	else if(ucMPType == CON_MPTYPE_TER)
////	{
////		stHisVal = Get_History_V_Disp_Ter(ucPos,ucNo);
////	}
//	else
//		ClearHisVal(&stHisVal);
//	return stHisVal;
//}
////得到电流值，数据格式XXXX.XXXX
//STHISVAL Get_History_I_Disp_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo)
//{
//	unsigned char ucMeterNo;
//	unsigned char ucMPType = CON_MPTYPE_NULL;
//	STHISVAL	stHisVal;
//
//	ucMPType	= RunPara_GetMPType(ucMPNo);
//	if(ucMPType ==CON_MPTYPE_METER)
//	{
//		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
//		stHisVal=Get_History_I(ucMeterNo,ucPos,ucNo);
//		if(stHisVal.lVal!=CONINVALIDVAL)
//			stHisVal.lVal = stHisVal.lVal * 10;
//	}
////	else if(ucMPType == CON_MPTYPE_TER)
////	{
////		stHisVal = Get_History_I_Disp_Ter(ucPos,ucNo);
////	}
//	else
//		ClearHisVal(&stHisVal);
//	return stHisVal;
//}
//STHISVAL Get_History_InstantVar_Disp_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo)
//{
//	unsigned char ucMeterNo;
////	unsigned char ucPulseNo;
//	unsigned char ucMPType = CON_MPTYPE_NULL;
//	STHISVAL	stHisVal;
//
//	ucMPType	= RunPara_GetMPType(ucMPNo);
//	if(ucMPType ==CON_MPTYPE_METER)
//	{
//		ucMeterNo	= RunPara_GetMeterNo_byMPNo(ucMPNo);
//		stHisVal=Get_History_InstantVar(ucMeterNo,ucPos,ucNo);
//		if(stHisVal.lVal!=CONINVALIDVAL)
//			stHisVal.lVal = stHisVal.lVal * 100;
//	}
////	else if(ucMPType == CON_MPTYPE_TER)
////	{
////		stHisVal = Get_History_InstantVar_Disp_Ter(ucPos,ucNo);
////	}
////	else if(ucMPType == CON_MPTYPE_PULSE)
////	{
////		ucPulseNo = RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
////		stHisVal = Get_History_InstantVar_byPulseNo(ucPulseNo,ucPos,ucNo);
////		if(stHisVal.lVal!=CONINVALIDVAL)
////			stHisVal.lVal = stHisVal.lVal * 100;
////	}
//	else
//		ClearHisVal(&stHisVal);
//	return stHisVal;
//}


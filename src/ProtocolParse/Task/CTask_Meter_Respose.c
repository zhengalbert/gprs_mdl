/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : CTask_Meter_Respose.c
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-08-15    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

STRS485TASK	  stFillInTask;

//有效数据
static unsigned char Fill_GRTaskData_DirPower(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel);
static unsigned char Fill_GRTaskData_UnDirPower(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel);
static unsigned char Fill_GRTaskData_DirVar(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel);
static unsigned char Fill_GRTaskData_UnDirVar(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel);
//static unsigned char Fill_GRTaskData_DirPower_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel);
//static unsigned char Fill_GRTaskData_UnDirPower_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel);
static unsigned char Fill_GRTaskData_Vc(unsigned char ucMPNo,unsigned char ucTaskSour);
static unsigned char Fill_GRTaskData_Vb(unsigned char ucMPNo,unsigned char ucTaskSour);
static unsigned char Fill_GRTaskData_Va(unsigned char ucMPNo,unsigned char ucTaskSour);
static unsigned char Fill_GRTaskData_Ic(unsigned char ucMPNo,unsigned char ucTaskSour);
static unsigned char Fill_GRTaskData_Ib(unsigned char ucMPNo,unsigned char ucTaskSour);
static unsigned char Fill_GRTaskData_Ia(unsigned char ucMPNo,unsigned char ucTaskSour);
static unsigned char Fill_GRTaskData_DirVar1(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usItemLable);
static unsigned char Fill_GRTaskData_DirVar2(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usItemLable);
static unsigned char Fill_GRTaskData_DirVar3(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usItemLable);
static unsigned char Fill_GRTaskData_DirVar4(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usItemLable);
//static unsigned char Fill_GRTaskData_DirVar_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel);
//static unsigned char Fill_GRTaskData_UnDirVar_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel);
//static unsigned char Fill_GRTaskData_DirVar1_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usItemLable);
//static unsigned char Fill_GRTaskData_DirVar2_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usItemLable);
//static unsigned char Fill_GRTaskData_DirVar3_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usItemLable);
//static unsigned char Fill_GRTaskData_DirVar4_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usItemLable);
static unsigned char Fill_GRTaskData_InstantPow(unsigned char ucMPNo,unsigned char ucTaskSour);
static unsigned char Fill_GRTaskData_InstantPowA(unsigned char ucMPNo,unsigned char ucTaskSour);
static unsigned char Fill_GRTaskData_InstantPowB(unsigned char ucMPNo,unsigned char ucTaskSour);
static unsigned char Fill_GRTaskData_InstantPowC(unsigned char ucMPNo,unsigned char ucTaskSour);
static unsigned char Fill_GRTaskData_InstantVar(unsigned char ucMPNo,unsigned char ucTaskSour);
static unsigned char Fill_GRTaskData_InstantVarA(unsigned char ucMPNo,unsigned char ucTaskSour);
static unsigned char Fill_GRTaskData_InstantVarB(unsigned char ucMPNo,unsigned char ucTaskSour);
static unsigned char Fill_GRTaskData_InstantVarC(unsigned char ucMPNo,unsigned char ucTaskSour);

//static unsigned char Fill_GRTaskData_DirPowMaxDemand(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel);
//static unsigned char Fill_GRTaskData_DirPowMaxDemandTime(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel);
//static unsigned char Fill_GRTaskData_UnDirPowMaxDemand(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel);
//static unsigned char Fill_GRTaskData_UnDirPowMaxDemandTime(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel);
//
//static unsigned char Fill_GRTaskData_DirPowMaxDemand_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel);
//static unsigned char Fill_GRTaskData_DirPowMaxDemandTime_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel);
//static unsigned char Fill_GRTaskData_UnDirPowMaxDemand_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel);
//static unsigned char Fill_GRTaskData_UnDirPowMaxDemandTime_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel);
//static unsigned char Fill_GRTaskData_PowConst(unsigned char ucMPNo,unsigned char ucTaskSour);
//static unsigned char Fill_GRTaskData_VarConst(unsigned char ucMPNo,unsigned char ucTaskSour);
//static unsigned char Fill_GRTaskData_FeeRate(unsigned char ucMPNo,unsigned char ucTaskSour);
//static unsigned char Fill_GRTaskData_SumInstantPow(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned char ucGroupNo);
//static unsigned char Fill_GRTaskData_SumInstantVar(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned char ucGroupNo);
//static unsigned char Fill_GRTaskData_SumPow(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned char ucGroupNo);
//static unsigned char Fill_GRTaskData_SumVar(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned char ucGroupNo);
//无效数据
//static unsigned char Fill_GRTaskData_DirPower_Invalid(void);
//static unsigned char Fill_GRTaskData_UnDirPower_Invalid(void);
//static unsigned char Fill_GRTaskData_DirVar_Invalid(void);
//static unsigned char Fill_GRTaskData_UnDirVar_Invalid(void);
//static unsigned char Fill_GRTaskData_I_Invalid(unsigned short usDI);
//static unsigned char Fill_GRTaskData_V_Invalid(unsigned short usDI);
//static unsigned char Fill_GRTaskData_InstantVal_Invalid(void);//专为浙江规约
//static unsigned char Fill_GRTaskData_DirVar1_Invalid(void);
//static unsigned char Fill_GRTaskData_DirVar2_Invalid(void);
//static unsigned char Fill_GRTaskData_DirVar3_Invalid(void);
//static unsigned char Fill_GRTaskData_DirVar4_Invalid(void);
//static unsigned char Fill_GRTaskData_InstantPow_Invalid(unsigned short usDI);
//static unsigned char Fill_GRTaskData_InstantVar_Invalid(unsigned short usDI);
//static unsigned char Fill_GRTaskData_DiraPowMaxDemand_Invalid(void);
//static unsigned char Fill_GRTaskData_DiraPowMaxDemandLM_Invalid(void);
//static unsigned char Fill_GRTaskData_DiraPowMaxDemandTime_Invalid(void);
//static unsigned char Fill_GRTaskData_DiraPowMaxDemandTimeLM_Invalid(void);
//static unsigned char Fill_GRTaskData_UnDiraPowMaxDemand_Invalid(void);
//static unsigned char Fill_GRTaskData_UnDiraPowMaxDemandLM_Invalid(void);
//static unsigned char Fill_GRTaskData_UnDiraPowMaxDemandTime_Invalid(void);
//static unsigned char Fill_GRTaskData_UnDiraPowMaxDemandTimeLM_Invalid(void);
//static unsigned char Fill_GRTaskData_DirVar1LM_Invalid(void);
//static unsigned char Fill_GRTaskData_DirVar2LM_Invalid(void);
//static unsigned char Fill_GRTaskData_DirVar3LM_Invalid(void);
//static unsigned char Fill_GRTaskData_DirVar4LM_Invalid(void);
//static unsigned char Fill_GRTaskData_DirPowerLM_Invalid(void);
//static unsigned char Fill_GRTaskData_UnDirPowerLM_Invalid(void);
//static unsigned char Fill_GRTaskData_DirVarLM_Invalid(void);
//static unsigned char Fill_GRTaskData_UnDirVarLM_Invalid(void);
//static unsigned char Fill_GRTaskData_Const_Invalid(unsigned short usDI);
//static unsigned char Fill_GRTaskData_FeeRate_Invalid(void);
static unsigned char Fill_GRTaskData_Val(unsigned char ucDINo,unsigned short usDI_Sml,unsigned short usDI_Big,unsigned char ucSml_l,unsigned char ucBig_l,unsigned char ucValLen);
//static unsigned char Fill_GRTaskData_Val_Direct(unsigned char ucDINo,unsigned short usDI_Sml,unsigned short usDI_Big,unsigned char ucSml_l,unsigned char ucBig_l,unsigned char ucValLen);
static unsigned char Set_FillInTask(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel,unsigned char *pucDIContent,STDATETIME stDateTime);
static unsigned short GetGlobeRecvBufPtr(unsigned ucIndex);
/*********************************************************************************/
unsigned char Fill_TaskData_Manage_Invalid(unsigned short usDILabel,unsigned char ucTaskSour,unsigned char ucMPNo)
{
	unsigned char ucRet=RET_SUCCESS;
//azh 由于曲线任务数据直接从全局数据组里获取数据 都没有去实时抄表 所以不需要填无效数据 再说初始化时已经默认填成无效数据
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPDIRPOW_BLOCK:	//正向有功电量
//		Fill_GRTaskData_DirPower_Invalid();
//		break;
//	case CON_DI_TYPE_MPUNDIRPOW_BLOCK:	//反向有功电量
//		Fill_GRTaskData_UnDirPower_Invalid();
//		break;
//	case CON_DI_TYPE_MPDIRPOW_LM_BLOCK://正向有功电能数据块（上月）
//		Fill_GRTaskData_DirPowerLM_Invalid();
//		break;
//	case CON_DI_TYPE_MPUNDIRPOW_LM_BLOCK://反向有功电能数据块（上月）
//		Fill_GRTaskData_UnDirPowerLM_Invalid();
//		break;
//	case CON_DI_TYPE_MPDIRVAR_BLOCK:	//正向有功电量
//		Fill_GRTaskData_DirVar_Invalid();
//		break;
//	case CON_DI_TYPE_MPUNDIRVAR_BLOCK:	//反向有功电量
//		Fill_GRTaskData_UnDirVar_Invalid();
//		break;
///////////////////////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPVAR1_BLOCK:	//一象限
//		Fill_GRTaskData_DirVar1_Invalid();
//		break;
//	case CON_DI_TYPE_MPVAR4_BLOCK:	//四象限
//		Fill_GRTaskData_DirVar4_Invalid();	
//		break;
//	case CON_DI_TYPE_MPVAR2_BLOCK:	//二象限
//		Fill_GRTaskData_DirVar2_Invalid();
//		break;
//	case CON_DI_TYPE_MPVAR3_BLOCK:	//三象限
//		Fill_GRTaskData_DirVar3_Invalid();
//		break;
//
//	case CON_DI_TYPE_MPDIRVAR_LM_BLOCK://正向无功电能数据块（上月）
//		Fill_GRTaskData_DirVarLM_Invalid();
//		break;
//	case CON_DI_TYPE_MPUNDIRVAR_LM_BLOCK://反向无功电能数据块（上月）
//		Fill_GRTaskData_UnDirVarLM_Invalid();
//		break;
//	case CON_DI_TYPE_MPVAR1_LM_BLOCK://一象限无功总电能（上月）
//		Fill_GRTaskData_DirVar1LM_Invalid();
//		break;
//	case CON_DI_TYPE_MPVAR2_LM_BLOCK://二象限无功总电能（上月）
//		Fill_GRTaskData_DirVar2LM_Invalid();
//		break;
//	case CON_DI_TYPE_MPVAR3_LM_BLOCK://三象限无功总电能（上月）
//		Fill_GRTaskData_DirVar3LM_Invalid();
//		break;
//	case CON_DI_TYPE_MPVAR4_LM_BLOCK://四象限无功总电能（上月）
//		Fill_GRTaskData_DirVar4LM_Invalid();
//		break;
////////////////////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPVOL_BLOCK:
//		break;
//		//下面的处理只对全国规约有用
//	case CON_DI_TYPE_MPCURA:			//A电流
//	case CON_DI_TYPE_MPCURB:			//B电流
//	case CON_DI_TYPE_MPCURC:			//C电流
//		Fill_GRTaskData_I_Invalid(usDILabel);
//		break;
//	case CON_DI_TYPE_MPVOLA:			//A电压
//	case CON_DI_TYPE_MPVOLB:			//B电压
//	case CON_DI_TYPE_MPVOLC:			//C电压
//		Fill_GRTaskData_V_Invalid(usDILabel);
//		break;
//	case CON_DI_TYPE_MPINSTANTPOW:		//瞬时有功功率
//	case CON_DI_TYPE_MPINSTANTPOWA:		//A相有功功率
//	case CON_DI_TYPE_MPINSTANTPOWB:		//B相有功功率
//	case CON_DI_TYPE_MPINSTANTPOWC:		//C相有功功率
//		Fill_GRTaskData_InstantPow_Invalid(usDILabel);
//		break;
//	case CON_DI_TYPE_MPINSTANTVAR:		//瞬时无功功率
//	case CON_DI_TYPE_MPINSTANTVARA:		//A相无功功率
//	case CON_DI_TYPE_MPINSTANTVARB:		//B相无功功率
//	case CON_DI_TYPE_MPINSTANTVARC:		//C相无功功率
//		Fill_GRTaskData_InstantVar_Invalid(usDILabel);
//		break;
//////////////////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_BLOCK:			//正向有功总最大需量
//		Fill_GRTaskData_DiraPowMaxDemand_Invalid();
//		break;
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_BLOCK:		//正向有功总最大需量（上月）
//		Fill_GRTaskData_DiraPowMaxDemandLM_Invalid();
//		break;
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_BLOCK:		//正向有功总最大需量发生时间
//		Fill_GRTaskData_DiraPowMaxDemandTime_Invalid();
//		break;
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_BLOCK:	//正向有功总最大需量发生时间（上月）
//		Fill_GRTaskData_DiraPowMaxDemandTimeLM_Invalid();
//		break;
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_BLOCK:			//反向有功总最大需量
//		Fill_GRTaskData_UnDiraPowMaxDemand_Invalid();
//		break;
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_BLOCK:		//反向有功总最大需量（上月）
//		Fill_GRTaskData_UnDiraPowMaxDemandLM_Invalid();
//		break;
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_BLOCK:		//反向有功总最大需量发生时间
//		Fill_GRTaskData_UnDiraPowMaxDemandTime_Invalid();
//		break;
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_BLOCK:	//反向有功总最大需量发生时间（上月）
//		Fill_GRTaskData_UnDiraPowMaxDemandTimeLM_Invalid();
//		break;
///////////////////////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPPOWCONST:						//有功常数
//	case CON_DI_TYPE_MPVARCONST:						//无功常数
//		Fill_GRTaskData_Const_Invalid(usDILabel);
//		break;
//	case CON_DI_TYPE_MPFEERATE_BLOCK://费率	
//		Fill_GRTaskData_FeeRate_Invalid();
//		break;
//	default:
//		ucRet=RET_ERROR;
//		break;
//	}
	return ucRet;
}

/*******************************有效数据填写******************************************/
unsigned char Fill_TaskData_Manage(unsigned short usDILabel,unsigned char ucTaskSour,unsigned char ucMPNo)
{
	unsigned char	ucRet=RET_SUCCESS;
//	STDATETIME		stDateTime;

//	stDateTime	= GetCurrentTime();
	switch(usDILabel)
	{
	case	CON_DI_TYPE_MPDIRPOWTOTAL:		//正向有功总电量处理
	case	CON_DI_TYPE_MPDIRPOWPLUS:		//正向有功尖电量处理
	case	CON_DI_TYPE_MPDIRPOWAPEX:		//正向有功峰电量处理
	case	CON_DI_TYPE_MPDIRPOWFLAT:		//正向有功平电量处理
	case	CON_DI_TYPE_MPDIRPOWVALE:		//正向有功谷电量处理
		ucRet=Fill_GRTaskData_DirPower(ucMPNo,ucTaskSour,usDILabel);
		break;
		////////////////////////////////////////////////////////
	case	CON_DI_TYPE_MPDIRVARTOTAL:		//正向无功总电量处理
	case	CON_DI_TYPE_MPDIRVARPLUS:		//正向无功尖电量处理
	case	CON_DI_TYPE_MPDIRVARAPEX:		//正向无功峰电量处理
	case	CON_DI_TYPE_MPDIRVARFLAT:		//正向无功平电量处理
	case	CON_DI_TYPE_MPDIRVARVALE:		//正向无功谷电量处理
		ucRet=Fill_GRTaskData_DirVar(ucMPNo,ucTaskSour,usDILabel);
		break;
		/////////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPDIRPOWTOTAL_LM:		//正向有功总电能（上月）
//	case CON_DI_TYPE_MPDIRPOWPLUS_LM:		//正向有功尖电能（上月）
//	case CON_DI_TYPE_MPDIRPOWAPEX_LM:		//正向有功峰电能（上月）
//	case CON_DI_TYPE_MPDIRPOWFLAT_LM:		//正向有功平电能（上月）
//	case CON_DI_TYPE_MPDIRPOWVALE_LM:		//正向有功谷电能（上月）
//		ucRet=Fill_GRTaskData_DirPower_Lm(ucMPNo,ucTaskSour,usDILabel);
//		break;
		/////////////////////////////////////////////////////////
	case	CON_DI_TYPE_MPUNDIRPOWTOTAL:	//反向有功总电量处理
	case CON_DI_TYPE_MPUNDIRPOWPLUS:		//反向有功尖电量处理
	case CON_DI_TYPE_MPUNDIRPOWAPEX:		//反向有功峰电量处理
	case CON_DI_TYPE_MPUNDIRPOWFLAT:		//反向有功平电量处理
	case CON_DI_TYPE_MPUNDIRPOWVALE:		//反向有功谷电量处理
		ucRet=Fill_GRTaskData_UnDirPower(ucMPNo,ucTaskSour,usDILabel);
		break;
		///////////////////////////////////////////////////////
	case	CON_DI_TYPE_MPUNDIRVARTOTAL:	//反向无功总电量处理
	case CON_DI_TYPE_MPUNDIRVARPLUS:		//反向无功尖电量处理
	case CON_DI_TYPE_MPUNDIRVARAPEX:		//反向无功峰电量处理
	case CON_DI_TYPE_MPUNDIRVARFLAT:		//反向无功平电量处理
	case CON_DI_TYPE_MPUNDIRVARVALE:		//反向无功谷电量处理
		ucRet=Fill_GRTaskData_UnDirVar(ucMPNo,ucTaskSour,usDILabel);
		break;
		////////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPUNDIRPOWTOTAL_LM:	//反向有功总电能（上月）
//	case CON_DI_TYPE_MPUNDIRPOWPLUS_LM:		//反向有功尖电能（上月）
//	case CON_DI_TYPE_MPUNDIRPOWAPEX_LM:		//反向有功峰电能（上月）
//	case CON_DI_TYPE_MPUNDIRPOWFLAT_LM:		//反向有功平电能（上月）
//	case CON_DI_TYPE_MPUNDIRPOWVALE_LM:		//反向有功谷电能（上月）	
//		ucRet=Fill_GRTaskData_UnDirPower_Lm(ucMPNo,ucTaskSour,usDILabel);
//		break;
		////////////////////////////////////////////////////////
	case	CON_DI_TYPE_MPVAR1:				//1象限无功处理
	case CON_DI_TYPE_MPVAR1_FEE1:
	case CON_DI_TYPE_MPVAR1_FEE2:
	case CON_DI_TYPE_MPVAR1_FEE3:
	case CON_DI_TYPE_MPVAR1_FEE4:
		ucRet=Fill_GRTaskData_DirVar1(ucMPNo,ucTaskSour,usDILabel);
		break;
	case	CON_DI_TYPE_MPVAR2:				//2象限无功处理
	case	CON_DI_TYPE_MPVAR2_FEE1:		
	case	CON_DI_TYPE_MPVAR2_FEE2:		
	case	CON_DI_TYPE_MPVAR2_FEE3:		
	case	CON_DI_TYPE_MPVAR2_FEE4:		
		ucRet=Fill_GRTaskData_DirVar2(ucMPNo,ucTaskSour,usDILabel);
		break;
	case	CON_DI_TYPE_MPVAR3:				//3象限无功处理
	case	CON_DI_TYPE_MPVAR3_FEE1:		
	case	CON_DI_TYPE_MPVAR3_FEE2:		
	case	CON_DI_TYPE_MPVAR3_FEE3:		
	case	CON_DI_TYPE_MPVAR3_FEE4:		
		ucRet=Fill_GRTaskData_DirVar3(ucMPNo,ucTaskSour,usDILabel);
		break;
	case	CON_DI_TYPE_MPVAR4:				//4象限无功处理
	case	CON_DI_TYPE_MPVAR4_FEE1:		
	case	CON_DI_TYPE_MPVAR4_FEE2:		
	case	CON_DI_TYPE_MPVAR4_FEE3:		
	case	CON_DI_TYPE_MPVAR4_FEE4:		
		ucRet=Fill_GRTaskData_DirVar4(ucMPNo,ucTaskSour,usDILabel);
		break;
		/////////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPDIRVARTOTAL_LM:		//正向无功总电能（上月）
//	case CON_DI_TYPE_MPDIRVARPLUS_LM:		//正向无功尖电能（上月）
//	case CON_DI_TYPE_MPDIRVARAPEX_LM:		//正向无功峰电能（上月）
//	case CON_DI_TYPE_MPDIRVARFLAT_LM:		//正向无功平电能（上月）
//	case CON_DI_TYPE_MPDIRVARVALE_LM:		//正向无功谷电能（上月）
//		ucRet=Fill_GRTaskData_DirVar_Lm(ucMPNo,ucTaskSour,usDILabel);
//		break;
		//////////////////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPUNDIRVARTOTAL_LM:	//反向无功总电能（上月）
//	case CON_DI_TYPE_MPUNDIRVARPLUS_LM:		//反向无功尖电能（上月）
//	case CON_DI_TYPE_MPUNDIRVARAPEX_LM:		//反向无功峰电能（上月）
//	case CON_DI_TYPE_MPUNDIRVARFLAT_LM:		//反向无功平电能（上月）
//	case CON_DI_TYPE_MPUNDIRVARVALE_LM:		//反向无功谷电能（上月）	
//		ucRet=Fill_GRTaskData_UnDirVar_Lm(ucMPNo,ucTaskSour,usDILabel);
//		break;
		////////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPVAR1_LM:				//一象限无功总电能（上月）
//	case CON_DI_TYPE_MPVAR1_LM_FEE1:
//	case CON_DI_TYPE_MPVAR1_LM_FEE2:
//	case CON_DI_TYPE_MPVAR1_LM_FEE3:
//	case CON_DI_TYPE_MPVAR1_LM_FEE4:
//		ucRet=Fill_GRTaskData_DirVar1_Lm(ucMPNo,ucTaskSour,usDILabel);
//		break;
//	case CON_DI_TYPE_MPVAR4_LM:				//四象限无功总电能（上月）
//	case CON_DI_TYPE_MPVAR4_LM_FEE1:
//	case CON_DI_TYPE_MPVAR4_LM_FEE2:
//	case CON_DI_TYPE_MPVAR4_LM_FEE3:
//	case CON_DI_TYPE_MPVAR4_LM_FEE4:
//		ucRet=Fill_GRTaskData_DirVar4_Lm(ucMPNo,ucTaskSour,usDILabel);
//		break;
//	case CON_DI_TYPE_MPVAR2_LM:				//二象限无功总电能（上月）
//	case CON_DI_TYPE_MPVAR2_LM_FEE1:
//	case CON_DI_TYPE_MPVAR2_LM_FEE2:
//	case CON_DI_TYPE_MPVAR2_LM_FEE3:
//	case CON_DI_TYPE_MPVAR2_LM_FEE4:
//		ucRet=Fill_GRTaskData_DirVar2_Lm(ucMPNo,ucTaskSour,usDILabel);
//		break;
//	case CON_DI_TYPE_MPVAR3_LM:				//三象限无功总电能（上月）
//	case CON_DI_TYPE_MPVAR3_LM_FEE1:
//	case CON_DI_TYPE_MPVAR3_LM_FEE2:
//	case CON_DI_TYPE_MPVAR3_LM_FEE3:
//	case CON_DI_TYPE_MPVAR3_LM_FEE4:
//		ucRet=Fill_GRTaskData_DirVar3_Lm(ucMPNo,ucTaskSour,usDILabel);
//		break;
//		//////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND:		//正向有功总最大需量
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE1:
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE2:
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE3:
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE4:
//		ucRet=Fill_GRTaskData_DirPowMaxDemand(ucMPNo,ucTaskSour,usDILabel);
//		break;
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME:	//正向有功总最大需量发生时间
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE1:
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE2:
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE3:
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE4:
//		ucRet=Fill_GRTaskData_DirPowMaxDemandTime(ucMPNo,ucTaskSour,usDILabel);
//		break;
//		//////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND:	//反向有功总最大需量
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE1:
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE2:
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE3:
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE4:
//		ucRet=Fill_GRTaskData_UnDirPowMaxDemand(ucMPNo,ucTaskSour,usDILabel);
//		break;
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME://反向有功总最大需量发生时间
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE1:
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE2:
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE3:
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE4:
//		ucRet=Fill_GRTaskData_UnDirPowMaxDemandTime(ucMPNo,ucTaskSour,usDILabel);
//		break;
//		//////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM://正向有功总最大需量（上月）
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE1:
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE2:
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE3:
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE4:
//		ucRet=Fill_GRTaskData_DirPowMaxDemand_Lm(ucMPNo,ucTaskSour,usDILabel);
//		break;
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM://正向有功总最大需量发生时间（上月）
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE1:
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE2:
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE3:
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE4:
//		ucRet=Fill_GRTaskData_DirPowMaxDemandTime_Lm(ucMPNo,ucTaskSour,usDILabel);
//		break;
//		//////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM://反向有功总最大需量（上月）
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE1:
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE2:
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE3:
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE4:
//		ucRet=Fill_GRTaskData_UnDirPowMaxDemand_Lm(ucMPNo,ucTaskSour,usDILabel);
//		break;
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM://反向有功总最大需量发生时间（上月）
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE1:
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE2:
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE3:
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE4:
//		ucRet=Fill_GRTaskData_UnDirPowMaxDemandTime_Lm(ucMPNo,ucTaskSour,usDILabel);
//		break;
		//////////////////////////////////////////////////////////////////
	case	CON_DI_TYPE_MPCURA:				//A相电流
		ucRet=Fill_GRTaskData_Ia(ucMPNo,ucTaskSour);
		break;
	case	CON_DI_TYPE_MPCURB:				//B相电流
		ucRet=Fill_GRTaskData_Ib(ucMPNo,ucTaskSour);
		break;
	case	CON_DI_TYPE_MPCURC:				//C相电流
		ucRet=Fill_GRTaskData_Ic(ucMPNo,ucTaskSour);
		break;
	case	CON_DI_TYPE_MPVOLA:				//A相电压
		ucRet=Fill_GRTaskData_Va(ucMPNo,ucTaskSour);
		break;
	case	CON_DI_TYPE_MPVOLB:				//B相电压
		ucRet=Fill_GRTaskData_Vb(ucMPNo,ucTaskSour);
		break;
	case	CON_DI_TYPE_MPVOLC:				//C相电压
		ucRet=Fill_GRTaskData_Vc(ucMPNo,ucTaskSour);
		break;
		//////////////////////////////////////////////////////////
	case CON_DI_TYPE_MPINSTANTPOW:		//瞬时有功功率
		ucRet=Fill_GRTaskData_InstantPow(ucMPNo,ucTaskSour);
		break;
	case CON_DI_TYPE_MPINSTANTPOWA:		//A相有功功率
		ucRet=Fill_GRTaskData_InstantPowA(ucMPNo,ucTaskSour);
		break;
	case CON_DI_TYPE_MPINSTANTPOWB:		//B相有功功率
		ucRet=Fill_GRTaskData_InstantPowB(ucMPNo,ucTaskSour);
		break;
	case CON_DI_TYPE_MPINSTANTPOWC:		//C相有功功率
		ucRet=Fill_GRTaskData_InstantPowC(ucMPNo,ucTaskSour);
		break;
	case CON_DI_TYPE_MPINSTANTVAR:		//瞬时无功功率
		ucRet=Fill_GRTaskData_InstantVar(ucMPNo,ucTaskSour);
		break;
	case CON_DI_TYPE_MPINSTANTVARA:		//A相无功功率
		ucRet=Fill_GRTaskData_InstantVarA(ucMPNo,ucTaskSour);
		break;
	case CON_DI_TYPE_MPINSTANTVARB:		//B相无功功率
		ucRet=Fill_GRTaskData_InstantVarB(ucMPNo,ucTaskSour);
		break;
	case CON_DI_TYPE_MPINSTANTVARC:		//C相无功功率
		ucRet=Fill_GRTaskData_InstantVarC(ucMPNo,ucTaskSour);
		break;
		////////////////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPPOWCONST:		//有功常数
//		ucRet=Fill_GRTaskData_PowConst(ucMPNo,ucTaskSour);
//		break;
//	case CON_DI_TYPE_MPVARCONST:		//无功常数
//		ucRet=Fill_GRTaskData_VarConst(ucMPNo,ucTaskSour);
//		break;
//	case CON_DI_TYPE_MPFEERATE_BLOCK:	//费率数据块
//		ucRet=Fill_GRTaskData_FeeRate(ucMPNo,ucTaskSour);
//		break;
//	case CON_DI_TYPE_SUMINSTANTPOW1:						//0x8F50//有功功率总加组1
//		ucRet=Fill_GRTaskData_SumInstantPow(ucMPNo,ucTaskSour,0);
//		break;
//	case CON_DI_TYPE_SUMINSTANTPOW2:						//0x8F51//有功功率总加组2
//		ucRet=Fill_GRTaskData_SumInstantPow(ucMPNo,ucTaskSour,1);
//		break;
//	case CON_DI_TYPE_SUMINSTANTPOW3:						//0x8F52//有功功率总加组3
//		ucRet=Fill_GRTaskData_SumInstantPow(ucMPNo,ucTaskSour,2);
//		break;
//	case CON_DI_TYPE_SUMINSTANTPOW4:						//0x8F53//有功功率总加组4
//		ucRet=Fill_GRTaskData_SumInstantPow(ucMPNo,ucTaskSour,3);
//		break;
//	case CON_DI_TYPE_SUMINSTANTPOW5:						//0x8F54//有功功率总加组5
//		ucRet=Fill_GRTaskData_SumInstantPow(ucMPNo,ucTaskSour,4);
//		break;
//	case CON_DI_TYPE_SUMINSTANTPOW6:						//0x8F55//有功功率总加组6
//		ucRet=Fill_GRTaskData_SumInstantPow(ucMPNo,ucTaskSour,5);
//		break;
//	case CON_DI_TYPE_SUMINSTANTPOW7:						//0x8F56//有功功率总加组7
//		ucRet=Fill_GRTaskData_SumInstantPow(ucMPNo,ucTaskSour,6);
//		break;
//	case CON_DI_TYPE_SUMINSTANTPOW8:						//0x8F57//有功功率总加组8
//		ucRet=Fill_GRTaskData_SumInstantPow(ucMPNo,ucTaskSour,7);
//		break;
//	case CON_DI_TYPE_SUMINSTANTVAR1:					//0x8F60//无功功率总加组1
//		ucRet=Fill_GRTaskData_SumInstantVar(ucMPNo,ucTaskSour,0);
//		break;
//	case CON_DI_TYPE_SUMINSTANTVAR2:					//0x8F61//无功功率总加组2
//		ucRet=Fill_GRTaskData_SumInstantVar(ucMPNo,ucTaskSour,1);
//		break;
//	case CON_DI_TYPE_SUMINSTANTVAR3:					//0x8F62//无功功率总加组3
//		ucRet=Fill_GRTaskData_SumInstantVar(ucMPNo,ucTaskSour,2);
//		break;
//	case CON_DI_TYPE_SUMINSTANTVAR4:					//0x8F63//无功功率总加组4
//		ucRet=Fill_GRTaskData_SumInstantVar(ucMPNo,ucTaskSour,3);
//		break;
//	case CON_DI_TYPE_SUMINSTANTVAR5:					//0x8F64//无功功率总加组5
//		ucRet=Fill_GRTaskData_SumInstantVar(ucMPNo,ucTaskSour,4);
//		break;
//	case CON_DI_TYPE_SUMINSTANTVAR6:					//0x8F65//无功功率总加组6
//		ucRet=Fill_GRTaskData_SumInstantVar(ucMPNo,ucTaskSour,5);
//		break;
//	case CON_DI_TYPE_SUMINSTANTVAR7:					//0x8F66//无功功率总加组7
//		ucRet=Fill_GRTaskData_SumInstantVar(ucMPNo,ucTaskSour,6);
//		break;
//	case CON_DI_TYPE_SUMINSTANTVAR8:					//0x8F67//无功功率总加组8
//		ucRet=Fill_GRTaskData_SumInstantVar(ucMPNo,ucTaskSour,7);
//		break;
//	case CON_DI_TYPE_SUMPOW1:							//0x8F70//有功总加组1
//		ucRet=Fill_GRTaskData_SumPow(ucMPNo,ucTaskSour,0);
//		break;
//	case CON_DI_TYPE_SUMPOW2:							//0x8F71//有功总加组2
//		ucRet=Fill_GRTaskData_SumPow(ucMPNo,ucTaskSour,1);
//		break;
//	case CON_DI_TYPE_SUMPOW3:							//0x8F72//有功总加组3
//		ucRet=Fill_GRTaskData_SumPow(ucMPNo,ucTaskSour,2);
//		break;
//	case CON_DI_TYPE_SUMPOW4:							//0x8F73//有功总加组4
//		ucRet=Fill_GRTaskData_SumPow(ucMPNo,ucTaskSour,3);
//		break;
//	case CON_DI_TYPE_SUMPOW5:							//0x8F74//有功总加组5
//		ucRet=Fill_GRTaskData_SumPow(ucMPNo,ucTaskSour,4);
//		break;
//	case CON_DI_TYPE_SUMPOW6:							//0x8F75//有功总加组6
//		ucRet=Fill_GRTaskData_SumPow(ucMPNo,ucTaskSour,5);
//		break;
//	case CON_DI_TYPE_SUMPOW7:							//0x8F76//有功总加组7
//		ucRet=Fill_GRTaskData_SumPow(ucMPNo,ucTaskSour,6);
//		break;
//	case CON_DI_TYPE_SUMPOW8:							//0x8F77//有功总加组8
//		ucRet=Fill_GRTaskData_SumPow(ucMPNo,ucTaskSour,7);
//		break;
//	case CON_DI_TYPE_SUMVAR1:						//0x8F80//无功总加组1
//		ucRet=Fill_GRTaskData_SumVar(ucMPNo,ucTaskSour,0);
//		break;
//	case CON_DI_TYPE_SUMVAR2:						//0x8F81//无功总加组2
//		ucRet=Fill_GRTaskData_SumVar(ucMPNo,ucTaskSour,1);
//		break;
//	case CON_DI_TYPE_SUMVAR3:						//0x8F82//无功总加组3
//		ucRet=Fill_GRTaskData_SumVar(ucMPNo,ucTaskSour,2);
//		break;
//	case CON_DI_TYPE_SUMVAR4:						//0x8F83//无功总加组4
//		ucRet=Fill_GRTaskData_SumVar(ucMPNo,ucTaskSour,3);
//		break;
//	case CON_DI_TYPE_SUMVAR5:						//0x8F84//无功总加组5
//		ucRet=Fill_GRTaskData_SumVar(ucMPNo,ucTaskSour,4);
//		break;
//	case CON_DI_TYPE_SUMVAR6:						//0x8F85//无功总加组6
//		ucRet=Fill_GRTaskData_SumVar(ucMPNo,ucTaskSour,5);
//		break;
//	case CON_DI_TYPE_SUMVAR7:						//0x8F86//无功总加组7
//		ucRet=Fill_GRTaskData_SumVar(ucMPNo,ucTaskSour,6);
//		break;
//	case CON_DI_TYPE_SUMVAR8:						//0x8F87//无功总加组8
//		ucRet=Fill_GRTaskData_SumVar(ucMPNo,ucTaskSour,7);
//		break;
	default:
		ucRet=RET_ERROR;
		break;
	}
	return ucRet;
}

//unsigned char Fill_GRTaskData_DirVar_Invalid()
//{//正向无功电量（无效）
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRVARTOTAL ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRVARPLUS || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRVARAPEX || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRVARFLAT || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRVARVALE )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//		}
//		if(stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRVAR_BLOCK)
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+4,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+8,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+16,4);
//		}
//	}
//	return ucRet;
//}
//unsigned char Fill_GRTaskData_DirPower_Invalid()
//{//正向有功电量（无效）
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWTOTAL ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWPLUS || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWAPEX || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWFLAT || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWVALE )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//		}
//		if(stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOW_BLOCK)
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+4,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+8,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+16,4);
//		}
//	}
//	return ucRet;
//}
//unsigned char Fill_GRTaskData_DirPowerLM_Invalid()
//{//正向有功电量（无效）（上月）
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWTOTAL_LM ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWPLUS_LM || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWAPEX_LM || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWFLAT_LM || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWVALE_LM )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//		}
//		if(stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOW_LM_BLOCK)
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+4,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+8,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+16,4);
//		}
//	}
//	return ucRet;
//}
//unsigned char Fill_GRTaskData_DirVarLM_Invalid()
//{//正向无功电量（无效）（上月）
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRVARTOTAL_LM ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRVARPLUS_LM || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRVARAPEX_LM || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRVARFLAT_LM || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRVARVALE_LM )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//		}
//		if(stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRVAR_LM_BLOCK)
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+4,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+8,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+16,4);
//		}
//	}
//	return ucRet;
//}
//unsigned char Fill_GRTaskData_UnDirVar_Invalid()
//{//反向有功电量（无效）
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRVARTOTAL ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRVARPLUS || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRVARAPEX ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRVARFLAT || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRVARVALE )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//		}
//		if(stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRVAR_BLOCK )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+4,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+8,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+16,4);
//		}
//	}
//	return ucRet;
//}
//unsigned char Fill_GRTaskData_UnDirPower_Invalid()
//{//反向有功电量（无效）
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWTOTAL ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWPLUS || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWAPEX ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWFLAT || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWVALE )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//		}
//		if(stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOW_BLOCK )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+4,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+8,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+16,4);
//		}
//	}
//	return ucRet;
//}
//unsigned char Fill_GRTaskData_UnDirPowerLM_Invalid()
//{//反向有功电量（无效）（上月）
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWTOTAL_LM ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWPLUS_LM || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWAPEX_LM ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWFLAT_LM || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWVALE_LM )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//		}
//		if(stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOW_LM_BLOCK)
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+4,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+8,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+16,4);
//		}
//	}
//	return ucRet;
//}
//unsigned char Fill_GRTaskData_UnDirVarLM_Invalid()
//{//反向无功电量（无效）（上月）
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRVARTOTAL_LM ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRVARPLUS_LM || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRVARAPEX_LM ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRVARFLAT_LM || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRVARVALE_LM )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//		}
//		if(stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRVAR_LM_BLOCK)
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+4,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+8,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+16,4);
//		}
//	}
//	return ucRet;
//}
//unsigned char Fill_GRTaskData_InstantVal_Invalid()
//{//专为浙江规约
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPCURA ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPCURB ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPCURC ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVOLA ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVOLB ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVOLC ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPINSTANTVAR )
//
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,2);
//
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPINSTANTPOW )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,3);
//
//	}
//	return ucRet;
//}
//unsigned char Fill_GRTaskData_I_Invalid(unsigned short usDI)
//{//电流（无效）
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==usDI )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,2);
//	}
//	return ucRet;
//}
//unsigned char Fill_GRTaskData_V_Invalid(unsigned short usDI)
//{//电压（无效）
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==usDI )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,2);
//	}
//	return ucRet;
//}
//unsigned char Fill_GRTaskData_DirVar1_Invalid()
//{//1象限无功（无效）
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR1 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR1_FEE1 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR1_FEE2 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR1_FEE3 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR1_FEE4 )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//		
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR1_BLOCK )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+4,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+8,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+16,4);
//		}
//	}
//	return ucRet;
//}
//
//unsigned char Fill_GRTaskData_DirVar2_Invalid()
//{//2象限无功（无效）
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR2 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR2_FEE1 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR2_FEE2 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR2_FEE3 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR2_FEE4 )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//		
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR2_BLOCK )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+4,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+8,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+16,4);
//		}
//	}
//	return ucRet;
//}
//unsigned char Fill_GRTaskData_DirVar3_Invalid()
//{//3象限无功（无效）
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR3 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR3_FEE1 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR3_FEE2 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR3_FEE3 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR3_FEE4 )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//		
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR3_BLOCK )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+4,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+8,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+16,4);
//		}
//	}
//	return ucRet;
//}
//unsigned char Fill_GRTaskData_DirVar4_Invalid()
//{//4象限无功（无效）
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR4 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR4_FEE1 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR4_FEE2 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR4_FEE3 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR4_FEE4 )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//		
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR4_BLOCK )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+4,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+8,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+16,4);
//		}
//	}
//	return ucRet;
//}
//unsigned char Fill_GRTaskData_DirVar1LM_Invalid()
//{//1象限无功（无效）（上月）
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR1_LM || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR1_LM_FEE1 ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR1_LM_FEE2 ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR1_LM_FEE3 ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR1_LM_FEE4 )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR1_LM_BLOCK )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+4,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+8,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+16,4);
//		}
//	}
//	return ucRet;
//}
//unsigned char Fill_GRTaskData_DirVar2LM_Invalid()
//{//2象限无功（无效）（上月）
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR2_LM || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR2_LM_FEE1 ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR2_LM_FEE2 ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR2_LM_FEE3 ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR2_LM_FEE4 )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR2_LM_BLOCK )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+4,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+8,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+16,4);
//		}
//	}
//	return ucRet;
//}
//unsigned char Fill_GRTaskData_DirVar3LM_Invalid()
//{//3象限无功（无效）（上月）
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR3_LM || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR3_LM_FEE1 ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR3_LM_FEE2 ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR3_LM_FEE3 ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR3_LM_FEE4 )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR3_LM_BLOCK )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+4,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+8,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+16,4);
//		}
//	}
//	return ucRet;
//}
//unsigned char Fill_GRTaskData_DirVar4LM_Invalid()
//{//4象限无功（无效）（上月）
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR4_LM || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR4_LM_FEE1 ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR4_LM_FEE2 ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR4_LM_FEE3 ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR4_LM_FEE4 )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPVAR1_LM_BLOCK )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+4,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+8,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+16,4);
//		}
//	}
//	return ucRet;
//}
//unsigned char Fill_GRTaskData_InstantPow_Invalid(unsigned short usDI)
//{//瞬时有功（无效）
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==usDI )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,3);
//	}
//	return ucRet;
//}
//unsigned char Fill_GRTaskData_InstantVar_Invalid(unsigned short usDI)
//{//瞬时无功（无效）
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==usDI )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,2);
//	}
//	return ucRet;	
//}
//unsigned char Fill_GRTaskData_DiraPowMaxDemand_Invalid()
//{//正向有功总最大需量
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWMAXDEMAND || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE1 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE2 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE3 ||
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE4 )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,3);
//
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWMAXDEMAND_BLOCK )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,3);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+3,3);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+6,3);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+9,3);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,3);
//		}
//	}
//	return ucRet;	
//}
//unsigned char Fill_GRTaskData_DiraPowMaxDemandLM_Invalid()
//{//正向有功总最大需量（上月）
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE1 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE2 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE3 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE4 )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,3);
//
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_BLOCK )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,3);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+3,3);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+6,3);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+9,3);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,3);
//		}
//	}
//	return ucRet;	
//}
//unsigned char Fill_GRTaskData_DiraPowMaxDemandTime_Invalid()
//{//正向有功总最大需量时间
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE1 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE2 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE3 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE4 )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_BLOCK )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+4,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+8,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+16,4);
//		}
//	}
//	return ucRet;	
//}
//unsigned char Fill_GRTaskData_DiraPowMaxDemandTimeLM_Invalid()
//{//正向有功总最大需量时间（上月）
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE1 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE2 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE3 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE4 )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_BLOCK )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+4,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+8,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+16,4);
//		}
//	}
//	return ucRet;	
//}
//unsigned char Fill_GRTaskData_UnDiraPowMaxDemand_Invalid()
//{//反向有功总最大需量
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWMAXDEMAND || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE1 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE2 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE3 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE4 )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,3);
//
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_BLOCK )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,3);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+3,3);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+6,3);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+9,3);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,3);
//		}
//	}
//	return ucRet;	
//}
//unsigned char Fill_GRTaskData_UnDiraPowMaxDemandLM_Invalid()
//{//反向有功总最大需量（上月）
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE1 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE2 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE3 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE4 )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,3);
//
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_BLOCK )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,3);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+3,3);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+6,3);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+9,3);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,3);
//		}
//	}
//	return ucRet;	
//}
//unsigned char Fill_GRTaskData_UnDiraPowMaxDemandTime_Invalid()
//{//正向有功总最大需量时间
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE1 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE2 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE3 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE4 )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_BLOCK )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+4,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+8,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+16,4);
//		}
//	}
//	return ucRet;	
//}
//unsigned char Fill_GRTaskData_UnDiraPowMaxDemandTimeLM_Invalid()
//{//正向有功总最大需量时间（上月）
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE1 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE2 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE3 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE4 )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_BLOCK )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+4,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+8,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,4);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+16,4);
//		}
//	}
//	return ucRet;	
//}
//unsigned char Fill_GRTaskData_Const_Invalid(unsigned short usDI)
//{//常数
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==usDI )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,3);
//	}
//	return ucRet;
//}
//
//unsigned char Fill_GRTaskData_FeeRate_Invalid()
//{//费率	
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE1 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE2 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE3 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE4 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE5 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE6 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE7 || 
//			stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE8 )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,3);
//		}
//
//		if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE_BLOCK )
//		{
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,3);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+3,3);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+6,3);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+9,3);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+12,3);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+15,3);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+18,3);
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL+21,3);
//		}
//	}
//	return ucRet;
//}

///////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char Fill_GRTaskData_Val(unsigned char ucDINo,unsigned short usDI_Sml,unsigned short usDI_Big,unsigned char ucSml_l,unsigned char ucBig_l,unsigned char ucValLen)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucTempSrc[8],ucTempDes[8];
	unsigned short usDataPtr=0;

	memset(ucTempSrc,0,8);
	memset(ucTempDes,0,8);
	if(ucValLen>8)
		return RET_ERROR;
	if( stGRDefineArea.stGRTaskRetVal.stDIVal[ucDINo].usDI==usDI_Sml)
	{
		memcpy(ucTempSrc,stFillInTask.ucDataArea,stFillInTask.ucDataLen);
		Convert_Hex_To_TaskData(usDI_Sml,ucTempSrc,ucTempDes);
        usDataPtr = GetGlobeRecvBufPtr(ucDINo);
		for(i=0;i<ucValLen;i++)
			//stGRDefineArea.stGRTaskRetVal.stDIVal[ucDINo].ucDIVAL[i+ucSml_l]=ucTempDes[i];//azh
			stGRDefineArea.stGRTaskRetVal.ucTaskData[usDataPtr+i+ucSml_l]=ucTempDes[i];
	}

	if( stGRDefineArea.stGRTaskRetVal.stDIVal[ucDINo].usDI==usDI_Big)
	{
		memcpy(ucTempSrc,stFillInTask.ucDataArea,stFillInTask.ucDataLen);
		Convert_Hex_To_TaskData(usDI_Sml,ucTempSrc,ucTempDes);
		for(i=0;i<ucValLen;i++)
			//stGRDefineArea.stGRTaskRetVal.stDIVal[ucDINo].ucDIVAL[i+ucBig_l]=ucTempDes[i];
			stGRDefineArea.stGRTaskRetVal.ucTaskData[usDataPtr+i+ucBig_l]=ucTempDes[i];
	}
	return ucRet;
}

//unsigned char Fill_GRTaskData_Val_Direct(unsigned char ucDINo,unsigned short usDI_Sml,unsigned short usDI_Big,unsigned char ucSml_l,unsigned char ucBig_l,unsigned char ucValLen)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	unsigned char ucTemp[8];
//	unsigned short usDataPtr=0;
//	
//	memset(ucTemp,0,8);
//	if(ucValLen>8)
//		return RET_ERROR;
//	if( stGRDefineArea.stGRTaskRetVal.stDIVal[ucDINo].usDI==usDI_Sml)
//	{
//		memcpy(ucTemp,stFillInTask.ucDataArea,stFillInTask.ucDataLen);
//		for(i=0;i<ucValLen;i++)
//			stGRDefineArea.stGRTaskRetVal.ucTaskData[usDataPtr+i+ucSml_l]=ucTemp[i];
//	}
//
//	if( stGRDefineArea.stGRTaskRetVal.stDIVal[ucDINo].usDI==usDI_Big)
//	{
//		memcpy(ucTemp,stFillInTask.ucDataArea,stFillInTask.ucDataLen);
//		for(i=0;i<ucValLen;i++)
//			stGRDefineArea.stGRTaskRetVal.ucTaskData[usDataPtr+i+ucBig_l]=ucTemp[i];
//	}
//	return ucRet;
//}

//无功电量
unsigned char Fill_GRTaskData_DirVar(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
{//正向无功总
	unsigned char ucRet=RET_SUCCESS,i=0;
	STHISVAL	  stHisVal;
	unsigned char ucFeeNo;

	ucFeeNo=usDILabel-CON_DI_TYPE_MPDIRVARTOTAL;
	if(ucTaskSour==CONRS485SOUR_GRTASK)
		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
	else
	{
		stHisVal	= Get_History_DirVar_byMPNo(ucMPNo,0,ucFeeNo);
		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usDILabel,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
	}
	if(ucRet==RET_SUCCESS)
	{
		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
			Fill_GRTaskData_Val(i,usDILabel,CON_DI_TYPE_MPDIRVAR_BLOCK,0,(unsigned char)(ucFeeNo*4),4);
	}
	return ucRet;
}
unsigned char Fill_GRTaskData_UnDirVar(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
{//反向无功总
	unsigned char ucRet=RET_SUCCESS,i=0;
	STHISVAL	  stHisVal;
	unsigned char ucFeeNo;

	ucFeeNo=usDILabel-CON_DI_TYPE_MPUNDIRVARTOTAL;
	if(ucTaskSour==CONRS485SOUR_GRTASK)
		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
	else
	{
		stHisVal	= Get_History_UnDirVar_byMPNo(ucMPNo,0,ucFeeNo);
		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usDILabel,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
	}
	if(ucRet==RET_SUCCESS)
	{
		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
			Fill_GRTaskData_Val(i,usDILabel,CON_DI_TYPE_MPUNDIRVAR_BLOCK,0,(unsigned char)(ucFeeNo*4),4);
	}
	return ucRet;
}
/************************************正向有功电量****************************************/
unsigned char Fill_GRTaskData_DirPower(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
{//正向有功总
	unsigned char ucRet=RET_SUCCESS,i=0;
	STHISVAL	  stHisVal;
	unsigned char ucFeeNo;

	ucFeeNo=usDILabel-CON_DI_TYPE_MPDIRPOWTOTAL;
	if(ucTaskSour==CONRS485SOUR_GRTASK)
		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
	else//CONRS485SOUR_GRTASK_UNREAD
	{
		stHisVal	= Get_History_DirPower_byMPNo(ucMPNo,0,ucFeeNo);
		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usDILabel,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
	}
	if(ucRet==RET_SUCCESS)
	{
		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
			Fill_GRTaskData_Val(i,usDILabel,CON_DI_TYPE_MPDIRPOW_BLOCK,0,(unsigned char)(ucFeeNo*4),4);
	}
	return ucRet;
}
/************************************反向有功电量****************************************/
unsigned char Fill_GRTaskData_UnDirPower(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
{//反向有功总
	unsigned char ucRet=RET_SUCCESS,i=0;
	STHISVAL	  stHisVal;
	unsigned char ucFeeNo;

	ucFeeNo=usDILabel-CON_DI_TYPE_MPUNDIRPOWTOTAL;
	if(ucTaskSour==CONRS485SOUR_GRTASK)
		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
	else
	{
		stHisVal	= Get_History_UnDirPower_byMPNo(ucMPNo,0,ucFeeNo);
		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usDILabel,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
	}
	if(ucRet==RET_SUCCESS)
	{
		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
			Fill_GRTaskData_Val(i,usDILabel,CON_DI_TYPE_MPUNDIRPOW_BLOCK,0,(unsigned char)(ucFeeNo*4),4);
	}
	return ucRet;
}
///************************************上月正向有功电量****************************************/
//unsigned char Fill_GRTaskData_DirPower_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
//{//正向有功总
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	STHISVAL	  stHisVal;
//	unsigned char ucFeeNo;
//
//	ucFeeNo=usDILabel-CON_DI_TYPE_MPDIRPOWTOTAL_LM;
//	if(ucTaskSour==CONRS485SOUR_GRTASK)
//		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
//	else
//	{
//		stHisVal	= Get_History_DirPowerLm_byMPNo(ucMPNo,0,ucFeeNo);
//		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usDILabel,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
//	}
//	if(ucRet==RET_SUCCESS)
//	{
//		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//			Fill_GRTaskData_Val(i,usDILabel,CON_DI_TYPE_MPDIRPOW_LM_BLOCK,0,(unsigned char)(ucFeeNo*4),4);
//	}
//	return ucRet;
//}
///************************************上月反向有功电量****************************************/
//unsigned char Fill_GRTaskData_UnDirPower_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
//{//反向有功总
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	STHISVAL	stHisVal;
//	unsigned char ucFeeNo;
//
//	ucFeeNo=usDILabel-CON_DI_TYPE_MPUNDIRPOWTOTAL_LM;
//	if(ucTaskSour==CONRS485SOUR_GRTASK)
//		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
//	else
//	{
//		stHisVal	= Get_History_UnDirPowerLm_byMPNo(ucMPNo,0,ucFeeNo);
//		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usDILabel,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
//	}
//
//	if(ucRet == RET_SUCCESS)
//	{
//		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//			Fill_GRTaskData_Val(i,usDILabel,CON_DI_TYPE_MPUNDIRPOW_LM_BLOCK,0,(unsigned char)(ucFeeNo*4),4);
//	}
//	return ucRet;
//}
/************************************三相电流************************************************/
unsigned char Fill_GRTaskData_Ia(unsigned char ucMPNo,unsigned char ucTaskSour)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	STHISVAL	  stHisVal;

	if(ucTaskSour==CONRS485SOUR_GRTASK)
		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
	else
	{
		stHisVal	= Get_History_I_byMPNo(ucMPNo,0,1);
		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,CON_DI_TYPE_MPCURA,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
	}
	if(ucRet==RET_SUCCESS)
	{
		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
			Fill_GRTaskData_Val(i,CON_DI_TYPE_MPCURA,CON_DI_TYPE_MPCUR_BLOCK,0,0,3);//0,0,2);
	}
	return ucRet;
}
unsigned char Fill_GRTaskData_Ib(unsigned char ucMPNo,unsigned char ucTaskSour)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	STHISVAL	  stHisVal;

	if(ucTaskSour==CONRS485SOUR_GRTASK)
		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
	else
	{
		stHisVal	= Get_History_I_byMPNo(ucMPNo,0,2);
		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,CON_DI_TYPE_MPCURB,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
	}
	if(ucRet==RET_SUCCESS)
	{
		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
			Fill_GRTaskData_Val(i,CON_DI_TYPE_MPCURB,CON_DI_TYPE_MPCUR_BLOCK,0,3,3);//,2,2);
	}
	return ucRet;
}
unsigned char Fill_GRTaskData_Ic(unsigned char ucMPNo,unsigned char ucTaskSour)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	STHISVAL	  stHisVal;

	if(ucTaskSour==CONRS485SOUR_GRTASK)
		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
	else
	{
		stHisVal	= Get_History_I_byMPNo(ucMPNo,0,3);
		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,CON_DI_TYPE_MPCURC,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
	}
	if(ucRet==RET_SUCCESS)
	{
		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
			Fill_GRTaskData_Val(i,CON_DI_TYPE_MPCURC,CON_DI_TYPE_MPCUR_BLOCK,0,6,3);//,4,2);//azh 
	}
	return ucRet;
}
/************************************三相电压************************************************/
unsigned char Fill_GRTaskData_Va(unsigned char ucMPNo,unsigned char ucTaskSour)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	STHISVAL	  stHisVal;

	if(ucTaskSour==CONRS485SOUR_GRTASK)
		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
	else
	{
		stHisVal	= Get_History_V_byMPNo(ucMPNo,0,1);
		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,CON_DI_TYPE_MPVOLA,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
	}
	if(ucRet==RET_SUCCESS)
	{
		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
			Fill_GRTaskData_Val(i,CON_DI_TYPE_MPVOLA,CON_DI_TYPE_MPVOL_BLOCK,0,0,2);
	}
	return ucRet;
}
unsigned char Fill_GRTaskData_Vb(unsigned char ucMPNo,unsigned char ucTaskSour)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	STHISVAL	  stHisVal;

	if(ucTaskSour==CONRS485SOUR_GRTASK)
		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
	else
	{
		stHisVal	= Get_History_V_byMPNo(ucMPNo,0,2);
		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,CON_DI_TYPE_MPVOLB,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
	}
	if(ucRet==RET_SUCCESS)
	{
		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
			Fill_GRTaskData_Val(i,CON_DI_TYPE_MPVOLB,CON_DI_TYPE_MPVOL_BLOCK,0,2,2);
	}
	return ucRet;
}
unsigned char Fill_GRTaskData_Vc(unsigned char ucMPNo,unsigned char ucTaskSour)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	STHISVAL	  stHisVal;

	if(ucTaskSour==CONRS485SOUR_GRTASK)
		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
	else
	{
		stHisVal	= Get_History_V_byMPNo(ucMPNo,0,3);
		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,CON_DI_TYPE_MPVOLC,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
	}
	if(ucRet==RET_SUCCESS)
	{
		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
			Fill_GRTaskData_Val(i,CON_DI_TYPE_MPVOLC,CON_DI_TYPE_MPVOL_BLOCK,0,4,2);
	}
	return ucRet;
}
/************************************瞬时有功功率************************************************/
unsigned char Fill_GRTaskData_InstantPow(unsigned char ucMPNo,unsigned char ucTaskSour)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	STHISVAL	  stHisVal;

	if(ucTaskSour==CONRS485SOUR_GRTASK)
		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
	else
	{
		stHisVal	= Get_History_InstantPow_byMPNo(ucMPNo,0,0);
		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,CON_DI_TYPE_MPINSTANTPOW,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
	}
	if(ucRet==RET_SUCCESS)
	{
		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
			Fill_GRTaskData_Val(i,CON_DI_TYPE_MPINSTANTPOW,CON_DI_TYPE_MPINSTANTPOW_BLOCK,0,0,3);
	}
	return ucRet;
}
unsigned char Fill_GRTaskData_InstantPowA(unsigned char ucMPNo,unsigned char ucTaskSour)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	STHISVAL	  stHisVal;

	if(ucTaskSour==CONRS485SOUR_GRTASK)
		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
	else
	{
		stHisVal	= Get_History_InstantPow_byMPNo(ucMPNo,0,1);
		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,CON_DI_TYPE_MPINSTANTPOWA,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
	}
	if(ucRet==RET_SUCCESS)
	{
		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
			Fill_GRTaskData_Val(i,CON_DI_TYPE_MPINSTANTPOWA,CON_DI_TYPE_MPINSTANTPOW_BLOCK,0,3,3);
	}
	return ucRet;
}
unsigned char Fill_GRTaskData_InstantPowB(unsigned char ucMPNo,unsigned char ucTaskSour)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	STHISVAL	  stHisVal;

	if(ucTaskSour==CONRS485SOUR_GRTASK)
		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
	else
	{
		stHisVal	= Get_History_InstantPow_byMPNo(ucMPNo,0,2);
		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,CON_DI_TYPE_MPINSTANTPOWB,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
	}
	if(ucRet==RET_SUCCESS)
	{
		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
			Fill_GRTaskData_Val(i,CON_DI_TYPE_MPINSTANTPOWB,CON_DI_TYPE_MPINSTANTPOW_BLOCK,0,6,3);
	}
	return ucRet;
}
unsigned char Fill_GRTaskData_InstantPowC(unsigned char ucMPNo,unsigned char ucTaskSour)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	STHISVAL	  stHisVal;

	if(ucTaskSour==CONRS485SOUR_GRTASK)
		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
	else
	{
		stHisVal	= Get_History_InstantPow_byMPNo(ucMPNo,0,3);
		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,CON_DI_TYPE_MPINSTANTPOWC,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
	}
	if(ucRet==RET_SUCCESS)
	{
		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
			Fill_GRTaskData_Val(i,CON_DI_TYPE_MPINSTANTPOWC,CON_DI_TYPE_MPINSTANTPOW_BLOCK,0,9,3);
	}
	return ucRet;
}
/************************************瞬时无功功率************************************************/
unsigned char Fill_GRTaskData_InstantVar(unsigned char ucMPNo,unsigned char ucTaskSour)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	STHISVAL	  stHisVal;

	if(ucTaskSour==CONRS485SOUR_GRTASK)
		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
	else
	{
		stHisVal	= Get_History_InstantVar_byMPNo(ucMPNo,0,0);
		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,CON_DI_TYPE_MPINSTANTVAR,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
	}
	if(ucRet==RET_SUCCESS)
	{
		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
			Fill_GRTaskData_Val(i,CON_DI_TYPE_MPINSTANTVAR,CON_DI_TYPE_MPINSTANTVAR_BLOCK,0,0,2);
	}
	return ucRet;
}
unsigned char Fill_GRTaskData_InstantVarA(unsigned char ucMPNo,unsigned char ucTaskSour)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	STHISVAL	  stHisVal;

	if(ucTaskSour==CONRS485SOUR_GRTASK)
		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
	else
	{
		stHisVal	= Get_History_InstantVar_byMPNo(ucMPNo,0,1);
		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,CON_DI_TYPE_MPINSTANTVARA,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
	}
	if(ucRet==RET_SUCCESS)
	{
		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
			Fill_GRTaskData_Val(i,CON_DI_TYPE_MPINSTANTVARA,CON_DI_TYPE_MPINSTANTVAR_BLOCK,0,2,2);
	}
	return ucRet;
}
unsigned char Fill_GRTaskData_InstantVarB(unsigned char ucMPNo,unsigned char ucTaskSour)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	STHISVAL	  stHisVal;

	if(ucTaskSour==CONRS485SOUR_GRTASK)
		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
	else
	{
		stHisVal	= Get_History_InstantVar_byMPNo(ucMPNo,0,2);
		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,CON_DI_TYPE_MPINSTANTVARB,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
	}
	if(ucRet==RET_SUCCESS)
	{
		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
			Fill_GRTaskData_Val(i,CON_DI_TYPE_MPINSTANTVARB,CON_DI_TYPE_MPINSTANTVAR_BLOCK,0,4,2);
	}
	return ucRet;
}
unsigned char Fill_GRTaskData_InstantVarC(unsigned char ucMPNo,unsigned char ucTaskSour)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	STHISVAL	  stHisVal;

	if(ucTaskSour==CONRS485SOUR_GRTASK)
		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
	else
	{
		stHisVal	= Get_History_InstantVar_byMPNo(ucMPNo,0,3);
		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,CON_DI_TYPE_MPINSTANTVARC,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
	}
	if(ucRet==RET_SUCCESS)
	{
		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
			Fill_GRTaskData_Val(i,CON_DI_TYPE_MPINSTANTVARC,CON_DI_TYPE_MPINSTANTVAR_BLOCK,0,6,2);
	}
	return ucRet;
}
/************************************一象限无功************************************************/
unsigned char Fill_GRTaskData_DirVar1(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	STHISVAL	  stHisVal;
	unsigned char ucFeeNo;

	ucFeeNo=usDILabel-CON_DI_TYPE_MPVAR1;
	if(ucTaskSour==CONRS485SOUR_GRTASK)
		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
	else
	{
		stHisVal	= Get_History_DirVar1_byMPNo(ucMPNo,0,ucFeeNo);
		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usDILabel,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
	}
	if(ucRet==RET_SUCCESS)
	{
		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
			Fill_GRTaskData_Val(i,usDILabel,CON_DI_TYPE_MPVAR1_BLOCK,0,(unsigned char)(ucFeeNo*4),4);
	}
	return ucRet;
}
/************************************二象限无功************************************************/
unsigned char Fill_GRTaskData_DirVar2(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	STHISVAL	  stHisVal;
	unsigned char ucFeeNo;

	ucFeeNo=usDILabel-CON_DI_TYPE_MPVAR2;
	if(ucTaskSour==CONRS485SOUR_GRTASK)
		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
	else
	{
		stHisVal	= Get_History_DirVar2_byMPNo(ucMPNo,0,ucFeeNo);
		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usDILabel,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
	}
	if(ucRet==RET_SUCCESS)
	{
		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
			Fill_GRTaskData_Val(i,usDILabel,CON_DI_TYPE_MPVAR2_BLOCK,0,(unsigned char)(ucFeeNo*4),4);
	}
	return ucRet;
}
/************************************三象限无功************************************************/
unsigned char Fill_GRTaskData_DirVar3(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	STHISVAL	  stHisVal;
	unsigned char ucFeeNo;

	ucFeeNo=usDILabel-CON_DI_TYPE_MPVAR3;
	if(ucTaskSour==CONRS485SOUR_GRTASK)
		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
	else
	{
		stHisVal	= Get_History_DirVar3_byMPNo(ucMPNo,0,ucFeeNo);
		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usDILabel,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
	}
	if(ucRet==RET_SUCCESS)
	{
		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
			Fill_GRTaskData_Val(i,usDILabel,CON_DI_TYPE_MPVAR3_BLOCK,0,(unsigned char)(ucFeeNo*4),4);
	}
	return ucRet;
}
/************************************四象限无功************************************************/
unsigned char Fill_GRTaskData_DirVar4(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	STHISVAL	  stHisVal;
	unsigned char ucFeeNo;

	ucFeeNo=usDILabel-CON_DI_TYPE_MPVAR4;
	if(ucTaskSour==CONRS485SOUR_GRTASK)
		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
	else
	{
		stHisVal	= Get_History_DirVar4_byMPNo(ucMPNo,0,ucFeeNo);
		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usDILabel,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
	}
	if(ucRet==RET_SUCCESS)
	{	
		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
			Fill_GRTaskData_Val(i,usDILabel,CON_DI_TYPE_MPVAR4_BLOCK,0,(unsigned char)(ucFeeNo*4),4);
	}
	return ucRet;
}

///************************************上月正向无功电量****************************************/
//unsigned char Fill_GRTaskData_DirVar_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
//{//正向无功总
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	STHISVAL	  stHisVal;
//	unsigned char ucFeeNo;
//
//	ucFeeNo=usDILabel-CON_DI_TYPE_MPDIRVARTOTAL_LM;
//	if(ucTaskSour==CONRS485SOUR_GRTASK)
//		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
//	else
//	{
//		stHisVal	= Get_History_DirVarLm_byMPNo(ucMPNo,0,0);
//		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usDILabel,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
//	}
//	if(ucRet==RET_SUCCESS)
//	{
//		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//			Fill_GRTaskData_Val(i,usDILabel,CON_DI_TYPE_MPDIRVAR_LM_BLOCK,0,(unsigned char)(ucFeeNo*4),4);
//	}
//	return ucRet;
//}
///************************************上月反向无功电量****************************************/
//unsigned char Fill_GRTaskData_UnDirVar_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
//{//反向无功总
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	STHISVAL	stHisVal;
//	unsigned char ucFeeNo;
//
//	ucFeeNo=usDILabel-CON_DI_TYPE_MPUNDIRVARTOTAL_LM;
//	if(ucTaskSour==CONRS485SOUR_GRTASK)
//		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
//	else
//	{
//		stHisVal	= Get_History_UnDirVarLm_byMPNo(ucMPNo,0,ucFeeNo);
//		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usDILabel,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
//	}
//
//	if(ucRet == RET_SUCCESS)
//	{
//		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//			Fill_GRTaskData_Val(i,usDILabel,CON_DI_TYPE_MPUNDIRVAR_LM_BLOCK,0,(unsigned char)(ucFeeNo*4),4);
//	}
//	return ucRet;
//}
//
///************************************上月一象限无功************************************************/
//unsigned char Fill_GRTaskData_DirVar1_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	STHISVAL	  stHisVal;
//	unsigned char ucFeeNo;
//
//	ucFeeNo=usDILabel-CON_DI_TYPE_MPVAR1_LM;	
//	if(ucTaskSour==CONRS485SOUR_GRTASK)
//		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
//	else
//	{
//		stHisVal	= Get_History_DirVar1Lm_byMPNo(ucMPNo,0,ucFeeNo);
//		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usDILabel,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
//	}
//
//	if(ucRet == RET_SUCCESS)
//	{
//		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//			Fill_GRTaskData_Val(i,usDILabel,CON_DI_TYPE_MPVAR1_LM_BLOCK,0,(unsigned char)(ucFeeNo*4),4);
//	}
//	return ucRet;
//}
///************************************上月二象限无功************************************************/
//unsigned char Fill_GRTaskData_DirVar2_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	STHISVAL	  stHisVal;
//	unsigned char ucFeeNo;
//
//	ucFeeNo=usDILabel-CON_DI_TYPE_MPVAR2_LM;	
//	if(ucTaskSour==CONRS485SOUR_GRTASK)
//		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
//	else
//	{
//		stHisVal	= Get_History_DirVar2Lm_byMPNo(ucMPNo,0,ucFeeNo);
//		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usDILabel,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
//	}
//
//	if(ucRet == RET_SUCCESS)
//	{
//		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//			Fill_GRTaskData_Val(i,usDILabel,CON_DI_TYPE_MPVAR2_LM_BLOCK,0,(unsigned char)(ucFeeNo*4),4);
//	}
//	return ucRet;
//}
///************************************上月三象限无功************************************************/
//unsigned char Fill_GRTaskData_DirVar3_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	STHISVAL	  stHisVal;
//	unsigned char ucFeeNo;
//
//	ucFeeNo=usDILabel-CON_DI_TYPE_MPVAR3_LM;
//	if(ucTaskSour==CONRS485SOUR_GRTASK)
//		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
//	else
//	{
//		stHisVal	= Get_History_DirVar3Lm_byMPNo(ucMPNo,0,ucFeeNo);
//		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usDILabel,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
//	}
//
//	if(ucRet == RET_SUCCESS)
//	{
//		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//			Fill_GRTaskData_Val(i,usDILabel,CON_DI_TYPE_MPVAR3_LM_BLOCK,0,(unsigned char)(ucFeeNo*4),4);
//	}
//	return ucRet;
//}
///************************************上月四象限无功************************************************/
//unsigned char Fill_GRTaskData_DirVar4_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	STHISVAL	  stHisVal;
//	unsigned char ucFeeNo;
//
//	ucFeeNo=usDILabel-CON_DI_TYPE_MPVAR4_LM;
//	if(ucTaskSour==CONRS485SOUR_GRTASK)
//		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
//	else
//	{
//		stHisVal	= Get_History_DirVar4Lm_byMPNo(ucMPNo,0,ucFeeNo);
//		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usDILabel,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
//	}
//
//	if(ucRet == RET_SUCCESS)
//	{
//		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//			Fill_GRTaskData_Val(i,usDILabel,CON_DI_TYPE_MPVAR4_LM_BLOCK,0,(unsigned char)(ucFeeNo*4),4);
//	}
//	return ucRet;
//}
///************************************正向有功需量************************************************/
//unsigned char Fill_GRTaskData_DirPowMaxDemand(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	STHISVAL	  stHisVal;
//	unsigned char ucFeeNo;
//
//	ucFeeNo=usDILabel-CON_DI_TYPE_MPDIRPOWMAXDEMAND;
//	if(ucTaskSour==CONRS485SOUR_GRTASK)
//		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
//	else
//	{
//		stHisVal	= Get_History_DirPowMaxDemand_byMPNo(ucMPNo,0,ucFeeNo);
//		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usDILabel,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
//	}
//	if(ucRet==RET_SUCCESS)
//	{	
//		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//			Fill_GRTaskData_Val(i,usDILabel,CON_DI_TYPE_MPDIRPOWMAXDEMAND_BLOCK,0,(unsigned char )(ucFeeNo*3),3);
//	}
//	return ucRet;
//}
///************************************上月正向有功需量********************************************/
//unsigned char Fill_GRTaskData_DirPowMaxDemand_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	STHISVAL	  stHisVal;
//	unsigned char ucFeeNo;
//
//	ucFeeNo=usDILabel-CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM;
//	if(ucTaskSour==CONRS485SOUR_GRTASK)
//		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
//	else
//	{
//		stHisVal	= Get_History_DirPowMaxDemandLm_byMPNo(ucMPNo,0,ucFeeNo);
//		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usDILabel,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
//	}
//	if(ucRet==RET_SUCCESS)
//	{
//		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//			Fill_GRTaskData_Val(i,usDILabel,CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_BLOCK,0,(unsigned char)(ucFeeNo*3),3);
//	}
//	return ucRet;
//}
///************************************反向有功需量********************************************/
//unsigned char Fill_GRTaskData_UnDirPowMaxDemand(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	STHISVAL	  stHisVal;
//	unsigned char ucFeeNo;
//
//	ucFeeNo=usDILabel-CON_DI_TYPE_MPUNDIRPOWMAXDEMAND;
//	if(ucTaskSour==CONRS485SOUR_GRTASK)
//		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
//	else
//	{
//		stHisVal	= Get_History_UnDirPowMaxDemand_byMPNo(ucMPNo,0,ucFeeNo);
//		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usDILabel,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
//	}
//	if(ucRet==RET_SUCCESS)
//	{	
//		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//			Fill_GRTaskData_Val(i,usDILabel,CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_BLOCK,0,(unsigned char)(ucFeeNo*3),3);
//	}
//	return ucRet;
//}
///************************************上月反向有功需量********************************************/
//unsigned char Fill_GRTaskData_UnDirPowMaxDemand_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	STHISVAL	  stHisVal;
//	unsigned char ucFeeNo;
//
//	ucFeeNo=usDILabel-CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM;
//	if(ucTaskSour==CONRS485SOUR_GRTASK)
//		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
//	else
//	{
//		stHisVal	= Get_History_UnDirPowMaxDemandLm_byMPNo(ucMPNo,0,ucFeeNo);
//		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usDILabel,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
//	}
//	if(ucRet==RET_SUCCESS)
//	{
//		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//			Fill_GRTaskData_Val(i,usDILabel,CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_BLOCK,0,(unsigned char)(ucFeeNo*3),3);
//	}
//	return ucRet;
//}
///************************************正向有功最大需量发生时间********************************************/
//unsigned char Fill_GRTaskData_DirPowMaxDemandTime(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	STHISTIME	  stHisTime;
//	unsigned char ucFeeNo;
//
//	ucFeeNo=usDILabel-CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME;
//	if(ucTaskSour==CONRS485SOUR_GRTASK)
//		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
//	else
//	{
//		stHisTime	= Get_History_DirPowMaxDemandTime_byMPNo(ucMPNo,0,ucFeeNo);
//		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usDILabel,(unsigned char*)&stHisTime.stHisTime,stHisTime.stDateTime);
//	}
//	if(ucRet==RET_SUCCESS)
//	{	
//		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//			Fill_GRTaskData_Val(i,usDILabel,CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_BLOCK,0,(unsigned char)(ucFeeNo*4),4);
//	}
//	return ucRet;
//}
///************************************上月正向有功最大需量发生时间****************************************/
//unsigned char Fill_GRTaskData_DirPowMaxDemandTime_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	STHISTIME	  stHisTime;
//	unsigned char ucFeeNo;	
//
//	ucFeeNo=usDILabel-CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM;
//	if(ucTaskSour==CONRS485SOUR_GRTASK)
//		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
//	else
//	{
//		stHisTime	= Get_History_DirPowMaxDemandTimeLm_byMPNo(ucMPNo,0,ucFeeNo);
//		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usDILabel,(unsigned char*)&stHisTime.stHisTime,stHisTime.stDateTime);
//	}
//	if(ucRet==RET_SUCCESS)
//	{	
//		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//			Fill_GRTaskData_Val(i,usDILabel,CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_BLOCK,0,(unsigned char)(ucFeeNo*4),4);
//	}
//	return ucRet;
//}
///************************************反向有功最大需量发生时间****************************************/
//unsigned char Fill_GRTaskData_UnDirPowMaxDemandTime(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	STHISTIME	  stHisTime;
//	unsigned char ucFeeNo;
//
//	ucFeeNo=usDILabel-CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME;
//	if(ucTaskSour==CONRS485SOUR_GRTASK)
//		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
//	else
//	{
//		stHisTime	= Get_History_UnDirPowMaxDemandTime_byMPNo(ucMPNo,0,ucFeeNo);
//		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usDILabel,(unsigned char*)&stHisTime.stHisTime,stHisTime.stDateTime);
//	}
//	if(ucRet==RET_SUCCESS)
//	{	
//		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//			Fill_GRTaskData_Val(i,usDILabel,CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_BLOCK,0,(unsigned char)(ucFeeNo*4),4);
//	}
//	return ucRet;
//}
///************************************上月反向有功最大需量发生时间****************************************/
//unsigned char Fill_GRTaskData_UnDirPowMaxDemandTime_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	STHISTIME	  stHisTime;
//	unsigned char ucFeeNo;
//
//	ucFeeNo=usDILabel-CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM;
//	if(ucTaskSour==CONRS485SOUR_GRTASK)
//		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
//	else
//	{
//		stHisTime	= Get_History_UnDirPowMaxDemandTimeLm_byMPNo(ucMPNo,0,ucFeeNo);
//		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usDILabel,(unsigned char*)&stHisTime.stHisTime,stHisTime.stDateTime);
//	}
//	if(ucRet==RET_SUCCESS)
//	{	
//		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//			Fill_GRTaskData_Val(i,usDILabel,CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_BLOCK,0,(unsigned char)(ucFeeNo*4),4);
//	}
//	return ucRet;
//}
///************************************电表常数****************************************/
//unsigned char Fill_GRTaskData_PowConst(unsigned char ucMPNo,unsigned char ucTaskSour)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	STHISVAL	  stHisVal;
//
//	if(ucTaskSour==CONRS485SOUR_GRTASK)
//		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
//	else
//	{
//		stHisVal	= Get_History_PowPulseConst_byMPNo(ucMPNo,0);
//		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,CON_DI_TYPE_MPPOWCONST,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
//	}
//	if(ucRet==RET_SUCCESS)
//	{	
//		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//			Fill_GRTaskData_Val(i,CON_DI_TYPE_MPPOWCONST,CON_DI_TYPE_MPCONST_BLOCK,0,0,3);
//	}
//	return ucRet;
//}
//unsigned char Fill_GRTaskData_VarConst(unsigned char ucMPNo,unsigned char ucTaskSour)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	STHISVAL	  stHisVal;
//
//	if(ucTaskSour==CONRS485SOUR_GRTASK)
//		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
//	else
//	{
//		stHisVal	= Get_History_VarPulseConst_byMPNo(ucMPNo,0);
//		ucRet = Set_FillInTask(ucMPNo,ucTaskSour,CON_DI_TYPE_MPVARCONST,(unsigned char*)&stHisVal.lVal,stHisVal.stDateTime);
//	}
//	if(ucRet==RET_SUCCESS)
//	{	
//		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//			Fill_GRTaskData_Val(i,CON_DI_TYPE_MPVARCONST,CON_DI_TYPE_MPCONST_BLOCK,0,3,3);
//	}
//	return ucRet;
//}
///************************************费率时段****************************************/
//unsigned char Fill_GRTaskData_FeeRate(unsigned char ucMPNo,unsigned char ucTaskSour)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0,j=0;
//	unsigned short usDataPtr=0;
//
//	if(ucTaskSour==CONRS485SOUR_GRTASK)
//	{
//		SetRS485Task(&stFillInTask,&stRS485RecvExpainVal);
//
//		for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//		{
//			usDataPtr = GetGlobeRecvBufPtr(i);
//			if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE1 )//费率1
//			{
//				if( stFillInTask.ucDataLen>=3 )
//				{
//					for(j=0;j<3;j++)
//						//stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL[j]=stFillInTask.ucDataArea[j];//azh
//						stGRDefineArea.stGRTaskRetVal.ucTaskData[usDataPtr+j]=stFillInTask.ucDataArea[j];
//				}else
//					//FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,3);
//					FillData_EE(stGRDefineArea.stGRTaskRetVal.ucTaskData+usDataPtr,3);
//			}
//
//			if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE2 )//费率2
//			{
//				if( stFillInTask.ucDataLen>=6 )
//				{
//					for(j=0;j<3;j++)
//						stGRDefineArea.stGRTaskRetVal.ucTaskData[usDataPtr+j]=stFillInTask.ucDataArea[j+3];
//				}else
//					FillData_EE(stGRDefineArea.stGRTaskRetVal.ucTaskData+usDataPtr,3);
//			}
//			if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE3 )//费率3
//			{
//				if( stFillInTask.ucDataLen>=9 )
//				{
//					for(j=0;j<3;j++)
//						stGRDefineArea.stGRTaskRetVal.ucTaskData[usDataPtr+j]=stFillInTask.ucDataArea[j+6];
//				}else
//					FillData_EE(stGRDefineArea.stGRTaskRetVal.ucTaskData+usDataPtr,3);
//			}
//			if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE4 )//费率4
//			{
//				if( stFillInTask.ucDataLen>=12 )
//				{
//					for(j=0;j<3;j++)
//						stGRDefineArea.stGRTaskRetVal.ucTaskData[usDataPtr+j]=stFillInTask.ucDataArea[j+9];
//				}else
//					FillData_EE(stGRDefineArea.stGRTaskRetVal.ucTaskData+usDataPtr,3);
//			}
//			if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE5 )//费率5
//			{
//				if( stFillInTask.ucDataLen>=15 )
//				{
//					for(j=0;j<3;j++)
//						stGRDefineArea.stGRTaskRetVal.ucTaskData[usDataPtr+j]=stFillInTask.ucDataArea[j+12];
//				}else
//					FillData_EE(stGRDefineArea.stGRTaskRetVal.ucTaskData+usDataPtr,3);
//			}
//			if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE6 )//费率6
//			{
//				if( stFillInTask.ucDataLen>=18 )
//				{
//					for(j=0;j<3;j++)
//						stGRDefineArea.stGRTaskRetVal.ucTaskData[usDataPtr+j]=stFillInTask.ucDataArea[j+15];
//				}else
//					FillData_EE(stGRDefineArea.stGRTaskRetVal.ucTaskData+usDataPtr,3);
//			}
//			if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE7 )//费率7
//			{
//				if( stFillInTask.ucDataLen>=21 )
//				{
//					for(j=0;j<3;j++)
//						stGRDefineArea.stGRTaskRetVal.ucTaskData[usDataPtr+j]=stFillInTask.ucDataArea[j+18];
//				}else
//					FillData_EE(stGRDefineArea.stGRTaskRetVal.ucTaskData+usDataPtr,3);
//			}
//			if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE8 )//费率8
//			{
//				if( stFillInTask.ucDataLen>=24 )
//				{
//					for(j=0;j<3;j++)
//						stGRDefineArea.stGRTaskRetVal.ucTaskData[usDataPtr+j]=stFillInTask.ucDataArea[j+21];
//				}else
//					FillData_EE(stGRDefineArea.stGRTaskRetVal.ucTaskData+usDataPtr,3);
//			}
//
//			if(	stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE_BLOCK )
//			{
//				if( stFillInTask.ucDataLen>=24 )
//				{
//					for( j=0;j<24;j++ )
//						stGRDefineArea.stGRTaskRetVal.ucTaskData[usDataPtr+j]=stFillInTask.ucDataArea[j];
//				}else
//				{
//					for( j=0;j<24; )
//					{
//						FillData_EE(stGRDefineArea.stGRTaskRetVal.ucTaskData+usDataPtr+j,3);
//						j+=3;
//					}
//					for( j=0;j<stFillInTask.ucDataLen;j++ )
//						stGRDefineArea.stGRTaskRetVal.ucTaskData[usDataPtr+j]=stFillInTask.ucDataArea[j];
//				}
//			}
//		}
//	}
//	return ucRet;
//}

unsigned char Set_FillInTask(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel,unsigned char *pucDIContent,STDATETIME stDateTime)
{
	unsigned char ucRet=RET_SUCCESS;
	STDIINFO	stDIInfo;
//	STDATETIME	stCurTime;

//	stCurTime	= GetCurrentTime();
//	if(SubDateTime(&(stCurTime),&(stDateTime)) <=8)
//	{
		stDIInfo	= Main_DI_Data_Query_Manage(usDILabel);

		SetRS485TaskPer(&stFillInTask,ucTaskSour,CON_SEQNO_NULL,CON_RS485_REPORT_TYPE_GET,usDILabel,ucMPNo,NULL);
		stFillInTask.ucDataLen = stDIInfo.ucContentLen;
		memcpy(stFillInTask.ucDataArea,pucDIContent,stFillInTask.ucDataLen);
//	}else
//		ucRet = RET_ERROR;
	return ucRet;
}

//unsigned char Fill_GRTaskData_SumInstantPow(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned char ucGroupNo)
//{//有功功率总加组
//	unsigned char ucRet=RET_ERROR;
////	unsigned char i=0;
////	unsigned short usID;
////	STHISSUMVALGROUP  stHisVal;
////	float fVal;
////	unsigned long ulInvalidVal=CON_GRTASK_DATA_INVALID;
////	if(ucGroupNo<CON_SUMGROUP_NUM)
////	{
////		usID=GetInstantPowDI_byGroupNo(ucGroupNo);
////		if(ucTaskSour!=CONRS485SOUR_GRTASK)
////		{
////			stHisVal	= Get_History_InstantPowSumVal(0);
////			fVal=stHisVal.stSumValGroup.fSumVal[ucGroupNo];
////			if(fVal!=CONINVALIDVAL)
////				ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usID,(unsigned char*)&(fVal),stHisVal.stDateTime);
////			else
////				ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usID,(unsigned char*)&(ulInvalidVal),stHisVal.stDateTime);
////		}
////		if(ucRet==RET_SUCCESS)
////		{
////			for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
////				Fill_GRTaskData_Val_Direct(i,usID,CON_DI_TYPE_INVALID,0,0,4);
////		}
////	}
//	return ucRet;
//}
//
//unsigned char Fill_GRTaskData_SumInstantVar(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned char ucGroupNo)
//{//无功功率总加组
//	unsigned char ucRet=RET_ERROR;
////	unsigned char i=0;
////	unsigned short usID;
////	STHISSUMVALGROUP  stHisVal;
////	float fVal;
////	unsigned long ulInvalidVal=CON_GRTASK_DATA_INVALID;
////	if(ucGroupNo<CON_SUMGROUP_NUM)
////	{
////		usID=GetInstantVarDI_byGroupNo(ucGroupNo);
////		if(ucTaskSour!=CONRS485SOUR_GRTASK)
////		{
////			stHisVal	= Get_History_InstantVarSumVal(0);
////			fVal=stHisVal.stSumValGroup.fSumVal[ucGroupNo];
////			if(fVal!=CONINVALIDVAL)
////				ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usID,(unsigned char*)&(fVal),stHisVal.stDateTime);
////			else
////				ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usID,(unsigned char*)&(ulInvalidVal),stHisVal.stDateTime);
////		}
////		if(ucRet==RET_SUCCESS)
////		{
////			for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
////				Fill_GRTaskData_Val_Direct(i,usID,CON_DI_TYPE_INVALID,0,0,4);
////		}
////	}
//	return ucRet;
//}
//
//unsigned char Fill_GRTaskData_SumPow(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned char ucGroupNo)
//{//有功总加组
//	unsigned char ucRet=RET_ERROR;
////	unsigned char i=0;
////	unsigned short usID;
////	STHISSUMVALGROUP  stHisVal;
////	float fVal;
////	unsigned long ulInvalidVal=CON_GRTASK_DATA_INVALID;
////	if(ucGroupNo<CON_SUMGROUP_NUM)
////	{
////		usID=GetPowDI_byGroupNo(ucGroupNo);
////		if(ucTaskSour!=CONRS485SOUR_GRTASK)
////		{
////			stHisVal	= Get_History_PowValSumVal(0,0);
////			fVal=stHisVal.stSumValGroup.fSumVal[ucGroupNo];
////			if(fVal!=CONINVALIDVAL)
////				ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usID,(unsigned char*)&(fVal),stHisVal.stDateTime);
////			else
////				ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usID,(unsigned char*)&(ulInvalidVal),stHisVal.stDateTime);
////		}
////		if(ucRet==RET_SUCCESS)
////		{
////			for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
////				Fill_GRTaskData_Val_Direct(i,usID,CON_DI_TYPE_INVALID,0,0,4);
////		}
////	}
//	return ucRet;
//}
//
//unsigned char Fill_GRTaskData_SumVar(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned char ucGroupNo)
//{//无功总加组
//	unsigned char ucRet=RET_ERROR;//,i=0;
////	unsigned short usID;
////	STHISSUMVALGROUP  stHisVal;
////	float fVal;
////	unsigned long ulInvalidVal=CON_GRTASK_DATA_INVALID;
////	if(ucGroupNo<CON_SUMGROUP_NUM)
////	{
////		usID=GetVarDI_byGroupNo(ucGroupNo);
////		if(ucTaskSour!=CONRS485SOUR_GRTASK)
////		{
////			stHisVal	= Get_History_VarValSumVal(0,0);
////			fVal=stHisVal.stSumValGroup.fSumVal[ucGroupNo];
////			if(fVal!=CONINVALIDVAL)
////				ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usID,(unsigned char*)&(fVal),stHisVal.stDateTime);
////			else
////				ucRet = Set_FillInTask(ucMPNo,ucTaskSour,usID,(unsigned char*)&(ulInvalidVal),stHisVal.stDateTime);
////		}
////		if(ucRet==RET_SUCCESS)
////		{
////			for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
////				Fill_GRTaskData_Val_Direct(i,usID,CON_DI_TYPE_INVALID,0,0,4);
////		}
////	}
//	return ucRet;
//}
//
unsigned short GetGlobeRecvBufPtr(unsigned ucIndex)
{
	unsigned char i=0;
	unsigned short usDataPtr=0;
	
	if(ucIndex < stGRDefineArea.stGRTaskRetVal.ucDINum)
	{    
    	for( i = 0 ; i < ucIndex ; i ++ )
    	{
    	   usDataPtr += stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDILen;
    	}
    }
    if(usDataPtr > CON_GRTASK_TaskDateMaxDateLen)
    {
        usDataPtr = CON_GRTASK_TaskDateMaxDateLen - 6;
    }        
    return usDataPtr;
}

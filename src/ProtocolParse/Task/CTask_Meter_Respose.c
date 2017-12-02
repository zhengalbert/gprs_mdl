/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : CTask_Meter_Respose.c
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-08-15    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

STRS485TASK	  stFillInTask;

//��Ч����
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
//��Ч����
//static unsigned char Fill_GRTaskData_DirPower_Invalid(void);
//static unsigned char Fill_GRTaskData_UnDirPower_Invalid(void);
//static unsigned char Fill_GRTaskData_DirVar_Invalid(void);
//static unsigned char Fill_GRTaskData_UnDirVar_Invalid(void);
//static unsigned char Fill_GRTaskData_I_Invalid(unsigned short usDI);
//static unsigned char Fill_GRTaskData_V_Invalid(unsigned short usDI);
//static unsigned char Fill_GRTaskData_InstantVal_Invalid(void);//רΪ�㽭��Լ
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
//azh ����������������ֱ�Ӵ�ȫ�����������ȡ���� ��û��ȥʵʱ���� ���Բ���Ҫ����Ч���� ��˵��ʼ��ʱ�Ѿ�Ĭ�������Ч����
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPDIRPOW_BLOCK:	//�����й�����
//		Fill_GRTaskData_DirPower_Invalid();
//		break;
//	case CON_DI_TYPE_MPUNDIRPOW_BLOCK:	//�����й�����
//		Fill_GRTaskData_UnDirPower_Invalid();
//		break;
//	case CON_DI_TYPE_MPDIRPOW_LM_BLOCK://�����й��������ݿ飨���£�
//		Fill_GRTaskData_DirPowerLM_Invalid();
//		break;
//	case CON_DI_TYPE_MPUNDIRPOW_LM_BLOCK://�����й��������ݿ飨���£�
//		Fill_GRTaskData_UnDirPowerLM_Invalid();
//		break;
//	case CON_DI_TYPE_MPDIRVAR_BLOCK:	//�����й�����
//		Fill_GRTaskData_DirVar_Invalid();
//		break;
//	case CON_DI_TYPE_MPUNDIRVAR_BLOCK:	//�����й�����
//		Fill_GRTaskData_UnDirVar_Invalid();
//		break;
///////////////////////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPVAR1_BLOCK:	//һ����
//		Fill_GRTaskData_DirVar1_Invalid();
//		break;
//	case CON_DI_TYPE_MPVAR4_BLOCK:	//������
//		Fill_GRTaskData_DirVar4_Invalid();	
//		break;
//	case CON_DI_TYPE_MPVAR2_BLOCK:	//������
//		Fill_GRTaskData_DirVar2_Invalid();
//		break;
//	case CON_DI_TYPE_MPVAR3_BLOCK:	//������
//		Fill_GRTaskData_DirVar3_Invalid();
//		break;
//
//	case CON_DI_TYPE_MPDIRVAR_LM_BLOCK://�����޹��������ݿ飨���£�
//		Fill_GRTaskData_DirVarLM_Invalid();
//		break;
//	case CON_DI_TYPE_MPUNDIRVAR_LM_BLOCK://�����޹��������ݿ飨���£�
//		Fill_GRTaskData_UnDirVarLM_Invalid();
//		break;
//	case CON_DI_TYPE_MPVAR1_LM_BLOCK://һ�����޹��ܵ��ܣ����£�
//		Fill_GRTaskData_DirVar1LM_Invalid();
//		break;
//	case CON_DI_TYPE_MPVAR2_LM_BLOCK://�������޹��ܵ��ܣ����£�
//		Fill_GRTaskData_DirVar2LM_Invalid();
//		break;
//	case CON_DI_TYPE_MPVAR3_LM_BLOCK://�������޹��ܵ��ܣ����£�
//		Fill_GRTaskData_DirVar3LM_Invalid();
//		break;
//	case CON_DI_TYPE_MPVAR4_LM_BLOCK://�������޹��ܵ��ܣ����£�
//		Fill_GRTaskData_DirVar4LM_Invalid();
//		break;
////////////////////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPVOL_BLOCK:
//		break;
//		//����Ĵ���ֻ��ȫ����Լ����
//	case CON_DI_TYPE_MPCURA:			//A����
//	case CON_DI_TYPE_MPCURB:			//B����
//	case CON_DI_TYPE_MPCURC:			//C����
//		Fill_GRTaskData_I_Invalid(usDILabel);
//		break;
//	case CON_DI_TYPE_MPVOLA:			//A��ѹ
//	case CON_DI_TYPE_MPVOLB:			//B��ѹ
//	case CON_DI_TYPE_MPVOLC:			//C��ѹ
//		Fill_GRTaskData_V_Invalid(usDILabel);
//		break;
//	case CON_DI_TYPE_MPINSTANTPOW:		//˲ʱ�й�����
//	case CON_DI_TYPE_MPINSTANTPOWA:		//A���й�����
//	case CON_DI_TYPE_MPINSTANTPOWB:		//B���й�����
//	case CON_DI_TYPE_MPINSTANTPOWC:		//C���й�����
//		Fill_GRTaskData_InstantPow_Invalid(usDILabel);
//		break;
//	case CON_DI_TYPE_MPINSTANTVAR:		//˲ʱ�޹�����
//	case CON_DI_TYPE_MPINSTANTVARA:		//A���޹�����
//	case CON_DI_TYPE_MPINSTANTVARB:		//B���޹�����
//	case CON_DI_TYPE_MPINSTANTVARC:		//C���޹�����
//		Fill_GRTaskData_InstantVar_Invalid(usDILabel);
//		break;
//////////////////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_BLOCK:			//�����й����������
//		Fill_GRTaskData_DiraPowMaxDemand_Invalid();
//		break;
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_BLOCK:		//�����й���������������£�
//		Fill_GRTaskData_DiraPowMaxDemandLM_Invalid();
//		break;
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_BLOCK:		//�����й��������������ʱ��
//		Fill_GRTaskData_DiraPowMaxDemandTime_Invalid();
//		break;
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_BLOCK:	//�����й��������������ʱ�䣨���£�
//		Fill_GRTaskData_DiraPowMaxDemandTimeLM_Invalid();
//		break;
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_BLOCK:			//�����й����������
//		Fill_GRTaskData_UnDiraPowMaxDemand_Invalid();
//		break;
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_BLOCK:		//�����й���������������£�
//		Fill_GRTaskData_UnDiraPowMaxDemandLM_Invalid();
//		break;
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_BLOCK:		//�����й��������������ʱ��
//		Fill_GRTaskData_UnDiraPowMaxDemandTime_Invalid();
//		break;
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_BLOCK:	//�����й��������������ʱ�䣨���£�
//		Fill_GRTaskData_UnDiraPowMaxDemandTimeLM_Invalid();
//		break;
///////////////////////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPPOWCONST:						//�й�����
//	case CON_DI_TYPE_MPVARCONST:						//�޹�����
//		Fill_GRTaskData_Const_Invalid(usDILabel);
//		break;
//	case CON_DI_TYPE_MPFEERATE_BLOCK://����	
//		Fill_GRTaskData_FeeRate_Invalid();
//		break;
//	default:
//		ucRet=RET_ERROR;
//		break;
//	}
	return ucRet;
}

/*******************************��Ч������д******************************************/
unsigned char Fill_TaskData_Manage(unsigned short usDILabel,unsigned char ucTaskSour,unsigned char ucMPNo)
{
	unsigned char	ucRet=RET_SUCCESS;
//	STDATETIME		stDateTime;

//	stDateTime	= GetCurrentTime();
	switch(usDILabel)
	{
	case	CON_DI_TYPE_MPDIRPOWTOTAL:		//�����й��ܵ�������
	case	CON_DI_TYPE_MPDIRPOWPLUS:		//�����й����������
	case	CON_DI_TYPE_MPDIRPOWAPEX:		//�����й����������
	case	CON_DI_TYPE_MPDIRPOWFLAT:		//�����й�ƽ��������
	case	CON_DI_TYPE_MPDIRPOWVALE:		//�����й��ȵ�������
		ucRet=Fill_GRTaskData_DirPower(ucMPNo,ucTaskSour,usDILabel);
		break;
		////////////////////////////////////////////////////////
	case	CON_DI_TYPE_MPDIRVARTOTAL:		//�����޹��ܵ�������
	case	CON_DI_TYPE_MPDIRVARPLUS:		//�����޹����������
	case	CON_DI_TYPE_MPDIRVARAPEX:		//�����޹����������
	case	CON_DI_TYPE_MPDIRVARFLAT:		//�����޹�ƽ��������
	case	CON_DI_TYPE_MPDIRVARVALE:		//�����޹��ȵ�������
		ucRet=Fill_GRTaskData_DirVar(ucMPNo,ucTaskSour,usDILabel);
		break;
		/////////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPDIRPOWTOTAL_LM:		//�����й��ܵ��ܣ����£�
//	case CON_DI_TYPE_MPDIRPOWPLUS_LM:		//�����й�����ܣ����£�
//	case CON_DI_TYPE_MPDIRPOWAPEX_LM:		//�����й�����ܣ����£�
//	case CON_DI_TYPE_MPDIRPOWFLAT_LM:		//�����й�ƽ���ܣ����£�
//	case CON_DI_TYPE_MPDIRPOWVALE_LM:		//�����й��ȵ��ܣ����£�
//		ucRet=Fill_GRTaskData_DirPower_Lm(ucMPNo,ucTaskSour,usDILabel);
//		break;
		/////////////////////////////////////////////////////////
	case	CON_DI_TYPE_MPUNDIRPOWTOTAL:	//�����й��ܵ�������
	case CON_DI_TYPE_MPUNDIRPOWPLUS:		//�����й����������
	case CON_DI_TYPE_MPUNDIRPOWAPEX:		//�����й����������
	case CON_DI_TYPE_MPUNDIRPOWFLAT:		//�����й�ƽ��������
	case CON_DI_TYPE_MPUNDIRPOWVALE:		//�����й��ȵ�������
		ucRet=Fill_GRTaskData_UnDirPower(ucMPNo,ucTaskSour,usDILabel);
		break;
		///////////////////////////////////////////////////////
	case	CON_DI_TYPE_MPUNDIRVARTOTAL:	//�����޹��ܵ�������
	case CON_DI_TYPE_MPUNDIRVARPLUS:		//�����޹����������
	case CON_DI_TYPE_MPUNDIRVARAPEX:		//�����޹����������
	case CON_DI_TYPE_MPUNDIRVARFLAT:		//�����޹�ƽ��������
	case CON_DI_TYPE_MPUNDIRVARVALE:		//�����޹��ȵ�������
		ucRet=Fill_GRTaskData_UnDirVar(ucMPNo,ucTaskSour,usDILabel);
		break;
		////////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPUNDIRPOWTOTAL_LM:	//�����й��ܵ��ܣ����£�
//	case CON_DI_TYPE_MPUNDIRPOWPLUS_LM:		//�����й�����ܣ����£�
//	case CON_DI_TYPE_MPUNDIRPOWAPEX_LM:		//�����й�����ܣ����£�
//	case CON_DI_TYPE_MPUNDIRPOWFLAT_LM:		//�����й�ƽ���ܣ����£�
//	case CON_DI_TYPE_MPUNDIRPOWVALE_LM:		//�����й��ȵ��ܣ����£�	
//		ucRet=Fill_GRTaskData_UnDirPower_Lm(ucMPNo,ucTaskSour,usDILabel);
//		break;
		////////////////////////////////////////////////////////
	case	CON_DI_TYPE_MPVAR1:				//1�����޹�����
	case CON_DI_TYPE_MPVAR1_FEE1:
	case CON_DI_TYPE_MPVAR1_FEE2:
	case CON_DI_TYPE_MPVAR1_FEE3:
	case CON_DI_TYPE_MPVAR1_FEE4:
		ucRet=Fill_GRTaskData_DirVar1(ucMPNo,ucTaskSour,usDILabel);
		break;
	case	CON_DI_TYPE_MPVAR2:				//2�����޹�����
	case	CON_DI_TYPE_MPVAR2_FEE1:		
	case	CON_DI_TYPE_MPVAR2_FEE2:		
	case	CON_DI_TYPE_MPVAR2_FEE3:		
	case	CON_DI_TYPE_MPVAR2_FEE4:		
		ucRet=Fill_GRTaskData_DirVar2(ucMPNo,ucTaskSour,usDILabel);
		break;
	case	CON_DI_TYPE_MPVAR3:				//3�����޹�����
	case	CON_DI_TYPE_MPVAR3_FEE1:		
	case	CON_DI_TYPE_MPVAR3_FEE2:		
	case	CON_DI_TYPE_MPVAR3_FEE3:		
	case	CON_DI_TYPE_MPVAR3_FEE4:		
		ucRet=Fill_GRTaskData_DirVar3(ucMPNo,ucTaskSour,usDILabel);
		break;
	case	CON_DI_TYPE_MPVAR4:				//4�����޹�����
	case	CON_DI_TYPE_MPVAR4_FEE1:		
	case	CON_DI_TYPE_MPVAR4_FEE2:		
	case	CON_DI_TYPE_MPVAR4_FEE3:		
	case	CON_DI_TYPE_MPVAR4_FEE4:		
		ucRet=Fill_GRTaskData_DirVar4(ucMPNo,ucTaskSour,usDILabel);
		break;
		/////////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPDIRVARTOTAL_LM:		//�����޹��ܵ��ܣ����£�
//	case CON_DI_TYPE_MPDIRVARPLUS_LM:		//�����޹�����ܣ����£�
//	case CON_DI_TYPE_MPDIRVARAPEX_LM:		//�����޹�����ܣ����£�
//	case CON_DI_TYPE_MPDIRVARFLAT_LM:		//�����޹�ƽ���ܣ����£�
//	case CON_DI_TYPE_MPDIRVARVALE_LM:		//�����޹��ȵ��ܣ����£�
//		ucRet=Fill_GRTaskData_DirVar_Lm(ucMPNo,ucTaskSour,usDILabel);
//		break;
		//////////////////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPUNDIRVARTOTAL_LM:	//�����޹��ܵ��ܣ����£�
//	case CON_DI_TYPE_MPUNDIRVARPLUS_LM:		//�����޹�����ܣ����£�
//	case CON_DI_TYPE_MPUNDIRVARAPEX_LM:		//�����޹�����ܣ����£�
//	case CON_DI_TYPE_MPUNDIRVARFLAT_LM:		//�����޹�ƽ���ܣ����£�
//	case CON_DI_TYPE_MPUNDIRVARVALE_LM:		//�����޹��ȵ��ܣ����£�	
//		ucRet=Fill_GRTaskData_UnDirVar_Lm(ucMPNo,ucTaskSour,usDILabel);
//		break;
		////////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPVAR1_LM:				//һ�����޹��ܵ��ܣ����£�
//	case CON_DI_TYPE_MPVAR1_LM_FEE1:
//	case CON_DI_TYPE_MPVAR1_LM_FEE2:
//	case CON_DI_TYPE_MPVAR1_LM_FEE3:
//	case CON_DI_TYPE_MPVAR1_LM_FEE4:
//		ucRet=Fill_GRTaskData_DirVar1_Lm(ucMPNo,ucTaskSour,usDILabel);
//		break;
//	case CON_DI_TYPE_MPVAR4_LM:				//�������޹��ܵ��ܣ����£�
//	case CON_DI_TYPE_MPVAR4_LM_FEE1:
//	case CON_DI_TYPE_MPVAR4_LM_FEE2:
//	case CON_DI_TYPE_MPVAR4_LM_FEE3:
//	case CON_DI_TYPE_MPVAR4_LM_FEE4:
//		ucRet=Fill_GRTaskData_DirVar4_Lm(ucMPNo,ucTaskSour,usDILabel);
//		break;
//	case CON_DI_TYPE_MPVAR2_LM:				//�������޹��ܵ��ܣ����£�
//	case CON_DI_TYPE_MPVAR2_LM_FEE1:
//	case CON_DI_TYPE_MPVAR2_LM_FEE2:
//	case CON_DI_TYPE_MPVAR2_LM_FEE3:
//	case CON_DI_TYPE_MPVAR2_LM_FEE4:
//		ucRet=Fill_GRTaskData_DirVar2_Lm(ucMPNo,ucTaskSour,usDILabel);
//		break;
//	case CON_DI_TYPE_MPVAR3_LM:				//�������޹��ܵ��ܣ����£�
//	case CON_DI_TYPE_MPVAR3_LM_FEE1:
//	case CON_DI_TYPE_MPVAR3_LM_FEE2:
//	case CON_DI_TYPE_MPVAR3_LM_FEE3:
//	case CON_DI_TYPE_MPVAR3_LM_FEE4:
//		ucRet=Fill_GRTaskData_DirVar3_Lm(ucMPNo,ucTaskSour,usDILabel);
//		break;
//		//////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND:		//�����й����������
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE1:
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE2:
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE3:
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE4:
//		ucRet=Fill_GRTaskData_DirPowMaxDemand(ucMPNo,ucTaskSour,usDILabel);
//		break;
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME:	//�����й��������������ʱ��
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE1:
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE2:
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE3:
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE4:
//		ucRet=Fill_GRTaskData_DirPowMaxDemandTime(ucMPNo,ucTaskSour,usDILabel);
//		break;
//		//////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND:	//�����й����������
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE1:
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE2:
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE3:
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE4:
//		ucRet=Fill_GRTaskData_UnDirPowMaxDemand(ucMPNo,ucTaskSour,usDILabel);
//		break;
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME://�����й��������������ʱ��
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE1:
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE2:
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE3:
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE4:
//		ucRet=Fill_GRTaskData_UnDirPowMaxDemandTime(ucMPNo,ucTaskSour,usDILabel);
//		break;
//		//////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM://�����й���������������£�
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE1:
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE2:
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE3:
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE4:
//		ucRet=Fill_GRTaskData_DirPowMaxDemand_Lm(ucMPNo,ucTaskSour,usDILabel);
//		break;
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM://�����й��������������ʱ�䣨���£�
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE1:
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE2:
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE3:
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE4:
//		ucRet=Fill_GRTaskData_DirPowMaxDemandTime_Lm(ucMPNo,ucTaskSour,usDILabel);
//		break;
//		//////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM://�����й���������������£�
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE1:
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE2:
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE3:
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE4:
//		ucRet=Fill_GRTaskData_UnDirPowMaxDemand_Lm(ucMPNo,ucTaskSour,usDILabel);
//		break;
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM://�����й��������������ʱ�䣨���£�
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE1:
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE2:
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE3:
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE4:
//		ucRet=Fill_GRTaskData_UnDirPowMaxDemandTime_Lm(ucMPNo,ucTaskSour,usDILabel);
//		break;
		//////////////////////////////////////////////////////////////////
	case	CON_DI_TYPE_MPCURA:				//A�����
		ucRet=Fill_GRTaskData_Ia(ucMPNo,ucTaskSour);
		break;
	case	CON_DI_TYPE_MPCURB:				//B�����
		ucRet=Fill_GRTaskData_Ib(ucMPNo,ucTaskSour);
		break;
	case	CON_DI_TYPE_MPCURC:				//C�����
		ucRet=Fill_GRTaskData_Ic(ucMPNo,ucTaskSour);
		break;
	case	CON_DI_TYPE_MPVOLA:				//A���ѹ
		ucRet=Fill_GRTaskData_Va(ucMPNo,ucTaskSour);
		break;
	case	CON_DI_TYPE_MPVOLB:				//B���ѹ
		ucRet=Fill_GRTaskData_Vb(ucMPNo,ucTaskSour);
		break;
	case	CON_DI_TYPE_MPVOLC:				//C���ѹ
		ucRet=Fill_GRTaskData_Vc(ucMPNo,ucTaskSour);
		break;
		//////////////////////////////////////////////////////////
	case CON_DI_TYPE_MPINSTANTPOW:		//˲ʱ�й�����
		ucRet=Fill_GRTaskData_InstantPow(ucMPNo,ucTaskSour);
		break;
	case CON_DI_TYPE_MPINSTANTPOWA:		//A���й�����
		ucRet=Fill_GRTaskData_InstantPowA(ucMPNo,ucTaskSour);
		break;
	case CON_DI_TYPE_MPINSTANTPOWB:		//B���й�����
		ucRet=Fill_GRTaskData_InstantPowB(ucMPNo,ucTaskSour);
		break;
	case CON_DI_TYPE_MPINSTANTPOWC:		//C���й�����
		ucRet=Fill_GRTaskData_InstantPowC(ucMPNo,ucTaskSour);
		break;
	case CON_DI_TYPE_MPINSTANTVAR:		//˲ʱ�޹�����
		ucRet=Fill_GRTaskData_InstantVar(ucMPNo,ucTaskSour);
		break;
	case CON_DI_TYPE_MPINSTANTVARA:		//A���޹�����
		ucRet=Fill_GRTaskData_InstantVarA(ucMPNo,ucTaskSour);
		break;
	case CON_DI_TYPE_MPINSTANTVARB:		//B���޹�����
		ucRet=Fill_GRTaskData_InstantVarB(ucMPNo,ucTaskSour);
		break;
	case CON_DI_TYPE_MPINSTANTVARC:		//C���޹�����
		ucRet=Fill_GRTaskData_InstantVarC(ucMPNo,ucTaskSour);
		break;
		////////////////////////////////////////////////////////////////
//	case CON_DI_TYPE_MPPOWCONST:		//�й�����
//		ucRet=Fill_GRTaskData_PowConst(ucMPNo,ucTaskSour);
//		break;
//	case CON_DI_TYPE_MPVARCONST:		//�޹�����
//		ucRet=Fill_GRTaskData_VarConst(ucMPNo,ucTaskSour);
//		break;
//	case CON_DI_TYPE_MPFEERATE_BLOCK:	//�������ݿ�
//		ucRet=Fill_GRTaskData_FeeRate(ucMPNo,ucTaskSour);
//		break;
//	case CON_DI_TYPE_SUMINSTANTPOW1:						//0x8F50//�й������ܼ���1
//		ucRet=Fill_GRTaskData_SumInstantPow(ucMPNo,ucTaskSour,0);
//		break;
//	case CON_DI_TYPE_SUMINSTANTPOW2:						//0x8F51//�й������ܼ���2
//		ucRet=Fill_GRTaskData_SumInstantPow(ucMPNo,ucTaskSour,1);
//		break;
//	case CON_DI_TYPE_SUMINSTANTPOW3:						//0x8F52//�й������ܼ���3
//		ucRet=Fill_GRTaskData_SumInstantPow(ucMPNo,ucTaskSour,2);
//		break;
//	case CON_DI_TYPE_SUMINSTANTPOW4:						//0x8F53//�й������ܼ���4
//		ucRet=Fill_GRTaskData_SumInstantPow(ucMPNo,ucTaskSour,3);
//		break;
//	case CON_DI_TYPE_SUMINSTANTPOW5:						//0x8F54//�й������ܼ���5
//		ucRet=Fill_GRTaskData_SumInstantPow(ucMPNo,ucTaskSour,4);
//		break;
//	case CON_DI_TYPE_SUMINSTANTPOW6:						//0x8F55//�й������ܼ���6
//		ucRet=Fill_GRTaskData_SumInstantPow(ucMPNo,ucTaskSour,5);
//		break;
//	case CON_DI_TYPE_SUMINSTANTPOW7:						//0x8F56//�й������ܼ���7
//		ucRet=Fill_GRTaskData_SumInstantPow(ucMPNo,ucTaskSour,6);
//		break;
//	case CON_DI_TYPE_SUMINSTANTPOW8:						//0x8F57//�й������ܼ���8
//		ucRet=Fill_GRTaskData_SumInstantPow(ucMPNo,ucTaskSour,7);
//		break;
//	case CON_DI_TYPE_SUMINSTANTVAR1:					//0x8F60//�޹������ܼ���1
//		ucRet=Fill_GRTaskData_SumInstantVar(ucMPNo,ucTaskSour,0);
//		break;
//	case CON_DI_TYPE_SUMINSTANTVAR2:					//0x8F61//�޹������ܼ���2
//		ucRet=Fill_GRTaskData_SumInstantVar(ucMPNo,ucTaskSour,1);
//		break;
//	case CON_DI_TYPE_SUMINSTANTVAR3:					//0x8F62//�޹������ܼ���3
//		ucRet=Fill_GRTaskData_SumInstantVar(ucMPNo,ucTaskSour,2);
//		break;
//	case CON_DI_TYPE_SUMINSTANTVAR4:					//0x8F63//�޹������ܼ���4
//		ucRet=Fill_GRTaskData_SumInstantVar(ucMPNo,ucTaskSour,3);
//		break;
//	case CON_DI_TYPE_SUMINSTANTVAR5:					//0x8F64//�޹������ܼ���5
//		ucRet=Fill_GRTaskData_SumInstantVar(ucMPNo,ucTaskSour,4);
//		break;
//	case CON_DI_TYPE_SUMINSTANTVAR6:					//0x8F65//�޹������ܼ���6
//		ucRet=Fill_GRTaskData_SumInstantVar(ucMPNo,ucTaskSour,5);
//		break;
//	case CON_DI_TYPE_SUMINSTANTVAR7:					//0x8F66//�޹������ܼ���7
//		ucRet=Fill_GRTaskData_SumInstantVar(ucMPNo,ucTaskSour,6);
//		break;
//	case CON_DI_TYPE_SUMINSTANTVAR8:					//0x8F67//�޹������ܼ���8
//		ucRet=Fill_GRTaskData_SumInstantVar(ucMPNo,ucTaskSour,7);
//		break;
//	case CON_DI_TYPE_SUMPOW1:							//0x8F70//�й��ܼ���1
//		ucRet=Fill_GRTaskData_SumPow(ucMPNo,ucTaskSour,0);
//		break;
//	case CON_DI_TYPE_SUMPOW2:							//0x8F71//�й��ܼ���2
//		ucRet=Fill_GRTaskData_SumPow(ucMPNo,ucTaskSour,1);
//		break;
//	case CON_DI_TYPE_SUMPOW3:							//0x8F72//�й��ܼ���3
//		ucRet=Fill_GRTaskData_SumPow(ucMPNo,ucTaskSour,2);
//		break;
//	case CON_DI_TYPE_SUMPOW4:							//0x8F73//�й��ܼ���4
//		ucRet=Fill_GRTaskData_SumPow(ucMPNo,ucTaskSour,3);
//		break;
//	case CON_DI_TYPE_SUMPOW5:							//0x8F74//�й��ܼ���5
//		ucRet=Fill_GRTaskData_SumPow(ucMPNo,ucTaskSour,4);
//		break;
//	case CON_DI_TYPE_SUMPOW6:							//0x8F75//�й��ܼ���6
//		ucRet=Fill_GRTaskData_SumPow(ucMPNo,ucTaskSour,5);
//		break;
//	case CON_DI_TYPE_SUMPOW7:							//0x8F76//�й��ܼ���7
//		ucRet=Fill_GRTaskData_SumPow(ucMPNo,ucTaskSour,6);
//		break;
//	case CON_DI_TYPE_SUMPOW8:							//0x8F77//�й��ܼ���8
//		ucRet=Fill_GRTaskData_SumPow(ucMPNo,ucTaskSour,7);
//		break;
//	case CON_DI_TYPE_SUMVAR1:						//0x8F80//�޹��ܼ���1
//		ucRet=Fill_GRTaskData_SumVar(ucMPNo,ucTaskSour,0);
//		break;
//	case CON_DI_TYPE_SUMVAR2:						//0x8F81//�޹��ܼ���2
//		ucRet=Fill_GRTaskData_SumVar(ucMPNo,ucTaskSour,1);
//		break;
//	case CON_DI_TYPE_SUMVAR3:						//0x8F82//�޹��ܼ���3
//		ucRet=Fill_GRTaskData_SumVar(ucMPNo,ucTaskSour,2);
//		break;
//	case CON_DI_TYPE_SUMVAR4:						//0x8F83//�޹��ܼ���4
//		ucRet=Fill_GRTaskData_SumVar(ucMPNo,ucTaskSour,3);
//		break;
//	case CON_DI_TYPE_SUMVAR5:						//0x8F84//�޹��ܼ���5
//		ucRet=Fill_GRTaskData_SumVar(ucMPNo,ucTaskSour,4);
//		break;
//	case CON_DI_TYPE_SUMVAR6:						//0x8F85//�޹��ܼ���6
//		ucRet=Fill_GRTaskData_SumVar(ucMPNo,ucTaskSour,5);
//		break;
//	case CON_DI_TYPE_SUMVAR7:						//0x8F86//�޹��ܼ���7
//		ucRet=Fill_GRTaskData_SumVar(ucMPNo,ucTaskSour,6);
//		break;
//	case CON_DI_TYPE_SUMVAR8:						//0x8F87//�޹��ܼ���8
//		ucRet=Fill_GRTaskData_SumVar(ucMPNo,ucTaskSour,7);
//		break;
	default:
		ucRet=RET_ERROR;
		break;
	}
	return ucRet;
}

//unsigned char Fill_GRTaskData_DirVar_Invalid()
//{//�����޹���������Ч��
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
//{//�����й���������Ч��
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
//{//�����й���������Ч�������£�
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
//{//�����޹���������Ч�������£�
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
//{//�����й���������Ч��
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
//{//�����й���������Ч��
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
//{//�����й���������Ч�������£�
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
//{//�����޹���������Ч�������£�
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
//{//רΪ�㽭��Լ
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
//{//��������Ч��
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==usDI )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,2);
//	}
//	return ucRet;
//}
//unsigned char Fill_GRTaskData_V_Invalid(unsigned short usDI)
//{//��ѹ����Ч��
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==usDI )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,2);
//	}
//	return ucRet;
//}
//unsigned char Fill_GRTaskData_DirVar1_Invalid()
//{//1�����޹�����Ч��
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
//{//2�����޹�����Ч��
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
//{//3�����޹�����Ч��
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
//{//4�����޹�����Ч��
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
//{//1�����޹�����Ч�������£�
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
//{//2�����޹�����Ч�������£�
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
//{//3�����޹�����Ч�������£�
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
//{//4�����޹�����Ч�������£�
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
//{//˲ʱ�й�����Ч��
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==usDI )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,3);
//	}
//	return ucRet;
//}
//unsigned char Fill_GRTaskData_InstantVar_Invalid(unsigned short usDI)
//{//˲ʱ�޹�����Ч��
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//	{
//		if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==usDI )
//			FillData_EE(stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL,2);
//	}
//	return ucRet;	
//}
//unsigned char Fill_GRTaskData_DiraPowMaxDemand_Invalid()
//{//�����й����������
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
//{//�����й���������������£�
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
//{//�����й����������ʱ��
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
//{//�����й����������ʱ�䣨���£�
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
//{//�����й����������
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
//{//�����й���������������£�
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
//{//�����й����������ʱ��
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
//{//�����й����������ʱ�䣨���£�
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
//{//����
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
//{//����	
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

//�޹�����
unsigned char Fill_GRTaskData_DirVar(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
{//�����޹���
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
{//�����޹���
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
/************************************�����й�����****************************************/
unsigned char Fill_GRTaskData_DirPower(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
{//�����й���
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
/************************************�����й�����****************************************/
unsigned char Fill_GRTaskData_UnDirPower(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
{//�����й���
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
///************************************���������й�����****************************************/
//unsigned char Fill_GRTaskData_DirPower_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
//{//�����й���
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
///************************************���·����й�����****************************************/
//unsigned char Fill_GRTaskData_UnDirPower_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
//{//�����й���
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
/************************************�������************************************************/
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
/************************************�����ѹ************************************************/
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
/************************************˲ʱ�й�����************************************************/
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
/************************************˲ʱ�޹�����************************************************/
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
/************************************һ�����޹�************************************************/
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
/************************************�������޹�************************************************/
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
/************************************�������޹�************************************************/
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
/************************************�������޹�************************************************/
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

///************************************���������޹�����****************************************/
//unsigned char Fill_GRTaskData_DirVar_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
//{//�����޹���
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
///************************************���·����޹�����****************************************/
//unsigned char Fill_GRTaskData_UnDirVar_Lm(unsigned char ucMPNo,unsigned char ucTaskSour,unsigned short usDILabel)
//{//�����޹���
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
///************************************����һ�����޹�************************************************/
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
///************************************���¶������޹�************************************************/
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
///************************************�����������޹�************************************************/
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
///************************************�����������޹�************************************************/
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
///************************************�����й�����************************************************/
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
///************************************���������й�����********************************************/
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
///************************************�����й�����********************************************/
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
///************************************���·����й�����********************************************/
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
///************************************�����й������������ʱ��********************************************/
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
///************************************���������й������������ʱ��****************************************/
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
///************************************�����й������������ʱ��****************************************/
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
///************************************���·����й������������ʱ��****************************************/
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
///************************************�����****************************************/
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
///************************************����ʱ��****************************************/
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
//			if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE1 )//����1
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
//			if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE2 )//����2
//			{
//				if( stFillInTask.ucDataLen>=6 )
//				{
//					for(j=0;j<3;j++)
//						stGRDefineArea.stGRTaskRetVal.ucTaskData[usDataPtr+j]=stFillInTask.ucDataArea[j+3];
//				}else
//					FillData_EE(stGRDefineArea.stGRTaskRetVal.ucTaskData+usDataPtr,3);
//			}
//			if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE3 )//����3
//			{
//				if( stFillInTask.ucDataLen>=9 )
//				{
//					for(j=0;j<3;j++)
//						stGRDefineArea.stGRTaskRetVal.ucTaskData[usDataPtr+j]=stFillInTask.ucDataArea[j+6];
//				}else
//					FillData_EE(stGRDefineArea.stGRTaskRetVal.ucTaskData+usDataPtr,3);
//			}
//			if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE4 )//����4
//			{
//				if( stFillInTask.ucDataLen>=12 )
//				{
//					for(j=0;j<3;j++)
//						stGRDefineArea.stGRTaskRetVal.ucTaskData[usDataPtr+j]=stFillInTask.ucDataArea[j+9];
//				}else
//					FillData_EE(stGRDefineArea.stGRTaskRetVal.ucTaskData+usDataPtr,3);
//			}
//			if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE5 )//����5
//			{
//				if( stFillInTask.ucDataLen>=15 )
//				{
//					for(j=0;j<3;j++)
//						stGRDefineArea.stGRTaskRetVal.ucTaskData[usDataPtr+j]=stFillInTask.ucDataArea[j+12];
//				}else
//					FillData_EE(stGRDefineArea.stGRTaskRetVal.ucTaskData+usDataPtr,3);
//			}
//			if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE6 )//����6
//			{
//				if( stFillInTask.ucDataLen>=18 )
//				{
//					for(j=0;j<3;j++)
//						stGRDefineArea.stGRTaskRetVal.ucTaskData[usDataPtr+j]=stFillInTask.ucDataArea[j+15];
//				}else
//					FillData_EE(stGRDefineArea.stGRTaskRetVal.ucTaskData+usDataPtr,3);
//			}
//			if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE7 )//����7
//			{
//				if( stFillInTask.ucDataLen>=21 )
//				{
//					for(j=0;j<3;j++)
//						stGRDefineArea.stGRTaskRetVal.ucTaskData[usDataPtr+j]=stFillInTask.ucDataArea[j+18];
//				}else
//					FillData_EE(stGRDefineArea.stGRTaskRetVal.ucTaskData+usDataPtr,3);
//			}
//			if( stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI==CON_DI_TYPE_MPFEERATE8 )//����8
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
//{//�й������ܼ���
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
//{//�޹������ܼ���
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
//{//�й��ܼ���
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
//{//�޹��ܼ���
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

/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Alarm_Take_Response.c
 Description    : �澯�������ݳ������ؽ���
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-02-05    azh     the original version
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

///******************************�����ѹ���·�Ŀǰȫ����Լ��С������/��Ҳ���ɱ����������********/
////�澯���ݲ���
//unsigned char Fill_AlarmTake_Vol_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned long lVal=0;
//
//	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPVOLA:					//A���ѹ
//	case CON_DI_TYPE_MPVOLB:					//B���ѹ
//	case CON_DI_TYPE_MPVOLC:					//C���ѹ
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
//		break;
//	case CON_DI_TYPE_MPVOL_BLOCK:				//�����ѹ
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
///******************************����������·�Ŀǰȫ����Լ��С������/��Ҳ���ɱ����������********/
//unsigned char Fill_AlarmTake_Cur_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned long lVal=0;
//
//	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPCURA:					//A�����
//	case CON_DI_TYPE_MPCURB:					//B�����
//	case CON_DI_TYPE_MPCURC:					//C�����
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
//		break;
//	case CON_DI_TYPE_MPCUR_BLOCK:				//�������
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
///******************************˲ʱ�й����·�Ŀǰȫ����Լ��С������/��Ҳ���ɱ����������********/
//unsigned char Fill_AlarmTake_InstantPow_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned long lVal=0;
//
//	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPINSTANTPOW:					//˲ʱ�й�
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
//		break;
//	case CON_DI_TYPE_MPINSTANTPOW_BLOCK:			//˲ʱ�й����ݿ�
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
///******************************˲ʱ�޹����·�Ŀǰȫ����Լ��С������/��Ҳ���ɱ����������********/
//unsigned char Fill_AlarmTake_InstantVar_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned long lVal=0;
//
//	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
//	switch(usDILabel)
//	{
//	case CON_DI_TYPE_MPINSTANTVAR:					//˲ʱ�޹�
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
//		break;
//	case CON_DI_TYPE_MPINSTANTVAR_BLOCK:			//˲ʱ�޹����ݿ�
//	case CON_DI_TYPE_MPALLDI:
//		if(ucIsValid==CONALRTME_INVALID_FLAG)	
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,CON_DI_TYPE_MPINSTANTVAR);
//		break;
//	default: 
//		ucRet=RET_ERROR;
//	}
//	return ucRet;
//}

/******************************�����й��������·�Ŀǰȫ����Լ��С������/��Ҳ���ɱ����������********/
unsigned char Fill_AlarmTake_DirPower_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long lVal=0;

	lVal=BYTE_TO_LONG(stRS485RecvExpainVal.ucDataArea);
	switch(usDILabel)
	{
	case CON_DI_TYPE_MPDIRPOWTOTAL:					//�����й��ܵ���
	case CON_DI_TYPE_MPDIRPOWPLUS:					//�����й������
	case CON_DI_TYPE_MPDIRPOWAPEX:					//�����й������
	case CON_DI_TYPE_MPDIRPOWFLAT:					//�����й�ƽ����
	case CON_DI_TYPE_MPDIRPOWVALE:					//�����й��ȵ���
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
		else
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
		break;
	case CON_DI_TYPE_MPDIRPOW_BLOCK:				//�����й����ݿ�
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
	case CON_DI_TYPE_MPUNDIRPOWTOTAL:					//�����й��ܵ���
	case CON_DI_TYPE_MPUNDIRPOWPLUS:					//�����й������
	case CON_DI_TYPE_MPUNDIRPOWAPEX:					//�����й������
	case CON_DI_TYPE_MPUNDIRPOWFLAT:					//�����й�ƽ����
	case CON_DI_TYPE_MPUNDIRPOWVALE:					//�����й��ȵ���
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
		else
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
		break;
	case CON_DI_TYPE_MPUNDIRPOW_BLOCK:					//�����й����ݿ�
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
//	case CON_DI_TYPE_MPDIRPOWTOTAL_LM:					//�����й�����(����)
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
//		break;
//	case CON_DI_TYPE_MPDIRPOW_LM_BLOCK:					//�����й����ݿ�
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
	case CON_DI_TYPE_MPDIRVARTOTAL:					//�����޹��ܵ���
	case CON_DI_TYPE_MPDIRVARPLUS:					//�����޹������
	case CON_DI_TYPE_MPDIRVARAPEX:					//�����޹������
	case CON_DI_TYPE_MPDIRVARFLAT:					//�����޹�ƽ����
	case CON_DI_TYPE_MPDIRVARVALE:					//�����޹��ȵ���
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
		else
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
		break;
	case CON_DI_TYPE_MPDIRVAR_BLOCK:				//�����޹����ݿ�
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
	case CON_DI_TYPE_MPUNDIRVARTOTAL:					//�����޹��ܵ���
	case CON_DI_TYPE_MPUNDIRVARPLUS:					//�����޹������
	case CON_DI_TYPE_MPUNDIRVARAPEX:					//�����޹������
	case CON_DI_TYPE_MPUNDIRVARFLAT:					//�����޹�ƽ����
	case CON_DI_TYPE_MPUNDIRVARVALE:					//�����޹��ȵ���
		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
		else
			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
		break;
	case CON_DI_TYPE_MPUNDIRVAR_BLOCK:					//�����޹����ݿ�
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
//	case CON_DI_TYPE_MPVAR1:					//һ�����޹��ܵ���
//	case CON_DI_TYPE_MPVAR1_FEE1:					//һ�����޹������
//	case CON_DI_TYPE_MPVAR1_FEE2:					//һ�����޹������
//	case CON_DI_TYPE_MPVAR1_FEE3:					//һ�����޹�ƽ����
//	case CON_DI_TYPE_MPVAR1_FEE4:					//һ�����޹��ȵ���
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
//		break;
//	case CON_DI_TYPE_MPVAR1_BLOCK:					//һ�����޹����ݿ�
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
//	case CON_DI_TYPE_MPVAR2:						//�������޹��ܵ���
//	case CON_DI_TYPE_MPVAR2_FEE1:					//�������޹������
//	case CON_DI_TYPE_MPVAR2_FEE2:					//�������޹������
//	case CON_DI_TYPE_MPVAR2_FEE3:					//�������޹�ƽ����
//	case CON_DI_TYPE_MPVAR2_FEE4:					//�������޹��ȵ���
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
//		break;
//	case CON_DI_TYPE_MPVAR2_BLOCK:					//�������޹����ݿ�
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
//	case CON_DI_TYPE_MPVAR3:					//�������޹��ܵ���
//	case CON_DI_TYPE_MPVAR3_FEE1:					//�������޹������
//	case CON_DI_TYPE_MPVAR3_FEE2:					//�������޹������
//	case CON_DI_TYPE_MPVAR3_FEE3:					//�������޹�ƽ����
//	case CON_DI_TYPE_MPVAR3_FEE4:					//�������޹��ȵ���
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
//		break;
//	case CON_DI_TYPE_MPVAR3_BLOCK:					//�������޹����ݿ�
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
//	case CON_DI_TYPE_MPVAR4:					//�������޹��ܵ���
//	case CON_DI_TYPE_MPVAR4_FEE1:					//�������޹������
//	case CON_DI_TYPE_MPVAR4_FEE2:					//�������޹������
//	case CON_DI_TYPE_MPVAR4_FEE3:					//�������޹�ƽ����
//	case CON_DI_TYPE_MPVAR4_FEE4:					//�������޹��ȵ���
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
//		break;
//	case CON_DI_TYPE_MPVAR4_BLOCK:					//�������޹����ݿ�
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
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND:						//�����й����������
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE1:					//�����й����������
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE2:					//�����й����������
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE3:					//�����й�ƽ�������
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE4:					//�����й����������
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
//		break;
//	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_BLOCK:					//�����й�����������ݿ�
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
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME:						//�����й��������������ʱ��
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE1:					//�����й��������������ʱ��
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE2:					//�����й��������������ʱ��
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE3:					//�����й�ƽ�����������ʱ��
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE4:					//�����й��������������ʱ��
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stAviDateTime,usDILabel);
//		break;
//	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_BLOCK:					//�����й������������ʱ�����ݿ�
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
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND:						//�����й����������
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE1:					//�����й����������
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE2:					//�����й����������
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE3:					//�����й�ƽ�������
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE4:					//�����й����������
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
//		break;
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_BLOCK:					//�����й�����������ݿ�
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
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME:						//�����й��������������ʱ��
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE1:					//�����й��������������ʱ��
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE2:					//�����й��������������ʱ��
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE3:					//�����й�ƽ�����������ʱ��
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE4:					//�����й��������������ʱ��
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stDateTime,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_stTimeValue(ucMPNo,ucNum,stAviDateTime,usDILabel);
//		break;
//	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_BLOCK:					//�����й������������ʱ�����ݿ�
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
//	case CON_DI_TYPE_MPPOWCONST:					//�й����峣��
//	case CON_DI_TYPE_MPVARCONST:					//�޹����峣��
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,CONINVALIDVAL,usDILabel);
//		else
//			ucRet=Add_AlarmEnsureJudge_lValue(ucMPNo,ucNum,lVal,usDILabel);
//		break;
//	case CON_DI_TYPE_MPCONST_BLOCK:					//���峣��
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
//	case CON_DI_TYPE_MPFEERATE_BLOCK:				//�������ݿ�
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
//	case CON_DI_TYPE_MPLATELYPROGTIME:			//���ʱ��
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_ProgramTime(ucMPNo,ucNum,stDateTime);
//		else
//			ucRet=Add_AlarmEnsureJudge_ProgramTime(ucMPNo,ucNum,stAviDateTime);
//		break;
//	case CON_DI_TYPE_MPPROGTIMETIMES:			//��̴���
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_ProgramCount(ucMPNo,ucNum,CONINVALIDVAL);
//		else
//			ucRet=Add_AlarmEnsureJudge_ProgramCount(ucMPNo,ucNum,lVal);
//		break;
//	case CON_DI_TYPE_MPB21F_BLOCK:				//���ʱ��
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
//	case CON_DI_TYPE_MPMETERSTATFLAG:				//���״̬��
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,1)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_MeterStatFlag(ucMPNo,ucNum,CONINVALIDVAL);
//		else
//			ucRet=Add_AlarmEnsureJudge_MeterStatFlag(ucMPNo,ucNum,ucByte);
//		break;
//	case CON_DI_TYPE_MPB21F_BLOCK:				//���ʱ��
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
//	case CON_DI_TYPE_MPCOPYDAYTIME:				//������
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_CopyDayTime(ucMPNo,ucNum,stDateTime);
//		else
//			ucRet=Add_AlarmEnsureJudge_CopyDayTime(ucMPNo,ucNum,stAviDateTime);
//		break;
//	case CON_DI_TYPE_MPC11F_BLOCK:				//������
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
//	case CON_DI_TYPE_MPLOSTVOLNUM:				//���������
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_EE(stRS485RecvExpainVal.ucDataArea,4)==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_LostVolNum(ucMPNo,ucNum,CONINVALIDVAL);
//		else
//			ucRet=Add_AlarmEnsureJudge_LostVolNum(ucMPNo,ucNum,lVal);
//		break;
//	case CON_DI_TYPE_MPB31F_BLOCK:				//���������
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
//	case CON_DI_TYPE_MPDATEANDWEEK:				//������
//		if(ucIsValid==CONALRTME_INVALID_FLAG || Is_Data_AllEE(stRS485RecvExpainVal.ucDataArea,sizeof(STDATETIME))==RET_SUCCESS)
//			ucRet=Add_AlarmEnsureJudge_Date(ucMPNo,ucNum,stDateTime);
//		else
//			ucRet=Add_AlarmEnsureJudge_Date(ucMPNo,ucNum,stAviDateTime);
//		break;
//	case CON_DI_TYPE_MPTIME:					//ʱ����
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

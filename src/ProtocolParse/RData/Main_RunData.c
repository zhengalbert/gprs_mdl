/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_RunData.c
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-08-15    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

/****************************************运行数据初始化********************************************/
void RunData_Init_All(void)
{
	unsigned char i=0;
	unsigned char ucTmp[64];
	for(i=0;i<CON_METER_NUM;i++)
	{
		Init_MeterHisData(&(stMeterTimerVal[i]));
		//HdConvertWatchDog();						//喂狗
	}
//	Init_MeterSumValGroup(&(stMeterSumValGroup));
	//HdConvertWatchDog();						//喂狗
//	Init_PulseHisData_All();
//	Init_TerHisData_All(&stTerTimerVal);
	//HdConvertWatchDog();						//喂狗
	DataProc_Init_All();
//azh	Load_Inital_All();
	Alarm_Init_All();
	memset(ucTmp,0,64);
	//HdConvertWatchDog();						//喂狗
//	ADESetMiscData(0xbd01,ucTmp,64);		//电量
//	ADESetMiscData(0xbd02,ucTmp,64);		//需量
	CTask_Rebuild_Task();
}

void RunData_Init(void)
{
	unsigned char i=0;
	for(i=0;i<CON_METER_NUM;i++)
	{
		Init_MeterHisData(&(stMeterTimerVal[i]));
		//HdConvertWatchDog();						//喂狗
	}
//	Init_MeterSumValGroup(&(stMeterSumValGroup));
	//HdConvertWatchDog();						//喂狗
//	Init_PulseHisData();
	//HdConvertWatchDog();						//喂狗
	Init_TerHisData(&stTerTimerVal);
	//HdConvertWatchDog();						//喂狗
	DataProc_Init();
//	Load_Inital();
	//HdConvertWatchDog();						//喂狗
}

void RunData_Meter_Init(unsigned char ucMeterNo)
{
	if(ucMeterNo<CON_METER_NUM)
		Init_MeterHisData(&(stMeterTimerVal[ucMeterNo]));						//表当前数据初始化
}

//辅助函数
void ClearHisVal( STHISVAL *pstHisVal )
{
	if( pstHisVal!=NULL )
	{
		ClearDateTime( &(pstHisVal->stDateTime) );
		pstHisVal->lVal=CONINVALIDVAL;
		pstHisVal->ucSign=0;				//1为负，其他为正
	}
}

void ClearHisTime( STHISTIME *pstHisTime )
{
	if( pstHisTime!=NULL )
	{
		ClearDateTime( &(pstHisTime->stDateTime) );
		ClearDateTime( &(pstHisTime->stHisTime) );
	}
}

void ClearHisFeeRate( STHISFEERATE *pstHisFeeRate )
{
//	unsigned char i=0;
    
	if( pstHisFeeRate!=NULL )
	{
		ClearDateTime( &(pstHisFeeRate->stDateTime) );
		ClearFeeRateList(&(pstHisFeeRate->stFeeRateList));
	}
}

void ClearFeeRateList(STFEERATELIST *pstFeeRateList)
{
	unsigned char i=0;
	if( pstFeeRateList!=NULL )
	{
		pstFeeRateList->ucFeeRateNum=(unsigned char)CONINVALIDVAL;
		for(i=0;i<8;i++)
		{
			pstFeeRateList->stFeeRate[i].ucFeeRate[0]=0;
			pstFeeRateList->stFeeRate[i].ucFeeRate[1]=0;
			pstFeeRateList->stFeeRate[i].ucFeeRate[2]=0;
		}
	}
}

void ClearHisState(STHISSTATE *pstHisState )
{
	if( pstHisState!=NULL )
	{
		ClearDateTime( &(pstHisState->stDateTime) );
		pstHisState->ucStateFlag=(unsigned char)CONINVALIDVAL;
	}
}

//void ClearHisSumValGroup(STHISSUMVALGROUP *pstHisSumValGroup)
//{
//	if(pstHisSumValGroup != NULL)
//	{
//		ClearDateTime(&(pstHisSumValGroup->stDateTime));
//		SumVal_Initial_Per(&(pstHisSumValGroup->stSumValGroup));
//	}
//}

void Add_History_HisVal(STHISVAL	*pstHisVal,unsigned char ucHisValNum,unsigned long lVal)
{
	unsigned char i=0;
	for(i=0;i<ucHisValNum-1;i++)
		pstHisVal[ucHisValNum-1-i]=pstHisVal[ucHisValNum-2-i];
	pstHisVal[0].stDateTime=GetCurrentTime();
	pstHisVal[0].lVal=lVal;
}
void Add_History_HisTime(STHISTIME	*pstHisTime,unsigned char ucHisTimeNum,STDATETIME	stDateTime)
{
	unsigned char i=0;
	for(i=0;i<ucHisTimeNum-1;i++)
		pstHisTime[ucHisTimeNum-1-i] = pstHisTime[ucHisTimeNum-2-i];
	pstHisTime[0].stDateTime=GetCurrentTime();
	pstHisTime[0].stHisTime=stDateTime;
}
void Add_History_HisState(STHISSTATE	*pstHisState,unsigned char ucHisStateNum,unsigned char ucStateFlag)
{
	unsigned char i=0;
	for(i=0;i<ucHisStateNum-1;i++)
		pstHisState[ucHisStateNum-1-i] = pstHisState[ucHisStateNum-2-i];
	pstHisState[0].stDateTime=GetCurrentTime();
	pstHisState[0].ucStateFlag=ucStateFlag;
}

void Init_MeterHisData(STMETERTIMERVAL *pstMeterTimerVal)
{
	unsigned char i=0;
	//正向（反向）有功电量
//	for( i=0;i<CONHISDATANUM_ADDLASTVALIDONE;i++ )							//额外增加最近有效一点
//	{
//		ClearHisVal( &(pstMeterTimerVal->stHisDirPowTotal[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisDirPowFee1[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirPowFee2[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirPowFee3[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirPowFee4[i]) );
//		
//		ClearHisVal( &(pstMeterTimerVal->stHisRevPowTotal[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisRevPowFee1[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisRevPowFee2[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisRevPowFee3[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisRevPowFee4[i]) );
//	}
//	for( i=0;i<CONHISDATANUM;i++ )
//	{
//		//（上月）
//		ClearHisVal( &(pstMeterTimerVal->stHisDirPowLmTotal[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisDirPowLmFee1[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirPowLmFee2[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirPowLmFee3[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirPowLmFee4[i]) );
//		
//		ClearHisVal( &(pstMeterTimerVal->stHisRevPowLmTotal[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisRevPowLmFee1[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisRevPowLmFee2[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisRevPowLmFee3[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisRevPowLmFee4[i]) );
//	}
//	//正向（反向）无功电量
//	for( i=0;i<CONHISDATANUM_ADDLASTVALIDONE;i++ )
//	{
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVarTotal[i]));				//额外增加最近有效一点
//	}
//	for( i=0;i<CONHISDATANUM;i++ )
//	{
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVarFee1[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVarFee2[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVarFee3[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVarFee4[i]) );
//		
//		ClearHisVal( &(pstMeterTimerVal->stHisRevVarTotal[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisRevVarFee1[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisRevVarFee2[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisRevVarFee3[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisRevVarFee4[i]) );
//		//（上月）
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVarLmTotal[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVarLmFee1[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVarLmFee2[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVarLmFee3[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVarLmFee4[i]) );
//		
//		ClearHisVal( &(pstMeterTimerVal->stHisRevVarLmTotal[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisRevVarLmFee1[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisRevVarLmFee2[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisRevVarLmFee3[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisRevVarLmFee4[i]) );
//
//		ClearHisVal(&(pstMeterTimerVal->stHisDirPowA[i]));
//		ClearHisVal(&(pstMeterTimerVal->stHisRevDirPowA[i]));
//		ClearHisVal(&(pstMeterTimerVal->stHisComVar1A[i]));
//		ClearHisVal(&(pstMeterTimerVal->stHisComVar2A[i]));
//
//		ClearHisVal(&(pstMeterTimerVal->stHisDirPowB[i]));
//		ClearHisVal(&(pstMeterTimerVal->stHisRevDirPowB[i]));
//		ClearHisVal(&(pstMeterTimerVal->stHisComVar1B[i]));
//		ClearHisVal(&(pstMeterTimerVal->stHisComVar2B[i]));
//
//		ClearHisVal(&(pstMeterTimerVal->stHisDirPowC[i]));
//		ClearHisVal(&(pstMeterTimerVal->stHisRevDirPowC[i]));
//		ClearHisVal(&(pstMeterTimerVal->stHisComVar1C[i]));
//		ClearHisVal(&(pstMeterTimerVal->stHisComVar2C[i]));
//
//		ClearHisVal(&(pstMeterTimerVal->stHisDirPowALm[i]));
//		ClearHisVal(&(pstMeterTimerVal->stHisRevDirPowALm[i]));
//		ClearHisVal(&(pstMeterTimerVal->stHisComVar1ALm[i]));
//		ClearHisVal(&(pstMeterTimerVal->stHisComVar2ALm[i]));
//
//		ClearHisVal(&(pstMeterTimerVal->stHisDirPowBLm[i]));
//		ClearHisVal(&(pstMeterTimerVal->stHisRevDirPowBLm[i]));
//		ClearHisVal(&(pstMeterTimerVal->stHisComVar1BLm[i]));
//		ClearHisVal(&(pstMeterTimerVal->stHisComVar2BLm[i]));
//
//		ClearHisVal(&(pstMeterTimerVal->stHisDirPowCLm[i]));
//		ClearHisVal(&(pstMeterTimerVal->stHisRevDirPowCLm[i]));
//		ClearHisVal(&(pstMeterTimerVal->stHisComVar1CLm[i]));
//		ClearHisVal(&(pstMeterTimerVal->stHisComVar2CLm[i]));
//	}
//	//1234象限无功电量
//	for( i=0;i<CONHISDATANUM;i++ )
//	{
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar1[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar1Fee1[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar1Fee2[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar1Fee3[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar1Fee4[i]) );
//		
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar2[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar2Fee1[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar2Fee2[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar2Fee3[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar2Fee4[i]) );
//		
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar3[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar3Fee1[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar3Fee2[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar3Fee3[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar3Fee4[i]) );
//		
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar4[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar4Fee1[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar4Fee2[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar4Fee3[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar4Fee4[i]) );
//		//（上月）
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar1Lm[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar1LmFee1[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar1LmFee2[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar1LmFee3[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar1LmFee4[i]) );
//		
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar2Lm[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar2LmFee1[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar2LmFee2[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar2LmFee3[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar2LmFee4[i]) );
//		
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar3Lm[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar3LmFee1[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar3LmFee2[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar3LmFee3[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar3LmFee4[i]) );
//		
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar4Lm[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar4LmFee1[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar4LmFee2[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar4LmFee3[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVar4LmFee4[i]) );
//	}
//	//电压、电流、相角、功率（电表）
//	for( i=0;i<CONHISDATANUM;i++ )
//	{
//		ClearHisVal( &(pstMeterTimerVal->stHisVolA[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisVolB[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisVolC[i]) );
//
//		ClearHisVal( &(pstMeterTimerVal->stHisVolA_Phase[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisVolB_Phase[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisVolC_Phase[i]) );
//		
//		ClearHisVal( &(pstMeterTimerVal->stHisIa[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisIb[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisIc[i]) );
//
//		ClearHisVal( &(pstMeterTimerVal->stHisIa_Phase[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisIb_Phase[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisIc_Phase[i]) );
//		
//		ClearHisVal( &(pstMeterTimerVal->stHisInstantPow[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisInstantPowA[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisInstantPowB[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisInstantPowC[i]) );
//		
//		ClearHisVal( &(pstMeterTimerVal->stHisInstantVar[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisInstantVarA[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisInstantVarB[i]) );
//		ClearHisVal( &(pstMeterTimerVal->stHisInstantVarC[i]) );
//	}
//	//需量及其发生时间
//	for( i=0;i<CONHISDATANUM;i++ )
//	{
//		ClearHisVal( &(pstMeterTimerVal->stHisDirPowMaxDemand[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisDirPowMaxDemandFee1[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisDirPowMaxDemandFee2[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisDirPowMaxDemandFee3[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisDirPowMaxDemandFee4[i]));
//		
//		ClearHisTime(&(pstMeterTimerVal->stHisDirPowMaxDemandTime[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisDirPowMaxDemandTimeFee1[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisDirPowMaxDemandTimeFee2[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisDirPowMaxDemandTimeFee3[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisDirPowMaxDemandTimeFee4[i]));
//		
//		ClearHisVal( &(pstMeterTimerVal->stHisRevPowMaxDemand[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisRevPowMaxDemandFee1[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisRevPowMaxDemandFee2[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisRevPowMaxDemandFee3[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisRevPowMaxDemandFee4[i]));
//		
//		ClearHisTime(&(pstMeterTimerVal->stHisRevPowMaxDemandTime[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisRevPowMaxDemandTimeFee1[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisRevPowMaxDemandTimeFee2[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisRevPowMaxDemandTimeFee3[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisRevPowMaxDemandTimeFee4[i]));
//		
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVarMaxDemand[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVarMaxDemandFee1[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVarMaxDemandFee2[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVarMaxDemandFee3[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVarMaxDemandFee4[i]));
//		
//		ClearHisTime(&(pstMeterTimerVal->stHisDirVarMaxDemandTime[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisDirVarMaxDemandTimeFee1[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisDirVarMaxDemandTimeFee2[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisDirVarMaxDemandTimeFee3[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisDirVarMaxDemandTimeFee4[i]));
//		
//		ClearHisVal( &(pstMeterTimerVal->stHisRevVarMaxDemand[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisRevVarMaxDemandFee1[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisRevVarMaxDemandFee2[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisRevVarMaxDemandFee3[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisRevVarMaxDemandFee4[i]));
//		
//		ClearHisTime(&(pstMeterTimerVal->stHisRevVarMaxDemandTime[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisRevVarMaxDemandTimeFee1[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisRevVarMaxDemandTimeFee2[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisRevVarMaxDemandTimeFee3[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisRevVarMaxDemandTimeFee4[i]));
//		//（上月）
//		ClearHisVal( &(pstMeterTimerVal->stHisDirPowMaxDemandLm[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisDirPowMaxDemandLmFee1[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisDirPowMaxDemandLmFee2[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisDirPowMaxDemandLmFee3[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisDirPowMaxDemandLmFee4[i]));
//		
//		ClearHisTime(&(pstMeterTimerVal->stHisDirPowMaxDemandTimeLm[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisDirPowMaxDemandTimeLmFee1[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisDirPowMaxDemandTimeLmFee2[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisDirPowMaxDemandTimeLmFee3[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisDirPowMaxDemandTimeLmFee4[i]));
//		
//		ClearHisVal( &(pstMeterTimerVal->stHisRevPowMaxDemandLm[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisRevPowMaxDemandLmFee1[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisRevPowMaxDemandLmFee2[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisRevPowMaxDemandLmFee3[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisRevPowMaxDemandLmFee4[i]));
//		
//		ClearHisTime(&(pstMeterTimerVal->stHisRevPowMaxDemandTimeLm[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisRevPowMaxDemandTimeLmFee1[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisRevPowMaxDemandTimeLmFee2[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisRevPowMaxDemandTimeLmFee3[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisRevPowMaxDemandTimeLmFee4[i]));
//		
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVarMaxDemandLm[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVarMaxDemandLmFee1[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVarMaxDemandLmFee2[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVarMaxDemandLmFee3[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisDirVarMaxDemandLmFee4[i]));
//		
//		ClearHisTime(&(pstMeterTimerVal->stHisDirVarMaxDemandTimeLm[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisDirVarMaxDemandTimeLmFee1[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisDirVarMaxDemandTimeLmFee2[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisDirVarMaxDemandTimeLmFee3[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisDirVarMaxDemandTimeLmFee4[i]));
//		
//		ClearHisVal( &(pstMeterTimerVal->stHisRevVarMaxDemandLm[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisRevVarMaxDemandLmFee1[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisRevVarMaxDemandLmFee2[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisRevVarMaxDemandLmFee3[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisRevVarMaxDemandLmFee4[i]));
//		
//		ClearHisTime(&(pstMeterTimerVal->stHisRevVarMaxDemandTimeLm[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisRevVarMaxDemandTimeLmFee1[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisRevVarMaxDemandTimeLmFee2[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisRevVarMaxDemandTimeLmFee3[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisRevVarMaxDemandTimeLmFee4[i]));
//	}
//	
//	//脉冲常数等
//	for( i=0;i<CONHISDATANUM;i++ )
//	{
//		ClearHisVal( &(pstMeterTimerVal->stHisPowPulseConst[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisVarPulseConst[i]));
//		
//		ClearHisFeeRate(&(pstMeterTimerVal->stHisFeeRate[i]));
//		
//		ClearHisTime(&(pstMeterTimerVal->stHisProgramTime[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisMeterClrTime[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisDemandClrTime[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisEventClrTime[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisJiaoshiTime[i]));
//		
//		ClearHisVal( &(pstMeterTimerVal->stHisProgramCount[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisMeterClrCount[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisDemandClrCount[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisEventClrCount[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisJiaoshiCount[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisBatWorkTime[i]));
//		
//		ClearHisState(&(pstMeterTimerVal->stHisMeterStatFlag1[i]));
//		ClearHisState(&(pstMeterTimerVal->stHisMeterStatFlag2[i]));
//		ClearHisState(&(pstMeterTimerVal->stHisMeterStatFlag3[i]));
//		ClearHisState(&(pstMeterTimerVal->stHisMeterStatFlag4[i]));
//		ClearHisState(&(pstMeterTimerVal->stHisMeterStatFlag5[i]));
//		ClearHisState(&(pstMeterTimerVal->stHisMeterStatFlag6[i]));
//		ClearHisState(&(pstMeterTimerVal->stHisMeterStatFlag7[i]));
//		
//		ClearHisTime(&(pstMeterTimerVal->stHisCopyDayTime[i]));
//		
//		ClearHisVal( &(pstMeterTimerVal->stHisLostVolNum[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisLostVolANum[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisLostVolBNum[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisLostVolCNum[i]));
//		
//		ClearHisVal( &(pstMeterTimerVal->stHisLostVolTime[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisLostVolATime[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisLostVolBTime[i]));
//		ClearHisVal( &(pstMeterTimerVal->stHisLostVolCTime[i]));
//		
//		ClearHisTime(&(pstMeterTimerVal->stHisLastVolOffBegTime[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisLastVolAOffBegTime[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisLastVolBOffBegTime[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisLastVolCOffBegTime[i]));
//		
//		ClearHisTime(&(pstMeterTimerVal->stHisLastVolOffEndTime[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisLastVolAOffEndTime[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisLastVolBOffEndTime[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisLastVolCOffEndTime[i]));
//		
//		ClearHisTime(&(pstMeterTimerVal->stHisDate[i]));
//		ClearHisTime(&(pstMeterTimerVal->stHisTime[i]));
//	}
    memset((unsigned char*)pstMeterTimerVal, (unsigned char)CONINVALIDVAL, sizeof(STMETERTIMERVAL));//有些符号位也被清成CONINVALIDVAL=0xBB了
	for( i=0;i<CONHISDATANUM;i++ )
	{
		ClearHisFeeRate(&(pstMeterTimerVal->stHisFeeRate[i]));
	}
}
/****************************************添加历史值**********************************************/
//电压
unsigned char Add_History_V(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo)
{
	switch(ucNo)
	{
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisVolA,CONHISDATANUM,lVal);break;
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisVolB,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisVolC,CONHISDATANUM,lVal);break;
	default:		break;
	}
	return RET_SUCCESS;
}
//电压相角
//unsigned char Add_History_V_Phase(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo)
//{
//	switch(ucNo)
//	{
//	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisVolA_Phase,CONHISDATANUM,lVal);break;
//	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisVolB_Phase,CONHISDATANUM,lVal);break;
//	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisVolC_Phase,CONHISDATANUM,lVal);break;
//	default:		break;
//	}
//	return RET_SUCCESS;
//}

//电流
unsigned char Add_History_I(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo)
{
	switch(ucNo)
	{
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisIa,CONHISDATANUM,lVal);break;
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisIb,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisIc,CONHISDATANUM,lVal);break;
	default:		break;
	}
	return RET_SUCCESS;
}

//电流相角
//unsigned char Add_History_I_Phase(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo)
//{
//	switch(ucNo)
//	{
//	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisIa_Phase,CONHISDATANUM,lVal);break;
//	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisIb_Phase,CONHISDATANUM,lVal);break;
//	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisIc_Phase,CONHISDATANUM,lVal);break;
//	default:		break;
//	}
//	return RET_SUCCESS;
//}

//瞬时有功
unsigned char Add_History_InstantPow(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo)
{
	switch(ucNo)
	{
	case 0: 	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisInstantPow,CONHISDATANUM,lVal);break;
	case 1: 	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisInstantPowA,CONHISDATANUM,lVal);break;
	case 2: 	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisInstantPowB,CONHISDATANUM,lVal);break;
	case 3: 	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisInstantPowC,CONHISDATANUM,lVal);break;
	}
	return RET_SUCCESS;
}
//瞬时无功
unsigned char Add_History_InstantVar(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo)
{
	switch(ucNo)
	{
	case 0: Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisInstantVar,CONHISDATANUM,lVal); break;
	case 1: Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisInstantVarA,CONHISDATANUM,lVal); break;
	case 2: Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisInstantVarB,CONHISDATANUM,lVal); break;
	case 3: Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisInstantVarC,CONHISDATANUM,lVal); break;
	}
	
	return RET_SUCCESS;
}
//正向有功电量
unsigned char Add_History_DirPower(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirPowTotal,CONHISDATANUM,lVal);break;
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirPowFee1,CONHISDATANUM,lVal);break;
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirPowFee2,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirPowFee3,CONHISDATANUM,lVal);break;
	case 4:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirPowFee4,CONHISDATANUM,lVal);break;
	default:		break;
	}
	return RET_SUCCESS;
}
//最近有效一点正向有功电量
unsigned char Add_History_DirPower_LastValid(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:
		stMeterTimerVal[ucMeterNo].stHisDirPowTotal[CONHISDATA_LASTVALID_POS].lVal=lVal;
		stMeterTimerVal[ucMeterNo].stHisDirPowTotal[CONHISDATA_LASTVALID_POS].stDateTime=GetCurrentTime();
		break;
	case 1:	
		stMeterTimerVal[ucMeterNo].stHisDirPowFee1[CONHISDATA_LASTVALID_POS].lVal=lVal;
		stMeterTimerVal[ucMeterNo].stHisDirPowFee1[CONHISDATA_LASTVALID_POS].stDateTime=GetCurrentTime();
		break;
	case 2:	
		stMeterTimerVal[ucMeterNo].stHisDirPowFee2[CONHISDATA_LASTVALID_POS].lVal=lVal;
		stMeterTimerVal[ucMeterNo].stHisDirPowFee2[CONHISDATA_LASTVALID_POS].stDateTime=GetCurrentTime();
		break;
	case 3:	
		stMeterTimerVal[ucMeterNo].stHisDirPowFee3[CONHISDATA_LASTVALID_POS].lVal=lVal;
		stMeterTimerVal[ucMeterNo].stHisDirPowFee3[CONHISDATA_LASTVALID_POS].stDateTime=GetCurrentTime();
		break;
	case 4:	
		stMeterTimerVal[ucMeterNo].stHisDirPowFee4[CONHISDATA_LASTVALID_POS].lVal=lVal;
		stMeterTimerVal[ucMeterNo].stHisDirPowFee4[CONHISDATA_LASTVALID_POS].stDateTime=GetCurrentTime();
		break;
	default:		
		break;
	}
	return RET_SUCCESS;
}

//反向有功电量
unsigned char Add_History_UnDirPower (unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevPowTotal,CONHISDATANUM,lVal);break;
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevPowFee1,CONHISDATANUM,lVal);break;
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevPowFee2,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevPowFee3,CONHISDATANUM,lVal);break;
	case 4:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevPowFee4,CONHISDATANUM,lVal);break;
	default:
		break;
	}
	return RET_SUCCESS;
}
//最近有效一点反向有功电量
unsigned char Add_History_UnDirPower_LastValid(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:
		stMeterTimerVal[ucMeterNo].stHisRevPowTotal[CONHISDATA_LASTVALID_POS].lVal=lVal;
		stMeterTimerVal[ucMeterNo].stHisRevPowTotal[CONHISDATA_LASTVALID_POS].stDateTime=GetCurrentTime();
		break;
	case 1:	
		stMeterTimerVal[ucMeterNo].stHisRevPowFee1[CONHISDATA_LASTVALID_POS].lVal=lVal;
		stMeterTimerVal[ucMeterNo].stHisRevPowFee1[CONHISDATA_LASTVALID_POS].stDateTime=GetCurrentTime();
		break;
	case 2:	
		stMeterTimerVal[ucMeterNo].stHisRevPowFee2[CONHISDATA_LASTVALID_POS].lVal=lVal;
		stMeterTimerVal[ucMeterNo].stHisRevPowFee2[CONHISDATA_LASTVALID_POS].stDateTime=GetCurrentTime();
		break;
	case 3:	
		stMeterTimerVal[ucMeterNo].stHisRevPowFee3[CONHISDATA_LASTVALID_POS].lVal=lVal;
		stMeterTimerVal[ucMeterNo].stHisRevPowFee3[CONHISDATA_LASTVALID_POS].stDateTime=GetCurrentTime();
		break;
	case 4:	
		stMeterTimerVal[ucMeterNo].stHisRevPowFee4[CONHISDATA_LASTVALID_POS].lVal=lVal;
		stMeterTimerVal[ucMeterNo].stHisRevPowFee4[CONHISDATA_LASTVALID_POS].stDateTime=GetCurrentTime();
		break;
	default:		
		break;
	}
	return RET_SUCCESS;
}

//正向无功电量
unsigned char Add_History_DirVar(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVarTotal,CONHISDATANUM,lVal);break;
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVarFee1,CONHISDATANUM,lVal);break;
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVarFee2,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVarFee3,CONHISDATANUM,lVal);break;
	case 4:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVarFee4,CONHISDATANUM,lVal);break;
	default:		break;
	}
	return RET_SUCCESS;
}
//最近有效一点正向无功电量
unsigned char Add_History_DirVar_LastValid(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:
		stMeterTimerVal[ucMeterNo].stHisDirVarTotal[CONHISDATA_LASTVALID_POS].lVal=lVal;
		stMeterTimerVal[ucMeterNo].stHisDirVarTotal[CONHISDATA_LASTVALID_POS].stDateTime=GetCurrentTime();
		break;
	default:		
		break;
	}
	return RET_SUCCESS;
}

//反向无功电量
unsigned char Add_History_UnDirVar(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevVarTotal,CONHISDATANUM,lVal);break;
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevVarFee1,CONHISDATANUM,lVal);break;
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevVarFee2,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevVarFee3,CONHISDATANUM,lVal);break;
	case 4:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevVarFee4,CONHISDATANUM,lVal);break;
	default:		break;
	}
	return RET_SUCCESS;
}

//一象限无功
unsigned char Add_History_DirVar1(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar1,CONHISDATANUM,lVal);break;
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar1Fee1,CONHISDATANUM,lVal);break;
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar1Fee2,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar1Fee3,CONHISDATANUM,lVal);break;
	case 4:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar1Fee4,CONHISDATANUM,lVal);break;
	default:		break;
	}
	return RET_SUCCESS;
}


//二象限无功
unsigned char Add_History_DirVar2(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar2,CONHISDATANUM,lVal);break;
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar2Fee1,CONHISDATANUM,lVal);break;
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar2Fee2,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar2Fee3,CONHISDATANUM,lVal);break;
	case 4:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar2Fee4,CONHISDATANUM,lVal);break;
	default:		break;
	}
	return RET_SUCCESS;
}

//三象限无功
unsigned char Add_History_DirVar3(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar3,CONHISDATANUM,lVal);break;
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar3Fee1,CONHISDATANUM,lVal);break;
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar3Fee2,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar3Fee3,CONHISDATANUM,lVal);break;
	case 4:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar3Fee4,CONHISDATANUM,lVal);break;
	default:		break;
	}
	return RET_SUCCESS;
}


//四象限无功
unsigned char Add_History_DirVar4(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar4,CONHISDATANUM,lVal);break;
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar4Fee1,CONHISDATANUM,lVal);break;
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar4Fee2,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar4Fee3,CONHISDATANUM,lVal);break;
	case 4:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar4Fee4,CONHISDATANUM,lVal);break;
	default:		break;
	}
	return RET_SUCCESS;
}

//正向有功最大需量
unsigned char Add_History_DirPowMaxDemand(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemand,CONHISDATANUM,lVal);break;
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandFee1,CONHISDATANUM,lVal);break;
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandFee2,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandFee3,CONHISDATANUM,lVal);break;
	case 4:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandFee4,CONHISDATANUM,lVal);break;
	default:		break;
	}
	return RET_SUCCESS;
}
//正向有功最大需量发生时间
unsigned char Add_History_DirPowMaxDemandTime(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandTime,CONHISDATANUM,stDateTime);break;
	case 1:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandTimeFee1,CONHISDATANUM,stDateTime);break;
	case 2:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandTimeFee2,CONHISDATANUM,stDateTime);break;
	case 3:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandTimeFee3,CONHISDATANUM,stDateTime);break;
	case 4:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandTimeFee4,CONHISDATANUM,stDateTime);break;
	default:		break;
	}
	return RET_SUCCESS;
}


//正向无功最大需量
unsigned char Add_History_DirVarMaxDemand(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemand,CONHISDATANUM,lVal);break;
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandFee1,CONHISDATANUM,lVal);break;
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandFee2,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandFee3,CONHISDATANUM,lVal);break;
	case 4:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandFee4,CONHISDATANUM,lVal);break;
	}
	return RET_SUCCESS;
}
//正向无功最大需量发生时间
unsigned char Add_History_DirVarMaxDemandTime(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandTime,CONHISDATANUM,stDateTime);break;
	case 1:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandTimeFee1,CONHISDATANUM,stDateTime);break;
	case 2:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandTimeFee2,CONHISDATANUM,stDateTime);break;
	case 3:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandTimeFee3,CONHISDATANUM,stDateTime);break;
	case 4:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandTimeFee4,CONHISDATANUM,stDateTime);break;
	}
	return RET_SUCCESS;
}

//反向有功最大需量
unsigned char Add_History_UnDirPowMaxDemand(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemand,CONHISDATANUM,lVal);break;
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandFee1,CONHISDATANUM,lVal);break;
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandFee2,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandFee3,CONHISDATANUM,lVal);break;
	case 4:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandFee4,CONHISDATANUM,lVal);break;
	default:		break;
	}
	return RET_SUCCESS;
}
//反向有功最大需量发生时间
unsigned char Add_History_UnDirPowMaxDemandTime(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandTime,CONHISDATANUM,stDateTime);break;
	case 1:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandTimeFee1,CONHISDATANUM,stDateTime);break;
	case 2:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandTimeFee2,CONHISDATANUM,stDateTime);break;
	case 3:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandTimeFee3,CONHISDATANUM,stDateTime);break;
	case 4:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandTimeFee4,CONHISDATANUM,stDateTime);break;
	default:		break;
	}
	return RET_SUCCESS;
}

//反向无功最大需量
unsigned char Add_History_UnDirVarMaxDemand(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemand,CONHISDATANUM,lVal);break;
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandFee1,CONHISDATANUM,lVal);break;
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandFee2,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandFee3,CONHISDATANUM,lVal);break;
	case 4:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandFee4,CONHISDATANUM,lVal);break;
	}
	return RET_SUCCESS;
}
//反向无功最大需量发生时间
unsigned char Add_History_UnDirVarMaxDemandTime(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandTime,CONHISDATANUM,stDateTime);break;
	case 1:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandTimeFee1,CONHISDATANUM,stDateTime);break;
	case 2:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandTimeFee2,CONHISDATANUM,stDateTime);break;
	case 3:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandTimeFee3,CONHISDATANUM,stDateTime);break;
	case 4:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandTimeFee4,CONHISDATANUM,stDateTime);break;
	}
	return RET_SUCCESS;
}
//正向有功电量(上月)
unsigned char Add_History_DirPowerLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirPowLmTotal,CONHISDATANUM,lVal);break;
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirPowLmFee1,CONHISDATANUM,lVal);break;
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirPowLmFee2,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirPowLmFee3,CONHISDATANUM,lVal);break;
	case 4:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirPowLmFee4,CONHISDATANUM,lVal);break;
	default:		break;
	}
	return RET_SUCCESS;
}

unsigned char Add_History_DirPowA(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo)
{
	switch(ucNo)
	{
		case 0:
			Add_History_HisVal( stMeterTimerVal[ucMeterNo].stHisDirPowA,CONHISDATANUM, lVal);break;
		case 1:
			Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevDirPowA, CONHISDATANUM, lVal);break;
		case 2:
			Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisComVar1A,CONHISDATANUM,lVal);break;
		case 3:
			Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisComVar2A, CONHISDATANUM, lVal);break;
	}
	return RET_SUCCESS;
}
unsigned char Add_History_DirPowB(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo)
{
	switch(ucNo)
	{
		case 0:
			Add_History_HisVal( stMeterTimerVal[ucMeterNo].stHisDirPowB,CONHISDATANUM, lVal);break;
		case 1:
			Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevDirPowB, CONHISDATANUM, lVal);break;
		case 2:
			Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisComVar1B,CONHISDATANUM,lVal);break;
		case 3:
			Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisComVar2B, CONHISDATANUM, lVal);break;
	}
	return RET_SUCCESS;
}
unsigned char Add_History_DirPowC(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo)
{
	switch(ucNo)
	{
		case 0:
			Add_History_HisVal( stMeterTimerVal[ucMeterNo].stHisDirPowC,CONHISDATANUM, lVal);break;
		case 1:
			Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevDirPowC, CONHISDATANUM, lVal);break;
		case 2:
			Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisComVar1C,CONHISDATANUM,lVal);break;
		case 3:
			Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisComVar2C, CONHISDATANUM, lVal);break;
	}
	return RET_SUCCESS;
}
unsigned char Add_History_DirPowALm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo)
{
	switch(ucNo)
	{
		case 0:
			Add_History_HisVal( stMeterTimerVal[ucMeterNo].stHisDirPowALm,CONHISDATANUM, lVal);break;
		case 1:
			Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevDirPowALm, CONHISDATANUM, lVal);break;
		case 2:
			Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisComVar1ALm,CONHISDATANUM,lVal);break;
		case 3:
			Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisComVar2ALm, CONHISDATANUM, lVal);break;
	}
	return RET_SUCCESS;
}
unsigned char Add_History_DirPowBLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo)
{
	switch(ucNo)
	{
		case 0:
			Add_History_HisVal( stMeterTimerVal[ucMeterNo].stHisDirPowBLm,CONHISDATANUM, lVal);break;
		case 1:
			Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevDirPowBLm, CONHISDATANUM, lVal);break;
		case 2:
			Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisComVar1BLm,CONHISDATANUM,lVal);break;
		case 3:
			Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisComVar2BLm, CONHISDATANUM, lVal);break;
	}
	return RET_SUCCESS;
}
unsigned char Add_History_DirPowCLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo)
{
	switch(ucNo)
	{
		case 0:
			Add_History_HisVal( stMeterTimerVal[ucMeterNo].stHisDirPowCLm,CONHISDATANUM, lVal);break;
		case 1:
			Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevDirPowCLm, CONHISDATANUM, lVal);break;
		case 2:
			Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisComVar1CLm,CONHISDATANUM,lVal);break;
		case 3:
			Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisComVar2CLm, CONHISDATANUM, lVal);break;
	}
	return RET_SUCCESS;
}

//反向有功电量(上月)
unsigned char Add_History_UnDirPowerLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevPowLmTotal,CONHISDATANUM,lVal);break;
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevPowLmFee1,CONHISDATANUM,lVal);break;
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevPowLmFee2,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevPowLmFee3,CONHISDATANUM,lVal);break;
	case 4:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevPowLmFee4,CONHISDATANUM,lVal);break;
	default:		break;
	}
	return RET_SUCCESS;
}

//正向无功电量(上月)
unsigned char Add_History_DirVarLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVarLmTotal,CONHISDATANUM,lVal);break;
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVarLmFee1,CONHISDATANUM,lVal);break;
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVarLmFee2,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVarLmFee3,CONHISDATANUM,lVal);break;
	case 4:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVarLmFee4,CONHISDATANUM,lVal);break;
	default:		break;
	}
	return RET_SUCCESS;
}

//反向无功电量(上月)
unsigned char Add_History_UnDirVarLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0: Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevVarLmTotal,CONHISDATANUM,lVal);break;
	case 1: Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevVarLmFee1,CONHISDATANUM,lVal);break;
	case 2: Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevVarLmFee2,CONHISDATANUM,lVal);break;
	case 3: Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevVarLmFee3,CONHISDATANUM,lVal);break;
	case 4: Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevVarLmFee4,CONHISDATANUM,lVal);break;
	default:		break;
	}
	return RET_SUCCESS;
}

//一象限无功(上月)
unsigned char Add_History_DirVar1Lm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar1Lm,CONHISDATANUM,lVal);break;
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar1LmFee1,CONHISDATANUM,lVal);break;
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar1LmFee2,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar1LmFee3,CONHISDATANUM,lVal);break;
	case 4:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar1LmFee4,CONHISDATANUM,lVal);break;
	default:		break;
	}
	return RET_SUCCESS;
}

//二象限无功(上月)
unsigned char Add_History_DirVar2Lm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar2Lm,CONHISDATANUM,lVal);break;
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar2LmFee1,CONHISDATANUM,lVal);break;
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar2LmFee2,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar2LmFee3,CONHISDATANUM,lVal);break;
	case 4:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar2LmFee4,CONHISDATANUM,lVal);break;
	default:		break;
	}
	return RET_SUCCESS;
}


//三象限无功(上月)
unsigned char Add_History_DirVar3Lm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar3Lm,CONHISDATANUM,lVal);break;
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar3LmFee1,CONHISDATANUM,lVal);break;
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar3LmFee2,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar3LmFee3,CONHISDATANUM,lVal);break;
	case 4:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar3LmFee4,CONHISDATANUM,lVal);break;
	default:		break;
	}
	return RET_SUCCESS;
}
//四象限无功(上月)
unsigned char Add_History_DirVar4Lm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar4Lm,CONHISDATANUM,lVal);break;
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar4LmFee1,CONHISDATANUM,lVal);break;	
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar4LmFee2,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar4LmFee3,CONHISDATANUM,lVal);break;
	case 4:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVar4LmFee4,CONHISDATANUM,lVal);break;
	}
	return RET_SUCCESS;
}

//正向有功最大需量(上月)
unsigned char Add_History_DirPowMaxDemandLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandLm,CONHISDATANUM,lVal);break;
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandLmFee1,CONHISDATANUM,lVal);break;
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandLmFee2,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandLmFee3,CONHISDATANUM,lVal);break;
	case 4:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandLmFee4,CONHISDATANUM,lVal);break;
	}
	return RET_SUCCESS;
}
//正向有功最大需量发生时间(上月)
unsigned char Add_History_DirPowMaxDemandTimeLm(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandTimeLm,CONHISDATANUM,stDateTime);break;
	case 1:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandTimeLmFee1,CONHISDATANUM,stDateTime);break;
	case 2:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandTimeLmFee2,CONHISDATANUM,stDateTime);break;
	case 3:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandTimeLmFee3,CONHISDATANUM,stDateTime);break;
	case 4:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandTimeLmFee4,CONHISDATANUM,stDateTime);break;
	}
	return RET_SUCCESS;
}

//正向无功最大需量(上月)
unsigned char Add_History_DirVarMaxDemandLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandLm,CONHISDATANUM,lVal);break;
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandLmFee1,CONHISDATANUM,lVal);break;
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandLmFee2,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandLmFee3,CONHISDATANUM,lVal);break;
	case 4:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandLmFee4,CONHISDATANUM,lVal);break;
	}
	return RET_SUCCESS;
}

//正向无功最大需量发生时间(上月)
unsigned char Add_History_DirVarMaxDemandTimeLm(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandTimeLm,CONHISDATANUM,stDateTime);break;
	case 1:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandTimeLmFee1,CONHISDATANUM,stDateTime);break;
	case 2:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandTimeLmFee2,CONHISDATANUM,stDateTime);break;
	case 3:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandTimeLmFee3,CONHISDATANUM,stDateTime);break;
	case 4:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandTimeLmFee4,CONHISDATANUM,stDateTime);break;
	}
	return RET_SUCCESS;
}
//反向有功最大需量(上月)
unsigned char Add_History_UnDirPowMaxDemandLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandLm,CONHISDATANUM,lVal);break;
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandLmFee1,CONHISDATANUM,lVal);break;
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandLmFee2,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandLmFee3,CONHISDATANUM,lVal);break;
	case 4:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandLmFee4,CONHISDATANUM,lVal);break;
	}
	return RET_SUCCESS;
}
//反向有功最大需量发生时间(上月)
unsigned char Add_History_UnDirPowMaxDemandTimeLm(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandTimeLm,CONHISDATANUM,stDateTime);break;
	case 1:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandTimeLmFee1,CONHISDATANUM,stDateTime);break;
	case 2:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandTimeLmFee2,CONHISDATANUM,stDateTime);break;
	case 3:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandTimeLmFee3,CONHISDATANUM,stDateTime);break;
	case 4:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandTimeLmFee4,CONHISDATANUM,stDateTime);break;
	}
	return RET_SUCCESS;
}

//反向无功最大需量(上月)
unsigned char Add_History_UnDirVarMaxDemandLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandLm,CONHISDATANUM,lVal);break;
	case 1:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandLmFee1,CONHISDATANUM,lVal);break;
	case 2:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandLmFee2,CONHISDATANUM,lVal);break;
	case 3:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandLmFee3,CONHISDATANUM,lVal);break;
	case 4:	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandLmFee4,CONHISDATANUM,lVal);break;
	}
	return RET_SUCCESS;
}
//反向无功最大需量发生时间(上月)
unsigned char Add_History_UnDirVarMaxDemandTimeLm(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucFeeNo)
{
	switch(ucFeeNo)
	{
	case 0:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandTimeLm,CONHISDATANUM,stDateTime);break;
	case 1:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandTimeLmFee1,CONHISDATANUM,stDateTime);break;
	case 2:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandTimeLmFee2,CONHISDATANUM,stDateTime);break;
	case 3:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandTimeLmFee3,CONHISDATANUM,stDateTime);break;
	case 4:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandTimeLmFee4,CONHISDATANUM,stDateTime);break;
	}
	return RET_SUCCESS;
}
//有功、无功脉冲常数
unsigned char Add_History_PowPulseConst(unsigned char ucMeterNo,unsigned long lVal)
{
	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisPowPulseConst,CONHISDATANUM,lVal);
	return RET_SUCCESS;
}
unsigned char Add_History_VarPulseConst(unsigned char ucMeterNo,unsigned long lVal)
{
	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisVarPulseConst,CONHISDATANUM,lVal);
	return RET_SUCCESS;
}

//费率
unsigned char Add_History_FeeRate(unsigned char ucMeterNo,STFEERATELIST	stFeeRateList)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	for(i=0;i<CONHISDATANUM-1;i++)
		stMeterTimerVal[ucMeterNo].stHisFeeRate[CONHISDATANUM-1-i]=stMeterTimerVal[ucMeterNo].stHisFeeRate[CONHISDATANUM-2-i];
	stMeterTimerVal[ucMeterNo].stHisFeeRate[0].stDateTime=GetCurrentTime();
	stMeterTimerVal[ucMeterNo].stHisFeeRate[0].stFeeRateList=stFeeRateList;
	return ucRet;
}
//编程时间
unsigned char Add_History_ProgramTime(unsigned char ucMeterNo,STDATETIME	stDateTime)
{
	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisProgramTime,CONHISDATANUM,stDateTime);
	return RET_SUCCESS;
}

//电表清零时间
unsigned char Add_History_MeterClrTime(unsigned char ucMeterNo,STDATETIME	stDateTime)
{
	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisMeterClrTime,CONHISDATANUM,stDateTime);
	return RET_SUCCESS;
}

//需量清零时间
unsigned char Add_History_DemandClrTime(unsigned char ucMeterNo,STDATETIME	stDateTime)
{
	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisDemandClrTime,CONHISDATANUM,stDateTime);
	return RET_SUCCESS;
}

//事件清零时间
unsigned char Add_History_EventClrTime(unsigned char ucMeterNo,STDATETIME	stDateTime)
{
	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisEventClrTime,CONHISDATANUM,stDateTime);
	return RET_SUCCESS;
}

//校时时间
unsigned char Add_History_JiaoshiTime(unsigned char ucMeterNo,STDATETIME	stDateTime)
{
	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisJiaoshiTime,CONHISDATANUM,stDateTime);
	return RET_SUCCESS;
}

//编程次数
unsigned char Add_History_ProgramCount(unsigned char ucMeterNo,unsigned long	lVal)
{
	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisProgramCount,CONHISDATANUM,lVal);
	return RET_SUCCESS;
}

//电表清零次数
unsigned char Add_History_MeterClrCount(unsigned char ucMeterNo,unsigned long	lVal)
{
	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisMeterClrCount,CONHISDATANUM,lVal);
	return RET_SUCCESS;
}

//需量清零次数
unsigned char Add_History_DemandClrCount(unsigned char ucMeterNo,unsigned long	lVal)
{
	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisDemandClrCount,CONHISDATANUM,lVal);
	return RET_SUCCESS;
}

//事件清零次数
unsigned char Add_History_EventClrCount(unsigned char ucMeterNo,unsigned long	lVal)
{
	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisEventClrCount,CONHISDATANUM,lVal);
	return RET_SUCCESS;
}

//校时次数
unsigned char Add_History_JiaoshiCount(unsigned char ucMeterNo,unsigned long	lVal)
{
	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisJiaoshiCount,CONHISDATANUM,lVal);
	return RET_SUCCESS;
}
//电池工作时间
unsigned char Add_History_BatWorkTime(unsigned char ucMeterNo,unsigned long	lVal)
{
	Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisBatWorkTime,CONHISDATANUM,lVal);
	return RET_SUCCESS;
}

//电表运行状态字
unsigned char Add_History_MeterStatFlag(unsigned char ucMeterNo,unsigned char	ucStateFlag,unsigned char ucFlagNum)
{
	switch(ucFlagNum)
	{
	case 1:
		Add_History_HisState(stMeterTimerVal[ucMeterNo].stHisMeterStatFlag1,CONHISDATANUM,ucStateFlag);
		break;
	case 2:
		Add_History_HisState(stMeterTimerVal[ucMeterNo].stHisMeterStatFlag2,CONHISDATANUM,ucStateFlag);
		break;
	case 3:
		Add_History_HisState(stMeterTimerVal[ucMeterNo].stHisMeterStatFlag3,CONHISDATANUM,ucStateFlag);
		break;
	case 4:
		Add_History_HisState(stMeterTimerVal[ucMeterNo].stHisMeterStatFlag4,CONHISDATANUM,ucStateFlag);
		break;
	case 5:
		Add_History_HisState(stMeterTimerVal[ucMeterNo].stHisMeterStatFlag5,CONHISDATANUM,ucStateFlag);
		break;
	case 6:
		Add_History_HisState(stMeterTimerVal[ucMeterNo].stHisMeterStatFlag6,CONHISDATANUM,ucStateFlag);
		break;
	case 7:
		Add_History_HisState(stMeterTimerVal[ucMeterNo].stHisMeterStatFlag7,CONHISDATANUM,ucStateFlag);
		break;
	}
	return RET_SUCCESS;
}

//电网运行状态字
//unsigned char Add_History_NetStatFlag(unsigned char ucMeterNo,unsigned char	ucStateFlag)
//{
//	Add_History_HisState(stMeterTimerVal[ucMeterNo].stHisMeterStatFlag2,CONHISDATANUM,ucStateFlag);
//	return RET_SUCCESS;
//}

//抄表日
unsigned char Add_History_CopyDayTime(unsigned char ucMeterNo,STDATETIME	stDateTime)
{
	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisCopyDayTime,CONHISDATANUM,stDateTime);
	return RET_SUCCESS;
}

//断相次数
unsigned char Add_History_LostVolNum(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo)
{
	switch( ucNo )
	{
	case 0:Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisLostVolNum,CONHISDATANUM,lVal);
		break;
	case 1:Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisLostVolANum,CONHISDATANUM,lVal);
		break;
	case 2:Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisLostVolBNum,CONHISDATANUM,lVal);
		break;
	case 3:Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisLostVolCNum,CONHISDATANUM,lVal);
		break;
	default:
		break;
	}
	return RET_SUCCESS;
}

//断相时间累计值
unsigned char Add_History_LostVolTime(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo)
{
	switch( ucNo )
	{
	case 0:Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisLostVolTime,CONHISDATANUM,lVal);
		break;
	case 1:Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisLostVolATime,CONHISDATANUM,lVal);
		break;
	case 2:Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisLostVolBTime,CONHISDATANUM,lVal);
		break;
	case 3:Add_History_HisVal(stMeterTimerVal[ucMeterNo].stHisLostVolCTime,CONHISDATANUM,lVal);
		break;
	default:
		break;
	}
	return RET_SUCCESS;
}

//最近一次断相起始时刻
unsigned char Add_History_LastVolOffBegTime(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucNo)
{
	switch(ucNo)
	{
	case 0:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisLastVolOffBegTime,CONHISDATANUM,stDateTime);break;
	case 1:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisLastVolAOffBegTime,CONHISDATANUM,stDateTime);break;
	case 2:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisLastVolBOffBegTime,CONHISDATANUM,stDateTime);break;
	case 3:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisLastVolCOffBegTime,CONHISDATANUM,stDateTime);break;
	}
	return RET_SUCCESS;
}


//最近一次断相结束时刻
unsigned char Add_History_LastVolOffEndTime(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucNo)
{
	switch(ucNo)
	{
	case 0:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisLastVolOffEndTime,CONHISDATANUM,stDateTime);break;
	case 1:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisLastVolAOffEndTime,CONHISDATANUM,stDateTime);break;
	case 2:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisLastVolBOffEndTime,CONHISDATANUM,stDateTime);break;
	case 3:	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisLastVolCOffEndTime,CONHISDATANUM,stDateTime);break;
	}
	return RET_SUCCESS;
}
//年月日
unsigned char Add_History_MeterDate(unsigned char ucMeterNo,STDATETIME	stDateTime)
{
	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisDate,CONHISDATANUM,stDateTime);
	return RET_SUCCESS;
}
//时分秒
unsigned char Add_History_MeterTime(unsigned char ucMeterNo,STDATETIME	stDateTime)
{
	Add_History_HisTime(stMeterTimerVal[ucMeterNo].stHisTime,CONHISDATANUM,stDateTime);
	return RET_SUCCESS;
}

/****************************************获取历史值**********************************************/
//正向有功电量
STHISVAL	Get_History_DirPower(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISVAL	stHisVal;
	ClearHisVal(&stHisVal);
	if(ucPos<CONHISDATANUM_ADDLASTVALIDONE && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisDirPowTotal[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisDirPowFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisDirPowFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisDirPowFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisDirPowFee4[ucPos];
		}
	}
	
	return(stHisVal);
}
//反向有功电量
STHISVAL Get_History_UnDirPower(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISVAL	stHisVal;
	ClearHisVal(&stHisVal);
	if(ucPos<CONHISDATANUM_ADDLASTVALIDONE && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisRevPowTotal[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisRevPowFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisRevPowFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisRevPowFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisRevPowFee4[ucPos];
		}
	}
	
	return(stHisVal);
}
//正向无功电量
STHISVAL Get_History_DirVar(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0: return stMeterTimerVal[ucMeterNo].stHisDirVarTotal[ucPos];
		case 1: return stMeterTimerVal[ucMeterNo].stHisDirVarFee1[ucPos];
		case 2: return stMeterTimerVal[ucMeterNo].stHisDirVarFee2[ucPos];
		case 3: return stMeterTimerVal[ucMeterNo].stHisDirVarFee3[ucPos];
		case 4: return stMeterTimerVal[ucMeterNo].stHisDirVarFee4[ucPos];
		}
	}
	else if(ucPos<CONHISDATANUM_ADDLASTVALIDONE && ucMeterNo < CON_METER_NUM)
	{
		if(ucFeeNo==0)
			return stMeterTimerVal[ucMeterNo].stHisDirVarTotal[ucPos];
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//反向无功电量
STHISVAL Get_History_UnDirVar(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisRevVarTotal[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisRevVarFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisRevVarFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisRevVarFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisRevVarFee4[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//1象限无功电量
STHISVAL Get_History_DirVar1(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisDirVar1[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisDirVar1Fee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisDirVar1Fee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisDirVar1Fee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisDirVar1Fee4[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//2象限无功电量
STHISVAL Get_History_DirVar2(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisDirVar2[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisDirVar2Fee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisDirVar2Fee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisDirVar2Fee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisDirVar2Fee4[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//3象限无功电量
STHISVAL Get_History_DirVar3(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisDirVar3[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisDirVar3Fee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisDirVar3Fee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisDirVar3Fee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisDirVar3Fee4[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//4象限无功电量
STHISVAL Get_History_DirVar4(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisDirVar4[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisDirVar4Fee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisDirVar4Fee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisDirVar4Fee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisDirVar4Fee4[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//正向有功最大需量
STHISVAL Get_History_DirPowMaxDemand(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemand[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandFee4[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//正向有功最大需量发生时间
STHISTIME Get_History_DirPowMaxDemandTime(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISTIME	stHisTime;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandTime[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandTimeFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandTimeFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandTimeFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandTimeFee4[ucPos];
		}
	}
	ClearHisTime(&stHisTime);
	return(stHisTime);
}
//正向无功最大需量
STHISVAL Get_History_DirVarMaxDemand(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemand[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandFee4[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//正向无功最大需量发生时间
STHISTIME Get_History_DirVarMaxDemandTime(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISTIME	stHisTime;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandTime[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandTimeFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandTimeFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandTimeFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandTimeFee4[ucPos];
		}
	}
	ClearHisTime(&stHisTime);
	return(stHisTime);
}
//反向有功最大需量
STHISVAL Get_History_UnDirPowMaxDemand(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemand[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandFee4[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//反向有功最大需量发生时间
STHISTIME Get_History_UnDirPowMaxDemandTime(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISTIME	stHisTime;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandTime[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandTimeFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandTimeFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandTimeFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandTimeFee4[ucPos];
		}
	}
	ClearHisTime(&stHisTime);
	return(stHisTime);
}
//反向无功最大需量
STHISVAL Get_History_UnDirVarMaxDemand(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemand[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandFee4[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//反向无功最大需量发生时间
STHISTIME Get_History_UnDirVarMaxDemandTime(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISTIME	stHisTime;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandTime[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandTimeFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandTimeFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandTimeFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandTimeFee4[ucPos];
		}
	}
	ClearHisTime(&stHisTime);
	return(stHisTime);
}
STHISVAL Get_History_DirPowA(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo)
{
	STHISVAL	stHisVal;

	if(ucPos < CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucNo)
		{
			case 0:return stMeterTimerVal[ucMeterNo].stHisDirPowA[ucPos];
			case 1:return stMeterTimerVal[ucMeterNo].stHisRevDirPowA[ucPos];
			case 2:return stMeterTimerVal[ucMeterNo].stHisComVar1A[ucPos];
			case 3:return stMeterTimerVal[ucMeterNo].stHisComVar2A[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return stHisVal;
}
STHISVAL Get_History_DirPowB(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo)
{
	STHISVAL	stHisVal;

	if(ucPos < CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucNo)
		{
			case 0:return stMeterTimerVal[ucMeterNo].stHisDirPowB[ucPos];
			case 1:return stMeterTimerVal[ucMeterNo].stHisRevDirPowB[ucPos];
			case 2:return stMeterTimerVal[ucMeterNo].stHisComVar1B[ucPos];
			case 3:return stMeterTimerVal[ucMeterNo].stHisComVar2B[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return stHisVal;
}
STHISVAL Get_History_DirPowC(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo)
{
	STHISVAL	stHisVal;

	if(ucPos < CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucNo)
		{
			case 0:return stMeterTimerVal[ucMeterNo].stHisDirPowC[ucPos];
			case 1:return stMeterTimerVal[ucMeterNo].stHisRevDirPowC[ucPos];
			case 2:return stMeterTimerVal[ucMeterNo].stHisComVar1C[ucPos];
			case 3:return stMeterTimerVal[ucMeterNo].stHisComVar2C[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return stHisVal;
}
STHISVAL Get_History_DirPowALm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo)
{
	STHISVAL	stHisVal;

	if(ucPos < CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucNo)
		{
			case 0:return stMeterTimerVal[ucMeterNo].stHisDirPowALm[ucPos];
			case 1:return stMeterTimerVal[ucMeterNo].stHisRevDirPowALm[ucPos];
			case 2:return stMeterTimerVal[ucMeterNo].stHisComVar1ALm[ucPos];
			case 3:return stMeterTimerVal[ucMeterNo].stHisComVar2ALm[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return stHisVal;
}
STHISVAL Get_History_DirPowBLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo)
{
	STHISVAL	stHisVal;

	if(ucPos < CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucNo)
		{
			case 0:return stMeterTimerVal[ucMeterNo].stHisDirPowBLm[ucPos];
			case 1:return stMeterTimerVal[ucMeterNo].stHisRevDirPowBLm[ucPos];
			case 2:return stMeterTimerVal[ucMeterNo].stHisComVar1BLm[ucPos];
			case 3:return stMeterTimerVal[ucMeterNo].stHisComVar2BLm[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return stHisVal;
}
STHISVAL Get_History_DirPowCLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo)
{
	STHISVAL	stHisVal;

	if(ucPos < CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucNo)
		{
			case 0:return stMeterTimerVal[ucMeterNo].stHisDirPowCLm[ucPos];
			case 1:return stMeterTimerVal[ucMeterNo].stHisRevDirPowCLm[ucPos];
			case 2:return stMeterTimerVal[ucMeterNo].stHisComVar1CLm[ucPos];
			case 3:return stMeterTimerVal[ucMeterNo].stHisComVar2CLm[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return stHisVal;
}
//正向有功电量(上月)
STHISVAL Get_History_DirPowerLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISVAL	stHisVal;
	ClearHisVal(&stHisVal);
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisDirPowLmTotal[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisDirPowLmFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisDirPowLmFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisDirPowLmFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisDirPowLmFee4[ucPos];
		}
	}
	
	return(stHisVal);
}
//反向有功电量(上月)
STHISVAL Get_History_UnDirPowerLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisRevPowLmTotal[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisRevPowLmFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisRevPowLmFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisRevPowLmFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisRevPowLmFee4[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//正向无功电量(上月)
STHISVAL Get_History_DirVarLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisDirVarLmTotal[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisDirVarLmFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisDirVarLmFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisDirVarLmFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisDirVarLmFee4[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//反向无功电量(上月)
STHISVAL Get_History_UnDirVarLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisRevVarLmTotal[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisRevVarLmFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisRevVarLmFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisRevVarLmFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisRevVarLmFee4[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//1象限无功电量(上月)
STHISVAL Get_History_DirVar1Lm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisDirVar1Lm[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisDirVar1LmFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisDirVar1LmFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisDirVar1LmFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisDirVar1LmFee4[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//2象限无功电量(上月)
STHISVAL Get_History_DirVar2Lm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisDirVar2Lm[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisDirVar2LmFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisDirVar2LmFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisDirVar2LmFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisDirVar2LmFee4[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//3象限无功电量(上月)
STHISVAL Get_History_DirVar3Lm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisDirVar3Lm[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisDirVar3LmFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisDirVar3LmFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisDirVar3LmFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisDirVar3LmFee4[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//4象限无功电量(上月)
STHISVAL Get_History_DirVar4Lm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisDirVar4Lm[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisDirVar4LmFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisDirVar4LmFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisDirVar4LmFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisDirVar4LmFee4[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//正向有功最大需量(上月)
STHISVAL Get_History_DirPowMaxDemandLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandLm[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandLmFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandLmFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandLmFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandLmFee4[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//正向有功最大需量发生时间(上月)
STHISTIME Get_History_DirPowMaxDemandTimeLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISTIME	stHisTime;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandTimeLm[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandTimeLmFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandTimeLmFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandTimeLmFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisDirPowMaxDemandTimeLmFee4[ucPos];
		}
	}
	ClearHisTime(&stHisTime);
	return stHisTime;
}
//正向无功最大需量(上月)
STHISVAL Get_History_DirVarMaxDemandLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandLm[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandLmFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandLmFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandLmFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandLmFee4[ucPos];			
		}
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//正向无功最大需量发生时间(上月)
STHISTIME Get_History_DirVarMaxDemandTimeLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISTIME	stHisTime;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandTimeLm[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandTimeLmFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandTimeLmFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandTimeLmFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisDirVarMaxDemandTimeLmFee4[ucPos];
		}
	}
	ClearHisTime(&stHisTime);
	return stHisTime;
}
//反向有功最大需量(上月)
STHISVAL Get_History_UnDirPowMaxDemandLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandLm[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandLmFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandLmFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandLmFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandLmFee4[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//反向有功最大需量发生时间(上月)
STHISTIME Get_History_UnDirPowMaxDemandTimeLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISTIME	stHisTime;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandTimeLm[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandTimeLmFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandTimeLmFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandTimeLmFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisRevPowMaxDemandTimeLmFee4[ucPos];
		}
	}
	ClearHisTime(&stHisTime);
	return stHisTime;
}
//反向无功最大需量(上月)
STHISVAL Get_History_UnDirVarMaxDemandLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandLm[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandLmFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandLmFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandLmFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandLmFee4[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//反向无功最大需量发生时间(上月)
STHISTIME Get_History_UnDirVarMaxDemandTimeLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo)
{
	STHISTIME	stHisTime;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFeeNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandTimeLm[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandTimeLmFee1[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandTimeLmFee2[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandTimeLmFee3[ucPos];
		case 4:	return stMeterTimerVal[ucMeterNo].stHisRevVarMaxDemandTimeLmFee4[ucPos];
		}
	}
	ClearHisTime(&stHisTime);
	return stHisTime;
}
//电流
STHISVAL Get_History_I(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucNo)
		{
		//		case 0:return stMeterTimerVal[ucMeterNo].stHisIn[ucPos];
		case 1:return stMeterTimerVal[ucMeterNo].stHisIa[ucPos];
		case 2:return stMeterTimerVal[ucMeterNo].stHisIb[ucPos];
		case 3:return stMeterTimerVal[ucMeterNo].stHisIc[ucPos];
		}
		
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//电流相角
STHISVAL Get_History_I_Phase(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucNo)
		{
		//		case 0:return stMeterTimerVal[ucMeterNo].stHisIn[ucPos];
		case 1:return stMeterTimerVal[ucMeterNo].stHisIa_Phase[ucPos];
		case 2:return stMeterTimerVal[ucMeterNo].stHisIb_Phase[ucPos];
		case 3:return stMeterTimerVal[ucMeterNo].stHisIc_Phase[ucPos];
		}
		
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//电压XXX.X
STHISVAL Get_History_V(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucNo)
		{
		case 1:return stMeterTimerVal[ucMeterNo].stHisVolA[ucPos];
		case 2:return stMeterTimerVal[ucMeterNo].stHisVolB[ucPos];
		case 3:return stMeterTimerVal[ucMeterNo].stHisVolC[ucPos];
		}
		
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//电压相角
STHISVAL Get_History_V_Phase(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucNo)
		{
		case 1:return stMeterTimerVal[ucMeterNo].stHisVolA_Phase[ucPos];
		case 2:return stMeterTimerVal[ucMeterNo].stHisVolB_Phase[ucPos];
		case 3:return stMeterTimerVal[ucMeterNo].stHisVolC_Phase[ucPos];
		}
		
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}

//瞬时有功、无功
STHISVAL Get_History_InstantPow(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucNo)
		{
		case 0:return stMeterTimerVal[ucMeterNo].stHisInstantPow[ucPos];
		case 1:return stMeterTimerVal[ucMeterNo].stHisInstantPowA[ucPos];
		case 2:return stMeterTimerVal[ucMeterNo].stHisInstantPowB[ucPos];
		case 3:return stMeterTimerVal[ucMeterNo].stHisInstantPowC[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
STHISVAL Get_History_InstantVar(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo)
{
	STHISVAL	stHisVal;
	ClearHisVal(&stHisVal);
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisInstantVar[ucPos];
		case 1:     return stMeterTimerVal[ucMeterNo].stHisInstantVarA[ucPos];
		case 2:     return stMeterTimerVal[ucMeterNo].stHisInstantVarB[ucPos];
		case 3:    return stMeterTimerVal[ucMeterNo].stHisInstantVarC[ucPos];
		}
	}
	
	return(stHisVal);
}
//有功、无功脉冲常数
STHISVAL Get_History_PowPulseConst(unsigned char ucMeterNo,unsigned char ucPos)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
		return stMeterTimerVal[ucMeterNo].stHisPowPulseConst[ucPos];
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
STHISVAL Get_History_VarPulseConst(unsigned char ucMeterNo,unsigned char ucPos)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
		return stMeterTimerVal[ucMeterNo].stHisVarPulseConst[ucPos];
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//费率
STHISFEERATE Get_History_FeeRate(unsigned char ucMeterNo,unsigned char ucPos)
{
	STHISFEERATE	stHisFeeRate;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
		return stMeterTimerVal[ucMeterNo].stHisFeeRate[ucPos];
	ClearHisFeeRate(&stHisFeeRate);
	return stHisFeeRate;
}
//编程时间
STHISTIME Get_History_ProgramTime(unsigned char ucMeterNo,unsigned char ucPos)
{
	STHISTIME	stHisTime;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
		return stMeterTimerVal[ucMeterNo].stHisProgramTime[ucPos];
	ClearHisTime(&stHisTime);
	return stHisTime;
}
//电表清零时间
STHISTIME Get_History_MeterClrTime(unsigned char ucMeterNo,unsigned char ucPos)
{
	STHISTIME	stHisTime;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
		return stMeterTimerVal[ucMeterNo].stHisMeterClrTime[ucPos];
	ClearHisTime(&stHisTime);
	return stHisTime;
}
//需量清零时间
STHISTIME Get_History_DemandClrTime(unsigned char ucMeterNo,unsigned char ucPos)
{
	STHISTIME	stHisTime;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
		return stMeterTimerVal[ucMeterNo].stHisDemandClrTime[ucPos];
	ClearHisTime(&stHisTime);
	return stHisTime;
}
//事件清零时间
STHISTIME Get_History_EventClrTime(unsigned char ucMeterNo,unsigned char ucPos)
{
	STHISTIME	stHisTime;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
		return stMeterTimerVal[ucMeterNo].stHisEventClrTime[ucPos];
	ClearHisTime(&stHisTime);
	return stHisTime;
}
//校时时间
STHISTIME Get_History_JiaoshiTime(unsigned char ucMeterNo,unsigned char ucPos)
{
	STHISTIME	stHisTime;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
		return stMeterTimerVal[ucMeterNo].stHisJiaoshiTime[ucPos];
	ClearHisTime(&stHisTime);
	return stHisTime;
}
//编程次数
STHISVAL Get_History_ProgramCount(unsigned char ucMeterNo,unsigned char ucPos)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
		return stMeterTimerVal[ucMeterNo].stHisProgramCount[ucPos];
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//电表清零次数
STHISVAL Get_History_MeterClrCount(unsigned char ucMeterNo,unsigned char ucPos)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
		return stMeterTimerVal[ucMeterNo].stHisMeterClrCount[ucPos];
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//需量清零次数
STHISVAL Get_History_DemandClrCount(unsigned char ucMeterNo,unsigned char ucPos)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
		return stMeterTimerVal[ucMeterNo].stHisDemandClrCount[ucPos];
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//事件清零次数
STHISVAL Get_History_EventClrCount(unsigned char ucMeterNo,unsigned char ucPos)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
		return stMeterTimerVal[ucMeterNo].stHisEventClrCount[ucPos];
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//校时次数
STHISVAL Get_History_JiaoshiCount(unsigned char ucMeterNo,unsigned char ucPos)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
		return stMeterTimerVal[ucMeterNo].stHisJiaoshiCount[ucPos];
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//电池工作时间
STHISVAL Get_History_BatWorkTime(unsigned char ucMeterNo,unsigned char ucPos)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
		return stMeterTimerVal[ucMeterNo].stHisBatWorkTime[ucPos];
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//电表运行状态字
STHISSTATE Get_History_MeterStatFlag(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFlagNum)
{
	STHISSTATE	stHisState;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucFlagNum)
		{
		case 1:
			stHisState = stMeterTimerVal[ucMeterNo].stHisMeterStatFlag1[ucPos];
			break;
		case 2:
			stHisState = stMeterTimerVal[ucMeterNo].stHisMeterStatFlag2[ucPos];
			break;
		case 3:
			stHisState = stMeterTimerVal[ucMeterNo].stHisMeterStatFlag3[ucPos];
			break;
		case 4:
			stHisState = stMeterTimerVal[ucMeterNo].stHisMeterStatFlag4[ucPos];
			break;
		case 5:
			stHisState = stMeterTimerVal[ucMeterNo].stHisMeterStatFlag5[ucPos];
			break;
		case 6:
			stHisState = stMeterTimerVal[ucMeterNo].stHisMeterStatFlag6[ucPos];
			break;
		case 7:
			stHisState = stMeterTimerVal[ucMeterNo].stHisMeterStatFlag7[ucPos];
			break;
		default:
			ClearHisState(&stHisState);
			break;
		}
	}
	else
		ClearHisState(&stHisState);
	return(stHisState);
}
//电网运行状态字
STHISSTATE Get_History_NetStatFlag(unsigned char ucMeterNo,unsigned char ucPos)
{
	STHISSTATE	stHisState;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
		return stMeterTimerVal[ucMeterNo].stHisMeterStatFlag2[ucPos];
	ClearHisState(&stHisState);
	return(stHisState);
}

//抄表日
STHISTIME Get_History_CopyDayTime(unsigned char ucMeterNo,unsigned char ucPos)
{
	STHISTIME	stHisTime;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
		return stMeterTimerVal[ucMeterNo].stHisCopyDayTime[ucPos];
	ClearHisTime(&stHisTime);
	return stHisTime;
}
//断相次数
STHISVAL Get_History_VolBreakNum(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucNo)
		{
		case 0:	return stMeterTimerVal[ucMeterNo].stHisLostVolNum[ucPos];
		case 1:return stMeterTimerVal[ucMeterNo].stHisLostVolANum[ucPos];
		case 2: return stMeterTimerVal[ucMeterNo].stHisLostVolBNum[ucPos];
		case 3: return stMeterTimerVal[ucMeterNo].stHisLostVolCNum[ucPos];
			
		}
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//断相时间累计值
STHISVAL Get_History_VolBreakTime(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo)
{
	STHISVAL	stHisVal;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
	{
		switch(ucNo)
		{
		case 0:		return stMeterTimerVal[ucMeterNo].stHisLostVolTime[ucPos];
		case 1:		return stMeterTimerVal[ucMeterNo].stHisLostVolATime[ucPos];
		case 2:		return stMeterTimerVal[ucMeterNo].stHisLostVolBTime[ucPos];
		case 3:		return stMeterTimerVal[ucMeterNo].stHisLostVolCTime[ucPos];
		}
	}
	ClearHisVal(&stHisVal);
	return(stHisVal);
}
//最近一次断相起始时刻
STHISTIME Get_History_LastVolBreakBegTime(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo)
{
	STHISTIME	stHisTime;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
		switch(ucNo)
	{
		case 0:		return stMeterTimerVal[ucMeterNo].stHisLastVolOffBegTime[ucPos];
		case 1:	return stMeterTimerVal[ucMeterNo].stHisLastVolAOffBegTime[ucPos];
		case 2:	return stMeterTimerVal[ucMeterNo].stHisLastVolBOffBegTime[ucPos];
		case 3:	return stMeterTimerVal[ucMeterNo].stHisLastVolCOffBegTime[ucPos];
	}
	
	
	
	ClearHisTime(&stHisTime);
	return stHisTime;
}
//最近一次断相结束时刻
STHISTIME Get_History_LastVolBreakEndTime(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo)
{
	STHISTIME	stHisTime;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
		switch(ucNo)
	{
	case 0:			return stMeterTimerVal[ucMeterNo].stHisLastVolOffEndTime[ucPos];
	case 1:			return stMeterTimerVal[ucMeterNo].stHisLastVolAOffEndTime[ucPos];
	case 2:			return stMeterTimerVal[ucMeterNo].stHisLastVolBOffEndTime[ucPos];
	case 3:			return stMeterTimerVal[ucMeterNo].stHisLastVolCOffEndTime[ucPos];
	}	
	ClearHisTime(&stHisTime);
	return stHisTime;
}
//年月日
STHISTIME Get_History_Date(unsigned char ucMeterNo,unsigned char ucPos)
{
	STHISTIME	stHisTime;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
		return stMeterTimerVal[ucMeterNo].stHisDate[ucPos];
	ClearHisTime(&stHisTime);
	return stHisTime;
}
//时分秒
STHISTIME Get_History_Time(unsigned char ucMeterNo,unsigned char ucPos)
{
	STHISTIME	stHisTime;
	if(ucPos<CONHISDATANUM && ucMeterNo < CON_METER_NUM)
		return stMeterTimerVal[ucMeterNo].stHisTime[ucPos];
	ClearHisTime(&stHisTime);
	return stHisTime;
}

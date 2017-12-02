/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Alarm_Manage_Meter.c
 Description    : ������澯���ݴ���
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2013-01-08    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

//static unsigned char Alarm_ForeJudge_VaOverUp(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_VbOverUp(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_VcOverUp(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_VaOverDown(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_VbOverDown(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_VcOverDown(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_IaOverUp(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_IbOverUp(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_IcOverUp(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_IaErrOverUp(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_IbErrOverUp(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_IcErrOverUp(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_SPowOverUp(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_SPowErrUp(unsigned char ucMPNo);

//static unsigned char Alarm_ForeJudge_VaBreak(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_VbBreak(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_VcBreak(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_VaLack(unsigned char ucMPNo);				//A��ʧѹ
//static unsigned char Alarm_ForeJudge_VbLack(unsigned char ucMPNo);				//B��ʧѹ
//static unsigned char Alarm_ForeJudge_VcLack(unsigned char ucMPNo);				//C��ʧѹ

//static unsigned char Alarm_ForeJudge_IUnBalanceOverUp(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_UUnBalanceOverUp(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_ProgramTimeChange(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_ProgramCountChange(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_PulseConstChange(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_CopyDayTimeChange(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_LostVolNumChange(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_FeeRateChange(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_DemandClear(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_MeterFly(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_PowDiffOwer(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_MeterStop(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_MeterDown(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_MeterBattLow(unsigned char ucMPNo);
//static unsigned char Alarm_ForeJudge_MeterTimeErr(unsigned char ucMPNo);
//
//static unsigned char Alarm_EnsureJudge_VaOverUp(STALRFOREJUDGE *pstAlrForeJudgeList,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_VbOverUp(STALRFOREJUDGE *pstAlrForeJudgeList,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_VcOverUp(STALRFOREJUDGE *pstAlrForeJudgeList,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_VaOverDown(STALRFOREJUDGE *pstAlrForeJudgeList,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_VbOverDown(STALRFOREJUDGE *pstAlrForeJudgeList,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_VcOverDown(STALRFOREJUDGE *pstAlrForeJudgeList,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_IaOverUp(STALRFOREJUDGE *pstAlrForeJudgeList,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_IbOverUp(STALRFOREJUDGE *pstAlrForeJudgeList,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_IcOverUp(STALRFOREJUDGE *pstAlrForeJudgeList,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_IaHeavyOverUp(STALRFOREJUDGE *pstAlrForeJudgeList,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_IbHeavyOverUp(STALRFOREJUDGE *pstAlrForeJudgeList,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_IcHeavyOverUp(STALRFOREJUDGE *pstAlrForeJudgeList,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_SPowOverUp(STALRFOREJUDGE *pstAlrForeJudgeList,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_SPowErrUp(STALRFOREJUDGE *pstAlrForeJudgeList,unsigned char ucListNo);
//
//static unsigned char Alarm_EnsureJudge_VaBreak(STALRFOREJUDGE *pstAlrForeJudgeList,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_VbBreak(STALRFOREJUDGE *pstAlrForeJudgeList,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_VcBreak(STALRFOREJUDGE *pstAlrForeJudgeList,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_VaLack(STALRFOREJUDGE *pstAlrForeJudgeList,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_VbLack(STALRFOREJUDGE *pstAlrForeJudgeList,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_VcLack(STALRFOREJUDGE *pstAlrForeJudgeList,unsigned char ucListNo);
//
//static unsigned char Alarm_EnsureJudge_IUnBalanceOverUp(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_UUnBalanceOverUp(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_ProgramTime(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_ProgramCount(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_PulseConst(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_CopyDayTime(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_LostVolNum(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_FeeRate(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo);
//static unsigned char Alarm_EnsureJudge_MeterStop(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo);
// static unsigned char Alarm_EnsureJudge_MeterBattLow(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo);
// static unsigned char Alarm_EnsureJudge_MeterTimeErr(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo);

//static unsigned char Alarm_ForeJudge_ValOverUp(unsigned char ucMPNo,STHISVAL stChkVal,unsigned long ulUpLimit,unsigned short usBack,unsigned char *pucAlrStat,unsigned short usAlrCode);
//static unsigned char Alarm_ForeJudge_ValOverDown(unsigned char ucMPNo,STHISVAL stChkVal,unsigned long ulDownLimit,unsigned short usBack,unsigned char *pucAlrStat,unsigned short usAlrCode);
//static unsigned char  Alarm_ForeJudge_VolBBreak(unsigned char ucMPNo,STHISVAL stChkValA,STHISVAL stChkValC,unsigned long ulDownLimit,unsigned char *pucAlrStat,unsigned short usAlrCode);
//static unsigned long  Get_MeterAveCurrent(unsigned char ucMPNo,STHISVAL stHisA,STHISVAL stHisB,STHISVAL stHisC);

//static unsigned long  Get_MeterFly_FixVal(unsigned char ucMPNo);
//static unsigned long  Get_MeterPowDiff_FixVal(unsigned char ucMPNo);

//static unsigned long  Get_MeterStop_FixVal(unsigned char ucMPNo);
//static unsigned char  Chk_MeterDown(STHISVAL stHisVal0,STHISVAL stHisVal1);

//static unsigned char Alarm_EnsureJudge_ValOverUp(STALRFOREJUDGE *pstAlrForeJudge,unsigned long ulUpLimit,unsigned short usBack,unsigned char ucListNo,unsigned short usDI,unsigned char *pucAlrStat);
//static unsigned char Alarm_EnsureJudge_ValOverDown(STALRFOREJUDGE *pstAlrForeJudge,unsigned long ulDownLimit,unsigned short usBack,unsigned char ucListNo,unsigned short usDI,unsigned char *pucAlrStat);
//static unsigned char  Alarm_EnsureJudge_VolBBreak(STALRFOREJUDGE *pstAlrForeJudge,unsigned long ulDownLimit,unsigned char ucListNo,unsigned char *pucAlrStat);
//static unsigned char Alarm_EnsureJudge_SQRTOverUp(STALRFOREJUDGE *pstAlrForeJudge,unsigned long ulUpLimit,unsigned short usBack,
//												   unsigned char ucListNo,unsigned short usDIP,unsigned short usDIQ,unsigned char *pucAlrStat);
//static unsigned char Alarm_EnsureJudge_ValUnBalanceOverUp(STALRFOREJUDGE *pstAlrForeJudge,unsigned long ulUpLimit,unsigned short usBack,
// 												   unsigned char ucListNo,unsigned short usDIA,unsigned short usDIB,unsigned short usDIC,unsigned char *pucAlrStat);
 
//static unsigned char Alarm_GetForeJudgeInfo_VaOver(STALRFOREJUDGE *pstAlrForeJudge);
//static unsigned char Alarm_GetForeJudgeInfo_VbOver(STALRFOREJUDGE *pstAlrForeJudge);
//static unsigned char Alarm_GetForeJudgeInfo_VcOver(STALRFOREJUDGE *pstAlrForeJudge);
//static unsigned char Alarm_GetForeJudgeInfo_IaOverUp(STALRFOREJUDGE *pstAlrForeJudge);
//static unsigned char Alarm_GetForeJudgeInfo_IbOverUp(STALRFOREJUDGE *pstAlrForeJudge);
//static unsigned char Alarm_GetForeJudgeInfo_IcOverUp(STALRFOREJUDGE *pstAlrForeJudge);
//static unsigned char Alarm_GetForeJudgeInfo_IaHeavyOverUp(STALRFOREJUDGE *pstAlrForeJudge);
//static unsigned char Alarm_GetForeJudgeInfo_IbHeavyOverUp(STALRFOREJUDGE *pstAlrForeJudge);
//static unsigned char Alarm_GetForeJudgeInfo_IcHeavyOverUp(STALRFOREJUDGE *pstAlrForeJudge);
//static unsigned char Alarm_GetForeJudgeInfo_SPowOverUp(STALRFOREJUDGE *pstAlrForeJudge);
//static unsigned char Alarm_GetForeJudgeInfo_VbBreak(STALRFOREJUDGE *pstAlrForeJudge);
 
//static unsigned char Alarm_GetForeJudgeInfo_IUnBalanceOverUp(STALRFOREJUDGE *pstAlrForeJudge);
//static unsigned char Alarm_GetForeJudgeInfo_UUnBalanceOverUp(STALRFOREJUDGE *pstAlrForeJudge);
// static unsigned char Alarm_GetForeJudgeInfo_ProgramTime(STALRFOREJUDGE *pstAlrForeJudge);
// static unsigned char Alarm_GetForeJudgeInfo_ProgramCount(STALRFOREJUDGE *pstAlrForeJudge);
// static unsigned char Alarm_GetForeJudgeInfo_PulseConst(STALRFOREJUDGE *pstAlrForeJudge);
// static unsigned char Alarm_GetForeJudgeInfo_CopyDayTime(STALRFOREJUDGE *pstAlrForeJudge);
// static unsigned char Alarm_GetForeJudgeInfo_LostVolNum(STALRFOREJUDGE *pstAlrForeJudge);
// static unsigned char Alarm_GetForeJudgeInfo_FeeRate(STALRFOREJUDGE *pstAlrForeJudge);
//static unsigned char Alarm_GetForeJudgeInfo_MeterStop(STALRFOREJUDGE *pstAlrForeJudge);
//static unsigned char Alarm_GetForeJudgeInfo_MeterBattLow(STALRFOREJUDGE *pstAlrForeJudge);
//static unsigned char Alarm_GetForeJudgeInfo_MeterTimeErr(STALRFOREJUDGE *pstAlrForeJudge);
 
//static unsigned char Alarm_GetEnsureJudgeInfo_VolOver(STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_GetEnsureJudgeInfo_CurOverUp(STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_GetEnsureJudgeInfo_CurHeavyOverUp(STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_GetEnsureJudgeInfo_VabcBreak(STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_GetEnsureJudgeInfo_SPowOverUp(STALRENSUREJUDGE *pstAlrEnsureJudge);
//
//static unsigned char Alarm_GetEnsureJudgeInfo_UnBalanceOverUp(STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_GetEnsureJudgeInfo_ProgramTime(STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_GetEnsureJudgeInfo_ProgramCount(STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_GetEnsureJudgeInfo_PulseConst(STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_GetEnsureJudgeInfo_CopyDayTime(STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_GetEnsureJudgeInfo_LostVolNum(STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_GetEnsureJudgeInfo_FeeRate(STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_GetEnsureJudgeInfo_MeterFly(STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_GetEnsureJudgeInfo_PowDiffOver(STALRENSUREJUDGE *pstAlrEnsureJudge);
// 
//static unsigned char Alarm_GetEnsureJudgeInfo_MeterStop(STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_GetEnsureJudgeInfo_MeterDown(STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_GetEnsureJudgeInfo_MeterBattLow(STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_GetEnsureJudgeInfo_MeterTimeErr(STALRENSUREJUDGE *pstAlrEnsureJudge);
static unsigned char Alarm_GetEnsureJudgeInfo_AllDataInit(STALRENSUREJUDGE *pstAlrEnsureJudge);
static unsigned char Alarm_GetEnsureJudgeInfo_SoftwareChange(STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_GetEnsureJudgeInfo_CurrentErr(STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_GetEnsureJudgeInfo_VolCirReverse(STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_GetEnsureJudgeInfo_SumCompOverLimit(STALRENSUREJUDGE *pstAlrEnsureJudge);
static unsigned char Alarm_GetEnsureJudgeInfo_NoTake(STALRENSUREJUDGE *pstAlrEnsureJudge,unsigned char ucMPNo);
//static unsigned char Is_FeeRateList_Equel(STFEERATELIST *pstFeeRateList1,STFEERATELIST *pstFeeRateList2);
static unsigned char Alarm_GetEnsureJudgeInfo_MeterState(STALRENSUREJUDGE *pstAlrEnsureJudge);

//static unsigned char Alarm_Check_485err(unsigned char ucCurrMeterNo);
/************************************************************************************************/
//�澯Ԥ������
unsigned char Alarm_ForeJudge_Meter(void)
{
	unsigned char	ucRet=RET_SUCCESS,i=0;
	unsigned char	ucMPNo=0;
	unsigned char ucMPType=CON_MPTYPE_NULL;
	
	for(i=0;i<CON_METER_NUM;i++)
	{
		ucMPNo		= RunPara_GetMeter_MPNo(i);
		ucMPType	= RunPara_GetMPType(ucMPNo);
		if(ucMPType == CON_MPTYPE_METER)
		{
//			if(Get_Shield_ACD_byAlrCode(CONALRCODE_VAOVERUP,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD_NOT)		//��ѹԽ����
//			{
//				Alarm_ForeJudge_VaOverUp(ucMPNo);			//��ѹ�ϸ�Ԥ��_A���ѹԽ����
//				Alarm_ForeJudge_VbOverUp(ucMPNo);
//				Alarm_ForeJudge_VcOverUp(ucMPNo);
//				Alarm_ForeJudge_VaOverDown(ucMPNo);			//��ѹ�ϸ�Ԥ��_A���ѹԽ����
//				Alarm_ForeJudge_VbOverDown(ucMPNo);
//				Alarm_ForeJudge_VcOverDown(ucMPNo);
//			}
//
//			if(Get_Shield_ACD_byAlrCode(CONALRCODE_IAOVERUP,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD_NOT)
//			{
//				Alarm_ForeJudge_IaOverUp(ucMPNo);			//����Խ��
//				Alarm_ForeJudge_IbOverUp(ucMPNo);
//				Alarm_ForeJudge_IcOverUp(ucMPNo);
//
//				Alarm_ForeJudge_IaErrOverUp(ucMPNo);		//����Խ������
//				Alarm_ForeJudge_IbErrOverUp(ucMPNo);
//				Alarm_ForeJudge_IcErrOverUp(ucMPNo);
//			}
//
//			if(Get_Shield_ACD_byAlrCode(CONALRCODE_VABREAK,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD_NOT)
//			{
//				Alarm_ForeJudge_VaBreak(ucMPNo);				//A���ѹ����
//				Alarm_ForeJudge_VbBreak(ucMPNo);				//B���ѹ����
//				Alarm_ForeJudge_VcBreak(ucMPNo);				//C���ѹ����
//				Alarm_ForeJudge_VaLack(ucMPNo);					//A���ѹʧѹ
//				Alarm_ForeJudge_VbLack(ucMPNo);					//B���ѹʧѹ
//				Alarm_ForeJudge_VcLack(ucMPNo);					//C���ѹʧѹ
//			}
//
//			if(Get_Shield_ACD_byAlrCode(CONALRCODE_SPOWNORUP,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD_NOT)
//			{
//				Alarm_ForeJudge_SPowOverUp(ucMPNo);			//���ڹ���
//				Alarm_ForeJudge_SPowErrUp(ucMPNo);			//���ڹ���Խ����
//			}
//			if(Get_Shield_ACD_byAlrCode(CONALRCODE_IUNBLANCE,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD_NOT)
//			{
//				Alarm_ForeJudge_IUnBalanceOverUp(ucMPNo);	//��ƽ���Խ��
//				Alarm_ForeJudge_UUnBalanceOverUp(ucMPNo);
//			}
//
//			Alarm_ForeJudge_DemandClear(ucMPNo);		//��������
//
//			if(Get_Shield_ACD_byAlrCode(CONALRCODE_PULSECONST,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD_NOT)
//			{
//				Alarm_ForeJudge_ProgramTimeChange(ucMPNo);	//���һ�α��ʱ�䷢���仯
//				Alarm_ForeJudge_PulseConstChange(ucMPNo);	//���峣��
//				Alarm_ForeJudge_CopyDayTimeChange(ucMPNo);	//�����ո���
//				Alarm_ForeJudge_FeeRateChange(ucMPNo);		//���ʸ���
//			}
//
//			if(Get_Shield_ACD_byAlrCode(CONALRCODE_METERFLY,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD_NOT)
//				Alarm_ForeJudge_MeterFly(ucMPNo);			//��Ʒ���
//
//			if(Get_Shield_ACD_byAlrCode(CONALRCODE_METERSTOP,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD_NOT)
//				Alarm_ForeJudge_MeterStop(ucMPNo);			//���ͣ��
//
//			if(Get_Shield_ACD_byAlrCode(CONALRCODE_METERDOWN,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD_NOT)
//				Alarm_ForeJudge_MeterDown(ucMPNo);			//��Ƶ���
//
//			if(Get_Shield_ACD_byAlrCode(CONALRCODE_METERBATTLOW,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD_NOT)
//			{
//				Alarm_ForeJudge_ProgramCountChange(ucMPNo);	//��̴��������仯
//				Alarm_ForeJudge_MeterBattLow(ucMPNo);		//��Ƶ�ص�ѹ��
//				Alarm_ForeJudge_LostVolNumChange(ucMPNo);	//�������
//			}
//
//			if(Get_Shield_ACD_byAlrCode(CONALRCODE_METERTIMEERR,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD_NOT)
//				Alarm_ForeJudge_MeterTimeErr(ucMPNo);		//���ʱ�Ӵ���
//			if(Get_Shield_ACD_byAlrCode(CONALRCODE_POWDIFFOVER,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD_NOT)
//				Alarm_ForeJudge_PowDiffOwer(ucMPNo);	// ERC28	�����������¼

				
		}
	}
		
	return ucRet;
}

unsigned char Alarm_EnsureJudge_Meter(unsigned char ucAlrForeNo)
{//�澯ȷ��
	unsigned char ucRet=RET_SUCCESS;
	stAlrForeJudgeList[ucAlrForeNo].ucAlrJudgeCount++;
	if(	(stAlrForeJudgeList[ucAlrForeNo].ucAlrJudgeCount >=stAlrForeJudgeList[ucAlrForeNo].ucAlrJudgeTimes ) &&
		(stAlrForeJudgeList[ucAlrForeNo].ucAlrJudgeTimes != 0))
	{
		switch(stAlrForeJudgeList[ucAlrForeNo].usAlrCode)
		{
//		case	CONALRCODE_VAOVERUP:		//A���ѹԽ����
//			Alarm_EnsureJudge_VaOverUp(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_VBOVERUP:		//B���ѹԽ����	
//			Alarm_EnsureJudge_VbOverUp(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_VCOVERUP:		//C���ѹԽ����	
//			Alarm_EnsureJudge_VcOverUp(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_VAOVERDOWN:		//A���ѹԽ����	
//			Alarm_EnsureJudge_VaOverDown(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_VBOVERDOWN:		//B���ѹԽ����	
//			Alarm_EnsureJudge_VbOverDown(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_VCOVERDOWN:		//C���ѹԽ����	
//			Alarm_EnsureJudge_VcOverDown(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_IAOVERUP:		//A�����Խ����	
//			Alarm_EnsureJudge_IaOverUp(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_IBOVERUP:		//B�����Խ����	
//			Alarm_EnsureJudge_IbOverUp(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_ICOVERUP:		//C�����Խ����	
//			Alarm_EnsureJudge_IcOverUp(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_IAERROVERUP:		//A�����Խ������
//			Alarm_EnsureJudge_IaHeavyOverUp(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_IBERROVERUP:		//B�����Խ������
//			Alarm_EnsureJudge_IbHeavyOverUp(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_ICERROVERUP:		//C�����Խ������
//			Alarm_EnsureJudge_IcHeavyOverUp(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_VABREAK:			//A���ѹ����
//			Alarm_EnsureJudge_VaBreak(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_VBBREAK:			//B���ѹ����
//			Alarm_EnsureJudge_VbBreak(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_VCBREAK:			//C���ѹ����
//			Alarm_EnsureJudge_VcBreak(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_VALACK:			//A���ѹʧѹ
//			Alarm_EnsureJudge_VaLack(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_VBLACK:			//B���ѹʧѹ
//			Alarm_EnsureJudge_VbLack(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_VCLACK:			//C���ѹʧѹ
//			Alarm_EnsureJudge_VcLack(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_SPOWNORUP:		//���ڹ���Խ����
//			Alarm_EnsureJudge_SPowOverUp(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_SPOWERRUP:		//���ڹ���Խ������
//			Alarm_EnsureJudge_SPowErrUp(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_IUNBLANCE:		//������ƽ��	
//			Alarm_EnsureJudge_IUnBalanceOverUp(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_UUNBLANCE:		//��ѹ��ƽ��
//			Alarm_EnsureJudge_UUnBalanceOverUp(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_PROGRAMTIME:		//���ʱ�����
//			Alarm_EnsureJudge_ProgramTime(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_PROGRAMNUM:		//��̴������
//			Alarm_EnsureJudge_ProgramCount(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_PULSECONST:		//���峣������
//			Alarm_EnsureJudge_PulseConst(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_COPYDAYTIME:		//�����ո���
//			Alarm_EnsureJudge_CopyDayTime(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_LOSTVOLNUM:		//�����������
//			Alarm_EnsureJudge_LostVolNum(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_FEERATE:			//���ʸ���
//			Alarm_EnsureJudge_FeeRate(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_METERSTOP:		//���ͣ��
//			Alarm_EnsureJudge_MeterStop(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_METERBATTLOW:	//��Ƶ�ص�ѹ��
//			Alarm_EnsureJudge_MeterBattLow(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_METERTIMEERR:	//���ʱ�Ӵ���
//			Alarm_EnsureJudge_MeterTimeErr(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
//		case	CONALRCODE_METERSTATECHANGE:	//ERC33
//	//		Alarm_EnsureJudge_MeterTimeErr(&(stAlrForeJudgeList[ucAlrForeNo]),ucAlrForeNo);
//			ucRet = RET_ENSURE_FINISH;
//			break;
		default	:
			ucRet=RET_ERROR;
			break;
		}
	}
	return ucRet;
}

////��ѹaԽ����Ԥ��
//unsigned char Alarm_ForeJudge_VaOverUp(unsigned char ucMPNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Vol;
//	
//	stHisVal_Vol=Get_History_V_byMPNo(ucMPNo,0,1);
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//
//	ucRet=Alarm_ForeJudge_ValOverUp(ucMPNo,stHisVal_Vol,stMPLimit.usVolErrUpLimit,stMPLimit.ucVolErrUpBack,
//										&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolaUpOver),CONALRCODE_VAOVERUP);
//	
//	if(ucRet==RET_SUCCESS)
//		Alarm_FillForeJudgeList(CONALRCODE_VAOVERUP,ucMPNo);
//	return ucRet;
//}
//
////��ѹbԽ����Ԥ��
//unsigned char Alarm_ForeJudge_VbOverUp(unsigned char ucMPNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Vol;
//
//	stHisVal_Vol=Get_History_V_byMPNo(ucMPNo,0,2);
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	ucRet=Alarm_ForeJudge_ValOverUp(ucMPNo,stHisVal_Vol,stMPLimit.usVolErrUpLimit, stMPLimit.ucVolErrUpBack,
//		&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolbUpOver),CONALRCODE_VBOVERUP);
//	
//	if(ucRet==RET_SUCCESS)
//		Alarm_FillForeJudgeList(CONALRCODE_VBOVERUP,ucMPNo);
//	return ucRet;
//}
//
////��ѹcԽ����Ԥ��
//unsigned char Alarm_ForeJudge_VcOverUp(unsigned char ucMPNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Vol;
//	
//	stHisVal_Vol=Get_History_V_byMPNo(ucMPNo,0,3);
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	ucRet=Alarm_ForeJudge_ValOverUp(ucMPNo,stHisVal_Vol,stMPLimit.usVolErrUpLimit, stMPLimit.ucVolErrUpBack,
//		&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolcUpOver),CONALRCODE_VCOVERUP);
//	
//	if(ucRet==RET_SUCCESS)
//		Alarm_FillForeJudgeList(CONALRCODE_VCOVERUP,ucMPNo);
//	return ucRet;
//}
//
////��ѹaԽ����Ԥ��
//unsigned char Alarm_ForeJudge_VaOverDown(unsigned char ucMPNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Vol;
//	
//	stHisVal_Vol=Get_History_V_byMPNo(ucMPNo,0,1);
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	ucRet=Alarm_ForeJudge_ValOverDown(ucMPNo,stHisVal_Vol,stMPLimit.usVolErrDownLimit,stMPLimit.ucVolErrDownBack,
//		&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolaDownOver),CONALRCODE_VAOVERDOWN);
//	
//	if(ucRet==RET_SUCCESS)
//		Alarm_FillForeJudgeList(CONALRCODE_VAOVERDOWN,ucMPNo);
//	return ucRet;
//}
//
////��ѹbԽ����Ԥ��
//unsigned char Alarm_ForeJudge_VbOverDown(unsigned char ucMPNo)
//{
//	unsigned char	ucRet=RET_ERROR;
//	STMPLIMIT		stMPLimit;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_Vol;
//	
//	stHisVal_Vol=Get_History_V_byMPNo(ucMPNo,0,2);
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	stMPRateVal = RunPara_GetMPRateVal(ucMPNo);
//
//	if(stMPRateVal.ucMPLineType == CON_METERTYPE_3P4L)
//	{
//		ucRet=Alarm_ForeJudge_ValOverDown(ucMPNo,stHisVal_Vol,stMPLimit.usVolErrDownLimit,stMPLimit.ucVolErrDownBack,
//			&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolbDownOver),CONALRCODE_VBOVERDOWN);
//	}
//	
//	if(ucRet==RET_SUCCESS)
//		Alarm_FillForeJudgeList(CONALRCODE_VBOVERDOWN,ucMPNo);
//	return ucRet;
//}
//
////��ѹcԽ����Ԥ��
//unsigned char Alarm_ForeJudge_VcOverDown(unsigned char ucMPNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Vol;
//	
//	stHisVal_Vol=Get_History_V_byMPNo(ucMPNo,0,3);
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	ucRet=Alarm_ForeJudge_ValOverDown(ucMPNo,stHisVal_Vol,stMPLimit.usVolErrDownLimit, stMPLimit.ucVolErrDownBack,
//		&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolcDownOver),CONALRCODE_VCOVERDOWN);
//	
//	if(ucRet==RET_SUCCESS)
//		Alarm_FillForeJudgeList(CONALRCODE_VCOVERDOWN,ucMPNo);
//	return ucRet;
//}
//
////����aԽ����Ԥ��
//unsigned char Alarm_ForeJudge_IaOverUp(unsigned char ucMPNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Cur;
//	
//	stHisVal_Cur=Get_History_I_byMPNo(ucMPNo,0,1);
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	if(stHisVal_Cur.lVal!=CONINVALIDVAL)
//	{
//		if(stHisVal_Cur.lVal<=(unsigned long)(stMPLimit.usCurErrUpLimit*CON_UPFLOAT_COEF/10))
//		{
//			ucRet=Alarm_ForeJudge_ValOverUp(ucMPNo,stHisVal_Cur,stMPLimit.usCurUpLimit, stMPLimit.ucCurUpBack,
//				&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_IaOverLoad),CONALRCODE_IAOVERUP);
//			
//			if(ucRet==RET_SUCCESS)
//				Alarm_FillForeJudgeList(CONALRCODE_IAOVERUP,ucMPNo);
//		}
//	}
//	return ucRet;
//}
//
////����bԽ����Ԥ��
//unsigned char Alarm_ForeJudge_IbOverUp(unsigned char ucMPNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Cur;
//	
//	stHisVal_Cur=Get_History_I_byMPNo(ucMPNo,0,2);
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	if(stHisVal_Cur.lVal!=CONINVALIDVAL)
//	{
//		if(stHisVal_Cur.lVal<=(unsigned long)(stMPLimit.usCurErrUpLimit*CON_UPFLOAT_COEF/10))
//		{
//			ucRet=Alarm_ForeJudge_ValOverUp(ucMPNo,stHisVal_Cur,stMPLimit.usCurUpLimit, stMPLimit.ucCurUpBack,
//				&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_IbOverLoad),CONALRCODE_IBOVERUP);
//			
//			if(ucRet==RET_SUCCESS)
//				Alarm_FillForeJudgeList(CONALRCODE_IBOVERUP,ucMPNo);
//		}
//	}
//	return ucRet;
//}
//
////����cԽ����Ԥ��
//unsigned char Alarm_ForeJudge_IcOverUp(unsigned char ucMPNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Cur;
//	
//	stHisVal_Cur=Get_History_I_byMPNo(ucMPNo,0,3);
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	if(stHisVal_Cur.lVal!=CONINVALIDVAL)
//	{
//		if(stHisVal_Cur.lVal<=(unsigned long)(stMPLimit.usCurErrUpLimit*CON_UPFLOAT_COEF/10))
//		{
//			ucRet=Alarm_ForeJudge_ValOverUp(ucMPNo,stHisVal_Cur,stMPLimit.usCurUpLimit, stMPLimit.ucCurUpBack,
//				&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_IcOverLoad),CONALRCODE_ICOVERUP);
//			
//			if(ucRet==RET_SUCCESS)
//				Alarm_FillForeJudgeList(CONALRCODE_ICOVERUP,ucMPNo);
//		}
//	}
//	return ucRet;
//}
//
////����aԽ������Ԥ��
//unsigned char Alarm_ForeJudge_IaErrOverUp(unsigned char ucMPNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Cur;
//	
//	stHisVal_Cur=Get_History_I_byMPNo(ucMPNo,0,1);
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	ucRet=Alarm_ForeJudge_ValOverUp(ucMPNo,stHisVal_Cur,stMPLimit.usCurErrUpLimit, stMPLimit.ucCurErrUpBack,
//		&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_IaErrOverLoad),CONALRCODE_IAERROVERUP);
//	
//	if(ucRet==RET_SUCCESS)
//		Alarm_FillForeJudgeList(CONALRCODE_IAERROVERUP,ucMPNo);
//	return ucRet;
//}
//
////����bԽ������Ԥ��
//unsigned char Alarm_ForeJudge_IbErrOverUp(unsigned char ucMPNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Cur;
//	
//	stHisVal_Cur=Get_History_I_byMPNo(ucMPNo,0,2);
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	ucRet=Alarm_ForeJudge_ValOverUp(ucMPNo,stHisVal_Cur,stMPLimit.usCurErrUpLimit, stMPLimit.ucCurErrUpBack,
//		&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_IbErrOverLoad),CONALRCODE_IBERROVERUP);
//	
//	if(ucRet==RET_SUCCESS)
//		Alarm_FillForeJudgeList(CONALRCODE_IBERROVERUP,ucMPNo);
//	return ucRet;
//}
//
////����cԽ������Ԥ��
//unsigned char Alarm_ForeJudge_IcErrOverUp(unsigned char ucMPNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Cur;
//	
//	stHisVal_Cur=Get_History_I_byMPNo(ucMPNo,0,3);
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	ucRet=Alarm_ForeJudge_ValOverUp(ucMPNo,stHisVal_Cur,stMPLimit.usCurErrUpLimit, stMPLimit.ucCurErrUpBack,
//		&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_IcErrOverLoad),CONALRCODE_ICERROVERUP);
//	
//	if(ucRet==RET_SUCCESS)
//		Alarm_FillForeJudgeList(CONALRCODE_ICERROVERUP,ucMPNo);
//	return ucRet;
//}
//
////A���ѹ����
////unsigned char ucAlrStat;
//unsigned char Alarm_ForeJudge_VaBreak(unsigned char ucMPNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Vol,stHisVal_Volc;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_Cur;
//	unsigned long	ul05perCur;
//
//	stHisVal_Vol=Get_History_V_byMPNo(ucMPNo,0,1);
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	stMPRateVal  = RunPara_GetMPRateVal(ucMPNo);
//	ul05perCur=(unsigned long)(stMPRateVal.usRateCur * 5/10);
//	if(ul05perCur==0)
//		ul05perCur=1;
//	
//	stHisVal_Cur= Get_History_I_byMPNo(ucMPNo,0,1);
////	ucAlrStat = stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolaBreak;
//	if(((stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolaBreak==CONALR_STAT_HAPPEN_NOT) && (stHisVal_Cur.lVal <= ul05perCur))
//		|| (stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolaBreak==CONALR_STAT_HAPPEN))
//	{
//		if(stMPRateVal.ucMPLineType == CON_METERTYPE_3P3L)
//		{
//			stHisVal_Volc=Get_History_V_byMPNo(ucMPNo,0,3);
//			if((stHisVal_Vol.lVal + stHisVal_Volc.lVal) >= ((CON_RATEVOL_3P3L + 20)*10))
//				ucRet=Alarm_ForeJudge_ValOverDown(ucMPNo,stHisVal_Vol,stMPLimit.usVolBreakLimit,1010,
//					&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolaBreak),CONALRCODE_VABREAK);
//		}else
//		{
//			ucRet=Alarm_ForeJudge_ValOverDown(ucMPNo,stHisVal_Vol,stMPLimit.usVolBreakLimit,1010,
//				&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolaBreak),CONALRCODE_VABREAK);
//		}
//	}
//	else
//		ucRet=RET_ERROR;
//	
//	if(ucRet==RET_SUCCESS)
//		Alarm_FillForeJudgeList(CONALRCODE_VABREAK,ucMPNo);
//	return ucRet;
//}
//
////B���ѹ����
//unsigned char Alarm_ForeJudge_VbBreak(unsigned char ucMPNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_Vol;
//	STHISVAL		stHisVal_Vol1;
//	STHISVAL		stHisVal_Cur;
//	unsigned long	ul05perCur;
//	
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	stMPRateVal = RunPara_GetMPRateVal(ucMPNo);
//	ul05perCur=(unsigned long)(stMPRateVal.usRateCur * 5/10);
//	if(ul05perCur==0)
//		ul05perCur=1;
//
//	if(stMPRateVal.ucMPLineType == CON_METERTYPE_3P4L)
//	{
//		stHisVal_Cur= Get_History_I_byMPNo(ucMPNo,0,2);
//		if(((stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolbBreak==CONALR_STAT_HAPPEN_NOT) && (stHisVal_Cur.lVal <= ul05perCur))
//			|| (stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolbBreak==CONALR_STAT_HAPPEN))
//		{
//			stHisVal_Vol=Get_History_V_byMPNo(ucMPNo,0,2);
//			ucRet =Alarm_ForeJudge_ValOverDown(ucMPNo,stHisVal_Vol,stMPLimit.usVolBreakLimit,1010,
//				&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolbBreak),CONALRCODE_VBBREAK);
//		}
//		else
//			ucRet=RET_ERROR;
//	}else
//	{
//		stHisVal_Vol=Get_History_V_byMPNo(ucMPNo,0,1);
//		stHisVal_Vol1=Get_History_V_byMPNo(ucMPNo,0,3);
//		ucRet =Alarm_ForeJudge_VolBBreak(ucMPNo,stHisVal_Vol,stHisVal_Vol1,stMPLimit.usVolBreakLimit,
//			&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolbBreak),CONALRCODE_VBBREAK); 
//	}
//	
//	if(ucRet==RET_SUCCESS)
//		Alarm_FillForeJudgeList(CONALRCODE_VBBREAK,ucMPNo);
//	return ucRet;
//}
//
////C���ѹ����
//unsigned char Alarm_ForeJudge_VcBreak(unsigned char ucMPNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Vol,stHisVal_Vola;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_Cur;
//	unsigned long	ul05perCur;
//	
//	stHisVal_Vol=Get_History_V_byMPNo(ucMPNo,0,3);
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	stMPRateVal  = RunPara_GetMPRateVal(ucMPNo);
//	ul05perCur=(unsigned long)(stMPRateVal.usRateCur * 5/10);
//	if(ul05perCur==0)
//		ul05perCur=1;
//	
//	stHisVal_Cur= Get_History_I_byMPNo(ucMPNo,0,3);
//
//	if(((stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolcBreak==CONALR_STAT_HAPPEN_NOT) && (stHisVal_Cur.lVal <= ul05perCur))
//		|| (stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolcBreak==CONALR_STAT_HAPPEN))
//	{
//		if(stMPRateVal.ucMPLineType == CON_METERTYPE_3P3L)
//		{
//			stHisVal_Vola=Get_History_V_byMPNo(ucMPNo,0,1);
//			if((stHisVal_Vol.lVal + stHisVal_Vola.lVal) >= ((CON_RATEVOL_3P3L + 20)*10))
//				ucRet=Alarm_ForeJudge_ValOverDown(ucMPNo,stHisVal_Vol,stMPLimit.usVolBreakLimit,1010,
//					&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolcBreak),CONALRCODE_VCBREAK);
//		}else
//		{
//			ucRet=Alarm_ForeJudge_ValOverDown(ucMPNo,stHisVal_Vol,stMPLimit.usVolBreakLimit,1010,
//				&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolcBreak),CONALRCODE_VCBREAK);
//		}
//	}
//	else
//		ucRet=RET_ERROR;
//	
//	if(ucRet==RET_SUCCESS)
//		Alarm_FillForeJudgeList(CONALRCODE_VCBREAK,ucMPNo);
//	return ucRet;
//}
//
////A���ѹʧѹ
//unsigned char Alarm_ForeJudge_VaLack(unsigned char ucMPNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Vol,stHisVal_Volc;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_Cur;
//	unsigned long	ul05perCur;
//	
//	stHisVal_Vol=Get_History_V_byMPNo(ucMPNo,0,1);
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	stMPRateVal  = RunPara_GetMPRateVal(ucMPNo);
//	ul05perCur=(unsigned long)(stMPRateVal.usRateCur * 5/10);
//	if(ul05perCur==0)
//		ul05perCur=1;
//	
//	stHisVal_Cur= Get_History_I_byMPNo(ucMPNo,0,1);
//	if(((stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolaLack==CONALR_STAT_HAPPEN_NOT) && (stHisVal_Cur.lVal > ul05perCur))
//		|| (stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolaLack==CONALR_STAT_HAPPEN))
//	{
//		if(stMPRateVal.ucMPLineType == CON_METERTYPE_3P3L)
//		{
//			stHisVal_Volc=Get_History_V_byMPNo(ucMPNo,0,3);
//			if((stHisVal_Vol.lVal + stHisVal_Volc.lVal) >= ((CON_RATEVOL_3P3L + 20)*10))
//				ucRet=Alarm_ForeJudge_ValOverDown(ucMPNo,stHisVal_Vol,stMPLimit.usVolBreakLimit,1010,
//					&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolaLack),CONALRCODE_VALACK);
//		}else
//		{
//			ucRet=Alarm_ForeJudge_ValOverDown(ucMPNo,stHisVal_Vol,stMPLimit.usVolBreakLimit,1010,
//				&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolaLack),CONALRCODE_VALACK);
//		}
//	}
//	else
//		ucRet=RET_ERROR;
//	
//	if(ucRet==RET_SUCCESS)
//		Alarm_FillForeJudgeList(CONALRCODE_VALACK,ucMPNo);
//	return ucRet;
//}
//
////B���ѹʧѹ
//unsigned char Alarm_ForeJudge_VbLack(unsigned char ucMPNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_Vol;
//	STHISVAL		stHisVal_Vol1;
//	STHISVAL		stHisVal_Cur;
//	unsigned long	ul05perCur;
//	
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	stMPRateVal = RunPara_GetMPRateVal(ucMPNo);
//	ul05perCur=(unsigned long)(stMPRateVal.usRateCur * 5/10);
//	if(ul05perCur==0)
//		ul05perCur=1;
//
//	if(stMPRateVal.ucMPLineType == CON_METERTYPE_3P4L)
//	{
//		stHisVal_Cur= Get_History_I_byMPNo(ucMPNo,0,2);
//		if(((stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolbLack==CONALR_STAT_HAPPEN_NOT) && (stHisVal_Cur.lVal > ul05perCur))
//			|| (stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolbLack==CONALR_STAT_HAPPEN))
//		{
//			stHisVal_Vol=Get_History_V_byMPNo(ucMPNo,0,2);
//			ucRet =Alarm_ForeJudge_ValOverDown(ucMPNo,stHisVal_Vol,stMPLimit.usVolBreakLimit,1010,
//				&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolbLack),CONALRCODE_VBLACK);
//		}
//		else
//			ucRet=RET_ERROR;
//	}else
//	{
//		stHisVal_Vol=Get_History_V_byMPNo(ucMPNo,0,1);
//		stHisVal_Vol1=Get_History_V_byMPNo(ucMPNo,0,3);
//		ucRet =Alarm_ForeJudge_VolBBreak(ucMPNo,stHisVal_Vol,stHisVal_Vol1,stMPLimit.usVolBreakLimit,
//			&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolbLack),CONALRCODE_VBLACK); 
//	}
//	
//	if(ucRet==RET_SUCCESS)
//		Alarm_FillForeJudgeList(CONALRCODE_VBLACK,ucMPNo);
//	return ucRet;
//}
//
////C���ѹʧѹ
//unsigned char Alarm_ForeJudge_VcLack(unsigned char ucMPNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Vol,stHisVal_Vola;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_Cur;
//	unsigned long	ul05perCur;
//	
//	stHisVal_Vol=Get_History_V_byMPNo(ucMPNo,0,3);
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	stMPRateVal  = RunPara_GetMPRateVal(ucMPNo);
//	ul05perCur=(unsigned long)(stMPRateVal.usRateCur * 5/10);
//	if(ul05perCur==0)
//		ul05perCur=1;
//	
//	stHisVal_Cur= Get_History_I_byMPNo(ucMPNo,0,3);
//	if(((stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolcLack==CONALR_STAT_HAPPEN_NOT) && (stHisVal_Cur.lVal > ul05perCur))
//		|| (stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolcLack==CONALR_STAT_HAPPEN))
//	{
//		if(stMPRateVal.ucMPLineType == CON_METERTYPE_3P3L)
//		{
//			stHisVal_Vola=Get_History_V_byMPNo(ucMPNo,0,1);
//			if((stHisVal_Vol.lVal + stHisVal_Vola.lVal) >= ((CON_RATEVOL_3P3L + 20)*10))
//				ucRet=Alarm_ForeJudge_ValOverDown(ucMPNo,stHisVal_Vol,stMPLimit.usVolBreakLimit,1010,
//					&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolcLack),CONALRCODE_VCLACK);
//		}else
//		{
//			ucRet=Alarm_ForeJudge_ValOverDown(ucMPNo,stHisVal_Vol,stMPLimit.usVolBreakLimit,1010,
//				&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolcLack),CONALRCODE_VCLACK);
//		}
//	}
//	else
//		ucRet=RET_ERROR;
//	
//	if(ucRet==RET_SUCCESS)
//		Alarm_FillForeJudgeList(CONALRCODE_VCLACK,ucMPNo);
//	return ucRet;
//}
//
////���ڹ���Խ����Ԥ��
//unsigned char Alarm_ForeJudge_SPowOverUp(unsigned char ucMPNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_P;
//	STHISVAL		stHisVal_Q;
//	STHISVAL		stHisVal_Cur;
//	
//	stHisVal_P=Get_History_InstantPow_byMPNo(ucMPNo,0,0);
//	stHisVal_Q=Get_History_InstantVar_byMPNo(ucMPNo,0,0);
//	stHisVal_Cur=stHisVal_P;
//	if(stHisVal_P.lVal!=CONINVALIDVAL && stHisVal_Q.lVal!=CONINVALIDVAL)
//		stHisVal_Cur.lVal = GetSPow(stHisVal_P.lVal,stHisVal_Q.lVal);	//4λС��
//	else
//		ClearHisVal(&stHisVal_Cur);
//
//	if(stHisVal_Cur.lVal!=CONINVALIDVAL)
//	{
//		stMPLimit = RunPara_GetMPLimit(ucMPNo);
//		if(stHisVal_Cur.lVal<=(unsigned long)(stMPLimit.ulPowErrUpLimit*CON_UPFLOAT_COEF/100))
//		{
//			ucRet=Alarm_ForeJudge_ValOverUp(ucMPNo,stHisVal_Cur,(unsigned long)(stMPLimit.ulPowUpLimit/10), stMPLimit.ucPowUpBack,
//				&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_SPowOverUp),CONALRCODE_SPOWNORUP);
//			
//			if(ucRet==RET_SUCCESS)
//				Alarm_FillForeJudgeList(CONALRCODE_SPOWNORUP,ucMPNo);
//		}
//	}
//	return ucRet;
//}
//
////���ڹ���Խ������Ԥ��
//unsigned char Alarm_ForeJudge_SPowErrUp(unsigned char ucMPNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_P;
//	STHISVAL		stHisVal_Q;
//	STHISVAL		stHisVal_Cur;
//	
//	stHisVal_P=Get_History_InstantPow_byMPNo(ucMPNo,0,0);
//	stHisVal_Q=Get_History_InstantVar_byMPNo(ucMPNo,0,0);
//	stHisVal_Cur=stHisVal_P;
//	if(stHisVal_P.lVal!=CONINVALIDVAL && stHisVal_Q.lVal!=CONINVALIDVAL)
//		stHisVal_Cur.lVal = GetSPow(stHisVal_P.lVal,stHisVal_Q.lVal);	//4λС��
//	else
//		ClearHisVal(&stHisVal_Cur);
//
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	ucRet=Alarm_ForeJudge_ValOverUp(ucMPNo,stHisVal_Cur,(unsigned long)(stMPLimit.ulPowErrUpLimit/10), stMPLimit.ucPowErrUpBack,
//		&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_SPowErrUp),CONALRCODE_SPOWERRUP);
//	
//	if(ucRet==RET_SUCCESS)
//		Alarm_FillForeJudgeList(CONALRCODE_SPOWERRUP,ucMPNo);
//	return ucRet;
//}
//
//unsigned char Alarm_ForeJudge_IUnBalanceOverUp(unsigned char ucMPNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_CurA,stHisVal_CurB,stHisVal_CurC;
//	STHISVAL		stHisVal_Cur;
//	unsigned long	ulMax=0,ulTmp;
//	
//	stHisVal_CurA=Get_History_I_byMPNo(ucMPNo,0,1);
//	stHisVal_CurB=Get_History_I_byMPNo(ucMPNo,0,2);
//	stHisVal_CurC=Get_History_I_byMPNo(ucMPNo,0,3);
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	stMPRateVal = RunPara_GetMPRateVal(ucMPNo);
//	if(stHisVal_CurA.lVal!=CONINVALIDVAL && stHisVal_CurB.lVal!=CONINVALIDVAL && stHisVal_CurC.lVal!=CONINVALIDVAL)
//	{
//		ulMax=stHisVal_CurA.lVal;
//		if(stHisVal_CurB.lVal > ulMax)
//			ulMax=stHisVal_CurB.lVal;
//		if(stHisVal_CurC.lVal > ulMax)
//			ulMax=stHisVal_CurC.lVal;
//		ulTmp=stMPRateVal.usRateCur*10/10;				//10%In
//		if(ulMax>ulTmp)
//		{
//			stHisVal_Cur = GetUnBalanceVal(stHisVal_CurA,stHisVal_CurB,stHisVal_CurC,stMPRateVal.ucMPLineType);
//			stHisVal_Cur.lVal *= 10;
//			
//			ucRet=Alarm_ForeJudge_ValOverUp(ucMPNo,stHisVal_Cur,stMPLimit.usCurUnBalance, stMPLimit.ucCurUnBalanceBack,
//				&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_IUnbalance),CONALRCODE_IUNBLANCE);
//			if(ucRet==RET_SUCCESS)
//				Alarm_FillForeJudgeList(CONALRCODE_IUNBLANCE,ucMPNo);
//		}
//	}
//	return ucRet;
//}
//
//unsigned char Alarm_ForeJudge_UUnBalanceOverUp(unsigned char ucMPNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_VolA,stHisVal_VolB,stHisVal_VolC;
//	STHISVAL		stHisVal_Vol;
//	
//	stHisVal_VolA =Get_History_V_byMPNo(ucMPNo,0,1);
//	stHisVal_VolB =Get_History_V_byMPNo(ucMPNo,0,2);
//	stHisVal_VolC =Get_History_V_byMPNo(ucMPNo,0,3);
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	stMPRateVal = RunPara_GetMPRateVal(ucMPNo);
//	
//	stHisVal_Vol = GetUnBalanceVal(stHisVal_VolA,stHisVal_VolB,stHisVal_VolC,stMPRateVal.ucMPLineType);
//	if(stHisVal_Vol.lVal != CONINVALIDVAL)
//		stHisVal_Vol.lVal *= 10;
//	
//	ucRet=Alarm_ForeJudge_ValOverUp(ucMPNo,stHisVal_Vol,stMPLimit.usVolUnBalance, stMPLimit.ucVolUnBalanceBack,
//		&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_UUnbalance),CONALRCODE_UUNBLANCE);
//	
//	if(ucRet==RET_SUCCESS)
//		Alarm_FillForeJudgeList(CONALRCODE_UUNBLANCE,ucMPNo);
//	return ucRet;
//}
//
//unsigned char Alarm_ForeJudge_ProgramTimeChange(unsigned char ucMPNo)
//{//���ʱ�����
//	unsigned char ucRet=RET_SUCCESS;
//	STHISTIME	stHisTime0,stHisTime1;
//	stHisTime0=Get_History_ProgramTime_byMPNo(ucMPNo,0);
//	stHisTime1=Get_History_ProgramTime_byMPNo(ucMPNo,1);
//	
//	if(Is_DateTime_Availble(&stHisTime0.stHisTime)==RET_SUCCESS && Is_DateTime_Availble(&stHisTime1.stHisTime)==RET_SUCCESS)
//	{
//		if(Is_DateTime_Equel(&(stHisTime0.stHisTime),&(stHisTime1.stHisTime))==RET_ERROR)
//			Alarm_FillForeJudgeList(CONALRCODE_PROGRAMTIME,ucMPNo);
//	}
//	
//	return ucRet;
//}
//
//unsigned char Alarm_ForeJudge_ProgramCountChange(unsigned char ucMPNo)
//{//��̴������
//	unsigned char ucRet=RET_SUCCESS;
//	STHISVAL	stHisVal0,stHisVal1;
//
//	stHisVal0=Get_History_ProgramCount_byMPNo(ucMPNo,0);
//	stHisVal1=Get_History_ProgramCount_byMPNo(ucMPNo,1);
//	
//	if(	stHisVal0.lVal!=CONINVALIDVAL && stHisVal1.lVal!=CONINVALIDVAL)
//	{
//		if(stHisVal0.lVal != stHisVal1.lVal)
//			Alarm_FillForeJudgeList(CONALRCODE_PROGRAMNUM,ucMPNo);
//	}
//	
//	return ucRet;
//}
//
//unsigned char Alarm_ForeJudge_PulseConstChange(unsigned char ucMPNo)
//{//���峣������
//	unsigned char ucRet=RET_SUCCESS;
//	STHISVAL	stHisVal00,stHisVal01;
//	STHISVAL	stHisVal10,stHisVal11;
//	stHisVal00=Get_History_PowPulseConst_byMPNo(ucMPNo,0);
//	stHisVal01=Get_History_PowPulseConst_byMPNo(ucMPNo,1);
//	
//	stHisVal10=Get_History_VarPulseConst_byMPNo(ucMPNo,0);
//	stHisVal11=Get_History_VarPulseConst_byMPNo(ucMPNo,1);
//	
//	if(	stHisVal00.lVal!=CONINVALIDVAL && stHisVal01.lVal!=CONINVALIDVAL && 
//		stHisVal10.lVal!=CONINVALIDVAL && stHisVal11.lVal!=CONINVALIDVAL)
//	{
//		if(stHisVal00.lVal != stHisVal01.lVal || stHisVal10.lVal != stHisVal11.lVal)
//			Alarm_FillForeJudgeList(CONALRCODE_PULSECONST,ucMPNo);
//	}
//	
//	return ucRet;
//}
//
//unsigned char Alarm_ForeJudge_CopyDayTimeChange(unsigned char ucMPNo)
//{//�����ո���
//	unsigned char ucRet=RET_SUCCESS;
//	STHISTIME	stHisTime0,stHisTime1;
//	stHisTime0=Get_History_CopyDayTime_byMPNo(ucMPNo,0);
//	stHisTime1=Get_History_CopyDayTime_byMPNo(ucMPNo,1);
//	
//	if(Is_DateTime_Availble(&stHisTime0.stHisTime)==RET_SUCCESS && Is_DateTime_Availble(&stHisTime1.stHisTime)==RET_SUCCESS)
//	{
//		if(Is_DateTime_Equel(&(stHisTime0.stHisTime),&(stHisTime1.stHisTime))==RET_ERROR)
//			Alarm_FillForeJudgeList(CONALRCODE_COPYDAYTIME,ucMPNo);
//	}
//	
//	return ucRet;
//}
//
//unsigned char Alarm_ForeJudge_LostVolNumChange(unsigned char ucMPNo)
//{//�������
//	unsigned char ucRet=RET_SUCCESS;
//	STHISVAL	stHisVal0,stHisVal1;
//	
//	stHisVal0=Get_History_VolBreakNum_byMPNo(ucMPNo,0,0);
//	stHisVal1=Get_History_VolBreakNum_byMPNo(ucMPNo,1,0);
//	
//	if(	stHisVal0.lVal!=CONINVALIDVAL && stHisVal1.lVal!=CONINVALIDVAL)
//	{
//		if(stHisVal0.lVal != stHisVal1.lVal)
//			Alarm_FillForeJudgeList(CONALRCODE_LOSTVOLNUM,ucMPNo);
//	}
//	
//	return ucRet;
//}
//
//unsigned char Alarm_ForeJudge_FeeRateChange(unsigned char ucMPNo)
//{//���ʸ���
//	unsigned char ucRet=RET_SUCCESS;
//	STHISFEERATE	stHisFeeRate0,stHisFeeRate1;
//	
//	stHisFeeRate0=Get_History_FeeRate_byMPNo(ucMPNo,0);
//	stHisFeeRate1=Get_History_FeeRate_byMPNo(ucMPNo,1);
//	
//	if(	stHisFeeRate0.stFeeRateList.ucFeeRateNum!=CONINVALIDVAL &&
//		stHisFeeRate1.stFeeRateList.ucFeeRateNum!=CONINVALIDVAL)
//	{
//		if(Is_FeeRateList_Equel(&(stHisFeeRate0.stFeeRateList),&(stHisFeeRate1.stFeeRateList))==RET_ERROR)
//			Alarm_FillForeJudgeList(CONALRCODE_FEERATE,ucMPNo);
//	}
//	
//	return ucRet;
//}
//
//unsigned char Alarm_ForeJudge_DemandClear(unsigned char ucMPNo)
//{//�������������������仯��������Ҫȷ��
//	unsigned char ucRet = RET_SUCCESS;
//	unsigned char ucMeterNo;
//	
//	STHISVAL	stHisDemandClrCount0  = Get_History_DemandClrCount_byMPNo(ucMPNo,0);
//	STHISVAL	stHisDemandClrCount1  = Get_History_DemandClrCount_byMPNo(ucMPNo,1);
//	
//	STHISTIME	stCopyDay	 = Get_History_CopyDayTime_byMPNo(ucMPNo,0);
//	STDATETIME	stCurrTime   = GetCurrentTime();
//	
//	ucMeterNo=RunPara_GetMeterNo_byMPNo(ucMPNo);
//
//	if(	stHisDemandClrCount0.lVal != CONINVALIDVAL &&	stHisDemandClrCount1.lVal != CONINVALIDVAL &&
//		Is_DateTime_Availble(&stCopyDay.stHisTime) == RET_SUCCESS )
//	{
//		if(stHisDemandClrCount0.lVal != stHisDemandClrCount1.lVal && stCurrTime.ucDay != stCopyDay.stHisTime.ucDay)
//		{
//			if((Alarm_CheckEnsureJudgeList( CONALRCODE_DEMANDTIMECLR,ucMPNo)!=RET_SUCCESS) &&	//�Ѿ��澯���ڣ����ж�
//				(stAlarmStatus[ucMPNo].ucAlrStat_Sys_DemandClr	== CONALR_STAT_HAPPEN_NOT) )
//				Alarm_FillEnsureJudgeList( CONALRCODE_DEMANDTIMECLR,ucMPNo);
//
//			if((Alarm_CheckEnsureJudgeList( CONALRCODE_DEMANDTIMENUM,ucMPNo)!=RET_SUCCESS) &&	//�Ѿ��澯���ڣ����ж�
//				(stAlarmStatus[ucMPNo].ucAlrStat_Sys_DemandClr	== CONALR_STAT_HAPPEN_NOT) )
//				Alarm_FillEnsureJudgeList( CONALRCODE_DEMANDTIMENUM,ucMPNo);
//			
////			stAlarmStatus[ucMPNo].ucAlrStat_Sys_DemandClr	= CONALR_STAT_HAPPEN;
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_DemandClr	= CONALR_STAT_HAPPEN_NOT;	//�ڶ��θ澯�޷�����
//
//			Add_History_DemandClrCount(ucMeterNo,stHisDemandClrCount0.lVal);
//			Add_History_DemandClrCount(ucMeterNo,stHisDemandClrCount0.lVal);
//		}else
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_DemandClr	= CONALR_STAT_HAPPEN_NOT;			
//	}
//	return ucRet;
//}

//unsigned char Alarm_ForeJudge_MeterFly(unsigned char ucMPNo)
//{//��Ʒ���
//	unsigned char ucRet=RET_SUCCESS;
//	STHISVAL	  stHisVal0,stHisVal1;
//	unsigned long ulTimeDiff=0;
//	unsigned long ulAvePow=0,ulFixValL=0,ulDiff;
//	unsigned char ucCopyInter;
//	STMPRATEVAL		stMPRateVal;
//	float fval=0;
//	ucCopyInter=RunPara_GetMeterCopyInter();		//������
//
//	stHisVal0=Get_History_DirPower_byMPNo(ucMPNo,0,0);
//	stHisVal1=Get_History_DirPower_byMPNo(ucMPNo,1,0);
//	stMPRateVal = RunPara_GetMPRateVal(ucMPNo);
//	if(stHisVal0.lVal!=CONINVALIDVAL && stHisVal1.lVal!=CONINVALIDVAL)
//	{
//		ulTimeDiff=SubDateTime_Second(&(stHisVal0.stDateTime),&(stHisVal1.stDateTime));
//		if( (ulTimeDiff>=(unsigned long )(ucCopyInter*60-60) ) && 
//			(ulTimeDiff<(unsigned long )(ucCopyInter*60+CON_ALR_TIMEINTER_COEF) ) )
//		{
//			if(stHisVal0.lVal >= stHisVal1.lVal)
//				ulAvePow = (stHisVal0.lVal-stHisVal1.lVal);//����ǰ��ĵ���
//			else
//				ulAvePow = 0;
//
//			ulFixValL = Get_MeterFly_FixVal(ucMPNo);
//			ulDiff = stMPRateVal.usRateCur*stMPRateVal.usRateVol*3;
//			fval = (float)ulDiff;				//0.001va����ת���ɸ�����
//			fval = fval/1000;
//			fval/=36000;//���㷽ʽ	//
//			fval *= ulTimeDiff;				//0.0001wh
//			ulDiff = (unsigned long)(fval*ulFixValL+0.5);		//������ת��������0.01kwh			
//			ulDiff = ulDiff/10;
//			if(ulAvePow > ulDiff && stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterFly	== CONALR_STAT_HAPPEN_NOT)
//			{
//				if(Alarm_CheckEnsureJudgeList(CONALRCODE_METERFLY,ucMPNo)!=RET_SUCCESS)	//�Ѿ��澯���ڣ����ж�
//				{
//					stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterFly	= CONALR_STAT_HAPPEN;
//					Alarm_FillEnsureJudgeList(CONALRCODE_METERFLY,ucMPNo);
//				}
//			}else if(ulAvePow < ulDiff && stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterFly != CONALR_STAT_HAPPEN_NOT)
//				stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterFly	= CONALR_STAT_HAPPEN_NOT;
//		}
//	}
//	
//	return ucRet;
//}
//unsigned char Alarm_ForeJudge_PowDiffOwer(unsigned char ucMPNo)
//{//����������
//	unsigned char ucRet=RET_SUCCESS;
//	STHISVAL	  stHisVal0,stHisVal1;
//	unsigned long ulTimeDiff=0;
//	unsigned long ulAvePow=0,ulFixValL=0,ulDiff,ulDiff1;
//	unsigned char ucCopyInter;
//	STMPRATEVAL		stMPRateVal;
//	float fval=0;
//	ucCopyInter=RunPara_GetMeterCopyInter();		//������
//	
//	stHisVal0=Get_History_DirPower_byMPNo(ucMPNo,0,0);
//	stHisVal1=Get_History_DirPower_byMPNo(ucMPNo,1,0);
//	stMPRateVal = RunPara_GetMPRateVal(ucMPNo);
//	if(stHisVal0.lVal!=CONINVALIDVAL && stHisVal1.lVal!=CONINVALIDVAL)
//	{
//		ulTimeDiff=SubDateTime_Second(&(stHisVal0.stDateTime),&(stHisVal1.stDateTime));
//		if( (ulTimeDiff>=(unsigned long )(ucCopyInter*60-60) ) && 
//			(ulTimeDiff<(unsigned long )(ucCopyInter*60+CON_ALR_TIMEINTER_COEF) ) )
//		{
//			if(stHisVal0.lVal >= stHisVal1.lVal)
//				ulAvePow = (stHisVal0.lVal-stHisVal1.lVal);//����ǰ��ĵ���
//			else
//				ulAvePow = 0;
//			
//			ulFixValL = Get_MeterPowDiff_FixVal(ucMPNo);
//			ulDiff = stMPRateVal.usRateCur*stMPRateVal.usRateVol*3;
//			fval = (float)ulDiff;
//			fval = fval/1000;			//0.001VA
//			fval/=36000;//			���㷽ʽ
//			fval *= ulTimeDiff;			//0.0001wh
//			ulDiff = (unsigned long)(fval*ulFixValL+0.5);	//wh
//			ulDiff = ulDiff/10;				//10wh 0.01kwh
//			
//			ulFixValL = Get_MeterFly_FixVal(ucMPNo);
//			ulDiff1 = (unsigned long)(fval*ulFixValL+0.5);			
//			ulDiff1 = ulDiff1/10;
//			if(ulAvePow<ulDiff1&&ulAvePow > ulDiff && stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterPowDiff	== CONALR_STAT_HAPPEN_NOT)
//			{
//				if(Alarm_CheckEnsureJudgeList(CONALRCODE_POWDIFFOVER,ucMPNo)!=RET_SUCCESS)	//�Ѿ��澯���ڣ����ж�
//				{
//					stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterPowDiff	= CONALR_STAT_HAPPEN;
//					Alarm_FillEnsureJudgeList(CONALRCODE_POWDIFFOVER,ucMPNo);
//				}
//			}else if(ulAvePow < ulDiff  && stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterPowDiff != CONALR_STAT_HAPPEN_NOT)
//				stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterPowDiff	= CONALR_STAT_HAPPEN_NOT;
//		}
//	}
//	
//	return ucRet;
//}

//unsigned char Alarm_ForeJudge_MeterStop(unsigned char ucMPNo)
//{//���ͣ��
//	unsigned char ucRet=RET_SUCCESS;
//	STHISVAL	  stHisVal00,stHisVal01;
//	STHISVAL	  stHisVal10,stHisVal11;
//	STHISVAL	  stHisVal20,stHisVal21;
//	STHISVAL	  stHisVal30,stHisVal31;
//	STHISVAL	  stHisI0A,stHisI0B,stHisI0C;
//	STHISVAL	  stHisI1A,stHisI1B,stHisI1C;
//	unsigned long ulFixVal=0,ulAveI0=0,ulAveI1=0;
//	unsigned long ulTimeDiff=0;
//	unsigned char ucCopyInter;
//	
//	ucCopyInter=RunPara_GetMeterCopyInter();		//������
//
//	stHisVal00=Get_History_DirPower_byMPNo(ucMPNo,0,0);
//	stHisVal01=Get_History_DirPower_byMPNo(ucMPNo,1,0);
//	
//	stHisVal10=Get_History_UnDirPower_byMPNo(ucMPNo,0,0);
//	stHisVal11=Get_History_UnDirPower_byMPNo(ucMPNo,1,0);
//	
//	stHisVal20=Get_History_DirVar_byMPNo(ucMPNo,0,0);
//	stHisVal21=Get_History_DirVar_byMPNo(ucMPNo,1,0);
//	
//	stHisVal30=Get_History_UnDirVar_byMPNo(ucMPNo,0,0);
//	stHisVal31=Get_History_UnDirVar_byMPNo(ucMPNo,1,0);
//	
//	stHisI0A = Get_History_I_byMPNo(ucMPNo,0,1);
//	stHisI0B = Get_History_I_byMPNo(ucMPNo,0,2);
//	stHisI0C = Get_History_I_byMPNo(ucMPNo,0,3);
//	
//	stHisI1A = Get_History_I_byMPNo(ucMPNo,1,1);
//	stHisI1B = Get_History_I_byMPNo(ucMPNo,1,2);
//	stHisI1C = Get_History_I_byMPNo(ucMPNo,1,3);
//	
//	if(	stHisI0A.lVal  !=CONINVALIDVAL && stHisI0B.lVal  !=CONINVALIDVAL && stHisI0C.lVal  !=CONINVALIDVAL &&
//		stHisI1A.lVal  !=CONINVALIDVAL && stHisI1B.lVal  !=CONINVALIDVAL && stHisI1C.lVal  !=CONINVALIDVAL &&
//		stHisVal00.lVal!=CONINVALIDVAL && stHisVal01.lVal!=CONINVALIDVAL && stHisVal10.lVal!=CONINVALIDVAL && stHisVal11.lVal != CONINVALIDVAL &&
//		stHisVal20.lVal!=CONINVALIDVAL && stHisVal21.lVal!=CONINVALIDVAL && stHisVal30.lVal!=CONINVALIDVAL && stHisVal31.lVal != CONINVALIDVAL )
//	{
//		ulAveI0	 = Get_MeterAveCurrent(ucMPNo,stHisI0A,stHisI0B,stHisI0C);
//		ulAveI1	 = Get_MeterAveCurrent(ucMPNo,stHisI1A,stHisI1B,stHisI1C);
//		ulFixVal = Get_MeterStop_FixVal(ucMPNo);
//		ulFixVal = ulFixVal*2/ucCopyInter;
//		if(ulFixVal<100)
//			ulFixVal = 100;				//�������1A
//		if(ulAveI0 > ulFixVal && ulAveI1 > ulFixVal )
//		{
//			ulTimeDiff=SubDateTime_Second(&(stHisVal00.stDateTime),&(stHisVal01.stDateTime));
//			if( (ulTimeDiff>=(unsigned long )ucCopyInter*60-5 )&& 
//				(ulTimeDiff<(unsigned long )(ucCopyInter*60+CON_ALR_TIMEINTER_COEF) ) )
//			{
//				if(	stHisVal00.lVal == stHisVal01.lVal && stHisVal10.lVal == stHisVal11.lVal &&
//					stHisVal20.lVal == stHisVal21.lVal && stHisVal30.lVal == stHisVal31.lVal &&
//					stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterStop	== CONALR_STAT_HAPPEN_NOT)
//				{
//					if(Alarm_CheckForeJudgeList(CONALRCODE_METERSTOP,ucMPNo)!=RET_SUCCESS)	//�Ѿ��澯���ڣ����ж�
//						Alarm_FillForeJudgeList(CONALRCODE_METERSTOP,ucMPNo);
//				}
//			}
//		}
//		if(	stHisVal00.lVal == stHisVal01.lVal && stHisVal10.lVal == stHisVal11.lVal &&
//			stHisVal20.lVal == stHisVal21.lVal && stHisVal30.lVal == stHisVal31.lVal)
//		{
//		}
//		else if(stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterStop	== CONALR_STAT_HAPPEN)
//		{//�ָ�
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterStop = CONALR_STAT_HAPPEN_NOT;
//		}
//	}
//	
//	return ucRet;
//}

//unsigned char Alarm_ForeJudge_MeterDown(unsigned char ucMPNo)
//{//��Ƶ���(�����й�����)
//	unsigned char ucRet=RET_SUCCESS;
//	STHISVAL	  stHisVal0;
//	STHISVAL	  stHisVal1;
//	
//	stHisVal0=Get_History_DirPower_byMPNo(ucMPNo,0,0);
//	stHisVal1=Get_History_DirPower_byMPNo(ucMPNo,1,0);
//	ucRet =  Chk_MeterDown(stHisVal0,stHisVal1);
//	if(ucRet==RET_ERROR)
//	{
//		stHisVal0=Get_History_DirPower_byMPNo(ucMPNo,0,1);		
//		stHisVal1=Get_History_DirPower_byMPNo(ucMPNo,1,1);
//		ucRet =  Chk_MeterDown(stHisVal0,stHisVal1);
//	}
//	if(ucRet==RET_ERROR)
//	{
//		stHisVal0=Get_History_DirPower_byMPNo(ucMPNo,0,2);		
//		stHisVal1=Get_History_DirPower_byMPNo(ucMPNo,1,2);
//		ucRet =  Chk_MeterDown(stHisVal0,stHisVal1);
//	}
//	if(ucRet==RET_ERROR)
//	{
//		stHisVal0=Get_History_DirPower_byMPNo(ucMPNo,0,3);		
//		stHisVal1=Get_History_DirPower_byMPNo(ucMPNo,1,3);
//		ucRet =  Chk_MeterDown(stHisVal0,stHisVal1);
//	}
//	if(ucRet==RET_ERROR)
//	{
//		stHisVal0=Get_History_DirPower_byMPNo(ucMPNo,0,4);		
//		stHisVal1=Get_History_DirPower_byMPNo(ucMPNo,1,4);
//		ucRet =  Chk_MeterDown(stHisVal0,stHisVal1);
//	}
//	if(ucRet==RET_ERROR)
//	{
//		stHisVal0=Get_History_UnDirPower_byMPNo(ucMPNo,0,0);		
//		stHisVal1=Get_History_UnDirPower_byMPNo(ucMPNo,1,0);
//		ucRet =  Chk_MeterDown(stHisVal0,stHisVal1);
//	}
//	if(ucRet==RET_ERROR)
//	{
//		stHisVal0=Get_History_UnDirPower_byMPNo(ucMPNo,0,1);		
//		stHisVal1=Get_History_UnDirPower_byMPNo(ucMPNo,1,1);
//		ucRet =  Chk_MeterDown(stHisVal0,stHisVal1);
//	}
//	if(ucRet==RET_ERROR)
//	{
//		stHisVal0=Get_History_UnDirPower_byMPNo(ucMPNo,0,2);		
//		stHisVal1=Get_History_UnDirPower_byMPNo(ucMPNo,1,2);
//		ucRet =  Chk_MeterDown(stHisVal0,stHisVal1);
//	}
//	if(ucRet==RET_ERROR)
//	{
//		stHisVal0=Get_History_UnDirPower_byMPNo(ucMPNo,0,3);		
//		stHisVal1=Get_History_UnDirPower_byMPNo(ucMPNo,1,3);
//		ucRet =  Chk_MeterDown(stHisVal0,stHisVal1);
//	}
//	if(ucRet==RET_ERROR)
//	{
//		stHisVal0=Get_History_UnDirPower_byMPNo(ucMPNo,0,4);		
//		stHisVal1=Get_History_UnDirPower_byMPNo(ucMPNo,1,4);
//		ucRet =  Chk_MeterDown(stHisVal0,stHisVal1);
//	}
//	if(ucRet==RET_ERROR)
//	{
//		stHisVal0=Get_History_DirVar_byMPNo(ucMPNo,0,0);		
//		stHisVal1=Get_History_DirVar_byMPNo(ucMPNo,1,0);
//		ucRet =  Chk_MeterDown(stHisVal0,stHisVal1);
//	}
//	if(ucRet==RET_ERROR)
//	{
//		stHisVal0=Get_History_DirVar_byMPNo(ucMPNo,0,1);		
//		stHisVal1=Get_History_DirVar_byMPNo(ucMPNo,1,1);
//		ucRet =  Chk_MeterDown(stHisVal0,stHisVal1);
//	}
//	if(ucRet==RET_ERROR)
//	{
//		stHisVal0=Get_History_DirVar_byMPNo(ucMPNo,0,2);		
//		stHisVal1=Get_History_DirVar_byMPNo(ucMPNo,1,2);
//		ucRet =  Chk_MeterDown(stHisVal0,stHisVal1);
//	}
//	if(ucRet==RET_ERROR)
//	{
//		stHisVal0=Get_History_DirVar_byMPNo(ucMPNo,0,3);	
//		stHisVal1=Get_History_DirVar_byMPNo(ucMPNo,1,3);
//		ucRet =  Chk_MeterDown(stHisVal0,stHisVal1);
//	}
//	if(ucRet==RET_ERROR)
//	{
//		stHisVal0=Get_History_DirVar_byMPNo(ucMPNo,0,4);	
//		stHisVal1=Get_History_DirVar_byMPNo(ucMPNo,1,4);
//		ucRet =  Chk_MeterDown(stHisVal0,stHisVal1);
//	}
//	if(ucRet==RET_ERROR)
//	{
//		stHisVal0=Get_History_UnDirVar_byMPNo(ucMPNo,0,0);
//		stHisVal1=Get_History_UnDirVar_byMPNo(ucMPNo,1,0);
//		ucRet =  Chk_MeterDown(stHisVal0,stHisVal1);
//	}
//	if(ucRet==RET_ERROR)
//	{
//		stHisVal0=Get_History_UnDirVar_byMPNo(ucMPNo,0,1);	
//		stHisVal1=Get_History_UnDirVar_byMPNo(ucMPNo,1,1);
//		ucRet =  Chk_MeterDown(stHisVal0,stHisVal1);
//	}
//	if(ucRet==RET_ERROR)
//	{
//		stHisVal0=Get_History_UnDirVar_byMPNo(ucMPNo,0,2);
//		stHisVal1=Get_History_UnDirVar_byMPNo(ucMPNo,1,2);
//		ucRet =  Chk_MeterDown(stHisVal0,stHisVal1);
//	}
//	if(ucRet==RET_ERROR)
//	{
//		stHisVal0=Get_History_UnDirVar_byMPNo(ucMPNo,0,3);	
//		stHisVal1=Get_History_UnDirVar_byMPNo(ucMPNo,1,3);
//		ucRet =  Chk_MeterDown(stHisVal0,stHisVal1);
//	}
//	if(ucRet==RET_ERROR)
//	{
//		stHisVal0=Get_History_UnDirVar_byMPNo(ucMPNo,0,4);
//		stHisVal1=Get_History_UnDirVar_byMPNo(ucMPNo,1,4);
//		ucRet =  Chk_MeterDown(stHisVal0,stHisVal1);
//	}
//	
//	if(ucRet==RET_SUCCESS)
//	{
//		if(Alarm_CheckEnsureJudgeList(CONALRCODE_METERDOWN,ucMPNo)!=RET_SUCCESS)	//�Ѿ��澯���ڣ����ж�
//		{
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterDown	= CONALR_STAT_HAPPEN;
//			Alarm_FillEnsureJudgeList(CONALRCODE_METERDOWN,ucMPNo);
//		}
//	}else
//		stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterDown	= CONALR_STAT_HAPPEN_NOT;
//	
//	return ucRet;
//}
//
//unsigned char Alarm_ForeJudge_MeterBattLow(unsigned char ucMPNo)
//{//��Ƶ�ص�ѹ��
//	unsigned char ucRet=RET_SUCCESS;
//	STHISSTATE	  stHisState;
//	
//	stHisState=Get_History_MeterStatFlag_byMPNo(ucMPNo,0,1);
//	if(CONINVALIDVAL != stHisState.ucStateFlag)
//	{
//		if(	(stHisState.ucStateFlag & 0x04)== 0x04 &&
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterBattLow	== CONALR_STAT_HAPPEN_NOT)
//		{
//			if(Alarm_CheckForeJudgeList(CONALRCODE_METERBATTLOW,ucMPNo)!=RET_SUCCESS)	//�Ѿ��澯���ڣ����ж�
//				Alarm_FillForeJudgeList(CONALRCODE_METERBATTLOW,ucMPNo);
//		}
//	}
//	return ucRet;
//}
//unsigned char Alarm_ForeJudge_MeterTimeErr(unsigned char ucMPNo)
//{//���ʱ�Ӵ���
//	unsigned char ucRet=RET_SUCCESS;
//	STHISTIME	  stHisDate,stHisTime;
//	STDATETIME	  stCurrTime,stMeterTime;
//	unsigned long ulTimeDiff=0;
//	unsigned char   ucMeterTimeDiff=0;
//	
//	stHisDate=Get_History_Date_byMPNo(ucMPNo,0);
//	stHisTime=Get_History_Time_byMPNo(ucMPNo,0);
//	
//	stMeterTime = stHisDate.stHisTime;
//	stMeterTime.ucHour	= stHisTime.stHisTime.ucHour;
//	stMeterTime.ucMinute= stHisTime.stHisTime.ucMinute;
//	stMeterTime.ucSecond= stHisTime.stHisTime.ucSecond;
//	if(Is_DateTime_Availble(&stMeterTime)==RET_SUCCESS)
//	{
//		stCurrTime = GetCurrentTime();
//		ulTimeDiff = SubDateTime(&stCurrTime,&stMeterTime);
//		ucMeterTimeDiff=RunPara_GetTerMisc_MeterTimeDiff();
//		if(	ulTimeDiff > ucMeterTimeDiff && stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterTimeErr	== CONALR_STAT_HAPPEN_NOT)
//		{
//			if(Alarm_CheckForeJudgeList(CONALRCODE_METERTIMEERR,ucMPNo)!=RET_SUCCESS)	//�Ѿ��澯���ڣ����ж�
//				Alarm_FillForeJudgeList(CONALRCODE_METERTIMEERR,ucMPNo);
//		}else if(ulTimeDiff < ucMeterTimeDiff && stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterTimeErr	== CONALR_STAT_HAPPEN)
//		{
//			if(Alarm_CheckForeJudgeList(CONALRCODE_METERTIMEERR,ucMPNo)!=RET_SUCCESS)	//�Ѿ��澯���ڣ����ж�
//				Alarm_FillForeJudgeList(CONALRCODE_METERTIMEERR,ucMPNo);
//		}
//	}
//	
//	return ucRet;
//}

//unsigned char Alarm_ForeJudge_ValOverUp(unsigned char ucMPNo,STHISVAL stChkVal,unsigned long ulUpLimit,unsigned short usBack,unsigned char *pucAlrStat,unsigned short usAlrCode)
//{//+-1%�ĸ������Է�ֹƵ���ϱ�
//	unsigned char ucRet=RET_ERROR;
//	if(stChkVal.lVal!=CONINVALIDVAL)
//	{
//		if(stChkVal.lVal>(unsigned long)(ulUpLimit*CON_UPFLOAT_COEF/10))
//		{//����澯�Ѿ����ڣ�����Ҫ�澯
//			if((*pucAlrStat)==CONALR_STAT_HAPPEN_NOT)
//			{
//				if(Alarm_CheckForeJudgeList(usAlrCode,ucMPNo)!=RET_SUCCESS)	//�Ѿ��澯���ڣ����ж�
//					ucRet=RET_SUCCESS;
//			}
//		}else if(stChkVal.lVal < (unsigned long)(ulUpLimit*(usBack/10)/10))
//		{
//			if((*pucAlrStat)==CONALR_STAT_HAPPEN)
//			{
//				if(Alarm_CheckForeJudgeList(usAlrCode,ucMPNo)!=RET_SUCCESS)	//�Ѿ��澯���ڣ����ж�
//					ucRet=RET_SUCCESS;
//			}
//		}
//	}
//	
//	return ucRet;
//}
//
//unsigned char Alarm_ForeJudge_ValOverDown(unsigned char ucMPNo,STHISVAL stChkVal,unsigned long ulDownLimit,unsigned short usBack,unsigned char *pucAlrStat,unsigned short usAlrCode)
//{//+-1%�ĸ������Է�ֹƵ���ϱ�
//	unsigned char ucRet=RET_ERROR;
//	if(stChkVal.lVal!=CONINVALIDVAL)
//	{
//		if(stChkVal.lVal<(unsigned long)(ulDownLimit*CON_DOWNFLOAT_COEF/10))
//		{//����澯�Ѿ����ڣ�����Ҫ�澯
//			if((*pucAlrStat)==CONALR_STAT_HAPPEN_NOT)
//			{
//				if(Alarm_CheckForeJudgeList(usAlrCode,ucMPNo)!=RET_SUCCESS)	//�Ѿ��澯���ڣ����ж�
//					ucRet=RET_SUCCESS;
//			}
//		}else if(stChkVal.lVal > (unsigned long)(ulDownLimit*(usBack/10)/10))
//		{
//			if((*pucAlrStat)==CONALR_STAT_HAPPEN)
//			{
//				if(Alarm_CheckForeJudgeList(usAlrCode,ucMPNo)!=RET_SUCCESS)	//�Ѿ��澯���ڣ���bu  �ж�
//					ucRet=RET_SUCCESS;
//			}
//		}
//	}
//	
//	return ucRet;
//}
//
//unsigned char Alarm_ForeJudge_VolBBreak(unsigned char ucMPNo,STHISVAL stChkValA,STHISVAL stChkValC,unsigned long ulDownLimit,unsigned char *pucAlrStat,unsigned short usAlrCode)
//{
//	unsigned char ucRet=RET_ERROR;
//	if(stChkValA.lVal!=CONINVALIDVAL && stChkValC.lVal != CONINVALIDVAL)
//	{
//		stChkValA.lVal = stChkValA.lVal / 10;
//		stChkValC.lVal = stChkValC.lVal / 10;
//		if( stChkValA.lVal > (CON_RATEVOL_3P3L/2-10) && stChkValA.lVal < (CON_RATEVOL_3P3L/2+10) &&
//			stChkValC.lVal > (CON_RATEVOL_3P3L/2-10) && stChkValC.lVal < (CON_RATEVOL_3P3L/2+10))
//		{//����澯�Ѿ����ڣ�����Ҫ�澯
//			if((*pucAlrStat)==CONALR_STAT_HAPPEN_NOT)
//			{
//				if(Alarm_CheckForeJudgeList(usAlrCode,ucMPNo)!=RET_SUCCESS)	//�Ѿ��澯���ڣ����ж�
//					ucRet=RET_SUCCESS;
//			}
//		}else
//		{
//			if((*pucAlrStat)==CONALR_STAT_HAPPEN)
//			{
//				if(Alarm_CheckForeJudgeList(usAlrCode,ucMPNo)!=RET_SUCCESS)	//�Ѿ��澯���ڣ����ж�
//					ucRet=RET_SUCCESS;
//			}
//		}
//	}
//	
//	return ucRet;
//}

/********************************************�澯ȷ��************************************************************/
////��ѹAԽ����ȷ��
//unsigned char Alarm_EnsureJudge_VaOverUp(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	unsigned char	ucMPNo=0;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	Alarm_EnsureJudge_ValOverUp(pstAlrForeJudge,stMPLimit.usVolErrUpLimit,stMPLimit.ucVolErrUpBack, ucListNo,CON_DI_TYPE_MPVOLA,
//		&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolaUpOver));
//	return ucRet;
//}
//
////��ѹBԽ����ȷ��
//unsigned char Alarm_EnsureJudge_VbOverUp(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	unsigned char	ucMPNo=0;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	Alarm_EnsureJudge_ValOverUp(pstAlrForeJudge,stMPLimit.usVolErrUpLimit,stMPLimit.ucVolErrUpBack,ucListNo,CON_DI_TYPE_MPVOLB,
//		&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolbUpOver));
//	return ucRet;
//}
//
////��ѹCԽ����ȷ��
//unsigned char Alarm_EnsureJudge_VcOverUp(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	unsigned char	ucMPNo=0;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	Alarm_EnsureJudge_ValOverUp(pstAlrForeJudge,stMPLimit.usVolErrUpLimit,stMPLimit.ucVolErrUpBack,ucListNo,CON_DI_TYPE_MPVOLC,
//		&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolcUpOver));
//	return ucRet;
//}
//
////��ѹAԽ����ȷ��
//unsigned char Alarm_EnsureJudge_VaOverDown(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	unsigned char	ucMPNo=0;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	Alarm_EnsureJudge_ValOverDown(pstAlrForeJudge,stMPLimit.usVolErrDownLimit,stMPLimit.ucVolErrDownBack,ucListNo,CON_DI_TYPE_MPVOLA,
//		&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolaDownOver));
//	return ucRet;
//}
//
////��ѹBԽ����ȷ��
//unsigned char Alarm_EnsureJudge_VbOverDown(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STMPRATEVAL		stMPRateVal;
//	unsigned char	ucMPNo=0;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	stMPRateVal = RunPara_GetMPRateVal(ucMPNo);
//	if(stMPRateVal.ucMPLineType == CON_METERTYPE_3P4L)
//	{
//		Alarm_EnsureJudge_ValOverDown(pstAlrForeJudge,stMPLimit.usVolErrDownLimit,stMPLimit.ucVolErrDownBack,ucListNo,CON_DI_TYPE_MPVOLB,
//			&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolbDownOver));
//	}
//	return ucRet;
//}
//
////��ѹCԽ����ȷ��
//unsigned char Alarm_EnsureJudge_VcOverDown(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	unsigned char	ucMPNo=0;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	Alarm_EnsureJudge_ValOverDown(pstAlrForeJudge,stMPLimit.usVolErrDownLimit,stMPLimit.ucVolErrDownBack,ucListNo,CON_DI_TYPE_MPVOLC,
//		&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolcDownOver));
//	return ucRet;
//}
//
////����AԽ����ȷ��
//unsigned char Alarm_EnsureJudge_IaOverUp(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	unsigned char	ucMPNo=0;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	Alarm_EnsureJudge_ValOverUp(pstAlrForeJudge,stMPLimit.usCurUpLimit,stMPLimit.ucCurUpBack,ucListNo,CON_DI_TYPE_MPCURA,
//		&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_IaOverLoad));
//	return ucRet;
//}
//
////����BԽ����ȷ��
//unsigned char Alarm_EnsureJudge_IbOverUp(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	unsigned char	ucMPNo=0;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	Alarm_EnsureJudge_ValOverUp(pstAlrForeJudge,stMPLimit.usCurUpLimit,stMPLimit.ucCurUpBack,ucListNo,CON_DI_TYPE_MPCURB,
//		&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_IbOverLoad));
//	return ucRet;
//}
//
////����CԽ����ȷ��
//unsigned char Alarm_EnsureJudge_IcOverUp(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	unsigned char	ucMPNo=0;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	Alarm_EnsureJudge_ValOverUp(pstAlrForeJudge,stMPLimit.usCurUpLimit,stMPLimit.ucCurUpBack,ucListNo,CON_DI_TYPE_MPCURC,
//		&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_IcOverLoad));
//	return ucRet;
//}
//
////����AԽ������ȷ��
//unsigned char Alarm_EnsureJudge_IaHeavyOverUp(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	unsigned char	ucMPNo=0;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	Alarm_EnsureJudge_ValOverUp(pstAlrForeJudge,stMPLimit.usCurErrUpLimit,stMPLimit.ucCurErrUpBack,ucListNo,CON_DI_TYPE_MPCURA,
//		&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_IaErrOverLoad));
//	return ucRet;
//}
//
////����BԽ������ȷ��
//unsigned char Alarm_EnsureJudge_IbHeavyOverUp(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	unsigned char	ucMPNo=0;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	Alarm_EnsureJudge_ValOverUp(pstAlrForeJudge,stMPLimit.usCurErrUpLimit,stMPLimit.ucCurErrUpBack,ucListNo,CON_DI_TYPE_MPCURB,
//		&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_IbErrOverLoad));
//	return ucRet;
//}
//
////����CԽ������ȷ��
//unsigned char Alarm_EnsureJudge_IcHeavyOverUp(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	unsigned char	ucMPNo=0;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	Alarm_EnsureJudge_ValOverUp(pstAlrForeJudge,stMPLimit.usCurErrUpLimit,stMPLimit.ucCurErrUpBack,ucListNo,CON_DI_TYPE_MPCURC,
//		&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_IcErrOverLoad));
//	return ucRet;
//}
//
////A���ѹ����
//unsigned char Alarm_EnsureJudge_VaBreak(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	unsigned char	ucMPNo=0;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_Cur;
//	unsigned long	ul05perCur;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	stMPRateVal = RunPara_GetMPRateVal(ucMPNo);
//	ul05perCur=(unsigned long)(stMPRateVal.usRateCur * 5/10);
//	if(ul05perCur==0)
//		ul05perCur=1;
//	
//	stHisVal_Cur= Get_History_I_byMPNo(ucMPNo,0,1);
//	if(((stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolaBreak==CONALR_STAT_HAPPEN_NOT) && (stHisVal_Cur.lVal <= ul05perCur))
//		|| (stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolaBreak==CONALR_STAT_HAPPEN))
//	{
//		Alarm_EnsureJudge_ValOverDown(pstAlrForeJudge,stMPLimit.usVolBreakLimit,1010,ucListNo,CON_DI_TYPE_MPVOLA,
//			&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolaBreak));
//	}
//	
//	return ucRet;
//}
//
////B���ѹ����
//unsigned char Alarm_EnsureJudge_VbBreak(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STMPRATEVAL		stMPRateVal;
//	unsigned char	ucMPNo=0;
//	STHISVAL		stHisVal_Cur;
//	unsigned long	ul05perCur;
//	
//	ucMPNo		= pstAlrForeJudge->ucMPNo;
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	stMPRateVal = RunPara_GetMPRateVal(ucMPNo);
//	ul05perCur=(unsigned long)(stMPRateVal.usRateCur * 5/10);
//	if(ul05perCur==0)
//		ul05perCur=1;
//
//	if(stMPRateVal.ucMPLineType== CON_METERTYPE_3P4L)
//	{
//		stHisVal_Cur= Get_History_I_byMPNo(ucMPNo,0,2);
//		if(((stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolbBreak==CONALR_STAT_HAPPEN_NOT) && (stHisVal_Cur.lVal <= ul05perCur))
//			|| (stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolbBreak==CONALR_STAT_HAPPEN))
//		{
//			Alarm_EnsureJudge_ValOverDown(pstAlrForeJudge,stMPLimit.usVolBreakLimit,1010,ucListNo,CON_DI_TYPE_MPVOLB,
//				&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolbBreak));
//		}
//	}else
//	{
//		Alarm_EnsureJudge_VolBBreak(pstAlrForeJudge,stMPLimit.usVolBreakLimit,ucListNo,
//			&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolbBreak));
//	}
//	return ucRet;
//}
//
////C���ѹ����
//unsigned char Alarm_EnsureJudge_VcBreak(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	unsigned char	ucMPNo=0;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_Cur;
//	unsigned long	ul05perCur;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	stMPRateVal = RunPara_GetMPRateVal(ucMPNo);
//	ul05perCur=(unsigned long)(stMPRateVal.usRateCur * 5/10);
//	if(ul05perCur==0)
//		ul05perCur=1;
//	
//	stHisVal_Cur= Get_History_I_byMPNo(ucMPNo,0,3);
//	if(((stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolcBreak==CONALR_STAT_HAPPEN_NOT) && (stHisVal_Cur.lVal <= ul05perCur))
//		|| (stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolcBreak==CONALR_STAT_HAPPEN))
//	{
//		Alarm_EnsureJudge_ValOverDown(pstAlrForeJudge,stMPLimit.usVolBreakLimit,1010,ucListNo,CON_DI_TYPE_MPVOLC,
//			&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolcBreak));
//	}
//
//	return ucRet;
//}
//
////A���ѹʧѹ
//unsigned char Alarm_EnsureJudge_VaLack(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	unsigned char	ucMPNo=0;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_Cur;
//	unsigned long	ul05perCur;
//
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	stMPRateVal = RunPara_GetMPRateVal(ucMPNo);
//	ul05perCur=(unsigned long)(stMPRateVal.usRateCur * 5/10);
//	if(ul05perCur==0)
//		ul05perCur=1;
//
//	stHisVal_Cur= Get_History_I_byMPNo(ucMPNo,0,1);
//	if(((stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolaLack==CONALR_STAT_HAPPEN_NOT) && (stHisVal_Cur.lVal > ul05perCur))
//		|| (stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolaLack==CONALR_STAT_HAPPEN))
//	{
//		Alarm_EnsureJudge_ValOverDown(pstAlrForeJudge,stMPLimit.usVolBreakLimit,1010,ucListNo,CON_DI_TYPE_MPVOLA,
//			&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolaLack));
//	}
//	
//	return ucRet;
//}
//
////B���ѹʧѹ
//unsigned char Alarm_EnsureJudge_VbLack(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STMPRATEVAL		stMPRateVal;
//	unsigned char	ucMPNo=0;
//	STHISVAL		stHisVal_Cur;
//	unsigned long	ul05perCur;
//	
//	ucMPNo		= pstAlrForeJudge->ucMPNo;
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	stMPRateVal = RunPara_GetMPRateVal(ucMPNo);
//	ul05perCur=(unsigned long)(stMPRateVal.usRateCur * 5/10);
//	if(ul05perCur==0)
//		ul05perCur=1;
//
//	if(stMPRateVal.ucMPLineType== CON_METERTYPE_3P4L)
//	{
//		stHisVal_Cur= Get_History_I_byMPNo(ucMPNo,0,2);
//		if(((stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolbLack==CONALR_STAT_HAPPEN_NOT) && (stHisVal_Cur.lVal > ul05perCur))
//			|| (stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolbLack==CONALR_STAT_HAPPEN))
//		{
//			Alarm_EnsureJudge_ValOverDown(pstAlrForeJudge,stMPLimit.usVolBreakLimit,1010,ucListNo,CON_DI_TYPE_MPVOLB,
//				&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolbLack));
//		}
//	}
//	return ucRet;
//}
//
////C���ѹʧѹ
//unsigned char Alarm_EnsureJudge_VcLack(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	unsigned char	ucMPNo=0;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_Cur;
//	unsigned long	ul05perCur;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	stMPRateVal = RunPara_GetMPRateVal(ucMPNo);
//	ul05perCur=(unsigned long)(stMPRateVal.usRateCur * 5/10);
//	if(ul05perCur==0)
//		ul05perCur=1;
//
//	stHisVal_Cur= Get_History_I_byMPNo(ucMPNo,0,3);
//	if(((stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolcLack==CONALR_STAT_HAPPEN_NOT) && (stHisVal_Cur.lVal > ul05perCur))
//		|| (stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolcLack==CONALR_STAT_HAPPEN))
//	{
//		Alarm_EnsureJudge_ValOverDown(pstAlrForeJudge,stMPLimit.usVolBreakLimit,1010,ucListNo,CON_DI_TYPE_MPVOLC,
//			&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_VolcLack));
//	}
//	
//	return ucRet;
//}
//
////���ڹ���Խ��
//unsigned char Alarm_EnsureJudge_SPowOverUp(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	unsigned char	ucMPNo=0;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	Alarm_EnsureJudge_SQRTOverUp(pstAlrForeJudge,stMPLimit.ulPowUpLimit,stMPLimit.ucPowUpBack,ucListNo,
//		CON_DI_TYPE_MPINSTANTPOW,CON_DI_TYPE_MPINSTANTVAR,	&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_SPowOverUp));
//	return ucRet;
//}
//
////���ڹ���Խ������
//unsigned char Alarm_EnsureJudge_SPowErrUp(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	unsigned char	ucMPNo=0;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	Alarm_EnsureJudge_SQRTOverUp(pstAlrForeJudge,stMPLimit.ulPowErrUpLimit,stMPLimit.ucPowErrUpBack,ucListNo,
//		CON_DI_TYPE_MPINSTANTPOW,CON_DI_TYPE_MPINSTANTVAR,	&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_SPowErrUp));
//	return ucRet;
//}
//
////���������ƽ���ȷ��
//unsigned char Alarm_EnsureJudge_IUnBalanceOverUp(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	unsigned char	ucMPNo=0;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	Alarm_EnsureJudge_ValUnBalanceOverUp(pstAlrForeJudge,stMPLimit.usCurUnBalance,stMPLimit.ucCurUnBalanceBack,ucListNo,
//		CON_DI_TYPE_MPCURA,CON_DI_TYPE_MPCURB,CON_DI_TYPE_MPCURC,
//		&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_IUnbalance));
//	return ucRet;
//}
//
////�����ѹ��ƽ���ȷ��
//unsigned char Alarm_EnsureJudge_UUnBalanceOverUp(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	unsigned char	ucMPNo=0;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	stMPLimit = RunPara_GetMPLimit(ucMPNo);
//	
//	Alarm_EnsureJudge_ValUnBalanceOverUp(pstAlrForeJudge,stMPLimit.usVolUnBalance,stMPLimit.ucVolUnBalanceBack,ucListNo,
//		CON_DI_TYPE_MPVOLA,CON_DI_TYPE_MPVOLB,CON_DI_TYPE_MPVOLC,
//		&(stAlarmStatus[ucMPNo].ucAlrStat_Sys_UUnbalance));
//	return ucRet;
//}

//unsigned char Alarm_EnsureJudge_ProgramTime(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{//���ʱ�����ȷ��
//	unsigned char	ucRet=RET_SUCCESS;
//	STHISTIME		stHisTime;
//	STHISTIME		stHisTime0,stHisTime1;
//	unsigned char	ucMPNo=0;
//	unsigned char	ucMeterNo=0;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	ucMeterNo=RunPara_GetMeterNo_byMPNo(ucMPNo);
//	stHisTime0=Get_History_ProgramTime_byMPNo(ucMPNo,0);
//	stHisTime1=Get_History_ProgramTime_byMPNo(ucMPNo,1);
//	
//	Alarm_GetHisTime_ForeJudge(ucListNo,CON_DI_TYPE_MPLATELYPROGTIME,0,&stHisTime);
//	
//	if(	Is_DateTime_Availble(&stHisTime0.stHisTime)==RET_SUCCESS && 
//		Is_DateTime_Availble(&stHisTime1.stHisTime)==RET_SUCCESS &&
//		Is_DateTime_Availble(&stHisTime.stHisTime)==RET_SUCCESS)
//	{
//		if(	Is_DateTime_Equel(&stHisTime0.stHisTime,&stHisTime1.stHisTime)==RET_ERROR &&
//			Is_DateTime_Equel(&stHisTime0.stHisTime,&stHisTime.stHisTime) ==RET_SUCCESS)
//		{
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_ProgramTime	= CONALR_STAT_HAPPEN;
//			Alarm_FillEnsureJudgeList(CONALRCODE_PROGRAMTIME,ucMPNo);
//		}else if(Is_DateTime_Equel(&stHisTime0.stHisTime,&stHisTime1.stHisTime)==RET_ERROR &&
//			Is_DateTime_Equel(&stHisTime1.stHisTime,&stHisTime.stHisTime) ==RET_SUCCESS)
//		{//���T0,T1,T2�У�T0!=T1,T2==T0,�����ΪT1����,�ɽ��串��֮
//			Add_History_ProgramTime(ucMeterNo,stHisTime.stHisTime);
//			Add_History_ProgramTime(ucMeterNo,stHisTime.stHisTime);
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_ProgramTime	= CONALR_STAT_HAPPEN_NOT;
//		}else
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_ProgramTime	= CONALR_STAT_HAPPEN_NOT;
//	}
//	return ucRet;
//}
//
//unsigned char Alarm_EnsureJudge_ProgramCount(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{//��̴������ȷ��
//	unsigned char	ucRet=RET_SUCCESS;
//	STHISVAL		stHisVal;
//	STHISVAL		stHisVal0,stHisVal1;
//	unsigned char	ucMPNo=0;
//	unsigned char	ucMeterNo=0;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	ucMeterNo=RunPara_GetMeterNo_byMPNo(ucMPNo);
//	stHisVal0=Get_History_ProgramCount_byMPNo(ucMPNo,0);
//	stHisVal1=Get_History_ProgramCount_byMPNo(ucMPNo,1);
//	
//	Alarm_GetHisVal_ForeJudge(ucListNo,CON_DI_TYPE_MPPROGTIMETIMES,0,&stHisVal);
//	
//	if(stHisVal.lVal!=CONINVALIDVAL && stHisVal0.lVal!=CONINVALIDVAL && stHisVal1.lVal!=CONINVALIDVAL)
//	{
//		if(	stHisVal0.lVal!=stHisVal1.lVal && stHisVal0.lVal==stHisVal.lVal)
//		{
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_ProgramCount	= CONALR_STAT_HAPPEN;
//			Alarm_FillEnsureJudgeList(CONALRCODE_PROGRAMNUM,ucMPNo);
//		}
//		else if( stHisVal0.lVal!=stHisVal1.lVal && stHisVal1.lVal==stHisVal.lVal )
//		{//���0,1,2�У�0!=1,2==0,�����Ϊ1����,�ɽ��串��֮
//			Add_History_ProgramCount(ucMeterNo,stHisVal.lVal);
//			Add_History_ProgramCount(ucMeterNo,stHisVal.lVal);
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_ProgramCount	= CONALR_STAT_HAPPEN_NOT;
//		}else
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_ProgramCount	= CONALR_STAT_HAPPEN_NOT;
//	}
//	return ucRet;
//}
//
//unsigned char Alarm_EnsureJudge_PulseConst(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{//���峣������ȷ��
//	unsigned char	ucRet=RET_SUCCESS;
//	STHISVAL		stHisVal;
//	STHISVAL		stHisVal0,stHisVal1;
//	unsigned char	ucMPNo=0;
//	unsigned char	ucMeterNo=0;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	ucMeterNo=RunPara_GetMeterNo_byMPNo(ucMPNo);
//	stHisVal0=Get_History_PowPulseConst_byMPNo(ucMPNo,0);
//	stHisVal1=Get_History_PowPulseConst_byMPNo(ucMPNo,1);
//	
//	Alarm_GetHisVal_ForeJudge(ucListNo,CON_DI_TYPE_MPPOWCONST,0,&stHisVal);
//	
//	if(stHisVal.lVal!=CONINVALIDVAL && stHisVal0.lVal!=CONINVALIDVAL && stHisVal1.lVal!=CONINVALIDVAL)
//	{
//		if(	stHisVal0.lVal!=stHisVal1.lVal && stHisVal0.lVal==stHisVal.lVal)
//		{
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_PulseConst	= CONALR_STAT_HAPPEN;
//			Alarm_FillEnsureJudgeList(pstAlrForeJudge->usAlrCode,ucMPNo);
//		}else if( stHisVal0.lVal!=stHisVal1.lVal && stHisVal1.lVal==stHisVal.lVal )
//		{//���0,1,2�У�0!=1,2==0,�����Ϊ1����,�ɽ��串��֮
//			Add_History_PowPulseConst(ucMeterNo,stHisVal.lVal);
//			Add_History_PowPulseConst(ucMeterNo,stHisVal.lVal);
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_PulseConst	= CONALR_STAT_HAPPEN_NOT;
//		}else
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_PulseConst	= CONALR_STAT_HAPPEN_NOT;
//	}
//	
//	if(stAlarmStatus[ucMPNo].ucAlrStat_Sys_PulseConst	== CONALR_STAT_HAPPEN_NOT)	//��ʾ�й����峣��δ�����䶯
//	{
//		stHisVal0=Get_History_VarPulseConst_byMPNo(ucMPNo,0);
//		stHisVal1=Get_History_VarPulseConst_byMPNo(ucMPNo,1);
//		
//		Alarm_GetHisVal_ForeJudge(ucListNo,CON_DI_TYPE_MPVARCONST,0,&stHisVal);
//		
//		if(stHisVal.lVal!=CONINVALIDVAL && stHisVal0.lVal!=CONINVALIDVAL && stHisVal1.lVal!=CONINVALIDVAL)
//		{
//			if(	stHisVal0.lVal!=stHisVal1.lVal && stHisVal0.lVal==stHisVal.lVal)
//			{
//				stAlarmStatus[ucMPNo].ucAlrStat_Sys_PulseConst	= CONALR_STAT_HAPPEN;
//				Alarm_FillEnsureJudgeList(pstAlrForeJudge->usAlrCode,ucMPNo);
//			}else if( stHisVal0.lVal!=stHisVal1.lVal && stHisVal1.lVal==stHisVal.lVal )
//			{//���0,1,2�У�0!=1,2==0,�����Ϊ1����,�ɽ��串��֮
//				Add_History_VarPulseConst(ucMeterNo,stHisVal.lVal);
//				Add_History_VarPulseConst(ucMeterNo,stHisVal.lVal);
//				stAlarmStatus[ucMPNo].ucAlrStat_Sys_PulseConst	= CONALR_STAT_HAPPEN_NOT;
//			}else
//				stAlarmStatus[ucMPNo].ucAlrStat_Sys_PulseConst	= CONALR_STAT_HAPPEN_NOT;
//		}
//	}
//	return ucRet;
//}
//
//unsigned char Alarm_EnsureJudge_CopyDayTime(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{//�����ո���ȷ��
//	unsigned char	ucRet=RET_SUCCESS;
//	STHISTIME		stHisTime;
//	STHISTIME		stHisTime0,stHisTime1;
//	unsigned char	ucMPNo=0;
//	unsigned char	ucMeterNo=0;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	ucMeterNo=RunPara_GetMeterNo_byMPNo(ucMPNo);
//	stHisTime0=Get_History_CopyDayTime_byMPNo(ucMPNo,0);
//	stHisTime1=Get_History_CopyDayTime_byMPNo(ucMPNo,1);
//	
//	Alarm_GetHisTime_ForeJudge(ucListNo,CON_DI_TYPE_MPCOPYDAYTIME,0,&stHisTime);
//	
//	if(	Is_DateTime_Availble(&stHisTime0.stHisTime)==RET_SUCCESS && 
//		Is_DateTime_Availble(&stHisTime1.stHisTime)==RET_SUCCESS &&
//		Is_DateTime_Availble(&stHisTime.stHisTime)==RET_SUCCESS)
//	{
//		if(	Is_DateTime_Equel(&stHisTime0.stHisTime,&stHisTime1.stHisTime)==RET_ERROR &&
//			Is_DateTime_Equel(&stHisTime0.stHisTime,&stHisTime.stHisTime) ==RET_SUCCESS)
//		{
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_CopyDayTime	= CONALR_STAT_HAPPEN;
//			Alarm_FillEnsureJudgeList(pstAlrForeJudge->usAlrCode,ucMPNo);
//		}else if(Is_DateTime_Equel(&stHisTime0.stHisTime,&stHisTime1.stHisTime)==RET_ERROR &&
//			Is_DateTime_Equel(&stHisTime1.stHisTime,&stHisTime.stHisTime) ==RET_SUCCESS)
//		{//���0,1,2�У�0!=1,2==0,�����Ϊ1����,�ɽ��串��֮
//			Add_History_CopyDayTime(ucMeterNo,stHisTime.stHisTime);
//			Add_History_CopyDayTime(ucMeterNo,stHisTime.stHisTime);
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_CopyDayTime	= CONALR_STAT_HAPPEN_NOT;
//		}else
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_CopyDayTime	= CONALR_STAT_HAPPEN_NOT;
//	}
//	return ucRet;
//}
//
//unsigned char Alarm_EnsureJudge_LostVolNum(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{//�����������ȷ��
//	unsigned char	ucRet=RET_SUCCESS;
//	STHISVAL		stHisVal;
//	STHISVAL		stHisVal0,stHisVal1;
//	unsigned char	ucMPNo=0;
//	unsigned char	ucMeterNo=0;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	ucMeterNo=RunPara_GetMeterNo_byMPNo(ucMPNo);
//	stHisVal0=Get_History_VolBreakNum_byMPNo(ucMPNo,0,0);
//	stHisVal1=Get_History_VolBreakNum_byMPNo(ucMPNo,1,0);
//	
//	Alarm_GetHisVal_ForeJudge(ucListNo,CON_DI_TYPE_MPLOSTVOLNUM,0,&stHisVal);
//	
//	if(stHisVal.lVal!=CONINVALIDVAL && stHisVal0.lVal!=CONINVALIDVAL && stHisVal1.lVal!=CONINVALIDVAL)
//	{
//		if(	stHisVal0.lVal!=stHisVal1.lVal && stHisVal0.lVal==stHisVal.lVal)
//		{
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_LostVolNum	= CONALR_STAT_HAPPEN;
//			Alarm_FillEnsureJudgeList(pstAlrForeJudge->usAlrCode,ucMPNo);
//		}else if( stHisVal0.lVal!=stHisVal1.lVal && stHisVal1.lVal==stHisVal.lVal )
//		{//���0,1,2�У�0!=1,2==0,�����Ϊ1����,�ɽ��串��֮
//			Add_History_LostVolNum(ucMeterNo,stHisVal.lVal,0);
//			Add_History_LostVolNum(ucMeterNo,stHisVal.lVal,0);
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_LostVolNum	= CONALR_STAT_HAPPEN_NOT;
//		}else
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_LostVolNum	= CONALR_STAT_HAPPEN_NOT;
//	}
//	return ucRet;
//}
//
//unsigned char Alarm_EnsureJudge_FeeRate(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{//���ʸ���ȷ��
//	unsigned char	ucRet=RET_SUCCESS;
//	STHISFEERATE	stHisFeeRate;
//	STHISFEERATE	stHisFeeRate0,stHisFeeRate1;
//	unsigned char	ucMPNo=0;
//	unsigned char	ucMeterNo=0;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	ucMeterNo=RunPara_GetMeterNo_byMPNo(ucMPNo);
//	stHisFeeRate0=Get_History_FeeRate_byMPNo(ucMPNo,0);
//	stHisFeeRate1=Get_History_FeeRate_byMPNo(ucMPNo,1);
//	Alarm_GetHisFeeRate_ForeJudge(ucListNo,CON_DI_TYPE_MPFEERATE_BLOCK,0,&stHisFeeRate);
//	
//	if(	stHisFeeRate0.stFeeRateList.ucFeeRateNum!=CONINVALIDVAL &&
//		stHisFeeRate1.stFeeRateList.ucFeeRateNum!=CONINVALIDVAL &&
//		stHisFeeRate.stFeeRateList.ucFeeRateNum !=CONINVALIDVAL )
//	{
//		if(	Is_FeeRateList_Equel(&(stHisFeeRate0.stFeeRateList),&(stHisFeeRate1.stFeeRateList))==RET_ERROR &&
//			Is_FeeRateList_Equel(&(stHisFeeRate0.stFeeRateList),&(stHisFeeRate.stFeeRateList))==RET_SUCCESS)
//		{
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_FeeRate	= CONALR_STAT_HAPPEN;
//			Alarm_FillEnsureJudgeList(pstAlrForeJudge->usAlrCode,ucMPNo);
//		}else if(Is_FeeRateList_Equel(&(stHisFeeRate0.stFeeRateList),&(stHisFeeRate1.stFeeRateList))==RET_ERROR &&
//			Is_FeeRateList_Equel(&(stHisFeeRate1.stFeeRateList),&(stHisFeeRate.stFeeRateList))==RET_ERROR )
//		{
//			Add_History_FeeRate(ucMeterNo,stHisFeeRate.stFeeRateList);
//			Add_History_FeeRate(ucMeterNo,stHisFeeRate.stFeeRateList);
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_FeeRate	= CONALR_STAT_HAPPEN_NOT;
//		}else
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_FeeRate	= CONALR_STAT_HAPPEN_NOT;
//	}
//	
//	return ucRet;
//}

//unsigned char Alarm_EnsureJudge_MeterStop(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{//���ͣ��ȷ��
//	unsigned char	ucRet=RET_SUCCESS,i=0;
//	STHISVAL		stHisVal00,stHisVal01,stHisVal10,stHisVal11;
//	STHISVAL		stHisA,stHisB,stHisC;
//	unsigned char	ucMPNo=0;
//	unsigned long	ulAveI=0,ulFixVal=0;
//	unsigned char ucCopyInter;
//	
//	ucCopyInter=RunPara_GetMeterCopyInter();		//������
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	
//	for(i=0;i<pstAlrForeJudge->ucAlrJudgeTimes;i++)
//	{
//		Alarm_GetHisVal_ForeJudge(ucListNo,CON_DI_TYPE_MPCURA,i,&stHisA);
//		Alarm_GetHisVal_ForeJudge(ucListNo,CON_DI_TYPE_MPCURB,i,&stHisB);
//		Alarm_GetHisVal_ForeJudge(ucListNo,CON_DI_TYPE_MPCURC,i,&stHisC);
//		
//		if(stHisA.lVal!=CONINVALIDVAL && stHisB.lVal!=CONINVALIDVAL && stHisC.lVal!=CONINVALIDVAL)
//		{
//			ulAveI	 = Get_MeterAveCurrent(ucMPNo,stHisA,stHisB,stHisC);
//			ulFixVal = Get_MeterStop_FixVal(ucMPNo);
//			ulFixVal = ulFixVal*2/ucCopyInter;
//			if(ulFixVal<100)
//				ulFixVal = 100;				//�������1A
//			if(ulAveI <= ulFixVal)
//				break;
//		}else
//			break;
//	}
//	
//	if(i >=pstAlrForeJudge->ucAlrJudgeTimes)
//	{//���б��ͣ��
//		Alarm_GetHisVal_ForeJudge(ucListNo,CON_DI_TYPE_MPDIRPOWTOTAL,0,&stHisVal00);
//		Alarm_GetHisVal_ForeJudge(ucListNo,CON_DI_TYPE_MPUNDIRPOWTOTAL,0,&stHisVal01);
////		Alarm_GetHisVal_ForeJudge(ucListNo,CON_DI_TYPE_MPDIRPOWTOTAL,4,&stHisVal10);
////		Alarm_GetHisVal_ForeJudge(ucListNo,CON_DI_TYPE_MPUNDIRPOWTOTAL,4,&stHisVal11);
//		Alarm_GetHisVal_ForeJudge(ucListNo,CON_DI_TYPE_MPDIRPOWTOTAL,1,&stHisVal10);
//		Alarm_GetHisVal_ForeJudge(ucListNo,CON_DI_TYPE_MPUNDIRPOWTOTAL,1,&stHisVal11);
//		
//		if(	stHisVal00.lVal!=CONINVALIDVAL &&stHisVal01.lVal!=CONINVALIDVAL &&
//			stHisVal10.lVal!=CONINVALIDVAL &&stHisVal11.lVal!=CONINVALIDVAL)
//		{
//			if( stHisVal00.lVal==stHisVal10.lVal && stHisVal01.lVal == stHisVal11.lVal )
//			{
//				stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterStop	= CONALR_STAT_HAPPEN;
//				Alarm_FillEnsureJudgeList(pstAlrForeJudge->usAlrCode,ucMPNo);
//			}else
//				stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterStop	= CONALR_STAT_HAPPEN_NOT;
//			
//		}
//	}
//	return ucRet;
//}
//
//unsigned char Alarm_EnsureJudge_MeterBattLow(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{//��Ƶ�ص�ѹ��
//	unsigned char	ucRet=RET_SUCCESS;
//	STHISSTATE		stHisState;
//	unsigned char	ucMPNo=0;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	Alarm_GetHisState_ForeJudge(ucListNo,CON_DI_TYPE_MPMETERSTATFLAG,0,&stHisState);
//	
//	if((stHisState.ucStateFlag&0x04) == 0x04 && 
//		stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterBattLow == CONALR_STAT_HAPPEN_NOT)
//	{
//		stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterBattLow	= CONALR_STAT_HAPPEN;
//		Alarm_FillEnsureJudgeList(pstAlrForeJudge->usAlrCode,ucMPNo);
//	}else
//		stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterBattLow	= CONALR_STAT_HAPPEN_NOT;
//	
//	return ucRet;
//}

//unsigned char Alarm_EnsureJudge_MeterTimeErr(STALRFOREJUDGE *pstAlrForeJudge,unsigned char ucListNo)
//{//���ʱ���쳣
//	unsigned char	ucRet=RET_SUCCESS;
//	STHISTIME		stHisDate,stHisTime;
//	STDATETIME		stCurrTime,stMeterTime;
//	unsigned long	ulTimeDiff=0;
//	unsigned char	ucMPNo=0;
//	unsigned char   ucMeterTimeDiff=0;
//	
//	ucMPNo = pstAlrForeJudge->ucMPNo;
//	Alarm_GetHisTime_ForeJudge(ucListNo,CON_DI_TYPE_MPDATEANDWEEK,0,&stHisDate);
//	Alarm_GetHisTime_ForeJudge(ucListNo,CON_DI_TYPE_MPTIME,0,&stHisTime);
//	
//	stMeterTime = stHisDate.stHisTime;
//	stMeterTime.ucHour	= stHisTime.stHisTime.ucHour;
//	stMeterTime.ucMinute= stHisTime.stHisTime.ucMinute;
//	stMeterTime.ucSecond= stHisTime.stHisTime.ucSecond;
//	
//	if(Is_DateTime_Availble(&stMeterTime)==RET_SUCCESS)
//	{
//		ucMeterTimeDiff=RunPara_GetTerMisc_MeterTimeDiff();
//		stCurrTime = GetCurrentTime();
//		ulTimeDiff = SubDateTime(&stCurrTime,&stMeterTime);
//		if(	ulTimeDiff > ucMeterTimeDiff && stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterTimeErr == CONALR_STAT_HAPPEN_NOT)
//		{
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterTimeErr	= CONALR_STAT_HAPPEN;
//			Alarm_FillEnsureJudgeList(pstAlrForeJudge->usAlrCode,ucMPNo);
//		}
//		else if( ulTimeDiff < ucMeterTimeDiff && stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterTimeErr == CONALR_STAT_HAPPEN)
//		{
//			stAlarmStatus[ucMPNo].ucAlrStat_Sys_MeterTimeErr	= CONALR_STAT_HAPPEN_NOT;
//			Alarm_FillEnsureJudgeList(pstAlrForeJudge->usAlrCode,ucMPNo);
//		}
//	}
//	
//	return ucRet;
//}




unsigned char Alarm_EnsureJudge_AllDataInit()
{//�������ݳ�ʼ��
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo=RunPara_GetTer_MPNo();
	Alarm_FillEnsureJudgeList(CONALRCODE_ALLDATAINIT,ucMPNo);
	return ucRet;
}

unsigned char Alarm_EnsureJudge_SoftwareChange()
{//�������ݳ�ʼ��
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo=RunPara_GetTer_MPNo();
	Alarm_FillEnsureJudgeList(CONALRCODE_SOFTWARECHANGE,ucMPNo);
	return ucRet;
}

//unsigned char Alarm_EnsureJudge_PasswordErr(unsigned char *pucPass,unsigned char ucMsa)
//{//�������
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucListNo;
//	unsigned char ucMPNo=RunPara_GetTer_MPNo();
//
//	if(Get_Shield_ACD_byAlrCode(CONALRCODE_COMM_PASSWORDERR,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD)
//		return ucRet;
//
//	ucListNo=Alarm_FillEnsureJudgeList(CONALRCODE_COMM_PASSWORDERR,ucMPNo);
//	if(ucListNo!=0xFF)
//	{
//		stAlrEnsureJudgeList[ucListNo].ucAlrDataItemNum = 2;
//	//	stAlrEnsureJudgeList[ucListNo].stAlrDataItem[0].ucDataContent[0]= ucPass1;
//	//	stAlrEnsureJudgeList[ucListNo].stAlrDataItem[0].ucDataContent[1]= ucPass2;
//		memcpy(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[0].ucDataContent,pucPass,16);
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[0].usDataLen = 16; 
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[1].ucDataContent[0]= (ucMsa>>1); 
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[1].usDataLen = 1; 
//	}
//	return ucRet;
//}

unsigned char Alarm_EnsureJudge_OverFluxLimit(unsigned long ulCurrFlux,unsigned long ulFluxLimit)
{//������
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucListNo;
	unsigned char ucMPNo=RunPara_GetTer_MPNo();

	if(Get_Shield_ACD_byAlrCode(CONALRCODE_FLUXLIMIT,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD)
		return ucRet;

	ucListNo=Alarm_FillEnsureJudgeList(CONALRCODE_FLUXLIMIT,ucMPNo);
	if(ucListNo!=0xFF)
	{
		stAlrEnsureJudgeList[ucListNo].ucAlrDataItemNum = 2;
		memcpy(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[0].ucDataContent,&ulCurrFlux,4);
		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[0].usDataLen = 4; 
		memcpy(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[1].ucDataContent,&ulFluxLimit,4);
		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[1].usDataLen = 4; 
	}
	return ucRet;
}
unsigned char Alarm_EnsureJudge_ParaModify_IDOnly(unsigned char *pucTmp,unsigned char ucMsa)
{//�������
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucListNo;
	unsigned char ucMPNo=RunPara_GetTer_MPNo();

	if(Get_Shield_ACD_byAlrCode(CONALRCODE_PARA_MODIFY,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD)
		return ucRet;

	ucListNo=Alarm_FillEnsureJudgeList(CONALRCODE_PARA_MODIFY,ucMPNo);
	if(ucListNo!=0xFF)
	{
		stAlrEnsureJudgeList[ucListNo].ucAlrDataItemNum = 2;
		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[0].ucDataContent[0]= (ucMsa>>1); 
		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[0].usDataLen = 1; 
		memcpy(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[1].ucDataContent,pucTmp,4);
		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[1].usDataLen = 4; 
	}
	return ucRet;
}

//unsigned char Alarm_EnsureJudge_RemoteCtrl(STSWITCHALARMINFO *pstSwitchAlarmInfo)
//{//ң�ظ澯
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucListNo;
//	unsigned char ucMPNo=RunPara_GetTer_MPNo();
//	ucListNo=Alarm_FillEnsureJudgeList(CONALRCODE_REMOTECTRL,ucMPNo);
//	if(ucListNo!=0xFF)
//	{
//		stAlrEnsureJudgeList[ucListNo].ucAlrDataItemNum = 3;
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[0].ucDataContent[0]= pstSwitchAlarmInfo->ucCtrlTurn;
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[0].usDataLen = 1; 
//		memcpy(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[1].ucDataContent,&(pstSwitchAlarmInfo->dCurrVal),8);
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[1].usDataLen = 8; 
//		memcpy(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[2].ucDataContent,&(pstSwitchAlarmInfo->dLastVal),8);
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[2].usDataLen = 8; 
//	}
//	return ucRet;
//}
//unsigned char Alarm_EnsureJudge_PowerCtrl(STSWITCHALARMINFO *pstSwitchAlarmInfo)
//{//���ظ澯
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucListNo;
//	unsigned char ucMPNo=RunPara_GetTer_MPNo();
//	ucListNo=Alarm_FillEnsureJudgeList(CONALRCODE_POWERCTRL,ucMPNo);
//	if(ucListNo!=0xFF)
//	{
//		stAlrEnsureJudgeList[ucListNo].ucAlrDataItemNum = 5;
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[0].ucDataContent[0]= pstSwitchAlarmInfo->ucCtrlTurn;
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[0].usDataLen = 1; 
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[1].ucDataContent[0]= pstSwitchAlarmInfo->ucCtrlCurveNo;
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[1].usDataLen = 1; 
//		memcpy(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[2].ucDataContent,&(pstSwitchAlarmInfo->dCurrVal),8);
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[2].usDataLen = 8; 
//		memcpy(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[3].ucDataContent,&(pstSwitchAlarmInfo->dLastVal),8);
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[3].usDataLen = 8; 
//		memcpy(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[4].ucDataContent,&(pstSwitchAlarmInfo->dLoadFixVal),8);
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[4].usDataLen = 8; 
//	}
//	return ucRet;
//}
//
//unsigned char Alarm_EnsureJudge_EnergyCtrl(STSWITCHALARMINFO *pstSwitchAlarmInfo)
//{//��ظ澯
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucListNo;
//	unsigned char ucMPNo=RunPara_GetTer_MPNo();
//	ucListNo=Alarm_FillEnsureJudgeList(CONALRCODE_ENERCTRL,ucMPNo);
//	if(ucListNo!=0xFF)
//	{
//		stAlrEnsureJudgeList[ucListNo].ucAlrDataItemNum = 4;
//		if(RunPara_GetKeepPower()==CON_AVALIBLE_FLAG)
//			stAlrEnsureJudgeList[ucListNo].stAlrDataItem[0].ucDataContent[0] = 7;		//��������£���8�ֱ�ʾ�澯
//		else
//			stAlrEnsureJudgeList[ucListNo].stAlrDataItem[0].ucDataContent[0] = pstSwitchAlarmInfo->ucCtrlTurn;
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[0].usDataLen = 1; 
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[1].ucDataContent[0]= pstSwitchAlarmInfo->ucCtrlCurveNo;
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[1].usDataLen = 1; 
//		memcpy(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[2].ucDataContent,&(pstSwitchAlarmInfo->dCurrVal),8);
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[2].usDataLen = 8; 
//		memcpy(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[3].ucDataContent,&(pstSwitchAlarmInfo->dLoadFixVal),8);
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[3].usDataLen = 8; 
//	}
//	return ucRet;
//}
//
//unsigned char Alarm_EnsureJudge_BuyPowEnerRec(unsigned char *pstBuyEnergyRecPtr,double dLeftVal_befor,double dLeftVal_after)
//{//�����������
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucListNo;
//	unsigned char ucMPNo=RunPara_GetTer_MPNo();
//	STBUYENERGYREC *pstBuyEnergyRec;
//	unsigned long ulDT3;
//	if(Get_Shield_ACD_byAlrCode(CONALRCODE_BUYPOWPARA,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD)
//		return ucRet;
//
//	pstBuyEnergyRec=(STBUYENERGYREC *)pstBuyEnergyRecPtr;
//	ucListNo=Alarm_FillEnsureJudgeList(CONALRCODE_BUYPOWPARA,ucMPNo);
//	if(ucListNo!=0xFF)
//	{
//		stAlrEnsureJudgeList[ucListNo].ucAlrDataItemNum = 8;
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[0].ucDataContent[0]= 1;	//�ܼ����
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[0].usDataLen = 1; 
//		memcpy(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[1].ucDataContent,&(pstBuyEnergyRec->ulBuyBillNo),4);
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[1].usDataLen = 4; 
//		if(pstBuyEnergyRec->ucFlashFlag==CON_ENERGYFLASH_NEW)
//			stAlrEnsureJudgeList[ucListNo].stAlrDataItem[2].ucDataContent[0]=0xAA;
//		else
//			stAlrEnsureJudgeList[ucListNo].stAlrDataItem[2].ucDataContent[0]=0x55;
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[2].usDataLen = 1; 
//
//		ulDT3 = Float_To_DT3(pstBuyEnergyRec->stBuyFixVal.dFixVal,(unsigned char)(pstBuyEnergyRec->stBuyFixVal.usDT2));
//		memcpy(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[3].ucDataContent,&(ulDT3),4);
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[3].usDataLen = 4; 
//		ulDT3 = Float_To_DT3(pstBuyEnergyRec->stAlrFixVal.dFixVal,(unsigned char)(pstBuyEnergyRec->stAlrFixVal.usDT2));
//		memcpy(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[4].ucDataContent,&(ulDT3),4);
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[4].usDataLen = 4; 
//		ulDT3 = Float_To_DT3(pstBuyEnergyRec->stJumpFixVal.dFixVal,(unsigned char)(pstBuyEnergyRec->stJumpFixVal.usDT2));
//		memcpy(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[5].ucDataContent,&(ulDT3),4);
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[5].usDataLen = 4; 
//
//		memcpy(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[6].ucDataContent,&(dLeftVal_befor),8);
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[6].usDataLen = 8; 
//		memcpy(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[7].ucDataContent,&(dLeftVal_after),8);
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[7].usDataLen = 8; 
//	}
//	return ucRet;
//}
//unsigned char Alarm_EnsureJudge_EnergyAlr(unsigned char ucEnerType,unsigned char ucCtrlTurn,double dEnerVal,double dFixVal)
//{//��ظ澯
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucListNo;
//	unsigned char ucMPNo=RunPara_GetTer_MPNo();
//
//	if(Get_Shield_ACD_byAlrCode(CONALRCODE_ENERALR,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD)
//		return ucRet;
//
//	ucListNo=Alarm_FillEnsureJudgeList(CONALRCODE_ENERALR,ucMPNo);
//	if(ucListNo!=0xFF)
//	{
//		stAlrEnsureJudgeList[ucListNo].ucAlrDataItemNum = 4;
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[0].ucDataContent[0]= ucCtrlTurn;
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[0].usDataLen = 1; 
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[1].ucDataContent[0]= ucEnerType;
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[1].usDataLen = 1; 
//		memcpy(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[2].ucDataContent,&(dEnerVal),8);
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[2].usDataLen = 8; 
//		memcpy(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[3].ucDataContent,&(dFixVal),8);
//		stAlrEnsureJudgeList[ucListNo].stAlrDataItem[3].usDataLen = 8; 
//	}
//	return ucRet;
//}

unsigned char Alarm_EnsureJudge_TerErr_RAM()
{//�ն������ڴ����
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo=RunPara_GetTer_MPNo();
	Alarm_FillEnsureJudgeList(CONALRCODE_TERERR_RAM,ucMPNo);
	return ucRet;
}

unsigned char Alarm_EnsureJudge_TerErr_Clock()
{//�ն�ʱ���쳣
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo=RunPara_GetTer_MPNo();
	Alarm_FillEnsureJudgeList(CONALRCODE_TERERR_CLOCK,ucMPNo);
	return ucRet;
}

unsigned char Alarm_EnsureJudge_TerErr_MainComm()
{//����ͨ�Ź���
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo=RunPara_GetTer_MPNo();

	if(Get_Shield_ACD_byAlrCode(CONALRCODE_TERERR_MAINCOMM,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD)
		return ucRet;

	Alarm_FillEnsureJudgeList(CONALRCODE_TERERR_MAINCOMM,ucMPNo);
	return ucRet;
}

//unsigned char Alarm_EnsureJudge_485Err(void)
//{//485����
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucMPNo=RunPara_GetTer_MPNo();
//
//	if(Get_Shield_ACD_byAlrCode(CONALRCODE_TERERR_485ERROR,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD)
//		return ucRet;
//
//	Alarm_FillEnsureJudgeList(CONALRCODE_TERERR_485ERROR,ucMPNo);
//	return ucRet;
//}

unsigned char Alarm_EnsureJudge_MeterCopy(unsigned char ucMeterCopyErrFlag,unsigned char ucMPNo)
{//����ʧ��
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucListNo;
	unsigned char ucMeterNo;
	STHISVAL stHisVal;
	
	if(!PORT0_IS_HIGH(P0_PD_CHK))//�����û��Ҫ�ж� ��Ϊ��ص�zigbee��Դ
	{
	    return ucRet;    
	}
	if(ucMeterCopyErrFlag != METER_OVERTIME_FLAG )
	{
		if(ucMeterCopyErrTimes[ucMPNo]>CON_COPYERR_TIMES)
		{//����ʧ�ָܻ�
			ucListNo=Alarm_FillEnsureJudgeList(CONALRCODE_METERCOPY,ucMPNo);
			if(ucListNo!=0xFF)
			{
				ucMeterNo=RunPara_GetMeterNo_byMPNo(ucMPNo);
				if(ucMeterNo>=CON_METER_NUM)
					ucMeterNo=0;
				stAlrEnsureJudgeList[ucListNo].ucAlrDataItemNum = 4;
				stAlrEnsureJudgeList[ucListNo].stAlrDataItem[0].ucDataContent[0]= 0; 
				stAlrEnsureJudgeList[ucListNo].stAlrDataItem[0].usDataLen = 1; 

				stHisVal=Get_History_DirPower_byMPNo(ucMPNo,CONHISDATA_LASTVALID_POS,0);		//ȡ�����Чһ��
				SetMainComm_Buf_DateTime_YMDHM(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[1].ucDataContent,0,stHisVal.stDateTime);
				stAlrEnsureJudgeList[ucListNo].stAlrDataItem[1].usDataLen = 5; 
				SetMainComm_Buf_lVal_5B_DT14(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[2].ucDataContent,0,stHisVal.lVal);
				stAlrEnsureJudgeList[ucListNo].stAlrDataItem[2].usDataLen = 5; 

				stHisVal=Get_History_DirVar_byMPNo(ucMPNo,CONHISDATA_LASTVALID_POS,0);		//ȡ�����Чһ��
				SetMainComm_Buf_lVal_4B(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[3].ucDataContent,0,stHisVal.lVal);
				stAlrEnsureJudgeList[ucListNo].stAlrDataItem[3].usDataLen = 4; 
//				ucMeterCopyErrAlrFlags[ucMPNo]=CON_STATUS_NO;						
			}
		}
		ucMeterCopyErrTimes[ucMPNo]=0;
	}
	else
	{
		if(ucMeterCopyErrTimes[ucMPNo]<CON_COPYERR_TIMES)
			ucMeterCopyErrTimes[ucMPNo]++;
		else if(ucMeterCopyErrTimes[ucMPNo]==CON_COPYERR_TIMES)
		{//����ʧ��
			ucListNo=Alarm_FillEnsureJudgeList(CONALRCODE_METERCOPY,ucMPNo);
			if(ucListNo!=0xFF)
			{
				ucMeterNo=RunPara_GetMeterNo_byMPNo(ucMPNo);
				if(ucMeterNo>=CON_METER_NUM)
					ucMeterNo=0;
				stAlrEnsureJudgeList[ucListNo].ucAlrDataItemNum = 4;
				stAlrEnsureJudgeList[ucListNo].stAlrDataItem[0].ucDataContent[0]= CON_COPYERR_TIMES; 
				stAlrEnsureJudgeList[ucListNo].stAlrDataItem[0].usDataLen = 1; 

				stHisVal=Get_History_DirPower_byMPNo(ucMPNo,CONHISDATA_LASTVALID_POS,0);		//ȡ�����Чһ��
				SetMainComm_Buf_DateTime_YMDHM(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[1].ucDataContent,0,stHisVal.stDateTime);
				stAlrEnsureJudgeList[ucListNo].stAlrDataItem[1].usDataLen = 5; 
				SetMainComm_Buf_lVal_5B_DT14(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[2].ucDataContent,0,stHisVal.lVal);
				stAlrEnsureJudgeList[ucListNo].stAlrDataItem[2].usDataLen = 5; 

				stHisVal=Get_History_DirVar_byMPNo(ucMPNo,CONHISDATA_LASTVALID_POS,0);		//ȡ�����Чһ��
				SetMainComm_Buf_lVal_4B(stAlrEnsureJudgeList[ucListNo].stAlrDataItem[3].ucDataContent,0,stHisVal.lVal);
				stAlrEnsureJudgeList[ucListNo].stAlrDataItem[3].usDataLen = 4; 
				
				//ͬһ�˿��ϣ�������һ������ʧ�ܣ���澯485err
//azh ����˿��ϼ��������������������ж�����˿ڳ�������
//				if(Alarm_Check_485err(ucMeterNo)==CON_STATUS_YES)
//					Alarm_EnsureJudge_485Err();
//				ucMeterCopyErrAlrFlags[ucMPNo]=CON_STATUS_YES;
			}
			ucMeterCopyErrTimes[ucMPNo]++;		//����ʧ�ܺ󣬱��ֳ���ʧ�ܣ����澯ֻ��һ��
		}
	}
	return ucRet;
}

//unsigned char Alarm_Check_485err(unsigned char ucCurrMeterNo)
//{
//	unsigned char ucRet=CON_STATUS_YES;
//	unsigned char i;
//	unsigned char ucMPNo,ucPort;
//	ucPort=RunPara_GetMeterPara_PortNo(ucCurrMeterNo);
//	for(i=0;i<CON_METER_NUM;i++)
//	{
//		if(i!=ucCurrMeterNo)
//		{
//			ucMPNo=RunPara_GetMeter_MPNo(i);
//			if(ucMPNo<CON_MAX_MPNUM)		//����Ч
//			{
//				if(RunPara_GetMeterPara_PortNo(i)==ucPort)
//				{
//					if(ucMeterCopyErrAlrFlags[ucMPNo]==CON_STATUS_NO)
//					{//�ҵ�����һ�������������ľ�����
//						ucRet=CON_STATUS_NO;
//						break;
//					}
//				}
//			}
//		}
//	}
//	return ucRet;
//}


unsigned char Alarm_EnsureJudge_TerErr_Display()
{//��ʾ�����
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo=RunPara_GetTer_MPNo();
	Alarm_FillEnsureJudgeList(CONALRCODE_TERERR_DISPLAY,ucMPNo);
	return ucRet;
}

//unsigned char Alarm_EnsureJudge_ValOverUp(STALRFOREJUDGE *pstAlrForeJudge,unsigned long ulUpLimit,unsigned short usBack,unsigned char ucListNo,unsigned short usDI,unsigned char *pucAlrStat)
//{
//	unsigned char	ucRet=RET_SUCCESS,i=0;
//	STHISVAL		stHisVal;
////	unsigned short	usAlrCode=CONALRCODE_INVALID;
//	unsigned char	ucHappen=CON_STATUS_YES,ucHappenNot=CON_STATUS_YES;
//	
//	for(i=0;i<pstAlrForeJudge->ucAlrJudgeTimes;i++)
//	{
//		Alarm_GetHisVal_ForeJudge(ucListNo,usDI,i,&stHisVal);
//		if(stHisVal.lVal!=CONINVALIDVAL)
//		{
//			if(stHisVal.lVal<(unsigned long)(ulUpLimit*usBack/100))
//				ucHappen	=CON_STATUS_NO;
//			else if(stHisVal.lVal>(unsigned long)(ulUpLimit*CON_UPFLOAT_COEF/10))
//				ucHappenNot	=CON_STATUS_NO;
//			else
//			{
//				ucHappenNot	=CON_STATUS_NO;
//				ucHappen	=CON_STATUS_NO;
//			}
//		}else
//			break;
//	}
//	if(i>=pstAlrForeJudge->ucAlrJudgeTimes)
//	{
//		if(ucHappen==CON_STATUS_YES && ucHappenNot==CON_STATUS_NO && (*pucAlrStat)==CONALR_STAT_HAPPEN_NOT)
//		{
//			(*pucAlrStat)=CONALR_STAT_HAPPEN;
//			Alarm_FillEnsureJudgeList(pstAlrForeJudge->usAlrCode,pstAlrForeJudge->ucMPNo);	//�澯
//		}
//		else if(ucHappenNot==CON_STATUS_YES && ucHappen==CON_STATUS_NO && (*pucAlrStat)==CONALR_STAT_HAPPEN)
//		{
//			(*pucAlrStat)=CONALR_STAT_HAPPEN_NOT;
//			Alarm_FillEnsureJudgeList(pstAlrForeJudge->usAlrCode,pstAlrForeJudge->ucMPNo);	//�ָ��澯
//		}
//	}
//	return ucRet;
//}
//
//unsigned char Alarm_EnsureJudge_ValOverDown(STALRFOREJUDGE *pstAlrForeJudge,unsigned long ulDownLimit,unsigned short usBack,unsigned char ucListNo,unsigned short usDI,unsigned char *pucAlrStat)
//{
//	unsigned char	ucRet=RET_SUCCESS,i=0;
//	STHISVAL		stHisVal;
////	unsigned short	usAlrCode=CONALRCODE_INVALID;
//	unsigned char	ucHappen=CON_STATUS_YES,ucHappenNot=CON_STATUS_YES;
//	
//	for(i=0;i<pstAlrForeJudge->ucAlrJudgeTimes;i++)
//	{
//		Alarm_GetHisVal_ForeJudge(ucListNo,usDI,i,&stHisVal);
//		if(stHisVal.lVal!=CONINVALIDVAL)
//		{
//			if(stHisVal.lVal<(unsigned long)(ulDownLimit*CON_DOWNFLOAT_COEF/10))
//				ucHappenNot	=CON_STATUS_NO;
//			else if(stHisVal.lVal>(unsigned long)(ulDownLimit*usBack/100))
//				ucHappen	=CON_STATUS_NO;
//			else
//			{
//				ucHappenNot	=CON_STATUS_NO;
//				ucHappen	=CON_STATUS_NO;
//			}
//		}else
//			break;
//	}
//	if(i>=pstAlrForeJudge->ucAlrJudgeTimes)
//	{
//		if(ucHappen==CON_STATUS_YES && ucHappenNot==CON_STATUS_NO && (*pucAlrStat)==CONALR_STAT_HAPPEN_NOT)
//		{//�澯
//			(*pucAlrStat)=CONALR_STAT_HAPPEN;
//			Alarm_FillEnsureJudgeList(pstAlrForeJudge->usAlrCode,pstAlrForeJudge->ucMPNo);
//		}
//		else if(ucHappenNot==CON_STATUS_YES && ucHappen==CON_STATUS_NO  && (*pucAlrStat)==CONALR_STAT_HAPPEN)
//		{//�ָ��澯
//			(*pucAlrStat)=CONALR_STAT_HAPPEN_NOT;
//			Alarm_FillEnsureJudgeList(pstAlrForeJudge->usAlrCode,pstAlrForeJudge->ucMPNo);
//		}
//	}
//	return ucRet;
//}
//
////�������ߵ�B������ж�
//unsigned char Alarm_EnsureJudge_VolBBreak(STALRFOREJUDGE *pstAlrForeJudge,unsigned long ulDownLimit,unsigned char ucListNo,unsigned char *pucAlrStat)
//{
//	unsigned char	ucRet=RET_SUCCESS,i=0;
//	STHISVAL		stHisVal1,stHisVal2;
////	unsigned short	usAlrCode=CONALRCODE_INVALID;
//	unsigned char	ucHappen=CON_STATUS_YES,ucHappenNot=CON_STATUS_YES;
//	for(i=0;i<pstAlrForeJudge->ucAlrJudgeTimes;i++)
//	{
//		Alarm_GetHisVal_ForeJudge(ucListNo,CON_DI_TYPE_MPVOLA,i,&stHisVal1);
//		Alarm_GetHisVal_ForeJudge(ucListNo,CON_DI_TYPE_MPVOLC,i,&stHisVal2);
//		if(stHisVal1.lVal != CONINVALIDVAL && stHisVal2.lVal != CONINVALIDVAL)
//		{
//			if( stHisVal1.lVal > (CON_RATEVOL_3P3L/2-10) && stHisVal1.lVal < (CON_RATEVOL_3P3L/2+10) &&
//				stHisVal2.lVal > (CON_RATEVOL_3P3L/2-10) && stHisVal2.lVal < (CON_RATEVOL_3P3L/2+10))
//				ucHappenNot	= CON_STATUS_NO;
//			else
//				ucHappen	= CON_STATUS_NO;
//		}else
//			break;
//	}
//	
//	if(i>=pstAlrForeJudge->ucAlrJudgeTimes)
//	{
//		if(ucHappen==CON_STATUS_YES && ucHappenNot==CON_STATUS_NO && (*pucAlrStat)==CONALR_STAT_HAPPEN_NOT)
//		{//�澯
//			(*pucAlrStat)=CONALR_STAT_HAPPEN;
//			Alarm_FillEnsureJudgeList(pstAlrForeJudge->usAlrCode,pstAlrForeJudge->ucMPNo);
//		}
//		else if(ucHappenNot==CON_STATUS_YES && ucHappen==CON_STATUS_NO  && (*pucAlrStat)==CONALR_STAT_HAPPEN)
//		{//�ָ��澯
//			(*pucAlrStat)=CONALR_STAT_HAPPEN_NOT;
//			Alarm_FillEnsureJudgeList(pstAlrForeJudge->usAlrCode,pstAlrForeJudge->ucMPNo);
//		}
//	}
//	return ucRet;
//}
//
//
////PQ�����ڹ���Խ��
//unsigned char Alarm_EnsureJudge_SQRTOverUp(STALRFOREJUDGE *pstAlrForeJudge,unsigned long ulUpLimit,unsigned short usBack,
//												   unsigned char ucListNo,unsigned short usDIP,unsigned short usDIQ,unsigned char *pucAlrStat)
//{
//	unsigned char	ucRet=RET_SUCCESS,i=0;
//	STHISVAL		stHisP,stHisQ;
//	STHISVAL		stHisVal_Cur;
////	unsigned short	usAlrCode=CONALRCODE_INVALID;
//	unsigned char	ucHappen=CON_STATUS_YES,ucHappenNot=CON_STATUS_YES;
//
//	for(i=0;i<pstAlrForeJudge->ucAlrJudgeTimes;i++)
//	{
//		Alarm_GetHisVal_ForeJudge(ucListNo,usDIP,i,&stHisP);
//		Alarm_GetHisVal_ForeJudge(ucListNo,usDIQ,i,&stHisQ);
//		stHisVal_Cur=stHisP;
//		if(stHisP.lVal!=CONINVALIDVAL && stHisQ.lVal!=CONINVALIDVAL)
//			stHisVal_Cur.lVal = GetSPow(stHisP.lVal,stHisQ.lVal);	//4λС��
//		else
//			ClearHisVal(&stHisVal_Cur);
//
//		if(stHisVal_Cur.lVal!=CONINVALIDVAL)
//		{
//			if(stHisVal_Cur.lVal<(unsigned long)(ulUpLimit*usBack/1000))
//				ucHappen	=CON_STATUS_NO;
//			else if(stHisVal_Cur.lVal>(unsigned long)(ulUpLimit*CON_UPFLOAT_COEF/100))
//				ucHappenNot	=CON_STATUS_NO;
//			else
//			{
//				ucHappenNot	=CON_STATUS_NO;
//				ucHappen	=CON_STATUS_NO;
//			}
//		}else
//			break;
//	}
//	if(i>=pstAlrForeJudge->ucAlrJudgeTimes)
//	{
//		if(ucHappen==CON_STATUS_YES && ucHappenNot==CON_STATUS_NO &&(*pucAlrStat)==CONALR_STAT_HAPPEN_NOT)
//		{//�澯
//			(*pucAlrStat)=CONALR_STAT_HAPPEN;
//			Alarm_FillEnsureJudgeList(pstAlrForeJudge->usAlrCode,pstAlrForeJudge->ucMPNo);
//		}
//		else if(ucHappenNot==CON_STATUS_YES && ucHappen==CON_STATUS_NO && (*pucAlrStat)==CONALR_STAT_HAPPEN)
//		{//�ָ��澯
//			(*pucAlrStat)=CONALR_STAT_HAPPEN_NOT;
//			Alarm_FillEnsureJudgeList(pstAlrForeJudge->usAlrCode,pstAlrForeJudge->ucMPNo);
//		}
//	}
//	return ucRet;
//}
//
//unsigned char Alarm_EnsureJudge_ValUnBalanceOverUp(STALRFOREJUDGE *pstAlrForeJudge,unsigned long ulUpLimit,unsigned short usBack,
//												   unsigned char ucListNo,unsigned short usDIA,unsigned short usDIB,unsigned short usDIC,unsigned char *pucAlrStat)
//{
//	unsigned char	ucRet=RET_SUCCESS,i=0;
//	STHISVAL		stHisA,stHisB,stHisC;
//	STHISVAL		stHisVal_Vol;
////	unsigned short	usAlrCode=CONALRCODE_INVALID;
//	STMPRATEVAL		stMPRateVal;
//	unsigned char	ucHappen=CON_STATUS_YES,ucHappenNot=CON_STATUS_YES;
//	
//	stMPRateVal	= RunPara_GetMPRateVal(pstAlrForeJudge->ucMPNo);
//	for(i=0;i<pstAlrForeJudge->ucAlrJudgeTimes;i++)
//	{
//		Alarm_GetHisVal_ForeJudge(ucListNo,usDIA,i,&stHisA);
//		Alarm_GetHisVal_ForeJudge(ucListNo,usDIB,i,&stHisB);
//		Alarm_GetHisVal_ForeJudge(ucListNo,usDIC,i,&stHisC);
//		stHisVal_Vol = GetUnBalanceVal(stHisA,stHisB,stHisC,stMPRateVal.ucMPLineType);
//		if(stHisVal_Vol.lVal!=CONINVALIDVAL)
//		{
//			if(stHisVal_Vol.lVal<(unsigned long)(ulUpLimit*usBack/1000))
//				ucHappen	=CON_STATUS_NO;
//			else if(stHisVal_Vol.lVal>(unsigned long)(ulUpLimit*CON_UPFLOAT_COEF/100))
//				ucHappenNot	=CON_STATUS_NO;
//			else
//			{
//				ucHappenNot	=CON_STATUS_NO;
//				ucHappen	=CON_STATUS_NO;
//			}
//		}else
//			break;
//	}
//	if(i>=pstAlrForeJudge->ucAlrJudgeTimes)
//	{
//		if(ucHappen==CON_STATUS_YES && ucHappenNot==CON_STATUS_NO && (*pucAlrStat)==CONALR_STAT_HAPPEN_NOT)
//		{//�澯
//			(*pucAlrStat)=CONALR_STAT_HAPPEN;
//			Alarm_FillEnsureJudgeList(pstAlrForeJudge->usAlrCode,pstAlrForeJudge->ucMPNo);
//		}
//		else if(ucHappenNot==CON_STATUS_YES && ucHappen==CON_STATUS_NO && (*pucAlrStat)==CONALR_STAT_HAPPEN)
//		{//�ָ��澯
//			(*pucAlrStat)=CONALR_STAT_HAPPEN_NOT;
//			Alarm_FillEnsureJudgeList(pstAlrForeJudge->usAlrCode,pstAlrForeJudge->ucMPNo);
//		}
//	}
//	return ucRet;
//}

//unsigned long Get_MeterFly_FixVal(unsigned char ucMPNo)
//{//��ȡ��Ʒ��߶�ֵ
//	unsigned long	ulRetVal=0;
//	STTERMISCPARA stTerMiscPara;
//	stTerMiscPara=RunPara_GetTerMiscPara();
//	ulRetVal=stTerMiscPara.ucMeterFly;			//��λ0.1kw ת��Ϊ0.01kw
//	return ulRetVal;
//}
//unsigned long Get_MeterPowDiff_FixVal(unsigned char ucMPNo)
//{//��ȡ��Ʒ��߶�ֵ
//	unsigned long	ulRetVal=0;
//	STTERMISCPARA stTerMiscPara;
//	stTerMiscPara=RunPara_GetTerMiscPara();
//	ulRetVal=stTerMiscPara.ucMeterOverDiff;			//��λ0.1kw ת��Ϊ0.01kw
//	return ulRetVal;
//}
//unsigned long Get_MeterStop_FixVal(unsigned char ucMPNo)
//{//��ȡ���ͣ�߶�ֵ 2����
//	unsigned long	ulRetVal=0;
//	STMPRATEVAL		stMPRateVal;
//	
//	stMPRateVal = RunPara_GetMPRateVal(ucMPNo);
//	if(stMPRateVal.ucMPLineType== CON_METERTYPE_3P4L)
//	{
////		ulRetVal = 50;		//0.5 * 220 * 3 = 330  0.33KW,��һСʱΪ0.33�ȣ�ÿ15����Ϊ0.08�ȣ�ÿ2����0.01
//		ulRetVal = 200;
//	}
//	else if( stMPRateVal.ucMPLineType== CON_METERTYPE_3P3L )
//	{
////		ulRetVal = 100;		//1 * 100 * 1.732 = 173  0.173KW,��һСʱΪ0.173�ȣ�ÿ15����Ϊ0.04�ȣ�ÿ2����0.005
//		ulRetVal = 400;
//	}else
//		ulRetVal = 200;
//	
//	return ulRetVal;
//}

//unsigned long Get_MeterAveCurrent(unsigned char ucMPNo,STHISVAL stHisA,STHISVAL stHisB,STHISVAL stHisC)
//{//��ȡƽ������
//	unsigned long	ulRetVal=0;
//	STMPRATEVAL		stMPRateVal;
//	
//	stMPRateVal = RunPara_GetMPRateVal(ucMPNo);
//	if(stMPRateVal.ucMPLineType == CON_METERTYPE_3P4L)
//	{
//		ulRetVal = stHisA.lVal + stHisB.lVal + stHisC.lVal;
//		ulRetVal = ulRetVal/3;
//	}
//	else if( stMPRateVal.ucMPLineType == CON_METERTYPE_3P3L )
//	{
//		ulRetVal = stHisA.lVal + stHisC.lVal;
//		ulRetVal = ulRetVal/2;
//	}else
//	{
//		ulRetVal = stHisA.lVal + stHisB.lVal + stHisC.lVal;
//		ulRetVal = ulRetVal/3;
//	}
//	
//	return ulRetVal;
//}

//unsigned char Chk_MeterDown(STHISVAL stHisVal0,STHISVAL stHisVal1)
//{
//	unsigned char ucRet=RET_ERROR;
//	if(stHisVal0.lVal != CONINVALIDVAL && stHisVal1.lVal != CONINVALIDVAL)
//	{
//		if(stHisVal0.lVal < stHisVal1.lVal )
//		{
//			if((stHisVal1.lVal - stHisVal0.lVal)<99999000)
//				ucRet = RET_SUCCESS;
//		}
//	}
//	return ucRet;
//}
//-------------------------   ��������  ------------------
//���ݸ澯�����ȡ�澯ȷ�Ϸ�ʽ
unsigned char Alarm_GetForeJudgeInfo(unsigned short usAlrCode,unsigned char ucMPNo,STALRFOREJUDGE *pstAlrForeJudge)
{
	unsigned char ucRet=RET_SUCCESS;
	Clear_AlrForeJudge(pstAlrForeJudge);
	
	pstAlrForeJudge->usAlrCode	=	usAlrCode;
	pstAlrForeJudge->ucMPNo		=	ucMPNo;
	switch(usAlrCode)
	{
//	case CONALRCODE_VAOVERUP:
//	case CONALRCODE_VAOVERDOWN:
//	case CONALRCODE_VABREAK:
//	case CONALRCODE_VALACK:
//		Alarm_GetForeJudgeInfo_VaOver(pstAlrForeJudge);
//		break;
//	case CONALRCODE_VBOVERUP:
//	case CONALRCODE_VBOVERDOWN:
//		Alarm_GetForeJudgeInfo_VbOver(pstAlrForeJudge);
//		break;
//	case CONALRCODE_VBBREAK:
//	case CONALRCODE_VBLACK:
//		Alarm_GetForeJudgeInfo_VbBreak(pstAlrForeJudge);
//		break;
//	case CONALRCODE_VCOVERUP:
//	case CONALRCODE_VCOVERDOWN:
//	case CONALRCODE_VCBREAK:
//	case CONALRCODE_VCLACK:
//		Alarm_GetForeJudgeInfo_VcOver(pstAlrForeJudge);
//		break;
//	case CONALRCODE_IAOVERUP:
//		Alarm_GetForeJudgeInfo_IaOverUp(pstAlrForeJudge);
//		break;
//	case CONALRCODE_IBOVERUP:
//		Alarm_GetForeJudgeInfo_IbOverUp(pstAlrForeJudge);
//		break;
//	case CONALRCODE_ICOVERUP:
//		Alarm_GetForeJudgeInfo_IcOverUp(pstAlrForeJudge);
//		break;
//	case CONALRCODE_IAERROVERUP:
//		Alarm_GetForeJudgeInfo_IaHeavyOverUp(pstAlrForeJudge);
//		break;
//	case CONALRCODE_IBERROVERUP:
//		Alarm_GetForeJudgeInfo_IbHeavyOverUp(pstAlrForeJudge);
//		break;
//	case CONALRCODE_ICERROVERUP:
//		Alarm_GetForeJudgeInfo_IcHeavyOverUp(pstAlrForeJudge);
//		break;
//	case CONALRCODE_SPOWNORUP:
//	case CONALRCODE_SPOWERRUP:
//		Alarm_GetForeJudgeInfo_SPowOverUp(pstAlrForeJudge);
//		break;
//	case CONALRCODE_IUNBLANCE:
//		Alarm_GetForeJudgeInfo_IUnBalanceOverUp(pstAlrForeJudge);
//		break;
//	case CONALRCODE_UUNBLANCE:
//		Alarm_GetForeJudgeInfo_UUnBalanceOverUp(pstAlrForeJudge);
//		break;
//	case CONALRCODE_PROGRAMTIME:
//		Alarm_GetForeJudgeInfo_ProgramTime(pstAlrForeJudge);
//		break;
//	case CONALRCODE_PROGRAMNUM:
//		Alarm_GetForeJudgeInfo_ProgramCount(pstAlrForeJudge);
//		break;
//	case CONALRCODE_PULSECONST:
//		Alarm_GetForeJudgeInfo_PulseConst(pstAlrForeJudge);
//		break;
//	case CONALRCODE_COPYDAYTIME:
//		Alarm_GetForeJudgeInfo_CopyDayTime(pstAlrForeJudge);
//		break;
//	case CONALRCODE_LOSTVOLNUM:
//		Alarm_GetForeJudgeInfo_LostVolNum(pstAlrForeJudge);
//		break;
//	case CONALRCODE_FEERATE:
//		Alarm_GetForeJudgeInfo_FeeRate(pstAlrForeJudge);
//		break;
//	case CONALRCODE_METERSTOP:
//		Alarm_GetForeJudgeInfo_MeterStop(pstAlrForeJudge);
// 		break;
//	case CONALRCODE_METERBATTLOW:
//		Alarm_GetForeJudgeInfo_MeterBattLow(pstAlrForeJudge);
//		break;
//	case CONALRCODE_METERTIMEERR:
//		Alarm_GetForeJudgeInfo_MeterTimeErr(pstAlrForeJudge);
//		break;
	default:
		ucRet=RET_ERROR;
	}
	return ucRet;
}

//���ݸ澯������дȷ��Я������
unsigned char Alarm_GetEnsureJudgeInfo(unsigned short usAlrCode,unsigned char ucMPNo,STALRENSUREJUDGE *pstAlrEnsureJudge)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucTerMPNo;
	Clear_AlrEnsureJudge(pstAlrEnsureJudge);
	
	pstAlrEnsureJudge->usAlrCode	=	usAlrCode;
	pstAlrEnsureJudge->ucMPNo		=	ucMPNo;
	switch(usAlrCode)
	{
//	case CONALRCODE_VAOVERUP:				//��ѹԽ��_Va
//	case CONALRCODE_VAOVERDOWN:				//��ѹԽ��_Va(����)
//	case CONALRCODE_VBOVERUP:				//��ѹԽ��_Vb
//	case CONALRCODE_VBOVERDOWN:				//��ѹԽ��_Vb(����)
//	case CONALRCODE_VCOVERUP:				//��ѹԽ��_Vc
//	case CONALRCODE_VCOVERDOWN:				//��ѹԽ��_Vc(����)
//		ucRet = Alarm_GetEnsureJudgeInfo_VolOver(pstAlrEnsureJudge);
//		break;
//	case CONALRCODE_VABREAK:				//A��ѹ����
//	case CONALRCODE_VBBREAK:				//B��ѹ����
//	case CONALRCODE_VCBREAK:				//C��ѹ����
//	case CONALRCODE_VALACK:					//A��ѹȱ��
//	case CONALRCODE_VBLACK:					//B��ѹȱ��
//	case CONALRCODE_VCLACK:					//C��ѹȱ��
//		ucRet = Alarm_GetEnsureJudgeInfo_VabcBreak(pstAlrEnsureJudge);
//		break;
//	case CONALRCODE_IAOVERUP:				//����Խ��_Ia
//	case CONALRCODE_IBOVERUP:				//����Խ��_Ib
//	case CONALRCODE_ICOVERUP:				//����Խ��_Ic
//		ucRet = Alarm_GetEnsureJudgeInfo_CurOverUp(pstAlrEnsureJudge);
//		break;
//	case CONALRCODE_IAERROVERUP:			//����Խ������_Ia
//	case CONALRCODE_IBERROVERUP:			//����Խ������_Ib
//	case CONALRCODE_ICERROVERUP:			//����Խ������_Ic
//		ucRet = Alarm_GetEnsureJudgeInfo_CurHeavyOverUp(pstAlrEnsureJudge);
//		break;
//	case CONALRCODE_SPOWNORUP:				//���ڹ���Խ��
//	case CONALRCODE_SPOWERRUP:				//���ڹ���Խ������
//		ucRet = Alarm_GetEnsureJudgeInfo_SPowOverUp(pstAlrEnsureJudge);
//		break;
//	case CONALRCODE_IUNBLANCE:				//������ƽ��Խ��
//	case CONALRCODE_UUNBLANCE:				//��ѹ��ƽ��Խ��
//		ucRet = Alarm_GetEnsureJudgeInfo_UnBalanceOverUp(pstAlrEnsureJudge);
//		break;
//	case CONALRCODE_PROGRAMTIME:			//���ʱ�����
//		ucRet = Alarm_GetEnsureJudgeInfo_ProgramTime(pstAlrEnsureJudge);
//		break;
//	case CONALRCODE_PROGRAMNUM:				//��̴����仯
//		ucRet = Alarm_GetEnsureJudgeInfo_ProgramCount(pstAlrEnsureJudge);
//		break;
//	case CONALRCODE_PULSECONST:				//���峣��
//		ucRet = Alarm_GetEnsureJudgeInfo_PulseConst(pstAlrEnsureJudge);
//		break;
//	case CONALRCODE_COPYDAYTIME:			//������
//		ucRet = Alarm_GetEnsureJudgeInfo_CopyDayTime(pstAlrEnsureJudge);
//		break;
//	case CONALRCODE_LOSTVOLNUM:				//�������
//		ucRet = Alarm_GetEnsureJudgeInfo_LostVolNum(pstAlrEnsureJudge);
//		break;
//	case CONALRCODE_FEERATE:				//���ʸ���
//		ucRet = Alarm_GetEnsureJudgeInfo_FeeRate(pstAlrEnsureJudge);
//		break;
//	case 	CONALRCODE_METERFLY:			//��Ʒ���
//		ucRet = Alarm_GetEnsureJudgeInfo_MeterFly(pstAlrEnsureJudge);
//		break;
//	case 	CONALRCODE_METERSTOP:			//���ͣ��
//		ucRet = Alarm_GetEnsureJudgeInfo_MeterStop(pstAlrEnsureJudge);
//		break;
//	case 	CONALRCODE_METERDOWN:			//ʾ���½�
//		ucRet = Alarm_GetEnsureJudgeInfo_MeterDown(pstAlrEnsureJudge);
//		break;
//	case 	CONALRCODE_METERBATTLOW:		//����ص�ѹ��
//		ucRet = Alarm_GetEnsureJudgeInfo_MeterBattLow(pstAlrEnsureJudge);
//		break;
//	case	CONALRCODE_METERTIMEERR:		//���ʱ���쳣
//		ucRet = Alarm_GetEnsureJudgeInfo_MeterTimeErr(pstAlrEnsureJudge);
//		break;
	case	CONALRCODE_ALLDATAINIT:			//���ݳ�ʼ��
		ucRet = Alarm_GetEnsureJudgeInfo_AllDataInit(pstAlrEnsureJudge);
		break;
	case	CONALRCODE_SOFTWARECHANGE:		//�������
		ucRet = Alarm_GetEnsureJudgeInfo_SoftwareChange(pstAlrEnsureJudge);
		break;	
//	case	CONALRCODE_POLREVERSEA:			//������
//	case	CONALRCODE_POLREVERSEB:
//	case	CONALRCODE_POLREVERSEC:
//	case	CONALRCODE_SHORTCIRA1:			//CTһ�β��·
//	case	CONALRCODE_SHORTCIRB1:
//	case	CONALRCODE_SHORTCIRC1:
//	case	CONALRCODE_SHORTCIRA2:			//CT���β��·
//	case	CONALRCODE_SHORTCIRB2:
//	case	CONALRCODE_SHORTCIRC2:
//	case	CONALRCODE_OPENCIRA2:			//CT���β࿪·
//	case	CONALRCODE_OPENCIRB2:
//	case	CONALRCODE_OPENCIRC2:
//		ucRet =Alarm_GetEnsureJudgeInfo_CurrentErr(pstAlrEnsureJudge);
//		break;
//	case	CONALRCODE_VOLCIRREVERSE:		//��ѹ��·������
//		ucRet =Alarm_GetEnsureJudgeInfo_VolCirReverse(pstAlrEnsureJudge);
//		break;
//	case	CONALRCODE_SUMCOMPOVERLIMIT:
//	case	CONALRCODE_SUMCOMPOVERLIMIT+1:
//	case	CONALRCODE_SUMCOMPOVERLIMIT+2:
//	case	CONALRCODE_SUMCOMPOVERLIMIT+3:
//	case	CONALRCODE_SUMCOMPOVERLIMIT+4:
//	case	CONALRCODE_SUMCOMPOVERLIMIT+5:
//	case	CONALRCODE_SUMCOMPOVERLIMIT+6:
//	case	CONALRCODE_SUMCOMPOVERLIMIT+7:
//		ucRet =Alarm_GetEnsureJudgeInfo_SumCompOverLimit(pstAlrEnsureJudge);
//		break;
	case	CONALRCODE_PARA_MODIFY:
//	case	CONALRCODE_SUMINSTANTPOWUP1:
//	case	CONALRCODE_SUMINSTANTPOWUP2:
//	case	CONALRCODE_SUMINSTANTPOWUP3:
//	case	CONALRCODE_SUMINSTANTPOWUP4:
//	case	CONALRCODE_SUMINSTANTPOWUP5:
//	case	CONALRCODE_SUMINSTANTPOWUP6:
//	case	CONALRCODE_SUMINSTANTPOWUP7:
//	case	CONALRCODE_SUMINSTANTPOWUP8:
//	case	CONALRCODE_SUMINSTANTVARUP1:
//	case	CONALRCODE_SUMINSTANTVARUP2:
//	case	CONALRCODE_SUMINSTANTVARUP3:
//	case	CONALRCODE_SUMINSTANTVARUP4:
//	case	CONALRCODE_SUMINSTANTVARUP5:
//	case	CONALRCODE_SUMINSTANTVARUP6:
//	case	CONALRCODE_SUMINSTANTVARUP7:
//	case	CONALRCODE_SUMINSTANTVARUP8:
//	case	CONALRCODE_SUMPOWUP1:
//	case	CONALRCODE_SUMPOWUP2:
//	case	CONALRCODE_SUMPOWUP3:
//	case	CONALRCODE_SUMPOWUP4:
//	case	CONALRCODE_SUMPOWUP5:
//	case	CONALRCODE_SUMPOWUP6:
//	case	CONALRCODE_SUMPOWUP7:
//	case	CONALRCODE_SUMPOWUP8:
//	case	CONALRCODE_SUMVARUP1:
//	case	CONALRCODE_SUMVARUP2:
//	case	CONALRCODE_SUMVARUP3:
//	case	CONALRCODE_SUMVARUP4:
//	case	CONALRCODE_SUMVARUP5:
//	case	CONALRCODE_SUMVARUP6:
//	case	CONALRCODE_SUMVARUP7:
//	case	CONALRCODE_SUMVARUP8:

//	case	CONALRCODE_DOOROPEN:
//	case	CONALRCODE_DOORCLOSE:
	case	CONALRCODE_POWERON:
	case	CONALRCODE_POWEROFF:

	case	CONALRCODE_TERERR_RAM:
	case	CONALRCODE_TERERR_CLOCK:
	case	CONALRCODE_TERERR_MAINCOMM:
	case	CONALRCODE_TERERR_485ERROR:
	case	CONALRCODE_TERERR_DISPLAY:

	case	CONALRCODE_COMM_PASSWORDERR:

//	case	CONALRCODE_SWITCH0OPEN:
//	case	CONALRCODE_SWITCH1OPEN:
//	case	CONALRCODE_SWITCH2OPEN:
//	case	CONALRCODE_SWITCH3OPEN:
//	case	CONALRCODE_SWITCH4OPEN:
//	case	CONALRCODE_SWITCH5OPEN:
//	case	CONALRCODE_SWITCH6OPEN:
//	case	CONALRCODE_SWITCH0CLOSE:
//	case	CONALRCODE_SWITCH1CLOSE:
//	case	CONALRCODE_SWITCH2CLOSE:
//	case	CONALRCODE_SWITCH3CLOSE:
//	case	CONALRCODE_SWITCH4CLOSE:
//	case	CONALRCODE_SWITCH5CLOSE:
//	case	CONALRCODE_SWITCH6CLOSE:

//	case	CONALRCODE_REMOTECTRL:
//	case	CONALRCODE_POWERCTRL:
//	case	CONALRCODE_ENERCTRL:
//	case	CONALRCODE_BUYPOWPARA:
//	case	CONALRCODE_ENERALR:
	case	CONALRCODE_FLUXLIMIT:
		ucTerMPNo=RunPara_GetTer_MPNo();
		ucRet =Alarm_GetEnsureJudgeInfo_NoTake(pstAlrEnsureJudge,ucTerMPNo);
		break;
	case	CONALRCODE_METERCOPY:
	case	CONALRCODE_DEMANDTIMECLR:			//��������
	case	CONALRCODE_DEMANDTIMENUM:			//�����������
		ucRet =Alarm_GetEnsureJudgeInfo_NoTake(pstAlrEnsureJudge,pstAlrEnsureJudge->ucMPNo);
		break;
//	case   CONALRCODE_POWDIFFOVER://ERC28	�����������¼
//		ucRet = Alarm_GetEnsureJudgeInfo_PowDiffOver(pstAlrEnsureJudge);
//		break;
	case	CONALRCODE_METERSTATECHANGE:
		ucRet = Alarm_GetEnsureJudgeInfo_MeterState(pstAlrEnsureJudge);
		break;
	default:
		ucRet=RET_ERROR;
		break;
	}
	return ucRet;
}

//------------------------ ��ȡ�澯Ԥ����Ϣ -------------------------------
////��ȡA���ѹԽ���޵�Ԥ����Ϣ
//unsigned char Alarm_GetForeJudgeInfo_VaOver(STALRFOREJUDGE *pstAlrForeJudge)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	unsigned short usCount;
//	pstAlrForeJudge->ucAlrJudgeTimes	=2;			//2��
//	if(pstAlrForeJudge->usAlrCode==CONALRCODE_VALACK || pstAlrForeJudge->usAlrCode==CONALRCODE_VABREAK)
//	{
//		usCount=RunPara_GetMPLimit_usLostVolTime(pstAlrForeJudge->ucMPNo);
//		if(usCount==0)
//			usCount=1;
//		pstAlrForeJudge->ucAlrJudgeTimes=(unsigned char)usCount;
//	}
//	pstAlrForeJudge->ucAlrJudgeCycle	=1;			//ÿ����1��
//	pstAlrForeJudge->ucAlrDataItemNum	=1;			//��Ҫ����1��
//	for(i=0;i<CONALRFOREREPLYNUM;i++)
//	{
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].usItemLabel=CON_DI_TYPE_MPVOLA;
//	}
//	return ucRet;
//}
//
////��ȡB���ѹԽ���޵�Ԥ����Ϣ
//unsigned char Alarm_GetForeJudgeInfo_VbOver(STALRFOREJUDGE *pstAlrForeJudge)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	pstAlrForeJudge->ucAlrJudgeTimes	=2;
//	pstAlrForeJudge->ucAlrJudgeCycle	=1;
//	pstAlrForeJudge->ucAlrDataItemNum	=1;
//	for(i=0;i<CONALRFOREREPLYNUM;i++)
//	{
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].usItemLabel=CON_DI_TYPE_MPVOLB;
//	}
//	return ucRet;
//}
//
////��ȡC���ѹԽ���޵�Ԥ����Ϣ
//unsigned char Alarm_GetForeJudgeInfo_VcOver(STALRFOREJUDGE *pstAlrForeJudge)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	unsigned short usCount;
//	pstAlrForeJudge->ucAlrJudgeTimes	=2;
//	if(pstAlrForeJudge->usAlrCode==CONALRCODE_VCLACK || pstAlrForeJudge->usAlrCode==CONALRCODE_VCBREAK)
//	{
//		usCount=RunPara_GetMPLimit_usLostVolTime(pstAlrForeJudge->ucMPNo);
//		if(usCount==0)
//			usCount=1;
//		pstAlrForeJudge->ucAlrJudgeTimes=(unsigned char)usCount;
//	}
//	pstAlrForeJudge->ucAlrJudgeCycle	=1;
//	pstAlrForeJudge->ucAlrDataItemNum	=1;
//	for(i=0;i<CONALRFOREREPLYNUM;i++)
//	{
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].usItemLabel=CON_DI_TYPE_MPVOLC;
//	}
//	return ucRet;
//}
//
////��ȡA�����Խ�޵�Ԥ����Ϣ
//unsigned char Alarm_GetForeJudgeInfo_IaOverUp(STALRFOREJUDGE *pstAlrForeJudge)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	pstAlrForeJudge->ucAlrJudgeTimes	=2;
//	pstAlrForeJudge->ucAlrJudgeCycle	=1;			//ÿ����1��
//	pstAlrForeJudge->ucAlrDataItemNum	=1;			//��Ҫ����1��
//	for(i=0;i<CONALRFOREREPLYNUM;i++)
//	{
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].usItemLabel=CON_DI_TYPE_MPCURA;
//	}
//	return ucRet;
//}
//
////��ȡB�����Խ�޵�Ԥ����Ϣ
//unsigned char Alarm_GetForeJudgeInfo_IbOverUp(STALRFOREJUDGE *pstAlrForeJudge)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	pstAlrForeJudge->ucAlrJudgeTimes	=2;
//	pstAlrForeJudge->ucAlrJudgeCycle	=1;
//	pstAlrForeJudge->ucAlrDataItemNum	=1;
//	for(i=0;i<CONALRFOREREPLYNUM;i++)
//	{
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].usItemLabel=CON_DI_TYPE_MPCURB;
//	}
//	return ucRet;
//}
//
////��ȡC�����Խ�޵�Ԥ����Ϣ
//unsigned char Alarm_GetForeJudgeInfo_IcOverUp(STALRFOREJUDGE *pstAlrForeJudge)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	pstAlrForeJudge->ucAlrJudgeTimes	=2;
//	pstAlrForeJudge->ucAlrJudgeCycle	=1;
//	pstAlrForeJudge->ucAlrDataItemNum	=1;
//	for(i=0;i<CONALRFOREREPLYNUM;i++)
//	{
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].usItemLabel=CON_DI_TYPE_MPCURC;
//	}
//	return ucRet;
//}
//
////��ȡA�����Խ�����޵�Ԥ����Ϣ
//unsigned char Alarm_GetForeJudgeInfo_IaHeavyOverUp(STALRFOREJUDGE *pstAlrForeJudge)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	pstAlrForeJudge->ucAlrJudgeTimes	=2;
//	pstAlrForeJudge->ucAlrJudgeCycle	=1;
//	pstAlrForeJudge->ucAlrDataItemNum	=1;
//	for(i=0;i<CONALRFOREREPLYNUM;i++)
//	{
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].usItemLabel=CON_DI_TYPE_MPCURA;
//	}
//	return ucRet;
//}
//
////��ȡB�����Խ�����޵�Ԥ����Ϣ
//unsigned char Alarm_GetForeJudgeInfo_IbHeavyOverUp(STALRFOREJUDGE *pstAlrForeJudge)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	pstAlrForeJudge->ucAlrJudgeTimes	=2;
//	pstAlrForeJudge->ucAlrJudgeCycle	=1;
//	pstAlrForeJudge->ucAlrDataItemNum	=1;
//	for(i=0;i<CONALRFOREREPLYNUM;i++)
//	{
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].usItemLabel=CON_DI_TYPE_MPCURB;
//	}
//	return ucRet;
//}
//
////��ȡC�����Խ�����޵�Ԥ����Ϣ
//unsigned char Alarm_GetForeJudgeInfo_IcHeavyOverUp(STALRFOREJUDGE *pstAlrForeJudge)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	pstAlrForeJudge->ucAlrJudgeTimes	=2;
//	pstAlrForeJudge->ucAlrJudgeCycle	=1;
//	pstAlrForeJudge->ucAlrDataItemNum	=1;
//	for(i=0;i<CONALRFOREREPLYNUM;i++)
//	{
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].usItemLabel=CON_DI_TYPE_MPCURC;
//	}
//	return ucRet;
//}
//
////��ȡB���ѹ�����Ԥ����Ϣ
//unsigned char Alarm_GetForeJudgeInfo_VbBreak(STALRFOREJUDGE *pstAlrForeJudge)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	STMPRATEVAL		stMPRateVal;
//	unsigned short usCount;
////	pstAlrForeJudge->ucAlrJudgeTimes	=2;
////	pstAlrForeJudge->ucAlrJudgeTimes	=5;
//
//	usCount=RunPara_GetMPLimit_usLostVolTime(pstAlrForeJudge->ucMPNo);
//	if(usCount==0)
//		usCount=1;
//	pstAlrForeJudge->ucAlrJudgeTimes=(unsigned char)usCount;
//	
//	pstAlrForeJudge->ucAlrJudgeCycle	=1;
//
//	stMPRateVal = RunPara_GetMPRateVal(pstAlrForeJudge->ucMPNo);
//	if(stMPRateVal.ucMPLineType == CON_METERTYPE_3P4L)
//	{
//		pstAlrForeJudge->ucAlrDataItemNum	=1;
//		for(i=0;i<CONALRFOREREPLYNUM;i++)
//		{
//			pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].ucMPNo = pstAlrForeJudge->ucMPNo;
//			pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].usItemLabel=CON_DI_TYPE_MPVOLB;
//		}
//	}else
//	{
//		pstAlrForeJudge->ucAlrDataItemNum	=2;
//		for(i=0;i<CONALRFOREREPLYNUM;i++)
//		{
//			pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].ucMPNo = pstAlrForeJudge->ucMPNo;
//			pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].usItemLabel=CON_DI_TYPE_MPVOLA;
//			pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[1].ucMPNo = pstAlrForeJudge->ucMPNo;
//			pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[1].usItemLabel=CON_DI_TYPE_MPVOLC;
//		}
//	}
//	return ucRet;
//}
//
////���ڹ���Խ��Ԥ����Ϣ
//unsigned char Alarm_GetForeJudgeInfo_SPowOverUp(STALRFOREJUDGE *pstAlrForeJudge)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	pstAlrForeJudge->ucAlrJudgeTimes	=2;
//	pstAlrForeJudge->ucAlrJudgeCycle	=1;
//	pstAlrForeJudge->ucAlrDataItemNum	=2;
//	for(i=0;i<CONALRFOREREPLYNUM;i++)
//	{
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].usItemLabel=CON_DI_TYPE_MPINSTANTPOW;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[1].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[1].usItemLabel=CON_DI_TYPE_MPINSTANTVAR;
//	}
//	return ucRet;
//}
//
////��ȡ���������ƽ���Խ�޵�Ԥ����Ϣ
//unsigned char Alarm_GetForeJudgeInfo_IUnBalanceOverUp(STALRFOREJUDGE *pstAlrForeJudge)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	pstAlrForeJudge->ucAlrJudgeTimes	=2;
//	pstAlrForeJudge->ucAlrJudgeCycle	=1;
//	pstAlrForeJudge->ucAlrDataItemNum	=3;
//	for(i=0;i<CONALRFOREREPLYNUM;i++)
//	{
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].usItemLabel=CON_DI_TYPE_MPCURA;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[1].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[1].usItemLabel=CON_DI_TYPE_MPCURB;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[2].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[2].usItemLabel=CON_DI_TYPE_MPCURC;
//	}
//	return ucRet;
//}
////��ȡ�����ѹ��ƽ���Խ�޵�Ԥ����Ϣ
//unsigned char Alarm_GetForeJudgeInfo_UUnBalanceOverUp(STALRFOREJUDGE *pstAlrForeJudge)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	pstAlrForeJudge->ucAlrJudgeTimes	=2;
//	pstAlrForeJudge->ucAlrJudgeCycle	=1;
//	pstAlrForeJudge->ucAlrDataItemNum	=3;
//	for(i=0;i<CONALRFOREREPLYNUM;i++)
//	{
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].usItemLabel=CON_DI_TYPE_MPVOLA;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[1].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[1].usItemLabel=CON_DI_TYPE_MPVOLB;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[2].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[2].usItemLabel=CON_DI_TYPE_MPVOLC;
//	}
//	return ucRet;
//}

////��ȡ���ʱ����ĵ�Ԥ����Ϣ
//unsigned char Alarm_GetForeJudgeInfo_ProgramTime(STALRFOREJUDGE *pstAlrForeJudge)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	pstAlrForeJudge->ucAlrJudgeTimes	=1;
//	pstAlrForeJudge->ucAlrJudgeCycle	=1;
//	pstAlrForeJudge->ucAlrDataItemNum	=1;
//	for(i=0;i<CONALRFOREREPLYNUM;i++)
//	{
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].usItemLabel=CON_DI_TYPE_MPLATELYPROGTIME;
//	}
//	
//	return ucRet;
//}
//
////��ȡ��̴��������Ԥ����Ϣ
//unsigned char Alarm_GetForeJudgeInfo_ProgramCount(STALRFOREJUDGE *pstAlrForeJudge)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	pstAlrForeJudge->ucAlrJudgeTimes	=1;
//	pstAlrForeJudge->ucAlrJudgeCycle	=1;
//	pstAlrForeJudge->ucAlrDataItemNum	=1;
//	for(i=0;i<CONALRFOREREPLYNUM;i++)
//	{
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].usItemLabel=CON_DI_TYPE_MPPROGTIMETIMES;
//	}
//	
//	return ucRet;
//}
//
////��ȡ���峣�����ĵ�Ԥ����Ϣ
//unsigned char Alarm_GetForeJudgeInfo_PulseConst(STALRFOREJUDGE *pstAlrForeJudge)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	pstAlrForeJudge->ucAlrJudgeTimes	=1;
//	pstAlrForeJudge->ucAlrJudgeCycle	=1;
//	pstAlrForeJudge->ucAlrDataItemNum	=2;
//	for(i=0;i<CONALRFOREREPLYNUM;i++)
//	{
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].usItemLabel=CON_DI_TYPE_MPPOWCONST;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[1].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[1].usItemLabel=CON_DI_TYPE_MPVARCONST;
//	}
//	
//	return ucRet;
//}
//
////��ȡ�����ո��ĵ�Ԥ����Ϣ
//unsigned char Alarm_GetForeJudgeInfo_CopyDayTime(STALRFOREJUDGE *pstAlrForeJudge)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	pstAlrForeJudge->ucAlrJudgeTimes	=1;
//	pstAlrForeJudge->ucAlrJudgeCycle	=1;
//	pstAlrForeJudge->ucAlrDataItemNum	=1;
//	for(i=0;i<CONALRFOREREPLYNUM;i++)
//	{
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].usItemLabel=CON_DI_TYPE_MPCOPYDAYTIME;
//	}
//	
//	return ucRet;
//}
//
////��ȡ����������ĵ�Ԥ����Ϣ
//unsigned char Alarm_GetForeJudgeInfo_LostVolNum(STALRFOREJUDGE *pstAlrForeJudge)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	pstAlrForeJudge->ucAlrJudgeTimes	=1;
//	pstAlrForeJudge->ucAlrJudgeCycle	=1;
//	pstAlrForeJudge->ucAlrDataItemNum	=1;
//	for(i=0;i<CONALRFOREREPLYNUM;i++)
//	{
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].usItemLabel=CON_DI_TYPE_MPLOSTVOLNUM;
//	}
//	
//	return ucRet;
//}
//
////��ȡ���ʸ��ĵ�Ԥ����Ϣ
//unsigned char Alarm_GetForeJudgeInfo_FeeRate(STALRFOREJUDGE *pstAlrForeJudge)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	pstAlrForeJudge->ucAlrJudgeTimes	=1;
//	pstAlrForeJudge->ucAlrJudgeCycle	=1;
//	pstAlrForeJudge->ucAlrDataItemNum	=1;
//	for(i=0;i<CONALRFOREREPLYNUM;i++)
//	{
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].usItemLabel=CON_DI_TYPE_MPFEERATE_BLOCK;
//	}
//	
//	return ucRet;
//}

//��ȡ���ͣ�ߵ�Ԥ����Ϣ
//unsigned char Alarm_GetForeJudgeInfo_MeterStop(STALRFOREJUDGE *pstAlrForeJudge)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
////	pstAlrForeJudge->ucAlrJudgeTimes	=5;
////	pstAlrForeJudge->ucAlrJudgeCycle	=4;
//	pstAlrForeJudge->ucAlrJudgeTimes	=2;		
//	pstAlrForeJudge->ucAlrJudgeCycle	=2;
//	pstAlrForeJudge->ucAlrDataItemNum	=5;
//	for(i=0;i<CONALRFOREREPLYNUM;i++)
//	{
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].usItemLabel=CON_DI_TYPE_MPDIRPOWTOTAL;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[1].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[1].usItemLabel=CON_DI_TYPE_MPUNDIRPOWTOTAL;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[2].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[2].usItemLabel=CON_DI_TYPE_MPCURA;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[3].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[3].usItemLabel=CON_DI_TYPE_MPCURB;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[4].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[4].usItemLabel=CON_DI_TYPE_MPCURC;
//	}
//	
//	return ucRet;
//}

////��ȡ��ص�ѹ�͵�Ԥ����Ϣ
//unsigned char Alarm_GetForeJudgeInfo_MeterBattLow(STALRFOREJUDGE *pstAlrForeJudge)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	pstAlrForeJudge->ucAlrJudgeTimes	=1;
//	pstAlrForeJudge->ucAlrJudgeCycle	=1;
//	pstAlrForeJudge->ucAlrDataItemNum	=1;
//	for(i=0;i<CONALRFOREREPLYNUM;i++)
//	{
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].usItemLabel=CON_DI_TYPE_MPMETERSTATFLAG;
//	}
//	
//	return ucRet;
//}
//
////��ȡ���ʱ���쳣��Ԥ����Ϣ
//unsigned char Alarm_GetForeJudgeInfo_MeterTimeErr(STALRFOREJUDGE *pstAlrForeJudge)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	pstAlrForeJudge->ucAlrJudgeTimes	=1;
//	pstAlrForeJudge->ucAlrJudgeCycle	=1;
//	pstAlrForeJudge->ucAlrDataItemNum	=2;
//	for(i=0;i<CONALRFOREREPLYNUM;i++)
//	{
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[0].usItemLabel=CON_DI_TYPE_MPDATEANDWEEK;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[1].ucMPNo = pstAlrForeJudge->ucMPNo;
//		pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[1].usItemLabel=CON_DI_TYPE_MPTIME;
//	}
//	
//	return ucRet;
//}
//
//------------------------ ��ȡ�澯ȷ����Ϣ -------------------------------
////��ȡ��ѹԽ���޵�ȷ����Ϣ
//unsigned char Alarm_GetEnsureJudgeInfo_VolOver(STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucMPType= CON_MPTYPE_NULL;
//	STHISVAL	  stHisVal;
//
//	ucMPType = RunPara_GetMPType(pstAlrEnsureJudge->ucMPNo);
//	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
//	pstAlrEnsureJudge->ucAlrDataItemNum	=3;
//	pstAlrEnsureJudge->stAlrDataItem[0].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[0].usItemLabel=CON_DI_TYPE_MPVOLA;
//	pstAlrEnsureJudge->stAlrDataItem[1].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[1].usItemLabel=CON_DI_TYPE_MPVOLB;
//	pstAlrEnsureJudge->stAlrDataItem[2].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[2].usItemLabel=CON_DI_TYPE_MPVOLC;
//	if(ucMPType == CON_MPTYPE_TER)
//	{
//		stHisVal = Get_History_V_byMPNo(pstAlrEnsureJudge->ucMPNo,0,1);
//		pstAlrEnsureJudge->stAlrDataItem[0].usDataLen = 4;
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,&(stHisVal.lVal),4);
//
//		stHisVal = Get_History_V_byMPNo(pstAlrEnsureJudge->ucMPNo,0,2);
//		pstAlrEnsureJudge->stAlrDataItem[1].usDataLen = 4;
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,&(stHisVal.lVal),4);
//
//		stHisVal = Get_History_V_byMPNo(pstAlrEnsureJudge->ucMPNo,0,3);
//		pstAlrEnsureJudge->stAlrDataItem[2].usDataLen = 4;
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[2].ucDataContent,&(stHisVal.lVal),4);
//
//		ucRet =RET_DATA_AVAILABLE; 
//	}
//	return ucRet;
//}
//
////��ȡA�����Խ�޵�ȷ����Ϣ
//unsigned char Alarm_GetEnsureJudgeInfo_CurOverUp(STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucMPType= CON_MPTYPE_NULL;
//	STHISVAL	  stHisVal;
//
//	ucMPType = RunPara_GetMPType(pstAlrEnsureJudge->ucMPNo);
//	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
//	pstAlrEnsureJudge->ucAlrDataItemNum	=3;
//	pstAlrEnsureJudge->stAlrDataItem[0].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[0].usItemLabel=CON_DI_TYPE_MPCURA;
//	pstAlrEnsureJudge->stAlrDataItem[1].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[1].usItemLabel=CON_DI_TYPE_MPCURB;
//	pstAlrEnsureJudge->stAlrDataItem[2].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[2].usItemLabel=CON_DI_TYPE_MPCURC;
//	if(ucMPType == CON_MPTYPE_TER)
//	{
//		stHisVal = Get_History_I_byMPNo(pstAlrEnsureJudge->ucMPNo,0,1);
//		pstAlrEnsureJudge->stAlrDataItem[0].usDataLen = 4;
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,&(stHisVal.lVal),4);
//
//		stHisVal = Get_History_I_byMPNo(pstAlrEnsureJudge->ucMPNo,0,2);
//		pstAlrEnsureJudge->stAlrDataItem[1].usDataLen = 4;
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,&(stHisVal.lVal),4);
//
//		stHisVal = Get_History_I_byMPNo(pstAlrEnsureJudge->ucMPNo,0,3);
//		pstAlrEnsureJudge->stAlrDataItem[2].usDataLen = 4;
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[2].ucDataContent,&(stHisVal.lVal),4);
//
//		ucRet =RET_DATA_AVAILABLE; 
//	}
//	return ucRet;
//}
//
////��ȡA�����Խ�����޵�ȷ����Ϣ
//unsigned char Alarm_GetEnsureJudgeInfo_CurHeavyOverUp(STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucMPType= CON_MPTYPE_NULL;
//	STHISVAL	  stHisVal;
//
//	ucMPType = RunPara_GetMPType(pstAlrEnsureJudge->ucMPNo);
//	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
//	pstAlrEnsureJudge->ucAlrDataItemNum	=3;
//	pstAlrEnsureJudge->stAlrDataItem[0].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[0].usItemLabel=CON_DI_TYPE_MPCURA;
//	pstAlrEnsureJudge->stAlrDataItem[1].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[1].usItemLabel=CON_DI_TYPE_MPCURB;
//	pstAlrEnsureJudge->stAlrDataItem[2].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[2].usItemLabel=CON_DI_TYPE_MPCURC;
//	if(ucMPType == CON_MPTYPE_TER)
//	{
//		stHisVal = Get_History_I_byMPNo(pstAlrEnsureJudge->ucMPNo,0,1);
//		pstAlrEnsureJudge->stAlrDataItem[0].usDataLen = 4;
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,&(stHisVal.lVal),4);
//
//		stHisVal = Get_History_I_byMPNo(pstAlrEnsureJudge->ucMPNo,0,2);
//		pstAlrEnsureJudge->stAlrDataItem[1].usDataLen = 4;
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,&(stHisVal.lVal),4);
//
//		stHisVal = Get_History_I_byMPNo(pstAlrEnsureJudge->ucMPNo,0,3);
//		pstAlrEnsureJudge->stAlrDataItem[2].usDataLen = 4;
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[2].ucDataContent,&(stHisVal.lVal),4);
//
//		ucRet =RET_DATA_AVAILABLE; 
//	}
//	return ucRet;
//}
//
//unsigned char Alarm_GetEnsureJudgeInfo_VabcBreak(STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	STHISVAL	  stHisVal;
//	unsigned char ucMPType= CON_MPTYPE_NULL;
//
//	ucMPType = RunPara_GetMPType(pstAlrEnsureJudge->ucMPNo);
//	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
//	pstAlrEnsureJudge->ucAlrDataItemNum	=7;
//	pstAlrEnsureJudge->stAlrDataItem[0].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[0].usItemLabel=CON_DI_TYPE_MPVOLA;
//	pstAlrEnsureJudge->stAlrDataItem[1].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[1].usItemLabel=CON_DI_TYPE_MPVOLB;
//	pstAlrEnsureJudge->stAlrDataItem[2].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[2].usItemLabel=CON_DI_TYPE_MPVOLC;
//	pstAlrEnsureJudge->stAlrDataItem[3].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[3].usItemLabel=CON_DI_TYPE_MPCURA;
//	pstAlrEnsureJudge->stAlrDataItem[4].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[4].usItemLabel=CON_DI_TYPE_MPCURB;
//	pstAlrEnsureJudge->stAlrDataItem[5].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[5].usItemLabel=CON_DI_TYPE_MPCURC;
//	pstAlrEnsureJudge->stAlrDataItem[6].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[6].usItemLabel=CON_DI_TYPE_MPDIRPOWTOTAL;
//
//	if(ucMPType == CON_MPTYPE_TER)
//	{
//		stHisVal = Get_History_V_byMPNo(pstAlrEnsureJudge->ucMPNo,0,1);
//		pstAlrEnsureJudge->stAlrDataItem[0].usDataLen = 4;
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,&(stHisVal.lVal),4);
//
//		stHisVal = Get_History_V_byMPNo(pstAlrEnsureJudge->ucMPNo,0,2);
//		pstAlrEnsureJudge->stAlrDataItem[1].usDataLen = 4;
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,&(stHisVal.lVal),4);
//
//		stHisVal = Get_History_V_byMPNo(pstAlrEnsureJudge->ucMPNo,0,3);
//		pstAlrEnsureJudge->stAlrDataItem[2].usDataLen = 4;
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[2].ucDataContent,&(stHisVal.lVal),4);
//
//		stHisVal = Get_History_I_byMPNo(pstAlrEnsureJudge->ucMPNo,0,1);
//		pstAlrEnsureJudge->stAlrDataItem[3].usDataLen = 4;
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[3].ucDataContent,&(stHisVal.lVal),4);
//
//		stHisVal = Get_History_I_byMPNo(pstAlrEnsureJudge->ucMPNo,0,2);
//		pstAlrEnsureJudge->stAlrDataItem[4].usDataLen = 4;
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[4].ucDataContent,&(stHisVal.lVal),4);
//
//		stHisVal = Get_History_I_byMPNo(pstAlrEnsureJudge->ucMPNo,0,3);
//		pstAlrEnsureJudge->stAlrDataItem[5].usDataLen = 4;
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[5].ucDataContent,&(stHisVal.lVal),4);
//
//		stHisVal = Get_History_DirPower_byMPNo(CON_METER_BASE_MPNO,0,0);
//		pstAlrEnsureJudge->stAlrDataItem[6].usDataLen = 4;
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[6].ucDataContent,&(stHisVal.lVal),4);
//
//		ucRet =RET_DATA_AVAILABLE; 
//	}
//	return ucRet;
//}
//
////��ȡ���ڹ���Խ�޵�ȷ����Ϣ
//unsigned char Alarm_GetEnsureJudgeInfo_SPowOverUp(STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	STHISVAL	  stHisVal;
//	unsigned char ucMPType= CON_MPTYPE_NULL;
//
//	ucMPType = RunPara_GetMPType(pstAlrEnsureJudge->ucMPNo);
//	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
//	pstAlrEnsureJudge->ucAlrDataItemNum	=2;
//	pstAlrEnsureJudge->stAlrDataItem[0].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[0].usItemLabel=CON_DI_TYPE_MPINSTANTPOW;
//	pstAlrEnsureJudge->stAlrDataItem[1].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[1].usItemLabel=CON_DI_TYPE_MPINSTANTVAR;
//	if(ucMPType == CON_MPTYPE_TER || ucMPType == CON_MPTYPE_PULSE)
//	{
//		stHisVal = Get_History_InstantPow_byMPNo(pstAlrEnsureJudge->ucMPNo,0,0);
//		pstAlrEnsureJudge->stAlrDataItem[0].usDataLen = 4;
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,&(stHisVal.lVal),4);
//
//		stHisVal = Get_History_InstantVar_byMPNo(pstAlrEnsureJudge->ucMPNo,0,0);
//		pstAlrEnsureJudge->stAlrDataItem[1].usDataLen = 4;
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,&(stHisVal.lVal),4);
//
//		ucRet =RET_DATA_AVAILABLE; 
//	}
//	return ucRet;
//}
//
////��ȡ�������/��ѹ��ƽ��Խ�޵�ȷ����Ϣ
//unsigned char Alarm_GetEnsureJudgeInfo_UnBalanceOverUp(STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucMPType= CON_MPTYPE_NULL;
//	STHISVAL	  stHisVal;
//
//	ucMPType = RunPara_GetMPType(pstAlrEnsureJudge->ucMPNo);
//	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
//	pstAlrEnsureJudge->ucAlrDataItemNum	=6;
//	pstAlrEnsureJudge->stAlrDataItem[0].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[0].usItemLabel=CON_DI_TYPE_MPVOLA;
//	pstAlrEnsureJudge->stAlrDataItem[1].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[1].usItemLabel=CON_DI_TYPE_MPVOLB;
//	pstAlrEnsureJudge->stAlrDataItem[2].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[2].usItemLabel=CON_DI_TYPE_MPVOLC;
//
//	pstAlrEnsureJudge->stAlrDataItem[3].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[3].usItemLabel=CON_DI_TYPE_MPCURA;
//	pstAlrEnsureJudge->stAlrDataItem[4].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[4].usItemLabel=CON_DI_TYPE_MPCURB;
//	pstAlrEnsureJudge->stAlrDataItem[5].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[5].usItemLabel=CON_DI_TYPE_MPCURC;
//
//	if(ucMPType == CON_MPTYPE_TER)
//	{
//		stHisVal = Get_History_V_byMPNo(pstAlrEnsureJudge->ucMPNo,0,1);
//		pstAlrEnsureJudge->stAlrDataItem[0].usDataLen = 4;
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,&(stHisVal.lVal),4);
//
//		stHisVal = Get_History_V_byMPNo(pstAlrEnsureJudge->ucMPNo,0,2);
//		pstAlrEnsureJudge->stAlrDataItem[1].usDataLen = 4;
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,&(stHisVal.lVal),4);
//
//		stHisVal = Get_History_V_byMPNo(pstAlrEnsureJudge->ucMPNo,0,3);
//		pstAlrEnsureJudge->stAlrDataItem[2].usDataLen = 4;
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[2].ucDataContent,&(stHisVal.lVal),4);
//
//		stHisVal = Get_History_I_byMPNo(pstAlrEnsureJudge->ucMPNo,0,1);
//		pstAlrEnsureJudge->stAlrDataItem[3].usDataLen = 4;
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[3].ucDataContent,&(stHisVal.lVal),4);
//
//		stHisVal = Get_History_I_byMPNo(pstAlrEnsureJudge->ucMPNo,0,2);
//		pstAlrEnsureJudge->stAlrDataItem[4].usDataLen = 4;
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[4].ucDataContent,&(stHisVal.lVal),4);
//
//		stHisVal = Get_History_I_byMPNo(pstAlrEnsureJudge->ucMPNo,0,3);
//		pstAlrEnsureJudge->stAlrDataItem[5].usDataLen = 4;
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[5].ucDataContent,&(stHisVal.lVal),4);
//
//		ucRet =RET_DATA_AVAILABLE; 
//	}
//	return ucRet;
//}
//
////��ȡ���ʱ����ĵ�ȷ����Ϣ
//unsigned char Alarm_GetEnsureJudgeInfo_ProgramTime(STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
//	pstAlrEnsureJudge->ucAlrDataItemNum	=1;
//	pstAlrEnsureJudge->stAlrDataItem[0].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[0].usItemLabel=CON_DI_TYPE_MPLATELYPROGTIME;
//	return ucRet;
//}
//
////��ȡ��̴��������ȷ����Ϣ
//unsigned char Alarm_GetEnsureJudgeInfo_ProgramCount(STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
//	pstAlrEnsureJudge->ucAlrDataItemNum	=1;
//	pstAlrEnsureJudge->stAlrDataItem[0].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[0].usItemLabel=CON_DI_TYPE_MPPROGTIMETIMES;
//	return ucRet;
//}
//
////��ȡ���峣�����ĵ�ȷ����Ϣ
//unsigned char Alarm_GetEnsureJudgeInfo_PulseConst(STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
//	pstAlrEnsureJudge->ucAlrDataItemNum	=2;
//	pstAlrEnsureJudge->stAlrDataItem[0].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[0].usItemLabel=CON_DI_TYPE_MPPOWCONST;
//	pstAlrEnsureJudge->stAlrDataItem[1].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[1].usItemLabel=CON_DI_TYPE_MPVARCONST;
//	return ucRet;
//}
////��ȡ�����ո��ĵ�ȷ����Ϣ
//unsigned char Alarm_GetEnsureJudgeInfo_CopyDayTime(STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
//	pstAlrEnsureJudge->ucAlrDataItemNum	=1;
//	pstAlrEnsureJudge->stAlrDataItem[0].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[0].usItemLabel=CON_DI_TYPE_MPCOPYDAYTIME;
//	return ucRet;
//}
////��ȡ���������ȷ����Ϣ
//unsigned char Alarm_GetEnsureJudgeInfo_LostVolNum(STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
//	pstAlrEnsureJudge->ucAlrDataItemNum	=1;
//	pstAlrEnsureJudge->stAlrDataItem[0].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[0].usItemLabel=CON_DI_TYPE_MPLOSTVOLNUM;
//	return ucRet;
//}
//
////��ȡ���ʸ��ĵ�ȷ����Ϣ
//unsigned char Alarm_GetEnsureJudgeInfo_FeeRate(STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
//	pstAlrEnsureJudge->ucAlrDataItemNum	=1;
//	pstAlrEnsureJudge->stAlrDataItem[0].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[0].usItemLabel=CON_DI_TYPE_MPFEERATE_BLOCK;
//	return ucRet;
//}

////��ȡ��Ʒ��ߵ�ȷ����Ϣ
//unsigned char Alarm_GetEnsureJudgeInfo_MeterFly(STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_DATA_AVAILABLE;
//	STHISVAL	stHisVal;
//	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
//	pstAlrEnsureJudge->ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->ucAlrDataItemNum	=2;
//	stHisVal = Get_History_DirPower_byMPNo(pstAlrEnsureJudge->ucMPNo,1,0);
//	memcpy(pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,&(stHisVal.lVal),4);
//	pstAlrEnsureJudge->stAlrDataItem[0].usDataLen=4;
//	stHisVal = Get_History_DirPower_byMPNo(pstAlrEnsureJudge->ucMPNo,0,0);
//	memcpy(pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,&(stHisVal.lVal),4);
//	pstAlrEnsureJudge->stAlrDataItem[1].usDataLen=4;
//	return ucRet;
//}
//unsigned char Alarm_GetEnsureJudgeInfo_PowDiffOver(STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_DATA_AVAILABLE;
//	STHISVAL	stHisVal;
//	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
//	pstAlrEnsureJudge->ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->ucAlrDataItemNum	=2;
//	stHisVal = Get_History_DirPower_byMPNo(pstAlrEnsureJudge->ucMPNo,1,0);
//	memcpy(pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,&(stHisVal.lVal),4);
//	pstAlrEnsureJudge->stAlrDataItem[0].usDataLen=4;
//	stHisVal = Get_History_DirPower_byMPNo(pstAlrEnsureJudge->ucMPNo,0,0);
//	memcpy(pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,&(stHisVal.lVal),4);
//	pstAlrEnsureJudge->stAlrDataItem[1].usDataLen=4;
//	return ucRet;
//}


unsigned char Alarm_GetEnsureJudgeInfo_MeterState(STALRENSUREJUDGE *pstAlrEnsureJudge)
{
	unsigned char ucRet=RET_DATA_AVAILABLE;
	STHISSTATE	stHisVal;
	unsigned char ucBuffer[14];
	unsigned char i,j;
	unsigned char ucMPNo = pstAlrEnsureJudge->ucMPNo;
	unsigned char ucMeterNo = RunPara_GetMeterNo_byMPNo(pstAlrEnsureJudge->ucMPNo);
	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
	pstAlrEnsureJudge->ucMPNo=pstAlrEnsureJudge->ucMPNo;
	
	pstAlrEnsureJudge->ucAlrDataItemNum	=2;
	for(i=0;i<7;i++)
	{
		ucBuffer[2*i] = ucMeterStateChangeFlag[ucMeterNo][i];
		ucBuffer[2*i+1] = 0;
	}
	memcpy(pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,ucBuffer,14);
	pstAlrEnsureJudge->stAlrDataItem[0].usDataLen=14;
	for(i=0;i<7;i++)
	{
		j=i+1;
		stHisVal = Get_History_MeterStatFlag_byMPNo(ucMPNo,0,j);
		ucBuffer[2*i] = stHisVal.ucStateFlag;
		ucBuffer[2*i+1] = 0;
	}
	memcpy(pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,ucBuffer,14);
	pstAlrEnsureJudge->stAlrDataItem[1].usDataLen=14;
	return ucRet;
}

//��ȡ���ͣ�ߵ�ȷ����Ϣ
//unsigned char Alarm_GetEnsureJudgeInfo_MeterStop(STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
//	pstAlrEnsureJudge->ucAlrDataItemNum	=2;
//	pstAlrEnsureJudge->stAlrDataItem[0].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[0].usItemLabel=CON_DI_TYPE_MPDIRPOWTOTAL;
//	pstAlrEnsureJudge->stAlrDataItem[1].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[1].usItemLabel=CON_DI_TYPE_MPUNDIRPOWTOTAL;
//	return ucRet;
//}
////��ȡʾ���½���ȷ����Ϣ
//unsigned char Alarm_GetEnsureJudgeInfo_MeterDown(STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_DATA_AVAILABLE;
//	STHISVAL	stHisVal;
//	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
//	pstAlrEnsureJudge->ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->ucAlrDataItemNum	=2;
//	stHisVal = Get_History_DirPower_byMPNo(pstAlrEnsureJudge->ucMPNo,1,0);
//	memcpy(pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,&(stHisVal.lVal),4);
//	pstAlrEnsureJudge->stAlrDataItem[0].usDataLen=4;
//	stHisVal = Get_History_DirPower_byMPNo(pstAlrEnsureJudge->ucMPNo,0,0);
//	memcpy(pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,&(stHisVal.lVal),4);
//	pstAlrEnsureJudge->stAlrDataItem[1].usDataLen=4;
//	return ucRet;
//}

////��ȡ��ص�ѹ�͵�ȷ����Ϣ
//unsigned char Alarm_GetEnsureJudgeInfo_MeterBattLow(STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
//	pstAlrEnsureJudge->ucAlrDataItemNum	=1;
//	pstAlrEnsureJudge->stAlrDataItem[0].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[0].usItemLabel=CON_DI_TYPE_MPMETERSTATFLAG;
//	return ucRet;
//}
//
////��ȡ���ʱ���쳣��ȷ����Ϣ
//unsigned char Alarm_GetEnsureJudgeInfo_MeterTimeErr(STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
//	pstAlrEnsureJudge->ucAlrDataItemNum	=2;
//	pstAlrEnsureJudge->stAlrDataItem[0].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[0].usItemLabel=CON_DI_TYPE_MPDATEANDWEEK;
//	pstAlrEnsureJudge->stAlrDataItem[1].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[1].usItemLabel=CON_DI_TYPE_MPTIME;
//	return ucRet;
//}

unsigned char Alarm_GetEnsureJudgeInfo_AllDataInit(STALRENSUREJUDGE *pstAlrEnsureJudge)
{
	unsigned char ucRet=RET_DATA_AVAILABLE;
	unsigned char m=0;
	STTERAUXPARA	stTerAuxPara;
	stTerAuxPara= RunPara_GetTerAuxPara();
	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
	pstAlrEnsureJudge->ucMPNo	=RunPara_GetTer_MPNo();
	pstAlrEnsureJudge->ucAlrDataItemNum	=1;
	pstAlrEnsureJudge->stAlrDataItem[0].usDataLen=9;
	pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent[m++]=0x01;		//���в�������������ʼ��
	memcpy(pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent+m,stTerAuxPara.ucSoftwareVerOld,4);	//���ǰ����汾��
	m+=4;
	memcpy(pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent+m,stTerAuxPara.ucSoftwareVer,4);	//���������汾��
	return ucRet;
}

unsigned char Alarm_GetEnsureJudgeInfo_SoftwareChange(STALRENSUREJUDGE *pstAlrEnsureJudge)
{
	unsigned char ucRet=RET_DATA_AVAILABLE;
	unsigned char m=0;//,*pucFlag;
	STTERAUXPARA	stTerAuxPara;
	stTerAuxPara= RunPara_GetTerAuxPara();
	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
	pstAlrEnsureJudge->ucMPNo	=RunPara_GetTer_MPNo();
	pstAlrEnsureJudge->ucAlrDataItemNum	=1;
	pstAlrEnsureJudge->stAlrDataItem[0].usDataLen=9;
	pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent[m++]=0x02;		//�������
//azh	pucFlag=(unsigned char *)(RM_FLAG_START_ADDR+DP_DOWNLOAD_OLD_VERSION_ADDR);
//azh��ʱ
	memcpy(pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent+m,stTerAuxPara.ucSoftwareVer,4);	//���ǰ����汾��
	m+=4;
	memcpy(pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent+m,stTerAuxPara.ucSoftwareVer,4);	//���������汾��
	return ucRet;
}

//unsigned char Alarm_GetEnsureJudgeInfo_CurrentErr(STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	STHISVAL	stHisVal;
//	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
//	pstAlrEnsureJudge->ucAlrDataItemNum	=7;
//
//	pstAlrEnsureJudge->stAlrDataItem[0].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[0].usItemLabel=CON_DI_TYPE_MPVOLA;
//	stHisVal = Get_History_V_byMPNo(pstAlrEnsureJudge->ucMPNo,0,1);
//	pstAlrEnsureJudge->stAlrDataItem[0].usDataLen = 4;
//	memcpy(pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,&(stHisVal.lVal),4);
//
//	pstAlrEnsureJudge->stAlrDataItem[1].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[1].usItemLabel=CON_DI_TYPE_MPVOLB;
//	stHisVal = Get_History_V_byMPNo(pstAlrEnsureJudge->ucMPNo,0,2);
//	pstAlrEnsureJudge->stAlrDataItem[1].usDataLen = 4;
//	memcpy(pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,&(stHisVal.lVal),4);
//
//	pstAlrEnsureJudge->stAlrDataItem[2].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[2].usItemLabel=CON_DI_TYPE_MPVOLC;
//	stHisVal = Get_History_V_byMPNo(pstAlrEnsureJudge->ucMPNo,0,3);
//	pstAlrEnsureJudge->stAlrDataItem[2].usDataLen = 4;
//	memcpy(pstAlrEnsureJudge->stAlrDataItem[2].ucDataContent,&(stHisVal.lVal),4);
//
//	pstAlrEnsureJudge->stAlrDataItem[3].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[3].usItemLabel=CON_DI_TYPE_MPCURA;
//	stHisVal = Get_History_I_byMPNo(pstAlrEnsureJudge->ucMPNo,0,1);
//	pstAlrEnsureJudge->stAlrDataItem[3].usDataLen = 4;
//	memcpy(pstAlrEnsureJudge->stAlrDataItem[3].ucDataContent,&(stHisVal.lVal),4);
//
//	pstAlrEnsureJudge->stAlrDataItem[4].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[4].usItemLabel=CON_DI_TYPE_MPCURB;
//	stHisVal = Get_History_I_byMPNo(pstAlrEnsureJudge->ucMPNo,0,2);
//	pstAlrEnsureJudge->stAlrDataItem[4].usDataLen = 4;
//	memcpy(pstAlrEnsureJudge->stAlrDataItem[4].ucDataContent,&(stHisVal.lVal),4);
//
//	pstAlrEnsureJudge->stAlrDataItem[5].ucMPNo=pstAlrEnsureJudge->ucMPNo;
//	pstAlrEnsureJudge->stAlrDataItem[5].usItemLabel=CON_DI_TYPE_MPCURC;
//	stHisVal = Get_History_I_byMPNo(pstAlrEnsureJudge->ucMPNo,0,3);
//	pstAlrEnsureJudge->stAlrDataItem[5].usDataLen = 4;
//	memcpy(pstAlrEnsureJudge->stAlrDataItem[5].ucDataContent,&(stHisVal.lVal),4);
//
//	pstAlrEnsureJudge->stAlrDataItem[6].ucMPNo=CON_METER_BASE_MPNO;
//	pstAlrEnsureJudge->stAlrDataItem[6].usItemLabel=CON_DI_TYPE_MPDIRPOWTOTAL;
//	return ucRet;
//}

//unsigned char Alarm_GetEnsureJudgeInfo_SumCompOverLimit(STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char i=0,ucPos=1;
//	unsigned char ucSumGNoComp=0,ucSumGNoReff=0;
//	unsigned char ucCompInter=0;
//	unsigned char ucMPNum=0,ucMPNo=0,ucMPSum[CON_MAX_MPNUM];
//	unsigned char ucDataItemNum=0;	
//	unsigned char ucSumCompGroupNo=pstAlrEnsureJudge->usAlrCode-CONALRCODE_SUMCOMPOVERLIMIT;
//	float		  fPowVal=0,fPowValReff=0,fDiffVal=0,fTmp;
//	double		  dDiffVal=0;
//	STHISVAL	stHisVal;
//	STTERMISCPARA	stTerMiscPara;
//	STSUMGROUPPARA	*pstSumGParaTmp;
//	STHISSUMVALGROUP stPowSumVal,stPowSumVallast;
//	unsigned short m=0;
//	unsigned char ucDiffPer;
//
//	for(i=0;i<CON_MAX_MPNUM;i++)
//		ucMPSum[i]=0;
//
//	stTerMiscPara=RunPara_GetTerMiscPara();
//	ucSumGNoComp=stTerMiscPara.stPowDiffLimit[ucSumCompGroupNo].ucSumGNoComp;
//	ucSumGNoReff=stTerMiscPara.stPowDiffLimit[ucSumCompGroupNo].ucSumGNoReff;
//
//	ucCompInter=15;
//	ucPos = 1;
//	if((stTerMiscPara.stPowDiffLimit[ucSumCompGroupNo].ucFlag&0x03)==0)
//	{
//		ucCompInter=60;
//		ucPos = 4;
//	}
//	else if((stTerMiscPara.stPowDiffLimit[ucSumCompGroupNo].ucFlag&0x03)==1)
//	{
//		ucCompInter=30;
//		ucPos =2;
//	}
//	else if((stTerMiscPara.stPowDiffLimit[ucSumCompGroupNo].ucFlag&0x03)==2)
//	{
//		ucCompInter=15;
//		ucPos = 1;
//	}
//
//	stPowSumVallast=Get_History_PowValHourSumVal(ucPos);
//	stPowSumVal=Get_History_PowValSumVal(0,0);	
//	if( (stPowSumVal.stSumValGroup.fSumVal[ucSumGNoComp]!=CONINVALIDVAL) &&
//		(stPowSumVallast.stSumValGroup.fSumVal[ucSumGNoComp]!=CONINVALIDVAL)&&
//		(stPowSumVal.stSumValGroup.fSumVal[ucSumGNoReff]!=CONINVALIDVAL)&&
//		(stPowSumVallast.stSumValGroup.fSumVal[ucSumGNoReff]!=CONINVALIDVAL) )
//	{
//		fPowVal=stPowSumVal.stSumValGroup.fSumVal[ucSumGNoComp]-stPowSumVallast.stSumValGroup.fSumVal[ucSumGNoComp];
//		fPowValReff=stPowSumVal.stSumValGroup.fSumVal[ucSumGNoReff]-stPowSumVallast.stSumValGroup.fSumVal[ucSumGNoReff];
//
//		if(fPowVal>=fPowValReff)
//			fDiffVal=fPowVal-fPowValReff;
//		else
//			fDiffVal=fPowValReff-fPowVal;
//
//		if(fPowValReff<100)
//		{//С��1kwh
//			if(fDiffVal>100)
//				ucDiffPer=100;				//99%
//			else
//				ucDiffPer=0;
//		}
//		else
//		{
//			fTmp=fDiffVal*100/fPowValReff;
//			if(fTmp>100)
//				ucDiffPer=100;
//			else
//				ucDiffPer=(unsigned char)fTmp;
//		}
//		dDiffVal=fDiffVal;
//	}
//	
//	memcpy(pstAlrEnsureJudge->stAlrDataItem[ucDataItemNum].ucDataContent,&(fPowVal),4);
//	pstAlrEnsureJudge->stAlrDataItem[ucDataItemNum].usDataLen = 4;
//	ucDataItemNum++;
//
//	memcpy(pstAlrEnsureJudge->stAlrDataItem[ucDataItemNum].ucDataContent,&(fPowValReff),4);
//	pstAlrEnsureJudge->stAlrDataItem[ucDataItemNum].usDataLen = 4;
//	ucDataItemNum++;
//
////	memcpy(pstAlrEnsureJudge->stAlrDataItem[ucDataItemNum].ucDataContent,&(stTerMiscPara.stPowDiffLimit[ucSumCompGroupNo].ucDiffLimit),1);
//	memcpy(pstAlrEnsureJudge->stAlrDataItem[ucDataItemNum].ucDataContent,&ucDiffPer,1);
//	pstAlrEnsureJudge->stAlrDataItem[ucDataItemNum].usDataLen = 1;
//	ucDataItemNum++;
//
////	memcpy(pstAlrEnsureJudge->stAlrDataItem[ucDataItemNum].ucDataContent,&(stTerMiscPara.stPowDiffLimit[ucSumCompGroupNo].dDiffLimitVal),8);
//	memcpy(pstAlrEnsureJudge->stAlrDataItem[ucDataItemNum].ucDataContent,&dDiffVal,8);
//	pstAlrEnsureJudge->stAlrDataItem[ucDataItemNum].usDataLen = 8;
//	ucDataItemNum++;
//
//	//�Ա�
//	pstSumGParaTmp= RunPara_GetSumGroupParaPtr(ucSumGNoComp);
//	for(i=0;i<CON_MAX_MPNUM;i++)
//	{
//		if(pstSumGParaTmp->stPowVal.stDir.ucMPSum[i]==CON_MPNO_INVALID)
//			break;
//		else
//			ucMPSum[i]=pstSumGParaTmp->stPowVal.stDir.ucMPSum[i];
//	}
//	
//	if(i>8)  
//		ucMPNum=8;		//��ֹ��������Խ�磬���������ݳ���Ϊ48����ÿ��������ĳ���Ϊ5���ݶ����ϲ���������Ϊ8
//	else 
//		ucMPNum=i;
//
//	memcpy(pstAlrEnsureJudge->stAlrDataItem[ucDataItemNum].ucDataContent,&(ucMPNum),1);
//	pstAlrEnsureJudge->stAlrDataItem[ucDataItemNum].usDataLen = 1;
//	ucDataItemNum++;
//
//	if(ucDataItemNum<CONALRTAKEITEMNUM-1)	//	//ucDataItemNum=6<CONALRTAKEITEMNUM
//	{
//		pstAlrEnsureJudge->stAlrDataItem[ucDataItemNum].usDataLen = 5*ucMPNum;
//		m =0;
//		for(i=0;i<ucMPNum;i++)
//		{
//			ucMPNo=ucMPSum[i];
//			stHisVal=Get_History_DirPower_byMPNo(ucMPNo,0,0);
//			m=SetMainComm_Buf_lVal_5B_DT14(pstAlrEnsureJudge->stAlrDataItem[ucDataItemNum].ucDataContent,m,stHisVal.lVal);
//		}
//		ucDataItemNum++;
//	}
//	
//	//����
//	pstSumGParaTmp= RunPara_GetSumGroupParaPtr(ucSumGNoReff);
//	for(i=0;i<CON_MAX_MPNUM;i++)
//	{
//		if(pstSumGParaTmp->stPowVal.stDir.ucMPSum[i]==CON_MPNO_INVALID)
//			break;
//		else
//			ucMPSum[i]=pstSumGParaTmp->stPowVal.stDir.ucMPSum[i];
//	}
//
//	if(i>8)  
//		ucMPNum=8;
//	else 
//		ucMPNum=i;
//
//	if(ucDataItemNum<CONALRTAKEITEMNUM-1)		//ucDataItemNum=7<CONALRTAKEITEMNUM
//	{
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[ucDataItemNum].ucDataContent,&(ucMPNum),1);
//		pstAlrEnsureJudge->stAlrDataItem[ucDataItemNum].usDataLen = 1;
//		ucDataItemNum++;
//	}
//
//	if(ucDataItemNum<CONALRTAKEITEMNUM-1)		//ucDataItemNum=8<CONALRTAKEITEMNUM
//	{
//		pstAlrEnsureJudge->stAlrDataItem[ucDataItemNum].usDataLen = 5*ucMPNum;
//		m =0;
//		for(i=0;i<ucMPNum;i++)
//		{
//			ucMPNo=ucMPSum[i];
//			stHisVal=Get_History_DirPower_byMPNo(ucMPNo,0,0);			
//			m=SetMainComm_Buf_lVal_5B_DT14(pstAlrEnsureJudge->stAlrDataItem[ucDataItemNum].ucDataContent,m,stHisVal.lVal);
//		}
//		ucDataItemNum++;
//	}
//	
//	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
//	pstAlrEnsureJudge->ucAlrDataItemNum	=ucDataItemNum;
//	ucRet =RET_DATA_AVAILABLE; 
//	return ucRet;
//}
//unsigned char Alarm_GetEnsureJudgeInfo_VolCirReverse(STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
////	unsigned char ucMPType = CON_MPTYPE_NULL;
////	STHISVAL stHisVal;
////	ucMPType = RunPara_GetMPType(pstAlrEnsureJudge->ucMPNo);
//	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
//	pstAlrEnsureJudge->ucAlrDataItemNum	=1;
//
//	pstAlrEnsureJudge->stAlrDataItem[0].ucMPNo=CON_METER_BASE_MPNO;
//	pstAlrEnsureJudge->stAlrDataItem[0].usItemLabel=CON_DI_TYPE_MPDIRPOWTOTAL;
///*	if(ucMPType == CON_MPTYPE_TER)
//	{
//		stHisVal = Get_History_DirPower_byMPNo(pstAlrEnsureJudge->ucMPNo, 0, CON_FEE_TOTAL);
//		memcpy(pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,&(stHisVal.lVal),4);
//		pstAlrEnsureJudge->stAlrDataItem[0].usDataLen = 4;
//		ucRet=RET_DATA_AVAILABLE;
//	}*/
//	return ucRet;
//}

unsigned char Alarm_GetEnsureJudgeInfo_NoTake(STALRENSUREJUDGE *pstAlrEnsureJudge,unsigned char ucMPNo)
{
	unsigned char ucRet=RET_DATA_AVAILABLE;
	pstAlrEnsureJudge->stDateTime=GetCurrentTime();
	pstAlrEnsureJudge->ucMPNo	=ucMPNo;
	return ucRet;
}


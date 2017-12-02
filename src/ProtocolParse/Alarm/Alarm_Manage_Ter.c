/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Alarm_Manage_Ter.c
 Description    : 终端自身相关告警处理
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

//static STTERALARMJUDGECOUNT	stTerAlrJudgeCount;
//static STTERALARMJUDGEINFO	stTerAlrJudgeInfo;
//static STTERALARMJUDGECOUNT	stTerAlrMaxCount;
//static unsigned short usCTShortVal;

//static unsigned char Alarm_ForeJudge_DoorStat(void);
static unsigned char Alarm_ForeJudge_PowerStat(void);
//static unsigned char Alarm_ForeJudge_TerStat(void);
//static unsigned char Alarm_ForeJudge_SwitchStat(unsigned char ucSwitchNo);
//static unsigned char Alarm_ForeJudge_PolarityReverseA(void);		//A相电流反极性
//static unsigned char Alarm_ForeJudge_PolarityReverseB(void);		//B相电流反极性
//static unsigned char Alarm_ForeJudge_PolarityReverseC(void);		//C相电流反极性

//static unsigned char Alarm_ForeJudge_ShortCircuitA1(void);			//A相1次侧CT短路
//static unsigned char Alarm_ForeJudge_ShortCircuitB1(void);			//B相1次侧CT短路
//static unsigned char Alarm_ForeJudge_ShortCircuitC1(void);			//C相1次侧CT短路
//static unsigned char Alarm_ForeJudge_OpenCircuitA2(void);			//A相2次侧CT开路
//static unsigned char Alarm_ForeJudge_OpenCircuitB2(void);			//B相2次侧CT开路
//static unsigned char Alarm_ForeJudge_OpenCircuitC2(void);			//C相2次侧CT开路
//static unsigned char Alarm_ForeJudge_ShortCircuitA2(void);			//A相2次侧CT短路
//static unsigned char Alarm_ForeJudge_ShortCircuitB2(void);			//B相2次侧CT短路
//static unsigned char Alarm_ForeJudge_ShortCircuitC2(void);			//C相2次侧CT短路

//static unsigned char Alarm_ForeJudge_VaBreak_Ter(void);			//A相断相
//static unsigned char Alarm_ForeJudge_VbBreak_Ter(void);			//B相断相
//static unsigned char Alarm_ForeJudge_VcBreak_Ter(void);			//C相断相
//static unsigned char Alarm_ForeJudge_VaLack_Ter(void);				//A相缺相
//static unsigned char Alarm_ForeJudge_VbLack_Ter(void);				//B相缺相
//static unsigned char Alarm_ForeJudge_VcLack_Ter(void);				//C相缺相
//static unsigned char Alarm_ForeJudge_VolCirReverse(void);			//电压回路逆相序
//static unsigned char Alarm_ForeJudge_IUnBalanceOverUp_Ter(void);	//电流不平衡
//static unsigned char Alarm_ForeJudge_UUnBalanceOverUp_Ter(void);	//电压不平衡

//static unsigned char Alarm_ForeJudge_VolAErrDown(void);			//A相电压越下下限
//static unsigned char Alarm_ForeJudge_VolBErrDown(void);			//B相电压越下下限
//static unsigned char Alarm_ForeJudge_VolCErrDown(void);			//C相电压越下下限
//static unsigned char Alarm_ForeJudge_VolAErrUp(void);				//A相电压越上上限
//static unsigned char Alarm_ForeJudge_VolBErrUp(void);				//B相电压越上上限
//static unsigned char Alarm_ForeJudge_VolCErrUp(void);				//C相电压越上上限
//static unsigned char Alarm_ForeJudge_CurAErrUp(void);				//A相电流越上上限
//static unsigned char Alarm_ForeJudge_CurBErrUp(void);				//B相电流越上上限
//static unsigned char Alarm_ForeJudge_CurCErrUp(void);				//C相电流越上上限
//static unsigned char Alarm_ForeJudge_CurANorUp(void);				//A相电流越上限
//static unsigned char Alarm_ForeJudge_CurBNorUp(void);				//B相电流越上限
//static unsigned char Alarm_ForeJudge_CurCNorUp(void);				//C相电流越上限
//static unsigned char Alarm_ForeJudge_SPowNorUp_Ter(void);			//视在功率越上限
//static unsigned char Alarm_ForeJudge_SPowErrUp_Ter(void);			//视在功率越上上限

//static unsigned char Alarm_ForeJudge_TerValOverUp(	STHISVAL	stHisVal,	unsigned long ulLimit,	
//											unsigned short usBack,		unsigned char *pucAlrStat,	
//											unsigned char *pucAlrSumCount,unsigned char ucMaxAlrCount,
//											STDATETIME *pstAlrNewTime,    unsigned short  usAlrCode);
//static unsigned char Alarm_ForeJudge_TerValOverDown(	STHISVAL	stHisVal,	unsigned long ulLimit,	unsigned short usBack,unsigned char *pucAlrStat,	
//												unsigned char *pucAlrSumCount,unsigned char ucMaxAlrCount,
//												STDATETIME *pstAlrNewTime,    unsigned short  usAlrCode);
//static unsigned char Alarm_ForeJudge_TerStatOn(	STHISSTATE	stHisState,	unsigned char *pucAlrStat,
//											unsigned char *pucAlrSumCount,unsigned char ucMaxAlrCount,
//											STDATETIME *pstAlrNewTime,    unsigned short  usAlrCode);
//static unsigned char Alarm_Chk_SPow_Err(unsigned char ucMPNo);


//unsigned char Alarm_Init_Ter(void)
//{
//	unsigned char ucRet = RET_SUCCESS;
//	memset(&(stTerAlrJudgeCount),0,sizeof(stTerAlrJudgeCount));
//	memset(&(stTerAlrJudgeInfo),CONINVALIDVAL,sizeof(stTerAlrJudgeInfo));
//	return ucRet;
//}

//unsigned char Alarm_Manage_Ter(unsigned char ucAlrExeNo)
//{//5秒处理一次
//	unsigned char ucRet = RET_SUCCESS;
//	stTerAlrMaxCount = RunPara_GetAlrMaxCount();
//	switch(ucAlrExeNo)
//	{
//	case	0:
//	case	4:
//		if(Get_Shield_ACD_byAlrCode(CONALRCODE_POLREVERSEA,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD_NOT)
//		{
//			Alarm_ForeJudge_PolarityReverseA();	
//			Alarm_ForeJudge_PolarityReverseB();	
//			Alarm_ForeJudge_PolarityReverseC();
//		//	Alarm_ForeJudge_ShortCircuitA1();	
//		//	Alarm_ForeJudge_ShortCircuitB1();	
//		//	Alarm_ForeJudge_ShortCircuitC1();	
//		}
//		if(Get_Shield_ACD_byAlrCode(CONALRCODE_VOLCIRREVERSE,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD_NOT)
//			Alarm_ForeJudge_VolCirReverse();
//		break;
////	case	1:
////	case	5:
////		if(Get_Shield_ACD_byAlrCode(CONALRCODE_POLREVERSEA,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD_NOT)
////		{
////			Alarm_ForeJudge_OpenCircuitA2();		
////			Alarm_ForeJudge_OpenCircuitB2();		
////			Alarm_ForeJudge_OpenCircuitC2();	
////			Alarm_ForeJudge_ShortCircuitA2();	
////			Alarm_ForeJudge_ShortCircuitB2();	
////			Alarm_ForeJudge_ShortCircuitC2();	
////		}
////		break;
//	case	2:
//	case	6:
////		if((CON_PORT70_NOPOWER & PORT70_NO_POWER_MASK)	!= 0)
//        if(PORT0_IS_HIGH(P0_PD_CHK) != 0)//azh 有电才进行
//    {//终端判断速度快，停电时候应该屏蔽这些告警处理
//			if(Get_Shield_ACD_byAlrCode(CONALRCODE_VABREAK,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD_NOT)
//			{
//				Alarm_ForeJudge_VaBreak_Ter();			
//				Alarm_ForeJudge_VbBreak_Ter();			
//				Alarm_ForeJudge_VcBreak_Ter();	
//				Alarm_ForeJudge_VaLack_Ter();			
//				Alarm_ForeJudge_VbLack_Ter();			
//				Alarm_ForeJudge_VcLack_Ter();	
//			}
//			if(Get_Shield_ACD_byAlrCode(CONALRCODE_IUNBLANCE,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD_NOT)
//			{
//				Alarm_ForeJudge_IUnBalanceOverUp_Ter();		
//				Alarm_ForeJudge_UUnBalanceOverUp_Ter();
//			}
//		}
//		break;
//	case	3:
//	case	7:
//
//        //if((CON_PORT70_NOPOWER & PORT70_NO_POWER_MASK)	!= 0)
//        if(PORT0_IS_HIGH(P0_PD_CHK) != 0)
//		{//终端判断速度快，停电时候应该屏蔽这些告警处理
//			if(Get_Shield_ACD_byAlrCode(CONALRCODE_VAOVERUP,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD_NOT)
//			{
//				Alarm_ForeJudge_VolAErrDown();		
//				Alarm_ForeJudge_VolBErrDown();		
//				Alarm_ForeJudge_VolCErrDown();	
//				Alarm_ForeJudge_VolAErrUp();			
//				Alarm_ForeJudge_VolBErrUp();			
//				Alarm_ForeJudge_VolCErrUp();	
//			}
//			if(Get_Shield_ACD_byAlrCode(CONALRCODE_IAOVERUP,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD_NOT)
//			{
//				Alarm_ForeJudge_CurAErrUp();			
//				Alarm_ForeJudge_CurBErrUp();			
//				Alarm_ForeJudge_CurCErrUp();	
//				Alarm_ForeJudge_CurANorUp();			
//				Alarm_ForeJudge_CurBNorUp();			
//				Alarm_ForeJudge_CurCNorUp();	
//			}
//			if(Get_Shield_ACD_byAlrCode(CONALRCODE_SPOWNORUP,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD_NOT)
//			{
//				Alarm_ForeJudge_SPowNorUp_Ter();
//				Alarm_ForeJudge_SPowErrUp_Ter();
//			}
//		}
//		break;
//	}
//	
//	return ucRet;
//}

unsigned char Alarm_Manage_Ter_QuickPart(void)
{
	unsigned char ucRet = RET_SUCCESS;
//	Alarm_ForeJudge_DoorStat();
	Alarm_ForeJudge_PowerStat();
//	Alarm_ForeJudge_SwitchStat(0);
//	Alarm_ForeJudge_SwitchStat(1);
//	Alarm_ForeJudge_SwitchStat(2);
//	Alarm_ForeJudge_SwitchStat(3);
//	if(RunPara_GetSwitchPara_PulseIsTypeSwitch(0)==CON_STATUS_YES)
//		Alarm_ForeJudge_SwitchStat(4);
//	if(RunPara_GetSwitchPara_PulseIsTypeSwitch(1)==CON_STATUS_YES)
//		Alarm_ForeJudge_SwitchStat(5);
//	Alarm_ForeJudge_SwitchStat(6);
//	Alarm_ForeJudge_TerStat();
	return ucRet;
}

//门节点变换报警
//unsigned char Alarm_ForeJudge_DoorStat()
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	unsigned char	ucDoorStat=CON_SWITCH_STAT_CLOSE;
//	STHISSTATE stHisDoorState0,stHisDoorState1,stHisDoorState2,stHisDoorState3;
//	unsigned char	ucTmp;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//
//	ucDoorStat = Get_CurrentDoorStat();
//	Add_History_DoorStat_Ter(ucDoorStat);
//
//	stHisDoorState0 = Get_History_DoorStat_byMPNo(ucTerMPNo,0); 
//	stHisDoorState1 = Get_History_DoorStat_byMPNo(ucTerMPNo,1); 
//	stHisDoorState2 = Get_History_DoorStat_byMPNo(ucTerMPNo,2); 
//	stHisDoorState3 = Get_History_DoorStat_byMPNo(ucTerMPNo,3); 
//
//	if(	stHisDoorState0.ucStateFlag != CONINVALIDVAL && stHisDoorState1.ucStateFlag != CONINVALIDVAL &&
//		stHisDoorState2.ucStateFlag != CONINVALIDVAL && stHisDoorState3.ucStateFlag != CONINVALIDVAL)
//	{
//		ucTmp=Get_Switch_Change_Info();
//	//	ucTmp |= 0x80;
//		if(	stHisDoorState0.ucStateFlag == stHisDoorState1.ucStateFlag && stHisDoorState0.ucStateFlag == CON_SWITCH_STAT_CLOSE &&
//			stHisDoorState2.ucStateFlag == stHisDoorState3.ucStateFlag && stHisDoorState2.ucStateFlag == CON_SWITCH_STAT_OPEN)
//		{
//			Alarm_FillEnsureJudgeList(CONALRCODE_DOORCLOSE,ucTerMPNo);
//			ucTmp |= 0x80;	//cfh 10-04-26
//			Set_Switch_Change_Info(ucTmp);
//		}
//		if(	stHisDoorState0.ucStateFlag == stHisDoorState1.ucStateFlag && stHisDoorState0.ucStateFlag == CON_SWITCH_STAT_OPEN &&
//			stHisDoorState2.ucStateFlag == stHisDoorState3.ucStateFlag && stHisDoorState2.ucStateFlag == CON_SWITCH_STAT_CLOSE)
//		{
//			Alarm_FillEnsureJudgeList(CONALRCODE_DOOROPEN,ucTerMPNo);
//			ucTmp |= 0x80;	//cfh 10-04-26
//			Set_Switch_Change_Info(ucTmp);
//		}
//	}
//	return ucRet;
//}

//停电报警
unsigned char Alarm_ForeJudge_PowerStat()
{
	unsigned char	ucRet=RET_SUCCESS;
//	unsigned char	ucK6Val = 0x00;
	STHISSTATE stHisPowerState0,stHisPowerState1,stHisPowerState2,stHisPowerState3;
	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();

//	if((CON_PORT70_NOPOWER & PORT70_NO_POWER_MASK)==0)
    if(PORT0_IS_HIGH(P0_PD_CHK) == 0)
        Add_History_PowerStat_Ter(CON_SWITCH_STAT_CLOSE);					//0 掉电
	else
		Add_History_PowerStat_Ter(CON_SWITCH_STAT_OPEN);	

	stHisPowerState0 = Get_History_PowerStat_byMPNo(ucTerMPNo,0); 
	stHisPowerState1 = Get_History_PowerStat_byMPNo(ucTerMPNo,1); 
	stHisPowerState2 = Get_History_PowerStat_byMPNo(ucTerMPNo,2); 
	stHisPowerState3 = Get_History_PowerStat_byMPNo(ucTerMPNo,3); 

	if(	stHisPowerState0.ucStateFlag != CONINVALIDVAL && stHisPowerState1.ucStateFlag != CONINVALIDVAL &&
		stHisPowerState2.ucStateFlag != CONINVALIDVAL && stHisPowerState3.ucStateFlag != CONINVALIDVAL)
	{
		if(	stHisPowerState0.ucStateFlag == stHisPowerState1.ucStateFlag && stHisPowerState0.ucStateFlag == CON_SWITCH_STAT_CLOSE &&
			stHisPowerState2.ucStateFlag == stHisPowerState3.ucStateFlag && stHisPowerState2.ucStateFlag == CON_SWITCH_STAT_OPEN)
		{
			Alarm_FillEnsureJudgeList(CONALRCODE_POWEROFF,ucTerMPNo);
		}
		if(	stHisPowerState0.ucStateFlag == stHisPowerState1.ucStateFlag && stHisPowerState0.ucStateFlag == CON_SWITCH_STAT_OPEN &&
			stHisPowerState2.ucStateFlag == stHisPowerState3.ucStateFlag && stHisPowerState2.ucStateFlag == CON_SWITCH_STAT_CLOSE)
		{
			Alarm_FillEnsureJudgeList(CONALRCODE_POWERON,ucTerMPNo);
		}
	}
	return ucRet;
}

//终端异常状态
// unsigned char Alarm_ForeJudge_TerStat()
// {
// 	unsigned char ucRet=RET_SUCCESS;
// 	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
// 	STTERMISCPARA	stTerMiscPara=RunPara_GetTerMiscPara();
// 	//CON_STATUS_NO发生，CON_STATUS_YES恢复
// 	if( stTerMiscPara.stRCommRunInfo.ucRCommStat==CON_STATUS_NO	
// 		&& stTerMiscPara.stRCommRunInfo.ucRCommStatBak1==CON_STATUS_NO	
// 		&& stTerMiscPara.stRCommRunInfo.ucRCommStatBak2==CON_STATUS_NO	
// 		&& stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_MainComm == CONALR_STAT_HAPPEN_NOT)
// 	{
// 		stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_MainComm	= CONALR_STAT_HAPPEN;
// 		Alarm_FillEnsureJudgeList(CONALRCODE_TERERR_MAINCOMM,ucTerMPNo);
// 	}
// 	else if( stTerMiscPara.stRCommRunInfo.ucRCommStat==CON_STATUS_YES 
// 		&& stTerMiscPara.stRCommRunInfo.ucRCommStatBak1==CON_STATUS_YES	
// 		&& stTerMiscPara.stRCommRunInfo.ucRCommStatBak2==CON_STATUS_YES	
// 		&& stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_MainComm == CONALR_STAT_HAPPEN)
// 	{
// 		stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_MainComm	= CONALR_STAT_HAPPEN_NOT;
// 		Alarm_FillEnsureJudgeList(CONALRCODE_TERERR_MAINCOMM,ucTerMPNo);
// 	}
// 	return ucRet;
// }

//开关状态变换报警
//unsigned char Alarm_ForeJudge_SwitchStat( unsigned char ucSwitchNo )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	unsigned char	ucSwitchStat = CON_SWITCH_STAT_CLOSE;
//	STHISSTATE stHisSwitchState0,stHisSwitchState1,stHisSwitchState2,stHisSwitchState3;
//	unsigned char	ucTmp;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//
//	ucSwitchStat = Get_CurrentSwitchStat( ucSwitchNo );
//
//	switch(ucSwitchNo)
//	{
//	case	0:
//		Add_History_SwitchStat_Ter(ucSwitchStat,0);
//		stHisSwitchState0 = Get_History_SwitchStat_byMPNo(ucTerMPNo,0,0); 
//		stHisSwitchState1 = Get_History_SwitchStat_byMPNo(ucTerMPNo,1,0); 
//		stHisSwitchState2 = Get_History_SwitchStat_byMPNo(ucTerMPNo,2,0); 
//		stHisSwitchState3 = Get_History_SwitchStat_byMPNo(ucTerMPNo,3,0); 
//		break;
//	case	1:
//		Add_History_SwitchStat_Ter(ucSwitchStat,1);
//		stHisSwitchState0 = Get_History_SwitchStat_byMPNo(ucTerMPNo,0,1); 
//		stHisSwitchState1 = Get_History_SwitchStat_byMPNo(ucTerMPNo,1,1); 
//		stHisSwitchState2 = Get_History_SwitchStat_byMPNo(ucTerMPNo,2,1); 
//		stHisSwitchState3 = Get_History_SwitchStat_byMPNo(ucTerMPNo,3,1); 
//		break;
//	case	2:
//		Add_History_SwitchStat_Ter(ucSwitchStat,2);
//		stHisSwitchState0 = Get_History_SwitchStat_byMPNo(ucTerMPNo,0,2); 
//		stHisSwitchState1 = Get_History_SwitchStat_byMPNo(ucTerMPNo,1,2); 
//		stHisSwitchState2 = Get_History_SwitchStat_byMPNo(ucTerMPNo,2,2); 
//		stHisSwitchState3 = Get_History_SwitchStat_byMPNo(ucTerMPNo,3,2); 
//		break;
//	case	3:
//		Add_History_SwitchStat_Ter(ucSwitchStat,3);
//		stHisSwitchState0 = Get_History_SwitchStat_byMPNo(ucTerMPNo,0,3); 
//		stHisSwitchState1 = Get_History_SwitchStat_byMPNo(ucTerMPNo,1,3); 
//		stHisSwitchState2 = Get_History_SwitchStat_byMPNo(ucTerMPNo,2,3); 
//		stHisSwitchState3 = Get_History_SwitchStat_byMPNo(ucTerMPNo,3,3); 
//		break;
//	case	4:
//		Add_History_SwitchStat_Ter(ucSwitchStat,4);
//		stHisSwitchState0 = Get_History_SwitchStat_byMPNo(ucTerMPNo,0,4); 
//		stHisSwitchState1 = Get_History_SwitchStat_byMPNo(ucTerMPNo,1,4); 
//		stHisSwitchState2 = Get_History_SwitchStat_byMPNo(ucTerMPNo,2,4); 
//		stHisSwitchState3 = Get_History_SwitchStat_byMPNo(ucTerMPNo,3,4); 
//		break;
//	case	5:
//		Add_History_SwitchStat_Ter(ucSwitchStat,5);
//		stHisSwitchState0 = Get_History_SwitchStat_byMPNo(ucTerMPNo,0,5); 
//		stHisSwitchState1 = Get_History_SwitchStat_byMPNo(ucTerMPNo,1,5); 
//		stHisSwitchState2 = Get_History_SwitchStat_byMPNo(ucTerMPNo,2,5); 
//		stHisSwitchState3 = Get_History_SwitchStat_byMPNo(ucTerMPNo,3,5); 
//		break;
//	case	6:
//		Add_History_SwitchStat_Ter(ucSwitchStat,6);
//		stHisSwitchState0 = Get_History_SwitchStat_byMPNo(ucTerMPNo,0,6); 
//		stHisSwitchState1 = Get_History_SwitchStat_byMPNo(ucTerMPNo,1,6); 
//		stHisSwitchState2 = Get_History_SwitchStat_byMPNo(ucTerMPNo,2,6); 
//		stHisSwitchState3 = Get_History_SwitchStat_byMPNo(ucTerMPNo,3,6); 
//		break;
//	default:
//		ClearHisState(&(stHisSwitchState0));
//		ClearHisState(&(stHisSwitchState1));
//		ClearHisState(&(stHisSwitchState2));
//		ClearHisState(&(stHisSwitchState3));
//	}
//
//	if(	stHisSwitchState0.ucStateFlag != CONINVALIDVAL && stHisSwitchState1.ucStateFlag != CONINVALIDVAL &&
//		stHisSwitchState2.ucStateFlag != CONINVALIDVAL && stHisSwitchState3.ucStateFlag != CONINVALIDVAL)
//	{
//		ucTmp=Get_Switch_Change_Info();
////		ucTmp |= (1<<ucSwitchNo);
//		if(	stHisSwitchState0.ucStateFlag == stHisSwitchState1.ucStateFlag && stHisSwitchState0.ucStateFlag == CON_SWITCH_STAT_CLOSE &&
//			stHisSwitchState2.ucStateFlag == stHisSwitchState3.ucStateFlag && stHisSwitchState2.ucStateFlag == CON_SWITCH_STAT_OPEN)
//		{
//			Alarm_FillEnsureJudgeList((unsigned short)(CONALRCODE_SWITCH0OPEN+ucSwitchNo),ucTerMPNo);
//			ucTmp |= (1<<ucSwitchNo);		//cfh 10-04-26
//			Set_Switch_Change_Info(ucTmp);
//		}
//		if(	stHisSwitchState0.ucStateFlag == stHisSwitchState1.ucStateFlag && stHisSwitchState0.ucStateFlag == CON_SWITCH_STAT_OPEN &&
//			stHisSwitchState2.ucStateFlag == stHisSwitchState3.ucStateFlag && stHisSwitchState2.ucStateFlag == CON_SWITCH_STAT_CLOSE)
//		{
//			Alarm_FillEnsureJudgeList((unsigned short)(CONALRCODE_SWITCH0CLOSE+ucSwitchNo),ucTerMPNo);
//			ucTmp |= (1<<ucSwitchNo);		//cfh 10-04-26
//			Set_Switch_Change_Info(ucTmp);
//		}
//	}
//
//	return ucRet;
//}

////A相电流反极性
//unsigned char Alarm_ForeJudge_PolarityReverseA( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STHISSTATE		stHisState;
//	STHISSTATE		stHisState_Chk;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	
//	if(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_VolCirReve==CONALR_STAT_HAPPEN)
//		return ucRet;
//
//	stHisState=Get_History_PolReverseStat_byMPNo(ucTerMPNo,0,1);
//
//	stHisState_Chk = stHisState;
//	if(stHisState_Chk.ucStateFlag == CON_ALR_STAT_ON)
//	{
//		ucRet = Alarm_Chk_SPow_Err(ucTerMPNo);
//		if(ucRet == RET_SUCCESS)
//			stHisState_Chk.ucStateFlag = CON_ALR_STAT_OFF;
//	}
//	
//	ucRet = Alarm_ForeJudge_TerStatOn(	stHisState_Chk,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_PolReverseA),
//										&(stTerAlrJudgeCount.ucAlrCount_PolarityReverseA),
//										stTerAlrMaxCount.ucAlrCount_PolarityReverseA,
//										&(stTerAlrJudgeInfo.stAlrNewTime_PolarityReverseA),CONALRCODE_POLREVERSEA);
//	if(ucRet == RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_POLREVERSEA,ucTerMPNo);
//
//	return ucRet;
//}
////B相电流反极性
//unsigned char Alarm_ForeJudge_PolarityReverseB( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STHISSTATE		stHisState;
//	STHISSTATE		stHisState_Chk;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	
//	if(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_VolCirReve==CONALR_STAT_HAPPEN)
//		return ucRet;
//
//	stHisState=Get_History_PolReverseStat_byMPNo(ucTerMPNo,0,2);
//
//	stHisState_Chk = stHisState;
//	if(stHisState_Chk.ucStateFlag == CON_ALR_STAT_ON)
//	{
//		ucRet = Alarm_Chk_SPow_Err(ucTerMPNo);
//		if(ucRet == RET_SUCCESS)
//			stHisState_Chk.ucStateFlag = CON_ALR_STAT_OFF;
//	}
//
//	ucRet = Alarm_ForeJudge_TerStatOn(	stHisState_Chk,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_PolReverseB),
//										&(stTerAlrJudgeCount.ucAlrCount_PolarityReverseB),
//										stTerAlrMaxCount.ucAlrCount_PolarityReverseB,
//										&(stTerAlrJudgeInfo.stAlrNewTime_PolarityReverseB),CONALRCODE_POLREVERSEB);
//	if(ucRet == RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_POLREVERSEB,ucTerMPNo);
//
//	return ucRet;
//}
////C相电流反极性
//unsigned char Alarm_ForeJudge_PolarityReverseC( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STHISSTATE		stHisState;
//	STHISSTATE		stHisState_Chk;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	
//	if(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_VolCirReve==CONALR_STAT_HAPPEN)
//		return ucRet;
//
//	stHisState=Get_History_PolReverseStat_byMPNo(ucTerMPNo,0,3);
//
//	stHisState_Chk = stHisState;
//	if(stHisState_Chk.ucStateFlag == CON_ALR_STAT_ON)
//	{
//		ucRet = Alarm_Chk_SPow_Err(ucTerMPNo);
//		if(ucRet == RET_SUCCESS)
//			stHisState_Chk.ucStateFlag = CON_ALR_STAT_OFF;
//	}
//
//	ucRet = Alarm_ForeJudge_TerStatOn(	stHisState_Chk,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_PolReverseC),
//										&(stTerAlrJudgeCount.ucAlrCount_PolarityReverseC),
//										stTerAlrMaxCount.ucAlrCount_PolarityReverseC,
//										&(stTerAlrJudgeInfo.stAlrNewTime_PolarityReverseC),CONALRCODE_POLREVERSEC);
//	if(ucRet == RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_POLREVERSEC,ucTerMPNo);
//
//	return ucRet;
//}

//delete by cfh 10-04-04 一次侧判断去掉
/*
//A相1次侧CT短路
unsigned char Alarm_ForeJudge_ShortCircuitA1( )
{
	unsigned char	ucRet=RET_SUCCESS;
	STMPRATEVAL		stMPRateVal;
	STHISVAL		stHisVal_Cur;
	STHISSTATE		stHisState_Chk;
	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
	
	stMPRateVal		= RunPara_GetMPRateVal(ucTerMPNo);	
	stHisVal_Cur	= Get_History_I_byMPNo(ucTerMPNo,0,1);
	stHisState_Chk=Get_History_ShortCir1Stat_byMPNo(ucTerMPNo,0,1);


	//电流较小的时候开始判别CT告警
	if((stHisState_Chk.ucStateFlag == CON_ALR_STAT_ON) && (stHisVal_Cur.lVal < (unsigned long)stMPRateVal.usRateCur*3/100) )		//此处还需探讨，是否低了
		stHisState_Chk.ucStateFlag = CON_ALR_STAT_ON;
	else
		stHisState_Chk.ucStateFlag = CON_ALR_STAT_OFF;

	ucRet = Alarm_ForeJudge_TerStatOn(	stHisState_Chk,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_ShortCirA1),
										&(stTerAlrJudgeCount.ucAlrCount_ShortCircuitA1),
										stTerAlrMaxCount.ucAlrCount_ShortCircuitA1,
										&(stTerAlrJudgeInfo.stAlrNewTime_ShortCircuitA1),CONALRCODE_SHORTCIRA1);
	if(ucRet == RET_SUCCESS)
		Alarm_FillEnsureJudgeList(CONALRCODE_SHORTCIRA1,ucTerMPNo);

	return ucRet;
}
//B相1次侧CT短路
unsigned char Alarm_ForeJudge_ShortCircuitB1( )
{
	unsigned char	ucRet=RET_SUCCESS;
	STMPRATEVAL		stMPRateVal;
	STHISVAL		stHisVal_Cur;
	STHISSTATE		stHisState_Chk;
	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
	
	stMPRateVal		= RunPara_GetMPRateVal(ucTerMPNo);	
	stHisVal_Cur	= Get_History_I_byMPNo(ucTerMPNo,0,2);
	stHisState_Chk	=Get_History_ShortCir1Stat_byMPNo(ucTerMPNo,0,2);

		//电流较小的时候开始判别CT告警
	if((stHisState_Chk.ucStateFlag == CON_ALR_STAT_ON) && (stHisVal_Cur.lVal < (unsigned long)stMPRateVal.usRateCur*3/100) )		//此处还需探讨，是否低了
		stHisState_Chk.ucStateFlag = CON_ALR_STAT_ON;
	else
		stHisState_Chk.ucStateFlag = CON_ALR_STAT_OFF;

	ucRet = Alarm_ForeJudge_TerStatOn(	stHisState_Chk,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_ShortCirB1),
										&(stTerAlrJudgeCount.ucAlrCount_ShortCircuitB1),
										stTerAlrMaxCount.ucAlrCount_ShortCircuitB1,
										&(stTerAlrJudgeInfo.stAlrNewTime_ShortCircuitB1),CONALRCODE_SHORTCIRB1);
	if(ucRet == RET_SUCCESS)
		Alarm_FillEnsureJudgeList(CONALRCODE_SHORTCIRB1,ucTerMPNo);

	return ucRet;
}
//C相1次侧CT短路
unsigned char Alarm_ForeJudge_ShortCircuitC1( )
{
	unsigned char	ucRet=RET_SUCCESS;
	STMPRATEVAL		stMPRateVal;
	STHISVAL		stHisVal_Cur;
	STHISSTATE		stHisState_Chk;
	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
	
	stMPRateVal		= RunPara_GetMPRateVal(ucTerMPNo);	
	stHisVal_Cur	= Get_History_I_byMPNo(ucTerMPNo,0,3);
	stHisState_Chk	=Get_History_ShortCir1Stat_byMPNo(ucTerMPNo,0,3);

		//电流较小的时候开始判别CT告警
	if((stHisState_Chk.ucStateFlag == CON_ALR_STAT_ON) && (stHisVal_Cur.lVal < (unsigned long)stMPRateVal.usRateCur*3/100) )		//此处还需探讨，是否低了
		stHisState_Chk.ucStateFlag = CON_ALR_STAT_ON;
	else
		stHisState_Chk.ucStateFlag = CON_ALR_STAT_OFF;

	ucRet = Alarm_ForeJudge_TerStatOn(	stHisState_Chk,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_ShortCirC1),
										&(stTerAlrJudgeCount.ucAlrCount_ShortCircuitC1),
										stTerAlrMaxCount.ucAlrCount_ShortCircuitC1,
										&(stTerAlrJudgeInfo.stAlrNewTime_ShortCircuitC1),CONALRCODE_SHORTCIRC1);
	if(ucRet == RET_SUCCESS)
		Alarm_FillEnsureJudgeList(CONALRCODE_SHORTCIRC1,ucTerMPNo);

	return ucRet;
}
*/
//A相2次侧CT开路
//unsigned char Alarm_ForeJudge_OpenCircuitA2( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_Cur;
//	STHISSTATE		stHisState_Chk;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	
//	stMPRateVal		= RunPara_GetMPRateVal(ucTerMPNo);
//	stHisVal_Cur	= Get_History_I_byMPNo(ucTerMPNo,0,1);	
//	stHisState_Chk	=Get_History_OpenCir2Stat_byMPNo(ucTerMPNo,0,1);
//
//	//电流较小的时候开始判别CT告警
//	if((stHisState_Chk.ucStateFlag == CON_ALR_STAT_ON) && (stHisVal_Cur.lVal < (unsigned long)stMPRateVal.usRateCur*3/100) )	//此处还需探讨
//		stHisState_Chk.ucStateFlag = CON_ALR_STAT_ON;
//	else
//		stHisState_Chk.ucStateFlag = CON_ALR_STAT_OFF;
//
//	ucRet = Alarm_ForeJudge_TerStatOn(	stHisState_Chk,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_OpenCirA2),
//										&(stTerAlrJudgeCount.ucAlrCount_OpenCircuitA2),
//										stTerAlrMaxCount.ucAlrCount_OpenCircuitA2,
//										&(stTerAlrJudgeInfo.stAlrNewTime_OpenCircuitA2),CONALRCODE_OPENCIRA2);
//	if(ucRet == RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_OPENCIRA2,ucTerMPNo);
//
//	return ucRet;
//}
////B相2次侧CT开路
//unsigned char Alarm_ForeJudge_OpenCircuitB2( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_Cur;
//	STHISSTATE		stHisState_Chk;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	
//	stMPRateVal		= RunPara_GetMPRateVal(ucTerMPNo);
//	stHisVal_Cur	= Get_History_I_byMPNo(ucTerMPNo,0,2);
//	stHisState_Chk	=Get_History_OpenCir2Stat_byMPNo(ucTerMPNo,0,2);
//
//		//电流较小的时候开始判别CT告警
//	if((stHisState_Chk.ucStateFlag == CON_ALR_STAT_ON) && (stHisVal_Cur.lVal < (unsigned long)stMPRateVal.usRateCur*3/100) )	//此处还需探讨
//		stHisState_Chk.ucStateFlag = CON_ALR_STAT_ON;
//	else
//		stHisState_Chk.ucStateFlag = CON_ALR_STAT_OFF;
//
//	ucRet = Alarm_ForeJudge_TerStatOn(	stHisState_Chk,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_OpenCirB2),
//										&(stTerAlrJudgeCount.ucAlrCount_OpenCircuitB2),
//										stTerAlrMaxCount.ucAlrCount_OpenCircuitB2,
//										&(stTerAlrJudgeInfo.stAlrNewTime_OpenCircuitB2),CONALRCODE_OPENCIRB2);
//	if(ucRet == RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_OPENCIRB2,ucTerMPNo);
//
//	return ucRet;
//}
////C相2次侧CT开路
//unsigned char Alarm_ForeJudge_OpenCircuitC2( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_Cur;
//	STHISSTATE		stHisState_Chk;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	
//	stMPRateVal		= RunPara_GetMPRateVal(ucTerMPNo);
//	stHisVal_Cur	= Get_History_I_byMPNo(ucTerMPNo,0,3);
//	stHisState_Chk	=Get_History_OpenCir2Stat_byMPNo(ucTerMPNo,0,3);
//
//		//电流较小的时候开始判别CT告警
//	if((stHisState_Chk.ucStateFlag == CON_ALR_STAT_ON) && (stHisVal_Cur.lVal < (unsigned long)stMPRateVal.usRateCur*3/100) )	//此处还需探讨
//		stHisState_Chk.ucStateFlag = CON_ALR_STAT_ON;
//	else
//		stHisState_Chk.ucStateFlag = CON_ALR_STAT_OFF;
//
//	ucRet = Alarm_ForeJudge_TerStatOn(	stHisState_Chk,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_OpenCirC2),
//										&(stTerAlrJudgeCount.ucAlrCount_OpenCircuitC2),
//										stTerAlrMaxCount.ucAlrCount_OpenCircuitC2,
//										&(stTerAlrJudgeInfo.stAlrNewTime_OpenCircuitC2),CONALRCODE_OPENCIRC2);
//	if(ucRet == RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_OPENCIRC2,ucTerMPNo);
//
//	return ucRet;
//}
////A相2次侧CT短路
//unsigned char Alarm_ForeJudge_ShortCircuitA2( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_Cur;
//	STHISSTATE		stHisState_Chk;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	
//	stMPRateVal		= RunPara_GetMPRateVal(ucTerMPNo);
//	stHisVal_Cur	= Get_History_I_byMPNo(ucTerMPNo,0,1);
//	stHisState_Chk	=Get_History_ShortCir2Stat_byMPNo(ucTerMPNo,0,1);
//
//	if((stHisState_Chk.ucStateFlag == CON_ALR_STAT_ON) && (stHisVal_Cur.lVal < (unsigned long)stMPRateVal.usRateCur*80/100) )	//此处还需探讨，是否高了
//		stHisState_Chk.ucStateFlag = CON_ALR_STAT_ON;
//	else
//		stHisState_Chk.ucStateFlag = CON_ALR_STAT_OFF;
//
//	ucRet = Alarm_ForeJudge_TerStatOn(	stHisState_Chk,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_ShortCirA2),
//										&(stTerAlrJudgeCount.ucAlrCount_ShortCircuitA2),
//										stTerAlrMaxCount.ucAlrCount_ShortCircuitA2,
//										&(stTerAlrJudgeInfo.stAlrNewTime_ShortCircuitA2),CONALRCODE_SHORTCIRA2);
//	if(ucRet == RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_SHORTCIRA2,ucTerMPNo);
//
//	return ucRet;
//}
////B相2次侧CT短路
//unsigned char Alarm_ForeJudge_ShortCircuitB2( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_Cur;
//	STHISSTATE		stHisState_Chk;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	
//	stMPRateVal		= RunPara_GetMPRateVal(ucTerMPNo);
//	stHisVal_Cur	= Get_History_I_byMPNo(ucTerMPNo,0,2);
//	stHisState_Chk	=Get_History_ShortCir2Stat_byMPNo(ucTerMPNo,0,2);
//
//	if((stHisState_Chk.ucStateFlag == CON_ALR_STAT_ON) && (stHisVal_Cur.lVal < (unsigned long)stMPRateVal.usRateCur*80/100) )	//此处还需探讨，是否高了
//		stHisState_Chk.ucStateFlag = CON_ALR_STAT_ON;
//	else
//		stHisState_Chk.ucStateFlag = CON_ALR_STAT_OFF;
//
//	ucRet = Alarm_ForeJudge_TerStatOn(	stHisState_Chk,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_ShortCirB2),
//										&(stTerAlrJudgeCount.ucAlrCount_ShortCircuitB2),
//										stTerAlrMaxCount.ucAlrCount_ShortCircuitB2,
//										&(stTerAlrJudgeInfo.stAlrNewTime_ShortCircuitB2),CONALRCODE_SHORTCIRB2);
//	if(ucRet == RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_SHORTCIRB2,ucTerMPNo);
//
//	return ucRet;
//}
////C相2次侧CT短路
//unsigned char Alarm_ForeJudge_ShortCircuitC2( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_Cur;
//	STHISSTATE		stHisState_Chk;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	
//	stMPRateVal		= RunPara_GetMPRateVal(ucTerMPNo);
//	stHisVal_Cur	= Get_History_I_byMPNo(ucTerMPNo,0,3);
//	stHisState_Chk	=Get_History_ShortCir2Stat_byMPNo(ucTerMPNo,0,3);
//
//	if((stHisState_Chk.ucStateFlag == CON_ALR_STAT_ON) && (stHisVal_Cur.lVal < (unsigned long)stMPRateVal.usRateCur*80/100) )	//此处还需探讨，是否高了
//		stHisState_Chk.ucStateFlag = CON_ALR_STAT_ON;
//	else
//		stHisState_Chk.ucStateFlag = CON_ALR_STAT_OFF;
//
//	ucRet = Alarm_ForeJudge_TerStatOn(	stHisState_Chk,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_ShortCirC2),
//										&(stTerAlrJudgeCount.ucAlrCount_ShortCircuitC2),
//										stTerAlrMaxCount.ucAlrCount_ShortCircuitC2,
//										&(stTerAlrJudgeInfo.stAlrNewTime_ShortCircuitC2),CONALRCODE_SHORTCIRC2);
//	if(ucRet == RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_SHORTCIRC2,ucTerMPNo);
//
//	return ucRet;
//}
//A相断相
//unsigned char Alarm_ForeJudge_VaBreak_Ter( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_Vol;
//	STHISVAL		stHisVal_Cur;
//	STHISSTATE		stHisState_Chk;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	unsigned long	ul05perCur;
//	
//	ClearHisState(&(stHisState_Chk));
//	stHisVal_Vol= Get_History_V_byMPNo(ucTerMPNo,0,1);
//	stHisVal_Cur= Get_History_I_byMPNo(ucTerMPNo,0,1);
//	stMPLimit	= RunPara_GetMPLimit(ucTerMPNo);
//	stMPRateVal = RunPara_GetMPRateVal(ucTerMPNo);
//	ul05perCur=(unsigned long)(stMPRateVal.usRateCur * 5/10);
//	if(ul05perCur==0)
//		ul05perCur=1;
//
//	if(stHisVal_Vol.lVal != CONINVALIDVAL && stHisVal_Cur.lVal != CONINVALIDVAL)
//	{
//		stHisState_Chk.stDateTime  = stHisVal_Vol.stDateTime;
//		if(	stHisVal_Vol.lVal <= (unsigned long)(stMPLimit.usVolBreakLimit * CON_DOWNFLOAT_COEF/10) &&		//断相:电流<0.0.5%的额定电流
//			stHisVal_Cur.lVal <= ul05perCur)
//			stHisState_Chk.ucStateFlag = CON_ALR_STAT_ON;
//		else if(stHisVal_Vol.lVal >= (unsigned long)(stMPLimit.usVolBreakLimit * CON_UPFLOAT_COEF/10))
//			stHisState_Chk.ucStateFlag = CON_ALR_STAT_OFF;
//		else
//			stHisState_Chk.ucStateFlag = CON_ALR_STAT_MID;
//	}
//
//	ucRet = Alarm_ForeJudge_TerStatOn(	stHisState_Chk,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_VolaBreak),
//										&(stTerAlrJudgeCount.ucAlrCount_VolABreak),
//										stTerAlrMaxCount.ucAlrCount_VolABreak,
//										&(stTerAlrJudgeInfo.stAlrNewTime_VolABreak),CONALRCODE_VABREAK);
//	if(ucRet == RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_VABREAK,ucTerMPNo);
//
//	return ucRet;
//}
////B相断相
//unsigned char Alarm_ForeJudge_VbBreak_Ter( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_Vol,stHisVal_VolA,stHisVal_VolC;
//	STHISVAL		stHisVal_Cur;
//	STHISSTATE		stHisState_Chk;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	unsigned long	ul05perCur;
//	
//	ClearHisState(&(stHisState_Chk));
//	stMPLimit	= RunPara_GetMPLimit(ucTerMPNo);
//	stMPRateVal = RunPara_GetMPRateVal(ucTerMPNo);
//	ul05perCur=(unsigned long)(stMPRateVal.usRateCur * 5/10);
//	if(ul05perCur==0)
//		ul05perCur=1;
//	
//	if(stMPRateVal.ucMPLineType == CON_METERTYPE_3P4L)
//	{
//		stHisVal_Vol=Get_History_V_byMPNo(ucTerMPNo,0,2);
//		stHisVal_Cur=Get_History_I_byMPNo(ucTerMPNo,0,2);
//		if(stHisVal_Vol.lVal != CONINVALIDVAL && stHisVal_Cur.lVal != CONINVALIDVAL)
//		{
//			stHisState_Chk.stDateTime  = stHisVal_Vol.stDateTime;
//			if(	stHisVal_Vol.lVal <= (unsigned long)(stMPLimit.usVolBreakLimit * CON_DOWNFLOAT_COEF/10) &&		//断相:电流<0.5%的额定电流
//				stHisVal_Cur.lVal <= ul05perCur)
//				stHisState_Chk.ucStateFlag = CON_ALR_STAT_ON;
//			else if(stHisVal_Vol.lVal >= (unsigned long)(stMPLimit.usVolBreakLimit * CON_UPFLOAT_COEF/10))
//				stHisState_Chk.ucStateFlag = CON_ALR_STAT_OFF;
//			else
//				stHisState_Chk.ucStateFlag = CON_ALR_STAT_MID;
//		}
//	}
//
//	if(stMPRateVal.ucMPLineType == CON_METERTYPE_3P3L)
//	{
//		stHisVal_VolA=Get_History_V_byMPNo(ucTerMPNo,0,3);
//		stHisVal_VolC=Get_History_V_byMPNo(ucTerMPNo,0,3);
//		if(stHisVal_VolA.lVal!=CONINVALIDVAL && stHisVal_VolC.lVal != CONINVALIDVAL)
//		{
//			stHisState_Chk.stDateTime  = stHisVal_VolA.stDateTime;
//			if( stHisVal_VolA.lVal > ((CON_RATEVOL_3P3L/2-10)*10) && stHisVal_VolA.lVal < ((CON_RATEVOL_3P3L/2+10)*10) &&
//				stHisVal_VolC.lVal > ((CON_RATEVOL_3P3L/2-10)*10) && stHisVal_VolC.lVal < ((CON_RATEVOL_3P3L/2+10)*10))
//
//				stHisState_Chk.ucStateFlag = CON_ALR_STAT_ON;
//			else
//				stHisState_Chk.ucStateFlag = CON_ALR_STAT_OFF;
//		}
//	}
//
//	ucRet = Alarm_ForeJudge_TerStatOn(	stHisState_Chk,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_VolbBreak),
//										&(stTerAlrJudgeCount.ucAlrCount_VolBBreak),
//										stTerAlrMaxCount.ucAlrCount_VolBBreak,
//										&(stTerAlrJudgeInfo.stAlrNewTime_VolBBreak),CONALRCODE_VBBREAK);
//
//	if(ucRet == RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_VBBREAK,ucTerMPNo);
//	return ucRet;
//}
////C相断相
//unsigned char Alarm_ForeJudge_VcBreak_Ter()
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_Vol;
//	STHISVAL		stHisVal_Cur;
//	STHISSTATE		stHisState_Chk;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	unsigned long	ul05perCur;
//	
//	ClearHisState(&(stHisState_Chk));
//	stHisVal_Vol=Get_History_V_byMPNo(ucTerMPNo,0,3);
//	stHisVal_Cur=Get_History_I_byMPNo(ucTerMPNo,0,3);
//	stMPLimit	= RunPara_GetMPLimit(ucTerMPNo);
//	stMPRateVal = RunPara_GetMPRateVal(ucTerMPNo);
//	ul05perCur=(unsigned long)(stMPRateVal.usRateCur * 5/10);
//	if(ul05perCur==0)
//		ul05perCur=1;
//
//	if(stHisVal_Vol.lVal != CONINVALIDVAL && stHisVal_Cur.lVal != CONINVALIDVAL)
//	{
//		stHisState_Chk.stDateTime  = stHisVal_Vol.stDateTime;
//		if(	stHisVal_Vol.lVal <= (unsigned long)(stMPLimit.usVolBreakLimit * CON_DOWNFLOAT_COEF/10) &&		//断相:电流<0.5%的额定电流
//			stHisVal_Cur.lVal <= ul05perCur)
//			stHisState_Chk.ucStateFlag = CON_ALR_STAT_ON;
//		else if(stHisVal_Vol.lVal >= (unsigned long)(stMPLimit.usVolBreakLimit * CON_UPFLOAT_COEF/10))
//			stHisState_Chk.ucStateFlag = CON_ALR_STAT_OFF;
//		else
//			stHisState_Chk.ucStateFlag = CON_ALR_STAT_MID;
//	}
//
//	ucRet = Alarm_ForeJudge_TerStatOn(	stHisState_Chk,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_VolcBreak),
//										&(stTerAlrJudgeCount.ucAlrCount_VolCBreak),
//										stTerAlrMaxCount.ucAlrCount_VolCBreak,
//										&(stTerAlrJudgeInfo.stAlrNewTime_VolCBreak),CONALRCODE_VCBREAK);
//	if(ucRet == RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_VCBREAK,ucTerMPNo);
//
//	return ucRet;
//}
////A相缺相
//unsigned char Alarm_ForeJudge_VaLack_Ter( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
////	STMPLIMIT		stMPLimit;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_Vol;
//	STHISVAL		stHisVal_Cur;
//	STHISSTATE		stHisState_Chk;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	unsigned long	ul05perCur;
//	
//	ClearHisState(&(stHisState_Chk));
//	stHisVal_Vol=Get_History_V_byMPNo(ucTerMPNo,0,1);
//	stHisVal_Cur=Get_History_I_byMPNo(ucTerMPNo,0,1);
////	stMPLimit	= RunPara_GetMPLimit(ucTerMPNo);
//	stMPRateVal = RunPara_GetMPRateVal(ucTerMPNo);
//	ul05perCur=(unsigned long)(stMPRateVal.usRateCur * 5/10);
//	if(ul05perCur==0)
//		ul05perCur=1;
//
//	if(stHisVal_Vol.lVal != CONINVALIDVAL && stHisVal_Cur.lVal != CONINVALIDVAL)
//	{
//		stHisState_Chk.stDateTime  = stHisVal_Vol.stDateTime;
//		if(	stHisVal_Vol.lVal <= (unsigned long)(stMPRateVal.usRateVol *7* CON_DOWNFLOAT_COEF/1000) &&		//断相:电流<0.5%的额定电流
//			stHisVal_Cur.lVal >  ul05perCur)
//			stHisState_Chk.ucStateFlag = CON_ALR_STAT_ON;
//		else if(stHisVal_Vol.lVal >= (unsigned long)(stMPRateVal.usRateVol *7 * CON_UPFLOAT_COEF/1000))
//			stHisState_Chk.ucStateFlag = CON_ALR_STAT_OFF;
//		else
//			stHisState_Chk.ucStateFlag = CON_ALR_STAT_MID;
//	}
//
//	ucRet = Alarm_ForeJudge_TerStatOn(	stHisState_Chk,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_VolaLack),
//										&(stTerAlrJudgeCount.ucAlrCount_VolALack),
//										stTerAlrMaxCount.ucAlrCount_VolALack,
//										&(stTerAlrJudgeInfo.stAlrNewTime_VolALack),CONALRCODE_VALACK);
//	if(ucRet == RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_VALACK,ucTerMPNo);
//
//	return ucRet;
//}
////B相缺相
//unsigned char Alarm_ForeJudge_VbLack_Ter( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_Vol;
//	STHISVAL		stHisVal_Cur;
//	STHISSTATE		stHisState_Chk;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	unsigned long	ul05perCur;
//	
//	ClearHisState(&(stHisState_Chk));
//	stHisVal_Vol=Get_History_V_byMPNo(ucTerMPNo,0,2);
//	stHisVal_Cur=Get_History_I_byMPNo(ucTerMPNo,0,2);
////	stMPLimit	= RunPara_GetMPLimit(ucTerMPNo);
//	stMPRateVal = RunPara_GetMPRateVal(ucTerMPNo);
//	ul05perCur=(unsigned long)(stMPRateVal.usRateCur * 5/10);
//	if(ul05perCur==0)
//		ul05perCur=1;
//
//	if(stMPRateVal.ucMPLineType == CON_METERTYPE_3P4L)
//	{
//		if(stHisVal_Vol.lVal != CONINVALIDVAL && stHisVal_Cur.lVal != CONINVALIDVAL)
//		{
//			stHisState_Chk.stDateTime  = stHisVal_Vol.stDateTime;
//			if(	stHisVal_Vol.lVal <= (unsigned long)(stMPRateVal.usRateVol *7 * CON_DOWNFLOAT_COEF/1000) &&		//断相:电流<0.5%的额定电流
//				stHisVal_Cur.lVal >  ul05perCur)
//				stHisState_Chk.ucStateFlag = CON_ALR_STAT_ON;
//			else if(stHisVal_Vol.lVal >= (unsigned long)(stMPRateVal.usRateVol *7 * CON_UPFLOAT_COEF/1000))
//				stHisState_Chk.ucStateFlag = CON_ALR_STAT_OFF;
//			else
//				stHisState_Chk.ucStateFlag = CON_ALR_STAT_MID;
//		}
//
//		ucRet = Alarm_ForeJudge_TerStatOn(	stHisState_Chk,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_VolbLack),
//											&(stTerAlrJudgeCount.ucAlrCount_VolBLack),
//											stTerAlrMaxCount.ucAlrCount_VolBLack,
//											&(stTerAlrJudgeInfo.stAlrNewTime_VolBLack),CONALRCODE_VBLACK);
//		if(ucRet == RET_SUCCESS)
//			Alarm_FillEnsureJudgeList(CONALRCODE_VBLACK,ucTerMPNo);
//	}
//	return ucRet;
//}
////C相缺相
//unsigned char Alarm_ForeJudge_VcLack_Ter( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_Vol;
//	STHISVAL		stHisVal_Cur;
//	STHISSTATE		stHisState_Chk;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	unsigned long	ul05perCur;
//	
//	ClearHisState(&(stHisState_Chk));
//	stHisVal_Vol=Get_History_V_byMPNo(ucTerMPNo,0,3);
//	stHisVal_Cur=Get_History_I_byMPNo(ucTerMPNo,0,3);
////	stMPLimit = RunPara_GetMPLimit(ucTerMPNo);
//	stMPRateVal = RunPara_GetMPRateVal(ucTerMPNo);
//	ul05perCur=(unsigned long)(stMPRateVal.usRateCur * 5/10);
//	if(ul05perCur==0)
//		ul05perCur=1;
//
//	if(stHisVal_Vol.lVal != CONINVALIDVAL && stHisVal_Cur.lVal != CONINVALIDVAL)
//	{
//		stHisState_Chk.stDateTime  = stHisVal_Vol.stDateTime;
//		if(	stHisVal_Vol.lVal <= (unsigned long)(stMPRateVal.usRateVol *7 * CON_DOWNFLOAT_COEF/1000) &&		//断相:电流<0.5%的额定电流
//			stHisVal_Cur.lVal >  ul05perCur)
//			stHisState_Chk.ucStateFlag = CON_ALR_STAT_ON;
//		else if(stHisVal_Vol.lVal >= (unsigned long)(stMPRateVal.usRateVol *7 * CON_UPFLOAT_COEF/1000))
//			stHisState_Chk.ucStateFlag = CON_ALR_STAT_OFF;
//		else
//			stHisState_Chk.ucStateFlag = CON_ALR_STAT_MID;
//	}
//
//	ucRet = Alarm_ForeJudge_TerStatOn(	stHisState_Chk,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_VolcLack),
//										&(stTerAlrJudgeCount.ucAlrCount_VolCLack),
//										stTerAlrMaxCount.ucAlrCount_VolCLack,
//										&(stTerAlrJudgeInfo.stAlrNewTime_VolCLack),CONALRCODE_VCLACK);
//	if(ucRet == RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_VCLACK,ucTerMPNo);
//
//	return ucRet;
//}
////电压回路逆相序
//unsigned char Alarm_ForeJudge_VolCirReverse( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STHISSTATE		stHisState;
//	STHISSTATE		stHisState_Chk;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	STHISVAL		stHisVal_Va,stHisVal_Vc;
//	
//	stHisState=Get_History_VolCirReveStat_byMPNo(ucTerMPNo,0);
//	stHisVal_Va=Get_History_V_byMPNo(ucTerMPNo,0,1);
//	stHisVal_Vc=Get_History_V_byMPNo(ucTerMPNo,0,3);
//
//	stHisState_Chk = stHisState;
//	if(stHisState_Chk.ucStateFlag == CON_ALR_STAT_ON)
//	{
//		ucRet = Alarm_Chk_SPow_Err(ucTerMPNo);
//		if(ucRet == RET_SUCCESS)
//			stHisState_Chk.ucStateFlag = CON_ALR_STAT_OFF;
//	}
//
//	if(stHisVal_Va.lVal!=CONINVALIDVAL && stHisVal_Vc.lVal!=CONINVALIDVAL)
//	{
//		if(stHisVal_Va.lVal>300 && stHisVal_Vc.lVal>300)		//判断电压要大于30V
//		{}
//		else
//		{
//			stHisState_Chk.ucStateFlag = CON_ALR_STAT_OFF;
//		}
//	}
//	else
//	{
//		stHisState_Chk.ucStateFlag = CON_ALR_STAT_OFF;
//	}
//
//	ucRet = Alarm_ForeJudge_TerStatOn(	stHisState_Chk,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_VolCirReve),
//										&(stTerAlrJudgeCount.ucAlrCount_VolCirReverse),
//										stTerAlrMaxCount.ucAlrCount_VolCirReverse,
//										&(stTerAlrJudgeInfo.stAlrNewTime_VolCirReverse),CONALRCODE_VOLCIRREVERSE);
//	if(ucRet == RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_VOLCIRREVERSE,ucTerMPNo);
//
//	return ucRet;
//}
////电流不平衡
//unsigned char Alarm_ForeJudge_IUnBalanceOverUp_Ter( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_CurA,stHisVal_CurB,stHisVal_CurC;
//	STHISVAL		stHisVal_Cur;
//	STHISSTATE		stHisState_Chk;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	
//	ClearHisState(&(stHisState_Chk));
//	stHisVal_CurA=Get_History_I_byMPNo(ucTerMPNo,0,1);
//	stHisVal_CurB=Get_History_I_byMPNo(ucTerMPNo,0,2);
//	stHisVal_CurC=Get_History_I_byMPNo(ucTerMPNo,0,3);
//	stMPLimit	= RunPara_GetMPLimit(ucTerMPNo);
//	stMPRateVal = RunPara_GetMPRateVal(ucTerMPNo);
//
//	if(stHisVal_CurA.lVal != CONINVALIDVAL && stHisVal_CurB.lVal != CONINVALIDVAL && stHisVal_CurC.lVal!=CONINVALIDVAL)
//	{
//		stHisVal_Cur = GetUnBalanceVal(stHisVal_CurA,stHisVal_CurB,stHisVal_CurC,stMPRateVal.ucMPLineType);
//
//		stHisState_Chk.stDateTime = stHisVal_Cur.stDateTime;
//		if(	stHisVal_Cur.lVal >= (unsigned long)(stMPLimit.usCurUnBalance * CON_UPFLOAT_COEF/100) &&
//			(stHisVal_CurA.lVal +stHisVal_CurB.lVal +stHisVal_CurC.lVal) > (unsigned long)(stMPRateVal.usRateCur *5/10))
//			stHisState_Chk.ucStateFlag = CON_ALR_STAT_ON;
//		else if(stHisVal_Cur.lVal <(unsigned long)(stMPLimit.usCurUnBalance * stMPLimit.ucCurUnBalanceBack /1000))
//			stHisState_Chk.ucStateFlag = CON_ALR_STAT_OFF;
//	}
//	
//	ucRet = Alarm_ForeJudge_TerStatOn(	stHisState_Chk,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_IUnbalance),
//										&(stTerAlrJudgeCount.ucAlrCount_IUnBalance),
//										stTerAlrMaxCount.ucAlrCount_IUnBalance,
//										&(stTerAlrJudgeInfo.stAlrNewTime_IUnBalance),CONALRCODE_IUNBLANCE);
//
//	if((ucRet==RET_SUCCESS) && (stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_UUnbalance == CONALR_STAT_HAPPEN_NOT))		//等待电压不平衡恢复在报电流不平衡
//		Alarm_FillEnsureJudgeList(CONALRCODE_IUNBLANCE,ucTerMPNo);
//	return ucRet;
//}
////电压不平衡
//unsigned char Alarm_ForeJudge_UUnBalanceOverUp_Ter( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STMPRATEVAL		stMPRateVal;
//	STHISVAL		stHisVal_VolA,stHisVal_VolB,stHisVal_VolC;
//	STHISVAL		stHisVal_Vol;
//	STHISSTATE		stHisState_Chk;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	
//	ClearHisState(&(stHisState_Chk));	
//	stHisVal_VolA =Get_History_V_byMPNo(ucTerMPNo,0,1);
//	stHisVal_VolB =Get_History_V_byMPNo(ucTerMPNo,0,2);
//	stHisVal_VolC =Get_History_V_byMPNo(ucTerMPNo,0,3);
//	stMPLimit	= RunPara_GetMPLimit(ucTerMPNo);
//	stMPRateVal = RunPara_GetMPRateVal(ucTerMPNo);
//
//	if(stHisVal_VolA.lVal != CONINVALIDVAL && stHisVal_VolB.lVal != CONINVALIDVAL && stHisVal_VolC.lVal!=CONINVALIDVAL)
//	{
//		stHisVal_Vol = GetUnBalanceVal(stHisVal_VolA,stHisVal_VolB,stHisVal_VolC,stMPRateVal.ucMPLineType);
//
//		stHisState_Chk.stDateTime = stHisVal_Vol.stDateTime;
//		if(	stHisVal_Vol.lVal >= (unsigned long)(stMPLimit.usVolUnBalance * CON_UPFLOAT_COEF/100) &&
//			(stHisVal_VolA.lVal +stHisVal_VolB.lVal +stHisVal_VolC.lVal) > (unsigned long)(stMPRateVal.usRateVol*5/1000))	//0.5% //usRateVol 0.1v
//			stHisState_Chk.ucStateFlag = CON_ALR_STAT_ON;
//		else if(stHisVal_Vol.lVal <(unsigned long)(stMPLimit.usVolUnBalance * stMPLimit.ucVolUnBalanceBack /1000))
//			stHisState_Chk.ucStateFlag = CON_ALR_STAT_OFF;
//	}
//	
//	ucRet = Alarm_ForeJudge_TerStatOn(	stHisState_Chk,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_UUnbalance),
//										&(stTerAlrJudgeCount.ucAlrCount_UUnBalance),
//										stTerAlrMaxCount.ucAlrCount_UUnBalance,
//										&(stTerAlrJudgeInfo.stAlrNewTime_UUnBalance),CONALRCODE_UUNBLANCE);
//	
//	if(ucRet==RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_UUNBLANCE,ucTerMPNo);
//	return ucRet;
//}
////A相电压越下下限
//unsigned char Alarm_ForeJudge_VolAErrDown( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Vol;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	
//	stHisVal_Vol=Get_History_V_byMPNo(ucTerMPNo,0,1);
//	stHisVal_Vol.lVal = stHisVal_Vol.lVal/10;		//取整数
//	stMPLimit = RunPara_GetMPLimit(ucTerMPNo);
//
//	ucRet = Alarm_ForeJudge_TerValOverDown(	stHisVal_Vol,stMPLimit.usVolErrDownLimit,stMPLimit.ucVolErrDownBack,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_VolaDownOver),
//											&(stTerAlrJudgeCount.ucAlrCount_VolAErrDown),
//											stTerAlrMaxCount.ucAlrCount_VolAErrDown,
//											&(stTerAlrJudgeInfo.stAlrNewTime_VolAErrDown),CONALRCODE_VAOVERDOWN);
//	if(ucRet == RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_VAOVERDOWN,ucTerMPNo);
//
//	return ucRet;
//}
////B相电压越下下限
//unsigned char Alarm_ForeJudge_VolBErrDown( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Vol;
//	STMPRATEVAL 	stMPRateVal;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	
//	stHisVal_Vol=Get_History_V_byMPNo(ucTerMPNo,0,2);
//	stHisVal_Vol.lVal = stHisVal_Vol.lVal/10;		//取整数
//	stMPLimit = RunPara_GetMPLimit(ucTerMPNo);
//	stMPRateVal = RunPara_GetMPRateVal(ucTerMPNo);
//
//	if(stMPRateVal.ucMPLineType == CON_METERTYPE_3P4L)
//	{
//		ucRet = Alarm_ForeJudge_TerValOverDown(	stHisVal_Vol,stMPLimit.usVolErrDownLimit,stMPLimit.ucVolErrDownBack,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_VolbDownOver),
//												&(stTerAlrJudgeCount.ucAlrCount_VolBErrDown),
//												stTerAlrMaxCount.ucAlrCount_VolBErrDown,
//												&(stTerAlrJudgeInfo.stAlrNewTime_VolBErrDown),CONALRCODE_VBOVERDOWN);
//		if(ucRet == RET_SUCCESS)
//			Alarm_FillEnsureJudgeList(CONALRCODE_VBOVERDOWN,ucTerMPNo);
//	}
//	return ucRet;
//}
////C相电压越下下限
//unsigned char Alarm_ForeJudge_VolCErrDown( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Vol;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	
//	stHisVal_Vol=Get_History_V_byMPNo(ucTerMPNo,0,3);
//	stHisVal_Vol.lVal = stHisVal_Vol.lVal/10;		//取整数
//	stMPLimit = RunPara_GetMPLimit(ucTerMPNo);
//
//	ucRet = Alarm_ForeJudge_TerValOverDown(	stHisVal_Vol,stMPLimit.usVolErrDownLimit,stMPLimit.ucVolErrDownBack,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_VolcDownOver),
//											&(stTerAlrJudgeCount.ucAlrCount_VolCErrDown),
//											stTerAlrMaxCount.ucAlrCount_VolCErrDown,
//											&(stTerAlrJudgeInfo.stAlrNewTime_VolCErrDown),CONALRCODE_VCOVERDOWN);
//	if(ucRet == RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_VCOVERDOWN,ucTerMPNo);
//
//	return ucRet;
//}
////A相电压越上上限
//unsigned char Alarm_ForeJudge_VolAErrUp( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
////	unsigned long ulBackLimit=0;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Vol;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//
//	stHisVal_Vol=Get_History_V_byMPNo(ucTerMPNo,0,1);
//	stHisVal_Vol.lVal = stHisVal_Vol.lVal/10;		//取整数
//	stMPLimit = RunPara_GetMPLimit(ucTerMPNo);
//
//	ucRet = Alarm_ForeJudge_TerValOverUp(	stHisVal_Vol,stMPLimit.usVolErrUpLimit,stMPLimit.ucVolErrUpBack,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_VolaUpOver),
//											&(stTerAlrJudgeCount.ucAlrCount_VolAErrUp),
//											stTerAlrMaxCount.ucAlrCount_VolAErrUp,
//											&(stTerAlrJudgeInfo.stAlrNewTime_VolAErrUp),CONALRCODE_VAOVERUP);
//	if(ucRet == RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_VAOVERUP,ucTerMPNo);
//
//	return ucRet;
//}
////B相电压越上上限
//unsigned char Alarm_ForeJudge_VolBErrUp( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Vol;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	
//	stHisVal_Vol=Get_History_V_byMPNo(ucTerMPNo,0,2);
//	stHisVal_Vol.lVal = stHisVal_Vol.lVal/10;		//取整数
//	stMPLimit = RunPara_GetMPLimit(ucTerMPNo);
//
//	ucRet = Alarm_ForeJudge_TerValOverUp(	stHisVal_Vol,stMPLimit.usVolErrUpLimit,stMPLimit.ucVolErrUpBack,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_VolbUpOver),
//											&(stTerAlrJudgeCount.ucAlrCount_VolBErrUp),
//											stTerAlrMaxCount.ucAlrCount_VolBErrUp,
//											&(stTerAlrJudgeInfo.stAlrNewTime_VolBErrUp),CONALRCODE_VBOVERUP);
//	if(ucRet == RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_VBOVERUP,ucTerMPNo);
//
//	return ucRet;
//}
////C相电压越上上限
//unsigned char Alarm_ForeJudge_VolCErrUp( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Vol;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	
//	stHisVal_Vol=Get_History_V_byMPNo(ucTerMPNo,0,3);
//	stHisVal_Vol.lVal = stHisVal_Vol.lVal/10;		//取整数
//	stMPLimit = RunPara_GetMPLimit(ucTerMPNo);
//
//	ucRet = Alarm_ForeJudge_TerValOverUp(	stHisVal_Vol,stMPLimit.usVolErrUpLimit,stMPLimit.ucVolErrUpBack,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_VolcUpOver),
//											&(stTerAlrJudgeCount.ucAlrCount_VolCErrUp),
//											stTerAlrMaxCount.ucAlrCount_VolCErrUp,
//											&(stTerAlrJudgeInfo.stAlrNewTime_VolCErrUp),CONALRCODE_VCOVERUP);
//	if(ucRet == RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_VCOVERUP,ucTerMPNo);
//
//	return ucRet;
//}
////A相电流越上上限
//unsigned char Alarm_ForeJudge_CurAErrUp( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Cur;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//
//	stHisVal_Cur=Get_History_I_byMPNo(ucTerMPNo,0,1);
//	stHisVal_Cur.lVal /= 10;									//取2 位小数
//	stMPLimit = RunPara_GetMPLimit(ucTerMPNo);
//	ucRet = Alarm_ForeJudge_TerValOverUp(	stHisVal_Cur,stMPLimit.usCurErrUpLimit,stMPLimit.ucCurErrUpBack,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_IaErrOverLoad),
//											&(stTerAlrJudgeCount.ucAlrCount_CurAErrUp),
//											stTerAlrMaxCount.ucAlrCount_CurAErrUp,
//											&(stTerAlrJudgeInfo.stAlrNewTime_CurAErrUp),CONALRCODE_IAERROVERUP);
//	if(ucRet == RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_IAERROVERUP,ucTerMPNo);
//
//	return ucRet;
//}
////B相电流越上上限
//unsigned char Alarm_ForeJudge_CurBErrUp( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Cur;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	
//	stHisVal_Cur=Get_History_I_byMPNo(ucTerMPNo,0,2);
//	stHisVal_Cur.lVal /= 10;
//	stMPLimit = RunPara_GetMPLimit(ucTerMPNo);
//
//	ucRet = Alarm_ForeJudge_TerValOverUp(	stHisVal_Cur,stMPLimit.usCurErrUpLimit,stMPLimit.ucCurErrUpBack,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_IbErrOverLoad),
//											&(stTerAlrJudgeCount.ucAlrCount_CurBErrUp),
//											stTerAlrMaxCount.ucAlrCount_CurBErrUp,
//											&(stTerAlrJudgeInfo.stAlrNewTime_CurBErrUp),CONALRCODE_IBERROVERUP);
//	if(ucRet == RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_IBERROVERUP,ucTerMPNo);
//
//	return ucRet;
//}
////C相电流越上上限
//unsigned char Alarm_ForeJudge_CurCErrUp( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Cur;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	
//	stHisVal_Cur=Get_History_I_byMPNo(ucTerMPNo,0,3);
//	stHisVal_Cur.lVal /= 10;
//	stMPLimit = RunPara_GetMPLimit(ucTerMPNo);
//
//	ucRet = Alarm_ForeJudge_TerValOverUp(	stHisVal_Cur,stMPLimit.usCurErrUpLimit,stMPLimit.ucCurErrUpBack,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_IcErrOverLoad),
//											&(stTerAlrJudgeCount.ucAlrCount_CurCErrUp),
//											stTerAlrMaxCount.ucAlrCount_CurCErrUp,
//											&(stTerAlrJudgeInfo.stAlrNewTime_CurCErrUp),CONALRCODE_ICERROVERUP);
//	if(ucRet == RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_ICERROVERUP,ucTerMPNo);
//
//	return ucRet;
//}
////A相电流越上限
//unsigned char Alarm_ForeJudge_CurANorUp( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Cur;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	
//	stHisVal_Cur=Get_History_I_byMPNo(ucTerMPNo,0,1);
//	stHisVal_Cur.lVal /= 10;
//
//	if(stHisVal_Cur.lVal!=CONINVALIDVAL)
//	{
//		stMPLimit = RunPara_GetMPLimit(ucTerMPNo);
//		if(stHisVal_Cur.lVal<=(unsigned long)(stMPLimit.usCurErrUpLimit*CON_UPFLOAT_COEF/100))
//		{
//			ucRet = Alarm_ForeJudge_TerValOverUp(	stHisVal_Cur,stMPLimit.usCurUpLimit,stMPLimit.ucCurUpBack,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_IaOverLoad),
//													&(stTerAlrJudgeCount.ucAlrCount_CurANorUp),
//													stTerAlrMaxCount.ucAlrCount_CurANorUp,
//													&(stTerAlrJudgeInfo.stAlrNewTime_CurANorUp),CONALRCODE_IAOVERUP);
//			if(ucRet == RET_SUCCESS)
//				Alarm_FillEnsureJudgeList(CONALRCODE_IAOVERUP,ucTerMPNo);
//		}
//	}
//
//	return ucRet;
//}
////B相电流越上限
//unsigned char Alarm_ForeJudge_CurBNorUp( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Cur;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	
//	stHisVal_Cur=Get_History_I_byMPNo(ucTerMPNo,0,2);
//	stHisVal_Cur.lVal /= 10;
//
//	if(stHisVal_Cur.lVal!=CONINVALIDVAL)
//	{
//		stMPLimit = RunPara_GetMPLimit(ucTerMPNo);
//		if(stHisVal_Cur.lVal<=(unsigned long)(stMPLimit.usCurErrUpLimit*CON_UPFLOAT_COEF/100))
//		{
//			ucRet = Alarm_ForeJudge_TerValOverUp(	stHisVal_Cur,stMPLimit.usCurUpLimit,stMPLimit.ucCurUpBack,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_IbOverLoad),
//													&(stTerAlrJudgeCount.ucAlrCount_CurBNorUp),
//													stTerAlrMaxCount.ucAlrCount_CurBNorUp,
//													&(stTerAlrJudgeInfo.stAlrNewTime_CurBNorUp),CONALRCODE_IBOVERUP);
//			if(ucRet == RET_SUCCESS)
//				Alarm_FillEnsureJudgeList(CONALRCODE_IBOVERUP,ucTerMPNo);
//		}
//	}
//	return ucRet;
//}
////C相电流越上限
//unsigned char Alarm_ForeJudge_CurCNorUp( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_Cur;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	
//	stHisVal_Cur=Get_History_I_byMPNo(ucTerMPNo,0,3);
//	stHisVal_Cur.lVal /= 10;
//
//	if(stHisVal_Cur.lVal!=CONINVALIDVAL)
//	{
//		stMPLimit = RunPara_GetMPLimit(ucTerMPNo);
//		if(stHisVal_Cur.lVal<=(unsigned long)(stMPLimit.usCurErrUpLimit*CON_UPFLOAT_COEF/100))
//		{
//			ucRet = Alarm_ForeJudge_TerValOverUp(	stHisVal_Cur,stMPLimit.usCurUpLimit,stMPLimit.ucCurUpBack,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_IcOverLoad),
//													&(stTerAlrJudgeCount.ucAlrCount_CurCNorUp),
//													stTerAlrMaxCount.ucAlrCount_CurCNorUp,
//													&(stTerAlrJudgeInfo.stAlrNewTime_CurCNorUp),CONALRCODE_ICOVERUP);
//			if(ucRet == RET_SUCCESS)
//				Alarm_FillEnsureJudgeList(CONALRCODE_ICOVERUP,ucTerMPNo);
//		}
//	}
//
//	return ucRet;
//}
////视在功率越上限
//unsigned char Alarm_ForeJudge_SPowNorUp_Ter( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_P;
//	STHISVAL		stHisVal_Q;
//	STHISVAL		stHisVal_Cur;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	
//	stHisVal_P=Get_History_InstantPow_byMPNo(ucTerMPNo,0,0);
//	stHisVal_Q=Get_History_InstantVar_byMPNo(ucTerMPNo,0,0);
//	stHisVal_Cur=stHisVal_P;
//
//	if(stHisVal_P.lVal!=CONINVALIDVAL && stHisVal_Q.lVal!=CONINVALIDVAL)
//		stHisVal_Cur.lVal = GetSPow(stHisVal_P.lVal,stHisVal_Q.lVal);	//4位小数
//	else
//		ClearHisVal(&stHisVal_Cur);
//
//	if(stHisVal_Cur.lVal!=CONINVALIDVAL)
//	{
//		stMPLimit = RunPara_GetMPLimit(ucTerMPNo);
//		if(stHisVal_Cur.lVal<=(unsigned long)(stMPLimit.ulPowErrUpLimit*CON_UPFLOAT_COEF/100))
//		{
//			ucRet = Alarm_ForeJudge_TerValOverUp(	stHisVal_Cur,stMPLimit.ulPowUpLimit,stMPLimit.ucPowUpBack,&(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_SPowOverUp),
//													&(stTerAlrJudgeCount.ucAlrCount_SPowNorUp),
//													stTerAlrMaxCount.ucAlrCount_SPowNorUp,
//													&(stTerAlrJudgeInfo.stAlrNewTime_SPowNorUp),CONALRCODE_SPOWNORUP);
//			
//			if(ucRet==RET_SUCCESS)
//				Alarm_FillEnsureJudgeList(CONALRCODE_SPOWNORUP,ucTerMPNo);
//		}
//	}
//	return ucRet;
//}
////视在功率越上上限
//unsigned char Alarm_ForeJudge_SPowErrUp_Ter( )
//{
//	unsigned char	ucRet=RET_SUCCESS;
//	STMPLIMIT		stMPLimit;
//	STHISVAL		stHisVal_P;
//	STHISVAL		stHisVal_Q;
//	STHISVAL		stHisVal_Cur;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//	
//	stHisVal_P=Get_History_InstantPow_byMPNo(ucTerMPNo,0,0);
//	stHisVal_Q=Get_History_InstantVar_byMPNo(ucTerMPNo,0,0);
//	stHisVal_Cur=stHisVal_P;
//
//	if(stHisVal_P.lVal!=CONINVALIDVAL && stHisVal_Q.lVal!=CONINVALIDVAL)
//		stHisVal_Cur.lVal = GetSPow(stHisVal_P.lVal,stHisVal_Q.lVal);	//4位小数
//	else
//		ClearHisVal(&stHisVal_Cur);
//
//	stMPLimit = RunPara_GetMPLimit(ucTerMPNo);
//
//	ucRet = Alarm_ForeJudge_TerValOverUp(	stHisVal_Cur,stMPLimit.ulPowErrUpLimit,stMPLimit.ucPowErrUpBack, &(stAlarmStatus[ucTerMPNo].ucAlrStat_Sys_SPowErrUp),
//											&(stTerAlrJudgeCount.ucAlrCount_SPowErrUp),
//											stTerAlrMaxCount.ucAlrCount_SPowErrUp,
//											&(stTerAlrJudgeInfo.stAlrNewTime_SPowErrUp),CONALRCODE_SPOWERRUP);
//	
//	if(ucRet==RET_SUCCESS)
//		Alarm_FillEnsureJudgeList(CONALRCODE_SPOWERRUP,ucTerMPNo);
//	return ucRet;
//}
//unsigned char Alarm_ForeJudge_TerValOverUp(	STHISVAL	stHisVal,	unsigned long ulLimit,	
//											unsigned short usBack,		unsigned char *pucAlrStat,	
//											unsigned char *pucAlrSumCount,unsigned char ucMaxAlrCount,
//											STDATETIME *pstAlrNewTime,    unsigned short  usAlrCode)
//{
//	unsigned char ucRet = RET_ERROR;
//	unsigned long ulVal = stHisVal.lVal;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//
//	if(ulVal != CONINVALIDVAL)
//	{
//		if(Is_DateTime_Equel(&(stHisVal.stDateTime),pstAlrNewTime)== RET_ERROR)
//		{
//			(*pstAlrNewTime) = stHisVal.stDateTime;
//			
//			if( (*pucAlrStat) == CONALR_STAT_HAPPEN_NOT )
//			{
//				if( ulVal > ulLimit * CON_UPFLOAT_COEF/100	) 
//				{
//					(*pucAlrSumCount)++;
//					if((*pucAlrSumCount) >= ucMaxAlrCount)
//					{
//						(*pucAlrStat)		=CONALR_STAT_HAPPEN;
//						(*pucAlrSumCount)	=0;
//						if(Alarm_CheckEnsureJudgeList(usAlrCode,ucTerMPNo)!=RET_SUCCESS)
//							ucRet=RET_SUCCESS;
//					}
//				}else
//					(*pucAlrSumCount)	=0;
//			}
//			else if( (*pucAlrStat) == CONALR_STAT_HAPPEN)
//			{
//				if( ulVal< ulLimit * usBack/1000)
//				{
//					(*pucAlrSumCount)++;
//					if((*pucAlrSumCount) >= ucMaxAlrCount)
//					{
//						(*pucAlrStat)		=CONALR_STAT_HAPPEN_NOT;
//						(*pucAlrSumCount)	=0;
//						if(Alarm_CheckEnsureJudgeList(usAlrCode,ucTerMPNo)!=RET_SUCCESS)
//							ucRet=RET_SUCCESS;
//					}
//				}else
//					(*pucAlrSumCount)	=0;
//			}else
//			{
//				(*pucAlrSumCount)	=0;
//				(*pucAlrStat) = CONALR_STAT_HAPPEN_NOT;
//			}
//		}
//	}
//
//	return ucRet;
//}

//unsigned char Alarm_ForeJudge_TerValOverDown(	STHISVAL	stHisVal,	unsigned long ulLimit,	unsigned short usBack,unsigned char *pucAlrStat,	
//												unsigned char *pucAlrSumCount,unsigned char ucMaxAlrCount,
//												STDATETIME *pstAlrNewTime,    unsigned short  usAlrCode)
//{
//	unsigned char ucRet = RET_ERROR;
//	unsigned long ulVal = stHisVal.lVal;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//
//	if(ulVal != CONINVALIDVAL)
//	{
//		if(Is_DateTime_Equel(&(stHisVal.stDateTime),pstAlrNewTime)== RET_ERROR)
//		{
//			(*pstAlrNewTime) = stHisVal.stDateTime;
//			
//			if( (*pucAlrStat) == CONALR_STAT_HAPPEN_NOT )
//			{
//				if( ulVal < ulLimit * CON_DOWNFLOAT_COEF/100	) 
//				{
//					(*pucAlrSumCount)++;
//					if((*pucAlrSumCount) >= ucMaxAlrCount)
//					{
//						(*pucAlrStat)		=CONALR_STAT_HAPPEN;
//						(*pucAlrSumCount)	=0;
//						if(Alarm_CheckEnsureJudgeList(usAlrCode,ucTerMPNo)!=RET_SUCCESS)
//							ucRet=RET_SUCCESS;
//					}
//				}else
//					(*pucAlrSumCount)	=0;
//			}
//			else if( (*pucAlrStat) == CONALR_STAT_HAPPEN)
//			{
//				if( ulVal > ulLimit *usBack/1000 )
//				{
//					(*pucAlrSumCount)++;
//					if((*pucAlrSumCount) >= ucMaxAlrCount)
//					{
//						(*pucAlrStat)		=CONALR_STAT_HAPPEN_NOT;
//						(*pucAlrSumCount)	=0;
//						if(Alarm_CheckEnsureJudgeList(usAlrCode,ucTerMPNo)!=RET_SUCCESS)
//							ucRet=RET_SUCCESS;
//					}
//				}else
//					(*pucAlrSumCount)	=0;
//			}else
//			{
//				(*pucAlrSumCount)	=0;
//				(*pucAlrStat) = CONALR_STAT_HAPPEN_NOT;
//			}
//		}
//	}
//
//	return ucRet;
//}

//unsigned char Alarm_ForeJudge_TerStatOn(	STHISSTATE	stHisState,	unsigned char *pucAlrStat,
//											unsigned char *pucAlrSumCount,unsigned char ucMaxAlrCount,
//											STDATETIME *pstAlrNewTime,    unsigned short  usAlrCode)
//{
//	unsigned char ucRet = RET_ERROR;
//	unsigned char ucStat = stHisState.ucStateFlag;
//	unsigned char	ucTerMPNo=RunPara_GetTer_MPNo();
//
//	if(ucStat != CONINVALIDVAL)
//	{
//		if(Is_DateTime_Equel(&(stHisState.stDateTime),pstAlrNewTime)== RET_ERROR)
//		{
//			(*pstAlrNewTime) = stHisState.stDateTime;
//			
//			if( (*pucAlrStat) == CONALR_STAT_HAPPEN_NOT )
//			{
//				if( ucStat == CON_ALR_STAT_ON) 
//				{
//					(*pucAlrSumCount)++;
//					if((*pucAlrSumCount) >= ucMaxAlrCount)
//					{
//						(*pucAlrStat)		=CONALR_STAT_HAPPEN;
//						(*pucAlrSumCount)	=0;
//						if(Alarm_CheckEnsureJudgeList(usAlrCode,ucTerMPNo)!=RET_SUCCESS)
//							ucRet=RET_SUCCESS;
//					}
//				}else
//					(*pucAlrSumCount)	=0;
//			}
//			else if( (*pucAlrStat) == CONALR_STAT_HAPPEN)
//			{
//				if( ucStat == CON_ALR_STAT_OFF) 
//				{
//					(*pucAlrSumCount)++;
//					if((*pucAlrSumCount) >= ucMaxAlrCount)
//					{
//						(*pucAlrStat)		=CONALR_STAT_HAPPEN_NOT;
//						(*pucAlrSumCount)	=0;
//						if(Alarm_CheckEnsureJudgeList(usAlrCode,ucTerMPNo)!=RET_SUCCESS)
//							ucRet=RET_SUCCESS;
//					}
//				}else
//					(*pucAlrSumCount)	=0;
//			}else
//			{
//				(*pucAlrSumCount)	=0;
//				(*pucAlrStat) = CONALR_STAT_HAPPEN_NOT;
//			}
//		}
//	}
//
//	return ucRet;
//}

//unsigned char Alarm_Chk_SPow_Err(unsigned char ucMPNo)
//{
//	return RET_ERROR;
///*	unsigned char ucRet = RET_SUCCESS;
//	STHISVAL	stHisP,stHisQ;
//	STHISVAL	stHisVa,stHisVb,stHisVc;
//	STHISVAL	stHisIa,stHisIb,stHisIc;
//	STHISVAL	stHisTmp;
//	unsigned long ulSPow,ulTmp;
//	double		dTmp=0;
//
//	stHisP = Get_History_InstantPow_byMPNo(ucMPNo,0);
//	stHisTmp = Get_History_RevInstantPow_byMPNo(ucMPNo,0);
//	if(stHisTmp.lVal!=CONINVALIDVAL)
//		stHisP.lVal = stHisP.lVal + stHisTmp.lVal;
//	stHisQ = Get_History_InstantVar_byMPNo(ucMPNo,0);
//	stHisTmp = Get_History_RevInstantVar_byMPNo(ucMPNo,0);
//	if(stHisTmp.lVal!=CONINVALIDVAL)
//		stHisQ.lVal = stHisQ.lVal + stHisTmp.lVal;
//
//	stHisVa = Get_History_Va_byMPNo(ucMPNo,0);
//	stHisVb = Get_History_Vb_byMPNo(ucMPNo,0);
//	stHisVc = Get_History_Vc_byMPNo(ucMPNo,0);
//
//	stHisIa = Get_History_Ia_byMPNo(ucMPNo,0);
//	stHisIb = Get_History_Ib_byMPNo(ucMPNo,0);
//	stHisIc = Get_History_Ic_byMPNo(ucMPNo,0);
//
//	if(	stHisP.lVal !=CONINVALIDVAL && stHisQ.lVal!=CONINVALIDVAL	&&
//		stHisVa.lVal!=CONINVALIDVAL && stHisVb.lVal!=CONINVALIDVAL	&& stHisVc.lVal!=CONINVALIDVAL &&
//		stHisIa.lVal!=CONINVALIDVAL && stHisIb.lVal!=CONINVALIDVAL	&& stHisIc.lVal!=CONINVALIDVAL )
//	{
//		ulSPow = GetSPow(stHisP.lVal,stHisQ.lVal);		//4位小数
//		dTmp = sqrt(stHisVa.lVal * stHisVa.lVal * stHisIa.lVal * stHisIa.lVal +
//					stHisVb.lVal * stHisVb.lVal * stHisIb.lVal * stHisIb.lVal +
//					stHisVc.lVal * stHisVc.lVal * stHisIc.lVal * stHisIc.lVal );
//		ulTmp = (unsigned long)dTmp;
//		if((ulSPow/100) < (ulTmp*4/5))
//			ucRet = RET_ERROR;
//	}
//
//	return ucRet;
//*/
//}

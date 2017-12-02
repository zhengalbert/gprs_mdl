/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Ter_Val.h
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-08-15    azh     the original version
******************************************************************************/
#ifndef _TER_VAL_H_
#define _TER_VAL_H_

#define		CONHISTERDATANUM		2
#define		CONHISTERSWITCHNUM		10

typedef struct _STTERTIMERVAL
{
//	STHISVAL	stHisDirPowTotal[CONHISTERDATANUM];
//	STHISVAL	stHisDirPowFee1[CONHISTERDATANUM];
//	STHISVAL	stHisDirPowFee2[CONHISTERDATANUM];
//	STHISVAL	stHisDirPowFee3[CONHISTERDATANUM];
//	STHISVAL	stHisDirPowFee4[CONHISTERDATANUM];				//	正向有功电量
//
//	STHISVAL	stHisRevPowTotal[CONHISTERDATANUM];
//	STHISVAL	stHisRevPowFee1[CONHISTERDATANUM];
//	STHISVAL	stHisRevPowFee2[CONHISTERDATANUM];
//	STHISVAL	stHisRevPowFee3[CONHISTERDATANUM];
//	STHISVAL	stHisRevPowFee4[CONHISTERDATANUM];				//	反向有功电量
//
//	STHISVAL	stHisDirVarTotal[CONHISTERDATANUM];
//	STHISVAL	stHisDirVarFee1[CONHISTERDATANUM];
//	STHISVAL	stHisDirVarFee2[CONHISTERDATANUM];
//	STHISVAL	stHisDirVarFee3[CONHISTERDATANUM];
//	STHISVAL	stHisDirVarFee4[CONHISTERDATANUM];				//	正向无功电量
//
//	STHISVAL	stHisRevVarTotal[CONHISTERDATANUM];
//	STHISVAL	stHisRevVarFee1[CONHISTERDATANUM];
//	STHISVAL	stHisRevVarFee2[CONHISTERDATANUM];
//	STHISVAL	stHisRevVarFee3[CONHISTERDATANUM];
//	STHISVAL	stHisRevVarFee4[CONHISTERDATANUM];				//	反向无功电量
//
//	STHISVAL	stHisDirVar1[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar1Fee1[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar1Fee2[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar1Fee3[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar1Fee4[CONHISTERDATANUM];				//	1象限无功电量
//
//	STHISVAL	stHisDirVar2[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar2Fee1[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar2Fee2[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar2Fee3[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar2Fee4[CONHISTERDATANUM];				//	2象限无功电量
//
//	STHISVAL	stHisDirVar3[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar3Fee1[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar3Fee2[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar3Fee3[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar3Fee4[CONHISTERDATANUM];				//	3象限无功电量
//
//	STHISVAL	stHisDirVar4[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar4Fee1[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar4Fee2[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar4Fee3[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar4Fee4[CONHISTERDATANUM];				//	4象限无功电量
//
//	STHISVAL	stHisDirPowMaxDemand[CONHISTERDATANUM];
//	STHISVAL	stHisDirPowMaxDemandFee1[CONHISTERDATANUM];
//	STHISVAL	stHisDirPowMaxDemandFee2[CONHISTERDATANUM];
//	STHISVAL	stHisDirPowMaxDemandFee3[CONHISTERDATANUM];
//	STHISVAL	stHisDirPowMaxDemandFee4[CONHISTERDATANUM];		//	正向有功最大需量
//
//	STHISTIME	stHisDirPowMaxDemandTime[CONHISTERDATANUM];
//	STHISTIME	stHisDirPowMaxDemandTimeFee1[CONHISTERDATANUM];
//	STHISTIME	stHisDirPowMaxDemandTimeFee2[CONHISTERDATANUM];
//	STHISTIME	stHisDirPowMaxDemandTimeFee3[CONHISTERDATANUM];
//	STHISTIME	stHisDirPowMaxDemandTimeFee4[CONHISTERDATANUM];	//	正向有功最大需量发生时间
//
//	STHISVAL	stHisDirVarMaxDemand[CONHISTERDATANUM];
//	STHISVAL	stHisDirVarMaxDemandFee1[CONHISTERDATANUM];
//	STHISVAL	stHisDirVarMaxDemandFee2[CONHISTERDATANUM];
//	STHISVAL	stHisDirVarMaxDemandFee3[CONHISTERDATANUM];
//	STHISVAL	stHisDirVarMaxDemandFee4[CONHISTERDATANUM];		//	正向无功最大需量
//
//	STHISTIME	stHisDirVarMaxDemandTime[CONHISTERDATANUM];
//	STHISTIME	stHisDirVarMaxDemandTimeFee1[CONHISTERDATANUM];
//	STHISTIME	stHisDirVarMaxDemandTimeFee2[CONHISTERDATANUM];
//	STHISTIME	stHisDirVarMaxDemandTimeFee3[CONHISTERDATANUM];
//	STHISTIME	stHisDirVarMaxDemandTimeFee4[CONHISTERDATANUM];	//	正向无功最大需量发生时间
//
//	STHISVAL	stHisRevPowMaxDemand[CONHISTERDATANUM];
//	STHISVAL	stHisRevPowMaxDemandFee1[CONHISTERDATANUM];
//	STHISVAL	stHisRevPowMaxDemandFee2[CONHISTERDATANUM];
//	STHISVAL	stHisRevPowMaxDemandFee3[CONHISTERDATANUM];
//	STHISVAL	stHisRevPowMaxDemandFee4[CONHISTERDATANUM];		//	反向有功最大需量
//
//	STHISTIME	stHisRevPowMaxDemandTime[CONHISTERDATANUM];
//	STHISTIME	stHisRevPowMaxDemandTimeFee1[CONHISTERDATANUM];
//	STHISTIME	stHisRevPowMaxDemandTimeFee2[CONHISTERDATANUM];
//	STHISTIME	stHisRevPowMaxDemandTimeFee3[CONHISTERDATANUM];
//	STHISTIME	stHisRevPowMaxDemandTimeFee4[CONHISTERDATANUM];	//	反向有功最大需量发生时间
//
//	STHISVAL	stHisRevVarMaxDemand[CONHISTERDATANUM];
//	STHISVAL	stHisRevVarMaxDemandFee1[CONHISTERDATANUM];
//	STHISVAL	stHisRevVarMaxDemandFee2[CONHISTERDATANUM];
//	STHISVAL	stHisRevVarMaxDemandFee3[CONHISTERDATANUM];
//	STHISVAL	stHisRevVarMaxDemandFee4[CONHISTERDATANUM];		//	反向无功最大需量
//
//	STHISTIME	stHisRevVarMaxDemandTime[CONHISTERDATANUM];
//	STHISTIME	stHisRevVarMaxDemandTimeFee1[CONHISTERDATANUM];
//	STHISTIME	stHisRevVarMaxDemandTimeFee2[CONHISTERDATANUM];
//	STHISTIME	stHisRevVarMaxDemandTimeFee3[CONHISTERDATANUM];
//	STHISTIME	stHisRevVarMaxDemandTimeFee4[CONHISTERDATANUM];//	反向无功最大需量发生时间
//
//	STHISVAL	stHisVolA[CONHISTERDATANUM];
//	STHISVAL	stHisVolB[CONHISTERDATANUM];
//	STHISVAL	stHisVolC[CONHISTERDATANUM];					//	三相电压
//
//	STHISVAL	stHisVolA_Phase[CONHISTERDATANUM];
//	STHISVAL	stHisVolB_Phase[CONHISTERDATANUM];
//	STHISVAL	stHisVolC_Phase[CONHISTERDATANUM];				//	三相电压相角
//
//	STHISVAL	stHisIa[CONHISTERDATANUM];
//	STHISVAL	stHisIb[CONHISTERDATANUM];
//	STHISVAL	stHisIc[CONHISTERDATANUM];						//	三相电流
//
//	STHISVAL	stHisIa_Phase[CONHISTERDATANUM];
//	STHISVAL	stHisIb_Phase[CONHISTERDATANUM];
//	STHISVAL	stHisIc_Phase[CONHISTERDATANUM];				//	三相电流相角
//
//	STHISVAL	stHisInstantPow[CONHISTERDATANUM];		
//	STHISVAL	stHisInstantPowA[CONHISTERDATANUM];	
//	STHISVAL	stHisInstantPowB[CONHISTERDATANUM];	
//	STHISVAL	stHisInstantPowC[CONHISTERDATANUM];				//	瞬时有功
//
//	STHISVAL	stHisInstantVar[CONHISTERDATANUM];		
//	STHISVAL	stHisInstantVarA[CONHISTERDATANUM];	
//	STHISVAL	stHisInstantVarB[CONHISTERDATANUM];	
//	STHISVAL	stHisInstantVarC[CONHISTERDATANUM];				//	瞬时无功

//	STHISSTATE	stHisDoorStat[CONHISTERSWITCHNUM];				//	门节点状态
	STHISSTATE	stHisPowerStat[CONHISTERSWITCHNUM];				//	供电状态
//	STHISSTATE	stHisSwitch0Stat[CONHISTERSWITCHNUM];			//	开关1状态
//	STHISSTATE	stHisSwitch1Stat[CONHISTERSWITCHNUM];			//	开关2状态
//	STHISSTATE	stHisSwitch2Stat[CONHISTERSWITCHNUM];			//	开关3状态
//	STHISSTATE	stHisSwitch3Stat[CONHISTERSWITCHNUM];			//	开关4状态
//	STHISSTATE	stHisSwitch4Stat[CONHISTERSWITCHNUM];			//	开关5状态
//	STHISSTATE	stHisSwitch5Stat[CONHISTERSWITCHNUM];			//	开关6状态
//	STHISSTATE	stHisSwitch6Stat[CONHISTERSWITCHNUM];			//	开关7状态

//	STHISSTATE	stHisPolReverseAStat[CONHISTERDATANUM];			//	A相反极性状态
//	STHISSTATE	stHisPolReverseBStat[CONHISTERDATANUM];			//	B相反极性状态
//	STHISSTATE	stHisPolReverseCStat[CONHISTERDATANUM];			//	C相反极性状态

//	STHISSTATE	stHisShortCirA1Stat[CONHISTERDATANUM];			//	A相CT1次侧短路
//	STHISSTATE	stHisShortCirB1Stat[CONHISTERDATANUM];			//	B相CT1次侧短路
//	STHISSTATE	stHisShortCirC1Stat[CONHISTERDATANUM];			//	C相CT1次侧短路
//
//	STHISSTATE	stHisOpenCirA2Stat[CONHISTERDATANUM];			//	A相CT2次侧开路
//	STHISSTATE	stHisOpenCirB2Stat[CONHISTERDATANUM];			//	B相CT2次侧开路
//	STHISSTATE	stHisOpenCirC2Stat[CONHISTERDATANUM];			//	C相CT2次侧开路
//
//	STHISSTATE	stHisShortCirA2Stat[CONHISTERDATANUM];			//	A相CT2次侧短路
//	STHISSTATE	stHisShortCirB2Stat[CONHISTERDATANUM];			//	B相CT2次侧短路
//	STHISSTATE	stHisShortCirC2Stat[CONHISTERDATANUM];			//	C相CT2次侧短路

//	STHISSTATE	stHisVolCirReveStat[CONHISTERDATANUM];			//	电压回路逆相序
}STTERTIMERVAL;

__EXTERN void	Init_TerHisData(STTERTIMERVAL *pstTerTimerVal);
__EXTERN void	Init_TerHisData_All(STTERTIMERVAL *pstTerTimerVal);
//
////正向有功电量
//__EXTERN STHISVAL Get_History_DirPower_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////反向有功电量
//__EXTERN STHISVAL Get_History_UnDirPower_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////正向无功电量
//__EXTERN STHISVAL Get_History_DirVar_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////反向无功电量
//__EXTERN STHISVAL Get_History_UnDirVar_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////1象限无功电量
//__EXTERN STHISVAL Get_History_DirVar1_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////2象限无功电量
//__EXTERN STHISVAL Get_History_DirVar2_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////3象限无功电量
//__EXTERN STHISVAL Get_History_DirVar3_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////4象限无功电量
//__EXTERN STHISVAL Get_History_DirVar4_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////正向有功最大需量
//__EXTERN STHISVAL Get_History_DirPowMaxDemand_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////正向有功最大需量发生时间
//__EXTERN STHISTIME Get_History_DirPowMaxDemandTime_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////正向无功最大需量
//__EXTERN STHISVAL Get_History_DirVarMaxDemand_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////正向无功最大需量发生时间
//__EXTERN STHISTIME Get_History_DirVarMaxDemandTime_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////反向有功最大需量
//__EXTERN STHISVAL Get_History_UnDirPowMaxDemand_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////反向有功最大需量发生时间
//__EXTERN STHISTIME Get_History_UnDirPowMaxDemandTime_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////反向无功最大需量
//__EXTERN STHISVAL Get_History_UnDirVarMaxDemand_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////反向无功最大需量发生时间
//__EXTERN STHISTIME Get_History_UnDirVarMaxDemandTime_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////电压
//__EXTERN STHISVAL Get_History_V_Disp_Ter(unsigned char ucPos,unsigned char ucNo);
////电流
//__EXTERN STHISVAL Get_History_I_Disp_Ter(unsigned char ucPos,unsigned char ucNo);
////显示电压相角
//__EXTERN STHISVAL Get_History_V_Phase_Disp_Ter(unsigned char ucPos,unsigned char ucNo);
////显示电流相角
//__EXTERN STHISVAL Get_History_I_Phase_Disp_Ter(unsigned char ucPos,unsigned char ucNo);
////无功功率
//__EXTERN STHISVAL Get_History_InstantVar_Disp_Ter(unsigned char ucPos,unsigned char ucNo);
////瞬时有功、无功
//__EXTERN STHISVAL Get_History_InstantPow_Ter(unsigned char ucPos,unsigned char ucNo);
////开关量
//__EXTERN STHISSTATE Get_History_DoorStat_Ter(unsigned char ucPos);
__EXTERN STHISSTATE Get_History_PowerStat_Ter(unsigned char ucPos);
//__EXTERN STHISSTATE Get_History_SwitchStat_Ter(unsigned char ucPos,unsigned char ucSwitchNo);
////告警信息……电流反极性
//__EXTERN STHISSTATE Get_History_PolReverseStat_Ter(unsigned char ucPos,unsigned char ucNo);
////告警信息……CT一次侧短路
//__EXTERN STHISSTATE Get_History_ShortCir1Stat_Ter(unsigned char ucPos,unsigned char ucNo);
////告警信息……CT二次侧开路
//__EXTERN STHISSTATE Get_History_OpenCir2Stat_Ter(unsigned char ucPos,unsigned char ucNo);
////告警信息……CT二次侧短路
//__EXTERN STHISSTATE Get_History_ShortCir2Stat_Ter(unsigned char ucPos,unsigned char ucNo);
////告警信息……逆相序
//__EXTERN STHISSTATE Get_History_VolCirReveStat_Ter(unsigned char ucPos);

///*******************************************终端数据获取*************************************/
////正向有功电量
//__EXTERN unsigned char Add_History_DirPower_Ter(unsigned long lVal,unsigned char ucFeeNo);
////反向有功电量
//__EXTERN unsigned char Add_History_UnDirPower_Ter(unsigned long lVal,unsigned char ucFeeNo);
////正向无功电量
//__EXTERN unsigned char Add_History_DirVar_Ter(unsigned long lVal,unsigned char ucFeeNo);
////反向无功电量
//__EXTERN unsigned char Add_History_UnDirVar_Ter(unsigned long lVal,unsigned char ucFeeNo);
////1象限无功
//__EXTERN unsigned char Add_History_DirVar1_Ter(unsigned long lVal,unsigned char ucFeeNo);
////2象限无功
//__EXTERN unsigned char Add_History_DirVar2_Ter(unsigned long lVal,unsigned char ucFeeNo);
////3象限无功
//__EXTERN unsigned char Add_History_DirVar3_Ter(unsigned long lVal,unsigned char ucFeeNo);
////4象限无功
//__EXTERN unsigned char Add_History_DirVar4_Ter(unsigned long lVal,unsigned char ucFeeNo);
////正向有功最大需量
//__EXTERN unsigned char Add_History_DirPowMaxDemand_Ter(unsigned long lVal,unsigned char ucFeeNo);
////正向有功最大需量发生时间
//__EXTERN unsigned char Add_History_DirPowMaxDemandTime_Ter(STDATETIME	stDateTime,unsigned char ucFeeNo);
////正向无功最大需量
//__EXTERN unsigned char Add_History_DirVarMaxDemand_Ter(unsigned long lVal,unsigned char ucFeeNo);
////正向无功最大需量发生时间
//__EXTERN unsigned char Add_History_DirVarMaxDemandTime_Ter(STDATETIME	stDateTime,unsigned char ucFeeNo);
////反向有功最大需量
//__EXTERN unsigned char Add_History_UnDirPowMaxDemand_Ter(unsigned long lVal,unsigned char ucFeeNo);
////反向有功最大需量发生时间
//__EXTERN unsigned char Add_History_UnDirPowMaxDemandTime_Ter(STDATETIME	stDateTime,unsigned char ucFeeNo);
////反向无功最大需量
//__EXTERN unsigned char Add_History_UnDirVarMaxDemand_Ter(unsigned long lVal,unsigned char ucFeeNo);
////反向无功最大需量发生时间
//__EXTERN unsigned char Add_History_UnDirVarMaxDemandTime_Ter(STDATETIME	stDateTime,unsigned char ucFeeNo);
////电压
//__EXTERN unsigned char Add_History_V_Ter(unsigned long lVal,unsigned char ucNo);
////电压相角
//__EXTERN unsigned char Add_History_V_Phase_Ter(unsigned long lVal,unsigned char ucNo);
////电流
//__EXTERN unsigned char Add_History_I_Ter(unsigned long lVal,unsigned char ucNo);
////电流相角
//__EXTERN unsigned char Add_History_I_Phase_Ter(unsigned long lVal,unsigned char ucNo);
////瞬时有功
//__EXTERN unsigned char Add_History_InstantPow_Ter(unsigned long lVal,unsigned char ucSign,unsigned char ucNo);
////瞬时无功
//__EXTERN unsigned char Add_History_InstantVar_Ter(unsigned long lVal,unsigned char ucSign,unsigned char ucNo);
////门节点状态
////__EXTERN unsigned char Add_History_DoorStat_Ter(unsigned char	ucStateFlag);
//供电状态
__EXTERN unsigned char Add_History_PowerStat_Ter(unsigned char	ucStateFlag);
//开关状态
//__EXTERN unsigned char Add_History_SwitchStat_Ter(unsigned char	ucStateFlag,unsigned char ucSwithcNo);
//告警信息……电流反极性
//__EXTERN unsigned char  Add_History_PolReverseStat_Ter(unsigned char ucStateFlag,unsigned char ucNo);
////告警信息……CT一次侧短路
//__EXTERN unsigned char  Add_History_ShortCir1Stat_Ter(unsigned char ucStateFlag,unsigned char ucNo);
////告警信息……CT二次侧开路
//__EXTERN unsigned char  Add_History_OpenCir2Stat_Ter(unsigned char ucStateFlag,unsigned char ucNo);
////告警信息……CT二次侧短路
//__EXTERN unsigned char  Add_History_ShortCir2Stat_Ter(unsigned char ucStateFlag,unsigned char ucNo);
////告警信息……逆相序
//__EXTERN unsigned char  Add_History_VolCirReveStat_Ter(unsigned char ucStateFlag);
#endif

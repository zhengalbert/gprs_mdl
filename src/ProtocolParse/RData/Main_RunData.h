/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_RunData.h
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-08-15    azh     the original version
******************************************************************************/
#ifndef _MAIN_RUNDATA_H_
#define _MAIN_RUNDATA_H_

__EXTERN STMETERTIMERVAL stMeterTimerVal[CON_METER_NUM];		//定时抄表数据列表

__EXTERN void Init_MeterHisData(STMETERTIMERVAL *pstMeterTimerVal);
__EXTERN void Add_History_HisVal(STHISVAL	*pstHisVal,unsigned char ucHisValNum,unsigned long lVal);
__EXTERN void Add_History_HisTime(STHISTIME	*pstHisTime,unsigned char ucHisTimeNum,STDATETIME	stDateTime);
__EXTERN void Add_History_HisState(STHISSTATE	*pstHisState,unsigned char ucHisStateNum,unsigned char ucStateFlag);

//正向有功电量
__EXTERN STHISVAL Get_History_DirPower(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//反向有功电量
__EXTERN STHISVAL Get_History_UnDirPower(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//正向无功电量
__EXTERN STHISVAL Get_History_DirVar(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//反向无功电量
__EXTERN STHISVAL Get_History_UnDirVar(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//1象限无功电量
__EXTERN STHISVAL Get_History_DirVar1(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//2象限无功电量
__EXTERN STHISVAL Get_History_DirVar2(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//3象限无功电量
__EXTERN STHISVAL Get_History_DirVar3(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//4象限无功电量
__EXTERN STHISVAL Get_History_DirVar4(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//正向有功最大需量
__EXTERN STHISVAL Get_History_DirPowMaxDemand(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//正向有功最大需量发生时间
__EXTERN STHISTIME Get_History_DirPowMaxDemandTime(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//正向无功最大需量
__EXTERN STHISVAL Get_History_DirVarMaxDemand(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//正向无功最大需量发生时间
__EXTERN STHISTIME Get_History_DirVarMaxDemandTime(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//反向有功最大需量
__EXTERN STHISVAL Get_History_UnDirPowMaxDemand(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//反向有功最大需量发生时间
__EXTERN STHISTIME Get_History_UnDirPowMaxDemandTime(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//反向无功最大需量
__EXTERN STHISVAL Get_History_UnDirVarMaxDemand(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//反向无功最大需量发生时间
__EXTERN STHISTIME Get_History_UnDirVarMaxDemandTime(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);

__EXTERN STHISVAL Get_History_DirPowA(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
__EXTERN STHISVAL Get_History_DirPowB(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
__EXTERN STHISVAL Get_History_DirPowC(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
__EXTERN STHISVAL Get_History_DirPowALm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
__EXTERN STHISVAL Get_History_DirPowBLm(unsigned char ucMeterNo, unsigned char ucPos, unsigned char ucNo);
__EXTERN STHISVAL Get_History_DirPowCLm(unsigned char ucMeterNo, unsigned char ucPos, unsigned char ucNo);

//正向有功电量（上月）
__EXTERN STHISVAL Get_History_DirPowerLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//反向有功电量(上月)
__EXTERN STHISVAL Get_History_UnDirPowerLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//正向无功电量(上月)
__EXTERN STHISVAL Get_History_DirVarLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//反向无功电量(上月)
__EXTERN STHISVAL Get_History_UnDirVarLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//1象限无功电量(上月)
__EXTERN STHISVAL Get_History_DirVar1Lm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//2象限无功电量(上月)
__EXTERN STHISVAL Get_History_DirVar2Lm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//3象限无功电量(上月)
__EXTERN STHISVAL Get_History_DirVar3Lm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//4象限无功电量(上月)
__EXTERN STHISVAL Get_History_DirVar4Lm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//正向有功最大需量(上月)
__EXTERN STHISVAL Get_History_DirPowMaxDemandLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//正向有功最大需量发生时间(上月)
__EXTERN STHISTIME Get_History_DirPowMaxDemandTimeLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//正向无功最大需量(上月)
__EXTERN STHISVAL Get_History_DirVarMaxDemandLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//正向无功最大需量发生时间(上月)
__EXTERN STHISTIME Get_History_DirVarMaxDemandTimeLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//反向有功最大需量(上月)
__EXTERN STHISVAL Get_History_UnDirPowMaxDemandLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//反向有功最大需量发生时间(上月)
__EXTERN STHISTIME Get_History_UnDirPowMaxDemandTimeLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//正向无功最大需量(上月)
__EXTERN STHISVAL Get_History_UnDirVarMaxDemandLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//正向无功最大需量发生时间(上月)
__EXTERN STHISTIME Get_History_UnDirVarMaxDemandTimeLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//电压
__EXTERN STHISVAL Get_History_V(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
//电流
__EXTERN STHISVAL Get_History_I(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
//电压相角
__EXTERN STHISVAL Get_History_V_Phase(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
//电流相角
__EXTERN STHISVAL Get_History_I_Phase(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
//瞬时有功、无功
__EXTERN STHISVAL Get_History_InstantPow(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
__EXTERN STHISVAL Get_History_InstantVar(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
//有功、无功脉冲常数
__EXTERN STHISVAL Get_History_PowPulseConst(unsigned char ucMeterNo,unsigned char ucPos);
__EXTERN STHISVAL Get_History_VarPulseConst(unsigned char ucMeterNo,unsigned char ucPos);
//费率
__EXTERN STHISFEERATE Get_History_FeeRate(unsigned char ucMeterNo,unsigned char ucPos);
//编程时间
__EXTERN STHISTIME Get_History_ProgramTime(unsigned char ucMeterNo,unsigned char ucPos);
//需量清零时间
__EXTERN STHISTIME Get_History_DemandClrTime(unsigned char ucMeterNo,unsigned char ucPos);
//编程次数
__EXTERN STHISVAL Get_History_ProgramCount(unsigned char ucMeterNo,unsigned char ucPos);
//需量清零次数
__EXTERN STHISVAL Get_History_DemandClrCount(unsigned char ucMeterNo,unsigned char ucPos);
//电表清零时间
__EXTERN STHISTIME Get_History_MeterClrTime(unsigned char ucMeterNo,unsigned char ucPos);
//事件清零时间
__EXTERN STHISTIME Get_History_EventClrTime(unsigned char ucMeterNo,unsigned char ucPos);
//校时时间
__EXTERN STHISTIME Get_History_JiaoshiTime(unsigned char ucMeterNo,unsigned char ucPos);
//电表清零次数
__EXTERN STHISVAL Get_History_MeterClrCount(unsigned char ucMeterNo,unsigned char ucPos);
//事件清零次数
__EXTERN STHISVAL Get_History_EventClrCount(unsigned char ucMeterNo,unsigned char ucPos);
//校时次数
__EXTERN STHISVAL Get_History_JiaoshiCount(unsigned char ucMeterNo,unsigned char ucPos);
//电池工作时间
__EXTERN STHISVAL Get_History_BatWorkTime(unsigned char ucMeterNo,unsigned char ucPos);
//电表运行状态字
__EXTERN STHISSTATE Get_History_MeterStatFlag(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFlagNum);
//电网运行状态字
__EXTERN STHISSTATE Get_History_NetStatFlag(unsigned char ucMeterNo,unsigned char ucPos);
//抄表日
__EXTERN STHISTIME Get_History_CopyDayTime(unsigned char ucMeterNo,unsigned char ucPos);
//断相次数
__EXTERN STHISVAL Get_History_VolBreakNum(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
//断相时间累计值
__EXTERN STHISVAL Get_History_VolBreakTime(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
//最近一次断相起始时刻
__EXTERN STHISTIME Get_History_LastVolBreakBegTime(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
//最近一次断相结束时刻
__EXTERN STHISTIME Get_History_LastVolBreakEndTime(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
//年月日
__EXTERN STHISTIME Get_History_Date(unsigned char ucMeterNo,unsigned char ucPos);
//时分秒
__EXTERN STHISTIME Get_History_Time(unsigned char ucMeterNo,unsigned char ucPos);

__EXTERN unsigned char Add_History_DirPowA(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo);
__EXTERN unsigned char Add_History_DirPowB(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo);
__EXTERN unsigned char Add_History_DirPowC(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo);
__EXTERN unsigned char Add_History_DirPowALm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo);
__EXTERN unsigned char Add_History_DirPowBLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo);
__EXTERN unsigned char Add_History_DirPowCLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo);
#endif

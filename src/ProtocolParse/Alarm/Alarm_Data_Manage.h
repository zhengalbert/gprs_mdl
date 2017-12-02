/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Alarm_Data_Manage.h
 Description    : ��д�澯��ظ���������
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2013-01-08    azh     the original version
******************************************************************************/

#ifndef _ALARM_DATA_MANAGE_H_
#define _ALARM_DATA_MANAGE_H_

/*
//�����й����������£�
unsigned char Add_AlarmForeJudge_DirPowerLmTotal(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//�����й�����
unsigned char Add_AlarmForeJudge_DirPowerTotal(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirPowerPlus(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirPowerApex(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirPowerFlat(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirPowerVale(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//�����й�����
unsigned char Add_AlarmForeJudge_UnDirPowerTotal(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_UnDirPowerPlus(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_UnDirPowerApex(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_UnDirPowerFlat(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_UnDirPowerVale(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//�����޹�����
unsigned char Add_AlarmForeJudge_DirVarTotal(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirVarPlus(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirVarApex(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirVarFlat(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirVarVale(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//�����޹�����
unsigned char Add_AlarmForeJudge_UnDirVarTotal(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_UnDirVarPlus(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_UnDirVarApex(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_UnDirVarFlat(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_UnDirVarVale(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//1�����޹�����
unsigned char Add_AlarmForeJudge_DirVar1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirVar1Fee1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirVar1Fee2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirVar1Fee3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirVar1Fee4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//2�����޹�����
unsigned char Add_AlarmForeJudge_DirVar2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirVar2Fee1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirVar2Fee2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirVar2Fee3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirVar2Fee4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//3�����޹�����
unsigned char Add_AlarmForeJudge_DirVar3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirVar3Fee1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirVar3Fee2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirVar3Fee3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirVar3Fee4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//4�����޹�����
unsigned char Add_AlarmForeJudge_DirVar4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirVar4Fee1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirVar4Fee2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirVar4Fee3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirVar4Fee4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//��ѹ
unsigned char Add_AlarmForeJudge_Va(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_Vb(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_Vc(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//����
unsigned char Add_AlarmForeJudge_Ia(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_Ib(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_Ic(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//˲ʱ�й����޹�
unsigned char Add_AlarmForeJudge_InstantPow(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_InstantVar(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//�����й��������
unsigned char Add_AlarmForeJudge_DirPowMaxDemand(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirPowMaxDemandFee1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirPowMaxDemandFee2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirPowMaxDemandFee3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_DirPowMaxDemandFee4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//�����й������������ʱ��
unsigned char Add_AlarmForeJudge_DirPowMaxDemandTime(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
unsigned char Add_AlarmForeJudge_DirPowMaxDemandTimeFee1(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
unsigned char Add_AlarmForeJudge_DirPowMaxDemandTimeFee2(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
unsigned char Add_AlarmForeJudge_DirPowMaxDemandTimeFee3(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
unsigned char Add_AlarmForeJudge_DirPowMaxDemandTimeFee4(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
//�����й��������
unsigned char Add_AlarmForeJudge_UnDirPowMaxDemand(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_UnDirPowMaxDemandFee1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_UnDirPowMaxDemandFee2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_UnDirPowMaxDemandFee3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_UnDirPowMaxDemandFee4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//�����й������������ʱ��
unsigned char Add_AlarmForeJudge_UnDirPowMaxDemandTime(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
unsigned char Add_AlarmForeJudge_UnDirPowMaxDemandTimeFee1(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
unsigned char Add_AlarmForeJudge_UnDirPowMaxDemandTimeFee2(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
unsigned char Add_AlarmForeJudge_UnDirPowMaxDemandTimeFee3(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
unsigned char Add_AlarmForeJudge_UnDirPowMaxDemandTimeFee4(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
//�й����޹����峣��
unsigned char Add_AlarmForeJudge_PowPulseConst(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmForeJudge_VarPulseConst(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);*/
//����
//__EXTERN unsigned char Add_AlarmForeJudge_FeeRate(unsigned char ucMPNo,unsigned char ucNum,STFEERATELIST	stFeeRateList);
////���ʱ��
//__EXTERN unsigned char Add_AlarmForeJudge_ProgramTime(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
////��̴���
//__EXTERN unsigned char Add_AlarmForeJudge_ProgramCount(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
////�������״̬��
//__EXTERN unsigned char Add_AlarmForeJudge_MeterStatFlag(unsigned char ucMPNo,unsigned char ucNum,unsigned char	ucStateFlag);
////������
//__EXTERN unsigned char Add_AlarmForeJudge_CopyDayTime(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
////�������
//__EXTERN unsigned char Add_AlarmForeJudge_LostVolNum(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
////������
//__EXTERN unsigned char Add_AlarmForeJudge_Date(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
////ʱ����
//__EXTERN unsigned char Add_AlarmForeJudge_Time(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);

__EXTERN unsigned char Alarm_GetHisState_ForeJudge(unsigned char ucNum,unsigned short usItemLabel,unsigned char ucListNo,STHISSTATE *pstHisState);
//__EXTERN unsigned char Alarm_GetHisVal_ForeJudge(unsigned char ucNum,unsigned short usItemLabel,unsigned char ucListNo,STHISVAL *pstHisVal);
__EXTERN unsigned char Alarm_GetHisTime_ForeJudge(unsigned char ucNum,unsigned short usItemLabel,unsigned char ucListNo,STHISTIME *pstHisTime);
//__EXTERN unsigned char Alarm_GetHisFeeRate_ForeJudge(unsigned char ucNum,unsigned short usItemLabel,unsigned char ucListNo,STHISFEERATE *pstHisFeeRate);
/*
//�����й����������£�
unsigned char Add_AlarmEnsureJudge_DirPowerLmTotal(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//�����й�����
unsigned char Add_AlarmEnsureJudge_DirPowerTotal(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirPowerPlus(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirPowerApex(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirPowerFlat(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirPowerVale(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//�����й�����
unsigned char Add_AlarmEnsureJudge_UnDirPowerTotal(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_UnDirPowerPlus(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_UnDirPowerApex(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_UnDirPowerFlat(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_UnDirPowerVale(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//�����޹�����
unsigned char Add_AlarmEnsureJudge_DirVarTotal(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirVarPlus(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirVarApex(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirVarFlat(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirVarVale(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//�����޹�����
unsigned char Add_AlarmEnsureJudge_UnDirVarTotal(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_UnDirVarPlus(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_UnDirVarApex(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_UnDirVarFlat(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_UnDirVarVale(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//1�����޹�����
unsigned char Add_AlarmEnsureJudge_DirVar1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirVar1Fee1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirVar1Fee2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirVar1Fee3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirVar1Fee4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//2�����޹�����
unsigned char Add_AlarmEnsureJudge_DirVar2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirVar2Fee1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirVar2Fee2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirVar2Fee3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirVar2Fee4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//3�����޹�����
unsigned char Add_AlarmEnsureJudge_DirVar3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirVar3Fee1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirVar3Fee2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirVar3Fee3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirVar3Fee4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//4�����޹�����
unsigned char Add_AlarmEnsureJudge_DirVar4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirVar4Fee1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirVar4Fee2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirVar4Fee3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirVar4Fee4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//��ѹ
unsigned char Add_AlarmEnsureJudge_Va(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_Vb(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_Vc(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//����
unsigned char Add_AlarmEnsureJudge_Ia(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_Ib(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_Ic(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//˲ʱ�й����޹�
unsigned char Add_AlarmEnsureJudge_InstantPow(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_InstantVar(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//�����й��������
unsigned char Add_AlarmEnsureJudge_DirPowMaxDemand(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirPowMaxDemandFee1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirPowMaxDemandFee2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirPowMaxDemandFee3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_DirPowMaxDemandFee4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//�����й������������ʱ��
unsigned char Add_AlarmEnsureJudge_DirPowMaxDemandTime(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
unsigned char Add_AlarmEnsureJudge_DirPowMaxDemandTimeFee1(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
unsigned char Add_AlarmEnsureJudge_DirPowMaxDemandTimeFee2(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
unsigned char Add_AlarmEnsureJudge_DirPowMaxDemandTimeFee3(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
unsigned char Add_AlarmEnsureJudge_DirPowMaxDemandTimeFee4(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
//�����й��������
unsigned char Add_AlarmEnsureJudge_UnDirPowMaxDemand(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_UnDirPowMaxDemandFee1(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_UnDirPowMaxDemandFee2(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_UnDirPowMaxDemandFee3(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_UnDirPowMaxDemandFee4(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
//�����й������������ʱ��
unsigned char Add_AlarmEnsureJudge_UnDirPowMaxDemandTime(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
unsigned char Add_AlarmEnsureJudge_UnDirPowMaxDemandTimeFee1(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
unsigned char Add_AlarmEnsureJudge_UnDirPowMaxDemandTimeFee2(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
unsigned char Add_AlarmEnsureJudge_UnDirPowMaxDemandTimeFee3(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
unsigned char Add_AlarmEnsureJudge_UnDirPowMaxDemandTimeFee4(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
//�й����޹����峣��
unsigned char Add_AlarmEnsureJudge_PowPulseConst(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
unsigned char Add_AlarmEnsureJudge_VarPulseConst(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);*/
//����
//__EXTERN unsigned char Add_AlarmEnsureJudge_FeeRate(unsigned char ucMPNo,unsigned char ucNum,STFEERATELIST	stFeeRateList);
////���ʱ��
//__EXTERN unsigned char Add_AlarmEnsureJudge_ProgramTime(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
////��̴���
//__EXTERN unsigned char Add_AlarmEnsureJudge_ProgramCount(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
////�������״̬��
//__EXTERN unsigned char Add_AlarmEnsureJudge_MeterStatFlag(unsigned char ucMPNo,unsigned char ucNum,unsigned char	ucStateFlag);
////������
//__EXTERN unsigned char Add_AlarmEnsureJudge_CopyDayTime(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
////�������
//__EXTERN unsigned char Add_AlarmEnsureJudge_LostVolNum(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal);
////������
//__EXTERN unsigned char Add_AlarmEnsureJudge_Date(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);
////ʱ����
//__EXTERN unsigned char Add_AlarmEnsureJudge_Time(unsigned char ucMPNo,unsigned char ucNum,STDATETIME	stDateTime);

__EXTERN unsigned char Add_AlarmForeJudge_lValue(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal,unsigned short usItemLablePara);
//__EXTERN unsigned char Add_AlarmForeJudge_stTimeValue(unsigned char ucMPNo,unsigned char ucNum,STDATETIME stDateTime,unsigned short usItemLablePara);
__EXTERN unsigned char Add_AlarmEnsureJudge_lValue(unsigned char ucMPNo,unsigned char ucNum,unsigned long lVal,unsigned short usItemLablePara);
//__EXTERN unsigned char Add_AlarmEnsureJudge_stTimeValue(unsigned char ucMPNo,unsigned char ucNum,STDATETIME stDateTime,unsigned short usItemLablePara);

#endif

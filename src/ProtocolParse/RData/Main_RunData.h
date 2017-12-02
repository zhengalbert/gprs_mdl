/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_RunData.h
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-08-15    azh     the original version
******************************************************************************/
#ifndef _MAIN_RUNDATA_H_
#define _MAIN_RUNDATA_H_

__EXTERN STMETERTIMERVAL stMeterTimerVal[CON_METER_NUM];		//��ʱ���������б�

__EXTERN void Init_MeterHisData(STMETERTIMERVAL *pstMeterTimerVal);
__EXTERN void Add_History_HisVal(STHISVAL	*pstHisVal,unsigned char ucHisValNum,unsigned long lVal);
__EXTERN void Add_History_HisTime(STHISTIME	*pstHisTime,unsigned char ucHisTimeNum,STDATETIME	stDateTime);
__EXTERN void Add_History_HisState(STHISSTATE	*pstHisState,unsigned char ucHisStateNum,unsigned char ucStateFlag);

//�����й�����
__EXTERN STHISVAL Get_History_DirPower(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����й�����
__EXTERN STHISVAL Get_History_UnDirPower(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����޹�����
__EXTERN STHISVAL Get_History_DirVar(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����޹�����
__EXTERN STHISVAL Get_History_UnDirVar(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//1�����޹�����
__EXTERN STHISVAL Get_History_DirVar1(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//2�����޹�����
__EXTERN STHISVAL Get_History_DirVar2(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//3�����޹�����
__EXTERN STHISVAL Get_History_DirVar3(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//4�����޹�����
__EXTERN STHISVAL Get_History_DirVar4(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����й��������
__EXTERN STHISVAL Get_History_DirPowMaxDemand(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����й������������ʱ��
__EXTERN STHISTIME Get_History_DirPowMaxDemandTime(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����޹��������
__EXTERN STHISVAL Get_History_DirVarMaxDemand(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����޹������������ʱ��
__EXTERN STHISTIME Get_History_DirVarMaxDemandTime(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����й��������
__EXTERN STHISVAL Get_History_UnDirPowMaxDemand(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����й������������ʱ��
__EXTERN STHISTIME Get_History_UnDirPowMaxDemandTime(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����޹��������
__EXTERN STHISVAL Get_History_UnDirVarMaxDemand(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����޹������������ʱ��
__EXTERN STHISTIME Get_History_UnDirVarMaxDemandTime(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);

__EXTERN STHISVAL Get_History_DirPowA(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
__EXTERN STHISVAL Get_History_DirPowB(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
__EXTERN STHISVAL Get_History_DirPowC(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
__EXTERN STHISVAL Get_History_DirPowALm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
__EXTERN STHISVAL Get_History_DirPowBLm(unsigned char ucMeterNo, unsigned char ucPos, unsigned char ucNo);
__EXTERN STHISVAL Get_History_DirPowCLm(unsigned char ucMeterNo, unsigned char ucPos, unsigned char ucNo);

//�����й����������£�
__EXTERN STHISVAL Get_History_DirPowerLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����й�����(����)
__EXTERN STHISVAL Get_History_UnDirPowerLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����޹�����(����)
__EXTERN STHISVAL Get_History_DirVarLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����޹�����(����)
__EXTERN STHISVAL Get_History_UnDirVarLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//1�����޹�����(����)
__EXTERN STHISVAL Get_History_DirVar1Lm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//2�����޹�����(����)
__EXTERN STHISVAL Get_History_DirVar2Lm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//3�����޹�����(����)
__EXTERN STHISVAL Get_History_DirVar3Lm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//4�����޹�����(����)
__EXTERN STHISVAL Get_History_DirVar4Lm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����й��������(����)
__EXTERN STHISVAL Get_History_DirPowMaxDemandLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����й������������ʱ��(����)
__EXTERN STHISTIME Get_History_DirPowMaxDemandTimeLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����޹��������(����)
__EXTERN STHISVAL Get_History_DirVarMaxDemandLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����޹������������ʱ��(����)
__EXTERN STHISTIME Get_History_DirVarMaxDemandTimeLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����й��������(����)
__EXTERN STHISVAL Get_History_UnDirPowMaxDemandLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����й������������ʱ��(����)
__EXTERN STHISTIME Get_History_UnDirPowMaxDemandTimeLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����޹��������(����)
__EXTERN STHISVAL Get_History_UnDirVarMaxDemandLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����޹������������ʱ��(����)
__EXTERN STHISTIME Get_History_UnDirVarMaxDemandTimeLm(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFeeNo);
//��ѹ
__EXTERN STHISVAL Get_History_V(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
//����
__EXTERN STHISVAL Get_History_I(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
//��ѹ���
__EXTERN STHISVAL Get_History_V_Phase(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
//�������
__EXTERN STHISVAL Get_History_I_Phase(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
//˲ʱ�й����޹�
__EXTERN STHISVAL Get_History_InstantPow(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
__EXTERN STHISVAL Get_History_InstantVar(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
//�й����޹����峣��
__EXTERN STHISVAL Get_History_PowPulseConst(unsigned char ucMeterNo,unsigned char ucPos);
__EXTERN STHISVAL Get_History_VarPulseConst(unsigned char ucMeterNo,unsigned char ucPos);
//����
__EXTERN STHISFEERATE Get_History_FeeRate(unsigned char ucMeterNo,unsigned char ucPos);
//���ʱ��
__EXTERN STHISTIME Get_History_ProgramTime(unsigned char ucMeterNo,unsigned char ucPos);
//��������ʱ��
__EXTERN STHISTIME Get_History_DemandClrTime(unsigned char ucMeterNo,unsigned char ucPos);
//��̴���
__EXTERN STHISVAL Get_History_ProgramCount(unsigned char ucMeterNo,unsigned char ucPos);
//�����������
__EXTERN STHISVAL Get_History_DemandClrCount(unsigned char ucMeterNo,unsigned char ucPos);
//�������ʱ��
__EXTERN STHISTIME Get_History_MeterClrTime(unsigned char ucMeterNo,unsigned char ucPos);
//�¼�����ʱ��
__EXTERN STHISTIME Get_History_EventClrTime(unsigned char ucMeterNo,unsigned char ucPos);
//Уʱʱ��
__EXTERN STHISTIME Get_History_JiaoshiTime(unsigned char ucMeterNo,unsigned char ucPos);
//����������
__EXTERN STHISVAL Get_History_MeterClrCount(unsigned char ucMeterNo,unsigned char ucPos);
//�¼��������
__EXTERN STHISVAL Get_History_EventClrCount(unsigned char ucMeterNo,unsigned char ucPos);
//Уʱ����
__EXTERN STHISVAL Get_History_JiaoshiCount(unsigned char ucMeterNo,unsigned char ucPos);
//��ع���ʱ��
__EXTERN STHISVAL Get_History_BatWorkTime(unsigned char ucMeterNo,unsigned char ucPos);
//�������״̬��
__EXTERN STHISSTATE Get_History_MeterStatFlag(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucFlagNum);
//��������״̬��
__EXTERN STHISSTATE Get_History_NetStatFlag(unsigned char ucMeterNo,unsigned char ucPos);
//������
__EXTERN STHISTIME Get_History_CopyDayTime(unsigned char ucMeterNo,unsigned char ucPos);
//�������
__EXTERN STHISVAL Get_History_VolBreakNum(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
//����ʱ���ۼ�ֵ
__EXTERN STHISVAL Get_History_VolBreakTime(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
//���һ�ζ�����ʼʱ��
__EXTERN STHISTIME Get_History_LastVolBreakBegTime(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
//���һ�ζ������ʱ��
__EXTERN STHISTIME Get_History_LastVolBreakEndTime(unsigned char ucMeterNo,unsigned char ucPos,unsigned char ucNo);
//������
__EXTERN STHISTIME Get_History_Date(unsigned char ucMeterNo,unsigned char ucPos);
//ʱ����
__EXTERN STHISTIME Get_History_Time(unsigned char ucMeterNo,unsigned char ucPos);

__EXTERN unsigned char Add_History_DirPowA(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo);
__EXTERN unsigned char Add_History_DirPowB(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo);
__EXTERN unsigned char Add_History_DirPowC(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo);
__EXTERN unsigned char Add_History_DirPowALm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo);
__EXTERN unsigned char Add_History_DirPowBLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo);
__EXTERN unsigned char Add_History_DirPowCLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo);
#endif

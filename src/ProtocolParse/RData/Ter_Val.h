/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Ter_Val.h
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-08-15    azh     the original version
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
//	STHISVAL	stHisDirPowFee4[CONHISTERDATANUM];				//	�����й�����
//
//	STHISVAL	stHisRevPowTotal[CONHISTERDATANUM];
//	STHISVAL	stHisRevPowFee1[CONHISTERDATANUM];
//	STHISVAL	stHisRevPowFee2[CONHISTERDATANUM];
//	STHISVAL	stHisRevPowFee3[CONHISTERDATANUM];
//	STHISVAL	stHisRevPowFee4[CONHISTERDATANUM];				//	�����й�����
//
//	STHISVAL	stHisDirVarTotal[CONHISTERDATANUM];
//	STHISVAL	stHisDirVarFee1[CONHISTERDATANUM];
//	STHISVAL	stHisDirVarFee2[CONHISTERDATANUM];
//	STHISVAL	stHisDirVarFee3[CONHISTERDATANUM];
//	STHISVAL	stHisDirVarFee4[CONHISTERDATANUM];				//	�����޹�����
//
//	STHISVAL	stHisRevVarTotal[CONHISTERDATANUM];
//	STHISVAL	stHisRevVarFee1[CONHISTERDATANUM];
//	STHISVAL	stHisRevVarFee2[CONHISTERDATANUM];
//	STHISVAL	stHisRevVarFee3[CONHISTERDATANUM];
//	STHISVAL	stHisRevVarFee4[CONHISTERDATANUM];				//	�����޹�����
//
//	STHISVAL	stHisDirVar1[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar1Fee1[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar1Fee2[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar1Fee3[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar1Fee4[CONHISTERDATANUM];				//	1�����޹�����
//
//	STHISVAL	stHisDirVar2[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar2Fee1[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar2Fee2[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar2Fee3[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar2Fee4[CONHISTERDATANUM];				//	2�����޹�����
//
//	STHISVAL	stHisDirVar3[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar3Fee1[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar3Fee2[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar3Fee3[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar3Fee4[CONHISTERDATANUM];				//	3�����޹�����
//
//	STHISVAL	stHisDirVar4[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar4Fee1[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar4Fee2[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar4Fee3[CONHISTERDATANUM];
//	STHISVAL	stHisDirVar4Fee4[CONHISTERDATANUM];				//	4�����޹�����
//
//	STHISVAL	stHisDirPowMaxDemand[CONHISTERDATANUM];
//	STHISVAL	stHisDirPowMaxDemandFee1[CONHISTERDATANUM];
//	STHISVAL	stHisDirPowMaxDemandFee2[CONHISTERDATANUM];
//	STHISVAL	stHisDirPowMaxDemandFee3[CONHISTERDATANUM];
//	STHISVAL	stHisDirPowMaxDemandFee4[CONHISTERDATANUM];		//	�����й��������
//
//	STHISTIME	stHisDirPowMaxDemandTime[CONHISTERDATANUM];
//	STHISTIME	stHisDirPowMaxDemandTimeFee1[CONHISTERDATANUM];
//	STHISTIME	stHisDirPowMaxDemandTimeFee2[CONHISTERDATANUM];
//	STHISTIME	stHisDirPowMaxDemandTimeFee3[CONHISTERDATANUM];
//	STHISTIME	stHisDirPowMaxDemandTimeFee4[CONHISTERDATANUM];	//	�����й������������ʱ��
//
//	STHISVAL	stHisDirVarMaxDemand[CONHISTERDATANUM];
//	STHISVAL	stHisDirVarMaxDemandFee1[CONHISTERDATANUM];
//	STHISVAL	stHisDirVarMaxDemandFee2[CONHISTERDATANUM];
//	STHISVAL	stHisDirVarMaxDemandFee3[CONHISTERDATANUM];
//	STHISVAL	stHisDirVarMaxDemandFee4[CONHISTERDATANUM];		//	�����޹��������
//
//	STHISTIME	stHisDirVarMaxDemandTime[CONHISTERDATANUM];
//	STHISTIME	stHisDirVarMaxDemandTimeFee1[CONHISTERDATANUM];
//	STHISTIME	stHisDirVarMaxDemandTimeFee2[CONHISTERDATANUM];
//	STHISTIME	stHisDirVarMaxDemandTimeFee3[CONHISTERDATANUM];
//	STHISTIME	stHisDirVarMaxDemandTimeFee4[CONHISTERDATANUM];	//	�����޹������������ʱ��
//
//	STHISVAL	stHisRevPowMaxDemand[CONHISTERDATANUM];
//	STHISVAL	stHisRevPowMaxDemandFee1[CONHISTERDATANUM];
//	STHISVAL	stHisRevPowMaxDemandFee2[CONHISTERDATANUM];
//	STHISVAL	stHisRevPowMaxDemandFee3[CONHISTERDATANUM];
//	STHISVAL	stHisRevPowMaxDemandFee4[CONHISTERDATANUM];		//	�����й��������
//
//	STHISTIME	stHisRevPowMaxDemandTime[CONHISTERDATANUM];
//	STHISTIME	stHisRevPowMaxDemandTimeFee1[CONHISTERDATANUM];
//	STHISTIME	stHisRevPowMaxDemandTimeFee2[CONHISTERDATANUM];
//	STHISTIME	stHisRevPowMaxDemandTimeFee3[CONHISTERDATANUM];
//	STHISTIME	stHisRevPowMaxDemandTimeFee4[CONHISTERDATANUM];	//	�����й������������ʱ��
//
//	STHISVAL	stHisRevVarMaxDemand[CONHISTERDATANUM];
//	STHISVAL	stHisRevVarMaxDemandFee1[CONHISTERDATANUM];
//	STHISVAL	stHisRevVarMaxDemandFee2[CONHISTERDATANUM];
//	STHISVAL	stHisRevVarMaxDemandFee3[CONHISTERDATANUM];
//	STHISVAL	stHisRevVarMaxDemandFee4[CONHISTERDATANUM];		//	�����޹��������
//
//	STHISTIME	stHisRevVarMaxDemandTime[CONHISTERDATANUM];
//	STHISTIME	stHisRevVarMaxDemandTimeFee1[CONHISTERDATANUM];
//	STHISTIME	stHisRevVarMaxDemandTimeFee2[CONHISTERDATANUM];
//	STHISTIME	stHisRevVarMaxDemandTimeFee3[CONHISTERDATANUM];
//	STHISTIME	stHisRevVarMaxDemandTimeFee4[CONHISTERDATANUM];//	�����޹������������ʱ��
//
//	STHISVAL	stHisVolA[CONHISTERDATANUM];
//	STHISVAL	stHisVolB[CONHISTERDATANUM];
//	STHISVAL	stHisVolC[CONHISTERDATANUM];					//	�����ѹ
//
//	STHISVAL	stHisVolA_Phase[CONHISTERDATANUM];
//	STHISVAL	stHisVolB_Phase[CONHISTERDATANUM];
//	STHISVAL	stHisVolC_Phase[CONHISTERDATANUM];				//	�����ѹ���
//
//	STHISVAL	stHisIa[CONHISTERDATANUM];
//	STHISVAL	stHisIb[CONHISTERDATANUM];
//	STHISVAL	stHisIc[CONHISTERDATANUM];						//	�������
//
//	STHISVAL	stHisIa_Phase[CONHISTERDATANUM];
//	STHISVAL	stHisIb_Phase[CONHISTERDATANUM];
//	STHISVAL	stHisIc_Phase[CONHISTERDATANUM];				//	����������
//
//	STHISVAL	stHisInstantPow[CONHISTERDATANUM];		
//	STHISVAL	stHisInstantPowA[CONHISTERDATANUM];	
//	STHISVAL	stHisInstantPowB[CONHISTERDATANUM];	
//	STHISVAL	stHisInstantPowC[CONHISTERDATANUM];				//	˲ʱ�й�
//
//	STHISVAL	stHisInstantVar[CONHISTERDATANUM];		
//	STHISVAL	stHisInstantVarA[CONHISTERDATANUM];	
//	STHISVAL	stHisInstantVarB[CONHISTERDATANUM];	
//	STHISVAL	stHisInstantVarC[CONHISTERDATANUM];				//	˲ʱ�޹�

//	STHISSTATE	stHisDoorStat[CONHISTERSWITCHNUM];				//	�Žڵ�״̬
	STHISSTATE	stHisPowerStat[CONHISTERSWITCHNUM];				//	����״̬
//	STHISSTATE	stHisSwitch0Stat[CONHISTERSWITCHNUM];			//	����1״̬
//	STHISSTATE	stHisSwitch1Stat[CONHISTERSWITCHNUM];			//	����2״̬
//	STHISSTATE	stHisSwitch2Stat[CONHISTERSWITCHNUM];			//	����3״̬
//	STHISSTATE	stHisSwitch3Stat[CONHISTERSWITCHNUM];			//	����4״̬
//	STHISSTATE	stHisSwitch4Stat[CONHISTERSWITCHNUM];			//	����5״̬
//	STHISSTATE	stHisSwitch5Stat[CONHISTERSWITCHNUM];			//	����6״̬
//	STHISSTATE	stHisSwitch6Stat[CONHISTERSWITCHNUM];			//	����7״̬

//	STHISSTATE	stHisPolReverseAStat[CONHISTERDATANUM];			//	A�෴����״̬
//	STHISSTATE	stHisPolReverseBStat[CONHISTERDATANUM];			//	B�෴����״̬
//	STHISSTATE	stHisPolReverseCStat[CONHISTERDATANUM];			//	C�෴����״̬

//	STHISSTATE	stHisShortCirA1Stat[CONHISTERDATANUM];			//	A��CT1�β��·
//	STHISSTATE	stHisShortCirB1Stat[CONHISTERDATANUM];			//	B��CT1�β��·
//	STHISSTATE	stHisShortCirC1Stat[CONHISTERDATANUM];			//	C��CT1�β��·
//
//	STHISSTATE	stHisOpenCirA2Stat[CONHISTERDATANUM];			//	A��CT2�β࿪·
//	STHISSTATE	stHisOpenCirB2Stat[CONHISTERDATANUM];			//	B��CT2�β࿪·
//	STHISSTATE	stHisOpenCirC2Stat[CONHISTERDATANUM];			//	C��CT2�β࿪·
//
//	STHISSTATE	stHisShortCirA2Stat[CONHISTERDATANUM];			//	A��CT2�β��·
//	STHISSTATE	stHisShortCirB2Stat[CONHISTERDATANUM];			//	B��CT2�β��·
//	STHISSTATE	stHisShortCirC2Stat[CONHISTERDATANUM];			//	C��CT2�β��·

//	STHISSTATE	stHisVolCirReveStat[CONHISTERDATANUM];			//	��ѹ��·������
}STTERTIMERVAL;

__EXTERN void	Init_TerHisData(STTERTIMERVAL *pstTerTimerVal);
__EXTERN void	Init_TerHisData_All(STTERTIMERVAL *pstTerTimerVal);
//
////�����й�����
//__EXTERN STHISVAL Get_History_DirPower_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////�����й�����
//__EXTERN STHISVAL Get_History_UnDirPower_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////�����޹�����
//__EXTERN STHISVAL Get_History_DirVar_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////�����޹�����
//__EXTERN STHISVAL Get_History_UnDirVar_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////1�����޹�����
//__EXTERN STHISVAL Get_History_DirVar1_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////2�����޹�����
//__EXTERN STHISVAL Get_History_DirVar2_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////3�����޹�����
//__EXTERN STHISVAL Get_History_DirVar3_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////4�����޹�����
//__EXTERN STHISVAL Get_History_DirVar4_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////�����й��������
//__EXTERN STHISVAL Get_History_DirPowMaxDemand_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////�����й������������ʱ��
//__EXTERN STHISTIME Get_History_DirPowMaxDemandTime_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////�����޹��������
//__EXTERN STHISVAL Get_History_DirVarMaxDemand_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////�����޹������������ʱ��
//__EXTERN STHISTIME Get_History_DirVarMaxDemandTime_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////�����й��������
//__EXTERN STHISVAL Get_History_UnDirPowMaxDemand_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////�����й������������ʱ��
//__EXTERN STHISTIME Get_History_UnDirPowMaxDemandTime_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////�����޹��������
//__EXTERN STHISVAL Get_History_UnDirVarMaxDemand_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////�����޹������������ʱ��
//__EXTERN STHISTIME Get_History_UnDirVarMaxDemandTime_Ter(unsigned char ucPos,unsigned char ucFeeNo);
////��ѹ
//__EXTERN STHISVAL Get_History_V_Disp_Ter(unsigned char ucPos,unsigned char ucNo);
////����
//__EXTERN STHISVAL Get_History_I_Disp_Ter(unsigned char ucPos,unsigned char ucNo);
////��ʾ��ѹ���
//__EXTERN STHISVAL Get_History_V_Phase_Disp_Ter(unsigned char ucPos,unsigned char ucNo);
////��ʾ�������
//__EXTERN STHISVAL Get_History_I_Phase_Disp_Ter(unsigned char ucPos,unsigned char ucNo);
////�޹�����
//__EXTERN STHISVAL Get_History_InstantVar_Disp_Ter(unsigned char ucPos,unsigned char ucNo);
////˲ʱ�й����޹�
//__EXTERN STHISVAL Get_History_InstantPow_Ter(unsigned char ucPos,unsigned char ucNo);
////������
//__EXTERN STHISSTATE Get_History_DoorStat_Ter(unsigned char ucPos);
__EXTERN STHISSTATE Get_History_PowerStat_Ter(unsigned char ucPos);
//__EXTERN STHISSTATE Get_History_SwitchStat_Ter(unsigned char ucPos,unsigned char ucSwitchNo);
////�澯��Ϣ��������������
//__EXTERN STHISSTATE Get_History_PolReverseStat_Ter(unsigned char ucPos,unsigned char ucNo);
////�澯��Ϣ����CTһ�β��·
//__EXTERN STHISSTATE Get_History_ShortCir1Stat_Ter(unsigned char ucPos,unsigned char ucNo);
////�澯��Ϣ����CT���β࿪·
//__EXTERN STHISSTATE Get_History_OpenCir2Stat_Ter(unsigned char ucPos,unsigned char ucNo);
////�澯��Ϣ����CT���β��·
//__EXTERN STHISSTATE Get_History_ShortCir2Stat_Ter(unsigned char ucPos,unsigned char ucNo);
////�澯��Ϣ����������
//__EXTERN STHISSTATE Get_History_VolCirReveStat_Ter(unsigned char ucPos);

///*******************************************�ն����ݻ�ȡ*************************************/
////�����й�����
//__EXTERN unsigned char Add_History_DirPower_Ter(unsigned long lVal,unsigned char ucFeeNo);
////�����й�����
//__EXTERN unsigned char Add_History_UnDirPower_Ter(unsigned long lVal,unsigned char ucFeeNo);
////�����޹�����
//__EXTERN unsigned char Add_History_DirVar_Ter(unsigned long lVal,unsigned char ucFeeNo);
////�����޹�����
//__EXTERN unsigned char Add_History_UnDirVar_Ter(unsigned long lVal,unsigned char ucFeeNo);
////1�����޹�
//__EXTERN unsigned char Add_History_DirVar1_Ter(unsigned long lVal,unsigned char ucFeeNo);
////2�����޹�
//__EXTERN unsigned char Add_History_DirVar2_Ter(unsigned long lVal,unsigned char ucFeeNo);
////3�����޹�
//__EXTERN unsigned char Add_History_DirVar3_Ter(unsigned long lVal,unsigned char ucFeeNo);
////4�����޹�
//__EXTERN unsigned char Add_History_DirVar4_Ter(unsigned long lVal,unsigned char ucFeeNo);
////�����й��������
//__EXTERN unsigned char Add_History_DirPowMaxDemand_Ter(unsigned long lVal,unsigned char ucFeeNo);
////�����й������������ʱ��
//__EXTERN unsigned char Add_History_DirPowMaxDemandTime_Ter(STDATETIME	stDateTime,unsigned char ucFeeNo);
////�����޹��������
//__EXTERN unsigned char Add_History_DirVarMaxDemand_Ter(unsigned long lVal,unsigned char ucFeeNo);
////�����޹������������ʱ��
//__EXTERN unsigned char Add_History_DirVarMaxDemandTime_Ter(STDATETIME	stDateTime,unsigned char ucFeeNo);
////�����й��������
//__EXTERN unsigned char Add_History_UnDirPowMaxDemand_Ter(unsigned long lVal,unsigned char ucFeeNo);
////�����й������������ʱ��
//__EXTERN unsigned char Add_History_UnDirPowMaxDemandTime_Ter(STDATETIME	stDateTime,unsigned char ucFeeNo);
////�����޹��������
//__EXTERN unsigned char Add_History_UnDirVarMaxDemand_Ter(unsigned long lVal,unsigned char ucFeeNo);
////�����޹������������ʱ��
//__EXTERN unsigned char Add_History_UnDirVarMaxDemandTime_Ter(STDATETIME	stDateTime,unsigned char ucFeeNo);
////��ѹ
//__EXTERN unsigned char Add_History_V_Ter(unsigned long lVal,unsigned char ucNo);
////��ѹ���
//__EXTERN unsigned char Add_History_V_Phase_Ter(unsigned long lVal,unsigned char ucNo);
////����
//__EXTERN unsigned char Add_History_I_Ter(unsigned long lVal,unsigned char ucNo);
////�������
//__EXTERN unsigned char Add_History_I_Phase_Ter(unsigned long lVal,unsigned char ucNo);
////˲ʱ�й�
//__EXTERN unsigned char Add_History_InstantPow_Ter(unsigned long lVal,unsigned char ucSign,unsigned char ucNo);
////˲ʱ�޹�
//__EXTERN unsigned char Add_History_InstantVar_Ter(unsigned long lVal,unsigned char ucSign,unsigned char ucNo);
////�Žڵ�״̬
////__EXTERN unsigned char Add_History_DoorStat_Ter(unsigned char	ucStateFlag);
//����״̬
__EXTERN unsigned char Add_History_PowerStat_Ter(unsigned char	ucStateFlag);
//����״̬
//__EXTERN unsigned char Add_History_SwitchStat_Ter(unsigned char	ucStateFlag,unsigned char ucSwithcNo);
//�澯��Ϣ��������������
//__EXTERN unsigned char  Add_History_PolReverseStat_Ter(unsigned char ucStateFlag,unsigned char ucNo);
////�澯��Ϣ����CTһ�β��·
//__EXTERN unsigned char  Add_History_ShortCir1Stat_Ter(unsigned char ucStateFlag,unsigned char ucNo);
////�澯��Ϣ����CT���β࿪·
//__EXTERN unsigned char  Add_History_OpenCir2Stat_Ter(unsigned char ucStateFlag,unsigned char ucNo);
////�澯��Ϣ����CT���β��·
//__EXTERN unsigned char  Add_History_ShortCir2Stat_Ter(unsigned char ucStateFlag,unsigned char ucNo);
////�澯��Ϣ����������
//__EXTERN unsigned char  Add_History_VolCirReveStat_Ter(unsigned char ucStateFlag);
#endif

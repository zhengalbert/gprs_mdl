/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Run_Data_Export.h
 Description    : ������������ ���ݽṹ����
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-08-05    azh     the original version
******************************************************************************/

//�������ݽṹ����
#ifndef _RUN_DATA_EXPORT_H_
#define _RUN_DATA_EXPORT_H_


#define		CONHISDATANUM		2				//��ǰ���ݵ��������ڵ�ǰ���ݱ��桢�澯������
#define		CONHISDATANUM_ADDLASTVALIDONE	(CONHISDATANUM+1)		//���������������Чһ��
#define		CONHISDATA_LASTVALID_POS	(CONHISDATANUM_ADDLASTVALIDONE-1)		//�����Чһ��

#define		CONPULSEHISDATANUM	2
#define		CONPULSECOUNTNUM	20
#define		CONHISHOURDATANUM		5				//��ǰ���ݵ��������ڵ�ǰ���ݱ��桢�澯������

#define		CON_SUMGROUP_NUM	8				//�ܼ�������

#define		CON_FEE_TOTAL		0x00
#define		CON_FEE_PLUS		0x01
#define		CON_FEE_APEX		0x02
#define		CON_FEE_FLAT		0x03
#define		CON_FEE_VALE		0x04

//��ʷֵ
typedef struct _STHISVAL
{
	STDATETIME	stDateTime;
	unsigned char ucSign;
	unsigned long lVal;
}STHISVAL;
//��ʷʱ��ֵ	
typedef struct _STHISTIME
{
	STDATETIME	stDateTime;
	STDATETIME	stHisTime;
}STHISTIME;
//��ʷ״ֵ̬
typedef struct _STHISSTATE			
{
	STDATETIME		stDateTime;
	unsigned char	ucStateFlag;
}STHISSTATE;
typedef struct _STFEERATE
{
	unsigned char ucFeeRate[3];
}STFEERATE;
typedef struct _STFEERATELIST
{
	unsigned char ucFeeRateNum;
	STFEERATE stFeeRate[8];
}STFEERATELIST;
typedef struct _STHISFEERATE		
{
	STDATETIME		stDateTime;
	STFEERATELIST stFeeRateList;
}STHISFEERATE;		//����

typedef struct _STSUMVALGROUP 
{
	float	fSumVal[CON_SUMGROUP_NUM];
}STSUMVALGROUP;
typedef struct _STHISSUMVALGROUP
{
	STDATETIME		stDateTime;
	STSUMVALGROUP	stSumValGroup;
}STHISSUMVALGROUP;	//�ܼ���

typedef struct _STMETERTIMERVAL
{
	STHISVAL	stHisDirPowTotal[CONHISDATANUM_ADDLASTVALIDONE];			//�����Ҫ�����Ч�й���������
	STHISVAL	stHisDirPowFee1[CONHISDATANUM_ADDLASTVALIDONE];
	STHISVAL	stHisDirPowFee2[CONHISDATANUM_ADDLASTVALIDONE];
	STHISVAL	stHisDirPowFee3[CONHISDATANUM_ADDLASTVALIDONE];
	STHISVAL	stHisDirPowFee4[CONHISDATANUM_ADDLASTVALIDONE];				//�����й�����

	STHISVAL	stHisRevPowTotal[CONHISDATANUM_ADDLASTVALIDONE];
	STHISVAL	stHisRevPowFee1[CONHISDATANUM_ADDLASTVALIDONE];
	STHISVAL	stHisRevPowFee2[CONHISDATANUM_ADDLASTVALIDONE];
	STHISVAL	stHisRevPowFee3[CONHISDATANUM_ADDLASTVALIDONE];
	STHISVAL	stHisRevPowFee4[CONHISDATANUM_ADDLASTVALIDONE];				//�����й�����

	STHISVAL	stHisDirVarTotal[CONHISDATANUM_ADDLASTVALIDONE];			//�����쳣��Ҫ��������޹���
	STHISVAL	stHisDirVarFee1[CONHISDATANUM];
	STHISVAL	stHisDirVarFee2[CONHISDATANUM];
	STHISVAL	stHisDirVarFee3[CONHISDATANUM];
	STHISVAL	stHisDirVarFee4[CONHISDATANUM];				//�����޹�����

	STHISVAL	stHisRevVarTotal[CONHISDATANUM];
	STHISVAL	stHisRevVarFee1[CONHISDATANUM];
	STHISVAL	stHisRevVarFee2[CONHISDATANUM];
	STHISVAL	stHisRevVarFee3[CONHISDATANUM];
	STHISVAL	stHisRevVarFee4[CONHISDATANUM];				//�����޹�����

	STHISVAL	stHisDirVar1[CONHISDATANUM];
	STHISVAL	stHisDirVar1Fee1[CONHISDATANUM];
	STHISVAL	stHisDirVar1Fee2[CONHISDATANUM];
	STHISVAL	stHisDirVar1Fee3[CONHISDATANUM];
	STHISVAL	stHisDirVar1Fee4[CONHISDATANUM];			//1�����޹�����

	STHISVAL	stHisDirVar2[CONHISDATANUM];
	STHISVAL	stHisDirVar2Fee1[CONHISDATANUM];
	STHISVAL	stHisDirVar2Fee2[CONHISDATANUM];
	STHISVAL	stHisDirVar2Fee3[CONHISDATANUM];
	STHISVAL	stHisDirVar2Fee4[CONHISDATANUM];			//2�����޹�����

	STHISVAL	stHisDirVar3[CONHISDATANUM];
	STHISVAL	stHisDirVar3Fee1[CONHISDATANUM];
	STHISVAL	stHisDirVar3Fee2[CONHISDATANUM];
	STHISVAL	stHisDirVar3Fee3[CONHISDATANUM];
	STHISVAL	stHisDirVar3Fee4[CONHISDATANUM];			//3�����޹�����

	STHISVAL	stHisDirVar4[CONHISDATANUM];
	STHISVAL	stHisDirVar4Fee1[CONHISDATANUM];
	STHISVAL	stHisDirVar4Fee2[CONHISDATANUM];
	STHISVAL	stHisDirVar4Fee3[CONHISDATANUM];
	STHISVAL	stHisDirVar4Fee4[CONHISDATANUM];			//4�����޹�����

	STHISVAL	stHisDirPowMaxDemand[CONHISDATANUM];
	STHISVAL	stHisDirPowMaxDemandFee1[CONHISDATANUM];
	STHISVAL	stHisDirPowMaxDemandFee2[CONHISDATANUM];
	STHISVAL	stHisDirPowMaxDemandFee3[CONHISDATANUM];
	STHISVAL	stHisDirPowMaxDemandFee4[CONHISDATANUM];	//�����й��������

	STHISTIME	stHisDirPowMaxDemandTime[CONHISDATANUM];
	STHISTIME	stHisDirPowMaxDemandTimeFee1[CONHISDATANUM];
	STHISTIME	stHisDirPowMaxDemandTimeFee2[CONHISDATANUM];
	STHISTIME	stHisDirPowMaxDemandTimeFee3[CONHISDATANUM];
	STHISTIME	stHisDirPowMaxDemandTimeFee4[CONHISDATANUM];//�����й������������ʱ��

	STHISVAL	stHisDirVarMaxDemand[CONHISDATANUM];
	STHISVAL	stHisDirVarMaxDemandFee1[CONHISDATANUM];
	STHISVAL	stHisDirVarMaxDemandFee2[CONHISDATANUM];
	STHISVAL	stHisDirVarMaxDemandFee3[CONHISDATANUM];
	STHISVAL	stHisDirVarMaxDemandFee4[CONHISDATANUM];	//�����޹��������

	STHISTIME	stHisDirVarMaxDemandTime[CONHISDATANUM];
	STHISTIME	stHisDirVarMaxDemandTimeFee1[CONHISDATANUM];
	STHISTIME	stHisDirVarMaxDemandTimeFee2[CONHISDATANUM];
	STHISTIME	stHisDirVarMaxDemandTimeFee3[CONHISDATANUM];
	STHISTIME	stHisDirVarMaxDemandTimeFee4[CONHISDATANUM];//�����޹������������ʱ��

	STHISVAL	stHisRevPowMaxDemand[CONHISDATANUM];
	STHISVAL	stHisRevPowMaxDemandFee1[CONHISDATANUM];
	STHISVAL	stHisRevPowMaxDemandFee2[CONHISDATANUM];
	STHISVAL	stHisRevPowMaxDemandFee3[CONHISDATANUM];
	STHISVAL	stHisRevPowMaxDemandFee4[CONHISDATANUM];	//�����й��������

	STHISTIME	stHisRevPowMaxDemandTime[CONHISDATANUM];
	STHISTIME	stHisRevPowMaxDemandTimeFee1[CONHISDATANUM];
	STHISTIME	stHisRevPowMaxDemandTimeFee2[CONHISDATANUM];
	STHISTIME	stHisRevPowMaxDemandTimeFee3[CONHISDATANUM];
	STHISTIME	stHisRevPowMaxDemandTimeFee4[CONHISDATANUM];//�����й������������ʱ��

	STHISVAL	stHisRevVarMaxDemand[CONHISDATANUM];
	STHISVAL	stHisRevVarMaxDemandFee1[CONHISDATANUM];
	STHISVAL	stHisRevVarMaxDemandFee2[CONHISDATANUM];
	STHISVAL	stHisRevVarMaxDemandFee3[CONHISDATANUM];
	STHISVAL	stHisRevVarMaxDemandFee4[CONHISDATANUM];	//�����޹��������

	STHISTIME	stHisRevVarMaxDemandTime[CONHISDATANUM];
	STHISTIME	stHisRevVarMaxDemandTimeFee1[CONHISDATANUM];
	STHISTIME	stHisRevVarMaxDemandTimeFee2[CONHISDATANUM];
	STHISTIME	stHisRevVarMaxDemandTimeFee3[CONHISDATANUM];
	STHISTIME	stHisRevVarMaxDemandTimeFee4[CONHISDATANUM];//�����޹������������ʱ��

	STHISVAL 	stHisDirPowA[CONHISDATANUM];
	STHISVAL 	stHisRevDirPowA[CONHISDATANUM];
	STHISVAL 	stHisComVar1A[CONHISDATANUM];
	STHISVAL 	stHisComVar2A[CONHISDATANUM];
	
	STHISVAL 	stHisDirPowB[CONHISDATANUM];
	STHISVAL 	stHisRevDirPowB[CONHISDATANUM];
	STHISVAL 	stHisComVar1B[CONHISDATANUM];
	STHISVAL 	stHisComVar2B[CONHISDATANUM];
	
	STHISVAL 	stHisDirPowC[CONHISDATANUM];
	STHISVAL 	stHisRevDirPowC[CONHISDATANUM];
	STHISVAL 	stHisComVar1C[CONHISDATANUM];
	STHISVAL 	stHisComVar2C[CONHISDATANUM];
	
	STHISVAL 	stHisDirPowALm[CONHISDATANUM];
	STHISVAL 	stHisRevDirPowALm[CONHISDATANUM];
	STHISVAL 	stHisComVar1ALm[CONHISDATANUM];
	STHISVAL 	stHisComVar2ALm[CONHISDATANUM];
	
	STHISVAL 	stHisDirPowBLm[CONHISDATANUM];
	STHISVAL 	stHisRevDirPowBLm[CONHISDATANUM];
	STHISVAL 	stHisComVar1BLm[CONHISDATANUM];
	STHISVAL 	stHisComVar2BLm[CONHISDATANUM];
	
	STHISVAL 	stHisDirPowCLm[CONHISDATANUM];
	STHISVAL 	stHisRevDirPowCLm[CONHISDATANUM];
	STHISVAL 	stHisComVar1CLm[CONHISDATANUM];
	STHISVAL 	stHisComVar2CLm[CONHISDATANUM];
	
	STHISVAL	stHisDirPowLmTotal[CONHISDATANUM];		
	STHISVAL	stHisDirPowLmFee1[CONHISDATANUM];		
	STHISVAL	stHisDirPowLmFee2[CONHISDATANUM];		
	STHISVAL	stHisDirPowLmFee3[CONHISDATANUM];		
	STHISVAL	stHisDirPowLmFee4[CONHISDATANUM];			//�����й����������£�

	STHISVAL	stHisRevPowLmTotal[CONHISDATANUM];
	STHISVAL	stHisRevPowLmFee1[CONHISDATANUM];
	STHISVAL	stHisRevPowLmFee2[CONHISDATANUM];
	STHISVAL	stHisRevPowLmFee3[CONHISDATANUM];
	STHISVAL	stHisRevPowLmFee4[CONHISDATANUM];			//�����й�����(����)

	STHISVAL	stHisDirVarLmTotal[CONHISDATANUM];
	STHISVAL	stHisDirVarLmFee1[CONHISDATANUM];
	STHISVAL	stHisDirVarLmFee2[CONHISDATANUM];
	STHISVAL	stHisDirVarLmFee3[CONHISDATANUM];
	STHISVAL	stHisDirVarLmFee4[CONHISDATANUM];			//�����޹�����(����)

	STHISVAL	stHisRevVarLmTotal[CONHISDATANUM];
	STHISVAL	stHisRevVarLmFee1[CONHISDATANUM];
	STHISVAL	stHisRevVarLmFee2[CONHISDATANUM];
	STHISVAL	stHisRevVarLmFee3[CONHISDATANUM];
	STHISVAL	stHisRevVarLmFee4[CONHISDATANUM];			//�����޹�����(����)

	STHISVAL	stHisDirVar1Lm[CONHISDATANUM];
	STHISVAL	stHisDirVar1LmFee1[CONHISDATANUM];
	STHISVAL	stHisDirVar1LmFee2[CONHISDATANUM];
	STHISVAL	stHisDirVar1LmFee3[CONHISDATANUM];
	STHISVAL	stHisDirVar1LmFee4[CONHISDATANUM];			//1�����޹�����(����)

	STHISVAL	stHisDirVar2Lm[CONHISDATANUM];
	STHISVAL	stHisDirVar2LmFee1[CONHISDATANUM];
	STHISVAL	stHisDirVar2LmFee2[CONHISDATANUM];
	STHISVAL	stHisDirVar2LmFee3[CONHISDATANUM];
	STHISVAL	stHisDirVar2LmFee4[CONHISDATANUM];			//2�����޹�����(����)
	STHISVAL	stHisDirVar3Lm[CONHISDATANUM];
	STHISVAL	stHisDirVar3LmFee1[CONHISDATANUM];
	STHISVAL	stHisDirVar3LmFee2[CONHISDATANUM];
	STHISVAL	stHisDirVar3LmFee3[CONHISDATANUM];
	STHISVAL	stHisDirVar3LmFee4[CONHISDATANUM];			//3�����޹�����(����)
		
	STHISVAL	stHisDirVar4Lm[CONHISDATANUM];
	STHISVAL	stHisDirVar4LmFee1[CONHISDATANUM];
	STHISVAL	stHisDirVar4LmFee2[CONHISDATANUM];
	STHISVAL	stHisDirVar4LmFee3[CONHISDATANUM];
	STHISVAL	stHisDirVar4LmFee4[CONHISDATANUM];				//4�����޹�����(����)
	
	STHISVAL	stHisDirPowMaxDemandLm[CONHISDATANUM];
	STHISVAL	stHisDirPowMaxDemandLmFee1[CONHISDATANUM];
	STHISVAL	stHisDirPowMaxDemandLmFee2[CONHISDATANUM];
	STHISVAL	stHisDirPowMaxDemandLmFee3[CONHISDATANUM];
	STHISVAL	stHisDirPowMaxDemandLmFee4[CONHISDATANUM];		//�����й��������(����)

	STHISTIME	stHisDirPowMaxDemandTimeLm[CONHISDATANUM];
	STHISTIME	stHisDirPowMaxDemandTimeLmFee1[CONHISDATANUM];
	STHISTIME	stHisDirPowMaxDemandTimeLmFee2[CONHISDATANUM];
	STHISTIME	stHisDirPowMaxDemandTimeLmFee3[CONHISDATANUM];
	STHISTIME	stHisDirPowMaxDemandTimeLmFee4[CONHISDATANUM];	//�����й������������ʱ��(����)

	STHISVAL	stHisDirVarMaxDemandLm[CONHISDATANUM];
	STHISVAL	stHisDirVarMaxDemandLmFee1[CONHISDATANUM];
	STHISVAL	stHisDirVarMaxDemandLmFee2[CONHISDATANUM];
	STHISVAL	stHisDirVarMaxDemandLmFee3[CONHISDATANUM];
	STHISVAL	stHisDirVarMaxDemandLmFee4[CONHISDATANUM];		//�����޹��������(����)

	STHISTIME	stHisDirVarMaxDemandTimeLm[CONHISDATANUM];
	STHISTIME	stHisDirVarMaxDemandTimeLmFee1[CONHISDATANUM];
	STHISTIME	stHisDirVarMaxDemandTimeLmFee2[CONHISDATANUM];
	STHISTIME	stHisDirVarMaxDemandTimeLmFee3[CONHISDATANUM];
	STHISTIME	stHisDirVarMaxDemandTimeLmFee4[CONHISDATANUM];	//�����޹������������ʱ��(����)

	STHISVAL	stHisRevPowMaxDemandLm[CONHISDATANUM];
	STHISVAL	stHisRevPowMaxDemandLmFee1[CONHISDATANUM];
	STHISVAL	stHisRevPowMaxDemandLmFee2[CONHISDATANUM];
	STHISVAL	stHisRevPowMaxDemandLmFee3[CONHISDATANUM];
	STHISVAL	stHisRevPowMaxDemandLmFee4[CONHISDATANUM];		//�����й��������(����)

	STHISTIME	stHisRevPowMaxDemandTimeLm[CONHISDATANUM];
	STHISTIME	stHisRevPowMaxDemandTimeLmFee1[CONHISDATANUM];
	STHISTIME	stHisRevPowMaxDemandTimeLmFee2[CONHISDATANUM];
	STHISTIME	stHisRevPowMaxDemandTimeLmFee3[CONHISDATANUM];
	STHISTIME	stHisRevPowMaxDemandTimeLmFee4[CONHISDATANUM];	//�����й������������ʱ��(����)

	STHISVAL	stHisRevVarMaxDemandLm[CONHISDATANUM];
	STHISVAL	stHisRevVarMaxDemandLmFee1[CONHISDATANUM];
	STHISVAL	stHisRevVarMaxDemandLmFee2[CONHISDATANUM];
	STHISVAL	stHisRevVarMaxDemandLmFee3[CONHISDATANUM];
	STHISVAL	stHisRevVarMaxDemandLmFee4[CONHISDATANUM];		//�����޹��������(����)

	STHISTIME	stHisRevVarMaxDemandTimeLm[CONHISDATANUM];
	STHISTIME	stHisRevVarMaxDemandTimeLmFee1[CONHISDATANUM];
	STHISTIME	stHisRevVarMaxDemandTimeLmFee2[CONHISDATANUM];
	STHISTIME	stHisRevVarMaxDemandTimeLmFee3[CONHISDATANUM];
	STHISTIME	stHisRevVarMaxDemandTimeLmFee4[CONHISDATANUM];	//�����޹������������ʱ��(����)

	STHISVAL	stHisVolA[CONHISDATANUM];
	STHISVAL	stHisVolB[CONHISDATANUM];
	STHISVAL	stHisVolC[CONHISDATANUM];				//�����ѹ

	STHISVAL	stHisVolA_Phase[CONHISDATANUM];
	STHISVAL	stHisVolB_Phase[CONHISDATANUM];
	STHISVAL	stHisVolC_Phase[CONHISDATANUM];			//�����ѹ���

	STHISVAL	stHisIa[CONHISDATANUM];
	STHISVAL	stHisIb[CONHISDATANUM];
	STHISVAL	stHisIc[CONHISDATANUM];					//�������

	STHISVAL	stHisIa_Phase[CONHISDATANUM];
	STHISVAL	stHisIb_Phase[CONHISDATANUM];
	STHISVAL	stHisIc_Phase[CONHISDATANUM];			//����������

	STHISVAL	stHisInstantPow[CONHISDATANUM];		
	STHISVAL	stHisInstantPowA[CONHISDATANUM];	
	STHISVAL	stHisInstantPowB[CONHISDATANUM];	
	STHISVAL	stHisInstantPowC[CONHISDATANUM];		//˲ʱ�й�

	STHISVAL	stHisInstantVar[CONHISDATANUM];		
	STHISVAL	stHisInstantVarA[CONHISDATANUM];	
	STHISVAL	stHisInstantVarB[CONHISDATANUM];	
	STHISVAL	stHisInstantVarC[CONHISDATANUM];		//˲ʱ�޹�

	STHISVAL	stHisPowPulseConst[CONHISDATANUM];
	STHISVAL	stHisVarPulseConst[CONHISDATANUM];			//���峣��

	STHISFEERATE stHisFeeRate[CONHISDATANUM];				//����

	STHISTIME	stHisProgramTime[CONHISDATANUM];			//���ʱ��
	STHISTIME	stHisMeterClrTime[CONHISDATANUM];			//�������ʱ��
	STHISTIME	stHisDemandClrTime[CONHISDATANUM];			//��������ʱ��
	STHISTIME	stHisEventClrTime[CONHISDATANUM];			//�¼�����ʱ��
	STHISTIME	stHisJiaoshiTime[CONHISDATANUM];			//Уʱʱ��
	
	STHISVAL	stHisProgramCount[CONHISDATANUM];			//��̴���
	STHISVAL	stHisMeterClrCount[CONHISDATANUM];			//����������
	STHISVAL	stHisDemandClrCount[CONHISDATANUM];			//�����������
	STHISVAL	stHisEventClrCount[CONHISDATANUM];			//�¼��������
	STHISVAL	stHisJiaoshiCount[CONHISDATANUM];			//Уʱ����
	STHISVAL	stHisBatWorkTime[CONHISDATANUM];			//��ع���ʱ��
	

	STHISSTATE	stHisMeterStatFlag1[CONHISDATANUM];			//�������״̬��
	STHISSTATE	stHisMeterStatFlag2[CONHISDATANUM];			//��������״̬��
	STHISSTATE	stHisMeterStatFlag3[CONHISDATANUM];			//��������״̬��
	STHISSTATE	stHisMeterStatFlag4[CONHISDATANUM];			//��������״̬��
	STHISSTATE	stHisMeterStatFlag5[CONHISDATANUM];			//��������״̬��
	STHISSTATE	stHisMeterStatFlag6[CONHISDATANUM];			//��������״̬��
	STHISSTATE	stHisMeterStatFlag7[CONHISDATANUM];			//��������״̬��

	STHISTIME	stHisCopyDayTime[CONHISDATANUM];			//������

	STHISVAL	stHisLostVolNum[CONHISDATANUM];				//�������
	STHISVAL	stHisLostVolANum[CONHISDATANUM];			//A��������
	STHISVAL	stHisLostVolBNum[CONHISDATANUM];			//B��������
	STHISVAL	stHisLostVolCNum[CONHISDATANUM];			//C��������

	STHISVAL	stHisLostVolTime[CONHISDATANUM];			//����ʱ���ۼ�ֵ
	STHISVAL	stHisLostVolATime[CONHISDATANUM];			//A�����ʱ���ۼ�ֵ
	STHISVAL	stHisLostVolBTime[CONHISDATANUM];			//B�����ʱ���ۼ�ֵ
	STHISVAL	stHisLostVolCTime[CONHISDATANUM];			//C�����ʱ���ۼ�ֵ

	STHISTIME	stHisLastVolOffBegTime[CONHISDATANUM];		//���һ�ζ�����ʼʱ��
	STHISTIME	stHisLastVolAOffBegTime[CONHISDATANUM];		//A�����һ�ζ�����ʼʱ��
	STHISTIME	stHisLastVolBOffBegTime[CONHISDATANUM];		//B�����һ�ζ�����ʼʱ��
	STHISTIME	stHisLastVolCOffBegTime[CONHISDATANUM];		//C�����һ�ζ�����ʼʱ��

	STHISTIME	stHisLastVolOffEndTime[CONHISDATANUM];		//���һ�ζ������ʱ��
	STHISTIME	stHisLastVolAOffEndTime[CONHISDATANUM];		//A�����һ�ζ������ʱ��
	STHISTIME	stHisLastVolBOffEndTime[CONHISDATANUM];		//B�����һ�ζ������ʱ��
	STHISTIME	stHisLastVolCOffEndTime[CONHISDATANUM];		//C�����һ�ζ������ʱ��

	STHISTIME	stHisDate[CONHISDATANUM];					//��Ƶ�������
	STHISTIME	stHisTime[CONHISDATANUM];					//��Ƶ�ʱ����

}STMETERTIMERVAL;

typedef struct _STMETERSUMVALGROUP
{
	STHISSUMVALGROUP	stInstantPowSumGroup[CONHISDATANUM];	//˲ʱ�й��ܼ�
	STHISSUMVALGROUP	stInstantVarSumGroup[CONHISDATANUM];	//˲ʱ�޹��ܼ�

	STHISSUMVALGROUP	stPowValueSumGroup[CONHISDATANUM];		//�й������ܼ�
	STHISSUMVALGROUP	stPowFee1ValueSumGroup[CONHISDATANUM];	//�й������ܼ�
	STHISSUMVALGROUP	stPowFee2ValueSumGroup[CONHISDATANUM];	//�й������ܼ�
	STHISSUMVALGROUP	stPowFee3ValueSumGroup[CONHISDATANUM];	//�й������ܼ�
	STHISSUMVALGROUP	stPowFee4ValueSumGroup[CONHISDATANUM];	//�й������ܼ�

	STHISSUMVALGROUP	stVarValueSumGroup[CONHISDATANUM];		//�޹������ܼ�
	STHISSUMVALGROUP	stVarFee1ValueSumGroup[CONHISDATANUM];	//�޹������ܼ�
	STHISSUMVALGROUP	stVarFee2ValueSumGroup[CONHISDATANUM];	//�޹������ܼ�
	STHISSUMVALGROUP	stVarFee3ValueSumGroup[CONHISDATANUM];	//�޹������ܼ�
	STHISSUMVALGROUP	stVarFee4ValueSumGroup[CONHISDATANUM];	//�޹������ܼ�
	
	STHISSUMVALGROUP	stPowHourSumGroup[CONHISHOURDATANUM];	//�й������ܼ�
}STMETERSUMVALGROUP;



typedef	struct	_STHISRUNDATA					//ͳ������
{
	STDATETIME	stRecordTime;
	unsigned short	usReserve0;

	unsigned long	ulHisDirPowTotal;
	unsigned long	ulHisDirPowFee1;
	unsigned long	ulHisDirPowFee2;
	unsigned long	ulHisDirPowFee3;
	unsigned long	ulHisDirPowFee4;				//�����й�����

	unsigned long	ulHisRevPowTotal;
	unsigned long	ulHisRevPowFee1;
	unsigned long	ulHisRevPowFee2;
	unsigned long	ulHisRevPowFee3;
	unsigned long	ulHisRevPowFee4;				//�����й�����

	unsigned long	ulHisDirVarTotal;
	unsigned long	ulHisDirVarFee1;
	unsigned long	ulHisDirVarFee2;
	unsigned long	ulHisDirVarFee3;
	unsigned long	ulHisDirVarFee4;				//�����޹�����

	unsigned long	ulHisRevVarTotal;
	unsigned long	ulHisRevVarFee1;
	unsigned long	ulHisRevVarFee2;
	unsigned long	ulHisRevVarFee3;
	unsigned long	ulHisRevVarFee4;				//�����޹�����

	unsigned long	ulHisLastDayDirPowTotal;
	unsigned long	ulHisLastDayDirPowFee1;
	unsigned long	ulHisLastDayDirPowFee2;
	unsigned long	ulHisLastDayDirPowFee3;
	unsigned long	ulHisLastDayDirPowFee4;				//��һ�������й�����

	unsigned long	ulHisLastDayRevPowTotal;
	unsigned long	ulHisLastDayRevPowFee1;
	unsigned long	ulHisLastDayRevPowFee2;
	unsigned long	ulHisLastDayRevPowFee3;
	unsigned long	ulHisLastDayRevPowFee4;				//��һ�췴���й�����

	unsigned long	ulHisLastDayDirVarTotal;
	unsigned long	ulHisLastDayDirVarFee1;
	unsigned long	ulHisLastDayDirVarFee2;
	unsigned long	ulHisLastDayDirVarFee3;
	unsigned long	ulHisLastDayDirVarFee4;				//��һ�������޹�����

	unsigned long	ulHisLastDayRevVarTotal;
	unsigned long	ulHisLastDayRevVarFee1;
	unsigned long	ulHisLastDayRevVarFee2;
	unsigned long	ulHisLastDayRevVarFee3;
	unsigned long	ulHisLastDayRevVarFee4;				//��һ�췴���޹�����

	unsigned long	ulHisDirVar1;
	unsigned long	ulHisDirVar1Fee1;
	unsigned long	ulHisDirVar1Fee2;
	unsigned long	ulHisDirVar1Fee3;
	unsigned long	ulHisDirVar1Fee4;				//1�����޹�����

	unsigned long	ulHisDirVar2;
	unsigned long	ulHisDirVar2Fee1;
	unsigned long	ulHisDirVar2Fee2;
	unsigned long	ulHisDirVar2Fee3;
	unsigned long	ulHisDirVar2Fee4;				//2�����޹�����

	unsigned long	ulHisDirVar3;
	unsigned long	ulHisDirVar3Fee1;
	unsigned long	ulHisDirVar3Fee2;
	unsigned long	ulHisDirVar3Fee3;
	unsigned long	ulHisDirVar3Fee4;				//3�����޹�����

	unsigned long	ulHisDirVar4;
	unsigned long	ulHisDirVar4Fee1;
	unsigned long	ulHisDirVar4Fee2;
	unsigned long	ulHisDirVar4Fee3;
	unsigned long	ulHisDirVar4Fee4;				//4�����޹�����

	unsigned long ulHisDirPowA;
	unsigned long ulHisDirPowB;
	unsigned long ulHisDirPowC;
	
	unsigned long ulHisUnDirPowA;
	unsigned long ulHisUnDirPowB;
	unsigned long ulHisUnDirPowC;
	
	unsigned long ulHisDirVarA;
	unsigned long ulHisDirVarB;
	unsigned long ulHisDirVarC;
	
	unsigned long ulHisUnDirVarA;	
	unsigned long ulHisUnDirVarB;	
	unsigned long ulHisUnDirVarC;

	unsigned long	ulHisDirPowMaxDemand;
	unsigned long	ulHisDirPowMaxDemandFee1;
	unsigned long	ulHisDirPowMaxDemandFee2;
	unsigned long	ulHisDirPowMaxDemandFee3;
	unsigned long	ulHisDirPowMaxDemandFee4;		//�����й��������

	unsigned long	ulHisDirVarMaxDemand;
	unsigned long	ulHisDirVarMaxDemandFee1;
	unsigned long	ulHisDirVarMaxDemandFee2;
	unsigned long	ulHisDirVarMaxDemandFee3;
	unsigned long	ulHisDirVarMaxDemandFee4;		//�����޹��������

	unsigned long	ulHisRevPowMaxDemand;
	unsigned long	ulHisRevPowMaxDemandFee1;
	unsigned long	ulHisRevPowMaxDemandFee2;
	unsigned long	ulHisRevPowMaxDemandFee3;
	unsigned long	ulHisRevPowMaxDemandFee4;		//�����й��������

	unsigned long	ulHisRevVarMaxDemand;
	unsigned long	ulHisRevVarMaxDemandFee1;
	unsigned long	ulHisRevVarMaxDemandFee2;
	unsigned long	ulHisRevVarMaxDemandFee3;
	unsigned long	ulHisRevVarMaxDemandFee4;		//�����޹��������

	STDATETIME	stHisDirPowMaxDemandTime;
	STDATETIME	stHisDirPowMaxDemandTimeFee1;
	STDATETIME	stHisDirPowMaxDemandTimeFee2;
	STDATETIME	stHisDirPowMaxDemandTimeFee3;
	STDATETIME	stHisDirPowMaxDemandTimeFee4;	//�����й������������ʱ��

	STDATETIME	stHisDirVarMaxDemandTime;
	STDATETIME	stHisDirVarMaxDemandTimeFee1;
	STDATETIME	stHisDirVarMaxDemandTimeFee2;
	STDATETIME	stHisDirVarMaxDemandTimeFee3;
	STDATETIME	stHisDirVarMaxDemandTimeFee4;	//�����޹������������ʱ��

	STDATETIME	stHisRevPowMaxDemandTime;
	STDATETIME	stHisRevPowMaxDemandTimeFee1;
	STDATETIME	stHisRevPowMaxDemandTimeFee2;
	STDATETIME	stHisRevPowMaxDemandTimeFee3;
	STDATETIME	stHisRevPowMaxDemandTimeFee4;	//�����й������������ʱ��

	STDATETIME	stHisRevVarMaxDemandTime;
	STDATETIME	stHisRevVarMaxDemandTimeFee1;
	STDATETIME	stHisRevVarMaxDemandTimeFee2;
	STDATETIME	stHisRevVarMaxDemandTimeFee3;
	STDATETIME	stHisRevVarMaxDemandTimeFee4;	//�����޹������������ʱ��

	STHISVAL	stHisMaxInstantPow;				//˲ʱ�й�ͳ��
	STHISVAL	stHisMaxInstantPowA;			//����STHISVAL�ṹ������STHISVAL.stDateTime��ʾ���ֵ����ʱ�䣬�����ʾ��ͬ
	STHISVAL	stHisMaxInstantPowB;	
	STHISVAL	stHisMaxInstantPowC;				
	unsigned short	usHisZeroTimeInstantPow;
	unsigned short	usHisZeroTimeInstantPowA;	
	unsigned short	usHisZeroTimeInstantPowB;	
	unsigned short	usHisZeroTimeInstantPowC;

	STHISVAL	stHisMaxDemand;					//���Լ���������
	STHISVAL	stHisMaxDemandA;	
	STHISVAL	stHisMaxDemandB;	
	STHISVAL	stHisMaxDemandC;				

	STHISVAL	stHisMaxVolA;					//��ѹͳ��
	STHISVAL	stHisMaxVolB;
	STHISVAL	stHisMaxVolC;				
	STHISVAL	stHisMinVolA;
	STHISVAL	stHisMinVolB;
	STHISVAL	stHisMinVolC;
	unsigned short	usHisNormalTimeVolA;
	unsigned short	usHisNormalTimeVolB;
	unsigned short	usHisNormalTimeVolC;
	unsigned short	usHisErrUpTimeVolA;
	unsigned short	usHisErrUpTimeVolB;
	unsigned short	usHisErrUpTimeVolC;
	unsigned short	usHisNorUpTimeVolA;
	unsigned short	usHisNorUpTimeVolB;
	unsigned short	usHisNorUpTimeVolC;
	unsigned short	usHisErrDownTimeVolA;
	unsigned short	usHisErrDownTimeVolB;
	unsigned short	usHisErrDownTimeVolC;
	unsigned short	usHisNorDownTimeVolA;
	unsigned short	usHisNorDownTimeVolB;
	unsigned short	usHisNorDownTimeVolC;
	unsigned long	ulHisSumVolA;				//��ѹ�ۼӣ�����ƽ����ѹ����
	unsigned long	ulHisSumVolB;
	unsigned long	ulHisSumVolC;

	STHISVAL	stHisMaxCurrA;					//����ͳ��
	STHISVAL	stHisMaxCurrB;
	STHISVAL	stHisMaxCurrC;				
	STHISVAL	stHisMaxCurrN;
	unsigned short	usHisErrUpTimeCurrA;
	unsigned short	usHisErrUpTimeCurrB;
	unsigned short	usHisErrUpTimeCurrC;
	unsigned short	usHisNorUpTimeCurrA;
	unsigned short	usHisNorUpTimeCurrB;
	unsigned short	usHisNorUpTimeCurrC;
	unsigned short	usHisNorUpTimeCurrN;

	unsigned short	usHisUnbalanceTimeVolt;		//��ѹ��ƽ��ʱ��
	unsigned short	usHisUnbalanceTimeCurr;		//������ƽ��ʱ��
	STHISVAL 		stHisMaxUnBalanceVol;
	STHISVAL		stHisMaxUnBalanceCur;

	unsigned short	usHisErrUpTimeSPower;		//���ڹ���Խ������ʱ��
	unsigned short	usHisNorUpTimeSPower;		//���ڹ���Խ����ʱ��

	STHISVAL 		stHisMaxLoadRate;			//�������
	STHISVAL 		stHisMinLoadRate;			//��С������

	unsigned short usHisVolBreakNum;			//�������
	unsigned short usHisVolABreakNum;			//A��������
	unsigned short usHisVolBBreakNum;			//B��������
	unsigned short usHisVolCBreakNum;			//C��������
	unsigned short usHisVolBreakTime;			//����ʱ���ۼ�ֵ
	unsigned short usHisVolABreakTime;			//A�����ʱ���ۼ�ֵ
	unsigned short usHisVolBBreakTime;			//B�����ʱ���ۼ�ֵ
	unsigned short usHisVolCBreakTime;			//C�����ʱ���ۼ�ֵ
	STDATETIME	stHisLastVolBreakBegTime;		//���һ�ζ�����ʼʱ��
	STDATETIME	stHisLastVolABreakBegTime;		//A�����һ�ζ�����ʼʱ��
	STDATETIME	stHisLastVolBBreakBegTime;		//B�����һ�ζ�����ʼʱ��
	STDATETIME	stHisLastVolCBreakBegTime;		//C�����һ�ζ�����ʼʱ��
	STDATETIME	stHisLastVolBreakEndTime;		//���һ�ζ������ʱ��
	STDATETIME	stHisLastVolABreakEndTime;		//A�����һ�ζ������ʱ��
	STDATETIME	stHisLastVolBBreakEndTime;		//B�����һ�ζ������ʱ��
	STDATETIME	stHisLastVolCBreakEndTime;		//C�����һ�ζ������ʱ��

	unsigned short usHisPowRateSect1Time;		//���������ֲ�����1�ۼ�ʱ�䣨�������� < ��ֵ1��
	unsigned short usHisPowRateSect2Time;		//���������ֲ�����2�ۼ�ʱ�䣨��ֵ1�ܹ�������<��ֵ2�� 
	unsigned short usHisPowRateSect3Time;		//���������ֲ�����3�ۼ�ʱ�䣨���������ݶ�ֵ2�� 

	unsigned short usHisTerPowerOnTime;			//�ն��չ���ʱ��
	unsigned short usHisTerResetNum;			//�ն��ո�λ�ۼƴ��� 
	unsigned short usHisMonthEnergyCtrlTripNum;	//�µ����բ����
	unsigned short usHisBuyEnergyCtrlTripNum;	//�������բ����
	unsigned short usHisPowerCtrlTripNum;		//������բ����
	unsigned short usHisRemoteCtrlTripNum;		//ң����բ����

	unsigned long ulHisFluxBytes;				//�����ֽ���
}STHISRUNDATA;		//840 bytes	��Լ +80

//typedef	struct	_STHISSUMGROUPRUNDATA
//{
//	STDATETIME	stRecordTime;
//	unsigned short	usReserve0;
//
//	float	fSumGroupPowTotal;
//	float	fSumGroupPowFee1;
//	float	fSumGroupPowFee2;
//	float	fSumGroupPowFee3;
//	float	fSumGroupPowFee4;				//�ܼ��й�����ʾֵ
//
//	float	fSumGroupVarTotal;
//	float	fSumGroupVarFee1;
//	float	fSumGroupVarFee2;
//	float	fSumGroupVarFee3;
//	float	fSumGroupVarFee4;				//�ܼ��޹�����ʾֵ
//
//	float	fSumGroupLastDayPowTotal;
//	float	fSumGroupLastDayPowFee1;
//	float	fSumGroupLastDayPowFee2;
//	float	fSumGroupLastDayPowFee3;
//	float	fSumGroupLastDayPowFee4;				//��һ���ܼ��й�����ʾֵ
//
//	float	fSumGroupLastDayVarTotal;
//	float	fSumGroupLastDayVarFee1;
//	float	fSumGroupLastDayVarFee2;
//	float	fSumGroupLastDayVarFee3;
//	float	fSumGroupLastDayVarFee4;				//��һ���ܼ��޹�����ʾֵ
//
//	float	fSumGroupMaxInstantPow;					//����ܼ��й�ͳ��
//	STDATETIME	stSumGroupMaxTimeInstantPow;
//
//	float	fSumGroupMinInstantPow;					//��С�ܼ�ͳ��
//	STDATETIME	stSumGroupMinTimeInstantPow;
//
//	unsigned short	usSumGroupZeroTimeInstantPow;	//�ܼӹ���Ϊ�����ʱ��
//
//	unsigned short	usSumGroupPowerOverTime;			//�ܼӹ��ʳ���ֵʱ��
//	unsigned short	usSumGroupEnergyOverTime;			//�ܼ��µ�������ֵʱ��
//	float	fSumGroupPowerOverAccValTotal;				//���ʳ���ֵ�µ����ۼ�
//	float	fSumGroupEnergyOverAccValTotal;				//�µ�������ֵ�µ����ۼ�
//	float	fSumGroupPowerOverStartValTotal;			//���ʳ���ֵ��ʼʾֵ
//	float	fSumGroupEnergyOverStartValTotal;			//�µ�������ֵ��ʼʾֵ	//����Խ���򱣳ָ�ֵ��Խ�޻ָ����޸ĸ�ֵ�޸ĵ����ۼƣ��ɻָ���Խ�����޸ĸ�ֵ
//}STHISSUMGROUPRUNDATA;			//156 byte +40

//typedef	struct	_STHISSUMGOURPALLDATA
//{
//	STHISSUMGROUPRUNDATA   stHisSumGroupRunData[CON_SUMGROUP_NUM];
//}STHISSUMGROUPALLDATA;
//
//typedef	struct	_STHISPULSERUNDATA
//{
//	STDATETIME	stRecordTime;
//	unsigned short	usReserve0;
//
//	unsigned long	ulHisPowTotal;
//	unsigned long	ulHisPowFee1;
//	unsigned long	ulHisPowFee2;
//	unsigned long	ulHisPowFee3;
//	unsigned long	ulHisPowFee4;				//����
//
//	unsigned long	ulHisLastDayPowTotal;
//	unsigned long	ulHisLastDayPowFee1;
//	unsigned long	ulHisLastDayPowFee2;
//	unsigned long	ulHisLastDayPowFee3;
//	unsigned long	ulHisLastDayPowFee4;				//��һ�����
//
//	unsigned long	ulHisPowMaxDemand;
//	unsigned long	ulHisPowMaxDemandFee1;
//	unsigned long	ulHisPowMaxDemandFee2;
//	unsigned long	ulHisPowMaxDemandFee3;
//	unsigned long	ulHisPowMaxDemandFee4;		//�������
//
//	STDATETIME	stHisPowMaxDemandTime;
//	STDATETIME	stHisPowMaxDemandTimeFee1;
//	STDATETIME	stHisPowMaxDemandTimeFee2;
//	STDATETIME	stHisPowMaxDemandTimeFee3;
//	STDATETIME	stHisPowMaxDemandTimeFee4;		//�����������ʱ��
//
//	STHISVAL	stHisMaxInstantPow;				//˲ʱ�й�ͳ��//����STHISVAL�ṹ������STHISVAL.stDateTime��ʾ���ֵ����ʱ�䣬�����ʾ��ͬ
//	unsigned short	usHisZeroTimeInstantPow;
//
//	STHISVAL	stHisMaxDemand;					//ͳ������
//}STHISPULSERUNDATA;

//typedef	struct	_STHISPULSEALLDATA
//{
//	STHISPULSERUNDATA   stHisPulseRunData[CON_PULSE_NUM];
//}STHISPULSEALLDATA;

typedef	struct	_STHISCQSUPRUNDATA				//���������
{
	STDATETIME	stRecordTime;
	unsigned short	usReserve0;
	unsigned long	ulHisDirPowA;			//A���й���
	unsigned long	ulHisDirPowB;			//B���й���
	unsigned long	ulHisDirPowC;			//C���й���
	unsigned long	ulHisVarLA;				//A������޹���
	unsigned long	ulHisVarLB;				//B������޹���
	unsigned long	ulHisVarLC;				//C������޹���
	unsigned long	ulHisVarCA;				//A�������޹���
	unsigned long	ulHisVarCB;				//B�������޹���
	unsigned long	ulHisVarCC;				//C�������޹���
	unsigned long	ulHisInstantS;			//���ڹ�����
	unsigned long	ulHisInstantSA;			//A�����ڹ���
	unsigned long	ulHisInstantSB;			//B�����ڹ���
	unsigned long	ulHisInstantSC;			//C�����ڹ���
}STHISCQSUPRUNDATA;

typedef	struct	_STHISCQSUPALLDATA
{
	STHISCQSUPRUNDATA   stHisCqSupRunData[CON_METER_NUM];	//���������
}STHISCQSUPALLDATA;


//�ճ�ʱ�䡢�³�ʱ�䣬����ǰ��������ĩʱ�䡢��ĩʱ��
#define CON_DAYDATA_HOUR			0				//�����ݴ���ʱ��
#define CON_DAYDATA_MINUTE			8
#define CON_DAYDATA_MINUTE_SUB		14				//��ǰʱ�䣬��λ����
#define CON_MONTHDATA_DAY			1				//�����ݴ���ʱ��
#define CON_MONTHDATA_HOUR			0
#define CON_MONTHDATA_MINUTE		9
#define CON_MONTHDATA_MINUTE_SUB	15				//��ǰʱ�䣬��λ����
#define CON_MINUTE_DIFF				3				//���������ʱ��ƫ��

#define CON_DAYDATA_TYPE     0//�ն���
#define CON_MONTHDATA_TYPE   1//�¶���
#define CON_FROZEDATA_TYPE   2//�����ն���
#define CON_FROZE_METERCOPY_TYPE   3//�����ճ���ʱ�䶳��

#define CON_DAYDATA_NUM				36//32				//
#define CON_MONTHDATA_NUM			16//13				//
#define CON_DAYFROZENDATA_NUM		12//6				//���6�㣬����ÿ�����3�������գ�����������
//���¶������ݱ��浽EE�������Ϣ ָ�뼰ʱ����Ϣ
//#define CON_DAY_MONTH_INFO_BASE_ADDR (CON_GRTASK_PARA_BASE_ADDR+CON_GRTASK_PARA_BASE_BOLCK)
//
#define CON_DAYMONTH_INDEX_LEN       (2+1)//+CRC
#define CON_DAYMONTH_TIME_LEN        (6+1)//+CRC
//
#define CON_DAYDATA_INDEX_ADDR       0
#define CON_DAYDATA_INDEX_LEN        CON_DAYMONTH_INDEX_LEN//+CRC
#define CON_DAYDATA_TIME_ADDR        (CON_DAYDATA_INDEX_ADDR+CON_DAYDATA_INDEX_LEN)
#define CON_DAYDATA_TIME_LEN         CON_DAYMONTH_TIME_LEN//(6+1)//+CRC
#define CON_DAYDATA_TIME_BLOCK_LEN   (CON_DAYDATA_TIME_LEN*(CON_DAYDATA_NUM+1))//��Ԥ��һ��ʱ��ռ�
//
#define CON_MONTHDATA_INDEX_ADDR       (CON_DAYDATA_TIME_ADDR+CON_DAYDATA_TIME_BLOCK_LEN)
#define CON_MONTHDATA_INDEX_LEN        CON_DAYMONTH_INDEX_LEN//+CRC
#define CON_MONTHDATA_TIME_ADDR        (CON_MONTHDATA_INDEX_ADDR+CON_MONTHDATA_INDEX_LEN)
#define CON_MONTHDATA_TIME_LEN         CON_DAYMONTH_TIME_LEN//(6+1)//+CRC
#define CON_MONTHDATA_TIME_BLOCK_LEN   (CON_MONTHDATA_TIME_LEN*(CON_MONTHDATA_NUM+1))
//
#define CON_DAYFROZENDATA_INDEX_ADDR       (CON_MONTHDATA_TIME_ADDR+CON_MONTHDATA_TIME_BLOCK_LEN)
#define CON_DAYFROZENDATA_INDEX_LEN        CON_DAYMONTH_INDEX_LEN//+CRC
#define CON_DAYFROZENDATA_TIME_ADDR        (CON_DAYFROZENDATA_INDEX_ADDR+CON_DAYFROZENDATA_INDEX_LEN)
#define CON_DAYFROZENDATA_TIME_LEN         CON_DAYMONTH_TIME_LEN//(6+1)//+CRC
#define CON_DAYFROZENDATA_TIME_BLOCK_LEN   (CON_DAYFROZENDATA_TIME_LEN*(CON_DAYFROZENDATA_NUM+1))
//ÿ����������ռ�ռ䳤��
#define CON_DAYMONTH_DATA_LEN_MP            (CON_DAYDATA_INDEX_LEN*3+CON_DAYDATA_TIME_BLOCK_LEN+CON_MONTHDATA_TIME_BLOCK_LEN+CON_DAYFROZENDATA_TIME_BLOCK_LEN)

//#define CON_DFPAGE_SUMG_HISDATA_PAGENUM		16		//150�ֽ�*��32+13��ÿ��16ҳ���̶�8k
//#define CON_DFPAGE_PULSE_HISDATA_PAGENUM	16		//150�ֽ�*��32+13��ÿ��16ҳ���̶�8k
//
//#define CON_DFPAGE_HEAD_TER_HISDATA		(CON_DFPAGE_HEAD_HISDATA+CON_DFPAGE_HISDATA_PAGENUM*CON_METER_NUM)	//ǰ���ռ��8��
//#define CON_DFPAGE_HEAD_SUMG_HISDATA	(CON_DFPAGE_HEAD_TER_HISDATA+CON_DFPAGE_HISDATA_PAGENUM)			//�ն�ռ��1��
//#define CON_DFPAGE_HEAD_PULSE_HISDATA	(CON_DFPAGE_HEAD_SUMG_HISDATA+CON_DFPAGE_SUMG_HISDATA_PAGENUM*CON_SUMGROUP_NUM)




typedef	struct	_STDAYFROZENLISTINDEX
{
	unsigned char ucHead;
	unsigned char ucTail;
	STDATETIME stDateTimeList[CON_DAYFROZENDATA_NUM];
}STDAYFROZENLISTINDEX;

typedef	struct	_STDAYLISTINDEX
{
	unsigned char ucHead;
	unsigned char ucTail;
	STDATETIME stDateTimeList[CON_DAYDATA_NUM];
}STDAYLISTINDEX;

typedef	struct	_STMONTHLISTINDEX
{
	unsigned char ucHead;
	unsigned char ucTail;
	STDATETIME stDateTimeList[CON_MONTHDATA_NUM];
}STMONTHLISTINDEX;

typedef	struct	_STHISRUNDATAINDEX
{
	STDAYFROZENLISTINDEX	stDayFrozenDataIndex;
	STDAYLISTINDEX			stDayDataIndex;
	STMONTHLISTINDEX		stMonthDataIndex;
}STHISRUNDATAINDEX;

typedef	struct	_STHISSUMGROUPDATAINDEX
{//�ܼ�
	STDAYLISTINDEX			stSumGDayDataIndex;
	STMONTHLISTINDEX		stSumGMonthDataIndex;
}STHISSUMGROUPDATAINDEX;

typedef	struct	_STHISTERDATAINDEX
{//�ն�
	STDAYFROZENLISTINDEX	stDayFrozenDataIndex;
	STDAYLISTINDEX			stTerDayDataIndex;
	STMONTHLISTINDEX		stTerMonthDataIndex;
}STHISTERDATAINDEX;

typedef	struct	_STHISPULSEDATAINDEX
{//����
	STDAYLISTINDEX			stPulseDayDataIndex;
	STMONTHLISTINDEX		stPulseMonthDataIndex;
}STHISPULSEDATAINDEX;

//---------------------------------------------------------------------
__EXTERN void RunData_Init(void);
__EXTERN void RunData_Init_All(void);
__EXTERN void RunData_Meter_Init(unsigned char ucMeterNo);

//�����й�����
__EXTERN unsigned char Add_History_DirPower(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
__EXTERN unsigned char Add_History_DirPower_LastValid(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//�����й�����
__EXTERN unsigned char Add_History_UnDirPower(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
__EXTERN unsigned char Add_History_UnDirPower_LastValid(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//�����޹�����
__EXTERN unsigned char Add_History_DirVar(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
__EXTERN unsigned char Add_History_DirVar_LastValid(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//�����޹�����
__EXTERN unsigned char Add_History_UnDirVar(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//1�����޹�����
__EXTERN unsigned char Add_History_DirVar1(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//2�����޹�����
__EXTERN unsigned char Add_History_DirVar2(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//3�����޹�����
__EXTERN unsigned char Add_History_DirVar3(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//4�����޹�����
__EXTERN unsigned char Add_History_DirVar4(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//��ѹ
__EXTERN unsigned char Add_History_I(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucPhaseNo);
//����
__EXTERN unsigned char Add_History_V(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucPhaseNo);
//˲ʱ�й����޹�
__EXTERN unsigned char Add_History_InstantPow(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo);
__EXTERN unsigned char Add_History_InstantVar(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo);
//�����й��������
__EXTERN unsigned char Add_History_DirPowMaxDemand(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//�����й������������ʱ��
__EXTERN unsigned char Add_History_DirPowMaxDemandTime(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucFeeNo);
//�����޹��������
__EXTERN unsigned char Add_History_DirVarMaxDemand(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//�����޹������������ʱ��
__EXTERN unsigned char Add_History_DirVarMaxDemandTime(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucFeeNo);
//�����й��������
__EXTERN unsigned char Add_History_UnDirPowMaxDemand(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//�����й������������ʱ��
__EXTERN unsigned char Add_History_UnDirPowMaxDemandTime(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucFeeNo);
//�����޹��������
__EXTERN unsigned char Add_History_UnDirVarMaxDemand(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//�����޹������������ʱ��
__EXTERN unsigned char Add_History_UnDirVarMaxDemandTime(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucFeeNo);
//�����й����������£�
__EXTERN unsigned char Add_History_DirPowerLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//�����й�����(����)
__EXTERN unsigned char Add_History_UnDirPowerLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//�����޹�����(����)
__EXTERN unsigned char Add_History_DirVarLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//�����޹�����(����)
__EXTERN unsigned char Add_History_UnDirVarLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//һ�����޹�(����)
__EXTERN unsigned char Add_History_DirVar1Lm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//�������޹�(����)
__EXTERN unsigned char Add_History_DirVar2Lm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//�������޹�(����)
__EXTERN unsigned char Add_History_DirVar3Lm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//�������޹�(����)
__EXTERN unsigned char Add_History_DirVar4Lm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//�����й��������(����)
__EXTERN unsigned char Add_History_DirPowMaxDemandLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//�����й������������ʱ��(����)
__EXTERN unsigned char Add_History_DirPowMaxDemandTimeLm(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucFeeNo);
//�����޹��������(����)
__EXTERN unsigned char Add_History_DirVarMaxDemandLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//�����޹������������ʱ��(����)
__EXTERN unsigned char Add_History_DirVarMaxDemandTimeLm(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucFeeNo);
//�����й��������(����)
__EXTERN unsigned char Add_History_UnDirPowMaxDemandLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//�����й������������ʱ��(����)
__EXTERN unsigned char Add_History_UnDirPowMaxDemandTimeLm(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucFeeNo);
//�����޹��������(����)
__EXTERN unsigned char Add_History_UnDirVarMaxDemandLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//�����޹������������ʱ��(����)
__EXTERN unsigned char Add_History_UnDirVarMaxDemandTimeLm(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucFeeNo);
//�й����޹����峣��
__EXTERN unsigned char Add_History_PowPulseConst(unsigned char ucMeterNo,unsigned long lVal);
__EXTERN unsigned char Add_History_VarPulseConst(unsigned char ucMeterNo,unsigned long lVal);
//����
__EXTERN unsigned char Add_History_FeeRate(unsigned char ucMeterNo,STFEERATELIST	stFeeRateList);
//���ʱ��
__EXTERN unsigned char Add_History_ProgramTime(unsigned char ucMeterNo,STDATETIME	stDateTime);
//��������ʱ��
__EXTERN unsigned char Add_History_DemandClrTime(unsigned char ucMeterNo,STDATETIME	stDateTime);
//��̴���
__EXTERN unsigned char Add_History_ProgramCount(unsigned char ucMeterNo,unsigned long	lVal);
//�����������
__EXTERN unsigned char Add_History_DemandClrCount(unsigned char ucMeterNo,unsigned long	lVal);
//�������ʱ��
__EXTERN unsigned char Add_History_MeterClrTime(unsigned char ucMeterNo,STDATETIME	stDateTime);
//�¼�����ʱ��
__EXTERN unsigned char Add_History_EventClrTime(unsigned char ucMeterNo,STDATETIME	stDateTime);
//Уʱʱ��
__EXTERN unsigned char Add_History_JiaoshiTime(unsigned char ucMeterNo,STDATETIME	stDateTime);
//����������
__EXTERN unsigned char Add_History_MeterClrCount(unsigned char ucMeterNo,unsigned long	lVal);
//�¼��������
__EXTERN unsigned char Add_History_EventClrCount(unsigned char ucMeterNo,unsigned long	lVal);
//Уʱ����
__EXTERN unsigned char Add_History_JiaoshiCount(unsigned char ucMeterNo,unsigned long	lVal);
//��ع���ʱ��
__EXTERN unsigned char Add_History_BatWorkTime(unsigned char ucMeterNo,unsigned long	lVal);
//�������״̬��
__EXTERN unsigned char Add_History_MeterStatFlag(unsigned char ucMeterNo,unsigned char	ucStateFlag,unsigned char ucFlagNum);
//��������״̬��
//__EXTERN unsigned char Add_History_NetStatFlag(unsigned char ucMeterNo,unsigned char	ucStateFlag);
//������
__EXTERN unsigned char Add_History_CopyDayTime(unsigned char ucMeterNo,STDATETIME	stDateTime);
//�������
__EXTERN unsigned char Add_History_LostVolNum(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo);
//����ʱ���ۼ�ֵ
__EXTERN unsigned char Add_History_LostVolTime(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo);
//���һ�ζ�����ʼʱ��
__EXTERN unsigned char Add_History_LastVolOffBegTime(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucNo);
//���һ�ζ������ʱ��
__EXTERN unsigned char Add_History_LastVolOffEndTime(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucNo);
//������
__EXTERN unsigned char Add_History_MeterDate(unsigned char ucMeterNo,STDATETIME	stDateTime);
//ʱ����
__EXTERN unsigned char Add_History_MeterTime(unsigned char ucMeterNo,STDATETIME	stDateTime);

///*******************************************�ܼ������ݻ�ȡ*************************************/
//__EXTERN unsigned char Add_History_SumValInstantPowGroup(void);
//__EXTERN unsigned char Add_History_SumValInstantVarGroup(void);
//__EXTERN unsigned char Add_History_SumValPowValueGroup(void);
//__EXTERN unsigned char Add_History_SumValVarValueGroup(void);
//__EXTERN unsigned char Add_History_SumValHourPowValueGroup(void);

/************************************************************************************/
////__EXTERN STHISSUMVALGROUP Get_History_InstantPowSumVal(unsigned char ucPos);	//˲ʱ�й��ܼ���
//__EXTERN STHISSUMVALGROUP Get_History_PowValSumVal(unsigned char ucPos,unsigned char ucFeeNo);	//�й������ܼ���
////__EXTERN STHISSUMVALGROUP Get_History_InstantVarSumVal(unsigned char ucPos);	//˲ʱ�޹��ܼ���
//__EXTERN STHISSUMVALGROUP Get_History_VarValSumVal(unsigned char ucPos,unsigned char ucFeeNo);	//�޹������ܼ���
//__EXTERN STHISSUMVALGROUP Get_History_PowValSumVal_LM(unsigned char ucPos,unsigned char ucFeeNo);//���µ�
//__EXTERN STHISSUMVALGROUP Get_History_VarValSumVal_LM(unsigned char ucPos,unsigned char ucFeeNo);//���µ�
//__EXTERN STHISSUMVALGROUP Get_History_PowValHourSumVal(unsigned char ucPos);

//�����й�����
__EXTERN STHISVAL Get_History_DirPower_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//���߾��ȵ���0.0000KWH
__EXTERN STHISVAL Get_History_DirPower_byMPNo_High(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����й�����
__EXTERN STHISVAL Get_History_UnDirPower_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����޹�����
__EXTERN STHISVAL Get_History_DirVar_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����޹�����
__EXTERN STHISVAL Get_History_UnDirVar_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//1�����޹�����
__EXTERN STHISVAL Get_History_DirVar1_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//2�����޹�����
__EXTERN STHISVAL Get_History_DirVar2_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//3�����޹�����
__EXTERN STHISVAL Get_History_DirVar3_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//4�����޹�����
__EXTERN STHISVAL Get_History_DirVar4_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����й��������
__EXTERN STHISVAL Get_History_DirPowMaxDemand_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����й������������ʱ��
__EXTERN STHISTIME Get_History_DirPowMaxDemandTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����޹��������
__EXTERN STHISVAL Get_History_DirVarMaxDemand_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����޹������������ʱ��
__EXTERN STHISTIME Get_History_DirVarMaxDemandTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����й��������
__EXTERN STHISVAL Get_History_UnDirPowMaxDemand_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����й������������ʱ��
__EXTERN STHISTIME Get_History_UnDirPowMaxDemandTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����޹��������
__EXTERN STHISVAL Get_History_UnDirVarMaxDemand_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����޹������������ʱ��
__EXTERN STHISTIME Get_History_UnDirVarMaxDemandTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);

__EXTERN STHISVAL Get_History_DirPowA_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
__EXTERN STHISVAL Get_History_DirPowB_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
__EXTERN STHISVAL Get_History_DirPowC_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
__EXTERN STHISVAL Get_History_DirPowALm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
__EXTERN STHISVAL Get_History_DirPowBLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
__EXTERN STHISVAL Get_History_DirPowCLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);

//�����й����������£�
__EXTERN STHISVAL Get_History_DirPowerLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����й�����(����)
__EXTERN STHISVAL Get_History_UnDirPowerLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����޹�����(����)
__EXTERN STHISVAL Get_History_DirVarLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����޹�����(����)
__EXTERN STHISVAL Get_History_UnDirVarLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//1�����޹�����(����)
__EXTERN STHISVAL Get_History_DirVar1Lm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//2�����޹�����(����)
__EXTERN STHISVAL Get_History_DirVar2Lm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//3�����޹�����(����)
__EXTERN STHISVAL Get_History_DirVar3Lm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//4�����޹�����(����)
__EXTERN STHISVAL Get_History_DirVar4Lm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����й��������(����)
__EXTERN STHISVAL Get_History_DirPowMaxDemandLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����й������������ʱ��(����)
__EXTERN STHISTIME Get_History_DirPowMaxDemandTimeLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����޹��������(����)
__EXTERN STHISVAL Get_History_DirVarMaxDemandLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����޹������������ʱ��(����)
__EXTERN STHISTIME Get_History_DirVarMaxDemandTimeLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����й��������(����)
__EXTERN STHISVAL Get_History_UnDirPowMaxDemandLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����й������������ʱ��(����)
__EXTERN STHISTIME Get_History_UnDirPowMaxDemandTimeLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����޹��������(����)
__EXTERN STHISVAL Get_History_UnDirVarMaxDemandLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//�����޹������������ʱ��(����)
__EXTERN STHISTIME Get_History_UnDirVarMaxDemandTimeLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//��ѹ
__EXTERN STHISVAL Get_History_V_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//����
__EXTERN STHISVAL Get_History_I_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//���
__EXTERN STHISVAL Get_History_V_Phase_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
__EXTERN STHISVAL Get_History_I_Phase_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//����˲ʱ�й����޹�
__EXTERN STHISVAL Get_History_InstantPow_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
__EXTERN STHISVAL Get_History_InstantVar_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//����˲ʱ�й����޹�
__EXTERN STHISVAL Get_History_RevInstantPow_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
__EXTERN STHISVAL Get_History_RevInstantVar_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//��������15����ƽ���й�
//__EXTERN STHISVAL Get_History_AvePow15_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//__EXTERN STHISVAL Get_History_AveVar15_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//__EXTERN STHISVAL Get_History_RevAvePow15_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//__EXTERN STHISVAL Get_History_RevAveVar15_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//�й����޹����峣��
//__EXTERN STHISVAL Get_History_PowPulseConst_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//__EXTERN STHISVAL Get_History_VarPulseConst_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//����
//__EXTERN STHISFEERATE Get_History_FeeRate_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//���ʱ��
__EXTERN STHISTIME Get_History_ProgramTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//��������ʱ��
__EXTERN STHISTIME Get_History_DemandClrTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//��̴���
__EXTERN STHISVAL Get_History_ProgramCount_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//�����������
__EXTERN STHISVAL Get_History_DemandClrCount_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//�������ʱ��
__EXTERN STHISTIME Get_History_MeterClrTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//�¼�����ʱ��
__EXTERN STHISTIME Get_History_EventClrTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//Уʱʱ��
__EXTERN STHISTIME Get_History_JiaoshiTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//����������
__EXTERN STHISVAL Get_History_MeterClrCount_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//�¼��������
__EXTERN STHISVAL Get_History_EventClrCount_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//Уʱ����
__EXTERN STHISVAL Get_History_JiaoshiCount_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//��ع���ʱ��
__EXTERN STHISVAL Get_History_BatWorkTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//�������״̬��
__EXTERN STHISSTATE Get_History_MeterStatFlag_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFlagNum);
//��������״̬��
//__EXTERN STHISSTATE Get_History_NetStatFlag_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//������
//__EXTERN STHISTIME Get_History_CopyDayTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//�������
__EXTERN STHISVAL Get_History_VolBreakNum_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//����ʱ���ۼ�ֵ
__EXTERN STHISVAL Get_History_VolBreakTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//���һ�ζ�����ʼʱ��
__EXTERN STHISTIME Get_History_LastVolBreakBegTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//���һ�ζ������ʱ��
__EXTERN STHISTIME Get_History_LastVolBreakEndTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//������
__EXTERN STHISTIME Get_History_Date_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//ʱ����
__EXTERN STHISTIME Get_History_Time_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//�Žڵ�״̬
//__EXTERN STHISSTATE Get_History_DoorStat_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//�ն�ͣ�ϵ�״̬
__EXTERN STHISSTATE Get_History_PowerStat_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//����״̬
//__EXTERN STHISSTATE Get_History_SwitchStat_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucSwitchNo);
//����������
//__EXTERN STHISSTATE Get_History_PolReverseStat_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//CTһ�β��·
//__EXTERN STHISSTATE Get_History_ShortCir1Stat_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//CT���β࿪·
////__EXTERN STHISSTATE Get_History_OpenCir2Stat_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//CT���β��·
//__EXTERN STHISSTATE Get_History_ShortCir2Stat_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//��ѹ��·������
//__EXTERN STHISSTATE Get_History_VolCirReveStat_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//��ʾ��ѹ
//__EXTERN STHISVAL Get_History_V_Disp_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//��ʾ����
//__EXTERN STHISVAL Get_History_I_Disp_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//��ʾ�޹�
//__EXTERN STHISVAL Get_History_InstantVar_Disp_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);

//����0.001���ȵ����ɼ�
//__EXTERN STHISVAL Get_P1000_History_DirPower_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);

__EXTERN void ClearHisVal(STHISVAL *pstHisVal);
__EXTERN void ClearHisTime(STHISTIME *pstHisTime);
__EXTERN void ClearHisFeeRate(STHISFEERATE *pstHisFeeRate);
__EXTERN void ClearFeeRateList(STFEERATELIST *pstFeeRateList);
__EXTERN void ClearHisState(STHISSTATE *pstHisState);
//__EXTERN void ClearHisSumValGroup(STHISSUMVALGROUP *pstHisSumValGroup);

//__EXTERN unsigned char  Get_Status_TerPowLow(void);


//-------------�������------------------------
__EXTERN void Job_ProcData_Timer(void);
__EXTERN unsigned char Job_Data_Explain(unsigned char *pucBuffer,unsigned char usLen,unsigned char ucRepType,unsigned char *pucRetTerMsgNum);
__EXTERN void TimeChange_JobManage(STDATETIME stOldTime,STDATETIME stNewTime);
__EXTERN void Job_Adjust_MonthUp_FreeseDayData(STDATETIME *psTime,unsigned char ucMPNo);
__EXTERN unsigned char Job_GetData_Data1(STDATAUNITID stDUID,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Job_GetData_Data2(STDATAUNITID stDUID,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);


__EXTERN unsigned char Fill_AlarmTimer_Manage(unsigned char ucMPNo,unsigned short usDILabel,unsigned char ucIsValid);
//unsigned char Fill_AlarmTer_Manage(STADEMETERVAR	*pstADEMeterVal,STADEMETERENERGYVAR *pstADEMeterEnergyVal,PSTADEERRSTAT	pstADEErrStat,STADEDEMAND *pstAdeDemand);

//__EXTERN unsigned short GetPowDI_byGroupNo(unsigned char ucGroupNo);
//__EXTERN unsigned short GetVarDI_byGroupNo(unsigned char ucGroupNo);

//-------------����������----------------------
//__EXTERN void		Pulse_Counter(void);
//__EXTERN void		Make_PulseTimer(void);
//__EXTERN unsigned long GetPulseBaseWidth(unsigned char ucPulse);		//test
//__EXTERN void TimeChange_PulseHisData(STDATETIME stOldTime,STDATETIME stNewTime);
//__EXTERN void Pulse_InitMeterData(void);
//__EXTERN void Init_PulseHisData(void);
//__EXTERN unsigned char Set_PulseMeterBase_DirPowTotal(unsigned char ucPulseNo,unsigned long ulVal);
//__EXTERN unsigned char Set_PulseMeterBase_DirVarTotal(unsigned char ucPulseNo,unsigned long ulVal);
//__EXTERN unsigned char Set_PulseMeterBase_RevPowTotal(unsigned char ucPulseNo,unsigned long ulVal);
//__EXTERN unsigned char Set_PulseMeterBase_RevVarTotal(unsigned char ucPulseNo,unsigned long ulVal);
//__EXTERN unsigned char Set_PulseMeterBase_DirPow(unsigned char ucPulseNo,unsigned char ucFeeNo,unsigned long ulVal);
//__EXTERN unsigned char Set_PulseMeterBase_DirVar(unsigned char ucPulseNo,unsigned char ucFeeNo,unsigned long ulVal);
//__EXTERN unsigned char Set_PulseMeterBase_RevPow(unsigned char ucPulseNo,unsigned char ucFeeNo,unsigned long ulVal);
//__EXTERN unsigned char Set_PulseMeterBase_RevVar(unsigned char ucPulseNo,unsigned char ucFeeNo,unsigned long ulVal);

//__EXTERN unsigned char Clear_PulseMaxDemand_DirPowTotal(unsigned char ucPulseNo,unsigned char ucPos);
//__EXTERN unsigned char Clear_PulseMaxDemand_DirVarTotal(unsigned char ucPulseNo,unsigned char ucPos);
//__EXTERN unsigned char Clear_PulseMaxDemand_RevPowTotal(unsigned char ucPulseNo,unsigned char ucPos);
//__EXTERN unsigned char Clear_PulseMaxDemand_RevVarTotal(unsigned char ucPulseNo,unsigned char ucPos);
//
//__EXTERN unsigned char Set_PulseCount_AllAsNow(unsigned char ucPulseNo);
//__EXTERN unsigned char Set_PulseCount_AllSubNow(unsigned char ucPulseNo);
//__EXTERN void Make_PulseMonthChange(unsigned char ucPulseNo);
////__EXTERN unsigned short Get_PulseCount_Num(unsigned char ucPulseNo);
//__EXTERN void Pulse_Comp(unsigned char ucPulseNo);
//__EXTERN void Pulse_Comp_Init(void);
//__EXTERN unsigned char Clear_PulseMeterVal_LastMonth(unsigned char ucPulseNo);
////unsigned char Get_PulseSwtich_InStat(unsigned char ucPulseNo);

//-------------���ݴ�������ͳ��----------------------
__EXTERN void DataProc_Init_All(void);
__EXTERN void DataProc_Init(void);

__EXTERN void DataProc_Timer(void);

__EXTERN unsigned char DataProc_DayFrozenData_Stat(STDATETIME stDateTime);
__EXTERN unsigned char DataProc_DayData_Stat(STDATETIME stDateTime);
__EXTERN unsigned char DataProc_MonthData_Stat(STDATETIME stDateTime);
__EXTERN unsigned char DataProc_MonthData_View_Stat(STDATETIME stDateTime);
//__EXTERN unsigned char DataProc_SumGDayData_Stat(STDATETIME stDateTime);
//__EXTERN unsigned char DataProc_SumGMonthData_Stat(STDATETIME stDateTime);

__EXTERN unsigned char DataProc_GetData_DayFrozen(unsigned char ucMeterNo,STDATETIME stDateTime,STHISRUNDATA *pstHisRunData);
__EXTERN unsigned char DataProc_GetData_Day(unsigned char ucMeterNo,STDATETIME stDateTime,STHISRUNDATA *pstHisRunData);
__EXTERN unsigned char DataProc_GetData_Month(unsigned char ucMeterNo,STDATETIME stDateTime,STHISRUNDATA *pstHisRunData);
//__EXTERN unsigned char DataProc_GetSumGData_Day(unsigned char ucSumGNo,STDATETIME stDateTime,STHISSUMGROUPRUNDATA *pstHisSumGRunData);
//__EXTERN unsigned char DataProc_GetSumGData_Month(unsigned char ucSumGNo,STDATETIME stDateTime,STHISSUMGROUPRUNDATA *pstHisSumGRunData);

__EXTERN void DataProc_Stat_TerResetNum_Inc(void);
//__EXTERN void DataProc_Stat_MonthEnerCtrlNum_Inc(void);
//__EXTERN void DataProc_Stat_BuyEnerCtrlNum_Inc(void);
//__EXTERN void DataProc_Stat_PowCtrlNum_Inc(void);
//__EXTERN void DataProc_Stat_RemoteCtrlNum_Inc(void);
__EXTERN void DataProc_Stat_FluxBytes_Inc(unsigned long ulBytes);
__EXTERN void DataProc_Stat_FluxBytes_LimitChange(unsigned long ulOldLimit,unsigned long ulNewLimit);
 
//__EXTERN unsigned char DataProc_TerDayData_Stat(STDATETIME stDateTime);
//__EXTERN unsigned char DataProc_TerMonthData_Stat(STDATETIME stDateTime);
__EXTERN unsigned char DataProc_GetData_Day_Ter(STDATETIME stDateTime,STHISRUNDATA *pstHisRunData);
__EXTERN unsigned char DataProc_GetData_Month_Ter(STDATETIME stDateTime,STHISRUNDATA *pstHisRunData);

//__EXTERN unsigned char DataProc_TerDayFrozenData_Stat(STDATETIME stDateTime);
//__EXTERN unsigned char DataProc_GetData_TerDayFrozen(STDATETIME stDateTime,STHISRUNDATA *pstHisRunData);

__EXTERN unsigned char DataProc_PulseDayData_Stat(STDATETIME stDateTime);
__EXTERN unsigned char DataProc_PulseMonthData_Stat(STDATETIME stDateTime);
//__EXTERN unsigned char DataProc_GetData_Day_Pulse(unsigned char ucPulseNo,STDATETIME stDateTime,STHISPULSERUNDATA *pstHisRunData);
//__EXTERN unsigned char DataProc_GetData_Month_Pulse(unsigned char ucPulseNo,STDATETIME stDateTime,STHISPULSERUNDATA *pstHisRunData);
 
__EXTERN unsigned char DataProc_MonthData_FlashLast(STDATETIME stDateTime);
//__EXTERN unsigned char DataProc_SumGMonthData_FlashLast(STDATETIME stDateTime);
//__EXTERN unsigned char DataProc_TerMonthData_FlashLast(STDATETIME stDateTime);
//__EXTERN unsigned char DataProc_PulseMonthData_FlashLast(STDATETIME stDateTime);

//__EXTERN void Inner_Make_PulsePowData_Per(STHISVAL *pstRetVal,STHISVAL *pstBaseVal,unsigned short usPulseCount,unsigned short usPulseCount_Bak,unsigned short usConst);
//__EXTERN void Inner_Make_PulsePowData_Per_P1000(STHISVAL *pstRetVal,STHISVAL *pstBaseVal,unsigned short usPulseCount,unsigned short usPulseCount_Bak,unsigned short usConst);
__EXTERN unsigned char Inner_RunPara_GetDefaultMeterCommPara(STMETERPARA *pstMeterPara,unsigned char ucInnerMeterProtocol);
//__EXTERN unsigned char Inner_Load_PowerCtrl(STDATETIME *pstDateTime,unsigned char ucPowCurveNo,unsigned char ucEndHour,unsigned char ucEndMinute);

#endif

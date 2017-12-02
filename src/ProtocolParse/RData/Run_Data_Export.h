/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Run_Data_Export.h
 Description    : 所有运行数据 数据结构声明
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-08-05    azh     the original version
******************************************************************************/

//运行数据结构定义
#ifndef _RUN_DATA_EXPORT_H_
#define _RUN_DATA_EXPORT_H_


#define		CONHISDATANUM		2				//当前数据点数，用于当前数据保存、告警分析等
#define		CONHISDATANUM_ADDLASTVALIDONE	(CONHISDATANUM+1)		//测量点增加最近有效一点
#define		CONHISDATA_LASTVALID_POS	(CONHISDATANUM_ADDLASTVALIDONE-1)		//最近有效一点

#define		CONPULSEHISDATANUM	2
#define		CONPULSECOUNTNUM	20
#define		CONHISHOURDATANUM		5				//当前数据点数，用于当前数据保存、告警分析等

#define		CON_SUMGROUP_NUM	8				//总加组数量

#define		CON_FEE_TOTAL		0x00
#define		CON_FEE_PLUS		0x01
#define		CON_FEE_APEX		0x02
#define		CON_FEE_FLAT		0x03
#define		CON_FEE_VALE		0x04

//历史值
typedef struct _STHISVAL
{
	STDATETIME	stDateTime;
	unsigned char ucSign;
	unsigned long lVal;
}STHISVAL;
//历史时间值	
typedef struct _STHISTIME
{
	STDATETIME	stDateTime;
	STDATETIME	stHisTime;
}STHISTIME;
//历史状态值
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
}STHISFEERATE;		//费率

typedef struct _STSUMVALGROUP 
{
	float	fSumVal[CON_SUMGROUP_NUM];
}STSUMVALGROUP;
typedef struct _STHISSUMVALGROUP
{
	STDATETIME		stDateTime;
	STSUMVALGROUP	stSumValGroup;
}STHISSUMVALGROUP;	//总加组

typedef struct _STMETERTIMERVAL
{
	STHISVAL	stHisDirPowTotal[CONHISDATANUM_ADDLASTVALIDONE];			//电控需要最近有效有功电量数据
	STHISVAL	stHisDirPowFee1[CONHISDATANUM_ADDLASTVALIDONE];
	STHISVAL	stHisDirPowFee2[CONHISDATANUM_ADDLASTVALIDONE];
	STHISVAL	stHisDirPowFee3[CONHISDATANUM_ADDLASTVALIDONE];
	STHISVAL	stHisDirPowFee4[CONHISDATANUM_ADDLASTVALIDONE];				//正向有功电量

	STHISVAL	stHisRevPowTotal[CONHISDATANUM_ADDLASTVALIDONE];
	STHISVAL	stHisRevPowFee1[CONHISDATANUM_ADDLASTVALIDONE];
	STHISVAL	stHisRevPowFee2[CONHISDATANUM_ADDLASTVALIDONE];
	STHISVAL	stHisRevPowFee3[CONHISDATANUM_ADDLASTVALIDONE];
	STHISVAL	stHisRevPowFee4[CONHISDATANUM_ADDLASTVALIDONE];				//反向有功电量

	STHISVAL	stHisDirVarTotal[CONHISDATANUM_ADDLASTVALIDONE];			//抄表异常需要最近正向无功总
	STHISVAL	stHisDirVarFee1[CONHISDATANUM];
	STHISVAL	stHisDirVarFee2[CONHISDATANUM];
	STHISVAL	stHisDirVarFee3[CONHISDATANUM];
	STHISVAL	stHisDirVarFee4[CONHISDATANUM];				//正向无功电量

	STHISVAL	stHisRevVarTotal[CONHISDATANUM];
	STHISVAL	stHisRevVarFee1[CONHISDATANUM];
	STHISVAL	stHisRevVarFee2[CONHISDATANUM];
	STHISVAL	stHisRevVarFee3[CONHISDATANUM];
	STHISVAL	stHisRevVarFee4[CONHISDATANUM];				//反向无功电量

	STHISVAL	stHisDirVar1[CONHISDATANUM];
	STHISVAL	stHisDirVar1Fee1[CONHISDATANUM];
	STHISVAL	stHisDirVar1Fee2[CONHISDATANUM];
	STHISVAL	stHisDirVar1Fee3[CONHISDATANUM];
	STHISVAL	stHisDirVar1Fee4[CONHISDATANUM];			//1象限无功电量

	STHISVAL	stHisDirVar2[CONHISDATANUM];
	STHISVAL	stHisDirVar2Fee1[CONHISDATANUM];
	STHISVAL	stHisDirVar2Fee2[CONHISDATANUM];
	STHISVAL	stHisDirVar2Fee3[CONHISDATANUM];
	STHISVAL	stHisDirVar2Fee4[CONHISDATANUM];			//2象限无功电量

	STHISVAL	stHisDirVar3[CONHISDATANUM];
	STHISVAL	stHisDirVar3Fee1[CONHISDATANUM];
	STHISVAL	stHisDirVar3Fee2[CONHISDATANUM];
	STHISVAL	stHisDirVar3Fee3[CONHISDATANUM];
	STHISVAL	stHisDirVar3Fee4[CONHISDATANUM];			//3象限无功电量

	STHISVAL	stHisDirVar4[CONHISDATANUM];
	STHISVAL	stHisDirVar4Fee1[CONHISDATANUM];
	STHISVAL	stHisDirVar4Fee2[CONHISDATANUM];
	STHISVAL	stHisDirVar4Fee3[CONHISDATANUM];
	STHISVAL	stHisDirVar4Fee4[CONHISDATANUM];			//4象限无功电量

	STHISVAL	stHisDirPowMaxDemand[CONHISDATANUM];
	STHISVAL	stHisDirPowMaxDemandFee1[CONHISDATANUM];
	STHISVAL	stHisDirPowMaxDemandFee2[CONHISDATANUM];
	STHISVAL	stHisDirPowMaxDemandFee3[CONHISDATANUM];
	STHISVAL	stHisDirPowMaxDemandFee4[CONHISDATANUM];	//正向有功最大需量

	STHISTIME	stHisDirPowMaxDemandTime[CONHISDATANUM];
	STHISTIME	stHisDirPowMaxDemandTimeFee1[CONHISDATANUM];
	STHISTIME	stHisDirPowMaxDemandTimeFee2[CONHISDATANUM];
	STHISTIME	stHisDirPowMaxDemandTimeFee3[CONHISDATANUM];
	STHISTIME	stHisDirPowMaxDemandTimeFee4[CONHISDATANUM];//正向有功最大需量发生时间

	STHISVAL	stHisDirVarMaxDemand[CONHISDATANUM];
	STHISVAL	stHisDirVarMaxDemandFee1[CONHISDATANUM];
	STHISVAL	stHisDirVarMaxDemandFee2[CONHISDATANUM];
	STHISVAL	stHisDirVarMaxDemandFee3[CONHISDATANUM];
	STHISVAL	stHisDirVarMaxDemandFee4[CONHISDATANUM];	//正向无功最大需量

	STHISTIME	stHisDirVarMaxDemandTime[CONHISDATANUM];
	STHISTIME	stHisDirVarMaxDemandTimeFee1[CONHISDATANUM];
	STHISTIME	stHisDirVarMaxDemandTimeFee2[CONHISDATANUM];
	STHISTIME	stHisDirVarMaxDemandTimeFee3[CONHISDATANUM];
	STHISTIME	stHisDirVarMaxDemandTimeFee4[CONHISDATANUM];//正向无功最大需量发生时间

	STHISVAL	stHisRevPowMaxDemand[CONHISDATANUM];
	STHISVAL	stHisRevPowMaxDemandFee1[CONHISDATANUM];
	STHISVAL	stHisRevPowMaxDemandFee2[CONHISDATANUM];
	STHISVAL	stHisRevPowMaxDemandFee3[CONHISDATANUM];
	STHISVAL	stHisRevPowMaxDemandFee4[CONHISDATANUM];	//反向有功最大需量

	STHISTIME	stHisRevPowMaxDemandTime[CONHISDATANUM];
	STHISTIME	stHisRevPowMaxDemandTimeFee1[CONHISDATANUM];
	STHISTIME	stHisRevPowMaxDemandTimeFee2[CONHISDATANUM];
	STHISTIME	stHisRevPowMaxDemandTimeFee3[CONHISDATANUM];
	STHISTIME	stHisRevPowMaxDemandTimeFee4[CONHISDATANUM];//反向有功最大需量发生时间

	STHISVAL	stHisRevVarMaxDemand[CONHISDATANUM];
	STHISVAL	stHisRevVarMaxDemandFee1[CONHISDATANUM];
	STHISVAL	stHisRevVarMaxDemandFee2[CONHISDATANUM];
	STHISVAL	stHisRevVarMaxDemandFee3[CONHISDATANUM];
	STHISVAL	stHisRevVarMaxDemandFee4[CONHISDATANUM];	//反向无功最大需量

	STHISTIME	stHisRevVarMaxDemandTime[CONHISDATANUM];
	STHISTIME	stHisRevVarMaxDemandTimeFee1[CONHISDATANUM];
	STHISTIME	stHisRevVarMaxDemandTimeFee2[CONHISDATANUM];
	STHISTIME	stHisRevVarMaxDemandTimeFee3[CONHISDATANUM];
	STHISTIME	stHisRevVarMaxDemandTimeFee4[CONHISDATANUM];//反向无功最大需量发生时间

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
	STHISVAL	stHisDirPowLmFee4[CONHISDATANUM];			//正向有功电量（上月）

	STHISVAL	stHisRevPowLmTotal[CONHISDATANUM];
	STHISVAL	stHisRevPowLmFee1[CONHISDATANUM];
	STHISVAL	stHisRevPowLmFee2[CONHISDATANUM];
	STHISVAL	stHisRevPowLmFee3[CONHISDATANUM];
	STHISVAL	stHisRevPowLmFee4[CONHISDATANUM];			//反向有功电量(上月)

	STHISVAL	stHisDirVarLmTotal[CONHISDATANUM];
	STHISVAL	stHisDirVarLmFee1[CONHISDATANUM];
	STHISVAL	stHisDirVarLmFee2[CONHISDATANUM];
	STHISVAL	stHisDirVarLmFee3[CONHISDATANUM];
	STHISVAL	stHisDirVarLmFee4[CONHISDATANUM];			//正向无功电量(上月)

	STHISVAL	stHisRevVarLmTotal[CONHISDATANUM];
	STHISVAL	stHisRevVarLmFee1[CONHISDATANUM];
	STHISVAL	stHisRevVarLmFee2[CONHISDATANUM];
	STHISVAL	stHisRevVarLmFee3[CONHISDATANUM];
	STHISVAL	stHisRevVarLmFee4[CONHISDATANUM];			//反向无功电量(上月)

	STHISVAL	stHisDirVar1Lm[CONHISDATANUM];
	STHISVAL	stHisDirVar1LmFee1[CONHISDATANUM];
	STHISVAL	stHisDirVar1LmFee2[CONHISDATANUM];
	STHISVAL	stHisDirVar1LmFee3[CONHISDATANUM];
	STHISVAL	stHisDirVar1LmFee4[CONHISDATANUM];			//1象限无功电量(上月)

	STHISVAL	stHisDirVar2Lm[CONHISDATANUM];
	STHISVAL	stHisDirVar2LmFee1[CONHISDATANUM];
	STHISVAL	stHisDirVar2LmFee2[CONHISDATANUM];
	STHISVAL	stHisDirVar2LmFee3[CONHISDATANUM];
	STHISVAL	stHisDirVar2LmFee4[CONHISDATANUM];			//2象限无功电量(上月)
	STHISVAL	stHisDirVar3Lm[CONHISDATANUM];
	STHISVAL	stHisDirVar3LmFee1[CONHISDATANUM];
	STHISVAL	stHisDirVar3LmFee2[CONHISDATANUM];
	STHISVAL	stHisDirVar3LmFee3[CONHISDATANUM];
	STHISVAL	stHisDirVar3LmFee4[CONHISDATANUM];			//3象限无功电量(上月)
		
	STHISVAL	stHisDirVar4Lm[CONHISDATANUM];
	STHISVAL	stHisDirVar4LmFee1[CONHISDATANUM];
	STHISVAL	stHisDirVar4LmFee2[CONHISDATANUM];
	STHISVAL	stHisDirVar4LmFee3[CONHISDATANUM];
	STHISVAL	stHisDirVar4LmFee4[CONHISDATANUM];				//4象限无功电量(上月)
	
	STHISVAL	stHisDirPowMaxDemandLm[CONHISDATANUM];
	STHISVAL	stHisDirPowMaxDemandLmFee1[CONHISDATANUM];
	STHISVAL	stHisDirPowMaxDemandLmFee2[CONHISDATANUM];
	STHISVAL	stHisDirPowMaxDemandLmFee3[CONHISDATANUM];
	STHISVAL	stHisDirPowMaxDemandLmFee4[CONHISDATANUM];		//正向有功最大需量(上月)

	STHISTIME	stHisDirPowMaxDemandTimeLm[CONHISDATANUM];
	STHISTIME	stHisDirPowMaxDemandTimeLmFee1[CONHISDATANUM];
	STHISTIME	stHisDirPowMaxDemandTimeLmFee2[CONHISDATANUM];
	STHISTIME	stHisDirPowMaxDemandTimeLmFee3[CONHISDATANUM];
	STHISTIME	stHisDirPowMaxDemandTimeLmFee4[CONHISDATANUM];	//正向有功最大需量发生时间(上月)

	STHISVAL	stHisDirVarMaxDemandLm[CONHISDATANUM];
	STHISVAL	stHisDirVarMaxDemandLmFee1[CONHISDATANUM];
	STHISVAL	stHisDirVarMaxDemandLmFee2[CONHISDATANUM];
	STHISVAL	stHisDirVarMaxDemandLmFee3[CONHISDATANUM];
	STHISVAL	stHisDirVarMaxDemandLmFee4[CONHISDATANUM];		//正向无功最大需量(上月)

	STHISTIME	stHisDirVarMaxDemandTimeLm[CONHISDATANUM];
	STHISTIME	stHisDirVarMaxDemandTimeLmFee1[CONHISDATANUM];
	STHISTIME	stHisDirVarMaxDemandTimeLmFee2[CONHISDATANUM];
	STHISTIME	stHisDirVarMaxDemandTimeLmFee3[CONHISDATANUM];
	STHISTIME	stHisDirVarMaxDemandTimeLmFee4[CONHISDATANUM];	//正向无功最大需量发生时间(上月)

	STHISVAL	stHisRevPowMaxDemandLm[CONHISDATANUM];
	STHISVAL	stHisRevPowMaxDemandLmFee1[CONHISDATANUM];
	STHISVAL	stHisRevPowMaxDemandLmFee2[CONHISDATANUM];
	STHISVAL	stHisRevPowMaxDemandLmFee3[CONHISDATANUM];
	STHISVAL	stHisRevPowMaxDemandLmFee4[CONHISDATANUM];		//反向有功最大需量(上月)

	STHISTIME	stHisRevPowMaxDemandTimeLm[CONHISDATANUM];
	STHISTIME	stHisRevPowMaxDemandTimeLmFee1[CONHISDATANUM];
	STHISTIME	stHisRevPowMaxDemandTimeLmFee2[CONHISDATANUM];
	STHISTIME	stHisRevPowMaxDemandTimeLmFee3[CONHISDATANUM];
	STHISTIME	stHisRevPowMaxDemandTimeLmFee4[CONHISDATANUM];	//反向有功最大需量发生时间(上月)

	STHISVAL	stHisRevVarMaxDemandLm[CONHISDATANUM];
	STHISVAL	stHisRevVarMaxDemandLmFee1[CONHISDATANUM];
	STHISVAL	stHisRevVarMaxDemandLmFee2[CONHISDATANUM];
	STHISVAL	stHisRevVarMaxDemandLmFee3[CONHISDATANUM];
	STHISVAL	stHisRevVarMaxDemandLmFee4[CONHISDATANUM];		//正向无功最大需量(上月)

	STHISTIME	stHisRevVarMaxDemandTimeLm[CONHISDATANUM];
	STHISTIME	stHisRevVarMaxDemandTimeLmFee1[CONHISDATANUM];
	STHISTIME	stHisRevVarMaxDemandTimeLmFee2[CONHISDATANUM];
	STHISTIME	stHisRevVarMaxDemandTimeLmFee3[CONHISDATANUM];
	STHISTIME	stHisRevVarMaxDemandTimeLmFee4[CONHISDATANUM];	//正向无功最大需量发生时间(上月)

	STHISVAL	stHisVolA[CONHISDATANUM];
	STHISVAL	stHisVolB[CONHISDATANUM];
	STHISVAL	stHisVolC[CONHISDATANUM];				//三相电压

	STHISVAL	stHisVolA_Phase[CONHISDATANUM];
	STHISVAL	stHisVolB_Phase[CONHISDATANUM];
	STHISVAL	stHisVolC_Phase[CONHISDATANUM];			//三相电压相角

	STHISVAL	stHisIa[CONHISDATANUM];
	STHISVAL	stHisIb[CONHISDATANUM];
	STHISVAL	stHisIc[CONHISDATANUM];					//三相电流

	STHISVAL	stHisIa_Phase[CONHISDATANUM];
	STHISVAL	stHisIb_Phase[CONHISDATANUM];
	STHISVAL	stHisIc_Phase[CONHISDATANUM];			//三相电流相角

	STHISVAL	stHisInstantPow[CONHISDATANUM];		
	STHISVAL	stHisInstantPowA[CONHISDATANUM];	
	STHISVAL	stHisInstantPowB[CONHISDATANUM];	
	STHISVAL	stHisInstantPowC[CONHISDATANUM];		//瞬时有功

	STHISVAL	stHisInstantVar[CONHISDATANUM];		
	STHISVAL	stHisInstantVarA[CONHISDATANUM];	
	STHISVAL	stHisInstantVarB[CONHISDATANUM];	
	STHISVAL	stHisInstantVarC[CONHISDATANUM];		//瞬时无功

	STHISVAL	stHisPowPulseConst[CONHISDATANUM];
	STHISVAL	stHisVarPulseConst[CONHISDATANUM];			//脉冲常数

	STHISFEERATE stHisFeeRate[CONHISDATANUM];				//费率

	STHISTIME	stHisProgramTime[CONHISDATANUM];			//编程时间
	STHISTIME	stHisMeterClrTime[CONHISDATANUM];			//电表清零时间
	STHISTIME	stHisDemandClrTime[CONHISDATANUM];			//需量清零时间
	STHISTIME	stHisEventClrTime[CONHISDATANUM];			//事件清零时间
	STHISTIME	stHisJiaoshiTime[CONHISDATANUM];			//校时时间
	
	STHISVAL	stHisProgramCount[CONHISDATANUM];			//编程次数
	STHISVAL	stHisMeterClrCount[CONHISDATANUM];			//电表清零次数
	STHISVAL	stHisDemandClrCount[CONHISDATANUM];			//需量清零次数
	STHISVAL	stHisEventClrCount[CONHISDATANUM];			//事件清零次数
	STHISVAL	stHisJiaoshiCount[CONHISDATANUM];			//校时次数
	STHISVAL	stHisBatWorkTime[CONHISDATANUM];			//电池工作时间
	

	STHISSTATE	stHisMeterStatFlag1[CONHISDATANUM];			//电表运行状态字
	STHISSTATE	stHisMeterStatFlag2[CONHISDATANUM];			//电网运行状态字
	STHISSTATE	stHisMeterStatFlag3[CONHISDATANUM];			//电网运行状态字
	STHISSTATE	stHisMeterStatFlag4[CONHISDATANUM];			//电网运行状态字
	STHISSTATE	stHisMeterStatFlag5[CONHISDATANUM];			//电网运行状态字
	STHISSTATE	stHisMeterStatFlag6[CONHISDATANUM];			//电网运行状态字
	STHISSTATE	stHisMeterStatFlag7[CONHISDATANUM];			//电网运行状态字

	STHISTIME	stHisCopyDayTime[CONHISDATANUM];			//抄表日

	STHISVAL	stHisLostVolNum[CONHISDATANUM];				//断相次数
	STHISVAL	stHisLostVolANum[CONHISDATANUM];			//A相断相次数
	STHISVAL	stHisLostVolBNum[CONHISDATANUM];			//B相断相次数
	STHISVAL	stHisLostVolCNum[CONHISDATANUM];			//C相断相次数

	STHISVAL	stHisLostVolTime[CONHISDATANUM];			//断相时间累计值
	STHISVAL	stHisLostVolATime[CONHISDATANUM];			//A相断相时间累计值
	STHISVAL	stHisLostVolBTime[CONHISDATANUM];			//B相断相时间累计值
	STHISVAL	stHisLostVolCTime[CONHISDATANUM];			//C相断相时间累计值

	STHISTIME	stHisLastVolOffBegTime[CONHISDATANUM];		//最近一次断相起始时刻
	STHISTIME	stHisLastVolAOffBegTime[CONHISDATANUM];		//A相最近一次断相起始时刻
	STHISTIME	stHisLastVolBOffBegTime[CONHISDATANUM];		//B相最近一次断相起始时刻
	STHISTIME	stHisLastVolCOffBegTime[CONHISDATANUM];		//C相最近一次断相起始时刻

	STHISTIME	stHisLastVolOffEndTime[CONHISDATANUM];		//最近一次断相结束时刻
	STHISTIME	stHisLastVolAOffEndTime[CONHISDATANUM];		//A相最近一次断相结束时刻
	STHISTIME	stHisLastVolBOffEndTime[CONHISDATANUM];		//B相最近一次断相结束时刻
	STHISTIME	stHisLastVolCOffEndTime[CONHISDATANUM];		//C相最近一次断相结束时刻

	STHISTIME	stHisDate[CONHISDATANUM];					//表计的日月年
	STHISTIME	stHisTime[CONHISDATANUM];					//表计的时分秒

}STMETERTIMERVAL;

typedef struct _STMETERSUMVALGROUP
{
	STHISSUMVALGROUP	stInstantPowSumGroup[CONHISDATANUM];	//瞬时有功总加
	STHISSUMVALGROUP	stInstantVarSumGroup[CONHISDATANUM];	//瞬时无功总加

	STHISSUMVALGROUP	stPowValueSumGroup[CONHISDATANUM];		//有功电量总加
	STHISSUMVALGROUP	stPowFee1ValueSumGroup[CONHISDATANUM];	//有功电量总加
	STHISSUMVALGROUP	stPowFee2ValueSumGroup[CONHISDATANUM];	//有功电量总加
	STHISSUMVALGROUP	stPowFee3ValueSumGroup[CONHISDATANUM];	//有功电量总加
	STHISSUMVALGROUP	stPowFee4ValueSumGroup[CONHISDATANUM];	//有功电量总加

	STHISSUMVALGROUP	stVarValueSumGroup[CONHISDATANUM];		//无功电量总加
	STHISSUMVALGROUP	stVarFee1ValueSumGroup[CONHISDATANUM];	//无功电量总加
	STHISSUMVALGROUP	stVarFee2ValueSumGroup[CONHISDATANUM];	//无功电量总加
	STHISSUMVALGROUP	stVarFee3ValueSumGroup[CONHISDATANUM];	//无功电量总加
	STHISSUMVALGROUP	stVarFee4ValueSumGroup[CONHISDATANUM];	//无功电量总加
	
	STHISSUMVALGROUP	stPowHourSumGroup[CONHISHOURDATANUM];	//有功电量总加
}STMETERSUMVALGROUP;



typedef	struct	_STHISRUNDATA					//统计数据
{
	STDATETIME	stRecordTime;
	unsigned short	usReserve0;

	unsigned long	ulHisDirPowTotal;
	unsigned long	ulHisDirPowFee1;
	unsigned long	ulHisDirPowFee2;
	unsigned long	ulHisDirPowFee3;
	unsigned long	ulHisDirPowFee4;				//正向有功电量

	unsigned long	ulHisRevPowTotal;
	unsigned long	ulHisRevPowFee1;
	unsigned long	ulHisRevPowFee2;
	unsigned long	ulHisRevPowFee3;
	unsigned long	ulHisRevPowFee4;				//反向有功电量

	unsigned long	ulHisDirVarTotal;
	unsigned long	ulHisDirVarFee1;
	unsigned long	ulHisDirVarFee2;
	unsigned long	ulHisDirVarFee3;
	unsigned long	ulHisDirVarFee4;				//正向无功电量

	unsigned long	ulHisRevVarTotal;
	unsigned long	ulHisRevVarFee1;
	unsigned long	ulHisRevVarFee2;
	unsigned long	ulHisRevVarFee3;
	unsigned long	ulHisRevVarFee4;				//反向无功电量

	unsigned long	ulHisLastDayDirPowTotal;
	unsigned long	ulHisLastDayDirPowFee1;
	unsigned long	ulHisLastDayDirPowFee2;
	unsigned long	ulHisLastDayDirPowFee3;
	unsigned long	ulHisLastDayDirPowFee4;				//上一天正向有功电量

	unsigned long	ulHisLastDayRevPowTotal;
	unsigned long	ulHisLastDayRevPowFee1;
	unsigned long	ulHisLastDayRevPowFee2;
	unsigned long	ulHisLastDayRevPowFee3;
	unsigned long	ulHisLastDayRevPowFee4;				//上一天反向有功电量

	unsigned long	ulHisLastDayDirVarTotal;
	unsigned long	ulHisLastDayDirVarFee1;
	unsigned long	ulHisLastDayDirVarFee2;
	unsigned long	ulHisLastDayDirVarFee3;
	unsigned long	ulHisLastDayDirVarFee4;				//上一天正向无功电量

	unsigned long	ulHisLastDayRevVarTotal;
	unsigned long	ulHisLastDayRevVarFee1;
	unsigned long	ulHisLastDayRevVarFee2;
	unsigned long	ulHisLastDayRevVarFee3;
	unsigned long	ulHisLastDayRevVarFee4;				//上一天反向无功电量

	unsigned long	ulHisDirVar1;
	unsigned long	ulHisDirVar1Fee1;
	unsigned long	ulHisDirVar1Fee2;
	unsigned long	ulHisDirVar1Fee3;
	unsigned long	ulHisDirVar1Fee4;				//1象限无功电量

	unsigned long	ulHisDirVar2;
	unsigned long	ulHisDirVar2Fee1;
	unsigned long	ulHisDirVar2Fee2;
	unsigned long	ulHisDirVar2Fee3;
	unsigned long	ulHisDirVar2Fee4;				//2象限无功电量

	unsigned long	ulHisDirVar3;
	unsigned long	ulHisDirVar3Fee1;
	unsigned long	ulHisDirVar3Fee2;
	unsigned long	ulHisDirVar3Fee3;
	unsigned long	ulHisDirVar3Fee4;				//3象限无功电量

	unsigned long	ulHisDirVar4;
	unsigned long	ulHisDirVar4Fee1;
	unsigned long	ulHisDirVar4Fee2;
	unsigned long	ulHisDirVar4Fee3;
	unsigned long	ulHisDirVar4Fee4;				//4象限无功电量

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
	unsigned long	ulHisDirPowMaxDemandFee4;		//正向有功最大需量

	unsigned long	ulHisDirVarMaxDemand;
	unsigned long	ulHisDirVarMaxDemandFee1;
	unsigned long	ulHisDirVarMaxDemandFee2;
	unsigned long	ulHisDirVarMaxDemandFee3;
	unsigned long	ulHisDirVarMaxDemandFee4;		//正向无功最大需量

	unsigned long	ulHisRevPowMaxDemand;
	unsigned long	ulHisRevPowMaxDemandFee1;
	unsigned long	ulHisRevPowMaxDemandFee2;
	unsigned long	ulHisRevPowMaxDemandFee3;
	unsigned long	ulHisRevPowMaxDemandFee4;		//反向有功最大需量

	unsigned long	ulHisRevVarMaxDemand;
	unsigned long	ulHisRevVarMaxDemandFee1;
	unsigned long	ulHisRevVarMaxDemandFee2;
	unsigned long	ulHisRevVarMaxDemandFee3;
	unsigned long	ulHisRevVarMaxDemandFee4;		//反向无功最大需量

	STDATETIME	stHisDirPowMaxDemandTime;
	STDATETIME	stHisDirPowMaxDemandTimeFee1;
	STDATETIME	stHisDirPowMaxDemandTimeFee2;
	STDATETIME	stHisDirPowMaxDemandTimeFee3;
	STDATETIME	stHisDirPowMaxDemandTimeFee4;	//正向有功最大需量发生时间

	STDATETIME	stHisDirVarMaxDemandTime;
	STDATETIME	stHisDirVarMaxDemandTimeFee1;
	STDATETIME	stHisDirVarMaxDemandTimeFee2;
	STDATETIME	stHisDirVarMaxDemandTimeFee3;
	STDATETIME	stHisDirVarMaxDemandTimeFee4;	//正向无功最大需量发生时间

	STDATETIME	stHisRevPowMaxDemandTime;
	STDATETIME	stHisRevPowMaxDemandTimeFee1;
	STDATETIME	stHisRevPowMaxDemandTimeFee2;
	STDATETIME	stHisRevPowMaxDemandTimeFee3;
	STDATETIME	stHisRevPowMaxDemandTimeFee4;	//反向有功最大需量发生时间

	STDATETIME	stHisRevVarMaxDemandTime;
	STDATETIME	stHisRevVarMaxDemandTimeFee1;
	STDATETIME	stHisRevVarMaxDemandTimeFee2;
	STDATETIME	stHisRevVarMaxDemandTimeFee3;
	STDATETIME	stHisRevVarMaxDemandTimeFee4;	//反向无功最大需量发生时间

	STHISVAL	stHisMaxInstantPow;				//瞬时有功统计
	STHISVAL	stHisMaxInstantPowA;			//采用STHISVAL结构，其中STHISVAL.stDateTime表示最大值发生时间，后面表示相同
	STHISVAL	stHisMaxInstantPowB;	
	STHISVAL	stHisMaxInstantPowC;				
	unsigned short	usHisZeroTimeInstantPow;
	unsigned short	usHisZeroTimeInstantPowA;	
	unsigned short	usHisZeroTimeInstantPowB;	
	unsigned short	usHisZeroTimeInstantPowC;

	STHISVAL	stHisMaxDemand;					//总以及分相需量
	STHISVAL	stHisMaxDemandA;	
	STHISVAL	stHisMaxDemandB;	
	STHISVAL	stHisMaxDemandC;				

	STHISVAL	stHisMaxVolA;					//电压统计
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
	unsigned long	ulHisSumVolA;				//电压累加，用于平均电压计算
	unsigned long	ulHisSumVolB;
	unsigned long	ulHisSumVolC;

	STHISVAL	stHisMaxCurrA;					//电流统计
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

	unsigned short	usHisUnbalanceTimeVolt;		//电压不平衡时间
	unsigned short	usHisUnbalanceTimeCurr;		//电流不平衡时间
	STHISVAL 		stHisMaxUnBalanceVol;
	STHISVAL		stHisMaxUnBalanceCur;

	unsigned short	usHisErrUpTimeSPower;		//视在功率越上上限时间
	unsigned short	usHisNorUpTimeSPower;		//视在功率越上限时间

	STHISVAL 		stHisMaxLoadRate;			//最大负载率
	STHISVAL 		stHisMinLoadRate;			//最小负载率

	unsigned short usHisVolBreakNum;			//断相次数
	unsigned short usHisVolABreakNum;			//A相断相次数
	unsigned short usHisVolBBreakNum;			//B相断相次数
	unsigned short usHisVolCBreakNum;			//C相断相次数
	unsigned short usHisVolBreakTime;			//断相时间累计值
	unsigned short usHisVolABreakTime;			//A相断相时间累计值
	unsigned short usHisVolBBreakTime;			//B相断相时间累计值
	unsigned short usHisVolCBreakTime;			//C相断相时间累计值
	STDATETIME	stHisLastVolBreakBegTime;		//最近一次断相起始时刻
	STDATETIME	stHisLastVolABreakBegTime;		//A相最近一次断相起始时刻
	STDATETIME	stHisLastVolBBreakBegTime;		//B相最近一次断相起始时刻
	STDATETIME	stHisLastVolCBreakBegTime;		//C相最近一次断相起始时刻
	STDATETIME	stHisLastVolBreakEndTime;		//最近一次断相结束时刻
	STDATETIME	stHisLastVolABreakEndTime;		//A相最近一次断相结束时刻
	STDATETIME	stHisLastVolBBreakEndTime;		//B相最近一次断相结束时刻
	STDATETIME	stHisLastVolCBreakEndTime;		//C相最近一次断相结束时刻

	unsigned short usHisPowRateSect1Time;		//功率因数分布区域1累计时间（功率因数 < 定值1）
	unsigned short usHisPowRateSect2Time;		//功率因数分布区域2累计时间（定值1≤功率因数<定值2） 
	unsigned short usHisPowRateSect3Time;		//功率因数分布区域3累计时间（功率因数≥定值2） 

	unsigned short usHisTerPowerOnTime;			//终端日供电时间
	unsigned short usHisTerResetNum;			//终端日复位累计次数 
	unsigned short usHisMonthEnergyCtrlTripNum;	//月电控跳闸次数
	unsigned short usHisBuyEnergyCtrlTripNum;	//购电控跳闸次数
	unsigned short usHisPowerCtrlTripNum;		//功控跳闸次数
	unsigned short usHisRemoteCtrlTripNum;		//遥控跳闸次数

	unsigned long ulHisFluxBytes;				//流量字节数
}STHISRUNDATA;		//840 bytes	大约 +80

//typedef	struct	_STHISSUMGROUPRUNDATA
//{
//	STDATETIME	stRecordTime;
//	unsigned short	usReserve0;
//
//	float	fSumGroupPowTotal;
//	float	fSumGroupPowFee1;
//	float	fSumGroupPowFee2;
//	float	fSumGroupPowFee3;
//	float	fSumGroupPowFee4;				//总加有功电量示值
//
//	float	fSumGroupVarTotal;
//	float	fSumGroupVarFee1;
//	float	fSumGroupVarFee2;
//	float	fSumGroupVarFee3;
//	float	fSumGroupVarFee4;				//总加无功电量示值
//
//	float	fSumGroupLastDayPowTotal;
//	float	fSumGroupLastDayPowFee1;
//	float	fSumGroupLastDayPowFee2;
//	float	fSumGroupLastDayPowFee3;
//	float	fSumGroupLastDayPowFee4;				//上一天总加有功电量示值
//
//	float	fSumGroupLastDayVarTotal;
//	float	fSumGroupLastDayVarFee1;
//	float	fSumGroupLastDayVarFee2;
//	float	fSumGroupLastDayVarFee3;
//	float	fSumGroupLastDayVarFee4;				//上一天总加无功电量示值
//
//	float	fSumGroupMaxInstantPow;					//最大总加有功统计
//	STDATETIME	stSumGroupMaxTimeInstantPow;
//
//	float	fSumGroupMinInstantPow;					//最小总加统计
//	STDATETIME	stSumGroupMinTimeInstantPow;
//
//	unsigned short	usSumGroupZeroTimeInstantPow;	//总加功率为零持续时间
//
//	unsigned short	usSumGroupPowerOverTime;			//总加功率超定值时间
//	unsigned short	usSumGroupEnergyOverTime;			//总加月电量超定值时间
//	float	fSumGroupPowerOverAccValTotal;				//功率超定值月电量累计
//	float	fSumGroupEnergyOverAccValTotal;				//月电量超定值月电量累计
//	float	fSumGroupPowerOverStartValTotal;			//功率超定值开始示值
//	float	fSumGroupEnergyOverStartValTotal;			//月电量超定值开始示值	//持续越限则保持改值，越限恢复则修改该值修改电量累计，由恢复到越限则修改改值
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
//	unsigned long	ulHisPowFee4;				//电量
//
//	unsigned long	ulHisLastDayPowTotal;
//	unsigned long	ulHisLastDayPowFee1;
//	unsigned long	ulHisLastDayPowFee2;
//	unsigned long	ulHisLastDayPowFee3;
//	unsigned long	ulHisLastDayPowFee4;				//上一天电量
//
//	unsigned long	ulHisPowMaxDemand;
//	unsigned long	ulHisPowMaxDemandFee1;
//	unsigned long	ulHisPowMaxDemandFee2;
//	unsigned long	ulHisPowMaxDemandFee3;
//	unsigned long	ulHisPowMaxDemandFee4;		//最大需量
//
//	STDATETIME	stHisPowMaxDemandTime;
//	STDATETIME	stHisPowMaxDemandTimeFee1;
//	STDATETIME	stHisPowMaxDemandTimeFee2;
//	STDATETIME	stHisPowMaxDemandTimeFee3;
//	STDATETIME	stHisPowMaxDemandTimeFee4;		//最大需量发生时间
//
//	STHISVAL	stHisMaxInstantPow;				//瞬时有功统计//采用STHISVAL结构，其中STHISVAL.stDateTime表示最大值发生时间，后面表示相同
//	unsigned short	usHisZeroTimeInstantPow;
//
//	STHISVAL	stHisMaxDemand;					//统计需量
//}STHISPULSERUNDATA;

//typedef	struct	_STHISPULSEALLDATA
//{
//	STHISPULSERUNDATA   stHisPulseRunData[CON_PULSE_NUM];
//}STHISPULSEALLDATA;

typedef	struct	_STHISCQSUPRUNDATA				//重庆表增补
{
	STDATETIME	stRecordTime;
	unsigned short	usReserve0;
	unsigned long	ulHisDirPowA;			//A相有功总
	unsigned long	ulHisDirPowB;			//B相有功总
	unsigned long	ulHisDirPowC;			//C相有功总
	unsigned long	ulHisVarLA;				//A相感性无功总
	unsigned long	ulHisVarLB;				//B相感性无功总
	unsigned long	ulHisVarLC;				//C相感性无功总
	unsigned long	ulHisVarCA;				//A相容性无功总
	unsigned long	ulHisVarCB;				//B相容性无功总
	unsigned long	ulHisVarCC;				//C相容性无功总
	unsigned long	ulHisInstantS;			//视在功率总
	unsigned long	ulHisInstantSA;			//A相视在功率
	unsigned long	ulHisInstantSB;			//B相视在功率
	unsigned long	ulHisInstantSC;			//C相视在功率
}STHISCQSUPRUNDATA;

typedef	struct	_STHISCQSUPALLDATA
{
	STHISCQSUPRUNDATA   stHisCqSupRunData[CON_METER_NUM];	//重庆表增补
}STHISCQSUPALLDATA;


//日初时间、月初时间，用提前量计算日末时间、月末时间
#define CON_DAYDATA_HOUR			0				//日数据处理时间
#define CON_DAYDATA_MINUTE			8
#define CON_DAYDATA_MINUTE_SUB		14				//提前时间，单位分钟
#define CON_MONTHDATA_DAY			1				//月数据处理时间
#define CON_MONTHDATA_HOUR			0
#define CON_MONTHDATA_MINUTE		9
#define CON_MONTHDATA_MINUTE_SUB	15				//提前时间，单位分钟
#define CON_MINUTE_DIFF				3				//处理可以有时间偏差

#define CON_DAYDATA_TYPE     0//日冻结
#define CON_MONTHDATA_TYPE   1//月冻结
#define CON_FROZEDATA_TYPE   2//抄表日冻结
#define CON_FROZE_METERCOPY_TYPE   3//抄表日抄表时间冻结

#define CON_DAYDATA_NUM				36//32				//
#define CON_MONTHDATA_NUM			16//13				//
#define CON_DAYFROZENDATA_NUM		12//6				//最多6点，限制每月最多3个冻结日，至少有两月
//日月冻结数据保存到EE里相关信息 指针及时间信息
//#define CON_DAY_MONTH_INFO_BASE_ADDR (CON_GRTASK_PARA_BASE_ADDR+CON_GRTASK_PARA_BASE_BOLCK)
//
#define CON_DAYMONTH_INDEX_LEN       (2+1)//+CRC
#define CON_DAYMONTH_TIME_LEN        (6+1)//+CRC
//
#define CON_DAYDATA_INDEX_ADDR       0
#define CON_DAYDATA_INDEX_LEN        CON_DAYMONTH_INDEX_LEN//+CRC
#define CON_DAYDATA_TIME_ADDR        (CON_DAYDATA_INDEX_ADDR+CON_DAYDATA_INDEX_LEN)
#define CON_DAYDATA_TIME_LEN         CON_DAYMONTH_TIME_LEN//(6+1)//+CRC
#define CON_DAYDATA_TIME_BLOCK_LEN   (CON_DAYDATA_TIME_LEN*(CON_DAYDATA_NUM+1))//多预留一个时间空间
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
//每个测量点所占空间长度
#define CON_DAYMONTH_DATA_LEN_MP            (CON_DAYDATA_INDEX_LEN*3+CON_DAYDATA_TIME_BLOCK_LEN+CON_MONTHDATA_TIME_BLOCK_LEN+CON_DAYFROZENDATA_TIME_BLOCK_LEN)

//#define CON_DFPAGE_SUMG_HISDATA_PAGENUM		16		//150字节*（32+13）每块16页，固定8k
//#define CON_DFPAGE_PULSE_HISDATA_PAGENUM	16		//150字节*（32+13）每块16页，固定8k
//
//#define CON_DFPAGE_HEAD_TER_HISDATA		(CON_DFPAGE_HEAD_HISDATA+CON_DFPAGE_HISDATA_PAGENUM*CON_METER_NUM)	//前面表占用8块
//#define CON_DFPAGE_HEAD_SUMG_HISDATA	(CON_DFPAGE_HEAD_TER_HISDATA+CON_DFPAGE_HISDATA_PAGENUM)			//终端占用1块
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
{//总加
	STDAYLISTINDEX			stSumGDayDataIndex;
	STMONTHLISTINDEX		stSumGMonthDataIndex;
}STHISSUMGROUPDATAINDEX;

typedef	struct	_STHISTERDATAINDEX
{//终端
	STDAYFROZENLISTINDEX	stDayFrozenDataIndex;
	STDAYLISTINDEX			stTerDayDataIndex;
	STMONTHLISTINDEX		stTerMonthDataIndex;
}STHISTERDATAINDEX;

typedef	struct	_STHISPULSEDATAINDEX
{//脉冲
	STDAYLISTINDEX			stPulseDayDataIndex;
	STMONTHLISTINDEX		stPulseMonthDataIndex;
}STHISPULSEDATAINDEX;

//---------------------------------------------------------------------
__EXTERN void RunData_Init(void);
__EXTERN void RunData_Init_All(void);
__EXTERN void RunData_Meter_Init(unsigned char ucMeterNo);

//正向有功电量
__EXTERN unsigned char Add_History_DirPower(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
__EXTERN unsigned char Add_History_DirPower_LastValid(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//反向有功电量
__EXTERN unsigned char Add_History_UnDirPower(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
__EXTERN unsigned char Add_History_UnDirPower_LastValid(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//正向无功电量
__EXTERN unsigned char Add_History_DirVar(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
__EXTERN unsigned char Add_History_DirVar_LastValid(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//反向无功电量
__EXTERN unsigned char Add_History_UnDirVar(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//1象限无功电量
__EXTERN unsigned char Add_History_DirVar1(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//2象限无功电量
__EXTERN unsigned char Add_History_DirVar2(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//3象限无功电量
__EXTERN unsigned char Add_History_DirVar3(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//4象限无功电量
__EXTERN unsigned char Add_History_DirVar4(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//电压
__EXTERN unsigned char Add_History_I(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucPhaseNo);
//电流
__EXTERN unsigned char Add_History_V(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucPhaseNo);
//瞬时有功、无功
__EXTERN unsigned char Add_History_InstantPow(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo);
__EXTERN unsigned char Add_History_InstantVar(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo);
//正向有功最大需量
__EXTERN unsigned char Add_History_DirPowMaxDemand(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//正向有功最大需量发生时间
__EXTERN unsigned char Add_History_DirPowMaxDemandTime(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucFeeNo);
//正向无功最大需量
__EXTERN unsigned char Add_History_DirVarMaxDemand(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//正向无功最大需量发生时间
__EXTERN unsigned char Add_History_DirVarMaxDemandTime(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucFeeNo);
//反向有功最大需量
__EXTERN unsigned char Add_History_UnDirPowMaxDemand(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//反向有功最大需量发生时间
__EXTERN unsigned char Add_History_UnDirPowMaxDemandTime(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucFeeNo);
//反向无功最大需量
__EXTERN unsigned char Add_History_UnDirVarMaxDemand(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//反向无功最大需量发生时间
__EXTERN unsigned char Add_History_UnDirVarMaxDemandTime(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucFeeNo);
//正向有功电量（上月）
__EXTERN unsigned char Add_History_DirPowerLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//反向有功电量(上月)
__EXTERN unsigned char Add_History_UnDirPowerLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//正向无功电量(上月)
__EXTERN unsigned char Add_History_DirVarLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//反向无功电量(上月)
__EXTERN unsigned char Add_History_UnDirVarLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//一象限无功(上月)
__EXTERN unsigned char Add_History_DirVar1Lm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//二象限无功(上月)
__EXTERN unsigned char Add_History_DirVar2Lm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//三象限无功(上月)
__EXTERN unsigned char Add_History_DirVar3Lm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//四象限无功(上月)
__EXTERN unsigned char Add_History_DirVar4Lm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//正向有功最大需量(上月)
__EXTERN unsigned char Add_History_DirPowMaxDemandLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//正向有功最大需量发生时间(上月)
__EXTERN unsigned char Add_History_DirPowMaxDemandTimeLm(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucFeeNo);
//正向无功最大需量(上月)
__EXTERN unsigned char Add_History_DirVarMaxDemandLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//正向无功最大需量发生时间(上月)
__EXTERN unsigned char Add_History_DirVarMaxDemandTimeLm(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucFeeNo);
//反向有功最大需量(上月)
__EXTERN unsigned char Add_History_UnDirPowMaxDemandLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//反向有功最大需量发生时间(上月)
__EXTERN unsigned char Add_History_UnDirPowMaxDemandTimeLm(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucFeeNo);
//反向无功最大需量(上月)
__EXTERN unsigned char Add_History_UnDirVarMaxDemandLm(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucFeeNo);
//反向无功最大需量发生时间(上月)
__EXTERN unsigned char Add_History_UnDirVarMaxDemandTimeLm(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucFeeNo);
//有功、无功脉冲常数
__EXTERN unsigned char Add_History_PowPulseConst(unsigned char ucMeterNo,unsigned long lVal);
__EXTERN unsigned char Add_History_VarPulseConst(unsigned char ucMeterNo,unsigned long lVal);
//费率
__EXTERN unsigned char Add_History_FeeRate(unsigned char ucMeterNo,STFEERATELIST	stFeeRateList);
//编程时间
__EXTERN unsigned char Add_History_ProgramTime(unsigned char ucMeterNo,STDATETIME	stDateTime);
//需量清零时间
__EXTERN unsigned char Add_History_DemandClrTime(unsigned char ucMeterNo,STDATETIME	stDateTime);
//编程次数
__EXTERN unsigned char Add_History_ProgramCount(unsigned char ucMeterNo,unsigned long	lVal);
//需量清零次数
__EXTERN unsigned char Add_History_DemandClrCount(unsigned char ucMeterNo,unsigned long	lVal);
//电表清零时间
__EXTERN unsigned char Add_History_MeterClrTime(unsigned char ucMeterNo,STDATETIME	stDateTime);
//事件清零时间
__EXTERN unsigned char Add_History_EventClrTime(unsigned char ucMeterNo,STDATETIME	stDateTime);
//校时时间
__EXTERN unsigned char Add_History_JiaoshiTime(unsigned char ucMeterNo,STDATETIME	stDateTime);
//电表清零次数
__EXTERN unsigned char Add_History_MeterClrCount(unsigned char ucMeterNo,unsigned long	lVal);
//事件清零次数
__EXTERN unsigned char Add_History_EventClrCount(unsigned char ucMeterNo,unsigned long	lVal);
//校时次数
__EXTERN unsigned char Add_History_JiaoshiCount(unsigned char ucMeterNo,unsigned long	lVal);
//电池工作时间
__EXTERN unsigned char Add_History_BatWorkTime(unsigned char ucMeterNo,unsigned long	lVal);
//电表运行状态字
__EXTERN unsigned char Add_History_MeterStatFlag(unsigned char ucMeterNo,unsigned char	ucStateFlag,unsigned char ucFlagNum);
//电网运行状态字
//__EXTERN unsigned char Add_History_NetStatFlag(unsigned char ucMeterNo,unsigned char	ucStateFlag);
//抄表日
__EXTERN unsigned char Add_History_CopyDayTime(unsigned char ucMeterNo,STDATETIME	stDateTime);
//断相次数
__EXTERN unsigned char Add_History_LostVolNum(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo);
//断相时间累计值
__EXTERN unsigned char Add_History_LostVolTime(unsigned char ucMeterNo,unsigned long lVal,unsigned char ucNo);
//最近一次断相起始时刻
__EXTERN unsigned char Add_History_LastVolOffBegTime(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucNo);
//最近一次断相结束时刻
__EXTERN unsigned char Add_History_LastVolOffEndTime(unsigned char ucMeterNo,STDATETIME	stDateTime,unsigned char ucNo);
//年月日
__EXTERN unsigned char Add_History_MeterDate(unsigned char ucMeterNo,STDATETIME	stDateTime);
//时分秒
__EXTERN unsigned char Add_History_MeterTime(unsigned char ucMeterNo,STDATETIME	stDateTime);

///*******************************************总加组数据获取*************************************/
//__EXTERN unsigned char Add_History_SumValInstantPowGroup(void);
//__EXTERN unsigned char Add_History_SumValInstantVarGroup(void);
//__EXTERN unsigned char Add_History_SumValPowValueGroup(void);
//__EXTERN unsigned char Add_History_SumValVarValueGroup(void);
//__EXTERN unsigned char Add_History_SumValHourPowValueGroup(void);

/************************************************************************************/
////__EXTERN STHISSUMVALGROUP Get_History_InstantPowSumVal(unsigned char ucPos);	//瞬时有功总加组
//__EXTERN STHISSUMVALGROUP Get_History_PowValSumVal(unsigned char ucPos,unsigned char ucFeeNo);	//有功电量总加组
////__EXTERN STHISSUMVALGROUP Get_History_InstantVarSumVal(unsigned char ucPos);	//瞬时无功总加组
//__EXTERN STHISSUMVALGROUP Get_History_VarValSumVal(unsigned char ucPos,unsigned char ucFeeNo);	//无功电量总加组
//__EXTERN STHISSUMVALGROUP Get_History_PowValSumVal_LM(unsigned char ucPos,unsigned char ucFeeNo);//上月的
//__EXTERN STHISSUMVALGROUP Get_History_VarValSumVal_LM(unsigned char ucPos,unsigned char ucFeeNo);//上月的
//__EXTERN STHISSUMVALGROUP Get_History_PowValHourSumVal(unsigned char ucPos);

//正向有功电量
__EXTERN STHISVAL Get_History_DirPower_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//读高精度电量0.0000KWH
__EXTERN STHISVAL Get_History_DirPower_byMPNo_High(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//反向有功电量
__EXTERN STHISVAL Get_History_UnDirPower_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//正向无功电量
__EXTERN STHISVAL Get_History_DirVar_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//反向无功电量
__EXTERN STHISVAL Get_History_UnDirVar_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//1象限无功电量
__EXTERN STHISVAL Get_History_DirVar1_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//2象限无功电量
__EXTERN STHISVAL Get_History_DirVar2_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//3象限无功电量
__EXTERN STHISVAL Get_History_DirVar3_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//4象限无功电量
__EXTERN STHISVAL Get_History_DirVar4_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//正向有功最大需量
__EXTERN STHISVAL Get_History_DirPowMaxDemand_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//正向有功最大需量发生时间
__EXTERN STHISTIME Get_History_DirPowMaxDemandTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//正向无功最大需量
__EXTERN STHISVAL Get_History_DirVarMaxDemand_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//正向无功最大需量发生时间
__EXTERN STHISTIME Get_History_DirVarMaxDemandTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//反向有功最大需量
__EXTERN STHISVAL Get_History_UnDirPowMaxDemand_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//反向有功最大需量发生时间
__EXTERN STHISTIME Get_History_UnDirPowMaxDemandTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//反向无功最大需量
__EXTERN STHISVAL Get_History_UnDirVarMaxDemand_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//反向无功最大需量发生时间
__EXTERN STHISTIME Get_History_UnDirVarMaxDemandTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);

__EXTERN STHISVAL Get_History_DirPowA_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
__EXTERN STHISVAL Get_History_DirPowB_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
__EXTERN STHISVAL Get_History_DirPowC_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
__EXTERN STHISVAL Get_History_DirPowALm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
__EXTERN STHISVAL Get_History_DirPowBLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
__EXTERN STHISVAL Get_History_DirPowCLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);

//正向有功电量（上月）
__EXTERN STHISVAL Get_History_DirPowerLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//反向有功电量(上月)
__EXTERN STHISVAL Get_History_UnDirPowerLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//正向无功电量(上月)
__EXTERN STHISVAL Get_History_DirVarLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//反向无功电量(上月)
__EXTERN STHISVAL Get_History_UnDirVarLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//1象限无功电量(上月)
__EXTERN STHISVAL Get_History_DirVar1Lm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//2象限无功电量(上月)
__EXTERN STHISVAL Get_History_DirVar2Lm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//3象限无功电量(上月)
__EXTERN STHISVAL Get_History_DirVar3Lm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//4象限无功电量(上月)
__EXTERN STHISVAL Get_History_DirVar4Lm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//正向有功最大需量(上月)
__EXTERN STHISVAL Get_History_DirPowMaxDemandLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//正向有功最大需量发生时间(上月)
__EXTERN STHISTIME Get_History_DirPowMaxDemandTimeLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//正向无功最大需量(上月)
__EXTERN STHISVAL Get_History_DirVarMaxDemandLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//正向无功最大需量发生时间(上月)
__EXTERN STHISTIME Get_History_DirVarMaxDemandTimeLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//反向有功最大需量(上月)
__EXTERN STHISVAL Get_History_UnDirPowMaxDemandLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//反向有功最大需量发生时间(上月)
__EXTERN STHISTIME Get_History_UnDirPowMaxDemandTimeLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//正向无功最大需量(上月)
__EXTERN STHISVAL Get_History_UnDirVarMaxDemandLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//正向无功最大需量发生时间(上月)
__EXTERN STHISTIME Get_History_UnDirVarMaxDemandTimeLm_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);
//电压
__EXTERN STHISVAL Get_History_V_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//电流
__EXTERN STHISVAL Get_History_I_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//相角
__EXTERN STHISVAL Get_History_V_Phase_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
__EXTERN STHISVAL Get_History_I_Phase_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//正向瞬时有功、无功
__EXTERN STHISVAL Get_History_InstantPow_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
__EXTERN STHISVAL Get_History_InstantVar_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//反向瞬时有功、无功
__EXTERN STHISVAL Get_History_RevInstantPow_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
__EXTERN STHISVAL Get_History_RevInstantVar_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//正、反向15分钟平均有功
//__EXTERN STHISVAL Get_History_AvePow15_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//__EXTERN STHISVAL Get_History_AveVar15_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//__EXTERN STHISVAL Get_History_RevAvePow15_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//__EXTERN STHISVAL Get_History_RevAveVar15_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//有功、无功脉冲常数
//__EXTERN STHISVAL Get_History_PowPulseConst_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//__EXTERN STHISVAL Get_History_VarPulseConst_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//费率
//__EXTERN STHISFEERATE Get_History_FeeRate_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//编程时间
__EXTERN STHISTIME Get_History_ProgramTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//需量清零时间
__EXTERN STHISTIME Get_History_DemandClrTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//编程次数
__EXTERN STHISVAL Get_History_ProgramCount_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//需量清零次数
__EXTERN STHISVAL Get_History_DemandClrCount_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//电表清零时间
__EXTERN STHISTIME Get_History_MeterClrTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//事件清零时间
__EXTERN STHISTIME Get_History_EventClrTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//校时时间
__EXTERN STHISTIME Get_History_JiaoshiTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//电表清零次数
__EXTERN STHISVAL Get_History_MeterClrCount_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//事件清零次数
__EXTERN STHISVAL Get_History_EventClrCount_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//校时次数
__EXTERN STHISVAL Get_History_JiaoshiCount_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//电池工作时间
__EXTERN STHISVAL Get_History_BatWorkTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//电表运行状态字
__EXTERN STHISSTATE Get_History_MeterStatFlag_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFlagNum);
//电网运行状态字
//__EXTERN STHISSTATE Get_History_NetStatFlag_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//抄表日
//__EXTERN STHISTIME Get_History_CopyDayTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//断相次数
__EXTERN STHISVAL Get_History_VolBreakNum_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//断相时间累计值
__EXTERN STHISVAL Get_History_VolBreakTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//最近一次断相起始时刻
__EXTERN STHISTIME Get_History_LastVolBreakBegTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//最近一次断相结束时刻
__EXTERN STHISTIME Get_History_LastVolBreakEndTime_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//年月日
__EXTERN STHISTIME Get_History_Date_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//时分秒
__EXTERN STHISTIME Get_History_Time_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//门节点状态
//__EXTERN STHISSTATE Get_History_DoorStat_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//终端停上电状态
__EXTERN STHISSTATE Get_History_PowerStat_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//开关状态
//__EXTERN STHISSTATE Get_History_SwitchStat_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucSwitchNo);
//电流反极性
//__EXTERN STHISSTATE Get_History_PolReverseStat_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//CT一次侧短路
//__EXTERN STHISSTATE Get_History_ShortCir1Stat_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//CT二次侧开路
////__EXTERN STHISSTATE Get_History_OpenCir2Stat_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//CT二次侧短路
//__EXTERN STHISSTATE Get_History_ShortCir2Stat_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//电压回路逆相序
//__EXTERN STHISSTATE Get_History_VolCirReveStat_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//显示电压
//__EXTERN STHISVAL Get_History_V_Disp_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//显示电流
//__EXTERN STHISVAL Get_History_I_Disp_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);
//显示无功
//__EXTERN STHISVAL Get_History_InstantVar_Disp_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucNo);

//脉冲0.001精度电量采集
//__EXTERN STHISVAL Get_P1000_History_DirPower_byMPNo(unsigned char ucMPNo,unsigned char ucPos,unsigned char ucFeeNo);

__EXTERN void ClearHisVal(STHISVAL *pstHisVal);
__EXTERN void ClearHisTime(STHISTIME *pstHisTime);
__EXTERN void ClearHisFeeRate(STHISFEERATE *pstHisFeeRate);
__EXTERN void ClearFeeRateList(STFEERATELIST *pstFeeRateList);
__EXTERN void ClearHisState(STHISSTATE *pstHisState);
//__EXTERN void ClearHisSumValGroup(STHISSUMVALGROUP *pstHisSumValGroup);

//__EXTERN unsigned char  Get_Status_TerPowLow(void);


//-------------任务计算------------------------
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

//-------------脉冲量处理----------------------
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

//-------------数据处理数据统计----------------------
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

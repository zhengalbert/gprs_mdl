/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Alarm_ERC_Info.c
 Description    : 告警数据转换成国网上行格式
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2013-01-08    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

//static unsigned char Alarm_Get_SumGroupNo_byAlrCode(unsigned short usAlrCode);
static unsigned char Alarm_DataConvert_AllDataInit(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
static unsigned char Alarm_DataConvert_ParaLost(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
static unsigned char Alarm_DataConvert_ParaModify(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_DataConvert_SwitchChange(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_DataConvert_VoltOver(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_DataConvert_CurrOver(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_DataConvert_VoltBreak(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_DataConvert_SPowOver(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_DataConvert_UnBalanceOverUp(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_DataConvert_MeterParaModify(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
static unsigned char Alarm_DataConvert_MeterErrInfo(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
static unsigned char Alarm_DataConvert_MeterTimeErr(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_DataConvert_SumCompOverLimit(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_DataConvert_SumInstant(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_DataConvert_SumPow(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_DataConvert_CurrentErr(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
static unsigned char Alarm_DataConvert_VolCirReverse(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
static unsigned char Alarm_DataConvert_PowerOn(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
static unsigned char Alarm_DataConvert_TerErr(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
static unsigned char Alarm_DataConvert_PasswordErr(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_DataConvert_MeterFly(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_DataConvert_MeterStop(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_DataConvert_MeterDown(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_DataConvert_RemoteCtrl(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_DataConvert_PowerCtrl(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_DataConvert_EnergyCtrl(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_DataConvert_BuyPowEnerRec(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_DataConvert_EnerAlr(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
static unsigned char Alarm_DataConvert_MeterCopy(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
static unsigned char Alarm_DataConvert_FluxLimit(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_DataConvert_PowDiffOver(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
//static unsigned char Alarm_DataConvert_DataCopy(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);
static unsigned char Alarm_DataConvert_MeterState(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge);

//告警数据转换,这里把告警信息变成条文放入结构件中，用于上送和存储
unsigned char Alarm_DataConvert(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
{
	unsigned char ucRet=RET_SUCCESS;
	switch(pstAlrEnsureJudge->usAlrCode)
	{
	case CONALRCODE_ALLDATAINIT:			//数据初始化
	case CONALRCODE_SOFTWARECHANGE:
		ucRet=Alarm_DataConvert_AllDataInit(pstAlrRecord,pstAlrEnsureJudge);
		break;
	case CONALRCODE_PARA_LOST:				//终端参数丢失
		ucRet=Alarm_DataConvert_ParaLost(pstAlrRecord,pstAlrEnsureJudge);
		break;
	case CONALRCODE_PARA_MODIFY:			//终端重要参数变更
		ucRet=Alarm_DataConvert_ParaModify(pstAlrRecord,pstAlrEnsureJudge);
		break;
//	case CONALRCODE_DOOROPEN	:			//门开报警
//	case CONALRCODE_DOORCLOSE	:			//门关报警
//	case CONALRCODE_SWITCH0OPEN	:			//开关0开报警
//	case CONALRCODE_SWITCH0CLOSE:			//开关0关报警
//	case CONALRCODE_SWITCH1OPEN	:			//开关1开报警 
//	case CONALRCODE_SWITCH1CLOSE:			//开关1关报警 
//	case CONALRCODE_SWITCH2OPEN	:			//开关2开报警 
//	case CONALRCODE_SWITCH2CLOSE:			//开关2关报警 
//	case CONALRCODE_SWITCH3OPEN	:			//开关3开报警 
//	case CONALRCODE_SWITCH3CLOSE:			//开关3关报警 
//	case CONALRCODE_SWITCH4OPEN	:			//开关4开报警 
//	case CONALRCODE_SWITCH4CLOSE:			//开关4关报警 
//	case CONALRCODE_SWITCH5OPEN	:			//开关5开报警 
//	case CONALRCODE_SWITCH5CLOSE:			//开关5关报警 
//	case CONALRCODE_SWITCH6OPEN	:			//开关6开报警 
//	case CONALRCODE_SWITCH6CLOSE:			//开关6关报警 
//		ucRet=Alarm_DataConvert_SwitchChange(pstAlrRecord,pstAlrEnsureJudge);
//		break;
//	case CONALRCODE_FEERATE:				//费率更改
//	case CONALRCODE_PROGRAMTIME:			//编程时间更改
//	case CONALRCODE_COPYDAYTIME:			//抄表日
//	case CONALRCODE_PULSECONST:				//脉冲常数
//	case CONALRCODE_DEMANDTIMECLR:			//需量清零时间更改
//		ucRet=Alarm_DataConvert_MeterParaModify(pstAlrRecord,pstAlrEnsureJudge);
//		break;
// 	case CONALRCODE_POLREVERSEA	:			//反极性
// 	case CONALRCODE_POLREVERSEB	:	
// 	case CONALRCODE_POLREVERSEC	:
//	case CONALRCODE_SHORTCIRA1	:			//CT一次侧短路
//	case CONALRCODE_SHORTCIRB1	:
//	case CONALRCODE_SHORTCIRC1	:
//	case CONALRCODE_SHORTCIRA2	:			//CT二次侧短路
//	case CONALRCODE_SHORTCIRB2	:		
//	case CONALRCODE_SHORTCIRC2	:
//	case CONALRCODE_OPENCIRA2	:			//CT二次侧开路
//	case CONALRCODE_OPENCIRB2	:
//	case CONALRCODE_OPENCIRC2	:
// 		ucRet=Alarm_DataConvert_CurrentErr(pstAlrRecord,pstAlrEnsureJudge);
// 		break;
// 	case CONALRCODE_VABREAK:				//A电压断相
// 	case CONALRCODE_VBBREAK:				//B电压断相
// 	case CONALRCODE_VCBREAK:				//C电压断相
// 	case CONALRCODE_VALACK:					//A电压缺相
// 	case CONALRCODE_VBLACK:					//B电压缺相
// 	case CONALRCODE_VCLACK:					//C电压缺相
// 		ucRet=Alarm_DataConvert_VoltBreak(pstAlrRecord,pstAlrEnsureJudge);
// 		break;
	case CONALRCODE_VOLCIRREVERSE:			//电压回路逆相序
		ucRet=Alarm_DataConvert_VolCirReverse(pstAlrRecord,pstAlrEnsureJudge);
		break;
	case CONALRCODE_METERTIMEERR:			//表计时钟异常
		ucRet=Alarm_DataConvert_MeterTimeErr(pstAlrRecord,pstAlrEnsureJudge);
		break;
	case CONALRCODE_POWERON:				//停电
	case CONALRCODE_POWEROFF:				//上电
		ucRet=Alarm_DataConvert_PowerOn(pstAlrRecord,pstAlrEnsureJudge);
		break;
//	case	CONALRCODE_IUNBLANCE:			//电流不平衡越限
//	case	CONALRCODE_UUNBLANCE:			//电压不平衡越限
//		ucRet=Alarm_DataConvert_UnBalanceOverUp(pstAlrRecord,pstAlrEnsureJudge);
//		break;
	case CONALRCODE_LOSTVOLNUM:				//断相次数
	case CONALRCODE_METERBATTLOW:			//电池电压低
	case CONALRCODE_DEMANDTIMENUM:			//需量清零次数
	case CONALRCODE_PROGRAMNUM:				//编程次数变化
		ucRet=Alarm_DataConvert_MeterErrInfo(pstAlrRecord,pstAlrEnsureJudge);
		break;
//	case CONALRCODE_METERFLY:				//表计飞走
//		ucRet=Alarm_DataConvert_MeterFly(pstAlrRecord,pstAlrEnsureJudge);
//		break;
//	case CONALRCODE_METERSTOP:				//表计停走
//		ucRet=Alarm_DataConvert_MeterStop(pstAlrRecord,pstAlrEnsureJudge);
//		break;
//	case CONALRCODE_METERDOWN:				//表计倒走
//		ucRet=Alarm_DataConvert_MeterDown(pstAlrRecord,pstAlrEnsureJudge);
//		break;
//	case CONALRCODE_VAOVERUP:				//电压越限_Va
//	case CONALRCODE_VBOVERUP:				//电压越限_Vb
//	case CONALRCODE_VCOVERUP:				//电压越限_Vc
//	case CONALRCODE_VAOVERDOWN:				//电压越限_Va(下限)
//	case CONALRCODE_VBOVERDOWN:				//电压越限_Vb(下限)
//	case CONALRCODE_VCOVERDOWN:				//电压越限_Vc(下限)
//		ucRet=Alarm_DataConvert_VoltOver(pstAlrRecord,pstAlrEnsureJudge);
//		break;
//	case CONALRCODE_IAOVERUP:				//电流越限_Ia
//	case CONALRCODE_IBOVERUP:				//电流越限_Ib
//	case CONALRCODE_ICOVERUP:				//电流越限_Ic
//	case CONALRCODE_IAERROVERUP:			//电流越上上限_Ia
//	case CONALRCODE_IBERROVERUP:			//电流越上上限_Ib
//	case CONALRCODE_ICERROVERUP:			//电流越上上限_Ic
//		ucRet=Alarm_DataConvert_CurrOver(pstAlrRecord,pstAlrEnsureJudge);
//		break;
//	case CONALRCODE_SPOWNORUP:				//视在功率越限
//	case CONALRCODE_SPOWERRUP:				//视在功率越上上限
//		ucRet=Alarm_DataConvert_SPowOver(pstAlrRecord,pstAlrEnsureJudge);
//		break;
//	case CONALRCODE_SUMCOMPOVERLIMIT:
//	case CONALRCODE_SUMCOMPOVERLIMIT+1:
//	case CONALRCODE_SUMCOMPOVERLIMIT+2:
//	case CONALRCODE_SUMCOMPOVERLIMIT+3:
//	case CONALRCODE_SUMCOMPOVERLIMIT+4:
//	case CONALRCODE_SUMCOMPOVERLIMIT+5:
//	case CONALRCODE_SUMCOMPOVERLIMIT+6:
//	case CONALRCODE_SUMCOMPOVERLIMIT+7:
//		ucRet=Alarm_DataConvert_SumCompOverLimit(pstAlrRecord,pstAlrEnsureJudge);
//	break;

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
//		ucRet=Alarm_DataConvert_SumInstant(pstAlrRecord,pstAlrEnsureJudge);
//		break;
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
//		ucRet=Alarm_DataConvert_SumPow(pstAlrRecord,pstAlrEnsureJudge);
//		break;
	case	CONALRCODE_TERERR_RAM:				//终端主板内存故障
	case	CONALRCODE_TERERR_CLOCK:			//时钟故障
	case	CONALRCODE_TERERR_MAINCOMM:			//主板通信故障
	case	CONALRCODE_TERERR_485ERROR:			//485抄表故障
	case	CONALRCODE_TERERR_DISPLAY:			//显示板故障
		ucRet=Alarm_DataConvert_TerErr(pstAlrRecord,pstAlrEnsureJudge);
		break;
	case	CONALRCODE_COMM_PASSWORDERR:		//密码错误
		ucRet=Alarm_DataConvert_PasswordErr(pstAlrRecord,pstAlrEnsureJudge);
		break;
//	case	CONALRCODE_REMOTECTRL:					//遥控跳闸记录
//		ucRet=Alarm_DataConvert_RemoteCtrl(pstAlrRecord,pstAlrEnsureJudge);
//		break;
//	case	CONALRCODE_POWERCTRL:					//功控跳闸记录
//		ucRet=Alarm_DataConvert_PowerCtrl(pstAlrRecord,pstAlrEnsureJudge);
//		break;
//	case	CONALRCODE_ENERCTRL:					//电控跳闸记录
//		ucRet=Alarm_DataConvert_EnergyCtrl(pstAlrRecord,pstAlrEnsureJudge);
//		break;
//	case	CONALRCODE_BUYPOWPARA:					//购电参数设置
//		ucRet=Alarm_DataConvert_BuyPowEnerRec(pstAlrRecord,pstAlrEnsureJudge);
//		break;
//	case	CONALRCODE_ENERALR:						//ERC23 电控告警事件
//		ucRet=Alarm_DataConvert_EnerAlr(pstAlrRecord,pstAlrEnsureJudge);
//		break;
	case	CONALRCODE_METERCOPY:					//ERC31 485抄表失败事件记录
		ucRet=Alarm_DataConvert_MeterCopy(pstAlrRecord,pstAlrEnsureJudge);
		break;
	case	CONALRCODE_FLUXLIMIT:					//ERC32 月通信超流量记录
		ucRet=Alarm_DataConvert_FluxLimit(pstAlrRecord,pstAlrEnsureJudge);
		break;
//	case	CONALRCODE_POWDIFFOVER:// ERC28	电能量超差记录
//		ucRet=Alarm_DataConvert_PowDiffOver(pstAlrRecord,pstAlrEnsureJudge);
//		break;
	case	CONALRCODE_METERSTATECHANGE://ERC33 电表状态字变更记录
		ucRet=Alarm_DataConvert_MeterState(pstAlrRecord,pstAlrEnsureJudge);
		break;
	default:
		break;
	}

	return ucRet;
}

////数据仅仅复制
//unsigned char Alarm_DataConvert_DataCopy(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	if(pstAlrEnsureJudge->ucAlrDataItemNum>=CONALRTAKEITEMNUM)
//		pstAlrRecord->ucAlrDataItemNum=CONALRTAKEITEMNUM;
//	else
//		pstAlrRecord->ucAlrDataItemNum=pstAlrEnsureJudge->ucAlrDataItemNum;
//	for(i=0;i<pstAlrRecord->ucAlrDataItemNum;i++)
//	{
//		memcpy(pstAlrRecord->stAlrDataItem[i].ucDataContent,
//			pstAlrEnsureJudge->stAlrDataItem[i].ucDataContent,
//			pstAlrEnsureJudge->stAlrDataItem[i].usDataLen);
//		pstAlrRecord->stAlrDataItem[i].usDataLen=pstAlrEnsureJudge->stAlrDataItem[i].usDataLen;
//	}
//	return ucRet;
//}

////电压越限
//unsigned char Alarm_DataConvert_VoltOver(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucTmp1=0,ucTmp2=0;
//	unsigned long ulVal=0;
//	STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
//	pstAlrRecord->ucAlrDataItemNum=5;
//	if(stTerAdjPara.ucMPNoFromZero==1)
//		ucTmp1=pstAlrRecord->ucMPNo+1;
//	else
//		ucTmp1=pstAlrRecord->ucMPNo;
//	switch(pstAlrEnsureJudge->usAlrCode)
//	{
//	case CONALRCODE_VAOVERUP		:		//电压越限_Va
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_VolaUpOver==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;						//0x80	越限=1，恢复=0
//		ucTmp2=0x40+0x01;
//		break;
//	case CONALRCODE_VBOVERUP		:		//电压越限_Vb
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_VolbUpOver==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;
//		ucTmp2=0x40+0x02;
//		break;
//	case CONALRCODE_VCOVERUP		:		//电压越限_Vc
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_VolcUpOver==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;
//		ucTmp2=0x40+0x04;
//		break;
//	case CONALRCODE_VAOVERDOWN		:		//电压越限_Va(下限)
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_VolaDownOver==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;
//		ucTmp2=0x80+0x01;
//		break;
//	case CONALRCODE_VBOVERDOWN		:		//电压越限_Vb(下限)
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_VolbDownOver==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;
//		ucTmp2=0x80+0x02;
//		break;
//	case CONALRCODE_VCOVERDOWN		:		//电压越限_Vc(下限)
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_VolcDownOver==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;
//		ucTmp2=0x80+0x04;
//		break;
//	default:
//		ucTmp2=0x40+0x01;
//	}
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=ucTmp1&0x7f;
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[1]=ucTmp1&0x80;
//	pstAlrRecord->stAlrDataItem[0].usDataLen=2;
//	pstAlrRecord->stAlrDataItem[1].ucDataContent[0]=ucTmp2;
//	pstAlrRecord->stAlrDataItem[1].usDataLen=1;
//
//	memcpy((unsigned char *)&ulVal,pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,2);
//	pstAlrRecord->stAlrDataItem[2].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_2B_DT07(pstAlrRecord->stAlrDataItem[2].ucDataContent,0,ulVal);
//
//	memcpy((unsigned char *)&ulVal,pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,2);
//	pstAlrRecord->stAlrDataItem[3].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_2B_DT07(pstAlrRecord->stAlrDataItem[3].ucDataContent,0,ulVal);
//
//	memcpy((unsigned char *)&ulVal,pstAlrEnsureJudge->stAlrDataItem[2].ucDataContent,2);
//	pstAlrRecord->stAlrDataItem[4].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_2B_DT07(pstAlrRecord->stAlrDataItem[4].ucDataContent,0,ulVal);
//
//	return ucRet;
//}

////电流越限
//unsigned char Alarm_DataConvert_CurrOver(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucTmp1=0,ucTmp2=0;
//	unsigned long ulVal=0;
//	STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
//	pstAlrRecord->ucAlrDataItemNum=5;
//	if(stTerAdjPara.ucMPNoFromZero==1)
//		ucTmp1=pstAlrRecord->ucMPNo+1;
//	else
//		ucTmp1=pstAlrRecord->ucMPNo;
//	switch(pstAlrEnsureJudge->usAlrCode)
//	{
//	case CONALRCODE_IAOVERUP		:		//电流越限_Ia
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_IaOverLoad==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;						//0x80	越限=1，恢复=0
//		ucTmp2=0x80+0x01;
//		break;
//	case CONALRCODE_IBOVERUP		:		//电流越限_Ib
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_IbOverLoad==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;
//		ucTmp2=0x80+0x02;
//		break;
//	case CONALRCODE_ICOVERUP		:		//电流越限_Ic
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_IcOverLoad==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;
//		ucTmp2=0x80+0x04;
//		break;
//	case CONALRCODE_IAERROVERUP:			//电流越上上限_Ia
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_IaErrOverLoad==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;
//		ucTmp2=0x40+0x01;
//		break;
//	case CONALRCODE_IBERROVERUP:			//电流越上上限_Ib
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_IbErrOverLoad==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;
//		ucTmp2=0x40+0x02;
//		break;
//	case CONALRCODE_ICERROVERUP:			//电流越上上限_Ic
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_IcErrOverLoad==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;
//		ucTmp2=0x40+0x04;
//		break;
//	default:
//		ucTmp2=0x80+0x01;
//	}
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=ucTmp1&0x7f;
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[1]=ucTmp1&0x80;
//	pstAlrRecord->stAlrDataItem[0].usDataLen=2;
//	pstAlrRecord->stAlrDataItem[1].ucDataContent[0]=ucTmp2;
//	pstAlrRecord->stAlrDataItem[1].usDataLen=1;
//	
//	memcpy((unsigned char *)&ulVal,pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,2);
////	ulVal *= 10;
//	pstAlrRecord->stAlrDataItem[2].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_3B(pstAlrRecord->stAlrDataItem[2].ucDataContent,0,ulVal);
//	
//	memcpy((unsigned char *)&ulVal,pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,2);
////	ulVal *= 10;
//	pstAlrRecord->stAlrDataItem[3].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_3B(pstAlrRecord->stAlrDataItem[3].ucDataContent,0,ulVal);
//
//	memcpy((unsigned char *)&ulVal,pstAlrEnsureJudge->stAlrDataItem[2].ucDataContent,2);
////	ulVal *= 10;
//	pstAlrRecord->stAlrDataItem[4].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_3B(pstAlrRecord->stAlrDataItem[4].ucDataContent,0,ulVal);
//
//	return ucRet;
//}

////断相
//unsigned char Alarm_DataConvert_VoltBreak(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucTmp1=0,ucTmp2=0;
//	unsigned long ulVal=0;
//	STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
//	pstAlrRecord->ucAlrDataItemNum=9;
//	if(stTerAdjPara.ucMPNoFromZero==1)
//		ucTmp1=pstAlrRecord->ucMPNo+1;				
//	else
//		ucTmp1=pstAlrRecord->ucMPNo;				
//	switch(pstAlrEnsureJudge->usAlrCode)
//	{
//	case CONALRCODE_VABREAK		:		//A相电压断相
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_VolaBreak==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;				//0x80	越限=1，恢复=0
//		ucTmp2=0x40+0x01;
//		break;
//	case CONALRCODE_VBBREAK		:		//B相电压断相
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_VolbBreak==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;
//		ucTmp2=0x40+0x02;
//		break;
//	case CONALRCODE_VCBREAK		:		//C相电压断相
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_VolcBreak==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;
//		ucTmp2=0x40+0x04;
//		break;
//	case CONALRCODE_VALACK		:		//A相电压缺相
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_VolaLack==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;				//0x80	越限=1，恢复=0
//		ucTmp2=0x80+0x01;
//		break;
//	case CONALRCODE_VBLACK		:		//B相电压缺相
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_VolbLack==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;
//		ucTmp2=0x80+0x02;
//		break;
//	case CONALRCODE_VCLACK		:		//C相电压缺相
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_VolcLack==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;
//		ucTmp2=0x80+0x04;
//		break;
//	default:
//		ucTmp2=0x40+0x01;
//	}
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=ucTmp1 & 0x7f;
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[1]=ucTmp1 & 0x80;
//	pstAlrRecord->stAlrDataItem[0].usDataLen=2;
//	pstAlrRecord->stAlrDataItem[1].ucDataContent[0]=ucTmp2;
//	pstAlrRecord->stAlrDataItem[1].usDataLen=1;
//	
//	memcpy((unsigned char *)&ulVal,pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,2);
//	pstAlrRecord->stAlrDataItem[2].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_2B_DT07(pstAlrRecord->stAlrDataItem[2].ucDataContent,0,ulVal);
//	memcpy((unsigned char *)&ulVal,pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,2);
//	pstAlrRecord->stAlrDataItem[3].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_2B_DT07(pstAlrRecord->stAlrDataItem[3].ucDataContent,0,ulVal);
//	memcpy((unsigned char *)&ulVal,pstAlrEnsureJudge->stAlrDataItem[2].ucDataContent,2);
//	pstAlrRecord->stAlrDataItem[4].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_2B_DT07(pstAlrRecord->stAlrDataItem[4].ucDataContent,0,ulVal);
//
//	memcpy((unsigned char *)&ulVal,pstAlrEnsureJudge->stAlrDataItem[3].ucDataContent,2);
////	ulVal *= 10;
//	pstAlrRecord->stAlrDataItem[5].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_3B(pstAlrRecord->stAlrDataItem[5].ucDataContent,0,ulVal);
//	memcpy((unsigned char *)&ulVal,pstAlrEnsureJudge->stAlrDataItem[4].ucDataContent,2);
////	ulVal *= 10;
//	pstAlrRecord->stAlrDataItem[6].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_3B(pstAlrRecord->stAlrDataItem[6].ucDataContent,0,ulVal);
//	memcpy((unsigned char *)&ulVal,pstAlrEnsureJudge->stAlrDataItem[5].ucDataContent,2);
////	ulVal *= 10;
//	pstAlrRecord->stAlrDataItem[7].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_3B(pstAlrRecord->stAlrDataItem[7].ucDataContent,0,ulVal);
//
//	memcpy((unsigned char *)&ulVal,pstAlrEnsureJudge->stAlrDataItem[6].ucDataContent,4);
//	pstAlrRecord->stAlrDataItem[8].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_5B_DT14(pstAlrRecord->stAlrDataItem[8].ucDataContent,0,ulVal);
//
//	return ucRet;
//}

//电压回路逆相序
unsigned char Alarm_DataConvert_VolCirReverse(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
{
	unsigned char ucRet=RET_SUCCESS;

	unsigned char ucTmp1=0;
	unsigned long ulVal=0;
	STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
	pstAlrRecord->ucAlrDataItemNum=3;
	if(stTerAdjPara.ucMPNoFromZero==1)
		ucTmp1=pstAlrRecord->ucMPNo+1;
	else
		ucTmp1=pstAlrRecord->ucMPNo;
	if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_VolCirReve==CONALR_STAT_HAPPEN)
		ucTmp1 |= 0x80;						//0x80	越限=1，恢复=0

	pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=ucTmp1&0X7F;
	pstAlrRecord->stAlrDataItem[0].ucDataContent[1]=ucTmp1&0X80;
	pstAlrRecord->stAlrDataItem[0].usDataLen=2;

//del 20061109
//	memset(pstAlrRecord->stAlrDataItem[1].ucDataContent,0xEE,12);			//电压电流角度6项12个字节，数据格式05
//	pstAlrRecord->stAlrDataItem[1].usDataLen=12;

//add start 20061109		固定显示几个角度
	if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_VolCirReve==CONALR_STAT_HAPPEN)
	{//逆相序
//azh 先用三相四线
//		if(ADECheckLineType_Is3P3L()==CON_STATUS_YES)				//需要保证先ADEInit获得正确的接线类型
//		{//三相三线
//			pstAlrRecord->stAlrDataItem[1].ucDataContent[0]=0x00;
//			pstAlrRecord->stAlrDataItem[1].ucDataContent[1]=0x00;
//			pstAlrRecord->stAlrDataItem[1].ucDataContent[2]=0xEE;
//			pstAlrRecord->stAlrDataItem[1].ucDataContent[3]=0xEE;
//			pstAlrRecord->stAlrDataItem[1].ucDataContent[4]=0x00;
//			pstAlrRecord->stAlrDataItem[1].ucDataContent[5]=0x06;
//		}
//		else
//		{//三相四线
			pstAlrRecord->stAlrDataItem[1].ucDataContent[0]=0x00;
			pstAlrRecord->stAlrDataItem[1].ucDataContent[1]=0x00;
			pstAlrRecord->stAlrDataItem[1].ucDataContent[2]=0x00;
			pstAlrRecord->stAlrDataItem[1].ucDataContent[3]=0x24;
			pstAlrRecord->stAlrDataItem[1].ucDataContent[4]=0x00;
			pstAlrRecord->stAlrDataItem[1].ucDataContent[5]=0x12;
//		}
	}
	else
	{//正常
//		if(ADECheckLineType_Is3P3L()==CON_STATUS_YES)				//需要保证先ADEInit获得正确的接线类型
//		{//三相三线
//			pstAlrRecord->stAlrDataItem[1].ucDataContent[0]=0x00;
//			pstAlrRecord->stAlrDataItem[1].ucDataContent[1]=0x00;
//			pstAlrRecord->stAlrDataItem[1].ucDataContent[2]=0xEE;
//			pstAlrRecord->stAlrDataItem[1].ucDataContent[3]=0xEE;
//			pstAlrRecord->stAlrDataItem[1].ucDataContent[4]=0x00;
//			pstAlrRecord->stAlrDataItem[1].ucDataContent[5]=0x30;
//		}
//		else
//		{//三相四线
			pstAlrRecord->stAlrDataItem[1].ucDataContent[0]=0x00;
			pstAlrRecord->stAlrDataItem[1].ucDataContent[1]=0x00;
			pstAlrRecord->stAlrDataItem[1].ucDataContent[2]=0x00;
			pstAlrRecord->stAlrDataItem[1].ucDataContent[3]=0x12;
			pstAlrRecord->stAlrDataItem[1].ucDataContent[4]=0x00;
			pstAlrRecord->stAlrDataItem[1].ucDataContent[5]=0x24;
//		}
	}

	memset(pstAlrRecord->stAlrDataItem[1].ucDataContent+6,0xEE,6);			//电流角度3项6个字节，数据格式05
	pstAlrRecord->stAlrDataItem[1].usDataLen=12;
//add end 20061109

	memcpy((unsigned char *)&ulVal,pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,4);
	pstAlrRecord->stAlrDataItem[2].usDataLen
		=(unsigned char)SetMainComm_Buf_lVal_5B_DT14(pstAlrRecord->stAlrDataItem[2].ucDataContent,0,ulVal);

	return ucRet;
}

////视在功率越限
//unsigned char Alarm_DataConvert_SPowOver(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucTmp=0,ucTmp2=0;
//	unsigned long ulVal=0;
//	unsigned long lPower=0,lVar=0;
//	STMPLIMIT stMPLimit;
//	STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
//	pstAlrRecord->ucAlrDataItemNum=4;
//	if(stTerAdjPara.ucMPNoFromZero==1)
//		ucTmp=pstAlrRecord->ucMPNo+1;
//	else
//		ucTmp=pstAlrRecord->ucMPNo;
//	if(pstAlrEnsureJudge->usAlrCode==CONALRCODE_SPOWERRUP)
//	{
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_SPowErrUp==CONALR_STAT_HAPPEN)
//			ucTmp|=0x80;					//越限=1，恢复=0
//		ucTmp2=0x40;						//0x40	
//	}else
//	{
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_SPowOverUp==CONALR_STAT_HAPPEN)
//			ucTmp|=0x80;					//越限=1，恢复=0
//		ucTmp2=0x80;						//0x80	
//	}
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=ucTmp&0X7F;
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[1]=ucTmp&0X80;
//	pstAlrRecord->stAlrDataItem[0].usDataLen=2;
//	pstAlrRecord->stAlrDataItem[1].ucDataContent[0]=ucTmp2;
//	pstAlrRecord->stAlrDataItem[1].usDataLen=1;
//
//	memcpy((unsigned char *)&lPower,pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,pstAlrEnsureJudge->stAlrDataItem[0].usDataLen);
//	memcpy((unsigned char *)&lVar,pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,pstAlrEnsureJudge->stAlrDataItem[0].usDataLen);
//	if(lPower!=CONINVALIDVAL && lVar!=CONINVALIDVAL)
//		ulVal = GetSPow(lPower,lVar);
//	else
//		ulVal = CONINVALIDVAL;
//
//	pstAlrRecord->stAlrDataItem[2].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_3B(pstAlrRecord->stAlrDataItem[2].ucDataContent,0,ulVal);
//
//	stMPLimit =	RunPara_GetMPLimit(pstAlrRecord->ucMPNo);
//	if(pstAlrEnsureJudge->usAlrCode==CONALRCODE_SPOWERRUP)
//		ulVal = stMPLimit.ulPowErrUpLimit;
//	else
//		ulVal = stMPLimit.ulPowUpLimit;
//	pstAlrRecord->stAlrDataItem[3].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_3B(pstAlrRecord->stAlrDataItem[3].ucDataContent,0,ulVal);
//
//	return ucRet;
//}

//参数初始化
unsigned char Alarm_DataConvert_AllDataInit(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
{
	unsigned char ucRet=RET_SUCCESS;
	pstAlrRecord->ucAlrDataItemNum=1;
	memcpy(pstAlrRecord->stAlrDataItem[0].ucDataContent,
		pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,
		pstAlrEnsureJudge->stAlrDataItem[0].usDataLen);
	pstAlrRecord->stAlrDataItem[0].usDataLen=pstAlrEnsureJudge->stAlrDataItem[0].usDataLen;
	return ucRet;
}

//参数丢失
unsigned char Alarm_DataConvert_ParaLost(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
{
	unsigned char ucRet=RET_SUCCESS;
	pstAlrRecord->ucAlrDataItemNum=1;
	memcpy(pstAlrRecord->stAlrDataItem[0].ucDataContent,
		pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,
		pstAlrEnsureJudge->stAlrDataItem[0].usDataLen);
	pstAlrRecord->stAlrDataItem[0].usDataLen=pstAlrEnsureJudge->stAlrDataItem[0].usDataLen;
	return ucRet;
}

//参数修改
unsigned char Alarm_DataConvert_ParaModify(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
{
	unsigned char ucRet=RET_SUCCESS;
	pstAlrRecord->ucAlrDataItemNum=2;
	memcpy(pstAlrRecord->stAlrDataItem[0].ucDataContent,
		pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,
		pstAlrEnsureJudge->stAlrDataItem[0].usDataLen);
	pstAlrRecord->stAlrDataItem[0].usDataLen=pstAlrEnsureJudge->stAlrDataItem[0].usDataLen;
	memcpy(pstAlrRecord->stAlrDataItem[1].ucDataContent,
		pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,
		pstAlrEnsureJudge->stAlrDataItem[1].usDataLen);
	pstAlrRecord->stAlrDataItem[1].usDataLen=pstAlrEnsureJudge->stAlrDataItem[1].usDataLen;
	return ucRet;
}
//开关量变位
//unsigned char Alarm_DataConvert_SwitchChange(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucTmp1=0,ucTmp2=0;
//	unsigned char ucSwitchStat;
//	pstAlrRecord->ucAlrDataItemNum=2;
//	switch(pstAlrEnsureJudge->usAlrCode)
//	{
//	case CONALRCODE_DOOROPEN	:			//门开报警
//	case CONALRCODE_DOORCLOSE	:			//门关报警
//		ucTmp1 |= 0x80;
//		break;
//	case CONALRCODE_SWITCH0OPEN	:			//开关0开报警
//	case CONALRCODE_SWITCH0CLOSE:			//开关0关报警
//		ucTmp1 |= 0x01;
//		break;
//	case CONALRCODE_SWITCH1OPEN	:			//开关1开报警 
//	case CONALRCODE_SWITCH1CLOSE:			//开关1关报警 
//		ucTmp1 |= 0x02;
//		break;
//	case CONALRCODE_SWITCH2OPEN	:			//开关2开报警 
//	case CONALRCODE_SWITCH2CLOSE:			//开关2关报警 
//		ucTmp1 |= 0x04;
//		break;
//	case CONALRCODE_SWITCH3OPEN	:			//开关3开报警 
//	case CONALRCODE_SWITCH3CLOSE:			//开关3关报警 
//		ucTmp1 |= 0x08;
//		break;
//	case CONALRCODE_SWITCH4OPEN	:			//开关4开报警 
//	case CONALRCODE_SWITCH4CLOSE:			//开关4关报警 
//		ucTmp1 |= 0x10;
//		break;
//	case CONALRCODE_SWITCH5OPEN	:			//开关5开报警 
//	case CONALRCODE_SWITCH5CLOSE:			//开关5关报警 
//		ucTmp1 |= 0x20;
//		break;
//	case CONALRCODE_SWITCH6OPEN	:			//开关6开报警 
//	case CONALRCODE_SWITCH6CLOSE:			//开关6关报警 
//		ucTmp1 |= 0x40;
//		break;
//	}
//	ucSwitchStat = Get_CurrentSwitchStat(0);
//	if(ucSwitchStat == CON_SWITCH_STAT_CLOSE)
//		 ucTmp2 |= 0x01;
//	ucSwitchStat = Get_CurrentSwitchStat(1);
//	if(ucSwitchStat == CON_SWITCH_STAT_CLOSE)
//		 ucTmp2 |= 0x02;
//	ucSwitchStat = Get_CurrentSwitchStat(2);
//	if(ucSwitchStat == CON_SWITCH_STAT_CLOSE)
//		 ucTmp2 |= 0x04;
//	ucSwitchStat = Get_CurrentSwitchStat(3);
//	if(ucSwitchStat == CON_SWITCH_STAT_CLOSE)
//		 ucTmp2 |= 0x08;
//	if(RunPara_GetSwitchPara_PulseIsTypeSwitch(0)==CON_STATUS_YES)
//	{
//		ucSwitchStat = Get_CurrentSwitchStat(4);
//		if(ucSwitchStat == CON_SWITCH_STAT_CLOSE)
//			 ucTmp2 |= 0x10;
//	}
//	if(RunPara_GetSwitchPara_PulseIsTypeSwitch(1)==CON_STATUS_YES)
//	{
//		ucSwitchStat = Get_CurrentSwitchStat(5);
//		if(ucSwitchStat == CON_SWITCH_STAT_CLOSE)
//			 ucTmp2 |= 0x20;
//	}
//	ucSwitchStat = Get_CurrentSwitchStat(6);
//	if(ucSwitchStat == CON_SWITCH_STAT_CLOSE)
//		 ucTmp2 |= 0x40;
//	ucSwitchStat = Get_CurrentDoorStat();
//	if(ucSwitchStat == CON_SWITCH_STAT_CLOSE)
//		 ucTmp2 |= 0x80;
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=ucTmp1;
//	pstAlrRecord->stAlrDataItem[0].usDataLen = 1;
//	pstAlrRecord->stAlrDataItem[1].ucDataContent[0]=ucTmp2;
//	pstAlrRecord->stAlrDataItem[1].usDataLen = 1;
//
//	return ucRet;
//}

////电流电压不平衡
//unsigned char Alarm_DataConvert_UnBalanceOverUp(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucTmp1=0,ucTmp2=0;
//	unsigned long ulVal=0,ulValA=0,ulValB=0,ulValC=0;
//	STMPRATEVAL		stMPRateVal;
//	STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
//	
//	stMPRateVal	= RunPara_GetMPRateVal(pstAlrEnsureJudge->ucMPNo);
//	pstAlrRecord->ucAlrDataItemNum=6;
//	if(stTerAdjPara.ucMPNoFromZero==1)
//		ucTmp1=pstAlrRecord->ucMPNo+1;				
//	else
//		ucTmp1=pstAlrRecord->ucMPNo;
//	switch(pstAlrEnsureJudge->usAlrCode)
//	{
//	case CONALRCODE_UUNBLANCE		:		//电压不平衡
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_UUnbalance==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;
//		ucTmp2=0x01;
//		break;
//	case CONALRCODE_IUNBLANCE		:		//电流不平衡
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_IUnbalance==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;
//		ucTmp2=0x02;
//		break;
//	default:
//		ucTmp2=0x01;
//	}
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=ucTmp1&0x7f;
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[1]=ucTmp1&0x80;
//	pstAlrRecord->stAlrDataItem[0].usDataLen=2;
//	pstAlrRecord->stAlrDataItem[1].ucDataContent[0]=ucTmp2;
//	pstAlrRecord->stAlrDataItem[1].usDataLen=1;
//
//	memcpy(&ulValA,pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,2);
//	memcpy(&ulValB,pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,2);
//	memcpy(&ulValC,pstAlrEnsureJudge->stAlrDataItem[2].ucDataContent,2);
//	ulVal=GetUnBalanceVal_byVal(ulValA,ulValB,ulValC,stMPRateVal.ucMPLineType);
//	if(ulVal == 0xFFFFFFFF)
//		ulVal = CONINVALIDVAL;
//
//	pstAlrRecord->stAlrDataItem[2].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_2B(pstAlrRecord->stAlrDataItem[2].ucDataContent,0,ulVal);
//
//	pstAlrRecord->stAlrDataItem[3].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_2B_DT07(pstAlrRecord->stAlrDataItem[3].ucDataContent,0,ulValA);
//	pstAlrRecord->stAlrDataItem[3].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_2B_DT07(pstAlrRecord->stAlrDataItem[3].ucDataContent,2,ulValB);
//	pstAlrRecord->stAlrDataItem[3].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_2B_DT07(pstAlrRecord->stAlrDataItem[3].ucDataContent,4,ulValC);
//
//	memcpy(&ulValA,pstAlrEnsureJudge->stAlrDataItem[3].ucDataContent,2);
//	memcpy(&ulValB,pstAlrEnsureJudge->stAlrDataItem[4].ucDataContent,2);
//	memcpy(&ulValC,pstAlrEnsureJudge->stAlrDataItem[5].ucDataContent,2);
//	ulVal=GetUnBalanceVal_byVal(ulValA,ulValB,ulValC,stMPRateVal.ucMPLineType);
//	if(ulVal == 0xFFFFFFFF)
//		ulVal = CONINVALIDVAL;
//
//	pstAlrRecord->stAlrDataItem[2].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_2B(pstAlrRecord->stAlrDataItem[2].ucDataContent,2,ulVal);
//
//	ulValA *= 10;
//	ulValB *= 10;
//	ulValC *= 10;
//	pstAlrRecord->stAlrDataItem[4].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_3B(pstAlrRecord->stAlrDataItem[4].ucDataContent,0,ulValA);
//	pstAlrRecord->stAlrDataItem[4].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_3B(pstAlrRecord->stAlrDataItem[4].ucDataContent,3,ulValB);
//	pstAlrRecord->stAlrDataItem[4].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_3B(pstAlrRecord->stAlrDataItem[4].ucDataContent,6,ulValC);
//
//	return ucRet;
//}

//表参数更改
//unsigned char Alarm_DataConvert_MeterParaModify(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucTmp=0;
//	STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
//	pstAlrRecord->ucAlrDataItemNum=2;
//	if(stTerAdjPara.ucMPNoFromZero==1)
//		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=pstAlrEnsureJudge->ucMPNo+1;
//	else
//		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=pstAlrEnsureJudge->ucMPNo;
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[1] = 0;
//	pstAlrRecord->stAlrDataItem[0].usDataLen=2;
//	switch(pstAlrEnsureJudge->usAlrCode)
//	{
//	case CONALRCODE_FEERATE		:		//费率更改
//		ucTmp=0x01;
//		break;
//	case CONALRCODE_PROGRAMTIME:		//编程时间更改
//		ucTmp=0x02;
//		break;
//	case CONALRCODE_COPYDAYTIME:		//抄表日
//		ucTmp=0x04;
//		break;
//	case CONALRCODE_PULSECONST:			//脉冲常数
//		ucTmp=0x08;
//		break;
//	case CONALRCODE_DEMANDTIMECLR:		//需量清零时间更改
//		ucTmp=0x20;
//		break;
//	default:
//		ucTmp=0x00;
//	}
//	pstAlrRecord->stAlrDataItem[1].ucDataContent[0]=ucTmp;
//	pstAlrRecord->stAlrDataItem[1].usDataLen=1;
//	return ucRet;
//}

////电流回路异常
//unsigned char Alarm_DataConvert_CurrentErr(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucTmp1=0,ucTmp2=0;
//	unsigned long ulVal=0;
//	STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
//	pstAlrRecord->ucAlrDataItemNum=9;
//	if(stTerAdjPara.ucMPNoFromZero==1)
//		ucTmp1=pstAlrRecord->ucMPNo+1;
//	else
//		ucTmp1=pstAlrRecord->ucMPNo;
//	switch(pstAlrEnsureJudge->usAlrCode)
//	{
//	case CONALRCODE_POLREVERSEA	:			//反极性
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_PolReverseA==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;						//0x80	越限=1，恢复=0
//		ucTmp2 = 0xC1;	//11000001
//		break;
//	case CONALRCODE_POLREVERSEB	:	
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_PolReverseB==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;						//0x80	越限=1，恢复=0
//		ucTmp2 = 0xC2;	//11000010
//		break;
//	case CONALRCODE_POLREVERSEC	:
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_PolReverseC==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;						//0x80	越限=1，恢复=0
//		ucTmp2 = 0xC4;	//11000100
//		break;
////	case CONALRCODE_SHORTCIRA1	:			//CT一次侧短路
////		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_ShortCirA1==CONALR_STAT_HAPPEN)
////			ucTmp1 |= 0x80;						//0x80	越限=1，恢复=0
////		ucTmp2 = 0x41;	//01000001
////		break;
////	case CONALRCODE_SHORTCIRA2	:			//CT二次侧短路
////		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_ShortCirA2==CONALR_STAT_HAPPEN)
////			ucTmp1 |= 0x80;						//0x80	越限=1，恢复=0
////		ucTmp2 = 0x41;	//01000001
////		break;
////	case CONALRCODE_SHORTCIRB1	:
////		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_ShortCirB1==CONALR_STAT_HAPPEN)
////			ucTmp1 |= 0x80;						//0x80	越限=1，恢复=0
////		ucTmp2 = 0x42;	//01000010
////		break;
////	case CONALRCODE_SHORTCIRB2	:		
////		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_ShortCirB2==CONALR_STAT_HAPPEN)
////			ucTmp1 |= 0x80;						//0x80	越限=1，恢复=0
////		ucTmp2 = 0x42;	//01000010
////		break;
////	case CONALRCODE_SHORTCIRC1	:
////		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_ShortCirC1==CONALR_STAT_HAPPEN)
////			ucTmp1 |= 0x80;						//0x80	越限=1，恢复=0
////		ucTmp2 = 0x44;	//01000100
////		break;
////	case CONALRCODE_SHORTCIRC2	:
////		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_ShortCirC2==CONALR_STAT_HAPPEN)
////			ucTmp1 |= 0x80;						//0x80	越限=1，恢复=0
////		ucTmp2 = 0x44;	//01000100
////		break;
////	case CONALRCODE_OPENCIRA2	:			//CT二次侧开路
////		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_OpenCirA2==CONALR_STAT_HAPPEN)
////			ucTmp1 |= 0x80;						//0x80	越限=1，恢复=0
////		ucTmp2 = 0x81;	//10000001
////		break;
////	case CONALRCODE_OPENCIRB2	:
////		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_OpenCirB2==CONALR_STAT_HAPPEN)
////			ucTmp1 |= 0x80;						//0x80	越限=1，恢复=0
////		ucTmp2 = 0x82;	//10000010
////		break;
////	case CONALRCODE_OPENCIRC2	:
////		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_OpenCirC2==CONALR_STAT_HAPPEN)
////			ucTmp1 |= 0x80;						//0x80	越限=1，恢复=0
////		ucTmp2 = 0x84;	//10000100
////		break;
//	}
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=ucTmp1&0x7f;
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[1]=ucTmp1&0x80;
//	pstAlrRecord->stAlrDataItem[0].usDataLen=2;
//	pstAlrRecord->stAlrDataItem[1].ucDataContent[0]=ucTmp2;
//	pstAlrRecord->stAlrDataItem[1].usDataLen=1;
//	
//	memcpy((unsigned char *)&ulVal,pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,2);
//	pstAlrRecord->stAlrDataItem[2].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_2B_DT07(pstAlrRecord->stAlrDataItem[2].ucDataContent,0,ulVal);
//	memcpy((unsigned char *)&ulVal,pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,2);
//	pstAlrRecord->stAlrDataItem[3].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_2B_DT07(pstAlrRecord->stAlrDataItem[3].ucDataContent,0,ulVal);
//	memcpy((unsigned char *)&ulVal,pstAlrEnsureJudge->stAlrDataItem[2].ucDataContent,2);
//	pstAlrRecord->stAlrDataItem[4].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_2B_DT07(pstAlrRecord->stAlrDataItem[4].ucDataContent,0,ulVal);
//
//	memcpy((unsigned char *)&ulVal,pstAlrEnsureJudge->stAlrDataItem[3].ucDataContent,2);
//	ulVal *= 10;
//	pstAlrRecord->stAlrDataItem[5].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_3B(pstAlrRecord->stAlrDataItem[5].ucDataContent,0,ulVal);
//	memcpy((unsigned char *)&ulVal,pstAlrEnsureJudge->stAlrDataItem[4].ucDataContent,2);
//	ulVal *= 10;
//	pstAlrRecord->stAlrDataItem[6].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_3B(pstAlrRecord->stAlrDataItem[6].ucDataContent,0,ulVal);
//	memcpy((unsigned char *)&ulVal,pstAlrEnsureJudge->stAlrDataItem[5].ucDataContent,2);
//	ulVal *= 10;
//	pstAlrRecord->stAlrDataItem[7].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_3B(pstAlrRecord->stAlrDataItem[7].ucDataContent,0,ulVal);
//
//	memcpy((unsigned char *)&ulVal,pstAlrEnsureJudge->stAlrDataItem[6].ucDataContent,4);
//	pstAlrRecord->stAlrDataItem[8].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_5B_DT14(pstAlrRecord->stAlrDataItem[8].ucDataContent,0,ulVal);
//	return ucRet;
//}

//表计故障信息
unsigned char Alarm_DataConvert_MeterErrInfo(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucTmp=0,ucStat=0;
	STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
	pstAlrRecord->ucAlrDataItemNum=2;
	if(stTerAdjPara.ucMPNoFromZero==1)
		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=pstAlrEnsureJudge->ucMPNo+1;
	else
		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=pstAlrEnsureJudge->ucMPNo;
	pstAlrRecord->stAlrDataItem[0].ucDataContent[1] = 0;
	pstAlrRecord->stAlrDataItem[0].usDataLen=2;
	switch(pstAlrEnsureJudge->usAlrCode)
	{
	case CONALRCODE_PROGRAMNUM:			//编程次数变化
		ucTmp=0x01;
		break;
	case CONALRCODE_DEMANDTIMENUM:		//需量清零次数变化
		ucTmp=0x01;
		break;
	case CONALRCODE_LOSTVOLNUM:			//断相次数变化
		ucTmp=0x02;
		break;
	case CONALRCODE_METERBATTLOW:		//电池电压低
		ucStat=pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent[0];
		if((ucStat & 0x04)==0x04)					//电池欠压
			ucTmp=0x10;
		else
			ucTmp=0x00;
		break;
	default:
		ucTmp=0x00;
	}
	pstAlrRecord->stAlrDataItem[1].ucDataContent[0]=ucTmp;
	pstAlrRecord->stAlrDataItem[1].usDataLen=1;
	return ucRet;
}

//表计时钟超差
unsigned char Alarm_DataConvert_MeterTimeErr(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucTmp1=0;
	STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
	pstAlrRecord->ucAlrDataItemNum=1;
	if(stTerAdjPara.ucMPNoFromZero==1)
		ucTmp1=pstAlrEnsureJudge->ucMPNo+1;
	else
		ucTmp1=pstAlrEnsureJudge->ucMPNo;
	if(stAlarmStatus[pstAlrEnsureJudge->ucMPNo].ucAlrStat_Sys_MeterTimeErr==CONALR_STAT_HAPPEN)
		ucTmp1 |= 0x80;
	pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=ucTmp1&0X7F;
	pstAlrRecord->stAlrDataItem[0].ucDataContent[1]=ucTmp1&0X80;
	pstAlrRecord->stAlrDataItem[0].usDataLen=1;
	return ucRet;
}
//停电上电
unsigned char Alarm_DataConvert_PowerOn(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
{
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME	stTerChangeTime,stCurrTime;
	stCurrTime=GetCurrentTime();
	stTerChangeTime=Get_TerPowerChangeTime();
	Set_TerPowerChangeTime(&stCurrTime);
	pstAlrRecord->ucAlrDataItemNum=2;
	if(pstAlrEnsureJudge->usAlrCode==CONALRCODE_POWERON)
	{
		if(Is_DateTime_Availble(&stTerChangeTime)==RET_SUCCESS)
		{
			pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=(unsigned char)HEX_TO_BCD(stTerChangeTime.ucMinute,1);
			pstAlrRecord->stAlrDataItem[0].ucDataContent[1]=(unsigned char)HEX_TO_BCD(stTerChangeTime.ucHour,1);
			pstAlrRecord->stAlrDataItem[0].ucDataContent[2]=(unsigned char)HEX_TO_BCD(stTerChangeTime.ucDay,1);
			pstAlrRecord->stAlrDataItem[0].ucDataContent[3]=(unsigned char)HEX_TO_BCD(stTerChangeTime.ucMonth,1);
			pstAlrRecord->stAlrDataItem[0].ucDataContent[4]=(unsigned char)HEX_TO_BCD(stTerChangeTime.ucYear,1);
		}else
		{
			pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=0xEE;
			pstAlrRecord->stAlrDataItem[0].ucDataContent[1]=0xEE;
			pstAlrRecord->stAlrDataItem[0].ucDataContent[2]=0xEE;
			pstAlrRecord->stAlrDataItem[0].ucDataContent[3]=0xEE;
			pstAlrRecord->stAlrDataItem[0].ucDataContent[4]=0xEE;
		}
		pstAlrRecord->stAlrDataItem[1].ucDataContent[0]=(unsigned char)HEX_TO_BCD(pstAlrEnsureJudge->stDateTime.ucMinute,1);
		pstAlrRecord->stAlrDataItem[1].ucDataContent[1]=(unsigned char)HEX_TO_BCD(pstAlrEnsureJudge->stDateTime.ucHour,1);
		pstAlrRecord->stAlrDataItem[1].ucDataContent[2]=(unsigned char)HEX_TO_BCD(pstAlrEnsureJudge->stDateTime.ucDay,1);
		pstAlrRecord->stAlrDataItem[1].ucDataContent[3]=(unsigned char)HEX_TO_BCD(pstAlrEnsureJudge->stDateTime.ucMonth,1);
		pstAlrRecord->stAlrDataItem[1].ucDataContent[4]=(unsigned char)HEX_TO_BCD(pstAlrEnsureJudge->stDateTime.ucYear,1);
	}else
	{
		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=(unsigned char)HEX_TO_BCD(pstAlrEnsureJudge->stDateTime.ucMinute,1);
		pstAlrRecord->stAlrDataItem[0].ucDataContent[1]=(unsigned char)HEX_TO_BCD(pstAlrEnsureJudge->stDateTime.ucHour,1);
		pstAlrRecord->stAlrDataItem[0].ucDataContent[2]=(unsigned char)HEX_TO_BCD(pstAlrEnsureJudge->stDateTime.ucDay,1);
		pstAlrRecord->stAlrDataItem[0].ucDataContent[3]=(unsigned char)HEX_TO_BCD(pstAlrEnsureJudge->stDateTime.ucMonth,1);
		pstAlrRecord->stAlrDataItem[0].ucDataContent[4]=(unsigned char)HEX_TO_BCD(pstAlrEnsureJudge->stDateTime.ucYear,1);
/*
		if(Is_DateTime_Availble(&stTerChangeTime)==RET_SUCCESS)
		{
			pstAlrRecord->stAlrDataItem[1].ucDataContent[0]=(unsigned char)HEX_TO_BCD(stTerChangeTime.ucMinute,1);
			pstAlrRecord->stAlrDataItem[1].ucDataContent[1]=(unsigned char)HEX_TO_BCD(stTerChangeTime.ucHour,1);
			pstAlrRecord->stAlrDataItem[1].ucDataContent[2]=(unsigned char)HEX_TO_BCD(stTerChangeTime.ucDay,1);
			pstAlrRecord->stAlrDataItem[1].ucDataContent[3]=(unsigned char)HEX_TO_BCD(stTerChangeTime.ucMonth,1);
			pstAlrRecord->stAlrDataItem[1].ucDataContent[4]=(unsigned char)HEX_TO_BCD(stTerChangeTime.ucYear,1);
		}else
		{
			pstAlrRecord->stAlrDataItem[1].ucDataContent[0]=0xEE;
			pstAlrRecord->stAlrDataItem[1].ucDataContent[1]=0xEE;
			pstAlrRecord->stAlrDataItem[1].ucDataContent[2]=0xEE;
			pstAlrRecord->stAlrDataItem[1].ucDataContent[3]=0xEE;
			pstAlrRecord->stAlrDataItem[1].ucDataContent[4]=0xEE;
		}
*/
		//停电告警里面，上电时间固定为无效
		pstAlrRecord->stAlrDataItem[1].ucDataContent[0]=0xEE;
		pstAlrRecord->stAlrDataItem[1].ucDataContent[1]=0xEE;
		pstAlrRecord->stAlrDataItem[1].ucDataContent[2]=0xEE;
		pstAlrRecord->stAlrDataItem[1].ucDataContent[3]=0xEE;
		pstAlrRecord->stAlrDataItem[1].ucDataContent[4]=0xEE;
	}
	pstAlrRecord->stAlrDataItem[0].usDataLen=5;
	pstAlrRecord->stAlrDataItem[1].usDataLen=5;
	return ucRet;
}

//终端故障信息
unsigned char Alarm_DataConvert_TerErr(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucTmp=0;
	pstAlrRecord->ucAlrDataItemNum=1;
	switch(pstAlrEnsureJudge->usAlrCode)
	{
	case CONALRCODE_TERERR_RAM:			//终端主板内存故障
		ucTmp=0x01;
		break;
	case CONALRCODE_TERERR_CLOCK:		//时钟故障
		ucTmp=0x02;
		break;
	case CONALRCODE_TERERR_MAINCOMM:	//主板通信故障
//		if(stAlarmStatus[pstAlrRecord->ucMPNo].ucAlrStat_Sys_MainComm==CONALR_STAT_HAPPEN)
			ucTmp=0x03;
//		else
//			ucTmp=0x83;
		break;
	case CONALRCODE_TERERR_485ERROR:	//485抄表故障
		ucTmp=0x04;
		break;
	case CONALRCODE_TERERR_DISPLAY:		//显示板故障
		ucTmp=0x05;
		break;
	default:
		ucTmp=0x00;
	}
	pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=ucTmp;
	pstAlrRecord->stAlrDataItem[0].usDataLen=1;
	return ucRet;
}

//密码错误
unsigned char Alarm_DataConvert_PasswordErr(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
{
	unsigned char ucRet=RET_SUCCESS;
	pstAlrRecord->ucAlrDataItemNum=2;
	pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent[0];
	pstAlrRecord->stAlrDataItem[0].ucDataContent[1]=pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent[1];
	memcpy(pstAlrRecord->stAlrDataItem[0].ucDataContent,pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,16);
	pstAlrRecord->stAlrDataItem[0].usDataLen=16;
	pstAlrRecord->stAlrDataItem[1].ucDataContent[0]=pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent[0];
	pstAlrRecord->stAlrDataItem[1].usDataLen=1;
	return ucRet;
}

//unsigned char Alarm_DataConvert_RemoteCtrl(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
//{//遥控
//	unsigned char ucRet=RET_SUCCESS;
//
//	double	dVal;
//	unsigned short usDT2;
//	pstAlrRecord->ucAlrDataItemNum=3;
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=(1<<pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent[0]);
//	pstAlrRecord->stAlrDataItem[0].usDataLen=1;
//	memcpy(&dVal,pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,8);
//	usDT2=Float_To_DT2((float)dVal);
//	pstAlrRecord->stAlrDataItem[1].ucDataContent[0]=(unsigned char)usDT2;
//	pstAlrRecord->stAlrDataItem[1].ucDataContent[1]=(unsigned char)(usDT2>>8);
//	pstAlrRecord->stAlrDataItem[1].usDataLen=2;
//	memcpy(&dVal,pstAlrEnsureJudge->stAlrDataItem[2].ucDataContent,8);
//	usDT2=Float_To_DT2((float)dVal);
//	pstAlrRecord->stAlrDataItem[2].ucDataContent[0]=(unsigned char)usDT2;
//	pstAlrRecord->stAlrDataItem[2].ucDataContent[1]=(unsigned char)(usDT2>>8);
//	pstAlrRecord->stAlrDataItem[2].usDataLen=2;
//
//	return ucRet;
//}
//
//unsigned char Alarm_DataConvert_PowerCtrl(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
//{//功控
//	unsigned char ucRet=RET_SUCCESS;
//
//	double	dVal;
//	unsigned short usDT2;
//	unsigned char ucTmp;
//	STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
//	unsigned char ucSumGNo;
//	ucSumGNo=Load_GetPowerCtrl_SumGNo();
//	if(ucSumGNo>=CON_SUMGROUP_NUM)
//	{
//		ucSumGNo=0;
//		Load_SetPowerCtrl_SumGNo(ucSumGNo);
//	}
//	pstAlrRecord->ucAlrDataItemNum=5;
//	if(stTerAdjPara.ucSumGFromZero==1)
//		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=ucSumGNo+1;				//补上总加组号为1
//	else
//		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=ucSumGNo;
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[1]=(1<<pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent[0]);
//	pstAlrRecord->stAlrDataItem[0].usDataLen=2;
//	ucTmp=pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent[0];
//	if(ucTmp==CONSWITCHCURVE_POWLOWER)
//		pstAlrRecord->stAlrDataItem[1].ucDataContent[0] = 0x08;
//	if(ucTmp==CONSWITCHCURVE_SALESTOP)
//		pstAlrRecord->stAlrDataItem[1].ucDataContent[0] = 0x04;
//	if(ucTmp==CONSWITCHCURVE_RESTCTRL)
//		pstAlrRecord->stAlrDataItem[1].ucDataContent[0] = 0x02;
//	if(ucTmp==CONSWITCHCURVE_TIMEPER1 || ucTmp==CONSWITCHCURVE_TIMEPER2 || ucTmp==CONSWITCHCURVE_TIMEPER3)
//		pstAlrRecord->stAlrDataItem[1].ucDataContent[0] = 0x01;
//	pstAlrRecord->stAlrDataItem[1].usDataLen=1;
//
//	memcpy(&dVal,pstAlrEnsureJudge->stAlrDataItem[2].ucDataContent,8);
//	usDT2=Float_To_DT2((float)dVal);
//	pstAlrRecord->stAlrDataItem[2].ucDataContent[0]=(unsigned char)usDT2;
//	pstAlrRecord->stAlrDataItem[2].ucDataContent[1]=(unsigned char)(usDT2>>8);
//	pstAlrRecord->stAlrDataItem[2].usDataLen=2;
//
//	memcpy(&dVal,pstAlrEnsureJudge->stAlrDataItem[3].ucDataContent,8);
//	usDT2=Float_To_DT2((float)dVal);
//	pstAlrRecord->stAlrDataItem[3].ucDataContent[0]=(unsigned char)usDT2;
//	pstAlrRecord->stAlrDataItem[3].ucDataContent[1]=(unsigned char)(usDT2>>8);
//	pstAlrRecord->stAlrDataItem[3].usDataLen=2;
//
//	memcpy(&dVal,pstAlrEnsureJudge->stAlrDataItem[4].ucDataContent,8);
//	usDT2=Float_To_DT2((float)dVal);
//	pstAlrRecord->stAlrDataItem[4].ucDataContent[0]=(unsigned char)usDT2;
//	pstAlrRecord->stAlrDataItem[4].ucDataContent[1]=(unsigned char)(usDT2>>8);
//	pstAlrRecord->stAlrDataItem[4].usDataLen=2;
//
//	return ucRet;
//}
//
//unsigned char Alarm_DataConvert_EnergyCtrl(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
//{//电控
//	unsigned char ucRet=RET_SUCCESS;
//
//	double	dVal;
//	unsigned long ulDT3;
//	unsigned char ucTmp;
//	STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
//	unsigned char ucSumGNo;
//	ucSumGNo=Load_GetEnergyCtrl_SumGNo();
//	if(ucSumGNo>=CON_SUMGROUP_NUM)
//	{
//		ucSumGNo=0;
//		Load_SetEnergyCtrl_SumGNo(ucSumGNo);
//	}
//	pstAlrRecord->ucAlrDataItemNum=4;
//	if(stTerAdjPara.ucSumGFromZero==1)
//		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=ucSumGNo+1;				//补上总加组号为1
//	else
//		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=ucSumGNo;
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[1]=(1<<pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent[0]);
//	pstAlrRecord->stAlrDataItem[0].usDataLen=2;
//	ucTmp=pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent[0];
//	if(ucTmp==CON_ENERGYCTRL_MON)
//		pstAlrRecord->stAlrDataItem[1].ucDataContent[0] = 0x01;
//	else
//		pstAlrRecord->stAlrDataItem[1].ucDataContent[0] = 0x02;
//	pstAlrRecord->stAlrDataItem[1].usDataLen=1;
//	memcpy(&dVal,pstAlrEnsureJudge->stAlrDataItem[2].ucDataContent,8);
//	ulDT3=Float_To_DT3(dVal,0);
//	memcpy(pstAlrRecord->stAlrDataItem[2].ucDataContent,&(ulDT3),4);
//	pstAlrRecord->stAlrDataItem[2].usDataLen=4;
//	memcpy(&dVal,pstAlrEnsureJudge->stAlrDataItem[3].ucDataContent,8);
//	ulDT3=Float_To_DT3(dVal,0);
//	memcpy(pstAlrRecord->stAlrDataItem[3].ucDataContent,&(ulDT3),4);
//	pstAlrRecord->stAlrDataItem[3].usDataLen=4;
//
//	return ucRet;
//}
//
//unsigned char Alarm_DataConvert_BuyPowEnerRec(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
//{//购电参数设置
//	unsigned char ucRet=RET_SUCCESS;
//
//	double	dVal;
//	unsigned long ulDT3;
//	STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
//	unsigned char ucSumGNo;
//	ucSumGNo=Load_GetEnergyCtrl_SumGNo();
//	if(ucSumGNo>=CON_SUMGROUP_NUM)
//	{
//		ucSumGNo=0;
//		Load_SetEnergyCtrl_SumGNo(ucSumGNo);
//	}
//	pstAlrRecord->ucAlrDataItemNum=8;
//	if(stTerAdjPara.ucSumGFromZero==1)
//		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=ucSumGNo+1;				//补上总加组号为1
//	else
//		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=ucSumGNo;
//	pstAlrRecord->stAlrDataItem[0].usDataLen=1;
//	memcpy(pstAlrRecord->stAlrDataItem[1].ucDataContent,pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,4);
//	pstAlrRecord->stAlrDataItem[1].usDataLen=4;
//	pstAlrRecord->stAlrDataItem[2].ucDataContent[0]=pstAlrEnsureJudge->stAlrDataItem[2].ucDataContent[0];
//	pstAlrRecord->stAlrDataItem[2].usDataLen=1;
//	
//	memcpy(pstAlrRecord->stAlrDataItem[3].ucDataContent,pstAlrEnsureJudge->stAlrDataItem[3].ucDataContent,4);
//	pstAlrRecord->stAlrDataItem[3].usDataLen=4;
//	memcpy(pstAlrRecord->stAlrDataItem[4].ucDataContent,pstAlrEnsureJudge->stAlrDataItem[4].ucDataContent,4);
//	pstAlrRecord->stAlrDataItem[4].usDataLen=4;
//	memcpy(pstAlrRecord->stAlrDataItem[5].ucDataContent,pstAlrEnsureJudge->stAlrDataItem[5].ucDataContent,4);
//	pstAlrRecord->stAlrDataItem[5].usDataLen=4;
//	memcpy(&dVal,pstAlrEnsureJudge->stAlrDataItem[6].ucDataContent,8);
//	ulDT3=Float_To_DT3(dVal,0);
//	memcpy(pstAlrRecord->stAlrDataItem[6].ucDataContent,&(ulDT3),4);
//	pstAlrRecord->stAlrDataItem[6].usDataLen=4;
//	memcpy(&dVal,pstAlrEnsureJudge->stAlrDataItem[7].ucDataContent,8);
//	ulDT3=Float_To_DT3(dVal,0);
//	memcpy(pstAlrRecord->stAlrDataItem[7].ucDataContent,&(ulDT3),4);
//	pstAlrRecord->stAlrDataItem[7].usDataLen=4;
//
//	return ucRet;
//}
//
//unsigned char Alarm_DataConvert_EnerAlr(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
//{//ERC23 电控告警事件
//	unsigned char ucRet=RET_SUCCESS;
//
//	double	dVal;
//	unsigned long ulDT3;
//	STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
//	unsigned char ucSumGNo;
//	ucSumGNo=Load_GetEnergyCtrl_SumGNo();
//	if(ucSumGNo>=CON_SUMGROUP_NUM)
//	{
//		ucSumGNo=0;
//		Load_SetEnergyCtrl_SumGNo(ucSumGNo);
//	}
//	pstAlrRecord->ucAlrDataItemNum=3;
//	if(stTerAdjPara.ucSumGFromZero==1)
//		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=ucSumGNo+1;				//补上总加组号为1
//	else
//		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=ucSumGNo;
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[1]=(1<<pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent[0]);
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[2]=pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent[0];
//	pstAlrRecord->stAlrDataItem[0].usDataLen=3;
//
//	memcpy(&dVal,pstAlrEnsureJudge->stAlrDataItem[2].ucDataContent,8);
//	ulDT3=Float_To_DT3(dVal,0);
//	memcpy(pstAlrRecord->stAlrDataItem[1].ucDataContent,&(ulDT3),4);
//	pstAlrRecord->stAlrDataItem[1].usDataLen=4;
//
//	memcpy(&dVal,pstAlrEnsureJudge->stAlrDataItem[3].ucDataContent,8);
//	ulDT3=Float_To_DT3(dVal,0);
//	memcpy(pstAlrRecord->stAlrDataItem[2].ucDataContent,&(ulDT3),4);
//	pstAlrRecord->stAlrDataItem[2].usDataLen=4;
//
//	return ucRet;
//}
unsigned char Alarm_DataConvert_MeterCopy(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
{//ERC31 485抄表失败事件记录
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucTmp=0;
	pstAlrRecord->ucAlrDataItemNum=4;
	if(pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent[0]>=CON_COPYERR_TIMES)
		ucTmp=0x80;
	if(RunPara_GetTerAdj_MPNoFromZero()==1)
		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=pstAlrRecord->ucMPNo+1;
	else
		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=pstAlrRecord->ucMPNo;
	pstAlrRecord->stAlrDataItem[0].ucDataContent[1] = ucTmp;
	pstAlrRecord->stAlrDataItem[0].usDataLen=2;
	memcpy(pstAlrRecord->stAlrDataItem[1].ucDataContent,pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,5);	//最近一次成功时间
	pstAlrRecord->stAlrDataItem[1].usDataLen=5;
	memcpy(pstAlrRecord->stAlrDataItem[2].ucDataContent,pstAlrEnsureJudge->stAlrDataItem[2].ucDataContent,5);	//成功时有功电量示值
	pstAlrRecord->stAlrDataItem[2].usDataLen=5;
	memcpy(pstAlrRecord->stAlrDataItem[3].ucDataContent,pstAlrEnsureJudge->stAlrDataItem[3].ucDataContent,4);	//成功时无功电量示值
	pstAlrRecord->stAlrDataItem[3].usDataLen=4;
	return ucRet;
}


unsigned char Alarm_DataConvert_MeterState(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
{//ERC33 电表状态字变更
	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucTmp=0;
	pstAlrRecord->ucAlrDataItemNum=3;
	if(RunPara_GetTerAdj_MPNoFromZero()==1)
		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=pstAlrRecord->ucMPNo+1;
	else
		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=pstAlrRecord->ucMPNo;
	pstAlrRecord->stAlrDataItem[0].ucDataContent[1] = 0;
	pstAlrRecord->stAlrDataItem[0].usDataLen=2;
	memcpy(pstAlrRecord->stAlrDataItem[1].ucDataContent,pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,14);	//变位标志
	pstAlrRecord->stAlrDataItem[1].usDataLen=14;
	memcpy(pstAlrRecord->stAlrDataItem[2].ucDataContent,pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,14);	//最新的状态字
	pstAlrRecord->stAlrDataItem[2].usDataLen=14;
	return ucRet;
}


unsigned char Alarm_DataConvert_FluxLimit(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
{//ERC32 月通信超流量记录
	unsigned char ucRet=RET_SUCCESS;
	pstAlrRecord->ucAlrDataItemNum=2;
	memcpy(pstAlrRecord->stAlrDataItem[0].ucDataContent,pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,4);
	pstAlrRecord->stAlrDataItem[0].usDataLen=4;
	memcpy(pstAlrRecord->stAlrDataItem[1].ucDataContent,pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,4);
	pstAlrRecord->stAlrDataItem[1].usDataLen=4;
	return ucRet;
}

////表计飞走
//unsigned char Alarm_DataConvert_MeterFly(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned long ulPowLast,ulPowNow;
//	STTERMISCPARA stTerMiscPara;
//	STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
//	stTerMiscPara=RunPara_GetTerMiscPara();
//	pstAlrRecord->ucAlrDataItemNum=4;
//	if(stTerAdjPara.ucMPNoFromZero==1)
//		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=pstAlrRecord->ucMPNo+1;
//	else
//		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=pstAlrRecord->ucMPNo;
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[1]=0;
//	pstAlrRecord->stAlrDataItem[0].usDataLen=2;
//	memcpy((unsigned char *)&ulPowLast,pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,pstAlrEnsureJudge->stAlrDataItem[0].usDataLen);
//	pstAlrRecord->stAlrDataItem[1].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_5B_DT14(pstAlrRecord->stAlrDataItem[1].ucDataContent,0,ulPowLast);
//	memcpy((unsigned char *)&ulPowNow,pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,pstAlrEnsureJudge->stAlrDataItem[1].usDataLen);
//	pstAlrRecord->stAlrDataItem[2].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_5B_DT14(pstAlrRecord->stAlrDataItem[2].ucDataContent,0,ulPowNow);
//	
//	pstAlrRecord->stAlrDataItem[3].ucDataContent[0]=(unsigned char)HEX_TO_BCD(stTerMiscPara.ucMeterFly,1);
//	pstAlrRecord->stAlrDataItem[3].usDataLen=1;
//	return ucRet;
//}
//
////表计停走
//unsigned char Alarm_DataConvert_MeterStop(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned long ulPowNow;
//	STTERMISCPARA stTerMiscPara;
//	STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
//	stTerMiscPara=RunPara_GetTerMiscPara();
//	pstAlrRecord->ucAlrDataItemNum=3;
//	if(stTerAdjPara.ucMPNoFromZero==1)
//		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=pstAlrRecord->ucMPNo+1;
//	else
//		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=pstAlrRecord->ucMPNo;
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[1]=0;
//	pstAlrRecord->stAlrDataItem[0].usDataLen=2;
//	memcpy((unsigned char *)&ulPowNow,pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,pstAlrEnsureJudge->stAlrDataItem[0].usDataLen);
//	pstAlrRecord->stAlrDataItem[1].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_5B_DT14(pstAlrRecord->stAlrDataItem[1].ucDataContent,0,ulPowNow);
//	pstAlrRecord->stAlrDataItem[2].ucDataContent[0]=stTerMiscPara.ucMeterStop;
//	pstAlrRecord->stAlrDataItem[2].usDataLen=1;
//	return ucRet;
//}
//
////表计倒走
//unsigned char Alarm_DataConvert_MeterDown(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned long ulPowLast,ulPowNow;
//	STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
//	pstAlrRecord->ucAlrDataItemNum=3;
//	if(stTerAdjPara.ucMPNoFromZero==1)
//		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=pstAlrRecord->ucMPNo+1;
//	else
//		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=pstAlrRecord->ucMPNo;
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[1]=0;
//	pstAlrRecord->stAlrDataItem[0].usDataLen=2;
//	memcpy((unsigned char *)&ulPowLast,pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,pstAlrEnsureJudge->stAlrDataItem[0].usDataLen);
//	pstAlrRecord->stAlrDataItem[1].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_5B_DT14(pstAlrRecord->stAlrDataItem[1].ucDataContent,0,ulPowLast);
//	memcpy((unsigned char *)&ulPowNow,pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,pstAlrEnsureJudge->stAlrDataItem[1].usDataLen);
//	pstAlrRecord->stAlrDataItem[2].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_5B_DT14(pstAlrRecord->stAlrDataItem[2].ucDataContent,0,ulPowNow);
//	return ucRet;
//}
//unsigned char Alarm_DataConvert_SumCompOverLimit(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucSumCompGroupNo=0;
//	unsigned char ucTmp1=0;
//	unsigned char i=0,j=0;
//	unsigned long ulVal=0,ulDT3=0;
//	float		  fVal=0;
//	double		  dVal=0;
//
//	pstAlrRecord->ucAlrDataItemNum=pstAlrEnsureJudge->ucAlrDataItemNum+1;
//
//	ucSumCompGroupNo=pstAlrEnsureJudge->usAlrCode-CONALRCODE_SUMCOMPOVERLIMIT;
//	ucTmp1=RunPara_GetTerMisc_PowDiffLimit_ucSumGNo(ucSumCompGroupNo);
//	ucTmp1=ucTmp1+1;				//ucSumCompGroupNo是序号，而不是总加组号
//	if(stSumAlarmStatus[ucSumCompGroupNo].ucAlrStat_Sys_CompOverLimit==CONALR_STAT_HAPPEN)
//		ucTmp1 |= 0x80;	
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=ucTmp1;
//	pstAlrRecord->stAlrDataItem[0].usDataLen=1;
//
//	if(pstAlrEnsureJudge->ucAlrDataItemNum>CONALRRECITEMNUM-1)
//		pstAlrEnsureJudge->ucAlrDataItemNum=CONALRRECITEMNUM-1;		//防止告警记录携带的数据项越界
//
//	for(i=0;i<pstAlrEnsureJudge->ucAlrDataItemNum;i++)
//	{
//		if(i==0 || i==1 )
//		{
//			memcpy(&fVal,pstAlrEnsureJudge->stAlrDataItem[i].ucDataContent,4);			
////			ulDT3=Float_To_DT3(fVal,0);
//			ulDT3=Float_To_DT3_Div100(fVal);
//			memcpy(pstAlrRecord->stAlrDataItem[i+1].ucDataContent,&(ulDT3),4);
//			pstAlrRecord->stAlrDataItem[i+1].usDataLen=4;
//		}
//		else if(i==3)
//		{
//			memcpy(&dVal,pstAlrEnsureJudge->stAlrDataItem[i].ucDataContent,8);
////			ulDT3=Float_To_DT3(dVal,0);
//			ulDT3=Float_To_DT3_Div100((float)dVal);
//			memcpy(pstAlrRecord->stAlrDataItem[i+1].ucDataContent,&(ulDT3),4);
//			pstAlrRecord->stAlrDataItem[i+1].usDataLen=4;
//		}
//		else if(i==2)
//		{
//			pstAlrRecord->stAlrDataItem[i+1].ucDataContent[0]=pstAlrEnsureJudge->stAlrDataItem[i].ucDataContent[0];
//			pstAlrRecord->stAlrDataItem[i+1].usDataLen=1;
//		}
//		else
//		{
//			memcpy(pstAlrRecord->stAlrDataItem[i+1].ucDataContent,pstAlrEnsureJudge->stAlrDataItem[i].ucDataContent,pstAlrEnsureJudge->stAlrDataItem[i].usDataLen);
//			pstAlrRecord->stAlrDataItem[i+1].usDataLen = pstAlrEnsureJudge->stAlrDataItem[i].usDataLen;
//		}
//	}
//	return ucRet;
//}

//总加瞬时有功
//unsigned char Alarm_DataConvert_SumInstant(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//
//	unsigned char ucGroupNo=0,ucTmp1;
//	unsigned short usDT2=0,usLimit=0;
//	unsigned long ulVal;
//	STSUMGROUPPARA	*pstSumGParaTmp;
//	STHISSUMVALGROUP  stHisVal;
//	STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
//	pstAlrRecord->ucAlrDataItemNum=3;
//	ucGroupNo=Alarm_Get_SumGroupNo_byAlrCode(pstAlrEnsureJudge->usAlrCode);
//	if(stTerAdjPara.ucSumGFromZero==1)
//		ucTmp1=ucGroupNo+1;
//	else
//		ucTmp1=ucGroupNo;
//	pstSumGParaTmp = RunPara_GetSumGroupParaPtr(ucGroupNo);
//	switch(pstAlrEnsureJudge->usAlrCode)
//	{
//	case CONALRCODE_SUMINSTANTPOWUP1	:		//瞬时有功总加组1越上限
//	case CONALRCODE_SUMINSTANTPOWUP2	:		//瞬时有功总加组2越上限
//	case CONALRCODE_SUMINSTANTPOWUP3	:		//瞬时有功总加组3越上限
//	case CONALRCODE_SUMINSTANTPOWUP4	:		//瞬时有功总加组4越上限
//	case CONALRCODE_SUMINSTANTPOWUP5	:		//瞬时有功总加组5越上限
//	case CONALRCODE_SUMINSTANTPOWUP6	:		//瞬时有功总加组6越上限
//	case CONALRCODE_SUMINSTANTPOWUP7	:		//瞬时有功总加组7越上限
//	case CONALRCODE_SUMINSTANTPOWUP8	:		//瞬时有功总加组8越上限
//		if(stSumAlarmStatus[ucGroupNo].ucAlrStat_Sys_SumInstantPowUp==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;
//		stHisVal = Get_History_InstantPowSumVal(0);
//		if(stHisVal.stSumValGroup.fSumVal[ucGroupNo]!=CONINVALIDVAL)
//		{
//			ulVal=(unsigned long)stHisVal.stSumValGroup.fSumVal[ucGroupNo];
//			usDT2=Hex_To_DT2(ulVal,4,0);
//		}
//		usLimit=Hex_To_DT2(pstSumGParaTmp->stInstantPow.stLimit.ulSumGUpLimit,4,0);
//		break;
//	case CONALRCODE_SUMINSTANTVARUP1	:		//瞬时无功总加组1越上限
//	case CONALRCODE_SUMINSTANTVARUP2	:		//瞬时无功总加组2越上限
//	case CONALRCODE_SUMINSTANTVARUP3	:		//瞬时无功总加组3越上限
//	case CONALRCODE_SUMINSTANTVARUP4	:		//瞬时无功总加组4越上限
//	case CONALRCODE_SUMINSTANTVARUP5	:		//瞬时无功总加组5越上限
//	case CONALRCODE_SUMINSTANTVARUP6	:		//瞬时无功总加组6越上限
//	case CONALRCODE_SUMINSTANTVARUP7	:		//瞬时无功总加组7越上限
//	case CONALRCODE_SUMINSTANTVARUP8	:		//瞬时无功总加组8越上限
//		ucTmp1 |= 0x40;
//		if(stSumAlarmStatus[ucGroupNo].ucAlrStat_Sys_SumInstantVarUp==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;
//		stHisVal = Get_History_InstantVarSumVal(0);
//		if(stHisVal.stSumValGroup.fSumVal[ucGroupNo]!=CONINVALIDVAL)
//		{
//			ulVal=(unsigned long)stHisVal.stSumValGroup.fSumVal[ucGroupNo];
//			usDT2=Hex_To_DT2(ulVal,2,0);
//		}
//		usLimit=Hex_To_DT2(pstSumGParaTmp->stInstantVar.stLimit.ulSumGUpLimit,2,0);
//		break;
//	default:
//		break;
//	}
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=ucTmp1;
//	pstAlrRecord->stAlrDataItem[0].usDataLen=1;
//	pstAlrRecord->stAlrDataItem[1].ucDataContent[0]=(unsigned char)usDT2;
//	pstAlrRecord->stAlrDataItem[1].ucDataContent[1]=(unsigned char)(usDT2>>8);
//	pstAlrRecord->stAlrDataItem[1].usDataLen=2;
//	pstAlrRecord->stAlrDataItem[2].ucDataContent[0]=(unsigned char)usLimit;
//	pstAlrRecord->stAlrDataItem[2].ucDataContent[1]=(unsigned char)(usLimit>>8);
//	pstAlrRecord->stAlrDataItem[2].usDataLen=2;
//
//	return ucRet;
//}

//总加电量
//unsigned char Alarm_DataConvert_SumPow(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//
//	unsigned char ucGroupNo=0,ucTmp1;
//	unsigned long ulVal=0,ulLimit=0;
//	STSUMGROUPPARA	*pstSumGParaTmp;
//	STHISSUMVALGROUP  stHisVal;
//	STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
//	pstAlrRecord->ucAlrDataItemNum=3;
//	ucGroupNo=Alarm_Get_SumGroupNo_byAlrCode(pstAlrEnsureJudge->usAlrCode);
//	if(stTerAdjPara.ucSumGFromZero==1)
//		ucTmp1=ucGroupNo+1;
//	else
//		ucTmp1=ucGroupNo;
//	pstSumGParaTmp = RunPara_GetSumGroupParaPtr(ucGroupNo);
//	switch(pstAlrEnsureJudge->usAlrCode)
//	{
//	case CONALRCODE_SUMPOWUP1	:		//有功总加组1越上限
//	case CONALRCODE_SUMPOWUP2	:		//有功总加组2越上限
//	case CONALRCODE_SUMPOWUP3	:		//有功总加组3越上限
//	case CONALRCODE_SUMPOWUP4	:		//有功总加组4越上限
//	case CONALRCODE_SUMPOWUP5	:		//有功总加组5越上限
//	case CONALRCODE_SUMPOWUP6	:		//有功总加组6越上限
//	case CONALRCODE_SUMPOWUP7	:		//有功总加组7越上限
//	case CONALRCODE_SUMPOWUP8	:		//有功总加组8越上限
//		if(stSumAlarmStatus[ucGroupNo].ucAlrStat_Sys_SumPowValUp==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;
//		stHisVal = Get_History_InstantPowSumVal(0);
//		ulVal=(unsigned long)stHisVal.stSumValGroup.fSumVal[ucGroupNo];
//		pstAlrRecord->stAlrDataItem[1].usDataLen
//			=(unsigned char)SetMainComm_Buf_lVal_4B(pstAlrRecord->stAlrDataItem[1].ucDataContent,0,ulVal);
//		pstAlrRecord->stAlrDataItem[2].usDataLen
//			=(unsigned char)SetMainComm_Buf_lVal_4B(pstAlrRecord->stAlrDataItem[2].ucDataContent,0,pstSumGParaTmp->stInstantPow.stLimit.ulSumGUpLimit);
//		break;
//	case CONALRCODE_SUMVARUP1	:		//无功总加组1越上限
//	case CONALRCODE_SUMVARUP2	:		//无功总加组2越上限
//	case CONALRCODE_SUMVARUP3	:		//无功总加组3越上限
//	case CONALRCODE_SUMVARUP4	:		//无功总加组4越上限
//	case CONALRCODE_SUMVARUP5	:		//无功总加组5越上限
//	case CONALRCODE_SUMVARUP6	:		//无功总加组6越上限
//	case CONALRCODE_SUMVARUP7	:		//无功总加组7越上限
//	case CONALRCODE_SUMVARUP8	:		//无功总加组8越上限
//		ucTmp1 |= 0x40;
//		if(stSumAlarmStatus[ucGroupNo].ucAlrStat_Sys_SumVarValUp==CONALR_STAT_HAPPEN)
//			ucTmp1 |= 0x80;
//		stHisVal = Get_History_InstantVarSumVal(0);
//		ulVal=(unsigned long)stHisVal.stSumValGroup.fSumVal[ucGroupNo];
//		pstAlrRecord->stAlrDataItem[1].usDataLen
//			=(unsigned char)SetMainComm_Buf_lVal_4B(pstAlrRecord->stAlrDataItem[1].ucDataContent,0,ulVal);
//		pstAlrRecord->stAlrDataItem[2].usDataLen
//			=(unsigned char)SetMainComm_Buf_lVal_4B(pstAlrRecord->stAlrDataItem[2].ucDataContent,0,pstSumGParaTmp->stInstantVar.stLimit.ulSumGUpLimit);
//		break;
//	default:
//		pstAlrRecord->stAlrDataItem[1].usDataLen
//			=(unsigned char)SetMainComm_Buf_lVal_4B(pstAlrRecord->stAlrDataItem[1].ucDataContent,0,0);
//		pstAlrRecord->stAlrDataItem[2].usDataLen
//			=(unsigned char)SetMainComm_Buf_lVal_4B(pstAlrRecord->stAlrDataItem[2].ucDataContent,0,pstSumGParaTmp->stInstantPow.stLimit.ulSumGUpLimit);
//		break;
//	}
//	pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=ucTmp1;
//	pstAlrRecord->stAlrDataItem[0].usDataLen=1;
//
//	return ucRet;
//}
//
////计算总加组号
//unsigned char Alarm_Get_SumGroupNo_byAlrCode(unsigned short usAlrCode)
//{
//	unsigned char ucGroupNo=0;
//	ucGroupNo=(usAlrCode-1) % 8;
//	return ucGroupNo;
//}
//unsigned char Alarm_DataConvert_PowDiffOver(STALRRECORD *pstAlrRecord,STALRENSUREJUDGE *pstAlrEnsureJudge)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned long ulPowLast,ulPowNow;
//	STTERMISCPARA stTerMiscPara;
//	STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
//	stTerMiscPara=RunPara_GetTerMiscPara();
//	pstAlrRecord->ucAlrDataItemNum=4;
//	if(stTerAdjPara.ucMPNoFromZero==1)
//		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=pstAlrRecord->ucMPNo+1;
//	else
//		pstAlrRecord->stAlrDataItem[0].ucDataContent[0]=pstAlrRecord->ucMPNo;
//	pstAlrRecord->stAlrDataItem[0].usDataLen=1;
//	memcpy((unsigned char *)&ulPowLast,pstAlrEnsureJudge->stAlrDataItem[0].ucDataContent,pstAlrEnsureJudge->stAlrDataItem[0].usDataLen);
//	pstAlrRecord->stAlrDataItem[1].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_5B_DT14(pstAlrRecord->stAlrDataItem[1].ucDataContent,0,ulPowLast);
//	memcpy((unsigned char *)&ulPowNow,pstAlrEnsureJudge->stAlrDataItem[1].ucDataContent,pstAlrEnsureJudge->stAlrDataItem[1].usDataLen);
//	pstAlrRecord->stAlrDataItem[2].usDataLen
//		=(unsigned char)SetMainComm_Buf_lVal_5B_DT14(pstAlrRecord->stAlrDataItem[2].ucDataContent,0,ulPowNow);
//	
//	pstAlrRecord->stAlrDataItem[3].ucDataContent[0]=(unsigned char)HEX_TO_BCD(stTerMiscPara.ucMeterOverDiff,1);
//	pstAlrRecord->stAlrDataItem[3].usDataLen=1;
//	return ucRet;
//}

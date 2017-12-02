/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Remote_comm.c
 Description    : 远方通讯控制模块
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2011-09-23      azh           the original version
******************************************************************************/
#include "..\inc\global_config.h"


//extern unsigned short	usLoginSEQCount[CON_LOGINSEQ_NUM];				//登录帧seq

static unsigned char Main_RComm_ResetManage(unsigned char ucLoginFlag);
static unsigned char Main_RComm_ConnetManage(void);
static void Main_RComm_OverFlux_Logout(void);
static void HeartBeat_SetCount(unsigned short usCount);

//---------------------------------------------------------------------------------------
//返回在客户端方式下的三种通信方法，永久在线，短信激活，时段在线
unsigned char   RunPara_GetTerCommPara_RCommType(void)
{
    return (stTerRunPara.stTerCommPara_Inner.ucRCommMode & CON_ONLINE_TYPE_MASK);
}

void HeartBeat_SetCount(unsigned short usCount)
{
	usHeartBeatCount=usCount;
}

unsigned short HeartBeat_GetCount(void)
{
	return usHeartBeatCount;
}

void CommConnect_SetNeedLoginFlag(unsigned char ucFlag)
{
	ucNeedLoginFlag=ucFlag;
}

//unsigned char CommConnect_GetNeedLoginFlag(void)
//{
//	return ucNeedLoginFlag;
//}

void CommConnect_SetBeatHeartNoAckCount(unsigned char ucCount)
{
	ucBeatHeartNoAckCount=ucCount;
}

//unsigned char CommConnect_GetBeatHeartNoAckCount(void)
//{
//	return ucBeatHeartNoAckCount;
//}
unsigned char Main_Get_Comm_Type(void)
{
	unsigned char ucRet=CON_TERMSG_SOUR_RCOMM_SMS;
	//STTERMISCPARA stTerMiscPara=RunPara_GetTerMiscPara();

	if(	(stTerRunPara.stTerMiscPara.ucIPInfoType[0]==CON_TC35COMMTYPE_INVALID) &&
		(stTerRunPara.stTerMiscPara.ucIPInfoType[1]==CON_TC35COMMTYPE_INVALID) )
		ucRet=CON_TERMSG_SOUR_RCOMM_SMS;
	else if( (stTerRunPara.stTerMiscPara.ucIPInfoType[0]==CON_TC35COMMTYPE_UDP) &&
			 (stTerRunPara.stTerMiscPara.ucIPInfoType[1]==CON_TC35COMMTYPE_TCP) )
		ucRet=CON_TERMSG_SOUR_RCOMM_UDP;
	else if( (stTerRunPara.stTerMiscPara.ucIPInfoType[0]==CON_TC35COMMTYPE_TCP) &&
			 (stTerRunPara.stTerMiscPara.ucIPInfoType[1]==CON_TC35COMMTYPE_UDP) )
		ucRet=CON_TERMSG_SOUR_RCOMM_TCP;
	return ucRet;
}

unsigned char TerSeq_GetCount(void)
{
	return ucTerSEQCount;
}
void HeartBeat_Manage(void)
{//心跳管理
//	unsigned char i=0;
//	STHEARTBEAT stHeartBeat;
//	unsigned char ucBuf[10];
	unsigned short usBeatCount=HeartBeat_GetCount();
//	STTERCOMMPARA_INNER	stTerCommPara_In=RunPara_GetTerCommPara_Inner();
	
//	STTERMISCPARA	stTerMiscPara=RunPara_GetTerMiscPara();
	unsigned char ucSeq=0;
	unsigned char ucComType=Main_Get_Comm_Type();
	unsigned char ucGPRSType = RunPara_GetTerCommPara_RCommType();
//	if(stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat==CON_STATUS_NO)//无连接打开自动保电
//		RunPara_SetGprsKeepPow(CON_AVALIBLE_FLAG);

//	if(ucGPRSType == CON_ONLINE_CLIENT)	//客户端方式
	if(1)
	{
		if( (stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLastLoginStat==CON_STATUS_NO) &&
			(stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat==CON_STATUS_YES) )//刚刚登陆上
		{
//			memset(ucBuf,0x34,10);
//			RComm_FillUpData(ucBuf,10,CON_TERMSG_SOUR_RCOMM_TCP);
			usBeatCount=12;										//准备5秒后立即登录
			CommConnect_SetNeedLoginFlag(CON_STATUS_YES);		//该标志由接收到主站确认后设置为CON_STATUS_NO,登陆帧使能
			
		//	LCD_Set_HintSeqno(CON_LCDHINT_CONNECTED);
		//	RunPara_SetGprsKeepPow(CON_AVALIBLE_FLAG);
		}
		else if (stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat==CON_STATUS_YES)
		{//一直处于连接状态
		//	RunPara_SetGprsKeepPow(CON_INVALIBLE_FLAG);
			//if(LCD_Get_HintSeqno()==CON_LCDHINT_STARTLOG)
		//	i=LCD_Get_HintSeqno();
		//	if((i==CON_LCDHINT_STARTLOG) || (i==CON_LCDHINT_INIT))
		//		LCD_Set_HintSeqno(CON_LCDHINT_CONNECTED);
			if(usBeatCount==0)
			{
				//azh
				//stTerRunPara.stHeartBeat=RunPara_GetHeartBeat();
				usBeatCount=stTerRunPara.stHeartBeat.ucHeartBeat*120-4;	//单位1分钟转换为单位0.5秒
//				if(stTerRunPara.stTerCommPara_Inner.ucRCommMode!=CON_ONLINE_INVALID && ucUpActiveSelfFlag == CON_ONLINE_INVALID) //永久在线才上送心跳
				if(stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat==CON_STATUS_YES)
					Main_SendComm_CommConnect(3);			//心跳
			}
			else if(usBeatCount==8)
			{
				Main_SendComm_CommConnect(1);			//登陆
				usBeatCount--;
			}
			else if(usBeatCount>12)
			{
				usBeatCount--;
				if(((usBeatCount % 10)==0)&&(RunPara_GetTerCommPara_RCommMode()!=CON_ONLINE_INVALID))
				{//间隔5秒发送,发送主动上报的内容
//azh 2013-01-23
					ucSeq=Main_Get_TmpUpSendTask();
					if(ucSeq<CONRMSENDTASKNUM)
					{//有主动上报的内容

						Main_FillUpData(stRMSendTaskList.stRMSendBuffer[ucSeq].Send_Buffer,
							stRMSendTaskList.stRMSendBuffer[ucSeq].Send_Len,
							ucComType,			//固定tcp主动上送
							stRMSendTaskList.stRMSendBuffer[ucSeq].ucLink);		

					}
//
				}
			}else
				usBeatCount--;
		}
		HeartBeat_SetCount(usBeatCount);
		
		
		
		Main_RComm_ResetManage(stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat);
		Main_RComm_ConnetManage();
		Main_RComm_OverFlux_Logout();			//通信流量越限退出管理
	}
	else if(ucGPRSType == CON_ONLINE_MIX)	//混合模式
	{
		if( (stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLastLoginStat==CON_STATUS_NO) &&
			(stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat==CON_STATUS_YES) )//刚刚登陆上
		{
			if(ucLoginFlag == CON_STATUS_YES)
			{
				CommConnect_SetNeedLoginFlag(CON_STATUS_YES);		//该标志由接收到主站确认后设置为CON_STATUS_NO,登陆帧使能
				Main_SendComm_CommConnect(1);			//发登陆帧
				ucLoginFlag = CON_STATUS_NO;
				ucLogoffConter = 240;		//120S后断开连接
			}
		}
		else if (stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat==CON_STATUS_YES)
		{

//			ucSeq=Main_Get_TmpUpSendTask();
			if(ucSeq<CONRMSENDTASKNUM)	//有主动上报的内容，需要继续连着
			{//有主动上报的内容
/*
				Main_FillUpData(stRMSendTaskList.stRMSendBuffer[ucSeq].Send_Buffer,
					stRMSendTaskList.stRMSendBuffer[ucSeq].Send_Len,
					CON_TERMSG_SOUR_RCOMM_UDP,			//固定tcp主动上送主动上送用UDP通道2
					stRMSendTaskList.stRMSendBuffer[ucSeq].ucLink);		
*/
				ucLogoffConter = 240;		//120S后断开连接
			}
			else	//没有需要上送的队列了
			{
				if(ucLogoffConter > 20)
					ucLogoffConter = 20;		//10S后断开连接
			}
		}
		if (stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat==CON_STATUS_NO)//在调线的情况下
		{
//			ucSeq=Main_Get_TmpUpSendTask();
			if(ucSeq<CONRMSENDTASKNUM)	//有主动上报的内容，需要继续连着
			{//有主动上报的内容
			//	Main_RComm_UpStart();	//马上去连接
/*
				Main_FillUpData(stRMSendTaskList.stRMSendBuffer[ucSeq].Send_Buffer,
					stRMSendTaskList.stRMSendBuffer[ucSeq].Send_Len,
					CON_TERMSG_SOUR_RCOMM_UDP,			//固定tcp主动上送主动上送用UDP通道2
					stRMSendTaskList.stRMSendBuffer[ucSeq].ucLink);		
*/
				ucLogoffConter = 240;		//120S后断开连接
			}
		}
		if(ucLogoffConter > 0 && ucLogoffConter != 0xff)
		{
			ucLogoffConter --;
			ucClientNeed = 0;
		}
		if(ucLogoffConter == 0)	//连续时间到，断开
		{
			ucClientNeed = 1;
			ucLogoffConter = 0xff;
//			Main_RComm_UpStop();	//断开连续
		}
		usServerTimeoutCount ++;
		if(usServerTimeoutCount > (unsigned short)(RunPara_GetHeartBeat().ucHeartBeat)*120*10)
		{
			usServerTimeoutCount = 0;
			ucReset_Mc55_Flag = 1;	//复位模块
			ucLoginFlag = CON_STATUS_YES;
			Main_RComm_UpStart();	//重新连接
		}
		if(ucClientNeed == 0)//在有TCP CLIENT连接的情况要做连接判断
			Main_RComm_ContinueOnLine();
		else	//不需要连接
		{
			usDisConnectCount=0xFFFF;
			usReConnectCount=0xFFFF;
		}
	}

	if(stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLastLoginStat!=stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat)
	{
		stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLastLoginStat=stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat;
		//azh
//		RunPara_SetTerMiscPara(&stTerRunPara.stTerMiscPara);
	//	TerPara_Set(CON_STATUS_YES,(1<<CON_TERPARA_BLOCKNUM_MISCPARA),0);
	}
}


//模块复位管理
//每天11，23时，检查，不在线则复位模块一次
unsigned char Main_RComm_ResetManage(unsigned char ucLoginFlag)
{	
	unsigned char  ucRet=RET_SUCCESS;
	STDATETIME stCurrtime;
	if(ucLoginFlag==CON_STATUS_NO)
	{
		stCurrtime=GetCurrentTime();
		if((stCurrtime.ucHour % 12)==11 && stCurrtime.ucHour != ucRCommResetHour)
		{
			ucRCommResetHour=stCurrtime.ucHour;
			ucReset_Mc55_Flag = 1;
		}
	}
	
	return ucRet;
}
//连接管理
unsigned char Main_RComm_ConnetManage(void)
{//通讯在线控制管理
	unsigned char  ucRet=RET_SUCCESS;
	if(RunPara_GetTerCommPara_RCommMode()==CON_ONLINE_ALLWAYS		//永久在线方式
		&& ucOverFluxLogoutCount>0)									//且不是流量越限保护动作
	{
		ucUpActiveSelfFlag		= CON_ONLINE_INVALID;
		Main_RComm_ContinueOnLine();
	}
	else if(RunPara_GetTerCommPara_RCommMode()==CON_ONLINE_SMS_UP
		|| ucOverFluxLogoutCount==0)				//激活或者保护过程
	{
		Main_RComm_UnContinueOnLine();	
	}
	else//否则不登陆
	{
		Main_RComm_UpStop();
	}
	return ucRet;
}
//永久在线并且没有流量超限的情况下管理连接，发现断线后马上去重连
void Main_RComm_ContinueOnLine(void)
{
	//STTERMISCPARA stTerMiscPara=RunPara_GetTerMiscPara();
	if(stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat!=CON_STATUS_YES)
	{
		if(usDisConnectCount!=0xFFFF)
			usDisConnectCount++;
		else if(usReConnectCount!=0xFFFF)
			usReConnectCount++;
		else if(usReConnectCount==0xFFFF)
			usDisConnectCount=0;							//在线变掉线，则开始计数
	}else
	{
		usDisConnectCount=0xFFFF;
		usReConnectCount=0xFFFF;
	}
	if((usDisConnectCount>=CON_DISCONNECT_COUNT) && (usDisConnectCount!=0xFFFF))
	{//确认掉线,连了几次没有连上
		if(ucIPFrist == 0)
			ucIPFrist = 1;
		else
			ucIPFrist = 0;
		Main_RComm_UpStop();
		usReConnectCount=0;				//准备重连接，开始计数
		usDisConnectCount=0xFFFF;
	}
	if((usReConnectCount>=RunPara_GetReConnectTime()) && (usReConnectCount!=0xFFFF))
	{//立即重连接
		Main_RComm_UpStart();
		usReConnectCount=0xFFFF;		//立即重连接
		usDisConnectCount=0;			//掉线计数器开始计数
	}
}
//被动激活下看有没有超过连接次数，超过了就断开不再继续连
unsigned char Main_RComm_StopLogin(void)
{
	if(ucUpOnErrCount>=RunPara_GetTerCommPara_ReConnectTimes())
	{
		ucUpActiveSelfFlag=CON_ONLINE_INVALID;
		Main_RComm_UpStop();
		return RET_ERROR;
	}
	else
	{
		if(usUpOnConnectCount!=0xFFFF)
			usUpOnConnectCount++;
		else
			usUpOnConnectCount=0;							//在线变掉线，则开始计数
		if((usUpOnConnectCount>=CON_DISCONNECT_COUNT) && (usUpOnConnectCount!=0xFFFF))
		{//确认掉线
			ucUpOnErrCount++;
			Main_RComm_UpStart();
			usUpOnConnectCount=0xFFFF;
		}	
	}
	return RET_SUCCESS;
	
}
unsigned char Main_RComm_SetStandBy_Time(void)
{	
	unsigned char ucRet=RET_SUCCESS;
	if(ucUpActiveSelfFlag==CON_ONLINE_SMS_ACTIVE)
		usStopOnConnectCount=RunPara_GetTerCommPara_StandbyTime();		//自己主动上线则延迟50秒自动掉线		

	return ucRet;
}
//在短信激活或者流量超下情况下的连接管理
void Main_RComm_UnContinueOnLine(void)
{//天津模块表，通讯在线控制管理
//	STTERMISCPARA stTerMiscPara=RunPara_GetTerMiscPara();
//	unsigned char    ucFlag=CON_STATUS_YES;
	
	if(stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat!=CON_STATUS_YES)//不在线
	{
		usDisConnectCount=0xFFFF;
		usReConnectCount=RunPara_GetReConnectTime()-1;//下次立即建立连接
		if(ucUpActiveSelfFlag!=CON_ONLINE_INVALID)
			Main_RComm_StopLogin();					
	}
	else	//在线
	{
		ucUpOnErrCount = 0;
		usDisConnectCount = 0xFFFF;
		usReConnectCount = 0xFFFF;
		usUpOnConnectCount = 0xFFFF;
		
		if(ucUpActiveSelfFlag!=CON_ONLINE_INVALID)
		{
			if(usStopOnConnectCount==0xFFFF)
			{
				if(ucUpActiveSelfFlag==CON_ONLINE_SMS_ACTIVE)
					usStopOnConnectCount=RunPara_GetTerCommPara_StandbyTime();		//自己主动上线则延迟50秒自动掉线
				else
					usStopOnConnectCount =100;
			}
			else
				usStopOnConnectCount--;//在线计数
			if((usStopOnConnectCount==0) && (usStopOnConnectCount!=0xFFFF))
				ucUpActiveSelfFlag=CON_ONLINE_INVALID;
		}
		
		
		//azh if(ucUpActiveSelfFlag==CON_ONLINE_INVALID&&DP_Get_TerDP_State()!=CON_TERDP_STATE_DOWNLOAD)//不在不在升级的状态
		if(ucUpActiveSelfFlag==CON_ONLINE_INVALID)//不在不在升级的状态
		{
			//计时大于自动掉线时间，则掉线终止
			if(stTerRunPara.stTerMiscPara.ucIPInfoType[0]!=CON_TC35COMMTYPE_INVALID||
				stTerRunPara.stTerMiscPara.ucIPInfoType[1]!=CON_TC35COMMTYPE_INVALID)
				Main_RComm_UpStop();
			ucUpActiveSelfFlag=CON_ONLINE_INVALID;
			usStopOnConnectCount=0xFFFF;
			
		}
	}	
	
	
}


//void Main_Connect_DelayUpCount(unsigned short usCount)
//{
//	//usDelayUpCount=usCount;
//}

void Main_RComm_Init_LoginSleepCount(void)
{
	//STTERMISCPARA	stTerMiscPara=RunPara_GetTerMiscPara();
	usDisConnectCount		= 0xFFFF;
	usReConnectCount		= 0xFFFF;
	usUpOnConnectCount		= 0xFFFF;
	usServerTimeoutCount	= 0;
	ucUpActiveSelfFlag		= CON_ONLINE_INVALID;
	ucUpOnErrCount = 0;
	ucRCommResetCount = 0xFF;
	ucRCommResetHour = 0xFF;
	LoginFlag = CON_STATUS_NO;		//睡眠计数器
	ucIPFrist = 0;			//初始化后默认用主IP
	if(RunPara_GetTerMisc_ucRcommBaseType()==CON_TC35COMMTYPE_TCP)
	{
		stTerRunPara.stTerMiscPara.ucIPInfoType[0]=CON_TC35COMMTYPE_TCP;
		stTerRunPara.stTerMiscPara.ucIPInfoType[1]=CON_TC35COMMTYPE_UDP;
	}
	else if(RunPara_GetTerMisc_ucRcommBaseType()==CON_TC35COMMTYPE_UDP)
	{
		stTerRunPara.stTerMiscPara.ucIPInfoType[0]=CON_TC35COMMTYPE_UDP;
		stTerRunPara.stTerMiscPara.ucIPInfoType[1]=CON_TC35COMMTYPE_TCP;
	}
	else
	{
	//	stTerRunPara.stTerMiscPara.ucIPInfoType[0]=CON_TC35COMMTYPE_INVALID;
	//	stTerRunPara.stTerMiscPara.ucIPInfoType[1]=CON_TC35COMMTYPE_INVALID;
		stTerRunPara.stTerMiscPara.ucIPInfoType[0]=CON_TC35COMMTYPE_TCP;
		stTerRunPara.stTerMiscPara.ucIPInfoType[1]=CON_TC35COMMTYPE_UDP;
	}
//azh
//	RunPara_SetTerMiscPara(&stTerRunPara.stTerMiscPara);

//	TerPara_Set(CON_STATUS_YES,(1<<CON_TERPARA_BLOCKNUM_MISCPARA),0);
}




void Main_RComm_UpStop(void)
{//激发模式睡眠
	//STTERMISCPARA stTerMiscPara=RunPara_GetTerMiscPara();
	STMODPARACHINFO stModParaChInfoTmp;
	Get_ModChInfo(&stModParaChInfoTmp);
	stModParaChInfoTmp.bIsCHInfoChanged = 1;
	Set_ModChInfo(&stModParaChInfoTmp);
	stTerRunPara.stTerMiscPara.ucIPInfoType[0]=CON_TC35COMMTYPE_INVALID;
	stTerRunPara.stTerMiscPara.ucIPInfoType[1]=CON_TC35COMMTYPE_INVALID;
//azh
//	RunPara_SetTerMiscPara(&stTerRunPara.stTerMiscPara);
	Reload_Chinfo_Para();
//	TerPara_Set(CON_STATUS_YES,(1<<CON_TERPARA_BLOCKNUM_MISCPARA),0);
}
void Main_Connect_UpActiveSelfFlag(unsigned char ucFlag)
{
	ucUpOnErrCount = 0;
	ucUpActiveSelfFlag=ucFlag;
}
//由睡眠开始主动登陆并上送，永久状态不需要短信唤醒
void MainComm_StartUp(unsigned char ucFlag)
{
	
//	STTERCOMMPARA_INNER	stTerCommPara_In=RunPara_GetTerCommPara_Inner();
//	STTERMISCPARA	stTerMiscPara;
	
//	stTerMiscPara=RunPara_GetTerMiscPara();
	
	//if(stTerRunPara.stTerCommPara_Inner.ucRCommMode==CON_ONLINE_SMS_UP) //可以短信唤醒
//	{		
		Main_RComm_UpStart();
		Main_Connect_UpActiveSelfFlag(ucFlag);			//主动上送后50秒就掉线
		//			usUpOnConnectCount=CON_DISCONNECT_COUNT;
//	}
}

unsigned char Main_RComm_UpStart(void)
{//激发模式启动
	unsigned char  ucRet=RET_SUCCESS;
	//STTERMISCPARA stTerMiscPara=RunPara_GetTerMiscPara();
	if(RunPara_GetTerMisc_ucRcommBaseType()==CON_TC35COMMTYPE_TCP)
	{
		stTerRunPara.stTerMiscPara.ucIPInfoType[0]=CON_TC35COMMTYPE_TCP;
		stTerRunPara.stTerMiscPara.ucIPInfoType[1]=CON_TC35COMMTYPE_UDP;
	}
	else if (RunPara_GetTerMisc_ucRcommBaseType()==CON_TC35COMMTYPE_UDP)
	{
		stTerRunPara.stTerMiscPara.ucIPInfoType[0]=CON_TC35COMMTYPE_UDP;
		stTerRunPara.stTerMiscPara.ucIPInfoType[1]=CON_TC35COMMTYPE_TCP;
	}else
	{
//		stTerMiscPara.ucIPInfoType[0]=CON_TC35COMMTYPE_INVALID;
//		stTerMiscPara.ucIPInfoType[1]=CON_TC35COMMTYPE_INVALID;
		stTerRunPara.stTerMiscPara.ucIPInfoType[0]=CON_TC35COMMTYPE_TCP;
		stTerRunPara.stTerMiscPara.ucIPInfoType[1]=CON_TC35COMMTYPE_UDP;
	}
//azh	
//	RunPara_SetTerMiscPara(&stTerRunPara.stTerMiscPara);
	Reload_Chinfo_Para();
//	TerPara_Set(CON_STATUS_YES,(1<<CON_TERPARA_BLOCKNUM_MISCPARA),0);
	return ucRet;
}


unsigned char Main_Init_LoginCount(void)
{
	unsigned char  ucRet=RET_SUCCESS;
	usDisConnectCount=0xFFFF;
	usReConnectCount=RunPara_GetReConnectTime()*2;
	return ucRet;
}

//当接到电话后调用该函数用于登陆
unsigned char Main_RComm_RingStart(void)
{
	unsigned char  ucRet=RET_SUCCESS;
//	STTERCOMMPARA_INNER	stTerCommPara_In=RunPara_GetTerCommPara_Inner();
	//if(stTerCommPara_In.ucRCommMode==CON_ONLINE_SMS_UP)
//	{
		
		CommConnect_SetNeedLoginFlag(CON_STATUS_YES);
		MainComm_StartUp(CON_ONLINE_SMS_ACTIVE);
//	}
	return ucRet;
}
//通信流量超限后下线管理，发退出登陆帧。然后下线
void Main_RComm_OverFlux_Logout(void)
{//流量越限退出登陆
	if(ucOverFluxLogoutCount!=0xFF && ucOverFluxLogoutCount!=0)
		ucOverFluxLogoutCount--;
	if(ucOverFluxLogoutCount==40)
	{//最后20秒准备发送退出登陆帧
//		Main_SendComm_CommConnect(2);
	}
	else if(ucOverFluxLogoutCount==1)
	{//下线
		Main_RComm_UpStop();
	}
}
////设置通信越限退出登陆计数器
void Main_RComm_Set_OverFlux_Logout(unsigned char ucCount)
{
	ucOverFluxLogoutCount=ucCount;
}
////读取通信越限退出登陆计数器
//unsigned char Main_RComm_Get_OverFlux_Logout(void)
//{
//	return ucOverFluxLogoutCount;
//}
//读取当前是否流量超限
unsigned char Main_RComm_Get_OverFlux_AlrStatus(void)
{
	if(ucOverFluxLogoutCount<=100)
		return CON_STATUS_YES;
	else
		return CON_STATUS_NO;
}

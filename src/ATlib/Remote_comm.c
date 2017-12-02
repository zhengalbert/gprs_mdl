/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Remote_comm.c
 Description    : Զ��ͨѶ����ģ��
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2011-09-23      azh           the original version
******************************************************************************/
#include "..\inc\global_config.h"


//extern unsigned short	usLoginSEQCount[CON_LOGINSEQ_NUM];				//��¼֡seq

static unsigned char Main_RComm_ResetManage(unsigned char ucLoginFlag);
static unsigned char Main_RComm_ConnetManage(void);
static void Main_RComm_OverFlux_Logout(void);
static void HeartBeat_SetCount(unsigned short usCount);

//---------------------------------------------------------------------------------------
//�����ڿͻ��˷�ʽ�µ�����ͨ�ŷ������������ߣ����ż��ʱ������
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
{//��������
//	unsigned char i=0;
//	STHEARTBEAT stHeartBeat;
//	unsigned char ucBuf[10];
	unsigned short usBeatCount=HeartBeat_GetCount();
//	STTERCOMMPARA_INNER	stTerCommPara_In=RunPara_GetTerCommPara_Inner();
	
//	STTERMISCPARA	stTerMiscPara=RunPara_GetTerMiscPara();
	unsigned char ucSeq=0;
	unsigned char ucComType=Main_Get_Comm_Type();
	unsigned char ucGPRSType = RunPara_GetTerCommPara_RCommType();
//	if(stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat==CON_STATUS_NO)//�����Ӵ��Զ�����
//		RunPara_SetGprsKeepPow(CON_AVALIBLE_FLAG);

//	if(ucGPRSType == CON_ONLINE_CLIENT)	//�ͻ��˷�ʽ
	if(1)
	{
		if( (stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLastLoginStat==CON_STATUS_NO) &&
			(stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat==CON_STATUS_YES) )//�ոյ�½��
		{
//			memset(ucBuf,0x34,10);
//			RComm_FillUpData(ucBuf,10,CON_TERMSG_SOUR_RCOMM_TCP);
			usBeatCount=12;										//׼��5���������¼
			CommConnect_SetNeedLoginFlag(CON_STATUS_YES);		//�ñ�־�ɽ��յ���վȷ�Ϻ�����ΪCON_STATUS_NO,��½֡ʹ��
			
		//	LCD_Set_HintSeqno(CON_LCDHINT_CONNECTED);
		//	RunPara_SetGprsKeepPow(CON_AVALIBLE_FLAG);
		}
		else if (stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat==CON_STATUS_YES)
		{//һֱ��������״̬
		//	RunPara_SetGprsKeepPow(CON_INVALIBLE_FLAG);
			//if(LCD_Get_HintSeqno()==CON_LCDHINT_STARTLOG)
		//	i=LCD_Get_HintSeqno();
		//	if((i==CON_LCDHINT_STARTLOG) || (i==CON_LCDHINT_INIT))
		//		LCD_Set_HintSeqno(CON_LCDHINT_CONNECTED);
			if(usBeatCount==0)
			{
				//azh
				//stTerRunPara.stHeartBeat=RunPara_GetHeartBeat();
				usBeatCount=stTerRunPara.stHeartBeat.ucHeartBeat*120-4;	//��λ1����ת��Ϊ��λ0.5��
//				if(stTerRunPara.stTerCommPara_Inner.ucRCommMode!=CON_ONLINE_INVALID && ucUpActiveSelfFlag == CON_ONLINE_INVALID) //�������߲���������
				if(stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat==CON_STATUS_YES)
					Main_SendComm_CommConnect(3);			//����
			}
			else if(usBeatCount==8)
			{
				Main_SendComm_CommConnect(1);			//��½
				usBeatCount--;
			}
			else if(usBeatCount>12)
			{
				usBeatCount--;
				if(((usBeatCount % 10)==0)&&(RunPara_GetTerCommPara_RCommMode()!=CON_ONLINE_INVALID))
				{//���5�뷢��,���������ϱ�������
//azh 2013-01-23
					ucSeq=Main_Get_TmpUpSendTask();
					if(ucSeq<CONRMSENDTASKNUM)
					{//�������ϱ�������

						Main_FillUpData(stRMSendTaskList.stRMSendBuffer[ucSeq].Send_Buffer,
							stRMSendTaskList.stRMSendBuffer[ucSeq].Send_Len,
							ucComType,			//�̶�tcp��������
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
		Main_RComm_OverFlux_Logout();			//ͨ������Խ���˳�����
	}
	else if(ucGPRSType == CON_ONLINE_MIX)	//���ģʽ
	{
		if( (stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLastLoginStat==CON_STATUS_NO) &&
			(stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat==CON_STATUS_YES) )//�ոյ�½��
		{
			if(ucLoginFlag == CON_STATUS_YES)
			{
				CommConnect_SetNeedLoginFlag(CON_STATUS_YES);		//�ñ�־�ɽ��յ���վȷ�Ϻ�����ΪCON_STATUS_NO,��½֡ʹ��
				Main_SendComm_CommConnect(1);			//����½֡
				ucLoginFlag = CON_STATUS_NO;
				ucLogoffConter = 240;		//120S��Ͽ�����
			}
		}
		else if (stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat==CON_STATUS_YES)
		{

//			ucSeq=Main_Get_TmpUpSendTask();
			if(ucSeq<CONRMSENDTASKNUM)	//�������ϱ������ݣ���Ҫ��������
			{//�������ϱ�������
/*
				Main_FillUpData(stRMSendTaskList.stRMSendBuffer[ucSeq].Send_Buffer,
					stRMSendTaskList.stRMSendBuffer[ucSeq].Send_Len,
					CON_TERMSG_SOUR_RCOMM_UDP,			//�̶�tcp������������������UDPͨ��2
					stRMSendTaskList.stRMSendBuffer[ucSeq].ucLink);		
*/
				ucLogoffConter = 240;		//120S��Ͽ�����
			}
			else	//û����Ҫ���͵Ķ�����
			{
				if(ucLogoffConter > 20)
					ucLogoffConter = 20;		//10S��Ͽ�����
			}
		}
		if (stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat==CON_STATUS_NO)//�ڵ��ߵ������
		{
//			ucSeq=Main_Get_TmpUpSendTask();
			if(ucSeq<CONRMSENDTASKNUM)	//�������ϱ������ݣ���Ҫ��������
			{//�������ϱ�������
			//	Main_RComm_UpStart();	//����ȥ����
/*
				Main_FillUpData(stRMSendTaskList.stRMSendBuffer[ucSeq].Send_Buffer,
					stRMSendTaskList.stRMSendBuffer[ucSeq].Send_Len,
					CON_TERMSG_SOUR_RCOMM_UDP,			//�̶�tcp������������������UDPͨ��2
					stRMSendTaskList.stRMSendBuffer[ucSeq].ucLink);		
*/
				ucLogoffConter = 240;		//120S��Ͽ�����
			}
		}
		if(ucLogoffConter > 0 && ucLogoffConter != 0xff)
		{
			ucLogoffConter --;
			ucClientNeed = 0;
		}
		if(ucLogoffConter == 0)	//����ʱ�䵽���Ͽ�
		{
			ucClientNeed = 1;
			ucLogoffConter = 0xff;
//			Main_RComm_UpStop();	//�Ͽ�����
		}
		usServerTimeoutCount ++;
		if(usServerTimeoutCount > (unsigned short)(RunPara_GetHeartBeat().ucHeartBeat)*120*10)
		{
			usServerTimeoutCount = 0;
			ucReset_Mc55_Flag = 1;	//��λģ��
			ucLoginFlag = CON_STATUS_YES;
			Main_RComm_UpStart();	//��������
		}
		if(ucClientNeed == 0)//����TCP CLIENT���ӵ����Ҫ�������ж�
			Main_RComm_ContinueOnLine();
		else	//����Ҫ����
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


//ģ�鸴λ����
//ÿ��11��23ʱ����飬��������λģ��һ��
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
//���ӹ���
unsigned char Main_RComm_ConnetManage(void)
{//ͨѶ���߿��ƹ���
	unsigned char  ucRet=RET_SUCCESS;
	if(RunPara_GetTerCommPara_RCommMode()==CON_ONLINE_ALLWAYS		//�������߷�ʽ
		&& ucOverFluxLogoutCount>0)									//�Ҳ�������Խ�ޱ�������
	{
		ucUpActiveSelfFlag		= CON_ONLINE_INVALID;
		Main_RComm_ContinueOnLine();
	}
	else if(RunPara_GetTerCommPara_RCommMode()==CON_ONLINE_SMS_UP
		|| ucOverFluxLogoutCount==0)				//������߱�������
	{
		Main_RComm_UnContinueOnLine();	
	}
	else//���򲻵�½
	{
		Main_RComm_UpStop();
	}
	return ucRet;
}
//�������߲���û���������޵�����¹������ӣ����ֶ��ߺ�����ȥ����
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
			usDisConnectCount=0;							//���߱���ߣ���ʼ����
	}else
	{
		usDisConnectCount=0xFFFF;
		usReConnectCount=0xFFFF;
	}
	if((usDisConnectCount>=CON_DISCONNECT_COUNT) && (usDisConnectCount!=0xFFFF))
	{//ȷ�ϵ���,���˼���û������
		if(ucIPFrist == 0)
			ucIPFrist = 1;
		else
			ucIPFrist = 0;
		Main_RComm_UpStop();
		usReConnectCount=0;				//׼�������ӣ���ʼ����
		usDisConnectCount=0xFFFF;
	}
	if((usReConnectCount>=RunPara_GetReConnectTime()) && (usReConnectCount!=0xFFFF))
	{//����������
		Main_RComm_UpStart();
		usReConnectCount=0xFFFF;		//����������
		usDisConnectCount=0;			//���߼�������ʼ����
	}
}
//���������¿���û�г������Ӵ����������˾ͶϿ����ټ�����
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
			usUpOnConnectCount=0;							//���߱���ߣ���ʼ����
		if((usUpOnConnectCount>=CON_DISCONNECT_COUNT) && (usUpOnConnectCount!=0xFFFF))
		{//ȷ�ϵ���
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
		usStopOnConnectCount=RunPara_GetTerCommPara_StandbyTime();		//�Լ������������ӳ�50���Զ�����		

	return ucRet;
}
//�ڶ��ż������������������µ����ӹ���
void Main_RComm_UnContinueOnLine(void)
{//���ģ���ͨѶ���߿��ƹ���
//	STTERMISCPARA stTerMiscPara=RunPara_GetTerMiscPara();
//	unsigned char    ucFlag=CON_STATUS_YES;
	
	if(stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat!=CON_STATUS_YES)//������
	{
		usDisConnectCount=0xFFFF;
		usReConnectCount=RunPara_GetReConnectTime()-1;//�´�������������
		if(ucUpActiveSelfFlag!=CON_ONLINE_INVALID)
			Main_RComm_StopLogin();					
	}
	else	//����
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
					usStopOnConnectCount=RunPara_GetTerCommPara_StandbyTime();		//�Լ������������ӳ�50���Զ�����
				else
					usStopOnConnectCount =100;
			}
			else
				usStopOnConnectCount--;//���߼���
			if((usStopOnConnectCount==0) && (usStopOnConnectCount!=0xFFFF))
				ucUpActiveSelfFlag=CON_ONLINE_INVALID;
		}
		
		
		//azh if(ucUpActiveSelfFlag==CON_ONLINE_INVALID&&DP_Get_TerDP_State()!=CON_TERDP_STATE_DOWNLOAD)//���ڲ���������״̬
		if(ucUpActiveSelfFlag==CON_ONLINE_INVALID)//���ڲ���������״̬
		{
			//��ʱ�����Զ�����ʱ�䣬�������ֹ
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
	LoginFlag = CON_STATUS_NO;		//˯�߼�����
	ucIPFrist = 0;			//��ʼ����Ĭ������IP
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
{//����ģʽ˯��
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
//��˯�߿�ʼ������½�����ͣ�����״̬����Ҫ���Ż���
void MainComm_StartUp(unsigned char ucFlag)
{
	
//	STTERCOMMPARA_INNER	stTerCommPara_In=RunPara_GetTerCommPara_Inner();
//	STTERMISCPARA	stTerMiscPara;
	
//	stTerMiscPara=RunPara_GetTerMiscPara();
	
	//if(stTerRunPara.stTerCommPara_Inner.ucRCommMode==CON_ONLINE_SMS_UP) //���Զ��Ż���
//	{		
		Main_RComm_UpStart();
		Main_Connect_UpActiveSelfFlag(ucFlag);			//�������ͺ�50��͵���
		//			usUpOnConnectCount=CON_DISCONNECT_COUNT;
//	}
}

unsigned char Main_RComm_UpStart(void)
{//����ģʽ����
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

//���ӵ��绰����øú������ڵ�½
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
//ͨ���������޺����߹������˳���½֡��Ȼ������
void Main_RComm_OverFlux_Logout(void)
{//����Խ���˳���½
	if(ucOverFluxLogoutCount!=0xFF && ucOverFluxLogoutCount!=0)
		ucOverFluxLogoutCount--;
	if(ucOverFluxLogoutCount==40)
	{//���20��׼�������˳���½֡
//		Main_SendComm_CommConnect(2);
	}
	else if(ucOverFluxLogoutCount==1)
	{//����
		Main_RComm_UpStop();
	}
}
////����ͨ��Խ���˳���½������
void Main_RComm_Set_OverFlux_Logout(unsigned char ucCount)
{
	ucOverFluxLogoutCount=ucCount;
}
////��ȡͨ��Խ���˳���½������
//unsigned char Main_RComm_Get_OverFlux_Logout(void)
//{
//	return ucOverFluxLogoutCount;
//}
//��ȡ��ǰ�Ƿ���������
unsigned char Main_RComm_Get_OverFlux_AlrStatus(void)
{
	if(ucOverFluxLogoutCount<=100)
		return CON_STATUS_YES;
	else
		return CON_STATUS_NO;
}

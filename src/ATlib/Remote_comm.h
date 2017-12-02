/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Remote_comm.h
 Description    : Զ��ͨѶ����ģ�� ͨѶ���ƣ���ʱ�ж�
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2011-09-23      azh           the original version
******************************************************************************/
#ifndef _REMOTE_COMM_H_
#define _REMOTE_COMM_H_

#define		CON_LOGINSEQ_NUM	10
#define	    CON_DISCONNECT_COUNT  600//300�����������ϵ��´�����
#define		CON_BEATHEARTNOACK_MAXNUM	3					//��������δȷ��������

typedef	struct _STRMSENDBUFFER
{
	unsigned short	Send_Len;
	unsigned char	ucLink;
	unsigned char	Send_Buffer[400];	//#define 	CON_TC35COMM_DATA_LEN			640//(512+96)		//TC35ͨ��������󳤶�
	unsigned char	Reserve[3];
}STRMSENDBUFFER;

#define	CONRMSENDTASKNUM			4//20
typedef	struct	_STRMSENDTASKLIST
{
	unsigned char	ucList_Head_P;
	unsigned char	ucList_Tail_P;
	STRMSENDBUFFER	stRMSendBuffer[CONRMSENDTASKNUM];
}STRMSENDTASKLIST;
//-----------------------------------------------------------------------------
__EXTERN unsigned short		usLoginSEQCount[CON_LOGINSEQ_NUM];				//��¼֡seq
__EXTERN unsigned short		usSmsSEQCount;
__EXTERN unsigned char		ucTerSEQCount;
__EXTERN unsigned char		ucBeatHeartNoAckCount;				//��������δȷ�ϵĴ���
__EXTERN unsigned short		usHeartBeatCount;
__EXTERN unsigned char		ucNeedLoginFlag;					//��Ҫ��¼�ı�־


__EXTERN unsigned char	ucRCommResetCount;
__EXTERN unsigned char	ucRCommResetHour;


__EXTERN unsigned short		usDisConnectCount;				//�����������������Ͽ����Ӽ�������ÿ���������ֵ��0����300��û�����Ͼ͵��ߣ����´�����
__EXTERN unsigned short		usReConnectCount;				//������׼��������,��������ʱ�������ȥ����
__EXTERN unsigned short		usUpOnConnectCount;				//���ʽ��½��ʱ,��������������
__EXTERN unsigned short		usStopOnConnectCount;           //ֹͣ���߿�ʼ��ʱ���������Ϻ��Զ����߼�����
__EXTERN unsigned short		usServerTimeoutCount;			//������ģʽ����ͨ�ų�ʱ������
__EXTERN unsigned char		ucIPFrist;		//IP��ַѡ��λ��0����IP��1�ô�IP

__EXTERN unsigned char		ucUpOnErrCount;					//���ʽδ��½����
__EXTERN unsigned char		ucUpActiveSelfFlag;				//�Զ�����ԭ���־,��Ч�����Ƕ��ż���
__EXTERN unsigned char 		LoginFlag;				   //��Ҫ���ñ�־
__EXTERN unsigned char		ucOverFluxLogoutCount;	//ͨ��Խ���˳���½������
 
__EXTERN unsigned char ucLoginFlag;
__EXTERN unsigned char ucLogoffConter;
__EXTERN unsigned char ucClientNeed;
//-----------------------------------------------------------------------------
__EXTERN void Main_Connect_UpActiveSelfFlag(unsigned char ucFlag);

__EXTERN void Main_RComm_ContinueOnLine(void);
__EXTERN void Main_RComm_UnContinueOnLine(void);
__EXTERN unsigned char Main_RComm_StopLogin(void);
__EXTERN unsigned char Main_RComm_UpStart(void);
__EXTERN unsigned char Main_RComm_SetStandBy_Time(void);
__EXTERN unsigned char Main_RComm_RingStart(void);
__EXTERN unsigned char Main_RComm_Get_OverFlux_AlrStatus(void);
__EXTERN void Main_RComm_Set_OverFlux_Logout(unsigned char ucCount);
__EXTERN void HeartBeat_Manage(void);
__EXTERN unsigned char Main_Get_Comm_Type(void);
__EXTERN void Main_RComm_Init_LoginSleepCount(void);
__EXTERN unsigned char TerSeq_GetCount(void);
__EXTERN unsigned char Main_SendComm_CommConnect(unsigned char ucCmdType);
__EXTERN void CommConnect_SetNeedLoginFlag(unsigned char ucFlag);
__EXTERN unsigned char CommConnect_GetNeedLoginFlag(void);
__EXTERN void CommConnect_SetBeatHeartNoAckCount(unsigned char ucCount);
__EXTERN unsigned char   RunPara_GetTerCommPara_RCommType(void);
#endif



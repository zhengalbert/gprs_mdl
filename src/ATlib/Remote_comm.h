/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Remote_comm.h
 Description    : 远方通讯控制模块 通讯控制，超时判断
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2011-09-23      azh           the original version
******************************************************************************/
#ifndef _REMOTE_COMM_H_
#define _REMOTE_COMM_H_

#define		CON_LOGINSEQ_NUM	10
#define	    CON_DISCONNECT_COUNT  600//300三分钟连不上等下次再连
#define		CON_BEATHEARTNOACK_MAXNUM	3					//心跳连续未确认最大次数

typedef	struct _STRMSENDBUFFER
{
	unsigned short	Send_Len;
	unsigned char	ucLink;
	unsigned char	Send_Buffer[400];	//#define 	CON_TC35COMM_DATA_LEN			640//(512+96)		//TC35通信数据最大长度
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
__EXTERN unsigned short		usLoginSEQCount[CON_LOGINSEQ_NUM];				//登录帧seq
__EXTERN unsigned short		usSmsSEQCount;
__EXTERN unsigned char		ucTerSEQCount;
__EXTERN unsigned char		ucBeatHeartNoAckCount;				//心跳连续未确认的次数
__EXTERN unsigned short		usHeartBeatCount;
__EXTERN unsigned char		ucNeedLoginFlag;					//需要登录的标志


__EXTERN unsigned char	ucRCommResetCount;
__EXTERN unsigned char	ucRCommResetHour;


__EXTERN unsigned short		usDisConnectCount;				//持续断连计数器，断开连接计数器，每次重连后该值置0，到300后还没有连上就掉线，等下次再连
__EXTERN unsigned short		usReConnectCount;				//重连接准备计数器,到达重连时间后马上去连接
__EXTERN unsigned short		usUpOnConnectCount;				//激活方式登陆计时,激活重连计数器
__EXTERN unsigned short		usStopOnConnectCount;           //停止断线开始计时，激活连上后自动掉线计数器
__EXTERN unsigned short		usServerTimeoutCount;			//服务器模式下无通信超时计数器
__EXTERN unsigned char		ucIPFrist;		//IP地址选择位，0用主IP，1用从IP

__EXTERN unsigned char		ucUpOnErrCount;					//激活方式未登陆次数
__EXTERN unsigned char		ucUpActiveSelfFlag;				//自动上线原因标志,无效或者是短信激活
__EXTERN unsigned char 		LoginFlag;				   //需要重置标志
__EXTERN unsigned char		ucOverFluxLogoutCount;	//通信越限退出登陆计数器
 
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



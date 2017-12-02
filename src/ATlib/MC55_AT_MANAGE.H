/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : MC55_AT_MANAGE.h
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2011-09-23      azh           the original version
******************************************************************************/

#ifndef		_AT_MANAGE_H
#define		_AT_MANAGE_H
#include	"MC55_AT_TASK.H"
#include	"MC55_buffermanage.h"
#include	"MC55_module.h"

#define		AT_CMD_INVALID				0x0000
#define		AT_CMD_SET_AT				0x0101
#define		AT_CMD_SET_ATE0				0x0102
#define		AT_CMD_SET_CNMI				0x0103
//#define		AT_CMD_QUERY_CREG			0x0104
#define     AT_CMD_SET_CREG             0x0104
#define		AT_CMD_SET_CHUP				0x0105
#define		AT_CMD_SET_CSCA				0x0106
#define		AT_CMD_QUERY_CSCA			0x0107
#define		AT_CMD_QUERY_CSQ			0x0108
#define		AT_CMD_SET_FLO              0x0133

//密码相关指令
#define		AT_CMD_QUERY_CPIN			0x0109
#define		AT_CMD_SET_UNLOCK_PIN		0x010a
#define		AT_CMD_SET_UNLOCK_PUK		0x010b
#define		AT_CMD_SET_CPWD				0x010c
#define		AT_CMD_SET_CLCK0				0x010d
#define		AT_CMD_SET_CLCK1				0x010e

//短信相关
#define		AT_CMD_SET_CMGF				0x010f
#define		AT_CMD_QUERY_CMGL			0x0110
#define		AT_CMD_SET_CPMS				0x0111
#define		AT_CMD_SET_CMGS				0x0112
#define		AT_CMD_SET_CMGD				0x0113


//GPRS相关
#define		AT_CMD_SET_SICS_GPRS0		0x0114
#define		AT_CMD_SET_SICS_APN			0x0115
#define		AT_CMD_SET_SICS_USER			0x0116
#define		AT_CMD_SET_SICS_PASSWD		0x0117

#define		AT_CMD_SET_SISS_SRVTYPE_SOCKET	0x0118
#define		AT_CMD_SET_SISS_ADDRESS_TCP	0x0119
#define		AT_CMD_SET_SISS_ADDRESS_UDP	0x011a

#define		AT_CMD_SET_SISO				0x011b
#define		AT_CMD_SET_SISC				0x011c
#define		AT_CMD_SET_SISW				0x011d
#define		AT_CMD_SET_SISR				0x011e

#define		AT_CMD_QUERY_SICS			0x011f

#define   	AT_CMD_SET_SISS_CONID		0X0120
//模块关电
#define		AT_CMD_SET_SMSO				0x0121


#define		AT_CMD_SET_SISS_ALPHABETAT  0x0124

#define     AT_CMD_SET_IPR				0x0125
#define		AT_CMD_SET_CFUN				0x0126
#define		AT_CMD_SET_SSMSS              		0x0127
#define		AT_CMD_SET_SM20                		0x0128
#define 	AT_CMD_SET_ATQ3				0x0129
#define 	AT_CMD_SET_CMEE				0x012A
#define		AT_CMD_QUERY_GMR				0x012B
#define		AT_CMD_QUERY_CPMS           		0x012C
#define		AT_CMD_QUERY_CNMI			0x012D
#define		AT_CMD_SET_SGCONF			0X12E
#define		AT_CMD_QUERY_SGCONF			0x12F
#define		AT_CMD_SET_CGATT_DETACH		0x0131
#define		AT_CMD_SET_CGATT_TACH			0x0132
#define		AT_CMD_SET_GPRS             0x0134
#define		AT_CMD_SET_SCFGEXT          0x0135
#define		AT_CMD_SET_SCFG             0x0136
#define		AT_CMD_SET_SGACT            0x0137
#define		AT_CMD_SET_CGATT_DESOC      0x0138
#define		AT_CMD_SET_DESGACT          0x0139
#define		AT_CMD_SET_FRS              0x013A
#define		AT_CMD_SET_SA               0x013B
#define		AT_CMD_SET_SL               0x013C
#define		AT_CMD_SET_FRWL             0x013D
#define		AT_CMD_SET_CGATT_OFFSERVER  0x013E
#define		PIN_CMD_DELAY				0x0200
#define		PIN_CMD_IGT					0x0201
#define		PIN_CMD_DELAY_10MS			0x0202
#define		PIN_CMD_EMERGOFF				0x0203


//#define		PIN_CMD_ONOFF			0x0201
//#define		PIN_CMD_DTR				0x0202
//#define		PIN_CMD_DSR				0x0203
#define		PIN_CMD_PWCTK				0x0204

//#define		CON_ConnectIndicateTimes	50//100		   //间隔500ms
#define		CON_CONNECTINDICATE_TIMES	AT_DLY_500MS//50//100		   //间隔500ms
#define     NEW_SMS_ENTITY				0xFF
#define     NEW_SMS_NOT_ENTITY			0x00

#define		CON_ShortSIMIndicateTimes	25
#define		CON_LongSIMIndicateTimes	205
#define		CON_DATAREADTIMES			AT_DLY_5S//500




typedef enum _ENU_AT_STATE
{	
		STATE_POWEROFF	, 
		STATE_POWERON	, 
		STATE_PWD_CHK	,
		STATE_LOOP		,
		STATE_INIT		,
		STATE_GSM		,
		STATE_GPRS		
//		STATE_GPRS_CMD	,
//		STATE_TCP		,
//		STATE_UDP
}ENU_COMMUNICATE_STATE;

typedef enum _ENU_BUFFER_STATE
{	
	STATE_BUFFER_IDLE	, 
	STATE_BUFFER_SEND	, 
	STATE_BUFFER_WAIT	
}ENU_BUFFER_STATE;

typedef enum _ENU_AT_RESULT
{	
	AT_RESULT_OK	, 
	AT_RESULT_ERROR	, 
	AT_RESULT_CSQ,
	AT_RESULT_CREG,
	AT_RESULT_SMS,
	AT_RESULT_CMS_ERROR,
	AT_RESULT_CME_ERROR,
	AT_RESULT_CMGL_ERROR,
	AT_RESULT_CGDCONT,
	AT_RESULT_RING,
	AT_RESULT_NOCARRIER	, 
	AT_RESULT_CONNECT, 
	AT_RESULT_NEEDDATA, 
	AT_RESULT_CMGL,
	AT_RESULT_CGREG,
	AT_RESULT_NOPROTECT, 
	AT_RESULT_PUK,
	AT_RESULT_PIN,
	AT_RESULT_CPIN,
	AT_RESULT_REVISION,
	AT_RESULT_UNKNOW, 
	AT_RESULT_BLANK,
	AT_RESULT_CPMS,
	AT_RESULT_CNMI,
	AT_RESULT_URC_SMS,
	AT_RESULT_SISW,
	AT_RESULT_URC_SIS,
	AT_RESULT_SISR,
	AT_RESULT_URC_SISR,
	AT_RESULT_SYSSTART,
	AT_RESULT_SHUTDOWN,
	AT_RESULT_SMSO,
	AT_RESULT_CMGS,
	AT_RESULT_LINK_CLOSED,
	AT_RESULT_SERVERLINK_CLOSED,
	AT_RESULT_SERVERLINK,
	AT_RESULT_SGACT,
	AT_RESULT_FORMATERROR
	
}ENU_AT_RESULT;

typedef enum _ENU_GR47_STATE
{	
	GR47_STATE_CMD,
	GR47_STATE_DATA,
	GR47_STATE_ONLINECMD,
	GR47_STATE_INVALID
}ENU_GR47_STATE;

typedef struct 
{
	ENU_BUFFER_STATE		enuBufferState;
	unsigned short	ucATCMD;		//命令代码
//	unsigned char	ucParam;		//命令参数
	unsigned short	ucParam;		//命令参数	//azh 主要是延时命令 一个字节放不下
	unsigned char	ucRetryCnt;		//命令重发次数ms
	unsigned int	uTimeOutCount;	//命令超时时间ms	
	unsigned char	ucLength;		//命令解释字符长度
	unsigned char	ucCMDBuffer[80];
}ST_AT_CMD_BUFFER;

typedef	struct {
	unsigned char	ucIsValid;		//是否有效
	unsigned short	usLength;		//总长度
	unsigned short	usTPALen;		
	unsigned char	ucData[CON_SMS_BUFFER_MAX];
}ST_SMS_BUFFER;

typedef struct 
{
	unsigned short	usLength;		
	unsigned short	usProcPtr;		
	unsigned char	ucIsGetVaild;
	unsigned char	ucRetryCnt;		//命令重发次数ms
	unsigned short	usSilenceCnt;	//字符接收停止后的等待时间ms
	unsigned int	uTimeOutCount;	//命令超时时间ms	
	unsigned char	ucBuffer[CON_AT_MAX_REVEIVE_PACKAGE];
}ST_AT_RECEIVE_BUFFER;

typedef struct 
{
	

	unsigned int	uStateTimerCounter;		//状态切换的计数器，减为0后开始判断状态作相应的切换
	unsigned int	uPowerOffTimer;			//关电定时计数器,减为0后关机
	unsigned int	uReceiveTimer;			//数据检测定时计数器，一定时间内无数据接收则重新连接,最大值为断线重连时间(打开AT命令)
	unsigned int	uNoReceiveTimer;		//无数据收到计数器，初始值20小时，如果在20小时内一直没有收到过数据，重启模块
	unsigned char	ucERRORCount;			//当前短信发送错误计数器：
	unsigned char	ucTimeOutTimes;			//短信发送失败次数
	unsigned char	ucReconnectTimes;		//GPRS连接次数计数器,超过6次连不上就不去连了
	unsigned char	ucCSQ;					//CSQ
	unsigned char   ucCSQErrorCount; 		//CSQ返回错误次数
	unsigned char   ucQCPMSErrorCount; 		//CSQ返回错误次数	
	unsigned char   ucSCPMSErrorCount; 		//CSQ返回错误次数	
	unsigned int 	uConnectIndicateTimes;	//
	unsigned char   GPRSNeedReconnect;
	unsigned char   OffRegTimes;
	unsigned char   ucPowerOffNum;
	unsigned short  usDataReceiveLength;
	unsigned short  usDataSendLength;
	unsigned short  usDataSendSutLength;
	unsigned char	ucDataReadDelayTimes;
	unsigned char   ucQUERYTimes; 		//10*x
	unsigned char 	ucCMGLErrorCount;
	unsigned short 	uServertimes;
	unsigned char 	ucSISWBusyCheckTimes;
	
//	unsigned char	ucCurrentAccount;		//GPRS当前连接帐号
	unsigned char	ucIsSMSReceived;
	unsigned char	ucCurrentCid;			//GPRS当前连接通道
	unsigned char   bIsNewSMSENTITY;
	unsigned char   ucPinDisconnectedIndicate;
	unsigned char   ucIsDataSending;
	unsigned char 	ucIsDataRecving;
	unsigned int	usReadDataTimes;        //定时读取数据间隔
	unsigned char   ucSISRNeeded; 
	unsigned char   ucSISRParam;
	unsigned char 	ucTaskCount;

	unsigned char	bIsPWDUsed;				//是否启动密码保护//pin
	
	unsigned short 	uSIMIndicateTimes;
	unsigned char	bIsPowerOff;				//软件关电标志	
}ST_AT_MANAGE_STATE;

//--------------------------------------------------------------
__EXTERN ST_AT_MANAGE_STATE				stAtManageState;	//at状态处理相关变量
__EXTERN ST_AT_RECEIVE_BUFFER			stReceiveBuffer;	//GR数据接收缓冲 
__EXTERN ST_AT_CMD_BUFFER				stCMDBuffer;		//AT命令解释缓冲区
__EXTERN ST_SMS_BUFFER					stSMSBufer;			//短信缓冲区;

__EXTERN unsigned char     Serverflag;
__EXTERN unsigned char     Clientflag;
__EXTERN ENU_COMMUNICATE_STATE			enuATSourceState,enuATDstState,enuATCurrentState,enuATNextState;
//--------------------------------------------------------------
__EXTERN void AT_Init(void);
__EXTERN void AT_State_Manage(void);
__EXTERN void AT_Send_Manage(void);
__EXTERN void AT_Receive_Manage(void);
__EXTERN void AT_Send_CMD_EXPLAIN(unsigned char ucPara);
__EXTERN void AT_DataList_Clear(void);
__EXTERN void AT_Timer(void);
__EXTERN void AT_Manage(void);
__EXTERN void AT_Power(void);


__EXTERN unsigned char IsNoDataInATBuffer(PST_SCI_BUFFER pBuffer);

__EXTERN unsigned char AT_Send_FillBuffer(void);
__EXTERN unsigned char AT_CMD_Fill(unsigned short usAtCmd, unsigned short ucCmdParam);
__EXTERN unsigned short GetStringFromStr(unsigned char* pucStr);

__EXTERN unsigned char AT_Receive_Process_CGREG(char* pStr);
__EXTERN unsigned char AT_Receive_Process_CMSERROR(void);
__EXTERN unsigned char AT_Receive_Process_CMEERROR(void);
__EXTERN unsigned char AT_Receive_Process_CSQ(unsigned char* pStr);
__EXTERN unsigned char AT_Receive_Process_CMGL(unsigned char* pStr);
__EXTERN unsigned char AT_Receive_Process_ERROR(void);
__EXTERN unsigned char AT_Receive_Process_OK(void);
__EXTERN unsigned char AT_Receive_Process_OK_T(void); //azh 141024
__EXTERN unsigned char AT_Receive_Process_UNKNOWN(unsigned char* );
__EXTERN unsigned char AT_Receive_Process_CPMS(unsigned char* pStr);
__EXTERN unsigned char AT_Receive_Process_CNMI(unsigned char* pStr);
__EXTERN void CMD_TimeOutProcess(void);

__EXTERN ENU_AT_RESULT AT_CHECK_STRING(unsigned char* pucSrc);
__EXTERN ENU_AT_RESULT AT_Receive_Explain(void);

__EXTERN void TCPUDP_Receive_Explain(void);
__EXTERN void ConnectIndicate(void);
__EXTERN unsigned char AT_Receive_Process_URC_SIS(unsigned char* pStr);
__EXTERN unsigned char AT_Receive_Process_SISW(unsigned char* pStr);
__EXTERN unsigned char AT_Receive_Process_SISW_T(unsigned char* pStr);

__EXTERN unsigned char AT_CMD_Process_SendSMS(void);
__EXTERN unsigned char AT_CHANGE_PWD(void);
__EXTERN void ClearCmdBuffer(void);
//void ModemLedPower(void);
__EXTERN unsigned char AT_Receive_Process_SISR(unsigned char* pStr);
__EXTERN unsigned char AT_Receive_Process_SISR_T(unsigned char* pStr);

__EXTERN unsigned char AT_StrFromStr_ByChr(char* pStr, char*pDst, char cIndicate);
__EXTERN unsigned short AT_GetStringFromATBuffer(unsigned char* pstr);
__EXTERN void AT_CMD_Resend(ENU_BUFFER_STATE	 enuDestState);
__EXTERN unsigned char IP_Check(unsigned char ucChannel);
//void Character_Replace(unsigned char *pparameter,unsigned char ucLen);
__EXTERN void  ASCTOHEX(unsigned char *Hexdata,unsigned char *Ascdata,unsigned short len);

__EXTERN unsigned char Init_ModChInfo(STMODPARACHINFO *pstModParaChInfo);
__EXTERN unsigned char AT_Recvive_LinkClose(unsigned char* pStr);
__EXTERN unsigned char IPchange(unsigned char ucChannel,unsigned char *ucIP);
__EXTERN unsigned char IPportchange(unsigned char ucChannel,unsigned char *ucPort);
__EXTERN unsigned char IPportchange2(unsigned short usinPort,unsigned char *ucPort);
__EXTERN unsigned char AT_Recvive_ServerLinkClose(unsigned char* pStr);
__EXTERN unsigned char AT_Receive_Process_SGACT(unsigned char* pStr);
__EXTERN unsigned char Get_PhoneNum_Str(unsigned char *pucPhone,unsigned char ucPhoneLen,unsigned char *pstr);
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
__EXTERN unsigned char Get_Signal_Strenth(void);
#endif

#endif

#ifndef _REMOTECOMM_PUBLIC_H
#define _REMOTECOMM_PUBLIC_H

#define 	CON_TC35COMM_RECV_DATA_LEN		1200//1400//1024//2176

#define		CON_SMSBUFF_MAXLEN		140
//#define		CON_SMSBUFF_RESULTLEN	100//552//1104				//138*4=552
//#define		CON_SMS_NUM				1//8
//#define		CON_SMSUNITE_NUM		1//3

#define 	CON_TC35COMM_TCP_LimitLen			460+22+1		//TCP通信数据最大长度为540,规约为512字节，实际限定460
#define 	CON_TC35COMM_UDP_LimitLen			220+22+1		//UDP通信数据最大长度为540，主战只识别255字节
#define 	CON_TC35COMM_SMS_LimitLen			140				//SMS通信数据最大长度

//通讯方式
#define CON_TC35COMMTYPE_INVALID	0x00
#define CON_TC35COMMTYPE_GSM		0x01
#define CON_TC35COMMTYPE_TCP		0x02
#define CON_TC35COMMTYPE_UDP		0x03

#define	CON_PIN_LOCK					0x55
#define	CON_PIN_UNLOCK					0xAA	//未锁定

#define	CON_RSSI_BAR_4			31
#define	CON_RSSI_BAR_3			18
#define	CON_RSSI_BAR_2			10
#define	CON_RSSI_BAR_1			5

typedef struct _STREMOTERUNINFO{
	unsigned char	ucSignIntension;	//信号强度	
	unsigned char	ucCurrChanType;		//当前通信通道类型
	unsigned char	ucPinStat;			//PIN码锁定标记
	unsigned char	ucRCommStat;		//模块通迅状态
	unsigned char   ucGprsLastLoginStat; //上次登录状态
	unsigned char   ucGprsLoginStat;	//登录状态		在混合模式下特指client已连接。与服务器是否连接无关
	unsigned char	ucRCommStatBak1;	//模块通迅状态备份1
	unsigned char	ucRCommStatBak2;	//模块通迅状态备份2
	unsigned char   ucModHardVer[4];	//模块硬件版本信息
	unsigned char   ucModSoftVer[4];	//模块软件版本信息
}STREMOTERUNINFO;	


typedef struct _STRCOMMRECEDATA
{
	unsigned short usHead_P;
	unsigned short usTail_P;
	unsigned char ucDataArea[CON_TC35COMM_RECV_DATA_LEN];		//数据区内容
}STRCOMMRECEDATA;

typedef struct _STSMSBUFF
{
	unsigned short usSmlSeq;						//短信帧序号
	unsigned char  ucDataLen;						//长度	
	unsigned char  ucBuff[CON_SMSBUFF_MAXLEN];		//短消息内容
}STSMSBUFF;
//typedef struct _STSMSUNITEINFO
//{
//	STDATETIME	   stDateTime_Recvstart;			//收到第一条短信时间
//	unsigned short usSmsSeq;						//报文帧序号
//	unsigned short usSmlSeq_End;					//短信结束帧序号
//	unsigned short usSmlSeq_RecvNum;					//接收数目
//	STSMSBUFF	   stSMSBuf[CON_SMS_NUM];			
//}STSMSUNITEINFO;
//---------------------------------------------------------
//Gprs相关变量
__EXTERN STRCOMMRECEDATA	stGprsDataBuf;		//现TCP、UDP用同一个缓冲区，若终端能同时使用TCP、UDP则增加缓冲区分开处理
//SMS相关变量 国网用
//__EXTERN STSMSUNITEINFO	stSMSUniteInfo[CON_SMSUNITE_NUM];
//__EXTERN unsigned char	ucSMSResponseBuff[CON_SMSBUFF_RESULTLEN];	//复合后数据缓冲
//__EXTERN STDATETIME	gstCurrDateTime;		//终端当前时间
//--------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///功能: 初始化本模块的一些操作，变量
//__EXTERN void			RComm_Initial(unsigned long ulBoadrate);		//初始化函数
__EXTERN void			RComm_Initial(void);		//初始化函数
__EXTERN unsigned char	RComm_FillUpData(unsigned char *pBuf,unsigned short usLen,unsigned char ucMsgSource);
//__EXTERN unsigned char RComm_AddUpSendTask(unsigned char *pucSendBuffer,unsigned short usLen,unsigned char ucChanNo);

#endif 


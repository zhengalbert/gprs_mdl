/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Meter_Protocol_Public.h
 Description    : 抄表控制
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/
#ifndef _METER_PROTOCOL_PUBLIC_H_
#define	_METER_PROTOCOL_PUBLIC_H_

#define		CON_METER_NUM_PRO	2//8
#define		CONCOMMAND_BIG		0x55
#define		CONCOMMAND_SML		0xAA

#define		CONCOMMAND_SEND		0x55
#define		CONCOMMAND_NOSEND	0xAA

#define		CON_CMDLIST_NUM		150//100		//100条命令不够了，增加 cfh 10-04-04

#define		CONALRTME_VALID_FLAG		0x55
#define		CONALRTME_INVALID_FLAG		0xAA
#define		CONALRAFR_RESP_FLAG			0xBB
/****************表通信规约编号定义*******************/		
#define		CON_PROTOCOL_INVALID	0		//无效			//内部定义通信规约类型
#define		CON_PROTOCOL_DL645		1		//645规约
#define		CON_PROTOCOL_SAMPLE		2		//交流采集

#define		CON_PROTOCOL_ABB_R		3		//ABB方表
#define		CON_PROTOCOL_LANDD		4		//西门子D表
#define		CON_PROTOCOL_LANDB		5		//西门子B表
//#define		CON_PROTOCOL_GENIUS		6		//红相
#define		CON_PROTOCOL_AS1_DL645	6		//645规约类似一(鄂规（多功能）)
#define		CON_PROTOCOL_WS			7		//威胜4.x


#define		CON_PROTOCOL_HND		10		//浩宁达

#define		CON_PROTOCOL_AS2_DL645	15		//645规约类似二(鄂规（简易多功能）)
#define		CON_PROTOCOL_HB_PT		17		//鄂规之普通电子式电能表
#define		CON_PROTOCOL_HB_JM		18		//鄂规之居民复费率电能表

#define		CON_PROTOCOL_HLi		21		//浙江华立 (645)
#define		CON_PROTOCOL_KL			22		//科陆 (645)
#define		CON_PROTOCOL_LD			23		//龙电 (645)
#define		CON_PROTOCOL_SKD		24		//深科达 (645)
#define		CON_PROTOCOL_JSLY		25		//江苏羚羊 (645)
#define		CON_PROTOCOL_HND645		26		//浩宁达 (645)
#define		CON_PROTOCOL_DFDZ		27		//东方电子 (645)
#define		CON_PROTOCOL_JQ			28		//金雀 (645)

#define		CON_PROTOCOL_DL64507	30

#define		CON_PROTOCOL_MAX		71		//限制内部编号最大为71
/*******************************************************/

#define	CONRECVEXPLAINNUM	12//azh 100			//大项解释转分项个数
#define	CONRELAYCOMMANDLEN	70
#define	CON645RELAYCOMMANDLEN	500			//中继长度限制500字节

#define RS485_CH_ID			1			//uart 通道编号
#define	RS485_CH_ID2		4
/*
#define	RS485_BYTE_OVER_TIME		1600	//单位0.25*4ms	1.6秒
#define	RS485_RECV_OVER_TIME		2000	//单位0.25*4ms	2秒

#define	RS485_RECV_DELAY			120	//单位0.25ms   30ms
#define RS485_SEND_DELAY			80	//单位0.25ms   20ms
*/
#define	RS485_BYTE_OVER_TIME		160	//单位10ms	1.6秒
#define	RS485_RECV_OVER_TIME		200	//单位10ms	2秒

#define	RS485_RECV_DELAY			3	//单位10ms   30ms
#define RS485_SEND_DELAY			2//20//2	//单位10ms   20ms		changed for test 09-11-24

#define CONFRONTBYTENUM		0			//帧前导字节个数
#define CONFRONTBYTEVALUE	0xFE		//帧前导字节

typedef struct _STCMDFLAG
{
	unsigned char	ucDirPowerFlag;										//正向有功电量块
	unsigned char	ucUnDirPowerFlag;									//反向有功电量块
	unsigned char	ucDirVarFlag;										//正向无功电量块
	unsigned char	ucUnDirVarFlag;										//反向无功电量块
	unsigned char	ucVar1Flag;											//一象限无功电量块
	unsigned char	ucVar2Flag;											//二象限无功电量块
	unsigned char	ucVar3Flag;											//三象限无功电量块
	unsigned char	ucVar4Flag;											//四象限无功电量块
	unsigned char	ucDirPowerMaxDemandFlag;				//正向有功最大需量数据块
	unsigned char	ucDirPowerMaxDemandTimeFlag;		//正向有功最大需量发生时间数据块
	unsigned char	ucDirVarMaxDemandFlag;					//正向无功最大需量数据块
	unsigned char	ucDirVarMaxDemandTimeFlag;			//正向无功最大需量发生时间数据块
	unsigned char	ucUnDirPowerMaxDemandFlag;			//反向有功最大需量数据块
	unsigned char	ucUnDirPowerMaxDemandTimeFlag;	//反向有功最大需量发生时间数据块
	unsigned char	ucUnDirVarMaxDemandFlag;				//反向无功最大需量数据块
	unsigned char	ucUnDirVarMaxDemandTimeFlag;		//反向无功最大需量发生时间数据块

	unsigned char	ucDirPowerLmFlag;								//正向有功电量块(上月)                
	unsigned char	ucUnDirPowerLmFlag;             //反向有功电量块(上月)                    
	unsigned char	ucDirVarLmFlag;                 //正向无功电量块(上月)                   
	unsigned char	ucUnDirVarLmFlag;               //反向无功电量块(上月)                    
	unsigned char	ucVar1LmFlag;                   //一象限无功电量块(上月)                  
	unsigned char	ucVar2LmFlag;                   //二象限无功电量块(上月)                  
	unsigned char	ucVar3LmFlag;                   //三象限无功电量块(上月)                  
	unsigned char	ucVar4LmFlag;                   //四象限无功电量块(上月)                  
	unsigned char	ucDirPowerMaxDemandLmFlag;      //正向有功最大需量数据块(上月)            
	unsigned char	ucDirPowerMaxDemandTimeLmFlag;  //正向有功最大需量发生时间数据块(上月)    
	unsigned char	ucDirVarMaxDemandLmFlag;        //正向无功最大需量数据块(上月)            
	unsigned char	ucDirVarMaxDemandTimeLmFlag;    //正向无功最大需量发生时间数据块(上月)    
	unsigned char	ucUnDirPowerMaxDemandLmFlag;    //反向有功最大需量数据块(上月)           
	unsigned char	ucUnDirPowerMaxDemandTimeLmFlag;//反向有功最大需量发生时间数据块(上月)    
	unsigned char	ucUnDirVarMaxDemandLmFlag;      //反向无功最大需量数据块(上月)            
	unsigned char	ucUnDirVarMaxDemandTimeLmFlag;  //反向无功最大需量发生时间数据块(上月)    

	unsigned char	ucVolFlag;						//电压
	unsigned char	ucCurFlag;						//电流
	unsigned char	ucInstantPowFlag;				//有功功率
	unsigned char	ucInstantVarFlag;               //无功功率
	unsigned char	ucProgramTimeFlag;				//编程时间、电池工作时间等信息，对于07规约就表示电池工作时间
	unsigned char	ucPulseConstFlag;               //脉冲常数
	unsigned char	ucMeterStatFlag;				//电表状态字
	unsigned char	ucCopyDayTimeFlag;              //抄表日
	unsigned char	ucLostVolNumFlag;               //断相次数
	unsigned char	ucLostVolTimeFlag;              //断相时间
	unsigned char	ucLastVolOffBegTimeFlag;        //最近一次断相开始时间
	unsigned char	ucLastVolOffEndTimeFlag;        //最近一次断相结束时间
	unsigned char	ucFeeRateFlag;					//时段表
	unsigned char	ucDateTimeFlag;                 //日期时间

	unsigned char	ucProgramNumFlag;				//编程总次数
	unsigned char	ucLastProgramTimeFlag;			//最近一次编程时间
	unsigned char	ucMeterclrNumFlag;				//电表清0总次数
	unsigned char	ucLastMeterclrTimeFlag;			//最近一次电表清0时间
	unsigned char	ucDemondclrNumFlag;				//需量清0总次数
	unsigned char	ucLastDemondclrTimeFlag;		//最近一次需量清0时间
	unsigned char	ucEventclrNumFlag;				//事件清0总次数
	unsigned char	ucLastEventclrTimeFlag;			//最近一次事件清0时间
	unsigned char	ucJiaoshiNumFlag;				//校时总次数
	unsigned char	ucLastJiaoshiTimeFlag;			//最近一次校时时间
	unsigned char ucFeCuFlag;						//铜损、铁损参数

	unsigned char ucDirPowAFlag;	
	unsigned char ucDirPowBFlag;
	unsigned char ucDirPowCFlag;

	unsigned char ucDirPowALmFlag;	
	unsigned char ucDirPowBLmFlag;
	unsigned char ucDirPowCLmFlag;
}STCMDFLAG; 

/********************************************************/
//表计测量点与表计地址的对应表
typedef struct _STMPNOADDRREC
{
	unsigned char	ucMPNo;
	unsigned char	ucProtocolNo;
	unsigned char	ucMeterPass[8];
	unsigned char	ucMeterAddr[6];
	unsigned char	ucMeterCommName[8];
}STMPNOADDRREC;
/*
#define CON_METER_PASSLEN		8		//密码长度
#define	CON_METER_ADDRLEN		6		//电表地址长度
#define CON_METER_NAMELEN		8		//通迅名长度
*/
typedef struct _STMPNOADDRLIST
{
	STMPNOADDRREC	stMPNoAddrRec[CON_METER_NUM_PRO];
}STMPNOADDRLIST;
/********************************************************/
typedef struct _STRS485TASKCMD
{
	unsigned char	ucReportType;
	unsigned short	usDataType;
}STRS485TASKCMD;

typedef struct _STRS485TASKCMDLIST
{
	STRS485TASKCMD	stRS485TaskCmd[CON_CMDLIST_NUM];
}STRS485TASKCMDLIST;
//---------------------------------------------------------
__EXTERN STRS485TASK stRS485TaskRecvExplain[CONRECVEXPLAINNUM];   //接受解释
__EXTERN STMPNOADDRLIST	stMPNoAddrList;
__EXTERN unsigned char	ucErrCount;			//重发次数，对于ABB来说，WHO ARE YOU 、握手 、 读CLASS 的命令是可重发的，其余，尤其是续读是不可重发的

/********************************************************/
//azh
////初始化规约解释队列
__EXTERN unsigned char Meter_Protocol_Init(STMPNOADDRLIST	*pstMPNoAddrList);
//
////发送解释
__EXTERN unsigned char Meter_Protocol_SendExplain(STRS485TASK *pstRS485Task,unsigned char *pSendBuffer,unsigned short *pusLen);
//
////接收解释
__EXTERN unsigned char Meter_Protocol_RecvExplain(STRS485TASK *pstRS485SendTask,
								unsigned char	*pRecvBuffer,
								unsigned short	usRecvLen,
								unsigned char	ucOverTimeFlag,
								unsigned char	*pucExplainResultNum);
////接收分离
__EXTERN unsigned char Meter_Protocol_GetExplainResult(unsigned char ucNo,STRS485TASK *pstRS485RecvExpainVal);
//
////帧校验
__EXTERN unsigned char Meter_Protocol_CheckFrame(STRS485TASK *pstRS485Task,unsigned char *pRecvBuffer,unsigned short usRecvLen,unsigned short *pusJumpLen);
//
////输出收发控制常数
//unsigned char Meter_Protocol_GetTimerConst(STRS485TASK *pstRS485Task,unsigned char *pucChID,
//								   unsigned short *pusMaxRecvOverTime,unsigned short *pusMaxByteOverTime,
//								   unsigned short *pusMaxSendDelay);
//
////定时15分钟数据抄读
//unsigned char Meter_Protocol_GetTimerCommand(STCMDFLAG	stCmdFlag,STCMDFLAG stIsSendFlag,STRS485TASKCMDLIST	*pstRS485TaskCmdList);
//unsigned char Meter_GetReSendContent(STRS485TASK *pstRS485SendTask,STRS485TASK *pstRS485RecvTask,STRS485TASK *pstRS485RetTask);
//unsigned char Meter_Protocol_DispartOverTime(STRS485TASK *pstRS485SendTask,STRS485TASK *pstRS485RecvTask,unsigned char *pucExplainResultNum);
//unsigned char Meter_Protocol_GetPortPara(STRS485TASK *pstRS485Task,unsigned long *pulBaudRate,unsigned char *pucEPN,unsigned char *pucDataBit,unsigned char *pucStopBit);
//unsigned char Get_ChID(unsigned char ucPortNo);
__EXTERN unsigned char Get_MeterNo_byMPNo(unsigned char ucMPNo);
#endif

/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : public.h
 Description    : 通用数据结构定义
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/
#ifndef _PUBLIC_H_
#define _PUBLIC_H_

#include "math.h"
#include "float.h"
#include "errno.h"
#include "limits.h"
#include "string.h"

//azh  140918
#define	CONINVALIDVAL						0xBBBBBBBB
//#define	CONINVALIDVAL						0xFFFFFFFF
#define CONINVALIDSTA						0xFFFF
#define CONINVALIDBYTE						0xff

#ifndef NULL
#define NULL ( ( void* )0 )
#endif

typedef enum
{
	RET_SUCCESS=0,
	RET_ERROR,
	RET_FILL_SAME,				//内容相同，不再填写
	RET_MEM_OVER,				//内存溢出

	RET_TOO_LITTLE,				//未到最小长度
	RET_HEAD_ERROR,				//帧头错误
	RET_MID_END,				//中途结束
	RET_NOENOUGH_LONG,			//长度未到

	RET_GRTASK_NEEDPROC,		//GR任务队列中有任务要执行
	RET_GRTASK_NONEEDPROC,		//GR任务队列中无任务要执行

	RET_OVERTIME,
	RET_RS485_TASKSOURCE_FULL,
	RET_RS485_TASKSOURCE_NULL,
	RET_TASK_EMPTY,				//任务为空
	RET_TASK_NOEMPTY,			//任务为非空
	RET_SPEC,
	RET_NEEDPROC,				//需要处理
	RET_NEEDRESTORE,			//需要数据恢复
	RET_DATA_AVAILABLE,			//数据有效
	RET_NEED_JUDGE,				//需要预判
	RET_NONEED_JUDGE,			//不需要预判

	RET_PASS_ERR,				//密码错误
	RET_SEQ_ERR,				//SEQ错误
	RET_ADDR_ERR,				//地址错误
	RET_CTRLB_ERR,				//控制字错误
	RET_LEN_ERR,				//长度错误
	RET_REP_ERR,				//报文类型错误

	RET_ENSURE_FINISH,			//确诊结束
	RET_LCD_BUSY,				//LCD_Busy
	
	RET_ALR_HAPPEN,				//告警发生
	RET_ALR_RECOVER,			//告警恢复

	RET_MCM_NONEEDSEND,			//无须上送了(需要清除 CommInfoList)
	RET_MCM_WAITSEND,			//等待上送,(不可清除 CommInfoList)
	RET_NEEDCLRGROUP,			//需清除相同任务来源以及表号的任务
	RET_COMM_TIMEOVER,			//通信超时次数到
	RET_CHANGEPORTPARA,			//需要改变端口参数

	RET_CONNECT_OK				//收到完成CONNECT
}ENURETVALUE;

#define		METER_NORMAL_FLAG		00	//用以规约解释，表示是否超时
#define		METER_OVERTIME_FLAG		11
#define		METER_MIDEND_FLAG		22


//azh 2个测量点 2个曲线任务空间相关宏定义
#define FLASHSECTOR_SIZE		        0x1000  //4K
#define MCP_FLASH_BLOCK_SIZE	        FLASHSECTOR_SIZE//0x20000
#define SECTORNUM_PERTASK               64      //4k*64=256K 每个曲线任务所占sector数目
//日月及抄表日数据相关宏定义
//数据总和限制800k每份数据为0.85k，总计940份，对于8个点，每块限制110份
//数据顺序日冻结，日数据，月数据固定
//#define CON_DFPAGE_HISDATA_PAGENUM		96		//每块96页，固定48k，限制56份（相当于6个+32日+13月）
#define CON_DAYDATA_BLOCKNUM_PER_MP     9//(4K/850)*9=36点
#define CON_MONTHDATA_BLOCKNUM_PER_MP   4//(4K/850)*4=16点
#define CON_FROZEDATA_BLOCKNUM_PER_MP   3//(4K/850)*3=12点
#define CON_HISDATA_BLOCKNUM_PERMP      (CON_DAYDATA_BLOCKNUM_PER_MP+CON_MONTHDATA_BLOCKNUM_PER_MP+CON_FROZEDATA_BLOCKNUM_PER_MP)//(4K/850)*3=12点

//改成 EN25Q64 2048*4k OR 128*64K
//--------------------------
#define CON_DFPAGE_HEAD_RESERVE1		0		//限制8K，页0-1
//曲线任务数据保存空间
#define MCP_FLASH_TASK_START_BLOCK	    2       //4K*64*2(CON_GRTASK_NUM) 页2-129
#define CON_TASK_BLOCK_TOTNUM           (SECTORNUM_PERTASK*8)//azh CON_GRTASK_NUM=2 这里是8预留6个 测量点不能随便改动
//重要告警记录保存空间
#define CON_IMPALARM_START_BLOCK		(MCP_FLASH_TASK_START_BLOCK+CON_TASK_BLOCK_TOTNUM)//130//限制160K，页130-169		告警日志
#define CON_IMPALARM_BLOCK_TOTNUM       10//因为告警记录计数器一个字节256条 而由于flash块擦除的特性是变动的 4k/330=12*10=120条告警记录
#define CON_IMPALARM_BLOCK_RESVER       20//保留20
//一般告警记录保存空间
#define CON_NORMALARM_START_BLOCK		(CON_IMPALARM_START_BLOCK+CON_IMPALARM_BLOCK_TOTNUM+CON_IMPALARM_BLOCK_RESVER)//170//限制160K，页170-209		告警日志
#define CON_NORMALARM_BLOCK_TOTNUM      10
#define CON_NORMALARM_BLOCK_RESVER      20//保留20
//日月及抄表日数据保存空间
#define CON_DFPAGE_HEAD_HISDATA			(CON_NORMALARM_START_BLOCK+CON_NORMALARM_BLOCK_TOTNUM+CON_NORMALARM_BLOCK_RESVER)//210//限制800K，页210-409		历史数据月数据，日数据
#define CON_HISDATA_BLOCK_TOTNUM        (CON_HISDATA_BLOCKNUM_PERMP*8)//CON_GRTASK_NUM=2 这里是8预留6个
//azh 目前用到0~722
#define MCP_FLASH_HEAD_TAIL			    2048	    //4K*2048=8M bytes 最大值，不能达到该值*/

//4M片子两片，第一片0-8191，第二片8192-16383
//DATAFLASH数据区划分块首页编号，总共16384页
//#define CON_DFPAGE_HEAD_RESERVE1		0		//限制16K，页48-127
////#define CON_DFPAGE_HEAD_PROGRAM		128		//限制960K，页128-2047
//#define CON_DFPAGE_HEAD_DATASTART		0//2048	//程序下装后面的数据区
//#define CON_DFPAGE_HEAD_ALARM_IMP		1//2048	//限制150K，页2048-2347		告警日志
//#define CON_DFPAGE_HEAD_ALARM_NORM		301//2348	//限制150K，页2348-2647		告警日志
//#define CON_DFPAGE_HEAD_HISDATA			601//2648	//限制800K，页2648-4247		历史数据月数据，日数据
//#define CON_DFPAGE_HEAD_RESERVE2		2201//4248	//页4248-8191
//#define CON_DFPAGE_HEAD_2ND_START		8192	//第二片起始编号，连续编号
//#define CON_DFPAGE_HEAD_DATABLOCK		8224	//限制3456K，页8224-15135	18个192k数据空间的任务
//#define CON_DFPAGE_HEAD_RESERVE3		15136	//页15136-16383
//
//#define CON_DFPAGE_HEAD_MAX				8192	//单片最大值
//#define CON_DFPAGE_HEAD_TOTAL_MAX		16384	//最大值
//
//#define CON_DFPAGE_DATABLOCK			0x30000	//字节数目192k
//#define CON_DFPAGE_DATABLOCK_NUM		18
//#define CON_DFPAGE_DATABLOCK_PAGENUM	384		//每块384页，固定不可变

//终端自检出错代码
#define	CON_TERERR0_RAM_INT			0x01	//1	片内RAM				//第0组
#define	CON_TERERR0_RAM_EXT			0x02	//2	片外RAM
#define	CON_TERERR0_DATAFLASH		0x04	//3	DATAFLASH
#define	CON_TERERR0_TIMER			0x08	//4	时钟异常
#define	CON_TERERR1_BATTER			0x01	//9	电池电压低			//第1组
#define	CON_TERERR2_ERR1			0x01	//17	异常1			//第2组
#define	CON_TERERR3_ERR1			0x01	//25	异常1			//第3组
#define	CON_TERERR4_ERR1			0x01	//33	异常1			//第4组
#define	CON_TERERR5_ERR1			0x01	//41	异常1			//第5组
#define	CON_TERERR6_ERR1			0x01	//49	异常1			//第6组
#define	CON_TERERR7_ERR1			0x01	//57	异常1			//第7组

/********************************公用函数*********************************/
#define CON_STATUS_YES_2BYTE		0x55AA	//两个字节标记：是
#define CON_STATUS_NO_2BYTE			0xAA55	//两个字节标记：否
#define CON_STATUS_YES				0x55	//字节标记：是
#define CON_STATUS_NO				0xAA	//字节标记：否
#define CON_MIN_FLOAT_VAL			0.1

#define CON_STATUS_INIT		        0xA1A1	//两个字节标记：初始状态
#define CON_STATUS_START		    0xA2A2	//两个字节标记：启动状态
#define CON_STATUS_FINISH		    0xA3A3	//两个字节标记：完成状态

/********************************时钟函数*********************************/
#define CON_TIMEUNIT_SEC			0x01	//秒
#define CON_TIMEUNIT_MIN			0x02	//分
#define CON_TIMEUNIT_HOUR			0x03	//时
#define CON_TIMEUNIT_DAY			0x04	//日
#define CON_TIMEUNIT_MON			0x05	//月

/********************************RS485TASK结构操作******************************/
#define		CONRS485BIGMAXLEN       255 //azh 用于存放中继命令的长度 因为有升级帧有200多字节
#define		CONRS485DATAMAXLEN		96	//任务数据区最大长度，需要保证整个命令结构是偶数个字节
#define		CONRS485DATAMINLEN		24	//由于在某些结构不考虑中继命令，因此可缩短
#define		CON_SEQNO_NULL			0

//报文类型
#define		CON_RS485_REPORT_TYPE_IDLE		0		//空报文
#define		CON_RS485_REPORT_TYPE_SET		1		//设置报文
#define		CON_RS485_REPORT_TYPE_GET		2		//查询报文
#define		CON_RS485_REPORT_TYPE_SPEC		3		//特殊报文
#define		CON_RS485_REPORT_TYPE_RELAY		4		//中继命令
#define		CON_RS485_REPORT_TYPE_EXT		5		//扩展报文

#define		CON_RS485_PORT_NUM				2
#define 		CON_RELAY_NUM						4
typedef struct _STRELAYADDR
{
	unsigned char ucRelayAddr[6];
}STRELAYADDR;
typedef struct _STRELAYTASKPARA			//中继任务参数
{
	unsigned char	ucPortBaudRate;					//波特率代码
	unsigned char	ucRecvWaitTime;					//中继任务接收超时时间(秒)
	unsigned char	ucByteWaitTime;					//中继任务字节超时时间(秒)
	unsigned char	ucCharacterChar;				//特征字符
	unsigned char	ucReserve;						//保留字符
	unsigned char	ucCommInfo;						//通讯特征字符
	unsigned short	usGetFrom;						//截取开始
	unsigned short	usGetLen;						//截取长度
	
	unsigned char ucFnVal;				//Fn
	unsigned char ucRelayNum;			//中继级数
	unsigned char ucDIType;
	STRELAYADDR stRelayAddr[CON_RELAY_NUM];	//中继地址
	STRELAYADDR stDestAddr;
	unsigned long ulDI;
}STRELAYTASKPARA;

typedef struct _STRS485TASK
{
	unsigned char	ucSeqNo;						//任务序号
	unsigned char	ucTaskSource;					//任务来源
	unsigned short	usDataType;						//数据类型
	unsigned char	ucReportType;					//报文类型
	unsigned char	ucDevNo;						//设备号
	unsigned char	ucDataLen;						//数据长度
	STRELAYTASKPARA stRelayPara;					//中继任务参数
	unsigned char   ucDataArea[CONRS485DATAMAXLEN];	//数据区内容
}STRS485TASK;		//data_size+20=116 bytes

typedef struct _STRS485TASKNOR
{
	unsigned char	ucSeqNo;						//任务序号
	unsigned char	ucTaskSource;					//任务来源
	unsigned short	usDataType;						//数据类型
	unsigned char	ucReportType;					//报文类型
	unsigned char	ucDevNo;						//设备号
	unsigned char	ucDataLen;						//数据长度
	unsigned char   ucDataArea[CONRS485DATAMINLEN];	//数据区内容
}STRS485TASKNOR;		//data_size+20=116 bytes

typedef struct _STRS485BIG//空间大一点的
{
	unsigned char	ucSeqNo;						//任务序号
	unsigned char	ucTaskSource;					//任务来源
	unsigned short	usDataType;						//数据类型
	unsigned char	ucReportType;					//报文类型
	unsigned char	ucDevNo;						//设备号
	unsigned char	ucDataLen;						//数据长度
	unsigned char   ucDataArea[CONRS485BIGMAXLEN];	//数据区内容
}STRS485BIG;		//

typedef struct _STRELAYDATA//空间大一点的
{
	unsigned short	usDataLen;						//数据长度
	unsigned char   ucDataArea[CONRS485BIGMAXLEN];	//数据区内容
}STRELAYDATA;
/********************************TerMsg结构操作******************************/
#define	CON_MAX_MPNUMDIV8			8
#define	CON_DI_DATA_LEN				96			//数据项的最大数据内容，中继可以占用8个数据项，16*8个数据项数目=128字节
#define	CON_DI_DATA_LEN_ALR			24//48//14//24//24//48			//告警记录
#define	CON_TERMSG_DI_NUM			8			//一条消息的最大数据项数目
#define	CON_INFOMSG_DI_NUM			4			//信息项最大数目

//azh 注意不能与RS485的任务来源冲突!!!!!
//消息来源：
#define	CON_TERMSG_SOUR_ZIGBEE			9		//azh 171017来源于zigbee电子式高压表GPRS模块
#define	CON_TERMSG_SOUR_RS232_0			10		//azh 来源于RS485_2
#define	CON_TERMSG_SOUR_RS232			11		//来源于RS232
#define	CON_TERMSG_SOUR_RS485			12		//来源于RS485
#define CON_TERMSG_SOUR_TERSELF			13	
#define CON_TERMSG_SOUR_CSD				14		
#define CON_TERMSG_SOUR_IRDA			15	
#define CON_TERMSG_SOUR_AUD				16	
#define CON_TERMSG_SOUR_RCOMM_SMS		17		//短信
#define CON_TERMSG_SOUR_RCOMM_TCP		18		//TCP
#define CON_TERMSG_SOUR_RCOMM_UDP		19		//UDP

#define CON_TERMSG_SOUR_RCOMM_1			20
#define CON_TERMSG_SOUR_RCOMM_2			21
#define CON_TERMSG_SOUR_RCOMM_3			22


#define	CON_TERMSG_SOUR_INVALID			0xFF
/*	注意不能与RS485的任务来源冲突
#define		CONRS485SOURTIMER				6		//来源于默认定时读取
#define		CONRS485SOUR_GRTASK				9		//来源GRTASK
#define		CONRS485SOUR_GRTASK_UNREAD		10		//来源GRTASK，但无需抄表，将历史值直接填写入任务回应区
#define		CONRS485SOUR_ALRAFFIRMTASK		40		//来源于报警的确认	
#define		CONRS485SOURALR_1				50		//来源于报警任务的补召
*/
//RS485的任务来源 
#define		CONRS485SOURTIMER				6		//来源于默认定时读取
#define		CONRS485SOUR_GRTASK				7//9		//来源GRTASK
#define		CONRS485SOUR_GRTASK_UNREAD		8//10		//来源GRTASK，但无需抄表，将历史值直接填写入任务回应区
//		11-29保留为TERMSG
#define		CONRS485SOUR_ALRAFFIRMTASK		40		//来源于报警的确认	
#define		CONRS485SOURALR_1				50		//来源于报警任务的补召

//azh temp
#define CON_RS485CACHE_ASKOPE	        60
#define CON_RS485CACHE_RELAY	        61
#define CON_TER_SYNFLASH	            62
#define CON_RS485_645RELAY				63//外部端口直接645命令透传到高压表抄数据
#define CON_RELAYSOUR_RELAYTASK		    64	//中继任务

//#define CON_RS485ASKOPESOUR_TIMER		0x30	//定时
#define CON_RS485ASKOPESOUR_TASK		65	//任务
#define CON_RS485ASKOPESOUR_ALARMUP		66	//报警上送数据抄读
#define CON_RS485ASKOPESOUR_ALARMFORE	67	//报警确诊发起的抄表

//azh

typedef struct _STDATAITEM						//数据项结构
{
	unsigned char	ucMPNo;		
	unsigned char	ucExeRet;
	unsigned short	usItemLabel;			//数据项标识
	unsigned short	usDataLen;				//数据长度
	unsigned char*	ucDataContent;//ucDataContent[CON_DI_DATA_LEN];	//数据内容
}STDATAITEM;

typedef struct _STDATAITEM_ALR					//告警数据项结构
{
	unsigned char	ucMPNo;		
	unsigned char	ucExeRet;
	unsigned short	usItemLabel;			//数据项标识
	unsigned short	usDataLen;				//数据长度
	unsigned char	ucDataContent[CON_DI_DATA_LEN_ALR];	//数据内容
}STDATAITEM_ALR;

typedef struct _STTERMSG						//终端消息结构
{
	unsigned char	ucSeqNo;				//通讯信息队列编号
	unsigned char	ucMsgSource;			//消息来源
	unsigned char	ucMsgType;				//消息类型
//	unsigned char	ucMsgObject;			//消息执行的对象,测量点或任务号
	unsigned char	ucInfoItemNum;			//信息项个数
	STDATAITEM		stInfoItem[CON_INFOMSG_DI_NUM];	//信息项
	unsigned char	ucDataItemNum;			//数据项数量
	STDATAITEM		stDataItem[CON_TERMSG_DI_NUM];	//数据项
}STTERMSG;


typedef struct _STUSERIP						//终端以太网IP地址
{
	unsigned char uciIP[20];
	unsigned char ucipusernamelen;
	unsigned char ucipusername[20];
	unsigned char ucipuserpasswordlen;
	unsigned char ucipuserpassword[20];
	unsigned char ucip_net[2];
}STUSERIP;


typedef struct _STSWITCHDELAY
{
	unsigned char	ucJumpDelay;
	unsigned short  usJumpInter;
}STSWITCHDELAY;
//-----------------------------------------------------------------------

__EXTERN unsigned char gCurrent_FlashBlock_Valib_Flag;
__EXTERN unsigned char gsave_use;
__EXTERN unsigned char ucsave_flag;

__EXTERN unsigned char	ucPulseTimerMonFlag;
__EXTERN unsigned char	ucPulseTimer1MinFlag;

__EXTERN unsigned char			ucTimeChange[CONSWITCHCURVENUM];
__EXTERN unsigned short			usCtrlLockTimer;
__EXTERN unsigned char			g_ucEnergyCtrl_SumGNo;						//电量控制总加组编号
__EXTERN unsigned char			g_ucPowerCtrl_SumGNo;						//功率控制总加组编号

__EXTERN STDATETIME 				g_stAutoKeepPower;
__EXTERN STDATETIME 				g_stKeepPowerEnd;

//__EXTERN STPOWERCTRLCOUNT	stPowerCtrlCount;
__EXTERN unsigned char		ucPowLowerCtrlTime;								//0.5h为单位，下浮控的控制时间
__EXTERN unsigned short		usPowLowerJumpAlrTime[4];						//下浮控的告警时间，固定4个轮次，实际只有两轮，单位0.5秒

__EXTERN unsigned char	ucMonChangeFlag;
__EXTERN unsigned char  ucMinChangeFlag;
__EXTERN unsigned char	ucAlrHour;
__EXTERN unsigned char	ucCurrEnerCtrlTurn;
__EXTERN unsigned char	ucgEnerCtrlAlrCount;

__EXTERN STDATETIME	stPowerOffTime;

__EXTERN STDATETIME g_stTime_DayFrozenData;
__EXTERN STDATETIME g_stTime_DayFrozenCopy;
__EXTERN STDATETIME g_stTime_DayData;
__EXTERN STDATETIME g_stTime_MonthData;

//__EXTERN STDATETIME	gstCurrDateTime;

__EXTERN unsigned char g_ucDownloadType;					//下装程序或者参数
__EXTERN unsigned char g_ucTerDP_State;					//终端程序下装状态
__EXTERN unsigned char g_ucDPEnd_RestartTime;			//中止程序下装等待复位时间－－15s
__EXTERN unsigned char g_ucRAMReadTimes;					//RAM读访问次数
__EXTERN unsigned short g_usRAMWriteLifeCycle;			//RAM写允许

__EXTERN unsigned char g_ucVerNo;
__EXTERN unsigned char g_ucParaCS;

__EXTERN STTERRUNPARA			stTerRunPara;
__EXTERN STTERTIMERVAL			stTerTimerVal;

//azh__EXTERN STADEDEMAND			stAdeDemand;			//最大需量
//__EXTERN STADEMETERENERGYVAR stAdeMeterEnergyVar;

//__EXTERN STRATEPERIOD		stRatePeriod[12];		//一年最多分为12个负荷区
//__EXTERN STRATESET			stRateSet[5];			//一共最多有5套费率

//__EXTERN STSWITCHDELAY			stSwitchDelay[CON_SWITCHOUT_NUM];
//__EXTERN STSWITCHALARMINFO 		stSwitchAlarmInfo[CON_SWITCHOUT_NUM];

//__EXTERN STHISSUMVALGROUP stHisSumGPowTotal_Sta;
//__EXTERN STHISSUMVALGROUP stHisSumGPowFee1_Sta;
//__EXTERN STHISSUMVALGROUP stHisSumGPowFee2_Sta;
//__EXTERN STHISSUMVALGROUP stHisSumGPowFee3_Sta;
//__EXTERN STHISSUMVALGROUP stHisSumGPowFee4_Sta;

//__EXTERN STDATETIME			stPowerCtrl_RecoverTime[CONSWITCHCURVENUM];		//不越限情况下，控制恢复的时间，惩罚性，一般为时段结束时间

//__EXTERN STREMOTECTRLCOUNT stRemoteCtrlCount[CON_SWITCHOUT_NUM];
//__EXTERN STREMOTECTRLPARA  stRemoteCtrlPara[CON_SWITCHOUT_NUM];

//__EXTERN unsigned short	usPulseCountTotal[CON_PULSE_NUM];			//脉冲计数
//__EXTERN unsigned short	usPulseCountFee1[CON_PULSE_NUM];			//脉冲计数费率1
//__EXTERN unsigned short	usPulseCountFee2[CON_PULSE_NUM];			//脉冲计数费率2
//__EXTERN unsigned short	usPulseCountFee3[CON_PULSE_NUM];			//脉冲计数费率3
//__EXTERN unsigned short	usPulseCountFee4[CON_PULSE_NUM];			//脉冲计数费率4
//
//__EXTERN unsigned short	usPulseCountFeeT_Bak[CON_PULSE_NUM];		//脉冲计数费率1
//__EXTERN unsigned short	usPulseCountFee1_Bak[CON_PULSE_NUM];		//脉冲计数费率1STPULSEAUXINFO	stPulseAuxInfo[CON_PULSE_NUM];				//脉冲测量辅助信息
//__EXTERN unsigned short	usPulseCountFee2_Bak[CON_PULSE_NUM];		//脉冲计数费率2STPULSEMETERVAL stPulseMeterVal[CON_PULSE_NUM];				//脉冲电量数据
//__EXTERN unsigned short	usPulseCountFee3_Bak[CON_PULSE_NUM];		//脉冲计数费率3STPULSEMETERBASE stPulseMeterBase[CON_PULSE_NUM];			//脉冲底度
//__EXTERN unsigned short	usPulseCountFee4_Bak[CON_PULSE_NUM];		//脉冲计数费率4

//__EXTERN STPULSEAUXINFO	stPulseAuxInfo[CON_PULSE_NUM];				//脉冲测量辅助信息
//__EXTERN STPULSEMETERVAL stPulseMeterVal[CON_PULSE_NUM];				//脉冲电量数据
//__EXTERN STPULSEMETERBASE stPulseMeterBase[CON_PULSE_NUM];			//脉冲底度

//__EXTERN STPULSEMETERVAL_LM stPulseMeterVal_LM[CON_PULSE_NUM];		//脉冲电量数据上月

__EXTERN STHISRUNDATAINDEX 	stHisRunDataIndex[CON_METER_NUM];
__EXTERN STHISRUNDATA		stHisDayFrozenData[CON_METER_NUM];						//表数据
__EXTERN STHISRUNDATA		stHisDayData[CON_METER_NUM];
__EXTERN STHISRUNDATA		stHisMonthData[CON_METER_NUM];

//__EXTERN STHISSUMGROUPALLDATA	stHisSumGroupThisDay;							//总加	//以下三块数据需要转入DATAFLASH
//__EXTERN STHISSUMGROUPALLDATA	stHisSumGroupThisMonth;
//__EXTERN STHISSUMGROUPDATAINDEX	stHisSumGroupDataIndex[CON_SUMGROUP_NUM];

__EXTERN STRS485TASK		    stRS485RecvExpainVal;		//接收解释结果

//azh 不带计量 且高压表占用了一个测量点
//__EXTERN STHISTERDATAINDEX		stHisTerDataIndex;									//终端
//__EXTERN STHISRUNDATA			stHisTerDayFrozenData;
//__EXTERN STHISRUNDATA			stHisTerDataThisDay;
//__EXTERN STHISRUNDATA			stHisTerDataThisMonth;

//__EXTERN STHISPULSEDATAINDEX		stHisPulseDataIndex[CON_PULSE_NUM];					//脉冲
//__EXTERN STHISPULSEALLDATA		stHisPulseDayDataThisDay;
//__EXTERN STHISPULSEALLDATA		stHisPulseDayDataThisMonth;

//ADD by cfh 10-01-15	当前告警状态，需要保存的

//__EXTERN STSUMALARMSTAT		stSumAlarmStatus[CON_SUMGROUP_NUM];			//当前告警状态信息列表（总加组）

//__EXTERN STTERRUNPARA_NOUSE	stTerRunPara_Nouse;

//-----------------------------------------------------------------------
__EXTERN unsigned long	BCD_TO_HEX_1(unsigned char *pucBCD);
__EXTERN unsigned long	BCD_TO_HEX_2(unsigned char *pucBCD);
__EXTERN unsigned long	BCD_TO_HEX_3(unsigned char *pucBCD);
__EXTERN unsigned long	BCD_TO_HEX_4(unsigned char *pucBCD);
__EXTERN unsigned long	HEX_TO_BCD(unsigned long	lHex,unsigned char ucSize);
__EXTERN unsigned char	HEXCHAR_TO_BCDSTR(unsigned char ucHex,unsigned char *pstr);
__EXTERN unsigned char	HEXSHORT_TO_BCDSTR(unsigned short usHex,unsigned char *pstr);
__EXTERN unsigned char	Check_BCD_Data(unsigned char ucData);
__EXTERN unsigned char Check_BCD_Data_Buffer(unsigned char *pucData,unsigned char ucLen);
__EXTERN unsigned char	ClearDateTime(STDATETIME *pstDateTime);
__EXTERN unsigned char	FillData_RealEE(unsigned char *pucData,unsigned char ucLen);
__EXTERN unsigned char	FillData_RealAllEE(unsigned char *pucData,unsigned char ucLen);
//__EXTERN void 			Call_NOP(void);
__EXTERN unsigned char ChkBitFlag(unsigned char *puc, unsigned short  nbit);
__EXTERN void SetBitFlag(unsigned char *puc, unsigned short n);

__EXTERN unsigned char	FillData_EE(unsigned char *pucData,unsigned char ucLen);
__EXTERN unsigned char	FillData_AllEE(unsigned char *pucData,unsigned char ucLen);
__EXTERN unsigned char	Is_Data_EE(unsigned char *pucData,unsigned char ucLen);
__EXTERN unsigned char	Is_Data_AllEE(unsigned char *pucData,unsigned char ucLen);
__EXTERN unsigned char	Get_Sum(unsigned char *pBuffer,unsigned long ulLen);
__EXTERN unsigned long	BYTE_TO_LONG(unsigned char *pucByte);
__EXTERN STDATETIME		BYTE_TO_DATETIME(unsigned char *pucByte);
__EXTERN unsigned long	makelabs(long lval);
__EXTERN unsigned char   Is_In_Buffer(unsigned char *pBuffer,unsigned char ucLen,unsigned char ucVal);
__EXTERN unsigned long	GetPQRate(unsigned long lPower,unsigned long lVar);	//得到功率因数值，格式xxx.xx
__EXTERN unsigned long	GetPQRate_Var4B(unsigned long lPower,unsigned long lVar);	//得到功率因数值，格式xxx.xx
__EXTERN unsigned long	GetSPow(unsigned long lPower,unsigned long lVar);	//得到视在功率
__EXTERN unsigned long	Adjust_Zero(unsigned long ulVal);
__EXTERN unsigned long	Get_Correct_Boadrate(unsigned long ulBoadrate);

__EXTERN unsigned char	ClearDateTime(STDATETIME *pstDateTime);
__EXTERN unsigned long BCD_TO_HEX_1(unsigned char *pucBCD);
__EXTERN unsigned char Check_BCD_MuiltData(unsigned char *pucData,unsigned char ucDataNum);
__EXTERN unsigned char CheckDateTime(STDATETIME *pstDateTime);
__EXTERN unsigned char ConvertHisVal0(unsigned long ulVal,unsigned char *pucData,unsigned char ucLen);
__EXTERN unsigned long GetPQRate_Var4B(unsigned long lPower,unsigned long lVar);	//得到功率因数值，格式xxx.xx
__EXTERN unsigned long GetPQRate_Var4B_3P(unsigned long lPower,unsigned long lVar);	//得到功率因数值，格式x.xxx
__EXTERN unsigned long	Adjust_Zero(unsigned long ulVal);
__EXTERN unsigned long GetSPow(unsigned long lPower,unsigned long lVar);	//得到视在功率
__EXTERN STDATETIME	BYTE_TO_DATETIME(unsigned char *pucByte);
__EXTERN unsigned long	HEX_TO_BCD(unsigned long	lHex,unsigned char ucSize);
__EXTERN unsigned long BCD_TO_HEX_2(unsigned char *pucBCD);
__EXTERN unsigned long BCD_TO_HEX_4(unsigned char *pucBCD);
__EXTERN unsigned char Get_Sum(unsigned char *pBuffer,unsigned long ulLen);
__EXTERN unsigned long BCD_TO_HEX_3(unsigned char *pucBCD);
__EXTERN unsigned char FillData_EE(unsigned char *pucData,unsigned char ucLen);
#endif//_PUBLIC_H_

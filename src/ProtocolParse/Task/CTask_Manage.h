/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : CTask_Manage.c
 Description    : 曲线数据模块任务管理
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-08-15    azh     the original version
******************************************************************************/
#ifndef _CTASK_MANAGE_H
#define _CTASK_MANAGE_H

#define CON_TASK_FRAME_RESEND_MAX	3	    //任务数据命令帧最多重发次数

#define CON_TASK_RETDATA_VALID		0x55
#define CON_TASK_RETDATA_INVALID	0xAA
#define CON_TIME_TASK_WAIT_PROCESS  0xA1//定时任务待处理
#define CON_TASK_RETDATA_PROCESS   	0xA5//AZH 数据标识正在抄读中没有完成


//极大值常量定义
#define CON_GRTASK_NUM				2//18		//最多18个任务，数据块划分所限制
#define CON_GRTASK_METER_TASKNUM	1			//表个数8   //表的曲线任务从0~(表个数-1)
#define CON_GRTASK_TER_TASKNO		CON_GRTASK_METER_TASKNUM+0		//终端交流采集任务 从上面排下来


#define	CON_GRTASK_STATFLAG_NUM		32		//任务状态标记字节数

//-----------------------------------------------------------
//azh 曲线任务参数保存
#define CON_TASK_PARA_SINGLE_SAVE   0
#define CON_TASK_PARA_BLOCK_SAVE    0xA5
#define CON_TASK_CURVE_DI_NUM       22        //曲线任务要抄的数据标识

#define CON_TASK_FRAME_RESEND_MAX	3	    //任务数据命令帧最多重发次数

#define CON_GRTASK_STATE_VALID		0x11	//检查有效任务时用的宏 不一定是激活状态
#define CON_GRTASK_STATE_ACTIVE		0x22	//检查有效任务时用的宏 并且激活状态

#define CON_TASK_CONFIG_ACTIVESTATE             0//前后状态不同 才会更新及处理下次时间
#define CON_TASK_RESTART_ACTIVESTATE            1//任务重启 直接更新及处理下次时间

#define CON_GRTASK_PARA_VALID           0xa55a
//#define CON_GRTASK_PARA_BASE_ADDR		0x61005400  //temp
//#define CON_GRTASK_PARA_BASE_BOLCK		0x1000      //128*32可保存32个任务参数
#define CON_GRTASK_PER_SPACE    		128         //每个任务块参数保存的空间大小
//---
#define CON_TASK_BLK_OFST               0
#define CON_TASK_TYPE_OFST              CON_TASK_BLK_OFST//ucTaskType//任务类型
#define CON_TASK_TYPE_LENTH             1            
#define CON_TASK_MPNO_OFST              (CON_TASK_TYPE_OFST+CON_TASK_TYPE_LENTH)//ucMPNo普通任务的测量点号&中继任务的端口号
#define CON_TASK_MPNO_LENTH             1          
#define CON_TASK_SAVENUM_OFST           (CON_TASK_MPNO_OFST+CON_TASK_MPNO_LENTH)//usSavePointNum任务的保存点数(不使用设定值，使用自身计算得的值)
#define CON_TASK_SAVENUM_LENTH          2                                          
#define CON_TASK_PERRETLEN_OFST         (CON_TASK_SAVENUM_OFST+CON_TASK_SAVENUM_LENTH)//usPerTaskRetMem任务执行一次所虚的字节数
#define CON_TASK_PERRETLEN_LENTH        2
#define CON_TASK_ACTIVE_OFST            (CON_TASK_PERRETLEN_OFST+CON_TASK_PERRETLEN_LENTH)//ucActive//任务状态：激活
#define CON_TASK_ACTIVE_LENTH           1          
#define CON_TASK_STARTACTIVE_OFST       (CON_TASK_ACTIVE_OFST+CON_TASK_ACTIVE_LENTH)//stStartActive//任务开始活动时间
#define CON_TASK_STARTACTIVE_LENTH      6          
#define CON_TASK_BLKCRC_LENTH           (13+1)//1crc
//---
#define CON_TASKIF_BLK_OFST             (CON_TASK_BLK_OFST+CON_TASK_BLKCRC_LENTH)
#define CON_TASKIF_EXEDNUM_OFST         CON_TASKIF_BLK_OFST//usExedNum//任务执行了次数
#define CON_TASKIF_EXEDNUM_LENTH        2          
#define CON_TASKIF_TAILPOS_OFST         (CON_TASKIF_EXEDNUM_OFST+CON_TASKIF_EXEDNUM_LENTH)//usTailPos//最早的数据点，数据区循环存储usSavePointNum个点
#define CON_TASKIF_TAILPOS_LENTH        2        
#define CON_TASKIF_TAILTIME_OFST        (CON_TASKIF_TAILPOS_OFST+CON_TASKIF_TAILPOS_LENTH)//ulTailTime//最早的数据点对应的时间，从1990年开始，单位：秒
#define CON_TASKIF_TAILTIME_LENTH       4          
#define CON_TASKIF_BLKCRC_LENTH         (8+1)//1crc
//---
#define CON_TASKRIF_BLK_OFST            (CON_TASKIF_BLK_OFST+CON_TASKIF_BLKCRC_LENTH)
#define CON_TASKRIF_NEXTEXTIME_OFST     CON_TASKRIF_BLK_OFST//stNextExeTime//下次执行时间(任务)
#define CON_TASKRIF_NEXTEXTIME_LENTH    6
#define CON_TASKRIF_NEXTUPTIME_OFST     (CON_TASKRIF_NEXTEXTIME_OFST+CON_TASKRIF_NEXTEXTIME_LENTH)//stNextUpTime//下次数据上行时间
#define CON_TASKRIF_NEXTUPTIME_LENTH    6
#define CON_TASKRIF_HEADPOS_OFST        (CON_TASKRIF_NEXTUPTIME_OFST+CON_TASKRIF_NEXTUPTIME_LENTH)//usHeadPos//当前存储的点号
#define CON_TASKRIF_HEADPOS_LENTH       2
#define CON_TASKRIF_BLKCRC_LENTH        (14+1)//1crc
//
#define CON_TASKINFO_BLKADDR_END        (CON_TASKRIF_HEADPOS_OFST+CON_TASKRIF_HEADPOS_LENTH)    //整个任务运行参数块末地址

//#define CON_TASK_BEGINPAGE_OFST       (CON_GRTASK_NEXTUPTIME_OFST+CON_GRTASK_NEXTUPTIME_LENTH)//任务起始页
//#define CON_TASK_BEGINPAGE_LENTH      2    
//#define CON_TASK_BEGINPOS_OFST        (CON_GRTASK_BEGINPAGE_OFST+CON_GRTASK_BEGINPAGE_LENTH)//任务起始页地址
//#define CON_TASK_BEGINPOS_LENTH       2   
//#define CON_TASK_BEGIN_BLKCRC_LENTH   (4+1)//1crc 

//默认电表任务号 用于生成测量点的曲线任务 
//-----------------------------------------------------------------------------------
#define	CON_GRTASK_DINUM			65//32		//数据项个数
#define CON_GRTASK_DATANUM			64		//数据项字节数

#define CON_GRTASK_DATA_INVALID		0xEE	//无效数据
#define CON_DATAFLSH_INIT_DATA		0xFF	//未擦除状态

//任务类型
#define CON_GRTASK_TYPE_NORMAL		0x01	//普通任务
#define CON_GRTASK_TYPE_RELAY		0x02	//中继任务
#define CON_GRTASK_TYPE_INVALID		0xFF	//无效任务
//azh
#define CON_GRTASK_TaskDateInfoLen		6	//一个任务数据信息预留的存储空间，现在是任务执行时间
#define CON_GRTASK_TaskDateRELAYLen		96	//中继任务的任务数据存储空间(任务信息不包括)
#define CON_GRTASK_TaskDateMaxDateLen	512	//一个任务执行一次最大的存储空间

//任务分为
//	表8块占用8个任务
//	总加占用4个任务
//	脉冲4路占用4个任务
//	终端采集占用1个任务
//	其他剩余1个任务
//	合计18个任务，每个任务数据区固定192k，共3456k
//#define CON_GRTASK_METER_TASKNUM				1			//表个数8
//#define CON_GRTASK_SUMGROUP_INSTANTPOW_TASKNO	CON_GRTASK_METER_TASKNUM+0		//有功功率总加任务
//#define CON_GRTASK_SUMGROUP_INSTANTVAR_TASKNO	CON_GRTASK_METER_TASKNUM+1		//无功功率总加任务
//#define CON_GRTASK_SUMGROUP_POW_TASKNO			CON_GRTASK_METER_TASKNUM+2		//有功电量总加任务
//#define CON_GRTASK_SUMGROUP_VAR_TASKNO			CON_GRTASK_METER_TASKNUM+3		//无功电量总加任务

//#define CON_GRTASK_PULSE_1_TASKNO			CON_GRTASK_METER_TASKNUM+4		//脉冲1任务
//#define CON_GRTASK_PULSE_2_TASKNO			CON_GRTASK_METER_TASKNUM+5		//脉冲2任务
//#define CON_GRTASK_PULSE_3_TASKNO			CON_GRTASK_METER_TASKNUM+6		//脉冲2任务
//#define CON_GRTASK_PULSE_4_TASKNO			CON_GRTASK_METER_TASKNUM+7		//脉冲2任务
//#define CON_GRTASK_TER_TASKNO				CON_GRTASK_METER_TASKNUM+0		//终端交流采集任务
//#define CON_GRTASK_USERDEF_START_TASKNO		CON_GRTASK_METER_TASKNUM+9		//用户可自定义任务

#define CON_GRTASK_DATAPOINT_NUM			3072		//32天15分钟一点，共3072点，每点64字节
#define CON_GRTASK_DATA_TOTAL_SECONDS		5529600//2764800		//32天2764800秒
//#define CON_GRTASK_DATAPOINT_NUM			1152		//12天15分钟一点，共1152点，每点48字节
//#define CON_GRTASK_DATA_TOTAL_SECONDS		1036800		//12天1036800秒

#define CON_MP_NUM				2			//终端测量点个数，请不要更改此常数		//临时在这里
#define CON_MP_INVALID			0			//无效测量点
#define CON_MP_VALID			1			//有效测量点	

#define CON_METERCOPY_CHECK_TIME            3000//5分钟 单位ms

typedef struct _STGRTASKEXEINFO
{
	unsigned char ucNowExeTaskNo;	//当前正执行的任务号
	unsigned char ucTaskValidFlag;	//任务返回数据有效标记
//azh
    unsigned char ucTaskType;       //当前任务类型
    unsigned char unInCom;          //中继任务的或者测量点所在的内部端口号
    unsigned short usValidCnt;      //任务(定时抄表 曲线任务 告警抄表)有效时间计数 对ucTaskValidFlag效时间的监控
}STGRTASKEXEINFO;

typedef struct _STSENDDILIST
{
	unsigned char ucDINum;
	unsigned short usDILabel[CON_GRTASK_DINUM];
//azh
	unsigned char	ucDIPtr;        //数据项数
    unsigned char   ucReSendnum;    //每个数据标识重抄次数
}STSENDDILIST;

typedef struct _STTIMEINFO
{
	unsigned char	ucTimeInter;			//时间间隔
	unsigned char	ucTimeUnit;				//时间单位
}STTIMEINFO;

typedef struct _STDIVAL
{
	unsigned short	usDI;					//数据项标识
	unsigned char	ucDILen;				//数据项数据内容长度
//	unsigned char	ucDIVAL[96];			//数据项数据内容
}STDIVAL;

typedef struct _STGRTASKRETVAL
{
	unsigned char	ucTaskNo;				//任务序号
	STDATETIME		stRetDateTime;			//数据回应的时间
	unsigned char	ucDINum;				//数据项数
	STDIVAL			stDIVal[CON_GRTASK_DINUM];		//对应任务号的数据项内容
//azh 保留一个所有任务数据的缓冲
	unsigned char   ucTaskData[CON_GRTASK_TaskDateMaxDateLen];
}STGRTASKRETVAL;

#define CON_GRTASK_ACTIVE			0x55
#define CON_GRTASK_NOACTIVE			0xAA

#define CON_TASK_RETDATA_SENDED		0x55		//已被上送
#define CON_TASK_RETDATA_NOSEND		0xAA		//尚未被上送

//#define	 CON_TASK_MAXBUFFER			256//8192		//调整将涉及程序下装，至少4k
//#define	 CON_BUFFER_CLASS2_DIFF		512
//#define	 CON_BUFFER_CLASS3_DIFF		1024
#define	 CON_TASK_DATA_MAXBUFFER	512//1024 现在只是一个曲线任务//azh 此处内存块为1536 减去一些帧前帧后 及串口前导字节

typedef struct _STGRTASK				//GR系统任务
{
	unsigned char	ucTaskType;						//任务类型
	unsigned char	ucMPNo;							//普通任务的测量点号&中继任务的端口号
	STTIMEINFO		stTITaskStart;					//采样开始基准时间
	STTIMEINFO		stTITaskInter;					//采样间隔时间
	STTIMEINFO		stTIResendStart;				//上送基准时间
	STTIMEINFO		stTIReSendInter;				//上送间隔时间
	unsigned char	ucResendDataInter;				//上送数据的频率		
	unsigned char	ucDataItemNum;					//普通任务的数据项数量&中继任务的中继命令长度
	unsigned short	usSavePointNum;					//任务的保存点数(不使用设定值，使用自身计算得的值)
	unsigned short	usExeNum;						//任务执行次数
//azh	STRELAYTASKPARA stRelayTaskPara;				//中继参数
//azh 为了节省ram 反正在曲线默认任务 抄读数据项的固定的 用表格就行
//	unsigned char	ucDataArea[CON_GRTASK_DATANUM];	//任务的数据项列表

	unsigned short	usPerTaskRetMem;				//任务执行一次所虚的字节数
	unsigned char	ucActive;						//任务状态：激活
	STDATETIME		stStartActive;					//任务开始活动时间
}STGRTASK;			//小于128字节

typedef struct _STGRTASKRUNINFO						//任务运行信息，可以由其他信息计算得到
{
	STDATETIME		stNextExeTime;					//下次执行时间(任务)
	STDATETIME		stNextUpTime;					//下次数据上行时间
	unsigned short	usHeadPos;						//当前存储的点号
}STGRTASKRUNINFO;

typedef struct _STGRTASKINFO						//任务主信息，需要存入DATAFLASH
{
	unsigned short	usExedNum;						//任务执行了次数
	unsigned short	usTailPos;						//最早的数据点，数据区循环存储usSavePointNum个点
	unsigned long	ulTailTime;						//最早的数据点对应的时间，从1990年开始，单位：秒
}STGRTASKINFO;		//控制为8个字节			//64*8=512，可以存入DATAFLASH的一页

//azh
typedef struct _STGRDEFINEAREA				//任务数据定义区
{
	STGRTASK		stGRTask[CON_GRTASK_NUM];		//任务列表
	STGRTASKINFO	stGRTaskInfo[CON_GRTASK_NUM];	//任务主信息列表
	STGRTASKRUNINFO stGRTaskRunInfo[CON_GRTASK_NUM];//任务运行信息列表
	STGRTASKRETVAL	stGRTaskRetVal;					//任务回应数据区，一个任务一个时间点的多项数据
}STGRDEFINEAREA;	//8k字节

//--------------------------------------------------------------------------
__EXTERN STGRDEFINEAREA			stGRDefineArea;				//8k字节
__EXTERN STGRTASKEXEINFO		stGRTaskExeInfo;
__EXTERN STSENDDILIST           stSendDIList;
//--------------------------------------------------------------------------
//任务控制初始化
__EXTERN unsigned char Task_Manage_Init(void);
//配置任务，由于总的任务数量、回应数据的空间分配都是固定的，因此，就无需进行任务的创建等工作。
__EXTERN unsigned char Task_Config(unsigned char ucTaskNo,unsigned char ucActive);
//清除任务
__EXTERN unsigned char Task_Clear(unsigned char ucTaskNo);
//获取任务单时间点数据长度
unsigned short Task_GetPerTimeSpace(unsigned char ucTaskNo);
//获取任务单时间点数据长度
__EXTERN unsigned short Task_GetPerTimeSpaceLittle(unsigned char ucTaskNo);
//任务回应数据存储
__EXTERN unsigned char Task_SaveRetVal(unsigned char ucTaskNo,unsigned char *pucRetData,unsigned short usRetDataLen);
//任务回应数据查询
__EXTERN unsigned char Task_QueryRetVal(unsigned char ucTaskNo,unsigned char *pucRetData,STDATETIME *pstStartTime,unsigned short usPointNum,unsigned char ucResendDataInter,unsigned short *pusOnePointLen);
__EXTERN unsigned char Task_QueryRetVal_ByDI(unsigned char ucTaskNo,unsigned char *pucRetData,STDATETIME *pstStartTime,unsigned short usPointNum,unsigned char ucResendDataInter,unsigned char *pucDataLen,unsigned short usDI);
//azh 170925 对应376.1-2013版的二类数据F219:测量点组合数据曲线数据
__EXTERN unsigned char Task_QueryRetVal_ByGroupDI(unsigned char ucTaskNo,unsigned char *pucRetData,STDATETIME *pstStartTime,unsigned short usPointNum,unsigned char ucResendDataInter,unsigned char *pucDataLen,unsigned short usDI);

//任务回应数据查询获取，获取最近几点
//__EXTERN unsigned char Task_QueryRetVal_Newest(unsigned char ucTaskNo,unsigned char *pucRetData,unsigned short usPointNum,unsigned char ucResendDataInter,STDATETIME *pstNewestTime,unsigned short *pusOnePointLen);

//根据执行时间间隔和上送时间间隔以及上送的选择频率获取任务上送的点数
//__EXTERN unsigned short Task_GetUpSendPointNum(unsigned char ucTaskNo);

//矫正任务信息
__EXTERN unsigned char Task_CheckAllTaskInfo(void);
//获得任务运行信息
__EXTERN unsigned char Task_GetTaskRunInfo(void);

//获得执行时间
__EXTERN STDATETIME	GetTaskNextExeTime(unsigned char ucTaskNo);
//获得上送时间
__EXTERN STDATETIME	GetTaskNextUpTime(unsigned char ucTaskNo);
__EXTERN STDATETIME	Task_FixNextTime(STDATETIME stDatetime,STTIMEINFO stStartTimeInfo,STTIMEINFO stInterTimeInfo);

__EXTERN unsigned char TaskCheckMeterIsValid(unsigned char ucMeterNo);
__EXTERN unsigned char SetDefaultMeterTask(unsigned char ucMeterNo);		//生成默认任务
__EXTERN unsigned char DeleteDefaultMeterTask(unsigned char ucMeterNo);

__EXTERN unsigned char Task_GetTaskNo_byMPNo(unsigned char ucMPNo,unsigned char ucPos);
//__EXTERN unsigned char Task_GetTaskNo_byPulseNo(unsigned char ucPulseNo);
__EXTERN unsigned char Task_GetTaskNo_Ter(void);

//__EXTERN unsigned char TaskCheckSumGroupIsValid(void);
//__EXTERN unsigned char SetDefaultSumGroupTask(void);
//__EXTERN unsigned char DeleteDefaultSumGroupTask(void);

//__EXTERN unsigned char TaskCheckPulseIsValid(unsigned char ucPulseNo);
//__EXTERN unsigned char SetDefaultPulseTask(unsigned char ucPulseNo);
//__EXTERN unsigned char DeleteDefaultPulseTask(unsigned char ucPulseNo);

__EXTERN unsigned char TaskCheckTerIsValid(void);
__EXTERN unsigned char SetDefaultTerTask(void);
__EXTERN unsigned char DeleteDefaultTerTask(void);

__EXTERN void PowOn_Get_Curve_Task_Para(void);
__EXTERN void Save_Curve_Task_Para(unsigned char ucTaskNo, unsigned char ucOffsetAddr, unsigned char type);
//-----------------------------------------------------------------------------------------
#endif

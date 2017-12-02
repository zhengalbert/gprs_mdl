/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Alarm_Timer_Response.h
 Description    : 定时刷新数据更新到全局数组
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/
#ifndef _ALARM_MANAGE_H
#define _ALARM_MANAGE_H

#define CON_COPYERR_TIMES		5	
//----------------------------------
#define		CONALRITEMNUM		8				//预判队列数据项
#define		CONALRTAKEITEMNUM	10				//告警携带数据项

#define		CONALR_METER_TIMEINTER		15		//抄表时间间隔，单位：分钟

#define		CONALR_UPFLAG_NONE			0xFF		//不上送
#define		CONALR_UPFLAG_NEED			0x11		//要上送，但是还没有上送
#define		CONALR_UPFLAG_DOING			0x22		//正在上送
#define		CONALR_UPFLAG_OK			0x33		//已经成功上送

#define		CON_UPFLOAT_COEF		101		//告警的上浮系数
#define		CON_DOWNFLOAT_COEF		99		//告警的下浮系数

#define		CON_SUMGROUP_ALR_NUM_INSTANT	4	//总加瞬时量越限判断次数
#define		CON_SUMGROUP_ALR_NUM_POW		4	//总加电量越限判断次数
#define		CON_ALR_TIMEINTER				10	//终端判断时间间隔，单位0.5秒
#define		CON_ALR_TIMEINTER_COEF			120	//终端定时抄表间隔浮动值，单位秒

#define		CON_ALRACTION_METERCOPY		30		//上电抄表延迟时间，单位0.5秒
#define		CON_ALRACTION_ALRUP			120		//上电告警上送延迟时间，单位0.5秒
//-------------------------------------------------
#define		CONALRRECORD_VALID  0xA5    //flash保存的记录效标识

#define		CONALRCODE_INVALID  0xFFFF
#define		CONALRFOREREPLYNUM	3				//告警确认补充抄表次数
#define		CONALRFORELISTNUM	5//10				//固定10项(同时发生报警数量)(预判队列)，抄表回应需要对应
#define		CONALRENSURELISTNUM	5//10				//固定10项(同时发生报警数量)(确认队列)

#define		CONALRRECITEMNUM	10				//告警记录携带数据项
#define		CONALRRECORDNUM_IMPORTANT	255		//重要告警日志255条
#define		CONALRRECORDNUM_NORMAL		255		//一般告警日志255条
#define		CONALRRECORDNUM_NEEDACK		16//国网帧序号0~15 azh 255		//告警确认255条

#define		CONALRCODE_INVALID			0xFFFF
#define		CONALRCODE_ALLDATAINIT		0x2001		//ERC1：数据初始化
#define		CONALRCODE_PARA_LOST		0x2002		//ERC2：参数丢失
#define		CONALRCODE_PARA_MODIFY		0x2003		//ERC3：重要参数变更
#define		CONALRCODE_SOFTWARECHANGE	0x2004		//ERC1：软件变更
#define		CONALRCODE_COMM_PASSWORDERR	0x2005		//ERC20:密码错误

#define		CONALRCODE_TERERR_RAM		0x2101		//终端主板内存故障
#define		CONALRCODE_TERERR_CLOCK		0x2102		//时钟故障
#define		CONALRCODE_TERERR_MAINCOMM	0x2103		//主板通信故障
#define		CONALRCODE_TERERR_485ERROR	0x2104		//485抄表故障
#define		CONALRCODE_TERERR_DISPLAY	0x2105		//显示板故障

#define		CONALRCODE_VAOVERUP			0x2801		//分路模拟量越限_Va
#define		CONALRCODE_VBOVERUP			0x2802		//分路模拟量越限_Vb
#define		CONALRCODE_VCOVERUP			0x2803		//分路模拟量越限_Vc

#define		CONALRCODE_VAOVERDOWN		0x2811		//分路模拟量越限_Va(下限)
#define		CONALRCODE_VBOVERDOWN		0x2812		//分路模拟量越限_Vb(下限)
#define		CONALRCODE_VCOVERDOWN		0x2813		//分路模拟量越限_Vc(下限)

#define		CONALRCODE_IAOVERUP			0x2804		//分路模拟量越限_Ia
#define		CONALRCODE_IBOVERUP			0x2805		//分路模拟量越限_Ib
#define		CONALRCODE_ICOVERUP			0x2806		//分路模拟量越限_Ic

#define		CONALRCODE_IAERROVERUP		0x2824		//Ia越上上限
#define		CONALRCODE_IBERROVERUP		0x2825		//Ib越上上限
#define		CONALRCODE_ICERROVERUP		0x2826		//Ic越上上限

#define		CONALRCODE_VABREAK			0x2821		//A相电压断相
#define		CONALRCODE_VBBREAK			0x2822		//B相电压断相
#define		CONALRCODE_VCBREAK			0x2823		//C相电压断相

#define		CONALRCODE_VALACK			0x2831		//A相电压缺相
#define		CONALRCODE_VBLACK			0x2832		//B相电压缺相
#define		CONALRCODE_VCLACK			0x2833		//C相电压缺相

#define		CONALRCODE_IUNBLANCE		0x2807		//电流不平衡越限
#define		CONALRCODE_UUNBLANCE		0x2808		//电压不平衡越限

#define		CONALRCODE_SPOWNORUP		0x2901		//超报装容量上限（kVA）,视在功率
#define		CONALRCODE_SPOWERRUP		0x2902		//超报装容量上上限（kVA）,视在功率

#define		CONALRCODE_PROGRAMTIME		0x2911		//编程时间更改
#define		CONALRCODE_DEMANDTIMECLR	0x2912		//需量清零时间更改
#define		CONALRCODE_PULSECONST		0x2913		//脉冲常数
#define		CONALRCODE_COPYDAYTIME		0x2914		//抄表日
#define		CONALRCODE_LOSTVOLNUM		0x2915		//断相次数
#define		CONALRCODE_FEERATE			0x2916		//费率更改
#define		CONALRCODE_DEMANDTIMENUM	0x2917		//需量清零次数变化
#define		CONALRCODE_PROGRAMNUM		0x2918		//编程次数变化

#define		CONALRCODE_METERFLY			0x2921		//表计飞走
#define		CONALRCODE_METERSTOP		0x2922		//表计停走
#define		CONALRCODE_METERDOWN		0x2923		//表计倒走

#define		CONALRCODE_METERBATTLOW		0x2924		//电池电压低
#define		CONALRCODE_METERTIMEERR		0x2925		//表计时钟异常

#define		CONALRCODE_SUMINSTANTPOWUP1	0x3001		//瞬时有功总加组1越上限
#define		CONALRCODE_SUMINSTANTPOWUP2	0x3002		//瞬时有功总加组2越上限
#define		CONALRCODE_SUMINSTANTPOWUP3	0x3003		//瞬时有功总加组3越上限
#define		CONALRCODE_SUMINSTANTPOWUP4	0x3004		//瞬时有功总加组4越上限
#define		CONALRCODE_SUMINSTANTPOWUP5	0x3005		//瞬时有功总加组5越上限
#define		CONALRCODE_SUMINSTANTPOWUP6	0x3006		//瞬时有功总加组6越上限
#define		CONALRCODE_SUMINSTANTPOWUP7	0x3007		//瞬时有功总加组7越上限
#define		CONALRCODE_SUMINSTANTPOWUP8	0x3008		//瞬时有功总加组8越上限

#define		CONALRCODE_SUMINSTANTPOWDOWN1	0x3011		//瞬时有功总加组1越下限
#define		CONALRCODE_SUMINSTANTPOWDOWN2	0x3012		//瞬时有功总加组2越下限
#define		CONALRCODE_SUMINSTANTPOWDOWN3	0x3013		//瞬时有功总加组3越下限
#define		CONALRCODE_SUMINSTANTPOWDOWN4	0x3014		//瞬时有功总加组4越下限
#define		CONALRCODE_SUMINSTANTPOWDOWN5	0x3015		//瞬时有功总加组5越下限
#define		CONALRCODE_SUMINSTANTPOWDOWN6	0x3016		//瞬时有功总加组6越下限
#define		CONALRCODE_SUMINSTANTPOWDOWN7	0x3017		//瞬时有功总加组7越下限
#define		CONALRCODE_SUMINSTANTPOWDOWN8	0x3018		//瞬时有功总加组8越下限

#define		CONALRCODE_SUMINSTANTVARUP1	0x3101		//瞬时无功总加组1越上限
#define		CONALRCODE_SUMINSTANTVARUP2	0x3102		//瞬时无功总加组2越上限
#define		CONALRCODE_SUMINSTANTVARUP3	0x3103		//瞬时无功总加组3越上限
#define		CONALRCODE_SUMINSTANTVARUP4	0x3104		//瞬时无功总加组4越上限
#define		CONALRCODE_SUMINSTANTVARUP5	0x3105		//瞬时无功总加组5越上限
#define		CONALRCODE_SUMINSTANTVARUP6	0x3106		//瞬时无功总加组6越上限
#define		CONALRCODE_SUMINSTANTVARUP7	0x3107		//瞬时无功总加组7越上限
#define		CONALRCODE_SUMINSTANTVARUP8	0x3108		//瞬时无功总加组8越上限

#define		CONALRCODE_SUMINSTANTVARDOWN1	0x3111		//瞬时无功总加组1越下限
#define		CONALRCODE_SUMINSTANTVARDOWN2	0x3112		//瞬时无功总加组2越下限
#define		CONALRCODE_SUMINSTANTVARDOWN3	0x3113		//瞬时无功总加组3越下限
#define		CONALRCODE_SUMINSTANTVARDOWN4	0x3114		//瞬时无功总加组4越下限
#define		CONALRCODE_SUMINSTANTVARDOWN5	0x3115		//瞬时无功总加组5越下限
#define		CONALRCODE_SUMINSTANTVARDOWN6	0x3116		//瞬时无功总加组6越下限
#define		CONALRCODE_SUMINSTANTVARDOWN7	0x3117		//瞬时无功总加组7越下限
#define		CONALRCODE_SUMINSTANTVARDOWN8	0x3118		//瞬时无功总加组8越下限

#define		CONALRCODE_SUMPOWUP1	0x3201		//有功总加组1越上限
#define		CONALRCODE_SUMPOWUP2	0x3202		//有功总加组2越上限
#define		CONALRCODE_SUMPOWUP3	0x3203		//有功总加组3越上限
#define		CONALRCODE_SUMPOWUP4	0x3204		//有功总加组4越上限
#define		CONALRCODE_SUMPOWUP5	0x3205		//有功总加组5越上限
#define		CONALRCODE_SUMPOWUP6	0x3206		//有功总加组6越上限
#define		CONALRCODE_SUMPOWUP7	0x3207		//有功总加组7越上限
#define		CONALRCODE_SUMPOWUP8	0x3208		//有功总加组8越上限

#define		CONALRCODE_SUMPOWDOWN1	0x3211		//有功总加组1越下限
#define		CONALRCODE_SUMPOWDOWN2	0x3212		//有功总加组2越下限
#define		CONALRCODE_SUMPOWDOWN3	0x3213		//有功总加组3越下限
#define		CONALRCODE_SUMPOWDOWN4	0x3214		//有功总加组4越下限
#define		CONALRCODE_SUMPOWDOWN5	0x3215		//有功总加组5越下限
#define		CONALRCODE_SUMPOWDOWN6	0x3216		//有功总加组6越下限
#define		CONALRCODE_SUMPOWDOWN7	0x3217		//有功总加组7越下限
#define		CONALRCODE_SUMPOWDOWN8	0x3218		//有功总加组8越下限

#define		CONALRCODE_SUMVARUP1	0x3301		//无功总加组1越上限
#define		CONALRCODE_SUMVARUP2	0x3302		//无功总加组2越上限
#define		CONALRCODE_SUMVARUP3	0x3303		//无功总加组3越上限
#define		CONALRCODE_SUMVARUP4	0x3304		//无功总加组4越上限
#define		CONALRCODE_SUMVARUP5	0x3305		//无功总加组5越上限
#define		CONALRCODE_SUMVARUP6	0x3306		//无功总加组6越上限
#define		CONALRCODE_SUMVARUP7	0x3307		//无功总加组7越上限
#define		CONALRCODE_SUMVARUP8	0x3308		//无功总加组8越上限

#define		CONALRCODE_SUMVARDOWN1	0x3311		//无功总加组1越下限
#define		CONALRCODE_SUMVARDOWN2	0x3312		//无功总加组2越下限
#define		CONALRCODE_SUMVARDOWN3	0x3313		//无功总加组3越下限
#define		CONALRCODE_SUMVARDOWN4	0x3314		//无功总加组4越下限
#define		CONALRCODE_SUMVARDOWN5	0x3315		//无功总加组5越下限
#define		CONALRCODE_SUMVARDOWN6	0x3316		//无功总加组6越下限
#define		CONALRCODE_SUMVARDOWN7	0x3317		//无功总加组7越下限
#define		CONALRCODE_SUMVARDOWN8	0x3318		//无功总加组8越下限

#define		CONALRCODE_SUMCOMPOVERLIMIT     0x3321 //总加差动越限,代码：0x3321-0x3328

#define		CONALRCODE_DOOROPEN				0x3401		//门开报警
#define		CONALRCODE_DOORCLOSE			0x3411		//门关报警
#define		CONALRCODE_POWERON				0x3402		//上电报警
#define		CONALRCODE_POWEROFF				0x3412		//停电报警

#define		CONALRCODE_SWITCH0OPEN			0x3403							//开关0开报警
#define		CONALRCODE_SWITCH0CLOSE			0x3413							//开关0关报警
#define		CONALRCODE_SWITCH1OPEN			CONALRCODE_SWITCH0OPEN+1		//开关1开报警 3404
#define		CONALRCODE_SWITCH1CLOSE			CONALRCODE_SWITCH0CLOSE+1		//开关1关报警 3414
#define		CONALRCODE_SWITCH2OPEN			CONALRCODE_SWITCH0OPEN+2		//开关2开报警 3405
#define		CONALRCODE_SWITCH2CLOSE			CONALRCODE_SWITCH0CLOSE+2		//开关2关报警 3415
#define		CONALRCODE_SWITCH3OPEN			CONALRCODE_SWITCH0OPEN+3		//开关3开报警 3406
#define		CONALRCODE_SWITCH3CLOSE			CONALRCODE_SWITCH0CLOSE+3		//开关3关报警 3416
#define		CONALRCODE_SWITCH4OPEN			CONALRCODE_SWITCH0OPEN+4		//开关4开报警 3407
#define		CONALRCODE_SWITCH4CLOSE			CONALRCODE_SWITCH0CLOSE+4		//开关4关报警 3417
#define		CONALRCODE_SWITCH5OPEN			CONALRCODE_SWITCH0OPEN+5		//开关5开报警 3408
#define		CONALRCODE_SWITCH5CLOSE			CONALRCODE_SWITCH0CLOSE+5		//开关5关报警 3418
#define		CONALRCODE_SWITCH6OPEN			CONALRCODE_SWITCH0OPEN+6		//开关6开报警 3409
#define		CONALRCODE_SWITCH6CLOSE			CONALRCODE_SWITCH0CLOSE+6		//开关6关报警 3419

#define		CONALRCODE_POLREVERSEA			0x3501							//A相电流反极性
#define		CONALRCODE_POLREVERSEB			0x3502							//B相电流反极性
#define		CONALRCODE_POLREVERSEC			0x3503							//C相电流反极性

#define		CONALRCODE_SHORTCIRA1			0x3504							//A相CT一次侧短路
#define		CONALRCODE_SHORTCIRB1			0x3505							//B相CT一次侧短路
#define		CONALRCODE_SHORTCIRC1			0x3506							//C相CT一次侧短路

#define		CONALRCODE_OPENCIRA2			0x3507							//A相CT二次侧开路
#define		CONALRCODE_OPENCIRB2			0x3508							//B相CT二次侧开路
#define		CONALRCODE_OPENCIRC2			0x3509							//C相CT二次侧开路

#define		CONALRCODE_SHORTCIRA2			0x350A							//A相CT二次侧短路
#define		CONALRCODE_SHORTCIRB2			0x350B							//B相CT二次侧短路
#define		CONALRCODE_SHORTCIRC2			0x350C							//C相CT二次侧短路

#define		CONALRCODE_VOLCIRREVERSE		0x350D							//电压回路逆相序

#define		CONALRCODE_REMOTECTRL			0x3601							//遥控跳闸记录
#define		CONALRCODE_POWERCTRL			0x3602							//功控跳闸记录
#define		CONALRCODE_ENERCTRL				0x3603							//电控跳闸记录
#define		CONALRCODE_BUYPOWPARA			0x3611							//购电参数设置

#define		CONALRCODE_ENERALR				0x3612							//ERC23 电控告警事件
#define		CONALRCODE_METERCOPY			0x3613							//ERC31 抄表失败事件记录
#define		CONALRCODE_FLUXLIMIT			0x3614							//ERC32 月通信超流量记录
#define		CONALRCODE_POWDIFFOVER			0x3615							//ERC28	电能量超差记录

#define		CONALRCODE_METERSTATECHANGE		0x3701							//ERC33 电表状态字变更


#define		CONALR_TYPE_IMPORTANT			0x55		//重要事件
#define		CONALR_TYPE_NORMAL				0xAA		//一般事件
#define		CONALR_TYPE_NOT_RECORD			0xFF		//事件不记录

#define		CON_ALR_STAT_ON			0xAA
#define		CON_ALR_STAT_OFF		0x55
#define		CON_ALR_STAT_MID		0x5A

#define		CON_SWITCH_STAT_OPEN		0x55
#define		CON_SWITCH_STAT_CLOSE		0xAA

/////////////////////////CT检测//////////////////////////

#define		CON_WORKMODE_NORMAL		0x55
#define		CON_WORKMODE_TEST		0xAA

#define		CON_CTTYPE_D		0xAA
#define		CON_CTTYPE_C		0x55
//-------------------------------------------------
typedef struct _STALRFOREITEMLIST
{
	STDATETIME		stDateTime;
	STDATAITEM_ALR	stDataItem_alr[CONALRITEMNUM];	//数据项
}STALRFOREITEMLIST;

typedef struct _STALRFOREJUDGE
{
	unsigned short	usAlrCode;			//告警编码
	unsigned char	ucMPNo;				//测量点号
	unsigned char	ucAlrJudgeTimes;	//重复次数
	unsigned char	ucAlrJudgeCount;	//重复次数 计数器
	unsigned char	ucAlrJudgeCycle;	//重复周期(分)
	unsigned char	ucAlrDataItemNum;	//补充判断数据项值
	STALRFOREITEMLIST	stAlrDataItem[CONALRFOREREPLYNUM];
}STALRFOREJUDGE;

typedef struct _STALRENSUREJUDGE
{
	unsigned short	usAlrCode;			//告警编码
	unsigned char	ucMPNo;				//测量点号
	STDATETIME		stDateTime;			//告警发生时间
	unsigned char	ucAlrDataItemNum;	//携带数据
	STDATAITEM_ALR	stAlrDataItem[CONALRTAKEITEMNUM];	//数据项
}STALRENSUREJUDGE;

#define		CONALR_STAT_HAPPEN_NOT	0xAA	//告警未发生或者恢复
#define		CONALR_STAT_HAPPEN		0x55	//告警发生

typedef struct _STALARMSTAT
{
	unsigned char ucAlrStat_Sys_VolaUpOver;			//A相电压越上上限
	unsigned char ucAlrStat_Sys_VolbUpOver;			//B相电压越上上限
	unsigned char ucAlrStat_Sys_VolcUpOver;			//C相电压越上上限
	unsigned char ucAlrStat_Sys_VolaDownOver;		//A相电压越下下限
	unsigned char ucAlrStat_Sys_VolbDownOver;		//B相电压越下下限
	unsigned char ucAlrStat_Sys_VolcDownOver;		//C相电压越下下限
	unsigned char ucAlrStat_Sys_IaOverLoad;			//A相电流越上限
	unsigned char ucAlrStat_Sys_IbOverLoad;			//B相电流越上限
	unsigned char ucAlrStat_Sys_IcOverLoad;			//C相电流越上限

	unsigned char ucAlrStat_Sys_IaErrOverLoad;		//A相电流越上上限
	unsigned char ucAlrStat_Sys_IbErrOverLoad;		//B相电流越上上限
	unsigned char ucAlrStat_Sys_IcErrOverLoad;		//C相电流越上上限

	unsigned char ucAlrStat_Sys_VolaBreak;			//A相电压断相
	unsigned char ucAlrStat_Sys_VolbBreak;			//B相电压断相
	unsigned char ucAlrStat_Sys_VolcBreak;			//C相电压断相

	unsigned char ucAlrStat_Sys_VolaLack;			//A相电压缺相
	unsigned char ucAlrStat_Sys_VolbLack;			//B相电压缺相
	unsigned char ucAlrStat_Sys_VolcLack;			//C相电压缺相

	unsigned char ucAlrStat_Sys_SPowOverUp;			//视在功率越限
	unsigned char ucAlrStat_Sys_SPowErrUp;			//视在功率越上上限

	unsigned char ucAlrStat_Sys_IUnbalance;			//电流不平衡
	unsigned char ucAlrStat_Sys_UUnbalance;			//电压不平衡

	unsigned char ucAlrStat_Sys_ProgramTime;		//编程时间变化
	unsigned char ucAlrStat_Sys_PulseConst;			//脉冲常数变化
	unsigned char ucAlrStat_Sys_CopyDayTime;		//抄表日变化
	unsigned char ucAlrStat_Sys_LostVolNum;			//断相次数变化
	unsigned char ucAlrStat_Sys_FeeRate;			//费率更改
	unsigned char ucAlrStat_Sys_DemandClr;			//需量清零

//	unsigned char ucAlrStat_Sys_MeterFly;			//表计飞走
//	unsigned char ucAlrStat_Sys_MeterStop;			//表计停走
//	unsigned char ucAlrStat_Sys_MeterDown;			//表计倒走
	unsigned char ucAlrStat_Sys_MeterBattLow;		//电池电压低
	unsigned char ucAlrStat_Sys_MeterTimeErr;		//表计时钟误差
//	unsigned char ucAlrStat_Sys_MeterPowDiff;		//电能量超差

	unsigned char ucAlrStat_Sys_PolReverseA;		//A相电流反极性
	unsigned char ucAlrStat_Sys_PolReverseB;		//B相电流反极性
	unsigned char ucAlrStat_Sys_PolReverseC;		//C相电流反极性
//
//	unsigned char ucAlrStat_Sys_ShortCirA1;			//A相CT一次侧短路
//	unsigned char ucAlrStat_Sys_ShortCirB1;			//B相CT一次侧短路
//	unsigned char ucAlrStat_Sys_ShortCirC1;			//C相CT一次侧短路
//
//	unsigned char ucAlrStat_Sys_OpenCirA2;			//A相CT二次侧开路
//	unsigned char ucAlrStat_Sys_OpenCirB2;			//B相CT二次侧开路
//	unsigned char ucAlrStat_Sys_OpenCirC2;			//C相CT二次侧开路
//
//	unsigned char ucAlrStat_Sys_ShortCirA2;			//A相CT二次侧短路
//	unsigned char ucAlrStat_Sys_ShortCirB2;			//B相CT二次侧短路
//	unsigned char ucAlrStat_Sys_ShortCirC2;			//C相CT二次侧短路

	unsigned char ucAlrStat_Sys_VolCirReve;			//电压回路逆相序
	unsigned char ucAlrStat_Sys_MainComm;			//模块通讯状态

	unsigned char ucAlrStat_Sys_ProgramCount;		//编程次数变更
	unsigned char ucAlrStat_Sys_MeterStateChange;		//状态字变更
}STALARMSTAT;

typedef struct _STSUMALARMSTAT
{
	unsigned char ucAlrStat_Sys_SumInstantPowUp;	//瞬时有功总加组越上限
	unsigned char ucAlrStat_Sys_SumInstantVarUp;	//瞬时无功总加组越上限
	unsigned char ucAlrStat_Sys_SumPowValUp;		//有功电量总加组越上限
	unsigned char ucAlrStat_Sys_SumVarValUp;		//无功电量总加组越上限

	unsigned char ucAlrStat_Sys_SumInstantPowDown;	//瞬时有功总加组越下限
	unsigned char ucAlrStat_Sys_SumInstantVarDown;	//瞬时无功总加组越下限
	unsigned char ucAlrStat_Sys_SumPowValDown;		//有功电量总加组越下限
	unsigned char ucAlrStat_Sys_SumVarValDown;		//无功电量总加组越下限

	unsigned char ucAlrStat_Sys_CompOverLimit;		//差动越限告警
}STSUMALARMSTAT;
//-------------------------------------------
//////////////////////////////////////告警部分结构定义//////////////////////////////////////
//azh 
typedef struct _STALRAUXINFO
{	
	unsigned char   ucUpFlag;            //本条告警记录状态 无须主动上报 没有上报 正在上报 已上报
//	unsigned char	ucAlrSendFrameSEQ;	//上送告警时对应的ＳＥＱ 用于确认告警上报 改上面这个状态 azh 放在RAM里 效率高
//	STDATETIME		stNextUpDateTime;	//下次重报时间 
    unsigned char   ucUpCount;          //由于比较占空间 所以只保存一个已上报次数 也可以确定下次上报重发时间
}STALRAUXINFO;//这是每条告警记录保存在EEPROM里的用于确认和下次重报时间 序号是和Flash里记录是相对应的
typedef struct _STPERALRAUXINFO
{	
    STALRAUXINFO    stAlrAuxInfo;
	unsigned char   ucCrc;              //累加校验合(累加和+偏移)
}STPERALRAUXINFO;

typedef struct _STALRACKINFO
{	
	unsigned char	ucAlrRecordNo;		//告警记录号
	unsigned char	ucAlrSendFrameSEQ;	//上送告警时对应的ＳＥＱ
}STALRNEEDACKINFO;
typedef struct _STALRNEEDACKINFOLIST
{	
	unsigned char	ucList_Head_P;		//最新一条
	unsigned char	ucList_Tail_P;		//事件第一条
	STALRNEEDACKINFO	stAlrNeedAckInfoList[CONALRRECORDNUM_NEEDACK];//
}STALRNEEDACKINFOLIST;

typedef struct _STALRNEEDACKINFOGROUP
{
	STALRNEEDACKINFOLIST stAlrNeedAckInfo_Imp;
	STALRNEEDACKINFOLIST stAlrNeedAckInfo_Nor;
}STALRNEEDACKINFOGROUP;

typedef struct _STALRRECORDINDEXINFO
{
	unsigned char	ucList_Head_P;		//最新一条
	unsigned char	ucList_Tail_P;		//事件第一条
	unsigned char	ucList_Proc_Tail_P;	//需要上送处理的第一条
}STALRRECORDINDEXINFO;

typedef struct _STALRRECORDINFO
{
	unsigned char ucAlrValidFlag[8];		//信息有效标志
	STALRRECORDINDEXINFO stAlrRecordInfo_Imp;	//重要事件
	STALRRECORDINDEXINFO stAlrRecordInfo_Norm;	//一般事件
}STALRRECORDINFO;

typedef struct _STALRRECORD
{
    unsigned char   ucavailble;         //azh 没有校验码 留一个有效标志 也是好的
	unsigned char	ucAlrERCCode;		//告警编码
	unsigned char	ucReserve;
	unsigned char	ucMPNo;				//测量点号
	unsigned char	ucUpFlag;			//上送标志		未上送00，正在上送11，已经上送22，不上送33
	STDATETIME		stNextUpDateTime;	//下次发送时间
	STDATETIME		stDateTime;			//告警发生时间
	unsigned char	ucAlrDataItemNum;	//携带数据
	STDATAITEM_ALR	stAlrDataItem[CONALRRECITEMNUM];	//数据项
}STALRRECORD;	//~330//558 bytes

typedef struct	_STALRCODEINFO
{
	unsigned char ucMPNo;
	unsigned short	usAlrCode;
}STALRCODEINFO;

typedef struct _STSWITCHALARMINFO
{
	unsigned char	ucValid;
	unsigned char	ucCtrlTurn;				//轮次
	unsigned short  usDelayCount;			//计数器
	unsigned char	ucCtrlType;				//控制类型	
	unsigned char	ucCtrlCurveNo;			//控制曲线
	double			dCurrVal;				//执行当时的电量或者功率
	double			dLastVal;				//最终电量或者功率
	double			dLoadFixVal;			//定值
}STSWITCHALARMINFO;
//-------------------------------------------
__EXTERN STDATETIME	g_stTerPowerChangeTime;

__EXTERN unsigned short	usTimerFinishFlag;
__EXTERN unsigned char gucOneLoopFlag;
__EXTERN STCMDFLAG		g_stCmdFlag[CON_METER_NUM];
__EXTERN unsigned short	usAlrActionLockTimer;						//上电后动作延迟控制计数器
__EXTERN STALARMSTAT	stAlarmStatus[CON_MAX_MPNUM];				//当前告警状态信息队列(当前的)
__EXTERN unsigned short usTimerCopyFlag[CON_METER_NUM];				//抄表标志

__EXTERN STALRFOREJUDGE		stAlrForeJudgeList[CONALRFORELISTNUM];		//预判队列
__EXTERN STALRENSUREJUDGE	stAlrEnsureJudgeList[CONALRENSURELISTNUM];	//确诊队列

__EXTERN unsigned char	ucMeterCopyErrTimes[CON_MAX_MPNUM];
__EXTERN unsigned char	ucMeterCopyErrAlrFlags[CON_MAX_MPNUM];

__EXTERN STALRRECORDINFO		stAlrRecordInfo;			//告警日志索引信息
__EXTERN STALRNEEDACKINFOGROUP	stAlrNeedAckInfoGroup;		//告警需要确认列表
__EXTERN STALRCATEGORY			stAlrHappenWord;
__EXTERN STALRRECORD	        stAlrRecordTemp;	//告警记录临时

__EXTERN unsigned char g_ucEventCount_Important;//重要事件记数器，对应最新一条
__EXTERN unsigned char g_ucEventCount_Normal;//一般事件记数器，对应最新一条

__EXTERN unsigned char ucMeterStateChangeFlag[CON_METER_NUM][8];

//-----------------------------------------------

__EXTERN unsigned char Alarm_CheckForeJudgeList(unsigned short usAlrCode,unsigned char ucMPNo);
__EXTERN unsigned char Alarm_FillForeJudgeList(unsigned short usAlrCode,unsigned char ucMPNo);
__EXTERN unsigned char Alarm_CheckEnsureJudgeList(unsigned short usAlrCode,unsigned char ucMPNo);
__EXTERN unsigned char Alarm_FillEnsureJudgeList(unsigned short usAlrCode,unsigned char ucMPNo);
__EXTERN unsigned char Clear_AlrForeJudge(STALRFOREJUDGE	*pstAlrForeJudge);
__EXTERN unsigned char Clear_AlrEnsureJudge(STALRENSUREJUDGE	*pstAlrEnsureJudge);

__EXTERN unsigned char Get_Shield_ACD_byAlrCode(unsigned short usAlrCode,unsigned char ucFlag);
//unsigned char Get_ACD_byAlrCode(unsigned short usAlrCode);

__EXTERN STHISVAL	GetUnBalanceVal(STHISVAL stHisA,STHISVAL stHisB,STHISVAL stHisC,unsigned char ucMeterType);

//-------------------------------
__EXTERN unsigned char Alarm_Init(unsigned char ucFlag);
__EXTERN unsigned char Alarm_Init_All(void);

__EXTERN unsigned char Alarm_CopyMeter_Timer(void);
__EXTERN unsigned char Alarm_Manage(void);

__EXTERN unsigned char Fill_AlarmAffirm_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
__EXTERN unsigned char Fill_AlarmTake_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);

__EXTERN unsigned char Alarm_EnsureJudge_AllDataInit(void);
__EXTERN unsigned char Alarm_EnsureJudge_SoftwareChange(void);
//__EXTERN unsigned char Alarm_EnsureJudge_485Err(void);
__EXTERN unsigned char Alarm_EnsureJudge_MeterCopy(unsigned char ucMeterCopyErrFlag,unsigned char ucMPNo);
__EXTERN unsigned char Alarm_EnsureJudge_ParaModify_IDOnly(unsigned char *pucTmp,unsigned char ucMsa);
__EXTERN unsigned char Alarm_EnsureJudge_TerErr_RAM(void);
__EXTERN unsigned char Alarm_EnsureJudge_TerErr_Clock(void);
__EXTERN unsigned char Alarm_EnsureJudge_TerErr_MainComm(void);
__EXTERN unsigned char Alarm_EnsureJudge_TerErr_Display(void);
//__EXTERN unsigned char Alarm_EnsureJudge_PasswordErr(unsigned char *pucPass,unsigned char ucMsa);
__EXTERN unsigned char Alarm_EnsureJudge_OverFluxLimit(unsigned long ulCurrFlux,unsigned long ulFluxLimit);

//__EXTERN unsigned char Alarm_EnsureJudge_RemoteCtrl(STSWITCHALARMINFO *pstSwitchAlarmInfo);
//__EXTERN unsigned char Alarm_EnsureJudge_PowerCtrl(STSWITCHALARMINFO *pstSwitchAlarmInfo);
//__EXTERN unsigned char Alarm_EnsureJudge_EnergyCtrl(STSWITCHALARMINFO *pstSwitchAlarmInfo);
//__EXTERN unsigned char Alarm_EnsureJudge_BuyPowEnerRec(unsigned char *pstBuyEnergyRecPtr,double dLeftVal_befor,double dLeftVal_after);
//__EXTERN unsigned char Alarm_EnsureJudge_EnergyAlr(unsigned char ucEnerType,unsigned char ucCtrlTurn,double dEnerVal,double dFixVal);

__EXTERN unsigned char Alarm_InitAlrStatus(void);
__EXTERN unsigned char Alarm_InitAlrHappenWord(void);
//__EXTERN unsigned char Alarm_InitAlrRecordInfo(void);
__EXTERN unsigned char Alarm_Init_AlrNeedAckInfoGroup(void);
__EXTERN unsigned char Alarm_DeleteAlrNeedAckInfo(unsigned char ucAlrRecordNo,unsigned char ucAlrType);
__EXTERN unsigned char DeleteAlrNeedAckInfo(STALRNEEDACKINFOLIST *pstAlrNeedAckInfo,unsigned char ucAlrRecordNo);
__EXTERN unsigned char Alarm_QueryRecord(unsigned char ucAlrRecordNo,STALRRECORD *pstAlrRecord,unsigned char ucAlrType);
__EXTERN unsigned char ClearAlrRecord(STALRRECORD *pstAlrRecord);
__EXTERN unsigned char ClearAlrNeedAckInfoList(STALRNEEDACKINFOLIST *pstAlrNeedAckInfoList);

__EXTERN unsigned char Alarm_DownRecv_Manage(unsigned char ucSEQ,unsigned char ucAlrType);
__EXTERN unsigned char Alarm_SetAlrRecordFlag(unsigned char ucAlrRecordNo,unsigned char ucAlrType);
//__EXTERN void Get_AlrCode_Newest(unsigned char ucResetFlag,unsigned short *pusAlrCode,unsigned char *pucMPNo);
__EXTERN unsigned long  GetUnBalanceVal_byVal(unsigned long ulValA,unsigned long ulValB,unsigned long ulValC,unsigned char ucMeterType);

__EXTERN void TimeChange_AlarmManage(STDATETIME stOldTime,STDATETIME stNewTime);
__EXTERN void CopyMeter_Timer_StartNow(void);
__EXTERN void Set_AlrActionLockTimer(unsigned short usLockTimer);

__EXTERN unsigned short Alarm_GetFrameACD(void);
__EXTERN void Alarm_SetFrameACD(unsigned short usACD);

__EXTERN unsigned char Alarm_GetAlrERCNo(unsigned short usAlrCode);
//__EXTERN unsigned char Get_CurrentDoorStat(void);
//__EXTERN unsigned char Get_CurrentSwitchStat(unsigned char ucSwitchNo);
//__EXTERN unsigned char Get_Switch_Change_Info(void);
//__EXTERN unsigned char Set_Switch_Change_Info(unsigned char ucInfo);
//__EXTERN unsigned char Init_Switch_Change_Info(void);
__EXTERN STDATETIME Get_TerPowerChangeTime(void);
__EXTERN unsigned char Set_TerPowerChangeTime(STDATETIME *pstTime);
__EXTERN STCTVALUE GetCTCheckVal(void);

__EXTERN unsigned char Alarm_FillRS485MeterDi(unsigned char ucTaskSource);
__EXTERN void Set_Alarm_CopyMeter_Flag(unsigned char uctasktype, unsigned char uctaskno, unsigned short flag);
__EXTERN void Clr_Alarm_CopyMeter_Flag(unsigned char uctasktype, unsigned char uctaskno);
__EXTERN void Alarm_Get_Last_TerTime(void);
__EXTERN void Alarm_Save_Last_TerTime(void);
__EXTERN void MeterCopy_Proc_Check(void);

#endif

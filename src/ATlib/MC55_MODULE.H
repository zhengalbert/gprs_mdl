/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : MC55_MODULE.H
 Description    :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2011-09-23      azh           the original version
******************************************************************************/
#ifndef     _MODULE_H
#define     _MODULE_H

//#include "..\include_public\public.h"

#define     _MC55_DEBUG         0

#define		CON_AT_DLY_INVAL				100//10ms 是在timing_task任务100ms调用的
//主要用于 	PIN_Timer(); AT_Timer();这二个函数里的超时 原来是10ms计数 现在改成100ms计数
#define     AT_DLY_100MS        (100 / CON_AT_DLY_INVAL)
#define     AT_DLY_200MS        (200 / CON_AT_DLY_INVAL)
#define     AT_DLY_400MS        (400 / CON_AT_DLY_INVAL)
#define     AT_DLY_500MS        (500 / CON_AT_DLY_INVAL)
#define     AT_DLY_1S           (1000 / CON_AT_DLY_INVAL)
#define     AT_DLY_2S           (2000 / CON_AT_DLY_INVAL)
#define     AT_DLY_3S           (3000 / CON_AT_DLY_INVAL)
#define     AT_DLY_4S           (3000 / CON_AT_DLY_INVAL)
#define     AT_DLY_5S           (5000 / CON_AT_DLY_INVAL)
#define     AT_DLY_6S           (5000 / CON_AT_DLY_INVAL)
#define     AT_DLY_10S          (10000 / CON_AT_DLY_INVAL)
#define     AT_DLY_15S          (15000 / CON_AT_DLY_INVAL)
#define     AT_DLY_18S          (18000 / CON_AT_DLY_INVAL)
#define     AT_DLY_25S          (25000 / CON_AT_DLY_INVAL)
#define     AT_DLY_30S          (30000 / CON_AT_DLY_INVAL)
#define     AT_DLY_35S          (35000 / CON_AT_DLY_INVAL)
#define     AT_DLY_60S          (60000 / CON_AT_DLY_INVAL)
#define     AT_DLY_80S          (90000 / CON_AT_DLY_INVAL)
#define     AT_DLY_90S          (90000 / CON_AT_DLY_INVAL)
#define     AT_DLY_120S         (120000 / CON_AT_DLY_INVAL)
#define     AT_DLY_10M          (10*60*1000 / CON_AT_DLY_INVAL)
//#define       _PZF_VC_COMPILE

#define     CON_HARDWARE_VER                0x40000001
#define     CON_SOFTWARE_VER                0x56303031

#define     CON_COMUNNICATION_MAX_CHANNEL   3           //最多的通迅通道数量

#define     TYPE_COMUNNICATION_INVALID      0
#define     TYPE_COMUNNICATION_SMS          1
#define     TYPE_COMUNNICATION_TCP          2
#define     TYPE_COMUNNICATION_UDP          3

#define     CON_PIN_DELAY_MBPWCTL           AT_DLY_100MS//10
#define     CON_PIN_DELAY_MBACK             5
#define     CON_PIN_DELAY_RING              5
#define     CON_PIN_DELAY_GSMACK            5
#define     CON_PIN_DELAY_ONOFF             10

#define     CON_GR47_TX_BUFFER_DEPTH    1200//700//1024//4096//2048//1024       //缓冲区深度
#define     CON_GR47_RX_BUFFER_DEPTH    1024//800//700//1024//4096//2048//1024      //缓冲区深度
#define     CON_SCI_TX_BUFFER_DEPTH     1024//4096//2048//1024      //缓冲区深度
#define     CON_SCI_RX_BUFFER_DEPTH     1024//4096//2048//1024      //缓冲区深度
#define     CON_SCI_DEBUG_BUFFER_DEPTH	64//256//用于GPRS调试输出

#define     CON_DATA_SEND_BUFFER_DEPTH  1200//530       //cfh 10-04-08

#define     CON_AT_MAX_REVEIVE_PACKAGE  650//700//1500//750         //GPRS数据帧700
#define     CON_AT_MAX_REVEIVE_DATA     CON_AT_MAX_REVEIVE_PACKAGE-40           //GPRS数据负荷660
#define     CON_SMS_BUFFER_MAX          280//400            //短信缓冲区长度
#define     CON_FRAMEDATA_LENGTH_MAX    650//1500//750//700         //主板－>模块数据量，大

#define     CON_AT_CMD_DEPTH            18              //AT命令队列深度
#define     CON_AT_DATALIST_DEPTH       6//3                //对于2132队列深度是3  AT上发数据队列深度cfh 10-04-12
#define     CON_TASK_SCI_MAX            6//3                //SCI发送任务队列深度   cfh 10-04-12

#define     CON_FRAME_LENGTH_MIN        8


//超时时间常量设置,每10MS调用一次 //azh 110928 改成100ms
//CON_CMD_DELAY_10MS对于小于100ms 都用100ms代替 !! 有问题？？
#define     CON_CMD_DELAY               AT_DLY_1S//100     //隔延时1s
#define     CON_CMD_DELAY_10MS          AT_DLY_100MS//1          //延时10ms
#define     CON_CMD_DELAY_100MS         AT_DLY_100MS//1          //延时10ms
#define     CON_AT_TIMEOUT_MAX          AT_DLY_5S//500     //AT命令超时设定：5妙
#define     CON_GPRS_TIMEOUT_MAX        AT_DLY_90S//9000        //GPRS连接命令超时设定：30s

#define     CON_AT_MC55_SILENCE         AT_DLY_500MS//50
#define     CON_AT_SILENCE_MAX          AT_DLY_400MS//40//20          //接收字符后停止时间100ms
#define     CON_TIMEER_GSMSTATE         AT_DLY_25S//AT_DLY_18S	//1800        //GSM状态定时器预值
#define     CON_TIMEER_GPRSSTATE        AT_DLY_10M	//10*60*100   //GPRS状态定时器预值30s
#define     CON_TIMEER_GPRSCMDSTATE     AT_DLY_2S	//200     //GPRSCMD状态定时器预值5s
#define     CON_TIMEER_POWEROFF         AT_DLY_60S	//6000        //关电定时器预值1M
#define     CON_SMS_TIMEOUT_MAX         AT_DLY_30S	//3000

#define     CON_POWEROFF_DELAY          AT_DLY_60S//6000        //关电定时器预值1M

#define     CMD_GPRSDATA_AUTO_UP            0x01
#define     CMD_SMSDATA_AUTO_UP             0x02

#define     CON_AT_MAX_SEND_PACKAGE     600//512//700//1500//750//600//530       //cfh 10-04-08      //GPRS数据包700
#define     CONTC35UPLISTNUM            4//8//50

//短信参数
typedef struct
{
    unsigned char ucLen;
    unsigned char ucCode[16];      // 主站号码ASCII  8613516807125
}ST_SMS_PARAM,*PST_SMS_PARAM;

//IP参数
typedef struct
{
    unsigned char ucIP[15];     //IP地址 ASCII
    unsigned char ucPort[5];    //端口号 ASCII
}ST_IP_PARAM,*PST_IP_PARAM;

typedef union
{
    ST_SMS_PARAM        stSMSParam;
    ST_IP_PARAM         stIPParam;
}UN_CHANNEL_ADDR,*PUN_CHANNEL_ADDR;


//通道参数结构体
typedef struct
{
    unsigned char       ucType;//通道通讯方式 TYPE_COMUNNICATION_SMS/TYPE_COMUNNICATION_TCP/TYPE_COMUNNICATION_UDP
    UN_CHANNEL_ADDR     unChInfo;

}ST_COMMUNICATION_CHANNEL,*PST_COMMUNICATION_CHANNEL;

//公用通迅参数
typedef struct
{
    unsigned char   ucSCALen;               // 短消息服务中心号码(SMSC地址) 例如:杭州8613800571500
    unsigned char   ucAPNLen;
    unsigned char   SCA[16];
    unsigned char   cAPN[24];               //APN
}ST_COMMUNICATION_COMMON_PARAM,*PST_COMMUNICATION_COMMON_PARAM;

//PIN,PUK参数
typedef struct
{
    unsigned char   ucIsPwdValid;       //密码是否启动
    unsigned char   ucPinPwd[4];        //PIN 密码
    unsigned char   ucPinPwdOld[4];     //旧的PIN密码
    unsigned char   ucPukPwd[8];        //PUK密码
}ST_PIN_PUK,*PST_PIN_PUK;


//接入账户参数
typedef struct
{
    unsigned char   ucIsAccountValid;       //密码是否启动
    unsigned char   ucUserNameLen;          //用户名长度
    unsigned char   ucUserName[32];         //用户名
    unsigned char   ucPwdLen;               //密码长度
    unsigned char   ucPwd[32];              //密码
}ST_PDP_ACCOUNT,*PST_PDP_ACCOUNT;

typedef struct
{
    unsigned char ucInforLength;        // 模块版本信息长度
    unsigned char ucInformation[32];        // 模块版本信息
}ST_MODULE_INFOR,PST_MODULE_INFOR;

typedef struct _STTC35TASK
{
    unsigned char               ucChNo;             //通道号
    ST_COMMUNICATION_CHANNEL    unCurAddrInfo;      //当前通迅地址
    unsigned short              ucDataLength;       //数据长度
    unsigned char               pData[CON_AT_MAX_SEND_PACKAGE]; //数据缓冲区<256
}STTC35TASK,*PSTTC35TASK;

typedef struct _STTC35UPLIST
{
    unsigned char ucList_Head_P;
    unsigned char ucList_Tail_P;
    STTC35TASK  stTC35Task[CONTC35UPLISTNUM];
}STTC35UPLIST,*PSTTC35UPLIST;


/**************************************************************************/

//模块通迅参数
typedef struct
{
    ST_COMMUNICATION_COMMON_PARAM   stCommonParam;      //42
    ST_PIN_PUK                      stPINPUK;           //22
    ST_COMMUNICATION_CHANNEL        stCh[CON_COMUNNICATION_MAX_CHANNEL];    //22
    ST_PDP_ACCOUNT                  stPDPAccount;
    ST_MODULE_INFOR                 stModuleInfor;
    unsigned short                  usTimerParam;       //GPRS重新连接定时器参数（分钟）
    unsigned char                   ucIs32BytePDP;
    unsigned char                   ucGPRSServer;       //是否起用GPRSServer功能，1表示启用，0表示不启用

}ST_MODULE_COMM_PARAM,*PST_MODULE_COMM_PARAM;


/**************************************************************************/

typedef struct _MODULE_RUN_STATE
{
    unsigned int  ucHardWareVer;
    unsigned int  ucSortWareVer;
    unsigned char ucCurCommChannel;             //当前处于的通道
    unsigned char ucSignIntension;              //信号强度

    unsigned char bnouse1           :1;         //IIC读写错误
    unsigned char bIsRing           :1;         //是否有振铃
    unsigned char bIsPWDError       :1;         //密码错误
    unsigned char bIsPUKLock        :1;         //PUK锁定
    unsigned char bIsPINLock        :1;         //PIN锁定
    unsigned char bIsGPRSConnected  :1;         //GPRS是否已连接
    unsigned char bIsSIMExist       :1;         //是否有服务网络(SIM卡存在)
    unsigned char bIsExist          :1;         //手机模块是否异常，是否存在

}ST_MODULE_RUN_STATE;

typedef struct _STMODPARACHINFO
{
    unsigned char   bIsPWDChanged:1;            //密码设置
    unsigned char   bIsAPNChanged:1;            //PDP账户是否有效
    unsigned char   bIsAccountChanged:1;        //账户是否变更
    unsigned char   bIsCHInfoChanged:1;         //当前通道配置改变
    unsigned char   bIsSMSCenterChanged:1;      //短信中心
    unsigned char   bIsSMSNoChanged:1;          //短信号码
    unsigned char   bIsLocalIPChanged:1;        //本地IP
    unsigned char   bIsSubMackChanged:1;        //子网掩码

    unsigned char   bIsGateWayChanged:1;        //网关
    unsigned char   bIsServerChanged:1;         //代理服务器
    unsigned char   Reserved1:6;

    unsigned char   Reserved2;
}STMODPARACHINFO;

/**************************************************************************/

typedef struct
{
    unsigned char   bMBACK:1;
    unsigned char   bOldMBACK:1;
    unsigned char   bIsMBACKPulse:1;

//  unsigned char   bRing:1;
//  unsigned char   bOldRing:1;
//  unsigned char   bIsRingPulse:1;

    unsigned char   bMBPWCTL:1;
    unsigned char   bOldMBPWCTL:1;
    unsigned char   bIsMBPWCTLPulse:1;

    unsigned short  usMBACKCnt;
//  unsigned short  usRINGCnt;
//  unsigned short  usGSMACKCnt;
    unsigned short  usMBPWCTLCnt;
}ST_PIN_STATE;

/********************************************************************
*   缓冲区管理
*   > 头指针＝尾指针时，缓冲区为空
*   > 新数据写入后尾指针增加
*   > 数据读出后头指针后移
*   > 尾指针＋1 ＝ 头指针时，缓冲区满
*********************************************************************/
typedef struct  _BUFFER_BLOCK
{
    unsigned short  usHeadPtr;      //头指针
    unsigned short  usTailPtr;      //尾指针
    unsigned short  usMaxDepth;     //缓冲区深度
    unsigned char*  pBuf;           //存储区指针
}ST_SCI_BUFFER,*PST_SCI_BUFFER;

typedef struct  _ST_SEND_HANDLE//用于监视输出
{
	ST_SCI_BUFFER	stSendBuf;		
    unsigned char	dly;           //间隔多少时间才去启动发送
    
}ST_SEND_HANDLE,*PST_SEND_HANDLE;

//AZH 100302
//#define     PIN_IGT                     (1 << 1)    //P2.1
//#define     PIN_EMERGOFF                (1 << 2)    //P2.2
//#define     PIN_POWERON                 (1 << 20)   //P0.20

#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
#define     PIN_IGT_SET                 FIO1SET = P1_CPU_IGT
#define     PIN_IGT_CLR                 FIO1CLR = P1_CPU_IGT
#define     PIN_EMERGOFF_SET            FIO1SET = P1_CPU_EVERGOFF
#define     PIN_EMERGOFF_CLR            FIO1CLR = P1_CPU_EVERGOFF
#define     PIN_POWERON_SET             FIO2SET = P2_VGPRS_ON
#define     PIN_POWERON_CLR             FIO2CLR = P2_VGPRS_ON
#else//H_METER_PRJ,WLS_CUR_PRJ
#define     PIN_IGT_SET                 FIO0SET = P0_CPU_IGT
#define     PIN_IGT_CLR                 FIO0CLR = P0_CPU_IGT
#define     PIN_EMERGOFF_SET            FIO0SET = P0_CPU_EVERGOFF
#define     PIN_EMERGOFF_CLR            FIO0CLR = P0_CPU_EVERGOFF
#define     PIN_POWERON_SET             FIO0SET = P0_C_GPRSON
#define     PIN_POWERON_CLR             FIO0CLR = P0_C_GPRSON
#endif//
//#define     PIN_NETLED                  (1 << 17)//P0.17  网络灯
//#define     NETLED_SET                  FIO1CLR = P1_GNET_LED
//#define     NETLED_CLR                  FIO1SET = P1_GNET_LED
#define     NETLED_SET                  GPRS_RLED_ON()
#define     NETLED_CLR                  GPRS_RLED_OFF()

//#define     PIN_DATALED                 (1 << 18)//P0.18  数据灯
//#define     DATALED_SET                 FIO1CLR = P1_GDATA_LED
//#define     DATALED_CLR                 FIO1SET = P1_GDATA_LED
#define     DATALED_SET                 GPRS_GLED_ON()
#define     DATALED_CLR                 GPRS_GLED_OFF()



//#define       PIN_PORTK1_GSMCHECK         0x80//PI7

//#define   CON_MC55_EXIST              IO_PI//模块是否安装，为1表示没有安装，为0表示安装
//#define   CON_MC55_EXIST_MESK         0X80//PI7
//-------------------------------------------------------------------------
//__EXTERN unsigned char  ucGr47_TX_Buffer[CON_GR47_TX_BUFFER_DEPTH];
__EXTERN unsigned char  ucGr47_RX_Buffer[CON_GR47_RX_BUFFER_DEPTH];

__EXTERN unsigned char   ucDataSendBuffer[CON_DATA_SEND_BUFFER_DEPTH];

__EXTERN ST_SCI_BUFFER  stAT_TX_BUFFER;
__EXTERN ST_SCI_BUFFER  stAT_RX_BUFFER;
__EXTERN ST_SEND_HANDLE  stDebOut_TX_Handle;

__EXTERN ST_MODULE_COMM_PARAM   stModuleComParam;
__EXTERN ST_MODULE_RUN_STATE    stModuleRunState;
__EXTERN ST_PIN_STATE           stPinSate;
__EXTERN unsigned short         ucLink_Time;

__EXTERN STMODPARACHINFO        stModParaChInfo;    //模块相关参数更改情况

__EXTERN STTC35UPLIST       stTC35UpList;       //上送队列
__EXTERN STTC35TASK         stTC35TaskTmp;      //借助这个临时性全局变量，是考虑减少堆栈的使用
/**************************************************************************/
__EXTERN void PIN_Timer(void);
//void PIN_SET_ONOFF(unsigned char bSet);
//void PIN_SET_DTR(unsigned char bSet);
__EXTERN void MC55_SysInit(void);
//void PIN_Write0(unsigned int uPinMask, unsigned char ucDts);
//void PIN_Write1(unsigned int uPinMask, unsigned char ucDts);
//unsigned int GetPinState(void);

__EXTERN void DebugPrint(unsigned char* pstr, unsigned short usLen);
//extern unsigned char UART0_SendData(void);
//extern unsigned char UART1_SendData(void);

__EXTERN void MC55_Init(void);
__EXTERN void Mc55_Run(void);
__EXTERN void IRQ_Timer0(void);

__EXTERN void Init_CommPara_MC55(void);
__EXTERN void Reload_SCA_Para(void);
__EXTERN void Reload_Chinfo_Para(void);
__EXTERN void Reload_PINPUK_Para(void);
__EXTERN void Reload_PDP_Para(void);
__EXTERN void Reload_ReLink_Para(void);
__EXTERN void RComm_PinSetFlag(void);

__EXTERN void RComm_ResetStart(void);
__EXTERN void GPRS_TargetInit(void);

#endif

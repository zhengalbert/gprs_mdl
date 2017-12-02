/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : MC55_MODULE.H
 Description    :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2011-09-23      azh           the original version
******************************************************************************/
#ifndef     _MODULE_H
#define     _MODULE_H

//#include "..\include_public\public.h"

#define     _MC55_DEBUG         0

#define		CON_AT_DLY_INVAL				100//10ms ����timing_task����100ms���õ�
//��Ҫ���� 	PIN_Timer(); AT_Timer();�����������ĳ�ʱ ԭ����10ms���� ���ڸĳ�100ms����
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

#define     CON_COMUNNICATION_MAX_CHANNEL   3           //����ͨѸͨ������

#define     TYPE_COMUNNICATION_INVALID      0
#define     TYPE_COMUNNICATION_SMS          1
#define     TYPE_COMUNNICATION_TCP          2
#define     TYPE_COMUNNICATION_UDP          3

#define     CON_PIN_DELAY_MBPWCTL           AT_DLY_100MS//10
#define     CON_PIN_DELAY_MBACK             5
#define     CON_PIN_DELAY_RING              5
#define     CON_PIN_DELAY_GSMACK            5
#define     CON_PIN_DELAY_ONOFF             10

#define     CON_GR47_TX_BUFFER_DEPTH    1200//700//1024//4096//2048//1024       //���������
#define     CON_GR47_RX_BUFFER_DEPTH    1024//800//700//1024//4096//2048//1024      //���������
#define     CON_SCI_TX_BUFFER_DEPTH     1024//4096//2048//1024      //���������
#define     CON_SCI_RX_BUFFER_DEPTH     1024//4096//2048//1024      //���������
#define     CON_SCI_DEBUG_BUFFER_DEPTH	64//256//����GPRS�������

#define     CON_DATA_SEND_BUFFER_DEPTH  1200//530       //cfh 10-04-08

#define     CON_AT_MAX_REVEIVE_PACKAGE  650//700//1500//750         //GPRS����֡700
#define     CON_AT_MAX_REVEIVE_DATA     CON_AT_MAX_REVEIVE_PACKAGE-40           //GPRS���ݸ���660
#define     CON_SMS_BUFFER_MAX          280//400            //���Ż���������
#define     CON_FRAMEDATA_LENGTH_MAX    650//1500//750//700         //���壭>ģ������������

#define     CON_AT_CMD_DEPTH            18              //AT����������
#define     CON_AT_DATALIST_DEPTH       6//3                //����2132���������3  AT�Ϸ����ݶ������cfh 10-04-12
#define     CON_TASK_SCI_MAX            6//3                //SCI��������������   cfh 10-04-12

#define     CON_FRAME_LENGTH_MIN        8


//��ʱʱ�䳣������,ÿ10MS����һ�� //azh 110928 �ĳ�100ms
//CON_CMD_DELAY_10MS����С��100ms ����100ms���� !! �����⣿��
#define     CON_CMD_DELAY               AT_DLY_1S//100     //����ʱ1s
#define     CON_CMD_DELAY_10MS          AT_DLY_100MS//1          //��ʱ10ms
#define     CON_CMD_DELAY_100MS         AT_DLY_100MS//1          //��ʱ10ms
#define     CON_AT_TIMEOUT_MAX          AT_DLY_5S//500     //AT���ʱ�趨��5��
#define     CON_GPRS_TIMEOUT_MAX        AT_DLY_90S//9000        //GPRS�������ʱ�趨��30s

#define     CON_AT_MC55_SILENCE         AT_DLY_500MS//50
#define     CON_AT_SILENCE_MAX          AT_DLY_400MS//40//20          //�����ַ���ֹͣʱ��100ms
#define     CON_TIMEER_GSMSTATE         AT_DLY_25S//AT_DLY_18S	//1800        //GSM״̬��ʱ��Ԥֵ
#define     CON_TIMEER_GPRSSTATE        AT_DLY_10M	//10*60*100   //GPRS״̬��ʱ��Ԥֵ30s
#define     CON_TIMEER_GPRSCMDSTATE     AT_DLY_2S	//200     //GPRSCMD״̬��ʱ��Ԥֵ5s
#define     CON_TIMEER_POWEROFF         AT_DLY_60S	//6000        //�ص綨ʱ��Ԥֵ1M
#define     CON_SMS_TIMEOUT_MAX         AT_DLY_30S	//3000

#define     CON_POWEROFF_DELAY          AT_DLY_60S//6000        //�ص綨ʱ��Ԥֵ1M

#define     CMD_GPRSDATA_AUTO_UP            0x01
#define     CMD_SMSDATA_AUTO_UP             0x02

#define     CON_AT_MAX_SEND_PACKAGE     600//512//700//1500//750//600//530       //cfh 10-04-08      //GPRS���ݰ�700
#define     CONTC35UPLISTNUM            4//8//50

//���Ų���
typedef struct
{
    unsigned char ucLen;
    unsigned char ucCode[16];      // ��վ����ASCII  8613516807125
}ST_SMS_PARAM,*PST_SMS_PARAM;

//IP����
typedef struct
{
    unsigned char ucIP[15];     //IP��ַ ASCII
    unsigned char ucPort[5];    //�˿ں� ASCII
}ST_IP_PARAM,*PST_IP_PARAM;

typedef union
{
    ST_SMS_PARAM        stSMSParam;
    ST_IP_PARAM         stIPParam;
}UN_CHANNEL_ADDR,*PUN_CHANNEL_ADDR;


//ͨ�������ṹ��
typedef struct
{
    unsigned char       ucType;//ͨ��ͨѶ��ʽ TYPE_COMUNNICATION_SMS/TYPE_COMUNNICATION_TCP/TYPE_COMUNNICATION_UDP
    UN_CHANNEL_ADDR     unChInfo;

}ST_COMMUNICATION_CHANNEL,*PST_COMMUNICATION_CHANNEL;

//����ͨѸ����
typedef struct
{
    unsigned char   ucSCALen;               // ����Ϣ�������ĺ���(SMSC��ַ) ����:����8613800571500
    unsigned char   ucAPNLen;
    unsigned char   SCA[16];
    unsigned char   cAPN[24];               //APN
}ST_COMMUNICATION_COMMON_PARAM,*PST_COMMUNICATION_COMMON_PARAM;

//PIN,PUK����
typedef struct
{
    unsigned char   ucIsPwdValid;       //�����Ƿ�����
    unsigned char   ucPinPwd[4];        //PIN ����
    unsigned char   ucPinPwdOld[4];     //�ɵ�PIN����
    unsigned char   ucPukPwd[8];        //PUK����
}ST_PIN_PUK,*PST_PIN_PUK;


//�����˻�����
typedef struct
{
    unsigned char   ucIsAccountValid;       //�����Ƿ�����
    unsigned char   ucUserNameLen;          //�û�������
    unsigned char   ucUserName[32];         //�û���
    unsigned char   ucPwdLen;               //���볤��
    unsigned char   ucPwd[32];              //����
}ST_PDP_ACCOUNT,*PST_PDP_ACCOUNT;

typedef struct
{
    unsigned char ucInforLength;        // ģ��汾��Ϣ����
    unsigned char ucInformation[32];        // ģ��汾��Ϣ
}ST_MODULE_INFOR,PST_MODULE_INFOR;

typedef struct _STTC35TASK
{
    unsigned char               ucChNo;             //ͨ����
    ST_COMMUNICATION_CHANNEL    unCurAddrInfo;      //��ǰͨѸ��ַ
    unsigned short              ucDataLength;       //���ݳ���
    unsigned char               pData[CON_AT_MAX_SEND_PACKAGE]; //���ݻ�����<256
}STTC35TASK,*PSTTC35TASK;

typedef struct _STTC35UPLIST
{
    unsigned char ucList_Head_P;
    unsigned char ucList_Tail_P;
    STTC35TASK  stTC35Task[CONTC35UPLISTNUM];
}STTC35UPLIST,*PSTTC35UPLIST;


/**************************************************************************/

//ģ��ͨѸ����
typedef struct
{
    ST_COMMUNICATION_COMMON_PARAM   stCommonParam;      //42
    ST_PIN_PUK                      stPINPUK;           //22
    ST_COMMUNICATION_CHANNEL        stCh[CON_COMUNNICATION_MAX_CHANNEL];    //22
    ST_PDP_ACCOUNT                  stPDPAccount;
    ST_MODULE_INFOR                 stModuleInfor;
    unsigned short                  usTimerParam;       //GPRS�������Ӷ�ʱ�����������ӣ�
    unsigned char                   ucIs32BytePDP;
    unsigned char                   ucGPRSServer;       //�Ƿ�����GPRSServer���ܣ�1��ʾ���ã�0��ʾ������

}ST_MODULE_COMM_PARAM,*PST_MODULE_COMM_PARAM;


/**************************************************************************/

typedef struct _MODULE_RUN_STATE
{
    unsigned int  ucHardWareVer;
    unsigned int  ucSortWareVer;
    unsigned char ucCurCommChannel;             //��ǰ���ڵ�ͨ��
    unsigned char ucSignIntension;              //�ź�ǿ��

    unsigned char bnouse1           :1;         //IIC��д����
    unsigned char bIsRing           :1;         //�Ƿ�������
    unsigned char bIsPWDError       :1;         //�������
    unsigned char bIsPUKLock        :1;         //PUK����
    unsigned char bIsPINLock        :1;         //PIN����
    unsigned char bIsGPRSConnected  :1;         //GPRS�Ƿ�������
    unsigned char bIsSIMExist       :1;         //�Ƿ��з�������(SIM������)
    unsigned char bIsExist          :1;         //�ֻ�ģ���Ƿ��쳣���Ƿ����

}ST_MODULE_RUN_STATE;

typedef struct _STMODPARACHINFO
{
    unsigned char   bIsPWDChanged:1;            //��������
    unsigned char   bIsAPNChanged:1;            //PDP�˻��Ƿ���Ч
    unsigned char   bIsAccountChanged:1;        //�˻��Ƿ���
    unsigned char   bIsCHInfoChanged:1;         //��ǰͨ�����øı�
    unsigned char   bIsSMSCenterChanged:1;      //��������
    unsigned char   bIsSMSNoChanged:1;          //���ź���
    unsigned char   bIsLocalIPChanged:1;        //����IP
    unsigned char   bIsSubMackChanged:1;        //��������

    unsigned char   bIsGateWayChanged:1;        //����
    unsigned char   bIsServerChanged:1;         //����������
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
*   ����������
*   > ͷָ�룽βָ��ʱ��������Ϊ��
*   > ������д���βָ������
*   > ���ݶ�����ͷָ�����
*   > βָ�룫1 �� ͷָ��ʱ����������
*********************************************************************/
typedef struct  _BUFFER_BLOCK
{
    unsigned short  usHeadPtr;      //ͷָ��
    unsigned short  usTailPtr;      //βָ��
    unsigned short  usMaxDepth;     //���������
    unsigned char*  pBuf;           //�洢��ָ��
}ST_SCI_BUFFER,*PST_SCI_BUFFER;

typedef struct  _ST_SEND_HANDLE//���ڼ������
{
	ST_SCI_BUFFER	stSendBuf;		
    unsigned char	dly;           //�������ʱ���ȥ��������
    
}ST_SEND_HANDLE,*PST_SEND_HANDLE;

//AZH 100302
//#define     PIN_IGT                     (1 << 1)    //P2.1
//#define     PIN_EMERGOFF                (1 << 2)    //P2.2
//#define     PIN_POWERON                 (1 << 20)   //P0.20

#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
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
//#define     PIN_NETLED                  (1 << 17)//P0.17  �����
//#define     NETLED_SET                  FIO1CLR = P1_GNET_LED
//#define     NETLED_CLR                  FIO1SET = P1_GNET_LED
#define     NETLED_SET                  GPRS_RLED_ON()
#define     NETLED_CLR                  GPRS_RLED_OFF()

//#define     PIN_DATALED                 (1 << 18)//P0.18  ���ݵ�
//#define     DATALED_SET                 FIO1CLR = P1_GDATA_LED
//#define     DATALED_CLR                 FIO1SET = P1_GDATA_LED
#define     DATALED_SET                 GPRS_GLED_ON()
#define     DATALED_CLR                 GPRS_GLED_OFF()



//#define       PIN_PORTK1_GSMCHECK         0x80//PI7

//#define   CON_MC55_EXIST              IO_PI//ģ���Ƿ�װ��Ϊ1��ʾû�а�װ��Ϊ0��ʾ��װ
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

__EXTERN STMODPARACHINFO        stModParaChInfo;    //ģ����ز����������

__EXTERN STTC35UPLIST       stTC35UpList;       //���Ͷ���
__EXTERN STTC35TASK         stTC35TaskTmp;      //���������ʱ��ȫ�ֱ������ǿ��Ǽ��ٶ�ջ��ʹ��
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
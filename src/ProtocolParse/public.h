/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : public.h
 Description    : ͨ�����ݽṹ����
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-02-05    azh     the original version
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
	RET_FILL_SAME,				//������ͬ��������д
	RET_MEM_OVER,				//�ڴ����

	RET_TOO_LITTLE,				//δ����С����
	RET_HEAD_ERROR,				//֡ͷ����
	RET_MID_END,				//��;����
	RET_NOENOUGH_LONG,			//����δ��

	RET_GRTASK_NEEDPROC,		//GR���������������Ҫִ��
	RET_GRTASK_NONEEDPROC,		//GR���������������Ҫִ��

	RET_OVERTIME,
	RET_RS485_TASKSOURCE_FULL,
	RET_RS485_TASKSOURCE_NULL,
	RET_TASK_EMPTY,				//����Ϊ��
	RET_TASK_NOEMPTY,			//����Ϊ�ǿ�
	RET_SPEC,
	RET_NEEDPROC,				//��Ҫ����
	RET_NEEDRESTORE,			//��Ҫ���ݻָ�
	RET_DATA_AVAILABLE,			//������Ч
	RET_NEED_JUDGE,				//��ҪԤ��
	RET_NONEED_JUDGE,			//����ҪԤ��

	RET_PASS_ERR,				//�������
	RET_SEQ_ERR,				//SEQ����
	RET_ADDR_ERR,				//��ַ����
	RET_CTRLB_ERR,				//�����ִ���
	RET_LEN_ERR,				//���ȴ���
	RET_REP_ERR,				//�������ʹ���

	RET_ENSURE_FINISH,			//ȷ�����
	RET_LCD_BUSY,				//LCD_Busy
	
	RET_ALR_HAPPEN,				//�澯����
	RET_ALR_RECOVER,			//�澯�ָ�

	RET_MCM_NONEEDSEND,			//����������(��Ҫ��� CommInfoList)
	RET_MCM_WAITSEND,			//�ȴ�����,(������� CommInfoList)
	RET_NEEDCLRGROUP,			//�������ͬ������Դ�Լ���ŵ�����
	RET_COMM_TIMEOVER,			//ͨ�ų�ʱ������
	RET_CHANGEPORTPARA,			//��Ҫ�ı�˿ڲ���

	RET_CONNECT_OK				//�յ����CONNECT
}ENURETVALUE;

#define		METER_NORMAL_FLAG		00	//���Թ�Լ���ͣ���ʾ�Ƿ�ʱ
#define		METER_OVERTIME_FLAG		11
#define		METER_MIDEND_FLAG		22


//azh 2�������� 2����������ռ���غ궨��
#define FLASHSECTOR_SIZE		        0x1000  //4K
#define MCP_FLASH_BLOCK_SIZE	        FLASHSECTOR_SIZE//0x20000
#define SECTORNUM_PERTASK               64      //4k*64=256K ÿ������������ռsector��Ŀ
//���¼�������������غ궨��
//�����ܺ�����800kÿ������Ϊ0.85k���ܼ�940�ݣ�����8���㣬ÿ������110��
//����˳���ն��ᣬ�����ݣ������ݹ̶�
//#define CON_DFPAGE_HISDATA_PAGENUM		96		//ÿ��96ҳ���̶�48k������56�ݣ��൱��6��+32��+13�£�
#define CON_DAYDATA_BLOCKNUM_PER_MP     9//(4K/850)*9=36��
#define CON_MONTHDATA_BLOCKNUM_PER_MP   4//(4K/850)*4=16��
#define CON_FROZEDATA_BLOCKNUM_PER_MP   3//(4K/850)*3=12��
#define CON_HISDATA_BLOCKNUM_PERMP      (CON_DAYDATA_BLOCKNUM_PER_MP+CON_MONTHDATA_BLOCKNUM_PER_MP+CON_FROZEDATA_BLOCKNUM_PER_MP)//(4K/850)*3=12��

//�ĳ� EN25Q64 2048*4k OR 128*64K
//--------------------------
#define CON_DFPAGE_HEAD_RESERVE1		0		//����8K��ҳ0-1
//�����������ݱ���ռ�
#define MCP_FLASH_TASK_START_BLOCK	    2       //4K*64*2(CON_GRTASK_NUM) ҳ2-129
#define CON_TASK_BLOCK_TOTNUM           (SECTORNUM_PERTASK*8)//azh CON_GRTASK_NUM=2 ������8Ԥ��6�� �����㲻�����Ķ�
//��Ҫ�澯��¼����ռ�
#define CON_IMPALARM_START_BLOCK		(MCP_FLASH_TASK_START_BLOCK+CON_TASK_BLOCK_TOTNUM)//130//����160K��ҳ130-169		�澯��־
#define CON_IMPALARM_BLOCK_TOTNUM       10//��Ϊ�澯��¼������һ���ֽ�256�� ������flash������������Ǳ䶯�� 4k/330=12*10=120���澯��¼
#define CON_IMPALARM_BLOCK_RESVER       20//����20
//һ��澯��¼����ռ�
#define CON_NORMALARM_START_BLOCK		(CON_IMPALARM_START_BLOCK+CON_IMPALARM_BLOCK_TOTNUM+CON_IMPALARM_BLOCK_RESVER)//170//����160K��ҳ170-209		�澯��־
#define CON_NORMALARM_BLOCK_TOTNUM      10
#define CON_NORMALARM_BLOCK_RESVER      20//����20
//���¼����������ݱ���ռ�
#define CON_DFPAGE_HEAD_HISDATA			(CON_NORMALARM_START_BLOCK+CON_NORMALARM_BLOCK_TOTNUM+CON_NORMALARM_BLOCK_RESVER)//210//����800K��ҳ210-409		��ʷ���������ݣ�������
#define CON_HISDATA_BLOCK_TOTNUM        (CON_HISDATA_BLOCKNUM_PERMP*8)//CON_GRTASK_NUM=2 ������8Ԥ��6��
//azh Ŀǰ�õ�0~722
#define MCP_FLASH_HEAD_TAIL			    2048	    //4K*2048=8M bytes ���ֵ�����ܴﵽ��ֵ*/

//4MƬ����Ƭ����һƬ0-8191���ڶ�Ƭ8192-16383
//DATAFLASH���������ֿ���ҳ��ţ��ܹ�16384ҳ
//#define CON_DFPAGE_HEAD_RESERVE1		0		//����16K��ҳ48-127
////#define CON_DFPAGE_HEAD_PROGRAM		128		//����960K��ҳ128-2047
//#define CON_DFPAGE_HEAD_DATASTART		0//2048	//������װ�����������
//#define CON_DFPAGE_HEAD_ALARM_IMP		1//2048	//����150K��ҳ2048-2347		�澯��־
//#define CON_DFPAGE_HEAD_ALARM_NORM		301//2348	//����150K��ҳ2348-2647		�澯��־
//#define CON_DFPAGE_HEAD_HISDATA			601//2648	//����800K��ҳ2648-4247		��ʷ���������ݣ�������
//#define CON_DFPAGE_HEAD_RESERVE2		2201//4248	//ҳ4248-8191
//#define CON_DFPAGE_HEAD_2ND_START		8192	//�ڶ�Ƭ��ʼ��ţ��������
//#define CON_DFPAGE_HEAD_DATABLOCK		8224	//����3456K��ҳ8224-15135	18��192k���ݿռ������
//#define CON_DFPAGE_HEAD_RESERVE3		15136	//ҳ15136-16383
//
//#define CON_DFPAGE_HEAD_MAX				8192	//��Ƭ���ֵ
//#define CON_DFPAGE_HEAD_TOTAL_MAX		16384	//���ֵ
//
//#define CON_DFPAGE_DATABLOCK			0x30000	//�ֽ���Ŀ192k
//#define CON_DFPAGE_DATABLOCK_NUM		18
//#define CON_DFPAGE_DATABLOCK_PAGENUM	384		//ÿ��384ҳ���̶����ɱ�

//�ն��Լ�������
#define	CON_TERERR0_RAM_INT			0x01	//1	Ƭ��RAM				//��0��
#define	CON_TERERR0_RAM_EXT			0x02	//2	Ƭ��RAM
#define	CON_TERERR0_DATAFLASH		0x04	//3	DATAFLASH
#define	CON_TERERR0_TIMER			0x08	//4	ʱ���쳣
#define	CON_TERERR1_BATTER			0x01	//9	��ص�ѹ��			//��1��
#define	CON_TERERR2_ERR1			0x01	//17	�쳣1			//��2��
#define	CON_TERERR3_ERR1			0x01	//25	�쳣1			//��3��
#define	CON_TERERR4_ERR1			0x01	//33	�쳣1			//��4��
#define	CON_TERERR5_ERR1			0x01	//41	�쳣1			//��5��
#define	CON_TERERR6_ERR1			0x01	//49	�쳣1			//��6��
#define	CON_TERERR7_ERR1			0x01	//57	�쳣1			//��7��

/********************************���ú���*********************************/
#define CON_STATUS_YES_2BYTE		0x55AA	//�����ֽڱ�ǣ���
#define CON_STATUS_NO_2BYTE			0xAA55	//�����ֽڱ�ǣ���
#define CON_STATUS_YES				0x55	//�ֽڱ�ǣ���
#define CON_STATUS_NO				0xAA	//�ֽڱ�ǣ���
#define CON_MIN_FLOAT_VAL			0.1

#define CON_STATUS_INIT		        0xA1A1	//�����ֽڱ�ǣ���ʼ״̬
#define CON_STATUS_START		    0xA2A2	//�����ֽڱ�ǣ�����״̬
#define CON_STATUS_FINISH		    0xA3A3	//�����ֽڱ�ǣ����״̬

/********************************ʱ�Ӻ���*********************************/
#define CON_TIMEUNIT_SEC			0x01	//��
#define CON_TIMEUNIT_MIN			0x02	//��
#define CON_TIMEUNIT_HOUR			0x03	//ʱ
#define CON_TIMEUNIT_DAY			0x04	//��
#define CON_TIMEUNIT_MON			0x05	//��

/********************************RS485TASK�ṹ����******************************/
#define		CONRS485BIGMAXLEN       255 //azh ���ڴ���м�����ĳ��� ��Ϊ������֡��200���ֽ�
#define		CONRS485DATAMAXLEN		96	//������������󳤶ȣ���Ҫ��֤��������ṹ��ż�����ֽ�
#define		CONRS485DATAMINLEN		24	//������ĳЩ�ṹ�������м������˿�����
#define		CON_SEQNO_NULL			0

//��������
#define		CON_RS485_REPORT_TYPE_IDLE		0		//�ձ���
#define		CON_RS485_REPORT_TYPE_SET		1		//���ñ���
#define		CON_RS485_REPORT_TYPE_GET		2		//��ѯ����
#define		CON_RS485_REPORT_TYPE_SPEC		3		//���ⱨ��
#define		CON_RS485_REPORT_TYPE_RELAY		4		//�м�����
#define		CON_RS485_REPORT_TYPE_EXT		5		//��չ����

#define		CON_RS485_PORT_NUM				2
#define 		CON_RELAY_NUM						4
typedef struct _STRELAYADDR
{
	unsigned char ucRelayAddr[6];
}STRELAYADDR;
typedef struct _STRELAYTASKPARA			//�м��������
{
	unsigned char	ucPortBaudRate;					//�����ʴ���
	unsigned char	ucRecvWaitTime;					//�м�������ճ�ʱʱ��(��)
	unsigned char	ucByteWaitTime;					//�м������ֽڳ�ʱʱ��(��)
	unsigned char	ucCharacterChar;				//�����ַ�
	unsigned char	ucReserve;						//�����ַ�
	unsigned char	ucCommInfo;						//ͨѶ�����ַ�
	unsigned short	usGetFrom;						//��ȡ��ʼ
	unsigned short	usGetLen;						//��ȡ����
	
	unsigned char ucFnVal;				//Fn
	unsigned char ucRelayNum;			//�м̼���
	unsigned char ucDIType;
	STRELAYADDR stRelayAddr[CON_RELAY_NUM];	//�м̵�ַ
	STRELAYADDR stDestAddr;
	unsigned long ulDI;
}STRELAYTASKPARA;

typedef struct _STRS485TASK
{
	unsigned char	ucSeqNo;						//�������
	unsigned char	ucTaskSource;					//������Դ
	unsigned short	usDataType;						//��������
	unsigned char	ucReportType;					//��������
	unsigned char	ucDevNo;						//�豸��
	unsigned char	ucDataLen;						//���ݳ���
	STRELAYTASKPARA stRelayPara;					//�м��������
	unsigned char   ucDataArea[CONRS485DATAMAXLEN];	//����������
}STRS485TASK;		//data_size+20=116 bytes

typedef struct _STRS485TASKNOR
{
	unsigned char	ucSeqNo;						//�������
	unsigned char	ucTaskSource;					//������Դ
	unsigned short	usDataType;						//��������
	unsigned char	ucReportType;					//��������
	unsigned char	ucDevNo;						//�豸��
	unsigned char	ucDataLen;						//���ݳ���
	unsigned char   ucDataArea[CONRS485DATAMINLEN];	//����������
}STRS485TASKNOR;		//data_size+20=116 bytes

typedef struct _STRS485BIG//�ռ��һ���
{
	unsigned char	ucSeqNo;						//�������
	unsigned char	ucTaskSource;					//������Դ
	unsigned short	usDataType;						//��������
	unsigned char	ucReportType;					//��������
	unsigned char	ucDevNo;						//�豸��
	unsigned char	ucDataLen;						//���ݳ���
	unsigned char   ucDataArea[CONRS485BIGMAXLEN];	//����������
}STRS485BIG;		//

typedef struct _STRELAYDATA//�ռ��һ���
{
	unsigned short	usDataLen;						//���ݳ���
	unsigned char   ucDataArea[CONRS485BIGMAXLEN];	//����������
}STRELAYDATA;
/********************************TerMsg�ṹ����******************************/
#define	CON_MAX_MPNUMDIV8			8
#define	CON_DI_DATA_LEN				96			//�����������������ݣ��м̿���ռ��8�������16*8����������Ŀ=128�ֽ�
#define	CON_DI_DATA_LEN_ALR			24//48//14//24//24//48			//�澯��¼
#define	CON_TERMSG_DI_NUM			8			//һ����Ϣ�������������Ŀ
#define	CON_INFOMSG_DI_NUM			4			//��Ϣ�������Ŀ

//azh ע�ⲻ����RS485��������Դ��ͻ!!!!!
//��Ϣ��Դ��
#define	CON_TERMSG_SOUR_ZIGBEE			9		//azh 171017��Դ��zigbee����ʽ��ѹ��GPRSģ��
#define	CON_TERMSG_SOUR_RS232_0			10		//azh ��Դ��RS485_2
#define	CON_TERMSG_SOUR_RS232			11		//��Դ��RS232
#define	CON_TERMSG_SOUR_RS485			12		//��Դ��RS485
#define CON_TERMSG_SOUR_TERSELF			13	
#define CON_TERMSG_SOUR_CSD				14		
#define CON_TERMSG_SOUR_IRDA			15	
#define CON_TERMSG_SOUR_AUD				16	
#define CON_TERMSG_SOUR_RCOMM_SMS		17		//����
#define CON_TERMSG_SOUR_RCOMM_TCP		18		//TCP
#define CON_TERMSG_SOUR_RCOMM_UDP		19		//UDP

#define CON_TERMSG_SOUR_RCOMM_1			20
#define CON_TERMSG_SOUR_RCOMM_2			21
#define CON_TERMSG_SOUR_RCOMM_3			22


#define	CON_TERMSG_SOUR_INVALID			0xFF
/*	ע�ⲻ����RS485��������Դ��ͻ
#define		CONRS485SOURTIMER				6		//��Դ��Ĭ�϶�ʱ��ȡ
#define		CONRS485SOUR_GRTASK				9		//��ԴGRTASK
#define		CONRS485SOUR_GRTASK_UNREAD		10		//��ԴGRTASK�������賭������ʷֱֵ����д�������Ӧ��
#define		CONRS485SOUR_ALRAFFIRMTASK		40		//��Դ�ڱ�����ȷ��	
#define		CONRS485SOURALR_1				50		//��Դ�ڱ�������Ĳ���
*/
//RS485��������Դ 
#define		CONRS485SOURTIMER				6		//��Դ��Ĭ�϶�ʱ��ȡ
#define		CONRS485SOUR_GRTASK				7//9		//��ԴGRTASK
#define		CONRS485SOUR_GRTASK_UNREAD		8//10		//��ԴGRTASK�������賭������ʷֱֵ����д�������Ӧ��
//		11-29����ΪTERMSG
#define		CONRS485SOUR_ALRAFFIRMTASK		40		//��Դ�ڱ�����ȷ��	
#define		CONRS485SOURALR_1				50		//��Դ�ڱ�������Ĳ���

//azh temp
#define CON_RS485CACHE_ASKOPE	        60
#define CON_RS485CACHE_RELAY	        61
#define CON_TER_SYNFLASH	            62
#define CON_RS485_645RELAY				63//�ⲿ�˿�ֱ��645����͸������ѹ������
#define CON_RELAYSOUR_RELAYTASK		    64	//�м�����

//#define CON_RS485ASKOPESOUR_TIMER		0x30	//��ʱ
#define CON_RS485ASKOPESOUR_TASK		65	//����
#define CON_RS485ASKOPESOUR_ALARMUP		66	//�����������ݳ���
#define CON_RS485ASKOPESOUR_ALARMFORE	67	//����ȷ�﷢��ĳ���

//azh

typedef struct _STDATAITEM						//������ṹ
{
	unsigned char	ucMPNo;		
	unsigned char	ucExeRet;
	unsigned short	usItemLabel;			//�������ʶ
	unsigned short	usDataLen;				//���ݳ���
	unsigned char*	ucDataContent;//ucDataContent[CON_DI_DATA_LEN];	//��������
}STDATAITEM;

typedef struct _STDATAITEM_ALR					//�澯������ṹ
{
	unsigned char	ucMPNo;		
	unsigned char	ucExeRet;
	unsigned short	usItemLabel;			//�������ʶ
	unsigned short	usDataLen;				//���ݳ���
	unsigned char	ucDataContent[CON_DI_DATA_LEN_ALR];	//��������
}STDATAITEM_ALR;

typedef struct _STTERMSG						//�ն���Ϣ�ṹ
{
	unsigned char	ucSeqNo;				//ͨѶ��Ϣ���б��
	unsigned char	ucMsgSource;			//��Ϣ��Դ
	unsigned char	ucMsgType;				//��Ϣ����
//	unsigned char	ucMsgObject;			//��Ϣִ�еĶ���,������������
	unsigned char	ucInfoItemNum;			//��Ϣ�����
	STDATAITEM		stInfoItem[CON_INFOMSG_DI_NUM];	//��Ϣ��
	unsigned char	ucDataItemNum;			//����������
	STDATAITEM		stDataItem[CON_TERMSG_DI_NUM];	//������
}STTERMSG;


typedef struct _STUSERIP						//�ն���̫��IP��ַ
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
__EXTERN unsigned char			g_ucEnergyCtrl_SumGNo;						//���������ܼ�����
__EXTERN unsigned char			g_ucPowerCtrl_SumGNo;						//���ʿ����ܼ�����

__EXTERN STDATETIME 				g_stAutoKeepPower;
__EXTERN STDATETIME 				g_stKeepPowerEnd;

//__EXTERN STPOWERCTRLCOUNT	stPowerCtrlCount;
__EXTERN unsigned char		ucPowLowerCtrlTime;								//0.5hΪ��λ���¸��صĿ���ʱ��
__EXTERN unsigned short		usPowLowerJumpAlrTime[4];						//�¸��صĸ澯ʱ�䣬�̶�4���ִΣ�ʵ��ֻ�����֣���λ0.5��

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

__EXTERN unsigned char g_ucDownloadType;					//��װ������߲���
__EXTERN unsigned char g_ucTerDP_State;					//�ն˳�����װ״̬
__EXTERN unsigned char g_ucDPEnd_RestartTime;			//��ֹ������װ�ȴ���λʱ�䣭��15s
__EXTERN unsigned char g_ucRAMReadTimes;					//RAM�����ʴ���
__EXTERN unsigned short g_usRAMWriteLifeCycle;			//RAMд����

__EXTERN unsigned char g_ucVerNo;
__EXTERN unsigned char g_ucParaCS;

__EXTERN STTERRUNPARA			stTerRunPara;
__EXTERN STTERTIMERVAL			stTerTimerVal;

//azh__EXTERN STADEDEMAND			stAdeDemand;			//�������
//__EXTERN STADEMETERENERGYVAR stAdeMeterEnergyVar;

//__EXTERN STRATEPERIOD		stRatePeriod[12];		//һ������Ϊ12��������
//__EXTERN STRATESET			stRateSet[5];			//һ�������5�׷���

//__EXTERN STSWITCHDELAY			stSwitchDelay[CON_SWITCHOUT_NUM];
//__EXTERN STSWITCHALARMINFO 		stSwitchAlarmInfo[CON_SWITCHOUT_NUM];

//__EXTERN STHISSUMVALGROUP stHisSumGPowTotal_Sta;
//__EXTERN STHISSUMVALGROUP stHisSumGPowFee1_Sta;
//__EXTERN STHISSUMVALGROUP stHisSumGPowFee2_Sta;
//__EXTERN STHISSUMVALGROUP stHisSumGPowFee3_Sta;
//__EXTERN STHISSUMVALGROUP stHisSumGPowFee4_Sta;

//__EXTERN STDATETIME			stPowerCtrl_RecoverTime[CONSWITCHCURVENUM];		//��Խ������£����ƻָ���ʱ�䣬�ͷ��ԣ�һ��Ϊʱ�ν���ʱ��

//__EXTERN STREMOTECTRLCOUNT stRemoteCtrlCount[CON_SWITCHOUT_NUM];
//__EXTERN STREMOTECTRLPARA  stRemoteCtrlPara[CON_SWITCHOUT_NUM];

//__EXTERN unsigned short	usPulseCountTotal[CON_PULSE_NUM];			//�������
//__EXTERN unsigned short	usPulseCountFee1[CON_PULSE_NUM];			//�����������1
//__EXTERN unsigned short	usPulseCountFee2[CON_PULSE_NUM];			//�����������2
//__EXTERN unsigned short	usPulseCountFee3[CON_PULSE_NUM];			//�����������3
//__EXTERN unsigned short	usPulseCountFee4[CON_PULSE_NUM];			//�����������4
//
//__EXTERN unsigned short	usPulseCountFeeT_Bak[CON_PULSE_NUM];		//�����������1
//__EXTERN unsigned short	usPulseCountFee1_Bak[CON_PULSE_NUM];		//�����������1STPULSEAUXINFO	stPulseAuxInfo[CON_PULSE_NUM];				//�������������Ϣ
//__EXTERN unsigned short	usPulseCountFee2_Bak[CON_PULSE_NUM];		//�����������2STPULSEMETERVAL stPulseMeterVal[CON_PULSE_NUM];				//�����������
//__EXTERN unsigned short	usPulseCountFee3_Bak[CON_PULSE_NUM];		//�����������3STPULSEMETERBASE stPulseMeterBase[CON_PULSE_NUM];			//����׶�
//__EXTERN unsigned short	usPulseCountFee4_Bak[CON_PULSE_NUM];		//�����������4

//__EXTERN STPULSEAUXINFO	stPulseAuxInfo[CON_PULSE_NUM];				//�������������Ϣ
//__EXTERN STPULSEMETERVAL stPulseMeterVal[CON_PULSE_NUM];				//�����������
//__EXTERN STPULSEMETERBASE stPulseMeterBase[CON_PULSE_NUM];			//����׶�

//__EXTERN STPULSEMETERVAL_LM stPulseMeterVal_LM[CON_PULSE_NUM];		//���������������

__EXTERN STHISRUNDATAINDEX 	stHisRunDataIndex[CON_METER_NUM];
__EXTERN STHISRUNDATA		stHisDayFrozenData[CON_METER_NUM];						//������
__EXTERN STHISRUNDATA		stHisDayData[CON_METER_NUM];
__EXTERN STHISRUNDATA		stHisMonthData[CON_METER_NUM];

//__EXTERN STHISSUMGROUPALLDATA	stHisSumGroupThisDay;							//�ܼ�	//��������������Ҫת��DATAFLASH
//__EXTERN STHISSUMGROUPALLDATA	stHisSumGroupThisMonth;
//__EXTERN STHISSUMGROUPDATAINDEX	stHisSumGroupDataIndex[CON_SUMGROUP_NUM];

__EXTERN STRS485TASK		    stRS485RecvExpainVal;		//���ս��ͽ��

//azh �������� �Ҹ�ѹ��ռ����һ��������
//__EXTERN STHISTERDATAINDEX		stHisTerDataIndex;									//�ն�
//__EXTERN STHISRUNDATA			stHisTerDayFrozenData;
//__EXTERN STHISRUNDATA			stHisTerDataThisDay;
//__EXTERN STHISRUNDATA			stHisTerDataThisMonth;

//__EXTERN STHISPULSEDATAINDEX		stHisPulseDataIndex[CON_PULSE_NUM];					//����
//__EXTERN STHISPULSEALLDATA		stHisPulseDayDataThisDay;
//__EXTERN STHISPULSEALLDATA		stHisPulseDayDataThisMonth;

//ADD by cfh 10-01-15	��ǰ�澯״̬����Ҫ�����

//__EXTERN STSUMALARMSTAT		stSumAlarmStatus[CON_SUMGROUP_NUM];			//��ǰ�澯״̬��Ϣ�б��ܼ��飩

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
__EXTERN unsigned long	GetPQRate(unsigned long lPower,unsigned long lVar);	//�õ���������ֵ����ʽxxx.xx
__EXTERN unsigned long	GetPQRate_Var4B(unsigned long lPower,unsigned long lVar);	//�õ���������ֵ����ʽxxx.xx
__EXTERN unsigned long	GetSPow(unsigned long lPower,unsigned long lVar);	//�õ����ڹ���
__EXTERN unsigned long	Adjust_Zero(unsigned long ulVal);
__EXTERN unsigned long	Get_Correct_Boadrate(unsigned long ulBoadrate);

__EXTERN unsigned char	ClearDateTime(STDATETIME *pstDateTime);
__EXTERN unsigned long BCD_TO_HEX_1(unsigned char *pucBCD);
__EXTERN unsigned char Check_BCD_MuiltData(unsigned char *pucData,unsigned char ucDataNum);
__EXTERN unsigned char CheckDateTime(STDATETIME *pstDateTime);
__EXTERN unsigned char ConvertHisVal0(unsigned long ulVal,unsigned char *pucData,unsigned char ucLen);
__EXTERN unsigned long GetPQRate_Var4B(unsigned long lPower,unsigned long lVar);	//�õ���������ֵ����ʽxxx.xx
__EXTERN unsigned long GetPQRate_Var4B_3P(unsigned long lPower,unsigned long lVar);	//�õ���������ֵ����ʽx.xxx
__EXTERN unsigned long	Adjust_Zero(unsigned long ulVal);
__EXTERN unsigned long GetSPow(unsigned long lPower,unsigned long lVar);	//�õ����ڹ���
__EXTERN STDATETIME	BYTE_TO_DATETIME(unsigned char *pucByte);
__EXTERN unsigned long	HEX_TO_BCD(unsigned long	lHex,unsigned char ucSize);
__EXTERN unsigned long BCD_TO_HEX_2(unsigned char *pucBCD);
__EXTERN unsigned long BCD_TO_HEX_4(unsigned char *pucBCD);
__EXTERN unsigned char Get_Sum(unsigned char *pBuffer,unsigned long ulLen);
__EXTERN unsigned long BCD_TO_HEX_3(unsigned char *pucBCD);
__EXTERN unsigned char FillData_EE(unsigned char *pucData,unsigned char ucLen);
#endif//_PUBLIC_H_

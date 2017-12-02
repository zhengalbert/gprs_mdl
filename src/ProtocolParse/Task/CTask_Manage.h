/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : CTask_Manage.c
 Description    : ��������ģ���������
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-08-15    azh     the original version
******************************************************************************/
#ifndef _CTASK_MANAGE_H
#define _CTASK_MANAGE_H

#define CON_TASK_FRAME_RESEND_MAX	3	    //������������֡����ط�����

#define CON_TASK_RETDATA_VALID		0x55
#define CON_TASK_RETDATA_INVALID	0xAA
#define CON_TIME_TASK_WAIT_PROCESS  0xA1//��ʱ���������
#define CON_TASK_RETDATA_PROCESS   	0xA5//AZH ���ݱ�ʶ���ڳ�����û�����


//����ֵ��������
#define CON_GRTASK_NUM				2//18		//���18���������ݿ黮��������
#define CON_GRTASK_METER_TASKNUM	1			//������8   //�������������0~(������-1)
#define CON_GRTASK_TER_TASKNO		CON_GRTASK_METER_TASKNUM+0		//�ն˽����ɼ����� ������������


#define	CON_GRTASK_STATFLAG_NUM		32		//����״̬����ֽ���

//-----------------------------------------------------------
//azh ���������������
#define CON_TASK_PARA_SINGLE_SAVE   0
#define CON_TASK_PARA_BLOCK_SAVE    0xA5
#define CON_TASK_CURVE_DI_NUM       22        //��������Ҫ�������ݱ�ʶ

#define CON_TASK_FRAME_RESEND_MAX	3	    //������������֡����ط�����

#define CON_GRTASK_STATE_VALID		0x11	//�����Ч����ʱ�õĺ� ��һ���Ǽ���״̬
#define CON_GRTASK_STATE_ACTIVE		0x22	//�����Ч����ʱ�õĺ� ���Ҽ���״̬

#define CON_TASK_CONFIG_ACTIVESTATE             0//ǰ��״̬��ͬ �Ż���¼������´�ʱ��
#define CON_TASK_RESTART_ACTIVESTATE            1//�������� ֱ�Ӹ��¼������´�ʱ��

#define CON_GRTASK_PARA_VALID           0xa55a
//#define CON_GRTASK_PARA_BASE_ADDR		0x61005400  //temp
//#define CON_GRTASK_PARA_BASE_BOLCK		0x1000      //128*32�ɱ���32���������
#define CON_GRTASK_PER_SPACE    		128         //ÿ��������������Ŀռ��С
//---
#define CON_TASK_BLK_OFST               0
#define CON_TASK_TYPE_OFST              CON_TASK_BLK_OFST//ucTaskType//��������
#define CON_TASK_TYPE_LENTH             1            
#define CON_TASK_MPNO_OFST              (CON_TASK_TYPE_OFST+CON_TASK_TYPE_LENTH)//ucMPNo��ͨ����Ĳ������&�м�����Ķ˿ں�
#define CON_TASK_MPNO_LENTH             1          
#define CON_TASK_SAVENUM_OFST           (CON_TASK_MPNO_OFST+CON_TASK_MPNO_LENTH)//usSavePointNum����ı������(��ʹ���趨ֵ��ʹ����������õ�ֵ)
#define CON_TASK_SAVENUM_LENTH          2                                          
#define CON_TASK_PERRETLEN_OFST         (CON_TASK_SAVENUM_OFST+CON_TASK_SAVENUM_LENTH)//usPerTaskRetMem����ִ��һ��������ֽ���
#define CON_TASK_PERRETLEN_LENTH        2
#define CON_TASK_ACTIVE_OFST            (CON_TASK_PERRETLEN_OFST+CON_TASK_PERRETLEN_LENTH)//ucActive//����״̬������
#define CON_TASK_ACTIVE_LENTH           1          
#define CON_TASK_STARTACTIVE_OFST       (CON_TASK_ACTIVE_OFST+CON_TASK_ACTIVE_LENTH)//stStartActive//����ʼ�ʱ��
#define CON_TASK_STARTACTIVE_LENTH      6          
#define CON_TASK_BLKCRC_LENTH           (13+1)//1crc
//---
#define CON_TASKIF_BLK_OFST             (CON_TASK_BLK_OFST+CON_TASK_BLKCRC_LENTH)
#define CON_TASKIF_EXEDNUM_OFST         CON_TASKIF_BLK_OFST//usExedNum//����ִ���˴���
#define CON_TASKIF_EXEDNUM_LENTH        2          
#define CON_TASKIF_TAILPOS_OFST         (CON_TASKIF_EXEDNUM_OFST+CON_TASKIF_EXEDNUM_LENTH)//usTailPos//��������ݵ㣬������ѭ���洢usSavePointNum����
#define CON_TASKIF_TAILPOS_LENTH        2        
#define CON_TASKIF_TAILTIME_OFST        (CON_TASKIF_TAILPOS_OFST+CON_TASKIF_TAILPOS_LENTH)//ulTailTime//��������ݵ��Ӧ��ʱ�䣬��1990�꿪ʼ����λ����
#define CON_TASKIF_TAILTIME_LENTH       4          
#define CON_TASKIF_BLKCRC_LENTH         (8+1)//1crc
//---
#define CON_TASKRIF_BLK_OFST            (CON_TASKIF_BLK_OFST+CON_TASKIF_BLKCRC_LENTH)
#define CON_TASKRIF_NEXTEXTIME_OFST     CON_TASKRIF_BLK_OFST//stNextExeTime//�´�ִ��ʱ��(����)
#define CON_TASKRIF_NEXTEXTIME_LENTH    6
#define CON_TASKRIF_NEXTUPTIME_OFST     (CON_TASKRIF_NEXTEXTIME_OFST+CON_TASKRIF_NEXTEXTIME_LENTH)//stNextUpTime//�´���������ʱ��
#define CON_TASKRIF_NEXTUPTIME_LENTH    6
#define CON_TASKRIF_HEADPOS_OFST        (CON_TASKRIF_NEXTUPTIME_OFST+CON_TASKRIF_NEXTUPTIME_LENTH)//usHeadPos//��ǰ�洢�ĵ��
#define CON_TASKRIF_HEADPOS_LENTH       2
#define CON_TASKRIF_BLKCRC_LENTH        (14+1)//1crc
//
#define CON_TASKINFO_BLKADDR_END        (CON_TASKRIF_HEADPOS_OFST+CON_TASKRIF_HEADPOS_LENTH)    //�����������в�����ĩ��ַ

//#define CON_TASK_BEGINPAGE_OFST       (CON_GRTASK_NEXTUPTIME_OFST+CON_GRTASK_NEXTUPTIME_LENTH)//������ʼҳ
//#define CON_TASK_BEGINPAGE_LENTH      2    
//#define CON_TASK_BEGINPOS_OFST        (CON_GRTASK_BEGINPAGE_OFST+CON_GRTASK_BEGINPAGE_LENTH)//������ʼҳ��ַ
//#define CON_TASK_BEGINPOS_LENTH       2   
//#define CON_TASK_BEGIN_BLKCRC_LENTH   (4+1)//1crc 

//Ĭ�ϵ������� �������ɲ�������������� 
//-----------------------------------------------------------------------------------
#define	CON_GRTASK_DINUM			65//32		//���������
#define CON_GRTASK_DATANUM			64		//�������ֽ���

#define CON_GRTASK_DATA_INVALID		0xEE	//��Ч����
#define CON_DATAFLSH_INIT_DATA		0xFF	//δ����״̬

//��������
#define CON_GRTASK_TYPE_NORMAL		0x01	//��ͨ����
#define CON_GRTASK_TYPE_RELAY		0x02	//�м�����
#define CON_GRTASK_TYPE_INVALID		0xFF	//��Ч����
//azh
#define CON_GRTASK_TaskDateInfoLen		6	//һ������������ϢԤ���Ĵ洢�ռ䣬����������ִ��ʱ��
#define CON_GRTASK_TaskDateRELAYLen		96	//�м�������������ݴ洢�ռ�(������Ϣ������)
#define CON_GRTASK_TaskDateMaxDateLen	512	//һ������ִ��һ�����Ĵ洢�ռ�

//�����Ϊ
//	��8��ռ��8������
//	�ܼ�ռ��4������
//	����4·ռ��4������
//	�ն˲ɼ�ռ��1������
//	����ʣ��1������
//	�ϼ�18������ÿ�������������̶�192k����3456k
//#define CON_GRTASK_METER_TASKNUM				1			//������8
//#define CON_GRTASK_SUMGROUP_INSTANTPOW_TASKNO	CON_GRTASK_METER_TASKNUM+0		//�й������ܼ�����
//#define CON_GRTASK_SUMGROUP_INSTANTVAR_TASKNO	CON_GRTASK_METER_TASKNUM+1		//�޹������ܼ�����
//#define CON_GRTASK_SUMGROUP_POW_TASKNO			CON_GRTASK_METER_TASKNUM+2		//�й������ܼ�����
//#define CON_GRTASK_SUMGROUP_VAR_TASKNO			CON_GRTASK_METER_TASKNUM+3		//�޹������ܼ�����

//#define CON_GRTASK_PULSE_1_TASKNO			CON_GRTASK_METER_TASKNUM+4		//����1����
//#define CON_GRTASK_PULSE_2_TASKNO			CON_GRTASK_METER_TASKNUM+5		//����2����
//#define CON_GRTASK_PULSE_3_TASKNO			CON_GRTASK_METER_TASKNUM+6		//����2����
//#define CON_GRTASK_PULSE_4_TASKNO			CON_GRTASK_METER_TASKNUM+7		//����2����
//#define CON_GRTASK_TER_TASKNO				CON_GRTASK_METER_TASKNUM+0		//�ն˽����ɼ�����
//#define CON_GRTASK_USERDEF_START_TASKNO		CON_GRTASK_METER_TASKNUM+9		//�û����Զ�������

#define CON_GRTASK_DATAPOINT_NUM			3072		//32��15����һ�㣬��3072�㣬ÿ��64�ֽ�
#define CON_GRTASK_DATA_TOTAL_SECONDS		5529600//2764800		//32��2764800��
//#define CON_GRTASK_DATAPOINT_NUM			1152		//12��15����һ�㣬��1152�㣬ÿ��48�ֽ�
//#define CON_GRTASK_DATA_TOTAL_SECONDS		1036800		//12��1036800��

#define CON_MP_NUM				2			//�ն˲�����������벻Ҫ���Ĵ˳���		//��ʱ������
#define CON_MP_INVALID			0			//��Ч������
#define CON_MP_VALID			1			//��Ч������	

#define CON_METERCOPY_CHECK_TIME            3000//5���� ��λms

typedef struct _STGRTASKEXEINFO
{
	unsigned char ucNowExeTaskNo;	//��ǰ��ִ�е������
	unsigned char ucTaskValidFlag;	//���񷵻�������Ч���
//azh
    unsigned char ucTaskType;       //��ǰ��������
    unsigned char unInCom;          //�м�����Ļ��߲��������ڵ��ڲ��˿ں�
    unsigned short usValidCnt;      //����(��ʱ���� �������� �澯����)��Чʱ����� ��ucTaskValidFlagЧʱ��ļ��
}STGRTASKEXEINFO;

typedef struct _STSENDDILIST
{
	unsigned char ucDINum;
	unsigned short usDILabel[CON_GRTASK_DINUM];
//azh
	unsigned char	ucDIPtr;        //��������
    unsigned char   ucReSendnum;    //ÿ�����ݱ�ʶ�س�����
}STSENDDILIST;

typedef struct _STTIMEINFO
{
	unsigned char	ucTimeInter;			//ʱ����
	unsigned char	ucTimeUnit;				//ʱ�䵥λ
}STTIMEINFO;

typedef struct _STDIVAL
{
	unsigned short	usDI;					//�������ʶ
	unsigned char	ucDILen;				//�������������ݳ���
//	unsigned char	ucDIVAL[96];			//��������������
}STDIVAL;

typedef struct _STGRTASKRETVAL
{
	unsigned char	ucTaskNo;				//�������
	STDATETIME		stRetDateTime;			//���ݻ�Ӧ��ʱ��
	unsigned char	ucDINum;				//��������
	STDIVAL			stDIVal[CON_GRTASK_DINUM];		//��Ӧ����ŵ�����������
//azh ����һ�������������ݵĻ���
	unsigned char   ucTaskData[CON_GRTASK_TaskDateMaxDateLen];
}STGRTASKRETVAL;

#define CON_GRTASK_ACTIVE			0x55
#define CON_GRTASK_NOACTIVE			0xAA

#define CON_TASK_RETDATA_SENDED		0x55		//�ѱ�����
#define CON_TASK_RETDATA_NOSEND		0xAA		//��δ������

//#define	 CON_TASK_MAXBUFFER			256//8192		//�������漰������װ������4k
//#define	 CON_BUFFER_CLASS2_DIFF		512
//#define	 CON_BUFFER_CLASS3_DIFF		1024
#define	 CON_TASK_DATA_MAXBUFFER	512//1024 ����ֻ��һ����������//azh �˴��ڴ��Ϊ1536 ��ȥһЩ֡ǰ֡�� ������ǰ���ֽ�

typedef struct _STGRTASK				//GRϵͳ����
{
	unsigned char	ucTaskType;						//��������
	unsigned char	ucMPNo;							//��ͨ����Ĳ������&�м�����Ķ˿ں�
	STTIMEINFO		stTITaskStart;					//������ʼ��׼ʱ��
	STTIMEINFO		stTITaskInter;					//�������ʱ��
	STTIMEINFO		stTIResendStart;				//���ͻ�׼ʱ��
	STTIMEINFO		stTIReSendInter;				//���ͼ��ʱ��
	unsigned char	ucResendDataInter;				//�������ݵ�Ƶ��		
	unsigned char	ucDataItemNum;					//��ͨ���������������&�м�������м������
	unsigned short	usSavePointNum;					//����ı������(��ʹ���趨ֵ��ʹ����������õ�ֵ)
	unsigned short	usExeNum;						//����ִ�д���
//azh	STRELAYTASKPARA stRelayTaskPara;				//�м̲���
//azh Ϊ�˽�ʡram ����������Ĭ������ ����������Ĺ̶��� �ñ������
//	unsigned char	ucDataArea[CON_GRTASK_DATANUM];	//������������б�

	unsigned short	usPerTaskRetMem;				//����ִ��һ��������ֽ���
	unsigned char	ucActive;						//����״̬������
	STDATETIME		stStartActive;					//����ʼ�ʱ��
}STGRTASK;			//С��128�ֽ�

typedef struct _STGRTASKRUNINFO						//����������Ϣ��������������Ϣ����õ�
{
	STDATETIME		stNextExeTime;					//�´�ִ��ʱ��(����)
	STDATETIME		stNextUpTime;					//�´���������ʱ��
	unsigned short	usHeadPos;						//��ǰ�洢�ĵ��
}STGRTASKRUNINFO;

typedef struct _STGRTASKINFO						//��������Ϣ����Ҫ����DATAFLASH
{
	unsigned short	usExedNum;						//����ִ���˴���
	unsigned short	usTailPos;						//��������ݵ㣬������ѭ���洢usSavePointNum����
	unsigned long	ulTailTime;						//��������ݵ��Ӧ��ʱ�䣬��1990�꿪ʼ����λ����
}STGRTASKINFO;		//����Ϊ8���ֽ�			//64*8=512�����Դ���DATAFLASH��һҳ

//azh
typedef struct _STGRDEFINEAREA				//�������ݶ�����
{
	STGRTASK		stGRTask[CON_GRTASK_NUM];		//�����б�
	STGRTASKINFO	stGRTaskInfo[CON_GRTASK_NUM];	//��������Ϣ�б�
	STGRTASKRUNINFO stGRTaskRunInfo[CON_GRTASK_NUM];//����������Ϣ�б�
	STGRTASKRETVAL	stGRTaskRetVal;					//�����Ӧ��������һ������һ��ʱ���Ķ�������
}STGRDEFINEAREA;	//8k�ֽ�

//--------------------------------------------------------------------------
__EXTERN STGRDEFINEAREA			stGRDefineArea;				//8k�ֽ�
__EXTERN STGRTASKEXEINFO		stGRTaskExeInfo;
__EXTERN STSENDDILIST           stSendDIList;
//--------------------------------------------------------------------------
//������Ƴ�ʼ��
__EXTERN unsigned char Task_Manage_Init(void);
//�������������ܵ�������������Ӧ���ݵĿռ���䶼�ǹ̶��ģ���ˣ��������������Ĵ����ȹ�����
__EXTERN unsigned char Task_Config(unsigned char ucTaskNo,unsigned char ucActive);
//�������
__EXTERN unsigned char Task_Clear(unsigned char ucTaskNo);
//��ȡ����ʱ������ݳ���
unsigned short Task_GetPerTimeSpace(unsigned char ucTaskNo);
//��ȡ����ʱ������ݳ���
__EXTERN unsigned short Task_GetPerTimeSpaceLittle(unsigned char ucTaskNo);
//�����Ӧ���ݴ洢
__EXTERN unsigned char Task_SaveRetVal(unsigned char ucTaskNo,unsigned char *pucRetData,unsigned short usRetDataLen);
//�����Ӧ���ݲ�ѯ
__EXTERN unsigned char Task_QueryRetVal(unsigned char ucTaskNo,unsigned char *pucRetData,STDATETIME *pstStartTime,unsigned short usPointNum,unsigned char ucResendDataInter,unsigned short *pusOnePointLen);
__EXTERN unsigned char Task_QueryRetVal_ByDI(unsigned char ucTaskNo,unsigned char *pucRetData,STDATETIME *pstStartTime,unsigned short usPointNum,unsigned char ucResendDataInter,unsigned char *pucDataLen,unsigned short usDI);
//azh 170925 ��Ӧ376.1-2013��Ķ�������F219:���������������������
__EXTERN unsigned char Task_QueryRetVal_ByGroupDI(unsigned char ucTaskNo,unsigned char *pucRetData,STDATETIME *pstStartTime,unsigned short usPointNum,unsigned char ucResendDataInter,unsigned char *pucDataLen,unsigned short usDI);

//�����Ӧ���ݲ�ѯ��ȡ����ȡ�������
//__EXTERN unsigned char Task_QueryRetVal_Newest(unsigned char ucTaskNo,unsigned char *pucRetData,unsigned short usPointNum,unsigned char ucResendDataInter,STDATETIME *pstNewestTime,unsigned short *pusOnePointLen);

//����ִ��ʱ����������ʱ�����Լ����͵�ѡ��Ƶ�ʻ�ȡ�������͵ĵ���
//__EXTERN unsigned short Task_GetUpSendPointNum(unsigned char ucTaskNo);

//����������Ϣ
__EXTERN unsigned char Task_CheckAllTaskInfo(void);
//�������������Ϣ
__EXTERN unsigned char Task_GetTaskRunInfo(void);

//���ִ��ʱ��
__EXTERN STDATETIME	GetTaskNextExeTime(unsigned char ucTaskNo);
//�������ʱ��
__EXTERN STDATETIME	GetTaskNextUpTime(unsigned char ucTaskNo);
__EXTERN STDATETIME	Task_FixNextTime(STDATETIME stDatetime,STTIMEINFO stStartTimeInfo,STTIMEINFO stInterTimeInfo);

__EXTERN unsigned char TaskCheckMeterIsValid(unsigned char ucMeterNo);
__EXTERN unsigned char SetDefaultMeterTask(unsigned char ucMeterNo);		//����Ĭ������
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
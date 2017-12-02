/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Meter_Protocol_Public.h
 Description    : �������
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-02-05    azh     the original version
******************************************************************************/
#ifndef _METER_PROTOCOL_PUBLIC_H_
#define	_METER_PROTOCOL_PUBLIC_H_

#define		CON_METER_NUM_PRO	2//8
#define		CONCOMMAND_BIG		0x55
#define		CONCOMMAND_SML		0xAA

#define		CONCOMMAND_SEND		0x55
#define		CONCOMMAND_NOSEND	0xAA

#define		CON_CMDLIST_NUM		150//100		//100��������ˣ����� cfh 10-04-04

#define		CONALRTME_VALID_FLAG		0x55
#define		CONALRTME_INVALID_FLAG		0xAA
#define		CONALRAFR_RESP_FLAG			0xBB
/****************��ͨ�Ź�Լ��Ŷ���*******************/		
#define		CON_PROTOCOL_INVALID	0		//��Ч			//�ڲ�����ͨ�Ź�Լ����
#define		CON_PROTOCOL_DL645		1		//645��Լ
#define		CON_PROTOCOL_SAMPLE		2		//�����ɼ�

#define		CON_PROTOCOL_ABB_R		3		//ABB����
#define		CON_PROTOCOL_LANDD		4		//������D��
#define		CON_PROTOCOL_LANDB		5		//������B��
//#define		CON_PROTOCOL_GENIUS		6		//����
#define		CON_PROTOCOL_AS1_DL645	6		//645��Լ����һ(���棨�๦�ܣ�)
#define		CON_PROTOCOL_WS			7		//��ʤ4.x


#define		CON_PROTOCOL_HND		10		//������

#define		CON_PROTOCOL_AS2_DL645	15		//645��Լ���ƶ�(���棨���׶๦�ܣ�)
#define		CON_PROTOCOL_HB_PT		17		//����֮��ͨ����ʽ���ܱ�
#define		CON_PROTOCOL_HB_JM		18		//����֮���񸴷��ʵ��ܱ�

#define		CON_PROTOCOL_HLi		21		//�㽭���� (645)
#define		CON_PROTOCOL_KL			22		//��½ (645)
#define		CON_PROTOCOL_LD			23		//���� (645)
#define		CON_PROTOCOL_SKD		24		//��ƴ� (645)
#define		CON_PROTOCOL_JSLY		25		//�������� (645)
#define		CON_PROTOCOL_HND645		26		//������ (645)
#define		CON_PROTOCOL_DFDZ		27		//�������� (645)
#define		CON_PROTOCOL_JQ			28		//��ȸ (645)

#define		CON_PROTOCOL_DL64507	30

#define		CON_PROTOCOL_MAX		71		//�����ڲ�������Ϊ71
/*******************************************************/

#define	CONRECVEXPLAINNUM	12//azh 100			//�������ת�������
#define	CONRELAYCOMMANDLEN	70
#define	CON645RELAYCOMMANDLEN	500			//�м̳�������500�ֽ�

#define RS485_CH_ID			1			//uart ͨ�����
#define	RS485_CH_ID2		4
/*
#define	RS485_BYTE_OVER_TIME		1600	//��λ0.25*4ms	1.6��
#define	RS485_RECV_OVER_TIME		2000	//��λ0.25*4ms	2��

#define	RS485_RECV_DELAY			120	//��λ0.25ms   30ms
#define RS485_SEND_DELAY			80	//��λ0.25ms   20ms
*/
#define	RS485_BYTE_OVER_TIME		160	//��λ10ms	1.6��
#define	RS485_RECV_OVER_TIME		200	//��λ10ms	2��

#define	RS485_RECV_DELAY			3	//��λ10ms   30ms
#define RS485_SEND_DELAY			2//20//2	//��λ10ms   20ms		changed for test 09-11-24

#define CONFRONTBYTENUM		0			//֡ǰ���ֽڸ���
#define CONFRONTBYTEVALUE	0xFE		//֡ǰ���ֽ�

typedef struct _STCMDFLAG
{
	unsigned char	ucDirPowerFlag;										//�����й�������
	unsigned char	ucUnDirPowerFlag;									//�����й�������
	unsigned char	ucDirVarFlag;										//�����޹�������
	unsigned char	ucUnDirVarFlag;										//�����޹�������
	unsigned char	ucVar1Flag;											//һ�����޹�������
	unsigned char	ucVar2Flag;											//�������޹�������
	unsigned char	ucVar3Flag;											//�������޹�������
	unsigned char	ucVar4Flag;											//�������޹�������
	unsigned char	ucDirPowerMaxDemandFlag;				//�����й�����������ݿ�
	unsigned char	ucDirPowerMaxDemandTimeFlag;		//�����й������������ʱ�����ݿ�
	unsigned char	ucDirVarMaxDemandFlag;					//�����޹�����������ݿ�
	unsigned char	ucDirVarMaxDemandTimeFlag;			//�����޹������������ʱ�����ݿ�
	unsigned char	ucUnDirPowerMaxDemandFlag;			//�����й�����������ݿ�
	unsigned char	ucUnDirPowerMaxDemandTimeFlag;	//�����й������������ʱ�����ݿ�
	unsigned char	ucUnDirVarMaxDemandFlag;				//�����޹�����������ݿ�
	unsigned char	ucUnDirVarMaxDemandTimeFlag;		//�����޹������������ʱ�����ݿ�

	unsigned char	ucDirPowerLmFlag;								//�����й�������(����)                
	unsigned char	ucUnDirPowerLmFlag;             //�����й�������(����)                    
	unsigned char	ucDirVarLmFlag;                 //�����޹�������(����)                   
	unsigned char	ucUnDirVarLmFlag;               //�����޹�������(����)                    
	unsigned char	ucVar1LmFlag;                   //һ�����޹�������(����)                  
	unsigned char	ucVar2LmFlag;                   //�������޹�������(����)                  
	unsigned char	ucVar3LmFlag;                   //�������޹�������(����)                  
	unsigned char	ucVar4LmFlag;                   //�������޹�������(����)                  
	unsigned char	ucDirPowerMaxDemandLmFlag;      //�����й�����������ݿ�(����)            
	unsigned char	ucDirPowerMaxDemandTimeLmFlag;  //�����й������������ʱ�����ݿ�(����)    
	unsigned char	ucDirVarMaxDemandLmFlag;        //�����޹�����������ݿ�(����)            
	unsigned char	ucDirVarMaxDemandTimeLmFlag;    //�����޹������������ʱ�����ݿ�(����)    
	unsigned char	ucUnDirPowerMaxDemandLmFlag;    //�����й�����������ݿ�(����)           
	unsigned char	ucUnDirPowerMaxDemandTimeLmFlag;//�����й������������ʱ�����ݿ�(����)    
	unsigned char	ucUnDirVarMaxDemandLmFlag;      //�����޹�����������ݿ�(����)            
	unsigned char	ucUnDirVarMaxDemandTimeLmFlag;  //�����޹������������ʱ�����ݿ�(����)    

	unsigned char	ucVolFlag;						//��ѹ
	unsigned char	ucCurFlag;						//����
	unsigned char	ucInstantPowFlag;				//�й�����
	unsigned char	ucInstantVarFlag;               //�޹�����
	unsigned char	ucProgramTimeFlag;				//���ʱ�䡢��ع���ʱ�����Ϣ������07��Լ�ͱ�ʾ��ع���ʱ��
	unsigned char	ucPulseConstFlag;               //���峣��
	unsigned char	ucMeterStatFlag;				//���״̬��
	unsigned char	ucCopyDayTimeFlag;              //������
	unsigned char	ucLostVolNumFlag;               //�������
	unsigned char	ucLostVolTimeFlag;              //����ʱ��
	unsigned char	ucLastVolOffBegTimeFlag;        //���һ�ζ��࿪ʼʱ��
	unsigned char	ucLastVolOffEndTimeFlag;        //���һ�ζ������ʱ��
	unsigned char	ucFeeRateFlag;					//ʱ�α�
	unsigned char	ucDateTimeFlag;                 //����ʱ��

	unsigned char	ucProgramNumFlag;				//����ܴ���
	unsigned char	ucLastProgramTimeFlag;			//���һ�α��ʱ��
	unsigned char	ucMeterclrNumFlag;				//�����0�ܴ���
	unsigned char	ucLastMeterclrTimeFlag;			//���һ�ε����0ʱ��
	unsigned char	ucDemondclrNumFlag;				//������0�ܴ���
	unsigned char	ucLastDemondclrTimeFlag;		//���һ��������0ʱ��
	unsigned char	ucEventclrNumFlag;				//�¼���0�ܴ���
	unsigned char	ucLastEventclrTimeFlag;			//���һ���¼���0ʱ��
	unsigned char	ucJiaoshiNumFlag;				//Уʱ�ܴ���
	unsigned char	ucLastJiaoshiTimeFlag;			//���һ��Уʱʱ��
	unsigned char ucFeCuFlag;						//ͭ���������

	unsigned char ucDirPowAFlag;	
	unsigned char ucDirPowBFlag;
	unsigned char ucDirPowCFlag;

	unsigned char ucDirPowALmFlag;	
	unsigned char ucDirPowBLmFlag;
	unsigned char ucDirPowCLmFlag;
}STCMDFLAG; 

/********************************************************/
//��Ʋ��������Ƶ�ַ�Ķ�Ӧ��
typedef struct _STMPNOADDRREC
{
	unsigned char	ucMPNo;
	unsigned char	ucProtocolNo;
	unsigned char	ucMeterPass[8];
	unsigned char	ucMeterAddr[6];
	unsigned char	ucMeterCommName[8];
}STMPNOADDRREC;
/*
#define CON_METER_PASSLEN		8		//���볤��
#define	CON_METER_ADDRLEN		6		//����ַ����
#define CON_METER_NAMELEN		8		//ͨѸ������
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
__EXTERN STRS485TASK stRS485TaskRecvExplain[CONRECVEXPLAINNUM];   //���ܽ���
__EXTERN STMPNOADDRLIST	stMPNoAddrList;
__EXTERN unsigned char	ucErrCount;			//�ط�����������ABB��˵��WHO ARE YOU ������ �� ��CLASS �������ǿ��ط��ģ����࣬�����������ǲ����ط���

/********************************************************/
//azh
////��ʼ����Լ���Ͷ���
__EXTERN unsigned char Meter_Protocol_Init(STMPNOADDRLIST	*pstMPNoAddrList);
//
////���ͽ���
__EXTERN unsigned char Meter_Protocol_SendExplain(STRS485TASK *pstRS485Task,unsigned char *pSendBuffer,unsigned short *pusLen);
//
////���ս���
__EXTERN unsigned char Meter_Protocol_RecvExplain(STRS485TASK *pstRS485SendTask,
								unsigned char	*pRecvBuffer,
								unsigned short	usRecvLen,
								unsigned char	ucOverTimeFlag,
								unsigned char	*pucExplainResultNum);
////���շ���
__EXTERN unsigned char Meter_Protocol_GetExplainResult(unsigned char ucNo,STRS485TASK *pstRS485RecvExpainVal);
//
////֡У��
__EXTERN unsigned char Meter_Protocol_CheckFrame(STRS485TASK *pstRS485Task,unsigned char *pRecvBuffer,unsigned short usRecvLen,unsigned short *pusJumpLen);
//
////����շ����Ƴ���
//unsigned char Meter_Protocol_GetTimerConst(STRS485TASK *pstRS485Task,unsigned char *pucChID,
//								   unsigned short *pusMaxRecvOverTime,unsigned short *pusMaxByteOverTime,
//								   unsigned short *pusMaxSendDelay);
//
////��ʱ15�������ݳ���
//unsigned char Meter_Protocol_GetTimerCommand(STCMDFLAG	stCmdFlag,STCMDFLAG stIsSendFlag,STRS485TASKCMDLIST	*pstRS485TaskCmdList);
//unsigned char Meter_GetReSendContent(STRS485TASK *pstRS485SendTask,STRS485TASK *pstRS485RecvTask,STRS485TASK *pstRS485RetTask);
//unsigned char Meter_Protocol_DispartOverTime(STRS485TASK *pstRS485SendTask,STRS485TASK *pstRS485RecvTask,unsigned char *pucExplainResultNum);
//unsigned char Meter_Protocol_GetPortPara(STRS485TASK *pstRS485Task,unsigned long *pulBaudRate,unsigned char *pucEPN,unsigned char *pucDataBit,unsigned char *pucStopBit);
//unsigned char Get_ChID(unsigned char ucPortNo);
__EXTERN unsigned char Get_MeterNo_byMPNo(unsigned char ucMPNo);
#endif

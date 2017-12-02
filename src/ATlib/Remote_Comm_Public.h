#ifndef _REMOTECOMM_PUBLIC_H
#define _REMOTECOMM_PUBLIC_H

#define 	CON_TC35COMM_RECV_DATA_LEN		1200//1400//1024//2176

#define		CON_SMSBUFF_MAXLEN		140
//#define		CON_SMSBUFF_RESULTLEN	100//552//1104				//138*4=552
//#define		CON_SMS_NUM				1//8
//#define		CON_SMSUNITE_NUM		1//3

#define 	CON_TC35COMM_TCP_LimitLen			460+22+1		//TCPͨ��������󳤶�Ϊ540,��ԼΪ512�ֽڣ�ʵ���޶�460
#define 	CON_TC35COMM_UDP_LimitLen			220+22+1		//UDPͨ��������󳤶�Ϊ540����սֻʶ��255�ֽ�
#define 	CON_TC35COMM_SMS_LimitLen			140				//SMSͨ��������󳤶�

//ͨѶ��ʽ
#define CON_TC35COMMTYPE_INVALID	0x00
#define CON_TC35COMMTYPE_GSM		0x01
#define CON_TC35COMMTYPE_TCP		0x02
#define CON_TC35COMMTYPE_UDP		0x03

#define	CON_PIN_LOCK					0x55
#define	CON_PIN_UNLOCK					0xAA	//δ����

#define	CON_RSSI_BAR_4			31
#define	CON_RSSI_BAR_3			18
#define	CON_RSSI_BAR_2			10
#define	CON_RSSI_BAR_1			5

typedef struct _STREMOTERUNINFO{
	unsigned char	ucSignIntension;	//�ź�ǿ��	
	unsigned char	ucCurrChanType;		//��ǰͨ��ͨ������
	unsigned char	ucPinStat;			//PIN���������
	unsigned char	ucRCommStat;		//ģ��ͨѸ״̬
	unsigned char   ucGprsLastLoginStat; //�ϴε�¼״̬
	unsigned char   ucGprsLoginStat;	//��¼״̬		�ڻ��ģʽ����ָclient�����ӡ���������Ƿ������޹�
	unsigned char	ucRCommStatBak1;	//ģ��ͨѸ״̬����1
	unsigned char	ucRCommStatBak2;	//ģ��ͨѸ״̬����2
	unsigned char   ucModHardVer[4];	//ģ��Ӳ���汾��Ϣ
	unsigned char   ucModSoftVer[4];	//ģ������汾��Ϣ
}STREMOTERUNINFO;	


typedef struct _STRCOMMRECEDATA
{
	unsigned short usHead_P;
	unsigned short usTail_P;
	unsigned char ucDataArea[CON_TC35COMM_RECV_DATA_LEN];		//����������
}STRCOMMRECEDATA;

typedef struct _STSMSBUFF
{
	unsigned short usSmlSeq;						//����֡���
	unsigned char  ucDataLen;						//����	
	unsigned char  ucBuff[CON_SMSBUFF_MAXLEN];		//����Ϣ����
}STSMSBUFF;
//typedef struct _STSMSUNITEINFO
//{
//	STDATETIME	   stDateTime_Recvstart;			//�յ���һ������ʱ��
//	unsigned short usSmsSeq;						//����֡���
//	unsigned short usSmlSeq_End;					//���Ž���֡���
//	unsigned short usSmlSeq_RecvNum;					//������Ŀ
//	STSMSBUFF	   stSMSBuf[CON_SMS_NUM];			
//}STSMSUNITEINFO;
//---------------------------------------------------------
//Gprs��ر���
__EXTERN STRCOMMRECEDATA	stGprsDataBuf;		//��TCP��UDP��ͬһ�������������ն���ͬʱʹ��TCP��UDP�����ӻ������ֿ�����
//SMS��ر��� ������
//__EXTERN STSMSUNITEINFO	stSMSUniteInfo[CON_SMSUNITE_NUM];
//__EXTERN unsigned char	ucSMSResponseBuff[CON_SMSBUFF_RESULTLEN];	//���Ϻ����ݻ���
//__EXTERN STDATETIME	gstCurrDateTime;		//�ն˵�ǰʱ��
//--------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///����: ��ʼ����ģ���һЩ����������
//__EXTERN void			RComm_Initial(unsigned long ulBoadrate);		//��ʼ������
__EXTERN void			RComm_Initial(void);		//��ʼ������
__EXTERN unsigned char	RComm_FillUpData(unsigned char *pBuf,unsigned short usLen,unsigned char ucMsgSource);
//__EXTERN unsigned char RComm_AddUpSendTask(unsigned char *pucSendBuffer,unsigned short usLen,unsigned char ucChanNo);

#endif 


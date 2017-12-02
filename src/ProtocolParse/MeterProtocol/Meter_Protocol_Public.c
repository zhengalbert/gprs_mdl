/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Meter_Protocol_Public.c
 Description    : ����ģ���Լ���ʹ���
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-02-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

//˽�к�������
static unsigned char _Meter_Protocol_Init(STMPNOADDRLIST	*pstMPNoAddrList);
static unsigned char _Meter_Protocol_SendExplain(STRS485TASK *pstRS485Task,unsigned char *pSendBuffer,unsigned short *pusLen);
static unsigned char _Meter_Protocol_RecvExplain(STRS485TASK *pstRS485SendTask,unsigned char *pRecvBuffer,unsigned short usRecvLen,unsigned char ucOverTimeFlag,unsigned char *pucExplainResultNum);
static unsigned char _Meter_Protocol_GetExplainResult(unsigned char ucNo,STRS485TASK *pstRS485RecvExpainVal);
static unsigned char _Meter_Protocol_CheckFrame(STRS485TASK *pstRS485Task,unsigned char *pRecvBuffer,unsigned short usRecvLen,unsigned short *pusJumpLen);
//unsigned char _Meter_Protocol_GetTimerConst(STRS485TASK *pstRS485Task,unsigned char *pucChID,unsigned short *pusMaxRecvOverTime,unsigned short *pusMaxByteOverTime,unsigned short *pusMaxSendDelay);
//unsigned char _Meter_Protocol_GetTimerCommand(STCMDFLAG	stCmdFlag,STCMDFLAG stIsSendFlag,STRS485TASKCMDLIST	*pstRS485TaskCmdList);
//static unsigned char _Meter_GetReSendContent(STRS485TASK *pstRS485SendTask,STRS485TASK *pstRS485RecvTask,STRS485TASK *pstRS485RetTask);
//static unsigned char _Meter_Protocol_DispartOverTime(STRS485TASK *pstRS485SendTask,STRS485TASK *pstRS485RecvTask,unsigned char *pucExplainResultNum);
//static unsigned char _Meter_Protocol_GetPortPara(STRS485TASK *pstRS485Task,unsigned long *pulBaudRate,unsigned char *pucEPN,unsigned char *pucDataBit,unsigned char *pucStopBit);

//unsigned short GetCheckCode_16BitCRC(unsigned char *ptr,unsigned short len);
//static unsigned char  Get_MeterNo_byMPNo(unsigned char ucMPNo);

//////////////////////////////// ����ӳ�� //////////////////////////////////////
//////////////������ ȷ����ӳ�亯��λ�ڳ���ǰ�棬�Ҵ��򲻵ø��� ������///////////////
//////////////////// ������ת��ڱ��̶ֹ������ָ���lib�Ķ�������ڲ��� //////////////////////
//1
unsigned char Meter_Protocol_Init(STMPNOADDRLIST	*pstMPNoAddrList)
{return(_Meter_Protocol_Init(pstMPNoAddrList));}
//2
unsigned char Meter_Protocol_SendExplain(STRS485TASK *pstRS485Task,unsigned char *pSendBuffer,unsigned short *pusLen)
{return(_Meter_Protocol_SendExplain(pstRS485Task,pSendBuffer,pusLen));}
//3
unsigned char Meter_Protocol_RecvExplain(STRS485TASK *pstRS485SendTask,unsigned char *pRecvBuffer,unsigned short usRecvLen,unsigned char ucOverTimeFlag,unsigned char *pucExplainResultNum)
{return(_Meter_Protocol_RecvExplain(pstRS485SendTask,pRecvBuffer,usRecvLen,ucOverTimeFlag,pucExplainResultNum));}
//4
unsigned char Meter_Protocol_GetExplainResult(unsigned char ucNo,STRS485TASK *pstRS485RecvExpainVal)
{return(_Meter_Protocol_GetExplainResult(ucNo,pstRS485RecvExpainVal));}
//5
unsigned char Meter_Protocol_CheckFrame(STRS485TASK *pstRS485Task,unsigned char *pRecvBuffer,unsigned short usRecvLen,unsigned short *pusJumpLen)
{return(_Meter_Protocol_CheckFrame(pstRS485Task,pRecvBuffer,usRecvLen,pusJumpLen));}
//6
//unsigned char Meter_Protocol_GetTimerConst(STRS485TASK *pstRS485Task,unsigned char *pucChID,unsigned short *pusMaxRecvOverTime,unsigned short *pusMaxByteOverTime,unsigned short *pusMaxSendDelay)
//{return(_Meter_Protocol_GetTimerConst(pstRS485Task,pucChID,pusMaxRecvOverTime,pusMaxByteOverTime,pusMaxSendDelay));}
//7
//unsigned char Meter_Protocol_GetTimerCommand(STCMDFLAG	stCmdFlag,STCMDFLAG stIsSendFlag,STRS485TASKCMDLIST	*pstRS485TaskCmdList)
//{return(_Meter_Protocol_GetTimerCommand(stCmdFlag,stIsSendFlag,pstRS485TaskCmdList));}
//8
//unsigned char Meter_GetReSendContent(STRS485TASK *pstRS485SendTask,STRS485TASK *pstRS485RecvTask,STRS485TASK *pstRS485RetTask)
//{return(_Meter_GetReSendContent(pstRS485SendTask,pstRS485RecvTask,pstRS485RetTask));}
//9
//unsigned char Meter_Protocol_DispartOverTime(STRS485TASK *pstRS485SendTask,STRS485TASK *pstRS485RecvTask,unsigned char *pucExplainResultNum)
//{return(_Meter_Protocol_DispartOverTime(pstRS485SendTask,pstRS485RecvTask,pucExplainResultNum));}
//10
//unsigned char Meter_Protocol_GetPortPara(STRS485TASK *pstRS485Task,unsigned long *pulBaudRate,unsigned char *pucEPN,unsigned char *pucDataBit,unsigned char *pucStopBit)
//{return(_Meter_Protocol_GetPortPara(pstRS485Task,pulBaudRate,pucEPN,pucDataBit,pucStopBit));}
//////////////������ ȷ����ӳ�亯��λ�ڳ���ǰ�棬�����ϴ��򲻵ø��� ������///////////////


//////////////////////////////// ����ʵ�� //////////////////////////////////////
//��ʼ����Լ���Ͷ���
unsigned char _Meter_Protocol_Init(STMPNOADDRLIST	*pstMPNoAddrList)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char i=0;
	for(i=0;i<CONRECVEXPLAINNUM;i++)
		ClearRS485Task(&(stRS485TaskRecvExplain[i]));

	if( pstMPNoAddrList != NULL)
		stMPNoAddrList	 = (*pstMPNoAddrList);

	ucErrCount = 0;

	return ucRet;
}

//����Ϊ����ǰ�ı���
unsigned char _Meter_Protocol_SendExplain(STRS485TASK *pstRS485Task,unsigned char *pSendBuffer,unsigned short *pusLen)
{
	unsigned char ucRet=RET_ERROR;
	unsigned char m=0;
	unsigned char ucMeterNo=0xFF;
//	unsigned char ucCheck;

	if(pstRS485Task->ucReportType==CON_RS485_REPORT_TYPE_RELAY)
	{//�м̴���
			if( pstRS485Task->ucDataLen < CONRS485BIGMAXLEN)//CONRS485DATAMAXLEN)
			{
				memcpy(pSendBuffer,pstRS485Task->ucDataArea,pstRS485Task->ucDataLen);
				m += pstRS485Task->ucDataLen;
			}
			(*pusLen) = m;
			if( m>0 )
				ucRet=RET_SUCCESS;	
	}else
	{
		ucMeterNo = Get_MeterNo_byMPNo(pstRS485Task->ucDevNo);
		if(ucMeterNo!=0xFF)
		{
			switch(stMPNoAddrList.stMPNoAddrRec[ucMeterNo].ucProtocolNo)
			{
//			case	CON_PROTOCOL_DL645:			//645��Լ
//			case	CON_PROTOCOL_AS1_DL645:		//645��Լ����һ
//			case	CON_PROTOCOL_AS2_DL645:		//645��Լ���ƶ�
//				
//			case	CON_PROTOCOL_HLi:			//�㽭���� (645)
//			case	CON_PROTOCOL_KL:			//��½ (645)
//			case	CON_PROTOCOL_LD:			//���� (645)
//			case	CON_PROTOCOL_SKD:			//��ƴ� (645)
//			case	CON_PROTOCOL_JSLY:			//�������� (645)
//			case	CON_PROTOCOL_HND645:			//������ (645)
//			case	CON_PROTOCOL_DFDZ:			//�������� (645)
//			case	CON_PROTOCOL_JQ:			//��ȸ (645)
//				m=Meter_DI_Type_DownExplain_Manage(pstRS485Task,ucMeterNo,pSendBuffer,m);
//				if(m>0)		ucRet = RET_SUCCESS;
//				break;
//			case	CON_PROTOCOL_HB_PT:	//����֮��ͨ����ʽ���ܱ�
//				m=HBPT_DI_Type_DownExplain_Manage(pstRS485Task,ucMeterNo,pSendBuffer,m);
//				if(m>0) 	ucRet = RET_NEEDCLRGROUP;				
//				break;
//			case	CON_PROTOCOL_HB_JM:	//����֮���񸴷��ʵ��ܱ�
//				m=HBJM_DI_Type_DownExplain_Manage(pstRS485Task,ucMeterNo,pSendBuffer,m);
//				if(m>0) 	ucRet = RET_NEEDCLRGROUP;				
//				break;	
//			/*
//			case	CON_PROTOCOL_ABB_R:		//ABB����
//				m=ABBR_DI_Type_DownExplain_Manage(pstRS485Task,ucMeterNo,pSendBuffer,m);
//				if(m>0) 	ucRet = RET_NEEDCLRGROUP;
//				break;
//			case	CON_PROTOCOL_LANDB:		//������B��
//				m=LANDB_DI_Type_DownExplain_Manage(pstRS485Task,ucMeterNo,pSendBuffer,m);
//				if(m>0) 	ucRet = RET_NEEDCLRGROUP;				
//				break;
//			case	CON_PROTOCOL_LANDD:	//������D��
//				m=LANDD_DI_Type_DownExplain_Manage(pstRS485Task,ucMeterNo,pSendBuffer,m);
//				if(m>0) 	ucRet = RET_NEEDCLRGROUP;				
//				break;
//			case	CON_PROTOCOL_WS	:	//��ʤ��Լ
//				m=WS_DI_Type_DownExplain_Manage(pstRS485Task,ucMeterNo,pSendBuffer,m);
//				if(m>0) 	ucRet = RET_SUCCESS;				
//				break;				
//			case	CON_PROTOCOL_GENIUS://����
//				m=Genius_DI_Type_DownExplain_Manage(pstRS485Task,ucMeterNo,pSendBuffer,m);
//				if(m>0) 	ucRet = RET_NEEDCLRGROUP;				
//				break;		
//			case      CON_PROTOCOL_HND://������
//				m=HND_DI_Type_DownExplain_Manage(pstRS485Task,ucMeterNo,pSendBuffer,m);
//				if(m>0) 	ucRet = RET_NEEDCLRGROUP;
//				break;
//			*/
			case	CON_PROTOCOL_DL64507:			//645��Լ2007 yzy
				m=Meter_DI64507_DownExplain_Manage(pstRS485Task,ucMeterNo,pSendBuffer,m);
				if(m>0)		ucRet = RET_SUCCESS;
				break;
			case	CON_PROTOCOL_INVALID://��Ч��Լ
				break;
			}
			(*pusLen) = m;
		}
	}
	return ucRet;
}
//�м̽��մ�����
unsigned char RS485_RecvRelay_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned short usBeginPos=0,usEndPos=0,i=0,usDataLen;
	
	SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,
		pstRS485SendTask->ucReportType,	pstRS485SendTask->usDataType,
		pstRS485SendTask->ucDevNo,&(pstRS485SendTask->stRelayPara));
	memset(stRelayDataBuf.ucDataArea,0,CONRS485BIGMAXLEN);
	
	if( usRecvLen<CON645RELAYCOMMANDLEN )
	{
		if( pstRS485SendTask->stRelayPara.ucCharacterChar==0x00 )
		{//������Ϊ��,�����ǽ�ȡ
			usBeginPos=0;
			usEndPos=usRecvLen;			//ע��ע��ע��
		}else
		{
			for( i=0;i<usRecvLen;i++)
			{//���������� 
				if( pucRecvBuffer[i]==pstRS485SendTask->stRelayPara.ucCharacterChar )
					break;
			}
			if(i>=usRecvLen)
			{//û���ҵ�������
				usBeginPos=0xFFFF;
			}else
			{
				usBeginPos=i;
				if( usBeginPos+pstRS485SendTask->stRelayPara.usGetFrom>usRecvLen )
					usBeginPos=0xFFFF;
				else
					usBeginPos+=pstRS485SendTask->stRelayPara.usGetFrom;

				if(usBeginPos!=0xFFFF)
				{
					if(pstRS485SendTask->stRelayPara.usGetLen==0)
						usEndPos=usRecvLen;
					else
					{
						usEndPos=usBeginPos+pstRS485SendTask->stRelayPara.usGetLen;
						if(usEndPos>usRecvLen)
							usEndPos=usRecvLen;
					}
				}
			}
		}
	}else
		usBeginPos=0xFFFF;

	if(usBeginPos!=0xFFFF)
	{
		usDataLen=usEndPos-usBeginPos;
		stRS485TaskRecvExplain[0].ucDataLen=0xFF;
//		stRS485TaskRecvExplain[0].ucDataArea[0]=(unsigned char)usDataLen;
//		stRS485TaskRecvExplain[0].ucDataArea[1]=(unsigned char)(usDataLen>>8);
//		memcpy(stRS485TaskRecvExplain[0].ucDataArea+2,pucRecvBuffer+usBeginPos,usDataLen);
//azh �����������
        if(usDataLen > CONRS485BIGMAXLEN) usDataLen = CONRS485BIGMAXLEN;
        stRelayDataBuf.usDataLen = usDataLen;
        memcpy(stRelayDataBuf.ucDataArea,pucRecvBuffer+usBeginPos,usDataLen);

	}else
	{
//		stRS485TaskRecvExplain[0].ucDataLen=0;
//		memset(stRS485TaskRecvExplain[0].ucDataArea,0,CONRS485DATAMAXLEN);
        stRelayDataBuf.usDataLen = 0;
	}
	(*pucRecvNum)=1;		//�����1������
	return ucRet;
}
//���ͽ��յı���
//	����ɱ�׼���������������ȫ����ԼΪ����������׼
unsigned char _Meter_Protocol_RecvExplain(STRS485TASK *pstRS485SendTask,
								unsigned char *pRecvBuffer,
								unsigned short usRecvLen,
								unsigned char ucOverTimeFlag,
								unsigned char *pucExplainResultNum)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMeterNo=0;

	if(pstRS485SendTask==NULL)
		return RET_ERROR;
	if(pstRS485SendTask->ucReportType==CON_RS485_REPORT_TYPE_RELAY)//�м�����
	{
//		if(pstRS485SendTask->stRelayPara.ucFnVal == 1)			
			ucRet=RS485_RecvRelay_Explain(pstRS485SendTask,pRecvBuffer,usRecvLen,pucExplainResultNum);
//		else if(pstRS485SendTask->stRelayPara.ucFnVal == 9)
//		{
//			if(ucOverTimeFlag==METER_OVERTIME_FLAG)
//				ucRet=RS485_Recv_OverTime_Explain(pstRS485SendTask,pRecvBuffer,usRecvLen,pucExplainResultNum);
//			else
//				ucRet = RS485_RecvRelay_ExplainF9(pstRS485SendTask,pRecvBuffer, usRecvLen, pucExplainResultNum);
//		}
	}
	else
	{
//azh		if(ucOverTimeFlag==METER_OVERTIME_FLAG)
//			ucRet=RS485_Recv_OverTime_Explain(pstRS485SendTask,pRecvBuffer,usRecvLen,pucExplainResultNum);
//		else
//		{
			ucMeterNo = Get_MeterNo_byMPNo(pstRS485SendTask->ucDevNo);
			if(ucMeterNo!=0xFF)
			{
				switch(stMPNoAddrList.stMPNoAddrRec[ucMeterNo].ucProtocolNo)
				{
//				case	CON_PROTOCOL_DL645:	//645��Լ
//				case	CON_PROTOCOL_AS1_DL645:		//645��Լ����һ
//				case	CON_PROTOCOL_AS2_DL645:		//645��Լ���ƶ�
//
//				case	CON_PROTOCOL_HLi:		//�㽭���� (645)
//				case	CON_PROTOCOL_KL:		//��½ (645)
//				case	CON_PROTOCOL_LD:		//���� (645)
//				case	CON_PROTOCOL_SKD:		//��ƴ� (645)
//				case	CON_PROTOCOL_JSLY:		//�������� (645)
//				case	CON_PROTOCOL_HND645:		//������ (645)
//				case	CON_PROTOCOL_DFDZ:		//�������� (645)
//				case	CON_PROTOCOL_JQ:		//��ȸ (645)
//					ucRet= Meter_DI_Type_UpExplain_Manage(pstRS485SendTask,pRecvBuffer,usRecvLen,pucExplainResultNum);
//					break;
//				case	CON_PROTOCOL_HB_PT	:	//����֮��ͨ����ʽ���ܱ�
//					ucRet= HBPT_DI_Type_UpExplain_Manage(pstRS485SendTask,pRecvBuffer,usRecvLen,pucExplainResultNum);
//					break;
//				case	CON_PROTOCOL_HB_JM	:	//����֮���񸴷��ʵ��ܱ�
//					ucRet= HBJM_DI_Type_UpExplain_Manage(pstRS485SendTask,pRecvBuffer,usRecvLen,pucExplainResultNum);
//					break;
				/*
				case	CON_PROTOCOL_ABB_R:	//ABB����
					ucRet= ABBR_DI_Type_UpExplain_Manage(pstRS485SendTask,pRecvBuffer,usRecvLen,pucExplainResultNum);
					break;
				case	CON_PROTOCOL_LANDD://������D��
					ucRet= LANDD_DI_Type_UpExplain_Manage(pstRS485SendTask,pRecvBuffer,usRecvLen,pucExplainResultNum);
					break;
				case	CON_PROTOCOL_LANDB://������B��
					ucRet= LANDB_DI_Type_UpExplain_Manage(pstRS485SendTask,pRecvBuffer,usRecvLen,pucExplainResultNum);
					break;
				case	CON_PROTOCOL_WS	:	//��ʤ��Լ
					ucRet= WS_DI_Type_UpExplain_Manage(pstRS485SendTask,pRecvBuffer,usRecvLen,pucExplainResultNum);
					break;
				case	CON_PROTOCOL_GENIUS	:	//�����Լ
					ucRet= Genius_DI_Type_UpExplain_Manage(pstRS485SendTask,pRecvBuffer,usRecvLen,pucExplainResultNum);
					break;
				case      CON_PROTOCOL_HND://������
				      ucRet= HND_DI_Type_UpExplain_Manage(pstRS485SendTask,pRecvBuffer,usRecvLen,pucExplainResultNum);
					break;
				*/
				case	CON_PROTOCOL_DL64507:	//645��Լ2007 yzy
					ucRet= Meter_DI64507_UpExplain_Manage(pstRS485SendTask,pRecvBuffer,usRecvLen,pucExplainResultNum);
					break;
				case	CON_PROTOCOL_INVALID:
					ucRet= RET_ERROR;
					break;
				}
			}
//		}
	}

	return ucRet;
}

//��÷���
unsigned char _Meter_Protocol_GetExplainResult(unsigned char ucNo,STRS485TASK *pstRS485RecvExpainVal)
{
	unsigned char ucRet=RET_SUCCESS;
//	unsigned char usDataLen;
	if(pstRS485RecvExpainVal!=NULL)
	{
		if((ucNo==0) && (stRS485TaskRecvExplain[0].ucReportType==CON_RS485_REPORT_TYPE_RELAY))	//�м�����
		{
			memcpy((unsigned char *)pstRS485RecvExpainVal,(unsigned char *)(&(stRS485TaskRecvExplain[0])),sizeof(STRS485TASK));
//			usDataLen=stRS485TaskRecvExplain[0].ucDataArea[0]+0x100*stRS485TaskRecvExplain[0].ucDataArea[1]+2;
//			memcpy(pstRS485RecvExpainVal->ucDataArea,stRS485TaskRecvExplain[0].ucDataArea,usDataLen);
		}else
		{
			if(ucNo<CONRECVEXPLAINNUM)
			{
				SetRS485Task(pstRS485RecvExpainVal,&(stRS485TaskRecvExplain[ucNo]));
			}else
				ucRet=RET_ERROR;
		}

	}
	return	ucRet;
}

//����֡��ʽ���
//	���֡���ȣ�֡ͷβ��У���
//	����ERROR��ʾ��֡����ȫ������
unsigned char _Meter_Protocol_CheckFrame(STRS485TASK *pstRS485Task,unsigned char *pRecvBuffer,unsigned short usRecvLen,unsigned short *pusJumpLen )
{
	unsigned char ucRet = RET_ERROR,ucMeterNo=0;
	ucMeterNo = Get_MeterNo_byMPNo(pstRS485Task->ucDevNo);
	if(ucMeterNo!=0xFF)
	{
		switch(stMPNoAddrList.stMPNoAddrRec[ucMeterNo].ucProtocolNo)
		{
		case	CON_PROTOCOL_DL645:		//645��Լ
//		case	CON_PROTOCOL_AS1_DL645:		//645��Լ����һ
//		case	CON_PROTOCOL_AS2_DL645:		//645��Լ���ƶ�
//
//		case	CON_PROTOCOL_HLi:		//�㽭���� (645)
//		case	CON_PROTOCOL_KL:		//��½ (645)
//		case	CON_PROTOCOL_LD:		//���� (645)
//		case	CON_PROTOCOL_SKD:		//��ƴ� (645)
//		case	CON_PROTOCOL_JSLY:		//�������� (645)
//		case	CON_PROTOCOL_HND645:		//������ (645)
//		case	CON_PROTOCOL_DFDZ:		//�������� (645)
//		case	CON_PROTOCOL_JQ:		//��ȸ (645)
		case	CON_PROTOCOL_DL64507:		//645��Լ2007 yzy
			ucRet = DL645_Protocol_CheckFrame(pRecvBuffer,usRecvLen,pusJumpLen);
			break;
//		case	CON_PROTOCOL_HB_PT	:	//����֮��ͨ����ʽ���ܱ�
//			ucRet = HBPT_Protocol_CheckFrame(pstRS485Task,pRecvBuffer,usRecvLen,pusJumpLen);
//			break;
//		case	CON_PROTOCOL_HB_JM	:	//����֮���񸴷��ʵ��ܱ�
//			ucRet = HBJM_Protocol_CheckFrame(pstRS485Task,pRecvBuffer,usRecvLen,pusJumpLen);
//			break;
		/*
		case	CON_PROTOCOL_ABB_R:		//ABB����
			ucRet = ABBR_Protocol_CheckFrame(pstRS485Task,pRecvBuffer,usRecvLen,pusJumpLen);
			break;
		case	CON_PROTOCOL_LANDB:		//������B��
			ucRet = LANDB_Protocol_CheckFrame(pstRS485Task,pRecvBuffer,usRecvLen,pusJumpLen);
			break;
		case	CON_PROTOCOL_LANDD:		//������D��
			ucRet = LANDD_Protocol_CheckFrame(pstRS485Task,pRecvBuffer,usRecvLen,pusJumpLen);
			break;
		case	CON_PROTOCOL_WS	:	//��ʤ��Լ
			ucRet = WS_Protocol_CheckFrame(pstRS485Task,pRecvBuffer,usRecvLen,pusJumpLen);
			break;
		case	CON_PROTOCOL_GENIUS	:	//����
			ucRet = Genius_Protocol_CheckFrame(pstRS485Task,pRecvBuffer,usRecvLen,pusJumpLen);
			break;
		case	CON_PROTOCOL_HND:		//�������Լ
			ucRet = HND_Protocol_CheckFrame(pstRS485Task,pRecvBuffer,usRecvLen,pusJumpLen);
			break;
		*/
		case	CON_PROTOCOL_INVALID:	//��Ч��Լ
			ucRet = RET_ERROR;
		}
	}
	return ucRet;
}

//unsigned char _Meter_GetReSendContent(STRS485TASK *pstRS485SendTask,STRS485TASK *pstRS485RecvTask,STRS485TASK *pstRS485RetTask)
//{//���أ�RET_SUCCESS;RET_ERROR;RET_COMM_TIMEOVER:һ��ͨ�Ž����������ط�
//	unsigned char ucRet = RET_ERROR;
//	unsigned char ucMeterNo=0;
//	ucMeterNo = Get_MeterNo_byMPNo(pstRS485SendTask->ucDevNo);
//	if(ucMeterNo!=0xFF)
//	{
//		switch(stMPNoAddrList.stMPNoAddrRec[ucMeterNo].ucProtocolNo)
//		{
//		case	CON_PROTOCOL_DL645:		//645��Լ
//		case	CON_PROTOCOL_AS1_DL645:		//645��Լ����һ
//		case	CON_PROTOCOL_AS2_DL645:		//645��Լ���ƶ�
//			
//		case	CON_PROTOCOL_HLi:		//�㽭���� (645)
//		case	CON_PROTOCOL_KL:		//��½ (645)
//		case	CON_PROTOCOL_LD:		//���� (645)
//		case	CON_PROTOCOL_SKD:		//��ƴ� (645)
//		case	CON_PROTOCOL_JSLY:		//�������� (645)
//		case	CON_PROTOCOL_HND645:		//������ (645)
//		case	CON_PROTOCOL_DFDZ:		//�������� (645)
//		case	CON_PROTOCOL_JQ:		//��ȸ (645)
//        case	CON_PROTOCOL_DL64507:		//645��Լ2007 yzy
//			ucRet = DL645_GetReSendContent(pstRS485SendTask,pstRS485RecvTask,pstRS485RetTask);
//			break;
//		case  	CON_PROTOCOL_HB_PT	:	//����֮��ͨ����ʽ���ܱ�
//			ucRet = HBPT_GetReSendContent(pstRS485SendTask,pstRS485RecvTask,pstRS485RetTask);
//			break;
//		case  	CON_PROTOCOL_HB_JM	:	//����֮���񸴷��ʵ��ܱ�
//			ucRet = HBJM_GetReSendContent(pstRS485SendTask,pstRS485RecvTask,pstRS485RetTask);
//			break;
		/*
		case	CON_PROTOCOL_ABB_R:		//ABB����
			ucRet = ABBR_GetReSendContent(pstRS485SendTask,pstRS485RecvTask,pstRS485RetTask);
			break;
		case	CON_PROTOCOL_LANDB:		//������B��
			ucRet = LANDB_GetReSendContent(pstRS485SendTask,pstRS485RecvTask,pstRS485RetTask);
			break;
		case	CON_PROTOCOL_LANDD:		//������D��
			ucRet = LANDD_GetReSendContent(pstRS485SendTask,pstRS485RecvTask,pstRS485RetTask);
			break;
		case	CON_PROTOCOL_WS	:	//��ʤ��Լ
			ucRet = WS_GetReSendContent(pstRS485SendTask,pstRS485RecvTask,pstRS485RetTask);
			break;
		case  	CON_PROTOCOL_GENIUS	:	//�����Լ
			ucRet = Genius_GetReSendContent(pstRS485SendTask,pstRS485RecvTask,pstRS485RetTask);
			break;
		case	CON_PROTOCOL_HND:		//�������Լ
			ucRet = HND_GetReSendContent(pstRS485SendTask,pstRS485RecvTask,pstRS485RetTask);
			break;
		*/
//		case	CON_PROTOCOL_INVALID:	//��Ч��Լ
//			ClearRS485Task(pstRS485RetTask);
//			ucRet = RET_SUCCESS;
//		}
//	}else
//		ClearRS485Task(pstRS485RetTask);
//
//	return ucRet;
//}

//unsigned char _Meter_Protocol_DispartOverTime(STRS485TASK *pstRS485SendTask,STRS485TASK *pstRS485RecvTask,unsigned char *pucExplainResultNum)
//{
//	unsigned char ucRet = RET_ERROR;
//	unsigned char ucMeterNo=0;
//	ucMeterNo = Get_MeterNo_byMPNo(pstRS485SendTask->ucDevNo);
//	if(ucMeterNo!=0xFF)
//	{
//		switch(stMPNoAddrList.stMPNoAddrRec[ucMeterNo].ucProtocolNo)
//		{
//		case	CON_PROTOCOL_DL645:		//645��Լ
//		case	CON_PROTOCOL_AS1_DL645:		//645��Լ����һ
//		case	CON_PROTOCOL_AS2_DL645:		//645��Լ���ƶ�
//			
//		case	CON_PROTOCOL_HLi:		//�㽭���� (645)
//		case	CON_PROTOCOL_KL:		//��½ (645)
//		case	CON_PROTOCOL_LD:		//���� (645)
//		case	CON_PROTOCOL_SKD:		//��ƴ� (645)
//		case	CON_PROTOCOL_JSLY:		//�������� (645)
//		case	CON_PROTOCOL_HND645:		//������ (645)
//		case	CON_PROTOCOL_DFDZ:		//�������� (645)
//		case	CON_PROTOCOL_JQ:		//��ȸ (645)
//		case	CON_PROTOCOL_DL64507:		//645��Լ2007 yzy
//			ucRet = DL645_DispartOverTime(pstRS485RecvTask,pucExplainResultNum);
//			break;
//		case	CON_PROTOCOL_HB_PT	:	//����֮��ͨ����ʽ���ܱ�
//			ucRet = HBPT_DispartOverTime(pstRS485RecvTask,pucExplainResultNum);
//			break;
//		case	CON_PROTOCOL_HB_JM	:	//����֮���񸴷��ʵ��ܱ�
//			ucRet = HBJM_DispartOverTime(pstRS485RecvTask,pucExplainResultNum);
//			break;
		/*
		case	CON_PROTOCOL_ABB_R:		//ABB����
			ucRet = ABBR_DispartOverTime(pstRS485RecvTask,pucExplainResultNum);
			break;
		case	CON_PROTOCOL_LANDB:		//������B��
		case	CON_PROTOCOL_LANDD:		//������D��
			ucRet = LANDD_DispartOverTime(pstRS485RecvTask,pucExplainResultNum);
			break;
		case	CON_PROTOCOL_WS	:	//��ʤ��Լ
			ucRet = WS_DispartOverTime(pstRS485RecvTask,pucExplainResultNum);
			break;
		case	CON_PROTOCOL_GENIUS	:	//�����Լ
			ucRet = Genius_DispartOverTime(pstRS485RecvTask,pucExplainResultNum);
			break;
		case	CON_PROTOCOL_HND:		//�������Լ
			ucRet = HND_DispartOverTime(pstRS485RecvTask,pucExplainResultNum);
			break;
		*/
//		case	CON_PROTOCOL_INVALID:	//��Ч��Լ
//			ucRet = RET_ERROR;
//		}
//	}
//	return ucRet;
//}


//���֡�����ʱ�䳣��
//unsigned char _Meter_Protocol_GetTimerConst(STRS485TASK *pstRS485Task,unsigned char *pucChID,
//								   unsigned short *pusMaxRecvOverTime,unsigned short *pusMaxByteOverTime,
//								   unsigned short *pusMaxSendDelay)
//{
//	unsigned char ucRet=RET_SUCCESS,ucMeterNo=0;
//	if(pstRS485Task==NULL)
//	{
////		(*pucChID)				=RS485_CH_ID;
//		(*pusMaxRecvOverTime)	=RS485_RECV_OVER_TIME;
//		(*pusMaxByteOverTime)	=RS485_BYTE_OVER_TIME;
//		(*pusMaxSendDelay)		=RS485_SEND_DELAY;
//	}else if(pstRS485Task->ucReportType!=CON_RS485_REPORT_TYPE_RELAY)
//	{
//		ucMeterNo = Get_MeterNo_byMPNo(pstRS485Task->ucDevNo);
//		if(ucMeterNo!=0xFF)
//		{
//			switch(stMPNoAddrList.stMPNoAddrRec[ucMeterNo].ucProtocolNo)
//			{
//			case	CON_PROTOCOL_DL645:		//645��Լ
//			case	CON_PROTOCOL_AS1_DL645:		//645��Լ����һ
//			case	CON_PROTOCOL_AS2_DL645:		//645��Լ���ƶ�
//
//			case	CON_PROTOCOL_HLi:		//�㽭���� (645)
//			case	CON_PROTOCOL_KL:		//��½ (645)
//			case	CON_PROTOCOL_LD:		//���� (645)
//			case	CON_PROTOCOL_SKD:		//��ƴ� (645)
//			case	CON_PROTOCOL_JSLY:		//�������� (645)
//			case	CON_PROTOCOL_HND645:		//������ (645)
//			case	CON_PROTOCOL_DFDZ:		//�������� (645)
//			case	CON_PROTOCOL_JQ:		//��ȸ (645)
//			case	CON_PROTOCOL_DL64507:		//645��Լ2007 yzy
//				DL645_GetTimerConst(pstRS485Task,pucChID,pusMaxRecvOverTime,pusMaxByteOverTime,pusMaxSendDelay);
//				break;
//			case	CON_PROTOCOL_HB_PT	:	//����֮��ͨ����ʽ���ܱ�
//				HBPT_GetTimerConst(pstRS485Task,pucChID,pusMaxRecvOverTime,pusMaxByteOverTime,pusMaxSendDelay);
//				break;
//			case	CON_PROTOCOL_HB_JM	:	//����֮���񸴷��ʵ��ܱ�
//				HBJM_GetTimerConst(pstRS485Task,pucChID,pusMaxRecvOverTime,pusMaxByteOverTime,pusMaxSendDelay);
//				break;
//			/*
//			case	CON_PROTOCOL_ABB_R:		//ABB����
//				ABBR_GetTimerConst(pstRS485Task,pucChID,pusMaxRecvOverTime,pusMaxByteOverTime,pusMaxSendDelay);
//				break;
//			case	CON_PROTOCOL_LANDB:		//������B��
//			case	CON_PROTOCOL_LANDD:		//������D��
//				LANDD_GetTimerConst(pstRS485Task,pucChID,pusMaxRecvOverTime,pusMaxByteOverTime,pusMaxSendDelay);
//				break;
//			case	CON_PROTOCOL_WS	:		//��ʤ��Լ
//				WS_GetTimerConst(pstRS485Task,pucChID,pusMaxRecvOverTime,pusMaxByteOverTime,pusMaxSendDelay);
//				break;
//			case	CON_PROTOCOL_GENIUS	:	//�����Լ
//				Genius_GetTimerConst(pstRS485Task,pucChID,pusMaxRecvOverTime,pusMaxByteOverTime,pusMaxSendDelay);
//				break;
//			case	CON_PROTOCOL_HND:		//�������Լ
//				HND_GetTimerConst(pstRS485Task,pucChID,pusMaxRecvOverTime,pusMaxByteOverTime,pusMaxSendDelay);
//				break;
//			*/
//			case	CON_PROTOCOL_INVALID:	//��Ч��Լ
//				ucRet =RET_ERROR;
//			}
//		}
//	}else
//	{
////		(*pucChID)				=RS485_CH_ID;
//		(*pusMaxRecvOverTime)	=pstRS485Task->stRelayPara.ucRecvWaitTime;	//��λ�ⲿ10ms
//		(*pusMaxByteOverTime)	=pstRS485Task->stRelayPara.ucByteWaitTime;
//		(*pusMaxSendDelay)		=RS485_SEND_DELAY;
//	}
//	return ucRet;
//}
//unsigned char _Meter_Protocol_GetPortPara(STRS485TASK *pstRS485Task,unsigned long *pulBaudRate,unsigned char *pucEPN,unsigned char *pucDataBit,unsigned char *pucStopBit)
//{
//	unsigned char ucRet = RET_SUCCESS;
//	unsigned char ucMeterNo;
//
//	ucMeterNo = Get_MeterNo_byMPNo(pstRS485Task->ucDevNo);
//	if(ucMeterNo!=0xFF)
//	{
//		switch(stMPNoAddrList.stMPNoAddrRec[ucMeterNo].ucProtocolNo)
//		{
//		case	CON_PROTOCOL_DL645:		//645��Լ
//			//DL645_Protocol_GetPortPara(pulBaud,pucEPN,pucDataBit);
//			break;
//		case	CON_PROTOCOL_ABB_R:		//ABB����
//			//ABBR_Protocol_GetPortPara(pulBaud,pucEPN,pucDataBit);
//			break;
//		case	CON_PROTOCOL_LANDD:		//������D��
//			//LANDD_Protocol_GetPortPara(pulBaud,pucEPN,pucDataBit);
//			break;
//		case	CON_PROTOCOL_LANDB:		//������B��
//			//ucRet =LANDB_Protocol_GetPortPara(pulBaudRate,pucEPN,pucDataBit,pucStopBit);
//			break;
//		case	CON_PROTOCOL_WS	:		//��ʤ��Լ
//			//WS_Protocol_GetPortPara(pulBaud,pucEPN,pucDataBit);
//			break;
////		case	CON_PROTOCOL_GENIUS:	//�����Լ
//			//Genius_Protocol_GetPortPara(pulBaud,pucEPN,pucDataBit);
////			break;
//		case      CON_PROTOCOL_HND:   //�������Լ
//			//ucRet =HND_Protocol_GetPortPara(pulBaudRate,pucEPN,pucDataBit);
//			break;
//		case	CON_PROTOCOL_DL64507:		//645��Լ2007 yzy
//			//DL645_Protocol_GetPortPara(pulBaud,pucEPN,pucDataBit);
//			break;
//		default:						//include  CON_PROTOCOL_INVALID
//			ucRet =RET_ERROR;		
//		}
//	}else
//		ucRet = RET_ERROR;
//
//	if(ucRet == RET_ERROR)
//	{
//		(*pulBaudRate)		= 1200;		//1200������
//		(*pucEPN)		= UART_EVEN_PARITY;		//żУ��
//		(*pucDataBit)	= 8;		//8λ����λ
//		ucRet = RET_SUCCESS;		
//	}
//	return ucRet;
//}
//unsigned char _Meter_Protocol_GetTimerCommand(STCMDFLAG	stCmdFlag,STCMDFLAG stIsSendFlag,STRS485TASKCMDLIST	*pstRS485TaskCmdList)
//{
//	unsigned char ucRet=RET_SUCCESS,m=0;
//	//�����й�		
//	if(stIsSendFlag.ucDirPowerFlag   == CONCOMMAND_SEND)	GetCommand_DirPower(stCmdFlag,pstRS485TaskCmdList,&m);
//	//�����޹�
//	if(stIsSendFlag.ucUnDirVarFlag == CONCOMMAND_SEND)		GetCommand_UnDirVar(stCmdFlag,pstRS485TaskCmdList,&m);
//	//�����й�	
//	if(stIsSendFlag.ucUnDirPowerFlag == CONCOMMAND_SEND)	GetCommand_UnDirPower(stCmdFlag,pstRS485TaskCmdList,&m);
//	//�����޹�
//	if(stIsSendFlag.ucDirVarFlag     == CONCOMMAND_SEND)	GetCommand_DirVar(stCmdFlag,pstRS485TaskCmdList,&m);
//	//1�����޹�
//	if(stIsSendFlag.ucVar1Flag == CONCOMMAND_SEND)			GetCommand_Var1(stCmdFlag,pstRS485TaskCmdList,&m);
//	//2�����޹�
//	if(stIsSendFlag.ucVar2Flag == CONCOMMAND_SEND)			GetCommand_Var2(stCmdFlag,pstRS485TaskCmdList,&m);
//	//3�����޹�
//	if(stIsSendFlag.ucVar3Flag == CONCOMMAND_SEND)			GetCommand_Var3(stCmdFlag,pstRS485TaskCmdList,&m);
//	//4�����޹�
//	if(stIsSendFlag.ucVar4Flag == CONCOMMAND_SEND)			GetCommand_Var4(stCmdFlag,pstRS485TaskCmdList,&m);
//	//�����й��������
//	if(stIsSendFlag.ucDirPowerMaxDemandFlag == CONCOMMAND_SEND)			GetCommand_DirPowerMaxDemand(stCmdFlag,pstRS485TaskCmdList,&m);
//	//�����й���������ʱ��
//	if(stIsSendFlag.ucDirPowerMaxDemandTimeFlag == CONCOMMAND_SEND)		GetCommand_DirPowerMaxDemandTime(stCmdFlag,pstRS485TaskCmdList,&m);
//	//�����޹��������
//	if(stIsSendFlag.ucDirVarMaxDemandFlag == CONCOMMAND_SEND)			GetCommand_DirVarMaxDemand(stCmdFlag,pstRS485TaskCmdList,&m);
//	//�����޹���������ʱ��
//	if(stIsSendFlag.ucDirVarMaxDemandTimeFlag == CONCOMMAND_SEND)		GetCommand_DirVarMaxDemandTime(stCmdFlag,pstRS485TaskCmdList,&m);
//	//�����й��������
//	if(stIsSendFlag.ucUnDirPowerMaxDemandFlag == CONCOMMAND_SEND)		GetCommand_UnDirPowerMaxDemand(stCmdFlag,pstRS485TaskCmdList,&m);
//	//�����й������������ʱ��
//	if(stIsSendFlag.ucUnDirPowerMaxDemandTimeFlag == CONCOMMAND_SEND)	GetCommand_UnDirPowerMaxDemandTime(stCmdFlag,pstRS485TaskCmdList,&m);
//	//�����޹��������
//	if(stIsSendFlag.ucUnDirVarMaxDemandFlag == CONCOMMAND_SEND)		GetCommand_UnDirVarMaxDemand(stCmdFlag,pstRS485TaskCmdList,&m);
//	//�����޹������������ʱ��
//	if(stIsSendFlag.ucUnDirVarMaxDemandTimeFlag == CONCOMMAND_SEND)	GetCommand_UnDirVarMaxDemandTime(stCmdFlag,pstRS485TaskCmdList,&m);
//
//	//�����й�(����)
//	if(stIsSendFlag.ucDirPowerLmFlag == CONCOMMAND_SEND)		GetCommand_DirPowerLm(stCmdFlag,pstRS485TaskCmdList,&m);	
//	//�����й�(����)	
//	if(stIsSendFlag.ucUnDirPowerLmFlag == CONCOMMAND_SEND)		GetCommand_UnDirPowerLm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//�����޹�(����)
//	if(stIsSendFlag.ucDirVarLmFlag     == CONCOMMAND_SEND)		GetCommand_DirVarLm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//�����޹�(����)
//	if(stIsSendFlag.ucUnDirVarLmFlag == CONCOMMAND_SEND)		GetCommand_UnDirVarLm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//1�����޹�(����)
//	if(stIsSendFlag.ucVar1LmFlag == CONCOMMAND_SEND)			GetCommand_Var1Lm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//2�����޹�(����)
//	if(stIsSendFlag.ucVar2LmFlag == CONCOMMAND_SEND)			GetCommand_Var2Lm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//3�����޹�(����)
//	if(stIsSendFlag.ucVar3LmFlag == CONCOMMAND_SEND)			GetCommand_Var3Lm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//4�����޹�(����)
//	if(stIsSendFlag.ucVar4LmFlag == CONCOMMAND_SEND)			GetCommand_Var4Lm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//�����й��������(����)
//	if(stIsSendFlag.ucDirPowerMaxDemandLmFlag == CONCOMMAND_SEND)		GetCommand_DirPowerMaxDemandLm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//�����й���������ʱ��(����)
//	if(stIsSendFlag.ucDirPowerMaxDemandTimeLmFlag == CONCOMMAND_SEND)	GetCommand_DirPowerMaxDemandTimeLm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//�����޹��������(����)
//	if(stIsSendFlag.ucDirVarMaxDemandLmFlag == CONCOMMAND_SEND)			GetCommand_DirVarMaxDemandLm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//�����޹���������ʱ��(����)
//	if(stIsSendFlag.ucDirVarMaxDemandTimeLmFlag == CONCOMMAND_SEND)		GetCommand_DirVarMaxDemandTimeLm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//�����й��������(����)
//	if(stIsSendFlag.ucUnDirPowerMaxDemandLmFlag == CONCOMMAND_SEND)		GetCommand_UnDirPowerMaxDemandLm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//�����й������������ʱ��(����)
//	if(stIsSendFlag.ucUnDirPowerMaxDemandTimeLmFlag == CONCOMMAND_SEND)	GetCommand_UnDirPowerMaxDemandTimeLm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//�����޹��������(����)
//	if(stIsSendFlag.ucUnDirVarMaxDemandLmFlag == CONCOMMAND_SEND)		GetCommand_UnDirVarMaxDemandLm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//�����޹������������ʱ��(����)
//	if(stIsSendFlag.ucUnDirVarMaxDemandTimeLmFlag == CONCOMMAND_SEND)	GetCommand_UnDirVarMaxDemandTimeLm(stCmdFlag,pstRS485TaskCmdList,&m);
//	
//	//�����ѹ
//	if(stIsSendFlag.ucVolFlag == CONCOMMAND_SEND)		GetCommand_Uabc(stCmdFlag,pstRS485TaskCmdList,&m);
//	//�������
//	if(stIsSendFlag.ucCurFlag == CONCOMMAND_SEND)		GetCommand_Iabc(stCmdFlag,pstRS485TaskCmdList,&m);
//
//	//˲ʱ�й�
//	if(stIsSendFlag.ucInstantPowFlag == CONCOMMAND_SEND)	GetCommand_InstantPow(stCmdFlag,pstRS485TaskCmdList,&m);
//	//˲ʱ�޹�
//	if(stIsSendFlag.ucInstantVarFlag == CONCOMMAND_SEND)	GetCommand_InstantVar(stCmdFlag,pstRS485TaskCmdList,&m);
//
//	//���ʱ���
//	if(stIsSendFlag.ucProgramTimeFlag == CONCOMMAND_SEND)	GetCommand_ProgramTime(stCmdFlag,pstRS485TaskCmdList,&m);
//
//	//���峣����
//	if(stIsSendFlag.ucPulseConstFlag == CONCOMMAND_SEND)	GetCommand_PulseConst(stCmdFlag,pstRS485TaskCmdList,&m);
//
//	//���״̬��
//	if(stIsSendFlag.ucMeterStatFlag == CONCOMMAND_SEND)		GetCommand_MeterStat(stCmdFlag,pstRS485TaskCmdList,&m);
//
//	//������
//	if(stIsSendFlag.ucCopyDayTimeFlag == CONCOMMAND_SEND)	GetCommand_CopyDayTime(stCmdFlag,pstRS485TaskCmdList,&m);
//
//	//�������
//	if(stIsSendFlag.ucLostVolNumFlag == CONCOMMAND_SEND)	GetCommand_LostVolNum(stCmdFlag,pstRS485TaskCmdList,&m);
//	//�����ۼ�ʱ��
//	if(stIsSendFlag.ucLostVolTimeFlag == CONCOMMAND_SEND)	GetCommand_LostVolTime(stCmdFlag,pstRS485TaskCmdList,&m);
//	//���һ�ζ��࿪ʼʱ��
//	if(stIsSendFlag.ucLastVolOffBegTimeFlag == CONCOMMAND_SEND)	GetCommand_LastVolOffBegTime(stCmdFlag,pstRS485TaskCmdList,&m);
//	//���һ�ζ������ʱ��
//	if(stIsSendFlag.ucLastVolOffEndTimeFlag == CONCOMMAND_SEND)	GetCommand_LastVolOffEndTime(stCmdFlag,pstRS485TaskCmdList,&m);
//
//	//����
//	if(stIsSendFlag.ucFeeRateFlag == CONCOMMAND_SEND)			GetCommand_FeeRate(stCmdFlag,pstRS485TaskCmdList,&m);
//
//	//ʱ��
//	if(stIsSendFlag.ucDateTimeFlag == CONCOMMAND_SEND)				GetCommand_DateTime(stCmdFlag,pstRS485TaskCmdList,&m);
//
//	if(stIsSendFlag.ucProgramNumFlag == CONCOMMAND_SEND)			GetCommand_ProgramNum(stCmdFlag,pstRS485TaskCmdList,&m);
//	if(stIsSendFlag.ucLastProgramTimeFlag== CONCOMMAND_SEND)		GetCommand_LastProgramTime(stCmdFlag,pstRS485TaskCmdList,&m);
//
//	if(stIsSendFlag.ucMeterclrNumFlag == CONCOMMAND_SEND)			GetCommand_MeterclrNum(stCmdFlag,pstRS485TaskCmdList,&m);
//	if(stIsSendFlag.ucLastMeterclrTimeFlag == CONCOMMAND_SEND)		GetCommand_LastMeterclrTime(stCmdFlag,pstRS485TaskCmdList,&m);
//
//	if(stIsSendFlag.ucDemondclrNumFlag== CONCOMMAND_SEND)			GetCommand_DemondclrNum(stCmdFlag,pstRS485TaskCmdList,&m);
//	if(stIsSendFlag.ucLastDemondclrTimeFlag == CONCOMMAND_SEND)		GetCommand_LastDemondclrTime(stCmdFlag,pstRS485TaskCmdList,&m);
//
//	if(stIsSendFlag.ucEventclrNumFlag== CONCOMMAND_SEND)			GetCommand_EventclrNum(stCmdFlag,pstRS485TaskCmdList,&m);
//	if(stIsSendFlag.ucLastEventclrTimeFlag == CONCOMMAND_SEND)		GetCommand_LastEventclrTime(stCmdFlag,pstRS485TaskCmdList,&m);
//
//	if(stIsSendFlag.ucJiaoshiNumFlag== CONCOMMAND_SEND)				GetCommand_JiaoshiNum(stCmdFlag,pstRS485TaskCmdList,&m);
//	if(stIsSendFlag.ucLastJiaoshiTimeFlag == CONCOMMAND_SEND)		GetCommand_LastJiaoshiTime(stCmdFlag,pstRS485TaskCmdList,&m);
//
///*	if(stIsSendFlag.ucFeCuFlag == CONCOMMAND_SEND)				GetCommand_FeCu(stCmdFlag,pstRS485TaskCmdList,&m);*/
//
//	if(stIsSendFlag.ucDirPowAFlag == CONCOMMAND_SEND)			GetCommand_DirPowA(stCmdFlag,pstRS485TaskCmdList,&m);	
//	if(stIsSendFlag.ucDirPowBFlag == CONCOMMAND_SEND)			GetCommand_DirPowB(stCmdFlag,pstRS485TaskCmdList,&m);
//	if(stIsSendFlag.ucDirPowCFlag == CONCOMMAND_SEND)			GetCommand_DirPowC(stCmdFlag,pstRS485TaskCmdList,&m);
//	if(stIsSendFlag.ucDirPowALmFlag == CONCOMMAND_SEND)			GetCommand_DirPowALM(stCmdFlag,pstRS485TaskCmdList,&m);	
//	if(stIsSendFlag.ucDirPowBLmFlag == CONCOMMAND_SEND)			GetCommand_DirPowBLM(stCmdFlag,pstRS485TaskCmdList,&m);
//	if(stIsSendFlag.ucDirPowCLmFlag == CONCOMMAND_SEND)			GetCommand_DirPowCLM(stCmdFlag,pstRS485TaskCmdList,&m);
//	return ucRet;
//}



//unsigned short GetCheckCode_16BitCRC(unsigned char *ptr,unsigned short len)
//{
//	unsigned short crc;
//	unsigned char da;
//	unsigned int crc_ta[16]={0x0000, 0x1021, 0x2042, 0x3063,
//						     0x4084, 0x50a5, 0x60c6, 0x70e7,
//						     0x8108, 0x9129, 0xa14a, 0xb16b,
//						     0xc18c, 0xd1ad, 0xe1ce, 0xf1ef};
//	crc=0;
//
//	while( len--!=0 )
//	{
//		da=((unsigned char)(crc/256))/16;  	/* �ݴ�CRC�ĸ���λ */
//		crc<<=4;                  	 		/* CRC����4λ���൱��ȡCRC�ĵ�12λ��*/
//		crc^=crc_ta[da^(*ptr/16)];     		/* CRC�ĸ�4λ�ͱ��ֽڵ�ǰ���ֽ���Ӻ������CRC��
//												Ȼ�������һ��CRC������ */
//		da=((unsigned char)(crc/256))/16;   /* �ݴ�CRC�ĸ�4λ */
//		crc<<=4;                   			/* CRC����4λ���൱��CRC�ĵ�12λ�� */
//		crc^=crc_ta[da^(*ptr&0x0f)];   		/* CRC�ĸ�4λ�ͱ��ֽڵĺ���ֽ���Ӻ������CRC��
//												Ȼ���ټ�����һ��CRC������ */
//		ptr++;
//	}
//	return crc;
//}

unsigned char Get_MeterNo_byMPNo(unsigned char ucMPNo)
{
	unsigned char ucMeterNo = 0xFF,i=0;
    if(ucMPNo == 0)//azh
    {
        ucMeterNo = 0;
    }    
	for(i=0;i<CON_METER_NUM_PRO;i++)
	{
		if(stMPNoAddrList.stMPNoAddrRec[i].ucMPNo == ucMPNo)
			break;
	}
	if(i<CON_METER_NUM_PRO)
		ucMeterNo = i;

	return ucMeterNo;
}

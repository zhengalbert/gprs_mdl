/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_Up_Explain.c
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-02-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

//������
static unsigned char Get_Frame_Info_Ctrl(unsigned char ucRetMsgType,unsigned char ucUpActive,unsigned char ucSrcMsgType);
static unsigned char Get_Frame_Info_SEQ(unsigned char ucConFlag,STMAINCOMMINFO	*pstMainCommInfo,unsigned char ucFrameFlag);

//�ն����ͱ��Ĵ��� 
unsigned char Main_SendExplain(unsigned char ucMsgType,unsigned char *pucSrcBuffer,unsigned char *pucSendBuffer,unsigned short *pusLen,STMAINCOMMINFO *pstMainCommInfo,
							   unsigned char ucUpActive,unsigned char ucConFlag,unsigned char ucFrameFlag)
{
//	unsigned char ucRet=RET_ERROR;
	unsigned char	ucC,ucSEQ;
	unsigned short	usDataLen=0,m=0;
	unsigned char	ucRetMsgType = Get_Ret_MsgType(ucMsgType);
	STTERDEVADDR	stTerAddr=RunPara_GetTerDevAddr();
	STTERREGIONADDR	stTerRegionAddr=RunPara_GetTerRegionAddr();
	unsigned short	usSeqCount;
	STDATETIME		stCurrentTime;
	STTERCOMMPARA	stTerCommPara;
	unsigned char 	ucCheckSum;

	if( pucSrcBuffer==NULL || pucSendBuffer==NULL || pusLen==NULL )
		return RET_ERROR;

	usDataLen=(*pusLen);
	pucSendBuffer[m++]=0x68;
	pucSendBuffer[m++]=0;								//֡����L
	pucSendBuffer[m++]=0;
	pucSendBuffer[m++]=0;
	pucSendBuffer[m++]=0;
	pucSendBuffer[m++]=0x68;

	ucC=Get_Frame_Info_Ctrl(ucRetMsgType,ucUpActive,ucMsgType);
	if(pstMainCommInfo==NULL && ucRetMsgType==CON_TERMSG_CONNECT)					//��������ȥ��ACD�������澯����
		ucC &= (~CON_FRAME_CMD_ACD_IMP);
	pucSendBuffer[m++]=ucC;								

	pucSendBuffer[m++]=stTerRegionAddr.ucRegionAddr[0];	
	pucSendBuffer[m++]=stTerRegionAddr.ucRegionAddr[1];
	pucSendBuffer[m++]=stTerAddr.ucDevAddr[0];
	pucSendBuffer[m++]=stTerAddr.ucDevAddr[1];

	if(pstMainCommInfo!=NULL)
		pucSendBuffer[m++]=(pstMainCommInfo->ucMSA & 0xFE);	//�����ն˵�ַ���������ն����ַ
	else
		pucSendBuffer[m++]=0;							//��ַ�����������ͣ���վ��ַ��Ϊ0
	//add 
//	memcpy(ucLogin,pucSrcBuffer,usDataLen);	//������
	if((ucRetMsgType == CON_TERMSG_LOGIN) && (pucSrcBuffer[2] == 0x01))
	{
		ucRetMsgType = CON_TERMSG_ACK;
		usDataLen = 4;
	}
	pucSendBuffer[m++]=ucRetMsgType;					//Ӧ�ò㹦����AFN

	ucSEQ=Get_Frame_Info_SEQ(ucConFlag,pstMainCommInfo,ucFrameFlag);
	if((pstMainCommInfo==NULL) && (ucRetMsgType==CON_TERMSG_REQDATA_1 || ucRetMsgType==CON_TERMSG_REQDATA_2))	//����������������
		ucSEQ |= CON_FRAME_SEQ_TPV;						//���������������ͣ���Ҫ��ʱ���ǩ
	pucSendBuffer[m++]=ucSEQ;

	memcpy(pucSendBuffer+m,pucSrcBuffer,usDataLen);	//������
	m+=usDataLen;
	
	if((ucC & CON_FRAME_CMD_ACD_IMP)==CON_FRAME_CMD_ACD_IMP)
	{													//���ACD=1����Ҫ����EC
		pucSendBuffer[m++]=g_ucEventCount_Important;
		pucSendBuffer[m++]=g_ucEventCount_Normal;
	}

	if((ucSEQ & CON_FRAME_SEQ_TPV)==CON_FRAME_SEQ_TPV)	//�����Ҫ����TPv
	{
		if(pstMainCommInfo!=NULL)						//��վ��ѯֱ�Ӹ��ƻ�Ӧ
		{
			memcpy(pucSendBuffer+m,pstMainCommInfo->ucTPvInfo,6);
			m+=6;
		}else											//��������
		{
			stCurrentTime=GetCurrentTime();
			stTerCommPara=RunPara_GetTerCommPara();
			usSeqCount=TerSeq_GetCount();
			pucSendBuffer[m++]=(unsigned char)usSeqCount;
			pucSendBuffer[m++]=(unsigned char)HEX_TO_BCD(stCurrentTime.ucSecond,1);
			pucSendBuffer[m++]=(unsigned char)HEX_TO_BCD(stCurrentTime.ucMinute,1);
			pucSendBuffer[m++]=(unsigned char)HEX_TO_BCD(stCurrentTime.ucHour,1);
			pucSendBuffer[m++]=(unsigned char)HEX_TO_BCD(stCurrentTime.ucDay,1);
			pucSendBuffer[m++]=stTerCommPara.ucMaxOvertimeOrig;
		}
	}
	ucCheckSum = Get_Sum(pucSendBuffer+6,(unsigned long)(m-6));	//У���
	pucSendBuffer[m++]=ucCheckSum;
	pucSendBuffer[m++]=0x16;						//֡β

	(*pusLen)=m;
	m=m-8;											//���ȴӵ�ַ��Ϣ��ʼ
//	m=(m<<2)+0x01;
	m=(m<<2)+0x02;
	pucSendBuffer[1]=(unsigned char)m;			//L1			
	pucSendBuffer[2]=(unsigned char)(m>>8);		//L2
	pucSendBuffer[3]=(unsigned char)m;			//L1
	pucSendBuffer[4]=(unsigned char)(m>>8);		//L2

	return RET_SUCCESS;
}

//������
unsigned char Get_Frame_Info_Ctrl(unsigned char ucRetMsgType,unsigned char ucUpActive,unsigned char ucSrcMsgType)
{
	unsigned char ucC=0;
	unsigned short usFrameACD=CON_STATUS_NO_2BYTE;
	ucC=ucC | CON_FRAME_CMD_DIR_UP;
	if(ucUpActive==CON_STATUS_YES)
		ucC=ucC | CON_FRAME_CMD_PRM_ORG;
	usFrameACD=Alarm_GetFrameACD();
	if(usFrameACD==CON_STATUS_YES_2BYTE)
		ucC=ucC | CON_FRAME_CMD_ACD_IMP;
	switch(ucRetMsgType)
	{
	case CON_TERMSG_ACK:			//ȷ�Ϸ���	//0:�Ͽ�
		if(ucSrcMsgType==CON_TERMSG_RESET)
			ucC=ucC+0;					//0��ȷ��
		else if(ucSrcMsgType==CON_TERMSG_SETPARA || ucSrcMsgType==CON_TERMSG_CONTROL || ucSrcMsgType==CON_TERMSG_ACK)
			ucC=ucC+8;					//8����Ӧ֡
		else
			ucC=ucC+9;					//9����Ӧ֡�����ϣ������ٻ�������
		break;
	case CON_TERMSG_CONNECT:		//��¼����
		if(ucUpActive==CON_STATUS_YES)
			ucC=ucC+9;					//9����·���ԣ���������
		else
			ucC=ucC+11;					//11����·���ԣ�Ӧ��
		break;
	case CON_TERMSG_GETPARA:		//4�����ݷ��ͣ���Ӧ��������
	case CON_TERMSG_FILETRANS:		//�ļ�����
	case CON_TERMSG_EXTCMD:			//��չ����
	case CON_TERMSG_DATATRANS:		//����ת��
	case CON_TERMSG_CONFIGURATION:	//�����ն�����
	case CON_TERMSG_LOGIN:
		ucC=ucC+8;					//8���û�����
		break;
	case CON_TERMSG_REQDATA_1:		//1�����ݷ��ͣ�ʵʱ���ݣ�
	case CON_TERMSG_REQDATA_2:		//2�����ݷ��ͣ��������ݣ�
	case CON_TERMSG_TASKDATA:		//��������
	
		if(ucUpActive==CON_STATUS_NO)
			ucC=ucC+8;					//8���û�����
		else
			ucC=ucC+4;					//4�������޻ش�  ��������
		break;
	case CON_TERMSG_REQDATA_3:		//3�����ݷ��ͣ��¼���
		if(ucUpActive==CON_STATUS_NO)
			ucC=ucC+8;					//8���û�����
		else
		{
			ucC=ucC+4;					//4����������
			ucC=ucC & (~CON_FRAME_CMD_ACD_IMP);		//ȥ���¼�λ
		}
//			ucC=ucC+10;					//10���������ͣ���Ҫȷ��
		break;
	case CON_TERMSG_RELAYCMD:		//�м�վ����
		break;
	default:
		break;
	}
	return ucC;
}

unsigned char Get_Frame_Info_SEQ(unsigned char ucConFlag,STMAINCOMMINFO	*pstMainCommInfo,unsigned char ucFrameFlag)
{
	unsigned char ucSEQ=0,ucSEQ1=0;
	unsigned char ucSeqCount=0;
	if(pstMainCommInfo!=NULL)
	{
		ucSEQ=pstMainCommInfo->ucSEQ;
		ucSeqCount=ucSEQ&0x0F;
		ucSeqCount=(ucSeqCount+ucFrameFlag)&0x0F;//����֡���
		ucSEQ &= (~CON_FRAME_SEQ_CON);
		ucSEQ=ucSEQ&0x90+(ucFrameFlag&0x60);
		ucSEQ=ucSEQ+ucSeqCount;//��ȡ֡�ı�־��֡\ĩ֡\��֡\�м�֡����֡���
	}
	else
	{//��������
		ucSeqCount=TerSeq_GetCount();
		ucSeqCount++;
		TerSeq_SetCount(ucSeqCount);
		ucSEQ1=(ucSeqCount & 0x0F);
		ucSEQ=ucFrameFlag&0xF0;
		ucSEQ=ucSEQ1+ucSEQ;
		if(ucConFlag==CON_STATUS_YES)
			ucSEQ |= CON_FRAME_SEQ_CON;							//����������ͣ���CON=1
		else
			ucSEQ &= (~CON_FRAME_SEQ_CON);
	}

	return ucSEQ;
}

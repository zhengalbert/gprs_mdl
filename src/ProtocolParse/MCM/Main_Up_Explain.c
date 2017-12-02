/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_Up_Explain.c
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

//控制字
static unsigned char Get_Frame_Info_Ctrl(unsigned char ucRetMsgType,unsigned char ucUpActive,unsigned char ucSrcMsgType);
static unsigned char Get_Frame_Info_SEQ(unsigned char ucConFlag,STMAINCOMMINFO	*pstMainCommInfo,unsigned char ucFrameFlag);

//终端上送报文处理 
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
	pucSendBuffer[m++]=0;								//帧长度L
	pucSendBuffer[m++]=0;
	pucSendBuffer[m++]=0;
	pucSendBuffer[m++]=0;
	pucSendBuffer[m++]=0x68;

	ucC=Get_Frame_Info_Ctrl(ucRetMsgType,ucUpActive,ucMsgType);
	if(pstMainCommInfo==NULL && ucRetMsgType==CON_TERMSG_CONNECT)					//主动上送去掉ACD，保留告警上送
		ucC &= (~CON_FRAME_CMD_ACD_IMP);
	pucSendBuffer[m++]=ucC;								

	pucSendBuffer[m++]=stTerRegionAddr.ucRegionAddr[0];	
	pucSendBuffer[m++]=stTerRegionAddr.ucRegionAddr[1];
	pucSendBuffer[m++]=stTerAddr.ucDevAddr[0];
	pucSendBuffer[m++]=stTerAddr.ucDevAddr[1];

	if(pstMainCommInfo!=NULL)
		pucSendBuffer[m++]=(pstMainCommInfo->ucMSA & 0xFE);	//返回终端地址，不返回终端组地址
	else
		pucSendBuffer[m++]=0;							//地址域，如主动上送，主站地址填为0
	//add 
//	memcpy(ucLogin,pucSrcBuffer,usDataLen);	//数据区
	if((ucRetMsgType == CON_TERMSG_LOGIN) && (pucSrcBuffer[2] == 0x01))
	{
		ucRetMsgType = CON_TERMSG_ACK;
		usDataLen = 4;
	}
	pucSendBuffer[m++]=ucRetMsgType;					//应用层功能码AFN

	ucSEQ=Get_Frame_Info_SEQ(ucConFlag,pstMainCommInfo,ucFrameFlag);
	if((pstMainCommInfo==NULL) && (ucRetMsgType==CON_TERMSG_REQDATA_1 || ucRetMsgType==CON_TERMSG_REQDATA_2))	//任务数据主动上送
		ucSEQ |= CON_FRAME_SEQ_TPV;						//任务数据主动上送，需要带时间标签
	pucSendBuffer[m++]=ucSEQ;

	memcpy(pucSendBuffer+m,pucSrcBuffer,usDataLen);	//数据区
	m+=usDataLen;
	
	if((ucC & CON_FRAME_CMD_ACD_IMP)==CON_FRAME_CMD_ACD_IMP)
	{													//如果ACD=1则需要增加EC
		pucSendBuffer[m++]=g_ucEventCount_Important;
		pucSendBuffer[m++]=g_ucEventCount_Normal;
	}

	if((ucSEQ & CON_FRAME_SEQ_TPV)==CON_FRAME_SEQ_TPV)	//如果需要附加TPv
	{
		if(pstMainCommInfo!=NULL)						//主站查询直接复制回应
		{
			memcpy(pucSendBuffer+m,pstMainCommInfo->ucTPvInfo,6);
			m+=6;
		}else											//主动上送
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
	ucCheckSum = Get_Sum(pucSendBuffer+6,(unsigned long)(m-6));	//校验和
	pucSendBuffer[m++]=ucCheckSum;
	pucSendBuffer[m++]=0x16;						//帧尾

	(*pusLen)=m;
	m=m-8;											//长度从地址信息开始
//	m=(m<<2)+0x01;
	m=(m<<2)+0x02;
	pucSendBuffer[1]=(unsigned char)m;			//L1			
	pucSendBuffer[2]=(unsigned char)(m>>8);		//L2
	pucSendBuffer[3]=(unsigned char)m;			//L1
	pucSendBuffer[4]=(unsigned char)(m>>8);		//L2

	return RET_SUCCESS;
}

//控制字
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
	case CON_TERMSG_ACK:			//确认否认	//0:认可
		if(ucSrcMsgType==CON_TERMSG_RESET)
			ucC=ucC+0;					//0：确认
		else if(ucSrcMsgType==CON_TERMSG_SETPARA || ucSrcMsgType==CON_TERMSG_CONTROL || ucSrcMsgType==CON_TERMSG_ACK)
			ucC=ucC+8;					//8：响应帧
		else
			ucC=ucC+9;					//9：响应帧，否认：无所召唤的数据
		break;
	case CON_TERMSG_CONNECT:		//登录链接
		if(ucUpActive==CON_STATUS_YES)
			ucC=ucC+9;					//9：链路测试，主动上送
		else
			ucC=ucC+11;					//11：链路测试，应答
		break;
	case CON_TERMSG_GETPARA:		//4类数据发送（响应参数请求）
	case CON_TERMSG_FILETRANS:		//文件传输
	case CON_TERMSG_EXTCMD:			//扩展命令
	case CON_TERMSG_DATATRANS:		//数据转发
	case CON_TERMSG_CONFIGURATION:	//请求终端配置
	case CON_TERMSG_LOGIN:
		ucC=ucC+8;					//8：用户数据
		break;
	case CON_TERMSG_REQDATA_1:		//1类数据发送（实时数据）
	case CON_TERMSG_REQDATA_2:		//2类数据发送（冻结数据）
	case CON_TERMSG_TASKDATA:		//任务数据
	
		if(ucUpActive==CON_STATUS_NO)
			ucC=ucC+8;					//8：用户数据
		else
			ucC=ucC+4;					//4：发送无回答  任务上送
		break;
	case CON_TERMSG_REQDATA_3:		//3类数据发送（事件）
		if(ucUpActive==CON_STATUS_NO)
			ucC=ucC+8;					//8：用户数据
		else
		{
			ucC=ucC+4;					//4：主动上送
			ucC=ucC & (~CON_FRAME_CMD_ACD_IMP);		//去掉事件位
		}
//			ucC=ucC+10;					//10：主动上送，需要确认
		break;
	case CON_TERMSG_RELAYCMD:		//中继站命令
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
		ucSeqCount=(ucSeqCount+ucFrameFlag)&0x0F;//数据帧序号
		ucSEQ &= (~CON_FRAME_SEQ_CON);
		ucSEQ=ucSEQ&0x90+(ucFrameFlag&0x60);
		ucSEQ=ucSEQ+ucSeqCount;//获取帧的标志首帧\末帧\单帧\中间帧及其帧序号
	}
	else
	{//主动上送
		ucSeqCount=TerSeq_GetCount();
		ucSeqCount++;
		TerSeq_SetCount(ucSeqCount);
		ucSEQ1=(ucSeqCount & 0x0F);
		ucSEQ=ucFrameFlag&0xF0;
		ucSEQ=ucSEQ1+ucSEQ;
		if(ucConFlag==CON_STATUS_YES)
			ucSEQ |= CON_FRAME_SEQ_CON;							//如果主动上送，则CON=1
		else
			ucSEQ &= (~CON_FRAME_SEQ_CON);
	}

	return ucSEQ;
}

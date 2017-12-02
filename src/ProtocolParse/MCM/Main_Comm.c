/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_Comm.c
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-02-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"


static STMAINCOMMINFOLIST	stMainCommInfoList;					//��վͨ����Ϣ(Ŀǰ��Ҫ��¼����SEQ��MSA)
static unsigned char		ucSEQListCount;
static unsigned char		g_ucMsgSource;

static unsigned char	Main_Init_CommInfoList(void);
static unsigned char	Main_AddCommInfoList(unsigned char ucSeqNo,STMAINCOMMINFO stMainCommInfo);
static STMAINCOMMINFO	Main_Recv_GetCommInfo(unsigned char *pucRecvBuffer,unsigned short usRecvLen);

unsigned char Init_Main_Comm(void)
{
	unsigned char ucRet= RET_SUCCESS;
	unsigned char ucTerDP_State;
	
	Main_Init_CommInfoList();
	Main_Init_Protocol();

	ucSEQListCount			= 1;
	g_ucMsgSource			= CON_TERMSG_TYPE_INVALID;
	g_usNeedDebugCommInfo	= CON_STATUS_NO_2BYTE;//CON_STATUS_YES_2BYTE;
	usSmsSEQCount			= 0;
	ucTerSEQCount			= 0;
	ucBeatHeartNoAckCount	= 0;
	usHeartBeatCount		= 120;
	ucNeedLoginFlag			= CON_STATUS_NO;
	memset((unsigned char *)usLoginSEQCount,0xFF,(CON_LOGINSEQ_NUM+CON_LOGINSEQ_NUM));

	ucRCommResetCount = 0xFF;
	ucRCommResetHour = 0xFF;
	ucOverFluxLogoutCount = 0xFF;

//azh	gucJob1HoursFlag = 0xFF;
//	gucJobMonthUpFlag = CON_STATUS_NO;

    ucLoginFlag  = CON_STATUS_YES;
    ucLogoffConter = 0xff;
    ucClientNeed = 0;
	Main_RComm_Init_LoginSleepCount();
	
	RunPara_ClearPassErrTimes();

	ucTerDP_State=DP_Get_TerDP_State();
	if(ucTerDP_State!=CON_TERDP_STATE_NORMAL && ucTerDP_State!=CON_TERDP_STATE_DOWNLOAD)
	{
		DP_Set_TerDP_State(CON_TERDP_STATE_NORMAL);
	//	TC35_ModuleDown_SetDownState(CON_TERDP_STATE_NORMAL);
	}

	stRMSendTaskList.ucList_Head_P=0;				//��ʼ����ʱ���Ͷ���
	stRMSendTaskList.ucList_Tail_P=0;
	memset(&(stRMSendTaskList.stRMSendBuffer[0]),0x00,sizeof(STRMSENDBUFFER)*CONRMSENDTASKNUM);
	return ucRet;
}

//���մ�������ڣ���TC35�������
unsigned char Main_Recv_Proc(unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char ucMsgSource)
{
    unsigned char  mem_type, *pret_mem = NULL;
	unsigned char	ucRet=RET_SUCCESS;
//	unsigned char	i=0,j=0;
    unsigned char	ucRetTerMsgNum=0;
	unsigned short	usPos=0,usDataLen=0,usProcDataLen=0;//usDataPos�������ŵ�λ��
	unsigned char	ucJudgeFlag;
	unsigned char	ucLinkFN=0;
	unsigned char	ucMSA;
	STMAINCOMMINFO	stMainCommInfo;
//	STDATETIME		stCurrentTime;
//	unsigned char	ucHours;
	unsigned char	ucIrMsgSourSetFlag;
//	unsigned char	ucDataTransMode=0;
//	STLINKSENDBUFFER *pstLinkSendCache = NULL;
//	STSLAVEADDR stSlaveAddr;
//	STLINKPARA stLinkPara = RunPara_GetLinkPara();

/*************************************
	ucDataTransMode = DataModeConver(pucRecvBuffer);		
	
	
***********************************/
    pret_mem = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(pret_mem == NULL)
    {
        return RET_ERROR;
    }    

	Clr_Group_RetData();
	usDataLen=(pucRecvBuffer[1]>>2)+pucRecvBuffer[2]*0x40;
	ucRet = Main_RecvProc_Judge(pucRecvBuffer,(unsigned short)(usDataLen+8),&usProcDataLen,ucMsgSource);//��ʽ�ж�

	if(ucRet == RET_SUCCESS || ucRet == RET_PASS_ERR)			//������Ϣ�У��������벻Ӧ�л�Ӧ
	{
//		if(ucRet==RET_PASS_ERR)
//		{
//			if((pucRecvBuffer[13] & CON_FRAME_SEQ_TPV)==CON_FRAME_SEQ_TPV)
//azh				Alarm_EnsureJudge_PasswordErr(&pucRecvBuffer[usDataLen-2],pucRecvBuffer[11]);
//			else
//				Alarm_EnsureJudge_PasswordErr(&pucRecvBuffer[usDataLen+4],pucRecvBuffer[11]);
//		}
		Main_Set_MsgSource(ucMsgSource);
		ucJudgeFlag = ucRet;
		ucLinkFN=pucRecvBuffer[6]&0x0F;
		stMainCommInfo=Main_Recv_GetCommInfo(pucRecvBuffer,(unsigned short)(usDataLen+8));		//��ȡ���ͨ����Ϣ
		Main_AddCommInfoList(ucSEQListCount,stMainCommInfo);
		ucMSA=stMainCommInfo.ucMSA;
		Clr_Group_RetExtData();
		usPos=14;
		while(usPos < (usProcDataLen+6) && (ucRet!=RET_MEM_OVER))
		{
			ucRet=Main_Recv_Explain( pucRecvBuffer,(unsigned short)(usDataLen+8),&ucRetTerMsgNum,
									 &usPos,ucMsgSource,ucJudgeFlag,ucSEQListCount);
			if(ucRetTerMsgNum!=0 && ucRetTerMsgNum<=CONTERMSGNUM)								//��Լ���ʹ����ؽ��λ��stTerMsgList[]
			{
				ucIrMsgSourSetFlag=CON_STATUS_YES;
				if(ucMsgSource==CON_TERMSG_SOUR_IRDA)
				{
					//if(RunPara_GetTerMiscParaStatus_ProgramKeyIsSet()!=CON_STATUS_YES)
					if(__FALSE == prog_key_enable())
					{
						if(stTerMsgList[0].ucMsgType==CON_TERMSG_RESET || stTerMsgList[0].ucMsgType==CON_TERMSG_SETPARA || 
							stTerMsgList[0].ucMsgType==CON_TERMSG_CONTROL || stTerMsgList[0].ucMsgType==CON_TERMSG_EXTCMD
							|| stTerMsgList[0].ucMsgType==CON_TERMSG_LOGIN)
							ucIrMsgSourSetFlag=CON_STATUS_NO;
					}
				}
				if(ucIrMsgSourSetFlag==CON_STATUS_YES)
				{
					Main_GetSendData(ucRetTerMsgNum,ucMSA,stMainCommInfo.ucSEQ, pret_mem);
				}
			}
		}
		ucSEQListCount++;
		if(ucSEQListCount>=100)
			ucSEQListCount = 1;
//		if(ucMsgSource==CON_TERMSG_SOUR_RCOMM_SMS || ucMsgSource==CON_TERMSG_SOUR_RCOMM_TCP || ucMsgSource==CON_TERMSG_SOUR_RCOMM_UDP || ucMsgSource==CON_TERMSG_SOUR_RS232)
//		{//������վͨѶ�����������Զ�����ʱ��		���Ӵ�������
//			ucHours=Get_AutoKeepPower_Hours();
//			if(ucHours==0)
//				ClearDateTime(&stCurrentTime);
//			else
//			{
//				stCurrentTime=GetCurrentTime();
//				AddTime(&stCurrentTime,ucHours,CON_TIMEUNIT_HOUR);
//			}
//			Set_AutoKeepPower_Time(&stCurrentTime);
//		}
	}
//azh ��������
//	else if((ucRet == RET_ADDR_ERR) && (stLinkPara.ucSlaveFlag == CON_LINK_MASTER))	//���ն�
//	{
//		for(i=0;i<stLinkPara.ucSlaveNum;i++)
//		{
//			stSlaveAddr = stLinkPara.stSlaveAddr[i];
//			if((pucRecvBuffer[7] == stSlaveAddr.stTerRegionAddr.ucRegionAddr[0]) && (pucRecvBuffer[8] == stSlaveAddr.stTerRegionAddr.ucRegionAddr[1])
//			    &&(pucRecvBuffer[9] == stSlaveAddr.stTerAddr.ucDevAddr[0]) && (pucRecvBuffer[10] == stSlaveAddr.stTerAddr.ucDevAddr[1]))
//				break;
//		}
//		if(i<stLinkPara.ucSlaveNum)
//		{
//		
//			if( stLinkPara.stLinkRemove.ucFlag != CON_AVALIBLE_FLAG)
//			{
//				for(j=0;j<stLinkPara.stLinkRemove.ucTransNum;j++)
//				{
//					if((pucRecvBuffer[9] == stLinkPara.stLinkRemove.stTransAddr[j].ucDevAddr[0]) && (pucRecvBuffer[10] == stLinkPara.stLinkRemove.stTransAddr[j].ucDevAddr[1]))
//						break;				
//				}
//				if(j>=stLinkPara.stLinkRemove.ucTransNum) 
//					ucRet = RET_SUCCESS;
//			}else
//				ucRet = RET_SUCCESS;
//			if(ucRet == RET_SUCCESS)	
//			{
//				if((ucMsgSource == CON_TERMSG_SOUR_RCOMM_TCP) || (ucMsgSource == CON_TERMSG_SOUR_RCOMM_UDP))
//					LinkComm_Add_SendTask(pucRecvBuffer,usRecvLen,ucMsgSource);
//				else if(ucMsgSource == CON_TERMSG_SOUR_RS485)
//				{
//					LinkComm_GetSendCache(pstLinkSendCache);
//					RComm_FillUpData(pucRecvBuffer, usRecvLen, pstLinkSendCache->ucReportType);
//				}
//			}
//		}
//	}

	Manage_Group_ExtData();			//��Ҫ����ȷ���¼�������չ��Ϣ��Ϊȫȷ��/ȫ����ʱ����������Ϣ�����
	Main_SendComm_Recv(ucMsgSource,ucLinkFN,CON_STATUS_NO);
//azh	Alarm_SetFrameACD(CON_STATUS_NO_2BYTE);

    free_mem_block(pret_mem);//Mem_1536_Byte	
	return ucRet;
}

unsigned char Main_GetSendData(unsigned char ucRetTerMsgNum,unsigned char ucMSA,unsigned char ucSEQ, unsigned char *pret_mem)
{
	unsigned char *pucBuf,ucRet=RET_SUCCESS,i,j;
    unsigned char  mem_type, *mem_ptr = NULL;

	if(ucRetTerMsgNum!=0 && ucRetTerMsgNum<=CONTERMSGNUM)								//��Լ���ʹ����ؽ��λ��stTerMsgList[]
	{
		mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr != NULL)
        {    
    		pucBuf=mem_ptr;
    		memset(pucBuf,0xFF,MAX_ALLITEMCONT_LEN);
    		
    		for(i=0;i<ucRetTerMsgNum;i++)
    		{
    			for(j=0;j<stTerMsgList[i].ucDataItemNum;j++)
    			{
    				memcpy(pucBuf,stTerMsgList[i].stDataItem[j].ucDataContent,stTerMsgList[i].stDataItem[j].usDataLen+5);
    				stTerMsgList[i].stDataItem[j].ucDataContent = pucBuf;
    				Main_Recv_Manage(&stTerMsgList[i],j,ucMSA,ucSEQ);
    				pucBuf = pucBuf+stTerMsgList[i].stDataItem[j].usDataLen;
    				if(pucBuf >= (mem_ptr+MAX_ALLITEMCONT_LEN))//azh
    				{
    				    break;
    				}    
    			}
    		}
            ucRet=Main_InsertGroupData(ucRetTerMsgNum, pret_mem);   //���ݴ���
    	
    	    free_mem_block(mem_ptr);//Mem_1536_Byte
    	}
	}
	return ucRet;
}
/********************************************************************************************************/
//����
unsigned char Main_SendComm_Recv(unsigned char ucMsgSource,unsigned char ucLinkFN,unsigned char ucUpActive)
{
    unsigned char  mem_type,*mem_ptr=NULL,frame_f=0;

	unsigned char	ucRet=RET_SUCCESS,i=0,j=0;
	unsigned char	*pucSrcBuffer=NULL,*pucRetBuffer=NULL;
	unsigned short	usSendLen=0;
	STMAINCOMMINFO	stMainCommInfo;
	STMAINCOMMINFO *pstMainCommInfo=NULL;
	unsigned char    iFrameFlag=0x60,iFrameCount=0;

	if(stTerMsgList[0].ucMsgType==CON_TERMSG_ACK)								//��վ��ȷ�Ϸ��ϣ��ն˲���Ҫ��Ӧ
	{
		ucRet = Main_GetCommInfoList(stTerMsgList[0].ucSeqNo,&stMainCommInfo);
		return ucRet;
	}
	if(stTerMsgList[0].ucMsgType==CON_TERMSG_DATATRANS)
	{
		if(stTerMsgList[0].stDataItem[0].ucExeRet==CON_NACK_ERR_NORMAL)
			return ucRet;															//�����м̲��ظ����ȴ���Ч��������Żظ�
		else
			stTerMsgList[0].ucMsgType=CON_TERMSG_ACK;								//����ת�������쳣�ط��ϣ����������쳣���ش�����Բο�
	}
	if(stTerMsgList[0].ucMsgType==CON_TERMSG_FILETRANS)
	{
		if(stTerMsgList[0].stDataItem[0].ucExeRet==CON_NACK_ERR_NORMAL)
			return ucRet;											
		else
			stTerMsgList[0].ucMsgType=CON_TERMSG_ACK;				
	}
	if(stTerMsgList[0].ucMsgType == CON_TERMSG_DATAREQ_SLAVE)
	{
		if(stTerMsgList[0].stDataItem[0].ucExeRet == CON_NACK_ERR_NORMAL)
			return ucRet;
		else
			stTerMsgList[0].ucMsgType = CON_TERMSG_ACK;
	}


	ucRet = Main_GetCommInfoList(stTerMsgList[0].ucSeqNo,&stMainCommInfo);
	if(ucRet == RET_ERROR&&stTerMsgList[0].ucSeqNo!=0xBB)
		return ucRet;
	if(stTerMsgList[0].ucSeqNo!=0xBB)
		pstMainCommInfo=&stMainCommInfo;

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }    
	pucSrcBuffer=(unsigned char *)mem_ptr+16;							//������ʱ������
	pucRetBuffer=(unsigned char *)mem_ptr+16+CON_SEND_BLOCK_MAX_SIZE;	//pucBuffer����CON_SEND_BLOCK_MAX_SIZE
	memset(pucRetBuffer,00,CON_SEND_BLOCK_MAX_SIZE);
	memset(pucSrcBuffer,00,CON_SEND_BLOCK_MAX_SIZE);
	usSendLen=0;
	if(stExplainRetData.stExplainExtData.usLen !=0 && stExplainRetData.stExplainExtData.usLen<32)
	{
		memcpy(pucSrcBuffer,stExplainRetData.stExplainExtData.ucBuff,stExplainRetData.stExplainExtData.usLen);
		usSendLen+=stExplainRetData.stExplainExtData.usLen;
		if(stExplainRetData.ucExplainRetDataNum==0)
		{
			Main_SendExplain(stTerMsgList[0].ucMsgType,pucSrcBuffer,pucRetBuffer,&usSendLen,pstMainCommInfo,ucUpActive,CON_STATUS_NO,iFrameFlag);				//��֡�����ϱ���ͷβ
			Main_FillUpData(pucRetBuffer,usSendLen,ucMsgSource,ucLinkFN);
		}
	}
	for(i=0,j=0;i<stExplainRetData.ucExplainRetDataNum*2;i++)//azh ǰ���Ѿ���֡���� ������Ϸ��������Ϣ���·�֡ ���������֡���Ϊԭ����2��
	{
		if(j < stExplainRetData.ucExplainRetDataNum)
		{    
    		if((usSendLen+stExplainRetData.stExplainRetData[j].usLen) < (CON_SEND_BLOCK_MAX_SIZE-22))
    		{
    			memcpy(pucSrcBuffer+usSendLen,stExplainRetData.stExplainRetData[j].ucBuff,stExplainRetData.stExplainRetData[j].usLen);
    			usSendLen+=stExplainRetData.stExplainRetData[j].usLen;
    			j++;
        		if(j>=stExplainRetData.ucExplainRetDataNum)
        		{
        		    frame_f = 1;    
        		}        			
    		}
    		else
    		{
    		    frame_f = 1;    
    		}    
    	}
    	else if(stExplainRetData.ucExplainRetDataNum != 0)
    	{
    	    frame_f = 1;    
    	}
    	else
    	{   
    	    break;
    	}        
        if(frame_f == 1)
		{
			
			if(iFrameCount==0)//��һ֡
			{
				if(j>=stExplainRetData.ucExplainRetDataNum)//�޺�������
					iFrameFlag=0x60;
				else//�к�������
					iFrameFlag=0x40;//��֡�ĵ�һ֡
				
			}else
			{
				if(j>=stExplainRetData.ucExplainRetDataNum)//û�к���֡
					iFrameFlag=0x20|iFrameCount;			           //��֡�����һ֡
				else//�к���֡
					iFrameFlag=0x00|iFrameCount;                          //��֡���м�֡
			}
			
			iFrameCount++;			
			Main_SendExplain(stTerMsgList[0].ucMsgType,pucSrcBuffer,pucRetBuffer,&usSendLen,pstMainCommInfo,ucUpActive,CON_STATUS_NO,iFrameFlag);				//��֡�����ϱ���ͷβ
			Main_FillUpData(pucRetBuffer,usSendLen,ucMsgSource,ucLinkFN);
			usSendLen=0;
			if(stExplainRetData.stExplainExtData.usLen !=0 && stExplainRetData.stExplainExtData.usLen<32)
			{
				memcpy(pucSrcBuffer,stExplainRetData.stExplainExtData.ucBuff,stExplainRetData.stExplainExtData.usLen);
				usSendLen+=stExplainRetData.stExplainExtData.usLen;
			}
			frame_f = 0;
		}
		if(j>=stExplainRetData.ucExplainRetDataNum)
		{
		    break;    
		}    
	}
    free_mem_block(mem_ptr);//Mem_1536_Byte

	return ucRet;
}

/********************************************************************************************************/
//MainCommInfo��������
unsigned char Main_Init_CommInfoList()
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	for(i=0;i<CON_MAINCOMMINFO_NUM;i++)
	{
		stMainCommInfoList.ucSeqNo[i] = CON_INVALID_SEQNO;
		stMainCommInfoList.stMainCommInfo[i].ucMSA = 0;
		stMainCommInfoList.stMainCommInfo[i].ucSEQ = 0;
		memset(stMainCommInfoList.stMainCommInfo[i].ucTPvInfo,0,6);
	}
	return ucRet;
}

unsigned char Main_AddCommInfoList(unsigned char ucSeqNo,STMAINCOMMINFO stMainCommInfo)
{
	unsigned char ucRet=RET_ERROR,i=0;
	for(i=0;i<CON_MAINCOMMINFO_NUM;i++)
	{
		if(stMainCommInfoList.ucSeqNo[i] == CON_INVALID_SEQNO)
		{
			stMainCommInfoList.ucSeqNo[i]		 = ucSeqNo;
			stMainCommInfoList.stMainCommInfo[i] = stMainCommInfo;
			ucRet=RET_SUCCESS;
			break;
		}
	}
	if(ucRet!=RET_SUCCESS)
	{
		Main_Init_CommInfoList();			//�쳣���ʼ��
		stMainCommInfoList.ucSeqNo[i]		 = ucSeqNo;
		stMainCommInfoList.stMainCommInfo[i] = stMainCommInfo;
		ucRet=RET_SUCCESS;
	}
	return ucRet;
}

unsigned char Main_GetCommInfoList(unsigned char ucSeqNo,STMAINCOMMINFO *pstMainCommInfo)
{
	unsigned char ucRet=RET_ERROR,i=0;

	if( ucSeqNo != CON_INVALID_SEQNO )
	{
		for( i = 0 ; i < CON_MAINCOMMINFO_NUM ; i ++ )
		{
			if( stMainCommInfoList.ucSeqNo[i] == ucSeqNo )
			{
				if(pstMainCommInfo!=NULL)
					(*pstMainCommInfo) = stMainCommInfoList.stMainCommInfo[i];
				stMainCommInfoList.ucSeqNo[i]		 = CON_INVALID_SEQNO;
				stMainCommInfoList.stMainCommInfo[i].ucMSA =0;
				stMainCommInfoList.stMainCommInfo[i].ucSEQ =0;
				memset(stMainCommInfoList.stMainCommInfo[i].ucTPvInfo,0,6);
				ucRet=RET_SUCCESS;
				break;
			}
		}
	}
	return ucRet;
}


STMAINCOMMINFO	Main_Recv_GetCommInfo(unsigned char *pucRecvBuffer,unsigned short usRecvLen)
{
	STMAINCOMMINFO stMainCommInfo;
	unsigned short usDataLen;
	usDataLen=(pucRecvBuffer[1]>>2)+pucRecvBuffer[2]*0x40;
	if(usRecvLen>14)
	{
		stMainCommInfo.ucSEQ = pucRecvBuffer[13];
		stMainCommInfo.ucMSA = pucRecvBuffer[11];
		if((stMainCommInfo.ucSEQ & CON_FRAME_SEQ_TPV)==CON_FRAME_SEQ_TPV)
			memcpy(stMainCommInfo.ucTPvInfo,pucRecvBuffer+usDataLen,6);
	}else
	{
		stMainCommInfo.ucSEQ = 0;
		stMainCommInfo.ucMSA = 0;
	}
	return stMainCommInfo;
}

unsigned char Main_SendComm_DownloadProgram(unsigned char *pucSrcBuf,unsigned short usLen,unsigned char ucMsgSource,unsigned char ucFlag)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	unsigned char *pucRetBuffer,*pucSrcBuffer;
	unsigned short usSendLen;
	STMAINCOMMINFO	stMainCommInfo;
    
    ucRet=Main_GetCommInfoList(stTerMsgList[0].ucSeqNo,&stMainCommInfo);
    if(ucRet==RET_ERROR)
    {    
		return ucRet;
	}
    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucSrcBuffer=(unsigned char *)mem_ptr+16;							//������ʱ������
	pucRetBuffer=(unsigned char *)mem_ptr+16+CON_SEND_BLOCK_MAX_SIZE;	//pucBuffer����CON_SEND_BLOCK_MAX_SIZE
	pucSrcBuffer[0]	= 0x00;					//ucDA1
	pucSrcBuffer[1]	= 0x00;					//ucDA2
	pucSrcBuffer[2]	= 0x08;					//ucDT1
	pucSrcBuffer[3]	= 0x0C;					//ucDT2
	memcpy(pucSrcBuffer+4,pucSrcBuf,usLen);
	usSendLen=usLen+4;
	Main_SendExplain(CON_TERMSG_FILETRANS,pucSrcBuffer,pucRetBuffer,&usSendLen,&stMainCommInfo,CON_STATUS_NO,CON_STATUS_NO,0x60);		//��֡�����ϱ���ͷβ
	Main_FillUpData(pucRetBuffer,usSendLen,ucMsgSource,11);
    
    free_mem_block(mem_ptr);
	return ucRet;
}

unsigned char Main_FillUpData(unsigned char *pucSendBuffer,unsigned short usLen,unsigned char ucMsgSource,unsigned char ucLink)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucFlag=0x55;
//	unsigned short m=0,i=0;
//	unsigned char ucSmsBuf[140];

	if(ucLink!=4)						//4Ϊ�㲥������Ӧ�������������Ϣ����Ҫ����
	{
		switch(ucMsgSource)
		{
		case CON_TERMSG_SOUR_RCOMM_SMS:
//			if(usLen<138*CONTC35UPLISTNUM)
//			{
//				for(i=0;i<CONTC35UPLISTNUM;i++)
//				{
//					ucSmsBuf[0]=(unsigned char)usSmsSEQCount;
//					ucSmsBuf[1]=(unsigned char)(usSmsSEQCount>>8);
//					ucSmsBuf[1]=ucSmsBuf[1]+(unsigned char)(i<<4);
//					if(m+138>=usLen)
//					{//���һ֡
//						ucSmsBuf[1]=ucSmsBuf[1]+0x80;
//						memcpy(ucSmsBuf+2,pucSendBuffer+m,(unsigned char)(usLen-m));
//						RComm_FillUpData(ucSmsBuf,(unsigned short)(usLen-m+2),CON_TERMSG_SOUR_RCOMM_SMS);
//						break;
//					}else
//					{
//						memcpy(ucSmsBuf+2,pucSendBuffer+m,138);
//						RComm_FillUpData(ucSmsBuf,140,CON_TERMSG_SOUR_RCOMM_SMS);
//					}
//					m+=138;
//				}
//			}
//			usSmsSEQCount++;
			break;

            case CON_TERMSG_SOUR_ZIGBEE://azh 171017��Դ��zigbee����ʽ��ѹ��GPRSģ��
			case CON_TERMSG_SOUR_IRDA:
			case CON_TERMSG_SOUR_RS232_0:
			case CON_TERMSG_SOUR_RS232:
			case CON_TERMSG_SOUR_RS485:
                LocalCom_UpSendData(pucSendBuffer, usLen,ucMsgSource);
				break;
			case CON_TERMSG_SOUR_RCOMM_TCP:
			case CON_TERMSG_SOUR_RCOMM_1:
			case CON_TERMSG_SOUR_RCOMM_2:
			case CON_TERMSG_SOUR_RCOMM_3:
                RComm_FillUpData(pucSendBuffer,usLen,CON_TERMSG_SOUR_RCOMM_TCP);//ucChanNo);	
				break;					
		case CON_TERMSG_SOUR_RCOMM_UDP:
			RComm_FillUpData(pucSendBuffer,usLen,CON_TERMSG_SOUR_RCOMM_UDP);
			break;
		default:
//			DbgComm_RS232_AddSendTaskList(pucSendBuffer,usLen);
            LocalCom_UpSendData(pucSendBuffer, usLen,CON_TERMSG_SOUR_RS232);
        ucFlag=0xAA;
			break;
		}
	}
	if(g_usNeedDebugCommInfo==CON_STATUS_YES_2BYTE &&				//��Ҫ��������Ϣ
		ucMsgSource!=CON_TERMSG_SOUR_RS232 && ucMsgSource!=CON_TERMSG_SOUR_RS232_0 && ucMsgSource!=CON_TERMSG_SOUR_IRDA//azh 20130903
		&& ucFlag==0x55)
    {    
		//DbgComm_RS232_AddSendTaskList(pucSendBuffer,usLen);	//��䷢�Ͷ��У�Ŀǰֻ��DEBUGͨ�����Ժ���Ҫ������Դת��
        LocalCom_UpSendData(pucSendBuffer, usLen,CON_TERMSG_SOUR_RS232);
    }
	return ucRet;
}

/****************************************************************************
�������ƣ�GPRS ģ������֡����
����������
��    �룺��
��    ������
��    �أ�
ȫ�ֱ�������
����ģ�飺��
****************************************************************************/
unsigned char Main_SendComm_CommConnect(unsigned char ucCmdType)
{
    unsigned char  mem_type, *mem_ptr = NULL;
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char *pucRetBuffer,*pucSrcBuffer;
	unsigned short usSendLen;
	unsigned char ucCommType=CON_TERMSG_SOUR_RCOMM_TCP;
	
#ifdef PROTOCOL_TYPE_376_2013	
	unsigned char tmp,m;
	STDATETIME	stCurrentTime;//azh 1700908
#endif//PROTOCOL_TYPE_376_2013
	
	STTERADJPARA stTerAdjPara;
	stTerAdjPara=RunPara_GetTerAdjPara();

    mem_ptr = alloc_comm_mem(MEM_64_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }    

	pucSrcBuffer=(unsigned char *)mem_ptr+16;							//������ʱ������
	pucRetBuffer=(unsigned char *)mem_ptr+16+CON_SEND_BLOCK_MAX_SIZE;	//pucBuffer����CON_SEND_BLOCK_MAX_SIZE
	pucSrcBuffer[0]	= 0x00;					//ucDA1
	pucSrcBuffer[1]	= 0x00;					//ucDA2
	if(ucCmdType==1)
		pucSrcBuffer[2]	= 0x01;				//ucDT1		//��½
	else if(ucCmdType==2)
		pucSrcBuffer[2]	= 0x02;
	else
		pucSrcBuffer[2]	= 0x04;				//ucDT1		//����
	pucSrcBuffer[3]	= 0x00;					//ucDT2
	usSendLen=4;
	if(ucCmdType==1)
	{
		if(ucNeedLoginFlag==CON_STATUS_YES)
		{
			Main_SendExplain(CON_TERMSG_CONNECT,pucSrcBuffer,pucRetBuffer,&usSendLen,NULL,CON_STATUS_YES,CON_STATUS_YES,0x60);	//��½��Ҫ��վȷ��
			ucCommType=Main_Get_Comm_Type();
			if(ucCommType!=CON_TERMSG_SOUR_RCOMM_SMS)
			{
				Main_FillUpData(pucRetBuffer,usSendLen,ucCommType,9);
				for(i=0;i<CON_LOGINSEQ_NUM;i++)
				{
					if(usLoginSEQCount[i]==0xFFFF)
					{
						usLoginSEQCount[i]=TerSeq_GetCount();
						break;
					}
				}
				if(i==CON_LOGINSEQ_NUM)
				{
					memset((unsigned char *)usLoginSEQCount,0xFF,(CON_LOGINSEQ_NUM+CON_LOGINSEQ_NUM));
					usLoginSEQCount[0]=TerSeq_GetCount();
				}
				//�����¼����Ҫȷ�ϣ����¼ֻ��һ��				
				if(stTerAdjPara.ucLoginNeedAck!=CON_STATUS_YES)
				{
					ucNeedLoginFlag=CON_STATUS_NO;
				}
			}
		}
	}else if(ucCmdType==2)
	{
		Main_SendExplain(CON_TERMSG_CONNECT,pucSrcBuffer,pucRetBuffer,&usSendLen,NULL,CON_STATUS_YES,CON_STATUS_YES,0x60);	//��½��Ҫ��վȷ��
		ucCommType=Main_Get_Comm_Type();
		if(ucCommType!=CON_TERMSG_SOUR_RCOMM_SMS)
		{
			Main_FillUpData(pucRetBuffer,usSendLen,ucCommType,9);
		}
	}else
	{
		if(ucBeatHeartNoAckCount<CON_BEATHEARTNOACK_MAXNUM)
		{
#ifdef PROTOCOL_TYPE_376_2013
		m = usSendLen;
		stCurrentTime=GetCurrentTime();
		pucSrcBuffer[m++]=(unsigned char)HEX_TO_BCD(stCurrentTime.ucSecond,1);
		pucSrcBuffer[m++]=(unsigned char)HEX_TO_BCD(stCurrentTime.ucMinute,1);
		pucSrcBuffer[m++]=(unsigned char)HEX_TO_BCD(stCurrentTime.ucHour,1);
		pucSrcBuffer[m++]=(unsigned char)HEX_TO_BCD(stCurrentTime.ucDay,1);
		tmp = GetDateTime_Week();
		tmp <<= 5;
		pucSrcBuffer[m] = tmp;
		tmp = (unsigned char)HEX_TO_BCD(stCurrentTime.ucMonth,1);
		tmp &= 0x1F;
		pucSrcBuffer[m++] |= tmp;
		pucSrcBuffer[m++]=(unsigned char)HEX_TO_BCD(stCurrentTime.ucYear,1);
		usSendLen = m;
#endif//PROTOCOL_TYPE_376_2013
			
			Main_SendExplain(CON_TERMSG_CONNECT,pucSrcBuffer,pucRetBuffer,&usSendLen,NULL,CON_STATUS_YES,CON_STATUS_YES,0x60);
			ucCommType=Main_Get_Comm_Type();
			if(ucCommType!=CON_TERMSG_SOUR_RCOMM_SMS)
			{
				Main_FillUpData(pucRetBuffer,usSendLen,ucCommType,9);
			}
			if(stTerAdjPara.ucBeatHeartNeedAck==CON_STATUS_YES)
			{
				if(DP_Get_TerDP_State()!=CON_TERDP_STATE_DOWNLOAD)
					ucBeatHeartNoAckCount++;			//��Ҫ��վȷ������
			}
		}else
		{
			//CommConnect_SetNeedLoginFlag(CON_STATUS_YES);		//�ñ�־�ɽ��յ���վȷ�Ϻ�����ΪCON_STATUS_NO
			//HeartBeat_SetCount(20);
			ucBeatHeartNoAckCount=0;
			RComm_ResetStart();
			Init_Main_Comm();
		}
	}
    free_mem_block(mem_ptr);//Mem_64_Byte	

	return ucRet;
}

void TerSeq_SetCount(unsigned char ucCount)
{
	ucTerSEQCount=ucCount;
}

void Main_Set_MsgSource(unsigned char ucMsgSource)
{
	g_ucMsgSource=ucMsgSource;
}

unsigned char Main_Get_MsgSource(void)
{
	return g_ucMsgSource;
}

unsigned char Check_CurrentLoginSeq_IsOk(void)
{
	unsigned char ucRet=RET_ERROR;
	unsigned char i=0,j=0;
	unsigned short usTmpSeq;
	for(i=0;i<CON_MAINCOMMINFO_NUM;i++)
	{
		if(stMainCommInfoList.ucSeqNo[i] == ucSEQListCount)
		{
			usTmpSeq=stMainCommInfoList.stMainCommInfo[i].ucSEQ;
			for(j=0;j<CON_LOGINSEQ_NUM;j++)
			{
				if((usLoginSEQCount[j]&0x000F)==(usTmpSeq&0x000F))
				{
					usLoginSEQCount[j]=0xFFFF;			//�ҵ��ͳ�ʼ��
					break;
				}
			}
			if(j<CON_LOGINSEQ_NUM)
				ucRet=RET_SUCCESS;
			break;
		}
	}
	return ucRet;
}
unsigned char Main_Get_TmpUpSendTask(void)
{//��ȡ��ʱ���Ͷ��У������
	unsigned char ucSeq=0xFF;
	unsigned char ucTailNo=stRMSendTaskList.ucList_Tail_P;
	unsigned char ucHeadNo=stRMSendTaskList.ucList_Head_P;
	if(ucTailNo!=ucHeadNo )
	{
		if(ucTailNo<CONRMSENDTASKNUM)
		{
			ucSeq=ucTailNo;
			stRMSendTaskList.ucList_Tail_P++;
			if(stRMSendTaskList.ucList_Tail_P>=CONRMSENDTASKNUM)
				stRMSendTaskList.ucList_Tail_P=0;
		}
	}
	return ucSeq;
}

unsigned char Main_Fill_TmpUpSendTask(unsigned char *pucSendBuffer,unsigned short usLen,unsigned char ucLinkFN)
{//��д��ʱ���Ͷ���
	unsigned char ucRet=RET_SUCCESS;
	if(usLen>640)
		return ucRet;
	if(pucSendBuffer!=NULL && stRMSendTaskList.ucList_Head_P<CONRMSENDTASKNUM)
	{
		if( (stRMSendTaskList.ucList_Head_P +1 == stRMSendTaskList.ucList_Tail_P) ||
			(stRMSendTaskList.ucList_Head_P +1 == CONRMSENDTASKNUM && stRMSendTaskList.ucList_Tail_P==0))
		{
		}else
		{
			stRMSendTaskList.stRMSendBuffer[stRMSendTaskList.ucList_Head_P].Send_Len=usLen;
			stRMSendTaskList.stRMSendBuffer[stRMSendTaskList.ucList_Head_P].ucLink=ucLinkFN;
			memcpy(&(stRMSendTaskList.stRMSendBuffer[stRMSendTaskList.ucList_Head_P].Send_Buffer[0]),pucSendBuffer,usLen);
			stRMSendTaskList.ucList_Head_P++;
			if(stRMSendTaskList.ucList_Head_P>=CONRMSENDTASKNUM)
				stRMSendTaskList.ucList_Head_P=0;
		}
	}
	return ucRet;
}
/******************************************************
���ܣ����ݴ���ģʽ�жϣ����Ҹ��ݴ���ģʽ��������ת��

����
���أ���ǰ���ݴ���ģʽ
******************************************************
unsigned char DataModeConver(unsigned char *pucRecvBuffer,unsigned short usDataLength)
{
	unsigned char ucRet;
	unsigned char ucTransMode;			//���ݴ���ģʽ(���ܣ�ѹ��������)
	unsigned short usDataLen;			//ԭ���ݳ���

	usDataLen=(pucRecvBuffer[1]>>2)+pucRecvBuffer[2]*0x40;
	ucTransMode = (pucRecvBuffer[1] & 0x03)
	if (ucTransMode == CON_NEEDCIPHER)
	{
		usDataLength = usDataLen;


	}
	else if (ucTransMode == CON_NEEDZIP)
	{

	}
	else if (ucTransMode == CON_NEEDCIPHERZIP )
	{
	
	}
	else 
	{
		ucTransMode = CON_NOTCIPHERZIP;
		usDataLength = usDataLen;
	}
	ucRet = ucTransMode;
	return ucRet;

}
*/

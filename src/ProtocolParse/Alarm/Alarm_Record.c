/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Alarm_Record.c
 Description    : 告警日志相关处理
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

static unsigned char ucNowExeAlarmRecordNo_Important;					//当前正在处理的重要告警记录
static unsigned char ucNowExeAlarmRecordNo_Normal;						//当前正在处理的一般告警记录
static unsigned short	g_usFrameACD;

static unsigned char Main_SendComm_Alr(unsigned char ucAlrType,unsigned char ucNowExeAlrRecordNo,unsigned char ucNowAlrNo_Imp,unsigned char ucNowAlrNo_Nor,STALRRECORD *pstAlrRecord,unsigned char *pucSEQ);
static unsigned char Alarm_SaveRecord(unsigned char ucAlrRecordNo,STALRRECORD *pstAlrRecord,unsigned char ucAlrType);
static unsigned char Main_Comm_GetDT1_Alr(unsigned char ucAlrType);
static unsigned char Alarm_SetAlrHappen(unsigned char ucAlrERCCode);
static unsigned char Fill_AlrNeedAckInfo(STALRNEEDACKINFOLIST  *pstAlrNeedAckInfo,unsigned char ucSEQ,unsigned char ucAlrRecordNo);

//读取历史告警记录
//unsigned char Alarm_GetAlrRecordList(void)
//{
//	unsigned char ucRet=RET_ERROR;
//	if(	stAlrRecordInfo.ucAlrValidFlag[0]!=0x55 || stAlrRecordInfo.ucAlrValidFlag[1]!=0xAA	|| 
//		stAlrRecordInfo.ucAlrValidFlag[2]!=0x55	|| stAlrRecordInfo.ucAlrValidFlag[3]!=0xAA 	|| 
//		stAlrRecordInfo.ucAlrValidFlag[4]!=0x55	|| stAlrRecordInfo.ucAlrValidFlag[5]!=0xAA	|| 
//		stAlrRecordInfo.ucAlrValidFlag[6]!=0x55	|| stAlrRecordInfo.ucAlrValidFlag[7]!=0xAA)
//	{
//		Alarm_InitAlrRecordInfo();
//		Alarm_Init_AlrNeedAckInfoGroup();
//		g_usFrameACD = CON_STATUS_NO_2BYTE;
//		g_ucEventCount_Important=0;
//		g_ucEventCount_Normal=0;
//	}else
//	{
//		if(stAlrRecordInfo.stAlrRecordInfo_Imp.ucList_Tail_P!=stAlrRecordInfo.stAlrRecordInfo_Imp.ucList_Proc_Tail_P)
//			g_usFrameACD = CON_STATUS_YES_2BYTE;
//		else
//			g_usFrameACD = CON_STATUS_NO_2BYTE;
//		g_ucEventCount_Important=stAlrRecordInfo.stAlrRecordInfo_Imp.ucList_Head_P;
//		g_ucEventCount_Normal=stAlrRecordInfo.stAlrRecordInfo_Norm.ucList_Head_P;
//	}
//
//	ucNowExeAlarmRecordNo_Important	=stAlrRecordInfo.stAlrRecordInfo_Imp.ucList_Proc_Tail_P;
//	ucNowExeAlarmRecordNo_Normal	=stAlrRecordInfo.stAlrRecordInfo_Norm.ucList_Proc_Tail_P;
//
//	return ucRet;
//}

//告警记录存储,将格式转化成规约的格式
unsigned char AddToAlrRecordList(STALRENSUREJUDGE *pstAlrEnsureJudge,unsigned char ucUpFlag)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucAlrType;
	unsigned char ucArrayNo=0;
	unsigned char ucMaxAlrNum=0;
	STALRRECORDINDEXINFO *pstAlrRecordInfo=NULL;
	unsigned char ucDP_Stat,num_perblock=0;
	unsigned char	*pucNowExeAlarmRecordNo;

 	unsigned short	usBeginBlock=0;
	STPERALRAUXINFO stAlarmAuxInfo;
	
	if(pstAlrEnsureJudge!=NULL)
	{
		ucAlrType=Get_Shield_ACD_byAlrCode(pstAlrEnsureJudge->usAlrCode,CON_ALR_ACDFLAG);			//根据告警编码获得告警等级
		ucMaxAlrNum = Alarm_GetRecordMaxNum(ucAlrType);
		num_perblock = MCP_FLASH_BLOCK_SIZE/sizeof(STALRRECORD);
		if(ucAlrType==CON_ALR_ACD)			//重要事件
		{
//			ucMaxAlrNum=CONALRRECORDNUM_IMPORTANT;
			pstAlrRecordInfo=&stAlrRecordInfo.stAlrRecordInfo_Imp;
			pucNowExeAlarmRecordNo=&ucNowExeAlarmRecordNo_Important;			//当前处理的告警记录
			usBeginBlock = CON_IMPALARM_START_BLOCK;
		}
		else if(ucAlrType==CON_ALR_ACD_NOT)	//普通事件
		{
//			ucMaxAlrNum=CONALRRECORDNUM_NORMAL;
			pstAlrRecordInfo=&stAlrRecordInfo.stAlrRecordInfo_Norm;
			pucNowExeAlarmRecordNo=&ucNowExeAlarmRecordNo_Normal;
			usBeginBlock=CON_NORMALARM_START_BLOCK;
		}
		ucDP_Stat=DP_Get_TerDP_State();
		if((ucAlrType==CON_ALR_ACD || ucAlrType==CON_ALR_ACD_NOT) && ucDP_Stat!=CON_TERDP_STATE_DOWNLOAD)
		{
			ucArrayNo=pstAlrRecordInfo->ucList_Head_P;						//计算记录编号
			(*pucNowExeAlarmRecordNo)=ucArrayNo;							//需要立即处理
			pstAlrRecordInfo->ucList_Head_P++;
			if(pstAlrRecordInfo->ucList_Head_P>=ucMaxAlrNum)
				pstAlrRecordInfo->ucList_Head_P=0;
			if((pstAlrRecordInfo->ucList_Head_P % num_perblock) == 0)//与每block首地址对齐要擦除
			{
			    usBeginBlock += pstAlrRecordInfo->ucList_Head_P/num_perblock;
			    MCP_Flash_Sector_Erase(usBeginBlock*MCP_FLASH_BLOCK_SIZE);
			}
			if(pstAlrRecordInfo->ucList_Head_P==pstAlrRecordInfo->ucList_Tail_P)
			{
				//如果和擦除的在同块就重新对齐
				if((pstAlrRecordInfo->ucList_Proc_Tail_P/num_perblock)==(pstAlrRecordInfo->ucList_Tail_P/num_perblock))
				{
					//pstAlrRecordInfo->ucList_Proc_Tail_P++;
					pstAlrRecordInfo->ucList_Proc_Tail_P = ((pstAlrRecordInfo->ucList_Proc_Tail_P/num_perblock) +1)*num_perblock;
    				if(pstAlrRecordInfo->ucList_Proc_Tail_P>=ucMaxAlrNum)
    				{
    					pstAlrRecordInfo->ucList_Proc_Tail_P = 0;
    				}					
				}
                //pstAlrRecordInfo->ucList_Tail_P++;
				pstAlrRecordInfo->ucList_Tail_P += num_perblock;
				if(pstAlrRecordInfo->ucList_Tail_P>=ucMaxAlrNum)
				{
				    pstAlrRecordInfo->ucList_Tail_P=0;
				}

			}
		    Alarm_SaveManageInfo(ucAlrType);//azh
		    
			ClearAlrRecord(&stAlrRecordTemp);
			ClearAlrRecordAuxInfo(&stAlarmAuxInfo);
			
			stAlrRecordTemp.ucUpFlag	=ucUpFlag;
			stAlrRecordTemp.ucAlrERCCode=Alarm_GetAlrERCNo(pstAlrEnsureJudge->usAlrCode);
			Alarm_SetAlrHappen(stAlrRecordTemp.ucAlrERCCode);
			stAlrRecordTemp.ucMPNo		=pstAlrEnsureJudge->ucMPNo;
			stAlrRecordTemp.stDateTime	=pstAlrEnsureJudge->stDateTime;
			stAlrRecordTemp.stNextUpDateTime	=GetCurrentTime();
			ucRet=Alarm_DataConvert(&stAlrRecordTemp,pstAlrEnsureJudge);		//告警数据格式转换
			if(ucRet==RET_SUCCESS)
			{
				Alarm_SaveRecord(ucArrayNo,&stAlrRecordTemp,ucAlrType);
				stAlarmAuxInfo.stAlrAuxInfo.ucUpFlag = ucUpFlag;
				stAlarmAuxInfo.stAlrAuxInfo.ucUpCount = 0;//没有上报过 上报次数为0
			}
		    Alarm_SaveRecordAuxInfo(ucArrayNo, &stAlarmAuxInfo, ucAlrType);//保存这条告警记录相关上报的辅助信息
		}
	}
	return ucRet;
}

unsigned char Alarm_UpSend_Manage(unsigned char ucAlrType)
{
	unsigned char	ucRet=RET_SUCCESS;
	unsigned long	lCurrTime=0,lNextUpTime=0;
	unsigned char	ucMaxAlrNum=0;
	unsigned char	*pucNowExeAlarmRecordNo;
	unsigned char   ucSEQ=0;
	STDATETIME		stCurrentTime=GetCurrentTime();
	STDATETIME		stTimeTemp;
	STALRRECORDINDEXINFO *pstAlrRecordInfo=NULL;
	STALRNEEDACKINFOLIST  *pstAlrNeedAckInfo=NULL;
	STTERMISCPARA	stTerMiscPara;
	STTERCOMMPARA	stTerCommPara=RunPara_GetTerCommPara();
	
	unsigned short	usReUpSendInter=stTerCommPara.usACKOvertime;			//单位秒
	unsigned char	ucReUpSendTimes=stTerCommPara.ucReSendtimes;
	STPERALRAUXINFO stAlarmAuxInfo;
	
	
	if(usAlrActionLockTimer<CON_ALRACTION_ALRUP)							//上电一定时候后才能上送
		return ucRet;
//	if(CommConnect_GetNeedLoginFlag()==CON_STATUS_YES)						//登陆后被主站确认才能上送，没有tcp则允许上送告警
//		return ucRet;

	ucMaxAlrNum = Alarm_GetRecordMaxNum(ucAlrType);
	if(ucAlrType==CON_ALR_ACD)
	{
		pucNowExeAlarmRecordNo	=&ucNowExeAlarmRecordNo_Important;			//当前处理的告警记录
		pstAlrRecordInfo		=&stAlrRecordInfo.stAlrRecordInfo_Imp;		//重要告警的信息
//		ucMaxAlrNum				=CONALRRECORDNUM_IMPORTANT;					//数量
		pstAlrNeedAckInfo		=&stAlrNeedAckInfoGroup.stAlrNeedAckInfo_Imp;
	}
	else if(ucAlrType==CON_ALR_ACD_NOT)
	{
		pucNowExeAlarmRecordNo	=&ucNowExeAlarmRecordNo_Normal;
		pstAlrRecordInfo		=&stAlrRecordInfo.stAlrRecordInfo_Norm;
//		ucMaxAlrNum				=CONALRRECORDNUM_NORMAL;
		pstAlrNeedAckInfo		=&stAlrNeedAckInfoGroup.stAlrNeedAckInfo_Nor;
	}
	
	if((ucAlrType==CON_ALR_ACD) || (ucAlrType==CON_ALR_ACD_NOT))
	{
		ClearAlrRecord(&stAlrRecordTemp);					//初始化 stAlrRecordTemp
//		ucRet=Alarm_QueryRecord((*pucNowExeAlarmRecordNo),&stAlrRecordTemp,ucAlrType);
        ucRet=Alarm_QueryRecordAuxInfo((*pucNowExeAlarmRecordNo),&stAlarmAuxInfo,ucAlrType);
		if(ucRet==RET_SUCCESS)
		{
			if(stAlarmAuxInfo.stAlrAuxInfo.ucUpFlag==CONALR_UPFLAG_NEED)		//第一次上送
			{
				if(ucReUpSendTimes==0)
					stAlarmAuxInfo.stAlrAuxInfo.ucUpFlag=CONALR_UPFLAG_NONE;
				else
					stAlarmAuxInfo.stAlrAuxInfo.ucUpFlag=CONALR_UPFLAG_DOING;
				stTerMiscPara=RunPara_GetTerMiscPara();
				if(stTerMiscPara.ucUpActiveOn!=CON_INVALIBLE_FLAG)
				{
					ucRet=Alarm_QueryRecord((*pucNowExeAlarmRecordNo),&stAlrRecordTemp,ucAlrType);
					if(ucRet==RET_SUCCESS)
					{
    					Main_SendComm_Alr(ucAlrType,(*pucNowExeAlarmRecordNo),g_ucEventCount_Important,
    					g_ucEventCount_Normal,&stAlrRecordTemp,&ucSEQ);
        				if(stTerCommPara.ucServerNeedAck[0]!=CON_STATUS_YES && ucAlrType==CON_ALR_ACD)
        					stAlarmAuxInfo.stAlrAuxInfo.ucUpFlag=CONALR_UPFLAG_NONE;			//不需要确认则只发送一次，不再上送
        				if(stTerCommPara.ucServerNeedAck[1]!=CON_STATUS_YES && ucAlrType!=CON_ALR_ACD)
        					stAlarmAuxInfo.stAlrAuxInfo.ucUpFlag=CONALR_UPFLAG_NONE;
                        if(stAlarmAuxInfo.stAlrAuxInfo.ucUpFlag==CONALR_UPFLAG_DOING)
                        {
                            Fill_AlrNeedAckInfo(pstAlrNeedAckInfo,ucSEQ,(*pucNowExeAlarmRecordNo));	    
                        }
        				//AddTime(&stCurrentTime,usReUpSendInter,CON_TIMEUNIT_SEC);
        				stAlarmAuxInfo.stAlrAuxInfo.ucUpCount++;    						
//				        stAlrRecordTemp.stNextUpDateTime=stCurrentTime;
//				        Alarm_SaveRecord((*pucNowExeAlarmRecordNo),&stAlrRecordTemp,ucAlrType);	//以后可以仅仅存储前面
    				}
    				else
    				{
    				    stAlarmAuxInfo.stAlrAuxInfo.ucUpFlag=CONALR_UPFLAG_NONE;
    				}		
				}
				Alarm_SaveRecordAuxInfo((*pucNowExeAlarmRecordNo),&stAlarmAuxInfo,ucAlrType);
			}else if(stAlarmAuxInfo.stAlrAuxInfo.ucUpFlag==CONALR_UPFLAG_DOING)
			{
				ucRet=Alarm_QueryRecord((*pucNowExeAlarmRecordNo),&stAlrRecordTemp,ucAlrType);
				if(ucRet==RET_SUCCESS)
				{
    				lCurrTime=TimeConver(&stCurrentTime,CON_TIMEUNIT_SEC);
//    				lNextUpTime=TimeConver(&(stAlrRecordTemp.stNextUpDateTime),CON_TIMEUNIT_SEC);
    				stTimeTemp=stAlrRecordTemp.stDateTime;
    				if(stAlarmAuxInfo.stAlrAuxInfo.ucUpCount > ucReUpSendTimes)
    				{
    				    stAlarmAuxInfo.stAlrAuxInfo.ucUpCount = ucReUpSendTimes;
    				}
    				AddTime(&stTimeTemp,stAlarmAuxInfo.stAlrAuxInfo.ucUpCount*usReUpSendInter,CON_TIMEUNIT_SEC);
    				lNextUpTime=TimeConver(&stTimeTemp,CON_TIMEUNIT_SEC);
    				
    				if(lCurrTime>=lNextUpTime)
    				{//重发时间到
    					stTerMiscPara=RunPara_GetTerMiscPara();
    					if(stTerMiscPara.ucUpActiveOn!=CON_INVALIBLE_FLAG)
    					{
    						Main_SendComm_Alr(ucAlrType,(*pucNowExeAlarmRecordNo),g_ucEventCount_Important,
    						g_ucEventCount_Normal,&stAlrRecordTemp,&ucSEQ);
    					}
                        stAlarmAuxInfo.stAlrAuxInfo.ucUpCount++;
    					if(stTerCommPara.ucServerNeedAck[0]!=CON_STATUS_YES && ucAlrType==CON_ALR_ACD)
    						stAlarmAuxInfo.stAlrAuxInfo.ucUpFlag=CONALR_UPFLAG_NONE;			//不需要确认则只发送一次，不再上送
    					if(stTerCommPara.ucServerNeedAck[1]!=CON_STATUS_YES && ucAlrType!=CON_ALR_ACD)
    						stAlarmAuxInfo.stAlrAuxInfo.ucUpFlag=CONALR_UPFLAG_NONE;
    					if(stAlarmAuxInfo.stAlrAuxInfo.ucUpFlag!=CONALR_UPFLAG_NONE)
    					{
                            Fill_AlrNeedAckInfo(pstAlrNeedAckInfo,ucSEQ,(*pucNowExeAlarmRecordNo));
                        }	
//    					AddTime(&stCurrentTime,usReUpSendInter,CON_TIMEUNIT_SEC);
//    					stAlrRecordTemp.stNextUpDateTime=stCurrentTime;
//    					Alarm_SaveRecord((*pucNowExeAlarmRecordNo),&stAlrRecordTemp,ucAlrType);	//以后可以仅仅存储前面
                        Alarm_SaveRecordAuxInfo((*pucNowExeAlarmRecordNo),&stAlarmAuxInfo,ucAlrType);
    				}
    				
    				stTimeTemp=stAlrRecordTemp.stDateTime;
    				AddTime(&stTimeTemp,usReUpSendInter*ucReUpSendTimes,CON_TIMEUNIT_SEC);
    				if(lCurrTime>=TimeConver(&stTimeTemp,CON_TIMEUNIT_SEC))
    				{//过期不再上送
    					stAlarmAuxInfo.stAlrAuxInfo.ucUpFlag=CONALR_UPFLAG_NONE;		
//    					Alarm_SaveRecord((*pucNowExeAlarmRecordNo),&stAlrRecordTemp,ucAlrType);					//以后可以仅仅存储前面
    					Alarm_SaveRecordAuxInfo((*pucNowExeAlarmRecordNo),&stAlarmAuxInfo,ucAlrType);
    					Alarm_DeleteAlrNeedAckInfo((*pucNowExeAlarmRecordNo),ucAlrType);
    					if((*pucNowExeAlarmRecordNo)==pstAlrRecordInfo->ucList_Proc_Tail_P)
    					{
    						pstAlrRecordInfo->ucList_Proc_Tail_P++;
    						if(pstAlrRecordInfo->ucList_Head_P>=pstAlrRecordInfo->ucList_Tail_P)
    						{//无指针回绕
    							if(pstAlrRecordInfo->ucList_Proc_Tail_P>=pstAlrRecordInfo->ucList_Head_P)
    								pstAlrRecordInfo->ucList_Proc_Tail_P= pstAlrRecordInfo->ucList_Head_P;
    						}else	
    						{//头指针回绕
    							if(pstAlrRecordInfo->ucList_Proc_Tail_P>=ucMaxAlrNum)
    								pstAlrRecordInfo->ucList_Proc_Tail_P=0;
    							if((pstAlrRecordInfo->ucList_Proc_Tail_P>= pstAlrRecordInfo->ucList_Head_P)&&
    								(pstAlrRecordInfo->ucList_Proc_Tail_P< pstAlrRecordInfo->ucList_Tail_P ))
    							{//指针约束
    								pstAlrRecordInfo->ucList_Proc_Tail_P = pstAlrRecordInfo->ucList_Head_P;
    							}
    						}
    					}
    				}
			    }
		        else
				{
				    Alarm_DeleteAlrNeedAckInfo((*pucNowExeAlarmRecordNo),ucAlrType);
				    ClearAlrRecordAuxInfo(&stAlarmAuxInfo);
				    Alarm_SaveRecordAuxInfo((*pucNowExeAlarmRecordNo),&stAlarmAuxInfo,ucAlrType);
				}	
			}
		}
		(*pucNowExeAlarmRecordNo)++;										//准备处理下一条
		
		if(pstAlrRecordInfo->ucList_Head_P>=pstAlrRecordInfo->ucList_Proc_Tail_P)
		{//指针无回绕
			if(((*pucNowExeAlarmRecordNo)>=pstAlrRecordInfo->ucList_Head_P)||
				((*pucNowExeAlarmRecordNo)< pstAlrRecordInfo->ucList_Proc_Tail_P ))
			{
				(*pucNowExeAlarmRecordNo)= pstAlrRecordInfo->ucList_Proc_Tail_P;
			}
		}else	
		{//指针回绕
			if((*pucNowExeAlarmRecordNo)>=ucMaxAlrNum)
				(*pucNowExeAlarmRecordNo)=0;
			if(((*pucNowExeAlarmRecordNo)>= pstAlrRecordInfo->ucList_Head_P)&&
				((*pucNowExeAlarmRecordNo)< pstAlrRecordInfo->ucList_Proc_Tail_P ))
			{
				(*pucNowExeAlarmRecordNo) = pstAlrRecordInfo->ucList_Proc_Tail_P;
			}
		}
	}
	return ucRet;
}
unsigned char Alarm_DeleteAlrNeedAckInfo(unsigned char ucAlrRecordNo,unsigned char ucAlrType)
{
	unsigned char	ucRet=RET_SUCCESS;
	if(ucAlrType==CON_ALR_ACD) 
	{
		DeleteAlrNeedAckInfo(&stAlrNeedAckInfoGroup.stAlrNeedAckInfo_Imp,ucAlrRecordNo);	
	}
	else if(ucAlrType==CON_ALR_ACD_NOT)
	{
		DeleteAlrNeedAckInfo(&stAlrNeedAckInfoGroup.stAlrNeedAckInfo_Nor,ucAlrRecordNo);
	}
	return ucRet;
}
unsigned char DeleteAlrNeedAckInfo(STALRNEEDACKINFOLIST  *pstAlrNeedAckInfo,unsigned char ucAlrRecordNo)
{
	unsigned char	ucRet=RET_SUCCESS;
	unsigned char	i=0;//,j=0;
	unsigned char	ucHead=pstAlrNeedAckInfo->ucList_Head_P;
//	STALRNEEDACKINFOLIST stAlrNeedAckInfo;	
//	ClearAlrNeedAckInfoList(&stAlrNeedAckInfo);
	
	if(ucHead > CONALRRECORDNUM_NEEDACK)
	{
	    ClearAlrNeedAckInfoList(pstAlrNeedAckInfo);
	}
	for(i=0;i<ucHead;i++)
	{
		if(pstAlrNeedAckInfo->stAlrNeedAckInfoList[i].ucAlrRecordNo==ucAlrRecordNo)
		{
			break;
		}	
	}

    if((i < ucHead) && (ucHead >=1 ))
	{
		ucHead--;
		for(; i<ucHead; i++)
		{
			pstAlrNeedAckInfo->stAlrNeedAckInfoList[i]=pstAlrNeedAckInfo->stAlrNeedAckInfoList[i+1];
		}
		pstAlrNeedAckInfo->ucList_Head_P=ucHead;		
	}

	return ucRet;
}

unsigned char Fill_AlrNeedAckInfo(STALRNEEDACKINFOLIST  *pstAlrNeedAckInfo,unsigned char ucSEQ,unsigned char ucAlrRecordNo)
{
	unsigned char	ucRet=RET_SUCCESS;
	unsigned char	ucHead=pstAlrNeedAckInfo->ucList_Head_P;
	if(ucHead>=CONALRRECORDNUM_NEEDACK)
	{
		ucRet=RET_MEM_OVER;
	}else
	{
		pstAlrNeedAckInfo->stAlrNeedAckInfoList[ucHead].ucAlrRecordNo=ucAlrRecordNo;
		pstAlrNeedAckInfo->stAlrNeedAckInfoList[ucHead].ucAlrSendFrameSEQ=(0x0F & ucSEQ);
		pstAlrNeedAckInfo->ucList_Head_P++;
	}
	return ucRet;
}

unsigned char Alarm_Init_AlrNeedAckInfoGroup(void)
{
	unsigned char	ucRet=RET_SUCCESS;
	
	ClearAlrNeedAckInfoList(&(stAlrNeedAckInfoGroup.stAlrNeedAckInfo_Imp));
	ClearAlrNeedAckInfoList(&(stAlrNeedAckInfoGroup.stAlrNeedAckInfo_Nor));

	return ucRet; 
}
unsigned char ClearAlrNeedAckInfoList(STALRNEEDACKINFOLIST *pstAlrNeedAckInfoList)
{
	unsigned char	ucRet=RET_SUCCESS;
	unsigned char	i=0;
	pstAlrNeedAckInfoList->ucList_Head_P=0;
	pstAlrNeedAckInfoList->ucList_Tail_P=0;
	
	for(i=0;i<CONALRRECORDNUM_NEEDACK;i++)
		memset(&(pstAlrNeedAckInfoList->stAlrNeedAckInfoList[i]),0,sizeof(STALRNEEDACKINFO));
	
	return ucRet; 
}
//接收主站的确认信息
unsigned char Alarm_DownRecv_Manage(unsigned char ucSEQ,unsigned char ucAlrType)
{
	unsigned char	ucRet=RET_SUCCESS;
	unsigned char   i=0;//,j=0;
	unsigned char   ucHead=0;
	unsigned char	ucAlrRecordNo=0;
	STALRNEEDACKINFOLIST  *pstAlrNeedAckInfo=NULL;
//	STALRNEEDACKINFOLIST stAlrNeedAckInfo;	
//	ClearAlrNeedAckInfoList(&stAlrNeedAckInfo);

	if(ucAlrType==CON_ALR_ACD)
		pstAlrNeedAckInfo = &stAlrNeedAckInfoGroup.stAlrNeedAckInfo_Imp;
	else if(ucAlrType==CON_ALR_ACD_NOT)
		pstAlrNeedAckInfo = &stAlrNeedAckInfoGroup.stAlrNeedAckInfo_Nor;

	ucHead=pstAlrNeedAckInfo->ucList_Head_P;
    if(ucHead > CONALRRECORDNUM_NEEDACK)
    {
        ClearAlrNeedAckInfoList(pstAlrNeedAckInfo);
    }
	for(i=0;i<ucHead;i++)
	{
		if(pstAlrNeedAckInfo->stAlrNeedAckInfoList[i].ucAlrSendFrameSEQ==ucSEQ)
		{
			ucAlrRecordNo=pstAlrNeedAckInfo->stAlrNeedAckInfoList[i].ucAlrRecordNo;
			Alarm_SetAlrRecordFlag(ucAlrRecordNo, ucAlrType);			//置告警记录队列中的相应告警记录的标志
			break;
		}
	}
	if((i < ucHead) && (ucHead >= 1))
	{
		ucHead--;
		for(; i<ucHead; i++)
		{
			pstAlrNeedAckInfo->stAlrNeedAckInfoList[i]=pstAlrNeedAckInfo->stAlrNeedAckInfoList[i+1];
		}
		pstAlrNeedAckInfo->ucList_Head_P=ucHead;		
	}

	return ucRet; 
}

unsigned char Alarm_SetAlrRecordFlag(unsigned char ucAlrRecordNo,unsigned char ucAlrType)
{
	unsigned char	ucRet=RET_SUCCESS;
    STPERALRAUXINFO stAlarmAuxInfo;

//	ClearAlrRecord(&stAlrRecordTemp);					//初始化 stAlrRecordTemp
    ClearAlrRecordAuxInfo(&stAlarmAuxInfo);
//	ucRet=Alarm_QueryRecord(ucAlrRecordNo,&stAlrRecordTemp,ucAlrType);
    Alarm_QueryRecordAuxInfo(ucAlrRecordNo, &stAlarmAuxInfo, ucAlrType);//azh
	if(ucRet==RET_SUCCESS)
	{
//		stAlrRecordTemp.ucUpFlag=CONALR_UPFLAG_OK;		
//		Alarm_SaveRecord(ucAlrRecordNo,&stAlrRecordTemp,ucAlrType);
        stAlarmAuxInfo.stAlrAuxInfo.ucUpFlag=CONALR_UPFLAG_OK;
        Alarm_SaveRecordAuxInfo(ucAlrRecordNo,&stAlarmAuxInfo,ucAlrType);
	}
	return ucRet; 
}

/********************************************************************************************************/
unsigned char ClearAlrRecordAuxInfo(STPERALRAUXINFO *pstAlarmAuxInfo)
{
	unsigned char	ucRet=RET_SUCCESS;
	memset(pstAlarmAuxInfo,0x00,sizeof(STPERALRAUXINFO));
	pstAlarmAuxInfo->stAlrAuxInfo.ucUpFlag=CONALR_UPFLAG_NONE;
	return ucRet;
}
unsigned char ClearAlrRecord(STALRRECORD *pstAlrRecord)
{
	unsigned char	ucRet=RET_SUCCESS;
	memset(pstAlrRecord,0x00,sizeof(STALRRECORD));
	pstAlrRecord->ucUpFlag=CONALR_UPFLAG_NONE;
	return ucRet;
}

//存储告警记录到指定地点，点位置不检查
//azh 只在记录刚产生时保存到的FLASH中 后续维护上报及要不要上报 通过EEPROM中的辅助信息来处理
unsigned char Alarm_SaveRecord(unsigned char ucAlrRecordNo,STALRRECORD *pstAlrRecord,unsigned char ucAlrType)
{
	unsigned char	ucRet=RET_ERROR,num_perblock;
 	unsigned short	usBeginBlock=0,usBeginPos=0;
 	unsigned long	ulDataAddr=0;
 	unsigned char	ucMaxAlrNum=0;
 	
 	ucMaxAlrNum = Alarm_GetRecordMaxNum(ucAlrType);
 	if(ucAlrType==CON_ALR_ACD)
 	{
// 		ucMaxAlrNum=CONALRRECORDNUM_IMPORTANT;
 		usBeginBlock = CON_IMPALARM_START_BLOCK;
 	}else
 	{
// 		ucMaxAlrNum=CONALRRECORDNUM_NORMAL;
 		usBeginBlock=CON_NORMALARM_START_BLOCK;
 	}
 	
 	if(ucAlrRecordNo<ucMaxAlrNum && pstAlrRecord!=NULL)
 	{

 		num_perblock = MCP_FLASH_BLOCK_SIZE/sizeof(STALRRECORD);//azh 在flash中保存的地址是不连续的 每个block都是整数条记录 因为会块擦除
 		usBeginPos = ucAlrRecordNo%num_perblock;
 		ulDataAddr = ((ucAlrRecordNo/num_perblock)+usBeginBlock)*MCP_FLASH_BLOCK_SIZE + usBeginPos*sizeof(STALRRECORD);
        pstAlrRecord->ucavailble = CONALRRECORD_VALID;//azh 做写入标志
    	ucRet = MCP_Flash_Write(ulDataAddr,(unsigned char *)pstAlrRecord,sizeof(STALRRECORD));	
     	if(ucRet==NO_ERROR)
 			ucRet=RET_SUCCESS;
 	}
	
	return ucRet;
}
//azh 保存在eeprom中的告警上报辅助信息(上报情况 重发情况)
unsigned char Alarm_SaveRecordAuxInfo(unsigned char ucAlrRecordNo,STPERALRAUXINFO *pstAlarmAuxInfo,unsigned char ucAlrType)
{
	unsigned char	ucRet=RET_ERROR;
 	unsigned long	ulDataAddr=0;
 	unsigned char	ucMaxAlrNum=0;
 	
 	ucMaxAlrNum = Alarm_GetRecordMaxNum(ucAlrType);
 	if(ucAlrType==CON_ALR_ACD)
 	{
 		ulDataAddr = CON_IMPALARM_AUXINFO_HEAD_ADDR;//EEprom空间有限有限1024bytes 最多不超过250条
 	}else
 	{
 		ulDataAddr = CON_NORMALARM_AUXINFO_HEAD_ADDR;
 	}
 	
 	if(ucAlrRecordNo<ucMaxAlrNum && pstAlarmAuxInfo!=NULL)
 	{
        pstAlarmAuxInfo->ucCrc = addverify((unsigned char*)&pstAlarmAuxInfo->stAlrAuxInfo, sizeof(STALRAUXINFO));
     	ulDataAddr += ucAlrRecordNo*sizeof(STPERALRAUXINFO);
     	ucRet = file_write(ulDataAddr, (unsigned char*)pstAlarmAuxInfo, sizeof(STPERALRAUXINFO));
     	if(ucRet==NO_ERROR)
 			ucRet=RET_SUCCESS;
 	}
	
	return ucRet;
}
//查询告警记录
unsigned char Alarm_QueryRecord(unsigned char ucAlrRecordNo,STALRRECORD *pstAlrRecord,unsigned char ucAlrType)
{
	unsigned char	ucRet=RET_ERROR;
 	unsigned short	usBeginBlock=0,usBeginPos=0;
 	unsigned long	ulDataAddr=0;
 	unsigned char	ucHead=0,ucTail=0;
 	unsigned char	ucMaxAlrNum=0,num_perblock=0;
 	unsigned char	ucFlag = 0x55;
 	STALRRECORDINDEXINFO *pstAlrRecordInfo=NULL;
 	
    ucMaxAlrNum = Alarm_GetRecordMaxNum(ucAlrType);
 	if(ucAlrType==CON_ALR_ACD)
 	{
 		pstAlrRecordInfo=&stAlrRecordInfo.stAlrRecordInfo_Imp;
// 		ucMaxAlrNum=CONALRRECORDNUM_IMPORTANT;
 		usBeginBlock = CON_IMPALARM_START_BLOCK;
 	}else
 	{
 		pstAlrRecordInfo=&stAlrRecordInfo.stAlrRecordInfo_Norm;
// 		ucMaxAlrNum=CONALRRECORDNUM_NORMAL;
 		usBeginBlock=CON_NORMALARM_START_BLOCK;
 	}
 	if(ucAlrRecordNo<ucMaxAlrNum && pstAlrRecord!=NULL)
 	{
 		ucHead=pstAlrRecordInfo->ucList_Head_P;
 		ucTail=pstAlrRecordInfo->ucList_Tail_P;
 		if(ucHead!=ucTail)
 		{
 			if(ucHead>ucTail)
 			{
 				if((ucAlrRecordNo>=ucTail) && (ucAlrRecordNo<ucHead))		//head位置为空
 					ucFlag=0xAA;
 			}else
 			{
 				if((ucAlrRecordNo<ucHead) || (ucAlrRecordNo>=ucTail))
 					ucFlag=0xAA;
 			}
 			if(ucFlag==0xAA)
 			{	
         		num_perblock = MCP_FLASH_BLOCK_SIZE/sizeof(STALRRECORD);//azh 在flash中保存的地址是不连续的 每个block都是整数条记录 因为会块擦除
         		usBeginPos = ucAlrRecordNo%num_perblock;
         		ulDataAddr = ((ucAlrRecordNo/num_perblock)+usBeginBlock)*MCP_FLASH_BLOCK_SIZE + usBeginPos*sizeof(STALRRECORD);
 				ucRet=MCP_Flash_Read(ulDataAddr,(unsigned char *)pstAlrRecord,sizeof(STALRRECORD));
 				if((ucRet==NO_ERROR) && (pstAlrRecord->ucavailble == CONALRRECORD_VALID))
 					ucRet=RET_SUCCESS;
 			}
 		}
 	}
	
	return ucRet;
}
//查询告警其他有关上报的辅助信息
unsigned char Alarm_QueryRecordAuxInfo(unsigned char ucAlrRecordNo,STPERALRAUXINFO *pstAlarmAuxInfo,unsigned char ucAlrType)
{
	unsigned char	ucRet=RET_ERROR;
// 	unsigned short	usBeginPage=0,usBeginPos=0;
 	unsigned long	ulDataAddr=0;
 	unsigned char	ucHead=0,ucTail=0;
 	unsigned char	ucMaxAlrNum=0;
 	unsigned char	ucFlag = 0x55;
 	STALRRECORDINDEXINFO *pstAlrRecordInfo=NULL;
 	
    ucMaxAlrNum = Alarm_GetRecordMaxNum(ucAlrType);
 	if(ucAlrType==CON_ALR_ACD)
 	{
 		pstAlrRecordInfo=&stAlrRecordInfo.stAlrRecordInfo_Imp;
// 		ucMaxAlrNum=CONALRRECORDNUM_IMPORTANT;
 		ulDataAddr=CON_IMPALARM_AUXINFO_HEAD_ADDR;
 	}else
 	{
 		pstAlrRecordInfo=&stAlrRecordInfo.stAlrRecordInfo_Norm;
// 		ucMaxAlrNum=CONALRRECORDNUM_NORMAL;
 		ulDataAddr=CON_NORMALARM_AUXINFO_HEAD_ADDR;
 	}
 	if(ucAlrRecordNo<ucMaxAlrNum && pstAlarmAuxInfo!=NULL)
 	{
 		ucHead=pstAlrRecordInfo->ucList_Head_P;
 		ucTail=pstAlrRecordInfo->ucList_Tail_P;
 		if(ucHead!=ucTail)
 		{
 			if(ucHead>ucTail)
 			{
 				if((ucAlrRecordNo>=ucTail) && (ucAlrRecordNo<ucHead))		//head位置为空
 					ucFlag=0xAA;
 			}else
 			{
 				if((ucAlrRecordNo<ucHead) || (ucAlrRecordNo>=ucTail))
 					ucFlag=0xAA;
 			}
 			if(ucFlag==0xAA)
 			{	
 				memset(pstAlarmAuxInfo,0,sizeof(STPERALRAUXINFO));
 				ulDataAddr += ucAlrRecordNo*sizeof(STPERALRAUXINFO);
// 				usBeginPage	=usBeginPage+(unsigned short)(ulDataAddr/FPAGE_SIZE);
// 				usBeginPos	=(unsigned short)(ulDataAddr % FPAGE_SIZE);
// 				ucRet=Flash_GetMainMemory(usBeginPage,usBeginPos,sizeof(STALRRECORD),(unsigned char *)pstAlrRecord);
                ucRet = file_read(ulDataAddr, (unsigned char*)pstAlarmAuxInfo, sizeof(STPERALRAUXINFO));
 				if(ucRet==NO_ERROR)
 				{
 					if(pstAlarmAuxInfo->ucCrc == addverify((unsigned char*)&pstAlarmAuxInfo->stAlrAuxInfo, sizeof(STALRAUXINFO)))
 					{
 					    ucRet=RET_SUCCESS;
 					}
 				}
 			}
 		}
 	}
	
	return ucRet;
}
//初始化告警记录列表
//unsigned char Alarm_InitAlrRecordInfo(void)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	stAlrRecordInfo.ucAlrValidFlag[0]=0x55;
//	stAlrRecordInfo.ucAlrValidFlag[1]=0xAA;
//	stAlrRecordInfo.ucAlrValidFlag[2]=0x55;
//	stAlrRecordInfo.ucAlrValidFlag[3]=0xAA;
//	stAlrRecordInfo.ucAlrValidFlag[4]=0x55;
//	stAlrRecordInfo.ucAlrValidFlag[5]=0xAA;
//	stAlrRecordInfo.ucAlrValidFlag[6]=0x55;
//	stAlrRecordInfo.ucAlrValidFlag[7]=0xAA;
//	stAlrRecordInfo.stAlrRecordInfo_Imp.ucList_Head_P		=0;
//	stAlrRecordInfo.stAlrRecordInfo_Imp.ucList_Tail_P		=0;
//	stAlrRecordInfo.stAlrRecordInfo_Imp.ucList_Proc_Tail_P	=0;
//	stAlrRecordInfo.stAlrRecordInfo_Norm.ucList_Head_P		=0;
//	stAlrRecordInfo.stAlrRecordInfo_Norm.ucList_Tail_P		=0;
//	stAlrRecordInfo.stAlrRecordInfo_Norm.ucList_Proc_Tail_P	=0;
//	return ucRet;
//}

//获得国网告警编码
unsigned char Alarm_GetAlrERCNo(unsigned short usAlrCode)
{
//	unsigned char ucRet=0xFF;
	unsigned char ucRet=0;
	switch(usAlrCode)
	{
	case CONALRCODE_ALLDATAINIT	:	//ERC1：数据初始化
	case CONALRCODE_SOFTWARECHANGE:	//ERC1：软件变更
		ucRet=1;
		break;
	case CONALRCODE_PARA_LOST	:	//ERC2：参数丢失
		ucRet=2;
		break;
	case CONALRCODE_PARA_MODIFY	:	//ERC3：重要参数变更
		ucRet=3;
		break;
//	case CONALRCODE_DOOROPEN	:			//门开报警
//	case CONALRCODE_DOORCLOSE	:			//门关报警
//	case CONALRCODE_SWITCH0OPEN	:			//开关0开报警
//	case CONALRCODE_SWITCH1OPEN	:			//开关1开报警 
//	case CONALRCODE_SWITCH2OPEN	:			//开关2开报警
//	case CONALRCODE_SWITCH3OPEN	:			//开关3开报警
//	case CONALRCODE_SWITCH4OPEN	:			//开关4开报警
//	case CONALRCODE_SWITCH5OPEN	:			//开关5开报警
//	case CONALRCODE_SWITCH6OPEN	:			//开关6开报警
//	case CONALRCODE_SWITCH0CLOSE:			//开关0关报警
//	case CONALRCODE_SWITCH1CLOSE:			//开关1关报警 
//	case CONALRCODE_SWITCH2CLOSE:			//开关2关报警  
//	case CONALRCODE_SWITCH3CLOSE:			//开关3关报警 
//	case CONALRCODE_SWITCH4CLOSE:			//开关4关报警 
//	case CONALRCODE_SWITCH5CLOSE:			//开关5关报警 
//	case CONALRCODE_SWITCH6CLOSE:			//开关6关报警 
//		ucRet=4;
//		break;
//	case CONALRCODE_REMOTECTRL:				//遥控跳闸记录
//		ucRet=5;
//		break;		
//	case CONALRCODE_POWERCTRL:				//功控跳闸记录
//		ucRet=6;
//		break;		
//	case CONALRCODE_ENERCTRL:				//电控跳闸记录
//		ucRet=7;
//		break;		
	case CONALRCODE_FEERATE			:		//费率更改
	case CONALRCODE_PROGRAMTIME		:		//编程时间更改
	case CONALRCODE_COPYDAYTIME		:		//抄表日
	case CONALRCODE_PULSECONST		:		//脉冲常数
	case CONALRCODE_DEMANDTIMECLR	:		//需量清零时间更改
		ucRet=8;
		break;		
	case CONALRCODE_POLREVERSEA	:			//反极性
	case CONALRCODE_POLREVERSEB	:	
	case CONALRCODE_POLREVERSEC	:
//	case CONALRCODE_SHORTCIRA1	:			//CT一次侧短路
//	case CONALRCODE_SHORTCIRB1	:
//	case CONALRCODE_SHORTCIRC1	:
//	case CONALRCODE_SHORTCIRA2	:			//CT二次侧短路
//	case CONALRCODE_SHORTCIRB2	:		
//	case CONALRCODE_SHORTCIRC2	:
//	case CONALRCODE_OPENCIRA2	:			//CT二次侧开路
//	case CONALRCODE_OPENCIRB2	:
//	case CONALRCODE_OPENCIRC2	:
		ucRet=9;
		break;	
	case CONALRCODE_VABREAK			:		//A相电压断相
	case CONALRCODE_VBBREAK			:		//B相电压断相
	case CONALRCODE_VCBREAK			:		//C相电压断相
	case CONALRCODE_VALACK			:		//A相电压缺相
	case CONALRCODE_VBLACK			:		//B相电压缺相
	case CONALRCODE_VCLACK			:		//C相电压缺相
		ucRet=10;
		break;
	case CONALRCODE_VOLCIRREVERSE :			//电压回路逆相序
		ucRet=11;
		break;
	case CONALRCODE_METERTIMEERR	:		//表计时钟异常
		ucRet=12;
		break;	
	case CONALRCODE_LOSTVOLNUM		:		//断相次数
	case CONALRCODE_METERBATTLOW	:		//电池电压低
	case CONALRCODE_DEMANDTIMENUM	:		//需量次数变化
	case CONALRCODE_PROGRAMNUM		:		//编程次数变化
		ucRet=13;
		break;		
	case CONALRCODE_POWERON:				//停电
	case CONALRCODE_POWEROFF:				//上电
		ucRet=14;
		break;
	case CONALRCODE_IUNBLANCE		:		//电流不平衡越限
	case CONALRCODE_UUNBLANCE		:		//电压不平衡越限
		ucRet=17;
		break;	
//	case	CONALRCODE_BUYPOWPARA:			//购电参数设置
//		ucRet=19;
//		break;	
	case	CONALRCODE_COMM_PASSWORDERR:	//密码错误
		ucRet=20;
		break;	
//	case CONALRCODE_SUMCOMPOVERLIMIT:
//	case CONALRCODE_SUMCOMPOVERLIMIT+1:
//	case CONALRCODE_SUMCOMPOVERLIMIT+2:
//	case CONALRCODE_SUMCOMPOVERLIMIT+3:
//	case CONALRCODE_SUMCOMPOVERLIMIT+4:
//	case CONALRCODE_SUMCOMPOVERLIMIT+5:
//	case CONALRCODE_SUMCOMPOVERLIMIT+6:
//	case CONALRCODE_SUMCOMPOVERLIMIT+7:
//		ucRet=22;
//		break;	
//	case CONALRCODE_SUMINSTANTPOWUP1	:		//瞬时有功总加组1越上限
//	case CONALRCODE_SUMINSTANTPOWUP2	:		//瞬时有功总加组2越上限
//	case CONALRCODE_SUMINSTANTPOWUP3	:		//瞬时有功总加组3越上限
//	case CONALRCODE_SUMINSTANTPOWUP4	:		//瞬时有功总加组4越上限
//	case CONALRCODE_SUMINSTANTPOWUP5	:		//瞬时有功总加组5越上限
//	case CONALRCODE_SUMINSTANTPOWUP6	:		//瞬时有功总加组6越上限
//	case CONALRCODE_SUMINSTANTPOWUP7	:		//瞬时有功总加组7越上限
//	case CONALRCODE_SUMINSTANTPOWUP8	:		//瞬时有功总加组8越上限
//	case CONALRCODE_SUMINSTANTPOWDOWN1	:		//瞬时有功总加组1越下限
//	case CONALRCODE_SUMINSTANTPOWDOWN2	:		//瞬时有功总加组2越下限
//	case CONALRCODE_SUMINSTANTPOWDOWN3	:		//瞬时有功总加组3越下限
//	case CONALRCODE_SUMINSTANTPOWDOWN4	:		//瞬时有功总加组4越下限
//	case CONALRCODE_SUMINSTANTPOWDOWN5	:		//瞬时有功总加组5越下限
//	case CONALRCODE_SUMINSTANTPOWDOWN6	:		//瞬时有功总加组6越下限
//	case CONALRCODE_SUMINSTANTPOWDOWN7	:		//瞬时有功总加组7越下限
//	case CONALRCODE_SUMINSTANTPOWDOWN8	:		//瞬时有功总加组8越下限
//		ucRet=22;		
//		break;	
//	case CONALRCODE_SUMINSTANTVARUP1	:		//瞬时无功总加组1越上限
//	case CONALRCODE_SUMINSTANTVARUP2	:		//瞬时无功总加组2越上限
//	case CONALRCODE_SUMINSTANTVARUP3	:		//瞬时无功总加组3越上限
//	case CONALRCODE_SUMINSTANTVARUP4	:		//瞬时无功总加组4越上限
//	case CONALRCODE_SUMINSTANTVARUP5	:		//瞬时无功总加组5越上限
//	case CONALRCODE_SUMINSTANTVARUP6	:		//瞬时无功总加组6越上限
//	case CONALRCODE_SUMINSTANTVARUP7	:		//瞬时无功总加组7越上限
//	case CONALRCODE_SUMINSTANTVARUP8	:		//瞬时无功总加组8越上限
//	case CONALRCODE_SUMINSTANTVARDOWN1	:		//瞬时无功总加组1越下限
//	case CONALRCODE_SUMINSTANTVARDOWN2	:		//瞬时无功总加组2越下限
//	case CONALRCODE_SUMINSTANTVARDOWN3	:		//瞬时无功总加组3越下限
//	case CONALRCODE_SUMINSTANTVARDOWN4	:		//瞬时无功总加组4越下限
//	case CONALRCODE_SUMINSTANTVARDOWN5	:		//瞬时无功总加组5越下限
//	case CONALRCODE_SUMINSTANTVARDOWN6	:		//瞬时无功总加组6越下限
//	case CONALRCODE_SUMINSTANTVARDOWN7	:		//瞬时无功总加组7越下限
//	case CONALRCODE_SUMINSTANTVARDOWN8	:		//瞬时无功总加组8越下限
//		ucRet=22;
//		break;
//	case CONALRCODE_SUMPOWUP1	:		//有功总加组1越上限
//	case CONALRCODE_SUMPOWUP2	:		//有功总加组2越上限
//	case CONALRCODE_SUMPOWUP3	:		//有功总加组3越上限
//	case CONALRCODE_SUMPOWUP4	:		//有功总加组4越上限
//	case CONALRCODE_SUMPOWUP5	:		//有功总加组5越上限
//	case CONALRCODE_SUMPOWUP6	:		//有功总加组6越上限
//	case CONALRCODE_SUMPOWUP7	:		//有功总加组7越上限
//	case CONALRCODE_SUMPOWUP8	:		//有功总加组8越上限
//	case CONALRCODE_SUMPOWDOWN1	:		//有功总加组1越下限
//	case CONALRCODE_SUMPOWDOWN2	:		//有功总加组2越下限
//	case CONALRCODE_SUMPOWDOWN3	:		//有功总加组3越下限
//	case CONALRCODE_SUMPOWDOWN4	:		//有功总加组4越下限
//	case CONALRCODE_SUMPOWDOWN5	:		//有功总加组5越下限
//	case CONALRCODE_SUMPOWDOWN6	:		//有功总加组6越下限
//	case CONALRCODE_SUMPOWDOWN7	:		//有功总加组7越下限
//	case CONALRCODE_SUMPOWDOWN8	:		//有功总加组8越下限
//		ucRet=53;		//23		
//		break;
//	case CONALRCODE_SUMVARUP1	:		//无功总加组1越上限
//	case CONALRCODE_SUMVARUP2	:		//无功总加组2越上限
//	case CONALRCODE_SUMVARUP3	:		//无功总加组3越上限
//	case CONALRCODE_SUMVARUP4	:		//无功总加组4越上限
//	case CONALRCODE_SUMVARUP5	:		//无功总加组5越上限
//	case CONALRCODE_SUMVARUP6	:		//无功总加组6越上限
//	case CONALRCODE_SUMVARUP7	:		//无功总加组7越上限
//	case CONALRCODE_SUMVARUP8	:		//无功总加组8越上限
//	case CONALRCODE_SUMVARDOWN1	:		//无功总加组1越下限
//	case CONALRCODE_SUMVARDOWN2	:		//无功总加组2越下限
//	case CONALRCODE_SUMVARDOWN3	:		//无功总加组3越下限
//	case CONALRCODE_SUMVARDOWN4	:		//无功总加组4越下限
//	case CONALRCODE_SUMVARDOWN5	:		//无功总加组5越下限
//	case CONALRCODE_SUMVARDOWN6	:		//无功总加组6越下限
//	case CONALRCODE_SUMVARDOWN7	:		//无功总加组7越下限
//	case CONALRCODE_SUMVARDOWN8	:		//无功总加组8越下限
//		ucRet=53;		//23
//		break;
	case CONALRCODE_VAOVERUP	:		//电压越限_Va
	case CONALRCODE_VBOVERUP	:		//电压越限_Vb
	case CONALRCODE_VCOVERUP	:		//电压越限_Vc
	case CONALRCODE_VAOVERDOWN	:		//电压越限_Va(下限)
	case CONALRCODE_VBOVERDOWN	:		//电压越限_Vb(下限)
	case CONALRCODE_VCOVERDOWN	:		//电压越限_Vc(下限)
		ucRet=24;
		break;
	case CONALRCODE_IAOVERUP	:		//电流越限_Ia
	case CONALRCODE_IBOVERUP	:		//电流越限_Ib
	case CONALRCODE_ICOVERUP	:		//电流越限_Ic
	case CONALRCODE_IAERROVERUP	:		//电流越上上限_Ia
	case CONALRCODE_IBERROVERUP	:		//电流越上上限_Ib
	case CONALRCODE_ICERROVERUP	:		//电流越上上限_Ic
		ucRet=25;
		break;
	case CONALRCODE_SPOWNORUP	:		//超报装容量上限（kVA），视在功率越限
	case CONALRCODE_SPOWERRUP	:		//超报装容量上上限（kVA），视在功率越限
		ucRet=26;
		break;		
	case CONALRCODE_TERERR_RAM:				//终端主板内存故障
	case CONALRCODE_TERERR_CLOCK:			//时钟故障
	case CONALRCODE_TERERR_MAINCOMM:		//主板通信故障
	case CONALRCODE_TERERR_485ERROR:		//485抄表故障
	case CONALRCODE_TERERR_DISPLAY:			//显示板故障
		ucRet=21;
		break;
//	case CONALRCODE_METERDOWN		:		//表计倒走
//		ucRet=27;
//		break;	
//	case CONALRCODE_POWDIFFOVER:			//ERC28	电能量超差记录
//		ucRet = 28;
//		break;
//	case CONALRCODE_METERFLY		:		//表计飞走
//		ucRet=29;
//		break;		
//	case CONALRCODE_METERSTOP		:		//表计停走
//		ucRet=30;
//		break;	

//	case	CONALRCODE_ENERALR:				//ERC23 电控告警事件
//		ucRet=23;
//		break;	
	case	CONALRCODE_METERCOPY:			//ERC31 485抄表失败事件记录
		ucRet=31;
		break;	
	case	CONALRCODE_FLUXLIMIT:			//ERC32 月通信超流量记录
		ucRet=32;
		break;	
	case	CONALRCODE_METERSTATECHANGE:	//ERC 33
		ucRet=33;
		break;	
	default:
		break;
	}
	return ucRet;
}

unsigned char Main_Comm_GetDT1_Alr(unsigned char ucAlrType)
{
	unsigned char ucRet=2;
	if( ucAlrType == CON_ALR_ACD)
		ucRet = 1;
	return ucRet;
}

unsigned char Main_SendComm_Alr(unsigned char ucAlrType,
								unsigned char ucNowExeAlrRecordNo,
								unsigned char ucNowAlrNo_Imp,
								unsigned char ucNowAlrNo_Nor,
								STALRRECORD *pstAlrRecord,
								unsigned char *pucSEQ)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char	ucRet = RET_SUCCESS;
	unsigned char	i=0;
	unsigned char	*pucSrcBuffer=NULL,*pucRetBuffer=NULL;
	unsigned short	m=0,usSendLen=0;
	unsigned char ucLink=10;				//需要主站确认
	unsigned char ucCommType=CON_TERMSG_SOUR_RCOMM_TCP;
	STTERCOMMPARA stTerCommPara;
	STMAINCOMMINFO	*pstMainCommInfo=NULL;
	unsigned char ucConFlag=CON_STATUS_NO;
	unsigned char ucTmpAlrERCCode;

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucSrcBuffer=(unsigned char *)mem_ptr;							//利用临时缓冲区
	pucRetBuffer=(unsigned char *)mem_ptr+512;

	memset(pucSrcBuffer,0,512);
	memset(pucRetBuffer,0,512);

	pucSrcBuffer[m++]	= 0;					//ucDA1
	pucSrcBuffer[m++]	= 0;					//ucDA2
	pucSrcBuffer[m++]	= Main_Comm_GetDT1_Alr(ucAlrType);	//DT1
	pucSrcBuffer[m++]	= 0;					//ucDT2
	pucSrcBuffer[m++]	= ucNowAlrNo_Imp;				//当前重要事件计数器n_imp		主动上送仅仅一条
	pucSrcBuffer[m++]	= ucNowAlrNo_Nor;				//当前一般事件计数器n_norm		主动上送仅仅一条
	pucSrcBuffer[m++]	= ucNowExeAlrRecordNo;			//开始计数器n1
	pucSrcBuffer[m++]	= ucNowExeAlrRecordNo+1;		//结束计数器n2
	pucSrcBuffer[m++]	= pstAlrRecord->ucAlrERCCode;	//获得告警信息编码
	pucSrcBuffer[m++]	= 0;							//信息长度
	ucTmpAlrERCCode=Alarm_GetAlrERCNo(CONALRCODE_POWERON);
	if(pstAlrRecord->ucAlrERCCode==ucTmpAlrERCCode)
	{
		for(i=0;i<pstAlrRecord->ucAlrDataItemNum;i++)
		{
			memcpy((pucSrcBuffer+m),pstAlrRecord->stAlrDataItem[i].ucDataContent,
				pstAlrRecord->stAlrDataItem[i].usDataLen);
			m+=pstAlrRecord->stAlrDataItem[i].usDataLen;
		}
	}else
	{
		pucSrcBuffer[m++]	= (unsigned char)HEX_TO_BCD(pstAlrRecord->stDateTime.ucMinute,1);
		pucSrcBuffer[m++]	= (unsigned char)HEX_TO_BCD(pstAlrRecord->stDateTime.ucHour,1);
		pucSrcBuffer[m++]	= (unsigned char)HEX_TO_BCD(pstAlrRecord->stDateTime.ucDay,1);
		pucSrcBuffer[m++]	= (unsigned char)HEX_TO_BCD(pstAlrRecord->stDateTime.ucMonth,1);
		pucSrcBuffer[m++]	= (unsigned char)HEX_TO_BCD(pstAlrRecord->stDateTime.ucYear,1);

		for(i=0;i<pstAlrRecord->ucAlrDataItemNum;i++)
		{
			if(pstAlrRecord->stAlrDataItem[i].usDataLen>CON_DI_DATA_LEN_ALR)
				pstAlrRecord->stAlrDataItem[i].usDataLen=CON_DI_DATA_LEN_ALR;
			memcpy((pucSrcBuffer+m),pstAlrRecord->stAlrDataItem[i].ucDataContent,
				pstAlrRecord->stAlrDataItem[i].usDataLen);
			m+=pstAlrRecord->stAlrDataItem[i].usDataLen;
		}
	}

	pucSrcBuffer[9]=(unsigned char)(m-10);									//信息长度
	usSendLen=m;

	stTerCommPara=RunPara_GetTerCommPara();
	if(stTerCommPara.ucServerNeedAck[0]==CON_STATUS_YES && ucAlrType==CON_ALR_ACD)
		ucConFlag=CON_STATUS_YES;
	if(stTerCommPara.ucServerNeedAck[1]==CON_STATUS_YES && ucAlrType!=CON_ALR_ACD)
		ucConFlag=CON_STATUS_YES;
	Main_SendExplain(CON_TERMSG_REQDATA_3,pucSrcBuffer,pucRetBuffer,&usSendLen,pstMainCommInfo,CON_STATUS_YES,ucConFlag,0x60);		//告警主动上送
	ucCommType=Main_Get_Comm_Type();
	if(ucCommType!=CON_TERMSG_SOUR_RCOMM_SMS)
//		Main_FillUpData(pucRetBuffer,usSendLen,ucCommType,ucLink);
		Main_Fill_TmpUpSendTask(pucRetBuffer,usSendLen,ucLink);
	(*pucSEQ)=pucRetBuffer[13];
	Main_FillUpData(pucRetBuffer,usSendLen,CON_TERMSG_SOUR_RS232,ucLink);
	
	free_mem_block(mem_ptr);//
	
	return ucRet;
}

//STALRCATEGORY Alarm_Get_Happen_word(void)
//{
//	return (stAlrHappenWord);
//}

unsigned char	Alarm_Set_Happen_word(STALRCATEGORY *pstAlrHappenWord)
{
	unsigned char	ucRet = RET_SUCCESS;
	stAlrHappenWord=(*pstAlrHappenWord);	
	return ucRet;
}

unsigned char	Alarm_Set_AlrCate_word(STALRCATEGORY *pstAlrHappenWord,unsigned char ucAlrERCCode,unsigned char ucSetVal)
{
	unsigned char	ucRet = RET_SUCCESS;
	unsigned char	ucFlag = CON_ALR_HAPPEN;
	if(ucSetVal!=CON_ALR_HAPPEN)
		ucFlag=CON_ALR_HAPPEN_NOT;
	switch(ucAlrERCCode)
	{
	case 1:
		pstAlrHappenWord->ucTerDataInit = ucFlag;
		break;
	case 2:
		pstAlrHappenWord->ucTerParaLost = ucFlag;
		break;
	case 3:
		pstAlrHappenWord->ucTerParaChange = ucFlag;
		break;
	case 4:
		pstAlrHappenWord->ucStatChange = ucFlag;
		break;
	case 5:
		pstAlrHappenWord->ucRemoteCtrl = ucFlag;
		break;
	case 6:
		pstAlrHappenWord->ucPowerCtrl = ucFlag;
		break;
	case 7:
		pstAlrHappenWord->ucEnergyCtrl = ucFlag;
		break;
	case 8:
		pstAlrHappenWord->ucMeterParaChange = ucFlag;
		break;

	case 9:
		pstAlrHappenWord->ucTAErr = ucFlag;
		break;
	case 10:
		pstAlrHappenWord->ucTVErr = ucFlag;
		break;
	case 11:
		pstAlrHappenWord->ucPhaseErr = ucFlag;
		break;
	case 12:
		pstAlrHappenWord->ucMeterTimeErr = ucFlag;
		break;
	case 13:
		pstAlrHappenWord->ucMeterErrInfo = ucFlag;
		break;
	case 14:
		pstAlrHappenWord->ucTerPowOff = ucFlag;
		break;
	case 15:
		pstAlrHappenWord->ucHarmWaveOver = ucFlag;
		break;
	case 16:
		pstAlrHappenWord->ucDirSimuValOver = ucFlag;
		break;

	case 17:
		pstAlrHappenWord->ucUnbalanceOver = ucFlag;
		break;
	case 18:
		pstAlrHappenWord->ucCapLockSelf = ucFlag;
		break;
	case 19:
		pstAlrHappenWord->ucBuyPowPara = ucFlag;
		break;
	case 20:
		pstAlrHappenWord->ucPassErr = ucFlag;
		break;
	case 21:
		pstAlrHappenWord->ucTerErr = ucFlag;
		break;
	case 24:
		pstAlrHappenWord->ucVolOverUp = ucFlag;
		break;

	case 25:
		pstAlrHappenWord->ucCurOverUp = ucFlag;
		break;
	case 26:
		pstAlrHappenWord->ucSPowOverUp = ucFlag;
		break;
	case 27:
		pstAlrHappenWord->ucMeterDown = ucFlag;
		break;
	case 28:
		pstAlrHappenWord->ucPowDiffOver = ucFlag;
		break;
	case 29:
		pstAlrHappenWord->ucMeterFly = ucFlag;
		break;
	case 30:
		pstAlrHappenWord->ucMeterStop = ucFlag;
		break;

	case 23:
		pstAlrHappenWord->ucEnerAlr = ucFlag;
		break;
	case 31:
		pstAlrHappenWord->ucMeterCopy = ucFlag;
		break;
	case 32:
		pstAlrHappenWord->ucFluxLimit = ucFlag;
		break;
	}
	return ucRet;}

unsigned char	Alarm_SetAlrHappen(unsigned char ucAlrERCCode)
{
	unsigned char	ucRet = RET_SUCCESS;
	switch(ucAlrERCCode)
	{
	case 1:
		stAlrHappenWord.ucTerDataInit = CON_ALR_HAPPEN;
		break;
	case 2:
		stAlrHappenWord.ucTerParaLost = CON_ALR_HAPPEN;
		break;
	case 3:
		stAlrHappenWord.ucTerParaChange = CON_ALR_HAPPEN;
		break;
	case 4:
		stAlrHappenWord.ucStatChange = CON_ALR_HAPPEN;
		break;
	case 5:
		stAlrHappenWord.ucRemoteCtrl = CON_ALR_HAPPEN;
		break;
	case 6:
		stAlrHappenWord.ucPowerCtrl = CON_ALR_HAPPEN;
		break;
	case 7:
		stAlrHappenWord.ucEnergyCtrl = CON_ALR_HAPPEN;
		break;
	case 8:
		stAlrHappenWord.ucMeterParaChange = CON_ALR_HAPPEN;
		break;

	case 9:
		stAlrHappenWord.ucTAErr = CON_ALR_HAPPEN;
		break;
	case 10:
		stAlrHappenWord.ucTVErr = CON_ALR_HAPPEN;
		break;
	case 11:
		stAlrHappenWord.ucPhaseErr = CON_ALR_HAPPEN;
		break;
	case 12:
		stAlrHappenWord.ucMeterTimeErr = CON_ALR_HAPPEN;
		break;
	case 13:
		stAlrHappenWord.ucMeterErrInfo = CON_ALR_HAPPEN;
		break;
	case 14:
		stAlrHappenWord.ucTerPowOff = CON_ALR_HAPPEN;
		break;
	case 15:
		stAlrHappenWord.ucHarmWaveOver = CON_ALR_HAPPEN;
		break;
	case 16:
		stAlrHappenWord.ucDirSimuValOver = CON_ALR_HAPPEN;
		break;

	case 17:
		stAlrHappenWord.ucUnbalanceOver = CON_ALR_HAPPEN;
		break;
	case 18:
		stAlrHappenWord.ucCapLockSelf = CON_ALR_HAPPEN;
		break;
	case 19:
		stAlrHappenWord.ucBuyPowPara = CON_ALR_HAPPEN;
		break;
	case 20:
		stAlrHappenWord.ucPassErr = CON_ALR_HAPPEN;
		break;
	case 21:
		stAlrHappenWord.ucTerErr = CON_ALR_HAPPEN;
		break;
	case 24:
		stAlrHappenWord.ucVolOverUp = CON_ALR_HAPPEN;
		break;

	case 25:
		stAlrHappenWord.ucCurOverUp = CON_ALR_HAPPEN;
		break;
	case 26:
		stAlrHappenWord.ucSPowOverUp = CON_ALR_HAPPEN;
		break;
	case 27:
		stAlrHappenWord.ucMeterDown = CON_ALR_HAPPEN;
		break;
	case 28:
		stAlrHappenWord.ucPowDiffOver = CON_ALR_HAPPEN;
		break;
	case 29:
		stAlrHappenWord.ucMeterFly = CON_ALR_HAPPEN;
		break;
	case 30:
		stAlrHappenWord.ucMeterStop = CON_ALR_HAPPEN;
		break;

	case 23:
		stAlrHappenWord.ucEnerAlr = CON_ALR_HAPPEN;
		break;
	case 31:
		stAlrHappenWord.ucMeterCopy = CON_ALR_HAPPEN;
		break;
	case 32:
		stAlrHappenWord.ucFluxLimit = CON_ALR_HAPPEN;
		break;
	case 33:
		stAlrHappenWord.ucMeterState= CON_ALR_HAPPEN;
		break;
	}
	return ucRet;
}

unsigned short Alarm_GetFrameACD(void)
{
	return g_usFrameACD;
}

void Alarm_SetFrameACD(unsigned short usACD)
{
	g_usFrameACD=usACD;
}
/******************************************************************************
** 函数名称: unsigned char Alarm_GetRecordMaxNum(unsigned char ucAlrType)
** 函数功能: 获取告警记录的最大条数
** 入口参数: ucAlrType任务类型：一般 重要
** 返 回 值: 记录的最大条数
** 说    明: 无
******************************************************************************/
unsigned char Alarm_GetRecordMaxNum(unsigned char ucAlrType)
{
    unsigned char num=0;
    
	if(ucAlrType==CON_ALR_ACD)//重要告警
	{
        num = CON_IMPALARM_BLOCK_TOTNUM * (MCP_FLASH_BLOCK_SIZE / sizeof(STALRRECORD));//约=4k/330
	}
	else if(ucAlrType==CON_ALR_ACD_NOT)
	{
        num = CON_NORMALARM_BLOCK_TOTNUM * (MCP_FLASH_BLOCK_SIZE / sizeof(STALRRECORD));//约=4k/330
	}    
    return num;
}
/******************************************************************************
** 函数名称: void Alarm_SaveManageInfo(unsigned char ucAlrType)
** 函数功能: 保存有关告警的管理信息
** 入口参数: ucAlrType任务类型：一般 重要
** 返 回 值: 
** 说    明: 无
******************************************************************************/
void Alarm_SaveManageInfo(unsigned char ucAlrType)
{
    unsigned char len=0;
    unsigned char buf[8];
    
	if(ucAlrType==CON_ALR_ACD)//重要告警
	{
        len = sizeof(STALRRECORDINDEXINFO);
        if(len >= 8) len =7;//缓冲就开了8个字节
        memcpy(buf, (unsigned char*)&stAlrRecordInfo.stAlrRecordInfo_Imp, len);
        buf[len] = addverify(buf, len);
        file_write(CON_IMPALARM_INFO_BASE_ADDR, buf, (len+1)); 
	}
	else if(ucAlrType==CON_ALR_ACD_NOT)
	{
        len = sizeof(STALRRECORDINDEXINFO);
        if(len >= 8) len =7;//缓冲就开了8个字节
        memcpy(buf, (unsigned char*)&stAlrRecordInfo.stAlrRecordInfo_Norm, len);
        buf[len] = addverify(buf, len);
        file_write(CON_NORMALARM_INFO_BASE_ADDR, buf, (len+1)); 
	}    

}
/******************************************************************************
** 函数名称: void Alarm_InitManageInfo(unsigned char ucFlag)
** 函数功能: 初始化有关告警的管理信息(包括上电初始化和全清命令初始化)
** 入口参数: 
** 返 回 值: 
** 说    明: 无
******************************************************************************/
void Alarm_InitManageInfo(unsigned char ucFlag)
{
    unsigned char len=0;
    unsigned char buf[8];

    len = sizeof(STALRRECORDINDEXINFO);
    if(len >= 8) len =7;//缓冲就开了8个字节    
    if(ucFlag == CON_PARA_INIT_ALL_CLR)//总清命令
	{
        memset((unsigned char*)&stAlrRecordInfo.stAlrRecordInfo_Imp, 0, len);
        memcpy(buf, (unsigned char*)&stAlrRecordInfo.stAlrRecordInfo_Imp, len);
        buf[len] = addverify(buf, len);
        file_write(CON_IMPALARM_INFO_BASE_ADDR, buf, (len+1));    
        MCP_Flash_Sector_Erase(CON_IMPALARM_START_BLOCK * MCP_FLASH_BLOCK_SIZE);//清掉flash中保存告警记录的首块block   

        memset((unsigned char*)&stAlrRecordInfo.stAlrRecordInfo_Norm, 0, len);
        memcpy(buf, (unsigned char*)&stAlrRecordInfo.stAlrRecordInfo_Norm, len);
        buf[len] = addverify(buf, len);
        file_write(CON_NORMALARM_INFO_BASE_ADDR, buf, (len+1));    
        MCP_Flash_Sector_Erase(CON_NORMALARM_START_BLOCK * MCP_FLASH_BLOCK_SIZE);  
	}
	else//上电
	{
//重要告警相关信息
        file_read(CON_IMPALARM_INFO_BASE_ADDR, buf, (len+1)); 
        if(buf[len] == addverify(buf, len))
        {
           memcpy((unsigned char*)&stAlrRecordInfo.stAlrRecordInfo_Imp, buf, len); 
        }
        else
        {
            memset((unsigned char*)&stAlrRecordInfo.stAlrRecordInfo_Imp, 0, len);
            memcpy(buf, (unsigned char*)&stAlrRecordInfo.stAlrRecordInfo_Imp, len);
            buf[len] = addverify(buf, len);
            file_write(CON_IMPALARM_INFO_BASE_ADDR, buf, (len+1));    
            MCP_Flash_Sector_Erase(CON_IMPALARM_START_BLOCK * MCP_FLASH_BLOCK_SIZE);         
        }
//一般告警相关信息        
        file_read(CON_NORMALARM_INFO_BASE_ADDR, buf, (len+1)); 
        if(buf[len] == addverify(buf, len))
        {
           memcpy((unsigned char*)&stAlrRecordInfo.stAlrRecordInfo_Norm, buf, len); 
        }
        else
        {
            memset((unsigned char*)&stAlrRecordInfo.stAlrRecordInfo_Norm, 0, len);
            memcpy(buf, (unsigned char*)&stAlrRecordInfo.stAlrRecordInfo_Norm, len);
            buf[len] = addverify(buf, len);
            file_write(CON_NORMALARM_INFO_BASE_ADDR, buf, (len+1));    
            MCP_Flash_Sector_Erase(CON_NORMALARM_START_BLOCK * MCP_FLASH_BLOCK_SIZE);         
        }
	}

	if(stAlrRecordInfo.stAlrRecordInfo_Imp.ucList_Tail_P!=stAlrRecordInfo.stAlrRecordInfo_Imp.ucList_Proc_Tail_P)
		g_usFrameACD = CON_STATUS_YES_2BYTE;
	else
		g_usFrameACD = CON_STATUS_NO_2BYTE;

	g_ucEventCount_Important = stAlrRecordInfo.stAlrRecordInfo_Imp.ucList_Head_P;
	g_ucEventCount_Normal = stAlrRecordInfo.stAlrRecordInfo_Norm.ucList_Head_P;
	ucNowExeAlarmRecordNo_Important	= stAlrRecordInfo.stAlrRecordInfo_Imp.ucList_Proc_Tail_P;
	ucNowExeAlarmRecordNo_Normal = stAlrRecordInfo.stAlrRecordInfo_Norm.ucList_Proc_Tail_P;	
	
	Alarm_Init_AlrNeedAckInfoGroup();
}

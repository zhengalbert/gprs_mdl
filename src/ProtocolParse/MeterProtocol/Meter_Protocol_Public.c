/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Meter_Protocol_Public.c
 Description    : 抄表模块规约解释处理
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

//私有函数申明
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

//////////////////////////////// 函数映射 //////////////////////////////////////
//////////////★★★★★ 确保本映射函数位于程序前面，且次序不得更改 ★★★★★///////////////
//////////////////// 函数跳转入口保持固定，保持更新lib改动函数入口不变 //////////////////////
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
//////////////★★★★★ 确保本映射函数位于程序前面，且以上次序不得更改 ★★★★★///////////////


//////////////////////////////// 函数实现 //////////////////////////////////////
//初始化规约解释队列
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

//解释为发送前的报文
unsigned char _Meter_Protocol_SendExplain(STRS485TASK *pstRS485Task,unsigned char *pSendBuffer,unsigned short *pusLen)
{
	unsigned char ucRet=RET_ERROR;
	unsigned char m=0;
	unsigned char ucMeterNo=0xFF;
//	unsigned char ucCheck;

	if(pstRS485Task->ucReportType==CON_RS485_REPORT_TYPE_RELAY)
	{//中继处理
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
//			case	CON_PROTOCOL_DL645:			//645规约
//			case	CON_PROTOCOL_AS1_DL645:		//645规约类似一
//			case	CON_PROTOCOL_AS2_DL645:		//645规约类似二
//				
//			case	CON_PROTOCOL_HLi:			//浙江华立 (645)
//			case	CON_PROTOCOL_KL:			//科陆 (645)
//			case	CON_PROTOCOL_LD:			//龙电 (645)
//			case	CON_PROTOCOL_SKD:			//深科达 (645)
//			case	CON_PROTOCOL_JSLY:			//江苏羚羊 (645)
//			case	CON_PROTOCOL_HND645:			//浩宁达 (645)
//			case	CON_PROTOCOL_DFDZ:			//东方电子 (645)
//			case	CON_PROTOCOL_JQ:			//金雀 (645)
//				m=Meter_DI_Type_DownExplain_Manage(pstRS485Task,ucMeterNo,pSendBuffer,m);
//				if(m>0)		ucRet = RET_SUCCESS;
//				break;
//			case	CON_PROTOCOL_HB_PT:	//鄂规之普通电子式电能表
//				m=HBPT_DI_Type_DownExplain_Manage(pstRS485Task,ucMeterNo,pSendBuffer,m);
//				if(m>0) 	ucRet = RET_NEEDCLRGROUP;				
//				break;
//			case	CON_PROTOCOL_HB_JM:	//鄂规之居民复费率电能表
//				m=HBJM_DI_Type_DownExplain_Manage(pstRS485Task,ucMeterNo,pSendBuffer,m);
//				if(m>0) 	ucRet = RET_NEEDCLRGROUP;				
//				break;	
//			/*
//			case	CON_PROTOCOL_ABB_R:		//ABB方表
//				m=ABBR_DI_Type_DownExplain_Manage(pstRS485Task,ucMeterNo,pSendBuffer,m);
//				if(m>0) 	ucRet = RET_NEEDCLRGROUP;
//				break;
//			case	CON_PROTOCOL_LANDB:		//西门子B表
//				m=LANDB_DI_Type_DownExplain_Manage(pstRS485Task,ucMeterNo,pSendBuffer,m);
//				if(m>0) 	ucRet = RET_NEEDCLRGROUP;				
//				break;
//			case	CON_PROTOCOL_LANDD:	//西门子D表
//				m=LANDD_DI_Type_DownExplain_Manage(pstRS485Task,ucMeterNo,pSendBuffer,m);
//				if(m>0) 	ucRet = RET_NEEDCLRGROUP;				
//				break;
//			case	CON_PROTOCOL_WS	:	//威胜规约
//				m=WS_DI_Type_DownExplain_Manage(pstRS485Task,ucMeterNo,pSendBuffer,m);
//				if(m>0) 	ucRet = RET_SUCCESS;				
//				break;				
//			case	CON_PROTOCOL_GENIUS://红相
//				m=Genius_DI_Type_DownExplain_Manage(pstRS485Task,ucMeterNo,pSendBuffer,m);
//				if(m>0) 	ucRet = RET_NEEDCLRGROUP;				
//				break;		
//			case      CON_PROTOCOL_HND://浩宁达
//				m=HND_DI_Type_DownExplain_Manage(pstRS485Task,ucMeterNo,pSendBuffer,m);
//				if(m>0) 	ucRet = RET_NEEDCLRGROUP;
//				break;
//			*/
			case	CON_PROTOCOL_DL64507:			//645规约2007 yzy
				m=Meter_DI64507_DownExplain_Manage(pstRS485Task,ucMeterNo,pSendBuffer,m);
				if(m>0)		ucRet = RET_SUCCESS;
				break;
			case	CON_PROTOCOL_INVALID://无效规约
				break;
			}
			(*pusLen) = m;
		}
	}
	return ucRet;
}
//中继接收处理函数
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
		{//特征码为零,不考虑截取
			usBeginPos=0;
			usEndPos=usRecvLen;			//注意注意注意
		}else
		{
			for( i=0;i<usRecvLen;i++)
			{//查找特征码 
				if( pucRecvBuffer[i]==pstRS485SendTask->stRelayPara.ucCharacterChar )
					break;
			}
			if(i>=usRecvLen)
			{//没有找到特征码
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
//azh 单独搞个缓冲
        if(usDataLen > CONRS485BIGMAXLEN) usDataLen = CONRS485BIGMAXLEN;
        stRelayDataBuf.usDataLen = usDataLen;
        memcpy(stRelayDataBuf.ucDataArea,pucRecvBuffer+usBeginPos,usDataLen);

	}else
	{
//		stRS485TaskRecvExplain[0].ucDataLen=0;
//		memset(stRS485TaskRecvExplain[0].ucDataArea,0,CONRS485DATAMAXLEN);
        stRelayDataBuf.usDataLen = 0;
	}
	(*pucRecvNum)=1;		//结果是1项内容
	return ucRet;
}
//解释接收的报文
//	处理成标准的数据项，数据项以全国规约为基础建立标准
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
	if(pstRS485SendTask->ucReportType==CON_RS485_REPORT_TYPE_RELAY)//中继命令
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
//				case	CON_PROTOCOL_DL645:	//645规约
//				case	CON_PROTOCOL_AS1_DL645:		//645规约类似一
//				case	CON_PROTOCOL_AS2_DL645:		//645规约类似二
//
//				case	CON_PROTOCOL_HLi:		//浙江华立 (645)
//				case	CON_PROTOCOL_KL:		//科陆 (645)
//				case	CON_PROTOCOL_LD:		//龙电 (645)
//				case	CON_PROTOCOL_SKD:		//深科达 (645)
//				case	CON_PROTOCOL_JSLY:		//江苏羚羊 (645)
//				case	CON_PROTOCOL_HND645:		//浩宁达 (645)
//				case	CON_PROTOCOL_DFDZ:		//东方电子 (645)
//				case	CON_PROTOCOL_JQ:		//金雀 (645)
//					ucRet= Meter_DI_Type_UpExplain_Manage(pstRS485SendTask,pRecvBuffer,usRecvLen,pucExplainResultNum);
//					break;
//				case	CON_PROTOCOL_HB_PT	:	//鄂规之普通电子式电能表
//					ucRet= HBPT_DI_Type_UpExplain_Manage(pstRS485SendTask,pRecvBuffer,usRecvLen,pucExplainResultNum);
//					break;
//				case	CON_PROTOCOL_HB_JM	:	//鄂规之居民复费率电能表
//					ucRet= HBJM_DI_Type_UpExplain_Manage(pstRS485SendTask,pRecvBuffer,usRecvLen,pucExplainResultNum);
//					break;
				/*
				case	CON_PROTOCOL_ABB_R:	//ABB方表
					ucRet= ABBR_DI_Type_UpExplain_Manage(pstRS485SendTask,pRecvBuffer,usRecvLen,pucExplainResultNum);
					break;
				case	CON_PROTOCOL_LANDD://西门子D表
					ucRet= LANDD_DI_Type_UpExplain_Manage(pstRS485SendTask,pRecvBuffer,usRecvLen,pucExplainResultNum);
					break;
				case	CON_PROTOCOL_LANDB://西门子B表
					ucRet= LANDB_DI_Type_UpExplain_Manage(pstRS485SendTask,pRecvBuffer,usRecvLen,pucExplainResultNum);
					break;
				case	CON_PROTOCOL_WS	:	//威胜规约
					ucRet= WS_DI_Type_UpExplain_Manage(pstRS485SendTask,pRecvBuffer,usRecvLen,pucExplainResultNum);
					break;
				case	CON_PROTOCOL_GENIUS	:	//红相规约
					ucRet= Genius_DI_Type_UpExplain_Manage(pstRS485SendTask,pRecvBuffer,usRecvLen,pucExplainResultNum);
					break;
				case      CON_PROTOCOL_HND://浩宁达
				      ucRet= HND_DI_Type_UpExplain_Manage(pstRS485SendTask,pRecvBuffer,usRecvLen,pucExplainResultNum);
					break;
				*/
				case	CON_PROTOCOL_DL64507:	//645规约2007 yzy
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

//获得分项
unsigned char _Meter_Protocol_GetExplainResult(unsigned char ucNo,STRS485TASK *pstRS485RecvExpainVal)
{
	unsigned char ucRet=RET_SUCCESS;
//	unsigned char usDataLen;
	if(pstRS485RecvExpainVal!=NULL)
	{
		if((ucNo==0) && (stRS485TaskRecvExplain[0].ucReportType==CON_RS485_REPORT_TYPE_RELAY))	//中继命令
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

//基本帧格式检查
//	检查帧长度，帧头尾和校验和
//	返回ERROR表示此帧可以全部丢弃
unsigned char _Meter_Protocol_CheckFrame(STRS485TASK *pstRS485Task,unsigned char *pRecvBuffer,unsigned short usRecvLen,unsigned short *pusJumpLen )
{
	unsigned char ucRet = RET_ERROR,ucMeterNo=0;
	ucMeterNo = Get_MeterNo_byMPNo(pstRS485Task->ucDevNo);
	if(ucMeterNo!=0xFF)
	{
		switch(stMPNoAddrList.stMPNoAddrRec[ucMeterNo].ucProtocolNo)
		{
		case	CON_PROTOCOL_DL645:		//645规约
//		case	CON_PROTOCOL_AS1_DL645:		//645规约类似一
//		case	CON_PROTOCOL_AS2_DL645:		//645规约类似二
//
//		case	CON_PROTOCOL_HLi:		//浙江华立 (645)
//		case	CON_PROTOCOL_KL:		//科陆 (645)
//		case	CON_PROTOCOL_LD:		//龙电 (645)
//		case	CON_PROTOCOL_SKD:		//深科达 (645)
//		case	CON_PROTOCOL_JSLY:		//江苏羚羊 (645)
//		case	CON_PROTOCOL_HND645:		//浩宁达 (645)
//		case	CON_PROTOCOL_DFDZ:		//东方电子 (645)
//		case	CON_PROTOCOL_JQ:		//金雀 (645)
		case	CON_PROTOCOL_DL64507:		//645规约2007 yzy
			ucRet = DL645_Protocol_CheckFrame(pRecvBuffer,usRecvLen,pusJumpLen);
			break;
//		case	CON_PROTOCOL_HB_PT	:	//鄂规之普通电子式电能表
//			ucRet = HBPT_Protocol_CheckFrame(pstRS485Task,pRecvBuffer,usRecvLen,pusJumpLen);
//			break;
//		case	CON_PROTOCOL_HB_JM	:	//鄂规之居民复费率电能表
//			ucRet = HBJM_Protocol_CheckFrame(pstRS485Task,pRecvBuffer,usRecvLen,pusJumpLen);
//			break;
		/*
		case	CON_PROTOCOL_ABB_R:		//ABB方表
			ucRet = ABBR_Protocol_CheckFrame(pstRS485Task,pRecvBuffer,usRecvLen,pusJumpLen);
			break;
		case	CON_PROTOCOL_LANDB:		//西门子B表
			ucRet = LANDB_Protocol_CheckFrame(pstRS485Task,pRecvBuffer,usRecvLen,pusJumpLen);
			break;
		case	CON_PROTOCOL_LANDD:		//西门子D表
			ucRet = LANDD_Protocol_CheckFrame(pstRS485Task,pRecvBuffer,usRecvLen,pusJumpLen);
			break;
		case	CON_PROTOCOL_WS	:	//威胜规约
			ucRet = WS_Protocol_CheckFrame(pstRS485Task,pRecvBuffer,usRecvLen,pusJumpLen);
			break;
		case	CON_PROTOCOL_GENIUS	:	//红相
			ucRet = Genius_Protocol_CheckFrame(pstRS485Task,pRecvBuffer,usRecvLen,pusJumpLen);
			break;
		case	CON_PROTOCOL_HND:		//浩宁达规约
			ucRet = HND_Protocol_CheckFrame(pstRS485Task,pRecvBuffer,usRecvLen,pusJumpLen);
			break;
		*/
		case	CON_PROTOCOL_INVALID:	//无效规约
			ucRet = RET_ERROR;
		}
	}
	return ucRet;
}

//unsigned char _Meter_GetReSendContent(STRS485TASK *pstRS485SendTask,STRS485TASK *pstRS485RecvTask,STRS485TASK *pstRS485RetTask)
//{//返回：RET_SUCCESS;RET_ERROR;RET_COMM_TIMEOVER:一组通信结束，包括重发
//	unsigned char ucRet = RET_ERROR;
//	unsigned char ucMeterNo=0;
//	ucMeterNo = Get_MeterNo_byMPNo(pstRS485SendTask->ucDevNo);
//	if(ucMeterNo!=0xFF)
//	{
//		switch(stMPNoAddrList.stMPNoAddrRec[ucMeterNo].ucProtocolNo)
//		{
//		case	CON_PROTOCOL_DL645:		//645规约
//		case	CON_PROTOCOL_AS1_DL645:		//645规约类似一
//		case	CON_PROTOCOL_AS2_DL645:		//645规约类似二
//			
//		case	CON_PROTOCOL_HLi:		//浙江华立 (645)
//		case	CON_PROTOCOL_KL:		//科陆 (645)
//		case	CON_PROTOCOL_LD:		//龙电 (645)
//		case	CON_PROTOCOL_SKD:		//深科达 (645)
//		case	CON_PROTOCOL_JSLY:		//江苏羚羊 (645)
//		case	CON_PROTOCOL_HND645:		//浩宁达 (645)
//		case	CON_PROTOCOL_DFDZ:		//东方电子 (645)
//		case	CON_PROTOCOL_JQ:		//金雀 (645)
//        case	CON_PROTOCOL_DL64507:		//645规约2007 yzy
//			ucRet = DL645_GetReSendContent(pstRS485SendTask,pstRS485RecvTask,pstRS485RetTask);
//			break;
//		case  	CON_PROTOCOL_HB_PT	:	//鄂规之普通电子式电能表
//			ucRet = HBPT_GetReSendContent(pstRS485SendTask,pstRS485RecvTask,pstRS485RetTask);
//			break;
//		case  	CON_PROTOCOL_HB_JM	:	//鄂规之居民复费率电能表
//			ucRet = HBJM_GetReSendContent(pstRS485SendTask,pstRS485RecvTask,pstRS485RetTask);
//			break;
		/*
		case	CON_PROTOCOL_ABB_R:		//ABB方表
			ucRet = ABBR_GetReSendContent(pstRS485SendTask,pstRS485RecvTask,pstRS485RetTask);
			break;
		case	CON_PROTOCOL_LANDB:		//西门子B表
			ucRet = LANDB_GetReSendContent(pstRS485SendTask,pstRS485RecvTask,pstRS485RetTask);
			break;
		case	CON_PROTOCOL_LANDD:		//西门子D表
			ucRet = LANDD_GetReSendContent(pstRS485SendTask,pstRS485RecvTask,pstRS485RetTask);
			break;
		case	CON_PROTOCOL_WS	:	//威胜规约
			ucRet = WS_GetReSendContent(pstRS485SendTask,pstRS485RecvTask,pstRS485RetTask);
			break;
		case  	CON_PROTOCOL_GENIUS	:	//红相规约
			ucRet = Genius_GetReSendContent(pstRS485SendTask,pstRS485RecvTask,pstRS485RetTask);
			break;
		case	CON_PROTOCOL_HND:		//浩宁达规约
			ucRet = HND_GetReSendContent(pstRS485SendTask,pstRS485RecvTask,pstRS485RetTask);
			break;
		*/
//		case	CON_PROTOCOL_INVALID:	//无效规约
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
//		case	CON_PROTOCOL_DL645:		//645规约
//		case	CON_PROTOCOL_AS1_DL645:		//645规约类似一
//		case	CON_PROTOCOL_AS2_DL645:		//645规约类似二
//			
//		case	CON_PROTOCOL_HLi:		//浙江华立 (645)
//		case	CON_PROTOCOL_KL:		//科陆 (645)
//		case	CON_PROTOCOL_LD:		//龙电 (645)
//		case	CON_PROTOCOL_SKD:		//深科达 (645)
//		case	CON_PROTOCOL_JSLY:		//江苏羚羊 (645)
//		case	CON_PROTOCOL_HND645:		//浩宁达 (645)
//		case	CON_PROTOCOL_DFDZ:		//东方电子 (645)
//		case	CON_PROTOCOL_JQ:		//金雀 (645)
//		case	CON_PROTOCOL_DL64507:		//645规约2007 yzy
//			ucRet = DL645_DispartOverTime(pstRS485RecvTask,pucExplainResultNum);
//			break;
//		case	CON_PROTOCOL_HB_PT	:	//鄂规之普通电子式电能表
//			ucRet = HBPT_DispartOverTime(pstRS485RecvTask,pucExplainResultNum);
//			break;
//		case	CON_PROTOCOL_HB_JM	:	//鄂规之居民复费率电能表
//			ucRet = HBJM_DispartOverTime(pstRS485RecvTask,pucExplainResultNum);
//			break;
		/*
		case	CON_PROTOCOL_ABB_R:		//ABB方表
			ucRet = ABBR_DispartOverTime(pstRS485RecvTask,pucExplainResultNum);
			break;
		case	CON_PROTOCOL_LANDB:		//西门子B表
		case	CON_PROTOCOL_LANDD:		//西门子D表
			ucRet = LANDD_DispartOverTime(pstRS485RecvTask,pucExplainResultNum);
			break;
		case	CON_PROTOCOL_WS	:	//威胜规约
			ucRet = WS_DispartOverTime(pstRS485RecvTask,pucExplainResultNum);
			break;
		case	CON_PROTOCOL_GENIUS	:	//红相规约
			ucRet = Genius_DispartOverTime(pstRS485RecvTask,pucExplainResultNum);
			break;
		case	CON_PROTOCOL_HND:		//浩宁达规约
			ucRet = HND_DispartOverTime(pstRS485RecvTask,pucExplainResultNum);
			break;
		*/
//		case	CON_PROTOCOL_INVALID:	//无效规约
//			ucRet = RET_ERROR;
//		}
//	}
//	return ucRet;
//}


//获得帧传输的时间常数
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
//			case	CON_PROTOCOL_DL645:		//645规约
//			case	CON_PROTOCOL_AS1_DL645:		//645规约类似一
//			case	CON_PROTOCOL_AS2_DL645:		//645规约类似二
//
//			case	CON_PROTOCOL_HLi:		//浙江华立 (645)
//			case	CON_PROTOCOL_KL:		//科陆 (645)
//			case	CON_PROTOCOL_LD:		//龙电 (645)
//			case	CON_PROTOCOL_SKD:		//深科达 (645)
//			case	CON_PROTOCOL_JSLY:		//江苏羚羊 (645)
//			case	CON_PROTOCOL_HND645:		//浩宁达 (645)
//			case	CON_PROTOCOL_DFDZ:		//东方电子 (645)
//			case	CON_PROTOCOL_JQ:		//金雀 (645)
//			case	CON_PROTOCOL_DL64507:		//645规约2007 yzy
//				DL645_GetTimerConst(pstRS485Task,pucChID,pusMaxRecvOverTime,pusMaxByteOverTime,pusMaxSendDelay);
//				break;
//			case	CON_PROTOCOL_HB_PT	:	//鄂规之普通电子式电能表
//				HBPT_GetTimerConst(pstRS485Task,pucChID,pusMaxRecvOverTime,pusMaxByteOverTime,pusMaxSendDelay);
//				break;
//			case	CON_PROTOCOL_HB_JM	:	//鄂规之居民复费率电能表
//				HBJM_GetTimerConst(pstRS485Task,pucChID,pusMaxRecvOverTime,pusMaxByteOverTime,pusMaxSendDelay);
//				break;
//			/*
//			case	CON_PROTOCOL_ABB_R:		//ABB方表
//				ABBR_GetTimerConst(pstRS485Task,pucChID,pusMaxRecvOverTime,pusMaxByteOverTime,pusMaxSendDelay);
//				break;
//			case	CON_PROTOCOL_LANDB:		//西门子B表
//			case	CON_PROTOCOL_LANDD:		//西门子D表
//				LANDD_GetTimerConst(pstRS485Task,pucChID,pusMaxRecvOverTime,pusMaxByteOverTime,pusMaxSendDelay);
//				break;
//			case	CON_PROTOCOL_WS	:		//威胜规约
//				WS_GetTimerConst(pstRS485Task,pucChID,pusMaxRecvOverTime,pusMaxByteOverTime,pusMaxSendDelay);
//				break;
//			case	CON_PROTOCOL_GENIUS	:	//红相规约
//				Genius_GetTimerConst(pstRS485Task,pucChID,pusMaxRecvOverTime,pusMaxByteOverTime,pusMaxSendDelay);
//				break;
//			case	CON_PROTOCOL_HND:		//浩宁达规约
//				HND_GetTimerConst(pstRS485Task,pucChID,pusMaxRecvOverTime,pusMaxByteOverTime,pusMaxSendDelay);
//				break;
//			*/
//			case	CON_PROTOCOL_INVALID:	//无效规约
//				ucRet =RET_ERROR;
//			}
//		}
//	}else
//	{
////		(*pucChID)				=RS485_CH_ID;
//		(*pusMaxRecvOverTime)	=pstRS485Task->stRelayPara.ucRecvWaitTime;	//单位外部10ms
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
//		case	CON_PROTOCOL_DL645:		//645规约
//			//DL645_Protocol_GetPortPara(pulBaud,pucEPN,pucDataBit);
//			break;
//		case	CON_PROTOCOL_ABB_R:		//ABB方表
//			//ABBR_Protocol_GetPortPara(pulBaud,pucEPN,pucDataBit);
//			break;
//		case	CON_PROTOCOL_LANDD:		//西门子D表
//			//LANDD_Protocol_GetPortPara(pulBaud,pucEPN,pucDataBit);
//			break;
//		case	CON_PROTOCOL_LANDB:		//西门子B表
//			//ucRet =LANDB_Protocol_GetPortPara(pulBaudRate,pucEPN,pucDataBit,pucStopBit);
//			break;
//		case	CON_PROTOCOL_WS	:		//威胜规约
//			//WS_Protocol_GetPortPara(pulBaud,pucEPN,pucDataBit);
//			break;
////		case	CON_PROTOCOL_GENIUS:	//红相规约
//			//Genius_Protocol_GetPortPara(pulBaud,pucEPN,pucDataBit);
////			break;
//		case      CON_PROTOCOL_HND:   //浩宁达规约
//			//ucRet =HND_Protocol_GetPortPara(pulBaudRate,pucEPN,pucDataBit);
//			break;
//		case	CON_PROTOCOL_DL64507:		//645规约2007 yzy
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
//		(*pulBaudRate)		= 1200;		//1200波特率
//		(*pucEPN)		= UART_EVEN_PARITY;		//偶校验
//		(*pucDataBit)	= 8;		//8位数据位
//		ucRet = RET_SUCCESS;		
//	}
//	return ucRet;
//}
//unsigned char _Meter_Protocol_GetTimerCommand(STCMDFLAG	stCmdFlag,STCMDFLAG stIsSendFlag,STRS485TASKCMDLIST	*pstRS485TaskCmdList)
//{
//	unsigned char ucRet=RET_SUCCESS,m=0;
//	//正向有功		
//	if(stIsSendFlag.ucDirPowerFlag   == CONCOMMAND_SEND)	GetCommand_DirPower(stCmdFlag,pstRS485TaskCmdList,&m);
//	//反向无功
//	if(stIsSendFlag.ucUnDirVarFlag == CONCOMMAND_SEND)		GetCommand_UnDirVar(stCmdFlag,pstRS485TaskCmdList,&m);
//	//反向有功	
//	if(stIsSendFlag.ucUnDirPowerFlag == CONCOMMAND_SEND)	GetCommand_UnDirPower(stCmdFlag,pstRS485TaskCmdList,&m);
//	//正向无功
//	if(stIsSendFlag.ucDirVarFlag     == CONCOMMAND_SEND)	GetCommand_DirVar(stCmdFlag,pstRS485TaskCmdList,&m);
//	//1象限无功
//	if(stIsSendFlag.ucVar1Flag == CONCOMMAND_SEND)			GetCommand_Var1(stCmdFlag,pstRS485TaskCmdList,&m);
//	//2象限无功
//	if(stIsSendFlag.ucVar2Flag == CONCOMMAND_SEND)			GetCommand_Var2(stCmdFlag,pstRS485TaskCmdList,&m);
//	//3象限无功
//	if(stIsSendFlag.ucVar3Flag == CONCOMMAND_SEND)			GetCommand_Var3(stCmdFlag,pstRS485TaskCmdList,&m);
//	//4象限无功
//	if(stIsSendFlag.ucVar4Flag == CONCOMMAND_SEND)			GetCommand_Var4(stCmdFlag,pstRS485TaskCmdList,&m);
//	//正向有功最大需量
//	if(stIsSendFlag.ucDirPowerMaxDemandFlag == CONCOMMAND_SEND)			GetCommand_DirPowerMaxDemand(stCmdFlag,pstRS485TaskCmdList,&m);
//	//正向有功需量发生时间
//	if(stIsSendFlag.ucDirPowerMaxDemandTimeFlag == CONCOMMAND_SEND)		GetCommand_DirPowerMaxDemandTime(stCmdFlag,pstRS485TaskCmdList,&m);
//	//正向无功最大需量
//	if(stIsSendFlag.ucDirVarMaxDemandFlag == CONCOMMAND_SEND)			GetCommand_DirVarMaxDemand(stCmdFlag,pstRS485TaskCmdList,&m);
//	//正向无功需量发生时间
//	if(stIsSendFlag.ucDirVarMaxDemandTimeFlag == CONCOMMAND_SEND)		GetCommand_DirVarMaxDemandTime(stCmdFlag,pstRS485TaskCmdList,&m);
//	//反向有功最大需量
//	if(stIsSendFlag.ucUnDirPowerMaxDemandFlag == CONCOMMAND_SEND)		GetCommand_UnDirPowerMaxDemand(stCmdFlag,pstRS485TaskCmdList,&m);
//	//反向有功最大需量发生时间
//	if(stIsSendFlag.ucUnDirPowerMaxDemandTimeFlag == CONCOMMAND_SEND)	GetCommand_UnDirPowerMaxDemandTime(stCmdFlag,pstRS485TaskCmdList,&m);
//	//反向无功最大需量
//	if(stIsSendFlag.ucUnDirVarMaxDemandFlag == CONCOMMAND_SEND)		GetCommand_UnDirVarMaxDemand(stCmdFlag,pstRS485TaskCmdList,&m);
//	//反向无功最大需量发生时间
//	if(stIsSendFlag.ucUnDirVarMaxDemandTimeFlag == CONCOMMAND_SEND)	GetCommand_UnDirVarMaxDemandTime(stCmdFlag,pstRS485TaskCmdList,&m);
//
//	//正向有功(上月)
//	if(stIsSendFlag.ucDirPowerLmFlag == CONCOMMAND_SEND)		GetCommand_DirPowerLm(stCmdFlag,pstRS485TaskCmdList,&m);	
//	//反向有功(上月)	
//	if(stIsSendFlag.ucUnDirPowerLmFlag == CONCOMMAND_SEND)		GetCommand_UnDirPowerLm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//正向无功(上月)
//	if(stIsSendFlag.ucDirVarLmFlag     == CONCOMMAND_SEND)		GetCommand_DirVarLm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//反向无功(上月)
//	if(stIsSendFlag.ucUnDirVarLmFlag == CONCOMMAND_SEND)		GetCommand_UnDirVarLm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//1象限无功(上月)
//	if(stIsSendFlag.ucVar1LmFlag == CONCOMMAND_SEND)			GetCommand_Var1Lm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//2象限无功(上月)
//	if(stIsSendFlag.ucVar2LmFlag == CONCOMMAND_SEND)			GetCommand_Var2Lm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//3象限无功(上月)
//	if(stIsSendFlag.ucVar3LmFlag == CONCOMMAND_SEND)			GetCommand_Var3Lm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//4象限无功(上月)
//	if(stIsSendFlag.ucVar4LmFlag == CONCOMMAND_SEND)			GetCommand_Var4Lm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//正向有功最大需量(上月)
//	if(stIsSendFlag.ucDirPowerMaxDemandLmFlag == CONCOMMAND_SEND)		GetCommand_DirPowerMaxDemandLm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//正向有功需量发生时间(上月)
//	if(stIsSendFlag.ucDirPowerMaxDemandTimeLmFlag == CONCOMMAND_SEND)	GetCommand_DirPowerMaxDemandTimeLm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//正向无功最大需量(上月)
//	if(stIsSendFlag.ucDirVarMaxDemandLmFlag == CONCOMMAND_SEND)			GetCommand_DirVarMaxDemandLm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//正向无功需量发生时间(上月)
//	if(stIsSendFlag.ucDirVarMaxDemandTimeLmFlag == CONCOMMAND_SEND)		GetCommand_DirVarMaxDemandTimeLm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//反向有功最大需量(上月)
//	if(stIsSendFlag.ucUnDirPowerMaxDemandLmFlag == CONCOMMAND_SEND)		GetCommand_UnDirPowerMaxDemandLm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//反向有功最大需量发生时间(上月)
//	if(stIsSendFlag.ucUnDirPowerMaxDemandTimeLmFlag == CONCOMMAND_SEND)	GetCommand_UnDirPowerMaxDemandTimeLm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//反向无功最大需量(上月)
//	if(stIsSendFlag.ucUnDirVarMaxDemandLmFlag == CONCOMMAND_SEND)		GetCommand_UnDirVarMaxDemandLm(stCmdFlag,pstRS485TaskCmdList,&m);
//	//反向无功最大需量发生时间(上月)
//	if(stIsSendFlag.ucUnDirVarMaxDemandTimeLmFlag == CONCOMMAND_SEND)	GetCommand_UnDirVarMaxDemandTimeLm(stCmdFlag,pstRS485TaskCmdList,&m);
//	
//	//三相电压
//	if(stIsSendFlag.ucVolFlag == CONCOMMAND_SEND)		GetCommand_Uabc(stCmdFlag,pstRS485TaskCmdList,&m);
//	//三相电流
//	if(stIsSendFlag.ucCurFlag == CONCOMMAND_SEND)		GetCommand_Iabc(stCmdFlag,pstRS485TaskCmdList,&m);
//
//	//瞬时有功
//	if(stIsSendFlag.ucInstantPowFlag == CONCOMMAND_SEND)	GetCommand_InstantPow(stCmdFlag,pstRS485TaskCmdList,&m);
//	//瞬时无功
//	if(stIsSendFlag.ucInstantVarFlag == CONCOMMAND_SEND)	GetCommand_InstantVar(stCmdFlag,pstRS485TaskCmdList,&m);
//
//	//编程时间等
//	if(stIsSendFlag.ucProgramTimeFlag == CONCOMMAND_SEND)	GetCommand_ProgramTime(stCmdFlag,pstRS485TaskCmdList,&m);
//
//	//脉冲常数等
//	if(stIsSendFlag.ucPulseConstFlag == CONCOMMAND_SEND)	GetCommand_PulseConst(stCmdFlag,pstRS485TaskCmdList,&m);
//
//	//电表状态字
//	if(stIsSendFlag.ucMeterStatFlag == CONCOMMAND_SEND)		GetCommand_MeterStat(stCmdFlag,pstRS485TaskCmdList,&m);
//
//	//抄表日
//	if(stIsSendFlag.ucCopyDayTimeFlag == CONCOMMAND_SEND)	GetCommand_CopyDayTime(stCmdFlag,pstRS485TaskCmdList,&m);
//
//	//断相次数
//	if(stIsSendFlag.ucLostVolNumFlag == CONCOMMAND_SEND)	GetCommand_LostVolNum(stCmdFlag,pstRS485TaskCmdList,&m);
//	//断相累计时间
//	if(stIsSendFlag.ucLostVolTimeFlag == CONCOMMAND_SEND)	GetCommand_LostVolTime(stCmdFlag,pstRS485TaskCmdList,&m);
//	//最近一次断相开始时间
//	if(stIsSendFlag.ucLastVolOffBegTimeFlag == CONCOMMAND_SEND)	GetCommand_LastVolOffBegTime(stCmdFlag,pstRS485TaskCmdList,&m);
//	//最近一次断相结束时间
//	if(stIsSendFlag.ucLastVolOffEndTimeFlag == CONCOMMAND_SEND)	GetCommand_LastVolOffEndTime(stCmdFlag,pstRS485TaskCmdList,&m);
//
//	//费率
//	if(stIsSendFlag.ucFeeRateFlag == CONCOMMAND_SEND)			GetCommand_FeeRate(stCmdFlag,pstRS485TaskCmdList,&m);
//
//	//时钟
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
//		da=((unsigned char)(crc/256))/16;  	/* 暂存CRC的高四位 */
//		crc<<=4;                  	 		/* CRC右移4位，相当于取CRC的低12位）*/
//		crc^=crc_ta[da^(*ptr/16)];     		/* CRC的高4位和本字节的前半字节相加后查表计算CRC，
//												然后加上上一次CRC的余数 */
//		da=((unsigned char)(crc/256))/16;   /* 暂存CRC的高4位 */
//		crc<<=4;                   			/* CRC右移4位，相当于CRC的低12位） */
//		crc^=crc_ta[da^(*ptr&0x0f)];   		/* CRC的高4位和本字节的后半字节相加后查表计算CRC，
//												然后再加上上一次CRC的余数 */
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

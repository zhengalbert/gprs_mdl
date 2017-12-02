/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Meter_Response.c
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

static unsigned char Meter_Init_MPNoAddrList(STMPNOADDRLIST	*pstMPNoAddrList);

//电表回应数据的处理
unsigned char Meter_Response_Manage(STRS485TASK *pstRS485RevTask)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo ;

	ucMPNo = pstRS485RevTask->ucDevNo;

	switch( stRS485RecvExpainVal.ucTaskSource )			//根据任务来源区分
	{
	case CONRS485SOURTIMER:								//报警定时任务
		if(stRS485RecvExpainVal.ucReportType==CON_RS485_REPORT_TYPE_SPEC)//超时处理
		{
			if( pstRS485RevTask->ucReportType==CON_RS485_REPORT_TYPE_GET )
				ucRet=Fill_AlarmTimer_Manage(ucMPNo,pstRS485RevTask->usDataType,CONALRTME_INVALID_FLAG);
		}
		if(stRS485RecvExpainVal.ucReportType==CON_RS485_REPORT_TYPE_GET)
			ucRet=Fill_AlarmTimer_Manage(ucMPNo,stRS485RecvExpainVal.usDataType,CONALRTME_VALID_FLAG);

		break;
	case CONRS485SOUR_ALRAFFIRMTASK:					//报警确认任务0
	case CONRS485SOUR_ALRAFFIRMTASK+1:					//报警确认任务1
	case CONRS485SOUR_ALRAFFIRMTASK+2:					//报警确认任务2
	case CONRS485SOUR_ALRAFFIRMTASK+3:					//报警确认任务3
	case CONRS485SOUR_ALRAFFIRMTASK+4:					//报警确认任务4
	case CONRS485SOUR_ALRAFFIRMTASK+5:					//报警确认任务5
	case CONRS485SOUR_ALRAFFIRMTASK+6:					//报警确认任务6
	case CONRS485SOUR_ALRAFFIRMTASK+7:					//报警确认任务7
	case CONRS485SOUR_ALRAFFIRMTASK+8:					//报警确认任务8
	case CONRS485SOUR_ALRAFFIRMTASK+9:					//报警确认任务9
		if( stRS485RecvExpainVal.ucReportType==CON_RS485_REPORT_TYPE_SPEC )//超时处理
		{
			if( pstRS485RevTask->ucReportType==CON_RS485_REPORT_TYPE_GET )
				ucRet=Fill_AlarmAffirm_Manage(ucMPNo,(unsigned char)(stRS485RecvExpainVal.ucTaskSource-CONRS485SOUR_ALRAFFIRMTASK),
												pstRS485RevTask->usDataType,CONALRTME_INVALID_FLAG);
		}
		if(stRS485RecvExpainVal.ucReportType==CON_RS485_REPORT_TYPE_GET )
			ucRet=Fill_AlarmAffirm_Manage(ucMPNo,(unsigned char)(stRS485RecvExpainVal.ucTaskSource-CONRS485SOUR_ALRAFFIRMTASK),
												stRS485RecvExpainVal.usDataType,CONALRTME_VALID_FLAG);
		break;
	case CONRS485SOURALR_1:								//报警补招数据0
	case CONRS485SOURALR_1+1:							//报警补招数据1
	case CONRS485SOURALR_1+2:							//报警补招数据2
	case CONRS485SOURALR_1+3:							//报警补招数据3
	case CONRS485SOURALR_1+4:							//报警补招数据4
	case CONRS485SOURALR_1+5:							//报警补招数据5
	case CONRS485SOURALR_1+6:							//报警补招数据6
	case CONRS485SOURALR_1+7:							//报警补招数据7
	case CONRS485SOURALR_1+8:							//报警补招数据8
	case CONRS485SOURALR_1+9:							//报警补招数据9
		if( stRS485RecvExpainVal.ucReportType==CON_RS485_REPORT_TYPE_SPEC )//超时处理
		{
			if( pstRS485RevTask->ucReportType==CON_RS485_REPORT_TYPE_GET )
				ucRet=Fill_AlarmTake_Manage(ucMPNo,(unsigned char)(stRS485RecvExpainVal.ucTaskSource-CONRS485SOURALR_1),
												pstRS485RevTask->usDataType,CONALRTME_INVALID_FLAG);
		}
		if(stRS485RecvExpainVal.ucReportType==CON_RS485_REPORT_TYPE_GET)	//
				ucRet=Fill_AlarmTake_Manage(ucMPNo,(unsigned char)(stRS485RecvExpainVal.ucTaskSource-CONRS485SOURALR_1),
												stRS485RecvExpainVal.usDataType,CONALRTME_VALID_FLAG);
		break;
	case CON_TERMSG_SOUR_RS232:case CON_TERMSG_SOUR_IRDA://外界任务:中继 
	case CON_TERMSG_SOUR_CSD  :case CON_TERMSG_SOUR_AUD:
	case CON_TERMSG_SOUR_RCOMM_SMS:case CON_TERMSG_SOUR_RCOMM_TCP:
	case CON_TERMSG_SOUR_RCOMM_UDP:  
		if(stRS485RecvExpainVal.ucReportType==CON_RS485_REPORT_TYPE_RELAY)
		{
//			if(stRS485RecvExpainVal.stRelayPara.ucFnVal == 1)
				ucRet=Fill_DataTrans_UpSendData_Manage(stRS485RecvExpainVal.ucTaskSource,stRS485RecvExpainVal.ucDevNo);
//			else if(stRS485RecvExpainVal.stRelayPara.ucFnVal == 9)
//				ucRet = Fill_DataTrans_UpSendDataF9_Manage(stRS485RecvExpainVal.ucTaskSource,stRS485RecvExpainVal.ucDevNo,0);
				
		}
//		if(stRS485RecvExpainVal.ucReportType == CON_RS485_REPORT_TYPE_SPEC)
//		{
//			if(pstRS485RevTask->ucReportType == CON_RS485_REPORT_TYPE_RELAY)
//			{
//				if(stRS485RecvExpainVal.stRelayPara.ucFnVal = 9)
//					ucRet = Fill_DataTrans_UpSendDataF9_Manage(stRS485RecvExpainVal.ucTaskSource,stRS485RecvExpainVal.ucDevNo,1);
//			}
//		}
		break;
	case CONRS485SOUR_GRTASK:							//GRTASK
		if( stRS485RecvExpainVal.ucReportType==CON_RS485_REPORT_TYPE_SPEC )//超时处理
			ucRet=GRTask_Response_Manage_RecvOverTime(stRS485RecvExpainVal.ucDevNo);	
		if(stRS485RecvExpainVal.ucReportType==CON_RS485_REPORT_TYPE_RELAY)//中继处理
			ucRet=GRTask_Response_Manage_Relay(stRS485RecvExpainVal.ucDevNo);	
		if(stRS485RecvExpainVal.ucReportType==CON_RS485_REPORT_TYPE_GET)//
			ucRet=GRTask_Response_Manage_Normal(stRS485RecvExpainVal.ucDevNo);
		break;
	default: 
		ucRet=RET_ERROR;
		break;
	}
	return ucRet;
}

/*************************************GRTASK返回处理***************************************/
unsigned char GRTask_Response_Manage_RecvOverTime(unsigned char ucMPNo)
{
	unsigned char ucRet=RET_SUCCESS;

	if( stRS485TaskSend.ucReportType==CON_RS485_REPORT_TYPE_RELAY && 
		stRS485TaskSend.usDataType==CON_DI_TYPE_MPRELAY )
	{//处理中继超时
		stGRDefineArea.stGRTaskRetVal.stDIVal[0].ucDILen=00;
	}else
		ucRet=Fill_TaskData_Manage_Invalid(stRS485TaskSend.usDataType,stRS485TaskSend.ucTaskSource,stRS485TaskSend.ucDevNo);

	return ucRet;
}
unsigned char GRTask_Response_Manage_Relay(unsigned char ucMPNo)
{//中继
	unsigned char ucRet=RET_SUCCESS,i=0;

	if(	stRS485RecvExpainVal.usDataType==CON_DI_TYPE_MPRELAY )
	{
		for(i=0;i<stRS485RecvExpainVal.ucDataLen;i++)
//			stGRDefineArea.stGRTaskRetVal.stDIVal[0].ucDIVAL[i]=stRS485RecvExpainVal.ucDataArea[i];	
            stGRDefineArea.stGRTaskRetVal.ucTaskData[i]=stRS485RecvExpainVal.ucDataArea[i];	
		stGRDefineArea.stGRTaskRetVal.stDIVal[0].ucDILen=stRS485RecvExpainVal.ucDataLen;
	}

	return ucRet;
}
unsigned char GRTask_Response_Manage_Normal(unsigned char ucMPNo)
{
	unsigned char ucRet=RET_SUCCESS;

	if( stRS485RecvExpainVal.ucReportType == CON_RS485_REPORT_TYPE_GET )
		ucRet=Fill_TaskData_Manage(stRS485RecvExpainVal.usDataType,stRS485RecvExpainVal.ucTaskSource,stRS485RecvExpainVal.ucDevNo);
	return ucRet;
}

//初始化函数
void Meter_Init(unsigned long ulBoadrate,unsigned char ucPortNo)
{
//	unsigned char	ucChID = RS485_CH_ID;
	STMPNOADDRLIST	stMPNoAddrList;
//	unsigned long	ulTmp;
	
//	ucClockCount=0;

	Meter_Init_MPNoAddrList(&stMPNoAddrList);
	Meter_Protocol_Init(&stMPNoAddrList);				//规约解释接收分项数据区

//	ulTmp=Get_Correct_Boadrate(ulBoadrate);
//	Uart_InitCh(ucChID,&(stRS485Tx[ucPortNo]),&(stRS485Rx[ucPortNo]));
//	Uart_InitChBaud(ucChID,ulTmp,CON_UART_EPN_EVEN,8,CON_UART_STPB_1);
	ClearRS485Task(&stRelaySend);
	ClearRS485Task(&stRS485TaskSend);
	ClearRS485Task(&stRS485RecvExpainVal);

//	if(ucPortNo==1)
//		ucMeterTrans_ErrShowFlag = 0x55;
}
unsigned char	Meter_Init_MPNoAddrList(STMPNOADDRLIST	*pstMPNoAddrList)
{//初始化规约模块中的地址信息
	unsigned char ucRet=RET_SUCCESS,i=0;
	STMETERPARA		stMeterPara;
	unsigned char	ucMPType=CON_MPTYPE_NULL;
	unsigned char	ucMPNo=0;
	
	for(i=0;i<CON_METER_NUM_PRO;i++)
	{
		if(i < CON_METER_NUM)
		{
			ucMPNo			= RunPara_GetMeter_MPNo(i);
			stMeterPara		=RunPara_GetMeterPara(i);
			ucMPType		= RunPara_GetMPType(ucMPNo);
			
			if(ucMPType == CON_MPTYPE_METER)
			{
				pstMPNoAddrList->stMPNoAddrRec[i].ucMPNo = ucMPNo;
				pstMPNoAddrList->stMPNoAddrRec[i].ucProtocolNo	= stMeterPara.ucProtocol;
				memcpy(pstMPNoAddrList->stMPNoAddrRec[i].ucMeterPass,stMeterPara.stPassword.ucMeterPass,8);
				memcpy(pstMPNoAddrList->stMPNoAddrRec[i].ucMeterCommName,stMeterPara.stCommName.ucMeterCommName,8);
				memcpy(pstMPNoAddrList->stMPNoAddrRec[i].ucMeterAddr,stMeterPara.stMeterAddr.ucMeterAddr,6);
			}else
			{
				pstMPNoAddrList->stMPNoAddrRec[i].ucMPNo = CON_MPNO_INVALID;
				pstMPNoAddrList->stMPNoAddrRec[i].ucProtocolNo	= CON_PROTOCOL_INVALID;
				memset(pstMPNoAddrList->stMPNoAddrRec[i].ucMeterPass,0,8);
				memset(pstMPNoAddrList->stMPNoAddrRec[i].ucMeterCommName,0,8);
				memset(pstMPNoAddrList->stMPNoAddrRec[i].ucMeterAddr,0,6);
			}
		}else
		{
			pstMPNoAddrList->stMPNoAddrRec[i].ucMPNo = CON_MPNO_INVALID;
			pstMPNoAddrList->stMPNoAddrRec[i].ucProtocolNo	= CON_PROTOCOL_INVALID;
			memset(pstMPNoAddrList->stMPNoAddrRec[i].ucMeterPass,0,8);
			memset(pstMPNoAddrList->stMPNoAddrRec[i].ucMeterCommName,0,8);
			memset(pstMPNoAddrList->stMPNoAddrRec[i].ucMeterAddr,0,6);
		}
	}
	return ucRet;
}
//unsigned char Get_ChID(unsigned char ucPortNo)
//{
//	if(ucPortNo==1)
//		return RS485_CH_ID2;
//	else
//		return RS485_CH_ID;
//}
//任务结构赋值，将源任务内容，填写到目标任务内容
unsigned char SetRS485Task(STRS485TASK *pstDestTask,STRS485TASK *pstSourTask)
{
	unsigned char ucRet=RET_SUCCESS;

	if( pstDestTask!=NULL && pstSourTask!=NULL )
		memcpy(pstDestTask,pstSourTask,sizeof(STRS485TASK));
	else
		ucRet=RET_ERROR;

	return ucRet;
}

//任务结构赋值，将源任务内容，填写到目标任务内容，初始化数据区
unsigned char SetRS485TaskPer(STRS485TASK	*pstDestTask,
							  unsigned char ucTaskSource, unsigned char ucSeqNo,
							  unsigned char ucReportType,unsigned short usDataType,
							  unsigned char ucDevNo,STRELAYTASKPARA	*pstRelayTaskPara)
{
	pstDestTask->ucSeqNo		=ucSeqNo;
	pstDestTask->ucTaskSource	=ucTaskSource;
	pstDestTask->ucReportType	=ucReportType;
	pstDestTask->usDataType		=usDataType;
	pstDestTask->ucDevNo		=ucDevNo;
	pstDestTask->ucDataLen		=0;
	if(pstRelayTaskPara==NULL)
	{
		memset((unsigned char *)(&(pstDestTask->stRelayPara)),0,sizeof(pstDestTask->stRelayPara));
	}else
		pstDestTask->stRelayPara			=(*pstRelayTaskPara);

	memset(pstDestTask->ucDataArea,0,CONRS485DATAMAXLEN);
	return RET_SUCCESS;
}

//清除结构
unsigned char ClearRS485Task(STRS485TASK *pstDestTask)
{
	if(pstDestTask!=NULL)
	{
		memset((unsigned char *)pstDestTask,0,sizeof(STRS485TASK));
		pstDestTask->ucReportType=CON_RS485_REPORT_TYPE_IDLE;
	}
	return RET_SUCCESS;
}
////下面二个用于大一点空间的 比如中继升级帧
////任务结构赋值，将源任务内容，填写到目标任务内容，初始化数据区
//unsigned char SetRS485TaskBig(STRS485BIG	*pstDestTask,
//							  unsigned char ucTaskSource, unsigned char ucSeqNo,
//							  unsigned char ucReportType,unsigned short usDataType,
//							  unsigned char ucDevNo,STRELAYTASKPARA	*pstRelayTaskPara)
//{
//	pstDestTask->ucSeqNo		=ucSeqNo;
//	pstDestTask->ucTaskSource	=ucTaskSource;
//	pstDestTask->ucReportType	=ucReportType;
//	pstDestTask->usDataType		=usDataType;
//	pstDestTask->ucDevNo		=ucDevNo;
//	pstDestTask->ucDataLen		=0;
//	if(pstRelayTaskPara==NULL)
//	{
//		memset((unsigned char *)(&(pstDestTask->stRelayPara)),0,sizeof(pstDestTask->stRelayPara));
//	}else
//		pstDestTask->stRelayPara			=(*pstRelayTaskPara);
//
//	memset(pstDestTask->ucDataArea,0,CONRS485BIGMAXLEN);
//	return RET_SUCCESS;
//}
//
////清除结构
//unsigned char ClearRS485TaskBig(STRS485BIG *pstDestTask)
//{
//	if(pstDestTask!=NULL)
//	{
//		memset((unsigned char *)pstDestTask,0,sizeof(STRS485BIG));
//		pstDestTask->ucReportType=CON_RS485_REPORT_TYPE_IDLE;
//	}
//	return RET_SUCCESS;
//}


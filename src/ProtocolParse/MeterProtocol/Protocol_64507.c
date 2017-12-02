/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Protocol_64507.c
 Description    : DLT-645规约解释详细处理
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-08-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

static unsigned long usDI64507=0;

static unsigned char RS485_RecvGet07_PowVarABC_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI);
static unsigned char Fill_DataList_Byte3U_Per(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char ucContentNo,unsigned char uctype);
static unsigned char Fill_DataList_Byte3U(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char ucContentNum,unsigned char uctype);
static unsigned char Check_CONINVALIDVAL(unsigned char *pucVal,unsigned char ucLen);
static unsigned char Fill_DataList_Byte2(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char ucContentNum);
static unsigned char Fill_DataList_Byte3(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char ucContentNum);
static unsigned char RS485_RecvGet_Power_Explain1(STRS485TASK *pstRS485SendTask,unsigned short usDITotal,unsigned short usDIFee1,unsigned short usDIFee2,unsigned short usDIFee3,unsigned short usDIFee4);
static unsigned char GetRecvContent(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char *pucRecvBuffer,unsigned short usRecvLen);
//----------------------------------------------------------------------
unsigned char DL645_Protocol_CheckFrame(unsigned char *pRecvBuffer,unsigned short usRecvLen,unsigned short *pusJumpLen)
{
	unsigned char i=0,ucCheckSum=0;
	(*pusJumpLen) = 0;
	if(usRecvLen<12)					//最小长度为12，无任何数据内容
		return RET_TOO_LITTLE;
	
	if(pRecvBuffer[0]!=0x68)			//帧头
	{
		(*pusJumpLen) = 1;
		return RET_HEAD_ERROR;
	}
	
	if(pRecvBuffer[7]!=0x68)
		return RET_ERROR;
	
	if(pRecvBuffer[9]>240)				//最大长度：没有考证
		return RET_ERROR;

	if( usRecvLen<(pRecvBuffer[9]+12) )
		return RET_NOENOUGH_LONG;

	//检查校验码
	for(i=0;i<pRecvBuffer[9]+10;i++)
		ucCheckSum+=pRecvBuffer[i];
	if( ucCheckSum!=pRecvBuffer[pRecvBuffer[9]+10] )
		return RET_ERROR;
	
	if( pRecvBuffer[pRecvBuffer[9]+11]!=0x16 )
		return RET_ERROR;

	ucErrCount = 0;
	return RET_SUCCESS;
}
unsigned char Fill_DataList_DateTime_Per2(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char ucContentNo)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	STDATETIME	stDateTime;
	ClearDateTime(&stDateTime);
	if( ucContentNo<CONRECVEXPLAINNUM )
	{
		stDateTime.ucYear	=0x05;
		stDateTime.ucMonth	=0x01;
		stDateTime.ucDay	=(unsigned char)BCD_TO_HEX_1(pucRecvContent+1);
		stDateTime.ucHour	=(unsigned char)BCD_TO_HEX_1(pucRecvContent);
		stDateTime.ucMinute	=0x00;
		stDateTime.ucSecond	=0x00;

		for(i=0;i<6;i++)
			(stRS485TaskRecvExplain[ucContentNo].ucDataArea)[i]=((unsigned char *)(&stDateTime))[i];
	}else
		ucRet=RET_ERROR;
	return ucRet;
}
unsigned char GetRecvContent(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char *pucRecvBuffer,unsigned short usRecvLen)
{
	unsigned char i=0;

	for( i=0;i<ucRecvContentLen;i++ )
	{
		if( (i+12) >= (usRecvLen-2))
			break;
		else
			pucRecvContent[i]=pucRecvBuffer[i+12]-0x33;
	}

	return i;
}
unsigned char Fill_DataList_Byte3(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char ucContentNum)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucPos=0,i=0,j=0;
	unsigned long ulVal=0;
	for(j=0;j<ucContentNum;j++)	
{
		if((ucPos+3) > ucRecvContentLen)
			break;
		if( pucRecvContent[ucPos]!=0xBB )
		{
			memset(stRS485TaskRecvExplain[j].ucDataArea,0,CONRS485DATAMAXLEN);
			ulVal	=  BCD_TO_HEX_3(pucRecvContent+ucPos);
			for(i=0;i<4;i++)
				(stRS485TaskRecvExplain[j].ucDataArea)[i]=((unsigned char *)(&ulVal))[i];
			ucPos+=3;
		}else
			ucPos++;
	}
	return ucRet;
}

unsigned char Fill_DataList_Byte2(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char ucContentNum)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucPos=0,i=0,j=0;
	unsigned long ulVal=0;
	for(j=0;j<ucContentNum;j++)	
	{
		if((ucPos+2) > ucRecvContentLen)
			break;
		memset(stRS485TaskRecvExplain[j].ucDataArea,0,CONRS485DATAMAXLEN);
		if( pucRecvContent[ucPos]!=0xBB )
		{
			ulVal	=  BCD_TO_HEX_2(pucRecvContent+ucPos);
			for(i=0;i<4;i++)
				(stRS485TaskRecvExplain[j].ucDataArea)[i]=((unsigned char *)(&ulVal))[i];
			ucPos+=2;
		}else
			ucPos++;
	}
	return ucRet;
}
unsigned char Fill_DataList_Byte3U(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char ucContentNum,unsigned char uctype)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucPos=0,i=0,j=0;
	unsigned long ulVal=0;
	for(j=0;j<ucContentNum;j++)	
	{ 
		if((ucPos+3) > ucRecvContentLen)
			break;
		if( pucRecvContent[ucPos]!=0xBB )
		{
			pucRecvContent[ucPos + 2] &= 0x7f;
			memset(stRS485TaskRecvExplain[j].ucDataArea,0,CONRS485DATAMAXLEN);
			ulVal	=  BCD_TO_HEX_3(pucRecvContent+ucPos);
			if(uctype == 1)
				ulVal /= 100;
			for(i=0;i<4;i++)
				(stRS485TaskRecvExplain[j].ucDataArea)[i]=((unsigned char *)(&ulVal))[i];
			ucPos+=3;
		}else
			ucPos++;
	}
	return ucRet;
}
unsigned char Check_CONINVALIDVAL(unsigned char *pucVal,unsigned char ucLen)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned long ulVal=0;
	for(i=0;i<ucLen;i++)
	{
		if(i>=4)
			break;
		((unsigned char *)&ulVal)[i] = pucVal[i];
	}
	if(ulVal==CONINVALIDVAL)
	{
		ulVal++;
		for(i=0;i<ucLen;i++)
		{
			if(i>=4)
				break;
			pucVal[i] =((unsigned char *)&ulVal)[i];
		}
	}
	return ucRet;
}
unsigned char Fill_DataList_Byte3U_Per(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char ucContentNo,unsigned char uctype)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned long ulVal=0;
	if( ucContentNo<CONRECVEXPLAINNUM )
	{
		pucRecvContent[2] &= 0x7f;
		ulVal	=  BCD_TO_HEX_3(pucRecvContent);
		if(uctype == 1)
			ulVal /= 100;
		for(i=0;i<4;i++)
			(stRS485TaskRecvExplain[ucContentNo].ucDataArea)[i]=((unsigned char *)(&ulVal))[i];
	}else
		ucRet=RET_ERROR;
	return ucRet;
}
unsigned char Meter_DI64507_DownExplain_Manage(STRS485TASK *pstRS485Task,unsigned char ucMeterNo,unsigned char *pSendBuffer,unsigned char ucPos)
{
    unsigned char ucRet=RET_ERROR;
	unsigned char ucRetPos=ucPos;
	unsigned char ucCheckCode=0;
	unsigned long usDI64507=0;

	if( pstRS485Task->ucReportType==CON_RS485_REPORT_TYPE_GET )
	{
		pSendBuffer[ucRetPos++]=0x68;
		memcpy(pSendBuffer+ucRetPos,stMPNoAddrList.stMPNoAddrRec[ucMeterNo].ucMeterAddr,6);
		ucRetPos+=6;
		pSendBuffer[ucRetPos++]=0x68;
        pSendBuffer[ucRetPos++]=0x11;					//请求读数据
		pSendBuffer[ucRetPos++]=0x00;					//数据长度
		ucRet=GetDI64507( &usDI64507,pstRS485Task->usDataType );
		
		if( ucRet==RET_SUCCESS )
		{
		
			
			pSendBuffer[ucRetPos++]=(unsigned char )( usDI64507)+0x33;             //数据项
		    pSendBuffer[ucRetPos++]=(unsigned char)( usDI64507/0x100 )+0x33;
		    pSendBuffer[ucRetPos++]=(unsigned char)( usDI64507/0x10000 )+0x33;
		    pSendBuffer[ucRetPos++]=(unsigned char)( usDI64507/0x1000000 )+0x33;
		
		}
		if( ucRetPos>=14 )
		{
			pSendBuffer[ucRetPos-5]=ucRetPos-10;
			ucCheckCode=Get_Sum(pSendBuffer,ucRetPos);
			pSendBuffer[ucRetPos++]=ucCheckCode;
			pSendBuffer[ucRetPos++]=0x16;
		}else
			ucRetPos =0;
	}
	return ucRetPos;
}

unsigned char Meter_DI64507_UpExplain_Manage(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum)
{
	unsigned char ucRet=RET_SUCCESS;
//	unsigned long usDI64507=0;
	unsigned char ucControlCode=0,i=0;
	unsigned char DI64507[4];


	ucControlCode=pucRecvBuffer[8];
	if( ucControlCode==0x91 )
	{
	    for(i=0;i<4;i++)
	    DI64507[i]=pucRecvBuffer[10+i]-0x33;	

		usDI64507=DI64507[0]+DI64507[1]*0x100+DI64507[2]*0x10000+DI64507[3]*0x1000000;
		switch( usDI64507 )
		{
		case CON_DI64507_MPDIRPOW_BLOCK:			//读取正向有功电量
		case CON_DI64507_MPUNDIRPOW_BLOCK:			//读取反向有功电量
		case CON_DI64507_MPDIRVAR_BLOCK:			//读取正向无功电量
		case CON_DI64507_MPUNDIRVAR_BLOCK:			//读取反向无功电量
		case CON_DI64507_MPDIRPOW_LM_BLOCK:			//读取正向有功电量（上月）
		case CON_DI64507_MPUNDIRPOW_LM_BLOCK:		//读取反向有功电量（上月）
		case CON_DI64507_MPDIRVAR_LM_BLOCK:			//读取正向无功电量（上月)
		case CON_DI64507_MPUNDIRVAR_LM_BLOCK:		//读取反向无功电量（上月）
		case CON_DI64507_MPVAR1_BLOCK:				//读取无功电量_1象限
		case CON_DI64507_MPVAR2_BLOCK:				//读取无功电量_2象限
		case CON_DI64507_MPVAR3_BLOCK:				//读取无功电量_3象限
		case CON_DI64507_MPVAR4_BLOCK:				//读取无功电量_4象限
		case CON_DI64507_MPVAR1_LM_BLOCK:			//读取无功电量_1象限（上月）
		case CON_DI64507_MPVAR2_LM_BLOCK:			//读取无功电量_2象限（上月）
		case CON_DI64507_MPVAR3_LM_BLOCK:			//读取无功电量_3象限（上月）
		case CON_DI64507_MPVAR4_LM_BLOCK:			//读取无功电量_4象限（上月）
			ucRet=RS485_RecvGet07_Energy_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum,pstRS485SendTask->usDataType);
			break;
		case CON_DI64507_MPDIRPOWA:
		case CON_DI64507_MPUNDIRPOWA:
		case CON_DI64507_MPCOMVAR1A:
		case CON_DI64507_MPCOMVAR2A:
		case CON_DI64507_MPDIRPOWB:
		case CON_DI64507_MPUNDIRPOWB:
		case CON_DI64507_MPCOMVAR1B:
		case CON_DI64507_MPCOMVAR2B:
		case CON_DI64507_MPDIRPOWC:
		case CON_DI64507_MPUNDIRPOWC:
		case CON_DI64507_MPCOMVAR1C:
		case CON_DI64507_MPCOMVAR2C:
		case CON_DI64507_MPDIRPOWA_LM:
		case CON_DI64507_MPUNDIRPOWA_LM:
		case CON_DI64507_MPCOMVAR1A_LM:
		case CON_DI64507_MPCOMVAR2A_LM:
		case CON_DI64507_MPDIRPOWB_LM:
		case CON_DI64507_MPUNDIRPOWB_LM:
		case CON_DI64507_MPCOMVAR1B_LM:
		case CON_DI64507_MPCOMVAR2B_LM:
		case CON_DI64507_MPDIRPOWC_LM:
		case CON_DI64507_MPUNDIRPOWC_LM:
		case CON_DI64507_MPCOMVAR1C_LM:
		case CON_DI64507_MPCOMVAR2C_LM:
			ucRet=RS485_RecvGet07_PowVarABC_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum,pstRS485SendTask->usDataType);
			break;
		case CON_DI64507_MPDIRPOWMAXDEMTIM_BLOCK:		//正向有功最大需量及时间数据块
		case CON_DI64507_MPDIRVARMAXDEMTIM_BLOCK:		//正向无功最大需量及发生时间
		case CON_DI64507_MPUNDIRPOWMAXDEMTIM_BLOCK:		//反向有功最大需量及发生时间数据块
		case CON_DI64507_MPUNDIRVARMAXDEMTIM_BLOCK:		//反向无功最大需量及发生时间数据块
		case CON_DI64507_MPDIRPOWMAXDEMTIM_LM_BLOCK:		//正向有功最大需量及发生时间数据块（上月）
		case CON_DI64507_MPUNDIRPOWMAXDEMTIM_LM_BLOCK:		//反向有功最大需量及发生时间数据块（上月）
		case CON_DI64507_MPDIRVARMAXDEMTIM_LM_BLOCK:		//正向无功最大需量及发生时间数据块（上月）
		case CON_DI64507_MPUNDIRVARMAXDEMTIM_LM_BLOCK:		//反向无功最大需量及发生时间数据块（上月）
			ucRet=RS485_RecvGet07_MaxDemTim_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum,pstRS485SendTask->usDataType);
			break;

		case CON_DI64507_MPVOLA:
		case CON_DI64507_MPVOLB:
		case CON_DI64507_MPVOLC:
			ucRet=RS485_RecvGet07_UPer_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum,pstRS485SendTask->usDataType);
			break;
		case CON_DI64507_MPVOL_BLOCK:			//电压数据块
			ucRet=RS485_RecvGet07_UABC_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum);
			break;
		case CON_DI64507_MPCURA:
		case CON_DI64507_MPCURB:
		case CON_DI64507_MPCURC:
			ucRet=RS485_RecvGet07_IPer_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum,pstRS485SendTask->usDataType);
			break;
		case CON_DI64507_MPCUR_BLOCK:			//电流数据块
			ucRet=RS485_RecvGet07_IABC_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum);
			break;
		case CON_DI64507_MPINSTANTPOW:
		case CON_DI64507_MPINSTANTPOWA:
		case CON_DI64507_MPINSTANTPOWB:
		case CON_DI64507_MPINSTANTPOWC:
		case CON_DI64507_MPINSTANTVAR:
		case CON_DI64507_MPINSTANTVARA:
		case CON_DI64507_MPINSTANTVARB:
		case CON_DI64507_MPINSTANTVARC:
			ucRet=RS485_RecvGet07_InstantPowPer_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum,pstRS485SendTask->usDataType);
			break;
		case CON_DI64507_MPINSTANTPOW_BLOCK:	//瞬时有功数据块
		case CON_DI64507_MPINSTANTVAR_BLOCK:	//瞬时无功数据块
			ucRet=RS485_RecvGet07_InstantPow_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum,pstRS485SendTask->usDataType);
			break;
		case CON_DI64507_MPPOWCONST:						//有功脉冲常数（小项命令）
		case CON_DI64507_MPVARCONST:						//无功脉冲常数（小项命令）
			ucRet=RS485_RecvGet07_MeterConst_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum,pstRS485SendTask->usDataType);
			break;
		case CON_DI64507_MPFEERATE_BLOCK:					//费率数据块
			ucRet=RS485_RecvGet07_FeeRate_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum);
			break;

		case CON_DI64507_MPLATELYPROGTIME:					//编程时间（小项命令）
		case CON_DI64507_MPLATELYMAXDEMANDCLEARTIME:          //最大需量清零时间
		case CON_DI64507_MPMETERCLRTIME:               //最近一次电表清0时间
		case CON_DI64507_MPEVENTCLRTIME:             //最近一次事件清0时间
			ucRet=RS485_RecvGet07_ProgramTime_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum,pstRS485SendTask->usDataType);
			break;
		case CON_DI64507_MPPROGTIMETIMES:
        case CON_DI64507_MPMAXDEMANDCLEARTIMES:	              //最大需量清零次数
		case CON_DI64507_MPMETERCLRNUM:                        //电表清0总次数
		case CON_DI64507_MPEVENTCLRNUM:               //事件清0总次数
		case CON_DI64507_MPJIAOSHINUM:               //校时总次数
            ucRet=RS485_RecvGet07_ProgramTimes_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum,pstRS485SendTask->usDataType);
			break;
        case CON_DI64507_MPBATWORKTIME:                      //电池时间
            ucRet=RS485_RecvGet07_BATWORKTIME_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum);
			break;			
	   case CON_DI64507_MPJIAOSHITIME:             //最近一次校时时间
            ucRet=RS485_RecvGet07_JIAOSHITIME_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum);
		    break;		
		
		case CON_DI64507_MPMETERSTATFLAG_BLOCK:						//电表状态字块
			ucRet=RS485_RecvGet07_MeterStatFlag_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum);
			break;

		case CON_DI64507_MPCOPYDAYTIME:						//抄表日（小项命令）
			ucRet=RS485_RecvGet07_CopyDay_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum);
			break;
		case CON_DI64507_MPDATEANDWEEK:						//日期及周次
			ucRet=RS485_RecvGet07_DateAndWeek_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum);
			break;
		case CON_DI64507_MPTIME:							//时间
			ucRet=RS485_RecvGet07_Time_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum);
			break;
		case CON_DI64507_MPBREAKNUMA:						//累计断相次数时间
		case CON_DI64507_MPBREAKNUMB:	
		case CON_DI64507_MPBREAKNUMC:
		case CON_DI64507_MPBREAKTOTALTIMEA:
		case CON_DI64507_MPBREAKTOTALTIMEB:
		case CON_DI64507_MPBREAKTOTALTIMEC:
			ucRet=RS485_RecvGet07_LostVolNumTime_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum,pstRS485SendTask->usDataType);//changed by cfh 10-01-16
			break;
		case CON_DI64507_MPBREAKBEGTIME_A:	
		case CON_DI64507_MPBREAKBEGTIME_B:
		case CON_DI64507_MPBREAKBEGTIME_C:
		case CON_DI64507_MPBREAKENDTIME_A:
		case CON_DI64507_MPBREAKENDTIME_B:
		case CON_DI64507_MPBREAKENDTIME_C:	//最近一次断相起始时刻
			ucRet=RS485_RecvGet07_LastVolOffBegEnd_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum,pstRS485SendTask->usDataType);
			break;
		case CON_DI64507_MPCUFE_BLOCK:
			ucRet = RS485_RecvGet07_CuFe_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum);
			break;
		default:
			ucRet=RET_ERROR;
			break;
		}
	}else
		ucRet=RET_ERROR;

	return ucRet;
}




unsigned char RS485_RecvGet07_Energy_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI )
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucRecvContent[20];
	unsigned char ucRecvContentLen;

	ucRet=RS485_RecvGet_Power_Explain1( pstRS485SendTask,
										(unsigned short)(usDI&0xFFF0  ),(unsigned short)(usDI&0xFFF0+1),
										(unsigned short)(usDI&0xFFF0+2),(unsigned short)(usDI&0xFFF0+3),
										(unsigned short)(usDI&0xFFF0+4));
	memset( ucRecvContent,0,20 );
	ucRecvContentLen=GetRecvContent07(ucRecvContent,20,pucRecvBuffer,usRecvLen);
	Fill_PowerDataList(ucRecvContent,ucRecvContentLen);

	(*pucRecvNum)=5;

	return ucRet;
}
/*
unsigned char RS485_RecvGet07_EnergyPer_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucRecvContent[4];
	unsigned char ucRecvContentLen;
	unsigned long lVal = 0;

	if(usRecvLen<21)
		ucRet=RET_ERROR;
	if(ucRet == RET_SUCCESS)
	{
		SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
			pstRS485SendTask->ucReportType,pstRS485SendTask->usDataType,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[0].ucDataLen=4;
		FillData_EE(stRS485TaskRecvExplain[0].ucDataArea,4);
		
		memset( ucRecvContent,0,4 );
		ucRecvContentLen=GetRecvContent07(ucRecvContent,4,pucRecvBuffer,usRecvLen);

		lVal = BCD_TO_HEX_4(ucRecvContent);
		memcpy(stRS485TaskRecvExplain[0].ucDataArea,&lVal,4);

		(*pucRecvNum = 1);
	}
	return ucRet;
}
*/
unsigned char RS485_RecvGet_Power_Explain1(STRS485TASK *pstRS485SendTask,unsigned short usDITotal,unsigned short usDIFee1,unsigned short usDIFee2,unsigned short usDIFee3,unsigned short usDIFee4)
{
	unsigned char ucRet=RET_SUCCESS;

	SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
		pstRS485SendTask->ucReportType,usDITotal,pstRS485SendTask->ucDevNo,NULL);
	stRS485TaskRecvExplain[0].ucDataLen=4;
	FillData_EE(stRS485TaskRecvExplain[0].ucDataArea,4);
	//费率1
	SetRS485TaskPer(&(stRS485TaskRecvExplain[1]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
		pstRS485SendTask->ucReportType,usDIFee1,pstRS485SendTask->ucDevNo,NULL);
	stRS485TaskRecvExplain[1].ucDataLen=4;
	FillData_EE(stRS485TaskRecvExplain[1].ucDataArea,4);
	//费率2
	SetRS485TaskPer(&(stRS485TaskRecvExplain[2]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
		pstRS485SendTask->ucReportType,usDIFee2,pstRS485SendTask->ucDevNo,NULL);
	stRS485TaskRecvExplain[2].ucDataLen=4;
	FillData_EE(stRS485TaskRecvExplain[2].ucDataArea,4);
	//费率3
	SetRS485TaskPer(&(stRS485TaskRecvExplain[3]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
		pstRS485SendTask->ucReportType,usDIFee3,pstRS485SendTask->ucDevNo,NULL);
	stRS485TaskRecvExplain[3].ucDataLen=4;
	FillData_EE(stRS485TaskRecvExplain[3].ucDataArea,4);
	//费率4
	SetRS485TaskPer(&(stRS485TaskRecvExplain[4]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
		pstRS485SendTask->ucReportType,usDIFee4,pstRS485SendTask->ucDevNo,NULL);
	stRS485TaskRecvExplain[4].ucDataLen=4;
	FillData_EE(stRS485TaskRecvExplain[4].ucDataArea,4);
	
	return ucRet;
}
unsigned char RS485_RecvGet07_IPer_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI)
{//单相电流
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucRecvContent[3];
	unsigned char ucRecvContentLen;

	if(usRecvLen<19)
		ucRet=RET_ERROR;
	if(ucRet==RET_SUCCESS)
	{
		SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
							pstRS485SendTask->ucReportType,usDI,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[0].ucDataLen=4;
		FillData_EE(stRS485TaskRecvExplain[0].ucDataArea,4);

		memset( ucRecvContent,0,3 );
		ucRecvContentLen=GetRecvContent07(ucRecvContent,3,pucRecvBuffer,usRecvLen);
		Fill_DataList_Byte3U_Per(ucRecvContent,ucRecvContentLen,1,0);

		Check_CONINVALIDVAL(stRS485TaskRecvExplain[0].ucDataArea,stRS485TaskRecvExplain[0].ucDataLen);

		(*pucRecvNum)=1;//结果是1项内容
	}
	return ucRet;
}

unsigned char RS485_RecvGet07_IABC_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum)
{//三相电流处理
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucRecvContent[9];
	unsigned char ucRecvContentLen,i=0;

	if(usRecvLen<21)
		ucRet=RET_ERROR;

	if(ucRet==RET_SUCCESS)
	{
		SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
							pstRS485SendTask->ucReportType,CON_DI_TYPE_MPCURA,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[0].ucDataLen=4;
		FillData_EE(stRS485TaskRecvExplain[0].ucDataArea,4);

		SetRS485TaskPer(&(stRS485TaskRecvExplain[1]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
							pstRS485SendTask->ucReportType,CON_DI_TYPE_MPCURB,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[1].ucDataLen=4;
		FillData_EE(stRS485TaskRecvExplain[1].ucDataArea,4);

		SetRS485TaskPer(&(stRS485TaskRecvExplain[2]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
							pstRS485SendTask->ucReportType,CON_DI_TYPE_MPCURC,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[2].ucDataLen=4;
		FillData_EE(stRS485TaskRecvExplain[2].ucDataArea,4);

		memset( ucRecvContent,0,9 );
		ucRecvContentLen=GetRecvContent07(ucRecvContent,9,pucRecvBuffer,usRecvLen);
		Fill_DataList_Byte3U(ucRecvContent,ucRecvContentLen,3,0);

		for(i=0;i<3;i++)
			Check_CONINVALIDVAL(stRS485TaskRecvExplain[i].ucDataArea,stRS485TaskRecvExplain[i].ucDataLen);

		(*pucRecvNum)=3;//结果是3项内容
	}
	return ucRet;
}
unsigned char RS485_RecvGet07_UPer_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI)
{//单相电压处理
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucRecvContent[2];
	unsigned char ucRecvContentLen;

	if( usRecvLen<18 )
		ucRet=RET_ERROR;

	if(ucRet==RET_SUCCESS)
	{
		SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
							pstRS485SendTask->ucReportType,usDI,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[0].ucDataLen=4;
		FillData_EE( stRS485TaskRecvExplain[0].ucDataArea,4 );

		memset( ucRecvContent,0,2 );
		ucRecvContentLen=GetRecvContent07(ucRecvContent,2,pucRecvBuffer,usRecvLen);
		Fill_DataList_Byte2(ucRecvContent,ucRecvContentLen,1);

		Check_CONINVALIDVAL(stRS485TaskRecvExplain[0].ucDataArea,stRS485TaskRecvExplain[0].ucDataLen);

		(*pucRecvNum)=1;//结果是1项内容
	}
	return ucRet;
}
unsigned char RS485_RecvGet07_UABC_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum)
{//三相电压处理
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucRecvContent[6];
	unsigned char ucRecvContentLen;

	if(usRecvLen<20)
		ucRet=RET_ERROR;
	if(ucRet==RET_SUCCESS)
	{
		SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
							pstRS485SendTask->ucReportType,CON_DI_TYPE_MPVOLA,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[0].ucDataLen=4;
		FillData_EE(stRS485TaskRecvExplain[0].ucDataArea,4);

		SetRS485TaskPer(&(stRS485TaskRecvExplain[1]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
							pstRS485SendTask->ucReportType,CON_DI_TYPE_MPVOLB,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[1].ucDataLen=4;
		FillData_EE(stRS485TaskRecvExplain[1].ucDataArea,4);

		SetRS485TaskPer(&(stRS485TaskRecvExplain[2]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
							pstRS485SendTask->ucReportType,CON_DI_TYPE_MPVOLC,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[2].ucDataLen=4;
		FillData_EE(stRS485TaskRecvExplain[2].ucDataArea,4);

		memset( ucRecvContent,0,6 );
		ucRecvContentLen=GetRecvContent07(ucRecvContent,6,pucRecvBuffer,usRecvLen);
		Fill_DataList_Byte2(ucRecvContent,ucRecvContentLen,3);

		for(i=0;i<3;i++)
			Check_CONINVALIDVAL(stRS485TaskRecvExplain[i].ucDataArea,stRS485TaskRecvExplain[i].ucDataLen);

		(*pucRecvNum)=3;//结果是3项内容
	}
	return ucRet;
}
unsigned char RS485_RecvGet07_InstantPowPer_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI)
{//总瞬时有无功功率
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucRecvContent[3];
	unsigned char ucRecvContentLen;

	if( usRecvLen<19 )
		ucRet=RET_ERROR;

	if(ucRet==RET_SUCCESS)
	{
		SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,
			pstRS485SendTask->ucReportType,usDI,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[0].ucDataLen=4;
		FillData_EE(stRS485TaskRecvExplain[0].ucDataArea,4);
		if((usDI & 0x00f0) == 0x0030)
		{
			memset( ucRecvContent,0,3 );
			ucRecvContentLen=GetRecvContent(ucRecvContent,3,pucRecvBuffer,usRecvLen);
			Fill_DataList_Byte3U_Per(ucRecvContent,ucRecvContentLen,1,0);
		}
		else
		{
			memset( ucRecvContent,0,3 );
			ucRecvContentLen=GetRecvContent(ucRecvContent,3,pucRecvBuffer,usRecvLen);
			Fill_DataList_Byte3U_Per(ucRecvContent,ucRecvContentLen,1,1);
		}
		(*pucRecvNum)=1;
	}
		return ucRet;
}
unsigned char RS485_RecvGet07_InstantPow_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI)
{//瞬时量有无功
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucRecvContent[12];
	unsigned char ucRecvContentLen;
    for(i=0;i<4;i++)
    {
	   SetRS485TaskPer(&(stRS485TaskRecvExplain[i]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
		   pstRS485SendTask->ucReportType,(unsigned short)(usDI&0xFFF0+i),pstRS485SendTask->ucDevNo,NULL);
	   stRS485TaskRecvExplain[i].ucDataLen=4;
	   FillData_EE(stRS485TaskRecvExplain[i].ucDataArea,4);
    }
	if((usDI & 0x00f0) == 0x0030)
	{
		memset( ucRecvContent,0,12 );
		ucRecvContentLen=GetRecvContent07(ucRecvContent,12,pucRecvBuffer,usRecvLen);
		Fill_DataList_Byte3U(ucRecvContent,ucRecvContentLen,4,0);
	}
	else
	{
		memset( ucRecvContent,0,12 );
		ucRecvContentLen=GetRecvContent07(ucRecvContent,12,pucRecvBuffer,usRecvLen);
		Fill_DataList_Byte3U(ucRecvContent,ucRecvContentLen,4,1);
	}
	for(i=0;i<4;i++)
	{
		Check_CONINVALIDVAL(stRS485TaskRecvExplain[i].ucDataArea,stRS485TaskRecvExplain[i].ucDataLen);	
		if(ucRecvContentLen<12)									//检查总，肯定有效，分可能无效
			break;
	}

	(*pucRecvNum)=4;
	return ucRet;
}


unsigned char RS485_RecvGet07_MeterStatFlag_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum)
{//电表运行状态字等

	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucRecvContent[14];
	unsigned char ucRecvContentLen;	

	if( usRecvLen<30 ) 
		ucRet=RET_ERROR;

	if(ucRet == RET_SUCCESS)
	{
		SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,
							pstRS485SendTask->ucReportType,CON_DI_TYPE_MPCO2F_BLOCK,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[0].ucDataLen=14;
		FillData_EE(stRS485TaskRecvExplain[0].ucDataArea,14);

		memset( ucRecvContent,0,14 );
		ucRecvContentLen=GetRecvContent07(ucRecvContent,14,pucRecvBuffer,usRecvLen);

		for(i=0;i<ucRecvContentLen;i++)
			stRS485TaskRecvExplain[0].ucDataArea[i]=ucRecvContent[i];
		
		(*pucRecvNum)=1;
	}
	return ucRet;
}
unsigned char RS485_RecvGet07_CopyDay_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucRecvContent[2];
	unsigned char ucRecvContentLen;	

	if( usRecvLen<18 ) 
		ucRet=RET_ERROR;

	if( ucRet==RET_SUCCESS )
	{
		SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,
							pstRS485SendTask->ucReportType,CON_DI_TYPE_MPCOPYDAYTIME,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[0].ucDataLen=6;
		FillData_AllEE(stRS485TaskRecvExplain[0].ucDataArea,6);

		memset( ucRecvContent,0,2 );
		ucRecvContentLen=GetRecvContent07(ucRecvContent,2,pucRecvBuffer,usRecvLen);

		if( ucRecvContentLen >= 2)
			Fill_DataList_DateTime_Per2(ucRecvContent,ucRecvContentLen,0);
		
		(*pucRecvNum)=1;
	}
	return ucRet=RET_SUCCESS;
}


unsigned char RS485_RecvGet07_ProgramTime_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI)
{//编程时间解释
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucRecvContent[6];
	unsigned char ucRecvContentLen;	

	if( usRecvLen<22 ) 
		ucRet=RET_ERROR;

	if( ucRet==RET_SUCCESS )
	{//最近一次编程时间
		SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,
							pstRS485SendTask->ucReportType,usDI,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[0].ucDataLen=6;
		FillData_AllEE(stRS485TaskRecvExplain[0].ucDataArea,6);

		memset( ucRecvContent,0,6 );
		ucRecvContentLen=GetRecvContent07(ucRecvContent,6,pucRecvBuffer,usRecvLen);
		if( ucRecvContentLen >= 6)
			Fill_DataList_DateTime07(ucRecvContent,ucRecvContentLen,1);

		(*pucRecvNum)=1;
	}
	return ucRet;
}

unsigned char RS485_RecvGet07_ProgramTimes_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI)
{//编程次数解释
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucRecvContent[3];
	unsigned char ucRecvContentLen;
//	unsigned char m=0,i=0,ucPos=0;

	if(ucRet==RET_SUCCESS)
	{
		//编程次数
		SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,
							pstRS485SendTask->ucReportType,usDI,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[0].ucDataLen=4;
		FillData_EE(stRS485TaskRecvExplain[0].ucDataArea,4);
		
		memset( ucRecvContent,0,3 );
		ucRecvContentLen=GetRecvContent07(ucRecvContent,3,pucRecvBuffer,usRecvLen);
        Fill_DataList_Byte3(ucRecvContent, ucRecvContentLen,1);

		(*pucRecvNum)=1;//结果是1项内容
	}

	return ucRet;
}

unsigned char RS485_RecvGet07_BATWORKTIME_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum)
{//电池工作时间解释
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucRecvContent[4];
	unsigned char ucRecvContentLen;
//	unsigned char m=0,i=0,ucPos=0;

	if(ucRet==RET_SUCCESS)
	{
		//电池工作时间
		SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,
							pstRS485SendTask->ucReportType,CON_DI_TYPE_MPBATWORKTIME,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[0].ucDataLen=4;
		FillData_EE(stRS485TaskRecvExplain[0].ucDataArea,4);
		
		memset( ucRecvContent,0,4 );
		ucRecvContentLen=GetRecvContent07(ucRecvContent,4,pucRecvBuffer,usRecvLen);
        Fill_DataList_Byte4(ucRecvContent, ucRecvContentLen,1);

		(*pucRecvNum)=1;//结果是1项内容
	}

	return ucRet;
}

unsigned char RS485_RecvGet07_JIAOSHITIME_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum)
{//最近一次校时时间解释
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucRecvContent[16];
	unsigned char ucRecvContentLen;	

	if( usRecvLen<22 ) 
		ucRet=RET_ERROR;

	if( ucRet==RET_SUCCESS )
	{//最近一次校时时间
		SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,
							pstRS485SendTask->ucReportType,CON_DI_TYPE_MPJIAOSHITIME,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[0].ucDataLen=6;
		FillData_AllEE(stRS485TaskRecvExplain[0].ucDataArea,6);

		memset( ucRecvContent,0,16 );
		ucRecvContentLen=GetRecvContent07(ucRecvContent,16,pucRecvBuffer,usRecvLen);
		if( ucRecvContentLen >= 6)
			Fill_DataList_DateTime07((unsigned char *)(ucRecvContent+4),ucRecvContentLen,1);
		(*pucRecvNum)=1;
	}
	return ucRet;
}

unsigned char RS485_RecvGet07_DateAndWeek_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucRecvContent[4];
	unsigned char ucRecvContentLen;
	STDATETIME	  stDateTime;
	ClearDateTime(&stDateTime);

	if( usRecvLen<20 ) 
		ucRet=RET_ERROR;

	if( ucRet==RET_SUCCESS )
	{//日期及周次
		SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,
			pstRS485SendTask->ucReportType,CON_DI_TYPE_MPDATEANDWEEK,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[0].ucDataLen=6;
		FillData_AllEE(stRS485TaskRecvExplain[0].ucDataArea,6);

		memset( ucRecvContent,0,4 );
		ucRecvContentLen=GetRecvContent07(ucRecvContent,4,pucRecvBuffer,usRecvLen);

		if( ucRecvContentLen >= 4)
		{//周就不考虑了
			stDateTime.ucYear	= (unsigned char)BCD_TO_HEX_1(&ucRecvContent[3]);
			stDateTime.ucMonth	= (unsigned char)BCD_TO_HEX_1(&ucRecvContent[2]);
			stDateTime.ucDay	= (unsigned char)BCD_TO_HEX_1(&ucRecvContent[1]);
			stDateTime.ucHour	= 0x00;
			stDateTime.ucMinute = 0x00;
			stDateTime.ucSecond = 0x00;
			for(i=0;i<6;i++)
				(stRS485TaskRecvExplain[0].ucDataArea)[i]=((unsigned char *)(&stDateTime))[i];
		}

		(*pucRecvNum)=1;
	}
	return ucRet;
}

unsigned char RS485_RecvGet07_Time_Explain(STRS485TASK *pstRS485SendTask,unsigned char * pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucRecvContent[3];
	unsigned char ucRecvContentLen;
	STDATETIME	  stDateTime;
	ClearDateTime(&stDateTime);

	if( usRecvLen<19 ) 
		ucRet=RET_ERROR;

	if( ucRet==RET_SUCCESS )
	{
		SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,
			pstRS485SendTask->ucReportType,CON_DI_TYPE_MPTIME,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[0].ucDataLen=6;
		FillData_AllEE(stRS485TaskRecvExplain[0].ucDataArea,6);

		memset( ucRecvContent,0,3 );
		ucRecvContentLen=GetRecvContent07(ucRecvContent,3,pucRecvBuffer,usRecvLen);
		if( ucRecvContentLen >= 3)
		{
			stDateTime.ucYear	= 0x09;
			stDateTime.ucMonth	= 0x01;
			stDateTime.ucDay	= 0x01;
			stDateTime.ucHour	= (unsigned char)BCD_TO_HEX_1(&ucRecvContent[2]);
			stDateTime.ucMinute = (unsigned char)BCD_TO_HEX_1(&ucRecvContent[1]);
			stDateTime.ucSecond = (unsigned char)BCD_TO_HEX_1(&ucRecvContent[0]);
			for(i=0;i<6;i++)
				(stRS485TaskRecvExplain[0].ucDataArea)[i]=((unsigned char *)(&stDateTime))[i];
		}
		(*pucRecvNum)=1;
	}
	return ucRet;
}
/*
unsigned char RS485_RecvGet07_DateTime_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum)
{//日期及时间
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucRecvContent[7];
	unsigned char ucRecvContentLen;
	STDATETIME	  stDateTime;
	ClearDateTime(&stDateTime);

	if( usRecvLen<20 )
		ucRet=RET_ERROR;

	if(ucRet==RET_SUCCESS)
	{	//日期及周次
		SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,
			pstRS485SendTask->ucReportType,CON_DI_TYPE_MPDATEANDWEEK,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[0].ucDataLen=6;
		FillData_AllEE(stRS485TaskRecvExplain[0].ucDataArea,6);
		//时间
		SetRS485TaskPer(&(stRS485TaskRecvExplain[1]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,
			pstRS485SendTask->ucReportType,CON_DI_TYPE_MPTIME,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[1].ucDataLen=6;
		FillData_AllEE(stRS485TaskRecvExplain[1].ucDataArea,6);

		memset( ucRecvContent,0,7 );
		ucRecvContentLen=GetRecvContent(ucRecvContent,7,pucRecvBuffer,usRecvLen);
		if( ucRecvContentLen >= 7)
		{
			stDateTime.ucYear	= (unsigned char)BCD_TO_HEX_1(&ucRecvContent[3]);
			stDateTime.ucMonth	= (unsigned char)BCD_TO_HEX_1(&ucRecvContent[2]);
			stDateTime.ucDay	= (unsigned char)BCD_TO_HEX_1(&ucRecvContent[1]);
			stDateTime.ucHour	= 0x00;
			stDateTime.ucMinute = 0x00;
			stDateTime.ucSecond = 0x00;
			for(i=0;i<6;i++)
				(stRS485TaskRecvExplain[0].ucDataArea)[i]=((unsigned char *)(&stDateTime))[i];

			stDateTime.ucYear	= 0x05;
			stDateTime.ucMonth	= 0x01;
			stDateTime.ucDay	= 0x01;
			stDateTime.ucHour	= (unsigned char)BCD_TO_HEX_1(&ucRecvContent[6]);
			stDateTime.ucMinute = (unsigned char)BCD_TO_HEX_1(&ucRecvContent[5]);
			stDateTime.ucSecond = (unsigned char)BCD_TO_HEX_1(&ucRecvContent[4]);
			for(i=0;i<6;i++)
				(stRS485TaskRecvExplain[1].ucDataArea)[i]=((unsigned char *)(&stDateTime))[i];
		}
		
		(*pucRecvNum)=2;//结果是2项内容
	}
	return ucRet;
}
*/
unsigned char RS485_RecvGet07_MaxDemTim_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI)
{//最大需量及发生时间解释
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucRecvContent[40];
	unsigned char ucRecvContentLen;

	for(i=0;i<5;i++)
	{
	  SetRS485TaskPer(&(stRS485TaskRecvExplain[2*i]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
		  pstRS485SendTask->ucReportType,(unsigned short)((usDI&0xFFF0)+i),pstRS485SendTask->ucDevNo,NULL);
	  stRS485TaskRecvExplain[2*i].ucDataLen=4;
	  FillData_EE(stRS485TaskRecvExplain[2*i].ucDataArea,4);

	  SetRS485TaskPer(&(stRS485TaskRecvExplain[2*i+1]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
		  pstRS485SendTask->ucReportType,(unsigned short)((usDI+0x1000)&0xFFF0+i),pstRS485SendTask->ucDevNo,NULL);
	  stRS485TaskRecvExplain[2*i+1].ucDataLen=6;
	  FillData_AllEE(stRS485TaskRecvExplain[2*i+1].ucDataArea,6);
	}
	

	memset( ucRecvContent,0,40 );
	ucRecvContentLen=GetRecvContent07(ucRecvContent,40,pucRecvBuffer,usRecvLen);
	Fill_DataList_MAXDEMTIIM(ucRecvContent,ucRecvContentLen,5);

	(*pucRecvNum)=10;

	return ucRet;
}


unsigned char RS485_RecvGet07_FeeRate_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum)
{//费率时段
	unsigned char ucRet=RET_SUCCESS,ucPos=0,m=0;
	unsigned char ucRecvContent[42];
	unsigned char ucRecvContentLen;

	if(ucRet==RET_SUCCESS)
	{
		SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
							pstRS485SendTask->ucReportType,CON_DI_TYPE_MPFEERATE_BLOCK,pstRS485SendTask->ucDevNo,NULL);

		memset( ucRecvContent,0,42);
		ucRecvContentLen=GetRecvContent07(ucRecvContent,42,pucRecvBuffer,usRecvLen);

		while( (ucPos+3)<=ucRecvContentLen)
		{
			if( ucRecvContent[ucPos]!=0xBB && ucRecvContent[ucPos+1]!=0xBB && ucRecvContent[ucPos+2]!=0xBB )
			{
				stRS485TaskRecvExplain[0].ucDataArea[ucPos+1]=(unsigned char)BCD_TO_HEX_1(&ucRecvContent[ucPos]);
				ucPos++;
				stRS485TaskRecvExplain[0].ucDataArea[ucPos+1]=(unsigned char)BCD_TO_HEX_1(&ucRecvContent[ucPos]);
				ucPos++;
				stRS485TaskRecvExplain[0].ucDataArea[ucPos+1]=(unsigned char)BCD_TO_HEX_1(&ucRecvContent[ucPos]);
				ucPos++;
				m++;
			}else
				break;
		}

		stRS485TaskRecvExplain[0].ucDataArea[0] = m;
		stRS485TaskRecvExplain[0].ucDataLen=ucPos;

		(*pucRecvNum)=1;
	}
	return ucRet;
}
unsigned char RS485_RecvGet07_MeterConst_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucRecvContent[3];
	unsigned char ucRecvContentLen;

	if( usRecvLen<19 ) 
		ucRet=RET_ERROR;

	if( ucRet==RET_SUCCESS )
	{
		SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
							pstRS485SendTask->ucReportType,usDI,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[0].ucDataLen=4;
		FillData_EE( stRS485TaskRecvExplain[0].ucDataArea,4 );

		memset( ucRecvContent,0,3);
		ucRecvContentLen=GetRecvContent07(ucRecvContent,3,pucRecvBuffer,usRecvLen);
		Fill_DataList_Byte3(ucRecvContent,ucRecvContentLen,1);

		(*pucRecvNum)=1;
	}
	return ucRet;
}
/*
unsigned char RS485_RecvGet_MeterConstBlock_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum)
{//电表常数
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucRecvContent[24];
	unsigned char ucRecvContentLen;

	if( usRecvLen<19 ) 
		ucRet=RET_ERROR;
	
	if(ucRet==RET_SUCCESS)
	{
		SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
							pstRS485SendTask->ucReportType,CON_DI_TYPE_MPPOWCONST,pstRS485SendTask->ucDevNo,NULL);//有功
		stRS485TaskRecvExplain[0].ucDataLen=4;
		FillData_EE( stRS485TaskRecvExplain[0].ucDataArea,4 );

		SetRS485TaskPer(&(stRS485TaskRecvExplain[1]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
							pstRS485SendTask->ucReportType,CON_DI_TYPE_MPVARCONST,pstRS485SendTask->ucDevNo,NULL);//无功
		stRS485TaskRecvExplain[1].ucDataLen=4;
		FillData_EE( stRS485TaskRecvExplain[1].ucDataArea,4 );

		memset( ucRecvContent,0,24);
		ucRecvContentLen=GetRecvContent(ucRecvContent,24,pucRecvBuffer,usRecvLen);
		Fill_DataList_Byte3(ucRecvContent,ucRecvContentLen,2);

		(*pucRecvNum)=2;
	}
	return ucRet;
}
*/
/****************************************************************************/


//changed by cfh 10-01-16
unsigned char RS485_RecvGet07_LostVolNumTime_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI)
{//断相次数时间
	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucTmp[12];
	unsigned char ucRecvContent[18],ucRecvContentLen;
 //   for(i=0;i<4;i++)
 //   {
   SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
	   pstRS485SendTask->ucReportType,usDI,pstRS485SendTask->ucDevNo,NULL);
   stRS485TaskRecvExplain[0].ucDataLen=4;
   FillData_EE(stRS485TaskRecvExplain[0].ucDataArea,4);
 //   }	


	memset( ucRecvContent,0,3 );
//	memset( ucTmp,0,12);
	ucRecvContentLen=GetRecvContent07(ucRecvContent,3,pucRecvBuffer,usRecvLen);
//	ucRecvContentLen /=2 ;
//	if((usDI & 0x00f0) == 0x0010)	//要读的是断相总次数
//	{
//		memcpy(&ucTmp[3],ucRecvContent,3);
//		memcpy(&ucTmp[6],&ucRecvContent[6],3);
//		memcpy(&ucTmp[9],&ucRecvContent[12],3);
//	}
//	else							//读断相总累计时间
//	{
//		memcpy(&ucTmp[3],&ucRecvContent[3],3);
//		memcpy(&ucTmp[6],&ucRecvContent[9],3);
//		memcpy(&ucTmp[9],&ucRecvContent[15],3);
//	}
	Fill_DataList_Byte3(ucRecvContent,ucRecvContentLen,1);

	(*pucRecvNum)=1;
	return ucRet;
}

//changed by cfh 10-01-16
unsigned char RS485_RecvGet07_LastVolOffBegEnd_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI)
{//最近一次断相起始时间
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucRecvContent[12];
	unsigned char ucRecvContentLen;

//	for(i=0;i<2;i++)
//	{
		SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
			pstRS485SendTask->ucReportType,usDI,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[0].ucDataLen=6;
		FillData_AllEE(stRS485TaskRecvExplain[0].ucDataArea,6);
//	}
//	SetRS485TaskPer(&(stRS485TaskRecvExplain[1]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
//		pstRS485SendTask->ucReportType,(unsigned short)(usDI+0x10),pstRS485SendTask->ucDevNo,NULL);
//	stRS485TaskRecvExplain[1].ucDataLen=6;
//	FillData_AllEE(stRS485TaskRecvExplain[1].ucDataArea,6);


	memset( ucRecvContent,0,6 );
	ucRecvContentLen=GetRecvContent07(ucRecvContent,6,pucRecvBuffer,usRecvLen);
//	if((usDI & 0x00f0) == 0x0030)	//读断相开始时间
//	{
//		Fill_DataList_DateTime07(ucRecvContent,ucRecvContentLen,1);
//	}
//	else
//	{
	Fill_DataList_DateTime07(ucRecvContent,ucRecvContentLen,1);
//	}

	(*pucRecvNum)=1;
	return ucRet;
}
unsigned char RS485_RecvGet07_CuFe_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucRecvContent[24];
	unsigned char ucRecvContentLen,i=0;

	for(i=0;i<12;i++)
	{
		SetRS485TaskPer(&(stRS485TaskRecvExplain[i]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
			pstRS485SendTask->ucReportType,(unsigned short)(pstRS485SendTask->usDataType&0xFFF0+i),pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[i].ucDataLen=6;
		FillData_AllEE(stRS485TaskRecvExplain[i].ucDataArea,6);
	}
	memset( ucRecvContent,0,24 );
	ucRecvContentLen=GetRecvContent07(ucRecvContent,24,pucRecvBuffer,usRecvLen);
	Fill_DataList_Byte2(ucRecvContent,ucRecvContentLen,12);

	(*pucRecvNum)=12;
	return ucRet;
}

//add by cfh 10-01-18 分相电量解析
unsigned char RS485_RecvGet07_PowVarABC_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI)
{//最近一次断相起始时间
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucRecvContent[4];
	unsigned char ucRecvContentLen;

	SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
		pstRS485SendTask->ucReportType,usDI,pstRS485SendTask->ucDevNo,NULL);
	stRS485TaskRecvExplain[0].ucDataLen=4;
	FillData_AllEE(stRS485TaskRecvExplain[0].ucDataArea,4);

	memset( ucRecvContent,0,4 );
	ucRecvContentLen=GetRecvContent07(ucRecvContent,4,pucRecvBuffer,usRecvLen);
	Fill_PowerDataList(ucRecvContent,ucRecvContentLen);

	(*pucRecvNum)=1;
	return ucRet;
}


//填写电量数据
unsigned char Fill_PowerDataList(unsigned char *pucRecvContent,unsigned char ucRecvContentLen)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucPos=0,i=0,j=0;
	unsigned long ulVal=0;
	for(j=0;j<5;j++)									//5=总+4个费率
	{
		if((ucPos+4) > ucRecvContentLen)
			break;
		if( pucRecvContent[ucPos]!=0xBB )
		{
			ulVal	=  BCD_TO_HEX_4(pucRecvContent+ucPos);
			for(i=0;i<4;i++)
				(stRS485TaskRecvExplain[j].ucDataArea)[i]=((unsigned char *)(&ulVal))[i];
			ucPos+=4;
		}else
			ucPos++;
	}
	return ucRet;
}
/*
unsigned char Fill_DataList07_Byte2(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char ucContentNum)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucPos=0,i=0,j=0;
	unsigned long ulVal=0;
	for(j=0;j<ucContentNum;j++)	
	{
		if((ucPos+2) > ucRecvContentLen)
			break;
		memset(stRS485TaskRecvExplain[j].ucDataArea,0,CONRS485DATAMAXLEN);
		if( pucRecvContent[ucPos]!=0xBB )
		{
			ulVal	=  BCD_TO_HEX_2(pucRecvContent+ucPos);
			for(i=0;i<4;i++)
				(stRS485TaskRecvExplain[j].ucDataArea)[i]=((unsigned char *)(&ulVal))[i];
			ucPos+=2;
		}else
			ucPos++;
	}
	return ucRet;
}
*/


unsigned char Fill_DataList_MAXDEMTIIM(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char ucContentNum)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucPos=0,i=0,j=0;
	unsigned long ulVal=0;
    STDATETIME	stDateTime;
	ClearDateTime(&stDateTime);
	
	for(j=0;j<ucContentNum;j++)	
	{ 
		if((ucPos+8) > ucRecvContentLen)
			break;
		if( pucRecvContent[ucPos]!=0xBB )
		{
			memset(stRS485TaskRecvExplain[2*j].ucDataArea,0,CONRS485DATAMAXLEN);
			ulVal	=  BCD_TO_HEX_3(pucRecvContent+ucPos);
			for(i=0;i<4;i++)
				(stRS485TaskRecvExplain[2*j].ucDataArea)[i]=((unsigned char *)(&ulVal))[i];
			ucPos+=3;

			stDateTime.ucYear	=(unsigned char)BCD_TO_HEX_1(pucRecvContent+ucPos+4);
			stDateTime.ucMonth	=(unsigned char)BCD_TO_HEX_1(pucRecvContent+ucPos+3);
			stDateTime.ucDay	=(unsigned char)BCD_TO_HEX_1(pucRecvContent+ucPos+2);
			stDateTime.ucHour	=(unsigned char)BCD_TO_HEX_1(pucRecvContent+ucPos+1);
			stDateTime.ucMinute	=(unsigned char)BCD_TO_HEX_1(pucRecvContent+ucPos);
			stDateTime.ucSecond	=0x00;

			for(i=0;i<6;i++)
				(stRS485TaskRecvExplain[2*j+1].ucDataArea)[i]=((unsigned char *)(&stDateTime))[i];
			ucPos+=5;
		}else
			ucPos++;
	}
	return ucRet;
}
//unsigned char Fill_DataList_Byte6(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char ucContentNum)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	/*
//	unsigned char ucPos=0,i=0,j=0;
//	unsigned long ulVal=0,ulVal2=0,ulVal3=0;
//	for(j=1;j<ucContentNum;j++)	
//	{
//		if((ucPos+6) > ucRecvContentLen)
//			break;
//		memset(stRS485TaskRecvExplain[2*j].ucDataArea,0,CONRS485DATAMAXLEN);
//		memset(stRS485TaskRecvExplain[2*j+1].ucDataArea,0,CONRS485DATAMAXLEN);
//		if( pucRecvContent[ucPos]!=0xBB )
//		{
//            if((pucRecvContent[ucPos+2]&0xFF) != 0)
//            {
//               pucRecvContent[ucPos]=0x99;
//			   pucRecvContent[ucPos+1]=0x99;
//            }
//			ulVal	=  BCD_TO_HEX_2(pucRecvContent+ucPos);
//			for(i=0;i<4;i++)
//				(stRS485TaskRecvExplain[2*j].ucDataArea)[i]=((unsigned char *)(&ulVal))[i];
//			ucPos+=3;
//            ulVal	=  BCD_TO_HEX_3(pucRecvContent+ucPos);
//			for(i=0;i<4;i++)
//				(stRS485TaskRecvExplain[2*j+1].ucDataArea)[i]=((unsigned char *)(&ulVal))[i];
//			ucPos+=3;
//		}else
//			ucPos++;
//	}
//	memset(stRS485TaskRecvExplain[0].ucDataArea,0,CONRS485DATAMAXLEN);
//	memset(stRS485TaskRecvExplain[1].ucDataArea,0,CONRS485DATAMAXLEN);
//    
//	for(i=1;i<ucContentNum;i++)	
//    {
//	    ulVal2+=((stRS485TaskRecvExplain[2*i].ucDataArea)[0]+(stRS485TaskRecvExplain[2*i].ucDataArea)[1]*0x100);
//	    ulVal3+=((stRS485TaskRecvExplain[2*i+1].ucDataArea)[0]+(stRS485TaskRecvExplain[2*i+1].ucDataArea)[1]*0x100+(stRS485TaskRecvExplain[2*i+1].ucDataArea)[2]*0x10000);
//    }
//    if(ulVal2>=0x270F)
//		ulVal2=0x270F;
//	if(ulVal3>=0x0F423F)
//	    ulVal3==0x0F423F;
//	for(i=0;i<4;i++)
//	{
//		(stRS485TaskRecvExplain[0].ucDataArea)[i]=((unsigned char *)(&ulVal2))[i];
//	    (stRS485TaskRecvExplain[1].ucDataArea)[i]=((unsigned char *)(&ulVal3))[i];
//	}
//	*/
//	return ucRet;
//	
//}

unsigned char Fill_DataList_Byte4(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char ucContentNum)
{
	
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucPos=0,i=0,j=0;
	unsigned long ulVal=0;
	for(j=0;j<ucContentNum;j++)	
	{ 
		if((ucPos+4) > ucRecvContentLen)
			break;
		if( pucRecvContent[ucPos]!=0xBB )
		{
			memset(stRS485TaskRecvExplain[j].ucDataArea,0,CONRS485DATAMAXLEN);
			ulVal	=  BCD_TO_HEX_4(pucRecvContent+ucPos);
			for(i=0;i<4;i++)
				(stRS485TaskRecvExplain[j].ucDataArea)[i]=((unsigned char *)(&ulVal))[i];
			ucPos+=4;
		}else
			ucPos++;
	}
	return ucRet;
}


unsigned char Fill_DataList_DateTime07(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char ucContentNum)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucPos=0,i=0,j=0;
	STDATETIME	stDateTime;
	ClearDateTime(&stDateTime);
	for(j=0;j<ucContentNum;j++)	
	{
		if((ucPos+6) > ucRecvContentLen)
			break;
		if( pucRecvContent[ucPos]!=0xBB )
		{
			stDateTime.ucYear	=(unsigned char)BCD_TO_HEX_1(pucRecvContent+ucPos+5);
			stDateTime.ucMonth	=(unsigned char)BCD_TO_HEX_1(pucRecvContent+ucPos+4);
			stDateTime.ucDay	=(unsigned char)BCD_TO_HEX_1(pucRecvContent+ucPos+3);
			stDateTime.ucHour	=(unsigned char)BCD_TO_HEX_1(pucRecvContent+ucPos+2);
			stDateTime.ucMinute	=(unsigned char)BCD_TO_HEX_1(pucRecvContent+ucPos+1);
			stDateTime.ucSecond	=(unsigned char)BCD_TO_HEX_1(pucRecvContent+ucPos);

			for(i=0;i<6;i++)
				(stRS485TaskRecvExplain[j].ucDataArea)[i]=((unsigned char *)(&stDateTime))[i];
			ucPos+=6;
		}else
			ucPos++;
	}
	return ucRet;
}


unsigned char GetRecvContent07(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char *pucRecvBuffer,unsigned short usRecvLen)
{
	unsigned char i=0;

	for( i=0;i<ucRecvContentLen;i++ )
	{
		if( (i+14) >= (usRecvLen-2))
			break;
		else
			pucRecvContent[i]=pucRecvBuffer[i+14]-0x33;
	}

	return i;
}




unsigned char GetDI64507( unsigned long *usDestDI,unsigned short usSourDI)
{
	unsigned char ucRet=RET_SUCCESS;
	
	switch( usSourDI)
	{
	case CON_DI_TYPE_MPDIRPOW_BLOCK:                    //正向有功电量
		*usDestDI=CON_DI64507_MPDIRPOW_BLOCK;
		break;	
	case CON_DI_TYPE_MPUNDIRPOW_BLOCK:                  //反向有功电量
		*usDestDI=CON_DI64507_MPUNDIRPOW_BLOCK;
		break;
	case  CON_DI_TYPE_MPDIRPOW_LM_BLOCK:				//上月正向有功电量
		*usDestDI=CON_DI64507_MPDIRPOW_LM_BLOCK;
		break;
	case  CON_DI_TYPE_MPUNDIRPOW_LM_BLOCK:				//上月反向有功电量
		*usDestDI=CON_DI64507_MPUNDIRPOW_LM_BLOCK;
		break;
	
    case CON_DI_TYPE_MPDIRVAR_BLOCK:                      //正向无功
		*usDestDI=CON_DI64507_MPDIRVAR_BLOCK;
		break;
    case CON_DI_TYPE_MPUNDIRVAR_BLOCK:                      //反向无功
		*usDestDI=CON_DI64507_MPUNDIRVAR_BLOCK;
		break;
		
   	case CON_DI_TYPE_MPVAR1_BLOCK:                      //一象限无功
		*usDestDI=CON_DI64507_MPVAR1_BLOCK;
		break;
    case CON_DI_TYPE_MPVAR2_BLOCK:                      //二象限无功
		*usDestDI=CON_DI64507_MPVAR2_BLOCK;
		break;
    case CON_DI_TYPE_MPVAR3_BLOCK:                      //三象限无功
		*usDestDI=CON_DI64507_MPVAR3_BLOCK;
		break;
	case CON_DI_TYPE_MPVAR4_BLOCK:                      //四象限无功
		*usDestDI=CON_DI64507_MPVAR4_BLOCK;
		break;
	case CON_DI_TYPE_MPDIRVAR_LM_BLOCK:                      //上月正向无功
		*usDestDI=CON_DI64507_MPDIRVAR_LM_BLOCK;
		break;
    case CON_DI_TYPE_MPUNDIRVAR_LM_BLOCK:                      //上月反向无功
		*usDestDI=CON_DI64507_MPUNDIRVAR_LM_BLOCK;
		break;
	case CON_DI_TYPE_MPVAR1_LM_BLOCK:                   //上月一象限无功
		*usDestDI=CON_DI64507_MPVAR1_LM_BLOCK;
		break;
    case CON_DI_TYPE_MPVAR2_LM_BLOCK:                   //上月二象限无功
		*usDestDI=CON_DI64507_MPVAR2_LM_BLOCK;
		break;
	case CON_DI_TYPE_MPVAR3_LM_BLOCK:                   //上月三象限无功
		*usDestDI=CON_DI64507_MPVAR3_LM_BLOCK;
		break;
	case CON_DI_TYPE_MPVAR4_LM_BLOCK:                   //上月四象限无功
		*usDestDI=CON_DI64507_MPVAR4_LM_BLOCK;
		break;
		/////////////////////////////////////////////////////////////////
	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_BLOCK:           //正向需量及时间
	//case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_BLOCK:
		*usDestDI=CON_DI64507_MPDIRPOWMAXDEMTIM_BLOCK;
		break;
	case  CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_BLOCK:	    //反向需量及时间
	//case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_BLOCK:		
		*usDestDI=CON_DI64507_MPUNDIRPOWMAXDEMTIM_BLOCK;
		break;
	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_BLOCK:        //上月正向需量及时间
	//case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_BLOCK:
		*usDestDI=CON_DI64507_MPDIRPOWMAXDEMTIM_LM_BLOCK;
		break;
	case  CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_BLOCK:	    //上月反向需量及时间
	//case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_BLOCK:		
		*usDestDI=CON_DI64507_MPUNDIRPOWMAXDEMTIM_LM_BLOCK;
		break;
	case CON_DI_TYPE_MPDIRVARMAXDEMAND_BLOCK:			//正向无功最大需量及时间
	    *usDestDI=CON_DI64507_MPDIRVARMAXDEMTIM_BLOCK;
		break;
	case CON_DI_TYPE_MPUNDIRVARMAXDEMAND_BLOCK:			//反向无功最大需量及时间
	    *usDestDI=CON_DI64507_MPUNDIRVARMAXDEMTIM_BLOCK;
		break;
	case CON_DI_TYPE_MPDIRVARMAXDEMAND_LM_BLOCK:		//正向无功最大需量（上月）
	    *usDestDI=CON_DI64507_MPDIRVARMAXDEMTIM_LM_BLOCK;
		break;
	case CON_DI_TYPE_MPUNDIRVARMAXDEMAND_LM_BLOCK:			//反向无功最大需量及时间（上月）
	    *usDestDI=CON_DI64507_MPUNDIRVARMAXDEMTIM_LM_BLOCK;
		break;
		/////////////////////////////////////////////////////////////////
	case CON_DI_TYPE_MPVOLA:							//A相电压
	    *usDestDI=CON_DI64507_MPVOLA;
		break;
	case CON_DI_TYPE_MPVOLB:							//B相电压
	    *usDestDI=CON_DI64507_MPVOLB;
		break;
	case CON_DI_TYPE_MPVOLC:							//C相电压
	    *usDestDI=CON_DI64507_MPVOLC;
		break;
	case CON_DI_TYPE_MPVOL_BLOCK:						//电压数据块
	    *usDestDI=CON_DI64507_MPVOL_BLOCK;
		break;
	case CON_DI_TYPE_MPCURA:							//A相电流
	    *usDestDI=CON_DI64507_MPCURA;
		break;
	case CON_DI_TYPE_MPCURB:							//B相电流
	    *usDestDI=CON_DI64507_MPCURB;
		break;
	case CON_DI_TYPE_MPCURC:							//C相电流 
	    *usDestDI=CON_DI64507_MPCURC;
		break;
	case CON_DI_TYPE_MPCUR_BLOCK:						//电流数据块
	    *usDestDI=CON_DI64507_MPCUR_BLOCK;
		break;
	case CON_DI_TYPE_MPINSTANTPOW:						//瞬时有功功率
	    *usDestDI=CON_DI64507_MPINSTANTPOW;
		break;	
	case CON_DI_TYPE_MPINSTANTPOWA:						//A相有功功率
	    *usDestDI=CON_DI64507_MPINSTANTPOWA;
		break;	
	case CON_DI_TYPE_MPINSTANTPOWB:						//B相有功功率
	    *usDestDI=CON_DI64507_MPINSTANTPOWB;
		break;
	case CON_DI_TYPE_MPINSTANTPOWC:						//C相有功功率
	    *usDestDI=CON_DI64507_MPINSTANTPOWC;
		break;
	case CON_DI_TYPE_MPINSTANTPOW_BLOCK:				//
	    *usDestDI=CON_DI64507_MPINSTANTPOW_BLOCK;
		break;
	case CON_DI_TYPE_MPINSTANTVAR:						//瞬时无功功率
	    *usDestDI=CON_DI64507_MPINSTANTVAR;
		break;
	case CON_DI_TYPE_MPINSTANTVARA:						//A相无功功率
	    *usDestDI=CON_DI64507_MPINSTANTVARA;
		break;
	case CON_DI_TYPE_MPINSTANTVARB:						//B相无功功率
	    *usDestDI=CON_DI64507_MPINSTANTVARB;
		break;
	case CON_DI_TYPE_MPINSTANTVARC:						//C相无功功率
	    *usDestDI=CON_DI64507_MPINSTANTVARC;
		break;
	case CON_DI_TYPE_MPINSTANTVAR_BLOCK:				//	
		*usDestDI=CON_DI64507_MPINSTANTVAR_BLOCK;
		break;
	case CON_DI_TYPE_MPCUFE:
		*usDestDI = CON_DI64507_MPCUFE_BLOCK;
		/////////////////////////////////////////////////////////////////
/*
    case CON_DI_TYPE_MPPOWERFACTOR:                   //总功率因数
		*usDestDI=CON_DI64507_MPPOWERFACTOR;
		break;
    case CON_DI_TYPE_MPPOWERFACTORA:                   //功率因数A
		*usDestDI=CON_DI64507_MPPOWERFACTORA;
		break;
    case CON_DI_TYPE_MPPOWERFACTORB:                   //功率因数B
		*usDestDI=CON_DI64507_MPPOWERFACTORB;
		break;
    case CON_DI_TYPE_MPPOWERFACTORC:                   //功率因数C
		*usDestDI=CON_DI64507_MPPOWERFACTORC;
		break;
    case CON_DI_TYPE_MPPOWERFACTOR_BLOCK:                   //功率因数块
		*usDestDI=CON_DI64507_MPPOWERFACTOR_BLOCK;
		break;
*/
        ///////////////////////////////////////////////////////////////

	case CON_DI_TYPE_MPPROGRAMTIME:             //最近一次编程时间
        *usDestDI=CON_DI64507_MPLATELYPROGTIME;
		break;
	case CON_DI_TYPE_MPDEMODECLRTIME:             //最近一次最大需量清零时间
        *usDestDI=CON_DI64507_MPLATELYMAXDEMANDCLEARTIME;
		break;	
    case CON_DI_TYPE_MPPROGRAMNUM:             //编程次数
        *usDestDI=CON_DI64507_MPPROGTIMETIMES;
		break;
    case CON_DI_TYPE_MPDEMODECLRNUM:				//最大需量清零次数
        *usDestDI=CON_DI64507_MPMAXDEMANDCLEARTIMES;
		break;
	case CON_DI_TYPE_MPB21F_BLOCK:                   //电池时间
		*usDestDI=CON_DI64507_MPBATWORKTIME;
		break;
	case CON_DI_TYPE_MPMETERCLRNUM:                 //电表清0总次数
        *usDestDI=CON_DI64507_MPMETERCLRNUM;
		break;
	case CON_DI_TYPE_MPMETERCLRTIME:               //最近一次电表清0时间
        *usDestDI=CON_DI64507_MPMETERCLRTIME;
		break;
	case CON_DI_TYPE_MPEVENTCLRNUM:               //事件清0总次数
        *usDestDI=CON_DI64507_MPEVENTCLRNUM;
		break;
	case CON_DI_TYPE_MPEVENTCLRTIME:             //最近一次事件清0时间
        *usDestDI=CON_DI64507_MPEVENTCLRTIME;
		break;
	case CON_DI_TYPE_MPJIAOSHINUM:               //校时总次数
        *usDestDI=CON_DI64507_MPJIAOSHINUM;
		break;
	case CON_DI_TYPE_MPJIAOSHITIME:             //最近一次校时时间
        *usDestDI=CON_DI64507_MPJIAOSHITIME;
		break;		
		////////////////////////////////////////////////////////////////////
  	case CON_DI_TYPE_MPPOWCONST:					//有功常数
   		*usDestDI=CON_DI64507_MPPOWCONST;
		break;
    case CON_DI_TYPE_MPVARCONST:					//无功常数	
		*usDestDI=CON_DI64507_MPVARCONST;
		break;
	case CON_DI_TYPE_MPFEERATE_BLOCK:					//费率时段	
		*usDestDI=CON_DI64507_MPFEERATE_BLOCK;
		break;
	case CON_DI_TYPE_MPDATEANDWEEK:						//日期及周次
	    *usDestDI=CON_DI64507_MPDATEANDWEEK;
		break;
	case CON_DI_TYPE_MPTIME:							//时间
	    *usDestDI=CON_DI64507_MPTIME;
		break;
	//case CON_DI_TYPE_MPDATETIME_BLOCK:					//
	case CON_DI_TYPE_MPCOPYDAYTIME:						//抄表日
	    *usDestDI=CON_DI64507_MPCOPYDAYTIME;
		break;
    //case CON_DI_TYPE_MPB31F_BLOCK://断相次数
    case CON_DI_TYPE_MPLOSTVOLANUM:
	case CON_DI_TYPE_MPLOSTVOLNUM:
		*usDestDI=CON_DI64507_MPBREAKNUMA;
		break;
	case CON_DI_TYPE_MPLOSTVOLBNUM:
		*usDestDI=CON_DI64507_MPBREAKNUMB;
		break;
	case CON_DI_TYPE_MPLOSTVOLCNUM:
		*usDestDI=CON_DI64507_MPBREAKNUMC;
		break;
	//case CON_DI_TYPE_MPBREAKTIME_BLOCK://断相时间
	//    *usDestDI=CON_DI64507_MPBREAKNOTIM_BLOCK;
	//	break;
	case CON_DI_TYPE_MPLOSTVOLTIME:
	case CON_DI_TYPE_MPLOSTVOLATIME:	
		*usDestDI=CON_DI64507_MPBREAKTOTALTIMEA;
		break;
	case CON_DI_TYPE_MPLOSTVOLBTIME:
		*usDestDI=CON_DI64507_MPBREAKTOTALTIMEB;
		break;
	case CON_DI_TYPE_MPLOSTVOLCTIME:
		*usDestDI=CON_DI64507_MPBREAKTOTALTIMEC;
		break;
	
	case CON_DI_TYPE_MPLASTVOLAOFFBEGTIME:
		*usDestDI=CON_DI64507_MPBREAKBEGTIME_A;//最近断相起始时间结束时间A	
		break;	
	case CON_DI_TYPE_MPLASTVOLBOFFBEGTIME:	
		*usDestDI=CON_DI64507_MPBREAKBEGTIME_B;
		break;	
	case CON_DI_TYPE_MPLASTVOLCOFFBEGTIME:
		*usDestDI=CON_DI64507_MPBREAKBEGTIME_C;
		break;
	case CON_DI_TYPE_MPLASTVOLOFFBEGTIME:
		*usDestDI=CON_DI64507_MPBREAKBEGTIME_A;
		break;
	case CON_DI_TYPE_MPLASTVOLAOFFENDTIME:	//结束时间
		*usDestDI=CON_DI64507_MPBREAKENDTIME_A;
		break;
	case CON_DI_TYPE_MPLASTVOLBOFFENDTIME:
		*usDestDI=CON_DI64507_MPBREAKENDTIME_B;
		break;
	case CON_DI_TYPE_MPLASTVOLCOFFENDTIME:
		*usDestDI=CON_DI64507_MPBREAKENDTIME_C;
		break;
	case CON_DI_TYPE_MPLASTVOLOFFENDTIME:
		*usDestDI=CON_DI64507_MPBREAKENDTIME_A;
		break;
	case CON_DI_TYPE_MPCO2F_BLOCK:  //表记电池电压
		*usDestDI=CON_DI64507_MPMETERSTATFLAG_BLOCK;
		break;
		
	case CON_DI_TYPE_MPDIRPOWA:
		*usDestDI = CON_DI64507_MPDIRPOWA;
		break;
	case CON_DI_TYPE_MPUNDIRPOWA:
		*usDestDI = CON_DI64507_MPUNDIRPOWA;
		break;
	case CON_DI_TYPE_MPCOMVAR1A:
		*usDestDI = CON_DI64507_MPCOMVAR1A;
		break;
	case CON_DI_TYPE_MPCOMVAR2A:
		*usDestDI = CON_DI64507_MPCOMVAR2A;
		break;
	case CON_DI_TYPE_MPDIRPOWB:
		*usDestDI = CON_DI64507_MPDIRPOWB;
		break;
	case CON_DI_TYPE_MPUNDIRPOWB:
		*usDestDI = CON_DI64507_MPUNDIRPOWB;
		break;
	case CON_DI_TYPE_MPCOMVAR1B:
		*usDestDI = CON_DI64507_MPCOMVAR1B;
		break;
	case CON_DI_TYPE_MPCOMVAR2B:
		*usDestDI = CON_DI64507_MPCOMVAR2B;
		break;
	case CON_DI_TYPE_MPDIRPOWC:
		*usDestDI = CON_DI64507_MPDIRPOWC;
		break;
	case CON_DI_TYPE_MPUNDIRPOWC:
		*usDestDI = CON_DI64507_MPUNDIRPOWC;
		break;
	case CON_DI_TYPE_MPCOMVAR1C:
		*usDestDI = CON_DI64507_MPCOMVAR1C;
		break;
	case CON_DI_TYPE_MPCOMVAR2C:
		*usDestDI = CON_DI64507_MPCOMVAR2C;
		break;
	case CON_DI_TYPE_MPDIRPOWA_LM:
		*usDestDI = CON_DI64507_MPDIRPOWA_LM;
		break;
	case CON_DI_TYPE_MPUNDIRPOWA_LM:
		*usDestDI = CON_DI64507_MPUNDIRPOWA_LM;
		break;
	case CON_DI_TYPE_MPCOMVAR1A_LM:
		*usDestDI = CON_DI64507_MPCOMVAR1A_LM;
		break;
	case CON_DI_TYPE_MPCOMVAR2A_LM:
		*usDestDI = CON_DI64507_MPCOMVAR2A_LM;
		break;
	case CON_DI_TYPE_MPDIRPOWB_LM:
		*usDestDI = CON_DI64507_MPDIRPOWB_LM;
		break;
	case CON_DI_TYPE_MPUNDIRPOWB_LM:
		*usDestDI = CON_DI64507_MPUNDIRPOWB_LM;
		break;
	case CON_DI_TYPE_MPCOMVAR1B_LM:
		*usDestDI = CON_DI64507_MPCOMVAR1B_LM;
		break;
	case CON_DI_TYPE_MPCOMVAR2B_LM:
		*usDestDI = CON_DI64507_MPCOMVAR2B_LM;
		break;
	case CON_DI_TYPE_MPDIRPOWC_LM:
		*usDestDI = CON_DI64507_MPDIRPOWC_LM;
		break;
	case CON_DI_TYPE_MPUNDIRPOWC_LM:
		*usDestDI = CON_DI64507_MPUNDIRPOWC_LM;
		break;
	case CON_DI_TYPE_MPCOMVAR1C_LM:
		*usDestDI = CON_DI64507_MPCOMVAR1C_LM;
		break;
	case CON_DI_TYPE_MPCOMVAR2C_LM:
		*usDestDI = CON_DI64507_MPCOMVAR2C_LM;
		break;
	default:
		ucRet=RET_ERROR;
		break;
	}

	return ucRet;
}

/*
unsigned short Get_97645DI(unsigned long ulDI)
{
	unsigned short usDI;
	switch(ulDI)
	{
	case CON_DI64507_MPDIRPOW_BLOCK:                    //正向有功电量
		usDI = CON_DI_TYPE_MPDIRPOW_BLOCK;
		break;	
	case CON_DI64507_MPUNDIRPOW_BLOCK:                  //反向有功电量
		usDI = CON_DI_TYPE_MPUNDIRPOW_BLOCK;
		break;
	case  CON_DI64507_MPDIRPOW_LM_BLOCK:				//上月正向有功电量
		usDI = CON_DI_TYPE_MPDIRPOW_LM_BLOCK;
		break;
	case  CON_DI64507_MPUNDIRPOW_LM_BLOCK:				//上月反向有功电量
		usDI = CON_DI_TYPE_MPUNDIRPOW_LM_BLOCK;
		break;

	case CON_DI64507_MPDIRVAR_BLOCK:                      //正向无功
		usDI = CON_DI_TYPE_MPDIRVAR_BLOCK;
		break;
	case CON_DI64507_MPUNDIRVAR_BLOCK:                      //反向无功
		usDI = CON_DI_TYPE_MPUNDIRVAR_BLOCK;
		break;		
	case CON_DI64507_MPVAR1_BLOCK:                      //一象限无功
		usDI = CON_DI_TYPE_MPVAR1_BLOCK;
		break;
	case CON_DI64507_MPVAR2_BLOCK:                      //二象限无功
		usDI = CON_DI_TYPE_MPVAR2_BLOCK;
		break;
	case CON_DI64507_MPVAR3_BLOCK:                      //三象限无功
		usDI = CON_DI_TYPE_MPVAR3_BLOCK;
		break;
	case CON_DI64507_MPVAR4_BLOCK:                      //四象限无功
		usDI = CON_DI_TYPE_MPVAR4_BLOCK;
		break;
	case CON_DI64507_MPDIRVAR_LM_BLOCK:                      //上月正向无功
		usDI = CON_DI_TYPE_MPDIRVAR_LM_BLOCK;
		break;
	case CON_DI64507_MPUNDIRVAR_LM_BLOCK:                      //上月反向无功
		usDI = CON_DI_TYPE_MPUNDIRVAR_LM_BLOCK;
		break;
	case CON_DI64507_MPVAR1_LM_BLOCK:                   //上月一象限无功
		usDI = CON_DI_TYPE_MPVAR1_LM_BLOCK;
		break;
	case CON_DI64507_MPVAR2_LM_BLOCK:                   //上月二象限无功
		usDI = CON_DI_TYPE_MPVAR2_LM_BLOCK;
		break;
	case CON_DI64507_MPVAR3_LM_BLOCK:                   //上月三象限无功
		usDI = CON_DI_TYPE_MPVAR3_LM_BLOCK;
		break;
	case CON_DI64507_MPVAR4_LM_BLOCK:                   //上月四象限无功
		usDI = CON_DI_TYPE_MPVAR4_LM_BLOCK;
		break;
		/////////////////////////////////////////////////////////////////
	case CON_DI64507_MPDIRPOWMAXDEMTIM_BLOCK:           //正向需量及时间
		usDI = CON_DI_TYPE_MPDIRPOWMAXDEMAND_BLOCK;
		break;
	case  CON_DI64507_MPUNDIRPOWMAXDEMTIM_BLOCK:	    //反向需量及时间
		usDI = CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_BLOCK;
		break;
	case CON_DI64507_MPDIRPOWMAXDEMTIM_LM_BLOCK:        //上月正向需量及时间
		usDI = CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_BLOCK;
		break;
	case  CON_DI64507_MPUNDIRPOWMAXDEMTIM_LM_BLOCK:	    //上月反向需量及时间
		usDI = CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_BLOCK;
		break;
	case CON_DI64507_MPDIRVARMAXDEMTIM_BLOCK:			//正向无功最大需量及时间
		usDI = CON_DI_TYPE_MPDIRVARMAXDEMAND_BLOCK;
		break;
	case CON_DI64507_MPUNDIRVARMAXDEMTIM_BLOCK:			//反向无功最大需量及时间
		usDI = CON_DI_TYPE_MPUNDIRVARMAXDEMAND_BLOCK;
		break;
	case CON_DI64507_MPDIRVARMAXDEMTIM_LM_BLOCK:		//正向无功最大需量（上月）
		usDI = CON_DI_TYPE_MPDIRVARMAXDEMAND_LM_BLOCK;
		break;
	case CON_DI64507_MPUNDIRVARMAXDEMTIM_LM_BLOCK:			//反向无功最大需量及时间（上月）
		usDI = CON_DI_TYPE_MPUNDIRVARMAXDEMAND_LM_BLOCK;
		break;
		/////////////////////////////////////////////////////////////////
	case CON_DI64507_MPVOLA:							//A相电压
		usDI = CON_DI_TYPE_MPVOLA;
		break;
	case CON_DI64507_MPVOLB:							//B相电压
		usDI = CON_DI_TYPE_MPVOLB;
		break;
	case CON_DI64507_MPVOLC:							//C相电压
		usDI = CON_DI_TYPE_MPVOLC;
		break;
	case CON_DI64507_MPVOL_BLOCK:						//电压数据块
		usDI = CON_DI_TYPE_MPVOL_BLOCK;
		break;
	case CON_DI64507_MPCURA:							//A相电流
		usDI = CON_DI_TYPE_MPCURA;
		break;
	case CON_DI64507_MPCURB:							//B相电流
		usDI = CON_DI_TYPE_MPCURB;
		break;
	case CON_DI64507_MPCURC:							//C相电流 
		usDI = CON_DI_TYPE_MPCURC;
		break;
	case CON_DI64507_MPCUR_BLOCK:						//电流数据块
		usDI = CON_DI_TYPE_MPCUR_BLOCK;
		break;
	case CON_DI64507_MPINSTANTPOW:						//瞬时有功功率
		usDI = CON_DI_TYPE_MPINSTANTPOW;
		break;	
	case CON_DI64507_MPINSTANTPOWA:						//A相有功功率
		usDI = CON_DI_TYPE_MPINSTANTPOWA;
		break;	
	case CON_DI64507_MPINSTANTPOWB:						//B相有功功率
		usDI = CON_DI_TYPE_MPINSTANTPOWB;
		break;
	case CON_DI64507_MPINSTANTPOWC:						//C相有功功率
		usDI = CON_DI_TYPE_MPINSTANTPOWC;
		break;
	case CON_DI64507_MPINSTANTPOW_BLOCK:				//
		usDI = CON_DI_TYPE_MPINSTANTPOW_BLOCK;
		break;
	case CON_DI64507_MPINSTANTVAR:						//瞬时无功功率
		usDI = CON_DI_TYPE_MPINSTANTVAR;
		break;
	case CON_DI64507_MPINSTANTVARA:						//A相无功功率
		usDI = CON_DI_TYPE_MPINSTANTVARA;
		break;
	case CON_DI64507_MPINSTANTVARB:						//B相无功功率
		usDI = CON_DI_TYPE_MPINSTANTVARB;
		break;
	case CON_DI64507_MPINSTANTVARC:						//C相无功功率
		usDI = CON_DI_TYPE_MPINSTANTVARC;
		break;
	case CON_DI64507_MPINSTANTVAR_BLOCK:				//	
		usDI = CON_DI_TYPE_MPINSTANTVAR_BLOCK;
		break;
		/////////////////////////////////////////////////////////////////
	
//	case CON_DI_TYPE_MPPOWERFACTOR:                   //总功率因数
//		*usDestDI=CON_DI64507_MPPOWERFACTOR;
//		break;
//	case CON_DI_TYPE_MPPOWERFACTORA:                   //功率因数A
//		*usDestDI=CON_DI64507_MPPOWERFACTORA;
//		break;
//	case CON_DI_TYPE_MPPOWERFACTORB:                   //功率因数B
//		*usDestDI=CON_DI64507_MPPOWERFACTORB;
//		break;
//	case CON_DI_TYPE_MPPOWERFACTORC:                   //功率因数C
//		*usDestDI=CON_DI64507_MPPOWERFACTORC;
//		break;
//	case CON_DI_TYPE_MPPOWERFACTOR_BLOCK:                   //功率因数块
//		*usDestDI=CON_DI64507_MPPOWERFACTOR_BLOCK;
//		break;
	
	    ///////////////////////////////////////////////////////////////

	case CON_DI64507_MPLATELYPROGTIME:             //最近一次编程时间
		usDI = CON_DI_TYPE_MPPROGRAMTIME;
		break;
	case CON_DI64507_MPLATELYMAXDEMANDCLEARTIME:             //最近一次最大需量清零时间
		usDI = CON_DI_TYPE_MPDEMODECLRTIME;
		break;	
	case CON_DI64507_MPPROGTIMETIMES:             //编程次数
		usDI = CON_DI_TYPE_MPPROGRAMNUM;
		break;
	case CON_DI64507_MPMAXDEMANDCLEARTIMES:				//最大需量清零次数
		usDI = CON_DI_TYPE_MPDEMODECLRNUM;
		break;
	case CON_DI64507_MPBATWORKTIME:                   //电池时间
		usDI = CON_DI_TYPE_MPB21F_BLOCK;
		break;
	case CON_DI64507_MPMETERCLRNUM:                 //电表清0总次数
		usDI = CON_DI_TYPE_MPMETERCLRNUM;
		break;
	case CON_DI64507_MPMETERCLRTIME:               //最近一次电表清0时间
		usDI = CON_DI_TYPE_MPMETERCLRTIME;
		break;
	case CON_DI64507_MPEVENTCLRNUM:               //事件清0总次数
		usDI = CON_DI_TYPE_MPEVENTCLRNUM;
		break;
	case CON_DI64507_MPEVENTCLRTIME:             //最近一次事件清0时间
		usDI = CON_DI_TYPE_MPEVENTCLRTIME;
		break;
	case CON_DI64507_MPJIAOSHINUM:               //校时总次数
		usDI = CON_DI_TYPE_MPJIAOSHINUM;
		break;
	case CON_DI64507_MPJIAOSHITIME:             //最近一次校时时间
		usDI = CON_DI_TYPE_MPJIAOSHITIME;
		break;		
		////////////////////////////////////////////////////////////////////
	case CON_DI64507_MPPOWCONST:					//有功常数
		usDI = CON_DI_TYPE_MPPOWCONST;
		break;
	case CON_DI64507_MPVARCONST:					//无功常数	
		usDI = CON_DI_TYPE_MPVARCONST;
		break;
	case CON_DI64507_MPFEERATE_BLOCK:					//费率时段	
		usDI = CON_DI_TYPE_MPFEERATE_BLOCK;
		break;
	case CON_DI64507_MPDATEANDWEEK:						//日期及周次
		usDI = CON_DI_TYPE_MPDATEANDWEEK;
		break;
	case CON_DI64507_MPTIME:							//时间
		usDI = CON_DI_TYPE_MPTIME;
		break;
	//case CON_DI_TYPE_MPDATETIME_BLOCK:					//
	case CON_DI64507_MPCOPYDAYTIME:						//抄表日
		usDI = CON_DI_TYPE_MPCOPYDAYTIME;
		break;
	case CON_DI64507_MPBREAKNOTIM_BLOCK://断相次数
	//case CON_DI_TYPE_MPBREAKTIME_BLOCK://断相时间
		usDI = CON_DI_TYPE_MPB31F_BLOCK;
		break;
	case CON_DI64507_MPMETERSTATFLAG_BLOCK:  //表记电池电压
		usDI = CON_DI_TYPE_MPCO2F_BLOCK;
		break;
	case CON_DI64507_MPVOLBREAKA_BEGEND:
		usDI = CON_DI_TYPE_MPLASTVOLAOFFBEGTIME;//最近断相起始时间结束时间A	
		break;	
	case CON_DI64507_MPVOLBREAKB_BEGEND:	
		usDI = CON_DI_TYPE_MPLASTVOLBOFFBEGTIME;
		break;	
	case CON_DI64507_MPVOLBREAKC_BEGEND:
		usDI = CON_DI_TYPE_MPLASTVOLCOFFBEGTIME;
		break;
	case CON_DI64507_MPVOLBREAKALL_BEG:
		usDI = CON_DI_TYPE_MPLASTVOLOFFBEGTIME;
		break;
	case CON_DI64507_MPVOLBREAKALL_END:
		usDI = CON_DI_TYPE_MPLASTVOLOFFENDTIME;
		break;
	default:
		usDI=0xFFFF;
		break;
	}
	return usDI;
}
*/

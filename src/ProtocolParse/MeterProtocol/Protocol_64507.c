/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Protocol_64507.c
 Description    : DLT-645��Լ������ϸ����
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-08-05    azh     the original version
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
	if(usRecvLen<12)					//��С����Ϊ12�����κ���������
		return RET_TOO_LITTLE;
	
	if(pRecvBuffer[0]!=0x68)			//֡ͷ
	{
		(*pusJumpLen) = 1;
		return RET_HEAD_ERROR;
	}
	
	if(pRecvBuffer[7]!=0x68)
		return RET_ERROR;
	
	if(pRecvBuffer[9]>240)				//��󳤶ȣ�û�п�֤
		return RET_ERROR;

	if( usRecvLen<(pRecvBuffer[9]+12) )
		return RET_NOENOUGH_LONG;

	//���У����
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
        pSendBuffer[ucRetPos++]=0x11;					//���������
		pSendBuffer[ucRetPos++]=0x00;					//���ݳ���
		ucRet=GetDI64507( &usDI64507,pstRS485Task->usDataType );
		
		if( ucRet==RET_SUCCESS )
		{
		
			
			pSendBuffer[ucRetPos++]=(unsigned char )( usDI64507)+0x33;             //������
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
		case CON_DI64507_MPDIRPOW_BLOCK:			//��ȡ�����й�����
		case CON_DI64507_MPUNDIRPOW_BLOCK:			//��ȡ�����й�����
		case CON_DI64507_MPDIRVAR_BLOCK:			//��ȡ�����޹�����
		case CON_DI64507_MPUNDIRVAR_BLOCK:			//��ȡ�����޹�����
		case CON_DI64507_MPDIRPOW_LM_BLOCK:			//��ȡ�����й����������£�
		case CON_DI64507_MPUNDIRPOW_LM_BLOCK:		//��ȡ�����й����������£�
		case CON_DI64507_MPDIRVAR_LM_BLOCK:			//��ȡ�����޹�����������)
		case CON_DI64507_MPUNDIRVAR_LM_BLOCK:		//��ȡ�����޹����������£�
		case CON_DI64507_MPVAR1_BLOCK:				//��ȡ�޹�����_1����
		case CON_DI64507_MPVAR2_BLOCK:				//��ȡ�޹�����_2����
		case CON_DI64507_MPVAR3_BLOCK:				//��ȡ�޹�����_3����
		case CON_DI64507_MPVAR4_BLOCK:				//��ȡ�޹�����_4����
		case CON_DI64507_MPVAR1_LM_BLOCK:			//��ȡ�޹�����_1���ޣ����£�
		case CON_DI64507_MPVAR2_LM_BLOCK:			//��ȡ�޹�����_2���ޣ����£�
		case CON_DI64507_MPVAR3_LM_BLOCK:			//��ȡ�޹�����_3���ޣ����£�
		case CON_DI64507_MPVAR4_LM_BLOCK:			//��ȡ�޹�����_4���ޣ����£�
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
		case CON_DI64507_MPDIRPOWMAXDEMTIM_BLOCK:		//�����й����������ʱ�����ݿ�
		case CON_DI64507_MPDIRVARMAXDEMTIM_BLOCK:		//�����޹��������������ʱ��
		case CON_DI64507_MPUNDIRPOWMAXDEMTIM_BLOCK:		//�����й��������������ʱ�����ݿ�
		case CON_DI64507_MPUNDIRVARMAXDEMTIM_BLOCK:		//�����޹��������������ʱ�����ݿ�
		case CON_DI64507_MPDIRPOWMAXDEMTIM_LM_BLOCK:		//�����й��������������ʱ�����ݿ飨���£�
		case CON_DI64507_MPUNDIRPOWMAXDEMTIM_LM_BLOCK:		//�����й��������������ʱ�����ݿ飨���£�
		case CON_DI64507_MPDIRVARMAXDEMTIM_LM_BLOCK:		//�����޹��������������ʱ�����ݿ飨���£�
		case CON_DI64507_MPUNDIRVARMAXDEMTIM_LM_BLOCK:		//�����޹��������������ʱ�����ݿ飨���£�
			ucRet=RS485_RecvGet07_MaxDemTim_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum,pstRS485SendTask->usDataType);
			break;

		case CON_DI64507_MPVOLA:
		case CON_DI64507_MPVOLB:
		case CON_DI64507_MPVOLC:
			ucRet=RS485_RecvGet07_UPer_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum,pstRS485SendTask->usDataType);
			break;
		case CON_DI64507_MPVOL_BLOCK:			//��ѹ���ݿ�
			ucRet=RS485_RecvGet07_UABC_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum);
			break;
		case CON_DI64507_MPCURA:
		case CON_DI64507_MPCURB:
		case CON_DI64507_MPCURC:
			ucRet=RS485_RecvGet07_IPer_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum,pstRS485SendTask->usDataType);
			break;
		case CON_DI64507_MPCUR_BLOCK:			//�������ݿ�
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
		case CON_DI64507_MPINSTANTPOW_BLOCK:	//˲ʱ�й����ݿ�
		case CON_DI64507_MPINSTANTVAR_BLOCK:	//˲ʱ�޹����ݿ�
			ucRet=RS485_RecvGet07_InstantPow_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum,pstRS485SendTask->usDataType);
			break;
		case CON_DI64507_MPPOWCONST:						//�й����峣����С�����
		case CON_DI64507_MPVARCONST:						//�޹����峣����С�����
			ucRet=RS485_RecvGet07_MeterConst_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum,pstRS485SendTask->usDataType);
			break;
		case CON_DI64507_MPFEERATE_BLOCK:					//�������ݿ�
			ucRet=RS485_RecvGet07_FeeRate_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum);
			break;

		case CON_DI64507_MPLATELYPROGTIME:					//���ʱ�䣨С�����
		case CON_DI64507_MPLATELYMAXDEMANDCLEARTIME:          //�����������ʱ��
		case CON_DI64507_MPMETERCLRTIME:               //���һ�ε����0ʱ��
		case CON_DI64507_MPEVENTCLRTIME:             //���һ���¼���0ʱ��
			ucRet=RS485_RecvGet07_ProgramTime_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum,pstRS485SendTask->usDataType);
			break;
		case CON_DI64507_MPPROGTIMETIMES:
        case CON_DI64507_MPMAXDEMANDCLEARTIMES:	              //��������������
		case CON_DI64507_MPMETERCLRNUM:                        //�����0�ܴ���
		case CON_DI64507_MPEVENTCLRNUM:               //�¼���0�ܴ���
		case CON_DI64507_MPJIAOSHINUM:               //Уʱ�ܴ���
            ucRet=RS485_RecvGet07_ProgramTimes_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum,pstRS485SendTask->usDataType);
			break;
        case CON_DI64507_MPBATWORKTIME:                      //���ʱ��
            ucRet=RS485_RecvGet07_BATWORKTIME_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum);
			break;			
	   case CON_DI64507_MPJIAOSHITIME:             //���һ��Уʱʱ��
            ucRet=RS485_RecvGet07_JIAOSHITIME_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum);
		    break;		
		
		case CON_DI64507_MPMETERSTATFLAG_BLOCK:						//���״̬�ֿ�
			ucRet=RS485_RecvGet07_MeterStatFlag_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum);
			break;

		case CON_DI64507_MPCOPYDAYTIME:						//�����գ�С�����
			ucRet=RS485_RecvGet07_CopyDay_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum);
			break;
		case CON_DI64507_MPDATEANDWEEK:						//���ڼ��ܴ�
			ucRet=RS485_RecvGet07_DateAndWeek_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum);
			break;
		case CON_DI64507_MPTIME:							//ʱ��
			ucRet=RS485_RecvGet07_Time_Explain(pstRS485SendTask,pucRecvBuffer,usRecvLen,pucRecvNum);
			break;
		case CON_DI64507_MPBREAKNUMA:						//�ۼƶ������ʱ��
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
		case CON_DI64507_MPBREAKENDTIME_C:	//���һ�ζ�����ʼʱ��
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
	//����1
	SetRS485TaskPer(&(stRS485TaskRecvExplain[1]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
		pstRS485SendTask->ucReportType,usDIFee1,pstRS485SendTask->ucDevNo,NULL);
	stRS485TaskRecvExplain[1].ucDataLen=4;
	FillData_EE(stRS485TaskRecvExplain[1].ucDataArea,4);
	//����2
	SetRS485TaskPer(&(stRS485TaskRecvExplain[2]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
		pstRS485SendTask->ucReportType,usDIFee2,pstRS485SendTask->ucDevNo,NULL);
	stRS485TaskRecvExplain[2].ucDataLen=4;
	FillData_EE(stRS485TaskRecvExplain[2].ucDataArea,4);
	//����3
	SetRS485TaskPer(&(stRS485TaskRecvExplain[3]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
		pstRS485SendTask->ucReportType,usDIFee3,pstRS485SendTask->ucDevNo,NULL);
	stRS485TaskRecvExplain[3].ucDataLen=4;
	FillData_EE(stRS485TaskRecvExplain[3].ucDataArea,4);
	//����4
	SetRS485TaskPer(&(stRS485TaskRecvExplain[4]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
		pstRS485SendTask->ucReportType,usDIFee4,pstRS485SendTask->ucDevNo,NULL);
	stRS485TaskRecvExplain[4].ucDataLen=4;
	FillData_EE(stRS485TaskRecvExplain[4].ucDataArea,4);
	
	return ucRet;
}
unsigned char RS485_RecvGet07_IPer_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI)
{//�������
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

		(*pucRecvNum)=1;//�����1������
	}
	return ucRet;
}

unsigned char RS485_RecvGet07_IABC_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum)
{//�����������
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

		(*pucRecvNum)=3;//�����3������
	}
	return ucRet;
}
unsigned char RS485_RecvGet07_UPer_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI)
{//�����ѹ����
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

		(*pucRecvNum)=1;//�����1������
	}
	return ucRet;
}
unsigned char RS485_RecvGet07_UABC_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum)
{//�����ѹ����
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

		(*pucRecvNum)=3;//�����3������
	}
	return ucRet;
}
unsigned char RS485_RecvGet07_InstantPowPer_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI)
{//��˲ʱ���޹�����
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
{//˲ʱ�����޹�
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
		if(ucRecvContentLen<12)									//����ܣ��϶���Ч���ֿ�����Ч
			break;
	}

	(*pucRecvNum)=4;
	return ucRet;
}


unsigned char RS485_RecvGet07_MeterStatFlag_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum)
{//�������״̬�ֵ�

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
{//���ʱ�����
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucRecvContent[6];
	unsigned char ucRecvContentLen;	

	if( usRecvLen<22 ) 
		ucRet=RET_ERROR;

	if( ucRet==RET_SUCCESS )
	{//���һ�α��ʱ��
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
{//��̴�������
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucRecvContent[3];
	unsigned char ucRecvContentLen;
//	unsigned char m=0,i=0,ucPos=0;

	if(ucRet==RET_SUCCESS)
	{
		//��̴���
		SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,
							pstRS485SendTask->ucReportType,usDI,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[0].ucDataLen=4;
		FillData_EE(stRS485TaskRecvExplain[0].ucDataArea,4);
		
		memset( ucRecvContent,0,3 );
		ucRecvContentLen=GetRecvContent07(ucRecvContent,3,pucRecvBuffer,usRecvLen);
        Fill_DataList_Byte3(ucRecvContent, ucRecvContentLen,1);

		(*pucRecvNum)=1;//�����1������
	}

	return ucRet;
}

unsigned char RS485_RecvGet07_BATWORKTIME_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum)
{//��ع���ʱ�����
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucRecvContent[4];
	unsigned char ucRecvContentLen;
//	unsigned char m=0,i=0,ucPos=0;

	if(ucRet==RET_SUCCESS)
	{
		//��ع���ʱ��
		SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,
							pstRS485SendTask->ucReportType,CON_DI_TYPE_MPBATWORKTIME,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[0].ucDataLen=4;
		FillData_EE(stRS485TaskRecvExplain[0].ucDataArea,4);
		
		memset( ucRecvContent,0,4 );
		ucRecvContentLen=GetRecvContent07(ucRecvContent,4,pucRecvBuffer,usRecvLen);
        Fill_DataList_Byte4(ucRecvContent, ucRecvContentLen,1);

		(*pucRecvNum)=1;//�����1������
	}

	return ucRet;
}

unsigned char RS485_RecvGet07_JIAOSHITIME_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum)
{//���һ��Уʱʱ�����
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucRecvContent[16];
	unsigned char ucRecvContentLen;	

	if( usRecvLen<22 ) 
		ucRet=RET_ERROR;

	if( ucRet==RET_SUCCESS )
	{//���һ��Уʱʱ��
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
	{//���ڼ��ܴ�
		SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,
			pstRS485SendTask->ucReportType,CON_DI_TYPE_MPDATEANDWEEK,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[0].ucDataLen=6;
		FillData_AllEE(stRS485TaskRecvExplain[0].ucDataArea,6);

		memset( ucRecvContent,0,4 );
		ucRecvContentLen=GetRecvContent07(ucRecvContent,4,pucRecvBuffer,usRecvLen);

		if( ucRecvContentLen >= 4)
		{//�ܾͲ�������
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
{//���ڼ�ʱ��
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucRecvContent[7];
	unsigned char ucRecvContentLen;
	STDATETIME	  stDateTime;
	ClearDateTime(&stDateTime);

	if( usRecvLen<20 )
		ucRet=RET_ERROR;

	if(ucRet==RET_SUCCESS)
	{	//���ڼ��ܴ�
		SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,
			pstRS485SendTask->ucReportType,CON_DI_TYPE_MPDATEANDWEEK,pstRS485SendTask->ucDevNo,NULL);
		stRS485TaskRecvExplain[0].ucDataLen=6;
		FillData_AllEE(stRS485TaskRecvExplain[0].ucDataArea,6);
		//ʱ��
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
		
		(*pucRecvNum)=2;//�����2������
	}
	return ucRet;
}
*/
unsigned char RS485_RecvGet07_MaxDemTim_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI)
{//�������������ʱ�����
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
{//����ʱ��
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
{//�����
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucRecvContent[24];
	unsigned char ucRecvContentLen;

	if( usRecvLen<19 ) 
		ucRet=RET_ERROR;
	
	if(ucRet==RET_SUCCESS)
	{
		SetRS485TaskPer(&(stRS485TaskRecvExplain[0]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
							pstRS485SendTask->ucReportType,CON_DI_TYPE_MPPOWCONST,pstRS485SendTask->ucDevNo,NULL);//�й�
		stRS485TaskRecvExplain[0].ucDataLen=4;
		FillData_EE( stRS485TaskRecvExplain[0].ucDataArea,4 );

		SetRS485TaskPer(&(stRS485TaskRecvExplain[1]),pstRS485SendTask->ucTaskSource,pstRS485SendTask->ucSeqNo,	
							pstRS485SendTask->ucReportType,CON_DI_TYPE_MPVARCONST,pstRS485SendTask->ucDevNo,NULL);//�޹�
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
{//�������ʱ��
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
//	if((usDI & 0x00f0) == 0x0010)	//Ҫ�����Ƕ����ܴ���
//	{
//		memcpy(&ucTmp[3],ucRecvContent,3);
//		memcpy(&ucTmp[6],&ucRecvContent[6],3);
//		memcpy(&ucTmp[9],&ucRecvContent[12],3);
//	}
//	else							//���������ۼ�ʱ��
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
{//���һ�ζ�����ʼʱ��
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
//	if((usDI & 0x00f0) == 0x0030)	//�����࿪ʼʱ��
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

//add by cfh 10-01-18 �����������
unsigned char RS485_RecvGet07_PowVarABC_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI)
{//���һ�ζ�����ʼʱ��
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


//��д��������
unsigned char Fill_PowerDataList(unsigned char *pucRecvContent,unsigned char ucRecvContentLen)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucPos=0,i=0,j=0;
	unsigned long ulVal=0;
	for(j=0;j<5;j++)									//5=��+4������
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
	case CON_DI_TYPE_MPDIRPOW_BLOCK:                    //�����й�����
		*usDestDI=CON_DI64507_MPDIRPOW_BLOCK;
		break;	
	case CON_DI_TYPE_MPUNDIRPOW_BLOCK:                  //�����й�����
		*usDestDI=CON_DI64507_MPUNDIRPOW_BLOCK;
		break;
	case  CON_DI_TYPE_MPDIRPOW_LM_BLOCK:				//���������й�����
		*usDestDI=CON_DI64507_MPDIRPOW_LM_BLOCK;
		break;
	case  CON_DI_TYPE_MPUNDIRPOW_LM_BLOCK:				//���·����й�����
		*usDestDI=CON_DI64507_MPUNDIRPOW_LM_BLOCK;
		break;
	
    case CON_DI_TYPE_MPDIRVAR_BLOCK:                      //�����޹�
		*usDestDI=CON_DI64507_MPDIRVAR_BLOCK;
		break;
    case CON_DI_TYPE_MPUNDIRVAR_BLOCK:                      //�����޹�
		*usDestDI=CON_DI64507_MPUNDIRVAR_BLOCK;
		break;
		
   	case CON_DI_TYPE_MPVAR1_BLOCK:                      //һ�����޹�
		*usDestDI=CON_DI64507_MPVAR1_BLOCK;
		break;
    case CON_DI_TYPE_MPVAR2_BLOCK:                      //�������޹�
		*usDestDI=CON_DI64507_MPVAR2_BLOCK;
		break;
    case CON_DI_TYPE_MPVAR3_BLOCK:                      //�������޹�
		*usDestDI=CON_DI64507_MPVAR3_BLOCK;
		break;
	case CON_DI_TYPE_MPVAR4_BLOCK:                      //�������޹�
		*usDestDI=CON_DI64507_MPVAR4_BLOCK;
		break;
	case CON_DI_TYPE_MPDIRVAR_LM_BLOCK:                      //���������޹�
		*usDestDI=CON_DI64507_MPDIRVAR_LM_BLOCK;
		break;
    case CON_DI_TYPE_MPUNDIRVAR_LM_BLOCK:                      //���·����޹�
		*usDestDI=CON_DI64507_MPUNDIRVAR_LM_BLOCK;
		break;
	case CON_DI_TYPE_MPVAR1_LM_BLOCK:                   //����һ�����޹�
		*usDestDI=CON_DI64507_MPVAR1_LM_BLOCK;
		break;
    case CON_DI_TYPE_MPVAR2_LM_BLOCK:                   //���¶������޹�
		*usDestDI=CON_DI64507_MPVAR2_LM_BLOCK;
		break;
	case CON_DI_TYPE_MPVAR3_LM_BLOCK:                   //�����������޹�
		*usDestDI=CON_DI64507_MPVAR3_LM_BLOCK;
		break;
	case CON_DI_TYPE_MPVAR4_LM_BLOCK:                   //�����������޹�
		*usDestDI=CON_DI64507_MPVAR4_LM_BLOCK;
		break;
		/////////////////////////////////////////////////////////////////
	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_BLOCK:           //����������ʱ��
	//case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_BLOCK:
		*usDestDI=CON_DI64507_MPDIRPOWMAXDEMTIM_BLOCK;
		break;
	case  CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_BLOCK:	    //����������ʱ��
	//case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_BLOCK:		
		*usDestDI=CON_DI64507_MPUNDIRPOWMAXDEMTIM_BLOCK;
		break;
	case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_BLOCK:        //��������������ʱ��
	//case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_BLOCK:
		*usDestDI=CON_DI64507_MPDIRPOWMAXDEMTIM_LM_BLOCK;
		break;
	case  CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_BLOCK:	    //���·���������ʱ��
	//case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_BLOCK:		
		*usDestDI=CON_DI64507_MPUNDIRPOWMAXDEMTIM_LM_BLOCK;
		break;
	case CON_DI_TYPE_MPDIRVARMAXDEMAND_BLOCK:			//�����޹����������ʱ��
	    *usDestDI=CON_DI64507_MPDIRVARMAXDEMTIM_BLOCK;
		break;
	case CON_DI_TYPE_MPUNDIRVARMAXDEMAND_BLOCK:			//�����޹����������ʱ��
	    *usDestDI=CON_DI64507_MPUNDIRVARMAXDEMTIM_BLOCK;
		break;
	case CON_DI_TYPE_MPDIRVARMAXDEMAND_LM_BLOCK:		//�����޹�������������£�
	    *usDestDI=CON_DI64507_MPDIRVARMAXDEMTIM_LM_BLOCK;
		break;
	case CON_DI_TYPE_MPUNDIRVARMAXDEMAND_LM_BLOCK:			//�����޹����������ʱ�䣨���£�
	    *usDestDI=CON_DI64507_MPUNDIRVARMAXDEMTIM_LM_BLOCK;
		break;
		/////////////////////////////////////////////////////////////////
	case CON_DI_TYPE_MPVOLA:							//A���ѹ
	    *usDestDI=CON_DI64507_MPVOLA;
		break;
	case CON_DI_TYPE_MPVOLB:							//B���ѹ
	    *usDestDI=CON_DI64507_MPVOLB;
		break;
	case CON_DI_TYPE_MPVOLC:							//C���ѹ
	    *usDestDI=CON_DI64507_MPVOLC;
		break;
	case CON_DI_TYPE_MPVOL_BLOCK:						//��ѹ���ݿ�
	    *usDestDI=CON_DI64507_MPVOL_BLOCK;
		break;
	case CON_DI_TYPE_MPCURA:							//A�����
	    *usDestDI=CON_DI64507_MPCURA;
		break;
	case CON_DI_TYPE_MPCURB:							//B�����
	    *usDestDI=CON_DI64507_MPCURB;
		break;
	case CON_DI_TYPE_MPCURC:							//C����� 
	    *usDestDI=CON_DI64507_MPCURC;
		break;
	case CON_DI_TYPE_MPCUR_BLOCK:						//�������ݿ�
	    *usDestDI=CON_DI64507_MPCUR_BLOCK;
		break;
	case CON_DI_TYPE_MPINSTANTPOW:						//˲ʱ�й�����
	    *usDestDI=CON_DI64507_MPINSTANTPOW;
		break;	
	case CON_DI_TYPE_MPINSTANTPOWA:						//A���й�����
	    *usDestDI=CON_DI64507_MPINSTANTPOWA;
		break;	
	case CON_DI_TYPE_MPINSTANTPOWB:						//B���й�����
	    *usDestDI=CON_DI64507_MPINSTANTPOWB;
		break;
	case CON_DI_TYPE_MPINSTANTPOWC:						//C���й�����
	    *usDestDI=CON_DI64507_MPINSTANTPOWC;
		break;
	case CON_DI_TYPE_MPINSTANTPOW_BLOCK:				//
	    *usDestDI=CON_DI64507_MPINSTANTPOW_BLOCK;
		break;
	case CON_DI_TYPE_MPINSTANTVAR:						//˲ʱ�޹�����
	    *usDestDI=CON_DI64507_MPINSTANTVAR;
		break;
	case CON_DI_TYPE_MPINSTANTVARA:						//A���޹�����
	    *usDestDI=CON_DI64507_MPINSTANTVARA;
		break;
	case CON_DI_TYPE_MPINSTANTVARB:						//B���޹�����
	    *usDestDI=CON_DI64507_MPINSTANTVARB;
		break;
	case CON_DI_TYPE_MPINSTANTVARC:						//C���޹�����
	    *usDestDI=CON_DI64507_MPINSTANTVARC;
		break;
	case CON_DI_TYPE_MPINSTANTVAR_BLOCK:				//	
		*usDestDI=CON_DI64507_MPINSTANTVAR_BLOCK;
		break;
	case CON_DI_TYPE_MPCUFE:
		*usDestDI = CON_DI64507_MPCUFE_BLOCK;
		/////////////////////////////////////////////////////////////////
/*
    case CON_DI_TYPE_MPPOWERFACTOR:                   //�ܹ�������
		*usDestDI=CON_DI64507_MPPOWERFACTOR;
		break;
    case CON_DI_TYPE_MPPOWERFACTORA:                   //��������A
		*usDestDI=CON_DI64507_MPPOWERFACTORA;
		break;
    case CON_DI_TYPE_MPPOWERFACTORB:                   //��������B
		*usDestDI=CON_DI64507_MPPOWERFACTORB;
		break;
    case CON_DI_TYPE_MPPOWERFACTORC:                   //��������C
		*usDestDI=CON_DI64507_MPPOWERFACTORC;
		break;
    case CON_DI_TYPE_MPPOWERFACTOR_BLOCK:                   //����������
		*usDestDI=CON_DI64507_MPPOWERFACTOR_BLOCK;
		break;
*/
        ///////////////////////////////////////////////////////////////

	case CON_DI_TYPE_MPPROGRAMTIME:             //���һ�α��ʱ��
        *usDestDI=CON_DI64507_MPLATELYPROGTIME;
		break;
	case CON_DI_TYPE_MPDEMODECLRTIME:             //���һ�������������ʱ��
        *usDestDI=CON_DI64507_MPLATELYMAXDEMANDCLEARTIME;
		break;	
    case CON_DI_TYPE_MPPROGRAMNUM:             //��̴���
        *usDestDI=CON_DI64507_MPPROGTIMETIMES;
		break;
    case CON_DI_TYPE_MPDEMODECLRNUM:				//��������������
        *usDestDI=CON_DI64507_MPMAXDEMANDCLEARTIMES;
		break;
	case CON_DI_TYPE_MPB21F_BLOCK:                   //���ʱ��
		*usDestDI=CON_DI64507_MPBATWORKTIME;
		break;
	case CON_DI_TYPE_MPMETERCLRNUM:                 //�����0�ܴ���
        *usDestDI=CON_DI64507_MPMETERCLRNUM;
		break;
	case CON_DI_TYPE_MPMETERCLRTIME:               //���һ�ε����0ʱ��
        *usDestDI=CON_DI64507_MPMETERCLRTIME;
		break;
	case CON_DI_TYPE_MPEVENTCLRNUM:               //�¼���0�ܴ���
        *usDestDI=CON_DI64507_MPEVENTCLRNUM;
		break;
	case CON_DI_TYPE_MPEVENTCLRTIME:             //���һ���¼���0ʱ��
        *usDestDI=CON_DI64507_MPEVENTCLRTIME;
		break;
	case CON_DI_TYPE_MPJIAOSHINUM:               //Уʱ�ܴ���
        *usDestDI=CON_DI64507_MPJIAOSHINUM;
		break;
	case CON_DI_TYPE_MPJIAOSHITIME:             //���һ��Уʱʱ��
        *usDestDI=CON_DI64507_MPJIAOSHITIME;
		break;		
		////////////////////////////////////////////////////////////////////
  	case CON_DI_TYPE_MPPOWCONST:					//�й�����
   		*usDestDI=CON_DI64507_MPPOWCONST;
		break;
    case CON_DI_TYPE_MPVARCONST:					//�޹�����	
		*usDestDI=CON_DI64507_MPVARCONST;
		break;
	case CON_DI_TYPE_MPFEERATE_BLOCK:					//����ʱ��	
		*usDestDI=CON_DI64507_MPFEERATE_BLOCK;
		break;
	case CON_DI_TYPE_MPDATEANDWEEK:						//���ڼ��ܴ�
	    *usDestDI=CON_DI64507_MPDATEANDWEEK;
		break;
	case CON_DI_TYPE_MPTIME:							//ʱ��
	    *usDestDI=CON_DI64507_MPTIME;
		break;
	//case CON_DI_TYPE_MPDATETIME_BLOCK:					//
	case CON_DI_TYPE_MPCOPYDAYTIME:						//������
	    *usDestDI=CON_DI64507_MPCOPYDAYTIME;
		break;
    //case CON_DI_TYPE_MPB31F_BLOCK://�������
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
	//case CON_DI_TYPE_MPBREAKTIME_BLOCK://����ʱ��
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
		*usDestDI=CON_DI64507_MPBREAKBEGTIME_A;//���������ʼʱ�����ʱ��A	
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
	case CON_DI_TYPE_MPLASTVOLAOFFENDTIME:	//����ʱ��
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
	case CON_DI_TYPE_MPCO2F_BLOCK:  //��ǵ�ص�ѹ
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
	case CON_DI64507_MPDIRPOW_BLOCK:                    //�����й�����
		usDI = CON_DI_TYPE_MPDIRPOW_BLOCK;
		break;	
	case CON_DI64507_MPUNDIRPOW_BLOCK:                  //�����й�����
		usDI = CON_DI_TYPE_MPUNDIRPOW_BLOCK;
		break;
	case  CON_DI64507_MPDIRPOW_LM_BLOCK:				//���������й�����
		usDI = CON_DI_TYPE_MPDIRPOW_LM_BLOCK;
		break;
	case  CON_DI64507_MPUNDIRPOW_LM_BLOCK:				//���·����й�����
		usDI = CON_DI_TYPE_MPUNDIRPOW_LM_BLOCK;
		break;

	case CON_DI64507_MPDIRVAR_BLOCK:                      //�����޹�
		usDI = CON_DI_TYPE_MPDIRVAR_BLOCK;
		break;
	case CON_DI64507_MPUNDIRVAR_BLOCK:                      //�����޹�
		usDI = CON_DI_TYPE_MPUNDIRVAR_BLOCK;
		break;		
	case CON_DI64507_MPVAR1_BLOCK:                      //һ�����޹�
		usDI = CON_DI_TYPE_MPVAR1_BLOCK;
		break;
	case CON_DI64507_MPVAR2_BLOCK:                      //�������޹�
		usDI = CON_DI_TYPE_MPVAR2_BLOCK;
		break;
	case CON_DI64507_MPVAR3_BLOCK:                      //�������޹�
		usDI = CON_DI_TYPE_MPVAR3_BLOCK;
		break;
	case CON_DI64507_MPVAR4_BLOCK:                      //�������޹�
		usDI = CON_DI_TYPE_MPVAR4_BLOCK;
		break;
	case CON_DI64507_MPDIRVAR_LM_BLOCK:                      //���������޹�
		usDI = CON_DI_TYPE_MPDIRVAR_LM_BLOCK;
		break;
	case CON_DI64507_MPUNDIRVAR_LM_BLOCK:                      //���·����޹�
		usDI = CON_DI_TYPE_MPUNDIRVAR_LM_BLOCK;
		break;
	case CON_DI64507_MPVAR1_LM_BLOCK:                   //����һ�����޹�
		usDI = CON_DI_TYPE_MPVAR1_LM_BLOCK;
		break;
	case CON_DI64507_MPVAR2_LM_BLOCK:                   //���¶������޹�
		usDI = CON_DI_TYPE_MPVAR2_LM_BLOCK;
		break;
	case CON_DI64507_MPVAR3_LM_BLOCK:                   //�����������޹�
		usDI = CON_DI_TYPE_MPVAR3_LM_BLOCK;
		break;
	case CON_DI64507_MPVAR4_LM_BLOCK:                   //�����������޹�
		usDI = CON_DI_TYPE_MPVAR4_LM_BLOCK;
		break;
		/////////////////////////////////////////////////////////////////
	case CON_DI64507_MPDIRPOWMAXDEMTIM_BLOCK:           //����������ʱ��
		usDI = CON_DI_TYPE_MPDIRPOWMAXDEMAND_BLOCK;
		break;
	case  CON_DI64507_MPUNDIRPOWMAXDEMTIM_BLOCK:	    //����������ʱ��
		usDI = CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_BLOCK;
		break;
	case CON_DI64507_MPDIRPOWMAXDEMTIM_LM_BLOCK:        //��������������ʱ��
		usDI = CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_BLOCK;
		break;
	case  CON_DI64507_MPUNDIRPOWMAXDEMTIM_LM_BLOCK:	    //���·���������ʱ��
		usDI = CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_BLOCK;
		break;
	case CON_DI64507_MPDIRVARMAXDEMTIM_BLOCK:			//�����޹����������ʱ��
		usDI = CON_DI_TYPE_MPDIRVARMAXDEMAND_BLOCK;
		break;
	case CON_DI64507_MPUNDIRVARMAXDEMTIM_BLOCK:			//�����޹����������ʱ��
		usDI = CON_DI_TYPE_MPUNDIRVARMAXDEMAND_BLOCK;
		break;
	case CON_DI64507_MPDIRVARMAXDEMTIM_LM_BLOCK:		//�����޹�������������£�
		usDI = CON_DI_TYPE_MPDIRVARMAXDEMAND_LM_BLOCK;
		break;
	case CON_DI64507_MPUNDIRVARMAXDEMTIM_LM_BLOCK:			//�����޹����������ʱ�䣨���£�
		usDI = CON_DI_TYPE_MPUNDIRVARMAXDEMAND_LM_BLOCK;
		break;
		/////////////////////////////////////////////////////////////////
	case CON_DI64507_MPVOLA:							//A���ѹ
		usDI = CON_DI_TYPE_MPVOLA;
		break;
	case CON_DI64507_MPVOLB:							//B���ѹ
		usDI = CON_DI_TYPE_MPVOLB;
		break;
	case CON_DI64507_MPVOLC:							//C���ѹ
		usDI = CON_DI_TYPE_MPVOLC;
		break;
	case CON_DI64507_MPVOL_BLOCK:						//��ѹ���ݿ�
		usDI = CON_DI_TYPE_MPVOL_BLOCK;
		break;
	case CON_DI64507_MPCURA:							//A�����
		usDI = CON_DI_TYPE_MPCURA;
		break;
	case CON_DI64507_MPCURB:							//B�����
		usDI = CON_DI_TYPE_MPCURB;
		break;
	case CON_DI64507_MPCURC:							//C����� 
		usDI = CON_DI_TYPE_MPCURC;
		break;
	case CON_DI64507_MPCUR_BLOCK:						//�������ݿ�
		usDI = CON_DI_TYPE_MPCUR_BLOCK;
		break;
	case CON_DI64507_MPINSTANTPOW:						//˲ʱ�й�����
		usDI = CON_DI_TYPE_MPINSTANTPOW;
		break;	
	case CON_DI64507_MPINSTANTPOWA:						//A���й�����
		usDI = CON_DI_TYPE_MPINSTANTPOWA;
		break;	
	case CON_DI64507_MPINSTANTPOWB:						//B���й�����
		usDI = CON_DI_TYPE_MPINSTANTPOWB;
		break;
	case CON_DI64507_MPINSTANTPOWC:						//C���й�����
		usDI = CON_DI_TYPE_MPINSTANTPOWC;
		break;
	case CON_DI64507_MPINSTANTPOW_BLOCK:				//
		usDI = CON_DI_TYPE_MPINSTANTPOW_BLOCK;
		break;
	case CON_DI64507_MPINSTANTVAR:						//˲ʱ�޹�����
		usDI = CON_DI_TYPE_MPINSTANTVAR;
		break;
	case CON_DI64507_MPINSTANTVARA:						//A���޹�����
		usDI = CON_DI_TYPE_MPINSTANTVARA;
		break;
	case CON_DI64507_MPINSTANTVARB:						//B���޹�����
		usDI = CON_DI_TYPE_MPINSTANTVARB;
		break;
	case CON_DI64507_MPINSTANTVARC:						//C���޹�����
		usDI = CON_DI_TYPE_MPINSTANTVARC;
		break;
	case CON_DI64507_MPINSTANTVAR_BLOCK:				//	
		usDI = CON_DI_TYPE_MPINSTANTVAR_BLOCK;
		break;
		/////////////////////////////////////////////////////////////////
	
//	case CON_DI_TYPE_MPPOWERFACTOR:                   //�ܹ�������
//		*usDestDI=CON_DI64507_MPPOWERFACTOR;
//		break;
//	case CON_DI_TYPE_MPPOWERFACTORA:                   //��������A
//		*usDestDI=CON_DI64507_MPPOWERFACTORA;
//		break;
//	case CON_DI_TYPE_MPPOWERFACTORB:                   //��������B
//		*usDestDI=CON_DI64507_MPPOWERFACTORB;
//		break;
//	case CON_DI_TYPE_MPPOWERFACTORC:                   //��������C
//		*usDestDI=CON_DI64507_MPPOWERFACTORC;
//		break;
//	case CON_DI_TYPE_MPPOWERFACTOR_BLOCK:                   //����������
//		*usDestDI=CON_DI64507_MPPOWERFACTOR_BLOCK;
//		break;
	
	    ///////////////////////////////////////////////////////////////

	case CON_DI64507_MPLATELYPROGTIME:             //���һ�α��ʱ��
		usDI = CON_DI_TYPE_MPPROGRAMTIME;
		break;
	case CON_DI64507_MPLATELYMAXDEMANDCLEARTIME:             //���һ�������������ʱ��
		usDI = CON_DI_TYPE_MPDEMODECLRTIME;
		break;	
	case CON_DI64507_MPPROGTIMETIMES:             //��̴���
		usDI = CON_DI_TYPE_MPPROGRAMNUM;
		break;
	case CON_DI64507_MPMAXDEMANDCLEARTIMES:				//��������������
		usDI = CON_DI_TYPE_MPDEMODECLRNUM;
		break;
	case CON_DI64507_MPBATWORKTIME:                   //���ʱ��
		usDI = CON_DI_TYPE_MPB21F_BLOCK;
		break;
	case CON_DI64507_MPMETERCLRNUM:                 //�����0�ܴ���
		usDI = CON_DI_TYPE_MPMETERCLRNUM;
		break;
	case CON_DI64507_MPMETERCLRTIME:               //���һ�ε����0ʱ��
		usDI = CON_DI_TYPE_MPMETERCLRTIME;
		break;
	case CON_DI64507_MPEVENTCLRNUM:               //�¼���0�ܴ���
		usDI = CON_DI_TYPE_MPEVENTCLRNUM;
		break;
	case CON_DI64507_MPEVENTCLRTIME:             //���һ���¼���0ʱ��
		usDI = CON_DI_TYPE_MPEVENTCLRTIME;
		break;
	case CON_DI64507_MPJIAOSHINUM:               //Уʱ�ܴ���
		usDI = CON_DI_TYPE_MPJIAOSHINUM;
		break;
	case CON_DI64507_MPJIAOSHITIME:             //���һ��Уʱʱ��
		usDI = CON_DI_TYPE_MPJIAOSHITIME;
		break;		
		////////////////////////////////////////////////////////////////////
	case CON_DI64507_MPPOWCONST:					//�й�����
		usDI = CON_DI_TYPE_MPPOWCONST;
		break;
	case CON_DI64507_MPVARCONST:					//�޹�����	
		usDI = CON_DI_TYPE_MPVARCONST;
		break;
	case CON_DI64507_MPFEERATE_BLOCK:					//����ʱ��	
		usDI = CON_DI_TYPE_MPFEERATE_BLOCK;
		break;
	case CON_DI64507_MPDATEANDWEEK:						//���ڼ��ܴ�
		usDI = CON_DI_TYPE_MPDATEANDWEEK;
		break;
	case CON_DI64507_MPTIME:							//ʱ��
		usDI = CON_DI_TYPE_MPTIME;
		break;
	//case CON_DI_TYPE_MPDATETIME_BLOCK:					//
	case CON_DI64507_MPCOPYDAYTIME:						//������
		usDI = CON_DI_TYPE_MPCOPYDAYTIME;
		break;
	case CON_DI64507_MPBREAKNOTIM_BLOCK://�������
	//case CON_DI_TYPE_MPBREAKTIME_BLOCK://����ʱ��
		usDI = CON_DI_TYPE_MPB31F_BLOCK;
		break;
	case CON_DI64507_MPMETERSTATFLAG_BLOCK:  //��ǵ�ص�ѹ
		usDI = CON_DI_TYPE_MPCO2F_BLOCK;
		break;
	case CON_DI64507_MPVOLBREAKA_BEGEND:
		usDI = CON_DI_TYPE_MPLASTVOLAOFFBEGTIME;//���������ʼʱ�����ʱ��A	
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

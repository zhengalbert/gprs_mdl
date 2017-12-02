/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_DataTrans.c
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

static unsigned char	Main_SendComm_DataTrans(unsigned char ucTaskSour);

//发送，数据转发，中继
unsigned char Main_SendComm_DataTrans(unsigned char ucTaskSour)
{
	unsigned char	ucRet=RET_SUCCESS;
    unsigned char   *mem_ptr=NULL,mem_type;
	unsigned char	*pucSrcBuffer=NULL,*pucRetBuffer=NULL;
	unsigned char	m=0;
	unsigned char	uclink=11;				//不需要主站确认
	unsigned short	usSendLen=0;
	STMAINCOMMINFO	stMainCommInfo;
	unsigned short	usDataLen;

	ucRet = Main_GetCommInfoList(stRS485RecvExpainVal.ucSeqNo,&stMainCommInfo);
	if(ucRet == RET_ERROR)
		return ucRet;

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
//	pucSrcBuffer=(unsigned char *)CTaskWorkBuffer+16;							//利用临时缓冲区
//	pucRetBuffer=(unsigned char *)CTaskWorkBuffer+16+CON_SEND_BLOCK_MAX_SIZE;	//pucBuffer限制CON_SEND_BLOCK_MAX_SIZE
	pucSrcBuffer=(unsigned char *)mem_ptr+16;							//利用临时缓冲区
	pucRetBuffer=(unsigned char *)mem_ptr+16+CON_SEND_BLOCK_MAX_SIZE;	//pucBuffer限制CON_SEND_BLOCK_MAX_SIZE
//	usDataLen=stRS485RecvExpainVal.ucDataArea[0]+0x100*stRS485RecvExpainVal.ucDataArea[1];
    usDataLen = stRelayDataBuf.usDataLen; 
	if(usDataLen!=0)
	{
		pucSrcBuffer[m++]=0;					//DA1
		pucSrcBuffer[m++]=0;					//DA2
		pucSrcBuffer[m++]=1;					//DT1
		pucSrcBuffer[m++]=0;					//DT2
		if(usDataLen<CON_SEND_BLOCK_BUFFER_SIZE)
			usSendLen=usDataLen;
		else
			usSendLen=CON_SEND_BLOCK_BUFFER_SIZE;
	//	pucSrcBuffer[m++]=(unsigned char)usSendLen;
		pucSrcBuffer[m++] = stRS485RecvExpainVal.ucDevNo +2;
		pucSrcBuffer[m++]=(unsigned char)usSendLen;
		pucSrcBuffer[m++]=(unsigned char)(usSendLen >> 8);
//		memcpy(pucSrcBuffer+m,stRS485RecvExpainVal.ucDataArea+2,usSendLen);
        memcpy(pucSrcBuffer+m,stRelayDataBuf.ucDataArea,usSendLen);

		usSendLen+=m;
		Main_SendExplain(CON_TERMSG_DATATRANS,pucSrcBuffer,pucRetBuffer,&usSendLen,&stMainCommInfo,CON_STATUS_NO,CON_STATUS_NO,0x60);			//组帧，加上报文头尾
	}
	else
	{//空，回否认
		pucSrcBuffer[m++]=0;
		pucSrcBuffer[m++]=0;
		pucSrcBuffer[m++]=2;
		pucSrcBuffer[m++]=0;
		usSendLen+=m;
		Main_SendExplain(CON_TERMSG_ACK,pucSrcBuffer,pucRetBuffer,&usSendLen,&stMainCommInfo,CON_STATUS_NO,CON_STATUS_NO,0x60);			//组帧，加上报文头尾
	}
	Main_FillUpData(pucRetBuffer,usSendLen,ucTaskSour,uclink);
	free_mem_block(mem_ptr);//
	return ucRet;
}
/*
unsigned char Main_SendComm_DataTransF9(unsigned char ucTaskSour,unsigned char ucPortNo,unsigned char ucFlag)
{
	unsigned char	ucRet=RET_SUCCESS,i=0;
	unsigned char	*pucSrcBuffer=NULL,*pucRetBuffer=NULL;
	unsigned char	m=0;
	unsigned char	uclink=11;				//不需要主站确认
	unsigned short	usSendLen=0;
	STMAINCOMMINFO	stMainCommInfo;
	unsigned short	usDataLen;

	ucRet = Main_GetCommInfoList(stRS485RecvExpainVal.ucSeqNo,&stMainCommInfo);
	if(ucRet == RET_ERROR)
		return ucRet;

	pucSrcBuffer=(unsigned char *)CTaskWorkBuffer+16;							//利用临时缓冲区
	pucRetBuffer=(unsigned char *)CTaskWorkBuffer+16+CON_SEND_BLOCK_MAX_SIZE;	//pucBuffer限制CON_SEND_BLOCK_MAX_SIZE

	pucSrcBuffer[m++] = 0;
	pucSrcBuffer[m++] = 0;
	pucSrcBuffer[m++] = 0x01;
	pucSrcBuffer[m++] = 0x01;
	
	pucSrcBuffer[m++] = ucPortNo + 1;
	memcpy(pucSrcBuffer+m,stRS485RecvExpainVal.stRelayPara.stDestAddr.ucRelayAddr,6);
	m += 6;
	if(ucFlag == 0)
	{
		if(stRS485RecvExpainVal.ucDataLen != 0)
		{
			pucSrcBuffer[m++] = 3;
			pucSrcBuffer[m++] = stRS485RecvExpainVal.ucDataLen +2;
			pucSrcBuffer[m++] =(unsigned char)stRS485RecvExpainVal.stRelayPara.ulDI;
			pucSrcBuffer[m++] = (unsigned char)(stRS485RecvExpainVal.stRelayPara.ulDI>>8);
			memcpy(pucSrcBuffer+m,stRS485RecvExpainVal.ucDataArea,stRS485RecvExpainVal.ucDataLen);
			m += stRS485RecvExpainVal.ucDataLen;
		}else
		{
			pucSrcBuffer[m++] = 1;
			pucSrcBuffer[m++] = 2;
			pucSrcBuffer[m++] = (unsigned char)(stRS485RecvExpainVal.stRelayPara.ulDI);
			pucSrcBuffer[m++] = (unsigned char)(stRS485RecvExpainVal.stRelayPara.ulDI>>8);
		}
	}
	else
	{
		pucSrcBuffer[m++] = 1;
		pucSrcBuffer[m++] = 2;
		pucSrcBuffer[m++] = (unsigned char)(stRS485RecvExpainVal.stRelayPara.ulDI);
		pucSrcBuffer[m++] = (unsigned char)(stRS485RecvExpainVal.stRelayPara.ulDI>>8);
	}
	Main_SendExplain(CON_TERMSG_DATATRANS,pucSrcBuffer,pucRetBuffer,&usSendLen,&stMainCommInfo,CON_STATUS_NO,CON_STATUS_NO,0x60);			//组帧，加上报文头尾
	Main_FillUpData(pucRetBuffer,usSendLen,ucTaskSour,uclink);		
}*/
unsigned char Fill_DataTrans_UpSendData_Manage(unsigned char ucTaskSour,unsigned char ucMPNo)
{
	unsigned char	ucRet=RET_SUCCESS;
	ucRet=Main_SendComm_DataTrans(ucTaskSour);
	return ucRet;
}
/*
unsigned char Fill_DataTrans_UpSendDataF9_Manage(unsigned char ucTaskSour,unsigned char ucPortNo,unsigned char ucFlag)
{
	unsigned char ucRet = RET_SUCCESS;
	ucRet = Main_SendComm_DataTransF9(ucTaskSour,ucPortNo,ucFlag);
	return ucRet;
}*/

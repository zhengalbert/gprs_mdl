/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : TerMsg.c
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1¡¢    1.00        2012-02-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

unsigned char ClearDataItem(STDATAITEM *pstDataItem)
{
	unsigned char ucRet=RET_SUCCESS;
	if(pstDataItem!=NULL)
	{
		memset(pstDataItem,0,sizeof(STDATAITEM));
		pstDataItem->ucExeRet=CON_ERRCODE_INVALID;
		pstDataItem->usItemLabel=CON_DI_TYPE_INVALID;
	}
	return ucRet;
}

//unsigned char SetDataItemPer(STDATAITEM *pstDataItem, unsigned char ucMPNo,unsigned short usDI,unsigned char ucExeRet,unsigned char *pucDIContent,unsigned char ucDIContentLen,unsigned char ucContentStartPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	if(pstDataItem!=NULL)
//	{
//		pstDataItem->ucMPNo=ucMPNo;
//		pstDataItem->usItemLabel=usDI;
//		pstDataItem->ucExeRet=ucExeRet;
//		if( pucDIContent!=NULL )
//		{
//			if( (ucDIContentLen+ucContentStartPos)<=CON_DI_DATA_LEN )
//			{
//				pstDataItem->usDataLen=ucDIContentLen+ucContentStartPos;
//				memcpy(pstDataItem->ucDataContent+ucContentStartPos,pucDIContent,ucDIContentLen);
//			}else
//				ucRet=RET_ERROR;
//		}else
//		{
//			pstDataItem->usDataLen=0;
//			memset( (pstDataItem->ucDataContent),0,CON_DI_DATA_LEN);
//		}
//	}	
//	return ucRet;
//}

unsigned char SetDataItem(STDATAITEM *pstDestDataItem,STDATAITEM *pstSourDataItem)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned short usDataLenTmp=0,i=0;
	if( pstDestDataItem!=NULL && pstSourDataItem!=NULL )
	{
		pstDestDataItem->ucMPNo			=pstSourDataItem->ucMPNo;
		pstDestDataItem->usItemLabel	=pstSourDataItem->usItemLabel;
		pstDestDataItem->usDataLen		=pstSourDataItem->usDataLen;
		pstDestDataItem->ucExeRet		=pstSourDataItem->ucExeRet;		
		usDataLenTmp					=pstSourDataItem->usDataLen;
		if(usDataLenTmp<=CON_DI_DATA_LEN)
		{
			pstDestDataItem->usDataLen	=pstSourDataItem->usDataLen;
			for(i=0;i<usDataLenTmp;i++)
				pstDestDataItem->ucDataContent[i]=pstSourDataItem->ucDataContent[i];
		}
	}
	return ucRet;
}

unsigned char ClearTerMsg(STTERMSG *pstTerMsg)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	if( pstTerMsg!=NULL )
	{
		memset( pstTerMsg,0,sizeof(STTERMSG) );
		pstTerMsg->ucMsgType	=CON_TERMSG_TYPE_INVALID;
		pstTerMsg->ucMsgSource	=0;
//		pstTerMsg->ucMsgObject	=0;
		pstTerMsg->ucDataItemNum=0;
		for(i=0;i<CON_TERMSG_DI_NUM;i++)
			ClearDataItem( &(pstTerMsg->stDataItem[i]) );
	}
	return ucRet;
}

unsigned char SetTerMsgPer(STTERMSG *pstTerMsg, unsigned char ucSeqNo,
						   unsigned char ucMsgSource,unsigned char ucMsgType)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	if( pstTerMsg!=NULL )
	{
		pstTerMsg->ucSeqNo		=ucSeqNo;
		pstTerMsg->ucMsgType	=ucMsgType;
		pstTerMsg->ucMsgSource	=ucMsgSource;
//		pstTerMsg->ucMsgObject	=ucMsgObject;
		pstTerMsg->ucDataItemNum=0;
		for(i=0;i<CON_TERMSG_DI_NUM;i++)
			ucRet=ClearDataItem( &(pstTerMsg->stDataItem[i]) );
	}
	return ucRet;
}

//unsigned char SetTerMsg(STTERMSG *pstDestMsg,STTERMSG *pstSourMsg)
//{
//	unsigned char ucRet=RET_ERROR,i=0;
//	unsigned char ucDataItemNumTmp=0;
//
//	if(pstDestMsg!=NULL && pstSourMsg!=NULL)
//	{
//		pstDestMsg->ucMsgType			=pstSourMsg->ucMsgType;
//		pstDestMsg->ucMsgSource			=pstSourMsg->ucMsgSource;
//		//pstDestMsg->ucMsgObject			=pstSourMsg->ucMsgObject;
//
//		if(pstSourMsg->ucMsgType == CON_TERMSG_EXTCMD)
//		{
//			memcpy(pstDestMsg,pstSourMsg,sizeof(STTERMSG));
//			ucRet=RET_SUCCESS;
//		}else
//		{
//			ucDataItemNumTmp=pstSourMsg->ucDataItemNum;
//			if(ucDataItemNumTmp<=CON_TERMSG_DI_NUM)
//			{
//				pstDestMsg->ucDataItemNum=ucDataItemNumTmp;
//				for(i=0;i<ucDataItemNumTmp;i++)
//					SetDataItem( &(pstDestMsg->stDataItem[i]),&(pstSourMsg->stDataItem[i]) ); 
//				ucRet=RET_SUCCESS;
//			}
//		}
//	}	
//	return ucRet;
//}

//--¸æ¾¯¼ÇÂ¼²Ù×÷--
unsigned char ClearDataItem_alr(STDATAITEM_ALR *pstDataItem_alr)
{
	unsigned char ucRet=RET_SUCCESS;
	if(pstDataItem_alr!=NULL)
	{
		memset(pstDataItem_alr,0,sizeof(STDATAITEM_ALR));
		pstDataItem_alr->ucExeRet=CON_ERRCODE_INVALID;
		pstDataItem_alr->usItemLabel=CON_DI_TYPE_INVALID;
	}
	return ucRet;
}

unsigned char SetDataItemPer_alr(STDATAITEM_ALR *pstDataItem_alr, unsigned char ucMPNo,unsigned short usDI,unsigned char ucExeRet,unsigned char *pucDIContent,unsigned char ucDIContentLen,unsigned char ucContentStartPos)
{
	unsigned char ucRet=RET_SUCCESS;
	if(pstDataItem_alr!=NULL)
	{
		pstDataItem_alr->ucMPNo=ucMPNo;
		pstDataItem_alr->usItemLabel=usDI;
		pstDataItem_alr->ucExeRet=ucExeRet;
		if( pucDIContent!=NULL )
		{
			if( (ucDIContentLen+ucContentStartPos)<=CON_DI_DATA_LEN_ALR )
			{
				pstDataItem_alr->usDataLen=ucDIContentLen+ucContentStartPos;
				memcpy(pstDataItem_alr->ucDataContent+ucContentStartPos,pucDIContent,ucDIContentLen);
			}else
				ucRet=RET_ERROR;
		}else
		{
			pstDataItem_alr->usDataLen=0;
			memset( (pstDataItem_alr->ucDataContent),0,CON_DI_DATA_LEN_ALR);
		}
	}	
	return ucRet;
}

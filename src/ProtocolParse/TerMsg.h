/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : TerMsg.h
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1¡¢    1.00        2012-02-05    azh     the original version
******************************************************************************/
#ifndef _TERMSG_H_
#define _TERMSG_H_

__EXTERN unsigned char ClearDataItem(STDATAITEM *pstDataItem);
__EXTERN unsigned char ClearDataItem_alr(STDATAITEM_ALR *pstDataItem_alr);
__EXTERN unsigned char ClearTerMsg(STTERMSG *pstTerMsg);
__EXTERN unsigned char SetDataItem(STDATAITEM *pstDestDataItem,STDATAITEM *pstSourDataItem);
__EXTERN unsigned char SetDataItemPer(STDATAITEM *pstDataItem, unsigned char ucMPNo,unsigned short usDI,unsigned char ucExeRet,unsigned char *pucDIContent,unsigned char ucDIContentLen,unsigned char ucContentStartPos);
__EXTERN unsigned char SetDataItemPer_alr(STDATAITEM_ALR *pstDataItem_alr, unsigned char ucMPNo,unsigned short usDI,unsigned char ucExeRet,unsigned char *pucDIContent,unsigned char ucDIContentLen,unsigned char ucContentStartPos);
__EXTERN unsigned char SetTerMsg(STTERMSG *pstDestMsg,STTERMSG *pstSourMsg);
__EXTERN unsigned char SetTerMsgPer(STTERMSG *pstTerMsg, unsigned char ucSeqNo, unsigned char ucMsgSource,unsigned char ucMsgType);

#endif

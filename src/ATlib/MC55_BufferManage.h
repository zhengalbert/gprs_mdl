/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : MC55_BufferManage.h
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1¡¢    1.00        2011-09-23      azh           the original version
******************************************************************************/
#ifndef		_BUFFERMANAGE_H
#define		_BUFFERMANAGE_H

#define  CON_ATCMD_OUT_ENABLE   0xA55A

//#define		FPCLK	11059200				//Ê±ÖÓÆµÂÊ
#include	"MC55_module.h"

__EXTERN void SCIBufferInit(void);
__EXTERN unsigned short SCIBufferGetDataLength(PST_SCI_BUFFER pSciBuffer);
__EXTERN unsigned char  SCIBufferReadByte(PST_SCI_BUFFER pSciBuffer, unsigned short ucOffset , unsigned char* pch);
__EXTERN unsigned short SCIBufferRead(PST_SCI_BUFFER pSciBuffer ,  unsigned short ucOffset , unsigned char* pDstBuf , unsigned short usLen);
__EXTERN unsigned short SCIBufferWrite(PST_SCI_BUFFER pSciBuffer , unsigned char* pSrcBuf , unsigned short usLen);
__EXTERN unsigned char  SCIBufferGetByte(PST_SCI_BUFFER pSciBuffer, unsigned char* pch);
__EXTERN unsigned char  SCIBufferMoveHeadPtr(PST_SCI_BUFFER pSciBuffer , unsigned short usLen);
__EXTERN unsigned short SCIBufferGetSpareLength(PST_SCI_BUFFER pSciBuffer);
__EXTERN void SCIBufferClearAll(PST_SCI_BUFFER pSciBuffer);
__EXTERN void Gprs_send_frame(unsigned char* pstr, unsigned short usLen);
__EXTERN void debug_come_send_frame(unsigned char* pstr, unsigned short usLen);
__EXTERN void debug_handle_interval(void);
#endif



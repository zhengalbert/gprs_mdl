/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : MC55_SMS_Manage.h
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1¡¢    1.00        2011-09-23      azh           the original version
******************************************************************************/
#ifndef		_SMS_MANAGE_H

#define		_SMS_MANAGE_H

#include	"MC55_module.h"

#include	"MC55_AT_TASK.H"

__EXTERN void ByteToHexAssiic(unsigned char* pDstStr , unsigned char ucSrcData);
__EXTERN unsigned char  AssiicHexToByte(unsigned char* pSrcStr , unsigned char* pucDstData);
__EXTERN unsigned char  SMS_Encode_PhoneNumber(unsigned char* pDst , unsigned char* pSrc , unsigned char ucLen,unsigned char  ucJump);
__EXTERN unsigned char  SMS_Decode_PhoneNumber(unsigned char* pSrc , unsigned char* pDst , unsigned char ucLen, unsigned char ucAnd);
__EXTERN unsigned short AT_SMS_Encode(PSTTC35TASK pstDataContent);
__EXTERN unsigned char  AT_SMS_Decode(unsigned char* pSrc, unsigned char* pDst, PST_SMS_PARAM pSmsCode);
__EXTERN void ByteToBCDAssiic(unsigned char* pDstStr , unsigned char ucSrcData);

#endif

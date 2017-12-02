/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : DI_Info_Oper.h
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1¡¢    1.00        2012-08-15    azh     the original version
******************************************************************************/

#ifndef _DI_INFO_OPER_H_
#define _DI_INFO_OPER_H_

__EXTERN unsigned char ClearDIInfo(STDIINFO *pstDIInfo);
__EXTERN unsigned char SetDIInfo(STDIINFO *pstDIInfo,
						unsigned char ucHave,unsigned char ucType,unsigned char ucMPType,
						unsigned char ucSetEnable,unsigned char ucSetPurView,
						unsigned char ucGetEnable,unsigned char ucReadMeterEnable,
						unsigned char ucContentLen,unsigned char ucCommandLen,unsigned short usBigDI);
__EXTERN unsigned char Is_DI_Available(STDIINFO *pstDIInfo);

#endif

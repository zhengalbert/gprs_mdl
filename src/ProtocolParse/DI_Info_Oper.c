/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : DI_Info_Oper.c
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1¡¢    1.00        2012-08-15    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

unsigned char ClearDIInfo(STDIINFO *pstDIInfo)
{
	if( pstDIInfo!=NULL )
	{
		memset(pstDIInfo,0,sizeof(STDIINFO));
		pstDIInfo->ucHave	=CON_DI_NOHAVE;
		pstDIInfo->ucCommandLen =0;
		pstDIInfo->ucContentLen   =0;
		return RET_SUCCESS;
	}
	return RET_ERROR;
}
unsigned char SetDIInfo(STDIINFO *pstDIInfo,
						unsigned char ucHave,unsigned char ucType,unsigned char ucMPType,
						unsigned char ucSetEnable,unsigned char ucSetPurView,
						unsigned char ucGetEnable,unsigned char ucReadMeterEnable,
						unsigned char ucContentLen,unsigned char ucCommandLen,unsigned short usBigDI)
{
	if( pstDIInfo!=NULL )
	{
		pstDIInfo->ucHave=ucHave;
								
		pstDIInfo->ucType=ucType;
		pstDIInfo->ucMPType=ucMPType;
		
		pstDIInfo->ucSetEnable=ucSetEnable;
		pstDIInfo->ucPurView=ucSetPurView;
		
		pstDIInfo->ucGetEnable=ucGetEnable;
		pstDIInfo->ucReadMeterEnabel=ucReadMeterEnable;
		
		pstDIInfo->ucContentLen=ucContentLen;
		pstDIInfo->ucCommandLen=ucCommandLen;

		pstDIInfo->usBigDI=usBigDI;
		return RET_SUCCESS;
	}
	return RET_ERROR;
}

unsigned char Is_DI_Available(STDIINFO *pstDIInfo)
{
	if(pstDIInfo->ucHave == CON_DI_NOHAVE)
		return RET_ERROR;
	else
		return RET_SUCCESS;
}

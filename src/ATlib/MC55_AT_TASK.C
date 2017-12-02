/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : MC55_AT_TASK.c
 Description    : Ö÷º¯Êý
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1¡¢    1.00        2011-09-23      azh           the original version
******************************************************************************/
#include "..\inc\global_config.h"

#include "MC55_AT_TASK.H"
#include "MC55_module.h"


const	char STR_DEBUG_DATA_LOSE[]={"\n send data losed ........................................... "};

unsigned char AT_CMD_GetLength()
{
	unsigned char ucLen = 0;
	
	if( stATCmdList.ucTailPtr >= stATCmdList.ucHeadPtr)
		ucLen = stATCmdList.ucTailPtr - stATCmdList.ucHeadPtr;
	else
		ucLen = stATCmdList.ucTailPtr + CON_AT_CMD_DEPTH - stATCmdList.ucHeadPtr;
	
	return ucLen;
}

//unsigned char AT_CMD_Fill(unsigned short usAtCmd, unsigned char ucCmdParam)
unsigned char AT_CMD_Fill(unsigned short usAtCmd, unsigned short ucCmdParam)
{
	unsigned char ucLen = 0;
	
	ucLen = AT_CMD_GetLength();
	if(ucLen >= CON_AT_CMD_DEPTH-1)	//Âú
	{
		return 0;
	}
	stATCmdList.ucATCMD[stATCmdList.ucTailPtr] = usAtCmd;
	stATCmdList.ucCmdParam[stATCmdList.ucTailPtr] = ucCmdParam;
	stATCmdList.ucTailPtr++;
	
	if(stATCmdList.ucTailPtr >= CON_AT_CMD_DEPTH)
		stATCmdList.ucTailPtr = 0;
	return 1;
}

unsigned char AT_CMD_Get(unsigned short* pusAtCmd)
{
	unsigned char ucRet = 0;

	unsigned char ucLen = 0;
	
	ucLen = AT_CMD_GetLength();
	if(ucLen == 0)	//¿Õ
	{
		return 0xff;
	}
	*pusAtCmd = stATCmdList.ucATCMD[stATCmdList.ucHeadPtr];

	ucRet = stATCmdList.ucHeadPtr;
	
	stATCmdList.ucHeadPtr++;
	
	if(stATCmdList.ucHeadPtr >= CON_AT_CMD_DEPTH)
		stATCmdList.ucHeadPtr = 0;

	return ucRet;
}

void AT_CMD_Clear()
{
	stATCmdList.ucHeadPtr = stATCmdList.ucTailPtr =0;
}

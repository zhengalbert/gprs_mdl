/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : MC55_AT_TASK.h
 Description    : 主函数
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2011-09-23      azh           the original version
******************************************************************************/
#ifndef		_AT_TASK_H
#define		_AT_TASK_H

#include "MC55_module.h"

typedef struct 
{
	unsigned char	ucHeadPtr;		//头指针
	unsigned char	ucTailPtr;		//尾指针
	unsigned short	ucATCMD[CON_AT_CMD_DEPTH];		//AT命令
//	unsigned char	ucCmdParam[CON_AT_CMD_DEPTH];	//命令附加参数针对CMGD命令
	unsigned short	ucCmdParam[CON_AT_CMD_DEPTH];	//命令附加参数针对CMGD命令	//azh 110928 为了那几条延时命令 一个字节计数不够 
}
ST_AT_CMD_LIST;
/*
typedef	struct 
{
		unsigned char				ucChNo;				//通道号
		ST_COMMUNICATION_CHANNEL	unCurAddrInfo;		//当前通迅地址
		unsigned short				ucDataLength;		//数据长度
		unsigned char				pData[CON_AT_MAX_SEND_PACKAGE];	//数据缓冲区<256
}ST_DATALIST_CONTENT,*PST_DATALIST_CONTENT;

typedef	struct 
{
	unsigned char	ucHeadPtr;		//头指针
	unsigned char	ucTailPtr;		//尾指针
	ST_DATALIST_CONTENT	stDataContent[CON_AT_DATALIST_DEPTH];
}ST_AT_DATA_LIST;

*/
//----------------------------------------------------------------------------
__EXTERN ST_AT_CMD_LIST					stATCmdList;		//AT命令队列
//----------------------------------------------------------------------------
__EXTERN unsigned char ATDataListPop(void);
__EXTERN unsigned char IsATCmdFinish(void);
//__EXTERN unsigned char ATFillCMD(unsigned short usAtCmd, unsigned char ucCmdParam);
__EXTERN unsigned char AT_CMD_Get(unsigned short* pusAtCmd);
__EXTERN void AT_CMD_Clear(void);
__EXTERN unsigned char AT_CMD_GetLength(void);
  
#endif



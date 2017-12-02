/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : MC55_AT_TASK.h
 Description    : ������
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2011-09-23      azh           the original version
******************************************************************************/
#ifndef		_AT_TASK_H
#define		_AT_TASK_H

#include "MC55_module.h"

typedef struct 
{
	unsigned char	ucHeadPtr;		//ͷָ��
	unsigned char	ucTailPtr;		//βָ��
	unsigned short	ucATCMD[CON_AT_CMD_DEPTH];		//AT����
//	unsigned char	ucCmdParam[CON_AT_CMD_DEPTH];	//����Ӳ������CMGD����
	unsigned short	ucCmdParam[CON_AT_CMD_DEPTH];	//����Ӳ������CMGD����	//azh 110928 Ϊ���Ǽ�����ʱ���� һ���ֽڼ������� 
}
ST_AT_CMD_LIST;
/*
typedef	struct 
{
		unsigned char				ucChNo;				//ͨ����
		ST_COMMUNICATION_CHANNEL	unCurAddrInfo;		//��ǰͨѸ��ַ
		unsigned short				ucDataLength;		//���ݳ���
		unsigned char				pData[CON_AT_MAX_SEND_PACKAGE];	//���ݻ�����<256
}ST_DATALIST_CONTENT,*PST_DATALIST_CONTENT;

typedef	struct 
{
	unsigned char	ucHeadPtr;		//ͷָ��
	unsigned char	ucTailPtr;		//βָ��
	ST_DATALIST_CONTENT	stDataContent[CON_AT_DATALIST_DEPTH];
}ST_AT_DATA_LIST;

*/
//----------------------------------------------------------------------------
__EXTERN ST_AT_CMD_LIST					stATCmdList;		//AT�������
//----------------------------------------------------------------------------
__EXTERN unsigned char ATDataListPop(void);
__EXTERN unsigned char IsATCmdFinish(void);
//__EXTERN unsigned char ATFillCMD(unsigned short usAtCmd, unsigned char ucCmdParam);
__EXTERN unsigned char AT_CMD_Get(unsigned short* pusAtCmd);
__EXTERN void AT_CMD_Clear(void);
__EXTERN unsigned char AT_CMD_GetLength(void);
  
#endif


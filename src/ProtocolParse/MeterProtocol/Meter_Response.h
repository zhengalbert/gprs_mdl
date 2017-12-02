/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Meter_Response.h
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/
#ifndef _METER_RESPONSE_H_
#define	_METER_RESPONSE_H_

/*********************************函数声明***************************************/
//总处理函数
__EXTERN unsigned char Meter_Response_Manage(STRS485TASK *pstRS485RevTask);
//分类处理函数
/*************************************GRTASK返回处理***************************************/
__EXTERN unsigned char GRTask_Response_Manage_RecvOverTime(unsigned char ucMPNo);
__EXTERN unsigned char GRTask_Response_Manage_Relay(unsigned char ucMPNo);
__EXTERN unsigned char GRTask_Response_Manage_Normal(unsigned char ucMPNo);
__EXTERN unsigned char SetRS485TaskPer(STRS485TASK	*pstDestTask,
							  unsigned char ucTaskSource, unsigned char ucSeqNo,
							  unsigned char ucReportType,unsigned short usDataType,
							  unsigned char ucDevNo,STRELAYTASKPARA	*pstRelayTaskPara);
//__EXTERN unsigned char	ClearRS485TaskNor(STRS485TASKNOR *pstDestTask);							  

__EXTERN unsigned char SetRS485Task(STRS485TASK *pstDestTask,STRS485TASK *pstSourTask);
__EXTERN unsigned char ClearRS485Task(STRS485TASK *pstDestTask);
//__EXTERN unsigned char SetRS485TaskBig(STRS485TASK *pstDestTask,STRS485TASK *pstSourTask);
//__EXTERN unsigned char ClearRS485TaskBig(STRS485TASK *pstDestTask);
__EXTERN unsigned char Fill_DataTrans_UpSendData_Manage(unsigned char ucTaskSour,unsigned char ucMPNo);

#endif


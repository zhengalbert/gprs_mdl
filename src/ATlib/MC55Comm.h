/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : MC55Comm.h
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2011-09-23      azh           the original version
******************************************************************************/

#ifndef _TC35COMM_H
#define _TC35COMM_H


//此三个常量范围为1－3
#define CON_RCOMMCHAN_1		0		//通道序号		//服务器通道
#define CON_RCOMMCHAN_2		1						//客户端通道
#define CON_RCOMMCHAN_3		2		//固定为短消息
//#define CON_RCOMMCHAN_NUM	2

#define CON_RCOMMTYPE_SMS	1
#define CON_RCOMMTYPE_TCP	2
#define CON_RCOMMTYPE_UDP	3

#define		TC35_TCPDATA_SENDINTER				25			//单位 100ms ，至少2200毫秒
//----------------------------------------------------------------
__EXTERN unsigned short	usLateSmsSeq;
__EXTERN unsigned char		ucPinSetFlag;
__EXTERN unsigned short		usRCommModResetTimer;
__EXTERN unsigned char		ucRCommModResetFlag;
__EXTERN unsigned char 		ucReset_Mc55_Flag;
//----------------------------------------------------------------
__EXTERN unsigned char Init_ModChInfo(STMODPARACHINFO *pstModParaChInfo);
__EXTERN unsigned char Get_ModChInfo(STMODPARACHINFO *pstModParaChInfo);
__EXTERN unsigned char Set_ModChInfo(STMODPARACHINFO *pstModParaChInfo);
__EXTERN void TC35_ClearTask(PSTTC35TASK pstTC35Task);
__EXTERN void TC35_SetTaskPer(STTC35TASK* pstTC35Task,unsigned char ucReportType,unsigned char ucDataType,unsigned char ucChanNo);

__EXTERN PSTTC35TASK ATGetDataContentPtr(unsigned char ucOffset);
__EXTERN unsigned char ATGetDataListLength(void);
__EXTERN unsigned char ATDataListPop(void);

__EXTERN void Reload_Module_RunState(ST_MODULE_RUN_STATE *pstModule_Run_State);

__EXTERN unsigned char Rcomm_Fill_RecvData(unsigned char ucType , unsigned char* pData , unsigned short usLength , PUN_CHANNEL_ADDR pCommunInfo);
__EXTERN unsigned char MC55_Comm_Timer(void);

#endif


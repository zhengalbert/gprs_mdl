/******************************************************************************
 Copyright (C) 2011	 R&D Institute of HuaCai Co.,Ltd.
 Module			: sync data	head file
 File Name		: sync_data.h
 Description	:
 Author			:
 Build Date		:
 others			:
 ------------------------------------------------------------------------------
 Modification History:
 <No.>	<version>		<time>		<author>		<contents>
   2、
   1、	   1.00		  2011-07-17	 azh	   		create
******************************************************************************/

#ifndef	__SYNC_DATA_H
#define	__SYNC_DATA_H

/*-----------------------------	macro definition ----------------------------*/
#define SYNC_TIME_MAX		1		//当前日期及时间
#define SYNC_ENG_MAX		13
#define SYNC_DMD_MAX		13
#define SYNC_MINUTE_MAX		(SYNC_TIME_MAX + SYNC_ENG_MAX + SYNC_ENG_MAX)
#define SYNC_HOUR_MAX		SYNC_MINUTE_MAX
/*-----------------------------	type definition	-----------------------------*/
//typedef	struct
//{
//	U8	   Sn;					// 同步数据项计数器 
//	U8	   MaxNum;				// 总共要同步的数据项数
//}SYNC_CTRL_STRUCT;
//-------------------------- functions declaration ----------------------------
__EXTERN S8 assemble_sync_send_645frame (U32 di);
__EXTERN void sync_sec_data(void);
__EXTERN void sync_minute_data(U8 sec);
#if CODE_VER_TYPE==H_METER_PRJ				//配合高压表
__EXTERN void sync_hour_data(U8 sec);
#endif
__EXTERN void sync_data_init(void);
#ifdef LCD_VALID
__EXTERN U8 dis_ctrl_sync(void);
#endif
//azh 171019
#ifdef ZIGBEE_TEST
U8 check_zigbee_addr_unmatch(U8 *buf);
#endif
//--------------------------- variable declaration ----------------------------
//__EXTERN SYNC_CTRL_STRUCT gSync_Minute;
//__EXTERN SYNC_CTRL_STRUCT gSync_LastEngDmd;
//-----------------------------------------------------------------------------

#endif//__SYNC_DATA_H
/******************************************************************************
							End	Of File
 *****************************************************************************/

/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : event.h
 Description    : 声明事件记录部分的接口函数
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2010-09-27    wwllzz          the original version
******************************************************************************/

#ifndef __EVENT_H
#define __EVENT_H

//----------------------------- macro definition ------------------------------

//----------------------------- type definition -------------------------------

//-------------------------- functions declaration ----------------------------
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不带CPU_GPRS模块
__EXTERN U8 add_program_dix(U8 dix[]);
//__EXTERN void record_energy_zero_event(U8 buf[]);
//__EXTERN void record_demand_zero_event(U8 buf[]);
//__EXTERN void record_event_zero_event(U8 buf[]);
//__EXTERN void record_check_time_event(U8 buf[]);
__EXTERN void record_table_program_event(U8 buf[]);
__EXTERN void record_zone_program_event(U8 buf[]);
//__EXTERN void record_weekday_program_event(U8 buf[], U32 count_addr);
__EXTERN void record_holiday_program_event(U8 buf[]);
__EXTERN void record_account_day_program_event(U8 buf[]);
//__EXTERN void record_event_open_cover(void);
//__EXTERN void record_event_power_down(void);
//__EXTERN void restore_second(void);
//__EXTERN void save_down_time(void);
#endif//CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不带CPU_GPRS模块
//--------------------------- variable declaration ----------------------------

#endif//__EVENT_H

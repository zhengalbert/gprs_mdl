/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : pmu.h
 Description    : LPC17系列芯片硬件电源管理子程序的头文件
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、    2.00        2011-02-25     rosoon
   1、    1.00        2010-09-14     rosoon     the original version
******************************************************************************/

#ifndef __PMU_H
#define __PMU_H

//----------------------------- macro definition ------------------------------
#define __WFI               __wfi                       //睡眠，等待中断指令

#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
#define CLRWDT              SET_PORT0_OUT(P0_WDI);\
                            SET_PORT0_REV(P0_WDI)       //清除硬件看门狗宏指令
#else
#define CLRWDT              SET_PORT1_OUT(P1_WDI);\
                            SET_PORT1_REV(P1_WDI)       //清除硬件看门狗宏指令
#endif                            
#define PWR_ON              0                   //系统上电标识
#define PWR_DN              (1 << 0)                    //系统掉电标识

#define BAT_ON_MXTIME       1800    //unit=100ms 掉电时充电电池最多工作3分钟
#define BAT_LOW_GATE_VOLT   2980    //0xBA4:2.4V[2^12*(2.4/3.3)](对应实际值4.8V) 充电电池最低工作门限电压

//----------------------------- type definition -------------------------------
typedef struct _B_PD_CTRL
{
    U8  last_state;     //上次状态
    U8  cmd_sended;       //数据报文上报发出
    U16  send_time;      //数据发出后倒计时60秒 关模块电源
    U16 dncount;        //充电电池工作倒计时

}B_PD_CTRL;
//-------------------------- functions declaration ----------------------------
__EXTERN void check_power_on(U16 cnt);
__EXTERN void check_power_down (void);
__EXTERN void before_pd_pwr_manage(void);
//--------------------------- variable declaration ----------------------------
__EXTERN B_PD_CTRL gstB_Pd_Ctrl;

#endif//__PMU_H

/******************************************************************************
    End Of File
******************************************************************************/

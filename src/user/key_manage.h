/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : key_manage.h
 Description    : 按键处理程序
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2011-07-03      AZH           the original version
******************************************************************************/

#ifndef __KEY_MANAGE_H
#define __KEY_MANAGE_H

//----------------------------- macro definition ------------------------------
#define CON_CHECK_TIMES		3
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不是CPU_GPRS模块
#define CON_PIN0_INT			(P0_KEYPRG | P0_KEYG | P0_KEYD | P0_KEYUP | P0_KEYDOWN)
#endif
//#define CON_PIN2_INT			(P0_KEYDOWN)

#define CON_KEYBIT_DN		(1<<0)//下翻键
#define CON_KEYBIT_UP		(1<<1)//上翻键
#define CON_KEYBIT_PG		(1<<2)//编程键
#define CON_KEYBIT_UC		(1<<3)//上盖键
#define CON_KEYBIT_DC		(1<<4)//下盖键
#define CON_KEYBIT_NO		0//
//----------------------------- type definition -------------------------------
#define COM_PROG_HOLD_TIME	9000//unit:100ms 15分钟

typedef struct  _key_info
{
	U16 hold_time;		//编程键保持时间
    U8  pre_state;		//上次状态
    U8  dly_cnt;		//状态确认去抖
}key_info;
//-------------------------- functions declaration ----------------------------
__EXTERN void key_init(void);
__EXTERN U8 check_prog_key_state(void);
__EXTERN U8 prog_key_enable(void);
//--------------------------- variable declaration ----------------------------
__EXTERN U8 Key_CurState;
__EXTERN key_info Prog_Ctrl;
#endif//__KEY_MANAGE_H

/******************************************************************************
    End Of File
******************************************************************************/

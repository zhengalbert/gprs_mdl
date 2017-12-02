/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : interface.h
 Description    : 环境接口函数的头文件
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2010-07-19      snap           the original version
******************************************************************************/

#ifndef __INTERFACE_H
#define __INTERFACE_H

//----------------------------- macro definition ------------------------------
//#define CLOSE_INTERFACE_NO_USE_CODE                     //不再编译源码文件中不使用的函数

#define MAX_NVIC        51                              //异常向量表数目

//-----------------------------------------------------------------------------
// 复位模式定义
//-----------------------------------------------------------------------------
#define POWER_RESET     0                               //上电复位
#define HARD_RESET      1                               //硬件复位
#define SOFT_RESET      2                               //软件复位

//----------------------------- type definition -------------------------------

//-------------------------- functions declaration ----------------------------
__EXTERN void chip_reset(U8 mode);

#ifndef CLOSE_INTERFACE_NO_USE_CODE
//__EXTERN S32 isr_set(U32 channel, U32 prio);
//__EXTERN S32 isr_clr(U32 channel);
__EXTERN S32 __swi(1) isr_set(U32 channel, U32 prio);
__EXTERN S32 __swi(2) isr_clr(U32 channel);
__EXTERN void __swi(3) power_mode_shift(void);
__EXTERN void __swi(4) disable_irq(void);
//__EXTERN void __swi(5) enable_irq(void);      //管理模式下关中断 后出来就不能使能中断 所以在管理模式下直接使能
__EXTERN void __swi(5) iap_programe(U32 param_tab[], U32 result_tab[]);
//__EXTERN void __swi(6) enable_user_int(void);
__EXTERN U32 __swi(6) operation_at_admin(U8 id);

//__EXTERN S32 __swi(2) isr_clr(U32);

__EXTERN S32 isr_disable(U32 channel);
__EXTERN S32 isr_enable(U32 channel);
__EXTERN __asm S32 irq_disable(void);
__EXTERN __asm S32 irq_enable(void);
#endif  //CLOSE_INTERFACE_NO_USE_CODE

//--------------------------- variable declaration ----------------------------

#endif//__INTERFACE_H

/******************************************************************************
    End Of File
******************************************************************************/

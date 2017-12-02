/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : timer1.h
 Description    : 定时器1子程序的头文件
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2011-01-22      snap           the original version
******************************************************************************/

#ifndef __TIMER1_H
#define __TIMER1_H

//----------------------------- macro definition ------------------------------
#if CODE_VER_TYPE==WLS_CUR_PRJ			//配合无线采集器
//azh 清溪确认需要比较精确一个时间间隔 所以开了一个定时器2的匹配0做为5ms tick
#define SYS_TICK_ON_T1          (FPCLK_TIMER1 / (1000 / SYS_TICK))      //每个Tick对应的T1计数值
#define SYS_TICK_ON_T2          (FPCLK_TIMER2 / (1000 / SYS_TICK))      //每个Tick对应的T1计数值
#else
//#define SYS_TICK_ON_T1          (FPCLK_TIMER1 / (1000 / SYS_TICK))      //每个Tick对应的T1计数值
#define SYS_TICK_ON_T1          (FPCLK_TIMER1 / (1000 / 5))      //每个Tick对应的T1计数值
#endif
#define T1_MAT_252US            (252 * (FPCLK_TIMER1 / 1000000) + 40)   //252微秒
#define T1_MAT_250US            (250 * (FPCLK_TIMER1 / 1000000) + 40)   //250微秒
#define T1_TIME_2US             (2 * (FPCLK_TIMER1 / 1000000))          //2uS
#define T1_MAT_1S               FPCLK_TIMER1                            //1S时间对应的时钟计数
#define T1_MAT_FAR_INFRARED     (FAR_INFRARED_BIT_WIDTH * (FPCLK_TIMER1 / 1000000) + 40)   //远红外位宽延时

#define MR0_BIT                 0                       //定时器的标识位
#define MR1_BIT                 1
#define MR2_BIT                 2
#define MR3_BIT                 3
#define CR0_BIT                 4
#define CR1_BIT                 5

#define AD1_CR_BIT              CR1_BIT
#define AD2_CR_BIT              CR0_BIT
#define T1_AD1_CR               T1CR1
#define T1_AD2_CR               T1CR0

//----------------------------- type definition -------------------------------

//-------------------------- functions declaration ----------------------------
__EXTERN void TIMER1_IRQHandler(void);
__EXTERN void timer1_init(void);
#if CODE_VER_TYPE==WLS_CUR_PRJ			//配合无线采集器
__EXTERN void timer2_init(void);
#endif
__EXTERN __asm void copy_ssp0_buf(U32 p[], U32 buf[]);

//--------------------------- variable declaration ----------------------------
__EXTERN U32 Last_T1;

#endif//__TIMER1_H

/******************************************************************************
    End Of File
******************************************************************************/

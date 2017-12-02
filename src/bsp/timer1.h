/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : timer1.h
 Description    : ��ʱ��1�ӳ����ͷ�ļ�
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��    1.00        2011-01-22      snap           the original version
******************************************************************************/

#ifndef __TIMER1_H
#define __TIMER1_H

//----------------------------- macro definition ------------------------------
#if CODE_VER_TYPE==WLS_CUR_PRJ			//������߲ɼ���
//azh ��Ϫȷ����Ҫ�ȽϾ�ȷһ��ʱ���� ���Կ���һ����ʱ��2��ƥ��0��Ϊ5ms tick
#define SYS_TICK_ON_T1          (FPCLK_TIMER1 / (1000 / SYS_TICK))      //ÿ��Tick��Ӧ��T1����ֵ
#define SYS_TICK_ON_T2          (FPCLK_TIMER2 / (1000 / SYS_TICK))      //ÿ��Tick��Ӧ��T1����ֵ
#else
//#define SYS_TICK_ON_T1          (FPCLK_TIMER1 / (1000 / SYS_TICK))      //ÿ��Tick��Ӧ��T1����ֵ
#define SYS_TICK_ON_T1          (FPCLK_TIMER1 / (1000 / 5))      //ÿ��Tick��Ӧ��T1����ֵ
#endif
#define T1_MAT_252US            (252 * (FPCLK_TIMER1 / 1000000) + 40)   //252΢��
#define T1_MAT_250US            (250 * (FPCLK_TIMER1 / 1000000) + 40)   //250΢��
#define T1_TIME_2US             (2 * (FPCLK_TIMER1 / 1000000))          //2uS
#define T1_MAT_1S               FPCLK_TIMER1                            //1Sʱ���Ӧ��ʱ�Ӽ���
#define T1_MAT_FAR_INFRARED     (FAR_INFRARED_BIT_WIDTH * (FPCLK_TIMER1 / 1000000) + 40)   //Զ����λ����ʱ

#define MR0_BIT                 0                       //��ʱ���ı�ʶλ
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
#if CODE_VER_TYPE==WLS_CUR_PRJ			//������߲ɼ���
__EXTERN void timer2_init(void);
#endif
__EXTERN __asm void copy_ssp0_buf(U32 p[], U32 buf[]);

//--------------------------- variable declaration ----------------------------
__EXTERN U32 Last_T1;

#endif//__TIMER1_H

/******************************************************************************
    End Of File
******************************************************************************/

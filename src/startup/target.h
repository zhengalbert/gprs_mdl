/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : target.h
 Description    : the specific codes for LPC1700 target boards
                  every project should include a copy of this file
                  user may modify it as needed
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��    1.00        2010-07-22      snap           the original version
******************************************************************************/

#ifndef __TARGET_H
#define __TARGET_H

//----------------------------- macro definition ------------------------------

//-----------------------------------------------------------------------------
// ϵͳʱ������
//-----------------------------------------------------------------------------
#define FOSC                4096000                                     //����Ƶ��

#define PLL_NVALUE          1                                           //1~256
#define PLL_MVALUE          45                                          //6~512
#define FCCO                (2 * FOSC * PLL_MVALUE) / PLL_NVALUE        //368.640MHz, PLLƵ��(275Mhz~550Mhz)

#define FCCLK_DIV           4                                           //CCLKCFG, ��Ƶϵ��
#define FCCLK               (FCCO / FCCLK_DIV)                          //92.160MHz, ��ʱ��Ƶ��(<=100Mhz)

#define ONE_CPU_TIME        (1000000000000 / FCCLK)                     //����һ��������ʱ�����ڣ���λpS��

//1-->1��Ƶ��2-->2��Ƶ��0-->4��Ƶ��3-->8��Ƶ��û���õ�������Ĭ��4��Ƶ
#define FPCLK_TIMER0_DIV    2
#define FPCLK_TIMER0        (FCCLK / 2)                                 //46.080MHz, ����ʱ��Ƶ��FPCLK��FCCLK��1/2
#define FPCLK_TIMER1_DIV    1
#define FPCLK_TIMER1        (FCCLK / 1)                                 //92.160MHz, ����ʱ��Ƶ��FPCLK��FCCLK��ͬ
#define FPCLK_UART0_DIV     2
#define FPCLK_UART0         (FCCLK / 2)                                 //46.080MHz, ����ʱ��Ƶ��FPCLK��FCCLK��1/2
#define FPCLK_UART1_DIV     2
#define FPCLK_UART1         (FCCLK / 2)                                 //46.080MHz, ����ʱ��Ƶ��FPCLK��FCCLK��1/2
#define FPCLK_SSP1_DIV      2
#define FPCLK_SSP1          (FCCLK / 2)                                 //46.080MHz, ����ʱ��Ƶ��FPCLK��FCCLK��1/2

#define FPCLK_I2C0_DIV      3
#define FPCLK_I2C0          (FCCLK / 8)                                 //11.520MHz, ����ʱ��Ƶ��FPCLK��FCCLK��1/8
#define FPCLK_I2C1_DIV      3
#define FPCLK_I2C1          (FCCLK / 8)                                 //11.520MHz, ����ʱ��Ƶ��FPCLK��FCCLK��1/8
#define FPCLK_SSP0_DIV      2
#define FPCLK_SSP0          (FCCLK / 2)                                 //46.080MHz, ����ʱ��Ƶ��FPCLK��FCCLK��1/2
#define FPCLK_TIMER2_DIV    1
#define FPCLK_TIMER2        (FCCLK / 1)                                 //92.160MHz, ����ʱ��Ƶ��FPCLK��FCCLK��ͬ
#define FPCLK_UART2_DIV     2
#define FPCLK_UART2         (FCCLK / 2)                                 //46.080MHz, ����ʱ��Ƶ��FPCLK��FCCLK��1/2
#define FPCLK_UART3_DIV     2
#define FPCLK_UART3         (FCCLK / 2)                                 //46.080MHz, ����ʱ��Ƶ��FPCLK��FCCLK��1/2
#define FPCLK_PWM1_DIV      3
#define FPCLK_PWM1          (FCCLK / 8)                                 //11.520MHz, ����ʱ��Ƶ��FPCLK��FCCLK��1/8
#define FPCLK_AD_DIV        3
#define FPCLK_AD            (FCCLK / 8)                                 //11.520MHz, ����ʱ��Ƶ��FPCLK��FCCLK��1/8

#define PCLKSEL0_INIT       ((FPCLK_SSP1_DIV << 20) |   \
                             (FPCLK_UART1_DIV << 8) |   \
                             (FPCLK_UART0_DIV << 6) |   \
                             (FPCLK_TIMER1_DIV << 4)|   \
                             (FPCLK_I2C0_DIV << 14) |   \
                             (FPCLK_PWM1_DIV << 12) |   \
                             (FPCLK_AD_DIV << 24) |     \
                             (FPCLK_TIMER0_DIV << 2)    \
                            )

#define PCLKSEL1_INIT       ((FPCLK_UART3_DIV << 18) |  \
                             (FPCLK_UART2_DIV << 16) |  \
                             (FPCLK_TIMER2_DIV << 12) | \
                             (FPCLK_SSP0_DIV << 10) |   \
                             (FPCLK_I2C1_DIV << 6)      \
                            )

//-------------------------- functions declaration ----------------------------
__EXTERN void default_vector_handle(void);
__EXTERN void target_init(void);

#endif//__TARGET_H

/******************************************************************************
    End Of File
******************************************************************************/

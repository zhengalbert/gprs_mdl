/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 Module         : pd_manage.h头文件
 File Name      : pd_manage.h
 Description    :
 Author         :
 Build Date     :
 others         :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、     1.00       2011-08-15     azh            create
******************************************************************************/

#ifndef __PD_MANAGE_H
#define __PD_MANAGE_H

#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//带CPU_GPRS模块
__EXTERN void deep_pd_loop (void);
#else
/*----------------------------- macro definition ----------------------------*/
#define CON_CLOSE_VLCD             0
#define CON_UNCLOSE_VLCD           1

#define PD_MODE_SLEEP              0                   //PCON掉电标识位:睡眠
#define PD_MODE_DSLEEP             1                   //PCON掉电标识位:深度深度
#define PD_MODE_POWDN              2                   //PCON掉电标识位:掉电
#define PD_MODE_DPOWDN             3                   //PCON掉电标识位:深度掉电
//-----------------------------------------------------------------------------
// 系统时钟配置
//-----------------------------------------------------------------------------
#define PD_FOSC                4096000//32768//4096000                                     //振荡器频率

#define PD_FCCLK_DIV           1//1//4                                           //CCLKCFG, 分频系数
#define PD_FCCLK               (PD_FOSC / PD_FCCLK_DIV)                       //92.160MHz, 主时钟频率(<=100Mhz)

#define PD_ONE_CPU_TIME        (1000000000000 / PD_FCCLK)                     //定义一个处理器时钟周期（单位pS）

//1-->1分频，2-->2分频，0-->4分频，3-->8分频；没有用到的外设默认4分频
#define PD_FPCLK_TIMER0_DIV    1
#define PD_FPCLK_TIMER0        (PD_FCCLK / 1)                                 //4.096MHz//46.080MHz, 外设时钟频率FPCLK是FCCLK的1/2
#define PD_FPCLK_TIMER1_DIV    1
#define PD_FPCLK_TIMER1        (PD_FCCLK / 1)                                 //4.096MHz//92.160MHz, 外设时钟频率FPCLK与FCCLK相同
#define PD_FPCLK_UART0_DIV     1
#define PD_FPCLK_UART0         (PD_FCCLK / 1)                                 //4.096MHz//46.080MHz, 外设时钟频率FPCLK是FCCLK的1/2
#define PD_FPCLK_UART1_DIV     1
#define PD_FPCLK_UART1         (PD_FCCLK / 1)                                 //4.096MHz//46.080MHz, 外设时钟频率FPCLK是FCCLK的1/2
#define PD_FPCLK_SSP1_DIV      1
#define PD_FPCLK_SSP1          (PD_FCCLK / 1)                                 //4.096MHz//46.080MHz, 外设时钟频率FPCLK是FCCLK的1/2

#define PD_FPCLK_I2C0_DIV      1
#define PD_FPCLK_I2C0          (PD_FCCLK / 1)                                 //4.096MHz//11.520MHz, 外设时钟频率FPCLK是FCCLK的1/8
#define PD_FPCLK_I2C1_DIV      1
#define PD_FPCLK_I2C1          (PD_FCCLK / 1)                                 //4.096MHz//11.520MHz, 外设时钟频率FPCLK是FCCLK的1/8
#define PD_FPCLK_SSP0_DIV      1
#define PD_FPCLK_SSP0          (PD_FCCLK / 1)                                 //4.096MHz//46.080MHz, 外设时钟频率FPCLK是FCCLK的1/2
#define PD_FPCLK_TIMER2_DIV    1
#define PD_FPCLK_TIMER2        (PD_FCCLK / 1)                                 //4.096MHz//92.160MHz, 外设时钟频率FPCLK与FCCLK相同
#define PD_FPCLK_UART2_DIV     1
#define PD_FPCLK_UART2         (PD_FCCLK / 1)                                 //4.096MHz//46.080MHz, 外设时钟频率FPCLK是FCCLK的1/2
#define PD_FPCLK_UART3_DIV     1
#define PD_FPCLK_UART3         (PD_FCCLK / 1)                                 //4.096MHz//46.080MHz, 外设时钟频率FPCLK是FCCLK的1/2
#define PD_FPCLK_PWM1_DIV      1
#define PD_FPCLK_PWM1          (PD_FCCLK / 1)                                 //4.096MHz//11.520MHz, 外设时钟频率FPCLK是FCCLK的1/8
#define PD_FPCLK_AD_DIV        1
#define PD_FPCLK_AD            (PD_FCCLK / 1)                                 //4.096MHz//11.520MHz, 外设时钟频率FPCLK是FCCLK的1/8

#define PD_PCLKSEL0_INIT     ((PD_FPCLK_SSP1_DIV << 20) |   \
                             (PD_FPCLK_UART1_DIV << 8) |   \
                             (PD_FPCLK_UART0_DIV << 6) |   \
                             (PD_FPCLK_TIMER1_DIV << 4)|   \
                             (PD_FPCLK_I2C0_DIV << 14) |   \
                             (PD_FPCLK_PWM1_DIV << 12) |   \
                             (PD_FPCLK_AD_DIV << 24) |     \
                             (PD_FPCLK_TIMER0_DIV << 2)    \
                            )

#define PD_PCLKSEL1_INIT     ((PD_FPCLK_UART3_DIV << 18) |  \
                             (PD_FPCLK_UART2_DIV << 16) |  \
                             (PD_FPCLK_TIMER2_DIV << 12) | \
                             (PD_FPCLK_SSP0_DIV << 10) |   \
                             (PD_FPCLK_I2C1_DIV << 6)      \
                            )
/******************************************************************************
** 在进入掉电情况下各个PORT口输出初始化值定义，把设置为输出‘1’的IO写到下面的宏，空闲的IO全部置为输出‘0’
******************************************************************************/
#define     PD_P0_INIT_DATA      (P0_PECTL |         \
                                 P0_ALARM_LED |      \
                                 P0_KEYPRG  |        \
                                 P0_SDA_LCD |        \
                                 P0_SCL_LCD          \
                                 )

#define     PD_P1_INIT_DATA      (P1_RLED_GPRS |     \
                                 P1_GLED_GPRS |      \
                                 P1_RLED_WLS |       \
                                 P1_GLED_WLS |       \
                                 P1_ALARM     |      \
                                 P1_FUNOUT    |      \
                                 P1_SW4851    |      \
                                 P1_SW4852    |      \
                                 P1_IR_CTL    |      \
                                 P1_DBG_LED   |      \
                                 P1_CPU_YGP   |      \
                                 P1_CPU_WGP          \
                                 )

#define     PD_P2_INIT_DATA      (0)

#define     PD_P3_INIT_DATA      (0)

#define     PD_P4_INIT_DATA      (P4_GPRS_RXD1 |    \
                                  P4_GPRS_TXD1      \
                                 )
/******************************************************************************
** 在进入掉电情况下 各个PORT口方向初始化值定义，把设置为输入的IO写到下面的宏，空闲的IO全部置为输出
******************************************************************************/
#define     PD_P0_INIT_DIR         (~(  P0_KEYG |         \
                                        P0_KEYD |         \
                                        P0_KEYUP |        \
                                        P0_KEYPRG|        \
                                        P0_BAT1_AD|       \
                                        P0_BAT_AD |       \
                                        P0_BAT2_AD|       \
                                        P0_PD_CHK |       \
                                        P0_KEYDOWN        \
                                     )                    \
                                   )

#define     PD_P1_INIT_DIR        (0xffffffff)

#define     PD_P2_INIT_DIR        ( ~(P2_ISP_EN  |        \
                                      P2_IRRXD            \
                                     )                    \
                                  )

#define     PD_P3_INIT_DIR         (0xffffffff)

#define     PD_P4_INIT_DIR         (0xffffffff)
//
//外部掉电后 做个标志 便于复位后进入掉电模式后要先轮显一遍 标志保存在CPU RTC备份RAM里
#define COP_PD_1ST_DIS          1 << 0  //RTCGPREG0的bit0位

//掉电模式
#define CON_PD_MODE_NO          0
#define CON_PD_MODE_YES         0xa1

/*----------------------------- type definition -----------------------------*/


/*-------------------------- functions declaration --------------------------*/
__EXTERN void pd_target_init(void);
__EXTERN void pd_pin_init(U8 flag);
__EXTERN S32 pd_main(void);
//__EXTERN U8 is_pd_mode(void);
__EXTERN void pd_lcd_init(U8 mode);
__EXTERN void pd_mode_ctrl(U8 mode);
__EXTERN void deep_pd_loop (void);

/*--------------------------- variable declaration --------------------------*/
//__EXTERN U8  pd_mode_flag;
__EXTERN U8  pd_autodis_finish;
__EXTERN U16 gflash_ct;
#endif
#define CON_CPU_DEPPD_MODE      0xa55a//低功耗模式
#define CON_CPU_NORM_MODE       0x5aa5//正常运行模式

__EXTERN U16 cpu_run_mode(void);
__EXTERN U16 Cpu_Run_Mode;


#endif//__PD_MANAGE_H
/******************************************************************************
                            End Of File
 *****************************************************************************/

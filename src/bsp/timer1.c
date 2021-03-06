/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : timer1.c
 Description    : 定时器1初始化、中断服务子程序
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、    1.10        2011-06-26      azh
   1、    1.00        2011-08-01     rosoon       the original version
******************************************************************************/

#include "..\inc\global_config.h"

//-------------------------------- 常数定义 -----------------------------------

//------------------------------ 私有变量声明 ---------------------------------

//------------------------------ 私有函数声明 ---------------------------------
#pragma push
#pragma Otime
/******************************************************************************
** 函数名称: void TIMER1_IRQHandler(void)
** 函数功能: 定时器1中断服务子程序
** 入口参数: 无
** 返 回 值: 无
** 说    明: 1、T1MR0匹配中断:用于远红外接收位宽定时
**           2、T1MR1匹配中断:用于远红外发送位宽定时
             3、T1MR2匹配中断:用于接收字节超时定时
             4、T1MR3匹配中断:用于发送字节间隔定时
******************************************************************************/
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
void TIMER1_IRQHandler (void)
{
    U32 tmp;

    tmp = T1IR;                                         //读取中断标识

    if(tmp & (1 << MR0_BIT))                            //远红外位宽定时匹配中断(MR0匹配用于数据接收功能)
    {
        T1IR = (1 << MR0_BIT);                          //异步事件必须各自清
//        far_infrared_rx_drv();                          //启动远红外发送处理
    }
    if(tmp & (1 << MR1_BIT))                            //远红外位宽定时匹配中断(MR1匹配用于数据发送功能)
    {
        T1IR = (1 << MR1_BIT);                          //异步事件必须各自清
//小李待处理 发送太占中断资源
//        far_infrared_tx_drv();                          //启动远红外发送处理
    }
    if(tmp & (1 << MR2_BIT))                            //远红外字节接收超时定时匹配中断(MR2匹配用于字节接收超时功能)
    {
        T1IR = (1 << MR2_BIT);                          //异步事件必须各自清
//        far_infrared_rx_timeout();                      //启动远红外字节接收超时处理
    }
    if(tmp & (1 << MR3_BIT))                            //远红外字节发送间隔定时匹配中断(MR3匹配用于字节发送间隔功能)
    {
        T1IR = (1 << MR3_BIT);                          //异步事件必须各自清
//        far_infrared_tx_timing();                       //启动远红外字节发送定时处理
    }
}
#else
void TIMER1_IRQHandler (void)
{
    U32 tmp;

    tmp = T1IR;                                         //读取中断标识

    if(tmp & (1 << MR0_BIT))                            //远红外位宽定时匹配中断(MR0匹配用于数据接收功能)
    {
        T1IR = (1 << MR0_BIT);                          //异步事件必须各自清
        far_infrared_rx_drv();                          //启动远红外发送处理
    }
    if(tmp & (1 << MR1_BIT))                            //远红外位宽定时匹配中断(MR1匹配用于数据发送功能)
    {
        T1IR = (1 << MR1_BIT);                          //异步事件必须各自清
//小李待处理 发送太占中断资源
        far_infrared_tx_drv();                          //启动远红外发送处理
    }
    if(tmp & (1 << MR2_BIT))                            //远红外字节接收超时定时匹配中断(MR2匹配用于字节接收超时功能)
    {
        T1IR = (1 << MR2_BIT);                          //异步事件必须各自清
        far_infrared_rx_timeout();                      //启动远红外字节接收超时处理
    }
    if(tmp & (1 << MR3_BIT))                            //远红外字节发送间隔定时匹配中断(MR3匹配用于字节发送间隔功能)
    {
        T1IR = (1 << MR3_BIT);                          //异步事件必须各自清
        far_infrared_tx_timing();                       //启动远红外字节发送定时处理
    }
}
#endif
#pragma pop

/******************************************************************************
** 函数名称: void timer1_init(void)
** 函数功能: 定时器1初始化子程序
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void timer1_init (void)
{

    PCONP |= 1 << PCTIM1;                           //使能TIMER1时钟
    T1TCR = 0;                                      //停止定时器
    PCLKSEL0 &= ~(0x03u << 4);
    PCLKSEL0 |= (FPCLK_TIMER1_DIV << 4);            //TIMER1外设时钟PCLK = CCLK
    T1PR = 0;                                       //配置预分频计数器不分频
    T1CTCR = 0;                                     //设置为定时器模式
    T1MCR = 0 ;                                     //禁止匹配产生动作
    T1EMR = 0;                                      //禁止外部匹配产生动作
    T1IR = 0x3f;                                    //清中断标识

    T1TCR |= 1;                                     //启动定时器
}

#if CODE_VER_TYPE==WLS_CUR_PRJ			//配合无线采集器 有硬件时钟
/******************************************************************************
** 函数名称: void timer2_init(void)
** 函数功能: 定时器2初始化子程序
** 入口参数: 无
** 返 回 值: 无
** 说    明: 1、MR0:Tick定时5ms匹配中断
**           
******************************************************************************/
void timer2_init (void)
{
    PCONP |= 1 << PCTIM2;                           //使能TIMER2时钟

    T2TCR = 0;                                      //停止定时器

    PCLKSEL1 &= ~(0x03u << 12);
    PCLKSEL1 |= (FPCLK_TIMER2_DIV << 12);                      //TIMER2外设时钟PCLK = CCLK

    T2PR = 0;                                       //配置预分频计数器不分频

    T2CTCR = 0;                                     //设置为定时器模式
    T2MCR = 1;                						//只允许MR0匹配产生中断
    T2CCR = 0;                  
    T2EMR = 0;                                      //禁止匹配产生动作

    T2IR = 0x3f;                                    //清中断标识

	T2MR0 = SYS_TICK_ON_T1;							//5ms
    T2TCR |= 1;                                     //启动定时器
}

#pragma push
#pragma Otime
/******************************************************************************
** 函数名称: void TIMER2_IRQHandler(void)
** 函数功能: 定时器2中断服务子程序
** 入口参数: 无
** 返 回 值: 无
** 说    明: 1、MR0:Tick定时5ms匹配中断
**           
**           
******************************************************************************/
void TIMER2_IRQHandler (void)
{
    U32 tmp;

    tmp = T2IR;                                         //一次读入中断标识

    if(tmp & (1 << MR0_BIT))                                  //CAP2[0]捕获中断
    {
        T2IR = (1 << MR0_BIT);
		    Sys_Tick++;
        T2CCR = 0;                                      //禁止T2CAP2[0]捕获中断，在一次通讯完成后再开启
    }

    T2MR0 = T2TC + SYS_TICK_ON_T1;

}
#pragma pop
#endif
/******************************************************************************
    End Of File
******************************************************************************/

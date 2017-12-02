/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : target.c
 Description    : the specific codes for LPC1700 target boards
                  every project should include a copy of this file
                  user may modify it as needed
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2010-07-16      snap           the original version
******************************************************************************/

#include "..\inc\global_config.h"

//static void start_ad_cpu_clk (void);

/******************************************************************************
** 函数名称: void default_vector_handle(void)
** 函数功能: 默认异常处理程序
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
//void default_vector_handle (void)
//{
//#ifdef __DEBUG
//    while(1);
//#else//__DEBUG
//    chip_reset(HARD_RESET);
//#endif//__DEBUG
//}

/******************************************************************************
** 函数名称: void target_init(void)
** 函数功能: Initialize the target
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void target_init (void)
{
//    AIRCR = (0x05fa << 16) + (PRIO_GROUP_CONFIG << 8);  //配置确认中断优先级组为x个, 子优先级x个
    FLASHCFG = (0x05ul << 12) | 0x003a;                 //Flash访问使用6个CPU时钟，让FLASH在安全区域运行，避免调速错误

    PCLKSEL0 = PCLKSEL0_INIT;                           //初始化外设时钟分频系数，没有用到的外设默认4分频
    PCLKSEL1 = PCLKSEL1_INIT;

    CLKOUTCFG = 0;                                      //关闭时钟输出

    if((PLL0STAT >> 24) == 1)
    {
        PLL0CON = 1;                                    //Enable PLL, disconnected
        PLL0FEED = 0xAA;
        PLL0FEED = 0x55;
    }
    PLL0CON = 0;                                        //Disable PLL, disconnected
    PLL0FEED = 0xAA;
    PLL0FEED = 0x55;                                    //关闭PLL，避免调速错误
    while(PLL0STAT & (3 << 24));                        //等待关闭PLL

//    start_ad_cpu_clk();                                 //
    SCS = (SCS & 0x04) | 0x20;                          //Enable main OSC, 1MHz~20MHz
    while((SCS & (1ul << 6)) == 0);                     //等待振荡器稳定

    CLKSRCSEL = 0x01;                                   //select main OSC as the PLL clock source

    PLL0CFG   = (((PLL_NVALUE - 1) << 16) | (PLL_MVALUE - 1));
    PLL0FEED  = 0xAA;
    PLL0FEED  = 0x55;                                   //设置分频系数

    PLL0CON   = 1;
    PLL0FEED  = 0xAA;
    PLL0FEED  = 0x55;                                   //Enable but disconnect PLL
    while((PLL0STAT & (1ul << 24)) == 0);               //等待PLL使能

    CCLKCFG = FCCLK_DIV - 1;                            //设置系统时钟分频系数

    while(((PLL0STAT & (1ul << 26)) == 0));             //Check lock bit status

    while(((PLL0STAT & 0x00007FFF) != (PLL_MVALUE - 1))
      && (((PLL0STAT & 0x00FF0000) >> 16) != (PLL_NVALUE - 1)));

    PLL0CON  = 3;                                       //Connect the PLL
    PLL0FEED = 0xAA;
    PLL0FEED = 0x55;
    while((PLL0STAT & (1ul << 25)) == 0);               //Wait until the PLL is connected

#if FCCLK <= 20000000
    FLASHCFG = (0x00ul << 12) | 0x003a;                 //Flash访问使用1个CPU时钟，设置存储器加速模块
#endif//FCCLK <= 20000000

#if FCCLK > 20000000 && FCCLK <= 40000000
    FLASHCFG = (0x01ul << 12) | 0x003a;                 //Flash访问使用2个CPU时钟
#endif//FCCLK > 20000000 && FCCLK <= 40000000

#if FCCLK > 40000000 && FCCLK <= 60000000
    FLASHCFG = (0x02ul << 12) | 0x003a;                 //Flash访问使用3个CPU时钟
#endif//FCCLK > 40000000 && FCCLK <= 60000000

#if FCCLK > 60000000 && FCCLK <= 80000000
    FLASHCFG = (0x03ul << 12) | 0x003a;                 //Flash访问使用4个CPU时钟
#endif//FCCLK > 60000000 && FCCLK <= 80000000

#if FCCLK > 80000000 && FCCLK <= 100000000
    FLASHCFG = (0x04ul << 12) | 0x003a;                 //Flash访问使用5个CPU时钟
#endif//FCCLK > 80000000 && FCCLK <= 100000000

#if FCCLK > 100000000
    FLASHCFG = (0x05ul << 12) | 0x003a;                 //Flash访问使用6个CPU时钟
#endif//FCCLK > 100000000
}
/******************************************************************************
** 函数名称: void start_ad_cpu_clk (void)
** 函数功能: 强制启动AD输出CPU时钟功能函数
** 入口参数: 无
** 返 回 值: 无
** 说    明: 当AD因意外停止CPU时钟输出时调用该函数
******************************************************************************/
//void start_ad_cpu_clk (void)
//{
//
//    //初始化SPI总线为通用IO口[CS/SDI/CLK/RST];
//    //复位10us
//    //准备待发数据:写0x000006到CTRLR,大端模式[U32 dat = 0x06000078]
//    //翻转时钟IO脚，安位打出待发数据
//    //检测CPU时钟是否启动
//    //当CPU时钟输出后,还原SPI总线配置
////-----------------------------------------------------------------------------
//    U8 i;
//    U32 dat;
//
//    FIO1PIN |= (P1_CS5463_CLK | P1_CS5463_RST | P1_CS5463_SDI | P1_CS5463_SDO | P1_CS5463_CS1);//设为输出1
//    FIO1DIR |= (P1_CS5463_CLK | P1_CS5463_RST | P1_CS5463_SDI | P1_CS5463_SDO | P1_CS5463_CS1);//设为输出脚
//   /*
//     * 引脚连接配置P1.16 -- P1.31[只改变P1_20(CLK)、P1_22(RST)、P1_24(SDI)、P1_25(CS1)为IO口]
//     */
//    PINSEL3  = (P1_31_FNUC << 30)|(P1_30_FNUC << 28)|(P1_29_FNUC << 26)|(P1_28_FNUC << 24)|
//               (P1_27_FNUC << 22)|(P1_26_FNUC << 20)|(P1_25_GPIO << 18)|(P1_24_GPIO << 16)|
//               (P1_23_FNUC << 14)|(P1_22_GPIO << 12)|(P1_21_FNUC << 10)|(P1_20_GPIO << 8) |
//               (P1_19_FNUC << 6) |(P1_18_FNUC << 4) |(P1_17_FNUC << 2) |(P1_16_FNUC << 0);
////-----------------------------------------------------------------------------
////    CS5463_RST_LOW();                                   //复位, 测试用勿删
////    delay(2);
////    CS5463_RST_HIGH();
////    dat = 0xfffffffeu;                                  //同步
////    CS5463_CS_EN1();
////    for(i = 0; i < 32; i++)
////    {
////        SET_PORT1_LOW(P1_CS5463_CLK);                   //下降沿
////        if(dat & 0x80000000)
////        {
////            SET_PORT1_HIGH(P1_CS5463_SDI);              //发1
////        }
////        else
////        {
////            SET_PORT1_LOW(P1_CS5463_SDI);               //发0
////        }
////        dat <<= 1;
////        __nop();
////        SET_PORT1_HIGH(P1_CS5463_CLK);                  //上升沿
////    }
////    SET_PORT1_LOW(P1_CS5463_CLK);                       //下降沿
////    CS5463_CS_DIS1();
////    SET_PORT1_HIGH(P1_CS5463_CLK);                      //上升沿
////
////    __nop();
////
////    dat = 0x78000004;                                   //关时钟
////    CS5463_CS_EN1();
////    for(i = 0; i < 32; i++)
////    {
////        SET_PORT1_LOW(P1_CS5463_CLK);                   //下降沿
////        if(dat & 0x80000000)
////        {
////            SET_PORT1_HIGH(P1_CS5463_SDI);              //发1
////        }
////        else
////        {
////            SET_PORT1_LOW(P1_CS5463_SDI);               //发0
////        }
////        dat <<= 1;
////        __nop();
////        SET_PORT1_HIGH(P1_CS5463_CLK);                  //上升沿
////    }
////    SET_PORT1_LOW(P1_CS5463_CLK);                       //下降沿
////    CS5463_CS_DIS1();
////    SET_PORT1_HIGH(P1_CS5463_CLK);                      //上升沿
////
////    __nop();
//
//    CS5463_RST_LOW();                                   //复位
//    delay(2);
//    CS5463_RST_HIGH();
//    dat = 0x78000000;                                   //开时钟
//    CS5463_CS_EN1();
//    for(i = 0; i < 32; i++)
//    {
//        SET_PORT1_LOW(P1_CS5463_CLK);                   //下降沿
//        if(dat & 0x80000000)
//        {
//            SET_PORT1_HIGH(P1_CS5463_SDI);              //发1
//        }
//        else
//        {
//            SET_PORT1_LOW(P1_CS5463_SDI);               //发0
//        }
//        dat <<= 1;
//        __nop();
//        SET_PORT1_HIGH(P1_CS5463_CLK);                  //上升沿
//    }
//    SET_PORT1_LOW(P1_CS5463_CLK);                       //下降沿
//    CS5463_CS_DIS1();
//    SET_PORT1_HIGH(P1_CS5463_CLK);                      //上升沿
//    delay(1);
////-----------------------------------------------------------------------------
//   /*
//     * 引脚连接配置P1.16 -- P1.31[只还原P1_20(CLK)、P1_22(RST)、P1_24(SDI)、P1_25(CS1)为IO口]
//     */
//    PINSEL3  = (P1_31_FNUC << 30)|(P1_30_FNUC << 28)|(P1_29_FNUC << 26)|(P1_28_FNUC << 24)|
//               (P1_27_FNUC << 22)|(P1_26_FNUC << 20)|(P1_25_FNUC << 18)|(P1_24_FNUC << 16)|
//               (P1_23_FNUC << 14)|(P1_22_FNUC << 12)|(P1_21_FNUC << 10)|(P1_20_FNUC << 8) |
//               (P1_19_FNUC << 6) |(P1_18_FNUC << 4) |(P1_17_FNUC << 2) |(P1_16_FNUC << 0);
//
//}
/******************************************************************************
    End Of File
******************************************************************************/

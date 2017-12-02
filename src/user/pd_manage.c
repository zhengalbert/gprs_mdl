/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : pd_manage.c
 Description    : 掉电运行程序所有用到的函数
-------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2011-08-20    azh          the original version
******************************************************************************/
#include    "..\inc\global_config.h"

#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//带CPU_GPRS模块
void deep_pd_loop (void)
{
}
#else
#define __WFI               __wfi                       //睡眠，等待中断指令

//------------------------------ 私有变量声明 ---------------------------------
U8 pd_cnt=0;
//------------------------------ 私有函数声明 -------------------------------
static void pd_bat_on(void);
/*********************************************************************************************************
** Function name:       void pd_mode_ctrl(U8 mode)
** Descriptions:        设置系统功率控制模式
** input parameters:    mode: 0 - 使系统进入普通睡眠模式;
**                                  1 - 使系统进入深度睡眠模式;
**                                  2 - 使系统进入普通掉电模式;
**                                  3 - 使系统进入深度掉电模式。
** output parameters:   none
** Returned value:      1:  成功
**                      0:  失败
*********************************************************************************************************/
void pd_mode_ctrl (U8 mode)
{
    if (mode == 0){                                             /*  进入普通睡眠模式            */
        PCON = 0x00;
        SCR  = 0x00;
    } else {
        SCR  |= 0x04;
        if (mode == 1){                                         /*  进入深度睡眠模式            */
            PCON = 0x00;
        }
        if (mode == 2){                                         /*  进入掉电模式                */
            PCON = 0x01;
        }
        if (mode == 3){                                         /*  进入深度掉电模式            */
            PCON = 0x03;
        }
    }
    //isrWait();                                                          /*  等待中断唤醒              */
    __WFI();
}
/******************************************************************************
** 函数名称: void rtc_ain_init(void)
** 函数功能: 片上RTC秒增量中断初始化功能函数
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void rtc_ain_init (void)
{
    PCONP |= (1 << PCRTC);                  //使能片上RTC时钟

    RTCCCR  = 0x10;                         //禁止时间计数器
    RTCILR  = 0x03;                         //清除中断
    RTCCIIR = 0x01;                       //仅使能秒增量中断
//    RTCCIIR = 0x02;                         //仅使能分增量中断
    RTCAMR = 0xff;                          //禁止报警
    RTC_AUX = 0x10;                         //清除振荡器停振的标志位
    RTC_AUXEN = 0x0;                        //禁止振荡器停振中断
    RTCCCR  = 0x11;                         //启动RTC
}
/******************************************************************************
** 函数名称: void RTC_IRQHandler(void)
** 函数功能: 片上RTC秒增量中断处理功能函数
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void RTC_IRQHandler (void)
{
    RTCILR = 0x03;                          // 清除中断标志
//    if(PCON & DPDFLAG)                  //若CPU在深度掉电模式下，则执行唤醒序列
//    {
//        cpu_wake_up();
//    }
//    SET_PORT1_REV(P1_DBG_LED);//debug

//    SET_PORT1_REV(P1_GNET_LED);//P1_GDATA_LED
//    pd_cnt++;
    RTCGPREG3++;                            //掉电工况秒时标参考
}
/******************************************************************************
** 函数名称: U8 is_pd_mode(void)
** 函数功能: 判断是否在掉电模式下运行 主要是有些驱动要用到RTX系统的资源 比如邮箱
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
//U8 is_pd_mode (void)
//{
//    /////////////debug!!!
////  return __TRUE;
//
////return __FALSE;
//
//    SET_PORT0_IN(P0_PD_CHK);            //配置p0.29为输入口
//
//    if(pd_mode_flag == CON_PD_MODE_YES)//是掉电模式标志
//    {
//        if(PORT0_IS_HIGH(P0_PD_CHK))   //检测掉电信号输入　如果是有电的　不是错误就是刚来电
//        {
//            __disable_irq();//不跑系统下　要调用管理模式下的函数
//            while(1);
//        }
//        return __TRUE;
//    }
//    else
//    {
//        return __FALSE;
//    }
//}

/******************************************************************************
** 函数名称: void pd_enable_modules_int (void)
** 函数功能: 在进入任务前使能各模块中断
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void pd_enable_modules_int (void)
{
    ENABLE_ISR(NVIC_I2C0, PRIO_I2C0);       //set I2C0 interrupt parameter
    ENABLE_ISR(NVIC_I2C1, PRIO_I2C1);       //set I2C1 interrupt parameter

//    ENABLE_ISR(NVIC_UART0, PRIO_UART0);     //set UART0 interrupt parameter
//    ENABLE_ISR(NVIC_UART1, PRIO_UART1);     //set UART0 interrupt parameter
//    ENABLE_ISR(NVIC_UART2, PRIO_UART2);     //set UART0 interrupt parameter
//    ENABLE_ISR(NVIC_UART3, PRIO_UART3);     //set UART0 interrupt parameter

////    ENABLE_ISR(NVIC_SSP0, PRIO_SSP0);       //set SSP0 interrupt parameter
////    ENABLE_ISR(NVIC_SSP1, PRIO_SSP1);       //set SSP1 interrupt parameter

////    ENABLE_ISR(NVIC_TIMER1, PRIO_TIMER1);   //使能TIMER1中断

//    ENABLE_ISR(NVIC_GP_DMA, PRIO_GPDMA);    //使能DMA

//    ENABLE_ISR(NVIC_PWM1, PRIO_PWM1);       //使能PWM1
//    ENABLE_ISR(NVIC_ADC, PRIO_ADC);         //使能ADC
    ENABLE_ISR(NVIC_RTC, PRIO_RTC);         //使能RTC
    ENABLE_ISR(NVIC_EINT3, PRIO_GPIO);      //使能EINT3中断

}
/******************************************************************************
 Function name:  void pd_data_init(void)
 Author       :  azh
 Description  :  一些数据的初始化

 Input        :  None
 Return       :  None
 *****************************************************************************/
//void pd_data_init (void)
//{
//    U8 i;
//
//    Require_Software_Reset = 0;
//    Power_Down_Flag = 0;
////    Global_Status_Word = 0;
//
//    gflash_ct = 0;
//
//    for(i=0; i<AD_MAX; i++)
//    {
//        Ad_Data[i] = 0;
//    }
//}
/******************************************************************************
** 函数名称: void pd_lcd_init(U8 mode)
** 函数功能: 显示控制及数据初始化 主要掉电情况下 判断间隔为1s
** 入口参数: 无 mode=0:上电初始化 mode=1:参数设置后调用
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void pd_lcd_init (U8 mode)
{
/*
    U8 i;
    U8 rev_buff[5];

    file_read(PAR_LPDISCNT_04000301_00_ADDR, rev_buff, 5);//依次读取关于循显&键显相关参数具体参见PAR_LPDISCNT_04000301_00_ADDR定义处
    for(i=0; i<2; i++)
    {
        Dis_Para.maxn[i] = bcd_to_hex(rev_buff[i * (PAR_KEYDIS_04000305_00_ADDR - PAR_LPDISCNT_04000301_00_ADDR)]);
        if(Dis_Para.maxn[i] == 0xff)//外存数据无效时,取默认值
        {
            Dis_Para.maxn[i] = DEBUG_DIS_MAX_NUM;
        }
        Dis_Ctrl.dis_bsn[i] = 0;
        Dis_Ctrl.exd_lsn[i] = 0;
        Dis_Ctrl.lsn_maxn[i] = 1;
    }
    Dis_Para.dly[DIS_MODE_AUTO] = bcd_to_hex(rev_buff[1]);//
    Dis_Para.dly[DIS_MODE_SKEY] = bcd_to_hex(rev_buff[1]);//键显时间规约未提,暂时与轮显共用
    if(rev_buff[1] == 0xff)//外存数据无效时,取默认值
    {
        Dis_Para.dly[DIS_MODE_AUTO] = DIS_TIME_AUTO/10;
        Dis_Para.dly[DIS_MODE_SKEY] = DIS_TIME_SKEY/10;
    }

    Dis_Ctrl.cur_mode = DIS_MODE_AUTO;
    Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_AUTO];
    Dis_Ctrl.pre_key = CON_KEYBIT_NO;
    Dis_Ctrl.cur_diret = DN_SN_DIRECT;
    if(mode == 0)
    {
        Dis_Ctrl.flash_flag = PON_INT_SN_FLAG;
        if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
        {
            Dis_Ctrl.cur_mode = DIS_MODE_ALLON;
            Dis_Ctrl.t_count = 6;//上电全显示6s
        }
    }
    Dis_Ctrl.lpress_ct = DIS_TIME_STLPKEY;

    Dis_Ctrl.dis_dly_time = 0;
    Dis_Ctrl.had_write = 0;

    get_dis_code(Dis_Ctrl.cur_mode);

    gState_Flash_Flag = 0;
    for(i=0; i<7; i++)
    {
        gMeter_State[i] = 0;//电表状态字
    }
*/
    U8 i;
    U8 rev_buff[5];

    file_read(PAR_LPDISCNT_04000301_00_ADDR, rev_buff, 5);//依次读取关于循显&键显相关参数具体参见PAR_LPDISCNT_04000301_00_ADDR定义处

    if(CON_DIS_PARA_VALID == check_dis_para_valid(DIS_MODE_AUTO))
    {
        Dis_Para.maxn[DIS_MODE_AUTO] = bcd_to_hex(rev_buff[0]);
        Dis_Para.dly[DIS_MODE_AUTO] = bcd_to_hex(rev_buff[1]);//
        if(Dis_Para.dly[DIS_MODE_AUTO] > 30)
        {
           Dis_Para.dly[DIS_MODE_AUTO] = DIS_TIME_AUTO/10;
        }
        Dis_Para.para_valid[DIS_MODE_AUTO] = CON_DIS_PARA_VALID;
    }
    else
    {
       Dis_Para.maxn[DIS_MODE_AUTO] = DEFAULT_AUTO_DIS_MAX_NUM;
       Dis_Para.dly[DIS_MODE_AUTO] = DIS_TIME_AUTO/10;
       Dis_Para.para_valid[DIS_MODE_AUTO] = CON_DIS_PARA_INVALID;
    }

    if(CON_DIS_PARA_VALID == check_dis_para_valid(DIS_MODE_SKEY))
    {
        Dis_Para.maxn[DIS_MODE_SKEY] = bcd_to_hex(rev_buff[PAR_KEYDIS_04000305_00_ADDR - PAR_LPDISCNT_04000301_00_ADDR]);
        Dis_Para.dly[DIS_MODE_SKEY] = DIS_TIME_SKEY/10;
        Dis_Para.para_valid[DIS_MODE_SKEY] = CON_DIS_PARA_VALID;
    }
    else
    {
       Dis_Para.maxn[DIS_MODE_SKEY] = DEFAULT_KEY_DIS_MAX_NUM;
       Dis_Para.dly[DIS_MODE_SKEY] = DIS_TIME_SKEY/10;
       Dis_Para.para_valid[DIS_MODE_SKEY] = CON_DIS_PARA_INVALID;
    }
    for(i=0; i<2; i++)
    {
        Dis_Ctrl.dis_bsn[i] = 0;
        Dis_Ctrl.exd_lsn[i] = 0;
        Dis_Ctrl.lsn_maxn[i] = 1;
        if(Dis_Para.dly[i] == 0)
        {
           Dis_Para.dly[i] = 1;
        }
    }

    Dis_Ctrl.cur_mode = DIS_MODE_AUTO;
    Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_AUTO];
    Dis_Ctrl.pre_key = CON_KEYBIT_NO;
    Dis_Ctrl.cur_diret = DN_SN_DIRECT;
    if(mode == 0)
    {
        Dis_Ctrl.flash_flag = PON_INT_SN_FLAG;
        if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
        {
            Dis_Ctrl.cur_mode = DIS_MODE_ALLON;
            Dis_Ctrl.t_count = 6;//上电全显示6s
        }
    }
    Dis_Ctrl.lpress_ct = DIS_TIME_STLPKEY;

    Dis_Ctrl.dis_dly_time = 0;
    Dis_Ctrl.had_write = 0;

    get_dis_code(Dis_Ctrl.cur_mode);

    gState_Flash_Flag = 0;
    for(i=0; i<7; i++)
    {
        gMeter_State[i] = 0;//电表状态字
    }
}
/******************************************************************************
 Function name:  void pd_lcd_display_task (void)
 Author       :
 Description  :
 Input        :  None
 Return       :  None
 *****************************************************************************/
void pd_lcd_display_task (void)
{
#ifdef LCD_VALID
    lcd_mode_ctrl();//上电全显轮显或键显等模式控制字处理

//    if((gflash_ct % 5) == 0)//闪烁标志在最前面刷新 放后面的话有可能会有停顿
//    {
//      rollback_flash_flag();
//  }
    CLRWDT;
//  lcd_display_flash(gflash_ct);
//    if((Dis_Ctrl.flash_flag & (PON_INT_SN_FLAG | LSN_CHG_SN_FLAG | MOD_CHG_SN_FLAG))//1秒到或者有变化就重填显示数据
//    {
        //每秒都要进来
        fill_dis_data();
        Dis_Ctrl.flash_flag = 0;//清标志
//    }


//    if((gflash_ct % 5) == 0)
//  {
//      if(Dis_Ctrl.had_write == 0)//这一轮100ms写LCD前面已写过 这里就不用再刷新了
//      {
//          fill_dis_flash_data();//0.5秒刷新闪烁标志
//      }
//  }
//  Dis_Ctrl.had_write = 0;

//        os_dly_wait(OS_DLY_100MS);//OS_DLY_500MS
    gflash_ct += 10;//掉电情况是1S进入一次 比原来的100ms的10倍
    if(gflash_ct >= 50)
    {
        gflash_ct = 0;
    }
#endif
}
/******************************************************************************
** 函数名称: void ir_module_enable(void)
** 函数功能: 红外模块使能
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
//void ir_module_enable(void)//红外接收电源
//{
//    IR_VDD_ON();//红外接收电源
//    IO2IntEnf |= P2_IRRXD;                      //P2_IRRXD(p2.11)配置成下降沿输入IO中断
//    IO2IntClr = P2_IRRXD;                       //清除中断标志
//}
/******************************************************************************
** 函数名称: void ir_module_disable(void)
** 函数功能: 红外模块禁止
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void ir_module_disable(void)//红外接收电源
{
    IO2IntEnf &= ~P2_IRRXD;                      //P2_IRRXD(p2.11)不配置成下降沿输入IO中断
    IO2IntClr = P2_IRRXD;                       //清除中断标志
    IR_VDD_OFF();//红外接收电源
}


/******************************************************************************
** 函数名称: void debug_led_flash(U8 num)
** 函数功能: 红外模块禁止
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
//void debug_led_flash(U8 num)
//{
//    U8 i;
//
//    for(i=0;i<num;i++)
//    {
////            SET_PORT1_LOW(P1_GDATA_LED);
//            //GDATA_LED_OFF();
//            GPRS_GLED_ON();
//        delay(1000);
////            SET_PORT1_HIGH(P1_GDATA_LED);
//            //GDATA_LED_ON();
//            GPRS_GLED_OFF();
//        delay(1000);
//        CLRWDT;
//    }
//}

//void debug2_led_flash(U8 num)
//{
//    U8 i;
//
//    for(i=0;i<num;i++)
//    {
//            //SET_PORT1_LOW(P1_GDATA_LED);
////            GDATA_LED_ON();
//        GPRS_GLED_ON();
//        delay(4000);
//            //SET_PORT1_HIGH(P1_GDATA_LED);
////            GDATA_LED_OFF();
//        GPRS_GLED_OFF();
//        delay(4000);
//        CLRWDT;
//    }
//}

//void debug3_led_flash(U8 num)
//{
//    U8 i;
//
//    for(i=0;i<num;i++)
//    {
//            SET_PORT1_LOW(P1_CPU_YGP);
//        delay(4000);
//            SET_PORT1_HIGH(P1_CPU_YGP);
//
//        delay(4000);
//        CLRWDT;
//    }
//}
/******************************************************************************
** 函数名称: void pd_pin_init(U8 flag)
** 函数功能: 引脚初始化函数
**           将初始化所有对用户开发的引脚的配置,完成引脚连接,上下拉电阻设置
**           P0、P1口设置为低速访问模式以兼容老器件
** 入口参数: flag:VLCD开关控制(用于初始化时有选择的VLCD的开关 用于低功耗的处理)
** 返 回 值: 无
** 说    明: 需事先在 lpc1700_pin_cfg.h 头文件中选择好相应管脚的功能
******************************************************************************/
void pd_pin_init (U8 flag)
{
    PCONP |= (1 << PCGPIO);                                                   //使能GPIO时钟

    PINMODE_OD0 = 0;
    PINMODE_OD1 = 0;
    PINMODE_OD2 = 0;
    PINMODE_OD3 = 0;
    PINMODE_OD4 = 0;

    FIO0MASK = 0x00;                                                        //不屏蔽引脚高速功能
    FIO1MASK = 0x00;
    FIO2MASK = 0x00;
    FIO3MASK = 0x00;
    FIO4MASK = 0x00;

    FIO0PIN = PD_P0_INIT_DATA;
    FIO1PIN = PD_P1_INIT_DATA;
    if(flag == CON_CLOSE_VLCD)
    {
        FIO2PIN = PD_P2_INIT_DATA;
    }
    else
    {
       FIO2PIN = PD_P2_INIT_DATA|P2_DIS_CTL;
    }
    FIO3PIN = PD_P3_INIT_DATA;
    FIO4PIN = PD_P4_INIT_DATA;


    FIO0DIR = PD_P0_INIT_DIR;
    FIO1DIR = PD_P1_INIT_DIR;
    FIO2DIR = PD_P2_INIT_DIR;
    FIO3DIR = PD_P3_INIT_DIR;
    FIO4DIR = PD_P4_INIT_DIR;


    /*
     * 引脚连接配置P0.00 -- P0.15
     */
    PINSEL0  = 0;

    /*
     * 引脚连接配置P0.16 -- P0.30
     */
    PINSEL1  = 0;

    /*
     * 引脚连接配置P1.0 -- P1.15
     */
    PINSEL2  = 0;

    /*
     * 引脚连接配置P1.16 -- P1.31
     */
    PINSEL3  = 0;

    /*
     * 引脚连接配置P2.0 -- P2.13
     */
    PINSEL4  = 0;

    /*
     * 引脚连接配置P3.25 -- P3.26
     */
    PINSEL7  = 0;

    /*
     * 引脚连接配置P4.28 -- P4.29
     */
    PINSEL9  = 0;

    PINSEL10 = 0;                                              /*  禁止ETM接口功能             */

    /*
     * 引脚内部上下拉电阻配置
     */

    PINMODE0 = (PIN_NO_PULL << 30) | (PIN_NO_PULL << 28) | (PIN_NO_PULL << 26) | (PIN_NO_PULL << 24) |
               (PIN_NO_PULL << 22) | (PIN_NO_PULL << 20) | (PIN_NO_PULL << 18) | (PIN_NO_PULL << 16) |
               (PIN_NO_PULL << 14) | (PIN_NO_PULL << 12) | (PIN_NO_PULL << 10) | (PIN_NO_PULL << 8) |
               (PIN_NO_PULL << 6) | (PIN_NO_PULL << 4) | (PIN_NO_PULL << 2) | (PIN_NO_PULL << 0);

    PINMODE1 = (PIN_NO_PULL << 28) | (PIN_NO_PULL << 26) | (PIN_NO_PULL << 24) | (PIN_NO_PULL << 22) |
               (PIN_NO_PULL << 20) | (PIN_NO_PULL << 18) | (PIN_NO_PULL << 16) | (PIN_NO_PULL << 14) |
               (PIN_NO_PULL << 12) | (PIN_NO_PULL <<10) | (PIN_NO_PULL << 8) | (PIN_NO_PULL << 6) |
               (PIN_NO_PULL << 4) | (PIN_NO_PULL << 2) | (PIN_NO_PULL << 0);

    PINMODE2 = (PIN_NO_PULL << 30) | (PIN_NO_PULL << 28) | (PIN_NO_PULL << 26) | (PIN_NO_PULL << 24) |
               (PIN_NO_PULL << 22) | (PIN_NO_PULL << 20) | (PIN_NO_PULL << 18) | (PIN_NO_PULL << 16) |
               (PIN_NO_PULL << 14) | (PIN_NO_PULL << 12) | (PIN_NO_PULL << 10) | (PIN_NO_PULL << 8) |
               (PIN_NO_PULL << 6) | (PIN_NO_PULL << 4) | (PIN_NO_PULL << 2) | (PIN_NO_PULL << 0);

    PINMODE3 = (PIN_NO_PULL << 30) | (PIN_NO_PULL << 28) | (PIN_NO_PULL << 26) | (PIN_NO_PULL << 24) |
               (PIN_NO_PULL << 22) | (PIN_NO_PULL << 20) | (PIN_NO_PULL << 18) | (PIN_NO_PULL << 16) |
               (PIN_NO_PULL << 14) | (PIN_NO_PULL << 12) | (PIN_NO_PULL << 10) | (PIN_NO_PULL << 8) |
               (PIN_NO_PULL << 6) | (PIN_NO_PULL << 4) | (PIN_NO_PULL << 2) | (PIN_NO_PULL << 0);

    PINMODE4 = (PIN_NO_PULL << 30) | (PIN_NO_PULL << 28) | (PIN_NO_PULL << 26) | (PIN_NO_PULL << 24) |
               (PIN_NO_PULL << 22) | (PIN_NO_PULL << 20) | (PIN_NO_PULL << 18) | (PIN_NO_PULL << 16) |
               (PIN_NO_PULL << 14) | (PIN_NO_PULL << 12) | (PIN_NO_PULL << 10) | (PIN_NO_PULL << 8) |
               (PIN_NO_PULL << 6)  | (PIN_NO_PULL << 4) | (PIN_NO_PULL << 2) | (PIN_NO_PULL << 0);

//PINMODE0 = 0xaaaaaaaa;
//PINMODE1 = 0xaaaaaaaa;
//PINMODE2 = 0xaaaaaaaa;
//PINMODE3 = 0xaaaaaaaa;
//PINMODE4 = 0xaaaaaaaa;

    PINMODE7 = (PIN_NO_PULL << 20) | (PIN_NO_PULL << 18);

    PINMODE9 = (PIN_NO_PULL << 26) | (PIN_NO_PULL << 24);
}
/******************************************************************************
** 函数名称: pd_pconp_init (void)
** 函数功能: 外设功能初始化：1 表示对应外设使能
**                           0 表示对应外设禁止
** 入口参数: 无
** 返 回 值: 无
** 说    明: 需事先在 lpc1700_pin_cfg.h 头文件中选择好相应外设的使能
******************************************************************************/
void pd_pconp_init (void)
{
    PCONP = ((1<<PCRTC) | (1<<PCGPIO));
}
/******************************************************************************
** 函数名称: U8 pd_check_key_pressed (void)
** 函数功能: 深度掉电模式下 每秒唤醒检查一下有没有按键按下 有按下 则进入掉电显示模式
**                           0 表示对应外设禁止
** 入口参数: 无
** 返 回 值: 无
** 说    明: 需事先在 lpc1700_pin_cfg.h 头文件中选择好相应外设的使能
******************************************************************************/
U8 pd_check_key_pressed (void)
{
    U8 cnt,i;

    SET_PORT0_IN(P0_KEYUP | P0_KEYDOWN);
    SET_PORT2_IN(P2_IRRXD);//检测有没有红外信号

    IR_VDD_ON();//红外接收电源
    cnt = 0;
    while(1)
    {
        if(0 == PORT0_IS_HIGH(P0_KEYUP))
        {
            if(++cnt > 3)
            {
                return __TRUE;
            }
        }
        else
        {
            break;
        }
        delay(1);
    }

    cnt = 0;
    while(1)
    {
        if(0 == PORT0_IS_HIGH(P0_KEYDOWN))
        {
            if(++cnt > 3)
            {
                return __TRUE;
            }
        }
        else
        {
            break;
        }
        delay(1);
    }
//检测有没有红外信号
    cnt = 0;
    delay(10);
    for(i=0;i<10;i++)
    {
        if(0 == PORT2_IS_HIGH(P2_IRRXD))
        {
            if(++cnt > 1)
            {
                IR_VDD_OFF();//红外接收电源
                return __TRUE;
            }
        }
        delay(5);
    }
    IR_VDD_OFF();//红外接收电源
    return __FALSE;
}
/******************************************************************************
** 函数名称: void pd_check_power_on (void)
** 函数功能: 掉电模式下 如果判断到来电 死循环等看门狗复位
**
** 入口参数: 无
** 返 回 值: 无
** 说    明:
******************************************************************************/
void pd_check_power_on (void)
{
    U8  power_ok_cnt;

    power_ok_cnt = 0;

    SET_PORT0_IN(P0_PD_CHK);                    //配置p0.29为输入口

    while(1)
    {
        if(PORT0_IS_HIGH(P0_PD_CHK))        //检测掉电信号输入
        {
            if(++power_ok_cnt > 3)        //连续cnt(=200)次检测到掉电信号为高则认为电源已经稳定
            {
                disable_modules_int();
                while(1);
            }
            CLRWDT;
        }
        else
        {
            break;
        }
        delay(5);                          //延时 这时的处理器时钟是4MHz
    }
}
/******************************************************************************
** 函数名称: S32 pd_check_power_down(void)
** 函数功能: 上电判断外部是否有电
**
** 入口参数: 无
** 返 回 值: __TRUE:外部有电 __FALSE:外部没电
** 说    明:
******************************************************************************/
S32 pd_check_power_down (void)
{
    U8  power_ok_cnt;

    power_ok_cnt = 0;
    SET_PORT0_IN(P0_PD_CHK);                    //配置p0.29为输入口

    while(1)
    {
        if(PORT0_IS_HIGH(P0_PD_CHK))        //检测掉电信号输入
        {
            if(++power_ok_cnt > 3)        //连续3次检测到掉电信号为高则认为电源已经稳定
            {
//                pd_mode_flag = CON_PD_MODE_NO;  //外部有电状态
                return __TRUE;
            }
        }
        else
        {
            power_ok_cnt = 0;
//            pd_mode_flag = CON_PD_MODE_YES; //掉电状态
            return __FALSE;
        }
        CLRWDT;                         //清除硬件看门狗
        delay(2);                          //延时约1.2ms，这时的处理器时钟是4MHz
    }

}
/******************************************************************************
** 函数名称: S32 deep_pd_loop(void)
** 函数功能: 深度掉电处理：上电复位后简单的初始化后 检查外部是否有电
**           如果有电：则直接退出 后续会重新对CPU完全初始化 按全速运行
**           如果没电：则判断有没有按键按下 如果按下：则跳出进入掉电显示状态
**                                          如果没按：则直接进入深度掉电模式 等待唤醒复位
** 入口参数: 无
** 返 回 值: 无
** 说    明: 在汇编主循环main之前调用 为了深度掉电复位缩短时间(可以省去初始化时间4ms)
******************************************************************************/
void deep_pd_loop (void)
{
    U32 temp;

//    disable_modules_int();
    pd_pconp_init();                                       //初始化外设功能
    pd_pin_init(CON_CLOSE_VLCD);                                         //引脚初始化 同时打开IO模块电源
    Cpu_Run_Mode = CON_CPU_DEPPD_MODE;                      //上电默认为低功耗模式

//debug WLS_RST_LOW();//azh 110718 等初始化完及系统跑起来后，在复位拉高

//debug pd_target_init();                                   //初始化目标板

///////debug 测试用 先注释掉
    if(__TRUE == pd_check_power_down())
    {
        return;//外部有电 不用进入低功耗处理模式 后续接着正常初始化
    }
    rtc_ain_init();

//  pd_enable_modules_int();//在初始化任务里去 使能　否则会在任务调度前进入一些中断 导致进入莫名的异常
//深度掉电只要使能一个RTC中断就行了
    ENABLE_ISR(NVIC_RTC, PRIO_RTC);         //使能RTC

    PVDD_OFF();  //部分外设电源
//debug    VLCD_OFF();  //打开液晶5V电源
//debug    BGLED_OFF();//打开液晶背光电源

//debug    ir_module_disable();//红外接收电源

//充电
//debug CHARGE_DIS();

    while(1)
    {
        //深度掉电模式 每秒唤醒 复位
        while(1)
        {
            pd_bat_mange();                     //低功耗模式下电池采样管理

            //检测有没有按键按下
            if(__TRUE == pd_check_key_pressed())
            {
                break;
            }
            if(RTCGPREG0 & COP_PD_1ST_DIS)
            {
                RTCGPREG0 &= ~((U32)COP_PD_1ST_DIS);   //标识清零
                break;
            }
            pd_check_power_on();//外部有电 死循环等硬件看门狗复位

    //        debug2_led_flash(1);

            //debug3_led_flash(3);

            CLRWDT;                         //清除硬件看门狗

            pd_mode_ctrl(PD_MODE_DPOWDN);//唤醒后会自动复位

            rtc_ain_init();

            temp = PCON;
            PCON = temp;                //清掉电模式标识
        }
        while(1)
        {
            pd_main();//掉电模式下 显示处理
            break;
        }
    }
}
/******************************************************************************
** 函数名称: void pd_bat_on (void)
** 函数功能: 掉电情况下是否要打开充电电池 用于停显
** 入口参数: 无
** 返 回 值: 无
** 说    明: 兼容两类电池
******************************************************************************/
void pd_bat_on (void)
{
    U16 vbat;
    U16 gate_volt;   //为了兼容2.4V与4.8V两种类型电池的不同判定门限
    
    vbat = Ad_Data[BAT_GPRS];
    if((vbat >= BAT_48V_MIN) && (vbat < BAT_48V_MAX))//若是4.8V电池
    {
        gate_volt = BAT_LOW_GATE_VOLT;
    }
    else if((vbat >= BAT_24V_MIN) && (vbat < BAT_24V_MAX))//若是2.4V电池
    {
        gate_volt = (BAT_LOW_GATE_VOLT >> 1);
    }
    else        //无电池
    {
        gate_volt = (BAT_LOW_GATE_VOLT >> 1);
    }
    if((vbat >= gate_volt) && (vbat != 0))
    {
        BAT_ON();  //开启GPRS电池，停显电池因硬件设定压差而自动禁用
        PIN_POWERON_CLR;//在掉电的情况关闭GPRS模块电源
    }
    else
    {
        BAT_OFF();  //关闭GPRS电池，切换为停显电池供电
    }
}
/******************************************************************************
** 函数名称: S32 pd_main(void)
** 函数功能: 掉电情况下的　main function
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
S32 pd_main (void)
{
    U32 temp;

    disable_modules_int();
    pconp_init();                                       //初始化外设功能
    pin_init();                                         //引脚初始化 同时打开IO模块电源
    pd_bat_on();//azh 刚初始化管脚是默认关掉的

#ifdef ZIGBEE_TEST//azh171011
    WLS_RST_LOW();//azh 110718 等初始化完及系统跑起来后，在复位拉高
#endif

    pd_target_init();                                   //初始化目标板

//    far_infrared_init();                                //红外功能初始化

//    ad_ain_init();                                      //AD初始化

//    timer2_init();                                      //初始化定时器2

//    timer1_init();                                      //初始化定时器1(承载AD捕获中断任务，应在ad_init()之前调用)
//    ad_prep_init();


    key_init();

    rtc_ain_init();

    pd_enable_modules_int();//在初始化任务里去 使能　否则会在任务调度前进入一些中断 导致进入莫名的异常

//将EEPROM当前所有电量数据重载到RAM中用于显示,解决复位后无表可抄时显示乱码问题
    file_read(ENG_STORE_START_ADDR, gEngArray, ENG_RAM_ARRAY_MAX);

//    PVDD_ON();  //部分外设电源
    VLCD_ON();  //打开液晶5V电源
    BGLED_OFF();//打开液晶背光电源
    PVDD_OFF();  //部分外设电源

    ir_module_disable();//红外接收电源

//充电
    CHARGE_DIS();

    CLRWDT;                         //清除硬件看门狗

    _init_box(Mem_64_Byte, sizeof(Mem_64_Byte), MEM_64_SIZE);           //initialize a memory pool for 64 bytes
    _init_box(Mem_256_Byte, sizeof(Mem_256_Byte), MEM_256_SIZE);        //initialize a memory pool for 256 bytes
    _init_box(Mem_1536_Byte, sizeof(Mem_1536_Byte), MEM_1536_SIZE);     //initialize a memory pool for 1024 bytes
//初始化数据
#ifdef LCD_VALID
    lcd_reset();
    pd_lcd_init(0);

    get_flash_default_flag();
#endif

#ifdef ZIGBEE_TEST
    init_search_data();
#endif
//  debug_led_flash(5);

    pd_autodis_finish = 0;//没有按键则 则循环轮显一遍后 进入深度掉电模式

    while(1)
    {
        //掉电模式 每秒或者按键唤醒
            pd_check_power_on();//外部有电 死循环等硬件看门狗复位
            //100ms调用
            pd_lcd_display_task();

            //软时钟处理
            //红处通信
            //规约处理函数

            CLRWDT;                         //清除硬件看门狗

    //      SET_PORT1_REV(P1_GDATA_LED);//debug

            lcd_pd_mode();

            pd_pconp_init();//要在管脚重新配置之前
            PVDD_OFF();  //部分外设电源
            pd_pin_init(CON_UNCLOSE_VLCD); //引脚初始化
            pd_bat_on();//azh 刚初始化管脚是默认关掉的

            if(pd_autodis_finish)
            {
                pd_autodis_finish = 0;
                lcd_dis_off();
                VLCD_OFF();
                break;
            }

            //debug_led_flash(1);

            pd_mode_ctrl(PD_MODE_POWDN);

            disable_modules_int();

            pconp_init();                                       //初始化外设功能
//          pd_pin_init();
            pin_init();                                         //引脚初始化 同时打开IO模块电源
            pd_bat_on();//azh 刚初始化管脚是默认关掉的

            pd_target_init();                                   //初始化目标板
//            PVDD_ON();  //部分外设电源
            rtc_ain_init();

            pd_enable_modules_int();

//          debug2_led_flash(1);

            temp = PCON;
            PCON = temp;                //清掉电模式标识
    }
    return 0;
}
/******************************************************************************
** 函数名称: void pd_target_init(void)
** 函数功能: 掉电:时Initialize the target
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void pd_target_init (void)
{
//    AIRCR = (0x05fa << 16) + (PRIO_GROUP_CONFIG << 8);  //配置确认中断优先级组为x个, 子优先级x个
    FLASHCFG = (0x05ul << 12) | 0x003a;                 //Flash访问使用6个CPU时钟，让FLASH在安全区域运行，避免调速错误

    PCLKSEL0 = PD_PCLKSEL0_INIT;                           //初始化外设时钟分频系数，没有用到的外设默认4分频
    PCLKSEL1 = PD_PCLKSEL1_INIT;

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

//内部晶振不要待稳定
    SCS = (SCS & 0x04);                          //disable main OSC, 1MHz~20MHz
    CLKSRCSEL = 0;//0;//0x02;                                   //0:inter RC; 0x01:main_osc 0x02:select RTC as CPU clock source

//外部晶振要等待稳定后
//    SCS = (SCS & 0x04) | 0x20;                          //Enable main OSC, 1MHz~20MHz
//    while((SCS & (1ul << 6)) == 0);                     //等待振荡器稳定
//  CLKSRCSEL = 1;//0;//0x02;                                   //0:inter RC; 0x01:main_osc 0x02:select RTC as CPU clock source

//    PLL0CFG   = (((PLL_NVALUE - 1) << 16) | (PLL_MVALUE - 1));
//    PLL0FEED  = 0xAA;
//    PLL0FEED  = 0x55;                                   //设置分频系数
//
//    PLL0CON   = 1;
//    PLL0FEED  = 0xAA;
//    PLL0FEED  = 0x55;                                   //Enable but disconnect PLL
//    while((PLL0STAT & (1ul << 24)) == 0);               //等待PLL使能

    CCLKCFG = PD_FCCLK_DIV - 1;                            //设置系统时钟分频系数

//    while(((PLL0STAT & (1ul << 26)) == 0));             //Check lock bit status
//
//    while(((PLL0STAT & 0x00007FFF) != (PLL_MVALUE - 1))
//      && (((PLL0STAT & 0x00FF0000) >> 16) != (PLL_NVALUE - 1)));
//
//    PLL0CON  = 3;                                       //Connect the PLL
//    PLL0FEED = 0xAA;
//    PLL0FEED = 0x55;
//    while((PLL0STAT & (1ul << 25)) == 0);               //Wait until the PLL is connected

#if PD_FCCLK <= 20000000
    FLASHCFG = (0x00ul << 12) | 0x003a;                 //Flash访问使用1个CPU时钟，设置存储器加速模块
#endif//PD_FCCLK <= 20000000

#if PD_FCCLK > 20000000 && PD_FCCLK <= 40000000
    FLASHCFG = (0x01ul << 12) | 0x003a;                 //Flash访问使用2个CPU时钟
#endif//PD_FCCLK > 20000000 && PD_FCCLK <= 40000000

#if PD_FCCLK > 40000000 && PD_FCCLK <= 60000000
    FLASHCFG = (0x02ul << 12) | 0x003a;                 //Flash访问使用3个CPU时钟
#endif//PD_FCCLK > 40000000 && PD_FCCLK <= 60000000

#if PD_FCCLK > 60000000 && PD_FCCLK <= 80000000
    FLASHCFG = (0x03ul << 12) | 0x003a;                 //Flash访问使用4个CPU时钟
#endif//PD_FCCLK > 60000000 && PD_FCCLK <= 80000000

#if PD_FCCLK > 80000000 && PD_FCCLK <= 100000000
    FLASHCFG = (0x04ul << 12) | 0x003a;                 //Flash访问使用5个CPU时钟
#endif//PD_FCCLK > 80000000 && PD_FCCLK <= 100000000

#if PD_FCCLK > 100000000
    FLASHCFG = (0x05ul << 12) | 0x003a;                 //Flash访问使用6个CPU时钟
#endif//PD_FCCLK > 100000000

}
#endif//CODE_VER_TYPE==GPRS_MODULE_PRJ		//不带CPU_GPRS模块

/******************************************************************************
** 函数名称: U16 cpu_run_mode(void)
** 函数功能: 判断是否在掉电模式下运行 主要是有些驱动要用到RTX系统的资源 比如邮箱
**           在上电的时间确定当前运行模式
** 入口参数: 无
** 返 回 值: 运行模式 正常全速模式或者低功耗省电模式
** 说    明: 无
******************************************************************************/
U16 cpu_run_mode (void)
{
    return Cpu_Run_Mode;
}
/******************************************************************************
                            End Of File
 *****************************************************************************/

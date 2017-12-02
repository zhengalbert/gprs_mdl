/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : main.c
 Description    : 主函数
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、    2.00        2011-02-25      rosoon      将睡眠及电源检测独立到pmu.c
   1、    1.00        2010-07-16      snap           the original version
******************************************************************************/

#define  __MAIN_C
/*---------------------------------------------------------------------------*/
#include "..\inc\global_config.h"
#include <RTL.h>

extern void *os_active_TCB[];



extern void debug_led_flash(U8 num);

//-------------------------------- 常数定义 -----------------------------------

//------------------------------ 私有变量声明 ---------------------------------

//------------------------------ 私有函数声明 ---------------------------------
//static void enable_modules_int(void);
static void other_data_init(void);
static void record_reset_source(void);
/******************************************************************************
** 函数名称: void disable_modules_int (void)
** 函数功能: 在初始化各模块前关闭模块中断
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void disable_modules_int (void)
{
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不是CPU_GPRS模块
    DISABLE_ISR(NVIC_I2C0);             //disbale I2C0 interrupt
    DISABLE_ISR(NVIC_SSP0);             //disbale SSP0 interrupt
#endif
    DISABLE_ISR(NVIC_I2C1);             //disbale I2C1 interrupt

    DISABLE_ISR(NVIC_UART0);            //disbale UART0 interrupt
    DISABLE_ISR(NVIC_UART1);            //disbale UART1 interrupt
    DISABLE_ISR(NVIC_UART2);            //disbale UART2 interrupt
    DISABLE_ISR(NVIC_UART3);            //disbale UART3 interrupt


    DISABLE_ISR(NVIC_SSP1);             //disbale SSP1 interrupt

    DISABLE_ISR(NVIC_TIMER1);               //禁能TIMER1中断

    DISABLE_ISR(NVIC_EINT0);                //禁能NVIC_EINT0
    DISABLE_ISR(NVIC_RTC);
    DISABLE_ISR(NVIC_EINT3);                //禁能NVIC_EINT3
    DISABLE_ISR(NVIC_GP_DMA);
    DISABLE_ISR(NVIC_PWM1);
    DISABLE_ISR(NVIC_ADC);
}


/******************************************************************************
** 函数名称: void enable_modules_int (void)
** 函数功能: 在进入任务前使能各模块中断
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void enable_modules_int (void)
{
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不是CPU_GPRS模块
    ENABLE_ISR(NVIC_I2C0, PRIO_I2C0);       //set I2C0 interrupt parameter
    ENABLE_ISR(NVIC_SSP0, PRIO_SSP0);       //set SSP0 interrupt parameter
#endif
    
    ENABLE_ISR(NVIC_I2C1, PRIO_I2C1);       //set I2C1 interrupt parameter

    ENABLE_ISR(NVIC_UART0, PRIO_UART0);     //set UART0 interrupt parameter
    ENABLE_ISR(NVIC_UART1, PRIO_UART1);     //set UART0 interrupt parameter
    ENABLE_ISR(NVIC_UART2, PRIO_UART2);     //set UART0 interrupt parameter
    ENABLE_ISR(NVIC_UART3, PRIO_UART3);     //set UART0 interrupt parameter

    ENABLE_ISR(NVIC_SSP1, PRIO_SSP1);       //set SSP1 interrupt parameter

    ENABLE_ISR(NVIC_TIMER1, PRIO_TIMER1);   //使能TIMER1中断

    ENABLE_ISR(NVIC_EINT3, PRIO_GPIO);      //使能EINT3中断
    ENABLE_ISR(NVIC_GP_DMA, PRIO_GPDMA);    //使能DMA

    ENABLE_ISR(NVIC_PWM1, PRIO_PWM1);       //使能PWM1
    ENABLE_ISR(NVIC_ADC, PRIO_ADC);         //使能ADC
//    ENABLE_ISR(NVIC_RTC, PRIO_RTC);         //使能RTC

}

/******************************************************************************
** 函数名称: S32 main(void)
** 函数功能: main function
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
S32 main (void)
{
//    U8 power_ok_cnt;
//    power_ok_cnt = 0;

    disable_modules_int();
    pconp_init();                                       //初始化外设功能
    pin_init();                                         //引脚初始化

#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//CPU_GPRS模块
    Cpu_Run_Mode = CON_CPU_NORM_MODE;                   //正常运行模式
#endif  

#ifdef ZIGBEE_TEST//azh171011 CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不是CPU_GPRS模块
    WLS_RST_LOW();//azh 110718 等初始化完及系统跑起来后，在复位拉高
    #if CODE_VER_TYPE==GPRS_MODULE_PRJ		//CPU_GPRS模块//azh171011
    ZIGBEE_POWER_ON();
    #endif
#endif
    check_power_on(200);                                //系统上电检测(调试时可注释掉)

//  debug_led_flash(2);
    target_init();                                      //初始化目标板

    far_infrared_init();                                //红外功能初始化

    ad_ain_init();                                      //AD初始化

//    timer2_init();                                      //初始化定时器2
//    GPDMA_Init();                                       //初始化GPDMA, 用于SSP0和TIMER2匹配发送和串口发送

    UART_init();                                        //初始化串口参数，必须放在TIMER1初始化之前及DMA、TIMER2初始化后面

    timer1_init();                                      //初始化定时器1(承载AD捕获中断任务，应在ad_init()之前调用)
#if CODE_VER_TYPE==WLS_CUR_PRJ							//配合无线采集器 有硬件时钟
    timer2_init();
#endif
    GPDMA_Init();

    key_init();

    GPRS_TargetInit();//有对硬件的初始化
    SCIBufferInit();
//azh
    enable_modules_int();//在初始化任务里去 使能　否则会在任务调度前进入一些中断 导致进入莫名的异常

#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不是CPU_GPRS模块
    PVDD_ON();  //部分外设电源

    VLCD_ON();  //打开液晶5V电源
    BGLED_ON();//打开液晶背光电源
#endif

    os_sys_init_user(init_task, INIT_TASK_PRIO, &Init_Task_Stack, sizeof(Init_Task_Stack));     /* Initialize RTX and start init */
//    os_sys_init_prio(init_task, INIT_TASK_PRIO);     /* Initialize RTX and start init */
    while(1);
}

/******************************************************************************
 Function name:  __task void init_task (void)
 Author       :  snap.gao
 Description  :  initialize and start RTX kernal
 Input        :  None
 Return       :  None
 *****************************************************************************/
__task void init_task (void)
{
   U8 i;
/*
    os_mut_init(Mut_I2C_Mem);     //initialize MEM I2C1 bus protect mutex
    os_mut_init(Mut_EEPROM);      //initialize EEPROM       protect mutex

    os_mbx_init(Mbx_Load_Profiler, sizeof(Mbx_Load_Profiler));          //initialize load profiler record mailbox
    os_mbx_init(Mbx_Comm_Data, sizeof(Mbx_Comm_Data));                  //initialize communicated data receive mailbox
    os_mbx_init(Mbx_UART_Data, sizeof(Mbx_UART_Data));                  //initialize UART data receive mailbox
    for(i = 0; i < MAX_PORT_NUM; i++)
    {
        os_mut_init(Status_Mechanism[i].Mut_Com_Usable);                //初始化串口资源可用信号量
        os_mbx_init(Status_Mechanism[i].Mbx_Send_Buf, sizeof(Status_Mechanism[i].Mbx_Send_Buf));        //初始化发送队列
        os_mbx_init(Status_Mechanism[i].Mbx_Answer_Buf, sizeof(Status_Mechanism[i].Mbx_Answer_Buf));    //初始化命令返回邮箱
    }
    for(i = 0; i < MAX_SOCKET_NUM; i++)
    {
        os_mut_init(Net_Status_Mechanism[i].Mut_Socket_Usable);         //初始化串口资源可用信号量
        os_mbx_init(Net_Status_Mechanism[i].Mbx_Send_Buf, sizeof(Net_Status_Mechanism[i].Mbx_Send_Buf));        //初始化发送队列
        os_mbx_init(Net_Status_Mechanism[i].Mbx_Answer_Buf, sizeof(Net_Status_Mechanism[i].Mbx_Answer_Buf));    //初始化命令返回邮箱
    }

    _init_box(Mem_64_Byte, sizeof(Mem_64_Byte), MEM_64_SIZE);           //initialize a memory pool for 64 bytes
    _init_box(Mem_256_Byte, sizeof(Mem_256_Byte), MEM_256_SIZE);        //initialize a memory pool for 256 bytes
    _init_box(Mem_1536_Byte, sizeof(Mem_1536_Byte), MEM_1536_SIZE);     //initialize a memory pool for 1024 bytes

    Task_Schedule_Flag = 0;                 //clear task schedule flag
    Task_Schedule_Record = 0;
    Require_Software_Reset = 0;
    Power_Down_Flag = 0;
    Global_Status_Word = 0;
    Meter_LED_Cnt = 0;
    sys_board_init();

    Crawl_Task_ID = os_tsk_create_user(crawl_task, CRAWL_TASK_PRIO, &Crawl_Task_Stack, sizeof(Crawl_Task_Stack));                                               //create crawl task
    Protocol_Analyse_Task_ID = os_tsk_create_user(protocol_analyse_task, PROTOCOL_TASK_PRIO, &Protocol_Task_Stack, sizeof(Protocol_Task_Stack));                //create protocol analyse task
    System_Manage_Task_ID = os_tsk_create_user(system_manage_task, MANAGE_TASK_PRIO, &Manage_Task_Stack, sizeof(Manage_Task_Stack));                            //create system manage task
    UART_Task_ID = os_tsk_create_user(UART_task, UART_TASK_PRIO, &UART_Task_Stack, sizeof(UART_Task_Stack));                                                    //create UART manage task
    TCP_Poll_Task_ID = os_tsk_create_user(tcp_poll_task, TCP_POLL_TASK_PRIO, &TCP_Poll_Stack, sizeof(TCP_Poll_Stack));                                          //create TCP poll task
    Protocol_Status_Task_ID = os_tsk_create_user(protocol_status_task, PROTOCOL_STATUS_TASK_PRIO, &Protocol_Status_Stack, sizeof(Protocol_Status_Stack));       //create IEC1107 protocol status task
    SNTP_Task_ID = os_tsk_create_user(SNTP_task, SNTP_TASK_PRIO, &SNTP_Task_Stack, sizeof(SNTP_Task_Stack));                                                    //create SNTP task
    Tick_Timer_Task_ID = os_tsk_create_user(tick_timer_task, TICK_TIMER_TASK_PRIO, &Tick_Timer_Task_Stack, sizeof(Tick_Timer_Task_Stack));                      //create tick timer task
    Timing_Task_ID = os_tsk_create_user(timing_task, TIMING_TASK_PRIO, &Timing_Task_Stack, sizeof(Timing_Task_Stack));                                          //create timing task，必须放在最后创建
*/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不带CPU_GPRS模块时
    os_mut_init(Mut_I2C0_Mem);     //initialize LCD bu97950 I2C0 bus protect mutex
#endif
    os_mut_init(Mut_I2C1_Mem);     //initialize MEM I2C1 bus protect mutex
    os_mut_init(Mut_EEPROM);      //initialize EEPROM       protect mutex

    os_mut_init(Mut_SSP_EON_Flash); //initialize EONQ64     ssp0 protect mutex  azh140918

    os_mbx_init(Mbx_Comm_Data, sizeof(Mbx_Comm_Data));                  //initialize communicated data receive mailbox
    os_mbx_init(Mbx_UART_Data, sizeof(Mbx_UART_Data));                  //initialize UART data receive mailbox
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不是CPU_GPRS模块
//    os_mbx_init(Mbx_Transmit_Data, sizeof(Mbx_Transmit_Data));                  //initialize UART data receive mailbox
#endif    
    for(i = 0; i < MAX_PORT_NUM; i++)
    {
        os_mut_init(Status_Mechanism[i].Mut_Com_Usable);                //初始化串口资源可用信号量
        os_mbx_init(Status_Mechanism[i].Mbx_Send_Buf, sizeof(Status_Mechanism[i].Mbx_Send_Buf));        //初始化发送队列
        os_mbx_init(Status_Mechanism[i].Mbx_Answer_Buf, sizeof(Status_Mechanism[i].Mbx_Answer_Buf));    //初始化命令返回邮箱
    }
    for(i = 0; i < MAX_TOTPORT_NUM; i++)
    {
        os_mbx_init(stProtocol_Analyse[i].Mbx_Send_HiBuf, sizeof(stProtocol_Analyse[i].Mbx_Send_HiBuf));        //初始化发送队列
        os_mbx_init(stProtocol_Analyse[i].Mbx_Send_LoBuf, sizeof(stProtocol_Analyse[i].Mbx_Send_LoBuf));    //初始化命令返回邮箱
    }


    _init_box(Mem_64_Byte, sizeof(Mem_64_Byte), MEM_64_SIZE);           //initialize a memory pool for 64 bytes
    _init_box(Mem_256_Byte, sizeof(Mem_256_Byte), MEM_256_SIZE);        //initialize a memory pool for 256 bytes
    _init_box(Mem_1536_Byte, sizeof(Mem_1536_Byte), MEM_1536_SIZE);     //initialize a memory pool for 1024 bytes

//    UART_init();                                        //初始化串口参数，必须放在TIMER1初始化之前及DMA、TIMER2初始化后面

    record_reset_source();
    other_data_init();
    sync_data_init();

    Protocol_Analyse_Task_ID = os_tsk_create_user(protocol_analyse_task, PROTOCOL_TASK_PRIO, &Protocol_Task_Stack, sizeof(Protocol_Task_Stack));                //create protocol analyse task
    UART_Task_ID = os_tsk_create_user(UART_task, UART_TASK_PRIO, &UART_Task_Stack, sizeof(UART_Task_Stack));
//    System_Manage_Task_ID = os_tsk_create_user(system_manage_task, MANAGE_TASK_PRIO, &Manage_Task_Stack, sizeof(Manage_Task_Stack));                            //create system manage task
    System_Manage_Task_ID = os_tsk_create(system_manage_task, MANAGE_TASK_PRIO);                            //create system manage task
#ifdef LCD_VALID
    Lcd_Display_Task_ID = os_tsk_create_user(lcd_display_task, LCD_DISPLAY_PRIO, &Lcd_Display_Task_Stack, sizeof(Lcd_Display_Task_Stack));
#endif
//    Transmit_Analyse_Task_ID = os_tsk_create_user(transmit_analyse_task, TRANSMIT_TASK_PRIO, &Transmit_Task_Stack, sizeof(Transmit_Task_Stack));
//    Transmit_Analyse_Task_ID = os_tsk_create(transmit_analyse_task, TRANSMIT_TASK_PRIO);
    Timing_Task_ID = os_tsk_create_user(timing_task, TIMING_TASK_PRIO, &Timing_Task_Stack, sizeof(Timing_Task_Stack));                                          //create timing task，必须放在最后创建

    Gprs_Task_ID = os_tsk_create_user(gprs_task, GPRS_TASK_PRIO, &Gprs_Task_Stack, sizeof(Gprs_Task_Stack));                                          //create timing task，必须放在最后创建
//debug
//    TCP_Poll_Task_ID = os_tsk_create_user(debug_task, TCP_POLL_TASK_PRIO, &TCP_Poll_Stack, sizeof(TCP_Poll_Stack));
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不是CPU_GPRS模块
//    TCP_Poll_Task_ID = os_tsk_create(debug_task, TCP_POLL_TASK_PRIO);
#endif
    os_tsk_delete_self();                   //end of init task
}
/******************************************************************************
 Function name:  void record_reset_source(void);
 Author       :  snap.gao
 Description  :
                 注意，部分初始化模块有先后顺序，不得随意调整顺序
 Input        :  None
 Return       :  None
 *****************************************************************************/
void record_reset_source (void)
{
    if(RSID&(1<<2))                                     // 复位源判断和处理, 内部看门狗复位
    {
        Reset_Source = MCU_RST_WDTR;
    }
    else if(RSID&(1<<3))                                // BOD复位
    {
        Reset_Source = MCU_RST_BOD;
    }
    else if(RSID&(1<<1))                                // 复位引脚低电平, 该是外部看门狗复位
    {
        Reset_Source = MCU_RST_EXTR;
    }
    else if(RSID&(1<<0))                                // POR复位
    {
        Reset_Source = MCU_RST_POR;
    }
    else
    {
        Reset_Source = MCR_RST_SOFT;                    // 找不到其它任何复位源, 那就是软件复位
    }
    Reset_Source_Copy = Reset_Source;
    RSID = 0x0F;                                        // 清复位源寄存器标识
}

/******************************************************************************
 Function name:  void other_data_init(void)
 Author       :  azh
 Description  :  其他一些数据的初始化

 Input        :  None
 Return       :  None
 *****************************************************************************/
void other_data_init (void)
{
    U8 i;

    Task_Schedule_Flag = 0;                 //clear task schedule flag
    Task_Schedule_Record = 0;
    Require_Software_Reset = 0;
    Power_Down_Flag = 0;
//    Global_Status_Word = 0;

    for(i=0; i<MAX_PORT_NUM; i++)
    {
        Meter_LED_Cnt[i] = 0;
    }
//#ifdef ZIGBEE_TEST
//    zigbee_led_time[0] = 0;
//    zigbee_led_time[1] = 0;
//#endif

//#ifdef GPRS_MODULE_PRJ
    local_led_time[0] = 0;
    local_led_time[1] = 0;
//#endif    
//    file_read(ENG_STORE_START_ADDR, gEngArray, ENG_RAM_ARRAY_MAX);//放在有些版本会进入hardfault

    for(i=0; i<AD_MAX; i++)
    {
        Ad_Data[i] = 0;
    }

    memset(&gstB_Pd_Ctrl,0,sizeof(B_PD_CTRL));//110308
    gstB_Pd_Ctrl.last_state = PWR_ON;
    gstB_Pd_Ctrl.dncount = BAT_ON_MXTIME;//掉电充电池最长工作时间

    gucZigbee_Disable_Flag = 0;
#ifdef ZIGBEE_TEST
    get_match_addr();//azh 120426
#endif
    gucPoweronGetTimeOver = 0;//上电获取时间标志

#if CODE_VER_TYPE==WLS_CUR_PRJ							//配合无线采集器
    Sys_Tick = 0;
    user_init();
#endif

    gucGprs_Module_Type = TELIT_GPRS_MODULE; //azh 141030
}

//debug
void HardFault_Handler(void)
{
	while(1)
	{
//		ALARM_LED_ON();
	};
}
/******************************************************************************
                            End Of File
******************************************************************************/

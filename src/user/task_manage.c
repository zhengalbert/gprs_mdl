/******************************************************************************
 Copyright (C) 2010	 R&D Institute of HuaCai Co.,Ltd.
 File Name		: task manage function
 Description	: task_manage.c
-------------------------------------------------------------------------------
 Modification History:
 <No.>	<version>		<time>		<author>		<contents>
   2、
   1、	  1.00		  2011-08-17	azh			 the original version
******************************************************************************/
#include	"..\inc\global_config.h"

U32 gtask_error;
U16 gtask_cnt;
//------------------------------ 私有变量声明 ---------------------------------
//------------------------------ 私有函数声明 -------------------------------
/******************************************************************************
Function name: void sign_task_schedule (U8 prio)
Author       : snap.gao
Description  : 函数被每个任务调用，标记任务运行标志
               软件复位或掉电事件发生时，函数尝试删除自身任务
               一些特殊的任务需要清理现场，释放所有资源后再删除自身
               除最高优先级任务外的其它任务都删除后系统复位
               在清理现场耗时很长时，当软件复位请求超过指定时间后强制启动复位
               在清理现场耗时很长时，掉电处理将一直等待直到没电
Input        : prio is priority of the task
Return       : None
******************************************************************************/
void sign_task_schedule (U8 prio)
{

//    U32 i;

    prio &= 0x1f;
    Task_Schedule_Flag |= (1 << prio);
/*
    if((Require_Software_Reset != 0) || (Power_Down_Flag == POWER_DOWN_FLAG))       // 是软件复位或掉电事件
    {
        if(prio == TIMING_TASK_PRIO)                        // 这是运行到最后的任务
        {
            if((Require_Software_Reset == (SOFTWARE_RESET_DELAY / 2)) || (Power_Down_Flag == POWER_DOWN_FLAG))      // 在软件复位时间的一半或掉电事件
            {
                for(i = 0; i < MAX_SOCKET_NUM; i++)
                {
                    if(Net_Socket[i].Type == TCP_SOCKET)                    // 检查socket的类型，TCP连接需要结束
                    {
                        Net_Status_Mechanism[i].Close_Flag = 1;             // 启动结束连接
                    }
                }
                os_evt_set(POWER_DOWN_FLAG, System_Manage_Task_ID);         // 通知所有任务处理复位
                os_evt_set(POWER_DOWN_FLAG, Protocol_Status_Task_ID);
                os_evt_set(POWER_DOWN_FLAG, SNTP_Task_ID);
                os_evt_set(POWER_DOWN_FLAG, Crawl_Task_ID);
                while(os_mbx_send(Mbx_Load_Profiler, NULL, 0) == OS_R_TMO)
                {
                    os_dly_wait(2);
                }
                while(os_mbx_send(Mbx_Comm_Data, NULL, 0) == OS_R_TMO)
                {
                    os_dly_wait(2);
                }
            }
            if(Require_Software_Reset != 0)                 // 是软件复位
            {
                if(++Require_Software_Reset > SOFTWARE_RESET_DELAY)         // 增加并等待软件复位请求延时
                {
                    run_reset_head();                                       // 程序开始重启复位, 实现切换
                }
            }
            tsk_lock();                                     // 若其它任务都已删除, 那就可以复位了
            for(i = 0; i < (Timing_Task_ID - 1); i++)
            {
                if(os_active_TCB[i] != NULL)                // 发现有个任务没有被删除
                {
                    break;
                }
            }
            tsk_unlock();
            if(i == (Timing_Task_ID - 1))                   // 判断是否所有任务都已经删除
            {
                if(Require_Software_Reset != 0)             // 是软件复位
                {
                    run_reset_head();                       // 程序开始重启复位, 实现切换
                }
                else                                        // 是掉电事件
                {
                    VICIntEnClr = 0xffffffff;               // 关中断
                    while(1);                               // 等待看门狗复位
                }
            }
        }
    }
    else
    {
        Task_Schedule_Record |= Task_Schedule_Flag;
        if((Task_Schedule_Record>>1) != ((1<<TICK_TIMER_TASK_PRIO) - 1))        //判断是否所有任务都运行一次
        {
            return;
        }
        if((Global_Status_Word & HOST_CONNECT_FLAG) == 0)   //判断以太网是否连接到主机
        {
            return;
        }
        insure_run_flag_valid();
        return;
    }

    if((Require_Software_Reset >= (SOFTWARE_RESET_DELAY / 2)) || (Power_Down_Flag == POWER_DOWN_FLAG))      // 超过软件复位时间的一半或掉电事件
    {
        switch(prio)
        {
        case UART_TASK_PRIO:
            for(i = 0; i < MAX_PORT_NUM; i++)
            {
                if((Status_Mechanism[i].Send_State != COM_IDLE)
                && (Status_Mechanism[i].Send_State != WAIT_NEXT))
                {
                    return;
                }
            }
            os_tsk_delete_self();                           // 任务不占用资源了可以删除
            break;
        case TICK_TIMER_TASK_PRIO:                          // 不删除自身，被TCP poll任务删除
        case TIMING_TASK_PRIO:                              // 不删除自身，保持到最后
            break;
        case TCP_POLL_TASK_PRIO:
            for(i = 0; i < MAX_SOCKET_NUM; i++)
            {
                if((Net_Status_Mechanism[i].Net_State != WAIT_DATA)
                && (Net_Status_Mechanism[i].Net_State != DHCP_CHECK)
                && (Net_Status_Mechanism[i].Net_State != ZEROCONF_CHECK))
                {
                    return;
                }
            }
            FIO4DIR |= P4_RST_PHY;
            FIO4CLR |= P4_RST_PHY;                          // 复位PHY
            os_tsk_delete(Tick_Timer_Task_ID);
            os_tsk_delete_self();                           // 任务不占用资源了可以删除
        default:
            os_tsk_delete_self();                           // 不占用资源的任务直接删除
            break;
        }
    }
*/
}
/******************************************************************************
 Function name:  __task void system_manage_task (void)
 Author       :
 Description  :
 Input        :  None
 Return       :  None
 *****************************************************************************/
/*
__task void system_manage_task (void)
{
//    U8  mem_64_cnt, mem_256_cnt, mem_1536_cnt;
    static U16 cnt;
//    U32 err;

    gtask_error = 0;
    for(cnt = 0; cnt < 1800; cnt++)                        //3分钟后开始检查
    {
        sign_task_schedule(MANAGE_TASK_PRIO);               //响应系统复位操作
        os_dly_wait(OS_DLY_100MS);
        CLRWDT;
    }

    cnt = 0;
//    mem_64_cnt = 0;
//    mem_256_cnt = 0;
//    mem_1536_cnt = 0;

    for(;;)
    {
//        os_evt_wait_or(POWER_DOWN_FLAG, OS_DLY_1S);         //等待超时处理任务状态检查或掉电事件唤醒
		os_evt_wait_or(POWER_DOWN_FLAG, OS_DLY_500MS);         //等待超时处理任务状态检查或掉电事件唤醒
        sign_task_schedule(MANAGE_TASK_PRIO);

        gtask_error = 0;

//        if(CLKSRCSEL != 0x00000001)     //refer to "CLKSRCSEL_Val" at LPC2300.s
//        {
//            err |= (REGISTER_ERR << 24) | ((U32)(&CLKSRCSEL) & 0x00ffffff);
//        }
//        if(PLLSTAT != (12 | (1 << 16) | (1 << 24)  | (1 << 25) | (1 << 26)))    //refer to "PLLCFG_Val" at LPC2300.s
//        {
//            err |= (REGISTER_ERR << 24) | ((U32)(&PLLSTAT) & 0x00ffffff);
//        }
//        if(CCLKCFG != 0x00000003)       //refer to "CCLKCFG_Val" at LPC2300.s
//        {
//            err |= (REGISTER_ERR << 24) | ((U32)(&CCLKCFG) & 0x00ffffff);
//        }
//        if(SCS != 0x00000061)           //refer to "SCS_Val" at LPC2300.s
//        {
//            err |= (REGISTER_ERR << 24) | ((U32)(&SCS) & 0x00ffffff);
//        }
//        if(PCLKSEL0 != 0x00000144)      //refer to "PCLKSEL0_Val" at LPC2300.s
//        {
//            err |= (REGISTER_ERR << 24) | ((U32)(&PCLKSEL0) & 0x00ffffff);
//        }
//        if(PCLKSEL1 != 0x10050005)      //refer to "PCLKSEL1_Val" at LPC2300.s
//        {
//            err |= (REGISTER_ERR << 24) | ((U32)(&PCLKSEL1) & 0x00ffffff);
//        }
//        if(MAMCR != 0x00000002)         //refer to "MAMCR_Val" at LPC2300.s
//        {
//            err |= (REGISTER_ERR << 24) | ((U32)(&PCLKSEL1) & 0x00ffffff);
//        }
//        if(MAMTIM != 0x00000002)        //refer to "MAMTIM_Val" at LPC2300.s
//        {
//            err |= (REGISTER_ERR << 24) | ((U32)(&PCLKSEL1) & 0x00ffffff);
//        }
//        if(PCONP != 0x4308001A)         //refer to "PCONP_Val" at LPC2300.s
//        {
//            err |= (REGISTER_ERR << 24) | ((U32)(&PCONP) & 0x00ffffff);
//        }
//
//        if(*(Mem_64_Byte + 2) != MEM_64_SIZE)       //检查内存块size参数
//        {
//            err |= (MEM_SIZE_ERR << 24) | MEM_64_TYPE;
//        }
//        if(*(Mem_64_Byte + 1) != (U32)((U8*)Mem_64_Byte + sizeof(Mem_64_Byte)))         //检查内存块end指针
//        {
//            err |= (MEM_END_ERR << 24) | MEM_64_TYPE;
//        }
//        if(*Mem_64_Byte == NULL)        //检查内存块free指针
//        {
//            if((++mem_64_cnt) > 60)     //连续60次检测到内存块耗尽
//            {
//                err |= (MEM_FREE_ERR << 24) | MEM_64_TYPE;
//            }
//        }
//        else
//        {
//            mem_64_cnt = 0;
//        }
//
//        if(*(Mem_256_Byte + 2) != MEM_256_SIZE)     //检查内存块size参数
//        {
//            err |= (MEM_SIZE_ERR << 24) | MEM_256_TYPE;
//        }
//        if(*(Mem_256_Byte + 1) != (U32)((U8*)Mem_256_Byte + sizeof(Mem_256_Byte)))      //检查内存块end指针
//        {
//            err |= (MEM_END_ERR << 24) | MEM_256_TYPE;
//        }
//        if(*Mem_256_Byte == NULL)       //检查内存块free指针
//        {
//            if((++mem_256_cnt) > 60)    //连续60次检测到内存块耗尽
//            {
//                err |= (MEM_FREE_ERR << 24) | MEM_256_TYPE;
//            }
//        }
//        else
//        {
//            mem_256_cnt = 0;
//        }
//
//        if(*(Mem_1536_Byte + 2) != MEM_1536_SIZE)   //检查内存块size参数
//        {
//            err |= (MEM_SIZE_ERR << 24) | MEM_1536_TYPE;
//        }
//        if(*(Mem_1536_Byte + 1) != (U32)((U8*)Mem_1536_Byte + sizeof(Mem_1536_Byte)))   //检查内存块end指针
//        {
//            err |= (MEM_END_ERR << 24) | MEM_1536_TYPE;
//        }
//        if(*Mem_1536_Byte == NULL)      //检查内存块free指针
//        {
//            if((++mem_1536_cnt) > 60)   //连续60次检测到内存块耗尽
//            {
//                err |= (MEM_FREE_ERR << 24) | MEM_1536_TYPE;
//            }
//        }
//        else
//        {
//            mem_1536_cnt = 0;
//        }


        if((cnt % 20) == 0)             //每10秒进入检查一次
        {

//debug task test
//            if((Task_Schedule_Flag & (1 << TCP_POLL_TASK_PRIO)) == 0)
//            {
//                err |= (TASK_RUN_ERR << 24) | TCP_POLL_TASK_PRIO;
//            }
//            else
//            {
//                Task_Schedule_Flag &= ~(1 << TCP_POLL_TASK_PRIO);
//            }

            if((Task_Schedule_Flag & (1 << MANAGE_TASK_PRIO)) == 0)
            {
                gtask_error |= (TASK_RUN_ERR << 24) | MANAGE_TASK_PRIO;
            }
            else
            {
                Task_Schedule_Flag &= ~(1 << MANAGE_TASK_PRIO);
            }
            if((Task_Schedule_Flag & (1 << LCD_DISPLAY_PRIO)) == 0)
            {
                gtask_error |= (TASK_RUN_ERR << 24) | LCD_DISPLAY_PRIO;
            }
            else
            {
                Task_Schedule_Flag &= ~(1 << LCD_DISPLAY_PRIO);
            }
            if((Task_Schedule_Flag & (1 << TIMING_TASK_PRIO)) == 0)
            {
                gtask_error |= (TASK_RUN_ERR << 24) | TIMING_TASK_PRIO;
            }
            else
            {
                Task_Schedule_Flag &= ~(1 << TIMING_TASK_PRIO);
            }
            if((Task_Schedule_Flag & (1 << UART_TASK_PRIO)) == 0)
            {
                gtask_error |= (TASK_RUN_ERR << 24) | UART_TASK_PRIO;
            }
            else
            {
                Task_Schedule_Flag &= ~(1 << UART_TASK_PRIO);
            }

            if((Task_Schedule_Flag & (1 << TRANSMIT_TASK_PRIO)) == 0)
            {
                gtask_error |= (TASK_RUN_ERR << 24) | TRANSMIT_TASK_PRIO;
            }
            else
            {
                Task_Schedule_Flag &= ~(1 << TRANSMIT_TASK_PRIO);
            }
            if((Task_Schedule_Flag & (1 << GPRS_TASK_PRIO)) == 0)
            {
//azh debug
//                gtask_error |= (TASK_RUN_ERR << 24) | GPRS_TASK_PRIO;
            }
            else
            {
                Task_Schedule_Flag &= ~(1 << GPRS_TASK_PRIO);
            }
        }

        if((cnt % 240) == 0)            //每2分钟进入检查一次
        {
            if((Task_Schedule_Flag & (1 << PROTOCOL_TASK_PRIO)) == 0)
            {
                gtask_error |= (TASK_RUN_ERR << 24) | PROTOCOL_TASK_PRIO;
            }
            else
            {
                Task_Schedule_Flag &= ~(1 << PROTOCOL_TASK_PRIO);
            }

        }

        if(gtask_error != 0)
        {
            disable_irq();              // 关闭所有中断
            while(1);                   //等待看门狗复位
        }
		CLRWDT;                         //清看门狗 500ms清一次

        cnt++;
    }
}
*/

__task void system_manage_task (void)
{
//    U8  mem_64_cnt, mem_256_cnt, mem_1536_cnt;
//    static U16 cnt;
//    U32 err;
	U16 event_flag=0;

    gtask_error = 0;
    //for(gtask_cnt = 0; gtask_cnt < 1800; gtask_cnt++)                        //3分钟后开始检查
    for(gtask_cnt = 0; gtask_cnt < 400; gtask_cnt++)                        //40s
    {
        sign_task_schedule(MANAGE_TASK_PRIO);               //响应系统复位操作
        os_dly_wait(OS_DLY_100MS);
        CLRWDT;
    }

    gtask_cnt = 0;
//    mem_64_cnt = 0;
//    mem_256_cnt = 0;
//    mem_1536_cnt = 0;

    for(;;)
    {
//        os_evt_wait_or(POWER_DOWN_FLAG, OS_DLY_1S);         //等待超时处理任务状态检查或掉电事件唤醒
        sign_task_schedule(MANAGE_TASK_PRIO);
		if(os_evt_wait_or(POWER_DOWN_FLAG|EXTCMD_RST_FLAG, OS_DLY_500MS) == OS_R_EVT )         //等待超时处理任务状态检查或掉电事件唤醒
        {
            event_flag = os_evt_get();
            if(event_flag & POWER_DOWN_FLAG)          //响应掉电事件
            {
                continue;
            }        
            if(event_flag & EXTCMD_RST_FLAG)          //响应掉电事件
            {
    			disable_irq();              // 关闭所有中断
                while(1);                   //等待看门狗复位
            }
        }
        
        gtask_error = 0;

//        if(CLKSRCSEL != 0x00000001)     //refer to "CLKSRCSEL_Val" at LPC2300.s
//        {
//            err |= (REGISTER_ERR << 24) | ((U32)(&CLKSRCSEL) & 0x00ffffff);
//        }
//        if(PLLSTAT != (12 | (1 << 16) | (1 << 24)  | (1 << 25) | (1 << 26)))    //refer to "PLLCFG_Val" at LPC2300.s
//        {
//            err |= (REGISTER_ERR << 24) | ((U32)(&PLLSTAT) & 0x00ffffff);
//        }
//        if(CCLKCFG != 0x00000003)       //refer to "CCLKCFG_Val" at LPC2300.s
//        {
//            err |= (REGISTER_ERR << 24) | ((U32)(&CCLKCFG) & 0x00ffffff);
//        }
//        if(SCS != 0x00000061)           //refer to "SCS_Val" at LPC2300.s
//        {
//            err |= (REGISTER_ERR << 24) | ((U32)(&SCS) & 0x00ffffff);
//        }
//        if(PCLKSEL0 != 0x00000144)      //refer to "PCLKSEL0_Val" at LPC2300.s
//        {
//            err |= (REGISTER_ERR << 24) | ((U32)(&PCLKSEL0) & 0x00ffffff);
//        }
//        if(PCLKSEL1 != 0x10050005)      //refer to "PCLKSEL1_Val" at LPC2300.s
//        {
//            err |= (REGISTER_ERR << 24) | ((U32)(&PCLKSEL1) & 0x00ffffff);
//        }
//        if(MAMCR != 0x00000002)         //refer to "MAMCR_Val" at LPC2300.s
//        {
//            err |= (REGISTER_ERR << 24) | ((U32)(&PCLKSEL1) & 0x00ffffff);
//        }
//        if(MAMTIM != 0x00000002)        //refer to "MAMTIM_Val" at LPC2300.s
//        {
//            err |= (REGISTER_ERR << 24) | ((U32)(&PCLKSEL1) & 0x00ffffff);
//        }
//        if(PCONP != 0x4308001A)         //refer to "PCONP_Val" at LPC2300.s
//        {
//            err |= (REGISTER_ERR << 24) | ((U32)(&PCONP) & 0x00ffffff);
//        }
//
//        if(*(Mem_64_Byte + 2) != MEM_64_SIZE)       //检查内存块size参数
//        {
//            err |= (MEM_SIZE_ERR << 24) | MEM_64_TYPE;
//        }
//        if(*(Mem_64_Byte + 1) != (U32)((U8*)Mem_64_Byte + sizeof(Mem_64_Byte)))         //检查内存块end指针
//        {
//            err |= (MEM_END_ERR << 24) | MEM_64_TYPE;
//        }
//        if(*Mem_64_Byte == NULL)        //检查内存块free指针
//        {
//            if((++mem_64_cnt) > 60)     //连续60次检测到内存块耗尽
//            {
//                err |= (MEM_FREE_ERR << 24) | MEM_64_TYPE;
//            }
//        }
//        else
//        {
//            mem_64_cnt = 0;
//        }
//
//        if(*(Mem_256_Byte + 2) != MEM_256_SIZE)     //检查内存块size参数
//        {
//            err |= (MEM_SIZE_ERR << 24) | MEM_256_TYPE;
//        }
//        if(*(Mem_256_Byte + 1) != (U32)((U8*)Mem_256_Byte + sizeof(Mem_256_Byte)))      //检查内存块end指针
//        {
//            err |= (MEM_END_ERR << 24) | MEM_256_TYPE;
//        }
//        if(*Mem_256_Byte == NULL)       //检查内存块free指针
//        {
//            if((++mem_256_cnt) > 60)    //连续60次检测到内存块耗尽
//            {
//                err |= (MEM_FREE_ERR << 24) | MEM_256_TYPE;
//            }
//        }
//        else
//        {
//            mem_256_cnt = 0;
//        }
//
//        if(*(Mem_1536_Byte + 2) != MEM_1536_SIZE)   //检查内存块size参数
//        {
//            err |= (MEM_SIZE_ERR << 24) | MEM_1536_TYPE;
//        }
//        if(*(Mem_1536_Byte + 1) != (U32)((U8*)Mem_1536_Byte + sizeof(Mem_1536_Byte)))   //检查内存块end指针
//        {
//            err |= (MEM_END_ERR << 24) | MEM_1536_TYPE;
//        }
//        if(*Mem_1536_Byte == NULL)      //检查内存块free指针
//        {
//            if((++mem_1536_cnt) > 60)   //连续60次检测到内存块耗尽
//            {
//                err |= (MEM_FREE_ERR << 24) | MEM_1536_TYPE;
//            }
//        }
//        else
//        {
//            mem_1536_cnt = 0;
//        }


        if((gtask_cnt % 32) == 0)             //每10秒进入检查一次
        {

//debug task test
//            if((Task_Schedule_Flag & (1 << TCP_POLL_TASK_PRIO)) == 0)
//            {
//                err |= (TASK_RUN_ERR << 24) | TCP_POLL_TASK_PRIO;
//            }
//            else
//            {
//                Task_Schedule_Flag &= ~(1 << TCP_POLL_TASK_PRIO);
//            }

            if((Task_Schedule_Flag & (1 << MANAGE_TASK_PRIO)) == 0)
            {
                gtask_error |= (TASK_RUN_ERR << 24) | MANAGE_TASK_PRIO;
            }
            else
            {
                Task_Schedule_Flag &= ~(1 << MANAGE_TASK_PRIO);
            }
#ifdef LCD_VALID
            if((Task_Schedule_Flag & (1 << LCD_DISPLAY_PRIO)) == 0)
            {
                gtask_error |= (TASK_RUN_ERR << 24) | LCD_DISPLAY_PRIO;
            }
            else
            {
                Task_Schedule_Flag &= ~(1 << LCD_DISPLAY_PRIO);
            }
#endif            
            if((Task_Schedule_Flag & (1 << TIMING_TASK_PRIO)) == 0)
            {
                gtask_error |= (TASK_RUN_ERR << 24) | TIMING_TASK_PRIO;
            }
            else
            {
                Task_Schedule_Flag &= ~(1 << TIMING_TASK_PRIO);
            }
            if((Task_Schedule_Flag & (1 << UART_TASK_PRIO)) == 0)
            {
                gtask_error |= (TASK_RUN_ERR << 24) | UART_TASK_PRIO;
            }
            else
            {
                Task_Schedule_Flag &= ~(1 << UART_TASK_PRIO);
            }

//             if((Task_Schedule_Flag & (1 << TRANSMIT_TASK_PRIO)) == 0)
//             {
//                 gtask_error |= (TASK_RUN_ERR << 24) | TRANSMIT_TASK_PRIO;
//             }
//             else
//             {
//                 Task_Schedule_Flag &= ~(1 << TRANSMIT_TASK_PRIO);
//             }
            if((Task_Schedule_Flag & (1 << GPRS_TASK_PRIO)) == 0)
            {
//azh debug
//                gtask_error |= (TASK_RUN_ERR << 24) | GPRS_TASK_PRIO;
            }
            else
            {
                Task_Schedule_Flag &= ~(1 << GPRS_TASK_PRIO);
            }
        }

        if((gtask_cnt % 256) == 0)            //每2分钟进入检查一次
        {
            if((Task_Schedule_Flag & (1 << PROTOCOL_TASK_PRIO)) == 0)
            {
                gtask_error |= (TASK_RUN_ERR << 24) | PROTOCOL_TASK_PRIO;
            }
            else
            {
                Task_Schedule_Flag &= ~(1 << PROTOCOL_TASK_PRIO);
            }

        }

        if((gtask_error != 0) || (gusMemErrId != 0))//azh 
        {
            disable_irq();              // 关闭所有中断
            while(1);                   //等待看门狗复位
        }
		CLRWDT;                         //清看门狗 500ms清一次

        gtask_cnt++;
    }
}
/******************************************************************************
                            End Of File
 *****************************************************************************/

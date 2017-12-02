/******************************************************************************
 Copyright (C) 2010	 R&D Institute of HuaCai Co.,Ltd.
 File Name		: task manage function
 Description	: task_manage.c
-------------------------------------------------------------------------------
 Modification History:
 <No.>	<version>		<time>		<author>		<contents>
   2��
   1��	  1.00		  2011-08-17	azh			 the original version
******************************************************************************/
#include	"..\inc\global_config.h"

U32 gtask_error;
U16 gtask_cnt;
//------------------------------ ˽�б������� ---------------------------------
//------------------------------ ˽�к������� -------------------------------
/******************************************************************************
Function name: void sign_task_schedule (U8 prio)
Author       : snap.gao
Description  : ������ÿ��������ã�����������б�־
               �����λ������¼�����ʱ����������ɾ����������
               һЩ�����������Ҫ�����ֳ����ͷ�������Դ����ɾ������
               ��������ȼ����������������ɾ����ϵͳ��λ
               �������ֳ���ʱ�ܳ�ʱ���������λ���󳬹�ָ��ʱ���ǿ��������λ
               �������ֳ���ʱ�ܳ�ʱ�����紦��һֱ�ȴ�ֱ��û��
Input        : prio is priority of the task
Return       : None
******************************************************************************/
void sign_task_schedule (U8 prio)
{

//    U32 i;

    prio &= 0x1f;
    Task_Schedule_Flag |= (1 << prio);
/*
    if((Require_Software_Reset != 0) || (Power_Down_Flag == POWER_DOWN_FLAG))       // �������λ������¼�
    {
        if(prio == TIMING_TASK_PRIO)                        // �������е���������
        {
            if((Require_Software_Reset == (SOFTWARE_RESET_DELAY / 2)) || (Power_Down_Flag == POWER_DOWN_FLAG))      // �������λʱ���һ�������¼�
            {
                for(i = 0; i < MAX_SOCKET_NUM; i++)
                {
                    if(Net_Socket[i].Type == TCP_SOCKET)                    // ���socket�����ͣ�TCP������Ҫ����
                    {
                        Net_Status_Mechanism[i].Close_Flag = 1;             // ������������
                    }
                }
                os_evt_set(POWER_DOWN_FLAG, System_Manage_Task_ID);         // ֪ͨ����������λ
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
            if(Require_Software_Reset != 0)                 // �������λ
            {
                if(++Require_Software_Reset > SOFTWARE_RESET_DELAY)         // ���Ӳ��ȴ������λ������ʱ
                {
                    run_reset_head();                                       // ����ʼ������λ, ʵ���л�
                }
            }
            tsk_lock();                                     // ������������ɾ��, �ǾͿ��Ը�λ��
            for(i = 0; i < (Timing_Task_ID - 1); i++)
            {
                if(os_active_TCB[i] != NULL)                // �����и�����û�б�ɾ��
                {
                    break;
                }
            }
            tsk_unlock();
            if(i == (Timing_Task_ID - 1))                   // �ж��Ƿ����������Ѿ�ɾ��
            {
                if(Require_Software_Reset != 0)             // �������λ
                {
                    run_reset_head();                       // ����ʼ������λ, ʵ���л�
                }
                else                                        // �ǵ����¼�
                {
                    VICIntEnClr = 0xffffffff;               // ���ж�
                    while(1);                               // �ȴ����Ź���λ
                }
            }
        }
    }
    else
    {
        Task_Schedule_Record |= Task_Schedule_Flag;
        if((Task_Schedule_Record>>1) != ((1<<TICK_TIMER_TASK_PRIO) - 1))        //�ж��Ƿ�������������һ��
        {
            return;
        }
        if((Global_Status_Word & HOST_CONNECT_FLAG) == 0)   //�ж���̫���Ƿ����ӵ�����
        {
            return;
        }
        insure_run_flag_valid();
        return;
    }

    if((Require_Software_Reset >= (SOFTWARE_RESET_DELAY / 2)) || (Power_Down_Flag == POWER_DOWN_FLAG))      // ���������λʱ���һ�������¼�
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
            os_tsk_delete_self();                           // ����ռ����Դ�˿���ɾ��
            break;
        case TICK_TIMER_TASK_PRIO:                          // ��ɾ��������TCP poll����ɾ��
        case TIMING_TASK_PRIO:                              // ��ɾ���������ֵ����
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
            FIO4CLR |= P4_RST_PHY;                          // ��λPHY
            os_tsk_delete(Tick_Timer_Task_ID);
            os_tsk_delete_self();                           // ����ռ����Դ�˿���ɾ��
        default:
            os_tsk_delete_self();                           // ��ռ����Դ������ֱ��ɾ��
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
    for(cnt = 0; cnt < 1800; cnt++)                        //3���Ӻ�ʼ���
    {
        sign_task_schedule(MANAGE_TASK_PRIO);               //��Ӧϵͳ��λ����
        os_dly_wait(OS_DLY_100MS);
        CLRWDT;
    }

    cnt = 0;
//    mem_64_cnt = 0;
//    mem_256_cnt = 0;
//    mem_1536_cnt = 0;

    for(;;)
    {
//        os_evt_wait_or(POWER_DOWN_FLAG, OS_DLY_1S);         //�ȴ���ʱ��������״̬��������¼�����
		os_evt_wait_or(POWER_DOWN_FLAG, OS_DLY_500MS);         //�ȴ���ʱ��������״̬��������¼�����
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
//        if(*(Mem_64_Byte + 2) != MEM_64_SIZE)       //����ڴ��size����
//        {
//            err |= (MEM_SIZE_ERR << 24) | MEM_64_TYPE;
//        }
//        if(*(Mem_64_Byte + 1) != (U32)((U8*)Mem_64_Byte + sizeof(Mem_64_Byte)))         //����ڴ��endָ��
//        {
//            err |= (MEM_END_ERR << 24) | MEM_64_TYPE;
//        }
//        if(*Mem_64_Byte == NULL)        //����ڴ��freeָ��
//        {
//            if((++mem_64_cnt) > 60)     //����60�μ�⵽�ڴ��ľ�
//            {
//                err |= (MEM_FREE_ERR << 24) | MEM_64_TYPE;
//            }
//        }
//        else
//        {
//            mem_64_cnt = 0;
//        }
//
//        if(*(Mem_256_Byte + 2) != MEM_256_SIZE)     //����ڴ��size����
//        {
//            err |= (MEM_SIZE_ERR << 24) | MEM_256_TYPE;
//        }
//        if(*(Mem_256_Byte + 1) != (U32)((U8*)Mem_256_Byte + sizeof(Mem_256_Byte)))      //����ڴ��endָ��
//        {
//            err |= (MEM_END_ERR << 24) | MEM_256_TYPE;
//        }
//        if(*Mem_256_Byte == NULL)       //����ڴ��freeָ��
//        {
//            if((++mem_256_cnt) > 60)    //����60�μ�⵽�ڴ��ľ�
//            {
//                err |= (MEM_FREE_ERR << 24) | MEM_256_TYPE;
//            }
//        }
//        else
//        {
//            mem_256_cnt = 0;
//        }
//
//        if(*(Mem_1536_Byte + 2) != MEM_1536_SIZE)   //����ڴ��size����
//        {
//            err |= (MEM_SIZE_ERR << 24) | MEM_1536_TYPE;
//        }
//        if(*(Mem_1536_Byte + 1) != (U32)((U8*)Mem_1536_Byte + sizeof(Mem_1536_Byte)))   //����ڴ��endָ��
//        {
//            err |= (MEM_END_ERR << 24) | MEM_1536_TYPE;
//        }
//        if(*Mem_1536_Byte == NULL)      //����ڴ��freeָ��
//        {
//            if((++mem_1536_cnt) > 60)   //����60�μ�⵽�ڴ��ľ�
//            {
//                err |= (MEM_FREE_ERR << 24) | MEM_1536_TYPE;
//            }
//        }
//        else
//        {
//            mem_1536_cnt = 0;
//        }


        if((cnt % 20) == 0)             //ÿ10�������һ��
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

        if((cnt % 240) == 0)            //ÿ2���ӽ�����һ��
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
            disable_irq();              // �ر������ж�
            while(1);                   //�ȴ����Ź���λ
        }
		CLRWDT;                         //�忴�Ź� 500ms��һ��

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
    //for(gtask_cnt = 0; gtask_cnt < 1800; gtask_cnt++)                        //3���Ӻ�ʼ���
    for(gtask_cnt = 0; gtask_cnt < 400; gtask_cnt++)                        //40s
    {
        sign_task_schedule(MANAGE_TASK_PRIO);               //��Ӧϵͳ��λ����
        os_dly_wait(OS_DLY_100MS);
        CLRWDT;
    }

    gtask_cnt = 0;
//    mem_64_cnt = 0;
//    mem_256_cnt = 0;
//    mem_1536_cnt = 0;

    for(;;)
    {
//        os_evt_wait_or(POWER_DOWN_FLAG, OS_DLY_1S);         //�ȴ���ʱ��������״̬��������¼�����
        sign_task_schedule(MANAGE_TASK_PRIO);
		if(os_evt_wait_or(POWER_DOWN_FLAG|EXTCMD_RST_FLAG, OS_DLY_500MS) == OS_R_EVT )         //�ȴ���ʱ��������״̬��������¼�����
        {
            event_flag = os_evt_get();
            if(event_flag & POWER_DOWN_FLAG)          //��Ӧ�����¼�
            {
                continue;
            }        
            if(event_flag & EXTCMD_RST_FLAG)          //��Ӧ�����¼�
            {
    			disable_irq();              // �ر������ж�
                while(1);                   //�ȴ����Ź���λ
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
//        if(*(Mem_64_Byte + 2) != MEM_64_SIZE)       //����ڴ��size����
//        {
//            err |= (MEM_SIZE_ERR << 24) | MEM_64_TYPE;
//        }
//        if(*(Mem_64_Byte + 1) != (U32)((U8*)Mem_64_Byte + sizeof(Mem_64_Byte)))         //����ڴ��endָ��
//        {
//            err |= (MEM_END_ERR << 24) | MEM_64_TYPE;
//        }
//        if(*Mem_64_Byte == NULL)        //����ڴ��freeָ��
//        {
//            if((++mem_64_cnt) > 60)     //����60�μ�⵽�ڴ��ľ�
//            {
//                err |= (MEM_FREE_ERR << 24) | MEM_64_TYPE;
//            }
//        }
//        else
//        {
//            mem_64_cnt = 0;
//        }
//
//        if(*(Mem_256_Byte + 2) != MEM_256_SIZE)     //����ڴ��size����
//        {
//            err |= (MEM_SIZE_ERR << 24) | MEM_256_TYPE;
//        }
//        if(*(Mem_256_Byte + 1) != (U32)((U8*)Mem_256_Byte + sizeof(Mem_256_Byte)))      //����ڴ��endָ��
//        {
//            err |= (MEM_END_ERR << 24) | MEM_256_TYPE;
//        }
//        if(*Mem_256_Byte == NULL)       //����ڴ��freeָ��
//        {
//            if((++mem_256_cnt) > 60)    //����60�μ�⵽�ڴ��ľ�
//            {
//                err |= (MEM_FREE_ERR << 24) | MEM_256_TYPE;
//            }
//        }
//        else
//        {
//            mem_256_cnt = 0;
//        }
//
//        if(*(Mem_1536_Byte + 2) != MEM_1536_SIZE)   //����ڴ��size����
//        {
//            err |= (MEM_SIZE_ERR << 24) | MEM_1536_TYPE;
//        }
//        if(*(Mem_1536_Byte + 1) != (U32)((U8*)Mem_1536_Byte + sizeof(Mem_1536_Byte)))   //����ڴ��endָ��
//        {
//            err |= (MEM_END_ERR << 24) | MEM_1536_TYPE;
//        }
//        if(*Mem_1536_Byte == NULL)      //����ڴ��freeָ��
//        {
//            if((++mem_1536_cnt) > 60)   //����60�μ�⵽�ڴ��ľ�
//            {
//                err |= (MEM_FREE_ERR << 24) | MEM_1536_TYPE;
//            }
//        }
//        else
//        {
//            mem_1536_cnt = 0;
//        }


        if((gtask_cnt % 32) == 0)             //ÿ10�������һ��
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

        if((gtask_cnt % 256) == 0)            //ÿ2���ӽ�����һ��
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
            disable_irq();              // �ر������ж�
            while(1);                   //�ȴ����Ź���λ
        }
		CLRWDT;                         //�忴�Ź� 500ms��һ��

        gtask_cnt++;
    }
}
/******************************************************************************
                            End Of File
 *****************************************************************************/

/******************************************************************************
 Copyright (C) 2009  R&D Institute of Reallin Co., Ltd.
 Module         : uart mamage function
 File Name      : uart.c
 Description    :
 Author         :
 Build Date     :
 others         :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、     1.00       2009-06-30     snap.gao       create
******************************************************************************/
/*---------------------------------------------------------------------------*/
#include    "..\inc\global_config.h"


//static void __irq UART0_IRQ_handler (void);
//static void __irq UART1_IRQ_handler (void);
//static void __irq UART2_IRQ_handler (void);
//static void __irq UART3_IRQ_handler (void);
//azh 110516
static void UART0_init(U8 mode, U32 baud);
static void UART1_init(U8 mode, U32 baud);
static void UART2_init(U8 mode, U32 baud);
//static void   UART3_init(U8 mode, U32 baud);

static S8   store_comm_data (U8 com, U32 data);
static void record_status_mechanism (U8 com, U8 mechanism);
static void mode_config (U8 com, U8 mode);
static void baud_rate_config (U8 com, U32 baud);
static void UART_send_push (U8 com);

static void set_RS485_rec (U8 com);
static void set_RS485_send (U8 com);

#ifdef ZIGBEE_TEST
static U8 query_zigbee_send_event(void);
//static U8 check_mac_addr(U8 *buf);
//static U8 check_dest_addr(U8 *buf1, U8 *buf2);
static U32 check_local_addr(U8 *buf);
static U32 check_broadcast_addr(U8 *buf);
static U32 comp_zigbee_addr(U8 *buf1, U8 *buf2);
static U8 alloc_zigbee_rec_struct(void);
static U8 alloc_zigbee_send_mem(void);
static S8 zigbee_local_frame_analyse(U8 *buf);
static S8 zigbee_package_assemble(U8 *buf, U8 **ret_ptr);
static U16 get_fifo_vaild_data_len(U16 start_ptr, U16 end_ptr);
static U16 increase_end_ptr(U16 end_ptr, U16 num);
static U16 increase_start_ptr(U16 start_ptr, U16 end_ptr);
static S32 deal_with_zigbee_send(U8 *mem_ptr);
static S32 release_zigbee_send_mem(U8 frame_serial);
static S32 resend_zigbee_package(U8 frame_serial);
static void deal_with_zigbee_rec(U8 **ret_ptr);
static U32 check_duplicate_frame(U8 *addr, U8 serial);
static U8 record_frame_serial(U8 *addr, U8 serial);

static S8 format_zigbee_rec_buf(U8 com, U8 *info);
static S8 store_zigbee_comm_data(U8 com, U32 data);
static void init_zigbee_mem(void);
static void check_zigbee_rec_mem(void);
static void check_zigbee_send_mem(void);
static void copy_zigbee_rec_fifo_data(U8 *buf, U16 offset, U8 len);
#endif// ZIGBEE_TEST
/******************************************************************************
 Function name:  __task void UART_task (void)
 Author       :
 Description  :
 Input        :  None
 Return       :  None
 *****************************************************************************/
__task void UART_task (void)
{
    OS_RESULT uart_status;
    U32       uart_data;
    U8        *mem_ptr;
    U8        com;
    U8        mem_type;
    U8        flag;
#ifdef ZIGBEE_TEST
    U8        index;
#endif//ZIGBEE_TEST

    OS_Time = 0;

    status_mechanism_init(COM0);
    status_mechanism_init(COM1);
    status_mechanism_init(COM2);
    status_mechanism_init(COM3);

#ifdef ZIGBEE_TEST //CODE_VER_TYPE==H_METER_PRJ		//本高压表
    WLS_RST_HIGH(); //只有所有初始化结束才拉高，因为一上电模块就会有数据发出来的
#endif

    for(;;)
    {
        check_power_down();         //检查掉电，必须放在sign_task_schedule()和邮箱等待之前
//azh 110519 暂时注释掉
        sign_task_schedule(UART_TASK_PRIO);

#ifdef ZIGBEE_TEST
        check_zigbee_rec_mem();                             //检查ZigBee接收内存块是否超时
        check_zigbee_send_mem();                            //检查ZigBee发送内存块是否超时
#endif// ZIGBEE_TEST

        uart_status = os_mbx_wait(Mbx_UART_Data, (void**)&uart_data, 1);    //查询接收队列是否有数据
        if(uart_status == OS_R_TMO)//azh
        {
            tsk_lock();
            OS_Time++;
            tsk_unlock();
        }
        while(uart_status != OS_R_TMO)
        {
            if(uart_data == 0)
            {
                break;
            }
            com = (uart_data >> 24) & 0x0f;
            if(com > MAX_PORT_NUM)
            {
                break;
            }
            Meter_LED_Cnt[com] = LED_FLASH_TIMES;                    //指示接收
            Status_Mechanism[com].Tmp_Tick_Count = OS_Time;     //缓冲一个当前系统Tick
            Status_Mechanism[com].Rec_Tick_Count = Status_Mechanism[com].Tmp_Tick_Count;
            switch(Status_Mechanism[com].Rec_State)             //接收状态机处理
            {
            case WAIT_REC:          //空闲时收到数据
#ifdef ZIGBEE_TEST
                if(com == COM1)
                {
                    record_status_mechanism(com, DATA_REC << 4);
                    Status_Mechanism[com].Rec_State = DATA_REC;
                    Status_Mechanism[com].Last_Err_State = NO_MATCH_STATUS;
                }
                else
                {
#endif// ZIGBEE_TEST
                    mem_ptr = alloc_comm_mem(Status_Mechanism[com].Rec_Mem_Type, &mem_type, GET_MEM_ERR4);
                    if(NULL == mem_ptr)
                    {
                        os_mbx_send(Mbx_UART_Data, (void*)uart_data, 0);        //归还数据下一个Tick再试，因为中断在接收，所以可能引起顺序错
                        Status_Mechanism[com].Last_Err_State = MEM_EMPTY;       //内存块申请不到
//                      memcpy(gusCurMemErrFunName, "UART_tas", 8);
                        setmemgetnullid(GET_MEM_ERR4, gusCurMemErrFunName);
                        goto receive_end;
                    }
                    else
                    {
                        Status_Mechanism[com].Rx_Buf = mem_ptr;
                        init_frame_info(Status_Mechanism[com].Rx_Buf);          //初始化帧头信息
                        set_frame_len(Status_Mechanism[com].Rx_Buf, 0);         //长度清零
                        set_port_num(Status_Mechanism[com].Rx_Buf, com);        //标识端口号
                        if(__FAIL == store_comm_data(com, uart_data))           //保存串口数据
                        {
                            os_mbx_send(Mbx_UART_Data, (void*)uart_data, 0);    //归还数据下一个Tick再试，因为中断在接收，所以可能引起顺序错
                            Status_Mechanism[com].Last_Err_State = MEM_ERROR;   //内存块地址非法
                            goto receive_end;
                        }
                        Status_Mechanism[com].Rec_Tick_Count = Status_Mechanism[com].Tmp_Tick_Count;
                        record_status_mechanism(com, DATA_REC<<4);
                        Status_Mechanism[com].Rec_State = DATA_REC;             //推动状态机，开始接收外部数据
                    }
#ifdef ZIGBEE_TEST
                }
#endif// ZIGBEE_TEST
                break;
            case DATA_REC:          //接收数据
#ifdef ZIGBEE_TEST
                if(com == 1)//zigbee
                {
                    store_zigbee_comm_data(com, uart_data);
                }
                else
                {
#endif//ZIGBEE_TEST
                    Status_Mechanism[com].Rec_Tick_Count = Status_Mechanism[com].Tmp_Tick_Count;
                    if(NULL != Status_Mechanism[com].Rx_Buf)
                    {
                        if(__FAIL == store_comm_data(com, uart_data))           //保存串口数据
                        {
                            os_mbx_send(Mbx_UART_Data, (void*)uart_data, 0);    //归还数据下一个Tick再试，因为中断在接收，所以可能引起顺序错
                            Status_Mechanism[com].Last_Err_State = MEM_ERROR;   //内存块地址非法
                            goto receive_end;
                        }
                    }
                    else
                    {
                        Status_Mechanism[com].Last_Err_State = MEM_ERROR;       //内存块地址非法
                    }
#ifdef ZIGBEE_TEST
                }
#endif//ZIGBEE_TEST
                break;
            default:                //没有对应的状态机接受数据，是一些暂态的状态机造成的
                os_mbx_send(Mbx_UART_Data, (void*)uart_data, 0);            //归还数据下一个Tick再试，因为中断在接收，所以可能引起顺序错
                Status_Mechanism[com].Last_Err_State = NO_MATCH_STATUS;
                goto receive_end;
            }
            uart_status = os_mbx_wait(Mbx_UART_Data, (void**)&uart_data, 0);
        }
receive_end:
        for(com = 0; com < MAX_PORT_NUM; com++)
        {
            Status_Mechanism[com].Tmp_Tick_Count = OS_Time;         //缓冲一个当前系统Tick
            switch(Status_Mechanism[com].Rec_State)                 //处理接收状态机
            {
            case DATA_REC:                  //接收数据字节超时处理
#ifdef ZIGBEE_TEST
                if(com == COM1)//zigbee
                {
                    //deal_with_zigbee_rec();//使用固定的环状内存块接收数据
                    mem_ptr = NULL;
                    deal_with_zigbee_rec(&mem_ptr);//使用固定的环状内存块接收数据
                    if(mem_ptr != NULL)
                    {
                        //zigbee_led_time[0] = ZIGBEE_LED_FLASH_DELAY;//5*100ms
                        local_led_time[0] = LOCAL_LED_FLASH_DELAY;//5*100ms //azh 171011
                        Status_Mechanism[com].Rx_Buf = mem_ptr;

                        record_status_mechanism(com, BYTE_REC_TMO);                         //字节接收超时，记录过程状态机
                        if(__TRUE == check_mbox(Status_Mechanism[com].Mbx_Answer_Buf))      //判断是否是在接收应答
                        {
                            record_status_mechanism(com, CONFIG_DEFAULT_STATUS);            //记录状态机
                            baud_rate_config(com, Status_Mechanism[com].Default_Baud * 300);
                            Status_Mechanism[com].Rec_Baud = DEFAULT_BAUD_RATE;
                            Status_Mechanism[com].Send_Baud = DEFAULT_BAUD_RATE;
                            mode_config(com, Status_Mechanism[com].Default_Para);
                            Status_Mechanism[com].Rec_Para = DEFAULT_COM_PARA;
                            Status_Mechanism[com].Send_Para = DEFAULT_COM_PARA;
                            if(os_mbx_send(Status_Mechanism[com].Mbx_Answer_Buf, (void*)Status_Mechanism[com].Rx_Buf, 0) == OS_R_TMO)       //发送邮箱，通知应用程序通讯完成，收到应答
                            {
                                free_mem_block(Status_Mechanism[com].Rx_Buf);               //释放内存块，如果发生说明状态机逻辑有问题
                                Status_Mechanism[com].Rx_Buf = NULL;
                                Status_Mechanism[com].Last_Err_State = ANSWER_MBOX_ERROR;
                            }
                        }
                        else
                        {
                            if(os_mbx_send(Mbx_Comm_Data, (void*)Status_Mechanism[com].Rx_Buf, 0) == OS_R_TMO)      //发送邮箱，通知解析任务有通讯发生，收到外部数据
                            {
                                free_mem_block(Status_Mechanism[com].Rx_Buf);               //释放内存块，如果发生说明解析任务接收队列溢出
                                Status_Mechanism[com].Rx_Buf = NULL;
                                Status_Mechanism[com].Last_Err_State = COMM_MBOX_OVERFLOW;
                            }
                        }
                        Status_Mechanism[com].Rx_Buf = NULL;//azh 120816
                        Status_Mechanism[com].Rec_Mem_Type = NORMAL_TYPE;
                        record_status_mechanism(com, DATA_REC<<4);
                        Status_Mechanism[com].Rec_State = DATA_REC;                         //推动状态机，等待接收数据
                    }
                }
                else
                {
#endif
                if((Status_Mechanism[com].Tmp_Tick_Count - Status_Mechanism[com].Rec_Tick_Count) > Status_Mechanism[com].Byte_Space)
                {
                    record_status_mechanism(com, BYTE_REC_TMO);                         //字节接收超时，记录过程状态机
                    if(com == COM1)//azh 131108
                    {	
                    	set_normal_run_flag(1 << 0);//无线或者内部通信 
                    }
#ifdef GPRS_MODULE_PRJ	//模块版本 本地灯包括红外口和内部通信口	
    #ifdef ZIGBEE_TEST//azh 171011 
					if(com == COM0)//只要红外就行了，zigbee闪灯已经上面处理了
					{	
                    	local_led_time[0] = LOCAL_LED_FLASH_DELAY;//5*100ms
					}
    #else
					if((com == COM0) || (com == COM1))
					{	
                    	local_led_time[0] = LOCAL_LED_FLASH_DELAY;//5*100ms
					}    
    #endif					
#endif  
                    if(__TRUE == check_mbox(Status_Mechanism[com].Mbx_Answer_Buf))      //判断是否是在接收应答
                    {
                        record_status_mechanism(com, CONFIG_DEFAULT_STATUS);            //记录状态机
                        baud_rate_config(com, Status_Mechanism[com].Default_Baud * 300);
                        Status_Mechanism[com].Rec_Baud = DEFAULT_BAUD_RATE;
                        Status_Mechanism[com].Send_Baud = DEFAULT_BAUD_RATE;
                        mode_config(com, Status_Mechanism[com].Default_Para);
                        Status_Mechanism[com].Rec_Para = DEFAULT_COM_PARA;
                        Status_Mechanism[com].Send_Para = DEFAULT_COM_PARA;
                        if(os_mbx_send(Status_Mechanism[com].Mbx_Answer_Buf, (void*)Status_Mechanism[com].Rx_Buf, 0) == OS_R_TMO)       //发送邮箱，通知应用程序通讯完成，收到应答
                        {
                            free_mem_block(Status_Mechanism[com].Rx_Buf);               //释放内存块，如果发生说明状态机逻辑有问题
                            Status_Mechanism[com].Rx_Buf = NULL;
                            Status_Mechanism[com].Last_Err_State = ANSWER_MBOX_ERROR;
                        }
                    }
                    else
                    {
                        if(os_mbx_send(Mbx_Comm_Data, (void*)Status_Mechanism[com].Rx_Buf, 0) == OS_R_TMO)      //发送邮箱，通知解析任务有通讯发生，收到外部数据
                        {
                            free_mem_block(Status_Mechanism[com].Rx_Buf);               //释放内存块，如果发生说明解析任务接收队列溢出
                            Status_Mechanism[com].Rx_Buf = NULL;
                            Status_Mechanism[com].Last_Err_State = COMM_MBOX_OVERFLOW;
                        }
                    }
                    Status_Mechanism[com].Rx_Buf = NULL;//azh 120816
                    Status_Mechanism[com].Rec_Mem_Type = NORMAL_TYPE;
                    record_status_mechanism(com, WAIT_REC<<4);
                    Status_Mechanism[com].Rec_State = WAIT_REC;                         //推动状态机，等待接收数据
                }
#ifdef ZIGBEE_TEST
                }
#endif
                break;
            default:
#ifdef ZIGBEE_TEST
                if(com == COM1)
                {
                    record_status_mechanism(com, DATA_REC << 4);
                    Status_Mechanism[com].Rec_State = DATA_REC;
                    Status_Mechanism[com].Last_Err_State = NO_MATCH_STATUS;
                }
                else
                {
#endif// ZIGBEE_TEST
                    record_status_mechanism(com, WAIT_REC << 4);
                    Status_Mechanism[com].Rec_State = WAIT_REC;
                    Status_Mechanism[com].Last_Err_State = NO_MATCH_STATUS;
#ifdef ZIGBEE_TEST
                }
#endif// ZIGBEE_TEST
                break;
            }
            switch(Status_Mechanism[com].Send_State)                //处理发送状态机
            {
            case COM_IDLE:                  //空闲
                if((Status_Mechanism[com].Tmp_Tick_Count - Status_Mechanism[com].Idle_Tick_Count) > (OS_DLY_1S * 60 * 60 * 24))     //1天中没有操作串口
                {
                    Status_Mechanism[com].Idle_Tick_Count = Status_Mechanism[com].Tmp_Tick_Count;
                    set_RS485_rec(com);     //如果是RS485，接收控制放在这里
                    switch(com)
                    {
                    case 0:
                        status_mechanism_init(0);
                        UART0_init(Status_Mechanism[0].Default_Para, Status_Mechanism[0].Default_Baud * 300);       //初始化串口0通讯参数
                        break;
                    case 1:
#ifdef ZIGBEE_TEST//azh171011 CODE_VER_TYPE==H_METER_PRJ		//配高压表
                        WLS_RST_LOW();
#endif
                        status_mechanism_init(1);
//azh 120816                        UART1_init(Status_Mechanism[1].Default_Para, Status_Mechanism[1].Default_Baud * 300);       //初始化串口1通讯参数
//                        Zigbee_DMA_Init();//azh
#ifdef ZIGBEE_TEST//azh171011 CODE_VER_TYPE==H_METER_PRJ		//配高压表
                        WLS_RST_HIGH();
#endif                        
                        break;
                    case 2:
                        status_mechanism_init(2);
                        UART2_init(Status_Mechanism[2].Default_Para, Status_Mechanism[2].Default_Baud * 300);       //初始化串口2通讯参数
                        break;
                    case 3:
                        status_mechanism_init(3);
                        UART3_init(Status_Mechanism[3].Default_Para, Status_Mechanism[3].Default_Baud * 300);       //初始化串口3通讯参数
                        break;
                    }
                    Status_Mechanism[com].Last_Err_State = IDEL_TIMEOUT;
                }
                else
                {
#ifdef ZIGBEE_TEST
                    if(com == COM1)
                    {
                        if((index = query_zigbee_send_event()) < ZIGBEE_SEND_MEM_NUM)      //检查等待发送事件
                        {
                            Status_Mechanism[com].Index = index;//azh 增加这个变量
                            if(Zigbee_Send_Status[index].Len > MAX_ZIGBEE_FRAME_LEN + 24)       //检查要发送的字节总数
                            {
                                Zigbee_Send_Status[index].Len = MAX_ZIGBEE_FRAME_LEN + 24;
                            }
                            //Status_Mechanism[com].Tx_Buf = Zigbee_Send_Status[index].Buf;
                            record_status_mechanism(com, COM_READY);
                            Status_Mechanism[com].Send_State = COM_READY;//zigbee发送用了一个专门的状态机 所以没有COM_READY状态
                        }
                        //查询队列 折包
                        mem_ptr = NULL;//azh 120413
                        if(os_mbx_wait(Status_Mechanism[com].Mbx_Send_Buf, (void**)&mem_ptr, 0) != OS_R_TMO)    //检查发送队列,并分解成zigbee的多包
                        {
                            deal_with_zigbee_send(mem_ptr);//azh 全用zigbee发送状态结构变量控制
                        }
                    }
                    else
                    {
#endif// ZIGBEE_TEST
                        mem_ptr = NULL;//azh 120413
                        if(os_mbx_wait(Status_Mechanism[com].Mbx_Send_Buf, (void**)&mem_ptr, 0) != OS_R_TMO)    //检查发送队列
                        {
                            Status_Mechanism[com].Tx_Buf = mem_ptr;
                            if(DEFAULT_BAUD_RATE != Status_Mechanism[com].Send_Baud)    //判断发送时是否需要修改波特率
                            {
                                record_status_mechanism(com, CONFIG_SEND_STATUS);       //记录状态机
                                baud_rate_config(com, Status_Mechanism[com].Send_Baud * 300);
                            }
                            if(DEFAULT_COM_PARA != Status_Mechanism[com].Send_Para)     //判断发送时是否需要修改校验位、停止位、数据长度
                            {
                                record_status_mechanism(com, CONFIG_SEND_STATUS);       //记录状态机
                                mode_config(com, Status_Mechanism[com].Send_Para);
                            }
                            set_RS485_send(com);            //如果是RS485，发送控制放在这里
                            record_status_mechanism(com, COM_READY);
                            Status_Mechanism[com].Send_State = COM_READY;
                        }
#ifdef ZIGBEE_TEST
                    }
#endif// ZIGBEE_TEST
                }
                break;
            case COM_READY:         //准备通讯
                Status_Mechanism[com].Data_Send_Num = 0;
                Status_Mechanism[com].Send_Tick_Count = Status_Mechanism[com].Tmp_Tick_Count;
                record_status_mechanism(com, DATA_SEND);
                Status_Mechanism[com].Send_State = DATA_SEND;
                Meter_LED_Cnt[com] = LED_FLASH_TIMES;       //指示发送
                switch(com)
                {
                case 0:
                    U0THR = Status_Mechanism[0].Tx_Buf[DATA_OFFSET];
                    Status_Mechanism[0].Data_Send_Num++;
                    Status_Mechanism[com].Send_Tick_Space = OS_DLY_50MS * (Status_Mechanism[com].Tx_Buf[0] | (Status_Mechanism[com].Tx_Buf[1] << 8));//azh 140807 
                    U0IER |= 0x02;  //使能发送中断
#ifdef GPRS_MODULE_PRJ//模块版本 本地灯包括红外口和内部通信口	
                    local_led_time[1] = LOCAL_LED_FLASH_DELAY;//5*100ms
#endif  
                    break;
                case 1:
#ifdef ZIGBEE_TEST
                    U1THR = Zigbee_Send_Status[Status_Mechanism[1].Index].Buf[0];
                    //zigbee_led_time[1] = ZIGBEE_LED_FLASH_DELAY;//5*100ms
                    local_led_time[1] = LOCAL_LED_FLASH_DELAY;//5*100ms //azh 171011
                    Status_Mechanism[com].Send_Tick_Space = OS_DLY_10MS * Zigbee_Send_Status[Status_Mechanism[com].Index].Len;//azh 140807 
#else
                    U1THR = Status_Mechanism[1].Tx_Buf[DATA_OFFSET];
                    local_led_time[1] = LOCAL_LED_FLASH_DELAY;//5*100ms
                    Status_Mechanism[com].Send_Tick_Space = OS_DLY_50MS * (Status_Mechanism[com].Tx_Buf[0] | (Status_Mechanism[com].Tx_Buf[1] << 8));//azh 140807                 
#endif//ZIGBEE_TEST
                    Status_Mechanism[1].Data_Send_Num++;
                    U1IER |= 0x02;  //使能发送中断
                    break;
                case 2:
                    U2THR = Status_Mechanism[2].Tx_Buf[DATA_OFFSET];
                    Status_Mechanism[2].Data_Send_Num++;
                    Status_Mechanism[com].Send_Tick_Space = OS_DLY_50MS * (Status_Mechanism[com].Tx_Buf[0] | (Status_Mechanism[com].Tx_Buf[1] << 8));//azh 140807 
                    U2IER |= 0x02;  //使能发送中断
                    break;
                case 3:
                    U3THR = Status_Mechanism[3].Tx_Buf[DATA_OFFSET];
                    Status_Mechanism[3].Data_Send_Num++;
                    Status_Mechanism[com].Send_Tick_Space = OS_DLY_50MS * (Status_Mechanism[com].Tx_Buf[0] | (Status_Mechanism[com].Tx_Buf[1] << 8));//azh 140807 
                    U3IER |= 0x02;  //使能发送中断
                    break;
                }
                break;
            case DATA_SEND:         //发送
#ifdef ZIGBEE_TEST
                if(com == COM1)
                {
                    if(Status_Mechanism[com].Tmp_Tick_Count - Status_Mechanism[com].Send_Tick_Count > WAIT_ANSWER_100MS * 3)
                    {
                        free_zigbee_send_mem(Status_Mechanism[com].Index);    //释放内存块

                        status_mechanism_init(com);
                        UART1_init(Status_Mechanism[com].Default_Para, Status_Mechanism[com].Default_Baud * 300);       //初始化串口1通讯参数     //初始化串口1通讯参数
//                        Zigbee_DMA_Init();//azh

                        Status_Mechanism[com].Last_Err_State = SEND_TIMEOUT;
                        Status_Mechanism[com].Tx_Buf = NULL;//
                        Status_Mechanism[com].Idle_Tick_Count = Status_Mechanism[com].Tmp_Tick_Count;
                        record_status_mechanism(com, COM_IDLE);                      //推动状态机
                        Status_Mechanism[com].Send_State = COM_IDLE;
                    }
//                    else if((DMACC3Config & (1 << 17)) == 0)
//                    {
//
//                        if((Zigbee_Send_Status[Status_Mechanism[com].Index].Flag & WAIT_RESEND) == 0)
//                        {
//                            free_zigbee_send_mem(Status_Mechanism[com].Index);        //是发给模块的命令不需要等待重发，释放内存块
//                        }
//                        Status_Mechanism[com].Index = 0xff;
//                        Status_Mechanism[com].Send_End_Tick_Count = OS_Time;//Sys_Tick;
//                        record_status_mechanism(com, DATA_SEND_END);
//                        Status_Mechanism[com].Send_State = DATA_SEND_END;
//                    }
                }
                else
                {
#endif// ZIGBEE_TEST
                    if((Status_Mechanism[com].Tmp_Tick_Count - Status_Mechanism[com].Send_Tick_Count)
                     > (Status_Mechanism[com].Send_Tick_Space))//azh 140807 * (Status_Mechanism[com].Tx_Buf[0] | (Status_Mechanism[com].Tx_Buf[1] << 8))))        //设置值×数据个数
                    {
                        status_mechanism_init(com);//add
                        Status_Mechanism[com].Last_Err_State = SEND_TIMEOUT;
                        switch(com)
                        {
                        case 0:
                            UART0_init(Status_Mechanism[0].Default_Para, Status_Mechanism[0].Default_Baud * 300);       //初始化串口0通讯参数
                            break;
                        case 1:
//                            UART1_init(Status_Mechanism[1].Default_Para, Status_Mechanism[1].Default_Baud * 300);       //初始化串口1通讯参数
                            break;
                        case 2:
                            UART2_init(Status_Mechanism[2].Default_Para, Status_Mechanism[2].Default_Baud * 300);       //初始化串口2通讯参数
                            break;
                        case 3:
                            UART3_init(Status_Mechanism[3].Default_Para, Status_Mechanism[3].Default_Baud * 300);       //初始化串口3通讯参数
                            break;
                        }
                        free_mem_block(Status_Mechanism[com].Tx_Buf);                           //释放内存块
                        Status_Mechanism[com].Tx_Buf = NULL;
                        Status_Mechanism[com].Idle_Tick_Count = Status_Mechanism[com].Tmp_Tick_Count;
                        record_status_mechanism(com, COM_IDLE);                                 //推动状态机
                        Status_Mechanism[com].Send_State = COM_IDLE;
                    }
#ifdef ZIGBEE_TEST
                }
#endif// ZIGBEE_TEST
                break;
            case DATA_SEND_END:     //发送完成，数据打入FIFO
                flag = 0;
#ifdef ZIGBEE_TEST
                if(com == 1)
                {
//                    if(((U1LSR & (1 << 6)) != 0)                                        //检查移位寄存器空，真正发送完毕
//                        || ((OS_Time - Status_Mechanism[com].Send_End_Tick_Count) > (Status_Mechanism[com].Send_Tick_Space * 16)))   //发送超时，设置值×数据个数（最多一个FIFO的数据16字节）
                    if((U1LSR & (1 << 6)) != 0)
                    {
                        flag = 1;
                    }
                }
                else
                {
#endif// ZIGBEE_TEST
                    if((((U0LSR&(1<<6)) != 0) && (com == 0)) || (((U1LSR&(1<<6)) != 0) && (com == 1))
                        || (((U2LSR&(1<<6)) != 0) && (com == 2)) || (((U3LSR&(1<<6)) != 0) && (com == 3)))
                    {
                        flag = 1;
                    }
#ifdef ZIGBEE_TEST
                }
#endif// ZIGBEE_TEST
                if(flag == 1)
                {
                    set_RS485_rec(com);     //如果是RS485，接收控制放在这里
                    if(__TRUE != check_mbox(Status_Mechanism[com].Mbx_Answer_Buf))      //判断是否有应答
                    {
                        //azh 111216
                        if(DEFAULT_BAUD_RATE != Status_Mechanism[com].Send_Baud)             //判断发送时是否修改了波特率
                        {
                            record_status_mechanism(com, CONFIG_DEFAULT_STATUS);             //记录状态机
                            baud_rate_config(com, Status_Mechanism[com].Default_Baud * 300);
                        }
                        if(DEFAULT_COM_PARA != Status_Mechanism[com].Send_Para)              //判断发送时是否修改了校验位、停止位、数据长度
                        {
                            record_status_mechanism(com, CONFIG_DEFAULT_STATUS);             //记录状态机
                            mode_config(com, Status_Mechanism[com].Default_Para);
                        }
                    }
                    else
                    {
                        if(DEFAULT_BAUD_RATE != Status_Mechanism[com].Rec_Baud)         //判断接收应答时是否需要修改波特率
                        {
                            record_status_mechanism(com, CONFIG_REC_STATUS<<4);         //记录状态机
                            baud_rate_config(com, Status_Mechanism[com].Rec_Baud * 300);
                        }
                        if(DEFAULT_COM_PARA != Status_Mechanism[com].Rec_Para)          //判断接收应答时是否需要修改校验位、停止位、数据长度
                        {
                            record_status_mechanism(com, CONFIG_REC_STATUS<<4);         //记录状态机
                            mode_config(com, Status_Mechanism[com].Rec_Para);
                        }
                    }
                    Status_Mechanism[com].Dly_Tick_Count = Status_Mechanism[com].Tmp_Tick_Count;
                    record_status_mechanism(com, WAIT_NEXT);
                    Status_Mechanism[com].Send_State = WAIT_NEXT;                       //结束本次通讯
                }
                break;
            case WAIT_NEXT:         //延时，等待下次通讯
                if((Status_Mechanism[com].Tmp_Tick_Count - Status_Mechanism[com].Dly_Tick_Count) > Status_Mechanism[com].Dly_Tick_Space)
                {
                    Status_Mechanism[com].Idle_Tick_Count = Status_Mechanism[com].Tmp_Tick_Count;
                    record_status_mechanism(com, COM_IDLE);
                    Status_Mechanism[com].Send_State = COM_IDLE;                    //推动状态机，查询是否有新的通讯请求
                }
                break;
            }
        }
    }
}

/******************************************************************************
 Function name:  S8 store_comm_data (U8 com, U32 data)
 Author       :  snap.gao
 Description  :  保存串口接收到的数据
 Input        :  com :串口号
                 data:串口数据
 Return       :  内存块地址不合法时返回__FAIL
                 正确保存和内存块溢出错误时都返回__SUCCEED
 *****************************************************************************/
S8 store_comm_data (U8 com, U32 data)
{
    U8  len,j;
    U16 *len_ptr;
    U16 total_len;

    if(Status_Mechanism[com].Rx_Buf == NULL)
    {
        return __FAIL;
    }
    len = (data >> 28) & 0x03;          //数据长度，最大3个
    if(len == 0)                        //数据长度不能是‘0’
    {
        return __FAIL;
    }

    switch(check_mem_type(Status_Mechanism[com].Rx_Buf))
    {
    case MEM_64_TYPE:
        total_len = MEM_64_SIZE;
        break;
    case MEM_256_TYPE:
        total_len = MEM_256_SIZE;
        break;
    case MEM_1536_TYPE:
        total_len = MEM_1536_SIZE;
        break;
    default:
        total_len = MEM_64_SIZE;        //默认最小的长度
        break;
    }

    len_ptr = (U16*)&Status_Mechanism[com].Rx_Buf[DATA_LEN_OFFSET];
    for(j = 0; j < len; j++)
    {
        if((*len_ptr) < (total_len - DATA_OFFSET))      //检查是否超出内存块范围
        {
            Status_Mechanism[com].Rx_Buf[DATA_OFFSET + *len_ptr] = data >> ((len - 1 - j) * 8);     //保存一个数据
            (*len_ptr)++;               //增加长度
        }
        else
        {
            Status_Mechanism[com].Last_Err_State = LEN_OVERFLOW;
        }
    }
    return __SUCCEED;
}

/******************************************************************************
 Function name:  void record_status_mechanism(U8 com, U8 mechanism)
 Author       :  snap.gao
 Description  :  记录一组调试用状态机，发布时请用条件编译避免
 Input        :  com      : 串口号
                 mechanism: 状态机
 Return       :  None
 *****************************************************************************/
void record_status_mechanism (U8 com, U8 mechanism)
{
#ifdef __UART_DEBUG
    if((Mechanism_Record[0] == 0) || (Mechanism_Record[0] >= MECHANISM_RECORD_LEN))
    {
        Mechanism_Record[0] = 1;
    }
    Mechanism_Record[Mechanism_Record[0]] = ((OS_Time & 0xffff) | (mechanism<<16) | (com<<24));
    Mechanism_Record[0]++;
#endif//__UART_DEBUG
}

/******************************************************************************
 Function name:  void set_RS485_rec (U8 com)
 Author       :  snap.gao
 Description  :  RS485设置为接收
 Input        :  com: 串口号，0->MAX_PORT_NUM-1
 Return       :  None
 *****************************************************************************/
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//带CPU_GPRS模块
//此模块没有用到485 是内部一个串口直接经光耦通信
void set_RS485_rec (U8 com)
{
/*
    switch(com)
    {
    case 0: //485_1
        SET_PORT0_HIGH(P0_GPRS_4851_CTL);  //置为接收
        SET_PORT0_OUT(P0_GPRS_4851_CTL);
        break;
    case 1:
        break;
    case 2: //485_2
//        SET_PORT1_HIGH(P1_SW4852);  //置为接收
//        SET_PORT1_OUT(P1_SW4852);
        break;
    case 3:
        break;
    }
*/
}
#else//H_METER_PRJ,WLS_CUR_PRJ
void set_RS485_rec (U8 com)
{
    switch(com)
    {
    case 0: //485_1
        SET_PORT1_HIGH(P1_SW4851);  //置为接收
        SET_PORT1_OUT(P1_SW4851);
        break;
    case 1:
        break;
    case 2: //485_2
        SET_PORT1_HIGH(P1_SW4852);  //置为接收
        SET_PORT1_OUT(P1_SW4852);
        break;
    case 3:
        break;
    }
}
#endif
/******************************************************************************
 Function name:  void set_RS485_send (U8 com)
 Author       :  snap.gao
 Description  :  RS485设置为发送
 Input        :  com: 串口号，0->MAX_PORT_NUM-1
 Return       :  None
 *****************************************************************************/
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//带CPU_GPRS模块
void set_RS485_send (U8 com)
{
/*
    switch(com)
    {
    case 0: //485_1
        SET_PORT0_LOW(P0_GPRS_4851_CTL);   //置为发送
        SET_PORT0_OUT(P0_GPRS_4851_CTL);
        break;
    case 1:
        break;
    case 2://485_2
//        SET_PORT1_LOW(P1_SW4852);   //置为发送 输出低到485芯片为高 发送
//        SET_PORT1_OUT(P1_SW4852);
        break;
    case 3:
        break;
    }
*/
}

#else
void set_RS485_send (U8 com)
{
    switch(com)
    {
    case 0: //485_1
        SET_PORT1_LOW(P1_SW4851);   //置为发送
        SET_PORT1_OUT(P1_SW4851);
        break;
    case 1:
        break;
    case 2://485_2
        SET_PORT1_LOW(P1_SW4852);   //置为发送 输出低到485芯片为高 发送
        SET_PORT1_OUT(P1_SW4852);
        break;
    case 3:
        break;
    }
}
#endif
/******************************************************************************
 Function name:  void init_frame_info (U8 *info)
 Author       :  snap.gao
 Description  :  初始化UART通讯帧的头信息，不包括长度和端口号
 Input        :  info: 通讯帧起始地址
 Return       :  None
 *****************************************************************************/
void init_frame_info (U8 *info)
{
    info[SEND_BAUD_OFFSET] = (U8)DEFAULT_BAUD_RATE;
    info[SEND_BAUD_OFFSET + 1] = (U8)DEFAULT_BAUD_RATE;     //默认波特率
    info[SEND_PARA_OFFSET] = DEFAULT_COM_PARA;              //默认参数
    info[REC_BAUD_OFFSET] = (U8)DEFAULT_BAUD_RATE;
    info[REC_BAUD_OFFSET + 1] = (U8)DEFAULT_BAUD_RATE;      //默认波特率
    info[REC_PARA_OFFSET] = DEFAULT_COM_PARA;               //默认参数
    info[REC_MEM_TYPE_OFFSET] = NORMAL_TYPE;                //默认不指定接收应答内存块的类型
    info[DEAL_FLAG_OFFSET] = 0;                             //清除所有特殊处理标志
}

/******************************************************************************
 Function name:  void set_frame_len (U8 *info, U16 len)
 Author       :  snap.gao
 Description  :  设置通讯帧的长度
 Input        :  info: 通讯帧起始地址
                 len : 数据长度
 Return       :  None
 *****************************************************************************/
void set_frame_len (U8 *info, U16 len)
{
    info[DATA_LEN_OFFSET] = len;
    info[DATA_LEN_OFFSET + 1] = len >> 8;           //设置长度
}

/******************************************************************************
 Function name:  void set_port_num (U8 *info, U8 port)
 Author       :  snap.gao
 Description  :  设置通讯帧的串口号0->xx
 Input        :  info: 通讯帧起始地址
                 port: 串口号
 Return       :  None
 *****************************************************************************/
void set_port_num (U8 *info, U8 port)
{
    info[COM_NUM_OFFSET] = port;                    //设置串口号
}

/******************************************************************************
 Function name:  void set_send_para (U8 *info, U8 para, U16 baud)
 Author       :  snap.gao
 Description  :  设置通讯串口发送时的参数
 Input        :  info: 通讯帧起始地址
                 para: 串口参数
                 baud: 串口波特率
 Return       :  None
 *****************************************************************************/
void set_send_para (U8 *info, U8 para, U16 baud)
{
    info[SEND_BAUD_OFFSET] = baud;
    info[SEND_BAUD_OFFSET + 1] = baud >> 8;         //设置波特率
    info[SEND_PARA_OFFSET] = para;                  //设置参数
}

/******************************************************************************
 Function name:  void set_rec_para (U8 *info, U8 para, U16 baud)
 Author       :  snap.gao
 Description  :  设置通讯串口接收应答时的参数
 Input        :  info: 通讯帧起始地址
                 para: 串口参数
                 baud: 串口波特率
 Return       :  None
 *****************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不带CPU_GPRS模块
void set_rec_para (U8 *info, U8 para, U16 baud)
{
    info[REC_BAUD_OFFSET] = baud;
    info[REC_BAUD_OFFSET + 1] = baud >> 8;          //设置波特率
    info[REC_PARA_OFFSET] = para;                   //设置参数
}
#endif
/******************************************************************************
 Function name:  void set_deal_flag (U8 *info, U8 flag)
 Author       :  snap.gao
 Description  :  设置通讯帧的特殊处理标志
 Input        :  info: 通讯帧起始地址
                 flag: 特殊处理标志，按bit定义
 Return       :  None
 *****************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不带CPU_GPRS模块
void set_deal_flag (U8 *info, U8 flag)
{
    info[DEAL_FLAG_OFFSET] |= flag;                 //设置特殊处理标志
}
#endif
/******************************************************************************
 Function name:  void UART_send_push (U8 com)
 Author       :  snap.gao
 Description  :  在中断把数据都发出后（打入FIFO），把状态机由DATA_SEND推到DATA_SEND_END
 Input        :  com: 串口号
 Return       :  None
 *****************************************************************************/
void UART_send_push (U8 com)
{
    if(DATA_SEND == Status_Mechanism[com].Send_State)
    {
        Status_Mechanism[com].Send_End_Tick_Count = OS_Time;
        record_status_mechanism(com, DATA_SEND_END);                   //记录状态机
        Status_Mechanism[com].Send_State = DATA_SEND_END;
    }
}

/******************************************************************************
 Function name:  void UART_mode_config (VU32 *UARTx, U8 mode)
 Author       :  snap.gao
 Description  :  设置串口通讯参数：数据位，校验位，停止位
 Input        :  UARTx : UART0->UART3
                 mode  : 串口参数（数据位+校验位+停止位）
 Return       :  None
 *****************************************************************************/
void UART_mode_config (VU32 *UARTx, U8 mode)
{
    UARTx[LCR] &= 0x80;
    UARTx[LCR] |= mode;
}

/******************************************************************************
 Function name:  void UART_baud_rate_config (VU32 *UARTx, U32 baud)
 Author       :  snap.gao
 Description  :  设置串口通讯波特率：300～115200bps，高波特率下将产生较大的误差
 Input        :  UARTx : UART0->UART3
                 baud  : 波特率
 Return       :  None
 *****************************************************************************/
/*void UART_baud_rate_config (VU32 *UARTx, U32 baud)
{
    U16 dl;

    if((baud < 300) || (baud > 115200)){
        return;
    }

    dl = PCLK / 16 / baud;          // 计算波特率

    disable_irq();
    UARTx[LCR] |= 0x80;             // 令DLAB位为1

    U0FDR = 0x10;                   // 小数分频器禁能，令DIVADDVAL=0，MULVAL=0(不可为0)

    UARTx[DLM] = dl / 256;
    UARTx[DLL] = dl % 256;

    UARTx[LCR] &= ~0x80;            // 令DLAB位为0
    enable_irq();
}
*/
//azh 110516
/******************************************************************************
** 函数名称: void UART_baud_rate_config(VU32 *UARTx, U32 baud)
** 函数功能: 设置串口通讯波特率：300～115200bps，高波特率下可能产生较大的误差
** 入口参数: UARTx: UART0->UART3
**           baud : 波特率
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void UART_baud_rate_config (VU32 *UARTx, U32 baud)
{
    U16 dl;

    if((baud < 300) || (baud > 115200))
    {
        return;
    }

    dl = FCCLK / 2 / 16 / baud;     //计算波特率，4个串口的外设时钟都是2分频
//azh 111223
//    __disable_irq();
    UARTx[LCR] |= 0x80;             //令DLAB位为1，允许设置波特率

    UARTx[FDR] = 0x10;              //小数分频器禁能，令DIVADDVAL=0，MULVAL=0(不可为0)

    UARTx[DLM] = dl / 256;
    UARTx[DLL] = dl % 256;

    UARTx[LCR] &= ~0x80;            //令DLAB位为0
//    __enable_irq();
}
/******************************************************************************
 Function name:  void UART_mode_config (VU32 *UARTx, U8 mode)
 Author       :  snap.gao
 Description  :  在通讯时改变串口通讯参数：数据位，校验位，停止位
 Input        :  com : 0->xx
                 mode: 串口参数（数据位+校验位+停止位）
 Return       :  None
 *****************************************************************************/
/*
void mode_config (U8 com, U8 mode)
{
    switch(com)
    {
    case 0:
        UART_mode_config(UART0, mode);      //修改参数
        break;
    case 1:
        UART_mode_config(UART1, mode);      //修改参数
        break;
    case 2:
        UART_mode_config(UART2, mode);      //修改参数
        break;
    case 3:
        UART_mode_config(UART3, mode);      //修改参数
        break;
    }
}
*/
/******************************************************************************
** 函数名称: void mode_config(U8 com, U8 mode)
** 函数功能: 设置串口通讯参数：数据位，校验位，停止位
** 入口参数: com : 0->MAX_PORT_NUM-1
**           mode: 串口参数（数据位+校验位+停止位）
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void mode_config (U8 com, U8 mode)
{
    switch(com)
    {
    case 0:
        U0LCR = mode & 0x3f;        //修改参数
        break;
    case 1:
        U1LCR = mode & 0x3f;        //修改参数
        break;
    case 2:
        U2LCR = mode & 0x3f;        //修改参数
        break;
    case 3:
        U3LCR = mode & 0x3f;        //修改参数
        break;
    }
}
/******************************************************************************
 Function name:  void UART_baud_rate_config (VU32 *UARTx, U32 baud)
 Author       :  snap.gao
 Description  :  在通讯时改变串口通讯波特率
 Input        :  com : 0->xx
                 baud: 波特率
 Return       :  None
 *****************************************************************************/
void baud_rate_config (U8 com, U32 baud)
{
    switch(com)
    {
    case 0:
        UART_baud_rate_config(UART0, baud);     //修改波特率
        break;
    case 1:
        UART_baud_rate_config(UART1, baud);     //修改波特率
        break;
    case 2:
        UART_baud_rate_config(UART2, baud);     //修改波特率
        break;
    case 3:
        UART_baud_rate_config(UART3, baud);     //修改波特率
        break;
    }
}

/******************************************************************************
 Function name:  void UART0_init (U8 mode, U32 baud)
 Author       :  snap.gao
 Description  :  初始化串口0参数到一个默认值：数据位，校验位，停止位，波特率
 Input        :  mode  : 串口参数（数据位+校验位+停止位）
                 baud  : 波特率
 Return       :  None
 *****************************************************************************/
/*
void UART0_init (U8 mode, U32 baud)
{
    PCONP |= (1 << PCUART0);                        // 使能UART0
    VICIntEnClr |= (1 << UART0_INT);                // 初始化UART0中断VIC接口
    U0IER = 0;                                      // 初始化中断

    PINSEL0  &= ~((0x03u << 4) | (0x03u << 6));     // 先清除相关bits
    PINSEL0  |= ((FUN1 << 4) | (FUN1 << 6));        // 初始化相关的I/O，设置P0.2和P0.3连接到UART0

    UART_mode_config(UART0, mode);                  // 初始化串口通讯参数
    UART_baud_rate_config(UART0, baud);             // 初始化串口波特率

    U0FCR = 0x04;                       // 复位FIFO
    U0FCR = 0x01 | (2 << 6);            // 使能FIFO，设置FIFO深度为8
    U0ACR = 0;                          // 初始化自动波特率控制寄存
    U0IER = 0x01;                       // 使能接收中断

    VICVectPriority6 = UART0_ISR_PRIO;
    VICVectAddr6 = (U32)UART0_IRQ_handler;
    VICIntEnable |= (1 << UART0_INT);   // 允许UART0中断
}
*/
//azh 110516
/******************************************************************************
** 函数名称: void UART0_init(U8 mode, U32 baud)
** 函数功能: 初始化串口0参数到一个默认值：数据位，校验位，停止位，波特率
** 入口参数: mode: 串口参数（数据位+校验位+停止位）
**           baud: 波特率
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void UART0_init (U8 mode, U32 baud)
{
    PCONP |= (1 << PCUART0);                        //使能UART0
//    DISABLE_ISR(NVIC_UART0);                        //disbale UART0 interrupt
//isr_clr(NVIC_UART0);

    U0IER = 0;                                      //初始化中断

    PCLKSEL0 &= ~(0x03u << 6);
    PCLKSEL0 |= (FPCLK_UART0_DIV << 6);             //设置外设时钟分频系数

    if((PINSEL0 & ((0x03u << 4) | (0x03u << 6))) != ((P0_3_FNUC << 6) | (P0_2_FNUC << 4)))
    {
        PINSEL0 &= ~((0x03u << 4) | (0x03u << 6));              //先清除相关bits
        PINSEL0 |= ((P0_3_FNUC << 6) | (P0_2_FNUC << 4));       //configure the TXD0 and RXD0 as the alternate function 1 configuration
    }
    PINMODE0 &= ~((0x03u << 4) | (0x03u << 6));                 //先清除相关bits
    PINMODE0 |= ((P0_3_MODE << 6) | (P0_2_MODE << 4));          //TXD0 and RXD0 has neither pull-up nor pull-down registers
    PINMODE_OD0 &= ~(P0_TXD_4851 | P0_RXD_4851);                  //设置引脚非开漏模式

    UART_baud_rate_config(UART0, baud);             //初始化串口波特率
    U0LCR = mode & 0x3f;;                           //初始化串口通讯参数

    U0FCR = 0x01 | 0x06 | (1 << 3) | (2 << 6);      //复位/使能FIFO，设置FIFO深度为8，使能DMA
    U0RBR;
    U0ACR = 0;                                      //初始化自动波特率控制寄存
    U0IER = 0x01;                                   //使能接收中断

//    ENABLE_ISR(NVIC_UART0, PRIO_UART0);             //set UART0 interrupt parameter
//isr_set(NVIC_UART0, PRIO_UART0);                      //azh 110609 在MAIN函数中初始化
}
/******************************************************************************
 Function name:  void UART1_init (U8 mode, U32 baud)
 Author       :  snap.gao
 Description  :  初始化串口1参数到一个默认值：数据位，校验位，停止位，波特率
 Input        :  mode  : 串口参数（数据位+校验位+停止位）
                 baud  : 波特率
 Return       :  None
 *****************************************************************************/
/*
void UART1_init (U8 mode, U32 baud)
{
    PCONP |= (1 << PCUART1);                        // 使能UART1
    VICIntEnClr |= (1 << UART1_INT);                // 初始化UART1中断VIC接口
    U1IER = 0;                                      // 初始化中断

    PINSEL0  &= ~(0x03u << 30);                     // 先清除相关bits
    PINSEL1  &= ~(0x03u << 0);                      // 先清除相关bits
    PINSEL0  |= (FUN1 << 30);                       // 初始化相关的I/O，设置P0.15连接到UART1
    PINSEL1  |= (FUN1 << 0);                        // 初始化相关的I/O，设置P0.16连接到UART1

    UART_mode_config(UART1, mode);                  // 初始化串口通讯参数
    UART_baud_rate_config(UART1, baud);             // 初始化串口波特率

    U1FCR = 0x04;                       // 复位FIFO
    U1FCR = 0x01 | (2 << 6);            // 使能FIFO，设置FIFO深度为8
    U1ACR = 0;                          // 初始化自动波特率控制寄存
    U1IER = 0x01;                       // 使能接收中断

    VICVectPriority7 = UART1_ISR_PRIO;
    VICVectAddr7 = (U32)UART1_IRQ_handler;
    VICIntEnable |= (1 << UART1_INT);   // 允许UART1中断
}
*/
/******************************************************************************
** 函数名称: void UART1_init(U8 mode, U32 baud)
** 函数功能: 初始化串口1参数到一个默认值：数据位，校验位，停止位，波特率
** 入口参数: mode: 串口参数（数据位+校验位+停止位）
**           baud: 波特率
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void UART1_init (U8 mode, U32 baud)
{
    PCONP |= (1 << PCUART1);                        //使能UART1
//    DISABLE_ISR(NVIC_UART1);                        //disbale UART1 interrupt
    U1IER = 0;                                      //初始化中断

    PCLKSEL0 &= ~(0x03u << 8);
    PCLKSEL0 |= (FPCLK_UART1_DIV << 8);             //设置外设时钟分频系数
#if CODE_VER_TYPE==H_METER_PRJ				//配合高压表
    if((PINSEL4 & ((0x03u << 14) | (0x03u << 4) | (0x03u << 2) | (0x03u << 0)))
      != ((P2_7_FNUC << 14) | (P2_2_FNUC << 4) | (P2_1_FNUC << 2) | (P2_0_FNUC << 0)))
    {
        PINSEL4 &= ~((0x03u << 14) | (0x03u << 4) | (0x03u << 2) | (0x03u << 0));                   //先清除相关bits
        PINSEL4 |= ((P2_7_FNUC << 14) | (P2_2_FNUC << 4) | (P2_1_FNUC << 2) | (P2_0_FNUC << 0));    //configure the RTS1 CTS1 TXD1 and RXD1 as the alternate function 1 configuration
    }
    PINMODE4 &= ~((0x03u << 14) | (0x03u << 4) | (0x03u << 2) | (0x03u << 0));                      //先清除相关bits
    PINMODE4 |= ((P2_7_MODE << 14) | (P2_2_MODE << 4) | (P2_1_MODE << 2) | (P2_0_MODE << 0));       //RTS1 CTS1 TXD1 and RXD1 has neither pull-up nor pull-down registers
    PINMODE_OD2 &= ~(P2_RTS_WLS | P2_CTS_WLS | P2_TXD_WLS | P2_RXD_WLS);                            //设置引脚非开漏模式

    UART_baud_rate_config(UART1, baud);             //初始化串口波特率
    U1LCR = mode & 0x3f;                            //初始化串口通讯参数

    U1FCR = 0x01 | 0x06 | (1 << 3) | (2 << 6);      //复位/使能FIFO，设置FIFO深度为8，使能DMA
    U1RBR;
    U1ACR = 0;                                      //初始化自动波特率控制寄存
//#ifdef PHASE_C_MODULE
    U1MCR = (1 << 6) | (1 << 7);                    //使能自动CTS和RTS
    U1TER = (1 << 7);                               //使能软流控的发送

//#endif//PHASE_C_MODULE
    U1RS485CTRL = 0;                                //禁止RS485控制功能
    U1IER = 0x01;                                   //使能接收中断

//  WLS_RST_HIGH();                                 //RST_WLS输出‘1’,ZigBee复位禁能
//    ENABLE_ISR(NVIC_UART1, PRIO_UART1);             //set UART1 interrupt parameter

#elif CODE_VER_TYPE==WLS_CUR_PRJ			//配合无线采集器
    if((PINSEL4 & ((0x03u << 14) | (0x03u << 4) | (0x03u << 2) | (0x03u << 0)))
      != ((P2_7_FNUC << 14) | (P2_2_FNUC << 4) | (P2_1_FNUC << 2) | (P2_0_FNUC << 0)))
    {
        PINSEL4 &= ~((0x03u << 14) | (0x03u << 4) | (0x03u << 2) | (0x03u << 0));                   //先清除相关bits
        PINSEL4 |= ((P2_7_FNUC << 14) | (P2_2_FNUC << 4) | (P2_1_FNUC << 2) | (P2_0_FNUC << 0));    //configure the RTS1 CTS1 TXD1 and RXD1 as the alternate function 1 configuration
    }
    PINMODE4 &= ~((0x03u << 14) | (0x03u << 4) | (0x03u << 2) | (0x03u << 0));                      //先清除相关bits
    PINMODE4 |= ((P2_7_MODE << 14) | (P2_2_MODE << 4) | (P2_1_MODE << 2) | (P2_0_MODE << 0));       //RTS1 CTS1 TXD1 and RXD1 has neither pull-up nor pull-down registers
    PINMODE_OD2 &= ~(P2_RTS_WLS | P2_CTS_WLS | P2_TXD_WLS | P2_RXD_WLS);                            //设置引脚非开漏模式

    UART_baud_rate_config(UART1, baud);             //初始化串口波特率
    U1LCR = mode & 0x3f;                            //初始化串口通讯参数

    U1FCR = 0x01 | 0x06 | (1 << 3) | (2 << 6);      //复位/使能FIFO，设置FIFO深度为8，使能DMA
    U1RBR;
    U1ACR = 0;                                      //初始化自动波特率控制寄存

    U1RS485CTRL = 0;                                //禁止RS485控制功能
    U1IER = 0x01;                                   //使能接收中断

//  WLS_RST_HIGH();                                 //RST_WLS输出‘1’,ZigBee复位禁能

#elif CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
    #ifdef ZIGBEE_TEST
        if((PINSEL4 & ((0x03u << 14) | (0x03u << 4) | (0x03u << 2) | (0x03u << 0)))
          != ((P2_7_FNUC << 14) | (P2_2_FNUC << 4) | (P2_1_FNUC << 2) | (P2_0_FNUC << 0)))
        {
            PINSEL4 &= ~((0x03u << 14) | (0x03u << 4) | (0x03u << 2) | (0x03u << 0));                   //先清除相关bits
            PINSEL4 |= ((P2_7_FNUC << 14) | (P2_2_FNUC << 4) | (P2_1_FNUC << 2) | (P2_0_FNUC << 0));    //configure the RTS1 CTS1 TXD1 and RXD1 as the alternate function 1 configuration
        }
        PINMODE4 &= ~((0x03u << 14) | (0x03u << 4) | (0x03u << 2) | (0x03u << 0));                      //先清除相关bits
        PINMODE4 |= ((P2_7_MODE << 14) | (P2_2_MODE << 4) | (P2_1_MODE << 2) | (P2_0_MODE << 0));       //RTS1 CTS1 TXD1 and RXD1 has neither pull-up nor pull-down registers
        PINMODE_OD2 &= ~(P2_RTS_WLS | P2_CTS_WLS | P2_TXD_LOCAL | P2_RXD_LOCAL);                            //设置引脚非开漏模式
    
        UART_baud_rate_config(UART1, baud);             //初始化串口波特率
        U1LCR = mode & 0x3f;                            //初始化串口通讯参数
    
        U1FCR = 0x01 | 0x06 | (1 << 3) | (2 << 6);      //复位/使能FIFO，设置FIFO深度为8，使能DMA
        U1RBR;
        U1ACR = 0;                                      //初始化自动波特率控制寄存
    //#ifdef PHASE_C_MODULE
        U1MCR = (1 << 6) | (1 << 7);                    //使能自动CTS和RTS
        U1TER = (1 << 7);                               //使能软流控的发送
    
    //#endif//PHASE_C_MODULE
        U1RS485CTRL = 0;                                //禁止RS485控制功能
        U1IER = 0x01;                                   //使能接收中断    
    #else
        if((PINSEL4 & ((0x03u << 2) | (0x03u << 0)))
          != ((P2_1_FNUC << 2) | (P2_0_FNUC << 0)))
        {
            PINSEL4 &= ~((0x03u << 2) | (0x03u << 0));                   //先清除相关bits
            PINSEL4 |= ((P2_1_FNUC << 2) | (P2_0_FNUC << 0));    //configure the TXD1 and RXD1 as the alternate function 1 configuration
        }
        PINMODE4 &= ~((0x03u << 2) | (0x03u << 0));                      //先清除相关bits
        PINMODE4 |= ((P2_1_MODE << 2) | (P2_0_MODE << 0));       //RTS1 CTS1 TXD1 and RXD1 has neither pull-up nor pull-down registers
        PINMODE_OD2 &= ~(P2_TXD_LOCAL | P2_RXD_LOCAL);                            //设置引脚非开漏模式
    
        UART_baud_rate_config(UART1, baud);             //初始化串口波特率
        U1LCR = mode & 0x3f;                            //初始化串口通讯参数
    
        U1FCR = 0x01 | 0x06 | (1 << 3) | (2 << 6);      //复位/使能FIFO，设置FIFO深度为8，使能DMA
        U1RBR;
        U1ACR = 0;                                      //初始化自动波特率控制寄存
    
        U1RS485CTRL = 0;                                //禁止RS485控制功能
        U1IER = 0x01;                                   //使能接收中断
    #endif//ZIGBEE_TEST    
#endif//INTERNAL_COM_ZIGBEE
}
/******************************************************************************
 Function name:  void UART2_init (U8 mode, U32 baud)
 Author       :  snap.gao
 Description  :  初始化串口2参数到一个默认值：数据位，校验位，停止位，波特率
 Input        :  mode  : 串口参数（数据位+校验位+停止位）
                 baud  : 波特率
 Return       :  None
 *****************************************************************************/
/*void UART2_init (U8 mode, U32 baud)
{
    PCONP |= (1 << PCUART2);                        // 使能UART2
    VICIntEnClr |= (1 << UART2_INT);                // 初始化UART2中断VIC接口
    U2IER = 0;                                      // 初始化中断

    PINSEL0  &= ~((0x03u << 20) | (0x03u << 22));   // 先清除相关bits
    PINSEL0  |= ((FUN1 << 20) | (FUN1 << 22));      // 初始化相关的I/O，设置P0.10和P0.11连接到UART2

    UART_mode_config(UART2, mode);                  // 初始化串口通讯参数
    UART_baud_rate_config(UART2, baud);             // 初始化串口波特率

    U2FCR = 0x04;                       // 复位FIFO
    U2FCR = 0x01 | (2 << 6);            // 使能FIFO，设置FIFO深度为8
    U2ACR = 0;                          // 初始化自动波特率控制寄存
    U2IER = 0x01;                       // 使能接收中断

    VICVectPriority28 = UART2_ISR_PRIO;
    VICVectAddr28 = (U32)UART2_IRQ_handler;
    VICIntEnable |= (1 << UART2_INT);   // 允许UART2中断
}
*/
/******************************************************************************
** 函数名称: void UART2_init(U8 mode, U32 baud)
** 函数功能: 初始化串口2参数到一个默认值：数据位，校验位，停止位，波特率
** 入口参数: mode: 串口参数（数据位+校验位+停止位）
**           baud: 波特率
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void UART2_init (U8 mode, U32 baud)
{
    PCONP |= (1 << PCUART2);                        //使能UART2
//    DISABLE_ISR(NVIC_UART2);                        //disbale UART2 interrupt
    U2IER = 0;                                      //初始化中断

    PCLKSEL1 &= ~(0x03u << 16);
    PCLKSEL1 |= (FPCLK_UART2_DIV << 16);            //设置外设时钟分频系数

    if((PINSEL0 & ((0x03u << 20) | (0x03u << 22))) != ((P0_10_FNUC << 20) | (P0_11_FNUC << 22)))
    {
        PINSEL0 &= ~((0x03u << 20) | (0x03u << 22));            //先清除相关bits
        PINSEL0 |= ((P0_10_FNUC << 20) | (P0_11_FNUC << 22));   //configure the TXD2 and RXD2 as the alternate function 1 configuration
    }
    PINMODE0 &= ~((0x03u << 20) | (0x03u << 22));               //先清除相关bits
    PINMODE0 |= ((P0_10_MODE << 20) | (P0_11_MODE << 22));      //TXD2 and RXD2 has neither pull-up nor pull-down registers
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
    PINMODE_OD0 &= ~(P0_TXD_4852 | P0_RXD_4852);                //设置引脚非开漏模式
#endif
    UART_baud_rate_config(UART2, baud);             //初始化串口波特率
    U2LCR = mode & 0x3f;;                           //初始化串口通讯参数

    U2FCR = 0x01 | 0x06 | (2 << 6);                 //复位/使能FIFO，设置FIFO深度为8
    U2RBR;
    U2ACR = 0;                                      //初始化自动波特率控制寄存
    U2IER = 0x01;                                   //使能接收中断

//    ENABLE_ISR(NVIC_UART2, PRIO_UART2);             //set UART2 interrupt parameter

}
/******************************************************************************
 Function name:  void UART3_init (U8 mode, U32 baud)
 Author       :  snap.gao
 Description  :  初始化串口3参数到一个默认值：数据位，校验位，停止位，波特率
 Input        :  mode  : 串口参数（数据位+校验位+停止位）
                 baud  : 波特率
 Return       :  None
 *****************************************************************************/
/*
void UART3_init (U8 mode, U32 baud)
{
    PCONP |= (1 << PCUART3);                        // 使能UART3
    VICIntEnClr |= (1 << UART3_INT);                // 初始化UART3中断VIC接口
    U3IER = 0;                                      // 初始化中断

    PINSEL0  &= ~((0x03u << 0) | (0x03u << 2));     // 先清除相关bits
    PINSEL0  |= ((FUN2 << 0) | (FUN2 << 2));        // 初始化相关的I/O，设置P0.0和P0.1连接到UART3

    UART_mode_config(UART3, mode);                  // 初始化串口通讯参数
    UART_baud_rate_config(UART3, baud);             // 初始化串口波特率

    U3FCR = 0x04;                       // 复位FIFO
    U3FCR = 0x01 | (2 << 6);            // 使能FIFO，设置FIFO深度为8
    U3ACR = 0;                          // 初始化自动波特率控制寄存
    U3IER = 0x01;                       // 使能接收中断

    VICVectPriority29 = UART3_ISR_PRIO;
    VICVectAddr29 = (U32)UART3_IRQ_handler;
    VICIntEnable |= (1 << UART3_INT);   // 允许UART3中断
}
*/
/******************************************************************************
** 函数名称: void UART3_init(U8 mode, U32 baud)
** 函数功能: 初始化串口3参数到一个默认值：数据位，校验位，停止位，波特率
** 入口参数: mode: 串口参数（数据位+校验位+停止位）
**           baud: 波特率
** 返 回 值: 无
** 说    明: 这里只初始化RXD引脚，TXD引脚在TIMER2初始化中被初始化为MAT2.0
******************************************************************************/
void UART3_init (U8 mode, U32 baud)
{
//azh 110516
//    Sync_Send_Status = 0;

    PCONP |= (1 << PCUART3);                        //使能UART3
//    DISABLE_ISR(NVIC_UART3);                        //disbale UART3 interrupt
    U3IER = 0;                                      //初始化中断

    PCLKSEL1 &= ~(0x03u << 18);
    PCLKSEL1 |= (FPCLK_UART3_DIV << 18);            //设置外设时钟分频系数

    if((PINSEL9 & ((0x03u << 26) | (0x03u << 24))) != ((P4_29_FNUC << 26) | (P4_28_FNUC << 24)))
    {
        PINSEL9 &= ~((0x03u << 26) | (0x03u << 24));//先清除相关bits
        PINSEL9 |= (P4_29_FNUC << 26);              //configure the RXD3/TXD3 as the alternate function 1 configuration
    }
    PINMODE9 &= ~((0x03u << 26) | (0x03u << 24));   //先清除相关bits
    PINMODE9 |= (P4_29_MODE << 26) | (P4_28_MODE << 26);    //RXD3/TXD3 has neither pull-up nor pull-down registers
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
    PINMODE_OD4 &= ~(P4_MC52I_RXD | P4_MC52I_TXD);  //设置引脚非开漏模式
#else
    PINMODE_OD4 &= ~(P4_GPRS_RXD1 | P4_GPRS_TXD1);  //设置引脚非开漏模式
#endif
    UART_baud_rate_config(UART3, baud);             //初始化串口波特率
    U3LCR = mode & 0x3f;                            //初始化串口通讯参数

    U3FCR = 0x01 | 0x06 | (1 << 3) | (2 << 6);      //复位/使能FIFO，设置FIFO深度为8，使能DMA
    U3RBR;
    U3ACR = 0;                                      //初始化自动波特率控制寄存
    U3IER = 0x01;                                   //使能接收中断

//    ENABLE_ISR(NVIC_UART3, PRIO_UART3);             //set UART3 interrupt parameter
}
/******************************************************************************
 Function name:  void UART_init (void)
 Author       :  snap.gao
 Description  :  初始化所有串口参数
 Input        :  None
 Return       :  None
 *****************************************************************************/
void UART_init (void)
{
#ifdef ZIGBEE_TEST
    init_zigbee_mem();
#endif// PHASE_C_MODULE
    UART0_init(UART0_COMM_PARA, UART0_BAUD_RATE);       // 初始化串口0通讯参数
    UART1_init(UART1_COMM_PARA, UART1_BAUD_RATE);       // 初始化串口1通讯参数
    UART2_init(UART2_COMM_PARA, UART2_BAUD_RATE);       // 初始化串口2通讯参数
    UART3_init(UART3_COMM_PARA, UART3_BAUD_RATE);       // 初始化串口3通讯参数
}

/******************************************************************************
 Function name:  void status_mechanism_init (U8 com)
 Author       :  snap.gao
 Description  :  初始化所有串口状态机参数
 Input        :  None
 Return       :  None
 *****************************************************************************/
void status_mechanism_init (U8 com)
{
//  U8 com;
#if CODE_VER_TYPE==WLS_CUR_PRJ			//配合无线采集器
	U8 m,buf[6];
#endif	

    Status_Mechanism[com].Send_State = COM_IDLE;
    Status_Mechanism[com].Rec_State = WAIT_REC;
    Status_Mechanism[com].Tmp_Tick_Count = OS_Time;//0;//azh 120816
    Status_Mechanism[com].Rec_Tick_Count = OS_Time;//0;
    Status_Mechanism[com].Send_Tick_Count = OS_Time;//0;
    Status_Mechanism[com].Send_End_Tick_Count = OS_Time;//0;
    Status_Mechanism[com].Dly_Tick_Count = OS_Time;//0;
    Status_Mechanism[com].Idle_Tick_Count = OS_Time;//0;
    Status_Mechanism[com].Rx_Buf = NULL;
    Status_Mechanism[com].Tx_Buf = NULL;
    Status_Mechanism[com].Last_Err_State = STATUS_OK;
    Status_Mechanism[com].Data_Send_Num = 0;
    Status_Mechanism[com].Send_Baud = DEFAULT_BAUD_RATE;
    Status_Mechanism[com].Send_Para = DEFAULT_COM_PARA;
    Status_Mechanism[com].Rec_Baud = DEFAULT_BAUD_RATE;
    Status_Mechanism[com].Rec_Para = DEFAULT_COM_PARA;
    Status_Mechanism[com].Rec_Mem_Type = MEM_1536_TYPE;

    switch(com)
    {
        case COM0:
        {
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块 //用于红外才1200 or 2400波特率
            Status_Mechanism[COM0].Dly_Tick_Space = OS_DLY_250MS;
            Status_Mechanism[COM0].Send_Tick_Space = OS_DLY_100MS;
            Status_Mechanism[COM0].Byte_Space = OS_DLY_250MS;
#else//9600
            Status_Mechanism[COM0].Dly_Tick_Space = OS_DLY_200MS;
            Status_Mechanism[COM0].Send_Tick_Space = OS_DLY_50MS;
            Status_Mechanism[COM0].Byte_Space = OS_DLY_100MS;
#endif
            Status_Mechanism[COM0].Default_Baud = (UART0_BAUD_RATE / 300);
            Status_Mechanism[COM0].Default_Para = UART0_COMM_PARA;
        }
        break;
        case COM1:
        {
#if CODE_VER_TYPE==H_METER_PRJ				//配合高压表 zigbee
            Status_Mechanism[COM1].Rec_State = DATA_REC; //zigbee 是一直在接收的
            Status_Mechanism[COM1].Index = 0xff;

            Status_Mechanism[COM1].Dly_Tick_Space = OS_DLY_10MS;//OS_DLY_200MS;
            Status_Mechanism[COM1].Send_Tick_Space = OS_DLY_20MS;
            Status_Mechanism[COM1].Byte_Space = OS_DLY_10MS;//OS_DLY_200MS;
            Status_Mechanism[COM1].Default_Baud = (UART1_BAUD_RATE / 300);
            Status_Mechanism[COM1].Default_Para = UART1_COMM_PARA;
#elif CODE_VER_TYPE==WLS_CUR_PRJ			//配合无线采集器 //小无线 2400
            Status_Mechanism[COM1].Dly_Tick_Space = OS_DLY_200MS;
            Status_Mechanism[COM1].Send_Tick_Space = OS_DLY_50MS;
            Status_Mechanism[COM1].Byte_Space = OS_DLY_100MS;
            Status_Mechanism[COM1].Default_Baud = (UART1_BAUD_RATE / 300);
            Status_Mechanism[COM1].Default_Para = UART1_COMM_PARA;
#elif CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块 //
	#ifdef ZIGBEE_TEST
            Status_Mechanism[COM1].Rec_State = DATA_REC; //zigbee 是一直在接收的
            Status_Mechanism[COM1].Index = 0xff;
            Status_Mechanism[COM1].Dly_Tick_Space = OS_DLY_10MS;//OS_DLY_200MS;
            Status_Mechanism[COM1].Send_Tick_Space = OS_DLY_20MS;
            Status_Mechanism[COM1].Byte_Space = OS_DLY_10MS;//OS_DLY_200MS;
	#else
            Status_Mechanism[COM1].Dly_Tick_Space = OS_DLY_200MS;
            Status_Mechanism[COM1].Send_Tick_Space = OS_DLY_50MS;
            Status_Mechanism[COM1].Byte_Space = OS_DLY_200MS;
	#endif//ZIGBEE_TEST
            Status_Mechanism[COM1].Default_Baud = (UART1_BAUD_RATE / 300);
            Status_Mechanism[COM1].Default_Para = UART1_COMM_PARA;
#endif
            break;
        }
        case COM2:
        {
            Status_Mechanism[COM2].Dly_Tick_Space = OS_DLY_200MS;
            Status_Mechanism[COM2].Send_Tick_Space = OS_DLY_50MS;
            Status_Mechanism[COM2].Byte_Space = OS_DLY_100MS;
            Status_Mechanism[COM2].Default_Baud = (UART2_BAUD_RATE / 300);
            Status_Mechanism[COM2].Default_Para = UART2_COMM_PARA;
#if CODE_VER_TYPE==WLS_CUR_PRJ			//配合无线采集器
            //azh 131022  
			rw_mp_para(2, 0x8905, buf, &m, 'r');//测量点2的波特率
			modify_uart_in_run(COM2, buf[0]);          
			rw_mp_para(2, 0x8915, buf, &m, 'r');//测量点2的接收超时，占用了用户申报容量的低字节10ms单位 BCD格式 高字节0x55才起作用
			modify_rec_dly_in_run(COM2, &buf[0]);
#endif
            break;
        }
        case COM3:
        {
            Status_Mechanism[COM3].Dly_Tick_Space = OS_DLY_50MS;
            Status_Mechanism[COM3].Send_Tick_Space = OS_DLY_50MS;
            Status_Mechanism[COM3].Byte_Space = OS_DLY_50MS;
            Status_Mechanism[COM3].Default_Baud = (UART3_BAUD_RATE / 300);
            Status_Mechanism[COM3].Default_Para = UART3_COMM_PARA;
            break;
        }
        default:
            break;
    }
}

/******************************************************************************
 Function name:  void config_send_band (U8 com, U32 band)
 Author       :  snap.gao
 Description  :  设置串口发送数据时的波特率
 Input        :  com : 0->MAX_PORT_NUM-1
                 band: 波特率
 Return       :  None
 *****************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不带CPU_GPRS模块
void config_send_band (U8 com, U32 band)
{
    Status_Mechanism[com].Send_Baud = (band / 300);
}
#endif
/******************************************************************************
 Function name:  void config_send_para (U8 com, U8 para)
 Author       :  snap.gao
 Description  :  设置串口发送数据时的校验位、停止位、数据长度
 Input        :  com : 0->MAX_PORT_NUM-1
                 para: 校验位、停止位、数据长度
 Return       :  None
 *****************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不带CPU_GPRS模块
void config_send_para (U8 com, U8 para)
{
    Status_Mechanism[com].Send_Para = para;
}
#endif
/******************************************************************************
 Function name:  void config_rec_band (U8 com, U32 band)
 Author       :  snap.gao
 Description  :  设置串口接收应答数据时的波特率
 Input        :  com : 0->MAX_PORT_NUM-1
                 band: 波特率
 Return       :  None
 *****************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不带CPU_GPRS模块
void config_rec_band (U8 com, U32 band)
{
    Status_Mechanism[com].Rec_Baud = (band / 300);
}
#endif
/******************************************************************************
 Function name:  void config_rec_para (U8 com, U8 para)
 Author       :  snap.gao
 Description  :  设置串口接收应答数据时的校验位、停止位、数据长度
 Input        :  com : 0->MAX_PORT_NUM-1
                 para: 校验位、停止位、数据长度
 Return       :  None
 *****************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不带CPU_GPRS模块
void config_rec_para (U8 com, U8 para)
{
    Status_Mechanism[com].Rec_Para = para;
}
#endif
/******************************************************************************
 Function name:  void config_rec_mem_type (U8 com, U8 type)
 Author       :  snap.gao
 Description  :  设置串口接收应答数据时的内存块类型
 Input        :  com : 0->MAX_PORT_NUM-1
                 type: 内存块类型
 Return       :  None
 *****************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不带CPU_GPRS模块
void config_rec_mem_type (U8 com, U8 type)
{
    Status_Mechanism[com].Rec_Mem_Type = type;
}
#endif
/******************************************************************************
 Function name:  S8 UART_open (U8 com, U16 time_out)
 Author       :  snap.gao
 Description  :  打开串口，独占资源保持与其它任务互斥，成功打开后用默认参数初始化串口
                 在串口不可用的情况下等待time_out的时间，超时后返回失败
                 如果串口打开成功，在结束串口处理后必须调用UART_close()函数释放串口
 Input        :  com     : 0->MAX_PORT_NUM-1
                 time_out: 在串口不可用的情况下允许等待的时间，可以使用的宏：（Tick＝2ms时，最大不超过131s）
                          OS_DLY_10MS
                          OS_DLY_50MS
                          OS_DLY_100MS
                          OS_DLY_200MS
                          OS_DLY_500MS
                          OS_DLY_1S
                          OS_DLY_2S
                          OS_DLY_5S
                          OS_DLY_10S
                          OS_DLY_60S
                          ...
                          BLOCK（阻塞方式，尽量不要使用）
                          QUERY（查询方式，没有任何等待）
 Return       :  串口打开失败时返回__FAIL
                 串口打开成功时返回__SUCCEED
 *****************************************************************************/
S8 UART_open (U8 com, U16 time_out)
{
    if(com >= MAX_PORT_NUM)             //串口号0->xx
    {
        return __FAIL;                  //端口号错误
    }
    if(os_mut_wait(Status_Mechanism[com].Mut_Com_Usable, time_out) == OS_R_TMO)
    {
        return __FAIL;                  //没有得到串口资源
    }
    switch(com)
    {
    case 0:
//        UART0_init(UART0_COMM_PARA, UART0_BAUD_RATE);       // 初始化串口0通讯参数
        break;
    case 1:
//        UART1_init(UART1_COMM_PARA, UART1_BAUD_RATE);       // 初始化串口1通讯参数
        break;
    case 2:
//        UART2_init(UART2_COMM_PARA, UART2_BAUD_RATE);       // 初始化串口2通讯参数
        break;
    case 3:
//        UART3_init(UART3_COMM_PARA, UART3_BAUD_RATE);       // 初始化串口3通讯参数
        break;
    }
    Status_Mechanism[com].Send_Baud = DEFAULT_BAUD_RATE;
    Status_Mechanism[com].Send_Para = DEFAULT_COM_PARA;
    Status_Mechanism[com].Rec_Baud = DEFAULT_BAUD_RATE;
    Status_Mechanism[com].Rec_Para = DEFAULT_COM_PARA;
    Status_Mechanism[com].Rec_Mem_Type = NORMAL_TYPE;
    return __SUCCEED;
}
/******************************************************************************
** 函数名称: S32 uart_send_idle(U8 com, U32 time_out)
** 函数功能: 在使用串口之前检查串口是否空闲，成功打开后用默认参数初始化串口
**           在串口不可用的情况下等待time_out的时间，超时后返回失败
** 入口参数: com     : 0->MAX_PORT_NUM-1
**           time_out: 在串口不可用的情况下允许等待的时间，可以使用的宏
**              QUERY（查询方式，没有任何等待）
** 返 回 值: 串口打开失败时返回__FAIL
**           串口打开成功时返回__SUCCEED
** 说    明: 主要用于升级写内部FLASH时查询
******************************************************************************/
S32 uart_send_idle (U8 com, U32 time_out)
{

    if(com >= MAX_PORT_NUM)             //串口号0->xx
    {
        return __FAIL;                  //端口号错误
    }

    if(Status_Mechanism[com].Send_State == COM_IDLE)
    {
        return __SUCCEED;
    }
    else
    {
        return __FAIL;          //没有得到串口资源
    }
}
/******************************************************************************
 Function name:  S8 UART_send (U8 com, U16 len, U8 *sendbuf)
 Author       :  snap.gao
 Description  :  串口数据发送的接口函数
 Input        :  com      : 0->MAX_PORT_NUM-1
                 len      : 发送数据长度
                 sendbuf  : 发送数据的缓冲区
                            头部16字节保存通讯帧信息，具体含义参考“通讯帧头部信息的地址偏移宏定义”
 Return       :  __FAIL   ：发送失败，需要应用程序处理发送缓冲区，重发或释放
                 __SUCCEED：发送成功
 *****************************************************************************/
S8 UART_send (U8 com, U16 len, U8 *sendbuf)
{
    if(check_mem_type(sendbuf) == INVALID_TYPE)
    {
        return __FAIL;              //内存块地址错误，返回后需要处理发送缓冲区
    }
    if(com >= MAX_PORT_NUM)
    {
        return __FAIL;              //端口号错误，返回后需要处理发送缓冲区
    }

    init_frame_info(sendbuf);
//如果是零距离带显示 要把GPRS信号发送带到下面主板上显示
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
    if(com == ZIGBEE_CHL)
    {
        sendbuf[DATA_OFFSET+len] = 0xa5;
        len++;        
        sendbuf[DATA_OFFSET+len] = Get_Signal_Strenth();
        len++;
    }
#endif
    
    set_frame_len(sendbuf, len);
    set_port_num(sendbuf, com);
    set_send_para(sendbuf, Status_Mechanism[com].Send_Para, Status_Mechanism[com].Send_Baud);
    if(os_mbx_send(Status_Mechanism[com].Mbx_Send_Buf, (void*)sendbuf, OS_DLY_10MS) == OS_R_TMO)
    {
        return __FAIL;              //没有把数据发出，返回后需要处理发送缓冲区
    }
    else
    {
        return __SUCCEED;           //成功发出
    }
}

/******************************************************************************
 Function name:  U8* UART_wait_answer (U8 com, U8 wait_time)
 Author       :  snap.gao
 Description  :  等待串口应答数据的接口函数
 Input        :  com      : 0->MAX_PORT_NUM-1
                 wait_time: 等待应答的时间，可以使用的宏：（Tick＝2ms时，最大不超过131s）
                            bit7=0 单位10ms，bit7=1 单位1s
                     WAIT_ANSWER_200MS
                     WAIT_ANSWER_500MS
                     WAIT_ANSWER_1S
                     WAIT_ANSWER_2S
                     WAIT_ANSWER_3S
                     WAIT_ANSWER_5S
                     WAIT_ANSWER_10S
                     WAIT_ANSWER_15S
                     WAIT_ANSWER_20S
                     WAIT_ANSWER_30S
                     WAIT_ANSWER_60S
                     WAIT_ANSWER_120S
 Return       :  合法内存块地址：收到的应答帧
                 NULL          ：没有收到应答帧
 *****************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不带CPU_GPRS模块
U8* UART_wait_answer (U8 com, U8 wait_time)
{
    void *p;
    U16  wait_tick;

    if(os_mbx_wait(Status_Mechanism[com].Mbx_Answer_Buf, &p, 0) != OS_R_TMO)    //检查邮箱里有没有遗忘的内存块（正在接收应答但应用程序已经超时的情况下被遗忘）
    {
        free_mem_block(p);              //一般不可能出现这种内存块
    }

    if((wait_time & 0x80) == 0)
    {
        wait_tick = wait_time * 10 / (OS_TICK / 1000);
        //wait_tick =   wait_time * 10 / (OS_TICK / 1);
    }
    else
    {
        wait_tick = (wait_time & 0x7f) * 1000 / (OS_TICK / 1000);
        //wait_tick =   (wait_time & 0x7f) * 1000 / (OS_TICK / 1);   //azh  110717
    }
    if(wait_tick > 0xfffe)
    {
        wait_tick = 0xfffe;             //系统支持的最大延时
    }
    if(os_mbx_wait(Status_Mechanism[com].Mbx_Answer_Buf, &p, wait_tick) == OS_R_TMO)
    {
        return NULL;                    //没有收到应答帧
    }
    else
    {
        return (U8*)p;                  //收到应答帧
    }
}
#endif
/******************************************************************************
 Function name:  S8 UART_resume (U8 com)
 Author       :  snap.gao
 Description  :  恢复串口的默认参数，只用于接收应答超时后的参数恢复
 Input        :  com: 0->MAX_PORT_NUM-1
 Return       :  串口参数恢复失败时返回__FAIL
                 串口参数恢复成功时返回__SUCCEED
 *****************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不带CPU_GPRS模块
S8 UART_resume (U8 com)
{
    if(com >= MAX_PORT_NUM)             //串口号0->MAX_PORT_NUM-1
    {
        return __FAIL;                  //端口号错误
    }
    record_status_mechanism(com, ANSWER_REC_TMO);               //记录状态机，应答帧接收超时
    record_status_mechanism(com, CONFIG_DEFAULT_STATUS);        //记录状态机
    baud_rate_config(com, Status_Mechanism[com].Default_Baud * 300);
    mode_config(com, Status_Mechanism[com].Default_Para);
    Status_Mechanism[com].Send_Baud = DEFAULT_BAUD_RATE;
    Status_Mechanism[com].Send_Para = DEFAULT_COM_PARA;
    Status_Mechanism[com].Rec_Baud = DEFAULT_BAUD_RATE;
    Status_Mechanism[com].Rec_Para = DEFAULT_COM_PARA;
    Status_Mechanism[com].Rec_Mem_Type = NORMAL_TYPE;
    return __SUCCEED;
}
#endif
/******************************************************************************
 Function name:  S8 UART_close (U8 com)
 Author       :  snap.gao
 Description  :  关闭串口，释放资源
 Input        :  com: 0->MAX_PORT_NUM-1
 Return       :  串口关闭失败时返回__FAIL
                 串口关闭成功时返回__SUCCEED
 *****************************************************************************/
S8 UART_close (U8 com)
{
    if(com >= MAX_PORT_NUM)             //串口号0->MAX_PORT_NUM-1
    {
        return __FAIL;                  //端口号错误
    }
    os_mut_release(Status_Mechanism[com].Mut_Com_Usable);   // 释放串口
    return __SUCCEED;
}

/******************************************************************************
 Function name:  U8 check_mbox (OS_ID mailbox)
 Author       :  snap.gao
 Description  :  检查是否有任务在等待邮箱
 Input        :  mailbox: 邮箱ID
 Return       :  __TURE : 有任务在等待邮箱
                 __FALSE: 没有任务在等待邮箱
 *****************************************************************************/
U8 check_mbox (OS_ID mailbox)
{
    P_MCB mbox;

    mbox = mailbox;
    if(mbox->p_lnk != NULL)
    {
        return __TRUE;
    }
    return __FALSE;
}

/******************************************************************************
 Function name:  void __irq UART0_IRQ_handler (void)
 Author       :  snap.gao
 Description  :  UART0接收/发送中断服务
                 接收队列数据格式说明：
                     bit31<-bit28   bit27<-bit24    bit23<-bit16    bit15<-bit8    bit7<-bit0
                      有效字节数       端口号           数据2          数据1          数据0
 Input        :  None
 Return       :  None
 *****************************************************************************/
//void __irq UART0_IRQ_handler (void)
//azh 110516
void UART0_IRQHandler (void)
{
    U32 iir,rbr;
    U16 data_len;
    U8  i;

    while(((iir = U0IIR) & 0x01) == 0)          // 若尚有中断待处理
    {
        switch(iir & 0x0e)      // 判断中断类型
        {
        case (2 << 1):          // 若是接收中断
            rbr = 0;
            for(i = 0; i < 3; i++)              // 读取3字节
            {
                rbr <<= 8;
                rbr |= U0RBR;
            }
            rbr |= (COM0 | (3 << 4)) << 24;     // 端口号＋有效字节数
            isr_mbx_send(Mbx_UART_Data, (void*)rbr);        // 放到接收队列
            rbr = 0;
            for(i = 0; i < 3; i++)              // 读取3字节
            {
                rbr <<= 8;
                rbr |= U0RBR;
            }
            rbr |= (COM0 | (3 << 4)) << 24;     // 端口号＋有效字节数
            isr_mbx_send(Mbx_UART_Data, (void*)rbr);        // 放到接收队列
            rbr = 0;
            for(i = 0; i < 2; i++)              // 读取2字节，一次性读取8字节
            {
                rbr <<= 8;
                rbr |= U0RBR;
            }
            rbr |= (COM0 | (2 << 4)) << 24;     // 端口号＋有效字节数
            isr_mbx_send(Mbx_UART_Data, (void*)rbr);        // 放到接收队列
            break;
        case (6 << 1):          // 若是超时中断
            rbr = 0;
            for(i = 0; i < 3; i++)
            {
                if((U0LSR & 0x01) == 0x01)
                {
                    rbr <<= 8;
                    rbr |= U0RBR;
                }
                else
                {
                    break;
                }
            }
            if(i != 0){
                rbr |= (COM0 | (i << 4)) << 24;             // 有效字节数＋端口号
                isr_mbx_send(Mbx_UART_Data, (void*)rbr);    // 放到接收队列
            }
            rbr = 0;
            for(i = 0; i < 3; i++)
            {
                if((U0LSR & 0x01) == 0x01)
                {
                    rbr <<= 8;
                    rbr |= U0RBR;
                }
                else
                {
                    break;
                }
            }
            if(i != 0){
                rbr |= (COM0 | (i << 4)) << 24;             // 有效字节数＋端口号
                isr_mbx_send(Mbx_UART_Data, (void*)rbr);    // 放到接收队列
            }
            rbr = 0;
            for(i = 0; i < 2; i++)
            {
                if((U0LSR & 0x01) == 0x01)
                {
                    rbr <<= 8;
                    rbr |= U0RBR;
                }
                else
                {
                    break;
                }
            }
            if(i != 0){
                rbr |= (COM0 | (i << 4)) << 24;             // 有效字节数＋端口号
                isr_mbx_send(Mbx_UART_Data, (void*)rbr);    // 放到接收队列
            }
            break;
        case (1 << 1):          // 若是发送中断
            data_len = Status_Mechanism[0].Tx_Buf[0] + Status_Mechanism[0].Tx_Buf[1] * 256;       // 要发送的字节总数
            if(data_len > (1024 - 16))
            {
                data_len = 1024 - 16;
            }
            for(i = 0; i < 16; i++)                                 // 每次最多填充16个字节
            {
                if(Status_Mechanism[0].Data_Send_Num < data_len)    // 判断是否发送结束
                {
                    U0THR = Status_Mechanism[0].Tx_Buf[Status_Mechanism[0].Data_Send_Num + DATA_OFFSET];
                    Status_Mechanism[0].Data_Send_Num++;            // 调整字符串指针指向下一个字符
                }
                else
                {
                    U0IER &= ~0x02;                                 // 关闭发送中断，此时FIFO中的数据仍在发送
                    UART_send_push(0);                              // 推动状态机，以进入完成发送处理
                    free_mem_block(Status_Mechanism[0].Tx_Buf);     // 释放内存块
                    Status_Mechanism[0].Tx_Buf = NULL;
                    break;
                }
            }
            break;
         default:
            U0IER &= ~0x03;         // 清除没有用到的中断使能
            U0IER |= 0x01;          // 使能接收中断
            break;
        }
    }
//azh 110516
//    VICVectAddr = 0x00;
}

/******************************************************************************
 Function name:  void __irq UART1_IRQ_handler (void)
 Author       :  snap.gao
 Description  :  UART1接收/发送中断服务
 Input        :  None
 Return       :  None
 *****************************************************************************/
//void __irq UART1_IRQ_handler (void)
void UART1_IRQHandler (void)
{
    U32 iir,rbr;
    U16 data_len;
    U8  i;

    while(((iir = U1IIR) & 0x01) == 0)          // 若尚有中断待处理
    {
        switch(iir & 0x0e)      // 判断中断类型
        {
        case ( 2<< 1):          // 若是接收中断
            rbr = 0;
            for(i = 0; i < 3; i++)              // 读取3字节
            {
                rbr <<= 8;
                rbr |= U1RBR;
            }
            rbr |= (COM1 | (3 << 4)) << 24;     // 端口号＋有效字节数
            isr_mbx_send(Mbx_UART_Data, (void*)rbr);        // 放到接收队列
            rbr = 0;
            for(i = 0; i < 3; i++)              // 读取3字节
            {
                rbr <<= 8;
                rbr |= U1RBR;
            }
            rbr |= (COM1 | (3 << 4)) << 24;     // 端口号＋有效字节数
            isr_mbx_send(Mbx_UART_Data, (void*)rbr);        // 放到接收队列
            rbr = 0;
            for(i = 0; i < 2; i++)              // 读取2字节，一次性读取8字节
            {
                rbr <<= 8;
                rbr |= U1RBR;
            }
            rbr |= (COM1 | (2 << 4)) << 24;     // 端口号＋有效字节数
            isr_mbx_send(Mbx_UART_Data, (void*)rbr);        // 放到接收队列

            break;
        case (6 << 1):          // 若是超时中断
            rbr = 0;
            for(i = 0; i < 3; i++)
            {
                if((U1LSR & 0x01) == 0x01)
                {
                    rbr <<= 8;
                    rbr |= U1RBR;
                }
                else
                {
                    break;
                }
            }
            if(i != 0)
            {
                rbr |= (COM1 | (i << 4)) << 24;             // 有效字节数＋端口号
                isr_mbx_send(Mbx_UART_Data, (void*)rbr);    // 放到接收队列
            }
            rbr = 0;
            for(i = 0; i < 3; i++)
            {
                if((U1LSR & 0x01) == 0x01)
                {
                    rbr <<= 8;
                    rbr |= U1RBR;
                }
                else
                {
                    break;
                }
            }
            if(i != 0){
                rbr |= (COM1 | (i << 4)) << 24;             // 有效字节数＋端口号
                isr_mbx_send(Mbx_UART_Data, (void*)rbr);    // 放到接收队列
            }
            rbr = 0;
            for(i = 0; i < 2; i++)
            {
                if((U1LSR & 0x01) == 0x01)
                {
                    rbr <<= 8;
                    rbr |= U1RBR;
                }
                else
                {
                    break;
                }
            }
            if(i != 0){
                rbr |= (COM1 | (i << 4)) << 24;             // 有效字节数＋端口号
                isr_mbx_send(Mbx_UART_Data, (void*)rbr);    // 放到接收队列
            }
            break;
        case (1 << 1):          // 若是发送中断
#ifdef ZIGBEE_TEST
            if(Status_Mechanism[1].Index < ZIGBEE_SEND_MEM_NUM)
            {
                data_len = Zigbee_Send_Status[Status_Mechanism[1].Index].Len;// 要发送的字节总数
                if(data_len > (1024 - 16))
                {
                    data_len = 1024 - 16;
                }
                for(i = 0; i < 16; i++)                                 // 每次最多填充16个字节
                {
                    if(Status_Mechanism[1].Data_Send_Num < data_len)    // 判断是否发送结束
                    {
                        U1THR = Zigbee_Send_Status[Status_Mechanism[1].Index].Buf[Status_Mechanism[1].Data_Send_Num];
                        Status_Mechanism[1].Data_Send_Num++;            // 调整字符串指针指向下一个字符
                    }
                    else
                    {
                        U1IER &= ~0x02;                                 // 关闭发送中断，此时FIFO中的数据仍在发送
                        UART_send_push(1);                              // 推动状态机，以进入完成发送处理
                        if((Zigbee_Send_Status[Status_Mechanism[1].Index].Flag & WAIT_RESEND) == 0)
                        {
                            free_zigbee_send_mem(Status_Mechanism[1].Index);        //是发给模块的命令不需要等待重发，释放内存块
                        }
                        Status_Mechanism[1].Index = 0xff;
                        Status_Mechanism[1].Tx_Buf = NULL;
                        break;
                    }
                }
            }
#else
            data_len = Status_Mechanism[1].Tx_Buf[0] + Status_Mechanism[1].Tx_Buf[1] * 256;       // 要发送的字节总数
            if(data_len > (1024 - 16))
            {
                data_len = 1024 - 16;
            }
            for(i = 0; i < 16; i++)                                 // 每次最多填充16个字节
            {
                if(Status_Mechanism[1].Data_Send_Num < data_len)    // 判断是否发送结束
                {
                    U1THR = Status_Mechanism[1].Tx_Buf[Status_Mechanism[1].Data_Send_Num + DATA_OFFSET];
                    Status_Mechanism[1].Data_Send_Num++;            // 调整字符串指针指向下一个字符
                }
                else
                {
                    U1IER &= ~0x02;                                 // 关闭发送中断，此时FIFO中的数据仍在发送
                    UART_send_push(1);                              // 推动状态机，以进入完成发送处理
                    free_mem_block(Status_Mechanism[1].Tx_Buf);     // 释放内存块
                    Status_Mechanism[1].Tx_Buf = NULL;
                    break;
                }
            }
#endif//ZIGBEE_TEST
            break;
         default:
            U1IER &= ~0x03;         // 清除没有用到的中断使能
            U1IER |= 0x01;          // 使能接收中断
            break;
        }
    }
//azh 110516
//    VICVectAddr = 0x00;
}

/******************************************************************************
 Function name:  void __irq UART2_IRQ_handler (void)
 Author       :  snap.gao
 Description  :  UART2接收/发送中断服务
 Input        :  None
 Return       :  None
 *****************************************************************************/
//void __irq UART2_IRQ_handler (void)
void UART2_IRQHandler (void)
{
    U32 iir,rbr;
    U16 data_len;
    U8  i;

    while(((iir = U2IIR) & 0x01) == 0)          // 若尚有中断待处理
    {
        switch(iir & 0x0e)      // 判断中断类型
        {
        case (2 << 1):          // 若是接收中断
            rbr = 0;
            for(i = 0; i < 3; i++)              // 读取3字节
            {
                rbr <<= 8;
                rbr |= U2RBR;
            }
            rbr |= (COM2 | (3 << 4)) << 24;     // 端口号＋有效字节数
            isr_mbx_send(Mbx_UART_Data, (void*)rbr);        // 放到接收队列
            rbr = 0;
            for(i = 0; i < 3; i++)              // 读取3字节
            {
                rbr <<= 8;
                rbr |= U2RBR;
            }
            rbr |= (COM2 | (3 << 4)) << 24;     // 端口号＋有效字节数
            isr_mbx_send(Mbx_UART_Data, (void*)rbr);        // 放到接收队列
            rbr = 0;
            for(i = 0; i < 2; i++)              // 读取2字节，一次性读取8字节
            {
                rbr <<= 8;
                rbr |= U2RBR;
            }
            rbr |= (COM2 | (2 << 4)) << 24;     // 端口号＋有效字节数
            isr_mbx_send(Mbx_UART_Data, (void*)rbr);        // 放到接收队列
            break;
        case (6 << 1):          // 若是超时中断
            rbr = 0;
            for(i = 0; i < 3; i++)
            {
                if((U2LSR & 0x01) == 0x01)
                {
                    rbr <<= 8;
                    rbr |= U2RBR;
                }
                else
                {
                    break;
                }
            }
            if(i != 0){
                rbr |= (COM2 | (i << 4)) << 24;             // 有效字节数＋端口号
                isr_mbx_send(Mbx_UART_Data, (void*)rbr);    // 放到接收队列
            }
            rbr = 0;
            for(i = 0; i < 3; i++)
            {
                if((U2LSR & 0x01) == 0x01)
                {
                    rbr <<= 8;
                    rbr |= U2RBR;
                }
                else
                {
                    break;
                }
            }
            if(i != 0){
                rbr |= (COM2 | (i << 4)) << 24;             // 有效字节数＋端口号
                isr_mbx_send(Mbx_UART_Data, (void*)rbr);    // 放到接收队列
            }
            rbr = 0;
            for(i = 0; i < 2; i++)
            {
                if((U2LSR & 0x01) == 0x01)
                {
                    rbr <<= 8;
                    rbr |= U2RBR;
                }
                else
                {
                    break;
                }
            }
            if(i != 0){
                rbr |= (COM2 | (i << 4)) << 24;             // 有效字节数＋端口号
                isr_mbx_send(Mbx_UART_Data, (void*)rbr);    // 放到接收队列
            }
            break;
        case (1 << 1):          // 若是发送中断
            data_len = Status_Mechanism[2].Tx_Buf[0] + Status_Mechanism[2].Tx_Buf[1] * 256;       // 要发送的字节总数
            if(data_len > (1024 - 16))
            {
                data_len = 1024 - 16;
            }
            for(i = 0; i < 16; i++)                                 // 每次最多填充16个字节
            {
                if(Status_Mechanism[2].Data_Send_Num < data_len)    // 判断是否发送结束
                {
                    U2THR = Status_Mechanism[2].Tx_Buf[Status_Mechanism[2].Data_Send_Num + DATA_OFFSET];
                    Status_Mechanism[2].Data_Send_Num++;            // 调整字符串指针指向下一个字符
                }
                else
                {
                    U2IER &= ~0x02;                                 // 关闭发送中断，此时FIFO中的数据仍在发送
                    UART_send_push(2);                              // 推动状态机，以进入完成发送处理
                    free_mem_block(Status_Mechanism[2].Tx_Buf);     // 释放内存块
                    Status_Mechanism[2].Tx_Buf = NULL;
                    break;
                }
            }
            break;
         default:
            U2IER &= ~0x03;         // 清除没有用到的中断使能
            U2IER |= 0x01;          // 使能接收中断
            break;
        }
    }
//azh 110516
//    VICVectAddr = 0x00;
}

/******************************************************************************
 Function name:  void __irq UART3_IRQ_handler (void)
 Author       :  snap.gao
 Description  :  UART3接收/发送中断服务
 Input        :  None
 Return       :  None
 *****************************************************************************/
//void __irq UART3_IRQ_handler (void)
void UART3_IRQHandler (void)
{
    U32 iir;//,rbr;
    U16 data_len;
    U8  i=0;
    U8 GucRcvBuf[8];

    while(((iir = U3IIR) & 0x01) == 0)          // 若尚有中断待处理
    {
        switch(iir & 0x0e)      // 判断中断类型
        {
        case (2 << 1):          // 若是接收中断
            for (i = 0; i < 8; i++)
            {
                GucRcvBuf[i] = U3RBR;
            }
            SCIBufferWrite(&stAT_RX_BUFFER ,GucRcvBuf, 8);
            stReceiveBuffer.usSilenceCnt = CON_AT_SILENCE_MAX;
            stReceiveBuffer.ucIsGetVaild = 1;
            break;
        case (6 << 1):          // 若是超时中断
            i = 0;
            while ((U3LSR & 0x01) == 0x01)
            {
                GucRcvBuf[i] = U3RBR;
                i++;
            }
            SCIBufferWrite(&stAT_RX_BUFFER ,GucRcvBuf, i);
            stReceiveBuffer.usSilenceCnt = CON_AT_SILENCE_MAX;
            stReceiveBuffer.ucIsGetVaild = 1;
            break;
/*
            rbr = 0;
            for(i = 0; i < 3; i++)              // 读取3字节
            {
                rbr <<= 8;
                rbr |= U3RBR;
            }
            rbr |= (COM3 | (3 << 4)) << 24;     // 端口号＋有效字节数
            isr_mbx_send(Mbx_UART_Data, (void*)rbr);        // 放到接收队列
            rbr = 0;
            for(i = 0; i < 3; i++)              // 读取3字节
            {
                rbr <<= 8;
                rbr |= U3RBR;
            }
            rbr |= (COM3 | (3 << 4)) << 24;     // 端口号＋有效字节数
            isr_mbx_send(Mbx_UART_Data, (void*)rbr);        // 放到接收队列
            rbr = 0;
            for(i = 0; i < 2; i++)              // 读取2字节，一次性读取8字节
            {
                rbr <<= 8;
                rbr |= U3RBR;
            }
            rbr |= (COM3 | (2 << 4)) << 24;     // 端口号＋有效字节数
            isr_mbx_send(Mbx_UART_Data, (void*)rbr);        // 放到接收队列
            break;
        case (6 << 1):          // 若是超时中断
            rbr = 0;
            for(i = 0; i < 3; i++)
            {
                if((U3LSR & 0x01) == 0x01)
                {
                    rbr <<= 8;
                    rbr |= U3RBR;
                }
                else
                {
                    break;
                }
            }
            if(i != 0){
                rbr |= (COM3 | (i << 4)) << 24;             // 有效字节数＋端口号
                isr_mbx_send(Mbx_UART_Data, (void*)rbr);    // 放到接收队列
            }
            rbr = 0;
            for(i = 0; i < 3; i++)
            {
                if((U3LSR & 0x01) == 0x01)
                {
                    rbr <<= 8;
                    rbr |= U3RBR;
                }
                else
                {
                    break;
                }
            }
            if(i != 0){
                rbr |= (COM3 | (i << 4)) << 24;             // 有效字节数＋端口号
                isr_mbx_send(Mbx_UART_Data, (void*)rbr);    // 放到接收队列
            }
            rbr = 0;
            for(i = 0; i < 2; i++)
            {
                if((U3LSR & 0x01) == 0x01)
                {
                    rbr <<= 8;
                    rbr |= U3RBR;
                }
                else
                {
                    break;
                }
            }
            if(i != 0){
                rbr |= (COM3 | (i << 4)) << 24;             // 有效字节数＋端口号
                isr_mbx_send(Mbx_UART_Data, (void*)rbr);    // 放到接收队列
            }
            break;
*/
        case (1 << 1):          // 若是发送中断
            data_len = Status_Mechanism[3].Tx_Buf[0] + Status_Mechanism[3].Tx_Buf[1] * 256;       // 要发送的字节总数
            if(data_len > (1024 - 16))
            {
                data_len = 1024 - 16;
            }
            for(i = 0; i < 16; i++)                                 // 每次最多填充16个字节
            {
                if(Status_Mechanism[3].Data_Send_Num < data_len)    // 判断是否发送结束
                {
                    U3THR = Status_Mechanism[3].Tx_Buf[Status_Mechanism[3].Data_Send_Num + DATA_OFFSET];
                    Status_Mechanism[3].Data_Send_Num++;            // 调整字符串指针指向下一个字符
                }
                else
                {
                    U3IER &= ~0x02;                                 // 关闭发送中断，此时FIFO中的数据仍在发送
                    UART_send_push(3);                              // 推动状态机，以进入完成发送处理
                    free_mem_block(Status_Mechanism[3].Tx_Buf);     // 释放内存块
                    Status_Mechanism[3].Tx_Buf = NULL;
                    break;
                }
            }
            break;
         default:
            U3IER &= ~0x03;         // 清除没有用到的中断使能
            U3IER |= 0x01;          // 使能接收中断
            break;
        }
    }
//azh 110516
//    VICVectAddr = 0x00;
}

#ifdef ZIGBEE_TEST
/******************************************************************************
** 函数名称: U8 query_zigbee_send_event(void)
** 函数功能: 查询正在等待发送的内存块，总是优先发送最早申请的内存块
** 入口参数: 无
** 返 回 值: 找到等待发送事件时返回内存块数组的下标
**           找不到时返回0xff
** 说    明: 函数将清除等待发送标志
******************************************************************************/
U8 query_zigbee_send_event (void)
{
    U8 i,j;
    U32 tick_tmp,sub;

    tick_tmp = 0;
    j = 0xff;
    for(i = 0; i < ZIGBEE_SEND_MEM_NUM; i++)
    {
        if((Zigbee_Send_Status[i].Flag & WAIT_SEND) == WAIT_SEND)
        {
            sub = OS_Time - Zigbee_Send_Status[i].Tick;
            if(sub > tick_tmp)
            {
                tick_tmp = sub;
                j = i;
            }
        }
    }

    if(j < ZIGBEE_SEND_MEM_NUM)
    {
        Zigbee_Send_Status[j].Flag &= ~WAIT_SEND;
        return j;
    }
    else
    {
        return 0xff;
    }
}
/******************************************************************************
** 函数名称: void init_zigbee_mem(void)
** 函数功能: 初始化通讯内存块
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void init_zigbee_mem (void)
{
    U16 i;

    for(i = 0; i < ZIGBEE_REC_MEM_NUM; i++)
    {
        //Zigbee_Rec_Status[i].Mem_Index = 0xff;
        Zigbee_Rec_Status[i].Mem_Ptr = NULL;

        Zigbee_Duplicate_Frame[i].Ptr = 0;
        memset(Zigbee_Duplicate_Frame[i].Src_Addr, 0, 8);
        memset(Zigbee_Duplicate_Frame[i].Frame_Serial, 0xff, 8);
    }
    Start_Ptr = 1;
    End_Ptr = 0;
    for(i = 0; i < ZIGBEE_SEND_MEM_NUM; i++)
    {
        Zigbee_Send_Status[i].Flag = 0;
    }

    for(i=0; i<ZIGBEE_REC_FIFO_LEN; i++)
    {
        Zigbee_Rec_Fifo[i] = 0;
    }
}
/******************************************************************************
** 函数名称: void deal_with_zigbee_send(U8 *mem_ptr)
** 函数功能: 计算ZigBee接收FIFO中有效数据的长度
** 入口参数: start_ptr: 当前FIFO接收指针
**           end_ptr  : 当前FIFO处理指针
** 返 回 值: 有效数据的长度
** 说    明: FIFO中没有有效数据时start_ptr总比end_ptr大‘1’
**           start_ptr大于end_ptr且超过‘1’时为FIFO中有数据
**           start_ptr等于end_ptr时为FIFO满了
**           start_ptr小于end_ptr时为start_ptr已经翻转，但end_ptr还没有翻转，FIFO中有数据
******************************************************************************/
S32 deal_with_zigbee_send (U8 *mem_ptr)
{
    U8 i, total_package, send_mem;
    U16 package_len, len;

    //debug
    //memset(&mem_ptr[ZIGBEE_RET_ADDR_OFFSET], 0xff, 8);

    //if(check_mac_addr(Comm_Mem[index].Aim_Addr) == 1)       //检查目标地址是否是广播地址
    if(check_broadcast_addr(&mem_ptr[ZIGBEE_RET_ADDR_OFFSET]) == __TRUE)//检查目标地址是否是广播地址
    {
        package_len = BROADCAST_ZIGBEE_FRAME_LEN;
    }
    else
    {
        package_len = UNICAST_ZIGBEE_FRAME_LEN;
    }

    //len = Comm_Mem[index].Len;
    len = mem_ptr[DATA_LEN_OFFSET] + (mem_ptr[DATA_LEN_OFFSET+1]<<8);
    total_package = len / package_len;          //分包后的总包数
    if(len > package_len * total_package)       //非整除时，总包数需要加1
    {
        total_package++;
    }

    for(i = 0; i < total_package; i++)
    {
        send_mem = alloc_zigbee_send_mem();
        if(send_mem == 0xff)
        {
            return __FAIL;
        }

        Zigbee_Send_Status[send_mem].Buf[0] = '*';
        //memcpy(&Zigbee_Send_Status[send_mem].Buf[1], &Comm_Mem[index].Aim_Addr, 8);
        memcpy(&Zigbee_Send_Status[send_mem].Buf[1], &mem_ptr[ZIGBEE_RET_ADDR_OFFSET], 8);

        //memcpy(&Zigbee_Send_Status[send_mem].Buf[9], gSource_Addr, 8);
        //debug!!!!!!!!!!!!!!!!!
        memset(&Zigbee_Send_Status[send_mem].Buf[9], 0x12, 8);

        Zigbee_Send_Status[send_mem].Buf[17] = ZIGBEE_CHANNEL;      //无线通道号
        Zigbee_Send_Status[send_mem].Buf[18] = Frame_Serial_Num << 2;   //帧序号
        Zigbee_Send_Status[send_mem].Buf[19] = total_package;       //总包数
        Zigbee_Send_Status[send_mem].Buf[20] = i + 1;               //分包号
        if(i != total_package - 1)          //检查是否最后一包
        {
            Zigbee_Send_Status[send_mem].Len = package_len + 24;    //分包的总长度
            len -= package_len;             //减去已发送的数据长度
            Zigbee_Send_Status[send_mem].Buf[21] = package_len;     //数据域长度
            //memcpy(&Zigbee_Send_Status[send_mem].Buf[22], &Comm_Mem[index].Buf[package_len * i], package_len);
            memcpy(&Zigbee_Send_Status[send_mem].Buf[22], &mem_ptr[DATA_OFFSET + package_len * i], package_len);
            Zigbee_Send_Status[send_mem].Buf[package_len + 22] = cal_frmcs(Zigbee_Send_Status[send_mem].Buf, package_len + 22);
            Zigbee_Send_Status[send_mem].Buf[package_len + 23] = '#';
        }
        else
        {
            Zigbee_Send_Status[send_mem].Len = len + 24;            //分包的总长度
            Zigbee_Send_Status[send_mem].Buf[21] = len;             //数据域长度
            //memcpy(&Zigbee_Send_Status[send_mem].Buf[22], &Comm_Mem[index].Buf[package_len * i], len);
            memcpy(&Zigbee_Send_Status[send_mem].Buf[22], &mem_ptr[DATA_OFFSET + package_len * i], len);
            Zigbee_Send_Status[send_mem].Buf[len + 22] = cal_frmcs(Zigbee_Send_Status[send_mem].Buf, len + 22);
            Zigbee_Send_Status[send_mem].Buf[len + 23] = '#';
        }

        if(package_len == UNICAST_ZIGBEE_FRAME_LEN)
        {
            Zigbee_Send_Status[send_mem].Flag |= WAIT_RESEND;       //单播帧需要重发
        }
        Zigbee_Send_Status[send_mem].Flag |= WAIT_SEND;             //交给通讯状态机发送

        Frame_Serial_Num++;
    }

//    Comm_Mem[index].Flag = 0;               //释放内存块
    free_mem_block(mem_ptr);
    return __SUCCEED;//azh 返回成功 由解析任务去释放
}

/******************************************************************************
** 函数名称: void deal_with_zigbee_rec(U8 **ret_ptr)
** 函数功能: 计算ZigBee接收FIFO中有效数据的长度
** 入口参数: start_ptr: 当前FIFO接收指针
**           end_ptr  : 当前FIFO处理指针
** 返 回 值: 有效数据的长度
** 说    明: FIFO中没有有效数据时start_ptr总比end_ptr大‘1’
**           start_ptr大于end_ptr且超过‘1’时为FIFO中有数据
**           start_ptr等于end_ptr时为FIFO满了
**           start_ptr小于end_ptr时为start_ptr已经翻转，但end_ptr还没有翻转，FIFO中有数据
******************************************************************************/
void deal_with_zigbee_rec (U8 **ret_ptr)
{
    U8 mem_type;
    U8 cs, *buf;
    U16 data_len, len;

    buf = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR5);
    if(buf == NULL)
    {
//          memcpy(gusCurMemErrFunName, "deal_wit", 8);
        setmemgetnullid(GET_MEM_ERR5, gusCurMemErrFunName);
        return;
    }

    while((data_len = get_fifo_vaild_data_len(Start_Ptr, End_Ptr)) >= 24)       //检查FIFO中的有效数据长度是否大于一个最小ZigBee帧长
    {
        if(Zigbee_Rec_Fifo[increase_end_ptr(End_Ptr, 1)] != '*')    //查找帧起始符“*”
        {
            End_Ptr = increase_end_ptr(End_Ptr, 1);                 //处理指针加‘1’
            continue;
        }
        //buf = Comm_Mem[mem_tmp].Buf;
        copy_zigbee_rec_fifo_data(buf, increase_end_ptr(End_Ptr, 1), 22);       //把FIFO中的数据拷贝到缓存区

        len = buf[21];                                              //取得帧长度
        if(len > MAX_ZIGBEE_FRAME_LEN)                              //检查帧长度
        {
            End_Ptr = increase_end_ptr(End_Ptr, 1);                 //处理指针加‘1’
            continue;
        }

        if(data_len < len + 24)                                     //检查FIFO的有效长度是否足够
        {
            break;                                                  //继续接收直到长度够
        }
        copy_zigbee_rec_fifo_data(buf + 22, increase_end_ptr(End_Ptr, 23), len + (24 - 22));    //把FIFO中的数据拷贝到缓存区

        if(buf[len + 23] != '#')                                    //查找帧结束符“#”
        {
            End_Ptr = increase_end_ptr(End_Ptr, 1);                 //处理指针加‘1’
            continue;
        }

        cs = buf[len + 22];                                         //取得帧检验码
        if(cs != cal_frmcs(buf, len + 22))
        {
            End_Ptr = increase_end_ptr(End_Ptr, 1);                 //处理指针加‘1’
            continue;
        }

        //if(check_mac_addr(&buf[9]) == 0)                            //检查源地址是否是模块发出的
        if(check_local_addr(&buf[9]) == __TRUE)                     //检查源地址是否是模块发出的
        {
            zigbee_local_frame_analyse(buf);                        //ZigBee底层维护命令解析
            End_Ptr = increase_end_ptr(End_Ptr, len + 24);          //处理指针加上一个帧长
            continue;
        }

//        if(check_dest_addr(&buf[1], gSource_Addr) == 0)              //检查目标地址是否是发给自己的
//        {
//            End_Ptr = increase_end_ptr(End_Ptr, len + 24);          //处理指针加上一个帧长
//            continue;
//        }

        if(buf[19] > MAX_PACKAGE_NUM)                               //检查总包数
        {
            End_Ptr = increase_end_ptr(End_Ptr, len + 24);          //处理指针加上一个帧长
            continue;
        }

        if(buf[20] > buf[19])                                       //检查分包数
        {
            End_Ptr = increase_end_ptr(End_Ptr, len + 24);          //处理指针加上一个帧长
            continue;
        }
//azh 电子式高压表由于要支持被动抄上行规约，所以要区分是抄回的数据，被动收到数据地模块MAC地址，所以这部分代码保存到定时抄表回数据节点去保存
//        if(gucGetZigbeeAddrFlag == 1)//标志是要通过zigbee广播地址 攻取zigbee地址并保存
//        {
//            store_zigbee_addr(&buf[9]);
//            gucGetZigbeeAddrFlag = 0;
//        }

        if(zigbee_package_assemble(buf, (void*)ret_ptr) < 0)                        //开始组包
        {
            break;      //内存块不足继续等待
        }
        End_Ptr = increase_end_ptr(End_Ptr, len + 24);              //处理指针加上一个帧长
    }

    free_mem_block(buf);//Mem_256_Byte
}

/******************************************************************************
** 函数名称: S8 zigbee_local_frame_analyse(U8 *buf)
** 函数功能: 解析ZigBee模块与主机间的本地维护通讯命令
** 入口参数: buf: ZigBee模块回应帧数据起始地址
** 返 回 值: 解析成功0，帧格式错误-1，不能识别的帧-2，执行错误的帧-3
** 说    明: 无
******************************************************************************/
S8 zigbee_local_frame_analyse (U8 *buf)
{
    if(buf[21] == 0)
    {
        if('A' == buf[19])                              //buf[20]为信号强度
        {
            set_normal_run_flag(1 << 0);                    //升级有效标识检查之一zigbee通信正常确认
            if(__FAIL == release_zigbee_send_mem(buf[18]))
            {
                return -3;                              //执行错误
            }
        }
        else if('E' == buf[19])                         //buf[20]为最近3秒空中报文数量/4
        {
            if(__FAIL == resend_zigbee_package(buf[18]))
            {
                return -3;                              //执行错误
            }
        }
        else
        {
            return -2;                                  //不能识别的帧
        }
    }
    else
    {
        if(buf[19] != 1)                                //检查总包数
        {
            return -1;                                  //帧格式错误
        }

        if(buf[20] != 1)                                //检查分包数
        {
            return -1;                                  //帧格式错误
        }

        switch(buf[21])
        {
        case 12:
            if((buf[22] == 'M') && (buf[23] == 'A') && (buf[24] == 'C') && (buf[25] == '='))
            {
                memcpy(gSource_Addr, &buf[26], 8);       //保存ZigBee模块的MAC地址
            }
            else
            {
                return -2;                              //不能识别的帧
            }
            break;
        default:
            if((buf[24] == 'R') && (buf[25] == 'E') && (buf[26] == 'S') && (buf[27] == 'E') && (buf[28] == 'T'))
            {
                memcpy(Zigbee_Reset_Reason, &buf[22], buf[21]);     //保存ZigBee模块的复位原因
                Zigbee_Reset_Reason[buf[21]] = '\0';                //以‘0’结束字符串
//                if(__FAIL == read_zigbee_mac_addr())    //启动读取ZigBee模块MAC地址
//                {
//                    return -3;      //执行错误
//                }
            }
            else
            {
                return -2;                              //不能识别的帧
            }
            break;
        }
    }
    return 0;                                           //正确解析并执行
}

/******************************************************************************
** 函数名称: S8 zigbee_package_assemble(U8 *buf, U8 **ret_ptr)
** 函数功能: 拼装收到的ZigBee分包数据，如果分包全部收到了则交给默认解析函数
** 入口参数: buf: ZigBee分包数据起始地址
** 返 回 值: 组包成功0，帧格式错误1，重复的帧2，内存块申请不到-1
** 说    明: 无 全局变量 gSource_Addr
******************************************************************************/
S8 zigbee_package_assemble (U8 *buf, U8 **ret_ptr)
{
    U8 mem_type;
    U8  serial, i, j, *mem_ptr, package_len;
    U16 *len_ptr;

    if(__FALSE == check_broadcast_addr(buf + 1))        //不是广播帧
    {
        if(__FALSE != check_duplicate_frame(buf + 9, buf[18] >> 2))
        {
            return 2;                                   //重复帧
        }
        if(buf[19] & 0x80)                              //带签名加密帧, 不需要
        {
            return 1;
        }
        package_len = UNICAST_ZIGBEE_FRAME_LEN;         //单播帧长度
    }
    else                                                //广播帧
    {
        j = record_frame_serial(buf + 9, 0xff);         //记录源地址, 帧序号忽略
        memset(Zigbee_Duplicate_Frame[j].Frame_Serial, 0xff, 8);//令历史帧序号无效
        package_len = BROADCAST_ZIGBEE_FRAME_LEN;
    }

    serial = ((buf[18] >> 2) - (buf[20] & 0x7f)) & 0x3f;//计算帧识别号, 帧序号减分包号, 去掉路由帧标识
    for(i = 0; i < ZIGBEE_REC_MEM_NUM; i++)
    {
        if(Zigbee_Rec_Status[i].Mem_Ptr != NULL)        //检查是否在使用中
        {
            if(comp_zigbee_addr(buf + 9, &Zigbee_Rec_Status[i].Mem_Ptr[ZIGBEE_RET_ADDR_OFFSET]) != __FALSE)
            {
                break;
            }
        }
    }

    if(i >= ZIGBEE_REC_MEM_NUM)                         //如果是第一次接收
    {
        //mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR6);            //申请一块内存块用于保存数据
//azh 1209011
        mem_ptr = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR6);            //申请一块内存块用于保存数据
        if(mem_ptr == NULL)
        {
            setmemgetnullid(GET_MEM_ERR6, gusCurMemErrFunName);
            return -1;                                  //内存块申请不到
        }
        i = alloc_zigbee_rec_struct();                  //申请一个新的接收状态结构体

        format_zigbee_rec_buf (ZIGBEE_CHL, mem_ptr);    //初始化帧头信息
        Zigbee_Rec_Status[i].Mem_Ptr = mem_ptr;         //保存内存块编号
        Zigbee_Rec_Status[i].Total_Package = buf[19] & 0x7f;        //保存总包数, 为以后兼容
        Zigbee_Rec_Status[i].Frame_Serial = serial;                 //保存帧识别号
        memcpy(&mem_ptr[ZIGBEE_RET_ADDR_OFFSET], &buf[9], 8);       //保存接收到的帧的源地址，也就是应答时的目标地址
                                                        //azh 保存自己模块的地址 不是自己模块地址应该不转发到终端 不然有问题
        if(__FALSE == check_broadcast_addr(buf + 1))    //排除广播帧
        {
            memcpy(gSource_Addr, &buf[1], 8);           //保存ZigBee模块的MAC地址
        }
    }
    else
    {
        if((buf[19] & 0x7f) != Zigbee_Rec_Status[i].Total_Package)
        {
            return 1;                                   //帧格式错误
        }
        mem_ptr = Zigbee_Rec_Status[i].Mem_Ptr;
    }

    if((buf[20] & 0x7f) != (buf[19] & 0x7f))            //检查是不是最后的分包 都应该是满负荷填充数据
    {
        if(package_len != buf[21])
        {
            return 1;                                   //帧格式错误
        }
    }

    memcpy(&mem_ptr[DATA_OFFSET + package_len * ((buf[20] & 0x7f) - 1)], &buf[22], buf[21]);    //保存分包数据
    len_ptr = (U16*)&mem_ptr[DATA_LEN_OFFSET];          //内存安排上确保双字节对齐
    *len_ptr += buf[21];

    Zigbee_Rec_Status[i].Package_Flag |= 1 << ((buf[20] & 0x7f) - 1);
    record_frame_serial(buf + 9, buf[18] >> 2);         //记录帧序号, 帧最近接到分包时刻...

    for(j = 0; j < Zigbee_Rec_Status[i].Total_Package; j++)
    {
        if((Zigbee_Rec_Status[i].Package_Flag & (1 << j)) == 0)
        {
            break;
        }
    }

    if(j >= Zigbee_Rec_Status[i].Total_Package)         //检查所有分包是否都收到
    {
        *ret_ptr = mem_ptr;
        Zigbee_Rec_Status[i].Mem_Ptr = NULL;            //释放接收状态结构体
    }

    return 0;
}

/******************************************************************************
** 函数名称: void copy_zigbee_rec_fifo_data (U8 *buf, U16 offset, U8 len)
** 函数功能: 从ZigBee接收FIFO中拷贝数据，考虑FIFO的翻转
** 入口参数: buf   : 拷贝数据的缓存
**           offset：ZigBee接收FIFO的偏移
**           len   ：拷贝数据的长度
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void copy_zigbee_rec_fifo_data (U8 *buf, U16 offset, U8 len)
{
    U16 tmp;

    if(offset + len >= ZIGBEE_REC_FIFO_LEN)
    {
        tmp = ZIGBEE_REC_FIFO_LEN - offset;                 //计算拷贝到FIFO末尾的数据个数
        memcpy(buf, &Zigbee_Rec_Fifo[offset], tmp);         //先拷贝到FIFO末尾
        memcpy(buf + tmp, Zigbee_Rec_Fifo, len - tmp);      //从FIFO头部开始拷贝剩余的数据
    }
    else
    {
        memcpy(buf, &Zigbee_Rec_Fifo[offset], len);
    }
}

/******************************************************************************
** 函数名称: U8 check_dest_addr (U8 *buf1, U8 *buf2)
** 函数功能: 比较2个ZigBee地址，并检查第一个地址buf1是否广播地址
** 入口参数: buf1: 地址1缓存
**           buf2: 地址2缓存
** 返 回 值: 地址匹配返回1，广播地址返回2，否则返回0
** 说    明: 无
******************************************************************************/
U8 check_dest_addr (U8 *buf1, U8 *buf2)
{
    U8 i;

    for(i = 0; i < 8; i++)
    {
        if(buf1[i] != buf2[i])
        {
            break;
        }
    }
    if(i >= 8)
    {
        return 1;                 //地址匹配
    }

    for(i = 0; i < 8; i++)
    {
        if(buf1[i] != 0xff)
        {
            break;
        }
    }
    if(i >= 8)
    {
        return 2;                 //地址全‘0xff’
    }

    return 0;
}

/******************************************************************************
** 函数名称: U16 get_fifo_vaild_data_len(U16 start_ptr, U16 end_ptr)
** 函数功能: 计算ZigBee接收FIFO中有效数据的长度
** 入口参数: start_ptr: 当前FIFO接收指针
**           end_ptr  : 当前FIFO处理指针
** 返 回 值: 有效数据的长度
** 说    明: FIFO中没有有效数据时start_ptr总比end_ptr大‘1’
**           start_ptr大于end_ptr且超过‘1’时为FIFO中有数据
**           start_ptr等于end_ptr时为FIFO满了
**           start_ptr小于end_ptr时为start_ptr已经翻转，但end_ptr还没有翻转，FIFO中有数据
******************************************************************************/
U16 get_fifo_vaild_data_len (U16 start_ptr, U16 end_ptr)
{
    if(start_ptr <= end_ptr)
    {
        return start_ptr + ZIGBEE_REC_FIFO_LEN - end_ptr - 1;
    }
    else
    {
        return start_ptr - end_ptr - 1;
    }
}

/******************************************************************************
** 函数名称: U16 increase_end_ptr(U16 end_ptr, U16 num)
** 函数功能: ZigBee当前FIFO处理指针增加偏移，考虑指针翻转
** 入口参数: num    : 要增加的个数
**           end_ptr: 当前FIFO处理指针
** 返 回 值: 增加偏移后的当前FIFO处理指针，可能翻转
** 说    明: 无
******************************************************************************/
U16 increase_end_ptr (U16 end_ptr, U16 num)
{
    if((end_ptr + num) >= ZIGBEE_REC_FIFO_LEN)
    {
        return end_ptr + num - ZIGBEE_REC_FIFO_LEN;
    }
    else
    {
        return end_ptr + num;
    }
}

/******************************************************************************
** 函数名称: U16 increase_start_ptr(U16 start_ptr, U16 end_ptr)
** 函数功能: ZigBee接收FIFO指针加‘1’，如果接收指针已经碰到处理指针则不加‘1’返回原值
** 入口参数: start_ptr: 当前FIFO接收指针
**           end_ptr  : 当前FIFO处理指针
** 返 回 值: 接收指针加‘1’，有可能不加或翻转
** 说    明: 无
******************************************************************************/
U16 increase_start_ptr (U16 start_ptr, U16 end_ptr)
{
    if(start_ptr == end_ptr)
    {
        return start_ptr;
    }

    if(start_ptr == ZIGBEE_REC_FIFO_LEN - 1)
    {
        return 0;
    }
    return ++start_ptr;
}

/******************************************************************************
** 函数名称: U8 alloc_zigbee_rec_struct (void)
** 函数功能: 申请ZigBee接收状态结构体
**           优先申请空闲结构体，如果没有空闲结构体则申请最早使用的结构体
**           申请后初始化结构体的相关参数
** 入口参数: 无
** 返 回 值: 返回内存块数组的下标，必然会申请一个
** 说    明: 无
******************************************************************************/
U8 alloc_zigbee_rec_struct (void)
{
    U8 i,j;
    U32 tick_tmp,sub;

    tick_tmp = 0;
    j = 0xff;
    for(i = 0; i < ZIGBEE_REC_MEM_NUM; i++)
    {
        //if(Zigbee_Rec_Status[i].Mem_Index == 0xff)      //检查是否空闲
        if(Zigbee_Rec_Status[i].Mem_Ptr == NULL)      //检查是否空闲
        {
            Zigbee_Rec_Status[i].Tick = OS_Time;//Sys_Tick;       //保存申请时刻的系统Tick
            Zigbee_Rec_Status[i].Package_Flag = 0;      //分包接收标志位初始化
            return i;
        }
        else
        {
            sub = OS_Time - Zigbee_Rec_Status[i].Tick;
            if(sub >= tick_tmp)
            {
                tick_tmp = sub;
                j = i;
            }
        }
    }
    free_mem_block(Zigbee_Rec_Status[j].Mem_Ptr);          //强制释放
    Zigbee_Rec_Status[j].Mem_Ptr = NULL;

    //Zigbee_Rec_Status[j].Tick = Sys_Tick;               //保存申请时刻的系统Tick
    Zigbee_Rec_Status[j].Tick = OS_Time;               //保存申请时刻的系统Tick
    Zigbee_Rec_Status[j].Package_Flag = 0;              //分包接收标志位初始化
    return j;
}

/******************************************************************************
** 函数名称: U8 alloc_zigbee_send_mem(void)
** 函数功能: 申请ZigBee发送内存块，
**           优先申请空闲内存块，如果没有空闲内存块则申请最早的并且正在等待重发的内存块
**           如果申请成功则初始化内存块的相关参数
** 入口参数: 无
** 返 回 值: 正确申请时返回内存块数组的下标，申请不到内存块返回0xff
** 说    明: 无
******************************************************************************/
U8 alloc_zigbee_send_mem (void)
{
    U8 i,j;
    U32 tick_tmp,sub;

    tick_tmp = 0;
    j = 0xff;
    for(i = 0; i < ZIGBEE_SEND_MEM_NUM; i++)
    {
        if(Zigbee_Send_Status[i].Flag == 0)         //检查是否空闲
        {
            Zigbee_Send_Status[i].Len = 0;          //长度初始化
            Zigbee_Send_Status[i].Tick = OS_Time;//Sys_Tick;  //保存申请时刻的系统Tick
            Zigbee_Send_Status[i].Flag = IN_USE;    //标记内存块被占用
            return i;
        }
        else if((Zigbee_Send_Status[i].Flag & (IN_USE | WAIT_RESEND)) == (IN_USE | WAIT_RESEND))    //检查是否在等待重发
        {
            //sub = Sys_Tick - Zigbee_Send_Status[i].Tick;
            sub = OS_Time - Zigbee_Send_Status[i].Tick;
            if(sub > tick_tmp)
            {
                tick_tmp = sub;
                j = i;
            }
        }
    }

    if(j != 0xff)
    {
        Zigbee_Send_Status[j].Len = 0;              //长度初始化
        Zigbee_Send_Status[j].Tick = OS_Time;//Sys_Tick;      //保存申请时刻的系统Tick
        Zigbee_Send_Status[j].Flag = IN_USE;        //标记内存块被占用
    }
    return j;
}

/******************************************************************************
** 函数名称: S32 release_zigbee_send_mem(U8 frame_serial)
** 函数功能: 按照帧序号释放正在等待重发的ZigBee内存块
** 入口参数: frame_serial：帧序号
** 返 回 值: 找到符合帧序号的内存块并释放返回__SUCCEED，找不到内存块返回__FAIL
** 说    明: 无
******************************************************************************/
S32 release_zigbee_send_mem (U8 frame_serial)
{
    U8 i;

    for(i = 0; i < ZIGBEE_SEND_MEM_NUM; i++)
    {
        if((Zigbee_Send_Status[i].Flag & (IN_USE | WAIT_RESEND)) == (IN_USE | WAIT_RESEND))     //检查是否在等待重发
        {
            if(frame_serial == Zigbee_Send_Status[i].Buf[18])       //比对帧序号
            {
                Zigbee_Send_Status[i].Flag = 0;
                return __SUCCEED;
            }
        }
    }
    return __FAIL;
}

/******************************************************************************
** 函数名称: S32 resend_zigbee_package(U8 frame_serial)
** 函数功能: 按照帧序号再次发送正在等待重发的ZigBee内存块
** 入口参数: frame_serial：帧序号
** 返 回 值: 找到符合帧序号的内存块并重发返回__SUCCEED，找不到内存块返回__FAIL
** 说    明: 无
******************************************************************************/
S32 resend_zigbee_package (U8 frame_serial)
{
    U8 i;

    for(i = 0; i < ZIGBEE_SEND_MEM_NUM; i++)
    {
        if((Zigbee_Send_Status[i].Flag & (IN_USE | WAIT_RESEND)) == (IN_USE | WAIT_RESEND))     //检查是否在等待重发
        {
            if(frame_serial == Zigbee_Send_Status[i].Buf[18])       //比对帧序号
            {
                Zigbee_Send_Status[i].Flag |= WAIT_SEND;            //再次发送
                return __SUCCEED;
            }
        }
    }
    return __FAIL;
}

/******************************************************************************
** 函数名称: S32 read_zigbee_mac_addr(void)
** 函数功能: 发送读本地ZigBee物理地址帧
** 入口参数: 无
** 返 回 值: 发送成功就会返回__SUCCEED，否则返回__FAIL
** 说    明: 无
******************************************************************************/
S32 read_zigbee_mac_addr (void)
{
    U8 send_mem;

    send_mem = alloc_zigbee_send_mem();
    if(send_mem == 0xff)
    {
        return __FAIL;
    }

    Zigbee_Send_Status[send_mem].Buf[0] = '*';
    memset(&Zigbee_Send_Status[send_mem].Buf[1], 0, 8);         //目标地址全‘0’
    memcpy(&Zigbee_Send_Status[send_mem].Buf[9], gSource_Addr, 8);
    Zigbee_Send_Status[send_mem].Buf[17] = ZIGBEE_CHANNEL;      //无线通道号
    Zigbee_Send_Status[send_mem].Buf[18] = Frame_Serial_Num << 2;   //帧序号
    Zigbee_Send_Status[send_mem].Buf[19] = 1;                   //总包数
    Zigbee_Send_Status[send_mem].Buf[20] = 1;                   //分包号
    Zigbee_Send_Status[send_mem].Len = 5 + 24;                  //分包的总长度
    Zigbee_Send_Status[send_mem].Buf[21] = 5;                   //数据域长度

    Zigbee_Send_Status[send_mem].Buf[22] = 'M';                 //填充命令数据
    Zigbee_Send_Status[send_mem].Buf[23] = 'A';                 //填充命令数据
    Zigbee_Send_Status[send_mem].Buf[24] = 'C';                 //填充命令数据
    Zigbee_Send_Status[send_mem].Buf[25] = '=';                 //填充命令数据
    Zigbee_Send_Status[send_mem].Buf[26] = '?';                 //填充命令数据

    Zigbee_Send_Status[send_mem].Buf[27] = cal_frmcs(Zigbee_Send_Status[send_mem].Buf, 27);
    Zigbee_Send_Status[send_mem].Buf[28] = '#';

    Zigbee_Send_Status[send_mem].Flag |= WAIT_SEND;             //交给通讯状态机发送，不需要重发
    Frame_Serial_Num++;
    return __SUCCEED;
}
/******************************************************************************
** 函数名称: void check_zigbee_rec_mem(void)
** 函数功能: ZigBee接收内存块超时检查，超时后强制释放内存块
**           检查接收中断，如果接收FIFO中有空闲则打开中断使能
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void check_zigbee_rec_mem (void)
{
    U8 i;

    for(i = 0; i < ZIGBEE_REC_MEM_NUM; i++)
    {
        if(Zigbee_Rec_Status[i].Mem_Ptr != NULL)                      //检查是否在使用中
        {
            //if((Sys_Tick - Zigbee_Rec_Status[i].Tick) > DLY_1S * 10)    //检查是否超时，10秒
            if((OS_Time - Zigbee_Rec_Status[i].Tick) > OS_DLY_1S * 10)    //检查是否超时，10秒
            {
                //free_comm_mem(Zigbee_Rec_Status[i].Mem_Index);          //强制释放
                //Zigbee_Rec_Status[i].Mem_Index = 0xff;
                free_mem_block(Zigbee_Rec_Status[i].Mem_Ptr);          //强制释放
                Zigbee_Rec_Status[i].Mem_Ptr = NULL;
            }
        }
        if(check_local_addr(Zigbee_Duplicate_Frame[i].Src_Addr) == __FALSE)     //检查源地址是否有效
        {
            //if((Sys_Tick - Zigbee_Duplicate_Frame[i].Tick) > DLY_1S * 10)       //检查是否超时，10秒
            if((OS_Time - Zigbee_Duplicate_Frame[i].Tick) > OS_DLY_1S * 10)       //检查是否超时，10秒
            {
                Zigbee_Duplicate_Frame[i].Ptr = 0;
                memset(Zigbee_Duplicate_Frame[i].Src_Addr, 0, 8);
                memset(Zigbee_Duplicate_Frame[i].Frame_Serial, 0xff, 8);
            }
        }
    }

    if(Start_Ptr != End_Ptr)            //接收FIFO中是否有空闲
    {
        if((U1IER & 0x01) == 0)
        {
            U1IER |= 0x01;              //有内存块空闲，使能临时关闭的接收中断
        }
    }
}

/******************************************************************************
** 函数名称: void check_zigbee_send_mem(void)
** 函数功能: ZigBee发送内存块等待重发和占用超时检查，超时后强制释放内存块
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void check_zigbee_send_mem (void)
{
    U8 i;

    for(i = 0; i < ZIGBEE_SEND_MEM_NUM; i++)
    {
        if((Zigbee_Send_Status[i].Flag & (IN_USE | WAIT_RESEND)) == (IN_USE | WAIT_RESEND))     //检查是否在等待重发
        {

            //if((Sys_Tick - Zigbee_Send_Status[i].Tick) > DLY_500MS)         //检查是否需要重发，500ms
            if((OS_Time - Zigbee_Send_Status[i].Tick) > OS_DLY_500MS)         //检查是否需要重发，500ms
            {
                if((Zigbee_Send_Status[i].Buf[18] & 0x03) != 0x03)          //检查重发次数，一共3次，利用帧序号的低两位
                {
                    Zigbee_Send_Status[i].Buf[18]++;                        //重发次数加‘1’
                    Zigbee_Send_Status[i].Buf[Zigbee_Send_Status[i].Buf[21] + 22]++;            //处理校验和

                    Zigbee_Send_Status[i].Tick = OS_Time;//Sys_Tick;
                    Zigbee_Send_Status[i].Flag |= WAIT_SEND;                //再次发送
                }
                //if((Sys_Tick - Zigbee_Send_Status[i].Tick) > DLY_1S * 10)       //检查是否重发超时，10秒
                if((OS_Time - Zigbee_Send_Status[i].Tick) > OS_DLY_1S * 10)
                {
                    Zigbee_Send_Status[i].Flag = 0;                             //强制释放
                }
            }
            continue;
        }
        if((Zigbee_Send_Status[i].Flag & IN_USE) == IN_USE)                 //检查是否在使用中
        {
            //if((Sys_Tick - Zigbee_Send_Status[i].Tick) > DLY_1S * 60 * 3)   //检查是否超时，3分钟
            if((OS_Time - Zigbee_Send_Status[i].Tick) > OS_DLY_1S * 60 * 3)
            {
                Zigbee_Send_Status[i].Flag = 0;                             //强制释放
            }
        }
    }
}
/******************************************************************************
 Function name:  S8 store_zigbee_comm_data  (U8 com, U32 data)
 Author       :  snap.gao
 Description  :  保存串口接收到的数据
 Input        :  com :串口号
                 data:串口数据
 Return       :  内存块地址不合法时返回__FAIL
                 正确保存和内存块溢出错误时都返回__SUCCEED
 *****************************************************************************/
S8 store_zigbee_comm_data (U8 com,  U32 data)
{
    U8  len,i;

    if(com != 1)
    {
        return __FAIL;
    }
    len = (data >> 28) & 0x03;          //数据长度，最大3个
    if(len == 0)                        //数据长度不能是‘0’
    {
        return __FAIL;
    }

    for(i = 0; i < len; i++)
    {
        if(Start_Ptr == End_Ptr)        //检查FIFO是否满了
        {
            U1IER &= ~0x01;             //临时禁止接收中断，让硬件产生流控
            return __FAIL;
        }
        Zigbee_Rec_Fifo[Start_Ptr] = data   >> ((len - 1 - i) * 8);     //保存一个数据 AZH低字节在高位，看中断里的接收顺序
        Start_Ptr = increase_start_ptr(Start_Ptr, End_Ptr);
    }

    return __SUCCEED;
}
/******************************************************************************
 Function name:  S8 format_zigbee_rec_buf (U8 com, U8 *info)
 Author       :  azh
 Description  :  格式化Zigbee接收缓冲区 和串口的一样前面有一些帧头信息
 Input        :  com :串口号
                 *info
 Return       :  内存块地址不合法时返回__FAIL
                 正确保存和内存块溢出错误时都返回__SUCCEED
 *****************************************************************************/
S8 format_zigbee_rec_buf (U8 com, U8 *info)
{
    if(com != 1)
    {
        return __FAIL;
    }
    init_frame_info(info);          //初始化帧头信息
    set_frame_len(info, 0);         //长度清零
    set_port_num(info, com);        //标识端口号

    return __SUCCEED;
}
/******************************************************************************
** 函数名称: U32 check_duplicate_frame(U8 *addr, U8 serial)
** 函数功能: 通过检查接收帧的源地址和帧识别号，判别当前接收到的帧是否与前一个重复
** 入口参数: addr  : 源地址的缓存
**           serial：帧识别号
** 返 回 值: __FALSE帧识别号没有重复，不是重复帧，__TRUE帧识别号重复，是重复帧
** 说    明: 无
******************************************************************************/
U32 check_duplicate_frame (U8 *addr, U8 serial)
{
    U8 i,j;

    for(j = 0; j < ZIGBEE_REC_MEM_NUM; j++)
    {
        if(comp_zigbee_addr(addr, Zigbee_Duplicate_Frame[j].Src_Addr) == __TRUE)    //检查源地址是否匹配
        {
            for(i = 0; i < 8; i++)      //源地址匹配时检查帧识别号，最近的8个
            {
                if(Zigbee_Duplicate_Frame[j].Frame_Serial[i] == serial)
                {
                    break;
                }
            }

            if(i < 8)
            {
                return __TRUE;          //与最近的8个中的一个相符，认为是重复帧
            }
            else
            {
                return __FALSE;
            }
        }
    }
    return __FALSE;
}

/******************************************************************************
** 函数名称: U8 record_frame_serial(U8 *addr, U8 serial)
** 函数功能: 记录接收帧的源地址和帧序号
** 入口参数: addr  : 源地址的缓存
**           serial：帧序号
** 返 回 值: 保存地址记录的索引号
** 说    明: 为每个会话的主机保留最近8次通讯的帧序号
******************************************************************************/
U8 record_frame_serial (U8 *addr, U8 serial)
{
    U8 j, tmp, ptr;
    U32 tick, sub;

    tick = 0;
    tmp = 0;                                            //避免编译器警告
    for(j = 0; j < ZIGBEE_REC_MEM_NUM; j++)
    {
        sub = OS_Time - Zigbee_Duplicate_Frame[j].Tick;
        if(sub >= tick)
        {
            tick = sub;
            tmp = j;                                    //记录Tick值最早的位置
        }
        if(comp_zigbee_addr(addr, Zigbee_Duplicate_Frame[j].Src_Addr) == __TRUE)    //检查源地址是否匹配
        {
            tmp = j;                                    //源地址匹配时就在这个位置记录新的帧识别号
            break;
        }
    }
    if(ZIGBEE_REC_MEM_NUM == j)                         //没找到匹配的, 优先使用无效主机地址的位置
    {
        for(j = 0; j < ZIGBEE_REC_MEM_NUM; j++)
        {
            if(__TRUE == check_local_addr(Zigbee_Duplicate_Frame[j].Src_Addr))
            {
                tmp = j;
                break;
            }
        }
    }

    ptr = Zigbee_Duplicate_Frame[tmp].Ptr;
    Zigbee_Duplicate_Frame[tmp].Frame_Serial[ptr] = serial;             //找不到匹配的源地址时找个最早的位置记录帧识别号
    memcpy(Zigbee_Duplicate_Frame[tmp].Src_Addr, addr, 8);              //记录源地址
    if(++Zigbee_Duplicate_Frame[tmp].Ptr >= 8)                          //帧识别号指向下一个位置，目前一共8个
    {
        Zigbee_Duplicate_Frame[tmp].Ptr = 0;
    }
    Zigbee_Duplicate_Frame[tmp].Tick = OS_Time;                        //刷新为当前系统Tick
    return tmp;
}
/******************************************************************************
** 函数名称: U32 check_local_addr (U8 *buf)
** 函数功能: 检查ZigBee地址是否本地通讯地址，即全‘0’
** 入口参数: buf: 地址缓存
** 返 回 值: 全‘0’本地地址返回__TRUE，不是返回__FALSE
** 说    明: 无
******************************************************************************/
U32 check_local_addr (U8 *buf)
{
    U8 i;

    for(i = 0; i < 8; i++)
    {
        if(buf[i] != 0x00)
        {
            break;
        }
    }
    if(i >= 8)
    {
        return __TRUE;              //地址全‘0x00’
    }

    return __FALSE;
}

/******************************************************************************
** 函数名称: U32 check_broadcast_addr (U8 *buf)
** 函数功能: 检查ZigBee地址是否广播通讯地址，即全‘0xff’
** 入口参数: buf: 地址缓存
** 返 回 值: 全‘0xff’广播地址返回__TRUE，不是返回__FALSE
** 说    明: 无
******************************************************************************/
U32 check_broadcast_addr (U8 *buf)
{
    U8 i;

    for(i = 0; i < 8; i++)
    {
        if(buf[i] != 0xff)
        {
            break;
        }
    }
    if(i >= 8)
    {
        return __TRUE;              //地址全‘0xff’
    }

    return __FALSE;
}

/******************************************************************************
** 函数名称: U32 comp_zigbee_addr (U8 *buf1, U8 *buf2)
** 函数功能: 比较2个ZigBee地址
** 入口参数: buf1: 地址1缓存
**           buf2: 地址2缓存
** 返 回 值: 地址匹配返回__TRUE，否则返回__FALSE
** 说    明: 无
******************************************************************************/
U32 comp_zigbee_addr (U8 *buf1, U8 *buf2)
{
    U8 i;

    for(i = 0; i < 8; i++)
    {
        if(buf1[i] != buf2[i])
        {
            break;
        }
    }
    if(i >= 8)
    {
        return __TRUE;          //地址匹配
    }

    return __FALSE;
}
#endif// ZIGBEE_TEST

/******************************************************************************
** 函数名称: unsigned char ConvertToInerPort (unsigned char ucInPort)
** 函数功能: 对外端口转换成内部对应的端口(比如中继端口 下行测量点所在端口)
** 入口参数: ucPort: 外部端口
**          
** 返 回 值: 内部对应的端口 如果为FF则为非法
** 说    明: 无
******************************************************************************/
unsigned char ConvertToInerPort (unsigned char ucPort)
{
    unsigned char ucInnerPort=0xff;
    switch(ucPort)
    {
        case 0:
            ucInnerPort = RS4852_CHL;
            break;
        case 1:
            ucInnerPort = ZIGBEE_CHL;
            break;
        default:
            break;                    
    }
    return ucInnerPort;
}

/******************************************************************************
                            End Of File
 *****************************************************************************/

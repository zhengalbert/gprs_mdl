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
   2��
   1��     1.00       2009-06-30     snap.gao       create
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

#ifdef ZIGBEE_TEST //CODE_VER_TYPE==H_METER_PRJ		//����ѹ��
    WLS_RST_HIGH(); //ֻ�����г�ʼ�����������ߣ���Ϊһ�ϵ�ģ��ͻ������ݷ�������
#endif

    for(;;)
    {
        check_power_down();         //�����磬�������sign_task_schedule()������ȴ�֮ǰ
//azh 110519 ��ʱע�͵�
        sign_task_schedule(UART_TASK_PRIO);

#ifdef ZIGBEE_TEST
        check_zigbee_rec_mem();                             //���ZigBee�����ڴ���Ƿ�ʱ
        check_zigbee_send_mem();                            //���ZigBee�����ڴ���Ƿ�ʱ
#endif// ZIGBEE_TEST

        uart_status = os_mbx_wait(Mbx_UART_Data, (void**)&uart_data, 1);    //��ѯ���ն����Ƿ�������
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
            Meter_LED_Cnt[com] = LED_FLASH_TIMES;                    //ָʾ����
            Status_Mechanism[com].Tmp_Tick_Count = OS_Time;     //����һ����ǰϵͳTick
            Status_Mechanism[com].Rec_Tick_Count = Status_Mechanism[com].Tmp_Tick_Count;
            switch(Status_Mechanism[com].Rec_State)             //����״̬������
            {
            case WAIT_REC:          //����ʱ�յ�����
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
                        os_mbx_send(Mbx_UART_Data, (void*)uart_data, 0);        //�黹������һ��Tick���ԣ���Ϊ�ж��ڽ��գ����Կ�������˳���
                        Status_Mechanism[com].Last_Err_State = MEM_EMPTY;       //�ڴ�����벻��
//                      memcpy(gusCurMemErrFunName, "UART_tas", 8);
                        setmemgetnullid(GET_MEM_ERR4, gusCurMemErrFunName);
                        goto receive_end;
                    }
                    else
                    {
                        Status_Mechanism[com].Rx_Buf = mem_ptr;
                        init_frame_info(Status_Mechanism[com].Rx_Buf);          //��ʼ��֡ͷ��Ϣ
                        set_frame_len(Status_Mechanism[com].Rx_Buf, 0);         //��������
                        set_port_num(Status_Mechanism[com].Rx_Buf, com);        //��ʶ�˿ں�
                        if(__FAIL == store_comm_data(com, uart_data))           //���洮������
                        {
                            os_mbx_send(Mbx_UART_Data, (void*)uart_data, 0);    //�黹������һ��Tick���ԣ���Ϊ�ж��ڽ��գ����Կ�������˳���
                            Status_Mechanism[com].Last_Err_State = MEM_ERROR;   //�ڴ���ַ�Ƿ�
                            goto receive_end;
                        }
                        Status_Mechanism[com].Rec_Tick_Count = Status_Mechanism[com].Tmp_Tick_Count;
                        record_status_mechanism(com, DATA_REC<<4);
                        Status_Mechanism[com].Rec_State = DATA_REC;             //�ƶ�״̬������ʼ�����ⲿ����
                    }
#ifdef ZIGBEE_TEST
                }
#endif// ZIGBEE_TEST
                break;
            case DATA_REC:          //��������
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
                        if(__FAIL == store_comm_data(com, uart_data))           //���洮������
                        {
                            os_mbx_send(Mbx_UART_Data, (void*)uart_data, 0);    //�黹������һ��Tick���ԣ���Ϊ�ж��ڽ��գ����Կ�������˳���
                            Status_Mechanism[com].Last_Err_State = MEM_ERROR;   //�ڴ���ַ�Ƿ�
                            goto receive_end;
                        }
                    }
                    else
                    {
                        Status_Mechanism[com].Last_Err_State = MEM_ERROR;       //�ڴ���ַ�Ƿ�
                    }
#ifdef ZIGBEE_TEST
                }
#endif//ZIGBEE_TEST
                break;
            default:                //û�ж�Ӧ��״̬���������ݣ���һЩ��̬��״̬����ɵ�
                os_mbx_send(Mbx_UART_Data, (void*)uart_data, 0);            //�黹������һ��Tick���ԣ���Ϊ�ж��ڽ��գ����Կ�������˳���
                Status_Mechanism[com].Last_Err_State = NO_MATCH_STATUS;
                goto receive_end;
            }
            uart_status = os_mbx_wait(Mbx_UART_Data, (void**)&uart_data, 0);
        }
receive_end:
        for(com = 0; com < MAX_PORT_NUM; com++)
        {
            Status_Mechanism[com].Tmp_Tick_Count = OS_Time;         //����һ����ǰϵͳTick
            switch(Status_Mechanism[com].Rec_State)                 //�������״̬��
            {
            case DATA_REC:                  //���������ֽڳ�ʱ����
#ifdef ZIGBEE_TEST
                if(com == COM1)//zigbee
                {
                    //deal_with_zigbee_rec();//ʹ�ù̶��Ļ�״�ڴ���������
                    mem_ptr = NULL;
                    deal_with_zigbee_rec(&mem_ptr);//ʹ�ù̶��Ļ�״�ڴ���������
                    if(mem_ptr != NULL)
                    {
                        //zigbee_led_time[0] = ZIGBEE_LED_FLASH_DELAY;//5*100ms
                        local_led_time[0] = LOCAL_LED_FLASH_DELAY;//5*100ms //azh 171011
                        Status_Mechanism[com].Rx_Buf = mem_ptr;

                        record_status_mechanism(com, BYTE_REC_TMO);                         //�ֽڽ��ճ�ʱ����¼����״̬��
                        if(__TRUE == check_mbox(Status_Mechanism[com].Mbx_Answer_Buf))      //�ж��Ƿ����ڽ���Ӧ��
                        {
                            record_status_mechanism(com, CONFIG_DEFAULT_STATUS);            //��¼״̬��
                            baud_rate_config(com, Status_Mechanism[com].Default_Baud * 300);
                            Status_Mechanism[com].Rec_Baud = DEFAULT_BAUD_RATE;
                            Status_Mechanism[com].Send_Baud = DEFAULT_BAUD_RATE;
                            mode_config(com, Status_Mechanism[com].Default_Para);
                            Status_Mechanism[com].Rec_Para = DEFAULT_COM_PARA;
                            Status_Mechanism[com].Send_Para = DEFAULT_COM_PARA;
                            if(os_mbx_send(Status_Mechanism[com].Mbx_Answer_Buf, (void*)Status_Mechanism[com].Rx_Buf, 0) == OS_R_TMO)       //�������䣬֪ͨӦ�ó���ͨѶ��ɣ��յ�Ӧ��
                            {
                                free_mem_block(Status_Mechanism[com].Rx_Buf);               //�ͷ��ڴ�飬�������˵��״̬���߼�������
                                Status_Mechanism[com].Rx_Buf = NULL;
                                Status_Mechanism[com].Last_Err_State = ANSWER_MBOX_ERROR;
                            }
                        }
                        else
                        {
                            if(os_mbx_send(Mbx_Comm_Data, (void*)Status_Mechanism[com].Rx_Buf, 0) == OS_R_TMO)      //�������䣬֪ͨ����������ͨѶ�������յ��ⲿ����
                            {
                                free_mem_block(Status_Mechanism[com].Rx_Buf);               //�ͷ��ڴ�飬�������˵������������ն������
                                Status_Mechanism[com].Rx_Buf = NULL;
                                Status_Mechanism[com].Last_Err_State = COMM_MBOX_OVERFLOW;
                            }
                        }
                        Status_Mechanism[com].Rx_Buf = NULL;//azh 120816
                        Status_Mechanism[com].Rec_Mem_Type = NORMAL_TYPE;
                        record_status_mechanism(com, DATA_REC<<4);
                        Status_Mechanism[com].Rec_State = DATA_REC;                         //�ƶ�״̬�����ȴ���������
                    }
                }
                else
                {
#endif
                if((Status_Mechanism[com].Tmp_Tick_Count - Status_Mechanism[com].Rec_Tick_Count) > Status_Mechanism[com].Byte_Space)
                {
                    record_status_mechanism(com, BYTE_REC_TMO);                         //�ֽڽ��ճ�ʱ����¼����״̬��
                    if(com == COM1)//azh 131108
                    {	
                    	set_normal_run_flag(1 << 0);//���߻����ڲ�ͨ�� 
                    }
#ifdef GPRS_MODULE_PRJ	//ģ��汾 ���صư�������ں��ڲ�ͨ�ſ�	
    #ifdef ZIGBEE_TEST//azh 171011 
					if(com == COM0)//ֻҪ��������ˣ�zigbee�����Ѿ����洦����
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
                    if(__TRUE == check_mbox(Status_Mechanism[com].Mbx_Answer_Buf))      //�ж��Ƿ����ڽ���Ӧ��
                    {
                        record_status_mechanism(com, CONFIG_DEFAULT_STATUS);            //��¼״̬��
                        baud_rate_config(com, Status_Mechanism[com].Default_Baud * 300);
                        Status_Mechanism[com].Rec_Baud = DEFAULT_BAUD_RATE;
                        Status_Mechanism[com].Send_Baud = DEFAULT_BAUD_RATE;
                        mode_config(com, Status_Mechanism[com].Default_Para);
                        Status_Mechanism[com].Rec_Para = DEFAULT_COM_PARA;
                        Status_Mechanism[com].Send_Para = DEFAULT_COM_PARA;
                        if(os_mbx_send(Status_Mechanism[com].Mbx_Answer_Buf, (void*)Status_Mechanism[com].Rx_Buf, 0) == OS_R_TMO)       //�������䣬֪ͨӦ�ó���ͨѶ��ɣ��յ�Ӧ��
                        {
                            free_mem_block(Status_Mechanism[com].Rx_Buf);               //�ͷ��ڴ�飬�������˵��״̬���߼�������
                            Status_Mechanism[com].Rx_Buf = NULL;
                            Status_Mechanism[com].Last_Err_State = ANSWER_MBOX_ERROR;
                        }
                    }
                    else
                    {
                        if(os_mbx_send(Mbx_Comm_Data, (void*)Status_Mechanism[com].Rx_Buf, 0) == OS_R_TMO)      //�������䣬֪ͨ����������ͨѶ�������յ��ⲿ����
                        {
                            free_mem_block(Status_Mechanism[com].Rx_Buf);               //�ͷ��ڴ�飬�������˵������������ն������
                            Status_Mechanism[com].Rx_Buf = NULL;
                            Status_Mechanism[com].Last_Err_State = COMM_MBOX_OVERFLOW;
                        }
                    }
                    Status_Mechanism[com].Rx_Buf = NULL;//azh 120816
                    Status_Mechanism[com].Rec_Mem_Type = NORMAL_TYPE;
                    record_status_mechanism(com, WAIT_REC<<4);
                    Status_Mechanism[com].Rec_State = WAIT_REC;                         //�ƶ�״̬�����ȴ���������
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
            switch(Status_Mechanism[com].Send_State)                //������״̬��
            {
            case COM_IDLE:                  //����
                if((Status_Mechanism[com].Tmp_Tick_Count - Status_Mechanism[com].Idle_Tick_Count) > (OS_DLY_1S * 60 * 60 * 24))     //1����û�в�������
                {
                    Status_Mechanism[com].Idle_Tick_Count = Status_Mechanism[com].Tmp_Tick_Count;
                    set_RS485_rec(com);     //�����RS485�����տ��Ʒ�������
                    switch(com)
                    {
                    case 0:
                        status_mechanism_init(0);
                        UART0_init(Status_Mechanism[0].Default_Para, Status_Mechanism[0].Default_Baud * 300);       //��ʼ������0ͨѶ����
                        break;
                    case 1:
#ifdef ZIGBEE_TEST//azh171011 CODE_VER_TYPE==H_METER_PRJ		//���ѹ��
                        WLS_RST_LOW();
#endif
                        status_mechanism_init(1);
//azh 120816                        UART1_init(Status_Mechanism[1].Default_Para, Status_Mechanism[1].Default_Baud * 300);       //��ʼ������1ͨѶ����
//                        Zigbee_DMA_Init();//azh
#ifdef ZIGBEE_TEST//azh171011 CODE_VER_TYPE==H_METER_PRJ		//���ѹ��
                        WLS_RST_HIGH();
#endif                        
                        break;
                    case 2:
                        status_mechanism_init(2);
                        UART2_init(Status_Mechanism[2].Default_Para, Status_Mechanism[2].Default_Baud * 300);       //��ʼ������2ͨѶ����
                        break;
                    case 3:
                        status_mechanism_init(3);
                        UART3_init(Status_Mechanism[3].Default_Para, Status_Mechanism[3].Default_Baud * 300);       //��ʼ������3ͨѶ����
                        break;
                    }
                    Status_Mechanism[com].Last_Err_State = IDEL_TIMEOUT;
                }
                else
                {
#ifdef ZIGBEE_TEST
                    if(com == COM1)
                    {
                        if((index = query_zigbee_send_event()) < ZIGBEE_SEND_MEM_NUM)      //���ȴ������¼�
                        {
                            Status_Mechanism[com].Index = index;//azh �����������
                            if(Zigbee_Send_Status[index].Len > MAX_ZIGBEE_FRAME_LEN + 24)       //���Ҫ���͵��ֽ�����
                            {
                                Zigbee_Send_Status[index].Len = MAX_ZIGBEE_FRAME_LEN + 24;
                            }
                            //Status_Mechanism[com].Tx_Buf = Zigbee_Send_Status[index].Buf;
                            record_status_mechanism(com, COM_READY);
                            Status_Mechanism[com].Send_State = COM_READY;//zigbee��������һ��ר�ŵ�״̬�� ����û��COM_READY״̬
                        }
                        //��ѯ���� �۰�
                        mem_ptr = NULL;//azh 120413
                        if(os_mbx_wait(Status_Mechanism[com].Mbx_Send_Buf, (void**)&mem_ptr, 0) != OS_R_TMO)    //��鷢�Ͷ���,���ֽ��zigbee�Ķ��
                        {
                            deal_with_zigbee_send(mem_ptr);//azh ȫ��zigbee����״̬�ṹ��������
                        }
                    }
                    else
                    {
#endif// ZIGBEE_TEST
                        mem_ptr = NULL;//azh 120413
                        if(os_mbx_wait(Status_Mechanism[com].Mbx_Send_Buf, (void**)&mem_ptr, 0) != OS_R_TMO)    //��鷢�Ͷ���
                        {
                            Status_Mechanism[com].Tx_Buf = mem_ptr;
                            if(DEFAULT_BAUD_RATE != Status_Mechanism[com].Send_Baud)    //�жϷ���ʱ�Ƿ���Ҫ�޸Ĳ�����
                            {
                                record_status_mechanism(com, CONFIG_SEND_STATUS);       //��¼״̬��
                                baud_rate_config(com, Status_Mechanism[com].Send_Baud * 300);
                            }
                            if(DEFAULT_COM_PARA != Status_Mechanism[com].Send_Para)     //�жϷ���ʱ�Ƿ���Ҫ�޸�У��λ��ֹͣλ�����ݳ���
                            {
                                record_status_mechanism(com, CONFIG_SEND_STATUS);       //��¼״̬��
                                mode_config(com, Status_Mechanism[com].Send_Para);
                            }
                            set_RS485_send(com);            //�����RS485�����Ϳ��Ʒ�������
                            record_status_mechanism(com, COM_READY);
                            Status_Mechanism[com].Send_State = COM_READY;
                        }
#ifdef ZIGBEE_TEST
                    }
#endif// ZIGBEE_TEST
                }
                break;
            case COM_READY:         //׼��ͨѶ
                Status_Mechanism[com].Data_Send_Num = 0;
                Status_Mechanism[com].Send_Tick_Count = Status_Mechanism[com].Tmp_Tick_Count;
                record_status_mechanism(com, DATA_SEND);
                Status_Mechanism[com].Send_State = DATA_SEND;
                Meter_LED_Cnt[com] = LED_FLASH_TIMES;       //ָʾ����
                switch(com)
                {
                case 0:
                    U0THR = Status_Mechanism[0].Tx_Buf[DATA_OFFSET];
                    Status_Mechanism[0].Data_Send_Num++;
                    Status_Mechanism[com].Send_Tick_Space = OS_DLY_50MS * (Status_Mechanism[com].Tx_Buf[0] | (Status_Mechanism[com].Tx_Buf[1] << 8));//azh 140807 
                    U0IER |= 0x02;  //ʹ�ܷ����ж�
#ifdef GPRS_MODULE_PRJ//ģ��汾 ���صư�������ں��ڲ�ͨ�ſ�	
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
                    U1IER |= 0x02;  //ʹ�ܷ����ж�
                    break;
                case 2:
                    U2THR = Status_Mechanism[2].Tx_Buf[DATA_OFFSET];
                    Status_Mechanism[2].Data_Send_Num++;
                    Status_Mechanism[com].Send_Tick_Space = OS_DLY_50MS * (Status_Mechanism[com].Tx_Buf[0] | (Status_Mechanism[com].Tx_Buf[1] << 8));//azh 140807 
                    U2IER |= 0x02;  //ʹ�ܷ����ж�
                    break;
                case 3:
                    U3THR = Status_Mechanism[3].Tx_Buf[DATA_OFFSET];
                    Status_Mechanism[3].Data_Send_Num++;
                    Status_Mechanism[com].Send_Tick_Space = OS_DLY_50MS * (Status_Mechanism[com].Tx_Buf[0] | (Status_Mechanism[com].Tx_Buf[1] << 8));//azh 140807 
                    U3IER |= 0x02;  //ʹ�ܷ����ж�
                    break;
                }
                break;
            case DATA_SEND:         //����
#ifdef ZIGBEE_TEST
                if(com == COM1)
                {
                    if(Status_Mechanism[com].Tmp_Tick_Count - Status_Mechanism[com].Send_Tick_Count > WAIT_ANSWER_100MS * 3)
                    {
                        free_zigbee_send_mem(Status_Mechanism[com].Index);    //�ͷ��ڴ��

                        status_mechanism_init(com);
                        UART1_init(Status_Mechanism[com].Default_Para, Status_Mechanism[com].Default_Baud * 300);       //��ʼ������1ͨѶ����     //��ʼ������1ͨѶ����
//                        Zigbee_DMA_Init();//azh

                        Status_Mechanism[com].Last_Err_State = SEND_TIMEOUT;
                        Status_Mechanism[com].Tx_Buf = NULL;//
                        Status_Mechanism[com].Idle_Tick_Count = Status_Mechanism[com].Tmp_Tick_Count;
                        record_status_mechanism(com, COM_IDLE);                      //�ƶ�״̬��
                        Status_Mechanism[com].Send_State = COM_IDLE;
                    }
//                    else if((DMACC3Config & (1 << 17)) == 0)
//                    {
//
//                        if((Zigbee_Send_Status[Status_Mechanism[com].Index].Flag & WAIT_RESEND) == 0)
//                        {
//                            free_zigbee_send_mem(Status_Mechanism[com].Index);        //�Ƿ���ģ��������Ҫ�ȴ��ط����ͷ��ڴ��
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
                     > (Status_Mechanism[com].Send_Tick_Space))//azh 140807 * (Status_Mechanism[com].Tx_Buf[0] | (Status_Mechanism[com].Tx_Buf[1] << 8))))        //����ֵ�����ݸ���
                    {
                        status_mechanism_init(com);//add
                        Status_Mechanism[com].Last_Err_State = SEND_TIMEOUT;
                        switch(com)
                        {
                        case 0:
                            UART0_init(Status_Mechanism[0].Default_Para, Status_Mechanism[0].Default_Baud * 300);       //��ʼ������0ͨѶ����
                            break;
                        case 1:
//                            UART1_init(Status_Mechanism[1].Default_Para, Status_Mechanism[1].Default_Baud * 300);       //��ʼ������1ͨѶ����
                            break;
                        case 2:
                            UART2_init(Status_Mechanism[2].Default_Para, Status_Mechanism[2].Default_Baud * 300);       //��ʼ������2ͨѶ����
                            break;
                        case 3:
                            UART3_init(Status_Mechanism[3].Default_Para, Status_Mechanism[3].Default_Baud * 300);       //��ʼ������3ͨѶ����
                            break;
                        }
                        free_mem_block(Status_Mechanism[com].Tx_Buf);                           //�ͷ��ڴ��
                        Status_Mechanism[com].Tx_Buf = NULL;
                        Status_Mechanism[com].Idle_Tick_Count = Status_Mechanism[com].Tmp_Tick_Count;
                        record_status_mechanism(com, COM_IDLE);                                 //�ƶ�״̬��
                        Status_Mechanism[com].Send_State = COM_IDLE;
                    }
#ifdef ZIGBEE_TEST
                }
#endif// ZIGBEE_TEST
                break;
            case DATA_SEND_END:     //������ɣ����ݴ���FIFO
                flag = 0;
#ifdef ZIGBEE_TEST
                if(com == 1)
                {
//                    if(((U1LSR & (1 << 6)) != 0)                                        //�����λ�Ĵ����գ������������
//                        || ((OS_Time - Status_Mechanism[com].Send_End_Tick_Count) > (Status_Mechanism[com].Send_Tick_Space * 16)))   //���ͳ�ʱ������ֵ�����ݸ��������һ��FIFO������16�ֽڣ�
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
                    set_RS485_rec(com);     //�����RS485�����տ��Ʒ�������
                    if(__TRUE != check_mbox(Status_Mechanism[com].Mbx_Answer_Buf))      //�ж��Ƿ���Ӧ��
                    {
                        //azh 111216
                        if(DEFAULT_BAUD_RATE != Status_Mechanism[com].Send_Baud)             //�жϷ���ʱ�Ƿ��޸��˲�����
                        {
                            record_status_mechanism(com, CONFIG_DEFAULT_STATUS);             //��¼״̬��
                            baud_rate_config(com, Status_Mechanism[com].Default_Baud * 300);
                        }
                        if(DEFAULT_COM_PARA != Status_Mechanism[com].Send_Para)              //�жϷ���ʱ�Ƿ��޸���У��λ��ֹͣλ�����ݳ���
                        {
                            record_status_mechanism(com, CONFIG_DEFAULT_STATUS);             //��¼״̬��
                            mode_config(com, Status_Mechanism[com].Default_Para);
                        }
                    }
                    else
                    {
                        if(DEFAULT_BAUD_RATE != Status_Mechanism[com].Rec_Baud)         //�жϽ���Ӧ��ʱ�Ƿ���Ҫ�޸Ĳ�����
                        {
                            record_status_mechanism(com, CONFIG_REC_STATUS<<4);         //��¼״̬��
                            baud_rate_config(com, Status_Mechanism[com].Rec_Baud * 300);
                        }
                        if(DEFAULT_COM_PARA != Status_Mechanism[com].Rec_Para)          //�жϽ���Ӧ��ʱ�Ƿ���Ҫ�޸�У��λ��ֹͣλ�����ݳ���
                        {
                            record_status_mechanism(com, CONFIG_REC_STATUS<<4);         //��¼״̬��
                            mode_config(com, Status_Mechanism[com].Rec_Para);
                        }
                    }
                    Status_Mechanism[com].Dly_Tick_Count = Status_Mechanism[com].Tmp_Tick_Count;
                    record_status_mechanism(com, WAIT_NEXT);
                    Status_Mechanism[com].Send_State = WAIT_NEXT;                       //��������ͨѶ
                }
                break;
            case WAIT_NEXT:         //��ʱ���ȴ��´�ͨѶ
                if((Status_Mechanism[com].Tmp_Tick_Count - Status_Mechanism[com].Dly_Tick_Count) > Status_Mechanism[com].Dly_Tick_Space)
                {
                    Status_Mechanism[com].Idle_Tick_Count = Status_Mechanism[com].Tmp_Tick_Count;
                    record_status_mechanism(com, COM_IDLE);
                    Status_Mechanism[com].Send_State = COM_IDLE;                    //�ƶ�״̬������ѯ�Ƿ����µ�ͨѶ����
                }
                break;
            }
        }
    }
}

/******************************************************************************
 Function name:  S8 store_comm_data (U8 com, U32 data)
 Author       :  snap.gao
 Description  :  ���洮�ڽ��յ�������
 Input        :  com :���ں�
                 data:��������
 Return       :  �ڴ���ַ���Ϸ�ʱ����__FAIL
                 ��ȷ������ڴ���������ʱ������__SUCCEED
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
    len = (data >> 28) & 0x03;          //���ݳ��ȣ����3��
    if(len == 0)                        //���ݳ��Ȳ����ǡ�0��
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
        total_len = MEM_64_SIZE;        //Ĭ����С�ĳ���
        break;
    }

    len_ptr = (U16*)&Status_Mechanism[com].Rx_Buf[DATA_LEN_OFFSET];
    for(j = 0; j < len; j++)
    {
        if((*len_ptr) < (total_len - DATA_OFFSET))      //����Ƿ񳬳��ڴ�鷶Χ
        {
            Status_Mechanism[com].Rx_Buf[DATA_OFFSET + *len_ptr] = data >> ((len - 1 - j) * 8);     //����һ������
            (*len_ptr)++;               //���ӳ���
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
 Description  :  ��¼һ�������״̬��������ʱ���������������
 Input        :  com      : ���ں�
                 mechanism: ״̬��
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
 Description  :  RS485����Ϊ����
 Input        :  com: ���ںţ�0->MAX_PORT_NUM-1
 Return       :  None
 *****************************************************************************/
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//��CPU_GPRSģ��
//��ģ��û���õ�485 ���ڲ�һ������ֱ�Ӿ�����ͨ��
void set_RS485_rec (U8 com)
{
/*
    switch(com)
    {
    case 0: //485_1
        SET_PORT0_HIGH(P0_GPRS_4851_CTL);  //��Ϊ����
        SET_PORT0_OUT(P0_GPRS_4851_CTL);
        break;
    case 1:
        break;
    case 2: //485_2
//        SET_PORT1_HIGH(P1_SW4852);  //��Ϊ����
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
        SET_PORT1_HIGH(P1_SW4851);  //��Ϊ����
        SET_PORT1_OUT(P1_SW4851);
        break;
    case 1:
        break;
    case 2: //485_2
        SET_PORT1_HIGH(P1_SW4852);  //��Ϊ����
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
 Description  :  RS485����Ϊ����
 Input        :  com: ���ںţ�0->MAX_PORT_NUM-1
 Return       :  None
 *****************************************************************************/
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//��CPU_GPRSģ��
void set_RS485_send (U8 com)
{
/*
    switch(com)
    {
    case 0: //485_1
        SET_PORT0_LOW(P0_GPRS_4851_CTL);   //��Ϊ����
        SET_PORT0_OUT(P0_GPRS_4851_CTL);
        break;
    case 1:
        break;
    case 2://485_2
//        SET_PORT1_LOW(P1_SW4852);   //��Ϊ���� ����͵�485оƬΪ�� ����
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
        SET_PORT1_LOW(P1_SW4851);   //��Ϊ����
        SET_PORT1_OUT(P1_SW4851);
        break;
    case 1:
        break;
    case 2://485_2
        SET_PORT1_LOW(P1_SW4852);   //��Ϊ���� ����͵�485оƬΪ�� ����
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
 Description  :  ��ʼ��UARTͨѶ֡��ͷ��Ϣ�����������ȺͶ˿ں�
 Input        :  info: ͨѶ֡��ʼ��ַ
 Return       :  None
 *****************************************************************************/
void init_frame_info (U8 *info)
{
    info[SEND_BAUD_OFFSET] = (U8)DEFAULT_BAUD_RATE;
    info[SEND_BAUD_OFFSET + 1] = (U8)DEFAULT_BAUD_RATE;     //Ĭ�ϲ�����
    info[SEND_PARA_OFFSET] = DEFAULT_COM_PARA;              //Ĭ�ϲ���
    info[REC_BAUD_OFFSET] = (U8)DEFAULT_BAUD_RATE;
    info[REC_BAUD_OFFSET + 1] = (U8)DEFAULT_BAUD_RATE;      //Ĭ�ϲ�����
    info[REC_PARA_OFFSET] = DEFAULT_COM_PARA;               //Ĭ�ϲ���
    info[REC_MEM_TYPE_OFFSET] = NORMAL_TYPE;                //Ĭ�ϲ�ָ������Ӧ���ڴ�������
    info[DEAL_FLAG_OFFSET] = 0;                             //����������⴦���־
}

/******************************************************************************
 Function name:  void set_frame_len (U8 *info, U16 len)
 Author       :  snap.gao
 Description  :  ����ͨѶ֡�ĳ���
 Input        :  info: ͨѶ֡��ʼ��ַ
                 len : ���ݳ���
 Return       :  None
 *****************************************************************************/
void set_frame_len (U8 *info, U16 len)
{
    info[DATA_LEN_OFFSET] = len;
    info[DATA_LEN_OFFSET + 1] = len >> 8;           //���ó���
}

/******************************************************************************
 Function name:  void set_port_num (U8 *info, U8 port)
 Author       :  snap.gao
 Description  :  ����ͨѶ֡�Ĵ��ں�0->xx
 Input        :  info: ͨѶ֡��ʼ��ַ
                 port: ���ں�
 Return       :  None
 *****************************************************************************/
void set_port_num (U8 *info, U8 port)
{
    info[COM_NUM_OFFSET] = port;                    //���ô��ں�
}

/******************************************************************************
 Function name:  void set_send_para (U8 *info, U8 para, U16 baud)
 Author       :  snap.gao
 Description  :  ����ͨѶ���ڷ���ʱ�Ĳ���
 Input        :  info: ͨѶ֡��ʼ��ַ
                 para: ���ڲ���
                 baud: ���ڲ�����
 Return       :  None
 *****************************************************************************/
void set_send_para (U8 *info, U8 para, U16 baud)
{
    info[SEND_BAUD_OFFSET] = baud;
    info[SEND_BAUD_OFFSET + 1] = baud >> 8;         //���ò�����
    info[SEND_PARA_OFFSET] = para;                  //���ò���
}

/******************************************************************************
 Function name:  void set_rec_para (U8 *info, U8 para, U16 baud)
 Author       :  snap.gao
 Description  :  ����ͨѶ���ڽ���Ӧ��ʱ�Ĳ���
 Input        :  info: ͨѶ֡��ʼ��ַ
                 para: ���ڲ���
                 baud: ���ڲ�����
 Return       :  None
 *****************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//����CPU_GPRSģ��
void set_rec_para (U8 *info, U8 para, U16 baud)
{
    info[REC_BAUD_OFFSET] = baud;
    info[REC_BAUD_OFFSET + 1] = baud >> 8;          //���ò�����
    info[REC_PARA_OFFSET] = para;                   //���ò���
}
#endif
/******************************************************************************
 Function name:  void set_deal_flag (U8 *info, U8 flag)
 Author       :  snap.gao
 Description  :  ����ͨѶ֡�����⴦���־
 Input        :  info: ͨѶ֡��ʼ��ַ
                 flag: ���⴦���־����bit����
 Return       :  None
 *****************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//����CPU_GPRSģ��
void set_deal_flag (U8 *info, U8 flag)
{
    info[DEAL_FLAG_OFFSET] |= flag;                 //�������⴦���־
}
#endif
/******************************************************************************
 Function name:  void UART_send_push (U8 com)
 Author       :  snap.gao
 Description  :  ���жϰ����ݶ������󣨴���FIFO������״̬����DATA_SEND�Ƶ�DATA_SEND_END
 Input        :  com: ���ں�
 Return       :  None
 *****************************************************************************/
void UART_send_push (U8 com)
{
    if(DATA_SEND == Status_Mechanism[com].Send_State)
    {
        Status_Mechanism[com].Send_End_Tick_Count = OS_Time;
        record_status_mechanism(com, DATA_SEND_END);                   //��¼״̬��
        Status_Mechanism[com].Send_State = DATA_SEND_END;
    }
}

/******************************************************************************
 Function name:  void UART_mode_config (VU32 *UARTx, U8 mode)
 Author       :  snap.gao
 Description  :  ���ô���ͨѶ����������λ��У��λ��ֹͣλ
 Input        :  UARTx : UART0->UART3
                 mode  : ���ڲ���������λ+У��λ+ֹͣλ��
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
 Description  :  ���ô���ͨѶ�����ʣ�300��115200bps���߲������½������ϴ�����
 Input        :  UARTx : UART0->UART3
                 baud  : ������
 Return       :  None
 *****************************************************************************/
/*void UART_baud_rate_config (VU32 *UARTx, U32 baud)
{
    U16 dl;

    if((baud < 300) || (baud > 115200)){
        return;
    }

    dl = PCLK / 16 / baud;          // ���㲨����

    disable_irq();
    UARTx[LCR] |= 0x80;             // ��DLABλΪ1

    U0FDR = 0x10;                   // С����Ƶ�����ܣ���DIVADDVAL=0��MULVAL=0(����Ϊ0)

    UARTx[DLM] = dl / 256;
    UARTx[DLL] = dl % 256;

    UARTx[LCR] &= ~0x80;            // ��DLABλΪ0
    enable_irq();
}
*/
//azh 110516
/******************************************************************************
** ��������: void UART_baud_rate_config(VU32 *UARTx, U32 baud)
** ��������: ���ô���ͨѶ�����ʣ�300��115200bps���߲������¿��ܲ����ϴ�����
** ��ڲ���: UARTx: UART0->UART3
**           baud : ������
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void UART_baud_rate_config (VU32 *UARTx, U32 baud)
{
    U16 dl;

    if((baud < 300) || (baud > 115200))
    {
        return;
    }

    dl = FCCLK / 2 / 16 / baud;     //���㲨���ʣ�4�����ڵ�����ʱ�Ӷ���2��Ƶ
//azh 111223
//    __disable_irq();
    UARTx[LCR] |= 0x80;             //��DLABλΪ1���������ò�����

    UARTx[FDR] = 0x10;              //С����Ƶ�����ܣ���DIVADDVAL=0��MULVAL=0(����Ϊ0)

    UARTx[DLM] = dl / 256;
    UARTx[DLL] = dl % 256;

    UARTx[LCR] &= ~0x80;            //��DLABλΪ0
//    __enable_irq();
}
/******************************************************************************
 Function name:  void UART_mode_config (VU32 *UARTx, U8 mode)
 Author       :  snap.gao
 Description  :  ��ͨѶʱ�ı䴮��ͨѶ����������λ��У��λ��ֹͣλ
 Input        :  com : 0->xx
                 mode: ���ڲ���������λ+У��λ+ֹͣλ��
 Return       :  None
 *****************************************************************************/
/*
void mode_config (U8 com, U8 mode)
{
    switch(com)
    {
    case 0:
        UART_mode_config(UART0, mode);      //�޸Ĳ���
        break;
    case 1:
        UART_mode_config(UART1, mode);      //�޸Ĳ���
        break;
    case 2:
        UART_mode_config(UART2, mode);      //�޸Ĳ���
        break;
    case 3:
        UART_mode_config(UART3, mode);      //�޸Ĳ���
        break;
    }
}
*/
/******************************************************************************
** ��������: void mode_config(U8 com, U8 mode)
** ��������: ���ô���ͨѶ����������λ��У��λ��ֹͣλ
** ��ڲ���: com : 0->MAX_PORT_NUM-1
**           mode: ���ڲ���������λ+У��λ+ֹͣλ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void mode_config (U8 com, U8 mode)
{
    switch(com)
    {
    case 0:
        U0LCR = mode & 0x3f;        //�޸Ĳ���
        break;
    case 1:
        U1LCR = mode & 0x3f;        //�޸Ĳ���
        break;
    case 2:
        U2LCR = mode & 0x3f;        //�޸Ĳ���
        break;
    case 3:
        U3LCR = mode & 0x3f;        //�޸Ĳ���
        break;
    }
}
/******************************************************************************
 Function name:  void UART_baud_rate_config (VU32 *UARTx, U32 baud)
 Author       :  snap.gao
 Description  :  ��ͨѶʱ�ı䴮��ͨѶ������
 Input        :  com : 0->xx
                 baud: ������
 Return       :  None
 *****************************************************************************/
void baud_rate_config (U8 com, U32 baud)
{
    switch(com)
    {
    case 0:
        UART_baud_rate_config(UART0, baud);     //�޸Ĳ�����
        break;
    case 1:
        UART_baud_rate_config(UART1, baud);     //�޸Ĳ�����
        break;
    case 2:
        UART_baud_rate_config(UART2, baud);     //�޸Ĳ�����
        break;
    case 3:
        UART_baud_rate_config(UART3, baud);     //�޸Ĳ�����
        break;
    }
}

/******************************************************************************
 Function name:  void UART0_init (U8 mode, U32 baud)
 Author       :  snap.gao
 Description  :  ��ʼ������0������һ��Ĭ��ֵ������λ��У��λ��ֹͣλ��������
 Input        :  mode  : ���ڲ���������λ+У��λ+ֹͣλ��
                 baud  : ������
 Return       :  None
 *****************************************************************************/
/*
void UART0_init (U8 mode, U32 baud)
{
    PCONP |= (1 << PCUART0);                        // ʹ��UART0
    VICIntEnClr |= (1 << UART0_INT);                // ��ʼ��UART0�ж�VIC�ӿ�
    U0IER = 0;                                      // ��ʼ���ж�

    PINSEL0  &= ~((0x03u << 4) | (0x03u << 6));     // ��������bits
    PINSEL0  |= ((FUN1 << 4) | (FUN1 << 6));        // ��ʼ����ص�I/O������P0.2��P0.3���ӵ�UART0

    UART_mode_config(UART0, mode);                  // ��ʼ������ͨѶ����
    UART_baud_rate_config(UART0, baud);             // ��ʼ�����ڲ�����

    U0FCR = 0x04;                       // ��λFIFO
    U0FCR = 0x01 | (2 << 6);            // ʹ��FIFO������FIFO���Ϊ8
    U0ACR = 0;                          // ��ʼ���Զ������ʿ��ƼĴ�
    U0IER = 0x01;                       // ʹ�ܽ����ж�

    VICVectPriority6 = UART0_ISR_PRIO;
    VICVectAddr6 = (U32)UART0_IRQ_handler;
    VICIntEnable |= (1 << UART0_INT);   // ����UART0�ж�
}
*/
//azh 110516
/******************************************************************************
** ��������: void UART0_init(U8 mode, U32 baud)
** ��������: ��ʼ������0������һ��Ĭ��ֵ������λ��У��λ��ֹͣλ��������
** ��ڲ���: mode: ���ڲ���������λ+У��λ+ֹͣλ��
**           baud: ������
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void UART0_init (U8 mode, U32 baud)
{
    PCONP |= (1 << PCUART0);                        //ʹ��UART0
//    DISABLE_ISR(NVIC_UART0);                        //disbale UART0 interrupt
//isr_clr(NVIC_UART0);

    U0IER = 0;                                      //��ʼ���ж�

    PCLKSEL0 &= ~(0x03u << 6);
    PCLKSEL0 |= (FPCLK_UART0_DIV << 6);             //��������ʱ�ӷ�Ƶϵ��

    if((PINSEL0 & ((0x03u << 4) | (0x03u << 6))) != ((P0_3_FNUC << 6) | (P0_2_FNUC << 4)))
    {
        PINSEL0 &= ~((0x03u << 4) | (0x03u << 6));              //��������bits
        PINSEL0 |= ((P0_3_FNUC << 6) | (P0_2_FNUC << 4));       //configure the TXD0 and RXD0 as the alternate function 1 configuration
    }
    PINMODE0 &= ~((0x03u << 4) | (0x03u << 6));                 //��������bits
    PINMODE0 |= ((P0_3_MODE << 6) | (P0_2_MODE << 4));          //TXD0 and RXD0 has neither pull-up nor pull-down registers
    PINMODE_OD0 &= ~(P0_TXD_4851 | P0_RXD_4851);                  //�������ŷǿ�©ģʽ

    UART_baud_rate_config(UART0, baud);             //��ʼ�����ڲ�����
    U0LCR = mode & 0x3f;;                           //��ʼ������ͨѶ����

    U0FCR = 0x01 | 0x06 | (1 << 3) | (2 << 6);      //��λ/ʹ��FIFO������FIFO���Ϊ8��ʹ��DMA
    U0RBR;
    U0ACR = 0;                                      //��ʼ���Զ������ʿ��ƼĴ�
    U0IER = 0x01;                                   //ʹ�ܽ����ж�

//    ENABLE_ISR(NVIC_UART0, PRIO_UART0);             //set UART0 interrupt parameter
//isr_set(NVIC_UART0, PRIO_UART0);                      //azh 110609 ��MAIN�����г�ʼ��
}
/******************************************************************************
 Function name:  void UART1_init (U8 mode, U32 baud)
 Author       :  snap.gao
 Description  :  ��ʼ������1������һ��Ĭ��ֵ������λ��У��λ��ֹͣλ��������
 Input        :  mode  : ���ڲ���������λ+У��λ+ֹͣλ��
                 baud  : ������
 Return       :  None
 *****************************************************************************/
/*
void UART1_init (U8 mode, U32 baud)
{
    PCONP |= (1 << PCUART1);                        // ʹ��UART1
    VICIntEnClr |= (1 << UART1_INT);                // ��ʼ��UART1�ж�VIC�ӿ�
    U1IER = 0;                                      // ��ʼ���ж�

    PINSEL0  &= ~(0x03u << 30);                     // ��������bits
    PINSEL1  &= ~(0x03u << 0);                      // ��������bits
    PINSEL0  |= (FUN1 << 30);                       // ��ʼ����ص�I/O������P0.15���ӵ�UART1
    PINSEL1  |= (FUN1 << 0);                        // ��ʼ����ص�I/O������P0.16���ӵ�UART1

    UART_mode_config(UART1, mode);                  // ��ʼ������ͨѶ����
    UART_baud_rate_config(UART1, baud);             // ��ʼ�����ڲ�����

    U1FCR = 0x04;                       // ��λFIFO
    U1FCR = 0x01 | (2 << 6);            // ʹ��FIFO������FIFO���Ϊ8
    U1ACR = 0;                          // ��ʼ���Զ������ʿ��ƼĴ�
    U1IER = 0x01;                       // ʹ�ܽ����ж�

    VICVectPriority7 = UART1_ISR_PRIO;
    VICVectAddr7 = (U32)UART1_IRQ_handler;
    VICIntEnable |= (1 << UART1_INT);   // ����UART1�ж�
}
*/
/******************************************************************************
** ��������: void UART1_init(U8 mode, U32 baud)
** ��������: ��ʼ������1������һ��Ĭ��ֵ������λ��У��λ��ֹͣλ��������
** ��ڲ���: mode: ���ڲ���������λ+У��λ+ֹͣλ��
**           baud: ������
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void UART1_init (U8 mode, U32 baud)
{
    PCONP |= (1 << PCUART1);                        //ʹ��UART1
//    DISABLE_ISR(NVIC_UART1);                        //disbale UART1 interrupt
    U1IER = 0;                                      //��ʼ���ж�

    PCLKSEL0 &= ~(0x03u << 8);
    PCLKSEL0 |= (FPCLK_UART1_DIV << 8);             //��������ʱ�ӷ�Ƶϵ��
#if CODE_VER_TYPE==H_METER_PRJ				//��ϸ�ѹ��
    if((PINSEL4 & ((0x03u << 14) | (0x03u << 4) | (0x03u << 2) | (0x03u << 0)))
      != ((P2_7_FNUC << 14) | (P2_2_FNUC << 4) | (P2_1_FNUC << 2) | (P2_0_FNUC << 0)))
    {
        PINSEL4 &= ~((0x03u << 14) | (0x03u << 4) | (0x03u << 2) | (0x03u << 0));                   //��������bits
        PINSEL4 |= ((P2_7_FNUC << 14) | (P2_2_FNUC << 4) | (P2_1_FNUC << 2) | (P2_0_FNUC << 0));    //configure the RTS1 CTS1 TXD1 and RXD1 as the alternate function 1 configuration
    }
    PINMODE4 &= ~((0x03u << 14) | (0x03u << 4) | (0x03u << 2) | (0x03u << 0));                      //��������bits
    PINMODE4 |= ((P2_7_MODE << 14) | (P2_2_MODE << 4) | (P2_1_MODE << 2) | (P2_0_MODE << 0));       //RTS1 CTS1 TXD1 and RXD1 has neither pull-up nor pull-down registers
    PINMODE_OD2 &= ~(P2_RTS_WLS | P2_CTS_WLS | P2_TXD_WLS | P2_RXD_WLS);                            //�������ŷǿ�©ģʽ

    UART_baud_rate_config(UART1, baud);             //��ʼ�����ڲ�����
    U1LCR = mode & 0x3f;                            //��ʼ������ͨѶ����

    U1FCR = 0x01 | 0x06 | (1 << 3) | (2 << 6);      //��λ/ʹ��FIFO������FIFO���Ϊ8��ʹ��DMA
    U1RBR;
    U1ACR = 0;                                      //��ʼ���Զ������ʿ��ƼĴ�
//#ifdef PHASE_C_MODULE
    U1MCR = (1 << 6) | (1 << 7);                    //ʹ���Զ�CTS��RTS
    U1TER = (1 << 7);                               //ʹ�������صķ���

//#endif//PHASE_C_MODULE
    U1RS485CTRL = 0;                                //��ֹRS485���ƹ���
    U1IER = 0x01;                                   //ʹ�ܽ����ж�

//  WLS_RST_HIGH();                                 //RST_WLS�����1��,ZigBee��λ����
//    ENABLE_ISR(NVIC_UART1, PRIO_UART1);             //set UART1 interrupt parameter

#elif CODE_VER_TYPE==WLS_CUR_PRJ			//������߲ɼ���
    if((PINSEL4 & ((0x03u << 14) | (0x03u << 4) | (0x03u << 2) | (0x03u << 0)))
      != ((P2_7_FNUC << 14) | (P2_2_FNUC << 4) | (P2_1_FNUC << 2) | (P2_0_FNUC << 0)))
    {
        PINSEL4 &= ~((0x03u << 14) | (0x03u << 4) | (0x03u << 2) | (0x03u << 0));                   //��������bits
        PINSEL4 |= ((P2_7_FNUC << 14) | (P2_2_FNUC << 4) | (P2_1_FNUC << 2) | (P2_0_FNUC << 0));    //configure the RTS1 CTS1 TXD1 and RXD1 as the alternate function 1 configuration
    }
    PINMODE4 &= ~((0x03u << 14) | (0x03u << 4) | (0x03u << 2) | (0x03u << 0));                      //��������bits
    PINMODE4 |= ((P2_7_MODE << 14) | (P2_2_MODE << 4) | (P2_1_MODE << 2) | (P2_0_MODE << 0));       //RTS1 CTS1 TXD1 and RXD1 has neither pull-up nor pull-down registers
    PINMODE_OD2 &= ~(P2_RTS_WLS | P2_CTS_WLS | P2_TXD_WLS | P2_RXD_WLS);                            //�������ŷǿ�©ģʽ

    UART_baud_rate_config(UART1, baud);             //��ʼ�����ڲ�����
    U1LCR = mode & 0x3f;                            //��ʼ������ͨѶ����

    U1FCR = 0x01 | 0x06 | (1 << 3) | (2 << 6);      //��λ/ʹ��FIFO������FIFO���Ϊ8��ʹ��DMA
    U1RBR;
    U1ACR = 0;                                      //��ʼ���Զ������ʿ��ƼĴ�

    U1RS485CTRL = 0;                                //��ֹRS485���ƹ���
    U1IER = 0x01;                                   //ʹ�ܽ����ж�

//  WLS_RST_HIGH();                                 //RST_WLS�����1��,ZigBee��λ����

#elif CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
    #ifdef ZIGBEE_TEST
        if((PINSEL4 & ((0x03u << 14) | (0x03u << 4) | (0x03u << 2) | (0x03u << 0)))
          != ((P2_7_FNUC << 14) | (P2_2_FNUC << 4) | (P2_1_FNUC << 2) | (P2_0_FNUC << 0)))
        {
            PINSEL4 &= ~((0x03u << 14) | (0x03u << 4) | (0x03u << 2) | (0x03u << 0));                   //��������bits
            PINSEL4 |= ((P2_7_FNUC << 14) | (P2_2_FNUC << 4) | (P2_1_FNUC << 2) | (P2_0_FNUC << 0));    //configure the RTS1 CTS1 TXD1 and RXD1 as the alternate function 1 configuration
        }
        PINMODE4 &= ~((0x03u << 14) | (0x03u << 4) | (0x03u << 2) | (0x03u << 0));                      //��������bits
        PINMODE4 |= ((P2_7_MODE << 14) | (P2_2_MODE << 4) | (P2_1_MODE << 2) | (P2_0_MODE << 0));       //RTS1 CTS1 TXD1 and RXD1 has neither pull-up nor pull-down registers
        PINMODE_OD2 &= ~(P2_RTS_WLS | P2_CTS_WLS | P2_TXD_LOCAL | P2_RXD_LOCAL);                            //�������ŷǿ�©ģʽ
    
        UART_baud_rate_config(UART1, baud);             //��ʼ�����ڲ�����
        U1LCR = mode & 0x3f;                            //��ʼ������ͨѶ����
    
        U1FCR = 0x01 | 0x06 | (1 << 3) | (2 << 6);      //��λ/ʹ��FIFO������FIFO���Ϊ8��ʹ��DMA
        U1RBR;
        U1ACR = 0;                                      //��ʼ���Զ������ʿ��ƼĴ�
    //#ifdef PHASE_C_MODULE
        U1MCR = (1 << 6) | (1 << 7);                    //ʹ���Զ�CTS��RTS
        U1TER = (1 << 7);                               //ʹ�������صķ���
    
    //#endif//PHASE_C_MODULE
        U1RS485CTRL = 0;                                //��ֹRS485���ƹ���
        U1IER = 0x01;                                   //ʹ�ܽ����ж�    
    #else
        if((PINSEL4 & ((0x03u << 2) | (0x03u << 0)))
          != ((P2_1_FNUC << 2) | (P2_0_FNUC << 0)))
        {
            PINSEL4 &= ~((0x03u << 2) | (0x03u << 0));                   //��������bits
            PINSEL4 |= ((P2_1_FNUC << 2) | (P2_0_FNUC << 0));    //configure the TXD1 and RXD1 as the alternate function 1 configuration
        }
        PINMODE4 &= ~((0x03u << 2) | (0x03u << 0));                      //��������bits
        PINMODE4 |= ((P2_1_MODE << 2) | (P2_0_MODE << 0));       //RTS1 CTS1 TXD1 and RXD1 has neither pull-up nor pull-down registers
        PINMODE_OD2 &= ~(P2_TXD_LOCAL | P2_RXD_LOCAL);                            //�������ŷǿ�©ģʽ
    
        UART_baud_rate_config(UART1, baud);             //��ʼ�����ڲ�����
        U1LCR = mode & 0x3f;                            //��ʼ������ͨѶ����
    
        U1FCR = 0x01 | 0x06 | (1 << 3) | (2 << 6);      //��λ/ʹ��FIFO������FIFO���Ϊ8��ʹ��DMA
        U1RBR;
        U1ACR = 0;                                      //��ʼ���Զ������ʿ��ƼĴ�
    
        U1RS485CTRL = 0;                                //��ֹRS485���ƹ���
        U1IER = 0x01;                                   //ʹ�ܽ����ж�
    #endif//ZIGBEE_TEST    
#endif//INTERNAL_COM_ZIGBEE
}
/******************************************************************************
 Function name:  void UART2_init (U8 mode, U32 baud)
 Author       :  snap.gao
 Description  :  ��ʼ������2������һ��Ĭ��ֵ������λ��У��λ��ֹͣλ��������
 Input        :  mode  : ���ڲ���������λ+У��λ+ֹͣλ��
                 baud  : ������
 Return       :  None
 *****************************************************************************/
/*void UART2_init (U8 mode, U32 baud)
{
    PCONP |= (1 << PCUART2);                        // ʹ��UART2
    VICIntEnClr |= (1 << UART2_INT);                // ��ʼ��UART2�ж�VIC�ӿ�
    U2IER = 0;                                      // ��ʼ���ж�

    PINSEL0  &= ~((0x03u << 20) | (0x03u << 22));   // ��������bits
    PINSEL0  |= ((FUN1 << 20) | (FUN1 << 22));      // ��ʼ����ص�I/O������P0.10��P0.11���ӵ�UART2

    UART_mode_config(UART2, mode);                  // ��ʼ������ͨѶ����
    UART_baud_rate_config(UART2, baud);             // ��ʼ�����ڲ�����

    U2FCR = 0x04;                       // ��λFIFO
    U2FCR = 0x01 | (2 << 6);            // ʹ��FIFO������FIFO���Ϊ8
    U2ACR = 0;                          // ��ʼ���Զ������ʿ��ƼĴ�
    U2IER = 0x01;                       // ʹ�ܽ����ж�

    VICVectPriority28 = UART2_ISR_PRIO;
    VICVectAddr28 = (U32)UART2_IRQ_handler;
    VICIntEnable |= (1 << UART2_INT);   // ����UART2�ж�
}
*/
/******************************************************************************
** ��������: void UART2_init(U8 mode, U32 baud)
** ��������: ��ʼ������2������һ��Ĭ��ֵ������λ��У��λ��ֹͣλ��������
** ��ڲ���: mode: ���ڲ���������λ+У��λ+ֹͣλ��
**           baud: ������
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void UART2_init (U8 mode, U32 baud)
{
    PCONP |= (1 << PCUART2);                        //ʹ��UART2
//    DISABLE_ISR(NVIC_UART2);                        //disbale UART2 interrupt
    U2IER = 0;                                      //��ʼ���ж�

    PCLKSEL1 &= ~(0x03u << 16);
    PCLKSEL1 |= (FPCLK_UART2_DIV << 16);            //��������ʱ�ӷ�Ƶϵ��

    if((PINSEL0 & ((0x03u << 20) | (0x03u << 22))) != ((P0_10_FNUC << 20) | (P0_11_FNUC << 22)))
    {
        PINSEL0 &= ~((0x03u << 20) | (0x03u << 22));            //��������bits
        PINSEL0 |= ((P0_10_FNUC << 20) | (P0_11_FNUC << 22));   //configure the TXD2 and RXD2 as the alternate function 1 configuration
    }
    PINMODE0 &= ~((0x03u << 20) | (0x03u << 22));               //��������bits
    PINMODE0 |= ((P0_10_MODE << 20) | (P0_11_MODE << 22));      //TXD2 and RXD2 has neither pull-up nor pull-down registers
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
    PINMODE_OD0 &= ~(P0_TXD_4852 | P0_RXD_4852);                //�������ŷǿ�©ģʽ
#endif
    UART_baud_rate_config(UART2, baud);             //��ʼ�����ڲ�����
    U2LCR = mode & 0x3f;;                           //��ʼ������ͨѶ����

    U2FCR = 0x01 | 0x06 | (2 << 6);                 //��λ/ʹ��FIFO������FIFO���Ϊ8
    U2RBR;
    U2ACR = 0;                                      //��ʼ���Զ������ʿ��ƼĴ�
    U2IER = 0x01;                                   //ʹ�ܽ����ж�

//    ENABLE_ISR(NVIC_UART2, PRIO_UART2);             //set UART2 interrupt parameter

}
/******************************************************************************
 Function name:  void UART3_init (U8 mode, U32 baud)
 Author       :  snap.gao
 Description  :  ��ʼ������3������һ��Ĭ��ֵ������λ��У��λ��ֹͣλ��������
 Input        :  mode  : ���ڲ���������λ+У��λ+ֹͣλ��
                 baud  : ������
 Return       :  None
 *****************************************************************************/
/*
void UART3_init (U8 mode, U32 baud)
{
    PCONP |= (1 << PCUART3);                        // ʹ��UART3
    VICIntEnClr |= (1 << UART3_INT);                // ��ʼ��UART3�ж�VIC�ӿ�
    U3IER = 0;                                      // ��ʼ���ж�

    PINSEL0  &= ~((0x03u << 0) | (0x03u << 2));     // ��������bits
    PINSEL0  |= ((FUN2 << 0) | (FUN2 << 2));        // ��ʼ����ص�I/O������P0.0��P0.1���ӵ�UART3

    UART_mode_config(UART3, mode);                  // ��ʼ������ͨѶ����
    UART_baud_rate_config(UART3, baud);             // ��ʼ�����ڲ�����

    U3FCR = 0x04;                       // ��λFIFO
    U3FCR = 0x01 | (2 << 6);            // ʹ��FIFO������FIFO���Ϊ8
    U3ACR = 0;                          // ��ʼ���Զ������ʿ��ƼĴ�
    U3IER = 0x01;                       // ʹ�ܽ����ж�

    VICVectPriority29 = UART3_ISR_PRIO;
    VICVectAddr29 = (U32)UART3_IRQ_handler;
    VICIntEnable |= (1 << UART3_INT);   // ����UART3�ж�
}
*/
/******************************************************************************
** ��������: void UART3_init(U8 mode, U32 baud)
** ��������: ��ʼ������3������һ��Ĭ��ֵ������λ��У��λ��ֹͣλ��������
** ��ڲ���: mode: ���ڲ���������λ+У��λ+ֹͣλ��
**           baud: ������
** �� �� ֵ: ��
** ˵    ��: ����ֻ��ʼ��RXD���ţ�TXD������TIMER2��ʼ���б���ʼ��ΪMAT2.0
******************************************************************************/
void UART3_init (U8 mode, U32 baud)
{
//azh 110516
//    Sync_Send_Status = 0;

    PCONP |= (1 << PCUART3);                        //ʹ��UART3
//    DISABLE_ISR(NVIC_UART3);                        //disbale UART3 interrupt
    U3IER = 0;                                      //��ʼ���ж�

    PCLKSEL1 &= ~(0x03u << 18);
    PCLKSEL1 |= (FPCLK_UART3_DIV << 18);            //��������ʱ�ӷ�Ƶϵ��

    if((PINSEL9 & ((0x03u << 26) | (0x03u << 24))) != ((P4_29_FNUC << 26) | (P4_28_FNUC << 24)))
    {
        PINSEL9 &= ~((0x03u << 26) | (0x03u << 24));//��������bits
        PINSEL9 |= (P4_29_FNUC << 26);              //configure the RXD3/TXD3 as the alternate function 1 configuration
    }
    PINMODE9 &= ~((0x03u << 26) | (0x03u << 24));   //��������bits
    PINMODE9 |= (P4_29_MODE << 26) | (P4_28_MODE << 26);    //RXD3/TXD3 has neither pull-up nor pull-down registers
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
    PINMODE_OD4 &= ~(P4_MC52I_RXD | P4_MC52I_TXD);  //�������ŷǿ�©ģʽ
#else
    PINMODE_OD4 &= ~(P4_GPRS_RXD1 | P4_GPRS_TXD1);  //�������ŷǿ�©ģʽ
#endif
    UART_baud_rate_config(UART3, baud);             //��ʼ�����ڲ�����
    U3LCR = mode & 0x3f;                            //��ʼ������ͨѶ����

    U3FCR = 0x01 | 0x06 | (1 << 3) | (2 << 6);      //��λ/ʹ��FIFO������FIFO���Ϊ8��ʹ��DMA
    U3RBR;
    U3ACR = 0;                                      //��ʼ���Զ������ʿ��ƼĴ�
    U3IER = 0x01;                                   //ʹ�ܽ����ж�

//    ENABLE_ISR(NVIC_UART3, PRIO_UART3);             //set UART3 interrupt parameter
}
/******************************************************************************
 Function name:  void UART_init (void)
 Author       :  snap.gao
 Description  :  ��ʼ�����д��ڲ���
 Input        :  None
 Return       :  None
 *****************************************************************************/
void UART_init (void)
{
#ifdef ZIGBEE_TEST
    init_zigbee_mem();
#endif// PHASE_C_MODULE
    UART0_init(UART0_COMM_PARA, UART0_BAUD_RATE);       // ��ʼ������0ͨѶ����
    UART1_init(UART1_COMM_PARA, UART1_BAUD_RATE);       // ��ʼ������1ͨѶ����
    UART2_init(UART2_COMM_PARA, UART2_BAUD_RATE);       // ��ʼ������2ͨѶ����
    UART3_init(UART3_COMM_PARA, UART3_BAUD_RATE);       // ��ʼ������3ͨѶ����
}

/******************************************************************************
 Function name:  void status_mechanism_init (U8 com)
 Author       :  snap.gao
 Description  :  ��ʼ�����д���״̬������
 Input        :  None
 Return       :  None
 *****************************************************************************/
void status_mechanism_init (U8 com)
{
//  U8 com;
#if CODE_VER_TYPE==WLS_CUR_PRJ			//������߲ɼ���
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
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ�� //���ں����1200 or 2400������
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
#if CODE_VER_TYPE==H_METER_PRJ				//��ϸ�ѹ�� zigbee
            Status_Mechanism[COM1].Rec_State = DATA_REC; //zigbee ��һֱ�ڽ��յ�
            Status_Mechanism[COM1].Index = 0xff;

            Status_Mechanism[COM1].Dly_Tick_Space = OS_DLY_10MS;//OS_DLY_200MS;
            Status_Mechanism[COM1].Send_Tick_Space = OS_DLY_20MS;
            Status_Mechanism[COM1].Byte_Space = OS_DLY_10MS;//OS_DLY_200MS;
            Status_Mechanism[COM1].Default_Baud = (UART1_BAUD_RATE / 300);
            Status_Mechanism[COM1].Default_Para = UART1_COMM_PARA;
#elif CODE_VER_TYPE==WLS_CUR_PRJ			//������߲ɼ��� //С���� 2400
            Status_Mechanism[COM1].Dly_Tick_Space = OS_DLY_200MS;
            Status_Mechanism[COM1].Send_Tick_Space = OS_DLY_50MS;
            Status_Mechanism[COM1].Byte_Space = OS_DLY_100MS;
            Status_Mechanism[COM1].Default_Baud = (UART1_BAUD_RATE / 300);
            Status_Mechanism[COM1].Default_Para = UART1_COMM_PARA;
#elif CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ�� //
	#ifdef ZIGBEE_TEST
            Status_Mechanism[COM1].Rec_State = DATA_REC; //zigbee ��һֱ�ڽ��յ�
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
#if CODE_VER_TYPE==WLS_CUR_PRJ			//������߲ɼ���
            //azh 131022  
			rw_mp_para(2, 0x8905, buf, &m, 'r');//������2�Ĳ�����
			modify_uart_in_run(COM2, buf[0]);          
			rw_mp_para(2, 0x8915, buf, &m, 'r');//������2�Ľ��ճ�ʱ��ռ�����û��걨�����ĵ��ֽ�10ms��λ BCD��ʽ ���ֽ�0x55��������
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
 Description  :  ���ô��ڷ�������ʱ�Ĳ�����
 Input        :  com : 0->MAX_PORT_NUM-1
                 band: ������
 Return       :  None
 *****************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//����CPU_GPRSģ��
void config_send_band (U8 com, U32 band)
{
    Status_Mechanism[com].Send_Baud = (band / 300);
}
#endif
/******************************************************************************
 Function name:  void config_send_para (U8 com, U8 para)
 Author       :  snap.gao
 Description  :  ���ô��ڷ�������ʱ��У��λ��ֹͣλ�����ݳ���
 Input        :  com : 0->MAX_PORT_NUM-1
                 para: У��λ��ֹͣλ�����ݳ���
 Return       :  None
 *****************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//����CPU_GPRSģ��
void config_send_para (U8 com, U8 para)
{
    Status_Mechanism[com].Send_Para = para;
}
#endif
/******************************************************************************
 Function name:  void config_rec_band (U8 com, U32 band)
 Author       :  snap.gao
 Description  :  ���ô��ڽ���Ӧ������ʱ�Ĳ�����
 Input        :  com : 0->MAX_PORT_NUM-1
                 band: ������
 Return       :  None
 *****************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//����CPU_GPRSģ��
void config_rec_band (U8 com, U32 band)
{
    Status_Mechanism[com].Rec_Baud = (band / 300);
}
#endif
/******************************************************************************
 Function name:  void config_rec_para (U8 com, U8 para)
 Author       :  snap.gao
 Description  :  ���ô��ڽ���Ӧ������ʱ��У��λ��ֹͣλ�����ݳ���
 Input        :  com : 0->MAX_PORT_NUM-1
                 para: У��λ��ֹͣλ�����ݳ���
 Return       :  None
 *****************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//����CPU_GPRSģ��
void config_rec_para (U8 com, U8 para)
{
    Status_Mechanism[com].Rec_Para = para;
}
#endif
/******************************************************************************
 Function name:  void config_rec_mem_type (U8 com, U8 type)
 Author       :  snap.gao
 Description  :  ���ô��ڽ���Ӧ������ʱ���ڴ������
 Input        :  com : 0->MAX_PORT_NUM-1
                 type: �ڴ������
 Return       :  None
 *****************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//����CPU_GPRSģ��
void config_rec_mem_type (U8 com, U8 type)
{
    Status_Mechanism[com].Rec_Mem_Type = type;
}
#endif
/******************************************************************************
 Function name:  S8 UART_open (U8 com, U16 time_out)
 Author       :  snap.gao
 Description  :  �򿪴��ڣ���ռ��Դ�������������񻥳⣬�ɹ��򿪺���Ĭ�ϲ�����ʼ������
                 �ڴ��ڲ����õ�����µȴ�time_out��ʱ�䣬��ʱ�󷵻�ʧ��
                 ������ڴ򿪳ɹ����ڽ������ڴ����������UART_close()�����ͷŴ���
 Input        :  com     : 0->MAX_PORT_NUM-1
                 time_out: �ڴ��ڲ����õ����������ȴ���ʱ�䣬����ʹ�õĺ꣺��Tick��2msʱ����󲻳���131s��
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
                          BLOCK��������ʽ��������Ҫʹ�ã�
                          QUERY����ѯ��ʽ��û���κεȴ���
 Return       :  ���ڴ�ʧ��ʱ����__FAIL
                 ���ڴ򿪳ɹ�ʱ����__SUCCEED
 *****************************************************************************/
S8 UART_open (U8 com, U16 time_out)
{
    if(com >= MAX_PORT_NUM)             //���ں�0->xx
    {
        return __FAIL;                  //�˿ںŴ���
    }
    if(os_mut_wait(Status_Mechanism[com].Mut_Com_Usable, time_out) == OS_R_TMO)
    {
        return __FAIL;                  //û�еõ�������Դ
    }
    switch(com)
    {
    case 0:
//        UART0_init(UART0_COMM_PARA, UART0_BAUD_RATE);       // ��ʼ������0ͨѶ����
        break;
    case 1:
//        UART1_init(UART1_COMM_PARA, UART1_BAUD_RATE);       // ��ʼ������1ͨѶ����
        break;
    case 2:
//        UART2_init(UART2_COMM_PARA, UART2_BAUD_RATE);       // ��ʼ������2ͨѶ����
        break;
    case 3:
//        UART3_init(UART3_COMM_PARA, UART3_BAUD_RATE);       // ��ʼ������3ͨѶ����
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
** ��������: S32 uart_send_idle(U8 com, U32 time_out)
** ��������: ��ʹ�ô���֮ǰ��鴮���Ƿ���У��ɹ��򿪺���Ĭ�ϲ�����ʼ������
**           �ڴ��ڲ����õ�����µȴ�time_out��ʱ�䣬��ʱ�󷵻�ʧ��
** ��ڲ���: com     : 0->MAX_PORT_NUM-1
**           time_out: �ڴ��ڲ����õ����������ȴ���ʱ�䣬����ʹ�õĺ�
**              QUERY����ѯ��ʽ��û���κεȴ���
** �� �� ֵ: ���ڴ�ʧ��ʱ����__FAIL
**           ���ڴ򿪳ɹ�ʱ����__SUCCEED
** ˵    ��: ��Ҫ��������д�ڲ�FLASHʱ��ѯ
******************************************************************************/
S32 uart_send_idle (U8 com, U32 time_out)
{

    if(com >= MAX_PORT_NUM)             //���ں�0->xx
    {
        return __FAIL;                  //�˿ںŴ���
    }

    if(Status_Mechanism[com].Send_State == COM_IDLE)
    {
        return __SUCCEED;
    }
    else
    {
        return __FAIL;          //û�еõ�������Դ
    }
}
/******************************************************************************
 Function name:  S8 UART_send (U8 com, U16 len, U8 *sendbuf)
 Author       :  snap.gao
 Description  :  �������ݷ��͵Ľӿں���
 Input        :  com      : 0->MAX_PORT_NUM-1
                 len      : �������ݳ���
                 sendbuf  : �������ݵĻ�����
                            ͷ��16�ֽڱ���ͨѶ֡��Ϣ�����庬��ο���ͨѶ֡ͷ����Ϣ�ĵ�ַƫ�ƺ궨�塱
 Return       :  __FAIL   ������ʧ�ܣ���ҪӦ�ó������ͻ��������ط����ͷ�
                 __SUCCEED�����ͳɹ�
 *****************************************************************************/
S8 UART_send (U8 com, U16 len, U8 *sendbuf)
{
    if(check_mem_type(sendbuf) == INVALID_TYPE)
    {
        return __FAIL;              //�ڴ���ַ���󣬷��غ���Ҫ�����ͻ�����
    }
    if(com >= MAX_PORT_NUM)
    {
        return __FAIL;              //�˿ںŴ��󣬷��غ���Ҫ�����ͻ�����
    }

    init_frame_info(sendbuf);
//�������������ʾ Ҫ��GPRS�źŷ��ʹ���������������ʾ
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
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
        return __FAIL;              //û�а����ݷ��������غ���Ҫ�����ͻ�����
    }
    else
    {
        return __SUCCEED;           //�ɹ�����
    }
}

/******************************************************************************
 Function name:  U8* UART_wait_answer (U8 com, U8 wait_time)
 Author       :  snap.gao
 Description  :  �ȴ�����Ӧ�����ݵĽӿں���
 Input        :  com      : 0->MAX_PORT_NUM-1
                 wait_time: �ȴ�Ӧ���ʱ�䣬����ʹ�õĺ꣺��Tick��2msʱ����󲻳���131s��
                            bit7=0 ��λ10ms��bit7=1 ��λ1s
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
 Return       :  �Ϸ��ڴ���ַ���յ���Ӧ��֡
                 NULL          ��û���յ�Ӧ��֡
 *****************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//����CPU_GPRSģ��
U8* UART_wait_answer (U8 com, U8 wait_time)
{
    void *p;
    U16  wait_tick;

    if(os_mbx_wait(Status_Mechanism[com].Mbx_Answer_Buf, &p, 0) != OS_R_TMO)    //�����������û���������ڴ�飨���ڽ���Ӧ��Ӧ�ó����Ѿ���ʱ������±�������
    {
        free_mem_block(p);              //һ�㲻���ܳ��������ڴ��
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
        wait_tick = 0xfffe;             //ϵͳ֧�ֵ������ʱ
    }
    if(os_mbx_wait(Status_Mechanism[com].Mbx_Answer_Buf, &p, wait_tick) == OS_R_TMO)
    {
        return NULL;                    //û���յ�Ӧ��֡
    }
    else
    {
        return (U8*)p;                  //�յ�Ӧ��֡
    }
}
#endif
/******************************************************************************
 Function name:  S8 UART_resume (U8 com)
 Author       :  snap.gao
 Description  :  �ָ����ڵ�Ĭ�ϲ�����ֻ���ڽ���Ӧ��ʱ��Ĳ����ָ�
 Input        :  com: 0->MAX_PORT_NUM-1
 Return       :  ���ڲ����ָ�ʧ��ʱ����__FAIL
                 ���ڲ����ָ��ɹ�ʱ����__SUCCEED
 *****************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//����CPU_GPRSģ��
S8 UART_resume (U8 com)
{
    if(com >= MAX_PORT_NUM)             //���ں�0->MAX_PORT_NUM-1
    {
        return __FAIL;                  //�˿ںŴ���
    }
    record_status_mechanism(com, ANSWER_REC_TMO);               //��¼״̬����Ӧ��֡���ճ�ʱ
    record_status_mechanism(com, CONFIG_DEFAULT_STATUS);        //��¼״̬��
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
 Description  :  �رմ��ڣ��ͷ���Դ
 Input        :  com: 0->MAX_PORT_NUM-1
 Return       :  ���ڹر�ʧ��ʱ����__FAIL
                 ���ڹرճɹ�ʱ����__SUCCEED
 *****************************************************************************/
S8 UART_close (U8 com)
{
    if(com >= MAX_PORT_NUM)             //���ں�0->MAX_PORT_NUM-1
    {
        return __FAIL;                  //�˿ںŴ���
    }
    os_mut_release(Status_Mechanism[com].Mut_Com_Usable);   // �ͷŴ���
    return __SUCCEED;
}

/******************************************************************************
 Function name:  U8 check_mbox (OS_ID mailbox)
 Author       :  snap.gao
 Description  :  ����Ƿ��������ڵȴ�����
 Input        :  mailbox: ����ID
 Return       :  __TURE : �������ڵȴ�����
                 __FALSE: û�������ڵȴ�����
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
 Description  :  UART0����/�����жϷ���
                 ���ն������ݸ�ʽ˵����
                     bit31<-bit28   bit27<-bit24    bit23<-bit16    bit15<-bit8    bit7<-bit0
                      ��Ч�ֽ���       �˿ں�           ����2          ����1          ����0
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

    while(((iir = U0IIR) & 0x01) == 0)          // �������жϴ�����
    {
        switch(iir & 0x0e)      // �ж��ж�����
        {
        case (2 << 1):          // ���ǽ����ж�
            rbr = 0;
            for(i = 0; i < 3; i++)              // ��ȡ3�ֽ�
            {
                rbr <<= 8;
                rbr |= U0RBR;
            }
            rbr |= (COM0 | (3 << 4)) << 24;     // �˿ںţ���Ч�ֽ���
            isr_mbx_send(Mbx_UART_Data, (void*)rbr);        // �ŵ����ն���
            rbr = 0;
            for(i = 0; i < 3; i++)              // ��ȡ3�ֽ�
            {
                rbr <<= 8;
                rbr |= U0RBR;
            }
            rbr |= (COM0 | (3 << 4)) << 24;     // �˿ںţ���Ч�ֽ���
            isr_mbx_send(Mbx_UART_Data, (void*)rbr);        // �ŵ����ն���
            rbr = 0;
            for(i = 0; i < 2; i++)              // ��ȡ2�ֽڣ�һ���Զ�ȡ8�ֽ�
            {
                rbr <<= 8;
                rbr |= U0RBR;
            }
            rbr |= (COM0 | (2 << 4)) << 24;     // �˿ںţ���Ч�ֽ���
            isr_mbx_send(Mbx_UART_Data, (void*)rbr);        // �ŵ����ն���
            break;
        case (6 << 1):          // ���ǳ�ʱ�ж�
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
                rbr |= (COM0 | (i << 4)) << 24;             // ��Ч�ֽ������˿ں�
                isr_mbx_send(Mbx_UART_Data, (void*)rbr);    // �ŵ����ն���
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
                rbr |= (COM0 | (i << 4)) << 24;             // ��Ч�ֽ������˿ں�
                isr_mbx_send(Mbx_UART_Data, (void*)rbr);    // �ŵ����ն���
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
                rbr |= (COM0 | (i << 4)) << 24;             // ��Ч�ֽ������˿ں�
                isr_mbx_send(Mbx_UART_Data, (void*)rbr);    // �ŵ����ն���
            }
            break;
        case (1 << 1):          // ���Ƿ����ж�
            data_len = Status_Mechanism[0].Tx_Buf[0] + Status_Mechanism[0].Tx_Buf[1] * 256;       // Ҫ���͵��ֽ�����
            if(data_len > (1024 - 16))
            {
                data_len = 1024 - 16;
            }
            for(i = 0; i < 16; i++)                                 // ÿ��������16���ֽ�
            {
                if(Status_Mechanism[0].Data_Send_Num < data_len)    // �ж��Ƿ��ͽ���
                {
                    U0THR = Status_Mechanism[0].Tx_Buf[Status_Mechanism[0].Data_Send_Num + DATA_OFFSET];
                    Status_Mechanism[0].Data_Send_Num++;            // �����ַ���ָ��ָ����һ���ַ�
                }
                else
                {
                    U0IER &= ~0x02;                                 // �رշ����жϣ���ʱFIFO�е��������ڷ���
                    UART_send_push(0);                              // �ƶ�״̬�����Խ�����ɷ��ʹ���
                    free_mem_block(Status_Mechanism[0].Tx_Buf);     // �ͷ��ڴ��
                    Status_Mechanism[0].Tx_Buf = NULL;
                    break;
                }
            }
            break;
         default:
            U0IER &= ~0x03;         // ���û���õ����ж�ʹ��
            U0IER |= 0x01;          // ʹ�ܽ����ж�
            break;
        }
    }
//azh 110516
//    VICVectAddr = 0x00;
}

/******************************************************************************
 Function name:  void __irq UART1_IRQ_handler (void)
 Author       :  snap.gao
 Description  :  UART1����/�����жϷ���
 Input        :  None
 Return       :  None
 *****************************************************************************/
//void __irq UART1_IRQ_handler (void)
void UART1_IRQHandler (void)
{
    U32 iir,rbr;
    U16 data_len;
    U8  i;

    while(((iir = U1IIR) & 0x01) == 0)          // �������жϴ�����
    {
        switch(iir & 0x0e)      // �ж��ж�����
        {
        case ( 2<< 1):          // ���ǽ����ж�
            rbr = 0;
            for(i = 0; i < 3; i++)              // ��ȡ3�ֽ�
            {
                rbr <<= 8;
                rbr |= U1RBR;
            }
            rbr |= (COM1 | (3 << 4)) << 24;     // �˿ںţ���Ч�ֽ���
            isr_mbx_send(Mbx_UART_Data, (void*)rbr);        // �ŵ����ն���
            rbr = 0;
            for(i = 0; i < 3; i++)              // ��ȡ3�ֽ�
            {
                rbr <<= 8;
                rbr |= U1RBR;
            }
            rbr |= (COM1 | (3 << 4)) << 24;     // �˿ںţ���Ч�ֽ���
            isr_mbx_send(Mbx_UART_Data, (void*)rbr);        // �ŵ����ն���
            rbr = 0;
            for(i = 0; i < 2; i++)              // ��ȡ2�ֽڣ�һ���Զ�ȡ8�ֽ�
            {
                rbr <<= 8;
                rbr |= U1RBR;
            }
            rbr |= (COM1 | (2 << 4)) << 24;     // �˿ںţ���Ч�ֽ���
            isr_mbx_send(Mbx_UART_Data, (void*)rbr);        // �ŵ����ն���

            break;
        case (6 << 1):          // ���ǳ�ʱ�ж�
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
                rbr |= (COM1 | (i << 4)) << 24;             // ��Ч�ֽ������˿ں�
                isr_mbx_send(Mbx_UART_Data, (void*)rbr);    // �ŵ����ն���
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
                rbr |= (COM1 | (i << 4)) << 24;             // ��Ч�ֽ������˿ں�
                isr_mbx_send(Mbx_UART_Data, (void*)rbr);    // �ŵ����ն���
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
                rbr |= (COM1 | (i << 4)) << 24;             // ��Ч�ֽ������˿ں�
                isr_mbx_send(Mbx_UART_Data, (void*)rbr);    // �ŵ����ն���
            }
            break;
        case (1 << 1):          // ���Ƿ����ж�
#ifdef ZIGBEE_TEST
            if(Status_Mechanism[1].Index < ZIGBEE_SEND_MEM_NUM)
            {
                data_len = Zigbee_Send_Status[Status_Mechanism[1].Index].Len;// Ҫ���͵��ֽ�����
                if(data_len > (1024 - 16))
                {
                    data_len = 1024 - 16;
                }
                for(i = 0; i < 16; i++)                                 // ÿ��������16���ֽ�
                {
                    if(Status_Mechanism[1].Data_Send_Num < data_len)    // �ж��Ƿ��ͽ���
                    {
                        U1THR = Zigbee_Send_Status[Status_Mechanism[1].Index].Buf[Status_Mechanism[1].Data_Send_Num];
                        Status_Mechanism[1].Data_Send_Num++;            // �����ַ���ָ��ָ����һ���ַ�
                    }
                    else
                    {
                        U1IER &= ~0x02;                                 // �رշ����жϣ���ʱFIFO�е��������ڷ���
                        UART_send_push(1);                              // �ƶ�״̬�����Խ�����ɷ��ʹ���
                        if((Zigbee_Send_Status[Status_Mechanism[1].Index].Flag & WAIT_RESEND) == 0)
                        {
                            free_zigbee_send_mem(Status_Mechanism[1].Index);        //�Ƿ���ģ��������Ҫ�ȴ��ط����ͷ��ڴ��
                        }
                        Status_Mechanism[1].Index = 0xff;
                        Status_Mechanism[1].Tx_Buf = NULL;
                        break;
                    }
                }
            }
#else
            data_len = Status_Mechanism[1].Tx_Buf[0] + Status_Mechanism[1].Tx_Buf[1] * 256;       // Ҫ���͵��ֽ�����
            if(data_len > (1024 - 16))
            {
                data_len = 1024 - 16;
            }
            for(i = 0; i < 16; i++)                                 // ÿ��������16���ֽ�
            {
                if(Status_Mechanism[1].Data_Send_Num < data_len)    // �ж��Ƿ��ͽ���
                {
                    U1THR = Status_Mechanism[1].Tx_Buf[Status_Mechanism[1].Data_Send_Num + DATA_OFFSET];
                    Status_Mechanism[1].Data_Send_Num++;            // �����ַ���ָ��ָ����һ���ַ�
                }
                else
                {
                    U1IER &= ~0x02;                                 // �رշ����жϣ���ʱFIFO�е��������ڷ���
                    UART_send_push(1);                              // �ƶ�״̬�����Խ�����ɷ��ʹ���
                    free_mem_block(Status_Mechanism[1].Tx_Buf);     // �ͷ��ڴ��
                    Status_Mechanism[1].Tx_Buf = NULL;
                    break;
                }
            }
#endif//ZIGBEE_TEST
            break;
         default:
            U1IER &= ~0x03;         // ���û���õ����ж�ʹ��
            U1IER |= 0x01;          // ʹ�ܽ����ж�
            break;
        }
    }
//azh 110516
//    VICVectAddr = 0x00;
}

/******************************************************************************
 Function name:  void __irq UART2_IRQ_handler (void)
 Author       :  snap.gao
 Description  :  UART2����/�����жϷ���
 Input        :  None
 Return       :  None
 *****************************************************************************/
//void __irq UART2_IRQ_handler (void)
void UART2_IRQHandler (void)
{
    U32 iir,rbr;
    U16 data_len;
    U8  i;

    while(((iir = U2IIR) & 0x01) == 0)          // �������жϴ�����
    {
        switch(iir & 0x0e)      // �ж��ж�����
        {
        case (2 << 1):          // ���ǽ����ж�
            rbr = 0;
            for(i = 0; i < 3; i++)              // ��ȡ3�ֽ�
            {
                rbr <<= 8;
                rbr |= U2RBR;
            }
            rbr |= (COM2 | (3 << 4)) << 24;     // �˿ںţ���Ч�ֽ���
            isr_mbx_send(Mbx_UART_Data, (void*)rbr);        // �ŵ����ն���
            rbr = 0;
            for(i = 0; i < 3; i++)              // ��ȡ3�ֽ�
            {
                rbr <<= 8;
                rbr |= U2RBR;
            }
            rbr |= (COM2 | (3 << 4)) << 24;     // �˿ںţ���Ч�ֽ���
            isr_mbx_send(Mbx_UART_Data, (void*)rbr);        // �ŵ����ն���
            rbr = 0;
            for(i = 0; i < 2; i++)              // ��ȡ2�ֽڣ�һ���Զ�ȡ8�ֽ�
            {
                rbr <<= 8;
                rbr |= U2RBR;
            }
            rbr |= (COM2 | (2 << 4)) << 24;     // �˿ںţ���Ч�ֽ���
            isr_mbx_send(Mbx_UART_Data, (void*)rbr);        // �ŵ����ն���
            break;
        case (6 << 1):          // ���ǳ�ʱ�ж�
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
                rbr |= (COM2 | (i << 4)) << 24;             // ��Ч�ֽ������˿ں�
                isr_mbx_send(Mbx_UART_Data, (void*)rbr);    // �ŵ����ն���
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
                rbr |= (COM2 | (i << 4)) << 24;             // ��Ч�ֽ������˿ں�
                isr_mbx_send(Mbx_UART_Data, (void*)rbr);    // �ŵ����ն���
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
                rbr |= (COM2 | (i << 4)) << 24;             // ��Ч�ֽ������˿ں�
                isr_mbx_send(Mbx_UART_Data, (void*)rbr);    // �ŵ����ն���
            }
            break;
        case (1 << 1):          // ���Ƿ����ж�
            data_len = Status_Mechanism[2].Tx_Buf[0] + Status_Mechanism[2].Tx_Buf[1] * 256;       // Ҫ���͵��ֽ�����
            if(data_len > (1024 - 16))
            {
                data_len = 1024 - 16;
            }
            for(i = 0; i < 16; i++)                                 // ÿ��������16���ֽ�
            {
                if(Status_Mechanism[2].Data_Send_Num < data_len)    // �ж��Ƿ��ͽ���
                {
                    U2THR = Status_Mechanism[2].Tx_Buf[Status_Mechanism[2].Data_Send_Num + DATA_OFFSET];
                    Status_Mechanism[2].Data_Send_Num++;            // �����ַ���ָ��ָ����һ���ַ�
                }
                else
                {
                    U2IER &= ~0x02;                                 // �رշ����жϣ���ʱFIFO�е��������ڷ���
                    UART_send_push(2);                              // �ƶ�״̬�����Խ�����ɷ��ʹ���
                    free_mem_block(Status_Mechanism[2].Tx_Buf);     // �ͷ��ڴ��
                    Status_Mechanism[2].Tx_Buf = NULL;
                    break;
                }
            }
            break;
         default:
            U2IER &= ~0x03;         // ���û���õ����ж�ʹ��
            U2IER |= 0x01;          // ʹ�ܽ����ж�
            break;
        }
    }
//azh 110516
//    VICVectAddr = 0x00;
}

/******************************************************************************
 Function name:  void __irq UART3_IRQ_handler (void)
 Author       :  snap.gao
 Description  :  UART3����/�����жϷ���
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

    while(((iir = U3IIR) & 0x01) == 0)          // �������жϴ�����
    {
        switch(iir & 0x0e)      // �ж��ж�����
        {
        case (2 << 1):          // ���ǽ����ж�
            for (i = 0; i < 8; i++)
            {
                GucRcvBuf[i] = U3RBR;
            }
            SCIBufferWrite(&stAT_RX_BUFFER ,GucRcvBuf, 8);
            stReceiveBuffer.usSilenceCnt = CON_AT_SILENCE_MAX;
            stReceiveBuffer.ucIsGetVaild = 1;
            break;
        case (6 << 1):          // ���ǳ�ʱ�ж�
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
            for(i = 0; i < 3; i++)              // ��ȡ3�ֽ�
            {
                rbr <<= 8;
                rbr |= U3RBR;
            }
            rbr |= (COM3 | (3 << 4)) << 24;     // �˿ںţ���Ч�ֽ���
            isr_mbx_send(Mbx_UART_Data, (void*)rbr);        // �ŵ����ն���
            rbr = 0;
            for(i = 0; i < 3; i++)              // ��ȡ3�ֽ�
            {
                rbr <<= 8;
                rbr |= U3RBR;
            }
            rbr |= (COM3 | (3 << 4)) << 24;     // �˿ںţ���Ч�ֽ���
            isr_mbx_send(Mbx_UART_Data, (void*)rbr);        // �ŵ����ն���
            rbr = 0;
            for(i = 0; i < 2; i++)              // ��ȡ2�ֽڣ�һ���Զ�ȡ8�ֽ�
            {
                rbr <<= 8;
                rbr |= U3RBR;
            }
            rbr |= (COM3 | (2 << 4)) << 24;     // �˿ںţ���Ч�ֽ���
            isr_mbx_send(Mbx_UART_Data, (void*)rbr);        // �ŵ����ն���
            break;
        case (6 << 1):          // ���ǳ�ʱ�ж�
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
                rbr |= (COM3 | (i << 4)) << 24;             // ��Ч�ֽ������˿ں�
                isr_mbx_send(Mbx_UART_Data, (void*)rbr);    // �ŵ����ն���
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
                rbr |= (COM3 | (i << 4)) << 24;             // ��Ч�ֽ������˿ں�
                isr_mbx_send(Mbx_UART_Data, (void*)rbr);    // �ŵ����ն���
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
                rbr |= (COM3 | (i << 4)) << 24;             // ��Ч�ֽ������˿ں�
                isr_mbx_send(Mbx_UART_Data, (void*)rbr);    // �ŵ����ն���
            }
            break;
*/
        case (1 << 1):          // ���Ƿ����ж�
            data_len = Status_Mechanism[3].Tx_Buf[0] + Status_Mechanism[3].Tx_Buf[1] * 256;       // Ҫ���͵��ֽ�����
            if(data_len > (1024 - 16))
            {
                data_len = 1024 - 16;
            }
            for(i = 0; i < 16; i++)                                 // ÿ��������16���ֽ�
            {
                if(Status_Mechanism[3].Data_Send_Num < data_len)    // �ж��Ƿ��ͽ���
                {
                    U3THR = Status_Mechanism[3].Tx_Buf[Status_Mechanism[3].Data_Send_Num + DATA_OFFSET];
                    Status_Mechanism[3].Data_Send_Num++;            // �����ַ���ָ��ָ����һ���ַ�
                }
                else
                {
                    U3IER &= ~0x02;                                 // �رշ����жϣ���ʱFIFO�е��������ڷ���
                    UART_send_push(3);                              // �ƶ�״̬�����Խ�����ɷ��ʹ���
                    free_mem_block(Status_Mechanism[3].Tx_Buf);     // �ͷ��ڴ��
                    Status_Mechanism[3].Tx_Buf = NULL;
                    break;
                }
            }
            break;
         default:
            U3IER &= ~0x03;         // ���û���õ����ж�ʹ��
            U3IER |= 0x01;          // ʹ�ܽ����ж�
            break;
        }
    }
//azh 110516
//    VICVectAddr = 0x00;
}

#ifdef ZIGBEE_TEST
/******************************************************************************
** ��������: U8 query_zigbee_send_event(void)
** ��������: ��ѯ���ڵȴ����͵��ڴ�飬�������ȷ�������������ڴ��
** ��ڲ���: ��
** �� �� ֵ: �ҵ��ȴ������¼�ʱ�����ڴ��������±�
**           �Ҳ���ʱ����0xff
** ˵    ��: ����������ȴ����ͱ�־
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
** ��������: void init_zigbee_mem(void)
** ��������: ��ʼ��ͨѶ�ڴ��
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
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
** ��������: void deal_with_zigbee_send(U8 *mem_ptr)
** ��������: ����ZigBee����FIFO����Ч���ݵĳ���
** ��ڲ���: start_ptr: ��ǰFIFO����ָ��
**           end_ptr  : ��ǰFIFO����ָ��
** �� �� ֵ: ��Ч���ݵĳ���
** ˵    ��: FIFO��û����Ч����ʱstart_ptr�ܱ�end_ptr��1��
**           start_ptr����end_ptr�ҳ�����1��ʱΪFIFO��������
**           start_ptr����end_ptrʱΪFIFO����
**           start_ptrС��end_ptrʱΪstart_ptr�Ѿ���ת����end_ptr��û�з�ת��FIFO��������
******************************************************************************/
S32 deal_with_zigbee_send (U8 *mem_ptr)
{
    U8 i, total_package, send_mem;
    U16 package_len, len;

    //debug
    //memset(&mem_ptr[ZIGBEE_RET_ADDR_OFFSET], 0xff, 8);

    //if(check_mac_addr(Comm_Mem[index].Aim_Addr) == 1)       //���Ŀ���ַ�Ƿ��ǹ㲥��ַ
    if(check_broadcast_addr(&mem_ptr[ZIGBEE_RET_ADDR_OFFSET]) == __TRUE)//���Ŀ���ַ�Ƿ��ǹ㲥��ַ
    {
        package_len = BROADCAST_ZIGBEE_FRAME_LEN;
    }
    else
    {
        package_len = UNICAST_ZIGBEE_FRAME_LEN;
    }

    //len = Comm_Mem[index].Len;
    len = mem_ptr[DATA_LEN_OFFSET] + (mem_ptr[DATA_LEN_OFFSET+1]<<8);
    total_package = len / package_len;          //�ְ�����ܰ���
    if(len > package_len * total_package)       //������ʱ���ܰ�����Ҫ��1
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

        Zigbee_Send_Status[send_mem].Buf[17] = ZIGBEE_CHANNEL;      //����ͨ����
        Zigbee_Send_Status[send_mem].Buf[18] = Frame_Serial_Num << 2;   //֡���
        Zigbee_Send_Status[send_mem].Buf[19] = total_package;       //�ܰ���
        Zigbee_Send_Status[send_mem].Buf[20] = i + 1;               //�ְ���
        if(i != total_package - 1)          //����Ƿ����һ��
        {
            Zigbee_Send_Status[send_mem].Len = package_len + 24;    //�ְ����ܳ���
            len -= package_len;             //��ȥ�ѷ��͵����ݳ���
            Zigbee_Send_Status[send_mem].Buf[21] = package_len;     //�����򳤶�
            //memcpy(&Zigbee_Send_Status[send_mem].Buf[22], &Comm_Mem[index].Buf[package_len * i], package_len);
            memcpy(&Zigbee_Send_Status[send_mem].Buf[22], &mem_ptr[DATA_OFFSET + package_len * i], package_len);
            Zigbee_Send_Status[send_mem].Buf[package_len + 22] = cal_frmcs(Zigbee_Send_Status[send_mem].Buf, package_len + 22);
            Zigbee_Send_Status[send_mem].Buf[package_len + 23] = '#';
        }
        else
        {
            Zigbee_Send_Status[send_mem].Len = len + 24;            //�ְ����ܳ���
            Zigbee_Send_Status[send_mem].Buf[21] = len;             //�����򳤶�
            //memcpy(&Zigbee_Send_Status[send_mem].Buf[22], &Comm_Mem[index].Buf[package_len * i], len);
            memcpy(&Zigbee_Send_Status[send_mem].Buf[22], &mem_ptr[DATA_OFFSET + package_len * i], len);
            Zigbee_Send_Status[send_mem].Buf[len + 22] = cal_frmcs(Zigbee_Send_Status[send_mem].Buf, len + 22);
            Zigbee_Send_Status[send_mem].Buf[len + 23] = '#';
        }

        if(package_len == UNICAST_ZIGBEE_FRAME_LEN)
        {
            Zigbee_Send_Status[send_mem].Flag |= WAIT_RESEND;       //����֡��Ҫ�ط�
        }
        Zigbee_Send_Status[send_mem].Flag |= WAIT_SEND;             //����ͨѶ״̬������

        Frame_Serial_Num++;
    }

//    Comm_Mem[index].Flag = 0;               //�ͷ��ڴ��
    free_mem_block(mem_ptr);
    return __SUCCEED;//azh ���سɹ� �ɽ�������ȥ�ͷ�
}

/******************************************************************************
** ��������: void deal_with_zigbee_rec(U8 **ret_ptr)
** ��������: ����ZigBee����FIFO����Ч���ݵĳ���
** ��ڲ���: start_ptr: ��ǰFIFO����ָ��
**           end_ptr  : ��ǰFIFO����ָ��
** �� �� ֵ: ��Ч���ݵĳ���
** ˵    ��: FIFO��û����Ч����ʱstart_ptr�ܱ�end_ptr��1��
**           start_ptr����end_ptr�ҳ�����1��ʱΪFIFO��������
**           start_ptr����end_ptrʱΪFIFO����
**           start_ptrС��end_ptrʱΪstart_ptr�Ѿ���ת����end_ptr��û�з�ת��FIFO��������
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

    while((data_len = get_fifo_vaild_data_len(Start_Ptr, End_Ptr)) >= 24)       //���FIFO�е���Ч���ݳ����Ƿ����һ����СZigBee֡��
    {
        if(Zigbee_Rec_Fifo[increase_end_ptr(End_Ptr, 1)] != '*')    //����֡��ʼ����*��
        {
            End_Ptr = increase_end_ptr(End_Ptr, 1);                 //����ָ��ӡ�1��
            continue;
        }
        //buf = Comm_Mem[mem_tmp].Buf;
        copy_zigbee_rec_fifo_data(buf, increase_end_ptr(End_Ptr, 1), 22);       //��FIFO�е����ݿ�����������

        len = buf[21];                                              //ȡ��֡����
        if(len > MAX_ZIGBEE_FRAME_LEN)                              //���֡����
        {
            End_Ptr = increase_end_ptr(End_Ptr, 1);                 //����ָ��ӡ�1��
            continue;
        }

        if(data_len < len + 24)                                     //���FIFO����Ч�����Ƿ��㹻
        {
            break;                                                  //��������ֱ�����ȹ�
        }
        copy_zigbee_rec_fifo_data(buf + 22, increase_end_ptr(End_Ptr, 23), len + (24 - 22));    //��FIFO�е����ݿ�����������

        if(buf[len + 23] != '#')                                    //����֡��������#��
        {
            End_Ptr = increase_end_ptr(End_Ptr, 1);                 //����ָ��ӡ�1��
            continue;
        }

        cs = buf[len + 22];                                         //ȡ��֡������
        if(cs != cal_frmcs(buf, len + 22))
        {
            End_Ptr = increase_end_ptr(End_Ptr, 1);                 //����ָ��ӡ�1��
            continue;
        }

        //if(check_mac_addr(&buf[9]) == 0)                            //���Դ��ַ�Ƿ���ģ�鷢����
        if(check_local_addr(&buf[9]) == __TRUE)                     //���Դ��ַ�Ƿ���ģ�鷢����
        {
            zigbee_local_frame_analyse(buf);                        //ZigBee�ײ�ά���������
            End_Ptr = increase_end_ptr(End_Ptr, len + 24);          //����ָ�����һ��֡��
            continue;
        }

//        if(check_dest_addr(&buf[1], gSource_Addr) == 0)              //���Ŀ���ַ�Ƿ��Ƿ����Լ���
//        {
//            End_Ptr = increase_end_ptr(End_Ptr, len + 24);          //����ָ�����һ��֡��
//            continue;
//        }

        if(buf[19] > MAX_PACKAGE_NUM)                               //����ܰ���
        {
            End_Ptr = increase_end_ptr(End_Ptr, len + 24);          //����ָ�����һ��֡��
            continue;
        }

        if(buf[20] > buf[19])                                       //���ְ���
        {
            End_Ptr = increase_end_ptr(End_Ptr, len + 24);          //����ָ�����һ��֡��
            continue;
        }
//azh ����ʽ��ѹ������Ҫ֧�ֱ��������й�Լ������Ҫ�����ǳ��ص����ݣ������յ����ݵ�ģ��MAC��ַ�������ⲿ�ִ��뱣�浽��ʱ��������ݽڵ�ȥ����
//        if(gucGetZigbeeAddrFlag == 1)//��־��Ҫͨ��zigbee�㲥��ַ ��ȡzigbee��ַ������
//        {
//            store_zigbee_addr(&buf[9]);
//            gucGetZigbeeAddrFlag = 0;
//        }

        if(zigbee_package_assemble(buf, (void*)ret_ptr) < 0)                        //��ʼ���
        {
            break;      //�ڴ�鲻������ȴ�
        }
        End_Ptr = increase_end_ptr(End_Ptr, len + 24);              //����ָ�����һ��֡��
    }

    free_mem_block(buf);//Mem_256_Byte
}

/******************************************************************************
** ��������: S8 zigbee_local_frame_analyse(U8 *buf)
** ��������: ����ZigBeeģ����������ı���ά��ͨѶ����
** ��ڲ���: buf: ZigBeeģ���Ӧ֡������ʼ��ַ
** �� �� ֵ: �����ɹ�0��֡��ʽ����-1������ʶ���֡-2��ִ�д����֡-3
** ˵    ��: ��
******************************************************************************/
S8 zigbee_local_frame_analyse (U8 *buf)
{
    if(buf[21] == 0)
    {
        if('A' == buf[19])                              //buf[20]Ϊ�ź�ǿ��
        {
            set_normal_run_flag(1 << 0);                    //������Ч��ʶ���֮һzigbeeͨ������ȷ��
            if(__FAIL == release_zigbee_send_mem(buf[18]))
            {
                return -3;                              //ִ�д���
            }
        }
        else if('E' == buf[19])                         //buf[20]Ϊ���3����б�������/4
        {
            if(__FAIL == resend_zigbee_package(buf[18]))
            {
                return -3;                              //ִ�д���
            }
        }
        else
        {
            return -2;                                  //����ʶ���֡
        }
    }
    else
    {
        if(buf[19] != 1)                                //����ܰ���
        {
            return -1;                                  //֡��ʽ����
        }

        if(buf[20] != 1)                                //���ְ���
        {
            return -1;                                  //֡��ʽ����
        }

        switch(buf[21])
        {
        case 12:
            if((buf[22] == 'M') && (buf[23] == 'A') && (buf[24] == 'C') && (buf[25] == '='))
            {
                memcpy(gSource_Addr, &buf[26], 8);       //����ZigBeeģ���MAC��ַ
            }
            else
            {
                return -2;                              //����ʶ���֡
            }
            break;
        default:
            if((buf[24] == 'R') && (buf[25] == 'E') && (buf[26] == 'S') && (buf[27] == 'E') && (buf[28] == 'T'))
            {
                memcpy(Zigbee_Reset_Reason, &buf[22], buf[21]);     //����ZigBeeģ��ĸ�λԭ��
                Zigbee_Reset_Reason[buf[21]] = '\0';                //�ԡ�0�������ַ���
//                if(__FAIL == read_zigbee_mac_addr())    //������ȡZigBeeģ��MAC��ַ
//                {
//                    return -3;      //ִ�д���
//                }
            }
            else
            {
                return -2;                              //����ʶ���֡
            }
            break;
        }
    }
    return 0;                                           //��ȷ������ִ��
}

/******************************************************************************
** ��������: S8 zigbee_package_assemble(U8 *buf, U8 **ret_ptr)
** ��������: ƴװ�յ���ZigBee�ְ����ݣ�����ְ�ȫ���յ����򽻸�Ĭ�Ͻ�������
** ��ڲ���: buf: ZigBee�ְ�������ʼ��ַ
** �� �� ֵ: ����ɹ�0��֡��ʽ����1���ظ���֡2���ڴ�����벻��-1
** ˵    ��: �� ȫ�ֱ��� gSource_Addr
******************************************************************************/
S8 zigbee_package_assemble (U8 *buf, U8 **ret_ptr)
{
    U8 mem_type;
    U8  serial, i, j, *mem_ptr, package_len;
    U16 *len_ptr;

    if(__FALSE == check_broadcast_addr(buf + 1))        //���ǹ㲥֡
    {
        if(__FALSE != check_duplicate_frame(buf + 9, buf[18] >> 2))
        {
            return 2;                                   //�ظ�֡
        }
        if(buf[19] & 0x80)                              //��ǩ������֡, ����Ҫ
        {
            return 1;
        }
        package_len = UNICAST_ZIGBEE_FRAME_LEN;         //����֡����
    }
    else                                                //�㲥֡
    {
        j = record_frame_serial(buf + 9, 0xff);         //��¼Դ��ַ, ֡��ź���
        memset(Zigbee_Duplicate_Frame[j].Frame_Serial, 0xff, 8);//����ʷ֡�����Ч
        package_len = BROADCAST_ZIGBEE_FRAME_LEN;
    }

    serial = ((buf[18] >> 2) - (buf[20] & 0x7f)) & 0x3f;//����֡ʶ���, ֡��ż��ְ���, ȥ��·��֡��ʶ
    for(i = 0; i < ZIGBEE_REC_MEM_NUM; i++)
    {
        if(Zigbee_Rec_Status[i].Mem_Ptr != NULL)        //����Ƿ���ʹ����
        {
            if(comp_zigbee_addr(buf + 9, &Zigbee_Rec_Status[i].Mem_Ptr[ZIGBEE_RET_ADDR_OFFSET]) != __FALSE)
            {
                break;
            }
        }
    }

    if(i >= ZIGBEE_REC_MEM_NUM)                         //����ǵ�һ�ν���
    {
        //mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR6);            //����һ���ڴ�����ڱ�������
//azh 1209011
        mem_ptr = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR6);            //����һ���ڴ�����ڱ�������
        if(mem_ptr == NULL)
        {
            setmemgetnullid(GET_MEM_ERR6, gusCurMemErrFunName);
            return -1;                                  //�ڴ�����벻��
        }
        i = alloc_zigbee_rec_struct();                  //����һ���µĽ���״̬�ṹ��

        format_zigbee_rec_buf (ZIGBEE_CHL, mem_ptr);    //��ʼ��֡ͷ��Ϣ
        Zigbee_Rec_Status[i].Mem_Ptr = mem_ptr;         //�����ڴ����
        Zigbee_Rec_Status[i].Total_Package = buf[19] & 0x7f;        //�����ܰ���, Ϊ�Ժ����
        Zigbee_Rec_Status[i].Frame_Serial = serial;                 //����֡ʶ���
        memcpy(&mem_ptr[ZIGBEE_RET_ADDR_OFFSET], &buf[9], 8);       //������յ���֡��Դ��ַ��Ҳ����Ӧ��ʱ��Ŀ���ַ
                                                        //azh �����Լ�ģ��ĵ�ַ �����Լ�ģ���ַӦ�ò�ת�����ն� ��Ȼ������
        if(__FALSE == check_broadcast_addr(buf + 1))    //�ų��㲥֡
        {
            memcpy(gSource_Addr, &buf[1], 8);           //����ZigBeeģ���MAC��ַ
        }
    }
    else
    {
        if((buf[19] & 0x7f) != Zigbee_Rec_Status[i].Total_Package)
        {
            return 1;                                   //֡��ʽ����
        }
        mem_ptr = Zigbee_Rec_Status[i].Mem_Ptr;
    }

    if((buf[20] & 0x7f) != (buf[19] & 0x7f))            //����ǲ������ķְ� ��Ӧ�����������������
    {
        if(package_len != buf[21])
        {
            return 1;                                   //֡��ʽ����
        }
    }

    memcpy(&mem_ptr[DATA_OFFSET + package_len * ((buf[20] & 0x7f) - 1)], &buf[22], buf[21]);    //����ְ�����
    len_ptr = (U16*)&mem_ptr[DATA_LEN_OFFSET];          //�ڴ氲����ȷ��˫�ֽڶ���
    *len_ptr += buf[21];

    Zigbee_Rec_Status[i].Package_Flag |= 1 << ((buf[20] & 0x7f) - 1);
    record_frame_serial(buf + 9, buf[18] >> 2);         //��¼֡���, ֡����ӵ��ְ�ʱ��...

    for(j = 0; j < Zigbee_Rec_Status[i].Total_Package; j++)
    {
        if((Zigbee_Rec_Status[i].Package_Flag & (1 << j)) == 0)
        {
            break;
        }
    }

    if(j >= Zigbee_Rec_Status[i].Total_Package)         //������зְ��Ƿ��յ�
    {
        *ret_ptr = mem_ptr;
        Zigbee_Rec_Status[i].Mem_Ptr = NULL;            //�ͷŽ���״̬�ṹ��
    }

    return 0;
}

/******************************************************************************
** ��������: void copy_zigbee_rec_fifo_data (U8 *buf, U16 offset, U8 len)
** ��������: ��ZigBee����FIFO�п������ݣ�����FIFO�ķ�ת
** ��ڲ���: buf   : �������ݵĻ���
**           offset��ZigBee����FIFO��ƫ��
**           len   ���������ݵĳ���
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void copy_zigbee_rec_fifo_data (U8 *buf, U16 offset, U8 len)
{
    U16 tmp;

    if(offset + len >= ZIGBEE_REC_FIFO_LEN)
    {
        tmp = ZIGBEE_REC_FIFO_LEN - offset;                 //���㿽����FIFOĩβ�����ݸ���
        memcpy(buf, &Zigbee_Rec_Fifo[offset], tmp);         //�ȿ�����FIFOĩβ
        memcpy(buf + tmp, Zigbee_Rec_Fifo, len - tmp);      //��FIFOͷ����ʼ����ʣ�������
    }
    else
    {
        memcpy(buf, &Zigbee_Rec_Fifo[offset], len);
    }
}

/******************************************************************************
** ��������: U8 check_dest_addr (U8 *buf1, U8 *buf2)
** ��������: �Ƚ�2��ZigBee��ַ��������һ����ַbuf1�Ƿ�㲥��ַ
** ��ڲ���: buf1: ��ַ1����
**           buf2: ��ַ2����
** �� �� ֵ: ��ַƥ�䷵��1���㲥��ַ����2�����򷵻�0
** ˵    ��: ��
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
        return 1;                 //��ַƥ��
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
        return 2;                 //��ַȫ��0xff��
    }

    return 0;
}

/******************************************************************************
** ��������: U16 get_fifo_vaild_data_len(U16 start_ptr, U16 end_ptr)
** ��������: ����ZigBee����FIFO����Ч���ݵĳ���
** ��ڲ���: start_ptr: ��ǰFIFO����ָ��
**           end_ptr  : ��ǰFIFO����ָ��
** �� �� ֵ: ��Ч���ݵĳ���
** ˵    ��: FIFO��û����Ч����ʱstart_ptr�ܱ�end_ptr��1��
**           start_ptr����end_ptr�ҳ�����1��ʱΪFIFO��������
**           start_ptr����end_ptrʱΪFIFO����
**           start_ptrС��end_ptrʱΪstart_ptr�Ѿ���ת����end_ptr��û�з�ת��FIFO��������
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
** ��������: U16 increase_end_ptr(U16 end_ptr, U16 num)
** ��������: ZigBee��ǰFIFO����ָ������ƫ�ƣ�����ָ�뷭ת
** ��ڲ���: num    : Ҫ���ӵĸ���
**           end_ptr: ��ǰFIFO����ָ��
** �� �� ֵ: ����ƫ�ƺ�ĵ�ǰFIFO����ָ�룬���ܷ�ת
** ˵    ��: ��
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
** ��������: U16 increase_start_ptr(U16 start_ptr, U16 end_ptr)
** ��������: ZigBee����FIFOָ��ӡ�1�����������ָ���Ѿ���������ָ���򲻼ӡ�1������ԭֵ
** ��ڲ���: start_ptr: ��ǰFIFO����ָ��
**           end_ptr  : ��ǰFIFO����ָ��
** �� �� ֵ: ����ָ��ӡ�1�����п��ܲ��ӻ�ת
** ˵    ��: ��
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
** ��������: U8 alloc_zigbee_rec_struct (void)
** ��������: ����ZigBee����״̬�ṹ��
**           ����������нṹ�壬���û�п��нṹ������������ʹ�õĽṹ��
**           ������ʼ���ṹ�����ز���
** ��ڲ���: ��
** �� �� ֵ: �����ڴ��������±꣬��Ȼ������һ��
** ˵    ��: ��
******************************************************************************/
U8 alloc_zigbee_rec_struct (void)
{
    U8 i,j;
    U32 tick_tmp,sub;

    tick_tmp = 0;
    j = 0xff;
    for(i = 0; i < ZIGBEE_REC_MEM_NUM; i++)
    {
        //if(Zigbee_Rec_Status[i].Mem_Index == 0xff)      //����Ƿ����
        if(Zigbee_Rec_Status[i].Mem_Ptr == NULL)      //����Ƿ����
        {
            Zigbee_Rec_Status[i].Tick = OS_Time;//Sys_Tick;       //��������ʱ�̵�ϵͳTick
            Zigbee_Rec_Status[i].Package_Flag = 0;      //�ְ����ձ�־λ��ʼ��
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
    free_mem_block(Zigbee_Rec_Status[j].Mem_Ptr);          //ǿ���ͷ�
    Zigbee_Rec_Status[j].Mem_Ptr = NULL;

    //Zigbee_Rec_Status[j].Tick = Sys_Tick;               //��������ʱ�̵�ϵͳTick
    Zigbee_Rec_Status[j].Tick = OS_Time;               //��������ʱ�̵�ϵͳTick
    Zigbee_Rec_Status[j].Package_Flag = 0;              //�ְ����ձ�־λ��ʼ��
    return j;
}

/******************************************************************************
** ��������: U8 alloc_zigbee_send_mem(void)
** ��������: ����ZigBee�����ڴ�飬
**           ������������ڴ�飬���û�п����ڴ������������Ĳ������ڵȴ��ط����ڴ��
**           �������ɹ����ʼ���ڴ�����ز���
** ��ڲ���: ��
** �� �� ֵ: ��ȷ����ʱ�����ڴ��������±꣬���벻���ڴ�鷵��0xff
** ˵    ��: ��
******************************************************************************/
U8 alloc_zigbee_send_mem (void)
{
    U8 i,j;
    U32 tick_tmp,sub;

    tick_tmp = 0;
    j = 0xff;
    for(i = 0; i < ZIGBEE_SEND_MEM_NUM; i++)
    {
        if(Zigbee_Send_Status[i].Flag == 0)         //����Ƿ����
        {
            Zigbee_Send_Status[i].Len = 0;          //���ȳ�ʼ��
            Zigbee_Send_Status[i].Tick = OS_Time;//Sys_Tick;  //��������ʱ�̵�ϵͳTick
            Zigbee_Send_Status[i].Flag = IN_USE;    //����ڴ�鱻ռ��
            return i;
        }
        else if((Zigbee_Send_Status[i].Flag & (IN_USE | WAIT_RESEND)) == (IN_USE | WAIT_RESEND))    //����Ƿ��ڵȴ��ط�
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
        Zigbee_Send_Status[j].Len = 0;              //���ȳ�ʼ��
        Zigbee_Send_Status[j].Tick = OS_Time;//Sys_Tick;      //��������ʱ�̵�ϵͳTick
        Zigbee_Send_Status[j].Flag = IN_USE;        //����ڴ�鱻ռ��
    }
    return j;
}

/******************************************************************************
** ��������: S32 release_zigbee_send_mem(U8 frame_serial)
** ��������: ����֡����ͷ����ڵȴ��ط���ZigBee�ڴ��
** ��ڲ���: frame_serial��֡���
** �� �� ֵ: �ҵ�����֡��ŵ��ڴ�鲢�ͷŷ���__SUCCEED���Ҳ����ڴ�鷵��__FAIL
** ˵    ��: ��
******************************************************************************/
S32 release_zigbee_send_mem (U8 frame_serial)
{
    U8 i;

    for(i = 0; i < ZIGBEE_SEND_MEM_NUM; i++)
    {
        if((Zigbee_Send_Status[i].Flag & (IN_USE | WAIT_RESEND)) == (IN_USE | WAIT_RESEND))     //����Ƿ��ڵȴ��ط�
        {
            if(frame_serial == Zigbee_Send_Status[i].Buf[18])       //�ȶ�֡���
            {
                Zigbee_Send_Status[i].Flag = 0;
                return __SUCCEED;
            }
        }
    }
    return __FAIL;
}

/******************************************************************************
** ��������: S32 resend_zigbee_package(U8 frame_serial)
** ��������: ����֡����ٴη������ڵȴ��ط���ZigBee�ڴ��
** ��ڲ���: frame_serial��֡���
** �� �� ֵ: �ҵ�����֡��ŵ��ڴ�鲢�ط�����__SUCCEED���Ҳ����ڴ�鷵��__FAIL
** ˵    ��: ��
******************************************************************************/
S32 resend_zigbee_package (U8 frame_serial)
{
    U8 i;

    for(i = 0; i < ZIGBEE_SEND_MEM_NUM; i++)
    {
        if((Zigbee_Send_Status[i].Flag & (IN_USE | WAIT_RESEND)) == (IN_USE | WAIT_RESEND))     //����Ƿ��ڵȴ��ط�
        {
            if(frame_serial == Zigbee_Send_Status[i].Buf[18])       //�ȶ�֡���
            {
                Zigbee_Send_Status[i].Flag |= WAIT_SEND;            //�ٴη���
                return __SUCCEED;
            }
        }
    }
    return __FAIL;
}

/******************************************************************************
** ��������: S32 read_zigbee_mac_addr(void)
** ��������: ���Ͷ�����ZigBee�����ַ֡
** ��ڲ���: ��
** �� �� ֵ: ���ͳɹ��ͻ᷵��__SUCCEED�����򷵻�__FAIL
** ˵    ��: ��
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
    memset(&Zigbee_Send_Status[send_mem].Buf[1], 0, 8);         //Ŀ���ַȫ��0��
    memcpy(&Zigbee_Send_Status[send_mem].Buf[9], gSource_Addr, 8);
    Zigbee_Send_Status[send_mem].Buf[17] = ZIGBEE_CHANNEL;      //����ͨ����
    Zigbee_Send_Status[send_mem].Buf[18] = Frame_Serial_Num << 2;   //֡���
    Zigbee_Send_Status[send_mem].Buf[19] = 1;                   //�ܰ���
    Zigbee_Send_Status[send_mem].Buf[20] = 1;                   //�ְ���
    Zigbee_Send_Status[send_mem].Len = 5 + 24;                  //�ְ����ܳ���
    Zigbee_Send_Status[send_mem].Buf[21] = 5;                   //�����򳤶�

    Zigbee_Send_Status[send_mem].Buf[22] = 'M';                 //�����������
    Zigbee_Send_Status[send_mem].Buf[23] = 'A';                 //�����������
    Zigbee_Send_Status[send_mem].Buf[24] = 'C';                 //�����������
    Zigbee_Send_Status[send_mem].Buf[25] = '=';                 //�����������
    Zigbee_Send_Status[send_mem].Buf[26] = '?';                 //�����������

    Zigbee_Send_Status[send_mem].Buf[27] = cal_frmcs(Zigbee_Send_Status[send_mem].Buf, 27);
    Zigbee_Send_Status[send_mem].Buf[28] = '#';

    Zigbee_Send_Status[send_mem].Flag |= WAIT_SEND;             //����ͨѶ״̬�����ͣ�����Ҫ�ط�
    Frame_Serial_Num++;
    return __SUCCEED;
}
/******************************************************************************
** ��������: void check_zigbee_rec_mem(void)
** ��������: ZigBee�����ڴ�鳬ʱ��飬��ʱ��ǿ���ͷ��ڴ��
**           �������жϣ��������FIFO���п�������ж�ʹ��
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void check_zigbee_rec_mem (void)
{
    U8 i;

    for(i = 0; i < ZIGBEE_REC_MEM_NUM; i++)
    {
        if(Zigbee_Rec_Status[i].Mem_Ptr != NULL)                      //����Ƿ���ʹ����
        {
            //if((Sys_Tick - Zigbee_Rec_Status[i].Tick) > DLY_1S * 10)    //����Ƿ�ʱ��10��
            if((OS_Time - Zigbee_Rec_Status[i].Tick) > OS_DLY_1S * 10)    //����Ƿ�ʱ��10��
            {
                //free_comm_mem(Zigbee_Rec_Status[i].Mem_Index);          //ǿ���ͷ�
                //Zigbee_Rec_Status[i].Mem_Index = 0xff;
                free_mem_block(Zigbee_Rec_Status[i].Mem_Ptr);          //ǿ���ͷ�
                Zigbee_Rec_Status[i].Mem_Ptr = NULL;
            }
        }
        if(check_local_addr(Zigbee_Duplicate_Frame[i].Src_Addr) == __FALSE)     //���Դ��ַ�Ƿ���Ч
        {
            //if((Sys_Tick - Zigbee_Duplicate_Frame[i].Tick) > DLY_1S * 10)       //����Ƿ�ʱ��10��
            if((OS_Time - Zigbee_Duplicate_Frame[i].Tick) > OS_DLY_1S * 10)       //����Ƿ�ʱ��10��
            {
                Zigbee_Duplicate_Frame[i].Ptr = 0;
                memset(Zigbee_Duplicate_Frame[i].Src_Addr, 0, 8);
                memset(Zigbee_Duplicate_Frame[i].Frame_Serial, 0xff, 8);
            }
        }
    }

    if(Start_Ptr != End_Ptr)            //����FIFO���Ƿ��п���
    {
        if((U1IER & 0x01) == 0)
        {
            U1IER |= 0x01;              //���ڴ����У�ʹ����ʱ�رյĽ����ж�
        }
    }
}

/******************************************************************************
** ��������: void check_zigbee_send_mem(void)
** ��������: ZigBee�����ڴ��ȴ��ط���ռ�ó�ʱ��飬��ʱ��ǿ���ͷ��ڴ��
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void check_zigbee_send_mem (void)
{
    U8 i;

    for(i = 0; i < ZIGBEE_SEND_MEM_NUM; i++)
    {
        if((Zigbee_Send_Status[i].Flag & (IN_USE | WAIT_RESEND)) == (IN_USE | WAIT_RESEND))     //����Ƿ��ڵȴ��ط�
        {

            //if((Sys_Tick - Zigbee_Send_Status[i].Tick) > DLY_500MS)         //����Ƿ���Ҫ�ط���500ms
            if((OS_Time - Zigbee_Send_Status[i].Tick) > OS_DLY_500MS)         //����Ƿ���Ҫ�ط���500ms
            {
                if((Zigbee_Send_Status[i].Buf[18] & 0x03) != 0x03)          //����ط�������һ��3�Σ�����֡��ŵĵ���λ
                {
                    Zigbee_Send_Status[i].Buf[18]++;                        //�ط������ӡ�1��
                    Zigbee_Send_Status[i].Buf[Zigbee_Send_Status[i].Buf[21] + 22]++;            //����У���

                    Zigbee_Send_Status[i].Tick = OS_Time;//Sys_Tick;
                    Zigbee_Send_Status[i].Flag |= WAIT_SEND;                //�ٴη���
                }
                //if((Sys_Tick - Zigbee_Send_Status[i].Tick) > DLY_1S * 10)       //����Ƿ��ط���ʱ��10��
                if((OS_Time - Zigbee_Send_Status[i].Tick) > OS_DLY_1S * 10)
                {
                    Zigbee_Send_Status[i].Flag = 0;                             //ǿ���ͷ�
                }
            }
            continue;
        }
        if((Zigbee_Send_Status[i].Flag & IN_USE) == IN_USE)                 //����Ƿ���ʹ����
        {
            //if((Sys_Tick - Zigbee_Send_Status[i].Tick) > DLY_1S * 60 * 3)   //����Ƿ�ʱ��3����
            if((OS_Time - Zigbee_Send_Status[i].Tick) > OS_DLY_1S * 60 * 3)
            {
                Zigbee_Send_Status[i].Flag = 0;                             //ǿ���ͷ�
            }
        }
    }
}
/******************************************************************************
 Function name:  S8 store_zigbee_comm_data  (U8 com, U32 data)
 Author       :  snap.gao
 Description  :  ���洮�ڽ��յ�������
 Input        :  com :���ں�
                 data:��������
 Return       :  �ڴ���ַ���Ϸ�ʱ����__FAIL
                 ��ȷ������ڴ���������ʱ������__SUCCEED
 *****************************************************************************/
S8 store_zigbee_comm_data (U8 com,  U32 data)
{
    U8  len,i;

    if(com != 1)
    {
        return __FAIL;
    }
    len = (data >> 28) & 0x03;          //���ݳ��ȣ����3��
    if(len == 0)                        //���ݳ��Ȳ����ǡ�0��
    {
        return __FAIL;
    }

    for(i = 0; i < len; i++)
    {
        if(Start_Ptr == End_Ptr)        //���FIFO�Ƿ�����
        {
            U1IER &= ~0x01;             //��ʱ��ֹ�����жϣ���Ӳ����������
            return __FAIL;
        }
        Zigbee_Rec_Fifo[Start_Ptr] = data   >> ((len - 1 - i) * 8);     //����һ������ AZH���ֽ��ڸ�λ�����ж���Ľ���˳��
        Start_Ptr = increase_start_ptr(Start_Ptr, End_Ptr);
    }

    return __SUCCEED;
}
/******************************************************************************
 Function name:  S8 format_zigbee_rec_buf (U8 com, U8 *info)
 Author       :  azh
 Description  :  ��ʽ��Zigbee���ջ����� �ʹ��ڵ�һ��ǰ����һЩ֡ͷ��Ϣ
 Input        :  com :���ں�
                 *info
 Return       :  �ڴ���ַ���Ϸ�ʱ����__FAIL
                 ��ȷ������ڴ���������ʱ������__SUCCEED
 *****************************************************************************/
S8 format_zigbee_rec_buf (U8 com, U8 *info)
{
    if(com != 1)
    {
        return __FAIL;
    }
    init_frame_info(info);          //��ʼ��֡ͷ��Ϣ
    set_frame_len(info, 0);         //��������
    set_port_num(info, com);        //��ʶ�˿ں�

    return __SUCCEED;
}
/******************************************************************************
** ��������: U32 check_duplicate_frame(U8 *addr, U8 serial)
** ��������: ͨ��������֡��Դ��ַ��֡ʶ��ţ��б�ǰ���յ���֡�Ƿ���ǰһ���ظ�
** ��ڲ���: addr  : Դ��ַ�Ļ���
**           serial��֡ʶ���
** �� �� ֵ: __FALSE֡ʶ���û���ظ��������ظ�֡��__TRUE֡ʶ����ظ������ظ�֡
** ˵    ��: ��
******************************************************************************/
U32 check_duplicate_frame (U8 *addr, U8 serial)
{
    U8 i,j;

    for(j = 0; j < ZIGBEE_REC_MEM_NUM; j++)
    {
        if(comp_zigbee_addr(addr, Zigbee_Duplicate_Frame[j].Src_Addr) == __TRUE)    //���Դ��ַ�Ƿ�ƥ��
        {
            for(i = 0; i < 8; i++)      //Դ��ַƥ��ʱ���֡ʶ��ţ������8��
            {
                if(Zigbee_Duplicate_Frame[j].Frame_Serial[i] == serial)
                {
                    break;
                }
            }

            if(i < 8)
            {
                return __TRUE;          //�������8���е�һ���������Ϊ���ظ�֡
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
** ��������: U8 record_frame_serial(U8 *addr, U8 serial)
** ��������: ��¼����֡��Դ��ַ��֡���
** ��ڲ���: addr  : Դ��ַ�Ļ���
**           serial��֡���
** �� �� ֵ: �����ַ��¼��������
** ˵    ��: Ϊÿ���Ự�������������8��ͨѶ��֡���
******************************************************************************/
U8 record_frame_serial (U8 *addr, U8 serial)
{
    U8 j, tmp, ptr;
    U32 tick, sub;

    tick = 0;
    tmp = 0;                                            //�������������
    for(j = 0; j < ZIGBEE_REC_MEM_NUM; j++)
    {
        sub = OS_Time - Zigbee_Duplicate_Frame[j].Tick;
        if(sub >= tick)
        {
            tick = sub;
            tmp = j;                                    //��¼Tickֵ�����λ��
        }
        if(comp_zigbee_addr(addr, Zigbee_Duplicate_Frame[j].Src_Addr) == __TRUE)    //���Դ��ַ�Ƿ�ƥ��
        {
            tmp = j;                                    //Դ��ַƥ��ʱ�������λ�ü�¼�µ�֡ʶ���
            break;
        }
    }
    if(ZIGBEE_REC_MEM_NUM == j)                         //û�ҵ�ƥ���, ����ʹ����Ч������ַ��λ��
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
    Zigbee_Duplicate_Frame[tmp].Frame_Serial[ptr] = serial;             //�Ҳ���ƥ���Դ��ַʱ�Ҹ������λ�ü�¼֡ʶ���
    memcpy(Zigbee_Duplicate_Frame[tmp].Src_Addr, addr, 8);              //��¼Դ��ַ
    if(++Zigbee_Duplicate_Frame[tmp].Ptr >= 8)                          //֡ʶ���ָ����һ��λ�ã�Ŀǰһ��8��
    {
        Zigbee_Duplicate_Frame[tmp].Ptr = 0;
    }
    Zigbee_Duplicate_Frame[tmp].Tick = OS_Time;                        //ˢ��Ϊ��ǰϵͳTick
    return tmp;
}
/******************************************************************************
** ��������: U32 check_local_addr (U8 *buf)
** ��������: ���ZigBee��ַ�Ƿ񱾵�ͨѶ��ַ����ȫ��0��
** ��ڲ���: buf: ��ַ����
** �� �� ֵ: ȫ��0�����ص�ַ����__TRUE�����Ƿ���__FALSE
** ˵    ��: ��
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
        return __TRUE;              //��ַȫ��0x00��
    }

    return __FALSE;
}

/******************************************************************************
** ��������: U32 check_broadcast_addr (U8 *buf)
** ��������: ���ZigBee��ַ�Ƿ�㲥ͨѶ��ַ����ȫ��0xff��
** ��ڲ���: buf: ��ַ����
** �� �� ֵ: ȫ��0xff���㲥��ַ����__TRUE�����Ƿ���__FALSE
** ˵    ��: ��
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
        return __TRUE;              //��ַȫ��0xff��
    }

    return __FALSE;
}

/******************************************************************************
** ��������: U32 comp_zigbee_addr (U8 *buf1, U8 *buf2)
** ��������: �Ƚ�2��ZigBee��ַ
** ��ڲ���: buf1: ��ַ1����
**           buf2: ��ַ2����
** �� �� ֵ: ��ַƥ�䷵��__TRUE�����򷵻�__FALSE
** ˵    ��: ��
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
        return __TRUE;          //��ַƥ��
    }

    return __FALSE;
}
#endif// ZIGBEE_TEST

/******************************************************************************
** ��������: unsigned char ConvertToInerPort (unsigned char ucInPort)
** ��������: ����˿�ת�����ڲ���Ӧ�Ķ˿�(�����м̶˿� ���в��������ڶ˿�)
** ��ڲ���: ucPort: �ⲿ�˿�
**          
** �� �� ֵ: �ڲ���Ӧ�Ķ˿� ���ΪFF��Ϊ�Ƿ�
** ˵    ��: ��
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

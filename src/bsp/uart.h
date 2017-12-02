/******************************************************************************
 Copyright (C) 2009  R&D Institute of Reallin Co., Ltd.
 Module         : uart mamage function head file
 File Name      : uart.h
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

#ifndef __UART_H
#define __UART_H
/*----------------------------- macro definition ----------------------------*/
#ifdef ZIGBEE_TEST
    #define ZIGBEE_REC_FIFO_LEN     1024            //ZigBee���ն��г���
    #define ZIGBEE_REC_MEM_NUM      4               //ZigBee�����������ռ�õ��ڴ��������Ҳ����ͬʱ�ܴ���ͨѶ����ĸ���
    #define ZIGBEE_SEND_MEM_NUM     8               //ZigBee�����ڴ������
#endif// ZIGBEE_TEST

#ifdef ZIGBEE_TEST
    #define IN_USE                  (1<<0)          //�ڴ������ʹ��
    #define RCV_END                 (1<<1)          //����������ɣ���Ҫ��Ĭ�Ͻ�����������
    #define WAIT_SEND               (1<<2)          //�ȴ�����
    #define WAIT_RESEND             (1<<3)          //�ȴ��ط���ʱ���ȴ��ڼ������ط�����ʱ���ͷ��ڴ��


    #define BROADCAST_ZIGBEE_FRAME_LEN  (80 - 4)
    #define UNICAST_ZIGBEE_FRAME_LEN    (80 - 4 - 8)
    #define MAX_ZIGBEE_FRAME_LEN        (80 - 4)        //ZigBee���͵����������Ч���ݳ���
    #define MAX_PACKAGE_NUM             4
    #define ZIGBEE_CHANNEL              0x19u
#endif// ZIGBEE_TEST

#ifdef ZIGBEE_TEST
    #define free_zigbee_send_mem(index)     if(index < ZIGBEE_SEND_MEM_NUM)             \
                                            {                                           \
                                                Zigbee_Send_Status[index].Flag = 0;     \
                                            }               //�ͷ�ZigBeeͨѶ�ڴ�飬index�ڴ��������±�
#endif// ZIGBEE_TEST

#define     RBR                     (0x00 / 4)                //����UART�������궨��
#define     THR                     (0x00 / 4)
#define     DLL                     (0x00 / 4)
#define     DLM                     (0x04 / 4)
#define     IER                     (0x04 / 4)
#define     IIR                     (0x08 / 4)
#define     FCR                     (0x08 / 4)
#define     LCR                     (0x0C / 4)
#define     LSR                     (0x14 / 4)
//#define     SCR                     (0x1C / 4)       //azh ��ͻ
#define     ACR                     (0x20 / 4)
#define     ICR                     (0x24 / 4)
#define     FDR                     (0x28 / 4)
#define     TER                     (0x30 / 4)
/*
#define     UART0                   (volatile unsigned long *)UART0_BASE_ADDR
#define     UART1                   (volatile unsigned long *)UART1_BASE_ADDR
#define     UART2                   (volatile unsigned long *)UART2_BASE_ADDR
#define     UART3                   (volatile unsigned long *)UART3_BASE_ADDR
*/
//azh 110516
#define UART0                   (VU32 *)0x4000C000
#define UART1                   (VU32 *)0x40010000
#define UART2                   (VU32 *)0x40098000
#define UART3                   (VU32 *)0x4009C000

#define     MAX_PORT_NUM            4                       //ϵͳ��󴮿���
#define     MAX_TOTPORT_NUM         (MAX_PORT_NUM+1)        //ϵͳ��󴮿���+1��ģ�����

#define     BLOCK                   0xffff                  //��������ʽ���봮��
#define     QUERY                   0x0000                  //���ڲ�ѯʽ���봮��
#define     LEN_NOT_DEFINE          0                       //���Ȳ�����
#define     DEFAULT_BAUD_RATE       0xffff                  //Ĭ�ϵĲ�����
#define     DEFAULT_COM_PARA        0xff                    //Ĭ�ϵĴ��ڲ���



//1������ģʽ�֣�
//�м�����CON_RS485CACHE_RELAY
//�м�����CON_RELAYSOUR_RELAYTASK
//��ʱ������CONRS485SOURTIMER
//��ʱ��ͨ����CON_RS485ASKOPESOUR_TASK
//�澯����ʵʱ���ݵĳ���CON_RS485ASKOPESOUR_ALARMUP
//ͬ������ʾˢ��(���ѹ��ͬ������)CON_TER_SYNFLASH
//2���˿ںţ��м�������Ƿ��صĶ˿ں�  ���������ж˿ںż��ĸ��˿ڳ���
//3����ţ��м��������֡���     ��������������� 
//4���������ͣ���������

#define     DATA_LEN_OFFSET         0                       //���ݳ��ȣ�ͨѶ֡ͷ����Ϣ�ĵ�ַƫ�ƺ궨��
#define     COM_NUM_OFFSET          2                       //���ں�
#define     CON_MODE_OFFSET         3                       //����ģʽ��
#define     CON_ID_OFFSET        4                       //��Ż�������,�����
#define     CON_RW_TYPE_OFFSET      5                       //�������ͣ���������
#define     COM_TCPCHN_OFFSET       6                       //TCPͨ����
#define     SEND_BAUD_OFFSET        7                       //���Ͳ�����
#define     SEND_PARA_OFFSET        9                       //���ʹ��ڲ���
#define     REC_BAUD_OFFSET         10                      //���ղ�����
#define     REC_PARA_OFFSET         12                      //���մ��ڲ���
//��ʽ�� zigbee���ڻ���ʱ�Ķ���8֮�󱣴����zigbee�ظ���ַ
#define     ZIGBEE_RET_ADDR_OFFSET  13
#define     REC_MEM_TYPE_OFFSET     21                      //ָ������Ӧ��ʹ�õ��ڴ������
#define     DEAL_FLAG_OFFSET        22                      //����Ĵ����־1B ͸��ʱ����Э������
#define     DATA_OFFSET             23
//��������м������ͬ������ ������ֽ��ǣ����ݱ�ʶ����(1B)+���ݱ�ʶ(2*Num B)

//#define       IP_ADDR_OFFSET          3                       //UDP���յ������ݵ�����IP
//#define       PORT_NUM_OFFSET         7                       //UDP���յ������ݵ������˿ں�
//#define       REC_MEM_TYPE_OFFSET     11                      //ָ������Ӧ��ʹ�õ��ڴ������
//#define       SOCKET_OFFSET           12                      //ָ��ͨѶʹ�õ�socket������̫��ͨѶʱʹ��
//#define       SOCKET_TYPE_OFFSET      13                      //ָ��socket�����ͣ�����̫��ͨѶʱʹ��
//#define       DEAL_FLAG_OFFSET        15                      //����Ĵ����־
//#define       DATA_OFFSET             16                      //ͨѶ���ݿ�ʼ

#define     WAIT_ANSWER_10MS        (10 / 10)               //�ȴ�Ӧ��ʱʱ��ĺ궨��
#define     WAIT_ANSWER_100MS       (100 / 10)
#define     WAIT_ANSWER_200MS       (200 / 10)              //�ȴ�Ӧ��ʱʱ��ĺ궨��
#define     WAIT_ANSWER_500MS       (500 / 10)
#define     WAIT_ANSWER_1S          (1000 / 10)
#define     WAIT_ANSWER_2S          (2 | 0x80)
#define     WAIT_ANSWER_3S          (3 | 0x80)
#define     WAIT_ANSWER_5S          (5 | 0x80)
#define     WAIT_ANSWER_10S         (10 | 0x80)
#define     WAIT_ANSWER_15S         (15 | 0x80)
#define     WAIT_ANSWER_20S         (20 | 0x80)
#define     WAIT_ANSWER_30S         (30 | 0x80)
#define     WAIT_ANSWER_60S         (60 | 0x80)
#define     WAIT_ANSWER_120S        (120 | 0x80)            //����RTXϵͳ�������ƣ�Tick��2msʱ����󲻳���131s

#define     MECHANISM_RECORD_LEN    1024                    //UART������״̬����¼������

/*----------------------------- type definition -----------------------------*/
#define     LOCAL_LED_FLASH_DELAY  	1//����ͨ�ŵ���˸ʱ�� UNIT:100MS

#ifdef ZIGBEE_TEST
#define     ZIGBEE_LED_FLASH_DELAY  1//zigbeeͨ�ŵ���˸ʱ�� UNIT:100MS

typedef struct
{
    U32 Tick;                           //ϵͳTick�Ļ���
//    U8 Mem_Index;                       //���뵽���ڴ���±�
    U8 *Mem_Ptr;                       //���뵽���ڴ��ָ��
    U8 Package_Flag;                    //�յ��ķְ���־λ
    U8 Total_Package;                   //�ܰ���
    U8 Frame_Serial;                    //֡ʶ��ţ�֡��ż��ְ��ŵĲ�
}ZIGBEE_REC_STATUS_STRUCT;

typedef struct
{
    U32 Tick;                           //ϵͳTick�Ļ���
    U16 Len;                            //���ݳ���
    U8 Flag;                            //״̬��־�������������궨��
    U8 Buf[MAX_ZIGBEE_FRAME_LEN + 24];  //�������ݻ���
}ZIGBEE_SEND_STATUS_STRUCT;

typedef struct
{
    U32 Tick;                           //ϵͳTick�Ļ���
    U8 Ptr;                             //֡ʶ��ŵĵ�ǰָ�룬����ָ��һ����λ��
    U8 Frame_Serial[8];                 //֡ʶ��ţ�֡��ż��ְ��ŵĲ�
    U8 Src_Addr[8];                     //����֡��Դ��ַ
}ZIGBEE_DUPLICATE_FRAME_STRUCT;

#endif// ZIGBEE_TEST

typedef struct
{
    VU8     Index;                      //azh ���� ���ڸ�ѹ�����˴���״̬���������������ϵͳ״̬����һ�� ��������

    VU8    Send_State;                  // ��ǰ���ڵķ���״̬��
    VU8    Rec_State;                   // ��ǰ���ڵĽ���״̬��
    VU32   Tmp_Tick_Count;              // ����Tickֵ
    VU32   Rec_Tick_Count;              // �ϴ��յ�����ʱ��鵽��Tickֵ���ж��ֽڳ�ʱ��֡��ʱ����ʱ����ΪͨѶ��������������ֵ
    VU32   Send_Tick_Count;             // ״̬����ʼ����ʱ��鵽��Tickֵ������״̬�����ڷ��͵��ʱ�䣬��ʱ��״̬��ֱ�������ȴ��´η�����ʱ������ʧ�ܣ�����������ֵ
    VU32   Send_End_Tick_Count;           //״̬���ȴ����ͽ���ʱ��鵽��Tickֵ������״̬�����ڵȴ����ͽ������ʱ�䣬��ʱ��״̬��ֱ�������ȴ��´η�����ʱ������ʧ�ܣ�����������ֵ
    VU32   Dly_Tick_Count;              // ���ͨѶ����ʼ��ʱ��鵽��Tickֵ������ͨѶ����С�������ʱ��״̬�����ؿ��У���������ֵ
    VU32   Idle_Tick_Count;             // ״̬����ʼ���е�Tick���������ڳ�����ʱ����ʱ���ʼ�����ڣ�����Ĭ��ֵ
    U8     *Rx_Buf;                     // ��ǰ�������ݻ������׵�ַ, �����ֶ���, ��2�ֽ�Ϊ��ʾ����, ������ͨѶ��������, ��СΪ16
    U8     *Tx_Buf;                     // ��ǰ�������ݻ������׵�ַ, �����ֶ���, ��2�ֽ�Ϊ��ʾ����, ������ͨѶ��������, ��СΪ16
    U8     Last_Err_State;              // �ϴι��ϱ�־
    U32    Dly_Tick_Space;              // ����ͨѶ����С�������ʼ���õ�
    U32    Send_Tick_Space;             // ����״̬�ȴ���ʱ�����ÿ��ͨѶʱ����õ������ݳ��ȡ�50ms��֧��300bps��
    U16    Byte_Space;                  // �ֽڼ�����ʱ����, ����ʱ������·����ά�ֵ��ֽ����������10����Ϊ��λ����ʼ���õ�
    VU16   Data_Send_Num;               // �Ѿ����͵����ݸ���
    U16    Send_Baud;                   // ����ʱ�����ڵĽ��ղ�����/300
    U8     Send_Para;                   // ����ʱ�����ڵĽ��ղ�����У��λ��ֹͣλ�����ݳ���
    U16    Rec_Baud;                    // ������ɺ󣬴��ڵĽ��ղ�����/300
    U8     Rec_Para;                    // ������ɺ󣬴��ڵĽ��ղ�����У��λ��ֹͣλ�����ݳ���
    U16    Default_Baud;                // ����Ĭ�ϲ�����/300����ʼ���õ�
    U8     Default_Para;                // ����Ĭ�ϲ�����У��λ��ֹͣλ�����ݳ��ȣ���ʼ���õ�
    U8     Rec_Mem_Type;                // �����ڴ������
    OS_MUT Mut_Com_Usable;              // ������Դ�����ź���
    os_mbx_declare(Mbx_Send_Buf, 16);   // ���Ͷ���
    os_mbx_declare(Mbx_Answer_Buf, 1);  // �ȴ�����ص�����
}STATUS_MECHANISM_STRUCT;

typedef enum
{
    NULL_SEND_STATUS,       //��Ч----(0)
    COM_IDLE,               //����
    COM_READY,              //׼��ͨѶ
    CONFIG_SEND_STATUS,     //���ô��ڷ��Ͳ���
    DATA_SEND,              //����
    DATA_SEND_END,          //�������----(5)
    WAIT_NEXT,              //��ʱ���ȴ��´�ͨѶ
    CONFIG_DEFAULT_STATUS,  //���ô���Ĭ�Ͻ��ղ���
}SEND_STATUS_MECHANISM_MARK;

typedef enum
{
    NULL_REC_STATUS,        //��Ч----(0)
    WAIT_REC,               //�ȴ���������
    ANSWER_RECEIVED,        //�յ�Ӧ������
    DATA_REC,               //��������
    BYTE_REC_TMO,           //�ֽڽ��ճ�ʱ
    ANSWER_REC_TMO,         //Ӧ��֡���ճ�ʱ----(10)
    CONFIG_REC_STATUS,      //���ô��ڽ���Ӧ�����
}REC_STATUS_MECHANISM_MARK;

typedef enum
{
    STATUS_OK,              //����----(0)
    MEM_EMPTY,              //�ڴ����벻��
    MEM_ERROR,              //�����ڴ��ַ��
    LEN_OVERFLOW,           //���ȳ���1024��16���ֽ�
    IDEL_TIMEOUT,           //IDEL״̬��ʱ
    SEND_TIMEOUT,           //SEND״̬��ʱ----(5)
    REC_TIMEOUT,            //���մ�Ӧ״̬��ʱ
    COMM_MBOX_OVERFLOW,     //����������ն������
    ANSWER_MBOX_ERROR,      //Ӧ������������
    NO_MATCH_STATUS         //û��ƥ���״̬��
}STATUS_MECHANISM_ERROR;

typedef enum
{
    COM0,                   //��оƬ�ڲ�����UART0
    COM1,                   //��оƬ�ڲ�����UART1
    COM2,                   //��оƬ�ڲ�����UART2
    COM3,                   //��оƬ�ڲ�����UART3
    COM4                    //����ģ��
}COM_DISPATCH;
//azh from ��ѹ��uart.h
typedef enum
{
    RS4851_CHL,                         //RS485_1
    ZIGBEE_CHL,                         //zigbee����ģ���С���߻��ڲ�ͨ��
    RS4852_CHL,                         //RS485_2
    GPRS_CHL,                           //GPRSģ��
    IR_SOFT_CHL,                        //���� ���ģ��
}COM_CHANNEL;                           //LJL,2010.8.28��ӣ������ж�ͨѶbuff�±����;��

typedef enum
{
    UART_ODD_PARITY  = ((1 << 3) + (0x00 << 4)),
    UART_EVEN_PARITY = ((1 << 3) + (0x01 << 4)),
    UART_ONE_PARITY  = ((1 << 3) + (0x10 << 4)),
    UART_ZERO_PARITY = ((1 << 3) + (0x11 << 4)),
    UART_NO_PARITY   = (0 << 3)
}UART_PARITY_BIT;

typedef enum
{
    UART_1_STOP_BIT = (0 << 2),
    UART_2_STOP_BIT = (1 << 2)
}UART_STOP_BIT;

typedef enum
{
    UART_5BIT_DATA,
    UART_6BIT_DATA,
    UART_7BIT_DATA,
    UART_8BIT_DATA
}UART_BITS_OF_DATA;

typedef enum
{
    NORMAL_TYPE,        //��ָ���ڴ�����ͣ���Щ�����Ĭ��1536Byte����
    MEM_64_TYPE,        //�ڴ������64Bytes
    MEM_256_TYPE,       //�ڴ������256Bytes
    MEM_1536_TYPE       //�ڴ������1536Bytes
}MEM_TYPE;

typedef struct OS_MCB   //������RTX�ں��ļ���һ�ݿ�����ע��汾ͬ��
{
   U8     cb_type;                /* Control Block Type                      */
   U8     isr_st;                 /* State flag variable for isr functions   */
   struct OS_TCB *p_lnk;          /* Chain of tasks waiting for message      */
   U16    first;                  /* Index of the message list begin         */
   U16    last;                   /* Index of the message list end           */
   U16    msg_count;              /* Actual number of stored messages        */
   U16    max_count;              /* Maximum number of stored messages       */
   void   *msg[1];                /* FIFO for Message pointers 1st element   */
}*P_MCB;

/*-------------------------- functions declaration --------------------------*/
__EXTERN    __task  void    UART_task (void);
__EXTERN            void    UART_baud_rate_config (VU32 *UARTx, U32 baud);
__EXTERN            void    UART_mode_config (VU32 *UARTx, U8 mode);
//azh 110516
__EXTERN            void UART0_IRQHandler(void);
__EXTERN            void UART1_IRQHandler(void);
__EXTERN            void UART2_IRQHandler(void);
__EXTERN            void UART3_IRQHandler(void);
__EXTERN            void    UART_init (void);
//__EXTERN            void    UART0_init (U8 mode, U32 baud);
//__EXTERN            void    UART1_init (U8 mode, U32 baud);
//__EXTERN            void    UART2_init (U8 mode, U32 baud);
//__EXTERN            void    UART3_init (U8 mode, U32 baud);
__EXTERN            S8      UART_open (U8 com, U16 time_out);
__EXTERN            S8      UART_send (U8 com, U16 len, U8 *sendbuf);
__EXTERN            U8*     UART_wait_answer (U8 com, U8 wait_time);
__EXTERN            S8      UART_resume (U8 com);
__EXTERN            S8      UART_close (U8 com);
__EXTERN            U8      check_mbox (OS_ID mailbox);
__EXTERN            void    config_send_band (U8 com, U32 band);
__EXTERN            void    config_send_para (U8 com, U8 para);
__EXTERN            void    config_rec_band (U8 com, U32 band);
__EXTERN            void    config_rec_para (U8 com, U8 para);
__EXTERN            void    config_rec_mem_type (U8 com, U8 type);
__EXTERN            void    init_frame_info (U8 *info);
__EXTERN            void    set_frame_len (U8 *info, U16 len);
__EXTERN            void    set_port_num (U8 *info, U8 port);
__EXTERN            void    set_send_para (U8 *info, U8 para, U16 baud);
__EXTERN            void    set_rec_para (U8 *info, U8 para, U16 baud);
__EXTERN            void    set_answer_behavior (U8 *info, U8 time_out, U8 mem_type);
__EXTERN            void    set_deal_flag (U8 *info, U8 flag);
__EXTERN            void    status_mechanism_init (U8 com);

__EXTERN            void    UART3_init(U8 mode, U32 baud);
__EXTERN            S32 uart_send_idle(U8 com, U32 time_out);
__EXTERN            unsigned char ConvertToInerPort(unsigned char ucPort);
/*--------------------------- variable declaration --------------------------*/
__EXTERN    STATUS_MECHANISM_STRUCT     Status_Mechanism[MAX_PORT_NUM];     //UARTͨѶ״̬��

#ifdef __UART_DEBUG
__EXTERN    U32     Mechanism_Record[MECHANISM_RECORD_LEN];                 //UART������״̬����¼��
#endif//__UART_DEBUG

#ifdef ZIGBEE_TEST
//__EXTERN U8 zigbee_led_time[2];//zigbee�շ�����˸��ʱ100ms��λ //azh 171011
__EXTERN U8 gSource_Addr[8];
__EXTERN U8 gAim_Addr[8];
__EXTERN U8 Zigbee_Reset_Reason[MAX_ZIGBEE_FRAME_LEN];
__EXTERN U8 Frame_Serial_Num;
__EXTERN U8 Zigbee_Rec_Fifo[ZIGBEE_REC_FIFO_LEN];
__EXTERN U16 Start_Ptr;         //��ǰFIFO����ָ�룬����ָ��һ����λ��
__EXTERN U16 End_Ptr;           //��ǰFIFO����ָ�룬����ָ�����һ���Ѿ������λ��
__EXTERN ZIGBEE_REC_STATUS_STRUCT Zigbee_Rec_Status[ZIGBEE_REC_MEM_NUM];
__EXTERN ZIGBEE_SEND_STATUS_STRUCT Zigbee_Send_Status[ZIGBEE_SEND_MEM_NUM];
__EXTERN ZIGBEE_DUPLICATE_FRAME_STRUCT Zigbee_Duplicate_Frame[ZIGBEE_REC_MEM_NUM];
#endif// ZIGBEE_TEST

//#ifdef GPRS_MODULE_PRJ//azh 171011 ����
__EXTERN U8 local_led_time[2];//�����շ�����˸��ʱ100ms��λ
//#endif

#endif//__UART_H
/******************************************************************************
                            End Of File
 *****************************************************************************/

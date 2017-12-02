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
   2、
   1、     1.00       2009-06-30     snap.gao       create
******************************************************************************/

#ifndef __UART_H
#define __UART_H
/*----------------------------- macro definition ----------------------------*/
#ifdef ZIGBEE_TEST
    #define ZIGBEE_REC_FIFO_LEN     1024            //ZigBee接收队列长度
    #define ZIGBEE_REC_MEM_NUM      4               //ZigBee接收最多允许占用的内存块数量，也就是同时能处理通讯对象的个数
    #define ZIGBEE_SEND_MEM_NUM     8               //ZigBee发送内存块数量
#endif// ZIGBEE_TEST

#ifdef ZIGBEE_TEST
    #define IN_USE                  (1<<0)          //内存块正在使用
    #define RCV_END                 (1<<1)          //接收数据完成，需要由默认解析函数处理
    #define WAIT_SEND               (1<<2)          //等待发送
    #define WAIT_RESEND             (1<<3)          //等待重发超时，等待期间允许重发，超时后释放内存块


    #define BROADCAST_ZIGBEE_FRAME_LEN  (80 - 4)
    #define UNICAST_ZIGBEE_FRAME_LEN    (80 - 4 - 8)
    #define MAX_ZIGBEE_FRAME_LEN        (80 - 4)        //ZigBee发送单包的最大有效数据长度
    #define MAX_PACKAGE_NUM             4
    #define ZIGBEE_CHANNEL              0x19u
#endif// ZIGBEE_TEST

#ifdef ZIGBEE_TEST
    #define free_zigbee_send_mem(index)     if(index < ZIGBEE_SEND_MEM_NUM)             \
                                            {                                           \
                                                Zigbee_Send_Status[index].Flag = 0;     \
                                            }               //释放ZigBee通讯内存块，index内存块数组的下标
#endif// ZIGBEE_TEST

#define     RBR                     (0x00 / 4)                //关于UART的新增宏定义
#define     THR                     (0x00 / 4)
#define     DLL                     (0x00 / 4)
#define     DLM                     (0x04 / 4)
#define     IER                     (0x04 / 4)
#define     IIR                     (0x08 / 4)
#define     FCR                     (0x08 / 4)
#define     LCR                     (0x0C / 4)
#define     LSR                     (0x14 / 4)
//#define     SCR                     (0x1C / 4)       //azh 冲突
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

#define     MAX_PORT_NUM            4                       //系统最大串口数
#define     MAX_TOTPORT_NUM         (MAX_PORT_NUM+1)        //系统最大串口数+1个模拟红外

#define     BLOCK                   0xffff                  //用于阻塞式申请串口
#define     QUERY                   0x0000                  //用于查询式申请串口
#define     LEN_NOT_DEFINE          0                       //长度不定义
#define     DEFAULT_BAUD_RATE       0xffff                  //默认的波特率
#define     DEFAULT_COM_PARA        0xff                    //默认的串口参数



//1、命令模式字：
//中继命令CON_RS485CACHE_RELAY
//中继任务CON_RELAYSOUR_RELAYTASK
//定时读数据CONRS485SOURTIMER
//定时普通任务CON_RS485ASKOPESOUR_TASK
//告警上送实时数据的抄表CON_RS485ASKOPESOUR_ALARMUP
//同步及显示刷新(与高压表同步数据)CON_TER_SYNFLASH
//2、端口号：中继命令：则是返回的端口号  其他：下行端口号即哪个端口抄表
//3、序号：中继命令：上行帧序号     其他抄表：测量点号 
//4、操作类型：读或设置

#define     DATA_LEN_OFFSET         0                       //数据长度，通讯帧头部信息的地址偏移宏定义
#define     COM_NUM_OFFSET          2                       //串口号
#define     CON_MODE_OFFSET         3                       //命令模式字
#define     CON_ID_OFFSET        4                       //序号或测量点号,任务号
#define     CON_RW_TYPE_OFFSET      5                       //操作类型：读或设置
#define     COM_TCPCHN_OFFSET       6                       //TCP通道号
#define     SEND_BAUD_OFFSET        7                       //发送波特率
#define     SEND_PARA_OFFSET        9                       //发送串口参数
#define     REC_BAUD_OFFSET         10                      //接收波特率
#define     REC_PARA_OFFSET         12                      //接收串口参数
//格式化 zigbee串口缓冲时的定义8之后保存的是zigbee回复地址
#define     ZIGBEE_RET_ADDR_OFFSET  13
#define     REC_MEM_TYPE_OFFSET     21                      //指定接收应答使用的内存块类型
#define     DEAL_FLAG_OFFSET        22                      //特殊的处理标志1B 透传时用作协议类型
#define     DATA_OFFSET             23
//如果不是中继命令或同步命令 则后续字节是：数据标识数量(1B)+数据标识(2*Num B)

//#define       IP_ADDR_OFFSET          3                       //UDP接收到的数据的主机IP
//#define       PORT_NUM_OFFSET         7                       //UDP接收到的数据的主机端口号
//#define       REC_MEM_TYPE_OFFSET     11                      //指定接收应答使用的内存块类型
//#define       SOCKET_OFFSET           12                      //指定通讯使用的socket，在以太网通讯时使用
//#define       SOCKET_TYPE_OFFSET      13                      //指定socket的类型，在以太网通讯时使用
//#define       DEAL_FLAG_OFFSET        15                      //特殊的处理标志
//#define       DATA_OFFSET             16                      //通讯数据开始

#define     WAIT_ANSWER_10MS        (10 / 10)               //等待应答超时时间的宏定义
#define     WAIT_ANSWER_100MS       (100 / 10)
#define     WAIT_ANSWER_200MS       (200 / 10)              //等待应答超时时间的宏定义
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
#define     WAIT_ANSWER_120S        (120 | 0x80)            //由于RTX系统函数限制，Tick＝2ms时，最大不超过131s

#define     MECHANISM_RECORD_LEN    1024                    //UART调试用状态机记录的条数

/*----------------------------- type definition -----------------------------*/
#define     LOCAL_LED_FLASH_DELAY  	1//本地通信灯闪烁时间 UNIT:100MS

#ifdef ZIGBEE_TEST
#define     ZIGBEE_LED_FLASH_DELAY  1//zigbee通信灯闪烁时间 UNIT:100MS

typedef struct
{
    U32 Tick;                           //系统Tick的缓存
//    U8 Mem_Index;                       //申请到的内存块下标
    U8 *Mem_Ptr;                       //申请到的内存块指针
    U8 Package_Flag;                    //收到的分包标志位
    U8 Total_Package;                   //总包数
    U8 Frame_Serial;                    //帧识别号，帧序号减分包号的差
}ZIGBEE_REC_STATUS_STRUCT;

typedef struct
{
    U32 Tick;                           //系统Tick的缓存
    U16 Len;                            //数据长度
    U8 Flag;                            //状态标志：见本文上述宏定义
    U8 Buf[MAX_ZIGBEE_FRAME_LEN + 24];  //发送数据缓存
}ZIGBEE_SEND_STATUS_STRUCT;

typedef struct
{
    U32 Tick;                           //系统Tick的缓存
    U8 Ptr;                             //帧识别号的当前指针，总是指向一个新位置
    U8 Frame_Serial[8];                 //帧识别号，帧序号减分包号的差
    U8 Src_Addr[8];                     //接收帧的源地址
}ZIGBEE_DUPLICATE_FRAME_STRUCT;

#endif// ZIGBEE_TEST

typedef struct
{
    VU8     Index;                      //azh 增加 由于高压表用了串口状态机的这个变量，带系统状态机不一样 所以增加

    VU8    Send_State;                  // 当前串口的发送状态机
    VU8    Rec_State;                   // 当前串口的接收状态机
    VU32   Tmp_Tick_Count;              // 缓冲Tick值
    VU32   Rec_Tick_Count;              // 上次收到数据时检查到的Tick值，判断字节超时和帧超时，超时后认为通讯结束，采用设置值
    VU32   Send_Tick_Count;             // 状态机开始发送时检查到的Tick值，允许状态机处于发送的最长时间，超时后状态机直接跳至等待下次发送延时（发送失败），采用设置值
    VU32   Send_End_Tick_Count;           //状态机等待发送结束时检查到的Tick值，允许状态机处于等待发送结束的最长时间，超时后状态机直接跳至等待下次发送延时（发送失败），采用设置值
    VU32   Dly_Tick_Count;              // 完成通讯，开始延时检查到的Tick值，两次通讯的最小间隔，超时后状态机返回空闲，采用设置值
    VU32   Idle_Tick_Count;             // 状态机开始空闲的Tick计数，用于超长超时，超时后初始化串口，采用默认值
    U8     *Rx_Buf;                     // 当前接收数据缓冲区首地址, 必须字对齐, 首2字节为表示长度, 不包含通讯参数本身, 最小为16
    U8     *Tx_Buf;                     // 当前发送数据缓冲区首地址, 必须字对齐, 首2字节为表示长度, 不包含通讯参数本身, 最小为16
    U8     Last_Err_State;              // 上次故障标志
    U32    Dly_Tick_Space;              // 两次通讯的最小间隔，初始化得到
    U32    Send_Tick_Space;             // 发送状态等待超时间隔，每次通讯时计算得到（数据长度×50ms，支持300bps）
    U16    Byte_Space;                  // 字节间间隔超时设置, 接收时数据链路得以维持的字节最大间隔，以10毫秒为单位，初始化得到
    VU16   Data_Send_Num;               // 已经发送的数据个数
    U16    Send_Baud;                   // 发送时，串口的接收波特率/300
    U8     Send_Para;                   // 发送时，串口的接收参数：校验位、停止位、数据长度
    U16    Rec_Baud;                    // 发送完成后，串口的接收波特率/300
    U8     Rec_Para;                    // 发送完成后，串口的接收参数：校验位、停止位、数据长度
    U16    Default_Baud;                // 串口默认波特率/300，初始化得到
    U8     Default_Para;                // 串口默认参数：校验位、停止位、数据长度，初始化得到
    U8     Rec_Mem_Type;                // 接收内存块类型
    OS_MUT Mut_Com_Usable;              // 串口资源可用信号量
    os_mbx_declare(Mbx_Send_Buf, 16);   // 发送队列
    os_mbx_declare(Mbx_Answer_Buf, 1);  // 等待命令返回的邮箱
}STATUS_MECHANISM_STRUCT;

typedef enum
{
    NULL_SEND_STATUS,       //无效----(0)
    COM_IDLE,               //空闲
    COM_READY,              //准备通讯
    CONFIG_SEND_STATUS,     //配置串口发送参数
    DATA_SEND,              //发送
    DATA_SEND_END,          //发送完成----(5)
    WAIT_NEXT,              //延时，等待下次通讯
    CONFIG_DEFAULT_STATUS,  //配置串口默认接收参数
}SEND_STATUS_MECHANISM_MARK;

typedef enum
{
    NULL_REC_STATUS,        //无效----(0)
    WAIT_REC,               //等待接收数据
    ANSWER_RECEIVED,        //收到应答数据
    DATA_REC,               //接收数据
    BYTE_REC_TMO,           //字节接收超时
    ANSWER_REC_TMO,         //应答帧接收超时----(10)
    CONFIG_REC_STATUS,      //配置串口接收应答参数
}REC_STATUS_MECHANISM_MARK;

typedef enum
{
    STATUS_OK,              //正常----(0)
    MEM_EMPTY,              //内存申请不到
    MEM_ERROR,              //接收内存地址错
    LEN_OVERFLOW,           //长度超（1024－16）字节
    IDEL_TIMEOUT,           //IDEL状态超时
    SEND_TIMEOUT,           //SEND状态超时----(5)
    REC_TIMEOUT,            //接收答应状态超时
    COMM_MBOX_OVERFLOW,     //解析任务接收队列溢出
    ANSWER_MBOX_ERROR,      //应答接收邮箱错误
    NO_MATCH_STATUS         //没有匹配的状态机
}STATUS_MECHANISM_ERROR;

typedef enum
{
    COM0,                   //主芯片内部串口UART0
    COM1,                   //主芯片内部串口UART1
    COM2,                   //主芯片内部串口UART2
    COM3,                   //主芯片内部串口UART3
    COM4                    //红外模拟
}COM_DISPATCH;
//azh from 高压表uart.h
typedef enum
{
    RS4851_CHL,                         //RS485_1
    ZIGBEE_CHL,                         //zigbee无线模块或小无线或内部通信
    RS4852_CHL,                         //RS485_2
    GPRS_CHL,                           //GPRS模块
    IR_SOFT_CHL,                        //红外 软件模拟
}COM_CHANNEL;                           //LJL,2010.8.28添加，用于判断通讯buff下标的用途。

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
    NORMAL_TYPE,        //不指定内存块类型，有些情况下默认1536Byte类型
    MEM_64_TYPE,        //内存块类型64Bytes
    MEM_256_TYPE,       //内存块类型256Bytes
    MEM_1536_TYPE       //内存块类型1536Bytes
}MEM_TYPE;

typedef struct OS_MCB   //这里是RTX内核文件的一份拷贝，注意版本同步
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
__EXTERN    STATUS_MECHANISM_STRUCT     Status_Mechanism[MAX_PORT_NUM];     //UART通讯状态机

#ifdef __UART_DEBUG
__EXTERN    U32     Mechanism_Record[MECHANISM_RECORD_LEN];                 //UART调试用状态机记录区
#endif//__UART_DEBUG

#ifdef ZIGBEE_TEST
//__EXTERN U8 zigbee_led_time[2];//zigbee收发灯闪烁延时100ms单位 //azh 171011
__EXTERN U8 gSource_Addr[8];
__EXTERN U8 gAim_Addr[8];
__EXTERN U8 Zigbee_Reset_Reason[MAX_ZIGBEE_FRAME_LEN];
__EXTERN U8 Frame_Serial_Num;
__EXTERN U8 Zigbee_Rec_Fifo[ZIGBEE_REC_FIFO_LEN];
__EXTERN U16 Start_Ptr;         //当前FIFO接收指针，总是指向一个空位置
__EXTERN U16 End_Ptr;           //当前FIFO处理指针，总是指向最后一个已经处理的位置
__EXTERN ZIGBEE_REC_STATUS_STRUCT Zigbee_Rec_Status[ZIGBEE_REC_MEM_NUM];
__EXTERN ZIGBEE_SEND_STATUS_STRUCT Zigbee_Send_Status[ZIGBEE_SEND_MEM_NUM];
__EXTERN ZIGBEE_DUPLICATE_FRAME_STRUCT Zigbee_Duplicate_Frame[ZIGBEE_REC_MEM_NUM];
#endif// ZIGBEE_TEST

//#ifdef GPRS_MODULE_PRJ//azh 171011 共用
__EXTERN U8 local_led_time[2];//本地收发灯闪烁延时100ms单位
//#endif

#endif//__UART_H
/******************************************************************************
                            End Of File
 *****************************************************************************/

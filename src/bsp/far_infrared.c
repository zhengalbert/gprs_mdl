/******************************************************************************
 Copyright (C) 2011  R&D Institute of Reallin Co., Ltd.
 Module         : far infrared mamage function
 File Name      : far_infrared.c
 Description    : LPC17系列芯片硬件IO口模拟串口远红外通信
 Author         :
 Build Date     :
 others         :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、     1.00       2011-07-30     rosoon     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

//-------------------------------- 常数定义 -----------------------------------
#define CON_IR_PWM_FREQ     ((FPCLK_PWM1 / (PWM1PR + 1)) / 38000)
//------------------------------ 私有变量声明 ---------------------------------

//------------------------------ 私有函数声明 ---------------------------------


//------------------------------ 函数原型定义 ---------------------------------
/******************************************************************************
** 函数名称: void far_infrared_init(void)
** 函数功能: 远红外通信口初始化功能函数
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void far_infrared_init (void)
{
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//不带CPU_GPRS模块
//38k脚输出是P1.26 需要改!!!!

    PCONP |= ((1 << PCGPIO) | (1 << PCPWM1));   //使能GPIO时钟与PWM1时钟
    PCLKSEL0 |= (FPCLK_PWM1_DIV << 12);         //设置外设PWM1分频系数1/8

    PINMODE_OD1 &= ~P1_PWM_38K;                 //IRDA_TXD(p1.26):非开漏

    PINSEL3 &= ~(0x03u << 20);                   //先清除相关bits
    PINSEL3 |= (P1_26_FNUC << 20);               //PWM1[1]空闲时配置成GPIO,并输出高以节省功耗

    PINMODE3 &= (0x03u << 20);                           //先清除相关bits
    PINMODE3 = (P1_26_MODE << 20);                        //PIN_NO_PULL

//    SET_PORT1_OUT(P1_PWM_38K);                  //输出口
//    SET_PORT1_LOW(P1_PWM_38K);                  //输出低(减少能耗) 

    PWM1PR = 29;                                //预分频:将外设频率进行30(PWM1PR+1)分频[384kHz]
    PWM1MR0 = ((FPCLK_PWM1 / (PWM1PR + 1)) / FAR_INFRARED_CARRIER); //PWM输出频率38kHz

    PWM1MR6 = PWM1MR0 >> 1;                     //PWM输出频率38kHz的半周期占空比
    PWM1MCR |=(1 << 1);             //PWMMR0/PWMMR1与PWMTC的值相匹配时产生中断，且仅PWMMR0匹配时PWMTC复位
    PWM1CTCR = 0;                               //PWM计数控制寄存器复位
    PWM1PCR = (1 << 6) | (1 << 14);             //PWM控制寄存器复位
    PWM1TCR = (1 << 3) + 1;                     //PWM模式使能
    PWM1LER = 1 + (1 << 6);                                //使能PWM匹配0/1锁存
//
//    PWM1MR6 = PWM1MR0 >> 2;                     //PWM输出频率38kHz的4分之1周期占空比
//    PWM1MCR |= (3 << 0) | (1 << 3);             //PWMMR0/PWMMR1与PWMTC的值相匹配时产生中断，且仅PWMMR0匹配时PWMTC复位
//    PWM1CTCR = 0;                               //PWM计数控制寄存器复位
//    PWM1PCR = 0;                                //PWM控制寄存器复位
////    PWM1TCR = (1 << 0) | (1 << 3);              //PWM定时器控制寄存器:PWM定时器计数器、预分频计数器、PWM模式使能
////    PWM1TCR = (1 << 3);                         //PWM模式使能
//    PWM1LER = 3;                                //使能PWM匹配0/1锁存
//    PWM1TCR = (1 << 1);                         //复位
#else
    IR_VDD_ON();                                //使能红外电源
//IO口初始化：发送口&接收口(包括IO口中断模式配置)[注意在main.c里开关相关中断处理]
    PCONP |= ((1 << PCGPIO) | (1 << PCPWM1));   //使能GPIO时钟与PWM1时钟
    PCLKSEL0 |= (FPCLK_PWM1_DIV << 12);         //设置外设PWM1分频系数1/8

    PINMODE_OD1 &= ~P1_PWM_38K;                 //IRDA_TXD(p1.18):非开漏
    PINMODE_OD2 &= ~P2_IRRXD;                   //IRDA_RXD(p2.11):非开漏

    PINSEL3 &= ~(0x03u << 4);                   //先清除相关bits
    PINSEL3 |= (P1_18_GPIO << 4);               //PWM1[1]空闲时配置成GPIO,并输出高以节省功耗
    SET_PORT1_OUT(P1_PWM_38K);                  //输出口
    SET_PORT1_LOW(P1_PWM_38K);                  //输出低(减少能耗) 
    
//    if((PINSEL3 & (0x03u << 4))!= (P1_18_FNUC << 4))        //IRDA_TXD(p1.18):配置为PWM功能引脚
//    {
//        PINSEL3 &= ~(0x03u << 4);               //先清除相关bits
//        PINSEL3 |= (P1_18_FNUC << 4);           //PWM1[1]
//        PINMODE3 &= (0x03u << 4);               //先清除相关bits
//        PINMODE3 = (P1_18_MODE << 4);           //PIN_NO_PULL
//    }

    if((PINSEL4 & (0x03u << 22))!= (P2_11_FNUC << 22))      //IRDA_RXD(p2.11):配置为GPIO功能引脚
    {
        PINSEL4 &= ~(0x03u << 22);              //先清除相关bits
        PINSEL4 |= (P2_11_FNUC << 22);          //GPIO
        PINMODE4 &= (0x03u << 22);              //先清除相关bits
        PINMODE4 = (P2_11_MODE << 4);           //PIN_NO_PULL
    }
    SET_PORT2_IN(P2_IRRXD);                     //输入口

//    IO2IntEnr |= P2_IRRXD;                    //P2_IRRXD(p2.11)配置成上升沿输入IO中断
    IO2IntEnf |= P2_IRRXD;                      //P2_IRRXD(p2.11)配置成下降沿输入IO中断
    IO2IntClr = P2_IRRXD;                       //清除中断标志

//--------------------------------初始化PWM1[1]模块功能------------------------
    PWM1PR = 29;                                //预分频:将外设频率进行30(PWM1PR+1)分频[384kHz]
    PWM1MR0 = ((FPCLK_PWM1 / (PWM1PR + 1)) / FAR_INFRARED_CARRIER); //PWM输出频率38kHz
    PWM1MR1 = PWM1MR0 >> 2;                     //PWM输出频率38kHz的4分之1周期占空比
    PWM1MCR |= (3 << 0) | (1 << 3);             //PWMMR0/PWMMR1与PWMTC的值相匹配时产生中断，且仅PWMMR0匹配时PWMTC复位
    PWM1CTCR = 0;                               //PWM计数控制寄存器复位
    PWM1PCR = 0;                                //PWM控制寄存器复位
//    PWM1TCR = (1 << 0) | (1 << 3);              //PWM定时器控制寄存器:PWM定时器计数器、预分频计数器、PWM模式使能
//    PWM1TCR = (1 << 3);                         //PWM模式使能
    PWM1LER = 3;                                //使能PWM匹配0/1锁存
    PWM1TCR = (1 << 1);                         //复位
//--------------------------------初始化FAR_INFRARED_STRUCT--------------------
    Ir_Ctl[IR_RX].Bit_Cnt = 0;
    Ir_Ctl[IR_RX].Bit_Sum = 0;
    Ir_Ctl[IR_RX].Dat_Len = 0;
    Ir_Ctl[IR_RX].Dat_Tmp = 0;
    Ir_Ctl[IR_RX].Dat_Tok = 0;          //令牌初值0
    Ir_Ctl[IR_RX].Dat_Buf[IR_LS] = 0;   //令牌信息初值0
    Ir_Ctl[IR_RX].Dat_Buf[IR_FI + 1] = 0;   //数据帧首字节(兼具错误状态字)清零

    Ir_Ctl[IR_TX].Bit_Cnt = 11;
    Ir_Ctl[IR_TX].Bit_Sum = 0;
    Ir_Ctl[IR_TX].Dat_Len = 0;
    Ir_Ctl[IR_TX].Dat_Tmp = 0;
    Ir_Rxbyte_Timeout_Cnt[0] = Ir_Rxbyte_Timeout_Cnt[1] = 0;
    Ir_Occupy_State = IR_RX;            //默认为接收任务占用状态
#endif  
}

/******************************************************************************
** 函数名称: void PWM1_IRQHandler(void)
** 函数功能: PWM1中断服务子程序
** 入口参数: 无
** 返 回 值: 无
** 说    明: 1、MR0匹配中断,决定PWM1[1]波形周期
**           2、MR1匹配中断,决定PWM1[1]波形占空比
******************************************************************************/
void PWM1_IRQHandler (void)
{
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
	PWM1IR = 0x073F;        //清全不标识
#else//H_METER_PRJ,WLS_CUR_PRJ
    U16 int_flg;

    int_flg = PWM1IR;
	  if(int_flg & (1 << MR0_BIT))                        //PWM1周期(1/38kHz)中断
    {
        PWM1IR = (1 << MR0_BIT);                        //清中断标志
        PWM1MR0 = CON_IR_PWM_FREQ;                      //PWM输出频率38kHz
    }
    if(int_flg & (1 << MR1_BIT))                        //翻转位置(占空比)中断
    {
        PWM1IR = (1 << MR1_BIT);                        //清中断标志
        PWM1MR1 = CON_IR_PWM_FREQ >> 2;                 //PWM输出频率38kHz的4分之1周期占空比
    }
    else
    {
        PWM1IR = 0x073F;        //清全不标识
    }
#endif
}  

#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不是配合带CPU_GPRS模块
/******************************************************************************
** 函数名称: S8 get_far_infrared_data(U8 *buf)
** 函数功能: 远红外通信口收多字节数据功能函数
** 入口参数: buf 收到数据缓存指针
** 返 回 值: 1 完整数据帧
**          -1 无有效数据
** 说    明: 调用该函数后,若数据有效,则buf[0]为数据帧长度,buf[n:1]有效数据帧
******************************************************************************/
S8 get_far_infrared_data (U8 *buf)
{
    U8 cnt;

    cnt = Ir_Ctl[IR_RX].Dat_Buf[IR_LS];                 //读取令牌信息

    Ir_Ctl[IR_RX].Dat_Tok += 1;                         //令牌累加
    if(Ir_Ctl[IR_RX].Dat_Tok == cnt)                    //令牌同步时
    {
        cnt = Ir_Ctl[IR_RX].Dat_Buf[IR_FI];             //读取帧长信息
        if((cnt > IR_FI) && (cnt < IR_LS))              //数据帧完好
        {
            memcpy(buf, Ir_Ctl[IR_RX].Dat_Buf, cnt + 1);//包含帧长度信息
            Ir_Ctl[IR_RX].Dat_Buf[IR_FI] =  0;          //帧长度信息清零
            return 1;
        }
        else
        {
            return -1;
        }
    }
    else                        //令牌失步时
    {
        Ir_Ctl[IR_RX].Dat_Tok = cnt;                     //令牌同步,为下次准备
        return -1;
    }
}

/******************************************************************************
** 函数名称: void send_far_infrared_data(U8 *buf, U8 cnt)
** 函数功能: 远红外通信口发多字节数据功能函数
** 入口参数: buf 待发送数据指针
**           cnt 待发数据字节数
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void send_far_infrared_data (U8 *buf, U8 cnt)
{
    U8 i;

    if((PINSEL3 & (0x03u << 4))!= (P1_18_FNUC << 4))        //IRDA_TXD(p1.18):配置为PWM功能引脚
    {
        PINSEL3 &= ~(0x03u << 4);               //先清除相关bits
        PINSEL3 |= (P1_18_FNUC << 4);           //PWM1[1]
        PINMODE3 &= (0x03u << 4);               //先清除相关bits
        PINMODE3 = (P1_18_MODE << 4);           //PIN_NO_PULL
    }

    PWM1TCR = ((1 << 0) | (1 << 3));				//PWM定时器控制寄存器:PWM定时器计数器、预分频计数器、PWM模式使能
    IO2IntEnf &= ~P2_IRRXD;                         //有发送任务时禁止接收中断避免自发自收

    T1MR1 = T1TC + T1_MAT_FAR_INFRARED;             //加载匹配值
    T1MCR |= 1 << 3;                                //开启MR1匹配产生中断[作远红外发送通信位宽定时]

    T1MR3 = T1TC + FAR_INFRARED_TX_BYTE_TIMING;     //加载匹配值
    T1MCR |= 1 << 9;                                //开启MR3匹配产生中断[作远红外发送字节间隔定时]

    Ir_Ctl[IR_TX].Dat_Len = cnt;
    i = 0;
    while(cnt)
    {
        Ir_Ctl[IR_TX].Dat_Buf[i++] = buf[--cnt];
    }
}

/******************************************************************************
** 函数名称: void far_infrared_rx_drv(void)
** 函数功能: 远红外通信口中断接收多字节数据驱动功能函数
** 入口参数: 无
** 返 回 值: 无
**
** 说    明: 在TIMERx的波特率位宽定时匹配中断处调用
******************************************************************************/
void far_infrared_rx_drv (void)
{
    if(Ir_Ctl[IR_RX].Bit_Cnt < 10)
    {
        Ir_Ctl[IR_RX].Mat_Val = T1TC + T1_MAT_FAR_INFRARED;
        T1MR0 = Ir_Ctl[IR_RX].Mat_Val;          //下次时刻加载
    }

    if(0 == Ir_Ctl[IR_RX].Bit_Cnt)              //起始位验证
    {   //SET_PORT1_REV(P1_DBG_LED);             //debug,查看采样时刻
        SET_PORT2_IN(P2_IRRXD);                 //输入口
        if(PORT2_IS_HIGH(P2_IRRXD))
        {
            Ir_Ctl[IR_RX].Dat_Tmp = 0xEE;       //起始位错误;
        }
        Ir_Ctl[IR_RX].Bit_Sum = 0;              //清零
        Ir_Ctl[IR_RX].Bit_Cnt++;                //加计数
    }
    else if(Ir_Ctl[IR_RX].Bit_Cnt < 9)          //数据位处理
    {   //SET_PORT1_REV(P1_DBG_LED);             //debug,查看采样时刻
        SET_PORT2_IN(P2_IRRXD);                 //输入口
        if(PORT2_IS_HIGH(P2_IRRXD))
        {
            Ir_Ctl[IR_RX].Dat_Tmp |= (1 << (Ir_Ctl[IR_RX].Bit_Cnt - 1));//设置数据位
            Ir_Ctl[IR_RX].Bit_Sum++;            //校验和
        }
        else
        {
            Ir_Ctl[IR_RX].Dat_Tmp &= ~(1 << Ir_Ctl[IR_RX].Bit_Cnt);//设置数据位;
        }
        Ir_Ctl[IR_RX].Bit_Cnt++;                    //加计数
    }
    else if(9 == Ir_Ctl[IR_RX].Bit_Cnt)             //偶校验验证
    {   //SET_PORT1_REV(P1_DBG_LED);             //debug,查看采样时刻

        SET_PORT2_IN(P2_IRRXD);                     //输入口
        if((PORT2_IS_HIGH(P2_IRRXD) && (!(Ir_Ctl[IR_RX].Bit_Sum % 2)))
          || (!PORT2_IS_HIGH(P2_IRRXD) && (Ir_Ctl[IR_RX].Bit_Sum % 2)))
        {
            Ir_Ctl[IR_RX].Dat_Tmp = 0xED;           //偶检验错误;
        }
        Ir_Ctl[IR_RX].Bit_Cnt++;                    //加计数
    }
    else if(10 == Ir_Ctl[IR_RX].Bit_Cnt)            //是停止位吗
    {
        //SET_PORT1_REV(P1_DBG_LED);               //debug,查看采样时刻
        SET_PORT2_IN(P2_IRRXD);                     //输入口
        if(!PORT2_IS_HIGH(P2_IRRXD))
        {
            Ir_Ctl[IR_RX].Dat_Tmp = 0xEB;           //停止位错误;
        }
        else
        {
            Ir_Rxbyte_Timeout_Cnt[0]++;             //超时加计数
            T1MR2 = T1TC + FAR_INFRARED_RX_BYTE_OVERTIME;  //加载字节接收超时匹配值
            T1MCR |= (1 << 6);                      //开启MR2匹配产生中断[作字节接收超时匹配定时]
        }
        Ir_Ctl[IR_RX].Bit_Cnt = 0;                  //清零
        Ir_Ctl[IR_RX].Dat_Buf[++Ir_Ctl[IR_RX].Dat_Len] = Ir_Ctl[IR_RX].Dat_Tmp;//保留Ir_Ctl[IR_RX].Dat_Buf[0]用来放Ir_Ctl[IR_RX].Dat_Len的值
        Ir_Ctl[IR_RX].Dat_Tmp = 0;

        IO2IntClr = P2_IRRXD;                       //冗余清零[很重要]
        IO2IntEnf |= P2_IRRXD;                      //P2_IRRXD(p2.11)恢复下降沿输入IO中断
        T1MCR &= ~(1 << 0);                         //禁止MR0匹配产生中断
    }
}
/******************************************************************************
** 函数名称: void far_infrared_tx_drv(void)
** 函数功能: 远红外通信口中断发送多字节数据驱动功能函数
** 入口参数: 无
** 返 回 值: 无
**
** 说    明: 在TIMERx的波特率位宽定时匹配中断处调用
******************************************************************************/
void far_infrared_tx_drv (void)
{
    U8 i;

    if(Ir_Ctl[IR_TX].Dat_Len)                       //若数据尚未发完
    {
        if(Ir_Ctl[IR_TX].Bit_Cnt == 11)             //每发送1个字节前预处理
        {
            Ir_Ctl[IR_TX].Dat_Tmp = (Ir_Ctl[IR_TX].Dat_Buf[Ir_Ctl[IR_TX].Dat_Len - 1] << 1);    //加载待发数据&起始位0

            Ir_Ctl[IR_TX].Bit_Sum = 0;
            for(i = 1; i < 9; i++)                  //计算数据位bit[9:1]校验和
            {
                Ir_Ctl[IR_TX].Bit_Sum += ((Ir_Ctl[IR_TX].Dat_Tmp >> i) & 1);
            }
            if((Ir_Ctl[IR_TX].Bit_Sum % 2))
            {
                Ir_Ctl[IR_TX].Dat_Tmp |= (0x03 << 9);       //加载偶校验位&停止位
            }
            else
            {
                Ir_Ctl[IR_TX].Dat_Tmp |= (0x02 << 9);       //加载偶校验位&停止位
            }
        }
        if(Ir_Ctl[IR_TX].Bit_Cnt)                   //1个字节尚未发完
        {
            if((Ir_Ctl[IR_TX].Dat_Tmp >> (11 - Ir_Ctl[IR_TX].Bit_Cnt)) & 1)   //待发数据位为1
            {
                PWM1PCR = 0;                        //PWM控制寄存器:禁能PWM1输出
            }
            else
            {
                //输出38kHZ的PWM远红外调制波
                PWM1PCR = (1 << 9);                 //PWM控制寄存器:使能PWM1输出
            }
            Ir_Ctl[IR_TX].Bit_Cnt--;                //位数递减计数

            Ir_Ctl[IR_TX].Mat_Val = T1TC + T1_MAT_FAR_INFRARED;
            T1MR1 = Ir_Ctl[IR_TX].Mat_Val;          //下次时刻加载,为下一位做准备
        }
        else                                        //1个字节发送完毕
        {
            PWM1PCR = 0;                            //PWM控制寄存器:禁能PWM1输出
            Ir_Ctl[IR_TX].Dat_Tmp = 0;              //数据清零
            Ir_Ctl[IR_TX].Bit_Cnt = 11;             //位数计数器还原
            Ir_Ctl[IR_TX].Dat_Len--;                //减计数
        }
    }
}

/******************************************************************************
** 函数名称: void far_infrared_rx_timeout(void)
** 函数功能: 远红外通信口字节接收超时处理功能函数
** 入口参数: 无
** 返 回 值: 无
**
** 说    明: 在TIMERx的字节接收超时定时匹配中断处调用
******************************************************************************/
void far_infrared_rx_timeout (void)
{
    U8 cnt,mem_type;
    U8 *mem_ptr;
    U32 mr_val;

    cnt = Ir_Rxbyte_Timeout_Cnt[0] + 256 - Ir_Rxbyte_Timeout_Cnt[1];    //超时期间接收的字节数
    if(cnt >= 2)                //未出现超时
    {
        mr_val = T1TC + FAR_INFRARED_RX_BYTE_OVERTIME;
        T1MR2 = mr_val;         //刷新字节接收超时匹配值
    }
    else                        //超时，则认为当前帧接收任务全部完成
    {
        T1MCR &= ~(1 << 6);                     //禁止MR2匹配产生中断
        Ir_Ctl[IR_RX].Dat_Buf[IR_FI] = Ir_Ctl[IR_RX].Dat_Len;//保存当前接收数据总长度
        Ir_Ctl[IR_RX].Dat_Len = 0;              //复位
        Ir_Ctl[IR_RX].Dat_Buf[IR_LS]++;         //令牌信息
//发送红外接收数据到默认解析邮箱        
        if(Ir_Ctl[IR_RX].Dat_Buf[IR_FI] >= 5)
        {
            mem_ptr = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR28);
            if(mem_ptr != NULL)
            {
                mem_ptr[DATA_LEN_OFFSET] = Ir_Ctl[IR_RX].Dat_Buf[0];
                mem_ptr[DATA_LEN_OFFSET + 1] = 0;           //设置长度 长度为单字节 高字节填零
                mem_ptr[COM_NUM_OFFSET] = IR_SOFT_CHL;      //设置串口号
                memcpy(mem_ptr+DATA_OFFSET, Ir_Ctl[IR_RX].Dat_Buf+1, Ir_Ctl[IR_RX].Dat_Buf[0]);
     //           mem_ptr = _alloc_box(Mem_256_Byte);//Ir_Ctl[IR_RX].Dat_Buf;
                isr_mbx_send(Mbx_Comm_Data, (void*)mem_ptr);
            }
            else
            {
            	setmemgetnullid(GET_MEM_ERR0, gusCurMemErrFunName);
            }	
        }            
        Ir_Ctl[IR_RX].Dat_Buf[IR_FI] =  0;//
    }
    Ir_Rxbyte_Timeout_Cnt[1] = Ir_Rxbyte_Timeout_Cnt[0];    //计数器同步
}

/******************************************************************************
** 函数名称: void far_infrared_tx_timing(void)
** 函数功能: 远红外通信口字节发送定时处理功能函数
** 入口参数: 无
** 返 回 值: 无
**
** 说    明: 在TIMERx的字节接收超时定时匹配中断处调用
******************************************************************************/
void far_infrared_tx_timing (void)
{
    if(Ir_Ctl[IR_TX].Dat_Len)                       //若数据尚未发完
    {
        T1MR1 = T1TC + T1_MAT_FAR_INFRARED;         //加载匹配值
        T1MCR |= 1 << 3;                            //开启MR1匹配产生中断[作远红外发送通信位宽定时]

        T1MR3 = T1TC + FAR_INFRARED_TX_BYTE_TIMING; //加载匹配值
        T1MCR |= 1 << 9;                            //开启MR3匹配产生中断[作远红外发送字节间隔定时]
    }
    else                                            //发送缓冲区发送完毕
    {
        PWM1TCR = (1 << 1);                         //复位(关闭PWM模式及禁能PWM定时器)
        PWM1PCR = 0;                                //PWM控制寄存器:禁能PWM1输出
        T1MCR &= ~(1 << 3);                         //禁止MR1匹配产生中断
        T1MCR &= ~(1 << 9);                         //禁止MR3匹配产生中断

        IO2IntClr = P2_IRRXD;                       //冗余清零[很重要]
        IO2IntEnf |= P2_IRRXD;                      //发送任务完毕立即恢复P2_IRRXD(p2.11)下降沿输入IO中断
//----------------------恢复GPIO模式并输出低--------------------------------------------        
        PINSEL3 &= ~(0x03u << 4);                   //先清除相关bits
        PINSEL3 |= (P1_18_GPIO << 4);               //PWM1[1]空闲时配置成GPIO,并输出高以节省功耗
        SET_PORT1_OUT(P1_PWM_38K);                  //输出口
        SET_PORT1_LOW(P1_PWM_38K);                  //输出低(减少能耗)
    }
}
#endif

/******************************************************************************
    End Of File
******************************************************************************/

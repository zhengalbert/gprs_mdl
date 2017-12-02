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
   1、     1.00       2011-07-03     azh            create
******************************************************************************/
/*---------------------------------------------------------------------------*/
#include    "..\inc\global_config.h"

//-------------------------------- 常数定义 -----------------------------------

//------------------------------ 私有变量声明 ---------------------------------

//------------------------------ 私有函数声明 ---------------------------------

/******************************************************************************
** 函数名称: void key_init(void)
** 函数功能: 按键初始化(带IO中断功能)
** 入口参数: 无
** 返 回 值: 无
** 说    明:
******************************************************************************/
void key_init (void)
{
//  U8 i;

    PCONP |= 1 << PCGPIO;       //使能GPIO时钟

    if((PINSEL1 & ((0x03u << 6) | (0x03u << 8) | (0x03u << 10) | (0x03u << 12)))
        != ((P0_19_FNUC << 6) | (P0_20_FNUC << 8) | (P0_21_FNUC << 10) | (P0_22_FNUC << 12)))
    {
        PINSEL1 &= ~((0x03u << 6) | (0x03u << 8) | (0x03u << 10) | (0x03u << 12));  //先清除相关bits
        PINSEL1 |= ((P0_19_FNUC << 6) | (P0_20_FNUC << 8) | (P0_21_FNUC << 10) | (P0_22_FNUC << 12));     //GPIO
    }
    if((PINSEL4 & (0x03u << 26)) != (P2_13_FNUC << 26))
    {
        PINSEL4 &= ~(0x03u << 26);  //先清除相关bits
        PINSEL4 |= (P2_13_FNUC << 26);     //GPIO
    }
    //按键都配置成上下沿中断
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不是CPU_GPRS模块
    IO0IntEnr |= (P0_KEYPRG | P0_KEYG | P0_KEYD | P0_KEYUP | P0_KEYDOWN);
    IO0IntEnf |= (P0_KEYPRG | P0_KEYG | P0_KEYD | P0_KEYUP | P0_KEYDOWN);

    //清除中断标志
    IO0IntClr = (P0_KEYPRG | P0_KEYG | P0_KEYD | P0_KEYUP | P0_KEYDOWN);
#endif
////////debug
    FIO0DIR &= ~(P0_PD_CHK);//配置为输入

    //检测引脚配置成上下沿中断
//    IO0IntEnr |= P0_PD_CHK;
    IO0IntEnf |= P0_PD_CHK;
////////debug

    EXTINT = 0x0F;                  //外部中断总标识清零
//处理编程键
    Prog_Ctrl.pre_state = CON_KEYBIT_NO;
    Prog_Ctrl.hold_time = 0;
    Prog_Ctrl.dly_cnt = 0;

    Key_CurState = 0;
}
/******************************************************************************
** 函数名称: void key_init(void)
** 函数功能: IO中断处理
** 入口参数: 无
** 返 回 值: 无
** 说    明:
******************************************************************************/
void EINT3_IRQHandler (void)
{
    U32 pin;

    if(IOIntStatus & 0x01)  //IO口0中断
    {
        pin = IO0IntStatf;

#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不是CPU_GPRS模块
        if(pin & P0_KEYUP)
        {
            if(0 == PORT0_IS_HIGH(P0_KEYUP))
            {
            	Key_CurState |= CON_KEYBIT_UP;
            }
        }
        if(pin & P0_KEYPRG)
        {
            if(0 == PORT0_IS_HIGH(P0_KEYPRG))
            {
            	Key_CurState |= CON_KEYBIT_PG;
            }
        }
        if(pin & P0_KEYG)
        {
            if(0 == PORT0_IS_HIGH(P0_KEYG))
            {
            	Key_CurState |= CON_KEYBIT_UC;
            }
        }
        if(pin & P0_KEYD)
        {
            if(0 == PORT0_IS_HIGH(P0_KEYD))
            {
            	Key_CurState |= CON_KEYBIT_DC;
            }
        }

        if(pin & P0_KEYDOWN)
        {
            if(0 == PORT0_IS_HIGH(P0_KEYDOWN))
            {
            	Key_CurState |= CON_KEYBIT_DN;
            }
        }
#endif
///////////////debug
        if(pin & P0_PD_CHK)
        {
//            cnt = 100;
//            while(cnt--);
//            if(!PORT0_IS_HIGH(P0_PD_CHK))           //认为掉电发生
//            {
//                              sys_sleep();
//            }
        }
///////////////debug

        IO0IntClr = pin;
    //
        pin = IO0IntStatr;
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不是CPU_GPRS模块
        if(pin & P0_KEYUP)
        {
            Key_CurState &= ~CON_KEYBIT_UP;
        }
        if(pin & P0_KEYPRG)
        {
            Key_CurState &= ~CON_KEYBIT_PG;
        }
        if(pin & P0_KEYG)
        {
            Key_CurState &= ~CON_KEYBIT_UC;
        }
        if(pin & P0_KEYD)
        {
            Key_CurState &= ~CON_KEYBIT_DC;
        }

        if(pin & P0_KEYDOWN)
        {
            Key_CurState &= ~CON_KEYBIT_DN;
        }
#endif
        IO0IntClr = pin;
    }
    else if(IOIntStatus & 0x04)   //IO口2中断
    {
        pin = IO2IntStatf;

//---------------------------------------远红外接收数据中断------------------------------
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不是CPU_GPRS模块
        if((pin & P2_IRRXD) && (!PORT1_IS_HIGH(P1_IR_CTL)))//远红外接收数据中断(p2.11下降沿:即起始位触发并排除IRVDD被关闭产生影响)
        {
            //禁止P2_IRRXD边沿中断触发,目的是此处仅作为一次数据接收的触发入口,把数据接收任务移交给TIMER位宽采样中断处理
            IO2IntEnf &= ~P2_IRRXD;
            Ir_Ctl[IR_RX].Mat_Val = (T1TC + (T1_MAT_FAR_INFRARED >> 2));//检测起始位,在1/4位宽处采样(TIMER中断里要恢复位宽定时配置)
            T1MR0 = Ir_Ctl[IR_RX].Mat_Val;
            T1MCR |= (1 << 0);                      //开启MR0匹配产生中断[作远红外接收通信位宽定时]
//            SET_PORT1_REV(P1_GNET_LED);             //debug,查看采样时刻
        }
#endif        
//---------------------------------------远红外接收数据中断------------------------------
        IO2IntClr = pin;                //统一清中断

        pin = IO2IntStatr;

        IO2IntClr = pin;
    }
    EXTINT &= 0x08;

}
/******************************************************************************
** 函数名称: U8 prog_key_enable(void)
** 函数功能: 检查编程按键
** 入口参数: 无
** 返 回 值: 无
** 说    明:
******************************************************************************/
U8 prog_key_enable (void)
{
    U8 value = __FALSE;

    if(LOCAL_STATE_IS_HIG(CON_LOCAL_STATE_PROG))
    {
        value = __TRUE;
    }
    return value;
}
/******************************************************************************
** 函数名称: U8 check_prog_key_state(void)
** 函数功能: 检查编程按键
** 入口参数: 无
** 返 回 值: value 1:编程状态有变化 要实时去刷新LCD
** 说    明:
******************************************************************************/
U8 check_prog_key_state (void)
{
    U8 value = __FALSE;

    if(Key_CurState & CON_KEYBIT_PG)//按下
    {
        if(Prog_Ctrl.pre_state == CON_KEYBIT_NO)
        {
            if(LOCAL_STATE_IS_HIG(CON_LOCAL_STATE_PROG))
            {
                CLR_LOCAL_STATE(CON_LOCAL_STATE_PROG);
                Prog_Ctrl.hold_time = 0;
                value = __TRUE;
            }
            else
            {
                SET_LOCAL_STATE(CON_LOCAL_STATE_PROG);
                Prog_Ctrl.hold_time = COM_PROG_HOLD_TIME;
                value = __TRUE;
            }
        }
        Prog_Ctrl.pre_state = CON_KEYBIT_DN;
    }
    else//松开
    {
        Prog_Ctrl.pre_state = CON_KEYBIT_NO;
    }
//处理编程键自动保持时间15分钟
    if(LOCAL_STATE_IS_HIG(CON_LOCAL_STATE_PROG))
    {
        if(Prog_Ctrl.hold_time > COM_PROG_HOLD_TIME)
        {
            Prog_Ctrl.hold_time = COM_PROG_HOLD_TIME;
        }
        if(Prog_Ctrl.hold_time > 0)
        {
            Prog_Ctrl.hold_time--;
        }
        else
        {
            CLR_LOCAL_STATE(CON_LOCAL_STATE_PROG);
            value = __TRUE;
        }
    }
    return value;
}
/******************************************************************************
    End Of File
******************************************************************************/

/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : interface.c
 Description    : 用户编写的环境接口函数,必须在特权模式运行
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、    1.10        2011-01-13     wwllzz         关闭不使用的代码
   1、    1.00        2010-07-19      snap           the original version
******************************************************************************/
#include "..\inc\global_config.h"

/******************************************************************************
** 函数名称: void chip_reset(U8 mode)
** 函数功能: 复位系统
** 入口参数: mode: POWER_RESET: 上电复位
**                 HARD_RESET:  硬件复位
**                 SOFT_RESET:  软件复位
**                 其它:        与系统相关
** 返 回 值: 无
** 说    明:
******************************************************************************/
void chip_reset (U8 mode)
{

    switch(mode)
    {
    case POWER_RESET:                                   //此系统上电复位等同硬件复位
    case HARD_RESET:
        AIRCR = (0x05fa << 16) + (PRIO_GROUP_CONFIG << 8) + 4;  //x优先级组
        break;
    case SOFT_RESET:
        AIRCR = (0x05fa << 16) + (PRIO_GROUP_CONFIG << 8) + 1;  //x优先级组
        break;
    default:                                            //参数不正确不复位
        break;
    }

}

//#ifndef CLOSE_INTERFACE_NO_USE_CODE
/******************************************************************************
** 函数名称: __asm S32 irq_disable(void)
** 函数功能: 禁止中断
** 入口参数: 无
** 返 回 值: 成功: __SUCCEED
**           错误: 负数
** 说    明:
******************************************************************************/
__asm S32 irq_disable (void)
{
    CPSID   I
    MOV     R0, #0
    BX      LR
}

/******************************************************************************
** 函数名称: __asm S32 irq_enable(void)
** 函数功能: 允许中断
** 入口参数: 无
** 返 回 值: 成功: __SUCCEED
**           错误: 负数
** 说    明:
******************************************************************************/
__asm S32 irq_enable (void)
{
    CPSIE   I
    MOV     R0, #0
    BX      LR
}

/******************************************************************************
** 函数名称: S32 isr_set(U32 channel, U32 prio)
** 函数功能: IO系统设置中断服务程序
** 入口参数: channel:  中断通道号
**           prio:     中断优先级
** 返 回 值: 成功: __SUCCEED
**           错误: __FAIL
** 说    明: 中断服务程序地址都使用同弱定义相同的名字
******************************************************************************/
//S32 isr_set (U32 channel, U32 prio)
S32 __SVC_1(U32 channel, U32 prio)
{
    U32 tmp1, tmp2, tmp3;

    if(channel > MAX_NVIC)
    {
        return __FAIL;
    }

    irq_disable();

    if(channel >= 16)
    {
        tmp3 = channel - 16;
        tmp1 = tmp3 / 32;
        tmp2 = tmp3 % 32;

        ((U32 *)0xE000E100)[tmp1] = 1ul << tmp2;
        ((U8 *)0xE000E400)[tmp3] = prio;
    }
    else
    {
        switch(channel)
        {
        case MMI:
            SHCSR = SHCSR | (1 << 16);
            break;
        case BFI:
            SHCSR = SHCSR | (1 << 17);
            break;
        case UFI:
            SHCSR = SHCSR | (1 << 18);
            break;
        default:
            break;
        }

        if(channel >= MMI)
        {
            tmp3 = channel - MMI;
            ((U8 *)0xE000ED18)[tmp3] = prio;
        }
    }

    irq_enable();
    return __SUCCEED;
}
/******************************************************************************
** 函数名称: void mode_shift (void)
** 函数功能: IO系统设置中断服务程序
** 入口参数:
**
** 返 回 值:
**
** 说    明: 中断服务程序地址都使用同弱定义相同的名字
******************************************************************************/
//void mode_shift (void)
void __SVC_3(void)
{
    irq_disable();



    irq_enable();

}
/******************************************************************************
函数: void __swi(4) disable_irq(void)
作者: wwllzz
概述: 进入特权模式, 执行关闭irq中断功能, 不能在中断服务程序内调用
输入: 无
输出: 无
******************************************************************************/
void __SVC_4 (void)
{
    __disable_irq();
}
/******************************************************************************
函数: void __swi(5) enable_irq(void)
作者: wwllzz
概述: 进入特权模式, 执行使能irq中断功能, 不能在中断服务程序内调用
输入: 无
输出: 无
******************************************************************************/
//void __SVC_5 (void)
//{
//    __enable_irq();
//}
//__EXTERN void __swi(5) iap_programe(U32 param_tab[], U32 result_tab[]);
void __SVC_5 (U32 param_tab[], U32 result_tab[])
{
    void (*iap)(U32 [], U32 []);
    iap = (void (*)(U32 [], U32 []))IAP_LOCATION;

    __disable_irq();
    iap(param_tab, result_tab);
    __enable_irq();
}
/******************************************************************************
函数: void __swi(6) enable_user_int(void)
作者: wwllzz
概述: 进入特权模式, 执行使能用户配置的中断
输入: 无
输出: 无
******************************************************************************/
//void __SVC_6 (void)
//{
//    enable_modules_int();
//}
/******************************************************************************
** 函数名称: S32 isr_clr(U32 channel)
** 函数功能: IO系统清除中断服务程序
** 入口参数: channel: 中断通道号
** 返 回 值: 成功: __SUCCEED
**           错误: __FAIL
** 说    明:
******************************************************************************/
//S32 isr_clr (U32 channel)
S32 __SVC_2(U32 channel)
//S32 __SVC_2()
{
    U32 tmp1, tmp2, tmp3;

    if(channel > MAX_NVIC)
    {
        return __FAIL;
    }

    irq_disable();

    if(channel >= 16)
    {
        tmp3 = channel - 16;
        tmp1 = tmp3 / 32;
        tmp2 = tmp3 % 32;

        ((U32 *)0xE000E180)[tmp1] = 1ul << tmp2;
        ((U8 *)0xE000E400)[tmp3] = 0;
    }
    else
    {
        switch(channel)
        {
        case MMI:
            SHCSR = SHCSR & ~(1 << 16);
            break;
        case BFI:
            SHCSR = SHCSR & ~(1 << 17);
            break;
        case UFI:
            SHCSR = SHCSR & ~(1 << 18);
            break;
        default:
            break;
        }

        if(channel >= MMI)
        {
            tmp3 = channel - MMI;
            ((U8 *)0xE000ED18)[tmp3] = 0;
        }
    }

    irq_enable();
    return __SUCCEED;
}

/******************************************************************************
** 函数名称: S32 isr_disable(U32 channel)
** 函数功能: 禁止指定中断
** 入口参数: channel: 中断通道号
** 返 回 值: 成功: __SUCCEED
**           错误: __FAIL
** 说    明:
******************************************************************************/
S32 isr_disable (U32 channel)
{
    U32 tmp1, tmp2, tmp3;

    if(channel > MAX_NVIC)
    {
        return __FAIL;
    }

//    if(channel < 16)                                  //在用户模式下应打开这段
//    {
//        return __FAIL;
//    }

    irq_disable();

    if(channel >= 16)
    {
        tmp3 = channel - 16;
        tmp1 = tmp3 / 32;
        tmp2 = tmp3 % 32;

        ((U32 *)0xE000E180)[tmp1] = 1ul << tmp2;
    }
    else
    {
        switch(channel)
        {
        case MMI:
            SHCSR = SHCSR & ~(1 << 16);
            break;
        case BFI:
            SHCSR = SHCSR & ~(1 << 17);
            break;
        case UFI:
            SHCSR = SHCSR & ~(1 << 18);
            break;
        default:
            break;
        }
    }

    irq_enable();
    return __SUCCEED;
}

/******************************************************************************
** 函数名称: S32 isr_enable(U32 channel)
** 函数功能: 允许指定中断
** 入口参数: channel: 中断通道号
** 返 回 值: 成功: __SUCCEED
**           错误: __FAIL
** 说    明:
******************************************************************************/
S32 isr_enable (U32 channel)
{
    U32 tmp1, tmp2, tmp3;

    if(channel > MAX_NVIC)
    {
        return __FAIL;
    }

    irq_disable();

    if(channel >= 16)
    {
        tmp3 = channel - 16;
        tmp1 = tmp3 / 32;
        tmp2 = tmp3 % 32;
        ((U32 *)0xE000E100)[tmp1] = 1ul << tmp2;

    }
    else
    {
        switch(channel)
        {
        case MMI:
            SHCSR = SHCSR | (1 << 16);
            break;
        case BFI:
            SHCSR = SHCSR | (1 << 17);
            break;
        case UFI:
            SHCSR = SHCSR | (1 << 18);
            break;
        default:
            break;
        }
    }

    irq_enable();
    return __SUCCEED;
}
//#endif  //CLOSE_INTERFACE_NO_USE_CODE
/******************************************************************************
** 函数名称: U32 __SVC_6(U8 id)
** 函数功能: 软件中断进入管理模式下的一些操作
** 入口参数: id:  要读的变量id号 自定义
** 返 回 值: 地址值
** 说    明: 暂时用于升级时读一个地址
******************************************************************************/
//U32 operation_at_admin(U8 id)
U32 __SVC_6(U8 id)
{
    U32 tmp=0;

    switch(id)
    {
        case GET_RUN_ADDR_ADMIN:
            tmp = VTOR;
            break;
        case SOFT_RST_ADMIN:
        {
            __disable_irq();
            STCTRL = 0x04;                                      //关闭STI
            CLRENA0 = 0xffff8000;                               //STI以上都关闭
            ((U32 *)0xE000E180)[1] = 0xffffffff;
            ((U32 *)0xE000E180)[2] = 0xffffffff;
            ((U32 *)0xE000E180)[3] = 0xffffffff;
            __enable_irq();                                     //开中断, 已没有向量中断了
            __set_FAULTMASK(1);
            tmp = RSID;                                         //在此之前主程序应已记录并分析复位原因
            RSID = tmp & 0x0f;                                  //清复位原因的CPU寄存器
            chip_reset(SOFT_RESET);
        }
            break;
        case 2:
            break;
        default:
            break;
    }

    return tmp;
}
/******************************************************************************
    End Of File
******************************************************************************/

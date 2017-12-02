/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : interface.c
 Description    : �û���д�Ļ����ӿں���,��������Ȩģʽ����
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��    1.10        2011-01-13     wwllzz         �رղ�ʹ�õĴ���
   1��    1.00        2010-07-19      snap           the original version
******************************************************************************/
#include "..\inc\global_config.h"

/******************************************************************************
** ��������: void chip_reset(U8 mode)
** ��������: ��λϵͳ
** ��ڲ���: mode: POWER_RESET: �ϵ縴λ
**                 HARD_RESET:  Ӳ����λ
**                 SOFT_RESET:  �����λ
**                 ����:        ��ϵͳ���
** �� �� ֵ: ��
** ˵    ��:
******************************************************************************/
void chip_reset (U8 mode)
{

    switch(mode)
    {
    case POWER_RESET:                                   //��ϵͳ�ϵ縴λ��ͬӲ����λ
    case HARD_RESET:
        AIRCR = (0x05fa << 16) + (PRIO_GROUP_CONFIG << 8) + 4;  //x���ȼ���
        break;
    case SOFT_RESET:
        AIRCR = (0x05fa << 16) + (PRIO_GROUP_CONFIG << 8) + 1;  //x���ȼ���
        break;
    default:                                            //��������ȷ����λ
        break;
    }

}

//#ifndef CLOSE_INTERFACE_NO_USE_CODE
/******************************************************************************
** ��������: __asm S32 irq_disable(void)
** ��������: ��ֹ�ж�
** ��ڲ���: ��
** �� �� ֵ: �ɹ�: __SUCCEED
**           ����: ����
** ˵    ��:
******************************************************************************/
__asm S32 irq_disable (void)
{
    CPSID   I
    MOV     R0, #0
    BX      LR
}

/******************************************************************************
** ��������: __asm S32 irq_enable(void)
** ��������: �����ж�
** ��ڲ���: ��
** �� �� ֵ: �ɹ�: __SUCCEED
**           ����: ����
** ˵    ��:
******************************************************************************/
__asm S32 irq_enable (void)
{
    CPSIE   I
    MOV     R0, #0
    BX      LR
}

/******************************************************************************
** ��������: S32 isr_set(U32 channel, U32 prio)
** ��������: IOϵͳ�����жϷ������
** ��ڲ���: channel:  �ж�ͨ����
**           prio:     �ж����ȼ�
** �� �� ֵ: �ɹ�: __SUCCEED
**           ����: __FAIL
** ˵    ��: �жϷ�������ַ��ʹ��ͬ��������ͬ������
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
** ��������: void mode_shift (void)
** ��������: IOϵͳ�����жϷ������
** ��ڲ���:
**
** �� �� ֵ:
**
** ˵    ��: �жϷ�������ַ��ʹ��ͬ��������ͬ������
******************************************************************************/
//void mode_shift (void)
void __SVC_3(void)
{
    irq_disable();



    irq_enable();

}
/******************************************************************************
����: void __swi(4) disable_irq(void)
����: wwllzz
����: ������Ȩģʽ, ִ�йر�irq�жϹ���, �������жϷ�������ڵ���
����: ��
���: ��
******************************************************************************/
void __SVC_4 (void)
{
    __disable_irq();
}
/******************************************************************************
����: void __swi(5) enable_irq(void)
����: wwllzz
����: ������Ȩģʽ, ִ��ʹ��irq�жϹ���, �������жϷ�������ڵ���
����: ��
���: ��
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
����: void __swi(6) enable_user_int(void)
����: wwllzz
����: ������Ȩģʽ, ִ��ʹ���û����õ��ж�
����: ��
���: ��
******************************************************************************/
//void __SVC_6 (void)
//{
//    enable_modules_int();
//}
/******************************************************************************
** ��������: S32 isr_clr(U32 channel)
** ��������: IOϵͳ����жϷ������
** ��ڲ���: channel: �ж�ͨ����
** �� �� ֵ: �ɹ�: __SUCCEED
**           ����: __FAIL
** ˵    ��:
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
** ��������: S32 isr_disable(U32 channel)
** ��������: ��ָֹ���ж�
** ��ڲ���: channel: �ж�ͨ����
** �� �� ֵ: �ɹ�: __SUCCEED
**           ����: __FAIL
** ˵    ��:
******************************************************************************/
S32 isr_disable (U32 channel)
{
    U32 tmp1, tmp2, tmp3;

    if(channel > MAX_NVIC)
    {
        return __FAIL;
    }

//    if(channel < 16)                                  //���û�ģʽ��Ӧ�����
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
** ��������: S32 isr_enable(U32 channel)
** ��������: ����ָ���ж�
** ��ڲ���: channel: �ж�ͨ����
** �� �� ֵ: �ɹ�: __SUCCEED
**           ����: __FAIL
** ˵    ��:
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
** ��������: U32 __SVC_6(U8 id)
** ��������: ����жϽ������ģʽ�µ�һЩ����
** ��ڲ���: id:  Ҫ���ı���id�� �Զ���
** �� �� ֵ: ��ֵַ
** ˵    ��: ��ʱ��������ʱ��һ����ַ
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
            STCTRL = 0x04;                                      //�ر�STI
            CLRENA0 = 0xffff8000;                               //STI���϶��ر�
            ((U32 *)0xE000E180)[1] = 0xffffffff;
            ((U32 *)0xE000E180)[2] = 0xffffffff;
            ((U32 *)0xE000E180)[3] = 0xffffffff;
            __enable_irq();                                     //���ж�, ��û�������ж���
            __set_FAULTMASK(1);
            tmp = RSID;                                         //�ڴ�֮ǰ������Ӧ�Ѽ�¼��������λԭ��
            RSID = tmp & 0x0f;                                  //�帴λԭ���CPU�Ĵ���
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

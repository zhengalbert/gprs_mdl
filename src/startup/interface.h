/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : interface.h
 Description    : �����ӿں�����ͷ�ļ�
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��    1.00        2010-07-19      snap           the original version
******************************************************************************/

#ifndef __INTERFACE_H
#define __INTERFACE_H

//----------------------------- macro definition ------------------------------
//#define CLOSE_INTERFACE_NO_USE_CODE                     //���ٱ���Դ���ļ��в�ʹ�õĺ���

#define MAX_NVIC        51                              //�쳣��������Ŀ

//-----------------------------------------------------------------------------
// ��λģʽ����
//-----------------------------------------------------------------------------
#define POWER_RESET     0                               //�ϵ縴λ
#define HARD_RESET      1                               //Ӳ����λ
#define SOFT_RESET      2                               //�����λ

//----------------------------- type definition -------------------------------

//-------------------------- functions declaration ----------------------------
__EXTERN void chip_reset(U8 mode);

#ifndef CLOSE_INTERFACE_NO_USE_CODE
//__EXTERN S32 isr_set(U32 channel, U32 prio);
//__EXTERN S32 isr_clr(U32 channel);
__EXTERN S32 __swi(1) isr_set(U32 channel, U32 prio);
__EXTERN S32 __swi(2) isr_clr(U32 channel);
__EXTERN void __swi(3) power_mode_shift(void);
__EXTERN void __swi(4) disable_irq(void);
//__EXTERN void __swi(5) enable_irq(void);      //����ģʽ�¹��ж� ������Ͳ���ʹ���ж� �����ڹ���ģʽ��ֱ��ʹ��
__EXTERN void __swi(5) iap_programe(U32 param_tab[], U32 result_tab[]);
//__EXTERN void __swi(6) enable_user_int(void);
__EXTERN U32 __swi(6) operation_at_admin(U8 id);

//__EXTERN S32 __swi(2) isr_clr(U32);

__EXTERN S32 isr_disable(U32 channel);
__EXTERN S32 isr_enable(U32 channel);
__EXTERN __asm S32 irq_disable(void);
__EXTERN __asm S32 irq_enable(void);
#endif  //CLOSE_INTERFACE_NO_USE_CODE

//--------------------------- variable declaration ----------------------------

#endif//__INTERFACE_H

/******************************************************************************
    End Of File
******************************************************************************/

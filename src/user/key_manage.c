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
   1��     1.00       2011-07-03     azh            create
******************************************************************************/
/*---------------------------------------------------------------------------*/
#include    "..\inc\global_config.h"

//-------------------------------- �������� -----------------------------------

//------------------------------ ˽�б������� ---------------------------------

//------------------------------ ˽�к������� ---------------------------------

/******************************************************************************
** ��������: void key_init(void)
** ��������: ������ʼ��(��IO�жϹ���)
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��:
******************************************************************************/
void key_init (void)
{
//  U8 i;

    PCONP |= 1 << PCGPIO;       //ʹ��GPIOʱ��

    if((PINSEL1 & ((0x03u << 6) | (0x03u << 8) | (0x03u << 10) | (0x03u << 12)))
        != ((P0_19_FNUC << 6) | (P0_20_FNUC << 8) | (P0_21_FNUC << 10) | (P0_22_FNUC << 12)))
    {
        PINSEL1 &= ~((0x03u << 6) | (0x03u << 8) | (0x03u << 10) | (0x03u << 12));  //��������bits
        PINSEL1 |= ((P0_19_FNUC << 6) | (P0_20_FNUC << 8) | (P0_21_FNUC << 10) | (P0_22_FNUC << 12));     //GPIO
    }
    if((PINSEL4 & (0x03u << 26)) != (P2_13_FNUC << 26))
    {
        PINSEL4 &= ~(0x03u << 26);  //��������bits
        PINSEL4 |= (P2_13_FNUC << 26);     //GPIO
    }
    //���������ó��������ж�
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//����CPU_GPRSģ��
    IO0IntEnr |= (P0_KEYPRG | P0_KEYG | P0_KEYD | P0_KEYUP | P0_KEYDOWN);
    IO0IntEnf |= (P0_KEYPRG | P0_KEYG | P0_KEYD | P0_KEYUP | P0_KEYDOWN);

    //����жϱ�־
    IO0IntClr = (P0_KEYPRG | P0_KEYG | P0_KEYD | P0_KEYUP | P0_KEYDOWN);
#endif
////////debug
    FIO0DIR &= ~(P0_PD_CHK);//����Ϊ����

    //����������ó��������ж�
//    IO0IntEnr |= P0_PD_CHK;
    IO0IntEnf |= P0_PD_CHK;
////////debug

    EXTINT = 0x0F;                  //�ⲿ�ж��ܱ�ʶ����
//�����̼�
    Prog_Ctrl.pre_state = CON_KEYBIT_NO;
    Prog_Ctrl.hold_time = 0;
    Prog_Ctrl.dly_cnt = 0;

    Key_CurState = 0;
}
/******************************************************************************
** ��������: void key_init(void)
** ��������: IO�жϴ���
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��:
******************************************************************************/
void EINT3_IRQHandler (void)
{
    U32 pin;

    if(IOIntStatus & 0x01)  //IO��0�ж�
    {
        pin = IO0IntStatf;

#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//����CPU_GPRSģ��
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
//            if(!PORT0_IS_HIGH(P0_PD_CHK))           //��Ϊ���緢��
//            {
//                              sys_sleep();
//            }
        }
///////////////debug

        IO0IntClr = pin;
    //
        pin = IO0IntStatr;
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//����CPU_GPRSģ��
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
    else if(IOIntStatus & 0x04)   //IO��2�ж�
    {
        pin = IO2IntStatf;

//---------------------------------------Զ������������ж�------------------------------
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//����CPU_GPRSģ��
        if((pin & P2_IRRXD) && (!PORT1_IS_HIGH(P1_IR_CTL)))//Զ������������ж�(p2.11�½���:����ʼλ�������ų�IRVDD���رղ���Ӱ��)
        {
            //��ֹP2_IRRXD�����жϴ���,Ŀ���Ǵ˴�����Ϊһ�����ݽ��յĴ������,�����ݽ��������ƽ���TIMERλ������жϴ���
            IO2IntEnf &= ~P2_IRRXD;
            Ir_Ctl[IR_RX].Mat_Val = (T1TC + (T1_MAT_FAR_INFRARED >> 2));//�����ʼλ,��1/4λ������(TIMER�ж���Ҫ�ָ�λ��ʱ����)
            T1MR0 = Ir_Ctl[IR_RX].Mat_Val;
            T1MCR |= (1 << 0);                      //����MR0ƥ������ж�[��Զ�������ͨ��λ��ʱ]
//            SET_PORT1_REV(P1_GNET_LED);             //debug,�鿴����ʱ��
        }
#endif        
//---------------------------------------Զ������������ж�------------------------------
        IO2IntClr = pin;                //ͳһ���ж�

        pin = IO2IntStatr;

        IO2IntClr = pin;
    }
    EXTINT &= 0x08;

}
/******************************************************************************
** ��������: U8 prog_key_enable(void)
** ��������: ����̰���
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��:
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
** ��������: U8 check_prog_key_state(void)
** ��������: ����̰���
** ��ڲ���: ��
** �� �� ֵ: value 1:���״̬�б仯 Ҫʵʱȥˢ��LCD
** ˵    ��:
******************************************************************************/
U8 check_prog_key_state (void)
{
    U8 value = __FALSE;

    if(Key_CurState & CON_KEYBIT_PG)//����
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
    else//�ɿ�
    {
        Prog_Ctrl.pre_state = CON_KEYBIT_NO;
    }
//�����̼��Զ�����ʱ��15����
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

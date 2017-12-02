/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : pd_manage.c
 Description    : �������г��������õ��ĺ���
-------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��    1.00        2011-08-20    azh          the original version
******************************************************************************/
#include    "..\inc\global_config.h"

#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//��CPU_GPRSģ��
void deep_pd_loop (void)
{
}
#else
#define __WFI               __wfi                       //˯�ߣ��ȴ��ж�ָ��

//------------------------------ ˽�б������� ---------------------------------
U8 pd_cnt=0;
//------------------------------ ˽�к������� -------------------------------
static void pd_bat_on(void);
/*********************************************************************************************************
** Function name:       void pd_mode_ctrl(U8 mode)
** Descriptions:        ����ϵͳ���ʿ���ģʽ
** input parameters:    mode: 0 - ʹϵͳ������ͨ˯��ģʽ;
**                                  1 - ʹϵͳ�������˯��ģʽ;
**                                  2 - ʹϵͳ������ͨ����ģʽ;
**                                  3 - ʹϵͳ������ȵ���ģʽ��
** output parameters:   none
** Returned value:      1:  �ɹ�
**                      0:  ʧ��
*********************************************************************************************************/
void pd_mode_ctrl (U8 mode)
{
    if (mode == 0){                                             /*  ������ͨ˯��ģʽ            */
        PCON = 0x00;
        SCR  = 0x00;
    } else {
        SCR  |= 0x04;
        if (mode == 1){                                         /*  �������˯��ģʽ            */
            PCON = 0x00;
        }
        if (mode == 2){                                         /*  �������ģʽ                */
            PCON = 0x01;
        }
        if (mode == 3){                                         /*  ������ȵ���ģʽ            */
            PCON = 0x03;
        }
    }
    //isrWait();                                                          /*  �ȴ��жϻ���              */
    __WFI();
}
/******************************************************************************
** ��������: void rtc_ain_init(void)
** ��������: Ƭ��RTC�������жϳ�ʼ�����ܺ���
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void rtc_ain_init (void)
{
    PCONP |= (1 << PCRTC);                  //ʹ��Ƭ��RTCʱ��

    RTCCCR  = 0x10;                         //��ֹʱ�������
    RTCILR  = 0x03;                         //����ж�
    RTCCIIR = 0x01;                       //��ʹ���������ж�
//    RTCCIIR = 0x02;                         //��ʹ�ܷ������ж�
    RTCAMR = 0xff;                          //��ֹ����
    RTC_AUX = 0x10;                         //�������ͣ��ı�־λ
    RTC_AUXEN = 0x0;                        //��ֹ����ͣ���ж�
    RTCCCR  = 0x11;                         //����RTC
}
/******************************************************************************
** ��������: void RTC_IRQHandler(void)
** ��������: Ƭ��RTC�������жϴ����ܺ���
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void RTC_IRQHandler (void)
{
    RTCILR = 0x03;                          // ����жϱ�־
//    if(PCON & DPDFLAG)                  //��CPU����ȵ���ģʽ�£���ִ�л�������
//    {
//        cpu_wake_up();
//    }
//    SET_PORT1_REV(P1_DBG_LED);//debug

//    SET_PORT1_REV(P1_GNET_LED);//P1_GDATA_LED
//    pd_cnt++;
    RTCGPREG3++;                            //���繤����ʱ��ο�
}
/******************************************************************************
** ��������: U8 is_pd_mode(void)
** ��������: �ж��Ƿ��ڵ���ģʽ������ ��Ҫ����Щ����Ҫ�õ�RTXϵͳ����Դ ��������
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
//U8 is_pd_mode (void)
//{
//    /////////////debug!!!
////  return __TRUE;
//
////return __FALSE;
//
//    SET_PORT0_IN(P0_PD_CHK);            //����p0.29Ϊ�����
//
//    if(pd_mode_flag == CON_PD_MODE_YES)//�ǵ���ģʽ��־
//    {
//        if(PORT0_IS_HIGH(P0_PD_CHK))   //�������ź����롡������е�ġ����Ǵ�����Ǹ�����
//        {
//            __disable_irq();//����ϵͳ�¡�Ҫ���ù���ģʽ�µĺ���
//            while(1);
//        }
//        return __TRUE;
//    }
//    else
//    {
//        return __FALSE;
//    }
//}

/******************************************************************************
** ��������: void pd_enable_modules_int (void)
** ��������: �ڽ�������ǰʹ�ܸ�ģ���ж�
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void pd_enable_modules_int (void)
{
    ENABLE_ISR(NVIC_I2C0, PRIO_I2C0);       //set I2C0 interrupt parameter
    ENABLE_ISR(NVIC_I2C1, PRIO_I2C1);       //set I2C1 interrupt parameter

//    ENABLE_ISR(NVIC_UART0, PRIO_UART0);     //set UART0 interrupt parameter
//    ENABLE_ISR(NVIC_UART1, PRIO_UART1);     //set UART0 interrupt parameter
//    ENABLE_ISR(NVIC_UART2, PRIO_UART2);     //set UART0 interrupt parameter
//    ENABLE_ISR(NVIC_UART3, PRIO_UART3);     //set UART0 interrupt parameter

////    ENABLE_ISR(NVIC_SSP0, PRIO_SSP0);       //set SSP0 interrupt parameter
////    ENABLE_ISR(NVIC_SSP1, PRIO_SSP1);       //set SSP1 interrupt parameter

////    ENABLE_ISR(NVIC_TIMER1, PRIO_TIMER1);   //ʹ��TIMER1�ж�

//    ENABLE_ISR(NVIC_GP_DMA, PRIO_GPDMA);    //ʹ��DMA

//    ENABLE_ISR(NVIC_PWM1, PRIO_PWM1);       //ʹ��PWM1
//    ENABLE_ISR(NVIC_ADC, PRIO_ADC);         //ʹ��ADC
    ENABLE_ISR(NVIC_RTC, PRIO_RTC);         //ʹ��RTC
    ENABLE_ISR(NVIC_EINT3, PRIO_GPIO);      //ʹ��EINT3�ж�

}
/******************************************************************************
 Function name:  void pd_data_init(void)
 Author       :  azh
 Description  :  һЩ���ݵĳ�ʼ��

 Input        :  None
 Return       :  None
 *****************************************************************************/
//void pd_data_init (void)
//{
//    U8 i;
//
//    Require_Software_Reset = 0;
//    Power_Down_Flag = 0;
////    Global_Status_Word = 0;
//
//    gflash_ct = 0;
//
//    for(i=0; i<AD_MAX; i++)
//    {
//        Ad_Data[i] = 0;
//    }
//}
/******************************************************************************
** ��������: void pd_lcd_init(U8 mode)
** ��������: ��ʾ���Ƽ����ݳ�ʼ�� ��Ҫ��������� �жϼ��Ϊ1s
** ��ڲ���: �� mode=0:�ϵ��ʼ�� mode=1:�������ú����
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void pd_lcd_init (U8 mode)
{
/*
    U8 i;
    U8 rev_buff[5];

    file_read(PAR_LPDISCNT_04000301_00_ADDR, rev_buff, 5);//���ζ�ȡ����ѭ��&������ز�������μ�PAR_LPDISCNT_04000301_00_ADDR���崦
    for(i=0; i<2; i++)
    {
        Dis_Para.maxn[i] = bcd_to_hex(rev_buff[i * (PAR_KEYDIS_04000305_00_ADDR - PAR_LPDISCNT_04000301_00_ADDR)]);
        if(Dis_Para.maxn[i] == 0xff)//���������Чʱ,ȡĬ��ֵ
        {
            Dis_Para.maxn[i] = DEBUG_DIS_MAX_NUM;
        }
        Dis_Ctrl.dis_bsn[i] = 0;
        Dis_Ctrl.exd_lsn[i] = 0;
        Dis_Ctrl.lsn_maxn[i] = 1;
    }
    Dis_Para.dly[DIS_MODE_AUTO] = bcd_to_hex(rev_buff[1]);//
    Dis_Para.dly[DIS_MODE_SKEY] = bcd_to_hex(rev_buff[1]);//����ʱ���Լδ��,��ʱ�����Թ���
    if(rev_buff[1] == 0xff)//���������Чʱ,ȡĬ��ֵ
    {
        Dis_Para.dly[DIS_MODE_AUTO] = DIS_TIME_AUTO/10;
        Dis_Para.dly[DIS_MODE_SKEY] = DIS_TIME_SKEY/10;
    }

    Dis_Ctrl.cur_mode = DIS_MODE_AUTO;
    Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_AUTO];
    Dis_Ctrl.pre_key = CON_KEYBIT_NO;
    Dis_Ctrl.cur_diret = DN_SN_DIRECT;
    if(mode == 0)
    {
        Dis_Ctrl.flash_flag = PON_INT_SN_FLAG;
        if(CON_CPU_NORM_MODE == cpu_run_mode())//�е�״̬����ϵͳ����
        {
            Dis_Ctrl.cur_mode = DIS_MODE_ALLON;
            Dis_Ctrl.t_count = 6;//�ϵ�ȫ��ʾ6s
        }
    }
    Dis_Ctrl.lpress_ct = DIS_TIME_STLPKEY;

    Dis_Ctrl.dis_dly_time = 0;
    Dis_Ctrl.had_write = 0;

    get_dis_code(Dis_Ctrl.cur_mode);

    gState_Flash_Flag = 0;
    for(i=0; i<7; i++)
    {
        gMeter_State[i] = 0;//���״̬��
    }
*/
    U8 i;
    U8 rev_buff[5];

    file_read(PAR_LPDISCNT_04000301_00_ADDR, rev_buff, 5);//���ζ�ȡ����ѭ��&������ز�������μ�PAR_LPDISCNT_04000301_00_ADDR���崦

    if(CON_DIS_PARA_VALID == check_dis_para_valid(DIS_MODE_AUTO))
    {
        Dis_Para.maxn[DIS_MODE_AUTO] = bcd_to_hex(rev_buff[0]);
        Dis_Para.dly[DIS_MODE_AUTO] = bcd_to_hex(rev_buff[1]);//
        if(Dis_Para.dly[DIS_MODE_AUTO] > 30)
        {
           Dis_Para.dly[DIS_MODE_AUTO] = DIS_TIME_AUTO/10;
        }
        Dis_Para.para_valid[DIS_MODE_AUTO] = CON_DIS_PARA_VALID;
    }
    else
    {
       Dis_Para.maxn[DIS_MODE_AUTO] = DEFAULT_AUTO_DIS_MAX_NUM;
       Dis_Para.dly[DIS_MODE_AUTO] = DIS_TIME_AUTO/10;
       Dis_Para.para_valid[DIS_MODE_AUTO] = CON_DIS_PARA_INVALID;
    }

    if(CON_DIS_PARA_VALID == check_dis_para_valid(DIS_MODE_SKEY))
    {
        Dis_Para.maxn[DIS_MODE_SKEY] = bcd_to_hex(rev_buff[PAR_KEYDIS_04000305_00_ADDR - PAR_LPDISCNT_04000301_00_ADDR]);
        Dis_Para.dly[DIS_MODE_SKEY] = DIS_TIME_SKEY/10;
        Dis_Para.para_valid[DIS_MODE_SKEY] = CON_DIS_PARA_VALID;
    }
    else
    {
       Dis_Para.maxn[DIS_MODE_SKEY] = DEFAULT_KEY_DIS_MAX_NUM;
       Dis_Para.dly[DIS_MODE_SKEY] = DIS_TIME_SKEY/10;
       Dis_Para.para_valid[DIS_MODE_SKEY] = CON_DIS_PARA_INVALID;
    }
    for(i=0; i<2; i++)
    {
        Dis_Ctrl.dis_bsn[i] = 0;
        Dis_Ctrl.exd_lsn[i] = 0;
        Dis_Ctrl.lsn_maxn[i] = 1;
        if(Dis_Para.dly[i] == 0)
        {
           Dis_Para.dly[i] = 1;
        }
    }

    Dis_Ctrl.cur_mode = DIS_MODE_AUTO;
    Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_AUTO];
    Dis_Ctrl.pre_key = CON_KEYBIT_NO;
    Dis_Ctrl.cur_diret = DN_SN_DIRECT;
    if(mode == 0)
    {
        Dis_Ctrl.flash_flag = PON_INT_SN_FLAG;
        if(CON_CPU_NORM_MODE == cpu_run_mode())//�е�״̬����ϵͳ����
        {
            Dis_Ctrl.cur_mode = DIS_MODE_ALLON;
            Dis_Ctrl.t_count = 6;//�ϵ�ȫ��ʾ6s
        }
    }
    Dis_Ctrl.lpress_ct = DIS_TIME_STLPKEY;

    Dis_Ctrl.dis_dly_time = 0;
    Dis_Ctrl.had_write = 0;

    get_dis_code(Dis_Ctrl.cur_mode);

    gState_Flash_Flag = 0;
    for(i=0; i<7; i++)
    {
        gMeter_State[i] = 0;//���״̬��
    }
}
/******************************************************************************
 Function name:  void pd_lcd_display_task (void)
 Author       :
 Description  :
 Input        :  None
 Return       :  None
 *****************************************************************************/
void pd_lcd_display_task (void)
{
#ifdef LCD_VALID
    lcd_mode_ctrl();//�ϵ�ȫ�����Ի���Ե�ģʽ�����ִ���

//    if((gflash_ct % 5) == 0)//��˸��־����ǰ��ˢ�� �ź���Ļ��п��ܻ���ͣ��
//    {
//      rollback_flash_flag();
//  }
    CLRWDT;
//  lcd_display_flash(gflash_ct);
//    if((Dis_Ctrl.flash_flag & (PON_INT_SN_FLAG | LSN_CHG_SN_FLAG | MOD_CHG_SN_FLAG))//1�뵽�����б仯��������ʾ����
//    {
        //ÿ�붼Ҫ����
        fill_dis_data();
        Dis_Ctrl.flash_flag = 0;//���־
//    }


//    if((gflash_ct % 5) == 0)
//  {
//      if(Dis_Ctrl.had_write == 0)//��һ��100msдLCDǰ����д�� ����Ͳ�����ˢ����
//      {
//          fill_dis_flash_data();//0.5��ˢ����˸��־
//      }
//  }
//  Dis_Ctrl.had_write = 0;

//        os_dly_wait(OS_DLY_100MS);//OS_DLY_500MS
    gflash_ct += 10;//���������1S����һ�� ��ԭ����100ms��10��
    if(gflash_ct >= 50)
    {
        gflash_ct = 0;
    }
#endif
}
/******************************************************************************
** ��������: void ir_module_enable(void)
** ��������: ����ģ��ʹ��
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
//void ir_module_enable(void)//������յ�Դ
//{
//    IR_VDD_ON();//������յ�Դ
//    IO2IntEnf |= P2_IRRXD;                      //P2_IRRXD(p2.11)���ó��½�������IO�ж�
//    IO2IntClr = P2_IRRXD;                       //����жϱ�־
//}
/******************************************************************************
** ��������: void ir_module_disable(void)
** ��������: ����ģ���ֹ
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void ir_module_disable(void)//������յ�Դ
{
    IO2IntEnf &= ~P2_IRRXD;                      //P2_IRRXD(p2.11)�����ó��½�������IO�ж�
    IO2IntClr = P2_IRRXD;                       //����жϱ�־
    IR_VDD_OFF();//������յ�Դ
}


/******************************************************************************
** ��������: void debug_led_flash(U8 num)
** ��������: ����ģ���ֹ
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
//void debug_led_flash(U8 num)
//{
//    U8 i;
//
//    for(i=0;i<num;i++)
//    {
////            SET_PORT1_LOW(P1_GDATA_LED);
//            //GDATA_LED_OFF();
//            GPRS_GLED_ON();
//        delay(1000);
////            SET_PORT1_HIGH(P1_GDATA_LED);
//            //GDATA_LED_ON();
//            GPRS_GLED_OFF();
//        delay(1000);
//        CLRWDT;
//    }
//}

//void debug2_led_flash(U8 num)
//{
//    U8 i;
//
//    for(i=0;i<num;i++)
//    {
//            //SET_PORT1_LOW(P1_GDATA_LED);
////            GDATA_LED_ON();
//        GPRS_GLED_ON();
//        delay(4000);
//            //SET_PORT1_HIGH(P1_GDATA_LED);
////            GDATA_LED_OFF();
//        GPRS_GLED_OFF();
//        delay(4000);
//        CLRWDT;
//    }
//}

//void debug3_led_flash(U8 num)
//{
//    U8 i;
//
//    for(i=0;i<num;i++)
//    {
//            SET_PORT1_LOW(P1_CPU_YGP);
//        delay(4000);
//            SET_PORT1_HIGH(P1_CPU_YGP);
//
//        delay(4000);
//        CLRWDT;
//    }
//}
/******************************************************************************
** ��������: void pd_pin_init(U8 flag)
** ��������: ���ų�ʼ������
**           ����ʼ�����ж��û����������ŵ�����,�����������,��������������
**           P0��P1������Ϊ���ٷ���ģʽ�Լ���������
** ��ڲ���: flag:VLCD���ؿ���(���ڳ�ʼ��ʱ��ѡ���VLCD�Ŀ��� ���ڵ͹��ĵĴ���)
** �� �� ֵ: ��
** ˵    ��: �������� lpc1700_pin_cfg.h ͷ�ļ���ѡ�����Ӧ�ܽŵĹ���
******************************************************************************/
void pd_pin_init (U8 flag)
{
    PCONP |= (1 << PCGPIO);                                                   //ʹ��GPIOʱ��

    PINMODE_OD0 = 0;
    PINMODE_OD1 = 0;
    PINMODE_OD2 = 0;
    PINMODE_OD3 = 0;
    PINMODE_OD4 = 0;

    FIO0MASK = 0x00;                                                        //���������Ÿ��ٹ���
    FIO1MASK = 0x00;
    FIO2MASK = 0x00;
    FIO3MASK = 0x00;
    FIO4MASK = 0x00;

    FIO0PIN = PD_P0_INIT_DATA;
    FIO1PIN = PD_P1_INIT_DATA;
    if(flag == CON_CLOSE_VLCD)
    {
        FIO2PIN = PD_P2_INIT_DATA;
    }
    else
    {
       FIO2PIN = PD_P2_INIT_DATA|P2_DIS_CTL;
    }
    FIO3PIN = PD_P3_INIT_DATA;
    FIO4PIN = PD_P4_INIT_DATA;


    FIO0DIR = PD_P0_INIT_DIR;
    FIO1DIR = PD_P1_INIT_DIR;
    FIO2DIR = PD_P2_INIT_DIR;
    FIO3DIR = PD_P3_INIT_DIR;
    FIO4DIR = PD_P4_INIT_DIR;


    /*
     * ������������P0.00 -- P0.15
     */
    PINSEL0  = 0;

    /*
     * ������������P0.16 -- P0.30
     */
    PINSEL1  = 0;

    /*
     * ������������P1.0 -- P1.15
     */
    PINSEL2  = 0;

    /*
     * ������������P1.16 -- P1.31
     */
    PINSEL3  = 0;

    /*
     * ������������P2.0 -- P2.13
     */
    PINSEL4  = 0;

    /*
     * ������������P3.25 -- P3.26
     */
    PINSEL7  = 0;

    /*
     * ������������P4.28 -- P4.29
     */
    PINSEL9  = 0;

    PINSEL10 = 0;                                              /*  ��ֹETM�ӿڹ���             */

    /*
     * �����ڲ���������������
     */

    PINMODE0 = (PIN_NO_PULL << 30) | (PIN_NO_PULL << 28) | (PIN_NO_PULL << 26) | (PIN_NO_PULL << 24) |
               (PIN_NO_PULL << 22) | (PIN_NO_PULL << 20) | (PIN_NO_PULL << 18) | (PIN_NO_PULL << 16) |
               (PIN_NO_PULL << 14) | (PIN_NO_PULL << 12) | (PIN_NO_PULL << 10) | (PIN_NO_PULL << 8) |
               (PIN_NO_PULL << 6) | (PIN_NO_PULL << 4) | (PIN_NO_PULL << 2) | (PIN_NO_PULL << 0);

    PINMODE1 = (PIN_NO_PULL << 28) | (PIN_NO_PULL << 26) | (PIN_NO_PULL << 24) | (PIN_NO_PULL << 22) |
               (PIN_NO_PULL << 20) | (PIN_NO_PULL << 18) | (PIN_NO_PULL << 16) | (PIN_NO_PULL << 14) |
               (PIN_NO_PULL << 12) | (PIN_NO_PULL <<10) | (PIN_NO_PULL << 8) | (PIN_NO_PULL << 6) |
               (PIN_NO_PULL << 4) | (PIN_NO_PULL << 2) | (PIN_NO_PULL << 0);

    PINMODE2 = (PIN_NO_PULL << 30) | (PIN_NO_PULL << 28) | (PIN_NO_PULL << 26) | (PIN_NO_PULL << 24) |
               (PIN_NO_PULL << 22) | (PIN_NO_PULL << 20) | (PIN_NO_PULL << 18) | (PIN_NO_PULL << 16) |
               (PIN_NO_PULL << 14) | (PIN_NO_PULL << 12) | (PIN_NO_PULL << 10) | (PIN_NO_PULL << 8) |
               (PIN_NO_PULL << 6) | (PIN_NO_PULL << 4) | (PIN_NO_PULL << 2) | (PIN_NO_PULL << 0);

    PINMODE3 = (PIN_NO_PULL << 30) | (PIN_NO_PULL << 28) | (PIN_NO_PULL << 26) | (PIN_NO_PULL << 24) |
               (PIN_NO_PULL << 22) | (PIN_NO_PULL << 20) | (PIN_NO_PULL << 18) | (PIN_NO_PULL << 16) |
               (PIN_NO_PULL << 14) | (PIN_NO_PULL << 12) | (PIN_NO_PULL << 10) | (PIN_NO_PULL << 8) |
               (PIN_NO_PULL << 6) | (PIN_NO_PULL << 4) | (PIN_NO_PULL << 2) | (PIN_NO_PULL << 0);

    PINMODE4 = (PIN_NO_PULL << 30) | (PIN_NO_PULL << 28) | (PIN_NO_PULL << 26) | (PIN_NO_PULL << 24) |
               (PIN_NO_PULL << 22) | (PIN_NO_PULL << 20) | (PIN_NO_PULL << 18) | (PIN_NO_PULL << 16) |
               (PIN_NO_PULL << 14) | (PIN_NO_PULL << 12) | (PIN_NO_PULL << 10) | (PIN_NO_PULL << 8) |
               (PIN_NO_PULL << 6)  | (PIN_NO_PULL << 4) | (PIN_NO_PULL << 2) | (PIN_NO_PULL << 0);

//PINMODE0 = 0xaaaaaaaa;
//PINMODE1 = 0xaaaaaaaa;
//PINMODE2 = 0xaaaaaaaa;
//PINMODE3 = 0xaaaaaaaa;
//PINMODE4 = 0xaaaaaaaa;

    PINMODE7 = (PIN_NO_PULL << 20) | (PIN_NO_PULL << 18);

    PINMODE9 = (PIN_NO_PULL << 26) | (PIN_NO_PULL << 24);
}
/******************************************************************************
** ��������: pd_pconp_init (void)
** ��������: ���蹦�ܳ�ʼ����1 ��ʾ��Ӧ����ʹ��
**                           0 ��ʾ��Ӧ�����ֹ
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: �������� lpc1700_pin_cfg.h ͷ�ļ���ѡ�����Ӧ�����ʹ��
******************************************************************************/
void pd_pconp_init (void)
{
    PCONP = ((1<<PCRTC) | (1<<PCGPIO));
}
/******************************************************************************
** ��������: U8 pd_check_key_pressed (void)
** ��������: ��ȵ���ģʽ�� ÿ�뻽�Ѽ��һ����û�а������� �а��� ����������ʾģʽ
**                           0 ��ʾ��Ӧ�����ֹ
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: �������� lpc1700_pin_cfg.h ͷ�ļ���ѡ�����Ӧ�����ʹ��
******************************************************************************/
U8 pd_check_key_pressed (void)
{
    U8 cnt,i;

    SET_PORT0_IN(P0_KEYUP | P0_KEYDOWN);
    SET_PORT2_IN(P2_IRRXD);//�����û�к����ź�

    IR_VDD_ON();//������յ�Դ
    cnt = 0;
    while(1)
    {
        if(0 == PORT0_IS_HIGH(P0_KEYUP))
        {
            if(++cnt > 3)
            {
                return __TRUE;
            }
        }
        else
        {
            break;
        }
        delay(1);
    }

    cnt = 0;
    while(1)
    {
        if(0 == PORT0_IS_HIGH(P0_KEYDOWN))
        {
            if(++cnt > 3)
            {
                return __TRUE;
            }
        }
        else
        {
            break;
        }
        delay(1);
    }
//�����û�к����ź�
    cnt = 0;
    delay(10);
    for(i=0;i<10;i++)
    {
        if(0 == PORT2_IS_HIGH(P2_IRRXD))
        {
            if(++cnt > 1)
            {
                IR_VDD_OFF();//������յ�Դ
                return __TRUE;
            }
        }
        delay(5);
    }
    IR_VDD_OFF();//������յ�Դ
    return __FALSE;
}
/******************************************************************************
** ��������: void pd_check_power_on (void)
** ��������: ����ģʽ�� ����жϵ����� ��ѭ���ȿ��Ź���λ
**
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��:
******************************************************************************/
void pd_check_power_on (void)
{
    U8  power_ok_cnt;

    power_ok_cnt = 0;

    SET_PORT0_IN(P0_PD_CHK);                    //����p0.29Ϊ�����

    while(1)
    {
        if(PORT0_IS_HIGH(P0_PD_CHK))        //�������ź�����
        {
            if(++power_ok_cnt > 3)        //����cnt(=200)�μ�⵽�����ź�Ϊ������Ϊ��Դ�Ѿ��ȶ�
            {
                disable_modules_int();
                while(1);
            }
            CLRWDT;
        }
        else
        {
            break;
        }
        delay(5);                          //��ʱ ��ʱ�Ĵ�����ʱ����4MHz
    }
}
/******************************************************************************
** ��������: S32 pd_check_power_down(void)
** ��������: �ϵ��ж��ⲿ�Ƿ��е�
**
** ��ڲ���: ��
** �� �� ֵ: __TRUE:�ⲿ�е� __FALSE:�ⲿû��
** ˵    ��:
******************************************************************************/
S32 pd_check_power_down (void)
{
    U8  power_ok_cnt;

    power_ok_cnt = 0;
    SET_PORT0_IN(P0_PD_CHK);                    //����p0.29Ϊ�����

    while(1)
    {
        if(PORT0_IS_HIGH(P0_PD_CHK))        //�������ź�����
        {
            if(++power_ok_cnt > 3)        //����3�μ�⵽�����ź�Ϊ������Ϊ��Դ�Ѿ��ȶ�
            {
//                pd_mode_flag = CON_PD_MODE_NO;  //�ⲿ�е�״̬
                return __TRUE;
            }
        }
        else
        {
            power_ok_cnt = 0;
//            pd_mode_flag = CON_PD_MODE_YES; //����״̬
            return __FALSE;
        }
        CLRWDT;                         //���Ӳ�����Ź�
        delay(2);                          //��ʱԼ1.2ms����ʱ�Ĵ�����ʱ����4MHz
    }

}
/******************************************************************************
** ��������: S32 deep_pd_loop(void)
** ��������: ��ȵ��紦���ϵ縴λ��򵥵ĳ�ʼ���� ����ⲿ�Ƿ��е�
**           ����е磺��ֱ���˳� ���������¶�CPU��ȫ��ʼ�� ��ȫ������
**           ���û�磺���ж���û�а������� ������£����������������ʾ״̬
**                                          ���û������ֱ�ӽ�����ȵ���ģʽ �ȴ����Ѹ�λ
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: �ڻ����ѭ��main֮ǰ���� Ϊ����ȵ��縴λ����ʱ��(����ʡȥ��ʼ��ʱ��4ms)
******************************************************************************/
void deep_pd_loop (void)
{
    U32 temp;

//    disable_modules_int();
    pd_pconp_init();                                       //��ʼ�����蹦��
    pd_pin_init(CON_CLOSE_VLCD);                                         //���ų�ʼ�� ͬʱ��IOģ���Դ
    Cpu_Run_Mode = CON_CPU_DEPPD_MODE;                      //�ϵ�Ĭ��Ϊ�͹���ģʽ

//debug WLS_RST_LOW();//azh 110718 �ȳ�ʼ���꼰ϵͳ���������ڸ�λ����

//debug pd_target_init();                                   //��ʼ��Ŀ���

///////debug ������ ��ע�͵�
    if(__TRUE == pd_check_power_down())
    {
        return;//�ⲿ�е� ���ý���͹��Ĵ���ģʽ ��������������ʼ��
    }
    rtc_ain_init();

//  pd_enable_modules_int();//�ڳ�ʼ��������ȥ ʹ�ܡ���������������ǰ����һЩ�ж� ���½���Ī�����쳣
//��ȵ���ֻҪʹ��һ��RTC�жϾ�����
    ENABLE_ISR(NVIC_RTC, PRIO_RTC);         //ʹ��RTC

    PVDD_OFF();  //���������Դ
//debug    VLCD_OFF();  //��Һ��5V��Դ
//debug    BGLED_OFF();//��Һ�������Դ

//debug    ir_module_disable();//������յ�Դ

//���
//debug CHARGE_DIS();

    while(1)
    {
        //��ȵ���ģʽ ÿ�뻽�� ��λ
        while(1)
        {
            pd_bat_mange();                     //�͹���ģʽ�µ�ز�������

            //�����û�а�������
            if(__TRUE == pd_check_key_pressed())
            {
                break;
            }
            if(RTCGPREG0 & COP_PD_1ST_DIS)
            {
                RTCGPREG0 &= ~((U32)COP_PD_1ST_DIS);   //��ʶ����
                break;
            }
            pd_check_power_on();//�ⲿ�е� ��ѭ����Ӳ�����Ź���λ

    //        debug2_led_flash(1);

            //debug3_led_flash(3);

            CLRWDT;                         //���Ӳ�����Ź�

            pd_mode_ctrl(PD_MODE_DPOWDN);//���Ѻ���Զ���λ

            rtc_ain_init();

            temp = PCON;
            PCON = temp;                //�����ģʽ��ʶ
        }
        while(1)
        {
            pd_main();//����ģʽ�� ��ʾ����
            break;
        }
    }
}
/******************************************************************************
** ��������: void pd_bat_on (void)
** ��������: ����������Ƿ�Ҫ�򿪳���� ����ͣ��
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ����������
******************************************************************************/
void pd_bat_on (void)
{
    U16 vbat;
    U16 gate_volt;   //Ϊ�˼���2.4V��4.8V�������͵�صĲ�ͬ�ж�����
    
    vbat = Ad_Data[BAT_GPRS];
    if((vbat >= BAT_48V_MIN) && (vbat < BAT_48V_MAX))//����4.8V���
    {
        gate_volt = BAT_LOW_GATE_VOLT;
    }
    else if((vbat >= BAT_24V_MIN) && (vbat < BAT_24V_MAX))//����2.4V���
    {
        gate_volt = (BAT_LOW_GATE_VOLT >> 1);
    }
    else        //�޵��
    {
        gate_volt = (BAT_LOW_GATE_VOLT >> 1);
    }
    if((vbat >= gate_volt) && (vbat != 0))
    {
        BAT_ON();  //����GPRS��أ�ͣ�Ե����Ӳ���趨ѹ����Զ�����
        PIN_POWERON_CLR;//�ڵ��������ر�GPRSģ���Դ
    }
    else
    {
        BAT_OFF();  //�ر�GPRS��أ��л�Ϊͣ�Ե�ع���
    }
}
/******************************************************************************
** ��������: S32 pd_main(void)
** ��������: ��������µġ�main function
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
S32 pd_main (void)
{
    U32 temp;

    disable_modules_int();
    pconp_init();                                       //��ʼ�����蹦��
    pin_init();                                         //���ų�ʼ�� ͬʱ��IOģ���Դ
    pd_bat_on();//azh �ճ�ʼ���ܽ���Ĭ�Ϲص���

#ifdef ZIGBEE_TEST//azh171011
    WLS_RST_LOW();//azh 110718 �ȳ�ʼ���꼰ϵͳ���������ڸ�λ����
#endif

    pd_target_init();                                   //��ʼ��Ŀ���

//    far_infrared_init();                                //���⹦�ܳ�ʼ��

//    ad_ain_init();                                      //AD��ʼ��

//    timer2_init();                                      //��ʼ����ʱ��2

//    timer1_init();                                      //��ʼ����ʱ��1(����AD�����ж�����Ӧ��ad_init()֮ǰ����)
//    ad_prep_init();


    key_init();

    rtc_ain_init();

    pd_enable_modules_int();//�ڳ�ʼ��������ȥ ʹ�ܡ���������������ǰ����һЩ�ж� ���½���Ī�����쳣

//��EEPROM��ǰ���е����������ص�RAM��������ʾ,�����λ���ޱ�ɳ�ʱ��ʾ��������
    file_read(ENG_STORE_START_ADDR, gEngArray, ENG_RAM_ARRAY_MAX);

//    PVDD_ON();  //���������Դ
    VLCD_ON();  //��Һ��5V��Դ
    BGLED_OFF();//��Һ�������Դ
    PVDD_OFF();  //���������Դ

    ir_module_disable();//������յ�Դ

//���
    CHARGE_DIS();

    CLRWDT;                         //���Ӳ�����Ź�

    _init_box(Mem_64_Byte, sizeof(Mem_64_Byte), MEM_64_SIZE);           //initialize a memory pool for 64 bytes
    _init_box(Mem_256_Byte, sizeof(Mem_256_Byte), MEM_256_SIZE);        //initialize a memory pool for 256 bytes
    _init_box(Mem_1536_Byte, sizeof(Mem_1536_Byte), MEM_1536_SIZE);     //initialize a memory pool for 1024 bytes
//��ʼ������
#ifdef LCD_VALID
    lcd_reset();
    pd_lcd_init(0);

    get_flash_default_flag();
#endif

#ifdef ZIGBEE_TEST
    init_search_data();
#endif
//  debug_led_flash(5);

    pd_autodis_finish = 0;//û�а����� ��ѭ������һ��� ������ȵ���ģʽ

    while(1)
    {
        //����ģʽ ÿ����߰�������
            pd_check_power_on();//�ⲿ�е� ��ѭ����Ӳ�����Ź���λ
            //100ms����
            pd_lcd_display_task();

            //��ʱ�Ӵ���
            //�촦ͨ��
            //��Լ������

            CLRWDT;                         //���Ӳ�����Ź�

    //      SET_PORT1_REV(P1_GDATA_LED);//debug

            lcd_pd_mode();

            pd_pconp_init();//Ҫ�ڹܽ���������֮ǰ
            PVDD_OFF();  //���������Դ
            pd_pin_init(CON_UNCLOSE_VLCD); //���ų�ʼ��
            pd_bat_on();//azh �ճ�ʼ���ܽ���Ĭ�Ϲص���

            if(pd_autodis_finish)
            {
                pd_autodis_finish = 0;
                lcd_dis_off();
                VLCD_OFF();
                break;
            }

            //debug_led_flash(1);

            pd_mode_ctrl(PD_MODE_POWDN);

            disable_modules_int();

            pconp_init();                                       //��ʼ�����蹦��
//          pd_pin_init();
            pin_init();                                         //���ų�ʼ�� ͬʱ��IOģ���Դ
            pd_bat_on();//azh �ճ�ʼ���ܽ���Ĭ�Ϲص���

            pd_target_init();                                   //��ʼ��Ŀ���
//            PVDD_ON();  //���������Դ
            rtc_ain_init();

            pd_enable_modules_int();

//          debug2_led_flash(1);

            temp = PCON;
            PCON = temp;                //�����ģʽ��ʶ
    }
    return 0;
}
/******************************************************************************
** ��������: void pd_target_init(void)
** ��������: ����:ʱInitialize the target
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void pd_target_init (void)
{
//    AIRCR = (0x05fa << 16) + (PRIO_GROUP_CONFIG << 8);  //����ȷ���ж����ȼ���Ϊx��, �����ȼ�x��
    FLASHCFG = (0x05ul << 12) | 0x003a;                 //Flash����ʹ��6��CPUʱ�ӣ���FLASH�ڰ�ȫ�������У�������ٴ���

    PCLKSEL0 = PD_PCLKSEL0_INIT;                           //��ʼ������ʱ�ӷ�Ƶϵ����û���õ�������Ĭ��4��Ƶ
    PCLKSEL1 = PD_PCLKSEL1_INIT;

    CLKOUTCFG = 0;                                      //�ر�ʱ�����

    if((PLL0STAT >> 24) == 1)
    {
        PLL0CON = 1;                                    //Enable PLL, disconnected
        PLL0FEED = 0xAA;
        PLL0FEED = 0x55;
    }
    PLL0CON = 0;                                        //Disable PLL, disconnected
    PLL0FEED = 0xAA;
    PLL0FEED = 0x55;                                    //�ر�PLL��������ٴ���
    while(PLL0STAT & (3 << 24));                        //�ȴ��ر�PLL

//    start_ad_cpu_clk();                                 //

//�ڲ�����Ҫ���ȶ�
    SCS = (SCS & 0x04);                          //disable main OSC, 1MHz~20MHz
    CLKSRCSEL = 0;//0;//0x02;                                   //0:inter RC; 0x01:main_osc 0x02:select RTC as CPU clock source

//�ⲿ����Ҫ�ȴ��ȶ���
//    SCS = (SCS & 0x04) | 0x20;                          //Enable main OSC, 1MHz~20MHz
//    while((SCS & (1ul << 6)) == 0);                     //�ȴ������ȶ�
//  CLKSRCSEL = 1;//0;//0x02;                                   //0:inter RC; 0x01:main_osc 0x02:select RTC as CPU clock source

//    PLL0CFG   = (((PLL_NVALUE - 1) << 16) | (PLL_MVALUE - 1));
//    PLL0FEED  = 0xAA;
//    PLL0FEED  = 0x55;                                   //���÷�Ƶϵ��
//
//    PLL0CON   = 1;
//    PLL0FEED  = 0xAA;
//    PLL0FEED  = 0x55;                                   //Enable but disconnect PLL
//    while((PLL0STAT & (1ul << 24)) == 0);               //�ȴ�PLLʹ��

    CCLKCFG = PD_FCCLK_DIV - 1;                            //����ϵͳʱ�ӷ�Ƶϵ��

//    while(((PLL0STAT & (1ul << 26)) == 0));             //Check lock bit status
//
//    while(((PLL0STAT & 0x00007FFF) != (PLL_MVALUE - 1))
//      && (((PLL0STAT & 0x00FF0000) >> 16) != (PLL_NVALUE - 1)));
//
//    PLL0CON  = 3;                                       //Connect the PLL
//    PLL0FEED = 0xAA;
//    PLL0FEED = 0x55;
//    while((PLL0STAT & (1ul << 25)) == 0);               //Wait until the PLL is connected

#if PD_FCCLK <= 20000000
    FLASHCFG = (0x00ul << 12) | 0x003a;                 //Flash����ʹ��1��CPUʱ�ӣ����ô洢������ģ��
#endif//PD_FCCLK <= 20000000

#if PD_FCCLK > 20000000 && PD_FCCLK <= 40000000
    FLASHCFG = (0x01ul << 12) | 0x003a;                 //Flash����ʹ��2��CPUʱ��
#endif//PD_FCCLK > 20000000 && PD_FCCLK <= 40000000

#if PD_FCCLK > 40000000 && PD_FCCLK <= 60000000
    FLASHCFG = (0x02ul << 12) | 0x003a;                 //Flash����ʹ��3��CPUʱ��
#endif//PD_FCCLK > 40000000 && PD_FCCLK <= 60000000

#if PD_FCCLK > 60000000 && PD_FCCLK <= 80000000
    FLASHCFG = (0x03ul << 12) | 0x003a;                 //Flash����ʹ��4��CPUʱ��
#endif//PD_FCCLK > 60000000 && PD_FCCLK <= 80000000

#if PD_FCCLK > 80000000 && PD_FCCLK <= 100000000
    FLASHCFG = (0x04ul << 12) | 0x003a;                 //Flash����ʹ��5��CPUʱ��
#endif//PD_FCCLK > 80000000 && PD_FCCLK <= 100000000

#if PD_FCCLK > 100000000
    FLASHCFG = (0x05ul << 12) | 0x003a;                 //Flash����ʹ��6��CPUʱ��
#endif//PD_FCCLK > 100000000

}
#endif//CODE_VER_TYPE==GPRS_MODULE_PRJ		//����CPU_GPRSģ��

/******************************************************************************
** ��������: U16 cpu_run_mode(void)
** ��������: �ж��Ƿ��ڵ���ģʽ������ ��Ҫ����Щ����Ҫ�õ�RTXϵͳ����Դ ��������
**           ���ϵ��ʱ��ȷ����ǰ����ģʽ
** ��ڲ���: ��
** �� �� ֵ: ����ģʽ ����ȫ��ģʽ���ߵ͹���ʡ��ģʽ
** ˵    ��: ��
******************************************************************************/
U16 cpu_run_mode (void)
{
    return Cpu_Run_Mode;
}
/******************************************************************************
                            End Of File
 *****************************************************************************/

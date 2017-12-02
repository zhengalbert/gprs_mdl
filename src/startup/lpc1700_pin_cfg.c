/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : lpc1700_pin_cfg.c
 Description    : ���ź�����ʹ�ܳ�ʼ������
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   3��    2.00        2011-06-16      LFG            ��ʾ�ն˹ܽ�����
   2��    1.01        2010-08-19      LFG            �����5463���жϽź�Ƭѡ������,
   1��    1.00        2010-07-19      snap           the original version
******************************************************************************/

#include "..\inc\global_config.h"

/******************************************************************************
** ��������: void pin_init(void)
** ��������: ���ų�ʼ������
**           ����ʼ�����ж��û����������ŵ�����,�����������,��������������
**           P0��P1������Ϊ���ٷ���ģʽ�Լ���������
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: �������� lpc1700_pin_cfg.h ͷ�ļ���ѡ�����Ӧ�ܽŵĹ���
******************************************************************************/
void pin_init (void)
{
    PCONP |= 1 << PCGPIO;                                                   //ʹ��GPIOʱ��

#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
    PINMODE_OD0 = (P0_SDA_E2 | P0_SCL_E2);        //�������ſ�©ģʽ
#else
    PINMODE_OD0 = (P0_SDA_E2 | P0_SCL_E2 | P0_SDA_LCD | P0_SCL_LCD);        //�������ſ�©ģʽ
#endif
    PINMODE_OD1 = 0;
    PINMODE_OD2 = 0;
    PINMODE_OD3 = 0;
    PINMODE_OD4 = 0;

    FIO0MASK = 0x00;                                                        //���������Ÿ��ٹ���
    FIO1MASK = 0x00;
    FIO2MASK = 0x00;
    FIO3MASK = 0x00;
    FIO4MASK = 0x00;

    FIO0PIN = P0_INIT_DATA;
    FIO1PIN = P1_INIT_DATA;
    FIO2PIN = P2_INIT_DATA;
    FIO3PIN = P3_INIT_DATA;
    FIO4PIN = P4_INIT_DATA;

    FIO0DIR = P0_INIT_DIR;
    FIO1DIR = P1_INIT_DIR;
    FIO2DIR = P2_INIT_DIR;
    FIO3DIR = P3_INIT_DIR;
    FIO4DIR = P4_INIT_DIR;

    /*
     * ������������P0.00 -- P0.15
     */
    PINSEL0  = (P0_15_FNUC << 30) |
               (P0_11_FNUC << 22) | (P0_10_FNUC << 20) | (P0_9_FNUC << 18) | (P0_8_FNUC << 16) |
               (P0_7_FNUC << 14) | (P0_6_FNUC << 12) | (P0_5_FNUC << 10) | (P0_4_FNUC << 8) |
               (P0_3_FNUC << 6) | (P0_2_FNUC << 4) | (P0_1_FNUC << 2) | (P0_0_FNUC << 0);

    /*
     * ������������P0.16 -- P0.30
     */
    PINSEL1  = (P0_30_FNUC << 28) | (P0_29_FNUC << 26) | (P0_28_FNUC << 24) | (P0_27_FNUC << 22) |
               (P0_26_FNUC << 20) | (P0_25_FNUC <<18) | (P0_24_FNUC << 16) | (P0_23_FNUC <<14) |
               (P0_22_FNUC << 12) | (P0_21_FNUC << 10) | (P0_20_FNUC << 8) | (P0_19_FNUC << 6) |
               (P0_18_FNUC << 4) | (P0_17_FNUC << 2) | (P0_16_FNUC << 0);

    /*
     * ������������P1.0 -- P1.15
     */
    PINSEL2  = (P1_15_FNUC << 30) | (P1_14_FNUC << 28) | (P1_10_FNUC << 20) | (P1_9_FNUC << 18)|
               (P1_8_FNUC  << 16) | (P1_4_FNUC << 8)  |(P1_1_FNUC << 2)  | (P1_0_FNUC << 0);

    /*
     * ������������P1.16 -- P1.31
     */
    PINSEL3  = (P1_31_FNUC << 30) | (P1_30_FNUC << 28) | (P1_29_FNUC << 26) | (P1_28_FNUC << 24) |
               (P1_27_FNUC << 22) | (P1_26_FNUC << 20) | (P1_25_FNUC << 18) | (P1_24_FNUC << 16) |
               (P1_23_FNUC << 14) | (P1_22_FNUC << 12) | (P1_21_FNUC << 10) | (P1_20_FNUC << 8) |
               (P1_19_FNUC << 6) | (P1_18_FNUC << 4) | (P1_17_FNUC << 2) | (P1_16_FNUC << 0);

    /*
     * ������������P2.0 -- P2.13
     */
    PINSEL4  = (P2_13_FNUC << 26) | (P2_12_FNUC << 24) | (P2_11_FNUC << 22) | (P2_10_FNUC << 20) |
               (P2_9_FNUC << 18) | (P2_8_FNUC  << 16) | (P2_7_FNUC << 14) | (P2_6_FNUC << 12) |
               (P2_5_FNUC << 10) | (P2_4_FNUC << 8)  | (P2_3_FNUC << 6)  | (P2_2_FNUC << 4) |
               (P2_1_FNUC << 2) | (P2_0_FNUC << 0);

    /*
     * ������������P3.25 -- P3.26
     */
    PINSEL7  = (P3_26_FNUC << 20) | (P3_25_FNUC << 18);

    /*
     * ������������P4.28 -- P4.29
     */
    PINSEL9  = (P4_29_FNUC << 26) | (P4_28_FNUC << 24);

    PINSEL10 = P10_3_FNUC;                                              /*  ��ֹETM�ӿڹ���             */

    /*
     * �����ڲ���������������
     */
    PINMODE0 = (P0_15_MODE << 30) | (P0_14_MODE << 28) | (P0_13_MODE << 26) | (P0_12_MODE << 24) |
               (P0_11_MODE << 22) | (P0_10_MODE << 20) | (P0_9_MODE << 18) | (P0_8_MODE << 16) |
               (P0_7_MODE << 14) | (P0_6_MODE << 12) | (P0_5_MODE << 10) | (P0_4_MODE << 8) |
               (P0_3_MODE << 6) | (P0_2_MODE << 4) | (P0_1_MODE << 2) | (P0_0_MODE << 0);

    PINMODE1 = (P0_30_MODE << 28) | (P0_29_MODE << 26) | (P0_28_MODE << 24) | (P0_27_MODE << 22) |
               (P0_26_MODE << 20) | (P0_25_MODE << 18) | (P0_24_MODE << 16) | (P0_23_MODE << 14) |
               (P0_22_MODE << 12) | (P0_21_MODE <<10) | (P0_20_MODE << 8) | (P0_19_MODE << 6) |
               (P0_18_MODE << 4) | (P0_17_MODE << 2) | (P0_16_MODE << 0);

    PINMODE2 = (P1_15_MODE << 30) | (P1_14_MODE << 28) | (P1_13_MODE << 26) | (P1_12_MODE << 24) |
               (P1_11_MODE << 22) | (P1_10_MODE << 20) | (P1_9_MODE << 18) | (P1_8_MODE << 16) |
               (P1_7_MODE << 14) | (P1_6_MODE << 12) | (P1_5_MODE << 10) | (P1_4_MODE << 8) |
               (P1_3_MODE << 6) | (P1_2_MODE << 4) | (P1_1_MODE << 2) | (P1_0_MODE << 0);

    PINMODE3 = (P1_31_MODE << 30) | (P1_30_MODE << 28) | (P1_29_MODE << 26) | (P1_28_MODE << 24) |
               (P1_27_MODE << 22) | (P1_26_MODE << 20) | (P1_25_MODE << 18) | (P1_24_MODE << 16) |
               (P1_23_MODE << 14) | (P1_22_MODE << 12) | (P1_21_MODE << 10) | (P1_20_MODE << 8) |
               (P1_19_MODE << 6) | (P1_18_MODE << 4) | (P1_17_MODE << 2) | (P1_16_MODE << 0);

    PINMODE4 = (P2_15_MODE << 30) | (P2_14_MODE << 28) | (P2_13_MODE << 26) | (P2_12_MODE << 24) |
               (P2_11_MODE << 22) | (P2_10_MODE << 20) | (P2_9_MODE << 18) | (P2_8_MODE << 16) |
               (P2_7_MODE << 14) | (P2_6_MODE << 12) | (P2_5_MODE << 10) | (P2_4_MODE << 8) |
               (P2_3_MODE << 6)  | (P2_2_MODE << 4) | (P2_1_MODE << 2) | (P2_0_MODE << 0);

    PINMODE7 = (P3_26_MODE << 20) | (P3_25_MODE << 18);

    PINMODE9 = (P4_29_MODE << 26) | (P4_28_MODE << 24);
}

/******************************************************************************
** ��������: pconp_init (void)
** ��������: ���蹦�ܳ�ʼ����1 ��ʾ��Ӧ����ʹ��
**                           0 ��ʾ��Ӧ�����ֹ
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: �������� lpc1700_pin_cfg.h ͷ�ļ���ѡ�����Ӧ�����ʹ��
******************************************************************************/
void pconp_init (void)
{
    PCONP = (PCTIM0_FUNC << 1) | (PCTIM1_FUNC << 2) | (PCUART0_FUNC << 3) | (PCUART1_FUNC << 4) |
            (PWM1_FUNC << 6) | (PCI2C0_FUNC << 7) | (PCSPI_FUNC << 8) | (PCRTC_FUNC << 9) |
            (PCSSP1_FUNC << 10) | (PCAD_FUNC << 12) | (PCCAN1_FUNC << 13) | (PCCAN2_FUNC << 14) |
            (PCGPIO_FUNC << 15) | (PCRIT_FUNC << 16) | (PCMC_FUNC << 17) | (PCQEI_FUNC << 18) |
            (PCI2C1_FUNC << 19) | (PCSSP0_FUNC << 21) | (PCTIM2_FUNC << 22) | (PCTIM3_FUNC << 23) |
            (PCUART2_FUNC << 24) | (PCUART3_FUNC << 25) | (PCI2C2_FUNC << 26) | (PCI2S_FUNC << 27) |
            (PCGPDMA_FUNC << 29) | (PCENET_FUNC << 30) | (PCUSB_FUNC << 31);
}

/******************************************************************************
    End Of File
******************************************************************************/

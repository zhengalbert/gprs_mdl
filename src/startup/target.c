/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : target.c
 Description    : the specific codes for LPC1700 target boards
                  every project should include a copy of this file
                  user may modify it as needed
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��    1.00        2010-07-16      snap           the original version
******************************************************************************/

#include "..\inc\global_config.h"

//static void start_ad_cpu_clk (void);

/******************************************************************************
** ��������: void default_vector_handle(void)
** ��������: Ĭ���쳣�������
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
//void default_vector_handle (void)
//{
//#ifdef __DEBUG
//    while(1);
//#else//__DEBUG
//    chip_reset(HARD_RESET);
//#endif//__DEBUG
//}

/******************************************************************************
** ��������: void target_init(void)
** ��������: Initialize the target
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void target_init (void)
{
//    AIRCR = (0x05fa << 16) + (PRIO_GROUP_CONFIG << 8);  //����ȷ���ж����ȼ���Ϊx��, �����ȼ�x��
    FLASHCFG = (0x05ul << 12) | 0x003a;                 //Flash����ʹ��6��CPUʱ�ӣ���FLASH�ڰ�ȫ�������У�������ٴ���

    PCLKSEL0 = PCLKSEL0_INIT;                           //��ʼ������ʱ�ӷ�Ƶϵ����û���õ�������Ĭ��4��Ƶ
    PCLKSEL1 = PCLKSEL1_INIT;

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
    SCS = (SCS & 0x04) | 0x20;                          //Enable main OSC, 1MHz~20MHz
    while((SCS & (1ul << 6)) == 0);                     //�ȴ������ȶ�

    CLKSRCSEL = 0x01;                                   //select main OSC as the PLL clock source

    PLL0CFG   = (((PLL_NVALUE - 1) << 16) | (PLL_MVALUE - 1));
    PLL0FEED  = 0xAA;
    PLL0FEED  = 0x55;                                   //���÷�Ƶϵ��

    PLL0CON   = 1;
    PLL0FEED  = 0xAA;
    PLL0FEED  = 0x55;                                   //Enable but disconnect PLL
    while((PLL0STAT & (1ul << 24)) == 0);               //�ȴ�PLLʹ��

    CCLKCFG = FCCLK_DIV - 1;                            //����ϵͳʱ�ӷ�Ƶϵ��

    while(((PLL0STAT & (1ul << 26)) == 0));             //Check lock bit status

    while(((PLL0STAT & 0x00007FFF) != (PLL_MVALUE - 1))
      && (((PLL0STAT & 0x00FF0000) >> 16) != (PLL_NVALUE - 1)));

    PLL0CON  = 3;                                       //Connect the PLL
    PLL0FEED = 0xAA;
    PLL0FEED = 0x55;
    while((PLL0STAT & (1ul << 25)) == 0);               //Wait until the PLL is connected

#if FCCLK <= 20000000
    FLASHCFG = (0x00ul << 12) | 0x003a;                 //Flash����ʹ��1��CPUʱ�ӣ����ô洢������ģ��
#endif//FCCLK <= 20000000

#if FCCLK > 20000000 && FCCLK <= 40000000
    FLASHCFG = (0x01ul << 12) | 0x003a;                 //Flash����ʹ��2��CPUʱ��
#endif//FCCLK > 20000000 && FCCLK <= 40000000

#if FCCLK > 40000000 && FCCLK <= 60000000
    FLASHCFG = (0x02ul << 12) | 0x003a;                 //Flash����ʹ��3��CPUʱ��
#endif//FCCLK > 40000000 && FCCLK <= 60000000

#if FCCLK > 60000000 && FCCLK <= 80000000
    FLASHCFG = (0x03ul << 12) | 0x003a;                 //Flash����ʹ��4��CPUʱ��
#endif//FCCLK > 60000000 && FCCLK <= 80000000

#if FCCLK > 80000000 && FCCLK <= 100000000
    FLASHCFG = (0x04ul << 12) | 0x003a;                 //Flash����ʹ��5��CPUʱ��
#endif//FCCLK > 80000000 && FCCLK <= 100000000

#if FCCLK > 100000000
    FLASHCFG = (0x05ul << 12) | 0x003a;                 //Flash����ʹ��6��CPUʱ��
#endif//FCCLK > 100000000
}
/******************************************************************************
** ��������: void start_ad_cpu_clk (void)
** ��������: ǿ������AD���CPUʱ�ӹ��ܺ���
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��AD������ֹͣCPUʱ�����ʱ���øú���
******************************************************************************/
//void start_ad_cpu_clk (void)
//{
//
//    //��ʼ��SPI����Ϊͨ��IO��[CS/SDI/CLK/RST];
//    //��λ10us
//    //׼����������:д0x000006��CTRLR,���ģʽ[U32 dat = 0x06000078]
//    //��תʱ��IO�ţ���λ�����������
//    //���CPUʱ���Ƿ�����
//    //��CPUʱ�������,��ԭSPI��������
////-----------------------------------------------------------------------------
//    U8 i;
//    U32 dat;
//
//    FIO1PIN |= (P1_CS5463_CLK | P1_CS5463_RST | P1_CS5463_SDI | P1_CS5463_SDO | P1_CS5463_CS1);//��Ϊ���1
//    FIO1DIR |= (P1_CS5463_CLK | P1_CS5463_RST | P1_CS5463_SDI | P1_CS5463_SDO | P1_CS5463_CS1);//��Ϊ�����
//   /*
//     * ������������P1.16 -- P1.31[ֻ�ı�P1_20(CLK)��P1_22(RST)��P1_24(SDI)��P1_25(CS1)ΪIO��]
//     */
//    PINSEL3  = (P1_31_FNUC << 30)|(P1_30_FNUC << 28)|(P1_29_FNUC << 26)|(P1_28_FNUC << 24)|
//               (P1_27_FNUC << 22)|(P1_26_FNUC << 20)|(P1_25_GPIO << 18)|(P1_24_GPIO << 16)|
//               (P1_23_FNUC << 14)|(P1_22_GPIO << 12)|(P1_21_FNUC << 10)|(P1_20_GPIO << 8) |
//               (P1_19_FNUC << 6) |(P1_18_FNUC << 4) |(P1_17_FNUC << 2) |(P1_16_FNUC << 0);
////-----------------------------------------------------------------------------
////    CS5463_RST_LOW();                                   //��λ, ��������ɾ
////    delay(2);
////    CS5463_RST_HIGH();
////    dat = 0xfffffffeu;                                  //ͬ��
////    CS5463_CS_EN1();
////    for(i = 0; i < 32; i++)
////    {
////        SET_PORT1_LOW(P1_CS5463_CLK);                   //�½���
////        if(dat & 0x80000000)
////        {
////            SET_PORT1_HIGH(P1_CS5463_SDI);              //��1
////        }
////        else
////        {
////            SET_PORT1_LOW(P1_CS5463_SDI);               //��0
////        }
////        dat <<= 1;
////        __nop();
////        SET_PORT1_HIGH(P1_CS5463_CLK);                  //������
////    }
////    SET_PORT1_LOW(P1_CS5463_CLK);                       //�½���
////    CS5463_CS_DIS1();
////    SET_PORT1_HIGH(P1_CS5463_CLK);                      //������
////
////    __nop();
////
////    dat = 0x78000004;                                   //��ʱ��
////    CS5463_CS_EN1();
////    for(i = 0; i < 32; i++)
////    {
////        SET_PORT1_LOW(P1_CS5463_CLK);                   //�½���
////        if(dat & 0x80000000)
////        {
////            SET_PORT1_HIGH(P1_CS5463_SDI);              //��1
////        }
////        else
////        {
////            SET_PORT1_LOW(P1_CS5463_SDI);               //��0
////        }
////        dat <<= 1;
////        __nop();
////        SET_PORT1_HIGH(P1_CS5463_CLK);                  //������
////    }
////    SET_PORT1_LOW(P1_CS5463_CLK);                       //�½���
////    CS5463_CS_DIS1();
////    SET_PORT1_HIGH(P1_CS5463_CLK);                      //������
////
////    __nop();
//
//    CS5463_RST_LOW();                                   //��λ
//    delay(2);
//    CS5463_RST_HIGH();
//    dat = 0x78000000;                                   //��ʱ��
//    CS5463_CS_EN1();
//    for(i = 0; i < 32; i++)
//    {
//        SET_PORT1_LOW(P1_CS5463_CLK);                   //�½���
//        if(dat & 0x80000000)
//        {
//            SET_PORT1_HIGH(P1_CS5463_SDI);              //��1
//        }
//        else
//        {
//            SET_PORT1_LOW(P1_CS5463_SDI);               //��0
//        }
//        dat <<= 1;
//        __nop();
//        SET_PORT1_HIGH(P1_CS5463_CLK);                  //������
//    }
//    SET_PORT1_LOW(P1_CS5463_CLK);                       //�½���
//    CS5463_CS_DIS1();
//    SET_PORT1_HIGH(P1_CS5463_CLK);                      //������
//    delay(1);
////-----------------------------------------------------------------------------
//   /*
//     * ������������P1.16 -- P1.31[ֻ��ԭP1_20(CLK)��P1_22(RST)��P1_24(SDI)��P1_25(CS1)ΪIO��]
//     */
//    PINSEL3  = (P1_31_FNUC << 30)|(P1_30_FNUC << 28)|(P1_29_FNUC << 26)|(P1_28_FNUC << 24)|
//               (P1_27_FNUC << 22)|(P1_26_FNUC << 20)|(P1_25_FNUC << 18)|(P1_24_FNUC << 16)|
//               (P1_23_FNUC << 14)|(P1_22_FNUC << 12)|(P1_21_FNUC << 10)|(P1_20_FNUC << 8) |
//               (P1_19_FNUC << 6) |(P1_18_FNUC << 4) |(P1_17_FNUC << 2) |(P1_16_FNUC << 0);
//
//}
/******************************************************************************
    End Of File
******************************************************************************/

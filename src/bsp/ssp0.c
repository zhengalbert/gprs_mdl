/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : ssp0.c
 Description    : LPC17ϵ��оƬӲ��SSP0�ӳ���
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��    1.01		  2011-06-24	  azh			 ����FLASH��д
   1��    1.00        2010-08-19      snap           the original version
******************************************************************************/

#include "..\inc\global_config.h"

//-------------------------------- �������� -----------------------------------

//------------------------------ ˽�б������� ---------------------------------

//------------------------------ ˽�к������� ---------------------------------
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//����CPU_GPRSģ��
static void ssp0_rst(void);
#endif
/******************************************************************************
** ��������: S32 ssp0_operation(U8 *send_buff, U8 *rcv_buff, U16 num, U8 operation)
** ��������: ��ȡ��д��DataFlash���ݣ����ȴ��ж�������߲���
** ��ڲ���: *send_buff: д�����ݻ���
**           *rcv_buff : ��ȡ���ݻ���
**           num       : ���ݳ���
**           operation : ������ʽ--��/д
** �� �� ֵ: indicating the result of the operation
**              1: normal
**             -3: waiting time overflow
**        -others: state error
** ˵    ��: ��
******************************************************************************/
S32 ssp0_operation (U8 *send_buff, U8 *rcv_buff, U16 num, U8 operation)
{
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//����CPU_GPRSģ��
    S32 wait_time;

    SSP_Ctrl[SSP0].Operation = operation;
    SSP_Ctrl[SSP0].End_Flag = SSP_IDLE;
    SSP_Ctrl[SSP0].Send_Ptr = send_buff;
    SSP_Ctrl[SSP0].Rcv_Ptr = rcv_buff;
    SSP_Ctrl[SSP0].Send_Num = num;
    SSP_Ctrl[SSP0].Rcv_Num = num;
    SSP_Ctrl[SSP0].Error = NO_ERROR;

    ssp0_init();
    wait_time = num * 8 * (1000000 / SSP0_FREQ) * 10;       //�ȴ�10��������ͨѶʱ��
    while(wait_time--)                      //�ȴ�SSP�������
    {
        if(SSP_Ctrl[SSP0].End_Flag == SSP_END)
        {
            break;
        }
        delay(1);                           //��ʱԼ1uS
    }
    if(wait_time < 0)
    {
        SSP_Ctrl[SSP0].Error = TIME_OVF;    //�ȴ���ʱ
    }
    ssp0_rst();

    return(SSP_Ctrl[SSP0].Error);
#else
	return 1;

#endif
}
/******************************************************************************
** ��������: void ssp0_init(void)
** ��������: ��ʼ��SSP0�����������߲���
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//����CPU_GPRSģ��
void ssp0_init (void)
{
    U8 k;

    PCONP |= (1 << PCSSP0);                 //enable SSP0
    ssp0_rst();                             //��ֹSSP0�����ж�

    PCLKSEL1 &= ~(0x03u << 10);
    PCLKSEL1 |= (FPCLK_SSP0_DIV << 10);     //��������ʱ�ӷ�Ƶϵ��

    if((PINSEL3 & ((0x03u << 8) | (0x03u << 14) | (0x03u << 16)))
      != ((P1_20_FNUC << 8) | (P1_23_FNUC << 14) | (P1_24_FNUC << 16)))             //���SCK0��MISO0��MOSI0�����Ź�������
    {
        PINSEL3 &= ~((0x03u << 8) | (0x03u << 14) | (0x03u << 16));                 //��������bits
        PINSEL3 |= ((P1_20_FNUC << 8) | (P1_23_FNUC << 14) | (P1_24_FNUC << 16));   //configure the SCK0��MISO0 and MOSI0 as the alternate function 2 configuration
    }
    PINMODE3 &= ~((0x03u << 8) | (0x03u << 14) | (0x03u << 16));                    //��������bits
    PINMODE3 |= ((P1_20_MODE << 8) | (P1_23_MODE << 14) | (P1_24_MODE << 16));      //SCK0��MISO0 and MOSI0 has neither pull-up nor pull-down registers
    PINMODE_OD1 &= ~(P1_F1_SCK | P1_F1_MISO | P1_F1_MOSI);                //�������ŷǿ�©ģʽ

    SSP0CPSR = CPSR_DIV;                // - PCLK / 2, 2��254 �е�һ��ż��ֵ

#if (FPCLK_SSP0 / CPSR_DIV / SSP0_FREQ) > 0
    k = FPCLK_SSP0 / CPSR_DIV / SSP0_FREQ - 1;
#else//(FPCLK_SSP0 / CPSR_DIV / SSP0_FREQ) > 0
#error("SPI0 bus frequency not allowed")
    k = 0;                              // - ��СΪ��0������2��Ƶ����ʱʵ�ʵ����ʲ���������ֵ��
#endif//(FPCLK_SSP0 / CPSR_DIV / SSP0_FREQ) > 0

    SSP0CR0  = ((0x07 << CR0_DSS) |     // - 8 bits data format
                (0x00 << CR0_FRF) |     // - SPI format
                (1 << CR0_CPOL) |       // - SPI maintains high between frames
                (1 << CRO_CPHA) |       // - capture the first clock transition
                (k << CRO_SCR));        // - clock number

    for(k = 0; k < SSP_FIFO_SIZE; k++)
    {
        SSP0DR;                         // - clear the RxFIFO
    }

    SSP0IMSC |= (IMSC_RORIM |           // - data is overwritten
                 IMSC_RTIM |            // - receive timeout occurs
                 IMSC_RXIM |            // - RxFIFO is at least half full
                 IMSC_TXIM);            // - TxFIFO is at least half empty

    SSP0CR1 |= 0x02;                    // - SSE enabled, SSP0 controller is enabled
//    ENABLE_ISR(NVIC_SSP0, PRIO_SSP0);   // - set SSP0 interrupt parameter
}
#endif//H_METER_PRJ,WLS_CUR_PRJ
/******************************************************************************
** ��������: void ssp0_rst(void)
** ��������: ��λSSP0������ж�
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//����CPU_GPRSģ��
void ssp0_rst (void)
{
//    DISABLE_ISR(NVIC_SSP0);             // - disbale SSP0 interrupt
    SSP0CR1 = 0x00;                     // - disbale SSP0
    SSP0IMSC = 0x00;                    // - clear interrupt masked bits
    SSP0ICR = (ICR_RORIC | ICR_RTIC);   // - clear interrupt flag
}
#endif
/******************************************************************************
** ��������: void SSP0_IRQHandler(void)
** ��������: SSP0�жϷ������
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//����CPU_GPRSģ��
void SSP0_IRQHandler (void)
{
    U8 mis;

    mis = SSP0MIS;

    if(mis & MIS_RORMIS)                                //Rx FiFo overrun
    {
        while(SSP0SR & SR_RNE)
        {
            SSP0DR;
        }
        SSP0ICR |= ICR_RORIC;
        SSP_Ctrl[SSP0].Error = DATA_OVF;                //���ݽ������
//        DISABLE_ISR(NVIC_SSP0);                         //Disbale SSP0 interrupt
        SSP_Ctrl[SSP0].End_Flag = SSP_END;
    }

    if(mis & (MIS_RTMIS | MIS_RXMIS))                   //Receive timeout
    {
        while(SSP0SR & SR_RNE)
        {
            if(SSP_Ctrl[SSP0].Rcv_Num)
            {
                SSP_Ctrl[SSP0].Rcv_Num--;
            }

            if(SSP_Ctrl[SSP0].Operation == SSP_WR)
            {
                SSP0DR;
            }
            else if(SSP_Ctrl[SSP0].Operation == SSP_RD)
            {
                *SSP_Ctrl[SSP0].Rcv_Ptr++ = SSP0DR;
            }

            if(SSP_Ctrl[SSP0].Rcv_Num == 0)
            {
//                DISABLE_ISR(NVIC_SSP0);                 //Disbale SSP0 interrupt
                SSP_Ctrl[SSP0].End_Flag = SSP_END;
                break;
            }
        }
        SSP0ICR |= ICR_RTIC;
    }

    if(mis & MIS_TXMIS)                                 //Tx FiFo is atleast half empty
    {
        while(SSP0SR & SR_TNF)
        {
            if(SSP_Ctrl[SSP0].Send_Num)
            {
                if(SSP0SR & SR_RNE)
                {
                    if(SSP_Ctrl[SSP0].Rcv_Num)
                    {
                        SSP_Ctrl[SSP0].Rcv_Num--;
                    }

                    if(SSP_Ctrl[SSP0].Operation == SSP_WR)
                    {
                        SSP0DR;
                    }
                    else if(SSP_Ctrl[SSP0].Operation == SSP_RD)
                    {
                        *SSP_Ctrl[SSP0].Rcv_Ptr++ = SSP0DR;
                    }

                    if(SSP_Ctrl[SSP0].Rcv_Num == 0)
                    {
//                        DISABLE_ISR(NVIC_SSP0);         //Disbale SSP0 interrupt
                        SSP_Ctrl[SSP0].End_Flag = SSP_END;
                        break;
                    }
                }

                SSP0DR = *SSP_Ctrl[SSP0].Send_Ptr++;
                SSP_Ctrl[SSP0].Send_Num--;

                if(SSP_Ctrl[SSP0].Send_Num == 0)
                {
                    SSP0IMSC &= (~IMSC_TXIM);
                    break;
                }
            }
            else
            {
                SSP0IMSC &= (~IMSC_TXIM);
                break;
            }
        }
    }
}
#endif
/******************************************************************************
    End Of File
******************************************************************************/

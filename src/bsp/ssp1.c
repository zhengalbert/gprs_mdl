/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : ssp1.c
 Description    : LPC17ϵ��оƬӲ��SSP1�ӳ���
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��    1.00        2010-07-27      snap           the original version
******************************************************************************/

#include "..\inc\global_config.h"

//-------------------------------- �������� -----------------------------------

//------------------------------ ˽�б������� ---------------------------------

//------------------------------ ˽�к������� ---------------------------------

static void ssp1_rst(void);
static void ssp1_init(void);

/******************************************************************************
** ��������: S32 ssp1_operation(U8 *send_buff, U8 *rcv_buff, U16 num, U8 operation)
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
S32 ssp1_operation (U8 *send_buff, U8 *rcv_buff, U16 num, U8 operation)
{
    S32 wait_time;

    SSP_Ctrl[SSP1].Operation = operation;
    SSP_Ctrl[SSP1].End_Flag = SSP_IDLE;
    SSP_Ctrl[SSP1].Send_Ptr = send_buff;
    SSP_Ctrl[SSP1].Rcv_Ptr = rcv_buff;
    SSP_Ctrl[SSP1].Send_Num = num;
    SSP_Ctrl[SSP1].Rcv_Num = num;
    SSP_Ctrl[SSP1].Error = NO_ERROR;

    ssp1_init();
    wait_time = num * 8 * (1000000 / SSP1_FREQ) * 10;       //�ȴ�10��������ͨѶʱ��
    while(wait_time--)                      //�ȴ�SSP�������
    {
        if(SSP_Ctrl[SSP1].End_Flag == SSP_END)
        {
            break;
        }
        delay(1);                           //��ʱԼ1uS
    }
    if(wait_time < 0)
    {
        SSP_Ctrl[SSP1].Error = TIME_OVF;    //�ȴ���ʱ
    }
    ssp1_rst();

    return(SSP_Ctrl[SSP1].Error);
}

/******************************************************************************
** ��������: void ssp1_init(void)
** ��������: ��ʼ��SSP1�����������߲���
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void ssp1_init (void)
{
    U8 k;

    PCONP |= (1 << PCSSP1);                 //enable SSP1
    ssp1_rst();                             //��ֹSSP1�����ж�

    PCLKSEL0 &= ~(0x03u << 20);
    PCLKSEL0 |= (FPCLK_SSP1_DIV << 20);     //��������ʱ�ӷ�Ƶϵ��

    if((PINSEL0 & ((0x03u << 14) | (0x03u << 16) | (0x03u << 18)))
      != ((P0_7_FNUC << 14) | (P0_8_FNUC << 16) | (P0_9_FNUC << 18)))               //���SCK1��MISO1��MOSI1�����Ź�������
    {
        PINSEL0 &= ~((0x03u << 14) | (0x03u << 16) | (0x03u << 18));                //��������bits
        PINSEL0 |= ((P0_7_FNUC << 14) | (P0_8_FNUC << 16) | (P0_9_FNUC << 18));     //configure the SCK1��MISO1 and MOSI1 as the alternate function 2 configuration
    }
    PINMODE0 &= ~((0x03u << 14) | (0x03u << 16) | (0x03u << 18));                   //��������bits
    PINMODE0 |= ((P0_7_MODE << 14) | (P0_8_MODE << 16) | (P0_9_MODE << 18));        //SCK1��MISO1 and MOSI1 has neither pull-up nor pull-down registers
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
    PINMODE_OD0 &= ~(P0_FLASH_SCK1 | P0_FLASH_SDO1 | P0_FLASH_SDI1); 
#else
    PINMODE_OD0 &= ~(P0_ESAMI_CLK | P0_ESAM_DI | P0_ESAM_DO);                       //�������ŷǿ�©ģʽ
#endif
    SSP1CPSR = CPSR_DIV;                // - PCLK / 2, 2��254 �е�һ��ż��ֵ

#if (FPCLK_SSP1 / CPSR_DIV / SSP1_FREQ) > 0
    k = FPCLK_SSP1 / CPSR_DIV / SSP1_FREQ - 1;
#else//(FPCLK_SSP1 / CPSR_DIV / SSP1_FREQ) > 0
#error("SPI1 bus frequency not allowed")
    k = 0;                              // - ��СΪ��0������2��Ƶ����ʱʵ�ʵ����ʲ���������ֵ��
#endif//(FPCLK_SSP1 / CPSR_DIV / SSP1_FREQ) > 0

    SSP1CR0  = ((0x07 << CR0_DSS) |     // - 8 bits data format
                (0x00 << CR0_FRF) |     // - SPI format
                (1 << CR0_CPOL) |       // - SPI maintains high between frames
                (1 << CRO_CPHA) |       // - capture the first clock transition
                (k << CRO_SCR));        // - clock number

    for(k = 0; k < SSP_FIFO_SIZE; k++)
    {
        SSP1DR;                         // - clear the RxFIFO
    }

    SSP1IMSC |= (IMSC_RORIM |           // - data is overwritten
                 IMSC_RTIM |            // - receive timeout occurs
                 IMSC_RXIM |            // - RxFIFO is at least half full
                 IMSC_TXIM);            // - TxFIFO is at least half empty

    SSP1CR1 |= 0x02;                    // - SSE enabled, SSP1 controller is enabled
//    ENABLE_ISR(NVIC_SSP1, PRIO_SSP1);   // - set SSP1 interrupt parameter
}

/******************************************************************************
** ��������: void ssp1_rst(void)
** ��������: ��λSSP1������ж�
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void ssp1_rst (void)
{
//    DISABLE_ISR(NVIC_SSP1);             // - disbale SSP1 interrupt
    SSP1CR1 = 0x00;                     // - disbale SSP1
    SSP1IMSC = 0x00;                    // - clear interrupt masked bits
    SSP1ICR = (ICR_RORIC | ICR_RTIC);   // - clear interrupt flag
}

/******************************************************************************
** ��������: void SSP1_IRQHandler(void)
** ��������: SSP1�жϷ������
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
**  2011-03-02 �����������ջ������20�ֽ�
******************************************************************************/
void SSP1_IRQHandler (void)
{
    U8 mis;

    mis = SSP1MIS;

    if(mis & MIS_RORMIS)                                //Rx FiFo overrun
    {
        while(SSP1SR & SR_RNE)
        {
            SSP1DR;
        }
        SSP1ICR |= ICR_RORIC;
        SSP_Ctrl[SSP1].Error = DATA_OVF;                //���ݽ������
//        DISABLE_ISR(NVIC_SSP1);                         //Disbale SSP1 interrupt
        SSP_Ctrl[SSP1].End_Flag = SSP_END;
    }

    if(mis & (MIS_RTMIS | MIS_RXMIS))                   //Receive timeout
    {
        while(SSP1SR & SR_RNE)
        {
            if(SSP_Ctrl[SSP1].Rcv_Num)
            {
                SSP_Ctrl[SSP1].Rcv_Num--;
            }

            if(SSP_Ctrl[SSP1].Operation == SSP_WR)
            {
                SSP1DR;
            }
            else if(SSP_Ctrl[SSP1].Operation == SSP_RD)
            {
                *SSP_Ctrl[SSP1].Rcv_Ptr++ = SSP1DR;
            }

            if(SSP_Ctrl[SSP1].Rcv_Num == 0)
            {
//                DISABLE_ISR(NVIC_SSP1);                 //Disbale SSP1 interrupt
                SSP_Ctrl[SSP1].End_Flag = SSP_END;
                break;
            }
        }
        SSP1ICR |= ICR_RTIC;
    }

    if(mis & MIS_TXMIS)                                 //Tx FiFo is atleast half empty
    {
        while(SSP1SR & SR_TNF)
        {
            if(SSP_Ctrl[SSP1].Send_Num)
            {
                if(SSP1SR & SR_RNE)
                {
                    if(SSP_Ctrl[SSP1].Rcv_Num)
                    {
                        SSP_Ctrl[SSP1].Rcv_Num--;
                    }

                    if(SSP_Ctrl[SSP1].Operation == SSP_WR)
                    {
                        SSP1DR;
                    }
                    else if(SSP_Ctrl[SSP1].Operation == SSP_RD)
                    {
                        *SSP_Ctrl[SSP1].Rcv_Ptr++ = SSP1DR;
                    }

                    if(SSP_Ctrl[SSP1].Rcv_Num == 0)
                    {
//                        DISABLE_ISR(NVIC_SSP1);         //Disbale SSP1 interrupt
                        SSP_Ctrl[SSP1].End_Flag = SSP_END;
                        break;
                    }
                }

                SSP1DR = *SSP_Ctrl[SSP1].Send_Ptr++;
                SSP_Ctrl[SSP1].Send_Num--;

                if(SSP_Ctrl[SSP1].Send_Num == 0)
                {
                    SSP1IMSC &= (~IMSC_TXIM);
                    break;
                }
            }
            else
            {
                SSP1IMSC &= (~IMSC_TXIM);
                break;
            }
        }
    }
}

/******************************************************************************
    End Of File
******************************************************************************/

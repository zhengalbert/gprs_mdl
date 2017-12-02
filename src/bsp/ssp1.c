/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : ssp1.c
 Description    : LPC17系列芯片硬件SSP1子程序
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2010-07-27      snap           the original version
******************************************************************************/

#include "..\inc\global_config.h"

//-------------------------------- 常数定义 -----------------------------------

//------------------------------ 私有变量声明 ---------------------------------

//------------------------------ 私有函数声明 ---------------------------------

static void ssp1_rst(void);
static void ssp1_init(void);

/******************************************************************************
** 函数名称: S32 ssp1_operation(U8 *send_buff, U8 *rcv_buff, U16 num, U8 operation)
** 函数功能: 读取或写入DataFlash数据，并等待中断完成总线操作
** 入口参数: *send_buff: 写入数据缓冲
**           *rcv_buff : 读取数据缓冲
**           num       : 数据长度
**           operation : 操作方式--读/写
** 返 回 值: indicating the result of the operation
**              1: normal
**             -3: waiting time overflow
**        -others: state error
** 说    明: 无
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
    wait_time = num * 8 * (1000000 / SSP1_FREQ) * 10;       //等待10倍的理论通讯时间
    while(wait_time--)                      //等待SSP操作完成
    {
        if(SSP_Ctrl[SSP1].End_Flag == SSP_END)
        {
            break;
        }
        delay(1);                           //延时约1uS
    }
    if(wait_time < 0)
    {
        SSP_Ctrl[SSP1].Error = TIME_OVF;    //等待超时
    }
    ssp1_rst();

    return(SSP_Ctrl[SSP1].Error);
}

/******************************************************************************
** 函数名称: void ssp1_init(void)
** 函数功能: 初始化SSP1，并启动总线操作
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void ssp1_init (void)
{
    U8 k;

    PCONP |= (1 << PCSSP1);                 //enable SSP1
    ssp1_rst();                             //禁止SSP1及其中断

    PCLKSEL0 &= ~(0x03u << 20);
    PCLKSEL0 |= (FPCLK_SSP1_DIV << 20);     //设置外设时钟分频系数

    if((PINSEL0 & ((0x03u << 14) | (0x03u << 16) | (0x03u << 18)))
      != ((P0_7_FNUC << 14) | (P0_8_FNUC << 16) | (P0_9_FNUC << 18)))               //检查SCK1、MISO1、MOSI1的引脚功能配置
    {
        PINSEL0 &= ~((0x03u << 14) | (0x03u << 16) | (0x03u << 18));                //先清除相关bits
        PINSEL0 |= ((P0_7_FNUC << 14) | (P0_8_FNUC << 16) | (P0_9_FNUC << 18));     //configure the SCK1、MISO1 and MOSI1 as the alternate function 2 configuration
    }
    PINMODE0 &= ~((0x03u << 14) | (0x03u << 16) | (0x03u << 18));                   //先清除相关bits
    PINMODE0 |= ((P0_7_MODE << 14) | (P0_8_MODE << 16) | (P0_9_MODE << 18));        //SCK1、MISO1 and MOSI1 has neither pull-up nor pull-down registers
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
    PINMODE_OD0 &= ~(P0_FLASH_SCK1 | P0_FLASH_SDO1 | P0_FLASH_SDI1); 
#else
    PINMODE_OD0 &= ~(P0_ESAMI_CLK | P0_ESAM_DI | P0_ESAM_DO);                       //设置引脚非开漏模式
#endif
    SSP1CPSR = CPSR_DIV;                // - PCLK / 2, 2～254 中的一个偶数值

#if (FPCLK_SSP1 / CPSR_DIV / SSP1_FREQ) > 0
    k = FPCLK_SSP1 / CPSR_DIV / SSP1_FREQ - 1;
#else//(FPCLK_SSP1 / CPSR_DIV / SSP1_FREQ) > 0
#error("SPI1 bus frequency not allowed")
    k = 0;                              // - 最小为‘0’，即2分频（这时实际的速率不符合设置值）
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
** 函数名称: void ssp1_rst(void)
** 函数功能: 复位SSP1，清除中断
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void ssp1_rst (void)
{
//    DISABLE_ISR(NVIC_SSP1);             // - disbale SSP1 interrupt
    SSP1CR1 = 0x00;                     // - disbale SSP1
    SSP1IMSC = 0x00;                    // - clear interrupt masked bits
    SSP1ICR = (ICR_RORIC | ICR_RTIC);   // - clear interrupt flag
}

/******************************************************************************
** 函数名称: void SSP1_IRQHandler(void)
** 函数功能: SSP1中断服务程序
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
**  2011-03-02 编译器报告堆栈最大深度20字节
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
        SSP_Ctrl[SSP1].Error = DATA_OVF;                //数据接收溢出
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

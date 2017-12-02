/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : ssp0.c
 Description    : LPC17系列芯片硬件SSP0子程序
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、    1.01		  2011-06-24	  azh			 用于FLASH读写
   1、    1.00        2010-08-19      snap           the original version
******************************************************************************/

#include "..\inc\global_config.h"

//-------------------------------- 常数定义 -----------------------------------

//------------------------------ 私有变量声明 ---------------------------------

//------------------------------ 私有函数声明 ---------------------------------
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不是CPU_GPRS模块
static void ssp0_rst(void);
#endif
/******************************************************************************
** 函数名称: S32 ssp0_operation(U8 *send_buff, U8 *rcv_buff, U16 num, U8 operation)
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
S32 ssp0_operation (U8 *send_buff, U8 *rcv_buff, U16 num, U8 operation)
{
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不是CPU_GPRS模块
    S32 wait_time;

    SSP_Ctrl[SSP0].Operation = operation;
    SSP_Ctrl[SSP0].End_Flag = SSP_IDLE;
    SSP_Ctrl[SSP0].Send_Ptr = send_buff;
    SSP_Ctrl[SSP0].Rcv_Ptr = rcv_buff;
    SSP_Ctrl[SSP0].Send_Num = num;
    SSP_Ctrl[SSP0].Rcv_Num = num;
    SSP_Ctrl[SSP0].Error = NO_ERROR;

    ssp0_init();
    wait_time = num * 8 * (1000000 / SSP0_FREQ) * 10;       //等待10倍的理论通讯时间
    while(wait_time--)                      //等待SSP操作完成
    {
        if(SSP_Ctrl[SSP0].End_Flag == SSP_END)
        {
            break;
        }
        delay(1);                           //延时约1uS
    }
    if(wait_time < 0)
    {
        SSP_Ctrl[SSP0].Error = TIME_OVF;    //等待超时
    }
    ssp0_rst();

    return(SSP_Ctrl[SSP0].Error);
#else
	return 1;

#endif
}
/******************************************************************************
** 函数名称: void ssp0_init(void)
** 函数功能: 初始化SSP0，并启动总线操作
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不是CPU_GPRS模块
void ssp0_init (void)
{
    U8 k;

    PCONP |= (1 << PCSSP0);                 //enable SSP0
    ssp0_rst();                             //禁止SSP0及其中断

    PCLKSEL1 &= ~(0x03u << 10);
    PCLKSEL1 |= (FPCLK_SSP0_DIV << 10);     //设置外设时钟分频系数

    if((PINSEL3 & ((0x03u << 8) | (0x03u << 14) | (0x03u << 16)))
      != ((P1_20_FNUC << 8) | (P1_23_FNUC << 14) | (P1_24_FNUC << 16)))             //检查SCK0、MISO0、MOSI0的引脚功能配置
    {
        PINSEL3 &= ~((0x03u << 8) | (0x03u << 14) | (0x03u << 16));                 //先清除相关bits
        PINSEL3 |= ((P1_20_FNUC << 8) | (P1_23_FNUC << 14) | (P1_24_FNUC << 16));   //configure the SCK0、MISO0 and MOSI0 as the alternate function 2 configuration
    }
    PINMODE3 &= ~((0x03u << 8) | (0x03u << 14) | (0x03u << 16));                    //先清除相关bits
    PINMODE3 |= ((P1_20_MODE << 8) | (P1_23_MODE << 14) | (P1_24_MODE << 16));      //SCK0、MISO0 and MOSI0 has neither pull-up nor pull-down registers
    PINMODE_OD1 &= ~(P1_F1_SCK | P1_F1_MISO | P1_F1_MOSI);                //设置引脚非开漏模式

    SSP0CPSR = CPSR_DIV;                // - PCLK / 2, 2～254 中的一个偶数值

#if (FPCLK_SSP0 / CPSR_DIV / SSP0_FREQ) > 0
    k = FPCLK_SSP0 / CPSR_DIV / SSP0_FREQ - 1;
#else//(FPCLK_SSP0 / CPSR_DIV / SSP0_FREQ) > 0
#error("SPI0 bus frequency not allowed")
    k = 0;                              // - 最小为‘0’，即2分频（这时实际的速率不符合设置值）
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
** 函数名称: void ssp0_rst(void)
** 函数功能: 复位SSP0，清除中断
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不是CPU_GPRS模块
void ssp0_rst (void)
{
//    DISABLE_ISR(NVIC_SSP0);             // - disbale SSP0 interrupt
    SSP0CR1 = 0x00;                     // - disbale SSP0
    SSP0IMSC = 0x00;                    // - clear interrupt masked bits
    SSP0ICR = (ICR_RORIC | ICR_RTIC);   // - clear interrupt flag
}
#endif
/******************************************************************************
** 函数名称: void SSP0_IRQHandler(void)
** 函数功能: SSP0中断服务程序
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不是CPU_GPRS模块
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
        SSP_Ctrl[SSP0].Error = DATA_OVF;                //数据接收溢出
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

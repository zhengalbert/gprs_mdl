/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : lpc1700_pin_cfg.h
 Description    : LPC1700系列CPU引脚连接与模式配置
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   3、    2.00        2011-06-16      LFG            显示终端管脚配置
   2、    1.01        2010-08-20      LFG            CS5463的中断脚/INT由原来P1_21改到P0_30作为外部中断输入
   1、    1.00        2010-07-16      snap           the original version
******************************************************************************/

#ifndef __LPC1700_PIN_CFG_H
#define __LPC1700_PIN_CFG_H

/*----------------------------- macro definition ----------------------------*/

#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
/******************************************************************************
** P0口引脚功能设置
******************************************************************************/
/*
 *  P0.0
 */
#define P0_0_GPIO             0x00                                      /* GPIO                         */
#define P0_0_RD1              0x01                                      /* CAN-1控制器接收引脚          */
#define P0_0_TXD3             0x02                                      /* UART3发送引脚                */
#define P0_0_SDA1             0x03                                      /* I2C-1数据线（开漏）          */

#define P0_0_FNUC             P0_0_SDA1

/*
 *  P0.1
 */
#define P0_1_GPIO             0x00                                      /* GPIO                         */
#define P0_1_TD1              0x01                                      /* CAN-1控制器发送引脚          */
#define P0_1_RXD3             0x02                                      /* UART3接收引脚                */
#define P0_1_SCL1             0x03                                      /* I2C-1时钟线（开漏）          */

#define P0_1_FNUC             P0_1_SCL1

/*
 *  P0.2
 */
#define P0_2_GPIO             0x00                                      /* GPIO                         */
#define P0_2_TXD0             0x01                                      /* UART0发送引脚                */
#define P0_2_AD07             0x02                                      /* AD0.7引脚                    */
//...   Reserved              0x03

#define P0_2_FNUC             P0_2_TXD0

/*
 *  P0.3
 */
#define P0_3_GPIO             0x00                                      /* GPIO                         */
#define P0_3_RXD0             0x01                                      /* UART0接收引脚                */
#define P0_3_AD06             0x02                                      /* AD0.6引脚                    */
//...   Reserved              0x03

#define P0_3_FNUC             P0_3_RXD0

/*
 *  P0.4
 */
#define P0_4_GPIO             0x00                                      /* GPIO                         */
#define P0_4_I2SRX_CLK        0x01                                      /* I2S总线接收时钟SCK           */
#define P0_4_RD2              0x02                                      /* CAN-2接收线                  */
#define P0_4_CAP20            0x03                                      /* 定时器2捕获输入，通道0       */

#define P0_4_FNUC             P0_4_GPIO

/*
 *  P0.5
 */
#define P0_5_GPIO             0x00                                      /* GPIO                         */
#define P0_5_I2SRX_WS         0x01                                      /* I2S总线接收字选择WS          */
#define P0_5_TD2              0x02                                      /* CAN-2发送线                  */
#define P0_5_CAP21            0x03                                      /* 定时器2捕获输入，通道1       */

#define P0_5_FNUC             P0_5_GPIO

/*
 *  P0.6
 */
#define P0_6_GPIO             0x00                                      /* GPIO                         */
#define P0_6_I2SRX_SDA        0x01                                      /* I2S总线接收数据线SD          */
#define P0_6_SSEL1            0x02                                      /* SSP-1总线从机选择            */
#define P0_6_MAT20            0x03                                      /* 定时器2匹配输出，通道0       */

#define P0_6_FNUC             P0_6_GPIO

/*
 *  P0.7
 */
#define P0_7_GPIO             0x00                                      /* GPIO                         */
#define P0_7_I2STX_CLK        0x01                                      /* I2S总线发送时钟线SCK         */
#define P0_7_SSCK1            0x02                                      /* SSP-1时钟线                  */
#define P0_7_MAT21            0x03                                      /* 定时器2匹配输出，通道1       */

//#ifdef PHASE_C_MODULE
#define P0_7_FNUC             P0_7_SSCK1
//#else// PHASE_C_MODULE
//#define P0_7_FNUC             P0_7_GPIO
//#endif// PHASE_C_MODULE

/*
 *  P0.8
 */
#define P0_8_GPIO             0x00                                      /* GPIO                         */
#define P0_8_I2STX_WS         0x01                                      /* I2S总线发送字选择WS          */
#define P0_8_SMISO1           0x02                                      /* SSP-1主输入从输出数据线      */
#define P0_8_MAT22            0x03                                      /* 定时器2匹配输出，通道2       */

#define P0_8_FNUC             P0_8_SMISO1

/*
 *  P0.9
 */
#define P0_9_GPIO             0x00                                      /* GPIO                         */
#define P0_9_I2STX_SDA        0x01                                      /* I2S总线发送数据线SD          */
#define P0_9_SMOSI1           0x02                                      /* SSP-1主输出从输入数据线      */
#define P0_9_MAT23            0x03                                      /* 定时器2匹配输出，通道3       */

#define P0_9_FNUC             P0_9_SMOSI1

/*
 *  P0.10
 */
#define P0_10_GPIO            0x00                                      /* GPIO                         */
#define P0_10_TXD2            0x01                                      /* UART2发送引脚                */
#define P0_10_SDA2            0x02                                      /* I2C-2数据线(非开漏引脚)      */
#define P0_10_MAT30           0x03                                      /* 定时器3匹配输出，通道0       */

#define P0_10_FNUC            P0_10_TXD2

/*
 *  P0.11
 */
#define P0_11_GPIO            0x00                                      /* GPIO                         */
#define P0_11_RXD2            0x01                                      /* UART2接收引脚                */
#define P0_11_SCL2            0x02                                      /* I2C-2时钟线(非开漏引脚)      */
#define P0_11_MAT31           0x03                                      /* 定时器3匹配输出，通道1       */

#define P0_11_FNUC            P0_11_RXD2

/*
 *  P0.15
 */
#define P0_15_GPIO            0x00                                      /* GPIO                         */
#define P0_15_TXD1            0x01                                      /* UART1发送引脚                */
#define P0_15_SCK0            0x02                                      /* SSP0时钟线                   */
#define P0_15_SCK             0x03                                      /* SPI时钟线                    */

#define P0_15_FNUC            P0_15_GPIO

/*
 *  P0.16
 */
#define P0_16_GPIO            0x00                                      /* GPIO                         */
#define P0_16_RXD1            0x01                                      /* UART1接收引脚                */
#define P0_16_SSEL0           0x02                                      /* SSP0从机选择                 */
#define P0_16_SSEL            0x03                                      /* SPI从机选择                  */

#define P0_16_FNUC            P0_16_GPIO

/*
 *  P0.17
 */
#define P0_17_GPIO            0x00                                      /* GPIO                         */
#define P0_17_CTS1            0x01                                      /* UART1清零发送信号CTS         */
#define P0_17_MISO0           0x02                                      /* SSP0主输入从输出数据线       */
#define P0_17_MISO            0x03                                      /* SPI主入从出数据线            */

#define P0_17_FNUC            P0_17_GPIO

/*
 *  P0.18
 */
#define P0_18_GPIO            0x00                                      /* GPIO                         */
#define P0_18_DCD1            0x01                                      /* UART1数据载波检测输入DCD     */
#define P0_18_MOSI0           0x02                                      /* SSP-0主输出从输入数据线      */
#define P0_18_MOSI            0x03                                      /* SPI主出从入数据线            */

#define P0_18_FNUC            P0_18_GPIO

/*
 *  P0.19
 */
#define P0_19_GPIO            0x00                                      /* GPIO                         */
#define P0_19_DSR1            0x01                                      /* UART1数据设置就绪DSR         */
//...   Reserved              0x02                                      /* 保留                         */
#define P0_19_SDA1            0x03                                      /* I2C-1数据线（非开漏）        */

#define P0_19_FNUC            P0_19_GPIO

/*
 *  P0.20
 */
#define P0_20_GPIO            0x00                                      /* GPIO                         */
#define P0_20_DTR1            0x01                                      /* UART1终端就绪DTR             */
//...   Reserved              0x02                                      /* 保留                         */
#define P0_20_SCL1            0x03                                      /* I2C-1时钟线（非开漏）        */

#define P0_20_FNUC            P0_20_GPIO

/*
 *  P0.21
 */
#define P0_21_GPIO            0x00                                      /* GPIO                         */
#define P0_21_RI1             0x01                                      /* UART1振铃信号                */
//...   Reserved              0x02                                      /* 保留                         */
#define P0_21_RD1             0x03                                      /* CAN-1接收引脚                */

#define P0_21_FNUC            P0_21_GPIO

/*
 *  P0.22
 */
#define P0_22_GPIO            0x00                                      /* GPIO                         */
#define P0_22_RTS1            0x01                                      /* UART1请求发送引脚            */
//...   Reserved              0x02                                      /* 保留                         */
#define P0_22_TD1             0x03                                      /* CAN-1发送线                  */

#define P0_22_FNUC            P0_22_GPIO

/*
 *  P0.23
 */
#define P0_23_GPIO            0x00                                      /* GPIO                         */
#define P0_23_AD00            0x01                                      /* ADC-0，通道0                 */
#define P0_23_I2SRX_CLK       0x02                                      /* I2S接收时钟SCK               */
#define P0_23_CAP30           0x03                                      /* 定时器3捕获输入，通道0       */

#define P0_23_FNUC            P0_23_AD00

/*
 *  P0.24
 */
#define P0_24_GPIO            0x00                                      /* GPIO                         */
#define P0_24_AD01            0x01                                      /* ADC-0，通道1                 */
#define P0_24_I2SRX_WS        0x02                                      /* I2S接收字选择WS              */
#define P0_24_CAP31           0x03                                      /* 定时器3捕获输入，通道1       */

#define P0_24_FNUC            P0_24_GPIO

/*
 *  P0.25
 */
#define P0_25_GPIO            0x00                                      /* GPIO                         */
#define P0_25_AD02            0x01                                      /* ADC-0，通道2                 */
#define P0_25_I2SRX_SDA       0x02                                      /* I2S接收数据SD                */
#define P0_25_TXD3            0x03                                      /* UART3发送引脚                */

#define P0_25_FNUC            P0_25_GPIO

/*
 *  P0.26
 */
#define P0_26_GPIO            0x00                                      /* GPIO                         */
#define P0_26_AD03            0x01                                      /* ADC-0，通道3                 */
#define P0_26_AOUT            0x02                                      /* DAC输出                      */
#define P0_26_RXD3            0x03                                      /* UART-3接收引脚               */

#define P0_26_FNUC            P0_26_GPIO

/*
 *  P0.27
 */
#define P0_27_GPIO            0x00                                      /* GPIO                         */
#define P0_27_SDA0            0x01                                      /* I2C0数据输入/输出，开漏      */
#define P0_27_USB_SDA         0x02                                      /* USB接口I2C串行数据OTG收发器  */
//..    Reserved              0x03                                      /* 保留                         */

#define P0_27_FNUC            P0_27_GPIO

/*
 *  P0.28
 */
#define P0_28_GPIO            0x00                                      /* GPIO                         */
#define P0_28_SCL0            0x01                                      /* I2C0时钟输入，开漏           */
#define P0_28_USB_SCL         0x02                                      /* USB接口I2C时钟线，OTG收发器  */
//..    Reserved              0x03                                   /* 保留                         */

#define P0_28_FNUC            P0_28_GPIO

/*
 *  P0.29
 */
#define P0_29_GPIO            0x00                                      /* GPIO                         */
#define P0_29_USB_DD          0x01                                      /* USB双向数据D+线              */
//..    Reserved              0x02                                      /* 保留                         */
//..    Reserved              0x03                                      /* 保留                         */

#define P0_29_FNUC            P0_29_GPIO

/*
 *  P0.30
 */
#define P0_30_GPIO            0x00                                      /* GPIO                         */
#define P0_30_USB_DS          0x01                                      /* USB双向数据D-线              */
//..    Reserved              0x02                                      /* 保留                         */
//..    Reserved              0x03                                      /* 保留                         */

#define P0_30_FNUC            P0_30_GPIO

/******************************************************************************
** P1口引脚功能设置
******************************************************************************/
/*
 *  P1.0
 */
#define P1_0_GPIO             0x00                                      /* GPIO                         */
#define P1_0_ENET_TXD0        0x01                                      /* 以太网发送数据脚0            */
//..    Reserved              0x02                                      /* 保留                         */
//..    Reserved              0x03                                      /* 保留                         */

#define P1_0_FNUC             P1_0_GPIO

/*
 *  P1.1
 */
#define P1_1_GPIO             0x00                                      /* GPIO                         */
#define P1_1_ENET_TX_EN       0x01                                      /* 以太网发送数据脚1            */
//..    Reserved              0x02                                      /* 保留                         */
//..    Reserved              0x03                                      /* 保留                         */

#define P1_1_FNUC             P1_1_GPIO

/*
 *  P1.4
 */
#define P1_4_GPIO             0x00                                      /* GPIO                         */
#define P1_4_ENET_TX_EN       0x01                                      /* 以太网发送使能               */
//..    Reserved              0x02                                      /* 保留                         */
//..    Reserved              0x03                                      /* 保留                         */

#define P1_4_FNUC             P1_4_GPIO

/*
 *  P1.8
 */
#define P1_8_GPIO             0x00                                      /* GPIO                         */
#define P1_8_ENET_CRS         0x01                                      /* 以太网载波侦测脚             */
//..    Reserved              0x02                                      /* 保留                         */
//..    Reserved              0x03                                      /* 保留                         */

#define P1_8_FNUC             P1_8_GPIO

/*
 *  P1.9
 */
#define P1_9_GPIO             0x00                                      /* GPIO                         */
#define P1_9_ENET_RXD0        0x01                                      /* 以太网接收数据脚0            */
//..    Reserved              0x02                                      /* 保留                         */
//..    Reserved              0x03                                      /* 保留                         */

#define P1_9_FNUC             P1_9_GPIO

/*
 *  P1.10
 */
#define P1_10_GPIO            0x00                                      /* GPIO                         */
#define P1_10_ENET_RXD1       0x01                                      /* 以太网接收数据脚1            */
//..    Reserved              0x02                                      /* 保留                         */
//..    Reserved              0x03                                      /* 保留                         */

#define P1_10_FNUC            P1_10_GPIO

/*
 *  P1.11
 */
#define P1_14_GPIO            0x00                                      /*  GPIO                        */
#define P1_14_ENET_RX_ER      0x01                                      /* 以太网接收错误               */
//..    Reserved              0x02                                      /* 保留                         */
//..    Reserved              0x03                                      /* 保留                         */

#define P1_14_FNUC            P1_14_GPIO

/*
 *  P1.15
 */
#define P1_15_GPIO            0x00                                      /* GPIO                         */
#define P1_15_ENET_REF_CLK    0x01                                      /* 以太网参拷时钟输入           */
//..    Reserved              0x02                                      /* 保留                         */
//..    Reserved              0x03                                      /* 保留                         */

#define P1_15_FNUC            P1_15_GPIO

/*
 *  P1.16
 */
#define P1_16_GPIO            0x00                                      /* GPIO                         */
#define P1_16_ENET_MDC        0x01                                      /* 以太网MIIM时钟               */
//..    Reserved              0x02                                      /* 保留                         */
//..    Reserved              0x03                                      /* 保留                         */

#define P1_16_FNUC            P1_16_GPIO

/*
 *  P1.17
 */
#define P1_17_GPIO            0x00                                      /* GPIO                         */
#define P1_17_ENET_MDIO       0x01                                      /* 以太网MI数据输入\输出        */
//..    Reserved              0x02                                      /* 保留                         */
//..    Reserved              0x03                                      /* 保留                         */

#define P1_17_FNUC            P1_17_GPIO

/*
 *  P1.18
 */
#define P1_18_GPIO            0x00                                      /*  GPIO                        */
#define P1_18_USB_UP_LED      0x01                                      /* USB GOODLINK指示脚           */
#define P1_18_PWM11           0x02                                      /* PWM1输出，通道1              */
#define P1_18_CAP10           0x03                                      /* Timer1捕获输入，通道0        */

#define P1_18_FNUC            P1_18_GPIO//

/*
 *  P1.19
 */
#define P1_19_GPIO            0x00                                      /*  GPIO                        */
#define P1_19_MCO0A           0x01                                      /* 电机控制PWM通道0，输出A      */
#define P1_19_USB_PPWR        0x02                                      /* USB端口电源使能信号          */
#define P1_19_CAP11           0x03                                      /* Timer1捕获输入，通道1        */

#define P1_19_FNUC            P1_19_GPIO

/*
 *  P1.20
 */
#define P1_20_GPIO            0x00                                      /*  GPIO                        */
#define P1_20_MCFB0           0x01                                      /* 电机控制PWM通道0，回环输入   */
#define P1_20_PWM12           0x02                                      /* PWM-2输出，通道2             */
#define P1_20_SCK0            0x03                                      /* SSP-0时钟输入                */

#define P1_20_FNUC            P1_20_GPIO

/*
 *  P1.21
 */
#define P1_21_GPIO            0x00                                      /* GPIO                         */
#define P1_21_MCABORT         0x01                                      /* 电机控制PWM，紧急中止        */
#define P1_21_PWM13           0x02                                      /* PWM-1输出，通道3             */
#define P1_21_SSEL0           0x03                                      /* SSP-0从机选择                */

#define P1_21_FNUC            P1_21_GPIO

/*
 *  P1.22
 */
#define P1_22_GPIO            0x00                                      /* GPIO                         */
#define P1_22_MC0B            0x01                                      /* 电机控制PWM通道0，输出B      */
#define P1_22_USB_PWRD        0x02                                      /* USB端口的电源状态            */
#define P1_22_MAT10           0x03                                      /* 定时器1匹配输出，通道0       */

#define P1_22_FNUC            P1_22_GPIO

/*
 *  P1.23
 */
#define P1_23_GPIO            0x00                                      /* GPIO                         */
#define P1_23_MCFB1           0x01                                      /* 电机控制PWM通道1，反馈输入   */
#define P1_23_PWM14           0x02                                      /* PWM-1输出，通道4             */
#define P1_23_MISO0           0x03                                      /* SSP-0主机输入，从机输出      */

#define P1_23_FNUC            P1_23_GPIO

/*
 *  P1.24
 */
#define P1_24_GPIO            0x00                                      /* GPIO                         */
#define P1_24_MCFB2           0x01                                      /* 电机控制PWM通道2，反馈输入   */
#define P1_24_PWM15           0x02                                      /* PWM-1输出，通道5             */
#define P1_24_MOSI0           0x03                                      /* SSP-0主机输出，从机输入      */

#define P1_24_FNUC            P1_24_GPIO

/*
 *  P1.25
 */
#define P1_25_GPIO            0x00                                      /* GPIO                         */
#define P1_25_MC1A            0x01                                      /* 电机控制PWM通道1，输出A      */
#define P1_25_CLKOUT          0x02                                      /* 时钟输出引脚                 */
#define P1_25_MAT11           0x03                                      /* 定时器1匹配输出，通道1       */

#define P1_25_FNUC            P1_25_GPIO

/*
 *  P1.26
 */
#define P1_26_GPIO            0x00                                      /* GPIO                         */
#define P1_26_MC1B            0x01                                      /* 电机控制PWM通道1，输出B      */
#define P1_26_PWM16           0x02                                      /* PWM-1输出，通道6             */
#define P1_26_CAP00           0x03                                      /* 定时器0捕获输入，通道0       */

#define P1_26_FNUC            P1_26_PWM16//P1_26_GPIO

/*
 *  P1.27
 */
#define P1_27_GPIO            0x00                                      /* GPIO                         */
#define P1_27_CLKOUT          0x01                                      /* 时钟输出引脚                 */
#define P1_27_USB_OVRCR       0x02                                      /* USB端口过载电流状态          */
#define P1_27_CAP01           0x03                                      /* 定时器0捕获输入，通道1       */

#define P1_27_FNUC            P1_27_GPIO

/*
 *  P1.28
 */
#define P1_28_GPIO            0x00                                      /* GPIO                         */
#define P1_28_MC2A            0x01                                      /* 电机控制PWM通道2，输出A      */
#define P1_28_PCAP10          0x02                                      /* PWM-1捕获输入，通道0         */
#define P1_28_MAT00           0x03                                      /* 定时器0匹配输出，通道0       */

#define P1_28_FNUC            P1_28_GPIO

/*
 *  P1.29
 */
#define P1_29_GPIO            0x00                                      /* GPIO                         */
#define P1_29_MC2B            0x01                                      /* 电机控制PWM通道2，输出B      */
#define P1_29_PCAP11          0x02                                      /* PWM-1捕获输入，通道1         */
#define P1_29_MAT01           0x03                                      /* 定时器0匹配输出，通道1       */

#define P1_29_FNUC            P1_29_GPIO

/*
 *  P1.30
 */
#define P1_30_GPIO            0x00                                      /* GPIO                         */
//...   Reserved              0x01                                      /* 保留                         */
#define P1_30_VBUS            0x02                                      /* 存在的USB总线电源            */
#define P1_30_AD04            0x03                                      /* ADC转换器0.通道4输入         */

#define P1_30_FNUC            P1_30_GPIO

/*
 *  P1.31
 */
#define P1_31_GPIO            0x00                                      /* GPIO                         */
//...   Reserved              0x01                                      /* 保留                         */
#define P1_31_SSCK1           0x02                                      /* SSP-1时钟线                  */
#define P1_31_AD05            0x03                                      /* ADC-0输入，通道5             */

#define P1_31_FNUC            P1_31_GPIO

/******************************************************************************
** P2口引脚功能设置
******************************************************************************/
/*
 *  P2.0
 */
#define P2_0_GPIO             0x00                                      /* GPIO                         */
#define P2_0_PWM11            0x01                                      /* PWM-1，通道1                 */
#define P2_0_TXD1             0x02                                      /* UART1数据发送引脚            */
//...   Reserved              0x03                                      /* 保留                         */

#define P2_0_FNUC             P2_0_TXD1

/*
 *  P2.1
 */
#define P2_1_GPIO             0x00                                      /* GPIO                         */
#define P2_1_PWM12            0x01                                      /* PWM-1，通道2                 */
#define P2_1_RXD1             0x02                                      /* UART1数据接收引脚            */
//...   Reserved              0x03                                      /* 保留                         */

#define P2_1_FNUC             P2_1_RXD1

/*
 *  P2.2
 */
#define P2_2_GPIO             0x00                                      /* GPIO                         */
#define P2_2_PWM13            0x01                                      /* PWM-1，通道3                 */
#define P2_2_CTS1             0x02                                      /* UART1清零发送线              */
//...   Reserved              0x03                                      /* 保留                         */

#define P2_2_FNUC             P2_2_CTS1//azh 171017//P2_2_GPIO

/*
 *  P2.3
 */
#define P2_3_GPIO             0x00                                      /* GPIO                         */
#define P2_3_PWM14            0x01                                      /* PWM-1，通道4                 */
#define P2_3_DCD1             0x02                                      /* UART1数据载波检测线          */
//...   Reserved              0x03                                      /* 保留                         */

#define P2_3_FNUC             P2_3_GPIO

/*
 *  P2.4
 */
#define P2_4_GPIO             0x00                                      /* GPIO                         */
#define P2_4_PWM15            0x01                                      /* PWM-1，通道5                 */
#define P2_4_DSR1             0x02                                      /* UART1数据设置就绪线          */
//...   Reserved              0x03                                      /* 保留                         */

#define P2_4_FNUC             P2_4_GPIO

/*
 *  P2.5
 */
#define P2_5_GPIO             0x00                                      /* GPIO                         */
#define P2_5_PWM16            0x01                                      /* PWM-1，通道6                 */
#define P2_5_DTR1             0x02                                      /* UART1数据终端就绪线          */
//...   Reserved              0x03                                      /* 保留                         */

#define P2_5_FNUC             P2_5_GPIO

/*
 *  P2.6
 */
#define P2_6_GPIO             0x00                                      /* GPIO                         */
#define P2_6_PCAP10           0x01                                      /* PWM-1捕获输入，通道0         */
#define P2_6_RI1              0x02                                      /* UART1振铃输入                */
//...   Reserved              0x03                                      /* 保留                         */

#define P2_6_FNUC             P2_6_GPIO

/*
 *  P2.7
 */
#define P2_7_GPIO             0x00                                      /* GPIO                         */
#define P2_7_RD2              0x01                                      /* CAN-2接收线                  */
#define P2_7_RTS1             0x02                                      /* UART1请求发送输出            */
                                                                        /* RS-485/EIA-485输出使能信号   */
//...   Reserved              0x03                                      /* 保留                         */

#define P2_7_FNUC             P2_7_RTS1//P2_7_GPIO

/*
 *  P2.8
 */
#define P2_8_GPIO             0x00                                      /* GPIO                         */
#define P2_8_TD2              0x01                                      /* CAN-2发送线                  */
#define P2_8_TXD2             0x02                                      /* UART2发送引脚                */
#define P2_8_ENET_MDC         0x03                                      /* 以太网MIIM时钟               */

#define P2_8_FNUC             P2_8_GPIO

/*
 *  P2.9
 */
#define P2_9_GPIO             0x00                                      /* GPIO                         */
#define P2_9_U1CONNECT        0x01                                      /* USB1软连接控制               */
#define P2_9_RXD2             0x02                                      /* UART2接收引脚                */
#define P2_9_ENET_MDIO        0x03                                      /* 以太网MIIM数据输入和输出     */

#define P2_9_FNUC             P2_9_GPIO

/*
 *  P2.10（ISP）
 */
#define P2_10_GPIO            0x00                                      /* GPIO                         */
#define P2_10_EINT0           0x01                                      /* 外部中断0输入                */
#define P2_10_NMI             0x02                                      /* 非屏蔽中断输入               */
//...   Reserved              0x03                                      /* 保留                         */

#define P2_10_FNUC            P2_10_GPIO

/*
 *  P2.11
 */
#define P2_11_GPIO            0x00                                      /* GPIO                         */
#define P2_11_EINT1           0x01                                      /* 外部中断1输入                */
//...   Reserved              0x02                                      /* 保留                         */
#define P2_11_I2STX_CLK       0x03                                      /* I2S发送时钟SCK               */
#define P2_11_FNUC            P2_11_GPIO
//#define P2_11_FNUC            P2_11_EINT1

/*
 *  P2.12
 */
#define P2_12_GPIO            0x00                                      /* GPIO                         */
#define P2_12_EINT2           0x01                                      /* 外部中断2输入                */
//...   Reserved              0x02                                      /* 保留                         */
#define P2_12_I2STX_WS        0x03                                      /* I2S发送字选择WS              */
#define P2_12_FNUC            P2_12_GPIO
//#define P2_12_FNUC            P2_12_EINT2

/*
 *  P2.13
 */
#define P2_13_GPIO            0x00                                      /* GPIO                         */
#define P2_13_EINT3           0x01                                      /* 外部中断3输入                */
//...   Reserved              0x02                                      /* 保留                         */
#define P2_13_I2STX_SDA       0x03                                      /* I2S发送数据SD                */

#define P2_13_FNUC            P2_13_GPIO//P2_13_EINT3

/******************************************************************************
** P3口引脚功能设置
******************************************************************************/
/*
 *  P3.25
 */
#define P3_25_GPIO            0x00                                      /*  GPIO                        */
//...   Reserved              0x01                                      /*  保留                        */
#define P3_25_MAT00           0x02                                      /*  定时器0匹配输出，通道0      */
#define P3_25_PWM12           0x03                                      /*  PWM-1输出，通道2            */

#define P3_25_FNUC            P3_25_GPIO

/*
 *  P3.26
 */
#define P3_26_GPIO            0x00                                      /* GPIO                         */
#define P3_26_STCLK           0x01                                      /* 系统节拍定时器时钟输入       */
#define P3_26_MAT01           0x02                                      /* 定时器0匹配输出，通道1       */
#define P3_26_PWM13           0x03                                      /* PWM-1输出，通道3             */

#define P3_26_FNUC            P3_26_GPIO

/******************************************************************************
** P4口引脚功能设置
******************************************************************************/
/*
 *  P4.28
 */
#define P4_28_GPIO            0x00                                      /* GPIO                         */
#define P4_28_RX_MCLK         0x01                                      /* —I2S接收主机时钟            */
#define P4_28_MAT20           0x02                                      /* 定时器2匹配输出，通道0       */
#define P4_28_TXD3            0x03                                      /* UART3发送引脚                */

#define P4_28_FNUC            P4_28_TXD3

/*
 *  P4.29
 */
#define P4_29_GPIO            0x00                                      /* GPIO                         */
#define P4_29_TX_MCLK         0x01                                      /* I2S发送主机时钟              */
#define P4_29_MAT21           0x02                                      /* 定时器2匹配输出，通道1       */
#define P4_29_RXD3            0x03                                      /* UART3接收引脚                */

#define P4_29_FNUC            P4_29_RXD3


/******************************************************************************
** P10  The Trace Port Interface Unit (TPIU)
******************************************************************************/

/*
 *  bit[3]
 */
#define TPIU_Disable          0x00                                      /* TPIU interface 禁止          */
#define TPIU_Enable           0x01                                      /* TPIU interface 使能          */

#define P10_3_FNUC            TPIU_Disable

/******************************************************************************
** 引脚内部上下拉电阻配置
******************************************************************************/
#define PIN_PULL_UP           0x00u                                     /* 引脚内部上拉                 */
#define PIN_REPEATER          0x01u                                     /* 引脚内部上拉                 */
#define PIN_NO_PULL           0x02u                                     /* 引脚既不上拉，也不下拉       */
#define PIN_PULL_DOWN         0x03u                                     /* 引脚内部下拉                 */

/*
 *  P0
 */
#define P0_0_MODE             PIN_NO_PULL
#define P0_1_MODE             PIN_NO_PULL
#define P0_2_MODE             PIN_NO_PULL
#define P0_3_MODE             PIN_NO_PULL
#define P0_4_MODE             PIN_NO_PULL         //下拉
#define P0_5_MODE             PIN_NO_PULL         //下拉
#define P0_6_MODE             PIN_NO_PULL           //上拉
#define P0_7_MODE             PIN_NO_PULL
#define P0_8_MODE             PIN_NO_PULL
#define P0_9_MODE             PIN_NO_PULL
#define P0_10_MODE            PIN_NO_PULL
#define P0_11_MODE            PIN_PULL_UP//PIN_NO_PULL
#define P0_12_MODE            PIN_NO_PULL
#define P0_13_MODE            PIN_NO_PULL
#define P0_14_MODE            PIN_NO_PULL
#define P0_15_MODE            PIN_NO_PULL

#define P0_16_MODE            PIN_NO_PULL
#define P0_17_MODE            PIN_NO_PULL         //上拉
#define P0_18_MODE            PIN_NO_PULL
#define P0_19_MODE            PIN_NO_PULL
#define P0_20_MODE            PIN_NO_PULL
#define P0_21_MODE            PIN_NO_PULL
#define P0_22_MODE            PIN_NO_PULL
#define P0_23_MODE            PIN_NO_PULL
#define P0_24_MODE            PIN_NO_PULL
#define P0_25_MODE            PIN_NO_PULL
#define P0_26_MODE            PIN_NO_PULL
#define P0_27_MODE            PIN_NO_PULL
#define P0_28_MODE            PIN_NO_PULL
#define P0_29_MODE            PIN_NO_PULL
#define P0_30_MODE            PIN_NO_PULL
#define P0_31_MODE            PIN_NO_PULL

/*
 *  P1
 */
#define P1_0_MODE             PIN_NO_PULL
#define P1_1_MODE             PIN_NO_PULL
#define P1_2_MODE             PIN_NO_PULL
#define P1_3_MODE             PIN_NO_PULL
#define P1_4_MODE             PIN_NO_PULL
#define P1_5_MODE             PIN_NO_PULL
#define P1_6_MODE             PIN_NO_PULL
#define P1_7_MODE             PIN_NO_PULL
#define P1_8_MODE             PIN_NO_PULL
#define P1_9_MODE             PIN_NO_PULL
#define P1_10_MODE            PIN_NO_PULL
#define P1_11_MODE            PIN_NO_PULL
#define P1_12_MODE            PIN_NO_PULL
#define P1_13_MODE            PIN_NO_PULL
#define P1_14_MODE            PIN_NO_PULL
#define P1_15_MODE            PIN_NO_PULL
#define P1_16_MODE            PIN_NO_PULL     //下拉
#define P1_17_MODE            PIN_NO_PULL
#define P1_18_MODE            PIN_NO_PULL
#define P1_19_MODE            PIN_NO_PULL
#define P1_20_MODE            PIN_NO_PULL
#define P1_21_MODE            PIN_NO_PULL
#define P1_22_MODE            PIN_NO_PULL
#define P1_23_MODE            PIN_NO_PULL
#define P1_24_MODE            PIN_NO_PULL
#define P1_25_MODE            PIN_NO_PULL
#define P1_26_MODE            PIN_NO_PULL
#define P1_27_MODE            PIN_NO_PULL
#define P1_28_MODE            PIN_NO_PULL
#define P1_29_MODE            PIN_NO_PULL
#define P1_30_MODE            PIN_NO_PULL
#define P1_31_MODE            PIN_NO_PULL

/*
 *  P2
 */
#define P2_0_MODE             PIN_NO_PULL
#define P2_1_MODE             PIN_PULL_UP//PIN_NO_PULL
#define P2_2_MODE             PIN_NO_PULL//PIN_PULL_DOWN         //下拉
#define P2_3_MODE             PIN_NO_PULL
#define P2_4_MODE             PIN_NO_PULL
#define P2_5_MODE             PIN_NO_PULL
#define P2_6_MODE             PIN_NO_PULL
#define P2_7_MODE             PIN_NO_PULL
#define P2_8_MODE             PIN_NO_PULL
#define P2_9_MODE             PIN_NO_PULL
#define P2_10_MODE            PIN_NO_PULL
#define P2_11_MODE            PIN_NO_PULL
#define P2_12_MODE            PIN_NO_PULL
#define P2_13_MODE            PIN_NO_PULL
#define P2_14_MODE            PIN_NO_PULL
#define P2_15_MODE            PIN_NO_PULL

/*
 *  P3
 */
#define P3_25_MODE            PIN_NO_PULL//PIN_PULL_UP       //上拉
#define P3_26_MODE            PIN_NO_PULL//PIN_PULL_DOWN     //下拉

/*
 *  P4
 */
#define P4_28_MODE            PIN_NO_PULL
#define P4_29_MODE            PIN_PULL_UP//PIN_NO_PULL

/******************************************************************************
** 按原理图定义PIN
******************************************************************************/
/*
 *  P0信号宏定义
 */
#define     P0_SDA_E2           (1ul << 0)
#define     P0_SCL_E2           (1ul << 1)
#define     P0_TXD_4851         (1ul << 2)
#define     P0_RXD_4851         (1ul << 3)
#define     P0_GPRS_4851_CTL    (1ul << 4)// W OR R
#define     P0_F1_WP            (1ul << 5)
#define     P0_F1_CS1           (1ul << 6)
#define     P0_FLASH_SCK1       (1ul << 7)
#define     P0_FLASH_SDO1       (1ul << 8)  
#define     P0_FLASH_SDI1       (1ul << 9)

#define     P0_RXD_PLCM         (1ul << 10)//cpu的发送 载波的收
#define     P0_TXD_PLCM         (1ul << 11)
//                              (1ul << 12)
//                              (1ul << 13)
//                              (1ul << 14)
#define     P0_RUN_LED          (1ul << 15)
//                              (1ul << 16)
//                              (1ul << 17)
//                              (1ul << 18)
//                              (1ul << 19)
#define     P0_EE_WP            (1ul << 20)
//                              (1ul << 21)
//                              (1ul << 22)
#define     P0_BAT_AD           (1ul << 23)
//                              (1ul << 24)
#define     P0_PD_CHK           (1ul << 25)
#define     P0_WDI              (1ul << 26)
//#define     P0_SDA_LCD          (1ul << 27)//azh
//#define     P0_SCL_LCD          (1ul << 28)
//                              (1ul << 29)
//                              (1ul << 30)
//                              (1ul << 31)

/*
 *  P1信号宏定义
 */
#define     P1_RLED_GPRS        (1ul << 0)
#define     P1_GLED_GPRS        (1ul << 1)
//                              (1ul << 2)
//                              (1ul << 3)
#define     P1_GPRS_RTS         (1ul << 4)
//                              (1ul << 5)
//                              (1ul << 6)
//                              (1ul << 7)
#define     P1_GPRS_CTS         (1ul << 8)
#define     P1_GPRS_DTR         (1ul << 9)
#define     P1_GPRS_DCD         (1ul << 10)
//                              (1ul << 11)
//                              (1ul << 12)
//                              (1ul << 13)
#define     P1_GPRS_RI          (1ul << 14)
#define     P1_CPU_IGT          (1ul << 15)
#define     P1_CPU_EVERGOFF     (1ul << 16)
#define     P1_EVENTOUT         (1ul << 17)//以前用于模块识别 现在用于与计量析通信握手
#define     P1_RLED_LOCAL       (1ul << 18)
#define     P1_GLED_LOCAL       (1ul << 19)
//                              (1ul << 20)
//                              (1ul << 21)
//                              (1ul << 22)
//                              (1ul << 23)
//                              (1ul << 24)
//                              (1ul << 25)
#define     P1_PWM_38K          (1ul << 26)
//                              (1ul << 27)
//                              (1ul << 28)
#define     P1_SET_EN           (1ul << 29)
//                              (1ul << 30)
#define     P1_RST_WLS          (1ul << 31)//azh 171011

/*
 *  P2信号宏定义
 */
#define     P2_TXD_LOCAL        (1ul << 0)
#define     P2_RXD_LOCAL        (1ul << 1)
#define     P2_CTS_WLS          (1ul << 2)//azh 171011
#define     P2_FAST_OFF         (1ul << 3)
#define     P2_BAT_ON           (1ul << 4)
#define     P2_VGPRS_ON         (1ul << 5)
#define     P2_CHAG_ON          (1ul << 6)
#define     P2_RTS_WLS          (1ul << 7)//P2_RTS_WLS
#define     P2_STATE_PLCM       (1ul << 8)
#define     P2_ZGB_PWR          (1ul << 9)//azh 171011 zigbee电源开关节约功耗
#define     P2_ISP_EN           (1ul << 10)
//                              (1ul << 11)
//                              (1ul << 12)
//                              (1ul << 13)

/*
 *  P3信号宏定义
 */
//                              (1ul << 25)
//                              (1ul << 26)

/*
 *  P4信号宏定义
 */
#define     P4_MC52I_RXD        (1ul << 28)//CPU_TXD
#define     P4_MC52I_TXD        (1ul << 29)//CPU_RXD


/******************************************************************************
** 各个PORT口方向初始化值定义，把设置为输入的IO写到下面的宏，空闲的IO全部置为输出
******************************************************************************/
#define     P0_INIT_DIR         (~(P0_SDA_E2 |       \
                                   P0_FLASH_SDO1 |   \
                                   P0_RXD_4851 |     \
                                   P0_TXD_PLCM |     \
                                   P0_BAT_AD |       \
                                   P0_PD_CHK         \
                                   )                 \
                                )

#define     P1_INIT_DIR         (~(P1_SET_EN   |     \
                                   P1_GPRS_DCD |     \
                                   P1_GPRS_CTS |     \
                                   P1_GPRS_RI  |     \
                                   P1_EVENTOUT       \
                                  )                  \
                                )

#define     P2_INIT_DIR        (~(P2_RXD_LOCAL |     \
                                   P2_ISP_EN |       \
                                   P2_CTS_WLS |      \
                                   P2_STATE_PLCM     \
                                   )                 \
                                )

#define     P3_INIT_DIR         (0xffffffff)

#define     P4_INIT_DIR         (~(P4_MC52I_TXD      \
                                   )                 \
                                )

/******************************************************************************
** 各个PORT口输出初始化值定义，把设置为输出‘1’的IO写到下面的宏，空闲的IO全部置为输出‘0’
******************************************************************************/
#define     P0_INIT_DATA        (P0_SCL_E2 |         \
                                 P0_TXD_4851 |       \
                                 P0_F1_CS1 |         \
                                 P0_FLASH_SCK1 |     \
                                 P0_FLASH_SDI1 |     \
                                 P0_RXD_PLCM |       \
                                 P0_RUN_LED|         \
                                 P0_GPRS_4851_CTL |  \
                                 P0_EE_WP            \
                                 )

#define     P1_INIT_DATA        (P1_RLED_LOCAL |     \
                                 P1_GLED_LOCAL |     \
                                 P1_CPU_IGT |        \
                                 P1_CPU_EVERGOFF |   \
                                 P1_SET_EN           \
                                 )

#define     P2_INIT_DATA        (P2_TXD_LOCAL |      \
                                 P2_RTS_WLS |        \
                                 P2_FAST_OFF         \
                                 )

#define     P3_INIT_DATA        (0         \
                                 )

#define     P4_INIT_DATA        (P4_MC52I_RXD)

/******************************************************************************
** 各个PORT口操作宏定义
******************************************************************************/
/*
 *  P0操作宏
 */
//#define     PVDD_ON()               FIO0DIR |= P0_PECTL;            \
//                                    FIO0CLR = P0_PECTL                  //PECTL输出‘0’,外设电源开启
//#define     PVDD_OFF()              FIO0DIR |= P0_PECTL;            \
//                                    FIO0SET = P0_PECTL                  //PECTL输出‘1’,外设电源关闭
#define     FLS1_CS_EN()            FIO0DIR |= P0_F1_CS1;           \
                                    FIO0CLR = P0_F1_CS1                 //F1_CS1输出‘0’,FLASH1片选使能
#define     FLS1_CS_DIS()           FIO0DIR |= P0_F1_CS1;           \
                                    FIO0SET = P0_F1_CS1                 //F1_CS1输出‘1’,FLASH1片选禁能
//#define     BEEP_ON_DIS()           FIO0DIR |= P0_BEEP_ON;          \
//                                    FIO0CLR = P0_BEEP_ON                //BEEP_ON输出‘0’关闭蜂鸣器
//#define     BEEP_ON_EN()            FIO0DIR |= P0_BEEP_ON;          \
//                                    FIO0SET = P0_BEEP_ON                //BEEP_ON输出‘1’开启蜂鸣器
//#define     ALARM_LED_ON()          FIO0DIR |= P0_ALARM_LED;        \
//                                    FIO0CLR = P0_ALARM_LED              //ALARM_LED输出‘0’点亮报警灯
//#define     ALARM_LED_OFF()         FIO0DIR |= P0_ALARM_LED;        \
//                                    FIO0SET = P0_ALARM_LED              //ALARM_LED输出‘1’熄灭报警灯
//#define     ESAM_RST_EN()           FIO0DIR |= P0_ESAM_RST;         \
//                                    FIO0CLR = P0_ESAM_RST               //ESAM_RST输出‘0’,使能复位?
//#define     ESAM_RST_DIS()          FIO0DIR |= P0_ESAM_RST;         \
//                                    FIO0SET = P0_ESAM_RST               //ESAM_RST输出‘1’,禁能复位?
#define     EE_WP_EN()              FIO0DIR |= P0_EE_WP;            \
                                    FIO0SET = P0_EE_WP                  //WP_E2输出‘1’,EEPROM写保护使能
#define     EE_WP_DIS()             FIO0DIR |= P0_EE_WP;            \
                                    FIO0CLR = P0_EE_WP                  //WP_E2输出‘0’,EEPROM写保护禁能
#define     SET_PORT0_IN(pin)       FIO0DIR &= ~pin
#define     PORT0_IS_HIGH(pin)     (FIO0PIN & pin)
#define     SET_PORT0_OUT(pin)      FIO0DIR |= pin
#define     SET_PORT0_HIGH(pin)     FIO0SET = pin
#define     SET_PORT0_LOW(pin)      FIO0CLR = pin
#define     SET_PORT0_REV(pin)      if(PORT0_IS_HIGH(pin))          \
                                    {                               \
                                        SET_PORT0_LOW(pin);         \
                                    }                               \
                                    else                            \
                                    {                               \
                                        SET_PORT0_HIGH(pin);        \
                                    }

/*
 *  P1操作宏
 */
#define     IGT_ON()                FIO1DIR |= P1_CPU_IGT;          \
                                    FIO1CLR = P1_CPU_IGT                //CPU_IGT输出‘0’Turn on MC55/56
#define     IGT_OFF()               FIO1DIR |= P1_CPU_IGT;          \
                                    FIO1SET = P1_CPU_IGT                //CPU_IGT输出‘1’Turn off MC55/56
#define     EMERGOFF_EN()           FIO1DIR |= P1_CPU_EVERGOFF;     \
                                    FIO1CLR = P1_CPU_EVERGOFF           //CPU_EVERGOFF输出‘0’使能GPRS的EMERGOFF
#define     EMERGOFF_DIS()          FIO1DIR |= P1_CPU_EVERGOFF;     \
                                    FIO1SET = P1_CPU_EVERGOFF           //CPU_EVERGOFF输出‘1’禁能GPRS的EMERGOFF
//#define     BGLED_ON()              FIO1DIR |= P1_BGLED;            \
//                                    FIO1SET = P1_BGLED                  //BGLED输出‘1’,点亮LCD背光灯
//#define     BGLED_OFF()             FIO1DIR |= P1_BGLED;            \
//                                    FIO1CLR = P1_BGLED                  //BGLED输出‘0’,熄灭LCD背光灯

#define     WLS_RLED_OFF()           FIO1DIR |= P1_RLED_LOCAL;         \
                                    FIO1SET = P1_RLED_LOCAL               //RLED_LOCAL输出‘1’,点亮ZIGBEE红灯
#define     WLS_RLED_ON()          FIO1DIR |= P1_RLED_LOCAL;         \
                                    FIO1CLR = P1_RLED_LOCAL               //RLED_LOCAL输出‘0’,熄灭ZIGBEE红灯
#define     WLS_GLED_OFF()           FIO1DIR |= P1_GLED_LOCAL;         \
                                    FIO1SET = P1_GLED_LOCAL               //GLED_LOCAL输出‘1’,点亮ZIGBEE绿灯
#define     WLS_GLED_ON()          FIO1DIR |= P1_GLED_LOCAL;         \
                                    FIO1CLR = P1_GLED_LOCAL               //GLED_LOCAL输出‘0’,熄灭ZIGBEE绿灯
#define     GPRS_RLED_OFF()          FIO1DIR |= P1_RLED_GPRS;        \
                                    FIO1SET = P1_RLED_GPRS              //RLED_GPRS输出‘1’,点亮GPRS红灯
#define     GPRS_RLED_ON()         FIO1DIR |= P1_RLED_GPRS;        \
                                    FIO1CLR = P1_RLED_GPRS              //RLED_GPRS输出‘0’,熄灭GPRS红灯
#define     GPRS_GLED_OFF()          FIO1DIR |= P1_GLED_GPRS;        \
                                    FIO1SET = P1_GLED_GPRS              //GLED_GPRS输出‘1’,点亮GPRS绿灯
#define     GPRS_GLED_ON()         FIO1DIR |= P1_GLED_GPRS;        \
                                    FIO1CLR = P1_GLED_GPRS              //GLED_GPRS输出‘0’,熄灭GPRS绿灯

//#define     FUNOUT_ON()             FIO1DIR |= P1_FUNOUT;           \
//                                    FIO1CLR = P1_FUNOUT                 //FUNOUT输出‘0’,功能输出开启
//#define     FUNOUT_OFF()            FIO1DIR |= P1_FUNOUT;           \
//                                    FIO1SET = P1_FUNOUT                 //FUNOUT输出‘1’,功能输出关闭
//#define     ALARM_ON()              FIO1DIR |= P1_ALARM;            \
//                                    FIO1CLR = P1_ALARM                  //ALARM输出‘0’,报警输出开启
//#define     ALARM_OFF()             FIO1DIR |= P1_ALARM;            \
//                                    FIO1SET = P1_ALARM                  //ALARM输出‘1’,报警输出关闭
//#define     IR_VDD_ON()             FIO1DIR |= P1_IR_CTL;           \
//                                    FIO1CLR = P1_IR_CTL                 //IR_CTL输出‘0’,IRDA电源开启
//#define     IR_VDD_OFF()            FIO1DIR |= P1_IR_CTL;           \
//                                    FIO1SET = P1_IR_CTL                 //IR_CTL输出‘1’,IRDA电源关闭

//#define     DBG_LED_ON()            FIO1DIR |= P1_DBG_LED;          \
//                                    FIO1CLR = P1_DBG_LED                //DEBUG_LED输出‘0’,DEBUG_LED点亮
//#define     DBG_LED_OFF()           FIO1DIR |= P1_DBG_LED;          \
//                                    FIO1SET = P1_DBG_LED                //DEBUG_LED输出‘1’,DEBUG_LED熄灭
//#define     FLS2_CS_EN()            FIO1DIR |= P1_F1_CS2;           \
//                                    FIO1CLR = P1_F1_CS2                 //F1_CS2输出‘0’,FLASH2片选使能
//#define     FLS2_CS_DIS()           FIO1DIR |= P1_F1_CS2;           \
//                                    FIO1SET = P1_F1_CS2                 //F1_CS2输出‘1’,FLASH2片选禁能
#define     DF_WP_EN()              FIO0DIR |= (P0_F1_WP);          \
                                    FIO0CLR = (P0_F1_WP)               //F1_WP输出‘0’,FLASH1写保护使能
#define     DF_WP_DIS()             FIO0DIR |= (P0_F1_WP);          \
                                    FIO0SET = (P0_F1_WP)               //F1_WP输出‘1’,FLASH1写保护禁能
//#define     FM_WP_EN()              FIO1DIR |= P1_FM_WP;            \
//                                    FIO1SET = P1_FM_WP                  //WP_FM输出‘1’,FRAM写保护使能
//#define     FM_WP_DIS()             FIO1DIR |= P1_FM_WP;            \
//                                    FIO1CLR = P1_FM_WP                  //WP_FM输出‘0’,FRAM写保护禁能
//#define     WGP_ON()                FIO1DIR |= P1_CPU_WGP;          \
//                                    FIO1CLR = P1_CPU_WGP                //CPU_WGP输出‘0’,点亮无功灯并输出无功脉冲
//#define     WGP_OFF()               FIO1DIR |= P1_CPU_WGP;          \
//                                    FIO1SET = P1_CPU_WGP                //CPU_WGP输出‘1’,熄灭无功灯并关闭无功脉冲
#define     RUN_LED_ON()            FIO0DIR |= P0_RUN_LED;          \
                                    FIO0CLR = P0_RUN_LED                //CPU_YGP输出‘0’,点亮有功灯并输出有功脉冲
#define     RUN_LED_OFF()           FIO0DIR |= P0_RUN_LED;          \
                                    FIO0SET = P0_RUN_LED                //CPU_YGP输出‘1’,熄灭有功灯并关闭有功脉冲

#define     SET_PORT1_IN(pin)       FIO1DIR &= ~pin
#define     PORT1_IS_HIGH(pin)     (FIO1PIN & pin)
#define     SET_PORT1_OUT(pin)      FIO1DIR |= pin
#define     SET_PORT1_HIGH(pin)     FIO1SET = pin
#define     SET_PORT1_LOW(pin)      FIO1CLR = pin
#define     SET_PORT1_REV(pin)      if(PORT1_IS_HIGH(pin))          \
                                    {                               \
                                        SET_PORT1_LOW(pin);         \
                                    }                               \
                                    else                            \
                                    {                               \
                                        SET_PORT1_HIGH(pin);        \
                                    }

/*
 *  P2操作宏
 */
#define     FAST_CHARGE_ON()        FIO2DIR |= P2_FAST_OFF;         \
                                    FIO2CLR = P2_FAST_OFF               //FAST_OFF输出‘0’开起充电
#define     FAST_CHARGE_OFF()       FIO2DIR |= P2_FAST_OFF;         \
                                    FIO2SET = P2_FAST_OFF               //FAST_OFF输出‘1’关闭充电
#define     CHARGE_EN()             FIO2DIR |= P2_CHAG_ON;          \
                                    FIO2SET = P2_CHAG_ON                //CHAG_ON输出‘1’,GPRS充电电源开启
#define     CHARGE_DIS()            FIO2DIR |= P2_CHAG_ON;          \
                                    FIO2CLR = P2_CHAG_ON                //CHAG_ON输出‘1’,GPRS充电电源关闭
#define     BAT_ON()                FIO2DIR |= P2_BAT_ON;          \
                                    FIO2SET = P2_BAT_ON                //BAT_ON输出‘0’,GPRS电池开启
#define     BAT_OFF()               FIO2DIR |= P2_BAT_ON;          \
                                    FIO2CLR = P2_BAT_ON                //BAT_ON输出‘1’,GPRS电池关闭
#define     GPRS_POWER_DIS()        FIO2DIR |= P2_VGPRS_ON;         \
                                    FIO2CLR = P2_VGPRS_ON               //VGPRS_ON输出‘0’禁能GPRS供电
#define     GPRS_POWER_EN()         FIO2DIR |= P2_VGPRS_ON;         \
                                    FIO2SET = P2_VGPRS_ON               //VGPRS_ON输出‘1’开启GPRS供电
//azh 171011 电子式高压表GPRS模块用到zigbee模块，所以打开模块复位脚并且是P1口，还有zigbee模块的电源开启
#define     WLS_RST_LOW()           FIO1DIR |= P1_RST_WLS;          \
                                    FIO1CLR = P1_RST_WLS                //RST_WLS输出‘0’,ZigBee复位使能
#define     WLS_RST_HIGH()          FIO1DIR |= P1_RST_WLS;          \
                                    FIO1SET = P1_RST_WLS                //RST_WLS输出‘1’,ZigBee复位禁能
#define     ZIGBEE_POWER_OFF()       FIO2DIR |= P2_ZGB_PWR;         \
                                    FIO2CLR = P2_ZGB_PWR               //P2_ZGB_PWR输出‘0’开启GPRS供电//OFF azh 171014 加上三极管逻辑要反一反
#define     ZIGBEE_POWER_ON()      FIO2DIR |= P2_ZGB_PWR;         \
                                    FIO2SET = P2_ZGB_PWR               //P2_ZGB_PWR输出‘1’关闭GPRS供电

#define     PLC_RST_LOW()           FIO2DIR |= P2_RST_PLC;          \
                                    FIO2CLR = P2_RST_PLC                //RST_PLC输出‘0’,ZigBee复位使能
#define     PLC_RST_HIGH()          FIO2DIR |= P2_RST_PLC;          \
                                    FIO2SET = P2_RST_PLC                //RST_PLC输出‘1’,ZigBee复位禁能
//#define     VLCD_ON()               FIO2DIR |= P2_DIS_CTL;          \
//                                    FIO2SET = P2_DIS_CTL                //DIS_CTL输出‘1’,LCD电源开启
//#define     VLCD_OFF()              FIO2DIR |= P2_DIS_CTL;          \
//                                    FIO2CLR = P2_DIS_CTL                //DIS_CTL输出‘0’,LCD电源关闭

#define     SET_PORT2_IN(pin)       FIO2DIR &= ~pin
#define     PORT2_IS_HIGH(pin)     (FIO2PIN & pin)
#define     SET_PORT2_OUT(pin)      FIO2DIR |= pin
#define     SET_PORT2_HIGH(pin)     FIO2SET = pin
#define     SET_PORT2_LOW(pin)      FIO2CLR = pin
#define     SET_PORT2_REV(pin)      if(PORT2_IS_HIGH(pin))          \
                                    {                               \
                                        SET_PORT2_LOW(pin);         \
                                    }                               \
                                    else                            \
                                    {                               \
                                        SET_PORT2_HIGH(pin);        \
                                    }

/*
 *  P3操作宏
 */
#define     SET_PORT3_IN(pin)       FIO3DIR &= ~pin
#define     PORT3_IS_HIGH(pin)     (FIO3PIN & pin)
#define     SET_PORT3_OUT(pin)      FIO3DIR |= pin
#define     SET_PORT3_HIGH(pin)     FIO3SET = pin
#define     SET_PORT3_LOW(pin)      FIO3CLR = pin
#define     SET_PORT3_REV(pin)      if(PORT3_IS_HIGH(pin))          \
                                    {                               \
                                        SET_PORT3_LOW(pin);         \
                                    }                               \
                                    else                            \
                                    {                               \
                                        SET_PORT3_HIGH(pin);        \
                                    }


/*
 *  P4操作宏
 */
#define     SET_PORT4_IN(pin)       FIO4DIR &= ~pin
#define     PORT4_IS_HIGH(pin)     (FIO4PIN & pin)
#define     SET_PORT4_OUT(pin)      FIO4DIR |= pin
#define     SET_PORT4_HIGH(pin)     FIO4SET = pin
#define     SET_PORT4_LOW(pin)      FIO4CLR = pin
#define     SET_PORT4_REV(pin)      if(PORT4_IS_HIGH(pin))          \
                                    {                               \
                                        SET_PORT4_LOW(pin);         \
                                    }                               \
                                    else                            \
                                    {                               \
                                        SET_PORT4_HIGH(pin);        \
                                    }
#else//H_METER_PRJ,WLS_CUR_PRJ
/******************************************************************************
** P0口引脚功能设置
******************************************************************************/
/*
 *  P0.0
 */
#define P0_0_GPIO             0x00                                      /* GPIO                         */
#define P0_0_RD1              0x01                                      /* CAN-1控制器接收引脚          */
#define P0_0_TXD3             0x02                                      /* UART3发送引脚                */
#define P0_0_SDA1             0x03                                      /* I2C-1数据线（开漏）          */

#define P0_0_FNUC             P0_0_SDA1

/*
 *  P0.1
 */
#define P0_1_GPIO             0x00                                      /* GPIO                         */
#define P0_1_TD1              0x01                                      /* CAN-1控制器发送引脚          */
#define P0_1_RXD3             0x02                                      /* UART3接收引脚                */
#define P0_1_SCL1             0x03                                      /* I2C-1时钟线（开漏）          */

#define P0_1_FNUC             P0_1_SCL1

/*
 *  P0.2
 */
#define P0_2_GPIO             0x00                                      /* GPIO                         */
#define P0_2_TXD0             0x01                                      /* UART0发送引脚                */
#define P0_2_AD07             0x02                                      /* AD0.7引脚                    */
//...   Reserved              0x03

#define P0_2_FNUC             P0_2_TXD0

/*
 *  P0.3
 */
#define P0_3_GPIO             0x00                                      /* GPIO                         */
#define P0_3_RXD0             0x01                                      /* UART0接收引脚                */
#define P0_3_AD06             0x02                                      /* AD0.6引脚                    */
//...   Reserved              0x03

#define P0_3_FNUC             P0_3_RXD0

/*
 *  P0.4
 */
#define P0_4_GPIO             0x00                                      /* GPIO                         */
#define P0_4_I2SRX_CLK        0x01                                      /* I2S总线接收时钟SCK           */
#define P0_4_RD2              0x02                                      /* CAN-2接收线                  */
#define P0_4_CAP20            0x03                                      /* 定时器2捕获输入，通道0       */

#define P0_4_FNUC             P0_4_GPIO

/*
 *  P0.5
 */
#define P0_5_GPIO             0x00                                      /* GPIO                         */
#define P0_5_I2SRX_WS         0x01                                      /* I2S总线接收字选择WS          */
#define P0_5_TD2              0x02                                      /* CAN-2发送线                  */
#define P0_5_CAP21            0x03                                      /* 定时器2捕获输入，通道1       */

#define P0_5_FNUC             P0_5_GPIO

/*
 *  P0.6
 */
#define P0_6_GPIO             0x00                                      /* GPIO                         */
#define P0_6_I2SRX_SDA        0x01                                      /* I2S总线接收数据线SD          */
#define P0_6_SSEL1            0x02                                      /* SSP-1总线从机选择            */
#define P0_6_MAT20            0x03                                      /* 定时器2匹配输出，通道0       */

#define P0_6_FNUC             P0_6_GPIO

/*
 *  P0.7
 */
#define P0_7_GPIO             0x00                                      /* GPIO                         */
#define P0_7_I2STX_CLK        0x01                                      /* I2S总线发送时钟线SCK         */
#define P0_7_SSCK1            0x02                                      /* SSP-1时钟线                  */
#define P0_7_MAT21            0x03                                      /* 定时器2匹配输出，通道1       */

//#ifdef PHASE_C_MODULE
#define P0_7_FNUC             P0_7_SSCK1
//#else// PHASE_C_MODULE
//#define P0_7_FNUC             P0_7_GPIO
//#endif// PHASE_C_MODULE

/*
 *  P0.8
 */
#define P0_8_GPIO             0x00                                      /* GPIO                         */
#define P0_8_I2STX_WS         0x01                                      /* I2S总线发送字选择WS          */
#define P0_8_SMISO1           0x02                                      /* SSP-1主输入从输出数据线      */
#define P0_8_MAT22            0x03                                      /* 定时器2匹配输出，通道2       */

#define P0_8_FNUC             P0_8_SMISO1

/*
 *  P0.9
 */
#define P0_9_GPIO             0x00                                      /* GPIO                         */
#define P0_9_I2STX_SDA        0x01                                      /* I2S总线发送数据线SD          */
#define P0_9_SMOSI1           0x02                                      /* SSP-1主输出从输入数据线      */
#define P0_9_MAT23            0x03                                      /* 定时器2匹配输出，通道3       */

#define P0_9_FNUC             P0_9_SMOSI1

/*
 *  P0.10
 */
#define P0_10_GPIO            0x00                                      /* GPIO                         */
#define P0_10_TXD2            0x01                                      /* UART2发送引脚                */
#define P0_10_SDA2            0x02                                      /* I2C-2数据线(非开漏引脚)      */
#define P0_10_MAT30           0x03                                      /* 定时器3匹配输出，通道0       */

#define P0_10_FNUC            P0_10_TXD2

/*
 *  P0.11
 */
#define P0_11_GPIO            0x00                                      /* GPIO                         */
#define P0_11_RXD2            0x01                                      /* UART2接收引脚                */
#define P0_11_SCL2            0x02                                      /* I2C-2时钟线(非开漏引脚)      */
#define P0_11_MAT31           0x03                                      /* 定时器3匹配输出，通道1       */

#define P0_11_FNUC            P0_11_RXD2

/*
 *  P0.15
 */
#define P0_15_GPIO            0x00                                      /* GPIO                         */
#define P0_15_TXD1            0x01                                      /* UART1发送引脚                */
#define P0_15_SCK0            0x02                                      /* SSP0时钟线                   */
#define P0_15_SCK             0x03                                      /* SPI时钟线                    */

#define P0_15_FNUC            P0_15_GPIO

/*
 *  P0.16
 */
#define P0_16_GPIO            0x00                                      /* GPIO                         */
#define P0_16_RXD1            0x01                                      /* UART1接收引脚                */
#define P0_16_SSEL0           0x02                                      /* SSP0从机选择                 */
#define P0_16_SSEL            0x03                                      /* SPI从机选择                  */

#define P0_16_FNUC            P0_16_GPIO

/*
 *  P0.17
 */
#define P0_17_GPIO            0x00                                      /* GPIO                         */
#define P0_17_CTS1            0x01                                      /* UART1清零发送信号CTS         */
#define P0_17_MISO0           0x02                                      /* SSP0主输入从输出数据线       */
#define P0_17_MISO            0x03                                      /* SPI主入从出数据线            */

#define P0_17_FNUC            P0_17_GPIO

/*
 *  P0.18
 */
#define P0_18_GPIO            0x00                                      /* GPIO                         */
#define P0_18_DCD1            0x01                                      /* UART1数据载波检测输入DCD     */
#define P0_18_MOSI0           0x02                                      /* SSP-0主输出从输入数据线      */
#define P0_18_MOSI            0x03                                      /* SPI主出从入数据线            */

#define P0_18_FNUC            P0_18_GPIO

/*
 *  P0.19
 */
#define P0_19_GPIO            0x00                                      /* GPIO                         */
#define P0_19_DSR1            0x01                                      /* UART1数据设置就绪DSR         */
//...   Reserved              0x02                                      /* 保留                         */
#define P0_19_SDA1            0x03                                      /* I2C-1数据线（非开漏）        */

#define P0_19_FNUC            P0_19_GPIO

/*
 *  P0.20
 */
#define P0_20_GPIO            0x00                                      /* GPIO                         */
#define P0_20_DTR1            0x01                                      /* UART1终端就绪DTR             */
//...   Reserved              0x02                                      /* 保留                         */
#define P0_20_SCL1            0x03                                      /* I2C-1时钟线（非开漏）        */

#define P0_20_FNUC            P0_20_GPIO

/*
 *  P0.21
 */
#define P0_21_GPIO            0x00                                      /* GPIO                         */
#define P0_21_RI1             0x01                                      /* UART1振铃信号                */
//...   Reserved              0x02                                      /* 保留                         */
#define P0_21_RD1             0x03                                      /* CAN-1接收引脚                */

#define P0_21_FNUC            P0_21_GPIO

/*
 *  P0.22
 */
#define P0_22_GPIO            0x00                                      /* GPIO                         */
#define P0_22_RTS1            0x01                                      /* UART1请求发送引脚            */
//...   Reserved              0x02                                      /* 保留                         */
#define P0_22_TD1             0x03                                      /* CAN-1发送线                  */

#define P0_22_FNUC            P0_22_GPIO

/*
 *  P0.23
 */
#define P0_23_GPIO            0x00                                      /* GPIO                         */
#define P0_23_AD00            0x01                                      /* ADC-0，通道0                 */
#define P0_23_I2SRX_CLK       0x02                                      /* I2S接收时钟SCK               */
#define P0_23_CAP30           0x03                                      /* 定时器3捕获输入，通道0       */

#define P0_23_FNUC            P0_23_AD00

/*
 *  P0.24
 */
#define P0_24_GPIO            0x00                                      /* GPIO                         */
#define P0_24_AD01            0x01                                      /* ADC-0，通道1                 */
#define P0_24_I2SRX_WS        0x02                                      /* I2S接收字选择WS              */
#define P0_24_CAP31           0x03                                      /* 定时器3捕获输入，通道1       */

#define P0_24_FNUC            P0_24_AD01

/*
 *  P0.25
 */
#define P0_25_GPIO            0x00                                      /* GPIO                         */
#define P0_25_AD02            0x01                                      /* ADC-0，通道2                 */
#define P0_25_I2SRX_SDA       0x02                                      /* I2S接收数据SD                */
#define P0_25_TXD3            0x03                                      /* UART3发送引脚                */

#define P0_25_FNUC            P0_25_AD02

/*
 *  P0.26
 */
#define P0_26_GPIO            0x00                                      /* GPIO                         */
#define P0_26_AD03            0x01                                      /* ADC-0，通道3                 */
#define P0_26_AOUT            0x02                                      /* DAC输出                      */
#define P0_26_RXD3            0x03                                      /* UART-3接收引脚               */

#define P0_26_FNUC            P0_26_AD03

/*
 *  P0.27
 */
#define P0_27_GPIO            0x00                                      /* GPIO                         */
#define P0_27_SDA0            0x01                                      /* I2C0数据输入/输出，开漏      */
#define P0_27_USB_SDA         0x02                                      /* USB接口I2C串行数据OTG收发器  */
//..    Reserved              0x03                                      /* 保留                         */

#define P0_27_FNUC            P0_27_SDA0

/*
 *  P0.28
 */
#define P0_28_GPIO            0x00                                      /* GPIO                         */
#define P0_28_SCL0            0x01                                      /* I2C0时钟输入，开漏           */
#define P0_28_USB_SCL         0x02                                      /* USB接口I2C时钟线，OTG收发器  */
//..    Reserved              0x03                                   /* 保留                         */

#define P0_28_FNUC            P0_28_SCL0

/*
 *  P0.29
 */
#define P0_29_GPIO            0x00                                      /* GPIO                         */
#define P0_29_USB_DD          0x01                                      /* USB双向数据D+线              */
//..    Reserved              0x02                                      /* 保留                         */
//..    Reserved              0x03                                      /* 保留                         */

#define P0_29_FNUC            P0_29_GPIO

/*
 *  P0.30
 */
#define P0_30_GPIO            0x00                                      /* GPIO                         */
#define P0_30_USB_DS          0x01                                      /* USB双向数据D-线              */
//..    Reserved              0x02                                      /* 保留                         */
//..    Reserved              0x03                                      /* 保留                         */

#define P0_30_FNUC            P0_30_GPIO

/******************************************************************************
** P1口引脚功能设置
******************************************************************************/
/*
 *  P1.0
 */
#define P1_0_GPIO             0x00                                      /* GPIO                         */
#define P1_0_ENET_TXD0        0x01                                      /* 以太网发送数据脚0            */
//..    Reserved              0x02                                      /* 保留                         */
//..    Reserved              0x03                                      /* 保留                         */

#define P1_0_FNUC             P1_0_GPIO

/*
 *  P1.1
 */
#define P1_1_GPIO             0x00                                      /* GPIO                         */
#define P1_1_ENET_TX_EN       0x01                                      /* 以太网发送数据脚1            */
//..    Reserved              0x02                                      /* 保留                         */
//..    Reserved              0x03                                      /* 保留                         */

#define P1_1_FNUC             P1_1_GPIO

/*
 *  P1.4
 */
#define P1_4_GPIO             0x00                                      /* GPIO                         */
#define P1_4_ENET_TX_EN       0x01                                      /* 以太网发送使能               */
//..    Reserved              0x02                                      /* 保留                         */
//..    Reserved              0x03                                      /* 保留                         */

#define P1_4_FNUC             P1_4_GPIO

/*
 *  P1.8
 */
#define P1_8_GPIO             0x00                                      /* GPIO                         */
#define P1_8_ENET_CRS         0x01                                      /* 以太网载波侦测脚             */
//..    Reserved              0x02                                      /* 保留                         */
//..    Reserved              0x03                                      /* 保留                         */

#define P1_8_FNUC             P1_8_GPIO

/*
 *  P1.9
 */
#define P1_9_GPIO             0x00                                      /* GPIO                         */
#define P1_9_ENET_RXD0        0x01                                      /* 以太网接收数据脚0            */
//..    Reserved              0x02                                      /* 保留                         */
//..    Reserved              0x03                                      /* 保留                         */

#define P1_9_FNUC             P1_9_GPIO

/*
 *  P1.10
 */
#define P1_10_GPIO            0x00                                      /* GPIO                         */
#define P1_10_ENET_RXD1       0x01                                      /* 以太网接收数据脚1            */
//..    Reserved              0x02                                      /* 保留                         */
//..    Reserved              0x03                                      /* 保留                         */

#define P1_10_FNUC            P1_10_GPIO

/*
 *  P1.11
 */
#define P1_14_GPIO            0x00                                      /*  GPIO                        */
#define P1_14_ENET_RX_ER      0x01                                      /* 以太网接收错误               */
//..    Reserved              0x02                                      /* 保留                         */
//..    Reserved              0x03                                      /* 保留                         */

#define P1_14_FNUC            P1_14_GPIO

/*
 *  P1.15
 */
#define P1_15_GPIO            0x00                                      /* GPIO                         */
#define P1_15_ENET_REF_CLK    0x01                                      /* 以太网参拷时钟输入           */
//..    Reserved              0x02                                      /* 保留                         */
//..    Reserved              0x03                                      /* 保留                         */

#define P1_15_FNUC            P1_15_GPIO

/*
 *  P1.16
 */
#define P1_16_GPIO            0x00                                      /* GPIO                         */
#define P1_16_ENET_MDC        0x01                                      /* 以太网MIIM时钟               */
//..    Reserved              0x02                                      /* 保留                         */
//..    Reserved              0x03                                      /* 保留                         */

#define P1_16_FNUC            P1_16_GPIO

/*
 *  P1.17
 */
#define P1_17_GPIO            0x00                                      /* GPIO                         */
#define P1_17_ENET_MDIO       0x01                                      /* 以太网MI数据输入\输出        */
//..    Reserved              0x02                                      /* 保留                         */
//..    Reserved              0x03                                      /* 保留                         */

#define P1_17_FNUC            P1_17_GPIO

/*
 *  P1.18
 */
#define P1_18_GPIO            0x00                                      /*  GPIO                        */
#define P1_18_USB_UP_LED      0x01                                      /* USB GOODLINK指示脚           */
#define P1_18_PWM11           0x02                                      /* PWM1输出，通道1              */
#define P1_18_CAP10           0x03                                      /* Timer1捕获输入，通道0        */

#define P1_18_FNUC            P1_18_PWM11//P1_18_GPIO//

/*
 *  P1.19
 */
#define P1_19_GPIO            0x00                                      /*  GPIO                        */
#define P1_19_MCO0A           0x01                                      /* 电机控制PWM通道0，输出A      */
#define P1_19_USB_PPWR        0x02                                      /* USB端口电源使能信号          */
#define P1_19_CAP11           0x03                                      /* Timer1捕获输入，通道1        */

#define P1_19_FNUC            P1_19_GPIO

/*
 *  P1.20
 */
#define P1_20_GPIO            0x00                                      /*  GPIO                        */
#define P1_20_MCFB0           0x01                                      /* 电机控制PWM通道0，回环输入   */
#define P1_20_PWM12           0x02                                      /* PWM-2输出，通道2             */
#define P1_20_SCK0            0x03                                      /* SSP-0时钟输入                */

#define P1_20_FNUC            P1_20_SCK0

/*
 *  P1.21
 */
#define P1_21_GPIO            0x00                                      /* GPIO                         */
#define P1_21_MCABORT         0x01                                      /* 电机控制PWM，紧急中止        */
#define P1_21_PWM13           0x02                                      /* PWM-1输出，通道3             */
#define P1_21_SSEL0           0x03                                      /* SSP-0从机选择                */

#define P1_21_FNUC            P1_21_GPIO

/*
 *  P1.22
 */
#define P1_22_GPIO            0x00                                      /* GPIO                         */
#define P1_22_MC0B            0x01                                      /* 电机控制PWM通道0，输出B      */
#define P1_22_USB_PWRD        0x02                                      /* USB端口的电源状态            */
#define P1_22_MAT10           0x03                                      /* 定时器1匹配输出，通道0       */

#define P1_22_FNUC            P1_22_GPIO

/*
 *  P1.23
 */
#define P1_23_GPIO            0x00                                      /* GPIO                         */
#define P1_23_MCFB1           0x01                                      /* 电机控制PWM通道1，反馈输入   */
#define P1_23_PWM14           0x02                                      /* PWM-1输出，通道4             */
#define P1_23_MISO0           0x03                                      /* SSP-0主机输入，从机输出      */

#define P1_23_FNUC            P1_23_MISO0

/*
 *  P1.24
 */
#define P1_24_GPIO            0x00                                      /* GPIO                         */
#define P1_24_MCFB2           0x01                                      /* 电机控制PWM通道2，反馈输入   */
#define P1_24_PWM15           0x02                                      /* PWM-1输出，通道5             */
#define P1_24_MOSI0           0x03                                      /* SSP-0主机输出，从机输入      */

#define P1_24_FNUC            P1_24_MOSI0

/*
 *  P1.25
 */
#define P1_25_GPIO            0x00                                      /* GPIO                         */
#define P1_25_MC1A            0x01                                      /* 电机控制PWM通道1，输出A      */
#define P1_25_CLKOUT          0x02                                      /* 时钟输出引脚                 */
#define P1_25_MAT11           0x03                                      /* 定时器1匹配输出，通道1       */

#define P1_25_FNUC            P1_25_GPIO

/*
 *  P1.26
 */
#define P1_26_GPIO            0x00                                      /* GPIO                         */
#define P1_26_MC1B            0x01                                      /* 电机控制PWM通道1，输出B      */
#define P1_26_PWM16           0x02                                      /* PWM-1输出，通道6             */
#define P1_26_CAP00           0x03                                      /* 定时器0捕获输入，通道0       */

#define P1_26_FNUC            P1_26_GPIO

/*
 *  P1.27
 */
#define P1_27_GPIO            0x00                                      /* GPIO                         */
#define P1_27_CLKOUT          0x01                                      /* 时钟输出引脚                 */
#define P1_27_USB_OVRCR       0x02                                      /* USB端口过载电流状态          */
#define P1_27_CAP01           0x03                                      /* 定时器0捕获输入，通道1       */

#define P1_27_FNUC            P1_27_GPIO

/*
 *  P1.28
 */
#define P1_28_GPIO            0x00                                      /* GPIO                         */
#define P1_28_MC2A            0x01                                      /* 电机控制PWM通道2，输出A      */
#define P1_28_PCAP10          0x02                                      /* PWM-1捕获输入，通道0         */
#define P1_28_MAT00           0x03                                      /* 定时器0匹配输出，通道0       */

#define P1_28_FNUC            P1_28_GPIO

/*
 *  P1.29
 */
#define P1_29_GPIO            0x00                                      /* GPIO                         */
#define P1_29_MC2B            0x01                                      /* 电机控制PWM通道2，输出B      */
#define P1_29_PCAP11          0x02                                      /* PWM-1捕获输入，通道1         */
#define P1_29_MAT01           0x03                                      /* 定时器0匹配输出，通道1       */

#define P1_29_FNUC            P1_29_GPIO

/*
 *  P1.30
 */
#define P1_30_GPIO            0x00                                      /* GPIO                         */
//...   Reserved              0x01                                      /* 保留                         */
#define P1_30_VBUS            0x02                                      /* 存在的USB总线电源            */
#define P1_30_AD04            0x03                                      /* ADC转换器0.通道4输入         */

#define P1_30_FNUC            P1_30_GPIO

/*
 *  P1.31
 */
#define P1_31_GPIO            0x00                                      /* GPIO                         */
//...   Reserved              0x01                                      /* 保留                         */
#define P1_31_SSCK1           0x02                                      /* SSP-1时钟线                  */
#define P1_31_AD05            0x03                                      /* ADC-0输入，通道5             */

#define P1_31_FNUC            P1_31_GPIO

/******************************************************************************
** P2口引脚功能设置
******************************************************************************/
/*
 *  P2.0
 */
#define P2_0_GPIO             0x00                                      /* GPIO                         */
#define P2_0_PWM11            0x01                                      /* PWM-1，通道1                 */
#define P2_0_TXD1             0x02                                      /* UART1数据发送引脚            */
//...   Reserved              0x03                                      /* 保留                         */

#define P2_0_FNUC             P2_0_TXD1

/*
 *  P2.1
 */
#define P2_1_GPIO             0x00                                      /* GPIO                         */
#define P2_1_PWM12            0x01                                      /* PWM-1，通道2                 */
#define P2_1_RXD1             0x02                                      /* UART1数据接收引脚            */
//...   Reserved              0x03                                      /* 保留                         */

#define P2_1_FNUC             P2_1_RXD1

/*
 *  P2.2
 */
#define P2_2_GPIO             0x00                                      /* GPIO                         */
#define P2_2_PWM13            0x01                                      /* PWM-1，通道3                 */
#define P2_2_CTS1             0x02                                      /* UART1清零发送线              */
//...   Reserved              0x03                                      /* 保留                         */

#define P2_2_FNUC             P2_2_CTS1

/*
 *  P2.3
 */
#define P2_3_GPIO             0x00                                      /* GPIO                         */
#define P2_3_PWM14            0x01                                      /* PWM-1，通道4                 */
#define P2_3_DCD1             0x02                                      /* UART1数据载波检测线          */
//...   Reserved              0x03                                      /* 保留                         */

#define P2_3_FNUC             P2_3_GPIO

/*
 *  P2.4
 */
#define P2_4_GPIO             0x00                                      /* GPIO                         */
#define P2_4_PWM15            0x01                                      /* PWM-1，通道5                 */
#define P2_4_DSR1             0x02                                      /* UART1数据设置就绪线          */
//...   Reserved              0x03                                      /* 保留                         */

#define P2_4_FNUC             P2_4_GPIO

/*
 *  P2.5
 */
#define P2_5_GPIO             0x00                                      /* GPIO                         */
#define P2_5_PWM16            0x01                                      /* PWM-1，通道6                 */
#define P2_5_DTR1             0x02                                      /* UART1数据终端就绪线          */
//...   Reserved              0x03                                      /* 保留                         */

#define P2_5_FNUC             P2_5_GPIO

/*
 *  P2.6
 */
#define P2_6_GPIO             0x00                                      /* GPIO                         */
#define P2_6_PCAP10           0x01                                      /* PWM-1捕获输入，通道0         */
#define P2_6_RI1              0x02                                      /* UART1振铃输入                */
//...   Reserved              0x03                                      /* 保留                         */

#define P2_6_FNUC             P2_6_GPIO

/*
 *  P2.7
 */
#define P2_7_GPIO             0x00                                      /* GPIO                         */
#define P2_7_RD2              0x01                                      /* CAN-2接收线                  */
#define P2_7_RTS1             0x02                                      /* UART1请求发送输出            */
                                                                        /* RS-485/EIA-485输出使能信号   */
//...   Reserved              0x03                                      /* 保留                         */

#define P2_7_FNUC             P2_7_RTS1

/*
 *  P2.8
 */
#define P2_8_GPIO             0x00                                      /* GPIO                         */
#define P2_8_TD2              0x01                                      /* CAN-2发送线                  */
#define P2_8_TXD2             0x02                                      /* UART2发送引脚                */
#define P2_8_ENET_MDC         0x03                                      /* 以太网MIIM时钟               */

#define P2_8_FNUC             P2_8_GPIO

/*
 *  P2.9
 */
#define P2_9_GPIO             0x00                                      /* GPIO                         */
#define P2_9_U1CONNECT        0x01                                      /* USB1软连接控制               */
#define P2_9_RXD2             0x02                                      /* UART2接收引脚                */
#define P2_9_ENET_MDIO        0x03                                      /* 以太网MIIM数据输入和输出     */

#define P2_9_FNUC             P2_9_GPIO

/*
 *  P2.10（ISP）
 */
#define P2_10_GPIO            0x00                                      /* GPIO                         */
#define P2_10_EINT0           0x01                                      /* 外部中断0输入                */
#define P2_10_NMI             0x02                                      /* 非屏蔽中断输入               */
//...   Reserved              0x03                                      /* 保留                         */

#define P2_10_FNUC            P2_10_GPIO

/*
 *  P2.11
 */
#define P2_11_GPIO            0x00                                      /* GPIO                         */
#define P2_11_EINT1           0x01                                      /* 外部中断1输入                */
//...   Reserved              0x02                                      /* 保留                         */
#define P2_11_I2STX_CLK       0x03                                      /* I2S发送时钟SCK               */
#define P2_11_FNUC            P2_11_GPIO
//#define P2_11_FNUC            P2_11_EINT1

/*
 *  P2.12
 */
#define P2_12_GPIO            0x00                                      /* GPIO                         */
#define P2_12_EINT2           0x01                                      /* 外部中断2输入                */
//...   Reserved              0x02                                      /* 保留                         */
#define P2_12_I2STX_WS        0x03                                      /* I2S发送字选择WS              */
#define P2_12_FNUC            P2_12_GPIO
//#define P2_12_FNUC            P2_12_EINT2

/*
 *  P2.13
 */
#define P2_13_GPIO            0x00                                      /* GPIO                         */
#define P2_13_EINT3           0x01                                      /* 外部中断3输入                */
//...   Reserved              0x02                                      /* 保留                         */
#define P2_13_I2STX_SDA       0x03                                      /* I2S发送数据SD                */

#define P2_13_FNUC            P2_13_GPIO//P2_13_EINT3

/******************************************************************************
** P3口引脚功能设置
******************************************************************************/
/*
 *  P3.25
 */
#define P3_25_GPIO            0x00                                      /*  GPIO                        */
//...   Reserved              0x01                                      /*  保留                        */
#define P3_25_MAT00           0x02                                      /*  定时器0匹配输出，通道0      */
#define P3_25_PWM12           0x03                                      /*  PWM-1输出，通道2            */

#define P3_25_FNUC            P3_25_GPIO

/*
 *  P3.26
 */
#define P3_26_GPIO            0x00                                      /* GPIO                         */
#define P3_26_STCLK           0x01                                      /* 系统节拍定时器时钟输入       */
#define P3_26_MAT01           0x02                                      /* 定时器0匹配输出，通道1       */
#define P3_26_PWM13           0x03                                      /* PWM-1输出，通道3             */

#define P3_26_FNUC            P3_26_GPIO

/******************************************************************************
** P4口引脚功能设置
******************************************************************************/
/*
 *  P4.28
 */
#define P4_28_GPIO            0x00                                      /* GPIO                         */
#define P4_28_RX_MCLK         0x01                                      /* —I2S接收主机时钟            */
#define P4_28_MAT20           0x02                                      /* 定时器2匹配输出，通道0       */
#define P4_28_TXD3            0x03                                      /* UART3发送引脚                */

#define P4_28_FNUC            P4_28_TXD3

/*
 *  P4.29
 */
#define P4_29_GPIO            0x00                                      /* GPIO                         */
#define P4_29_TX_MCLK         0x01                                      /* I2S发送主机时钟              */
#define P4_29_MAT21           0x02                                      /* 定时器2匹配输出，通道1       */
#define P4_29_RXD3            0x03                                      /* UART3接收引脚                */

#define P4_29_FNUC            P4_29_RXD3


/******************************************************************************
** P10  The Trace Port Interface Unit (TPIU)
******************************************************************************/

/*
 *  bit[3]
 */
#define TPIU_Disable          0x00                                      /* TPIU interface 禁止          */
#define TPIU_Enable           0x01                                      /* TPIU interface 使能          */

#define P10_3_FNUC            TPIU_Disable

/******************************************************************************
** 引脚内部上下拉电阻配置
******************************************************************************/
#define PIN_PULL_UP           0x00u                                     /* 引脚内部上拉                 */
#define PIN_REPEATER          0x01u                                     /* 引脚内部上拉                 */
#define PIN_NO_PULL           0x02u                                     /* 引脚既不上拉，也不下拉       */
#define PIN_PULL_DOWN         0x03u                                     /* 引脚内部下拉                 */

/*
 *  P0
 */
#define P0_0_MODE             PIN_NO_PULL
#define P0_1_MODE             PIN_NO_PULL
#define P0_2_MODE             PIN_NO_PULL
#define P0_3_MODE             PIN_NO_PULL
#define P0_4_MODE             PIN_PULL_DOWN         //下拉
#define P0_5_MODE             PIN_PULL_DOWN         //下拉
#define P0_6_MODE             PIN_PULL_UP           //上拉
#define P0_7_MODE             PIN_NO_PULL
#define P0_8_MODE             PIN_NO_PULL
#define P0_9_MODE             PIN_NO_PULL
#define P0_10_MODE            PIN_NO_PULL
#define P0_11_MODE            PIN_NO_PULL
#define P0_12_MODE            PIN_NO_PULL
#define P0_13_MODE            PIN_NO_PULL
#define P0_14_MODE            PIN_NO_PULL
#define P0_15_MODE            PIN_NO_PULL

#define P0_16_MODE            PIN_NO_PULL
#define P0_17_MODE            PIN_PULL_UP         //上拉
#define P0_18_MODE            PIN_NO_PULL
#define P0_19_MODE            PIN_NO_PULL
#define P0_20_MODE            PIN_NO_PULL
#define P0_21_MODE            PIN_NO_PULL
#define P0_22_MODE            PIN_NO_PULL
#define P0_23_MODE            PIN_NO_PULL
#define P0_24_MODE            PIN_NO_PULL
#define P0_25_MODE            PIN_NO_PULL
#define P0_26_MODE            PIN_NO_PULL
#define P0_27_MODE            PIN_NO_PULL
#define P0_28_MODE            PIN_NO_PULL
#define P0_29_MODE            PIN_NO_PULL
#define P0_30_MODE            PIN_NO_PULL
#define P0_31_MODE            PIN_NO_PULL

/*
 *  P1
 */
#define P1_0_MODE             PIN_NO_PULL
#define P1_1_MODE             PIN_NO_PULL
#define P1_2_MODE             PIN_NO_PULL
#define P1_3_MODE             PIN_NO_PULL
#define P1_4_MODE             PIN_NO_PULL
#define P1_5_MODE             PIN_NO_PULL
#define P1_6_MODE             PIN_NO_PULL
#define P1_7_MODE             PIN_NO_PULL
#define P1_8_MODE             PIN_NO_PULL
#define P1_9_MODE             PIN_NO_PULL
#define P1_10_MODE            PIN_NO_PULL
#define P1_11_MODE            PIN_NO_PULL
#define P1_12_MODE            PIN_NO_PULL
#define P1_13_MODE            PIN_NO_PULL
#define P1_14_MODE            PIN_NO_PULL
#define P1_15_MODE            PIN_NO_PULL
#define P1_16_MODE            PIN_PULL_DOWN     //下拉
#define P1_17_MODE            PIN_NO_PULL
#define P1_18_MODE            PIN_NO_PULL
#define P1_19_MODE            PIN_NO_PULL
#define P1_20_MODE            PIN_NO_PULL
#define P1_21_MODE            PIN_NO_PULL
#define P1_22_MODE            PIN_NO_PULL
#define P1_23_MODE            PIN_NO_PULL
#define P1_24_MODE            PIN_NO_PULL
#define P1_25_MODE            PIN_NO_PULL
#define P1_26_MODE            PIN_NO_PULL
#define P1_27_MODE            PIN_NO_PULL
#define P1_28_MODE            PIN_NO_PULL
#define P1_29_MODE            PIN_NO_PULL
#define P1_30_MODE            PIN_NO_PULL
#define P1_31_MODE            PIN_NO_PULL

/*
 *  P2
 */
#define P2_0_MODE             PIN_NO_PULL
#define P2_1_MODE             PIN_PULL_UP//PIN_NO_PULL
#define P2_2_MODE             PIN_PULL_DOWN         //下拉
#define P2_3_MODE             PIN_NO_PULL
#define P2_4_MODE             PIN_NO_PULL
#define P2_5_MODE             PIN_NO_PULL
#define P2_6_MODE             PIN_NO_PULL
#define P2_7_MODE             PIN_NO_PULL
#define P2_8_MODE             PIN_NO_PULL
#define P2_9_MODE             PIN_NO_PULL
#define P2_10_MODE            PIN_NO_PULL
#define P2_11_MODE            PIN_NO_PULL
#define P2_12_MODE            PIN_NO_PULL
#define P2_13_MODE            PIN_NO_PULL
#define P2_14_MODE            PIN_NO_PULL
#define P2_15_MODE            PIN_NO_PULL

/*
 *  P3
 */
#define P3_25_MODE            PIN_PULL_UP       //上拉
#define P3_26_MODE            PIN_PULL_DOWN     //下拉

/*
 *  P4
 */
#define P4_28_MODE            PIN_NO_PULL
#define P4_29_MODE            PIN_NO_PULL

/******************************************************************************
** 按原理图定义PIN
******************************************************************************/
/*
 *  P0信号宏定义
 */
#define     P0_SDA_E2           (1ul << 0)
#define     P0_SCL_E2           (1ul << 1)
#define     P0_TXD_4851         (1ul << 2)
#define     P0_RXD_4851         (1ul << 3)
#define     P0_CPU_EVERGOFF     (1ul << 4)
#define     P0_CPU_IGT          (1ul << 5)
#define     P0_PECTL            (1ul << 6)
#define     P0_ESAMI_CLK        (1ul << 7)
#define     P0_ESAM_DI          (1ul << 8)  //P0_ESAM_IO
#define     P0_ESAM_DO          (1ul << 9)

#define     P0_TXD_4852         (1ul << 10)
#define     P0_RXD_4852         (1ul << 11)
//                              (1ul << 12)
//                              (1ul << 13)
//                              (1ul << 14)
#define     P0_BEEP_ON          (1ul << 15)
#define     P0_ALARM_LED        (1ul << 16)
#define     P0_ESAM_RST         (1ul << 17)
#define     P0_C_GPRSON         (1ul << 18)
#define     P0_KEYPRG           (1ul << 19)
#define     P0_KEYG             (1ul << 20)
#define     P0_KEYD             (1ul << 21)
#define     P0_KEYUP            (1ul << 22)
#define     P0_BAT_AD           (1ul << 23)
#define     P0_BAT1_AD          (1ul << 24)
#define     P0_BAT2_AD          (1ul << 25)
#define     P0_EVENTOUT         (1ul << 26)
#define     P0_SDA_LCD          (1ul << 27)//azh
#define     P0_SCL_LCD          (1ul << 28)
#define     P0_PD_CHK           (1ul << 29)
#define     P0_KEYDOWN          (1ul << 30)
//                              (1ul << 31)

/*
 *  P1信号宏定义
 */
#define     P1_RLED_GPRS        (1ul << 0)
#define     P1_GLED_GPRS        (1ul << 1)
//                              (1ul << 2)
//                              (1ul << 3)
#define     P1_BGLED            (1ul << 4)
//                              (1ul << 5)
//                              (1ul << 6)
//                              (1ul << 7)
#define     P1_BAT_ON           (1ul << 8)
//#define     P1_GNET_LED         (1ul << 9)
//#define     P1_GDATA_LED        (1ul << 10)
#define     P1_RLED_WLS         (1ul << 9)
#define     P1_GLED_WLS         (1ul << 10)
//                              (1ul << 11)
//                              (1ul << 12)
//                              (1ul << 13)
#define     P1_FUNOUT           (1ul << 14)
#define     P1_ALARM            (1ul << 15)
#define     P1_DBG_LED          (1ul << 16)
#define     P1_IR_CTL           (1ul << 17)
#define     P1_PWM_38K          (1ul << 18)
#define     P1_WDI              (1ul << 19)
#define     P1_F1_SCK           (1ul << 20)
#define     P1_F1_CS2           (1ul << 21)
#define     P1_F1_CS1           (1ul << 22)
#define     P1_F1_MISO          (1ul << 23)
#define     P1_F1_MOSI          (1ul << 24)
#define     P1_FM_WP            (1ul << 25)
#define     P1_F1_WP            (1ul << 26)
#define     P1_E2_WP            (1ul << 27)
#define     P1_CPU_YGP          (1ul << 28)
#define     P1_CPU_WGP          (1ul << 29)
#define     P1_SW4852           (1ul << 30)
#define     P1_SW4851           (1ul << 31)

/*
 *  P2信号宏定义
 */
#define     P2_TXD_WLS          (1ul << 0)
#define     P2_RXD_WLS          (1ul << 1)
#define     P2_CTS_WLS          (1ul << 2)
#define     P2_GPRS_DTR         (1ul << 3)
#define     P2_RST_WLS          (1ul << 4)
#define     P2_GPRS_RTS         (1ul << 5)
#define     P2_GPRS_CTS         (1ul << 6)
#define     P2_RTS_WLS          (1ul << 7)
#define     P2_GPRS_DCD         (1ul << 8)
#define     P2_GPRS_RI          (1ul << 9)
#define     P2_ISP_EN           (1ul << 10)
#define     P2_IRRXD            (1ul << 11)
#define     P2_CPU_SECIN        (1ul << 12)
#define     P2_DIS_CTL          (1ul << 13)

/*
 *  P3信号宏定义
 */
#define     P3_FAST_OFF         (1ul << 25)
#define     P3_CHAG_ON          (1ul << 26)

/*
 *  P4信号宏定义
 */
#define     P4_GPRS_RXD1        (1ul << 28)//CPU_TXD
#define     P4_GPRS_TXD1        (1ul << 29)//CPU_RXD


/******************************************************************************
** 各个PORT口方向初始化值定义，把设置为输入的IO写到下面的宏，空闲的IO全部置为输出
******************************************************************************/
#define     P0_INIT_DIR         (~(P0_SDA_E2 |       \
                                   P0_RXD_4851 |     \
                                   P0_ESAM_DI |      \
                                   P0_RXD_4852 |     \
                                   P0_KEYPRG |       \
                                   P0_KEYG |         \
                                   P0_KEYD |         \
                                   P0_KEYUP  |       \
                                   P0_BAT_AD |       \
                                   P0_BAT1_AD |      \
                                   P0_BAT2_AD |      \
                                   P0_EVENTOUT |     \
                                   P0_SDA_LCD |      \
                                   P0_PD_CHK |       \
                                   P0_KEYDOWN        \
                                   )                 \
                                )

#define     P1_INIT_DIR         (~(P1_F1_MISO)       \
                                )

#define     P2_INIT_DIR        (~(P2_RXD_WLS |      \
                                   P2_ISP_EN |       \
                                   P2_IRRXD |        \
                                   P2_CPU_SECIN |    \
                                   P2_CTS_WLS |      \
                                   P2_GPRS_CTS |     \
                                   P2_GPRS_DCD |     \
                                   P2_GPRS_RI        \
                                   )                 \
                                )

#define     P3_INIT_DIR         (0xffffffff)

#define     P4_INIT_DIR         (~(P4_GPRS_TXD1     \
                                   )                 \
                                )

/******************************************************************************
** 各个PORT口输出初始化值定义，把设置为输出‘1’的IO写到下面的宏，空闲的IO全部置为输出‘0’
******************************************************************************/
#define     P0_INIT_DATA        (P0_SCL_E2 |         \
                                 P0_TXD_4851 |       \
                                 P0_CPU_EVERGOFF |   \
                                 P0_CPU_IGT |        \
                                 P0_ESAMI_CLK |      \
                                 P0_ESAM_DO |        \
                                 P0_TXD_4852 |       \
                                 P0_ALARM_LED |      \
                                 P0_SCL_LCD          \
                                 )

#define     P1_INIT_DATA        (P1_RLED_WLS |       \
                                 P1_GLED_WLS |       \
                                 P1_FUNOUT |         \
                                 P1_ALARM |          \
                                 P1_IR_CTL |         \
                                 P1_DBG_LED |        \
                                 P1_F1_SCK |         \
                                 P1_F1_CS2 |         \
                                 P1_F1_CS1 |         \
                                 P1_F1_MOSI |        \
                                 P1_FM_WP |          \
                                 P1_E2_WP |          \
                                 P1_SW4852 |         \
                                 P1_SW4851 |         \
                                 P1_CPU_YGP|         \
                                 P1_CPU_WGP          \
                                 )

#define     P2_INIT_DATA        (P2_TXD_WLS |        \
                                 P2_RTS_WLS |        \
                                 P2_DIS_CTL          \
                                 )

#define     P3_INIT_DATA        (P3_FAST_OFF         \
                                 )

#define     P4_INIT_DATA        (P4_GPRS_RXD1)

/******************************************************************************
** 各个PORT口操作宏定义
******************************************************************************/
/*
 *  P0操作宏
 */
#define     PVDD_ON()               FIO0DIR |= P0_PECTL;            \
                                    FIO0CLR = P0_PECTL                  //PECTL输出‘0’,外设电源开启
#define     PVDD_OFF()              FIO0DIR |= P0_PECTL;            \
                                    FIO0SET = P0_PECTL                  //PECTL输出‘1’,外设电源关闭
#define     EMERGOFF_EN()           FIO0DIR |= P0_CPU_EVERGOFF;     \
                                    FIO0CLR = P0_CPU_EVERGOFF           //CPU_EVERGOFF输出‘0’使能GPRS的EMERGOFF
#define     EMERGOFF_DIS()          FIO0DIR |= P0_CPU_EVERGOFF;     \
                                    FIO0SET = P0_CPU_EVERGOFF           //CPU_EVERGOFF输出‘1’禁能GPRS的EMERGOFF
#define     IGT_ON()                FIO0DIR |= P0_CPU_IGT;          \
                                    FIO0CLR = P0_CPU_IGT                //CPU_IGT输出‘0’Turn on MC55/56
#define     IGT_OFF()               FIO0DIR |= P0_CPU_IGT;          \
                                    FIO0SET = P0_CPU_IGT                //CPU_IGT输出‘1’Turn off MC55/56
#define     BEEP_ON_DIS()           FIO0DIR |= P0_BEEP_ON;          \
                                    FIO0CLR = P0_BEEP_ON                //BEEP_ON输出‘0’关闭蜂鸣器
#define     BEEP_ON_EN()            FIO0DIR |= P0_BEEP_ON;          \
                                    FIO0SET = P0_BEEP_ON                //BEEP_ON输出‘1’开启蜂鸣器
#define     ALARM_LED_ON()          FIO0DIR |= P0_ALARM_LED;        \
                                    FIO0CLR = P0_ALARM_LED              //ALARM_LED输出‘0’点亮报警灯
#define     ALARM_LED_OFF()         FIO0DIR |= P0_ALARM_LED;        \
                                    FIO0SET = P0_ALARM_LED              //ALARM_LED输出‘1’熄灭报警灯
#define     ESAM_RST_EN()           FIO0DIR |= P0_ESAM_RST;         \
                                    FIO0CLR = P0_ESAM_RST               //ESAM_RST输出‘0’,使能复位?
#define     ESAM_RST_DIS()          FIO0DIR |= P0_ESAM_RST;         \
                                    FIO0SET = P0_ESAM_RST               //ESAM_RST输出‘1’,禁能复位?
#define     GPRS_POWER_DIS()        FIO0DIR |= P0_C_GPRSON;         \
                                    FIO0CLR = P0_C_GPRSON               //C_GPRSON输出‘0’禁能GPRS供电
#define     GPRS_POWER_EN()         FIO0DIR |= P0_C_GPRSON;         \
                                    FIO0SET = P0_C_GPRSON               //C_GPRSON输出‘1’开启GPRS供电

#define     SET_PORT0_IN(pin)       FIO0DIR &= ~pin
#define     PORT0_IS_HIGH(pin)     (FIO0PIN & pin)
#define     SET_PORT0_OUT(pin)      FIO0DIR |= pin
#define     SET_PORT0_HIGH(pin)     FIO0SET = pin
#define     SET_PORT0_LOW(pin)      FIO0CLR = pin
#define     SET_PORT0_REV(pin)      if(PORT0_IS_HIGH(pin))          \
                                    {                               \
                                        SET_PORT0_LOW(pin);         \
                                    }                               \
                                    else                            \
                                    {                               \
                                        SET_PORT0_HIGH(pin);        \
                                    }

/*
 *  P1操作宏
 */
#define     BGLED_ON()              FIO1DIR |= P1_BGLED;            \
                                    FIO1SET = P1_BGLED                  //BGLED输出‘1’,点亮LCD背光灯
#define     BGLED_OFF()             FIO1DIR |= P1_BGLED;            \
                                    FIO1CLR = P1_BGLED                  //BGLED输出‘0’,熄灭LCD背光灯

#define     WLS_RLED_ON()           FIO1DIR |= P1_RLED_WLS;         \
                                    FIO1SET = P1_RLED_WLS               //RLED_WLS输出‘1’,点亮ZIGBEE红灯
#define     WLS_RLED_OFF()          FIO1DIR |= P1_RLED_WLS;         \
                                    FIO1CLR = P1_RLED_WLS               //RLED_WLS输出‘0’,熄灭ZIGBEE红灯
#define     WLS_GLED_ON()           FIO1DIR |= P1_GLED_WLS;         \
                                    FIO1SET = P1_GLED_WLS               //GLED_WLS输出‘1’,点亮ZIGBEE绿灯
#define     WLS_GLED_OFF()          FIO1DIR |= P1_GLED_WLS;         \
                                    FIO1CLR = P1_GLED_WLS               //GLED_WLS输出‘0’,熄灭ZIGBEE绿灯
#define     GPRS_RLED_ON()          FIO1DIR |= P1_RLED_GPRS;        \
                                    FIO1SET = P1_RLED_GPRS              //RLED_GPRS输出‘1’,点亮GPRS红灯
#define     GPRS_RLED_OFF()         FIO1DIR |= P1_RLED_GPRS;        \
                                    FIO1CLR = P1_RLED_GPRS              //RLED_GPRS输出‘0’,熄灭GPRS红灯
#define     GPRS_GLED_ON()          FIO1DIR |= P1_GLED_GPRS;        \
                                    FIO1SET = P1_GLED_GPRS              //GLED_GPRS输出‘1’,点亮GPRS绿灯
#define     GPRS_GLED_OFF()         FIO1DIR |= P1_GLED_GPRS;        \
                                    FIO1CLR = P1_GLED_GPRS              //GLED_GPRS输出‘0’,熄灭GPRS绿灯

#define     FUNOUT_ON()             FIO1DIR |= P1_FUNOUT;           \
                                    FIO1CLR = P1_FUNOUT                 //FUNOUT输出‘0’,功能输出开启
#define     FUNOUT_OFF()            FIO1DIR |= P1_FUNOUT;           \
                                    FIO1SET = P1_FUNOUT                 //FUNOUT输出‘1’,功能输出关闭
#define     ALARM_ON()              FIO1DIR |= P1_ALARM;            \
                                    FIO1CLR = P1_ALARM                  //ALARM输出‘0’,报警输出开启
#define     ALARM_OFF()             FIO1DIR |= P1_ALARM;            \
                                    FIO1SET = P1_ALARM                  //ALARM输出‘1’,报警输出关闭
#define     IR_VDD_ON()             FIO1DIR |= P1_IR_CTL;           \
                                    FIO1CLR = P1_IR_CTL                 //IR_CTL输出‘0’,IRDA电源开启
#define     IR_VDD_OFF()            FIO1DIR |= P1_IR_CTL;           \
                                    FIO1SET = P1_IR_CTL                 //IR_CTL输出‘1’,IRDA电源关闭

#define     DBG_LED_ON()            FIO1DIR |= P1_DBG_LED;          \
                                    FIO1CLR = P1_DBG_LED                //DEBUG_LED输出‘0’,DEBUG_LED点亮
#define     DBG_LED_OFF()           FIO1DIR |= P1_DBG_LED;          \
                                    FIO1SET = P1_DBG_LED                //DEBUG_LED输出‘1’,DEBUG_LED熄灭

#define     BAT_ON()                FIO1DIR |= P1_BAT_ON;          \
                                    FIO1SET = P1_BAT_ON                //BAT_ON输出‘0’,GPRS电池开启
#define     BAT_OFF()               FIO1DIR |= P1_BAT_ON;          \
                                    FIO1CLR = P1_BAT_ON                //BAT_ON输出‘1’,GPRS电池关闭



#define     FLS1_CS_EN()            FIO1DIR |= P1_F1_CS1;           \
                                    FIO1CLR = P1_F1_CS1                 //F1_CS1输出‘0’,FLASH1片选使能
#define     FLS1_CS_DIS()           FIO1DIR |= P1_F1_CS1;           \
                                    FIO1SET = P1_F1_CS1                 //F1_CS1输出‘1’,FLASH1片选禁能
#define     FLS2_CS_EN()            FIO1DIR |= P1_F1_CS2;           \
                                    FIO1CLR = P1_F1_CS2                 //F1_CS2输出‘0’,FLASH2片选使能
#define     FLS2_CS_DIS()           FIO1DIR |= P1_F1_CS2;           \
                                    FIO1SET = P1_F1_CS2                 //F1_CS2输出‘1’,FLASH2片选禁能
#define     DF_WP_EN()              FIO1DIR |= (P1_F1_WP);          \
                                    FIO1CLR = (P1_F1_WP)               //F1_WP输出‘0’,FLASH1写保护使能
#define     DF_WP_DIS()             FIO1DIR |= (P1_F1_WP);          \
                                    FIO1SET = (P1_F1_WP)               //F1_WP输出‘1’,FLASH1写保护禁能
#define     EE_WP_EN()              FIO1DIR |= P1_E2_WP;            \
                                    FIO1SET = P1_E2_WP                  //WP_E2输出‘1’,EEPROM写保护使能
#define     EE_WP_DIS()             FIO1DIR |= P1_E2_WP;            \
                                    FIO1CLR = P1_E2_WP                  //WP_E2输出‘0’,EEPROM写保护禁能
#define     FM_WP_EN()              FIO1DIR |= P1_FM_WP;            \
                                    FIO1SET = P1_FM_WP                  //WP_FM输出‘1’,FRAM写保护使能
#define     FM_WP_DIS()             FIO1DIR |= P1_FM_WP;            \
                                    FIO1CLR = P1_FM_WP                  //WP_FM输出‘0’,FRAM写保护禁能
#define     WGP_ON()                FIO1DIR |= P1_CPU_WGP;          \
                                    FIO1CLR = P1_CPU_WGP                //CPU_WGP输出‘0’,点亮无功灯并输出无功脉冲
#define     WGP_OFF()               FIO1DIR |= P1_CPU_WGP;          \
                                    FIO1SET = P1_CPU_WGP                //CPU_WGP输出‘1’,熄灭无功灯并关闭无功脉冲
#define     YGP_ON()                FIO1DIR |= P1_CPU_YGP;          \
                                    FIO1CLR = P1_CPU_YGP                //CPU_YGP输出‘0’,点亮有功灯并输出有功脉冲
#define     YGP_OFF()               FIO1DIR |= P1_CPU_YGP;          \
                                    FIO1SET = P1_CPU_YGP                //CPU_YGP输出‘1’,熄灭有功灯并关闭有功脉冲

#define     SET_PORT1_IN(pin)       FIO1DIR &= ~pin
#define     PORT1_IS_HIGH(pin)     (FIO1PIN & pin)
#define     SET_PORT1_OUT(pin)      FIO1DIR |= pin
#define     SET_PORT1_HIGH(pin)     FIO1SET = pin
#define     SET_PORT1_LOW(pin)      FIO1CLR = pin
#define     SET_PORT1_REV(pin)      if(PORT1_IS_HIGH(pin))          \
                                    {                               \
                                        SET_PORT1_LOW(pin);         \
                                    }                               \
                                    else                            \
                                    {                               \
                                        SET_PORT1_HIGH(pin);        \
                                    }

/*
 *  P2操作宏
 */
#define     WLS_RST_LOW()           FIO2DIR |= P2_RST_WLS;          \
                                    FIO2CLR = P2_RST_WLS                //RST_WLS输出‘0’,ZigBee复位使能
#define     WLS_RST_HIGH()          FIO2DIR |= P2_RST_WLS;          \
                                    FIO2SET = P2_RST_WLS                //RST_WLS输出‘1’,ZigBee复位禁能
#define     VLCD_ON()               FIO2DIR |= P2_DIS_CTL;          \
                                    FIO2SET = P2_DIS_CTL                //DIS_CTL输出‘1’,LCD电源开启
#define     VLCD_OFF()              FIO2DIR |= P2_DIS_CTL;          \
                                    FIO2CLR = P2_DIS_CTL                //DIS_CTL输出‘0’,LCD电源关闭

#define     SET_PORT2_IN(pin)       FIO2DIR &= ~pin
#define     PORT2_IS_HIGH(pin)     (FIO2PIN & pin)
#define     SET_PORT2_OUT(pin)      FIO2DIR |= pin
#define     SET_PORT2_HIGH(pin)     FIO2SET = pin
#define     SET_PORT2_LOW(pin)      FIO2CLR = pin
#define     SET_PORT2_REV(pin)      if(PORT2_IS_HIGH(pin))          \
                                    {                               \
                                        SET_PORT2_LOW(pin);         \
                                    }                               \
                                    else                            \
                                    {                               \
                                        SET_PORT2_HIGH(pin);        \
                                    }

/*
 *  P3操作宏
 */
#define     CHARGE_EN()             FIO3DIR |= P3_CHAG_ON;          \
                                    FIO3SET = P3_CHAG_ON                //CHAG_ON输出‘1’,GPRS充电电源开启
#define     CHARGE_DIS()            FIO3DIR |= P3_CHAG_ON;          \
                                    FIO3CLR = P3_CHAG_ON                //CHAG_ON输出‘1’,GPRS充电电源关闭
#define     FAST_CHARGE_ON()        FIO3DIR |= P3_FAST_OFF;         \
                                    FIO3CLR = P3_FAST_OFF               //FAST_OFF输出‘0’开起充电
#define     FAST_CHARGE_OFF()       FIO3DIR |= P3_FAST_OFF;         \
                                    FIO3SET = P3_FAST_OFF               //FAST_OFF输出‘1’关闭充电

#define     SET_PORT3_IN(pin)       FIO3DIR &= ~pin
#define     PORT3_IS_HIGH(pin)     (FIO3PIN & pin)
#define     SET_PORT3_OUT(pin)      FIO3DIR |= pin
#define     SET_PORT3_HIGH(pin)     FIO3SET = pin
#define     SET_PORT3_LOW(pin)      FIO3CLR = pin
#define     SET_PORT3_REV(pin)      if(PORT3_IS_HIGH(pin))          \
                                    {                               \
                                        SET_PORT3_LOW(pin);         \
                                    }                               \
                                    else                            \
                                    {                               \
                                        SET_PORT3_HIGH(pin);        \
                                    }


/*
 *  P4操作宏
 */
#define     SET_PORT4_IN(pin)       FIO4DIR &= ~pin
#define     PORT4_IS_HIGH(pin)     (FIO4PIN & pin)
#define     SET_PORT4_OUT(pin)      FIO4DIR |= pin
#define     SET_PORT4_HIGH(pin)     FIO4SET = pin
#define     SET_PORT4_LOW(pin)      FIO4CLR = pin
#define     SET_PORT4_REV(pin)      if(PORT4_IS_HIGH(pin))          \
                                    {                               \
                                        SET_PORT4_LOW(pin);         \
                                    }                               \
                                    else                            \
                                    {                               \
                                        SET_PORT4_HIGH(pin);        \
                                    }

#endif//                                    
/******************************************************************************
** 外设控制位在PCONP中定义
******************************************************************************/
#define PCTIM0_Enable         1                                         /* 禁止定时器/计数器0功率/时钟  */
#define PCTIM0_Disable        0                                         /* 使能定时器/计数器0功率/时钟  */
#define PCTIM0_FUNC           PCTIM0_Disable

#define PCTIM1_Enable         1                                         /* 禁止定时器/计数器1功率/时钟  */
#define PCTIM1_Disable        0                                         /* 禁止定时器/计数器1功率/时钟  */
#define PCTIM1_FUNC           PCTIM1_Disable

#define PCUART0_Enable        1                                         /* 使能UART0功率/时钟控制位     */
#define PCUART0_Disable       0                                         /* 禁止UART0功率/时钟控制位     */
#define PCUART0_FUNC          PCUART0_Disable

#define PCUART1_Enable        1                                         /* 使能UART1功率/时钟控制位     */
#define PCUART1_Disable       0                                         /* 禁止UART1功率/时钟控制位     */
#define PCUART1_FUNC          PCUART1_Disable

#define PWM1_Enable           1                                         /* 使能PWM1功率/时钟控制位      */
#define PWM1_Disable          0                                         /* 禁止PWM1功率/时钟控制位      */
#define PWM1_FUNC             PWM1_Disable

#define PCI2C0_Enable         1                                         /* 使能I2C0接口功率/时钟控制位  */
#define PCI2C0_Disable        0                                         /* 禁止I2C0接口功率/时钟控制位  */
#define PCI2C0_FUNC           PCI2C0_Disable

#define PCSPI_Enable          1                                         /* 使能SPI接口功率/时钟控制位   */
#define PCSPI_Disable         0                                         /* 禁止SPI接口功率/时钟控制位   */
#define PCSPI_FUNC            PCSPI_Disable

#define PCRTC_Enable          1                                         /* 使能RTC功率/时钟控制位       */
#define PCRTC_Disable         0                                         /* 禁止RTC功率/时钟控制位       */
#define PCRTC_FUNC            PCRTC_Disable

#define PCSSP1_Enable         1                                         /* 使能SSP1接口功率/时钟控制位  */
#define PCSSP1_Disable        0                                         /* 禁止SSP1接口功率/时钟控制位  */
#define PCSSP1_FUNC           PCSSP1_Disable

#define PCAD_Enable           1                                         /* 使能A/D转换器功率/时钟控制位 */
#define PCAD_Disable          0                                         /* 禁止A/D转换器功率/时钟控制位 */
#define PCAD_FUNC             PCAD_Disable

#define PCCAN1_Enable         1                                         /* 使能CAN控制器1功率/时钟控制位*/
#define PCCAN1_Disable        0                                         /* 禁止CAN控制器1功率/时钟控制位*/
#define PCCAN1_FUNC           PCCAN1_Disable

#define PCCAN2_Enable         1                                         /* 使能CAN控制器2功率/时钟控制位*/
#define PCCAN2_Disable        0                                         /* 禁止CAN控制器2功率/时钟控制位*/
#define PCCAN2_FUNC           PCCAN2_Disable

#define PCGPIO_Enable         1                                         /* 使能GPIO                     */
#define PCGPIO_Disable        0                                         /* 禁止GPIO                     */
#define PCGPIO_FUNC           PCGPIO_Disable

#define PCRIT_Enable          1                                         /* 使能重复中断定时器           */
#define PCRIT_Disable         0                                         /* 禁止重复中断定时器           */
#define PCRIT_FUNC            PCRIT_Disable

#define PCMC_Enable           1                                         /* 使能电机控制PWM              */
#define PCMC_Disable          0                                         /* 禁止电机控制PWM              */
#define PCMC_FUNC             PCMC_Disable

#define PCQEI_Enable          1                                         /* 使能正交编码器接口           */
#define PCQEI_Disable         0                                         /* 禁止正交编码器接口           */
#define PCQEI_FUNC            PCQEI_Disable

#define PCI2C0_Enable         1                                         /* 使能I2C0接口功率/时钟控制位  */
#define PCI2C0_Disable        0                                         /* 禁止I2C0接口功率/时钟控制位  */
#define PCI2C0_FUNC           PCI2C0_Disable

#define PCI2C1_Enable         1                                         /* 使能I2C1接口功率/时钟控制位  */
#define PCI2C1_Disable        0                                         /* 禁止I2C1接口功率/时钟控制位  */
#define PCI2C1_FUNC           PCI2C1_Disable

#define PCSSP0_Enable         1                                         /* 使能SSP0接口功率/时钟控制位  */
#define PCSSP0_Disable        0                                         /* 禁止SSP0接口功率/时钟控制位  */
#define PCSSP0_FUNC           PCSSP0_Disable

#define PCTIM2_Enable         1                                         /* 使能定时器2功率/时钟控制位   */
#define PCTIM2_Disable        0                                         /* 禁止定时器2功率/时钟控制位   */
#define PCTIM2_FUNC           PCTIM2_Disable

#define PCTIM3_Enable         1                                         /* 使能定时器3功率/时钟控制位   */
#define PCTIM3_Disable        0                                         /* 禁止定时器3功率/时钟控制位   */
#define PCTIM3_FUNC           PCTIM3_Disable

#define PCUART2_Enable        1                                         /* 使能UART2功率/时钟控制位     */
#define PCUART2_Disable       0                                         /* 禁止UART2功率/时钟控制位     */
#define PCUART2_FUNC          PCUART2_Disable

#define PCUART3_Enable        1                                         /* 使能UART3功率/时钟控制位     */
#define PCUART3_Disable       0                                         /* 禁止UART3功率/时钟控制位     */
#define PCUART3_FUNC          PCUART3_Disable

#define PCI2C2_Enable         1                                         /* 使能I2C接口2功率/时钟控制位  */
#define PCI2C2_Disable        0                                         /* 禁止I2C接口2功率/时钟控制位  */
#define PCI2C2_FUNC           PCI2C2_Disable

#define PCI2S_Enable          1                                         /* 使能I2S接口功率/时钟控制位   */
#define PCI2S_Disable         0                                         /* 禁止I2S接口功率/时钟控制位   */
#define PCI2S_FUNC            PCI2S_Disable

#define PCGPDMA_Enable        1                                         /* 使能GP DMA功能功率/时钟控制位*/
#define PCGPDMA_Disable       0                                         /* 禁止GP DMA功能功率/时钟控制位*/
#define PCGPDMA_FUNC          PCGPDMA_Disable

#define PCENET_Enable         1                                         /* 使能以太网模块功率/时钟控制位*/
#define PCENET_Disable        0                                         /* 禁止以太网模块功率/时钟控制位*/
#define PCENET_FUNC           PCENET_Disable

#define PCUSB_Enable          1                                         /* 使能USB接口功率/时钟控制位   */
#define PCUSB_Disable         0                                         /* 禁止USB接口功率/时钟控制位   */
#define PCUSB_FUNC            PCUSB_Disable

/*-------------------------- functions declaration --------------------------*/
__EXTERN    void    pin_init(void);
__EXTERN    void    pconp_init(void);

#endif//__LPC1700_PIN_CFG_H

/******************************************************************************
    End Of File
******************************************************************************/

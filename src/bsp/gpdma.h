/******************************************************************************
 Copyright (C) 2011  R&D Institute of HuaCai Co.,Ltd.
 File Name      : gpdma.h
 Description    : LPC17系列芯片GPDMA子程序的头文件, 配合gpdma.c
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2011-03-03    wwllzz          the original version
******************************************************************************/
#ifndef __GPDMA_H
#define __GPDMA_H
//----------------------------- macro definition ------------------------------
#define DMAReqSel               (*(volatile unsigned long *)0x400fc1c4)

#define DMACC0_CONTROL          (1 << 26)               //个数0, 8位, 源++, 关闭计数中断
#define DMACC1_CONTROL          ((1 << 27) | (1u << 31))//个数0, 8位, 目标++, 计数中断使能
#ifdef UART3_IRDA_ENABLE
#define DMACC2_CONTROL_FE       (4 | (2 << 18) | (2 << 21) | (1 << 26) | (1u << 31))
                                                        //个数4, 字宽度(32位), 源++, 计数中断使能
#else// UART3_IRDA_ENABLE
#define DMACC2_CONTROL_FE       (2 | (2 << 18) | (2 << 21) | (1 << 26) | (1u << 31))
                                                        //个数2, 字宽度(32位), 源++, 计数中断使能
#endif// UART3_IRDA_ENABLE
#define DMACC2_CONTROL_TX       ((1 << 26) | (1u << 31))
                                                        //个数0, 8位, 源++, 计数中断使能
#define DMACC3_CONTROL          (1 << 26)
                                                        //个数0, 8位, 源++, 关闭计数中断
//#define DMACC3_CONTROL          ((1 << 26)|0x8000)       //azh zlg

#define DMACC4_CONTROL          (1 << 26)
                                                        //个数0, 8位, 源++, 关闭计数中断

#define DMACC0_CONFIG           ((0 << 6) | (1 << 11))
                                                        //外设0, 内存到外设, 屏蔽中断输出
#define DMACC1_CONFIG           ((1 << 1) | (2 << 11) | (1 << 14) | (1 << 15))
                                                        //外设1, 外设到内存, 计数错误中断输出
#define DMACC2_CONFIG_FE        ((12 << 6) | (1 << 11) | (1 << 14) | (1 << 15))
                                                        //外设线12, 内存到外设, 计数错误中断输出
#define DMACC2_CONFIG_TX        ((14 << 6) | (1 << 11) | (1 << 14) | (1 << 15))
                                                        //外设14(UART3_TX), 内存到外设, 计数错误中断输出
#define DMACC3_CONFIG           ((10 << 6) | (1 << 11) | (1 << 14))
                                                        //外设10(UART1_TX), 内存到外设, 屏蔽中断输出
#define DMACC4_CONFIG           ((8 << 6) | (1 << 11) | (1 << 14))
                                                        //外设8(UART0_TX), 内存到外设, 屏蔽中断输出

#define start_uart3_fe_gpdma()              DMACC2SrcAddr = (U32)Mat2_0_Out_FE;     \
                                            DMACC2DestAddr = (U32)(&T2MR0);         \
                                            DMACC2Control = DMACC2_CONTROL_FE;      \
                                            DMACC2Config = DMACC2_CONFIG_FE | 1;        //配置DMA通道2用于T2MR0的匹配输出

#define start_uart3_tx_gpdma(src_addr, len) DMACC2SrcAddr = src_addr;                           \
                                            DMACC2DestAddr = (U32)(&U3THR);                     \
                                            DMACC2Control = DMACC2_CONTROL_TX | (len & 0x0fff); \
                                            DMACC2Config = DMACC2_CONFIG_TX | 1;        //配置DMA通道2用于UART3的发送，DMA传输的源地址和长度

#define start_uart1_tx_gpdma(src_addr, len) DMACC3SrcAddr = src_addr;                           \
                                            DMACC3DestAddr = (U32)(&U1THR);						\
                                            DMACC3Control = DMACC3_CONTROL | (len & 0x0fff);    \
                                            DMACC3Config = DMACC3_CONFIG | 1;           //配置DMA通道3用于UART1的发送，DMA传输的源地址和长度

#define start_uart0_tx_gpdma(src_addr, len) DMACC4SrcAddr = src_addr;                           \
                                            DMACC4Control = DMACC4_CONTROL | (len & 0x0fff);    \
                                            DMACC4Config = DMACC4_CONFIG | 1;           //配置DMA通道4用于UART0的发送，DMA传输的源地址和长度

//----------------------------- type definition -------------------------------

//--------------------------- variable declaration ----------------------------

//-------------------------- functions declaration ----------------------------
__EXTERN void GPDMA_Init(void);
__EXTERN void DMA_IRQHandler(void);
#ifdef ZIGBEE_TEST
__EXTERN void Zigbee_DMA_Init(void);
#endif
//__EXTERN void set_dma_channel4(U32 dest_addr);
//__EXTERN void startup_ssp0_gpdma(void);

#endif  //__GPDMA_H

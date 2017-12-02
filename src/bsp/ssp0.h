/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : ssp0.h
 Description    : LPC17系列芯片硬件SSP0子程序的头文件
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2010-07-27      snap           the original version
******************************************************************************/

#ifndef __SSP0_H
#define __SSP0_H

//----------------------------- macro definition ------------------------------
#define CPSR_DIV                    2                   //2～254 中的一个偶数值
#define SSP0_FREQ                   1000000//2000000             //SSP1 bus speed is defined as 2M bps(目前超过9M的值会影响程序中宏的计算)
                              
#define SSP_NUM                     2                   //定义使用的SSP的数量，与分配的变量数组相关
#define SSP_FIFO_SIZE               8                   //SSP FIFO definition
                              
#define SSP_RD                      0x00
#define SSP_WR                      0x01

//-----------------------------------------------------------------------------
// SSP interrupt maskable contrl
//-----------------------------------------------------------------------------
#define IMSC_RORIM                  0x01
#define IMSC_RTIM                   0x02
#define IMSC_RXIM                   0x04
#define IMSC_TXIM                   0x08

//-----------------------------------------------------------------------------
// SSP CR1 contrl
//-----------------------------------------------------------------------------
#define CR1_LBM                     0x01
#define CR1_SSE                     0x02
#define CR1_MS                      0x04
#define CR1_SOD                     0x08

//-----------------------------------------------------------------------------
// SSP CR0 contrl
//-----------------------------------------------------------------------------
#define CR0_DSS                     0
#define CR0_FRF                     4
#define CR0_CPOL                    6
#define CRO_CPHA                    7
#define CRO_SCR                     8

//-----------------------------------------------------------------------------
// SSP ICR contrl
//-----------------------------------------------------------------------------
#define ICR_RORIC                   0x01
#define ICR_RTIC                    0x02

//-----------------------------------------------------------------------------
// SSP SR contrl
//-----------------------------------------------------------------------------
#define SR_TFE                      0x01
#define SR_TNF                      0x02
#define SR_RNE                      0x04
#define SR_RFF                      0x08
#define SR_BSY                      0x10

//-----------------------------------------------------------------------------
// SSP MIS contrl
//-----------------------------------------------------------------------------
#define MIS_RORMIS                  0x01
#define MIS_RTMIS                   0x02
#define MIS_RXMIS                   0x04
#define MIS_TXMIS                   0x08

//----------------------------- type definition -------------------------------
typedef enum
{
    SSP0,
    SSP1
}SSP_INDEX;                     //该枚举类型与SSP_NUM相关，个数应与SSP_NUM一致

//-----------------------------------------------------------------------------
// SSP state definition
//-----------------------------------------------------------------------------
typedef enum
{
    SSP_IDLE,
    SSP_SEND,
    SSP_RCV,
    SSP_END,
	SSP_BUSY
}SSP_STATUS;

//-----------------------------------------------------------------------------
// SSP全局变量结构定义
//-----------------------------------------------------------------------------
typedef struct
{
    U8 Operation;
    VU8 End_Flag;
    VU8 *Send_Ptr;
    VU8 *Rcv_Ptr;
    VU16 Send_Num;
    VU16 Rcv_Num;
    VS32 Error;
}SSP_STRUCT;

//----------------------------- type definition -------------------------------

//-------------------------- functions declaration ----------------------------
__EXTERN void ssp0_init(void);
__EXTERN S32 ssp0_operation(U8 *send_buff, U8 *rcv_buff, U16 num, U8 operation);
__EXTERN void SSP0_IRQHandler(void);
//--------------------------- variable declaration ----------------------------
__EXTERN SSP_STRUCT SSP_Ctrl[SSP_NUM];

#endif//__SSP0_H

/******************************************************************************
    End Of File
******************************************************************************/

/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : ssp1.h
 Description    : LPC17ϵ��оƬӲ��SSP1�ӳ����ͷ�ļ�
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��    1.00        2010-07-27      snap           the original version
******************************************************************************/

#ifndef __SSP1_H
#define __SSP1_H

//----------------------------- macro definition ------------------------------
#define SSP1_FREQ                   1000000             //SSP1 bus speed is defined as 1M bps(��������FPCLK/CPSR_DIV�����������Դ��ڶ���ֵ)


//-------------------------- functions declaration ----------------------------
__EXTERN S32 ssp1_operation(U8 *send_buff, U8 *rcv_buff, U16 num, U8 operation);
__EXTERN void SSP1_IRQHandler(void);

//--------------------------- variable declaration ----------------------------

#endif//__SSP1_H

/******************************************************************************
    End Of File
******************************************************************************/

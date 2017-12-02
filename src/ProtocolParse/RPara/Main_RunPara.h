/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_RunPara.h
 Description    : ������Լ��Ӧ���в���
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-08-15    azh     the original version
******************************************************************************/

#ifndef _MAIN_RUNPARA_H_
#define	_MAIN_RUNPARA_H_

#define CON_PARA_INIT_POW_ON    0xaa//�ϵ������ʼ��
#define CON_PARA_INIT_ALL_CLR   0x55//���������ʼ��

__EXTERN void  Save_MP_ParaEE(unsigned char ucMeterNo, STMETERPARA *pstMeterPara);
__EXTERN void  Init_MP_ParaEE(unsigned char ucClrFlag);

#endif

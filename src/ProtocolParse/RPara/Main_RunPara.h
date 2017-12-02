/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_RunPara.h
 Description    : 国网规约对应运行参数
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-08-15    azh     the original version
******************************************************************************/

#ifndef _MAIN_RUNPARA_H_
#define	_MAIN_RUNPARA_H_

#define CON_PARA_INIT_POW_ON    0xaa//上电参数初始化
#define CON_PARA_INIT_ALL_CLR   0x55//参数总清初始化

__EXTERN void  Save_MP_ParaEE(unsigned char ucMeterNo, STMETERPARA *pstMeterPara);
__EXTERN void  Init_MP_ParaEE(unsigned char ucClrFlag);

#endif

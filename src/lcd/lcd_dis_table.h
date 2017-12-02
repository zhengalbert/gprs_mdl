/****************************************************************************** 
 Copyright (C) 2011  R&D Institute of HuaCai Co.,Ltd. 
 File Name  : lcd_dis_table.h 
 Description: 上位机软件配合显示生成的控制表格 
 ------------------------------------------------------------------------------ 
 Modification History: 
 <No.>  <version>       <time>      <author>        <contents> 
   2、 
   1、    1.00        2011-06-12      azh           the original version 
******************************************************************************/ 
 
#ifndef __LCD_DIS_TABLE_H 
#define __LCD_DIS_TABLE_H 
 
#ifdef LCD_VALID 
//----------------------------- macro definition ------------------------------ 
 
//----------------------------- type definition ------------------------------- 
 
 
//-------------------------- functions declaration ---------------------------- 
 
//--------------------------- variable declaration ---------------------------- 
#define FIXCHAR_TABLE_MAXNUM	725
#define DI3_00_TABLE_MAXNUM		69
#define DI3_01_TABLE_MAXNUM		120
#define DI3_02_TABLE_MAXNUM		41
#define DI3_03_TABLE_MAXNUM		202//1571  //azh 去掉一些不太可能的显示失压失流等
#define DI3_10_TABLE_MAXNUM		200//243//1563
#define DI3_04_TABLE_MAXNUM		100
#define OFFSET_TABLE_MAXNUM		88


//显示方案数据
extern const U8 fixchar_table[][8];

//电能量数据标识
extern const DIS_CTRL_TCB  disctrbydi_di300table[];

//最大需量及发生时间数据标识
extern const DIS_CTRL_TCB disctrbydi_di301table[];

//变量数据标识
extern const DIS_CTRL_TCB disctrbydi_di302table[];

//事件记录数据标识
extern const DIS_CTRL_TCB disctrbydi_di303table[];

//事件记录数据标识
extern const DIS_CTRL_TCB disctrbydi_di310table[];

//参变量数据标识
extern const DIS_CTRL_TCB disctrbydi_di304table[];

//偏移指针
extern const ADDR_OFFSET_TCB dis_exdctr_offsetbysn_table[];
#endif


#endif//__LCD_DIS_TABLE_H

/******************************************************************************
    End Of File
******************************************************************************/


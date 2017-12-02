/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 Module         : timing function head file
 File Name      : timing.h
 Description    :
 Author         :
 Build Date     :
 others         :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、     1.00       2009-06-30     azh       create
******************************************************************************/

#ifndef __TIMING_H
#define __TIMING_H
/*----------------------------- macro definition ----------------------------*/
#define     REPORT_CONTENT_BUF      1024

#define     VALID_SECOND_INDEX      (3600 * 24 * 365 * 100ul)

//用户应用其它宏定义:计量数据转存部分
#define DATA_SAVE_TIME              15      //内存转存至外存间隔时间,单位:分钟
/*----------------------------- type definition -----------------------------*/


/*-------------------------- functions declaration --------------------------*/
__EXTERN    __task  void    timing_task (void);

//__EXTERN            void    reflash_system_RTC (void);
/*--------------------------- variable declaration --------------------------*/
//__EXTERN    U32         Current_Date;               //BCD格式的当前日期： YYYY-MM-DD
//__EXTERN    U32         Current_Time;               //BCD格式的当前时间：WW-HH-MM-SS
//__EXTERN    U32         Second_Index;               //秒索引（二进制）

//__EXTERN    U32         Power_Up_Record_Delay;      //从上电开始到记录这个事件的秒延时
__EXTERN    U16         Min_Cnt;                    //分钟计数器

#endif//__TIMING_H
/******************************************************************************
                            End Of File
 *****************************************************************************/

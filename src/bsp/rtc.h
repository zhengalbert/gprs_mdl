/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : rtc.h
 Description    : RTC相关函数的头文件
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2010-07-23      snap           the original version
******************************************************************************/

#ifndef __RTC_H
#define __RTC_H

//----------------------------- macro definition ------------------------------
#define RX8025_CAPACITY             16                          //定义RTC(RX8025)的容量16Bytes
#define RX8025_ADDR                 0x64                        //RX8025 slave address

#define RX8025_HEAD_ADDR            0x63000000                  //定义RTC(RX8025)的逻辑起始地址
#define RX8025_EXT_REG_ADDR         (RX8025_HEAD_ADDR + 13)     //定义RTC(RX8025)的扩展寄存器地址
#define RX8025_FLAG_REG_ADDR        (RX8025_HEAD_ADDR + 14)     //定义RTC(RX8025)的标志寄存器地址
#define RX8025_CTRL_REG_ADDR        (RX8025_HEAD_ADDR + 15)     //定义RTC(RX8025)的控制寄存器地址

#define RX8025_EXT_REG_CONFIG       0x00                        //定义RTC(RX8025)的扩展寄存器的配置值，选择秒更新/INT输出1Hz、禁止FOUT输出
#define RX8025_FLAG_REG_CONFIG      0x00                        //定义RTC(RX8025)的标志寄存器的配置值
#define RX8025_CTRL_REG_CONFIG      0x60                        //定义RTC(RX8025)的控制寄存器的配置值，温度补偿的时间间隔2秒、使能更新中断
#define RX8025_FLAG_REG_MASK        0xc6                        //定义RTC(RX8025)的标志寄存器的检查屏蔽位（需要检查的位）

//-----------------------------------------------------------------------------
#define DS3231_CAPACITY             19                          //定义RTC(DS3231)的容量19Bytes
#define DS3231_ADDR                 0xd0                        //DS3231 slave address

#define DS3231_HEAD_ADDR            0x64000000                  //定义RTC(DS3231)的逻辑起始地址
#define DS3231_CTRL_REG_ADDR        (DS3231_HEAD_ADDR + 14)     //定义RTC(DS3231)的控制寄存器地址
#define DS3231_STATUS_REG_ADDR      (DS3231_HEAD_ADDR + 15)     //定义RTC(DS3231)的状态寄存器地址
#define DS3231_AGING_OFFSET_ADDR    (DS3231_HEAD_ADDR + 16)     //定义RTC(DS3231)的老化补偿寄存器地址

#define DS3231_CTRL_REG_CONFIG      0x00                        //定义RTC(DS3231)的控制寄存器的配置值，使能振荡器、禁止掉电SQW输出、选择SQW并输出1Hz
#define DS3231_STATUS_REG_CONFIG    0x00                        //定义RTC(DS3231)的状态寄存器的配置值，禁止32kHz输出
#define DS3231_STATUS_REG_MASK      0x88                        //定义RTC(DS3231)的状态寄存器的检查屏蔽位（需要检查的位）
#define DS3231_AGING_OFFSET_CONFIG  0x00                        //定义RTC(DS3231)的老化补偿寄存器值

//-----------------------------------------------------------------------------
#define RTC_RX8025                  0                           //RTC(RX8025)，用于时钟芯片选择
#define RTC_DS3231                  1                           //RTC(DS3231)，用于时钟芯片选择

#if HARDWARE_RTC == RTC_RX8025
#define HARDWARE_RTC_ADDR           RX8025_HEAD_ADDR            //时钟芯片地址指向RTC(RX8025)
#else//HARDWARE_RTC == RTC_RX8025
#define HARDWARE_RTC_ADDR           DS3231_HEAD_ADDR            //时钟芯片地址指向RTC(DS3231)
#endif//HARDWARE_RTC == RTC_RX8025

//----------------------------- type definition -------------------------------
typedef		struct	_STDATETIME
{
	unsigned	char	ucYear;
	unsigned	char	ucMonth;
	unsigned	char	ucDay;
	unsigned	char	ucHour;
	unsigned	char	ucMinute;
	unsigned	char	ucSecond;
}STDATETIME,*PSTDATETIME;

__packed typedef struct                                         //这里的时间都是二进制, 非BCD格式的
{
    U8 Sec;
    U8 Min;
    U8 Hour;
    U8 Day;                                                     //周次是用日期计算出来的
    U8 Date;
    U8 Month;
    U8 Year;
}RTC_STRUCT;

typedef struct
{
    U8 MonthDays;                                               //Number of days in each month
    U8 MonthVal;                                                //Value used to compute day of the week
}CALENDAR_MONTH;

//-------------------------- functions declaration ----------------------------
__EXTERN U8 software_rtc_run(void);
__EXTERN U8 last_day_of_month(U8 year, U8 month);
__EXTERN S32 set_sys_time(U8 *time);
__EXTERN S32 set_sys_date(U8 *date);
__EXTERN S32 get_sys_date(U8 *date);
__EXTERN S32 set_sys_calendar(U8 *calendar);
__EXTERN S32 check_rtc_batt(void);
#ifdef HARD_RTC
__EXTERN void sys_rtc_init(void);
__EXTERN void check_sys_rtc(void);
__EXTERN void SetDateTime_ToRTC(PSTDATETIME pstDateTime);
#endif
__EXTERN void update_date(void);
__EXTERN void update_day_of_week(void);
__EXTERN void get_software_rtc(U8 *time);
__EXTERN void get_dis_date_time(U32 di, U8 *buf, U8* len);
__EXTERN S32 get_hardware_rtc(U8 *time);
__EXTERN void GetDateTime_SoftRTC(PSTDATETIME pstDateTime);
__EXTERN U8 GetDateTime_Week(void);//azh 170908
__EXTERN void SetDateTime_SoftRTC(PSTDATETIME pstDateTime);
//--------------------------- variable declaration ----------------------------
__EXTERN U8 Sec_Sign;                                           //软件时钟秒标志
__EXTERN volatile RTC_STRUCT Calendar;                          //内部软时钟, 这里的时间都是二进制, 非BCD格式的

#endif//__RTC_H

/******************************************************************************
    End Of File
******************************************************************************/

/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : rtc.h
 Description    : RTC��غ�����ͷ�ļ�
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��    1.00        2010-07-23      snap           the original version
******************************************************************************/

#ifndef __RTC_H
#define __RTC_H

//----------------------------- macro definition ------------------------------
#define RX8025_CAPACITY             16                          //����RTC(RX8025)������16Bytes
#define RX8025_ADDR                 0x64                        //RX8025 slave address

#define RX8025_HEAD_ADDR            0x63000000                  //����RTC(RX8025)���߼���ʼ��ַ
#define RX8025_EXT_REG_ADDR         (RX8025_HEAD_ADDR + 13)     //����RTC(RX8025)����չ�Ĵ�����ַ
#define RX8025_FLAG_REG_ADDR        (RX8025_HEAD_ADDR + 14)     //����RTC(RX8025)�ı�־�Ĵ�����ַ
#define RX8025_CTRL_REG_ADDR        (RX8025_HEAD_ADDR + 15)     //����RTC(RX8025)�Ŀ��ƼĴ�����ַ

#define RX8025_EXT_REG_CONFIG       0x00                        //����RTC(RX8025)����չ�Ĵ���������ֵ��ѡ�������/INT���1Hz����ֹFOUT���
#define RX8025_FLAG_REG_CONFIG      0x00                        //����RTC(RX8025)�ı�־�Ĵ���������ֵ
#define RX8025_CTRL_REG_CONFIG      0x60                        //����RTC(RX8025)�Ŀ��ƼĴ���������ֵ���¶Ȳ�����ʱ����2�롢ʹ�ܸ����ж�
#define RX8025_FLAG_REG_MASK        0xc6                        //����RTC(RX8025)�ı�־�Ĵ����ļ������λ����Ҫ����λ��

//-----------------------------------------------------------------------------
#define DS3231_CAPACITY             19                          //����RTC(DS3231)������19Bytes
#define DS3231_ADDR                 0xd0                        //DS3231 slave address

#define DS3231_HEAD_ADDR            0x64000000                  //����RTC(DS3231)���߼���ʼ��ַ
#define DS3231_CTRL_REG_ADDR        (DS3231_HEAD_ADDR + 14)     //����RTC(DS3231)�Ŀ��ƼĴ�����ַ
#define DS3231_STATUS_REG_ADDR      (DS3231_HEAD_ADDR + 15)     //����RTC(DS3231)��״̬�Ĵ�����ַ
#define DS3231_AGING_OFFSET_ADDR    (DS3231_HEAD_ADDR + 16)     //����RTC(DS3231)���ϻ������Ĵ�����ַ

#define DS3231_CTRL_REG_CONFIG      0x00                        //����RTC(DS3231)�Ŀ��ƼĴ���������ֵ��ʹ����������ֹ����SQW�����ѡ��SQW�����1Hz
#define DS3231_STATUS_REG_CONFIG    0x00                        //����RTC(DS3231)��״̬�Ĵ���������ֵ����ֹ32kHz���
#define DS3231_STATUS_REG_MASK      0x88                        //����RTC(DS3231)��״̬�Ĵ����ļ������λ����Ҫ����λ��
#define DS3231_AGING_OFFSET_CONFIG  0x00                        //����RTC(DS3231)���ϻ������Ĵ���ֵ

//-----------------------------------------------------------------------------
#define RTC_RX8025                  0                           //RTC(RX8025)������ʱ��оƬѡ��
#define RTC_DS3231                  1                           //RTC(DS3231)������ʱ��оƬѡ��

#if HARDWARE_RTC == RTC_RX8025
#define HARDWARE_RTC_ADDR           RX8025_HEAD_ADDR            //ʱ��оƬ��ַָ��RTC(RX8025)
#else//HARDWARE_RTC == RTC_RX8025
#define HARDWARE_RTC_ADDR           DS3231_HEAD_ADDR            //ʱ��оƬ��ַָ��RTC(DS3231)
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

__packed typedef struct                                         //�����ʱ�䶼�Ƕ�����, ��BCD��ʽ��
{
    U8 Sec;
    U8 Min;
    U8 Hour;
    U8 Day;                                                     //�ܴ��������ڼ��������
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
__EXTERN U8 Sec_Sign;                                           //���ʱ�����־
__EXTERN volatile RTC_STRUCT Calendar;                          //�ڲ���ʱ��, �����ʱ�䶼�Ƕ�����, ��BCD��ʽ��

#endif//__RTC_H

/******************************************************************************
    End Of File
******************************************************************************/

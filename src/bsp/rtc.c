/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : rtc.c
 Description    : RTC操作相关函数。包括片内RTC、软件RTC和硬件RTC（RX8025、DS3231）
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2010-08-13    alan.l           the original version
******************************************************************************/

#include "..\inc\global_config.h"

//-------------------------------- 常数定义 -----------------------------------
const CALENDAR_MONTH Month_Tbl[] =          //MONTHS TABLE
{
    {0,  0},                                //Invalid month
    {31, 6},                                //January
    {28, 2},                                //February (note leap years are handled by code)
    {31, 2},                                //March
    {30, 5},                                //April
    {31, 0},                                //May
    {30, 3},                                //June
    {31, 5},                                //July
    {31, 1},                                //August
    {30, 4},                                //September
    {31, 6},                                //October
    {30, 2},                                //November
    {31, 4}                                 //December
};


//------------------------------ 私有变量声明 ---------------------------------

//------------------------------ 私有函数声明 ---------------------------------
static U8 calc_day_of_week (U8 *date);
//static S32 get_hardware_rtc(U8 *time);

static void set_software_rtc(U8 *time);
//static void get_software_rtc(U8 *time);

//onchip rtc
#ifdef ONCHIP_RTC
static void set_on_chip_rtc(U8 *time);
static S32 check_onchip_rtc_config(void);
static void on_chip_rtc_init(void);
static void get_on_chip_rtc(U8 *time);
#endif

#ifdef HARD_RTC
static S32 set_hardware_rtc(U8 *time);
static S32 check_hardware_rtc_config(void);
static void hardware_rtc_init(void);
static void reflash_sys_rtc(void);
#endif
/******************************************************************************
** 函数名称: void on_chip_rtc_init(void)
** 函数功能: 初始化片内实时时钟配置
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
#ifdef ONCHIP_RTC
void on_chip_rtc_init (void)
{
    RTCCCR = 0x10;                  //禁止时间计数器
    RTCILR = 0x03;                  //清除中断
    RTCCIIR = 0x0;                  //禁止增量中断
    RTCAMR = 0xff;                  //禁止报警
    RTC_AUX = 0x10;                 //清除振荡器停振的标志位
    RTC_AUXEN = 0x0;                //禁止振荡器停振中断

    RTCCCR = 0x11;                  //启动RTC
}
#endif
/******************************************************************************
** 函数名称: void get_on_chip_rtc(U8 *time)
** 函数功能: 读取片内实时时钟
** 入口参数: time: 返回时钟数据的缓冲区，7字节
** 返 回 值: 无
** 说    明: 返回的时钟数据为HEX格式，星期值是0~6
******************************************************************************/
#ifdef ONCHIP_RTC
void get_on_chip_rtc (U8 *time)
{
    U8 sec;

    time[0] = RTCSEC;                       //秒
    time[1] = RTCMIN;                       //分
    time[2] = RTCHOUR;                      //时
    time[3] = RTCDOW;                       //星期
    time[4] = RTCDOM;                       //日
    time[5] = RTCMONTH;                     //月
    time[6] = RTCYEAR;                      //年

    sec = RTCSEC;                           //再读一次秒
    if((time[0] != sec) && (sec == 0))      //如果2次读到的秒不一样且后一次的秒为'0'，时间进位了需要重读
    {
        time[0] = RTCSEC;                   //秒
        time[1] = RTCMIN;                   //分
        time[2] = RTCHOUR;                  //时
        time[3] = RTCDOW;                   //星期
        time[4] = RTCDOM;                   //日
        time[5] = RTCMONTH;                 //月
        time[6] = RTCYEAR;                  //年
    }
}
#endif
/******************************************************************************
** 函数名称: void set_on_chip_rtc(U8 *time)
** 函数功能: 设置片内实时时钟
** 入口参数: time: 需要写入内部寄存器的时钟数据缓冲区，7字节（HEX格式）
** 返 回 值: 无
** 说    明: 操作时禁止了时间计数器会造成一些延迟，星期值是0~6
******************************************************************************/
#ifdef ONCHIP_RTC
void set_on_chip_rtc (U8 *time)
{
    RTCCCR = 0x10;                          //禁止时间计数器

    RTCSEC = time[0];                       //秒
    RTCMIN = time[1];                       //分
    RTCHOUR = time[2];                      //时
    RTCDOW = time[3];                       //星期
    RTCDOM = time[4];                       //日
    RTCMONTH = time[5];                     //月
    RTCYEAR = time[6];                      //年

    RTCCCR = 0x11;                          //启动RTC
}
#endif
/******************************************************************************
** 函数名称: S32 check_onchip_rtc_config(void)
** 函数功能: 检查片内实时时钟的配置参数
** 入口参数: 无
** 返 回 值: __FALSE参数错误，__TRUE参数正确
** 说    明: 无
******************************************************************************/
#ifdef ONCHIP_RTC
S32 check_onchip_rtc_config (void)
{
    if(RTCCCR != 0x11)              //检查时钟控制寄存器
    {
        return __FALSE;
    }

    if(RTCILR != 0x00)              //检查中断位置寄存器
    {
        return __FALSE;
    }

    if(RTCCIIR != 0x00)             //检查计数器增量中断寄存器
    {
        return __FALSE;
    }

    if(RTCAMR != 0xff)              //检查报警屏蔽寄存器
    {
        return __FALSE;
    }

    if(RTC_AUX != 0x00)             //检查辅助控制寄存器
    {
        return __FALSE;
    }

    if(RTC_AUXEN != 0x00)           //检查辅助使能寄存器
    {
        return __FALSE;
    }

    return __TRUE;
}
#endif
/******************************************************************************
** 函数名称: GetDateTime_SoftRTC(PSTDATETIME pstDateTime)
** 函数功能: 读取软件实时时钟
** 入口参数: time: 返回时钟数据的缓冲区，6字节
** 返 回 值: 无
** 说    明: 返回的时钟数据为HEX格式
******************************************************************************/
void GetDateTime_SoftRTC(PSTDATETIME pstDateTime)
{
    pstDateTime->ucSecond = Calendar.Sec;                     //秒
    pstDateTime->ucMinute = Calendar.Min;                     //分
    pstDateTime->ucHour = Calendar.Hour;                    //时
    pstDateTime->ucDay = Calendar.Date;                    //日
    pstDateTime->ucMonth = Calendar.Month;                   //月
    pstDateTime->ucYear = Calendar.Year;                    //年
}
//azh 170908
U8 GetDateTime_Week(void)
{
    if(Calendar.Day==0)
    {
    	return 7;                    //周//azh 170830 376.1 周日：7
    }
    else
    {
    	return Calendar.Day;                    //周    	
    }		
}
/******************************************************************************
** 函数名称: void SetDateTime_SoftRTC(PSTDATETIME pstDateTime)
** 函数功能: 设置软件实时时钟
** 入口参数: time: 返回时钟数据的缓冲区，6字节
** 返 回 值: 无
** 说    明: 返回的时钟数据为HEX格式
******************************************************************************/
void SetDateTime_SoftRTC(PSTDATETIME pstDateTime)
{
    Calendar.Sec = pstDateTime->ucSecond;                     //秒
    Calendar.Min = pstDateTime->ucMinute;                     //分
    Calendar.Hour = pstDateTime->ucHour;                    //时
    Calendar.Date = pstDateTime->ucDay;                    //日
    Calendar.Month = pstDateTime->ucMonth;                   //月
    Calendar.Year = pstDateTime->ucYear;                    //年
}
/******************************************************************************
** 函数名称: void get_software_rtc(U8 *time)
** 函数功能: 读取软件实时时钟
** 入口参数: time: 返回时钟数据的缓冲区，7字节
** 返 回 值: 无
** 说    明: 返回的时钟数据为HEX格式，星期值是0~6
******************************************************************************/
void get_software_rtc (U8 *time)
{
//    STCTRL &= ~0x02;                            //禁止Tick中断

    time[0] = Calendar.Sec;                     //秒
    time[1] = Calendar.Min;                     //分
    time[2] = Calendar.Hour;                    //时
    time[3] = Calendar.Day;                     //星期
    time[4] = Calendar.Date;                    //日
    time[5] = Calendar.Month;                   //月
    time[6] = Calendar.Year;                    //年

//    STCTRL |= 0x02;                             //允许Tick中断
}
/******************************************************************************
** 函数名称: void set_software_rtc(U8 *time)
** 函数功能: 设置软件实时时钟
** 入口参数: time: 需要写入软件时钟的数据缓冲区，7字节（HEX格式）
** 返 回 值: 无
** 说    明: 星期值是0~6
******************************************************************************/
void set_software_rtc (U8 *time)
{
//    STCTRL &= ~0x02;                        //禁止Tick中断

    Calendar.Sec = time[0];                 //秒
    Calendar.Min = time[1];                 //分
    Calendar.Hour = time[2];                //时
    Calendar.Day = time[3];                 //星期
    Calendar.Date = time[4];                //日
    Calendar.Month = time[5];               //月
    Calendar.Year = time[6];                //年

//    STCTRL |= 0x02;                         //允许Tick中断
}

/******************************************************************************
** 函数名称: void hardware_rtc_init(void)
** 函数功能: 初始化硬件实时时钟配置
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
#ifdef HARD_RTC
void hardware_rtc_init (void)
{
    U8 i;
    U8 data[3];
    S32 res;

#if HARDWARE_RTC == RTC_RX8025
    data[0] = RX8025_EXT_REG_CONFIG;
    data[1] = RX8025_FLAG_REG_CONFIG;
    data[2] = RX8025_CTRL_REG_CONFIG;
    for(i = 0; i < 5; i++)              //失败时尝试5次
    {
        res = file_write(RX8025_EXT_REG_ADDR, data, 3);
        if(res > 0)                     //检查写入结果
        {
            break;                      //写入成功跳出循环
        }
    }
#else//HARDWARE_RTC == RTC_RX8025
    data[0] = DS3231_CTRL_REG_CONFIG;
    data[1] = DS3231_STATUS_REG_CONFIG;
    data[2] = DS3231_AGING_OFFSET_CONFIG;
    for(i = 0; i < 5; i++)              //失败时尝试5次
    {
        res = file_write(DS3231_CTRL_REG_ADDR, data, 3);
        if(res > 0)                     //检查写入结果
        {
            break;                      //写入成功跳出循环
        }
    }
#endif//HARDWARE_RTC == RTC_RX8025
}
#endif
/******************************************************************************
** 函数名称: S32 get_hardware_rtc(U8 *time)
** 函数功能: 读取硬件实时时钟
** 入口参数: time: 返回时钟数据的缓冲区，7字节（BCD格式）
** 返 回 值: 大于'0'读取成功，小于'0'读取失败
** 说    明: 返回的时钟数据为BCD格式，星期值是0~6
******************************************************************************/
#ifdef HARD_RTC
S32 get_hardware_rtc (U8 *time)
{
    U8 i;
    S32 res;

    for(i = 0; i < 5; i++)              //失败时尝试5次
    {
        res = file_read(HARDWARE_RTC_ADDR, time, 7);
        if(res > 0)                     //检查读取结果
        {
            break;                      //读取成功跳出循环
        }
    }
#if HARDWARE_RTC == RTC_RX8025
    for(i = 0; i < 5; i++)              //RX8025的星期值是按bit0~bit6表示的
    {
        if((time[3] & (1 << i)) != 0)   //从bit0->bit5查找是星期几
        {
            break;
        }
    }
    time[3] = i;
#else//HARDWARE_RTC == RTC_RX8025
    time[3]--;                          //DS3231的星期值是1~7，需要减'1'配合其他RTC
#endif//HARDWARE_RTC == RTC_RX8025

    return res;
}
#endif
/******************************************************************************
** 函数名称: S32 set_hardware_rtc(U8 *time)
** 函数功能: 设置硬件实时时钟
** 入口参数: time: 需要写入硬件时钟芯片的数据缓冲区，7字节（BCD格式）
** 返 回 值: 大于'0'设置成功，小于'0'设置失败
** 说    明: 星期值是0~6
******************************************************************************/
#ifdef HARD_RTC
S32 set_hardware_rtc (U8 *time)
{
    U8 i;
    U8 tmp[7];
    S32 res;

    memcpy(tmp, time, 7);
#if HARDWARE_RTC == RTC_RX8025
    tmp[3] = (1 << tmp[3]);             //RX8025的星期值是按bit0~bit6标示的
#else//HARDWARE_RTC == RTC_RX8025
    tmp[3]++;                           //DS3231的星期值是1~7，需要加'1'后设置
#endif//HARDWARE_RTC == RTC_RX8025

    for(i = 0; i < 5; i++)              //失败时尝试5次
    {
        res = file_write(HARDWARE_RTC_ADDR, tmp, 7);
        if(res > 0)                     //检查写入结果
        {
            break;                      //写入成功跳出循环
        }
    }

    return res;
}
#endif
/******************************************************************************
** 函数名称: S32 check_hardware_rtc_config(void)
** 函数功能: 检查硬件实时时钟芯片的配置参数
** 入口参数: 无
** 返 回 值: __FALSE参数错误，__TRUE参数正确
** 说    明: 无
******************************************************************************/
#ifdef HARD_RTC
S32 check_hardware_rtc_config (void)
{
    U8 data[3];
    S32 res;

#if HARDWARE_RTC == RTC_RX8025
    res = file_read(RX8025_EXT_REG_ADDR, data, 3);
    if(res > 0)                     //检查读取结果，如果读取错误则不检查
    {
        if(data[0] != RX8025_EXT_REG_CONFIG)
        {
            return __FALSE;
        }

        if((data[1] & RX8025_FLAG_REG_MASK) != RX8025_FLAG_REG_CONFIG)
        {
            return __FALSE;
        }

        if(data[2] != RX8025_CTRL_REG_CONFIG)
        {
            return __FALSE;
        }
    }
#else//HARDWARE_RTC == RTC_RX8025
    res = file_read(DS3231_CTRL_REG_ADDR, data, 3);
    if(res > 0)                     //检查读取结果，如果读取错误则不检查
    {
        if(data[0] != DS3231_CTRL_REG_CONFIG)
        {
            return __FALSE;
        }

        if((data[1] & DS3231_STATUS_REG_MASK) != DS3231_STATUS_REG_CONFIG)
        {
            return __FALSE;
        }

        if(data[2] != DS3231_AGING_OFFSET_CONFIG)
        {
            return __FALSE;
        }
    }
#endif//HARDWARE_RTC == RTC_RX8025

    return __TRUE;
}
#endif
/******************************************************************************
** 函数名称: void reflash_sys_rtc(void)
** 函数功能: 刷新系统实时时钟，用硬件时钟更新片内RTC和软件RTC
** 入口参数: 无
** 返 回 值: 无
** 说    明: 在上电时和每分钟循环调用，以硬件时钟为主时钟
**           如果硬件时钟数据合法则覆盖片内时钟和软件时钟
**           如果硬件时钟数据非法则继续检查片内时钟的数据合法性，记录错误日志
**           如果片内时钟数据合法则覆盖硬件时钟和软件时钟
**           如果片内时钟数据同样非法则继续使用硬件时钟（虽然是错的），记录错误日志
******************************************************************************/
#ifdef HARD_RTC
void reflash_sys_rtc (void)
{
    U8 rtc_tmp[7];

    if(get_hardware_rtc(rtc_tmp) > 0)                           //读取硬件时钟
    {
        if(__TRUE == check_bcd_time_valid(rtc_tmp))             //检查硬件时钟数据
        {
            convert_to_hex(rtc_tmp, 7);                         //转换为HEX格式
#ifdef ONCHIP_RTC
            set_on_chip_rtc(rtc_tmp);                           //用硬件时钟数据覆盖片内时钟
#endif
            set_software_rtc(rtc_tmp);                          //用硬件时钟数据覆盖软件时钟
        }
#ifdef ONCHIP_RTC
        else                                                    //硬件时钟数据非法，继续检查片内时钟
        {
            //记录错误日志...
            get_on_chip_rtc(rtc_tmp);                           //读取片内时钟
            if(__TRUE == check_hex_time_valid(rtc_tmp))         //检查片内时钟数据
            {
                set_software_rtc(rtc_tmp);                      //用片内时钟数据覆盖软件时钟
                convert_to_bcd(rtc_tmp, 7);                     //转换为BCD格式
                set_hardware_rtc(rtc_tmp);                      //用片内时钟数据覆盖硬件时钟
            }
#endif
            else                                                //片内时钟数据同样非法，只好使用硬件时钟（虽然是错的）
            {
                //记录错误日志...
                get_software_rtc(rtc_tmp);                      //读取软件时钟
                if(__TRUE == check_hex_time_valid(rtc_tmp))     //检查软件时钟数据
                {
#ifdef ONCHIP_RTC
                    set_on_chip_rtc(rtc_tmp);                   //用软件时钟数据覆盖片内时钟
#endif
                    convert_to_bcd(rtc_tmp, 7);                 //转换为BCD格式
                    set_hardware_rtc(rtc_tmp);                  //用软件时钟数据覆盖硬件时钟
                }
                else
                {
                    get_hardware_rtc(rtc_tmp);                  //仍然读取硬件时钟
                    convert_to_hex(rtc_tmp, 7);                 //转换为HEX格式
#ifdef ONCHIP_RTC
                    set_on_chip_rtc(rtc_tmp);                   //用硬件时钟数据覆盖片内时钟
#endif
                    set_software_rtc(rtc_tmp);                  //用硬件时钟数据覆盖软件时钟
                }
            }
        }
    }
    else
    {
        //记录错误日志...
    }
}
#endif
/******************************************************************************
** 函数名称: void sys_rtc_init(void)
** 函数功能: 初始化系统实时时钟以及配置，包括片内RTC、软件RTC和硬件RTC
** 入口参数: 无
** 返 回 值: 无
** 说    明: 在上电时调用
******************************************************************************/
#ifdef HARD_RTC
void sys_rtc_init (void)
{
    reflash_sys_rtc();                  //读取硬件时钟更新片内RTC、软件RTC
#ifdef ONCHIP_RTC
    on_chip_rtc_init();                 //初始化片内RTC配置
#endif
    hardware_rtc_init();                //初始化硬件RTC配置
}
#endif
/******************************************************************************
** 函数名称: void check_sys_rtc(void)
** 函数功能: 片内RTC、软件RTC更新，硬件时钟芯片配置参数检查//每分钟
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
#ifdef HARD_RTC
void check_sys_rtc (void)
{
    reflash_sys_rtc();                              //读取硬件时钟更新片内RTC、软件RTC

    if(__FALSE == check_hardware_rtc_config())      //检查硬件时钟的配置参数
    {
        hardware_rtc_init();                        //初始化硬件RTC配置
    }
#ifdef ONCHIP_RTC
    if(__FALSE == check_onchip_rtc_config())        //检查片内时钟的配置参数
    {
        on_chip_rtc_init();                         //初始化片内RTC配置
    }
#endif
}
#endif
/******************************************************************************
** 函数名称: U8 software_rtc_run(void)
** 函数功能: 软件时钟运行
** 入口参数: 无, 全局时间结构变量Calendar
** 返 回 值: 0-没有日期改变, 1-有日期改变
** 说    明: 在秒标志置位时调用, 仅作小时进位, 时间的改变是每秒钟在中断内实现的
**           直接改变Calendar
******************************************************************************/
U8 software_rtc_run (void)
{
    Calendar.Sec++;
    if(Calendar.Sec >= 60)                      //分钟变化
    {
        Calendar.Sec = 0;
        Calendar.Min++;
        if(Calendar.Min >=60)                   //小时变化
        {
            Calendar.Min = 0;
            Calendar.Hour++;
            if(Calendar.Hour >= 24)             //日变化
            {
                Calendar.Hour = 0;
                return 1;
            }
        }
    }
    return 0;
}

/******************************************************************************
** 函数名称: void update_date(void)
** 函数功能: 处理日期更新
** 入口参数: 无 全局时间结构变量Calendar
** 返 回 值: 无 直接改变Calendar
** 说    明: 无
******************************************************************************/
void update_date (void)
{
    U8 new_month;

    new_month = 1;
    if(Calendar.Date >= Month_Tbl[Calendar.Month].MonthDays)    //判断是否一个月的最后一天
    {
        if(Calendar.Month == 2)                                 //判断是否2月
        {
            if((Calendar.Year & 0x3) == 0)                      //判断是否闰年
            {
                if(Calendar.Date >= 29)                         //闰年的2月，天数是否大于29
                {
                    Calendar.Date = 1;                          //闰年的2月，天数大于29，日清零
                }
                else
                {
                    Calendar.Date++;                            //闰年的2月，但天数小于29，日加1
                    new_month = 0;
                }
            }
            else
            {
                Calendar.Date = 1;                              //不是闰年，日清零
            }
        }
        else
        {
            Calendar.Date = 1;                                  //不是2月，日清零
        }
    }
    else
    {
        Calendar.Date++;                                        //天数没有超过月最大天数，日加1
        new_month = 0;
    }

    if(new_month == 1)                                          //判断是否跨月
    {
        if(Calendar.Month >= 12)                                //判断是否跨年
        {
            Calendar.Month = 1;                                 //跨年，月清零
            Calendar.Year++;                                    //年加1
        }
        else
        {
            Calendar.Month++;                                   //没有跨年，月加1
        }
    }
    update_day_of_week();                                       //计算星期
}

/******************************************************************************
** 函数名称: void update_day_of_week(void)
** 函数功能: 按当前日期计算出星期, 星期值的范围是0-6
** 入口参数: 无 全局时间结构变量Calendar
** 返 回 值: 无 直接改变Calendar的星期值
** 说    明: 无
******************************************************************************/
void update_day_of_week (void)
{
    U8 day_of_week;
    U8 year;

    year = Calendar.Year;

    day_of_week = Calendar.Date + Month_Tbl[Calendar.Month].MonthVal;
    if(Calendar.Month < 3)
    {
        if((year & 0x3) == 0)
        {
            day_of_week--;
        }
    }
    day_of_week += year + (year / 4);
    day_of_week %= 7;
    Calendar.Day = day_of_week;
}

/******************************************************************************
** 函数名称: U8 calc_day_of_week(U8 *date)
** 函数功能: 按当前日期计算出星期，星期值的范围是0-6
** 入口参数: date: 日期数据，日+月+年，HEX格式
** 返 回 值: 星期值，取值0-6（0表示星期日）
** 说    明: 无
******************************************************************************/
U8 calc_day_of_week (U8 *date)
{
    U8 day_of_week;

    day_of_week = date[0] + Month_Tbl[date[1]].MonthVal;
    if(date[1] < 3)
    {
        if((date[2] & 0x3) == 0)
        {
            day_of_week--;
        }
    }
    day_of_week += date[2] + (date[2] / 4);
    day_of_week %= 7;
    return day_of_week;
}

/******************************************************************************
** 函数名称: S32 set_sys_time(U8 *time)
** 函数功能: 设置系统时间（秒+分+时），同时修改3个RTC的时间数据
** 入口参数: time: 待设置的时间数据，秒+分+时，BCD格式
** 返 回 值: 大于'0'设置成功，小于'0'设置失败
** 说    明: 无
******************************************************************************/
S32 set_sys_time (U8 *time)
{
    U8 i;
    U8 tmp[3];
    S32 res;

    for(i = 0; i < 3; i++)
    {
        if(((time[i] & 0x0f) > 9) || ((time[i] & 0xf0) > 0x90))
        {
            return __FAIL;
        }
    }
    if(time[0] > 0x59)                  //秒
    {
        return __FAIL;
    }
    if(time[1] > 0x59)                  //分
    {
        return __FAIL;
    }
    if(time[2] > 0x23)                  //时
    {
        return __FAIL;
    }

    memcpy(tmp, time, 3);
    for(i = 0; i < 5; i++)              //设置硬件RTC，失败时尝试5次
    {
        res = file_write(HARDWARE_RTC_ADDR, tmp, 3);
        if(res > 0)                     //检查写入结果
        {
            break;                      //写入成功跳出循环
        }
    }
    if(res < 0)
    {
        return res;                     //设置硬件RTC失败，返回错误
    }

    convert_to_hex(tmp, 3);             //转换为HEX格式

    RTCCCR = 0x10;                      //禁止时间计数器
    RTCSEC = tmp[0];                    //秒
    RTCMIN = tmp[1];                    //分
    RTCHOUR = tmp[2];                   //时
    RTCCCR = 0x11;                      //启动片内RTC

//    STCTRL &= ~0x02;                    //禁止Tick中断
    Calendar.Sec = tmp[0];              //秒
    Calendar.Min = tmp[1];              //分
    Calendar.Hour = tmp[2];             //时
//    STCTRL |= 0x02;                     //允许Tick中断

    return 1;
}

/******************************************************************************
** 函数名称: S32 set_sys_date(U8 *date)
** 函数功能: 设置系统日期（周+日+月+年），同时修改3个RTC的日期数据
** 入口参数: date: 待设置的日期数据，周+日+月+年，BCD格式
** 返 回 值: 大于'0'设置成功，小于'0'设置失败
** 说    明: 无
******************************************************************************/
S32 set_sys_date (U8 *date)
{
    U8 i, day;
    U8 tmp[4];
    S32 res;

    for(i = 0; i < 4; i++)
    {
        if(((date[i] & 0x0f) > 9) || ((date[i] & 0xf0) > 0x90))
        {
            return __FAIL;
        }
    }

    if(date[0] > 0x06)                          //周
    {
        return __FAIL;
    }
    if((date[1] == 0) || (date[1] > 0x31))      //日
    {
        return __FAIL;
    }
    if((date[2] == 0) || (date[2] > 0x12))      //月
    {
        return __FAIL;
    }
    if((date[3] < 0x10) || (date[3] > 0x50))    //年，在2010~2050之间
    {
        return __FAIL;
    }

    memcpy(tmp, date, 4);
    day = tmp[0];                       //保留一个'周'的值
#if HARDWARE_RTC == RTC_RX8025
    tmp[0] = (1 << tmp[0]);             //RX8025的星期值是按bit0~bit6标示的
#else//HARDWARE_RTC == RTC_RX8025
    tmp[0]++;                           //DS3231的星期值是1~7，需要加'1'后设置
#endif//HARDWARE_RTC == RTC_RX8025
    for(i = 0; i < 5; i++)              //设置硬件RTC，失败时尝试5次
    {
        res = file_write(HARDWARE_RTC_ADDR + 3, tmp, 4);
        if(res > 0)                     //检查写入结果
        {
            break;                      //写入成功跳出循环
        }
    }
    if(res < 0)
    {
        return res;                     //设置硬件RTC失败，返回错误
    }

    convert_to_hex(tmp + 1, 3);         //转换为HEX格式
    tmp[0] = day;                       //还原'周'

    RTCCCR = 0x10;                      //禁止时间计数器
    RTCDOW = tmp[0];                    //星期
    RTCDOM = tmp[1];                    //日
    RTCMONTH = tmp[2];                  //月
    RTCYEAR = tmp[3];                   //年
    RTCCCR = 0x11;                      //启动片内RTC

//    STCTRL &= ~0x02;                    //禁止Tick中断
    Calendar.Day = tmp[0];              //星期
    Calendar.Date = tmp[1];             //日
    Calendar.Month = tmp[2];            //月
    Calendar.Year = tmp[3];             //年
//    STCTRL |= 0x02;                     //允许Tick中断

    return 1;
}

/******************************************************************************
** 函数名称: S32 set_sys_calendar(U8 *calendar)
** 函数功能: 设置系统日日历（秒+分+时+周+日+月+年），同时修改3个RTC的日历数据
** 入口参数: calendar: 待设置的日历数据，秒+分+时+日+月+年，BCD格式
** 返 回 值: 大于'0'设置成功，小于'0'设置失败
** 说    明: calendar参数中没有'周'的数据，由函数按日期计算得到
******************************************************************************/
S32 set_sys_calendar (U8 *calendar)
{
    U8 tmp[7];

    memcpy(tmp, calendar, 3);               //拷贝：秒+分+时
    convert_to_hex(tmp, 3);                 //转换为HEX格式
    memcpy(tmp + 4, calendar + 3, 3);       //拷贝：日+月+年
    convert_to_hex(tmp + 4, 3);             //转换为HEX格式
    tmp[3] = calc_day_of_week(tmp + 4);     //计算出星期值

#ifdef ONCHIP_RTC							//芯片片上时钟
    set_on_chip_rtc(tmp);                   //设置片内RTC
#endif
    set_software_rtc(tmp);                  //设置软件RTC

#ifdef HARD_RTC						//配硬件时钟芯片
    memcpy(tmp, calendar, 3);               //拷贝：秒+分+时
    memcpy(tmp + 4, calendar + 3, 3);       //拷贝：日+月+年

    return (set_hardware_rtc(tmp));         //设置硬件RTC
#else
	return 1;
#endif
}

/******************************************************************************
** 函数名称: S32 get_sys_date(U8 *date)
** 函数功能: 读取系统日期（周+日+月+年）
** 入口参数: date: 返回读取的日期数据，周+日+月+年，BCD格式
** 返 回 值: 大于'0'读取成功，小于'0'读取失败
** 说    明: 返回的日期数据为BCD格式，星期值是0~6
******************************************************************************/
S32 get_sys_date (U8 *date)
{
    U8 i;
    S32 res;

    for(i = 0; i < 5; i++)              //失败时尝试5次
    {
        res = file_read(HARDWARE_RTC_ADDR + 3, date, 4);
        if(res > 0)                     //检查读取结果
        {
            break;                      //读取成功跳出循环
        }
    }
#if HARDWARE_RTC == RTC_RX8025
    for(i = 0; i < 6; i++)              //RX8025的星期值是按bit0~bit6表示的
    {
        if((date[0] & (1 << i)) != 0)   //从bit0->bit5查找是星期几
        {
            break;
        }
    }
    date[0] = i;
#else//HARDWARE_RTC == RTC_RX8025
    date[0]--;                          //DS3231的星期值是1~7，需要减'1'配合其他RTC
#endif//HARDWARE_RTC == RTC_RX8025

    return res;
}

/******************************************************************************
** 函数名称: S32 check_rtc_batt(void)
** 函数功能: 检查RTC电池电压，并刷新电表运行状态字
** 入口参数: 无
** 返 回 值: __SUCCEED电池电压正常，__FAIL电池欠压
** 说    明: 无
******************************************************************************/
S32 check_rtc_batt (void)
{
//
//    SET_PORT0_IN(P0_BAT_CHK);
//    if(PORT0_IS_HIGH(P0_BAT_CHK))       //检测电池电压信号输入
//    {
//        gMeter_State[0] &= ~(1 << 2);
        return __SUCCEED;
//    }
//    else
//{
//        gMeter_State[0] |= 1 << 2;
//        return __FAIL;
//}
}

/******************************************************************************
** 函数名称: U8 last_day_of_month(U8 year, U8 month)
** 函数功能: 计算月的最后一天，包括闰年的2月29日
** 入口参数: yaer : 年，BCD码
**           month: 月，BCD码
** 返 回 值: 天，HEX值
** 说    明: 如果无效的月则返回'0'
******************************************************************************/
U8 last_day_of_month (U8 year, U8 month)
{
    U8 day;

    month = bcd_to_hex(month);
    if(month > 12)
    {
        return 0;
    }

    day = Month_Tbl[month].MonthDays;
    if(month == 2)                          //2月
    {
        year = bcd_to_hex(year);
        if((year & 0x3) == 0)               //闰年
        {
            day++;                          //天数是29
        }
    }
    return day;
}
/******************************************************************************
** 函数名称: void get_dis_date_time(U32 di, U8 *buf, U8* len)
** 函数功能: 日期 时间要特殊处理 软件时钟用于显示
** 入口参数: di
**
** 返 回 值:
** 说    明:
******************************************************************************/
void get_dis_date_time (U32 di, U8 *buf, U8* len)
{
    if(di == 0x04000101)//日期 YYMMDDWW
    {
        buf[0] = Calendar.Day;
        buf[1] = HEX_TO_BCD(Calendar.Date, 1);
        buf[2] = HEX_TO_BCD(Calendar.Month, 1);
        buf[3] = HEX_TO_BCD(Calendar.Year, 1);
        *len = 4*2;//字符个数
    }
    else if(di == 0x04000102)//时间 hhmmss
    {
        buf[0] = HEX_TO_BCD(Calendar.Sec, 1);
        buf[1] = HEX_TO_BCD(Calendar.Min, 1);
        buf[2] = HEX_TO_BCD(Calendar.Hour,1);
        *len = 3*2;
    }
}
/******************************************************************************
** 函数名称: void SetDateTime_ToRTC(PSTDATETIME pstDateTime)
** 函数功能: 上行规约设置实时时钟（硬件时钟也会相应设置）
** 入口参数: pstDateTime时间结构 HEX
** 返 回 值: 无
** 说    明:
******************************************************************************/
#ifdef HARD_RTC
void SetDateTime_ToRTC(PSTDATETIME pstDateTime)
{
    U8 tmp[7];
//    Calendar.Sec = pstDateTime->ucSecond;                     //秒
//    Calendar.Min = pstDateTime->ucMinute;                     //分
//    Calendar.Hour = pstDateTime->ucHour;                    //时
//    Calendar.Date = pstDateTime->ucDay;                    //日
//    Calendar.Month = pstDateTime->ucMonth;                   //月
//    Calendar.Year = pstDateTime->ucYear;                    //年

    tmp[0] = pstDateTime->ucSecond;                     //秒
    tmp[1] = pstDateTime->ucMinute;                     //分
    tmp[2] = pstDateTime->ucHour;                    //时
    tmp[3] = 0;                                     //周 在设置之前会重新计算
    tmp[4] = pstDateTime->ucDay;                    //日
    tmp[5] = pstDateTime->ucMonth;                   //月
    tmp[6] = pstDateTime->ucYear;                    //年

    tmp[3] = calc_day_of_week(tmp + 4);     //计算出星期值

//    set_on_chip_rtc(tmp);                   //设置片内RTC
    set_software_rtc(tmp);                  //设置软件RTC

    convert_to_bcd(tmp, 7);                 //硬件是BCD格式的

    set_hardware_rtc(tmp);                  //设置硬件RTC
} 
#endif  
/******************************************************************************
    End Of File
******************************************************************************/

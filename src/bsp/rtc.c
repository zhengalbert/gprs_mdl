/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : rtc.c
 Description    : RTC������غ���������Ƭ��RTC�����RTC��Ӳ��RTC��RX8025��DS3231��
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��    1.00        2010-08-13    alan.l           the original version
******************************************************************************/

#include "..\inc\global_config.h"

//-------------------------------- �������� -----------------------------------
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


//------------------------------ ˽�б������� ---------------------------------

//------------------------------ ˽�к������� ---------------------------------
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
** ��������: void on_chip_rtc_init(void)
** ��������: ��ʼ��Ƭ��ʵʱʱ������
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
#ifdef ONCHIP_RTC
void on_chip_rtc_init (void)
{
    RTCCCR = 0x10;                  //��ֹʱ�������
    RTCILR = 0x03;                  //����ж�
    RTCCIIR = 0x0;                  //��ֹ�����ж�
    RTCAMR = 0xff;                  //��ֹ����
    RTC_AUX = 0x10;                 //�������ͣ��ı�־λ
    RTC_AUXEN = 0x0;                //��ֹ����ͣ���ж�

    RTCCCR = 0x11;                  //����RTC
}
#endif
/******************************************************************************
** ��������: void get_on_chip_rtc(U8 *time)
** ��������: ��ȡƬ��ʵʱʱ��
** ��ڲ���: time: ����ʱ�����ݵĻ�������7�ֽ�
** �� �� ֵ: ��
** ˵    ��: ���ص�ʱ������ΪHEX��ʽ������ֵ��0~6
******************************************************************************/
#ifdef ONCHIP_RTC
void get_on_chip_rtc (U8 *time)
{
    U8 sec;

    time[0] = RTCSEC;                       //��
    time[1] = RTCMIN;                       //��
    time[2] = RTCHOUR;                      //ʱ
    time[3] = RTCDOW;                       //����
    time[4] = RTCDOM;                       //��
    time[5] = RTCMONTH;                     //��
    time[6] = RTCYEAR;                      //��

    sec = RTCSEC;                           //�ٶ�һ����
    if((time[0] != sec) && (sec == 0))      //���2�ζ������벻һ���Һ�һ�ε���Ϊ'0'��ʱ���λ����Ҫ�ض�
    {
        time[0] = RTCSEC;                   //��
        time[1] = RTCMIN;                   //��
        time[2] = RTCHOUR;                  //ʱ
        time[3] = RTCDOW;                   //����
        time[4] = RTCDOM;                   //��
        time[5] = RTCMONTH;                 //��
        time[6] = RTCYEAR;                  //��
    }
}
#endif
/******************************************************************************
** ��������: void set_on_chip_rtc(U8 *time)
** ��������: ����Ƭ��ʵʱʱ��
** ��ڲ���: time: ��Ҫд���ڲ��Ĵ�����ʱ�����ݻ�������7�ֽڣ�HEX��ʽ��
** �� �� ֵ: ��
** ˵    ��: ����ʱ��ֹ��ʱ������������һЩ�ӳ٣�����ֵ��0~6
******************************************************************************/
#ifdef ONCHIP_RTC
void set_on_chip_rtc (U8 *time)
{
    RTCCCR = 0x10;                          //��ֹʱ�������

    RTCSEC = time[0];                       //��
    RTCMIN = time[1];                       //��
    RTCHOUR = time[2];                      //ʱ
    RTCDOW = time[3];                       //����
    RTCDOM = time[4];                       //��
    RTCMONTH = time[5];                     //��
    RTCYEAR = time[6];                      //��

    RTCCCR = 0x11;                          //����RTC
}
#endif
/******************************************************************************
** ��������: S32 check_onchip_rtc_config(void)
** ��������: ���Ƭ��ʵʱʱ�ӵ����ò���
** ��ڲ���: ��
** �� �� ֵ: __FALSE��������__TRUE������ȷ
** ˵    ��: ��
******************************************************************************/
#ifdef ONCHIP_RTC
S32 check_onchip_rtc_config (void)
{
    if(RTCCCR != 0x11)              //���ʱ�ӿ��ƼĴ���
    {
        return __FALSE;
    }

    if(RTCILR != 0x00)              //����ж�λ�üĴ���
    {
        return __FALSE;
    }

    if(RTCCIIR != 0x00)             //�������������жϼĴ���
    {
        return __FALSE;
    }

    if(RTCAMR != 0xff)              //��鱨�����μĴ���
    {
        return __FALSE;
    }

    if(RTC_AUX != 0x00)             //��鸨�����ƼĴ���
    {
        return __FALSE;
    }

    if(RTC_AUXEN != 0x00)           //��鸨��ʹ�ܼĴ���
    {
        return __FALSE;
    }

    return __TRUE;
}
#endif
/******************************************************************************
** ��������: GetDateTime_SoftRTC(PSTDATETIME pstDateTime)
** ��������: ��ȡ���ʵʱʱ��
** ��ڲ���: time: ����ʱ�����ݵĻ�������6�ֽ�
** �� �� ֵ: ��
** ˵    ��: ���ص�ʱ������ΪHEX��ʽ
******************************************************************************/
void GetDateTime_SoftRTC(PSTDATETIME pstDateTime)
{
    pstDateTime->ucSecond = Calendar.Sec;                     //��
    pstDateTime->ucMinute = Calendar.Min;                     //��
    pstDateTime->ucHour = Calendar.Hour;                    //ʱ
    pstDateTime->ucDay = Calendar.Date;                    //��
    pstDateTime->ucMonth = Calendar.Month;                   //��
    pstDateTime->ucYear = Calendar.Year;                    //��
}
//azh 170908
U8 GetDateTime_Week(void)
{
    if(Calendar.Day==0)
    {
    	return 7;                    //��//azh 170830 376.1 ���գ�7
    }
    else
    {
    	return Calendar.Day;                    //��    	
    }		
}
/******************************************************************************
** ��������: void SetDateTime_SoftRTC(PSTDATETIME pstDateTime)
** ��������: �������ʵʱʱ��
** ��ڲ���: time: ����ʱ�����ݵĻ�������6�ֽ�
** �� �� ֵ: ��
** ˵    ��: ���ص�ʱ������ΪHEX��ʽ
******************************************************************************/
void SetDateTime_SoftRTC(PSTDATETIME pstDateTime)
{
    Calendar.Sec = pstDateTime->ucSecond;                     //��
    Calendar.Min = pstDateTime->ucMinute;                     //��
    Calendar.Hour = pstDateTime->ucHour;                    //ʱ
    Calendar.Date = pstDateTime->ucDay;                    //��
    Calendar.Month = pstDateTime->ucMonth;                   //��
    Calendar.Year = pstDateTime->ucYear;                    //��
}
/******************************************************************************
** ��������: void get_software_rtc(U8 *time)
** ��������: ��ȡ���ʵʱʱ��
** ��ڲ���: time: ����ʱ�����ݵĻ�������7�ֽ�
** �� �� ֵ: ��
** ˵    ��: ���ص�ʱ������ΪHEX��ʽ������ֵ��0~6
******************************************************************************/
void get_software_rtc (U8 *time)
{
//    STCTRL &= ~0x02;                            //��ֹTick�ж�

    time[0] = Calendar.Sec;                     //��
    time[1] = Calendar.Min;                     //��
    time[2] = Calendar.Hour;                    //ʱ
    time[3] = Calendar.Day;                     //����
    time[4] = Calendar.Date;                    //��
    time[5] = Calendar.Month;                   //��
    time[6] = Calendar.Year;                    //��

//    STCTRL |= 0x02;                             //����Tick�ж�
}
/******************************************************************************
** ��������: void set_software_rtc(U8 *time)
** ��������: �������ʵʱʱ��
** ��ڲ���: time: ��Ҫд�����ʱ�ӵ����ݻ�������7�ֽڣ�HEX��ʽ��
** �� �� ֵ: ��
** ˵    ��: ����ֵ��0~6
******************************************************************************/
void set_software_rtc (U8 *time)
{
//    STCTRL &= ~0x02;                        //��ֹTick�ж�

    Calendar.Sec = time[0];                 //��
    Calendar.Min = time[1];                 //��
    Calendar.Hour = time[2];                //ʱ
    Calendar.Day = time[3];                 //����
    Calendar.Date = time[4];                //��
    Calendar.Month = time[5];               //��
    Calendar.Year = time[6];                //��

//    STCTRL |= 0x02;                         //����Tick�ж�
}

/******************************************************************************
** ��������: void hardware_rtc_init(void)
** ��������: ��ʼ��Ӳ��ʵʱʱ������
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
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
    for(i = 0; i < 5; i++)              //ʧ��ʱ����5��
    {
        res = file_write(RX8025_EXT_REG_ADDR, data, 3);
        if(res > 0)                     //���д����
        {
            break;                      //д��ɹ�����ѭ��
        }
    }
#else//HARDWARE_RTC == RTC_RX8025
    data[0] = DS3231_CTRL_REG_CONFIG;
    data[1] = DS3231_STATUS_REG_CONFIG;
    data[2] = DS3231_AGING_OFFSET_CONFIG;
    for(i = 0; i < 5; i++)              //ʧ��ʱ����5��
    {
        res = file_write(DS3231_CTRL_REG_ADDR, data, 3);
        if(res > 0)                     //���д����
        {
            break;                      //д��ɹ�����ѭ��
        }
    }
#endif//HARDWARE_RTC == RTC_RX8025
}
#endif
/******************************************************************************
** ��������: S32 get_hardware_rtc(U8 *time)
** ��������: ��ȡӲ��ʵʱʱ��
** ��ڲ���: time: ����ʱ�����ݵĻ�������7�ֽڣ�BCD��ʽ��
** �� �� ֵ: ����'0'��ȡ�ɹ���С��'0'��ȡʧ��
** ˵    ��: ���ص�ʱ������ΪBCD��ʽ������ֵ��0~6
******************************************************************************/
#ifdef HARD_RTC
S32 get_hardware_rtc (U8 *time)
{
    U8 i;
    S32 res;

    for(i = 0; i < 5; i++)              //ʧ��ʱ����5��
    {
        res = file_read(HARDWARE_RTC_ADDR, time, 7);
        if(res > 0)                     //����ȡ���
        {
            break;                      //��ȡ�ɹ�����ѭ��
        }
    }
#if HARDWARE_RTC == RTC_RX8025
    for(i = 0; i < 5; i++)              //RX8025������ֵ�ǰ�bit0~bit6��ʾ��
    {
        if((time[3] & (1 << i)) != 0)   //��bit0->bit5���������ڼ�
        {
            break;
        }
    }
    time[3] = i;
#else//HARDWARE_RTC == RTC_RX8025
    time[3]--;                          //DS3231������ֵ��1~7����Ҫ��'1'�������RTC
#endif//HARDWARE_RTC == RTC_RX8025

    return res;
}
#endif
/******************************************************************************
** ��������: S32 set_hardware_rtc(U8 *time)
** ��������: ����Ӳ��ʵʱʱ��
** ��ڲ���: time: ��Ҫд��Ӳ��ʱ��оƬ�����ݻ�������7�ֽڣ�BCD��ʽ��
** �� �� ֵ: ����'0'���óɹ���С��'0'����ʧ��
** ˵    ��: ����ֵ��0~6
******************************************************************************/
#ifdef HARD_RTC
S32 set_hardware_rtc (U8 *time)
{
    U8 i;
    U8 tmp[7];
    S32 res;

    memcpy(tmp, time, 7);
#if HARDWARE_RTC == RTC_RX8025
    tmp[3] = (1 << tmp[3]);             //RX8025������ֵ�ǰ�bit0~bit6��ʾ��
#else//HARDWARE_RTC == RTC_RX8025
    tmp[3]++;                           //DS3231������ֵ��1~7����Ҫ��'1'������
#endif//HARDWARE_RTC == RTC_RX8025

    for(i = 0; i < 5; i++)              //ʧ��ʱ����5��
    {
        res = file_write(HARDWARE_RTC_ADDR, tmp, 7);
        if(res > 0)                     //���д����
        {
            break;                      //д��ɹ�����ѭ��
        }
    }

    return res;
}
#endif
/******************************************************************************
** ��������: S32 check_hardware_rtc_config(void)
** ��������: ���Ӳ��ʵʱʱ��оƬ�����ò���
** ��ڲ���: ��
** �� �� ֵ: __FALSE��������__TRUE������ȷ
** ˵    ��: ��
******************************************************************************/
#ifdef HARD_RTC
S32 check_hardware_rtc_config (void)
{
    U8 data[3];
    S32 res;

#if HARDWARE_RTC == RTC_RX8025
    res = file_read(RX8025_EXT_REG_ADDR, data, 3);
    if(res > 0)                     //����ȡ����������ȡ�����򲻼��
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
    if(res > 0)                     //����ȡ����������ȡ�����򲻼��
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
** ��������: void reflash_sys_rtc(void)
** ��������: ˢ��ϵͳʵʱʱ�ӣ���Ӳ��ʱ�Ӹ���Ƭ��RTC�����RTC
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ���ϵ�ʱ��ÿ����ѭ�����ã���Ӳ��ʱ��Ϊ��ʱ��
**           ���Ӳ��ʱ�����ݺϷ��򸲸�Ƭ��ʱ�Ӻ����ʱ��
**           ���Ӳ��ʱ�����ݷǷ���������Ƭ��ʱ�ӵ����ݺϷ��ԣ���¼������־
**           ���Ƭ��ʱ�����ݺϷ��򸲸�Ӳ��ʱ�Ӻ����ʱ��
**           ���Ƭ��ʱ������ͬ���Ƿ������ʹ��Ӳ��ʱ�ӣ���Ȼ�Ǵ�ģ�����¼������־
******************************************************************************/
#ifdef HARD_RTC
void reflash_sys_rtc (void)
{
    U8 rtc_tmp[7];

    if(get_hardware_rtc(rtc_tmp) > 0)                           //��ȡӲ��ʱ��
    {
        if(__TRUE == check_bcd_time_valid(rtc_tmp))             //���Ӳ��ʱ������
        {
            convert_to_hex(rtc_tmp, 7);                         //ת��ΪHEX��ʽ
#ifdef ONCHIP_RTC
            set_on_chip_rtc(rtc_tmp);                           //��Ӳ��ʱ�����ݸ���Ƭ��ʱ��
#endif
            set_software_rtc(rtc_tmp);                          //��Ӳ��ʱ�����ݸ������ʱ��
        }
#ifdef ONCHIP_RTC
        else                                                    //Ӳ��ʱ�����ݷǷ����������Ƭ��ʱ��
        {
            //��¼������־...
            get_on_chip_rtc(rtc_tmp);                           //��ȡƬ��ʱ��
            if(__TRUE == check_hex_time_valid(rtc_tmp))         //���Ƭ��ʱ������
            {
                set_software_rtc(rtc_tmp);                      //��Ƭ��ʱ�����ݸ������ʱ��
                convert_to_bcd(rtc_tmp, 7);                     //ת��ΪBCD��ʽ
                set_hardware_rtc(rtc_tmp);                      //��Ƭ��ʱ�����ݸ���Ӳ��ʱ��
            }
#endif
            else                                                //Ƭ��ʱ������ͬ���Ƿ���ֻ��ʹ��Ӳ��ʱ�ӣ���Ȼ�Ǵ�ģ�
            {
                //��¼������־...
                get_software_rtc(rtc_tmp);                      //��ȡ���ʱ��
                if(__TRUE == check_hex_time_valid(rtc_tmp))     //������ʱ������
                {
#ifdef ONCHIP_RTC
                    set_on_chip_rtc(rtc_tmp);                   //�����ʱ�����ݸ���Ƭ��ʱ��
#endif
                    convert_to_bcd(rtc_tmp, 7);                 //ת��ΪBCD��ʽ
                    set_hardware_rtc(rtc_tmp);                  //�����ʱ�����ݸ���Ӳ��ʱ��
                }
                else
                {
                    get_hardware_rtc(rtc_tmp);                  //��Ȼ��ȡӲ��ʱ��
                    convert_to_hex(rtc_tmp, 7);                 //ת��ΪHEX��ʽ
#ifdef ONCHIP_RTC
                    set_on_chip_rtc(rtc_tmp);                   //��Ӳ��ʱ�����ݸ���Ƭ��ʱ��
#endif
                    set_software_rtc(rtc_tmp);                  //��Ӳ��ʱ�����ݸ������ʱ��
                }
            }
        }
    }
    else
    {
        //��¼������־...
    }
}
#endif
/******************************************************************************
** ��������: void sys_rtc_init(void)
** ��������: ��ʼ��ϵͳʵʱʱ���Լ����ã�����Ƭ��RTC�����RTC��Ӳ��RTC
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ���ϵ�ʱ����
******************************************************************************/
#ifdef HARD_RTC
void sys_rtc_init (void)
{
    reflash_sys_rtc();                  //��ȡӲ��ʱ�Ӹ���Ƭ��RTC�����RTC
#ifdef ONCHIP_RTC
    on_chip_rtc_init();                 //��ʼ��Ƭ��RTC����
#endif
    hardware_rtc_init();                //��ʼ��Ӳ��RTC����
}
#endif
/******************************************************************************
** ��������: void check_sys_rtc(void)
** ��������: Ƭ��RTC�����RTC���£�Ӳ��ʱ��оƬ���ò������//ÿ����
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
#ifdef HARD_RTC
void check_sys_rtc (void)
{
    reflash_sys_rtc();                              //��ȡӲ��ʱ�Ӹ���Ƭ��RTC�����RTC

    if(__FALSE == check_hardware_rtc_config())      //���Ӳ��ʱ�ӵ����ò���
    {
        hardware_rtc_init();                        //��ʼ��Ӳ��RTC����
    }
#ifdef ONCHIP_RTC
    if(__FALSE == check_onchip_rtc_config())        //���Ƭ��ʱ�ӵ����ò���
    {
        on_chip_rtc_init();                         //��ʼ��Ƭ��RTC����
    }
#endif
}
#endif
/******************************************************************************
** ��������: U8 software_rtc_run(void)
** ��������: ���ʱ������
** ��ڲ���: ��, ȫ��ʱ��ṹ����Calendar
** �� �� ֵ: 0-û�����ڸı�, 1-�����ڸı�
** ˵    ��: �����־��λʱ����, ����Сʱ��λ, ʱ��ĸı���ÿ�������ж���ʵ�ֵ�
**           ֱ�Ӹı�Calendar
******************************************************************************/
U8 software_rtc_run (void)
{
    Calendar.Sec++;
    if(Calendar.Sec >= 60)                      //���ӱ仯
    {
        Calendar.Sec = 0;
        Calendar.Min++;
        if(Calendar.Min >=60)                   //Сʱ�仯
        {
            Calendar.Min = 0;
            Calendar.Hour++;
            if(Calendar.Hour >= 24)             //�ձ仯
            {
                Calendar.Hour = 0;
                return 1;
            }
        }
    }
    return 0;
}

/******************************************************************************
** ��������: void update_date(void)
** ��������: �������ڸ���
** ��ڲ���: �� ȫ��ʱ��ṹ����Calendar
** �� �� ֵ: �� ֱ�Ӹı�Calendar
** ˵    ��: ��
******************************************************************************/
void update_date (void)
{
    U8 new_month;

    new_month = 1;
    if(Calendar.Date >= Month_Tbl[Calendar.Month].MonthDays)    //�ж��Ƿ�һ���µ����һ��
    {
        if(Calendar.Month == 2)                                 //�ж��Ƿ�2��
        {
            if((Calendar.Year & 0x3) == 0)                      //�ж��Ƿ�����
            {
                if(Calendar.Date >= 29)                         //�����2�£������Ƿ����29
                {
                    Calendar.Date = 1;                          //�����2�£���������29��������
                }
                else
                {
                    Calendar.Date++;                            //�����2�£�������С��29���ռ�1
                    new_month = 0;
                }
            }
            else
            {
                Calendar.Date = 1;                              //�������꣬������
            }
        }
        else
        {
            Calendar.Date = 1;                                  //����2�£�������
        }
    }
    else
    {
        Calendar.Date++;                                        //����û�г���������������ռ�1
        new_month = 0;
    }

    if(new_month == 1)                                          //�ж��Ƿ����
    {
        if(Calendar.Month >= 12)                                //�ж��Ƿ����
        {
            Calendar.Month = 1;                                 //���꣬������
            Calendar.Year++;                                    //���1
        }
        else
        {
            Calendar.Month++;                                   //û�п��꣬�¼�1
        }
    }
    update_day_of_week();                                       //��������
}

/******************************************************************************
** ��������: void update_day_of_week(void)
** ��������: ����ǰ���ڼ��������, ����ֵ�ķ�Χ��0-6
** ��ڲ���: �� ȫ��ʱ��ṹ����Calendar
** �� �� ֵ: �� ֱ�Ӹı�Calendar������ֵ
** ˵    ��: ��
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
** ��������: U8 calc_day_of_week(U8 *date)
** ��������: ����ǰ���ڼ�������ڣ�����ֵ�ķ�Χ��0-6
** ��ڲ���: date: �������ݣ���+��+�꣬HEX��ʽ
** �� �� ֵ: ����ֵ��ȡֵ0-6��0��ʾ�����գ�
** ˵    ��: ��
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
** ��������: S32 set_sys_time(U8 *time)
** ��������: ����ϵͳʱ�䣨��+��+ʱ����ͬʱ�޸�3��RTC��ʱ������
** ��ڲ���: time: �����õ�ʱ�����ݣ���+��+ʱ��BCD��ʽ
** �� �� ֵ: ����'0'���óɹ���С��'0'����ʧ��
** ˵    ��: ��
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
    if(time[0] > 0x59)                  //��
    {
        return __FAIL;
    }
    if(time[1] > 0x59)                  //��
    {
        return __FAIL;
    }
    if(time[2] > 0x23)                  //ʱ
    {
        return __FAIL;
    }

    memcpy(tmp, time, 3);
    for(i = 0; i < 5; i++)              //����Ӳ��RTC��ʧ��ʱ����5��
    {
        res = file_write(HARDWARE_RTC_ADDR, tmp, 3);
        if(res > 0)                     //���д����
        {
            break;                      //д��ɹ�����ѭ��
        }
    }
    if(res < 0)
    {
        return res;                     //����Ӳ��RTCʧ�ܣ����ش���
    }

    convert_to_hex(tmp, 3);             //ת��ΪHEX��ʽ

    RTCCCR = 0x10;                      //��ֹʱ�������
    RTCSEC = tmp[0];                    //��
    RTCMIN = tmp[1];                    //��
    RTCHOUR = tmp[2];                   //ʱ
    RTCCCR = 0x11;                      //����Ƭ��RTC

//    STCTRL &= ~0x02;                    //��ֹTick�ж�
    Calendar.Sec = tmp[0];              //��
    Calendar.Min = tmp[1];              //��
    Calendar.Hour = tmp[2];             //ʱ
//    STCTRL |= 0x02;                     //����Tick�ж�

    return 1;
}

/******************************************************************************
** ��������: S32 set_sys_date(U8 *date)
** ��������: ����ϵͳ���ڣ���+��+��+�꣩��ͬʱ�޸�3��RTC����������
** ��ڲ���: date: �����õ��������ݣ���+��+��+�꣬BCD��ʽ
** �� �� ֵ: ����'0'���óɹ���С��'0'����ʧ��
** ˵    ��: ��
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

    if(date[0] > 0x06)                          //��
    {
        return __FAIL;
    }
    if((date[1] == 0) || (date[1] > 0x31))      //��
    {
        return __FAIL;
    }
    if((date[2] == 0) || (date[2] > 0x12))      //��
    {
        return __FAIL;
    }
    if((date[3] < 0x10) || (date[3] > 0x50))    //�꣬��2010~2050֮��
    {
        return __FAIL;
    }

    memcpy(tmp, date, 4);
    day = tmp[0];                       //����һ��'��'��ֵ
#if HARDWARE_RTC == RTC_RX8025
    tmp[0] = (1 << tmp[0]);             //RX8025������ֵ�ǰ�bit0~bit6��ʾ��
#else//HARDWARE_RTC == RTC_RX8025
    tmp[0]++;                           //DS3231������ֵ��1~7����Ҫ��'1'������
#endif//HARDWARE_RTC == RTC_RX8025
    for(i = 0; i < 5; i++)              //����Ӳ��RTC��ʧ��ʱ����5��
    {
        res = file_write(HARDWARE_RTC_ADDR + 3, tmp, 4);
        if(res > 0)                     //���д����
        {
            break;                      //д��ɹ�����ѭ��
        }
    }
    if(res < 0)
    {
        return res;                     //����Ӳ��RTCʧ�ܣ����ش���
    }

    convert_to_hex(tmp + 1, 3);         //ת��ΪHEX��ʽ
    tmp[0] = day;                       //��ԭ'��'

    RTCCCR = 0x10;                      //��ֹʱ�������
    RTCDOW = tmp[0];                    //����
    RTCDOM = tmp[1];                    //��
    RTCMONTH = tmp[2];                  //��
    RTCYEAR = tmp[3];                   //��
    RTCCCR = 0x11;                      //����Ƭ��RTC

//    STCTRL &= ~0x02;                    //��ֹTick�ж�
    Calendar.Day = tmp[0];              //����
    Calendar.Date = tmp[1];             //��
    Calendar.Month = tmp[2];            //��
    Calendar.Year = tmp[3];             //��
//    STCTRL |= 0x02;                     //����Tick�ж�

    return 1;
}

/******************************************************************************
** ��������: S32 set_sys_calendar(U8 *calendar)
** ��������: ����ϵͳ����������+��+ʱ+��+��+��+�꣩��ͬʱ�޸�3��RTC����������
** ��ڲ���: calendar: �����õ��������ݣ���+��+ʱ+��+��+�꣬BCD��ʽ
** �� �� ֵ: ����'0'���óɹ���С��'0'����ʧ��
** ˵    ��: calendar������û��'��'�����ݣ��ɺ��������ڼ���õ�
******************************************************************************/
S32 set_sys_calendar (U8 *calendar)
{
    U8 tmp[7];

    memcpy(tmp, calendar, 3);               //��������+��+ʱ
    convert_to_hex(tmp, 3);                 //ת��ΪHEX��ʽ
    memcpy(tmp + 4, calendar + 3, 3);       //��������+��+��
    convert_to_hex(tmp + 4, 3);             //ת��ΪHEX��ʽ
    tmp[3] = calc_day_of_week(tmp + 4);     //���������ֵ

#ifdef ONCHIP_RTC							//оƬƬ��ʱ��
    set_on_chip_rtc(tmp);                   //����Ƭ��RTC
#endif
    set_software_rtc(tmp);                  //�������RTC

#ifdef HARD_RTC						//��Ӳ��ʱ��оƬ
    memcpy(tmp, calendar, 3);               //��������+��+ʱ
    memcpy(tmp + 4, calendar + 3, 3);       //��������+��+��

    return (set_hardware_rtc(tmp));         //����Ӳ��RTC
#else
	return 1;
#endif
}

/******************************************************************************
** ��������: S32 get_sys_date(U8 *date)
** ��������: ��ȡϵͳ���ڣ���+��+��+�꣩
** ��ڲ���: date: ���ض�ȡ���������ݣ���+��+��+�꣬BCD��ʽ
** �� �� ֵ: ����'0'��ȡ�ɹ���С��'0'��ȡʧ��
** ˵    ��: ���ص���������ΪBCD��ʽ������ֵ��0~6
******************************************************************************/
S32 get_sys_date (U8 *date)
{
    U8 i;
    S32 res;

    for(i = 0; i < 5; i++)              //ʧ��ʱ����5��
    {
        res = file_read(HARDWARE_RTC_ADDR + 3, date, 4);
        if(res > 0)                     //����ȡ���
        {
            break;                      //��ȡ�ɹ�����ѭ��
        }
    }
#if HARDWARE_RTC == RTC_RX8025
    for(i = 0; i < 6; i++)              //RX8025������ֵ�ǰ�bit0~bit6��ʾ��
    {
        if((date[0] & (1 << i)) != 0)   //��bit0->bit5���������ڼ�
        {
            break;
        }
    }
    date[0] = i;
#else//HARDWARE_RTC == RTC_RX8025
    date[0]--;                          //DS3231������ֵ��1~7����Ҫ��'1'�������RTC
#endif//HARDWARE_RTC == RTC_RX8025

    return res;
}

/******************************************************************************
** ��������: S32 check_rtc_batt(void)
** ��������: ���RTC��ص�ѹ����ˢ�µ������״̬��
** ��ڲ���: ��
** �� �� ֵ: __SUCCEED��ص�ѹ������__FAIL���Ƿѹ
** ˵    ��: ��
******************************************************************************/
S32 check_rtc_batt (void)
{
//
//    SET_PORT0_IN(P0_BAT_CHK);
//    if(PORT0_IS_HIGH(P0_BAT_CHK))       //����ص�ѹ�ź�����
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
** ��������: U8 last_day_of_month(U8 year, U8 month)
** ��������: �����µ����һ�죬���������2��29��
** ��ڲ���: yaer : �꣬BCD��
**           month: �£�BCD��
** �� �� ֵ: �죬HEXֵ
** ˵    ��: �����Ч�����򷵻�'0'
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
    if(month == 2)                          //2��
    {
        year = bcd_to_hex(year);
        if((year & 0x3) == 0)               //����
        {
            day++;                          //������29
        }
    }
    return day;
}
/******************************************************************************
** ��������: void get_dis_date_time(U32 di, U8 *buf, U8* len)
** ��������: ���� ʱ��Ҫ���⴦�� ���ʱ��������ʾ
** ��ڲ���: di
**
** �� �� ֵ:
** ˵    ��:
******************************************************************************/
void get_dis_date_time (U32 di, U8 *buf, U8* len)
{
    if(di == 0x04000101)//���� YYMMDDWW
    {
        buf[0] = Calendar.Day;
        buf[1] = HEX_TO_BCD(Calendar.Date, 1);
        buf[2] = HEX_TO_BCD(Calendar.Month, 1);
        buf[3] = HEX_TO_BCD(Calendar.Year, 1);
        *len = 4*2;//�ַ�����
    }
    else if(di == 0x04000102)//ʱ�� hhmmss
    {
        buf[0] = HEX_TO_BCD(Calendar.Sec, 1);
        buf[1] = HEX_TO_BCD(Calendar.Min, 1);
        buf[2] = HEX_TO_BCD(Calendar.Hour,1);
        *len = 3*2;
    }
}
/******************************************************************************
** ��������: void SetDateTime_ToRTC(PSTDATETIME pstDateTime)
** ��������: ���й�Լ����ʵʱʱ�ӣ�Ӳ��ʱ��Ҳ����Ӧ���ã�
** ��ڲ���: pstDateTimeʱ��ṹ HEX
** �� �� ֵ: ��
** ˵    ��:
******************************************************************************/
#ifdef HARD_RTC
void SetDateTime_ToRTC(PSTDATETIME pstDateTime)
{
    U8 tmp[7];
//    Calendar.Sec = pstDateTime->ucSecond;                     //��
//    Calendar.Min = pstDateTime->ucMinute;                     //��
//    Calendar.Hour = pstDateTime->ucHour;                    //ʱ
//    Calendar.Date = pstDateTime->ucDay;                    //��
//    Calendar.Month = pstDateTime->ucMonth;                   //��
//    Calendar.Year = pstDateTime->ucYear;                    //��

    tmp[0] = pstDateTime->ucSecond;                     //��
    tmp[1] = pstDateTime->ucMinute;                     //��
    tmp[2] = pstDateTime->ucHour;                    //ʱ
    tmp[3] = 0;                                     //�� ������֮ǰ�����¼���
    tmp[4] = pstDateTime->ucDay;                    //��
    tmp[5] = pstDateTime->ucMonth;                   //��
    tmp[6] = pstDateTime->ucYear;                    //��

    tmp[3] = calc_day_of_week(tmp + 4);     //���������ֵ

//    set_on_chip_rtc(tmp);                   //����Ƭ��RTC
    set_software_rtc(tmp);                  //�������RTC

    convert_to_bcd(tmp, 7);                 //Ӳ����BCD��ʽ��

    set_hardware_rtc(tmp);                  //����Ӳ��RTC
} 
#endif  
/******************************************************************************
    End Of File
******************************************************************************/

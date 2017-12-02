/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : misc.c
 Description    : 一些杂散的通用函数
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   3、    2.00        2011-02-25     rosoon          合并使A&C共享
   2、    1.01        2010-09-07      snap           增加check_event_time_valid()函数避免与check_bcd_time_valid()函数冲突
   1、    1.00        2010-07-22      snap           the original version
******************************************************************************/

#include "..\inc\global_config.h"
//U8 Irda_Buf[100];

//U8 *gdebug1_memptr;

//#define DEBUG_MEM   1

//-------------------------------- 常数定义 -----------------------------------
const U16 month_lengths[2][13] = {
    {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
    {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}
};
const U16 year_lengths[2] = {365, 366};

static const U16 crc_tbl[256] =      //CRC 0x1189余式表
{
    0x0000, 0x1189, 0x2312, 0x329B, 0x4624, 0x57AD, 0x6536, 0x74BF,
    0x8C48, 0x9DC1, 0xAF5A, 0xBED3, 0xCA6C, 0xDBE5, 0xE97E, 0xF8F7,
    0x1081, 0x0108, 0x3393, 0x221A, 0x56A5, 0x472C, 0x75B7, 0x643E,
    0x9CC9, 0x8D40, 0xBFDB, 0xAE52, 0xDAED, 0xCB64, 0xF9FF, 0xE876,
    0x2102, 0x308B, 0x0210, 0x1399, 0x6726, 0x76AF, 0x4434, 0x55BD,
    0xAD4A, 0xBCC3, 0x8E58, 0x9FD1, 0xEB6E, 0xFAE7, 0xC87C, 0xD9F5,
    0x3183, 0x200A, 0x1291, 0x0318, 0x77A7, 0x662E, 0x54B5, 0x453C,
    0xBDCB, 0xAC42, 0x9ED9, 0x8F50, 0xFBEF, 0xEA66, 0xD8FD, 0xC974,
    0x4204, 0x538D, 0x6116, 0x709F, 0x0420, 0x15A9, 0x2732, 0x36BB,
    0xCE4C, 0xDFC5, 0xED5E, 0xFCD7, 0x8868, 0x99E1, 0xAB7A, 0xBAF3,
    0x5285, 0x430C, 0x7197, 0x601E, 0x14A1, 0x0528, 0x37B3, 0x263A,
    0xDECD, 0xCF44, 0xFDDF, 0xEC56, 0x98E9, 0x8960, 0xBBFB, 0xAA72,
    0x6306, 0x728F, 0x4014, 0x519D, 0x2522, 0x34AB, 0x0630, 0x17B9,
    0xEF4E, 0xFEC7, 0xCC5C, 0xDDD5, 0xA96A, 0xB8E3, 0x8A78, 0x9BF1,
    0x7387, 0x620E, 0x5095, 0x411C, 0x35A3, 0x242A, 0x16B1, 0x0738,
    0xFFCF, 0xEE46, 0xDCDD, 0xCD54, 0xB9EB, 0xA862, 0x9AF9, 0x8B70,
    0x8408, 0x9581, 0xA71A, 0xB693, 0xC22C, 0xD3A5, 0xE13E, 0xF0B7,
    0x0840, 0x19C9, 0x2B52, 0x3ADB, 0x4E64, 0x5FED, 0x6D76, 0x7CFF,
    0x9489, 0x8500, 0xB79B, 0xA612, 0xD2AD, 0xC324, 0xF1BF, 0xE036,
    0x18C1, 0x0948, 0x3BD3, 0x2A5A, 0x5EE5, 0x4F6C, 0x7DF7, 0x6C7E,
    0xA50A, 0xB483, 0x8618, 0x9791, 0xE32E, 0xF2A7, 0xC03C, 0xD1B5,
    0x2942, 0x38CB, 0x0A50, 0x1BD9, 0x6F66, 0x7EEF, 0x4C74, 0x5DFD,
    0xB58B, 0xA402, 0x9699, 0x8710, 0xF3AF, 0xE226, 0xD0BD, 0xC134,
    0x39C3, 0x284A, 0x1AD1, 0x0B58, 0x7FE7, 0x6E6E, 0x5CF5, 0x4D7C,
    0xC60C, 0xD785, 0xE51E, 0xF497, 0x8028, 0x91A1, 0xA33A, 0xB2B3,
    0x4A44, 0x5BCD, 0x6956, 0x78DF, 0x0C60, 0x1DE9, 0x2F72, 0x3EFB,
    0xD68D, 0xC704, 0xF59F, 0xE416, 0x90A9, 0x8120, 0xB3BB, 0xA232,
    0x5AC5, 0x4B4C, 0x79D7, 0x685E, 0x1CE1, 0x0D68, 0x3FF3, 0x2E7A,
    0xE70E, 0xF687, 0xC41C, 0xD595, 0xA12A, 0xB0A3, 0x8238, 0x93B1,
    0x6B46, 0x7ACF, 0x4854, 0x59DD, 0x2D62, 0x3CEB, 0x0E70, 0x1FF9,
    0xF78F, 0xE606, 0xD49D, 0xC514, 0xB1AB, 0xA022, 0x92B9, 0x8330,
    0x7BC7, 0x6A4E, 0x58D5, 0x495C, 0x3DE3, 0x2C6A, 0x1EF1, 0x0F78
};

//-----------------------------------------------------------------------------
// 用于监测CPU寄存器的常量数组
//-----------------------------------------------------------------------------
/*
static const U32 cpu_reg_ref[] =
{
//系统控制块类寄存器参考值
0x00000001,                     //MEMMAP                0        0x400FC040
0x00000060,                     //SCS                   1        0x400FC1A0
0x00000001,                     //CLKSRCSEL             2        0x400FC10C
((0x07 << 24) | \
((PLL_NVALUE - 1) << 16) | \
(PLL_MVALUE - 1)),              //PLL0STAT              3        0x400FC088
0x2328861E,                     //PCONP                 4        0x400FC0C4

PCLKSEL0_INIT,                  //PCLKSEL0              5        0x400FC1A8
PCLKSEL1_INIT,                  //PCLKSEL1              6        0x400FC1AC

(FCCLK_DIV - 1),                //CCLKCFG               7        0x400FC104
0x00000000,                     //CLKOUTCFG             8        0x400FC1C8

#if FCCLK <= 20000000
(0x01ul << 12) | 0x003a,        //FLASHCFG              9        0x400FC000
#endif//FCCLK <= 20000000
#if FCCLK > 20000000 && FCCLK <= 40000000
(0x02ul << 12) | 0x003a,        //FLASHCFG              9        0x400FC000
#endif//FCCLK > 20000000 && FCCLK <= 40000000
#if FCCLK > 40000000 && FCCLK <= 60000000
(0x03ul << 12) | 0x003a,        //FLASHCFG              9        0x400FC000
#endif//FCCLK > 40000000 && FCCLK <= 60000000
#if FCCLK > 60000000 && FCCLK <= 80000000
(0x04ul << 12) | 0x003a,        //FLASHCFG              9        0x400FC000
#endif//FCCLK > 60000000 && FCCLK <= 80000000
#if FCCLK > 80000000 && FCCLK <= 100000000
(0x05ul << 12) | 0x003a,        //FLASHCFG              9        0x400FC000
#endif//FCCLK > 80000000 && FCCLK <= 100000000

//Sys_Tick模块
0x00000007,                     //STCTRL                10       0xE000E010
(FCCLK / (1000 / SYS_TICK)),    //STRELOAD              11       0xE000E014
//NVIC模块寄存器参考值
0x00002800,                     //IP0                   12       0xE000E400
0x40383000,                     //IP1                   13       0xE000E404
0x10000048,                     //IP2                   14       0xE000E408
0x20180000,                     //IP3                   15       0xE000E40C
0x00000000,                     //IP4                   16       0xE000E410
0x00005800,                     //IP5                   17       0xE000E414

#ifdef __RELEASE_L                                      //低区运行版本
0x00002000,                     //VTOR                  18       0xE000ED08
#endif  //__RELEASE_L
#ifdef __RELEASE_H                                      //低区运行版本
0x00020000,                     //VTOR                  18       0xE000ED08
#endif  //__RELEASE_H

0x00000000,                     //SCR                   19       0xE000ED10
0x00000200,                     //CCR                   20       0xE000ED14
0x00000000,                     //SHPR0                 21       0xE000ED18
0x00000000,                     //SHPR1                 22       0xE000ED1C
0x08000000,                     //SHPR2                 23       0xE000ED20
0x00000000,                     //SHCSR                 24       0xE000ED24
0x00000000,                     //CFSR                  25       0xE000ED28
0x00000000,                     //HFSR                  26       0xE000ED2C
};
*/
//------------------------------ 私有变量声明 ---------------------------------

//------------------------------ 私有函数声明 ---------------------------------
//static void cpu_one_reg_watch(V32 *reg, U32 ref, U8 flg);
//static void cpu_regs_watch(void);
static U32 power10(U8 num);
//static U32 check_leap_year(U8 year);
/****************************************************************************
函数名称：AddVerifyOffset
功能描述：和校验计算,为了防止全零的情况，把校验增加一个偏移
输    入：*pVerData,校验数据指针;
          uclen,数据长度;
                    ucVerConst,校验常数
输    出：无;
返    回：校验值 + 0x5A;
全局变量：无;
调用模块：无;
--------------------------------------------
创建时间：
版 本 号：1.00
创 建 人：
--------------------------------------------
修改时间：
修 改 号：
修 改 人：
修改内容：
****************************************************************************/
//int8u AddVerifyOffset( int8u *pVerData, int16u uiLen,int8u ucVerConst )
//{
//    int8u ucTmp;
//    int16u i;
//
//    ucTmp = 0;
////    while( uiLen-- )
////    {
////        ucTmp += *pVerData++;
////    }
//    for(i=0; i<uiLen; i++)
//    {
//        ucTmp += pVerData[i];
//    }
//    ucTmp += ucVerConst;
//
//    return ucTmp;
//}
/******************************************************************************
** 函数名称: U8 addverify(U8 *pverdata, U16 len)
** 函数功能: 和校验计算
** 入口参数:
**
**
** 返 回 值:
** 说    明:
******************************************************************************/
U8 addverify (U8 *pverdata, U16 len)
{
    U8 data;
    U16 i;
    data = 0;
//  while(len--)
//  {
//    data += *pverdata++;
//  }
    for(i=0; i<len; i++)
    {
        data += pverdata[i];
    }

    data += 0x3A; //azh 广东

    return data;
}
/******************************************************************************
** 函数名称: U16 calc_crc16(U8 *ptr, U32 len, U16 init_value)
** 函数功能: 计算16位长度的CRC
** 入口参数: ptr       : 需要计算CRC的数据
**           len       : 数据长度
**           init_value: 初始化值，可以排除全'0x00'数据计算结果为'0x0000'等问题
** 返 回 值: CRC值
** 说    明: 生成多项式为：x16+x12+x8+x7+x3+1，简记式为0x1189  0b 0001 0001 1000 1001
** 代码计算中需要超过64K的长度, 在主循环中花的时间应该可以接受
******************************************************************************/
U16 calc_crc16 (U8 *ptr, U32 len, U16 init_value)
{
    U16 crc;

    crc = init_value;

    while(len--)
    {
        crc = crc_tbl[(U8)(crc ^ *ptr++)] ^ (crc >> 8);
    }
//    crc ^= 0xffff;

    return crc;
}

//U8 calc_BCC (U8 *ptr, U16 len)
//{
//    U8  BCC;
//    U16 i;
//
//    BCC = 0;
//    for(i = 0; i < len; i++)
//    {
//        BCC ^= *(ptr + i);
//    }
//    return BCC;
//}
/******************************************************************************
** 函数名称: void delay(U32 time)
** 函数功能: 软件延时，程序将在这里阻塞等待
** 入口参数: time需要延时的微秒数，1 = 1uS
** 返 回 值: 无
** 说    明: 不能用于精确定时，实际延时值会略大于要求值，要求的延时值越小误差越大
**           三级优化下的延时情况，如果是其它优化等级延时值会加长, 适用主频92M
******************************************************************************/
void delay (U32 time)
{
    U32 i;

    while(time--)
    {
        for(i = 0; i < 25; i++);
    }
}

/******************************************************************************
** 函数名称: U8 hex_to_bcd_in8(U8 data)
** 函数功能: 单字节hex转换为BCD码
** 入口参数: data: 单字节hex数值
** 返 回 值: 单字节BCD码
** 说    明: 对大于99的数据同100求余，超过部分丢失
******************************************************************************/
U8 hex_to_bcd_in8 (U8 data)
{
    U8 c;

    if(data > 199)
    {
        data -= 200;
    }
    else if(data > 99)
    {
        data -= 100;
    }

    c = data / 10;
    data -= c * 10;         //得到个位的BCD数
    data += c << 4;         //得到十位的BCD数
    return(data);
}

/******************************************************************************
** 函数名称: U8 bcd_to_hex(U8 data)
** 函数功能: 单字节BCD码转换为hex
** 入口参数: data: 单字节BCD码
** 返 回 值: 单字节hex码
** 说    明: 无
******************************************************************************/
U8 bcd_to_hex (U8 data)
{
    return((data>>4) * 10 + (data & 0x0f));
}

/******************************************************************************
** 函数名称: void convert_to_bcd(U8 *p, U8 len)
** 函数功能: 将多个二进制数按每个字节转换为BCD码，输出到p
** 入口参数: p  : 待转换的二进制数的数组地址，同时也是结果返回的数组
**           len: 需要转换的字节数
** 返 回 值: 无
** 说    明: 转换后的值将直接覆盖数组p
******************************************************************************/
void convert_to_bcd (U8 *p, U8 len)
{
    U8 i;

    for(i = 0; i < len; i++)
    {
        p[i] = HEX_TO_BCD(p[i], 1);        //转换为BCD
    }
}

/******************************************************************************
** 函数名称: void convert_to_hex(U8 *p, U8 len)
** 函数功能: 将多个BCD数按每个字节转换为hex，输出到p
** 入口参数: p  : 待转换的BCD数的数组地址，同时也是结果返回的数组
**           len: 需要转换的字节数
** 返 回 值: 无
** 说    明: 转换后的值将直接覆盖数组p
******************************************************************************/
void convert_to_hex (U8 *p, U8 len)
{
    U8 i;

    for(i = 0; i < len; i++)
    {
        p[i] = bcd_to_hex(p[i]);        //转换为HEX
    }
}

/******************************************************************************
** 函数名称: U32 bytes_to_bcd(U32 data, U8 *p, U8 len)
** 函数功能: 将二进制数转换为指定个数的BCD码，输出到p
** 入口参数: data: 要转换的4字节hex数
**           p   : 返回结果的数组，低字节在前
**           len : 需要转换的BCD个数（不是字节数，例：要转换得到'123'应该传入'3'），取值1~10
** 返 回 值: 长度错误时返回'0'，p不做任何修改
**           正常情况下返回压缩的4字节BCD，如果有第5个字节BCD码则丢弃
**           p返回压缩的1－10个BCD数
** 说    明: p要满足len的长度要求，否则造成数组溢出
******************************************************************************/
U32 bytes_to_bcd (U32 data, U8 *p, U8 len)
{
    U8 i, j, k;
    U32 tmp, mi;

    if((len > 10) || (len == 0))
    {
        return 0;
    }

    j = len / 2;
    if(len != (j * 2))                                  //是奇数
    {
        j++;
    }
    k = j;
    for(i = 0; i < j; i++)
    {
        p[i] = 0;                                       //需要用到的缓冲区清零
    }
    mi = power10(len);
    tmp = data / mi;                                    //得到商
    if(tmp > 9)                                         //给了一个大的hex，但BCD只要截取其中几个
    {
        data -= tmp * mi;                               //取余数
    }
    for(i = len; i > 0; i--)
    {
        mi = power10(i - 1);
        tmp = data / mi;                                //得到商
        data -= tmp * mi;                               //取余数
        j = (i - 1) / 2;
        if((i - 1) == (j * 2))                          //减“1”后是偶数，原先是奇数
        {
            p[j] |= tmp;
        }
        else
        {
            p[j] |= tmp << 4;
        }
    }
    tmp = 0;
    if(k > 4)
    {
        k = 4;
    }
    for(i = 0; i < k; i++)
    {
        tmp |= p[i] << (i * 8);                         //得到BCD数
    }
    return tmp;
}

/******************************************************************************
** 函数名称: U32 bcd_to_bytes(U8 *p, U8 len)
** 函数功能: 将指定字节数的BCD码转换为二进制数
**           结果小于等于4字节，可转换的最大BCD数为'4294967295'
** 入口参数: p  : 需要转换的压缩BCD，低字节在前
**           len: 需要转换的字节数，取值1~5
** 返 回 值: 转换结果hex，发现非BCD码时返回'0'
** 说    明: 无
******************************************************************************/
U32 bcd_to_bytes (U8 *p, U8 len)
{
    U32 i, tmp;

    if((len > 5) || (len == 0))
    {
        return 0;
    }
    tmp = 0;
    for(i = len - 1; ; i--)
    {
        if((p[i] & 0x0f) > 9 || (p[i] & 0xf0) > 0x90)
        {
            return 0;
        }
        tmp += (p[i] & 0x0f);
        tmp += ((p[i] >> 4) * 10);
        if(i == 0)
        {
            return tmp;
        }
        tmp *= 100;
    }
}

/******************************************************************************
** 函数名称: U32 power10(U8 num)
** 函数功能: 求10的幂
** 入口参数: num: 幂的级数，取值0~9
** 返 回 值: 10的幂，错误返回'0'
** 说    明: 无
******************************************************************************/
U32 power10 (U8 num)
{
    U8 i;
    U32 tmp;

    if(num > 9)
    {
        return 0;
    }
    tmp = 1;
    for(i = 0; i < num; i++)
    {
        tmp *= 10;
    }
    return tmp;
}

/******************************************************************************
** 函数名称: U32 power2(U8 num)
** 函数功能: 求2的幂
** 入口参数: num: 幂的级数,取值0~20
** 返 回 值: 2的幂，错误返回'0'
** 说    明: 无
******************************************************************************/
U32 power2 (U8 num)
{
    U8 i;
    U32 tmp;

    if(num > 20)
    {
        return 0;
    }
    tmp = 1;
    for(i = 0; i < num; i++)
    {
        tmp *= 2;
    }
    return tmp;
}

/******************************************************************************
** 函数名称: void get_event_record_time(U8 *time)
** 函数功能: 获取一个事件发生时间，mm（分）hh（时）DD（日）MM（月）YY（年），十进制格式
** 入口参数: time: 返回时间数据的缓冲区，5字节
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
//void get_event_record_time (U8 *time)
//{
//    U8 sec1, sec2;
//
//    sec1 = RTCSEC;                          //秒
//    time[0] = RTCMIN;                       //分
//    time[1] = RTCHOUR;                      //时
//    time[2] = RTCDOM;                       //日
//    time[3] = RTCMONTH;                     //月
//    time[4] = RTCYEAR;                      //年
//
//    sec2 = RTCSEC;                          //再读一次秒
//    if((sec1 != sec2) && (sec2 == 0))       //如果2次读到的秒不一样且后一次的秒为'0'，时间进位了需要重读
//    {
//        time[0] = RTCMIN;                   //分
//        time[1] = RTCHOUR;                  //时
//        time[2] = RTCDOM;                   //日
//        time[3] = RTCMONTH;                 //月
//        time[4] = RTCYEAR;                  //年
//    }
//
//    convert_to_bcd(time, 5);                //转换为BCD
//}

/******************************************************************************
** 函数名称: S8 decrease_event_record_point(U8 point, U8 total, U8 num)
** 函数功能: 得到事件记录区从当前记录回溯'n'条的记录点
** 入口参数: total: 总的记录条数（数据区容量）
**           point: 当前记录指针（总是指向空记录或最老的一条记录）bit7 = '1'表示记录区已经翻转
**           num  : 回溯的条数
** 返 回 值: 回溯后的记录点，bit7 = '1'表示记录区已经翻转
**           '-1'没有找到记录点
** 说    明: 用返回值作为偏移读取记录点数据时必须屏蔽bit7
******************************************************************************/
S8 decrease_event_record_point (U8 point, U8 total, U8 num)
{
    U8 flag;

    if(num == 0)                    //回溯的条数为'0'，不处理
    {
        return -1;
    }

    flag = point & 0x80;            //保存翻转标志位
    point &= ~0x80;                 //屏蔽翻转标志位
    if(point >= total)
    {
        return -1;                  //如果当前指针超出最大容量（非法），返回'-1'
    }

    if(flag != 0)                   //判断记录是否翻转
    {
        if(num > total)             //如果回溯的条数大于总条数，找不到记录
        {
            return -1;
        }
        else if(num < total)        //如果回溯的条数小于总条数
        {
            if(num > point)         //判断回溯的数量是否翻越记录区头部
            {
                point = total - (num - point);
            }
            else
            {
                point -= num;
            }
        }
        else                        //如果回溯的条数等于总条数，直接返回当前记录指针
        {
            point = point;          //将被编译器优化
        }
    }
    else
    {
        if(num > point)             //判断回溯的数量是否翻越记录区头部
        {
            return -1;              //没有任何记录
        }
        else
        {
            point -= num;
        }
    }
    return (S8)(point | flag);
}

/******************************************************************************
** 函数名称: U8 next_event_record_point(U8 point, U8 total)
** 函数功能: 得到事件记录区的下一个记录点，总是指向空记录或最老的一条记录
** 入口参数: total: 总的记录条数（数据区容量）
**           point: 当前记录指针（总是指向空记录或最老的一条记录）bit7 = '1'表示记录区已经翻转
** 返 回 值: 当前记录的下一个记录点
** 说    明: 用返回值作为偏移读取记录点数据时必须屏蔽bit7
******************************************************************************/
//U8 next_event_record_point (U8 point, U8 total)
//{
//    U8 flag;
//
//    flag = point & 0x80;            //保存翻转标志位
//    point &= ~0x80;                 //屏蔽翻转标志位
//    if(++point >= total)
//    {
//        flag = 0x80;
//        point = 0;                  //记录翻转
//    }
//    return (point | flag);
//}

/******************************************************************************
** 函数名称: S32 decrease_load_profile_point(U32 point, U32 total, U32 num)
** 函数功能: 得到负荷曲线记录区从当前记录回溯'n'条的记录点
** 入口参数: total: 总的记录条数（数据区容量）
**           point: 当前记录指针（总是指向空记录或最老的一条记录）bit31 = '1'表示记录区已经翻转
**           num  : 回溯的条数
** 返 回 值: 回溯后的记录点，bit31 = '1'表示记录区已经翻转
**           '-1'没有找到记录点
** 说    明: 用返回值作为偏移读取记录点数据时必须屏蔽bit31
******************************************************************************/
//S32 decrease_load_profile_point (U32 point, U32 total, U32 num)
//{
//    U32 flag;
//
//    if(num == 0)                    //回溯的条数为'0'，不处理
//    {
//        return -1;
//    }
//
//    flag = point & 0x80000000;      //保存翻转标志位
//    point &= ~0x80000000;           //屏蔽翻转标志位
//    if(point >= total)
//    {
//        return -1;                  //如果当前指针超出最大容量（非法），返回'-1'
//    }
//
//    if(flag != 0)                   //判断记录是否翻转
//    {
//        if(num > total)             //如果回溯的条数大于总条数，找不到记录
//        {
//            return -1;
//        }
//        else if(num < total)        //如果回溯的条数小于总条数
//        {
//            if(num > point)         //判断回溯的数量是否翻越记录区头部
//            {
//                point = total - (num - point);
//            }
//            else
//            {
//                point -= num;
//            }
//        }
//        else                        //如果回溯的条数等于总条数，直接返回当前记录指针
//        {
//            point = point;          //将被编译器优化
//        }
//    }
//    else
//    {
//        if(num > point)             //判断回溯的数量是否翻越记录区头部
//        {
//            return -1;              //如果回溯的条数翻越记录区头部，找不到记录
//        }
//        else
//        {
//            point -= num;
//        }
//    }
//    return (S32)(point | flag);
//}

/******************************************************************************
** 函数名称: U32 next_load_profile_point(U32 point, U32 total)
** 函数功能: 得到负荷曲线记录区的下一个记录点，总是指向空记录或最老的一条记录
** 入口参数: total: 总的记录条数（数据区容量）
**           point: 当前记录指针（总是指向空记录或最老的一条记录）bit31 = '1'表示记录区已经翻转
** 返 回 值: 当前记录的下一个记录点
** 说    明: 用返回值作为偏移读取记录点数据时必须屏蔽bit31
******************************************************************************/
//U32 next_load_profile_point (U32 point, U32 total)
//{
//    U32 flag;
//
//    flag = point & 0x80000000;          //保存翻转标志位
//    point &= ~0x80000000;               //屏蔽翻转标志位
//    if(++point >= total)
//    {
//        flag = 0x80000000;
//        point = 0;                      //记录翻转
//    }
//    return (point | flag);
//}

/******************************************************************************
** 函数名称: U32 check_event_time_valid(U8 *time)
** 函数功能: 判断事件发生时间（BCD码）的格式是否有效
** 入口参数: time: 事件发生时间地址，分+时+日+月+年
** 返 回 值: __TRUE时间格式有效，__FALSE时间格式无效
** 说    明: 无
******************************************************************************/
//U32 check_event_time_valid (U8 *time)
//{
//    U8 i;
//
//    for(i = 0; i < 5; i++)
//    {
//        if(((time[i] & 0x0f) > 9) || ((time[i] & 0xf0) > 0x90))
//        {
//            return __FALSE;
//        }
//    }
//    if(time[0] > 0x59)                          //分
//    {
//        return __FALSE;
//    }
//    if(time[1] > 0x23)                          //时
//    {
//        return __FALSE;
//    }
//    if((time[2] == 0) || (time[2] > 0x31))      //日
//    {
//        return __FALSE;
//    }
//    if((time[3] == 0) || (time[3] > 0x12))      //月
//    {
//        return __FALSE;
//    }
//    return __TRUE;
//}

/******************************************************************************
** 函数名称: U32 check_bcd_time_valid(U8 *time)
** 函数功能: 判断BCD码时间的格式是否有效
** 入口参数: time: BCD时间地址，秒+分+时+周+日+月+年
** 返 回 值: __TRUE时间格式有效，__FALSE时间格式无效
** 说    明: 无
******************************************************************************/
U32 check_bcd_time_valid (U8 *time)
{
    U8 i;

    for(i = 0; i < 7; i++)
    {
        if(((time[i] & 0x0f) > 9) || ((time[i] & 0xf0) > 0x90))
        {
            return __FALSE;
        }
    }
    if(time[0] > 0x59)                          //秒
    {
        return __FALSE;
    }
    if(time[1] > 0x59)                          //分
    {
        return __FALSE;
    }
    if(time[2] > 0x23)                          //时
    {
        return __FALSE;
    }
    if(time[3] > 0x06)                          //周
    {
        return __FALSE;
    }
    if((time[4] == 0) || (time[4] > 0x31))      //日
    {
        return __FALSE;
    }
    if((time[5] == 0) || (time[5] > 0x12))      //月
    {
        return __FALSE;
    }
    if((time[6] < 0x10) || (time[6] > 0x50))    //年，在2010~2050之间
    {
        return __FALSE;
    }
    return __TRUE;
}

/******************************************************************************
** 函数名称: U32 check_hex_time_valid(U8 *time)
** 函数功能: 判断hex格式的时间是否在有效取值范围内
** 入口参数: time: hex时间地址，秒+分+时+周+日+月+年
** 返 回 值: __TRUE时间有效，__FALSE时间无效
** 说    明: 无
******************************************************************************/
U32 check_hex_time_valid (U8 *time)
{
    if(time[0] > 59)                            //秒
    {
        return __FALSE;
    }
    if(time[1] > 59)                            //分
    {
        return __FALSE;
    }
    if(time[2] > 23)                            //时
    {
        return __FALSE;
    }
    if(time[3] > 6)                             //周
    {
        return __FALSE;
    }
    if((time[4] == 0) || (time[4] > 31))        //日
    {
        return __FALSE;
    }
    if((time[5] == 0) || (time[5] > 12))        //月
    {
        return __FALSE;
    }
    if((time[6] < 10) || (time[6] > 50))        //年，在2010~2050之间
    {
        return __FALSE;
    }
    return __TRUE;
}

/******************************************************************************
** 函数名称: S32 compare_time(U8 *p1, U8 *p2)
** 函数功能: 比较两个时间的大小
** 入口参数: p1: 待比较时间1，分+时+日+月+年
**           p2: 待比较时间2，分+时+日+月+年
** 返 回 值: 1: p1 > p2，0: p1 = p2，-1: p1 < p2
** 说    明: 无
******************************************************************************/
//S32 compare_time (U8 *p1, U8 *p2)
//{
//    U32 time1, time2;
//
//    if(p1[4] > p2[4])       //比较'年'
//    {
//        return 1;
//    }
//    else if(p1[4] < p2[4])
//    {
//        return -1;
//    }
//    else                    //'年'相等
//    {
//        time1 = (p1[3] << 24) | (p1[2] << 16) | (p1[1] << 8) | p1[0];
//        time2 = (p2[3] << 24) | (p2[2] << 16) | (p2[1] << 8) | p2[0];
//        if(time1 > time2)
//        {
//            return 1;
//        }
//        else if(time1 < time2)
//        {
//            return -1;
//        }
//        else
//        {
//            return 0;
//        }
//    }
//}

/******************************************************************************
** 函数名称: U16 day_of_year(U8 year, U8 month, U8 day)
** 函数功能: 计算给定的'年'、'月'、'日'是一年中的第几天
** 入口参数: year : '年'值，HEX格式
**           month: '月'值，HEX格式
**           day  : '日'值，HEX格式
** 返 回 值: 一年中第几天，如果时间格式错误则返回'0'
** 说    明: 无
******************************************************************************/
//U16 day_of_year (U8 year, U8 month, U8 day)
//{
//    U16 sum;
//
//    switch(month)                   //计算月份代表的天数
//    {
//    case 1:
//        sum = 0;
//        break;
//    case 2:
//        sum = 31;
//        break;
//    case 3:
//        sum = 59;
//        break;
//    case 4:
//        sum = 90;
//        break;
//    case 5:
//        sum = 120
//        ;break;
//    case 6:
//        sum = 151;
//        break;
//    case 7:
//        sum = 181;
//        break;
//    case 8:
//        sum = 212;
//        break;
//    case 9:
//        sum = 243;
//        break;
//    case 10:
//        sum = 273;
//        break;
//    case 11:
//        sum = 304;
//        break;
//    case 12:
//        sum = 334;
//        break;
//    default:
//        return 0;
//    }
//    sum = sum + day;                //再加上天数
//    //if((__TRUE == check_leap_year(year)) && (month > 2))
//    if((1 == check_leap_year(year)) && (month > 2))
//    {
//        sum++;                      //如果是闰年且月份大于2，天数应该加一天
//    }
//    return sum;
//}

/******************************************************************************
** 函数名称: U32 check_leap_year(U8 year)
** 函数功能: 判断是否闰年
** 入口参数: year : '年'值，一字节hex格式
** 返 回 值: 闰年__TRUE，不是闰年__FALSE
** 说    明: 无
******************************************************************************/
U32 check_leap_year (U8 year)
{
    if(!(year % 4))
    {
        //return __TRUE;
        return 1;//azh
    }
    else
    {
        //return __FALSE;
        return 0;
    }
}

/******************************************************************************
** 函数名称: U32 max(U32 x, U32 y)
** 函数功能: 判断两个32位无符号数的大小
** 入口参数: x: 第一个32位无符号数
**           y: 第二个32位无符号数
** 返 回 值: 较大的数
** 说    明: 无
******************************************************************************/
U32 max (U32 x, U32 y)
{
    return(x > y? x : y);
}

/******************************************************************************
** 函数名称: U32 min(U32 x, U32 y)
** 函数功能: 判断两个32位无符号数的大小
** 入口参数: x: 第一个32位无符号数
**           y: 第二个32位无符号数
** 返 回 值: 较小的数
** 说    明: 无
******************************************************************************/
U32 min (U32 x, U32 y)
{
    return(x < y? x : y);
}

/******************************************************************************
** 函数名称: void energy_to_format_data(U8 *p1, U8 *p2, U8 flag)
** 函数功能: 把一个四个字节的hex转换为DL/T645-2007规定的电能量的格式4字节BCD
** 入口参数: *p1 : hex的指针
**           *p2 ：返回数组的存放位置
**           flag: 0,3,4组合电量，其他非组合电量
** 返 回 值: 转换后的DL/T645-2007规定的电能量数据，同'p2'值
** 说    明: 按DL/T645-2007规定bit31为符号位
**           组合电量转换结果0~79999999，非组合电量转换结果0~99999999
******************************************************************************/
//U32 energy_to_format_data (U8 *p1, U8 *p2, U8 flag)
//{
//    U8 i;
//    U8 sig;
//    S32 data;
//    U32 tmp;
//
//    data = 0;                       //必须先清零
//    for(i = 0; i < 4; i++)
//    {
//        data <<= 8;
//        data += p1[3 - i];          //字节逐个组合
//    }
//
//    if((flag == 0) || (flag == 3) || (flag == 4))
//    {
//        if(data < 0)
//        {
//            data = (~data) + 1;
//            sig = (1ul << 7);
//        }
//        else
//        {
//            sig = 0;
//        }
//
//        if(data > 79999999)         //检查数据是否溢出
//        {
//            data %= 80000000;       //数据去除溢出部分，留下可保存部分
//        }
//    }
//    else
//    {
//        sig = 0;
//
//        if(data > 99999999)         //检查数据是否溢出
//        {
//            data %= 100000000;      //数据去除溢出部分，留下可保存部分
//        }
//    }
//    tmp = bytes_to_bcd((U32)data, p2, 8);
//    p2[3] |= sig;
//    tmp |= sig << 24;
//    return tmp;
//}

/******************************************************************************
** 函数名称: U32 byte_merge(U8 *dat_buf, U8 num)
** 函数功能: 合并多个字节组合为一个U32数
** 入口参数: dat_buf: 需要合并的数的缓冲区起始地址
**           num    : 需要合并的数的个数，取值0~4
** 返 回 值: 合并后的U32数
** 说    明: num大于4字节时合并值为'dat_buf'的最后4字节，合并后'dat_buf'的低字节在返回值的高位
******************************************************************************/
U32 byte_merge (U8 *dat_buf, U8 num)
{
    U8 i;
    U32 dat_temp;

    dat_temp = 0;                       //必须先清零
    for(i = 0; i < num; i++)
    {
        dat_temp <<= 8;
        dat_temp += dat_buf[i];         //字节逐个组合
    }

    return dat_temp;
}

/******************************************************************************
** 函数名称: U32 byte_merge1(U8 *dat_buf, U8 num)
** 函数功能: 合并多个字节组合为一个U32数
** 入口参数: dat_buf: 需要合并的数的缓冲区起始地址
**           num    : 需要合并的数的个数，取值0~4
** 返 回 值: 合并后的U32数
** 说    明: num大于4字节时合并值为'dat_buf'的最后4字节，合并后'dat_buf'的低字节在返回值的高位
******************************************************************************/
U32 byte_merge1 (U8 *dat_buf, U8 num)
{
    U8 i;
    U32 dat_temp;

    dat_temp = 0;                       //必须先清零
    for(i = 0; i < num; i++)
    {
        dat_temp += (dat_buf[i] << (8 * i));//字节逐个组合
    }

    return dat_temp;
}

/******************************************************************************
** 函数名称: void fill_exmemory(U32 addr, U8 flg, U8 len)
** 函数功能: 外存区域填充函数
** 入口参数: addr：外存地址
**           flg : 取1:为0xFF填充;取0为0填充
**           len : 清零字节长度
** 返 回 值: 无
** 说    明: 本函数用于将EEPROM 或 FRAM 某段区域清零
******************************************************************************/
//void fill_exmemory (U32 addr, U8 flg, U8 len)
//{
/*
    U8  i;
    U8  dat_buf[1];

    file_read(addr, dat_buf, len);              //填充前回读
    for(i = 0; i < len; i++)
    {
        dat_buf[i] = flg * 0xFF;                //填充内容筹备
    }
    file_write(addr, dat_buf, len);             //数据填充
    file_read(addr, dat_buf, len);              //回读验证
*/
//}

/******************************************************************************
函数: void fill_char(U8 buf[], U32 len, U8 flag)
入口: buf[]-要填充缓冲区, len-缓冲区长度, flag-填充字符
出口: 无
概述: 向指定缓冲区填充固定字符
******************************************************************************/
//void fill_char (U8 buf[], U32 len, U8 flag)
//{
//    U32 i;
//
//    for(i = 0; i < len; i++)
//    {
//        buf[i] = flag;
//    }
//}

/******************************************************************************
** 函数名称: void cpu_one_reg_watch(V32 *reg, U32 ref, U8 flg)
** 函数功能: CPU寄存器设定参数监测处理函数
** 入口参数: reg : 寄存器指针,取值见lpc1700.h文件定义
**           ref : CPU稳定运行时配置的既定参数,见本文件常数定义
**           flg : 取值决定复位CPU或者覆盖寄存器：0 复位, 1 覆盖;
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
/*
void cpu_one_reg_watch (V32 *reg, U32 ref, U8 flg)
{
    U32 refer;

    refer = ref;                                    //暂存参考值
    ref = *reg;                                     //预读以将"控制&状态寄存器"(如:STCTRL)状态位清零
    if(*reg != refer)
    {
        if(flg)
        {
            *reg = refer;                           //一旦运行过程Reg配置被错误改写,即时恢复
        }
        else
        {
            while(1);                               //自毙(WTD复位)
        }
    }
}
*/
/******************************************************************************
** 函数名称: void cpu_regs_watch(void)
** 函数功能: CPU非数据寄存器设定参数监测恢复函数
** 入口参数: 无
**
** 返 回 值: 无
** 说    明: 要求每组regs监测间隔为5秒,即15秒完成全部三组reg监测
******************************************************************************/
//void cpu_regs_watch (void)
//{
/*
    cpu_one_reg_watch(&MEMMAP, cpu_reg_ref[0], 0);                       //检查MEMMAP
    cpu_one_reg_watch(&SCS, cpu_reg_ref[1], 0);                          //检查SCS
    cpu_one_reg_watch(&CLKSRCSEL, cpu_reg_ref[2], 0);                    //检查CLKSRCSEL
    cpu_one_reg_watch(&PLL0STAT, cpu_reg_ref[3], 0);                     //检查PLL0STAT
    cpu_one_reg_watch(&PCONP, cpu_reg_ref[4], 1);                        //检查PCONP
    cpu_one_reg_watch(&PCLKSEL0, cpu_reg_ref[5], 1);                     //检查PCLKSEL0
    cpu_one_reg_watch(&PCLKSEL1, cpu_reg_ref[6], 1);                     //检查PCLKSEL1
    cpu_one_reg_watch(&CCLKCFG, cpu_reg_ref[7], 0);                      //检查CCLKCFG
    cpu_one_reg_watch(&CLKOUTCFG, cpu_reg_ref[8], 1);                    //检查CLKOUTCFG
    cpu_one_reg_watch(&FLASHCFG, cpu_reg_ref[9], 0);                     //检查FLASHCFG

    cpu_one_reg_watch(&STCTRL, cpu_reg_ref[10], 1);                      //检查STCTRL
    cpu_one_reg_watch(&STRELOAD, cpu_reg_ref[11], 1);                    //检查STRELOAD
    cpu_one_reg_watch(&IP0, cpu_reg_ref[12], 1);                         //检查IP0
    cpu_one_reg_watch(&IP1, cpu_reg_ref[13], 1);                         //检查IP1
    cpu_one_reg_watch(&IP2, cpu_reg_ref[14], 1);                         //检查IP2
    cpu_one_reg_watch(&IP3, cpu_reg_ref[15], 1);                         //检查IP3
    cpu_one_reg_watch(&IP4, cpu_reg_ref[16], 1);                         //检查IP4
    cpu_one_reg_watch(&IP5, cpu_reg_ref[17], 1);                         //检查IP5
    cpu_one_reg_watch(&VTOR, cpu_reg_ref[18], 0);                        //检查VTOR
    cpu_one_reg_watch(&SCR, cpu_reg_ref[19], 1);                         //检查SCR
    cpu_one_reg_watch(&CCR, cpu_reg_ref[20], 1);                         //检查CCR
    cpu_one_reg_watch(&SHPR0, cpu_reg_ref[21], 0);                       //检查SHPR0
    cpu_one_reg_watch(&SHPR1, cpu_reg_ref[22], 0);                       //检查SHPR1
    cpu_one_reg_watch(&SHPR2, cpu_reg_ref[23], 0);                       //检查SHPR2
    cpu_one_reg_watch(&SHCSR, cpu_reg_ref[24], 0);                       //检查SHCSR
    cpu_one_reg_watch(&CFSR, cpu_reg_ref[25], 0);                        //检查CFSR
    cpu_one_reg_watch(&HFSR, cpu_reg_ref[26], 0);                        //检查HFSR
*/
//}
/******************************************************************************
 Function name:  U8 check_mem_type (void *mem)
 Author       :  snap.gao
 Description  :  函数判断内存块地址的合法性
                 尝试遍历并比对所有内存块地址来判别系统已知的类型
 Input        :  内存块地址
 Return       :  内存块类型
                 INVALID_TYPE ：无效类型
                 MEM_64_TYPE  : 64字节/块
                 MEM_256_TYPE : 256字节/块
                 MEM_1536_TYPE: 1536字节/块
 *****************************************************************************/
U8 check_mem_type (void *mem)
{
    U16 i;

    if((((U32)mem & 0xffff0000) != VALID_INSIDE_SRAM) && (((U32)mem & 0xfff00000) != VALID_INSIDE_SRAM1))       //判断地址合法性
   {
        return INVALID_TYPE;
    }


    if((mem >= (U8*)Mem_64_Byte + 12)
    && (mem <= (U8*)((U32)Mem_64_Byte + 12 + (MEM_64_SIZE * (MEM_64_BLOCK - 1)))))          //大致判一下是否在64Byte类型的内存块地址范围内
    {
        for(i = 0; i < MEM_64_BLOCK; i++)           //遍历所有内存块地址
        {
            if(mem == (U8*)((U32)Mem_64_Byte + 12 + (MEM_64_SIZE * i)))
            {
                return MEM_64_TYPE;
            }
        }
    }
    else if((mem >= (U8*)Mem_256_Byte + 12)
    && (mem <= (U8*)((U32)Mem_256_Byte + 12 + (MEM_256_SIZE * (MEM_256_BLOCK - 1)))))       //大致判一下是否在256Byte类型的内存块地址范围内
    {
        for(i = 0; i < MEM_256_BLOCK; i++)          //遍历所有内存块地址
        {
            if(mem == (U8*)((U32)Mem_256_Byte + 12 + (MEM_256_SIZE * i)))
            {
                return MEM_256_TYPE;
            }
        }
    }
    else if((mem >= (U8*)Mem_1536_Byte + 12)
    && (mem <= (U8*)((U32)Mem_1536_Byte + 12 + (MEM_1536_SIZE * (MEM_1536_BLOCK - 1)))))    //大致判一下是否在1024Byte类型的内存块地址范围内
    {
        for(i = 0; i < MEM_1536_BLOCK; i++)         //遍历所有内存块地址
        {
            if(mem == (U8*)((U32)Mem_1536_Byte + 12 + (MEM_1536_SIZE * i)))
            {
                return MEM_1536_TYPE;
            }
        }
    }
    return INVALID_TYPE;
}
/******************************************************************************
 Function name:  U8* alloc_comm_mem (U8 type, U8 *real)
 Author       :  snap.gao
 Description  :  按指定类型申请内存块
                 如果类型错误则默认申请1536Byte的内存块
 Input        :  type: 指定的内存块类型
                 real: 申请到的内存块类型，正常下与type相同，可能返回默认申请的类型
 Return       :  正确申请时返回内存块地址
                 申请不到内存块返回NULL
 *****************************************************************************/
/*
U8* alloc_comm_mem (U8 type, U8 *real)
{
    switch(type)
    {
    case MEM_64_TYPE:
        *real = MEM_64_TYPE;
        return _alloc_box(Mem_64_Byte);             //申请内存块
    case MEM_256_TYPE:
        *real = MEM_256_TYPE;
        return _alloc_box(Mem_256_Byte);            //申请内存块
    case MEM_1536_TYPE:
        *real = MEM_1536_TYPE;
        return _alloc_box(Mem_1536_Byte);           //申请内存块
    default:
        *real = MEM_1536_TYPE;
        return _alloc_box(Mem_1536_Byte);           //默认1536Byte类型
    }
}
*/
U8* alloc_comm_mem (U8 type, U8 *real, U8 id)
{
    U8 *mem_ptr = NULL;

    switch(type)
    {
    case MEM_64_TYPE:
        *real = MEM_64_TYPE;
        mem_ptr = _alloc_box(Mem_64_Byte);
#ifdef DEBUG_MEM
        if(mem_ptr != NULL)
        {
            mem_ptr[MEM_64_SIZE-5] = 0xaa;
            mem_ptr[MEM_64_SIZE-4] = 0x64;
            mem_ptr[MEM_64_SIZE-3] = 0x46;
            mem_ptr[MEM_64_SIZE-2] = id;
            mem_ptr[MEM_64_SIZE-1] = 0x55;
        }
#endif//DEBUG_MEM
        break;
        //return _alloc_box(Mem_64_Byte);             //申请内存块
    case MEM_256_TYPE:
        *real = MEM_256_TYPE;
        mem_ptr = _alloc_box(Mem_256_Byte);
#ifdef DEBUG_MEM
        if(mem_ptr != NULL)
        {
            mem_ptr[MEM_256_SIZE-5] = 0xaa;
            mem_ptr[MEM_256_SIZE-4] = 0x56;
            mem_ptr[MEM_256_SIZE-3] = 0x65;
            mem_ptr[MEM_256_SIZE-2] = id;
            mem_ptr[MEM_256_SIZE-1] = 0x55;
        }
#endif//DEBUG_MEM
        break;
        //return _alloc_box(Mem_256_Byte);            //申请内存块
    case MEM_1536_TYPE:
        *real = MEM_1536_TYPE;
        mem_ptr = _alloc_box(Mem_1536_Byte);
#ifdef DEBUG_MEM
        if(mem_ptr != NULL)
        {
            mem_ptr[MEM_1536_SIZE-5] = 0xaa;
            mem_ptr[MEM_1536_SIZE-4] = 0x36;
            mem_ptr[MEM_1536_SIZE-3] = 0x63;
            mem_ptr[MEM_1536_SIZE-2] = id;
            mem_ptr[MEM_1536_SIZE-1] = 0x55;
        }
#endif//DEBUG_MEM
        break;
        //return _alloc_box(Mem_1536_Byte);           //申请内存块
    default:
        *real = MEM_1536_TYPE;
        mem_ptr = _alloc_box(Mem_1536_Byte);
#ifdef DEBUG_MEM
        if(mem_ptr != NULL)
        {
            mem_ptr[MEM_1536_SIZE-5] = 0xaa;
            mem_ptr[MEM_1536_SIZE-4] = 0x36;
            mem_ptr[MEM_1536_SIZE-3] = 0x63;
            mem_ptr[MEM_1536_SIZE-2] = id;
            mem_ptr[MEM_1536_SIZE-1] = 0x55;
        }
#endif//DEBUG_MEM
        break;
    }
   
    return mem_ptr;
}
/******************************************************************************
 Function name:  S32 free_mem_block (void *mem)
 Author       :  snap.gao
 Description  :  释放内存块，函数将判断内存块地址的合法性
                 尝试遍历并比对所有内存块地址来判别系统已知的类型
                 并相应释放到不同的内存块区域
 Input        :  内存块地址
 Return       :  __FAIL   ：释放失败
                 __SUCCEED：释放成功
 *****************************************************************************/
S32 free_mem_block (void *mem)
{
    U16 i;

//    gdebug1_memptr = mem;
   if((((U32)mem & 0xffff0000) != VALID_INSIDE_SRAM) && (((U32)mem & 0xfff00000) != VALID_INSIDE_SRAM1))       //判断地址合法性
   {
        return __FAIL;
   }

    if((mem >= (U8*)((U32)Mem_64_Byte + 12))
    && (mem <= (U8*)((U32)Mem_64_Byte + 12 + (MEM_64_SIZE * (MEM_64_BLOCK - 1)))))          //大致判一下是否在64Byte类型的内存块地址范围内
    {
        for(i = 0; i < MEM_64_BLOCK; i++)           //遍历所有内存块地址
        {
            if(mem == (U8*)((U32)Mem_64_Byte + 12 + (MEM_64_SIZE * i)))
            {
#ifdef DEBUG_MEM
                memset((U8*)mem+MEM_64_SIZE-5, 0, 5);//申请的时候在内存块的最后写了5个标志字节
#endif//DEBUG_MEM
                _free_box(Mem_64_Byte, mem);        //释放内存块
                return __SUCCEED;
            }
        }
    }
    else if((mem >= (U8*)((U32)Mem_256_Byte + 12))
    && (mem <= (U8*)((U32)Mem_256_Byte + 12 + (MEM_256_SIZE * (MEM_256_BLOCK - 1)))))       //大致判一下是否在256Byte类型的内存块地址范围内
    {
        for(i = 0; i < MEM_256_BLOCK; i++)          //遍历所有内存块地址
        {
            if(mem == (U8*)((U32)Mem_256_Byte + 12 + (MEM_256_SIZE * i)))
            {
#ifdef DEBUG_MEM
                memset((U8*)mem+MEM_256_SIZE-5, 0, 5);//申请的时候在内存块的最后写了5个标志字节

#endif//DEBUG_MEM
                _free_box(Mem_256_Byte, mem);       //释放内存块
                return __SUCCEED;
            }
        }
    }
    else if((mem >= (U8*)((U32)Mem_1536_Byte + 12))
    && (mem <= (U8*)((U32)Mem_1536_Byte + 12 + (MEM_1536_SIZE * (MEM_1536_BLOCK - 1)))))    //大致判一下是否在1024Byte类型的内存块地址范围内
    {
        for(i = 0; i < MEM_1536_BLOCK; i++)         //遍历所有内存块地址
        {
            if(mem == (U8*)((U32)Mem_1536_Byte + 12 + (MEM_1536_SIZE * i)))
            {
#ifdef DEBUG_MEM
                memset((U8*)mem+MEM_1536_SIZE-5, 0, 5);//申请的时候在内存块的最后写了5个标志字节
#endif//DEBUG_MEM
                _free_box(Mem_1536_Byte, mem);      //释放内存块
                return __SUCCEED;
            }
        }
    }

    return __FAIL;
}
/******************************************************************************
 Function name:  void setmemgetnullid (U8 id)
 Author       :  snap.gao
 Description  :  释放内存块，函数将判断内存块地址的合法性
                 尝试遍历并比对所有内存块地址来判别系统已知的类型
                 并相应释放到不同的内存块区域
 Input        :  内存块地址
 Return       :  __FAIL   ：释放失败
                 __SUCCEED：释放成功
 *****************************************************************************/
void setmemgetnullid(U8 id, U8 *funname)
{
    if(id < 32)
    {
        gusMemErrId |= (1<<id);
//      memcpy(&gusMemErrFunName[id][0], funname, 8);
    }
}
/******************************************************************************
** 函数名称: S32 get_addr_offset(U8 last_num, ADDR_OFFSET_TCB *pst_addr, U32 *addr)
** 函数功能: 计算16位长度的CRC
** 入口参数: last_num  : 上xx数据 0:上1, 1:上2, 依次类推
**           pst_addr  : 指向有关偏移相关信息的结构
** 返 回 值: 失败
** 说    明:
******************************************************************************/
S32 get_addr_offset (U8 last_num, ADDR_OFFSET_TCB *pst_addr, U32 *addr)
{
    U32 num;
//  U32 len;
    U32 err;
//    U32 head_addr;        //首地址
//    U32 offset_addr;      //指针存放地址
//    U16 block_len;        //每个数据块长度
//    U8  offset_len;       //指针保存长度
//    U8  max_num;          //最大条数

    err = file_read (pst_addr->offset_addr, (U8 *)&num, pst_addr->offset_len);
    if(err != NO_ERROR)
    {
        return err;
    }
    num %= pst_addr->max_num;

    if(num <= last_num)
    {
        num += pst_addr->max_num;
    }
    num -= (last_num + 1);

    *addr = pst_addr->head_addr + num*pst_addr->block_len;

    return NO_ERROR;
}
/******************************************************************************
 Function name:  U32 RTC_2_second_index(CALENDAR time)
 Author       :  snap.gao
 Description  :  将实时时钟转换为秒索引，1900年为‘0’秒，时间小于1900年时返回‘0’
 Input        :  实时时钟结构体指针
 Return       :  秒索引
 *****************************************************************************/
//U32 RTC_2_second_index(RTC_STRUCT time)
//{
//    S64 res;
//    U16 year;
//    U8  month;
//
//    year = time.Year;
//    month = time.Month;
//    if(month <= 2)          // 1..12 -> 11,12,1..10, Puts Feb last since it has leap day
//    {
//        month += 10;
//        year -= 1;
//    }
//    else
//    {
//        month -= 2;
//    }
//
//    res = (S64)(year / 4 - year / 100 + year / 400)
//        + 367 * month / 12 + time.Date + year * 365 - 337 - 693595;             //Calculate how much days from 1900 to today
//    if(res < 0)     //时间小于1900年
//    {
//        return 0;
//    }
//    res = ((res * 24 + time.Hour) * 60 + time.Min) * 60 + time.Sec;       //Calculate how much seconds
//
//    return (U32)res;
//}

/******************************************************************************
 Function name:  void second_index_2_RTC(U32 res, CALENDAR *time)
 Author       :  snap.gao
 Description  :  将秒索引转换为实时时钟，‘0’秒为1900年
 Input        :  time：实时时钟结构体指针（计算结果）
                 res ：秒索引
 Return       :  无
 *****************************************************************************/
//void second_index_2_RTC(U32 res, RTC_STRUCT *time)
//{
//    U8  year;
//    U8  month;
//    U8  min_month;
//    U8  max_month;
//    U16 days;
//    U32 clock;
//    U8  is_leap;
//
//    days = res / 86400;         //计算天数
//    clock = res % 86400;        //剩余秒数
//
//    year = days / 366;
//    days -= year * 365 + (year + 1) / 4 - (year + 69) / 100 + (year + 369) / 400;
//
//    for(time->Year = year + 1900; ; time->Year++)       //Calcaulate year from 1900
//    {
//        //is_leap = check_leap(time->Year);
//        is_leap = check_leap_year(time->Year);
//        if(days < year_lengths[is_leap])
//        {
//            break;
//        }
//        days -= year_lengths[is_leap];
//    }
//
//    min_month = 0;
//    max_month = 12;
//    for(month = 5; month < 12 && month > 0; month = (min_month + max_month) / 2)    //Compute month and day, use the half search save time
//    {
//        if(days < month_lengths[is_leap][month])                //too big
//        {
//            max_month = month;
//        }
//        else if(days >= month_lengths[is_leap][month + 1])      //too small
//        {
//            min_month = month;
//        }
//        else
//        {
//            break;
//        }
//    }
//    days -= month_lengths[is_leap][month];
//    time->Month = month + 1;
//    time->Date = days + 1;
//
//    time->Hour = clock / 3600;          //Calcaulate hour, 3600s one hour
//    clock = clock % 3600;
//    time->Min = clock / 60;          //Calcaulate minute, 60s one minute
//    time->Sec = clock % 60;          //Calcaulate second
//
//    update_day_of_week();               //计算星期
//}
/******************************************************************************
** 函数名称: U8 asc_to_hex(U8 data)
** 函数功能: 单字节asc转换为4bit hex
** 入口参数:
** 返 回 值: 单字节asc码
** 说    明:
******************************************************************************/
U8 asc_to_hex (U8 data)
{
    if((data >= 0x30) && (data <= 0x39))
    {
        data -= 0x30;
    }
    else if((data >= 0x41) && (data <= 0x46))
    {
        data -= 0x41 + 0x0a;
    }
    else if((data >= 0x61) && (data <= 0x66))
    {
        data -= 0x61 + 0x0a;
    }

    return(data);
}
/******************************************************************************
** 函数名称: U32 HEX_TO_BCD_INT32U(U32 lHex, U8 ucSize)
** 函数功能: 转成4字节BCD
** 入口参数:
** 返 回 值: 单字节asc码
** 说    明:
******************************************************************************/
//U32 HEX_TO_BCD_INT32U (U32 lHex, U8 ucSize)
//{
//    int32u  lRetBCD=0;
//    if(ucSize==1)
//    {
//        if(lHex>99)
//            lHex=99;
//        lRetBCD=lHex%10+(lHex%100)/10*0x10;
//    }
//    if(ucSize==2)
//    {
//        if(lHex>9999)
//            lHex=9999;
//        lRetBCD=lHex%10+(lHex%100)/10*0x10;
//        lHex=lHex/100;
//        lRetBCD=(lHex%10+(lHex%100)/10*0x10)*0x100+lRetBCD;
//    }
//    if(ucSize==3)
//    {
//        if(lHex>999999)
//            lHex=999999;
//        lRetBCD=lHex%10+(lHex%100)/10*0x10;
//        lHex=lHex/100;
//        lRetBCD=(lHex%10+(lHex%100)/10*0x10)*0x100+lRetBCD;
//        lHex=lHex/100;
//        lRetBCD=(lHex%10+lHex/10*0x10)*0x10000+lRetBCD;
//    }
//    if(ucSize==4)
//    {
//        if(lHex>99999999)
//            lHex=99999999;
//        lRetBCD=lHex%10+(lHex%100)/10*0x10;
//        lHex=lHex/100;
//        lRetBCD=(lHex%10+(lHex%100)/10*0x10)*0x100+lRetBCD;
//        lHex=lHex/100;
//        lRetBCD=(lHex%10+(lHex%100)/10*0x10)*0x10000+lRetBCD;
//        lHex=lHex/100;
//        lRetBCD=(lHex%10+(lHex%100)/10*0x10)*0x1000000+lRetBCD;
//    }
//    return lRetBCD;
//}

/******************************************************************************
函数: void buf_add_33(U8 buf[], U16 len)
入口: buf - 缓冲区首地址, len - 要加33的缓冲区长度
出口: 无
概述: 将buf指定缓冲区的len长度字节数据按无符号+33处理, 针对类似645协议
    没有进行指针有效性检查, 可增加调试宏代码检查之
******************************************************************************/
//void buf_add_33 (U8 buf[], U16 len)
//{
//    U16 i;
//
//    for(i = 0; i < len; i++)
//    {
//        buf[i] += 0x33;
//    }
//}

/******************************************************************************
** 函数名称: U8 compare_update_buf(U8 *pd, U8 *ps, U32 cnt)
** 函数功能: 比较两个数组对应字节是否相等,不等则覆盖更新到目标数组
** 入口参数: ps: 数据源指针
**           pd: 目标数据指针
**           cnt: 字节长度
** 返 回 值: 1: 一旦发现有任何1组对应字节不等,则返回 1,对应字节全部相等返回 0
** 说    明: 无
******************************************************************************/
U8 compare_update_buf (U8 *pd, U8 *ps, U32 cnt)
{
    U8 cpy_flg = 0;

    while(cnt)
    {
        if((*pd) == (*ps))
        {
            pd++;
            ps++;
            cnt--;
        }
        else                //一旦发现不等,则由此处以后全部覆盖更新
        {
            memcpy(pd, ps, 1);
            cpy_flg = 1;
        }
    }
    return cpy_flg;         //0:全部相等;1:至少有一个字节不同
}
/******************************************************************************
 Function name:  __task void debug_task (void)
 Author       :
 Description  :
 Input        :  None
 Return       :  None
 *****************************************************************************/
//__task void debug_task (void)
//{
////    U8 i;
////    U8 *p = (U8 *)&gVarArray;
////    U8 p_rev[6] ={0x88,0x88,0x88,0x88,0x88,0x88};
////    U32 obj_buff[10];
////    file_write(PAR_HVMADDR_04000401_00_ADDR, p_rev, 6);
////    U32 eng[2],dmd[2];
////    U64 tmp;
////    GPRS_POWER_EN();
////    IGT_ON();//拉低IGT
////    os_dly_wait(OS_DLY_500MS);
////    IGT_OFF();//拉高IGT
////    os_dly_wait(OS_DLY_10MS);
////    IGT_ON();//拉低IGT
////    os_dly_wait(OS_DLY_200MS);
////    IGT_OFF();
////    max = PAR_RESTDAY_04000801_00_ADDR;min=PAR_HVMADDR_04000401_01_ADDR;
////    tmp = VAR_RAM_ARRAY_MAX;
////    for(i=0; i< 100;i++)//初始化变量数据源，调试用
////    {
////        *p = i % 10;
////        p++;
////    }
////    eng[0] = ENG_RAM_ARRAY_MAX;
////    eng[1] = ENG_LASTXMONTH_00000000_00_OFFSET_BLOCKLEN;
////    dmd[0] = DMD_RAM_ARRAY_MAX;
////    dmd[1] = DMD_LASTXMONTH_01010001_00_OFFSET_BLOCKLEN;
////    send_far_infrared_data(Irda_Buf, 10);
//    for(;;)
//    {
////        os_dly_wait(OS_DLY_1S);
////        BEEP_ON_EN();
////        WGP_ON();
////        YGP_OFF();
////        GNET_LED_OFF();
////        DBG_LED_ON();
//
////        SET_PORT1_REV(P1_DBG_LED);
//
//        if(PORT0_IS_HIGH(P0_PD_CHK))           //非掉电工况,闪烁运行灯
//        {
//            SET_PORT1_REV(P1_CPU_YGP);
//        }
//
////        BAT_ON();
//        os_dly_wait(OS_DLY_500MS);
////        BEEP_ON_DIS();
////        WGP_OFF();
////        YGP_ON();
////        GNET_LED_ON();
////        DBG_LED_OFF();
////        BAT_OFF();
////        short_link_check();
////        record_event_open_cover();
////        file_read(PAR_PRI0PSWD_04000C01_00_ADDR, (U8 *)&obj_buff, 40);
////        wr_byte_to_infrared(0x55);
////        send_far_infrared_data(Irda_Buf, 10);
////        bat_charge_mange();
//
////        if(1 == get_far_infrared_data(Irda_Buf))
////        {
////            send_far_infrared_data(Irda_Buf + 1, Irda_Buf[0]);
////        }
//    }
//}

/******************************************************************************
    End Of File
******************************************************************************/

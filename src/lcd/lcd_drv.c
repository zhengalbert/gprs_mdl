/******************************************************************************
 Copyright (C) 2011  R&D Institute of HuaCai Co.,Ltd.
 File Name      : i2c1.c
 Description    : 段码是LCD驱动芯片 BU97950驱动程序
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2011-06-12      azh           the original version
******************************************************************************/
#include "..\inc\global_config.h"

#ifdef LCD_VALID
extern U8 lcd_seg_buf[LCD_BUF_LENTH];//azh 120313
//-------------------------------- 常数定义 -----------------------------------

//----------------------------- type definition -------------------------------
//0xLitSn,
    //B3~B0: 同一数据标识下单项数据拆分序号
    //B4~B7: 同一数据标识下多项数据的拆分序号

//0xSpecialCtrl
    //B1~B0
        //=0: EEPROM;
        //=1:RAM;
        //=2:EEPROM带指针偏移的
        //=3:保留
    //B3~B2
        //=0: 多字节转BCD,
        //=1: 单字节转BCD
        //=2: 单字节转换成8位
    //B4=0: 直接显示，=1:数据要转换格式显示
    // B6~B5: 保留
    //B7=0: 无符号数，=1:有符号数

//0xFixCharType,
    //固定汉字显示类型序号FixCharTable表格下标

//0xDataLen,
    //B3~B0:数据原始长度，B4~B7:要显示的8的个数+1

//0xOffsetTabSn,
    // 针对带偏移指针保存事件记录，事件记录因为是循环保存的，所以另外一张表格定义了事件偏移指针保存的地址及长度。

//-----------------------------------------
const U8 fixchar_setbit_segbuf[64] =
{
    B00_SEG03_COM7, //当前
    B01_SEG03_COM6, //上
    B02_SEG06_COM7, //月
    B03_SEG07_COM7, //组合
    B04_SEG08_COM7, //反
    B05_SEG09_COM5, //正
    B06_SEG09_COM6, //向
    B07_SEG09_COM7, //无

    B08_SEG10_COM6, //有
    B09_SEG10_COM7, //功
    B10_SEG11_COM7, //I1(象限用)
    B11_SEG11_COM6, //I2
    B12_SEG12_COM6, //I3
    B13_SEG12_COM7, //V2
    B14_SEG13_COM7, //总
    B15_SEG13_COM6, //尖

    B16_SEG30_COM7, //峰
    B17_SEG30_COM6, //平
    B18_SEG31_COM6, //谷
    B19_SEG06_COM6, //A相
    B20_SEG06_COM5, //B相
    B21_SEG06_COM4, //C相
    B22_SEG07_COM4, //N相
    B23_SEG07_COM5, //COS

    B24_SEG07_COM6, //角度Five
    B25_SEG08_COM6, //阶梯
    B26_SEG08_COM5, //剩余
    B27_SEG09_COM4, //需
    B28_SEG10_COM4, //电
    B29_SEG10_COM5, //量
    B30_SEG11_COM5, //费
    B31_SEG11_COM4, //价

    B32_SEG12_COM5, //失
    B33_SEG13_COM4, //压
    B34_SEG13_COM5, //流
    B35_SEG30_COM5, //功率
    B36_SEG30_COM4, //时
    B37_SEG31_COM4, //间
    B38_SEG31_COM5, //段
    B39_SEG02_COM3, //P2小数点8.8888888

    B40_SEG04_COM3, //P3小数点88.888888
    B41_SEG06_COM3, //P4小数点888.88888
    B42_SEG08_COM3, //P5小数点8888.8888
    B43_SEG10_COM3, //P6小数点88888.888
    B44_SEG12_COM3, //P7小数点888888.88
    B45_SEG30_COM3, //P8小数点8888888.8
    B46_SEG27_COM7, //万
    B47_SEG26_COM7, //元

    B48_SEG29_COM7, //kV //kWAh中的段
    B49_SEG28_COM7, //V1
    B50_SEG27_COM6, //A1
    B51_SEG26_COM6, //h2//
    B52_SEG29_COM6, //kvar //kvarh中的段
    B53_SEG28_COM6, //h1
    B54_SEG08_COM4, //col1//时间上的小数点
    B55_SEG12_COM4  //col2
};
static const U16 lcd_8_char[16] =
{
    LCD_CHAR_0, LCD_CHAR_1, LCD_CHAR_2, LCD_CHAR_3, LCD_CHAR_4,
    LCD_CHAR_5, LCD_CHAR_6, LCD_CHAR_7, LCD_CHAR_8, LCD_CHAR_9,
    LCD_CHAR_A, LCD_CHAR_b, LCD_CHAR_C, LCD_CHAR_d, LCD_CHAR_E, LCD_CHAR_F
};
static const U16 lcd_err_char[3] =
{
    LCD_CHAR_E, LCD_CHAR_r, LCD_CHAR_r
};
static const U16 lcd_search_char[8] =
{
    LCD_CHAR_S, LCD_CHAR_E, LCD_CHAR_A, LCD_CHAR_r, LCD_CHAR_c, LCD_CHAR_h, LCD_CHAR__, LCD_CHAR__
};
//SET-Err
static const U16 lcd_seterr_char[7] =
{
    LCD_CHAR_S, LCD_CHAR_E, LCD_CHAR_T, LCD_CHAR__, LCD_CHAR_E, LCD_CHAR_r, LCD_CHAR_r
};
//SET-Good
static const U16 lcd_setgood_char[8] =
{
    LCD_CHAR_S, LCD_CHAR_E, LCD_CHAR_T, LCD_CHAR__, LCD_CHAR_G, LCD_CHAR_o, LCD_CHAR_o, LCD_CHAR_d
};
//数据区8在显示全局缓冲区(16bit)中的位置
static const U8 lcd_b8seg_index[8] =
{
    30, 12, 10, 8, 6, 4, 2, 0
};
//显示编码区8在显示全局缓冲区(16bit)中的位置
static const U8 lcd_l8seg_index[8] =
{
    28, 26, 24, 22, 20, 18, 16, 14
};
const U8 flashchar_setbit_segbuf[CON_FLASH_BIT_MAX_NUM] =
{
    FLASH_SEG23_COM4,   //N9 时钟电池
    FLASH_SEG23_COM5,   //N10 停电抄表电池
    FLASH_SEG29_COM4,   //Ua
    FLASH_SEG27_COM4,   //Ia
    FLASH_SEG29_COM5,   //Ub
    FLASH_SEG26_COM4,   //Ib
    FLASH_SEG28_COM5,   //Uc
    FLASH_SEG25_COM4,   //Ic
    FLASH_SEG28_COM4,   //逆相序
};
static const U8 lcd_sign_char[9] =
{
    SEG_SIGN_0, SEG_SIGN_1, SEG_SIGN_2, SEG_SIGN_3, SEG_SIGN_4,
    SEG_SIGN_5, SEG_SIGN_6, SEG_SIGN_7, SEG_SIGN_8
};
//------------------------------ 私有变量声明 ---------------------------------

//------------------------------ 私有函数声明 ---------------------------------
//static S32 lcd_read_data(U32 obj_addr, U8 *lcdbuf, U16 len);

/******************************************************************************
** 函数名称: void lcd_on_all_dis (void)
** 函数功能: initial bu97950 用于调试显示
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void lcd_on_all_dis (void)
{
    U8 i;
//    S8 err;
    U16 len;
    U32 lcd_addr;

    len = 0;
    lcd_addr = BU97950_HEAD_ADDR;
    lcd_seg_buf[len++] = ICSET1;
    lcd_seg_buf[len++] = DISCTL0;//DISCTL22;
//  lcd_seg_buf[len++] = EVRSET;
    lcd_seg_buf[len++] = ADSET;

    for(i=0;i<32;i++)
    {
        lcd_seg_buf[len++] = 0xff;              //清空RAM;
    }

    lcd_write_command(lcd_addr, lcd_seg_buf, len);
}
/******************************************************************************
** 函数名称: void lcd_pd_mode (void)
** 函数功能: CPU掉电模式下 让注意驱动也进入掉电模式
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void lcd_pd_mode (void)
{
    U8 lcd_buf[6];
    U8 len;
    U32 lcd_addr;

    len = 0;
    lcd_addr = BU97950_HEAD_ADDR;
    lcd_buf[len++] = ICSET1;
    lcd_buf[len++] = DISCTL30;

    lcd_write_command(lcd_addr, lcd_buf, len);

}
/******************************************************************************
** 函数名称: void lcd_dis_off (void)
** 函数功能: CPU掉电模式下 关显示
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void lcd_dis_off (void)
{
    U8 lcd_buf[2];
    U8 len;
    U32 lcd_addr;

    len = 0;
    lcd_addr = BU97950_HEAD_ADDR;
    lcd_buf[len++] = ICSET;//APCTL1;

    lcd_write_command(lcd_addr, lcd_buf, len);

}
/******************************************************************************
** 函数名称: void lcd_reset (void)
** 函数功能: initial bu97950
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void lcd_reset (void)
{
    U8 i;
//    S8 err;
    U16 len;
    U32 lcd_addr;

    len = 0;
    lcd_addr = BU97950_HEAD_ADDR;
    lcd_seg_buf[len++] = ICSET;
    lcd_seg_buf[len++] = DISCTL0;
    lcd_seg_buf[len++] = ADSET;

    for(i=0;i<32;i++)
    {
        lcd_seg_buf[len++] = 0x00;              //清空RAM;
    }

    lcd_write_command(lcd_addr, lcd_seg_buf, len);
}

/******************************************************************************
** 函数名称: S32 lcd_write_command (U32 obj_addr, U8 *lcdbuf, U8 len)
** 函数功能: write cmd to bu97950
** 入口参数: obj_addr : object logic adrress
**           *lcdbuf  : pointer for the receive buffer
**           len      : length of bytes for operation
** 返 回 值: indicating the result of the operation
**              1: normal
**             -1: length overflow, not allowed
**             -2: object memory not found
**             -3: waiting time overflow
**        -others: state error
** 说    明: 无
******************************************************************************/
S32 lcd_write_command (U32 obj_addr, U8 *lcdbuf, U16 len)
{
    S32 ret;

    ret = write_i2c0(obj_addr, lcdbuf, len);
    if((ret != 1) && (CON_CPU_NORM_MODE == cpu_run_mode()))//azh 130120
    {
        Alarm_EnsureJudge_TerErr_Display();
    }

    return ret;
}

/******************************************************************************
** 函数名称: S32 lcd_write_data (U32 obj_addr, U8 *lcdbuf, U8 len)
** 函数功能: write data from bu97950
** 入口参数: obj_addr : object logic adrress
**           *lcdbuf  : pointer for the receive buffer
**           len      : length of bytes for operation
** 返 回 值: indicating the result of the operation
**              1: normal
**             -1: length overflow, not allowed
**             -2: object memory not found
**             -3: waiting time overflow
**        -others: state error
** 说    明: 无
******************************************************************************/
S32 lcd_write_data (U32 obj_addr, U8 *lcdbuf, U16 len)
{
    S32 ret;

    ret = write_i2c0(obj_addr, lcdbuf, len);

    //if(ret == 1)
    if((ret == 1) && (CON_CPU_NORM_MODE == cpu_run_mode()))//120308要在有电的情况下 要不就进hardfault异常了
    {
        set_normal_run_flag(1 << 2);                    //升级有效标识检查之一显示通信正常确认
    }
    return ret;
}

/******************************************************************************
** 函数名称: S32 lcd_read_data (U32 obj_addr, U8 *lcdbuf, U8 len)
** 函数功能: read data from bu97950
** 入口参数: obj_addr : object logic adrress
**           *lcdbuf  : pointer for the receive buffer
**           len      : length of bytes for operation
** 返 回 值: indicating the result of the operation
**              1: normal
**             -1: length overflow, not allowed
**             -2: object memory not found
**             -3: waiting time overflow
**        -others: state error
** 说    明: 无
******************************************************************************/
/*
S32 lcd_read_data (U32 obj_addr, U8 *lcdbuf, U16 len)
{
    S32 ret;

    ret = read_i2c0(obj_addr, lcdbuf, len);

    return ret;
}
*/

/******************************************************************************
** 函数名称: void fill_data_segbuf(U8 *datbuf, U8 len, U8 *segbuf)
** 函数功能: 把要显示数据存放到全局显示RAM
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void fill_data_b8segbuf (U8 *datbuf, U8 len, U8 *segbuf)
{
    U8 i;
    U8 data;

    data = 0;
    if(len > 8) len = 8;
    for(i=0; i<len; i++)
    {
        if((i%2) == 0) data = datbuf[i/2];
        segbuf[lcd_b8seg_index[i]] |= lcd_8_char[data&0x0f];
        segbuf[lcd_b8seg_index[i]+1] |= (lcd_8_char[data&0x0f]>>8);
        data >>= 4;
    }
}
/******************************************************************************
** 函数名称: void fill_err_b8segbuf(U8 *datbuf, U8 len, U8 *segbuf)
** 函数功能: 把要显示数据存放到全局显示RAM
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void fill_err_b8segbuf (U8 *datbuf, U8 len, U8 *segbuf)
{
    U8 i;

    if(len > 4) len = 4;
    fill_data_b8segbuf(datbuf, len, segbuf);
    for(i=0; i<3; i++)//Err
    {
        segbuf[lcd_b8seg_index[3-i-1+len]] |= lcd_err_char[i];
        segbuf[lcd_b8seg_index[3-i-1+len]+1] |= (lcd_err_char[i]>>8);
    }
}
/******************************************************************************
** 函数名称: void fill_special_b8segbuf(U8 index, U8 *segbuf)
** 函数功能: 1:SEArch-- 2:"SET-Good" 3:SET-Err
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void fill_special_b8segbuf(U8 index, U8 *segbuf)
{
    U8 i;

    switch(index)
    {
        case 1:
        {
            for(i=0; i<8; i++)//SEArch--
            {
                segbuf[lcd_b8seg_index[8-i-1]] |= lcd_search_char[i];
                segbuf[lcd_b8seg_index[8-i-1]+1] |= (lcd_search_char[i]>>8);
            }
            break;
        }
        case 2:
        {
            for(i=0; i<8; i++)//SET-Good
            {
                segbuf[lcd_b8seg_index[8-i-1]] |= lcd_setgood_char[i];
                segbuf[lcd_b8seg_index[8-i-1]+1] |= (lcd_setgood_char[i]>>8);
            }
            break;
        }
        case 3:
        {
            for(i=0; i<7; i++)//SET-Err
            {
                segbuf[lcd_b8seg_index[7-i-1]] |= lcd_seterr_char[i];
                segbuf[lcd_b8seg_index[7-i-1]+1] |= (lcd_seterr_char[i]>>8);
            }
            break;
        }
    }
}
/******************************************************************************
** 函数名称: void fill_data_l8segbuf(U8 *datbuf, U8 len, U8 *segbuf)
** 函数功能: 把要显示编码存放到全局显示RAM
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void fill_data_l8segbuf (U8 *datbuf, U8 len, U8 *segbuf)
{
    U8 i;
    U8 data;

    data = 0;
    i = 0;
    if(i > 8) i = 8;
    for(i=0; i<len; i++)
    {
        if((i%2) == 0) data = datbuf[i/2];
        segbuf[lcd_l8seg_index[i]] |= lcd_8_char[data&0x0f];
        segbuf[lcd_l8seg_index[i]+1] |= (lcd_8_char[data&0x0f]>>8);
        data >>= 4;
    }
    //if 自动轮显 则要显示几个点 并且显示为轮显编码
    if(Dis_Ctrl.cur_mode == DIS_MODE_AUTO)
    {
        segbuf[SPE_SEG18_COM3 / 8] |= 1 << (SPE_SEG18_COM3 % 8);
        segbuf[SPE_SEG22_COM3 / 8] |= 1 << (SPE_SEG22_COM3 % 8);
        segbuf[SPE_SEG26_COM3 / 8] |= 1 << (SPE_SEG26_COM3 % 8);
    }

    //if 键显 则不要显示小数点 并且显示为序号
}
/******************************************************************************
** 函数名称: void fill_data_last8_segbuf(U8 num, U8 *segbuf)
** 函数功能: 把要最面的上xx,显示数据存放到全局显示RAM
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void fill_data_last8_segbuf (U8 num, U8 *segbuf)
{
    num = HEX_TO_BCD(num, 1);
    segbuf[4] |= (lcd_8_char[num&0x0f]>>4);//多移4位因为和其他的8显示不同
    segbuf[5] |= (lcd_8_char[num&0x0f]>>12);
    num >>= 4;
    if(num >=1 )
    {
        segbuf[4] |= 1;//0BC seg
    }
}
/******************************************************************************
** 函数名称: void fill_data_d88_segbuf(U8 data, U8 *segbuf)
** 函数功能: 把左下角的二个88显示数据存放到全局显示RAM
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void fill_data_d88_segbuf (U8 data, U8 *segbuf)
{
    data = HEX_TO_BCD(data, 1);

    segbuf[16] |= (lcd_8_char[data&0x0f]>>4);//多移4位因为和其他的8显示不同
    segbuf[17] |= (lcd_8_char[data&0x0f]>>12);
    data >>= 4;
    segbuf[14] |= (lcd_8_char[data&0x0f]>>4);
    segbuf[15] |= (lcd_8_char[data&0x0f]>>12);
}
/******************************************************************************
** 函数名称: S8 fill_dis_fix_char(U8 disfixsn, U8 *segbuf)
** 函数功能: 通过固定字符显示表格序号,填写显示缓冲区
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
S8 fill_dis_fix_char (U8 *inbuf, U8 *segbuf)
{
    U8 i;
    U8 sn;
    U8 b;
    U8 data;

    for(i=0; i< 8; i++)
    {
        if(inbuf[i] > 0)//非零
        {
            data = inbuf[i];
            for(b=0; b<8; b++)
            {
                if((data&(1<<b)) > 0)
                {
                    sn = i*8 + b;
                    if(sn>=64)
                    {
                        break;
                    }
                    sn = fixchar_setbit_segbuf[sn];//把表格中置起来的位转成全局SEGBUF中的位序号
                    segbuf[0 + sn / 8] |= 1 << (sn % 8);
                }
            }
        }
    }
    return __TRUE;
}
/******************************************************************************
** 函数名称: void fill_dis_flash_flag (U8 *segbuf)
** 函数功能: 根据闪烁标志 填相关显示缓冲
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void fill_dis_flash_flag (U8 *segbuf)
{
    U8 i;
    U8 sn;

    for(i=0; i< CON_FLASH_BIT_MAX_NUM; i++)
    {
        sn = flashchar_setbit_segbuf[i];//把标志状态中置起来的位转成全局SEGBUF中的位序号
        if((gState_Flash_Flag & (1<<i)) > 0)//非零
        {
            segbuf[sn / 8] |= 1 << (sn % 8);
            if((sn == FLASH_SEG27_COM4) && (gMeter_State[3] & 0x40))//A相电流反向
            {
                segbuf[FLASH_SEG27_COM5 / 8] |= 1 << (FLASH_SEG27_COM5 % 8);
            }
            else if((sn == FLASH_SEG26_COM4) && (gMeter_State[4] & 0x40))//B相电流反向
            {
                segbuf[FLASH_SEG26_COM5 / 8] |= 1 << (FLASH_SEG26_COM5 % 8);
            }
            else if((sn == FLASH_SEG25_COM4) && (gMeter_State[5] & 0x40))//C相电流反向
            {
                segbuf[FLASH_SEG25_COM5 / 8] |= 1 << (FLASH_SEG25_COM5 % 8);
            }
        }
        else
        {
            segbuf[sn / 8] &= ~(1 << (sn % 8));
            if((sn == FLASH_SEG27_COM4) && (gMeter_State[3] & 0x40))//A相电流反向
            {
                segbuf[FLASH_SEG27_COM5 / 8] &= ~(1 << (FLASH_SEG27_COM5 % 8));
            }
            else if((sn == FLASH_SEG26_COM4) && (gMeter_State[4] & 0x40))//B相电流反向
            {
                segbuf[FLASH_SEG26_COM5 / 8] &= ~(1 << (FLASH_SEG26_COM5 % 8));
            }
            else if((sn == FLASH_SEG25_COM4) && (gMeter_State[5] & 0x40))//C相电流反向
            {
                segbuf[FLASH_SEG25_COM5 / 8] &= ~(1 << (FLASH_SEG25_COM5 % 8));
            }
        }
    }
    if(gState_Com_Flag & CON_COM_RS4851_FLASH_BIT)//电话机1 对应rs485_1通信状态
    {
        segbuf[PHONE1_SEG20_COM5 / 8] |= 1 << (PHONE1_SEG20_COM5 % 8);
    }
    else
    {
        segbuf[PHONE1_SEG20_COM5 / 8] &= ~(1 << (PHONE1_SEG20_COM5 % 8));
    }
    if(gState_Com_Flag & CON_COM_RS4852_FLASH_BIT)//电话机2 对应rs485_2通信状态
    {
        segbuf[PHONE2_SEG20_COM4 / 8] |= 1 << (PHONE2_SEG20_COM4 % 8);
    }
    else
    {
        segbuf[PHONE2_SEG20_COM4 / 8] &= ~(1 << (PHONE2_SEG20_COM4 % 8));
    }
    //电话符号是共用的 就是用上面的 1 2区别二路485
    if((gState_Com_Flag & CON_COM_RS4851_FLASH_BIT) || (gState_Com_Flag & CON_COM_RS4852_FLASH_BIT))
    {
        segbuf[PHONE_SEG20_COM6 / 8] |= 1 << (PHONE_SEG20_COM6 % 8);
    }
    else
    {
        segbuf[PHONE_SEG20_COM6 / 8] &= ~(1 << (PHONE_SEG20_COM6 % 8));
    }

//azh 120314 增加电池1充电电池 及电池2停显电池的闪烁
//电池状态：0：正常(灭) 1：欠压(闪烁) 2：没加电池(亮)
    if(gState_Com_Flag & CON_COM_GPRS_BAT_BIT)//电池1 充电电池状态
    {
        segbuf[FLASH_SEG23_COM4 / 8] |= 1 << (FLASH_SEG23_COM4 % 8);
    }
    else
    {
        segbuf[FLASH_SEG23_COM4 / 8] &= ~(1 << (FLASH_SEG23_COM4 % 8));
    }
    if(gState_Com_Flag & CON_COM_DIS_BAT_BIT)//电话机2 对应rs485_2通信状态
    {
        segbuf[FLASH_SEG23_COM5 / 8] |= 1 << (FLASH_SEG23_COM5 % 8);
    }
    else
    {
        segbuf[FLASH_SEG23_COM5 / 8] &= ~(1 << (FLASH_SEG23_COM5 % 8));
    }
}
/******************************************************************************
** 函数名称: S8 fill_dis_special_fix_char(U8 *segbuf)
** 函数功能:
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
S8 fill_dis_special_fix_char (U8 *segbuf)
{
    U8 flag;

    segbuf[SPE_SEG01_COM6 / 8] |= (1 << (SPE_SEG01_COM6 % 8));//cor 象限十字
    //根据电表状态字相关位
    //有、无功功率方向确定象限
    //Bit5:无功功率方向 Bit4:有功功率方向 (0正向、1反向)
    flag = ((gMeter_State[0] >> 4) & 0x03);
    switch(flag)
    {
        case 0://一象限
            segbuf[COR1_SEG02_COM7 / 8] |= (1 << (COR1_SEG02_COM7 % 8));
            break;
        case 1://二象限
            segbuf[COR2_SEG01_COM7 / 8] |= (1 << (COR2_SEG01_COM7 % 8));
            break;
        case 2://四象限
            segbuf[COR3_SEG01_COM5 / 8] |= (1 << (COR3_SEG01_COM5 % 8));
            break;
        case 3://三象限
            segbuf[COR4_SEG02_COM6 / 8] |= (1 << (COR4_SEG02_COM6 % 8));
            break;
    }
    //实时费率 在电表状态字3 的bit7 bit6 bit5
    //flag = ((gMeter_State[2] >> 5) & 0x07);
    flag = (gMeter_State[3] & 0x07);
    if(flag < 4)
    {
        switch(flag)
        {
            case 0://尖
                segbuf[RATE1_SEG00_COM7 / 8] |= (1 << (RATE1_SEG00_COM7 % 8));
                break;
            case 1://峰
                segbuf[RATE2_SEG00_COM6 / 8] |= (1 << (RATE2_SEG00_COM6 % 8));
                break;
            case 2://平
                segbuf[RATE3_SEG18_COM4 / 8] |= (1 << (RATE3_SEG18_COM4 % 8));
                break;
            case 3://谷
                segbuf[RATE4_SEG01_COM4 / 8] |= (1 << (RATE4_SEG01_COM4 % 8));
                break;
        }
    }
    //电表状态字3的bit0:当前运行时段表 (0第一套，1第二套)
    if(gMeter_State[2] & 0x01)//第二套
    {
        segbuf[SPE_SEG24_COM4 / 8] |= (1 << (SPE_SEG24_COM4 % 8));
    }
    else//第一套
    {
        segbuf[SPE_SEG24_COM5 / 8] |= (1 << (SPE_SEG24_COM5 % 8));
    }
//编程键状态显示
    if(__TRUE == prog_key_enable())
    {
        segbuf[PROG_SEG21_COM4 / 8] |= (1 << (PROG_SEG21_COM4 % 8));
    }
    return __TRUE;
}
/******************************************************************************
** 函数名称: void fill_data_sign(U8 index, U8 *segbuf)
** 函数功能: 填写数据的符号位
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void fill_data_sign (U8 index, U8 *segbuf)
{
    segbuf[lcd_sign_char[index] / 8] |= (1 << (lcd_sign_char[index] % 8));//符号位"-"
}
/******************************************************************************
** 函数名称: void fill_dis_ssn_l8_data(U8 sn, U8 *segbuf)
** 函数功能: 小8区高4位填成S01-
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void fill_dis_ssn_l8_data(U8 sn, U8 *segbuf)
{
//S
    segbuf[lcd_l8seg_index[7]] |= LCD_CHAR_S;
    segbuf[lcd_l8seg_index[7]+1] |= (LCD_CHAR_S>>8);
//sn
    segbuf[lcd_l8seg_index[5]] |= lcd_8_char[sn&0x0f];
    segbuf[lcd_l8seg_index[5]+1] |= (lcd_8_char[sn&0x0f]>>8);
    sn >>= 4;
    segbuf[lcd_l8seg_index[6]] |= lcd_8_char[sn&0x0f];
    segbuf[lcd_l8seg_index[6]+1] |= (lcd_8_char[sn&0x0f]>>8);
//-
    segbuf[lcd_l8seg_index[4]] |= LCD_CHAR__;
    segbuf[lcd_l8seg_index[4]+1] |= (LCD_CHAR__>>8);
}
#endif//LCD_VALID
/******************************************************************************
    End Of File
******************************************************************************/

/******************************************************************************
 Copyright (C) 2011  R&D Institute of HuaCai., Ltd.
 Module         : lcd display function
 File Name      : lcd_display.c
 Description    :
 Author         :
 Build Date     :
 others         :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、     1.00       2011-05-09     azh       create
******************************************************************************/

/*---------------------------------------------------------------------------*/
#include    "..\inc\global_config.h"
#include    "..\src\bsp\event_define_d.h"               //有关事件宏的定义

#ifdef LCD_VALID
#define DATA_SOURCE_NOT_LOCAL//如果没有定义在全都是取本地EEPROM里的数据

//------------------------------ 私有变量声明 ---------------------------------
U8 lcd_seg_buf[LCD_BUF_LENTH];   //nbytes CMD + seg0~seg31


/*private functions defined in this file:------------------------------------*/
static S8 fill_dis_fix_b8_data(U32 di, DIS_CTRL_TCB *pdistable, U8 *segbuf);
static S8 fill_dis_fix_l8_data(U32 di, U8 *segbuf);
static U16 get_dis_sn_bydi (U32 di, U8 lsn, DIS_CTRL_TCB const *pdistable, U16 max_num);
static S8 get_dis_info (U8 *di_buf, U8 *fix_data, DIS_CTRL_TCB *pdis_tcb, U8 *segbuf);

void get_dis_code(U8 mode);
static U8 judge_data_sign(U8 *buf, DIS_CTRL_TCB *pdistable);
static void lcd_backlight_ctrl(U8 mode);
static void fill_search_addr_segbuf(U8 *segbuf);
static void fill_search_set_state_segbuf(U8 *segbuf);
//gprs 信号强度表格
const U8 gprs_signal_dis_tab[6][5] =
{
    0, 0, 0, 0, 0,   //无信号
    SIGNAL_DIS_I1, 0, 0, 0, 0,   //1格信号
    SIGNAL_DIS_I1, SIGNAL_DIS_I2, 0, 0, 0,   //2格信号
    SIGNAL_DIS_I1, SIGNAL_DIS_I2, SIGNAL_DIS_I3, 0, 0,   //3格信号
    SIGNAL_DIS_I1, SIGNAL_DIS_I2, SIGNAL_DIS_I3, SIGNAL_DIS_I4, 0,   //4格信号
    SIGNAL_DIS_I1, SIGNAL_DIS_I2, SIGNAL_DIS_I3, SIGNAL_DIS_I4, SIGNAL_DIS_I5,   //5格信号
};
//7个电表状态字 需要判断闪烁的状态位
//在7个状态字中的总的位序号
const U8 meter_state_flash_bit_tab[CON_FLASH_BIT_MAX_NUM] =
{
    2, 3,   //时钟电池欠压 抄表电池欠压
    48, 51, //A相失压 失流
    64, 67, //B相失压 失流
    80, 83, //C相失压 失流
    96,     //97  //电压 电流逆相序 这里是共用一个显示字符逆相序
};
const U8 meter_state_flash_bit_spectab[CON_FLASH_BIT_MAX_NUM] =
{
    0,  0,
    55, 56, //A相断相 断流
    71, 72, //B相断相 断流
    87, 88, //C相断相 断流
    0,
};
//默认或正常状态显示情况 0：不显示 1：显示
const U8 flash_bit_default_value_tab[CON_FLASH_BIT_MAX_NUM] =
{
    0, 0,         //时钟电池欠压 抄表电池欠压 默认或正常状态不显示
    0x81, 0x81,   //A相失压 失流 默认或正常状态显示 最高位置1说明还得判断一个状态才能确定 如断相
    0x81, 0x81,   //B相失压 失流 默认或正常状态显示
    0x81, 0x81,   //C相失压 失流 默认或正常状态显示
    0,            //电压 电流逆相序 默认或正常状态不显示
};
const DIS_CTRL_TCB disctrbydi_spec_di304table[2] =
{
    {//04000401
        0x04000401, //byte 0  0xDI3DI2DI1XX 标识
        PAR_HVMADDR_04000401_00_ADDR, //byte 1  0xAdr3Adr2Adr1Adr0 宏名字
        0x00, //byte 2 0xLitSn 多项数据的拆分序号  单项数据拆分序号
        0x00, //byte 3 0xSpecialCtrl
        0x0000, // byte 4 0xFixCharType 固定汉字显示类型序号FixCharTable表格下标
         ((PAR_HVMADDR_04000401_00_LEN*2) << 4) + PAR_HVMADDR_04000401_00_LEN,//byte 5 0xDataLen 宏名字
        0xff, //byte 6 0xOffsetTabSn

    },
    {//04000401
        0x04000401, //byte 0  0xDI3DI2DI1XX 标识
        PAR_HVMADDR_04000401_01_ADDR, //byte 1  0xAdr3Adr2Adr1Adr0 宏名字
        0x01, //byte 2 0xLitSn 多项数据的拆分序号  单项数据拆分序号
        0x00, //byte 3 0xSpecialCtrl
        0x0000, // byte 4 0xFixCharType 固定汉字显示类型序号FixCharTable表格下标
         ((PAR_HVMADDR_04000401_01_LEN*2) << 4) + PAR_HVMADDR_04000401_01_LEN,//byte 5 0xDataLen 宏名字
        0xff, //byte 6 0xOffsetTabSn

    },
};
//5字节 小序号在最高字节
const U8 default_auto_dis_tab[DEFAULT_AUTO_DIS_MAX_NUM][5] =
{
    0x01, 0x01, 0x00, 0x04, 0x00,   //0x04000101 00 当前日期
    0x02, 0x01, 0x00, 0x04, 0x00,   //0x04000102 00 当前时间

    0x00, 0x00, 0x00, 0x00, 0x00,   //0x00000000 00 当前组合有功总电量
    0x00, 0x00, 0x01, 0x00, 0x00,   //0x00010000 00 当前正向有功总电量
    0x00, 0x01, 0x01, 0x00, 0x00,   //0x00010100 00 当前正向有功尖电量
    0x00, 0x02, 0x01, 0x00, 0x00,   //0x00010200 00 当前正向有功峰电量
    0x00, 0x03, 0x01, 0x00, 0x00,   //0x00010300 00 当前正向有功平电量
    0x00, 0x04, 0x01, 0x00, 0x00,   //0x00010400 00 当前正向有功谷电量

    0x00, 0x00, 0x01, 0x01, 0x00,   //0x01010000 00 当前正向有功总最大需量

    0x00, 0x00, 0x05, 0x00, 0x00,   //0x00050000 00 当前第1象限无功总电量
    0x00, 0x00, 0x06, 0x00, 0x00,   //0x00060000 00 当前第2象限无功总电量
    0x00, 0x00, 0x07, 0x00, 0x00,   //0x00070000 00 当前第3象限无功总电量
    0x00, 0x00, 0x08, 0x00, 0x00,   //0x00080000 00 当前第4象限无功总电量
    0x00, 0x00, 0x02, 0x00, 0x00,   //0x00020000 00 当前反向有功总电量
    0x00, 0x01, 0x02, 0x00, 0x00,   //0x00020100 00 当前反向有功尖电量
    0x00, 0x02, 0x02, 0x00, 0x00,   //0x00020200 00 当前反向有功峰电量
    0x00, 0x03, 0x02, 0x00, 0x00,   //0x00020300 00 当前反向有功平电量
    0x00, 0x04, 0x02, 0x00, 0x00,   //0x00020400 00 当前反向有功谷电量

};
#if CODE_VER_TYPE==WLS_CUR_PRJ			//配合无线采集器
//固定显示下面几屏（无线采集器版本）
const U8 default_key_dis_tab[DEFAULT_KEY_DIS_MAX_NUM][5] =
{
    0x01, 0x01, 0x00, 0x04, 0x00,    //0x04000101 00 当前日期
    0x02, 0x01, 0x00, 0x04, 0x00,    //0x04000102 00 当前时间

//无线采集器显示数据
    0x01, 0x04, 0x00, 0x04, 0x00,   //0x04000401 00 电能表通信地址（表号）低8位

    0x00, 0x01, 0x02, 0x02, 0x00,   //0x02020100 00 A相电流
    0x00, 0x02, 0x02, 0x02, 0x00,   //0x02020200 00 B相电流
    0x00, 0x03, 0x02, 0x02, 0x00,   //0x02020300 00 C相电流
    0x00, 0x00, 0x05, 0x02, 0x00,   //0x02050000 00 瞬时总视在功率

//监听总表显示数据
    0x01, 0x04, 0x00, 0x04, 0x01,   //0x04000401 01 电能表通信地址（表号）高4位
    0x01, 0x04, 0x00, 0x04, 0x00,   //0x04000401 00 电能表通信地址（表号）低8位
//    0x00, 0x00, 0x00, 0x00, 0x00,    //0x00000000 00 当前组合有功总电量
    0x00, 0x00, 0x01, 0x00, 0x00,    //0x00010000 00 当前正向有功总电量

    0x00, 0x00, 0x02, 0x00, 0x00,   //0x00020000 00 当前反向有功总电量

    0x00, 0x00, 0x03, 0x00, 0x00,   //0x00030000 00 当前组合无功1总电量//正向无功总 反向无功总
    0x00, 0x00, 0x04, 0x00, 0x00,   //0x00040000 00 当前组合无功2总电量

//    0x00, 0x00, 0x05, 0x00, 0x00,   //0x00050000 00 当前第1象限无功总电量
//    0x00, 0x00, 0x06, 0x00, 0x00,   //0x00060000 00 当前第2象限无功总电量
//    0x00, 0x00, 0x07, 0x00, 0x00,   //0x00070000 00 当前第3象限无功总电量
//    0x00, 0x00, 0x08, 0x00, 0x00,   //0x00080000 00 当前第4象限无功总电量

    0x00, 0x01, 0x01, 0x02, 0x00,   //0x02010100 00 A相电压
    0x00, 0x02, 0x01, 0x02, 0x00,   //0x02010200 00 B相电压
    0x00, 0x03, 0x01, 0x02, 0x00,   //0x02010300 00 C相电压
    0x00, 0x01, 0x02, 0x02, 0x00,   //0x02020100 00 A相电流
    0x00, 0x02, 0x02, 0x02, 0x00,   //0x02020200 00 B相电流
    0x00, 0x03, 0x02, 0x02, 0x00,   //0x02020300 00 C相电流

    0x00, 0x00, 0x03, 0x02, 0x00,   //0x02030000 00 瞬时总有功功率
    0x00, 0x00, 0x04, 0x02, 0x00,   //0x02040000 00 瞬时总无功功率
};


#else
const U8 default_key_dis_tab[DEFAULT_KEY_DIS_MAX_NUM][5] =
{
    0x01, 0x01, 0x00, 0x04, 0x00,    //0x04000101 00 当前日期
    0x02, 0x01, 0x00, 0x04, 0x00,    //0x04000102 00 当前时间

    0x00, 0x00, 0x00, 0x00, 0x00,    //0x00000000 00 当前组合有功总电量
    0x00, 0x00, 0x01, 0x00, 0x00,    //0x00010000 00 当前正向有功总电量
    0x00, 0x01, 0x01, 0x00, 0x00,    //0x00010100 00 当前正向有功尖电量
    0x00, 0x02, 0x01, 0x00, 0x00,    //0x00010200 00 当前正向有功峰电量
    0x00, 0x03, 0x01, 0x00, 0x00,    //0x00010300 00 当前正向有功平电量
    0x00, 0x04, 0x01, 0x00, 0x00,    //0x00010400 00 当前正向有功谷电量

    0x00, 0x00, 0x01, 0x01, 0x00,   //0x01010000 00 当前正向有功总最大需量
    0x00, 0x00, 0x01, 0x01, 0x02,   //0x01010000 01 当前正向有功总最大需量发生日期
    0x00, 0x00, 0x01, 0x01, 0x01,   //0x01010000 02 当前正向有功总最大需量发生时间

    0x00, 0x00, 0x02, 0x00, 0x00,   //0x00020000 00 当前反向有功总电量
    0x00, 0x01, 0x02, 0x00, 0x00,   //0x00020100 00 当前反向有功尖电量
    0x00, 0x02, 0x02, 0x00, 0x00,   //0x00020200 00 当前反向有功峰电量
    0x00, 0x03, 0x02, 0x00, 0x00,   //0x00020300 00 当前反向有功平电量
    0x00, 0x04, 0x02, 0x00, 0x00,   //0x00020400 00 当前反向有功谷电量

    0x00, 0x00, 0x02, 0x01, 0x00,   //0x01020000 00 当前反向有功总最大需量
    0x00, 0x00, 0x02, 0x01, 0x02,   //0x01020000 01 当前反向有功总最大需量发生日期
    0x00, 0x00, 0x02, 0x01, 0x01,   //0x01020000 02 当前反向有功总最大需量发生时间

    0x00, 0x00, 0x05, 0x00, 0x00,   //0x00050000 00 当前第1象限无功总电量
    0x00, 0x00, 0x06, 0x00, 0x00,   //0x00060000 00 当前第2象限无功总电量
    0x00, 0x00, 0x07, 0x00, 0x00,   //0x00070000 00 当前第3象限无功总电量
    0x00, 0x00, 0x08, 0x00, 0x00,   //0x00080000 00 当前第4象限无功总电量

    0x01, 0x00, 0x01, 0x00, 0x00,   //0x00010001 00 上1月正向有功总电量
    0x01, 0x01, 0x01, 0x00, 0x00,   //0x00010101 00 上1月正向有功尖电量
    0x01, 0x02, 0x01, 0x00, 0x00,   //0x00010201 00 上1月正向有功峰电量
    0x01, 0x03, 0x01, 0x00, 0x00,   //0x00010301 00 上1月正向有功平电量
    0x01, 0x04, 0x01, 0x00, 0x00,   //0x00010401 00 上1月正向有功谷电量

    0x01, 0x00, 0x01, 0x01, 0x00,   //0x01010001 00 上1月正向有功总最大需量
    0x01, 0x00, 0x01, 0x01, 0x02,   //0x01010001 01 上1月正向有功总最大需量发生日期
    0x01, 0x00, 0x01, 0x01, 0x01,   //0x01010001 02 上1月正向有功总最大需量发生时间

    0x01, 0x00, 0x02, 0x00, 0x00,   //0x00020001 00 上1月反向有功总电量
    0x01, 0x01, 0x02, 0x00, 0x00,   //0x00020101 00 上1月反向有功尖电量
    0x01, 0x02, 0x02, 0x00, 0x00,   //0x00020201 00 上1月反向有功峰电量
    0x01, 0x03, 0x02, 0x00, 0x00,   //0x00020301 00 上1月反向有功平电量
    0x01, 0x04, 0x02, 0x00, 0x00,   //0x00020401 00 上1月反向有功谷电量

    0x01, 0x00, 0x02, 0x01, 0x00,   //0x01020001 00 上1月反向有功总最大需量
    0x01, 0x00, 0x02, 0x01, 0x02,   //0x01020001 01 上1月反向有功总最大需量发生日期
    0x01, 0x00, 0x02, 0x01, 0x01,   //0x01020001 02 上1月反向有功总最大需量发生时间

    0x01, 0x00, 0x05, 0x00, 0x00,   //0x00050001 00 上1月第1象限无功总电量
    0x01, 0x00, 0x06, 0x00, 0x00,   //0x00060001 00 上1月第2象限无功总电量
    0x01, 0x00, 0x07, 0x00, 0x00,   //0x00070001 00 上1月第3象限无功总电量
    0x01, 0x00, 0x08, 0x00, 0x00,   //0x00080001 00 上1月第4象限无功总电量

    0x01, 0x04, 0x00, 0x04, 0x00,   //0x04000401 00 电能表通信地址（表号）低8位
    0x01, 0x04, 0x00, 0x04, 0x01,   //0x04000401 01 电能表通信地址（表号）高4位
//    0x09, 0x04, 0x00, 0x04, 0x00,   //0x04000409 00 有功脉冲常数
//    0x0a, 0x04, 0x00, 0x04, 0x00,   //0x0400040a 00 无功脉冲常数
    0x00, 0x01, 0x01, 0x02, 0x00,   //0x02010100 00 A相电压
    0x00, 0x02, 0x01, 0x02, 0x00,   //0x02010200 00 B相电压
    0x00, 0x03, 0x01, 0x02, 0x00,   //0x02010300 00 C相电压
    0x00, 0x01, 0x02, 0x02, 0x00,   //0x02020100 00 A相电流
    0x00, 0x02, 0x02, 0x02, 0x00,   //0x02020200 00 B相电流
    0x00, 0x03, 0x02, 0x02, 0x00,   //0x02020300 00 C相电流
    0x00, 0x00, 0x03, 0x02, 0x00,   //0x02030000 00 瞬时总有功功率
    0x00, 0x01, 0x03, 0x02, 0x00,   //0x02030100 00 瞬时A相有功功率
    0x00, 0x02, 0x03, 0x02, 0x00,   //0x02030200 00 瞬时B相有功功率
    0x00, 0x03, 0x03, 0x02, 0x00,   //0x02030300 00 瞬时C相有功功率
    0x00, 0x00, 0x06, 0x02, 0x00,   //0x02060000 00 瞬时总功率因数
    0x00, 0x01, 0x06, 0x02, 0x00,   //0x02060100 00 瞬时A相功率因数
    0x00, 0x02, 0x06, 0x02, 0x00,   //0x02060200 00 瞬时B相功率因数
    0x00, 0x03, 0x06, 0x02, 0x00,   //0x02060300 00 瞬时C相功率因数
};
#endif

//事件记录数据标识
//{ID,SNUM,CNT}
static const DIS_AUX_CTRL evet_lsn_tab[DIS_AUX_EVET_NUM] =
{
    {0x03010000,0,6},               //A,B,C相失压总次数，总累计时间
    {0x03010101,0,4},               //A相失压记录:发生&结束时间
    {0x03010201,0,4},               //B相失压记录:发生&结束时间
    {0x03010301,0,4},               //C相失压记录:发生&结束时间

    {0x03020000,0,6},               //A,B,C相欠压总次数，总累计时间
    {0x03020101,0,4},               //A相欠压记录:发生&结束时间
    {0x03020201,0,4},               //B相欠压记录:发生&结束时间
    {0x03020301,0,4},               //C相欠压记录:发生&结束时间

    {0x03030000,0,6},               //A,B,C相过压总次数，总累计时间
    {0x03030101,0,4},               //A相过压记录:发生&结束时间
    {0x03030201,0,4},               //B相过压记录:发生&结束时间
    {0x03030301,0,4},               //C相过压记录:发生&结束时间

    {0x03040000,0,6},               //A,B,C相断相总次数，总累计时间
    {0x03040101,0,4},               //A相断相记录:发生&结束时间
    {0x03040201,0,4},               //B相断相记录:发生&结束时间
    {0x03040301,0,4},               //C相断相记录:发生&结束时间

    {0x03050000,0,2},               //全失压总次数，总累计时间
    {0x03050001,0,5},               //全失压发生时刻，电流值，结束时刻

    {0x03060000,0,2},               //辅助电源失电总次数，总累计时间
    {0x03060001,0,4},               //辅助电源失电发生时刻，结束时刻

    {0x03070000,0,2},               //电压逆相序总次数，总累计时间
    {0x03070001,0,4},               //电压逆相序记录：发生时刻，结束时刻

    {0x03080000,0,2},               //电流逆相序总次数，总累计时间
    {0x03080001,0,4},               //电流逆相序记录：发生时刻，结束时刻

    {0x03090000,0,2},               //电压不平衡总次数，总累计时间
    {0x03090001,0,4},               //电压不平衡记录：发生时刻，结束时刻

    {0x030A0000,0,2},               //电流不平衡总次数，总累计时间
    {0x030A0001,0,4},               //电流不平衡记录：发生时刻，结束时刻

    {0x030B0000,0,6},               //A,B,C相失流总次数，总累计时间
    {0x030B0101,0,4},               //A相失流记录:发生&结束时间
    {0x030B0201,0,4},               //B相失流记录:发生&结束时间
    {0x030B0301,0,4},               //C相失流记录:发生&结束时间

    {0x030C0000,0,6},               //A,B,C相过流总次数，总累计时间
    {0x030C0101,0,4},               //A相过流记录:发生&结束时间
    {0x030C0201,0,4},               //B相过流记录:发生&结束时间
    {0x030C0301,0,4},               //C相过流记录:发生&结束时间

    {0x030D0000,0,6},               //A,B,C相断流总次数，总累计时间
    {0x030D0101,0,4},               //A相断流记录:发生&结束时间
    {0x030D0201,0,4},               //B相断流记录:发生&结束时间
    {0x030D0301,0,4},               //C相断流记录:发生&结束时间

    {0x030E0000,0,6},               //A,B,C相潮流反向总次数，总累计时间
    {0x030E0101,0,4},               //A相潮流反向记录:发生&结束时间
    {0x030E0201,0,4},               //B相潮流反向记录:发生&结束时间
    {0x030E0301,0,4},               //C相潮流反向记录:发生&结束时间

    {0x030F0000,0,6},               //A,B,C相过载总次数，总累计时间
    {0x030F0101,0,4},               //A相过载记录:发生&结束时间
    {0x030F0201,0,4},               //B相过载记录:发生&结束时间
    {0x030F0301,0,4},               //C相过载记录:发生&结束时间

    {0x03100000,0,9},               //电压合格率统计数据块

    {0x03100100,0,9},               //A相电压合格率统计数据块
    {0x03100200,0,9},               //B相电压合格率统计数据块
    {0x03100300,0,9},               //C相电压合格率统计数据块

    {0x03110001,0,4},               //掉电记录：发生时刻，结束时刻

    {0x03120002,0,6},               //需量超限数据块

    {0x03120101,0,7},               //正向有功需量超限记录：发生时刻，结束时刻
                                    //期间正向有功最大需量及发生时间

    {0x03300001,0,3},               //编程记录：发生时刻，操作者代码

    {0x03300101,0,3},               //电表清零记录：发生时刻，操作者代码

    {0x03300201,0,3},               //需量清零记录：发生时刻，操作者代码

    {0x03300301,0,3},               //事件清零记录：发生时刻，操作者代码

    {0x03300401,0,5},               //校时记录：操作者代码，校时前、后时间

    {0x03300501,0,3},               //时段表编程记录：发生时刻，操作者代码

    {0x03300601,0,3},               //时区表编程记录：发生时刻，操作者代码

    {0x03300701,0,3},               //周休日编程记录：发生时刻，操作者代码

    {0x03300801,0,3},               //节假日编程记录：发生时刻，操作者代码

    {0x03300901,0,3},               //有功组合方式编程记录：发生时刻，操作者代码

    {0x03300A01,0,3},               //无功组合方式1编程记录：发生时刻，操作者代码

    {0x03300B01,0,3},               //无功组合方式2编程记录：发生时刻，操作者代码

    {0x03300C01,0,3},               //结算日编程记录：发生时刻，操作者代码

    {0x03300D01,0,4},               //结开表盖记录：发生时刻，结束时刻
};

//参变量数据标识
//{ID,SNUM,CNT}
static const DIS_AUX_CTRL para_lsn_tab[DIS_AUX_PARA_NUM] =
{
    {0x04000106,0,2},               //两套时区表切换时间
    {0x04000107,0,2},               //两套日时段表切换时间

    {0x04000401,0,2},               //通信地址
    {0x04000402,0,2},               //表号
};
/******************************************************************************
** 函数名称: void set_rate_char(U8 rate, U8 *fix_data)
** 函数功能:
** 入口参数: rate=0:总 1:尖......
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void set_rate_char (U8 rate, U8 *fix_data)//
{
    switch(rate)
    {
        case 0://总
            fix_data[1] |= BY1_B6;
            break;
        case 1://尖
            fix_data[1] |= BY1_B7;
            break;
        case 2:
            fix_data[2] |= BY2_B0;
            break;
        case 3:
            fix_data[2] |= BY2_B1;
            break;
        case 4:
            fix_data[2] |= BY2_B2;
            break;
    }
}
/******************************************************************************
** 函数名称: void set_totabc_char(U8 phase, U8 *fix_data)
** 函数功能:
** 入口参数: phase=0:总 1:A相......
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void set_totabc_char (U8 phase, U8 *fix_data)
{
    switch(phase)
    {
        case 0://总
            fix_data[1] |= BY1_B6;
            break;
        case 1://A
            fix_data[2] |= BY2_B3;
            break;
        case 2:
            fix_data[2] |= BY2_B4;
            break;
        case 3:
            fix_data[2] |= BY2_B5;
            break;
    }
}
/******************************************************************************
** 函数名称: void set_eng_dmd_char (U8 *indi, U8 *fix_data, U8 *segbuf)
** 函数功能:
** 入口参数:
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void set_eng_dmd_char (U8 *indi, U8 *fix_data, U8 *segbuf)
{
    U8 dix;

    if((indi[4]!=0) && (indi[4]!=1))
    {
        return;
    }
    fix_data[3] |= BY3_B5;//量

    if(indi[1] == 0 )//是当前 显示当前
    {
        fix_data[0] |= (BY0_B0 |BY0_B2);//当前月
    }
    else//不是当前 是上几个月
    {
        fix_data[0] |= (BY0_B1 |BY0_B2);//上月
        fill_data_last8_segbuf(indi[1], segbuf);//n
    }

    if(indi[4] == 0)
    {
//电量所用到字符填充
        fix_data[3] |= BY3_B4;//电
        fix_data[5] |= BY5_B4;//小数点xxxxxx.xx
        if(indi[3] <= 0x80)
        {
            if(indi[3] <= 0x0A)//合相
            {
                if(indi[2] > RATE_NUM)
                {
                    return;
                }
                set_rate_char(indi[2], fix_data);//总尖峰平谷
                dix = indi[3];
            }
            else//ABC
            {
                dix = indi[3] - 0x15;
                if(dix != 0)
                {
                    set_totabc_char((U8)(dix/0x14), fix_data);
                }
                dix %= 0x14;
            }
            switch(dix)//单位+汉字
            {
                case 0x00://组合有功
                    fix_data[0] |= BY0_B3;//组合
                    fix_data[1] |= (BY1_B0 | BY1_B1);//有功
                    fix_data[6] |= CON_UNIT_KWH;
                    break;
                case 0x01://正向有功
                    fix_data[0] |= (BY0_B5 | BY0_B6);//正向
                    fix_data[1] |= (BY1_B0 | BY1_B1);//有功
                    fix_data[6] |= CON_UNIT_KWH;

                    break;
                case 0x02://反向有功
                    fix_data[0] |= (BY0_B4 | BY0_B6);//反向
                    fix_data[1] |= (BY1_B0 | BY1_B1);//有功
                    fix_data[6] |= CON_UNIT_KWH;
                    break;
                case 0x03://组合无功1
                case 0x04://组合无功2
                    fix_data[0] |= BY0_B3;//组合
                    fix_data[0] |= BY0_B7;//无
                    fix_data[1] |= BY1_B1;//功
                    fix_data[6] |= CON_UNIT_KVARH;
                    break;
                case 0x05://一象限无功
                    fix_data[0] |= BY0_B7;//无
                    fix_data[1] |= BY1_B1;//功
                    fix_data[1] |= BY1_B2;//一象限
                    fix_data[6] |= CON_UNIT_KVARH;
                    break;
                case 0x06://二象限无功
                    fix_data[0] |= BY0_B7;//无
                    fix_data[1] |= BY1_B1;//功
                    fix_data[1] |= (BY1_B2 | BY1_B3);//二象限
                    fix_data[6] |= CON_UNIT_KVARH;
                    break;
                case 0x07://三象限无功
                    fix_data[0] |= BY0_B7;//无
                    fix_data[1] |= BY1_B1;//功
                    fix_data[1] |= (BY1_B2 | BY1_B3 | BY1_B4);//三象限
                    fix_data[6] |= CON_UNIT_KVARH;
                    break;
                case 0x08://四象限无功
                    fix_data[0] |= BY0_B7;//无
                    fix_data[1] |= BY1_B1;//功
                    fix_data[1] |= (BY1_B4 | BY1_B5);//四象限
                    fix_data[6] |= CON_UNIT_KVARH;
                    break;
                case 0x09://正向视在
                    fix_data[0] |= (BY0_B5 | BY0_B6);//正向
                    fix_data[6] |= CON_UNIT_KVAH;
                    break;
                case 0x0A://反向视在
                    fix_data[0] |= (BY0_B4 | BY0_B6);//反向
                    fix_data[6] |= CON_UNIT_KVAH;
                    break;
            }
        }
        else
        {
            dix = indi[3];
            if(dix >= 0x80)//总ABC关联
            {
                dix -= 0x80;
                //总ABC
                set_totabc_char((U8)(dix/0x14), fix_data);
                switch(dix%0x14)
                {
                    case 0x00://关联总电能
                    case 0x05://铜损有功总电能补偿量
                    case 0x06://铁损有功总电能补偿量
                        fix_data[1] |= (BY1_B0 | BY1_B1);//有功
                        fix_data[6] |= CON_UNIT_KWH;
                        break;
                    case 0x01://正向有功基波总电能
                    case 0x03://正向有功谐波总电能
                        fix_data[0] |= (BY0_B5 | BY0_B6);//正向
                        fix_data[1] |= (BY1_B0 | BY1_B1);//有功
                        fix_data[6] |= CON_UNIT_KWH;
                        break;
                    case 0x02://反向有功基波总电能
                    case 0x04://反向有功谐波总电能
                        fix_data[0] |= (BY0_B4 | BY0_B6);//反向
                        fix_data[1] |= (BY1_B0 | BY1_B1);//有功
                        fix_data[6] |= CON_UNIT_KWH;
                        break;
                }
            }
        }
    }
//需量所用到字符填充
    else
    {
        fix_data[3] |= BY3_B3;//需

        //小数点
        if(indi[0] == 0)//小项里的需量的小数点XX.XXXX
        {
            fix_data[5] |= BY5_B2;
        }
        else if(indi[0] == 1)//小项里的时间：时:分
        {
            fix_data[6] |= BY6_B7;
            fix_data[5] |= BY5_B4;
            fix_data[4] |= (BY4_B4 | BY4_B5);//时间
        }
        else//小项里的时间：年.月.日
        {
            fix_data[5] |= (BY5_B2 | BY5_B4);
            fix_data[4] |= (BY4_B4 | BY4_B5);//时间
        }
        if(indi[3] <= 0x80)
        {
            if(indi[3] <= 0x0A)//合相
            {
                if(indi[2] > RATE_NUM)
                {
                    return;
                }
                set_rate_char(indi[2], fix_data);//总尖峰平谷
                dix = indi[3];
            }
            else//ABC
            {
                dix = indi[3] - 0x15;
                if(dix != 0)
                {
                    set_totabc_char((U8)(dix/0x14), fix_data);
                }
                dix %= 0x14;
            }
            switch(dix)//单位+汉字
            {
                case 0x00://组合有功
                    fix_data[0] |= BY0_B3;//组合
                    fix_data[1] |= (BY1_B0 | BY1_B1);//有功
                    if(indi[0] == 0)
                    {
                        fix_data[6] |= CON_UNIT_KW;
                    }
                    break;
                case 0x01://正向有功
                    fix_data[0] |= (BY0_B5 | BY0_B6);//正向
                    fix_data[1] |= (BY1_B0 | BY1_B1);//有功
                    if(indi[0] == 0)
                    {
                        fix_data[6] |= CON_UNIT_KWH;
                    }
                    break;
                case 0x02://反向有功
                    fix_data[0] |= (BY0_B4 | BY0_B6);//反向
                    fix_data[1] |= (BY1_B0 | BY1_B1);//有功
                    if(indi[0] == 0)
                    {
                        fix_data[6] |= CON_UNIT_KWH;
                    }
                    break;
                case 0x03://组合无功1
                case 0x04://组合无功2
                    fix_data[0] |= BY0_B3;//组合
                    fix_data[0] |= BY0_B7;//无
                    fix_data[1] |= BY1_B1;//功
                    if(indi[0] == 0)
                    {
                        fix_data[6] |= CON_UNIT_KVAR;
                    }
                    break;
                case 0x05://一象限无功
                    fix_data[0] |= BY0_B7;//无
                    fix_data[1] |= BY1_B1;//功
                    fix_data[1] |= BY1_B2;//一象限
                    if(indi[0] == 0)
                    {
                        fix_data[6] |= CON_UNIT_KVAR;
                    }
                    break;
                case 0x06://二象限无功
                    fix_data[0] |= BY0_B7;//无
                    fix_data[1] |= BY1_B1;//功
                    fix_data[1] |= (BY1_B2 | BY1_B3);//二象限
                    if(indi[0] == 0)
                    {
                        fix_data[6] |= CON_UNIT_KVAR;
                    }
                    break;
                case 0x07://三象限无功
                    fix_data[0] |= BY0_B7;//无
                    fix_data[1] |= BY1_B1;//功
                    fix_data[1] |= (BY1_B2 | BY1_B3 | BY1_B4);//三象限
                    if(indi[0] == 0)
                    {
                        fix_data[6] |= CON_UNIT_KVAR;
                    }
                    break;
                case 0x08://四象限无功
                    fix_data[0] |= BY0_B7;//无
                    fix_data[1] |= BY1_B1;//功
                    fix_data[1] |= (BY1_B4 | BY1_B5);//四象限
                    if(indi[0] == 0)
                    {
                        fix_data[6] |= CON_UNIT_KVAR;
                    }
                    break;
                case 0x09://正向视在
                    fix_data[0] |= (BY0_B5 | BY0_B6);//正向
                    if(indi[0] == 0)
                    {
                        fix_data[6] |= CON_UNIT_KVA;
                    }
                    break;
                case 0x0A://反向视在
                    fix_data[0] |= (BY0_B4 | BY0_B6);//反向
                    if(indi[0] == 0)
                    {
                        fix_data[6] |= CON_UNIT_KVA;
                    }
                    break;
            }
        }
    }
}
/******************************************************************************
** 函数名称: void set_event_char(U8 *indi, U8 *fix_data, U8 *segbuf)
** 函数功能:
** 入口参数:
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void set_event_char (U8 *indi, U8 *fix_data, U8 *segbuf)
{
//  U8 dix;

    if(indi[3] <= 0x04)///电压相关ABC相的事件/失压　欠压　过压　断相
    {
        if(indi[1] == 0)//次数　时间//ABC
        {
            set_totabc_char((U8)((indi[0]/2)+1), fix_data);
        }
        else
        {
            set_totabc_char(indi[2], fix_data);//ABC
            fix_data[0] |= BY0_B1;//上
            fill_data_last8_segbuf(indi[1], segbuf);//n
        }
    }
    else if(indi[3] <= 0x0A)//没有ABC相的事件
    {
        fix_data[0] |= BY0_B1;//上
        fill_data_last8_segbuf(indi[1], segbuf);//n
    }
    else if(indi[3] <= 0x0F)//电流相关ABC相的事件
    {
        if(indi[1] == 0)//次数　时间//ABC
        {
            set_totabc_char((U8)((indi[0]/2)+1), fix_data);
        }
        else
        {
            set_totabc_char(indi[2], fix_data);//ABC
            fix_data[0] |= BY0_B1;//上
            fill_data_last8_segbuf(indi[1], segbuf);//n
        }
    }
    else if(indi[3] == 0x10)//电压合格率di0=0:i当前 di0=1:上１次
    {
        if(indi[1] == 0)
        {
            fix_data[0] |= (BY0_B0 |BY0_B2);//当前月
        }
        else
        {
            fix_data[0] |= (BY0_B1 |BY0_B2);//上月
            fill_data_last8_segbuf(indi[1], segbuf);//n
        }
        if(indi[2] != 0)
        {
            set_totabc_char(indi[2], fix_data);//ABC
        }
    }
    else
    {
        if(indi[1] != 0)
        {
            fix_data[0] |= BY0_B1;//上
            fill_data_last8_segbuf(indi[1], segbuf);//n
        }
    }
}
/******************************************************************************
** 函数名称: void set_gwevent_char(U8 *indi, U8 *fix_data, U8 *segbuf)
** 函数功能: 国网扩展的事件数据标识DI3=0x10~0x1F
** 入口参数:
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void set_gwevent_char (U8 *indi, U8 *fix_data, U8 *segbuf)
{
//  U8 dix;

    if(indi[4] <= 0x13)///电压相关ABC相的事件//失压 欠压 过压 断相
    {
        if(indi[4] == 0x10)//失压有点特殊
        {
            if(indi[3] == 0)
            {
                fix_data[0] |= BY0_B0;//当前
            }
            else if(indi[2] == 0)
            {
                fix_data[0] |= BY0_B0;//当前
                set_totabc_char(indi[3], fix_data);//ABC
            }
            else if(indi[1] != 0)
            {
                set_totabc_char(indi[3], fix_data);//ABC
                fix_data[0] |= BY0_B1;//上
                fill_data_last8_segbuf(indi[1], segbuf);//n
            }
        }
        else
        {
            if(indi[2] == 0)
            {
                fix_data[0] |= BY0_B0;//当前
                set_totabc_char(indi[3], fix_data);//ABC
            }
            else if(indi[1] != 0)
            {
                set_totabc_char(indi[3], fix_data);//ABC
                fix_data[0] |= BY0_B1;//上
                fill_data_last8_segbuf(indi[1], segbuf);//n
            }
        }
    }
    else if(indi[4] <= 0x17)//逆相序 不平衡
    {
        if(indi[2] == 0)
        {
            fix_data[0] |= BY0_B0;//当前
        }
        else if(indi[1] != 0)
        {
            fix_data[0] |= BY0_B1;//上
            fill_data_last8_segbuf(indi[1], segbuf);//n
        }
    }
    else if(indi[3] <= 0x1C)//电流相关ABC相的事件
    {
        if(indi[2] == 0)
        {
            fix_data[0] |= BY0_B0;//当前
            set_totabc_char(indi[3], fix_data);//ABC
        }
        else if(indi[1] != 0)
        {
            set_totabc_char(indi[3], fix_data);//ABC
            fix_data[0] |= BY0_B1;//上
            fill_data_last8_segbuf(indi[1], segbuf);//n
        }
    }
    else if(indi[3] == 0x1f)//总功率因数超下限
    {
        if(indi[2] == 0)
        {
            fix_data[0] |= BY0_B0;//当前
        }
        else if(indi[1] != 0)
        {
            fix_data[0] |= BY0_B1;//上
            fill_data_last8_segbuf(indi[1], segbuf);//n
        }
    }

}
/******************************************************************************
** 函数名称: void set_time_char(U8 *indi, U8 *fix_data, U8 *segbuf)
** 函数功能:
** 入口参数:
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void set_time_char (U8 *indi, U8 *fix_data, U8 *segbuf)
{
    U32 di;

    di = indi[1] + (indi[2]<<8) + (indi[3]<<16) + (indi[4]<<24);
    if(di == 0x04000101)//日期 YY.MM.DD.WW
    {
        fix_data[5] |= (BY5_B4 | BY5_B2 | BY5_B0);
    }
    else if(di == 0x04000102)//时间 hh:mm:ss
    {
        fix_data[5] |= (BY5_B4 | BY5_B2);
        fix_data[6] |= (BY6_B6 | BY6_B7);
    }
}
/******************************************************************************
** 函数名称: S8 get_dis_info (U8 *di_buf, U8 *fix_data, DIS_CTRL_TCB *pdis_tcb, U8 *segbuf)
** 函数功能:
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
S8 get_dis_info (U8 *di_buf, U8 *fix_data, DIS_CTRL_TCB *pdis_tcb, U8 *segbuf)
{
//  U8  lsn;
    U8  dix;
    U16 sn;
    U32 di_temp;
    U32 di;
#ifndef DATA_SOURCE_NOT_LOCAL
    U32 offset;
    U32 addr;
    U32 err;
    ADDR_OFFSET_TCB offset_tcb;
#endif//DATA_SOURCE_NOT_LOCAL

    di = di_buf[1] + (di_buf[2]<<8) + (di_buf[3]<<16) + (di_buf[4]<<24);

#if CODE_VER_TYPE==WLS_CUR_PRJ			//配合无线采集器
    get_dis_temp_data(di, di_buf[0]);//azh 无线采集器版本
#endif

    switch(di_buf[4])
    {
        case 0x00:
        {
            if(di_buf[3] <= 0x0A)
            {
                di_temp = di&0xffff0000;
            }
            else
            {
                 di_temp = di&0xffffff00;
            }
            //归类从表格里搜索
            sn = get_dis_sn_bydi(di_temp, di_buf[0], disctrbydi_di300table, DI3_00_TABLE_MAXNUM);
            if(sn == 0xffff)
            {
                return __FALSE;
            }
            *pdis_tcb = disctrbydi_di300table[sn];
            if(di_buf[3] <= 0x0A)
            {
                if(di_buf[2] > RATE_NUM)
                {
                    return __FALSE;
                }
                pdis_tcb->addr +=  ((pdis_tcb->datalen&0x0f)*di_buf[2]);//低四位是小项数据长度
            }
#ifdef DATA_SOURCE_NOT_LOCAL

	#if CODE_VER_TYPE==WLS_CUR_PRJ			//配合无线采集器
	//azh 130928 无线采集器版本 当前也显示临时的全局变里的数据
			pdis_tcb->addr = (U32)gTmpArray + 5;//转到全局临时RAM地址L+DI+DATA
            if((memcmp(gTmpArray+1,di_buf+1,4) != 0) || (gTmpArray[0] < ((pdis_tcb->datalen&0x0f)+4)))//比对数据标识是否相同
            {
            	memset(gTmpArray+5, 0xee, (pdis_tcb->datalen&0x0f));
            }
	#else
            if(di_buf[1] ==0 )////不是当前 是上几个月
            {
                pdis_tcb->addr = pdis_tcb->addr - ENG_STORE_START_ADDR + (U32)gEngArray;//转到全局RAM地址
            }
#endif
#endif
            if(di_buf[1] !=0 )////不是当前 是上几个月
            {
#ifdef DATA_SOURCE_NOT_LOCAL
                pdis_tcb->addr = (U32)gTmpArray + 5;//转到全局临时RAM地址L+DI+DATA
                if((memcmp(gTmpArray+1,di_buf+1,4) != 0) || (gTmpArray[0] < ((pdis_tcb->datalen&0x0f)+4)))//比对数据标识是否相同
                {
                    memset(gTmpArray+5, 0xee, (pdis_tcb->datalen&0x0f));
                }
#else
                offset = pdis_tcb->addr - ENG_COMPOSP_00000000_00_ADDR;//折算成偏移地址
                offset_tcb.head_addr = ENG_LASTXMONTH_00000000_00_HEAD_ADDR;
                offset_tcb.offset_addr = ENG_LASTXMONTH_00000000_00_OFFSET_ADDR;
                offset_tcb.block_len = ENG_LASTXMONTH_00000000_00_OFFSET_BLOCKLEN;
                offset_tcb.offset_len = ENG_LASTXMONTH_00000000_00_OFFSET_LEN;
                offset_tcb.max_num = ENG_LASTXMONTH_00000000_00_OFFSET_MAXNUM;

                err = get_addr_offset((U8)(di_buf[1]-1), &offset_tcb, &addr);
                if(err != NO_ERROR)
                {
                    return  __FALSE;
                }
                pdis_tcb->addr = addr + offset;
#endif
            }
            set_eng_dmd_char(di_buf, fix_data, segbuf);//填充有规律的汉字显示缓冲
            break;
        }
        case 0x01:
        {
            if(di_buf[3] <= 0x0A)
            {
                di_temp = di&0xffff0000;
            }
            else
            {
                 di_temp = di&0xffffff00;
            }
            //归类从表格里搜索
            sn = get_dis_sn_bydi(di_temp, di_buf[0], disctrbydi_di301table, DI3_01_TABLE_MAXNUM);
            if(sn == 0xffff)
            {
                return __FALSE;
            }
            *pdis_tcb = disctrbydi_di301table[sn];
            if(di_buf[3] <= 0x0A)
            {
                if(di_buf[2] > RATE_NUM)
                {
                    return __FALSE;
                }
                pdis_tcb->addr +=  ((pdis_tcb->datalen&0x0f)*di_buf[2]);//低四位是小项数据长度
            }
#ifdef DATA_SOURCE_NOT_LOCAL
            if(di_buf[1] ==0 )////不是当前 是上几个月
            {
                pdis_tcb->addr = pdis_tcb->addr - DMD_POSP_01010000_00_ADDR + (U32)gDmdArray;//转到全局RAM地址
            }
#endif
            if(di_buf[1] !=0 )////不是当前 是上几个月
            {
#ifdef DATA_SOURCE_NOT_LOCAL
                pdis_tcb->addr = (U32)gTmpArray + 5;//转到全局临时RAM地址L+DI+DATA
                if((memcmp(gTmpArray+1,di_buf+1,4) != 0) || (gTmpArray[0] < ((pdis_tcb->datalen&0x0f)+4)))//比对数据标识是否相同
                {
                    memset(gTmpArray+5, 0xee, (pdis_tcb->datalen&0x0f));
                }
                if(di_buf[0] != 0)//不是拆分的第一项
                {
                    if(sn >= di_buf[0])
                    {
                        pdis_tcb->addr += (disctrbydi_di301table[sn].addr - disctrbydi_di301table[sn - di_buf[0]].addr);
                    }
                }
#else
                offset = pdis_tcb->addr - DMD_POSP_01010000_00_ADDR;//折算成偏移地址
                offset_tcb.head_addr = DMD_LASTXMONTH_01010001_00_HEAD_ADDR;
                offset_tcb.offset_addr = DMD_LASTXMONTH_01010001_00_OFFSET_ADDR;
                offset_tcb.block_len = DMD_LASTXMONTH_01010001_00_OFFSET_BLOCKLEN;
                offset_tcb.offset_len = DMD_LASTXMONTH_01010001_00_OFFSET_LEN;
                offset_tcb.max_num = DMD_LASTXMONTH_01010001_00_OFFSET_MAXNUM;

                err = get_addr_offset((U8)(di_buf[1]-1), &offset_tcb, &addr);
                if(err != NO_ERROR)
                {
                    return  __FALSE;
                }
                pdis_tcb->addr = addr + offset;
#endif
            }
            set_eng_dmd_char (di_buf, fix_data, segbuf);//填充有规律的汉字显示缓冲
            break;
        }
        case 0x02:
        {
            //瞬时量不需要归类 是全列出来的
            if(di_buf[3] <= 0x0B)
            {
                set_totabc_char(di_buf[2], fix_data);//总ABC
            }
            sn = get_dis_sn_bydi(di, di_buf[0], disctrbydi_di302table, DI3_02_TABLE_MAXNUM);
            if(sn == 0xffff)
            {
                return __FALSE;
            }
            *pdis_tcb = disctrbydi_di302table[sn];
#ifdef DATA_SOURCE_NOT_LOCAL
            pdis_tcb->addr = (U32)gTmpArray + 5;//转到全局临时RAM地址L+DI+DATA
            if((memcmp(gTmpArray+1,di_buf+1,4) != 0) || (gTmpArray[0] < ((pdis_tcb->datalen&0x0f)+4)))//比对数据标识是否相同
            {
                memset(gTmpArray+5, 0xee, (pdis_tcb->datalen&0x0f));
            }
            if(di_buf[0] != 0)//不是拆分的第一项
            {
                if(sn >= di_buf[0])
                {
                    pdis_tcb->addr += (disctrbydi_di302table[sn].addr - disctrbydi_di302table[sn - di_buf[0]].addr);
                }
            }
#endif
            break;
        }
        case 0x03:
        {
            if(di_buf[3] == 0x10)//只有合格率事件是从di0=0　表格里也只有
            {
                di_temp = di&0xffffff00;
            }
            else
            {
                di_temp = (di&0xffffff00);
                if(di_buf[1] != 0)//其他事件表格可找到di0=0,di0=1
                {
                    di_temp += 1;
                }
            }
            //归类后从表格里搜索
            sn = get_dis_sn_bydi(di_temp, di_buf[0], disctrbydi_di303table, DI3_03_TABLE_MAXNUM);
            if(sn == 0xffff)
            {
                return __FALSE;
            }
            *pdis_tcb = disctrbydi_di303table[sn];
#ifdef DATA_SOURCE_NOT_LOCAL
            pdis_tcb->addr = (U32)gTmpArray + 5;//转到全局临时RAM地址L+DI+DATA
            if((memcmp(gTmpArray+1,di_buf+1,4) != 0) || (gTmpArray[0] < ((pdis_tcb->datalen&0x0f)+4)))//比对数据标识是否相同
            {
                memset(gTmpArray+5, 0xee, (pdis_tcb->datalen&0x0f));
            }
            if(di_buf[0] != 0)//不是拆分的第一项
            {
                if(sn >= di_buf[0])
                {
                    pdis_tcb->addr += (disctrbydi_di303table[sn].addr - disctrbydi_di303table[sn - di_buf[0]].addr);
                }
            }
#else
            if((di_buf[3] == 0x10) || (di_buf[1] != 0))
            {
                if(di_buf[3] == 0x10)//只有合格率事件是从di0=0　表格里也只有
                {
                    dix = di_buf[1];
                }
                else
                {
                    dix = di_buf[1] - 1;
                }
                if(pdis_tcb->offsettabsn != 0xff)//有偏移表格存在
                {
                    sn = pdis_tcb->offsettabsn;
                    offset = pdis_tcb->addr - dis_exdctr_offsetbysn_table[sn].head_addr;//折算成偏移地址

                    offset_tcb.head_addr = dis_exdctr_offsetbysn_table[sn].head_addr;
                    offset_tcb.offset_addr = dis_exdctr_offsetbysn_table[sn].offset_addr;
                    offset_tcb.block_len = dis_exdctr_offsetbysn_table[sn].block_len;
                    offset_tcb.offset_len = dis_exdctr_offsetbysn_table[sn].offset_len;
                    offset_tcb.max_num = dis_exdctr_offsetbysn_table[sn].max_num;

                    err = get_addr_offset(dix, &offset_tcb, &addr);
                    if(err != NO_ERROR)
                    {
                        return  __FALSE;
                    }
                    pdis_tcb->addr = addr + offset;
                }
            }
#endif
            set_event_char(di_buf, fix_data, segbuf);
            break;
        }
        case 0x04:
        {
            dix = di_buf[0];
            if((di_buf[3] == 0x01) || (di_buf[3] == 0x02))//第一二套时区时段表
            {
                di_temp = 0x00010000;//只有第一套时区及1时段表
                if(di_buf[0] != 0 )
                {
                    di_temp |= 0x00000001;
                }
                di_temp |= (di&0xffffff00);
                dix = 0;
            }
            else if((di_buf[3] == 0x03) || (di_buf[3] == 0x04))//公共假日 自动循环显示 按键循环显示 表格中存放的首地址
            {
                    di_temp = 1;
                    di_temp |= (di&0xffffff00);
                    dix = 0;
            }
            else
            {
                di_temp = di;
            }
            //归类后从表格里搜索
            sn = get_dis_sn_bydi(di_temp, dix, disctrbydi_di304table, DI3_04_TABLE_MAXNUM);
            if(sn == 0xffff)
            {
                if(di != 0x04000401)
                {
                   return __FALSE;
                }
                else
                {
                    sn = 0;
                }
            }
            *pdis_tcb = disctrbydi_di304table[sn];
#ifdef DATA_SOURCE_NOT_LOCAL
            if(di == 0x04000401)//由于全局表格这个通信地址这个数据标识有有错
            {
                if(di_buf[0]>1)
                {
                  return __FALSE;
                }
                *pdis_tcb = disctrbydi_spec_di304table[di_buf[0]];
                pdis_tcb->addr = (U32)gTmpArray + 5;//转到全局临时RAM地址L+DI+DATA
                if((memcmp(gTmpArray+1,di_buf+1,4) != 0) || (gTmpArray[0] < ((pdis_tcb->datalen&0x0f)+4)))//比对数据标识是否相同
                {
                    memset(gTmpArray+5, 0xee, (pdis_tcb->datalen&0x0f));
                }
                if(di_buf[0] != 0)//是高2字节
                {
                    pdis_tcb->addr += 4;
                }
            }
            else
            {
                pdis_tcb->addr = (U32)gTmpArray + 5;//转到全局临时RAM地址L+DI+DATA
                if((memcmp(gTmpArray+1,di_buf+1,4) != 0) || (gTmpArray[0] < ((pdis_tcb->datalen&0x0f)+4)))//比对数据标识是否相同
                {
                    memset(gTmpArray+5, 0xee, (pdis_tcb->datalen&0x0f));
                }
                if(di_buf[0] != 0)//不是拆分的第一项
                {
                    if(sn >= di_buf[0])
                    {
                        pdis_tcb->addr += (disctrbydi_di304table[sn].addr - disctrbydi_di304table[sn - di_buf[0]].addr);
                    }
                }
            }
#else
            if((di_buf[3] == 0x01) || (di_buf[3] == 0x02))//第一二套时区时段表
            {
                addr = pdis_tcb->addr + (di_buf[3]-1) * PAR_T1_TOT_LEN;
                offset = di_buf[0] * (pdis_tcb->datalen&0x0f);
                pdis_tcb->addr = addr + offset;
                fill_data_last8_segbuf(di_buf[0], segbuf);//n
            }
            else if((di_buf[3] == 0x03) || (di_buf[3] == 0x04))//公共假日 自动循环显示 按键循环显示
            {
                offset = di_buf[0] * (pdis_tcb->datalen&0x0f);
                pdis_tcb->addr += offset;
                fill_data_last8_segbuf(di_buf[0], segbuf);//n
            }
 #endif
            set_time_char(di_buf, fix_data, segbuf);
            break;
        }
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x17:
        case 0x18:
        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x1C:
//        case 0x1D://跳闸
//        case 0x1E://合闸
        case 0x1F:
        {
            di_temp = di;
            if(di_buf[2] >= 0x01)//只有合格率事件是从di0=0　表格里也只有
            {
                if((di_buf[4] != 0x10) || ((di_buf[4] == 0x10) && (di_buf[3] != 0)))//失压有点特殊
                {
                    di_temp = (di & 0xffffff00) + 1;
                }
            }
            //归类后从表格里搜索
            sn = get_dis_sn_bydi(di_temp, di_buf[0], disctrbydi_di310table, DI3_10_TABLE_MAXNUM);
            if(sn == 0xffff)
            {
                return __FALSE;
            }
            *pdis_tcb = disctrbydi_di310table[sn];
#ifdef DATA_SOURCE_NOT_LOCAL
            pdis_tcb->addr = (U32)gTmpArray + 5;//转到全局临时RAM地址L+DI+DATA
            if((memcmp(gTmpArray+1,di_buf+1,4) != 0) || (gTmpArray[0] < ((pdis_tcb->datalen&0x0f)+4)))//比对数据标识是否相同
            {
                memset(gTmpArray+5, 0xee, (pdis_tcb->datalen&0x0f));
            }
            if(di_buf[0] != 0)//不是拆分的第一项
            {
                if(sn >= di_buf[0])
                {
                    pdis_tcb->addr += (disctrbydi_di303table[sn].addr - disctrbydi_di303table[sn - di_buf[0]].addr);
                }
            }
#else
            if((di_buf[2] >= 0x01) && (di_buf[1] != 0))//上1~10几次
            {
                dix = di_buf[1] - 1;

                if(pdis_tcb->offsettabsn != 0xff)//有偏移表格存在
                {
                    sn = pdis_tcb->offsettabsn;
                    offset = pdis_tcb->addr - dis_exdctr_offsetbysn_table[sn].head_addr;//折算成偏移地址

                    offset_tcb.head_addr = dis_exdctr_offsetbysn_table[sn].head_addr;
                    offset_tcb.offset_addr = dis_exdctr_offsetbysn_table[sn].offset_addr;
                    offset_tcb.block_len = dis_exdctr_offsetbysn_table[sn].block_len;
                    offset_tcb.offset_len = dis_exdctr_offsetbysn_table[sn].offset_len;
                    offset_tcb.max_num = dis_exdctr_offsetbysn_table[sn].max_num;

                    err = get_addr_offset(dix, &offset_tcb, &addr);
                    if(err != NO_ERROR)
                    {
                        return  __FALSE;
                    }
                    pdis_tcb->addr = addr + offset;
                }
            }
#endif
            set_gwevent_char(di_buf, fix_data, segbuf);
            break;
        }
        default:
        {
            return __FALSE;
        }
    }
    return __TRUE;
}


/******************************************************************************
** 函数名称: S8 fill_dis_data(void)
** 函数功能: 填写固定显示的数据
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
S8 fill_dis_data (void)
{
    S8  err;
    U8  i;
    U8  pos;
    U8  disdi[5];
    U8  data[8];
    U16 sn;
    U32 di;
    U32 dis_code;
    DIS_CTRL_TCB dis_tcb;

    if(Dis_Ctrl.cur_mode == DIS_MODE_ALLON)//上电全显自检
    {
        lcd_on_all_dis();
        return  __TRUE;
    }

//初始化全局显示缓冲区
    for(i=0; i<LCD_BUF_LENTH; i++)
    {
        lcd_seg_buf[i] = 0;
    }

    pos = 0;
    lcd_seg_buf[LCD_BUF_LENTH-1] = 0;//显示缓冲有数据填充过 就可以0.5s刷屏了
    lcd_seg_buf[pos++] = ICSET1;
//  lcd_seg_buf[pos++] = APCTL;
    lcd_seg_buf[pos++] = DISCTL22;
//  lcd_seg_buf[pos++] = EVRSET;
    lcd_seg_buf[pos++] = ADSET;

    pos = CON_DIS_DATA_OFFSET;//3
//--------------------------------------------------
    memset(data, 0, 8);//有规律字节显示位标志缓冲

    fill_gprs_signal_strenth_segbuf(lcd_seg_buf+pos);//gprs 信号强度

    if(Dis_Ctrl.cur_mode == DIS_MODE_ERRON)//严重错误
    {
        di = get_meter_serious_err_id();

        fill_dis_err_page((U8*)&di, lcd_seg_buf+pos);
        //填写显示轮显编码区
        dis_code = di;
        fill_dis_fix_l8_data(dis_code, lcd_seg_buf+pos);
        //左下角88显示小序号
        dis_code = 0;
        fill_data_d88_segbuf(dis_code, lcd_seg_buf+pos);
    }
    else if(Dis_Ctrl.cur_mode == DIS_MODE_MATCH)//搜索表 配对状态
    {
        Dis_Ctrl.had_write = 1;
        dis_code = Dis_Search_Ctrl.totnum;
        if(Dis_Search_Ctrl.state == CON_SEARCH_RUN_STATE)
        {
            fill_special_b8segbuf(1, lcd_seg_buf+pos);//显示正在搜索表号SEArch--
        }
        else if(Dis_Search_Ctrl.state == CON_SEARCH_DISPLY_STATE)
        {
            fill_search_addr_segbuf(lcd_seg_buf+pos);//填写要显示的表地址
        }
        else if(Dis_Search_Ctrl.state == CON_SEARCH_FINISH_STATE)
        {
            fill_search_set_state_segbuf(lcd_seg_buf+pos);//显示SET-Good or SET-Err
        }
        fill_data_d88_segbuf(dis_code, lcd_seg_buf+pos);//搜索到的表数量

        //填写特殊固定字符
        fill_dis_special_fix_char(lcd_seg_buf+pos);//象限

        //根据闪烁标志 填相关显示缓冲
        fill_dis_flash_flag(lcd_seg_buf+pos);
        //所有显示区数据为32字节
        pos += 32;
        lcd_write_data(BU97950_HEAD_ADDR, lcd_seg_buf, pos);
    }
    else
    {
//重新计算待显示数据地址 并把有些要显示的有规律的字符放在标志缓冲 后续填到全局显示缓冲
        memset(&dis_tcb, 0, sizeof(DIS_CTRL_TCB));

        if(Dis_Ctrl.cur_mode == DIS_MODE_AUTO)
        {
            disdi[0] = Dis_Ctrl.exd_lsn[DIS_MODE_AUTO];
        }
        else//键显或长按键显
        {
            disdi[0] = Dis_Ctrl.exd_lsn[DIS_MODE_SKEY];
        }
        di = Dis_Ctrl.di;

        for(i=0;i<4;i++)
        {
            disdi[i+1] = (di>>(8*i));
        }

        err = get_dis_info(&disdi[0], &data[0], &dis_tcb, lcd_seg_buf+pos);
        if(err == __FALSE)
        {
            //return __FALSE;
            goto id_dis_err_handle;
        }
    //开始填写显示数据区
        err = fill_dis_fix_b8_data(di, &dis_tcb, lcd_seg_buf+pos);
        if(err == __FALSE)
        {
            //return  __FALSE;
            goto id_dis_err_handle;
        }
    //填写显示轮显编码区
    /*
        if(Dis_Ctrl.cur_mode == DIS_MODE_AUTO)
        {
            dis_code = di;
        }
        else//键显或长按键显
        {
            dis_code = Dis_Ctrl.dis_bsn[DIS_MODE_SKEY]+1;
            dis_code = hex_to_bcd_in8 (dis_code);
        }
    */
        dis_code = di;
        fill_dis_fix_l8_data(dis_code, lcd_seg_buf+pos);

    //左下角88显示小序号
#if CODE_VER_TYPE==WLS_CUR_PRJ			//配合无线采集器
//azh 130930暂时用作测量点显示
        dis_code = get_dis_mp();
#else
        dis_code = HEX_TO_BCD(disdi[0], 1);
#endif
        fill_data_d88_segbuf(dis_code, lcd_seg_buf+pos);

    //填写主要是指有规律的那些字符 又没有定义在表格里的
        fill_dis_fix_char(&data[0], lcd_seg_buf+pos);

    //填写固定字符
        sn = dis_tcb.fixchartype;
        memcpy(data, fixchar_table[sn],8);
        fill_dis_fix_char(&data[0], lcd_seg_buf+pos);
    }

//掉电时 下面二项没必要显示
    if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
    {
//填写特殊固定字符
        fill_dis_special_fix_char(lcd_seg_buf+pos);//象限

//根据闪烁标志 填相关显示缓冲
        fill_dis_flash_flag(lcd_seg_buf+pos);
//所有显示区数据为32字节
    }

    pos += 32;
    lcd_write_data(BU97950_HEAD_ADDR, lcd_seg_buf, pos);

    return __TRUE;

//错误处理 没有数据标识或者其他问题
id_dis_err_handle:
    data[0] = 0xee;
    data[1] = 0xee;
    fill_data_b8segbuf(data, 4, lcd_seg_buf+pos);

//填写显示轮显编码区
/*
    if(Dis_Ctrl.cur_mode == DIS_MODE_AUTO)
    {
        dis_code = di;
    }
    else//键显或长按键显
    {
        dis_code = Dis_Ctrl.dis_bsn[DIS_MODE_SKEY]+1;
        dis_code = hex_to_bcd_in8 (dis_code);
    }
*/
    dis_code = di;
    fill_dis_fix_l8_data(dis_code, lcd_seg_buf+pos);

//左下角88显示小序号
    dis_code = HEX_TO_BCD(disdi[0], 1);
    fill_data_d88_segbuf(dis_code, lcd_seg_buf+pos);
//填写特殊固定字符
    fill_dis_special_fix_char(lcd_seg_buf+pos);//象限

//根据闪烁标志 填相关显示缓冲
    fill_dis_flash_flag(lcd_seg_buf+pos);
//所有显示区数据为32字节
    pos += 32;
    lcd_write_data(BU97950_HEAD_ADDR, lcd_seg_buf, pos);
    return __TRUE;
}
/******************************************************************************
** 函数名称: U8 judge_data_sign(U8 *buf, DIS_CTRL_TCB *pdistable)
** 函数功能: 根据数据标识及扩展的序号 判断此项数据是否是有符号数 是负数 则返回负号的位置
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
U8 judge_data_sign(U8 *buf, DIS_CTRL_TCB *pdistable)
{
    U8 r_valu = 0xff;
    U8 dix;
    U8 len;
    U8 data;

    dix = (pdistable->di>>24);//di3
    len = pdistable->datalen&0x0f;
    if((len > 4) || (len == 0))
    {
        return r_valu;
    }
    data = buf[len - 1];
    if((data == 0xee) || ((data & 0x80) == 0))//是无效数据 或者有符号数是正数
    {
        return r_valu;
    }

    switch(dix)
    {
        case 00:
            dix = (pdistable->di>>16);//di2
            if((dix == 0) || (dix == 3) || (dix == 4) || (dix == 0x17) || (dix == 0x18)//组合电能有符号数
                 || (dix == 0x2b) || (dix == 0x2c) || (dix == 0x3f) || (dix == 0x40))
            {
                r_valu = 8;//-xxxxxx.xx
            }
            break;
        case 01:
            dix = (pdistable->di>>16);//di2
            if((dix == 3) || (dix == 4) || (dix == 0x17) || (dix == 0x18)//组合电能有符号数
                 || (dix == 0x2b) || (dix == 0x2c) || (dix == 0x3f) || (dix == 0x40))
            {
                if(pdistable->litsn == 0)//只有真正的需量
                {
                    r_valu = 6;//-xx.xxxx
                }
            }
            break;
        case 02:
            dix = (pdistable->di>>16);//di2
            if((dix == 2) ||(dix == 3) ||(dix == 4))//电流 瞬时有无功率
            {
                r_valu = 6;//-xxx.xxx -xx.xxxx
            }
            else if(dix == 6)//功率因素
            {
                r_valu = 4;//-x.xxx
            }
            else if(dix == 0x80)//一分钟有功总平均功率 当前有功需量 当前无功需量
            {
                dix = (pdistable->di>>8);//di1
                if((dix == 3) ||(dix == 4) ||(dix == 5))
                {
                    r_valu = 6;//-xx.xxxx
                }
            }
            break;
    }
    if(r_valu <= 8)//清掉符号位
    {
        buf[len - 1] &= 0x7f;
    }
    return r_valu;
}
/******************************************************************************
** 函数名称: S8 fill_dis_fix_b8_data(U32 di, DIS_CTRL_TCB *pdistable, U8 *segbuf)
** 函数功能: 把要显示的数据放入显示数据区8个大号数据区
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
S8 fill_dis_fix_b8_data (U32 di, DIS_CTRL_TCB *pdistable, U8 *segbuf)
{
    U8 mem_type;
    U8 i;
    U8 *buf;
    U8 len;
    U32 data;
    U32 addr;

    data = 0;
    len = pdistable->datalen&0x0f;

    if(len > 4)
    {
        return  __FALSE;
    }

    //B1~B0: 存储模式 地址在传入时已经算好了
    switch((pdistable->specialctrl&0x03))
    {
        case 0:     //EEPROM
        {
            addr = pdistable->addr;
            break;
        }
        case 1:     //RAM
        {
            addr = pdistable->addr;
            break;
        }
        case 2:     //EEPROM带指针偏移的
            addr = pdistable->addr;
            break;
        default:
            return  __FALSE;
    }
    buf = alloc_comm_mem(MEM_64_TYPE, &mem_type, GET_MEM_ERR7);
    if(buf  != NULL)
    {
        file_read(addr, buf, len);
    }
    else
    {
//          memcpy(gusCurMemErrFunName, "fill_dis", 8);
        setmemgetnullid(GET_MEM_ERR7, gusCurMemErrFunName);
        return  __FALSE;
    }

    //B3~B2:数据格式
    //B4=0: 直接显示，=1:数据要转换格式显示

    if((pdistable->specialctrl&0x10) != 0)
    {
        switch(((pdistable->specialctrl>>2)&0x03))
        {
            case 0: //多字节转BCD
            {
                data = byte_merge(buf, len);//合并多个字节组合为一个U32数
                //B7=0: 无符号数，=1:有符号数
                if((pdistable->specialctrl&0x80) != 0)//有符号数
                {
                    //转成绝对值，记住负号标志用于显示
                    //todo
                }
                bytes_to_bcd(data, buf, 8);//返回8位BCD码
                len = 8;
                break;
            }
            case 1: //单字节转BCD
            {
                convert_to_bcd(buf, len);
                len >>=1;
                break;
            }
            case 2: //单字节转换成8位  要拼成4字节 便于后结统一处理LCD数据
            {
                data = buf[0];
                for(i=0; i<8; i++)
                {
                    if((i&2) == 0)
                    {
                        buf[i/2] = 0;
                    }
                    if((data>>i)&0x01)
                    {
                        buf[i/2] += (0x01<<((i%2)*4));
                    }
                }
                len = 8;
                break;
            }
            default:
            {
                free_mem_block(buf);//Mem_64_Byte
                return __FALSE;
            }
        }
    }
    else
    {
        //pdistable->datalen>>4 //8的个数
        len <<= 1;
    }
    i = judge_data_sign(buf, pdistable);
    //把buf里的数据填写到全局显示缓冲区
    if(i <= 8)
    {
        fill_data_sign(i, segbuf);//填写数据的符号位
    }
    //日期 时间要特殊处理
    if((di == 0x04000101) || (di == 0x04000102))
    {
        get_dis_date_time(di, buf, &len);
    }
    fill_data_b8segbuf(buf, len, segbuf);

    free_mem_block(buf);//Mem_64_Byte

    return __TRUE;
}
/******************************************************************************
** 函数名称: U16 get_dis_lsn_info (U32 di, DIS_AUX_CTRL const *pdistable, U16 max_num)
** 函数功能:
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
U16 get_dis_lsn_info (U32 di, DIS_AUX_CTRL const *pdistable, U16 max_num)
{
    U16 i;

    for(i=0; i<max_num; i++)
    {
        if(pdistable[i].di == di)
        {
            return i;
        }
    }
    return 0xffff;
}
/******************************************************************************
** 函数名称: void get_dis_sn_bydi (U32 di, U8 lsn, DIS_CTRL_TCB const *pdistable, U16 max_num)
** 函数功能:
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
U16 get_dis_sn_bydi (U32 di, U8 lsn, DIS_CTRL_TCB const *pdistable, U16 max_num)
{
    U16 sn;
    U16 i;

    //后续可用二分法进行搜表
    sn = 0xffff;
    for(i=0; i<max_num; i++)
    {
        if(pdistable[i].di == di)
        {
            if(pdistable[i].litsn == lsn)
            {
                sn = i;
                break;
            }
        }
    }
    return sn;
}
/******************************************************************************
** 函数名称: S8 fill_dis_fix_l8_data(U32 di, U8 *segbuf)
** 函数功能: 把要显示的轮显编码或显示序号放入8个小号数据区
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
S8 fill_dis_fix_l8_data (U32 di, U8 *segbuf)
{
    fill_data_l8segbuf((U8* )(&di), 8, segbuf);

    return __TRUE;
}
/******************************************************************************
** 函数名称: void handle_page_sn(U8 mode, U8 direct)
** 函数功能: 显示模式切换
** 入口参数:
**
**
** 返 回 值:
** 说    明: 每100ms调用
******************************************************************************/
void handle_page_sn(U8 mode, U8 direct)
{
    if(mode >= DIS_MODE_NUM)
    {
        mode = DIS_MODE_AUTO;
    }
    if(mode == DIS_MODE_LKEY)
    {
        mode = DIS_MODE_SKEY;
    }
    switch(direct)
    {
        case DN_SN_DIRECT:
        {
            Dis_Ctrl.flash_flag |= LSN_CHG_SN_FLAG;
            if(Dis_Ctrl.lsn_maxn[mode] <= 1)
            {
                Dis_Ctrl.flash_flag |= BSN_CHG_SN_FLAG;
                Dis_Ctrl.exd_lsn[mode] = 0;
                Dis_Ctrl.dis_bsn[mode]++;
                if(Dis_Ctrl.dis_bsn[mode] >= Dis_Para.maxn[mode])
                {
                    Dis_Ctrl.dis_bsn[mode] = 0;
                    if(mode == DIS_MODE_AUTO)
                    {
                        pd_autodis_finish = 1;//自动循环显示完一遍了 在掉电模式下有用 要进入深度掉电
                    }
                }
            }
            else
            {
                Dis_Ctrl.exd_lsn[mode]++;
                if(Dis_Ctrl.exd_lsn[mode] >= Dis_Ctrl.lsn_maxn[mode])
                {
                    Dis_Ctrl.exd_lsn[mode] = 0;
                    Dis_Ctrl.dis_bsn[mode]++;
                    Dis_Ctrl.flash_flag |= BSN_CHG_SN_FLAG;
                    if(Dis_Ctrl.dis_bsn[mode] >= Dis_Para.maxn[mode])
                    {
                        Dis_Ctrl.dis_bsn[mode] = 0;
                    }
                }
            }
            break;
        }
        case UP_SN_DIRECT:
        {
            Dis_Ctrl.flash_flag |= LSN_CHG_SN_FLAG;
            if(Dis_Ctrl.lsn_maxn[mode] <= 1)
            {
                Dis_Ctrl.flash_flag |= BSN_CHG_SN_FLAG;
                Dis_Ctrl.exd_lsn[mode] = 0;
                if(Dis_Ctrl.dis_bsn[mode] > 0)
                {
                    Dis_Ctrl.dis_bsn[mode]--;
                }
                else
                {
                    Dis_Ctrl.dis_bsn[mode] = Dis_Para.maxn[mode] - 1;
                }
            }
            else
            {
                if(Dis_Ctrl.exd_lsn[mode] > 0)
                {
                    Dis_Ctrl.exd_lsn[mode]--;
                }
                else
                {
                    Dis_Ctrl.flash_flag |= BSN_CHG_SN_FLAG;
                    if(Dis_Ctrl.dis_bsn[mode] > 0)
                    {
                        Dis_Ctrl.dis_bsn[mode]--;
                    }
                    else
                    {
                        Dis_Ctrl.dis_bsn[mode] = Dis_Para.maxn[mode] - 1;
                    }
                }
            }
        }
    }
}
/******************************************************************************
** 函数名称: void lcd_mode_ctrl(void)
** 函数功能: 显示模式切换
** 入口参数:
**
**
** 返 回 值:
** 说    明: 每100ms调用
******************************************************************************/
void lcd_mode_ctrl (void)
{
    if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态
    {
        if(__TRUE == check_prog_key_state())//编程键状态变化
        {
            Dis_Ctrl.flash_flag |= MOD_CHG_SN_FLAG;//为了实时显示 作为模式有变化
        }

        judge_search_state();
    }

    switch(Dis_Ctrl.cur_mode)
    {
        case DIS_MODE_MATCH://搜索表 配对状态
        {
            handle_search_state();
            break;
        }
        case DIS_MODE_ALLON://上电全显
        {
            if(Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP))
            {
                Dis_Ctrl.lpress_ct = DIS_TIME_STLPKEY;
                Dis_Ctrl.cur_mode = DIS_MODE_SKEY;
                Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_SKEY];
                Dis_Ctrl.flash_flag |= MOD_CHG_SN_FLAG;
                if(Key_CurState & CON_KEYBIT_UP)
                {
                    Dis_Ctrl.pre_key = CON_KEYBIT_UP;
                    Dis_Ctrl.cur_diret = UP_SN_DIRECT;
                }
                else
                {
                    Dis_Ctrl.pre_key = CON_KEYBIT_DN;
                    Dis_Ctrl.cur_diret = DN_SN_DIRECT;
                }
            }
            else
            {
                if(Dis_Ctrl.t_count > 0)
                {
                    Dis_Ctrl.t_count--;
                }
                if(Dis_Ctrl.t_count == 0)
                {

                    Dis_Ctrl.cur_mode = DIS_MODE_AUTO;
                    Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_AUTO];
                    Dis_Ctrl.flash_flag |= MOD_CHG_SN_FLAG;
                    Dis_Ctrl.cur_diret = DN_SN_DIRECT;
                }
                Dis_Ctrl.pre_key = CON_KEYBIT_NO;
            }
            break;
        }
        case DIS_MODE_ERRON://严重错误 停在错误屏 按键可以显示其他
        {
            if(Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP))
            {
                Dis_Ctrl.lpress_ct = DIS_TIME_STLPKEY;
                Dis_Ctrl.cur_mode = DIS_MODE_SKEY;
                Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_SKEY];
                Dis_Ctrl.flash_flag |= MOD_CHG_SN_FLAG;
                if(Key_CurState & CON_KEYBIT_UP)
                {
                    Dis_Ctrl.pre_key = CON_KEYBIT_UP;
                    Dis_Ctrl.cur_diret = UP_SN_DIRECT;
                }
                else
                {
                    Dis_Ctrl.pre_key = CON_KEYBIT_DN;
                    Dis_Ctrl.cur_diret = DN_SN_DIRECT;
                }
            }
            else
            {
                if(__TRUE != check_meter_serious_happened())//错误解除 进入自动轮显
                {
                    Dis_Ctrl.cur_mode = DIS_MODE_AUTO;
                    Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_AUTO];
                    Dis_Ctrl.flash_flag |= MOD_CHG_SN_FLAG;
                    Dis_Ctrl.cur_diret = DN_SN_DIRECT;
                    Dis_Ctrl.pre_key = CON_KEYBIT_NO;
                }
            }
            break;
        }
        case DIS_MODE_AUTO:
        {
            if(Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP))
            {
                Dis_Ctrl.lpress_ct = DIS_TIME_STLPKEY;
                Dis_Ctrl.cur_mode = DIS_MODE_SKEY;
                Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_SKEY];
                Dis_Ctrl.flash_flag |= MOD_CHG_SN_FLAG;
                if(Key_CurState & CON_KEYBIT_UP)
                {
//从自动轮显示到键显第一屏切过去还是原来的序号比较合理
/*
                    if(Dis_Ctrl.dis_bsn[DIS_MODE_SKEY] > 0)
                    {
                        Dis_Ctrl.dis_bsn[DIS_MODE_SKEY]--;
                    }
                    else
                    {
                        Dis_Ctrl.dis_bsn[DIS_MODE_SKEY] = Dis_Para.maxn[DIS_MODE_SKEY] - 1;
                    }
*/
                    Dis_Ctrl.pre_key = CON_KEYBIT_UP;
                    Dis_Ctrl.cur_diret = UP_SN_DIRECT;
                }
                else
                {
/*                  Dis_Ctrl.dis_bsn[DIS_MODE_SKEY]++;
                    if(Dis_Ctrl.dis_bsn[DIS_MODE_SKEY] >= Dis_Para.maxn[DIS_MODE_SKEY])
                    {
                        Dis_Ctrl.dis_bsn[DIS_MODE_SKEY] = 0;
                    }
*/                  Dis_Ctrl.pre_key = CON_KEYBIT_DN;
                    Dis_Ctrl.cur_diret = DN_SN_DIRECT;
                }
            }
            else
            {
                if(Dis_Ctrl.t_count > 0)
                {
                    Dis_Ctrl.t_count--;
                }
                if(Dis_Ctrl.t_count == 0)
                {
                    Dis_Ctrl.cur_diret = DN_SN_DIRECT;
                    handle_page_sn(DIS_MODE_AUTO, DN_SN_DIRECT);
                    Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_AUTO];
                }
                Dis_Ctrl.pre_key = CON_KEYBIT_NO;
            }
            break;
        }
        case DIS_MODE_SKEY:
        {
            if(Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP))
            {
                if(Dis_Ctrl.pre_key == CON_KEYBIT_NO)//按键模式再次按下键
                {
                    if(Key_CurState & CON_KEYBIT_UP)
                    {
                        Dis_Ctrl.cur_diret = UP_SN_DIRECT;
                        handle_page_sn(DIS_MODE_SKEY, UP_SN_DIRECT);
                        Dis_Ctrl.pre_key = CON_KEYBIT_UP;
                    }
                    else
                    {
                        Dis_Ctrl.cur_diret = DN_SN_DIRECT;
                        handle_page_sn(DIS_MODE_SKEY, DN_SN_DIRECT);
                        Dis_Ctrl.pre_key = CON_KEYBIT_DN;
                    }

                }
                else//长按倒计时开始
                {
                    if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态才有长按模式
                    {
                        if(Dis_Ctrl.lpress_ct > 0)
                        {
                            Dis_Ctrl.lpress_ct--;
                        }
                        if(Dis_Ctrl.lpress_ct == 0)
                        {
                            if((Key_CurState == CON_KEYBIT_DN) && (Dis_Ctrl.pre_key == CON_KEYBIT_DN))
                            {
                                Dis_Ctrl.cur_diret = DN_SN_DIRECT;
                                Dis_Ctrl.cur_mode = DIS_MODE_LKEY;
                                Dis_Ctrl.lpress_ct = DIS_TIME_LPKEY;
                            }
                            else if((Key_CurState == CON_KEYBIT_UP) && (Dis_Ctrl.pre_key == CON_KEYBIT_UP))
                            {
                                Dis_Ctrl.cur_diret = UP_SN_DIRECT;
                                Dis_Ctrl.cur_mode = DIS_MODE_LKEY;
                                Dis_Ctrl.lpress_ct = DIS_TIME_LPKEY;
                            }
                            else
                            {
                                Dis_Ctrl.lpress_ct = DIS_TIME_STLPKEY;
                                Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_SKEY];
                                Dis_Ctrl.pre_key = (Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP));
                            }
                        }
                    }
                    else//在掉电情况 有时会丢掉按键弹起中断 所以这里处理掉
                    {
                        if(PORT0_IS_HIGH(P0_KEYUP))
                        {
                            Key_CurState &= ~CON_KEYBIT_UP;
                        }
                        if(PORT0_IS_HIGH(P0_KEYDOWN))
                        {
                            Key_CurState &= ~CON_KEYBIT_DN;
                        }
                    }

                }
            }
            else//按键已松开
            {
                if(Dis_Ctrl.pre_key & (CON_KEYBIT_DN | CON_KEYBIT_UP))//刚松开键显时间才计时
                {
                    Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_SKEY];
                }
                Dis_Ctrl.lpress_ct = DIS_TIME_STLPKEY;
                if(Dis_Ctrl.t_count > 0)
                {
                    Dis_Ctrl.t_count--;
                }
                else
                {
                    Dis_Ctrl.cur_mode = DIS_MODE_AUTO;
                    Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_AUTO];
                    Dis_Ctrl.flash_flag |= MOD_CHG_SN_FLAG;
                    Dis_Ctrl.cur_diret = DN_SN_DIRECT;
                }
                Dis_Ctrl.pre_key = CON_KEYBIT_NO;
            }

            break;
        }
        case DIS_MODE_LKEY:
        {
            if(Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP))
            {
                if(Dis_Ctrl.lpress_ct > 0)
                {
                    Dis_Ctrl.lpress_ct--;
                }
                if(Dis_Ctrl.lpress_ct == 0)
                {
                    if(Dis_Ctrl.pre_key & CON_KEYBIT_UP)
                    {
                        Dis_Ctrl.cur_diret = UP_SN_DIRECT;
                        handle_page_sn(DIS_MODE_SKEY, UP_SN_DIRECT);
                        Dis_Ctrl.pre_key = CON_KEYBIT_UP;
                    }

                    else
                    {
                        Dis_Ctrl.cur_diret = DN_SN_DIRECT;
                        handle_page_sn(DIS_MODE_SKEY, DN_SN_DIRECT);
                        Dis_Ctrl.pre_key = CON_KEYBIT_DN;
                    }
                    Dis_Ctrl.lpress_ct = DIS_TIME_LPKEY;
                }
            }
            else//长按键已松开 由长按模式进入按键模式 这二种不算真正的模式切换
            {
                Dis_Ctrl.lpress_ct = DIS_TIME_STLPKEY;
                Dis_Ctrl.cur_mode = DIS_MODE_SKEY;
                Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_SKEY];
                Dis_Ctrl.pre_key = CON_KEYBIT_NO;
            }
            break;
        }
    }
    if(Dis_Ctrl.cur_mode == DIS_MODE_AUTO)
    {
        if(__TRUE == check_meter_serious_happened())//错误解除 进入自动轮显
        {
            Dis_Ctrl.cur_mode = DIS_MODE_ERRON;
            Dis_Ctrl.flash_flag |= MOD_CHG_SN_FLAG;
        }
    }
    if(Dis_Ctrl.flash_flag & (BSN_CHG_SN_FLAG | MOD_CHG_SN_FLAG))//刚上电或大序号或模式有变化 //PON_INT_SN_FLAG
    {
        get_dis_code(Dis_Ctrl.cur_mode);//得到显示编码 并更新Dis_Ctrl显示控制结构变量
    }

    lcd_backlight_ctrl(Dis_Ctrl.cur_mode);//背光控制
}
/******************************************************************************
** 函数名称: void lcd_backlight_ctrl(U8 mode)
** 函数功能: 背光控制 上电及按键后延迟15秒关闭背光 外部掉电情况不打开
** 入口参数:
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void lcd_backlight_ctrl(U8 mode)
{
    if(PORT0_IS_HIGH(P0_PD_CHK) != 0)   //检测掉电信号输入//外部有电
    {
        if((mode == DIS_MODE_ALLON) ||(mode == DIS_MODE_SKEY) ||(mode == DIS_MODE_LKEY))
        {
            BGLED_ON();//打开液晶背光电源
            gBackLight_On_Time = DIS_TIME_BLIGHT;//10s
        }
        else if((mode == DIS_MODE_MATCH) && ((Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP)) != 0))
        {
            BGLED_ON();//打开液晶背光电源
            gBackLight_On_Time = DIS_TIME_BLIGHT;//10s
        }
        else
        {
            if(gBackLight_On_Time > DIS_TIME_BLIGHT)
            {
                gBackLight_On_Time = DIS_TIME_BLIGHT;
            }
            if(gBackLight_On_Time > 0)
            {
                gBackLight_On_Time--;
            }
            else
            {
                BGLED_OFF();
            }
        }
    }

}
/******************************************************************************
** 函数名称: void set_dis_para_flag (U8 id)
** 函数功能: 判断轮显或者键显参数是否有效(显示编码及显示时间有设置过就认为有效)
** 入口参数: 无 标志
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void set_dis_para_flag (U8 id)
{
    U8  data[PAR_DIS_VALID_FLAG_LEN];


//    switch(id)
//    {
//      case 0://轮显屏数
//          sn = 0;
//          break;
//      case 1://轮显时间
//          sn = 1;
//           break;
//      case 2://轮显编码
//          sn = 2;
//          break;
//      case 3://键显屏数
//          sn = 3;
//          break;
//      case 4://键显编码
//          sn = 4;
//          break;
//      }
    if(id < (PAR_DIS_VALID_FLAG_LEN-1))
    {
        file_read(PAR_DIS_VALID_FLAG_ADDR, data, PAR_DIS_VALID_FLAG_LEN);
        if(data[PAR_DIS_VALID_FLAG_LEN-1] != addverify(data, (PAR_DIS_VALID_FLAG_LEN-1)))
        {
            memset(data, 0, (PAR_DIS_VALID_FLAG_LEN-1));
        }
        data[id] = CON_DIS_PARA_VALID;
        data[PAR_DIS_VALID_FLAG_LEN-1] = addverify(data, (PAR_DIS_VALID_FLAG_LEN-1));
        file_write(PAR_DIS_VALID_FLAG_ADDR, data, PAR_DIS_VALID_FLAG_LEN);
    }
}
/******************************************************************************
** 函数名称: U8 check_dis_para_valid(U8 mode)
** 函数功能: 判断轮显或者键显参数是否有效(显示编码及显示时间有设置过就认为有效)
** 入口参数: 无 mode=DIS_MODE_AUTO:轮显参数 mode=DIS_MODE_SKEY:查询键显参数
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
U8 check_dis_para_valid (U8 mode)
{
    U8  data[PAR_DIS_VALID_FLAG_LEN];

    file_read(PAR_DIS_VALID_FLAG_ADDR, data, PAR_DIS_VALID_FLAG_LEN);
    if(data[PAR_DIS_VALID_FLAG_LEN-1] == addverify(data, (PAR_DIS_VALID_FLAG_LEN-1)))
    {
      switch(mode)
      {
        case DIS_MODE_AUTO:
            //轮显屏数 轮显时间 轮显编码
            if((data[0] == CON_DIS_PARA_VALID) && (data[1] == CON_DIS_PARA_VALID) && (data[2] == CON_DIS_PARA_VALID))
            {
               return CON_DIS_PARA_VALID;
            }
            break;
        case DIS_MODE_SKEY:
            //键显屏数及键显编码
            if((data[3] == CON_DIS_PARA_VALID) && (data[4] == CON_DIS_PARA_VALID))//键显退出时间没有地方设置是默认值15秒
            {
               return CON_DIS_PARA_VALID;
            }
            break;
      }
    }
    return CON_DIS_PARA_INVALID;
}
/******************************************************************************
** 函数名称: void lcd_init(U8 mode)
** 函数功能: 显示控制及数据初始化 判断键显或轮显参数是否有效 无效就读默认显示参数
** 入口参数: 无 mode=0:上电初始化 mode=1:参数设置后调用
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void lcd_init(U8 mode)
{
    U8 i;

#if CODE_VER_TYPE==WLS_CUR_PRJ			//配合无线采集器
       Dis_Para.maxn[DIS_MODE_AUTO] = DEFAULT_AUTO_DIS_MAX_NUM;
       Dis_Para.dly[DIS_MODE_AUTO] = DIS_TIME_AUTO;
       Dis_Para.para_valid[DIS_MODE_AUTO] = CON_DIS_PARA_INVALID;

       Dis_Para.maxn[DIS_MODE_SKEY] = DEFAULT_KEY_DIS_MAX_NUM;
       Dis_Para.dly[DIS_MODE_SKEY] = DIS_TIME_SKEY;
       Dis_Para.para_valid[DIS_MODE_SKEY] = CON_DIS_PARA_INVALID;
#else
    U8 rev_buff[5];

    file_read(PAR_LPDISCNT_04000301_00_ADDR, rev_buff, 5);//依次读取关于循显&键显相关参数具体参见PAR_LPDISCNT_04000301_00_ADDR定义处

    if(CON_DIS_PARA_VALID == check_dis_para_valid(DIS_MODE_AUTO))
    {
        Dis_Para.maxn[DIS_MODE_AUTO] = bcd_to_hex(rev_buff[0]);
        Dis_Para.dly[DIS_MODE_AUTO] = (bcd_to_hex(rev_buff[1]) * 10);//
        if(Dis_Para.dly[DIS_MODE_AUTO] > 30)
        {
           Dis_Para.dly[DIS_MODE_AUTO] = DIS_TIME_AUTO;
        }
        Dis_Para.para_valid[DIS_MODE_AUTO] = CON_DIS_PARA_VALID;
    }
    else
    {
       Dis_Para.maxn[DIS_MODE_AUTO] = DEFAULT_AUTO_DIS_MAX_NUM;
       Dis_Para.dly[DIS_MODE_AUTO] = DIS_TIME_AUTO;
       Dis_Para.para_valid[DIS_MODE_AUTO] = CON_DIS_PARA_INVALID;
    }

    if(CON_DIS_PARA_VALID == check_dis_para_valid(DIS_MODE_SKEY))
    {
        Dis_Para.maxn[DIS_MODE_SKEY] = bcd_to_hex(rev_buff[PAR_KEYDIS_04000305_00_ADDR - PAR_LPDISCNT_04000301_00_ADDR]);
        Dis_Para.dly[DIS_MODE_SKEY] = DIS_TIME_SKEY; //按键显示退出时间 没有设置的地方 就默认15秒
        Dis_Para.para_valid[DIS_MODE_SKEY] = CON_DIS_PARA_VALID;
    }
    else
    {
       Dis_Para.maxn[DIS_MODE_SKEY] = DEFAULT_KEY_DIS_MAX_NUM;
       Dis_Para.dly[DIS_MODE_SKEY] = DIS_TIME_SKEY;
       Dis_Para.para_valid[DIS_MODE_SKEY] = CON_DIS_PARA_INVALID;
    }
#endif


    for(i=0; i<2; i++)
    {
        Dis_Ctrl.dis_bsn[i] = 0;
        Dis_Ctrl.exd_lsn[i] = 0;
        Dis_Ctrl.lsn_maxn[i] = 1;
        if(Dis_Para.dly[i] == 0)
        {
           Dis_Para.dly[i] = 1;
        }
    }
/*
    file_read(PAR_LPDISCNT_04000301_00_ADDR, rev_buff, 5);//依次读取关于循显&键显相关参数具体参见PAR_LPDISCNT_04000301_00_ADDR定义处
    for(i=0; i<2; i++)
    {
        Dis_Para.maxn[i] = bcd_to_hex(rev_buff[i * (PAR_KEYDIS_04000305_00_ADDR - PAR_LPDISCNT_04000301_00_ADDR)]);
        if(Dis_Para.maxn[i] == 0xff)//外存数据无效时,取默认值
        {
            Dis_Para.maxn[i] = DEBUG_DIS_MAX_NUM;
        }
        Dis_Ctrl.dis_bsn[i] = 0;
        Dis_Ctrl.exd_lsn[i] = 0;
        Dis_Ctrl.lsn_maxn[i] = 1;
    }
    Dis_Para.dly[DIS_MODE_AUTO] = (bcd_to_hex(rev_buff[1]) * 10);//
    Dis_Para.dly[DIS_MODE_SKEY] = (bcd_to_hex(rev_buff[1]) * 10);//键显时间规约未提,暂时与轮显共用
    if(rev_buff[1] == 0xff)//外存数据无效时,取默认值
    {
        Dis_Para.dly[DIS_MODE_AUTO] = DIS_TIME_AUTO;
        Dis_Para.dly[DIS_MODE_SKEY] = DIS_TIME_SKEY;
    }
*/
    Dis_Ctrl.cur_mode = DIS_MODE_AUTO;
    Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_AUTO];
    Dis_Ctrl.pre_key = CON_KEYBIT_NO;
    Dis_Ctrl.cur_diret = DN_SN_DIRECT;

    if(mode == 0)
    {
        Dis_Ctrl.flash_flag = PON_INT_SN_FLAG;
        if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
        {
            Dis_Ctrl.cur_mode = DIS_MODE_ALLON;
            Dis_Ctrl.t_count = 60;//上电全显示6s
            lcd_seg_buf[LCD_BUF_LENTH-1] = 0x5a;//显示缓冲有数据填充过 就可以0.5s刷屏了
        }
    }
    Dis_Ctrl.lpress_ct = DIS_TIME_STLPKEY;

    Dis_Ctrl.dis_dly_time = 0;
    Dis_Ctrl.had_write = 0;

    get_dis_code(Dis_Ctrl.cur_mode);
}
/******************************************************************************
** 函数名称: void get_dis_code(U8 mode)
** 函数功能: 得到显示编码 并更新到Dis_Ctrl显示控制结构变量
**           是部规4字节显示编码 转换成5字节编码
** 入口参数: 无
** 返 回 值: 无
** 说    明: 只有在大的显示序号变化时才进来
******************************************************************************/
void get_dis_code (U8 mode)
{
    U32 di;
    U8  i;
    U16 sn;
    U8  dix;
    U8  lsn;
    U8  rev_buff[5];

    if(mode == DIS_MODE_ALLON)
    {
        mode = DIS_MODE_AUTO;
    }
    else if(mode == DIS_MODE_ERRON)
    {
        mode = DIS_MODE_SKEY;
    }

    if(mode == DIS_MODE_AUTO)
    {
        mode = DIS_MODE_AUTO;
        sn = Dis_Ctrl.dis_bsn[mode];//获取当前显示大屏序号
        if(sn > Dis_Para.maxn[mode])
        {
            sn = 0;
            Dis_Ctrl.dis_bsn[mode] = 0;
        }

#if CODE_VER_TYPE==WLS_CUR_PRJ			//配合无线采集器
//显示固定键显表格
          if(sn > DEFAULT_AUTO_DIS_MAX_NUM)
          {
              Dis_Para.maxn[mode] = DEFAULT_AUTO_DIS_MAX_NUM;
              sn = 0;
              Dis_Ctrl.dis_bsn[mode] = 0;
          }
           memcpy(rev_buff, &default_key_dis_tab[sn][0], 5);
#else
        if(Dis_Para.para_valid[DIS_MODE_AUTO] == CON_DIS_PARA_VALID)
        {
            file_read(PAR_LOOPDIS1_04040101_00_ADDR + sn * 5, rev_buff, PAR_LOOPDIS1_04040101_TOT_LEN); //读取显示项5字节DI
        }
        else
        {
            if(sn > DEFAULT_AUTO_DIS_MAX_NUM)
            {
                Dis_Para.maxn[mode] = DEFAULT_AUTO_DIS_MAX_NUM;
                sn = 0;
                Dis_Ctrl.dis_bsn[mode] = 0;
            }
            memcpy(rev_buff, &default_auto_dis_tab[sn][0], 5);
        }
#endif

    }
    else
    {
        mode = DIS_MODE_SKEY;
        sn = Dis_Ctrl.dis_bsn[mode];//获取当前显示大屏序号
        if(sn > Dis_Para.maxn[mode])
        {
            sn = 0;
            Dis_Ctrl.dis_bsn[mode] = 0;
        }
#if CODE_VER_TYPE==WLS_CUR_PRJ			//配合无线采集器
//显示固定键显表格
          if(sn > DEFAULT_KEY_DIS_MAX_NUM)
          {
              Dis_Para.maxn[mode] = DEFAULT_AUTO_DIS_MAX_NUM;
              sn = 0;
              Dis_Ctrl.dis_bsn[mode] = 0;
          }
          memcpy(rev_buff, &default_key_dis_tab[sn][0], 5);
#else
        if(Dis_Para.para_valid[DIS_MODE_SKEY] == CON_DIS_PARA_VALID)
        {
            file_read(PAR_KEYDIS1_04040201_00_ADDR + sn * 5, rev_buff, PAR_KEYDIS1_04040201_TOT_LEN);   //读取显示项5字节DI
        }
        else
        {
            if(sn > DEFAULT_KEY_DIS_MAX_NUM)
            {
                Dis_Para.maxn[mode] = DEFAULT_AUTO_DIS_MAX_NUM;
                sn = 0;
                Dis_Ctrl.dis_bsn[mode] = 0;
            }
            memcpy(rev_buff, &default_key_dis_tab[sn][0], 5);
        }
#endif        
    }
//国网上定义扩展序号在最高字节
    lsn = rev_buff[4];  //获取扩展DI(小序号)
    dix = rev_buff[3];  //获取扩展DI3
    di = 0;
    for(i = 0; i < 4; i++)
    {
        di += (rev_buff[i] << 8*i);
    }

    Dis_Ctrl.di = di;
    if(lsn == 0xff)//是部规4字节显示编码 转换成5字节编码
    {
        Dis_Ctrl.exd_lsn[mode] = 0;//默认值
        Dis_Ctrl.lsn_maxn[mode] = 1;
        switch(dix)
        {
            case 0x00:
            case 0x02:
            {
                break;
            }
            case 0x01:
            {
                Dis_Ctrl.exd_lsn[mode] = 0;
                Dis_Ctrl.lsn_maxn[mode] = 3;
                if(Dis_Ctrl.cur_diret == UP_SN_DIRECT)
                {
                    Dis_Ctrl.exd_lsn[mode] = Dis_Ctrl.lsn_maxn[mode] - 1;
                }
                break;
            }
            case 0x03:
            {
                if((Dis_Ctrl.di&0x00ff0000) == 0x00100000)
                {
                    di = (Dis_Ctrl.di&0xffffff00);
                }
                else
                {
                    di = (Dis_Ctrl.di&0xffffff00);
                    if((Dis_Ctrl.di&0xff) != 0)//从di0=1开始都归类为1去搜索
                    {
                        di += 0x01;
                    }
                }
                sn = get_dis_lsn_info(di, evet_lsn_tab, DIS_AUX_EVET_NUM);
                if(sn != 0xffff)
                {
                    Dis_Ctrl.exd_lsn[mode] = evet_lsn_tab[sn].sn;
                    Dis_Ctrl.lsn_maxn[mode] = evet_lsn_tab[sn].maxn;
                    if(Dis_Ctrl.cur_diret == UP_SN_DIRECT)
                    {
                        Dis_Ctrl.exd_lsn[mode] = Dis_Ctrl.lsn_maxn[mode] - 1;
                    }
                }
                break;
            }
            case 0x04:
            {
                sn = get_dis_lsn_info(Dis_Ctrl.di, para_lsn_tab, DIS_AUX_PARA_NUM);
                if(sn != 0xffff)
                {
                    Dis_Ctrl.exd_lsn[mode] = para_lsn_tab[sn].sn;
                    Dis_Ctrl.lsn_maxn[mode] = para_lsn_tab[sn].maxn;
                    if(Dis_Ctrl.cur_diret == UP_SN_DIRECT)
                    {
                        Dis_Ctrl.exd_lsn[mode] = Dis_Ctrl.lsn_maxn[mode] - 1;
                    }
                }
                break;
            }
        }
    }
    else//是扩展的5字节显示编码
    {
        Dis_Ctrl.exd_lsn[mode] = lsn;//
        Dis_Ctrl.lsn_maxn[mode] = 1;
    }
}
/******************************************************************************
** 函数名称: void fill_dis_flash_data (void)
** 函数功能: 得到显示编码 填写闪烁字符
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void fill_dis_flash_data (void)
{
    if(Dis_Ctrl.cur_mode == DIS_MODE_ALLON)
    {
        return;
    }
    //todo
    //先刷新相关闪烁标志 然后把上次写过的数据原封不动一起写到驱动芯片
    fill_dis_flash_flag(lcd_seg_buf+CON_DIS_DATA_OFFSET);//根据闪烁标志 填相关显示缓冲

    lcd_write_data(BU97950_HEAD_ADDR, lcd_seg_buf, 32+CON_DIS_DATA_OFFSET);
}
/******************************************************************************
** 函数名称: void get_flash_default_flag(void)
** 函数功能: 根据实时的电表状态字 转化成对应的闪烁标志0.5秒刷新
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void get_flash_default_flag (void)
{
    U8 i;

    gState_Flash_Flag = 0;
    for(i=0; i<CON_FLASH_BIT_MAX_NUM; i++)
    {
        gState_Flash_Flag |= ((flash_bit_default_value_tab[i] & 0x01) << i);
    }
}
/******************************************************************************
** 函数名称: void rollback_flash_flag (void)
** 函数功能: 根据实时的电表状态字 转化成对应的闪烁标志0.5秒 取反
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void rollback_flash_flag (void)
{
    U8 i,flag;
    U8 sn;
    U8 bit_num;

    for(i=0; i<CON_FLASH_BIT_MAX_NUM; i++)
    {
        if(96 == meter_state_flash_bit_tab[i])//判断逆相序
        {
            flag = ((gMeter_State[6] & 0x01) | ((gMeter_State[6]>>1) & 0x01));//电压和电流逆相序或在一起判断
            if(flag == 1)
            {
                if(gState_Flash_Flag & (1 << i))
                {
                    gState_Flash_Flag &= ~(1 << i);
                }
                else
                {
                    gState_Flash_Flag |= (1 << i);
                }
            }
            else
            {
                gState_Flash_Flag &= ~(1 << i);
                gState_Flash_Flag |= ((flash_bit_default_value_tab[i] & 0x01) << i);
            }
        }
        else
        {
            sn = meter_state_flash_bit_tab[i] / 16;
            bit_num = meter_state_flash_bit_tab[i] % 16;
            if(gMeter_State[sn] & (1 << bit_num))//出现异常时
            {
                if(gState_Flash_Flag & (1 << i))
                {
                    gState_Flash_Flag &= ~(1 << i);
                }
                else
                {
                    gState_Flash_Flag |= (1 << i);
                }
            }
            else//状态正常时(如果是失压类 还得判断是否断相类)
            {
                if(flash_bit_default_value_tab[i] & 0x80)//如果高位置位 则还得判断一种状态 如断相
                {
                    sn = meter_state_flash_bit_spectab[i] / 16;
                    bit_num = meter_state_flash_bit_spectab[i] % 16;
                    if(gMeter_State[sn] & (1 << bit_num))//出现异常时 有断相 或断流出现
                    {
                        gState_Flash_Flag &= ~(1 << i);//断相不显示
                    }
                    else
                    {
                        gState_Flash_Flag &= ~(1 << i);
                        gState_Flash_Flag |= ((flash_bit_default_value_tab[i] & 0x01) << i);
                    }
                }
                else
                {
                    gState_Flash_Flag &= ~(1 << i);
                    gState_Flash_Flag |= (flash_bit_default_value_tab[i] << i);
                }
            }
        }
    }
    if(Meter_LED_Cnt[RS4851_CHL] > 0)//电话机1
    {
        if(gState_Com_Flag & CON_COM_RS4851_FLASH_BIT)
        {
            gState_Com_Flag &= ~CON_COM_RS4851_FLASH_BIT;
        }
        else
        {
            gState_Com_Flag |= CON_COM_RS4851_FLASH_BIT;
        }
        if(Meter_LED_Cnt[RS4851_CHL] > LED_FLASH_TIMES)
        {
            Meter_LED_Cnt[RS4851_CHL] = LED_FLASH_TIMES;
        }
        Meter_LED_Cnt[RS4851_CHL]--;
    }
    else
    {
        gState_Com_Flag &= ~CON_COM_RS4851_FLASH_BIT;
    }
    if(Meter_LED_Cnt[RS4852_CHL] > 0)//电话机2
    {
        if(gState_Com_Flag & CON_COM_RS4852_FLASH_BIT)
        {
            gState_Com_Flag &= ~CON_COM_RS4852_FLASH_BIT;
        }
        else
        {
            gState_Com_Flag |= CON_COM_RS4852_FLASH_BIT;
        }
        if(Meter_LED_Cnt[RS4852_CHL] > LED_FLASH_TIMES)
        {
            Meter_LED_Cnt[RS4852_CHL] = LED_FLASH_TIMES;
        }
        Meter_LED_Cnt[RS4852_CHL]--;
    }
    else
    {
        gState_Com_Flag &= ~CON_COM_RS4852_FLASH_BIT;
    }

//azh 120314 增加电池1充电电池 及电池2停显电池的闪烁
//电池状态：0：正常(灭) 1：欠压(闪烁) 2：没加电池(亮)
#ifdef PD_MANAGE_VER12
    flag = get_batt_state(BAT_GPRS);    //仅用于V1.2主板4.8V电池充电管理
#else
    flag = get_battery_state(BAT_GPRS); //用于V1.3及以上主板升压方案兼容2.4V与4.8V电池充电管理
#endif
    if(flag==BAT_LOW)//欠压
    {
        if(gState_Com_Flag & CON_COM_GPRS_BAT_BIT)
        {
            gState_Com_Flag &= ~CON_COM_GPRS_BAT_BIT;
        }
        else
        {
            gState_Com_Flag |= CON_COM_GPRS_BAT_BIT;
        }        
    }
    else if(flag==BAT_NO)//没加电池
    {
        gState_Com_Flag |= CON_COM_GPRS_BAT_BIT;
    }
    else//正常
    {
        gState_Com_Flag &= ~CON_COM_GPRS_BAT_BIT;
    }            
//
#ifdef PD_MANAGE_VER12
    flag = get_batt_state(BAT_PDB);    //仅用于V1.2主板4.8V电池充电管理(对停显电池无所谓)
#else
    flag = get_battery_state(BAT_PDB); //用于V1.3及以上主板升压方案兼容2.4V与4.8V电池充电管理(对停显电池无所谓)
#endif
    if(flag==BAT_LOW)//欠压
    {
        if(gState_Com_Flag & CON_COM_DIS_BAT_BIT)
        {
            gState_Com_Flag &= ~CON_COM_DIS_BAT_BIT;
        }
        else
        {
            gState_Com_Flag |= CON_COM_DIS_BAT_BIT;
        }        
    }
    else if(flag==BAT_NO)//没加电池
    {
        gState_Com_Flag |= CON_COM_DIS_BAT_BIT;
    }
    else//正常
    {
        gState_Com_Flag &= ~CON_COM_DIS_BAT_BIT;
    }            
}

/******************************************************************************
 Function name:  void lcd_display_flash(U8 cnt)
 Author       :
 Description  :
 Input        :  None
 Return       :  None
 *****************************************************************************/
void lcd_display_flash (U8 cnt)
{

//    lcd_mode_ctrl();//轮显或键显控制处理
    if((Dis_Ctrl.cur_mode == DIS_MODE_ALLON) || (Dis_Ctrl.cur_mode == DIS_MODE_ERRON)
        || (Dis_Ctrl.cur_mode == DIS_MODE_MATCH))
    {
        if(((cnt%10) == 0) || ((Dis_Ctrl.flash_flag & BSN_CHG_SN_FLAG) == BSN_CHG_SN_FLAG))
        {
            fill_dis_data();
            if((Dis_Ctrl.flash_flag & BSN_CHG_SN_FLAG) == BSN_CHG_SN_FLAG)
            {
                Dis_Ctrl.flash_flag &= ~BSN_CHG_SN_FLAG;
            }
        }
        return;
    }
#if CODE_VER_TYPE==H_METER_PRJ				//配合高压表
    if(((cnt%10) == 0) ||
       (Dis_Ctrl.flash_flag & (PON_INT_SN_FLAG | LSN_CHG_SN_FLAG | MOD_CHG_SN_FLAG)))//1秒到或者有变化就重填显示数据
    {
        Dis_Ctrl.dis_dly_time = 0x83;//bit7是标志 bit6~bit0是dly计数 即延时100ms 等数据同步后再去刷新显示
        Dis_Ctrl.di_finish = 0;
        Dis_Ctrl.di_resend_cnt = 0;
        if(__FALSE == dis_ctrl_sync())//如果不用即时同步 则后面直接显示不用延时
        {
            Dis_Ctrl.dis_dly_time = 0x80;
        }
//        fill_dis_data();
//        Dis_Ctrl.flash_flag &= ~(PON_INT_SN_FLAG | LSN_CHG_SN_FLAG);
    }
    if(Dis_Ctrl.dis_dly_time & 0x80)
    {
        if(Dis_Ctrl.dis_dly_time == 0x80)
        {
            if(Dis_Ctrl.di_finish || (Dis_Ctrl.di_resend_cnt == 0)) //重抄效果不好 变成一直在发命令
            {
                fill_dis_data();
                Dis_Ctrl.dis_dly_time = 0;
                Dis_Ctrl.had_write = 1;
            }
            else
            {
                if(Dis_Ctrl.di_resend_cnt>0)
                {
                    Dis_Ctrl.dis_dly_time = 0x82;//bit7是标志 bit6~bit0是dly计数 即延时100ms 等数据同步后再去刷新显示
                    Dis_Ctrl.di_finish = 0;
                    if(__FALSE == dis_ctrl_sync())//如果不用即时同步 则后面直接显示不用延时
                    {
                        Dis_Ctrl.dis_dly_time = 0x80;
                    }
                    Dis_Ctrl.di_resend_cnt--;
                }
            }
            Dis_Ctrl.di_finish = 0;
        }
        else
        {
            Dis_Ctrl.dis_dly_time--;
        }
    }
#else			//配合无线采集器
    if(((cnt%10) == 0) ||
       (Dis_Ctrl.flash_flag & (PON_INT_SN_FLAG | LSN_CHG_SN_FLAG | MOD_CHG_SN_FLAG)))//1秒到或者有变化就重填显示数据
    {
        Dis_Ctrl.dis_dly_time = 0;//bit7是标志 bit6~bit0是dly计数 即延时100ms 等数据同步后再去刷新显示
        Dis_Ctrl.di_finish = 0;
        Dis_Ctrl.di_resend_cnt = 0;

        fill_dis_data();
        Dis_Ctrl.flash_flag &= ~(PON_INT_SN_FLAG | LSN_CHG_SN_FLAG);
    }    
#endif    
    Dis_Ctrl.flash_flag = 0;//清标志

}

/******************************************************************************
 Function name:  __task void lcd_display_task (void)
 Author       :
 Description  :
 Input        :  None
 Return       :  None
 *****************************************************************************/
__task void lcd_display_task (void)
{
    U8  flash_ct;
    U16 cnt;

    cnt = 0;
    flash_ct = 0;

    for(;;)
    {
        sign_task_schedule(LCD_DISPLAY_PRIO);

        lcd_mode_ctrl();//上电全显轮显或键显等模式控制字处理

        if(flash_ct >= 5)//闪烁标志在最前面刷新 放后面的话有可能会有停顿
        {
            rollback_flash_flag();
        }
        lcd_display_flash(cnt);
//      SET_PORT0_REV(P0_GRNU_LED);
        if(flash_ct >= 5)
        {
            if((Dis_Ctrl.had_write == 0) && (lcd_seg_buf[LCD_BUF_LENTH-1] != 0x5a))//这一轮100ms写LCD前面已写过 这里就不用再刷新了
            {
                fill_dis_flash_data();//0.5秒刷新闪烁标志
            }
            flash_ct = 0;
        }
        Dis_Ctrl.had_write = 0;

        os_dly_wait(OS_DLY_100MS);//OS_DLY_500MS

        flash_ct++;
        cnt++;
    }
}
/******************************************************************************
** 函数名称: void check_meter_serious_happened(void)
** 函数功能: 显示自检出错代码。出错故障一旦发生，显示器必须立即停留在该代码上
**           但按键显示可以改变当前代码,来显示其他选项。
**           出错代码至少包括下列故障： 时钟错误 存储器故障或损坏
** 入口参数:
**
**
** 返 回 值: __TRUE: 有严重错误发生
** 说    明: 每100ms调用
******************************************************************************/
U8 check_meter_serious_happened (void)
{
    //todo
    //......

//  return  __TRUE;//有严重错误发生
    return  __FALSE;//有严重错误发生
}
/******************************************************************************
** 函数名称: U8 get_meter_serious_err_id(void)
** 函数功能: 得到错误码代码号
** 入口参数:
**
**
** 返 回 值: 错误码代码号
** 说    明:
******************************************************************************/
U8 get_meter_serious_err_id (void)
{
    U8 id = 1;
    //todo
    //......

    return  id;//有严重错误发生
}
/******************************************************************************
** 函数名称: void fill_dis_err_page(U8 *errn, U8 *segbuf)
** 函数功能: 根据错误码号 填写错误屏显示缓冲
** 入口参数: 错误码代码号
**
**
** 返 回 值:
** 说    明:
******************************************************************************/
void fill_dis_err_page (U8 *errn, U8 *segbuf)
{
    fill_err_b8segbuf(errn, 2, segbuf);//errx
}

/******************************************************************************
** 函数名称: void fill_search_addr_segbuf(U8 *segbuf)
** 函数功能: 按搜索表结构中的显示序号 显示搜索到的表地址 高2字节显示在小8区 低4字节显示在大8区
**           把要显示的搜索到的表号的高2字节显示到小8区的低4位 小8区高4位填成S01-xxxx
** 入口参数:
**
**
** 返 回 值:
** 说    明:
******************************************************************************/
void fill_search_addr_segbuf (U8 *segbuf)
{
    U8 temp;

    if((Dis_Search_Ctrl.totnum == 0) || (Dis_Search_Ctrl.dis_sn >= Dis_Search_Ctrl.totnum))
    {
        memset(&Dis_Search_Ctrl.addr[0][0], 0xee, 6);
        Dis_Search_Ctrl.dis_sn = 0;
    }

    fill_data_b8segbuf(&Dis_Search_Ctrl.addr[Dis_Search_Ctrl.dis_sn][0], 8, segbuf);
    fill_data_l8segbuf(&Dis_Search_Ctrl.addr[Dis_Search_Ctrl.dis_sn][4], 4, segbuf);//小8区高4位填成S01-xxxx
    temp = HEX_TO_BCD((Dis_Search_Ctrl.dis_sn + 1), 1);
    fill_dis_ssn_l8_data(temp, segbuf);//Ssn-
}
/******************************************************************************
** 函数名称: void fill_search_set_state_segbuf(U8 *segbuf)
** 函数功能: 如果表地址有效 保存后就显示 SET-Good 否则显示 SET-Err
**
** 入口参数:
**
**
** 返 回 值:
** 说    明:
******************************************************************************/
void fill_search_set_state_segbuf (U8 *segbuf)
{
    if(Dis_Search_Ctrl.totnum == 0)
    {
        fill_special_b8segbuf(3, segbuf);//SET-Err
    }
    else
    {
        fill_special_b8segbuf(2, segbuf);//SET-Good
    }
    fill_dis_ssn_l8_data((Dis_Search_Ctrl.dis_sn + 1), segbuf);
}
/******************************************************************************
** 函数名称: void fill_gprs_signal_strenth_segbuf (U8 *segbuf)
** 函数功能: 填写GPRS信号强度：0~31 5等份即5格显示 如果是99或者其他值不显示信号符号
**
** 入口参数:
**
**
** 返 回 值:
** 说    明:
******************************************************************************/
void fill_gprs_signal_strenth_segbuf (U8 *segbuf)
{
    U8 i,value=0;

    if(stModuleRunState.ucSignIntension<10)
    {
        value = 1;
    }
    else if(stModuleRunState.ucSignIntension<16)
    {
        value = 2;
    }
    else if(stModuleRunState.ucSignIntension<19)
    {
        value = 3;
    }
    else if(stModuleRunState.ucSignIntension<23)
    {
        value = 4;
    }
    else if(stModuleRunState.ucSignIntension<32)
    {
        value = 5;
    }
    if(stModuleRunState.bIsGPRSConnected == 0)//不在线就清零
    {
       value = 1; 
    }    
    if(value)
    {
        for(i=0; i<5; i++)
        {
            segbuf[gprs_signal_dis_tab[value][i] / 8] |= 1 << (gprs_signal_dis_tab[value][i] % 8);
        }
    }
}
/******************************************************************************
** 函数名称: set_di_sync_finish (U8 flag)
** 函数功能: 置显示数据读取成功标志
**
** 入口参数: 0:是不需要直接去读的数据所以直接置完成标志 1:要临时去读的数据 则要比对一下读到的数据标识以确定完成情况
**
**
** 返 回 值:
** 说    明:
******************************************************************************/
void set_di_sync_finish(U8 flag)
{
    if(flag == 1)
    {
        if(0 == memcmp(gTmpArray+1, (U8*)&Dis_Ctrl.di, 4))
        {
            Dis_Ctrl.di_finish = 1;
        }
    }
    else
    {
       Dis_Ctrl.di_finish = 1;
    }
}


#if CODE_VER_TYPE==WLS_CUR_PRJ			//配合无线采集器
/******************************************************************************
** 函数名称: U8 get_dis_mp(void)
** 函数功能: 获取要显示的测量点号
** 入口参数: 
**
**
** 返 回 值:
** 说    明:
******************************************************************************/
U8 get_dis_mp(void)//测量点号
{
	U8 sn,mode;
	
	mode = Dis_Ctrl.cur_mode;
	if((mode!=DIS_MODE_AUTO) && (mode!=DIS_MODE_SKEY))
	{
		mode = DIS_MODE_AUTO;
	}	
	
	sn = Dis_Ctrl.dis_bsn[mode];//获取当前显示大屏序号

	if((sn >= 2) && (sn <= 6))
	{
		return 1;//测量点1
	}
	else if(sn > 6)
	{
		return 2;//测量点2
	}
	else
	{	
		return 0;//测量点0
	}
}

/******************************************************************************
** 函数名称: void get_dis_temp_data(U32 di, U8 lsn)
** 函数功能: 在显示之前把要显示的数据保存到一个全局缓冲
**
** 入口参数: 
**
**
** 返 回 值:
** 说    明:
******************************************************************************/
void get_dis_temp_data(U32 di, U8 lsn)
{
	U8 mp,m;
	STHISVAL stHisVal;
		
	mp = get_dis_mp();
	
	memcpy(gTmpArray+1, (U8*)&di, 4);
	gTmpArray[0] = 4;//数据长度(包含数据标识长度)
	 
	switch(di)
	{
		case 0x04000401:  //电能表通信地址（表号）低8位: 
			if((mp == 1) && (lsn == 0))//无线采集器地址
			{
				rw_mp_para(1, 0x8902, gTmpArray+5, &m, 'r');
				gTmpArray[0] += 4;
			}
			else//监听总表 后面其他地方默认处理
			{
				rw_mp_para(2, 0x8902, gTmpArray+5, &m, 'r');
				gTmpArray[0] += 6;
			}	
			break;	
		case 0x02020100:	//A相电流 
			if(mp == 1)//无线采集器
			{
				memcpy(gTmpArray+5, &Current_Detector_Data[0].Current[0], 3);
				gTmpArray[0] += 3;			
			}
			else//监听总表 后面其他地方默认处理
			{
				stHisVal = Get_History_I_byMPNo(2,1,(unsigned char)(0xb621&0x0f));
				stHisVal.lVal = stHisVal.lVal*10;//xx.xx -> xxx.xxx
				Fill_HisUnsignVal(stHisVal,gTmpArray+5,3);//2);
				gTmpArray[0] += 3;
			}	
			break;	
		case 0x02020200:	//B相电流: 
			if(mp == 1)//无线采集器
			{
				memcpy(gTmpArray+5, &Current_Detector_Data[1].Current[0], 3);
				gTmpArray[0] += 3;						
			}
			else//监听总表 后面其他地方默认处理
			{
				stHisVal = Get_History_I_byMPNo(2,1,(unsigned char)(0xb622&0x0f));//1显示同一次抄表的数据
				stHisVal.lVal = stHisVal.lVal*10;//xx.xx -> xxx.xxx
				Fill_HisUnsignVal(stHisVal,gTmpArray+5,3);//2);
				gTmpArray[0] += 3;
			}	
			break;		
		case 0x02020300:	//C相电流: 
			if(mp == 1)//无线采集器
			{
				memcpy(gTmpArray+5, &Current_Detector_Data[2].Current[0], 3);
				gTmpArray[0] += 3;						
			}
			else//监听总表 后面其他地方默认处理
			{
				stHisVal = Get_History_I_byMPNo(2,1,(unsigned char)(0xb623&0x0f));
				stHisVal.lVal = stHisVal.lVal*10;//xx.xx -> xxx.xxx
				Fill_HisUnsignVal(stHisVal,gTmpArray+5,3);//2);
				gTmpArray[0] += 3;
			}	
			break;	
		case 0x02050000:	//瞬时总视在功率: 
			if(mp == 1)//无线采集器
			{
				memcpy(gTmpArray+5, (U8*)&Real_Power, 3);
				gTmpArray[0] += 3;					
			}
			else//监听总表 后面其他地方默认处理
			{
				
			}	
			break;	
	
		case 0x00010000:	//当前正向有功总电量: 
			stHisVal = Get_History_DirPower_byMPNo(2,1,(unsigned char)(0x9010&0x0f));
			Fill_HisUnsignVal(stHisVal,gTmpArray+5,4);
			gTmpArray[0] += 4;
			break;	
	
		case 0x00020000:	//当前反向有功总电量: 
			stHisVal = Get_History_UnDirPower_byMPNo(2,1,(unsigned char)(0x9020&0x0f));
			Fill_HisUnsignVal(stHisVal,gTmpArray+5,4);
			gTmpArray[0] += 4;
			break;	
		case 0x00030000:	//当前组合无功1总电量//正向无功总 反向无功总: 
			stHisVal = Get_History_DirVar_byMPNo(2,1,(unsigned char)(0x9110&0x0f));
			Fill_HisUnsignVal(stHisVal,gTmpArray+5,4);
			gTmpArray[0] += 4;
			break;	
		case 0x00040000:	//当前组合无功2总电量: 
			stHisVal = Get_History_UnDirVar_byMPNo(2,1,(unsigned char)(0x9120&0x0f));
			Fill_HisUnsignVal(stHisVal,gTmpArray+5,4);
			gTmpArray[0] += 4;
			break;	
		case 0x02010100:	//A相电压: 
			stHisVal = Get_History_Vol_byMPNo(2,1,(unsigned char)(0xb611&0x0f));
			stHisVal.lVal = stHisVal.lVal*10;//xxx -> xxx.x(97保存07格式显示)
			Fill_HisUnsignVal(stHisVal,gTmpArray+5,2);
			gTmpArray[0] += 2;
			break;	
		case 0x02010200:	//B相电压: 
			stHisVal = Get_History_Vol_byMPNo(2,1,(unsigned char)(0xb612&0x0f));
			stHisVal.lVal = stHisVal.lVal*10;//xxx -> xxx.x
			Fill_HisUnsignVal(stHisVal,gTmpArray+5,2);
			gTmpArray[0] += 2;
			break;	
		case 0x02010300:	//C相电压: 
			stHisVal = Get_History_Vol_byMPNo(2,1,(unsigned char)(0xb613&0x0f));
			stHisVal.lVal = stHisVal.lVal*10;//xxx -> xxx.x
			Fill_HisUnsignVal(stHisVal,gTmpArray+5,2);
			gTmpArray[0] += 2;
			break;	
		case 0x02030000:	//瞬时总有功功率: 
			stHisVal = Get_History_InstantPow_byMPNo(2,1,(unsigned char)(0xb630&0x0f));
			Fill_HisUnsignVal(stHisVal,gTmpArray+5,3);//xx.xxxx
			gTmpArray[0] += 3;
			break;	
		case 0x02040000:	//瞬时总无功功率: 
			stHisVal = Get_History_InstantVar_byMPNo(2,1,(unsigned char)(0xb640&0x0f));
			stHisVal.lVal = stHisVal.lVal*100;//xx.xx -> xx.xxxx
			Fill_HisUnsignVal(stHisVal,gTmpArray+5,3);//2);
			gTmpArray[0] += 3;
			break;	
		case 0x02060000:	//瞬时总功率因数: 
			break;	
	}

}
#endif//WLS_CUR_PRJ

#endif//LCD_VALID
/******************************************************************************
                            End Of File
*****************************************************************************/

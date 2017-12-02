/******************************************************************************
 Copyright (C) 2011  R&D Institute of Reallin Co., Ltd.
 Module         : ad on chip samplefunction
 File Name      : ad_ain.h
 Description    : LPC17系列芯片硬件片上AD信号采样
 Author         :
 Build Date     :
 others         :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、     1.00       2011-08-05     rosoon     the original version
******************************************************************************/

#ifndef __AD_AIN_H
#define __AD_AIN_H

//----------------------------- macro definition ------------------------------
//AD0CR寄存器相关控制功能位设定值宏定义
#define AD_CH_SEL(n)                n << 0  //bit[7:0]对应位写1为使能通道
#define AD_CLK_DIV                  19 << 8 //bit[15:8]的值加1为分频系数
#define AD_BURST(b)                 b << 16 //bit[16]默认值0
#define AD_PDN(v)                   v << 21 //bit[21]:1正常,0掉电
#define AD_START(s)                 s << 24 //bit[26:24]:1立即启动,0不启动
#define AD_EDGE                     0 << 27 //bit[27]当AD_START取2~7时有效

//AD0INTEN寄存器相关控制功能位设定值宏定义
#define AD_GINTEN                   0 << 8

//AD0GDR寄存器相关控制功能位设定值宏定义
#define AD_RESULT                   4
#define AD_CHN                      24
#define AD_OVERRUN                  30
#define AD_DONE                     31
#define AD_MAX                      4

//用户应用其它宏定义:低功耗部分
#define ONE_MINUTE                  60                  //1分钟(60秒)
#define PD_AD_SAMP_GAP              10 * ONE_MINUTE     //低功耗工况AD电池采样间隔

//用户应用其它宏定义:GPRS电池充电管理部分
#define SCAN_TIME                   15                  //定时扫描时间间隔15min
#define FCG_TIME                    (750 + 50)          //快充理论计时+停充时间,单位:分钟(50是追加的停充时间大小等于FCG_CNT)
//FCG_TIME:[(Cbat/Icharge)/Echarge = (600mAh/60mA)/0.8 = 12.5h = 750 min],这是理论时间,还需追加停充时间
#define FCG_CNT                     (FCG_TIME / SCAN_TIME)  //快充计数器(也等于停充分钟数,需要将停充时间追加到FCG_TIME上)

#define SCG_TIME                    30                  //由慢充切为快充的锁定总计时,单位:分钟
//SCG_TIME:[(Cbat/Icharge)/Echarge = (600mAh/60mA)/0.8 = 12.5h = 750 min]
#define SMP_POWER_CNT               10                  //均值采样(2的)幂级数即采(2^SMP_POWER_CNT)次的平均值

#define BAT_ERR_VOLT                10                  //稳定采样误差容限:8毫伏[2^12*(0.008/3.3)]
#define NO_BAT_LOW_VOLT             2730                //0xAAA:2.2V[2^12*(2.2/3.3)](对应实际值4.4V)
#define NO_BAT_HIG_VOLT             3970                //0xF82:3.2V[2^12*(3.2/3.3)](对应实际值6.4V)
#define ON_BAT_MIN_VOLT             2854                //0xB26:2.3V[2^12*(2.3/3.3)](对应实际值4.6V)
#define ON_BAT_MAX_VOLT             3850                //0xECE:3.1V[2^12*(3.1/3.3)](对应实际值6.2V)
#define ON_BAT_FCL_VOLT             3230                //0xC9E:2.6V[2^12*(2.6/3.3)](对应实际值5.2V),快充下线电压
#define ON_BAT_FCH_VOLT             3480                //0xE10:2.8V[2^12*(2.8/3.3)](对应实际值5.6V),快充上线电压

//GPRS模块类型判别宏定义
#define EVEO_LIMIT_VOLT             124                 //事件输出电压范围阈值:100mV[2^12*(0.1/3.3)]
#define EVEO_ONE_SIXTH              682                 //满量程电压(3.3V)的六分之一(默认值)
#define EVEO_ONE_FIFTH              819                 //满量程电压(3.3V)的五分之一
#define EVEO_ONE_QUARTER            1024                //满量程电压(3.3V)的四分之一
#define EVEO_ONE_THIRD              1365                //满量程电压(3.3V)的三分之一
#define EVEO_ONE_HALF               2048                //满量程电压(3.3V)的二分之一

//充电管理函数battery_charge_mange(void)相关宏定义
#define REF_V0_1        124     //0x07C:0.1V[2^12*(0.1/3.3)](对应实际值0.2V)
#define REF_V0_5        620     //0x26C:0.5V[2^12*(0.5/3.3)](对应实际值1.0V)
#define REF_V1_0        1241    //0x4D9:1.0V[2^12*(1.0/3.3)](对应实际值2.0V)
#define REF_V1_2        1490    //0x5D2:1.2V[2^12*(1.2/3.3)](对应实际值2.4V)
#define REF_V1_4        1738    //0x6CA:1.4V[2^12*(1.4/3.3)](对应实际值2.8V)
#define REF_V1_5        1862    //0x746:1.5V[2^12*(1.5/3.3)](对应实际值3.0V)
#define REF_V2_4        2979    //0xBA3:2.4V[2^12*(2.4/3.3)](对应实际值4.8V)
#define REF_V2_5        3103    //0xC1F:2.5V[2^12*(2.5/3.3)](对应实际值5.0V)
#define REF_V2_6        3227    //0xC9B:2.6V[2^12*(2.6/3.3)](对应实际值5.2V)
#define REF_V2_8        3475    //0xD93:2.8V[2^12*(2.8/3.3)](对应实际值5.6V)
#define REF_V2_9        3600    //0xE10:2.9V[2^12*(2.9/3.3)](对应实际值5.8V)
#define REF_V3_0        3724    //0xE8C:3.0V[2^12*(3.0/3.3)](对应实际值6.0V)
#define FCG_GATE_HIGH   REF_V2_9//未饱和态：禁充工况扫描Vbat < 2.9V时启动快充，>= 2.9V时切换为饱和态
#define FCG_GATE_LOW    REF_V2_6//饱和态：  禁充工况扫描Vbat < 2.6V时切换为未饱和态
#define SCG_GATE_MIN    REF_V2_6//饱和态：  禁充工况扫描Vbat > 2.6V时启动慢充
#define SCG_NO_BAT      REF_V2_8//充电(慢充时)工况无电池时采样门限,Vbat > 5.6V则判为无电池
#define FCG_NO_BAT      REF_V3_0//充电(快充  )工况无电池时采样门限,Vbat > 3.0V则判为无电池
#define CRG_STA_FLG     1 << 1  //RTCGPREG0的bit1位,为1表示电池未充饱和,用于标识饱和与未饱和状态切换

#define BAT_24V_MIN     REF_V0_5//升压方案:判断2.4V型电池时BAT网络实际下限电压1.0V
#define BAT_24V_MAX     REF_V1_5//升压方案:判断2.4V型电池时BAT网络实际上限电压3.0V
#define BAT_48V_MIN     REF_V1_5//升压方案:判断4.8V型电池时BAT网络实际下限电压3.0V
#define BAT_48V_MAX     REF_V3_0//升压方案:判断4.8V型电池时BAT网络实际上限电压6.0V
#define BAT_24V_LOW     REF_V1_2//升压方案:判断2.4V型电池欠压门限 V < 2.4V为欠压
#define BAT_48V_LOW     REF_V2_4//升压方案:判断4.8V型电池欠压门限 V < 4.8V为欠压

#define RATED_VOLT_FLG_BIT  16  //GPRS电池额定电压标识在Bat_Stat_Flag中的起始位bit19:16
#define RATED_VOLT_FLG_24V  1   //1代表2.4V电池
#define RATED_VOLT_FLG_48V  0   //0代表4.8V电池其它无效
//----------------------------- type definition -------------------------------
typedef enum
{
    AD_BAT_GPRS = (1 << 0),     //AD0[0],GPRS电池
    AD_BAT_RTC = (1 << 1),      //AD0[1],RTC电池
    AD_BAT_PDB = (1 << 2),      //AD0[2],停显电池
    AD_EVE_TYP = (1 << 3),      //AD0[3],GPRS模块EVENTOUT跳线输出
}AD_AIN_CHL;

typedef enum
{
    BAT_GPRS,  //AD0[0],GPRS电池(标称值4.8V - 管压降)取1/2
    BAT_RTC,   //AD0[1],RTC电池(标称值3.6V - 管压降)取1/2
    BAT_PDB,   //AD0[2],停显电池(标称值 2 * 3V )取1/2
    EVE_TYP,   //AD0[3],EVENTOUT跳线输出:3.3V/N,N取1~6,代表不同型号GPRS模块
}SMP_TYPE;

typedef enum
{
    BAT_NORMAL,     //电池电压正常
    BAT_LOW,        //电池欠压
    BAT_NO,         //无电池
    BAT_UNKNOWN = 0x0F,   //电池状态未知
}BAT_STAT;

typedef enum
{
    GPRS_MC52I,     //西门子52I
    GPRS_MC53I,     //西门子53I
    GPRS_MC54I,     //西门子54I
    GPRS_MC55I,     //西门子55I
    GPRS_MC56I,     //西门子56I
    GPRS_UNKNOWN = 0x0F,   //未知模块
}GPRS_TYPE;
//GPRS电池充电状态结构体
typedef enum
{
    DIS_CRG,        //禁充 0
    SLW_CRG,        //慢充 1
    FST_CRG,        //快充 2
    CRG_STA_FLG_BIT = 20,   //充电状态标识在Bat_Stat_Flag中的起始位bit23:20
}BAT_CRG_STAT;

//-------------------------- functions declaration ----------------------------
__EXTERN void ad_ain_init(void);
__EXTERN void pd_bat_mange(void);
__EXTERN void bat_charge_mange(void);//原用于V1.2主板的初始版本，现已不再用
__EXTERN void batt_charge_mange(void);//仅用于V1.2主板,针对4.8V电池充电管理
__EXTERN U8 get_batt_state(U8 bat_num);//仅用于V1.2主板,针对4.8V电池状态处理
__EXTERN void battery_charge_mange(void);//用于V1.3及以上主板,兼容2.4V与4.8V电池充电管理
__EXTERN U8 get_battery_state(U8 bat_num);//用于V1.3及以上主板,兼容2.4V与4.8V电池状态处理
//__EXTERN

//--------------------------- variable declaration ----------------------------
__EXTERN U16 Ad_Data[AD_MAX];   //AD转换结果寄存器
__EXTERN U32 Bat_Stat_Flag;     //电池状态标识
//bit[3:0]  :GPRS电池状态 BAT_NORMAL:正常  BAT_LOW:欠压   BAT_NO:没加电池 BAT_UNKNOWN:未知状态
//bit[7:4]  :RTC 电池状态 BAT_NORMAL:正常  BAT_LOW:欠压   BAT_NO:没加电池 BAT_UNKNOWN:未知状态
//bit[11:8] :停显电池状态 BAT_NORMAL:正常  BAT_LOW:欠压   BAT_NO:没加电池 BAT_UNKNOWN:未知状态
//bit[15:12]:GPRS模块类型 0:MC52I 1:待补充 2:待补充 3:待补充 4:待补充 0xFF:未知状态
//bit[19:16]:GPRS电池额定电压类型(为了兼容2.4V与4.8V两款电池) 1 : 2.4V电池;  0 : 4.8V电池;其它无效
//bit[23:20]:充电状态 0 : 禁充; 1 : 慢充; 2 : 快充; 其它无效
//bit[31:24]:保留
//__EXTERN
//__EXTERN

#endif//__AD_AIN_H

/******************************************************************************
    End Of File
******************************************************************************/

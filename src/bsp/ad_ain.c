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
#include    "..\inc\global_config.h"

//-------------------------------- 常数定义 -----------------------------------

//------------------------------ 私有变量声明 ---------------------------------
U8 smp_ok_flg;          //稳定采样完成标识bit[2:0]分别对应AD[2:0]
U16 smp_cnt[AD_MAX];    //采样计数器
U32 smp_sum[AD_MAX];    //存放累加值
U32 bat_stat_flg = 0xFFFFFFFF;   //电池状态标识
//bit[3:0]  :GPRS电池状态 BAT_NORMAL:正常  BAT_LOW:欠压   BAT_NO:没加电池 BAT_UNKNOWN:未知状态
//bit[7:4]  :RTC 电池状态 BAT_NORMAL:正常  BAT_LOW:欠压   BAT_NO:没加电池 BAT_UNKNOWN:未知状态
//bit[11:8] :停显电池状态 BAT_NORMAL:正常  BAT_LOW:欠压   BAT_NO:没加电池 BAT_UNKNOWN:未知状态
//bit[15:12]:GPRS模块类型 0:MC52I 1:待补充 2:待补充 3:待补充 4:待补充 0xFF:未知状态
//bit[31:16]:保留
//------------------------------ 私有函数声明 ---------------------------------
static U8 get_charge_state(void);
static U8 get_gprs_type(U16 eveo_volt);
//------------------------------ 函数原型定义 ---------------------------------
/******************************************************************************
** 函数名称: void ad_ain_init(void)
** 函数功能: 远红外通信口初始化功能函数
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void ad_ain_init (void)
{

//IO口初始化:注意在main.c里开关相关中断处理
    PCONP |= (1 << PCAD);                       //使能ADC外设时钟
    PCLKSEL0 |= (FPCLK_AD_DIV << 24);           //设置外设AD分频系数1/20

#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
    PINMODE_OD1 &= ~(P0_BAT_AD);  //AD0[3:0]通道引脚(p0.26:23)非开漏

    if((PINSEL1 & (0x03u << 14))!= (P0_23_FNUC << 14))   //AD功能引脚配置
    {
        PINSEL1 &= ~(0x03u << 14);//先清除相关bits
        PINSEL1 |= (P0_23_FNUC << 14);//配置AD模式引脚
        PINMODE1 &= ~(0x03u << 14);//先清除相关bits
        PINMODE1 = (P0_23_FNUC << 14);//PIN_NO_PULL
    }
#else//H_METER_PRJ,WLS_CUR_PRJ
    PINMODE_OD1 &= ~(P0_BAT_AD | P0_BAT1_AD | P0_BAT2_AD | P0_EVENTOUT);  //AD0[3:0]通道引脚(p0.26:23)非开漏

    if((PINSEL1 & (0x03u << 14))!= (P0_23_FNUC << 14)
      || (PINSEL1 & (0x03u << 16))!= (P0_24_FNUC << 16)
      || (PINSEL1 & (0x03u << 18))!= (P0_25_FNUC << 18)
      || (PINSEL1 & (0x03u << 20))!= (P0_26_FNUC << 20))   //AD功能引脚配置
    {
        PINSEL1 &= ~((0x03u << 14) | (0x03u << 16) | (0x03u << 18) | (0x03u << 20));//先清除相关bits
        PINSEL1 |= ((P0_23_FNUC << 14) | (P0_24_FNUC << 16) | (P0_25_FNUC << 18) | (P0_26_FNUC << 20));//配置AD模式引脚
        PINMODE1 &= ~((0x03u << 14) | (0x03u << 16) | (0x03u << 18) | (0x03u << 20));//先清除相关bits
        PINMODE1 = ((P0_23_FNUC << 14) | (P0_24_FNUC << 16) | (P0_25_FNUC << 18) | (P0_26_FNUC << 20));//PIN_NO_PULL
    }
#endif    
    AD0CR = (AD_CH_SEL(0xF) | AD_CLK_DIV | AD_BURST(1) | AD_PDN(1) | AD_START(0));//配置A/D控制寄存器

    smp_sum[BAT_GPRS] = smp_sum[BAT_RTC] = smp_sum[BAT_PDB] = smp_sum[EVE_TYP] = 0;
    smp_ok_flg = 0;
    Bat_Stat_Flag = 0xFFFFFFFF;         //初始值

    ADINTEN = (1 << AD_GINTEN);         //使能ADDR的全局DONE标志产生中断
}

/******************************************************************************
** 函数名称: S8 get_ad_ain_data(U8 *buf)
** 函数功能: 远红外通信口收多字节数据功能函数
** 入口参数: buf 收到数据缓存指针
** 返 回 值: 1 完整数据帧
**          -1 无有效数据
** 说    明: 调用该函数后,若数据有效,则buf[0]为数据帧长度,buf[n:1]有效数据帧
******************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		
S8 get_ad_ain_data (U8 *buf)
{
    U8 cnt;

    cnt = Ir_Ctl[IR_RX].Dat_Buf[IR_LS];                 //读取令牌信息

    Ir_Ctl[IR_RX].Dat_Tok += 1;                         //令牌累加
    if(Ir_Ctl[IR_RX].Dat_Tok == cnt)                    //令牌不同步
    {
        cnt = Ir_Ctl[IR_RX].Dat_Buf[IR_FI];             //读取帧长信息
        if((cnt > IR_FI) && (cnt < IR_LS))              //数据帧完好
        {
            memcpy(buf, Ir_Ctl[IR_RX].Dat_Buf, cnt + 1);//包含帧长度信息
            Ir_Ctl[IR_RX].Dat_Buf[IR_FI] =  0;          //帧长度信息清零
            return 1;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        Ir_Ctl[IR_RX].Dat_Tok = cnt;                     //令牌同步,为下次准备
        return -1;
    }
}
#endif
/******************************************************************************
** 函数名称: void pd_bat_mange(void)
** 函数功能: 低功耗工况电池AD采样功能函数
** 入口参数: 无
** 返 回 值: 无
** 说    明: 只在掉电低功耗模式下调用,
             兼容2.4V与4.8V两类电池
******************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
void pd_bat_mange (void)
{
    U8 i;
    U16 gate_volt;
static U16 ad_result;

//--------------------------------AD初始化及电池采样--------------------------
    if(RTCGPREG3 < PD_AD_SAMP_GAP)
    {
        return;                                 //采样间隔时间不足，直接退出
    }
    RTCGPREG3 = 0;                              //计数器清零
    PCONP |= (1 << PCAD);                       //使能ADC外设时钟
    PCLKSEL0 |= (FPCLK_AD_DIV << 24);           //设置外设AD分频系数1/20

    PINMODE_OD1 &= ~(P0_BAT_AD | P0_BAT1_AD | P0_BAT2_AD | P0_EVENTOUT);  //AD0[3:0]通道引脚(p0.26:23)非开漏

    if((PINSEL1 & (0x03u << 14))!= (P0_23_FNUC << 14)
      || (PINSEL1 & (0x03u << 16))!= (P0_24_FNUC << 16)
      || (PINSEL1 & (0x03u << 18))!= (P0_25_FNUC << 18)
      || (PINSEL1 & (0x03u << 20))!= (P0_26_FNUC << 20))    //AD功能引脚配置
    {
        PINSEL1 &= ~((0x03u << 14) | (0x03u << 16) | (0x03u << 18) | (0x03u << 20));//先清除相关bits
        PINSEL1 |= ((P0_23_FNUC << 14) | (P0_24_FNUC << 16) | (P0_25_FNUC << 18) | (P0_26_FNUC << 20));//配置AD模式引脚
        PINMODE1 &= ~((0x03u << 14) | (0x03u << 16) | (0x03u << 18) | (0x03u << 20));//先清除相关bits
        PINMODE1 = ((P0_23_FNUC << 14) | (P0_24_FNUC << 16) | (P0_25_FNUC << 18) | (P0_26_FNUC << 20));//PIN_NO_PULL
    }
    AD0CR = (AD_CH_SEL(AD_BAT_GPRS) | AD_CLK_DIV | AD_BURST(0) | AD_PDN(1) | AD_START(1));//配置A/D控制寄存器[启动采样GPRS电池]
    while((ADSTAT & (AD_BAT_GPRS)) == 0);   //读取AD0STAT的通道0的Done(抛弃第一次采样数据)
    ad_result = 0;                          //清零
    for(i = 0; i < 4; i++)                  //启停模式采样4次
    {
        AD0CR |= AD_START(1);               //[启动采样GPRS电池]
        while((ADSTAT & (AD_BAT_GPRS)) == 0);    //读取AD0STAT的通道0的Done
        ad_result += (ADDR1 >> 4) & 0xfff;  //累加
    }
    ad_result >>= 2;                        //求采样4次平均值
//--------------------------------电池供电管理--------------------------
    Ad_Data[BAT_GPRS] = ad_result;  //azh
    if((ad_result >= BAT_48V_MIN) && (ad_result < BAT_48V_MAX))//若是4.8V电池
    {
        gate_volt = ON_BAT_MIN_VOLT;
    }
    else if((ad_result >= BAT_24V_MIN) && (ad_result < BAT_24V_MAX))//若是2.4V电池
    {
        gate_volt = (ON_BAT_MIN_VOLT >> 1);
    }
    else        //无电池
    {
        gate_volt = (ON_BAT_MIN_VOLT >> 1);
    }
    if(ad_result <= gate_volt)         //防止GPRS可充电池过放损坏控制
    {
        BAT_OFF();  //关闭GPRS电池，切换为停显电池供电
    }
    else//110309 azh
    {
        BAT_ON();  //关闭GPRS电池，切换为停显电池供电
        PIN_POWERON_CLR;//在掉电的情况关闭GPRS模块电源
    }
    PCONP &= ~(1 << PCAD);                  //禁能ADC外设时钟
}
#endif
/******************************************************************************
** 函数名称: void ADC_IRQHandler(void)
** 函数功能: AD中断服务子程序
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void ADC_IRQHandler (void)
{
    U8 chn;                                             //ad通道号
    U16 result;                                         //转换结果
    U32 ad_gdr;

    ad_gdr = AD0GDR;                                    //读取寄存器以清除OVERRUN、DOEN位
    if(ad_gdr & (1u << AD_OVERRUN))
    {
        ADSTAT;                                         //状态清零
    }
    else if(ad_gdr & (1u << AD_DONE))                   //AD通道转换完毕中断
    {
        chn = (ad_gdr >> AD_CHN) & 0x07;
        result = (ad_gdr >> AD_RESULT) & 0x0FFF;
        smp_sum[chn] += result;
        smp_cnt[chn]++;
        ADSTAT;                                         //状态清零
        if(smp_cnt[chn] >= power2(SMP_POWER_CNT))
        {
            smp_cnt[chn] = 0;
            Ad_Data[chn] = (smp_sum[chn] >> SMP_POWER_CNT);
            smp_sum[chn] = 0;
            if((max(Ad_Data[chn], result) - min(Ad_Data[chn], result)) < BAT_ERR_VOLT)
            {
                smp_ok_flg |= (1 << chn);
            }
        }
    }
}

/******************************************************************************
** 函数名称: void bat_charge_mange(void)
** 函数功能: GPRS电池充电管理功能函数
** 入口参数: 无
** 返 回 值: 无
** 说    明: 在非掉电工况下监管GPRS电池的有无、快慢充、过充、过放处理
             初始版本,无死区,对于特殊情况电池管理失控，且易频繁切换状态
******************************************************************************/
void bat_charge_mange (void)
{
    U16 vbat;
static U16 fcg_timer,scg_timer;             //快慢充分钟定时器

    if(!PORT0_IS_HIGH(P0_PD_CHK))           //掉电工况,不予处理
    {
        CHARGE_DIS();                       //充电禁能
        FAST_CHARGE_OFF();                  //快充关闭
        return;
    }

    if(!(smp_ok_flg & (1 << BAT_GPRS)))     //AD采样数据无效
    {
        return;
    }
    smp_ok_flg &= ~(1 << BAT_GPRS);

    vbat = Ad_Data[BAT_GPRS];
    switch(get_charge_state())
    {
    case 1:                                 //当前处于慢充状态
        if((vbat < ON_BAT_MIN_VOLT) || (vbat > (ON_BAT_MAX_VOLT - 10 * BAT_ERR_VOLT)))    //过放损坏或无电池
        {
            CHARGE_DIS();                   //禁充
            FAST_CHARGE_OFF();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
            WGP_OFF();//DBG
#endif
        }
        else if((vbat <= ON_BAT_FCL_VOLT) || (vbat <= ON_BAT_FCH_VOLT) && (((U16)(Min_Cnt + 1 - scg_timer + 0xFFFF)) >= SCG_TIME))
        {//快充启动必要条件,虚加1分钟,避免由慢充切到快充时误判
            CHARGE_EN();                    //快充
            FAST_CHARGE_ON();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
//            WGP_ON();//DBG
#endif
            fcg_timer = Min_Cnt;            //获取由慢充进入快充时当前分钟时标
        }
        break;

    case 2:                                 //当前处于快充状态
        if((vbat < ON_BAT_MIN_VOLT) || (vbat > NO_BAT_HIG_VOLT))    //过放损坏或无电池
        {
            CHARGE_DIS();                   //禁充
            FAST_CHARGE_OFF();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
            WGP_OFF();//DBG
#endif
        }
        else if((vbat >= ON_BAT_MAX_VOLT) || (((U16)(Min_Cnt + 1 - fcg_timer + 0xFFFF)) >= FCG_TIME))
        {//充满或定时满切为慢充,虚加1分钟,避免由快充切到慢充时误判
            CHARGE_EN();                    //慢充
            FAST_CHARGE_OFF();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
            WGP_OFF();//DBG
#endif
            scg_timer = Min_Cnt;            //获取由快充进入慢充时当前分钟时标,以便锁定时间窗口,防止快慢充震荡
        }
        break;

    case 3:             //当前处于充电禁能且快充关闭状态
        if((vbat >= ON_BAT_MIN_VOLT) && (vbat <= ON_BAT_MAX_VOLT))
        {
            CHARGE_EN();                    //慢充
            FAST_CHARGE_OFF();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
            WGP_OFF();//DBG
#endif
            scg_timer = (U16)(Min_Cnt - SCG_TIME + 0xFFFF);  //为由禁充->慢充->快充提速,即解除时间窗口锁定
        }
        break;

    default:
        break;
    }
}

/******************************************************************************
** 函数名称: U8 get_charge_state(void)
** 函数功能: 当前充电状态功能函数
** 入口参数: 无
** 返 回 值: 1:慢充; 2:快充; 3:充电禁能
** 说    明:
******************************************************************************/
U8 get_charge_state (void)
{
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
    if(PORT2_IS_HIGH(P2_CHAG_ON))    //充电使能
    {
        if(PORT2_IS_HIGH(P2_FAST_OFF))
#else        
    if(PORT3_IS_HIGH(P3_CHAG_ON))    //充电使能
    {
        if(PORT3_IS_HIGH(P3_FAST_OFF))
#endif
        {
            return 1;                //慢充状态
        }
        else
        {
            return 2;                //快充状态
        }
    }
    else                             //充电禁能
    {
        return 3;
    }
}

/******************************************************************************
** 函数名称: void batt_charge_mange(void)
** 函数功能: GPRS电池充电管理功能函数
** 入口参数: 无
** 返 回 值: 无
** 说    明: 在非掉电工况下监管GPRS电池的有无、快慢充、过充、过放处理
             仅针对4.8V电池充电管理(添加死区机制,避免频繁切换充电状态)
******************************************************************************/
void batt_charge_mange (void)
{
    U16 tmp;
    U16 vbat;
static U8 fcg_cnt = 0;          //快充计数器
static U8 save_time = 0;        //当检测到有电池时为迅速进入充电管理需要节约的时间:分钟数
static U16 bat_on_flg = 1;      //GPRS电池有无标识
//bit0: 为1 有电池(默认)，为0 无电池
//bit1:7保留
//bit15:8  临时计数器，辅助判断在线电池的电压检测
static U16 scan_timer = 0xFFFF; //防止同一个时标被多次使用，应该仅使用一次即可

    if(!PORT0_IS_HIGH(P0_PD_CHK))           //掉电工况,不予处理
    {
        CHARGE_DIS();                       //充电禁能
        FAST_CHARGE_OFF();                  //快充关闭
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ	
        WGP_OFF();//DBG
        DBG_LED_OFF();//DBG
#endif
        return;
    }

    if(!(smp_ok_flg & (1 << BAT_GPRS)))     //AD采样数据无效
    {
        return;
    }
    else                                    //采样有效，则进行有无电池动态判断
    {
        smp_ok_flg &= ~(1 << BAT_GPRS);     //标识复位

        vbat = Ad_Data[BAT_GPRS];
        if(vbat > FCG_NO_BAT)               //无电池时，切为慢充以辅助判别电池有无
        {
            CHARGE_EN();                    //充电使能
            FAST_CHARGE_OFF();              //快充关闭
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ	
            WGP_OFF();//DBG
            DBG_LED_OFF();//DBG
            ALARM_LED_ON();//DBG:无电池报警
#endif
            bat_on_flg &= ~(1 << 0);        //无电池标识
            bat_stat_flg &= ~(0xFF << (BAT_GPRS * 4));  //相关位先清零
            bat_stat_flg |= (BAT_NO << (BAT_GPRS * 4)); //设置GPRS电池状态字以备其它函数查询
            return;
        }
        else if(0 == (bat_on_flg & 1))      //有电池时仅进来一次即可
        {
            bat_on_flg |= (1 << 0);         //已有电池
            save_time = Min_Cnt % SCAN_TIME;//计算应节约的时间
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
            ALARM_LED_OFF();//DBG:无电池报警解除
#endif
        }
#if CODE_VER_TYPE==GPRS_MODULE_PRJ
        if((bat_on_flg & 1) && (PORT2_IS_HIGH(P2_CHAG_ON)))//有电池情况下,快速检测电池电压以备驱动LCD显示电池状态
#else        
        if((bat_on_flg & 1) && (PORT3_IS_HIGH(P3_CHAG_ON)))//有电池情况下,快速检测电池电压以备驱动LCD显示电池状态
#endif
        {
            if((bat_on_flg >> 8) > 2)           //在有电池后延时获取AD采样数据比较稳定可靠
            {
                bat_on_flg &= ~(0xFF << 8);     //计数器清零
                vbat = Ad_Data[BAT_GPRS];
                if(vbat > REF_V2_4)             //GPRS电池电压 > 4.8V 为正常
                {
                    bat_stat_flg &= ~(0x0F << (BAT_GPRS * 4));      //相关位先清零
                    bat_stat_flg |= (BAT_NORMAL << (BAT_GPRS * 4)); //设置GPRS电池状态字以备其它函数查询
                }
                else if(vbat <= REF_V2_4)       //GPRS电池电压 <= 4.8V 为欠压
                {
                    bat_stat_flg &= ~(0x0F << (BAT_GPRS * 4));      //相关位先清零
                    bat_stat_flg |= (BAT_LOW << (BAT_GPRS * 4));    //设置GPRS电池状态字以备其它函数查询
                }
            }
            bat_on_flg += (1 << 8);
        }
    }

    if(scan_timer != Min_Cnt)
    {
        scan_timer = Min_Cnt;
    }
    else
    {
        return;                 //同一分钟时标仅使用一次
    }

    tmp = (scan_timer - save_time) % SCAN_TIME;//每SCAN_TIME分钟停充一分钟，每SCAN_TIME+1分钟检测一次Vbat
    if(0 == tmp)                //定时切换为禁充状态来正确扫描Vbat值以便合理控制电池充电
    {
        CHARGE_DIS();           //禁充
        FAST_CHARGE_OFF();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
        WGP_OFF();//DBG
        DBG_LED_OFF();//DBG
#endif
    }
    else if(1 == tmp)           //状态切换1分钟后再扫描采样值比较稳定
    {
        vbat = Ad_Data[BAT_GPRS];
        if(CRG_STA_FLG & RTCGPREG0)  //电池未曾被充满
        {
            if(vbat < FCG_GATE_HIGH)
            {
                fcg_cnt++;
                if(fcg_cnt < ((U16)FCG_CNT))
                {
                    CHARGE_EN();        //快充
                    FAST_CHARGE_ON();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
//                    WGP_ON();//DBG
#endif
                }
                else
                {
                    CHARGE_EN();        //切为慢充
                    FAST_CHARGE_OFF();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
                    DBG_LED_ON();//DBG
#endif
                    RTCGPREG0 &= ~((U32)CRG_STA_FLG);  //标识清零，切回到饱和态
                }
            }
            else
            {
                CHARGE_EN();        //切为慢充
                FAST_CHARGE_OFF();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
                DBG_LED_ON();//DBG
#endif                
                RTCGPREG0 &= ~((U32)CRG_STA_FLG);  //标识清零，切回到饱和态
            }
        }
        else    //电池曾被充满过
        {
            if(vbat <= FCG_GATE_LOW)
            {
                CHARGE_EN();        //切为快充
                FAST_CHARGE_ON();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
//                WGP_ON();//DBG
#endif
                RTCGPREG0 |= CRG_STA_FLG;    //标识置位,切换至未饱和态
            }
            else if(vbat > SCG_GATE_MIN)
            {
                CHARGE_EN();        //慢充
                FAST_CHARGE_OFF();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
                DBG_LED_ON();//DBG
#endif
                RTCGPREG0 &= ~((U32)CRG_STA_FLG);  //标识清零，保持饱和态,相当于初始化
            }
        }
    }
}

/******************************************************************************
** 函数名称: void battery_charge_mange(void)
** 函数功能: GPRS电池充电管理功能函数
** 入口参数: 无
** 返 回 值: 无
** 说    明: 在非掉电工况下监管GPRS电池的有无、快慢充、过充、过放处理
             在升压硬件基础上兼容2.4V与4.8V电池充电管理(适用于V1.3及以上硬件版本)
******************************************************************************/
void battery_charge_mange (void)
{
    U8 volt_type;
    U16 tmp;
    U16 vbat;
static U8 fcg_cnt = 0xFF;       //快充计数器,第一次fcg_cnt++后翻转，从0开始计数
static U8 save_time = 0;        //当检测到有电池时为迅速进入充电管理需要节约的时间:分钟数
static U16 scan_timer = 0xFFFF; //防止同一个时标被多次使用，应该仅使用一次即可
    
    if(!PORT0_IS_HIGH(P0_PD_CHK))           //掉电工况,不予处理
    {
        CHARGE_DIS();                       //充电禁能
        FAST_CHARGE_OFF();                  //快充关闭
#if CODE_VER_TYPE==GPRS_MODULE_PRJ
		RUN_LED_OFF(); 
#else//H_METER_PRJ,WLS_CUR_PRJ
        YGP_OFF();                          //节省功耗
        ALARM_LED_OFF();                    //节省功耗
        WGP_OFF();//DBG
        DBG_LED_OFF();//DBG
#endif
        Bat_Stat_Flag &= ~(0x0F << CRG_STA_FLG_BIT);    //相关位先清零
        Bat_Stat_Flag |= (DIS_CRG << CRG_STA_FLG_BIT);  //设置GPRS电池状态字以备其它函数查询
        return;
    }

    if(!(smp_ok_flg & (1 << BAT_GPRS)))     //AD采样数据无效
    {
        return;
    }
    else                                    //采样有效，则进行有无电池动态判断
    {
        smp_ok_flg &= ~(1 << BAT_GPRS);     //标识复位

        vbat = Ad_Data[BAT_GPRS];
        if((vbat >= BAT_24V_MIN) && (vbat < BAT_24V_MAX))       //若是2.4V电池
        {
            Bat_Stat_Flag &= ~(0x0F << RATED_VOLT_FLG_BIT);     //相关位先清零
            Bat_Stat_Flag |= (RATED_VOLT_FLG_24V << RATED_VOLT_FLG_BIT);    //设置GPRS电池状态字以备其它函数查询
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
            ALARM_LED_OFF();                //无电池报警灯灭
#endif
            if(vbat > BAT_24V_LOW)          //正常
            {
                Bat_Stat_Flag &= ~(0x0F << (BAT_GPRS * 4));     //相关位先清零
                Bat_Stat_Flag |= (BAT_NORMAL << (BAT_GPRS * 4));//设置GPRS电池状态字以备其它函数查询
            }
            else                            //欠压
            {
                Bat_Stat_Flag &= ~(0x0F << (BAT_GPRS * 4));     //相关位先清零
                Bat_Stat_Flag |= (BAT_LOW << (BAT_GPRS * 4));   //设置GPRS电池状态字以备其它函数查询
            }
        }
        else if((vbat >= BAT_48V_MIN) && (vbat <= BAT_48V_MAX)) //若是4.8V电池
        {
            if((Bat_Stat_Flag & (1 << CRG_STA_FLG_BIT)) && (vbat > SCG_NO_BAT))//若在慢充时发现电池电压大于SCG_NO_BAT,则判断为无电池
            {
                goto __no_bat;          //进行无电池处理
            }
            Bat_Stat_Flag &= ~(0x0F << RATED_VOLT_FLG_BIT);     //相关位先清零
            Bat_Stat_Flag |= (RATED_VOLT_FLG_48V << RATED_VOLT_FLG_BIT);    //设置GPRS电池状态字以备其它函数查询
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
            ALARM_LED_OFF();                //无电池报警灯灭
#endif
            if(vbat > BAT_48V_LOW)          //正常
            {
                Bat_Stat_Flag &= ~(0x0F << (BAT_GPRS * 4));     //相关位先清零
                Bat_Stat_Flag |= (BAT_NORMAL << (BAT_GPRS * 4));//设置GPRS电池状态字以备其它函数查询
            }
            else                            //欠压
            {
                Bat_Stat_Flag &= ~(0x0F << (BAT_GPRS * 4));     //相关位先清零
                Bat_Stat_Flag |= (BAT_LOW << (BAT_GPRS * 4));   //设置GPRS电池状态字以备其它函数查询
            }
        }
        else        //其余情况(vbat <= 1.0V || vbat > 6.0V)均视为无电池
        {
__no_bat:   CHARGE_DIS();                   //充电禁能
            FAST_CHARGE_OFF();              //快充关闭
            Bat_Stat_Flag &= ~(0x0F << (BAT_GPRS * 4));     //相关位先清零
            Bat_Stat_Flag |= (BAT_NO << (BAT_GPRS * 4));    //设置GPRS电池状态字以备其它函数查询
            Bat_Stat_Flag |= (0x0F << RATED_VOLT_FLG_BIT);  //电池类型标识复位为无效
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
            ALARM_LED_ON();                 //无电池报警灯亮
            WGP_OFF();//DBG
            DBG_LED_OFF();//DBG
#endif            
            Bat_Stat_Flag &= ~(0x0F << CRG_STA_FLG_BIT);    //相关位先清零
            Bat_Stat_Flag |= (DIS_CRG << CRG_STA_FLG_BIT);  //设置GPRS电池状态字以备其它函数查询
            fcg_cnt = 0xFF;     //计数器复位
            
            save_time = Min_Cnt % SCAN_TIME;//计算检测到电池从无到有应节约的时间,以迅速启动充电管理流程,同时避免无电池时进入else if(1 == tmp)代码段
        }
    }
    //-----------------------------有电池时充电管理---------------------------------------------
    if(scan_timer != Min_Cnt)
    {
        scan_timer = Min_Cnt;
    }
    else
    {
        return;                 //同一分钟时标仅使用一次
    }
    volt_type = (U8)(0x01 & (Bat_Stat_Flag >> RATED_VOLT_FLG_BIT)); //获取电池类型信息,以备后面自适应门限选择
    tmp = (scan_timer - save_time) % SCAN_TIME;//每SCAN_TIME分钟停充一分钟，每SCAN_TIME+1分钟检测一次Vbat
    if(0 == tmp)                //定时切换为禁充状态来正确扫描Vbat值以便合理控制电池充电
    {
        CHARGE_DIS();           //禁充
        FAST_CHARGE_OFF();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
        WGP_OFF();//DBG
        DBG_LED_OFF();//DBG
#endif        
        Bat_Stat_Flag &= ~(0x0F << CRG_STA_FLG_BIT);    //相关位先清零
        Bat_Stat_Flag |= (DIS_CRG << CRG_STA_FLG_BIT);  //设置GPRS电池状态字以备其它函数查询
    }
    else if(1 == tmp)           //状态切换1分钟后再扫描采样值比较稳定
    {
        vbat = Ad_Data[BAT_GPRS];
        if(CRG_STA_FLG & RTCGPREG0)  //电池未曾被充满
        {
            if(vbat < (FCG_GATE_HIGH >> volt_type))
            {
                fcg_cnt++;
                if(fcg_cnt <= ((U16)FCG_CNT))
                {
                    CHARGE_EN();    //快充
                    FAST_CHARGE_ON();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
//                    WGP_ON();//DBG
#endif                    
                    Bat_Stat_Flag &= ~(0x0F << CRG_STA_FLG_BIT);    //相关位先清零
                    Bat_Stat_Flag |= (FST_CRG << CRG_STA_FLG_BIT);  //设置GPRS电池状态字以备其它函数查询
                }
                else
                {
                    fcg_cnt = 0xFF;     //计数器复位
                    save_time = 0;      //复位
                    CHARGE_EN();        //切为慢充
                    FAST_CHARGE_OFF();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
                    DBG_LED_ON();//DBG
#endif                    
                    Bat_Stat_Flag &= ~(0x0F << CRG_STA_FLG_BIT);    //相关位先清零
                    Bat_Stat_Flag |= (SLW_CRG << CRG_STA_FLG_BIT);  //设置GPRS电池状态字以备其它函数查询
                    RTCGPREG0 &= ~((U32)CRG_STA_FLG);  //标识清零，切回到饱和态
                }
            }
            else
            {
                fcg_cnt = 0xFF;     //计数器复位
                save_time = 0;      //复位
                CHARGE_EN();        //切为慢充
                FAST_CHARGE_OFF();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
                DBG_LED_ON();//DBG
#endif                
                Bat_Stat_Flag &= ~(0x0F << CRG_STA_FLG_BIT);    //相关位先清零
                Bat_Stat_Flag |= (SLW_CRG << CRG_STA_FLG_BIT);  //设置GPRS电池状态字以备其它函数查询
                RTCGPREG0 &= ~((U32)CRG_STA_FLG);  //标识清零，切回到饱和态
            }
        }
        else    //电池曾被充满过
        {
            if(vbat <= (FCG_GATE_LOW >> volt_type))
            {
                CHARGE_EN();        //切为快充
                FAST_CHARGE_ON();
                fcg_cnt++;
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
//                WGP_ON();//DBG
#endif                
                Bat_Stat_Flag &= ~(0x0F << CRG_STA_FLG_BIT);    //相关位先清零
                Bat_Stat_Flag |= (FST_CRG << CRG_STA_FLG_BIT);  //设置GPRS电池状态字以备其它函数查询
                RTCGPREG0 |= CRG_STA_FLG;    //标识置位,切换至未饱和态
            }
            else if(vbat > (SCG_GATE_MIN >> volt_type))
            {
                CHARGE_EN();        //慢充
                FAST_CHARGE_OFF();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
                DBG_LED_ON();//DBG
#endif                
                Bat_Stat_Flag &= ~(0x0F << CRG_STA_FLG_BIT);    //相关位先清零
                Bat_Stat_Flag |= (SLW_CRG << CRG_STA_FLG_BIT);  //设置GPRS电池状态字以备其它函数查询
                RTCGPREG0 &= ~((U32)CRG_STA_FLG);  //标识清零，保持饱和态,相当于初始化
            }
        }
    }
}

/******************************************************************************
** 函数名称: U8 get_batt_state(U8 bat_num)
** 函数功能: 主板电池(及GPRS模块类型)在线状态获取功能函数
** 入口参数: bat_num 电池(或GPRS模块类型)，取值出自ad_ain.h的枚举SMP_TYPE的内容
             包括BAT_GPRS、BAT_RTC、BAT_PDB、EVE_TYP
** 返 回 值: 电池状态: BAT_NORMAL:正常  BAT_LOW:欠压   BAT_NO:没加电池 BAT_UNKNOWN:未知状态
             GPRS类型: 0:MC52I 1:待补充 2:待补充 3:待补充 4:待补充 GPRS_UNKNOWN:未知状态
** 说    明: 用于电池在线状态或GPRS模块类型判断,若返回0xFF表明未来及作出状态判断
             仅针对4.8V电池充电管理
******************************************************************************/
U8 get_batt_state (U8 bat_num)
{
    U8 state;
    U16 vbat;

    switch(bat_num)
    {
    case BAT_GPRS:

        break;
    case BAT_RTC:
        if(smp_ok_flg & (1 << BAT_RTC))  //AD采样数据有效
        {
            smp_ok_flg &= ~(1 << BAT_RTC);     //标识复位
            vbat = Ad_Data[BAT_RTC];
            if(vbat < REF_V0_1)         //RTC电池电压 < 0.2V 为无电池
            {
                bat_stat_flg &= ~(0x0F << (BAT_RTC * 4));       //相关位先清零
                bat_stat_flg |= (BAT_NO << (BAT_RTC * 4));      //设置RTC电池状态字以备其它函数查询
            }
            else if(vbat < REF_V1_4)    //RTC电池电压 < 2.8V + 0.7V(管压降)为欠压
            {
                bat_stat_flg &= ~(0x0F << (BAT_RTC * 4));       //相关位先清零
                bat_stat_flg |= (BAT_LOW << (BAT_RTC * 4));     //设置RTC电池状态字以备其它函数查询
            }
            else                        //RTC电池电压 >= 2.8V + 0.7V(管压降)为正常
            {
                bat_stat_flg &= ~(0x0F << (BAT_RTC * 4));       //相关位先清零
                bat_stat_flg |= (BAT_NORMAL << (BAT_RTC * 4));  //设置RTC电池状态字以备其它函数查询
            }
        }
        break;
    case BAT_PDB:
        if(smp_ok_flg & (1 << BAT_PDB)) //AD采样数据有效
        {
            smp_ok_flg &= ~(1 << BAT_PDB);      //标识复位
            vbat = Ad_Data[BAT_PDB];
            if(vbat < REF_V0_1)         //停显电池电压 < 0.2V 为无电池
            {
                bat_stat_flg &= ~(0x0F << (BAT_PDB * 4));       //相关位先清零
                bat_stat_flg |= (BAT_NO << (BAT_PDB * 4));      //设置停显电池状态字以备其它函数查询
            }
            else if(vbat < REF_V2_8)    //停显电池电压 < 5.6V  为欠压
            {
                bat_stat_flg &= ~(0x0F << (BAT_PDB * 4));       //相关位先清零
                bat_stat_flg |= (BAT_LOW << (BAT_PDB * 4));     //设置停显电池状态字以备其它函数查询
            }
            else                        //停显电池电压 >= 5.6V 为正常
            {
                bat_stat_flg &= ~(0x0F << (BAT_PDB * 4));       //相关位先清零
                bat_stat_flg |= (BAT_NORMAL << (BAT_PDB * 4));  //设置停显电池状态字以备其它函数查询
            }
        }
        break;
    case EVE_TYP:
        if(smp_ok_flg & (1 << EVE_TYP)) //AD采样数据有效
        {
            smp_ok_flg &= ~(1 << EVE_TYP);      //标识复位
            vbat = Ad_Data[EVE_TYP];
            state = get_gprs_type(vbat);
            bat_stat_flg &= ~(0x0F << (EVE_TYP * 4)); //相关位先清零
            bat_stat_flg |= (state << (EVE_TYP * 4)); //设置GPRS模块类型字以备其它函数查询
        }
        break;
    default:
        return 0xEE;    //入口参数错误
    }

	state = (U8)((bat_stat_flg >> (bat_num * 4)) & 0x0F);
    return state;
}

/******************************************************************************
** 函数名称: U8 get_battery_state(U8 bat_num)
** 函数功能: 主板电池(及GPRS模块类型)在线状态获取功能函数
** 入口参数: bat_num 电池(或GPRS模块类型)，取值出自ad_ain.h的枚举SMP_TYPE的内容
             包括BAT_GPRS、BAT_RTC、BAT_PDB、EVE_TYP
** 返 回 值: 电池状态: BAT_NORMAL:正常  BAT_LOW:欠压   BAT_NO:没加电池 BAT_UNKNOWN:未知状态
             GPRS类型: 0:MC52I 1:待补充 2:待补充 3:待补充 4:待补充 GPRS_UNKNOWN:未知状态
** 说    明: 用于电池在线状态或GPRS模块类型判断,若返回0xFF表明未来及作出状态判断
             兼容2.4V与4.8V电池充电管理(适用于V1.3及以上硬件版本)
******************************************************************************/
U8 get_battery_state (U8 bat_num)
{
    U8 state;
    U16 vbat;

    switch(bat_num)
    {
    case BAT_GPRS:

        break;
    case BAT_RTC:
        if(smp_ok_flg & (1 << BAT_RTC))  //AD采样数据有效
        {
            smp_ok_flg &= ~(1 << BAT_RTC);     //标识复位
            vbat = Ad_Data[BAT_RTC];
            if(vbat < REF_V0_1)         //RTC电池电压 < 0.2V 为无电池
            {
                Bat_Stat_Flag &= ~(0x0F << (BAT_RTC * 4));  //相关位先清零
                Bat_Stat_Flag |= (BAT_NO << (BAT_RTC * 4)); //设置RTC电池状态字以备其它函数查询
            }
            else if(vbat < REF_V1_4)    //RTC电池电压 < 2.8V + 0.7V(管压降)为欠压
            {
                Bat_Stat_Flag &= ~(0x0F << (BAT_RTC * 4));  //相关位先清零
                Bat_Stat_Flag |= (BAT_LOW << (BAT_RTC * 4));//设置RTC电池状态字以备其它函数查询
            }
            else                        //RTC电池电压 >= 2.8V + 0.7V(管压降)为正常
            {
                Bat_Stat_Flag &= ~(0x0F << (BAT_RTC * 4));      //相关位先清零
                Bat_Stat_Flag |= (BAT_NORMAL << (BAT_RTC * 4)); //设置RTC电池状态字以备其它函数查询
            }
        }
        break;
    case BAT_PDB:
        if(smp_ok_flg & (1 << BAT_PDB)) //AD采样数据有效
        {
            smp_ok_flg &= ~(1 << BAT_PDB);      //标识复位
            vbat = Ad_Data[BAT_PDB];
            if(vbat < REF_V0_1)         //停显电池电压 < 0.2V 为无电池
            {
                Bat_Stat_Flag &= ~(0x0F << (BAT_PDB * 4));  //相关位先清零
                Bat_Stat_Flag |= (BAT_NO << (BAT_PDB * 4)); //设置停显电池状态字以备其它函数查询
            }
            else if(vbat < REF_V2_8)    //停显电池电压 < 5.6V  为欠压
            {
                Bat_Stat_Flag &= ~(0x0F << (BAT_PDB * 4));  //相关位先清零
                Bat_Stat_Flag |= (BAT_LOW << (BAT_PDB * 4));//设置停显电池状态字以备其它函数查询
            }
            else                        //停显电池电压 >= 5.6V 为正常
            {
                Bat_Stat_Flag &= ~(0x0F << (BAT_PDB * 4));      //相关位先清零
                Bat_Stat_Flag |= (BAT_NORMAL << (BAT_PDB * 4)); //设置停显电池状态字以备其它函数查询
            }
        }
        break;
    case EVE_TYP:
        if(smp_ok_flg & (1 << EVE_TYP)) //AD采样数据有效
        {
            smp_ok_flg &= ~(1 << EVE_TYP);      //标识复位
            vbat = Ad_Data[EVE_TYP];
            state = get_gprs_type(vbat);
            Bat_Stat_Flag &= ~(0x0F << (EVE_TYP * 4));  //相关位先清零
            Bat_Stat_Flag |= (state << (EVE_TYP * 4));  //设置GPRS模块类型字以备其它函数查询
        }
        break;
    default:
        return 0xEE;    //入口参数错误
    }

	state = (U8)((Bat_Stat_Flag >> (bat_num * 4)) & 0x0F);
    return state;
}

/******************************************************************************
** 函数名称: U8 get_gprs_type(U16 eveo_volt)
** 函数功能: 获取GPRS模块类型功能函数
** 入口参数: eveo_volt 事件输出电压值，依此判定事先定义好的GPRS模块类型
** 返 回 值: GPRS模块类型:见ad_ain.h枚举GPRS_TYPE内容,其它无效
** 说    明: 获取GPRS模块型号信息,共五种类型,目前仅定义一种，其它待补充
******************************************************************************/
U8 get_gprs_type(U16 eveo_volt)
{
    if((eveo_volt > EVEO_ONE_SIXTH - EVEO_LIMIT_VOLT) && (eveo_volt < EVEO_ONE_SIXTH + EVEO_LIMIT_VOLT))
    {
        return GPRS_MC52I;
    }
    else if((eveo_volt > EVEO_ONE_FIFTH - EVEO_LIMIT_VOLT) && (eveo_volt < EVEO_ONE_FIFTH + EVEO_LIMIT_VOLT))
    {
        return GPRS_MC53I;
    }
    else if((eveo_volt > EVEO_ONE_QUARTER - EVEO_LIMIT_VOLT) && (eveo_volt < EVEO_ONE_QUARTER + EVEO_LIMIT_VOLT))
    {
        return GPRS_MC54I;
    }
    else if((eveo_volt > EVEO_ONE_THIRD - EVEO_LIMIT_VOLT) && (eveo_volt < EVEO_ONE_THIRD + EVEO_LIMIT_VOLT))
    {
        return GPRS_MC55I;
    }
    else if((eveo_volt > EVEO_ONE_HALF - EVEO_LIMIT_VOLT) && (eveo_volt < EVEO_ONE_HALF + EVEO_LIMIT_VOLT))
    {
        return GPRS_MC56I;
    }
    else
    {
        return 0x0F;    //未知
    }
}
/******************************************************************************
    End Of File
******************************************************************************/


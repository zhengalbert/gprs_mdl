/******************************************************************************
 Copyright (C) 2011  R&D Institute of HuaCai Co., Ltd.
 Module         : lcd display function head file
 File Name      : lcd_display.h
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

#ifndef __LCD_DISPLAY_H
#define __LCD_DISPLAY_H

#ifdef LCD_VALID
/*----------------------------- macro definition ----------------------------*/
#define CON_DIS_PARA_INVALID  0
#define CON_DIS_PARA_VALID    0xA1

#define DN_SN_DIRECT        0//上次往下翻
#define UP_SN_DIRECT        1//上次往上翻

#define PON_INT_SN_FLAG     (1<<0)
#define BSN_CHG_SN_FLAG     (1<<1)
#define LSN_CHG_SN_FLAG     (1<<2)
#define MOD_CHG_SN_FLAG     (1<<3)

#define DIS_AUX_EVET_NUM    69
#define DIS_AUX_PARA_NUM    4

//默认显示表格是数组下标
#if CODE_VER_TYPE==WLS_CUR_PRJ			//配合无线采集器
	#define DEFAULT_AUTO_DIS_MAX_NUM   21//18//
	#define DEFAULT_KEY_DIS_MAX_NUM    21//
#else
#define DEFAULT_AUTO_DIS_MAX_NUM   18//
#define DEFAULT_KEY_DIS_MAX_NUM    59//
#endif

#define DIS_MODE_NUM        3//显示模式
//从零开始 要做为数组的下标的
#define DIS_MODE_AUTO       0//自动轮显模式
#define DIS_MODE_SKEY       1//普通键显
#define DIS_MODE_LKEY       2//长按后快速翻页模式
#define DIS_MODE_ERRON      3//出错时 停在错误屏 除非按键去按
#define DIS_MODE_ALLON      4//上电全显示模式
#define DIS_MODE_MATCH      5//搜索表地址 配对显示状态

#define DIS_TIME_AUTO       30//100ms unit
#define DIS_TIME_SKEY       150//30//按键显示退出时间
#define DIS_TIME_STLPKEY    20//第一次进入长按快速切屏模式所需要的时间
#define DIS_TIME_LPKEY      5//长按模式下按键轮显时间0.5S屏
#define DIS_TIME_BLIGHT     100//100ms unit 10S
//-----------------------------------------------------------------------------
// 定义电表状态字中对应在LCD屏需要闪烁的字符
//-----------------------------------------------------------------------------
#define CON_DIS_DATA_OFFSET         3  //显示数据在整个缓冲的编移
#define CON_FLASH_BIT_MAX_NUM       9  //需要闪烁的状态数
//电表状态字里需要闪烁的符号gState_Flash_Flag
#define CON_RTCBAT_FLASH_BIT        (1<<0)  //时钟电池欠压闪烁位
#define CON_RMTBAT_FLASH_BIT        (1<<1)  //抄表电池欠压
#define CON_ALOWVOLT_FLASH_BIT      (1<<2)  //A相失压 断相
#define CON_ALOWCURT_FLASH_BIT      (1<<3)  //A相失流 断流
#define CON_BLOWVOLT_FLASH_BIT      (1<<4)  //B相失压 断相
#define CON_BLOWCURT_FLASH_BIT      (1<<5)  //B相失流 断流
#define CON_CLOWVOLT_FLASH_BIT      (1<<6)  //C相失压 断相
#define CON_CLOWCURT_FLASH_BIT      (1<<7)  //C相失流 断流
#define CON_ERRPHASE_FLASH_BIT      (1<<8)  //电压,电流逆相序
//通信闪烁符号gState_Com_Flag
#define CON_COM_RS4851_FLASH_BIT    (1<<0)  //电话机1 对应rs485_1通信状态
#define CON_COM_RS4852_FLASH_BIT    (1<<1) //电话机2 对应rs485_2通信状态
#define CON_COM_GPRS_BAT_BIT        (1<<2) //模块充电电池 闪烁标志
#define CON_COM_DIS_BAT_BIT         (1<<3) //停电电池 闪烁标志
/*----------------------------- type definition -----------------------------*/
typedef struct _DIS_PARA
{
    U8  maxn[2];        //两种显示模式的总屏数（编程屏数）
    U8  dly[2];         //当前自动轮显序号
    U8  para_valid[2];  //显示参数 有效性 如果外部没有设置 过调用默认显示表格
}DIS_PARA;

typedef struct _DIS_CTRL
{
    U8  had_write;      //100ms循环内已写过一遍
    U8  dis_dly_time;   //先同步数据数据 然后延时100ms再去刷新显示
    U8  pre_key;        //上次按下的键值
    U8  cur_diret;      //上次按键方向用于处理小项往上翻
    U8  cur_mode;       //当前模式
    U8  dis_bsn[2];     //当前显序号
    U8  exd_lsn[2];     //扩展用的小序号
    U8  lsn_maxn[2];    //小序号总数
    U8  flash_flag;     //不等到1秒就要刷新
    U8  di_finish;      //显示时抄的数据标识完成
    U8  di_resend_cnt;  //上面的重发次数
    U16 t_count;        //计数器
    U16 lpress_ct;      //长按键模式计数
    U32 di;
}DIS_CTRL;

typedef struct{
    U32 di;         //645规约标识编码
    U8  sn;       //拆分项起始序号
    U8  maxn;        //拆分后总条目数
} DIS_AUX_CTRL;


/*-------------------------- functions declaration --------------------------*/
__EXTERN    __task void lcd_display_task (void);
__EXTERN S8 fill_dis_data_buf(void);
__EXTERN void lcd_init(U8 mode);
__EXTERN U8 get_meter_serious_err_id(void);
__EXTERN void fill_dis_err_page(U8 *errn, U8 *segbuf);
__EXTERN U8 check_meter_serious_happened(void);
__EXTERN void fill_dis_err_page(U8 *errn, U8 *segbuf);

__EXTERN void lcd_mode_ctrl(void);
__EXTERN void rollback_flash_flag (void);
__EXTERN void lcd_display_flash(U8 cnt);
__EXTERN void fill_dis_flash_data(void);
__EXTERN void get_flash_default_flag(void);
__EXTERN void get_dis_code(U8 mode);
__EXTERN S8 fill_dis_data(void);
__EXTERN void lcd_reset(void);
__EXTERN void lcd_pd_mode (void);
__EXTERN void set_dis_para_flag(U8 id);
__EXTERN U8 check_dis_para_valid(U8 mode);
__EXTERN void fill_gprs_signal_strenth_segbuf(U8 *segbuf);
__EXTERN void set_di_sync_finish(U8 flag);
	#if CODE_VER_TYPE==WLS_CUR_PRJ			//配合无线采集器
		__EXTERN U8 get_dis_mp(void);
		__EXTERN void get_dis_temp_data(U32 di, U8 lsn);
	#endif
/*--------------------------- variable declaration --------------------------*/
__EXTERN DIS_CTRL Dis_Ctrl;
__EXTERN DIS_PARA Dis_Para;

__EXTERN U16 gBackLight_On_Time;
__EXTERN U16 gState_Flash_Flag;             //对应电表状态字需要闪烁的符号以们代表
__EXTERN U8  gState_Com_Flag;               //485通信闪烁符号 二个电话机
#endif//LCD_VALID

#endif//__LCD_DISPLAY_H
/******************************************************************************
                            End Of File
 *****************************************************************************/

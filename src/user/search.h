/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 Module         : search.c头文件
 File Name      : search.h
 Description    :
 Author         :
 Build Date     :
 others         :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、     1.00       2011-08-15     azh            create
******************************************************************************/

#ifndef __SEARCH_H
#define __SEARCH_H

#ifdef ZIGBEE_TEST//azh 171011//CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不是带CPU_GPRS模块
/*----------------------------- macro definition ----------------------------*/
//-----------------------------------------------------------------------------
// 定义搜索表状态的一些宏定义
//-----------------------------------------------------------------------------
#define CON_SEARCH_IDLE_STATE      0//空闲状态
#define CON_SEARCH_RUN_STATE       1//发送命令搜索状态
#define CON_SEARCH_DISPLY_STATE    2//搜索完成显示状态
#define CON_SEARCH_FINISH_STATE    3//搜索,并保存后结束

#define CON_SEARCH_CMD_MAX          16
#define CON_SEARCH_ADDR_LEN         (6+8+2) //addr LEN(6)+ zigbee adr(8) + CRC(2)
#define DIS_SEARCH_JUDGE_TIME       20      //100ms unit 3S   同时按住上下翻页键计时
#define DIS_SEARCH_INTERVAL_TIME    10      //100ms unit 3S    发搜索间隔时间
#define DIS_SEARCH_KEY_TIME         3       //100ms unit 3S    按键确认时间延时
#define DIS_SEARCH_DIS_TIME         20       //100ms unit 3S    显示结果状态
/*----------------------------- type definition -----------------------------*/
typedef struct _DIS_SEARCH_CTRL
{
    U8  state;          //状态：1搜表状态 2搜索完成显示状态 3搜索,并保存后结束
    U8  dis_sn;         //搜索到表的显示序号
    U8  totnum;         //搜索到表的数量
    U8  cmd_ct;         //搜表命令发的最大次数
    U8  state_dly;      //用于状态延时100ms unit
    U8  pre_key;        //用于按键判定的
    U8  addr[CON_SEARCH_CMD_MAX][CON_SEARCH_ADDR_LEN];      //可以同时保存8个表地址用于显示
}DIS_SEARCH_CTRL;

/*-------------------------- functions declaration --------------------------*/
__EXTERN    U8 analyse_identity_frame(U8 *in_ptr);
__EXTERN    void send_match_module_frame(void);
__EXTERN    U8 check_match_valid(void);
__EXTERN    void init_search_data(void);
__EXTERN    void set_search_data(void);
__EXTERN    void store_match_addr(U8 index);
__EXTERN    void handle_search_state(void);
__EXTERN void judge_search_state(void);
__EXTERN void send_identity_frame(void);
__EXTERN void store_zigbee_addr(U8 *addr);
__EXTERN void store_hvmeter_addr(U8 *addr);
__EXTERN void get_match_addr(void);
/*--------------------------- variable declaration --------------------------*/
__EXTERN DIS_SEARCH_CTRL Dis_Search_Ctrl;
__EXTERN U16 GangMarkTime;                              //接到/? 等待OK期间的定时
#endif //CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不是带CPU_GPRS模块

#endif//__SEARCH_H
/******************************************************************************
                            End Of File
 *****************************************************************************/

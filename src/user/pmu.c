/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : pmu.c
 Description    : LPC11系列芯片电源管理子程序
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、    2.00        2011-02-25     rosoon
   1、    1.00        2010-09-14    rosoon     the original version
******************************************************************************/

#include "..\inc\global_config.h"

//-------------------------------- 常数定义 -----------------------------------

//------------------------------ 私有变量声明 ---------------------------------

//------------------------------ 私有函数声明 ---------------------------------


/******************************************************************************
** 函数名称: void before_pd_manage (void)
** 函数功能: 刚掉电后 电源管理(由哪个电池供电 供多长时间), 重新上电时 也是通过看门狗外部复位
** 入口参数:
**
** 返 回 值: 无
** 说    明: 兼容两类电池
******************************************************************************/
void before_pd_pwr_manage (void)
{
#ifndef PD_MANAGE_VER12
    U8 volt_type;   //为了兼容2.4V与4.8V两种类型电池的不同判定门限
#endif
    if(Power_Down_Flag == PWR_DN)//外部没电
    {
        if(gstB_Pd_Ctrl.last_state == PWR_ON)//刚停电
        {
            gstB_Pd_Ctrl.dncount = BAT_ON_MXTIME;//掉电充电池最长工作时间
            CHARGE_DIS();                       //充电禁能
            FAST_CHARGE_OFF();                  //快充关闭
            if(stModuleRunState.bIsGPRSConnected == 1)//如果在线的话 填写掉电上报告警
            {
//azh
//                up_send_pd_cmd();//填写掉电告警命令
                gstB_Pd_Ctrl.send_time = 600; //30秒
            }
            else//不在线
            {
                PIN_POWERON_CLR;//在掉电时，不在线则关闭GPRS模块电源
                gstB_Pd_Ctrl.send_time =600;//60秒
            }
            gstB_Pd_Ctrl.cmd_sended = 1;
            Alarm_Save_Last_TerTime();//刚掉电保存一次
//
            powdn_save_day_month_data(CON_DAYDATA_TYPE);//900多字节
            powdn_save_day_month_data(CON_MONTHDATA_TYPE);
        }
#ifndef PD_MANAGE_VER12
        volt_type = (U8)(0x01 & (Bat_Stat_Flag >> RATED_VOLT_FLG_BIT)); //获取电池类型信息,以备后面自适应门限选择
        if(((Ad_Data[BAT_GPRS] <= (BAT_LOW_GATE_VOLT >> volt_type))&&(Ad_Data[BAT_GPRS] != 0)) || (gstB_Pd_Ctrl.dncount == 0))//<=4.8V or 掉电电池工作时间
        {
            disable_irq();              // 关闭所有中断
            BAT_OFF();//充电电池供电关闭
            while(1);//debug
        }
#endif

        if((gstB_Pd_Ctrl.cmd_sended == 1) && (gstB_Pd_Ctrl.send_time == 0))
        {
            disable_irq();              // 关闭所有中断
            BAT_OFF();//充电电池供电关闭
            while(1);//debug
        }
    }
    else//外部有电
    {
        if(gstB_Pd_Ctrl.last_state == PWR_DN)//刚来电
        {
            disable_irq();              // 关闭所有中断
            BAT_OFF();//充电电池供电关闭
            while(1);//debug
        }
    }

    gstB_Pd_Ctrl.last_state = Power_Down_Flag;  //
    if(gstB_Pd_Ctrl.dncount > 0)
    {
        gstB_Pd_Ctrl.dncount--;
    }
    if(gstB_Pd_Ctrl.send_time > 0)
    {
        gstB_Pd_Ctrl.send_time--;
    }
}
/******************************************************************************
** 函数名称: void check_power_on(U16 cnt)
** 函数功能: 系统上电:检测功能函数
** 入口参数: cnt 设定上电检测引脚采样到高电平的次数,取值1~65535
**
** 返 回 值: 无
** 说    明: 经
******************************************************************************/
void check_power_on (U16 cnt)
{
    U8  power_ok_cnt;

    power_ok_cnt = 0;

    SET_PORT0_IN(P0_PD_CHK);                    //配置p0.29为输入口

    while(1)
    {
        if(PORT0_IS_HIGH(P0_PD_CHK))        //检测掉电信号输入
        {
            if(++power_ok_cnt > cnt)        //连续cnt(=200)次检测到掉电信号为高则认为电源已经稳定
            {
                return;
            }
        }
        else
        {
            while(1);
        }
        CLRWDT;                         //清除硬件看门狗
        delay(50);                          //延时约1.2ms，这时的处理器时钟是4MHz
    }
}
/******************************************************************************
Function name: void check_power_down (void)
Author       : snap.gao
Description  : 掉电检查，发生掉电时向每个任务发送消息
Input        : None
Return       : None
******************************************************************************/
void check_power_down (void)
{
    static U8 power_fail_cnt = 0;

    SET_PORT0_IN(P0_PD_CHK);                    //配置p0.29为输入口

    while(1)
    {
        if(PORT0_IS_HIGH(P0_PD_CHK) == 0)   //检测掉电信号输入
        {
            if(++power_fail_cnt > 2)        //连续3次检测到掉电信号则判定掉电
            {
    //            Power_Down_Flag = POWER_DOWN_FLAG;
    //            os_evt_set(POWER_DOWN_FLAG, System_Manage_Task_ID);
    //            os_evt_set(POWER_DOWN_FLAG, Protocol_Status_Task_ID);
    //            os_evt_set(POWER_DOWN_FLAG, SNTP_Task_ID);
    //            os_evt_set(POWER_DOWN_FLAG, Crawl_Task_ID);
    //            while(os_mbx_send(Mbx_Load_Profiler, NULL, 0) == OS_R_TMO)
    //            {
    //                os_dly_wait(2);
    //            }
    //            while(os_mbx_send(Mbx_Comm_Data, NULL, 0) == OS_R_TMO)
    //            {
    //                os_dly_wait(2);
    //            }
                //azh 110624 暂不处理
                //pd_main();//进入掉电主循环
                //
                Power_Down_Flag = PWR_DN;
                BAT_ON();//充电电池供电关闭
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不CPU_GPRS模块
                PVDD_OFF();  //Zigbee部分外设电源
#endif							
                power_fail_cnt = 0;
                break;

//              RTCGPREG0 = COP_PD_1ST_DIS;         //掉电后 复位进入掉电模式后 轮显一遍标志
//              disable_irq();              // 关闭所有中断
//              while(1);//debug
            }
        }
        else
        {
            Power_Down_Flag = PWR_ON;
            power_fail_cnt = 0;
//          BAT_OFF();//充电电池供电关闭
            break;
        }
    }
}

/******************************************************************************
    End Of File
******************************************************************************/

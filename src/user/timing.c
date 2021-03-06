/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name    : timing.c
 Description  : 定时处理任务
-------------------------------------------------------------------------------
 Modification History:
// <No.>  <version>   <time>    <author>    <contents>
   2、
   1、    1.00      2011-07-28  azh      the original version
******************************************************************************/
#include  "..\inc\global_config.h"


extern U16 os_time;                                     //使用系统变量
//------------------------------ 私有变量声明 ---------------------------------

//------------------------------ 私有函数声明 -------------------------------
//static  void    system_timer (void);
//static  U32     get_max_second_index (void);
//static  void    record_power_outages_event (void);
#if CODE_VER_TYPE==H_METER_PRJ				//配合高压表
static  void reflash_exrom_data(U8 sec_tag);
#endif
/******************************************************************************
** 函数名称: void rollback_flash_flag (void)
** 函数功能: zigbee本地通信灯收发指示
**           本地zigbee发送红灯亮 500ms
**           本地zigbee接收绿灯亮
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
//#ifdef ZIGBEE_TEST
//void zigbee_led_flash (void)
//{
//    if(zigbee_led_time[0] > 0)
//    {
//        zigbee_led_time[0]--;//收绿灯亮500ms
//        WLS_GLED_ON();
//    }
//    else
//    {
//        WLS_GLED_OFF();
//    }
//    if(zigbee_led_time[1] > 0)
//    {
//        zigbee_led_time[1]--;//发红灯亮500ms
//        WLS_RLED_ON();
//    }
//    else
//    {
//        WLS_RLED_OFF();
//    }
//}
//#endif

//模块版本 本地灯包括红外口和内部通信口	
//#ifdef GPRS_MODULE_PRJ
void local_led_flash (void)
{
    if(local_led_time[0] > 0)
    {
        local_led_time[0]--;//收绿灯亮500ms
        WLS_GLED_ON();
    }
    else
    {
        WLS_GLED_OFF();
    }
    if(local_led_time[1] > 0)
    {
        local_led_time[1]--;//发红灯亮500ms
        WLS_RLED_ON();
    }
    else
    {
        WLS_RLED_OFF();
    }
}
//#endif  
/******************************************************************************
 Function name:  __task void timing_task (void)
 Author       :
 Description  : run per 100ms
 Input        :  None
 Return       :  None
若有接到/? 每秒都尝试间隔4秒重新报送
 *****************************************************************************/
__task void timing_task (void)
{
    U32 loop_cnt = 0;
    U8  msec_cnt = 0;
    U8  sec_cnt = 0;

#ifdef ZIGBEE_TEST
    U16 tmp;                                            //此处的U8不节省内存, 很可能代码效率更低, 这里U16是为和os_time一致
#endif
//    Second_Index = 0;                           //上电后从‘0’秒开始
//    reflash_system_RTC();                       //初始化实时时钟

//    Power_Up_Record_Delay = 0;
 ////////   get_match_addr();//azh 120516

    PowOn_Para_Init_All();//上电初始参数
    Init_Main_Comm();//azh
    MC55_Init();
//
#ifdef LCD_VALID
    lcd_reset();
    lcd_init(0);
#endif    
#ifdef HARD_RTC
    sys_rtc_init();
#endif

#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不是CPU_GPRS模块
    init_search_data();
    get_flash_default_flag();
#endif
    file_read(ENG_STORE_START_ADDR, gEngArray, ENG_RAM_ARRAY_MAX);	//此句来自main.c的line291，暂时解决hardfault问题
	RunData_Init();
	CTask_Proc_Init();//azh

    Alarm_Init(CON_PARA_INIT_POW_ON);
    
    DataProc_Stat_TerResetNum_Inc();            //记录终端复位次数

    for(;;)
    {
        sign_task_schedule(TIMING_TASK_PRIO);
#ifdef PD_MANAGE_VER12
        batt_charge_mange();    //仅用于V1.2主板4.8V电池充电管理
#else
        battery_charge_mange(); //用于V1.3及以上主板升压方案兼容2.4V与4.8V电池充电管理
#endif
        before_pd_pwr_manage();
/*
    if(1 == is_run_position())              //代码运行在低区
    {
      SET_TEST_LED_REV;
    }
*/
        PIN_Timer();                  //azh 原先这二个函数是10ms调用的
        AT_Timer();

        if((loop_cnt % 5) == 0)             //GPRS时间管理
        {
            HeartBeat_Manage();             //心跳管理
            MC55_Comm_Timer();              //azh 110928 500ms调用一次
            DATALED_CLR;
        }

        if(msec_cnt == 0)                //每秒钟定时
        {
            msec_cnt = 0;
            sec_cnt++;
            if(!(sec_cnt % 60))
            {
                sec_cnt = 0;
                Min_Cnt++;
#ifdef HARD_RTC
                check_sys_rtc();
#endif
            }
#if CODE_VER_TYPE==H_METER_PRJ				//配合高压表
            reflash_exrom_data(sec_cnt);  //外存数据定时刷新处理
#endif
      //每分钟同步到时钟以后 每秒自己处理(硬件和片上时钟都不启用
            if(software_rtc_run())                          //处理软件RTC, 发现日期要更新
            {
                update_date();                              //更新日期
            }
//            if(Calendar.Sec == 0)         //在每分钟的第0秒，或费率无效进行时段判断
//            {
//
//            }
//            else if(Calendar.Sec == 3)            //在每分钟的第5秒启动当前秒索引记录
//            {
//
//            }
//            record_power_outages_event(); //记录一条上下电事件
//            system_timer();
#ifdef ZIGBEE_TEST
            if(0 != GangMarkTime)                       //有接到/?
            {
                tmp = os_time - GangMarkTime;
                tmp /= (1000 / 10);//SYS_TICK);         //Tick用宏是个好习惯
                if((0 != tmp) && ((tmp % 2) == 0))
                {
                    send_identity_frame();              //进行一次报告
                }
                if(tmp >= 8)                            //有把握说包过3次了
                {
                    GangMarkTime = 0;                   //以后不再报送了
                }
            }
#endif
        }
        else if(msec_cnt == 1)//每秒的 第1个100ms去读
        {
#if CODE_VER_TYPE==H_METER_PRJ				//配合高压表
            sync_sec_data();
#endif
        }
        else if(msec_cnt == 3)//每秒的 第3个100ms去读
        {
#if CODE_VER_TYPE!=WLS_CUR_PRJ			//配合无线采集器
            sync_minute_data(Calendar.Sec);//每分钟的每秒同步一项
#endif
        }
        else if(msec_cnt == 4)//每秒的 第4个100ms去读
        {
#if CODE_VER_TYPE==H_METER_PRJ				//配合高压表
            if(Calendar.Min == 1)//每小时的第一分钟
            {
                sync_hour_data(Calendar.Sec);//每小时的第一分钟的后30秒同步上一结算日数据
            }
#endif
        }

        msec_cnt++;
        if(msec_cnt >= 10)
        {
            msec_cnt = 0;
        }
//azh 171011 用统一的函数去控制灯
//#ifdef ZIGBEE_TEST
//        zigbee_led_flash();
//#endif

//#ifdef GPRS_MODULE_PRJ
		local_led_flash();
//#endif
        program_package_code();//azh 111223 在升级状态查询
        soft_reset_function();//查询升级完软复位

        debug_handle_interval();

        MeterCopy_Proc_Check();//130125

        loop_cnt++;
        os_dly_wait(OS_DLY_100MS);
    }
}

/******************************************************************************
 Function name:  void reflash_exrom_data(U8 sec_tag)
 Author       :  rosoon
 Description  :  定时检查更新外存数据,保持内外存同步
 Input        :  sec_tag 时标，这里依秒为时标进行动作
 Return       :  无
******************************************************************************/
#if CODE_VER_TYPE==H_METER_PRJ				//配合高压表
void reflash_exrom_data (U8 sec_tag)
{
    U8 *buf = NULL;
    U8 mem_type;
    U16 len;
    U32 addr_base,offset,addr;

    if(0 == (Min_Cnt % DATA_SAVE_TIME))//每间隔DATA_SAVE_TIME分钟处理一次
    {
        buf = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR25);
        if(NULL == buf)
        {
//          memcpy(gusCurMemErrFunName, "reflash_", 8);
            setmemgetnullid(GET_MEM_ERR25, gusCurMemErrFunName);
            return;           //申请内存失败
        }
        switch(sec_tag)
        {
        case 10://当前组合有功总及各费率电能处理
            addr_base = ENG_STORE_START_ADDR;
            offset = 0 * TOTAL_PHASE_ENERGY_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_ENERGY_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC比对:若相同,数据转存
            {
                file_write(addr, buf, len);
            }
            break;
        case 11://当前正向有功总及各费率电能处理
            addr_base = ENG_STORE_START_ADDR;
            offset = 1 * TOTAL_PHASE_ENERGY_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_ENERGY_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC比对:若相同,数据转存
            {
                file_write(addr, buf, len);
            }
            break;
        case 12://当前反向有功总及各费率电能处理
            addr_base = ENG_STORE_START_ADDR;
            offset = 2 * TOTAL_PHASE_ENERGY_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_ENERGY_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC比对:若相同,数据转存
            {
                file_write(addr, buf, len);
            }
            break;
        case 13://当前组合无功 1及各费率电能处理
            addr_base = ENG_STORE_START_ADDR;
            offset = 3 * TOTAL_PHASE_ENERGY_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_ENERGY_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC比对:若相同,数据转存
            {
                file_write(addr, buf, len);
            }
            break;
        case 14://当前组合无功 2及各费率电能处理
            addr_base = ENG_STORE_START_ADDR;
            offset = 4 * TOTAL_PHASE_ENERGY_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_ENERGY_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC比对:若相同,数据转存
            {
                file_write(addr, buf, len);
            }
            break;
        case 15://当前第一象限无功总及各费率电能处理
            addr_base = ENG_STORE_START_ADDR;
            offset = 5 * TOTAL_PHASE_ENERGY_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_ENERGY_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC比对:若相同,数据转存
            {
                file_write(addr, buf, len);
            }
            break;
        case 16://当前第二象限无功总及各费率电能处理
            addr_base = ENG_STORE_START_ADDR;
            offset = 6 * TOTAL_PHASE_ENERGY_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_ENERGY_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC比对:若相同,数据转存
            {
                file_write(addr, buf, len);
            }
            break;
        case 17://当前第三象限无功总及各费率电能处理
            addr_base = ENG_STORE_START_ADDR;
            offset = 7 * TOTAL_PHASE_ENERGY_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_ENERGY_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC比对:若相同,数据转存
            {
                file_write(addr, buf, len);
            }
            break;
        case 18://当前第四象限无功总及各费率电能处理
            addr_base = ENG_STORE_START_ADDR;
            offset = 8 * TOTAL_PHASE_ENERGY_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_ENERGY_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC比对:若相同,数据转存
            {
                file_write(addr, buf, len);
            }
            break;
        case 19://当前正向视在总及各费率电能处理
            addr_base = ENG_STORE_START_ADDR;
            offset = 9 * TOTAL_PHASE_ENERGY_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_ENERGY_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC比对:若相同,数据转存
            {
                file_write(addr, buf, len);
            }
            break;
        case 20://当前反向视在总及各费率电能处理
            addr_base = ENG_STORE_START_ADDR;
            offset = 10 * TOTAL_PHASE_ENERGY_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_ENERGY_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC比对:若相同,数据转存
            {
                file_write(addr, buf, len);
            }
            break;
        case 21://分相电能数据块处理
            addr_base = ENG_STORE_START_ADDR;
            offset = ENG_MERGED_PHASE_TOT_LEN;
            addr = addr_base + offset;
            len = ENG_SEPARATE_PHASE_TOT_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC比对:若相同,数据转存
            {
                file_write(addr, buf, len);
            }
            break;
        case 25://合、分相关联电能量数据块
            addr_base = ENG_STORE_START_ADDR;
            offset = ENG_MERGED_PHASE_TOT_LEN + ENG_SEPARATE_PHASE_TOT_LEN;
            addr = addr_base + offset;
            len = ENG_ALL_RELATOTP_TOT_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC比对:若相同,数据转存
            {
                file_write(addr, buf, len);
            }
            break;


        case 30://当前正向有功总最大需量及发生时间处理
            addr_base = DMD_STORE_START_ADDR;
            offset = 0 * TOTAL_PHASE_DEMAND_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_DEMAND_LEN;
            memcpy(buf,gDmdArray + offset, len);
//            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC比对:若相同,数据转存
//            {
                file_write(addr, buf, len);
//            }
            break;
        case 31://当前反向有功总最大需量及发生时间处理
            addr_base = DMD_STORE_START_ADDR;
            offset = 1 * TOTAL_PHASE_DEMAND_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_DEMAND_LEN;
            memcpy(buf,gDmdArray + offset, len);
//            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC比对:若相同,数据转存
//            {
                file_write(addr, buf, len);
//            }
            break;
        case 32://当前组合无功 1最大需量及发生时间处理
            addr_base = DMD_STORE_START_ADDR;
            offset = 2 * TOTAL_PHASE_DEMAND_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_DEMAND_LEN;
            memcpy(buf,gDmdArray + offset, len);
//            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC比对:若相同,数据转存
//            {
                file_write(addr, buf, len);
//            }
            break;
        case 33://当前组合无功 2最大需量及发生时间处理
            addr_base = DMD_STORE_START_ADDR;
            offset = 3 * TOTAL_PHASE_DEMAND_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_DEMAND_LEN;
            memcpy(buf,gDmdArray + offset, len);
//            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC比对:若相同,数据转存
//            {
                file_write(addr, buf, len);
//            }
            break;
        case 34://当前第一象限无功总最大需量及发生时间处理
            addr_base = DMD_STORE_START_ADDR;
            offset = 4 * TOTAL_PHASE_DEMAND_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_DEMAND_LEN;
            memcpy(buf,gDmdArray + offset, len);
//            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC比对:若相同,数据转存
//            {
                file_write(addr, buf, len);
//            }
            break;
        case 35://当前第二象限无功总最大需量及发生时间处理
            addr_base = DMD_STORE_START_ADDR;
            offset = 5 * TOTAL_PHASE_DEMAND_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_DEMAND_LEN;
            memcpy(buf,gDmdArray + offset, len);
//            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC比对:若相同,数据转存
//            {
                file_write(addr, buf, len);
//            }
            break;
        case 36://当前第三象限无功总最大需量及发生时间处理
            addr_base = DMD_STORE_START_ADDR;
            offset = 6 * TOTAL_PHASE_DEMAND_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_DEMAND_LEN;
            memcpy(buf,gDmdArray + offset, len);
//            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC比对:若相同,数据转存
//            {
                file_write(addr, buf, len);
//            }
            break;
        case 37://当前第四象限无功总最大需量及发生时间处理
            addr_base = DMD_STORE_START_ADDR;
            offset = 7 * TOTAL_PHASE_DEMAND_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_DEMAND_LEN;
            memcpy(buf,gDmdArray + offset, len);
//            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC比对:若相同,数据转存
//            {
                file_write(addr, buf, len);
//            }
            break;
        case 38://当前正向视在总最大需量及发生时间处理
            addr_base = DMD_STORE_START_ADDR;
            offset = 8 * TOTAL_PHASE_DEMAND_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_DEMAND_LEN;
            memcpy(buf,gDmdArray + offset, len);
//            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC比对:若相同,数据转存
//            {
                file_write(addr, buf, len);
//            }
            break;
        case 39://当前反向视在总最大需量及发生时间处理
            addr_base = DMD_STORE_START_ADDR;
            offset = 9 * TOTAL_PHASE_DEMAND_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_DEMAND_LEN;
            memcpy(buf,gDmdArray + offset, len);
//            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC比对:若相同,数据转存
//            {
                file_write(addr, buf, len);
//            }
            break;
        case 40://当前A相正、反向，组合无功1、2，四象限无功，正、反向视在最大需量及发生时间块处理

            break;
        case 45://当前C相正、反向，组合无功1、2，四象限无功，正、反向视在最大需量及发生时间块处理

            break;
        case 50://当前B相正、反向，组合无功1、2，四象限无功，正、反向视在最大需量及发生时间块处理

            break;
        default:

            break;
        }
        free_mem_block(buf);//释放内存块
    }
}
#endif
/******************************************************************************
 Function name:  void reflash_system_RTC (void)
 Author       :  snap.gao
 Description  :  用秒索引更新一下实时时钟
 Input        :  无
 Return       :  无
 *****************************************************************************/
/*
void reflash_system_RTC (void)
{
  second_index_2_RTC(Second_Index, &Calendar);        //用秒索引更新一下实时时钟
  update_day_of_week();
  Current_Date = char_2_BCD(Calendar.Date) | (char_2_BCD(Calendar.Month)<<8) | (int_2_BCD(Calendar.Year)<<16);
  Current_Time = char_2_BCD(Calendar.Second) | (char_2_BCD(Calendar.Minute)<<8) | (char_2_BCD(Calendar.Hour)<<16) | (char_2_BCD(Calendar.Day)<<24);
}
*/
/******************************************************************************
 Function name:  void system_timer (void)
 Author       :  snap.gao
 Description  :  要求精确的每秒钟调用一次，产生系统软时钟，复位后从‘0’开始运行
         相关寄存器：Calendar.Year
               Calendar.Month
               Calendar.Date
               Calendar.Hour
               Calendar.Minute
               Calendar.Second
               Calendar.Day
               Second_Index
 Input        :  无
 Return       :  无
 *****************************************************************************/
/*
void system_timer (void)
{
  tsk_lock();
  Second_Index++;
  if(software_rtc_run() == 1)     //调整时间，与秒索引独立增加
  {
    reflash_system_RTC();       //跨天时用秒索引更新一下实时时钟
  }
  else
  {
    Current_Time = char_2_BCD(Calendar.Second) | (char_2_BCD(Calendar.Minute)<<8) | (char_2_BCD(Calendar.Hour)<<16) | (char_2_BCD(Calendar.Day)<<24);
  }
  tsk_unlock();
}
*/

/******************************************************************************
 Function name:  U32 get_max_second_index (void)
 Author       :  snap.gao
 Description  :  在记录的秒索引缓冲中寻找一个最大值
 Input        :  无
 Return       :  找到的秒索引最大值，错误时返回‘0’
 *****************************************************************************/
/*
U32 get_max_second_index (void)
{
  U32 *si, max_si;
  U8  *buf, i;

  buf = _alloc_box(Mem_64_Byte);
  if(buf == NULL)
  {
    return 0;
  }

  max_si = 0;
  read_data(SECOND_INDEX_RECORD, buf, SECOND_INDEX_RECORD_LEN);       //读取秒索引记录缓冲
  for(i = 0; i < SECOND_INDEX_RECORD_LEN / 4; i++)
  {
    si = (U32*)&buf[i * 4];
    if(*si == 0xffffffff)
    {
      continue;
    }
    max_si = max(*si, max_si);
  }
  _free_box(Mem_64_Byte, buf);            //释放内存块
  return max_si;
}
*/
/******************************************************************************
 Function name:  void record_power_outages_event (void)
 Author       :  snap.gao
 Description  :  记录一条上下电事件，只记录POR和BOD事件，上电事件在秒索引有效时进行
 Input        :  无
 Return       :  无
 *****************************************************************************/
/*
void record_power_outages_event (void)
{
  U16 point;
  U8  buf[POWER_OUTAGES_RECORD_LEN];
  U32 si;

  if((Reset_Source_Copy == MCU_RST_POR) || (Reset_Source_Copy == MCU_RST_BOD))        //判断是否记录下电事件
  {
    read_data(POWER_OUTAGES_POINT, (U8*)&point, POWER_OUTAGES_POINT_LEN);           //读取上下电记录的当前指针
    if((point & ~0x8000) >= POWER_OUTAGES_NUM)
    {
      point = 0;                              //指针错，纠正为‘0’
    }
    si = get_max_second_index();
    if(si != 0)
    {
      mem_copy(buf, (U8*)&si, 4);
      buf[4] = 0;                             //下电事件标识
      write_data(POWER_OUTAGES + (point & 0x7fff) * POWER_OUTAGES_RECORD_LEN, buf, POWER_OUTAGES_RECORD_LEN);     //记录一条数据
      point = get_next_record_point(POWER_OUTAGES_NUM, point);
      write_data(POWER_OUTAGES_POINT, (U8*)&point, POWER_OUTAGES_POINT_LEN);      //刷新上下电记录的当前指针
    }
    Reset_Source_Copy = 0xff;                   //标识已经记录了下电，后续需要记录上电
  }
  else if(Reset_Source_Copy != 0xff)
  {
    Reset_Source_Copy = 0;
  }

  if(Second_Index <= VALID_SECOND_INDEX)          //判断当前时间是否有效
  {
    Power_Up_Record_Delay = Second_Index;
    return;
  }

  if(Reset_Source_Copy == 0xff)                   //判断是否记录上电事件
  {
    read_data(POWER_OUTAGES_POINT, (U8*)&point, POWER_OUTAGES_POINT_LEN);           //读取上下电记录的当前指针
    if((point & ~0x8000) >= POWER_OUTAGES_NUM)
    {
      point = 0;                              //指针错，纠正为‘0’
    }
    Power_Up_Record_Delay = Second_Index - Power_Up_Record_Delay;
    mem_copy(buf, (U8*)&Power_Up_Record_Delay, 4);
    buf[4] = Reset_Source;                      //上电事件标识，POR或BOD
    Reset_Source_Copy = 0;
    write_data(POWER_OUTAGES + (point & 0x7fff) * POWER_OUTAGES_RECORD_LEN, buf, POWER_OUTAGES_RECORD_LEN);     //记录一条数据
    point = get_next_record_point(POWER_OUTAGES_NUM, point);
    write_data(POWER_OUTAGES_POINT, (U8*)&point, POWER_OUTAGES_POINT_LEN);          //刷新上下电记录的当前指针
  }
}
*/
/******************************************************************************
              End Of File
 *****************************************************************************/

/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : event.c
 Description    : 事件记录相关的一些子程序
 Related Files  : event.h
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2010-09-27    wwllzz          the original version
******************************************************************************/

#include "..\inc\global_config.h"
#include    "..\src\bsp\event_define_d.h"	            //有关事件宏的定义

//-------------------------------- 常数定义 -----------------------------------

//------------------------------ 私有变量声明 ---------------------------------

//------------------------------ 私有函数声明 ---------------------------------
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不带CPU_GPRS模块





/******************************************************************************
函数: void record_energy_zero_event(U8 buf[])
入口: buf[]-足够106字节的缓冲区, 操作者代码存在buf[6..9]
出口: 无
概述: 增加电表清零总次数, 填写一个记录, 需在清零前调用
******************************************************************************/
//void record_energy_zero_event (U8 buf[])
//{
//    U32 tmp;
//    U32 i;
//    U8 k[4];
//
//    tmp = 0;                                            //先读入原次数
//    file_read(ENERGY_ZERO_COUNT_ADDR, (U8 *)(&tmp), 3);
//    if(bcd_to_bytes((U8 *)(&tmp), 3) == 0)              //转化为hex, 看数据是否有效
//    {
//        tmp = 0;
//    }
////    get_on_chip_time(buf);                              //组织记录, 发生时刻
//    for(i = 0; i < 6; i++)                              //系列电量, 没有分费率的, 没有组合能量和视在能量
//    {
//        if(i < 2)
//        {
//            file_read(ENG_POSP_00010000_00_ADDR + i * ENG_POSP_00010000_TOT_LEN, k, 4);
//        }
//        else
//        {
//            file_read(ENG_POSP_00010000_00_ADDR + (i + 2) * ENG_POSP_00010000_TOT_LEN, k, 4);
//        }
//        energy_to_format_data(k, buf + 10 + i * 4, 1);
//    }
//    for(i = 0; i < 6; i++)                              //A相
//    {
//        if(i < 2)
//        {
//            file_read(ENG_APOSP_00150000_00_ADDR + i * 4, k, 4);
//        }
//        else
//        {
//            file_read(ENG_APOSP_00150000_00_ADDR + 8 + i * 4, k, 4);
//        }
//        energy_to_format_data(k, buf + 10 + 24 + i * 4, 1);
//    }
//    fill_char(buf + 10 + 24 * 2, 24, GW645_2007_CHAR);  //B相
//    for(i = 0; i < 6; i++)                              //C相
//    {
//        if(i < 2)
//        {
//            file_read(ENG_CPOSP_003D0000_00_ADDR + i * 4, k, 4);
//        }
//        else
//        {
//            file_read(ENG_CPOSP_003D0000_00_ADDR + 8 + i * 4, k, 4);
//        }
//        energy_to_format_data(k, buf + 10 + 24 * 3 + i * 4, 1);
//    }
//    file_write(ENERGY_ZERO_RECORD_ADDR + (tmp & 0xf) * ENERGY_ZERO_RECORD_LEN, buf, ENERGY_ZERO_RECORD_LEN);   //写入记录
//    tmp = bcd_to_bytes((U8 *)(&tmp), 3);                //总次数++
//    tmp++;
//    bytes_to_bcd(tmp, (U8 *)(&tmp), 6);
//    file_write(ENERGY_ZERO_COUNT_ADDR, (U8 *)(&tmp), 3);
//}

/******************************************************************************
函数: void record_demand_zero_event(U8 buf[])
入口: buf[]-足够202字节的缓冲区, 操作者代码存在buf[6..9]
出口: 无
概述: 增加需量清零总次数, 填写一个记录, 需在执行需量清零前调用
******************************************************************************/
//void record_demand_zero_event (U8 buf[])
//{
//    U32 tmp;
//    U32 i;
//
//    tmp = 0;                                            //次数
//    file_read(EVET_DMDCLR_03300200_00_ADDR, (U8 *)(&tmp), 3);
//    if(bcd_to_bytes((U8 *)(&tmp), 3) == 0)              //次数数据是否有效
//    {
//        tmp = 0;
//    }
////    get_on_chip_time(buf);                              //组织记录的发生时刻
//    for(i = 0; i < 6; i++)                              //没有分费率的, 没有组合和视在
//    {
//        if(i < 2)
//        {
//            file_read(DMD_POSP_01010000_00_ADDR + i * TOTAL_PHASE_DEMAND_LEN, buf + 10 + i * 8, 8);
//        }
//        else
//        {
//            file_read(DMD_POSP_01010000_00_ADDR + (i + 2) * TOTAL_PHASE_DEMAND_LEN, buf + 10 + i * 8, 8);
//        }
//    }
//    for(i = 0; i < 6; i++)                              //A相
//    {
//        if(i < 2)
//        {
//            file_read(DMD_APOSP_01150000_00_ADDR + i * 8, buf + 10 + 48 + i * 8, 8);
//        }
//        else
//        {
//            file_read(DMD_APOSP_01150000_00_ADDR + (i + 2) * 8, buf + 10 + 48 + i * 8, 8);
//        }
//    }
//    fill_char(buf + 10 + 48 * 2, 48, GW645_2007_CHAR);  //B相
//    for(i = 0; i < 6; i++)                              //C相
//    {
//        if(i < 2)
//        {
//            file_read(DMD_CPOSP_013D0000_00_ADDR + i * 8, buf + 10 + 48 * 3 + i * 8, 8);
//        }
//        else
//        {
//            file_read(DMD_CPOSP_013D0000_00_ADDR + (i + 2) * 8, buf + 10 + 48 * 3 + i * 8, 8);
//        }
//    }
//    file_write(EVET_DMDCLR_03300201_00_OFFSET_ADDR + (tmp & 0xf) * EVET_DMDCLR_03300201_00_OFFSET_BLOCKLEN, buf, EVET_DMDCLR_03300201_00_OFFSET_BLOCKLEN);
//    tmp = bcd_to_bytes((U8 *)(&tmp), 3);                //总次数++
//    tmp++;
//    bytes_to_bcd(tmp, (U8 *)(&tmp), 6);
//    file_write(EVET_DMDCLR_03300200_00_ADDR, (U8 *)(&tmp), 3);
//}

/******************************************************************************
函数: void record_event_zero_event(U8 buf[])
入口: buf[]-足够14字节的缓冲区, 操作者代码存在buf[6..9], 标识码在buf[10..13]
出口: 无
概述: 增加清事件次数, 填写一个记录, 在完成清事件后调用
******************************************************************************/
//void record_event_zero_event (U8 buf[])
//{
//    U32 tmp;
//
//    tmp = 0;
//    file_read(EVENT_ZERO_COUNT_ADDR, (U8 *)(&tmp), 3);
//    if(bcd_to_bytes((U8 *)(&tmp), 3) == 0)              //次数数据是否有效
//    {
//        tmp = 0;
//    }
////    get_on_chip_time(buf);                              //组织记录的发生时刻
//    file_write(EVENT_ZERO_RECORD_ADDR + (tmp & 0xf) * EVENT_ZERO_RECORD_LEN, buf, EVENT_ZERO_RECORD_LEN);
//    tmp = bcd_to_bytes((U8 *)(&tmp), 3);                //总次数++
//    tmp++;
//    bytes_to_bcd(tmp, (U8 *)(&tmp), 6);
//    file_write(EVENT_ZERO_COUNT_ADDR, (U8 *)(&tmp), 3);
//}

/******************************************************************************
函数: void record_check_time_event(U8 buf[])
入口: buf[]-足够16字节的缓冲区, 操作者代码保存在buf[0..3], 校时前时间buf[4..9]
出口: 无
概述: 增加校时次数, 添一个校时记录, 需在校时完成后调用
******************************************************************************/
//void record_check_time_event (U8 buf[])
//{
//    U32 tmp;
//
//    tmp = 0;
//    file_read(CHECK_TIME_COUNT_ADDR, (U8 *)(&tmp), 3);
//    if(bcd_to_bytes((U8 *)(&tmp), 3) == 0)              //次数数据是否有效
//    {
//        tmp = 0;
//    }
////    get_on_chip_time(buf + 10);                         //校时后时刻
//    file_write(CHECK_TIME_RECORD_ADDR + (tmp & 0xf) * CHECK_TIME_RECORD_LEN, buf, CHECK_TIME_RECORD_LEN);
//    tmp = bcd_to_bytes((U8 *)(&tmp), 3);                //总次数++
//    tmp++;
//    bytes_to_bcd(tmp, (U8 *)(&tmp), 6);
//    file_write(CHECK_TIME_COUNT_ADDR, (U8 *)(&tmp), 3);
//}

/******************************************************************************
函数: void record_table_program_event(U8 buf[])
入口: buf[]-足够346字节的缓冲区, 操作者代码在buf[6..9]
出口: 无
概述: 增加时段表编程次数, 添一个记录, 需要在修改时段表前调用
******************************************************************************/
void record_table_program_event (U8 buf[])
{
//    U32 tmp;
//
//    tmp = 0;
//    file_read(TABLE_PROGRAM_COUNT_ADDR, (U8 *)(&tmp), 3);
//    if(bcd_to_bytes((U8 *)(&tmp), 3) == 0)              //次数数据是否有效
//    {
//        tmp = 0;
//    }
////    get_on_chip_time(buf);                              //发生时刻
//    file_read(FEE_TABLE_ONE1_ADDR, buf + 10, FEE_TABLE_ONE1_LEN * FEE_TABLE_NUM);   // 时刻+操作者代码+第一套8个日时段表
//    file_write(TABLE_PROGRAM_RECORD_ADDR + (tmp & 0xf) * TABLE_PROGRAM_RECORD_LEN, buf, 10 + FEE_TABLE_ONE1_LEN * FEE_TABLE_NUM);
//    file_read(FEE_TABLE_TWO1_ADDR, buf, FEE_TABLE_ONE1_LEN * FEE_TABLE_NUM);        // 第二套8个日时段表, 放在buf处
//    file_write(TABLE_PROGRAM_RECORD_ADDR + 10 + FEE_TABLE_ONE1_LEN * FEE_TABLE_NUM + (tmp & 0xf) * TABLE_PROGRAM_RECORD_LEN, buf, FEE_TABLE_ONE1_LEN * FEE_TABLE_NUM);
//    tmp = bcd_to_bytes((U8 *)(&tmp), 3);                //总次数++
//    tmp++;
//    bytes_to_bcd(tmp, (U8 *)(&tmp), 6);
//    file_write(TABLE_PROGRAM_COUNT_ADDR, (U8 *)(&tmp), 3);
}

/******************************************************************************
函数: void record_zone_program_event(U8 buf[])
入口: buf[]-足够94字节的缓冲区, 操作者代码在buf[6..9]
出口: 无
概述: 增加时区表编程次数, 添一个记录, 需要在修改时区表前调用
******************************************************************************/
void record_zone_program_event (U8 buf[])
{
//    U32 tmp;
//
//    tmp = 0;
//    file_read(ZONE_PROGRAM_COUNT_ADDR, (U8 *)(&tmp), 3);
//    if(bcd_to_bytes((U8 *)(&tmp), 3) == 0)              //次数数据是否有效
//    {
//        tmp = 0;
//    }
////    get_on_chip_time(buf);                              //发生时刻
//    file_read(ZONE_TABLE_ONE_ADDR, buf + 10, ZONE_TABLE_ONE_LEN);
//    file_read(ZONE_TABLE_TWO_ADDR, buf + 10 + ZONE_TABLE_ONE_LEN, ZONE_TABLE_ONE_LEN);
//    file_write(ZONE_PROGRAM_RECORD_ADDR + (tmp & 0xf) * ZONE_PROGRAM_RECORD_LEN, buf, ZONE_PROGRAM_RECORD_LEN);
//    tmp = bcd_to_bytes((U8 *)(&tmp), 3);                //总次数++
//    tmp++;
//    bytes_to_bcd(tmp, (U8 *)(&tmp), 6);
//    file_write(ZONE_PROGRAM_COUNT_ADDR, (U8 *)(&tmp), 3);
}

/******************************************************************************
函数: void record_weekday_program_event(U8 buf[], U32 count_addr)
入口: buf[]-足够11字节的缓冲区, 操作者代码在buf[6..9], 数据在buf[10]
    count_addr-区分不同调用, 具体描述如下:
    WEEKDAY_PROGRAM_COUNT_ADDR 增加周休日编程次数, 添一个记录, 需要在修改周休日配置前调用
    COMB_Y_PROGRAM_COUNT_ADDR 增加有功组合方式编程次数, 添一个记录, 需要在修改有功组合方式配置前调用
    COMB_1_PROGRAM_COUNT_ADDR 增加无功组合方式1编程次数, 添一个记录, 需要在修改无功组合方式1配置前调用
    COMB_2_PROGRAM_COUNT_ADDR 增加无功组合方式2编程次数, 添一个记录, 需要在修改无功组合方式2配置前调用
出口: 无
概述: 这些事件记录非常小, 处理方法类似
******************************************************************************/
//void record_weekday_program_event (U8 buf[], U32 count_addr)
//{
//    U32 tmp;
//
//    tmp = 0;
//    file_read(count_addr, (U8 *)(&tmp), 3);
//    if(bcd_to_bytes((U8 *)(&tmp), 3) == 0)              //次数数据是否有效
//    {
//        tmp = 0;
//    }
////    get_on_chip_time(buf);                              //发生时刻, 数据已填好
//    file_write(count_addr + 3 + (tmp & 0xf) * 11, buf, 11);
//    tmp = bcd_to_bytes((U8 *)(&tmp), 3);                //总次数++
//    tmp++;
//    bytes_to_bcd(tmp, (U8 *)(&tmp), 6);
//    file_write(count_addr, (U8 *)(&tmp), 3);
//}

/******************************************************************************
函数: void record_holiday_program_event(U8 buf[])
入口: buf[]-足够264字节的缓冲区, 操作者代码在buf[6..9]
出口: 无
概述: 增加节假日编程次数, 添一个记录, 需要在修改节假日配置前调用
******************************************************************************/
void record_holiday_program_event (U8 buf[])
{
//    U32 tmp;
//
//    tmp = 0;
//    file_read(HOLIDAY_PROGRAM_COUNT_ADDR, (U8 *)(&tmp), 3);
//    if(bcd_to_bytes((U8 *)(&tmp), 3) == 0)              //次数数据是否有效
//    {
//        tmp = 0;
//    }
////    get_on_chip_time(buf);                              //发生时刻
//    file_read(HOLIDAY_TABLE_ADDR, buf + 10, 254);
//    file_write(HOLIDAY_PROGRAM_RECORD_ADDR + (tmp & 0xf) * HOLIDAY_PROGRAM_RECORD_LEN, buf, 10 + 254);
//    file_read(HOLIDAY_TABLE_ADDR + 254, buf + 10, 254);
//    file_write(HOLIDAY_PROGRAM_RECORD_ADDR + 10 + 254 + (tmp & 0xf) * HOLIDAY_PROGRAM_RECORD_LEN, buf + 10, 254);
//    file_read(HOLIDAY_TABLE_ADDR + 254 * 2, buf + 10, 254);
//    file_write(HOLIDAY_PROGRAM_RECORD_ADDR + 10 + 254 * 2 + (tmp & 0xf) * HOLIDAY_PROGRAM_RECORD_LEN, buf + 10, 254);
//    file_read(HOLIDAY_TABLE_ADDR + 254 * 3, buf + 10, 254);
//    file_write(HOLIDAY_PROGRAM_RECORD_ADDR + 10 + 254 * 3 + (tmp & 0xf) * HOLIDAY_PROGRAM_RECORD_LEN, buf + 10, 254);
//    tmp = bcd_to_bytes((U8 *)(&tmp), 3);                //总次数++
//    tmp++;
//    bytes_to_bcd(tmp, (U8 *)(&tmp), 6);
//    file_write(HOLIDAY_PROGRAM_COUNT_ADDR, (U8 *)(&tmp), 3);
}

/******************************************************************************
函数: void record_account_day_program_event(U8 buf[])
入口: buf[]-足够16字节的缓冲区, 操作者代码在buf[6..9]
出口: 无
概述: 增加结算日编程次数, 填一个结算日编程记录
******************************************************************************/
void record_account_day_program_event (U8 buf[])
{
//    U32 tmp;
//
//    tmp = 0;
//    file_read(ACCOUNT_DAY_PROGRAM_COUNT_ADDR, (U8 *)(&tmp), 3);
//    if(bcd_to_bytes((U8 *)(&tmp), 3) == 0)              //次数数据是否有效
//    {
//        tmp = 0;
//    }
////    get_on_chip_time(buf);                              //发生时刻, 数据已填好
//    file_read(PAR_FIRACCNT_04000B01_00_ADDR, buf + 10, 6);
//    file_write(ACCOUNT_DAY_PROGRAM_COUNT_ADDR + 3 + (tmp & 0xf) * 16, buf, 16);
//    tmp = bcd_to_bytes((U8 *)(&tmp), 3);                //总次数++
//    tmp++;
//    bytes_to_bcd(tmp, (U8 *)(&tmp), 6);
//    file_write(ACCOUNT_DAY_PROGRAM_COUNT_ADDR, (U8 *)(&tmp), 3);
}

/******************************************************************************
函数: void restore_second(void)
入口: 无, 全局Calendar
出口: 无
概述: 将当前包含秒的时刻保存到FRAM, BCD码
******************************************************************************/
//void restore_second (void)
//{
//    U8 buf[6];
//
////    get_on_chip_time(buf);
//    file_write(BY_SECOND_RESTORE, buf, 6);              //写到FRAM
//}

/******************************************************************************
函数: void save_down_time(void)
入口: 无
出口: 无
概述: 保存掉电开始和结束记录, 忽略开始和结束在同一时刻的记录, 处理复位原因
    软复位-在RSID中不留下标识, 程序区分出来, 不记录上/掉电事件
    下面的复位暂时没作具体分析和记录, 只简单的去掉
    POR-上电复位
    EXTR-RESET引脚被拉低的复位, 可能是外部看门狗复位
    WDTR-芯片内部看门狗启动的复位
    BODR-主电源降低造成的复位
******************************************************************************/
//void save_down_time (void)
//{
//    U8 buf[12];
//    U32 i;
//
//    if(RSID == 0)                                       //软件复位
//    {
//        return;
//    }
//    RSID = 0xf;                                         //暂时清掉其它复位原因
//    file_read(BY_SECOND_RESTORE, buf, 6);
////    get_on_chip_time(buf + 6);
//    for(i = 1; i < 6; i++)
//    {
//        if(buf[i] != buf[i + 6])
//        {
//            break;
//        }
//    }
//    if(i != 6)                                          //开始和结束不在同一秒钟(时刻不同)
//    {
//        i = 0;
//        file_read(EVET_POWDNTIME_03110000_00_ADDR, (U8 *)(&i), 3);//读入次数
//        if(bcd_to_bytes((U8 *)(&i), 3) == 0)            //次数数据是否有效
//        {
//            i = 0;
//        }
//        file_write(EVET_POWDN_03110001_00_OFFSET_ADDR + (i & 0xf) * 12, buf, 12);
//        i = bcd_to_bytes((U8 *)(&i), 3);                //次数增加
//        i++;
//        bytes_to_bcd(i, (U8 *)(&i), 6);
//        file_write(EVET_POWDNTIME_03110000_00_ADDR, (U8 *)(&i), 3);
//    }
//}

/******************************************************************************
函数: void record_event_open_cover(void)
入口: 无
出口: 无
概述: 无
******************************************************************************/
//void record_event_open_cover (void)
//{
//    U8 oc_flg;
//    U8 i,j,tmp_buf[7]={0xFF,0xFF,0xFF};
//    U32 cnt1,cnt2,addr;
//
///*  之前必须将oc_flg & cnt1 & cnt2外存映射区刷成0xff
//    file_write(EEPROM_GFLG_ADDR, tmp_buf, 1);
//    file_write(EVET_OPNCOVER_03300D00_00_ADDR, tmp_buf, 3);
//    file_write(EVET_OPNTMNBX_03300E00_00_ADDR, tmp_buf, 3);
//*/
//    file_read(EEPROM_GFLG_ADDR, tmp_buf, 1);
//    file_read(EVET_OPNCOVER_03300D00_00_ADDR, tmp_buf + 1, 3);
//    file_read(EVET_OPNTMNBX_03300E00_00_ADDR, tmp_buf + 4, 3);
//    oc_flg = tmp_buf[0];
//    cnt1 = byte_merge1(tmp_buf + 1, EVET_OPNCOVER_03300D00_00_LEN);
//    cnt2 = byte_merge1(tmp_buf + 4, EVET_OPNTMNBX_03300E00_00_LEN);
//    if((oc_flg == 0xFF) && (cnt1 == 0xFFFFFF) && (cnt2 == 0xFFFFFF))
//    {
//        if((0 == (PORT0_IS_HIGH(P0_KEYG))) && (0 == (PORT0_IS_HIGH(P0_KEYD))))
//        {
//            tmp_buf[0] = tmp_buf[1] = tmp_buf[2] = 0;
//            file_write(EEPROM_GFLG_ADDR, tmp_buf, 1);               //oc_flg映射的外存清零
//            file_write(EVET_OPNCOVER_03300D00_00_ADDR, tmp_buf, 3); //开上盖cnt1映射的外存清零
//            file_write(EVET_OPNTMNBX_03300E00_00_ADDR, tmp_buf, 3); //开端钮盒cnt2映射的外存清零
//        }
//        else
//        {
//            return;
//        }
//    }
//
//    file_read(EEPROM_GFLG_ADDR, &oc_flg, 1);  //读取1字节上次开关表盖及端钮盒状态信息
//
//    SET_PORT0_IN(P0_KEYG);                    //配置p0.20为输入口[开上盖检测按钮]
//    if((!(oc_flg & OC_COVER_FLG)) && (PORT0_IS_HIGH(P0_KEYG)))  //开上盖
//    {
//        oc_flg |= OC_COVER_FLG;                 //标识为开盖
//        file_write(EEPROM_GFLG_ADDR, &oc_flg, 1);                   //保存本次开盖标识
////        ALARM_LED_ON();     //DBG
//        //开盖(发生时刻)记录处理
//        file_read(EVET_OPNCOVER_03300D00_00_ADDR, tmp_buf, EVET_OPNCOVER_03300D00_00_LEN);//读取三字节总次数
//        cnt1 = byte_merge1(tmp_buf, EVET_OPNCOVER_03300D00_00_LEN);//开盖总次数
//        if(cnt1 == 0xFFFFFF)
//        {
//            cnt1 = 0x000001;                     //记为新一轮的第一次
//        }
//        else
//        {
//            cnt1++;
//        }
//        for(i = 0; i < 3; i++)
//        {
//            tmp_buf[i] = (cnt1 >> (i * 8));
//        }
//        file_write(EVET_OPNCOVER_03300D00_00_ADDR, tmp_buf, EVET_OPNCOVER_03300D00_00_LEN);//保存总次数(即次数偏移指针)
//        cnt1 %= EVET_OPNCOVER_03300D01_00_OFFSET_MAXNUM;//求出当前开盖相对(10次)次数
//        addr = cnt1 * EVET_OPNCOVER_03300D01_00_OFFSET_BLOCKLEN + EVET_OPNCOVER_03300D01_00_HEAD_ADDR;//求出本次开盖数据保存起始地址
//        get_software_rtc(tmp_buf);              //获取开盖发生时刻
//		convert_to_bcd(tmp_buf, 7);//azh
//		for(i = 0; i < 3; i++)
//        {
//            tmp_buf[i + 3] = tmp_buf[i + 4];    //删除周次数据并将日期前移
//        }
//        file_write(addr, tmp_buf, 6);           //保存开盖发生时刻
//        //记录此时电量数据集:正、反向有功总电能、四象限无功总电能
//        addr += 12;                             //求出开盖时电量数据存储偏移地址
//        for(i = 0; i < 6; i++)
//        {
//            j = (i > 1? 3 : 1);
//            file_write(addr + i * 4, (U8 *)&gEngArray[6 * (i + j)], ENERGY_LEN);
//        }
////        file_write(addr, gEngArray + 6 * 1, ENERGY_LEN);       //正向有功总电能
////        file_write(addr + 4, gEngArray + 6 * 2, ENERGY_LEN);   //反向有功总电能
////        file_write(addr + 8, gEngArray + 6 * 5, ENERGY_LEN);   //第一象限无功总电能
////        file_write(addr + 12, gEngArray + 6 * 6, ENERGY_LEN);   //第二象限无功总电能
////        file_write(addr + 16, gEngArray + 6 * 7, ENERGY_LEN);   //第三象限无功总电能
////        file_write(addr + 20, gEngArray + 6 * 8, ENERGY_LEN);   //第四象限无功总电能
//    }
//    else if((oc_flg & OC_COVER_FLG) && (!PORT0_IS_HIGH(P0_KEYG)))    //关上盖
//    {
//        oc_flg &= ~OC_COVER_FLG;            //标识为关盖
//        file_write(EEPROM_GFLG_ADDR, &oc_flg, 1);                   //保存本次关盖标识
//        ALARM_LED_OFF();     //DBG
//        //关盖(结束时刻)记录处理
//        file_read(EVET_OPNCOVER_03300D00_00_ADDR, tmp_buf, EVET_OPNCOVER_03300D00_00_LEN);//读取三字节总次数
//        cnt1 = byte_merge1(tmp_buf, EVET_OPNCOVER_03300D00_00_LEN);//开盖总次数
//
//        cnt1 %= EVET_OPNCOVER_03300D01_00_OFFSET_MAXNUM;//求出当前开盖相对(10次)次数
//        addr = cnt1 * EVET_OPNCOVER_03300D01_00_OFFSET_BLOCKLEN + EVET_OPNCOVER_03300D01_00_HEAD_ADDR;//求出本次开盖数据保存起始地址
//        get_software_rtc(tmp_buf);              //获取开盖发生时刻
//		convert_to_bcd(tmp_buf, 7);//azh
//		for(i = 0; i < 3; i++)
//        {
//            tmp_buf[i + 3] = tmp_buf[i + 4];    //删除周次数据并将日期前移
//        }
//        file_write(addr + 6, tmp_buf, 6);       //保存开盖结束时刻
//        //记录此时电量数据集:正、反向有功总电能、四象限无功总电能
//        addr += (12 + 4 * 6);                   //求出闭盖时电量数据存储偏移地址
//        for(i = 0; i < 6; i++)
//        {
//            j = (i > 1? 3 : 1);
//            file_write(addr + i * 4, (U8 *)&gEngArray[6 * (i + j)], ENERGY_LEN);
//        }
//    }
//
//    SET_PORT0_IN(P0_KEYD);                    //配置p0.21为输入口[开端钮盒检测按钮]
//    if((!(oc_flg & OC_TMNBX_FLG)) && (PORT0_IS_HIGH(P0_KEYD)))  //开端钮盒
//    {
//        oc_flg |= OC_TMNBX_FLG;                 //标识为开端钮盒
//        file_write(EEPROM_GFLG_ADDR, &oc_flg, 1);                   //保存本次开端钮盒标识
////        ALARM_LED_ON();     //DBG
//        //开端钮盒(发生时刻)记录处理
//        file_read(EVET_OPNTMNBX_03300E00_00_ADDR, tmp_buf, EVET_OPNTMNBX_03300E00_00_LEN);//读取三字节总次数
//        cnt2 = byte_merge1(tmp_buf, EVET_OPNTMNBX_03300E00_00_LEN);//开端钮盒总次数
//        if(cnt2 == 0xFFFFFF)
//        {
//            cnt2 = 0x000001;                     //记为新一轮的第一次
//        }
//        else
//        {
//            cnt2++;
//        }
//        for(i = 0; i < 3; i++)
//        {
//            tmp_buf[i] = (cnt2 >> (i * 8));
//        }
//        file_write(EVET_OPNTMNBX_03300E00_00_ADDR, tmp_buf, EVET_OPNTMNBX_03300E00_00_LEN);//保存总次数(即次数偏移指针)
//        cnt2 %= EVET_OPNTMNBX_03300E01_00_OFFSET_MAXNUM;//求出当前开端钮盒相对(10次)次数
//        addr = cnt2 * EVET_OPNTMNBX_03300E01_00_OFFSET_BLOCKLEN + EVET_OPNTMNBX_03300E01_00_HEAD_ADDR;//求出本次开端钮盒数据保存起始地址
//        get_software_rtc(tmp_buf);              //获取开盖发生时刻
//		convert_to_bcd(tmp_buf, 7);//azh
//		for(i = 0; i < 3; i++)
//        {
//            tmp_buf[i + 3] = tmp_buf[i + 4];    //删除周次数据并将日期前移
//        }
//        file_write(addr, tmp_buf, 6);           //保存开盖发生时刻
//        //记录此时电量数据集:正、反向有功总电能、四象限无功总电能
//        addr += 12;                             //求出开端钮盒时电量数据存储偏移地址
//        for(i = 0; i < 6; i++)
//        {
//            j = (i > 1? 3 : 1);
//            file_write(addr + i * 4, (U8 *)&gEngArray[6 * (i + j)], ENERGY_LEN);
//        }
//    }
//    else if((oc_flg & OC_TMNBX_FLG) && (!PORT0_IS_HIGH(P0_KEYD)))    //关端钮盒
//    {
//        oc_flg &= ~OC_TMNBX_FLG;                //标识为关端钮盒
//        file_write(EEPROM_GFLG_ADDR, &oc_flg, 1);                   //保存本次关端钮盒标识
//        ALARM_LED_OFF();     //DBG
//        //关端钮盒(结束时刻)记录处理
//        file_read(EVET_OPNTMNBX_03300E00_00_ADDR, tmp_buf, EVET_OPNTMNBX_03300E00_00_LEN);//读取三字节总次数
//        cnt2 = byte_merge1(tmp_buf, EVET_OPNTMNBX_03300E00_00_LEN);//开端钮盒总次数
//
//        cnt2 %= EVET_OPNTMNBX_03300E01_00_OFFSET_MAXNUM;//求出当前开端钮盒相对(10次)次数
//        addr = cnt2 * EVET_OPNTMNBX_03300E01_00_OFFSET_BLOCKLEN + EVET_OPNTMNBX_03300E01_00_HEAD_ADDR;//求出本次开端钮盒数据保存起始地址
//        get_software_rtc(tmp_buf);              //获取开端钮盒发生时刻
//		convert_to_bcd(tmp_buf, 7);//azh
//		for(i = 0; i < 3; i++)
//        {
//            tmp_buf[i + 3] = tmp_buf[i + 4];    //删除周次数据并将日期前移
//        }
//        file_write(addr + 6, tmp_buf, 6);       //保存开端钮盒结束时刻
//        //记录此时电量数据集:正、反向有功总电能、四象限无功总电能
//        addr += (12 + 4 * 6);                   //求出闭端钮盒时电量数据存储偏移地址
//        for(i = 0; i < 6; i++)
//        {
//            j = (i > 1? 3 : 1);
//            file_write(addr + i * 4, (U8 *)&gEngArray[6 * (i + j)], ENERGY_LEN);
//        }
//    }
//}

/******************************************************************************
函数: void record_event_power_down(void)
入口: 无
出口: 无
概述: 无
******************************************************************************/
//void record_event_power_down (void)
//{
//    static U8 pd_flg = 0;
//    U8 i,tmp_buf[7];
//    U32 cnt,addr;
//
//    SET_PORT0_IN(P0_PD_CHK);            //配置p0.29为输入口
//    if(((pd_flg & 0x0f) == 0) && PORT0_IS_HIGH(P0_PD_CHK))        //检测掉电信号输入
//    {
//        os_dly_wait(OS_DLY_2MS);        //等待
//        if(PORT0_IS_HIGH(P0_PD_CHK))
//        {
//            pd_flg |= (1 << 0);         //未掉电标识
//        }
//    }
//    else if(((pd_flg & 0x0f) == 1) && ((PORT0_IS_HIGH(P0_PD_CHK) == 0)))//发现掉电
//    {
//        os_dly_wait(OS_DLY_2MS);        //等待
//        if((PORT0_IS_HIGH(P0_PD_CHK) == 0))
//        {
//            pd_flg |= (1 << 1);                     //掉电标识
//
//            file_read(EVET_POWDNTIME_03110000_00_ADDR, tmp_buf, EVET_POWDNTIME_03110000_00_LEN);//读取三字节总次数
//            cnt = byte_merge(tmp_buf, EVET_POWDNTIME_03110000_00_LEN);//掉电总次数
//            if(cnt == 0xFFFFFF)
//            {
//                cnt = 0x000000;                     //记为新一轮的第一次
//            }
//            else if(cnt  < 0xFFFFFF)
//            {
//                cnt++;
//            }
//            for(i = 0; i < 3; i++)
//            {
//                tmp_buf[i] = (cnt >> (i * 8));
//            }
//            file_write(EVET_POWDNTIME_03110000_00_ADDR, tmp_buf, EVET_POWDNTIME_03110000_00_LEN);//保存总次数(即次数偏移指针)
//            cnt %= EVET_POWDN_03110001_00_OFFSET_MAXNUM;//求出当前掉电相对(10次)次数
//            addr = cnt * EVET_POWDN_03110001_00_OFFSET_BLOCKLEN + EVET_POWDN_03110001_00_HEAD_ADDR;//求出本次掉电数据保存起始地址
//            get_software_rtc(tmp_buf);              //获取掉电发生时刻
//			convert_to_bcd(tmp_buf, 7);//azh
//			for(i = 0; i < 3; i++)
//            {
//                tmp_buf[i + 3] = tmp_buf[i + 4];    //删除周次数据并将日期前移
//            }
//            file_write(addr, tmp_buf, 6);           //保存掉电发生时刻
//        }
//    }
//    else if(((pd_flg & 0x0f) == 3) && PORT0_IS_HIGH(P0_PD_CHK))//重新上电
//    {
//        os_dly_wait(OS_DLY_2MS);        //等待
//        if(PORT0_IS_HIGH(P0_PD_CHK))
//        {
//            pd_flg &= 0xf0;                         //标识复位
//
//            file_read(EVET_POWDNTIME_03110000_00_ADDR, tmp_buf, EVET_POWDNTIME_03110000_00_LEN);//读取三字节总次数
//            cnt = byte_merge(tmp_buf, EVET_POWDNTIME_03110000_00_LEN);//掉电总次数
//
//            cnt %= EVET_POWDN_03110001_00_OFFSET_MAXNUM;//求出当前掉电相对(10次)次数
//            addr = cnt * EVET_POWDN_03110001_00_OFFSET_BLOCKLEN + EVET_POWDN_03110001_00_HEAD_ADDR;//求出本次掉电数据保存起始地址
//            get_software_rtc(tmp_buf);              //获取掉电结束时刻
//			convert_to_bcd(tmp_buf, 7);//azh
//			for(i = 0; i < 3; i++)
//            {
//                tmp_buf[i + 3] = tmp_buf[i + 4];    //删除周次数据并将日期前移
//            }
//            file_write(addr + 6, tmp_buf, 6);       //保存掉电结束时刻
//        }
//    }
//}

#endif//CODE_VER_TYPE!=GPRS_MODULE_PRJ
/******************************************************************************
                            End Of File
******************************************************************************/

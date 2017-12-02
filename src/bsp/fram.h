/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : fram.h
 Description    : 存储器FRAM的地址宏定义
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2010-07-23      snap           the original version
******************************************************************************/

#ifndef __FRAM_H
#define __FRAM_H

//---------------------------- 在使用new_convert工具看宏地址时需要用到eeprom.h energy.h中宏
//azh 110527 从energy.h拷贝过来
#define PHASE_A                         0           //A相数据，数组下标的宏定义
#define PHASE_C                         1           //C相数据，数组下标的宏定义
#define PHASE_TOTAL                     2           //合相数据，数组下标的宏定义 
#define ARRAY_NUM                       (PHASE_TOTAL + 1)       //数据组的个数，决定二维数组的第一个下标
#define LITTLE_ENERGY_NUM               14          //定义数组'Little_Energy'的数据个数

//----------------------------- macro definition ------------------------------
#define FRAM_HEAD_ADDR                      0x60000000          //定义FRAM的逻辑起始地址
#define FRAM_CAPACITY                       (32 * 1024)         //定义FRAM的容量32KBytes

#define FRAM_ADDR                           0xa0                //FRAM slave address

//azh 110926
//#define RATE_NUM                            4                   //费率数
//#define ENERGY_LEN                          4                   //单个电能量数据的长度
//#define CRC_LEN                             2                   //CRC校验的长度

//-----------------------------------------------------------------------------
// 小于0.01MW.h的电能量数据的保存区
//-----------------------------------------------------------------------------
#define LITTLE_ENERGY_ADDR                  FRAM_HEAD_ADDR      //小于0.01MW.h的电能量
#define LITTLE_ENERGY_LEN                   (LITTLE_ENERGY_NUM * 4 * ARRAY_NUM)

//-----------------------------------------------------------------------------
// 电能量的宏定义中间不能夹杂其他数据的宏定义，不能颠倒顺序，否则造成程序寻址出错
// COMB_ACTIVE_ENERGY_ADDR -> PHASE_C_ENERGY_ADDR
// 4字节二进制补码格式，低位在前
//-----------------------------------------------------------------------------
// 合相电能量的宏定义
//-----------------------------------------------------------------------------
//azh 110926
//#define TOTAL_PHASE_ENERGY_LEN              ((RATE_NUM + 1) * ENERGY_LEN + CRC_LEN)                             //合相电能量数据集合的长度

#define COMB_ACTIVE_ENERGY_ADDR             (LITTLE_ENERGY_ADDR + LITTLE_ENERGY_LEN)                            //组合有功电能量
//总
//费率1
//费率2
//...
//费率n(n取决于RATE_NUM)
//CRC(2Bytes)
#define POS_ACTIVE_ENERGY_ADDR              (COMB_ACTIVE_ENERGY_ADDR + TOTAL_PHASE_ENERGY_LEN)                  //正向有功电能量
#define NEG_ACTIVE_ENERGY_ADDR              (POS_ACTIVE_ENERGY_ADDR + TOTAL_PHASE_ENERGY_LEN)                   //反向有功电能量
#define COMB_1_REACTIVE_ENERGY_ADDR         (NEG_ACTIVE_ENERGY_ADDR + TOTAL_PHASE_ENERGY_LEN)                   //组合1无功电能量
#define COMB_2_REACTIVE_ENERGY_ADDR         (COMB_1_REACTIVE_ENERGY_ADDR + TOTAL_PHASE_ENERGY_LEN)              //组合2无功电能量
#define QUADRANT_1_REACTIVE_ENERGY_ADDR     (COMB_2_REACTIVE_ENERGY_ADDR + TOTAL_PHASE_ENERGY_LEN)              //象限1无功电能量
#define QUADRANT_2_REACTIVE_ENERGY_ADDR     (QUADRANT_1_REACTIVE_ENERGY_ADDR + TOTAL_PHASE_ENERGY_LEN)          //象限2无功电能量
#define QUADRANT_3_REACTIVE_ENERGY_ADDR     (QUADRANT_2_REACTIVE_ENERGY_ADDR + TOTAL_PHASE_ENERGY_LEN)          //象限3无功电能量
#define QUADRANT_4_REACTIVE_ENERGY_ADDR     (QUADRANT_3_REACTIVE_ENERGY_ADDR + TOTAL_PHASE_ENERGY_LEN)          //象限4无功电能量
#define POS_APPARENT_ENERGY_ADDR            (QUADRANT_4_REACTIVE_ENERGY_ADDR + TOTAL_PHASE_ENERGY_LEN)          //正向视在电能量
#define NEG_APPARENT_ENERGY_ADDR            (POS_APPARENT_ENERGY_ADDR + TOTAL_PHASE_ENERGY_LEN)                 //反向视在电能量

//-----------------------------------------------------------------------------
// 关联、基波、谐波电能量的宏定义
// 关联总电能
// 正向有功基波总电能
// 反向有功基波总电能
// 正向有功谐波总电能
// 反向有功谐波总电能
// CRC(2Bytes)
//-----------------------------------------------------------------------------
#define RELATED_ENERGY_ADDR                 (NEG_APPARENT_ENERGY_ADDR + TOTAL_PHASE_ENERGY_LEN)                 //关联电能量
#define POS_FUND_ACTIVE_ENERGY_ADDR         (RELATED_ENERGY_ADDR + ENERGY_LEN)                                  //正向基波有功电能量
#define NEG_FUND_ACTIVE_ENERGY_ADDR         (POS_FUND_ACTIVE_ENERGY_ADDR + ENERGY_LEN)                          //反向基波有功电能量
#define POS_HARMONIC_ACTIVE_ENERGY_ADDR     (NEG_FUND_ACTIVE_ENERGY_ADDR + ENERGY_LEN)                          //正向谐波有功电能量
#define NEG_HARMONIC_ACTIVE_ENERGY_ADDR     (POS_HARMONIC_ACTIVE_ENERGY_ADDR + ENERGY_LEN)                      //反向谐波有功电能量
#define RELATED_ENERGY_CRC                  (NEG_HARMONIC_ACTIVE_ENERGY_ADDR + ENERGY_LEN)                      //关联电能量CRC
//#define RELATED_ENERGY_LEN                  (5 * ENERGY_LEN + CRC_LEN)                                          //关联电能量数据集合的长度

//-----------------------------------------------------------------------------
// 分相电能量的宏定义
//-----------------------------------------------------------------------------
//azh 110926
//#define SINGLE_PHASE_ENERGY_LEN             (10 * ENERGY_LEN + CRC_LEN)                                         //分相电能量数据集合的长度
//#define SINGLE_PHASE_ALL_ENERGY_LEN         (SINGLE_PHASE_ENERGY_LEN + RELATED_ENERGY_LEN)                      //分相所有电能量数据集合的长度
#define PHASE_A_ENERGY_ADDR                 (RELATED_ENERGY_CRC + CRC_LEN)                                      //A相电能量
//正向有功电能
//反向有功电能
//组合无功1电能
//组合无功2电能
//第一象限无功电能
//第二象限无功电能
//第三象限无功电能
//第四象限无功电能
//正向视在电能
//反向视在电能
//CRC(2Bytes)
#define PHASE_A_RELATED_ENERGY_ADDR         (PHASE_A_ENERGY_ADDR + SINGLE_PHASE_ENERGY_LEN)                     //A相关联电能量
//关联电能
//正向有功基波电能
//反向有功基波电能
//正向有功谐波电能
//反向有功谐波电能
//CRC(2Bytes)
//---------------------------- 在使用new_convert工具看宏地址时需要改动, 确定具体是哪种编译
#ifdef PHASE_3_LINE_4
#define PHASE_B_ENERGY_ADDR                 (PHASE_A_RELATED_ENERGY_ADDR + RELATED_ENERGY_LEN)                  //B相电能量
#define PHASE_B_RELATED_ENERGY_ADDR         (PHASE_B_ENERGY_ADDR + SINGLE_PHASE_ENERGY_LEN)                     //B相关联电能量
#define PHASE_C_ENERGY_ADDR                 (PHASE_B_RELATED_ENERGY_ADDR + RELATED_ENERGY_LEN)                  //C相电能量
#define PHASE_C_RELATED_ENERGY_ADDR         (PHASE_C_ENERGY_ADDR + SINGLE_PHASE_ENERGY_LEN)                     //C相关联电能量
#else//PHASE_3_LINE_4
#define PHASE_C_ENERGY_ADDR                 (PHASE_A_RELATED_ENERGY_ADDR + RELATED_ENERGY_LEN)                  //C相电能量
#define PHASE_C_RELATED_ENERGY_ADDR         (PHASE_C_ENERGY_ADDR + SINGLE_PHASE_ENERGY_LEN)                     //C相关联电能量
#endif//PHASE_3_LINE_4

//-----------------------------------------------------------------------------
// 电能量备份的宏定义，数据排列与当前电量的排列是一样的
//-----------------------------------------------------------------------------
#define ENERGY_BACKUP_NUM                   3           //电能量备份的个数
#define ENERGY_BACKUP_LEN                   (PHASE_C_RELATED_ENERGY_ADDR + RELATED_ENERGY_LEN - COMB_ACTIVE_ENERGY_ADDR)

#define ENERGY_BACKUP_POINTER               (PHASE_C_RELATED_ENERGY_ADDR + RELATED_ENERGY_LEN)                  //当前电能量备份的指针，总是指向最新记录后的一个空记录或最老的记录
#define ENERGY_BACKUP_POINTER_LEN           1
#define ENERGY_BACKUP_ADDR                  (ENERGY_BACKUP_POINTER + ENERGY_BACKUP_POINTER_LEN)                 //电能量备份区

//-----------------------------------------------------------------------------
// 与数据冻结相关的宏定义
// 包括结算日、定时/瞬时冻结、时段表切换冻结、时区切换冻结
//-----------------------------------------------------------------------------
#define MONTH_OF_ACCOUNT_DAY1_RECORD        (ENERGY_BACKUP_ADDR + ENERGY_BACKUP_LEN * ENERGY_BACKUP_NUM)        //第1结算日最近一次冻结的数据的月份值
#define MONTH_OF_ACCOUNT_DAY_RECORD_LEN     1
#define MONTH_OF_ACCOUNT_DAY2_RECORD        (MONTH_OF_ACCOUNT_DAY1_RECORD + MONTH_OF_ACCOUNT_DAY_RECORD_LEN)    //第2结算日最近一次冻结的数据的月份值
#define MONTH_OF_ACCOUNT_DAY3_RECORD        (MONTH_OF_ACCOUNT_DAY2_RECORD + MONTH_OF_ACCOUNT_DAY_RECORD_LEN)    //第3结算日最近一次冻结的数据的月份值

#define ACCOUNT_DAY_DATA_POINTER            (MONTH_OF_ACCOUNT_DAY3_RECORD + MONTH_OF_ACCOUNT_DAY_RECORD_LEN)    //结算日冻结数据的指针，总是指向最新记录后的一个空记录或最老的记录
#define ACCOUNT_DAY_DATA_POINTER_LEN        1
#define TIMING_FREEZE_DATA_POINTER          (ACCOUNT_DAY_DATA_POINTER + ACCOUNT_DAY_DATA_POINTER_LEN)           //定时冻结数据的指针，总是指向最新记录后的一个空记录或最老的记录
#define FREEZE_DATA_POINTER_LEN             1
#define INSTANT_FREEZE_DATA_POINTER         (TIMING_FREEZE_DATA_POINTER + FREEZE_DATA_POINTER_LEN)              //瞬时冻结数据的指针，总是指向最新记录后的一个空记录或最老的记录
#define TIME_CHANGE_FREEZE_DATA_POINTER     (INSTANT_FREEZE_DATA_POINTER + FREEZE_DATA_POINTER_LEN)             //时段表切换冻结数据的指针，总是指向最新记录后的一个空记录或最老的记录
#define ZONE_CHANGE_FREEZE_DATA_POINTER     (TIME_CHANGE_FREEZE_DATA_POINTER + FREEZE_DATA_POINTER_LEN)         //时区切换冻结数据的指针，总是指向最新记录后的一个空记录或最老的记录
#define LOAD_PROFILE_RECORD_POINTER         (ZONE_CHANGE_FREEZE_DATA_POINTER + FREEZE_DATA_POINTER_LEN)         //负荷曲线数据的指针，总是指向最新记录后的一个空记录或最老的记录
#define LOAD_PROFILE_RECORD_POINTER_LEN     4

#define BY_SECOND_RESTORE                   (LOAD_PROFILE_RECORD_POINTER + LOAD_PROFILE_RECORD_POINTER_LEN)     //每秒写出的时刻数据
#define BY_SECOND_RESTORE_LEN               6

//#define FRAM_LAST_ADDR                      (BY_SECOND_RESTORE + BY_SECOND_RESTORE_LEN)
/*/-----------------------------------------------------------------------------
// 上电回读CS5463计量寄存器原始(累加)值在FRAM中保存逻辑地址宏定义
// 说明: 掉电检测时发现掉电将sum_buf[12]的数据进行判别保存至FRAM, 上电后要判别重载
//-----------------------------------------------------------------------------
#define MEASURE_DATA_LEN                    (12 * 4 + 2)                    //DATA(48) + CRC(2)
#define MEASURE_SAVE_ADDR                   (LOAD_PROFILE_RECORD_POINTER + LOAD_PROFILE_RECORD_POINTER_LEN)

#define FRAM_LAST_ADDR                      (MEASURE_SAVE_ADDR + MEASURE_DATA_LEN)
*/

//----------------------------- type definition -------------------------------

//-------------------------- functions declaration ----------------------------

//--------------------------- variable declaration ----------------------------

#endif//__FRAM_H

/******************************************************************************
    End Of File
******************************************************************************/

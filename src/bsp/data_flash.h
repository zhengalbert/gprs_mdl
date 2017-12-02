/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : data_flash.h
 Description    : 保存在DataFlash中数据的地址宏定义的头文件
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2010-07-26      snap           the original version
******************************************************************************/

#ifndef __DATA_FLASH_H
#define __DATA_FLASH_H

//---------------------------- 在使用new_convert工具看宏地址时需要用到fram.h eeprom.h energy.h中宏, 使用时注意同真正代码同步
//#define RATE_NUM                            4                   //费率数
//#define ENERGY_LEN                          4                   //单个电能量数据的长度
//#define CRC_LEN                             2                   //CRC校验的长度
//#define TOTAL_PHASE_ENERGY_LEN              ((RATE_NUM + 1) * ENERGY_LEN + CRC_LEN)                             //合相电能量数据集合的长度
//#define DEMAND_LEN                          8                   //单个需量数据的长度
//#define TOTAL_PHASE_DEMAND_LEN              ((RATE_NUM + 1) * DEMAND_LEN)                                       //合相需量数据集合的长度
//#define SINGLE_PHASE_ENERGY_LEN             (10 * ENERGY_LEN + CRC_LEN)                                         //分相电能量数据集合的长度
//#define SINGLE_PHASE_ALL_ENERGY_LEN         (SINGLE_PHASE_ENERGY_LEN + RELATED_ENERGY_LEN)                      //分相所有电能量数据集合的长度
//#define SINGLE_PHASE_DEMAND_LEN             (10 * DEMAND_LEN)                                                   //分相需量数据集合的长度


//----------------------------- macro definition ------------------------------
#define DATAFLASH1_HEAD_ADDR    0x70000000          //定义第一片DataFlash的逻辑起始地址
#define DATAFLASH2_HEAD_ADDR    0x71000000          //定义第二片DataFlash的逻辑起始地址

//-----------------------------------------------------------------------------
// 存储格式：4字节十进制数格式，低位在前
// 4费率情况下三相三线时需要冻结的电能量数据为360字节
// 4费率情况下三相四线时需要冻结的电能量数据为420字节
// 数据点中的数据顺序与当前电能量一致：
//  1、组合有功总+费率电能量------------(1 + 4) * 4 字节
//  2、正向有功总+费率电能量------------(1 + 4) * 4 字节
//  3、反向有功总+费率电能量------------(1 + 4) * 4 字节
//  4、组合无功1总+费率电能量-----------(1 + 4) * 4 字节
//  5、组合无功2总+费率电能量-----------(1 + 4) * 4 字节
//  6、第一象限无功总+费率电能量--------(1 + 4) * 4 字节
//  7、第二象限无功总+费率电能量--------(1 + 4) * 4 字节
//  8、第三象限无功总+费率电能量--------(1 + 4) * 4 字节
//  9、第四象限无功总+费率电能量--------(1 + 4) * 4 字节
// 10、正向视在总+费率电能量------------(1 + 4) * 4 字节
// 11、反向视在总+费率电能量------------(1 + 4) * 4 字节
// 12、关联、基波、谐波电能量----------- 5 * 4 字节
// 13、A相电能量------------------------10 * 4 字节
// 14、B相电能量（如果是三线四线表）----10 * 4 字节
// 15、C相电能量------------------------10 * 4 字节
//-----------------------------------------------------------------------------
// 存储格式：发生时间 + 最大需量
// 发生时间数据：mm（分）hh（时）DD（日）MM（月）YY（年），十进制格式
// 最大需量数据：3字节十进制格式，低位在前
// 4费率情况下三相三线时需要冻结的需量数据为560字节
// 4费率情况下三相四线时需要冻结的需量数据为640字节
// 数据点中的数据顺序与当前最大需量一致：
// 16、正向有功总+费率最大需量及发生时间------------(1 + 4) * 8 字节
// 17、反向有功总+费率最大需量及发生时间------------(1 + 4) * 8 字节
// 18、组合无功1总+费率最大需量及发生时间-----------(1 + 4) * 8 字节
// 19、组合无功2总+费率最大需量及发生时间-----------(1 + 4) * 8 字节
// 20、一象限无功总+费率最大需量及发生时间----------(1 + 4) * 8 字节
// 21、二象限无功总+费率最大需量及发生时间----------(1 + 4) * 8 字节
// 22、三象限无功总+费率最大需量及发生时间----------(1 + 4) * 8 字节
// 23、四象限无功总+费率最大需量及发生时间----------(1 + 4) * 8 字节
// 24、正向视在总+费率最大需量及发生时间------------(1 + 4) * 8 字节
// 25、反向视在总+费率最大需量及发生时间------------(1 + 4) * 8 字节
// 26、A相最大需量及发生时间------------------------10 * 8 字节
// 27、B相最大需量及发生时间（如果是三线四线表）----10 * 8 字节
// 28、C相最大需量及发生时间------------------------10 * 8 字节
//-----------------------------------------------------------------------------
// 结算日冻结数据保存区的宏定义
//-----------------------------------------------------------------------------
#ifdef PHASE_3_LINE_4
#define ACCOUNT_DAY_ENERGY_LEN                 ((TOTAL_PHASE_ENERGY_LEN - CRC_LEN) * 11 + \
                                                (RELATED_ENERGY_LEN - CRC_LEN) +          \
                                                (SINGLE_PHASE_ALL_ENERGY_LEN - 2 * CRC_LEN) * 3)                    //单条结算日电能量冻结数据实际长度（三线四线）
#define ACCOUNT_DAY_DEMAND_LEN                  (TOTAL_PHASE_DEMAND_LEN * 10 + SINGLE_PHASE_DEMAND_LEN * 3)         //单条结算日需  量冻结数据实际长度（三线四线）
#else//PHASE_3_LINE_4
#define ACCOUNT_DAY_ENERGY_LEN                 ((TOTAL_PHASE_ENERGY_LEN - CRC_LEN) * 11 + \
                                                (RELATED_ENERGY_LEN - CRC_LEN) +          \
                                                (SINGLE_PHASE_ALL_ENERGY_LEN - 2 * CRC_LEN) * 2)                    //单条结算日电能量冻结数据实际长度（三线三线）
#define ACCOUNT_DAY_DEMAND_LEN                  (TOTAL_PHASE_DEMAND_LEN * 10 + SINGLE_PHASE_DEMAND_LEN * 2)         //单条结算日需  量冻结数据实际长度（三线三线）
#endif//PHASE_3_LINE_4
//----------------------- 在使用new_convert工具看宏地址时需要注释掉前面的#if METER_TYPE系列行, 注意和真正代码同步
//#define ACCOUNT_DAY_ENERGY_LEN                 ((TOTAL_PHASE_ENERGY_LEN - CRC_LEN) * 11 + (RELATED_ENERGY_LEN - CRC_LEN) + (SINGLE_PHASE_ALL_ENERGY_LEN - 2 * CRC_LEN) * 2)                    //单条结算日电能量冻结数据实际长度（三线三线）
//#define ACCOUNT_DAY_DEMAND_LEN                  (TOTAL_PHASE_DEMAND_LEN * 10 + SINGLE_PHASE_DEMAND_LEN * 2)         //单条结算日需  量冻结数据实际长度（三线三线）


#define ACCOUNT_DAY_ENERGY_STORE_LEN            ACCOUNT_DAY_ENERGY_LEN                                              //单条结算日电能量冻结数据存储占用存储区长度
#define ACCOUNT_DAY_DEMAND_STORE_LEN            ACCOUNT_DAY_DEMAND_LEN                                              //单条结算日需  量冻结数据存储占用存储区长度
#define SINGLE_ACCOUNT_DAY_DATA_LEN             (ACCOUNT_DAY_ENERGY_STORE_LEN + ACCOUNT_DAY_DEMAND_STORE_LEN)       //单条结算日完整冻结数据存储占用存储区长度
#define ACCOUNT_DAY_DATA_NUM                    12                                                                  //结算日冻结数据的条数
#define ACCOUNT_DAY_BUF_LEN                     (SINGLE_ACCOUNT_DAY_DATA_LEN * ACCOUNT_DAY_DATA_NUM)                //结算日冻结数据保存区的长度
#define ACCOUNT_DAY_DATA_ADDR                   DATAFLASH1_HEAD_ADDR                                                //结算日冻结数据首地址
#define ACCOUNT_DAY_ENERGY_ADDR                 DATAFLASH1_HEAD_ADDR                                                //结算日电能量冻结数据首地址
#define ACCOUNT_DAY_DEMAND_ADDR                 (ACCOUNT_DAY_ENERGY_ADDR + ACCOUNT_DAY_ENERGY_STORE_LEN)            //结算日需  量冻结数据首地址

//-----------------------------------------------------------------------------
// 存储格式：4字节十进制数格式，低位在前
// 4费率情况下需要冻结的电能量、需量和瞬时量数据为269字节
// 数据点中的数据顺序：
//  1、正向有功总+费率电能量----------------(1 + 4) * 4 字节
//  2、反向有功总+费率电能量----------------(1 + 4) * 4 字节
//  3、组合无功1总+费率电能量---------------(1 + 4) * 4 字节
//  4、组合无功2总+费率电能量---------------(1 + 4) * 4 字节
//  5、第一象限无功总+费率电能量------------(1 + 4) * 4 字节
//  6、第二象限无功总+费率电能量------------(1 + 4) * 4 字节
//  7、第三象限无功总+费率电能量------------(1 + 4) * 4 字节
//  8、第四象限无功总+费率电能量------------(1 + 4) * 4 字节
//  9、正向有功总+费率最大需量及发生时间----(1 + 4) * 8 字节
// 10、反向有功总+费率最大需量及发生时间----(1 + 4) * 8 字节
// 11、变量数据
//       总有功功率-------------------------3 字节
//       A相有功功率------------------------3 字节
//       B相有功功率------------------------3 字节
//       C相有功功率------------------------3 字节
//       总无功功率-------------------------3 字节
//       A相无功功率------------------------3 字节
//       B相无功功率------------------------3 字节
//       C相无功功率------------------------3 字节
//-----------------------------------------------------------------------------
// 定时冻结数据保存区的宏定义
//-----------------------------------------------------------------------------
#define SINGLE_FREEZE_DATA_LEN                  (5 + (TOTAL_PHASE_ENERGY_LEN - CRC_LEN) * 8 + \
                                                TOTAL_PHASE_DEMAND_LEN * 2 + 3 * 8)                                 //单条冻结数据实际长度
//#define SINGLE_FREEZE_DATA_LEN                  (5 + (TOTAL_PHASE_ENERGY_LEN - CRC_LEN) * 8 + TOTAL_PHASE_DEMAND_LEN * 2 + 3 * 8)//单条冻结数据实际长度
#define SINGLE_FREEZE_STORE_LEN                 SINGLE_FREEZE_DATA_LEN                                              //单条冻结数据存储占用存储区长度
#define TIMING_FREEZE_DATA_NUM                  12                                                                  //定时冻结数据的条数
#define TIMING_FREEZE_BUF_LEN                   (SINGLE_FREEZE_STORE_LEN * TIMING_FREEZE_DATA_NUM)                  //定时冻结数据保存区的长度
#define TIMING_FREEZE_DATA_ADDR                 (ACCOUNT_DAY_DATA_ADDR + ACCOUNT_DAY_BUF_LEN)                       //定时冻结数据首地址

//-----------------------------------------------------------------------------
// 瞬时冻结数据保存区的宏定义
//-----------------------------------------------------------------------------
#define INSTANT_FREEZE_DATA_NUM                 3                                                                   //瞬时冻结数据的条数
#define INSTANT_FREEZE_DATA_LEN                 (SINGLE_FREEZE_STORE_LEN * INSTANT_FREEZE_DATA_NUM)                 //瞬时冻结数据保存区的长度
#define INSTANT_FREEZE_DATA_ADDR                (TIMING_FREEZE_DATA_ADDR + TIMING_FREEZE_BUF_LEN)                   //瞬时冻结数据首地址

//-----------------------------------------------------------------------------
// 时段表切换冻结数据保存区的宏定义
//-----------------------------------------------------------------------------
#define TIME_CHANGE_FREEZE_DATA_NUM             2                                                                   //时段表切换冻结数据的条数
#define TIME_CHANGE_FREEZE_BUF_LEN              (SINGLE_FREEZE_STORE_LEN * TIME_CHANGE_FREEZE_DATA_NUM)             //时段表切换冻结数据保存区的长度
#define TIME_CHANGE_FREEZE_DATA_ADDR            (INSTANT_FREEZE_DATA_ADDR + INSTANT_FREEZE_DATA_LEN)                //时段表切换冻结数据首地址

//-----------------------------------------------------------------------------
// 年时区切换冻结数据保存区的宏定义
//-----------------------------------------------------------------------------
#define ZONE_CHANGE_FREEZE_DATA_NUM             2                                                                   //年时区切换冻结数据的条数
#define ZONE_CHANGE_FREEZE_BUF_LEN              (SINGLE_FREEZE_STORE_LEN * ZONE_CHANGE_FREEZE_DATA_NUM)             //年时区切换冻结数据保存区的长度
#define ZONE_CHANGE_FREEZE_DATA_ADDR            (TIME_CHANGE_FREEZE_DATA_ADDR + TIME_CHANGE_FREEZE_BUF_LEN)         //年时区切换冻结数据首地址

//-----------------------------------------------------------------------------
// 负荷曲线数据保存区的宏定义
// 负荷记录数据顺序：（共92字节）
// 1、电压、电流、频率
//      A、B、C 相电压（每相 2 字节，共 6 字节，单位：0.1kV）
//      A、B、C 相电流（每相 3 字节，共 9 字节，单位：0.001A）
//      频率（2 字节，单位：0.01Hz）
// 2、有、无功功率
//      总及A、B、C 相有功功率（每个 3 字节，共 12 字节，单位：0.0001MW）
//      总及A、B、C 相无功功率（每个 3 字节，共 12 字节，单位：0.0001Mvar）
// 3、功率因数
//      总及 A、B、C 相功率因数（每个 2 字节，共 8 字节，单位：0.001）
// 4、有、无功总电能
//      正向有功总电能（4 字节，单位：0.01MWh）
//      反向有功总电能（4 字节，单位：0.01MWh）
//      组合无功 1 总电能（4 字节，单位：0.01Mvarh）
//      组合无功 2 总电能（4 字节，单位：0.01Mvarh）
// 5、四象限无功总电能
//      第一象限无功总电能（4 字节，单位：0.01Mvarh）
//      第二象限无功总电能（4 字节，单位：0.01Mvarh）
//      第三象限无功总电能（4 字节，单位：0.01Mvarh）
//      第四象限无功总电能（4 字节，单位：0.01Mvarh）
// 6、当前需量
//      当前有功需量（3 字节，单位：0.0001MW）
//      当前无功需量（3 字节，单位：0.0001Mvar）
//-----------------------------------------------------------------------------
#define LOAD_PROFILE_RECORD_NUM                 (60 * 24 * 7)                                                       //负荷曲线数据的条数，间隔1min记录7天
#define LOAD_PROFILE_RECORD_LEN                 92                                                                  //单条负荷曲线数据实际长度
#define LOAD_PROFILE_STORE_LEN                  92                                                                  //单条负荷曲线数据存储占用存储区长度
#define LOAD_PROFILE_BUF_LEN                    (LOAD_PROFILE_STORE_LEN * LOAD_PROFILE_RECORD_NUM)                  //负荷曲线数据保存区的长度
#define LOAD_PROFILE_DATA_ADDR                  (ZONE_CHANGE_FREEZE_DATA_ADDR + ZONE_CHANGE_FREEZE_BUF_LEN)         //负荷曲线数据首地址

#define ZONE_TABLE_UPDATE_TIME_ADDR             (LOAD_PROFILE_DATA_ADDR + LOAD_PROFILE_BUF_LEN)                     //两套时区表切换时间 分时日月年 [和下面相邻]
#define ZONE_TABLE_UPDATE_TIME_LEN              5
#define FEE_TABLE_UPDATE_TIME_ADDR              (ZONE_TABLE_UPDATE_TIME_ADDR + ZONE_TABLE_UPDATE_TIME_LEN)          //两套日时段表切换时间 分时日月年[和上面相邻]
#define FEE_TABLE_UPDATE_TIME_LEN               5
#define ZONE_TABLE_ONE_ADDR                     (FEE_TABLE_UPDATE_TIME_ADDR + FEE_TABLE_UPDATE_TIME_LEN)            //第一套时区表 14*3规约调用时需要连下
#define ZONE_TABLE_ONE_LEN                      (14 * 3)                                                            //MMDDNN
#define FEE_TABLE_ONE1_ADDR                     (ZONE_TABLE_ONE_ADDR + ZONE_TABLE_ONE_LEN)                          //第一套1日时段表连上
#define FEE_TABLE_ONE1_LEN                      (14 * 3)                                                            //日时段表的长度
#define FEE_TABLE_NUM                           8                                                                   //时段表数
#define ZONE_TABLE_TWO_ADDR                     (FEE_TABLE_ONE1_ADDR + FEE_TABLE_ONE1_LEN * FEE_TABLE_NUM)          //第二套时区表 14*3规约调用时需要连下
#define FEE_TABLE_TWO1_ADDR                     (ZONE_TABLE_TWO_ADDR + ZONE_TABLE_ONE_LEN)                          //第二套1日时段表连上
#define HOLIDAY_TABLE_ADDR                      (FEE_TABLE_TWO1_ADDR + FEE_TABLE_ONE1_LEN * FEE_TABLE_NUM)          //公共假日
#define HOLIDAY_TABLE_LEN                       (254 * 4)
#define WEEKDAY_STATUS_ADDR                     (HOLIDAY_TABLE_ADDR + HOLIDAY_TABLE_LEN)                            //周休日状态字 1字节 连接下
#define WEEKDAY_TABLE_ADDR                      (WEEKDAY_STATUS_ADDR + 1)                                           //周休日执行时段表号 1字节 连接上

#define POS_ACTIVE_DEMAND_LIM_ADDR              (WEEKDAY_TABLE_ADDR + 1)                                            //正向有功最大需量门限
#define POS_ACTIVE_DEMAND_LIM_LEN               3
#define NEG_ACTIVE_DEMAND_LIM_ADDR              (POS_ACTIVE_DEMAND_LIM_ADDR + POS_ACTIVE_DEMAND_LIM_LEN)            //反向有功最大需量门限
#define QUADRANT_1_REACTIVE_DEMAND_LIM_ADDR     (NEG_ACTIVE_DEMAND_LIM_ADDR + POS_ACTIVE_DEMAND_LIM_LEN)            //第1象限无功需量门限
#define QUADRANT_2_REACTIVE_DEMAND_LIM_ADDR     (QUADRANT_1_REACTIVE_DEMAND_LIM_ADDR + POS_ACTIVE_DEMAND_LIM_LEN)   //第2象限无功需量门限
#define QUADRANT_3_REACTIVE_DEMAND_LIM_ADDR     (QUADRANT_2_REACTIVE_DEMAND_LIM_ADDR + POS_ACTIVE_DEMAND_LIM_LEN)   //第3象限无功需量门限
#define QUADRANT_4_REACTIVE_DEMAND_LIM_ADDR     (QUADRANT_3_REACTIVE_DEMAND_LIM_ADDR + POS_ACTIVE_DEMAND_LIM_LEN)   //第4象限无功需量门限

#define POS_ACTIVE_DEMAND_REC_ADDR              (QUADRANT_4_REACTIVE_DEMAND_LIM_ADDR + POS_ACTIVE_DEMAND_LIM_LEN)   //正向有功最大需量记录1-10
#define POS_ACTIVE_DEMAND_REC_LEN               ((6 + 6 + 3 + 5) * 10)                                              //发生结束时刻(包含秒)最大需量及发生时间(没有秒) 参见规约03 12 01 01
#define NEG_ACTIVE_DEMAND_REC_ADDR              (POS_ACTIVE_DEMAND_REC_ADDR + POS_ACTIVE_DEMAND_REC_LEN)            //反向有功最大需量记录1-10
#define QUADRANT_1_REACTIVE_DEMAND_REC_ADDR     (NEG_ACTIVE_DEMAND_REC_ADDR + POS_ACTIVE_DEMAND_REC_LEN)            //第1象限无功需量记录1-10
#define QUADRANT_2_REACTIVE_DEMAND_REC_ADDR     (QUADRANT_1_REACTIVE_DEMAND_REC_ADDR + POS_ACTIVE_DEMAND_REC_LEN)   //第2象限无功需量记录1-10
#define QUADRANT_3_REACTIVE_DEMAND_REC_ADDR     (QUADRANT_2_REACTIVE_DEMAND_REC_ADDR + POS_ACTIVE_DEMAND_REC_LEN)   //第3象限无功需量记录1-10
#define QUADRANT_4_REACTIVE_DEMAND_REC_ADDR     (QUADRANT_3_REACTIVE_DEMAND_REC_ADDR + POS_ACTIVE_DEMAND_REC_LEN)   //第4象限无功需量记录1-10

#define PROGRAM_RECORD_ADDR                     (QUADRANT_4_REACTIVE_DEMAND_REC_ADDR + POS_ACTIVE_DEMAND_REC_LEN)   //编程记录, 10个
#define PROGRAM_RECORD_LEN                      (6 + 4 + 4 * 10)

#define ENERGY_ZERO_COUNT_ADDR                  (PROGRAM_RECORD_ADDR + PROGRAM_RECORD_LEN * 10)                     //电表清零总次数
#define ENERGY_ZERO_COUNT_LEN                   3
#define ENERGY_ZERO_RECORD_ADDR                 (ENERGY_ZERO_COUNT_ADDR + ENERGY_ZERO_COUNT_LEN)                    //电表清零记录(含B相数据), 10个
#define ENERGY_ZERO_RECORD_LEN                  (6 + 4 + 4 * 6 + 4 * 6 * 3)

#define DEMAND_ZERO_RECORD_ADDR                 (ENERGY_ZERO_RECORD_ADDR + ENERGY_ZERO_RECORD_LEN * 10)             //清需量记录(含B相数据), 10个
#define DEMAND_ZERO_RECORD_LEN                  (6 + 4 + (3 + 5) * 6 * 4)

#define EVENT_ZERO_COUNT_ADDR                   (DEMAND_ZERO_RECORD_ADDR + DEMAND_ZERO_RECORD_LEN * 10)             //事件清零次数
#define EVENT_ZERO_COUNT_LEN                    3
#define EVENT_ZERO_RECORD_ADDR                  (EVENT_ZERO_COUNT_ADDR + EVENT_ZERO_COUNT_LEN)                      //事件清零记录, 10个
#define EVENT_ZERO_RECORD_LEN                   (6 + 4 + 4)

#define CHECK_TIME_COUNT_ADDR                   (EVENT_ZERO_RECORD_ADDR + EVENT_ZERO_RECORD_LEN * 10)               //校时总次数
#define CHECK_TIME_COUNT_LEN                    3
#define CHECK_TIME_RECORD_ADDR                  (CHECK_TIME_COUNT_ADDR + CHECK_TIME_COUNT_LEN)                      //校时记录, 10个
#define CHECK_TIME_RECORD_LEN                   (4 + 6 + 6)

#define TABLE_PROGRAM_COUNT_ADDR                (CHECK_TIME_RECORD_ADDR + CHECK_TIME_RECORD_LEN * 10)               //时段表编程总次数
#define TABLE_PROGRAM_COUNT_LEN                 3
#define TABLE_PROGRAM_RECORD_ADDR               (TABLE_PROGRAM_COUNT_ADDR + TABLE_PROGRAM_COUNT_LEN)                //时段表编程记录, 10个
#define TABLE_PROGRAM_RECORD_LEN                (6 + 4 + 3 * 14 * 8 * 2)

#define ZONE_PROGRAM_COUNT_ADDR                 (TABLE_PROGRAM_RECORD_ADDR + TABLE_PROGRAM_RECORD_LEN * 10)         //时区编程总次数
#define ZONE_PROGRAM_COUNT_LEN                  3
#define ZONE_PROGRAM_RECORD_ADDR                (ZONE_PROGRAM_COUNT_ADDR + ZONE_PROGRAM_COUNT_LEN)                  //时区编程记录, 10个
#define ZONE_PROGRAM_RECORD_LEN                 (6 + 4 + 3 * 14 * 2)

#define WEEKDAY_PROGRAM_COUNT_ADDR              (ZONE_PROGRAM_RECORD_ADDR + ZONE_PROGRAM_RECORD_LEN * 10)           //周休日编程总次数
#define WEEKDAY_PROGRAM_COUNT_LEN               3
#define WEEKDAY_PROGRAM_RECORD_ADDR             (WEEKDAY_PROGRAM_COUNT_ADDR + WEEKDAY_PROGRAM_COUNT_LEN)            //周休日编程记录, 10个
#define WEEKDAY_PROGRAM_RECORD_LEN              (6 + 4 + 1)

#define HOLIDAY_PROGRAM_COUNT_ADDR              (WEEKDAY_PROGRAM_RECORD_ADDR + WEEKDAY_PROGRAM_RECORD_LEN * 10)     //节假日编程总次数
#define HOLIDAY_PROGRAM_COUNT_LEN               3
#define HOLIDAY_PROGRAM_RECORD_ADDR             (HOLIDAY_PROGRAM_COUNT_ADDR + HOLIDAY_PROGRAM_COUNT_LEN)            //节假日编程记录, 10个
#define HOLIDAY_PROGRAM_RECORD_LEN              (6 + 4 + 4 * 254)

#define COMB_Y_PROGRAM_COUNT_ADDR               (HOLIDAY_PROGRAM_RECORD_ADDR + HOLIDAY_PROGRAM_RECORD_LEN * 10)     //有功组合方式编程总次数
#define COMB_Y_PROGRAM_COUNT_LEN                3
#define COMB_Y_PROGRAM_RECORD_ADDR              (COMB_Y_PROGRAM_COUNT_ADDR + COMB_Y_PROGRAM_COUNT_LEN)              //有功组合方式编程记录, 10个
#define COMB_Y_PROGRAM_RECORD_LEN               (6 + 4 + 1)

#define COMB_1_PROGRAM_COUNT_ADDR               (COMB_Y_PROGRAM_RECORD_ADDR + COMB_Y_PROGRAM_RECORD_LEN * 10)       //无功组合方式1编程总次数
#define COMB_1_PROGRAM_COUNT_LEN                3
#define COMB_1_PROGRAM_RECORD_ADDR              (COMB_1_PROGRAM_COUNT_ADDR + COMB_1_PROGRAM_COUNT_LEN)              //无功组合方式1编程记录, 10个
#define COMB_1_PROGRAM_RECORD_LEN               (6 + 4 + 1)

#define COMB_2_PROGRAM_COUNT_ADDR               (COMB_1_PROGRAM_RECORD_ADDR + COMB_1_PROGRAM_RECORD_LEN * 10)       //无功组合方式2编程总次数
#define COMB_2_PROGRAM_COUNT_LEN                3
#define COMB_2_PROGRAM_RECORD_ADDR              (COMB_2_PROGRAM_COUNT_ADDR + COMB_2_PROGRAM_COUNT_LEN)              //无功组合方式2编程记录, 10个
#define COMB_2_PROGRAM_RECORD_LEN               (6 + 4 + 1)

#define ACCOUNT_DAY_PROGRAM_COUNT_ADDR          (COMB_2_PROGRAM_RECORD_ADDR + COMB_2_PROGRAM_RECORD_LEN * 10)       //结算日编程总次数
#define ACCOUNT_DAY_PROGRAM_COUNT_LEN           3
#define ACCOUNT_DAY_PROGRAM_RECORD_ADDR         (ACCOUNT_DAY_PROGRAM_COUNT_ADDR + ACCOUNT_DAY_PROGRAM_COUNT_LEN)    //结算日编程记录, 10个
#define ACCOUNT_DAY_PROGRAM_RECORD_LEN          (6 + 4 + 2 * 3)

#define DATAFLASH1_LAST_ADDR                    (ACCOUNT_DAY_PROGRAM_RECORD_ADDR + ACCOUNT_DAY_PROGRAM_RECORD_LEN * 10) //调试用 最后空余地址

//----------------------------- type definition -------------------------------

//-------------------------- functions declaration ----------------------------

//--------------------------- variable declaration ----------------------------

#endif//__DATA_FLASH_H

/******************************************************************************
    End Of File
******************************************************************************/

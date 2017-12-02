/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name  : var_define.h
 Description: 存储器EEPROM的地址宏定义 把事件从这个文件移出 优化编译速度
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2011-06-27      azh           the original version
******************************************************************************/

#ifndef __VAR_DEFINE_H
#define __VAR_DEFINE_H


//----------------------------- macro definition ------------------------------
#define EEPROM_HEAD_ADDR        0x61000000          //定义EEPROM的逻辑起始地址
#define EEPROM_CAPACITY         (64 * 1024)         //定义EEPROM的容量64KBytes
#define EEPROM_LAST_WORD_ADDR   (EEPROM_HEAD_ADDR + EEPROM_CAPACITY - 5)//最后1个字作为全局标识存放地址
#define EEPROM_GFLG_ADDR        EEPROM_LAST_WORD_ADDR       //全局条件标识(U32)存储地址
#define OC_COVER_FLG            (1 << 0)            //bit0:开关表盖标识:  0 盖上表盖,1 打开表盖;
#define OC_TMNBX_FLG            (1 << 1)            //bit1:开关端钮盒标识:0 盖上端钮盒,1 打开端钮盒;
//bit2~31:保留,待扩充应用
#define EEPROM_ADDR                 0xa0//0xa2                //EEPROM slave address


#define RATE_NUM                    4                   //费率数
#define ENERGY_LEN                  4                   //单个电能量数据的长度
#define CRC_LEN                     2                   //CRC校验的长度

//条件编译宏定义
//#define LINE_LOSS                                         //若有线损宏定义，则处理铜损、铁损数据默认为不处理
//#define PHASE_3_LINE_4                                    //METER_TYPE定义,默认为PHASE_3_LINE_3
//-----------------------------------------------------------------------------
// 合相电能量的宏定义
//-----------------------------------------------------------------------------
#define TOTAL_PHASE_ENERGY_LEN              ((RATE_NUM + 1) * ENERGY_LEN + CRC_LEN)                             //合相电能量数据集合的长度

//关联、基波、谐波电能量的宏定义
//#ifdef LINE_LOSS//LINE_LOSS
#define RELATED_ENERGY_LEN                  (7 * ENERGY_LEN)//包含铜损、铁损
//#else   //LINE_LOSS
//#define RELATED_ENERGY_LEN                  (5 * ENERGY_LEN)//不包含铜损、铁损
//#ifdef  //LINE_LOSS
//-----------------------------------------------------------------------------
// 分相电能量的宏定义
//-----------------------------------------------------------------------------
#define SINGLE_PHASE_ENERGY_LEN             (10 * ENERGY_LEN + CRC_LEN)                                     //分相电能量数据集合的长度
#define SINGLE_PHASE_ALL_ENERGY_LEN         (SINGLE_PHASE_ENERGY_LEN + RELATED_ENERGY_LEN)                  //分相所有电能量数据集合的长度
/*
//-----------------------------------------------------------------------------
// 合相需量的宏定义
//-----------------------------------------------------------------------------
//#define DEMAND_LEN                              (DEMAND_JUST_LEN+DEMAND_HM_LEN+DEMAND_YMD_LEN)//07规约单个需量数据的长度
//#define DEMAND_JUST_LEN                         3                   //仅仅是需量数据用于显示
//#define DEMAND_HM_LEN                           2                   //需量数据的时分用于显示
//#define DEMAND_YMD_LEN                          3                   //需量数据的年月日用于显示
//#define TOTAL_PHASE_DEMAND_LEN                  ((RATE_NUM + 1) * DEMAND_LEN)                                       //合相需量数据集合的长度

//-----------------------------------------------------------------------------
// 分相需量的宏定义
//-----------------------------------------------------------------------------
//#define SINGLE_PHASE_DEMAND_LEN                 (10 * DEMAND_LEN)                                                   //分相需量数据集合的长度



*/

//-----------------------------------------------------------------------------
// 电能量的宏定义中间不能夹杂其他数据的宏定义，不能颠倒顺序，否则造成程序寻址出错
// COMB_ACTIVE_ENERGY_ADDR -> PHASE_C_ENERGY_ADDR
// 4字节二进制补码格式，低位在前
//-----------------------------------------------------------------------------
// 合相电能量的宏定义
//-----------------------------------------------------------------------------
//#define RATE_NUM                    4                   //费率数
//#define ENERGY_LEN                  4                   //单个电能量数据的长度
//#define CRC_LEN                     2                   //CRC校验的长度
//#define TOTAL_PHASE_ENERGY_LEN              ((RATE_NUM + 1) * ENERGY_LEN + CRC_LEN)                             //合相电能量数据集合的长度
//总
//费率1
//费率2
//...
//费率n(n取决于RATE_NUM)
//CRC(2Bytes)

#define ENG_STORE_START_ADDR                EEPROM_HEAD_ADDR
#define ENG_COMPOSP_00000000_00_ADDR        ENG_STORE_START_ADDR//组合有功电能量
#define ENG_COMPOSP_00000000_00_LEN         ENERGY_LEN
#define ENG_COMPOSP_00000000_TOT_LEN        TOTAL_PHASE_ENERGY_LEN//+2B CRC

#define ENG_POSP_00010000_00_ADDR           (ENG_COMPOSP_00000000_00_ADDR+ENG_COMPOSP_00000000_TOT_LEN)//正向有功电能量
#define ENG_POSP_00010000_00_LEN            ENERGY_LEN
#define ENG_POSP_00010000_TOT_LEN           TOTAL_PHASE_ENERGY_LEN

#define ENG_NEGP_00020000_00_ADDR           (ENG_POSP_00010000_00_ADDR+ENG_POSP_00010000_TOT_LEN)//反向有功电能量
#define ENG_NEGP_00020000_00_LEN            ENERGY_LEN
#define ENG_NEGP_00020000_TOT_LEN           TOTAL_PHASE_ENERGY_LEN

#define ENG_COMQ1_00030000_00_ADDR          (ENG_NEGP_00020000_00_ADDR+ENG_NEGP_00020000_TOT_LEN)   //组合1无功电能量
#define ENG_COMQ1_00030000_00_LEN           ENERGY_LEN
#define ENG_COMQ1_00030000_TOT_LEN          TOTAL_PHASE_ENERGY_LEN

#define ENG_COMQ2_00040000_00_ADDR          (ENG_COMQ1_00030000_00_ADDR+ENG_COMQ1_00030000_TOT_LEN)  //组合2无功电能量
#define ENG_COMQ2_00040000_00_LEN           ENERGY_LEN
#define ENG_COMQ2_00040000_TOT_LEN          TOTAL_PHASE_ENERGY_LEN

#define ENG_REA1Q_00050000_00_ADDR          (ENG_COMQ2_00040000_00_ADDR+ENG_COMQ2_00040000_TOT_LEN) //象限1无功电能量
#define ENG_REA1Q_00050000_00_LEN           ENERGY_LEN
#define ENG_REA1Q_00050000_TOT_LEN          TOTAL_PHASE_ENERGY_LEN

#define ENG_REA2Q_00060000_00_ADDR          (ENG_REA1Q_00050000_00_ADDR+ENG_REA1Q_00050000_TOT_LEN) //象限2无功电能量
#define ENG_REA2Q_00060000_00_LEN           ENERGY_LEN
#define ENG_REA2Q_00060000_TOT_LEN          TOTAL_PHASE_ENERGY_LEN

#define ENG_REA3Q_00070000_00_ADDR          (ENG_REA2Q_00060000_00_ADDR+ENG_REA2Q_00060000_TOT_LEN) //象限3无功电能量
#define ENG_REA3Q_00070000_00_LEN           ENERGY_LEN
#define ENG_REA3Q_00070000_TOT_LEN          TOTAL_PHASE_ENERGY_LEN

#define ENG_REA4Q_00080000_00_ADDR          (ENG_REA3Q_00070000_00_ADDR+ENG_REA3Q_00070000_TOT_LEN) //象限4无功电能量
#define ENG_REA4Q_00080000_00_LEN           ENERGY_LEN
#define ENG_REA4Q_00080000_TOT_LEN          TOTAL_PHASE_ENERGY_LEN
//
#define ENG_POSS_00090000_00_ADDR           (ENG_REA4Q_00080000_00_ADDR+ENG_REA4Q_00080000_TOT_LEN) //正向视在电能量
#define ENG_POSS_00090000_00_LEN            ENERGY_LEN
#define ENG_POSS_00090000_TOT_LEN           TOTAL_PHASE_ENERGY_LEN

#define ENG_NEGS_000A0000_00_ADDR           (ENG_POSS_00090000_00_ADDR+ENG_POSS_00090000_TOT_LEN)   //反向视在电能量
#define ENG_NEGS_000A0000_00_LEN            ENERGY_LEN
#define ENG_NEGS_000A0000_TOT_LEN           TOTAL_PHASE_ENERGY_LEN

#define ENG_MERGED_PHASE_TOT_LEN            (TOTAL_PHASE_ENERGY_LEN * 11)

//-----------------------------------------------------------------------------
// 分相电能量的宏定义
//-----------------------------------------------------------------------------
//#define SINGLE_PHASE_ENERGY_LEN             (10 * ENERGY_LEN + CRC_LEN)                                     //分相电能量数据集合的长度
//#define SINGLE_PHASE_ALL_ENERGY_LEN         (SINGLE_PHASE_ENERGY_LEN + RELATED_ENERGY_LEN)                  //分相所有电能量数据集合的长度

//A相电能量
#define ENG_APOSP_00150000_00_ADDR          (ENG_NEGS_000A0000_00_ADDR + ENG_NEGS_000A0000_TOT_LEN)         //A相正向有功电能量
#define ENG_APOSP_00150000_00_LEN           ENERGY_LEN

#define ENG_ANEGP_00160000_00_ADDR          (ENG_APOSP_00150000_00_ADDR + ENG_APOSP_00150000_00_LEN)        //A相反向有功电能
#define ENG_ANEGP_00160000_00_LEN           ENERGY_LEN

#define ENG_ACOMQ1_00170000_00_ADDR         (ENG_ANEGP_00160000_00_ADDR + ENG_ANEGP_00160000_00_LEN)        //A相组合无功1电能
#define ENG_ACOMQ1_00170000_00_LEN          ENERGY_LEN

#define ENG_ACOMQ2_00180000_00_ADDR         (ENG_ACOMQ1_00170000_00_ADDR + ENG_ACOMQ1_00170000_00_LEN)      //A相组合无功2电能
#define ENG_ACOMQ2_00180000_00_LEN          ENERGY_LEN

#define ENG_AREA1Q_00190000_00_ADDR         (ENG_ACOMQ2_00180000_00_ADDR + ENG_ACOMQ2_00180000_00_LEN)      //A相第一象限无功电能
#define ENG_AREA1Q_00190000_00_LEN           ENERGY_LEN

#define ENG_AREA2Q_001A0000_00_ADDR         (ENG_AREA1Q_00190000_00_ADDR + ENG_AREA1Q_00190000_00_LEN)      //A相第二象限无功电能
#define ENG_AREA2Q_001A0000_00_LEN          ENERGY_LEN

#define ENG_AREA3Q_001B0000_00_ADDR          (ENG_AREA2Q_001A0000_00_ADDR + ENG_AREA2Q_001A0000_00_LEN)      //A相第三象限无功电能
#define ENG_AREA3Q_001B0000_00_LEN           ENERGY_LEN

#define ENG_AREA4Q_001C0000_00_ADDR          (ENG_AREA3Q_001B0000_00_ADDR + ENG_AREA3Q_001B0000_00_LEN)      //A相第四象限无功电能
#define ENG_AREA4Q_001C0000_00_LEN           ENERGY_LEN

#define ENG_APOSS_001D0000_00_ADDR          (ENG_AREA4Q_001C0000_00_ADDR + ENG_AREA4Q_001C0000_00_LEN)        //A相正向视在电能量
#define ENG_APOSS_001D0000_00_LEN           ENERGY_LEN

#define ENG_ANEGS_001E0000_00_ADDR          (ENG_APOSS_001D0000_00_ADDR + ENG_APOSS_001D0000_00_LEN)        //A相反向视在电能量
#define ENG_ANEGS_001E0000_00_LEN           ENERGY_LEN

//B相电能量
#ifdef PHASE_3_LINE_4
//#define ENG_BPOSP_00290000_00_ADDR          (ENG_ANEGS_001E0000_00_ADDR + ENG_ANEGS_001E0000_00_LEN)        //B相正向有功电能量
//#define ENG_BPOSP_00290000_00_LEN           ENERGY_LEN
//
//#define ENG_BNEGP_002A0000_00_ADDR          (ENG_BPOSP_00290000_00_ADDR + ENG_BPOSP_00290000_00_LEN)        //B相反向有功电能
//#define ENG_BNEGP_002A0000_00_LEN           ENERGY_LEN
//
//#define ENG_BCOMQ1_002B0000_00_ADDR         (ENG_BNEGP_002A0000_00_ADDR + ENG_BNEGP_002A0000_00_LEN)        //B相组合无功1电能
//#define ENG_BCOMQ1_002B0000_00_LEN          ENERGY_LEN
//
//#define ENG_BCOMQ2_002C0000_00_ADDR         (ENG_BCOMQ1_002B0000_00_ADDR + ENG_BCOMQ1_002B0000_00_LEN)      //B相组合无功2电能
//#define ENG_BCOMQ2_002C0000_00_LEN          ENERGY_LEN
//
//#define ENG_BREA1Q_002D0000_00_ADDR         (ENG_BCOMQ2_002C0000_00_ADDR + ENG_BCOMQ2_002C0000_00_LEN)      //B相第一象限无功电能
//#define ENG_BREA1Q_002D0000_00_LEN          ENERGY_LEN
//
//#define ENG_BREA2Q_002E0000_00_ADDR         (ENG_BREA1Q_002D0000_00_ADDR + ENG_BREA1Q_002D0000_00_LEN)      //B相第二象限无功电能
//#define ENG_BREA2Q_002E0000_00_LEN          ENERGY_LEN
//
//#define ENG_BREA3Q_002F0000_00_ADDR          (ENG_BREA2Q_002E0000_00_ADDR + ENG_BREA2Q_002E0000_00_LEN)      //B相第三象限无功电能
//#define ENG_BREA3Q_002F0000_00_LEN           ENERGY_LEN
//
//#define ENG_BREA4Q_00300000_00_ADDR          (ENG_BREA3Q_002F0000_00_ADDR + ENG_BREA3Q_002F0000_00_LEN)      //B相第四象限无功电能
//#define ENG_BREA4Q_00300000_00_LEN           ENERGY_LEN
//
//#define ENG_BPOSS_00310000_00_ADDR          (ENG_BREA4Q_00300000_00_ADDR + ENG_BREA4Q_00300000_00_LEN)        //B相正向视在电能量
//#define ENG_BPOSS_00310000_00_LEN           ENERGY_LEN
//
//#define ENG_BNEGS_00320000_00_ADDR          (ENG_BPOSS_00310000_00_ADDR + ENG_BPOSS_00310000_00_LEN)        //B相反向视在电能量
//#define ENG_BNEGS_00320000_00_LEN           ENERGY_LEN
//
////C相电能量
//#define ENG_CPOSP_003D0000_00_ADDR          (ENG_BNEGS_00320000_00_ADDR + ENG_BNEGS_00320000_00_LEN)        //C相正向有功电能量
//#define ENG_CPOSP_003D0000_00_LEN           ENERGY_LEN
#else   //PHASE_3_LINE_4
#define ENG_CPOSP_003D0000_00_ADDR          (ENG_ANEGS_001E0000_00_ADDR + ENG_ANEGS_001E0000_00_LEN)        //C相正向有功电能量
#define ENG_CPOSP_003D0000_00_LEN           ENERGY_LEN
#endif  //PHASE_3_LINE_4
#define ENG_CNEGP_003E0000_00_ADDR          (ENG_CPOSP_003D0000_00_ADDR + ENG_CPOSP_003D0000_00_LEN)        //C相反向有功电能
#define ENG_CNEGP_003E0000_00_LEN           ENERGY_LEN

#define ENG_CCOMQ1_003F0000_00_ADDR         (ENG_CNEGP_003E0000_00_ADDR + ENG_CNEGP_003E0000_00_LEN)        //C相组合无功1电能
#define ENG_CCOMQ1_003F0000_00_LEN          ENERGY_LEN

#define ENG_CCOMQ2_00400000_00_ADDR         (ENG_CCOMQ1_003F0000_00_ADDR + ENG_CCOMQ1_003F0000_00_LEN)      //C相组合无功2电能
#define ENG_CCOMQ2_00400000_00_LEN          ENERGY_LEN

#define ENG_CREA1Q_00410000_00_ADDR         (ENG_CCOMQ2_00400000_00_ADDR + ENG_CCOMQ2_00400000_00_LEN)      //C相第一象限无功电能
#define ENG_CREA1Q_00410000_00_LEN          ENERGY_LEN

#define ENG_CREA2Q_00420000_00_ADDR         (ENG_CREA1Q_00410000_00_ADDR + ENG_CREA1Q_00410000_00_LEN)      //C相第二象限无功电能
#define ENG_CREA2Q_00420000_00_LEN          ENERGY_LEN

#define ENG_CREA3Q_00430000_00_ADDR         (ENG_CREA2Q_00420000_00_ADDR + ENG_CREA2Q_00420000_00_LEN)      //C相第三象限无功电能
#define ENG_CREA3Q_00430000_00_LEN          ENERGY_LEN

#define ENG_CREA4Q_00440000_00_ADDR         (ENG_CREA3Q_00430000_00_ADDR + ENG_CREA3Q_00430000_00_LEN)        //C相第四象限无功电能
#define ENG_CREA4Q_00440000_00_LEN          ENERGY_LEN

#define ENG_CPOSS_00450000_00_ADDR          (ENG_CREA4Q_00440000_00_ADDR + ENG_CREA4Q_00440000_00_LEN)        //C相正向视在电能量
#define ENG_CPOSS_00450000_00_LEN           ENERGY_LEN

#define ENG_CNEGS_00460000_00_ADDR          (ENG_CPOSS_00450000_00_ADDR + ENG_CPOSS_00450000_00_LEN)          //C相反向视在电能量
#define ENG_CNEGS_00460000_00_LEN           ENERGY_LEN
#define ENG_CNEGS_00460000_TOT_LEN          (ENERGY_LEN + CRC_LEN)
#ifdef PHASE_3_LINE_4
//#define ENG_SEPARATE_PHASE_TOT_LEN          ((ENERGY_LEN * 10) * 3 + CRC_LEN)
#else   //PHASE_3_LINE_4
#define ENG_SEPARATE_PHASE_TOT_LEN          ((ENERGY_LEN * 10) * 2 + CRC_LEN)
#endif  //PHASE_3_LINE_4

//-----------------------------------------------------------------------------
// 关联、基波、谐波电能量的宏定义(合、分相):厂家扩展DI[]: 0x070001-01-09-00
// 关联总电能
// 正向有功基波总电能
// 反向有功基波总电能
// 正向有功谐波总电能
// 反向有功谐波总电能

// 关联电能_A相
// 正向有功基波总电能_A相
// 反向有功基波总电能_A相
// 正向有功谐波总电能_A相
// 反向有功谐波总电能_A相

// 关联电能_B相
// 正向有功基波总电能_B相
// 反向有功基波总电能_B相
// 正向有功谐波总电能_B相
// 反向有功谐波总电能_B相

// 关联电能_C相
// 正向有功基波总电能_C相
// 反向有功基波总电能_C相
// 正向有功谐波总电能_C相
// 反向有功谐波总电能_C相

// CRC(2Bytes)
//-----------------------------------------------------------------------------
//合相
#define ENG_RELATOTP_00800000_00_ADDR       (ENG_CNEGS_00460000_00_ADDR+ENG_CNEGS_00460000_TOT_LEN)         //关联电能量
#define ENG_RELATOTP_00800000_00_LEN        ENERGY_LEN
#define ENG_RELAPOSBP_00810000_00_ADDR      (ENG_RELATOTP_00800000_00_ADDR+ENG_RELATOTP_00800000_00_LEN)    //正向基波有功电能量
#define ENG_RELAPOSBP_00810000_00_LEN       ENERGY_LEN
#define ENG_RELANEGBP_00820000_00_ADDR      (ENG_RELAPOSBP_00810000_00_ADDR+ENG_RELAPOSBP_00810000_00_LEN)  //反向基波有功电能量
#define ENG_RELANEGBP_00820000_00_LEN       ENERGY_LEN
#define ENG_RELAPOSHP_00830000_00_ADDR      (ENG_RELANEGBP_00820000_00_ADDR+ENG_RELANEGBP_00820000_00_LEN)  //正向谐波有功电能量
#define ENG_RELAPOSHP_00830000_00_LEN       ENERGY_LEN
#define ENG_RELANEGHP_00840000_00_ADDR      (ENG_RELAPOSHP_00830000_00_ADDR+ENG_RELAPOSHP_00830000_00_LEN)  //反向谐波有功电能量
#define ENG_RELANEGHP_00840000_00_LEN       ENERGY_LEN
//#ifdef LINE_LOSS//LINE_LOSS
#define ENG_RELACOPCP_00850000_00_ADDR      (ENG_RELANEGHP_00840000_00_ADDR+ENG_RELANEGHP_00840000_00_LEN)  //铜损有功总电能补偿量
#define ENG_RELACOPCP_00850000_00_LEN       ENERGY_LEN
#define ENG_RELAIROCP_00860000_00_ADDR      (ENG_RELACOPCP_00850000_00_ADDR+ENG_RELACOPCP_00850000_00_LEN)  //铁损有功总电能补偿量
#define ENG_RELAIROCP_00860000_00_LEN       ENERGY_LEN
#define ENG_RELATOTP_00800000_TOT_LEN       RELATED_ENERGY_LEN                                              //关联电能量数据集合的长度

//A相
#define ENG_ARELATOTP_00940000_00_ADDR      (ENG_RELAIROCP_00860000_00_ADDR + ENG_RELAIROCP_00860000_00_LEN)    //A相关联电能量
#define ENG_ARELATOTP_00940000_00_LEN       ENERGY_LEN
//#else   //LINE_LOSS
//#define ENG_ARELATOTP_00940000_00_ADDR      (ENG_RELANEGHP_00840000_00_ADDR + ENG_RELANEGHP_00840000_00_LEN)    //A相关联电能量
//#define ENG_ARELATOTP_00940000_00_LEN       ENERGY_LEN
//#endif  //LINE_LOSS
#define ENG_ARELAPOSBP_00950000_00_ADDR     (ENG_ARELATOTP_00940000_00_ADDR + ENG_ARELATOTP_00940000_00_LEN)    //A相正向基波有功电能量
#define ENG_ARELAPOSBP_00950000_00_LEN      ENERGY_LEN
#define ENG_ARELANEGBP_00960000_00_ADDR     (ENG_ARELAPOSBP_00950000_00_ADDR + ENG_ARELAPOSBP_00950000_00_LEN)  //A相反向基波有功电能量
#define ENG_ARELANEGBP_00960000_00_LEN      ENERGY_LEN
#define ENG_ARELAPOSHP_00970000_00_ADDR     (ENG_ARELANEGBP_00960000_00_ADDR + ENG_ARELANEGBP_00960000_00_LEN)  //A相正向谐波有功电能量
#define ENG_ARELAPOSHP_00970000_00_LEN      ENERGY_LEN
#define ENG_ARELANEGHP_00980000_00_ADDR     (ENG_ARELAPOSHP_00970000_00_ADDR + ENG_ARELAPOSHP_00970000_00_LEN)  //A相反向谐波有功电能量
#define ENG_ARELANEGHP_00980000_00_LEN      ENERGY_LEN
//#ifdef LINE_LOSS//LINE_LOSS
#define ENG_ARELACOPCP_00990000_00_ADDR     (ENG_ARELANEGHP_00980000_00_ADDR + ENG_ARELANEGHP_00980000_00_LEN)  //A相铜损有功电能补偿量
#define ENG_ARELACOPCP_00990000_00_LEN      ENERGY_LEN
#define ENG_ARELAIROCP_009A0000_00_ADDR     (ENG_ARELACOPCP_00990000_00_ADDR + ENG_ARELACOPCP_00990000_00_LEN)  //A相铁损有功电能补偿量
#define ENG_ARELAIROCP_009A0000_00_LEN      ENERGY_LEN
//#endif  //LINE_LOSS
#define ENG_ARELATOTP_00940000_TOT_LEN      RELATED_ENERGY_LEN                                              //A相关联电能量数据集合的长度

//B相
#ifdef PHASE_3_LINE_4
////#ifdef LINE_LOSS//LINE_LOSS
//#define ENG_BRELATOTP_00A80000_00_ADDR      (ENG_ARELAIROCP_009A0000_00_ADDR + ENG_ARELAIROCP_009A0000_00_LEN) //B相关联电能量
//#define ENG_BRELATOTP_00A80000_00_LEN       ENERGY_LEN
////#else   //LINE_LOSS
////#define ENG_BRELATOTP_00A80000_00_ADDR      (ENG_ARELANEGHP_00980000_00_ADDR + ENG_ARELANEGHP_00980000_00_LEN) //B相关联电能量
////#define ENG_BRELATOTP_00A80000_00_LEN       ENERGY_LEN
////#endif   //LINE_LOSS
//#define ENG_BRELAPOSBP_00A90000_00_ADDR     (ENG_BRELATOTP_00A80000_00_ADDR + ENG_BRELATOTP_00A80000_00_LEN)    //B相正向基波有功电能量
//#define ENG_BRELAPOSBP_00A90000_00_LEN      ENERGY_LEN
//#define ENG_BRELANEGBP_00AA0000_00_ADDR     (ENG_BRELAPOSBP_00A90000_00_ADDR + ENG_BRELAPOSBP_00A90000_00_LEN)  //B相反向基波有功电能量
//#define ENG_BRELANEGBP_00AA0000_00_LEN      ENERGY_LEN
//#define ENG_BRELAPOSHP_00AB0000_00_ADDR     (ENG_BRELANEGBP_00AA0000_00_ADDR + ENG_BRELANEGBP_00AA0000_00_LEN)  //B相正向谐波有功电能量
//#define ENG_BRELAPOSHP_00AB0000_00_LEN      ENERGY_LEN
//#define ENG_BRELANEGHP_00AC0000_00_ADDR     (ENG_BRELAPOSHP_00AB0000_00_ADDR + ENG_BRELAPOSHP_00AB0000_00_LEN)  //B相反向谐波有功电能量
//#define ENG_BRELANEGHP_00AC0000_00_LEN      ENERGY_LEN
////#ifdef LINE_LOSS//LINE_LOSS
//#define ENG_BRELACOPCP_00AD0000_00_ADDR     (ENG_BRELANEGHP_00AC0000_00_ADDR + ENG_BRELANEGHP_00AC0000_00_LEN)  //B相铜损有功电能补偿量
//#define ENG_BRELACOPCP_00AD0000_00_LEN      ENERGY_LEN
//#define ENG_BRELAIROCP_00AE0000_00_ADDR     (ENG_BRELACOPCP_00AD0000_00_ADDR + ENG_BRELACOPCP_00AD0000_00_LEN)  //B相铁损有功电能补偿量
//#define ENG_BRELAIROCP_00AE0000_00_LEN      ENERGY_LEN
////#endif   //LINE_LOSS
//#define ENG_BRELATOTP_00A80000_TOT_LEN      RELATED_ENERGY_LEN                                                  //B相关联电能量数据集合的长度
//
////C相
////#ifdef LINE_LOSS//LINE_LOSS
//#define ENG_CRELATOTP_00BC0000_00_ADDR      (ENG_BRELAIROCP_00AE0000_00_ADDR + ENG_BRELAIROCP_00AE0000_00_LEN)  //C相关联电能量
//#define ENG_CRELATOTP_00BC0000_00_LEN       ENERGY_LEN
////#else   //LINE_LOSS
////#define ENG_CRELATOTP_00BC0000_00_ADDR      (ENG_BRELANEGHP_00AC0000_00_ADDR + ENG_BRELANEGHP_00AC0000_00_LEN)  //C相关联电能量
////#define ENG_CRELATOTP_00BC0000_00_LEN       ENERGY_LEN
////#endif  //LINE_LOSS

#else   //PHASE_3_LINE_4

//#ifdef LINE_LOSS//LINE_LOSS
#define ENG_CRELATOTP_00BC0000_00_ADDR      (ENG_ARELAIROCP_009A0000_00_ADDR + ENG_ARELAIROCP_009A0000_00_LEN)  //C相关联电能量
#define ENG_CRELATOTP_00BC0000_00_LEN       ENERGY_LEN
//#else   //LINE_LOSS
//#define ENG_CRELATOTP_00BC0000_00_ADDR      (ENG_ARELANEGHP_00980000_00_ADDR + ENG_ARELANEGHP_00980000_00_LEN)  //C相关联电能量
//#define ENG_CRELATOTP_00BC0000_00_LEN       ENERGY_LEN
//#endif  //LINE_LOSS

#endif  //PHASE_3_LINE_4

#define ENG_CRELAPOSBP_00BD0000_00_ADDR     (ENG_CRELATOTP_00BC0000_00_ADDR + ENG_CRELATOTP_00BC0000_00_LEN)    //C相正向基波有功电能量
#define ENG_CRELAPOSBP_00BD0000_00_LEN      ENERGY_LEN
#define ENG_CRELANEGBP_00BE0000_00_ADDR     (ENG_CRELAPOSBP_00BD0000_00_ADDR + ENG_CRELAPOSBP_00BD0000_00_LEN)  //C相反向基波有功电能量
#define ENG_CRELANEGBP_00BE0000_00_LEN      ENERGY_LEN
#define ENG_CRELAPOSHP_00BF0000_00_ADDR     (ENG_CRELANEGBP_00BE0000_00_ADDR + ENG_CRELANEGBP_00BE0000_00_LEN)  //C相正向谐波有功电能量
#define ENG_CRELAPOSHP_00BF0000_00_LEN      ENERGY_LEN
#define ENG_CRELANEGHP_00C00000_00_ADDR     (ENG_CRELAPOSHP_00BF0000_00_ADDR + ENG_CRELAPOSHP_00BF0000_00_LEN)  //C相反向谐波有功电能量
#define ENG_CRELANEGHP_00C00000_00_LEN      ENERGY_LEN
#define ENG_CRELANEGHP_00C00000_TOT_LEN     (ENERGY_LEN + CRC_LEN)
//#ifdef LINE_LOSS//LINE_LOSS
#define ENG_CRELACOPCP_00C10000_00_ADDR     (ENG_CRELANEGHP_00C00000_00_ADDR + ENG_CRELANEGHP_00C00000_00_LEN)  //C相铜损有功电能补偿量
#define ENG_CRELACOPCP_00C10000_00_LEN      ENERGY_LEN
#define ENG_CRELAIROCP_00C20000_00_ADDR     (ENG_CRELACOPCP_00C10000_00_ADDR + ENG_CRELACOPCP_00C10000_00_LEN)  //C相铁损有功电能补偿量
#define ENG_CRELAIROCP_00C20000_00_LEN      ENERGY_LEN
#define ENG_CRELAIROCP_00C20000_TOT_LEN     (ENERGY_LEN + CRC_LEN)
//#endif  //LINE_LOSS
#define ENG_CRELATOTP_00BC0000_TOT_LEN      RELATED_ENERGY_LEN                                              //C相关联电能量数据集合的长度
#ifdef PHASE_3_LINE_4
//#define ENG_ALL_RELATOTP_TOT_LEN            (RELATED_ENERGY_LEN * 4 + CRC_LEN)  //合、分相关联电能量数据块总长度
#else   //PHASE_3_LINE_4
#define ENG_ALL_RELATOTP_TOT_LEN            (RELATED_ENERGY_LEN * 3 + CRC_LEN)  //合、分相关联电能量数据块总长度
#endif  //PHASE_3_LINE_4

#define ENG_RAM_ARRAY_MAX   (ENG_MERGED_PHASE_TOT_LEN + ENG_SEPARATE_PHASE_TOT_LEN + ENG_ALL_RELATOTP_TOT_LEN)//当前电量RAM缓存容量[每数据块CRC占1个字]


//上1~12结算日需要冻结 开始[暂未应用到DLT645]
#define ENG_LASTXMONTH_00000000_00_OFFSET_MAXNUM    12  //
//#ifdef LINE_LOSS//LINE_LOSS
#define ENG_LASTXMONTH_00000000_00_OFFSET_ADDR      (ENG_CRELAIROCP_00C20000_00_ADDR+ENG_CRELAIROCP_00C20000_TOT_LEN)//次数指针保存的地址[暂不考虑铜铁损数据项]
//#else   //LINE_LOSS
//#define ENG_LASTXMONTH_00000000_00_OFFSET_ADDR      (ENG_CRELANEGHP_00C00000_00_ADDR+ENG_CRELANEGHP_00C00000_TOT_LEN)//次数指针保存的地址[暂不考虑铜铁损数据项]
//#endif  //LINE_LOSS
#define ENG_LASTXMONTH_00000000_00_OFFSET_LEN       3 + CRC_LEN  //次数指针长度[CRC用于判别指针的有效性]
#define ENG_LASTXMONTH_00000000_00_OFFSET_BLOCKLEN  ENG_RAM_ARRAY_MAX//单次电量冻结所有数据长度
#define ENG_LASTXMONTH_00000000_00_HEAD_ADDR        (ENG_LASTXMONTH_00000000_00_OFFSET_ADDR+ENG_LASTXMONTH_00000000_00_OFFSET_LEN)
#define ENG_LASTXMONTH_00000000_00_END_ADDR         (ENG_LASTXMONTH_00000000_00_HEAD_ADDR+ENG_LASTXMONTH_00000000_00_OFFSET_BLOCKLEN*ENG_LASTXMONTH_00000000_00_OFFSET_MAXNUM)
//上1~12结算日需要冻结 结束


/*
//---------------------------- 在使用new_convert工具看宏地址时需要改动, 确定具体是哪种编译
//#if METER_TYPE == PHASE_3_LINE_4
//#define PHASE_B_ENERGY_ADDR                 (PHASE_A_RELATED_ENERGY_ADDR + RELATED_ENERGY_LEN)                  //B相电能量
//#define PHASE_B_RELATED_ENERGY_ADDR         (PHASE_B_ENERGY_ADDR + SINGLE_PHASE_ENERGY_LEN)                     //B相关联电能量
//#define PHASE_C_ENERGY_ADDR                 (PHASE_B_RELATED_ENERGY_ADDR + RELATED_ENERGY_LEN)                  //C相电能量
//#define PHASE_C_RELATED_ENERGY_ADDR         (PHASE_C_ENERGY_ADDR + SINGLE_PHASE_ENERGY_LEN)                     //C相关联电能量
//#else//METER_TYPE == PHASE_3_LINE_4
//#define PHASE_C_ENERGY_ADDR                 (PHASE_A_RELATED_ENERGY_ADDR + RELATED_ENERGY_LEN)                  //C相电能量
//#define PHASE_C_RELATED_ENERGY_ADDR         (PHASE_C_ENERGY_ADDR + SINGLE_PHASE_ENERGY_LEN)                     //C相关联电能量
//#endif//METER_TYPE == PHASE_3_LINE_4

//-----------------------------------------------------------------------------
// 电能量备份的宏定义，数据排列与当前电量的排列是一样的
//-----------------------------------------------------------------------------
//#define ENERGY_BACKUP_NUM                   3           //电能量备份的个数
//#define ENERGY_BACKUP_LEN                   (PHASE_C_RELATED_ENERGY_ADDR + RELATED_ENERGY_LEN - COMB_ACTIVE_ENERGY_ADDR)
//
//#define ENERGY_BACKUP_POINTER               (PHASE_C_RELATED_ENERGY_ADDR + RELATED_ENERGY_LEN)                  //当前电能量备份的指针，总是指向最新记录后的一个空记录或最老的记录
//#define ENERGY_BACKUP_POINTER_LEN           1
//#define ENERGY_BACKUP_ADDR                  (ENERGY_BACKUP_POINTER + ENERGY_BACKUP_POINTER_LEN)                 //电能量备份区
*/

//-----------------------------------------------------------------------------
// 存储格式：发生时间 + 最大需量
// 发生时间数据：mm（分）hh（时）DD（日）MM（月）YY（年），十进制格式
// 最大需量数据：3字节十进制格式，低位在前
//-----------------------------------------------------------------------------
// 合相需量的宏定义
//-----------------------------------------------------------------------------
#define DEMAND_JUST_LEN                         3                   //仅仅是需量数据用于显示
#define DEMAND_HM_LEN                           2                   //需量数据的时分用于显示
#define DEMAND_YMD_LEN                          3                   //需量数据的年月日用于显示
#define DEMAND_LEN                              (DEMAND_JUST_LEN+DEMAND_HM_LEN+DEMAND_YMD_LEN)//07规约单个需量数据的长度
#define TOTAL_PHASE_DEMAND_LEN                  ((RATE_NUM + 1) * DEMAND_LEN)  //合相需量数据集合的长度
//总
//费率1
//费率2
//...
//费率n(n取决于RATE_NUM)
#define DMD_STORE_START_ADDR                ENG_LASTXMONTH_00000000_00_END_ADDR //需量基址
#define DMD_POSP_01010000_00_ADDR           (DMD_STORE_START_ADDR)              //正向有功需量
#define DMD_POSP_01010000_00_LEN            DEMAND_JUST_LEN
#define DMD_POSP_01010000_01_ADDR           (DMD_POSP_01010000_00_ADDR+DMD_POSP_01010000_00_LEN)//正向有功需量:时分
#define DMD_POSP_01010000_01_LEN            DEMAND_HM_LEN
#define DMD_POSP_01010000_02_ADDR           (DMD_POSP_01010000_01_ADDR+DMD_POSP_01010000_01_LEN)//正向有功需量:年月日
#define DMD_POSP_01010000_02_LEN            DEMAND_YMD_LEN
#define DMD_POSP_01010000_LEN               DEMAND_LEN  //8
#define DMD_POSP_01010000_TOT_LEN           TOTAL_PHASE_DEMAND_LEN

#define DMD_NEGP_01020000_00_ADDR           (DMD_POSP_01010000_00_ADDR+DMD_POSP_01010000_TOT_LEN)//反向有功需量
#define DMD_NEGP_01020000_00_LEN            DEMAND_JUST_LEN
#define DMD_NEGP_01020000_01_ADDR           (DMD_NEGP_01020000_00_ADDR+DMD_NEGP_01020000_00_LEN)//反向有功需量:时分
#define DMD_NEGP_01020000_01_LEN            DEMAND_HM_LEN
#define DMD_NEGP_01020000_02_ADDR           (DMD_NEGP_01020000_01_ADDR+DMD_NEGP_01020000_01_LEN)//反向有功需量:年月日
#define DMD_NEGP_01020000_02_LEN            DEMAND_YMD_LEN
#define DMD_NEGP_01020000_LEN               DEMAND_LEN  //8
#define DMD_NEGP_01020000_TOT_LEN           TOTAL_PHASE_DEMAND_LEN

#define DMD_COMQ1_01030000_00_ADDR          (DMD_NEGP_01020000_00_ADDR+DMD_NEGP_01020000_TOT_LEN)//组合1无功需量
#define DMD_COMQ1_01030000_00_LEN           DEMAND_JUST_LEN
#define DMD_COMQ1_01030000_01_ADDR          (DMD_COMQ1_01030000_00_ADDR+DMD_COMQ1_01030000_00_LEN)//组合1无功需量:时分
#define DMD_COMQ1_01030000_01_LEN           DEMAND_HM_LEN
#define DMD_COMQ1_01030000_02_ADDR          (DMD_COMQ1_01030000_01_ADDR+DMD_COMQ1_01030000_01_LEN)//组合1无功需量:年月日
#define DMD_COMQ1_01030000_02_LEN           DEMAND_YMD_LEN
#define DMD_COMQ1_01030000_LEN              DEMAND_LEN  //8
#define DMD_COMQ1_01030000_TOT_LEN          TOTAL_PHASE_DEMAND_LEN

#define DMD_COMQ2_01040000_00_ADDR          (DMD_COMQ1_01030000_00_ADDR+DMD_COMQ1_01030000_TOT_LEN)//组合2无功需量
#define DMD_COMQ2_01040000_00_LEN           DEMAND_JUST_LEN
#define DMD_COMQ2_01040000_01_ADDR          (DMD_COMQ2_01040000_00_ADDR+DMD_COMQ2_01040000_00_LEN)//组合2无功需量:时分
#define DMD_COMQ2_01040000_01_LEN           DEMAND_HM_LEN
#define DMD_COMQ2_01040000_02_ADDR          (DMD_COMQ2_01040000_01_ADDR+DMD_COMQ2_01040000_01_LEN)//组合2无功需量:年月日
#define DMD_COMQ2_01040000_02_LEN           DEMAND_YMD_LEN
#define DMD_COMQ2_01040000_LEN              DEMAND_LEN  //8
#define DMD_COMQ2_01040000_TOT_LEN          TOTAL_PHASE_DEMAND_LEN

#define DMD_REQ1Q_01050000_00_ADDR          (DMD_COMQ2_01040000_00_ADDR+DMD_COMQ2_01040000_TOT_LEN)//象限1无功需量
#define DMD_REQ1Q_01050000_00_LEN           DEMAND_JUST_LEN
#define DMD_REQ1Q_01050000_01_ADDR          (DMD_REQ1Q_01050000_00_ADDR+DMD_REQ1Q_01050000_00_LEN)//象限1无功需量:时分
#define DMD_REQ1Q_01050000_01_LEN           DEMAND_HM_LEN
#define DMD_REQ1Q_01050000_02_ADDR          (DMD_REQ1Q_01050000_01_ADDR+DMD_REQ1Q_01050000_01_LEN)//象限1无功需量:年月日
#define DMD_REQ1Q_01050000_02_LEN           DEMAND_YMD_LEN
#define DMD_REQ1Q_01050000_LEN              DEMAND_LEN  //8
#define DMD_REQ1Q_01050000_TOT_LEN          TOTAL_PHASE_DEMAND_LEN

#define DMD_REQ2Q_01060000_00_ADDR          (DMD_REQ1Q_01050000_00_ADDR+DMD_REQ1Q_01050000_TOT_LEN)//象限2无功需量
#define DMD_REQ2Q_01060000_00_LEN           DEMAND_JUST_LEN
#define DMD_REQ2Q_01060000_01_ADDR          (DMD_REQ2Q_01060000_00_ADDR+DMD_REQ2Q_01060000_00_LEN)//象限2无功需量:时分
#define DMD_REQ2Q_01060000_01_LEN           DEMAND_HM_LEN
#define DMD_REQ2Q_01060000_02_ADDR          (DMD_REQ2Q_01060000_01_ADDR+DMD_REQ2Q_01060000_01_LEN)//象限2无功需量:年月日
#define DMD_REQ2Q_01060000_02_LEN           DEMAND_YMD_LEN
#define DMD_REQ2Q_01060000_LEN              DEMAND_LEN  //8
#define DMD_REQ2Q_01060000_TOT_LEN          TOTAL_PHASE_DEMAND_LEN

#define DMD_REQ3Q_01070000_00_ADDR          (DMD_REQ2Q_01060000_00_ADDR+DMD_REQ2Q_01060000_TOT_LEN)//象限3无功需量
#define DMD_REQ3Q_01070000_00_LEN           DEMAND_JUST_LEN
#define DMD_REQ3Q_01070000_01_ADDR          (DMD_REQ3Q_01070000_00_ADDR+DMD_REQ3Q_01070000_00_LEN)//象限3无功需量:时分
#define DMD_REQ3Q_01070000_01_LEN           DEMAND_HM_LEN
#define DMD_REQ3Q_01070000_02_ADDR          (DMD_REQ3Q_01070000_01_ADDR+DMD_REQ3Q_01070000_01_LEN)//象限3无功需量:年月日
#define DMD_REQ3Q_01070000_02_LEN           DEMAND_YMD_LEN
#define DMD_REQ3Q_01070000_LEN              DEMAND_LEN  //8
#define DMD_REQ3Q_01070000_TOT_LEN          TOTAL_PHASE_DEMAND_LEN

#define DMD_REQ4Q_01080000_00_ADDR          (DMD_REQ3Q_01070000_00_ADDR+DMD_REQ3Q_01070000_TOT_LEN)//象限4无功需量
#define DMD_REQ4Q_01080000_00_LEN           DEMAND_JUST_LEN
#define DMD_REQ4Q_01080000_01_ADDR          (DMD_REQ4Q_01080000_00_ADDR+DMD_REQ4Q_01080000_00_LEN)//象限4无功需量:时分
#define DMD_REQ4Q_01080000_01_LEN           DEMAND_HM_LEN
#define DMD_REQ4Q_01080000_02_ADDR          (DMD_REQ4Q_01080000_01_ADDR+DMD_REQ4Q_01080000_01_LEN)//象限4无功需量:年月日
#define DMD_REQ4Q_01080000_02_LEN           DEMAND_YMD_LEN
#define DMD_REQ4Q_01080000_LEN              DEMAND_LEN  //8
#define DMD_REQ4Q_01080000_TOT_LEN          TOTAL_PHASE_DEMAND_LEN

#define DMD_POSS_01090000_00_ADDR           (DMD_REQ4Q_01080000_00_ADDR+DMD_REQ4Q_01080000_TOT_LEN)//正向视在需量
#define DMD_POSS_01090000_00_LEN            DEMAND_JUST_LEN
#define DMD_POSS_01090000_01_ADDR           (DMD_POSS_01090000_00_ADDR+DMD_POSS_01090000_00_LEN)//正向视在需量:时分
#define DMD_POSS_01090000_01_LEN            DEMAND_HM_LEN
#define DMD_POSS_01090000_02_ADDR           (DMD_POSS_01090000_01_ADDR+DMD_POSS_01090000_01_LEN)//正向视在需量:年月日
#define DMD_POSS_01090000_02_LEN            DEMAND_YMD_LEN
#define DMD_POSS_01090000_LEN               DEMAND_LEN  //8
#define DMD_POSS_01090000_TOT_LEN           TOTAL_PHASE_DEMAND_LEN

#define DMD_NEGS_010A0000_00_ADDR           (DMD_POSS_01090000_00_ADDR+DMD_POSS_01090000_TOT_LEN)//反向视在需量
#define DMD_NEGS_010A0000_00_LEN            DEMAND_JUST_LEN
#define DMD_NEGS_010A0000_01_ADDR           (DMD_NEGS_010A0000_00_ADDR+DMD_NEGS_010A0000_00_LEN)//反向视在需量:时分
#define DMD_NEGS_010A0000_01_LEN            DEMAND_HM_LEN
#define DMD_NEGS_010A0000_02_ADDR           (DMD_NEGS_010A0000_01_ADDR+DMD_NEGS_010A0000_01_LEN)//反向视在需量:年月日
#define DMD_NEGS_010A0000_02_LEN            DEMAND_YMD_LEN
#define DMD_NEGS_010A0000_LEN               DEMAND_LEN  //8
#define DMD_NEGS_010A0000_TOT_LEN           TOTAL_PHASE_DEMAND_LEN

//-----------------------------------------------------------------------------
// 分相需量的宏定义
//-----------------------------------------------------------------------------
#define SINGLE_PHASE_DEMAND_LEN                 (10 * DEMAND_LEN)                                                   //分相需量数据集合的长度
//正向有功需量
//反向有功需量
//组合无功1需量
//组合无功2需量
//第一象限无功需量
//第二象限无功需量
//第三象限无功需量
//第四象限无功需量
//正向视在需量
//反向视在需量
//------------------------------------------------------------------------------
//A相需量
#define DMD_APOSP_01150000_00_ADDR          (DMD_NEGS_010A0000_00_ADDR+DMD_NEGS_010A0000_TOT_LEN)//A相正向有功需量
#define DMD_APOSP_01150000_00_LEN           DEMAND_JUST_LEN
#define DMD_APOSP_01150000_01_ADDR          (DMD_APOSP_01150000_00_ADDR+DMD_APOSP_01150000_00_LEN)//A相正向有功需量:时分
#define DMD_APOSP_01150000_01_LEN           DEMAND_HM_LEN
#define DMD_APOSP_01150000_02_ADDR          (DMD_APOSP_01150000_01_ADDR+DMD_APOSP_01150000_01_LEN)//A相正向有功需量:年月日
#define DMD_APOSP_01150000_02_LEN           DEMAND_YMD_LEN
#define DMD_APOSP_01150000_LEN              DEMAND_LEN  //8

#define DMD_ANEGP_01160000_00_ADDR          (DMD_APOSP_01150000_00_ADDR+DMD_APOSP_01150000_LEN)//A相反向有功需量
#define DMD_ANEGP_01160000_00_LEN           DEMAND_JUST_LEN
#define DMD_ANEGP_01160000_01_ADDR          (DMD_ANEGP_01160000_00_ADDR+DMD_ANEGP_01160000_00_LEN)//A相反向有功需量:时分
#define DMD_ANEGP_01160000_01_LEN           DEMAND_HM_LEN
#define DMD_ANEGP_01160000_02_ADDR          (DMD_ANEGP_01160000_01_ADDR+DMD_ANEGP_01160000_01_LEN)//A相反向有功需量:年月日
#define DMD_ANEGP_01160000_02_LEN           DEMAND_YMD_LEN
#define DMD_ANEGP_01160000_LEN              DEMAND_LEN  //8

#define DMD_ACOMQ1_01170000_00_ADDR         (DMD_ANEGP_01160000_00_ADDR+DMD_ANEGP_01160000_LEN)//A相组合1无功需量
#define DMD_ACOMQ1_01170000_00_LEN          DEMAND_JUST_LEN
#define DMD_ACOMQ1_01170000_01_ADDR         (DMD_ACOMQ1_01170000_00_ADDR+DMD_ACOMQ1_01170000_00_LEN)//A相组合1无功需量:时分
#define DMD_ACOMQ1_01170000_01_LEN          DEMAND_HM_LEN
#define DMD_ACOMQ1_01170000_02_ADDR         (DMD_ACOMQ1_01170000_01_ADDR+DMD_ACOMQ1_01170000_01_LEN)//A相组合1无功需量:年月日
#define DMD_ACOMQ1_01170000_02_LEN          DEMAND_YMD_LEN
#define DMD_ACOMQ1_01170000_LEN             DEMAND_LEN  //8

#define DMD_ACOMQ2_01180000_00_ADDR         (DMD_ACOMQ1_01170000_00_ADDR+DMD_ACOMQ1_01170000_LEN)//A相组合2无功需量
#define DMD_ACOMQ2_01180000_00_LEN          DEMAND_JUST_LEN
#define DMD_ACOMQ2_01180000_01_ADDR         (DMD_ACOMQ2_01180000_00_ADDR+DMD_ACOMQ2_01180000_00_LEN)//A相组合2无功需量:时分
#define DMD_ACOMQ2_01180000_01_LEN          DEMAND_HM_LEN
#define DMD_ACOMQ2_01180000_02_ADDR         (DMD_ACOMQ2_01180000_01_ADDR+DMD_ACOMQ2_01180000_01_LEN)//A相组合2无功需量:年月日
#define DMD_ACOMQ2_01180000_02_LEN          DEMAND_YMD_LEN
#define DMD_ACOMQ2_01180000_LEN             DEMAND_LEN  //8

#define DMD_AREQ1Q_01190000_00_ADDR         (DMD_ACOMQ2_01180000_00_ADDR+DMD_ACOMQ2_01180000_LEN)//A相象限1无功需量
#define DMD_AREQ1Q_01190000_00_LEN          DEMAND_JUST_LEN
#define DMD_AREQ1Q_01190000_01_ADDR         (DMD_AREQ1Q_01190000_00_ADDR+DMD_AREQ1Q_01190000_00_LEN)//A相象限1无功需量:时分
#define DMD_AREQ1Q_01190000_01_LEN          DEMAND_HM_LEN
#define DMD_AREQ1Q_01190000_02_ADDR         (DMD_AREQ1Q_01190000_01_ADDR+DMD_AREQ1Q_01190000_01_LEN)//A相象限1无功需量:年月日
#define DMD_AREQ1Q_01190000_02_LEN          DEMAND_YMD_LEN
#define DMD_AREQ1Q_01190000_LEN             DEMAND_LEN  //8

#define DMD_AREQ2Q_011A0000_00_ADDR         (DMD_AREQ1Q_01190000_00_ADDR+DMD_AREQ1Q_01190000_LEN)//A相象限2无功需量
#define DMD_AREQ2Q_011A0000_00_LEN          DEMAND_JUST_LEN
#define DMD_AREQ2Q_011A0000_01_ADDR         (DMD_AREQ2Q_011A0000_00_ADDR+DMD_AREQ2Q_011A0000_00_LEN)//A相象限2无功需量:时分
#define DMD_AREQ2Q_011A0000_01_LEN          DEMAND_HM_LEN
#define DMD_AREQ2Q_011A0000_02_ADDR         (DMD_AREQ2Q_011A0000_01_ADDR+DMD_AREQ2Q_011A0000_01_LEN)//A相象限2无功需量:年月日
#define DMD_AREQ2Q_011A0000_02_LEN          DEMAND_YMD_LEN
#define DMD_AREQ2Q_011A0000_LEN             DEMAND_LEN  //8

#define DMD_AREQ3Q_011B0000_00_ADDR         (DMD_AREQ2Q_011A0000_00_ADDR+DMD_AREQ2Q_011A0000_LEN)//A相象限3无功需量
#define DMD_AREQ3Q_011B0000_00_LEN          DEMAND_JUST_LEN
#define DMD_AREQ3Q_011B0000_01_ADDR         (DMD_AREQ3Q_011B0000_00_ADDR+DMD_AREQ3Q_011B0000_00_LEN)//A相象限3无功需量:时分
#define DMD_AREQ3Q_011B0000_01_LEN          DEMAND_HM_LEN
#define DMD_AREQ3Q_011B0000_02_ADDR         (DMD_AREQ3Q_011B0000_01_ADDR+DMD_AREQ3Q_011B0000_01_LEN)//A相象限3无功需量:年月日
#define DMD_AREQ3Q_011B0000_02_LEN          DEMAND_YMD_LEN
#define DMD_AREQ3Q_011B0000_LEN             DEMAND_LEN  //8

#define DMD_AREQ4Q_011C0000_00_ADDR         (DMD_AREQ3Q_011B0000_00_ADDR+DMD_AREQ3Q_011B0000_LEN)//A相象限4无功需量
#define DMD_AREQ4Q_011C0000_00_LEN          DEMAND_JUST_LEN
#define DMD_AREQ4Q_011C0000_01_ADDR         (DMD_AREQ4Q_011C0000_00_ADDR+DMD_AREQ4Q_011C0000_00_LEN)//A相象限4无功需量:时分
#define DMD_AREQ4Q_011C0000_01_LEN          DEMAND_HM_LEN
#define DMD_AREQ4Q_011C0000_02_ADDR         (DMD_AREQ4Q_011C0000_01_ADDR+DMD_AREQ4Q_011C0000_01_LEN)//A相象限4无功需量:年月日
#define DMD_AREQ4Q_011C0000_02_LEN          DEMAND_YMD_LEN
#define DMD_AREQ4Q_011C0000_LEN             DEMAND_LEN  //8

#define DMD_APOSS_011D0000_00_ADDR          (DMD_AREQ4Q_011C0000_00_ADDR+DMD_AREQ4Q_011C0000_LEN)//A相正向视在需量
#define DMD_APOSS_011D0000_00_LEN           DEMAND_JUST_LEN
#define DMD_APOSS_011D0000_01_ADDR          (DMD_APOSS_011D0000_00_ADDR+DMD_APOSS_011D0000_00_LEN)//A相正向视在需量:时分
#define DMD_APOSS_011D0000_01_LEN           DEMAND_HM_LEN
#define DMD_APOSS_011D0000_02_ADDR          (DMD_APOSS_011D0000_01_ADDR+DMD_APOSS_011D0000_01_LEN)//A相正向视在需量:年月日
#define DMD_APOSS_011D0000_02_LEN           DEMAND_YMD_LEN
#define DMD_APOSS_011D0000_LEN              DEMAND_LEN  //8

#define DMD_ANEGS_011E0000_00_ADDR          (DMD_APOSS_011D0000_00_ADDR+DMD_APOSS_011D0000_LEN)//A相反向视在需量
#define DMD_ANEGS_011E0000_00_LEN           DEMAND_JUST_LEN
#define DMD_ANEGS_011E0000_01_ADDR          (DMD_ANEGS_011E0000_00_ADDR+DMD_ANEGS_011E0000_00_LEN)//A相反向视在需量:时分
#define DMD_ANEGS_011E0000_01_LEN           DEMAND_HM_LEN
#define DMD_ANEGS_011E0000_02_ADDR          (DMD_ANEGS_011E0000_01_ADDR+DMD_ANEGS_011E0000_01_LEN)//A相反向视在需量:年月日
#define DMD_ANEGS_011E0000_02_LEN           DEMAND_YMD_LEN
#define DMD_ANEGS_011E0000_LEN              DEMAND_LEN  //8
#define DMD_A_TOT_LEN                       SINGLE_PHASE_DEMAND_LEN        //A相需量总长度

#ifdef PHASE_3_LINE_4
//B相需量
//#define DMD_BPOSP_01290000_00_ADDR          (DMD_APOSP_01150000_00_ADDR+DMD_A_TOT_LEN)//B相正向有功需量
//#define DMD_BPOSP_01290000_00_LEN           DEMAND_JUST_LEN
//#define DMD_BPOSP_01290000_01_ADDR          (DMD_BPOSP_01290000_00_ADDR+DMD_BPOSP_01290000_00_LEN)//B相正向有功需量:时分
//#define DMD_BPOSP_01290000_01_LEN           DEMAND_HM_LEN
//#define DMD_BPOSP_01290000_02_ADDR          (DMD_BPOSP_01290000_01_ADDR+DMD_BPOSP_01290000_01_LEN)//B相正向有功需量:年月日
//#define DMD_BPOSP_01290000_02_LEN           DEMAND_YMD_LEN
//#define DMD_BPOSP_01290000_LEN              DEMAND_LEN  //8
//
//#define DMD_BNEGP_012A0000_00_ADDR          (DMD_BPOSP_01290000_00_ADDR+DMD_BPOSP_01290000_LEN)//B相反向有功需量
//#define DMD_BNEGP_012A0000_00_LEN           DEMAND_JUST_LEN
//#define DMD_BNEGP_012A0000_01_ADDR          (DMD_BNEGP_012A0000_00_ADDR+DMD_BNEGP_012A0000_00_LEN)//B相反向有功需量:时分
//#define DMD_BNEGP_012A0000_01_LEN           DEMAND_HM_LEN
//#define DMD_BNEGP_012A0000_02_ADDR          (DMD_BNEGP_012A0000_01_ADDR+DMD_BNEGP_012A0000_01_LEN)//B相反向有功需量:年月日
//#define DMD_BNEGP_012A0000_02_LEN           DEMAND_YMD_LEN
//#define DMD_BNEGP_012A0000_LEN              DEMAND_LEN  //8
//
//#define DMD_BCOMQ1_012B0000_00_ADDR         (DMD_BNEGP_012A0000_00_ADDR+DMD_BNEGP_012A0000_LEN)//B相组合1无功需量
//#define DMD_BCOMQ1_012B0000_00_LEN          DEMAND_JUST_LEN
//#define DMD_BCOMQ1_012B0000_01_ADDR         (DMD_BCOMQ1_012B0000_00_ADDR+DMD_BCOMQ1_012B0000_00_LEN)//B相组合1无功需量:时分
//#define DMD_BCOMQ1_012B0000_01_LEN          DEMAND_HM_LEN
//#define DMD_BCOMQ1_012B0000_02_ADDR         (DMD_BCOMQ1_012B0000_01_ADDR+DMD_BCOMQ1_012B0000_01_LEN)//B相组合1无功需量:年月日
//#define DMD_BCOMQ1_012B0000_02_LEN          DEMAND_YMD_LEN
//#define DMD_BCOMQ1_012B0000_LEN             DEMAND_LEN  //8
//
//#define DMD_BCOMQ2_012C0000_00_ADDR         (DMD_BCOMQ1_012B0000_00_ADDR+DMD_BCOMQ1_012B0000_LEN)//B相组合2无功需量
//#define DMD_BCOMQ2_012C0000_00_LEN          DEMAND_JUST_LEN
//#define DMD_BCOMQ2_012C0000_01_ADDR         (DMD_BCOMQ2_012C0000_00_ADDR+DMD_BCOMQ2_012C0000_00_LEN)//B相组合2无功需量:时分
//#define DMD_BCOMQ2_012C0000_01_LEN          DEMAND_HM_LEN
//#define DMD_BCOMQ2_012C0000_02_ADDR         (DMD_BCOMQ2_012C0000_01_ADDR+DMD_BCOMQ2_012C0000_01_LEN)//B相组合2无功需量:年月日
//#define DMD_BCOMQ2_012C0000_02_LEN          DEMAND_YMD_LEN
//#define DMD_BCOMQ2_012C0000_LEN             DEMAND_LEN  //8
//
//#define DMD_BREQ1Q_012D0000_00_ADDR         (DMD_BCOMQ2_012C0000_00_ADDR+DMD_BCOMQ2_012C0000_LEN)//B相象限1无功需量
//#define DMD_BREQ1Q_012D0000_00_LEN          DEMAND_JUST_LEN
//#define DMD_BREQ1Q_012D0000_01_ADDR         (DMD_BREQ1Q_012D0000_00_ADDR+DMD_BREQ1Q_012D0000_00_LEN)//B相象限1无功需量:时分
//#define DMD_BREQ1Q_012D0000_01_LEN          DEMAND_HM_LEN
//#define DMD_BREQ1Q_012D0000_02_ADDR         (DMD_BREQ1Q_012D0000_01_ADDR+DMD_BREQ1Q_012D0000_01_LEN)//B相象限1无功需量:年月日
//#define DMD_BREQ1Q_012D0000_02_LEN          DEMAND_YMD_LEN
//#define DMD_BREQ1Q_012D0000_LEN             DEMAND_LEN  //8
//
//#define DMD_BREQ2Q_012E0000_00_ADDR         (DMD_BREQ1Q_012D0000_00_ADDR+DMD_BREQ1Q_012D0000_LEN)//B相象限2无功需量
//#define DMD_BREQ2Q_012E0000_00_LEN          DEMAND_JUST_LEN
//#define DMD_BREQ2Q_012E0000_01_ADDR         (DMD_BREQ2Q_012E0000_00_ADDR+DMD_BREQ2Q_012E0000_00_LEN)//B相象限2无功需量:时分
//#define DMD_BREQ2Q_012E0000_01_LEN          DEMAND_HM_LEN
//#define DMD_BREQ2Q_012E0000_02_ADDR         (DMD_BREQ2Q_012E0000_01_ADDR+DMD_BREQ2Q_012E0000_01_LEN)//B相象限2无功需量:年月日
//#define DMD_BREQ2Q_012E0000_02_LEN          DEMAND_YMD_LEN
//#define DMD_BREQ2Q_012E0000_LEN             DEMAND_LEN  //8
//
//#define DMD_BREQ3Q_012F0000_00_ADDR         (DMD_BREQ2Q_012E0000_00_ADDR+DMD_BREQ2Q_012E0000_LEN)//B相象限3无功需量
//#define DMD_BREQ3Q_012F0000_00_LEN          DEMAND_JUST_LEN
//#define DMD_BREQ3Q_012F0000_01_ADDR         (DMD_BREQ3Q_012F0000_00_ADDR+DMD_BREQ3Q_012F0000_00_LEN)//B相象限3无功需量:时分
//#define DMD_BREQ3Q_012F0000_01_LEN          DEMAND_HM_LEN
//#define DMD_BREQ3Q_012F0000_02_ADDR         (DMD_BREQ3Q_012F0000_01_ADDR+DMD_BREQ3Q_012F0000_01_LEN)//B相象限3无功需量:年月日
//#define DMD_BREQ3Q_012F0000_02_LEN          DEMAND_YMD_LEN
//#define DMD_BREQ3Q_012F0000_LEN             DEMAND_LEN  //8
//
//#define DMD_BREQ4Q_01300000_00_ADDR         (DMD_BREQ3Q_012F0000_00_ADDR+DMD_BREQ3Q_012F0000_LEN)//B相象限4无功需量
//#define DMD_BREQ4Q_01300000_00_LEN          DEMAND_JUST_LEN
//#define DMD_BREQ4Q_01300000_01_ADDR         (DMD_BREQ4Q_01300000_00_ADDR+DMD_BREQ4Q_01300000_00_LEN)//B相象限4无功需量:时分
//#define DMD_BREQ4Q_01300000_01_LEN          DEMAND_HM_LEN
//#define DMD_BREQ4Q_01300000_02_ADDR         (DMD_BREQ4Q_01300000_01_ADDR+DMD_BREQ4Q_01300000_01_LEN)//B相象限4无功需量:年月日
//#define DMD_BREQ4Q_01300000_02_LEN          DEMAND_YMD_LEN
//#define DMD_BREQ4Q_01300000_LEN             DEMAND_LEN  //8
//
//#define DMD_BPOSS_01310000_00_ADDR          (DMD_BREQ4Q_01300000_00_ADDR+DMD_BREQ4Q_01300000_LEN)//B相正向视在需量
//#define DMD_BPOSS_01310000_00_LEN           DEMAND_JUST_LEN
//#define DMD_BPOSS_01310000_01_ADDR          (DMD_BPOSS_01310000_00_ADDR+DMD_BPOSS_01310000_00_LEN)//B相正向视在需量:时分
//#define DMD_BPOSS_01310000_01_LEN           DEMAND_HM_LEN
//#define DMD_BPOSS_01310000_02_ADDR          (DMD_BPOSS_01310000_01_ADDR+DMD_BPOSS_01310000_01_LEN)//B相正向视在需量:年月日
//#define DMD_BPOSS_01310000_02_LEN           DEMAND_YMD_LEN
//#define DMD_BPOSS_01310000_LEN              DEMAND_LEN  //8
//
//#define DMD_BNEGS_01320000_00_ADDR          (DMD_BPOSS_01310000_00_ADDR+DMD_BPOSS_01310000_LEN)//B相反向视在需量
//#define DMD_BNEGS_01320000_00_LEN           DEMAND_JUST_LEN
//#define DMD_BNEGS_01320000_01_ADDR          (DMD_BNEGS_01320000_00_ADDR+DMD_BNEGS_01320000_00_LEN)//B相反向视在需量:时分
//#define DMD_BNEGS_01320000_01_LEN           DEMAND_HM_LEN
//#define DMD_BNEGS_01320000_02_ADDR          (DMD_BNEGS_01320000_01_ADDR+DMD_ANEGS_011E0000_01_LEN)//B相反向视在需量:年月日
//#define DMD_BNEGS_01320000_02_LEN           DEMAND_YMD_LEN
//#define DMD_BNEGS_01320000_LEN              DEMAND_LEN  //8
//#define DMD_B_TOT_LEN                       SINGLE_PHASE_DEMAND_LEN        //B相需量总长度
////C相需量
//#define DMD_CPOSP_013D0000_00_ADDR          (DMD_BPOSP_01290000_00_ADDR+DMD_B_TOT_LEN)//C相正向有功需量
#else   //PHASE_3_LINE_4
#define DMD_CPOSP_013D0000_00_ADDR          (DMD_APOSP_01150000_00_ADDR+DMD_A_TOT_LEN)//C相正向有功需量
#endif  //PHASE_3_LINE_4
#define DMD_CPOSP_013D0000_00_LEN           DEMAND_JUST_LEN
#define DMD_CPOSP_013D0000_01_ADDR          (DMD_CPOSP_013D0000_00_ADDR+DMD_CPOSP_013D0000_00_LEN)//C相正向有功需量:时分
#define DMD_CPOSP_013D0000_01_LEN           DEMAND_HM_LEN
#define DMD_CPOSP_013D0000_02_ADDR          (DMD_CPOSP_013D0000_01_ADDR+DMD_CPOSP_013D0000_01_LEN)//C相正向有功需量:年月日
#define DMD_CPOSP_013D0000_02_LEN           DEMAND_YMD_LEN
#define DMD_CPOSP_013D0000_LEN              DEMAND_LEN  //8

#define DMD_CNEGP_013E0000_00_ADDR          (DMD_CPOSP_013D0000_00_ADDR+DMD_CPOSP_013D0000_LEN)//C相反向有功需量
#define DMD_CNEGP_013E0000_00_LEN           DEMAND_JUST_LEN
#define DMD_CNEGP_013E0000_01_ADDR          (DMD_CNEGP_013E0000_00_ADDR+DMD_CNEGP_013E0000_00_LEN)//C相反向有功需量:时分
#define DMD_CNEGP_013E0000_01_LEN           DEMAND_HM_LEN
#define DMD_CNEGP_013E0000_02_ADDR          (DMD_CNEGP_013E0000_01_ADDR+DMD_CNEGP_013E0000_01_LEN)//C相反向有功需量:年月日
#define DMD_CNEGP_013E0000_02_LEN           DEMAND_YMD_LEN
#define DMD_CNEGP_013E0000_LEN              DEMAND_LEN  //8

#define DMD_CCOMQ1_013F0000_00_ADDR         (DMD_CNEGP_013E0000_00_ADDR+DMD_CNEGP_013E0000_LEN)//C相组合1无功需量
#define DMD_CCOMQ1_013F0000_00_LEN          DEMAND_JUST_LEN
#define DMD_CCOMQ1_013F0000_01_ADDR         (DMD_CCOMQ1_013F0000_00_ADDR+DMD_CCOMQ1_013F0000_00_LEN)//C相组合1无功需量:时分
#define DMD_CCOMQ1_013F0000_01_LEN          DEMAND_HM_LEN
#define DMD_CCOMQ1_013F0000_02_ADDR         (DMD_CCOMQ1_013F0000_01_ADDR+DMD_CCOMQ1_013F0000_01_LEN)//C相组合1无功需量:年月日
#define DMD_CCOMQ1_013F0000_02_LEN          DEMAND_YMD_LEN
#define DMD_CCOMQ1_013F0000_LEN             DEMAND_LEN  //8

#define DMD_CCOMQ2_01400000_00_ADDR         (DMD_CCOMQ1_013F0000_00_ADDR+DMD_CCOMQ1_013F0000_LEN)//C相组合2无功需量
#define DMD_CCOMQ2_01400000_00_LEN          DEMAND_JUST_LEN
#define DMD_CCOMQ2_01400000_01_ADDR         (DMD_CCOMQ2_01400000_00_ADDR+DMD_CCOMQ2_01400000_00_LEN)//C相组合2无功需量:时分
#define DMD_CCOMQ2_01400000_01_LEN          DEMAND_HM_LEN
#define DMD_CCOMQ2_01400000_02_ADDR         (DMD_CCOMQ2_01400000_01_ADDR+DMD_CCOMQ2_01400000_01_LEN)//C相组合2无功需量:年月日
#define DMD_CCOMQ2_01400000_02_LEN          DEMAND_YMD_LEN
#define DMD_CCOMQ2_01400000_LEN             DEMAND_LEN  //8

#define DMD_CREQ1Q_01410000_00_ADDR         (DMD_CCOMQ2_01400000_00_ADDR+DMD_CCOMQ2_01400000_LEN)//C相象限1无功需量
#define DMD_CREQ1Q_01410000_00_LEN          DEMAND_JUST_LEN
#define DMD_CREQ1Q_01410000_01_ADDR         (DMD_CREQ1Q_01410000_00_ADDR+DMD_CREQ1Q_01410000_00_LEN)//C相象限1无功需量:时分
#define DMD_CREQ1Q_01410000_01_LEN          DEMAND_HM_LEN
#define DMD_CREQ1Q_01410000_02_ADDR         (DMD_CREQ1Q_01410000_01_ADDR+DMD_CREQ1Q_01410000_01_LEN)//C相象限1无功需量:年月日
#define DMD_CREQ1Q_01410000_02_LEN          DEMAND_YMD_LEN
#define DMD_CREQ1Q_01410000_LEN             DEMAND_LEN  //8

#define DMD_CREQ2Q_01420000_00_ADDR         (DMD_CREQ1Q_01410000_00_ADDR+DMD_CREQ1Q_01410000_LEN)//C相象限2无功需量
#define DMD_CREQ2Q_01420000_00_LEN          DEMAND_JUST_LEN
#define DMD_CREQ2Q_01420000_01_ADDR         (DMD_CREQ2Q_01420000_00_ADDR+DMD_CREQ2Q_01420000_00_LEN)//C相象限2无功需量:时分
#define DMD_CREQ2Q_01420000_01_LEN          DEMAND_HM_LEN
#define DMD_CREQ2Q_01420000_02_ADDR         (DMD_CREQ2Q_01420000_01_ADDR+DMD_CREQ2Q_01420000_01_LEN)//C相象限2无功需量:年月日
#define DMD_CREQ2Q_01420000_02_LEN          DEMAND_YMD_LEN
#define DMD_CREQ2Q_01420000_LEN             DEMAND_LEN  //8

#define DMD_CREQ3Q_01430000_00_ADDR         (DMD_CREQ2Q_01420000_00_ADDR+DMD_CREQ2Q_01420000_LEN)//C相象限3无功需量
#define DMD_CREQ3Q_01430000_00_LEN          DEMAND_JUST_LEN
#define DMD_CREQ3Q_01430000_01_ADDR         (DMD_CREQ3Q_01430000_00_ADDR+DMD_CREQ3Q_01430000_00_LEN)//C相象限3无功需量:时分
#define DMD_CREQ3Q_01430000_01_LEN          DEMAND_HM_LEN
#define DMD_CREQ3Q_01430000_02_ADDR         (DMD_CREQ3Q_01430000_01_ADDR+DMD_CREQ3Q_01430000_01_LEN)//C相象限3无功需量:年月日
#define DMD_CREQ3Q_01430000_02_LEN          DEMAND_YMD_LEN
#define DMD_CREQ3Q_01430000_LEN             DEMAND_LEN  //8

#define DMD_CREQ4Q_01440000_00_ADDR         (DMD_CREQ3Q_01430000_00_ADDR+DMD_CREQ3Q_01430000_LEN)//C相象限4无功需量
#define DMD_CREQ4Q_01440000_00_LEN          DEMAND_JUST_LEN
#define DMD_CREQ4Q_01440000_01_ADDR         (DMD_CREQ4Q_01440000_00_ADDR+DMD_CREQ4Q_01440000_00_LEN)//C相象限4无功需量:时分
#define DMD_CREQ4Q_01440000_01_LEN          DEMAND_HM_LEN
#define DMD_CREQ4Q_01440000_02_ADDR         (DMD_CREQ4Q_01440000_01_ADDR+DMD_CREQ4Q_01440000_01_LEN)//C相象限4无功需量:年月日
#define DMD_CREQ4Q_01440000_02_LEN          DEMAND_YMD_LEN
#define DMD_CREQ4Q_01440000_LEN             DEMAND_LEN  //8

#define DMD_CPOSS_01450000_00_ADDR          (DMD_CREQ4Q_01440000_00_ADDR+DMD_CREQ4Q_01440000_LEN)//C相正向视在需量
#define DMD_CPOSS_01450000_00_LEN           DEMAND_JUST_LEN
#define DMD_CPOSS_01450000_01_ADDR          (DMD_CPOSS_01450000_00_ADDR+DMD_CPOSS_01450000_00_LEN)//C相正向视在需量:时分
#define DMD_CPOSS_01450000_01_LEN           DEMAND_HM_LEN
#define DMD_CPOSS_01450000_02_ADDR          (DMD_CPOSS_01450000_01_ADDR+DMD_CPOSS_01450000_01_LEN)//C相正向视在需量:年月日
#define DMD_CPOSS_01450000_02_LEN           DEMAND_YMD_LEN
#define DMD_CPOSS_01450000_LEN              DEMAND_LEN  //8

#define DMD_CNEGS_01460000_00_ADDR          (DMD_CPOSS_01450000_00_ADDR+DMD_CPOSS_01450000_LEN)//C相反向视在需量
#define DMD_CNEGS_01460000_00_LEN           DEMAND_JUST_LEN
#define DMD_CNEGS_01460000_01_ADDR          (DMD_CNEGS_01460000_00_ADDR+DMD_CNEGS_01460000_00_LEN)//C相反向视在需量:时分
#define DMD_CNEGS_01460000_01_LEN           DEMAND_HM_LEN
#define DMD_CNEGS_01460000_02_ADDR          (DMD_CNEGS_01460000_01_ADDR+DMD_CNEGS_01460000_01_LEN)//BC相反向视在需量:年月日
#define DMD_CNEGS_01460000_02_LEN           DEMAND_YMD_LEN
#define DMD_CNEGS_01460000_LEN              DEMAND_LEN  //8
#define DMD_C_TOT_LEN                       SINGLE_PHASE_DEMAND_LEN        //C相需量总长度

#ifdef PHASE_3_LINE_4
//#define DMD_RAM_ARRAY_MAX                   (TOTAL_PHASE_DEMAND_LEN * 10 + DEMAND_LEN * 10 * 3)  //当前需量RAM缓存容量[需量、时分、年月日均占1个字]
#else   //PHASE_3_LINE_4
#define DMD_RAM_ARRAY_MAX                   (TOTAL_PHASE_DEMAND_LEN * 10 + DEMAND_LEN * 10 * 2)  //当前需量RAM缓存容量[需量、时分、年月日均占1个字]
#endif  //PHASE_3_LINE_4

//上1~12结算日需要冻结 开始[暂未应用到DLT645]
#define DMD_LASTXMONTH_01010001_00_OFFSET_MAXNUM    12  //
#define DMD_LASTXMONTH_01010001_00_OFFSET_ADDR      (DMD_CPOSP_013D0000_00_ADDR+DMD_C_TOT_LEN)//次数指针保存的地址
#define DMD_LASTXMONTH_01010001_00_OFFSET_LEN       3 + CRC_LEN   //次数指针长度
#ifdef PHASE_3_LINE_4
//#define DMD_LASTXMONTH_01010001_00_OFFSET_BLOCKLEN  (TOTAL_PHASE_DEMAND_LEN*10+SINGLE_PHASE_DEMAND_LEN*3)//单次需量冻结所有数据长度
#else   //PHASE_3_LINE_4
#define DMD_LASTXMONTH_01010001_00_OFFSET_BLOCKLEN  (TOTAL_PHASE_DEMAND_LEN*10+SINGLE_PHASE_DEMAND_LEN*2)//单次需量冻结所有数据长度
#endif  //PHASE_3_LINE_4
#define DMD_LASTXMONTH_01010001_00_HEAD_ADDR        (DMD_LASTXMONTH_01010001_00_OFFSET_ADDR+DMD_LASTXMONTH_01010001_00_OFFSET_LEN)
#define DMD_LASTXMONTH_01010001_00_END_ADDR         (DMD_LASTXMONTH_01010001_00_HEAD_ADDR+DMD_LASTXMONTH_01010001_00_OFFSET_BLOCKLEN*DMD_LASTXMONTH_01010001_00_OFFSET_MAXNUM)
//上1~12结算日需要冻结 结束


//-----------------------------------------------------------------------------
// 参变量定义：
//
//-----------------------------------------------------------------------------
#define PAR_DATE_04000101_00_ADDR           DMD_LASTXMONTH_01010001_00_END_ADDR //避开事件暂居的空间
//#define PAR_DATE_04000101_00_ADDR           (EVET_OPNTMNBX_END_ADDR)                                        //日期及星期(其中0代表星期天)[暂未应用到DLT645]
#define PAR_DATE_04000101_00_LEN            4
#define PAR_TIME_04000102_00_ADDR           (PAR_DATE_04000101_00_ADDR + PAR_DATE_04000101_00_LEN)          //时间[暂未应用到DLT645]
#define PAR_TIME_04000102_00_LEN            3
#define PAR_DMDMAXP_04000103_00_ADDR        (PAR_TIME_04000102_00_ADDR + PAR_TIME_04000102_00_LEN)          //最大需量周期
#define PAR_DMDMAXP_04000103_00_LEN         1
#define PAR_SLIDTIME_04000104_00_ADDR       (PAR_DMDMAXP_04000103_00_ADDR + PAR_DMDMAXP_04000103_00_LEN)    //滑差时间
#define PAR_SLIDTIME_04000104_00_LEN        1
#define PAR_PLSWIDTH_04000105_00_ADDR       (PAR_SLIDTIME_04000104_00_ADDR + PAR_SLIDTIME_04000104_00_LEN)  //校表脉冲宽度
#define PAR_PLSWIDTH_04000105_00_LEN        2
#define PAR_TZSWTIME_04000106_00_ADDR       (PAR_PLSWIDTH_04000105_00_ADDR + PAR_PLSWIDTH_04000105_00_LEN)  //两套时区表切换时间:时分
#define PAR_TZSWTIME_04000106_00_LEN        2
#define PAR_TZSWTIME_04000106_01_ADDR       (PAR_TZSWTIME_04000106_00_ADDR + PAR_TZSWTIME_04000106_00_LEN)  //两套时区表切换时间:年月日
#define PAR_TZSWTIME_04000106_01_LEN        3
#define PAR_TSSWTIME_04000107_00_ADDR       (PAR_TZSWTIME_04000106_01_ADDR + PAR_TZSWTIME_04000106_01_LEN)  //两套日时段表切换时间:时分
#define PAR_TSSWTIME_04000107_00_LEN        2
#define PAR_TSSWTIME_04000107_01_ADDR       (PAR_TSSWTIME_04000107_00_ADDR + PAR_TSSWTIME_04000107_00_LEN)  //两套日时段表切换时间:年月日
#define PAR_TSSWTIME_04000107_01_LEN        3

#define PAR_TZAMOUNT_04000201_00_ADDR       (PAR_TSSWTIME_04000107_01_ADDR + PAR_TSSWTIME_04000107_01_LEN)  //年时区数p(≤14)
#define PAR_TZAMOUNT_04000201_00_LEN        1
#define PAR_TSAMOUNT_04000202_00_ADDR       (PAR_TZAMOUNT_04000201_00_ADDR + PAR_TZAMOUNT_04000201_00_LEN)  //日时段表数q(≤8)
#define PAR_TSAMOUNT_04000202_00_LEN        1
#define PAR_TSSWCNT_04000203_00_ADDR        (PAR_TSAMOUNT_04000202_00_ADDR + PAR_TSAMOUNT_04000202_00_LEN)  //日时段数(每日切换数) m(≤14)
#define PAR_TSSWCNT_04000203_00_LEN         1
#define PAR_FEERATE_04000204_00_ADDR        (PAR_TSSWCNT_04000203_00_ADDR + PAR_TSSWCNT_04000203_00_LEN)    //费率数k(≤63)
#define PAR_FEERATE_04000204_00_LEN         1
#define PAR_HOLIDYS_04000205_00_ADDR        (PAR_FEERATE_04000204_00_ADDR + PAR_FEERATE_04000204_00_LEN)    //公共假日数n(≤254)
#define PAR_HOLIDYS_04000205_00_LEN         2
#define PAR_HMANLS_04000206_00_ADDR         (PAR_HOLIDYS_04000205_00_ADDR + PAR_HOLIDYS_04000205_00_LEN)    //谐波分析次数[暂未应用到DLT645]
#define PAR_HMANLS_04000206_00_LEN          1
//{[暂未应用到DLT645]
#define PAR_LPDISCNT_04000301_00_ADDR       (PAR_HMANLS_04000206_00_ADDR + PAR_HMANLS_04000206_00_LEN)      //自动循环显示屏数
#define PAR_LPDISCNT_04000301_00_LEN        1
#define PAR_DISTIME_04000302_00_ADDR        (PAR_LPDISCNT_04000301_00_ADDR + PAR_LPDISCNT_04000301_00_LEN)  //每屏显示时间
#define PAR_DISTIME_04000302_00_LEN         1
#define PAR_ENGDECIM_04000303_00_ADDR       (PAR_DISTIME_04000302_00_ADDR + PAR_DISTIME_04000302_00_LEN)    //显示电能小数位数
#define PAR_ENGDECIM_04000303_00_LEN        1
#define PAR_DMDDECIM_04000304_00_ADDR       (PAR_ENGDECIM_04000303_00_ADDR + PAR_ENGDECIM_04000303_00_LEN)  //显示功率(最大需量)小数位数
#define PAR_DMDDECIM_04000304_00_LEN        1
#define PAR_KEYDIS_04000305_00_ADDR         (PAR_DMDDECIM_04000304_00_ADDR + PAR_DMDDECIM_04000304_00_LEN)  //按键循环显示屏数
#define PAR_KEYDIS_04000305_00_LEN          1
//}[暂未应用到DLT645]
#define PAR_HVMADDR_04000401_00_ADDR        (PAR_KEYDIS_04000305_00_ADDR + PAR_KEYDIS_04000305_00_LEN)      //通信地址:A0~A3
#define PAR_HVMADDR_04000401_00_LEN         4
#define PAR_HVMADDR_04000401_01_ADDR        (PAR_HVMADDR_04000401_00_ADDR + PAR_HVMADDR_04000401_00_LEN)    //通信地址:A4~A5
#define PAR_HVMADDR_04000401_01_LEN         2
#define PAR_HVMADDR_04000401_TOT_LEN        (PAR_HVMADDR_04000401_00_LEN + PAR_HVMADDR_04000401_01_LEN)
#define PAR_HVMNUM_04000402_00_ADDR         (PAR_HVMADDR_04000401_00_ADDR + PAR_HVMADDR_04000401_TOT_LEN)   //表号:N0~N3
#define PAR_HVMNUM_04000402_00_LEN          4
#define PAR_HVMNUM_04000402_01_ADDR         (PAR_HVMNUM_04000402_00_ADDR + PAR_HVMNUM_04000402_00_LEN)      //表号:N4~N5
#define PAR_HVMNUM_04000402_01_LEN          2
#define PAR_HVMNUM_04000402_TOT_LEN         PAR_HVMNUM_04000402_00_LEN + PAR_HVMNUM_04000402_01_LEN
#define PAR_PROPTNUM_04000403_00_ADDR       (PAR_HVMNUM_04000402_00_ADDR + PAR_HVMNUM_04000402_TOT_LEN)     //资产管理编码(ASCII码):N0~N3
#define PAR_PROPTNUM_04000403_00_LEN        4
#define PAR_PROPTNUM_04000403_01_ADDR       (PAR_PROPTNUM_04000403_00_ADDR + PAR_PROPTNUM_04000403_00_LEN)  //资产管理编码(ASCII码):N4~N7
#define PAR_PROPTNUM_04000403_01_LEN        4
#define PAR_PROPTNUM_04000403_02_ADDR       (PAR_PROPTNUM_04000403_01_ADDR + PAR_PROPTNUM_04000403_01_LEN)  //资产管理编码(ASCII码):N8~N11
#define PAR_PROPTNUM_04000403_02_LEN        4
#define PAR_PROPTNUM_04000403_03_ADDR       (PAR_PROPTNUM_04000403_02_ADDR + PAR_PROPTNUM_04000403_02_LEN)  //资产管理编码(ASCII码):N12~N15
#define PAR_PROPTNUM_04000403_03_LEN        4
#define PAR_PROPTNUM_04000403_04_ADDR       (PAR_PROPTNUM_04000403_03_ADDR + PAR_PROPTNUM_04000403_03_LEN)  //资产管理编码(ASCII码):N16~N19
#define PAR_PROPTNUM_04000403_04_LEN        4
#define PAR_PROPTNUM_04000403_05_ADDR       (PAR_PROPTNUM_04000403_04_ADDR + PAR_PROPTNUM_04000403_04_LEN)  //资产管理编码(ASCII码):N20~N23
#define PAR_PROPTNUM_04000403_05_LEN        4
#define PAR_PROPTNUM_04000403_06_ADDR       (PAR_PROPTNUM_04000403_05_ADDR + PAR_PROPTNUM_04000403_05_LEN)  //资产管理编码(ASCII码):N24~N27
#define PAR_PROPTNUM_04000403_06_LEN        4
#define PAR_PROPTNUM_04000403_07_ADDR       (PAR_PROPTNUM_04000403_06_ADDR + PAR_PROPTNUM_04000403_06_LEN)  //资产管理编码(ASCII码):N28~N31
#define PAR_PROPTNUM_04000403_07_LEN        4
#define PAR_PROPTNUM_04000403_TOT_LEN       32
#define PAR_VRATING_04000404_00_ADDR        (PAR_PROPTNUM_04000403_00_ADDR + PAR_PROPTNUM_04000403_TOT_LEN) //额定电压(ASCII码)前4字节
#define PAR_VRATING_04000404_00_LEN         4
#define PAR_VRATING_04000404_01_ADDR        (PAR_VRATING_04000404_00_ADDR + PAR_VRATING_04000404_00_LEN)    //额定电压(ASCII码)后2字节
#define PAR_VRATING_04000404_01_LEN         2
#define PAR_VRATING_04000404_TOT_LEN        (PAR_VRATING_04000404_00_LEN + PAR_VRATING_04000404_01_LEN)
#define PAR_IRATING_04000405_00_ADDR        (PAR_VRATING_04000404_00_ADDR + PAR_VRATING_04000404_TOT_LEN)   //额定电流/基本电流(ASCII码)前4字节
#define PAR_IRATING_04000405_00_LEN         4
#define PAR_IRATING_04000405_01_ADDR        (PAR_IRATING_04000405_00_ADDR + PAR_IRATING_04000405_00_LEN)    //额定电流/基本电流(ASCII码)后2字节
#define PAR_IRATING_04000405_01_LEN         2
#define PAR_IRATING_04000405_TOT_LEN        (PAR_IRATING_04000405_00_LEN + PAR_IRATING_04000405_01_LEN)
#define PAR_IMAX_04000406_00_ADDR           (PAR_IRATING_04000405_00_ADDR + PAR_IRATING_04000405_TOT_LEN)   //最大电流(ASCII码)前4字节
#define PAR_IMAX_04000406_00_LEN            4
#define PAR_IMAX_04000406_01_ADDR           (PAR_IMAX_04000406_00_ADDR + PAR_IMAX_04000406_00_LEN)          //最大电流(ASCII码)后2字节
#define PAR_IMAX_04000406_01_LEN            2
#define PAR_IMAX_04000406_TOT_LEN           (PAR_IMAX_04000406_00_LEN + PAR_IMAX_04000406_01_LEN)
#define PAR_PACCURACY_04000407_00_ADDR      (PAR_IMAX_04000406_00_ADDR + PAR_IMAX_04000406_TOT_LEN)         //有功准确度等级(ASCII码)
#define PAR_PACCURACY_04000407_00_LEN       4
#define PAR_QACCURACY_04000408_00_ADDR      (PAR_PACCURACY_04000407_00_ADDR + PAR_PACCURACY_04000407_00_LEN)//无功准确度等级(ASCII码)
#define PAR_QACCURACY_04000408_00_LEN       4
#define PAR_PCONSTANT_04000409_00_ADDR      (PAR_QACCURACY_04000408_00_ADDR + PAR_QACCURACY_04000408_00_LEN)//电表有功常数
#define PAR_PCONSTANT_04000409_00_LEN       3
#define PAR_QCONSTANT_0400040A_00_ADDR      (PAR_PCONSTANT_04000409_00_ADDR + PAR_PCONSTANT_04000409_00_LEN)//电无有功常数
#define PAR_QCONSTANT_0400040A_00_LEN       3
#define PAR_HVMTYPE_0400040B_00_ADDR        (PAR_QCONSTANT_0400040A_00_ADDR + PAR_QCONSTANT_0400040A_00_LEN)//电表型号(ASCII码):前4字节
#define PAR_HVMTYPE_0400040B_00_LEN         4
#define PAR_HVMTYPE_0400040B_01_ADDR        (PAR_HVMTYPE_0400040B_00_ADDR + PAR_HVMTYPE_0400040B_00_LEN)    //电表型号(ASCII码):中间4字节
#define PAR_HVMTYPE_0400040B_01_LEN         4
#define PAR_HVMTYPE_0400040B_02_ADDR        (PAR_HVMTYPE_0400040B_01_ADDR + PAR_HVMTYPE_0400040B_01_LEN)    //电表型号(ASCII码):后2字节
#define PAR_HVMTYPE_0400040B_02_LEN         2
#define PAR_HVMTYPE_0400040B_TOT_LEN        (PAR_HVMTYPE_0400040B_00_LEN + PAR_HVMTYPE_0400040B_01_LEN + PAR_HVMTYPE_0400040B_02_LEN)
#define PAR_HVMBIRTH_0400040C_00_ADDR       (PAR_HVMTYPE_0400040B_00_ADDR + PAR_HVMTYPE_0400040B_TOT_LEN)   //生产日期(ASCII码):前4字节
#define PAR_HVMBIRTH_0400040C_00_LEN        4
#define PAR_HVMBIRTH_0400040C_01_ADDR       (PAR_HVMBIRTH_0400040C_00_ADDR + PAR_HVMBIRTH_0400040C_00_LEN)  //生产日期(ASCII码):中间4字节
#define PAR_HVMBIRTH_0400040C_01_LEN        4
#define PAR_HVMBIRTH_0400040C_02_ADDR       (PAR_HVMBIRTH_0400040C_01_ADDR + PAR_HVMBIRTH_0400040C_01_LEN)  //生产日期(ASCII码):后2字节
#define PAR_HVMBIRTH_0400040C_02_LEN        2
#define PAR_HVMBIRTH_0400040C_TOT_LEN       (PAR_HVMBIRTH_0400040C_00_LEN + PAR_HVMBIRTH_0400040C_01_LEN + PAR_HVMBIRTH_0400040C_02_LEN)
#define PAR_PROTOVER_0400040D_00_ADDR       (PAR_HVMBIRTH_0400040C_00_ADDR + PAR_HVMBIRTH_0400040C_TOT_LEN) //协议版本号(ASCII码):N0~N3
#define PAR_PROTOVER_0400040D_00_LEN        4
#define PAR_PROTOVER_0400040D_01_ADDR       (PAR_PROTOVER_0400040D_00_ADDR + PAR_PROTOVER_0400040D_00_LEN)  //协议版本号(ASCII码):N4~N7
#define PAR_PROTOVER_0400040D_01_LEN        4
#define PAR_PROTOVER_0400040D_02_ADDR       (PAR_PROTOVER_0400040D_01_ADDR + PAR_PROTOVER_0400040D_01_LEN)  //协议版本号(ASCII码):N8~N11
#define PAR_PROTOVER_0400040D_02_LEN        4
#define PAR_PROTOVER_0400040D_03_ADDR       (PAR_PROTOVER_0400040D_02_ADDR + PAR_PROTOVER_0400040D_02_LEN)  //协议版本号(ASCII码):N12~N15
#define PAR_PROTOVER_0400040D_03_LEN        4
#define PAR_PROTOVER_0400040D_TOT_LEN       16
//{[暂未应用到DLT645]
#define PAR_RUNSTAT1_04000501_00_ADDR       (PAR_PROTOVER_0400040D_00_ADDR + PAR_PROTOVER_0400040D_TOT_LEN) //电表运行状态字1
#define PAR_RUNSTAT1_04000501_00_LEN        2
#define PAR_RUNSTAT2_04000502_00_ADDR       (PAR_RUNSTAT1_04000501_00_ADDR + PAR_RUNSTAT1_04000501_00_LEN)  //电表运行状态字2
#define PAR_RUNSTAT2_04000502_00_LEN        2
#define PAR_RUNSTAT3_04000503_00_ADDR       (PAR_RUNSTAT2_04000502_00_ADDR + PAR_RUNSTAT2_04000502_00_LEN)  //电表运行状态字3
#define PAR_RUNSTAT3_04000503_00_LEN        2
#define PAR_RUNSTAT4_04000504_00_ADDR       (PAR_RUNSTAT3_04000503_00_ADDR + PAR_RUNSTAT3_04000503_00_LEN)  //电表运行状态字4
#define PAR_RUNSTAT4_04000504_00_LEN        2
#define PAR_RUNSTAT5_04000505_00_ADDR       (PAR_RUNSTAT4_04000504_00_ADDR + PAR_RUNSTAT4_04000504_00_LEN)  //电表运行状态字5
#define PAR_RUNSTAT5_04000505_00_LEN        2
#define PAR_RUNSTAT6_04000506_00_ADDR       (PAR_RUNSTAT5_04000505_00_ADDR + PAR_RUNSTAT5_04000505_00_LEN)  //电表运行状态字6
#define PAR_RUNSTAT6_04000506_00_LEN        2
#define PAR_RUNSTAT7_04000507_00_ADDR       (PAR_RUNSTAT6_04000506_00_ADDR + PAR_RUNSTAT6_04000506_00_LEN)  //电表运行状态字7
#define PAR_RUNSTAT7_04000507_00_LEN        2
//}[暂未应用到DLT645]
#define PAR_PCOMB_04000601_00_ADDR          (PAR_RUNSTAT7_04000507_00_ADDR + PAR_RUNSTAT7_04000507_00_LEN)  //有功组合方式特征字
#define PAR_PCOMB_04000601_00_LEN           1
#define PAR_QCOMB1_04000602_00_ADDR         (PAR_PCOMB_04000601_00_ADDR + PAR_PCOMB_04000601_00_LEN)        //无功组合方式1特征字
#define PAR_QCOMB1_04000602_00_LEN          1
#define PAR_QCOMB2_04000603_00_ADDR         (PAR_QCOMB1_04000602_00_ADDR + PAR_QCOMB1_04000602_00_LEN)      //无功组合方式2特征字
#define PAR_QCOMB2_04000603_00_LEN          1

#define PAR_MDIRDABD_04000701_00_ADDR       (PAR_QCOMB2_04000603_00_ADDR + PAR_QCOMB2_04000603_00_LEN)      //调制型红外光口通信速率特征字[暂未应用到DLT645]
#define PAR_MDIRDABD_04000701_00_LEN        1
#define PAR_CTIRDABD_04000702_00_ADDR       (PAR_MDIRDABD_04000701_00_ADDR + PAR_MDIRDABD_04000701_00_LEN)  //接触型红外光口通信速率特征字[暂未应用到DLT645]
#define PAR_CTIRDABD_04000702_00_LEN        1
#define PAR_PORT1BD_04000703_00_ADDR        (PAR_CTIRDABD_04000702_00_ADDR + PAR_CTIRDABD_04000702_00_LEN)  //通信口1通信速率特征字[暂未应用到DLT645]
#define PAR_PORT1BD_04000703_00_LEN         1
#define PAR_PORT2BD_04000704_00_ADDR        (PAR_PORT1BD_04000703_00_ADDR + PAR_PORT1BD_04000703_00_LEN)    //通信口2通信速率特征字[暂未应用到DLT645]
#define PAR_PORT2BD_04000704_00_LEN         1
#define PAR_PORT3BD_04000705_00_ADDR        (PAR_PORT2BD_04000704_00_ADDR + PAR_PORT2BD_04000704_00_LEN)    //通信口3通信速率特征字[暂未应用到DLT645]
#define PAR_PORT3BD_04000705_00_LEN         1

#define PAR_RESTDAY_04000801_00_ADDR        (PAR_PORT3BD_04000705_00_ADDR + PAR_PORT3BD_04000705_00_LEN)    //周休日特征字[暂未应用到DLT645]
#define PAR_RESTDAY_04000801_00_LEN         1
#define PAR_RESTDAYTS_04000802_00_ADDR      (PAR_RESTDAY_04000801_00_ADDR + PAR_RESTDAY_04000801_00_LEN)    //周休日采用的日时段表号[暂未应用到DLT645]
#define PAR_RESTDAYTS_04000802_00_LEN       1

#define PAR_LOADRCDMD_04000901_00_ADDR      (PAR_RESTDAYTS_04000802_00_ADDR + PAR_RESTDAYTS_04000802_00_LEN)//负荷记录模式字
#define PAR_LOADRCDMD_04000901_00_LEN       1
#define PAR_FREEZEMD_04000902_00_ADDR       (PAR_LOADRCDMD_04000901_00_ADDR + PAR_LOADRCDMD_04000901_00_LEN)//冻结数据模式字
#define PAR_FREEZEMD_04000902_00_LEN        1

#define PAR_LOADRCDST_04000A01_00_ADDR      (PAR_FREEZEMD_04000902_00_ADDR + PAR_FREEZEMD_04000902_00_LEN)  //负荷记录起始时间
#define PAR_LOADRCDST_04000A01_00_LEN       4
#define PAR_LOAD1INTV_04000A02_00_ADDR      (PAR_LOADRCDST_04000A01_00_ADDR + PAR_LOADRCDST_04000A01_00_LEN)//第1类负荷记录间隔时间
#define PAR_LOAD1INTV_04000A02_00_LEN       2
#define PAR_LOAD2INTV_04000A03_00_ADDR      (PAR_LOAD1INTV_04000A02_00_ADDR + PAR_LOAD1INTV_04000A02_00_LEN)//第2类负荷记录间隔时间
#define PAR_LOAD2INTV_04000A03_00_LEN       2
#define PAR_LOAD3INTV_04000A04_00_ADDR      (PAR_LOAD2INTV_04000A03_00_ADDR + PAR_LOAD2INTV_04000A03_00_LEN)//第3类负荷记录间隔时间
#define PAR_LOAD3INTV_04000A04_00_LEN       2
#define PAR_LOAD4INTV_04000A05_00_ADDR      (PAR_LOAD3INTV_04000A04_00_ADDR + PAR_LOAD3INTV_04000A04_00_LEN)//第4类负荷记录间隔时间
#define PAR_LOAD4INTV_04000A05_00_LEN       2
#define PAR_LOAD5INTV_04000A06_00_ADDR      (PAR_LOAD4INTV_04000A05_00_ADDR + PAR_LOAD4INTV_04000A05_00_LEN)//第5类负荷记录间隔时间
#define PAR_LOAD5INTV_04000A06_00_LEN       2
#define PAR_LOAD6INTV_04000A07_00_ADDR      (PAR_LOAD5INTV_04000A06_00_ADDR + PAR_LOAD5INTV_04000A06_00_LEN)//第6类负荷记录间隔时间
#define PAR_LOAD6INTV_04000A07_00_LEN       2

#define PAR_FIRACCNT_04000B01_00_ADDR       (PAR_LOAD6INTV_04000A07_00_ADDR + PAR_LOAD6INTV_04000A07_00_LEN)//每月第1结算日
#define PAR_FIRACCNT_04000B01_00_LEN        2
#define PAR_SECACCNT_04000B02_00_ADDR       (PAR_LOAD5INTV_04000A06_00_ADDR + PAR_LOAD5INTV_04000A06_00_LEN)//每月第2结算日
#define PAR_SECACCNT_04000B02_00_LEN        2
#define PAR_THIACCNT_04000B03_00_ADDR       (PAR_SECACCNT_04000B02_00_ADDR + PAR_SECACCNT_04000B02_00_LEN)  //每月第3结算日
#define PAR_THIACCNT_04000B03_00_LEN        2

#define PAR_PRI0PSWD_04000C01_00_ADDR       (PAR_THIACCNT_04000B03_00_ADDR + PAR_THIACCNT_04000B03_00_LEN)  //0级密码
#define PAR_PRI0PSWD_04000C01_00_LEN        4
#define PAR_PRI1PSWD_04000C02_00_ADDR       (PAR_PRI0PSWD_04000C01_00_ADDR + PAR_PRI0PSWD_04000C01_00_LEN)  //1级密码
#define PAR_PRI1PSWD_04000C02_00_LEN        4
#define PAR_PRI2PSWD_04000C03_00_ADDR       (PAR_PRI1PSWD_04000C02_00_ADDR + PAR_PRI1PSWD_04000C02_00_LEN)  //2级密码
#define PAR_PRI2PSWD_04000C03_00_LEN        4
#define PAR_PRI3PSWD_04000C04_00_ADDR       (PAR_PRI2PSWD_04000C03_00_ADDR + PAR_PRI2PSWD_04000C03_00_LEN)  //3级密码
#define PAR_PRI3PSWD_04000C04_00_LEN        4
#define PAR_PRI4PSWD_04000C05_00_ADDR       (PAR_PRI3PSWD_04000C04_00_ADDR + PAR_PRI3PSWD_04000C04_00_LEN)  //4级密码
#define PAR_PRI4PSWD_04000C05_00_LEN        4
#define PAR_PRI5PSWD_04000C06_00_ADDR       (PAR_PRI4PSWD_04000C05_00_ADDR + PAR_PRI4PSWD_04000C05_00_LEN)  //5级密码
#define PAR_PRI5PSWD_04000C06_00_LEN        4
#define PAR_PRI6PSWD_04000C07_00_ADDR       (PAR_PRI5PSWD_04000C06_00_ADDR + PAR_PRI5PSWD_04000C06_00_LEN)  //6级密码
#define PAR_PRI6PSWD_04000C07_00_LEN        4
#define PAR_PRI7PSWD_04000C08_00_ADDR       (PAR_PRI6PSWD_04000C07_00_ADDR + PAR_PRI6PSWD_04000C07_00_LEN)  //7级密码
#define PAR_PRI7PSWD_04000C08_00_LEN        4
#define PAR_PRI8PSWD_04000C09_00_ADDR       (PAR_PRI7PSWD_04000C08_00_ADDR + PAR_PRI7PSWD_04000C08_00_LEN)  //8级密码
#define PAR_PRI8PSWD_04000C09_00_LEN        4
#define PAR_PRI9PSWD_04000C0A_00_ADDR       (PAR_PRI8PSWD_04000C09_00_ADDR + PAR_PRI8PSWD_04000C09_00_LEN)  //9级密码
#define PAR_PRI9PSWD_04000C0A_00_LEN        4
//{[暂未应用到DLT645]
#define PAR_ACONDUCT_04000D01_00_ADDR       (PAR_PRI9PSWD_04000C0A_00_ADDR + PAR_PRI9PSWD_04000C0A_00_LEN)  //A相电导系数
#define PAR_ACONDUCT_04000D01_00_LEN        2
#define PAR_ASUSCEPT_04000D02_00_ADDR       (PAR_ACONDUCT_04000D01_00_ADDR + PAR_ACONDUCT_04000D01_00_LEN)  //A相电纳系数
#define PAR_ASUSCEPT_04000D02_00_LEN        2
#define PAR_ARESISTC_04000D03_00_ADDR       (PAR_ASUSCEPT_04000D02_00_ADDR + PAR_ASUSCEPT_04000D02_00_LEN)  //A相电阻系数
#define PAR_ARESISTC_04000D03_00_LEN        2
#define PAR_AREACTC_04000D04_00_ADDR        (PAR_ARESISTC_04000D03_00_ADDR + PAR_ARESISTC_04000D03_00_LEN)  //A相电抗系数
#define PAR_AREACTC_04000D04_00_LEN         2
#define PAR_BCONDUCT_04000D05_00_ADDR       (PAR_AREACTC_04000D04_00_ADDR + PAR_AREACTC_04000D04_00_LEN)    //B相电导系数
#define PAR_BCONDUCT_04000D05_00_LEN        2
#define PAR_BSUSCEPT_04000D06_00_ADDR       (PAR_BCONDUCT_04000D05_00_ADDR + PAR_BCONDUCT_04000D05_00_LEN)  //B相电纳系数
#define PAR_BSUSCEPT_04000D06_00_LEN        2
#define PAR_BRESISTC_04000D07_00_ADDR       (PAR_BSUSCEPT_04000D06_00_ADDR + PAR_BSUSCEPT_04000D06_00_LEN)  //B相电阻系数
#define PAR_BRESISTC_04000D07_00_LEN        2
#define PAR_BREACTC_04000D08_00_ADDR        (PAR_BRESISTC_04000D07_00_ADDR + PAR_BRESISTC_04000D07_00_LEN)  //B相电抗系数
#define PAR_BREACTC_04000D04_00_LEN         2
#define PAR_CCONDUCT_04000D09_00_ADDR       (PAR_BREACTC_04000D08_00_ADDR + PAR_BREACTC_04000D04_00_LEN)    //C相电导系数
#define PAR_CCONDUCT_04000D09_00_LEN        2
#define PAR_CSUSCEPT_04000D0A_00_ADDR       (PAR_CCONDUCT_04000D09_00_ADDR + PAR_CCONDUCT_04000D09_00_LEN)  //C相电纳系数
#define PAR_CSUSCEPT_04000D0A_00_LEN        2
#define PAR_CRESISTC_04000D0B_00_ADDR       (PAR_CSUSCEPT_04000D0A_00_ADDR + PAR_CSUSCEPT_04000D0A_00_LEN)  //C相电阻系数
#define PAR_CRESISTC_04000D0B_00_LEN        2
#define PAR_CREACTC_04000D0C_00_ADDR        (PAR_CRESISTC_04000D0B_00_ADDR + PAR_CRESISTC_04000D0B_00_LEN)  //C相电抗系数
#define PAR_CREACTC_04000D0C_00_LEN         2
//}[暂未应用到DLT645]
#define PAR_POSPTOP_04000E01_00_ADDR        (PAR_CREACTC_04000D0C_00_ADDR + PAR_CREACTC_04000D0C_00_LEN)    //正向有功功率上限值
#define PAR_POSPTOP_04000E01_00_LEN         3
#define PAR_NEGPTOP_04000E02_00_ADDR        (PAR_POSPTOP_04000E01_00_ADDR + PAR_POSPTOP_04000E01_00_LEN)    //反向有功功率上限值[暂未应用到DLT645]
#define PAR_NEGPTOP_04000E02_00_LEN         3
#define PAR_VTOP_04000E03_00_ADDR           (PAR_NEGPTOP_04000E02_00_ADDR + PAR_NEGPTOP_04000E02_00_LEN)    //电压上限值
#define PAR_VTOP_04000E03_00_LEN            2
#define PAR_VBOTTOM_04000E04_00_ADDR        (PAR_VTOP_04000E03_00_ADDR + PAR_VTOP_04000E03_00_LEN)          //电压下限值
#define PAR_VBOTTOM_04000E04_00_LEN         2

#define PAR_T1TZ1SDTS_04010000_00_ADDR      (PAR_VBOTTOM_04000E04_00_ADDR + PAR_VBOTTOM_04000E04_00_LEN)    //第一套时区表数据:第1时区起始日期及日时段表号
#define PAR_T1TZ1SDTS_04010000_00_LEN       3
#define PAR_T1TZ2SDTS_04010000_01_ADDR      (PAR_T1TZ1SDTS_04010000_00_ADDR + PAR_T1TZ1SDTS_04010000_00_LEN)//第一套时区表数据:第2时区起始日期及日时段表号
#define PAR_T1TZ2SDTS_04010000_01_LEN       3
#define PAR_T1TZ3SDTS_04010000_02_ADDR      (PAR_T1TZ2SDTS_04010000_00_ADDR + PAR_T1TZ2SDTS_04010000_00_LEN)//第一套时区表数据:第3时区起始日期及日时段表号
#define PAR_T1TZ3SDTS_04010000_02_LEN       3
#define PAR_T1TZ4SDTS_04010000_03_ADDR      (PAR_T1TZ3SDTS_04010000_00_ADDR + PAR_T1T3SDTS_04010000_00_LEN)//第一套时区表数据:第4时区起始日期及日时段表号
#define PAR_T1TZ4SDTS_04010000_03_LEN       3
#define PAR_T1TZ5SDTS_04010000_04_ADDR      (PAR_T1TZ4SDTS_04010000_00_ADDR + PAR_T1TZ4SDTS_04010000_00_LEN)//第一套时区表数据:第5时区起始日期及日时段表号
#define PAR_T1TZ5SDTS_04010000_04_LEN       3
#define PAR_T1TZ6SDTS_04010000_05_ADDR      (PAR_T1TZ5SDTS_04010000_00_ADDR + PAR_T1TZ5SDTS_04010000_00_LEN)//第一套时区表数据:第6时区起始日期及日时段表号
#define PAR_T1TZ6SDTS_04010000_05_LEN       3
#define PAR_T1TZ7SDTS_04010000_06_ADDR      (PAR_T1TZ6SDTS_04010000_00_ADDR + PAR_T1TZ6SDTS_04010000_00_LEN)//第一套时区表数据:第7时区起始日期及日时段表号
#define PAR_T1TZ7SDTS_04010000_06_LEN       3
#define PAR_T1TZ8SDTS_04010000_07_ADDR      (PAR_T1TZ7SDTS_04010000_00_ADDR + PAR_T1TZ7SDTS_04010000_00_LEN)//第一套时区表数据:第8时区起始日期及日时段表号
#define PAR_T1TZ8SDTS_04010000_07_LEN       3
#define PAR_T1TZ9SDTS_04010000_08_ADDR      (PAR_T1TZ8SDTS_04010000_00_ADDR + PAR_T1TZ8SDTS_04010000_00_LEN)//第一套时区表数据:第9时区起始日期及日时段表号
#define PAR_T1TZ9SDTS_04010000_08_LEN       3
#define PAR_T1TZ10SDTS_04010000_09_ADDR     (PAR_T1TZ9SDTS_04010000_00_ADDR + PAR_T1TZ9SDTS_04010000_00_LEN)//第一套时区表数据:第10时区起始日期及日时段表号
#define PAR_T1TZ10SDTS_04010000_09_LEN      3
#define PAR_T1TZ11SDTS_04010000_0A_ADDR     (PAR_T1TZ10SDTS_04010000_00_ADDR + PAR_T1TZ10SDTS_04010000_00_LEN)  //第一套时区表数据:第11时区起始日期及日时段表号
#define PAR_T1TZ11SDTS_04010000_0A_LEN      3
#define PAR_T1TZ12SDTS_04010000_0B_ADDR     (PAR_T1TZ11SDTS_04010000_00_ADDR + PAR_T1TZ11SDTS_04010000_00_LEN)  //第一套时区表数据:第12时区起始日期及日时段表号
#define PAR_T1TZ12SDTS_04010000_0B_LEN      3
#define PAR_T1TZ13SDTS_04010000_0C_ADDR     (PAR_T1TZ12SDTS_04010000_00_ADDR + PAR_T1TZ12SDTS_04010000_00_LEN)  //第一套时区表数据:第13时区起始日期及日时段表号
#define PAR_T1TZ13SDTS_04010000_0C_LEN      3
#define PAR_T1TZ14SDTS_04010000_0D_ADDR     (PAR_T1TZ13SDTS_04010000_00_ADDR + PAR_T1TZ13SDTS_04010000_00_LEN)  //第一套时区表数据:第14时区起始日期及日时段表号
#define PAR_T1TZ14SDTS_04010000_0D_LEN      3
#define PAR_T1TZSDTS_04010000_TOT_LEN       (PAR_T1TZ1SDTS_04010000_00_LEN * 14)

#define PAR_T1TS1STFN_04010001_00_ADDR      (PAR_T1TZ1SDTS_04010000_00_ADDR + PAR_T1TZSDTS_04010000_TOT_LEN)    //第一套第1日时段表数据:第1时段起始时间及费率号
#define PAR_T1TS1STFN_04010001_00_LEN       3
#define PAR_T1TS2STFN_04010001_01_ADDR      (PAR_T1TS1STFN_04010000_00_ADDR + PAR_T1TS1STFN_04010000_00_LEN)    //第一套第1日时段表数据:第2时段起始时间及费率号
#define PAR_T1TS2STFN_04010001_01_LEN       3
#define PAR_T1TS3STFN_04010001_02_ADDR      (PAR_T1TS2STFN_04010000_01_ADDR + PAR_T1TS2STFN_04010000_01_LEN)    //第一套第1日时段表数据:第3时段起始时间及费率号
#define PAR_T1TS3STFN_04010001_02_LEN       3
#define PAR_T1TS4STFN_04010001_03_ADDR      (PAR_T1TS3STFN_04010000_02_ADDR + PAR_T1TS3STFN_04010000_02_LEN)    //第一套第1日时段表数据:第4时段起始时间及费率号
#define PAR_T1TS4STFN_04010001_03_LEN       3
#define PAR_T1TS5STFN_04010001_04_ADDR      (PAR_T1TS4STFN_04010000_03_ADDR + PAR_T1TS4STFN_04010000_03_LEN)    //第一套第1日时段表数据:第5时段起始时间及费率号
#define PAR_T1TS5STFN_04010001_04_LEN       3
#define PAR_T1TS6STFN_04010001_05_ADDR      (PAR_T1TS5STFN_04010000_04_ADDR + PAR_T1TS5STFN_04010000_04_LEN)    //第一套第1日时段表数据:第6时段起始时间及费率号
#define PAR_T1TS6STFN_04010001_05_LEN       3
#define PAR_T1TS7STFN_04010001_06_ADDR      (PAR_T1TS6STFN_04010000_05_ADDR + PAR_T1TS6STFN_04010000_05_LEN)    //第一套第1日时段表数据:第7时段起始时间及费率号
#define PAR_T1TS7STFN_04010001_06_LEN       3
#define PAR_T1TS8STFN_04010001_07_ADDR      (PAR_T1TS7STFN_04010000_06_ADDR + PAR_T1TS7STFN_04010000_06_LEN)    //第一套第1日时段表数据:第8时段起始时间及费率号
#define PAR_T1TS8STFN_04010001_07_LEN       3
#define PAR_T1TS9STFN_04010001_08_ADDR      (PAR_T1TS8STFN_04010000_07_ADDR + PAR_T1TS8STFN_04010000_07_LEN)    //第一套第1日时段表数据:第9时段起始时间及费率号
#define PAR_T1TS9STFN_04010001_08_LEN       3
#define PAR_T1TS10STFN_04010001_09_ADDR     (PAR_T1TS9STFN_04010000_08_ADDR + PAR_T1TS9STFN_04010000_08_LEN)    //第一套第1日时段表数据:第10时段起始时间及费率号
#define PAR_T1TS10STFN_04010001_09_LEN      3
#define PAR_T1TS11STFN_04010001_0A_ADDR     (PAR_T1TS10STFN_04010000_09_ADDR + PAR_T1TS10STFN_04010000_09_LEN)  //第一套第1日时段表数据:第11时段起始时间及费率号
#define PAR_T1TS11STFN_04010001_0A_LEN      3
#define PAR_T1TS12STFN_04010001_0B_ADDR     (PAR_T1TS11STFN_04010000_0A_ADDR + PAR_T1TS11STFN_04010000_0A_LEN)  //第一套第1日时段表数据:第12时段起始时间及费率号
#define PAR_T1TS12STFN_04010001_0B_LEN      3
#define PAR_T1TS13STFN_04010001_0C_ADDR     (PAR_T1TS12STFN_04010000_0B_ADDR + PAR_T1TS12STFN_04010000_0B_LEN)  //第一套第1日时段表数据:第13时段起始时间及费率号
#define PAR_T1TS13STFN_04010001_0C_LEN      3
#define PAR_T1TS14STFN_04010001_0D_ADDR     (PAR_T1TS13STFN_04010000_0C_ADDR + PAR_T1TS13STFN_04010000_0C_LEN)  //第一套第1日时段表数据:第14时段起始时间及费率号
#define PAR_T1TS14STFN_04010001_0D_LEN      3
#define PAR_T1TSSTFN_04010001_TOT_LEN       (PAR_T1TS1STFN_04010001_00_LEN * 14) //第一日时段表所有数据(1~14个时段)总长度

#define PAR_T1TS2ND_04010002_00_ADDR        (PAR_T1TS1STFN_04010001_00_ADDR + PAR_T1TSSTFN_04010001_TOT_LEN)    //第一套第2日时段表数据(起始地址)
#define PAR_T1TS2ND_04010002_00_LEN         3
#define PAR_T1TS2ND_04010002_TOT_LEN        (PAR_T1TS2ND_04010002_00_LEN * 14)
#define PAR_T1TS3TH_04010002_00_ADDR        (PAR_T1TS2ND_04010002_00_ADDR + PAR_T1TS2ND_04010002_TOT_LEN)       //第一套第3日时段表数据(起始地址)
#define PAR_T1TS3TH_04010002_00_LEN         3
#define PAR_T1TS3TH_04010002_TOT_LEN        (PAR_T1TS3TH_04010002_00_LEN * 14)
#define PAR_T1TS4TH_04010002_00_ADDR        (PAR_T1TS3TH_04010002_00_ADDR + PAR_T1TS3TH_04010002_TOT_LEN)       //第一套第4日时段表数据(起始地址)
#define PAR_T1TS4TH_04010002_00_LEN         3
#define PAR_T1TS4TH_04010002_TOT_LEN        (PAR_T1TS4TH_04010002_00_LEN * 14)
#define PAR_T1TS5TH_04010002_00_ADDR        (PAR_T1TS4TH_04010002_00_ADDR + PAR_T1TS4TH_04010002_TOT_LEN)       //第一套第5日时段表数据(起始地址)
#define PAR_T1TS5TH_04010002_00_LEN         3
#define PAR_T1TS5TH_04010002_TOT_LEN        (PAR_T1TS4TH_04010002_00_LEN * 14)
#define PAR_T1TS6TH_04010002_00_ADDR        (PAR_T1TS5TH_04010002_00_ADDR + PAR_T1TS5TH_04010002_TOT_LEN)       //第一套第6日时段表数据(起始地址)
#define PAR_T1TS6TH_04010002_00_LEN         3
#define PAR_T1TS6TH_04010002_TOT_LEN        (PAR_T1TS6TH_04010002_00_LEN * 14)
#define PAR_T1TS7TH_04010002_00_ADDR        (PAR_T1TS6TH_04010002_00_ADDR + PAR_T1TS6TH_04010002_TOT_LEN)       //第一套第7日时段表数据(起始地址)
#define PAR_T1TS7TH_04010002_00_LEN         3
#define PAR_T1TS7TH_04010002_TOT_LEN        (PAR_T1TS7TH_04010002_00_LEN * 14)
#define PAR_T1TS8TH_04010002_00_ADDR        (PAR_T1TS7TH_04010002_00_ADDR + PAR_T1TS7TH_04010002_TOT_LEN)       //第一套第8日时段表数据(起始地址)
#define PAR_T1TS8TH_04010002_00_LEN         3
#define PAR_T1TS8TH_04010002_TOT_LEN        (PAR_T1TS8TH_04010002_00_LEN * 14)
//
#define PAR_T1_TOT_LEN                      (PAR_T1TZSDTS_04010000_TOT_LEN + PAR_T1TSSTFN_04010001_TOT_LEN * 8)

#define PAR_T2TZDAT_04020000_00_ADDR        (PAR_T1TZ1SDTS_04010000_00_ADDR + PAR_T1_TOT_LEN)                   //第二套时区表数据(起始地址)
#define PAR_T2TZDAT_04020000_00_LEN         3
#define PAR_T2TZDAT_04020000_TOT_LEN        (PAR_T2TZDAT_04020000_00_LEN * 14)

#define PAR_T2TS1DAT_04020001_00_ADDR       (PAR_T2TZDAT_04020000_00_ADDR + PAR_T2TZDAT_04020000_TOT_LEN)       //第二套第1日时段表数据(起始地址)
#define PAR_T2TS1DAT_04020001_00_LEN        3
#define PAR_T2TS1DAT_04020001_TOT_LEN       (PAR_T2TS1DAT_04020001_00_LEN * 14)

#define PAR_T2TSDAT_04020002_00_ADDR        (PAR_T2TS1DAT_04020001_00_ADDR + PAR_T2TS1DAT_04020001_TOT_LEN)     //第二套第2~8日时段表数据(起始地址)
#define PAR_T2TSDAT_04020002_00_LEN         3
#define TZAMOUNT                            14                                                                  //时区数
#define TSAMOUNT                            (8 - 1)                                                             //日时段表数(2~8)
#define PAR_T2TSDAT_04020002_TOT_LEN        (PAR_T2TSDAT_04020002_00_LEN * PAR_T2TSDAT_04020002_00_LEN * TSAMOUNT)  //第2~8日时段表数据总长度
//
#define PAR_T2_TOT_LEN                      (PAR_T2TZDAT_04020000_TOT_LEN + PAR_T2TS1DAT_04020001_TOT_LEN * 8)

#define PAR_HOLIDYFN_04030001_00_ADDR       (PAR_T2TZDAT_04020000_00_ADDR + PAR_T2_TOT_LEN)                     //第1(~254)公共假日日期及日时段表号
#define PAR_HOLIDYFN_04030001_00_LEN        4
#define HOLIAMOUNT                          4//254   azh 为后面留些空间 这里反正也没多大用                                                                       //公共假日数
#define PAR_HOLIDYFN_04030001_TOT_LEN       (PAR_HOLIDYFN_04030001_00_LEN * HOLIAMOUNT)                         //公共假日数据总长度
//{[暂未应用到DLT645]
#define PAR_LOOPDIS1_04040101_00_ADDR       (PAR_HOLIDYFN_04030001_00_ADDR + PAR_HOLIDYFN_04030001_TOT_LEN)     //自动循环显示第1屏显示数据项:DI3~DI0部分
#define PAR_LOOPDIS1_04040101_00_LEN        4
#define PAR_LOOPDIS1_04040101_01_ADDR       (PAR_HOLIDYFN_04030001_00_ADDR + PAR_HOLIDYFN_04030001_TOT_LEN)     //自动循环显示第1屏显示数据项:扩展DI部分
#define PAR_LOOPDIS1_04040101_01_LEN        1
#define PAR_LOOPDIS1_04040101_TOT_LEN       (PAR_LOOPDIS1_04040101_00_LEN + PAR_LOOPDIS1_04040101_01_LEN)
#define LOOPPAGE                            128//254                                                                 //总显示屏数
#define PAR_LOOPDIS_04040101_TOT_LEN        (PAR_LOOPDIS1_04040101_TOT_LEN * LOOPPAGE)                          //循显数据总长度

#define PAR_KEYDIS1_04040201_00_ADDR        (PAR_LOOPDIS1_04040101_00_ADDR + PAR_LOOPDIS_04040101_TOT_LEN)      //按键循环显示第1屏显示数据项:扩展DI部分
#define PAR_KEYDIS1_04040201_00_LEN         4
#define PAR_KEYDIS1_04040201_01_ADDR        (PAR_KEYDIS1_04040201_00_ADDR + PAR_KEYDIS1_04040201_00_LEN)        //按键循环显示第1屏显示数据项:DI3~DI0部分
#define PAR_KEYDIS1_04040201_01_LEN         1
#define PAR_KEYDIS1_04040201_TOT_LEN        (PAR_KEYDIS1_04040201_00_LEN + PAR_KEYDIS1_04040201_01_LEN)
#define KEYPAGE                             128//254                                                                 //总显示屏数
#define PAR_KEYDIS_04040201_TOT_LEN         (PAR_KEYDIS1_04040201_TOT_LEN * KEYPAGE)                            //键显数据总长度
//}[暂未应用到DLT645]
#define PAR_SWVER_04800001_00_ADDR          (PAR_KEYDIS1_04040201_00_ADDR + PAR_KEYDIS_04040201_TOT_LEN)        //厂家软件版本号(ASCII码):N0~N3
#define PAR_SWVER_04800001_00_LEN           4
#define PAR_SWVER_04800001_01_ADDR          (PAR_SWVER_04800001_00_ADDR + PAR_SWVER_04800001_00_LEN)            //厂家软件版本号(ASCII码):N4~N7
#define PAR_SWVER_04800001_01_LEN           4
#define PAR_SWVER_04800001_02_ADDR          (PAR_SWVER_04800001_01_ADDR + PAR_SWVER_04800001_01_LEN)            //厂家软件版本号(ASCII码):N8~N11
#define PAR_SWVER_04800001_02_LEN           4
#define PAR_SWVER_04800001_03_ADDR          (PAR_SWVER_04800001_02_ADDR + PAR_SWVER_04800001_02_LEN)            //厂家软件版本号(ASCII码):N12~N15
#define PAR_SWVER_04800001_03_LEN           4
#define PAR_SWVER_04800001_04_ADDR          (PAR_SWVER_04800001_03_ADDR + PAR_SWVER_04800001_03_LEN)            //厂家软件版本号(ASCII码):N16~N19
#define PAR_SWVER_04800001_04_LEN           4
#define PAR_SWVER_04800001_05_ADDR          (PAR_SWVER_04800001_04_ADDR + PAR_SWVER_04800001_04_LEN)            //厂家软件版本号(ASCII码):N20~N23
#define PAR_SWVER_04800001_05_LEN           4
#define PAR_SWVER_04800001_06_ADDR          (PAR_SWVER_04800001_05_ADDR + PAR_SWVER_04800001_05_LEN)            //厂家软件版本号(ASCII码):N24~N27
#define PAR_SWVER_04800001_06_LEN           4
#define PAR_SWVER_04800001_07_ADDR          (PAR_SWVER_04800001_06_ADDR + PAR_SWVER_04800001_06_LEN)            //厂家软件版本号(ASCII码):N28~N31
#define PAR_SWVER_04800001_07_LEN           4
#define PAR_SWVER_04800001_TOT_LEN          (PAR_SWVER_04800001_00_LEN * 8)
#define PAR_HWVER_04800002_00_ADDR          (PAR_SWVER_04800001_00_ADDR + PAR_SWVER_04800001_TOT_LEN)           //厂家硬件版本号(ASCII码):N0~N3
#define PAR_HWVER_04800002_00_LEN           4
#define PAR_HWVER_04800002_01_ADDR          (PAR_HWVER_04800002_00_ADDR + PAR_HWVER_04800002_00_LEN)            //厂家硬件版本号(ASCII码):N4~N7
#define PAR_HWVER_04800002_01_LEN           4
#define PAR_HWVER_04800002_02_ADDR          (PAR_HWVER_04800002_01_ADDR + PAR_HWVER_04800002_01_LEN)            //厂家硬件版本号(ASCII码):N8~N11
#define PAR_HWVER_04800002_02_LEN           4
#define PAR_HWVER_04800002_03_ADDR          (PAR_HWVER_04800002_02_ADDR + PAR_HWVER_04800002_02_LEN)            //厂家硬件版本号(ASCII码):N12~N15
#define PAR_HWVER_04800002_03_LEN           4
#define PAR_HWVER_04800002_04_ADDR          (PAR_HWVER_04800002_03_ADDR + PAR_HWVER_04800002_03_LEN)            //厂家硬件版本号(ASCII码):N16~N19
#define PAR_HWVER_04800002_04_LEN           4
#define PAR_HWVER_04800002_05_ADDR          (PAR_HWVER_04800002_04_ADDR + PAR_HWVER_04800002_04_LEN)            //厂家硬件版本号(ASCII码):N20~N23
#define PAR_HWVER_04800002_05_LEN           4
#define PAR_HWVER_04800002_06_ADDR          (PAR_HWVER_04800002_05_ADDR + PAR_HWVER_04800002_05_LEN)            //厂家硬件版本号(ASCII码):N24~N27
#define PAR_HWVER_04800002_06_LEN           4
#define PAR_HWVER_04800002_07_ADDR          (PAR_HWVER_04800002_06_ADDR + PAR_HWVER_04800002_06_LEN)            //厂家硬件版本号(ASCII码):N28~N31
#define PAR_HWVER_04800002_07_LEN           4
#define PAR_HWVER_04800002_TOT_LEN          (PAR_HWVER_04800002_00_LEN * 8)

#define PAR_MFTNUM_04800003_00_ADDR         (PAR_HWVER_04800002_00_ADDR + PAR_HWVER_04800002_TOT_LEN)           //厂家编号(ASCII码):N0~N3
#define PAR_MFTNUM_04800003_00_LEN          4
#define PAR_MFTNUM_04800003_01_ADDR         (PAR_MFTNUM_04800003_00_ADDR + PAR_MFTNUM_04800003_00_LEN)            //厂家编号(ASCII码):N4~N7
#define PAR_MFTNUM_04800003_01_LEN          4
#define PAR_MFTNUM_04800003_02_ADDR         (PAR_MFTNUM_04800003_01_ADDR + PAR_MFTNUM_04800003_01_LEN)            //厂家编号(ASCII码):N8~N11
#define PAR_MFTNUM_04800003_02_LEN          4
#define PAR_MFTNUM_04800003_03_ADDR         (PAR_MFTNUM_04800003_02_ADDR + PAR_MFTNUM_04800003_02_LEN)            //厂家编号(ASCII码):N12~N15
#define PAR_MFTNUM_04800003_03_LEN          4
#define PAR_MFTNUM_04800003_04_ADDR         (PAR_MFTNUM_04800003_03_ADDR + PAR_MFTNUM_04800003_03_LEN)            //厂家编号(ASCII码):N16~N19
#define PAR_MFTNUM_04800003_04_LEN          4
#define PAR_MFTNUM_04800003_05_ADDR         (PAR_MFTNUM_04800003_04_ADDR + PAR_MFTNUM_04800003_04_LEN)            //厂家编号(ASCII码):N20~N23
#define PAR_MFTNUM_04800003_05_LEN          4
#define PAR_MFTNUM_04800003_06_ADDR         (PAR_MFTNUM_04800003_05_ADDR + PAR_MFTNUM_04800003_05_LEN)            //厂家编号(ASCII码):N24~N27
#define PAR_MFTNUM_04800003_06_LEN          4
#define PAR_MFTNUM_04800003_07_ADDR         (PAR_MFTNUM_04800003_06_ADDR + PAR_MFTNUM_04800003_06_LEN)            //厂家编号(ASCII码):N28~N31
#define PAR_MFTNUM_04800003_07_LEN          4
#define PAR_MFTNUM_04800003_TOT_LEN         (PAR_MFTNUM_04800003_00_LEN * 8)
//COME_ON
//绑定的高压表地址及其zigbee模块地址
#define PAR_HVMETER_ADR_ADDR                (PAR_MFTNUM_04800003_07_ADDR +PAR_MFTNUM_04800003_07_LEN)
#define PAR_HVMETER_ADR_LEN                 6
#define PAR_HVMETER_ADR_CRC_LEN             (6 + 1)//+1bCRC
#define PAR_HVMETER_ZIGBEE_ADR_ADDR         (PAR_HVMETER_ADR_ADDR +PAR_HVMETER_ADR_CRC_LEN)
#define PAR_HVMETER_ZIGBEE_ADR_LEN          8
#define PAR_HVMETER_ZIGBEE_ADR_CRC_LEN      (8 + 1)//+1bCRC

//有关显示参数有效性标志轮显3标志字节和键显2标志字节 1字节保留 只要是设置过就置成有效
#define PAR_DIS_VALID_FLAG_ADDR             (PAR_HVMETER_ZIGBEE_ADR_ADDR +PAR_HVMETER_ZIGBEE_ADR_CRC_LEN)
#define PAR_DIS_VALID_FLAG_LEN              (6+1)//6+1crc

//终端上次掉电时间
#define PAR_TER_LAST_TIME_ADDR              (PAR_DIS_VALID_FLAG_ADDR+PAR_DIS_VALID_FLAG_LEN)
#define PAR_TER_LAST_TIME_LEN               (6+1)       

//azh 掉电时 把当前日冻结及月冻结数据(结构数据，只保存一个测量点)保存到eeprom 上电时倒出
#define CON_DAY_DATA_BASE_ADDR              (PAR_TER_LAST_TIME_ADDR+PAR_TER_LAST_TIME_LEN)
#define CON_DAY_DATA_BASE_BLOCK             1024//预留这些字节 这个结构
#define CON_MON_DATA_BASE_ADDR              (CON_DAY_DATA_BASE_ADDR+CON_DAY_DATA_BASE_BLOCK)
#define CON_MON_DATA_BASE_BLOCK             1024//预留这些字节 这个结构

//终端参数保存地址分配宏定义(预留间隔空间PAR_DIS_VALID_FLAG_ADDR 0x61004B1Fu 约200bytes)
#define TER_PAR_HEAD_ADDR                   0x61004c00
#define TER_PAR_BAK_LEN                     0x2000//实际占用空间:0x1853

#define     F1_TERMINAL_COM_PARA_ADDR   (TER_PAR_HEAD_ADDR)                       // F1  终端通信参数AFN=04
#define     F1_TERMINAL_COM_PARA_LEN    6
#define     CRC16_LEN    2
#define     F3_STAIPADDR_ADDR           F1_TERMINAL_COM_PARA_ADDR +  (F1_TERMINAL_COM_PARA_LEN + CRC16_LEN) * 2   // F3: 主站IP地址和端口
#define     F3_STAIPADDR_LEN            28
#define     F207_ADDR_ADDR              F3_STAIPADDR_ADDR +  (F3_STAIPADDR_LEN + 2) * 2                     //F207 集中器地址，4个字节
#define     F207_ADDR_LEN               4
#define     F4_STAPHONENO_ADDR          F207_ADDR_ADDR +   (F207_ADDR_LEN + 2) * 2                          //F4：主站电话号码和短信中心号码
#define     F4_STAPHONENO_LEN           16

#define     F8_TERUPCOMWORKING_ADDR     F4_STAPHONENO_ADDR +  (F4_STAPHONENO_LEN + 2) * 2                   //F8 终端上行通信工作方式（以太专网或虚拟专网）
#define     F8_TERUPCOMWORKING_LEN      8
#define     F9_EVENTRECORDCONFSET_ADDR  F8_TERUPCOMWORKING_ADDR +  (F8_TERUPCOMWORKING_LEN + 2) * 2         //F9：事件记录配置设置
#define     F9_EVENTRECORDCONFSET_LEN   16
#define     F10_METERPARA_ADDR          F9_EVENTRECORDCONFSET_ADDR + (F9_EVENTRECORDCONFSET_LEN + 2) * 2    //F10：终端电能表/交流采样装置配置参数,最多2+2040*27，目前最多存8个测量点
#define     F10_METERPARA_LEN           (2 + 8 * 27)

#define     F25_MPBASEPARA_ADDR         F10_METERPARA_ADDR + (F10_METERPARA_LEN + 2) * 2                    //F25：测量点基本参数
#define     F25_MPBASEPARA_LEN          (11 + 2) * 8

#define     F38_JOB1PARA_ADDR           F25_MPBASEPARA_ADDR + (F25_MPBASEPARA_LEN + 2) * 2                  //F38：1类数据配置设置
#define     F38_JOB1PARA_LEN            35                                                          //1字节大类号，1字节设置组数，1字节小类号，1字节信息类组数，31字节信息类标志

#define     F39_JOB2PARA_ADDR           F38_JOB1PARA_ADDR + (F38_JOB1PARA_LEN + 2) * 2                      //F39：2类数据配置设置
#define     F39_JOB2PARA_LEN            35

#define     F65_UPDATA1PARA_ADDR        F39_JOB2PARA_ADDR + (F39_JOB2PARA_LEN + 2) * 2                      //F65：定时上报1类数据任务设置，最多64项
#define     F65_UPDATA1DUID_NUM         32
#define     F65_UPDATA1PARA_LEN         F65_UPDATA1DUID_NUM * 4 + 9
#define     F65_UPDATA1JOB_NUM          16                                                                   //暂定最多8个任务
#define     F66_UPDATA2PARA_ADDR        F65_UPDATA1PARA_ADDR + (F65_UPDATA1PARA_LEN + 2) * F65_UPDATA1JOB_NUM
#define     F66_UPDATA2DUID_NUM         32
#define     F66_UPDATA2PARA_LEN         F66_UPDATA2DUID_NUM * 4 + 9
#define     F66_UPDATA2JOB_NUM          16
#define     F67_UPDATA1CTRL_ADDR        F66_UPDATA2PARA_ADDR + (F66_UPDATA2PARA_LEN + 2) * F66_UPDATA2JOB_NUM //F67：定时发送1类数据任务启动/停止设置,55H启动，AA停止，其他无效
#define     F67_UPDATA1CTRL_LEN         1
#define     F68_UPDATA2CTRL_ADDR        F67_UPDATA1CTRL_ADDR + F67_UPDATA1CTRL_LEN * F65_UPDATA1JOB_NUM //F68：定时发送2类数据任务启动/停止设置,55H启动，AA停止，其他无效
#define     F68_UPDATA2CTRL_LEN         1

#define     F26_MPLIMIT_ADDR            F68_UPDATA2CTRL_ADDR + F68_UPDATA2CTRL_LEN * F66_UPDATA2JOB_NUM //F26：测量点限值参数
#define     F26_MPLIMIT_LEN             57
#define     F27_COPIRONLOSSPARA_ADDR    F26_MPLIMIT_ADDR + F26_MPLIMIT_LEN + 2                     //F27：测量点铜损、铁损参数
#define     F27_COPIRONLOSSPARA_LEN     24
#define     F29_METERADDR_ADDR_ADDR     F27_COPIRONLOSSPARA_ADDR + F27_COPIRONLOSSPARA_LEN + 2      //F29：终端当地电能表显示号
#define     F29_METERADDR_ADDR_LEN      12
#define     F30_METERCOPYFLAG_ADDR      F29_METERADDR_ADDR_ADDR + F29_METERADDR_ADDR_LEN + 2       //F30：台区集中抄表停抄/投抄设置
#define     F30_METERCOPYFLAG_LEN       1
#define     F33_SUMGFREEZEPARA_ADDR     F30_METERCOPYFLAG_ADDR + F30_METERCOPYFLAG_LEN + 2         //F33：终端抄表运行参数设置
#define     F33_SUMGFREEZEPARA_LEN      4 * 24 + 13 + 1                                            //多一字节用来判断是否是有效的数据
#define     F33_PORTNUM                 4
#define     F34_PORTPROPERTY_ADDR       F33_SUMGFREEZEPARA_ADDR + (F33_SUMGFREEZEPARA_LEN + 2) * F33_PORTNUM     //
#define     F34_PORTPROPERTY_LEN        15
#define     F37_LINKPARA_ADDR               F34_PORTPROPERTY_ADDR + F34_PORTPROPERTY_LEN + 2
#define     F37_LINKPARA_LEN            15
#define     F57_SOUND_PROHIBIT              (F37_LINKPARA_ADDR + F57_SOUND_PROHIBIT_LEN) + 2              // F57 声音告警允许/禁止标志
#define     F57_SOUND_PROHIBIT_LEN          3
#define     F59_METER_EXCERPTION_LIMIT      (F57_SOUND_PROHIBIT + F57_SOUND_PROHIBIT_LEN) + 2            // F59 电能表异常判定阀值
#define     F59_METER_EXCERPTION_LIMIT_LEN  4

#define     F2_RELAY_TERMINAL_SET_ADDR  (F59_METER_EXCERPTION_LIMIT + F59_METER_EXCERPTION_LIMIT_LEN + 2)   //F2  终端中继转发设置
#define     F2_RELAY_TERMINAL_SET_LEN      (16 * 2 + 1)
#define     F5_TERENCRYPT_ADDR          F2_RELAY_TERMINAL_SET_ADDR +  F2_RELAY_TERMINAL_SET_LEN + 2            //F5  终端密码设置
#define     F5_TERENCRYPT_LEN           3
#define     F6_TERGROUPADDR_ADDR        F5_TERENCRYPT_ADDR +  F5_TERENCRYPT_LEN + 2                 //F6  终端组地址设置
#define     F6_TERGROUPADDR_LEN         16
#define     F7_TERIPADDR_ADDR           F6_TERGROUPADDR_ADDR +  F6_TERGROUPADDR_LEN + 2            //F7 终端IP地址和端口
#define     F7_TERIPADDR_LEN            (24 + 20 + 20)
#define     F16_PDPINFO_ADDR            F7_TERIPADDR_ADDR + F7_TERIPADDR_LEN + 2                   //F16：虚拟专网用户名、密码
#define     F16_PDPINFO_LEN             64

#define CURRENT_LAST_ADDR               (F16_PDPINFO_ADDR + F16_PDPINFO_LEN + 2)
//在其他地方定义占用了 以下空间
#define CON_GRTASK_PARA_BASE_ADDR         0x61006c00  //temp
#define CON_GRTASK_PARA_BASE_BOLCK        0x200      //128*4(每测量点一曲线任务)可保存32个任务参数

#define CON_DAY_MONTH_INFO_BASE_ADDR      (CON_GRTASK_PARA_BASE_ADDR+CON_GRTASK_PARA_BASE_BOLCK) //国网测量点日月数据冻结数据相关参数保存
#define CON_DAY_MONTH_INFO_BASE_BLOCK     (512*4)//(CON_DAYMONTH_DATA_LEN_MP*4) //4个测量点参数预留保存的空间
//0x7600
#define CON_IMPALARM_INFO_BASE_ADDR        (CON_DAY_MONTH_INFO_BASE_ADDR+CON_DAY_MONTH_INFO_BASE_BLOCK)
#define CON_IMPALARM_INFO_BLOCK             1024//注意 实际到不了256条 最多能保存250 实际才保存120条
#define CON_NORMALARM_INFO_BASE_ADDR       (CON_IMPALARM_INFO_BASE_ADDR+CON_IMPALARM_INFO_BLOCK)
#define CON_NORMALARM_INFO_BLOCK            1024
//0x7E00
#define CON_DAY_LAST_TIME_ADDR            (CON_NORMALARM_INFO_BASE_ADDR+CON_NORMALARM_INFO_BLOCK) //日冻结上次冻结时间
#define CON_DAY_LAST_TIME_LEN             (6+1) //1byte crc
#define CON_MONTH_LAST_TIME_ADDR          (CON_DAY_LAST_TIME_ADDR+CON_DAY_LAST_TIME_LEN) //月冻结上次冻结时间
#define CON_MONTH_LAST_TIME_LEN           (6+1) //1byte crc
#define CON_FROZE_LAST_TIME_ADDR          (CON_MONTH_LAST_TIME_ADDR+CON_MONTH_LAST_TIME_LEN) //结算日冻结上次冻结时间
#define CON_FROZE_LAST_TIME_LEN           (6+1) //1byte crc
#define CON_FROZE_COPY_LAST_TIME_ADDR     (CON_FROZE_LAST_TIME_ADDR+CON_FROZE_LAST_TIME_LEN) //结算日发起的上次抄表时间
#define CON_FROZE_COPY_LAST_TIME_LEN      (6+1) //1byte crc
//主要是保存一类任务及二类任务的运行参数(目前主要指下次任务运行时间)
#define CON_JOBRUNINFO_PER_LEN              (6+1)//1crc
#define CON_JOB1RUNINFO_ADDR                (CON_FROZE_COPY_LAST_TIME_ADDR+CON_FROZE_COPY_LAST_TIME_LEN)
#define CON_JOB2RUNINFO_ADDR                (CON_JOB1RUNINFO_ADDR+CON_JOBRUNINFO_BLOCK_LEN)
#define CON_JOBRUNINFO_BLOCK_LEN            (CON_JOBRUNINFO_PER_LEN*32)//预留32个任务 实际16

//-----------------------------------------------------------------------------
// 瞬时量定义：放在全局的数组gVarArray[VAR_RAM_ARRAY_MAX]里
// 定义的地址为：0x80000000 表示RAM里数组保存的序号, 每个变量占用4字节
//-----------------------------------------------------------------------------
#define INTERNAL_RAM_HEAD_ADDR              0x80000000          //表示RAM里数组保存的序号

#define VAR_AVOLT_02010100_00_ADDR          INTERNAL_RAM_HEAD_ADDR          //A相电压
#define VAR_AVOLT_02010100_00_LEN           2
#define VAR_BVOLT_02010200_00_ADDR          (VAR_AVOLT_02010100_00_ADDR + 1)//B相电压
#define VAR_BVOLT_02010200_00_LEN           2
#define VAR_CVOLT_02010300_00_ADDR          (VAR_BVOLT_02010200_00_ADDR + 1)//C相电压
#define VAR_CVOLT_02010300_00_LEN           2

#define VAR_ACURRENT_02020100_00_ADDR       (VAR_CVOLT_02010300_00_ADDR + 1)    //A相电流
#define VAR_ACURRENT_02020100_00_LEN        3
#define VAR_BCURRENT_02020200_00_ADDR       (VAR_ACURRENT_02020100_00_ADDR + 1) //B相电流
#define VAR_BCURRENT_02020200_00_LEN        3
#define VAR_CCURRENT_02020300_00_ADDR       (VAR_BCURRENT_02020200_00_ADDR + 1) //C相电流
#define VAR_CCURRENT_02020300_00_LEN        3

#define VAR_TOTPPOW_02030000_00_ADDR        (VAR_CCURRENT_02020300_00_ADDR + 1) //瞬时有功功率
#define VAR_TOTPPOW_02030000_00_LEN         3
#define VAR_APPOW_02030100_00_ADDR          (VAR_TOTPPOW_02030000_00_ADDR + 1)  //A相瞬时有功功率
#define VAR_APPOW_02030100_00_LEN           3
#define VAR_BPPOW_02030200_00_ADDR          (VAR_APPOW_02030100_00_ADDR + 1)    //B相瞬时有功功率
#define VAR_BPPOW_02030200_00_LEN           3
#define VAR_CPPOW_02030300_00_ADDR          (VAR_BPPOW_02030200_00_ADDR + 1)    //C相瞬时有功功率
#define VAR_CPPOW_02030300_00_LEN           3

#define VAR_TOTQPOW_02040000_00_ADDR        (VAR_CPPOW_02030300_00_ADDR + 1)    //瞬时无功功率
#define VAR_TOTQPOW_02040000_00_LEN         3
#define VAR_AQPOW_02040100_00_ADDR          (VAR_TOTQPOW_02040000_00_ADDR + 1)  //A相瞬时无功功率
#define VAR_AQPOW_02040100_00_LEN           3
#define VAR_BQPOW_02040200_00_ADDR          (VAR_AQPOW_02040100_00_ADDR + 1)    //B相瞬时无功功率
#define VAR_BQPOW_02040200_00_LEN           3
#define VAR_CQPOW_02040300_00_ADDR          (VAR_BQPOW_02040200_00_ADDR + 1)    //C相瞬时无功功率
#define VAR_CQPOW_02040300_00_LEN           3

#define VAR_TOTSPOW_02050000_00_ADDR        (VAR_CQPOW_02040300_00_ADDR + 1)    //瞬时视在功率
#define VAR_TOTSPOW_02050000_00_LEN         3
#define VAR_ASPOW_02050100_00_ADDR          (VAR_TOTSPOW_02050000_00_ADDR + 1)  //A相瞬时视在功率
#define VAR_ASPOW_02050100_00_LEN           3
#define VAR_BSPOW_02050200_00_ADDR          (VAR_ASPOW_02050100_00_ADDR + 1)    //B相瞬时视在功率
#define VAR_BSPOW_02050200_00_LEN           3
#define VAR_CSPOW_02050300_00_ADDR          (VAR_BSPOW_02050200_00_ADDR + 1)    //C相瞬时视在功率
#define VAR_CSPOW_02050300_00_LEN           3

#define VAR_TOTFACT_02060000_00_ADDR        (VAR_CSPOW_02050300_00_ADDR + 1)    //功率因数
#define VAR_TOTFACT_02060000_00_LEN         2
#define VAR_AFACT_02060100_00_ADDR          (VAR_TOTFACT_02060000_00_ADDR + 1)  //A相功率因数
#define VAR_AFACT_02060100_00_LEN           2
#define VAR_BFACT_02060200_00_ADDR          (VAR_AFACT_02060100_00_ADDR + 1)    //B相功率因数
#define VAR_BFACT_02060200_00_LEN           2
#define VAR_CFACT_02060300_00_ADDR          (VAR_BFACT_02060200_00_ADDR + 1)    //C相功率因数
#define VAR_CFACT_02060300_00_LEN           2

#define VAR_AANGL_02070100_00_ADDR          (VAR_CFACT_02060300_00_ADDR + 1)    //A相相角
#define VAR_AANGL_02070100_00_LEN           2
#define VAR_BANGL_02070200_00_ADDR          (VAR_AANGL_02070100_00_ADDR + 1)    //B相相角
#define VAR_BANGL_02070200_00_LEN           2
#define VAR_CANGL_02070300_00_ADDR          (VAR_BANGL_02070200_00_ADDR + 1)    //C相相角
#define VAR_CANGL_02070300_00_LEN           2

#define VAR_AVDIST_02080100_00_ADDR         (VAR_CANGL_02070300_00_ADDR + 1)    //A相电压波形失真度(Distortion)
#define VAR_AVDIST_02080100_00_LEN          2
#define VAR_BVDIST_02080200_00_ADDR         (VAR_AVDIST_02080100_00_ADDR + 1)   //B相电压波形失真度(Distortion)
#define VAR_BVDIST_02080200_00_LEN          2
#define VAR_CVDIST_02080300_00_ADDR         (VAR_BVDIST_02080200_00_ADDR + 1)   //C相电压波形失真度(Distortion)
#define VAR_CVDIST_02080300_00_LEN          2

#define VAR_AIDIST_02090100_00_ADDR         (VAR_CVDIST_02080300_00_ADDR + 1)   //A相电流波形失真度(Distortion)
#define VAR_AIDIST_02090100_00_LEN          2
#define VAR_BIDIST_02090200_00_ADDR         (VAR_AIDIST_02090100_00_ADDR + 1)   //B相电流波形失真度(Distortion)
#define VAR_BIDIST_02090200_00_LEN          2
#define VAR_CIDIST_02090300_00_ADDR         (VAR_BIDIST_02090200_00_ADDR + 1)   //C相电流波形失真度(Distortion)
#define VAR_CIDIST_02090300_00_LEN          2

#define VAR_AV1HARM_020A0101_00_ADDR        (VAR_CIDIST_02090300_00_ADDR + 1)   //A相电压1次谐波含量(Harmonic)
#define VAR_AV1HARM_020A0101_00_LEN         2
#define VAR_AV2HARM_020A0102_00_ADDR        (VAR_AV1HARM_020A0101_00_ADDR + 1)  //A相电压2次谐波含量(Harmonic)
#define VAR_AV2HARM_020A0102_00_LEN         2
#define VAR_AV3HARM_020A0103_00_ADDR        (VAR_AV2HARM_020A0102_00_ADDR + 1)  //A相电压3次谐波含量(Harmonic)
#define VAR_AV3HARM_020A0103_00_LEN         2
#define VAR_AV4HARM_020A0104_00_ADDR        (VAR_AV3HARM_020A0103_00_ADDR + 1)  //A相电压4次谐波含量(Harmonic)
#define VAR_AV4HARM_020A0104_00_LEN         2
#define VAR_AV5HARM_020A0105_00_ADDR        (VAR_AV4HARM_020A0104_00_ADDR + 1)  //A相电压5次谐波含量(Harmonic)
#define VAR_AV5HARM_020A0105_00_LEN         2
#define VAR_AV6HARM_020A0106_00_ADDR        (VAR_AV5HARM_020A0105_00_ADDR + 1)  //A相电压6次谐波含量(Harmonic)
#define VAR_AV6HARM_020A0106_00_LEN         2
#define VAR_AV7HARM_020A0107_00_ADDR        (VAR_AV6HARM_020A0106_00_ADDR + 1)  //A相电压7次谐波含量(Harmonic)
#define VAR_AV7HARM_020A0107_00_LEN         2
#define VAR_AV8HARM_020A0108_00_ADDR        (VAR_AV7HARM_020A0107_00_ADDR + 1)  //A相电压8次谐波含量(Harmonic)
#define VAR_AV8HARM_020A0108_00_LEN         2
#define VAR_AV9HARM_020A0109_00_ADDR        (VAR_AV8HARM_020A0108_00_ADDR + 1)  //A相电压9次谐波含量(Harmonic)
#define VAR_AV9HARM_020A0109_00_LEN         2
#define VAR_AV10HARM_020A010A_00_ADDR       (VAR_AV9HARM_020A0109_00_ADDR + 1)  //A相电压10次谐波含量(Harmonic)
#define VAR_AV10HARM_020A010A_00_LEN        2
#define VAR_AV11HARM_020A010B_00_ADDR       (VAR_AV10HARM_020A010A_00_ADDR + 1) //A相电压11次谐波含量(Harmonic)
#define VAR_AV11HARM_020A010B_00_LEN        2
#define VAR_AV12HARM_020A010C_00_ADDR       (VAR_AV11HARM_020A010B_00_ADDR + 1) //A相电压12次谐波含量(Harmonic)
#define VAR_AV12HARM_020A010C_00_LEN        2
#define VAR_AV13HARM_020A010D_00_ADDR       (VAR_AV12HARM_020A010C_00_ADDR + 1) //A相电压13次谐波含量(Harmonic)
#define VAR_AV13HARM_020A010D_00_LEN        2
#define VAR_AV14HARM_020A010E_00_ADDR       (VAR_AV13HARM_020A010D_00_ADDR + 1) //A相电压14次谐波含量(Harmonic)
#define VAR_AV14HARM_020A010E_00_LEN        2
#define VAR_AV15HARM_020A010F_00_ADDR       (VAR_AV14HARM_020A010E_00_ADDR + 1) //A相电压15次谐波含量(Harmonic)
#define VAR_AV15HARM_020A010F_00_LEN        2
#define VAR_AV16HARM_020A0110_00_ADDR       (VAR_AV15HARM_020A010F_00_ADDR + 1) //A相电压16次谐波含量(Harmonic)
#define VAR_AV16HARM_020A0110_00_LEN        2
#define VAR_AV17HARM_020A0111_00_ADDR       (VAR_AV16HARM_020A0110_00_ADDR + 1) //A相电压17次谐波含量(Harmonic)
#define VAR_AV17HARM_020A0111_00_LEN        2
#define VAR_AV18HARM_020A0112_00_ADDR       (VAR_AV17HARM_020A0111_00_ADDR + 1) //A相电压18次谐波含量(Harmonic)
#define VAR_AV18HARM_020A0112_00_LEN        2
#define VAR_AV19HARM_020A0113_00_ADDR       (VAR_AV18HARM_020A0112_00_ADDR + 1) //A相电压19次谐波含量(Harmonic)
#define VAR_AV19HARM_020A0113_00_LEN        2
#define VAR_AV20HARM_020A0114_00_ADDR       (VAR_AV19HARM_020A0113_00_ADDR + 1) //A相电压20次谐波含量(Harmonic)
#define VAR_AV20HARM_020A0114_00_LEN        2
#define VAR_AV21HARM_020A0115_00_ADDR       (VAR_AV20HARM_020A0114_00_ADDR + 1) //A相电压21次谐波含量(Harmonic)
#define VAR_AV21HARM_020A0115_00_LEN        2

#define VAR_BV1HARM_020A0201_00_ADDR        (VAR_AV21HARM_020A0115_00_ADDR + 1) //B相电压1次谐波含量(Harmonic)
#define VAR_BV1HARM_020A0201_00_LEN         2
#define VAR_BV2HARM_020A0202_00_ADDR        (VAR_BV1HARM_020A0201_00_ADDR + 1)  //B相电压2次谐波含量(Harmonic)
#define VAR_BV2HARM_020A0202_00_LEN         2
#define VAR_BV3HARM_020A0203_00_ADDR        (VAR_BV2HARM_020A0202_00_ADDR + 1)  //B相电压3次谐波含量(Harmonic)
#define VAR_BV3HARM_020A0203_00_LEN         2
#define VAR_BV4HARM_020A0204_00_ADDR        (VAR_BV3HARM_020A0203_00_ADDR + 1)  //B相电压4次谐波含量(Harmonic)
#define VAR_BV4HARM_020A0204_00_LEN         2
#define VAR_BV5HARM_020A0205_00_ADDR        (VAR_BV4HARM_020A0204_00_ADDR + 1)  //B相电压5次谐波含量(Harmonic)
#define VAR_BV5HARM_020A0205_00_LEN         2
#define VAR_BV6HARM_020A0206_00_ADDR        (VAR_BV5HARM_020A0205_00_ADDR + 1)  //B相电压6次谐波含量(Harmonic)
#define VAR_BV6HARM_020A0206_00_LEN         2
#define VAR_BV7HARM_020A0207_00_ADDR        (VAR_BV6HARM_020A0206_00_ADDR + 1)  //B相电压7次谐波含量(Harmonic)
#define VAR_BV7HARM_020A0207_00_LEN         2
#define VAR_BV8HARM_020A0208_00_ADDR        (VAR_BV7HARM_020A0207_00_ADDR + 1)  //B相电压8次谐波含量(Harmonic)
#define VAR_BV8HARM_020A0208_00_LEN         2
#define VAR_BV9HARM_020A0209_00_ADDR        (VAR_BV8HARM_020A0208_00_ADDR + 1)  //B相电压9次谐波含量(Harmonic)
#define VAR_BV9HARM_020A0209_00_LEN         2
#define VAR_BV10HARM_020A020A_00_ADDR       (VAR_BV9HARM_020A0209_00_ADDR + 1)  //B相电压10次谐波含量(Harmonic)
#define VAR_BV10HARM_020A020A_00_LEN        2
#define VAR_BV11HARM_020A020B_00_ADDR       (VAR_BV10HARM_020A020A_00_ADDR + 1) //B相电压11次谐波含量(Harmonic)
#define VAR_BV11HARM_020A020B_00_LEN        2
#define VAR_BV12HARM_020A020C_00_ADDR       (VAR_BV11HARM_020A020B_00_ADDR + 1) //B相电压12次谐波含量(Harmonic)
#define VAR_BV12HARM_020A020C_00_LEN        2
#define VAR_BV13HARM_020A020D_00_ADDR       (VAR_BV12HARM_020A020C_00_ADDR + 1) //B相电压13次谐波含量(Harmonic)
#define VAR_BV13HARM_020A020D_00_LEN        2
#define VAR_BV14HARM_020A020E_00_ADDR       (VAR_BV13HARM_020A020D_00_ADDR + 1) //B相电压14次谐波含量(Harmonic)
#define VAR_BV14HARM_020A020E_00_LEN        2
#define VAR_BV15HARM_020A020F_00_ADDR       (VAR_BV14HARM_020A020E_00_ADDR + 1) //B相电压15次谐波含量(Harmonic)
#define VAR_BV15HARM_020A020F_00_LEN        2
#define VAR_BV16HARM_020A0210_00_ADDR       (VAR_BV15HARM_020A020F_00_ADDR + 1) //B相电压16次谐波含量(Harmonic)
#define VAR_BV16HARM_020A0210_00_LEN        2
#define VAR_BV17HARM_020A0211_00_ADDR       (VAR_BV16HARM_020A0210_00_ADDR + 1) //B相电压17次谐波含量(Harmonic)
#define VAR_BV17HARM_020A0211_00_LEN        2
#define VAR_BV18HARM_020A0212_00_ADDR       (VAR_BV17HARM_020A0211_00_ADDR + 1) //B相电压18次谐波含量(Harmonic)
#define VAR_BV18HARM_020A0212_00_LEN        2
#define VAR_BV19HARM_020A0213_00_ADDR       (VAR_BV18HARM_020A0212_00_ADDR + 1) //B相电压19次谐波含量(Harmonic)
#define VAR_BV19HARM_020A0213_00_LEN        2
#define VAR_BV20HARM_020A0214_00_ADDR       (VAR_BV19HARM_020A0213_00_ADDR + 1) //B相电压20次谐波含量(Harmonic)
#define VAR_BV20HARM_020A0214_00_LEN        2
#define VAR_BV21HARM_020A0215_00_ADDR       (VAR_BV20HARM_020A0214_00_ADDR + 1) //B相电压21次谐波含量(Harmonic)
#define VAR_BV21HARM_020A0215_00_LEN        2

#define VAR_CV1HARM_020A0301_00_ADDR        (VAR_BV21HARM_020A0215_00_ADDR + 1) //C相电压1次谐波含量(Harmonic)
#define VAR_CV1HARM_020A0301_00_LEN         2
#define VAR_CV2HARM_020A0302_00_ADDR        (VAR_CV1HARM_020A0301_00_ADDR + 1)  //C相电压2次谐波含量(Harmonic)
#define VAR_CV2HARM_020A0302_00_LEN         2
#define VAR_CV3HARM_020A0303_00_ADDR        (VAR_CV2HARM_020A0302_00_ADDR + 1)  //C相电压3次谐波含量(Harmonic)
#define VAR_CV3HARM_020A0303_00_LEN         2
#define VAR_CV4HARM_020A0304_00_ADDR        (VAR_CV3HARM_020A0303_00_ADDR + 1)  //C相电压4次谐波含量(Harmonic)
#define VAR_CV4HARM_020A0304_00_LEN         2
#define VAR_CV5HARM_020A0305_00_ADDR        (VAR_CV4HARM_020A0304_00_ADDR + 1)  //C相电压5次谐波含量(Harmonic)
#define VAR_CV5HARM_020A0305_00_LEN         2
#define VAR_CV6HARM_020A0306_00_ADDR        (VAR_CV5HARM_020A0305_00_ADDR + 1)  //C相电压6次谐波含量(Harmonic)
#define VAR_CV6HARM_020A0306_00_LEN         2
#define VAR_CV7HARM_020A0307_00_ADDR        (VAR_CV6HARM_020A0306_00_ADDR + 1)  //C相电压7次谐波含量(Harmonic)
#define VAR_CV7HARM_020A0307_00_LEN         2
#define VAR_CV8HARM_020A0308_00_ADDR        (VAR_CV7HARM_020A0307_00_ADDR + 1)  //C相电压8次谐波含量(Harmonic)
#define VAR_CV8HARM_020A0308_00_LEN         2
#define VAR_CV9HARM_020A0309_00_ADDR        (VAR_CV8HARM_020A0308_00_ADDR + 1)  //C相电压9次谐波含量(Harmonic)
#define VAR_CV9HARM_020A0309_00_LEN         2
#define VAR_CV10HARM_020A030A_00_ADDR       (VAR_CV9HARM_020A0309_00_ADDR + 1)  //C相电压10次谐波含量(Harmonic)
#define VAR_CV10HARM_020A030A_00_LEN        2
#define VAR_CV11HARM_020A030B_00_ADDR       (VAR_CV10HARM_020A030A_00_ADDR + 1) //C相电压11次谐波含量(Harmonic)
#define VAR_CV11HARM_020A030B_00_LEN        2
#define VAR_CV12HARM_020A030C_00_ADDR       (VAR_CV11HARM_020A030B_00_ADDR + 1) //C相电压12次谐波含量(Harmonic)
#define VAR_CV12HARM_020A030C_00_LEN        2
#define VAR_CV13HARM_020A030D_00_ADDR       (VAR_CV12HARM_020A030C_00_ADDR + 1) //C相电压13次谐波含量(Harmonic)
#define VAR_CV13HARM_020A030D_00_LEN        2
#define VAR_CV14HARM_020A030E_00_ADDR       (VAR_CV13HARM_020A030D_00_ADDR + 1) //C相电压14次谐波含量(Harmonic)
#define VAR_CV14HARM_020A030E_00_LEN        2
#define VAR_CV15HARM_020A030F_00_ADDR       (VAR_CV14HARM_020A030E_00_ADDR + 1) //C相电压15次谐波含量(Harmonic)
#define VAR_CV15HARM_020A030F_00_LEN        2
#define VAR_CV16HARM_020A0310_00_ADDR       (VAR_CV15HARM_020A030F_00_ADDR + 1) //C相电压16次谐波含量(Harmonic)
#define VAR_CV16HARM_020A0310_00_LEN        2
#define VAR_CV17HARM_020A0311_00_ADDR       (VAR_CV16HARM_020A0310_00_ADDR + 1) //C相电压17次谐波含量(Harmonic)
#define VAR_CV17HARM_020A0311_00_LEN        2
#define VAR_CV18HARM_020A0312_00_ADDR       (VAR_CV17HARM_020A0311_00_ADDR + 1) //C相电压18次谐波含量(Harmonic)
#define VAR_CV18HARM_020A0312_00_LEN        2
#define VAR_CV19HARM_020A0313_00_ADDR       (VAR_CV18HARM_020A0312_00_ADDR + 1) //C相电压19次谐波含量(Harmonic)
#define VAR_CV19HARM_020A0313_00_LEN        2
#define VAR_CV20HARM_020A0314_00_ADDR       (VAR_CV19HARM_020A0313_00_ADDR + 1) //C相电压20次谐波含量(Harmonic)
#define VAR_CV20HARM_020A0314_00_LEN        2
#define VAR_CV21HARM_020A0315_00_ADDR       (VAR_CV20HARM_020A0314_00_ADDR + 1) //C相电压21次谐波含量(Harmonic)
#define VAR_CV21HARM_020A0315_00_LEN        2

#define VAR_AI1HARM_020B0101_00_ADDR        (VAR_CV21HARM_020A0315_00_ADDR + 1) //A相电流1次谐波含量(Harmonic)
#define VAR_AI1HARM_020B0101_00_LEN         2
#define VAR_AI2HARM_020B0102_00_ADDR        (VAR_AI1HARM_020B0101_00_ADDR + 1)  //A相电流2次谐波含量(Harmonic)
#define VAR_AI2HARM_020B0102_00_LEN         2
#define VAR_AI3HARM_020B0103_00_ADDR        (VAR_AI2HARM_020B0102_00_ADDR + 1)  //A相电流3次谐波含量(Harmonic)
#define VAR_AI3HARM_020B0103_00_LEN         2
#define VAR_AI4HARM_020B0104_00_ADDR        (VAR_AI3HARM_020B0103_00_ADDR + 1)  //A相电流4次谐波含量(Harmonic)
#define VAR_AI4HARM_020B0104_00_LEN         2
#define VAR_AI5HARM_020B0105_00_ADDR        (VAR_AI4HARM_020B0104_00_ADDR + 1)  //A相电流5次谐波含量(Harmonic)
#define VAR_AI5HARM_020B0105_00_LEN         2
#define VAR_AI6HARM_020B0106_00_ADDR        (VAR_AI5HARM_020B0105_00_ADDR + 1)  //A相电流6次谐波含量(Harmonic)
#define VAR_AI6HARM_020B0106_00_LEN         2
#define VAR_AI7HARM_020B0107_00_ADDR        (VAR_AI6HARM_020B0106_00_ADDR + 1)  //A相电流7次谐波含量(Harmonic)
#define VAR_AI7HARM_020B0107_00_LEN         2
#define VAR_AI8HARM_020B0108_00_ADDR        (VAR_AI7HARM_020B0107_00_ADDR + 1)  //A相电流8次谐波含量(Harmonic)
#define VAR_AI8HARM_020B0108_00_LEN         2
#define VAR_AI9HARM_020B0109_00_ADDR        (VAR_AI8HARM_020B0108_00_ADDR + 1)  //A相电流9次谐波含量(Harmonic)
#define VAR_AI9HARM_020B0109_00_LEN         2
#define VAR_AI10HARM_020B010A_00_ADDR       (VAR_AI9HARM_020B0109_00_ADDR + 1)  //A相电流10次谐波含量(Harmonic)
#define VAR_AI10HARM_020B010A_00_LEN        2
#define VAR_AI11HARM_020B010B_00_ADDR       (VAR_AI10HARM_020B010A_00_ADDR + 1) //A相电流11次谐波含量(Harmonic)
#define VAR_AI11HARM_020B010B_00_LEN        2
#define VAR_AI12HARM_020B010C_00_ADDR       (VAR_AI11HARM_020B010B_00_ADDR + 1) //A相电流12次谐波含量(Harmonic)
#define VAR_AI12HARM_020B010C_00_LEN        2
#define VAR_AI13HARM_020B010D_00_ADDR       (VAR_AI12HARM_020B010C_00_ADDR + 1) //A相电流13次谐波含量(Harmonic)
#define VAR_AI13HARM_020B010D_00_LEN        2
#define VAR_AI14HARM_020B010E_00_ADDR       (VAR_AI13HARM_020B010D_00_ADDR + 1) //A相电流14次谐波含量(Harmonic)
#define VAR_AI14HARM_020B010E_00_LEN        2
#define VAR_AI15HARM_020B010F_00_ADDR       (VAR_AI14HARM_020B010E_00_ADDR + 1) //A相电流15次谐波含量(Harmonic)
#define VAR_AI15HARM_020B010F_00_LEN        2
#define VAR_AI16HARM_020B0110_00_ADDR       (VAR_AI15HARM_020B010F_00_ADDR + 1) //A相电流16次谐波含量(Harmonic)
#define VAR_AI16HARM_020B0110_00_LEN        2
#define VAR_AI17HARM_020B0111_00_ADDR       (VAR_AI16HARM_020B0110_00_ADDR + 1) //A相电流17次谐波含量(Harmonic)
#define VAR_AI17HARM_020B0111_00_LEN        2
#define VAR_AI18HARM_020B0112_00_ADDR       (VAR_AI17HARM_020B0111_00_ADDR + 1) //A相电流18次谐波含量(Harmonic)
#define VAR_AI18HARM_020B0112_00_LEN        2
#define VAR_AI19HARM_020B0113_00_ADDR       (VAR_AI18HARM_020B0112_00_ADDR + 1) //A相电流19次谐波含量(Harmonic)
#define VAR_AI19HARM_020B0113_00_LEN        2
#define VAR_AI20HARM_020B0114_00_ADDR       (VAR_AI19HARM_020B0113_00_ADDR + 1) //A相电流20次谐波含量(Harmonic)
#define VAR_AI20HARM_020B0114_00_LEN        2
#define VAR_AI21HARM_020B0115_00_ADDR       (VAR_AI20HARM_020B0114_00_ADDR + 1) //A相电流21次谐波含量(Harmonic)
#define VAR_AI21HARM_020B0115_00_LEN        2

#define VAR_BI1HARM_020B0201_00_ADDR        (VAR_AI21HARM_020B0115_00_ADDR + 1) //B相电流1次谐波含量(Harmonic)
#define VAR_BI1HARM_020B0201_00_LEN         2
#define VAR_BI2HARM_020B0202_00_ADDR        (VAR_BI1HARM_020B0201_00_ADDR + 1)  //B相电流2次谐波含量(Harmonic)
#define VAR_BI2HARM_020B0202_00_LEN         2
#define VAR_BI3HARM_020B0203_00_ADDR        (VAR_BI2HARM_020B0202_00_ADDR + 1)  //B相电流3次谐波含量(Harmonic)
#define VAR_BI3HARM_020B0203_00_LEN         2
#define VAR_BI4HARM_020B0204_00_ADDR        (VAR_BI3HARM_020B0203_00_ADDR + 1)  //B相电流4次谐波含量(Harmonic)
#define VAR_BI4HARM_020B0204_00_LEN         2
#define VAR_BI5HARM_020B0205_00_ADDR        (VAR_BI4HARM_020B0204_00_ADDR + 1)  //B相电流5次谐波含量(Harmonic)
#define VAR_BI5HARM_020B0205_00_LEN         2
#define VAR_BI6HARM_020B0206_00_ADDR        (VAR_BI5HARM_020B0205_00_ADDR + 1)  //B相电流6次谐波含量(Harmonic)
#define VAR_BI6HARM_020B0206_00_LEN         2
#define VAR_BI7HARM_020B0207_00_ADDR        (VAR_BI6HARM_020B0206_00_ADDR + 1)  //B相电流7次谐波含量(Harmonic)
#define VAR_BI7HARM_020B0207_00_LEN         2
#define VAR_BI8HARM_020B0208_00_ADDR        (VAR_BI7HARM_020B0207_00_ADDR + 1)  //B相电流8次谐波含量(Harmonic)
#define VAR_BI8HARM_020B0208_00_LEN         2
#define VAR_BI9HARM_020B0209_00_ADDR        (VAR_BI8HARM_020B0208_00_ADDR + 1)  //B相电流9次谐波含量(Harmonic)
#define VAR_BI9HARM_020B0209_00_LEN         2
#define VAR_BI10HARM_020B020A_00_ADDR       (VAR_BI9HARM_020B0209_00_ADDR + 1)  //B相电流10次谐波含量(Harmonic)
#define VAR_BI10HARM_020B020A_00_LEN        2
#define VAR_BI11HARM_020B020B_00_ADDR       (VAR_BI10HARM_020B020A_00_ADDR + 1) //B相电流11次谐波含量(Harmonic)
#define VAR_BI11HARM_020B020B_00_LEN        2
#define VAR_BI12HARM_020B020C_00_ADDR       (VAR_BI11HARM_020B020B_00_ADDR + 1) //B相电流12次谐波含量(Harmonic)
#define VAR_BI12HARM_020B020C_00_LEN        2
#define VAR_BI13HARM_020B020D_00_ADDR       (VAR_BI12HARM_020B020C_00_ADDR + 1) //B相电流13次谐波含量(Harmonic)
#define VAR_BI13HARM_020B020D_00_LEN        2
#define VAR_BI14HARM_020B020E_00_ADDR       (VAR_BI13HARM_020B020D_00_ADDR + 1) //B相电流14次谐波含量(Harmonic)
#define VAR_BI14HARM_020B020E_00_LEN        2
#define VAR_BI15HARM_020B020F_00_ADDR       (VAR_BI14HARM_020B020E_00_ADDR + 1) //B相电流15次谐波含量(Harmonic)
#define VAR_BI15HARM_020B020F_00_LEN        2
#define VAR_BI16HARM_020B0210_00_ADDR       (VAR_BI15HARM_020B020F_00_ADDR + 1) //B相电流16次谐波含量(Harmonic)
#define VAR_BI16HARM_020B0210_00_LEN        2
#define VAR_BI17HARM_020B0211_00_ADDR       (VAR_BI16HARM_020B0210_00_ADDR + 1) //B相电流17次谐波含量(Harmonic)
#define VAR_BI17HARM_020B0211_00_LEN        2
#define VAR_BI18HARM_020B0212_00_ADDR       (VAR_BI17HARM_020B0211_00_ADDR + 1) //B相电流18次谐波含量(Harmonic)
#define VAR_BI18HARM_020B0212_00_LEN        2
#define VAR_BI19HARM_020B0213_00_ADDR       (VAR_BI18HARM_020B0212_00_ADDR + 1) //B相电流19次谐波含量(Harmonic)
#define VAR_BI19HARM_020B0213_00_LEN        2
#define VAR_BI20HARM_020B0214_00_ADDR       (VAR_BI19HARM_020B0213_00_ADDR + 1) //B相电流20次谐波含量(Harmonic)
#define VAR_BI20HARM_020B0214_00_LEN        2
#define VAR_BI21HARM_020B0215_00_ADDR       (VAR_BI20HARM_020B0214_00_ADDR + 1) //B相电流21次谐波含量(Harmonic)
#define VAR_BI21HARM_020B0215_00_LEN        2

#define VAR_CI1HARM_020B0301_00_ADDR        (VAR_BI21HARM_020B0215_00_ADDR + 1) //C相电流1次谐波含量(Harmonic)
#define VAR_CI1HARM_020B0301_00_LEN         2
#define VAR_CI2HARM_020B0302_00_ADDR        (VAR_CI1HARM_020B0301_00_ADDR + 1)  //C相电流2次谐波含量(Harmonic)
#define VAR_CI2HARM_020B0302_00_LEN         2
#define VAR_CI3HARM_020B0303_00_ADDR        (VAR_CI2HARM_020B0302_00_ADDR + 1)  //C相电流3次谐波含量(Harmonic)
#define VAR_CI3HARM_020B0303_00_LEN         2
#define VAR_CI4HARM_020B0304_00_ADDR        (VAR_CI3HARM_020B0303_00_ADDR + 1)  //C相电流4次谐波含量(Harmonic)
#define VAR_CI4HARM_020B0304_00_LEN         2
#define VAR_CI5HARM_020B0305_00_ADDR        (VAR_CI4HARM_020B0304_00_ADDR + 1)  //C相电流5次谐波含量(Harmonic)
#define VAR_CI5HARM_020B0305_00_LEN         2
#define VAR_CI6HARM_020B0306_00_ADDR        (VAR_CI5HARM_020B0305_00_ADDR + 1)  //C相电流6次谐波含量(Harmonic)
#define VAR_CI6HARM_020B0306_00_LEN         2
#define VAR_CI7HARM_020B0307_00_ADDR        (VAR_CI6HARM_020B0306_00_ADDR + 1)  //C相电流7次谐波含量(Harmonic)
#define VAR_CI7HARM_020B0307_00_LEN         2
#define VAR_CI8HARM_020B0308_00_ADDR        (VAR_CI7HARM_020B0307_00_ADDR + 1)  //C相电流8次谐波含量(Harmonic)
#define VAR_CI8HARM_020B0308_00_LEN         2
#define VAR_CI9HARM_020B0309_00_ADDR        (VAR_CI8HARM_020B0308_00_ADDR + 1)  //C相电流9次谐波含量(Harmonic)
#define VAR_CI9HARM_020B0309_00_LEN         2
#define VAR_CI10HARM_020B030A_00_ADDR       (VAR_CI9HARM_020B0309_00_ADDR + 1)  //C相电流10次谐波含量(Harmonic)
#define VAR_CI10HARM_020B030A_00_LEN        2
#define VAR_CI11HARM_020B030B_00_ADDR       (VAR_CI10HARM_020B030A_00_ADDR + 1) //C相电流11次谐波含量(Harmonic)
#define VAR_CI11HARM_020B030B_00_LEN        2
#define VAR_CI12HARM_020B030C_00_ADDR       (VAR_CI11HARM_020B030B_00_ADDR + 1) //C相电流12次谐波含量(Harmonic)
#define VAR_CI12HARM_020B030C_00_LEN        2
#define VAR_CI13HARM_020B030D_00_ADDR       (VAR_CI12HARM_020B030C_00_ADDR + 1) //C相电流13次谐波含量(Harmonic)
#define VAR_CI13HARM_020B030D_00_LEN        2
#define VAR_CI14HARM_020B030E_00_ADDR       (VAR_CI13HARM_020B030D_00_ADDR + 1) //C相电流14次谐波含量(Harmonic)
#define VAR_CI14HARM_020B030E_00_LEN        2
#define VAR_CI15HARM_020B030F_00_ADDR       (VAR_CI14HARM_020B030E_00_ADDR + 1) //C相电流15次谐波含量(Harmonic)
#define VAR_CI15HARM_020B030F_00_LEN        2
#define VAR_CI16HARM_020B0310_00_ADDR       (VAR_CI15HARM_020B030F_00_ADDR + 1) //C相电流16次谐波含量(Harmonic)
#define VAR_CI16HARM_020B0310_00_LEN        2
#define VAR_CI17HARM_020B0311_00_ADDR       (VAR_CI16HARM_020B0310_00_ADDR + 1) //C相电流17次谐波含量(Harmonic)
#define VAR_CI17HARM_020B0311_00_LEN        2
#define VAR_CI18HARM_020B0312_00_ADDR       (VAR_CI17HARM_020B0311_00_ADDR + 1) //C相电流18次谐波含量(Harmonic)
#define VAR_CI18HARM_020B0312_00_LEN        2
#define VAR_CI19HARM_020B0313_00_ADDR       (VAR_CI18HARM_020B0312_00_ADDR + 1) //C相电流19次谐波含量(Harmonic)
#define VAR_CI19HARM_020B0313_00_LEN        2
#define VAR_CI20HARM_020B0314_00_ADDR       (VAR_CI19HARM_020B0313_00_ADDR + 1) //C相电流20次谐波含量(Harmonic)
#define VAR_CI20HARM_020B0314_00_LEN        2
#define VAR_CI21HARM_020B0315_00_ADDR       (VAR_CI20HARM_020B0314_00_ADDR + 1) //C相电流21次谐波含量(Harmonic)
#define VAR_CI21HARM_020B0315_00_LEN        2
#define VAR_IN_02800001_00_ADDR             (VAR_CI21HARM_020B0315_00_ADDR + 1) //零线电流

#define VAR_IN_02800001_00_LEN              3
#define VAR_FREQ_02800002_00_ADDR           (VAR_IN_02800001_00_ADDR + 1)       //电网频率
#define VAR_FREQ_02800002_00_LEN            2
#define VAR_AVEPMIN_02800003_00_ADDR        (VAR_FREQ_02800002_00_ADDR + 1)     //一分钟有功总平均功率
#define VAR_AVEPMIN_02800003_00_LEN         3
#define VAR_DMDP_02800004_00_ADDR           (VAR_AVEPMIN_02800003_00_ADDR + 1)  //当前有功需量
#define VAR_DMDP_02800004_00_LEN            3
#define VAR_DMDQ_02800005_00_ADDR           (VAR_DMDP_02800004_00_ADDR + 1)     //当前无功需量
#define VAR_DMDQ_02800005_00_LEN            3
#define VAR_DMDS_02800006_00_ADDR           (VAR_DMDQ_02800005_00_ADDR + 1)     //当前视在需量
#define VAR_DMDS_02800006_00_LEN            3
#define VAR_TEMPIN_02800007_00_ADDR         (VAR_DMDS_02800006_00_ADDR + 1)     //表内温度
#define VAR_TEMPIN_02800007_00_LEN          2
#define VAR_VRTCBAT_02800008_00_ADDR        (VAR_TEMPIN_02800007_00_ADDR + 1)   //时钟电池电压(内部)
#define VAR_VRTCBAT_02800008_00_LEN         2
#define VAR_VSYSBAT_02800009_00_ADDR        (VAR_VRTCBAT_02800008_00_ADDR + 1)  //停电抄表电池电压 (外部)
#define VAR_VSYSBAT_02800009_00_LEN         2
#define VAR_TRTCBAT_0280000A_00_ADDR        (VAR_VSYSBAT_02800009_00_ADDR + 1)  //内部电池工作时间
#define VAR_TRTCBAT_0280000A_00_LEN         4

#define VAR_RAM_ARRAY_MAX                   (VAR_TRTCBAT_0280000A_00_ADDR - INTERNAL_RAM_HEAD_ADDR + 1 + 18)  //数组下标大小
#define PAR_BASE_INDEX                      (VAR_RAM_ARRAY_MAX - 18)            //变量数组起存参变量的数组下标
//最后的18个字用来固定存放[(日期&周次标识及数据),(时间标识及数据),(电表运行状态字1标识及数据),(……),(电表运行状态字7标识及数据)]

#endif//__VAR_DEFINE_H

/******************************************************************************
    End Of File
******************************************************************************/

/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : firmware_upgrade.h
 Description    : 用于代码升级, 大部分函数不可重入, 运行时阻断处理器正常程序运行
 Related Files  : firmware_upgrade.c file_head_info.h
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2010-09-16    wwllzz          the original version
******************************************************************************/
#ifndef __FIRMWARE_UPGRADE_H
#define __FIRMWARE_UPGRADE_H

#ifdef DISPLAY_TERMINAL
#include "..\inc\global_config.h"
#endif//DISPLAY_TERMINAL

//----------------------------- macro definition ------------------------------
#define FIRST_RESPONSE                                  //先回应后再编程写入的方式

#define PROGRAM_CODE_AFTER_ANSWER   0x01                //改变表明回应帧发完后需要编程代码
#define COM_NUM_ON_BIT          (3 << 1)                //串口号占用的比特位置
#define MAIN_LOOP_TEST_BIT      (1 << 3)                //主循环测试位

//#ifdef __DEBUG
//#define RANDOM_LIFE             255
//#define PROGRAM_LIFE            255
//#define UPGRADE_LIFE            255
//#else   //__DEBUG
#define RANDOM_LIFE             5                       //随机数有效时间
#define PROGRAM_LIFE            180                     //签名有效时间
#define UPGRADE_LIFE            60
#define CLEAR_LIFE              10
//#endif  //__DEBUG

//-----------------------------------------------------------------------------
// 处理器容量定义
//-----------------------------------------------------------------------------
//#define CODE_MEM_SIZE           256     //对LPC1763/65/66系列芯片的代码总容量256K
#define CODE_MEM_SIZE           512     //对LPC1767/68/69系列芯片的代码总容量512K

#define IAP_LOCATION            0x1fff1ff1

#define RUN_USING_FLAG          0x1000                  //程序运行地址标识存储位置

#define CRC_AND_RUN_INVALID_FLAG (RUN_USING_FLAG+4)     //验证标识字
#define LOW_RUN_INVALID         (1<<0)                  //低区未曾有效运行, 0x1004字节的位定义
#define HIGH_RUN_INVALID        (1<<1)                  //高区未曾有效运行
#define LOW_CRC_INVALID         (1<<2)                  //低区CRC未验证通过
#define HIGH_CRC_INVALID        (1<<3)                  //高区CRC未验证通过

#ifndef DISPLAY_TERMINAL
#if CODE_MEM_SIZE == 256
#define LOW_CODE_START          0x00002000ul            //低地址区开始位置
#define LOW_CODE_END            0x0001fffful            //低地址区结束位置
#define LOW_BLOCK_START         2
#define LOW_BLOCK_END           17
#define HIGH_CODE_START         0x00020000ul            //高地址区开始位置
#define HIGH_CODE_END           0x0003fffful            //高地址区结束位置
#define HIGH_BLOCK_START        18
#define HIGH_BLOCK_END          21
#endif  // CODE_MEM_SIZE

#if CODE_MEM_SIZE == 512
#define LOW_CODE_START          0x00002000ul            //低地址区开始位置
#define LOW_CODE_END            0x0003fffful            //低地址区结束位置
#define LOW_BLOCK_START         2
#define LOW_BLOCK_END           21
#define HIGH_CODE_START         0x00040000ul            //高地址区开始位置
#define HIGH_CODE_END           0x0007fffful            //高地址区结束位置
#define HIGH_BLOCK_START        22
#define HIGH_BLOCK_END          29
#endif  // CODE_MEM_SIZE

#define FLAG_FLASH_MAX_READ     4096                    //标识FLASH区保护长度, 权衡RAM和FALSH的资源消耗

#else   //DISPLAY_TERMINAL

#if CODE_MEM_SIZE == 256
#define LOW_CODE_START          0x00010000ul            //低地址区开始位置
#define LOW_CODE_END            0x00027ffful            //低地址区结束位置
#define LOW_BLOCK_START         16
#define LOW_BLOCK_END           18
#define HIGH_CODE_START         0x00028000ul            //高地址区开始位置
#define HIGH_CODE_END           0x0003fffful            //高地址区结束位置
#define HIGH_BLOCK_START        19
#define HIGH_BLOCK_END          21
#endif  // CODE_MEM_SIZE

#if CODE_MEM_SIZE == 512
//#define LOW_CODE_START          0x00020000ul            //低地址区开始位置
//#define LOW_CODE_END            0x0004fffful            //低地址区结束位置
//#define LOW_BLOCK_START         18
//#define LOW_BLOCK_END           23
//#define HIGH_CODE_START         0x00050000ul            //高地址区开始位置
//#define HIGH_CODE_END           0x0007fffful            //高地址区结束位置
//#define HIGH_BLOCK_START        24
//#define HIGH_BLOCK_END          29
//azh 由上面的124K常数表格改成60K
//#define LOW_CODE_START          0x00010000ul            //低地址区开始位置
//#define LOW_CODE_END            0x00047ffful            //低地址区结束位置
//#define LOW_BLOCK_START         16
//#define LOW_BLOCK_END           22
//#define HIGH_CODE_START         0x00048000ul            //高地址区开始位置
//#define HIGH_CODE_END           0x0007fffful            //高地址区结束位置
//#define HIGH_BLOCK_START        23
//#define HIGH_BLOCK_END          29
//azh 130314 由于国网代码空间不够 把常数区显示表格去掉
#define LOW_CODE_START          0x00002000ul            //低地址区开始位置
#define LOW_CODE_END            0x0003fffful            //低地址区结束位置
#define LOW_BLOCK_START         2
#define LOW_BLOCK_END           21
#define HIGH_CODE_START         0x00040000ul            //高地址区开始位置
#define HIGH_CODE_END           0x0007fffful            //高地址区结束位置
#define HIGH_BLOCK_START        22
#define HIGH_BLOCK_END          29
#endif  // CODE_MEM_SIZE

#ifdef DISPLAY_TERMINAL_2
#define FLAG_FLASH_MAX_READ     4096                    //采用浪费RAM节约FLASH, 省得改程序的方案
#else   //DISPLAY_TERMINAL_2
#define FLAG_FLASH_MAX_READ     256                     //标识FLASH区保护长度, 权衡RAM和FALSH的资源消耗, 显示终端RAM紧张
#endif  //DISPLAY_TERMINAL_2

#endif  //DISPLAY_TERMINAL

#ifdef __RELEASE_L
#define BLOCK_START             LOW_BLOCK_START
#define BLOCK_END               LOW_BLOCK_END
#define CODE_START_ADDRSS       LOW_CODE_START
#define CODE_END_ADDRESS        LOW_CODE_END
#define UPGRADE_INFO_ADDR       (HIGH_CODE_END - 63)
#endif  //__RELEASE_L
#ifdef __RELEASE_H
#define BLOCK_START             HIGH_BLOCK_START
#define BLOCK_END               HIGH_BLOCK_END
#define CODE_START_ADDRSS       HIGH_CODE_START
#define CODE_END_ADDRESS        HIGH_CODE_END
#define UPGRADE_INFO_ADDR       (LOW_CODE_END - 63)
#endif  //__RELEASE_H

#ifndef DISPLAY_TERMINAL
#define SOFT_RESET_DELAY_TIME   (5 * 1000 / SYS_TICK)   //软复位延迟等待时间, 5秒, Tick=5mS
#else   //DISPLAY_TERMINAL
#define SOFT_RESET_DELAY_TIME   (5 * 1000 / (OS_TICK / 1000))

#define TEST_FLAG_BIT(x, m)     (x & (m))               //位测试
#define SET_FLAG_BIT(x, m)      (x |= (m))              //位置1
#define CLR_FLAG_BIT(x, m)      (x &= (~(m)))           //位清零
//下面二个都要在管理模式下进行 共用一个函数 入口ID不一样而已
#define GET_RUN_ADDR_ADMIN      0//获取当前运行地址
#define SOFT_RST_ADMIN          1//软件复位

#endif  //DISPLAY_TERMINAL

//----------------------------- type definition -------------------------------
typedef enum                                            //IAP指令码
{
    PREPARE_SECTOR_FOR_WRITE = 50,
    COPY_RAM_TO_FLASH = 51,
    ERASE_SECTOR = 52,
    BLANK_CHECK_SECTOR = 53,
    READ_PART_ID = 54,
    READ_BOOT_VER = 55,
    COMPARE = 56,
    REINVOKE_ISP = 57,
    READ_DEVICE_SERIAL_NUMBER = 58
}IAP_Command_Code;

typedef enum                                            //IAP指令返回的结果码
{
    CMD_SUCCESS = 0,
    INVALID_COMMAND = 1,                                //指令码无效
    SRC_ADDR_ERROR = 2,                                 //Source address not on word boundary
    DST_ADDR_ERROR = 3,                                 //Destination address not on word or 256 byte boundary
    SRC_ADDR_NOT_MAPPED = 4,
    DST_ADDR_NOT_MAPPED = 5,
    COUNT_ERROR = 6,                                    //Byte count is not multiple of 4 or is not a permitted value
    INVALID_SECTOR = 7,                                 //SECTOR无效
    SECTOR_NOT_BLANK = 8,                               //SECTOR非空
    SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION = 9,        //SECTOR未指定
    COMPARE_ERROR = 10,                                 //比较时出错
    BUSY = 11,                                          //Flash programming hardware interface is busy
    PARAM_ERROR = 12,                                   //Insufficient number of parameters
    ADDR_ERROR_FLASH = 13,                              //Address not on word boundary
    ADDR_NOT_MAPPED = 14,
    CMD_LOCKED = 15,                                    //Command is locked
    INVALID_CODE = 16,                                  //Unlock code is invalid
    INVALID_BAUD_RATE = 17,
    INVALID_STOP_BIT = 18,
    CODE_READ_PROTECTION_ENABLED = 19
}Command_ErrorCode;

typedef struct
{
    U8 random[8];
    U8 life;
    U8 upgrade_life;
    U8 clear_life;
    U8 programe_life;
}KEY_RANDOM_STATUS;

//-------------------------- functions declaration ----------------------------
__EXTERN U8 activate_FW(U8 index);
__EXTERN S32 get_monitoring_status(U8 addr_buf[]);
__EXTERN void set_normal_run_flag(U32 c);
__EXTERN void soft_reset_function(void);
#ifdef FIRST_RESPONSE
__EXTERN U32 check_package_code(U8 buf[], U8 com, U32 len);
__EXTERN void program_package_code(void);
//__EXTERN U8 check_programing(void);
//__EXTERN U32 upgrading_on(void);
#else   //FIRST_RESPONSE
__EXTERN U8 write_package_code(U8 buf[]);
#endif  //FIRST_RESPONSE

#ifdef IDEA_ACC
__EXTERN void code_signature(void);
//__EXTERN void get_random(void);
#endif  //IDEA_ACC
#ifdef DISPLAY_TERMINAL
__EXTERN void iap_entry(U32 param_tab[], U32 result_tab[]);
#endif//DISPLAY_TERMINAL
//--------------------------- variable declaration ----------------------------
__EXTERN KEY_RANDOM_STATUS Random_Status;

//#ifdef PRINT_SLAVE_DBG
//__EXTERN VU32 CrdyCount;                                //一次FLASH操作的T1计数
//#endif  //PRINT_SLAVE_DBG

//__EXTERN U32 signature_secret(U32 index, U8 buf[]);

#endif  //__FIRMWARE_UPGRADE_H

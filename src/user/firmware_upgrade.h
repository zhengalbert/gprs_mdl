/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : firmware_upgrade.h
 Description    : ���ڴ�������, �󲿷ֺ�����������, ����ʱ��ϴ�����������������
 Related Files  : firmware_upgrade.c file_head_info.h
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��    1.00        2010-09-16    wwllzz          the original version
******************************************************************************/
#ifndef __FIRMWARE_UPGRADE_H
#define __FIRMWARE_UPGRADE_H

#ifdef DISPLAY_TERMINAL
#include "..\inc\global_config.h"
#endif//DISPLAY_TERMINAL

//----------------------------- macro definition ------------------------------
#define FIRST_RESPONSE                                  //�Ȼ�Ӧ���ٱ��д��ķ�ʽ

#define PROGRAM_CODE_AFTER_ANSWER   0x01                //�ı������Ӧ֡�������Ҫ��̴���
#define COM_NUM_ON_BIT          (3 << 1)                //���ں�ռ�õı���λ��
#define MAIN_LOOP_TEST_BIT      (1 << 3)                //��ѭ������λ

//#ifdef __DEBUG
//#define RANDOM_LIFE             255
//#define PROGRAM_LIFE            255
//#define UPGRADE_LIFE            255
//#else   //__DEBUG
#define RANDOM_LIFE             5                       //�������Чʱ��
#define PROGRAM_LIFE            180                     //ǩ����Чʱ��
#define UPGRADE_LIFE            60
#define CLEAR_LIFE              10
//#endif  //__DEBUG

//-----------------------------------------------------------------------------
// ��������������
//-----------------------------------------------------------------------------
//#define CODE_MEM_SIZE           256     //��LPC1763/65/66ϵ��оƬ�Ĵ���������256K
#define CODE_MEM_SIZE           512     //��LPC1767/68/69ϵ��оƬ�Ĵ���������512K

#define IAP_LOCATION            0x1fff1ff1

#define RUN_USING_FLAG          0x1000                  //�������е�ַ��ʶ�洢λ��

#define CRC_AND_RUN_INVALID_FLAG (RUN_USING_FLAG+4)     //��֤��ʶ��
#define LOW_RUN_INVALID         (1<<0)                  //����δ����Ч����, 0x1004�ֽڵ�λ����
#define HIGH_RUN_INVALID        (1<<1)                  //����δ����Ч����
#define LOW_CRC_INVALID         (1<<2)                  //����CRCδ��֤ͨ��
#define HIGH_CRC_INVALID        (1<<3)                  //����CRCδ��֤ͨ��

#ifndef DISPLAY_TERMINAL
#if CODE_MEM_SIZE == 256
#define LOW_CODE_START          0x00002000ul            //�͵�ַ����ʼλ��
#define LOW_CODE_END            0x0001fffful            //�͵�ַ������λ��
#define LOW_BLOCK_START         2
#define LOW_BLOCK_END           17
#define HIGH_CODE_START         0x00020000ul            //�ߵ�ַ����ʼλ��
#define HIGH_CODE_END           0x0003fffful            //�ߵ�ַ������λ��
#define HIGH_BLOCK_START        18
#define HIGH_BLOCK_END          21
#endif  // CODE_MEM_SIZE

#if CODE_MEM_SIZE == 512
#define LOW_CODE_START          0x00002000ul            //�͵�ַ����ʼλ��
#define LOW_CODE_END            0x0003fffful            //�͵�ַ������λ��
#define LOW_BLOCK_START         2
#define LOW_BLOCK_END           21
#define HIGH_CODE_START         0x00040000ul            //�ߵ�ַ����ʼλ��
#define HIGH_CODE_END           0x0007fffful            //�ߵ�ַ������λ��
#define HIGH_BLOCK_START        22
#define HIGH_BLOCK_END          29
#endif  // CODE_MEM_SIZE

#define FLAG_FLASH_MAX_READ     4096                    //��ʶFLASH����������, Ȩ��RAM��FALSH����Դ����

#else   //DISPLAY_TERMINAL

#if CODE_MEM_SIZE == 256
#define LOW_CODE_START          0x00010000ul            //�͵�ַ����ʼλ��
#define LOW_CODE_END            0x00027ffful            //�͵�ַ������λ��
#define LOW_BLOCK_START         16
#define LOW_BLOCK_END           18
#define HIGH_CODE_START         0x00028000ul            //�ߵ�ַ����ʼλ��
#define HIGH_CODE_END           0x0003fffful            //�ߵ�ַ������λ��
#define HIGH_BLOCK_START        19
#define HIGH_BLOCK_END          21
#endif  // CODE_MEM_SIZE

#if CODE_MEM_SIZE == 512
//#define LOW_CODE_START          0x00020000ul            //�͵�ַ����ʼλ��
//#define LOW_CODE_END            0x0004fffful            //�͵�ַ������λ��
//#define LOW_BLOCK_START         18
//#define LOW_BLOCK_END           23
//#define HIGH_CODE_START         0x00050000ul            //�ߵ�ַ����ʼλ��
//#define HIGH_CODE_END           0x0007fffful            //�ߵ�ַ������λ��
//#define HIGH_BLOCK_START        24
//#define HIGH_BLOCK_END          29
//azh �������124K�������ĳ�60K
//#define LOW_CODE_START          0x00010000ul            //�͵�ַ����ʼλ��
//#define LOW_CODE_END            0x00047ffful            //�͵�ַ������λ��
//#define LOW_BLOCK_START         16
//#define LOW_BLOCK_END           22
//#define HIGH_CODE_START         0x00048000ul            //�ߵ�ַ����ʼλ��
//#define HIGH_CODE_END           0x0007fffful            //�ߵ�ַ������λ��
//#define HIGH_BLOCK_START        23
//#define HIGH_BLOCK_END          29
//azh 130314 ���ڹ�������ռ䲻�� �ѳ�������ʾ���ȥ��
#define LOW_CODE_START          0x00002000ul            //�͵�ַ����ʼλ��
#define LOW_CODE_END            0x0003fffful            //�͵�ַ������λ��
#define LOW_BLOCK_START         2
#define LOW_BLOCK_END           21
#define HIGH_CODE_START         0x00040000ul            //�ߵ�ַ����ʼλ��
#define HIGH_CODE_END           0x0007fffful            //�ߵ�ַ������λ��
#define HIGH_BLOCK_START        22
#define HIGH_BLOCK_END          29
#endif  // CODE_MEM_SIZE

#ifdef DISPLAY_TERMINAL_2
#define FLAG_FLASH_MAX_READ     4096                    //�����˷�RAM��ԼFLASH, ʡ�øĳ���ķ���
#else   //DISPLAY_TERMINAL_2
#define FLAG_FLASH_MAX_READ     256                     //��ʶFLASH����������, Ȩ��RAM��FALSH����Դ����, ��ʾ�ն�RAM����
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
#define SOFT_RESET_DELAY_TIME   (5 * 1000 / SYS_TICK)   //��λ�ӳٵȴ�ʱ��, 5��, Tick=5mS
#else   //DISPLAY_TERMINAL
#define SOFT_RESET_DELAY_TIME   (5 * 1000 / (OS_TICK / 1000))

#define TEST_FLAG_BIT(x, m)     (x & (m))               //λ����
#define SET_FLAG_BIT(x, m)      (x |= (m))              //λ��1
#define CLR_FLAG_BIT(x, m)      (x &= (~(m)))           //λ����
//���������Ҫ�ڹ���ģʽ�½��� ����һ������ ���ID��һ������
#define GET_RUN_ADDR_ADMIN      0//��ȡ��ǰ���е�ַ
#define SOFT_RST_ADMIN          1//�����λ

#endif  //DISPLAY_TERMINAL

//----------------------------- type definition -------------------------------
typedef enum                                            //IAPָ����
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

typedef enum                                            //IAPָ��صĽ����
{
    CMD_SUCCESS = 0,
    INVALID_COMMAND = 1,                                //ָ������Ч
    SRC_ADDR_ERROR = 2,                                 //Source address not on word boundary
    DST_ADDR_ERROR = 3,                                 //Destination address not on word or 256 byte boundary
    SRC_ADDR_NOT_MAPPED = 4,
    DST_ADDR_NOT_MAPPED = 5,
    COUNT_ERROR = 6,                                    //Byte count is not multiple of 4 or is not a permitted value
    INVALID_SECTOR = 7,                                 //SECTOR��Ч
    SECTOR_NOT_BLANK = 8,                               //SECTOR�ǿ�
    SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION = 9,        //SECTORδָ��
    COMPARE_ERROR = 10,                                 //�Ƚ�ʱ����
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
//__EXTERN VU32 CrdyCount;                                //һ��FLASH������T1����
//#endif  //PRINT_SLAVE_DBG

//__EXTERN U32 signature_secret(U32 index, U8 buf[]);

#endif  //__FIRMWARE_UPGRADE_H

/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : firmware_upgrade.c
 Description    : 用于代码升级, 大部分函数不可重入, 运行时阻断处理器正常程序运行
 Related Files  : firmware_upgrade.h file_head_info.h
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、    1.10        2011-01-18    wwllzz          保护所有4K常数区
   1、    1.00        2010-09-16    wwllzz          the original version
******************************************************************************/
#ifndef DISPLAY_TERMINAL
#include "global_config.h"
#else   //DISPLAY_TERMINAL
#include "..\inc\global_config.h"
#define Sys_Tick OS_Time
#endif  //DISPLAY_TERMINAL

extern const U8 version_info[];

extern U8 Sample_Data[];//定义处用U32 这个文件用的时候都是按U8处理 要注意
//#define Sample_Data ((U8*)SampleData)
//-------------------------------- 常数定义 -----------------------------------
#ifdef IDEA_ACC
static const U8 VERSION_INFO_S[] = {"黄理说这地方用64字节固定串比较简单, 以后和程序版本就没有"};
static const U8 HUA_CAI[] = {"华采"};
static const U8 XIAO_PIP[] = {"这是清底度和校表用的密码核对生成种子要求52个"};
static const U8 DATE_PIP[] = {"生成20字节时间随机码的种子的60字节时间穿插在里面具体"};
#endif  //IDEA_ACC

//#ifdef PRINT_SLAVE_DBG
//#define DBGTHR U1THR                                    //从单元调试串口
//static const U8 Ascii[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
//#endif  //PRINT_SLAVE_DBG

//------------------------------ 私有变量声明 ---------------------------------
static U32 param_table[5];                              //程序FLASH操作函数参数传入用
static U32 result_table[5];                             //程序FLASH操作函数返回值用
static U32 last_addr;                                   //数据(代码)的开始地址, 256对齐
static U32 data_code[256/4];                            //数据(代码)缓冲区
#ifdef FIRST_RESPONSE
static U32 last_pa;                                     //上次编程的256字节块首地址
static U32 data_code_len;                               //缓冲区代码长度
static U32 upgrade_sign;                                //主循环用位标识变量
    //bit0----参照PROGRAM_CODE_AFTER_ANSWER, 标识需要记录发送完帧的索引号
    //bit1-bit2----串口号用2位标识
    //bit3----测试位
#endif  //FIRST_RESPONSE
#ifndef DISPLAY_TERMINAL
static U32 run_normal_ram;                              //程序部分有效运行标识
                                    //bit0-Zigbee能通信
                                    //bit1-与A模块能通信
                                    //bit2-计量工作正常
                                    //bit3~bit31-暂时保留
#else//DISPLAY_TERMINAL
static U32 run_normal_ram;                              //程序部分有效运行标识, 显示终端
                                    //bit0-Zigbee能通信
                                    //bit1-GPRS能通信
                                    //bit2-显示正常
                                    //bit3~bit31-暂时保留
#endif//DISPLAY_TERMINAL
static U32 soft_reset_count;                            //软复位用计数器, 计算Tick值
static U32 tmp_tick;                                    //上次tick值

//------------------------------ 私有函数声明 ---------------------------------
static void write_data_FW(U32 flash_address, U32 *flash_data_buf, U32 count);
static void erase_sector(U32 start_sector, U32 end_sector);
static void prepare_sector(U32 start_sector, U32 end_sector);
#ifndef DISPLAY_TERMINAL
static void iap_entry(U32 param_tab[], U32 result_tab[]);
#endif//DISPLAY_TERMINAL
static U32 address_to_section_number(U32 addr);
static U32 is_run_position(void);
static void program_run_using_flag(void *p);
static U32 check_code_crc(U32 c);
#ifdef IDEA_ACC
static U32 get_aa_addr(U32 zone);
#endif  //IDEA_ACC

/**
 * @brief  Set the Fault Mask value
 *
 * @param  faultMask  faultMask value
 *
 * Set the fault mask register
 */
#ifndef DISPLAY_TERMINAL
__asm void __set_FAULTMASK(U32 faultMask)
{
  msr faultmask, r0
  bx lr
}
#endif  //DISPLAY_TERMINAL

/******************************************************************************
函数: void write_data_FW(U32 flash_address, U32 *flash_data_buf, U32 count)
入口: flash_address FLASH开始地址, 必须256字节对齐
      flash_data_buf代码保存地址, 必须4字节(字)对齐
      count是256,512,1024,4096之一
出口: 无 全局结果码result_table[0..5]
概述: 烧写FLASH. 使用全局param_table等
******************************************************************************/
void write_data_FW (U32 flash_address, U32 *flash_data_buf, U32 count)
{
    if(flash_address < 4096)                            //禁止对boot块操作
    {
        return;
    }
    if((flash_address >= CODE_START_ADDRSS) && (flash_address <= CODE_END_ADDRESS))
    {
        return;                                         //要写代码在自身运行区, 啥也不做的返回
    }
    param_table[0] = COPY_RAM_TO_FLASH;                 //代码烧写
    param_table[1] = flash_address;
    param_table[2] = (U32)flash_data_buf;
    param_table[3] = count;
    param_table[4] = FCCLK / 1024;
    iap_entry(param_table, result_table);
}

/******************************************************************************
函数: void erase_sector(U32 start_sector, U32 end_sector)
入口: start_sector开始扇区号, end_sector结束扇区号
出口: 无 全局结果码result_table[0..5]
概述: 擦除操作扇区. 使用全局param_table等
******************************************************************************/
void erase_sector (U32 start_sector, U32 end_sector)
{
    if(start_sector == 0)                               //禁止对boot块操作
    {
        return;
    }
    if((start_sector >= BLOCK_START) && (start_sector <= BLOCK_END))
    {
        return;                                         //要擦除的块在当前运行区
    }
    param_table[0] = ERASE_SECTOR;
    param_table[1] = start_sector;
    param_table[2] = end_sector;
    param_table[3] = FCCLK / 1024;
    iap_entry(param_table, result_table);
}

/******************************************************************************
函数: void prepare_sector(U32 start_sector, U32 end_sector)
入口: start_sector开始扇区号, end_sector结束扇区号
出口: 无 使用全局param_table等
概述: 指定操作扇区. 使用全局param_table等
******************************************************************************/
void prepare_sector (U32 start_sector, U32 end_sector)
{
    if(start_sector == 0)                               //禁止对boot块操作
    {
        return;
    }
    if((start_sector >= BLOCK_START) && (start_sector <= BLOCK_END))
    {
        return;                                         //要指定块在当前运行区
    }
    param_table[0] = PREPARE_SECTOR_FOR_WRITE;
    param_table[1] = start_sector;
    param_table[2] = end_sector;
    param_table[3] = FCCLK / 1024;
    iap_entry(param_table, result_table);
}

/******************************************************************************
函数: void iap_entry(U32 param_tab[], U32 result_tab[])
概述: 调用芯片出厂Boot子程序的规范入口写法
入口: param_tab入口参数表, result_tab子程序结果参数
出口: 无, result_tab中包含结果参数
******************************************************************************/
void iap_entry (U32 param_tab[], U32 result_tab[])
{
#ifndef DISPLAY_TERMINAL
    void (*iap)(U32 [], U32 []);
    U32 tmp;
    iap = (void (*)(U32 [], U32 []))IAP_LOCATION;

    __disable_irq();                                    //关中断
    if((COPY_RAM_TO_FLASH == param_tab[0]) || (ERASE_SECTOR == param_tab[0]))
    {
        tmp = T1TC;
        iap(param_tab, result_tab);
        tmp = (T1TC - tmp);                             //实际观察这时间约1mS, 擦除和编程差不多
        tmp /= SYS_TICK_ON_T1;
        if(0 != tmp)
        {
            Sys_Tick += tmp;
        }
        AD_Ctr[AD1].CrdyFail = AD_Ctr[AD2].CrdyFail = 0;//清错误计数
    }
    else
    {
        iap(param_tab, result_tab);
    }
    __enable_irq();                                     //开中断
#else   //DISPLAY_TERMINAL
    iap_programe(param_tab, result_tab);
#endif  //DISPLAY_TERMINAL
}

/******************************************************************************
函数: U32 address_to_section_number(U32 addr)
入口: addr代码的运行地址
出口: 低8位为扇区号, bit8地址超范围, bit9地址不是256对齐, bit11能正确分析(bit8/9为0)
    bit10扇区开始
概述: 算出地址对应的扇区号, 从0开始. 只针对要下载写入的代码进行判别
******************************************************************************/
U32 address_to_section_number (U32 addr)
{
    U32 tmp;

    tmp = is_run_position();                            //判别当前代码运行区
    if(tmp == 1)                                        //当前运行在低区
    {
        if((addr < HIGH_CODE_START) || (addr > HIGH_CODE_END))  //不是在高代码区
        {
            return 1 << 8;
        }
    }
    else if(tmp == 2)                                   //当前代码运行在高区
    {
        if((addr < LOW_CODE_START) || (addr > LOW_CODE_END))
        {
            return 1 << 8;
        }
    }
    else                                                //当前代码运行区不明
    {
        return 1 << 8;
    }
    if((addr & 0xff) != 0)                              //开始地址不是256字节对齐
    {
        return 1 << 9;
    }
    if(addr < 0x10000)                                  //前64K是每块4K的
    {
        tmp = addr >> 12;                               //扇区号
        if((addr & 0xff) != 0)                          //没有256字节对齐
        {
            tmp |= (1 << 9);
        }
        if((addr & 0xfff) == 0)                         //一块的开始
        {
            tmp |= (1 << 10);
        }
    }
    else
    {
        tmp = 0x10 + ((addr - 0x10000) >> 15);          //扇区号
        if((addr & 0xff) != 0)                          //没有256字节对齐
        {
            tmp |= (1 << 9);
        }
        if((addr & 0x7fff) == 0)                        //一块的开始
        {
            tmp |= (1 << 10);
        }
    }
    return tmp;
}

#ifndef FIRST_RESPONSE
/******************************************************************************
函数: U8 write_package_code(U8 buf[])
入口: buf-指向接到的代码报文帧的长度字节位置, 从buf[15]开始为地址
出口: 0-成功, 其它为错误(疑问)编码, bit3-CRC校验错误, bit1-该地址禁止写, bit5-首包地址不对
    bit6-长度太小
概述: 报文传输来的缓冲区, 长度必须大于20, 是要下载报文. 内部对报文的数据进行整合
    拼接, 凑足256字节进行一次写入或者同时进行擦除.
    68+ 通信地址6+ 68+ C+ 长度1+ 部颁标识码4+密码4+操作者代码4+扩展标识码2+ 地址4+代码192+CRC2+ CS+16
    CRC是高字节在前 使用Sample_Data
******************************************************************************/
//U8 write_package_code (U8 buf[])
//{
//    U32 tmp, i, len;
//    U32 addr;
//    U8 *p;
//
//    if(buf[0] < 21)                                     //不可能包含代码, 已保证 buf[0] >= 20
//    {
//        return (1 << 6);
//    }
//    tmp = calc_crc16(buf + 15, buf[0] - 16, 0xffff);    //CRC计算, 采用0xffff为初值
//    if(tmp != (buf[buf[0]] + buf[buf[0] - 1] * 256))    //CRC错误
//    {
//        return (1 << 3);
//    }
//    addr = (buf[15] << 24) + (buf[16] << 16) + (buf[17] << 8) + buf[18];    //低字节在前
//    if(address_to_section_number(addr) & (1 << 8))      //地址超出目前可升级范围
//    {
//        return (1 << 1);
//    }
//    if(last_addr == 0)                                  //要升级的首包
//    {
//        p = Sample_Data;
//        memcpy(p, (void *)RUN_USING_FLAG, 4096);        //保护全部常数数据
//        if(is_run_position() == 1)                      //地址必须是确定值
//        {
//            if(addr != HIGH_CODE_START)                 //第一包的地址不对
//            {
//                return (1 << 5);
//            }
//            last_addr = HIGH_CODE_START;                //填上真正的开始地址
//            p[4] |= 0x1a;                               //标识高区是新代码, 高区CRC没验证, 没运行过
//        }
//        else
//        {
//            if(addr != LOW_CODE_START)                  //第一包的地址不对
//            {
//                return (1 << 5);
//            }
//            last_addr = LOW_CODE_START;
//            p[4] &= ~0x10;                              //最新代码为低区
//            p[4] |= 0x05;                               //低区没运行过, CRC也未验证
//        }
//        program_run_using_flag(p);                      //改写标识区
//        for(i = 0; i < 256 / 4; i++)                    //填充缓冲区
//        {
//            data_code[i] = 0xffffffff;
//        }
//    }
//    else
//    {
//        if((last_addr & 0xffffff00) != (addr & 0xffffff00))             //是要烧写新的256字节页面
//        {
//            len = buf[0] - 20;                          //先检查是否为重发帧, 重发则退出
//            p = (U8 *)data_code;
//            for(i = 0; i < len; i++)
//            {
//                if(((addr & 0xff) + i) < 256)           //代码已经写到FLASH
//                {
//                    if(buf[19 + i] != *((U8 *)(addr + i)))  //发现有不同的
//                    {
//                        break;
//                    }
//                }
//                else                                    //代码还在缓冲区
//                {
//                    if(buf[19 + i] != p[(addr + i) & 0xff])
//                    {
//                        break;
//                    }
//                }
//            }
//            if(i == len)                                //都相同
//            {
//                if(last_addr == (addr + len))           //这是重发的帧
//                {
//                    return 0;
//                }
//            }
//            if(last_addr & 0xff)                        //缓冲内不充满的页面需要烧写
//            {
//                tmp = address_to_section_number(last_addr & 0xffffff00);
//                if(tmp & (1 << 10))                     //扇区开始
//                {
//                    prepare_sector(tmp & 0xff, tmp & 0xff);             //指定页面
//                    erase_sector(tmp & 0xff, tmp & 0xff);               //擦除页面
//                }
//                prepare_sector(tmp & 0xff, tmp & 0xff); //指定页面
//                write_data_FW(last_addr & 0xffffff00, data_code, 256);
//            }
//            for(i = 0; i < 256 / 4; i++)                //填充缓冲区
//            {
//                data_code[i] = 0xffffffff;
//            }
//            if((last_addr & 0xffff8000) != (addr & 0xffff8000)) //是最后的信息区
//            {
//                for(i = 0; i < 64 / 4; i++)             //这新开始的程序对应块是否已被擦除了
//                {
//                    if(*((U32 *)(addr + i)) != 0xffffffff)
//                    {
//                        break;
//                    }
//                }
//                if(i != 64 / 4)                         //块需要擦除, 目前只考虑了文件信息块
//                {
//                    tmp = address_to_section_number(addr & 0xffff8000);
//                    prepare_sector(tmp & 0xff, tmp & 0xff);
//                    erase_sector(tmp & 0xff, tmp & 0xff);
//                }
//            }
//            last_addr = addr & 0xffffff00;              //同步地址
//        }
//    }
//    p = (U8 *)data_code;
//    len = buf[0] - 20;                                  //保证buf[0]多于20
//    for(i = 0; i < len; i++)
//    {
//        if((((addr + i) & 0xff)) == (256 - 1))
//        {
//            p[256 - 1] = buf[19 + i];                   //数据放到缓冲区
//            tmp = address_to_section_number(last_addr & 0xffffff00);    //缓冲区满了, 将这页面写入
//            if(tmp & (1 << 10))                         //扇区开始
//            {
//                prepare_sector(tmp & 0xff, tmp & 0xff); //指定页面
//                erase_sector(tmp & 0xff, tmp & 0xff);   //擦除页面
//            }
//            prepare_sector(tmp & 0xff, tmp & 0xff);     //指定页面
//            write_data_FW(last_addr & 0xffffff00, data_code, 256);
//
//            for(tmp = 0; tmp < 256 / 4; tmp++)          //缓冲区填充
//            {
//                data_code[tmp] = 0xffffffff;
//            }
//            addr += 256;
//            last_addr += 256;
//        }
//        else                                            //只需要数据赋值
//        {
//            p[(addr + i) & 0xff] = buf[19 + i];
//        }
//    }
//    last_addr = (last_addr & 0xffffff00) | ((addr + i) & 0xff); //加上缓冲内的偏移量
//    return 0;
//}
#endif  //FIRST_RESPONSE

/******************************************************************************
函数: U8 activate_FW(U8 index)
入口: index-0 the binary last loaded, 最新下载的
            1 the last but one binary loaded, 旧的
            2 切换到低区
            3 切换到高区
            other : rejected
出口: 1-成功, 0-失败, 2-无需复位, 3-程序逻辑漏洞, 为兼容0,2,3返回(1 << 4)
概述: 激活程序命令指令 收到该条指令后, 函数判断代码的完整和正确性, 依index做标识切换
    若要切换的代码校验不通过, 则返回失败, 不进行切换
说明: 真正切换在复位后进行, 但标志改写在本子程序实现
      返回1仅表明标志改写完成, 使用Sample_Data
      系统做相关数据保存确认主站收到回应及断开连接等后, 再执行复位
******************************************************************************/
U8 activate_FW (U8 index)
{
    U32 tmp;
    U8 *p;

    last_addr = 0;                                      //已能确认程序升级结束
    p = (U8*)Sample_Data;
    memcpy(p, (void *)RUN_USING_FLAG, FLAG_FLASH_MAX_READ); //保护4K字节
    tmp = is_run_position();                            //对应区的校验是否正确
    if(tmp == 1)                                        //当前运行在低区
    {
        if(p[0] == 1)                                   //要求运行低区
        {
            if((index == 2) ||                          //当前运行在低区, 标识要求运行低区, 要求切换到低区
                ((index == 0) && ((p[4] & (1 << 4)) == 0)) ||   //低区是新下载, 要求新的
                ((index == 1) && ((p[4] & (1 << 4)) != 0)))     //高区是新下载, 要求旧的
            {
                return (1 << 4);                        //return 0;//无需动作, 回应失败
            }
            if((index == 3) ||                          //当前运行在低区, 标识要求运行低区, 要求切换到高区
                ((index == 0) && ((p[4] & (1 << 4)) != 0)) ||   //高区是新下载的, 要求新的
                ((index == 1) && ((p[4] & (1 << 4)) == 0)))     //低区是新下载的, 要求旧的
            {
                if(((p[4] & 0xa) != 0xa) || check_code_crc(1))  //检查高区CRC, 运行过或者标识CRC正确的也算数
                {
                    if((p[4] & (1 << 3)) != 0)          //高区CRC没标识正确
                    {
                        p[4] &= ~(1 << 3);
                    }
                    p[0] = 2;                           //切到高区
                    program_run_using_flag(p);
                    soft_reset_count = SOFT_RESET_DELAY_TIME;
                    tmp_tick = Sys_Tick;                //开始复位时刻
                    return 1;
                }
                return (1 << 3);                        //return 0;
            }
        }
        else                                            //要求运行高区
        {
            if((index == 2) ||                          //当前运行在低区, 标识要求运行高区, 要求切换到低区, 只需要更改标识
                ((index == 0) && ((p[4] & (1 << 4)) == 0)) ||   //低区是新下载, 要求新的
                ((index == 1) && ((p[4] & (1 << 4)) != 0)))     //高区是新下载, 要求旧的
            {
                p[0] = 1;                               //修改运行标识
                program_run_using_flag(p);
                return (1 << 4);                        //return 2;//不用切换
            }
            if((index == 3) ||                          //当前运行在低区, 标识要求运行高区, 要求切换到高区
                ((index == 0) && ((p[4] & (1 << 4)) != 0)) ||   //高区是新下载的, 要求新的
                ((index == 1) && ((p[4] & (1 << 4)) == 0)))     //低区是新下载的, 要求旧的
            {
                if(((p[4] & 0xa) != 0xa) || check_code_crc(1))  //检查高区CRC, 运行过或者标识CRC正确的也算数
                {
                    if((p[4] & (1 << 3)) != 0)          //高区CRC标识没处理
                    {
                        p[4] &= ~(1 << 3);              //标识高区CRC正常
                        program_run_using_flag(p);
                    }
                    soft_reset_count = SOFT_RESET_DELAY_TIME;
                    tmp_tick = Sys_Tick;                //开始复位时刻
                    return 1;                           //直接软复位就行了
                }
                return (1 << 3);                        //return 0;//高区CRC异常
            }
        }
    }
    else if(tmp == 2)                                   //当前运行在高区
    {
        if(p[0] == 2)                                   //要求运行高区
        {
            if((index == 3) ||                          //当前运行高区, 标识要求运行高区, 要求切换到高区
                ((index == 0) && ((p[4] & (1 << 4)) != 0)) ||   //高区是新下载的, 要求新的
                ((index == 1) && ((p[4] & (1 << 4)) == 0)))     //低区是新下载的, 要求旧的
            {
                return (1 << 4);                        //return 0;//无需动作, 回应失败
            }
            if((index == 2) ||                          //当前运行高区, 标识要求运行高区, 要求切换到低区
                ((index == 0) && ((p[4] & (1 << 4)) == 0)) ||   //低区是新下载, 要求新的
                ((index == 1) && ((p[4] & (1 << 4)) != 0)))     //高区是新下载, 要求旧的
            {
                if(((p[4] & 0x5) != 0x5) || check_code_crc(0))  //检查低区CRC, 运行过或者标识CRC正确的也算数
                {
                    if((p[4] & (1 << 2)) != 0)          //低区CRC没标识正确
                    {
                        p[4] &= ~(1 << 2);
                    }
                    p[0] = 1;                           //切到低区
                    program_run_using_flag(p);
                    soft_reset_count = SOFT_RESET_DELAY_TIME;
                    tmp_tick = Sys_Tick;                //开始复位时刻
                    return 1;
                }
                return (1 << 3);                        //return 0;
            }
        }
        else                                            //要求运行低区
        {
            if((index == 3) ||                          //当前运行在高区, 标识要求运行低区, 要求切换到高区
                ((index == 0) && ((p[4] & (1 << 4)) != 0)) ||   //高区是新下载的, 要求新的
                ((index == 1) && ((p[4] & (1 << 4)) == 0)))     //低区是新下载的, 要求旧的
            {
                p[0] = 1;                               //修改运行标识
                program_run_using_flag(p);
                return (1 << 4);                        //return 2;//不用切换
            }
            if((index == 2) ||                          //当前运行高区, 标识要求运行低区, 要求切换到低区
                ((index == 0) && ((p[4] & (1 << 4)) == 0)) ||   //低区是新下载, 要求新的
                ((index == 1) && ((p[4] & (1 << 4)) != 0)))     //高区是新下载, 要求旧的
            {
                if(((p[4] & 0x5) != 0x5) || check_code_crc(0))  //检查低区CRC, 运行过或者标识CRC正确的也算数
                {
                    if((p[4] & (1 << 2)) != 0)          //低区没有CRC正常标识
                    {
                        p[4] &= ~(1 << 2);
                        program_run_using_flag(p);
                    }
                    soft_reset_count = SOFT_RESET_DELAY_TIME;
                    tmp_tick = Sys_Tick;                //开始复位时刻
                    return 1;
                }
                return (1 << 3);                        //return 0;
            }
        }
    }
    else                                                //当前运行状态不对
    {
        return (1 << 4);                                //return 0;
    }
    return (1 << 4);                                    //return 3;
}

/******************************************************************************
函数: S32 get_monitoring_status(U8 addr_buf[])
入口: addr_buf[]-信息帧数据首地址
出口: 返回数据的长度
概述: 读取当前写入代码的地址, 不掉电情况下有效, 也为断点续传吧, 版本信息, 已写入代码
******************************************************************************/
S32 get_monitoring_status (U8 addr_buf[])
{
#ifdef FIRST_RESPONSE
    U32 addr;
    U8 m;
#endif  //FIRST_RESPONSE

    memcpy(addr_buf, (void *)0xfc0, 16);                //Boot信息16字节
#ifndef DISPLAY_TERMINAL
    memcpy(addr_buf + 16, (void *)0x1ffc0, 64);         //第一块应用程序版本标识
#else//DISPLAY_TERMINAL
    #if CODE_MEM_SIZE == 512
        memcpy(addr_buf + 16, (void *)0x3ffc0, 64);         //第一块应用程序版本标识
    #else//256
        memcpy(addr_buf + 16, (void *)0x1ffc0, 64);         //第一块应用程序版本标识
    #endif
#endif//DISPLAY_TERMINAL
#if CODE_MEM_SIZE == 512
        memcpy(addr_buf + 16 + 64, (void *)0x7ffc0, 64);    //第二块应用程序版本标识
#else//256
    memcpy(addr_buf + 16 + 64, (void *)0x3ffc0, 64);    //第二块应用程序版本标识
#endif
    addr_buf[16 + 64 * 2] = ((*((U8 *)0x1004)) & 0x1f) | (is_run_position() << 5);
                                                        //bit0为0表示低区曾有效运行,
                                                        //bit1为0表示高区曾有效运行.
                                                        //bit2为0表示低区已下载代码CRC验证正确,
                                                        //bit3为0表示高区已下载代码CRC验证正确.
                                                        //bit4为0表示最新下载的为低区, 1-最新的是高区
                                                        //bit5-6表示当前运行状态 1-低区, 2-高区, 0-异常
#ifndef FIRST_RESPONSE
    addr_buf[16 + 1 + 64 * 2] = last_addr >> 24;        //上次已下载的代码地址
    addr_buf[16 + 2 + 64 * 2] = last_addr >> 16;
    addr_buf[16 + 3 + 64 * 2] = last_addr >> 8;
    addr_buf[16 + 4 + 64 * 2] = last_addr;
#else   //FIRST_RESPONSE
    addr = last_addr + data_code_len;                   //上次已下载的代码地址
    addr_buf[16 + 1 + 64 * 2] = addr >> 24;
    addr_buf[16 + 2 + 64 * 2] = addr >> 16;
    addr_buf[16 + 3 + 64 * 2] = addr >> 8;
    addr_buf[16 + 4 + 64 * 2] = addr;
    for(addr = LOW_CODE_END - 64; addr != LOW_CODE_START; addr--)   //低区当前非ff长度
    {
        m = *((U8 *)addr);
        if(m != 0xff)
        {
            break;
        }
    }
    addr_buf[16 + 5 + 64 * 2] = addr >> 24;             //返回非FF的地址
    addr_buf[16 + 6 + 64 * 2] = addr >> 16;
    addr_buf[16 + 7 + 64 * 2] = addr >> 8;
    addr_buf[16 + 8 + 64 * 2] = addr;

    for(addr = HIGH_CODE_END - 64; addr != HIGH_CODE_START; addr--) //高区当前非ff长度
    {
        m = *((U8 *)addr);
        if(m != 0xff)
        {
            break;
        }
    }
    addr_buf[16 + 9 + 64 * 2] = addr >> 24;             //返回非FF的地址
    addr_buf[16 + 10 + 64 * 2] = addr >> 16;
    addr_buf[16 + 11 + 64 * 2] = addr >> 8;
    addr_buf[16 + 12 + 64 * 2] = addr;
#endif  //FIRST_RESPONSE
    return 149 + 8;
}

/******************************************************************************
函数: U32 is_run_position(void)
入口: 无
出口: 1-低区, 2-高区, 0-异常, 其它无效
概述: 依据VTOR的值和运行首地址的范围 当前PC判别当前代码运行区
******************************************************************************/
U32 is_run_position (void)
{
    U32 tmp;

    if((version_info[14] != 'H') && (version_info[14] != 'L'))
    {
        return 0;
    }
#ifdef DISPLAY_TERMINAL
    tmp = operation_at_admin(GET_RUN_ADDR_ADMIN);//要用软中断切换到特权模式 才能读出
#else//DISPLAY_TERMINAL
    tmp = VTOR;                                         //向量表的位置
#endif//DISPLAY_TERMINAL
    if((HIGH_CODE_START <= tmp) && (tmp <= HIGH_CODE_END))      //向量表在高区
    {
        tmp = *((U32 *)(tmp + 4));                      //程序入口地址
        if((HIGH_CODE_START <= tmp) && (tmp <= HIGH_CODE_END))
        {
            tmp = __current_pc();                       //当前PC值
            if((HIGH_CODE_START <= tmp) && (tmp <= HIGH_CODE_END))
            {
                return 2;                               //确信运行在高区
            }
        }
        return 0;
    }
    else if((LOW_CODE_START <= tmp) && (tmp <= LOW_CODE_END))   //向量表在低区
    {
        tmp = *((U32 *)(tmp + 4));                      //程序入口地址
        if((LOW_CODE_START <= tmp) && (tmp <= LOW_CODE_END))
        {
            tmp = __current_pc();                       //当前PC值
            if((LOW_CODE_START <= tmp) && (tmp <= LOW_CODE_END))
            {
                return 1;
            }
        }
        return 0;
    }
    return 0;
}

/******************************************************************************
函数: void program_run_using_flag(void *p)
入口: p-数据缓冲区首地址
出口: 无
概述: 将缓冲区数据编程到RUN_USING_FLAG开始的程序地址上, 烧4K字节
******************************************************************************/
void program_run_using_flag (void *p)
{
    prepare_sector(1, 1);                           //写回标识
    erase_sector(1, 1);
    prepare_sector(1, 1);
    write_data_FW(RUN_USING_FLAG, (U32 *)p, FLAG_FLASH_MAX_READ);   //写入所有字节
}

/******************************************************************************
函数: U32 check_code_crc(U32 c)
入口: c-0表示低区, 1表示高区
出口: 1-CRC校验正确, 0-错误
概述: 对代码区进行CRC完整性检查, 不包括应用程序数据区, 包括版本信息区
******************************************************************************/
U32 check_code_crc (U32 c)
{
    U32 l, index;
    U32 tmp;
    U8 m;

    if(c)                                               //高区
    {
        for(index = HIGH_CODE_END - 64; index != HIGH_CODE_START; index--)
        {
            m = *((U8 *)index);
            if(m != 0xff)
            {
                if(m != 0xaa)                           //从尾巴开始找到的第一个不是aa
                {
                    return 0;
                }
                break;
            }
        }
        if(index == HIGH_CODE_START)
        {
            return 0;
        }
        l = index - HIGH_CODE_START + 1 - 3;
        tmp = calc_crc16((U8 *)HIGH_CODE_START, l, 0xffff);
//        if(l < 0x10000)
//        {
//            tmp = calc_crc16((U8 *)HIGH_CODE_START, l, 0xffff);
//        }
//        else                                            //代码超过64K, 目前支持接近128K
//        {
//            tmp = calc_crc16((U8 *)HIGH_CODE_START, 0xffff, 0xffff);
//            tmp = calc_crc16((U8 *)(HIGH_CODE_START+0xffff), l - 0xffff, tmp);
//        }
        tmp = calc_crc16((U8 *)(HIGH_CODE_END - 64 + 1), 64, tmp);
        if(tmp == ((*((U8 *)(index - 2))) * 256 + *((U8 *)(index - 1))))
        {
            return 1;
        }
    }
    else
    {
        for(index = LOW_CODE_END - 64; index != LOW_CODE_START; index--)
        {
            m = *((U8 *)index);
            if(m != 0xff)
            {
                if(m != 0xaa)                           //从尾巴开始找到的第一个不是aa
                {
                    return 0;
                }
                break;
            }
        }
        if(index == LOW_CODE_START)
        {
            return 0;
        }
        l = index - LOW_CODE_START + 1 - 3;
        tmp = calc_crc16((U8 *)LOW_CODE_START, l, 0xffff);
//        if(l < 0x10000)
//        {
//            tmp = calc_crc16((U8 *)LOW_CODE_START, l, 0xffff);
//        }
//        else                                            //代码长度超过64K, 目前支持接近128K
//        {
//            tmp = calc_crc16((U8 *)LOW_CODE_START, 0xffff, 0xffff);
//            tmp = calc_crc16((U8 *)(LOW_CODE_START+0xffff), l - 0xffff, tmp);
//        }
        tmp = calc_crc16((U8 *)(LOW_CODE_END - 64 + 1), 64, tmp);
        if(tmp == ((*((U8 *)(index - 2))) * 256 + *((U8 *)(index - 1))))
        {
            return 1;
        }
    }
    return 0;
}

/******************************************************************************
函数: void set_normal_run_flag(U32 c)
入口: c- bit0-Zigbee能通信, bit1-与A模块能通信, bit2-计量工作正常
    不推荐一次传入都已经正常运行
出口: 无
概述: 检查当前运行区程序有效运行标识, 没设置的标识则判定并设置, 使用Sample_Data
******************************************************************************/
void set_normal_run_flag (U32 c)
{
    U8 *p;

    run_normal_ram |= c;
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
	if((run_normal_ram & 0x3) != 0x3)
#else
    if((run_normal_ram & 0x7) != 0x7)
#endif
    {
        return;
    }
    p = (U8*)Sample_Data;
    p[4] = *((U8 *)CRC_AND_RUN_INVALID_FLAG);
    if(is_run_position() == 1)                          //现运行在低区
    {
        if((p[4] & (1 << 0)) == 0)                      //已经标识过了
        {
            return;
        }
        memcpy(p, (void *)RUN_USING_FLAG, FLAG_FLASH_MAX_READ); //保护所有数据
        p[4] &= ~(1 << 0);
        program_run_using_flag(p);
        return;
    }
    if((p[4] & (1 << 1)) == 0)                          //高区已标识有效运行
    {
        return;
    }
    memcpy(p, (void *)RUN_USING_FLAG, FLAG_FLASH_MAX_READ);     //保护所有数据
    p[4] &= ~(1 << 1);                                  //写入有效运行标识
    program_run_using_flag(p);
    return;
}

/******************************************************************************
函数: void soft_reset_function(void)
入口: 无
出口: 无
概述: 按soft_reset_count要求倒计时准备软复位
******************************************************************************/
void soft_reset_function (void)
{
    U32 tmp;

    if(soft_reset_count == 0)                           //没有复位要求
    {
        return;
    }
    tmp = Sys_Tick - tmp_tick;                          //过去的时间
    tmp_tick = Sys_Tick;
    if(tmp < soft_reset_count)                          //没到复位时刻
    {
        soft_reset_count -= tmp;
        return;
    }
    soft_reset_count = 0;
#ifndef DISPLAY_TERMINAL
    __disable_irq();
    STCTRL = 0x04;                                      //关闭STI
    CLRENA0 = 0xffff8000;                               //STI以上都关闭
    ((U32 *)0xE000E180)[1] = 0xffffffff;
    ((U32 *)0xE000E180)[2] = 0xffffffff;
    ((U32 *)0xE000E180)[3] = 0xffffffff;
    __enable_irq();                                     //开中断, 已没有向量中断了
    __set_FAULTMASK(1);
    tmp = RSID;                                         //在此之前主程序应已记录并分析复位原因
    RSID = tmp & 0x0f;                                  //清复位原因的CPU寄存器
    chip_reset(SOFT_RESET);
#else//DISPLAY_TERMINAL
    operation_at_admin(SOFT_RST_ADMIN); //到软件中断去处理 软件复位操作
#endif//DISPLAY_TERMINAL
}

#ifdef FIRST_RESPONSE
/******************************************************************************
函数: U32 check_package_code(U8 buf[], U8 com, U32 len)
入口: buf-指向开始为地址 com-串口号 len-报文长度
出口: 0-正确接受, 其它为错误(疑问)编码, bit3-CRC校验错误, bit1-该地址禁止写, bit5-首包地址不对
    bit6-长度太小, bit7-代表忙,正在写
概述: 验收代码, 将检验合格的代码保存在Sample_Data + 4096 的缓冲区, 留待以后写入
    68+ 通信地址6+ 68+ C+ 长度1+ 部颁标识码4+密码4+操作者代码4+扩展标识码2+ 地址4+代码192+CRC2+ CS+16
    CRC是高字节在前 使用Sample_Data, 需要透传给从相的扩展码后面加2字节长度, 2字节ID
    直接传入len, 不再包含规约信息, 首地址改为直接是缓冲区首地址 14/03-2011
    若是签名包就直接编程完毕再回应
******************************************************************************/
U32 check_package_code (U8 buf[], U8 com, U32 len)
{
    U32 tmp, i;
    U32 addr;
    U8 *p;

    if(len < 7)                                         //不可能包含代码, 已保证 len > 6
    {
        return (1 << 6);
    }
    tmp = calc_crc16(buf, len - 2, 0xffff);             //CRC计算, 采用0xffff为初值
    if(tmp != (buf[len - 1] + buf[len - 2] * 256))      //CRC错误, CRC高字节在前
    {
        return (1 << 3);
    }

    addr = (buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + buf[3];    //高字节在前
    if(address_to_section_number(addr) & (1 << 8))      //地址超出目前可升级范围
    {
#ifdef IDEA_ACC
        if((last_addr == 0) && (29 == len))             //此时可能是签名帧
        {
            if(addr < CODE_START_ADDRSS)                //不能跨区签名
            {
                return (1 << 1);
            }
            if(addr > CODE_END_ADDRESS)
            {
                return (1 << 1);
            }
            if((addr & 0xff) != 0)                      //这地址肯定不对
            {
                return (1 << 1);
            }
            for(i = 0; i < 64; i++)                     //这地址附近都擦除了
            {
                if(*((U32 *)(addr + i * 4)) != -1u)
                {
                    break;
                }
                *((U32 *)(data_code + i)) = -1u;        //填data_code[]为FF
            }
            if(i != 64)
            {
                return (1 << 1);
            }
            memcpy(data_code, buf + 4, 23);             //拷贝23字节过去
            tmp = 0x10 + ((addr - 0x10000) >> 15);      //所属块, 目前情况下合理, 代码过小就不行

            param_table[0] = PREPARE_SECTOR_FOR_WRITE;  //这里只能单独重新编写函数
            param_table[1] = param_table[2] = tmp;
            param_table[3] = FCCLK / 1024;
            iap_entry(param_table, result_table);

            param_table[0] = COPY_RAM_TO_FLASH;         //编程FLASH
            param_table[1] = addr;
            param_table[2] = (U32)data_code;
            param_table[3] = 256;
            param_table[4] = FCCLK / 1024;
            iap_entry(param_table, result_table);
            code_signature();                           //重新扫描签名
            buf[4] = buf[5] = 0;                        //不需要等待
            return 0;
        }
#endif  //IDEA_ACC
        return (1 << 1);
    }

    p = (U8 *)data_code;
    if(last_addr == 0)                                  //要升级的首包
    {
        if(is_run_position() == 1)                      //运行在低区
        {
            if(addr != HIGH_CODE_START)                 //要下代码首地址不是高区首地址
            {
                return (1 << 5);
            }
        }
        else
        {
            if(addr != LOW_CODE_START)                  //要下代码首地址不是低区首地址
            {
                return (1 << 5);
            }
        }
        data_code_len = 0;                              //缓冲区代码长度置0
        last_pa = 0;
    }
    else if((last_addr & 0xffffff00) != (addr & 0xffffff00))
    {
        if((last_addr + data_code_len) != addr)         //不是过载的连续帧
        {
            for(i = 0; i < (len - 6); i++)              //是否为重发帧
            {
                if(((addr & 0xff) + i) < 256)           //已经写入到FLASH的
                {
                    if(buf[4 + i] != *((U8 *)(addr + i)))   //发现有不一样的
                    {
                        break;
                    }
                }
                else                                    //代码还在缓冲区里
                {
                    if(buf[4 + i] != Sample_Data[FLAG_FLASH_MAX_READ + ((addr + i) & 0xff)])
                    {
                        break;
                    }
                }
            }
            if(i == (len - 6))                          //代码都相同
            {
                if(addr != UPGRADE_INFO_ADDR)           //只考虑最后的信息帧是分块的, 其它都是重发帧
                {
                    return 0;
                }
            }
            for(i = 0; i < 256 / 4; i++)                //有一个未满256字节的页面需要烧写, 先填充老缓冲
            {
                data_code[i] = 0xffffffff;
            }
            last_pa = last_addr & 0xffffff00;           //未满代码的首地址, 将代码搬到256缓冲区
            if((data_code_len + (last_addr & 0xff)) <= 256)
            {
                for(i = 0; i < data_code_len; i++)
                {
                    p[(last_addr & 0xff) + i] = Sample_Data[FLAG_FLASH_MAX_READ + i];
                }
            }
            data_code_len = 0;                          //最新(大)缓冲区的代码都被移出
        }
        else                                            //是连续帧, 可能前帧还没来得及编程
        {
            if(TEST_FLAG_BIT(upgrade_sign, PROGRAM_CODE_AFTER_ANSWER))
            {
                return (1 << 7);                        //表明忙, 暂时放弃这帧
            }
        }
    }
    if(((last_addr + data_code_len) & 0xff) == (addr & 0xff))
    {
        for(i = 0; i < (len - 6); i++)
        {
            Sample_Data[FLAG_FLASH_MAX_READ + data_code_len + i] = buf[4 + i];
        }
        data_code_len += (len - 6);                     //缓冲区代码长度
    }
    else if(UPGRADE_INFO_ADDR == addr)                  //地址不连续, 且是另外区的信息区
    {
        if(((last_pa + 256) & (~0xff)) != (addr & (~0xff))) //最后256字节块内不包含有效代码
        {
            for(i = 0; i < 256; i++)                        //填充ff, 只考虑最后的64字节信息区
            {
                Sample_Data[FLAG_FLASH_MAX_READ + i] = 0xff;
            }
        }
        else                                            //最后代码块到末尾都填充FF
        {
            for(i = data_code_len; i < 256; i++)
            {
                Sample_Data[FLAG_FLASH_MAX_READ + i] = 0xff;
            }
        }
        for(i = 0; i < (len - 6); i++)                  //将64字节的版本信息拷贝过来
        {
             Sample_Data[FLAG_FLASH_MAX_READ + (addr & 0xff) + i] = buf[4 + i];
        }
        data_code_len = 256;
    }
    last_addr = addr & 0xffffff00;
    buf[4] = buf[5] = 0;                                //一般无需等待
    if(data_code_len >= 256)                            //超过256字节的代码就写入一次
    {
        if(TEST_FLAG_BIT(upgrade_sign, PROGRAM_CODE_AFTER_ANSWER) == 0) //老缓冲区为空
        {
            upgrade_sign &= ~COM_NUM_ON_BIT;            //标记串口和延后写入
            SET_FLAG_BIT(upgrade_sign, PROGRAM_CODE_AFTER_ANSWER|(com << 1));
        }
        else
        {
            return (1 << 7);                            //前帧没写入, 新的帧又来了, 标识忙
        }
        if(last_pa == 0)                                //首包要擦除
        {
            buf[4] = 160;
        }
        else if(address_to_section_number(last_pa & ~(0xffu)) & (1u << 10))
        {
            buf[4] = 160;                               //需要擦除块
        }
        else                                            //只需要编程
        {
            buf[4] = 30;
        }
    }
    return 0;
}

/******************************************************************************
函数: void program_package_code(void)
入口: 无
出口: 无
概述: 查询到回应帧发送完毕后, 将足256字节的代码写入FLASH
    编程时间的消减也在这里
******************************************************************************/
void program_package_code (void)
{
#ifdef IDEA_ACC
    static U8 sec;
#endif  //IDEA_ACC
    U8 *p;
    U32 tmp, i;

#ifdef IDEA_ACC
    if(sec != Calendar.Sec)                             //签名标识生存期检查
    {
        if(Random_Status.life)                          //随机数
        {
            Random_Status.life--;
        }
        if(Sys_Class & (1 << ENABLE_UPDATE_CODE))       //升级
        {
            if(Random_Status.upgrade_life)
            {
                Random_Status.upgrade_life--;
            }
            else
            {
                Sys_Class &= ~(1 << ENABLE_UPDATE_CODE);
            }
        }
        if(Sys_Class & (1 << ENABLE_CLEAR_METER))       //清零
        {
            if(Random_Status.clear_life)
            {
                Random_Status.clear_life--;
            }
            else
            {
                Sys_Class &= ~(1 << ENABLE_CLEAR_METER);
            }
        }
        if(Sys_Class & (1 << ENABLE_PROGRAM))           //编程
        {
            if(Random_Status.programe_life)
            {
                Random_Status.programe_life--;
            }
            else
            {
                Sys_Class &= ~(1 << ENABLE_PROGRAM);
            }
        }
        sec = Calendar.Sec;
    }
#endif  //IDEA_ACC

//#ifdef PRINT_SLAVE_DBG
//    if(0 != CrdyCount)
//    {
//        DBGTHR = '\r';
//        DBGTHR = '\n';
//        DBGTHR = Ascii[(CrdyCount >> 28) & 0xf];
//        DBGTHR = Ascii[(CrdyCount >> 24) & 0xf];
//        DBGTHR = Ascii[(CrdyCount >> 20) & 0xf];
//        DBGTHR = Ascii[(CrdyCount >> 16) & 0xf];
//        DBGTHR = Ascii[(CrdyCount >> 12) & 0xf];
//        DBGTHR = Ascii[(CrdyCount >> 8) & 0xf];
//        DBGTHR = Ascii[(CrdyCount >> 4) & 0xf];
//        DBGTHR = Ascii[CrdyCount & 0xf];
//        CrdyCount = 0;
//    }
//#endif  //PRINT_SLAVE_DBG

    if(TEST_FLAG_BIT(upgrade_sign, PROGRAM_CODE_AFTER_ANSWER) == 0) //没有要写的
    {
        return;
    }
#ifdef DISPLAY_TERMINAL
    if(uart_send_idle((upgrade_sign >> 1) & 0x03, QUERY) == __FAIL)      //串口忙于发送
    {
        return;
    }
#else//DISPLAY_TERMINAL
    if(uart_open((upgrade_sign >> 1) & 0x03, QUERY) == __FAIL)      //串口忙于发送
    {
        return;
    }
#endif  //DISPLAY_TERMINAL
    CLR_FLAG_BIT(upgrade_sign, PROGRAM_CODE_AFTER_ANSWER);
    p = (U8*)Sample_Data;
    if(last_pa == 0)                                    //最近接收代码地址为0, 要编程的是首包, 先改写代码标识区
    {
        memcpy(p, (void *)RUN_USING_FLAG, FLAG_FLASH_MAX_READ); //保护全部常数数据, 改写标识区
        if(is_run_position() == 1)                      //运行在低区, 按运行区配置地址
        {
            last_pa = HIGH_CODE_START;                  //代码地址等已通过前面的检查
            p[4] |= 0x1a;                               //标识高区是新代码, 高区CRC没验证, 没运行过
        }
        else
        {
            last_pa = LOW_CODE_START;
            p[4] &= ~0x10;                              //最新代码为低区
            p[4] |= 0x05;                               //低区没运行过, CRC也未验证
        }
        program_run_using_flag(p);                      //改写标识区
    }
    else if((last_pa + 256) != last_addr)               //可能是信息块, 当前下载同上次写入地址不连续                                               //不是首帧
    {
        tmp = address_to_section_number(last_pa & 0xffffff00);  //将尾巴上的不完整帧写入
        if(tmp & (1 << 10))                             //扇区开始
        {
            prepare_sector(tmp & 0xff, tmp & 0xff);     //指定页面
            erase_sector(tmp & 0xff, tmp & 0xff);       //擦除页面
        }
        prepare_sector(tmp & 0xff, tmp & 0xff);         //指定页面
        write_data_FW(last_pa & 0xffffff00, data_code, 256);
        for(i = 0; i < 256 / 4; i++)                    //填充代码缓冲区
        {
            data_code[i] = 0xffffffff;
        }
        if((last_pa & 0xffff8000) != (last_addr & 0xffff8000))  //看看下一帧是否需要擦除, 考虑到最后的信息区块
        {
            for(i = 0; i < 256 / 4; i++)                //这新开始的程序对应块是否已被擦除了
            {
                if(*((U32 *)(last_addr + i * 4)) != 0xffffffff)
                {
                    break;
                }
            }
            if(i != 256 / 4)                            //块需要擦除, 目前只考虑了文件信息块
            {
                tmp = address_to_section_number(last_addr & 0xffff8000);
                prepare_sector(tmp & 0xff, tmp & 0xff);
                erase_sector(tmp & 0xff, tmp & 0xff);
            }
        }
    }
    while(data_code_len >= 256)                         //写入连续帧
    {
        p = (U8 *)data_code;
        tmp = i = last_addr & 0xff;                     //拷贝要烧写数据
        while(i < 256)
        {
            p[i] = Sample_Data[FLAG_FLASH_MAX_READ + i - tmp];
            i++;
        }
        data_code_len -= (256 - tmp);                   //长度减少了
        for(i = 0; i < data_code_len; i++)              //后面的代码前移
        {
            Sample_Data[FLAG_FLASH_MAX_READ + i] = Sample_Data[FLAG_FLASH_MAX_READ + 256 + i - tmp];
        }
        last_pa = last_addr & 0xffffff00;               //最后编程地址一定256对齐
        last_addr += (256 - tmp);                       //地址也增加
        tmp = address_to_section_number(last_pa);
        if(tmp & (1 << 10))                             //扇区开始
        {
            prepare_sector(tmp & 0xff, tmp & 0xff);     //指定页面
            erase_sector(tmp & 0xff, tmp & 0xff);       //擦除页面
        }
        prepare_sector(tmp & 0xff, tmp & 0xff);         //指定页面
        write_data_FW(last_pa, data_code, 256);
    }
    if(last_addr > UPGRADE_INFO_ADDR + 63)              //升级完毕将地址回零
    {
        last_pa = 0;
    }
}

/******************************************************************************
函数: U32 upgrading_on(void)
入口: 当前代码缓冲地址和长度
出口: 1 - 在升级中, 0 - 不是
概述: 判别当前是否在升级中, 为规避升级操作
******************************************************************************/
//U32 upgrading_on (void)
//{
//    if((last_addr <= CODE_END_ADDRESS) && (last_addr >= CODE_START_ADDRSS))
//    {
//        return 1;
//    }
//    return 0;
//}

#endif  //FIRST_RESPONSE

#ifdef IDEA_ACC
/******************************************************************************
函数: void code_signature(void)
入口: 无, 读取部分程序代码和签名芯片
出口: 无, 全局变量Sys_Class
概述: 验证代码和签名芯片的捆绑不可分割性
******************************************************************************/
void code_signature (void)
{
    U8 buf[64];
    U32 addr;
    U8 tmp;

    tmp = is_run_position();                            //验证代码CRC
    if(tmp == 0)                                        //代码运行区有疑问
    {
        Sys_Class |= (1u << SELF_CODE_SIGNATURE_FAIL);
        return;
    }
    if(check_code_crc(tmp - 1) != 1)                    //CRC检验码
    {
        Sys_Class |= (1u << SELF_CODE_SIGNATURE_FAIL);
        return;
    }
    memcpy(buf, HUA_CAI, 4);                            //华采, 4字节
//    memcpy(buf + 4, (void *)0, 8);                      //取BOOT和主程序入口向量, 16字节
    *((U32 *)(buf + 4)) = *((U32 *)0);                  //60020010 51010000
    *((U32 *)(buf + 8)) = *((U32 *)4);
    memcpy(buf + 12, (void *)CODE_START_ADDRSS, 8);
    file_read(DS2460_HEAD_ADDR + 0xf0, buf + 20, 8);    //DS2460芯片的唯一码, 8字节
    memcpy(buf + 28, version_info, 16);                 //版本信息等, 16字节
    memcpy(buf + 44, (void *)(CODE_START_ADDRSS + 0x3c), 4);  //TICK(3c) GPDMA(a8) T1(48)中断入口地址, 12+8字节
    memcpy(buf + 48, (void *)(CODE_START_ADDRSS + 0xa8), 4);
    memcpy(buf + 52, (void *)(CODE_START_ADDRSS + 0x48), 4 + 8);
    file_write(DS2460_HEAD_ADDR + 0, buf, 64);
    buf[0] = 0x8c;
    file_write(DS2460_HEAD_ADDR + 0x5c, buf, 1);
    delay(3300);
    addr = get_aa_addr(tmp);
    if(addr == 0)
    {
        Sys_Class |= (1u << SELF_CODE_SIGNATURE_FAIL);
        return;
    }
    memcpy(buf, (void *)(addr - 22), 20);
    file_write(DS2460_HEAD_ADDR + 0x5e, buf, 20);
    Sys_Class &= ~(1u << SELF_CODE_SIGNATURE_FAIL);
    if(I2C_Mem[I2C1].Error != NO_ERROR)
    {
        Sys_Class |= (1u << SELF_CODE_SIGNATURE_FAIL);  //签名验证没通过
    }
}

/******************************************************************************
函数: void get_random(void)
入口: 无, 直接保存在静态结构中
出口: 无, 直接存到buf里8字节
概述: 在进行外部密文核对前的随机数交换, 需要等待散列运算完成, 最少3.5mS
******************************************************************************/
//void get_random (void)
//{
//    U8 buf[64];
//
//                                                        //随机数8字节的产生用种子:
//    memcpy(buf, "竹海水韵", 8);
//    file_write(DS2460_HEAD_ADDR + 0x54, buf, 8);        //私密钥
//    get_on_chip_rtc(buf);                               //当前时间, 6字节, 星期的位置放年, hex格式的
//    buf[3] = buf[6];
//    buf[6] = Sys_Tick;                                  //Tick低, 2字节
//    buf[7] = Sys_Tick >> 8;
//#ifdef TEST_WWLLZZ
//    *((S32 *)(buf + 8)) = AD_Ctr[AD2].ha[0];            //历史AD2"过零"能量数据, 8字节
//    *((S32 *)(buf + 12)) = AD_Ctr[AD2].ha[1];
//#endif  //TEST_WWLLZZ
//    file_read(DS2460_HEAD_ADDR + 0xf0, buf + 16, 8);    //DS2460芯片的唯一码, 8字节
//    *((U32 *)(buf + 24)) = T1TC;                        //T1值, 4字节
//    *((U32 *)(buf + 28)) = T0TC;                        //T0值, 4字节
//    *((U32 *)(buf + 32)) = T2TC;                        //T2值, 4字节
//    buf[36] = buf[3];                                   //年月日时, 4字节
//    buf[37] = buf[5];
//    buf[38] = buf[4];
//    buf[39] = buf[2];
//    *((U32 *)(buf + 40)) = Data_From_CS5463[PHASE][ORG_TEMPERATURE];  //原始温度值, 4字节
//    memcpy(buf + 44, "浙江华采科技", 12);               //浙江华采科技, 12字节
//#ifdef TEST_WWLLZZ
//    *((S32 *)(buf + 56)) = AD_Ctr[AD1].ha[0];           //历史AD1"过零"能量数据, 8字节
//    *((S32 *)(buf + 60)) = AD_Ctr[AD1].ha[1];
//#endif  //TEST_WWLLZZ
//    file_write(DS2460_HEAD_ADDR + 0, buf, 64);
//    buf[0] = 0x84;                                      //用S0, 计算到out
//    file_write(DS2460_HEAD_ADDR + 0x5c, buf, 1);
//    delay(3500);
//    file_read(DS2460_HEAD_ADDR + 0x40, Random_Status.random, 8);
//    if(I2C_Mem[I2C1].Error != NO_ERROR)
//    {
//        Random_Status.life = 0;                         //DS2460芯片有故障
//    }
//    else
//    {
//        Random_Status.life = RANDOM_LIFE;
//    }
//}

/******************************************************************************
函数: U32 signature_secret(U32 index, U8 buf[])
入口: index - 要验证密码级别, UPDATE_CLASS升级, CLEAR_METER_CLASS清零, PROGRAM_CLASS编程. buf - 待比较20字节数据
出口: 1核对正确, 0不正确
概述: 核对约定级别签名
******************************************************************************/
//U32 signature_secret (U32 index, U8 buf[])
//{
//    U8 p[64];                                           //不是嵌套最深子程序, 以后需注意
//    U32 i;
//
//    if(Random_Status.life == 0)                         //随机数过期
//    {
//        return 0;
//    }
//    Random_Status.life = 0;                             //就进行这一次验证, 以后就失效
//    if(index == UPDATE_CLASS)                           //升级
//    {
//        memcpy(p, VERSION_INFO_S, 56);
////        memcpy(p + Calendar.Date, (void *)(Random_Status.random), 8);    //放的位置随日期变化, 编译其优化惹祸了
//        for(i = 0; i < 8; i++)                          //规避编译器优化问题
//        {
//            p[Calendar.Date + i] = Random_Status.random[i];
//        }
//        file_read(DS2460_HEAD_ADDR + 0xf0, p + 56, 8);
//        file_write(DS2460_HEAD_ADDR + 0, p, 64);
//        p[0] = 0x8c;                                    //用E1计算到out
//        file_write(DS2460_HEAD_ADDR + 0x5c, p, 1);
//        delay(3500);
//        i = file_write(DS2460_HEAD_ADDR + 0x5e, buf, 20);
//        if(i == NO_ERROR)
//        {
//            Sys_Class |= (1u << ENABLE_UPDATE_CODE);
//            Random_Status.upgrade_life = UPGRADE_LIFE;
//            return 1;
//        }
//    }
//    memcpy(p, HUA_CAI, 4);
//    if(index == CLEAR_METER_CLASS)                      //清零
//    {
//        memcpy(p + 4, Random_Status.random, 8);         //约定的随机数
//        memcpy(p + 12, XIAO_PIP, 44);
//        p[12] ^= RTCYEAR;                               //加入年月日信息
//        p[17] ^= RTCMONTH;
//        p[38] ^= RTCDOM;
//        file_read(DS2460_HEAD_ADDR + 0xf0, p + 56, 8);
//        file_write(DS2460_HEAD_ADDR + 0, p, 64);
//        p[0] = 0x94;                                    //用E2算出到out
//        file_write(DS2460_HEAD_ADDR + 0x5c, p, 1);
//        delay(3500);
//        i = file_write(DS2460_HEAD_ADDR + 0x5e, buf, 20);   //核对20字节签名
//        if(i == NO_ERROR)
//        {
//            Sys_Class |= (1u << ENABLE_CLEAR_METER);       //允许清底度
//            Random_Status.clear_life = CLEAR_LIFE;
//            return 1;
//        }
//    }
//    if(index == PROGRAM_CLASS)                          //编程
//    {
//        memcpy(p + 4, DATE_PIP, 52);                    //先生成时间随机数20字节
//        p[12] ^= RTCYEAR;                               //加入年月日信息
//        p[17] ^= RTCMONTH;
//        p[38] += RTCDOM;
//        memcpy(p + 24, Random_Status.random, 8);        //随机数
//        file_read(DS2460_HEAD_ADDR + 0xf0, p + 56, 8);
//        file_write(DS2460_HEAD_ADDR + 0, p, 64);
//        p[0] = 0x9c;                                    //用E3计算到out
//        file_write(DS2460_HEAD_ADDR + 0x5c, p, 1);
//        delay(3500);
//        i = file_write(DS2460_HEAD_ADDR + 0x5e, buf, 20);   //核对20字节签名
//        if(i == NO_ERROR)
//        {
//            Sys_Class |= (1u << ENABLE_PROGRAM); //允许编程
//            Random_Status.programe_life = PROGRAM_LIFE;
//            return 1;
//        }
//    }
//    return 0;
//}

/******************************************************************************
函数: U32 get_aa_addr(U32 zone)
入口: zone - 运行区, 1-低区, 2-高区, 其它无效
出口: 地址
概述: 从最后可能执行代码开始向前找第一个不是ff的位置
******************************************************************************/
U32 get_aa_addr (U32 zone)
{
    U32 i;

    if(zone == 2)                                       //高区
    {
        for(i = HIGH_CODE_END - 64 - 3; i != HIGH_CODE_START; i -= 4)
        {
            if(*((U32 *)i) != 0xffffffff)
            {
BYTE_SEARCH_ADDR:
                i += 3;
                if(*((U8 *)i) == 0xaa)
                {
                    return i;
                }
                i--;
                if(*((U8 *)i) == 0xaa)
                {
                    return i;
                }
                i--;
                if(*((U8 *)i) == 0xaa)
                {
                    return i;
                }
                i--;
                if(*((U8 *)i) == 0xaa)
                {
                    return i;
                }
                return 0;
            }
        }
    }
    if(zone == 1)                                       //低区
    {
        for(i = LOW_CODE_END - 64 - 3; i != LOW_CODE_START; i -= 4)
        {
            if(*((U32 *)i) != 0xffffffff)
            {
                goto BYTE_SEARCH_ADDR;
            }
        }
    }
    return 0;
}
#endif  //IDEA_ACC


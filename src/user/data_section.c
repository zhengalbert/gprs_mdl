/******************************************************************************
 Copyright (C) 2012  R&D Institute of HuaCai Co.,Ltd.
 File Name      : data_section.c
 Description    : 程序控制标识和应用程序数据区
 Related Files  :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2011-12-18    wwllzz          the original version
******************************************************************************/
#include "..\inc\global_config.h"
//#include    "..\src\bsp\event_define_d.h"               //有关事件宏的定义

//-------------------------------- 常数定义 -----------------------------------
#ifdef __RELEASE_H
const U32 RUNNING_FLAG __attribute__((section(".ARM.__at_0x1000"))) __attribute__((used)) = 2;            //要求运行代码H
const U8 FW_CRC_CORRECT_FLAG __attribute__((section(".ARM.__at_0x1004"))) __attribute__((used)) = 0xF5;   //高区运行过且CRC正确
//bit0为0表示低区曾有效运行,
//bit1为0表示高区曾有效运行.
//bit2为0表示低区已下载代码CRC验证正确,
//bit3为0表示高区已下载代码CRC验证正确.
//bit4为1表示最新下载为高区
#endif  //__RELEASE_H

#ifdef __RELEASE_L
const U32 RUNNING_FLAG __attribute__((section(".ARM.__at_0x1000"))) __attribute__((used)) = 1;            //要求运行代码L
const U8 FW_CRC_CORRECT_FLAG __attribute__((section(".ARM.__at_0x1004"))) __attribute__((used)) = 0xEA;   //低区运行过且CRC正确
//bit0为0表示低区曾有效运行,
//bit1为0表示高区曾有效运行.
//bit2为0表示低区已下载代码CRC验证正确,
//bit3为0表示高区已下载代码CRC验证正确.
//bit4为0表示最新下载为低区
#endif  //__RELEASE_L

//AZH
#pragma arm section rwdata = "RW_IRAM2"
#pragma pack(4)
//U8 Sample_Data[4096 * 2];                               //只需要至少4K内存, 再有一点点就够了

#ifdef DISPLAY_TERMINAL_2
U32 Sample_Data[1024 * 2];
#else   //DISPLAY_TERMINAL_2
U32 Sample_Data[256 * 3 / 4];                           //目前这些内存足够

#pragma arm section rwdata

static const U32 fil[1024 - 2] __attribute__((used)) = {    //用0填充
	0
};
#endif  //DISPLAY_TERMINAL_2


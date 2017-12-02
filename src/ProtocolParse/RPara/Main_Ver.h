/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_Ver.h
 Description    : 终端版本信息
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-08-15    azh     the original version
******************************************************************************/
#ifndef _MAIN_VER_H_
#define _MAIN_VER_H_

#ifdef ZIGBEE_TEST//azh 171011 配电子式高压表GPRS模块
    //终端版本信息
    #define CON_TER_SOFTWAREVER_0       '1'//0x30                       //软件版本，4字节
    #define CON_TER_SOFTWAREVER_1       '.'//0x32
    #define CON_TER_SOFTWAREVER_2       '0'//0x32
    #define CON_TER_SOFTWAREVER_3       '0'//0x35
    #define CON_TER_SOFTWARE_DAY        0x20//0x05//0x04                    //终端软件发布日期：日月年
    #define CON_TER_SOFTWARE_MON        0x10//0x04//0x05
    #define CON_TER_SOFTWARE_YEAR       0x17
    
    #define CON_TER_INNERVER_0          0x42                    //内部版本号 BF--
    #define CON_TER_INNERVER_1          0x46
    #define CON_TER_INNERVER_2          0x30
    #define CON_TER_INNERVER_3          0x31

	#define CON_TER_HARDWAREVER_0       '1'//0x30                       //硬件件版本，4字节
	#define CON_TER_HARDWAREVER_1       '.'//0x31
	#define CON_TER_HARDWAREVER_2       '0'//0x41
	#define CON_TER_HARDWAREVER_3       '0'//0x50
	#define CON_TER_HARDWARE_DAY        0x22//0x04                  //终端硬件发布日期：日月年
	#define CON_TER_HARDWARE_MON        0x07//0x05
	#define CON_TER_HARDWARE_YEAR       0x17
#else
    //终端版本信息
    #define CON_TER_SOFTWAREVER_0       '1'//0x30                       //软件版本，4字节
    #define CON_TER_SOFTWAREVER_1       '.'//0x32
    #define CON_TER_SOFTWAREVER_2       '4'//0x32
    #define CON_TER_SOFTWAREVER_3       '0'//0x35
    #define CON_TER_SOFTWARE_DAY        0x11//0x05//0x04                    //终端软件发布日期：日月年
    #define CON_TER_SOFTWARE_MON        0x10//0x04//0x05
    #define CON_TER_SOFTWARE_YEAR       0x17
    
    #define CON_TER_INNERVER_0          0x42                    //内部版本号 BF--
    #define CON_TER_INNERVER_1          0x46
    #define CON_TER_INNERVER_2          0x30
    #define CON_TER_INNERVER_3          0x31

	#define CON_TER_HARDWAREVER_0       '1'//0x30                       //硬件件版本，4字节
	#define CON_TER_HARDWAREVER_1       '.'//0x31
	#define CON_TER_HARDWAREVER_2       '2'//0x41
	#define CON_TER_HARDWAREVER_3       '0'//0x50
	#define CON_TER_HARDWARE_DAY        0x31//0x04                  //终端硬件发布日期：日月年
	#define CON_TER_HARDWARE_MON        0x03//0x05
	#define CON_TER_HARDWARE_YEAR       0x14
#endif//ZIGBEE_TEST




#define CON_LCD_AREA_0              'B'                     //液晶显示版本号附带地区名称
#define CON_LCD_AREA_1              'F'

#endif

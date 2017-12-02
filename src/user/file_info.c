/******************************************************************************
 Copyright (C) 2012  R&D Institute of HuaCai Co.,Ltd.
 File Name      : file_info.c
 Description    : 版本信息
 Related Files  :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2011-12-18    wwllzz          the original version
******************************************************************************/
#include "..\inc\global_config.h"

//-------------------------------- 常数定义 -----------------------------------
const U8 version_info[39] __attribute__((used)) =       //版本信息
{
    0x30 + (VER_MAIN >> 8),                             //主版本号
    0x30 + (VER_MAIN & 0xf),
    '.',
    0x30 + (VER_PARTICULAR >> 8),                       //次版本号
    0x30 + (VER_PARTICULAR & 0xf),
    ' ',
    0x30 + ((ISSUE_YMD_V >> 20) & 0xf),                 //发行日期
    0x30 + ((ISSUE_YMD_V >> 16) & 0xf),
    '-',
    0x30 + ((ISSUE_YMD_V >> 12) & 0xf),
    0x30 + ((ISSUE_YMD_V >> 8) & 0xf),
    '-',
    0x30 + ((ISSUE_YMD_V >> 4) & 0xf),
    0x30 + (ISSUE_YMD_V & 0xf),

#ifdef __RELEASE_H
    'H',                                                //高区
#endif  //__RELEASE_H
#ifdef __RELEASE_L
    'L',                                                //低区
#endif  //__RELEASE_L

    '2',                                                //表1 终端2

#if CODE_VER_TYPE==H_METER_PRJ				//配合高压表
//    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'G', 'W', 'D', 'i', 's', '-', 'T', 'Y', 'T', 'Y', '0', '0', 'G', '0', ' '
//	#ifdef MC55_GPRS
//    ' ', ' ', ' ', ' ', 'S', '5', '2', 'i', 'G', 'W', 'D', 'i', 's', '-', 'T', 'Y', 'T', 'Y', '0', '1', 'S', '1', ' '
//	#endif//MC55_GPRS
//	#ifdef TELIT_GPRS
//    ' ', ' ', ' ', ' ', 'T', '8', '6', '8', 'G', 'W', 'D', 'i', 's', '-', 'T', 'Y', 'T', 'Y', '0', '1', 'T', '0', ' '
//	#endif//TELIT_GPRS
    ' ', ' ', ' ', ' ', 'G', 'M', 'i', 'x', 'G', 'W', 'D', 'i', 's', '-', 'T', 'Y', 'T', 'Y', '0', '1', 'G', '0', ' '
#elif CODE_VER_TYPE==WLS_CUR_PRJ			//配合无线采集器
//    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'G', 'W', 'D', 'i', 's', '-', 'W', 'C', 'u', 'r', '0', '0', 'G', '1', ' '
//	#ifdef MC55_GPRS
//    ' ', ' ', ' ', ' ', 'S', '5', '2', 'i', 'G', 'W', 'D', 'i', 's', '-', 'W', 'C', 'u', 'r', '0', '0', 'S', '1', ' '
//	#endif//MC55_GPRS
//	#ifdef TELIT_GPRS
//    ' ', ' ', ' ', ' ', 'T', '8', '6', '8', 'G', 'W', 'D', 'i', 's', '-', 'W', 'C', 'u', 'r', '0', '0', 'T', '0', ' '
//	#endif//TELIT_GPRS
    ' ', ' ', ' ', ' ', 'G', 'M', 'i', 'x', 'G', 'W', 'D', 'i', 's', '-', 'W', 'C', 'u', 'r', '0', '0', 'G', '0', ' '
#elif CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
    //    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'G', 'W', 'M', 'd', 'l', '-', 'T', 'Y', 'T', 'Y', '0', '4', 'G', '0', ' '
    //	#ifdef MC55_GPRS
    //    ' ', ' ', ' ', ' ', 'S', '5', '2', 'i', 'G', 'W', 'M', 'd', 'l', '-', 'T', 'Y', 'T', 'Y', '0', '4', 'S', '1', ' '
    //	#endif//MC55_GPRS
    //	#ifdef TELIT_GPRS
    //    ' ', ' ', ' ', ' ', 'T', '8', '6', '8', 'G', 'W', 'M', 'd', 'l', '-', 'T', 'Y', 'T', 'Y', '0', '4', 'T', '0', ' '
    //	#endif//TELIT_GPRS
    #ifdef ZIGBEE_TEST//azh 171011 配电子式高压表GPRS模块
        ' ', ' ', ' ', ' ', 'G', 'M', 'i', 'x', 'G', 'W', 'M', 'd', 'l', '-', 'H', 'm', 't', 'Y', '0', '4', 'G', '0', ' '
    #else    
        ' ', ' ', ' ', ' ', 'G', 'M', 'i', 'x', 'G', 'W', 'M', 'd', 'l', '-', 'Y', 'd', 's', 'Y', '0', '4', 'G', '0', ' '  
    #endif//      
#endif
};
//azh 最后8位(倒数第二位开始)用于升级判别zzxxnnpn(字母) 只能是相同的 上位机软件都会支持升级
//zz: 地区省份 2位字母 通用则填TY
//xx  市 2位字母 全国通用则填TY
//nn: 产品序号 2位数字  
//pn: 协议  1位字母+1位数字 N(南网) G(国网)
//azh 140918 pn重新定义 因为升级软件就信息对后面几位 所以GPRS信息加到最后2位pn
//pn: p=S:SIEMENS-MC52i,  p=T:TELIT868
//    n=0贴片，1插件，

static const char d[] __attribute__((used)) = {"华采" __DATE__ " " __TIME__};


/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Run_Para_Export.h
 Description    : 所有运行参数 数据结构声明
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-08-05    azh     the original version
******************************************************************************/

#ifndef _RUN_PARA_EXPORT_H_
#define _RUN_PARA_EXPORT_H_

    //终端信息
#ifdef ZIGBEE_TEST//azh 171011 配电子式高压表GPRS模块
    #define CON_TER_MANUFACTORNO_0  0x31                        //1700生产厂商编号  4字节
    #define CON_TER_MANUFACTORNO_1  0x37
    #define CON_TER_MANUFACTORNO_2  0x30
    #define CON_TER_MANUFACTORNO_3  0x30
    #define CON_TER_DEVNO_0 'M'
    #define CON_TER_DEVNO_1 'd'
    #define CON_TER_DEVNO_2 'l'
    #define CON_TER_DEVNO_3 '-'//
    #define CON_TER_DEVNO_4 '-'
    #define CON_TER_DEVNO_5 'H'
    #define CON_TER_DEVNO_6 'm'
    #define CON_TER_DEVNO_7 't'
#else
    #define CON_TER_MANUFACTORNO_0  0x31                        //1700生产厂商编号  4字节
    #define CON_TER_MANUFACTORNO_1  0x37
    #define CON_TER_MANUFACTORNO_2  0x30
    #define CON_TER_MANUFACTORNO_3  0x30
    #define CON_TER_DEVNO_0 'M'
    #define CON_TER_DEVNO_1 'd'
    #define CON_TER_DEVNO_2 'l'
    #define CON_TER_DEVNO_3 '-'//
    #define CON_TER_DEVNO_4 '-'
    #define CON_TER_DEVNO_5 'G'
    #define CON_TER_DEVNO_6 'm'
    #define CON_TER_DEVNO_7 'l'
#endif//ZIGBEE_TEST

//参数数据标识
#define CON_PARA_BEG_CODE       0x1000  //参数起始编码
#define CON_PARA_END_CODE       0x14FF  //参数终止编码
#define CON_CTRL_BEG_CODE       0x1500  //控制起始编码
#define CON_CTRL_END_CODE       0x16FF  //控制终止编码
#define CON_CONFIG_BEG_CODE     0x1700  //终端配置起始编码
#define CON_CONFIG_END_CODE     0x17FF  //终端配置起始编码
#define CON_LOGIN_BEG_CODE      0x1800  //身份认证起始编码
#define CON_LOGIN_END_CODE      0x18FF  //身份认证起始编码

#define CON_PARA_F1_COMMPARA        CON_PARA_BEG_CODE+1     //F1：终端通信参数设置
#define CON_PARA_F2_RELAYPARA       CON_PARA_BEG_CODE+2     //F2：终端中继转发设置
#define CON_PARA_F3_STAIPADDR       CON_PARA_BEG_CODE+3     //F3：主站IP地址和端口
#define CON_PARA_F4_STAPHONENO      CON_PARA_BEG_CODE+4     //F4：主站电话号码和短信中心号码
#define CON_PARA_F5_TERENCRYPT      CON_PARA_BEG_CODE+5     //F5：终端密码设置
#define CON_PARA_F6_TERGROUPADDR    CON_PARA_BEG_CODE+6     //F6：终端组地址设置
#define CON_PARA_F7_IPADDR          CON_PARA_BEG_CODE+7     //F7：终端抄表日设置
#define CON_PARA_F8_ALRCTLWORD      CON_PARA_BEG_CODE+8     //F8：终端事件记录配置设置
#define CON_PARA_F9_CONFIGNUM       CON_PARA_BEG_CODE+9     //F9：终端配置数量表
#define CON_PARA_F10_METERPARA      CON_PARA_BEG_CODE+10    //F10：终端电能表/交流采样装置配置参数
#define CON_PARA_F11_PULSEPARA      CON_PARA_BEG_CODE+11    //F11：终端脉冲配置参数
#define CON_PARA_F12_DINPUTPARA     CON_PARA_BEG_CODE+12    //F12：终端状态量输入参数
#define CON_PARA_F13_ANALOGPARA     CON_PARA_BEG_CODE+13    //F13：终端电压/电流模拟量配置参数
#define CON_PARA_F14_SUMGPARA       CON_PARA_BEG_CODE+14    //F14：终端总加组配置参数
#define CON_PARA_F15_POWVALDIFF     CON_PARA_BEG_CODE+15    //F15：有功总电能量差动越限事件参数设置
#define CON_PARA_F16_PDPINFO        CON_PARA_BEG_CODE+16    //F16：虚拟专网用户名、密码
#define CON_PARA_F17_KEEPPOWER      CON_PARA_BEG_CODE+17    //F17：终端保安定值
#define CON_PARA_F18_POWCTRLTIMEPER CON_PARA_BEG_CODE+18    //F18：终端功控时段
#define CON_PARA_F19_POWCTRLCOEF    CON_PARA_BEG_CODE+19    //F19：终端时段功控定值浮动系数
#define CON_PARA_F20_ENERCTRLCOEF   CON_PARA_BEG_CODE+20    //F20：终端月电能量控定值浮动系数
#define CON_PARA_F21_PERTIMEFEE     CON_PARA_BEG_CODE+21    //F21：终端电能量费率时段和费率数
#define CON_PARA_F22_FEEPARA        CON_PARA_BEG_CODE+22    //F22：终端电能量费率
#define CON_PARA_F23_URGEALARMPARA  CON_PARA_BEG_CODE+23    //F23：终端催费告警参数
#define CON_PARA_F24_COPYINTER      CON_PARA_BEG_CODE+24    //F24：终端抄表间隔设置
#define CON_PARA_F25_MPBASEPARA     CON_PARA_BEG_CODE+25    //F25：测量点基本参数
#define CON_PARA_F26_MPLIMIT        CON_PARA_BEG_CODE+26    //F26：测量点限值参数
#define CON_PARA_F27_MPFREEZEPARA   CON_PARA_BEG_CODE+27    //F27：测量点数据冻结参数
#define CON_PARA_F28_MPPOWRATEPARA  CON_PARA_BEG_CODE+28    //F28：测量点功率因数分段限值
#define CON_PARA_F29_METERADDR      CON_PARA_BEG_CODE+29    //F29：终端当地电能表显示号
#define CON_PARA_F30_METERCOPYFLAG  CON_PARA_BEG_CODE+30        //F30  终端投抄/停抄
#define CON_PARA_F31_SLAVENODE      CON_PARA_BEG_CODE+31
#define CON_PARA_F33_SUMGFREEZEPARA CON_PARA_BEG_CODE+33    //F33：总加组数据冻结参数
#define CON_PARA_F34_PORTPROPERTY   CON_PARA_BEG_CODE+34    //F34
#define CON_PARA_F35_EMPHAFLAG      CON_PARA_BEG_CODE+35    //F35
#define CON_PARA_F37_LINKPARA       CON_PARA_BEG_CODE+37    //F37:级联通信参数
#define CON_PARA_F38_JOB1PARA       CON_PARA_BEG_CODE+38        //F38一类数据配置设置
#define CON_PARA_F39_JOB2PARA       CON_PARA_BEG_CODE+39        //F39二类数据配置设置
#define CON_PARA_F41_TIMEPERSETTING CON_PARA_BEG_CODE+41    //F41：时段功控定值
#define CON_PARA_F42_RESTCTRL       CON_PARA_BEG_CODE+42    //F42：厂休功控参数
#define CON_PARA_F43_POWERSLIP      CON_PARA_BEG_CODE+43    //F43：功率控制的功率计算滑差时间
#define CON_PARA_F44_SALESTOP       CON_PARA_BEG_CODE+44    //F44：营业报停控参数
#define CON_PARA_F45_POWERCTRLTURN  CON_PARA_BEG_CODE+45    //F45：功控轮次设定
#define CON_PARA_F46_MONTHENERCTRL  CON_PARA_BEG_CODE+46    //F46：月电量控定值
#define CON_PARA_F47_BUYENERCTRL    CON_PARA_BEG_CODE+47    //F47：购电量控参数
#define CON_PARA_F48_ENERCTRLTURN   CON_PARA_BEG_CODE+48    //F48：电控轮次设定
#define CON_PARA_F49_CTRLALRTIME    CON_PARA_BEG_CODE+49    //F49：功控告警时间
#define CON_PARA_F57_SOUNDALARM     CON_PARA_BEG_CODE+57    //F57：终端声音告警允许∕禁止设置
#define CON_PARA_F58_KEEPAUTO       CON_PARA_BEG_CODE+58    //F58：终端自动保电参数
#define CON_PARA_F59_MTRERRORPARA   CON_PARA_BEG_CODE+59    //F59：电能表异常判别阈值设定
#define CON_PARA_F60_HARMONPARA     CON_PARA_BEG_CODE+60    //F60：谐波限值
#define CON_PARA_F61_DCAINPARA      CON_PARA_BEG_CODE+61    //F61：直流模拟量接入参数
#define CON_PARA_F62_COMMTYPE       CON_PARA_BEG_CODE+62    //F62 虚拟专网工作方式
#define CON_PARA_F63_FLUXLIMIT      CON_PARA_BEG_CODE+36    //F63 日通信流量门限设置
#define CON_PARA_F65_UPDATA1PARA    CON_PARA_BEG_CODE+65    //F65：定时发送1类数据任务设置
#define CON_PARA_F66_UPDATA2PARA    CON_PARA_BEG_CODE+66    //F66：定时发送2类数据任务设置
#define CON_PARA_F67_UPDATA1CTRL    CON_PARA_BEG_CODE+67    //F67：定时发送1类数据任务启动/停止设置
#define CON_PARA_F68_UPDATA2CTRL    CON_PARA_BEG_CODE+68    //F68：定时发送2类数据任务启动/停止设置
#define CON_PARA_F73_CAPPARA        CON_PARA_BEG_CODE+73    //F73：电容器参数
#define CON_PARA_F74_CAPRUNPARA     CON_PARA_BEG_CODE+74    //F74：电容器投切运行参数
#define CON_PARA_F75_CAPRELAYPARA   CON_PARA_BEG_CODE+75    //F75：电容器保护参数
#define CON_PARA_F76_CAPCTRL        CON_PARA_BEG_CODE+76    //F76：电容器投切控制方式
#define CON_PARA_F81_DCARATE        CON_PARA_BEG_CODE+81    //F81：直流模拟量输入变比
#define CON_PARA_F82_DCALIMIT       CON_PARA_BEG_CODE+82    //F82：直流模拟量限值
#define CON_PARA_F83_DCAFREESE      CON_PARA_BEG_CODE+83    //F83：直流模拟量冻结参数
#define CON_PARA_F89_ADDR           CON_PARA_BEG_CODE+89    //F89：终端地址更改
//azh170916
#define CON_PARA_F90_WIRELESS       CON_PARA_BEG_CODE+90    //F90：设置无线通信参数
#define CON_PARA_F91_LOCATION       CON_PARA_BEG_CODE+91    //F91：终端地理位置信息
#define CON_PARA_F150_SNSTATE       CON_PARA_BEG_CODE+150   //F150：测量点状态

#define CON_PARA_F207_ADDR          CON_PARA_BEG_CODE+207   //F207：终端地址更改

#define CON_CTRL_F1_YK_OPEN         CON_CTRL_BEG_CODE+1     //F1遥控跳闸
#define CON_CTRL_F2_YK_CLOSE        CON_CTRL_BEG_CODE+2     //F2遥控允许合闸
#define CON_CTRL_F9_TIMEPER_ON      CON_CTRL_BEG_CODE+9     //F9时段功控投入
#define CON_CTRL_F10_RESTCTRL_ON    CON_CTRL_BEG_CODE+10    //F10厂休功控投入
#define CON_CTRL_F11_SALESTOP_ON    CON_CTRL_BEG_CODE+11    //F11营业报停功控投入
#define CON_CTRL_F12_POWERLOWER_ON  CON_CTRL_BEG_CODE+12    //F12当前功率下浮控投入
#define CON_CTRL_F15_MONTHENER_ON   CON_CTRL_BEG_CODE+15    //F15月电控投入
#define CON_CTRL_F16_BUYENER_ON     CON_CTRL_BEG_CODE+16    //F16购电控投入
#define CON_CTRL_F17_TIMEPER_OFF    CON_CTRL_BEG_CODE+17    //F17时段功控解除
#define CON_CTRL_F18_RESTCTRL_OFF   CON_CTRL_BEG_CODE+18    //F18厂休功控解除
#define CON_CTRL_F19_SALESTOP_OFF   CON_CTRL_BEG_CODE+19    //F19营业报停功控解除
#define CON_CTRL_F20_POWERLOWER_OFF CON_CTRL_BEG_CODE+20    //F20当前功率下浮控解除
#define CON_CTRL_F23_MONTHENER_OFF  CON_CTRL_BEG_CODE+23    //F23月电控解除
#define CON_CTRL_F24_BUYENER_OFF    CON_CTRL_BEG_CODE+24    //F24购电控解除
#define CON_CTRL_F25_KEEPPOWER_ON   CON_CTRL_BEG_CODE+25    //F25终端保电投入
#define CON_CTRL_F26_URGEALARM_ON   CON_CTRL_BEG_CODE+26    //F26催费告警投入
#define CON_CTRL_F27_TALK_ON        CON_CTRL_BEG_CODE+27    //F27允许终端与主站通话
#define CON_CTRL_F28_REMOVE_ON      CON_CTRL_BEG_CODE+28    //F28终端剔除投入
#define CON_CTRL_F29_UPACTIVE_ON    CON_CTRL_BEG_CODE+29    //F29允许终端主动上报
//azh 170920
#define CON_CTRL_F30_SETMETERTIME_SWITCH    CON_CTRL_BEG_CODE+30    //F30终端对电能表对时功能的开启与关闭

#define CON_CTRL_F31_SETTIME        CON_CTRL_BEG_CODE+31    //F31对时命令
#define CON_CTRL_F32_MESSAGE        CON_CTRL_BEG_CODE+32    //F32中文信息
#define CON_CTRL_F33_KEEPPOWER_OFF  CON_CTRL_BEG_CODE+33    //F33终端保电解除
#define CON_CTRL_F34_URGEFEE_OFF    CON_CTRL_BEG_CODE+34    //F34催费告警解除
#define CON_CTRL_F35_TALK_OFF       CON_CTRL_BEG_CODE+35    //F35禁止终端与主站通话
#define CON_CTRL_F36_REMOVE_OFF     CON_CTRL_BEG_CODE+36    //F36终端剔除解除
#define CON_CTRL_F37_UPACTIVE_OFF   CON_CTRL_BEG_CODE+37    //F37禁止终端主动上报
#define CON_CTRL_F38_SMS_ACTIVE     CON_CTRL_BEG_CODE+38    //F38短信激活
#define CON_CTRL_F39_DISCONECT      CON_CTRL_BEG_CODE+39    //F39断开连接
#define CON_CTRL_F41_CAPCTRL_ON     CON_CTRL_BEG_CODE+41    //F41电容控制投入
#define CON_CTRL_F42_CAPCTRL_OFF    CON_CTRL_BEG_CODE+42    //F42电容控制解除

#define CON_CTRL_F49                CON_CTRL_BEG_CODE+49
#define CON_CTRL_F50                CON_CTRL_BEG_CODE+50
#define CON_CTRL_F51                CON_CTRL_BEG_CODE+51
//#define CON_CTRL_F88_TIMEREVISE       CON_PARA_BEG_CODE+88    //F88：精确对时命令//山西规约加
//azh 170920
#define CON_CTRL_F149_METERSELF_SWITCH       CON_CTRL_BEG_CODE+149    //F149电能表通信参数自动维护功能开启与关闭
#define CON_CTRL_F150_RESETSNSTATE_SWITCH    CON_CTRL_BEG_CODE+150    //F150复位测量点参数变更标志

#define CON_CONFIG_F1_VERSION       CON_CONFIG_BEG_CODE+1   //F1配置终端版本信息
#define CON_CONFIG_F2_INTERFACE     CON_CONFIG_BEG_CODE+2   //F2配置终端输入、输出及通信端口配置
#define CON_CONFIG_F3_OTHER         CON_CONFIG_BEG_CODE+3   //F3配置终端其他配置
#define CON_CONFIG_F4_PARA          CON_CONFIG_BEG_CODE+4   //F4配置终端参数配置（本终端软硬件版本支持的参数）
#define CON_CONFIG_F5_CTRL          CON_CONFIG_BEG_CODE+5   //F5配置终端控制配置（本终端软硬件版本支持的控制命令）
#define CON_CONFIG_F6_DATA1         CON_CONFIG_BEG_CODE+6   //F6配置终端1类数据配置（本终端软硬件版本支持的1类数据）
#define CON_CONFIG_F7_DATA2         CON_CONFIG_BEG_CODE+7   //F7配置终端2类数据配置（本终端软硬件版本支持的2类数据）
#define CON_CONFIG_F8_ERC           CON_CONFIG_BEG_CODE+8   //F8配置终端事件记录配置（本终端软硬件版本支持的事件记录）
//azh 170920
#define CON_CONFIG_F9_REMOTEVER     CON_CONFIG_BEG_CODE+9   //F9远程通信模块版本信息
#define CON_CONFIG_F10_LOCALVER     CON_CONFIG_BEG_CODE+10  //F10本地通信模块版本信息

#define CON_LOGIN_F1_REQUEST        CON_LOGIN_BEG_CODE+1    //F1身份认证请求
#define CON_LOGIN_F2_ANSWER         CON_LOGIN_BEG_CODE+2    //F2身份认证响应


/*
将终端的数据分为两类：
1. 终端内部的数据，这些数据与具体的通信规约无关，例如正901F等
2. 规约中需要的数据，这些数据一般由1类数据构成
*/
/******************************************测量点参数****************************************/
#define CON_MAX_MPNUM               2//15
#define CON_METER_NUM               1//8
#define CON_PULSE_NUM               0//8
#define CON_SWITCHIN_NUM            0//8        //开关量数量，包括1路门节点，4路遥信量
#define CON_SWITCHOUT_NUM           0//4        //开关数量
#define CON_PARA_SWITCHOUT_NUM      0/4     //开关数量（参数用）
#define CON_ANALOG_NUM              0//8

#define     CON_PORT_NUM                    2
#define     CON_COMMPORT_NUM            2

#define CON_RATEVOL_3P4L1           220
#define CON_RATEVOL_3P3L            100
#define CON_RATEVOL_3P4L2           57

#define CON_MPTYPE_NULL             0x00    //  测量点类型为不存在
#define CON_MPTYPE_TER              0x01    //  测量点类型为终端
#define CON_MPTYPE_METER            0x02    //  测量点类型为表计
#define CON_MPTYPE_PULSE            0x03    //  测量点类型为脉冲量

#define CON_MPTYPE_SUMG             0x04    //  总加

#define CON_MPNO_INVALID            0xFF    // 无效MPNo
#define CON_MPNO_TER_FIXED          0xFE    //  终端固定
#define CON_MPNO_TER                0xFE//0//8 //  终端
//#define   CON_MPNO_SUMG_FIXED         63      //  总加虚拟的测量点，固定
#define CON_MPNO_SUMG_FIXED         40      //  总加虚拟的测量点，固定
#define CON_METER_BASE_MPNO         1       //  表计测量点启始编号
#define CON_PULSE_BASE_MPNO         32      //  脉冲测量点起始编号

#define CON_MAX_MTRFREEZENUM        75      //  最多的冻结数
#define CON_MAX_SUMGFREEZENUM       10      //  最多的总加组冻结数

#define CON_TIMEINTER_NOT           0x00    //  不冻结
#define CON_TIMEINTER_15MIN         0x01    //  15分钟
#define CON_TIMEINTER_30MIN         0x02    //  30分钟
#define CON_TIMEINTER_60MIN         0x03    //  60分钟
#define CON_TIMEINTER_INVALID       0x0F    //  无效

#define CON_MAX_FREEZEDAYNUM            31
#define CON_MAX_DIRPOWTOTAL_COMPNUM     8   //必须小于总加组数

//模块参数
#define CON_MAX_GPRSPDPUSERNAME_LEN     32//16      changed for test 09-11-25
#define CON_MAX_GPRAPDPPASSWORE_LEN     32//16      changed for test 09-11-25

//AES加密相关
#define CON_MAX_AESCIPHER_LEN           3

#define CON_NEEDZIP                     0x00
#define CON_NOTCIPHERZIP                0x01
#define CON_NEEDCIPHER                  0x02
#define CON_NEEDCIPHERZIP               0x03
typedef struct _STRMWORKZONE    //抄表时段
{
    unsigned char usRMTStart[2];//时分
    unsigned char usRMTEnd[2];//时分
}STRMWORKZONE;

typedef struct _STCOMPORTPARA   //通信端口号
{
    unsigned short usRMRunCtrlWord;//抄表运行控制字
    unsigned long  ulRMDate;//抄表日-日期
    unsigned char  ucRMTime[2];//抄表日-时分
    unsigned char  ucRMInval;//抄表间隔时间
    unsigned char  ucRMBcastTInval[3];//对电表广播校时定时时间 日时分
    unsigned char  ucRMWorkZoneNum;
    STRMWORKZONE stRMWorkZone[24];
}STCOMPORTPARA, *PSTCOMPORTPARA;
typedef struct _STRMRUNPARA //F33 终端抄表运行参数设置
{
    unsigned char  ucComPortEBit;//存在标志位
    STCOMPORTPARA stComPortPara[CON_PORT_NUM];
}STRMRUNPARA, *PSTRMRUNPARA;

typedef struct _STPORTPROP      //端口属性
{
    unsigned char ucBaudRate;
    unsigned char ucParity;
    unsigned char ucByteSize;
    unsigned char ucStopBits;
    unsigned long ulBaudRate;
}STPORTPROP;
typedef struct _STPORTLIST      //端口列表
{
    unsigned char ucNo[CON_COMMPORT_NUM];
    STPORTPROP stPortProp[CON_COMMPORT_NUM];
    STRMRUNPARA stRMRunPara;
}STPORTLIST;

typedef struct _STSTAT32BYTE
{
    unsigned char ucState[32];
}STSTAT32BYTE;

typedef struct _STMPRATEVAL
{
    unsigned char   ucMPLineType;           //接线方式
    unsigned char   ucMPPhaseNo;
    unsigned short  usRateVol;              //额定电压  //0.1  V
    unsigned short  usRateCur;              //额定电流  //0.01 A
    unsigned long   ulRateP;                //额定负荷  //0.0001    KVA
}STMPRATEVAL;

typedef struct _STMPALTERRATE
{
    unsigned short  usVolAlterRate;         //电压回路变比
    unsigned short  usCurAlterRate;         //电流回路变比
}STMPALTERRATE;

typedef struct _STMPLIMIT                   //测量点限值
{
    unsigned short  usVolUpLimit;           //电压合格上限  整数格式XXX
    unsigned short  usVolDownLimit;         //电压合格下限  整数格式XXX
    unsigned short  usVolBreakLimit;        //断相门限      整数格式XXX
    unsigned short  usVolErrUpLimit;        //电压异常上限  整数格式XXX
    unsigned short  usVolErrDownLimit;      //电压异常下限  整数格式XXX

    unsigned short  usVolUpLimit_2;         //电压合格上限  小数格式XXX.X
    unsigned short  usVolDownLimit_2;       //电压合格下限  小数格式XXX.X
    unsigned short  usVolBreakLimit_2;      //断相门限      小数格式XXX.X
    unsigned short  usVolErrUpLimit_2;      //电压异常上限  小数格式XXX.X
    unsigned short  usVolErrDownLimit_2;    //电压异常下限  小数格式XXX.X

    unsigned short  usCurErrUpLimit;        //电流异常上限  小数格式XX.XX
    unsigned short  usCurUpLimit;           //电流合格上限  小数格式XX.XX
    unsigned short  usCur0UpLimit;          //零线电流上限  小数格式XX.XX
    unsigned long   ulPowErrUpLimit;        //功率异常上限                  .XXXX
    unsigned long   ulPowUpLimit;           //功率合格上限                  .XXXX
    unsigned short  usVolUnBalance;         //电压不平衡限值            .X
    unsigned short  usCurUnBalance;         //电流不平衡限值            .X
    unsigned short  usLostVolTime;          //连续失压时间

    unsigned char   ucVolErrUpTime;         //越限持续时间
    unsigned char   ucVolErrDownTime;       //越限持续时间
    unsigned char   ucCurErrUpTime;         //越限持续时间
    unsigned char   ucCurUpTime;            //越限持续时间
    unsigned char   ucCur0UpTime;           //越限持续时间
    unsigned char   ucPowErrUpTime;         //越限持续时间
    unsigned char   ucPowUpTime;            //越限持续时间
    unsigned char   ucVolUnBalanceTime;     //越限持续时间
    unsigned char   ucCurUnBalanceTime;     //越限持续时间

    unsigned short  ucVolErrUpBack;         //越限持续时间
    unsigned short  ucVolErrDownBack;       //越限持续时间
    unsigned short  ucCurErrUpBack;         //越限持续时间
    unsigned short  ucCurUpBack;            //越限持续时间
    unsigned short  ucCur0UpBack;           //越限持续时间
    unsigned short  ucPowErrUpBack;         //越限持续时间
    unsigned short  ucPowUpBack;            //越限持续时间
    unsigned short  ucVolUnBalanceBack;     //越限持续时间
    unsigned short  ucCurUnBalanceBack;     //越限持续时间
}STMPLIMIT;

typedef struct _STMPFREEZEPARA
{
    unsigned char   ucItem;                 //1类数据标识中F89--F120
    unsigned char   ucFreezeInter;
}STMPFREEZEPARA;

typedef struct  _STFREEZEDAY
{
    STDATETIME  stFreezeDay[CON_MAX_FREEZEDAYNUM];
}STFREEZEDAY, *PSTFREEZEDAY;

typedef struct  _STPOWRATEPARA
{
    unsigned short  usPowRateLimit1;        //功率因数分段限值1
    unsigned short  usPowRateLimit2;        //功率因数分段限值2
}STPOWRATEPARA;

typedef struct _STCUFELOSTPARA
{
    unsigned short R;           //电阻
    unsigned short X;           //电抗
    unsigned short G;           //电导
    unsigned short B;           //电纳
}STCUFELOSTPARA;
typedef struct  _STTONGSUNTIANSUNPARA
{
    STCUFELOSTPARA  stCuFeLostParaA;
    STCUFELOSTPARA  stCuFeLostParaB;
    STCUFELOSTPARA  stCuFeLostParaC;
}STTONGSUNTIANSUNPARA;

typedef struct _STMPINFO
{
    unsigned char   ucMPType;               //1
    unsigned char   ucDevNo[4];             //测量点设备编号(表号，脉冲号)，一个脉冲测量点可以有4路不同属性的脉冲构成
    STMPRATEVAL     stMPRateVal;            //3
    STMPALTERRATE   stMPAlterRate;          //4
    STMPLIMIT       stMPLimit;              //30
    STMPFREEZEPARA  stMPFreezePara[CON_MAX_MTRFREEZENUM];   //150
    STFREEZEDAY     stFreezeDay;            //186
    STPOWRATEPARA   stPowRatePara;          //4
    STTONGSUNTIANSUNPARA    stTongsunTiansun;//24
}STMPINFO;

/******************************************终端配置个数****************************************/
typedef struct  _STCONFIGNUM
{
    unsigned char   ucMeterNum;         //电能表/交流采样装置配置总块数
    unsigned char   ucPulseNum;         //脉冲配置路数
    unsigned char   ucAnalogNum;        //电压/电流模拟量配置总路数
    unsigned char   ucSumGNum;          //总加组配置数
}STCONFIGNUM;
/******************************************脉冲配置参数****************************************/
#define CON_PULSEATTR_INVALID   0x00    //无效
#define CON_PULSEATTR_DIRPOW    0x01    //正向有功
#define CON_PULSEATTR_REVPOW    0x02    //反向有功
#define CON_PULSEATTR_DIRVAR    0x03    //正向无功
#define CON_PULSEATTR_REVVAR    0x04    //反向无功

typedef struct  _STPULSEPARA
{
    unsigned char   ucAttr;             //属性
    unsigned char   ucPortNo;           //端口号
    unsigned short  usConst;            //脉冲常数
}STPULSEPARA;
/******************************************状态量配置参数****************************************/
//0~3:对应于继电器的四个遥信输入，7：门节点
#define CON_SWITCHATTR_INVALID  0x00    //无效
#define CON_SWITCHATTR_TYPEA    0x01    //－A类触点 一致
#define CON_SWITCHATTR_TYPEB    0x02    //－B类触点 相反
#define CON_SWITCHALARM_IMP     0x55    //重要
#define CON_SWITCHALARM_NOR     0xAA    //一般

typedef struct _STSWITCHPARA
{
    unsigned char   ucAttr;         //属性，无效表示不接入
    unsigned char   ucAlarm;        //变位告警是重要事件还是一般事件
}STSWITCHPARA;
typedef struct _STDT2
{
    double dVal;
    unsigned short usDT2;
}STDT2;
typedef struct _STANALOGRATE
{
    STDT2   stStart;
    STDT2   stEnd;
}STANALOGRATE;
typedef struct _STANALOGLIMIT
{
    STDT2 stUpLimit;
    STDT2   stDownLimit;
}STANALOGLIMIT;
/******************************************模拟量配置参数****************************************/
#define CON_ANALOGTYPE_VA       0x00
#define CON_ANALOGTYPE_VB       0x01
#define CON_ANALOGTYPE_VC       0x02
#define CON_ANALOGTYPE_IA       0x03
#define CON_ANALOGTYPE_IB       0x04
#define CON_ANALOGTYPE_IC       0x05

typedef struct _STANALOGPARA
{
    unsigned char ucAvailable;
    unsigned char   ucMPNo;         //测量点号
    unsigned char   ucAttr;         //属性，类型
    unsigned char ucM;
    STANALOGRATE stAnalogRate;
    STANALOGLIMIT stAnalogLimit;
}STANALOGPARA;

//中文信息
#define     CON_MESSAGE_DATALEN             16//200             //中文信息长度最大值 200
#define     CON_MESSAGE_LIFETIME            600             //5分钟
#define     CON_MESSAGE_NUM                 5               //中文信息条数，山西技术条件，序号与编号含义不同
#define     CON_MESSAGE_TYPE_IMPT           1               //重要消息
#define     CON_MESSAGE_TYPE_NORM           0               //常规消息
typedef struct _STSHORTMESSAGE
{
    unsigned char   ucAvailble;         //有效标志
    unsigned char   ucType;             //类型
    unsigned char   ucNo;               //编号
    unsigned char   ucLength;           //长度
    unsigned char   ucPos;              //当前位置
    unsigned char   ucMsgStatusFlag;    //消息状态标志
    unsigned short  usLifeTime;         //有效期，单位0.5秒
    STDATETIME      stDateTimeTemp;     //中文信息接受时间
    unsigned char   ucMsg[CON_MESSAGE_DATALEN]; //中文信息
}STSHORTMESSAGE;
/******************************************电能表参数****************************************/
//外部定义通信规约类型，编号可以根据外部要求更改，记录在DATAFLASH CON_DFPAGE_HEAD_RESERVE1位置开始80字节
#define     CON_EXT_PROTOCOL_INVALID    0       //无效
#define     CON_EXT_PROTOCOL_DL645      1       //645规约
#define     CON_EXT_PROTOCOL_SAMPLE     2       //交流采集

#define     CON_EXT_PROTOCOL_WS         3       //威胜v4.x
#define     CON_EXT_PROTOCOL_LANDD      4       //西门子D表
#define     CON_EXT_PROTOCOL_ABB_R      5       //ABB方表
//#define       CON_EXT_PROTOCOL_GENIUS     6       //红相
#define     CON_EXT_PROTOCOL_AS1_DL645  6       //645规约类似一
#define     CON_EXT_PROTOCOL_HND        7       //浩宁达
#define     CON_EXT_PROTOCOL_LANDB      8       //西门子B表

#define     CON_EXT_PROTOCOL_AS2_DL645  15      //645规约类似二(鄂规（简易多功能）)
#define     CON_EXT_PROTOCOL_HB_PT      17      //鄂规之普通电子式电能表
#define     CON_EXT_PROTOCOL_HB_JM      18      //鄂规之居民复费率电能表

#define     CON_EXT_PROTOCOL_KL         9       //科陆 (645)
#define     CON_EXT_PROTOCOL_LD         11      //龙电 (645)
#define     CON_EXT_PROTOCOL_SKD        12      //深科达 (645)
#define     CON_EXT_PROTOCOL_JSLY       13      //江苏羚羊 (645)
#define     CON_EXT_PROTOCOL_HND645     14      //浩宁达 (645)
#define     CON_EXT_PROTOCOL_DFDZ       15      //东方电子 (645)
#define     CON_EXT_PROTOCOL_JQ         16      //金雀 (645)
#define     CON_EXT_PROTOCOL_HLi        17      //浙江华立 (645)

//#define       CON_EXT_PROTOCOL_AS1_DL645  18      //645规约类似一
//#define       CON_EXT_PROTOCOL_AS2_DL645  19      //645规约类似二

#define     CON_EXT_PROTOCOL_DL64507    30


#define CON_METERTYPE_3P3L      0x01
#define CON_METERTYPE_3P4L      0x02
#define CON_METERTYPE_1P        0x03

#define     CON_METER_PHASE_INCERTERN   0
#define     CON_METER_PHASEA                1
#define     CON_METER_PHASEB                2
#define     CON_METER_PHASEC                3

#define CON_METER_ADDRLEN       6       //电表地址长度
#define CON_METER_PASSLEN       8       //密码长度
#define CON_METER_NAMELEN       8       //通迅名长度
#define CON_COLLECT_ADDR_LEN    6
typedef struct _STMETERADDR
{
    unsigned char ucMeterAddr[CON_METER_ADDRLEN];
}STMETERADDR;
typedef struct _STMETERPASS
{
    unsigned char ucMeterPass[CON_METER_PASSLEN];
}STMETERPASS;
typedef struct _STMETERCOMMNAME
{
    unsigned char ucMeterCommName[CON_METER_NAMELEN];
}STMETERCOMMNAME;
typedef struct _STMETERPARA
{
    unsigned char ucProtocol;           //规约类型
    unsigned char ucPort;               //从1开始，1、2，如果0则为1
    unsigned char ucBaudCode;
    unsigned long ulBaudRate;           //波特率
    unsigned char ucEPN;                //校验位
    unsigned char ucDataBit;            //数据位
    unsigned char ucStopBit;            //停止位
    unsigned char ucSetFeeNum;
    unsigned char ucFeeNum;             //费率个数
    unsigned char ucIntNum;             //整数位个数
    unsigned char ucFloatNum;           //小数位个数
//  unsigned char ucPulseNo[CON_PULSE_NUM]; //对应的脉冲编号
    STMETERADDR   stMeterAddr;          //电表地址
    STMETERPASS   stPassword;           //通信密码
    STMETERCOMMNAME stCommName;         //通迅名
    unsigned char ucMeterJuNum[12];     //电表局号，12字节的ASCII码
    unsigned char ucCollectAddr[CON_COLLECT_ADDR_LEN];  //采集器地址
    unsigned char ucBigClass;
    unsigned char ucLittleClass;
    unsigned char ucEmpFlag;
}STMETERPARA;

/******************************************总加组参数****************************************/
#define CON_SUMGROUP_NUM        8
#define CON_SUMGOPER_ADD        0x55
#define CON_SUMGOPER_SUB        0xAA

typedef struct _STSUMGFREEZEPARA
{
    unsigned char   ucFreezeInter;
}STSUMGFREEZEPARA;
typedef struct _STSUMVALOPER
{
    unsigned char       ucMPSum[CON_MAX_MPNUM];     // 参与计算的测量点
    unsigned char       ucMPOper[CON_MAX_MPNUM];    // 运算符
}STSUMVALOPER;
typedef struct _STSUMGLIMIT
{
    unsigned long       ulSumGUpLimit;
    unsigned long       ulSumGDownLimit;
}STSUMGLIMIT;
typedef struct _STSUMVALPARAPER
{
    STSUMVALOPER        stDir;
    STSUMVALOPER        stRev;
    STSUMGFREEZEPARA    stFreeze;           // 总加组冻结参数
    STSUMGLIMIT         stLimit;
}STSUMVALPARAPER;
typedef struct _STSUMGROUPPARA
{
    unsigned char ucAvailbleFlag;
    STSUMVALPARAPER stInstantPow;
    STSUMVALPARAPER stPowVal;
    STSUMVALPARAPER stInstantVar;
    STSUMVALPARAPER stVarVal;
}STSUMGROUPPARA;

#define CON_SUMGTYPE_INSTANTPOW_DIR     0x01
#define CON_SUMGTYPE_INSTANTPOW_REV     0x02
#define CON_SUMGTYPE_POWVAL_DIR         0x03
#define CON_SUMGTYPE_POWVAL_REV         0x04
#define CON_SUMGTYPE_INSTANTVAR_DIR     0x05
#define CON_SUMGTYPE_INSTANTVAR_REV     0x06
#define CON_SUMGTYPE_VARVAL_DIR         0x07
#define CON_SUMGTYPE_VARVAL_REV         0x08
/******************************************通信参数****************************************/
#define CON_SERVER_NULL                 0x00    //空事件
#define CON_SERVER_NUM                  8

#define CON_ONLINE_MASK                 0xb3    //模式屏蔽字
#define CON_ONLINE_TCP                  0       //通信方式
#define CON_ONLINE_UDP                  0x80

#define CON_ONLINE_TYPE_MASK            0X30    //工作方式屏蔽字
#define CON_ONLINE_MIX                  0       //工作方式
#define CON_ONLINE_CLIENT               0X10
#define CON_ONLINE_SERVER               0X20

#define CON_ONLINE_MODE_MASK            0X03    //模式屏蔽字
#define CON_ONLINE_ALLWAYS              1       //永久在线
#define CON_ONLINE_SMS_UP               2       //短信激活断续在线
#define CON_ONLINE_TIME_UP              3       //时段在线模式
#define CON_ONLINE_INVALID              0       //无效的工作模式


#define CON_ONLINE_SMS_ACTIVE           1       //短信激活
//#define CON_ONLINE_TER_ACTIVE         2       //终端主动上送

typedef struct  _STTERCOMMPARA
{
    unsigned char   ucDelay;                    //终端数传机延时时间（单位：20ms）
    unsigned char   ucMaxOvertimeOrig;          //原发站允许最大超时时间（分钟）
    unsigned char   ucReSendtimes;              //重发次数
    unsigned short  usACKOvertime;              //确认（CON=1）超时（单位：s）
    unsigned char   ucServerNeedAck[CON_SERVER_NUM];//0,1事件是否需要确认，2:一类数据3:二类数据
                                                //需要主站确认的服务，8种通信服务
}STTERCOMMPARA;

typedef struct  _STTERCOMMPARA_INNER
{
    unsigned char   ucRCommMode;                //通信模块工作模式
    unsigned short  usReConnectInter;           //永久在线掉线重连间隔，单位秒
    unsigned char   ucReConnectTimes;           //被动激活重拨次数
    unsigned char   ucStandbyTime;              //被动激活保持在线持续时间，单位分钟
    unsigned char   ucTimeInt[3];               //时段在线模式，每个BIT表示一个小时点，为1表示可以在线，为0表示不能在线
}STTERCOMMPARA_INNER;

typedef struct _STHEARTBEAT                     //心跳
{
    unsigned char   ucHeartBeat;
}STHEARTBEAT;

//主站IP地址和端口
#define CON_MAXAPNLEN                   20
typedef struct _STIPADDR
{
    unsigned char  ucIPAddr[4];         //IP地址1-4段
    unsigned short usIPPort;            //IP地址端口号
}STIPADDR;
#define CON_USRNAME_LEN     20
#define CON_PASSWORD_LEN        20
typedef struct _STAGENTSERVER
{
    unsigned char ucServeType;      //代理类型
    unsigned char ucLinkType;           //连接方式
    unsigned char ucUsrNameLen;
    unsigned char ucUsrName[CON_USRNAME_LEN];
    unsigned char ucPwdLen;
    unsigned char ucPwd[CON_PASSWORD_LEN];
    STIPADDR stServeIP;             //IP及端口
}STAGENTSERVER;
typedef struct  _STSTAIPADDR
{
    STIPADDR    stBaseIP;               //主用IP地址
    STIPADDR    stBackIP;               //备用IP地址
    char        cAPN[CON_MAXAPNLEN];    //APN
}STSTAIPADDR;

typedef struct _STLOCALIP
{
    STIPADDR    stLocalIP;
    STIPADDR    stSubMask;
    STIPADDR    stGateWay;              //网关
    STAGENTSERVER   stAgentServer;          //代理服务器
}STLOCALIP;
//主站电话号码、短信中心号码
#define  CON_PHONENOLEN                 8
typedef struct _STPHONENO
{
    unsigned char   ucPhoneNo[CON_PHONENOLEN];
}STPHONENO;
typedef struct  _STSTAPHONENO
{
    STPHONENO   stPhoneNo;          //主站电话号码
    STPHONENO   stSMSCenter;        //短信中心号码
}STSTAPHONENO;

#define CON_SERVE_TYPE_NO               0x00
#define CON_SERVE_TYPE_HTTP         0x01
#define CON_SERVE_SOCKS4                0x02
#define CON_SERVE_SOCKS5                0x03

#define CON_SERVE_NOVERIFY              0
#define CON_SERVE_VERIFY                1

//加密设置
#define CON_ENCRYPT_INVALID         0xFF        //口令代码无效
#define CON_ENCRYPT_DIR             0x00        //直接密码  后来改为不验证密码
#define CON_ENCRYPT_CRC             0x01        //CRC加密
#define CON_MAX_PASSERR_TIMES       10          //口令错误允许次数
#define CON_PASSERR_LOCK_TIME       86400       //最近一次口令错误开始的闭锁时间，单位秒

typedef struct  _STTERENCRYPT
{
    unsigned char ucAlgoCode;           //口令算法代码
    unsigned char ucPassword[4];        //口令基数
    unsigned char ucPassword_New[16];   //16字节密码
    unsigned char ucAdminPassword[4];   //告警口令基数
    unsigned char ucErrPassword[4];     //错误的口令基数
    unsigned char ucErrTimes;           //错误次数
    unsigned long ulLockTime;           //闭锁发生时间
}STTERENCRYPT;

//行政编码
#define CON_REGIONADDRLEN               2
//终端地址
#define CON_TERDEVADDRLEN               2
typedef struct _STTERREGIONADDR
{
    unsigned char ucRegionAddr[CON_REGIONADDRLEN];
}STTERREGIONADDR;
typedef struct _STTERDEVADDR
{
    unsigned char ucDevAddr[CON_TERDEVADDRLEN];
}STTERDEVADDR;

//终端组地址
#define CON_GROUPADDRLEN                2
#define CON_GROUPADDRNUM                8
typedef struct _STTERGROUPADDR
{
    unsigned char ucGroupAddr[CON_GROUPADDRLEN];
}STTERGROUPADDR;
typedef struct  _STTERGROUPADDRLIST         //终端组地址列表
{
    STTERGROUPADDR  stGroupAddr[CON_GROUPADDRNUM];
}STTERGROUPADDRLIST;

#define     CON_TERCOMMSTAT_AUTOUP          0x55
#define     CON_TERCOMMSTAT_AUTOUP_NO       0xAA
#define     CON_TERCOMMSTAT_TALK            0x55
#define     CON_TERCOMMSTAT_TALK_NO         0xAA

typedef struct _STTERCOMMSTAT
{
    unsigned char   ucTerAutoUpFlag;
    unsigned char   ucTerTalkFlag;
}STTERCOMMSTAT;
/******************************************告警参数****************************************/
//事件告警设置
#define     CON_ALR_SHIELD          0x55
#define     CON_ALR_SHIELD_NOT      0xAA
#define     CON_ALR_ACD             0x55
#define     CON_ALR_ACD_NOT         0xAA
#define     CON_ALR_HAPPEN          0x55
#define     CON_ALR_HAPPEN_NOT      0xAA

#define         CON_ALR_SHIELDFLAG      0xAA
#define         CON_ALR_ACDFLAG         0x55

typedef struct _STALRCATEGORY
{
    unsigned char   ucTerDataInit;          //数据初始化
    unsigned char   ucTerParaLost;          //参数丢失
    unsigned char   ucTerParaChange;        //重要参数变化

    unsigned char   ucStatChange;           //状态量变化
    unsigned char   ucRemoteCtrl;           //遥控
    unsigned char   ucPowerCtrl;            //功控
    unsigned char   ucEnergyCtrl;           //电控

    unsigned char   ucMeterParaChange;      //表计参数变化
    unsigned char   ucTAErr;                //TA异常
    unsigned char   ucTVErr;                //TV异常
    unsigned char   ucPhaseErr;             //相位异常

    unsigned char   ucMeterTimeErr;         //表计时钟异常
    unsigned char   ucMeterErrInfo;         //表计故障信息

    unsigned char   ucTerPowOff;            //终端停电
    unsigned char   ucHarmWaveOver;         //谐波越限
    unsigned char   ucDirSimuValOver;       //直流模拟量越限
    unsigned char   ucUnbalanceOver;        //不平衡度越限

    unsigned char   ucCapLockSelf;          //电容器投切自锁
    unsigned char   ucBuyPowPara;           //购电参数设置
    unsigned char   ucPassErr;              //密码错误
    unsigned char   ucTerErr;               //终端故障

    unsigned char   ucVolOverUp;            //电压越限
    unsigned char   ucCurOverUp;            //电流越限
    unsigned char   ucSPowOverUp;           //视在功率越限
    unsigned char   ucSPowErrUp;            //视在功率越上上限

    unsigned char   ucMeterDown;            //表计示度下降
    unsigned char   ucPowDiffOver;          //电能量超差
    unsigned char   ucMeterFly;             //表计飞走
    unsigned char   ucMeterStop;            //表计停走

    unsigned char   ucEnerAlr;              //ERC23 电控告警事件
    unsigned char   ucMeterCopy;            //ERC31 485抄表失败事件记录
    unsigned char   ucFluxLimit;            //ERC32 日通信超流量记录

    unsigned char   ucMeterState;           //ERC33 电表状态字变位

//    unsigned char   ucSumInstantPowOver[CON_SUMGROUP_NUM];  //有功功率总加组越限
//    unsigned char   ucSumInstantVarOver[CON_SUMGROUP_NUM];  //无功功率总加组越限
//    unsigned char   ucSumPowValOver[CON_SUMGROUP_NUM];      //有功电量总加组越限
//    unsigned char   ucSumVarValOver[CON_SUMGROUP_NUM];      //无功电量总加组越限
//    unsigned char   ucSumCompOverLimit;                     //差动越限
}STALRCATEGORY;

typedef struct _STTERALARMJUDGECOUNT
{
    unsigned char ucAlrCount_PolarityReverseA;  //A相电流反极性
    unsigned char ucAlrCount_PolarityReverseB;  //B相电流反极性
    unsigned char ucAlrCount_PolarityReverseC;  //C相电流反极性

//  unsigned char ucAlrCount_ShortCircuitA1;    //A相1次侧CT短路
//  unsigned char ucAlrCount_ShortCircuitB1;    //B相1次侧CT短路
//  unsigned char ucAlrCount_ShortCircuitC1;    //C相1次侧CT短路
//
//  unsigned char ucAlrCount_ShortCircuitA2;    //A相2次侧CT短路
//  unsigned char ucAlrCount_ShortCircuitB2;    //B相2次侧CT短路
//  unsigned char ucAlrCount_ShortCircuitC2;    //C相2次侧CT短路
//
//  unsigned char ucAlrCount_OpenCircuitA2;     //A相2次侧CT开路
//  unsigned char ucAlrCount_OpenCircuitB2;     //B相2次侧CT开路
//  unsigned char ucAlrCount_OpenCircuitC2;     //C相2次侧CT开路

    unsigned char ucAlrCount_VolABreak;         //A相电压断相
    unsigned char ucAlrCount_VolBBreak;         //B相电压断相
    unsigned char ucAlrCount_VolCBreak;         //C相电压断相

    unsigned char ucAlrCount_VolALack;          //A相电压缺相
    unsigned char ucAlrCount_VolBLack;          //B相电压缺相
    unsigned char ucAlrCount_VolCLack;          //C相电压缺相

    unsigned char ucAlrCount_VolCirReverse;     //电压回路逆相序

    unsigned char ucAlrCount_IUnBalance;        //电流不平衡度越限
    unsigned char ucAlrCount_UUnBalance;        //电压不平衡度越限

    unsigned char ucAlrCount_VolAErrDown;       //A相电压越下下限
    unsigned char ucAlrCount_VolBErrDown;       //B相电压越下下限
    unsigned char ucAlrCount_VolCErrDown;       //C相电压越下下限

    unsigned char ucAlrCount_VolAErrUp;         //A相电压越上上限
    unsigned char ucAlrCount_VolBErrUp;         //B相电压越上上限
    unsigned char ucAlrCount_VolCErrUp;         //C相电压越上上限

    unsigned char ucAlrCount_CurAErrUp;         //A相电流越上上限
    unsigned char ucAlrCount_CurBErrUp;         //B相电流越上上限
    unsigned char ucAlrCount_CurCErrUp;         //C相电流越上上限

    unsigned char ucAlrCount_CurANorUp;         //A相电流越上限
    unsigned char ucAlrCount_CurBNorUp;         //B相电流越上限
    unsigned char ucAlrCount_CurCNorUp;         //C相电流越上限

    unsigned char ucAlrCount_SPowNorUp;         //视在功率越上限
    unsigned char ucAlrCount_SPowErrUp;         //视在功率越上上限
}STTERALARMJUDGECOUNT;

typedef struct  _STALRCTRLWORD
{
    STALRCATEGORY   stAlrShield;        //告警屏蔽字，0:做事件记录，1不做事件记录
    STALRCATEGORY   stAlrACD;           //重要事件标记
}STALRCTRLWORD;         //112 byte
/*************************************控制定值******************************************/
#define CONSWITCHPERTIMENUM     16      //时段数
#define CONSWITCHCURVENUM       8       //8条功率定值曲线
#define CONSWITCHCURVE_RESERVE0     0   //0备用1    高优先级
#define CONSWITCHCURVE_POWLOWER     1   //1当前功率下浮控
#define CONSWITCHCURVE_SALESTOP     2   //2营业报停控制
#define CONSWITCHCURVE_RESTCTRL     3   //3厂休控
#define CONSWITCHCURVE_TIMEPER1     4   //4时段控制1
#define CONSWITCHCURVE_TIMEPER2     5   //5时段控制2
#define CONSWITCHCURVE_TIMEPER3     6   //6时段控制3
#define CONSWITCHCURVE_RESERVE7     7   //7备用2    低优先级

#define CON_POWER_BIGVAL        999999

#define CON_AVALIBLE_FLAG       0x55
#define CON_INVALIBLE_FLAG      0xAA

#define CON_ENERGYCTRL_MON      0x11    //月电量
#define CON_ENERGYCTRL_BUY      0x22    //购电量
#define CON_ENERGYCTRL_INVALID  0xBB    //无效

#define CON_ENERGYFLASH_ADD     0x55    //追加
#define CON_ENERGYFLASH_NEW     0xAA    //新增

#define CON_CTRLFEE_NUM         48
#define CON_CTRLFEETIME_NUM     48      //30分钟一段，48段

#define CON_CYCLETYPE_DAY       0x11
#define CON_CYCLETYPE_WEEK      0x22
#define CON_CYCLETYPE_MONTH     0x33

#define CONCYCLEDAYNUM          31

#define CON_BUYBILL_NUM         1       //保留历史购电单号1次
//#define CON_BUYBILL_NUM           10      //保留历史购电单号10次

typedef struct _STLOADFIXVAL
{
    double          dFixVal;            //定值
    unsigned short  usFloatCoef;        //浮动系数
    unsigned short  usDT2;              //格式数据
}STLOADFIXVAL;

typedef struct _STPOWERCTRLPERTIME      //功率控制时段信息
{
    unsigned char ucAvailbleFlag;       //有效标志
    unsigned char ucCtrlFlag;           //是否控制标志
    unsigned char ucOnFlag;             //投入标志
    unsigned char ucBeginHour;          //时段开始时间,时
    unsigned char ucBeginMinute;        //时段开始时间,分
    STLOADFIXVAL  stLoadFixVal;         //功率定值（一次侧，单位kW）
}STPOWERCTRLPERTIME;

typedef struct _STPOWERCTRLPROJ
{
    unsigned char   ucProjAvailbleFlag;
    unsigned char   ucCycleType;        //日控制、周控制、月控制等
    unsigned short  usCycleNum;         //几个周期后结束
    unsigned char   ucAvailbleFlag[CONCYCLEDAYNUM];
    STDATETIME  stStartTime;            //计划起开始时间
    STDATETIME  stEndTime;              //计划结束时间
}STPOWERCTRLPROJ;

typedef struct _STPOWERCTRLTIME         //功率控制时段数
{
    unsigned char       ucAvailbleFlag; //投入标志
    STPOWERCTRLPERTIME  stPowerCtrlPer[CONSWITCHPERTIMENUM];
    unsigned char       ucTimerPerFlag[12];
}STPOWERCTRLTIME;

// typedef struct _STPOWERCTRLHOLDTIME
// {
//  unsigned short      usJumpTime[CON_PARA_SWITCHOUT_NUM];         //超过设定值跳闸的过负荷持续时间
//  unsigned short      usRevTime[CON_PARA_SWITCHOUT_NUM];          //恢复持续时间
//  unsigned short      usJumpAlrTime[CON_PARA_SWITCHOUT_NUM];      //拉闸告警时间
//  unsigned short      usRevAlrTime[CON_PARA_SWITCHOUT_NUM];       //恢复告警时间
// }STPOWERCTRLHOLDTIME;

// typedef struct _STCTRLTURN
// {
//  unsigned char       ucAvailbleFlag[CON_PARA_SWITCHOUT_NUM];
// }STCTRLTURN;

typedef struct _STENERGYCTRL
{
    unsigned char       ucEnergyCtrlType;   //当前电控类型
    unsigned char       ucTurnFlag;         //轮次标志
    unsigned char       ucAlrStatus;        //告警状态
    unsigned char       ucReserve;
    STLOADFIXVAL        stAlrFixVal;        //告警阀值
    STLOADFIXVAL        stJumpFixVal;       //跳闸阀值
    STLOADFIXVAL        stMonthEnergy;      //月电量
    STLOADFIXVAL        stBuyEnergy;        //购买电量
    STLOADFIXVAL        stTurnFixVal;       //轮次电量
    STLOADFIXVAL        stTurnAlrFixVal;    //轮次告警阀值
    STLOADFIXVAL        stTurnJumpFixVal;   //轮次跳闸阀值
}STENERGYCTRL;

typedef struct _STMONENERGYREC              //月电量记录
{
    STLOADFIXVAL        stMonFixVal;
}STMONENERGYREC;

typedef struct _STBUYENERGYREC              //最近一次购电量记录
{
    unsigned long       ulBuyBillNo;        //购电单号
    unsigned char       ucFlashFlag;        //刷新、追加标记
    STLOADFIXVAL        stBuyFixVal;        //本次购电量
    STLOADFIXVAL        stAlrFixVal;        //告警阀值
    STLOADFIXVAL        stJumpFixVal;       //跳闸阀值
    STLOADFIXVAL        stTurnFixVal;       //轮次电量
    double              dLeftVal_befor;     //购电前剩余电量
    double              dLeftVal_after;     //购电后剩余电量
}STBUYENERGYREC;

typedef struct _STCTRLFEERATE
{
    unsigned char ucFeeNum;                     //费率表数
    unsigned char ucUnitFlag;
    unsigned long ulFeeRate[CON_CTRLFEE_NUM];   //费率值,单位厘
}STCTRLFEERATE;

typedef struct _STCTRLFEERATETIME
{
    unsigned char ucFeeNum;
    unsigned char ucFeeRateNo[CON_CTRLFEETIME_NUM]; //费率时段情况
}STCTRLFEERATETIME;

//typedef struct _STCTRLSUMGROUPPARA    //总加组控制参数
//{
//  STPOWERCTRLPROJ     stPowerCtrlProj[CONSWITCHCURVENUM]; //功率控制方案
//  STPOWERCTRLTIME     stPowerCtrl[CONSWITCHCURVENUM];     //功率定值曲线
//  STPOWERCTRLHOLDTIME stPowerHoldTime;                    //持续时间
//  STCTRLTURN          stPowerCtrlTurn;                    //功控有效标记
//  STENERGYCTRL        stEnergyCtrl;                       //电控当前参数
//  STMONENERGYREC      stMonEnergyRec;                     //月电量记录
//  STBUYENERGYREC      stBuyEnergyRec;                     //购电量记录
//  STCTRLTURN          stEnergyCtrlTurn;                   //电控有效标记
//}STCTRLSUMGROUPPARA;

typedef struct _STBUYBILLREC
{
    unsigned long       ulBuyBillNo;        //历史购电单号
    unsigned char       ucFlashFlag;
}STBUYBILLREC;

// typedef struct _STSWITCHCTRLPARA
// {
//  unsigned char       ucIsCheckGprs;  //GPRS未登录保电
//  unsigned char       ucGprsKeepPow;  //GPRS未登录保电
//  unsigned char       ucKeepPower;            //保电标记  CON_AVALIBLE_FLAG/CON_INVALIBLE_FLAG
//  unsigned char       ucSwitchOut;            //开关是否输出，总闭锁所有开关
//  STLOADFIXVAL        stSafeFixVal;           //保安定值
//  STCTRLSUMGROUPPARA  stCtrlSumGroupPara[CON_SUMGROUP_NUM];   //控制总加组参数
//  STCTRLFEERATE       stCtrlFeeRate;          //费率
//  STCTRLFEERATETIME   stCtrlFeeRateTime;      //费率时段信息
//  STBUYBILLREC        stBuyBillRec[CON_BUYBILL_NUM];
// }STSWITCHCTRLPARA;

typedef struct _STLOADCTRLWORDPARA              //负荷控制字
{
    unsigned char   ucTimePerCtrl[CON_SUMGROUP_NUM];
    unsigned char   ucRestCtrl[CON_SUMGROUP_NUM];
    unsigned char   ucSaleStopCtrl[CON_SUMGROUP_NUM];
    unsigned char   ucPowerLowerCtrl[CON_SUMGROUP_NUM];
    unsigned char   ucMonthEnerCtrl[CON_SUMGROUP_NUM];
    unsigned char   ucBuyEnerCtrl[CON_SUMGROUP_NUM];
    unsigned char   ucUrgeFeeCtrl;
}STLOADCTRLWORDPARA;

/******************************************辅助参数****************************************/
typedef struct  _STTERAUXPARA
{
    unsigned char ucManufactor[4];              //厂商代号  ASCII   4
    unsigned char ucDevNo[8];                   //设备编号  ASCII   8
    unsigned char ucSoftwareVer[4];             //终端软件版本号    ASCII   4
    unsigned char ucSoftwareDate[3];            //终端软件发布日期：日月年  数据格式20  3
    unsigned char ucConfigInfo[11];             //终端配置容量信息码    ASCII   11
    unsigned char ucSoftwareVerOld[4];          //终端软件版本号    ASCII   4
    unsigned char ucUserDefPassword[4];         //用户自定义口令    HEX
    unsigned char ucProtoolVer[4];              //终端通信规约版本号 ASCII 4
    unsigned char ucHardwareVer[4];             //终端硬件版本号    ASCII  4
    unsigned char ucHardwareDate[3];            //终端硬件发布日期：日月年  数据格式20  3
    unsigned char ucLoginPass[2];
}STTERAUXPARA;          //30 byte

#define CON_TER_MANUFACTOR      0x99            //生产厂商 1字节
//#define   CON_FRM_DP_START        0xD1            //启动报文，启动应答报文    C1H
//#define   CON_FRM_DP_DATA         0xD5            //数据报文  C5H
//#define   CON_FRM_DP_QUERY        0xD7            //查询报文，查询应答报文    C7H
//#define   CON_FRM_DP_SET          0xD8            //设置报文，设置应答报文    C8H
//#define   CON_FRM_DP_SET_TEST     0xD9            //设置报文，设置应答报文    C8H
//#define   CON_FRM_DP_END          0xDA            //结束报文，结束应答报文    CAH
//#define   CON_FRM_DP_EXECUTE      0xDC            //下装报文，下装应答报文    CCH
//#define   CON_FRM_DP_FINISHED     0xDD            //下装完成报文  CDH

#define CON_FRM_DP_QUERY_VER    0xF000          //查询版本号
#define CON_FRM_DP_DATA         0xF001          //数据报文
#define CON_FRM_DP_EXECUTE      0xF002          //切换执行命令

#define CON_FRM_DP_TYPE_PROG    0xDD            //查询程序数据
#define CON_FRM_DP_TYPE_PARA    0x11            //查询参数数据
#define CON_FRM_DP_TYPE_PGPA    0xFF            //查询程序数据+参数数据
#define CON_FRM_DP_TYPE_MODULE  0x22            //通讯模块程序下装


#define CON_DOWNLOADBITMAP_SIZE 256             //下装块图
#define CON_TERDP_STATE_DOWNLOAD    0xAA        //程序下装状态
#define CON_TERDP_STATE_NORMAL      0x55        //正常运行状态

#define CON_TERDP_SOURCE_RS232      0x11        //程序下装，串口传数据
#define CON_TERDP_SOURCE_REMOTE     0x22        //程序下装，远方传数据
#define CON_TERDP_SOURCE_INVALID    0xFF        //程序下装，无效来源
/******************************************数据任务上送参数****************************************/
//#define       CONDATA1JOBNUM      64              //1类数据任务个数
#define     CONDATAJOBNUM       16//64              //1数据任务个数,修改该值时请修改var_define_x.h里eeprom的任务个数，目前是16
#define     CONDATAUNITNUM      16//18//    4       //任务所带数据单元个数，EEPROM里存的最多是32个

#define     CON_JOB_ACTIVE      0x55
#define     CON_JOB_NOACTIVE    0xAA

#define     CON_JOB_DATA1       0x11            //任务类型1类数据
#define     CON_JOB_DATA2       0x22            //任务类型2类数据

typedef struct _STDATAUNITID
{
    unsigned char ucDA1;
    unsigned char ucDA2;
    unsigned char ucDT1;
    unsigned char ucDT2;
}STDATAUNITID;

typedef struct _STJOBPARA
{
    unsigned char ucActive;
    unsigned char ucTimeInter;              //时间间隔
    unsigned char ucTimeUnit;               //时间单位
    STDATETIME    stStartActive;            //发送基准时间
    STDATETIME    stNextExecute;            //下次执行时间
    unsigned char ucResendDataInter;        //曲线抽取数据倍率
    unsigned char ucDUIDNum;                //数据单元个数
    STDATAUNITID  stDUID[CONDATAUNITNUM];   //数据单元
}STJOBPARA;
#define CON_LITUSR_NUM      8//16//azh
#define CON_UPUSR_NUM       4//16
typedef struct _STLITTYPE
{
    unsigned char ucFnGroupNum;
    unsigned char ucBitFlag[32];
}STLITTYPE;
typedef struct _STLITUSR
{
    STLITTYPE stLitType[CON_LITUSR_NUM];
}STUPUSR;
typedef struct _STJOBFNPARA
{
    STUPUSR     stUpUsr[CON_UPUSR_NUM];
}STJOBFNPARA;

/******************************************终端一般参数****************************************/
#define     CON_POWERCTRL_TIMEREC_END       0xFF    //大于240表示按照时段结束
#define     CON_POWERCTRL_TIMEREC_MAXMIN    240     //惩罚性停电从0分钟到240分钟，大于240表示按照时段结束

typedef struct _STTERNORMALPARA
{
    unsigned char ucCopyInter;                      //终端抄表间隔时间，单位：分钟
    unsigned char ucReserve1;
    unsigned char ucCopyFlag;
    unsigned char ucSamplePort;                     //采集端口
    unsigned char ucDataTypeInfo;                   //数据格式信息
    unsigned char ucCommAddr[6];                    //通讯地址，暂无用
    unsigned char ucCommPass[6];                    //通讯密码，暂无用
}STTERNORMALPARA;

/******************************************终端杂项参数****************************************/
typedef struct _STPOWDIFFLIMIT
{
    unsigned char ucSumGNo;                         //有功总电能量差动组号
    unsigned char ucSumGNoComp;                     //对比组号
    unsigned char ucSumGNoReff;                     //参考
    unsigned char ucFlag;
    unsigned char ucDiffLimit;
    double        dDiffLimitVal;
    unsigned char ucSign;
}STPOWDIFFLIMIT;

typedef struct _STGPRSPDPPARA
{
    unsigned char ucPDPUseFlag;                     //启用PDP密码标志
    unsigned char ucPDPUserName[CON_MAX_GPRSPDPUSERNAME_LEN];
    unsigned char ucPDPPassWord[CON_MAX_GPRAPDPPASSWORE_LEN];
}STGPRSPDPPARA;
typedef struct _STGPRSRELINKTIME
{
    unsigned short usRelinkTime;
}STGPRSRELINKTIME;
typedef struct _STRCOMMPARA
{
    STGPRSPDPPARA       stGprsPDPPara;
    STGPRSRELINKTIME    stGprsRelinkTime;
}STRCOMMPARA;

#define CON_SLAVE_NUM           3
#define CON_TRANS_ADDR_NUM          3

#define CON_LINK_SALVE          1
#define CON_LINK_MASTER     0
#define CON_LINK_INVALID        0xFF
typedef struct _STSLAVEADDR
{
    STTERREGIONADDR     stTerRegionAddr;            //行政编码
    STTERDEVADDR        stTerAddr;                  //终端地址
}STSLAVEADDR;
typedef struct _STLINKREMOVE
{
    unsigned char ucFlag;
    unsigned char ucTransNum;
    STTERDEVADDR stTransAddr[CON_TRANS_ADDR_NUM];
}STLINKREMOVE;
typedef struct _STLINKPARA
{
    unsigned char ucPortNo;
    STPORTPROP stLinkPortPara;
    unsigned char ucRecvOverTime;
    unsigned char ucByteOverTime;
    unsigned char ucResendTime;
    unsigned char ucInterTime;
    unsigned char ucSlaveFlag;
    unsigned char ucSlaveNum;
    STLINKREMOVE    stLinkRemove;
    STSLAVEADDR stSlaveAddr[CON_SLAVE_NUM];
}STLINKPARA;

typedef struct _STTERMISCPARA
{

    unsigned char ucAlarmSoundTime[3];                  //相应时间段
    unsigned char ucKeepAuto;                           //自动保电

    unsigned char ucMeterOverDiff;                      //电能表超差阈值
    unsigned char ucMeterFly;
    unsigned char ucMeterStop;
    unsigned char ucMeterTimeDiff;                      //电能表校时阈值

//azh   STPOWDIFFLIMIT stPowDiffLimit[CON_MAX_DIRPOWTOTAL_COMPNUM];             //有功总电能量差动越限事件参数设置

    unsigned char ucUrgeFeeTime[3];                 //相应时间段
    unsigned char ucFeeNum;

    unsigned char ucFeePara[24];
    unsigned char ucPowCtrlCoef;
    unsigned char ucEnerCtrlCoef;
    unsigned char ucCtrlTime[12];

    unsigned char ucTalkOn;                             //允许终端与主站通话标志
    unsigned char ucRemoveOn;                           //终端剔除投入
    unsigned char ucUpActiveOn;                         //允许终端主动上报
    unsigned char ucTerAddIsDecType;                    //终端地址显示10进制

    unsigned long ulBoadrate[MAX_PORT_NUM];             //波特率
    unsigned char ucPinOld[4];                          //SIM的PIN
    unsigned char ucPinNew[4];                          //SIM的新PIN，待验证
    unsigned char ucPuk[8];                             //SIM的PUK
    unsigned short usUsePin;                            //SIM的PIN启用

    unsigned char ucIPInfoType[2];                      //通讯通道类型
    unsigned char ucPowerCtrlTimeRecMinute;             //功率控制恢复时间长度
    unsigned char ucProgramKeyIsSet;                    //终端编程按键已经按下

    unsigned char ucRcommBaseType;                      //通讯通道类型，短信方式还是IP通讯方式
    unsigned char ucShowRCommDebug;                     //显示通讯模块调试信息
    unsigned long ulFluxLimit;                          //日通讯流量门限

    STRCOMMPARA   stRCommPara;                          //模块参数
    STREMOTERUNINFO stRCommRunInfo;                     //模块运行信息
}STTERMISCPARA;
//总字节数已212，在加变量时要注意堆栈异出的可能，是否可用全局变量

typedef struct _STTERADJPARA
{
    unsigned char ucPowerOnCtrlLockLeftTime;            //控制上电闭锁时间
    unsigned char ucSumGFromZero;                       //总加组从0开始还是从1开始
    unsigned char ucEnerBuyUnit;                        //购电量控制或者购电费控制单位元/kwh
    unsigned char ucDownProgramSource;                  //程序下装类型，需要相应停止一些功能

//  unsigned char ucDPDataTranErrNeedEcho;              //程序下装数据传输出错需要回应
    unsigned char ucJob1HourDataExt;                    //1类任务的小时冻结数据扩展
    unsigned char ucMPNoFromZero;                       //告警测量点从0开始还是从1开始
    unsigned char ucLoginNeedAck;                       //登录55需要确认，默认0不需要确认，不初始化
    unsigned char ucViewData_1;                         //显示1次侧数据，默认0显示2次侧，1为显示1次侧，其他显示2次侧

    unsigned char ucBeatHeartNeedAck;                   //心跳55需要确认，默认0不需要确认，不初始化
    unsigned char ucPulseDemandCyc;                     //脉冲需量计算周期，单位分钟，1-15分钟
    unsigned char ucMeterFEActive;                      //抄表增加前导字符启用标志
    unsigned char ucMeterFENum;                         //抄表增加前导字符FE的个数

    unsigned char ucRunMode;                            //工作模式：测试，运行
    unsigned char ucEnerBuyIsNotFee;                    //购电量控标志，55购电量，其他购电费
}STTERADJPARA;


///////////////////////////CT检测参数////////////////////////////////
typedef struct _STCTVALUE
{
    unsigned char   ucValueA;
    unsigned char   ucValueB;
    unsigned char   ucValueC;
}STCTVALUE;
typedef struct _STCTLIMITVALUE
{
    STCTVALUE stShortCircuit1;
    STCTVALUE stOpenCircuit2;
}STCTLIMITVALUE;
typedef struct _STCTCHECKPARA
{
    STCTLIMITVALUE stCTLimitValue;
    unsigned char  ucCTType;
    unsigned char  ucTerWorkMode;
    unsigned char  ucSlfStudyFlag;
}STCTCHECKPARA;
typedef struct _STCURALRCTRLTIME
{
    unsigned char ucFirstALRTime;
    unsigned char ucSecondALRTime;
    unsigned char ucThirdALRTime;
    unsigned char ucFourthALRTime;
    unsigned char ucCurCTLTime;
}STCURALRCTRLTIME;
typedef struct _STALRJUDGETIME
{
    unsigned char ucHappedTime;//事件发生判断时间（Bin）分钟
    unsigned char ucResumeTime;//事件恢复判断时间（Bin）分钟
    unsigned char ucMpNo;       //飞走、停走事件比较测量点（bin）
}STALRJUDGETIME;
typedef struct _STHARMVALUPPARA
{
    unsigned short usHarmValUpTotal;
    unsigned short usHarmValUpOdd;
    unsigned short usHarmValUpEven;
    unsigned short usHarmValUpLimit2;
    unsigned short usHarmValUpLimit4;
    unsigned short usHarmValUpLimit6;
    unsigned short usHarmValUpLimit8;
    unsigned short usHarmValUpLimit10;
    unsigned short usHarmValUpLimit12;
    unsigned short usHarmValUpLimit14;
    unsigned short usHarmValUpLimit16;
    unsigned short usHarmValUpLimit18;
    unsigned short usHarmValUpLimit3;
    unsigned short usHarmValUpLimit5;
    unsigned short usHarmValUpLimit7;
    unsigned short usHarmValUpLimit9;
    unsigned short usHarmValUpLimit11;
    unsigned short usHarmValUpLimit13;
    unsigned short usHarmValUpLimit15;
    unsigned short usHarmValUpLimit17;
    unsigned short usHarmValUpLimit19;
}STHARMVALUPPARA;
typedef struct _STHARMVALUPLIMIT
{
    STHARMVALUPPARA stVolRateLimit;
    STHARMVALUPPARA stCurRmsLimit;
}STHARMVALUPLIMIT;
typedef struct _AESCIPHER
{
    unsigned char ucAesLength;
    unsigned char ucAesKey[CON_MAX_AESCIPHER_LEN];
}STAESKEY;

/******************************************终端参数****************************************/
typedef struct  _STTERRUNPARA
{
    STTERCOMMPARA       stTerCommPara;              //F1：终端上行通信口通信参数设置
    STHEARTBEAT         stHeartBeat;                //F1:心跳
    STSTAIPADDR         stStaIPAddr;                //F3主站地址
    STSTAPHONENO        stStaPhoneNo;               //F4：主站电话号码和短信中心号码
    STTERENCRYPT        stTerEncrypt;               //口令算法
    STLOCALIP           stLocalIP;                  //F7：终端IP地址和端口
    STTERGROUPADDRLIST  stTerGroupAddrList;         //F6：终端组地址设置
    STTERCOMMSTAT       stTerCommStat;              //F5：终端上行通信消息认证参数设置
    STALRCTRLWORD       stAlrCtrlWord;              //F9：终端事件记录配置设置
    STTERALARMJUDGECOUNT    stTerAlrMaxCount;       //
    STMPINFO            stMPInfo[CON_MAX_MPNUM];    //
    STCONFIGNUM         stConfigNum;                //F10：终端电能表/交流采样装置配置参数
//  STPULSEPARA         stPulsePara[CON_PULSE_NUM]; //
    STMETERPARA         stMeterPara[CON_METER_NUM]; //
//  STSUMGROUPPARA      stSumGroupPara[CON_SUMGROUP_NUM];   //
//  STSWITCHPARA        stSwitchPara[CON_SWITCHIN_NUM];     //
//  STANALOGPARA        stAnalogPara[CON_ANALOG_NUM];       //
//  STSWITCHCTRLPARA    stSwitchCtrlPara;
    STLINKPARA          stLinkPara;

    STTERAUXPARA        stTerAuxPara;
    STTERREGIONADDR     stTerRegionAddr;            //行政编码
    STTERDEVADDR        stTerAddr;                  //终端地址
//  unsigned char       ucTer_Meter_Addr[12];           //终端当地电能表显示号

    STJOBPARA           stJobPara_Data1[CONDATAJOBNUM];     //1类数据任务
    STJOBPARA           stJobPara_Data2[CONDATAJOBNUM];     //2类数据任务
    STJOBFNPARA         stJobFnPara1;                           //用户大类号及小类号对应的一类数据
    STJOBFNPARA         stJobFnPara2;                           //用户大类号及小类号对应的二类数据
//  STLCDSHOWPARA       stLCDShowPara;              //液晶显示参数
    STTERNORMALPARA     stTerNormalPara;

    STTERMISCPARA       stTerMiscPara;              //杂项参数
    STTERADJPARA        stTerAdjPara;               //终端调节参数
//  STCTCHECKPARA       stCTCheckPara;              //CT检测

//  STLOADCTRLWORDPARA  stLoadCtrlWordPara;         //负荷控制字
    STCURALRCTRLTIME    stCurAlrCtrlTime;
    STALRJUDGETIME      stAlarmJudgeTime;       //缺相、断相等事件判断延时、飞走、停走事件比较测量点
    STHARMVALUPLIMIT    stHarmValUpLimit;
    STAESKEY            stAesKey;                   //AES密钥
    STTERCOMMPARA_INNER     stTerCommPara_Inner;
    STPORTLIST      stPortList;
//azh 170919
	unsigned char       ucWireless[2];              //F90：无线通信参数
    unsigned char       ucLocation[10];             //F91：终端地理位置
	unsigned char       ucSnstate[2];               //F150：测量点状态 //只支持8个测量点，而且不保存
//azh 170920	
	unsigned char       ucSetMeterTimeOn;               //F30：终端对电能表对时功能的开启与关闭
	unsigned char       ucMeterSelfOn;               //F149：电能表通信参数自动维护功能开启与关闭
	
    unsigned char       ucClrFlg[10];               //标志放后面，参数变化就得重新初始化
}STTERRUNPARA;


typedef struct  _STTERRUNPARA_NOUSE
{
    unsigned char ucF73[48];
    unsigned char ucF74[10];
    unsigned char ucF75[16];
    unsigned char ucF76;
//  unsigned char ucF81[4];
//  unsigned char ucF82[4];
//  unsigned char ucF83;
}STTERRUNPARA_NOUSE;

/******************************************终端参数初始化****************************************/
__EXTERN unsigned char RunPara_Init_All(unsigned char ucClrFlag);
//__EXTERN unsigned char RunPara_Valid_Check(void);

// __EXTERN U8  RunPara_InitTerCommPara(STTERCOMMPARA *pstTerCommpara);
// __EXTERN U8  RunPara_InitHeartBeat(STHEARTBEAT *pstHeartBeat);
__EXTERN U8 RunPara_InitTerCommPara(U8 ucClrFlag);
//__EXTERN U8  RunPara_InitStaIPAddr(STSTAIPADDR *pstStaIPAddr);
__EXTERN U8 RunPara_InitStaIPAddr(U8 ucClrFlag);
__EXTERN U8  RunPara_InitStaPhoneNo(STSTAPHONENO *pstStaPhoneNo);
__EXTERN U8  RunPara_InitLocalIP(STLOCALIP*pstLocalIP);
__EXTERN U8  RunPara_InitTerEncrypt(STTERENCRYPT *pstTerEncrypt);
__EXTERN U8  RunPara_InitTerGroupAddrList(STTERGROUPADDRLIST *pstTerGroupAddrList);
__EXTERN U8  RunPara_InitCommStat(STTERCOMMSTAT *pstTerCommStat);
__EXTERN U8  RunPara_InitAlrCtrlWord(STALRCTRLWORD *pstAlrCtrlWord);
//__EXTERN U8    RunPara_initAlrMaxCount(STTERALARMJUDGECOUNT *pstTerAlrMaxCount);

__EXTERN void   RunPara_InitMPRateVal(STMPRATEVAL *pstMPRateVal);
__EXTERN void   RunPara_InitMPAlterRate(STMPALTERRATE *pstMPAlterRate);
__EXTERN void   RunPara_InitMPLimit(STMPLIMIT *pstMPLimit);
__EXTERN void   RunPara_InitMPFreezePara(STMPFREEZEPARA *pstMPFreezePara);
__EXTERN void   RunPara_InitMPFreezeDay(STFREEZEDAY *pstFreezeDay);
__EXTERN void   RunPara_InitMPPowRatePara(STPOWRATEPARA *pstPowRatePara);
__EXTERN void   RunPara_InitMPTongsun(STTONGSUNTIANSUNPARA *pstTongsun);
__EXTERN unsigned char  RunPara_InitMPInfo(STMPINFO *pstMPInfo);
//__EXTERN unsigned char  RunPara_SetTerVolLackAlrMaxCount(unsigned char ucCount);
__EXTERN unsigned short RunPara_GetMPLimit_usLostVolTime(unsigned char ucMPNo);

__EXTERN unsigned char  RunPara_InitConfigNum(STCONFIGNUM *pstConfigNum);
__EXTERN unsigned char RunPara_InitCuFeLostPara(STCUFELOSTPARA *pstCuFeLostPara);
//__EXTERN unsigned char    RunPara_InitPulsePara(STPULSEPARA *pstPulsePara);
__EXTERN unsigned char  RunPara_InitMeterPara(STMETERPARA *pstMeterPara);
//__EXTERN unsigned char    RunPara_InitSumGroupPara(STSUMGROUPPARA *pstSumGroupPara);
//__EXTERN unsigned char    RunPara_InitSwitchPara(STSWITCHPARA* pstSwitchPara);
//__EXTERN unsigned char    RunPara_InitAnalogPara(STANALOGPARA* pstAnalogPara);
//__EXTERN unsigned char    RunPara_InitSwitchCtrlPara(STSWITCHCTRLPARA *pstSwitchCtrlPara);
//__EXTERN unsigned char    RunPara_InitLoadCtrlWordPara(STLOADCTRLWORDPARA *pstLoadCtrlWordPara);

//__EXTERN U8 RunPara_InitDT2(STDT2 *pstDT2);

__EXTERN unsigned char RunPara_InitLinkPara(STLINKPARA *pstLinkPara);
__EXTERN unsigned char RunPara_InitRemove(STLINKREMOVE  *pstLinkRemove);
__EXTERN unsigned char RunPara_InitPortPara(STPORTPROP *pstPortProp);
__EXTERN unsigned char  RunPara_InitAuxPara(STTERAUXPARA *pstTerAuxPara);
__EXTERN unsigned char  RunPara_InitTerDevAddr(STTERDEVADDR *pstTerAddr);
__EXTERN unsigned char  RunPara_InitTerRegionAddr(STTERREGIONADDR *pstTerRegionAddr);
__EXTERN U8  RunPara_InitTerDev_Region_RegionAddr(U8 ucClrFlag);

__EXTERN unsigned char  RunPara_InitJobPara(STJOBPARA *pstJobPara);
__EXTERN U8 RunPara_InitJobPara_eeprom(U8 ucjob_num, U8 ucjob_class, U8 ucflag);    //
__EXTERN unsigned char RunPara_InitJobFnPara(STJOBFNPARA *pstJobFnPara);
//__EXTERN unsigned char RunPara_InitHarmLimit(STHARMVALUPPARA *pstHarmLimitPara);
__EXTERN void RunPara_InitJobRunInfo(U8 ucflag);
__EXTERN unsigned char RunPara_InitJobRunInfoPer(STJOBPARA *pstJobPara);
__EXTERN STDATETIME RunPara_GetJobNextTime(STDATETIME stActiveTime,unsigned char ucTimeInter,unsigned char ucTimeUnit);
__EXTERN unsigned char RunPara_InitPortPropList(STPORTLIST *pstPortPropList, U8 ucflag);
__EXTERN unsigned char RunPara_InitPortProp(STPORTPROP *pstPortProp);
__EXTERN unsigned char RunPara_InitRMRunPara(U8 ucflag);

__EXTERN unsigned char  RunPara_InitTerNormalPara(STTERNORMALPARA *pstTerNormalPara);
__EXTERN unsigned char  RunPara_InitTerMiscPara(STTERMISCPARA *pstTerMiscPara);
__EXTERN unsigned char  RunPara_InitTerAdjPara(STTERADJPARA *pstTerAdjPara);
//__EXTERN unsigned char    RunPara_InitCTCheckPara(STCTCHECKPARA *pstCTCheckPara);
//__EXTERN unsigned char    RunPara_First_InitTerPara(void);
__EXTERN unsigned char  RunPara_InitMeterProtocolInfo(void);

/******************************************终端参数获取****************************************/
__EXTERN STTERCOMMPARA  RunPara_GetTerCommPara(void);
__EXTERN STHEARTBEAT        RunPara_GetHeartBeat(void);
__EXTERN STSTAIPADDR        RunPara_GetStaIPAddr(void);
__EXTERN STSTAPHONENO   RunPara_GetStaPhoneNo(void);
__EXTERN STLOCALIP  RunPara_GetLocalIP(void);
__EXTERN STTERENCRYPT   RunPara_GetTerEncrypt(void);
__EXTERN STTERGROUPADDRLIST RunPara_GetTerGroupAddrList(void);
//__EXTERN STTERCOMMSTAT    RunPara_GetTerCommStat(void);
//__EXTERN STGPRSPDPPARA*   RunPara_GetGprsPdpPara(void);
__EXTERN unsigned char  RunPara_GetTerCommPara_RCommMode(void);

__EXTERN STALRCATEGORY*  RunPara_GetAlrShieldACDWord(unsigned char ucNo);
__EXTERN STTERALARMJUDGECOUNT   RunPara_GetAlrMaxCount(void);
//__EXTERN STALRCATEGORY  Alarm_Get_Happen_word(void);
__EXTERN unsigned char  Alarm_Set_Happen_word(STALRCATEGORY *pstAlrHappenWord);
__EXTERN unsigned char  Alarm_Set_AlrCate_word(STALRCATEGORY *pstAlrHappenWord,unsigned char ucAlrERCCode,unsigned char ucSetVal);
__EXTERN unsigned char  RunPara_GetMPType(unsigned char ucMPNo);
__EXTERN unsigned char  RunPara_GetMPDevNo(unsigned char ucMPNo);
__EXTERN unsigned char  RunPara_GetMPNo_byDevNo(unsigned char ucDevNo,unsigned char ucMPType);
__EXTERN STMPRATEVAL        RunPara_GetMPRateVal(unsigned char ucMPNo);
__EXTERN STMPALTERRATE  RunPara_GetMPAlterRate(unsigned char ucMPNo);
__EXTERN STMPLIMIT      RunPara_GetMPLimit(unsigned char ucMPNo);
//__EXTERN STMPFREEZEPARA*  RunPara_GetMPFreezePara(unsigned char ucMPNo);
//__EXTERN STFREEZEDAY        RunPara_GetMPFreezeDay(unsigned char ucMPNo);
__EXTERN PSTFREEZEDAY        RunPara_GetMPFreezeDay(unsigned char ucMPNo);
__EXTERN STPOWRATEPARA  RunPara_GetMPPowRatePara(unsigned char ucMPNo);
__EXTERN STTONGSUNTIANSUNPARA RunPara_GetMPTongsun(unsigned char ucMPNo);
__EXTERN STLINKPARA RunPara_GetLinkPara(void);
//__EXTERN STSLAVEADDR RunPara_GetSlaveAddr(U8 ucSlaveNo);
__EXTERN STLINKREMOVE RunPara_GetLinkRemove(void);
__EXTERN STPORTLIST RunPara_GetPortPropList(void);
__EXTERN STPORTPROP RunPara_GetPortProp(U8 ucPort);
__EXTERN STRMRUNPARA *RunPara_GetRMRunPara(void);

__EXTERN STCONFIGNUM        RunPara_GetConfigNum(void);
//__EXTERN STPULSEPARA      RunPara_GetPulsePara(U8 ucPulseNo);
__EXTERN STMETERPARA        RunPara_GetMeterPara(U8 ucMeterNo);
//__EXTERN U8    RunPara_GetMeterPara_Proto(U8 ucMeterNo);
__EXTERN STMETERPARA        RunPara_GetMeterPara_byMPNo(U8 ucMPNo);
//__EXTERN unsigned long RunPara_GetMeterPara_Boadrate_byMPNo(U8 ucMPNo);
__EXTERN U8  RunPara_GetMeterPara_PortNo(U8 ucMeterNo);
__EXTERN U8  RunPara_GetMeterPara_PortNo_byMPNo(U8 ucMPNo);
//__EXTERN U8 RunPara_GetMeterPara_EmphaFlag(U8 ucMeterNo);

//__EXTERN STSUMGROUPPARA   RunPara_GetSumGroupPara(U8 ucSumGNo);
//__EXTERN STSUMGROUPPARA   *RunPara_GetSumGroupParaPtr(U8 ucSumGNo);
//__EXTERN STSWITCHPARA RunPara_GetSwitchPara(U8 ucSwitchNo);
//__EXTERN STANALOGPARA RunPara_GetAnalogPara(U8 ucAnalogNo);
//__EXTERN U8    RunPara_GetAnalogPortNo(U8 ucMPNo,U8 ucAttr);

//__EXTERN STPOWERCTRLPROJ RunPara_GetPowCtrlProj(U8 ucCurveNo,U8 ucSumGroupNo);
//__EXTERN STLOADFIXVAL RunPara_GetPowFixVal(U8 *pucPowCurveNo,U8 *pucEndHour,U8 *pucEndMinute,U8 ucSumGroupNo);
//__EXTERN STPOWERCTRLHOLDTIME RunPara_GetPowCtrlHoldTime(U8 ucSumGroupNo);
//__EXTERN STCTRLTURN       RunPara_GetPowerCtrlTurn(U8 ucSumGroupNo);
//__EXTERN STENERGYCTRL RunPara_GetEnergyCtrl(U8 ucSumGroupNo);
//__EXTERN STMONENERGYREC   RunPara_GetMonEnergyRec(U8 ucSumGroupNo);
//__EXTERN STBUYENERGYREC   RunPara_GetBuyEnergyRec(U8 ucSumGroupNo);
//__EXTERN STCTRLTURN       RunPara_GetEnergyCtrlTurn(U8 ucSumGroupNo);
//__EXTERN U8    RunPara_GetInfo_IsEnerTotalOnly(U8 ucSumGroupNo);
//__EXTERN U8    RunPara_Manage_BuyBillNo(unsigned long ulBuyBillNo,U8 ucFlashFlag);

//__EXTERN STCTRLFEERATE    RunPara_GetCtrlFeeRate(void);
//__EXTERN STCTRLFEERATETIME    RunPara_GetCtrlFeeRateTime(void);
//__EXTERN STLOADFIXVAL RunPara_GetCtrlSafeFixVal(void);
//__EXTERN U8   RunPara_GetSwitchOutFlag(void);
//__EXTERN STLOADCTRLWORDPARA   RunPara_GetLoadCtrlWordPara(void);
//__EXTERN STPOWERCTRLTIME RunPara_GetPowCtrlTimePer(U8 ucCurveNo,U8 ucSumGroupNo);
//__EXTERN U8    RunPara_GetKeepPower(void);
//__EXTERN U8    RunPara_GetGprsKeepPow(void);
//__EXTERN U8    RunPara_GetIsCheckGprs(void);

//__EXTERN U8    RunPara_GetLoadCtrl_UrgeFee(void);

__EXTERN STTERAUXPARA   RunPara_GetTerAuxPara(void);
__EXTERN STTERDEVADDR   RunPara_GetTerDevAddr(void);
__EXTERN STTERREGIONADDR    RunPara_GetTerRegionAddr(void);

__EXTERN STJOBPARA*      RunPara_GetJobPara(U8 ucJobNo,U8 ucJobClassNo);
__EXTERN STUPUSR RunPara_GetUpUsrJobPara(U8 ucJobNo,U8 ucUpUsr);

__EXTERN STTERNORMALPARA    RunPara_GetTerNormalPara(void);
__EXTERN STTERMISCPARA  RunPara_GetTerMiscPara(void);
//__EXTERN unsigned long    RunPara_GetUartBoad(U8 ucUartID);
__EXTERN STTERADJPARA   RunPara_GetTerAdjPara(void);
//__EXTERN STCTCHECKPARA    RunPara_GetCTCheckPara(void);
__EXTERN STALRJUDGETIME RunPara_GetAlrJuDgeTime(void);
__EXTERN STCURALRCTRLTIME   RunPara_GetAlrCtrlTime(void);
//__EXTERN STHARMVALUPLIMIT RunPara_GetHarmValUpLimit(void);
//__EXTERN U8   RunPara_GetTerAdjPara_DownloadSource(void);
__EXTERN U8   RunPara_GetTerAdjPara_Job1HourDataExt(void);
__EXTERN U8   RunPara_GetMeterCopyInter(void);
__EXTERN U8 RunPara_GetMeterCopyCopyFlag(void);
__EXTERN U8   RunPara_GetTerMiscParaStatus_ProgramKeyIsSet(void);
__EXTERN U8   RunPara_GetTerAdjPara_EnerBuyIsNotFee(void);

__EXTERN U8  RunPara_GetTerMisc_MeterTimeDiff(void);
//__EXTERN U8   RunPara_GetTerAdj_ViewData_1_Flag(void);
__EXTERN U8   RunPara_GetTerAdj_MPNoFromZero(void);
//__EXTERN unsigned short  RunPara_GetMP_VolAlterRate(U8 ucMPNo);
//__EXTERN unsigned short  RunPara_GetMP_CurAlterRate(U8 ucMPNo);
//__EXTERN unsigned long   RunPara_GetMP_PowerAlterRate(U8 ucMPNo);
//__EXTERN U8    RunPara_GetTerMisc_RemoveOn(void);
//__EXTERN U8   RunPara_GetSwitchPara_PulseIsTypeSwitch(U8 ucPulseNo);
//__EXTERN U8    RunPara_GetTerMisc_RCommRunStat(void);
//__EXTERN U8    RunPara_GetTerMisc_GprsLoginStat(void);
//__EXTERN U8    RunPara_GetTerMisc_TerAddIsDecType(void);
__EXTERN U8  RunPara_GetTerMisc_DefaultIPType(void);
__EXTERN U8  RunPara_GetTerMisc_ucRcommBaseType(void);
__EXTERN unsigned long  RunPara_GetTerMisc_FluxLimit(void);
//azh 170919
__EXTERN U8*   RunPara_GetTerMisc_Wireless(void);
__EXTERN U8*   RunPara_GetTerMisc_TerLocation(void);
__EXTERN U8*   RunPara_GetTerMisc_Snstate(void);

//__EXTERN U8    RunPara_GetTerMisc_ShowRCommDebug(void);
//__EXTERN U8    RunPara_GetTerMisc_RcommType(U8 *pucIPInfoType0,U8 *pucIPInfoType1,U8 *pucRcommBaseType);
//__EXTERN U8    RunPara_GetTerMisc_PowDiffLimit_ucSumGNo(U8 ucSeqNo);

/******************************************终端参数设置****************************************/
__EXTERN U8 RunPara_SetTerCommPara(STTERCOMMPARA *pstTerCommPara);
__EXTERN U8 RunPara_SetHeartBeat(STHEARTBEAT *pstHeartBeat);
__EXTERN U8 RunPara_SetStaIPAddr(STSTAIPADDR *pstStaIPAddr);
__EXTERN U8 RunPara_SetStaPhoneNo(STSTAPHONENO *pstStaPhoneNo);
__EXTERN U8 RunPara_SetTerLocalIP(STLOCALIP  *pstLocalIP);
__EXTERN U8 RunPara_SetTerEncrypt(STTERENCRYPT *pstTerEncrypt);
__EXTERN U8 RunPara_SetTerGroupAddrList(STTERGROUPADDRLIST *pstTerGroupAddr);
//__EXTERN U8 RunPara_SetTerCommStat(STTERCOMMSTAT *pstTerCommStat);
//__EXTERN U8 RunPara_SetGprsPdpPara(STGPRSPDPPARA *pstGprsPdpPara);
__EXTERN U8 RunPara_SetAlrCtrlWord(STALRCTRLWORD *pstAlrCtrlWord);
__EXTERN U8 RunPara_SetAlrMaxCount(STTERALARMJUDGECOUNT *pstAlrJudgeCount);
//__EXTERN U8 RunPara_SetRcommType(U8 ucIPInfoType0,U8 ucIPInfoType1,U8 ucRcommBaseType);

__EXTERN U8 RunPara_SetMPType(U8 ucMPNo,U8 ucMPType);
__EXTERN U8 RunPara_SetMPDevNo(U8 ucMPNo,U8 ucDevNo);
__EXTERN U8 RunPara_SetMPRateVal(U8 ucMPNo,STMPRATEVAL *pstMPRateVal);
__EXTERN U8 RunPara_SetMPAlterRate(U8 ucMPNo,STMPALTERRATE *pstMPAlterRate);
__EXTERN U8 RunPara_SetMPLimit(U8 ucMPNo,STMPLIMIT    *pstMPLimit);
//__EXTERN U8 RunPara_SetMPFreezePara(U8 ucMPNo,STMPFREEZEPARA    *pstMPFreezePara);
__EXTERN U8 RunPara_SetMPFreezeDay(U8 ucMPNo,STFREEZEDAY  *pstFreezeDay);
__EXTERN U8 RunPara_SetMPPowRatePara(U8 ucMPNo,STPOWRATEPARA  *pstPowRatePara);
__EXTERN U8 RunPara_SetMPTongsun(U8 ucMPNo,STTONGSUNTIANSUNPARA *pstTongsun);

//__EXTERN U8 RunPara_SetPulseMPDevNo(U8 ucMPNo,U8 ucDevNo,U8 ucPulseType);
//__EXTERN U8 RunPara_CheckPulseMPDevNo_IsNone(U8 ucMPNo);

//__EXTERN U8 RunPara_SetConfigNum(STCONFIGNUM *pstConfigNum);
//__EXTERN U8 RunPara_SetPulsePara(U8 ucPulseNo,STPULSEPARA *pstPulsePara);
__EXTERN U8 RunPara_SetMeterPara(U8 ucMeterNo,STMETERPARA *pstMeterPara);
//__EXTERN U8 RunPara_SetSumGroupPara(U8 ucSumGNo,STSUMGROUPPARA *pstSumGPara);
//__EXTERN U8 RunPara_SetSumGroupAvailableFalg(U8 ucSumGNo,U8 ucAvailableFlag);
//__EXTERN U8 RunPara_SetSumGroupParaOper(U8 ucSumGNo,STSUMGROUPPARA *pstSumGPara);
//__EXTERN U8 RunPara_SetSumGroupParaLimit(U8 ucSumGNo,STSUMGROUPPARA *pstSumGPara);
//__EXTERN U8 RunPara_SetSumGroupParaFreeze(U8 ucSumGNo,STSUMGROUPPARA *pstSumGPara);
//__EXTERN U8 RunPara_SetSumGroupOper(U8 ucNo,STSUMVALOPER *pstSumGValOper,U8 ucMPNo,U8 ucOper);
//__EXTERN U8 RunPara_SetSwitchPara(U8 ucSwitchNo,STSWITCHPARA *pstSwitchPara);
//__EXTERN U8 RunPara_SetAnalogPara(U8 ucAnalogNo,STANALOGPARA *pstAnalogPara);
__EXTERN U8 RunPara_SetLinkPara(STLINKPARA *pstLinkPara);
__EXTERN U8 RunPara_SetLinkRemove(STLINKREMOVE *pstLinkRemove);

//__EXTERN U8 RunPara_SetPowCtrlProj(U8 ucCurveNo,STPOWERCTRLPROJ *pstPowerCtrlProj,U8 ucSumGroupNo);
//__EXTERN U8 RunPara_SetPowCtrlProj_Availble(U8 ucFlag,U8 ucCurveNo,U8 ucSumGroupNo);
//__EXTERN U8 RunPara_SetPowerCtrlAvailble(U8 ucCurveNo,U8 ucFlag,U8 ucSumGroupNo);
//__EXTERN U8 RunPara_SetCtrlSafeFixVal(STLOADFIXVAL *pstSaveFixVal);
//__EXTERN U8 RunPara_SetCtrlFeeRate(STCTRLFEERATE *pstCtrlFeeRate);
//__EXTERN U8 RunPara_SetCtrlFeeRateTime(STCTRLFEERATETIME *pstCtrlFeeRateTime);
//__EXTERN U8 RunPara_SetLoadCtrlWordPara(STLOADCTRLWORDPARA *pstLoadCtrlWordPara);
//__EXTERN U8 RunPara_SetPowCtrlTimePer(U8 ucCurveNo,STPOWERCTRLTIME *pstPowerCtrlTime,U8 ucSumGroupNo);
//__EXTERN U8 RunPara_SetPowCtrlTimePer_Availble(U8 ucFlag,U8 ucCurveNo,U8 ucSumGroupNo);
//__EXTERN U8 RunPara_SetEnergyCtrl(STENERGYCTRL *pstEnergyCtrl,U8 ucSumGroupNo);
//__EXTERN U8 RunPara_SetPowerCtrlTurn(STCTRLTURN *pstPowerCtrlTurn,U8 ucSumGroupNo);
//__EXTERN U8 RunPara_SetEnergyCtrlTurn(STCTRLTURN *pstEnergyCtrlTurn,U8 ucSumGroupNo);
//__EXTERN U8 RunPara_SetPowCtrlHoldTime(STPOWERCTRLHOLDTIME *pstPowerHoldTime,U8 ucSumGroupNo);
//__EXTERN U8 RunPara_SetKeepPower(U8 ucFlag);
//__EXTERN U8 RunPara_SetGprsCheck(U8 ucFlag);
//__EXTERN U8 RunPara_SetGprsKeepPow(U8 ucFlag);
//__EXTERN U8 RunPara_SetBuyEnergyRec(STBUYENERGYREC *pstBuyEnergyRec,U8 ucSumGroupNo);
//__EXTERN U8 RunPara_SetMonEnergyRec(STMONENERGYREC *pstMonEnergyRec,U8 ucSumGroupNo);

__EXTERN U8 RunPara_SetTerAuxPara(STTERAUXPARA *pstTerAuxPara);
__EXTERN U8 RunPara_SetTerDevAddr(STTERDEVADDR *pstTerAddr);
__EXTERN U8 RunPara_SetTerRegionAddr(STTERREGIONADDR *pstTerRegionAddr);

__EXTERN U8 RunPara_SetJobPara(U8 ucJobClassNo,U8 ucJobNo,STJOBPARA *pstJobPara);
__EXTERN U8 RunPara_SetUpUsrJob(U8 ucJobNo,U8 ucUpUsr,STUPUSR *pstUpUsr);

__EXTERN U8 RunPara_SetPortPropList(STPORTLIST *pstPortList);
__EXTERN U8 RunPara_SetPortProp(STPORTPROP *pstPortProp,U8 ucPort);
//__EXTERN U8 RunPara_SetRMRunPara(STRMRUNPARA *pstRMRunPara);

__EXTERN U8 RunPara_SetTerNormalPara(STTERNORMALPARA *pstTerNormalPara);
__EXTERN U8 RunPara_SetCopyFlag(U8 ucCopyFlag);
__EXTERN U8 RunPara_SetTerMiscPara(STTERMISCPARA *pstTerMiscPara);
__EXTERN U8 RunPara_SetTerAdjPara(STTERADJPARA *pstTerAdjPara);
//__EXTERN U8 RunPara_SetCTCheckPara(STCTCHECKPARA *pstCTCheckPara);
//__EXTERN U8 RunPara_SetTerAdjPara_DownloadSource(U8 ucSource);
//__EXTERN U8 RunPara_SetUartBoad(U8 ucUartID,unsigned long ulBoad);
//__EXTERN U8 RunPara_SetTerMiscParaStatus_ProgramKeyIsSet(U8 ucFlag);
//__EXTERN U8 RunPara_SetTerMiscPara_TerAddIsDecType(U8 ucFlag);
//__EXTERN U8 RunPara_SetAlrJuDgeTime(STALRJUDGETIME *pstAlarmJudgeTime);
//__EXTERN U8    RunPara_SetAlrCtrlTime(STCURALRCTRLTIME *pstCurAlrCtrlTime);
//__EXTERN U8    RunPara_SetHarmValUpLimit(STHARMVALUPLIMIT *pstHarmValUpLimit);
//__EXTERN U8 RunPara_SetTerAdjPara_Change_ViewData_1(void);
//__EXTERN U8  RunPara_SetAesKey(STAESKEY *pstAesKey);       //AES加密
__EXTERN U8 RunPara_SetTerMiscPara_FluxLimit(unsigned long ulVal);

//azh 170919
__EXTERN U8 RunPara_SetTerMisc_Wireless(unsigned char* ucVal);
__EXTERN U8 RunPara_SetTerMisc_TerLocation(unsigned char* ucVal);
__EXTERN U8 RunPara_SetTerMisc_Snstate(unsigned char* ucVal);

//__EXTERN U8 RunPara_SetTerMiscPara_ShowRCommDebug(U8 ucVal);

//__EXTERN STAESKEY RunPara_GetAesKey(void);                //AES加密
__EXTERN U8 RunPara_GetMeter_MPNo(U8 ucMeterNo);
__EXTERN U8 RunPara_GetPulse_MPNo(U8 ucPulseNo);
__EXTERN U8 RunPara_GetTer_MPNo(void);
__EXTERN U8 RunPara_GetMeterNo_byMPNo(U8 ucMPNo);

//__EXTERN U8 RunPara_GetPulseNo_byMPNo(U8 ucMPNo,U8 ucPulseType);

//__EXTERN U8 RunPara_GetDirPowPulseNo_byMPNo(U8 ucMPNo);
//__EXTERN U8 RunPara_GetDirVarPulseNo_byMPNo(U8 ucMPNo);
//__EXTERN U8 RunPara_GetUnDirPowPulseNo_byMPNo(U8 ucMPNo);
__EXTERN U8 RunPara_GetUnDirVarPulseNo_byMPNo(U8 ucMPNo);

//__EXTERN U8 RunPara_GetPulseType_ByDI(unsigned short usDI);

__EXTERN U8  Main_DI_Para_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,U8 *pucRecvBuffer);
__EXTERN U8  Main_DI_ParaRead_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,U8 *pucRecvBuffer);
__EXTERN U8  Main_DI_Ctrl_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,U8 ucNum4);
__EXTERN U8  Main_DI_ExtCmd_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,U8 ucNum2);
__EXTERN U8  Main_DI_DataTrans_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,U8 *pucBuffer);
__EXTERN U8  Main_DI_FileTrans_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI);

__EXTERN void RunPara_ClearPassErrTimes(void);
__EXTERN U8 RunPara_GetPassErrTimes(void);
__EXTERN void RunPara_AddPassErrTimes(void);
__EXTERN U8 RunPara_PassLockIsOverTime(void);

__EXTERN U8 RunPara_Set_LCDCycleMPNo(U8 ucMPNo);
//__EXTERN U8 RunPara_GetCtrlFeeNo_ByTime(STDATETIME *pstDateTime);

__EXTERN U8 RunPara_GetExternMeterProtocol(U8 MeterProtocol);
__EXTERN U8 RunPara_GetInnerMeterProtocol(U8 MeterProtocol);
__EXTERN U8 RunPara_GetDefaultMeterCommPara(STMETERPARA *pstMeterPara,U8 ucInnerMeterProtocol);
__EXTERN U8 RunPara_GetMeterProtocolInfo(U8 *pucInfoBuff);

__EXTERN void RunPara_GetTerVerInfo(U8 *pucVerInfo);
__EXTERN void RunPara_GetTerInnerVerInfo(U8 *pucVerInfo);
__EXTERN void RunPara_GetTerHardwareVerInfo(U8 *pucVerInfo);
__EXTERN void RunPara_GetTerProtoolVerInfo(U8 *pucVerInfo);

__EXTERN unsigned long RunPara_Get_Relay_Boadrate(U8 ucBoadInfo);
//__EXTERN void RunPara_Get_Relay_CommInfo(U8 ucCommInfo,U8 *pucEPN,U8 *pucDataBit,U8 *pucStopBit);

__EXTERN U8 RunPara_Init_TerCommPara_Inner(STTERCOMMPARA_INNER *pstTerCommpara_Inner);
__EXTERN U8 RunPara_SetTerCommPara_Inner(STTERCOMMPARA_INNER *pstTerCommPara_Inner);
__EXTERN STTERCOMMPARA_INNER    RunPara_GetTerCommPara_Inner(void);
//__EXTERN unsigned short RunPara_GetCommMode(void);
__EXTERN unsigned short RunPara_GetReConnectTime(void);
__EXTERN U8  RunPara_GetTerCommPara_ReConnectTimes(void);
__EXTERN unsigned short RunPara_GetTerCommPara_StandbyTime(void);

//__EXTERN void SetBuyBillRec(STBUYBILLREC *pstBuyBillRec,unsigned long ulBuyBillNo,U8 ucFlashFlag);
//__EXTERN STBUYBILLREC RunPara_GetBuyBillRec_byBillNo(U8 ucBillNo);
//__EXTERN U8 RunPara_SetBuyBillRec_byBillNo(STBUYBILLREC *pstBuyBillRec,U8 ucBillNo);
__EXTERN U8  Main_DI_Login_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,U8 ucNum4);
__EXTERN U8  Main_DI_Config_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,U8 ucNum4);
__EXTERN U8  Main_DI_Taskdata_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,U8 ucNum4);

__EXTERN U8 PowOn_Para_Init_All(void);
#endif

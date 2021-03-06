/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_Comm_Export.c
 Description    :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/

#ifndef _MAIN_COMM_EXPORT_H_
#define _MAIN_COMM_EXPORT_H_

#define     CON_DATA_REQ1_BEG_CODE  0x2000
#define     CON_DATA_REQ1_END_CODE  0x20FF
#define     CON_DATA_REQ2_BEG_CODE  0x2100
#define     CON_DATA_REQ2_END_CODE  0x21FF
#define     CON_DATA_REQ3_BEG_CODE  0x2200
#define     CON_DATA_REQ3_END_CODE  0x22FF
#define     CON_RST_BEG_CODE        0x2500
#define     CON_RST_END_CODE        0x251F
#define     CON_ACK_BEG_CODE        0x2540
#define     CON_ACK_END_CODE        0x255F
#define     CON_TASKDATA_REQ_BEG_CODE   0x2600
#define     CON_TASKDATA_REQ_END_CODE   0x26FF
#define     CON_EXTCMD_BEG_CODE 0x2700
#define     CON_EXTCMD_END_CODE 0x27FF
#define     CON_DATATRANS_BEG_CODE  0x2800
#define     CON_DATATRANS_END_CODE  0x28FF
#define     CON_FILETRANS_BEG_CODE  0x2900
#define     CON_FILETRANS_END_CODE  0x29FF

#define CON_DATA_REQ1_F1        CON_DATA_REQ1_BEG_CODE+1    //F1    终端版本信息
#define CON_DATA_REQ1_F2        CON_DATA_REQ1_BEG_CODE+2    //F2    终端日历时钟
#define CON_DATA_REQ1_F3        CON_DATA_REQ1_BEG_CODE+3    //F3    终端参数状态
#define CON_DATA_REQ1_F4        CON_DATA_REQ1_BEG_CODE+4    //F4    终端通信状态
#define CON_DATA_REQ1_F5        CON_DATA_REQ1_BEG_CODE+5    //F5    终端控制设置状态
#define CON_DATA_REQ1_F6        CON_DATA_REQ1_BEG_CODE+6    //F6    终端当前控制状态
#define CON_DATA_REQ1_F7        CON_DATA_REQ1_BEG_CODE+7    //F7    终端事件计数器当前值
#define CON_DATA_REQ1_F8        CON_DATA_REQ1_BEG_CODE+8    //F8    终端事件标志状态
#define CON_DATA_REQ1_F9        CON_DATA_REQ1_BEG_CODE+9    //F9    终端状态量及变位标志
#define CON_DATA_REQ1_F10       CON_DATA_REQ1_BEG_CODE+10   //F10   终端与主站当日、月通信流量
#define CON_DATA_REQ1_F11       CON_DATA_REQ1_BEG_CODE+11   //F11   当前信号强度
//azh 170921
#define CON_DATA_REQ1_F12      CON_DATA_REQ1_BEG_CODE+12    //F12   控制输出回路开关接入状态量及变位标志
#define CON_DATA_REQ1_F13      CON_DATA_REQ1_BEG_CODE+13    //F13   搜索到的电表信息
#define CON_DATA_REQ1_F14      CON_DATA_REQ1_BEG_CODE+14    //F14   文件传输未收到数据段

#define CON_DATA_REQ1_F17       CON_DATA_REQ1_BEG_CODE+17   //F17   当前总加有功功率
#define CON_DATA_REQ1_F18       CON_DATA_REQ1_BEG_CODE+18   //F18   当前总加无功功率
#define CON_DATA_REQ1_F19       CON_DATA_REQ1_BEG_CODE+19   //F19   当日总加有功电能量（总、费率1~M）
#define CON_DATA_REQ1_F20       CON_DATA_REQ1_BEG_CODE+20   //F20   当日总加无功电能量（总、费率1~M）
#define CON_DATA_REQ1_F21       CON_DATA_REQ1_BEG_CODE+21   //F21   当月总加有功电能量（总、费率1~M）
#define CON_DATA_REQ1_F22       CON_DATA_REQ1_BEG_CODE+22   //F22   当月总加无功电能量（总、费率1~M）
#define CON_DATA_REQ1_F23       CON_DATA_REQ1_BEG_CODE+23   //F23   终端当前剩余电量（费）
#define CON_DATA_REQ1_F24       CON_DATA_REQ1_BEG_CODE+24   //F24   当前功率下浮控控后总加有功功率冻结值
#define CON_DATA_REQ1_F25       CON_DATA_REQ1_BEG_CODE+25   //F25   当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流
#define CON_DATA_REQ1_F26       CON_DATA_REQ1_BEG_CODE+26   //F26   A、B、C三相断相统计数据及最近一次断相记录
#define CON_DATA_REQ1_F27       CON_DATA_REQ1_BEG_CODE+27   //F27   电能表日历时钟、编程次数及其最近一次操作时间
#define CON_DATA_REQ1_F28       CON_DATA_REQ1_BEG_CODE+28   //F28   电表运行状态字及其变位标志
#define CON_DATA_REQ1_F31       CON_DATA_REQ1_BEG_CODE+31   //F31
#define CON_DATA_REQ1_F32       CON_DATA_REQ1_BEG_CODE+32   //F32
#define CON_DATA_REQ1_F33       CON_DATA_REQ1_BEG_CODE+33   //F33   当前正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
#define CON_DATA_REQ1_F34       CON_DATA_REQ1_BEG_CODE+34   //F34   当前反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
#define CON_DATA_REQ1_F35       CON_DATA_REQ1_BEG_CODE+35   //F35   当月正向有/无功最大需量及发生时间（总、费率1~M）
#define CON_DATA_REQ1_F36       CON_DATA_REQ1_BEG_CODE+36   //F36   当月反向有/无功最大需量及发生时间（总、费率1~M）
#define CON_DATA_REQ1_F37       CON_DATA_REQ1_BEG_CODE+37   //F37   上月正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
#define CON_DATA_REQ1_F38       CON_DATA_REQ1_BEG_CODE+38   //F38   上月反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
#define CON_DATA_REQ1_F39       CON_DATA_REQ1_BEG_CODE+39   //F39   上月正向有/无功最大需量及发生时间（总、费率1~M）
#define CON_DATA_REQ1_F40       CON_DATA_REQ1_BEG_CODE+40   //F40   上月反向有/无功最大需量及发生时间（总、费率1~M）
#define CON_DATA_REQ1_F41       CON_DATA_REQ1_BEG_CODE+41   //F41   当日正向有功电能量（总、费率1~M）
#define CON_DATA_REQ1_F42       CON_DATA_REQ1_BEG_CODE+42   //F42   当日正向无功电能量（总、费率1~M）
#define CON_DATA_REQ1_F43       CON_DATA_REQ1_BEG_CODE+43   //F43   当日反向有功电能量（总、费率1~M）
#define CON_DATA_REQ1_F44       CON_DATA_REQ1_BEG_CODE+44   //F44   当日反向无功电能量（总、费率1~M）
#define CON_DATA_REQ1_F45       CON_DATA_REQ1_BEG_CODE+45   //F45   当月正向有功电能量（总、费率1~M）
#define CON_DATA_REQ1_F46       CON_DATA_REQ1_BEG_CODE+46   //F46   当月正向无功电能量（总、费率1~M）
#define CON_DATA_REQ1_F47       CON_DATA_REQ1_BEG_CODE+47   //F47   当月反向有功电能量（总、费率1~M）
#define CON_DATA_REQ1_F48       CON_DATA_REQ1_BEG_CODE+48   //F48   当月反向无功电能量（总、费率1~M）
#define CON_DATA_REQ1_F49       CON_DATA_REQ1_BEG_CODE+49   //F49   当前电压、电流相位角
#define CON_DATA_REQ1_F57       CON_DATA_REQ1_BEG_CODE+57   //F57   当前A、B、C三相电压、电流2~N次谐波有效值
#define CON_DATA_REQ1_F58       CON_DATA_REQ1_BEG_CODE+58   //F58   当前A、B、C三相电压、电流2~N次谐波含有率
#define CON_DATA_REQ1_F65       CON_DATA_REQ1_BEG_CODE+65   //F65   当前电容器投切状态
#define CON_DATA_REQ1_F66       CON_DATA_REQ1_BEG_CODE+66   //F66   当前电容器累计补偿投入时间和次数
#define CON_DATA_REQ1_F67       CON_DATA_REQ1_BEG_CODE+67   //F67   当日、当月电容器累计补偿的无功电能量
#define CON_DATA_REQ1_F73       CON_DATA_REQ1_BEG_CODE+73   //F73   直流模拟量实时数据
#define CON_DATA_REQ1_F81       CON_DATA_REQ1_BEG_CODE+81   //F81   小时冻结总加有功功率
#define CON_DATA_REQ1_F82       CON_DATA_REQ1_BEG_CODE+82   //F82   小时冻结总加无功功率
#define CON_DATA_REQ1_F83       CON_DATA_REQ1_BEG_CODE+83   //F83   小时冻结总加有功总电能量
#define CON_DATA_REQ1_F84       CON_DATA_REQ1_BEG_CODE+84   //F84   小时冻结总加无功总电能量
#define CON_DATA_REQ1_F89       CON_DATA_REQ1_BEG_CODE+89   //F89   小时冻结有功功率
#define CON_DATA_REQ1_F90       CON_DATA_REQ1_BEG_CODE+90   //F90   小时冻结A相有功功率
#define CON_DATA_REQ1_F91       CON_DATA_REQ1_BEG_CODE+91   //F91   小时冻结B相有功功率
#define CON_DATA_REQ1_F92       CON_DATA_REQ1_BEG_CODE+92   //F92   小时冻结C相有功功率
#define CON_DATA_REQ1_F93       CON_DATA_REQ1_BEG_CODE+93   //F93   小时冻结无功功率
#define CON_DATA_REQ1_F94       CON_DATA_REQ1_BEG_CODE+94   //F94   小时冻结A相无功功率
#define CON_DATA_REQ1_F95       CON_DATA_REQ1_BEG_CODE+95   //F95   小时冻结B相无功功率
#define CON_DATA_REQ1_F96       CON_DATA_REQ1_BEG_CODE+96   //F96   小时冻结C相无功功率
#define CON_DATA_REQ1_F97       CON_DATA_REQ1_BEG_CODE+97   //F97   小时冻结A相电压
#define CON_DATA_REQ1_F98       CON_DATA_REQ1_BEG_CODE+98   //F98   小时冻结B相电压
#define CON_DATA_REQ1_F99       CON_DATA_REQ1_BEG_CODE+99   //F99   小时冻结C相电压
#define CON_DATA_REQ1_F100      CON_DATA_REQ1_BEG_CODE+100  //F100  小时冻结A相电流
#define CON_DATA_REQ1_F101      CON_DATA_REQ1_BEG_CODE+101  //F101  小时冻结B相电流
#define CON_DATA_REQ1_F102      CON_DATA_REQ1_BEG_CODE+102  //F102  小时冻结C相电流
#define CON_DATA_REQ1_F103      CON_DATA_REQ1_BEG_CODE+103  //F103  小时冻结零序电流
#define CON_DATA_REQ1_F105      CON_DATA_REQ1_BEG_CODE+105  //F105  小时冻结正向有功总电能量
#define CON_DATA_REQ1_F106      CON_DATA_REQ1_BEG_CODE+106  //F106  小时冻结正向无功总电能量
#define CON_DATA_REQ1_F107      CON_DATA_REQ1_BEG_CODE+107  //F107  小时冻结反向有功总电能量
#define CON_DATA_REQ1_F108      CON_DATA_REQ1_BEG_CODE+108  //F108  小时冻结反向无功总电能量
#define CON_DATA_REQ1_F109      CON_DATA_REQ1_BEG_CODE+109  //F109  小时冻结正向有功总电能示值
#define CON_DATA_REQ1_F110      CON_DATA_REQ1_BEG_CODE+110  //F110  小时冻结正向无功总电能示值
#define CON_DATA_REQ1_F111      CON_DATA_REQ1_BEG_CODE+111  //F111  小时冻结反向有功总电能示值
#define CON_DATA_REQ1_F112      CON_DATA_REQ1_BEG_CODE+112  //F112  小时冻结反向无功总电能示值
#define CON_DATA_REQ1_F113      CON_DATA_REQ1_BEG_CODE+113  //F113  小时冻结总功率因数
#define CON_DATA_REQ1_F114      CON_DATA_REQ1_BEG_CODE+114  //F114  小时冻结A相功率因数
#define CON_DATA_REQ1_F115      CON_DATA_REQ1_BEG_CODE+115  //F115  小时冻结B相功率因数
#define CON_DATA_REQ1_F116      CON_DATA_REQ1_BEG_CODE+116  //F116  小时冻结C相功率因数
#define CON_DATA_REQ1_F121      CON_DATA_REQ1_BEG_CODE+121  //F121  小时冻结直流模拟量

#define CON_DATA_REQ1_F129      CON_DATA_REQ1_BEG_CODE+129
#define CON_DATA_REQ1_F130      CON_DATA_REQ1_BEG_CODE+130
#define CON_DATA_REQ1_F131      CON_DATA_REQ1_BEG_CODE+131
#define CON_DATA_REQ1_F132      CON_DATA_REQ1_BEG_CODE+132
#define CON_DATA_REQ1_F133      CON_DATA_REQ1_BEG_CODE+133
#define CON_DATA_REQ1_F134      CON_DATA_REQ1_BEG_CODE+134
#define CON_DATA_REQ1_F135      CON_DATA_REQ1_BEG_CODE+135
#define CON_DATA_REQ1_F136      CON_DATA_REQ1_BEG_CODE+136
#define CON_DATA_REQ1_F137      CON_DATA_REQ1_BEG_CODE+137
#define CON_DATA_REQ1_F138      CON_DATA_REQ1_BEG_CODE+138
#define CON_DATA_REQ1_F139      CON_DATA_REQ1_BEG_CODE+139
#define CON_DATA_REQ1_F140      CON_DATA_REQ1_BEG_CODE+140
#define CON_DATA_REQ1_F141      CON_DATA_REQ1_BEG_CODE+141
#define CON_DATA_REQ1_F142      CON_DATA_REQ1_BEG_CODE+142
#define CON_DATA_REQ1_F143      CON_DATA_REQ1_BEG_CODE+143
#define CON_DATA_REQ1_F144      CON_DATA_REQ1_BEG_CODE+144
#define CON_DATA_REQ1_F145      CON_DATA_REQ1_BEG_CODE+145
#define CON_DATA_REQ1_F146      CON_DATA_REQ1_BEG_CODE+146
#define CON_DATA_REQ1_F147      CON_DATA_REQ1_BEG_CODE+147
#define CON_DATA_REQ1_F148      CON_DATA_REQ1_BEG_CODE+148
#define CON_DATA_REQ1_F149      CON_DATA_REQ1_BEG_CODE+149
#define CON_DATA_REQ1_F150      CON_DATA_REQ1_BEG_CODE+150
#define CON_DATA_REQ1_F151      CON_DATA_REQ1_BEG_CODE+151
#define CON_DATA_REQ1_F152      CON_DATA_REQ1_BEG_CODE+152
#define CON_DATA_REQ1_F161      CON_DATA_REQ1_BEG_CODE+161
//azh 170921
#define CON_DATA_REQ1_F162      CON_DATA_REQ1_BEG_CODE+162  //F162   电能表日历时钟
#define CON_DATA_REQ1_F177      CON_DATA_REQ1_BEG_CODE+177  //F177   当前组合有功电能示值（总、费率1～M）
#define CON_DATA_REQ1_F178      CON_DATA_REQ1_BEG_CODE+178  //F178   上1结算日组合有功电能示值（总、费率1～M）

#define CON_DATA_REQ2_F1        CON_DATA_REQ2_BEG_CODE+1    //F1    正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
#define CON_DATA_REQ2_F2        CON_DATA_REQ2_BEG_CODE+2    //F2    反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
#define CON_DATA_REQ2_F3        CON_DATA_REQ2_BEG_CODE+3    //F3    正向有/无功最大需量及发生时间（总、费率1~M）
#define CON_DATA_REQ2_F4        CON_DATA_REQ2_BEG_CODE+4    //F4    反向有/无功最大需量及发生时间（总、费率1~M）
#define CON_DATA_REQ2_F5        CON_DATA_REQ2_BEG_CODE+5    //F5    正向有功电能量（总、费率1~M）
#define CON_DATA_REQ2_F6        CON_DATA_REQ2_BEG_CODE+6    //F6    正向无功电能量（总、费率1~M）
#define CON_DATA_REQ2_F7        CON_DATA_REQ2_BEG_CODE+7    //F7    反向有功电能量（总、费率1~M）
#define CON_DATA_REQ2_F8        CON_DATA_REQ2_BEG_CODE+8    //F8    反向无功电能量（总、费率1~M）
#define CON_DATA_REQ2_F9        CON_DATA_REQ2_BEG_CODE+9    //F9    正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
#define CON_DATA_REQ2_F10       CON_DATA_REQ2_BEG_CODE+10   //F10   反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
#define CON_DATA_REQ2_F11       CON_DATA_REQ2_BEG_CODE+11   //F11   正向有/无功最大需量及发生时间（总、费率1~M）
#define CON_DATA_REQ2_F12       CON_DATA_REQ2_BEG_CODE+12   //F12   反向有/无功最大需量及发生时间（总、费率1~M）
#define CON_DATA_REQ2_F17       CON_DATA_REQ2_BEG_CODE+17   //F17   正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
#define CON_DATA_REQ2_F18       CON_DATA_REQ2_BEG_CODE+18   //F18   反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
#define CON_DATA_REQ2_F19       CON_DATA_REQ2_BEG_CODE+19   //F19   正向有/无功最大需量及发生时间（总、费率1~M）
#define CON_DATA_REQ2_F20       CON_DATA_REQ2_BEG_CODE+20   //F20   反向有/无功最大需量及发生时间（总、费率1~M）
#define CON_DATA_REQ2_F21       CON_DATA_REQ2_BEG_CODE+21   //F21   正向有功电能量（总、费率1~M）
#define CON_DATA_REQ2_F22       CON_DATA_REQ2_BEG_CODE+22   //F22   正向无功电能量（总、费率1~M）
#define CON_DATA_REQ2_F23       CON_DATA_REQ2_BEG_CODE+23   //F23   反向有功电能量（总、费率1~M）
#define CON_DATA_REQ2_F24       CON_DATA_REQ2_BEG_CODE+24   //F24   反向无功电能量（总、费率1~M）
#define CON_DATA_REQ2_F25       CON_DATA_REQ2_BEG_CODE+25   //F25   日分相最大有功功率及发生时间、有功功率为零时间
#define CON_DATA_REQ2_F26       CON_DATA_REQ2_BEG_CODE+26   //F26   日总及分相最大需量及发生时间
#define CON_DATA_REQ2_F27       CON_DATA_REQ2_BEG_CODE+27   //F27   日电压统计数据
#define CON_DATA_REQ2_F28       CON_DATA_REQ2_BEG_CODE+28   //F28   日不平衡度越限累计时间
#define CON_DATA_REQ2_F29       CON_DATA_REQ2_BEG_CODE+29   //F29   日电流越限统计
#define CON_DATA_REQ2_F30       CON_DATA_REQ2_BEG_CODE+30   //F30   日视在功率越限累计时间
#define CON_DATA_REQ2_F31       CON_DATA_REQ2_BEG_CODE+31   //F31   日电能表断相数据
#define CON_DATA_REQ2_F32       CON_DATA_REQ2_BEG_CODE+32   //F32   备用
#define CON_DATA_REQ2_F33       CON_DATA_REQ2_BEG_CODE+33   //F33   月分相最大有功功率及发生时间、分相有功功率为零时间
#define CON_DATA_REQ2_F34       CON_DATA_REQ2_BEG_CODE+34   //F34   月分相有功最大需量及发生时间、总有功最大需量及发生时间
#define CON_DATA_REQ2_F35       CON_DATA_REQ2_BEG_CODE+35   //F35   月电压统计数据
#define CON_DATA_REQ2_F36       CON_DATA_REQ2_BEG_CODE+36   //F36   月不平衡度越限累计时间
#define CON_DATA_REQ2_F37       CON_DATA_REQ2_BEG_CODE+37   //F37   月电流越限统计
#define CON_DATA_REQ2_F38       CON_DATA_REQ2_BEG_CODE+38   //F38   月视在功率越限累计时间
#define CON_DATA_REQ2_F41       CON_DATA_REQ2_BEG_CODE+41   //F41   电容器投入累计时间和次数
#define CON_DATA_REQ2_F42       CON_DATA_REQ2_BEG_CODE+42   //F42   日、月电容器累计补偿的无功电能量
#define CON_DATA_REQ2_F43       CON_DATA_REQ2_BEG_CODE+43   //F43   日功率因数区段累计时间
#define CON_DATA_REQ2_F44       CON_DATA_REQ2_BEG_CODE+44   //F44   月功率因数区段累计时间
#define CON_DATA_REQ2_F49       CON_DATA_REQ2_BEG_CODE+49   //F49   终端日供电时间、日复位累计次数
#define CON_DATA_REQ2_F50       CON_DATA_REQ2_BEG_CODE+50   //F50   终端日控制统计数据
#define CON_DATA_REQ2_F51       CON_DATA_REQ2_BEG_CODE+51   //F51   终端月供电时间、月复位累计次数
#define CON_DATA_REQ2_F52       CON_DATA_REQ2_BEG_CODE+52   //F52   终端月控制统计数据
#define CON_DATA_REQ2_F53       CON_DATA_REQ2_BEG_CODE+53   //F53   终端与主站日通信流量
#define CON_DATA_REQ2_F54       CON_DATA_REQ2_BEG_CODE+54   //F54   终端与主站月通信流量
#define CON_DATA_REQ2_F57       CON_DATA_REQ2_BEG_CODE+57   //F57   总加组日最大、最小有功功率及其发生时间，有功功率为零日累计时间
#define CON_DATA_REQ2_F58       CON_DATA_REQ2_BEG_CODE+58   //F58   总加组日累计有功电能量（总、费率1~M）
#define CON_DATA_REQ2_F59       CON_DATA_REQ2_BEG_CODE+59   //F59   总加组日累计无功电能量（总、费率1~M）
#define CON_DATA_REQ2_F60       CON_DATA_REQ2_BEG_CODE+60   //F60   总加组月最大、最小有功功率及其发生时间，有功功率为零月累计时间
#define CON_DATA_REQ2_F61       CON_DATA_REQ2_BEG_CODE+61   //F61   总加组月累计有功电能量（总、费率1~M）
#define CON_DATA_REQ2_F62       CON_DATA_REQ2_BEG_CODE+62   //F62   总加组月累计无功电能量（总、费率1~M）
#define CON_DATA_REQ2_F65       CON_DATA_REQ2_BEG_CODE+65   //F65   总加组超功率定值的月累计时间、月累计电能量
#define CON_DATA_REQ2_F66       CON_DATA_REQ2_BEG_CODE+66   //F66   总加组超月电能量定值的月累计时间、累计电能量
#define CON_DATA_REQ2_F73       CON_DATA_REQ2_BEG_CODE+73   //F73   总加组有功功率曲线
#define CON_DATA_REQ2_F74       CON_DATA_REQ2_BEG_CODE+74   //F74   总加组无功功率曲线
#define CON_DATA_REQ2_F75       CON_DATA_REQ2_BEG_CODE+75   //F75   总加组有功电能量曲线
#define CON_DATA_REQ2_F76       CON_DATA_REQ2_BEG_CODE+76   //F76   总加组无功电能量曲线
#define CON_DATA_REQ2_F81       CON_DATA_REQ2_BEG_CODE+81   //F81   有功功率曲线
#define CON_DATA_REQ2_F82       CON_DATA_REQ2_BEG_CODE+82   //F82   A相有功功率曲线
#define CON_DATA_REQ2_F83       CON_DATA_REQ2_BEG_CODE+83   //F83   B相有功功率曲线
#define CON_DATA_REQ2_F84       CON_DATA_REQ2_BEG_CODE+84   //F84   C相有功功率曲线
#define CON_DATA_REQ2_F85       CON_DATA_REQ2_BEG_CODE+85   //F85   无功功率曲线
#define CON_DATA_REQ2_F86       CON_DATA_REQ2_BEG_CODE+86   //F86   A相无功功率曲线
#define CON_DATA_REQ2_F87       CON_DATA_REQ2_BEG_CODE+87   //F87   B相无功功率曲线
#define CON_DATA_REQ2_F88       CON_DATA_REQ2_BEG_CODE+88   //F88   C相无功功率曲线
#define CON_DATA_REQ2_F89       CON_DATA_REQ2_BEG_CODE+89   //F89   A相电压曲线
#define CON_DATA_REQ2_F90       CON_DATA_REQ2_BEG_CODE+90   //F90   B相电压曲线
#define CON_DATA_REQ2_F91       CON_DATA_REQ2_BEG_CODE+91   //F91   C相电压曲线
#define CON_DATA_REQ2_F92       CON_DATA_REQ2_BEG_CODE+92   //F92   A相电流曲线
#define CON_DATA_REQ2_F93       CON_DATA_REQ2_BEG_CODE+93   //F93   B相电流曲线
#define CON_DATA_REQ2_F94       CON_DATA_REQ2_BEG_CODE+94   //F94   C相电流曲线
#define CON_DATA_REQ2_F95       CON_DATA_REQ2_BEG_CODE+95   //F95   零序电流曲线
#define CON_DATA_REQ2_F97       CON_DATA_REQ2_BEG_CODE+97   //F97   正向有功总电能量
#define CON_DATA_REQ2_F98       CON_DATA_REQ2_BEG_CODE+98   //F98   正向无功总电能量
#define CON_DATA_REQ2_F99       CON_DATA_REQ2_BEG_CODE+99   //F99   反向有功总电能量
#define CON_DATA_REQ2_F100      CON_DATA_REQ2_BEG_CODE+100  //F100  反向无功总电能量
#define CON_DATA_REQ2_F101      CON_DATA_REQ2_BEG_CODE+101  //F101  正向有功总电能示值
#define CON_DATA_REQ2_F102      CON_DATA_REQ2_BEG_CODE+102  //F102  正向无功总电能示值
#define CON_DATA_REQ2_F103      CON_DATA_REQ2_BEG_CODE+103  //F103  反向有功总电能示值
#define CON_DATA_REQ2_F104      CON_DATA_REQ2_BEG_CODE+104  //F104  反向无功总电能示值
#define CON_DATA_REQ2_F105      CON_DATA_REQ2_BEG_CODE+105  //F105  总功率因数
#define CON_DATA_REQ2_F106      CON_DATA_REQ2_BEG_CODE+106  //F106  A相功率因数
#define CON_DATA_REQ2_F107      CON_DATA_REQ2_BEG_CODE+107  //F107  B相功率因数
#define CON_DATA_REQ2_F108      CON_DATA_REQ2_BEG_CODE+108  //F108  C相功率因数
#define CON_DATA_REQ2_F109      CON_DATA_REQ2_BEG_CODE+109
#define CON_DATA_REQ2_F110      CON_DATA_REQ2_BEG_CODE+110
#define CON_DATA_REQ2_F113      CON_DATA_REQ2_BEG_CODE+113  //F113  A相2~19次谐波电流日最大值及发生时间
#define CON_DATA_REQ2_F114      CON_DATA_REQ2_BEG_CODE+114  //F114  B相2~19次谐波电流日最大值及发生时间
#define CON_DATA_REQ2_F115      CON_DATA_REQ2_BEG_CODE+115  //F115  C相2~19次谐波电流日最大值及发生时间
#define CON_DATA_REQ2_F116      CON_DATA_REQ2_BEG_CODE+116  //F116  A相2~19次谐波电压含有率及总畸变率日最大值及发生时间
#define CON_DATA_REQ2_F117      CON_DATA_REQ2_BEG_CODE+117  //F117  B相2~19次谐波电压含有率及总畸变率日最大值及发生时间
#define CON_DATA_REQ2_F118      CON_DATA_REQ2_BEG_CODE+118  //F118  C相2~19次谐波电压含有率及总畸变率日最大值及发生时间
#define CON_DATA_REQ2_F121      CON_DATA_REQ2_BEG_CODE+121  //F121  A相谐波越限日统计数据
#define CON_DATA_REQ2_F122      CON_DATA_REQ2_BEG_CODE+122  //F122  B相谐波越限日统计数据
#define CON_DATA_REQ2_F123      CON_DATA_REQ2_BEG_CODE+123  //F123  C相谐波越限日统计数据
#define CON_DATA_REQ2_F129      CON_DATA_REQ2_BEG_CODE+129  //F129  直流模拟量越限日累计时间、最大/最小值及发生时间
#define CON_DATA_REQ2_F130      CON_DATA_REQ2_BEG_CODE+130  //F130  直流模拟量越限月累计时间、最大/最小值及发生时间
#define CON_DATA_REQ2_F138      CON_DATA_REQ2_BEG_CODE+138  //F138  直流模拟量数据曲线
#define CON_DATA_REQ2_F145      CON_DATA_REQ2_BEG_CODE+145  //F145  一象限无功总电能示值曲线
#define CON_DATA_REQ2_F146      CON_DATA_REQ2_BEG_CODE+146  //F146  四象限无功总电能示值曲线
#define CON_DATA_REQ2_F147      CON_DATA_REQ2_BEG_CODE+147  //F147  二象限无功总电能示值曲线
#define CON_DATA_REQ2_F148      CON_DATA_REQ2_BEG_CODE+148  //F148  三象限无功总电能示值曲线

#define CON_DATA_REQ2_F153      CON_DATA_REQ2_BEG_CODE+153  //F148  三象限无功总电能示值曲线
#define CON_DATA_REQ2_F154      CON_DATA_REQ2_BEG_CODE+154  //F148  三象限无功总电能示值曲线
#define CON_DATA_REQ2_F155      CON_DATA_REQ2_BEG_CODE+155  //F148  三象限无功总电能示值曲线
#define CON_DATA_REQ2_F156      CON_DATA_REQ2_BEG_CODE+156  //F148  三象限无功总电能示值曲线
#define CON_DATA_REQ2_F157      CON_DATA_REQ2_BEG_CODE+157  //F148  三象限无功总电能示值曲线
#define CON_DATA_REQ2_F158      CON_DATA_REQ2_BEG_CODE+158  //F148  三象限无功总电能示值曲线
#define CON_DATA_REQ2_F159      CON_DATA_REQ2_BEG_CODE+159  //F148  三象限无功总电能示值曲线
#define CON_DATA_REQ2_F160      CON_DATA_REQ2_BEG_CODE+160  //F148  三象限无功总电能示值曲线

#define CON_DATA_REQ2_F161      CON_DATA_REQ2_BEG_CODE+161
#define CON_DATA_REQ2_F162      CON_DATA_REQ2_BEG_CODE+162
#define CON_DATA_REQ2_F163      CON_DATA_REQ2_BEG_CODE+163
#define CON_DATA_REQ2_F164      CON_DATA_REQ2_BEG_CODE+164
#define CON_DATA_REQ2_F165      CON_DATA_REQ2_BEG_CODE+165
#define CON_DATA_REQ2_F166      CON_DATA_REQ2_BEG_CODE+166
#define CON_DATA_REQ2_F167      CON_DATA_REQ2_BEG_CODE+167
#define CON_DATA_REQ2_F168      CON_DATA_REQ2_BEG_CODE+168
#define CON_DATA_REQ2_F169      CON_DATA_REQ2_BEG_CODE+169
#define CON_DATA_REQ2_F170      CON_DATA_REQ2_BEG_CODE+170
#define CON_DATA_REQ2_F171      CON_DATA_REQ2_BEG_CODE+171
#define CON_DATA_REQ2_F172      CON_DATA_REQ2_BEG_CODE+172
#define CON_DATA_REQ2_F173      CON_DATA_REQ2_BEG_CODE+173
#define CON_DATA_REQ2_F174      CON_DATA_REQ2_BEG_CODE+174
#define CON_DATA_REQ2_F175      CON_DATA_REQ2_BEG_CODE+175
#define CON_DATA_REQ2_F176      CON_DATA_REQ2_BEG_CODE+176
#define CON_DATA_REQ2_F177      CON_DATA_REQ2_BEG_CODE+177
#define CON_DATA_REQ2_F178      CON_DATA_REQ2_BEG_CODE+178
#define CON_DATA_REQ2_F179      CON_DATA_REQ2_BEG_CODE+179
#define CON_DATA_REQ2_F180      CON_DATA_REQ2_BEG_CODE+180
#define CON_DATA_REQ2_F181      CON_DATA_REQ2_BEG_CODE+181
#define CON_DATA_REQ2_F182      CON_DATA_REQ2_BEG_CODE+182
#define CON_DATA_REQ2_F183      CON_DATA_REQ2_BEG_CODE+183
#define CON_DATA_REQ2_F184      CON_DATA_REQ2_BEG_CODE+184
#define CON_DATA_REQ2_F185      CON_DATA_REQ2_BEG_CODE+185
#define CON_DATA_REQ2_F186      CON_DATA_REQ2_BEG_CODE+186
#define CON_DATA_REQ2_F187      CON_DATA_REQ2_BEG_CODE+187
#define CON_DATA_REQ2_F188      CON_DATA_REQ2_BEG_CODE+188
#define CON_DATA_REQ2_F189      CON_DATA_REQ2_BEG_CODE+189
#define CON_DATA_REQ2_F190      CON_DATA_REQ2_BEG_CODE+190
#define CON_DATA_REQ2_F191      CON_DATA_REQ2_BEG_CODE+191
#define CON_DATA_REQ2_F192      CON_DATA_REQ2_BEG_CODE+192
#define CON_DATA_REQ2_F193      CON_DATA_REQ2_BEG_CODE+193
#define CON_DATA_REQ2_F194      CON_DATA_REQ2_BEG_CODE+194
#define CON_DATA_REQ2_F195      CON_DATA_REQ2_BEG_CODE+195
#define CON_DATA_REQ2_F196      CON_DATA_REQ2_BEG_CODE+196
//azh 170925
#define CON_DATA_REQ2_F219      CON_DATA_REQ2_BEG_CODE+219//F219	测量点组合数据曲线数据单元格式

#define CON_DATA_REQ3_F1        CON_DATA_REQ3_BEG_CODE+1    //F1    重要事件
#define CON_DATA_REQ3_F2        CON_DATA_REQ3_BEG_CODE+2    //F2    一般事件

#define CON_TASKDATA_REQ_F1     CON_TASKDATA_REQ_BEG_CODE+1 //F1    1类数据
#define CON_TASKDATA_REQ_F2		CON_TASKDATA_REQ_BEG_CODE+2	//F2	2类数据//CON_TASKDATA_REQ_END_CODE+2	//F2	2类数据

#define CON_ACK_F1              CON_ACK_BEG_CODE+1          //F1    全部确认
#define CON_ACK_F2              CON_ACK_BEG_CODE+2          //F2    全部否认
#define CON_ACK_F3              CON_ACK_BEG_CODE+3          //F3    按数据项进行确认否认

#define CON_RST_F1_HARD         CON_RST_BEG_CODE+1          //F1    硬件初始化
#define CON_RST_F2_DATA         CON_RST_BEG_CODE+2          //F2    数据区初始化
#define CON_RST_F3_ALL          CON_RST_BEG_CODE+3          //F3    参数及全体数据区初始化
#define CON_RST_F4_EXCECOMM     CON_RST_BEG_CODE+4          //除通讯参数之外的数据初始化

#define CON_EXTCMD_F1           CON_EXTCMD_BEG_CODE+1       //F1 内存读
#define CON_EXTCMD_F2           CON_EXTCMD_BEG_CODE+2       //F2 内存写
#define CON_EXTCMD_F3           CON_EXTCMD_BEG_CODE+3       //F3 DATAFLASH读
#define CON_EXTCMD_F4           CON_EXTCMD_BEG_CODE+4       //F4 DATAFLASH写
#define CON_EXTCMD_F5           CON_EXTCMD_BEG_CODE+5       //F5 看门狗测试，让程序进入死循环
#define CON_EXTCMD_F6           CON_EXTCMD_BEG_CODE+6       //F6 手机模块通讯检测
#define CON_EXTCMD_F7           CON_EXTCMD_BEG_CODE+7       //F7 终端状态检测
#define CON_EXTCMD_F10          CON_EXTCMD_BEG_CODE+10      //F10 终端地址10/16进制切换
#define CON_EXTCMD_F11          CON_EXTCMD_BEG_CODE+11      //F11 中文信息读取
#define CON_EXTCMD_F21          CON_EXTCMD_BEG_CODE+21      //F21 GR任务读
#define CON_EXTCMD_F22          CON_EXTCMD_BEG_CODE+22      //F22 GR任务配置
#define CON_EXTCMD_F23          CON_EXTCMD_BEG_CODE+23      //F23 GR任务信息读
#define CON_EXTCMD_F24          CON_EXTCMD_BEG_CODE+24      //F24 GR任务信息保存
#define CON_EXTCMD_F31          CON_EXTCMD_BEG_CODE+31      //F31 脉冲需量清零
#define CON_EXTCMD_F33          CON_EXTCMD_BEG_CODE+33      //F33 脉冲底度设置内容全
#define CON_EXTCMD_F40          CON_EXTCMD_BEG_CODE+40      //F40 内表数据设置
#define CON_EXTCMD_F41          CON_EXTCMD_BEG_CODE+41      //F41 内表数据读取
#define CON_EXTCMD_F51          CON_EXTCMD_BEG_CODE+51      //F51 终端地址行政区号读
#define CON_EXTCMD_F52          CON_EXTCMD_BEG_CODE+52      //F52 终端序号读
#define CON_EXTCMD_F53          CON_EXTCMD_BEG_CODE+53      //F53 终端地址行政区号写
#define CON_EXTCMD_F54          CON_EXTCMD_BEG_CODE+54      //F54 终端序号写
#define CON_EXTCMD_F71          CON_EXTCMD_BEG_CODE+71      //F71 蜂鸣器控制
#define CON_EXTCMD_F72          CON_EXTCMD_BEG_CODE+72      //F72 修改时钟
#define CON_EXTCMD_F73          CON_EXTCMD_BEG_CODE+73      //F73 查询通讯波特率
#define CON_EXTCMD_F74          CON_EXTCMD_BEG_CODE+74      //F74 修改通讯波特率
#define CON_EXTCMD_F75          CON_EXTCMD_BEG_CODE+75      //F75 读取液晶轮显显示参数
#define CON_EXTCMD_F76          CON_EXTCMD_BEG_CODE+76      //F76 读取液晶操作显示参数
#define CON_EXTCMD_F77          CON_EXTCMD_BEG_CODE+77      //F77 读取液晶特殊显示参数
#define CON_EXTCMD_F78          CON_EXTCMD_BEG_CODE+78      //F78 修改液晶轮显显示参数
#define CON_EXTCMD_F79          CON_EXTCMD_BEG_CODE+79      //F79 修改液晶操作显示参数
#define CON_EXTCMD_F80          CON_EXTCMD_BEG_CODE+80      //F80 修改液晶特殊显示参数

#define CON_EXTCMD_F81          CON_EXTCMD_BEG_CODE+81      //F81 启用SIM卡的pin
#define CON_EXTCMD_F82          CON_EXTCMD_BEG_CODE+82      //F82 修改SIM卡的pin
#define CON_EXTCMD_F83          CON_EXTCMD_BEG_CODE+83      //F83 用PUK设置SIM卡的pin
#define CON_EXTCMD_F84          CON_EXTCMD_BEG_CODE+84      //F84 SIM卡的pin相关状态
#define CON_EXTCMD_F85          CON_EXTCMD_BEG_CODE+85      //F85 GSM/GPRS切换
#define CON_EXTCMD_F86          CON_EXTCMD_BEG_CODE+86      //F86 读功率控制恢复时间长度
#define CON_EXTCMD_F87          CON_EXTCMD_BEG_CODE+87      //F87 写功率控制恢复时间长度
#define CON_EXTCMD_F89          CON_EXTCMD_BEG_CODE+89      //F89 读功率控制越限确认时间
#define CON_EXTCMD_F90          CON_EXTCMD_BEG_CODE+90      //F90 写功率控制越限确认时间
#define CON_EXTCMD_F91          CON_EXTCMD_BEG_CODE+91      //F91 读自定义调整参数
#define CON_EXTCMD_F92          CON_EXTCMD_BEG_CODE+92      //F92 写自定义调整参数
#define CON_EXTCMD_F93          CON_EXTCMD_BEG_CODE+93      //F93 读取终端电压两位小数电流四位小数
#define CON_EXTCMD_F94          CON_EXTCMD_BEG_CODE+94      //F94 任务初始化
#define CON_EXTCMD_F95          CON_EXTCMD_BEG_CODE+95      //F95 8025时钟矫正
#define CON_EXTCMD_F96          CON_EXTCMD_BEG_CODE+96      //F96 液晶对比度设置
#define CON_EXTCMD_F97          CON_EXTCMD_BEG_CODE+97      //F97 设置表计通迅名和密码
#define CON_EXTCMD_F98          CON_EXTCMD_BEG_CODE+98      //F98 读取表计用户名和密码
#define CON_EXTCMD_F99          CON_EXTCMD_BEG_CODE+99      //F99 设置GPRS接入的PDP用户名、密码
#define CON_EXTCMD_F100         CON_EXTCMD_BEG_CODE+100     //F100 读取GPRS接入的PDP用户名、密码
#define CON_EXTCMD_F101         CON_EXTCMD_BEG_CODE+101     //F101 设置GPRS重连间隔时间
#define CON_EXTCMD_F102         CON_EXTCMD_BEG_CODE+102     //F102 读取GPRS重连间隔时间
#define CON_EXTCMD_F109         CON_EXTCMD_BEG_CODE+109     //F109 设置终端工作模式
#define CON_EXTCMD_F110         CON_EXTCMD_BEG_CODE+110     //F110 读取终端工作模式
#define CON_EXTCMD_F111         CON_EXTCMD_BEG_CODE+111     //F111 设置CT类型
#define CON_EXTCMD_F112         CON_EXTCMD_BEG_CODE+112     //F112 读取CT类型
#define CON_EXTCMD_F113         CON_EXTCMD_BEG_CODE+113     //F113 设置自学习有效标志
#define CON_EXTCMD_F114         CON_EXTCMD_BEG_CODE+114     //F114 读取自学习有效标志
#define CON_EXTCMD_F115         CON_EXTCMD_BEG_CODE+115     //F115 设置二次侧开路阈值
#define CON_EXTCMD_F116         CON_EXTCMD_BEG_CODE+116     //F116 读取二次侧开路阈值
#define CON_EXTCMD_F117         CON_EXTCMD_BEG_CODE+117     //F117 设置一次侧短路阈值
#define CON_EXTCMD_F118         CON_EXTCMD_BEG_CODE+118     //F118 读取一次侧短路阈值
#define CON_EXTCMD_F119         CON_EXTCMD_BEG_CODE+119     //F119 读取阻抗值
#define CON_EXTCMD_F120         CON_EXTCMD_BEG_CODE+120     //F120 设置AES加密密钥
#define CON_EXTCMD_F121         CON_EXTCMD_BEG_CODE+121     //F121 读取AES加密密钥
#define CON_EXTCMD_F122         CON_EXTCMD_BEG_CODE+122     //F122 读取设置网络断线终端处于保电状态
#define CON_EXTCMD_F123         CON_EXTCMD_BEG_CODE+123     //F123 读取温度

#define CON_EXTCMD_F124         CON_EXTCMD_BEG_CODE+124     //F124 读取校表EEPROM的参数
#define CON_EXTCMD_F125         CON_EXTCMD_BEG_CODE+125     //F125 设置校表EEPROM的参数
#define CON_EXTCMD_F126         CON_EXTCMD_BEG_CODE+126     //界面登录密码


#define CON_DATATRANS_F1        CON_DATATRANS_BEG_CODE+1    //F1 转发命令
#define CON_DATATRANS_F9        CON_DATATRANS_BEG_CODE+9
#define CON_DATATRANS_F10       CON_DATATRANS_BEG_CODE+10
#define CON_DATATRANS_F11       CON_DATATRANS_BEG_CODE+11
#define CON_FILETRANS_F1        CON_FILETRANS_BEG_CODE+1    //F1 文件转发
#define CON_FILETRANS_F100      CON_FILETRANS_BEG_CODE+100  //F100 程序下装命令

#define     CON_MAINCOMMINFO_NUM    20
#define     CON_INVALID_SEQNO       0xFF
typedef struct _STMAINCOMMINFO
{
    unsigned char ucSEQ;
    unsigned char ucMSA;
    unsigned char ucTPvInfo[6];
}STMAINCOMMINFO;

typedef struct _STMAINCOMMINFOLIST
{
    unsigned char   ucSeqNo[CON_MAINCOMMINFO_NUM];
    STMAINCOMMINFO  stMainCommInfo[CON_MAINCOMMINFO_NUM];
}STMAINCOMMINFOLIST;


__EXTERN unsigned short     g_usNeedDebugCommInfo;              //需要显示调试信息
__EXTERN STRMSENDTASKLIST	stRMSendTaskList;				//上送临时队列
//---------------------------------------------------------------------
__EXTERN unsigned char Init_Main_Comm(void);
//接收处理总入口
__EXTERN unsigned char Main_Recv_Proc(unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char ucMsgSource);
//接收到的报文帧格式检查
__EXTERN unsigned char Main_Protocol_CheckFrame(unsigned char *pucRecvBuffer,unsigned short usLen);
//上行解释，加帧头帧尾
__EXTERN unsigned char Main_SendExplain(unsigned char ucMsgType,unsigned char *pucSrcBuffer,unsigned char *pucSendBuffer,unsigned short *pusLen,STMAINCOMMINFO  *pstMainCommInfo,unsigned char ucUpActive,unsigned char ucConFlag,unsigned char ucFrameFlag);

//填写发送
__EXTERN unsigned char Main_FillUpData(unsigned char *pucSendBuffer,unsigned short usLen,unsigned char ucMsgSource,unsigned char ucLinkFN);
//远程通信接收处理

__EXTERN void TerSeq_SetCount(unsigned char ucCount);
__EXTERN void Main_Set_MsgSource(unsigned char ucMsgSource);

__EXTERN unsigned char Main_Get_MsgSource(void);
__EXTERN unsigned char Check_CurrentLoginSeq_IsOk(void);

__EXTERN unsigned char Main_GetCommInfoList(unsigned char ucSeqNo,STMAINCOMMINFO *pstMainCommInfo);


__EXTERN unsigned long GetPQRate_Var4B_01(unsigned long lPower,unsigned long lVar); //得到功率因数值，格式xxx.xxx
__EXTERN unsigned long Calc_In(unsigned long ulVala,unsigned long ulValb,unsigned long ulValc);

__EXTERN unsigned char Main_SendComm_DownloadProgram(unsigned char *pucSrcBuf,unsigned short usLen,unsigned char ucMsgSource,unsigned char ucFlag);

__EXTERN unsigned short SetMainComm_Buf_DateTime_YMDHMS(unsigned char *pucBuffer,unsigned short usPos,STDATETIME stDateTime);
__EXTERN unsigned short SetMainComm_Buf_DateTime_YMDHM(unsigned char *pucBuffer,unsigned short usPos,STDATETIME stDateTime);
__EXTERN unsigned short SetMainComm_Buf_DateTime_MDHM(unsigned char *pucBuffer,unsigned short usPos,STDATETIME stDateTime);
__EXTERN unsigned short SetMainComm_Buf_DateTime_HMS(unsigned char *pucBuffer,unsigned short usPos,STDATETIME stDateTime);
__EXTERN unsigned short SetMainComm_Buf_DateTime_DHM(unsigned char *pucBuffer,unsigned short usPos,STDATETIME stDateTime);
__EXTERN unsigned short SetMainComm_Buf_DateTime_YMD(unsigned char *pucBuffer,unsigned short usPos,STDATETIME stDateTime);
__EXTERN unsigned short SetMainComm_Buf_DateTime_YMDW(unsigned char *pucBuffer,unsigned short usPos,STDATETIME stDateTime);
__EXTERN unsigned short SetMainComm_Buf_DateTime_YM(unsigned char *pucBuffer,unsigned short usPos,STDATETIME stDateTime);

__EXTERN unsigned short  SetMainComm_Buf_lVal_2B(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal);
__EXTERN unsigned short  SetMainComm_Buf_lVal_3B(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal);
__EXTERN unsigned short  SetMainComm_Buf_lVal_4B(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal);
//__EXTERN unsigned short  SetMainComm_Buf_lVal_2B_DT07(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal);
__EXTERN unsigned short  SetMainComm_Buf_lVal_5B_DT14(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal);
__EXTERN unsigned short  SetMainComm_Buf_lVal_4B_DT13(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal);
__EXTERN unsigned short  SetMainComm_Buf_lVal_3B_DT09(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal,unsigned char ucSign);
__EXTERN unsigned short  SetMainComm_Buf_lVal_2B_DT05(unsigned char *pucBuffer,unsigned short usPos,short sVal);
__EXTERN unsigned short  SetMainComm_Buf_lVal_2B_Sign(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal,unsigned char ucSign);
__EXTERN unsigned short     SetMainComm_Buf_lVal_3B_Sign(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal,unsigned char ucSign);
__EXTERN unsigned char Main_GetXishu(unsigned char *ucBuffer,unsigned short *pusVal);

__EXTERN unsigned char Check_Data_DT05(unsigned char *pucData);

__EXTERN unsigned char Check_Data_DT07(unsigned char *pucData);
__EXTERN unsigned char Check_Data_DT23(unsigned char *pucData);

// __EXTERN unsigned char   Main_GetDTList(U8 *pucRecvBuffer, U8 *pucDTList);
__EXTERN U8 Main_GetDADTList(U8 *pucRecvBuffer, U16 *pusDAList, U8 *pucDTList);
__EXTERN unsigned char  Main_GetItemLabelList(unsigned char ucRepType,unsigned char *pucDTList,unsigned short *pusItemLabelList);
// __EXTERN U8  Main_GetDAList(U8 *pucRecvBuffer, U16 *pusDAList);
__EXTERN unsigned char  Get_DA_by_MPNo(unsigned char ucMPNo,unsigned char *pucDA1,unsigned char *pucDA2);

__EXTERN unsigned char  Get_DT_by_ItemLabel(unsigned char ucRepType,unsigned short usItemLabel,unsigned char *pucDT1,unsigned char *pucDT2);
__EXTERN unsigned short GetItemLabelbyDT(unsigned char ucRepType,unsigned char ucDT);

__EXTERN STDATETIME GetLastOneHourStartTime(unsigned char ucMinOffset);
//__EXTERN STDATETIME GetLastOneHourStartTime_Diff(unsigned char ucMinOffset);
__EXTERN unsigned char GetFreeseMByMPNo(unsigned char ucMPNo);
__EXTERN unsigned char GetFreeseMByGroupNo(unsigned char ucMPNo);
__EXTERN void Adjust_Curve_Td_Starttime(unsigned char *pucBuff,unsigned char ucM);

__EXTERN unsigned char Main_Get_ReqData_1_F1(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);   //F1    终端版本信息
__EXTERN unsigned char Main_Get_ReqData_1_F2(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);   //F2    终端日历时钟
__EXTERN unsigned char Main_Get_ReqData_1_F3(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);   //F3    终端参数状态
__EXTERN unsigned char Main_Get_ReqData_1_F4(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);   //F4    终端通信状态
__EXTERN unsigned char Main_Get_ReqData_1_F5(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);   //F5    终端控制设置状态
__EXTERN unsigned char Main_Get_ReqData_1_F6(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);   //F6    终端当前控制状态
__EXTERN unsigned char Main_Get_ReqData_1_F7(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);   //F7    终端事件计数器当前值
__EXTERN unsigned char Main_Get_ReqData_1_F8(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);   //F8    终端事件标志状态
__EXTERN unsigned char Main_Get_ReqData_1_F9(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);   //F9    终端状态量及变位标志
__EXTERN unsigned char Main_Get_ReqData_1_F10(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F10   终端与主站当日、月通信流量
__EXTERN unsigned char Main_Get_ReqData_1_F11(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F11   当前信号强度
//azh 170921
__EXTERN unsigned char Main_Get_ReqData_1_F12(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F12   控制输出回路开关接入状态量及变位标志
__EXTERN unsigned char Main_Get_ReqData_1_F13(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F13   搜索到的电表信息
__EXTERN unsigned char Main_Get_ReqData_1_F14(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F14   文件传输未收到数据段

__EXTERN unsigned char Main_Get_ReqData_1_F17(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F17   当前总加有功功率
__EXTERN unsigned char Main_Get_ReqData_1_F18(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F18   当前总加无功功率
__EXTERN unsigned char Main_Get_ReqData_1_F19(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F19   当日总加有功电能量（总、费率1~M）
__EXTERN unsigned char Main_Get_ReqData_1_F20(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F20   当日总加无功电能量（总、费率1~M）
__EXTERN unsigned char Main_Get_ReqData_1_F21(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F21   当月总加有功电能量（总、费率1~M）
__EXTERN unsigned char Main_Get_ReqData_1_F22(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F22   当月总加无功电能量（总、费率1~M）
__EXTERN unsigned char Main_Get_ReqData_1_F23(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F23   终端当前剩余电量（费）
__EXTERN unsigned char Main_Get_ReqData_1_F24(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F24   当前功率下浮控控后总加有功功率冻结值
__EXTERN unsigned char Main_Get_ReqData_1_F25(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F25   当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流
__EXTERN unsigned char Main_Get_ReqData_1_F26(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F26   A、B、C三相断相统计数据及最近一次断相记录
__EXTERN unsigned char Main_Get_ReqData_1_F27(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F27   电能表日历时钟、编程次数及其最近一次操作时间
__EXTERN unsigned char Main_Get_ReqData_1_F28(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F28   电表运行状态字及其变位标志
__EXTERN unsigned char Main_Get_ReqData_1_F31(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F32(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);

__EXTERN unsigned char Main_Get_ReqData_1_F33(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F33   当前正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
__EXTERN unsigned char Main_Get_ReqData_1_F34(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F34   当前反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
__EXTERN unsigned char Main_Get_ReqData_1_F35(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F35   当月正向有/无功最大需量及发生时间（总、费率1~M）
__EXTERN unsigned char Main_Get_ReqData_1_F36(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F36   当月反向有/无功最大需量及发生时间（总、费率1~M）
__EXTERN unsigned char Main_Get_ReqData_1_F37(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F37   上月正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
__EXTERN unsigned char Main_Get_ReqData_1_F38(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F38   上月反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
__EXTERN unsigned char Main_Get_ReqData_1_F39(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F39   上月正向有/无功最大需量及发生时间（总、费率1~M）
__EXTERN unsigned char Main_Get_ReqData_1_F40(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F40   上月反向有/无功最大需量及发生时间（总、费率1~M）
__EXTERN unsigned char Main_Get_ReqData_1_F41(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F41   当日正向有功电能量（总、费率1~M）
__EXTERN unsigned char Main_Get_ReqData_1_F42(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F42   当日正向无功电能量（总、费率1~M）
__EXTERN unsigned char Main_Get_ReqData_1_F43(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F43   当日反向有功电能量（总、费率1~M）
__EXTERN unsigned char Main_Get_ReqData_1_F44(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F44   当日反向无功电能量（总、费率1~M）
__EXTERN unsigned char Main_Get_ReqData_1_F45(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F45   当月正向有功电能量（总、费率1~M）
__EXTERN unsigned char Main_Get_ReqData_1_F46(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F46   当月正向无功电能量（总、费率1~M）
__EXTERN unsigned char Main_Get_ReqData_1_F47(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F47   当月反向有功电能量（总、费率1~M）
__EXTERN unsigned char Main_Get_ReqData_1_F48(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F48   当月反向无功电能量（总、费率1~M）
__EXTERN unsigned char Main_Get_ReqData_1_F49(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F49   当前电压、电流相位角
__EXTERN unsigned char Main_Get_ReqData_1_F57(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F57   当前A、B、C三相电压、电流2~N次谐波有效值
__EXTERN unsigned char Main_Get_ReqData_1_F58(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F58   当前A、B、C三相电压、电流2~N次谐波含有率
__EXTERN unsigned char Main_Get_ReqData_1_F65(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F65   当前电容器投切状态
__EXTERN unsigned char Main_Get_ReqData_1_F66(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F66   当前电容器累计补偿投入时间和次数
__EXTERN unsigned char Main_Get_ReqData_1_F67(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F67   当日、当月电容器累计补偿的无功电能量
__EXTERN unsigned char Main_Get_ReqData_1_F73(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F73   直流模拟量实时数据
__EXTERN unsigned char Main_Get_ReqData_1_F81(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F81   小时冻结总加有功功率
__EXTERN unsigned char Main_Get_ReqData_1_F82(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F82   小时冻结总加无功功率
__EXTERN unsigned char Main_Get_ReqData_1_F83(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F83   小时冻结总加有功总电能量
__EXTERN unsigned char Main_Get_ReqData_1_F84(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F84   小时冻结总加无功总电能量
__EXTERN unsigned char Main_Get_ReqData_1_F89(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F89   小时冻结有功功率
__EXTERN unsigned char Main_Get_ReqData_1_F90(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F90   小时冻结A相有功功率
__EXTERN unsigned char Main_Get_ReqData_1_F91(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F91   小时冻结B相有功功率
__EXTERN unsigned char Main_Get_ReqData_1_F92(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F92   小时冻结C相有功功率
__EXTERN unsigned char Main_Get_ReqData_1_F93(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F93   小时冻结无功功率
__EXTERN unsigned char Main_Get_ReqData_1_F94(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F94   小时冻结A相无功功率
__EXTERN unsigned char Main_Get_ReqData_1_F95(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F95   小时冻结B相无功功率
__EXTERN unsigned char Main_Get_ReqData_1_F96(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F96   小时冻结C相无功功率
__EXTERN unsigned char Main_Get_ReqData_1_F97(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F97   小时冻结A相电压
__EXTERN unsigned char Main_Get_ReqData_1_F98(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F98   小时冻结B相电压
__EXTERN unsigned char Main_Get_ReqData_1_F99(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F99   小时冻结C相电压
__EXTERN unsigned char Main_Get_ReqData_1_F100(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F100  小时冻结A相电流
__EXTERN unsigned char Main_Get_ReqData_1_F101(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F101  小时冻结B相电流
__EXTERN unsigned char Main_Get_ReqData_1_F102(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F102  小时冻结C相电流
__EXTERN unsigned char Main_Get_ReqData_1_F103(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F103  小时冻结零序电流
__EXTERN unsigned char Main_Get_ReqData_1_F105(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F105  小时冻结正向有功总电能量
__EXTERN unsigned char Main_Get_ReqData_1_F106(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F106  小时冻结正向无功总电能量
__EXTERN unsigned char Main_Get_ReqData_1_F107(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F107  小时冻结反向有功总电能量
__EXTERN unsigned char Main_Get_ReqData_1_F108(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F108  小时冻结反向无功总电能量
__EXTERN unsigned char Main_Get_ReqData_1_F109(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F109  小时冻结正向有功总电能示值
__EXTERN unsigned char Main_Get_ReqData_1_F110(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F110  小时冻结正向无功总电能示值
__EXTERN unsigned char Main_Get_ReqData_1_F111(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F111  小时冻结反向有功总电能示值
__EXTERN unsigned char Main_Get_ReqData_1_F112(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F112  小时冻结反向无功总电能示值
__EXTERN unsigned char Main_Get_ReqData_1_F113(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F113  小时冻结总功率因数
__EXTERN unsigned char Main_Get_ReqData_1_F114(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F114  小时冻结A相功率因数
__EXTERN unsigned char Main_Get_ReqData_1_F115(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F115  小时冻结B相功率因数
__EXTERN unsigned char Main_Get_ReqData_1_F116(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F116  小时冻结C相功率因数
//__EXTERN unsigned char Main_Get_ReqData_1_F121(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);   //F121  小时冻结直流模拟量
__EXTERN unsigned char Main_Get_ReqData_1_F129(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F130(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F131(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F132(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F133(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F134(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F135(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F136(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F137(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F138(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F139(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F140(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F141(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F142(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F143(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F144(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F145(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F146(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F147(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F148(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F149(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F150(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F151(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F152(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_InstantPow(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI);
__EXTERN unsigned char Main_Get_ReqData_1_Volt(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI);
__EXTERN unsigned char Main_Get_ReqData_1_Curr(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI);
__EXTERN unsigned char Main_Get_ReqData_1_PowVal(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI);
__EXTERN unsigned char Main_Get_ReqData_1_PowViewData(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI);
__EXTERN unsigned char Main_Get_ReqData_1_PowRate(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI);
//azh 170921
__EXTERN unsigned char Main_Get_ReqData_1_F162(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F162   电能表日历时钟

__EXTERN unsigned char Get_CurvePointNum_ByTime(STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucM);

__EXTERN unsigned char Main_Get_ReqData_2_F1(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F2(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F3(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F4(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F5(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F6(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F7(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F8(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F9(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F10(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F11(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F12(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F17(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F18(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F19(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F20(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F21(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F22(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F23(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F24(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F25(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F26(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F27(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F28(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F29(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F30(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F31(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F33(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F34(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F35(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F36(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F37(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F38(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F43(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F44(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F49(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
//__EXTERN unsigned char Main_Get_ReqData_2_F50(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F51(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
//__EXTERN unsigned char Main_Get_ReqData_2_F52(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F53(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F54(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
//__EXTERN unsigned char Main_Get_ReqData_2_F57(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
//__EXTERN unsigned char Main_Get_ReqData_2_F58(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
//__EXTERN unsigned char Main_Get_ReqData_2_F59(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
//__EXTERN unsigned char Main_Get_ReqData_2_F60(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
//__EXTERN unsigned char Main_Get_ReqData_2_F61(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
//__EXTERN unsigned char Main_Get_ReqData_2_F62(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
//__EXTERN unsigned char Main_Get_ReqData_2_F65(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
//__EXTERN unsigned char Main_Get_ReqData_2_F66(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
//__EXTERN unsigned char Main_Get_ReqData_2_F73(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
//__EXTERN unsigned char Main_Get_ReqData_2_F74(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
//__EXTERN unsigned char Main_Get_ReqData_2_F75(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
//__EXTERN unsigned char Main_Get_ReqData_2_F76(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F81(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F82(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F83(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F84(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F85(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F86(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F87(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F88(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F89(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F90(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F91(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F92(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F93(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F94(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F95(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F97(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F98(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F99(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F100(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F101(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F102(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F103(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F104(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F105(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F106(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F107(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F108(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F109(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F110(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
//azh 170923
__EXTERN unsigned char Main_Get_ReqData_2_F219(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);

__EXTERN unsigned char Main_Get_ReqData_2_InstantPow(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI);
__EXTERN unsigned char Main_Get_ReqData_2_Volt(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI);
__EXTERN unsigned char Main_Get_ReqData_2_Curr(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI);
__EXTERN unsigned char Main_Get_ReqData_2_PowVal(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI);
__EXTERN unsigned char Main_Get_ReqData_2_PowViewData(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI);
__EXTERN unsigned char Main_Get_ReqData_2_PowRate(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI);
__EXTERN unsigned char Main_Get_ReqData_2_SumGroup_Pow(unsigned char ucGroupNo,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI,unsigned char ucPowFlag);
//azh 170923
__EXTERN unsigned char Main_Get_ReqData_2_GroupCurveData(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI);

__EXTERN unsigned char Main_Get_ReqData_2_PhasePowData(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_PhasePowMonData(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI,STDATETIME *pstDataTime);

__EXTERN unsigned short Main_Get_ReqData_2_F27F35_Aux(unsigned short usPos,unsigned char *pucRetBuffer,STHISRUNDATA *pstHisRunData);
__EXTERN unsigned short Main_Get_ReqData_2_F29F37_Aux(unsigned short usPos,unsigned char *pucRetBuffer,STHISRUNDATA *pstHisRunData);
__EXTERN unsigned char Job_Data_Explain(unsigned char *pucBuffer,unsigned char usLen,unsigned char ucRepType,
                               unsigned char *pucRetTerMsgNum);
__EXTERN unsigned char Main_GetSendData(unsigned char ucRetTerMsgNum,unsigned char ucMSA,unsigned char ucSEQ, unsigned char *pret_mem);
__EXTERN unsigned char  Clr_Group_RetData(void);
__EXTERN unsigned char Clr_Group_RetExtData(void);//清除数据
__EXTERN unsigned char  Main_SendComm_Recv(unsigned char ucMsgSource,unsigned char ucLinkFN,unsigned char ucUpActive);
__EXTERN unsigned char Main_RComm_UpStart(void);
__EXTERN void MainComm_StartUp(unsigned char ucFlag);
__EXTERN unsigned char Main_RComm_RingStart(void);
__EXTERN void Main_RComm_UpStop(void);
__EXTERN unsigned char Main_Init_LoginCount(void);
__EXTERN unsigned char Main_RComm_SetStandBy_Time(void);

__EXTERN void Main_RComm_OverFlux_Logout(void);
__EXTERN void Main_RComm_Set_OverFlux_Logout(unsigned char ucCount);
__EXTERN unsigned char Main_RComm_Get_OverFlux_Logout(void);
__EXTERN unsigned char Main_RComm_Get_OverFlux_AlrStatus(void);
__EXTERN unsigned char Main_Fill_TmpUpSendTask(unsigned char *pucSendBuffer,unsigned short usLen,unsigned char ucLinkFN);

__EXTERN unsigned char Main_Get_TmpUpSendTask(void);

#endif

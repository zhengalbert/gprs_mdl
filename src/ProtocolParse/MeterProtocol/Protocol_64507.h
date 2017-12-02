/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Protocol_64507.h
 Description    : DLT-645规约解释详细处理
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-08-05    azh     the original version
******************************************************************************/
#ifndef _PROTOCOL_64507_H_
#define _PROTOCOL_64507_H_

////////////////////////////////////////////////////////////////////////////////////数据类型
#define CON_DI64507_MPDIRPOWTOTAL						0x00010000//正向有功总电能
#define CON_DI64507_MPDIRPOWPLUS						0x00010100//正向有功尖电能
#define CON_DI64507_MPDIRPOWAPEX						0x00010200//正向有功峰电能
#define CON_DI64507_MPDIRPOWFLAT						0x00010300//正向有功平电能
#define CON_DI64507_MPDIRPOWVALE						0x00010400//正向有功谷电能
#define CON_DI64507_MPDIRPOW_BLOCK						0x0001FF00//正向有功电能数据块

#define CON_DI64507_MPUNDIRPOWTOTAL						0x00020000//反向有功总电能
#define CON_DI64507_MPUNDIRPOWPLUS						0x00020100//反向有功尖电能
#define CON_DI64507_MPUNDIRPOWAPEX						0x00020200//反向有功峰电能
#define CON_DI64507_MPUNDIRPOWFLAT						0x00020300//反向有功平电能
#define CON_DI64507_MPUNDIRPOWVALE						0x00020400//反向有功谷电能
#define CON_DI64507_MPUNDIRPOW_BLOCK 					0x0002FF00//反向有功电能数据块

#define CON_DI64507_MPDIRVARTOTAL						0x00030000//组合无功1总电能
#define CON_DI64507_MPDIRVARPLUS						0x00030100//组合无功1尖电能
#define CON_DI64507_MPDIRVARAPEX						0x00030200//组合无功1峰电能
#define CON_DI64507_MPDIRVARFLAT						0x00030300//组合无功1平电能
#define CON_DI64507_MPDIRVARVALE						0x00030400//组合无功1谷电能
#define CON_DI64507_MPDIRVAR_BLOCK						0x0003FF00//组合无功1电能数据块

#define CON_DI64507_MPUNDIRVARTOTAL						0x00040000//组合无功2总电能
#define CON_DI64507_MPUNDIRVARPLUS						0x00040100//组合无功2尖电能
#define CON_DI64507_MPUNDIRVARAPEX						0x00040200//组合无功2峰电能
#define CON_DI64507_MPUNDIRVARFLAT						0x00040300//组合无功2平电能
#define CON_DI64507_MPUNDIRVARVALE						0x00040400//组合无功2谷电能
#define CON_DI64507_MPUNDIRVAR_BLOCK					0x0004FF00//组合无功2电能数据块
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPDIRPOWTOTAL_LM					0x00010001//正向有功总电能（上月）
#define CON_DI64507_MPDIRPOWPLUS_LM						0x00010101//正向有功尖电能（上月）
#define CON_DI64507_MPDIRPOWAPEX_LM						0x00010201//正向有功峰电能（上月）
#define CON_DI64507_MPDIRPOWFLAT_LM						0x00010301//正向有功平电能（上月）
#define CON_DI64507_MPDIRPOWVALE_LM						0x00010401//正向有功谷电能（上月）
#define CON_DI64507_MPDIRPOW_LM_BLOCK					0x0001FF01//正向有功电能数据块（上月）

#define CON_DI64507_MPUNDIRPOWTOTAL_LM					0x00020001//反向有功总电能（上月）
#define CON_DI64507_MPUNDIRPOWPLUS_LM					0x00020101//反向有功尖电能（上月）
#define CON_DI64507_MPUNDIRPOWAPEX_LM					0x00020201//反向有功峰电能（上月）
#define CON_DI64507_MPUNDIRPOWFLAT_LM					0x00020301//反向有功平电能（上月）
#define CON_DI64507_MPUNDIRPOWVALE_LM					0x00020401//反向有功谷电能（上月）
#define CON_DI64507_MPUNDIRPOW_LM_BLOCK 				0x0002FF01//反向有功电能数据块（上月）

#define CON_DI64507_MPDIRVARTOTAL_LM					0x00030001//组合无功1总电能（上月）
#define CON_DI64507_MPDIRVARPLUS_LM						0x00030101//组合无功1尖电能（上月）
#define CON_DI64507_MPDIRVARAPEX_LM						0x00030201//组合无功1峰电能（上月）
#define CON_DI64507_MPDIRVARFLAT_LM						0x00030301//组合无功1平电能（上月）
#define CON_DI64507_MPDIRVARVALE_LM						0x00030401//组合无功1谷电能（上月）
#define CON_DI64507_MPDIRVAR_LM_BLOCK					0x0003FF01//组合无功1电能数据块（上月）

#define CON_DI64507_MPUNDIRVARTOTAL_LM					0x00040001//组合无功2总电能（上月）
#define CON_DI64507_MPUNDIRVARPLUS_LM					0x00040101//组合无功2尖电能（上月）
#define CON_DI64507_MPUNDIRVARAPEX_LM					0x00040201//组合无功2峰电能（上月）
#define CON_DI64507_MPUNDIRVARFLAT_LM					0x00040301//组合无功2平电能（上月）
#define CON_DI64507_MPUNDIRVARVALE_LM					0x00040401//组合无功2谷电能（上月）
#define CON_DI64507_MPUNDIRVAR_LM_BLOCK 				0x0004FF01//组合无功2电能数据块（上月）
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPVAR1								0x00050000//一象限无功总电能
#define CON_DI64507_MPVAR1_FEE1							0x00050100//费率1
#define CON_DI64507_MPVAR1_FEE2							0x00050200//费率2
#define CON_DI64507_MPVAR1_FEE3							0x00050300//费率3
#define CON_DI64507_MPVAR1_FEE4							0x00050400//费率4
#define CON_DI64507_MPVAR1_BLOCK						0x0005FF00
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPVAR4								0x00080000//四象限无功总电能
#define CON_DI64507_MPVAR4_FEE1							0x00080100//费率1
#define CON_DI64507_MPVAR4_FEE2							0x00080200//费率2
#define CON_DI64507_MPVAR4_FEE3							0x00080300//费率3
#define CON_DI64507_MPVAR4_FEE4							0x00080400//费率4
#define CON_DI64507_MPVAR4_BLOCK						0x0008FF00
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPVAR2								0x00060000//二象限无功总电能
#define CON_DI64507_MPVAR2_FEE1							0x00060100//费率1
#define CON_DI64507_MPVAR2_FEE2							0x00060200//费率2
#define CON_DI64507_MPVAR2_FEE3							0x00060300//费率3
#define CON_DI64507_MPVAR2_FEE4							0x00060400//费率4
#define CON_DI64507_MPVAR2_BLOCK						0x0006FF00
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPVAR3								0x00070000//三象限无功总电能
#define CON_DI64507_MPVAR3_FEE1							0x00070100//费率1
#define CON_DI64507_MPVAR3_FEE2							0x00070200//费率2
#define CON_DI64507_MPVAR3_FEE3							0x00070300//费率3
#define CON_DI64507_MPVAR3_FEE4							0x00070400//费率4
#define CON_DI64507_MPVAR3_BLOCK						0x0007FF00
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPVAR1_LM							0x00050001//一象限无功总电能（上月）
#define CON_DI64507_MPVAR1_LM_FEE1						0x00050101//费率1
#define CON_DI64507_MPVAR1_LM_FEE2						0x00050201//费率2
#define CON_DI64507_MPVAR1_LM_FEE3						0x00050301//费率3
#define CON_DI64507_MPVAR1_LM_FEE4						0x00050401//费率4
#define CON_DI64507_MPVAR1_LM_BLOCK						0x0005FF01
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPVAR4_LM							0x00080001//四象限无功总电能（上月）
#define CON_DI64507_MPVAR4_LM_FEE1						0x00080101//费率1
#define CON_DI64507_MPVAR4_LM_FEE2						0x00080201//费率2
#define CON_DI64507_MPVAR4_LM_FEE3						0x00080301//费率3
#define CON_DI64507_MPVAR4_LM_FEE4						0x00080401//费率4
#define CON_DI64507_MPVAR4_LM_BLOCK						0x0008FF01
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPVAR2_LM							0x00060001//二象限无功总电能（上月）
#define CON_DI64507_MPVAR2_LM_FEE1						0x00060101//费率1
#define CON_DI64507_MPVAR2_LM_FEE2						0x00060201//费率2
#define CON_DI64507_MPVAR2_LM_FEE3						0x00060301//费率3
#define CON_DI64507_MPVAR2_LM_FEE4						0x00060401//费率4
#define CON_DI64507_MPVAR2_LM_BLOCK						0x0006FF01
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPVAR3_LM							0x00070001//三象限无功总电能（上月）
#define CON_DI64507_MPVAR3_LM_FEE1						0x00070101//费率1
#define CON_DI64507_MPVAR3_LM_FEE2						0x00070201//费率2
#define CON_DI64507_MPVAR3_LM_FEE3						0x00070301//费率3
#define CON_DI64507_MPVAR3_LM_FEE4						0x00070401//费率4
#define CON_DI64507_MPVAR3_LM_BLOCK						0x0007FF01
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPDIRPOWMAXDEMTIM					0x01010000//正向有功总最大需量及发生时间
#define CON_DI64507_MPDIRPOWMAXDEMTIM_FEE1				0x01010100//费率1最大需量及发生时间
#define CON_DI64507_MPDIRPOWMAXDEMTIM_FEE2				0x01010200//费率2最大需量及发生时间
#define CON_DI64507_MPDIRPOWMAXDEMTIM_FEE3				0x01010300//费率3最大需量及发生时间
#define CON_DI64507_MPDIRPOWMAXDEMTIM_FEE4				0x01010400//费率4最大需量及发生时间
#define CON_DI64507_MPDIRPOWMAXDEMTIM_BLOCK				0x0101FF00
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPUNDIRPOWMAXDEMTIM					0x01020000//反向有功总最大需量及发生时间
#define CON_DI64507_MPUNDIRPOWMAXDEMTIM_FEE1			0x01020100//费率1最大需量及发生时间
#define CON_DI64507_MPUNDIRPOWMAXDEMTIM_FEE2			0x01020200//费率2最大需量及发生时间
#define CON_DI64507_MPUNDIRPOWMAXDEMTIM_FEE3			0x01020300//费率3最大需量及发生时间
#define CON_DI64507_MPUNDIRPOWMAXDEMTIM_FEE4			0x01020400//费率4最大需量及发生时间
#define CON_DI64507_MPUNDIRPOWMAXDEMTIM_BLOCK			0x0102FF00
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPDIRVARMAXDEMTIM_BLOCK             0x0103FF00//正向无功总最大需量及发生时间
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPUNDIRVARMAXDEMTIM_BLOCK           0x0104FF00//反向无功总最大需量及发生时间
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPDIRPOWMAXDEMTIM_LM				0x01010001//正向有功总最大需量及发生时间（上月）
#define CON_DI64507_MPDIRPOWMAXDEMTIM_LM_FEE1			0x01010101//费率1最大需量及发生时间（上月）
#define CON_DI64507_MPDIRPOWMAXDEMTIM_LM_FEE2			0x01010201//费率2最大需量及发生时间（上月）
#define CON_DI64507_MPDIRPOWMAXDEMTIM_LM_FEE3			0x01010301//费率3最大需量及发生时间（上月）
#define CON_DI64507_MPDIRPOWMAXDEMTIM_LM_FEE4			0x01010401//费率4最大需量及发生时间（上月）
#define CON_DI64507_MPDIRPOWMAXDEMTIM_LM_BLOCK			0x0101FF01
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPUNDIRPOWMAXDEMTIM_LM				0x01020001//反向有功总最大需量及发生时间（上月）
#define CON_DI64507_MPUNDIRPOWMAXDEMTIM_LM_FEE1			0x01020101//费率1
#define CON_DI64507_MPUNDIRPOWMAXDEMTIM_LM_FEE2			0x01020201//费率2
#define CON_DI64507_MPUNDIRPOWMAXDEMTIM_LM_FEE3			0x01020301//费率3
#define CON_DI64507_MPUNDIRPOWMAXDEMTIM_LM_FEE4			0x01020401//费率4
#define CON_DI64507_MPUNDIRPOWMAXDEMTIM_LM_BLOCK		0x0102FF01
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPDIRVARMAXDEMTIM_LM_BLOCK          0x0103FF01//正向无功总最大需量及发生时间（上月）
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPUNDIRVARMAXDEMTIM_LM_BLOCK        0x0104FF01//反向无功总最大需量及发生时间（上月）
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPVOLA								0x02010100//A相电压
#define CON_DI64507_MPVOLB								0x02010200//B相电压
#define CON_DI64507_MPVOLC								0x02010300//C相电压
#define CON_DI64507_MPVOL_BLOCK							0x0201FF00//电压数据块

#define CON_DI64507_MPCURA								0x02020100//A相电流
#define CON_DI64507_MPCURB								0x02020200//B相电流
#define CON_DI64507_MPCURC								0x02020300//C相电流 
#define CON_DI64507_MPCUR_BLOCK							0x0202FF00//电流数据块

#define CON_DI64507_MPINSTANTPOW						0x02030000//瞬时有功功率
#define CON_DI64507_MPINSTANTPOWA						0x02030100//A相有功功率
#define CON_DI64507_MPINSTANTPOWB						0x02030200//B相有功功率
#define CON_DI64507_MPINSTANTPOWC						0x02030300//C相有功功率
#define CON_DI64507_MPINSTANTPOW_BLOCK					0x0203FF00//

#define CON_DI64507_MPINSTANTVAR						0x02040000//瞬时无功功率
#define CON_DI64507_MPINSTANTVARA						0x02040100//A相无功功率
#define CON_DI64507_MPINSTANTVARB						0x02040200//B相无功功率
#define CON_DI64507_MPINSTANTVARC						0x02040300//C相无功功率
#define CON_DI64507_MPINSTANTVAR_BLOCK					0x0204FF00//
/*
#define CON_DI64507_MPPOWERFACTOR						0x02060000//总功率因数
#define CON_DI64507_MPPOWERFACTORA						0x02060100//A
#define CON_DI64507_MPPOWERFACTORB						0x02060200//B
#define CON_DI64507_MPPOWERFACTORC						0x02060300//C
#define CON_DI64507_MPPOWERFACTOR_BLOCK					0x0206FF00//
*/
#define CON_DI64507_MPBATWORKTIME                       0x0280000A//电池时间

#define CON_DI64507_MPPOWCONST							0x04000409//有功常数
#define CON_DI64507_MPVARCONST							0x0400040A//无功常数

#define CON_DI64507_MPFEERATE_BLOCK						0x04010001	//费率
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPDATEANDWEEK						0x04000101//日期及周次
#define CON_DI64507_MPTIME								0x04000102//时间 yzy


#define CON_DI64507_MPMETERSTATFLAG_BLOCK				0x040005FF
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPCOPYDAYTIME						0x04000B01//抄表日

#define CON_DI64507_MPLATELYPROGTIME					0x03300001//最近一次编程时间
#define CON_DI64507_MPLATELYMAXDEMANDCLEARTIME			0x03300201//最近一次最大需量清零时间
#define CON_DI64507_MPPROGTIMETIMES						0x03300000//编程次数
#define CON_DI64507_MPMAXDEMANDCLEARTIMES				0x03300200//最大需量清零次数

#define CON_DI64507_MPMETERCLRNUM						0X03300100//电表清0总次数
#define CON_DI64507_MPMETERCLRTIME						0X03300101//最近一次电表清0时间


#define CON_DI64507_MPEVENTCLRNUM						0X03300300//事件清0总次数
#define CON_DI64507_MPEVENTCLRTIME						0X03300301//最近一次事件清0时间

#define CON_DI64507_MPJIAOSHINUM						0X03300400//校时总次数
#define CON_DI64507_MPJIAOSHITIME						0X03300401//最近一次校时时间

//#define CON_DI64507_MPBREAKNOTIM_BLOCK					0x03040000//断相次数、累计时间 块
#define CON_DI64507_MPBREAKNUMA							0x13010001		//断相次数A
#define CON_DI64507_MPBREAKNUMB							0x13020001		//断相次数B
#define CON_DI64507_MPBREAKNUMC							0x13030001		//断相次数C
#define CON_DI64507_MPBREAKTOTALTIMEA					0x13010002		//断相累计时间A
#define CON_DI64507_MPBREAKTOTALTIMEB					0x13020002		//断相累计时间B
#define CON_DI64507_MPBREAKTOTALTIMEC					0x13030002		//断相累计时间C

#define CON_DI64507_MPBREAKBEGTIME_A					0x13010101		//最近一次断相起始时间A
#define CON_DI64507_MPBREAKBEGTIME_B					0x13020101		//最近一次断相起始时间B
#define CON_DI64507_MPBREAKBEGTIME_C					0x13030101		//最近一次断相起始时间C
#define CON_DI64507_MPBREAKENDTIME_A					0x13012501		//最近一次断相结束时间A
#define CON_DI64507_MPBREAKENDTIME_B					0x13022501		//最近一次断相结束时间B
#define CON_DI64507_MPBREAKENDTIME_C					0x13032501		//最近一次断相结束时间C
//#define CON_DI64507_MPVOLBREAKA_BEGEND					0x03040101//最近断相起始时间结束时间A		
//#define CON_DI64507_MPVOLBREAKB_BEGEND					0x03040201//B
//#define CON_DI64507_MPVOLBREAKC_BEGEND					0x03040301//C
//扩展
#define CON_DI64507_MPVOLBREAKALL_BEG					0x03040401//最近总断相起始时间		
#define CON_DI64507_MPVOLBREAKALL_END					0x03040402//最近总断相结束时间

#define CON_DI64507_MPCUFE_BLOCK								0x04000DFF//

#define CON_DI64507_MPDIRPOWA							0x00150000
#define CON_DI64507_MPUNDIRPOWA							0x00160000
#define CON_DI64507_MPCOMVAR1A							0x00170000
#define CON_DI64507_MPCOMVAR2A							0x00180000

#define CON_DI64507_MPDIRPOWA_LM							0x00150001
#define CON_DI64507_MPUNDIRPOWA_LM						0x00160001
#define CON_DI64507_MPCOMVAR1A_LM						0x00170001
#define CON_DI64507_MPCOMVAR2A_LM						0x00180001

#define CON_DI64507_MPDIRPOWB								0x00290000
#define CON_DI64507_MPUNDIRPOWB							0x002A0000
#define CON_DI64507_MPCOMVAR1B							0x002B0000
#define CON_DI64507_MPCOMVAR2B							0x002C0000

#define CON_DI64507_MPDIRPOWB_LM							0x00290001
#define CON_DI64507_MPUNDIRPOWB_LM						0x002A0001
#define CON_DI64507_MPCOMVAR1B_LM						0x002B0001
#define CON_DI64507_MPCOMVAR2B_LM						0x002C0001

#define CON_DI64507_MPDIRPOWC								0x003D0000
#define CON_DI64507_MPUNDIRPOWC							0x003E0000
#define CON_DI64507_MPCOMVAR1C							0x003F0000
#define CON_DI64507_MPCOMVAR2C							0x00400000

#define CON_DI64507_MPDIRPOWC_LM							0x003D0001
#define CON_DI64507_MPUNDIRPOWC_LM						0x003E0001
#define CON_DI64507_MPCOMVAR1C_LM						0x003F0001
#define CON_DI64507_MPCOMVAR2C_LM						0x00400001


/********************************函数声明************************************/
__EXTERN void	DL64507_Protocol_Init(void);
//发送规约解释处理函数
__EXTERN unsigned char Meter_DI64507_DownExplain_Manage(STRS485TASK *pstRS485Task,unsigned char ucMeterNo,unsigned char *pSendBuffer,unsigned char ucPos);

//接收规约解释处理函数
__EXTERN unsigned char Meter_DI64507_UpExplain_Manage(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum);

//正向（反向）、有功（无功）电量（上月）
__EXTERN unsigned char RS485_RecvGet07_Energy_Explain(STRS485TASK *pstRS485Task,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI);
//正向（反向）有功(无功)最大需量及发生时间
__EXTERN unsigned char RS485_RecvGet07_MaxDemTim_Explain(STRS485TASK *pstRS485Task,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI);

//瞬时量
__EXTERN unsigned char RS485_RecvGet07_IPer_Explain(STRS485TASK *pstRS485Task,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI);
__EXTERN unsigned char RS485_RecvGet07_IABC_Explain(STRS485TASK *pstRS485Task,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum);
__EXTERN unsigned char RS485_RecvGet07_UPer_Explain(STRS485TASK *pstRS485Task,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI);
__EXTERN unsigned char RS485_RecvGet07_UABC_Explain(STRS485TASK *pstRS485Task,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum);
__EXTERN unsigned char RS485_RecvGet07_InstantPowPer_Explain(STRS485TASK *pstRS485Task,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI);
__EXTERN unsigned char RS485_RecvGet07_InstantPow_Explain(STRS485TASK *pstRS485Task,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI);
////其它规约解释
__EXTERN unsigned char RS485_RecvGet07_MeterConst_Explain(STRS485TASK *pstRS485Task,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI);
__EXTERN unsigned char RS485_RecvGet07_ProgramTime_Explain(STRS485TASK *pstRS485Task,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI);
__EXTERN unsigned char RS485_RecvGet07_ProgramTimes_Explain(STRS485TASK *pstRS485Task,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI);
__EXTERN unsigned char RS485_RecvGet07_BATWORKTIME_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum);
__EXTERN unsigned char RS485_RecvGet07_JIAOSHITIME_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum);
__EXTERN unsigned char RS485_RecvGet07_CopyDay_Explain(STRS485TASK *pstRS485Task,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum);
__EXTERN unsigned char RS485_RecvGet07_MeterStatFlag_Explain(STRS485TASK *pstRS485Task,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum);
__EXTERN unsigned char RS485_RecvGet07_FeeRate_Explain(STRS485TASK *pstRS485Task,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum);
__EXTERN unsigned char RS485_RecvGet07_Time_Explain(STRS485TASK *pstRS485Task,unsigned char * pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum);
__EXTERN unsigned char RS485_RecvGet07_DateAndWeek_Explain(STRS485TASK *pstRS485Task,unsigned char * pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum);
__EXTERN unsigned char RS485_RecvGet07_LostVolNumTime_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI);
__EXTERN unsigned char RS485_RecvGet07_LastVolOffBegEnd_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI);
__EXTERN unsigned char RS485_RecvGet07_CuFe_Explain(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum);

//辅助函数
__EXTERN unsigned char Fill_PowerDataList(unsigned char *pucRecvContent,unsigned char ucRecvContentLen);
__EXTERN unsigned char Fill_DataList_Byte6(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char ucContentNum);
__EXTERN unsigned char Fill_DataList_Byte4(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char ucContentNum);
__EXTERN unsigned char Fill_DataList_DateTime07(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char ucContentNum);
__EXTERN unsigned char GetRecvContent07(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char *pucRecvBuffer,unsigned short usRecvLen);
__EXTERN unsigned char GetDI64507( unsigned long *usDestDI,unsigned short usSourDI);
__EXTERN unsigned char Fill_DataList_MAXDEMTIIM(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char ucContentNum);

__EXTERN unsigned char DL645_Protocol_CheckFrame(unsigned char *pRecvBuffer,unsigned short usRecvLen,unsigned short *pusJumpLen);
#endif

/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Alarm_Manage_Ter.h
 Description    : 终端自身相关告警处理
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/

#ifndef _ALARM_MANAGE_TER_H_
#define _ALARM_MANAGE_TER_H_

typedef struct _STTERALARMJUDGEINFO
{
	STDATETIME stAlrNewTime_PolarityReverseA;	//A相电流反极性
	STDATETIME stAlrNewTime_PolarityReverseB;	//B相电流反极性
	STDATETIME stAlrNewTime_PolarityReverseC;	//C相电流反极性

	STDATETIME stAlrNewTime_ShortCircuitA1;		//A相1次侧CT短路
	STDATETIME stAlrNewTime_ShortCircuitB1;		//B相1次侧CT短路
	STDATETIME stAlrNewTime_ShortCircuitC1;		//C相1次侧CT短路

	STDATETIME stAlrNewTime_ShortCircuitA2;		//A相2次侧CT短路
	STDATETIME stAlrNewTime_ShortCircuitB2;		//B相2次侧CT短路
	STDATETIME stAlrNewTime_ShortCircuitC2;		//C相2次侧CT短路

	STDATETIME stAlrNewTime_OpenCircuitA2;		//A相2次侧CT开路
	STDATETIME stAlrNewTime_OpenCircuitB2;		//B相2次侧CT开路
	STDATETIME stAlrNewTime_OpenCircuitC2;		//C相2次侧CT开路

	STDATETIME stAlrNewTime_VolABreak;			//A相电压断相
	STDATETIME stAlrNewTime_VolBBreak;			//B相电压断相
	STDATETIME stAlrNewTime_VolCBreak;			//C相电压断相

	STDATETIME stAlrNewTime_VolALack;			//A相电压缺相
	STDATETIME stAlrNewTime_VolBLack;			//B相电压缺相
	STDATETIME stAlrNewTime_VolCLack;			//C相电压缺相

	STDATETIME stAlrNewTime_VolCirReverse;		//电压回路逆相序

	STDATETIME stAlrNewTime_IUnBalance;			//电流不平衡度越限
	STDATETIME stAlrNewTime_UUnBalance;			//电压不平衡度越限
	
	STDATETIME stAlrNewTime_VolAErrDown;		//A相电压越下下限
	STDATETIME stAlrNewTime_VolBErrDown;		//B相电压越下下限
	STDATETIME stAlrNewTime_VolCErrDown;		//C相电压越下下限

	STDATETIME stAlrNewTime_VolAErrUp;			//A相电压越上上限
	STDATETIME stAlrNewTime_VolBErrUp;			//B相电压越上上限
	STDATETIME stAlrNewTime_VolCErrUp;			//C相电压越上上限

	STDATETIME stAlrNewTime_CurAErrUp;			//A相电流越上上限
	STDATETIME stAlrNewTime_CurBErrUp;			//B相电流越上上限
	STDATETIME stAlrNewTime_CurCErrUp;			//C相电流越上上限

	STDATETIME stAlrNewTime_CurANorUp;			//A相电流越上限
	STDATETIME stAlrNewTime_CurBNorUp;			//B相电流越上限
	STDATETIME stAlrNewTime_CurCNorUp;			//C相电流越上限

	STDATETIME stAlrNewTime_SPowNorUp;			//视在功率越上限
	STDATETIME stAlrNewTime_SPowErrUp;			//视在功率越上上限
}STTERALARMJUDGEINFO;

__EXTERN unsigned char Alarm_Manage_Ter(unsigned char ucAlrExeNo);
__EXTERN unsigned char Alarm_Manage_Ter_QuickPart(void);
//__EXTERN unsigned char Alarm_Init_Ter(void);

#endif

/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Alarm_Manage_Ter.h
 Description    : �ն�������ظ澯����
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-02-05    azh     the original version
******************************************************************************/

#ifndef _ALARM_MANAGE_TER_H_
#define _ALARM_MANAGE_TER_H_

typedef struct _STTERALARMJUDGEINFO
{
	STDATETIME stAlrNewTime_PolarityReverseA;	//A�����������
	STDATETIME stAlrNewTime_PolarityReverseB;	//B�����������
	STDATETIME stAlrNewTime_PolarityReverseC;	//C�����������

	STDATETIME stAlrNewTime_ShortCircuitA1;		//A��1�β�CT��·
	STDATETIME stAlrNewTime_ShortCircuitB1;		//B��1�β�CT��·
	STDATETIME stAlrNewTime_ShortCircuitC1;		//C��1�β�CT��·

	STDATETIME stAlrNewTime_ShortCircuitA2;		//A��2�β�CT��·
	STDATETIME stAlrNewTime_ShortCircuitB2;		//B��2�β�CT��·
	STDATETIME stAlrNewTime_ShortCircuitC2;		//C��2�β�CT��·

	STDATETIME stAlrNewTime_OpenCircuitA2;		//A��2�β�CT��·
	STDATETIME stAlrNewTime_OpenCircuitB2;		//B��2�β�CT��·
	STDATETIME stAlrNewTime_OpenCircuitC2;		//C��2�β�CT��·

	STDATETIME stAlrNewTime_VolABreak;			//A���ѹ����
	STDATETIME stAlrNewTime_VolBBreak;			//B���ѹ����
	STDATETIME stAlrNewTime_VolCBreak;			//C���ѹ����

	STDATETIME stAlrNewTime_VolALack;			//A���ѹȱ��
	STDATETIME stAlrNewTime_VolBLack;			//B���ѹȱ��
	STDATETIME stAlrNewTime_VolCLack;			//C���ѹȱ��

	STDATETIME stAlrNewTime_VolCirReverse;		//��ѹ��·������

	STDATETIME stAlrNewTime_IUnBalance;			//������ƽ���Խ��
	STDATETIME stAlrNewTime_UUnBalance;			//��ѹ��ƽ���Խ��
	
	STDATETIME stAlrNewTime_VolAErrDown;		//A���ѹԽ������
	STDATETIME stAlrNewTime_VolBErrDown;		//B���ѹԽ������
	STDATETIME stAlrNewTime_VolCErrDown;		//C���ѹԽ������

	STDATETIME stAlrNewTime_VolAErrUp;			//A���ѹԽ������
	STDATETIME stAlrNewTime_VolBErrUp;			//B���ѹԽ������
	STDATETIME stAlrNewTime_VolCErrUp;			//C���ѹԽ������

	STDATETIME stAlrNewTime_CurAErrUp;			//A�����Խ������
	STDATETIME stAlrNewTime_CurBErrUp;			//B�����Խ������
	STDATETIME stAlrNewTime_CurCErrUp;			//C�����Խ������

	STDATETIME stAlrNewTime_CurANorUp;			//A�����Խ����
	STDATETIME stAlrNewTime_CurBNorUp;			//B�����Խ����
	STDATETIME stAlrNewTime_CurCNorUp;			//C�����Խ����

	STDATETIME stAlrNewTime_SPowNorUp;			//���ڹ���Խ����
	STDATETIME stAlrNewTime_SPowErrUp;			//���ڹ���Խ������
}STTERALARMJUDGEINFO;

__EXTERN unsigned char Alarm_Manage_Ter(unsigned char ucAlrExeNo);
__EXTERN unsigned char Alarm_Manage_Ter_QuickPart(void);
//__EXTERN unsigned char Alarm_Init_Ter(void);

#endif

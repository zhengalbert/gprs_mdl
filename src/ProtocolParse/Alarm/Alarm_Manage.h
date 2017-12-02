/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Alarm_Timer_Response.h
 Description    : ��ʱˢ�����ݸ��µ�ȫ������
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-02-05    azh     the original version
******************************************************************************/
#ifndef _ALARM_MANAGE_H
#define _ALARM_MANAGE_H

#define CON_COPYERR_TIMES		5	
//----------------------------------
#define		CONALRITEMNUM		8				//Ԥ�ж���������
#define		CONALRTAKEITEMNUM	10				//�澯Я��������

#define		CONALR_METER_TIMEINTER		15		//����ʱ��������λ������

#define		CONALR_UPFLAG_NONE			0xFF		//������
#define		CONALR_UPFLAG_NEED			0x11		//Ҫ���ͣ����ǻ�û������
#define		CONALR_UPFLAG_DOING			0x22		//��������
#define		CONALR_UPFLAG_OK			0x33		//�Ѿ��ɹ�����

#define		CON_UPFLOAT_COEF		101		//�澯���ϸ�ϵ��
#define		CON_DOWNFLOAT_COEF		99		//�澯���¸�ϵ��

#define		CON_SUMGROUP_ALR_NUM_INSTANT	4	//�ܼ�˲ʱ��Խ���жϴ���
#define		CON_SUMGROUP_ALR_NUM_POW		4	//�ܼӵ���Խ���жϴ���
#define		CON_ALR_TIMEINTER				10	//�ն��ж�ʱ��������λ0.5��
#define		CON_ALR_TIMEINTER_COEF			120	//�ն˶�ʱ����������ֵ����λ��

#define		CON_ALRACTION_METERCOPY		30		//�ϵ糭���ӳ�ʱ�䣬��λ0.5��
#define		CON_ALRACTION_ALRUP			120		//�ϵ�澯�����ӳ�ʱ�䣬��λ0.5��
//-------------------------------------------------
#define		CONALRRECORD_VALID  0xA5    //flash����ļ�¼Ч��ʶ

#define		CONALRCODE_INVALID  0xFFFF
#define		CONALRFOREREPLYNUM	3				//�澯ȷ�ϲ��䳭�����
#define		CONALRFORELISTNUM	5//10				//�̶�10��(ͬʱ������������)(Ԥ�ж���)�������Ӧ��Ҫ��Ӧ
#define		CONALRENSURELISTNUM	5//10				//�̶�10��(ͬʱ������������)(ȷ�϶���)

#define		CONALRRECITEMNUM	10				//�澯��¼Я��������
#define		CONALRRECORDNUM_IMPORTANT	255		//��Ҫ�澯��־255��
#define		CONALRRECORDNUM_NORMAL		255		//һ��澯��־255��
#define		CONALRRECORDNUM_NEEDACK		16//����֡���0~15 azh 255		//�澯ȷ��255��

#define		CONALRCODE_INVALID			0xFFFF
#define		CONALRCODE_ALLDATAINIT		0x2001		//ERC1�����ݳ�ʼ��
#define		CONALRCODE_PARA_LOST		0x2002		//ERC2��������ʧ
#define		CONALRCODE_PARA_MODIFY		0x2003		//ERC3����Ҫ�������
#define		CONALRCODE_SOFTWARECHANGE	0x2004		//ERC1��������
#define		CONALRCODE_COMM_PASSWORDERR	0x2005		//ERC20:�������

#define		CONALRCODE_TERERR_RAM		0x2101		//�ն������ڴ����
#define		CONALRCODE_TERERR_CLOCK		0x2102		//ʱ�ӹ���
#define		CONALRCODE_TERERR_MAINCOMM	0x2103		//����ͨ�Ź���
#define		CONALRCODE_TERERR_485ERROR	0x2104		//485�������
#define		CONALRCODE_TERERR_DISPLAY	0x2105		//��ʾ�����

#define		CONALRCODE_VAOVERUP			0x2801		//��·ģ����Խ��_Va
#define		CONALRCODE_VBOVERUP			0x2802		//��·ģ����Խ��_Vb
#define		CONALRCODE_VCOVERUP			0x2803		//��·ģ����Խ��_Vc

#define		CONALRCODE_VAOVERDOWN		0x2811		//��·ģ����Խ��_Va(����)
#define		CONALRCODE_VBOVERDOWN		0x2812		//��·ģ����Խ��_Vb(����)
#define		CONALRCODE_VCOVERDOWN		0x2813		//��·ģ����Խ��_Vc(����)

#define		CONALRCODE_IAOVERUP			0x2804		//��·ģ����Խ��_Ia
#define		CONALRCODE_IBOVERUP			0x2805		//��·ģ����Խ��_Ib
#define		CONALRCODE_ICOVERUP			0x2806		//��·ģ����Խ��_Ic

#define		CONALRCODE_IAERROVERUP		0x2824		//IaԽ������
#define		CONALRCODE_IBERROVERUP		0x2825		//IbԽ������
#define		CONALRCODE_ICERROVERUP		0x2826		//IcԽ������

#define		CONALRCODE_VABREAK			0x2821		//A���ѹ����
#define		CONALRCODE_VBBREAK			0x2822		//B���ѹ����
#define		CONALRCODE_VCBREAK			0x2823		//C���ѹ����

#define		CONALRCODE_VALACK			0x2831		//A���ѹȱ��
#define		CONALRCODE_VBLACK			0x2832		//B���ѹȱ��
#define		CONALRCODE_VCLACK			0x2833		//C���ѹȱ��

#define		CONALRCODE_IUNBLANCE		0x2807		//������ƽ��Խ��
#define		CONALRCODE_UUNBLANCE		0x2808		//��ѹ��ƽ��Խ��

#define		CONALRCODE_SPOWNORUP		0x2901		//����װ�������ޣ�kVA��,���ڹ���
#define		CONALRCODE_SPOWERRUP		0x2902		//����װ���������ޣ�kVA��,���ڹ���

#define		CONALRCODE_PROGRAMTIME		0x2911		//���ʱ�����
#define		CONALRCODE_DEMANDTIMECLR	0x2912		//��������ʱ�����
#define		CONALRCODE_PULSECONST		0x2913		//���峣��
#define		CONALRCODE_COPYDAYTIME		0x2914		//������
#define		CONALRCODE_LOSTVOLNUM		0x2915		//�������
#define		CONALRCODE_FEERATE			0x2916		//���ʸ���
#define		CONALRCODE_DEMANDTIMENUM	0x2917		//������������仯
#define		CONALRCODE_PROGRAMNUM		0x2918		//��̴����仯

#define		CONALRCODE_METERFLY			0x2921		//��Ʒ���
#define		CONALRCODE_METERSTOP		0x2922		//���ͣ��
#define		CONALRCODE_METERDOWN		0x2923		//��Ƶ���

#define		CONALRCODE_METERBATTLOW		0x2924		//��ص�ѹ��
#define		CONALRCODE_METERTIMEERR		0x2925		//���ʱ���쳣

#define		CONALRCODE_SUMINSTANTPOWUP1	0x3001		//˲ʱ�й��ܼ���1Խ����
#define		CONALRCODE_SUMINSTANTPOWUP2	0x3002		//˲ʱ�й��ܼ���2Խ����
#define		CONALRCODE_SUMINSTANTPOWUP3	0x3003		//˲ʱ�й��ܼ���3Խ����
#define		CONALRCODE_SUMINSTANTPOWUP4	0x3004		//˲ʱ�й��ܼ���4Խ����
#define		CONALRCODE_SUMINSTANTPOWUP5	0x3005		//˲ʱ�й��ܼ���5Խ����
#define		CONALRCODE_SUMINSTANTPOWUP6	0x3006		//˲ʱ�й��ܼ���6Խ����
#define		CONALRCODE_SUMINSTANTPOWUP7	0x3007		//˲ʱ�й��ܼ���7Խ����
#define		CONALRCODE_SUMINSTANTPOWUP8	0x3008		//˲ʱ�й��ܼ���8Խ����

#define		CONALRCODE_SUMINSTANTPOWDOWN1	0x3011		//˲ʱ�й��ܼ���1Խ����
#define		CONALRCODE_SUMINSTANTPOWDOWN2	0x3012		//˲ʱ�й��ܼ���2Խ����
#define		CONALRCODE_SUMINSTANTPOWDOWN3	0x3013		//˲ʱ�й��ܼ���3Խ����
#define		CONALRCODE_SUMINSTANTPOWDOWN4	0x3014		//˲ʱ�й��ܼ���4Խ����
#define		CONALRCODE_SUMINSTANTPOWDOWN5	0x3015		//˲ʱ�й��ܼ���5Խ����
#define		CONALRCODE_SUMINSTANTPOWDOWN6	0x3016		//˲ʱ�й��ܼ���6Խ����
#define		CONALRCODE_SUMINSTANTPOWDOWN7	0x3017		//˲ʱ�й��ܼ���7Խ����
#define		CONALRCODE_SUMINSTANTPOWDOWN8	0x3018		//˲ʱ�й��ܼ���8Խ����

#define		CONALRCODE_SUMINSTANTVARUP1	0x3101		//˲ʱ�޹��ܼ���1Խ����
#define		CONALRCODE_SUMINSTANTVARUP2	0x3102		//˲ʱ�޹��ܼ���2Խ����
#define		CONALRCODE_SUMINSTANTVARUP3	0x3103		//˲ʱ�޹��ܼ���3Խ����
#define		CONALRCODE_SUMINSTANTVARUP4	0x3104		//˲ʱ�޹��ܼ���4Խ����
#define		CONALRCODE_SUMINSTANTVARUP5	0x3105		//˲ʱ�޹��ܼ���5Խ����
#define		CONALRCODE_SUMINSTANTVARUP6	0x3106		//˲ʱ�޹��ܼ���6Խ����
#define		CONALRCODE_SUMINSTANTVARUP7	0x3107		//˲ʱ�޹��ܼ���7Խ����
#define		CONALRCODE_SUMINSTANTVARUP8	0x3108		//˲ʱ�޹��ܼ���8Խ����

#define		CONALRCODE_SUMINSTANTVARDOWN1	0x3111		//˲ʱ�޹��ܼ���1Խ����
#define		CONALRCODE_SUMINSTANTVARDOWN2	0x3112		//˲ʱ�޹��ܼ���2Խ����
#define		CONALRCODE_SUMINSTANTVARDOWN3	0x3113		//˲ʱ�޹��ܼ���3Խ����
#define		CONALRCODE_SUMINSTANTVARDOWN4	0x3114		//˲ʱ�޹��ܼ���4Խ����
#define		CONALRCODE_SUMINSTANTVARDOWN5	0x3115		//˲ʱ�޹��ܼ���5Խ����
#define		CONALRCODE_SUMINSTANTVARDOWN6	0x3116		//˲ʱ�޹��ܼ���6Խ����
#define		CONALRCODE_SUMINSTANTVARDOWN7	0x3117		//˲ʱ�޹��ܼ���7Խ����
#define		CONALRCODE_SUMINSTANTVARDOWN8	0x3118		//˲ʱ�޹��ܼ���8Խ����

#define		CONALRCODE_SUMPOWUP1	0x3201		//�й��ܼ���1Խ����
#define		CONALRCODE_SUMPOWUP2	0x3202		//�й��ܼ���2Խ����
#define		CONALRCODE_SUMPOWUP3	0x3203		//�й��ܼ���3Խ����
#define		CONALRCODE_SUMPOWUP4	0x3204		//�й��ܼ���4Խ����
#define		CONALRCODE_SUMPOWUP5	0x3205		//�й��ܼ���5Խ����
#define		CONALRCODE_SUMPOWUP6	0x3206		//�й��ܼ���6Խ����
#define		CONALRCODE_SUMPOWUP7	0x3207		//�й��ܼ���7Խ����
#define		CONALRCODE_SUMPOWUP8	0x3208		//�й��ܼ���8Խ����

#define		CONALRCODE_SUMPOWDOWN1	0x3211		//�й��ܼ���1Խ����
#define		CONALRCODE_SUMPOWDOWN2	0x3212		//�й��ܼ���2Խ����
#define		CONALRCODE_SUMPOWDOWN3	0x3213		//�й��ܼ���3Խ����
#define		CONALRCODE_SUMPOWDOWN4	0x3214		//�й��ܼ���4Խ����
#define		CONALRCODE_SUMPOWDOWN5	0x3215		//�й��ܼ���5Խ����
#define		CONALRCODE_SUMPOWDOWN6	0x3216		//�й��ܼ���6Խ����
#define		CONALRCODE_SUMPOWDOWN7	0x3217		//�й��ܼ���7Խ����
#define		CONALRCODE_SUMPOWDOWN8	0x3218		//�й��ܼ���8Խ����

#define		CONALRCODE_SUMVARUP1	0x3301		//�޹��ܼ���1Խ����
#define		CONALRCODE_SUMVARUP2	0x3302		//�޹��ܼ���2Խ����
#define		CONALRCODE_SUMVARUP3	0x3303		//�޹��ܼ���3Խ����
#define		CONALRCODE_SUMVARUP4	0x3304		//�޹��ܼ���4Խ����
#define		CONALRCODE_SUMVARUP5	0x3305		//�޹��ܼ���5Խ����
#define		CONALRCODE_SUMVARUP6	0x3306		//�޹��ܼ���6Խ����
#define		CONALRCODE_SUMVARUP7	0x3307		//�޹��ܼ���7Խ����
#define		CONALRCODE_SUMVARUP8	0x3308		//�޹��ܼ���8Խ����

#define		CONALRCODE_SUMVARDOWN1	0x3311		//�޹��ܼ���1Խ����
#define		CONALRCODE_SUMVARDOWN2	0x3312		//�޹��ܼ���2Խ����
#define		CONALRCODE_SUMVARDOWN3	0x3313		//�޹��ܼ���3Խ����
#define		CONALRCODE_SUMVARDOWN4	0x3314		//�޹��ܼ���4Խ����
#define		CONALRCODE_SUMVARDOWN5	0x3315		//�޹��ܼ���5Խ����
#define		CONALRCODE_SUMVARDOWN6	0x3316		//�޹��ܼ���6Խ����
#define		CONALRCODE_SUMVARDOWN7	0x3317		//�޹��ܼ���7Խ����
#define		CONALRCODE_SUMVARDOWN8	0x3318		//�޹��ܼ���8Խ����

#define		CONALRCODE_SUMCOMPOVERLIMIT     0x3321 //�ܼӲԽ��,���룺0x3321-0x3328

#define		CONALRCODE_DOOROPEN				0x3401		//�ſ�����
#define		CONALRCODE_DOORCLOSE			0x3411		//�Źر���
#define		CONALRCODE_POWERON				0x3402		//�ϵ籨��
#define		CONALRCODE_POWEROFF				0x3412		//ͣ�籨��

#define		CONALRCODE_SWITCH0OPEN			0x3403							//����0������
#define		CONALRCODE_SWITCH0CLOSE			0x3413							//����0�ر���
#define		CONALRCODE_SWITCH1OPEN			CONALRCODE_SWITCH0OPEN+1		//����1������ 3404
#define		CONALRCODE_SWITCH1CLOSE			CONALRCODE_SWITCH0CLOSE+1		//����1�ر��� 3414
#define		CONALRCODE_SWITCH2OPEN			CONALRCODE_SWITCH0OPEN+2		//����2������ 3405
#define		CONALRCODE_SWITCH2CLOSE			CONALRCODE_SWITCH0CLOSE+2		//����2�ر��� 3415
#define		CONALRCODE_SWITCH3OPEN			CONALRCODE_SWITCH0OPEN+3		//����3������ 3406
#define		CONALRCODE_SWITCH3CLOSE			CONALRCODE_SWITCH0CLOSE+3		//����3�ر��� 3416
#define		CONALRCODE_SWITCH4OPEN			CONALRCODE_SWITCH0OPEN+4		//����4������ 3407
#define		CONALRCODE_SWITCH4CLOSE			CONALRCODE_SWITCH0CLOSE+4		//����4�ر��� 3417
#define		CONALRCODE_SWITCH5OPEN			CONALRCODE_SWITCH0OPEN+5		//����5������ 3408
#define		CONALRCODE_SWITCH5CLOSE			CONALRCODE_SWITCH0CLOSE+5		//����5�ر��� 3418
#define		CONALRCODE_SWITCH6OPEN			CONALRCODE_SWITCH0OPEN+6		//����6������ 3409
#define		CONALRCODE_SWITCH6CLOSE			CONALRCODE_SWITCH0CLOSE+6		//����6�ر��� 3419

#define		CONALRCODE_POLREVERSEA			0x3501							//A�����������
#define		CONALRCODE_POLREVERSEB			0x3502							//B�����������
#define		CONALRCODE_POLREVERSEC			0x3503							//C�����������

#define		CONALRCODE_SHORTCIRA1			0x3504							//A��CTһ�β��·
#define		CONALRCODE_SHORTCIRB1			0x3505							//B��CTһ�β��·
#define		CONALRCODE_SHORTCIRC1			0x3506							//C��CTһ�β��·

#define		CONALRCODE_OPENCIRA2			0x3507							//A��CT���β࿪·
#define		CONALRCODE_OPENCIRB2			0x3508							//B��CT���β࿪·
#define		CONALRCODE_OPENCIRC2			0x3509							//C��CT���β࿪·

#define		CONALRCODE_SHORTCIRA2			0x350A							//A��CT���β��·
#define		CONALRCODE_SHORTCIRB2			0x350B							//B��CT���β��·
#define		CONALRCODE_SHORTCIRC2			0x350C							//C��CT���β��·

#define		CONALRCODE_VOLCIRREVERSE		0x350D							//��ѹ��·������

#define		CONALRCODE_REMOTECTRL			0x3601							//ң����բ��¼
#define		CONALRCODE_POWERCTRL			0x3602							//������բ��¼
#define		CONALRCODE_ENERCTRL				0x3603							//�����բ��¼
#define		CONALRCODE_BUYPOWPARA			0x3611							//�����������

#define		CONALRCODE_ENERALR				0x3612							//ERC23 ��ظ澯�¼�
#define		CONALRCODE_METERCOPY			0x3613							//ERC31 ����ʧ���¼���¼
#define		CONALRCODE_FLUXLIMIT			0x3614							//ERC32 ��ͨ�ų�������¼
#define		CONALRCODE_POWDIFFOVER			0x3615							//ERC28	�����������¼

#define		CONALRCODE_METERSTATECHANGE		0x3701							//ERC33 ���״̬�ֱ��


#define		CONALR_TYPE_IMPORTANT			0x55		//��Ҫ�¼�
#define		CONALR_TYPE_NORMAL				0xAA		//һ���¼�
#define		CONALR_TYPE_NOT_RECORD			0xFF		//�¼�����¼

#define		CON_ALR_STAT_ON			0xAA
#define		CON_ALR_STAT_OFF		0x55
#define		CON_ALR_STAT_MID		0x5A

#define		CON_SWITCH_STAT_OPEN		0x55
#define		CON_SWITCH_STAT_CLOSE		0xAA

/////////////////////////CT���//////////////////////////

#define		CON_WORKMODE_NORMAL		0x55
#define		CON_WORKMODE_TEST		0xAA

#define		CON_CTTYPE_D		0xAA
#define		CON_CTTYPE_C		0x55
//-------------------------------------------------
typedef struct _STALRFOREITEMLIST
{
	STDATETIME		stDateTime;
	STDATAITEM_ALR	stDataItem_alr[CONALRITEMNUM];	//������
}STALRFOREITEMLIST;

typedef struct _STALRFOREJUDGE
{
	unsigned short	usAlrCode;			//�澯����
	unsigned char	ucMPNo;				//�������
	unsigned char	ucAlrJudgeTimes;	//�ظ�����
	unsigned char	ucAlrJudgeCount;	//�ظ����� ������
	unsigned char	ucAlrJudgeCycle;	//�ظ�����(��)
	unsigned char	ucAlrDataItemNum;	//�����ж�������ֵ
	STALRFOREITEMLIST	stAlrDataItem[CONALRFOREREPLYNUM];
}STALRFOREJUDGE;

typedef struct _STALRENSUREJUDGE
{
	unsigned short	usAlrCode;			//�澯����
	unsigned char	ucMPNo;				//�������
	STDATETIME		stDateTime;			//�澯����ʱ��
	unsigned char	ucAlrDataItemNum;	//Я������
	STDATAITEM_ALR	stAlrDataItem[CONALRTAKEITEMNUM];	//������
}STALRENSUREJUDGE;

#define		CONALR_STAT_HAPPEN_NOT	0xAA	//�澯δ�������߻ָ�
#define		CONALR_STAT_HAPPEN		0x55	//�澯����

typedef struct _STALARMSTAT
{
	unsigned char ucAlrStat_Sys_VolaUpOver;			//A���ѹԽ������
	unsigned char ucAlrStat_Sys_VolbUpOver;			//B���ѹԽ������
	unsigned char ucAlrStat_Sys_VolcUpOver;			//C���ѹԽ������
	unsigned char ucAlrStat_Sys_VolaDownOver;		//A���ѹԽ������
	unsigned char ucAlrStat_Sys_VolbDownOver;		//B���ѹԽ������
	unsigned char ucAlrStat_Sys_VolcDownOver;		//C���ѹԽ������
	unsigned char ucAlrStat_Sys_IaOverLoad;			//A�����Խ����
	unsigned char ucAlrStat_Sys_IbOverLoad;			//B�����Խ����
	unsigned char ucAlrStat_Sys_IcOverLoad;			//C�����Խ����

	unsigned char ucAlrStat_Sys_IaErrOverLoad;		//A�����Խ������
	unsigned char ucAlrStat_Sys_IbErrOverLoad;		//B�����Խ������
	unsigned char ucAlrStat_Sys_IcErrOverLoad;		//C�����Խ������

	unsigned char ucAlrStat_Sys_VolaBreak;			//A���ѹ����
	unsigned char ucAlrStat_Sys_VolbBreak;			//B���ѹ����
	unsigned char ucAlrStat_Sys_VolcBreak;			//C���ѹ����

	unsigned char ucAlrStat_Sys_VolaLack;			//A���ѹȱ��
	unsigned char ucAlrStat_Sys_VolbLack;			//B���ѹȱ��
	unsigned char ucAlrStat_Sys_VolcLack;			//C���ѹȱ��

	unsigned char ucAlrStat_Sys_SPowOverUp;			//���ڹ���Խ��
	unsigned char ucAlrStat_Sys_SPowErrUp;			//���ڹ���Խ������

	unsigned char ucAlrStat_Sys_IUnbalance;			//������ƽ��
	unsigned char ucAlrStat_Sys_UUnbalance;			//��ѹ��ƽ��

	unsigned char ucAlrStat_Sys_ProgramTime;		//���ʱ��仯
	unsigned char ucAlrStat_Sys_PulseConst;			//���峣���仯
	unsigned char ucAlrStat_Sys_CopyDayTime;		//�����ձ仯
	unsigned char ucAlrStat_Sys_LostVolNum;			//��������仯
	unsigned char ucAlrStat_Sys_FeeRate;			//���ʸ���
	unsigned char ucAlrStat_Sys_DemandClr;			//��������

//	unsigned char ucAlrStat_Sys_MeterFly;			//��Ʒ���
//	unsigned char ucAlrStat_Sys_MeterStop;			//���ͣ��
//	unsigned char ucAlrStat_Sys_MeterDown;			//��Ƶ���
	unsigned char ucAlrStat_Sys_MeterBattLow;		//��ص�ѹ��
	unsigned char ucAlrStat_Sys_MeterTimeErr;		//���ʱ�����
//	unsigned char ucAlrStat_Sys_MeterPowDiff;		//����������

	unsigned char ucAlrStat_Sys_PolReverseA;		//A�����������
	unsigned char ucAlrStat_Sys_PolReverseB;		//B�����������
	unsigned char ucAlrStat_Sys_PolReverseC;		//C�����������
//
//	unsigned char ucAlrStat_Sys_ShortCirA1;			//A��CTһ�β��·
//	unsigned char ucAlrStat_Sys_ShortCirB1;			//B��CTһ�β��·
//	unsigned char ucAlrStat_Sys_ShortCirC1;			//C��CTһ�β��·
//
//	unsigned char ucAlrStat_Sys_OpenCirA2;			//A��CT���β࿪·
//	unsigned char ucAlrStat_Sys_OpenCirB2;			//B��CT���β࿪·
//	unsigned char ucAlrStat_Sys_OpenCirC2;			//C��CT���β࿪·
//
//	unsigned char ucAlrStat_Sys_ShortCirA2;			//A��CT���β��·
//	unsigned char ucAlrStat_Sys_ShortCirB2;			//B��CT���β��·
//	unsigned char ucAlrStat_Sys_ShortCirC2;			//C��CT���β��·

	unsigned char ucAlrStat_Sys_VolCirReve;			//��ѹ��·������
	unsigned char ucAlrStat_Sys_MainComm;			//ģ��ͨѶ״̬

	unsigned char ucAlrStat_Sys_ProgramCount;		//��̴������
	unsigned char ucAlrStat_Sys_MeterStateChange;		//״̬�ֱ��
}STALARMSTAT;

typedef struct _STSUMALARMSTAT
{
	unsigned char ucAlrStat_Sys_SumInstantPowUp;	//˲ʱ�й��ܼ���Խ����
	unsigned char ucAlrStat_Sys_SumInstantVarUp;	//˲ʱ�޹��ܼ���Խ����
	unsigned char ucAlrStat_Sys_SumPowValUp;		//�й������ܼ���Խ����
	unsigned char ucAlrStat_Sys_SumVarValUp;		//�޹������ܼ���Խ����

	unsigned char ucAlrStat_Sys_SumInstantPowDown;	//˲ʱ�й��ܼ���Խ����
	unsigned char ucAlrStat_Sys_SumInstantVarDown;	//˲ʱ�޹��ܼ���Խ����
	unsigned char ucAlrStat_Sys_SumPowValDown;		//�й������ܼ���Խ����
	unsigned char ucAlrStat_Sys_SumVarValDown;		//�޹������ܼ���Խ����

	unsigned char ucAlrStat_Sys_CompOverLimit;		//�Խ�޸澯
}STSUMALARMSTAT;
//-------------------------------------------
//////////////////////////////////////�澯���ֽṹ����//////////////////////////////////////
//azh 
typedef struct _STALRAUXINFO
{	
	unsigned char   ucUpFlag;            //�����澯��¼״̬ ���������ϱ� û���ϱ� �����ϱ� ���ϱ�
//	unsigned char	ucAlrSendFrameSEQ;	//���͸澯ʱ��Ӧ�ģӣţ� ����ȷ�ϸ澯�ϱ� ���������״̬ azh ����RAM�� Ч�ʸ�
//	STDATETIME		stNextUpDateTime;	//�´��ر�ʱ�� 
    unsigned char   ucUpCount;          //���ڱȽ�ռ�ռ� ����ֻ����һ�����ϱ����� Ҳ����ȷ���´��ϱ��ط�ʱ��
}STALRAUXINFO;//����ÿ���澯��¼������EEPROM�������ȷ�Ϻ��´��ر�ʱ�� ����Ǻ�Flash���¼�����Ӧ��
typedef struct _STPERALRAUXINFO
{	
    STALRAUXINFO    stAlrAuxInfo;
	unsigned char   ucCrc;              //�ۼ�У���(�ۼӺ�+ƫ��)
}STPERALRAUXINFO;

typedef struct _STALRACKINFO
{	
	unsigned char	ucAlrRecordNo;		//�澯��¼��
	unsigned char	ucAlrSendFrameSEQ;	//���͸澯ʱ��Ӧ�ģӣţ�
}STALRNEEDACKINFO;
typedef struct _STALRNEEDACKINFOLIST
{	
	unsigned char	ucList_Head_P;		//����һ��
	unsigned char	ucList_Tail_P;		//�¼���һ��
	STALRNEEDACKINFO	stAlrNeedAckInfoList[CONALRRECORDNUM_NEEDACK];//
}STALRNEEDACKINFOLIST;

typedef struct _STALRNEEDACKINFOGROUP
{
	STALRNEEDACKINFOLIST stAlrNeedAckInfo_Imp;
	STALRNEEDACKINFOLIST stAlrNeedAckInfo_Nor;
}STALRNEEDACKINFOGROUP;

typedef struct _STALRRECORDINDEXINFO
{
	unsigned char	ucList_Head_P;		//����һ��
	unsigned char	ucList_Tail_P;		//�¼���һ��
	unsigned char	ucList_Proc_Tail_P;	//��Ҫ���ʹ���ĵ�һ��
}STALRRECORDINDEXINFO;

typedef struct _STALRRECORDINFO
{
	unsigned char ucAlrValidFlag[8];		//��Ϣ��Ч��־
	STALRRECORDINDEXINFO stAlrRecordInfo_Imp;	//��Ҫ�¼�
	STALRRECORDINDEXINFO stAlrRecordInfo_Norm;	//һ���¼�
}STALRRECORDINFO;

typedef struct _STALRRECORD
{
    unsigned char   ucavailble;         //azh û��У���� ��һ����Ч��־ Ҳ�Ǻõ�
	unsigned char	ucAlrERCCode;		//�澯����
	unsigned char	ucReserve;
	unsigned char	ucMPNo;				//�������
	unsigned char	ucUpFlag;			//���ͱ�־		δ����00����������11���Ѿ�����22��������33
	STDATETIME		stNextUpDateTime;	//�´η���ʱ��
	STDATETIME		stDateTime;			//�澯����ʱ��
	unsigned char	ucAlrDataItemNum;	//Я������
	STDATAITEM_ALR	stAlrDataItem[CONALRRECITEMNUM];	//������
}STALRRECORD;	//~330//558 bytes

typedef struct	_STALRCODEINFO
{
	unsigned char ucMPNo;
	unsigned short	usAlrCode;
}STALRCODEINFO;

typedef struct _STSWITCHALARMINFO
{
	unsigned char	ucValid;
	unsigned char	ucCtrlTurn;				//�ִ�
	unsigned short  usDelayCount;			//������
	unsigned char	ucCtrlType;				//��������	
	unsigned char	ucCtrlCurveNo;			//��������
	double			dCurrVal;				//ִ�е�ʱ�ĵ������߹���
	double			dLastVal;				//���յ������߹���
	double			dLoadFixVal;			//��ֵ
}STSWITCHALARMINFO;
//-------------------------------------------
__EXTERN STDATETIME	g_stTerPowerChangeTime;

__EXTERN unsigned short	usTimerFinishFlag;
__EXTERN unsigned char gucOneLoopFlag;
__EXTERN STCMDFLAG		g_stCmdFlag[CON_METER_NUM];
__EXTERN unsigned short	usAlrActionLockTimer;						//�ϵ�����ӳٿ��Ƽ�����
__EXTERN STALARMSTAT	stAlarmStatus[CON_MAX_MPNUM];				//��ǰ�澯״̬��Ϣ����(��ǰ��)
__EXTERN unsigned short usTimerCopyFlag[CON_METER_NUM];				//�����־

__EXTERN STALRFOREJUDGE		stAlrForeJudgeList[CONALRFORELISTNUM];		//Ԥ�ж���
__EXTERN STALRENSUREJUDGE	stAlrEnsureJudgeList[CONALRENSURELISTNUM];	//ȷ�����

__EXTERN unsigned char	ucMeterCopyErrTimes[CON_MAX_MPNUM];
__EXTERN unsigned char	ucMeterCopyErrAlrFlags[CON_MAX_MPNUM];

__EXTERN STALRRECORDINFO		stAlrRecordInfo;			//�澯��־������Ϣ
__EXTERN STALRNEEDACKINFOGROUP	stAlrNeedAckInfoGroup;		//�澯��Ҫȷ���б�
__EXTERN STALRCATEGORY			stAlrHappenWord;
__EXTERN STALRRECORD	        stAlrRecordTemp;	//�澯��¼��ʱ

__EXTERN unsigned char g_ucEventCount_Important;//��Ҫ�¼�����������Ӧ����һ��
__EXTERN unsigned char g_ucEventCount_Normal;//һ���¼�����������Ӧ����һ��

__EXTERN unsigned char ucMeterStateChangeFlag[CON_METER_NUM][8];

//-----------------------------------------------

__EXTERN unsigned char Alarm_CheckForeJudgeList(unsigned short usAlrCode,unsigned char ucMPNo);
__EXTERN unsigned char Alarm_FillForeJudgeList(unsigned short usAlrCode,unsigned char ucMPNo);
__EXTERN unsigned char Alarm_CheckEnsureJudgeList(unsigned short usAlrCode,unsigned char ucMPNo);
__EXTERN unsigned char Alarm_FillEnsureJudgeList(unsigned short usAlrCode,unsigned char ucMPNo);
__EXTERN unsigned char Clear_AlrForeJudge(STALRFOREJUDGE	*pstAlrForeJudge);
__EXTERN unsigned char Clear_AlrEnsureJudge(STALRENSUREJUDGE	*pstAlrEnsureJudge);

__EXTERN unsigned char Get_Shield_ACD_byAlrCode(unsigned short usAlrCode,unsigned char ucFlag);
//unsigned char Get_ACD_byAlrCode(unsigned short usAlrCode);

__EXTERN STHISVAL	GetUnBalanceVal(STHISVAL stHisA,STHISVAL stHisB,STHISVAL stHisC,unsigned char ucMeterType);

//-------------------------------
__EXTERN unsigned char Alarm_Init(unsigned char ucFlag);
__EXTERN unsigned char Alarm_Init_All(void);

__EXTERN unsigned char Alarm_CopyMeter_Timer(void);
__EXTERN unsigned char Alarm_Manage(void);

__EXTERN unsigned char Fill_AlarmAffirm_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);
__EXTERN unsigned char Fill_AlarmTake_Manage(unsigned char ucMPNo,unsigned char ucNum,unsigned short usDILabel,unsigned char ucIsValid);

__EXTERN unsigned char Alarm_EnsureJudge_AllDataInit(void);
__EXTERN unsigned char Alarm_EnsureJudge_SoftwareChange(void);
//__EXTERN unsigned char Alarm_EnsureJudge_485Err(void);
__EXTERN unsigned char Alarm_EnsureJudge_MeterCopy(unsigned char ucMeterCopyErrFlag,unsigned char ucMPNo);
__EXTERN unsigned char Alarm_EnsureJudge_ParaModify_IDOnly(unsigned char *pucTmp,unsigned char ucMsa);
__EXTERN unsigned char Alarm_EnsureJudge_TerErr_RAM(void);
__EXTERN unsigned char Alarm_EnsureJudge_TerErr_Clock(void);
__EXTERN unsigned char Alarm_EnsureJudge_TerErr_MainComm(void);
__EXTERN unsigned char Alarm_EnsureJudge_TerErr_Display(void);
//__EXTERN unsigned char Alarm_EnsureJudge_PasswordErr(unsigned char *pucPass,unsigned char ucMsa);
__EXTERN unsigned char Alarm_EnsureJudge_OverFluxLimit(unsigned long ulCurrFlux,unsigned long ulFluxLimit);

//__EXTERN unsigned char Alarm_EnsureJudge_RemoteCtrl(STSWITCHALARMINFO *pstSwitchAlarmInfo);
//__EXTERN unsigned char Alarm_EnsureJudge_PowerCtrl(STSWITCHALARMINFO *pstSwitchAlarmInfo);
//__EXTERN unsigned char Alarm_EnsureJudge_EnergyCtrl(STSWITCHALARMINFO *pstSwitchAlarmInfo);
//__EXTERN unsigned char Alarm_EnsureJudge_BuyPowEnerRec(unsigned char *pstBuyEnergyRecPtr,double dLeftVal_befor,double dLeftVal_after);
//__EXTERN unsigned char Alarm_EnsureJudge_EnergyAlr(unsigned char ucEnerType,unsigned char ucCtrlTurn,double dEnerVal,double dFixVal);

__EXTERN unsigned char Alarm_InitAlrStatus(void);
__EXTERN unsigned char Alarm_InitAlrHappenWord(void);
//__EXTERN unsigned char Alarm_InitAlrRecordInfo(void);
__EXTERN unsigned char Alarm_Init_AlrNeedAckInfoGroup(void);
__EXTERN unsigned char Alarm_DeleteAlrNeedAckInfo(unsigned char ucAlrRecordNo,unsigned char ucAlrType);
__EXTERN unsigned char DeleteAlrNeedAckInfo(STALRNEEDACKINFOLIST *pstAlrNeedAckInfo,unsigned char ucAlrRecordNo);
__EXTERN unsigned char Alarm_QueryRecord(unsigned char ucAlrRecordNo,STALRRECORD *pstAlrRecord,unsigned char ucAlrType);
__EXTERN unsigned char ClearAlrRecord(STALRRECORD *pstAlrRecord);
__EXTERN unsigned char ClearAlrNeedAckInfoList(STALRNEEDACKINFOLIST *pstAlrNeedAckInfoList);

__EXTERN unsigned char Alarm_DownRecv_Manage(unsigned char ucSEQ,unsigned char ucAlrType);
__EXTERN unsigned char Alarm_SetAlrRecordFlag(unsigned char ucAlrRecordNo,unsigned char ucAlrType);
//__EXTERN void Get_AlrCode_Newest(unsigned char ucResetFlag,unsigned short *pusAlrCode,unsigned char *pucMPNo);
__EXTERN unsigned long  GetUnBalanceVal_byVal(unsigned long ulValA,unsigned long ulValB,unsigned long ulValC,unsigned char ucMeterType);

__EXTERN void TimeChange_AlarmManage(STDATETIME stOldTime,STDATETIME stNewTime);
__EXTERN void CopyMeter_Timer_StartNow(void);
__EXTERN void Set_AlrActionLockTimer(unsigned short usLockTimer);

__EXTERN unsigned short Alarm_GetFrameACD(void);
__EXTERN void Alarm_SetFrameACD(unsigned short usACD);

__EXTERN unsigned char Alarm_GetAlrERCNo(unsigned short usAlrCode);
//__EXTERN unsigned char Get_CurrentDoorStat(void);
//__EXTERN unsigned char Get_CurrentSwitchStat(unsigned char ucSwitchNo);
//__EXTERN unsigned char Get_Switch_Change_Info(void);
//__EXTERN unsigned char Set_Switch_Change_Info(unsigned char ucInfo);
//__EXTERN unsigned char Init_Switch_Change_Info(void);
__EXTERN STDATETIME Get_TerPowerChangeTime(void);
__EXTERN unsigned char Set_TerPowerChangeTime(STDATETIME *pstTime);
__EXTERN STCTVALUE GetCTCheckVal(void);

__EXTERN unsigned char Alarm_FillRS485MeterDi(unsigned char ucTaskSource);
__EXTERN void Set_Alarm_CopyMeter_Flag(unsigned char uctasktype, unsigned char uctaskno, unsigned short flag);
__EXTERN void Clr_Alarm_CopyMeter_Flag(unsigned char uctasktype, unsigned char uctaskno);
__EXTERN void Alarm_Get_Last_TerTime(void);
__EXTERN void Alarm_Save_Last_TerTime(void);
__EXTERN void MeterCopy_Proc_Check(void);

#endif

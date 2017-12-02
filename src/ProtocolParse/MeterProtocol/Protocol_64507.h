/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Protocol_64507.h
 Description    : DLT-645��Լ������ϸ����
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-08-05    azh     the original version
******************************************************************************/
#ifndef _PROTOCOL_64507_H_
#define _PROTOCOL_64507_H_

////////////////////////////////////////////////////////////////////////////////////��������
#define CON_DI64507_MPDIRPOWTOTAL						0x00010000//�����й��ܵ���
#define CON_DI64507_MPDIRPOWPLUS						0x00010100//�����й������
#define CON_DI64507_MPDIRPOWAPEX						0x00010200//�����й������
#define CON_DI64507_MPDIRPOWFLAT						0x00010300//�����й�ƽ����
#define CON_DI64507_MPDIRPOWVALE						0x00010400//�����й��ȵ���
#define CON_DI64507_MPDIRPOW_BLOCK						0x0001FF00//�����й��������ݿ�

#define CON_DI64507_MPUNDIRPOWTOTAL						0x00020000//�����й��ܵ���
#define CON_DI64507_MPUNDIRPOWPLUS						0x00020100//�����й������
#define CON_DI64507_MPUNDIRPOWAPEX						0x00020200//�����й������
#define CON_DI64507_MPUNDIRPOWFLAT						0x00020300//�����й�ƽ����
#define CON_DI64507_MPUNDIRPOWVALE						0x00020400//�����й��ȵ���
#define CON_DI64507_MPUNDIRPOW_BLOCK 					0x0002FF00//�����й��������ݿ�

#define CON_DI64507_MPDIRVARTOTAL						0x00030000//����޹�1�ܵ���
#define CON_DI64507_MPDIRVARPLUS						0x00030100//����޹�1�����
#define CON_DI64507_MPDIRVARAPEX						0x00030200//����޹�1�����
#define CON_DI64507_MPDIRVARFLAT						0x00030300//����޹�1ƽ����
#define CON_DI64507_MPDIRVARVALE						0x00030400//����޹�1�ȵ���
#define CON_DI64507_MPDIRVAR_BLOCK						0x0003FF00//����޹�1�������ݿ�

#define CON_DI64507_MPUNDIRVARTOTAL						0x00040000//����޹�2�ܵ���
#define CON_DI64507_MPUNDIRVARPLUS						0x00040100//����޹�2�����
#define CON_DI64507_MPUNDIRVARAPEX						0x00040200//����޹�2�����
#define CON_DI64507_MPUNDIRVARFLAT						0x00040300//����޹�2ƽ����
#define CON_DI64507_MPUNDIRVARVALE						0x00040400//����޹�2�ȵ���
#define CON_DI64507_MPUNDIRVAR_BLOCK					0x0004FF00//����޹�2�������ݿ�
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPDIRPOWTOTAL_LM					0x00010001//�����й��ܵ��ܣ����£�
#define CON_DI64507_MPDIRPOWPLUS_LM						0x00010101//�����й�����ܣ����£�
#define CON_DI64507_MPDIRPOWAPEX_LM						0x00010201//�����й�����ܣ����£�
#define CON_DI64507_MPDIRPOWFLAT_LM						0x00010301//�����й�ƽ���ܣ����£�
#define CON_DI64507_MPDIRPOWVALE_LM						0x00010401//�����й��ȵ��ܣ����£�
#define CON_DI64507_MPDIRPOW_LM_BLOCK					0x0001FF01//�����й��������ݿ飨���£�

#define CON_DI64507_MPUNDIRPOWTOTAL_LM					0x00020001//�����й��ܵ��ܣ����£�
#define CON_DI64507_MPUNDIRPOWPLUS_LM					0x00020101//�����й�����ܣ����£�
#define CON_DI64507_MPUNDIRPOWAPEX_LM					0x00020201//�����й�����ܣ����£�
#define CON_DI64507_MPUNDIRPOWFLAT_LM					0x00020301//�����й�ƽ���ܣ����£�
#define CON_DI64507_MPUNDIRPOWVALE_LM					0x00020401//�����й��ȵ��ܣ����£�
#define CON_DI64507_MPUNDIRPOW_LM_BLOCK 				0x0002FF01//�����й��������ݿ飨���£�

#define CON_DI64507_MPDIRVARTOTAL_LM					0x00030001//����޹�1�ܵ��ܣ����£�
#define CON_DI64507_MPDIRVARPLUS_LM						0x00030101//����޹�1����ܣ����£�
#define CON_DI64507_MPDIRVARAPEX_LM						0x00030201//����޹�1����ܣ����£�
#define CON_DI64507_MPDIRVARFLAT_LM						0x00030301//����޹�1ƽ���ܣ����£�
#define CON_DI64507_MPDIRVARVALE_LM						0x00030401//����޹�1�ȵ��ܣ����£�
#define CON_DI64507_MPDIRVAR_LM_BLOCK					0x0003FF01//����޹�1�������ݿ飨���£�

#define CON_DI64507_MPUNDIRVARTOTAL_LM					0x00040001//����޹�2�ܵ��ܣ����£�
#define CON_DI64507_MPUNDIRVARPLUS_LM					0x00040101//����޹�2����ܣ����£�
#define CON_DI64507_MPUNDIRVARAPEX_LM					0x00040201//����޹�2����ܣ����£�
#define CON_DI64507_MPUNDIRVARFLAT_LM					0x00040301//����޹�2ƽ���ܣ����£�
#define CON_DI64507_MPUNDIRVARVALE_LM					0x00040401//����޹�2�ȵ��ܣ����£�
#define CON_DI64507_MPUNDIRVAR_LM_BLOCK 				0x0004FF01//����޹�2�������ݿ飨���£�
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPVAR1								0x00050000//һ�����޹��ܵ���
#define CON_DI64507_MPVAR1_FEE1							0x00050100//����1
#define CON_DI64507_MPVAR1_FEE2							0x00050200//����2
#define CON_DI64507_MPVAR1_FEE3							0x00050300//����3
#define CON_DI64507_MPVAR1_FEE4							0x00050400//����4
#define CON_DI64507_MPVAR1_BLOCK						0x0005FF00
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPVAR4								0x00080000//�������޹��ܵ���
#define CON_DI64507_MPVAR4_FEE1							0x00080100//����1
#define CON_DI64507_MPVAR4_FEE2							0x00080200//����2
#define CON_DI64507_MPVAR4_FEE3							0x00080300//����3
#define CON_DI64507_MPVAR4_FEE4							0x00080400//����4
#define CON_DI64507_MPVAR4_BLOCK						0x0008FF00
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPVAR2								0x00060000//�������޹��ܵ���
#define CON_DI64507_MPVAR2_FEE1							0x00060100//����1
#define CON_DI64507_MPVAR2_FEE2							0x00060200//����2
#define CON_DI64507_MPVAR2_FEE3							0x00060300//����3
#define CON_DI64507_MPVAR2_FEE4							0x00060400//����4
#define CON_DI64507_MPVAR2_BLOCK						0x0006FF00
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPVAR3								0x00070000//�������޹��ܵ���
#define CON_DI64507_MPVAR3_FEE1							0x00070100//����1
#define CON_DI64507_MPVAR3_FEE2							0x00070200//����2
#define CON_DI64507_MPVAR3_FEE3							0x00070300//����3
#define CON_DI64507_MPVAR3_FEE4							0x00070400//����4
#define CON_DI64507_MPVAR3_BLOCK						0x0007FF00
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPVAR1_LM							0x00050001//һ�����޹��ܵ��ܣ����£�
#define CON_DI64507_MPVAR1_LM_FEE1						0x00050101//����1
#define CON_DI64507_MPVAR1_LM_FEE2						0x00050201//����2
#define CON_DI64507_MPVAR1_LM_FEE3						0x00050301//����3
#define CON_DI64507_MPVAR1_LM_FEE4						0x00050401//����4
#define CON_DI64507_MPVAR1_LM_BLOCK						0x0005FF01
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPVAR4_LM							0x00080001//�������޹��ܵ��ܣ����£�
#define CON_DI64507_MPVAR4_LM_FEE1						0x00080101//����1
#define CON_DI64507_MPVAR4_LM_FEE2						0x00080201//����2
#define CON_DI64507_MPVAR4_LM_FEE3						0x00080301//����3
#define CON_DI64507_MPVAR4_LM_FEE4						0x00080401//����4
#define CON_DI64507_MPVAR4_LM_BLOCK						0x0008FF01
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPVAR2_LM							0x00060001//�������޹��ܵ��ܣ����£�
#define CON_DI64507_MPVAR2_LM_FEE1						0x00060101//����1
#define CON_DI64507_MPVAR2_LM_FEE2						0x00060201//����2
#define CON_DI64507_MPVAR2_LM_FEE3						0x00060301//����3
#define CON_DI64507_MPVAR2_LM_FEE4						0x00060401//����4
#define CON_DI64507_MPVAR2_LM_BLOCK						0x0006FF01
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPVAR3_LM							0x00070001//�������޹��ܵ��ܣ����£�
#define CON_DI64507_MPVAR3_LM_FEE1						0x00070101//����1
#define CON_DI64507_MPVAR3_LM_FEE2						0x00070201//����2
#define CON_DI64507_MPVAR3_LM_FEE3						0x00070301//����3
#define CON_DI64507_MPVAR3_LM_FEE4						0x00070401//����4
#define CON_DI64507_MPVAR3_LM_BLOCK						0x0007FF01
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPDIRPOWMAXDEMTIM					0x01010000//�����й����������������ʱ��
#define CON_DI64507_MPDIRPOWMAXDEMTIM_FEE1				0x01010100//����1�������������ʱ��
#define CON_DI64507_MPDIRPOWMAXDEMTIM_FEE2				0x01010200//����2�������������ʱ��
#define CON_DI64507_MPDIRPOWMAXDEMTIM_FEE3				0x01010300//����3�������������ʱ��
#define CON_DI64507_MPDIRPOWMAXDEMTIM_FEE4				0x01010400//����4�������������ʱ��
#define CON_DI64507_MPDIRPOWMAXDEMTIM_BLOCK				0x0101FF00
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPUNDIRPOWMAXDEMTIM					0x01020000//�����й����������������ʱ��
#define CON_DI64507_MPUNDIRPOWMAXDEMTIM_FEE1			0x01020100//����1�������������ʱ��
#define CON_DI64507_MPUNDIRPOWMAXDEMTIM_FEE2			0x01020200//����2�������������ʱ��
#define CON_DI64507_MPUNDIRPOWMAXDEMTIM_FEE3			0x01020300//����3�������������ʱ��
#define CON_DI64507_MPUNDIRPOWMAXDEMTIM_FEE4			0x01020400//����4�������������ʱ��
#define CON_DI64507_MPUNDIRPOWMAXDEMTIM_BLOCK			0x0102FF00
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPDIRVARMAXDEMTIM_BLOCK             0x0103FF00//�����޹����������������ʱ��
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPUNDIRVARMAXDEMTIM_BLOCK           0x0104FF00//�����޹����������������ʱ��
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPDIRPOWMAXDEMTIM_LM				0x01010001//�����й����������������ʱ�䣨���£�
#define CON_DI64507_MPDIRPOWMAXDEMTIM_LM_FEE1			0x01010101//����1�������������ʱ�䣨���£�
#define CON_DI64507_MPDIRPOWMAXDEMTIM_LM_FEE2			0x01010201//����2�������������ʱ�䣨���£�
#define CON_DI64507_MPDIRPOWMAXDEMTIM_LM_FEE3			0x01010301//����3�������������ʱ�䣨���£�
#define CON_DI64507_MPDIRPOWMAXDEMTIM_LM_FEE4			0x01010401//����4�������������ʱ�䣨���£�
#define CON_DI64507_MPDIRPOWMAXDEMTIM_LM_BLOCK			0x0101FF01
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPUNDIRPOWMAXDEMTIM_LM				0x01020001//�����й����������������ʱ�䣨���£�
#define CON_DI64507_MPUNDIRPOWMAXDEMTIM_LM_FEE1			0x01020101//����1
#define CON_DI64507_MPUNDIRPOWMAXDEMTIM_LM_FEE2			0x01020201//����2
#define CON_DI64507_MPUNDIRPOWMAXDEMTIM_LM_FEE3			0x01020301//����3
#define CON_DI64507_MPUNDIRPOWMAXDEMTIM_LM_FEE4			0x01020401//����4
#define CON_DI64507_MPUNDIRPOWMAXDEMTIM_LM_BLOCK		0x0102FF01
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPDIRVARMAXDEMTIM_LM_BLOCK          0x0103FF01//�����޹����������������ʱ�䣨���£�
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPUNDIRVARMAXDEMTIM_LM_BLOCK        0x0104FF01//�����޹����������������ʱ�䣨���£�
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPVOLA								0x02010100//A���ѹ
#define CON_DI64507_MPVOLB								0x02010200//B���ѹ
#define CON_DI64507_MPVOLC								0x02010300//C���ѹ
#define CON_DI64507_MPVOL_BLOCK							0x0201FF00//��ѹ���ݿ�

#define CON_DI64507_MPCURA								0x02020100//A�����
#define CON_DI64507_MPCURB								0x02020200//B�����
#define CON_DI64507_MPCURC								0x02020300//C����� 
#define CON_DI64507_MPCUR_BLOCK							0x0202FF00//�������ݿ�

#define CON_DI64507_MPINSTANTPOW						0x02030000//˲ʱ�й�����
#define CON_DI64507_MPINSTANTPOWA						0x02030100//A���й�����
#define CON_DI64507_MPINSTANTPOWB						0x02030200//B���й�����
#define CON_DI64507_MPINSTANTPOWC						0x02030300//C���й�����
#define CON_DI64507_MPINSTANTPOW_BLOCK					0x0203FF00//

#define CON_DI64507_MPINSTANTVAR						0x02040000//˲ʱ�޹�����
#define CON_DI64507_MPINSTANTVARA						0x02040100//A���޹�����
#define CON_DI64507_MPINSTANTVARB						0x02040200//B���޹�����
#define CON_DI64507_MPINSTANTVARC						0x02040300//C���޹�����
#define CON_DI64507_MPINSTANTVAR_BLOCK					0x0204FF00//
/*
#define CON_DI64507_MPPOWERFACTOR						0x02060000//�ܹ�������
#define CON_DI64507_MPPOWERFACTORA						0x02060100//A
#define CON_DI64507_MPPOWERFACTORB						0x02060200//B
#define CON_DI64507_MPPOWERFACTORC						0x02060300//C
#define CON_DI64507_MPPOWERFACTOR_BLOCK					0x0206FF00//
*/
#define CON_DI64507_MPBATWORKTIME                       0x0280000A//���ʱ��

#define CON_DI64507_MPPOWCONST							0x04000409//�й�����
#define CON_DI64507_MPVARCONST							0x0400040A//�޹�����

#define CON_DI64507_MPFEERATE_BLOCK						0x04010001	//����
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPDATEANDWEEK						0x04000101//���ڼ��ܴ�
#define CON_DI64507_MPTIME								0x04000102//ʱ�� yzy


#define CON_DI64507_MPMETERSTATFLAG_BLOCK				0x040005FF
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI64507_MPCOPYDAYTIME						0x04000B01//������

#define CON_DI64507_MPLATELYPROGTIME					0x03300001//���һ�α��ʱ��
#define CON_DI64507_MPLATELYMAXDEMANDCLEARTIME			0x03300201//���һ�������������ʱ��
#define CON_DI64507_MPPROGTIMETIMES						0x03300000//��̴���
#define CON_DI64507_MPMAXDEMANDCLEARTIMES				0x03300200//��������������

#define CON_DI64507_MPMETERCLRNUM						0X03300100//�����0�ܴ���
#define CON_DI64507_MPMETERCLRTIME						0X03300101//���һ�ε����0ʱ��


#define CON_DI64507_MPEVENTCLRNUM						0X03300300//�¼���0�ܴ���
#define CON_DI64507_MPEVENTCLRTIME						0X03300301//���һ���¼���0ʱ��

#define CON_DI64507_MPJIAOSHINUM						0X03300400//Уʱ�ܴ���
#define CON_DI64507_MPJIAOSHITIME						0X03300401//���һ��Уʱʱ��

//#define CON_DI64507_MPBREAKNOTIM_BLOCK					0x03040000//����������ۼ�ʱ�� ��
#define CON_DI64507_MPBREAKNUMA							0x13010001		//�������A
#define CON_DI64507_MPBREAKNUMB							0x13020001		//�������B
#define CON_DI64507_MPBREAKNUMC							0x13030001		//�������C
#define CON_DI64507_MPBREAKTOTALTIMEA					0x13010002		//�����ۼ�ʱ��A
#define CON_DI64507_MPBREAKTOTALTIMEB					0x13020002		//�����ۼ�ʱ��B
#define CON_DI64507_MPBREAKTOTALTIMEC					0x13030002		//�����ۼ�ʱ��C

#define CON_DI64507_MPBREAKBEGTIME_A					0x13010101		//���һ�ζ�����ʼʱ��A
#define CON_DI64507_MPBREAKBEGTIME_B					0x13020101		//���һ�ζ�����ʼʱ��B
#define CON_DI64507_MPBREAKBEGTIME_C					0x13030101		//���һ�ζ�����ʼʱ��C
#define CON_DI64507_MPBREAKENDTIME_A					0x13012501		//���һ�ζ������ʱ��A
#define CON_DI64507_MPBREAKENDTIME_B					0x13022501		//���һ�ζ������ʱ��B
#define CON_DI64507_MPBREAKENDTIME_C					0x13032501		//���һ�ζ������ʱ��C
//#define CON_DI64507_MPVOLBREAKA_BEGEND					0x03040101//���������ʼʱ�����ʱ��A		
//#define CON_DI64507_MPVOLBREAKB_BEGEND					0x03040201//B
//#define CON_DI64507_MPVOLBREAKC_BEGEND					0x03040301//C
//��չ
#define CON_DI64507_MPVOLBREAKALL_BEG					0x03040401//����ܶ�����ʼʱ��		
#define CON_DI64507_MPVOLBREAKALL_END					0x03040402//����ܶ������ʱ��

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


/********************************��������************************************/
__EXTERN void	DL64507_Protocol_Init(void);
//���͹�Լ���ʹ�����
__EXTERN unsigned char Meter_DI64507_DownExplain_Manage(STRS485TASK *pstRS485Task,unsigned char ucMeterNo,unsigned char *pSendBuffer,unsigned char ucPos);

//���չ�Լ���ʹ�����
__EXTERN unsigned char Meter_DI64507_UpExplain_Manage(STRS485TASK *pstRS485SendTask,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum);

//���򣨷��򣩡��й����޹������������£�
__EXTERN unsigned char RS485_RecvGet07_Energy_Explain(STRS485TASK *pstRS485Task,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI);
//���򣨷����й�(�޹�)�������������ʱ��
__EXTERN unsigned char RS485_RecvGet07_MaxDemTim_Explain(STRS485TASK *pstRS485Task,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI);

//˲ʱ��
__EXTERN unsigned char RS485_RecvGet07_IPer_Explain(STRS485TASK *pstRS485Task,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI);
__EXTERN unsigned char RS485_RecvGet07_IABC_Explain(STRS485TASK *pstRS485Task,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum);
__EXTERN unsigned char RS485_RecvGet07_UPer_Explain(STRS485TASK *pstRS485Task,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI);
__EXTERN unsigned char RS485_RecvGet07_UABC_Explain(STRS485TASK *pstRS485Task,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum);
__EXTERN unsigned char RS485_RecvGet07_InstantPowPer_Explain(STRS485TASK *pstRS485Task,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI);
__EXTERN unsigned char RS485_RecvGet07_InstantPow_Explain(STRS485TASK *pstRS485Task,unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRecvNum,unsigned short usDI);
////������Լ����
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

//��������
__EXTERN unsigned char Fill_PowerDataList(unsigned char *pucRecvContent,unsigned char ucRecvContentLen);
__EXTERN unsigned char Fill_DataList_Byte6(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char ucContentNum);
__EXTERN unsigned char Fill_DataList_Byte4(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char ucContentNum);
__EXTERN unsigned char Fill_DataList_DateTime07(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char ucContentNum);
__EXTERN unsigned char GetRecvContent07(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char *pucRecvBuffer,unsigned short usRecvLen);
__EXTERN unsigned char GetDI64507( unsigned long *usDestDI,unsigned short usSourDI);
__EXTERN unsigned char Fill_DataList_MAXDEMTIIM(unsigned char *pucRecvContent,unsigned char ucRecvContentLen,unsigned char ucContentNum);

__EXTERN unsigned char DL645_Protocol_CheckFrame(unsigned char *pRecvBuffer,unsigned short usRecvLen,unsigned short *pusJumpLen);
#endif

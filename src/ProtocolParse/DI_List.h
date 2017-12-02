/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : DI_List.h
 Description    : �������ʶ
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-02-05    azh     the original version
******************************************************************************/

#ifndef _DI_LIST_H_
#define _DI_LIST_H_
//#include "public.h"

#define	CONTERMSGNUM					8		//8����Ϣ��
#define	CONTERMSGRETNUM					8		//8����������Ϣ��

#define	CON_TERMSG_SERVER_SENDACK		1		//����/ȷ��
#define	CON_TERMSG_SERVER_SENDNACK		4		//����/���ش�

#define	CON_TERMSG_TYPE_INVALID			0xFF
//��վ���б���
#define	CON_TERMSG_ACK				0x00		//ȷ��
#define	CON_TERMSG_RESET				0x01		//��λ�������
#define	CON_TERMSG_CONNECT			0x02		//��·�ӿڼ��
#define	CON_TERMSG_RELAYCMD			0x03		//�м�վ����
#define	CON_TERMSG_SETPARA			0x04		//��������
#define	CON_TERMSG_CONTROL			0x05		//��������
#define   CON_TERMSG_LOGIN				0x06		//�����֤����ԿЭ��
#define 	CON_TERMSG_DATAREQ_SLAVE	0x08		//���󱻼����ն������ϱ�
#define   CON_TERMSG_CONFIGURATION	0x09		//�ն�����
#define	CON_TERMSG_GETPARA			0x0A		//��ѯ�ն˲���
#define	CON_TERMSG_TASKDATA			0x0B		//�������ݲ�ѯ
#define	CON_TERMSG_REQDATA_1		0x0C		//����1�����ݣ�ʵʱ���ݣ�
#define	CON_TERMSG_REQDATA_2		0x0D		//����2�����ݣ��������ݣ�
#define	CON_TERMSG_REQDATA_3		0x0E		//����3�����ݣ��¼���
#define	CON_TERMSG_FILETRANS		0x0F		//�ļ�����
#define	CON_TERMSG_DATATRANS		0x10		//����ת��
#define	CON_TERMSG_EXTCMD			0xC0		//��չ����û��Զ��壩

//��Ϣ������
#define	CON_INFO_DI_TYPE_TIME			0x0101		//�����ݵ�ʱ��
#define	CON_INFO_DI_TYPE_DINUM			0x0102		//���ݵ���
#define	CON_INFO_DI_TYPE_DIINTER		0x0103		//����ʱ����
#define	CON_INFO_DI_TYPE_ALRCODE		0x0104		//�澯����

//ȷ�Ͻ��
#define	CON_NACK_ERR_NORMAL				0		//��ȷ���޴���
#define	CON_NACK_ERR_NOTEXE				1		//���ݳ�����ִ��
#define	CON_NACK_ERR_PASSWORD			2		//�����
#define	CON_NACK_ERR_NOVALIDDATA		3		//����Ч����
#define CON_ACK_NEEDMETERACK			0x10	//��Ҫ��ȡ������ݺ��Ӧ


/*******************************�������ʶ****************************************/
#define CON_DI_TYPE_INVALID								0xBBBB//��Ч���ݱ�ʶ
#define CON_DI_TYPE_MPRELAY								0x0000//�������м�����
#define CON_DI_TYPE_EXT									0xCDCD//��չ�������ݱ�ʶ
#define	CON_DI_TYPE_MPALLDI								0xFFFF//����

///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPDIRPOWTOTAL						0x9010//�����й��ܵ���
#define CON_DI_TYPE_MPDIRPOWPLUS						0x9011//�����й������
#define CON_DI_TYPE_MPDIRPOWAPEX						0x9012//�����й������
#define CON_DI_TYPE_MPDIRPOWFLAT						0x9013//�����й�ƽ����
#define CON_DI_TYPE_MPDIRPOWVALE						0x9014//�����й��ȵ���
#define CON_DI_TYPE_MPDIRPOW_BLOCK						0x901F//�����й��������ݿ�

#define CON_DI_TYPE_MPUNDIRPOWTOTAL						0x9020//�����й��ܵ���
#define CON_DI_TYPE_MPUNDIRPOWPLUS						0x9021//�����й������
#define CON_DI_TYPE_MPUNDIRPOWAPEX						0x9022//�����й������
#define CON_DI_TYPE_MPUNDIRPOWFLAT						0x9023//�����й�ƽ����
#define CON_DI_TYPE_MPUNDIRPOWVALE						0x9024//�����й��ȵ���
#define CON_DI_TYPE_MPUNDIRPOW_BLOCK 					0x902F//�����й��������ݿ�
///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPDIRPOWTOTAL_LM					0x9410//�����й��ܵ��ܣ����£�
#define CON_DI_TYPE_MPDIRPOWPLUS_LM						0x9411//�����й�����ܣ����£�
#define CON_DI_TYPE_MPDIRPOWAPEX_LM						0x9412//�����й�����ܣ����£�
#define CON_DI_TYPE_MPDIRPOWFLAT_LM						0x9413//�����й�ƽ���ܣ����£�
#define CON_DI_TYPE_MPDIRPOWVALE_LM						0x9414//�����й��ȵ��ܣ����£�
#define CON_DI_TYPE_MPDIRPOW_LM_BLOCK					0x941F//�����й��������ݿ飨���£�

#define CON_DI_TYPE_MPUNDIRPOWTOTAL_LM					0x9420//�����й��ܵ��ܣ����£�
#define CON_DI_TYPE_MPUNDIRPOWPLUS_LM					0x9421//�����й�����ܣ����£�
#define CON_DI_TYPE_MPUNDIRPOWAPEX_LM					0x9422//�����й�����ܣ����£�
#define CON_DI_TYPE_MPUNDIRPOWFLAT_LM					0x9423//�����й�ƽ���ܣ����£�
#define CON_DI_TYPE_MPUNDIRPOWVALE_LM					0x9424//�����й��ȵ��ܣ����£�
#define CON_DI_TYPE_MPUNDIRPOW_LM_BLOCK 				0x942F//�����й��������ݿ飨���£�
///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPDIRVARTOTAL						0x9110//�����޹��ܵ���
#define CON_DI_TYPE_MPDIRVARPLUS						0x9111//�����޹������
#define CON_DI_TYPE_MPDIRVARAPEX						0x9112//�����޹������
#define CON_DI_TYPE_MPDIRVARFLAT						0x9113//�����޹�ƽ����
#define CON_DI_TYPE_MPDIRVARVALE						0x9114//�����޹��ȵ���
#define CON_DI_TYPE_MPDIRVAR_BLOCK						0x911F//�����޹��������ݿ�

#define CON_DI_TYPE_MPUNDIRVARTOTAL						0x9120//�����޹��ܵ���
#define CON_DI_TYPE_MPUNDIRVARPLUS						0x9121//�����޹������
#define CON_DI_TYPE_MPUNDIRVARAPEX						0x9122//�����޹������
#define CON_DI_TYPE_MPUNDIRVARFLAT						0x9123//�����޹�ƽ����
#define CON_DI_TYPE_MPUNDIRVARVALE						0x9124//�����޹��ȵ���
#define CON_DI_TYPE_MPUNDIRVAR_BLOCK					0x912F//�����޹��������ݿ�
///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPDIRVARTOTAL_LM					0x9510//�����޹��ܵ��ܣ����£�
#define CON_DI_TYPE_MPDIRVARPLUS_LM						0x9511//�����޹�����ܣ����£�
#define CON_DI_TYPE_MPDIRVARAPEX_LM						0x9512//�����޹�����ܣ����£�
#define CON_DI_TYPE_MPDIRVARFLAT_LM						0x9513//�����޹�ƽ���ܣ����£�
#define CON_DI_TYPE_MPDIRVARVALE_LM						0x9514//�����޹��ȵ��ܣ����£�
#define CON_DI_TYPE_MPDIRVAR_LM_BLOCK					0x951F//�����޹��������ݿ飨���£�

#define CON_DI_TYPE_MPUNDIRVARTOTAL_LM					0x9520//�����޹��ܵ��ܣ����£�
#define CON_DI_TYPE_MPUNDIRVARPLUS_LM					0x9521//�����޹�����ܣ����£�
#define CON_DI_TYPE_MPUNDIRVARAPEX_LM					0x9522//�����޹�����ܣ����£�
#define CON_DI_TYPE_MPUNDIRVARFLAT_LM					0x9523//�����޹�ƽ���ܣ����£�
#define CON_DI_TYPE_MPUNDIRVARVALE_LM					0x9524//�����޹��ȵ��ܣ����£�
#define CON_DI_TYPE_MPUNDIRVAR_LM_BLOCK					0x952F//�����޹��������ݿ飨���£�
/////////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPVAR1								0x9130//һ�����޹��ܵ���
#define CON_DI_TYPE_MPVAR1_FEE1							0x9131//����1
#define CON_DI_TYPE_MPVAR1_FEE2							0x9132//����2
#define CON_DI_TYPE_MPVAR1_FEE3							0x9133//����3
#define CON_DI_TYPE_MPVAR1_FEE4							0x9134//����4
#define CON_DI_TYPE_MPVAR1_BLOCK						0x913F//һ�����޹��������ݿ�

#define CON_DI_TYPE_MPVAR4								0x9140//�������޹��ܵ���
#define CON_DI_TYPE_MPVAR4_FEE1							0x9141//����1
#define CON_DI_TYPE_MPVAR4_FEE2							0x9142//����2
#define CON_DI_TYPE_MPVAR4_FEE3							0x9143//����3
#define CON_DI_TYPE_MPVAR4_FEE4							0x9144//����4
#define CON_DI_TYPE_MPVAR4_BLOCK						0x914F//һ�����޹��������ݿ�

#define CON_DI_TYPE_MPVAR2								0x9150//�������޹��ܵ���
#define CON_DI_TYPE_MPVAR2_FEE1							0x9151//����1
#define CON_DI_TYPE_MPVAR2_FEE2							0x9152//����2
#define CON_DI_TYPE_MPVAR2_FEE3							0x9153//����3
#define CON_DI_TYPE_MPVAR2_FEE4							0x9154//����4
#define CON_DI_TYPE_MPVAR2_BLOCK						0x915F//�������޹��������ݿ�

#define CON_DI_TYPE_MPVAR3								0x9160//�������޹��ܵ���
#define CON_DI_TYPE_MPVAR3_FEE1							0x9161//����1
#define CON_DI_TYPE_MPVAR3_FEE2							0x9162//����2
#define CON_DI_TYPE_MPVAR3_FEE3							0x9163//����3
#define CON_DI_TYPE_MPVAR3_FEE4							0x9164//����4
#define CON_DI_TYPE_MPVAR3_BLOCK						0x916F//�������޹��������ݿ�
///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPVAR1_LM							0x9530//һ�����޹��ܵ��ܣ����£�
#define CON_DI_TYPE_MPVAR1_LM_FEE1						0x9531//����1
#define CON_DI_TYPE_MPVAR1_LM_FEE2						0x9532//����2
#define CON_DI_TYPE_MPVAR1_LM_FEE3						0x9533//����3
#define CON_DI_TYPE_MPVAR1_LM_FEE4						0x9534//����4
#define CON_DI_TYPE_MPVAR1_LM_BLOCK						0x953F

#define CON_DI_TYPE_MPVAR4_LM							0x9540//�������޹��ܵ��ܣ����£�
#define CON_DI_TYPE_MPVAR4_LM_FEE1						0x9541//����1
#define CON_DI_TYPE_MPVAR4_LM_FEE2						0x9542//����2
#define CON_DI_TYPE_MPVAR4_LM_FEE3						0x9543//����3
#define CON_DI_TYPE_MPVAR4_LM_FEE4						0x9544//����4
#define CON_DI_TYPE_MPVAR4_LM_BLOCK						0x954F

#define CON_DI_TYPE_MPVAR2_LM							0x9550//�������޹��ܵ��ܣ����£�
#define CON_DI_TYPE_MPVAR2_LM_FEE1						0x9551//����1
#define CON_DI_TYPE_MPVAR2_LM_FEE2						0x9552//����2
#define CON_DI_TYPE_MPVAR2_LM_FEE3						0x9553//����3
#define CON_DI_TYPE_MPVAR2_LM_FEE4						0x9554//����4
#define CON_DI_TYPE_MPVAR2_LM_BLOCK						0x955F

#define CON_DI_TYPE_MPVAR3_LM							0x9560//�������޹��ܵ��ܣ����£�
#define CON_DI_TYPE_MPVAR3_LM_FEE1						0x9561//����1
#define CON_DI_TYPE_MPVAR3_LM_FEE2						0x9562//����2
#define CON_DI_TYPE_MPVAR3_LM_FEE3						0x9563//����3
#define CON_DI_TYPE_MPVAR3_LM_FEE4						0x9564//����4
#define CON_DI_TYPE_MPVAR3_LM_BLOCK						0x956F
///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPDIRPOWMAXDEMAND					0xA010//�����й����������
#define CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE1				0xA011//����1
#define CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE2				0xA012//����2
#define CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE3				0xA013//����3
#define CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE4				0xA014//����4
#define CON_DI_TYPE_MPDIRPOWMAXDEMAND_BLOCK				0xA01F

#define CON_DI_TYPE_MPUNDIRPOWMAXDEMAND					0xA020//�����й����������
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE1			0xA021//����1
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE2			0xA022//����2
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE3			0xA023//����3
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE4			0xA024//����4
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_BLOCK			0xA02F

#define CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME				0xB010//�����й��������������ʱ��
#define CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE1			0xB011//����1
#define CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE2			0xB012//����2
#define CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE3			0xB013//����3
#define CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE4			0xB014//����4
#define CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_BLOCK			0xB01F

#define CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME				0xB020//�����й��������������ʱ��
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE1		0xB021//����1
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE2		0xB022//����2
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE3		0xB023//����3
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE4		0xB024//����4
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_BLOCK		0xB02F

#define CON_DI_TYPE_MPDIRVARMAXDEMAND					0xA110//�����޹����������
#define CON_DI_TYPE_MPDIRVARMAXDEMAND_FEE1				0xA111//����1
#define CON_DI_TYPE_MPDIRVARMAXDEMAND_FEE2				0xA112//����2
#define CON_DI_TYPE_MPDIRVARMAXDEMAND_FEE3				0xA113//����3
#define CON_DI_TYPE_MPDIRVARMAXDEMAND_FEE4				0xA114//����4
#define CON_DI_TYPE_MPDIRVARMAXDEMAND_BLOCK				0xA11F

#define CON_DI_TYPE_MPUNDIRVARMAXDEMAND					0xA120//�����޹����������
#define CON_DI_TYPE_MPUNDIRVARMAXDEMAND_FEE1			0xA121//����1
#define CON_DI_TYPE_MPUNDIRVARMAXDEMAND_FEE2			0xA122//����2
#define CON_DI_TYPE_MPUNDIRVARMAXDEMAND_FEE3			0xA123//����3
#define CON_DI_TYPE_MPUNDIRVARMAXDEMAND_FEE4			0xA124//����4
#define CON_DI_TYPE_MPUNDIRVARMAXDEMAND_BLOCK			0xA12F

#define CON_DI_TYPE_MPDIRVARMAXDEMANDTIME				0xB110//�����޹��������������ʱ��
#define CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_FEE1			0xB111//����1
#define CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_FEE2			0xB112//����2
#define CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_FEE3			0xB113//����3
#define CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_FEE4			0xB114//����4
#define CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_BLOCK			0xB11F

#define CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME				0xB120//�����޹��������������ʱ��
#define CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_FEE1		0xB121//����1
#define CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_FEE2		0xB122//����2
#define CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_FEE3		0xB123//����3
#define CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_FEE4		0xB124//����4
#define CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_BLOCK		0xB12F

///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM				0xA410//�����й���������������£�
#define CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE1			0xA411//����1
#define CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE2			0xA412//����2
#define CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE3			0xA413//����3
#define CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE4			0xA414//����4
#define CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_BLOCK			0xA41F
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM				0xA420//�����й���������������£�
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE1			0xA421//����1
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE2			0xA422//����2
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE3			0xA423//����3
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE4			0xA424//����4
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_BLOCK		0xA42F

#define CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM			0xB410//�����й��������������ʱ�䣨���£�
#define CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE1		0xB411//����1
#define CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE2		0xB412//����2
#define CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE3		0xB413//����3
#define CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE4		0xB414//����4
#define CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_BLOCK		0xB41F
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM			0xB420//�����й��������������ʱ�䣨���£�
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE1		0xB421//����1
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE2		0xB422//����2
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE3		0xB423//����3
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE4		0xB424//����4
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_BLOCK	0xB42F

#define CON_DI_TYPE_MPDIRVARMAXDEMAND_LM				0xA510//�����޹���������������£�
#define CON_DI_TYPE_MPDIRVARMAXDEMAND_LM_FEE1			0xA511//����1
#define CON_DI_TYPE_MPDIRVARMAXDEMAND_LM_FEE2			0xA512//����2
#define CON_DI_TYPE_MPDIRVARMAXDEMAND_LM_FEE3			0xA513//����3
#define CON_DI_TYPE_MPDIRVARMAXDEMAND_LM_FEE4			0xA514//����4
#define CON_DI_TYPE_MPDIRVARMAXDEMAND_LM_BLOCK			0xA51F
#define CON_DI_TYPE_MPUNDIRVARMAXDEMAND_LM				0xA520//�����޹���������������£�
#define CON_DI_TYPE_MPUNDIRVARMAXDEMAND_LM_FEE1			0xA521//����1
#define CON_DI_TYPE_MPUNDIRVARMAXDEMAND_LM_FEE2			0xA522//����2
#define CON_DI_TYPE_MPUNDIRVARMAXDEMAND_LM_FEE3			0xA523//����3
#define CON_DI_TYPE_MPUNDIRVARMAXDEMAND_LM_FEE4			0xA524//����4
#define CON_DI_TYPE_MPUNDIRVARMAXDEMAND_LM_BLOCK		0xA52F

#define CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_LM			0xB510//�����޹��������������ʱ�䣨���£�
#define CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_LM_FEE1		0xB511//����1
#define CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_LM_FEE2		0xB512//����2
#define CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_LM_FEE3		0xB513//����3
#define CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_LM_FEE4		0xB514//����4
#define CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_LM_BLOCK		0xB51F
#define CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_LM			0xB520//�����޹��������������ʱ�䣨���£�
#define CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_LM_FEE1		0xB521//����1
#define CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_LM_FEE2		0xB522//����2
#define CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_LM_FEE3		0xB523//����3
#define CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_LM_FEE4		0xB524//����4
#define CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_LM_BLOCK	0xB52F

///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPVOLA								0xB611//A���ѹ
#define CON_DI_TYPE_MPVOLB								0xB612//B���ѹ
#define CON_DI_TYPE_MPVOLC								0xB613//C���ѹ
#define CON_DI_TYPE_MPVOL_BLOCK							0xB61F//��ѹ���ݿ�

#define CON_DI_TYPE_MPCURA								0xB621//A�����
#define CON_DI_TYPE_MPCURB								0xB622//B�����
#define CON_DI_TYPE_MPCURC								0xB623//C����� 
#define CON_DI_TYPE_MPCUR_BLOCK							0xB62F//�������ݿ�

#define CON_DI_TYPE_MPINSTANTPOW						0xB630//˲ʱ�й�����
#define CON_DI_TYPE_MPINSTANTPOWA						0xB631//A���й�����
#define CON_DI_TYPE_MPINSTANTPOWB						0xB632//B���й�����
#define CON_DI_TYPE_MPINSTANTPOWC						0xB633//C���й�����
#define CON_DI_TYPE_MPINSTANTPOW_BLOCK					0xB63F//

#define CON_DI_TYPE_MPINSTANTVAR						0xB640//˲ʱ�޹�����
#define CON_DI_TYPE_MPINSTANTVARA						0xB641//A���޹�����
#define CON_DI_TYPE_MPINSTANTVARB						0xB642//B���޹�����
#define CON_DI_TYPE_MPINSTANTVARC						0xB643//C���޹�����
#define CON_DI_TYPE_MPINSTANTVAR_BLOCK					0xB64F//

#define	CON_DI_TYPE_MPPOWRATE							0xB650//�ܹ�������
#define	CON_DI_TYPE_MPPOWRATEA							0xB651//A�๦������
#define	CON_DI_TYPE_MPPOWRATEB							0xB652//B�๦������
#define	CON_DI_TYPE_MPPOWRATEC							0xB653//C�๦������
#define	CON_DI_TYPE_MPPOWRATE_BLOCK						0xB65F//
///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_SUMINSTANTPOW1						0x8F50//�й������ܼ���1
#define CON_DI_TYPE_SUMINSTANTPOW2						0x8F51//�й������ܼ���2
#define CON_DI_TYPE_SUMINSTANTPOW3						0x8F52//�й������ܼ���3
#define CON_DI_TYPE_SUMINSTANTPOW4						0x8F53//�й������ܼ���4
#define CON_DI_TYPE_SUMINSTANTPOW5						0x8F54//�й������ܼ���5
#define CON_DI_TYPE_SUMINSTANTPOW6						0x8F55//�й������ܼ���6
#define CON_DI_TYPE_SUMINSTANTPOW7						0x8F56//�й������ܼ���7
#define CON_DI_TYPE_SUMINSTANTPOW8						0x8F57//�й������ܼ���8

#define CON_DI_TYPE_SUMINSTANTVAR1						0x8F60//�޹������ܼ���1
#define CON_DI_TYPE_SUMINSTANTVAR2						0x8F61//�޹������ܼ���2
#define CON_DI_TYPE_SUMINSTANTVAR3						0x8F62//�޹������ܼ���3
#define CON_DI_TYPE_SUMINSTANTVAR4						0x8F63//�޹������ܼ���4
#define CON_DI_TYPE_SUMINSTANTVAR5						0x8F64//�޹������ܼ���5
#define CON_DI_TYPE_SUMINSTANTVAR6						0x8F65//�޹������ܼ���6
#define CON_DI_TYPE_SUMINSTANTVAR7						0x8F66//�޹������ܼ���7
#define CON_DI_TYPE_SUMINSTANTVAR8						0x8F67//�޹������ܼ���8

#define CON_DI_TYPE_SUMPOW1								0x8F70//�й��ܼ���1
#define CON_DI_TYPE_SUMPOW2								0x8F71//�й��ܼ���2
#define CON_DI_TYPE_SUMPOW3								0x8F72//�й��ܼ���3
#define CON_DI_TYPE_SUMPOW4								0x8F73//�й��ܼ���4
#define CON_DI_TYPE_SUMPOW5								0x8F74//�й��ܼ���5
#define CON_DI_TYPE_SUMPOW6								0x8F75//�й��ܼ���6
#define CON_DI_TYPE_SUMPOW7								0x8F76//�й��ܼ���7
#define CON_DI_TYPE_SUMPOW8								0x8F77//�й��ܼ���8

#define CON_DI_TYPE_SUMVAR1								0x8F80//�޹��ܼ���1
#define CON_DI_TYPE_SUMVAR2								0x8F81//�޹��ܼ���2
#define CON_DI_TYPE_SUMVAR3								0x8F82//�޹��ܼ���3
#define CON_DI_TYPE_SUMVAR4								0x8F83//�޹��ܼ���4
#define CON_DI_TYPE_SUMVAR5								0x8F84//�޹��ܼ���5
#define CON_DI_TYPE_SUMVAR6								0x8F85//�޹��ܼ���6
#define CON_DI_TYPE_SUMVAR7								0x8F86//�޹��ܼ���7
#define CON_DI_TYPE_SUMVAR8								0x8F87//�޹��ܼ���8

///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPPOWCONST							0xC030//�й�����
#define CON_DI_TYPE_MPVARCONST							0xC031//�޹�����
#define CON_DI_TYPE_MPCONST_BLOCK						0xC03F//

#define CON_DI_TYPE_MPFEERATE1							0xC331//����1
#define CON_DI_TYPE_MPFEERATE2							0xC332//����2
#define CON_DI_TYPE_MPFEERATE3							0xC333//����3
#define CON_DI_TYPE_MPFEERATE4							0xC334//����4
#define CON_DI_TYPE_MPFEERATE5							0xC335//����5
#define CON_DI_TYPE_MPFEERATE6							0xC336//����6
#define CON_DI_TYPE_MPFEERATE7							0xC337//����7
#define CON_DI_TYPE_MPFEERATE8							0xC338//����8
#define CON_DI_TYPE_MPFEERATE_BLOCK						0xC33F	
///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPDATEANDWEEK						0xC010//���ڼ��ܴ�
#define CON_DI_TYPE_MPTIME								0xC011//ʱ��
#define CON_DI_TYPE_MPDATETIME_BLOCK					0xC01F//����ʱ�����ݿ�
///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPMETERSTATFLAG						0xC020//�������״̬��
#define CON_DI_TYPE_MPNETSTATFLAG						0xC021//��������״̬��
#define CON_DI_TYPE_MPCO2F_BLOCK						0xC02F//״̬�����ݿ�
///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPCOPYDAYTIME						0xC117//������
#define CON_DI_TYPE_MPC11F_BLOCK						0xC11F
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPLATELYPROGTIME					0xB210//���һ�α��ʱ��
#define CON_DI_TYPE_MPLATELYMAXDEMANDCLEARTIME			0xB211//���һ�������������ʱ��
#define CON_DI_TYPE_MPPROGTIMETIMES						0xB212//��̴���
#define CON_DI_TYPE_MPMAXDEMANDCLEARTIMES				0xB213//��������������
#define CON_DI_TYPE_MPBATWORKTIME						0xB214//��ع���ʱ��
#define CON_DI_TYPE_MPB21F_BLOCK						0xB21F

#define CON_DI_TYPE_MPLOSTVOLNUM						0xB310
#define CON_DI_TYPE_MPLOSTVOLANUM						0xB311
#define CON_DI_TYPE_MPLOSTVOLBNUM						0xB312
#define CON_DI_TYPE_MPLOSTVOLCNUM						0xB313
#define CON_DI_TYPE_MPB31F_BLOCK						0xB31F		//�������  �ô���

#define CON_DI_TYPE_MPLOSTVOLTIME						0xB320
#define CON_DI_TYPE_MPLOSTVOLATIME						0xB321
#define CON_DI_TYPE_MPLOSTVOLBTIME						0xB322
#define CON_DI_TYPE_MPLOSTVOLCTIME						0xB323
#define CON_DI_TYPE_MPB32F_BLOCK						0xB32F		//�ۼƶ���ʱ��  �ô���

#define CON_DI_TYPE_MPLASTVOLOFFBEGTIME					0xB330
#define CON_DI_TYPE_MPLASTVOLAOFFBEGTIME				0xB331
#define CON_DI_TYPE_MPLASTVOLBOFFBEGTIME				0xB332
#define CON_DI_TYPE_MPLASTVOLCOFFBEGTIME				0xB333
#define CON_DI_TYPE_MPB33F_BLOCK						0xB33F		//���һ�ζ�����ʼʱ��		��С��

#define CON_DI_TYPE_MPLASTVOLOFFENDTIME					0xB340
#define CON_DI_TYPE_MPLASTVOLAOFFENDTIME				0xB341
#define CON_DI_TYPE_MPLASTVOLBOFFENDTIME				0xB342
#define CON_DI_TYPE_MPLASTVOLCOFFENDTIME				0xB343
#define CON_DI_TYPE_MPB34F_BLOCK						0xB34F		//���һ�ζ������ʱ��		��С��

//������չ
#define CON_DI_TYPE_HB_DIRVARTOTAL						0xE410		//��ǰ�޹����ܾ���ֵ�ͣ�1+4���ޣ�
#define CON_DI_TYPE_HB_UNDIRVARTOTAL					0xE411		//��ǰ�޹����ܾ���ֵ�ͣ�2+3���ޣ�
#define CON_DI_TYPE_HB_DIRVARTOTAL_LM					0xE430		//�����޹����ܾ���ֵ�ͣ�1+4���ޣ�
#define CON_DI_TYPE_HB_UNDIRVARTOTAL_LM					0xE441		//�����޹����ܾ���ֵ�ͣ�2+3���ޣ�


//07��Լ��չ
#define CON_DI_TYPE_MPVOLPHASEA							0x0101//A���ѹ���
#define CON_DI_TYPE_MPVOLPHASEB							0x0102//B���ѹ���
#define CON_DI_TYPE_MPVOLPHASEC							0x0103//C���ѹ���
#define CON_DI_TYPE_MPVOLPHASE_BLOCK					0x01FF//��ѹ������ݿ�

#define CON_DI_TYPE_MPCURPHASEA							0x0201//A��������
#define CON_DI_TYPE_MPCURPHASEB							0x0202//B��������
#define CON_DI_TYPE_MPCURPHASEC							0x0203//C��������
#define CON_DI_TYPE_MPCURPHASE_BLOCK					0x02FF//����������ݿ�


#define CON_DI_TYPE_MPPROGRAMNUM						0X3000//����ܴ���
#define CON_DI_TYPE_MPPROGRAMTIME						0X3001//���һ�α��ʱ��

#define CON_DI_TYPE_MPMETERCLRNUM						0X3010//�����0�ܴ���
#define CON_DI_TYPE_MPMETERCLRTIME						0X3011//���һ�ε����0ʱ��

#define CON_DI_TYPE_MPDEMODECLRNUM						0X3020//������0�ܴ���
#define CON_DI_TYPE_MPDEMODECLRTIME						0X3021//���һ��������0ʱ��

#define CON_DI_TYPE_MPEVENTCLRNUM						0X3030//�¼���0�ܴ���
#define CON_DI_TYPE_MPEVENTCLRTIME						0X3031//���һ���¼���0ʱ��

#define CON_DI_TYPE_MPJIAOSHINUM						0X3040//Уʱ�ܴ���
#define CON_DI_TYPE_MPJIAOSHITIME						0X3041//���һ��Уʱʱ��

#define CON_DI_TYPE_MPCUFE_GA							0x3050
#define CON_DI_TYPE_MPCUFE_BA							0x3051
#define CON_DI_TYPE_MPCUFE_RA							0x3052
#define CON_DI_TYPE_MPCUFE_XA							0x3053
#define CON_DI_TYPE_MPCUFE_GB							0x3054
#define CON_DI_TYPE_MPCUFE_BB							0x3055
#define CON_DI_TYPE_MPCUFE_RB							0x3056
#define CON_DI_TYPE_MPCUFE_XB							0x3057
#define CON_DI_TYPE_MPCUFE_GC							0x3058
#define CON_DI_TYPE_MPCUFE_BC							0x3059
#define CON_DI_TYPE_MPCUFE_RC							0x305A
#define CON_DI_TYPE_MPCUFE_XC							0x305B
#define CON_DI_TYPE_MPCUFE							0x305F//

#define CON_DI_TYPE_MPDIRPOWA						0x3060
#define CON_DI_TYPE_MPUNDIRPOWA						0x3061
#define CON_DI_TYPE_MPCOMVAR1A						0x3062
#define CON_DI_TYPE_MPCOMVAR2A						0x3063

#define CON_DI_TYPE_MPDIRPOWB							0x3064
#define CON_DI_TYPE_MPUNDIRPOWB						0x3065
#define CON_DI_TYPE_MPCOMVAR1B						0x3066
#define CON_DI_TYPE_MPCOMVAR2B						0x3067

#define CON_DI_TYPE_MPDIRPOWC							0x3068
#define CON_DI_TYPE_MPUNDIRPOWC						0x3069
#define CON_DI_TYPE_MPCOMVAR1C						0x306A
#define CON_DI_TYPE_MPCOMVAR2C						0x306B

#define CON_DI_TYPE_MPDIRPOWA_LM							0x3070
#define CON_DI_TYPE_MPUNDIRPOWA_LM						0x3071
#define CON_DI_TYPE_MPCOMVAR1A_LM						0x3072
#define CON_DI_TYPE_MPCOMVAR2A_LM						0x3073

#define CON_DI_TYPE_MPDIRPOWB_LM							0x3074
#define CON_DI_TYPE_MPUNDIRPOWB_LM						0x3075
#define CON_DI_TYPE_MPCOMVAR1B_LM						0x3076
#define CON_DI_TYPE_MPCOMVAR2B_LM						0x3077

#define CON_DI_TYPE_MPDIRPOWC_LM							0x3078
#define CON_DI_TYPE_MPUNDIRPOWC_LM						0x3079
#define CON_DI_TYPE_MPCOMVAR1C_LM						0x307A
#define CON_DI_TYPE_MPCOMVAR2C_LM						0x307B

/*************************�����������볣��**************************************/
#define CON_ERRCODE_NO				0		//��ȷ
#define CON_ERRCODE_RELAYOVERTIME	1		//�м̳�ʱ
#define CON_ERRCODE_PARA			2		//�������ݷǷ�
#define CON_ERRCODE_PASSWORD		3		//����Ȩ�޲���
#define CON_ERRCODE_DI				4		//�޴�������
#define CON_ERRCODE_INVALID			5
#define CON_ERRCODE_TIMEOVER		6		//ʵʱ���ò�����ʱ

#define	CON_PURVIEW_ADMIN	0x55
#define	CON_PURVIEW_SET		0xAA
#define	CON_PURVIEW_READ	0x00		//ֻ��

/*************************�ṹ�嶨��*************************************/
//��������ɽ���һ�ű������ݱ�ʶΪ������ͨ��������ѯ���Եõ��Ƿ��д˲����㣬���ݳ��ȣ�������������㡢��������Ƿ�����

#define CON_DI_HAVE				0x55
#define CON_DI_NOHAVE			0xAA
//������Ϣ
#define CON_DI_TYPE_PARA		0x55
#define CON_DI_TYPE_DATA		0xAA
//����������������Ϣ
#define CON_DI_MPTYPE_TERSELF	0x01		//�ն�
#define CON_DI_MPTYPE_NORMAL	0x10		//������
//�Ƿ������
#define CON_DI_OPERATE_ENABLE	0x55
#define CON_DI_OPERATE_DISABLE	0xAA
#define CON_DI_OPERATE_INVALID	0xEE
//����Ȩ��
#define CON_DI_SETPURVIEW_NORMAL	0x55
#define CON_DI_SETPURVIEW_ADMIN		0xAA
#define CON_DI_SETPURVIEW_INVALID	0xFF

#define	CON_DI_LEN_CHANGED			0xFF	//�䳤

typedef struct _STDIINFO			//��������Ϣ
{
	unsigned char ucHave;			//��ѯ�������/��
	unsigned char ucType;			//���ͣ�����/����
	unsigned char ucMPType;			//���������ͣ��ն�/������
	unsigned char ucSetEnable;		//���ã���/��
	unsigned char ucPurView;		//���ò���Ȩ�ޣ��ͼ�/�߼�
	unsigned char ucGetEnable;		//��ȡ����/��
	unsigned char ucReadMeterEnabel;//���Գ�����/�� �񣺱�ʾ�����������У������ݿɴ���ʷֵ�л�ȡ
	unsigned char ucContentLen;		//���ݳ���
	unsigned char ucCommandLen;		//��ȡ�������ݵ������������
	unsigned short usBigDI;			//��������
}STDIINFO;

//������
#define CON_PORT_BAUDRATE_300		0x01
#define CON_PORT_BAUDRATE_600		0x02
#define CON_PORT_BAUDRATE_1200		0x04
#define CON_PORT_BAUDRATE_2400		0x08
#define CON_PORT_BAUDRATE_4800		0x10
#define CON_PORT_BAUDRATE_9600		0x20

#endif

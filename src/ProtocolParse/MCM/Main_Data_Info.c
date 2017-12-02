/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_Data_Info.c
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-02-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

static unsigned char Main_DI_Req1_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,unsigned char * pucRecvBuffer);
static unsigned char Main_DI_Req2_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,unsigned char * pucRecvBuffer);
static unsigned char Main_DI_Req3_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI);
/**************************************************************************
*
*
*�޸ļ�¼:2006-06-09 �޸Ĳ�������,��ԭ��������һ��һ����ѯ�ķ�ʽ��Ϊswitch
*
****************************************************************************/
unsigned char Main_Comm_Data_Query_Manage(STDIINFO *stRetDIInfo,unsigned char ucRepType,unsigned short usDI,unsigned char *pucRecvBuffer)
{
	//STDIINFO stRetDIInfo;
	unsigned char	ucRet=RET_SUCCESS;
	ClearDIInfo( stRetDIInfo );
	switch(ucRepType)
{
	case CON_TERMSG_REQDATA_1:
       	ucRet=Main_DI_Req1_Query_Manage(stRetDIInfo,usDI,pucRecvBuffer);
		break;
	case CON_TERMSG_REQDATA_2:
       	ucRet=Main_DI_Req2_Query_Manage(stRetDIInfo,usDI,pucRecvBuffer);
		break;
	case CON_TERMSG_REQDATA_3:
       	ucRet=Main_DI_Req3_Query_Manage(stRetDIInfo,usDI);
		break;
	default:
		ucRet=RET_ERROR;
		break;
	}
	return ucRet;
}

unsigned char  Main_DI_Req1_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,unsigned char* pucRecvBuffer)
{//��Щ���ݵĳ��Ȼ���,�ط������ݳ���
	//STDIINFO stRetDIInfo;
	unsigned char	  ucRet=RET_SUCCESS;
	unsigned char	ucNum1=0;//ucNum=0,
	
//	ucNum = pucRecvBuffer[0];
//	ucNum1=(ucNum>>6)&0xf3;
	ucNum1=4;	//�̶�4����
	ClearDIInfo( stRetDIInfo );
	
	if(usDI < CON_DATA_REQ1_BEG_CODE || usDI > CON_DATA_REQ1_END_CODE)
	{
		ucRet=RET_ERROR;
		return ucRet;
	}
	SetDIInfo(stRetDIInfo,
		CON_DI_HAVE,									
		CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
		CON_DI_OPERATE_DISABLE,	CON_DI_SETPURVIEW_ADMIN,
		CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_DISABLE,	
		CON_DI_LEN_CHANGED,0,	CON_DI_TYPE_INVALID);

	switch(usDI)
	{
	case	CON_DATA_REQ1_F1:		//F1	�ն˰汾��Ϣ
		stRetDIInfo->ucContentLen=30;
		break;
	case	CON_DATA_REQ1_F2:		//F2	�ն�����ʱ��
		stRetDIInfo->ucContentLen=6;
		break;
	case	CON_DATA_REQ1_F3:		//F3	�ն˲���״̬
		stRetDIInfo->ucContentLen=31;
		break;
	case	CON_DATA_REQ1_F4:		//F4	�ն�ͨ��״̬
		stRetDIInfo->ucContentLen=1;
		break;
	case	CON_DATA_REQ1_F5:		//F5	�ն˿�������״̬
		stRetDIInfo->ucContentLen=6*CON_SUMGROUP_NUM+2;
		break;
	case	CON_DATA_REQ1_F6:		//F6	�ն˵�ǰ����״̬
		stRetDIInfo->ucContentLen=8*CON_SUMGROUP_NUM+3;
		break;
	case	CON_DATA_REQ1_F7:		//F7	�ն��¼���������ǰֵ
		stRetDIInfo->ucContentLen=2;
		break;
	case	CON_DATA_REQ1_F8:		//F8	�ն��¼���־״̬
		stRetDIInfo->ucContentLen=8;
		break;
	case	CON_DATA_REQ1_F9:		//F9	�ն�״̬������λ��־
		stRetDIInfo->ucContentLen=2;
		break;
	case	CON_DATA_REQ1_F10:		//F10	�ն�����վ���ա���ͨ������
		stRetDIInfo->ucContentLen=8;
		break;
	case	CON_DATA_REQ1_F11:		//F11	��ǰ�ź�ǿ��
		stRetDIInfo->ucContentLen=1;
		break;
//azh 170921
	case	CON_DATA_REQ1_F12:		//F12	���������·���ؽ���״̬������λ��־
		stRetDIInfo->ucContentLen=2;
		break;
	case	CON_DATA_REQ1_F13:		//F13	�������ĵ����Ϣ
		stRetDIInfo->ucContentLen=4;
		break;
	case	CON_DATA_REQ1_F14:		//F14	�ļ�����δ�յ����ݶ�
		stRetDIInfo->ucContentLen=130;
		break;		
//	case	CON_DATA_REQ1_F17:		//F17	��ǰ�ܼ��й�����
//	case	CON_DATA_REQ1_F18:		//F18	��ǰ�ܼ��޹�����
//		stRetDIInfo->ucContentLen=2;
//		break;
//	case	CON_DATA_REQ1_F19:		//F19	�����ܼ��й����������ܡ�����1~M��
//	case	CON_DATA_REQ1_F20:		//F20	�����ܼ��޹����������ܡ�����1~M��
//	case	CON_DATA_REQ1_F21:		//F21	�����ܼ��й����������ܡ�����1~M��
//	case	CON_DATA_REQ1_F22:		//F22	�����ܼ��޹����������ܡ�����1~M��
//		stRetDIInfo->ucContentLen=4*(4+1)+1;
//		break;
//	case	CON_DATA_REQ1_F23:		//F23	�ն˵�ǰʣ��������ѣ�
//		stRetDIInfo->ucContentLen=4;
//		break;
//	case	CON_DATA_REQ1_F24:		//F24	��ǰ�����¸��ؿغ��ܼ��й����ʶ���ֵ
//		stRetDIInfo->ucContentLen=2;
//		break;
	case	CON_DATA_REQ1_F25:		//F25	��ǰ���༰����/�޹����ʡ����������������ѹ���������������
		stRetDIInfo->ucContentLen=67;
		break;
	case	CON_DATA_REQ1_F26:		//F26	A��B��C�������ͳ�����ݼ����һ�ζ����¼
		stRetDIInfo->ucContentLen=57;
		break;
	case	CON_DATA_REQ1_F27:		//F27	���ܱ�����ʱ�ӡ���̴����������һ�β���ʱ��
		stRetDIInfo->ucContentLen=60;
		break;
	case	CON_DATA_REQ1_F28:		//F28	�������״̬�ּ����λ��־
		stRetDIInfo->ucContentLen=33;
		break;
	case 	CON_DATA_REQ1_F31:
	case 	CON_DATA_REQ1_F32:
		stRetDIInfo->ucContentLen = 59;
	case	CON_DATA_REQ1_F33:		//F33	��ǰ������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
	case	CON_DATA_REQ1_F34:		//F34	��ǰ������/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
		stRetDIInfo->ucContentLen=(5+4+4+4)*(4+1)+6;
		break;
	case	CON_DATA_REQ1_F35:		//F35	����������/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	CON_DATA_REQ1_F36:		//F36	���·�����/�޹��������������ʱ�䣨�ܡ�����1~M��
		stRetDIInfo->ucContentLen=(3+4+3+4)*(4+1)+6;
		break;
	case	CON_DATA_REQ1_F37:		//F37	����������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
	case	CON_DATA_REQ1_F38:		//F38	���·�����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
		stRetDIInfo->ucContentLen=(5+4+4+4)*(4+1)+6;
		break;
	case	CON_DATA_REQ1_F39:		//F39	����������/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	CON_DATA_REQ1_F40:		//F40	���·�����/�޹��������������ʱ�䣨�ܡ�����1~M��
		stRetDIInfo->ucContentLen=(3+4+3+4)*(4+1)+6;
		break;
	case	CON_DATA_REQ1_F41:		//F41	���������й����������ܡ�����1~M��
	case	CON_DATA_REQ1_F42:		//F42	���������޹����������ܡ�����1~M��
	case	CON_DATA_REQ1_F43:		//F43	���շ����й����������ܡ�����1~M��
	case	CON_DATA_REQ1_F44:		//F44	���շ����޹����������ܡ�����1~M��
	case	CON_DATA_REQ1_F45:		//F45	���������й����������ܡ�����1~M��
	case	CON_DATA_REQ1_F46:		//F46	���������޹����������ܡ�����1~M��
	case	CON_DATA_REQ1_F47:		//F47	���·����й����������ܡ�����1~M��
	case	CON_DATA_REQ1_F48:		//F48	���·����޹����������ܡ�����1~M��
		stRetDIInfo->ucContentLen=4*(4+1)+1;
		break;
	case	CON_DATA_REQ1_F49:		//F49	��ǰ��ѹ��������λ��
		stRetDIInfo->ucContentLen=12;
		break;
//	case	CON_DATA_REQ1_F57:		//F57	��ǰA��B��C�����ѹ������2~N��г����Чֵ
//		stRetDIInfo->ucContentLen=(ucNum-1)*12;
//		break;
//	case	CON_DATA_REQ1_F58:		//F58	��ǰA��B��C�����ѹ������2~N��г��������
//		stRetDIInfo->ucContentLen=(ucNum-1)*12+6;
//		break;
//	case	CON_DATA_REQ1_F65:		//F65	��ǰ������Ͷ��״̬
//		stRetDIInfo->ucContentLen=3;
//		break;
//	case	CON_DATA_REQ1_F66:		//F66	��ǰ�������ۼƲ���Ͷ��ʱ��ʹ���
//		stRetDIInfo->ucContentLen=72;
//		break;
//	case	CON_DATA_REQ1_F67:		//F67	���ա����µ������ۼƲ������޹�������
//		stRetDIInfo->ucContentLen=8;
//		break;
//	case	CON_DATA_REQ1_F73:		//F73	ֱ��ģ����ʵʱ����
//		stRetDIInfo->ucContentLen=8;
//		break;
//	case	CON_DATA_REQ1_F81:		//F81	Сʱ�����ܼ��й�����
//	case	CON_DATA_REQ1_F82:		//F82	Сʱ�����ܼ��޹�����
//		stRetDIInfo->ucContentLen=ucNum1*2+2;
//		break;
//	case	CON_DATA_REQ1_F83:		//F83	Сʱ�����ܼ��й��ܵ�����
//	case	CON_DATA_REQ1_F84:		//F84	Сʱ�����ܼ��޹��ܵ�����
//		stRetDIInfo->ucContentLen=ucNum1*4+2;
//		break;
	case	CON_DATA_REQ1_F89:		//F89	Сʱ�����й�����
	case	CON_DATA_REQ1_F90:		//F90	Сʱ����A���й�����
	case	CON_DATA_REQ1_F91:		//F91	Сʱ����B���й�����
	case	CON_DATA_REQ1_F92:		//F92	Сʱ����C���й�����
	case	CON_DATA_REQ1_F93:		//F93	Сʱ�����޹�����
	case	CON_DATA_REQ1_F94:		//F94	Сʱ����A���޹�����
	case	CON_DATA_REQ1_F95:		//F95	Сʱ����B���޹�����
	case	CON_DATA_REQ1_F96:		//F96	Сʱ����C���޹�����
		stRetDIInfo->ucContentLen=ucNum1*3+2;
		break;
	case	CON_DATA_REQ1_F97:		//F97	Сʱ����A���ѹ
	case	CON_DATA_REQ1_F98:		//F98	Сʱ����B���ѹ
	case	CON_DATA_REQ1_F99:		//F99	Сʱ����C���ѹ
		stRetDIInfo->ucContentLen=ucNum1*2+2;
		break;
	case	CON_DATA_REQ1_F100:		//F100	Сʱ����A�����
	case	CON_DATA_REQ1_F101:		//F101	Сʱ����B�����
	case	CON_DATA_REQ1_F102:		//F102	Сʱ����C�����
	case	CON_DATA_REQ1_F103:		//F103	Сʱ�����������
		stRetDIInfo->ucContentLen=ucNum1*3+2;
		break;
	case	CON_DATA_REQ1_F105:		//F105	Сʱ���������й��ܵ�����
	case	CON_DATA_REQ1_F106:		//F106	Сʱ���������޹��ܵ�����
	case	CON_DATA_REQ1_F107:		//F107	Сʱ���ᷴ���й��ܵ�����
	case	CON_DATA_REQ1_F108:		//F108	Сʱ���ᷴ���޹��ܵ�����
	case	CON_DATA_REQ1_F109:		//F109	Сʱ���������й��ܵ���ʾֵ
	case	CON_DATA_REQ1_F110:		//F110	Сʱ���������޹��ܵ���ʾֵ
	case	CON_DATA_REQ1_F111:		//F111	Сʱ���ᷴ���й��ܵ���ʾֵ
	case	CON_DATA_REQ1_F112:		//F112	Сʱ���ᷴ���޹��ܵ���ʾֵ
		stRetDIInfo->ucContentLen=ucNum1*4+2;
		break;
	case	CON_DATA_REQ1_F113:		//F113	Сʱ�����ܹ�������
	case	CON_DATA_REQ1_F114:		//F114	Сʱ����A�๦������
	case	CON_DATA_REQ1_F115:		//F115	Сʱ����B�๦������
	case	CON_DATA_REQ1_F116:		//F116	Сʱ����C�๦������
		stRetDIInfo->ucContentLen=ucNum1*2+2;
		break;
//azh 170921
	case	CON_DATA_REQ1_F162:		//F162	���ܱ�����ʱ��
		stRetDIInfo->ucContentLen=12;
		break;
//	case	CON_DATA_REQ1_F121:		//F121	Сʱ����ֱ��ģ����
//		stRetDIInfo->ucContentLen=CON_DI_LEN_CHANGED;
//		break;
	case	CON_DATA_REQ1_F129:
	case	CON_DATA_REQ1_F131:
	case	CON_DATA_REQ1_F137:
	case	CON_DATA_REQ1_F139:
//azh 170921
	case	CON_DATA_REQ1_F177://��ǰ����й�����ʾֵ���ܡ�����1��M��
	case	CON_DATA_REQ1_F178://��1����������й�����ʾֵ���ܡ�����1��M��    
		stRetDIInfo->ucContentLen=31;
		break;
	case	CON_DATA_REQ1_F130:
	case	CON_DATA_REQ1_F132:
	case	CON_DATA_REQ1_F133:
	case	CON_DATA_REQ1_F134:
	case	CON_DATA_REQ1_F135:
	case	CON_DATA_REQ1_F136:
	case	CON_DATA_REQ1_F138:
	case	CON_DATA_REQ1_F140:
	case	CON_DATA_REQ1_F141:
	case	CON_DATA_REQ1_F142:
	case	CON_DATA_REQ1_F143:
	case	CON_DATA_REQ1_F144:
		stRetDIInfo->ucContentLen=26;
		break;
	case	CON_DATA_REQ1_F145:
	case	CON_DATA_REQ1_F146:
	case	CON_DATA_REQ1_F147:
	case	CON_DATA_REQ1_F148:
	case	CON_DATA_REQ1_F149:
	case	CON_DATA_REQ1_F150:
	case	CON_DATA_REQ1_F151:
	case	CON_DATA_REQ1_F152:
		stRetDIInfo->ucContentLen=41;
		break;
	default:
		ucRet=RET_ERROR;
		ClearDIInfo( stRetDIInfo );
		break;
	}	
	return ucRet;
}

unsigned char  Main_DI_Req2_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,unsigned char * pucRecvBuffer)
{
	//STDIINFO stRetDIInfo;
	unsigned char	ucNum=0;
//	unsigned char	ucNum1=0;
	unsigned char	ucRet=RET_SUCCESS;
	
	ucNum = pucRecvBuffer[6];
	//ucNum1=(ucNum>>6)&0xf3;
	ClearDIInfo( stRetDIInfo );
	
	if(usDI < CON_DATA_REQ2_BEG_CODE || usDI > CON_DATA_REQ2_END_CODE)
	{
		ucRet=RET_ERROR;
		return ucRet;
	}
	
	SetDIInfo(stRetDIInfo,
		CON_DI_HAVE,									
		CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
		CON_DI_OPERATE_DISABLE,	CON_DI_SETPURVIEW_ADMIN,
		CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_DISABLE,	
		CON_DI_LEN_CHANGED,0,	CON_DI_TYPE_INVALID);
	
	switch(usDI)
	{
	case	CON_DATA_REQ2_F1:		//F1	�ն���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
	case	CON_DATA_REQ2_F2:		//F2	�ն��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
		stRetDIInfo->ucContentLen=(5+4+4+4)*(4+1)+9;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F3:		//F3	�ն���������/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	CON_DATA_REQ2_F4:		//F4	�ն��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��
		stRetDIInfo->ucContentLen=(3+4+3+4)*(4+1)+9;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F5:		//F5	�ն��������й����������ܡ�����1~M��
	case	CON_DATA_REQ2_F6:		//F6	�ն��������޹����������ܡ�����1~M��
	case	CON_DATA_REQ2_F7:		//F7	�ն��ᷴ���й����������ܡ�����1~M��
	case	CON_DATA_REQ2_F8:		//F8	�ն��ᷴ���޹����������ܡ�����1~M��
		stRetDIInfo->ucContentLen=4*(4+1)+3+1;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F9:		//F9	�����ն���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
	case	CON_DATA_REQ2_F10:		//F10	�����ն��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
		stRetDIInfo->ucContentLen=(5+4+4+4)*(4+1)+9;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F11:		//F11	�����ն���������/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	CON_DATA_REQ2_F12:		//F12	�����ն��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��
		stRetDIInfo->ucContentLen=(3+4+3+4)*(4+1)+9;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F17:		//F17	�¶���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
	case	CON_DATA_REQ2_F18:		//F18	�¶��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
		stRetDIInfo->ucContentLen=(5+4+4+4)*(4+1)+8;
		stRetDIInfo->ucCommandLen=2;
		break;									
	case	CON_DATA_REQ2_F19:		//F19	�¶���������/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	CON_DATA_REQ2_F20:		//F20	�¶��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��
		stRetDIInfo->ucContentLen=(3+4+3+4)*(4+1)+8;
		stRetDIInfo->ucCommandLen=2;
		break;									
	case	CON_DATA_REQ2_F21:		//F21	�¶��������й����������ܡ�����1~M��
	case	CON_DATA_REQ2_F22:		//F22	�¶��������޹����������ܡ�����1~M��
	case	CON_DATA_REQ2_F23:		//F23	�¶��ᷴ���й����������ܡ�����1~M��
	case	CON_DATA_REQ2_F24:		//F24	�¶��ᷴ���޹����������ܡ�����1~M��
		stRetDIInfo->ucContentLen=4*(4+1)+2+1;
		stRetDIInfo->ucCommandLen=2;
		break;									
	case	CON_DATA_REQ2_F25:		//F25	�շ�������й����ʼ�����ʱ�䡢�й�����Ϊ��ʱ��
		stRetDIInfo->ucContentLen=35;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F26:		//F26	���ܼ������������������ʱ��
		stRetDIInfo->ucContentLen=27;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F27:		//F27	�յ�ѹͳ������
		stRetDIInfo->ucContentLen=69;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F28:		//F28	�ղ�ƽ���Խ���ۼ�ʱ��
		stRetDIInfo->ucContentLen=7;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F29:		//F29	�յ���Խ��ͳ��
		stRetDIInfo->ucContentLen=37;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F30:		//F30	�����ڹ���Խ���ۼ�ʱ��
		stRetDIInfo->ucContentLen=7;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F32:		//F31	�յ��ܱ���������
		stRetDIInfo->ucContentLen=60;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F31:		//F32	����
		stRetDIInfo->ucContentLen=2;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F33:		//F33	�·�������й����ʼ�����ʱ�䡢�����й�����Ϊ��ʱ��
		stRetDIInfo->ucContentLen=34;
		stRetDIInfo->ucCommandLen=2;
		break;									
	case	CON_DATA_REQ2_F34:		//F34	�·����й��������������ʱ�䡢���й��������������ʱ��
		stRetDIInfo->ucContentLen=26;
		stRetDIInfo->ucCommandLen=2;
		break;									
	case	CON_DATA_REQ2_F35:		//F35	�µ�ѹͳ������
		stRetDIInfo->ucContentLen=68;
		stRetDIInfo->ucCommandLen=2;
		break;									
	case	CON_DATA_REQ2_F36:		//F36	�²�ƽ���Խ���ۼ�ʱ��
		stRetDIInfo->ucContentLen=6;
		stRetDIInfo->ucCommandLen=2;
		break;									
	case	CON_DATA_REQ2_F37:		//F37	�µ���Խ��ͳ��
		stRetDIInfo->ucContentLen=36;
		stRetDIInfo->ucCommandLen=2;
		break;									
	case	CON_DATA_REQ2_F38:		//F38	�����ڹ���Խ���ۼ�ʱ��
		stRetDIInfo->ucContentLen=6;
		stRetDIInfo->ucCommandLen=2;
		break;									
//	case	CON_DATA_REQ2_F41:		//F41	�ն��������Ͷ���ۼ�ʱ��ʹ���
//		stRetDIInfo->ucContentLen=75;
//		stRetDIInfo->ucCommandLen=3;
//		break;									
//	case	CON_DATA_REQ2_F42:		//F42	�ա��µ������ۼƲ������޹�������
//		stRetDIInfo->ucContentLen=11;
//		stRetDIInfo->ucCommandLen=3;
//		break;									
	case	CON_DATA_REQ2_F43:		//F43	�չ������������ۼ�ʱ��
		stRetDIInfo->ucContentLen=9;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F44:		//F44	�¹������������ۼ�ʱ��
		stRetDIInfo->ucContentLen=8;
		stRetDIInfo->ucCommandLen=2;
		break;									
	case	CON_DATA_REQ2_F49:		//F49	�ն��չ���ʱ�䡢�ո�λ�ۼƴ���
		stRetDIInfo->ucContentLen=7;
		stRetDIInfo->ucCommandLen=3;
		break;									
//	case	CON_DATA_REQ2_F50:		//F50	�ն��տ���ͳ������
//		stRetDIInfo->ucContentLen=7;
//		stRetDIInfo->ucCommandLen=3;
//		break;									
	case	CON_DATA_REQ2_F51:		//F51	�ն��¹���ʱ�䡢�¸�λ�ۼƴ���
		stRetDIInfo->ucContentLen=6;
		stRetDIInfo->ucCommandLen=2;
		break;									
//	case	CON_DATA_REQ2_F52:		//F52	�ն��¿���ͳ������
//		stRetDIInfo->ucContentLen=6;
//		stRetDIInfo->ucCommandLen=2;
//		break;									
	case	CON_DATA_REQ2_F53:		//F53	�ն�����վ��ͨ������
		stRetDIInfo->ucContentLen=7;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F54:		//F54	�ն�����վ��ͨ������
		stRetDIInfo->ucContentLen=6;
		stRetDIInfo->ucCommandLen=2;
		break;									
//	case	CON_DATA_REQ2_F57:		//F57	�ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��
//		stRetDIInfo->ucContentLen=15;
//		stRetDIInfo->ucCommandLen=3;
//		break;									
//	case	CON_DATA_REQ2_F58:		//F58	�ܼ������ۼ��й����������ܡ�����1~M��
//	case	CON_DATA_REQ2_F59:		//F59	�ܼ������ۼ��޹����������ܡ�����1~M��
//		stRetDIInfo->ucContentLen=4*(4+1)+4;
//		stRetDIInfo->ucCommandLen=3;
//		break;									
//	case	CON_DATA_REQ2_F60:		//F60	�ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��
//		stRetDIInfo->ucContentLen=14;
//		stRetDIInfo->ucCommandLen=2;
//		break;									
//	case	CON_DATA_REQ2_F61:		//F61	�ܼ������ۼ��й����������ܡ�����1~M��
//	case	CON_DATA_REQ2_F62:		//F62	�ܼ������ۼ��޹����������ܡ�����1~M��
//		stRetDIInfo->ucContentLen=4*(4+1)+3;
//		stRetDIInfo->ucCommandLen=2;
//		break;									
//	case	CON_DATA_REQ2_F65:		//F65	�ܼ��鳬���ʶ�ֵ�����ۼ�ʱ�䡢���ۼƵ�����
//	case	CON_DATA_REQ2_F66:		//F66	�ܼ��鳬�µ�������ֵ�����ۼ�ʱ�䡢�ۼƵ�����
//		stRetDIInfo->ucContentLen=8;
//		stRetDIInfo->ucCommandLen=2;
//		break;									
//	case	CON_DATA_REQ2_F73:		//F73	�ܼ����й���������
//	case	CON_DATA_REQ2_F74:		//F74	�ܼ����޹���������
//		stRetDIInfo->ucCommandLen=7;
//		stRetDIInfo->ucContentLen=ucNum*2+7;
//		break;							
//	case	CON_DATA_REQ2_F75:		//F75	�ܼ����й�����������
//	case	CON_DATA_REQ2_F76:		//F76	�ܼ����޹�����������
//		stRetDIInfo->ucCommandLen=7;
//		stRetDIInfo->ucContentLen=ucNum*4+7;
//		break;							
	case	CON_DATA_REQ2_F81:		//F81	�й���������
	case	CON_DATA_REQ2_F82:		//F82	A���й���������
	case	CON_DATA_REQ2_F83:		//F83	B���й���������
	case	CON_DATA_REQ2_F84:		//F84	C���й���������
	case	CON_DATA_REQ2_F85:		//F85	�޹���������
	case	CON_DATA_REQ2_F86:		//F86	A���޹���������
	case	CON_DATA_REQ2_F87:		//F87	B���޹���������
	case	CON_DATA_REQ2_F88:		//F88	C���޹���������
		stRetDIInfo->ucCommandLen=7;
		stRetDIInfo->ucContentLen=ucNum*3+7;
		break;							
	case	CON_DATA_REQ2_F89:		//F89	A���ѹ����
	case	CON_DATA_REQ2_F90:		//F90	B���ѹ����
	case	CON_DATA_REQ2_F91:		//F91	C���ѹ����
	case	CON_DATA_REQ2_F92:		//F92	A���������
	case	CON_DATA_REQ2_F93:		//F93	B���������
	case	CON_DATA_REQ2_F94:		//F94	C���������
	case	CON_DATA_REQ2_F95:		//F95	�����������
		stRetDIInfo->ucCommandLen=7;
		stRetDIInfo->ucContentLen=ucNum*2+7;
		break;							
	case	CON_DATA_REQ2_F97:		//F97	�����й��ܵ�����
	case	CON_DATA_REQ2_F98:		//F98	�����޹��ܵ�����
	case	CON_DATA_REQ2_F99:		//F99	�����й��ܵ�����
	case	CON_DATA_REQ2_F100:		//F100	�����޹��ܵ�����
	case	CON_DATA_REQ2_F101:		//F101	�����й��ܵ���ʾֵ
	case	CON_DATA_REQ2_F102:		//F102	�����޹��ܵ���ʾֵ
	case	CON_DATA_REQ2_F103:		//F103	�����й��ܵ���ʾֵ
	case	CON_DATA_REQ2_F104:		//F104	�����޹��ܵ���ʾֵ
		stRetDIInfo->ucCommandLen=7;
		stRetDIInfo->ucContentLen=ucNum*4+7;
		break;							
	case	CON_DATA_REQ2_F105:		//F105	�ܹ�������
	case	CON_DATA_REQ2_F106:		//F106	A�๦������
	case	CON_DATA_REQ2_F107:		//F107	B�๦������
	case	CON_DATA_REQ2_F108:		//F108	C�๦������
		stRetDIInfo->ucCommandLen=7;
		stRetDIInfo->ucContentLen=ucNum*2+7;
		break;		
	case	CON_DATA_REQ2_F109:	
	case	CON_DATA_REQ2_F110:	
		stRetDIInfo->ucCommandLen=7;
		stRetDIInfo->ucContentLen=ucNum*6+7;
		break;	
//	case	CON_DATA_REQ2_F113:		//F113	A��2~19��г�����������ֵ������ʱ��
//	case	CON_DATA_REQ2_F114:		//F114	B��2~19��г�����������ֵ������ʱ��
//	case	CON_DATA_REQ2_F115:		//F115	C��2~19��г�����������ֵ������ʱ��
//		stRetDIInfo->ucContentLen=117;
//		stRetDIInfo->ucCommandLen=3;
//		break;									
//	case	CON_DATA_REQ2_F116:		//F116	A��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
//	case	CON_DATA_REQ2_F117:		//F117	B��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
//	case	CON_DATA_REQ2_F118:		//F118	C��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
//		stRetDIInfo->ucContentLen=117;
//		stRetDIInfo->ucCommandLen=3;
//		break;									
//	case	CON_DATA_REQ2_F121:		//F121	A��г��Խ����ͳ������
//	case	CON_DATA_REQ2_F122:		//F122	B��г��Խ����ͳ������
//	case	CON_DATA_REQ2_F123:		//F123	C��г��Խ����ͳ������
//		stRetDIInfo->ucContentLen=80;
//		stRetDIInfo->ucCommandLen=3;
//		break;									
//	case	CON_DATA_REQ2_F129:		//F129	ֱ��ģ����Խ�����ۼ�ʱ�䡢���/��Сֵ������ʱ��
//		stRetDIInfo->ucContentLen=17;
//		stRetDIInfo->ucCommandLen=3;
//		break;									
//	case	CON_DATA_REQ2_F130:		//F130	ֱ��ģ����Խ�����ۼ�ʱ�䡢���/��Сֵ������ʱ��
//		stRetDIInfo->ucContentLen=16;
//		stRetDIInfo->ucCommandLen=2;
//		break;									
//	case	CON_DATA_REQ2_F138:		//F138	ֱ��ģ������������
//		stRetDIInfo->ucCommandLen=7;
//		break;							
	case	CON_DATA_REQ2_F145:		//F145	һ�����޹��ܵ���ʾֵ����
	case	CON_DATA_REQ2_F146:		//F146	�������޹��ܵ���ʾֵ����
	case	CON_DATA_REQ2_F147:		//F147	�������޹��ܵ���ʾֵ����
	case	CON_DATA_REQ2_F148:		//F148	�������޹��ܵ���ʾֵ����
		stRetDIInfo->ucCommandLen=7;
		stRetDIInfo->ucContentLen=ucNum*4+7;
		break;	
	case 	CON_DATA_REQ2_F153:
	case 	CON_DATA_REQ2_F155:
		stRetDIInfo->ucCommandLen=3;
		stRetDIInfo->ucContentLen=23;
		break;
	case 	CON_DATA_REQ2_F157:
	case 	CON_DATA_REQ2_F159:
		stRetDIInfo->ucCommandLen=2;
		stRetDIInfo->ucContentLen=22;
		break;
		
	case 	CON_DATA_REQ2_F154:
	case 	CON_DATA_REQ2_F156:
		stRetDIInfo->ucCommandLen=3;
		stRetDIInfo->ucContentLen=20;
		break;
	case 	CON_DATA_REQ2_F158:
	case 	CON_DATA_REQ2_F160:
		stRetDIInfo->ucCommandLen=2;
		stRetDIInfo->ucContentLen=19;
		break;
		
	case	CON_DATA_REQ2_F161:		
	case	CON_DATA_REQ2_F163:	
	case	CON_DATA_REQ2_F169:		
	case	CON_DATA_REQ2_F171:
		stRetDIInfo->ucCommandLen=3;
		stRetDIInfo->ucContentLen=34;
		break;
	case	CON_DATA_REQ2_F162:		
	case	CON_DATA_REQ2_F164:	
	case	CON_DATA_REQ2_F165:		
	case	CON_DATA_REQ2_F166:	
	case	CON_DATA_REQ2_F167:		
	case	CON_DATA_REQ2_F168:	
	case	CON_DATA_REQ2_F170:		
	case	CON_DATA_REQ2_F172:	
	case	CON_DATA_REQ2_F173:		
	case	CON_DATA_REQ2_F174:	
	case	CON_DATA_REQ2_F175:		
	case	CON_DATA_REQ2_F176:	
		stRetDIInfo->ucCommandLen=3;
		stRetDIInfo->ucContentLen=29;
		break;
	case	CON_DATA_REQ2_F177:		
	case	CON_DATA_REQ2_F179:	
		stRetDIInfo->ucCommandLen=2;
		stRetDIInfo->ucContentLen=34;
		break;
	case	CON_DATA_REQ2_F178:		
	case	CON_DATA_REQ2_F180:	
	case	CON_DATA_REQ2_F181:		
	case	CON_DATA_REQ2_F182:	
	case	CON_DATA_REQ2_F183:		
	case	CON_DATA_REQ2_F184:	
		stRetDIInfo->ucCommandLen=2;
		stRetDIInfo->ucContentLen=29;
		break;
	case	CON_DATA_REQ2_F185:		
	case	CON_DATA_REQ2_F186:	
	case	CON_DATA_REQ2_F187:		
	case	CON_DATA_REQ2_F188:	
	case	CON_DATA_REQ2_F189:		
	case	CON_DATA_REQ2_F190:	
	case	CON_DATA_REQ2_F191:	
	case	CON_DATA_REQ2_F192:	
		stRetDIInfo->ucCommandLen=3;
		stRetDIInfo->ucContentLen=44;
		break;
	case	CON_DATA_REQ2_F193:		
	case	CON_DATA_REQ2_F194:	
	case	CON_DATA_REQ2_F195:	
	case	CON_DATA_REQ2_F196:	
		stRetDIInfo->ucCommandLen=2;
		stRetDIInfo->ucContentLen=43;
		break;
//azh 170925
	case	CON_DATA_REQ2_F219:		//F219	��������������������ݵ�Ԫ��ʽ
		stRetDIInfo->ucCommandLen=7;
		stRetDIInfo->ucContentLen=ucNum*33+7;
		break;							
	default:
		ClearDIInfo( stRetDIInfo );
		ucRet=RET_ERROR;
		break;
	}
	return ucRet;
}

unsigned char  Main_DI_Req3_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI)
{
	unsigned char	ucRet=RET_ERROR;
	
	ClearDIInfo( stRetDIInfo );
	
	if(usDI < CON_DATA_REQ3_BEG_CODE || usDI > CON_DATA_REQ3_END_CODE)
		return ucRet;
	
	SetDIInfo(stRetDIInfo,
		CON_DI_HAVE,									
		CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
		CON_DI_OPERATE_DISABLE,	CON_DI_SETPURVIEW_ADMIN,
		CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_DISABLE,	
		CON_DI_LEN_CHANGED,2,	CON_DI_TYPE_INVALID);
	
	switch(usDI)
	{
	case	CON_DATA_REQ3_F1:	//F1	��Ҫ�¼�	
	case	CON_DATA_REQ3_F2:	//F2	һ���¼�	
       	ucRet=RET_SUCCESS;
		break;
	default:
		ClearDIInfo( stRetDIInfo );
	}
	return ucRet;
}
/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_Data.c
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-02-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

static unsigned char GetDTbyItemLabel_Ack(unsigned short usItemLabel);
static unsigned char GetDTbyItemLabel_RST(unsigned short usItemLabel);
static unsigned char GetDTbyItemLabel_Para(unsigned short usItemLabel);
static unsigned char GetDTbyItemLabel_Ctrl(unsigned short usItemLabel);
static unsigned char GetDTbyItemLabel_TaskData(unsigned short usItemLabel);
static unsigned char GetDTbyItemLabel_ReqData_1(unsigned short usItemLabel);
static unsigned char GetDTbyItemLabel_ReqData_2(unsigned short usItemLabel);
static unsigned char GetDTbyItemLabel_ReqData_3(unsigned short usItemLabel);
static unsigned char GetDTbyItemLabel_ExtCmd(unsigned short usItemLabel);
static unsigned char GetDTbyItemLabel_DataTrans(unsigned short usItemLabel);
static unsigned char GetDTbyItemLabel_FileTrans(unsigned short usItemLabel);

static unsigned short GetItemLabelbyDT_Ack(unsigned char ucDT);
static unsigned short GetItemLabelbyDT_RST(unsigned char ucDT);
static unsigned short GetItemLabelbyDT_Para(unsigned char ucDT);
static unsigned short GetItemLabelbyDT_Login(unsigned char ucDT);
static unsigned short GetItemLabelbyDT_Ctrl(unsigned char ucDT);
static unsigned short GetItemLabelbyDT_TaskData(unsigned char ucDT);
static unsigned short GetItemLabelbyDT_ReqData_1(unsigned char ucDT);
static unsigned short GetItemLabelbyDT_ReqData_2(unsigned char ucDT);
static unsigned short GetItemLabelbyDT_ReqData_3(unsigned char ucDT);
static unsigned short GetItemLabelbyDT_ExtCmd(unsigned char ucDT);
static unsigned short GetItemLabelbyDT_DataTrans(unsigned char ucDT);
static unsigned short GetItemLabelbyDT_FileTrans(unsigned char ucDT);
 
//static unsigned char Get_PowerCtrlPer_Info(STPOWERCTRLTIME *pstPowerCtrlTime);
//static unsigned char Main_Get_ReqData_1_F5_SumGroupPer(unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned char ucSumGroupNo);
//static unsigned char Main_Get_ReqData_1_F6_SumGroupPer(unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned char ucSumGroupNo,unsigned char ucSumGroupFlag);

static unsigned char GetDTbyItemLabel_Config(unsigned short usItemLabel);
static unsigned char GetDTbyItemLabel_Login(unsigned short usItemLabel);
static unsigned short GetItemLabelbyDT_Config(unsigned char ucDT);

//azh Ϊ�˽�ʡ�ռ�
const U32 protocol_para_fn_valid[8] =
{
    0x7fffffff,//001~032: F1~F31
    0x3f01ff7f,//033~064: F33~F39,F41~F49,F57~F62
    0x07070f0f,//065~096: F65~F68,F73~F76,F81~F83,F89,//azh 170920 //F90,F91
    0x00000000,//097~128: F
    0x00200000,//129~160: F
    0x00000000,//161~192: F
    0x00004000,//193~224: F207
    0x00000000,//225~256: F   
};
const U32 protocol_ctrl_fn_valid[8] =
{
    0xffcfcf03,//001~032: F1~F2,F9~F12,F15~F20,F23~F29,F31~F32//azh 170920 F30
    0x001f037f,//033~064: F33~F39,F41~F42,F49~F53
    0x02000000,//065~096: F90
    0x00000000,//097~128: F
    0x00300000,//129~160: F//azh 170920 F149 F150
    0x00000000,//161~192: F
    0x00000000,//193~224: F
    0x00000000,//225~256: F   
};
const U32 protocol_req1_fn_valid[8] =
{
    0xffffbffe,//001~032: F2~F11,F16~F32//azh 170921 F12~F14
    0x0301ffff,//033~064: F33~F49,F57~F58
    0xff0f0107,//065~096: F65~F67,F73,F81~F84,F89~F96
    0x010fff7f,//097~128: F97~F103,F105~F116,F121
    0xffffffff,//129~160: F129~F160
    0x000303f3,//161~192: F161,F165~F170//azh 170921 F162,F177,F178
    0x00000000,//193~224: F
    0x00000000,//225~256: F   
};
const U32 protocol_req2_fn_valid[8] =
{
    0xffff0fff,//001~032: F1~F12,F17~F32
    0x3f3f3f7f,//033~064: F33~F39,F41~F46,F49~F54,F57~F62
    0x7fff0f03,//065~096: F65~F66,F73~F76,F81~F95
    0x073f3fff,//097~128: F97~F110,F113~F118,F121~F123
    0xff0f0203,//129~160: F129~F130,F138,F145~F148,F153~F160
    0xffffffff,//161~192: F161~F192
    0x07f1ff0f,//193~224: F193~F196,F201~F209,F213~F218//azh170925 F219
    0x00000000,//225~256: F   
};
//azh 170925 Ϊ��ѹ������ռ�
unsigned char Main_Get_ReqData_2_Check(unsigned char ucDAp)
{
    unsigned char ucMPNo,ucMeterNo,ucMPType,ucTaskNo=0xFF;
    unsigned char ucRet=RET_SUCCESS;
    
	ucMPNo=ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
		if(ucMeterNo>=CON_METER_NUM)
			return RET_ERROR;
		ucTaskNo=ucMeterNo;
	}
//	else if(ucMPType==CON_MPTYPE_TER)
//		ucTaskNo=Task_GetTaskNo_Ter();
	if(ucTaskNo>=CON_GRTASK_NUM)
	{    
		return RET_ERROR;
	}
	return ucRet;
}
void format_day_time(STDATETIME *pstTempTime, STDATETIME *pstDataTime)
{
	pstTempTime->ucYear = pstDataTime->ucYear;
	pstTempTime->ucMonth = pstDataTime->ucMonth;
	pstTempTime->ucDay = pstDataTime->ucDay;    

    pstTempTime->ucSecond = 59;
	pstTempTime->ucMinute = 59;
	pstTempTime->ucHour=23;
}

//------------------------------------------------------------------------------------------------
//��д����ʱ�꼰������ azh �Ѿ�������Զ��Ż�
//unsigned short Fill_DataTdFee(U8 ucMeterNo, U8 *pucRetBuffer,U16 usPos,STDATETIME stRMeterTime, STDATETIME stRecordTime)
//{
//    STMETERPARA	 stMeterPara;
//
//	usPos=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,usPos,stRMeterTime);		//����ʱ��Td
//	stMeterPara=RunPara_GetMeterPara(ucMeterNo);
//	usPos=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,usPos,stRecordTime);
//	pucRetBuffer[usPos++]=stMeterPara.ucFeeNum;
//	
//	return usPos;
//}
			
unsigned char Get_DT_by_ItemLabel(unsigned char ucRepType,unsigned short usItemLabel,unsigned char *pucDT1,unsigned char *pucDT2)
{
	unsigned char ucDT =0xFF;
	switch(ucRepType)
	{
	case CON_TERMSG_ACK:			//ȷ�Ϸ���
		ucDT =GetDTbyItemLabel_Ack(usItemLabel);
		break;
	case CON_TERMSG_RESET:			//��λ�������
		ucDT =GetDTbyItemLabel_RST(usItemLabel);
		break;
	case CON_TERMSG_CONNECT:		//��·�ӿڼ��
	case CON_TERMSG_RELAYCMD:		//�м�վ����
		break;
	case CON_TERMSG_SETPARA:		//��������
	case CON_TERMSG_GETPARA:		//��ѯ�ն˲���
		ucDT =GetDTbyItemLabel_Para(usItemLabel);
		break;
	case CON_TERMSG_CONFIGURATION:	//�����ն�����
		ucDT =GetDTbyItemLabel_Config(usItemLabel);
		break;
	case CON_TERMSG_LOGIN:			//�����֤����ԿЭ�̣�AFN=06H��
		ucDT =GetDTbyItemLabel_Login(usItemLabel);
		break;
	case CON_TERMSG_CONTROL:		//��������
		ucDT =GetDTbyItemLabel_Ctrl(usItemLabel);
		break;
	case CON_TERMSG_DATAREQ_SLAVE:
		break;
	case CON_TERMSG_TASKDATA:		//�������ݲ�ѯ
		ucDT =GetDTbyItemLabel_TaskData(usItemLabel);
		break;
	case CON_TERMSG_REQDATA_1:		//����1�����ݣ�ʵʱ���ݣ�
		ucDT =GetDTbyItemLabel_ReqData_1(usItemLabel);
		break;
	case CON_TERMSG_REQDATA_2:		//����2�����ݣ��������ݣ�
		ucDT =GetDTbyItemLabel_ReqData_2(usItemLabel);
		break;
	case CON_TERMSG_REQDATA_3:		//����3�����ݣ��¼���
		ucDT =GetDTbyItemLabel_ReqData_3(usItemLabel);
		break;
	case CON_TERMSG_FILETRANS:		//�ļ�����
		ucDT =GetDTbyItemLabel_FileTrans(usItemLabel);
		break;
	case CON_TERMSG_DATATRANS:		//����ת��
		ucDT =GetDTbyItemLabel_DataTrans(usItemLabel);
		break;
	case CON_TERMSG_EXTCMD:			//��չ����
		ucDT =GetDTbyItemLabel_ExtCmd(usItemLabel);
		break;
	}
	if(ucDT!=0xFF)
	{
		(*pucDT2) = (ucDT-1)/8;
		(*pucDT1) = 1<<((ucDT-1)&0x07);
	}
	return RET_SUCCESS;
}

unsigned short GetItemLabelbyDT(unsigned char ucRepType,unsigned char ucDT)
{
	unsigned short usItemLabel=CON_DI_TYPE_INVALID;
	switch(ucRepType)
	{
	case CON_TERMSG_ACK:			//ȷ�Ϸ���
		usItemLabel = GetItemLabelbyDT_Ack(ucDT);
		break;
	case CON_TERMSG_RESET:			//��λ�������
		usItemLabel = GetItemLabelbyDT_RST(ucDT);
		break;
	case CON_TERMSG_CONNECT:		//��·�ӿڼ��
	case CON_TERMSG_RELAYCMD:		//�м�վ����
		break;
	case CON_TERMSG_SETPARA:		//��������
	case CON_TERMSG_GETPARA:		//��ѯ�ն˲���
		usItemLabel=GetItemLabelbyDT_Para(ucDT);
		break;
	case CON_TERMSG_LOGIN:			//�����֤����ԿЭ�̣�AFN=06H��
		usItemLabel=GetItemLabelbyDT_Login(ucDT);
		break;
	case CON_TERMSG_DATAREQ_SLAVE:
		break;
	case CON_TERMSG_CONFIGURATION:	//�����ն�����
		usItemLabel=GetItemLabelbyDT_Config(ucDT);
		break;
	case CON_TERMSG_CONTROL:		//��������
		usItemLabel=GetItemLabelbyDT_Ctrl(ucDT);
		break;
	case CON_TERMSG_TASKDATA:		//�������ݲ�ѯ
		usItemLabel=GetItemLabelbyDT_TaskData(ucDT);
		break;
	case CON_TERMSG_REQDATA_1:		//����1�����ݣ�ʵʱ���ݣ�
		usItemLabel=GetItemLabelbyDT_ReqData_1(ucDT);
		break;
	case CON_TERMSG_REQDATA_2:		//����2�����ݣ��������ݣ�
		usItemLabel=GetItemLabelbyDT_ReqData_2(ucDT);
		break;
	case CON_TERMSG_REQDATA_3:		//����3�����ݣ��¼���
		usItemLabel=GetItemLabelbyDT_ReqData_3(ucDT);
		break;
	case CON_TERMSG_FILETRANS:		//�ļ�����
		usItemLabel=GetItemLabelbyDT_FileTrans(ucDT);
		break;
	case CON_TERMSG_DATATRANS:		//����ת��
		usItemLabel=GetItemLabelbyDT_DataTrans(ucDT);
		break;
	case CON_TERMSG_EXTCMD:			//��չ����
		usItemLabel=GetItemLabelbyDT_ExtCmd(ucDT);
		break;
	}
	return usItemLabel;
}

unsigned char GetDTbyItemLabel_Ack(unsigned short usItemLabel)
{
	unsigned char ucDT =0xFF;
	switch(usItemLabel)
	{
	case	CON_ACK_F1:		ucDT=1;	break;		//F1:ȫ��ȷ��
	case	CON_ACK_F2:		ucDT=2;	break;		//F2:ȫ������
	case	CON_ACK_F3:		ucDT=3;	break;		//F3:�����������ȷ�Ϸ���
	}
	return ucDT;
}

unsigned char GetDTbyItemLabel_RST(unsigned short usItemLabel)
{
	unsigned char ucDT =0xFF;
	switch(usItemLabel)
	{
		case	CON_RST_F1_HARD:		ucDT=1;	break;		//F1:Ӳ����ʼ��
		case	CON_RST_F2_DATA:		ucDT=2;	break;		//F2:��������ʼ��
		case	CON_RST_F3_ALL:			ucDT=3;	break;		//F3:ȫ������
		case 	CON_RST_F4_EXCECOMM:	ucDT = 4;break;	//F4:ͨѶ����֮������ݼ�������ʼ��
	}
	return ucDT;
}

unsigned char GetDTbyItemLabel_Para(unsigned short usItemLabel)
{
	unsigned char ucDT=0xFF;
/*
	switch(usItemLabel)
	{
	case	CON_PARA_F1_COMMPARA:		ucDT = 1	;break;	//F1���ն�ͨ�Ų�������
	case	CON_PARA_F2_RELAYPARA:		ucDT = 2	;break;	//F2���ն��м�ת������
	case	CON_PARA_F3_STAIPADDR:		ucDT = 3	;break;	//F3����վIP��ַ�Ͷ˿�
	case	CON_PARA_F4_STAPHONENO:		ucDT = 4	;break;	//F4����վ�绰����Ͷ������ĺ���
	case	CON_PARA_F5_TERENCRYPT:		ucDT = 5	;break;	//F5���ն���������
	case	CON_PARA_F6_TERGROUPADDR:	ucDT = 6	;break;	//F6���ն����ַ����
	case	CON_PARA_F7_IPADDR:		    ucDT = 7	;break;	//F7���ն˳���������
	case	CON_PARA_F8_ALRCTLWORD:		ucDT = 8	;break;	//F8���ն��¼���¼��������
	case	CON_PARA_F9_CONFIGNUM:		ucDT = 9	;break;	//F9���ն�����������
	case	CON_PARA_F10_METERPARA:		ucDT = 10	;break;	//F10���ն˵��ܱ�/��������װ�����ò���
//	case	CON_PARA_F11_PULSEPARA:		ucDT = 11	;break;	//F11���ն��������ò���
//	case	CON_PARA_F12_DINPUTPARA:	ucDT = 12	;break;	//F12���ն�״̬���������
//	case	CON_PARA_F13_ANALOGPARA:	ucDT = 13	;break;	//F13���ն˵�ѹ/����ģ�������ò���
//	case	CON_PARA_F14_SUMGPARA:		ucDT = 14	;break;	//F14���ն��ܼ������ò���
//	case	CON_PARA_F15_POWVALDIFF:	ucDT = 15	;break;	//F15���й��ܵ������Խ���¼���������
	case	CON_PARA_F16_PDPINFO:		ucDT = 16	;break;	//F16������ר���û���������
//	case	CON_PARA_F17_KEEPPOWER:		ucDT = 17	;break;	//F17���ն˱�����ֵ
//	case	CON_PARA_F18_POWCTRLTIMEPER:ucDT = 18	;break;	//F18���ն˹���ʱ��
//	case	CON_PARA_F19_POWCTRLCOEF:	ucDT = 19	;break;	//F19���ն�ʱ�ι��ض�ֵ����ϵ��
//	case	CON_PARA_F20_ENERCTRLCOEF:	ucDT = 20	;break;	//F20���ն��µ������ض�ֵ����ϵ��
//	case	CON_PARA_F21_PERTIMEFEE:	ucDT = 21	;break;	//F21���ն˵���������ʱ�κͷ�����
//	case	CON_PARA_F22_FEEPARA:		ucDT = 22	;break;	//F22���ն˵���������
//	case	CON_PARA_F23_URGEALARMPARA:	ucDT = 23	;break;	//F23���ն˴߷Ѹ澯����
	case	CON_PARA_F24_COPYINTER:		ucDT = 24	;break;	//F24���ն˳���������
	case	CON_PARA_F25_MPBASEPARA:	ucDT = 25	;break;	//F25���������������
	case	CON_PARA_F26_MPLIMIT:		ucDT = 26	;break;	//F26����������ֵ����
	case	CON_PARA_F27_MPFREEZEPARA:	ucDT = 27	;break;	//F27�����������ݶ������
	case	CON_PARA_F28_MPPOWRATEPARA:	ucDT = 28	;break;	//F28�������㹦�������ֶ���ֵ
	case	CON_PARA_F29_METERADDR:		ucDT = 29	;break;	//F29��
	case 	CON_PARA_F30_METERCOPYFLAG:	ucDT = 30;	break;
	case 	CON_PARA_F31_SLAVENODE:		ucDT = 31;	break;
	case 	CON_PARA_F34_PORTPROPERTY:		ucDT = 34;break;
	case 	CON_PARA_F35_EMPHAFLAG:		ucDT = 35;break;
	case	CON_PARA_F33_SUMGFREEZEPARA:ucDT = 33	;break;	//F33���ܼ������ݶ������
	case 	CON_PARA_F37_LINKPARA:			ucDT = 37;break;//F37:����ͨ�Ų���
	case	CON_PARA_F38_JOB1PARA:			ucDT = 38;break;//F38:һ��������������
	case 	CON_PARA_F39_JOB2PARA:			ucDT = 39;break;//F39:����������������
//	case	CON_PARA_F41_TIMEPERSETTING:ucDT = 41	;break;	//F41��ʱ�ι��ض�ֵ
//	case	CON_PARA_F42_RESTCTRL:		ucDT = 42	;break;	//F42�����ݹ��ز���
//	case	CON_PARA_F43_POWERSLIP:		ucDT = 43	;break;	//F43�����ʿ��ƵĹ��ʼ��㻬��ʱ��
//	case	CON_PARA_F44_SALESTOP:		ucDT = 44	;break;	//F44��Ӫҵ��ͣ�ز���
//	case	CON_PARA_F45_POWERCTRLTURN:	ucDT = 45	;break;	//F45�������ִ��趨
//	case	CON_PARA_F46_MONTHENERCTRL:	ucDT = 46	;break;	//F46���µ����ض�ֵ
//	case	CON_PARA_F47_BUYENERCTRL:	ucDT = 47	;break;	//F47���������ز���
//	case	CON_PARA_F48_ENERCTRLTURN:	ucDT = 48	;break;	//F48������ִ��趨
//	case	CON_PARA_F49_CTRLALRTIME:	ucDT = 49	;break;	//F49�����ظ澯ʱ��
	case	CON_PARA_F57_SOUNDALARM:	ucDT = 57	;break;	//F57���ն������澯����M��ֹ����
	case	CON_PARA_F58_KEEPAUTO:		ucDT = 58	;break;	//F58���ն��Զ��������
	case	CON_PARA_F59_MTRERRORPARA:	ucDT = 59	;break;	//F59�����ܱ��쳣�б���ֵ�趨
//	case	CON_PARA_F60_HARMONPARA:	ucDT = 60	;break;	//F60��г����ֵ
//	case	CON_PARA_F61_DCAINPARA:		ucDT = 61	;break;	//F61��ֱ��ģ�����������
	case	CON_PARA_F62_COMMTYPE:		ucDT = 62	;break;	//F62 ����ר��������ʽ
	case	CON_PARA_F63_FLUXLIMIT:		ucDT = 36	;break;	//F63 ��ͨ��������������
	case	CON_PARA_F65_UPDATA1PARA:	ucDT = 65	;break;	//F65����ʱ����1��������������
	case	CON_PARA_F66_UPDATA2PARA:	ucDT = 66	;break;	//F66����ʱ����2��������������
	case	CON_PARA_F67_UPDATA1CTRL:	ucDT = 67	;break;	//F67����ʱ����1��������������/ֹͣ����
	case	CON_PARA_F68_UPDATA2CTRL:	ucDT = 68	;break;	//F68����ʱ����2��������������/ֹͣ����
	case	CON_PARA_F73_CAPPARA:		ucDT = 73	;break;	//F73������������
//	case	CON_PARA_F74_CAPRUNPARA:	ucDT = 74	;break;	//F74��������Ͷ�����в���
//	case	CON_PARA_F75_CAPRELAYPARA:	ucDT = 75	;break;	//F75����������������
//	case	CON_PARA_F76_CAPCTRL:		ucDT = 76	;break;	//F76��������Ͷ�п��Ʒ�ʽ
//	case	CON_PARA_F81_DCARATE:		ucDT = 81	;break;	//F81��ֱ��ģ����������
//	case	CON_PARA_F82_DCALIMIT:		ucDT = 82	;break;	//F82��ֱ��ģ������ֵ
//	case	CON_PARA_F83_DCAFREESE:		ucDT = 83	;break;	//F83��ֱ��ģ�����������
	case	CON_PARA_F89_ADDR:			ucDT = 89	;break;	//F89  �ն˵�ַ
	case	CON_PARA_F207_ADDR:			ucDT = 207	;break;	
	}
*/  

    if(usItemLabel > CON_PARA_BEG_CODE)
    {
        ucDT = usItemLabel - CON_PARA_BEG_CODE;
        if(protocol_para_fn_valid[(ucDT-1)/32]&(1<<((ucDT-1)%32))==0)//azh ��Ч����0xff
        {
            ucDT = 0xff;
        }
    }
    
	return ucDT;
}


unsigned char GetDTbyItemLabel_Config(unsigned short usItemLabel)
{
	unsigned char ucDT=0xFF;
	switch(usItemLabel)
	{
	case 	CON_CONFIG_F1_VERSION:		ucDT = 1	;break;	//F1�����ն˰汾��Ϣ
	case  	CON_CONFIG_F2_INTERFACE:	ucDT = 2	;break;	//F2�����ն����롢�����ͨ�Ŷ˿�����
	case 	CON_CONFIG_F3_OTHER:		ucDT = 3	;break;	//F3�����ն���������
	case 	CON_CONFIG_F4_PARA:			ucDT = 4	;break;	//F4�����ն˲������ã����ն���Ӳ���汾֧�ֵĲ�����
	case 	CON_CONFIG_F5_CTRL:			ucDT = 5	;break;	//F5�����ն˿������ã����ն���Ӳ���汾֧�ֵĿ������
	case 	CON_CONFIG_F6_DATA1:		ucDT = 6	;break;	//F6�����ն�1���������ã����ն���Ӳ���汾֧�ֵ�1�����ݣ�
	case 	CON_CONFIG_F7_DATA2:		ucDT = 7	;break;	//F7�����ն�2���������ã����ն���Ӳ���汾֧�ֵ�2�����ݣ�
	case 	CON_CONFIG_F8_ERC:			ucDT = 8	;break;	//F8�����ն��¼���¼���ã����ն���Ӳ���汾֧�ֵ��¼���¼��
//azh 170920
	case 	CON_CONFIG_F9_REMOTEVER:	ucDT = 9	;break;	//F9Զ��ͨ��ģ��汾��Ϣ
	case 	CON_CONFIG_F10_LOCALVER:	ucDT = 10   ;break;	//F10����ͨ��ģ��汾��Ϣ	   
	} 
	return ucDT;
}


unsigned char GetDTbyItemLabel_Login(unsigned short usItemLabel)
{
	unsigned char ucDT=0xFF;
	switch(usItemLabel)
	{
	case 	CON_LOGIN_F1_REQUEST:		ucDT = 1	;break;	//F1�����֤����
	case  	CON_LOGIN_F2_ANSWER:		ucDT = 2	;break;	//F2�����֤��Ӧ
	} 
	return ucDT;
}


unsigned char GetDTbyItemLabel_Ctrl(unsigned short usItemLabel)
{
	unsigned char ucDT=0xFF;
/*
	switch(usItemLabel)
	{
//	case CON_CTRL_F1_YK_OPEN:		ucDT = 1	;break;	//F1ң����բ
//	case CON_CTRL_F2_YK_CLOSE:		ucDT = 2	;break;	//F2ң�������բ
//	case CON_CTRL_F9_TIMEPER_ON:	ucDT = 9	;break;	//F9ʱ�ι���Ͷ��
//	case CON_CTRL_F10_RESTCTRL_ON:	ucDT = 10	;break;//F10���ݹ���Ͷ��
//	case CON_CTRL_F11_SALESTOP_ON:	ucDT = 11	;break;//F11Ӫҵ��ͣ����Ͷ��
//	case CON_CTRL_F12_POWERLOWER_ON:ucDT = 12	;break;//F12��ǰ�����¸���Ͷ��
//	case CON_CTRL_F15_MONTHENER_ON:	ucDT = 15	;break;//F15�µ��Ͷ��
//	case CON_CTRL_F16_BUYENER_ON:	ucDT = 16	;break;//F16�����Ͷ��
//	case CON_CTRL_F17_TIMEPER_OFF:	ucDT = 17	;break;//F17ʱ�ι��ؽ��
//	case CON_CTRL_F18_RESTCTRL_OFF:	ucDT = 18	;break;//F18���ݹ��ؽ��
//	case CON_CTRL_F19_SALESTOP_OFF:	ucDT = 19	;break;//F19Ӫҵ��ͣ���ؽ��
//	case CON_CTRL_F20_POWERLOWER_OFF:ucDT = 20	;break;//F20��ǰ�����¸��ؽ��
//	case CON_CTRL_F23_MONTHENER_OFF:ucDT = 23	;break;//F23�µ�ؽ��
//	case CON_CTRL_F24_BUYENER_OFF:	ucDT = 24	;break;//F24����ؽ��
//	case CON_CTRL_F25_KEEPPOWER_ON:	ucDT = 25	;break;//F25�ն˱���Ͷ��
//	case CON_CTRL_F26_URGEALARM_ON:	ucDT = 26	;break;//F26�߷Ѹ澯Ͷ��
	case CON_CTRL_F27_TALK_ON:		ucDT = 27	;break;//F27�����ն�����վͨ��
	case CON_CTRL_F28_REMOVE_ON:	ucDT = 28	;break;//F28�ն��޳�Ͷ��
	case CON_CTRL_F29_UPACTIVE_ON:	ucDT = 29	;break;//F29�����ն������ϱ�
	case CON_CTRL_F31_SETTIME:		ucDT = 31	;break;//F31��ʱ����
	case CON_CTRL_F32_MESSAGE:		ucDT = 32	;break;//F32������Ϣ
//	case CON_CTRL_F33_KEEPPOWER_OFF:ucDT = 33	;break;//F33�ն˱�����
//	case CON_CTRL_F34_URGEFEE_OFF:	ucDT = 34	;break;//F34�߷Ѹ澯���
	case CON_CTRL_F35_TALK_OFF:		ucDT = 35	;break;//F35��ֹ�ն�����վͨ��
	case CON_CTRL_F36_REMOVE_OFF:	ucDT = 36	;break;//F36�ն��޳����
	case CON_CTRL_F37_UPACTIVE_OFF:	ucDT = 37	;break;//F37��ֹ�ն������ϱ�
	case CON_CTRL_F38_SMS_ACTIVE:	ucDT = 38	;break;//F38���ż���
	case CON_CTRL_F39_DISCONECT:	ucDT = 39	;break;//F38���ż���
//	case CON_CTRL_F41_CAPCTRL_ON:	ucDT = 41	;break;//F41���ݿ���Ͷ��
//	case CON_CTRL_F42_CAPCTRL_OFF:	ucDT = 42	;break;//F42���ݿ��ƽ��
	case CON_CTRL_F49:	ucDT = 49	;break;//F42���ݿ��ƽ��
	case CON_CTRL_F50:	ucDT = 50	;break;//F42���ݿ��ƽ��
	case CON_CTRL_F51:	ucDT = 51	;break;//F42���ݿ��ƽ��
//	case CON_CTRL_F88_TIMEREVISE:	ucDT = 88	;break;//F88��ȷ��ʱ����
	}
*/
    if(usItemLabel > CON_CTRL_BEG_CODE)
    {
        ucDT = usItemLabel - CON_CTRL_BEG_CODE;
        if(protocol_ctrl_fn_valid[(ucDT-1)/32]&(1<<((ucDT-1)%32))==0)//azh ��Ч����0xff
        {
            ucDT = 0xff;
        }
    }
	return ucDT;
}

unsigned char GetDTbyItemLabel_TaskData(unsigned short usItemLabel)
{
	unsigned char ucDT=0xFF;
	switch(usItemLabel)
	{
	case	CON_TASKDATA_REQ_F1:		ucDT = 1	;break;	//F1 1������
	case	CON_TASKDATA_REQ_F2:		ucDT = 2	;break;	//F2 2������
	}
	return ucDT;
}

unsigned char GetDTbyItemLabel_ReqData_1(unsigned short usItemLabel)
{
	unsigned char ucDT=0xFF;
/*
	switch(usItemLabel)
	{
	case	CON_DATA_REQ1_F1:	ucDT = 1	;break;	//F1	�ն˰汾��Ϣ
	case	CON_DATA_REQ1_F2:	ucDT = 2	;break;	//F2	�ն�����ʱ��
	case	CON_DATA_REQ1_F3:	ucDT = 3	;break;	//F3	�ն˲���״̬
	case	CON_DATA_REQ1_F4:	ucDT = 4	;break;	//F4	�ն�ͨ��״̬
	case	CON_DATA_REQ1_F5:	ucDT = 5	;break;	//F5	�ն˿�������״̬
	case	CON_DATA_REQ1_F6:	ucDT = 6	;break;	//F6	�ն˵�ǰ����״̬
	case	CON_DATA_REQ1_F7:	ucDT = 7	;break;	//F7	�ն��¼���������ǰֵ
	case	CON_DATA_REQ1_F8:	ucDT = 8	;break;	//F8	�ն��¼���־״̬
	case	CON_DATA_REQ1_F9:	ucDT = 9	;break;	//F9	�ն�״̬������λ��־
	case	CON_DATA_REQ1_F10:	ucDT = 10	;break;	//F10	�ն�����վ���ա���ͨ������
	case	CON_DATA_REQ1_F11:	ucDT = 11	;break;	//F11	��ǰ�ź�ǿ��
//	case	CON_DATA_REQ1_F17:	ucDT = 17	;break;	//F17	��ǰ�ܼ��й�����
//	case	CON_DATA_REQ1_F18:	ucDT = 18	;break;	//F18	��ǰ�ܼ��޹�����
//	case	CON_DATA_REQ1_F19:	ucDT = 19	;break;	//F19	�����ܼ��й����������ܡ�����1~M��
//	case	CON_DATA_REQ1_F20:	ucDT = 20	;break;	//F20	�����ܼ��޹����������ܡ�����1~M��
//	case	CON_DATA_REQ1_F21:	ucDT = 21	;break;	//F21	�����ܼ��й����������ܡ�����1~M��
//	case	CON_DATA_REQ1_F22:	ucDT = 22	;break;	//F22	�����ܼ��޹����������ܡ�����1~M��
//	case	CON_DATA_REQ1_F23:	ucDT = 23	;break;	//F23	�ն˵�ǰʣ��������ѣ�
//	case	CON_DATA_REQ1_F24:	ucDT = 24	;break;	//F24	��ǰ�����¸��ؿغ��ܼ��й����ʶ���ֵ
	case	CON_DATA_REQ1_F25:	ucDT = 25	;break;	//F25	��ǰ���༰����/�޹����ʡ����������������ѹ���������������
	case	CON_DATA_REQ1_F26:	ucDT = 26	;break;	//F26	A��B��C�������ͳ�����ݼ����һ�ζ����¼
	case	CON_DATA_REQ1_F27:	ucDT = 27	;break;	//F27	���ܱ�����ʱ�ӡ���̴����������һ�β���ʱ��
	case	CON_DATA_REQ1_F28:	ucDT = 28	;break;	//F28	�������״̬�ּ����λ��־
	case 	CON_DATA_REQ1_F31:  ucDT = 31	;break;
	case 	CON_DATA_REQ1_F32:	ucDT = 32	;break;
	case	CON_DATA_REQ1_F33:	ucDT = 33	;break;	//F33	��ǰ������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
	case	CON_DATA_REQ1_F34:	ucDT = 34	;break;	//F34	��ǰ������/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
	case	CON_DATA_REQ1_F35:	ucDT = 35	;break;	//F35	����������/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	CON_DATA_REQ1_F36:	ucDT = 36	;break;	//F36	���·�����/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	CON_DATA_REQ1_F37:	ucDT = 37	;break;	//F37	����������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
	case	CON_DATA_REQ1_F38:	ucDT = 38	;break;	//F38	���·�����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
	case	CON_DATA_REQ1_F39:	ucDT = 39	;break;	//F39	����������/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	CON_DATA_REQ1_F40:	ucDT = 40	;break;	//F40	���·�����/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	CON_DATA_REQ1_F41:	ucDT = 41	;break;	//F41	���������й����������ܡ�����1~M��
	case	CON_DATA_REQ1_F42:	ucDT = 42	;break;	//F42	���������޹����������ܡ�����1~M��
	case	CON_DATA_REQ1_F43:	ucDT = 43	;break;	//F43	���շ����й����������ܡ�����1~M��
	case	CON_DATA_REQ1_F44:	ucDT = 44	;break;	//F44	���շ����޹����������ܡ�����1~M��
	case	CON_DATA_REQ1_F45:	ucDT = 45	;break;	//F45	���������й����������ܡ�����1~M��
	case	CON_DATA_REQ1_F46:	ucDT = 46	;break;	//F46	���������޹����������ܡ�����1~M��
	case	CON_DATA_REQ1_F47:	ucDT = 47	;break;	//F47	���·����й����������ܡ�����1~M��
	case	CON_DATA_REQ1_F48:	ucDT = 48	;break;	//F48	���·����޹����������ܡ�����1~M��
	case	CON_DATA_REQ1_F49:	ucDT = 49	;break;	//F49	��ǰ��ѹ��������λ��
//	case	CON_DATA_REQ1_F57:	ucDT = 57	;break;	//F57	��ǰA��B��C�����ѹ������2~N��г����Чֵ
//	case	CON_DATA_REQ1_F58:	ucDT = 58	;break;	//F58	��ǰA��B��C�����ѹ������2~N��г��������
//	case	CON_DATA_REQ1_F65:	ucDT = 65	;break;	//F65	��ǰ������Ͷ��״̬
//	case	CON_DATA_REQ1_F66:	ucDT = 66	;break;	//F66	��ǰ�������ۼƲ���Ͷ��ʱ��ʹ���
//	case	CON_DATA_REQ1_F67:	ucDT = 67	;break;	//F67	���ա����µ������ۼƲ������޹�������
//	case	CON_DATA_REQ1_F73:	ucDT = 73	;break;	//F73	ֱ��ģ����ʵʱ����
//	case	CON_DATA_REQ1_F81:	ucDT = 81	;break;	//F81	Сʱ�����ܼ��й�����
//	case	CON_DATA_REQ1_F82:	ucDT = 82	;break;	//F82	Сʱ�����ܼ��޹�����
//	case	CON_DATA_REQ1_F83:	ucDT = 83	;break;	//F83	Сʱ�����ܼ��й��ܵ�����
//	case	CON_DATA_REQ1_F84:	ucDT = 84	;break;	//F84	Сʱ�����ܼ��޹��ܵ�����
	case	CON_DATA_REQ1_F89:	ucDT = 89	;break;	//F89	Сʱ�����й�����
	case	CON_DATA_REQ1_F90:	ucDT = 90	;break;	//F90	Сʱ����A���й�����
	case	CON_DATA_REQ1_F91:	ucDT = 91	;break;	//F91	Сʱ����B���й�����
	case	CON_DATA_REQ1_F92:	ucDT = 92	;break;	//F92	Сʱ����C���й�����
	case	CON_DATA_REQ1_F93:	ucDT = 93	;break;	//F93	Сʱ�����޹�����
	case	CON_DATA_REQ1_F94:	ucDT = 94	;break;	//F94	Сʱ����A���޹�����
	case	CON_DATA_REQ1_F95:	ucDT = 95	;break;	//F95	Сʱ����B���޹�����
	case	CON_DATA_REQ1_F96:	ucDT = 96	;break;	//F96	Сʱ����C���޹�����
	case	CON_DATA_REQ1_F97:	ucDT = 97	;break;	//F97	Сʱ����A���ѹ
	case	CON_DATA_REQ1_F98:	ucDT = 98	;break;	//F98	Сʱ����B���ѹ
	case	CON_DATA_REQ1_F99:	ucDT = 99	;break;	//F99	Сʱ����C���ѹ
	case	CON_DATA_REQ1_F100:	ucDT = 100	;break;	//F100	Сʱ����A�����
	case	CON_DATA_REQ1_F101:	ucDT = 101	;break;	//F101	Сʱ����B�����
	case	CON_DATA_REQ1_F102:	ucDT = 102	;break;	//F102	Сʱ����C�����
	case	CON_DATA_REQ1_F103:	ucDT = 103	;break;	//F103	Сʱ�����������
	case	CON_DATA_REQ1_F105:	ucDT = 105	;break;	//F105	Сʱ���������й��ܵ�����
	case	CON_DATA_REQ1_F106:	ucDT = 106	;break;	//F106	Сʱ���������޹��ܵ�����
	case	CON_DATA_REQ1_F107:	ucDT = 107	;break;	//F107	Сʱ���ᷴ���й��ܵ�����
	case	CON_DATA_REQ1_F108:	ucDT = 108	;break;	//F108	Сʱ���ᷴ���޹��ܵ�����
	case	CON_DATA_REQ1_F109:	ucDT = 109	;break;	//F109	Сʱ���������й��ܵ���ʾֵ
	case	CON_DATA_REQ1_F110:	ucDT = 110	;break;	//F110	Сʱ���������޹��ܵ���ʾֵ
	case	CON_DATA_REQ1_F111:	ucDT = 111	;break;	//F111	Сʱ���ᷴ���й��ܵ���ʾֵ
	case	CON_DATA_REQ1_F112:	ucDT = 112	;break;	//F112	Сʱ���ᷴ���޹��ܵ���ʾֵ
	case	CON_DATA_REQ1_F113:	ucDT = 113	;break;	//F113	Сʱ�����ܹ�������
	case	CON_DATA_REQ1_F114:	ucDT = 114	;break;	//F114	Сʱ����A�๦������
	case	CON_DATA_REQ1_F115:	ucDT = 115	;break;	//F115	Сʱ����B�๦������
	case	CON_DATA_REQ1_F116:	ucDT = 116	;break;	//F116	Сʱ����C�๦������
	case	CON_DATA_REQ1_F121:	ucDT = 121	;break;	//F121	Сʱ����ֱ��ģ����
	case	CON_DATA_REQ1_F129:	ucDT = 129	;break;
	case	CON_DATA_REQ1_F130:	ucDT = 130	;break;
	case	CON_DATA_REQ1_F131:	ucDT = 131	;break;
	case	CON_DATA_REQ1_F132:	ucDT = 132	;break;
	case	CON_DATA_REQ1_F133:	ucDT = 133	;break;
	case	CON_DATA_REQ1_F134:	ucDT = 134	;break;
	case	CON_DATA_REQ1_F135:	ucDT = 135	;break;
	case	CON_DATA_REQ1_F136:	ucDT = 136	;break;
	case	CON_DATA_REQ1_F137:	ucDT = 137	;break;
	case	CON_DATA_REQ1_F138:	ucDT = 138	;break;
	case	CON_DATA_REQ1_F139:	ucDT = 139	;break;
	case	CON_DATA_REQ1_F140:	ucDT = 140	;break;
	case	CON_DATA_REQ1_F141:	ucDT = 141	;break;
	case	CON_DATA_REQ1_F142:	ucDT = 142	;break;
	case	CON_DATA_REQ1_F143:	ucDT = 143	;break;
	case	CON_DATA_REQ1_F144:	ucDT = 144	;break;
	case	CON_DATA_REQ1_F145:	ucDT = 145	;break;
	case	CON_DATA_REQ1_F146:	ucDT = 146	;break;
	case	CON_DATA_REQ1_F147:	ucDT = 147	;break;
	case	CON_DATA_REQ1_F148:	ucDT = 148	;break;
	case	CON_DATA_REQ1_F149:	ucDT = 149	;break;
	case	CON_DATA_REQ1_F150:	ucDT = 150	;break;
	case	CON_DATA_REQ1_F151:	ucDT = 151	;break;
	case	CON_DATA_REQ1_F152:	ucDT = 152	;break;
	}	
*/
    if(usItemLabel > CON_DATA_REQ1_BEG_CODE)
    {
        ucDT = usItemLabel - CON_DATA_REQ1_BEG_CODE;
        if(protocol_req1_fn_valid[(ucDT-1)/32]&(1<<((ucDT-1)%32))==0)//azh ��Ч����0xff
        {
            ucDT = 0xff;
        }
    }    

	return ucDT;
}	

unsigned char GetDTbyItemLabel_ReqData_2(unsigned short usItemLabel)
{
	unsigned char ucDT=0xFF;
/*
	switch(usItemLabel)
	{
	case	CON_DATA_REQ2_F1:	ucDT = 1	;break;	//F1	�ն���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
	case	CON_DATA_REQ2_F2:	ucDT = 2	;break;	//F2	�ն��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
	case	CON_DATA_REQ2_F3:	ucDT = 3	;break;	//F3	�ն���������/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	CON_DATA_REQ2_F4:	ucDT = 4	;break;	//F4	�ն��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	CON_DATA_REQ2_F5:	ucDT = 5	;break;	//F5	�ն��������й����������ܡ�����1~M��
	case	CON_DATA_REQ2_F6:	ucDT = 6	;break;	//F6	�ն��������޹����������ܡ�����1~M��
	case	CON_DATA_REQ2_F7:	ucDT = 7	;break;	//F7	�ն��ᷴ���й����������ܡ�����1~M��
	case	CON_DATA_REQ2_F8:	ucDT = 8	;break;	//F8	�ն��ᷴ���޹����������ܡ�����1~M��
	case	CON_DATA_REQ2_F9:	ucDT = 9	;break;	//F9	�����ն���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
	case	CON_DATA_REQ2_F10:	ucDT = 10	;break;	//F10	�����ն��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
	case	CON_DATA_REQ2_F11:	ucDT = 11	;break;	//F11	�����ն���������/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	CON_DATA_REQ2_F12:	ucDT = 12	;break;	//F12	�����ն��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	CON_DATA_REQ2_F17:	ucDT = 17	;break;	//F17	�¶���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
	case	CON_DATA_REQ2_F18:	ucDT = 18	;break;	//F18	�¶��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
	case	CON_DATA_REQ2_F19:	ucDT = 19	;break;	//F19	�¶���������/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	CON_DATA_REQ2_F20:	ucDT = 20	;break;	//F20	�¶��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	CON_DATA_REQ2_F21:	ucDT = 21	;break;	//F21	�¶��������й����������ܡ�����1~M��
	case	CON_DATA_REQ2_F22:	ucDT = 22	;break;	//F22	�¶��������޹����������ܡ�����1~M��
	case	CON_DATA_REQ2_F23:	ucDT = 23	;break;	//F23	�¶��ᷴ���й����������ܡ�����1~M��
	case	CON_DATA_REQ2_F24:	ucDT = 24	;break;	//F24	�¶��ᷴ���޹����������ܡ�����1~M��
	case	CON_DATA_REQ2_F25:	ucDT = 25	;break;	//F25	�շ�������й����ʼ�����ʱ�䡢�й�����Ϊ��ʱ��
	case	CON_DATA_REQ2_F26:	ucDT = 26	;break;	//F26	���ܼ������������������ʱ��
	case	CON_DATA_REQ2_F27:	ucDT = 27	;break;	//F27	�յ�ѹͳ������
	case	CON_DATA_REQ2_F28:	ucDT = 28	;break;	//F28	�ղ�ƽ���Խ���ۼ�ʱ��
	case	CON_DATA_REQ2_F29:	ucDT = 29	;break;	//F29	�յ���Խ��ͳ��
	case	CON_DATA_REQ2_F30:	ucDT = 30	;break;	//F30	�����ڹ���Խ���ۼ�ʱ��
	case	CON_DATA_REQ2_F31:	ucDT = 31	;break;	//F31	�յ��ܱ��������
	case	CON_DATA_REQ2_F32:	ucDT = 32	;break;	//F32	����
	case	CON_DATA_REQ2_F33:	ucDT = 33	;break;	//F33	�·�������й����ʼ�����ʱ�䡢�����й�����Ϊ��ʱ��
	case	CON_DATA_REQ2_F34:	ucDT = 34	;break;	//F34	�·����й��������������ʱ�䡢���й��������������ʱ��
	case	CON_DATA_REQ2_F35:	ucDT = 35	;break;	//F35	�µ�ѹͳ������
	case	CON_DATA_REQ2_F36:	ucDT = 36	;break;	//F36	�²�ƽ���Խ���ۼ�ʱ��
	case	CON_DATA_REQ2_F37:	ucDT = 37	;break;	//F37	�µ���Խ��ͳ��
	case	CON_DATA_REQ2_F38:	ucDT = 38	;break;	//F38	�����ڹ���Խ���ۼ�ʱ��
//	case	CON_DATA_REQ2_F41:	ucDT = 41	;break;	//F41	������Ͷ���ۼ�ʱ��ʹ���
//	case	CON_DATA_REQ2_F42:	ucDT = 42	;break;	//F42	�ա��µ������ۼƲ������޹�������
	case	CON_DATA_REQ2_F43:	ucDT = 43	;break;	//F43	�չ������������ۼ�ʱ��
	case	CON_DATA_REQ2_F44:	ucDT = 44	;break;	//F44	�¹������������ۼ�ʱ��
	case	CON_DATA_REQ2_F49:	ucDT = 49	;break;	//F49	�ն��չ���ʱ�䡢�ո�λ�ۼƴ���
//	case	CON_DATA_REQ2_F50:	ucDT = 50	;break;	//F50	�ն��տ���ͳ������
	case	CON_DATA_REQ2_F51:	ucDT = 51	;break;	//F51	�ն��¹���ʱ�䡢�¸�λ�ۼƴ���
//	case	CON_DATA_REQ2_F52:	ucDT = 52	;break;	//F52	�ն��¿���ͳ������
	case	CON_DATA_REQ2_F53:	ucDT = 53	;break;	//F53	�ն�����վ��ͨ������
	case	CON_DATA_REQ2_F54:	ucDT = 54	;break;	//F54	�ն�����վ��ͨ������
//	case	CON_DATA_REQ2_F57:	ucDT = 57	;break;	//F57	�ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��
//	case	CON_DATA_REQ2_F58:	ucDT = 58	;break;	//F58	�ܼ������ۼ��й����������ܡ�����1~M��
//	case	CON_DATA_REQ2_F59:	ucDT = 59	;break;	//F59	�ܼ������ۼ��޹����������ܡ�����1~M��
//	case	CON_DATA_REQ2_F60:	ucDT = 60	;break;	//F60	�ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��
//	case	CON_DATA_REQ2_F61:	ucDT = 61	;break;	//F61	�ܼ������ۼ��й����������ܡ�����1~M��
//	case	CON_DATA_REQ2_F62:	ucDT = 62	;break;	//F62	�ܼ������ۼ��޹����������ܡ�����1~M��
//	case	CON_DATA_REQ2_F65:	ucDT = 65	;break;	//F65	�ܼ��鳬���ʶ�ֵ�����ۼ�ʱ�䡢���ۼƵ�����
//	case	CON_DATA_REQ2_F66:	ucDT = 66	;break;	//F66	�ܼ��鳬�µ�������ֵ�����ۼ�ʱ�䡢�ۼƵ�����
//	case	CON_DATA_REQ2_F73:	ucDT = 73	;break;	//F73	�ܼ����й���������
//	case	CON_DATA_REQ2_F74:	ucDT = 74	;break;	//F74	�ܼ����޹���������
//	case	CON_DATA_REQ2_F75:	ucDT = 75	;break;	//F75	�ܼ����й�����������
//	case	CON_DATA_REQ2_F76:	ucDT = 76	;break;	//F76	�ܼ����޹�����������
	case	CON_DATA_REQ2_F81:	ucDT = 81	;break;	//F81	�й���������
	case	CON_DATA_REQ2_F82:	ucDT = 82	;break;	//F82	A���й���������
	case	CON_DATA_REQ2_F83:	ucDT = 83	;break;	//F83	B���й���������
	case	CON_DATA_REQ2_F84:	ucDT = 84	;break;	//F84	C���й���������
	case	CON_DATA_REQ2_F85:	ucDT = 85	;break;	//F85	�޹���������
	case	CON_DATA_REQ2_F86:	ucDT = 86	;break;	//F86	A���޹���������
	case	CON_DATA_REQ2_F87:	ucDT = 87	;break;	//F87	B���޹���������
	case	CON_DATA_REQ2_F88:	ucDT = 88	;break;	//F88	C���޹���������
	case	CON_DATA_REQ2_F89:	ucDT = 89	;break;	//F89	A���ѹ����
	case	CON_DATA_REQ2_F90:	ucDT = 90	;break;	//F90	B���ѹ����
	case	CON_DATA_REQ2_F91:	ucDT = 91	;break;	//F91	C���ѹ����
	case	CON_DATA_REQ2_F92:	ucDT = 92	;break;	//F92	A���������
	case	CON_DATA_REQ2_F93:	ucDT = 93	;break;	//F93	B���������
	case	CON_DATA_REQ2_F94:	ucDT = 94	;break;	//F94	C���������
	case	CON_DATA_REQ2_F95:	ucDT = 95	;break;	//F95	�����������
	case	CON_DATA_REQ2_F97:	ucDT = 97	;break;	//F97	�����й��ܵ�����
	case	CON_DATA_REQ2_F98:	ucDT = 98	;break;	//F98	�����޹��ܵ�����
	case	CON_DATA_REQ2_F99:	ucDT = 99	;break;	//F99	�����й��ܵ�����
	case	CON_DATA_REQ2_F100:	ucDT = 100	;break;	//F100	�����޹��ܵ�����
	case	CON_DATA_REQ2_F101:	ucDT = 101	;break;	//F101	�����й��ܵ���ʾֵ
	case	CON_DATA_REQ2_F102:	ucDT = 102	;break;	//F102	�����޹��ܵ���ʾֵ
	case	CON_DATA_REQ2_F103:	ucDT = 103	;break;	//F103	�����й��ܵ���ʾֵ
	case	CON_DATA_REQ2_F104:	ucDT = 104	;break;	//F104	�����޹��ܵ���ʾֵ
	case	CON_DATA_REQ2_F105:	ucDT = 105	;break;	//F105	�ܹ�������
	case	CON_DATA_REQ2_F106:	ucDT = 106	;break;	//F106	A�๦������
	case	CON_DATA_REQ2_F107:	ucDT = 107	;break;	//F107	B�๦������
	case	CON_DATA_REQ2_F108:	ucDT = 108	;break;	//F108	C�๦������
	case	CON_DATA_REQ2_F109:	ucDT = 109	;break;	
	case	CON_DATA_REQ2_F110:	ucDT = 110	;break;	
//	case	CON_DATA_REQ2_F113:	ucDT = 113	;break;	//F113	A��2~19��г�����������ֵ������ʱ��
//	case	CON_DATA_REQ2_F114:	ucDT = 114	;break;	//F114	B��2~19��г�����������ֵ������ʱ��
//	case	CON_DATA_REQ2_F115:	ucDT = 115	;break;	//F115	C��2~19��г�����������ֵ������ʱ��
//	case	CON_DATA_REQ2_F116:	ucDT = 116	;break;	//F116	A��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
//	case	CON_DATA_REQ2_F117:	ucDT = 117	;break;	//F117	B��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
//	case	CON_DATA_REQ2_F118:	ucDT = 118	;break;	//F118	C��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
//	case	CON_DATA_REQ2_F121:	ucDT = 121	;break;	//F121	A��г��Խ����ͳ������
//	case	CON_DATA_REQ2_F122:	ucDT = 122	;break;	//F122	B��г��Խ����ͳ������
//	case	CON_DATA_REQ2_F123:	ucDT = 123	;break;	//F123	C��г��Խ����ͳ������
//	case	CON_DATA_REQ2_F129:	ucDT = 129	;break;	//F129	ֱ��ģ����Խ�����ۼ�ʱ�䡢���/��Сֵ������ʱ��
//	case	CON_DATA_REQ2_F130:	ucDT = 130	;break;	//F130	ֱ��ģ����Խ�����ۼ�ʱ�䡢���/��Сֵ������ʱ��
//	case	CON_DATA_REQ2_F138:	ucDT = 138	;break;	//F138	ֱ��ģ������������
	case	CON_DATA_REQ2_F145:	ucDT = 145	;break;	//F145	һ�����޹��ܵ���ʾֵ����
	case	CON_DATA_REQ2_F146:	ucDT = 146	;break;	//F146	�������޹��ܵ���ʾֵ����
	case	CON_DATA_REQ2_F147:	ucDT = 147	;break;	//F147	�������޹��ܵ���ʾֵ����
	case	CON_DATA_REQ2_F148:	ucDT = 148	;break;	//F148	�������޹��ܵ���ʾֵ����
	case 	CON_DATA_REQ2_F153:	ucDT = 153	;break;
	case 	CON_DATA_REQ2_F154:	ucDT = 154	;break;
	case 	CON_DATA_REQ2_F155:	ucDT = 155	;break;
	case 	CON_DATA_REQ2_F156:	ucDT = 156	;break;
	case 	CON_DATA_REQ2_F157:	ucDT = 157	;break;
	case 	CON_DATA_REQ2_F158:	ucDT = 158	;break;
	case 	CON_DATA_REQ2_F159:	ucDT = 159	;break;
	case 	CON_DATA_REQ2_F160:	ucDT = 160	;break;
	case	CON_DATA_REQ2_F161:	ucDT = 161	;break;
	case	CON_DATA_REQ2_F162:	ucDT = 162	;break;
	case	CON_DATA_REQ2_F163:	ucDT = 163	;break;
	case	CON_DATA_REQ2_F164:	ucDT = 164	;break;
	case	CON_DATA_REQ2_F165:	ucDT = 165	;break;
	case	CON_DATA_REQ2_F166:	ucDT = 166	;break;
	case	CON_DATA_REQ2_F167:	ucDT = 167	;break;
	case	CON_DATA_REQ2_F168:	ucDT = 168	;break;
	case	CON_DATA_REQ2_F169:	ucDT = 169	;break;
	case	CON_DATA_REQ2_F170:	ucDT = 170	;break;
	case	CON_DATA_REQ2_F171:	ucDT = 171	;break;
	case	CON_DATA_REQ2_F172:	ucDT = 172	;break;
	case	CON_DATA_REQ2_F173:	ucDT = 173	;break;
	case	CON_DATA_REQ2_F174:	ucDT = 174	;break;
	case	CON_DATA_REQ2_F175:	ucDT = 175	;break;
	case	CON_DATA_REQ2_F176:	ucDT = 176	;break;
	case	CON_DATA_REQ2_F177:	ucDT = 177	;break;
	case	CON_DATA_REQ2_F178:	ucDT = 178	;break;
	case	CON_DATA_REQ2_F179:	ucDT = 179	;break;
	case	CON_DATA_REQ2_F180:	ucDT = 180	;break;
	case	CON_DATA_REQ2_F181:	ucDT = 181	;break;
	case	CON_DATA_REQ2_F182:	ucDT = 182	;break;
	case	CON_DATA_REQ2_F183:	ucDT = 183	;break;
	case	CON_DATA_REQ2_F184:	ucDT = 184	;break;
	case	CON_DATA_REQ2_F185:	ucDT = 185	;break;
	case	CON_DATA_REQ2_F186:	ucDT = 186	;break;
	case	CON_DATA_REQ2_F187:	ucDT = 187	;break;
	case	CON_DATA_REQ2_F188:	ucDT = 188	;break;
	case	CON_DATA_REQ2_F189:	ucDT = 189	;break;
	case	CON_DATA_REQ2_F190:	ucDT = 190	;break;
	case	CON_DATA_REQ2_F191:	ucDT = 191	;break;
	case	CON_DATA_REQ2_F192:	ucDT = 192	;break;
	case	CON_DATA_REQ2_F193:	ucDT = 193	;break;
	case	CON_DATA_REQ2_F194:	ucDT = 194	;break;
	case	CON_DATA_REQ2_F195:	ucDT = 195	;break;
	case	CON_DATA_REQ2_F196:	ucDT = 196	;break;
	}	
*/
    if(usItemLabel > CON_DATA_REQ2_BEG_CODE)
    {
        ucDT = usItemLabel - CON_DATA_REQ2_BEG_CODE;
        if(protocol_req2_fn_valid[(ucDT-1)/32]&(1<<((ucDT-1)%32))==0)//azh ��Ч����0xff
        {
            ucDT = 0xff;
        }
    }    
	return ucDT;
}
unsigned char GetDTbyItemLabel_ReqData_3(unsigned short usItemLabel)
{
	unsigned char ucDT=0xFF;
	switch(usItemLabel)
	{
	case	CON_DATA_REQ3_F1:		ucDT =	1	;break;	//��Ҫ�¼�						
	case	CON_DATA_REQ3_F2:		ucDT =	2	;break;	//һ���¼�
	}
	return ucDT;
}

 unsigned char GetDTbyItemLabel_ExtCmd(unsigned short usItemLabel)
 {
 	unsigned char ucDT=0xFF;
	switch(usItemLabel)
	{
//	case	CON_EXTCMD_F1:			ucDT=1		;break;	//F1 �ڴ��						
//	case	CON_EXTCMD_F2:			ucDT=2		;break;	//F2 �ڴ�д
//	case	CON_EXTCMD_F3:			ucDT=3		;break;	//F3 DATAFLASH��
//	case	CON_EXTCMD_F4:			ucDT=4		;break;	//F4 DATAFLASHд
//	case	CON_EXTCMD_F5:			ucDT=5		;break;	//F5 ���Ź����ԣ��ó��������ѭ��
//	case	CON_EXTCMD_F6:			ucDT=6		;break;	//F6 �ֻ�ģ��ͨѶ���
//	case	CON_EXTCMD_F7:			ucDT=7		;break;	//F7 �ն�״̬���
	case	CON_EXTCMD_F10:		ucDT=10		;break;	//F10 �ն˵�ַ10/16�����л�
//	case	CON_EXTCMD_F11:		ucDT=11		;break;	//F11 ������Ϣ��ȡ
//	case	CON_EXTCMD_F21:		ucDT=21		;break;	//F21 GR�����					
//	case	CON_EXTCMD_F22:		ucDT=22		;break;	//F22 GR��������
//	case	CON_EXTCMD_F23:		ucDT=23		;break;	//F23 GR������Ϣ��
//	case	CON_EXTCMD_F24:		ucDT=24		;break;	//F24 GR������Ϣ����
//	case	CON_EXTCMD_F31:		ucDT=31		;break;	//F31 ������������					
//	case	CON_EXTCMD_F33:		ucDT=33		;break;	//F33 ����׶���������ȫ
//	case	CON_EXTCMD_F40:		ucDT=40		;break;	//F40 �����ڱ�����
//	case	CON_EXTCMD_F41:		ucDT=41		;break;	//F41 ��ȡ�ڱ�����
//	case	CON_EXTCMD_F51:		ucDT=51		;break;	//F51 �ն˵�ַ					
//	case	CON_EXTCMD_F52:		ucDT=52		;break;	//F52 �ն���Ŷ�
//	case	CON_EXTCMD_F53:		ucDT=53		;break;	//F53 �ն˵�ַ��������д
//	case	CON_EXTCMD_F54:		ucDT=54		;break;	//F54 �ն����д
//	case	CON_EXTCMD_F71:		ucDT=71		;break;	//F71 ����������
//	case	CON_EXTCMD_F72:		ucDT=72		;break;	//F72 �޸�ʱ��
//	case	CON_EXTCMD_F73:		ucDT=73		;break;	//F73 ��ѯͨѶ������
//	case	CON_EXTCMD_F74:		ucDT=74		;break;	//F74 �޸�ͨѶ������
//	case	CON_EXTCMD_F75:		ucDT=75		;break;	//F75 ��ȡҺ��������ʾ����
//	case	CON_EXTCMD_F76:		ucDT=76		;break;	//F76 ��ȡҺ��������ʾ����
//	case	CON_EXTCMD_F77:		ucDT=77		;break;	//F77 ��ȡҺ��������ʾ����
//	case	CON_EXTCMD_F78:		ucDT=78		;break;	//F78 �޸�Һ��������ʾ����
//	case	CON_EXTCMD_F79:		ucDT=79		;break;	//F79 �޸�Һ��������ʾ����
//	case	CON_EXTCMD_F80:		ucDT=80		;break;	//F80 �޸�Һ��������ʾ����
//	case	CON_EXTCMD_F81:		ucDT=81		;break;	//F81 ����SIM����pin
//	case	CON_EXTCMD_F82:		ucDT=82		;break;	//F82 �޸�SIM����pin
//	case	CON_EXTCMD_F83:		ucDT=83		;break;	//F83 ��PUK����SIM����pin
//	case	CON_EXTCMD_F84:		ucDT=84		;break;	//F84 SIM����pin���״̬
//	case	CON_EXTCMD_F85:		ucDT=85		;break;	//F85 GSM/GPRS�л�
//	case	CON_EXTCMD_F86:		ucDT=86		;break;	//F86 �����ʿ��ƻָ�ʱ�䳤��
//	case	CON_EXTCMD_F87:		ucDT=87		;break;	//F87 д���ʿ��ƻָ�ʱ�䳤��
//	case	CON_EXTCMD_F89:		ucDT=89		;break;	//F89 �����ʿ���Խ��ȷ��ʱ��
//	case	CON_EXTCMD_F90:		ucDT=90		;break;	//F90 д���ʿ���Խ��ȷ��ʱ��
//	case	CON_EXTCMD_F91:		ucDT=91		;break;	//F91 ���Զ����������
//	case	CON_EXTCMD_F92:		ucDT=92		;break;	//F92 д�Զ����������
//	case	CON_EXTCMD_F93:		ucDT=93		;break;	//F93 ��ȡ�ն˵�ѹ��λС��������λС��
//	case	CON_EXTCMD_F94:		ucDT=94		;break;	//F94 �����ʼ��
//	case	CON_EXTCMD_F95:		ucDT=95		;break;	//F95 8025ʱ�ӽ���
//	case	CON_EXTCMD_F96:		ucDT=96		;break;	//F96 Һ���Աȶ�����
//	case	CON_EXTCMD_F97:		ucDT=97		;break;	//F97 ���ñ��ͨѸ��������
//	case	CON_EXTCMD_F98:		ucDT=98		;break;	//F98 ��ȡ���ͨѸ��������
//	case	CON_EXTCMD_F99:		ucDT=99		;break;	//F99 ����GPRS�����PDP�û���������
//	case	CON_EXTCMD_F100:	ucDT=100	;break;	//F100 ��ȡGPRS�����PDP�û���������
//	case	CON_EXTCMD_F101:	ucDT=101	;break;	//F101 ����GPRS�������ʱ��
//	case	CON_EXTCMD_F102:	ucDT=102	;break;	//F102 ��ȡGPRS�������ʱ��
//	case	CON_EXTCMD_F109:	ucDT=109	;break;	//F109 �����ն˹���ģʽ
//	case	CON_EXTCMD_F110:	ucDT=110	;break;	//F110 ��ȡ�ն˹���ģʽ
//	case	CON_EXTCMD_F111:	ucDT=111	;break;	//F111 ����CT����
//	case	CON_EXTCMD_F112:	ucDT=112	;break;	//F112 ��ȡCT����
//	case	CON_EXTCMD_F113:	ucDT=113	;break;	//F113 ������ѧϰ��Ч��־
//	case	CON_EXTCMD_F114:	ucDT=114	;break;	//F114 ��ȡ��ѧϰ��Ч��־
//	case	CON_EXTCMD_F115:	ucDT=115	;break;	//F115
//	case	CON_EXTCMD_F116:	ucDT=116	;break;	//F116
//	case	CON_EXTCMD_F117:	ucDT=117	;break;	//F117
//	case	CON_EXTCMD_F118:	ucDT=118	;break;	//F118
//	case	CON_EXTCMD_F119:	ucDT=119	;break;	//F119
//	case	CON_EXTCMD_F120:	ucDT=120	;break;	//F120 ����AES������Կ
//	case	CON_EXTCMD_F121:	ucDT=121	;break;	//F121 ��ȡAES������Կ	
//	case	CON_EXTCMD_F122:	ucDT=122	;break; //F122 Ͷ��ӽ�����߱������
//	case	CON_EXTCMD_F123:	ucDT=123	;break; //F122 Ͷ��ӽ�����߱������
//	case	CON_EXTCMD_F124:	ucDT=124	;break;	//F124 ��ȡУ��EEPROM�Ĳ���
//	case	CON_EXTCMD_F125:	ucDT=125	;break;	//F125 ����У��EEPROM�Ĳ���
//	case 	CON_EXTCMD_F126:	ucDT=126	;break;	
	}
 	return ucDT;
 }

unsigned char GetDTbyItemLabel_DataTrans(unsigned short usItemLabel)
{
	unsigned char ucDT=0xFF;
	switch(usItemLabel)
	{
	case	CON_DATATRANS_F1:		ucDT=1		;break;	//F1 ת������
	case 	CON_DATATRANS_F9:		ucDT = 9;	break;
	case 	CON_DATATRANS_F10:		ucDT = 10;	break;
	case 	CON_DATATRANS_F11:		ucDT = 11;	break;
	}
	return ucDT;
}

unsigned char GetDTbyItemLabel_FileTrans(unsigned short usItemLabel)
{
	unsigned char ucDT=0xFF;
	switch(usItemLabel)
	{
	case	CON_FILETRANS_F1:		ucDT=1		;break;	//F1 �ļ����䷽ʽ1						
	case	CON_FILETRANS_F100:		ucDT=100	;break;	//F100 ������װ����					
	}
	return ucDT;
}

unsigned short GetItemLabelbyDT_Ack(unsigned char ucDT)
{
	unsigned short usItemLabel = CON_DI_TYPE_INVALID;
	switch(ucDT)
	{
	case	1:	usItemLabel = CON_ACK_F1;	break;		//F1:ȫ��ȷ��
	case	2:	usItemLabel = CON_ACK_F2;	break;		//F2:ȫ������
	case	3:	usItemLabel = CON_ACK_F3;	break;		//F3:�����������ȷ�Ϸ���
	}
	return usItemLabel;
}

unsigned short GetItemLabelbyDT_RST(unsigned char ucDT)
{
	unsigned short usItemLabel = CON_DI_TYPE_INVALID;
	switch(ucDT)
	{
	case	1:	usItemLabel = CON_RST_F1_HARD;	break;		//F1:Ӳ����ʼ��
	case	2:	usItemLabel = CON_RST_F2_DATA;	break;		//F2:��������ʼ��
	case	3:	usItemLabel = CON_RST_F3_ALL;	break;		//F3:ȫ������
	case 	4:	usItemLabel = CON_RST_F4_EXCECOMM;break;
	}
	return usItemLabel;
}

unsigned short GetItemLabelbyDT_Para(unsigned char ucDT)
{
	unsigned short usItemLabel=CON_DI_TYPE_INVALID;
/*
	switch(ucDT)
	{
	case	1:	usItemLabel = CON_PARA_F1_COMMPARA;		break;	//F1���ն�ͨ�Ų�������
	case	2:	usItemLabel = CON_PARA_F2_RELAYPARA;	break;	//F2���ն��м�ת������
	case	3:	usItemLabel = CON_PARA_F3_STAIPADDR;	break;	//F3����վIP��ַ�Ͷ˿�
	case	4:	usItemLabel = CON_PARA_F4_STAPHONENO;	break;	//F4����վ�绰����Ͷ������ĺ���
	case	5:	usItemLabel = CON_PARA_F5_TERENCRYPT;	break;	//F5���ն���������
	case	6:	usItemLabel = CON_PARA_F6_TERGROUPADDR;	break;	//F6���ն����ַ����
	case	7:	usItemLabel = CON_PARA_F7_IPADDR;		break;	//F7���ն˳���������
	case	8:	usItemLabel = CON_PARA_F8_ALRCTLWORD;	break;	//F8���ն��¼���¼��������
	case	9:	usItemLabel = CON_PARA_F9_CONFIGNUM;	break;	//F9���ն�����������
	case	10:	usItemLabel = CON_PARA_F10_METERPARA;	break;	//F10���ն˵��ܱ�/��������װ�����ò���
//	case	11:	usItemLabel = CON_PARA_F11_PULSEPARA;	break;	//F11���ն��������ò���
//	case	12:	usItemLabel = CON_PARA_F12_DINPUTPARA;	break;	//F12���ն�״̬���������
//	case	13:	usItemLabel = CON_PARA_F13_ANALOGPARA;	break;	//F13���ն˵�ѹ/����ģ�������ò���
//	case	14:	usItemLabel = CON_PARA_F14_SUMGPARA;	break;	//F14���ն��ܼ������ò���
//	case	15:	usItemLabel = CON_PARA_F15_POWVALDIFF;	break;	//F15���й��ܵ������Խ���¼���������
	case	16:	usItemLabel = CON_PARA_F16_PDPINFO;	break;	//F16������ר���û���������
//	case	17:	usItemLabel = CON_PARA_F17_KEEPPOWER;	break;	//F17���ն˱�����ֵ
//	case	18:	usItemLabel = CON_PARA_F18_POWCTRLTIMEPER;	break;	//F18���ն˹���ʱ��
//	case	19:	usItemLabel = CON_PARA_F19_POWCTRLCOEF;	break;	//F19���ն�ʱ�ι��ض�ֵ����ϵ��
//	case	20:	usItemLabel = CON_PARA_F20_ENERCTRLCOEF;	break;	//F20���ն��µ������ض�ֵ����ϵ��
//	case	21:	usItemLabel = CON_PARA_F21_PERTIMEFEE;	break;	//F21���ն˵���������ʱ�κͷ�����
//	case	22:	usItemLabel = CON_PARA_F22_FEEPARA;		break;	//F22���ն˵���������
//	case	23:	usItemLabel = CON_PARA_F23_URGEALARMPARA;	break;	//F23���ն˴߷Ѹ澯����
	case	24:	usItemLabel = CON_PARA_F24_COPYINTER;	break;	//F24���ն˳���������
	case	25:	usItemLabel = CON_PARA_F25_MPBASEPARA;	break;	//F25���������������
	case	26:	usItemLabel = CON_PARA_F26_MPLIMIT;		break;	//F26����������ֵ����
	case	27:	usItemLabel = CON_PARA_F27_MPFREEZEPARA;	break;	//F27�����������ݶ������
	case	28:	usItemLabel = CON_PARA_F28_MPPOWRATEPARA;	break;	//F28�������㹦�������ֶ���ֵ
	case	29:	usItemLabel = CON_PARA_F29_METERADDR;	break;
	case 	30:	usItemLabel = CON_PARA_F30_METERCOPYFLAG;	break;	//F30:
	case 	31: usItemLabel = CON_PARA_F31_SLAVENODE;		break;
	case 	34:	usItemLabel = CON_PARA_F34_PORTPROPERTY;break;
	case 	35:	usItemLabel = CON_PARA_F35_EMPHAFLAG;	break;
//	case	33:	usItemLabel = CON_PARA_F33_SUMGFREEZEPARA;	break;	//F33���ܼ������ݶ������
	case 	37:	usItemLabel = CON_PARA_F37_LINKPARA;			break;	//F37:����ͨ�Ų���
	case 	38:	usItemLabel = CON_PARA_F38_JOB1PARA;			break;	//F38:һ��������������
	case 	39:  usItemLabel = CON_PARA_F39_JOB2PARA;			break;	//F39:����������������
//	case	41:	usItemLabel = CON_PARA_F41_TIMEPERSETTING;	break;	//F41��ʱ�ι��ض�ֵ
//	case	42:	usItemLabel = CON_PARA_F42_RESTCTRL;	break;	//F42�����ݹ��ز���
//	case	43:	usItemLabel = CON_PARA_F43_POWERSLIP;	break;	//F43�����ʿ��ƵĹ��ʼ��㻬��ʱ��
//	case	44:	usItemLabel = CON_PARA_F44_SALESTOP;	break;	//F44��Ӫҵ��ͣ�ز���
//	case	45:	usItemLabel = CON_PARA_F45_POWERCTRLTURN;	break;	//F45�������ִ��趨
//	case	46:	usItemLabel = CON_PARA_F46_MONTHENERCTRL;	break;	//F46���µ����ض�ֵ
//	case	47:	usItemLabel = CON_PARA_F47_BUYENERCTRL;	break;	//F47���������ز���
//	case	48:	usItemLabel = CON_PARA_F48_ENERCTRLTURN;	break;	//F48������ִ��趨
//	case	49:	usItemLabel = CON_PARA_F49_CTRLALRTIME;	break;	//F49�����ظ澯ʱ��
	case	57:	usItemLabel = CON_PARA_F57_SOUNDALARM;	break;	//F57���ն������澯����M��ֹ����
//	case	58:	usItemLabel = CON_PARA_F58_KEEPAUTO;	break;	//F58���ն��Զ��������
	case	59:	usItemLabel = CON_PARA_F59_MTRERRORPARA;	break;	//F59�����ܱ��쳣�б���ֵ�趨
//	case	60:	usItemLabel = CON_PARA_F60_HARMONPARA;	break;	//F60��г����ֵ
//	case	61:	usItemLabel = CON_PARA_F61_DCAINPARA;	break;	//F61��ֱ��ģ�����������
	case	62:	usItemLabel = CON_PARA_F62_COMMTYPE;	break;	//F62 ����ר��������ʽ
	case	36:	usItemLabel = CON_PARA_F63_FLUXLIMIT;	break;	//F63 ��ͨ��������������
	case	65:	usItemLabel = CON_PARA_F65_UPDATA1PARA;	break;	//F65����ʱ����1��������������
	case	66:	usItemLabel = CON_PARA_F66_UPDATA2PARA;	break;	//F66����ʱ����2��������������
	case	67:	usItemLabel = CON_PARA_F67_UPDATA1CTRL;	break;	//F67����ʱ����1��������������/ֹͣ����
	case	68:	usItemLabel = CON_PARA_F68_UPDATA2CTRL;	break;	//F68����ʱ����2��������������/ֹͣ����
	case	73:	usItemLabel = CON_PARA_F73_CAPPARA;		break;	//F73������������
//	case	74:	usItemLabel = CON_PARA_F74_CAPRUNPARA;	break;	//F74��������Ͷ�����в���
//	case	75:	usItemLabel = CON_PARA_F75_CAPRELAYPARA;	break;	//F75����������������
//	case	76:	usItemLabel = CON_PARA_F76_CAPCTRL;		break;	//F76��������Ͷ�п��Ʒ�ʽ
//	case	81:	usItemLabel = CON_PARA_F81_DCARATE;		break;	//F81��ֱ��ģ����������
//	case	82:	usItemLabel = CON_PARA_F82_DCALIMIT;	break;	//F82��ֱ��ģ������ֵ
//	case	83:	usItemLabel = CON_PARA_F83_DCAFREESE;	break;	//F83��ֱ��ģ�����������
	case	89:	usItemLabel = CON_PARA_F89_ADDR;	    break;	//F89���ն˵�ַ��Ϣ
	case	207:usItemLabel = CON_PARA_F207_ADDR;	    break;
	}
*/
    if(ucDT != 0)
    {
        if(protocol_para_fn_valid[(ucDT-1)/32]&(1<<((ucDT-1)%32)))
        {
            usItemLabel = CON_PARA_BEG_CODE + ucDT;	
        }
    }
	return usItemLabel;
}

unsigned short GetItemLabelbyDT_Config(unsigned char ucDT)
{
	unsigned short usItemLabel=CON_DI_TYPE_INVALID;
	switch(ucDT)
	{
	case 1:		usItemLabel = CON_CONFIG_F1_VERSION;			break;//F1�����ն˰汾��Ϣ
	case 2:		usItemLabel = CON_CONFIG_F2_INTERFACE;			break;//F2�����ն����롢�����ͨ�Ŷ˿�����
	case 3:		usItemLabel = CON_CONFIG_F3_OTHER;				break;//F3�����ն���������
	case 4:		usItemLabel = CON_CONFIG_F4_PARA;				break;//F4�����ն˲������ã����ն���Ӳ���汾֧�ֵĲ�����
	case 5:		usItemLabel = CON_CONFIG_F5_CTRL;				break;//F5�����ն˿������ã����ն���Ӳ���汾֧�ֵĿ������
	case 6:		usItemLabel = CON_CONFIG_F6_DATA1;				break;//F6�����ն�1���������ã����ն���Ӳ���汾֧�ֵ�1�����ݣ�
	case 7:		usItemLabel = CON_CONFIG_F7_DATA2;				break;//F7�����ն�2���������ã����ն���Ӳ���汾֧�ֵ�2�����ݣ�
	case 8:		usItemLabel = CON_CONFIG_F8_ERC;				break;//F8�����ն��¼���¼���ã����ն���Ӳ���汾֧�ֵ��¼���¼��
//azh 170920
	case 9:		usItemLabel = CON_CONFIG_F9_REMOTEVER;			break;//F9Զ��ͨ��ģ��汾��Ϣ    
	case 10:	usItemLabel = CON_CONFIG_F10_LOCALVER;			break;//F10����ͨ��ģ��汾��Ϣ    	    
	}
	return usItemLabel;
}


unsigned short GetItemLabelbyDT_Login(unsigned char ucDT)
{
	unsigned short usItemLabel=CON_DI_TYPE_INVALID;
	switch(ucDT)
	{
	case 1:		usItemLabel = CON_LOGIN_F1_REQUEST;			break;//F1�����֤����
	case 2:		usItemLabel = CON_LOGIN_F2_ANSWER;			break;//F2�����֤��Ӧ
	}
	return usItemLabel;
}


unsigned short GetItemLabelbyDT_Ctrl(unsigned char ucDT)
{
	unsigned short usItemLabel=CON_DI_TYPE_INVALID;
/*
	switch(ucDT)
	{
//	case 1:		usItemLabel = CON_CTRL_F1_YK_OPEN;			break;//F1ң����բ
//	case 2:		usItemLabel = CON_CTRL_F2_YK_CLOSE;			break;//F2ң�������բ
//	case 9:		usItemLabel = CON_CTRL_F9_TIMEPER_ON;		break;//F9ʱ�ι���Ͷ��
//	case 10:	usItemLabel = CON_CTRL_F10_RESTCTRL_ON;		break;//F10���ݹ���Ͷ��
//	case 11:	usItemLabel = CON_CTRL_F11_SALESTOP_ON;		break;//F11Ӫҵ��ͣ����Ͷ��
//	case 12:	usItemLabel = CON_CTRL_F12_POWERLOWER_ON;	break;//F12��ǰ�����¸���Ͷ��
//	case 15:	usItemLabel = CON_CTRL_F15_MONTHENER_ON;	break;//F15�µ��Ͷ��
//	case 16:	usItemLabel = CON_CTRL_F16_BUYENER_ON;		break;//F16�����Ͷ��
//	case 17:	usItemLabel = CON_CTRL_F17_TIMEPER_OFF;		break;//F17ʱ�ι��ؽ��
//	case 18:	usItemLabel = CON_CTRL_F18_RESTCTRL_OFF;	break;//F18���ݹ��ؽ��
//	case 19:	usItemLabel = CON_CTRL_F19_SALESTOP_OFF;	break;//F19Ӫҵ��ͣ���ؽ��
//	case 20:	usItemLabel = CON_CTRL_F20_POWERLOWER_OFF;	break;//F20��ǰ�����¸��ؽ��
//	case 23:	usItemLabel = CON_CTRL_F23_MONTHENER_OFF;	break;//F23�µ�ؽ��
//	case 24:	usItemLabel = CON_CTRL_F24_BUYENER_OFF;		break;//F24����ؽ��
//	case 25:	usItemLabel = CON_CTRL_F25_KEEPPOWER_ON;	break;//F25�ն˱���Ͷ��
//	case 26:	usItemLabel = CON_CTRL_F26_URGEALARM_ON;	break;//F26�߷Ѹ澯Ͷ��
	case 27:	usItemLabel = CON_CTRL_F27_TALK_ON;			break;//F27�����ն�����վͨ��
//	case 28:	usItemLabel = CON_CTRL_F28_REMOVE_ON;		break;//F28�ն��޳�Ͷ��
	case 29:	usItemLabel = CON_CTRL_F29_UPACTIVE_ON;		break;//F29�����ն������ϱ�
	case 31:	usItemLabel = CON_CTRL_F31_SETTIME;			break;//F31��ʱ����
	case 32:	usItemLabel = CON_CTRL_F32_MESSAGE;			break;//F32������Ϣ
//	case 33:	usItemLabel = CON_CTRL_F33_KEEPPOWER_OFF;	break;//F33�ն˱�����
//	case 34:	usItemLabel = CON_CTRL_F34_URGEFEE_OFF;		break;//F34�߷Ѹ澯���
	case 35:	usItemLabel = CON_CTRL_F35_TALK_OFF;		break;//F35��ֹ�ն�����վͨ��
//	case 36:	usItemLabel = CON_CTRL_F36_REMOVE_OFF;		break;//F36�ն��޳����
	case 37:	usItemLabel = CON_CTRL_F37_UPACTIVE_OFF;	break;//F37��ֹ�ն������ϱ�
	case 39:	usItemLabel = CON_CTRL_F39_DISCONECT;		break;//F38���ż���
//	case 41:	usItemLabel = CON_CTRL_F41_CAPCTRL_ON;		break;//F41���ݿ���Ͷ��
//	case 42:	usItemLabel = CON_CTRL_F42_CAPCTRL_OFF;		break;//F42���ݿ��ƽ��
//	case 88:	usItemLabel = CON_CTRL_F88_TIMEREVISE;		break;//F88��ȷ��ʱ����

	case 38:	usItemLabel = CON_CTRL_F38_SMS_ACTIVE;		break;//F38		changed for test 09-11-25
//	case 49:	usItemLabel = CON_CTRL_F49;		break;//F41���ݿ���Ͷ��
//	case 50:	usItemLabel = CON_CTRL_F50;		break;//F41���ݿ���Ͷ��
//	case 51:	usItemLabel = CON_CTRL_F51;		break;//F41���ݿ���Ͷ��
	}
*/	
    if(ucDT != 0)
    {
        if(protocol_ctrl_fn_valid[(ucDT-1)/32]&(1<<((ucDT-1)%32)))
        {
            usItemLabel = CON_CTRL_BEG_CODE + ucDT;	
        }
    }

	return usItemLabel;
}

unsigned short GetItemLabelbyDT_TaskData(unsigned char ucDT)
{
	unsigned short usItemLabel=CON_DI_TYPE_INVALID;
	switch(ucDT)
	{
	case	1:		usItemLabel = CON_TASKDATA_REQ_F1;		break;	//F1 1������
	case	2:		usItemLabel = CON_TASKDATA_REQ_F2;		break;	//F2 2������
	}
	return usItemLabel;
}

unsigned short GetItemLabelbyDT_ReqData_1(unsigned char ucDT)
{
	unsigned short usItemLabel=CON_DI_TYPE_INVALID;
/*
	switch(ucDT)
	{
	case	1:	usItemLabel = 	CON_DATA_REQ1_F1	;break;	//F1	�ն˰汾��Ϣ
	case	2:	usItemLabel = 	CON_DATA_REQ1_F2	;break;	//F2	�ն�����ʱ��
	case	3:	usItemLabel = 	CON_DATA_REQ1_F3	;break;	//F3	�ն˲���״̬
	case	4:	usItemLabel = 	CON_DATA_REQ1_F4	;break;	//F4	�ն�ͨ��״̬
//	case	5:	usItemLabel = 	CON_DATA_REQ1_F5	;break;	//F5	�ն˿�������״̬
//	case	6:	usItemLabel = 	CON_DATA_REQ1_F6	;break;	//F6	�ն˵�ǰ����״̬
	case	7:	usItemLabel = 	CON_DATA_REQ1_F7	;break;	//F7	�ն��¼���������ǰֵ
	case	8:	usItemLabel = 	CON_DATA_REQ1_F8	;break;	//F8	�ն��¼���־״̬
	case	9:	usItemLabel = 	CON_DATA_REQ1_F9	;break;	//F9	�ն�״̬������λ��־
	case	10:	usItemLabel = 	CON_DATA_REQ1_F10	;break;	//F10	�ն�����վ���ա���ͨ������
	case	11:	usItemLabel = 	CON_DATA_REQ1_F11	;break;	//F11	��ǰ�ź�ǿ��
//	case	17:	usItemLabel = 	CON_DATA_REQ1_F17	;break;	//F17	��ǰ�ܼ��й�����
//	case	18:	usItemLabel = 	CON_DATA_REQ1_F18	;break;	//F18	��ǰ�ܼ��޹�����
//	case	19:	usItemLabel = 	CON_DATA_REQ1_F19	;break;	//F19	�����ܼ��й����������ܡ�����1~M��
//	case	20:	usItemLabel = 	CON_DATA_REQ1_F20	;break;	//F20	�����ܼ��޹����������ܡ�����1~M��
//	case	21:	usItemLabel = 	CON_DATA_REQ1_F21	;break;	//F21	�����ܼ��й����������ܡ�����1~M��
//	case	22:	usItemLabel = 	CON_DATA_REQ1_F22	;break;	//F22	�����ܼ��޹����������ܡ�����1~M��
//	case	23:	usItemLabel = 	CON_DATA_REQ1_F23	;break;	//F23	�ն˵�ǰʣ��������ѣ�
//	case	24:	usItemLabel = 	CON_DATA_REQ1_F24	;break;	//F24	��ǰ�����¸��ؿغ��ܼ��й����ʶ���ֵ
	case	25:	usItemLabel = 	CON_DATA_REQ1_F25	;break;	//F25	��ǰ���༰����/�޹����ʡ����������������ѹ���������������
	case	26:	usItemLabel = 	CON_DATA_REQ1_F26	;break;	//F26	A��B��C�������ͳ�����ݼ����һ�ζ����¼
	case	27:	usItemLabel = 	CON_DATA_REQ1_F27	;break;	//F27	���ܱ�����ʱ�ӡ���̴����������һ�β���ʱ��
	case	28:	usItemLabel = 	CON_DATA_REQ1_F28	;break;	//F28	�������״̬�ּ����λ��־
	case 	31:	usItemLabel = 	CON_DATA_REQ1_F31	;break;
	case 	32:	usItemLabel = 	CON_DATA_REQ1_F32	;break;
	case	33:	usItemLabel = 	CON_DATA_REQ1_F33	;break;	//F33	��ǰ������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
	case	34:	usItemLabel = 	CON_DATA_REQ1_F34	;break;	//F34	��ǰ������/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
	case	35:	usItemLabel = 	CON_DATA_REQ1_F35	;break;	//F35	����������/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	36:	usItemLabel = 	CON_DATA_REQ1_F36	;break;	//F36	���·�����/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	37:	usItemLabel = 	CON_DATA_REQ1_F37	;break;	//F37	����������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
	case	38:	usItemLabel = 	CON_DATA_REQ1_F38	;break;	//F38	���·�����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
	case	39:	usItemLabel = 	CON_DATA_REQ1_F39	;break;	//F39	����������/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	40:	usItemLabel = 	CON_DATA_REQ1_F40	;break;	//F40	���·�����/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	41:	usItemLabel = 	CON_DATA_REQ1_F41	;break;	//F41	���������й����������ܡ�����1~M��
	case	42:	usItemLabel = 	CON_DATA_REQ1_F42	;break;	//F42	���������޹����������ܡ�����1~M��
	case	43:	usItemLabel = 	CON_DATA_REQ1_F43	;break;	//F43	���շ����й����������ܡ�����1~M��
	case	44:	usItemLabel = 	CON_DATA_REQ1_F44	;break;	//F44	���շ����޹����������ܡ�����1~M��
	case	45:	usItemLabel = 	CON_DATA_REQ1_F45	;break;	//F45	���������й����������ܡ�����1~M��
	case	46:	usItemLabel = 	CON_DATA_REQ1_F46	;break;	//F46	���������޹����������ܡ�����1~M��
	case	47:	usItemLabel = 	CON_DATA_REQ1_F47	;break;	//F47	���·����й����������ܡ�����1~M��
	case	48:	usItemLabel = 	CON_DATA_REQ1_F48	;break;	//F48	���·����޹����������ܡ�����1~M��
	case	49:	usItemLabel = 	CON_DATA_REQ1_F49	;break;	//F49	��ǰ��ѹ��������λ��
	case	57:	usItemLabel = 	CON_DATA_REQ1_F57	;break;	//F57	��ǰA��B��C�����ѹ������2~N��г����Чֵ
	case	58:	usItemLabel = 	CON_DATA_REQ1_F58	;break;	//F58	��ǰA��B��C�����ѹ������2~N��г��������
//	case	65:	usItemLabel = 	CON_DATA_REQ1_F65	;break;	//F65	��ǰ������Ͷ��״̬
//	case	66:	usItemLabel = 	CON_DATA_REQ1_F66	;break;	//F66	��ǰ�������ۼƲ���Ͷ��ʱ��ʹ���
//	case	67:	usItemLabel = 	CON_DATA_REQ1_F67	;break;	//F67	���ա����µ������ۼƲ������޹�������
//	case	73:	usItemLabel = 	CON_DATA_REQ1_F73	;break;	//F73	ֱ��ģ����ʵʱ����
//	case	81:	usItemLabel = 	CON_DATA_REQ1_F81	;break;	//F81	Сʱ�����ܼ��й�����
//	case	82:	usItemLabel = 	CON_DATA_REQ1_F82	;break;	//F82	Сʱ�����ܼ��޹�����
//	case	83:	usItemLabel = 	CON_DATA_REQ1_F83	;break;	//F83	Сʱ�����ܼ��й��ܵ�����
//	case	84:	usItemLabel = 	CON_DATA_REQ1_F84	;break;	//F84	Сʱ�����ܼ��޹��ܵ�����
	case	89:	usItemLabel = 	CON_DATA_REQ1_F89	;break;	//F89	Сʱ�����й�����
	case	90:	usItemLabel = 	CON_DATA_REQ1_F90	;break;	//F90	Сʱ����A���й�����
	case	91:	usItemLabel = 	CON_DATA_REQ1_F91	;break;	//F91	Сʱ����B���й�����
	case	92:	usItemLabel = 	CON_DATA_REQ1_F92	;break;	//F92	Сʱ����C���й�����
	case	93:	usItemLabel = 	CON_DATA_REQ1_F93	;break;	//F93	Сʱ�����޹�����
	case	94:	usItemLabel = 	CON_DATA_REQ1_F94	;break;	//F94	Сʱ����A���޹�����
	case	95:	usItemLabel = 	CON_DATA_REQ1_F95	;break;	//F95	Сʱ����B���޹�����
	case	96:	usItemLabel = 	CON_DATA_REQ1_F96	;break;	//F96	Сʱ����C���޹�����
	case	97:	usItemLabel = 	CON_DATA_REQ1_F97	;break;	//F97	Сʱ����A���ѹ
	case	98:	usItemLabel = 	CON_DATA_REQ1_F98	;break;	//F98	Сʱ����B���ѹ
	case	99:	usItemLabel = 	CON_DATA_REQ1_F99	;break;	//F99	Сʱ����C���ѹ
	case	100:	usItemLabel = 	CON_DATA_REQ1_F100	;break;	//F100	Сʱ����A�����
	case	101:	usItemLabel = 	CON_DATA_REQ1_F101	;break;	//F101	Сʱ����B�����
	case	102:	usItemLabel = 	CON_DATA_REQ1_F102	;break;	//F102	Сʱ����C�����
	case	103:	usItemLabel = 	CON_DATA_REQ1_F103	;break;	//F103	Сʱ�����������
	case	105:	usItemLabel = 	CON_DATA_REQ1_F105	;break;	//F105	Сʱ���������й��ܵ�����
	case	106:	usItemLabel = 	CON_DATA_REQ1_F106	;break;	//F106	Сʱ���������޹��ܵ�����
	case	107:	usItemLabel = 	CON_DATA_REQ1_F107	;break;	//F107	Сʱ���ᷴ���й��ܵ�����
	case	108:	usItemLabel = 	CON_DATA_REQ1_F108	;break;	//F108	Сʱ���ᷴ���޹��ܵ�����
	case	109:	usItemLabel = 	CON_DATA_REQ1_F109	;break;	//F109	Сʱ���������й��ܵ���ʾֵ
	case	110:	usItemLabel = 	CON_DATA_REQ1_F110	;break;	//F110	Сʱ���������޹��ܵ���ʾֵ
	case	111:	usItemLabel = 	CON_DATA_REQ1_F111	;break;	//F111	Сʱ���ᷴ���й��ܵ���ʾֵ
	case	112:	usItemLabel = 	CON_DATA_REQ1_F112	;break;	//F112	Сʱ���ᷴ���޹��ܵ���ʾֵ
	case	113:	usItemLabel = 	CON_DATA_REQ1_F113	;break;	//F113	Сʱ�����ܹ�������
	case	114:	usItemLabel = 	CON_DATA_REQ1_F114	;break;	//F114	Сʱ����A�๦������
	case	115:	usItemLabel = 	CON_DATA_REQ1_F115	;break;	//F115	Сʱ����B�๦������
	case	116:	usItemLabel = 	CON_DATA_REQ1_F116	;break;	//F116	Сʱ����C�๦������
	case	121:	usItemLabel = 	CON_DATA_REQ1_F121	;break;	//F121	Сʱ����ֱ��ģ����
	case	129:	usItemLabel = 	CON_DATA_REQ1_F129	;break;	
	case	130:	usItemLabel = 	CON_DATA_REQ1_F130	;break;	
	case	131:	usItemLabel = 	CON_DATA_REQ1_F131	;break;	
	case	132:	usItemLabel = 	CON_DATA_REQ1_F132	;break;	
	case	133:	usItemLabel = 	CON_DATA_REQ1_F133	;break;	
	case	134:	usItemLabel = 	CON_DATA_REQ1_F134	;break;	
	case	135:	usItemLabel = 	CON_DATA_REQ1_F135	;break;	
	case	136:	usItemLabel = 	CON_DATA_REQ1_F136	;break;	
	case	137:	usItemLabel = 	CON_DATA_REQ1_F137	;break;	
	case	138:	usItemLabel = 	CON_DATA_REQ1_F138	;break;	
	case	139:	usItemLabel = 	CON_DATA_REQ1_F139	;break;	
	case	140:	usItemLabel = 	CON_DATA_REQ1_F140	;break;	
	case	141:	usItemLabel = 	CON_DATA_REQ1_F141	;break;	
	case	142:	usItemLabel = 	CON_DATA_REQ1_F142	;break;	
	case	143:	usItemLabel = 	CON_DATA_REQ1_F143	;break;	
	case	144:	usItemLabel = 	CON_DATA_REQ1_F144	;break;	
	case	145:	usItemLabel = 	CON_DATA_REQ1_F145	;break;	
	case	146:	usItemLabel = 	CON_DATA_REQ1_F146	;break;	
	case	147:	usItemLabel = 	CON_DATA_REQ1_F147	;break;	
	case	148:	usItemLabel = 	CON_DATA_REQ1_F148	;break;	
	case	149:	usItemLabel = 	CON_DATA_REQ1_F149	;break;	
	case	150:	usItemLabel = 	CON_DATA_REQ1_F150	;break;	
	case	151:	usItemLabel = 	CON_DATA_REQ1_F151	;break;	
	case	152:	usItemLabel = 	CON_DATA_REQ1_F152	;break;	
	}	
*/
    if(ucDT != 0)
    {
        if(protocol_req1_fn_valid[(ucDT-1)/32]&(1<<((ucDT-1)%32)))
        {
            usItemLabel = CON_DATA_REQ1_BEG_CODE + ucDT;	
        }
    }
	return usItemLabel;
}	

unsigned short GetItemLabelbyDT_ReqData_2(unsigned char ucDT)
{
	unsigned short usItemLabel=CON_DI_TYPE_INVALID;
/*
	switch(ucDT)
	{
	case	1:	usItemLabel = 	CON_DATA_REQ2_F1	;break;	//F1	�ն���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
	case	2:	usItemLabel = 	CON_DATA_REQ2_F2	;break;	//F2	�ն��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
	case	3:	usItemLabel = 	CON_DATA_REQ2_F3	;break;	//F3	�ն���������/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	4:	usItemLabel = 	CON_DATA_REQ2_F4	;break;	//F4	�ն��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	5:	usItemLabel = 	CON_DATA_REQ2_F5	;break;	//F5	�ն��������й����������ܡ�����1~M��
	case	6:	usItemLabel = 	CON_DATA_REQ2_F6	;break;	//F6	�ն��������޹����������ܡ�����1~M��
	case	7:	usItemLabel = 	CON_DATA_REQ2_F7	;break;	//F7	�ն��ᷴ���й����������ܡ�����1~M��
	case	8:	usItemLabel = 	CON_DATA_REQ2_F8	;break;	//F8	�ն��ᷴ���޹����������ܡ�����1~M��
	case	9:	usItemLabel = 	CON_DATA_REQ2_F9	;break;	//F9	�����ն���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
	case	10:	usItemLabel = 	CON_DATA_REQ2_F10	;break;	//F10	�����ն��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
	case	11:	usItemLabel = 	CON_DATA_REQ2_F11	;break;	//F11	�����ն���������/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	12:	usItemLabel = 	CON_DATA_REQ2_F12	;break;	//F12	�����ն��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	17:	usItemLabel = 	CON_DATA_REQ2_F17	;break;	//F17	�¶���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
	case	18:	usItemLabel = 	CON_DATA_REQ2_F18	;break;	//F18	�¶��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
	case	19:	usItemLabel = 	CON_DATA_REQ2_F19	;break;	//F19	�¶���������/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	20:	usItemLabel = 	CON_DATA_REQ2_F20	;break;	//F20	�¶��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��
	case	21:	usItemLabel = 	CON_DATA_REQ2_F21	;break;	//F21	�¶��������й����������ܡ�����1~M��
	case	22:	usItemLabel = 	CON_DATA_REQ2_F22	;break;	//F22	�¶��������޹����������ܡ�����1~M��
	case	23:	usItemLabel = 	CON_DATA_REQ2_F23	;break;	//F23	�¶��ᷴ���й����������ܡ�����1~M��
	case	24:	usItemLabel = 	CON_DATA_REQ2_F24	;break;	//F24	�¶��ᷴ���޹����������ܡ�����1~M��
	case	25:	usItemLabel = 	CON_DATA_REQ2_F25	;break;	//F25	�շ�������й����ʼ�����ʱ�䡢�й�����Ϊ��ʱ��
	case	26:	usItemLabel = 	CON_DATA_REQ2_F26	;break;	//F26	���ܼ������������������ʱ��
	case	27:	usItemLabel = 	CON_DATA_REQ2_F27	;break;	//F27	�յ�ѹͳ������
	case	28:	usItemLabel = 	CON_DATA_REQ2_F28	;break;	//F28	�ղ�ƽ���Խ���ۼ�ʱ��
	case	29:	usItemLabel = 	CON_DATA_REQ2_F29	;break;	//F29	�յ���Խ��ͳ��
	case	30:	usItemLabel = 	CON_DATA_REQ2_F30	;break;	//F30	�����ڹ���Խ���ۼ�ʱ��
	case	31:	usItemLabel = 	CON_DATA_REQ2_F31	;break;	//F31	�յ��ܱ��������
	case	32:	usItemLabel = 	CON_DATA_REQ2_F32	;break;	//F32	����
	case	33:	usItemLabel = 	CON_DATA_REQ2_F33	;break;	//F33	�·�������й����ʼ�����ʱ�䡢�����й�����Ϊ��ʱ��
	case	34:	usItemLabel = 	CON_DATA_REQ2_F34	;break;	//F34	�·����й��������������ʱ�䡢���й��������������ʱ��
	case	35:	usItemLabel = 	CON_DATA_REQ2_F35	;break;	//F35	�µ�ѹͳ������
	case	36:	usItemLabel = 	CON_DATA_REQ2_F36	;break;	//F36	�²�ƽ���Խ���ۼ�ʱ��
	case	37:	usItemLabel = 	CON_DATA_REQ2_F37	;break;	//F37	�µ���Խ��ͳ��
	case	38:	usItemLabel = 	CON_DATA_REQ2_F38	;break;	//F38	�����ڹ���Խ���ۼ�ʱ��
//	case	41:	usItemLabel = 	CON_DATA_REQ2_F41	;break;	//F41	������Ͷ���ۼ�ʱ��ʹ���
//	case	42:	usItemLabel = 	CON_DATA_REQ2_F42	;break;	//F42	�ա��µ������ۼƲ������޹�������
	case	43:	usItemLabel = 	CON_DATA_REQ2_F43	;break;	//F43	�չ������������ۼ�ʱ��
	case	44:	usItemLabel = 	CON_DATA_REQ2_F44	;break;	//F44	�¹������������ۼ�ʱ��
	case	49:	usItemLabel = 	CON_DATA_REQ2_F49	;break;	//F49	�ն��չ���ʱ�䡢�ո�λ�ۼƴ���
//	case	50:	usItemLabel = 	CON_DATA_REQ2_F50	;break;	//F50	�ն��տ���ͳ������
	case	51:	usItemLabel = 	CON_DATA_REQ2_F51	;break;	//F51	�ն��¹���ʱ�䡢�¸�λ�ۼƴ���
//	case	52:	usItemLabel = 	CON_DATA_REQ2_F52	;break;	//F52	�ն��¿���ͳ������
	case	53:	usItemLabel = 	CON_DATA_REQ2_F53	;break;	//F53	�ն�����վ��ͨ������
	case	54:	usItemLabel = 	CON_DATA_REQ2_F54	;break;	//F54	�ն�����վ��ͨ������
//	case	57:	usItemLabel = 	CON_DATA_REQ2_F57	;break;	//F57	�ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��
//	case	58:	usItemLabel = 	CON_DATA_REQ2_F58	;break;	//F58	�ܼ������ۼ��й����������ܡ�����1~M��
//	case	59:	usItemLabel = 	CON_DATA_REQ2_F59	;break;	//F59	�ܼ������ۼ��޹����������ܡ�����1~M��
//	case	60:	usItemLabel = 	CON_DATA_REQ2_F60	;break;	//F60	�ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��
//	case	61:	usItemLabel = 	CON_DATA_REQ2_F61	;break;	//F61	�ܼ������ۼ��й����������ܡ�����1~M��
//	case	62:	usItemLabel = 	CON_DATA_REQ2_F62	;break;	//F62	�ܼ������ۼ��޹����������ܡ�����1~M��
//	case	65:	usItemLabel = 	CON_DATA_REQ2_F65	;break;	//F65	�ܼ��鳬���ʶ�ֵ�����ۼ�ʱ�䡢���ۼƵ�����
//	case	66:	usItemLabel = 	CON_DATA_REQ2_F66	;break;	//F66	�ܼ��鳬�µ�������ֵ�����ۼ�ʱ�䡢�ۼƵ�����
//	case	73:	usItemLabel = 	CON_DATA_REQ2_F73	;break;	//F73	�ܼ����й���������
//	case	74:	usItemLabel = 	CON_DATA_REQ2_F74	;break;	//F74	�ܼ����޹���������
//	case	75:	usItemLabel = 	CON_DATA_REQ2_F75	;break;	//F75	�ܼ����й�����������
//	case	76:	usItemLabel = 	CON_DATA_REQ2_F76	;break;	//F76	�ܼ����޹�����������
	case	81:	usItemLabel = 	CON_DATA_REQ2_F81	;break;	//F81	�й���������
	case	82:	usItemLabel = 	CON_DATA_REQ2_F82	;break;	//F82	A���й���������
	case	83:	usItemLabel = 	CON_DATA_REQ2_F83	;break;	//F83	B���й���������
	case	84:	usItemLabel = 	CON_DATA_REQ2_F84	;break;	//F84	C���й���������
	case	85:	usItemLabel = 	CON_DATA_REQ2_F85	;break;	//F85	�޹���������
	case	86:	usItemLabel = 	CON_DATA_REQ2_F86	;break;	//F86	A���޹���������
	case	87:	usItemLabel = 	CON_DATA_REQ2_F87	;break;	//F87	B���޹���������
	case	88:	usItemLabel = 	CON_DATA_REQ2_F88	;break;	//F88	C���޹���������
	case	89:	usItemLabel = 	CON_DATA_REQ2_F89	;break;	//F89	A���ѹ����
	case	90:	usItemLabel = 	CON_DATA_REQ2_F90	;break;	//F90	B���ѹ����
	case	91:	usItemLabel = 	CON_DATA_REQ2_F91	;break;	//F91	C���ѹ����
	case	92:	usItemLabel = 	CON_DATA_REQ2_F92	;break;	//F92	A���������
	case	93:	usItemLabel = 	CON_DATA_REQ2_F93	;break;	//F93	B���������
	case	94:	usItemLabel = 	CON_DATA_REQ2_F94	;break;	//F94	C���������
	case	95:	usItemLabel = 	CON_DATA_REQ2_F95	;break;	//F95	�����������
	case	97:	usItemLabel = 	CON_DATA_REQ2_F97	;break;	//F97	�����й��ܵ�����
	case	98:	usItemLabel = 	CON_DATA_REQ2_F98	;break;	//F98	�����޹��ܵ�����
	case	99:	usItemLabel = 	CON_DATA_REQ2_F99	;break;	//F99	�����й��ܵ�����
	case	100:	usItemLabel = 	CON_DATA_REQ2_F100	;break;	//F100	�����޹��ܵ�����
	case	101:	usItemLabel = 	CON_DATA_REQ2_F101	;break;	//F101	�����й��ܵ���ʾֵ
	case	102:	usItemLabel = 	CON_DATA_REQ2_F102	;break;	//F102	�����޹��ܵ���ʾֵ
	case	103:	usItemLabel = 	CON_DATA_REQ2_F103	;break;	//F103	�����й��ܵ���ʾֵ
	case	104:	usItemLabel = 	CON_DATA_REQ2_F104	;break;	//F104	�����޹��ܵ���ʾֵ
	case	105:	usItemLabel = 	CON_DATA_REQ2_F105	;break;	//F105	�ܹ�������
	case	106:	usItemLabel = 	CON_DATA_REQ2_F106	;break;	//F106	A�๦������
	case	107:	usItemLabel = 	CON_DATA_REQ2_F107	;break;	//F107	B�๦������
	case	108:	usItemLabel = 	CON_DATA_REQ2_F108	;break;	//F108	C�๦������
	case	109:	usItemLabel = 	CON_DATA_REQ2_F109	;break;	
	case	110:	usItemLabel = 	CON_DATA_REQ2_F110	;break;	
//	case	113:	usItemLabel = 	CON_DATA_REQ2_F113	;break;	//F113	A��2~19��г�����������ֵ������ʱ��
//	case	114:	usItemLabel = 	CON_DATA_REQ2_F114	;break;	//F114	B��2~19��г�����������ֵ������ʱ��
//	case	115:	usItemLabel = 	CON_DATA_REQ2_F115	;break;	//F115	C��2~19��г�����������ֵ������ʱ��
//	case	116:	usItemLabel = 	CON_DATA_REQ2_F116	;break;	//F116	A��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
//	case	117:	usItemLabel = 	CON_DATA_REQ2_F117	;break;	//F117	B��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
//	case	118:	usItemLabel = 	CON_DATA_REQ2_F118	;break;	//F118	C��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
//	case	121:	usItemLabel = 	CON_DATA_REQ2_F121	;break;	//F121	A��г��Խ����ͳ������
//	case	122:	usItemLabel = 	CON_DATA_REQ2_F122	;break;	//F122	B��г��Խ����ͳ������
//	case	123:	usItemLabel = 	CON_DATA_REQ2_F123	;break;	//F123	C��г��Խ����ͳ������
//	case	129:	usItemLabel = 	CON_DATA_REQ2_F129	;break;	//F129	ֱ��ģ����Խ�����ۼ�ʱ�䡢���/��Сֵ������ʱ��
//	case	130:	usItemLabel = 	CON_DATA_REQ2_F130	;break;	//F130	ֱ��ģ����Խ�����ۼ�ʱ�䡢���/��Сֵ������ʱ��
//	case	138:	usItemLabel = 	CON_DATA_REQ2_F138	;break;	//F138	ֱ��ģ������������
	case	145:	usItemLabel = 	CON_DATA_REQ2_F145	;break;	//F145	һ�����޹��ܵ���ʾֵ����
	case	146:	usItemLabel = 	CON_DATA_REQ2_F146	;break;	//F146	�������޹��ܵ���ʾֵ����
	case	147:	usItemLabel = 	CON_DATA_REQ2_F147	;break;	//F147	�������޹��ܵ���ʾֵ����
	case	148:	usItemLabel = 	CON_DATA_REQ2_F148	;break;	//F148	�������޹��ܵ���ʾֵ����
	case 	153:usItemLabel = 	CON_DATA_REQ2_F153;break;
	case 	154:usItemLabel = 	CON_DATA_REQ2_F154;break;
	case 	155:usItemLabel = 	CON_DATA_REQ2_F155;break;
	case 	156:usItemLabel = 	CON_DATA_REQ2_F156;break;
	case 	157:usItemLabel = 	CON_DATA_REQ2_F157;break;
	case 	158:usItemLabel = 	CON_DATA_REQ2_F158;break;
	case 	159:usItemLabel = 	CON_DATA_REQ2_F159;break;
	case 	160:usItemLabel = 	CON_DATA_REQ2_F160;break;
	case	161:	usItemLabel = 	CON_DATA_REQ2_F161	;break;	
	case	162:	usItemLabel = 	CON_DATA_REQ2_F162	;break;	
	case	163:	usItemLabel = 	CON_DATA_REQ2_F163	;break;	
	case	164:	usItemLabel = 	CON_DATA_REQ2_F164	;break;	
	case	165:	usItemLabel = 	CON_DATA_REQ2_F165	;break;	
	case	166:	usItemLabel = 	CON_DATA_REQ2_F166	;break;	
	case	167:	usItemLabel = 	CON_DATA_REQ2_F167	;break;	
	case	168:	usItemLabel = 	CON_DATA_REQ2_F168	;break;	
	case	169:	usItemLabel = 	CON_DATA_REQ2_F169	;break;	
	case	170:	usItemLabel = 	CON_DATA_REQ2_F170	;break;	
	case	171:	usItemLabel = 	CON_DATA_REQ2_F171	;break;	
	case	172:	usItemLabel = 	CON_DATA_REQ2_F172	;break;	
	case	173:	usItemLabel = 	CON_DATA_REQ2_F173	;break;	
	case	174:	usItemLabel = 	CON_DATA_REQ2_F174	;break;	
	case	175:	usItemLabel = 	CON_DATA_REQ2_F175	;break;	
	case	176:	usItemLabel = 	CON_DATA_REQ2_F176	;break;	
	case	177:	usItemLabel = 	CON_DATA_REQ2_F177	;break;	
	case	178:	usItemLabel = 	CON_DATA_REQ2_F178	;break;	
	case	179:	usItemLabel = 	CON_DATA_REQ2_F179	;break;	
	case	180:	usItemLabel = 	CON_DATA_REQ2_F180	;break;	
	case	181:	usItemLabel = 	CON_DATA_REQ2_F181	;break;	
	case	182:	usItemLabel = 	CON_DATA_REQ2_F182	;break;	
	case	183:	usItemLabel = 	CON_DATA_REQ2_F183	;break;	
	case	184:	usItemLabel = 	CON_DATA_REQ2_F184	;break;	
	case	185:	usItemLabel = 	CON_DATA_REQ2_F185	;break;	
	case	186:	usItemLabel = 	CON_DATA_REQ2_F186	;break;	
	case	187:	usItemLabel = 	CON_DATA_REQ2_F187	;break;	
	case	188:	usItemLabel = 	CON_DATA_REQ2_F188	;break;	
	case	189:	usItemLabel = 	CON_DATA_REQ2_F189	;break;	
	case	190:	usItemLabel = 	CON_DATA_REQ2_F190	;break;	
	case	191:	usItemLabel = 	CON_DATA_REQ2_F191	;break;	
	case	192:	usItemLabel = 	CON_DATA_REQ2_F192	;break;	
	case	193:	usItemLabel = 	CON_DATA_REQ2_F193	;break;	
	case	194:	usItemLabel = 	CON_DATA_REQ2_F194	;break;	
	case	195:	usItemLabel = 	CON_DATA_REQ2_F195	;break;	
	case	196:	usItemLabel = 	CON_DATA_REQ2_F196	;break;	
	}	
*/
    if(ucDT != 0)
    {
        if(protocol_req2_fn_valid[(ucDT-1)/32]&(1<<((ucDT-1)%32)))
        {
            usItemLabel = CON_DATA_REQ2_BEG_CODE + ucDT;	
        }
    }
	return usItemLabel;
}
unsigned short GetItemLabelbyDT_ReqData_3(unsigned char ucDT)
{
	unsigned short usItemLabel=CON_DI_TYPE_INVALID;
	switch(ucDT)
	{
	case	1:		usItemLabel=	CON_DATA_REQ3_F1	;break;	//��Ҫ�¼�						
	case	2:		usItemLabel=	CON_DATA_REQ3_F2	;break;	//һ���¼�
	}
	return usItemLabel;
}

 unsigned short GetItemLabelbyDT_ExtCmd(unsigned char ucDT)
 {
 	unsigned short usItemLabel=CON_DI_TYPE_INVALID;
	switch(ucDT)
	{
//	case	1:		usItemLabel=	CON_EXTCMD_F1	;break;	//F1 �ڴ��						
//	case	2:		usItemLabel=	CON_EXTCMD_F2	;break;	//F2 �ڴ�д
//	case	3:		usItemLabel=	CON_EXTCMD_F3	;break;	//F3 DATAFLASH��
//	case	4:		usItemLabel=	CON_EXTCMD_F4	;break;	//F4 DATAFLASHд
//	case	5:		usItemLabel=	CON_EXTCMD_F5	;break;	//F5 ���Ź����ԣ��ó��������ѭ��
//	case	6:		usItemLabel=	CON_EXTCMD_F6	;break;	//F6 �ֻ�ģ��ͨѶ���
//	case	7:		usItemLabel=	CON_EXTCMD_F7	;break;	//F7 �ն�״̬���
	case	10:		usItemLabel=	CON_EXTCMD_F10	;break;	//F10 �ն˵�ַ10/16�����л�
//	case	11:		usItemLabel=	CON_EXTCMD_F11	;break;	//F11 ������Ϣ��ȡ
//	case	21:		usItemLabel=	CON_EXTCMD_F21	;break;	//F21 GR�����					
//	case	22:		usItemLabel=	CON_EXTCMD_F22	;break;	//F22 GR��������
//	case	23:		usItemLabel=	CON_EXTCMD_F23	;break;	//F23 GR������Ϣ��
//	case	24:		usItemLabel=	CON_EXTCMD_F24	;break;	//F24 GR������Ϣ����
//	case	31:		usItemLabel=	CON_EXTCMD_F31	;break;	//F31 ������������					
//	case	33:		usItemLabel=	CON_EXTCMD_F33	;break;	//F33 ����׶���������ȫ
//	case	40:		usItemLabel=	CON_EXTCMD_F40	;break;	//F40 �ڱ���������
//	case	41:		usItemLabel=	CON_EXTCMD_F41	;break;	//F41 �ڱ����ݶ�ȡ
//	case	51:		usItemLabel=	CON_EXTCMD_F51	;break;	//F51 �ն˵�ַ					
//	case	52:		usItemLabel=	CON_EXTCMD_F52	;break;	//F52 �ն���Ŷ�
//	case	53:		usItemLabel=	CON_EXTCMD_F53	;break;	//F53 �ն˵�ַ��������д
//	case	54:		usItemLabel=	CON_EXTCMD_F54	;break;	//F54 �ն����д
//	case	71:		usItemLabel=	CON_EXTCMD_F71	;break;	//F71 ����������
//	case	72:		usItemLabel=	CON_EXTCMD_F72	;break;	//F72 �޸�ʱ��
//	case	73:		usItemLabel=	CON_EXTCMD_F73	;break;	//F73 ��ѯͨѶ������
//	case	74:		usItemLabel=	CON_EXTCMD_F74	;break;	//F74 �޸�ͨѶ������
//	case	75:		usItemLabel=	CON_EXTCMD_F75	;break;	//F75 ��ȡҺ��������ʾ����
//	case	76:		usItemLabel=	CON_EXTCMD_F76	;break;	//F76 ��ȡҺ��������ʾ����
//	case	77:		usItemLabel=	CON_EXTCMD_F77	;break;	//F77 ��ȡҺ��������ʾ����
//	case	78:		usItemLabel=	CON_EXTCMD_F78	;break;	//F78 �޸�Һ��������ʾ����
//	case	79:		usItemLabel=	CON_EXTCMD_F79	;break;	//F79 �޸�Һ��������ʾ����
//	case	80:		usItemLabel=	CON_EXTCMD_F80	;break;	//F80 �޸�Һ��������ʾ����
//	case	81:		usItemLabel=	CON_EXTCMD_F81	;break;	//F81 ����SIM����pin
//	case	82:		usItemLabel=	CON_EXTCMD_F82	;break;	//F82 �޸�SIM����pin
//	case	83:		usItemLabel=	CON_EXTCMD_F83	;break;	//F83 ��PUK����SIM����pin
//	case	84:		usItemLabel=	CON_EXTCMD_F84	;break;	//F84 SIM����pin���״̬
//	case	85:		usItemLabel=	CON_EXTCMD_F85	;break;	//F85 GSM/GPRS�л�
//	case	86:		usItemLabel=	CON_EXTCMD_F86	;break;	//F86 �����ʿ��ƻָ�ʱ�䳤��
//	case	87:		usItemLabel=	CON_EXTCMD_F87	;break;	//F87 д���ʿ��ƻָ�ʱ�䳤��
//	case	89:		usItemLabel=	CON_EXTCMD_F89	;break;	//F89 �����ʿ���Խ��ȷ��ʱ��
//	case	90:		usItemLabel=	CON_EXTCMD_F90	;break;	//F90 д���ʿ���Խ��ȷ��ʱ��
//	case	91:		usItemLabel=	CON_EXTCMD_F91	;break;	//F91 ���Զ����������
//	case	92:		usItemLabel=	CON_EXTCMD_F92	;break;	//F92 д�Զ����������
//	case	93:		usItemLabel=	CON_EXTCMD_F93	;break;	//F93 ��ȡ�ն˵�ѹ��λС��������λС��
//	case	94:		usItemLabel=	CON_EXTCMD_F94	;break;	//F94 �����ʼ��
//	case	95:		usItemLabel=	CON_EXTCMD_F95	;break;	//F95 8025ʱ�ӽ���
//	case	96:		usItemLabel=	CON_EXTCMD_F96	;break;	//F96 Һ���Աȶ�����
//	case	97:		usItemLabel=	CON_EXTCMD_F97	;break;	//F97 ���ñ��ͨѸ��������
//	case	98:		usItemLabel=	CON_EXTCMD_F98	;break;	//F98 ��ȡ���ͨѸ��������
//	case	99:		usItemLabel=	CON_EXTCMD_F99	;break;	//F99 ����GPRS�����PDP�û���������
//	case	100:	usItemLabel=	CON_EXTCMD_F100	;break;	//F100 ��ȡGPRS�����PDP�û���������
//	case	101:	usItemLabel=	CON_EXTCMD_F101	;break;	//F101 ����GPRS�������ʱ��
//	case	102:	usItemLabel=	CON_EXTCMD_F102	;break;	//F102 ��ȡGPRS�������ʱ��
//	case	109:	usItemLabel=	CON_EXTCMD_F109	;break;	//F109 �����ն˹���ģʽ
//	case	110:	usItemLabel=	CON_EXTCMD_F110	;break;	//F110 ��ȡ�ն˹���ģʽ
//	case	111:	usItemLabel=	CON_EXTCMD_F111	;break;	//F111 ����CT����
//	case	112:	usItemLabel=	CON_EXTCMD_F112	;break;	//F112 ��ȡCT����
//	case	113:	usItemLabel=	CON_EXTCMD_F113	;break;	//F113 ������ѧϰ��Ч��־
//	case	114:	usItemLabel=	CON_EXTCMD_F114	;break;	//F114 ��ȡ��ѧϰ��Ч��־
//	case	115:	usItemLabel=	CON_EXTCMD_F115	;break;	
//	case	116:	usItemLabel=	CON_EXTCMD_F116	;break;	
//	case	117:	usItemLabel=	CON_EXTCMD_F117	;break;	
//	case	118:	usItemLabel=	CON_EXTCMD_F118	;break;	
//	case	119:	usItemLabel=	CON_EXTCMD_F119	;break;	
//	case	120:	usItemLabel=	CON_EXTCMD_F120	;break;	//F120 ����AES��������
//	case	121:	usItemLabel=	CON_EXTCMD_F121	;break;	//F121 ��ȡAES��������
//	case	122: 	usItemLabel= 	CON_EXTCMD_F122	;break; //F122 Ͷ��ӽ�����߱������
//	case	123: 	usItemLabel= 	CON_EXTCMD_F123	;break; //F123 ��ȡ�¶�
//	case	124:	usItemLabel=	CON_EXTCMD_F124 ;break; //F124 ��ȡУ��EEPROM�Ĳ���
//	case	125:	usItemLabel=	CON_EXTCMD_F125 ;break; //F125 ����У��EEPROM�Ĳ���
//	case 	126:	usItemLabel= 	CON_EXTCMD_F126; break;
	}
 	return usItemLabel;
 }

unsigned short GetItemLabelbyDT_DataTrans(unsigned char ucDT)
{
	unsigned short usItemLabel=CON_DI_TYPE_INVALID;
	switch(ucDT)
	{
	case	1:		usItemLabel=	CON_DATATRANS_F1	;break;	//F1 ����ת��
	case 	9:		usItemLabel = CON_DATATRANS_F9;	break;
	case 	10:		usItemLabel = CON_DATATRANS_F10;	break;
	case 	11:		usItemLabel = CON_DATATRANS_F11;break;
	}
	return usItemLabel;
}

unsigned short GetItemLabelbyDT_FileTrans(unsigned char ucDT)
{
	unsigned short usItemLabel=CON_DI_TYPE_INVALID;
	switch(ucDT)
	{
	case	1:		usItemLabel=	CON_FILETRANS_F1	;break;	//F1 �ļ����䷽ʽ1
	case	100:	usItemLabel=	CON_FILETRANS_F100	;break;	//F100 ������װ����
	}
	return usItemLabel;
}

unsigned char Check_DataType_1(unsigned char *pucBuffer)
{
	if(Check_BCD_Data(pucBuffer[0])==RET_ERROR)				//DT1.byte1	BCD��
		return RET_ERROR;
	if(Check_BCD_Data((unsigned char)(pucBuffer[1]&0x0F))==RET_ERROR)		//DT1.byte2	BCD��
		return RET_ERROR;
	return RET_SUCCESS;
}

unsigned char Check_DataType_4(unsigned char *pucBuffer)
{
	if(Check_BCD_Data(pucBuffer[0])==RET_ERROR)				//DT4.byte1	BCD��
		return RET_ERROR;
	return RET_SUCCESS;
}

unsigned char Check_DataType_5(unsigned char *pucBuffer)
{
	if(Check_BCD_Data(pucBuffer[0])==RET_ERROR)				//DT5.byte1	BCD��
		return RET_ERROR;
	if(Check_BCD_Data(pucBuffer[1])==RET_ERROR)				//DT5.byte2	BCD��
		return RET_ERROR;
	if(Check_BCD_Data(pucBuffer[2])==RET_ERROR)				//DT5.byte3	BCD��
		return RET_ERROR;
	if(Check_BCD_Data((unsigned char)(pucBuffer[3]&0x0F))==RET_ERROR)		//DT5.byte4	BCD��
		return RET_ERROR;
	return RET_SUCCESS;
}

unsigned char Check_DataType_11(unsigned char *pucBuffer)
{
	if(Check_BCD_Data(pucBuffer[0])==RET_ERROR)				//DT11.byte1	BCD��
		return RET_ERROR;
	if(Check_BCD_Data(pucBuffer[1])==RET_ERROR)				//DT11.byte2	BCD��
		return RET_ERROR;
	return RET_SUCCESS;
}

//F1	�ն˰汾��Ϣ
unsigned char Main_Get_ReqData_1_F1(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	/*
	STTERAUXPARA	stTerAuxPara; 
	unsigned char ucVerInfo[7];
	stTerAuxPara = RunPara_GetTerAuxPara();
	memcpy(pucRetBuffer+m,stTerAuxPara.ucManufactor,4);		m+=4;
	memcpy(pucRetBuffer+m,stTerAuxPara.ucDevNo,8);			m+=8;
	RunPara_GetTerVerInfo(ucVerInfo);
	memcpy(pucRetBuffer+m,ucVerInfo,7);						m+=7;
	memcpy(pucRetBuffer+m,stTerAuxPara.ucConfigInfo,11);	m+=11;
	*/
	(*pusPos)=m;
	return ucRet;
}

//F2	�ն�����ʱ��
unsigned char Main_Get_ReqData_1_F2(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	STDATETIME stTempTime=GetCurrentTime();
	m=SetMainComm_Buf_DateTime_HMS(pucRetBuffer,m,stTempTime);
	m=SetMainComm_Buf_DateTime_YMDW(pucRetBuffer,m,stTempTime);
	(*pusPos)=m;
	return ucRet;
}

//F3	�ն˲���״̬
unsigned char Main_Get_ReqData_1_F3(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);

	pucRetBuffer[m++] = 0xFd;		//	F8	F7	F6	F5	F4	F3	F	F1
	pucRetBuffer[m++] = 0XFF;		//	F16	F15	F14	F13	F12	F11	F10	F9	
	pucRetBuffer[m++] = 0X7F;		//	F	F23	F22	F21	F20	F19	F18	F17
	pucRetBuffer[m++] = 0X1F;		//	F	F	F	F29	F28	F27	F26	F25
	pucRetBuffer[m++] = 0X7A;		//	F	F39	F38	F37	F36	F	F34	F33
	pucRetBuffer[m++] = 0XFF;		//	F48	F47	F46	F45	F44	F43	F42	F41
	pucRetBuffer[m++] = 1;			//	F	F	F	F	F	F	F	F49
	pucRetBuffer[m++] = 0X1F;		//	F	F	F	F61	F60	F59	F58	F57
	pucRetBuffer[m++] = 0X0F;		//	F	F	F	F	F68	F67	F66	F65
	
	memset(pucRetBuffer+m,0,22);
	(*pusPos)+=31;
	return ucRet;
}

//F4	�ն�ͨ��״̬
unsigned char Main_Get_ReqData_1_F4(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucTmp=0;
	unsigned short m=(*pusPos);
	STTERMISCPARA	stTerMiscPara;
	stTerMiscPara=RunPara_GetTerMiscPara();
	if(stTerMiscPara.ucTalkOn==CON_AVALIBLE_FLAG)
		ucTmp |= 0x04;
	else
		ucTmp |= 0x08;
	if(stTerMiscPara.ucUpActiveOn==CON_AVALIBLE_FLAG)
		ucTmp |= 0x01;
	else
		ucTmp |= 0x02;
	pucRetBuffer[m++]=ucTmp;
	(*pusPos)=m;
	return ucRet;
}

//F5	�ն˿�������״̬
unsigned char Main_Get_ReqData_1_F5(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_ERROR;//RET_SUCCESS;
//	unsigned char ucTmp;
//	unsigned short m=(*pusPos);
//	STLOADCTRLWORDPARA	stLoadCtrlWordPara;
//	STTERMISCPARA stTerMiscPara;
//	unsigned char ucPowerCtrlSumGNo,ucEnergyCtrlSumGNo;
//
//	ucPowerCtrlSumGNo=Load_GetPowerCtrl_SumGNo();
//	if(ucPowerCtrlSumGNo>=CON_SUMGROUP_NUM)
//	{
//		ucPowerCtrlSumGNo=0;
//		Load_SetPowerCtrl_SumGNo(ucPowerCtrlSumGNo);
//	}
//	ucEnergyCtrlSumGNo=Load_GetEnergyCtrl_SumGNo();
//	if(ucEnergyCtrlSumGNo>=CON_SUMGROUP_NUM)
//	{
//		ucEnergyCtrlSumGNo=0;
//		Load_SetEnergyCtrl_SumGNo(ucEnergyCtrlSumGNo);
//	}
//	//���硢�޳��ʹ߷Ѹ澯Ͷ��״̬
//	ucTmp=0;
//	if(RunPara_GetKeepPower()==CON_AVALIBLE_FLAG)
//		ucTmp |= 0x01;
//	stTerMiscPara=RunPara_GetTerMiscPara();
//	if(stTerMiscPara.ucRemoveOn==CON_AVALIBLE_FLAG)
//		ucTmp |= 0x02;
//	stLoadCtrlWordPara=RunPara_GetLoadCtrlWordPara();
//	if(stLoadCtrlWordPara.ucUrgeFeeCtrl==CON_AVALIBLE_FLAG)
//		ucTmp |= 0x04;
//	pucRetBuffer[m++]=ucTmp;
//	//�ܼ�����Ч��־λ
//	ucTmp = (unsigned char)(1<<ucPowerCtrlSumGNo);
//	ucTmp |= (unsigned char)(1<<ucEnergyCtrlSumGNo);
//	pucRetBuffer[m++]=ucTmp;
//
//	if(ucPowerCtrlSumGNo!=ucEnergyCtrlSumGNo)
//	{
//		if(ucPowerCtrlSumGNo<ucEnergyCtrlSumGNo)
//		{
//			Main_Get_ReqData_1_F5_SumGroupPer(pucRetBuffer,&m,ucPowerCtrlSumGNo);
//			Main_Get_ReqData_1_F5_SumGroupPer(pucRetBuffer,&m,ucEnergyCtrlSumGNo);
//		}else
//		{
//			Main_Get_ReqData_1_F5_SumGroupPer(pucRetBuffer,&m,ucEnergyCtrlSumGNo);
//			Main_Get_ReqData_1_F5_SumGroupPer(pucRetBuffer,&m,ucPowerCtrlSumGNo);
//		}
//	}else
//		Main_Get_ReqData_1_F5_SumGroupPer(pucRetBuffer,&m,ucPowerCtrlSumGNo);
//
//	(*pusPos)=m;
	return ucRet;
}

//unsigned char Main_Get_ReqData_1_F5_SumGroupPer(unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned char ucSumGroupNo)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucTmp,i;
//	unsigned short m=(*pusPos);
//
//	STPOWERCTRLPROJ	stPowerCtrlProj;
//	STPOWERCTRLTIME stPowerCtrlTime;
//	STENERGYCTRL stEnergyCtrl;
//	STCTRLTURN	stCtrlTurn;
//
//	//���ض�ֵ�����ż�����ʱ����Ч��־
//	stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER1,ucSumGroupNo);
//	stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_TIMEPER1,ucSumGroupNo);
//	if(stPowerCtrlProj.ucProjAvailbleFlag==CON_AVALIBLE_FLAG)
//	{
//		pucRetBuffer[m++]=0x01;
//		pucRetBuffer[m++]=Get_PowerCtrlPer_Info(&stPowerCtrlTime);
//	}else
//	{
//		stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER2,ucSumGroupNo);
//		stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_TIMEPER2,ucSumGroupNo);
//		if(stPowerCtrlProj.ucProjAvailbleFlag==CON_AVALIBLE_FLAG)
//		{
//			pucRetBuffer[m++]=0x02;
//			pucRetBuffer[m++]=Get_PowerCtrlPer_Info(&stPowerCtrlTime);
//		}else
//		{
//			stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER3,ucSumGroupNo);
//			stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_TIMEPER3,ucSumGroupNo);
//			if(stPowerCtrlProj.ucProjAvailbleFlag==CON_AVALIBLE_FLAG)
//			{
//				pucRetBuffer[m++]=0x03;
//				pucRetBuffer[m++]=Get_PowerCtrlPer_Info(&stPowerCtrlTime);
//			}else
//			{
//				pucRetBuffer[m++]=0x00;
//				pucRetBuffer[m++]=0x00;
//			}
//		}
//	}
//	//����״̬
//	ucTmp=0;
//	stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_TIMEPER1,ucSumGroupNo);
//	if(stPowerCtrlProj.ucProjAvailbleFlag==CON_AVALIBLE_FLAG)
//		ucTmp |= 0x01;
//	stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_TIMEPER2,ucSumGroupNo);
//	if(stPowerCtrlProj.ucProjAvailbleFlag==CON_AVALIBLE_FLAG)
//		ucTmp |= 0x01;
//	stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_TIMEPER3,ucSumGroupNo);
//	if(stPowerCtrlProj.ucProjAvailbleFlag==CON_AVALIBLE_FLAG)
//		ucTmp |= 0x01;
//	stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_RESTCTRL,ucSumGroupNo);
//	if(stPowerCtrlProj.ucProjAvailbleFlag==CON_AVALIBLE_FLAG)
//		ucTmp |= 0x02;
//	stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_SALESTOP,ucSumGroupNo);
//	if(stPowerCtrlProj.ucProjAvailbleFlag==CON_AVALIBLE_FLAG)
//		ucTmp |= 0x04;
//	stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_POWLOWER,ucSumGroupNo);
//	if(stPowerCtrlProj.ucProjAvailbleFlag==CON_AVALIBLE_FLAG)
//		ucTmp |= 0x08;
//	pucRetBuffer[m++]=ucTmp;
//	//���״̬
//	ucTmp=0;
//	stEnergyCtrl=RunPara_GetEnergyCtrl(ucSumGroupNo);
//	if(stEnergyCtrl.ucEnergyCtrlType==CON_ENERGYCTRL_MON)
//		ucTmp |= 0x01;
//	if(stEnergyCtrl.ucEnergyCtrlType==CON_ENERGYCTRL_BUY)
//		ucTmp |= 0x02;
//	pucRetBuffer[m++]=ucTmp;
//	//�����ִ�״̬
//	ucTmp=0;
//	stCtrlTurn=RunPara_GetPowerCtrlTurn(ucSumGroupNo);
//	for(i=0;i<CON_PARA_SWITCHOUT_NUM;i++)
//	{
//		if(stCtrlTurn.ucAvailbleFlag[i]==CON_AVALIBLE_FLAG)
//			ucTmp |= (1<<i);
//	}
//	pucRetBuffer[m++]=ucTmp;
//	//����ִ�״̬
//	ucTmp=0;
//	stCtrlTurn=RunPara_GetEnergyCtrlTurn(ucSumGroupNo);
//	for(i=0;i<CON_SWITCHOUT_NUM;i++)
//	{
//		if(stCtrlTurn.ucAvailbleFlag[i]==CON_AVALIBLE_FLAG)
//			ucTmp |= (1<<i);
//	}
//	pucRetBuffer[m++]=ucTmp;
//
//	(*pusPos)=m;
//	return ucRet;
//}

//unsigned char Get_PowerCtrlPer_Info(STPOWERCTRLTIME *pstPowerCtrlTime)
//{
//	unsigned char ucRet=0;
//	unsigned char ucTmp,i;
//	unsigned char ucFlag=0;
//	for(i=0;i<CONSWITCHPERTIMENUM;i++)
//	{
//		if(pstPowerCtrlTime->stPowerCtrlPer[i].ucAvailbleFlag==CON_AVALIBLE_FLAG && pstPowerCtrlTime->stPowerCtrlPer[i].ucCtrlFlag==CON_AVALIBLE_FLAG)
//		{
//			ucTmp=(1<<ucFlag);
//			if(pstPowerCtrlTime->stPowerCtrlPer[i].ucOnFlag==CON_AVALIBLE_FLAG)				//050430
//				ucRet |= ucTmp;
//			ucFlag++;
//			if(ucFlag==8)
//				break;
//		}
//	}
//	return ucRet;
//}

//F6	�ն˵�ǰ����״̬
unsigned char Main_Get_ReqData_1_F6(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
//	unsigned short m=(*pusPos);
//	STSWITCHSTAT stSwitchStat;
//	STLOADCTRLWORDPARA	stLoadCtrlWordPara;
//	unsigned char ucVal,i;
//	unsigned char ucPowerCtrlSumGNo,ucEnergyCtrlSumGNo;
//
//	ucPowerCtrlSumGNo=Load_GetPowerCtrl_SumGNo();
//	if(ucPowerCtrlSumGNo>=CON_SUMGROUP_NUM)
//	{
//		ucPowerCtrlSumGNo=0;
//		Load_SetPowerCtrl_SumGNo(ucPowerCtrlSumGNo);
//	}
//	ucEnergyCtrlSumGNo=Load_GetEnergyCtrl_SumGNo();
//	if(ucEnergyCtrlSumGNo>=CON_SUMGROUP_NUM)
//	{
//		ucEnergyCtrlSumGNo=0;
//		Load_SetEnergyCtrl_SumGNo(ucEnergyCtrlSumGNo);
//	}
//	stSwitchStat=GetSwitchShouldStat();
//	ucVal=0;
//	for(i=0;i<CON_SWITCHOUT_NUM;i++)
//	{
//		if(stSwitchStat.ucSwitchStat[i]==CONSWITCHOPEN && stSwitchStat.ucSwitchStatSour[i]==CONSWITCH_SOUR_REMOTE)
//			ucVal |= (1<<i);								//ң����բ���״̬
//	}
//	pucRetBuffer[m++]=ucVal;
//
//	stLoadCtrlWordPara=RunPara_GetLoadCtrlWordPara();
//	if(stLoadCtrlWordPara.ucUrgeFeeCtrl==CON_AVALIBLE_FLAG)	//��ǰ�߷Ѹ澯״̬
//		pucRetBuffer[m++] = 0x01;				
//	else
//		pucRetBuffer[m++] = 0x00;
//
//	ucVal = (unsigned char)(1<<ucPowerCtrlSumGNo);			//�ܼ�����Ч��־	
//	ucVal |= (unsigned char)(1<<ucEnergyCtrlSumGNo);
//	pucRetBuffer[m++]=ucVal;
//	if(ucPowerCtrlSumGNo!=ucEnergyCtrlSumGNo)
//	{
//		if(ucPowerCtrlSumGNo<ucEnergyCtrlSumGNo)
//		{
//			Main_Get_ReqData_1_F6_SumGroupPer(pucRetBuffer,&m,ucPowerCtrlSumGNo,0xAA);			//AA����
//			Main_Get_ReqData_1_F6_SumGroupPer(pucRetBuffer,&m,ucEnergyCtrlSumGNo,0x55);			//55���
//		}else
//		{
//			Main_Get_ReqData_1_F6_SumGroupPer(pucRetBuffer,&m,ucEnergyCtrlSumGNo,0x55);			//55���
//			Main_Get_ReqData_1_F6_SumGroupPer(pucRetBuffer,&m,ucPowerCtrlSumGNo,0xAA);			//AA����
//		}
//	}else
//		Main_Get_ReqData_1_F6_SumGroupPer(pucRetBuffer,&m,ucPowerCtrlSumGNo,0xFF);
//
//	(*pusPos)=m;
	return ucRet;
}

//unsigned char Main_Get_ReqData_1_F6_SumGroupPer(unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned char ucSumGroupNo,unsigned char ucSumGroupFlag)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned short m=(*pusPos);
//	STSWITCHSTAT stSwitchStat;
//	STLOADFIXVAL  stLoadFixVal,stSafeFixVal;
//	unsigned char ucPowCurveNo=0,ucEndHour=0,ucEndMinute=0;
//	STPOWERCTRLTIME stPowerCtrlTime;
//	STENERGYCTRL  stEnergyCtrl;
//	float fVal;
//	unsigned short usTmp;
//	unsigned char ucVal,i;
//
//	stSafeFixVal=RunPara_GetCtrlSafeFixVal();				//��ǰ���ض�ֵ
//	stLoadFixVal=RunPara_GetPowFixVal(&ucPowCurveNo,&ucEndHour,&ucEndMinute,ucSumGroupNo);
//	if(stLoadFixVal.dFixVal==CONINVALIDVAL)
//	{
//		pucRetBuffer[m++] = 0xEE;
//		pucRetBuffer[m++] = 0xEE;
//	}else
//	{
//		if(stSafeFixVal.dFixVal!=CONINVALIDVAL && (stLoadFixVal.dFixVal*stLoadFixVal.usFloatCoef/100)<stSafeFixVal.dFixVal)
//			fVal=(float)stSafeFixVal.dFixVal;
//		else
//			fVal=(float)(stLoadFixVal.dFixVal*stLoadFixVal.usFloatCoef/100);
//		usTmp=Float_To_DT2(fVal);
//		pucRetBuffer[m++]=(unsigned char)usTmp;
//		pucRetBuffer[m++]=(unsigned char)(usTmp>>8);
//	}
//	//�¸��ظ���ϵ��
//
//
//	if(ucSumGroupFlag!=CON_SUMGFLAG_ENERGYCTRL)
//	{
//		stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_POWLOWER,ucSumGroupNo);
//		usTmp=stPowerCtrlTime.stPowerCtrlPer[0].stLoadFixVal.usFloatCoef;
//		if(usTmp<100)
//		{
//			ucVal=(unsigned char)(100-usTmp);
//			ucVal=(unsigned char)HEX_TO_BCD(ucVal,1);
//			ucVal |= 0x80;
//		}else
//		{
//			ucVal=(unsigned char)(usTmp-100);
//			ucVal=(unsigned char)HEX_TO_BCD(ucVal,1);
//		}
//	}else
//		ucVal=0xEE;
//	pucRetBuffer[m++]=ucVal;
//
//	stSwitchStat=GetSwitchShouldStat();
//	//������բ���״̬
//	ucVal=0;
//	if(ucSumGroupFlag!=CON_SUMGFLAG_ENERGYCTRL)
//	{
//		for(i=0;i<CON_SWITCHOUT_NUM;i++)
//		{
//			if(stSwitchStat.ucSwitchStat[i]==CONSWITCHOPEN && stSwitchStat.ucSwitchStatSour[i]>=CONSWITCH_SOUR_POWER && stSwitchStat.ucSwitchStatSour[i]<(CONSWITCH_SOUR_POWER+CONSWITCHCURVENUM))
//				ucVal |= (1<<i);
//		}
//	}else
//		ucVal=0;
//	pucRetBuffer[m++]=ucVal;
//
//	//�µ����բ���״̬
//	ucVal=0;
//	if(ucSumGroupFlag!=CON_SUMGFLAG_POWERCTRL)
//	{
//		for(i=0;i<CON_SWITCHOUT_NUM;i++)
//		{
//			if(stSwitchStat.ucSwitchStat[i]==CONSWITCHOPEN && stSwitchStat.ucSwitchStatSour[i]==CONSWITCH_SOUR_ENERGY)
//				ucVal |= (1<<i);
//		}
//		stEnergyCtrl=RunPara_GetEnergyCtrl(ucSumGroupNo);
//		if(stEnergyCtrl.ucEnergyCtrlType==CON_ENERGYCTRL_MON)
//		{
//			pucRetBuffer[m++]=ucVal;
//			pucRetBuffer[m++]=0;
//		}else
//		{
//			pucRetBuffer[m++]=0;
//			pucRetBuffer[m++]=ucVal;
//	}
//	}else
//	{
//		pucRetBuffer[m++]=0;
//		pucRetBuffer[m++]=0;
//	}
//	//������բ�澯״̬
//	pucRetBuffer[m]=0x00;
//	if(ucSumGroupFlag!=CON_SUMGFLAG_ENERGYCTRL)
//	{
//		for(i=0;i<CON_SWITCHOUT_NUM;i++)
//		{
//			if(stSwitchStat.ucSwitchStat[i]==CONSWITCHREADY && stSwitchStat.ucSwitchStatSour[i]>=CONSWITCH_SOUR_POWER && stSwitchStat.ucSwitchStatSour[i]<(CONSWITCH_SOUR_POWER+CONSWITCHCURVENUM))
//			{
//				if(stSwitchStat.ucSwitchStatSour[i]==(CONSWITCH_SOUR_POWER+CONSWITCHCURVE_POWLOWER))
//					pucRetBuffer[m]=0x08;
//				else if(stSwitchStat.ucSwitchStatSour[i]==(CONSWITCH_SOUR_POWER+CONSWITCHCURVE_SALESTOP))
//					pucRetBuffer[m]=0x04;
//				else if(stSwitchStat.ucSwitchStatSour[i]==(CONSWITCH_SOUR_POWER+CONSWITCHCURVE_RESTCTRL))
//					pucRetBuffer[m]=0x02;
//				else
//					pucRetBuffer[m]=0x01;
//
//				break;
//			}
//		}
//	}else
//		pucRetBuffer[m]=0x00;
//	m++;
//
//	//�����բ�澯״̬
//	pucRetBuffer[m]=0x00;
//	ucVal=0xAA;
//	for(i=0;i<CON_SWITCHOUT_NUM;i++)
//	{
//		if(stSwitchStat.ucSwitchStat[i]==CONSWITCHREADY && stSwitchStat.ucSwitchStatSour[i]==CONSWITCH_SOUR_ENERGY)
//		{
//			ucVal=0x55;
//			break;
//		}
//	}
//	if(ucSumGroupFlag!=CON_SUMGFLAG_POWERCTRL)
//	{
//		if(ucVal==0x55)
//		{
//			if(stEnergyCtrl.ucEnergyCtrlType==CON_ENERGYCTRL_MON)
//				pucRetBuffer[m]=0x01;
//			else
//				pucRetBuffer[m]=0x02;
//		}
//	}else
//		pucRetBuffer[m]=0x00;
//	m++;
//
//	(*pusPos)=m;
//	return ucRet;
//}

//F7	�ն��¼���������ǰֵ
unsigned char Main_Get_ReqData_1_F7(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	pucRetBuffer[m++]=g_ucEventCount_Important;
	pucRetBuffer[m++]=g_ucEventCount_Normal;
	(*pusPos)=m;
	return ucRet;
}

//F8	�ն��¼���־״̬
unsigned char Main_Get_ReqData_1_F8(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
//	unsigned char i=0;
	unsigned short m=(*pusPos);
//	STALRCATEGORY	stAlrHappenWord;
//	stAlrHappenWord = Alarm_Get_Happen_word();//azh �Ѿ���ȫ�ֱ���

	memset(pucRetBuffer+m,0x00,8);

	SetAlrCategoryComm(stAlrHappenWord.ucTerDataInit	,	&pucRetBuffer[m],0,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	SetAlrCategoryComm(stAlrHappenWord.ucTerParaLost	,	&pucRetBuffer[m],1,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	SetAlrCategoryComm(stAlrHappenWord.ucTerParaChange	,&pucRetBuffer[m],2,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);	
	SetAlrCategoryComm(stAlrHappenWord.ucStatChange		,&pucRetBuffer[m],3,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	SetAlrCategoryComm(stAlrHappenWord.ucRemoteCtrl		,&pucRetBuffer[m],4,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	SetAlrCategoryComm(stAlrHappenWord.ucPowerCtrl		,&pucRetBuffer[m],5,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	SetAlrCategoryComm(stAlrHappenWord.ucEnergyCtrl		,&pucRetBuffer[m],6,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);	
	SetAlrCategoryComm(stAlrHappenWord.ucMeterParaChange,&pucRetBuffer[m],7,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	
	SetAlrCategoryComm(stAlrHappenWord.ucTAErr			,&pucRetBuffer[m+1],0,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	SetAlrCategoryComm(stAlrHappenWord.ucTVErr			,&pucRetBuffer[m+1],1,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	SetAlrCategoryComm(stAlrHappenWord.ucPhaseErr		,&pucRetBuffer[m+1],2,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	SetAlrCategoryComm(stAlrHappenWord.ucMeterTimeErr	,&pucRetBuffer[m+1],3,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	SetAlrCategoryComm(stAlrHappenWord.ucMeterErrInfo	,	&pucRetBuffer[m+1],4,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	SetAlrCategoryComm(stAlrHappenWord.ucTerPowOff		,&pucRetBuffer[m+1],5,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	SetAlrCategoryComm(stAlrHappenWord.ucHarmWaveOver	,&pucRetBuffer[m+1],6,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	SetAlrCategoryComm(stAlrHappenWord.ucDirSimuValOver	,&pucRetBuffer[m+1],7,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	
	SetAlrCategoryComm(stAlrHappenWord.ucUnbalanceOver	,&pucRetBuffer[m+2],0,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);	
	SetAlrCategoryComm(stAlrHappenWord.ucCapLockSelf	,	&pucRetBuffer[m+2],1,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	SetAlrCategoryComm(stAlrHappenWord.ucBuyPowPara		,&pucRetBuffer[m+2],2,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	SetAlrCategoryComm(stAlrHappenWord.ucPassErr		,	&pucRetBuffer[m+2],3,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	SetAlrCategoryComm(stAlrHappenWord.ucTerErr			,&pucRetBuffer[m+2],4,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	SetAlrCategoryComm(stAlrHappenWord.ucEnerAlr		,	&pucRetBuffer[m+2],6,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	SetAlrCategoryComm(stAlrHappenWord.ucVolOverUp		,&pucRetBuffer[m+2],7,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	
	SetAlrCategoryComm(stAlrHappenWord.ucCurOverUp		,&pucRetBuffer[m+3],0,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	SetAlrCategoryComm(stAlrHappenWord.ucSPowOverUp		,&pucRetBuffer[m+3],1,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	SetAlrCategoryComm(stAlrHappenWord.ucMeterDown		,&pucRetBuffer[m+3],2,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	SetAlrCategoryComm(stAlrHappenWord.ucPowDiffOver	,	&pucRetBuffer[m+3],3,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	SetAlrCategoryComm(stAlrHappenWord.ucMeterFly		,&pucRetBuffer[m+3],4,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	SetAlrCategoryComm(stAlrHappenWord.ucMeterStop		,&pucRetBuffer[m+3],5,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	SetAlrCategoryComm(stAlrHappenWord.ucMeterCopy		,&pucRetBuffer[m+3],6,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	SetAlrCategoryComm(stAlrHappenWord.ucFluxLimit		,&pucRetBuffer[m+3],7,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
	
//	for(i=0;i<CON_SUMGROUP_NUM;i++)
//	{
//		SetAlrCategoryComm((stAlrHappenWord.ucSumInstantPowOver[i]),&pucRetBuffer[m+2],5,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
//		SetAlrCategoryComm((stAlrHappenWord.ucSumInstantVarOver[i]),&pucRetBuffer[m+2],5,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
//		SetAlrCategoryComm((stAlrHappenWord.ucSumPowValOver[i])	  ,&pucRetBuffer[m+2],6,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
//		SetAlrCategoryComm((stAlrHappenWord.ucSumVarValOver[i])	  ,&pucRetBuffer[m+2],6,CON_ALR_HAPPEN_NOT,CON_ALR_HAPPEN);
//	}
	
	m+=4;
	pucRetBuffer[m++] = 0;
	pucRetBuffer[m++] = 0;
	pucRetBuffer[m++] = 0;
	pucRetBuffer[m++] = 0;

//	memset(&stAlrHappenWord,CON_ALR_HAPPEN_NOT,sizeof(STALRCATEGORY));
//	Alarm_Set_Happen_word(&stAlrHappenWord);

	(*pusPos)=m;
	return ucRet;
}

//F9	�ն�״̬������λ��־
unsigned char Main_Get_ReqData_1_F9(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucTmp=0;
//	unsigned short m=(*pusPos);
//	if(Get_CurrentSwitchStat(0)==CON_SWITCH_STAT_CLOSE)
//		ucTmp |= 0x01;
//	if(Get_CurrentSwitchStat(1)==CON_SWITCH_STAT_CLOSE)
//		ucTmp |= 0x02;
//	if(Get_CurrentSwitchStat(2)==CON_SWITCH_STAT_CLOSE)
//		ucTmp |= 0x04;
//	if(Get_CurrentSwitchStat(3)==CON_SWITCH_STAT_CLOSE)
//		ucTmp |= 0x08;
///*
//	if(RunPara_GetSwitchPara_PulseIsTypeSwitch(0)==CON_STATUS_YES)
//	{
//		if(Get_CurrentSwitchStat(4)==CON_SWITCH_STAT_CLOSE)
//			ucTmp |= 0x10;
//	}
//	if(RunPara_GetSwitchPara_PulseIsTypeSwitch(1)==CON_STATUS_YES)
//	{
//		if(Get_CurrentSwitchStat(5)==CON_SWITCH_STAT_CLOSE)
//			ucTmp |= 0x20;
//	}
//*/
////	if(Get_CurrentSwitchStat(6)==CON_SWITCH_STAT_CLOSE)
////		ucTmp |= 0x40;
//	if(Get_CurrentDoorStat()==CON_SWITCH_STAT_CLOSE)
//		ucTmp |= 0x80;
//	pucRetBuffer[m++]=ucTmp;
//	ucTmp=Get_Switch_Change_Info();
//	pucRetBuffer[m++]=ucTmp;
//	Init_Switch_Change_Info();
//	(*pusPos)=m;
	return ucRet;
}

//F10	�ն�����վ���ա���ͨ������
unsigned char Main_Get_ReqData_1_F10(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	unsigned long ulFluxBytes=0;
	STDATETIME stTempTime;
	STHISRUNDATA *pstHisRunData;

	stTempTime=GetCurrentTime();
	stTempTime.ucSecond=59;
	stTempTime.ucMinute=59;
	stTempTime.ucHour=23;

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
//	pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
    pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���
	ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
	if(ucRet==RET_SUCCESS)
	{
		ulFluxBytes = pstHisRunData->ulHisFluxBytes;
	}
	memcpy(pucRetBuffer+m,&ulFluxBytes,4);
	m+=4;
	
	stTempTime.ucDay=28;
	ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
	if(ucRet==RET_SUCCESS)
	{
		ulFluxBytes += pstHisRunData->ulHisFluxBytes;
	}
	memcpy(pucRetBuffer+m,&ulFluxBytes,4);
	m+=4;

	ucRet=RET_SUCCESS;

    free_mem_block(mem_ptr);//

	(*pusPos)=m;
	return ucRet;
}
//azh 170921
//F12	���������·���ؽ���״̬������λ��־
unsigned char Main_Get_ReqData_1_F12(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);

	memset(pucRetBuffer+m,0,2);//azh ��
	m+=2;
	ucRet=RET_SUCCESS;
	(*pusPos)=m;
	return ucRet;
}
//F13	�������ĵ����Ϣ
unsigned char Main_Get_ReqData_1_F13(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);

	memset(pucRetBuffer+m,0,4);//azh ��
	m+=4;
	ucRet=RET_SUCCESS;
	(*pusPos)=m;
	return ucRet;
}
//F14	�ļ�����δ�յ����ݶ�
unsigned char Main_Get_ReqData_1_F14(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);

	memset(pucRetBuffer+m,0,130);//azh ��
	m+=130;
	ucRet=RET_SUCCESS;
	(*pusPos)=m;
	return ucRet;
}
////F11	��ǰ�ź�ǿ��
//unsigned char Main_Get_ReqData_1_F11(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
/*	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	unsigned char ucSignIntension;
	STTERMISCPARA stTerMiscPara=RunPara_GetTerMiscPara();	
	ucSignIntension=stTerMiscPara.stRCommRunInfo.ucSignIntension;
	memcpy(pucRetBuffer+m,&ucSignIntension,1);
	m+=1;
	ucRet=RET_SUCCESS;
	(*pusPos)=m;
	return ucRet;
	*/
//	return RET_ERROR;
//}
//
////F16	�ն��ز�/���߳���ʧ�ܱ�־��
//unsigned char Main_Get_ReqData_1_F16(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_ERROR;
	/*
	unsigned short m=(*pusPos);
	unsigned char ucMPNo,ucMPType,i,ucMeterNum;
	ucMeterNum = 0;
	for(i=0;i<CON_METER_NUM;i++)
	{
		ucMPNo	 = RunPara_GetMeter_MPNo(i);
		ucMPType = RunPara_GetMPType(ucMPNo);
		if(ucMPType == CON_MPTYPE_METER)
			ucMeterNum ++;
	}
	pucRetBuffer[m++]=ucMeterNum;
	for(i=0;i<ucMeterNum;i++)
	{
		pucRetBuffer[m++] = 0;	//��"0"����ʾ������ò������ǲ��÷��ز�ͨ�ŷ�ʽ����ĵ��
	}
	(*pusPos)=m;
	*/
//	return ucRet;
//}
//
////F17	��ǰ�ܼ��й�����
//unsigned char Main_Get_ReqData_1_F17(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_ERROR;
//	unsigned short m=(*pusPos);
//	unsigned char ucGroupNo;
//	unsigned short usDT2=0;
//	float fVal=0;
//	STHISSUMVALGROUP stHisSumValGroup;
//	ucGroupNo=ucDAp;
//	if(ucGroupNo<CON_SUMGROUP_NUM)
//	{
//		stHisSumValGroup = Get_History_InstantPowSumVal(0);
//		fVal=stHisSumValGroup.stSumValGroup.fSumVal[ucGroupNo];
//		if(fVal!=CONINVALIDVAL)
//		{
//			fVal/=10000;
//			usDT2=Float_To_DT2(fVal);
//			pucRetBuffer[m++]=(unsigned char)usDT2;
//			pucRetBuffer[m++]=(unsigned char)(usDT2>>8);
//		}else
//		{
//			pucRetBuffer[m++]=0xEE;
//			pucRetBuffer[m++]=0xEE;
//		}
//		ucRet=RET_SUCCESS;
//	}
//	(*pusPos)=m;
//	return ucRet;
//}
//
////F18	��ǰ�ܼ��޹�����
//unsigned char Main_Get_ReqData_1_F18(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_ERROR;
//	unsigned short m=(*pusPos);
//	unsigned char ucGroupNo;
//	unsigned short usDT2=0;
//	float fVal=0;
//	STHISSUMVALGROUP	stHisSumValGroup;
//	ucGroupNo=ucDAp;
//	if(ucGroupNo<CON_SUMGROUP_NUM)
//	{
//		stHisSumValGroup = Get_History_InstantVarSumVal(0);
//		fVal=stHisSumValGroup.stSumValGroup.fSumVal[ucGroupNo];
//		if(fVal!=CONINVALIDVAL)
//		{
//			fVal/=100;
//			usDT2=Float_To_DT2(fVal);
//			pucRetBuffer[m++]=(unsigned char)usDT2;
//			pucRetBuffer[m++]=(unsigned char)(usDT2>>8);
//		}else
//		{
//			pucRetBuffer[m++]=0xEE;
//			pucRetBuffer[m++]=0xEE;
//		}
//		ucRet=RET_SUCCESS;
//	}
//	(*pusPos)=m;
//	return ucRet;
//}
//
////F19	�����ܼ��й����������ܡ�����1~M��
//unsigned char Main_Get_ReqData_1_F19(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{//��ǰ�������������֮��
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned short m=(*pusPos);
//	STDATETIME stTempTime;
//	STHISSUMVALGROUP stHisSumValGroup;
//	unsigned char ucSumGNo;
//	unsigned char ucFlag1;
//	float fVal,fFeeT,fFee1,fFee2,fFee3,fFee4;
//	unsigned long ulDT3;
//	STHISSUMGROUPRUNDATA *pstHisSumGRunData;
//	ucSumGNo=ucDAp;
//	if(ucSumGNo>=CON_SUMGROUP_NUM)
//		return RET_ERROR;
//	fFeeT=CONINVALIDVAL;	fFee1=CONINVALIDVAL;	fFee2=CONINVALIDVAL;	
//	fFee3=CONINVALIDVAL;	fFee4=CONINVALIDVAL;
//	pstHisSumGRunData=(STHISSUMGROUPRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//	pucRetBuffer[m++]=4;								//������
//	memset(pucRetBuffer+m,0xEE,20);
//	stTempTime=GetCurrentTime();
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
//	ucFlag1=DataProc_GetSumGData_Day(ucSumGNo,stTempTime,pstHisSumGRunData);
//	if(ucFlag1==RET_SUCCESS)
//	{
//		stHisSumValGroup = Get_History_PowValSumVal(0,0);
//		fVal=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//		if(fVal != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayPowTotal != CONINVALIDVAL)
//		{
//			if(fVal > pstHisSumGRunData->fSumGroupLastDayPowTotal)
//				fFeeT=fVal - pstHisSumGRunData->fSumGroupLastDayPowTotal;
//			else
//				fFeeT=0;
//		}else
//			fFeeT=fVal;
//		stHisSumValGroup = Get_History_PowValSumVal(0,1);
//		fVal=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//		if(fVal != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayPowFee1 != CONINVALIDVAL)
//		{
//			if(fVal > pstHisSumGRunData->fSumGroupLastDayPowFee1)
//				fFee1=fVal - pstHisSumGRunData->fSumGroupLastDayPowFee1;
//			else
//				fFee1=0;
//		}else
//			fFee1=fVal;
//		stHisSumValGroup = Get_History_PowValSumVal(0,2);
//		fVal=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//		if(fVal != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayPowFee2 != CONINVALIDVAL)
//		{
//			if(fVal > pstHisSumGRunData->fSumGroupLastDayPowFee2)
//				fFee2=fVal - pstHisSumGRunData->fSumGroupLastDayPowFee2;
//			else
//				fFee2=0;
//		}else
//			fFee2=fVal;
//		stHisSumValGroup = Get_History_PowValSumVal(0,3);
//		fVal=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//		if(fVal != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayPowFee3 != CONINVALIDVAL)
//		{
//			if(fVal > pstHisSumGRunData->fSumGroupLastDayPowFee3)
//				fFee3=fVal - pstHisSumGRunData->fSumGroupLastDayPowFee3;
//			else
//				fFee3=0;
//		}else
//			fFee3=fVal;
//		stHisSumValGroup = Get_History_PowValSumVal(0,4);
//		fVal=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//		if(fVal != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayPowFee4 != CONINVALIDVAL)
//		{
//			if(fVal > pstHisSumGRunData->fSumGroupLastDayPowFee4)
//				fFee4=fVal - pstHisSumGRunData->fSumGroupLastDayPowFee4;
//			else
//				fFee4=0;
//		}else
//			fFee4=fVal;
//	}else
//	{
//		stHisSumValGroup = Get_History_PowValSumVal(0,0);
//		fFeeT=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//		stHisSumValGroup = Get_History_PowValSumVal(0,1);
//		fFee1=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//		stHisSumValGroup = Get_History_PowValSumVal(0,2);
//		fFee2=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//		stHisSumValGroup = Get_History_PowValSumVal(0,3);
//		fFee3=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//		stHisSumValGroup = Get_History_PowValSumVal(0,4);
//		fFee4=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//	}
//	ulDT3=Float_To_DT3_Div100(fFeeT);
//	memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	ulDT3=Float_To_DT3_Div100(fFee1);
//	memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	ulDT3=Float_To_DT3_Div100(fFee2);
//	memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	ulDT3=Float_To_DT3_Div100(fFee3);
//	memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	ulDT3=Float_To_DT3_Div100(fFee4);
//	memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	(*pusPos)=m;
//	return ucRet;
//}
//
////F20	�����ܼ��޹����������ܡ�����1~M��
//unsigned char Main_Get_ReqData_1_F20(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned short m=(*pusPos);
//	STDATETIME stTempTime;
//	STHISSUMVALGROUP stHisSumValGroup;
//	unsigned char ucSumGNo;
//	unsigned char ucFlag1;
//	float fVal,fFeeT,fFee1,fFee2,fFee3,fFee4;
//	unsigned long ulDT3;
//	STHISSUMGROUPRUNDATA *pstHisSumGRunData;
//	ucSumGNo=ucDAp;
//	if(ucSumGNo>=CON_SUMGROUP_NUM)
//		return RET_ERROR;
//	fFeeT=CONINVALIDVAL;	fFee1=CONINVALIDVAL;	fFee2=CONINVALIDVAL;	
//	fFee3=CONINVALIDVAL;	fFee4=CONINVALIDVAL;
//	pstHisSumGRunData=(STHISSUMGROUPRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//	pucRetBuffer[m++]=4;								//������
//	memset(pucRetBuffer+m,0xEE,20);
//	stTempTime=GetCurrentTime();
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
//	ucFlag1=DataProc_GetSumGData_Day(ucSumGNo,stTempTime,pstHisSumGRunData);
//	if(ucFlag1==RET_SUCCESS)
//	{
//		stHisSumValGroup = Get_History_VarValSumVal(0,0);
//		fVal=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//		if(fVal != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayVarTotal != CONINVALIDVAL)
//		{
//			if(fVal > pstHisSumGRunData->fSumGroupLastDayVarTotal)
//				fFeeT=fVal - pstHisSumGRunData->fSumGroupLastDayVarTotal;
//			else
//				fFeeT=0;
//		}else
//			fFeeT=fVal;
//		stHisSumValGroup = Get_History_VarValSumVal(0,1);
//		fVal=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//		if(fVal != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayVarFee1 != CONINVALIDVAL)
//		{
//			if(fVal > pstHisSumGRunData->fSumGroupLastDayVarFee1)
//				fFee1=fVal - pstHisSumGRunData->fSumGroupLastDayVarFee1;
//			else
//				fFee1=0;
//		}else
//			fFee1=fVal;
//		stHisSumValGroup = Get_History_VarValSumVal(0,2);
//		fVal=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//		if(fVal != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayVarFee2 != CONINVALIDVAL)
//		{
//			if(fVal > pstHisSumGRunData->fSumGroupLastDayVarFee2)
//				fFee2=fVal - pstHisSumGRunData->fSumGroupLastDayVarFee2;
//			else
//				fFee2=0;
//		}else
//			fFee2=fVal;
//		stHisSumValGroup = Get_History_VarValSumVal(0,3);
//		fVal=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//		if(fVal != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayVarFee3 != CONINVALIDVAL)
//		{
//			if(fVal > pstHisSumGRunData->fSumGroupLastDayVarFee3)
//				fFee3=fVal - pstHisSumGRunData->fSumGroupLastDayVarFee3;
//			else
//				fFee3=0;
//		}else
//			fFee3=fVal;
//		stHisSumValGroup = Get_History_VarValSumVal(0,4);
//		fVal=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//		if(fVal != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayVarFee4 != CONINVALIDVAL)
//		{
//			if(fVal > pstHisSumGRunData->fSumGroupLastDayVarFee4)
//				fFee4=fVal - pstHisSumGRunData->fSumGroupLastDayVarFee4;
//			else
//				fFee4=0;
//		}else
//			fFee4=fVal;
//	}else
//	{
//		stHisSumValGroup = Get_History_VarValSumVal(0,0);
//		fFeeT=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//		stHisSumValGroup = Get_History_VarValSumVal(0,1);
//		fFee1=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//		stHisSumValGroup = Get_History_VarValSumVal(0,2);
//		fFee2=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//		stHisSumValGroup = Get_History_VarValSumVal(0,3);
//		fFee3=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//		stHisSumValGroup = Get_History_VarValSumVal(0,4);
//		fFee4=stHisSumValGroup.stSumValGroup.fSumVal[ucSumGNo];
//	}
//	ulDT3=Float_To_DT3_Div100(fFeeT);
//	memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	ulDT3=Float_To_DT3_Div100(fFee1);
//	memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	ulDT3=Float_To_DT3_Div100(fFee2);
//	memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	ulDT3=Float_To_DT3_Div100(fFee3);
//	memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	ulDT3=Float_To_DT3_Div100(fFee4);
//	memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	
//	(*pusPos)=m;
//	return ucRet;
//}
//
////F21	�����ܼ��й����������ܡ�����1~M��
//unsigned char Main_Get_ReqData_1_F21(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned short m=(*pusPos);
//	STDATETIME stTempTime;
//	unsigned char ucGroupNo;
//	unsigned char ucFlag1;
//	STHISSUMVALGROUP stHisSumValGroup;
//	float fVal=0;
//	float fFeeT,fFee1,fFee2,fFee3,fFee4;
//	unsigned long ulDT3;
//	STHISSUMGROUPRUNDATA *pstHisSumGRunData;
//	ucGroupNo = ucDAp;
//	if(ucGroupNo>=CON_SUMGROUP_NUM)
//		return RET_ERROR;
//	pucRetBuffer[m++]=4;								//������
//	memset(pucRetBuffer+m,0xEE,20);
//	fFeeT=CONINVALIDVAL;	fFee1=CONINVALIDVAL;	fFee2=CONINVALIDVAL;	
//	fFee3=CONINVALIDVAL;	fFee4=CONINVALIDVAL;
//	stTempTime=GetCurrentTime();
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
//	stTempTime.ucDay=28;
//	pstHisSumGRunData=(STHISSUMGROUPRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//	ucFlag1=DataProc_GetSumGData_Month(ucGroupNo,stTempTime,pstHisSumGRunData);
//	if(ucFlag1==RET_SUCCESS)
//	{
//		stHisSumValGroup=Get_History_PowValSumVal(0,0);
//		fVal=stHisSumValGroup.stSumValGroup.fSumVal[ucGroupNo];
//		if(fVal != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayPowTotal != CONINVALIDVAL)
//		{
//			if(fVal > pstHisSumGRunData->fSumGroupLastDayPowTotal)
//				fFeeT=fVal - pstHisSumGRunData->fSumGroupLastDayPowTotal;
//			else
//				fFeeT=0;
//		}
//		stHisSumValGroup=Get_History_PowValSumVal(0,1);
//		fVal=stHisSumValGroup.stSumValGroup.fSumVal[ucGroupNo];
//		if(fVal != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayPowFee1 != CONINVALIDVAL)
//		{
//			if(fVal > pstHisSumGRunData->fSumGroupLastDayPowFee1)
//				fFee1=fVal - pstHisSumGRunData->fSumGroupLastDayPowFee1;
//			else
//				fFee1=0;
//		}
//		stHisSumValGroup=Get_History_PowValSumVal(0,2);
//		fVal=stHisSumValGroup.stSumValGroup.fSumVal[ucGroupNo];
//		if(fVal != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayPowFee2 != CONINVALIDVAL)
//		{
//			if(fVal > pstHisSumGRunData->fSumGroupLastDayPowFee2)
//				fFee2=fVal - pstHisSumGRunData->fSumGroupLastDayPowFee2;
//			else
//				fFee2=0;
//		}
//		stHisSumValGroup=Get_History_PowValSumVal(0,3);
//		fVal=stHisSumValGroup.stSumValGroup.fSumVal[ucGroupNo];
//		if(fVal != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayPowFee3 != CONINVALIDVAL)
//		{
//			if(fVal > pstHisSumGRunData->fSumGroupLastDayPowFee3)
//				fFee3=fVal - pstHisSumGRunData->fSumGroupLastDayPowFee3;
//			else
//				fFee3=0;
//		}
//		stHisSumValGroup=Get_History_PowValSumVal(0,4);
//		fVal=stHisSumValGroup.stSumValGroup.fSumVal[ucGroupNo];
//		if(fVal != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayPowFee4 != CONINVALIDVAL)
//		{
//			if(fVal > pstHisSumGRunData->fSumGroupLastDayPowFee4)
//				fFee4=fVal - pstHisSumGRunData->fSumGroupLastDayPowFee4;
//			else
//				fFee4=0;
//		}
//	}else
//	{
//		stHisSumValGroup = Get_History_PowValSumVal(0,0);
//		fFeeT=stHisSumValGroup.stSumValGroup.fSumVal[ucGroupNo];
//		stHisSumValGroup = Get_History_PowValSumVal(0,1);
//		fFee1=stHisSumValGroup.stSumValGroup.fSumVal[ucGroupNo];
//		stHisSumValGroup = Get_History_PowValSumVal(0,2);
//		fFee2=stHisSumValGroup.stSumValGroup.fSumVal[ucGroupNo];
//		stHisSumValGroup = Get_History_PowValSumVal(0,3);
//		fFee3=stHisSumValGroup.stSumValGroup.fSumVal[ucGroupNo];
//		stHisSumValGroup = Get_History_PowValSumVal(0,4);
//		fFee4=stHisSumValGroup.stSumValGroup.fSumVal[ucGroupNo];
//	}
//	ulDT3=Float_To_DT3_Div100(fFeeT);
//	memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	ulDT3=Float_To_DT3_Div100(fFee1);
//	memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	ulDT3=Float_To_DT3_Div100(fFee2);
//	memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	ulDT3=Float_To_DT3_Div100(fFee3);
//	memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	ulDT3=Float_To_DT3_Div100(fFee4);
//	memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//
//	(*pusPos)=m;
//	return ucRet;
//}
//
////F22	�����ܼ��޹����������ܡ�����1~M��
//unsigned char Main_Get_ReqData_1_F22(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned short m=(*pusPos);
//	STDATETIME stTempTime;
//	unsigned char ucGroupNo;
//	unsigned char ucFlag1;
//	STHISSUMVALGROUP stHisSumValGroup;
//	float fVal=0;
//	float fFeeT,fFee1,fFee2,fFee3,fFee4;
//	unsigned long ulDT3;
//	STHISSUMGROUPRUNDATA *pstHisSumGRunData;
//	ucGroupNo = ucDAp;
//	if(ucGroupNo>=CON_SUMGROUP_NUM)
//		return RET_ERROR;
//	pucRetBuffer[m++]=4;								//������
//	memset(pucRetBuffer+m,0xEE,20);
//	fFeeT=CONINVALIDVAL;	fFee1=CONINVALIDVAL;	fFee2=CONINVALIDVAL;	
//	fFee3=CONINVALIDVAL;	fFee4=CONINVALIDVAL;
//	stTempTime=GetCurrentTime();
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
//	stTempTime.ucDay=28;
//	pstHisSumGRunData=(STHISSUMGROUPRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//	ucFlag1=DataProc_GetSumGData_Month(ucGroupNo,stTempTime,pstHisSumGRunData);
//	if(ucFlag1==RET_SUCCESS)
//	{
//		stHisSumValGroup=Get_History_VarValSumVal(0,0);
//		fVal=stHisSumValGroup.stSumValGroup.fSumVal[ucGroupNo];
//		if(fVal != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayVarTotal != CONINVALIDVAL)
//		{
//			if(fVal > pstHisSumGRunData->fSumGroupLastDayVarTotal)
//				fFeeT=fVal - pstHisSumGRunData->fSumGroupLastDayVarTotal;
//			else
//				fFeeT=0;
//		}
//		stHisSumValGroup=Get_History_VarValSumVal(0,1);
//		fVal=stHisSumValGroup.stSumValGroup.fSumVal[ucGroupNo];
//		if(fVal != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayVarFee1 != CONINVALIDVAL)
//		{
//			if(fVal > pstHisSumGRunData->fSumGroupLastDayVarFee1)
//				fFee1=fVal - pstHisSumGRunData->fSumGroupLastDayVarFee1;
//			else
//				fFee1=0;
//		}
//		stHisSumValGroup=Get_History_VarValSumVal(0,2);
//		fVal=stHisSumValGroup.stSumValGroup.fSumVal[ucGroupNo];
//		if(fVal != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayVarFee2 != CONINVALIDVAL)
//		{
//			if(fVal > pstHisSumGRunData->fSumGroupLastDayVarFee2)
//				fFee2=fVal - pstHisSumGRunData->fSumGroupLastDayVarFee2;
//			else
//				fFee2=0;
//		}
//		stHisSumValGroup=Get_History_VarValSumVal(0,3);
//		fVal=stHisSumValGroup.stSumValGroup.fSumVal[ucGroupNo];
//		if(fVal != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayVarFee3 != CONINVALIDVAL)
//		{
//			if(fVal > pstHisSumGRunData->fSumGroupLastDayVarFee3)
//				fFee3=fVal - pstHisSumGRunData->fSumGroupLastDayVarFee3;
//			else
//				fFee3=0;
//		}
//		stHisSumValGroup=Get_History_VarValSumVal(0,4);
//		fVal=stHisSumValGroup.stSumValGroup.fSumVal[ucGroupNo];
//		if(fVal != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayVarFee4 != CONINVALIDVAL)
//		{
//			if(fVal > pstHisSumGRunData->fSumGroupLastDayVarFee4)
//				fFee4=fVal - pstHisSumGRunData->fSumGroupLastDayVarFee4;
//			else
//				fFee4=0;
//		}
//	}else
//	{
//		stHisSumValGroup = Get_History_VarValSumVal(0,0);
//		fFeeT=stHisSumValGroup.stSumValGroup.fSumVal[ucGroupNo];
//		stHisSumValGroup = Get_History_VarValSumVal(0,1);
//		fFee1=stHisSumValGroup.stSumValGroup.fSumVal[ucGroupNo];
//		stHisSumValGroup = Get_History_VarValSumVal(0,2);
//		fFee2=stHisSumValGroup.stSumValGroup.fSumVal[ucGroupNo];
//		stHisSumValGroup = Get_History_VarValSumVal(0,3);
//		fFee3=stHisSumValGroup.stSumValGroup.fSumVal[ucGroupNo];
//		stHisSumValGroup = Get_History_VarValSumVal(0,4);
//		fFee4=stHisSumValGroup.stSumValGroup.fSumVal[ucGroupNo];
//	}
//	ulDT3=Float_To_DT3_Div100(fFeeT);
//	memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	ulDT3=Float_To_DT3_Div100(fFee1);
//	memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	ulDT3=Float_To_DT3_Div100(fFee2);
//	memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	ulDT3=Float_To_DT3_Div100(fFee3);
//	memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	ulDT3=Float_To_DT3_Div100(fFee4);
//	memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//
//	(*pusPos)=m;
//	return ucRet;
//}
//
////F23	�ն˵�ǰʣ��������ѣ�
////������������Թ����
//unsigned char Main_Get_ReqData_1_F23(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned short m=(*pusPos);
//	unsigned char ucGroupNo;
//	double dLeftPowVal=0;
//	STENERGYCTRL	stEnergyCtrl;
//	unsigned long ulSign=0;
//	unsigned long ulVal;
//
//	ucGroupNo=ucDAp;
//	if(ucGroupNo<CON_SUMGROUP_NUM)
//	{
//		stEnergyCtrl = RunPara_GetEnergyCtrl(ucGroupNo);
////		if(stEnergyCtrl.ucEnergyCtrlType==CON_ENERGYCTRL_MON)
////			Load_EnergyCtrl_MonFlash(&stEnergyCtrl);
////		dLeftPowVal = Load_Energy_CalcBuy_LeftPowVal_NotExe(&stEnergyCtrl,ucGroupNo);
//		dLeftPowVal = Load_Energy_LeftPowVal_CalcBuy(&stEnergyCtrl,ucGroupNo);
//		if(dLeftPowVal!=CONINVALIDVAL)
//		{
//			if(dLeftPowVal<0)
//			{
//				dLeftPowVal=0-dLeftPowVal;
//				ulSign=0x10000000;
//			}
//			if(Load_Check_EnerBuy_MWh_YUN()==CON_STATUS_YES && dLeftPowVal>=1000)
//			{
//				ulVal=(unsigned long)(dLeftPowVal/1000);
//				ulSign += 0x40000000;
//			}
//			else
//			{
//				if(dLeftPowVal>9999999)
//				{
//					ulVal=(unsigned long)(dLeftPowVal/1000);
//					ulSign += 0x40000000;
//				}
//				else
//				{
//					ulVal=(unsigned long)dLeftPowVal;
//				}
//			}
//			if(ulVal>9999999)
//				ulVal=9999999;				//�������9999999
//			ulVal=HEX_TO_BCD(ulVal,4);
//			ulVal=ulVal+ulSign;
//			memcpy(pucRetBuffer+m,&(ulVal),4);	m+=4;
//		}else
//		{
//			pucRetBuffer[m++]=0xEE;
//			pucRetBuffer[m++]=0xEE;
//			pucRetBuffer[m++]=0xEE;
//			pucRetBuffer[m++]=0xEE;
//		}
//		ucRet=RET_SUCCESS;
//	}
//	(*pusPos)=m;
//	return ucRet;
//}
//
////F24	��ǰ�����¸��ؿغ��ܼ��й����ʶ���ֵ
//unsigned char Main_Get_ReqData_1_F24(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{//��Ҫ�����ܼ����
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned short m=(*pusPos);
//	double dVal;
//	unsigned short usDT2=0;
//	dVal=Get_PowLower_Val();
//	if(dVal==CONINVALIDVAL)
//		usDT2=0xEEEE;
//	else
//		usDT2=Float_To_DT2((float)dVal);
//	pucRetBuffer[m++]=(unsigned char)usDT2;
//	pucRetBuffer[m++]=(unsigned char)(usDT2>>8);
//	(*pusPos)=m;
//	return ucRet;
//}

//F25	��ǰ���༰����/�޹����ʡ����������������ѹ���������������,���ڹ���
unsigned char Main_Get_ReqData_1_F25(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned short m=(*pusPos);
	unsigned char ucMPNo=0;
//	unsigned char ucMPType;

	double dVal=0;
	
	STHISVAL	  stHisVal;
	STHISVAL	  stHisVal1;
	unsigned long  	ulVala,ulValb,ulValc,ulValn;
	STHISVAL		stP_Total,stP_A,stP_B,stP_C;

	ucMPNo = ucDAp;
//	ucMPType = RunPara_GetMPType(ucMPNo);
	stHisVal = Get_History_DirPower_byMPNo(ucMPNo,0,0);
	if(Is_DateTime_Availble(&stHisVal.stDateTime)!=RET_SUCCESS)
	{
		stHisVal1 = Get_History_DirVar_byMPNo(ucMPNo,0,0);
		m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal1.stDateTime);
	}else
		m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);

	for(i=0;i<4;i++)
	{
		stHisVal = Get_History_InstantPow_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_3B_DT09( pucRetBuffer ,m, stHisVal.lVal,stHisVal.ucSign);
	}
/*	stHisVal = Get_History_InstantPow_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_lVal_3B_DT09( pucRetBuffer ,m, stHisVal.lVal,stHisVal.ucSign);
	
	stHisVal = Get_History_InstantPow_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_3B_DT09( pucRetBuffer ,m, stHisVal.lVal,stHisVal.ucSign);
	
	stHisVal = Get_History_InstantPow_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_3B_DT09( pucRetBuffer ,m, stHisVal.lVal,stHisVal.ucSign);
	
	stHisVal = Get_History_InstantPow_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_3B_DT09( pucRetBuffer ,m, stHisVal.lVal,stHisVal.ucSign);*/

	for(i=0;i<4;i++)
	{
//		if(ucMPType==CON_MPTYPE_TER)
//		{
//			stHisVal = Get_History_InstantVar_Disp_byMPNo(ucMPNo,0,i);
//		}else
//		{
			stHisVal = Get_History_InstantVar_byMPNo(ucMPNo,0,i);
//			if((stHisVal.lVal&0xff) !=CONINVALIDVAL) 
            if(stHisVal.lVal !=CONINVALIDVAL)//azh 140918
				stHisVal.lVal *=100;
//		}
		m=SetMainComm_Buf_lVal_3B_DT09( pucRetBuffer ,m, stHisVal.lVal,stHisVal.ucSign);
	}
	
/*	if(ucMPType==CON_MPTYPE_TER)
	{
		stHisVal = Get_History_InstantVar_Disp_byMPNo(ucMPNo,0,0);
	}else
	{
		stHisVal = Get_History_InstantVar_byMPNo(ucMPNo,0,0);
		if(stHisVal.lVal !=CONINVALIDVAL)
			stHisVal.lVal *=100;
	}
	m=SetMainComm_Buf_lVal_3B_DT09( pucRetBuffer ,m, stHisVal.lVal,stHisVal.ucSign);
	
	if(ucMPType==CON_MPTYPE_TER)
	{
		stHisVal = Get_History_InstantVar_Disp_byMPNo(ucMPNo,0,1);
	}else
	{
		stHisVal = Get_History_InstantVar_byMPNo(ucMPNo,0,1);
		if(stHisVal.lVal !=CONINVALIDVAL)
			stHisVal.lVal *=100;
	}
	m=SetMainComm_Buf_lVal_3B_DT09( pucRetBuffer ,m, stHisVal.lVal,stHisVal.ucSign);
	
	if(ucMPType==CON_MPTYPE_TER)
	{
		stHisVal = Get_History_InstantVar_Disp_byMPNo(ucMPNo,0,2);
	}else
	{
		stHisVal = Get_History_InstantVar_byMPNo(ucMPNo,0,2);
		if(stHisVal.lVal !=CONINVALIDVAL)
			stHisVal.lVal *=100;
	}
	m=SetMainComm_Buf_lVal_3B_DT09( pucRetBuffer ,m, stHisVal.lVal,stHisVal.ucSign);
	
	if(ucMPType==CON_MPTYPE_TER)
	{
		stHisVal = Get_History_InstantVar_Disp_byMPNo(ucMPNo,0,3);
	}else
	{
		stHisVal = Get_History_InstantVar_byMPNo(ucMPNo,0,3);
		if(stHisVal.lVal !=CONINVALIDVAL)
			stHisVal.lVal *=100;
	}
	m=SetMainComm_Buf_lVal_3B_DT09( pucRetBuffer ,m, stHisVal.lVal,stHisVal.ucSign);*/
	

//	if(ucMPType==CON_MPTYPE_TER)
//	{
//		for(i=0;i<4;i++)
//		{
//			stHisVal = Get_History_InstantPow_byMPNo(ucMPNo,0,i);
//			stHisVal1= Get_History_InstantVar_Disp_byMPNo(ucMPNo,0,i);
//			if(stHisVal.lVal!=CONINVALIDVAL && stHisVal1.lVal!=CONINVALIDVAL)
//			{
//			dVal = sqrt( (stHisVal.lVal*stHisVal.lVal+stHisVal1.lVal*stHisVal1.lVal) );
//				if(i==0)
//					stP_Total.lVal = (unsigned long)dVal;
//				else if(i==1)
//					stP_A.lVal= (unsigned long)dVal;
//				else if(i==2)
//					stP_B.lVal= (unsigned long)dVal;
//				else
//					stP_C.lVal= (unsigned long)dVal;
//					
//				stHisVal.lVal = GetPQRate_Var4B_01(stHisVal.lVal,stHisVal1.lVal);
//			}
//			else
//			{
//				ClearHisVal(&stHisVal);
//				if(i==0)
//					ClearHisVal(&stP_Total);
//				else if(i==1)
//					ClearHisVal(&stP_A);
//				else if(i==2)
//					ClearHisVal(&stP_B);
//				else
//					ClearHisVal(&stP_C);
//			}
//			m=SetMainComm_Buf_lVal_2B_Sign( pucRetBuffer ,m, stHisVal.lVal,stHisVal.ucSign);			
//		}
//		/*
//		stHisVal = Get_History_InstantPow_byMPNo(ucMPNo,0,0);
//		stHisVal1= Get_History_InstantVar_Disp_byMPNo(ucMPNo,0,0);
//		if(stHisVal.lVal!=CONINVALIDVAL && stHisVal1.lVal!=CONINVALIDVAL)
//		{
//			dVal = sqrt( (stHisVal.lVal*stHisVal.lVal+stHisVal1.lVal*stHisVal1.lVal) );
//			stP_Total.lVal = dVal;
//			stHisVal.lVal = GetPQRate_Var4B_01(stHisVal.lVal,stHisVal1.lVal);
//		}
//		else
//		{
//			ClearHisVal(&stHisVal);
//			ClearHisVal(&stP_Total);
//		}
//		m=SetMainComm_Buf_lVal_2B_Sign( pucRetBuffer ,m, stHisVal.lVal,stHisVal.ucSign);
//		
//		
//		stHisVal = Get_History_InstantPow_byMPNo(ucMPNo,0,1);
//		stHisVal1= Get_History_InstantVar_Disp_byMPNo(ucMPNo,0,1);
//		if(stHisVal.lVal!=CONINVALIDVAL && stHisVal1.lVal!=CONINVALIDVAL)
//		{
//			dVal = sqrt( (stHisVal.lVal*stHisVal.lVal+stHisVal1.lVal*stHisVal1.lVal) );
//			stP_A.lVal = dVal;
//			stHisVal.lVal = GetPQRate_Var4B_01(stHisVal.lVal,stHisVal1.lVal);		
//		}else
//		{
//			ClearHisVal(&stHisVal);
//			ClearHisVal(&stP_A);
//		}
//		m=SetMainComm_Buf_lVal_2B_Sign( pucRetBuffer ,m, stHisVal.lVal,stHisVal.ucSign);
//		
//		stHisVal = Get_History_InstantPow_byMPNo(ucMPNo,0,2);
//		stHisVal1= Get_History_InstantVar_Disp_byMPNo(ucMPNo,0,2);
//		if(stHisVal.lVal!=CONINVALIDVAL && stHisVal1.lVal!=CONINVALIDVAL)
//		{
//			dVal = sqrt( (stHisVal.lVal*stHisVal.lVal+stHisVal1.lVal*stHisVal1.lVal) );
//			stP_B.lVal = dVal;
//			stHisVal.lVal = GetPQRate_Var4B_01(stHisVal.lVal,stHisVal1.lVal);		
//		}else
//		{
//			ClearHisVal(&stHisVal);
//			ClearHisVal(&stP_B);
//		}
//		m=SetMainComm_Buf_lVal_2B_Sign( pucRetBuffer ,m, stHisVal.lVal,stHisVal.ucSign);
//
//		stHisVal = Get_History_InstantPow_byMPNo(ucMPNo,0,3);
//		stHisVal1= Get_History_InstantVar_Disp_byMPNo(ucMPNo,0,3);
//		if(stHisVal.lVal!=CONINVALIDVAL && stHisVal1.lVal!=CONINVALIDVAL)
//		{
//			dVal = sqrt( (stHisVal.lVal*stHisVal.lVal+stHisVal1.lVal*stHisVal1.lVal) );
//			stP_C.lVal = dVal;
//			stHisVal.lVal = GetPQRate_Var4B_01(stHisVal.lVal,stHisVal1.lVal);		
//		}else
//		{
//			ClearHisVal(&stHisVal);
//			ClearHisVal(&stP_C);
//		}
//		m=SetMainComm_Buf_lVal_2B_Sign( pucRetBuffer ,m, stHisVal.lVal,stHisVal.ucSign);*/
//		
//		for(i=1;i<4;i++)
//		{
//			stHisVal = Get_History_V_Disp_byMPNo(ucMPNo,0,i);
//			if(stHisVal.lVal!=CONINVALIDVAL)
//				stHisVal.lVal=(stHisVal.lVal+5)/10;
//			m=SetMainComm_Buf_lVal_2B( pucRetBuffer ,m, stHisVal.lVal);
//		}
///*		stHisVal = Get_History_V_Disp_byMPNo(ucMPNo,0,1);
//		if(stHisVal.lVal!=CONINVALIDVAL)
//			stHisVal.lVal=(stHisVal.lVal+5)/10;
//		m=SetMainComm_Buf_lVal_2B( pucRetBuffer ,m, stHisVal.lVal);
//		
//		stHisVal = Get_History_V_Disp_byMPNo(ucMPNo,0,2);
//		if(stHisVal.lVal!=CONINVALIDVAL)
//			stHisVal.lVal=(stHisVal.lVal+5)/10;
//		m=SetMainComm_Buf_lVal_2B( pucRetBuffer ,m, stHisVal.lVal);
//		
//		stHisVal = Get_History_V_Disp_byMPNo(ucMPNo,0,3);
//		if(stHisVal.lVal!=CONINVALIDVAL)
//			stHisVal.lVal=(stHisVal.lVal+5)/10;
//		m=SetMainComm_Buf_lVal_2B( pucRetBuffer ,m, stHisVal.lVal);*/
//	}
//	else
//	{
		for(i=0;i<4;i++)
		{
			//azh 140610��������Ĺ���С��λû����Ҫע��
			stHisVal = Get_History_InstantPow_byMPNo(ucMPNo,0,i); //xx.xxxx
			stHisVal1= Get_History_InstantVar_byMPNo(ucMPNo,0,i); //xx.xx
			if(stHisVal.lVal!=CONINVALIDVAL && stHisVal1.lVal!=CONINVALIDVAL)//azh 140918
			{
				dVal = sqrt( ((stHisVal.lVal/100)*(stHisVal.lVal/100)+stHisVal1.lVal*stHisVal1.lVal) );
				if(i==0)
					stP_Total.lVal = (unsigned long)dVal*100;
				else if(i==1)
					stP_A.lVal= (unsigned long)dVal*100;
				else if(i==2)
					stP_B.lVal= (unsigned long)dVal*100;
				else
					stP_C.lVal= (unsigned long)dVal*100;
				stHisVal.lVal = GetPQRate(stHisVal.lVal,stHisVal1.lVal);//azh 140610�˺������й��ǳ���100�� ע��	
				stHisVal.lVal *=10;
			}else
			{
				ClearHisVal(&stHisVal);
				if(i==0)
					ClearHisVal(&stP_Total);
				else if(i==1)
					ClearHisVal(&stP_A);
				else if(i==2)
					ClearHisVal(&stP_B);
				else
					ClearHisVal(&stP_C);
			}
			m=SetMainComm_Buf_lVal_2B( pucRetBuffer ,m, stHisVal.lVal);
		}
		/*
		stHisVal = Get_History_InstantPow_byMPNo(ucMPNo,0,0);
		stHisVal1= Get_History_InstantVar_byMPNo(ucMPNo,0,0);
		if(stHisVal.lVal!=CONINVALIDVAL && stHisVal1.lVal!=CONINVALIDVAL)
		{
			dVal = sqrt( (stHisVal.lVal*stHisVal.lVal+stHisVal1.lVal*stHisVal1.lVal) );
			stP_Total.lVal = dVal;
			stHisVal.lVal = GetPQRate(stHisVal.lVal,stHisVal1.lVal);	
			stHisVal.lVal *=10;
		}else
		{
			ClearHisVal(&stHisVal);
			ClearHisVal(&stP_Total);
		}
		m=SetMainComm_Buf_lVal_2B( pucRetBuffer ,m, stHisVal.lVal);
		
		stHisVal = Get_History_InstantPow_byMPNo(ucMPNo,0,1);
		stHisVal1= Get_History_InstantVar_byMPNo(ucMPNo,0,1);
		if(stHisVal.lVal!=CONINVALIDVAL && stHisVal1.lVal!=CONINVALIDVAL)
		{
			dVal = sqrt( (stHisVal.lVal*stHisVal.lVal+stHisVal1.lVal*stHisVal1.lVal) );
			stP_A.lVal = dVal;
			stHisVal.lVal = GetPQRate(stHisVal.lVal,stHisVal1.lVal);
			stHisVal.lVal *=10;
		}else
		{
			ClearHisVal(&stHisVal);
			ClearHisVal(&stP_A);
		}
		m=SetMainComm_Buf_lVal_2B( pucRetBuffer ,m, stHisVal.lVal);
		
		stHisVal = Get_History_InstantPow_byMPNo(ucMPNo,0,2);
		stHisVal1= Get_History_InstantVar_byMPNo(ucMPNo,0,2);
		if(stHisVal.lVal!=CONINVALIDVAL && stHisVal1.lVal!=CONINVALIDVAL)
		{
			dVal = sqrt( (stHisVal.lVal*stHisVal.lVal+stHisVal1.lVal*stHisVal1.lVal) );
			stP_B.lVal = dVal;
			stHisVal.lVal = GetPQRate(stHisVal.lVal,stHisVal1.lVal);
			stHisVal.lVal *=10;
		}else
		{
			ClearHisVal(&stHisVal);
			ClearHisVal(&stP_B);
		}
		m=SetMainComm_Buf_lVal_2B( pucRetBuffer ,m, stHisVal.lVal);
		
		stHisVal = Get_History_InstantPow_byMPNo(ucMPNo,0,3);
		stHisVal1= Get_History_InstantVar_byMPNo(ucMPNo,0,3);
		if(stHisVal.lVal!=CONINVALIDVAL && stHisVal1.lVal!=CONINVALIDVAL)
		{
			dVal = sqrt( (stHisVal.lVal*stHisVal.lVal+stHisVal1.lVal*stHisVal1.lVal) );
			stP_C.lVal = dVal;
			stHisVal.lVal = GetPQRate(stHisVal.lVal,stHisVal1.lVal);
			stHisVal.lVal *=10;
		}else
		{
			ClearHisVal(&stHisVal);
			ClearHisVal(&stP_C);
		}
		m=SetMainComm_Buf_lVal_2B( pucRetBuffer ,m, stHisVal.lVal);*/

		for(i=1;i<4;i++)
		{
			stHisVal = Get_History_V_byMPNo(ucMPNo,0,i);
			m=SetMainComm_Buf_lVal_2B( pucRetBuffer ,m, stHisVal.lVal);
		}
/*		stHisVal = Get_History_V_byMPNo(ucMPNo,0,1);
		m=SetMainComm_Buf_lVal_2B( pucRetBuffer ,m, stHisVal.lVal);
		
		stHisVal = Get_History_V_byMPNo(ucMPNo,0,2);
		m=SetMainComm_Buf_lVal_2B( pucRetBuffer ,m, stHisVal.lVal);
		
		stHisVal = Get_History_V_byMPNo(ucMPNo,0,3);
		m=SetMainComm_Buf_lVal_2B( pucRetBuffer ,m, stHisVal.lVal);*/
//	}
	stHisVal = Get_History_I_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_3B_Sign( pucRetBuffer ,m, stHisVal.lVal,stHisVal.ucSign);
	ulVala=stHisVal.lVal;
	
	stHisVal = Get_History_I_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_3B_Sign( pucRetBuffer ,m, stHisVal.lVal,stHisVal.ucSign);
	ulValb=stHisVal.lVal;
	
	stHisVal = Get_History_I_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_3B_Sign( pucRetBuffer ,m, stHisVal.lVal,stHisVal.ucSign);
	ulValc=stHisVal.lVal;
	//����������㣬��������������120��
	ulValn=Calc_In(ulVala,ulValb,ulValc);
	if((ulVala==CONINVALIDVAL) || (ulValb==CONINVALIDVAL) || (ulValc==CONINVALIDVAL)) //azh 140918
	{
	    ulValn = CONINVALIDVAL;//azh
	}
	m=SetMainComm_Buf_lVal_3B_Sign( pucRetBuffer ,m, ulValn,0);
	
	m=SetMainComm_Buf_lVal_3B_DT09( pucRetBuffer ,m, stP_Total.lVal,stP_Total.ucSign);
	m=SetMainComm_Buf_lVal_3B_DT09( pucRetBuffer ,m, stP_A.lVal,stP_A.ucSign);
	m=SetMainComm_Buf_lVal_3B_DT09( pucRetBuffer ,m, stP_B.lVal,stP_B.ucSign);
	m=SetMainComm_Buf_lVal_3B_DT09( pucRetBuffer ,m, stP_C.lVal,stP_C.ucSign);
	
	
	(*pusPos)=m;
	return ucRet;
}

//F26	A��B��C�������ͳ�����ݼ����һ�ζ����¼
unsigned char Main_Get_ReqData_1_F26(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned short m=(*pusPos);
	unsigned char ucMPNo=0;

	
	STHISVAL	  stHisVal;
	STHISTIME	  stHisTime;
	
//	if(RunPara_GetMPType(ucDAp)==CON_MPTYPE_TER)
//		return RET_ERROR;
	
	ucMPNo = ucDAp;
	
	stHisVal = Get_History_DirPower_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	for(i=0;i<4;i++)
	{
		stHisVal = Get_History_VolBreakNum_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_2B( pucRetBuffer ,m, stHisVal.lVal);
	}

/*	stHisVal = Get_History_VolBreakNum_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_lVal_2B( pucRetBuffer ,m, stHisVal.lVal);

	stHisVal = Get_History_VolBreakNum_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_2B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_VolBreakNum_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_2B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_VolBreakNum_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_2B( pucRetBuffer ,m, stHisVal.lVal);*/

	for(i=0;i<4;i++)
	{
		stHisVal = Get_History_VolBreakTime_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	}
	
/*	stHisVal = Get_History_VolBreakTime_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_VolBreakTime_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_VolBreakTime_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_VolBreakTime_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);*/

	for(i=0;i<4;i++)
	{
		stHisTime = Get_History_LastVolBreakBegTime_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,stHisTime.stHisTime);
	}
/*	stHisTime = Get_History_LastVolBreakBegTime_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,stHisTime.stHisTime);
	stHisTime = Get_History_LastVolBreakBegTime_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,stHisTime.stHisTime);
	stHisTime = Get_History_LastVolBreakBegTime_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,stHisTime.stHisTime);
	stHisTime = Get_History_LastVolBreakBegTime_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,stHisTime.stHisTime);*/

	for(i=0;i<4;i++)
	{
		stHisTime = Get_History_LastVolBreakEndTime_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,stHisTime.stHisTime);
	}
/*	stHisTime = Get_History_LastVolBreakEndTime_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,stHisTime.stHisTime);
	stHisTime = Get_History_LastVolBreakEndTime_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,stHisTime.stHisTime);
	stHisTime = Get_History_LastVolBreakEndTime_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,stHisTime.stHisTime);
	stHisTime = Get_History_LastVolBreakEndTime_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,stHisTime.stHisTime);*/
	
	(*pusPos)=m;
	return ucRet;
}

//F27	���ܱ�����ʱ�Ӽ����ܱ�״̬��Ϣ
unsigned char Main_Get_ReqData_1_F27(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
//	unsigned char i;
	
	STHISVAL	  stHisVal;
	STHISTIME	  stHisTime;
//	STHISSTATE	  stHisState;
	
//	if(RunPara_GetMPType(ucDAp)==CON_MPTYPE_TER)
//	{
//		stHisTime.stDateTime=GetCurrentTime();
//		m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisTime.stDateTime);
//		m=SetMainComm_Buf_DateTime_HMS( pucRetBuffer ,m, stHisTime.stDateTime);
//		m=SetMainComm_Buf_DateTime_YMDW( pucRetBuffer ,m, stHisTime.stDateTime);
//		memset(pucRetBuffer+m,0xEE,45);
//		m+=45;
//	}
//	else
//	{
		ucMPNo = ucDAp;
		
		stHisTime = Get_History_Time_byMPNo(ucMPNo,0);
		m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisTime.stDateTime);
		
		m=SetMainComm_Buf_DateTime_HMS( pucRetBuffer ,m, stHisTime.stHisTime);
		stHisTime = Get_History_Date_byMPNo(ucMPNo,0);
		m=SetMainComm_Buf_DateTime_YMDW( pucRetBuffer ,m, stHisTime.stHisTime);
		
		stHisVal = Get_History_BatWorkTime_byMPNo(ucMPNo,0);
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
		
		stHisVal = Get_History_ProgramCount_byMPNo(ucMPNo,0);
		m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
		stHisTime = Get_History_ProgramTime_byMPNo(ucMPNo,0);
		m=SetMainComm_Buf_DateTime_YMDHMS( pucRetBuffer ,m, stHisTime.stHisTime);

		stHisVal = Get_History_MeterClrCount_byMPNo(ucMPNo,0);
		m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
		stHisTime = Get_History_MeterClrTime_byMPNo(ucMPNo,0);
		m=SetMainComm_Buf_DateTime_YMDHMS( pucRetBuffer ,m, stHisTime.stHisTime);

		stHisVal = Get_History_DemandClrCount_byMPNo(ucMPNo,0);
		m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
		stHisTime = Get_History_DemandClrTime_byMPNo(ucMPNo,0);
		m=SetMainComm_Buf_DateTime_YMDHMS( pucRetBuffer ,m, stHisTime.stHisTime);

		stHisVal = Get_History_EventClrCount_byMPNo(ucMPNo,0);
		m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
		stHisTime = Get_History_EventClrTime_byMPNo(ucMPNo,0);
		m=SetMainComm_Buf_DateTime_YMDHMS( pucRetBuffer ,m, stHisTime.stHisTime);

		stHisVal = Get_History_JiaoshiCount_byMPNo(ucMPNo,0);
		m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
		stHisTime = Get_History_JiaoshiTime_byMPNo(ucMPNo,0);
		m=SetMainComm_Buf_DateTime_YMDHMS( pucRetBuffer ,m, stHisTime.stHisTime);
//	}
	
	(*pusPos)=m;
	return ucRet;
}
//azh 170921
//F162	���ܱ�����ʱ��
unsigned char Main_Get_ReqData_1_F162(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
//	unsigned char i;
	
//	STHISVAL	  stHisVal;
	STHISTIME	  stHisTime;

	ucMPNo = ucDAp;
	
	stHisTime = Get_History_Time_byMPNo(ucMPNo,0);
//	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisTime.stDateTime);
	
	m=SetMainComm_Buf_DateTime_HMS( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_Date_byMPNo(ucMPNo,0);
	m=SetMainComm_Buf_DateTime_YMDW( pucRetBuffer ,m, stHisTime.stHisTime);

	stHisTime = Get_History_Time_byMPNo(ucMPNo,0);
	m=SetMainComm_Buf_DateTime_HMS( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_Date_byMPNo(ucMPNo,0);
	m=SetMainComm_Buf_DateTime_YMDW( pucRetBuffer ,m, stHisTime.stHisTime);
	
	(*pusPos)=m;
	return ucRet;
}

//F28	���ܱ�����ʱ�Ӽ����ܱ�״̬��Ϣ
unsigned char Main_Get_ReqData_1_F28(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	unsigned char i;
	unsigned char ucmeterno;
	
//	STHISTIME	  stHisTime;
	STHISSTATE	  stHisState;
	
//	if(RunPara_GetMPType(ucDAp)==CON_MPTYPE_TER)
//	{
//		stHisTime.stDateTime=GetCurrentTime();
//		m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisTime.stDateTime);
//		memset(pucRetBuffer+m,0xEE,28);
//		m+=28;
//	}
//	else
//	{
		ucMPNo = ucDAp;
		ucmeterno = RunPara_GetMeterNo_byMPNo(ucMPNo);
		stHisState = Get_History_MeterStatFlag_byMPNo(ucMPNo,0,1);
		m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisState.stDateTime);
		for(i=0;i<7;i++)
		{
			pucRetBuffer[m++] = ucMeterStateChangeFlag[ucmeterno][i];		//״̬��λĿǰ��дΪ0��������и���	cfh_090821
			pucRetBuffer[m++] = 0;
		}
//		memcpy(pucRetBuffer+m,&ucMeterStateChangeFlag[ucmeterno][0])
		for(i=1;i<8;i++)
		{
			stHisState = Get_History_MeterStatFlag_byMPNo(ucMPNo,0,i);
			pucRetBuffer[m++] = stHisState.ucStateFlag;
			if( stHisState.ucStateFlag != (unsigned char)CONINVALIDVAL)
				pucRetBuffer[m++] = 0;
			else
				pucRetBuffer[m++] = (unsigned char)CONINVALIDVAL;
		}
//	}
	
	(*pusPos)=m;
	return ucRet;
}
unsigned char Main_Get_ReqData_1_F31(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	STHISVAL stHisVal;

	ucMPNo = ucDAp;
	stHisVal = Get_History_DirPowA_byMPNo(ucMPNo, 0, 0);
	if(Is_DateTime_Availble(&(stHisVal.stDateTime)) != RET_SUCCESS)
		stHisVal.stDateTime = GetCurrentTime();
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);

	m = SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,stHisVal.lVal);
	stHisVal = Get_History_DirPowA_byMPNo(ucMPNo,0,1);
	m = SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,stHisVal.lVal);
	stHisVal = Get_History_DirPowA_byMPNo(ucMPNo, 0, 2);
	m = SetMainComm_Buf_lVal_4B(pucRetBuffer, m, stHisVal.lVal);
	stHisVal = Get_History_DirPowA_byMPNo(ucMPNo, 0, 3);
	m = SetMainComm_Buf_lVal_4B(pucRetBuffer, m, stHisVal.lVal);
	
	stHisVal = Get_History_DirPowB_byMPNo(ucMPNo, 0, 0);
	m = SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,stHisVal.lVal);
	stHisVal = Get_History_DirPowB_byMPNo(ucMPNo,0,1);
	m = SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,stHisVal.lVal);
	stHisVal = Get_History_DirPowB_byMPNo(ucMPNo, 0, 2);
	m = SetMainComm_Buf_lVal_4B(pucRetBuffer, m, stHisVal.lVal);
	stHisVal = Get_History_DirPowB_byMPNo(ucMPNo, 0, 3);
	m = SetMainComm_Buf_lVal_4B(pucRetBuffer, m, stHisVal.lVal);
	
	stHisVal = Get_History_DirPowC_byMPNo(ucMPNo, 0, 0);
	m = SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,stHisVal.lVal);
	stHisVal = Get_History_DirPowC_byMPNo(ucMPNo,0,1);
	m = SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,stHisVal.lVal);
	stHisVal = Get_History_DirPowC_byMPNo(ucMPNo, 0, 2);
	m = SetMainComm_Buf_lVal_4B(pucRetBuffer, m, stHisVal.lVal);
	stHisVal = Get_History_DirPowC_byMPNo(ucMPNo, 0, 3);
	m = SetMainComm_Buf_lVal_4B(pucRetBuffer, m, stHisVal.lVal);
	(*pusPos)=m;
	return ucRet;
}
unsigned char Main_Get_ReqData_1_F32(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	STHISVAL stHisVal;

	ucMPNo = ucDAp;
	stHisVal = Get_History_DirPowALm_byMPNo(ucMPNo, 0, 0);
	if(Is_DateTime_Availble(&(stHisVal.stDateTime)) != RET_SUCCESS)
		stHisVal.stDateTime = GetCurrentTime();
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);

	m = SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,stHisVal.lVal);
	stHisVal = Get_History_DirPowALm_byMPNo(ucMPNo,0,1);
	m = SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,stHisVal.lVal);
	stHisVal = Get_History_DirPowALm_byMPNo(ucMPNo, 0, 2);
	m = SetMainComm_Buf_lVal_4B(pucRetBuffer, m, stHisVal.lVal);
	stHisVal = Get_History_DirPowALm_byMPNo(ucMPNo, 0, 3);
	m = SetMainComm_Buf_lVal_4B(pucRetBuffer, m, stHisVal.lVal);
	
	stHisVal = Get_History_DirPowBLm_byMPNo(ucMPNo, 0, 0);
	m = SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,stHisVal.lVal);
	stHisVal = Get_History_DirPowBLm_byMPNo(ucMPNo,0,1);
	m = SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,stHisVal.lVal);
	stHisVal = Get_History_DirPowBLm_byMPNo(ucMPNo, 0, 2);
	m = SetMainComm_Buf_lVal_4B(pucRetBuffer, m, stHisVal.lVal);
	stHisVal = Get_History_DirPowBLm_byMPNo(ucMPNo, 0, 3);
	m = SetMainComm_Buf_lVal_4B(pucRetBuffer, m, stHisVal.lVal);
	
	stHisVal = Get_History_DirPowCLm_byMPNo(ucMPNo, 0, 0);
	m = SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,stHisVal.lVal);
	stHisVal = Get_History_DirPowCLm_byMPNo(ucMPNo,0,1);
	m = SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,stHisVal.lVal);
	stHisVal = Get_History_DirPowCLm_byMPNo(ucMPNo, 0, 2);
	m = SetMainComm_Buf_lVal_4B(pucRetBuffer, m, stHisVal.lVal);
	stHisVal = Get_History_DirPowCLm_byMPNo(ucMPNo, 0, 3);
	m = SetMainComm_Buf_lVal_4B(pucRetBuffer, m, stHisVal.lVal);
	(*pusPos)=m;
	return ucRet;
}
//F33	��ǰ������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F33(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	
	STHISVAL	  stHisVal,stHisVal1;
	STMETERPARA		stMeterPara;

	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_DirPower_byMPNo_High(ucMPNo,0,0);
	if(Is_DateTime_Availble(&stHisVal.stDateTime)!=RET_SUCCESS)
	{
		stHisVal1 = Get_History_DirVar_byMPNo(ucMPNo,0,0);
		m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal1.stDateTime);
	}else
		m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;
	m=SetMainComm_Buf_lVal_5B_DT25( pucRetBuffer ,m, stHisVal.lVal);

	for(i=1;i<5;i++)
	{
		stHisVal = Get_History_DirPower_byMPNo_High(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_5B_DT25( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirPower_byMPNo_High(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_5B_DT25( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirPower_byMPNo_High(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_5B_DT25( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirPower_byMPNo_High(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_5B_DT25( pucRetBuffer ,m, stHisVal.lVal);*/
	
/*	stHisVal = Get_History_DirPower_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirPower_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirPower_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirPower_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);*/

	for(i=0;i<5;i++)
	{
		stHisVal = Get_History_DirVar_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirVar_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);*/

	for(i=0;i<5;i++)
	{
		stHisVal = Get_History_DirVar1_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	}
	
/*	stHisVal = Get_History_DirVar1_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar1_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar1_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar1_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar1_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);*/

	for(i=0;i<5;i++)
	{
		stHisVal = Get_History_DirVar4_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirVar4_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar4_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar4_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar4_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar4_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);*/
	
	(*pusPos)=m;
	return ucRet;
}

//F34	��ǰ������/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F34(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	
	STHISVAL	  stHisVal,stHisVal1;
	STMETERPARA		stMeterPara;
	
	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_UnDirPower_byMPNo(ucMPNo,0,0);
	if(Is_DateTime_Availble(&stHisVal.stDateTime)!=RET_SUCCESS)
	{
		stHisVal1 = Get_History_UnDirVar_byMPNo(ucMPNo,0,0);
		m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal1.stDateTime);
	}else
		m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	for(i=1;i<5;i++)
	{
		stHisVal = Get_History_UnDirPower_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_UnDirPower_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirPower_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirPower_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirPower_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);*/

	for(i=0;i<5;i++)
	{
		stHisVal = Get_History_UnDirVar_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_UnDirVar_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirVar_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirVar_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirVar_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirVar_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);*/

	for(i=0;i<5;i++)
	{
		stHisVal = Get_History_DirVar2_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirVar2_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar2_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar2_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar2_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar2_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);*/

	for(i=0;i<5;i++)
	{
		stHisVal = Get_History_DirVar3_byMPNo(ucMPNo,0,i);//azh 140919
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirVar3_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar3_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar3_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar3_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar3_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);*/
	
	(*pusPos)=m;
	return ucRet;
}

//F35	����������/�޹��������������ʱ�䣨�ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F35(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	
	STHISVAL	  stHisVal,stHisVal1;
	STHISTIME	  stHisTime;
	STMETERPARA		stMeterPara;
	
	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_DirPowMaxDemand_byMPNo(ucMPNo,0,0);
	if(Is_DateTime_Availble(&stHisVal.stDateTime)!=RET_SUCCESS)
	{
		stHisVal1 = Get_History_DirVarMaxDemand_byMPNo(ucMPNo,0,0);
		if(Is_DateTime_Availble(&stHisVal1.stDateTime)!=RET_SUCCESS)
		{
			stHisVal1 = Get_History_DirPowMaxDemand_byMPNo(ucMPNo,0,2);
		}
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal1.stDateTime);
	}else
		m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	for(i=1;i<5;i++)
	{
		stHisVal = Get_History_DirPowMaxDemand_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirPowMaxDemand_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirPowMaxDemand_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirPowMaxDemand_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirPowMaxDemand_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);*/

	for(i=0;i<5;i++)
	{
		stHisTime = Get_History_DirPowMaxDemandTime_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	}
/*	stHisTime = Get_History_DirPowMaxDemandTime_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_DirPowMaxDemandTime_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_DirPowMaxDemandTime_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_DirPowMaxDemandTime_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_DirPowMaxDemandTime_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);*/

	for(i=0;i<5;i++)
	{
		stHisVal = Get_History_DirVarMaxDemand_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirVarMaxDemand_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVarMaxDemand_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVarMaxDemand_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVarMaxDemand_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVarMaxDemand_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);*/

	for(i=0;i<5;i++)
	{
		stHisTime = Get_History_DirVarMaxDemandTime_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	}
/*	stHisTime = Get_History_DirVarMaxDemandTime_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_DirVarMaxDemandTime_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_DirVarMaxDemandTime_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_DirVarMaxDemandTime_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_DirVarMaxDemandTime_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);*/
	
	(*pusPos)=m;
	return ucRet;
}

//F36	���·�����/�޹��������������ʱ�䣨�ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F36(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	
	STHISVAL	  stHisVal,stHisVal1;
	STHISTIME	  stHisTime;
	STMETERPARA		stMeterPara;
	
	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_UnDirPowMaxDemand_byMPNo(ucMPNo,0,0);
	if(Is_DateTime_Availble(&stHisVal.stDateTime)!=RET_SUCCESS)
	{
		stHisVal1 = Get_History_UnDirVarMaxDemand_byMPNo(ucMPNo,0,0);
		if(Is_DateTime_Availble(&stHisVal1.stDateTime)!=RET_SUCCESS)
		{
			stHisVal1 = Get_History_UnDirPowMaxDemand_byMPNo(ucMPNo,0,2);
		}
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal1.stDateTime);
	}else
		m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	for(i=1;i<5;i++)
	{
		stHisVal = Get_History_UnDirPowMaxDemand_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_UnDirPowMaxDemand_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirPowMaxDemand_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirPowMaxDemand_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirPowMaxDemand_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);*/

	for(i=0;i<5;i++)
	{
		stHisTime = Get_History_UnDirPowMaxDemandTime_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	}
/*	stHisTime = Get_History_UnDirPowMaxDemandTime_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_UnDirPowMaxDemandTime_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_UnDirPowMaxDemandTime_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_UnDirPowMaxDemandTime_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_UnDirPowMaxDemandTime_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);*/

	for(i=0;i<5;i++)
	{
		stHisVal = Get_History_UnDirVarMaxDemand_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_UnDirVarMaxDemand_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirVarMaxDemand_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirVarMaxDemand_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirVarMaxDemand_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirVarMaxDemand_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);*/

	for(i=0;i<5;i++)
	{
		stHisTime = Get_History_UnDirVarMaxDemandTime_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	}
/*	stHisTime = Get_History_UnDirVarMaxDemandTime_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_UnDirVarMaxDemandTime_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_UnDirVarMaxDemandTime_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_UnDirVarMaxDemandTime_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_UnDirVarMaxDemandTime_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);*/
	
	(*pusPos)=m;
	return ucRet;
}

//F37	����������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F37(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	
	STHISVAL	  stHisVal,stHisVal1;
	STMETERPARA		stMeterPara;
	
//	if(RunPara_GetMPType(ucDAp)==CON_MPTYPE_TER)
//		return RET_ERROR;
	
	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_DirPowerLm_byMPNo(ucMPNo,0,0);
	if(Is_DateTime_Availble(&stHisVal.stDateTime)!=RET_SUCCESS)
	{
		stHisVal1 = Get_History_DirVarLm_byMPNo(ucMPNo,0,0);
		m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal1.stDateTime);
	}else
		m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	for(i=1;i<5;i++)
	{
		stHisVal = Get_History_DirPowerLm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirPowerLm_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirPowerLm_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirPowerLm_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirPowerLm_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);*/

	for(i=0;i<5;i++)
	{
		stHisVal = Get_History_DirVarLm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirVarLm_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVarLm_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVarLm_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVarLm_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVarLm_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);*/
	
	for(i=0;i<5;i++)
	{
		stHisVal = Get_History_DirVar1Lm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirVar1Lm_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar1Lm_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar1Lm_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar1Lm_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar1Lm_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);*/
	
	for(i=0;i<5;i++)
	{
		stHisVal = Get_History_DirVar4Lm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirVar4Lm_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar4Lm_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar4Lm_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar4Lm_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar4Lm_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);*/

	(*pusPos)=m;
	return ucRet;
}

//F38	���·�����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F38(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	
	STHISVAL	  stHisVal,stHisVal1;
	STMETERPARA		stMeterPara;
	
//	if(RunPara_GetMPType(ucDAp)==CON_MPTYPE_TER)
//		return RET_ERROR;
	
	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_UnDirPowerLm_byMPNo(ucMPNo,0,0);
	if(Is_DateTime_Availble(&stHisVal.stDateTime)!=RET_SUCCESS)
	{
		stHisVal1 = Get_History_UnDirVarLm_byMPNo(ucMPNo,0,0);
		m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal1.stDateTime);
	}else
		m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	for(i=1;i<5;i++)
	{
		stHisVal = Get_History_UnDirPowerLm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_UnDirPowerLm_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirPowerLm_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirPowerLm_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirPowerLm_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);*/

	for(i=0;i<5;i++)
	{
		stHisVal = Get_History_UnDirVarLm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);		
	}
	
/*	stHisVal = Get_History_UnDirVarLm_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirVarLm_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirVarLm_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirVarLm_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirVarLm_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);*/

	for(i=0;i<5;i++)
	{
		stHisVal = Get_History_DirVar2Lm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirVar2Lm_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar2Lm_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar2Lm_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar2Lm_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar2Lm_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);*/

	for(i=0;i<5;i++)
	{
		stHisVal = Get_History_DirVar3Lm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirVar3Lm_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar3Lm_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar3Lm_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar3Lm_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar3Lm_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);*/

	(*pusPos)=m;
	return ucRet;
}

//F39	����������/�޹��������������ʱ�䣨�ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F39(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	STDATETIME stCurr=GetCurrentTime();
	
	STHISVAL	  stHisVal,stHisVal1;
	STHISTIME	  stHisTime;
	STMETERPARA		stMeterPara;
	
//	if(RunPara_GetMPType(ucDAp)==CON_MPTYPE_TER)
//		return RET_ERROR;
	
	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_DirPowMaxDemandLm_byMPNo(ucMPNo,0,0);
	if(Is_DateTime_Availble(&stHisVal.stDateTime)!=RET_SUCCESS)
	{
		stHisVal1 = Get_History_DirVarMaxDemandLm_byMPNo(ucMPNo,0,0);
		m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal1.stDateTime);
	}else
		m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	for(i=1;i<5;i++)
	{
		stHisVal = Get_History_DirPowMaxDemandLm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirPowMaxDemandLm_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirPowMaxDemandLm_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirPowMaxDemandLm_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirPowMaxDemandLm_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);*/

	for(i=0;i<5;i++)
	{
		stHisTime = Get_History_DirPowMaxDemandTimeLm_byMPNo(ucMPNo,0,i);
		if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)			//����
			ClearDateTime(&(stHisTime.stHisTime));
		m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	}
/*	stHisTime = Get_History_DirPowMaxDemandTimeLm_byMPNo(ucMPNo,0,0);
	if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)			//����
		ClearDateTime(&(stHisTime.stHisTime));
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_DirPowMaxDemandTimeLm_byMPNo(ucMPNo,0,1);
	if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)
		ClearDateTime(&(stHisTime.stHisTime));
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_DirPowMaxDemandTimeLm_byMPNo(ucMPNo,0,2);
	if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)
		ClearDateTime(&(stHisTime.stHisTime));
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_DirPowMaxDemandTimeLm_byMPNo(ucMPNo,0,3);
	if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)
		ClearDateTime(&(stHisTime.stHisTime));
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_DirPowMaxDemandTimeLm_byMPNo(ucMPNo,0,4);
	if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)
		ClearDateTime(&(stHisTime.stHisTime));
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);*/

	for(i=0;i<5;i++)
	{
		stHisVal = Get_History_DirVarMaxDemandLm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirVarMaxDemandLm_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVarMaxDemandLm_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVarMaxDemandLm_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVarMaxDemandLm_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVarMaxDemandLm_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);*/

	for(i=0;i<5;i++)
	{
		stHisTime = Get_History_DirVarMaxDemandTimeLm_byMPNo(ucMPNo,0,i);
		if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)
			ClearDateTime(&(stHisTime.stHisTime));
		m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	}
/*	stHisTime = Get_History_DirVarMaxDemandTimeLm_byMPNo(ucMPNo,0,0);
	if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)
		ClearDateTime(&(stHisTime.stHisTime));
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_DirVarMaxDemandTimeLm_byMPNo(ucMPNo,0,1);
	if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)
		ClearDateTime(&(stHisTime.stHisTime));
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_DirVarMaxDemandTimeLm_byMPNo(ucMPNo,0,2);
	if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)
		ClearDateTime(&(stHisTime.stHisTime));
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_DirVarMaxDemandTimeLm_byMPNo(ucMPNo,0,3);
	if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)
		ClearDateTime(&(stHisTime.stHisTime));
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_DirVarMaxDemandTimeLm_byMPNo(ucMPNo,0,4);
	if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)
		ClearDateTime(&(stHisTime.stHisTime));
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);*/

	(*pusPos)=m;
	return ucRet;
}

//F40	���·�����/�޹��������������ʱ�䣨�ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F40(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);

	STDATETIME stCurr=GetCurrentTime();
	
	STHISVAL	  stHisVal,stHisVal1;
	STHISTIME	  stHisTime;
	STMETERPARA		stMeterPara;
	
//	if(RunPara_GetMPType(ucDAp)==CON_MPTYPE_TER)
//		return RET_ERROR;
//	
	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_UnDirPowMaxDemandLm_byMPNo(ucMPNo,0,0);
	if(Is_DateTime_Availble(&stHisVal.stDateTime)!=RET_SUCCESS)
	{
		stHisVal1 = Get_History_UnDirVarMaxDemandLm_byMPNo(ucMPNo,0,0);
		m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal1.stDateTime);
	}else
		m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	for(i=1;i<5;i++)
	{
		stHisVal = Get_History_UnDirPowMaxDemandLm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_UnDirPowMaxDemandLm_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirPowMaxDemandLm_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirPowMaxDemandLm_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirPowMaxDemandLm_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);*/

	for(i=0;i<5;i++)
	{
		stHisTime = Get_History_UnDirPowMaxDemandTimeLm_byMPNo(ucMPNo,0,i);
		if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)
			ClearDateTime(&(stHisTime.stHisTime));
		m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	}
/*	stHisTime = Get_History_UnDirPowMaxDemandTimeLm_byMPNo(ucMPNo,0,0);
	if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)
		ClearDateTime(&(stHisTime.stHisTime));
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_UnDirPowMaxDemandTimeLm_byMPNo(ucMPNo,0,1);
	if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)
		ClearDateTime(&(stHisTime.stHisTime));
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_UnDirPowMaxDemandTimeLm_byMPNo(ucMPNo,0,2);
	if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)
		ClearDateTime(&(stHisTime.stHisTime));
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_UnDirPowMaxDemandTimeLm_byMPNo(ucMPNo,0,3);
	if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)
		ClearDateTime(&(stHisTime.stHisTime));
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_UnDirPowMaxDemandTimeLm_byMPNo(ucMPNo,0,4);
	if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)
		ClearDateTime(&(stHisTime.stHisTime));
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);*/

	for(i=0;i<5;i++)
	{
		stHisVal = Get_History_UnDirVarMaxDemandLm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_UnDirVarMaxDemandLm_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirVarMaxDemandLm_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirVarMaxDemandLm_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirVarMaxDemandLm_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirVarMaxDemandLm_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);*/

	for(i=0;i<5;i++)
	{
		stHisTime = Get_History_UnDirVarMaxDemandTimeLm_byMPNo(ucMPNo,0,i);//azh 140919
		if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)
			ClearDateTime(&(stHisTime.stHisTime));
		m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	}
/*	stHisTime = Get_History_UnDirVarMaxDemandTimeLm_byMPNo(ucMPNo,0,0);
	if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)
		ClearDateTime(&(stHisTime.stHisTime));
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_UnDirVarMaxDemandTimeLm_byMPNo(ucMPNo,0,1);
	if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)
		ClearDateTime(&(stHisTime.stHisTime));
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_UnDirVarMaxDemandTimeLm_byMPNo(ucMPNo,0,2);
	if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)
		ClearDateTime(&(stHisTime.stHisTime));
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_UnDirVarMaxDemandTimeLm_byMPNo(ucMPNo,0,3);
	if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)
		ClearDateTime(&(stHisTime.stHisTime));
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	stHisTime = Get_History_UnDirVarMaxDemandTimeLm_byMPNo(ucMPNo,0,4);
	if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)
		ClearDateTime(&(stHisTime.stHisTime));
	m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);*/
	
	(*pusPos)=m;
	return ucRet;
}

//F41	���������й����������ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F41(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo=0,ucMeterNo=0,ucMPType;
	unsigned short m=(*pusPos);

	STDATETIME stStartTime=GetCurrentTime();
	unsigned char ucFlag1;
//  unsigned char ucPulseNo;
	unsigned long ulFeeT,ulFee1,ulFee2,ulFee3,ulFee4;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;
	STHISRUNDATA *pstHisRunData;
	
	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
		if(ucMeterNo>=CON_METER_NUM)
			return RET_ERROR;
		ulFeeT=CONINVALIDVAL;	ulFee1=CONINVALIDVAL;	ulFee2=CONINVALIDVAL;	
		ulFee3=CONINVALIDVAL;	ulFee4=CONINVALIDVAL;
		stStartTime.ucSecond=59;
		stStartTime.ucMinute=59;
		stStartTime.ucHour=23;
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }		
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);
		pstHisRunData=(STHISRUNDATA *)mem_ptr;
		ucFlag1=DataProc_GetData_Day(ucMeterNo,stStartTime,pstHisRunData);
		if(ucFlag1==RET_SUCCESS)
		{
			if(pstHisRunData->ulHisDirPowTotal != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowTotal != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirPowTotal > pstHisRunData->ulHisLastDayDirPowTotal)
					ulFeeT=pstHisRunData->ulHisDirPowTotal - pstHisRunData->ulHisLastDayDirPowTotal;
				else
					ulFeeT=0;
			}
			if(pstHisRunData->ulHisDirPowFee1 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowFee1 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirPowFee1 > pstHisRunData->ulHisLastDayDirPowFee1)
					ulFee1=pstHisRunData->ulHisDirPowFee1 - pstHisRunData->ulHisLastDayDirPowFee1;
				else
					ulFee1=0;
			}
			if(pstHisRunData->ulHisDirPowFee2 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowFee2 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirPowFee2 > pstHisRunData->ulHisLastDayDirPowFee2)
					ulFee2=pstHisRunData->ulHisDirPowFee2 - pstHisRunData->ulHisLastDayDirPowFee2;
				else
					ulFee2=0;
			}
			if(pstHisRunData->ulHisDirPowFee3 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowFee3 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirPowFee3 > pstHisRunData->ulHisLastDayDirPowFee3)
					ulFee3=pstHisRunData->ulHisDirPowFee3 - pstHisRunData->ulHisLastDayDirPowFee3;
				else
					ulFee3=0;
			}
			if(pstHisRunData->ulHisDirPowFee4 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowFee4 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirPowFee4 > pstHisRunData->ulHisLastDayDirPowFee4)
					ulFee4=pstHisRunData->ulHisDirPowFee4 - pstHisRunData->ulHisLastDayDirPowFee4;
				else
					ulFee4=0;
			}
			pucRetBuffer[m++]=4;		//����
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
		}else
		{
			pucRetBuffer[m++]=4;					//�̶��ķ���
			memset(pucRetBuffer+m,0xEE,20);
			m+=20;
		}
		free_mem_block(mem_ptr);//
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ulFeeT=CONINVALIDVAL;	ulFee1=CONINVALIDVAL;	ulFee2=CONINVALIDVAL;	
//		ulFee3=CONINVALIDVAL;	ulFee4=CONINVALIDVAL;
//		stStartTime.ucSecond=59;
//		stStartTime.ucMinute=59;
//		stStartTime.ucHour=23;
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);
//		ucFlag1=DataProc_GetData_Day_Pulse(ucPulseNo,stStartTime,pstHisPulseRunData);
//		stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//		if(ucFlag1==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//		{
//			if(pstHisPulseRunData->ulHisPowTotal != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowTotal != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowTotal > pstHisPulseRunData->ulHisLastDayPowTotal)
//					ulFeeT=pstHisPulseRunData->ulHisPowTotal - pstHisPulseRunData->ulHisLastDayPowTotal;
//				else
//					ulFeeT=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee1 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee1 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee1 > pstHisPulseRunData->ulHisLastDayPowFee1)
//					ulFee1=pstHisPulseRunData->ulHisPowFee1 - pstHisPulseRunData->ulHisLastDayPowFee1;
//				else
//					ulFee1=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee2 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee2 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee2 > pstHisPulseRunData->ulHisLastDayPowFee2)
//					ulFee2=pstHisPulseRunData->ulHisPowFee2 - pstHisPulseRunData->ulHisLastDayPowFee2;
//				else
//					ulFee2=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee3 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee3 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee3 > pstHisPulseRunData->ulHisLastDayPowFee3)
//					ulFee3=pstHisPulseRunData->ulHisPowFee3 - pstHisPulseRunData->ulHisLastDayPowFee3;
//				else
//					ulFee3=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee4 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee4 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee4 > pstHisPulseRunData->ulHisLastDayPowFee4)
//					ulFee4=pstHisPulseRunData->ulHisPowFee4 - pstHisPulseRunData->ulHisLastDayPowFee4;
//				else
//					ulFee4=0;
//			}
//			pucRetBuffer[m++]=4;		//����
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}else
//		{
//			pucRetBuffer[m++]=4;					//�̶��ķ���
//			memset(pucRetBuffer+m,0xEE,20);
//			m+=20;
//		}
//	}
//    else if(ucMPType==CON_MPTYPE_TER)
//	{
//		ulFeeT=CONINVALIDVAL;	ulFee1=CONINVALIDVAL;	ulFee2=CONINVALIDVAL;	
//		ulFee3=CONINVALIDVAL;	ulFee4=CONINVALIDVAL;
//		stStartTime.ucSecond=59;
//		stStartTime.ucMinute=59;
//		stStartTime.ucHour=23;
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }		
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);
//		pstHisRunData=(STHISRUNDATA *)mem_ptr;
//		ucFlag1=DataProc_GetData_Day_Ter(stStartTime,pstHisRunData);
//		if(ucFlag1==RET_SUCCESS)
//		{
//			if(pstHisRunData->ulHisDirPowTotal != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowTotal != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirPowTotal > pstHisRunData->ulHisLastDayDirPowTotal)
//					ulFeeT=pstHisRunData->ulHisDirPowTotal - pstHisRunData->ulHisLastDayDirPowTotal;
//				else
//					ulFeeT=0;
//			}
//			if(pstHisRunData->ulHisDirPowFee1 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowFee1 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirPowFee1 > pstHisRunData->ulHisLastDayDirPowFee1)
//					ulFee1=pstHisRunData->ulHisDirPowFee1 - pstHisRunData->ulHisLastDayDirPowFee1;
//				else
//					ulFee1=0;
//			}
//			if(pstHisRunData->ulHisDirPowFee2 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowFee2 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirPowFee2 > pstHisRunData->ulHisLastDayDirPowFee2)
//					ulFee2=pstHisRunData->ulHisDirPowFee2 - pstHisRunData->ulHisLastDayDirPowFee2;
//				else
//					ulFee2=0;
//			}
//			if(pstHisRunData->ulHisDirPowFee3 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowFee3 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirPowFee3 > pstHisRunData->ulHisLastDayDirPowFee3)
//					ulFee3=pstHisRunData->ulHisDirPowFee3 - pstHisRunData->ulHisLastDayDirPowFee3;
//				else
//					ulFee3=0;
//			}
//			if(pstHisRunData->ulHisDirPowFee4 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowFee4 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirPowFee4 > pstHisRunData->ulHisLastDayDirPowFee4)
//					ulFee4=pstHisRunData->ulHisDirPowFee4 - pstHisRunData->ulHisLastDayDirPowFee4;
//				else
//					ulFee4=0;
//			}
//			pucRetBuffer[m++]=4;		//����
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}else
//		{
//			pucRetBuffer[m++]=4;					//�̶��ķ���
//			memset(pucRetBuffer+m,0xEE,20);
//			m+=20;
//		}
//		free_mem_block(mem_ptr);//
//	}

	(*pusPos)=m;
	return ucRet;
}

//F42	���������޹����������ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F42(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo=0,ucMeterNo=0,ucMPType;
	unsigned short m=(*pusPos);

	STDATETIME stStartTime=GetCurrentTime();
	unsigned char ucFlag1;
//	unsigned char ucPulseNo;    
	unsigned long ulFeeT,ulFee1,ulFee2,ulFee3,ulFee4;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;
	STHISRUNDATA *pstHisRunData;
	
	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
		if(ucMeterNo>=CON_METER_NUM)
			return RET_ERROR;
		ulFeeT=CONINVALIDVAL;	ulFee1=CONINVALIDVAL;	ulFee2=CONINVALIDVAL;	
		ulFee3=CONINVALIDVAL;	ulFee4=CONINVALIDVAL;
		stStartTime.ucSecond=59;
		stStartTime.ucMinute=59;
		stStartTime.ucHour=23;
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;
    
		ucFlag1=DataProc_GetData_Day(ucMeterNo,stStartTime,pstHisRunData);
		if(ucFlag1==RET_SUCCESS)
		{
			if(pstHisRunData->ulHisDirVarTotal != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarTotal != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirVarTotal > pstHisRunData->ulHisLastDayDirVarTotal)
					ulFeeT=pstHisRunData->ulHisDirVarTotal - pstHisRunData->ulHisLastDayDirVarTotal;
				else
					ulFeeT=0;
			}
			if(pstHisRunData->ulHisDirVarFee1 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarFee1 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirVarFee1 > pstHisRunData->ulHisLastDayDirVarFee1)
					ulFee1=pstHisRunData->ulHisDirVarFee1 - pstHisRunData->ulHisLastDayDirVarFee1;
				else
					ulFee1=0;
			}
			if(pstHisRunData->ulHisDirVarFee2 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarFee2 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirVarFee2 > pstHisRunData->ulHisLastDayDirVarFee2)
					ulFee2=pstHisRunData->ulHisDirVarFee2 - pstHisRunData->ulHisLastDayDirVarFee2;
				else
					ulFee2=0;
			}
			if(pstHisRunData->ulHisDirVarFee3 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarFee3 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirVarFee3 > pstHisRunData->ulHisLastDayDirVarFee3)
					ulFee3=pstHisRunData->ulHisDirVarFee3 - pstHisRunData->ulHisLastDayDirVarFee3;
				else
					ulFee3=0;
			}
			if(pstHisRunData->ulHisDirVarFee4 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarFee4 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirVarFee4 > pstHisRunData->ulHisLastDayDirVarFee4)
					ulFee4=pstHisRunData->ulHisDirVarFee4 - pstHisRunData->ulHisLastDayDirVarFee4;
				else
					ulFee4=0;
			}
			pucRetBuffer[m++]=4;		//����
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
		}else
		{
			pucRetBuffer[m++]=4;					//�̶��ķ���
			memset(pucRetBuffer+m,0xEE,20);
			m+=20;
		}
		free_mem_block(mem_ptr);//
	}
//  else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ulFeeT=CONINVALIDVAL;	ulFee1=CONINVALIDVAL;	ulFee2=CONINVALIDVAL;	
//		ulFee3=CONINVALIDVAL;	ulFee4=CONINVALIDVAL;
//		stStartTime.ucSecond=59;
//		stStartTime.ucMinute=59;
//		stStartTime.ucHour=23;
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);
//		ucFlag1=DataProc_GetData_Day_Pulse(ucPulseNo,stStartTime,pstHisPulseRunData);
//		stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//		if(ucFlag1==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_DIRVAR)
//		{
//			if(pstHisPulseRunData->ulHisPowTotal != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowTotal != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowTotal > pstHisPulseRunData->ulHisLastDayPowTotal)
//					ulFeeT=pstHisPulseRunData->ulHisPowTotal - pstHisPulseRunData->ulHisLastDayPowTotal;
//				else
//					ulFeeT=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee1 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee1 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee1 > pstHisPulseRunData->ulHisLastDayPowFee1)
//					ulFee1=pstHisPulseRunData->ulHisPowFee1 - pstHisPulseRunData->ulHisLastDayPowFee1;
//				else
//					ulFee1=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee2 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee2 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee2 > pstHisPulseRunData->ulHisLastDayPowFee2)
//					ulFee2=pstHisPulseRunData->ulHisPowFee2 - pstHisPulseRunData->ulHisLastDayPowFee2;
//				else
//					ulFee2=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee3 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee3 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee3 > pstHisPulseRunData->ulHisLastDayPowFee3)
//					ulFee3=pstHisPulseRunData->ulHisPowFee3 - pstHisPulseRunData->ulHisLastDayPowFee3;
//				else
//					ulFee3=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee4 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee4 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee4 > pstHisPulseRunData->ulHisLastDayPowFee4)
//					ulFee4=pstHisPulseRunData->ulHisPowFee4 - pstHisPulseRunData->ulHisLastDayPowFee4;
//				else
//					ulFee4=0;
//			}
//			pucRetBuffer[m++]=4;		//����
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}else
//		{
//			pucRetBuffer[m++]=4;					//�̶��ķ���
//			memset(pucRetBuffer+m,0xEE,20);
//			m+=20;
//		}
//	}
//    else if(ucMPType==CON_MPTYPE_TER)
//	{
//		ulFeeT=CONINVALIDVAL;	ulFee1=CONINVALIDVAL;	ulFee2=CONINVALIDVAL;	
//		ulFee3=CONINVALIDVAL;	ulFee4=CONINVALIDVAL;
//		stStartTime.ucSecond=59;
//		stStartTime.ucMinute=59;
//		stStartTime.ucHour=23;
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;
//		ucFlag1=DataProc_GetData_Day_Ter(stStartTime,pstHisRunData);
//		if(ucFlag1==RET_SUCCESS)
//		{
//			if(pstHisRunData->ulHisDirVarTotal != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarTotal != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirVarTotal > pstHisRunData->ulHisLastDayDirVarTotal)
//					ulFeeT=pstHisRunData->ulHisDirVarTotal - pstHisRunData->ulHisLastDayDirVarTotal;
//				else
//					ulFeeT=0;
//			}
//			if(pstHisRunData->ulHisDirVarFee1 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarFee1 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirVarFee1 > pstHisRunData->ulHisLastDayDirVarFee1)
//					ulFee1=pstHisRunData->ulHisDirVarFee1 - pstHisRunData->ulHisLastDayDirVarFee1;
//				else
//					ulFee1=0;
//			}
//			if(pstHisRunData->ulHisDirVarFee2 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarFee2 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirVarFee2 > pstHisRunData->ulHisLastDayDirVarFee2)
//					ulFee2=pstHisRunData->ulHisDirVarFee2 - pstHisRunData->ulHisLastDayDirVarFee2;
//				else
//					ulFee2=0;
//			}
//			if(pstHisRunData->ulHisDirVarFee3 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarFee3 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirVarFee3 > pstHisRunData->ulHisLastDayDirVarFee3)
//					ulFee3=pstHisRunData->ulHisDirVarFee3 - pstHisRunData->ulHisLastDayDirVarFee3;
//				else
//					ulFee3=0;
//			}
//			if(pstHisRunData->ulHisDirVarFee4 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarFee4 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirVarFee4 > pstHisRunData->ulHisLastDayDirVarFee4)
//					ulFee4=pstHisRunData->ulHisDirVarFee4 - pstHisRunData->ulHisLastDayDirVarFee4;
//				else
//					ulFee4=0;
//			}
//			pucRetBuffer[m++]=4;		//����
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}else
//		{
//			pucRetBuffer[m++]=4;					//�̶��ķ���
//			memset(pucRetBuffer+m,0xEE,20);
//			m+=20;
//		}
//		free_mem_block(mem_ptr);//
//	}
	
	(*pusPos)=m;
	return ucRet;
}

//F43	���շ����й����������ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F43(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo=0,ucMeterNo=0,ucMPType;
	unsigned short m=(*pusPos);

	STDATETIME stStartTime=GetCurrentTime();
	unsigned char ucFlag1;
//	unsigned char ucPulseNo;    
	unsigned long ulFeeT,ulFee1,ulFee2,ulFee3,ulFee4;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;
	STHISRUNDATA *pstHisRunData;
	
	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
		if(ucMeterNo>=CON_METER_NUM)
			return RET_ERROR;
		ulFeeT=CONINVALIDVAL;	ulFee1=CONINVALIDVAL;	ulFee2=CONINVALIDVAL;	
		ulFee3=CONINVALIDVAL;	ulFee4=CONINVALIDVAL;
		stStartTime.ucSecond=59;
		stStartTime.ucMinute=59;
		stStartTime.ucHour=23;
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���
		ucFlag1=DataProc_GetData_Day(ucMeterNo,stStartTime,pstHisRunData);
		if(ucFlag1==RET_SUCCESS)
		{
			if(pstHisRunData->ulHisRevPowTotal != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowTotal != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevPowTotal > pstHisRunData->ulHisLastDayRevPowTotal)
					ulFeeT=pstHisRunData->ulHisRevPowTotal - pstHisRunData->ulHisLastDayRevPowTotal;
				else
					ulFeeT=0;
			}
			if(pstHisRunData->ulHisRevPowFee1 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowFee1 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevPowFee1 > pstHisRunData->ulHisLastDayRevPowFee1)
					ulFee1=pstHisRunData->ulHisRevPowFee1 - pstHisRunData->ulHisLastDayRevPowFee1;
				else
					ulFee1=0;
			}
			if(pstHisRunData->ulHisRevPowFee2 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowFee2 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevPowFee2 > pstHisRunData->ulHisLastDayRevPowFee2)
					ulFee2=pstHisRunData->ulHisRevPowFee2 - pstHisRunData->ulHisLastDayRevPowFee2;
				else
					ulFee2=0;
			}
			if(pstHisRunData->ulHisRevPowFee3 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowFee3 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevPowFee3 > pstHisRunData->ulHisLastDayRevPowFee3)
					ulFee3=pstHisRunData->ulHisRevPowFee3 - pstHisRunData->ulHisLastDayRevPowFee3;
				else
					ulFee3=0;
			}
			if(pstHisRunData->ulHisRevPowFee4 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowFee4 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevPowFee4 > pstHisRunData->ulHisLastDayRevPowFee4)
					ulFee4=pstHisRunData->ulHisRevPowFee4 - pstHisRunData->ulHisLastDayRevPowFee4;
				else
					ulFee4=0;
			}
			pucRetBuffer[m++]=4;		//����
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
		}else
		{
			pucRetBuffer[m++]=4;					//�̶��ķ���
			memset(pucRetBuffer+m,0xEE,20);
			m+=20;
		}
		free_mem_block(mem_ptr);//
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ulFeeT=CONINVALIDVAL;	ulFee1=CONINVALIDVAL;	ulFee2=CONINVALIDVAL;	
//		ulFee3=CONINVALIDVAL;	ulFee4=CONINVALIDVAL;
//		stStartTime.ucSecond=59;
//		stStartTime.ucMinute=59;
//		stStartTime.ucHour=23;
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);
//		ucFlag1=DataProc_GetData_Day_Pulse(ucPulseNo,stStartTime,pstHisPulseRunData);
//		stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//		if(ucFlag1==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_REVPOW)
//		{
//			if(pstHisPulseRunData->ulHisPowTotal != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowTotal != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowTotal > pstHisPulseRunData->ulHisLastDayPowTotal)
//					ulFeeT=pstHisPulseRunData->ulHisPowTotal - pstHisPulseRunData->ulHisLastDayPowTotal;
//				else
//					ulFeeT=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee1 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee1 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee1 > pstHisPulseRunData->ulHisLastDayPowFee1)
//					ulFee1=pstHisPulseRunData->ulHisPowFee1 - pstHisPulseRunData->ulHisLastDayPowFee1;
//				else
//					ulFee1=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee2 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee2 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee2 > pstHisPulseRunData->ulHisLastDayPowFee2)
//					ulFee2=pstHisPulseRunData->ulHisPowFee2 - pstHisPulseRunData->ulHisLastDayPowFee2;
//				else
//					ulFee2=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee3 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee3 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee3 > pstHisPulseRunData->ulHisLastDayPowFee3)
//					ulFee3=pstHisPulseRunData->ulHisPowFee3 - pstHisPulseRunData->ulHisLastDayPowFee3;
//				else
//					ulFee3=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee4 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee4 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee4 > pstHisPulseRunData->ulHisLastDayPowFee4)
//					ulFee4=pstHisPulseRunData->ulHisPowFee4 - pstHisPulseRunData->ulHisLastDayPowFee4;
//				else
//					ulFee4=0;
//			}
//			pucRetBuffer[m++]=4;		//����
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}else
//		{
//			pucRetBuffer[m++]=4;					//�̶��ķ���
//			memset(pucRetBuffer+m,0xEE,20);
//			m+=20;
//		}
//	}
//    else if(ucMPType==CON_MPTYPE_TER)
//	{
//		ulFeeT=CONINVALIDVAL;	ulFee1=CONINVALIDVAL;	ulFee2=CONINVALIDVAL;	
//		ulFee3=CONINVALIDVAL;	ulFee4=CONINVALIDVAL;
//		stStartTime.ucSecond=59;
//		stStartTime.ucMinute=59;
//		stStartTime.ucHour=23;
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���
//		ucFlag1=DataProc_GetData_Day_Ter(stStartTime,pstHisRunData);
//		if(ucFlag1==RET_SUCCESS)
//		{
//			if(pstHisRunData->ulHisRevPowTotal != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowTotal != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevPowTotal > pstHisRunData->ulHisLastDayRevPowTotal)
//					ulFeeT=pstHisRunData->ulHisRevPowTotal - pstHisRunData->ulHisLastDayRevPowTotal;
//				else
//					ulFeeT=0;
//			}
//			if(pstHisRunData->ulHisRevPowFee1 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowFee1 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevPowFee1 > pstHisRunData->ulHisLastDayRevPowFee1)
//					ulFee1=pstHisRunData->ulHisRevPowFee1 - pstHisRunData->ulHisLastDayRevPowFee1;
//				else
//					ulFee1=0;
//			}
//			if(pstHisRunData->ulHisRevPowFee2 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowFee2 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevPowFee2 > pstHisRunData->ulHisLastDayRevPowFee2)
//					ulFee2=pstHisRunData->ulHisRevPowFee2 - pstHisRunData->ulHisLastDayRevPowFee2;
//				else
//					ulFee2=0;
//			}
//			if(pstHisRunData->ulHisRevPowFee3 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowFee3 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevPowFee3 > pstHisRunData->ulHisLastDayRevPowFee3)
//					ulFee3=pstHisRunData->ulHisRevPowFee3 - pstHisRunData->ulHisLastDayRevPowFee3;
//				else
//					ulFee3=0;
//			}
//			if(pstHisRunData->ulHisRevPowFee4 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowFee4 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevPowFee4 > pstHisRunData->ulHisLastDayRevPowFee4)
//					ulFee4=pstHisRunData->ulHisRevPowFee4 - pstHisRunData->ulHisLastDayRevPowFee4;
//				else
//					ulFee4=0;
//			}
//			pucRetBuffer[m++]=4;		//����
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}else
//		{
//			pucRetBuffer[m++]=4;					//�̶��ķ���
//			memset(pucRetBuffer+m,0xEE,20);
//			m+=20;
//		}
//		free_mem_block(mem_ptr);//
//	}
	
	(*pusPos)=m;
	return ucRet;
}

//F44	���շ����޹����������ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F44(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo=0,ucMeterNo=0,ucMPType;
	unsigned short m=(*pusPos);

	STDATETIME stStartTime=GetCurrentTime();
	unsigned char ucFlag1;
//  unsigned char ucPulseNo;
	unsigned long ulFeeT,ulFee1,ulFee2,ulFee3,ulFee4;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;
	STHISRUNDATA *pstHisRunData;
	
	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
		if(ucMeterNo>=CON_METER_NUM)
			return RET_ERROR;
		ulFeeT=CONINVALIDVAL;	ulFee1=CONINVALIDVAL;	ulFee2=CONINVALIDVAL;	
		ulFee3=CONINVALIDVAL;	ulFee4=CONINVALIDVAL;
		stStartTime.ucSecond=59;
		stStartTime.ucMinute=59;
		stStartTime.ucHour=23;
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���
		ucFlag1=DataProc_GetData_Day(ucMeterNo,stStartTime,pstHisRunData);
		if(ucFlag1==RET_SUCCESS)
		{
			if(pstHisRunData->ulHisRevVarTotal != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarTotal != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevVarTotal > pstHisRunData->ulHisLastDayRevVarTotal)
					ulFeeT=pstHisRunData->ulHisRevVarTotal - pstHisRunData->ulHisLastDayRevVarTotal;
				else
					ulFeeT=0;
			}
			if(pstHisRunData->ulHisRevVarFee1 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarFee1 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevVarFee1 > pstHisRunData->ulHisLastDayRevVarFee1)
					ulFee1=pstHisRunData->ulHisRevVarFee1 - pstHisRunData->ulHisLastDayRevVarFee1;
				else
					ulFee1=0;
			}
			if(pstHisRunData->ulHisRevVarFee2 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarFee2 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevVarFee2 > pstHisRunData->ulHisLastDayRevVarFee2)
					ulFee2=pstHisRunData->ulHisRevVarFee2 - pstHisRunData->ulHisLastDayRevVarFee2;
				else
					ulFee2=0;
			}
			if(pstHisRunData->ulHisRevVarFee3 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarFee3 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevVarFee3 > pstHisRunData->ulHisLastDayRevVarFee3)
					ulFee3=pstHisRunData->ulHisRevVarFee3 - pstHisRunData->ulHisLastDayRevVarFee3;
				else
					ulFee3=0;
			}
			if(pstHisRunData->ulHisRevVarFee4 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarFee4 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevVarFee4 > pstHisRunData->ulHisLastDayRevVarFee4)
					ulFee4=pstHisRunData->ulHisRevVarFee4 - pstHisRunData->ulHisLastDayRevVarFee4;
				else
					ulFee4=0;
			}
			pucRetBuffer[m++]=4;		//����
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
		}else
		{
			pucRetBuffer[m++]=4;					//�̶��ķ���
			memset(pucRetBuffer+m,0xEE,20);
			m+=20;
		}
		free_mem_block(mem_ptr);//
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ulFeeT=CONINVALIDVAL;	ulFee1=CONINVALIDVAL;	ulFee2=CONINVALIDVAL;	
//		ulFee3=CONINVALIDVAL;	ulFee4=CONINVALIDVAL;
//		stStartTime.ucSecond=59;
//		stStartTime.ucMinute=59;
//		stStartTime.ucHour=23;
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVVAR);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);
//		ucFlag1=DataProc_GetData_Day_Pulse(ucPulseNo,stStartTime,pstHisPulseRunData);
//		stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//		if(ucFlag1==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_REVVAR)
//		{
//			if(pstHisPulseRunData->ulHisPowTotal != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowTotal != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowTotal > pstHisPulseRunData->ulHisLastDayPowTotal)
//					ulFeeT=pstHisPulseRunData->ulHisPowTotal - pstHisPulseRunData->ulHisLastDayPowTotal;
//				else
//					ulFeeT=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee1 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee1 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee1 > pstHisPulseRunData->ulHisLastDayPowFee1)
//					ulFee1=pstHisPulseRunData->ulHisPowFee1 - pstHisPulseRunData->ulHisLastDayPowFee1;
//				else
//					ulFee1=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee2 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee2 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee2 > pstHisPulseRunData->ulHisLastDayPowFee2)
//					ulFee2=pstHisPulseRunData->ulHisPowFee2 - pstHisPulseRunData->ulHisLastDayPowFee2;
//				else
//					ulFee2=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee3 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee3 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee3 > pstHisPulseRunData->ulHisLastDayPowFee3)
//					ulFee3=pstHisPulseRunData->ulHisPowFee3 - pstHisPulseRunData->ulHisLastDayPowFee3;
//				else
//					ulFee3=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee4 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee4 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee4 > pstHisPulseRunData->ulHisLastDayPowFee4)
//					ulFee4=pstHisPulseRunData->ulHisPowFee4 - pstHisPulseRunData->ulHisLastDayPowFee4;
//				else
//					ulFee4=0;
//			}
//			pucRetBuffer[m++]=4;		//����
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}else
//		{
//			pucRetBuffer[m++]=4;					//�̶��ķ���
//			memset(pucRetBuffer+m,0xEE,20);
//			m+=20;
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
//		ulFeeT=CONINVALIDVAL;	ulFee1=CONINVALIDVAL;	ulFee2=CONINVALIDVAL;	
//		ulFee3=CONINVALIDVAL;	ulFee4=CONINVALIDVAL;
//		stStartTime.ucSecond=59;
//		stStartTime.ucMinute=59;
//		stStartTime.ucHour=23;
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���
//		ucFlag1=DataProc_GetData_Day_Ter(stStartTime,pstHisRunData);
//		if(ucFlag1==RET_SUCCESS)
//		{
//			if(pstHisRunData->ulHisRevVarTotal != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarTotal != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevVarTotal > pstHisRunData->ulHisLastDayRevVarTotal)
//					ulFeeT=pstHisRunData->ulHisRevVarTotal - pstHisRunData->ulHisLastDayRevVarTotal;
//				else
//					ulFeeT=0;
//			}
//			if(pstHisRunData->ulHisRevVarFee1 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarFee1 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevVarFee1 > pstHisRunData->ulHisLastDayRevVarFee1)
//					ulFee1=pstHisRunData->ulHisRevVarFee1 - pstHisRunData->ulHisLastDayRevVarFee1;
//				else
//					ulFee1=0;
//			}
//			if(pstHisRunData->ulHisRevVarFee2 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarFee2 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevVarFee2 > pstHisRunData->ulHisLastDayRevVarFee2)
//					ulFee2=pstHisRunData->ulHisRevVarFee2 - pstHisRunData->ulHisLastDayRevVarFee2;
//				else
//					ulFee2=0;
//			}
//			if(pstHisRunData->ulHisRevVarFee3 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarFee3 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevVarFee3 > pstHisRunData->ulHisLastDayRevVarFee3)
//					ulFee3=pstHisRunData->ulHisRevVarFee3 - pstHisRunData->ulHisLastDayRevVarFee3;
//				else
//					ulFee3=0;
//			}
//			if(pstHisRunData->ulHisRevVarFee4 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarFee4 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevVarFee4 > pstHisRunData->ulHisLastDayRevVarFee4)
//					ulFee4=pstHisRunData->ulHisRevVarFee4 - pstHisRunData->ulHisLastDayRevVarFee4;
//				else
//					ulFee4=0;
//			}
//			pucRetBuffer[m++]=4;		//����
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}else
//		{
//			pucRetBuffer[m++]=4;					//�̶��ķ���
//			memset(pucRetBuffer+m,0xEE,20);
//			m+=20;
//		}
//		free_mem_block(mem_ptr);//
//	}
	
	(*pusPos)=m;
	return ucRet;
}

//F45	���������й����������ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F45(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	unsigned short usLen=0;
	unsigned char ucBuff[24];
	STDATETIME stTempTime;
	memset(ucBuff,0x00,24);
	stTempTime=GetCurrentTime();
	ucRet=Main_Get_ReqData_2_F21(ucDAp,ucBuff,&usLen,&stTempTime);			//F21   �¶��������й����������ܡ�����1~M��
	usLen -= 2;								//��ȥʱ�����ݱ�ʶ�ĳ���
	memcpy(pucRetBuffer+m,ucBuff+2,usLen);
	(*pusPos)=m+usLen;				
	return ucRet;
}

//F46	���������޹����������ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F46(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	unsigned short usLen=0;
	unsigned char ucBuff[24];
	STDATETIME stTempTime;
	memset(ucBuff,0x00,24);
	stTempTime=GetCurrentTime();
	ucRet=Main_Get_ReqData_2_F22(ucDAp,ucBuff,&usLen,&stTempTime);			//F22	�¶��������޹����������ܡ�����1~M��
	usLen -= 2;								//��ȥʱ�����ݱ�ʶ�ĳ���
	memcpy(pucRetBuffer+m,ucBuff+2,usLen);
	(*pusPos)=m+usLen;				
	return ucRet;
}

//F47	���·����й����������ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F47(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	unsigned short usLen=0;
	unsigned char ucBuff[24];
	STDATETIME stTempTime;
	memset(ucBuff,0x00,24);
	stTempTime=GetCurrentTime();
	ucRet=Main_Get_ReqData_2_F23(ucDAp,ucBuff,&usLen,&stTempTime);			//F23	�¶��ᷴ���й����������ܡ�����1~M��
	usLen -= 2;								//��ȥʱ�����ݱ�ʶ�ĳ���
	memcpy(pucRetBuffer+m,ucBuff+2,usLen);
	(*pusPos)=m+usLen;				
	return ucRet;
}

//F48	���·����޹����������ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F48(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	unsigned short usLen=0;
	unsigned char ucBuff[24];
	STDATETIME stTempTime;
	memset(ucBuff,0x00,24);
	stTempTime=GetCurrentTime();
	ucRet=Main_Get_ReqData_2_F24(ucDAp,ucBuff,&usLen,&stTempTime);			//F24	�¶��ᷴ���޹����������ܡ�����1~M��	
	usLen -= 2;								//��ȥʱ�����ݱ�ʶ�ĳ���
	memcpy(pucRetBuffer+m,ucBuff+2,usLen);
	(*pusPos)=m+usLen;				
	return ucRet;
}

//F49	��ǰ��ѹ��������λ��
unsigned char Main_Get_ReqData_1_F49(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	unsigned char i;
	
	STHISVAL	  stHisVal;
//	STMETERPARA		stMeterPara;

	ucMPNo = ucDAp;
//	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
//	stHisVal = Get_History_V_Phase_byMPNo(ucMPNo,0,1);
//	if(Is_DateTime_Availble(&stHisVal.stDateTime)!=RET_SUCCESS)
//	{
//		stHisVal1 = Get_History_I_Phase_byMPNo(ucMPNo,0,1);
//		m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal1.stDateTime);
//	}else
//		m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	for(i=1;i<4;i++)
	{
		stHisVal = Get_History_V_Phase_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_2B_Sign(pucRetBuffer,m,stHisVal.lVal,stHisVal.ucSign);
	}
	for(i=1;i<4;i++)
	{
		stHisVal = Get_History_I_Phase_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_2B_Sign(pucRetBuffer,m,stHisVal.lVal,stHisVal.ucSign);
	}
	(*pusPos)=m;	
	return ucRet;
}

////F57	��ǰA��B��C�����ѹ������2~N��г����Чֵ
//unsigned char Main_Get_ReqData_1_F57(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	ucRet=RET_ERROR;
//	return ucRet;
//}
//
////F58	��ǰA��B��C�����ѹ������2~N��г��������
//unsigned char Main_Get_ReqData_1_F58(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	ucRet=RET_ERROR;
//	return ucRet;
//}
//
////F65	��ǰ������Ͷ��״̬
//unsigned char Main_Get_ReqData_1_F65(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	ucRet=RET_ERROR;
//	return ucRet;
//}
//
////F66	��ǰ�������ۼƲ���Ͷ��ʱ��ʹ���
//unsigned char Main_Get_ReqData_1_F66(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	ucRet=RET_ERROR;
//	return ucRet;
//}
//
////F67	���ա����µ������ۼƲ������޹�������
//unsigned char Main_Get_ReqData_1_F67(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	ucRet=RET_ERROR;
//	return ucRet;
//}
//
////F73	ֱ��ģ����ʵʱ����
//unsigned char Main_Get_ReqData_1_F73(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	ucRet=RET_ERROR;
//	return ucRet;
//}
//
////F81	Сʱ�����ܼ��й�����
//unsigned char Main_Get_ReqData_1_F81(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned short m=(*pusPos);
//	unsigned char i;
//	unsigned char ucPointNum;			//����
//	unsigned char ucM=0;				//�ܶ�
//	unsigned char ucDataLen=0;
//	unsigned short usOnePointLen;
//	STDATETIME stStartTime;
//	unsigned char *pucBuffer;
//	unsigned char ucGroupNo,ucTaskNo;
//	unsigned char ucTmp[4];
//	unsigned short usDT2=0;
//	float fVal=0;
//	unsigned short usDI;
//	
//	ucGroupNo = ucDAp;
//	if(ucGroupNo>=CON_SUMGROUP_NUM)
//		return RET_ERROR;
//	usDI=GetInstantPowDI_byGroupNo(ucGroupNo);
//	ucTaskNo=CON_GRTASK_SUMGROUP_INSTANTPOW_TASKNO;
//
//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;			//����������
//
//	ucM=GetFreeseMByGroupNo(ucGroupNo);
//	if((ucM>4) || (ucM==0))
//		return RET_ERROR;
//	ucM=1<<(ucM-1);							//�ܶȴ���ת��Ϊ�ܶ���ֵ
//	ucPointNum=4/ucM;
//	stStartTime=GetLastOneHourStartTime((unsigned char)(15*ucM));
//	if(gucJob1HoursFlag>0 && gucJob1HoursFlag<8)
//		SubTime(&stStartTime,gucJob1HoursFlag,CON_TIMEUNIT_HOUR);
//	
//	pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//��һ�ֽڶ����ܶ�BCD��	����ʱ�䣨Сʱ��BCD��ʮλ	����ʱ�䣨Сʱ��BCD���λ
//	pucRetBuffer[m+1]=ucM;
//	memset(pucRetBuffer+m+2,0xEE,2*ucPointNum);		//���ݸ�ʽ02Ϊ2�ֽ�
//	(*pusPos)=m+2+2*ucPointNum;
//	Task_QueryRetVal(ucTaskNo,pucBuffer,&stStartTime,(unsigned short)ucPointNum,ucM,&usOnePointLen);
//	if(((pucBuffer[0]+0x100*pucBuffer[1])!=0) && (usOnePointLen!=0))
//	{
//		pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//��һ�ֽڶ����ܶ�BCD��	����ʱ�䣨Сʱ��BCD��ʮλ	����ʱ�䣨Сʱ��BCD���λ
//		pucRetBuffer[m+1]=ucM;
//		m+=2;
//		pucBuffer+=2;
//		for(i=0;i<ucPointNum;i++)
//		{
//			memset(ucTmp,0,4);
//			ucDataLen=Get_TaskData(ucTmp,pucBuffer,ucTaskNo,usDI);
//			if(ucTmp[0]!=CON_GRTASK_DATA_INVALID)
//			{
//				memcpy(&fVal,ucTmp,4);
//				fVal/=10000;					//�й�0.0001kW --�� kW
//				usDT2=Float_To_DT2(fVal);
//				pucRetBuffer[m]=(unsigned char)usDT2;
//				pucRetBuffer[m+1]=(unsigned char)(usDT2>>8);
//			}
//			m+=2;
//			pucBuffer+=usOnePointLen;
//		}
//		(*pusPos)=m;
//	}
//	return ucRet;
//}
//
////F82	Сʱ�����ܼ��޹�����
//unsigned char Main_Get_ReqData_1_F82(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned short m=(*pusPos);
//	unsigned char i;
//	unsigned char ucPointNum;			//����
//	unsigned char ucM=0;				//�ܶ�
//	unsigned char ucDataLen=0;
//	unsigned short usOnePointLen;
//	STDATETIME stStartTime;
//	unsigned char *pucBuffer;
//	unsigned char ucGroupNo,ucTaskNo;
//	unsigned char ucTmp[4];
//	unsigned short usDT2=0;
//	float fVal=0;
//	unsigned short usDI;
//	
//	ucGroupNo = ucDAp;
//	if(ucGroupNo>=CON_SUMGROUP_NUM)
//		return RET_ERROR;
//	usDI=GetInstantVarDI_byGroupNo(ucGroupNo);
//	ucTaskNo=CON_GRTASK_SUMGROUP_INSTANTVAR_TASKNO;
//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;			//����������
//	ucM=GetFreeseMByGroupNo(ucGroupNo);
//	if((ucM>4) || (ucM==0))
//		return RET_ERROR;
//	ucM=1<<(ucM-1);							//�ܶȴ���ת��Ϊ�ܶ���ֵ
//	ucPointNum=4/ucM;
//	stStartTime=GetLastOneHourStartTime((unsigned char)(15*ucM));
//	if(gucJob1HoursFlag>0 && gucJob1HoursFlag<8)
//		SubTime(&stStartTime,gucJob1HoursFlag,CON_TIMEUNIT_HOUR);
//	
//	pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//��һ�ֽڶ����ܶ�BCD��	����ʱ�䣨Сʱ��BCD��ʮλ	����ʱ�䣨Сʱ��BCD���λ
//	pucRetBuffer[m+1]=ucM;
//	memset(pucRetBuffer+m+2,0xEE,2*ucPointNum);		//���ݸ�ʽ02Ϊ2�ֽ�
//	(*pusPos)=m+2+2*ucPointNum;
//	Task_QueryRetVal(ucTaskNo,pucBuffer,&stStartTime,(unsigned short)ucPointNum,ucM,&usOnePointLen);
//	if(((pucBuffer[0]+0x100*pucBuffer[1])!=0) && (usOnePointLen!=0))
//	{
//		pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//��һ�ֽڶ����ܶ�BCD��	����ʱ�䣨Сʱ��BCD��ʮλ	����ʱ�䣨Сʱ��BCD���λ
//		pucRetBuffer[m+1]=ucM;
//		m+=2;
//		pucBuffer+=2;
//		for(i=0;i<ucPointNum;i++)
//		{
//			memset(ucTmp,0,4);
//			ucDataLen=Get_TaskData(ucTmp,pucBuffer,ucTaskNo,usDI);
//			if(ucTmp[0]!=CON_GRTASK_DATA_INVALID)
//			{
//				memcpy(&fVal,ucTmp,4);
//				fVal/=100;						//�޹�0.01kvar --�� kvar
//				usDT2=Float_To_DT2(fVal);
//				pucRetBuffer[m]=(unsigned char)usDT2;
//				pucRetBuffer[m+1]=(unsigned char)(usDT2>>8);
//			}
//			m+=2;
//			pucBuffer+=usOnePointLen;
//		}
//		(*pusPos)=m;
//	}
//	return ucRet;
//}
//
////F83	Сʱ�����ܼ��й��ܵ�����
//unsigned char Main_Get_ReqData_1_F83(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned short m=(*pusPos);
//	unsigned char i;
//	unsigned char ucPointNum,ucPointNumTmp;			//����
//	unsigned char ucM=0;				//�ܶ�
//	unsigned char ucDataLen=0;
//	unsigned short usOnePointLen;
//	STDATETIME stStartTime;
//	unsigned char *pucBuffer;
//	unsigned char ucGroupNo,ucTaskNo;
//	unsigned char ucTmp[4];
//	unsigned short usDT2=0;
//	unsigned long ulTmp=0;
//	unsigned short usDI;
//	float fValBase,fValThis;
//	
//	ucGroupNo = ucDAp;
//	if(ucGroupNo>=CON_SUMGROUP_NUM)
//		return RET_ERROR;
//	usDI=GetPowDI_byGroupNo(ucGroupNo);
//	ucTaskNo=CON_GRTASK_SUMGROUP_POW_TASKNO;
//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;			//����������
//	ucM=GetFreeseMByGroupNo(ucGroupNo);
//	if((ucM>4) || (ucM==0))
//		return RET_ERROR;
//	ucM=1<<(ucM-1);							//�ܶȴ���ת��Ϊ�ܶ���ֵ
//	ucPointNum=4/ucM;						
//	ucPointNumTmp=ucPointNum+1;				//��һ����
//	stStartTime=GetLastOneHourStartTime_Diff(0);
//	if(gucJob1HoursFlag>0 && gucJob1HoursFlag<8)
//		SubTime(&stStartTime,gucJob1HoursFlag,CON_TIMEUNIT_HOUR);
//	pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//��һ�ֽڶ����ܶ�BCD��	����ʱ�䣨Сʱ��BCD��ʮλ	����ʱ�䣨Сʱ��BCD���λ
//	pucRetBuffer[m+1]=ucM;
//	memset(pucRetBuffer+m+2,0xEE,4*ucPointNum);		//���ݸ�ʽ03Ϊ4�ֽ�
//	(*pusPos)=m+4*ucPointNum+2;
//	Task_QueryRetVal(ucTaskNo,pucBuffer,&stStartTime,(unsigned short)ucPointNumTmp,ucM,&usOnePointLen);
//	if(((pucBuffer[0]+0x100*pucBuffer[1])!=0) && (usOnePointLen!=0))
//	{
//		pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//��һ�ֽڶ����ܶ�BCD��	����ʱ�䣨Сʱ��BCD��ʮλ	����ʱ�䣨Сʱ��BCD���λ
//		pucRetBuffer[m+1]=ucM;
//		m+=2;
//		pucBuffer+=2;
//		memset(ucTmp,0,4);
//		ucDataLen=Get_TaskData(ucTmp,pucBuffer,ucTaskNo,usDI);
//		if(ucTmp[0]==CON_GRTASK_DATA_INVALID)
//			fValBase=CONINVALIDVAL;
//		else
//			memcpy(&fValBase,ucTmp,4);
//		pucBuffer+=usOnePointLen;
//		for(i=0;i<ucPointNum;i++)
//		{
//			memset(ucTmp,0,4);
//			ucDataLen=Get_TaskData(ucTmp,pucBuffer,ucTaskNo,usDI);
//			if(ucTmp[0]==CON_GRTASK_DATA_INVALID)
//				fValThis=CONINVALIDVAL;
//			else
//				memcpy(&fValThis,ucTmp,4);
//			if(fValThis!=CONINVALIDVAL && fValBase!=CONINVALIDVAL && ucTmp[0]!=CON_GRTASK_DATA_INVALID)
//			{
//				if(fValThis>fValBase)
//					ulTmp=Float_To_DT3_Div100(fValThis-fValBase);
//				else
//				{
//					fValThis=fValBase;
//					ulTmp=0;
//				}
//				memcpy(pucRetBuffer+m,&ulTmp,4);	
//			}else
//			{
//				memset(pucRetBuffer+m,0xEE,4);	
//			}
//			fValBase=fValThis;
//			m+=4;
//			pucBuffer+=usOnePointLen;
//		}
//		(*pusPos)=m;
//	}
//	return ucRet;
//}
//
////F84	Сʱ�����ܼ��޹��ܵ�����
//unsigned char Main_Get_ReqData_1_F84(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned short m=(*pusPos);
//	unsigned char i;
//	unsigned char ucPointNum,ucPointNumTmp;			//����
//	unsigned char ucM=0;				//�ܶ�
//	unsigned char ucDataLen=0;
//	unsigned short usOnePointLen;
//	STDATETIME stStartTime;
//	unsigned char *pucBuffer;
//	unsigned char ucGroupNo,ucTaskNo;
//	unsigned char ucTmp[4];
//	unsigned short usDT2=0;
//	unsigned long ulTmp=0;
//	unsigned short usDI;
//	float fValBase,fValThis;
//	
//	ucGroupNo = ucDAp;
//	if(ucGroupNo>=CON_SUMGROUP_NUM)
//		return RET_ERROR;
//	usDI=GetVarDI_byGroupNo(ucGroupNo);
//	ucTaskNo=CON_GRTASK_SUMGROUP_VAR_TASKNO;
//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;			//����������
//	ucM=GetFreeseMByGroupNo(ucGroupNo);
//	if((ucM>4) || (ucM==0))
//		return RET_ERROR;
//	ucM=1<<(ucM-1);							//�ܶȴ���ת��Ϊ�ܶ���ֵ
//	ucPointNum=4/ucM;						
//	ucPointNumTmp=ucPointNum+1;				//��һ����
//	stStartTime=GetLastOneHourStartTime_Diff(0);
//	if(gucJob1HoursFlag>0 && gucJob1HoursFlag<8)
//		SubTime(&stStartTime,gucJob1HoursFlag,CON_TIMEUNIT_HOUR);
//	pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//��һ�ֽڶ����ܶ�BCD��	����ʱ�䣨Сʱ��BCD��ʮλ	����ʱ�䣨Сʱ��BCD���λ
//	pucRetBuffer[m+1]=ucM;
//	memset(pucRetBuffer+m+2,0xEE,4*ucPointNum);		//���ݸ�ʽ03Ϊ4�ֽ�
//	(*pusPos)=m+4*ucPointNum+2;
//	Task_QueryRetVal(ucTaskNo,pucBuffer,&stStartTime,(unsigned short)ucPointNumTmp,ucM,&usOnePointLen);
//	if(((pucBuffer[0]+0x100*pucBuffer[1])!=0) && (usOnePointLen!=0))
//	{
//		pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//��һ�ֽڶ����ܶ�BCD��	����ʱ�䣨Сʱ��BCD��ʮλ	����ʱ�䣨Сʱ��BCD���λ
//		pucRetBuffer[m+1]=ucM;
//		m+=2;
//		pucBuffer+=2;
//		memset(ucTmp,0,4);
//		ucDataLen=Get_TaskData(ucTmp,pucBuffer,ucTaskNo,usDI);
//		if(ucTmp[0]==CON_GRTASK_DATA_INVALID)
//			fValBase=CONINVALIDVAL;
//		else
//			memcpy(&fValBase,ucTmp,4);
//		pucBuffer+=usOnePointLen;
//		for(i=0;i<ucPointNum;i++)
//		{
//			memset(ucTmp,0,4);
//			ucDataLen=Get_TaskData(ucTmp,pucBuffer,ucTaskNo,usDI);
//			if(ucTmp[0]==CON_GRTASK_DATA_INVALID)
//				fValThis=CONINVALIDVAL;
//			else
//				memcpy(&fValThis,ucTmp,4);
//			if(fValThis!=CONINVALIDVAL && fValBase!=CONINVALIDVAL && ucTmp[0]!=CON_GRTASK_DATA_INVALID)
//			{
//				if(fValThis>fValBase)
//					ulTmp=Float_To_DT3_Div100(fValThis-fValBase);
//				else
//				{
//					fValThis=fValBase;
//					ulTmp=0;
//				}
//				memcpy(pucRetBuffer+m,&ulTmp,4);	
//			}else
//			{
//				memset(pucRetBuffer+m,0xEE,4);	
//			}
//			fValBase=fValThis;
//			m+=4;
//			pucBuffer+=usOnePointLen;
//		}
//		(*pusPos)=m;
//	}
//	return ucRet;
//}

//F89	Сʱ�����й�����
unsigned char Main_Get_ReqData_1_F89(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_InstantPow(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPINSTANTPOW);
	return ucRet;
}

//F90	Сʱ����A���й�����
unsigned char Main_Get_ReqData_1_F90(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_InstantPow(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPINSTANTPOWA);
	return ucRet;
}

//F91	Сʱ����B���й�����
unsigned char Main_Get_ReqData_1_F91(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_InstantPow(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPINSTANTPOWB);
	return ucRet;
}

//F92	Сʱ����C���й�����
unsigned char Main_Get_ReqData_1_F92(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_InstantPow(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPINSTANTPOWC);
	return ucRet;
}

//F93	Сʱ�����޹�����
unsigned char Main_Get_ReqData_1_F93(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_InstantPow(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPINSTANTVAR);
	return ucRet;
}

//F94	Сʱ����A���޹�����
unsigned char Main_Get_ReqData_1_F94(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_InstantPow(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPINSTANTVARA);
	return ucRet;
}

//F95	Сʱ����B���޹�����
unsigned char Main_Get_ReqData_1_F95(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_InstantPow(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPINSTANTVARB);
	return ucRet;
}

//F96	Сʱ����C���޹�����
unsigned char Main_Get_ReqData_1_F96(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_InstantPow(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPINSTANTVARC);
	return ucRet;
}

//F97	Сʱ����A���ѹ
unsigned char Main_Get_ReqData_1_F97(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_Volt(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPVOLA);
	return ucRet;
}

//F98	Сʱ����B���ѹ
unsigned char Main_Get_ReqData_1_F98(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_Volt(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPVOLB);
	return ucRet;
}

//F99	Сʱ����C���ѹ
unsigned char Main_Get_ReqData_1_F99(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_Volt(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPVOLC);
	return ucRet;
}

//F100	Сʱ����A�����
unsigned char Main_Get_ReqData_1_F100(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_Curr(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPCURA);
	return ucRet;
}

//F101	Сʱ����B�����
unsigned char Main_Get_ReqData_1_F101(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_Curr(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPCURB);
	return ucRet;
}

//F102	Сʱ����C�����
unsigned char Main_Get_ReqData_1_F102(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_Curr(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPCURC);
	return ucRet;
}

//F103	Сʱ�����������
unsigned char Main_Get_ReqData_1_F103(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=RET_ERROR;
	return ucRet;
}

//F105	Сʱ���������й��ܵ�����
unsigned char Main_Get_ReqData_1_F105(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_PowVal(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPDIRPOWTOTAL);
	return ucRet;
}

//F106	Сʱ���������޹��ܵ�����
unsigned char Main_Get_ReqData_1_F106(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_PowVal(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPDIRVARTOTAL);
	return ucRet;
}

//F107	Сʱ���ᷴ���й��ܵ�����
unsigned char Main_Get_ReqData_1_F107(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_PowVal(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPUNDIRPOWTOTAL);
	return ucRet;
}

//F108	Сʱ���ᷴ���޹��ܵ�����
unsigned char Main_Get_ReqData_1_F108(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_PowVal(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPUNDIRVARTOTAL);
	return ucRet;
}

//F109	Сʱ���������й��ܵ���ʾֵ
unsigned char Main_Get_ReqData_1_F109(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_PowViewData(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPDIRPOWTOTAL);
	return ucRet;
}

//F110	Сʱ���������޹��ܵ���ʾֵ
unsigned char Main_Get_ReqData_1_F110(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_PowViewData(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPDIRVARTOTAL);
	return ucRet;
}

//F111	Сʱ���ᷴ���й��ܵ���ʾֵ
unsigned char Main_Get_ReqData_1_F111(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_PowViewData(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPUNDIRPOWTOTAL);
	return ucRet;
}

//F112	Сʱ���ᷴ���޹��ܵ���ʾֵ
unsigned char Main_Get_ReqData_1_F112(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_PowViewData(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPUNDIRVARTOTAL);
	return ucRet;
}

//F113	Сʱ�����ܹ�������
unsigned char Main_Get_ReqData_1_F113(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_PowRate(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPPOWRATE);
	return ucRet;
}

//F114	Сʱ����A�๦������
unsigned char Main_Get_ReqData_1_F114(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_PowRate(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPPOWRATEA);
	return ucRet;
}

//F115	Сʱ����B�๦������
unsigned char Main_Get_ReqData_1_F115(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_PowRate(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPPOWRATEB);
	return ucRet;
}

//F116	Сʱ����C�๦������
unsigned char Main_Get_ReqData_1_F116(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_PowRate(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPPOWRATEC);
	return ucRet;
}

//F121	Сʱ����ֱ��ģ����
//unsigned char Main_Get_ReqData_1_F121(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	ucRet=RET_ERROR;
//	return ucRet;
//}


//F129	��ǰ�����й�����ʾֵ�����ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F129(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	
	STHISVAL	  stHisVal;
	STMETERPARA		stMeterPara;

	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_DirPower_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	for(i=1;i<5;i++)
	{
		stHisVal = Get_History_DirPower_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirPower_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirPower_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirPower_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirPower_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);*/

	(*pusPos)=m;
	return ucRet;
}

//F130	��ǰ�����޹�����ʾֵ���ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F130(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	
	STHISVAL	  stHisVal;
	STMETERPARA		stMeterPara;

	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_DirVar_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	for(i=1;i<5;i++)
	{
		stHisVal = Get_History_DirVar_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirVar_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);*/
		
	(*pusPos)=m;
	return ucRet;
}


//F131	��ǰ�����й�����ʾֵ���ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F131(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	
	STHISVAL	  stHisVal;
	STMETERPARA		stMeterPara;

	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_UnDirPower_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	for(i=1;i<5;i++)
	{
		stHisVal = Get_History_UnDirPower_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_UnDirPower_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirPower_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirPower_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirPower_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);*/

	(*pusPos)=m;
	return ucRet;
}


//F132	��ǰ�����޹�����ʾֵ���ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F132(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	
	STHISVAL	  stHisVal;
	STMETERPARA		stMeterPara;

	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_UnDirVar_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	for(i=1;i<5;i++)
	{
		stHisVal = Get_History_UnDirVar_byMPNo(ucMPNo,0,i);//Get_History_UnDirPower_byMPNo(ucMPNo,0,i);//140912 azh
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_UnDirPower_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirPower_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirPower_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirPower_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);*/

	(*pusPos)=m;
	return ucRet;
}


//F133	��ǰһ�����޹�����ʾֵ���ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F133(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	
	STHISVAL	  stHisVal;
	STMETERPARA		stMeterPara;

	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_DirVar1_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	for(i=1;i<5;i++)
	{
		stHisVal = Get_History_DirVar1_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirVar1_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar1_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar1_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar1_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);*/

	(*pusPos)=m;
	return ucRet;
}

//F134	��ǰ�������޹�����ʾֵ���ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F134(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	
	STHISVAL	  stHisVal;
	STMETERPARA		stMeterPara;

	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_DirVar2_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	for(i=1;i<5;i++)
	{
		stHisVal = Get_History_DirVar2_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirVar2_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar2_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar2_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar2_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);*/

	(*pusPos)=m;
	return ucRet;
}

//F135	��ǰ�������޹�����ʾֵ���ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F135(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	
	STHISVAL	  stHisVal;
	STMETERPARA		stMeterPara;

	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_DirVar3_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	for(i=1;i<5;i++)
	{
		stHisVal = Get_History_DirVar3_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirVar3_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar3_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar3_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar3_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);*/

	(*pusPos)=m;
	return ucRet;
}

//F136	��ǰ�������޹�����ʾֵ���ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F136(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	
	STHISVAL	  stHisVal;
	STMETERPARA		stMeterPara;

	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_DirVar4_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	for(i=1;i<5;i++)
	{
		stHisVal = Get_History_DirVar4_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirVar4_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar4_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar4_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar4_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);*/

	(*pusPos)=m;
	return ucRet;
}


//F137	���������й�����ʾֵ���ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F137(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	
	STHISVAL	  stHisVal;
	STMETERPARA		stMeterPara;
	
//	if(RunPara_GetMPType(ucDAp)==CON_MPTYPE_TER)
//		return RET_ERROR;
	
	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_DirPowerLm_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	for(i=1;i<5;i++)
	{
		stHisVal = Get_History_DirPowerLm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirPowerLm_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirPowerLm_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirPowerLm_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirPowerLm_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);*/
	
	(*pusPos)=m;
	return ucRet;
}

//F138	���������޹�����ʾֵ���ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F138(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	
	STHISVAL	  stHisVal;
	STMETERPARA		stMeterPara;
	
//	if(RunPara_GetMPType(ucDAp)==CON_MPTYPE_TER)
//		return RET_ERROR;
	
	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_DirVarLm_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	for(i=1;i<5;i++)
	{
		stHisVal = Get_History_DirVarLm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirVarLm_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVarLm_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVarLm_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVarLm_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);*/
	
	(*pusPos)=m;
	return ucRet;
}


//F139	���·����й�����ʾֵ���ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F139(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	
	STHISVAL	  stHisVal;
	STMETERPARA		stMeterPara;
	
//	if(RunPara_GetMPType(ucDAp)==CON_MPTYPE_TER)
//		return RET_ERROR;
	
	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_UnDirPowerLm_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	for(i=1;i<5;i++)
	{
		stHisVal = Get_History_UnDirPowerLm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_UnDirPowerLm_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirPowerLm_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirPowerLm_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirPowerLm_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_5B_DT14( pucRetBuffer ,m, stHisVal.lVal);*/
	
	(*pusPos)=m;
	return ucRet;
}


//F140	���·����޹�����ʾֵ���ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F140(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	
	STHISVAL	  stHisVal;
	STMETERPARA		stMeterPara;
	
//	if(RunPara_GetMPType(ucDAp)==CON_MPTYPE_TER)
//		return RET_ERROR;
	
	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_UnDirVarLm_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	for(i=1;i<5;i++)
	{
		stHisVal = Get_History_UnDirVarLm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_UnDirVarLm_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirVarLm_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirVarLm_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_UnDirVarLm_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);*/
	
	(*pusPos)=m;
	return ucRet;
}


//F141	����һ�����޹�����ʾֵ���ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F141(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=1;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	
	STHISVAL	  stHisVal;
	STMETERPARA		stMeterPara;
	
//	if(RunPara_GetMPType(ucDAp)==CON_MPTYPE_TER)
//		return RET_ERROR;
	
	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_DirVar1Lm_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	for(i=1;i<5;i++)
	{
		stHisVal = Get_History_DirVar1Lm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirVar1Lm_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar1Lm_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar1Lm_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar1Lm_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);*/
	
	(*pusPos)=m;
	return ucRet;
}


//F142	���¶������޹�����ʾֵ���ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F142(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=1;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	
	STHISVAL	  stHisVal;
	STMETERPARA		stMeterPara;
	
//	if(RunPara_GetMPType(ucDAp)==CON_MPTYPE_TER)
//		return RET_ERROR;
	
	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_DirVar2Lm_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	for(i=1;i<5;i++)
	{
		stHisVal = Get_History_DirVar2Lm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirVar2Lm_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar2Lm_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar2Lm_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar2Lm_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);*/
	
	(*pusPos)=m;
	return ucRet;
}

//F143	�����������޹�����ʾֵ���ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F143(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	
	STHISVAL	  stHisVal;
	STMETERPARA		stMeterPara;
	
//	if(RunPara_GetMPType(ucDAp)==CON_MPTYPE_TER)
//		return RET_ERROR;
	
	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_DirVar3Lm_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	for(i=1;i<5;i++)
	{
		stHisVal = Get_History_DirVar3Lm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirVar3Lm_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar3Lm_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar3Lm_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar3Lm_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);*/
	
	(*pusPos)=m;
	return ucRet;
}

//F144	�����������޹�����ʾֵ���ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F144(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS,i=1;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	
	STHISVAL	  stHisVal;
	STMETERPARA		stMeterPara;
	
//	if(RunPara_GetMPType(ucDAp)==CON_MPTYPE_TER)
//		return RET_ERROR;
	
	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_DirVar4Lm_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	for(i=1;i<5;i++)
	{
		stHisVal = Get_History_DirVar4Lm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	}
/*	stHisVal = Get_History_DirVar4Lm_byMPNo(ucMPNo,0,1);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar4Lm_byMPNo(ucMPNo,0,2);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar4Lm_byMPNo(ucMPNo,0,3);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);
	stHisVal = Get_History_DirVar4Lm_byMPNo(ucMPNo,0,4);
	m=SetMainComm_Buf_lVal_4B( pucRetBuffer ,m, stHisVal.lVal);*/
	
	(*pusPos)=m;
	return ucRet;
}


//F145	���������й��������������ʱ�䣨�ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F145(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	unsigned char i;
	STHISVAL	  stHisVal;
	STHISTIME	  stHisTime;
	STMETERPARA		stMeterPara;
	
	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_DirPowMaxDemand_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;

	for(i=0;i<5;i++)
	{
		stHisVal = Get_History_DirPowMaxDemand_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
		stHisTime = Get_History_DirPowMaxDemandTime_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	}
	
	(*pusPos)=m;
	return ucRet;
}


//F146	���������޹��������������ʱ�䣨�ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F146(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	unsigned char i;
	STHISVAL	  stHisVal;
	STHISTIME	  stHisTime;
	STMETERPARA		stMeterPara;
	
	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_DirVarMaxDemand_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;

	for(i=0;i<5;i++)
	{
		stHisVal = Get_History_DirVarMaxDemand_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
		stHisTime = Get_History_DirVarMaxDemandTime_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	}
	
	(*pusPos)=m;
	return ucRet;
}


//F147	���·����й��������������ʱ�䣨�ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F147(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	unsigned char i;
	
	STHISVAL	  stHisVal;
	STHISTIME	  stHisTime;
	STMETERPARA		stMeterPara;
	
	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_UnDirPowMaxDemand_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;

	for(i=0;i<5;i++)
	{
		stHisVal = Get_History_UnDirPowMaxDemand_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
		stHisTime = Get_History_UnDirPowMaxDemandTime_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	}

	(*pusPos)=m;
	return ucRet;
}


//F148	���·����޹��������������ʱ�䣨�ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F148(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	unsigned char i;
	
	STHISVAL	  stHisVal;
	STHISTIME	  stHisTime;
	STMETERPARA		stMeterPara;
	
	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_UnDirPowMaxDemand_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;

	for(i=0;i<5;i++)
	{
		stHisVal = Get_History_UnDirPowMaxDemand_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
		stHisTime = Get_History_UnDirVarMaxDemandTime_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	}
	
	(*pusPos)=m;
	return ucRet;
}


//F149	���������й��������������ʱ�䣨�ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F149(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	unsigned char i;
	STHISVAL	  stHisVal;
	STHISTIME	  stHisTime;
	STMETERPARA		stMeterPara;
	
	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_DirPowMaxDemandLm_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;

	for(i=0;i<5;i++)
	{
		stHisVal = Get_History_DirPowMaxDemandLm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
		stHisTime = Get_History_DirPowMaxDemandTimeLm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	}
	
	(*pusPos)=m;
	return ucRet;
}


//F150	���������޹��������������ʱ�䣨�ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F150(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	unsigned char i;
	STHISVAL	  stHisVal;
	STHISTIME	  stHisTime;
	STMETERPARA		stMeterPara;
	
	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_DirVarMaxDemandLm_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;

	for(i=0;i<5;i++)
	{
		stHisVal = Get_History_DirVarMaxDemandLm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
		stHisTime = Get_History_DirVarMaxDemandTimeLm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	}
	
	(*pusPos)=m;
	return ucRet;
}


//F151	���·����й��������������ʱ�䣨�ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F151(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	unsigned char i;
	
	STHISVAL	  stHisVal;
	STHISTIME	  stHisTime;
	STMETERPARA		stMeterPara;
	
	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_UnDirPowMaxDemandLm_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;

	for(i=0;i<5;i++)
	{
		stHisVal = Get_History_UnDirPowMaxDemandLm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
		stHisTime = Get_History_UnDirPowMaxDemandTimeLm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	}

	(*pusPos)=m;
	return ucRet;
}


//F152	���·����޹��������������ʱ�䣨�ܡ�����1~M��
unsigned char Main_Get_ReqData_1_F152(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo=0;
	unsigned short m=(*pusPos);
	unsigned char i;
	
	STHISVAL	  stHisVal;
	STHISTIME	  stHisTime;
	STMETERPARA		stMeterPara;
	
	ucMPNo = ucDAp;
	stMeterPara=RunPara_GetMeterPara_byMPNo(ucMPNo);
	
	stHisVal = Get_History_UnDirPowMaxDemandLm_byMPNo(ucMPNo,0,0);
	m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stHisVal.stDateTime);
	
	pucRetBuffer[m++]=stMeterPara.ucFeeNum;

	for(i=0;i<5;i++)
	{
		stHisVal = Get_History_UnDirPowMaxDemandLm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_lVal_3B( pucRetBuffer ,m, stHisVal.lVal);
		stHisTime = Get_History_UnDirVarMaxDemandTimeLm_byMPNo(ucMPNo,0,i);
		m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	}
	
	(*pusPos)=m;
	return ucRet;
}

//��ȡ��һ����Ĺ���
unsigned char Main_Get_ReqData_1_InstantPow(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	unsigned char i;
	unsigned char ucPointNum;			//����
	unsigned char ucM=0;				//�ܶ�
	unsigned char ucDataLen=0;
	unsigned short usOnePointLen;
	STDATETIME stStartTime;
	unsigned char *pucBuffer;
	unsigned char ucMPNo,ucMeterNo,ucTaskNo=0xFF,ucMPType;
	unsigned char ucDesBuf[4];
//	unsigned char ucPulseType;
	
	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
		if(ucMeterNo>=CON_METER_NUM)
			return RET_ERROR;
		ucTaskNo=ucMeterNo;
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseType=RunPara_GetPulseType_ByDI(usDI);
//		if(ucPulseType!=CON_PULSEATTR_INVALID)
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,ucPulseType);
//			ucTaskNo=Task_GetTaskNo_byPulseNo(ucPulseNo);
//		}
//		else
//		{
//			ucTaskNo=CON_GRTASK_NUM;			//��Ч
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//		ucTaskNo=Task_GetTaskNo_Ter();
	if(ucTaskNo>=CON_GRTASK_NUM)
		return RET_ERROR;
		
//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;			//����������
	ucM=GetFreeseMByMPNo(ucMPNo);
	if((ucM>4) || (ucM==0))
		return RET_ERROR;
	ucM=1<<(ucM-1);							//�ܶȴ���ת��Ϊ�ܶ���ֵ
	ucPointNum=4/ucM;
	stStartTime=GetLastOneHourStartTime((unsigned char)(15*ucM));
	if(gucJob1HoursFlag>0 && gucJob1HoursFlag<8)
		SubTime(&stStartTime,gucJob1HoursFlag,CON_TIMEUNIT_HOUR);
//	pucRetBuffer[m]=(ucM<<6)+(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//��һ�ֽڶ����ܶ�BCD��	����ʱ�䣨Сʱ��BCD��ʮλ	����ʱ�䣨Сʱ��BCD���λ
	pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//��һ�ֽڶ����ܶ�BCD��	����ʱ�䣨Сʱ��BCD��ʮλ	����ʱ�䣨Сʱ��BCD���λ
	pucRetBuffer[m+1]=ucM;
	memset(pucRetBuffer+m+2,0xEE,3*ucPointNum);
	(*pusPos)=m+3*ucPointNum+2;

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }	
	pucBuffer=(unsigned char *)mem_ptr;			//����������
	Task_QueryRetVal(ucTaskNo,pucBuffer,&stStartTime,(unsigned short)ucPointNum,ucM,&usOnePointLen);
	if(((pucBuffer[0]+0x100*pucBuffer[1])!=0) && (usOnePointLen!=0))
	{
		m+=2;
		pucBuffer+=2;
		for(i=0;i<ucPointNum;i++)
		{
			ucDataLen=Get_TaskData(ucDesBuf,pucBuffer,ucTaskNo,usDI);
			if(ucDataLen==2)
			{
				if(ucDesBuf[0]==0xEE)
				{
					memset(pucRetBuffer+m,0xEE,3);
				}else
				{
					pucRetBuffer[m]=0;
					if(ucDesBuf[1]>0x79)
						ucDesBuf[1]=0x79;			//���ݸ�ʽ9��Ŀǰ���������ݣ����ֵ79.9999
					memcpy(pucRetBuffer+m+1,ucDesBuf,2);		//�޹�2�ֽڣ�0 ������ֽ�
				}
			}else
			{
				if(ucDesBuf[0]==0xEE)
				{
					memset(pucRetBuffer+m,0xEE,3);
				}else
				{
					if(ucDesBuf[2]>0x79)
						ucDesBuf[2]=0x79;			//���ݸ�ʽ9��Ŀǰ���������ݣ����ֵ79.9999
					memcpy(pucRetBuffer+m,ucDesBuf,3);			//�й�3�ֽ�
				}
			}
			m+=3;
			pucBuffer+=usOnePointLen;
		}
		(*pusPos)=m;
	}
	free_mem_block(mem_ptr);//
	return ucRet;
}

//��ȡ��һ����ĵ�ѹ
unsigned char Main_Get_ReqData_1_Volt(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	unsigned char i;
	unsigned char ucPointNum;			//����
	unsigned char ucM=0;				//�ܶ�
//	unsigned char ucDataLen=0;
	unsigned short usOnePointLen;
	STDATETIME stStartTime;
	unsigned char *pucBuffer;
	unsigned char ucMPNo,ucMeterNo,ucTaskNo=0xFF,ucMPType;
	unsigned long ulVal;
	
	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
		if(ucMeterNo>=CON_METER_NUM)
			return RET_ERROR;
		ucTaskNo=ucMeterNo;
	}
//	else if(ucMPType==CON_MPTYPE_TER)
//		ucTaskNo=Task_GetTaskNo_Ter();
	if(ucTaskNo>=CON_GRTASK_NUM)
		return RET_ERROR;
	
//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;			//����������
	ucM=GetFreeseMByMPNo(ucMPNo);
	if((ucM>4) || (ucM==0))
		return RET_ERROR;
	ucM=1<<(ucM-1);							//�ܶȴ���ת��Ϊ�ܶ���ֵ
	ucPointNum=4/ucM;
	stStartTime=GetLastOneHourStartTime((unsigned char)(15*ucM));
	if(gucJob1HoursFlag>0 && gucJob1HoursFlag<8)
		SubTime(&stStartTime,gucJob1HoursFlag,CON_TIMEUNIT_HOUR);
	pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//��һ�ֽڶ����ܶ�BCD��	����ʱ�䣨Сʱ��BCD��ʮλ	����ʱ�䣨Сʱ��BCD���λ
	pucRetBuffer[m+1]=ucM;
	memset(pucRetBuffer+m+2,0xEE,2*ucPointNum);
	(*pusPos)=m+2*ucPointNum+2;

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucBuffer=(unsigned char *)mem_ptr;			//����������
	Task_QueryRetVal(ucTaskNo,pucBuffer,&stStartTime,(unsigned short)ucPointNum,ucM,&usOnePointLen);
	if(((pucBuffer[0]+0x100*pucBuffer[1])!=0) && (usOnePointLen!=0))
	{
		m+=2;
		pucBuffer+=2;
		for(i=0;i<ucPointNum;i++)
		{
			ulVal=0;
			//ucDataLen=Get_TaskData((unsigned char *)&ulVal,pucBuffer,ucTaskNo,usDI);
            Get_TaskData((unsigned char *)&ulVal,pucBuffer,ucTaskNo,usDI);//azh
		//	if((unsigned char)ulVal!=0xEE)
		//		ulVal=(ulVal<<4);				//���ݸ�ʽ08ת��Ϊ���ݸ�ʽ07
			memcpy(pucRetBuffer+m,(unsigned char *)&ulVal,2);
			m+=2;
			pucBuffer+=usOnePointLen;
		}
		(*pusPos)=m;
	}
	free_mem_block(mem_ptr);//
	return ucRet;
}

//��ȡ��һ����ĵ���
unsigned char Main_Get_ReqData_1_Curr(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	unsigned char i;
	unsigned char ucPointNum;			//����
	unsigned char ucM=0;				//�ܶ�
	unsigned char ucDataLen=0;
	unsigned short usOnePointLen;
	STDATETIME stStartTime;
	unsigned char *pucBuffer;
	unsigned char ucMPNo,ucMeterNo,ucTaskNo=0xFF,ucMPType;
	
	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
		if(ucMeterNo>=CON_METER_NUM)
			return RET_ERROR;
		ucTaskNo=ucMeterNo;
	}
//	else if(ucMPType==CON_MPTYPE_TER)
//		ucTaskNo=Task_GetTaskNo_Ter();
	if(ucTaskNo>=CON_GRTASK_NUM)
		return RET_ERROR;
	
//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;			//����������
	ucM=GetFreeseMByMPNo(ucMPNo);
	if((ucM>4) || (ucM==0))
		return RET_ERROR;
	ucM=1<<(ucM-1);							//�ܶȴ���ת��Ϊ�ܶ���ֵ
	ucPointNum=4/ucM;
	stStartTime=GetLastOneHourStartTime((unsigned char)(15*ucM));
	if(gucJob1HoursFlag>0 && gucJob1HoursFlag<8)
		SubTime(&stStartTime,gucJob1HoursFlag,CON_TIMEUNIT_HOUR);
	pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//��һ�ֽڶ����ܶ�BCD��	����ʱ�䣨Сʱ��BCD��ʮλ	����ʱ�䣨Сʱ��BCD���λ
	pucRetBuffer[m+1]=ucM;
	memset(pucRetBuffer+m+2,0xEE,3*ucPointNum);
	(*pusPos)=m+3*ucPointNum+2;

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucBuffer=(unsigned char *)mem_ptr;			//����������       	
	Task_QueryRetVal(ucTaskNo,pucBuffer,&stStartTime,(unsigned short)ucPointNum,ucM,&usOnePointLen);
	if(((pucBuffer[0]+0x100*pucBuffer[1])!=0) && (usOnePointLen!=0))
	{
		m+=2;
		pucBuffer+=2;
		for(i=0;i<ucPointNum;i++)
		{
			ucDataLen=Get_TaskData(pucRetBuffer+m,pucBuffer,ucTaskNo,usDI);
			m+=ucDataLen;
			pucBuffer+=usOnePointLen;
		}
		(*pusPos)=m;
	}
	free_mem_block(mem_ptr);//
	return ucRet;
}

//��ȡ��һ����ĵ���
unsigned char Main_Get_ReqData_1_PowVal(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	unsigned char i;
	unsigned char ucPointNum,ucPointNumTmp;			//����
	unsigned char ucM=0;				//�ܶ�
//	unsigned char ucDataLen=0;
	unsigned short usOnePointLen;
	STDATETIME stStartTime;
	unsigned char *pucBuffer;
	unsigned char ucMPNo,ucMeterNo,ucTaskNo=0xFF,ucMPType;
	unsigned char ucTmp[4];
	unsigned long ulValBase,ulValThis,ulTmp;
//	unsigned char ucPulseType;
	
	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
		if(ucMeterNo>=CON_METER_NUM)
			return RET_ERROR;
		ucTaskNo=ucMeterNo;
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseType=RunPara_GetPulseType_ByDI(usDI);
//		if(ucPulseType!=CON_PULSEATTR_INVALID)
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,ucPulseType);
//			ucTaskNo=Task_GetTaskNo_byPulseNo(ucPulseNo);
//		}
//		else
//		{
//			ucTaskNo=CON_GRTASK_NUM;			//��Ч
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//		ucTaskNo=Task_GetTaskNo_Ter();
	if(ucTaskNo>=CON_GRTASK_NUM)
		return RET_ERROR;
	
//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;			//����������
	ucM=GetFreeseMByMPNo(ucMPNo);
	if((ucM>4) || (ucM==0))
		return RET_ERROR;
	ucM=1<<(ucM-1);							//�ܶȴ���ת��Ϊ�ܶ���ֵ
	ucPointNum=4/ucM;
	ucPointNumTmp=ucPointNum+1;
	stStartTime=GetLastOneHourStartTime(0);
	if(gucJob1HoursFlag>0 && gucJob1HoursFlag<8)
		SubTime(&stStartTime,gucJob1HoursFlag,CON_TIMEUNIT_HOUR);
	pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//��һ�ֽڶ����ܶ�BCD��	����ʱ�䣨Сʱ��BCD��ʮλ	����ʱ�䣨Сʱ��BCD���λ
	pucRetBuffer[m+1]=ucM;
	(*pusPos)=m+4*ucPointNum+2;

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucBuffer=(unsigned char *)mem_ptr;			//����������  
	Task_QueryRetVal(ucTaskNo,pucBuffer,&stStartTime,(unsigned short)ucPointNumTmp,ucM,&usOnePointLen);
	if(((pucBuffer[0]+0x100*pucBuffer[1])!=0) && (usOnePointLen!=0) && ((pucBuffer[0]+0x100*pucBuffer[1])!=1))
	{
		m+=2;
		pucBuffer+=2;
		memset(ucTmp,0,4);
		//ucDataLen=Get_TaskData(ucTmp,pucBuffer,ucTaskNo,usDI);
		Get_TaskData(ucTmp,pucBuffer,ucTaskNo,usDI);
		if(ucTmp[0]==CON_GRTASK_DATA_INVALID)
			ulValBase=CONINVALIDVAL;
		else
			ulValBase=BCD_TO_HEX_4(ucTmp);
		pucBuffer+=usOnePointLen;
		for(i=0;i<ucPointNum;i++)
		{
			memset(ucTmp,0,4);
			//ucDataLen=Get_TaskData(ucTmp,pucBuffer,ucTaskNo,usDI);
            Get_TaskData(ucTmp,pucBuffer,ucTaskNo,usDI);
			if(ucTmp[0]==CON_GRTASK_DATA_INVALID)
				ulValThis=CONINVALIDVAL;
			else
				ulValThis=BCD_TO_HEX_4(ucTmp);
			if(ulValThis!=CONINVALIDVAL && ulValBase!=CONINVALIDVAL)
			{
				if(ulValThis>ulValBase)
					ulTmp=ulValThis-ulValBase;
				else
				{
					ulValThis=ulValBase;
					ulTmp=0;
				}
				SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulTmp);
			}else
			{
				memset(pucRetBuffer+m,0xEE,4);	
			}
			ulValBase=ulValThis;
			m+=4;
			pucBuffer+=usOnePointLen;
		}
		(*pusPos)=m;
	}
	free_mem_block(mem_ptr);//
	return ucRet;
}

//��ȡ��һ����ĵ���ʾֵ
unsigned char Main_Get_ReqData_1_PowViewData(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	unsigned char i;
	unsigned char ucPointNum;			//����
	unsigned char ucM=0;				//�ܶ�
	unsigned char ucDataLen=0;
	unsigned short usOnePointLen;
	STDATETIME stStartTime;
	unsigned char *pucBuffer;
	unsigned char ucMPNo,ucMeterNo,ucTaskNo=0xFF,ucMPType;
//	unsigned char ucPulseType;
	
	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
		if(ucMeterNo>=CON_METER_NUM)
			return RET_ERROR;
		ucTaskNo=ucMeterNo;
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseType=RunPara_GetPulseType_ByDI(usDI);
//		if(ucPulseType!=CON_PULSEATTR_INVALID)
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,ucPulseType);
//			ucTaskNo=Task_GetTaskNo_byPulseNo(ucPulseNo);
//		}
//		else
//		{
//			ucTaskNo=CON_GRTASK_NUM;			//��Ч
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//		ucTaskNo=Task_GetTaskNo_Ter();
	if(ucTaskNo>=CON_GRTASK_NUM)
		return RET_ERROR;
	
//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;			//����������
	ucM=GetFreeseMByMPNo(ucMPNo);
	if((ucM>4) || (ucM==0))
		return RET_ERROR;
	ucM=1<<(ucM-1);							//�ܶȴ���ת��Ϊ�ܶ���ֵ
	ucPointNum=4/ucM;
	stStartTime=GetLastOneHourStartTime((unsigned char)(15*ucM));
	if(gucJob1HoursFlag>0 && gucJob1HoursFlag<8)
		SubTime(&stStartTime,gucJob1HoursFlag,CON_TIMEUNIT_HOUR);
//	pucRetBuffer[m]=(ucM<<6)+(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//��һ�ֽڶ����ܶ�BCD��	����ʱ�䣨Сʱ��BCD��ʮλ	����ʱ�䣨Сʱ��BCD���λ
	pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//��һ�ֽڶ����ܶ�BCD��	����ʱ�䣨Сʱ��BCD��ʮλ	����ʱ�䣨Сʱ��BCD���λ
	pucRetBuffer[m+1]=ucM;
	memset(pucRetBuffer+m+2,0xEE,4*ucPointNum);
	(*pusPos)=m+4*ucPointNum+2;

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucBuffer=(unsigned char *)mem_ptr;			//����������        
	Task_QueryRetVal(ucTaskNo,pucBuffer,&stStartTime,(unsigned short)ucPointNum,ucM,&usOnePointLen);
	if(((pucBuffer[0]+0x100*pucBuffer[1])!=0) && (usOnePointLen!=0))
	{
		m+=2;
		pucBuffer+=2;
		for(i=0;i<ucPointNum;i++)
		{
			ucDataLen=Get_TaskData(pucRetBuffer+m,pucBuffer,ucTaskNo,usDI);
			m+=ucDataLen;
			pucBuffer+=usOnePointLen;
		}
		(*pusPos)=m;
	}
	free_mem_block(mem_ptr);//
	return ucRet;
}

//��ȡ��һ����Ĺ�������
unsigned char Main_Get_ReqData_1_PowRate(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stStartTime;
	unsigned char *pucBufferP,*pucBufferQ;
	unsigned char ucDataLenP,ucDataLenQ;
	unsigned char i,ucM,ucPointNumP,ucPointNumQ;
	unsigned char ucMPNo,ucMeterNo,ucTaskNo=0xFF,ucTaskNo2=0xFF,ucMPType;
	unsigned short m=(*pusPos);
	unsigned long ulPow,ulVar,ulRate,ulTmp;
	unsigned short usDIP,usDIQ;
	
	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
		if(ucMeterNo>=CON_METER_NUM)
			return RET_ERROR;
		ucTaskNo=ucMeterNo;
		ucTaskNo2=ucTaskNo;
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRPOW);
//		ucTaskNo=Task_GetTaskNo_byPulseNo(ucPulseNo);
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
//		ucTaskNo2=Task_GetTaskNo_byPulseNo(ucPulseNo);
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
//		ucTaskNo=Task_GetTaskNo_Ter();
//		ucTaskNo2=ucTaskNo;
//	}
	if(ucTaskNo>=CON_GRTASK_NUM)
		return RET_ERROR;
	
//	pucBufferP=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;
//	pucBufferQ=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF+CON_BUFFER_CLASS3_DIFF;
	ucM=GetFreeseMByMPNo(ucMPNo);
	if((ucM>4) || (ucM==0))
		return RET_ERROR;
	ucM=1<<(ucM-1);							//�ܶȴ���ת��Ϊ�ܶ���ֵ
	ucPointNumP=4/ucM;
	ucPointNumQ=ucPointNumP;
	stStartTime=GetLastOneHourStartTime((unsigned char)(15*ucM));
	if(gucJob1HoursFlag>0 && gucJob1HoursFlag<8)
		SubTime(&stStartTime,gucJob1HoursFlag,CON_TIMEUNIT_HOUR);
//	pucRetBuffer[m]=(ucM<<6)+(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//��һ�ֽڶ����ܶ�BCD��	����ʱ�䣨Сʱ��BCD��ʮλ	����ʱ�䣨Сʱ��BCD���λ
	pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//��һ�ֽڶ����ܶ�BCD��	����ʱ�䣨Сʱ��BCD��ʮλ	����ʱ�䣨Сʱ��BCD���λ
	pucRetBuffer[m+1]=ucM;
	memset(pucRetBuffer+m+2,0xEE,2*ucPointNumP);
	(*pusPos)=m+2*ucPointNumP+2;
	switch(usDI)
	{
	case CON_DI_TYPE_MPPOWRATE:
		usDIP=CON_DI_TYPE_MPINSTANTPOW;
		usDIQ=CON_DI_TYPE_MPINSTANTVAR;
		break;
	case CON_DI_TYPE_MPPOWRATEA:
		usDIP=CON_DI_TYPE_MPINSTANTPOWA;
		usDIQ=CON_DI_TYPE_MPINSTANTVARA;
		break;
	case CON_DI_TYPE_MPPOWRATEB:
		usDIP=CON_DI_TYPE_MPINSTANTPOWB;
		usDIQ=CON_DI_TYPE_MPINSTANTVARB;
		break;
	case CON_DI_TYPE_MPPOWRATEC:
		usDIP=CON_DI_TYPE_MPINSTANTPOWC;
		usDIQ=CON_DI_TYPE_MPINSTANTVARC;
		break;
	default:
		usDIP=CON_DI_TYPE_MPINSTANTPOW;
		usDIQ=CON_DI_TYPE_MPINSTANTVAR;
		break;
	}

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucBufferP=(unsigned char *)mem_ptr;
	pucBufferQ=(unsigned char *)mem_ptr+768;

	ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBufferP,&stStartTime,ucPointNumP,ucM,&ucDataLenP,usDIP);	//��Ч������Ч���ݲ���
	if(ucRet!=RET_ERROR && ucDataLenP!=0)
	{
		ucRet=Task_QueryRetVal_ByDI(ucTaskNo2,pucBufferQ,&stStartTime,ucPointNumQ,ucM,&ucDataLenQ,usDIQ);
		if(ucRet!=RET_ERROR && ucDataLenQ!=0)
		{
			m+=2;
			pucBufferP+=2;
			pucBufferQ+=2;
			for(i=0;i<ucPointNumP;i++)
			{
				ulPow=0;
				ulVar=0;
				memcpy((unsigned char *)&ulTmp,pucBufferP,ucDataLenP);
				if((unsigned char)ulTmp==CON_GRTASK_DATA_INVALID)
					ulPow=CONINVALIDVAL;
				else
					ulPow=BCD_TO_HEX_3((unsigned char *)&ulTmp);
				memcpy((unsigned char *)&ulTmp,pucBufferQ,ucDataLenQ);
				if((unsigned char)ulTmp==CON_GRTASK_DATA_INVALID)
					ulVar=CONINVALIDVAL;
				else
					ulVar=BCD_TO_HEX_2((unsigned char *)&ulTmp);
				if(ulPow!=CONINVALIDVAL && ulVar!=CONINVALIDVAL)
				{
					ulRate=GetPQRate(ulPow,ulVar);
					ulRate=ulRate*10;
				}else
					ulRate=CONINVALIDVAL;
				m=SetMainComm_Buf_lVal_2B(pucRetBuffer,m,ulRate);
				pucBufferP+=ucDataLenP;
				pucBufferQ+=ucDataLenQ;
			}
			(*pusPos)=m;
		}
	}
	free_mem_block(mem_ptr);//
	return ucRet;
}


unsigned short Main_Get_ReqData_2_Dirkwh(unsigned char *pucBuffer,unsigned short usPos,STHISRUNDATA *pstHisRunData)
{
	unsigned char i;
	unsigned long *pulTmp;
	unsigned short m=usPos;
	pulTmp = &pstHisRunData->ulHisDirPowTotal;
	for(i=0;i<5;i++)
	{
		m=SetMainComm_Buf_lVal_5B_DT14(pucBuffer,m,*pulTmp);
		pulTmp ++;
	}

	pulTmp = &pstHisRunData->ulHisDirVarTotal;
	for(i=0;i<5;i++)
	{
		m=SetMainComm_Buf_lVal_4B(pucBuffer,m,*pulTmp);
		pulTmp ++;
	}

	pulTmp = &pstHisRunData->ulHisDirVar1;
	for(i=0;i<5;i++)
	{
		m=SetMainComm_Buf_lVal_4B(pucBuffer,m,*pulTmp);
		pulTmp ++;
	}

	pulTmp = &pstHisRunData->ulHisDirVar4;
	for(i=0;i<5;i++)
	{
		m=SetMainComm_Buf_lVal_4B(pucBuffer,m,*pulTmp);
		pulTmp ++;
	}
	return m;
}


unsigned short Main_Get_ReqData_2_Revkwh(unsigned char *pucBuffer,unsigned short usPos,STHISRUNDATA *pstHisRunData)
{
	unsigned char i;
	unsigned long *pulTmp;
	unsigned short m=usPos;
	pulTmp = &pstHisRunData->ulHisRevPowTotal;
	for(i=0;i<5;i++)
	{
		m=SetMainComm_Buf_lVal_5B_DT14(pucBuffer,m,*pulTmp);
		pulTmp ++;
	}

	pulTmp = &pstHisRunData->ulHisRevVarTotal;
	for(i=0;i<5;i++)
	{
		m=SetMainComm_Buf_lVal_4B(pucBuffer,m,*pulTmp);
		pulTmp ++;
	}

	pulTmp = &pstHisRunData->ulHisDirVar2;
	for(i=0;i<5;i++)
	{
		m=SetMainComm_Buf_lVal_4B(pucBuffer,m,*pulTmp);
		pulTmp ++;
	}

	pulTmp = &pstHisRunData->ulHisDirVar3;
	for(i=0;i<5;i++)
	{
		m=SetMainComm_Buf_lVal_4B(pucBuffer,m,*pulTmp);
		pulTmp ++;
	}
	return m;
}


//====================  �������� ===================
//F1	�ն���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F1(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;
//	unsigned char i;
//	unsigned long *pulTmp;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,94);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���
        
		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			
			m=Main_Get_ReqData_2_Dirkwh(pucRetBuffer,m,pstHisRunData);
		}
		free_mem_block(mem_ptr);//
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			if(stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//			{
//				pulTmp = &pstHisPulseRunData->ulHisPowTotal;
//				for(i=0;i<5;i++)
//				{
//					m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,*pulTmp);
//					pulTmp ++;
//				}
//			//	m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowTotal);
//			//	m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee1);
//			//	m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee2);
//			//	m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee3);
//			//	m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee4);
//			}else
//			{
//				m+=25;
//			}
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//			ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_DIRVAR)
//			{
//				pulTmp = &pstHisPulseRunData->ulHisPowTotal;
//				for(i=0;i<5;i++)
//				{
//					m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,*pulTmp);
//					pulTmp ++;
//				}
//			//	m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowTotal);
//			//	m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee1);
//			//	m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee2);
//			//	m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee3);
//			//	m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee4);
//			}else
//			{
//				m+=20;
//			}
//			m+=40;
//			ucRet=RET_SUCCESS;		//�ָ�
//		}else
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//			ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			if(ucRet==RET_SUCCESS)
//			{
//				m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//				stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//				m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//				pucRetBuffer[m++]=4;
//				if(stPulsePara.ucAttr==CON_PULSEATTR_DIRVAR)
//				{
//					m+=25;
//					pulTmp = &pstHisPulseRunData->ulHisPowTotal;
//					for(i=0;i<5;i++)
//					{
//						m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,*pulTmp);
//						pulTmp ++;
//					}
//				//	m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowTotal);
//				//	m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee1);
//				//	m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee2);
//				//	m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee3);
//				//	m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee4);
//					m+=40;
//				}else
//				{
//					m+=85;
//				}
//			}
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=Main_Get_ReqData_2_Dirkwh(pucRetBuffer,m,pstHisRunData);
//		}
//		free_mem_block(mem_ptr);//
//	}
	else
		return RET_ERROR;

	(*pusPos)=m;
	return ucRet;
}

//F2	�ն��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F2(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;
//	unsigned char i;
//	unsigned long *pulTmp;
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,94);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=Main_Get_ReqData_2_Revkwh(pucRetBuffer,m,pstHisRunData);
			
		}
		free_mem_block(mem_ptr);//
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			if(stPulsePara.ucAttr==CON_PULSEATTR_REVPOW)
//			{
//				pulTmp = &pstHisPulseRunData->ulHisPowTotal;
//				for(i=0;i<5;i++)
//				{
//					m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,*pulTmp);
//					pulTmp ++;
//				}
//				
//			//	m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowTotal);
//			//	m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee1);
//			//	m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee2);
//			//	m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee3);
//			//	m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee4);
//			}else
//			{
//				m+=25;
//			}
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVVAR);
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//			ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_REVVAR)
//			{
//				pulTmp = &pstHisPulseRunData->ulHisPowTotal;
//				for(i=0;i<5;i++)
//				{
//					m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,*pulTmp);
//					pulTmp ++;
//				}
//			//	m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowTotal);
//			//	m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee1);
//			//	m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee2);
//			//	m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee3);
//			//	m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee4);
//			}else
//			{
//				m+=20;
//			}
//			m+=40;
//			ucRet=RET_SUCCESS;		//�ָ�
//		}else
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVVAR);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//			ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			if(ucRet==RET_SUCCESS)
//			{
//				m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//				stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//				m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//				pucRetBuffer[m++]=4;
//				if(stPulsePara.ucAttr==CON_PULSEATTR_REVVAR)
//				{
//					m+=25;
//					pulTmp = &pstHisPulseRunData->ulHisPowTotal;
//					for(i=0;i<5;i++)
//					{
//						m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,*pulTmp);
//						pulTmp ++;
//					}
//				//	m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowTotal);
//				//	m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee1);
//				//	m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee2);
//				//	m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee3);
//				//	m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee4);
//					m+=40;
//				}else
//				{
//					m+=85;
//				}
//			}
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=Main_Get_ReqData_2_Revkwh(pucRetBuffer,m,pstHisRunData);
//		}
//		free_mem_block(mem_ptr);//
//	}
    else
		return RET_ERROR;

	(*pusPos)=m;
	return ucRet;
}

//F3	�ն���������/�޹��������������ʱ�䣨�ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F3(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;
	unsigned char i;
	unsigned long *pulTmp;
	PSTDATETIME pstdatetimetemp;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,79);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			pulTmp = &pstHisRunData->ulHisDirPowMaxDemand;
			for(i=0;i<5;i++)
			{
				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
				pulTmp ++;
			}
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemand);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee1);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee2);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee3);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee4);
			pstdatetimetemp = &pstHisRunData->stHisDirPowMaxDemandTime;
			for(i=0;i<5;i++)
			{
				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
				pstdatetimetemp ++;
			}
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTime);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee1);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee2);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee3);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee4);
			pulTmp = &pstHisRunData->ulHisDirVarMaxDemand;
			for(i=0;i<5;i++)
			{
				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
				pulTmp ++;
			}
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemand);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee1);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee2);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee3);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee4);
			pstdatetimetemp = &pstHisRunData->stHisDirVarMaxDemandTime;
			for(i=0;i<5;i++)
			{
				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
				pstdatetimetemp ++;
			}
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTime);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee1);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee2);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee3);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee4);
		}
		free_mem_block(mem_ptr);//
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			if(stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//			{
//				pulTmp = &pstHisPulseRunData->ulHisPowMaxDemand;
//				for(i=0;i<5;i++)
//				{
//					m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
//					pulTmp++;
//				}
//				pstdatetimetemp = &pstHisPulseRunData->stHisPowMaxDemandTime;
//				for(i=0;i<5;i++)
//				{
//					m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
//					pstdatetimetemp ++;
//				}
///*				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemand);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee1);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee2);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee3);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee4);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTime);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee1);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee2);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee3);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee4);*/
//			}else
//			{
//				m+=35;
//			}
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//			ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_DIRVAR)
//			{
//				pulTmp = &pstHisPulseRunData->ulHisPowMaxDemand;
//				for(i=0;i<5;i++)
//				{
//					m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
//					pulTmp++;
//				}
//				pstdatetimetemp = &pstHisPulseRunData->stHisPowMaxDemandTime;
//				for(i=0;i<5;i++)
//				{
//					m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
//					pstdatetimetemp ++;
//				}
//				
///*				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemand);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee1);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee2);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee3);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee4);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTime);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee1);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee2);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee3);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee4);*/
//			}else
//			{
//				m+=35;
//			}
//			ucRet=RET_SUCCESS;		//�ָ�
//		}else
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//			ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			if(ucRet==RET_SUCCESS)
//			{
//				m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//				stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//				m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//				pucRetBuffer[m++]=4;
//				if(stPulsePara.ucAttr==CON_PULSEATTR_DIRVAR)
//				{
//					m+=35;
//					pulTmp = &pstHisPulseRunData->ulHisPowMaxDemand;
//					for(i=0;i<5;i++)
//					{
//						m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
//						pulTmp++;
//					}
//					pstdatetimetemp = &pstHisPulseRunData->stHisPowMaxDemandTime;
//					for(i=0;i<5;i++)
//					{
//						m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
//						pstdatetimetemp ++;
//					}
//					
///*					m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemand);
//					m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee1);
//					m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee2);
//					m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee3);
//					m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee4);
//					m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTime);
//					m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee1);
//					m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee2);
//					m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee3);
//					m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee4);*/
//				}else
//				{
//					m+=70;
//				}
//			}
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			
//			pulTmp = &pstHisRunData->ulHisDirPowMaxDemand;
//			for(i=0;i<5;i++)
//			{
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
//				pulTmp ++;
//			}
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemand);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee1);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee2);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee3);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee4);
//			pstdatetimetemp = &pstHisRunData->stHisDirPowMaxDemandTime;
//			for(i=0;i<5;i++)
//			{
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
//				pstdatetimetemp ++;
//			}
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTime);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee1);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee2);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee3);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee4);
//			pulTmp = &pstHisRunData->ulHisDirVarMaxDemand;
//			for(i=0;i<5;i++)
//			{
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
//				pulTmp ++;
//			}
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemand);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee1);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee2);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee3);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee4);
//			pstdatetimetemp = &pstHisRunData->stHisDirVarMaxDemandTime;
//			for(i=0;i<5;i++)
//			{
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
//				pstdatetimetemp ++;
//			}
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTime);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee1);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee2);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee3);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee4);
//
//		
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemand);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee1);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee2);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee3);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee4);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTime);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee1);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee2);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee3);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee4);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemand);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee1);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee2);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee3);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee4);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTime);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee1);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee2);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee3);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee4);
//		}
//		free_mem_block(mem_ptr);//
//	}
    else
		return RET_ERROR;

	(*pusPos)=m;
	return ucRet;
}

//F4	�ն��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F4(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;
	unsigned char i;
	unsigned long *pulTmp;
	PSTDATETIME pstdatetimetemp;
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,79);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;

			pulTmp = &pstHisRunData->ulHisRevPowMaxDemand;
			for(i=0;i<5;i++)
			{
				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
				pulTmp ++;
			}
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemand);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee1);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee2);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee3);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee4);
			pstdatetimetemp = &pstHisRunData->stHisRevPowMaxDemandTime;
			for(i=0;i<5;i++)
			{
				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
				pstdatetimetemp ++;
			}
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTime);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee1);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee2);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee3);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee4);
			pulTmp = &pstHisRunData->ulHisRevVarMaxDemand;
			for(i=0;i<5;i++)
			{
				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
				pulTmp ++;
			}
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemand);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee1);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee2);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee3);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee4);
			pstdatetimetemp = &pstHisRunData->stHisRevVarMaxDemandTime;
			for(i=0;i<5;i++)
			{
				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
				pstdatetimetemp ++;
			}
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTime);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee1);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee2);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee3);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee4);


		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemand);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee1);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee2);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee3);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee4);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTime);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee1);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee2);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee3);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee4);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemand);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee1);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee2);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee3);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee4);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTime);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee1);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee2);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee3);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee4);
		}
		free_mem_block(mem_ptr);//
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			if(stPulsePara.ucAttr==CON_PULSEATTR_REVPOW)
//			{
//				pulTmp = &pstHisPulseRunData->ulHisPowMaxDemand;
//				for(i=0;i<5;i++)
//				{
//					m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
//					pulTmp++;
//				}
//				pstdatetimetemp = &pstHisPulseRunData->stHisPowMaxDemandTime;
//				for(i=0;i<5;i++)
//				{
//					m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
//					pstdatetimetemp ++;
//				}
//
///*				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemand);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee1);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee2);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee3);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee4);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTime);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee1);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee2);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee3);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee4);*/
//			}else
//			{
//				m+=35;
//			}
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVVAR);
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//			ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_REVVAR)
//			{
//				pulTmp = &pstHisPulseRunData->ulHisPowMaxDemand;
//				for(i=0;i<5;i++)
//				{
//					m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
//					pulTmp++;
//				}
//				pstdatetimetemp = &pstHisPulseRunData->stHisPowMaxDemandTime;
//				for(i=0;i<5;i++)
//				{
//					m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
//					pstdatetimetemp ++;
//				}
///*				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemand);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee1);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee2);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee3);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee4);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTime);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee1);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee2);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee3);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee4);*/
//			}else
//			{
//				m+=35;
//			}
//			ucRet=RET_SUCCESS;		//�ָ�
//		}else
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVVAR);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//			ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			if(ucRet==RET_SUCCESS)
//			{
//				m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//				stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//				m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//				pucRetBuffer[m++]=4;
//				if(stPulsePara.ucAttr==CON_PULSEATTR_REVVAR)
//				{
//					m+=35;
//					pulTmp = &pstHisPulseRunData->ulHisPowMaxDemand;
//					for(i=0;i<5;i++)
//					{
//						m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
//						pulTmp++;
//					}
//					pstdatetimetemp = &pstHisPulseRunData->stHisPowMaxDemandTime;
//					for(i=0;i<5;i++)
//					{
//						m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
//						pstdatetimetemp ++;
//					}
///*					m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemand);
//					m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee1);
//					m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee2);
//					m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee3);
//					m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee4);
//					m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTime);
//					m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee1);
//					m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee2);
//					m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee3);
//					m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee4);*/
//				}else
//				{
//					m+=70;
//				}
//			}
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			
//			pulTmp = &pstHisRunData->ulHisRevPowMaxDemand;
//			for(i=0;i<5;i++)
//			{
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
//				pulTmp ++;
//			}
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemand);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee1);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee2);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee3);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee4);
//			pstdatetimetemp = &pstHisRunData->stHisRevPowMaxDemandTime;
//			for(i=0;i<5;i++)
//			{
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
//				pstdatetimetemp ++;
//			}
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTime);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee1);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee2);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee3);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee4);
//			pulTmp = &pstHisRunData->ulHisRevVarMaxDemand;
//			for(i=0;i<5;i++)
//			{
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
//				pulTmp ++;
//			}
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemand);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee1);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee2);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee3);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee4);
//			pstdatetimetemp = &pstHisRunData->stHisRevVarMaxDemandTime;
//			for(i=0;i<5;i++)
//			{
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
//				pstdatetimetemp ++;
//			}
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTime);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee1);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee2);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee3);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee4);
//		
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemand);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee1);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee2);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee3);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee4);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTime);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee1);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee2);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee3);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee4);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemand);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee1);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee2);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee3);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee4);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTime);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee1);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee2);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee3);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee4);
//		}
//		free_mem_block(mem_ptr);//
//	}
    else
		return RET_ERROR;

	(*pusPos)=m;
	return ucRet;
}

//F5	�ն��������й����������ܡ�����1~M��		
unsigned char Main_Get_ReqData_2_F5(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{//��һ�����һ��
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	unsigned long ulFeeT,ulFee1,ulFee2,ulFee3,ulFee4;
	STHISRUNDATA *pstHisRunData;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

	ulFeeT=CONINVALIDVAL;	ulFee1=CONINVALIDVAL;	ulFee2=CONINVALIDVAL;	
	ulFee3=CONINVALIDVAL;	ulFee4=CONINVALIDVAL;
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,24);
	pucRetBuffer[m+3]=4;					//����

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			if(pstHisRunData->ulHisDirPowTotal != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowTotal != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirPowTotal > pstHisRunData->ulHisLastDayDirPowTotal)
					ulFeeT=pstHisRunData->ulHisDirPowTotal - pstHisRunData->ulHisLastDayDirPowTotal;
				else
					ulFeeT=0;
			}
			if(pstHisRunData->ulHisDirPowFee1 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowFee1 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirPowFee1 > pstHisRunData->ulHisLastDayDirPowFee1)
					ulFee1=pstHisRunData->ulHisDirPowFee1 - pstHisRunData->ulHisLastDayDirPowFee1;
				else
					ulFee1=0;
			}
			if(pstHisRunData->ulHisDirPowFee2 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowFee2 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirPowFee2 > pstHisRunData->ulHisLastDayDirPowFee2)
					ulFee2=pstHisRunData->ulHisDirPowFee2 - pstHisRunData->ulHisLastDayDirPowFee2;
				else
					ulFee2=0;
			}
			if(pstHisRunData->ulHisDirPowFee3 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowFee3 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirPowFee3 > pstHisRunData->ulHisLastDayDirPowFee3)
					ulFee3=pstHisRunData->ulHisDirPowFee3 - pstHisRunData->ulHisLastDayDirPowFee3;
				else
					ulFee3=0;
			}
			if(pstHisRunData->ulHisDirPowFee4 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowFee4 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirPowFee4 > pstHisRunData->ulHisLastDayDirPowFee4)
					ulFee4=pstHisRunData->ulHisDirPowFee4 - pstHisRunData->ulHisLastDayDirPowFee4;
				else
					ulFee4=0;
			}
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			pucRetBuffer[m++]=4;
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
		}
		free_mem_block(mem_ptr);//
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//		if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//		{
//			if(pstHisPulseRunData->ulHisPowTotal != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowTotal != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowTotal > pstHisPulseRunData->ulHisLastDayPowTotal)
//					ulFeeT=pstHisPulseRunData->ulHisPowTotal - pstHisPulseRunData->ulHisLastDayPowTotal;
//				else
//					ulFeeT=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee1 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee1 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee1 > pstHisPulseRunData->ulHisLastDayPowFee1)
//					ulFee1=pstHisPulseRunData->ulHisPowFee1 - pstHisPulseRunData->ulHisLastDayPowFee1;
//				else
//					ulFee1=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee2 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee2 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee2 > pstHisPulseRunData->ulHisLastDayPowFee2)
//					ulFee2=pstHisPulseRunData->ulHisPowFee2 - pstHisPulseRunData->ulHisLastDayPowFee2;
//				else
//					ulFee2=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee3 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee3 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee3 > pstHisPulseRunData->ulHisLastDayPowFee3)
//					ulFee3=pstHisPulseRunData->ulHisPowFee3 - pstHisPulseRunData->ulHisLastDayPowFee3;
//				else
//					ulFee3=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee4 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee4 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee4 > pstHisPulseRunData->ulHisLastDayPowFee4)
//					ulFee4=pstHisPulseRunData->ulHisPowFee4 - pstHisPulseRunData->ulHisLastDayPowFee4;
//				else
//					ulFee4=0;
//			}
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			if(pstHisRunData->ulHisDirPowTotal != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowTotal != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirPowTotal > pstHisRunData->ulHisLastDayDirPowTotal)
//					ulFeeT=pstHisRunData->ulHisDirPowTotal - pstHisRunData->ulHisLastDayDirPowTotal;
//				else
//					ulFeeT=0;
//			}
//			if(pstHisRunData->ulHisDirPowFee1 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowFee1 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirPowFee1 > pstHisRunData->ulHisLastDayDirPowFee1)
//					ulFee1=pstHisRunData->ulHisDirPowFee1 - pstHisRunData->ulHisLastDayDirPowFee1;
//				else
//					ulFee1=0;
//			}
//			if(pstHisRunData->ulHisDirPowFee2 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowFee2 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirPowFee2 > pstHisRunData->ulHisLastDayDirPowFee2)
//					ulFee2=pstHisRunData->ulHisDirPowFee2 - pstHisRunData->ulHisLastDayDirPowFee2;
//				else
//					ulFee2=0;
//			}
//			if(pstHisRunData->ulHisDirPowFee3 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowFee3 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirPowFee3 > pstHisRunData->ulHisLastDayDirPowFee3)
//					ulFee3=pstHisRunData->ulHisDirPowFee3 - pstHisRunData->ulHisLastDayDirPowFee3;
//				else
//					ulFee3=0;
//			}
//			if(pstHisRunData->ulHisDirPowFee4 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowFee4 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirPowFee4 > pstHisRunData->ulHisLastDayDirPowFee4)
//					ulFee4=pstHisRunData->ulHisDirPowFee4 - pstHisRunData->ulHisLastDayDirPowFee4;
//				else
//					ulFee4=0;
//			}
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}
//		free_mem_block(mem_ptr);//
//	}
	else
		return RET_ERROR;

	(*pusPos)=m;
	return ucRet;
}

//F6	�ն��������޹����������ܡ�����1~M��		
unsigned char Main_Get_ReqData_2_F6(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	unsigned long ulFeeT,ulFee1,ulFee2,ulFee3,ulFee4;
	STHISRUNDATA *pstHisRunData;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

	ulFeeT=CONINVALIDVAL;	ulFee1=CONINVALIDVAL;	ulFee2=CONINVALIDVAL;	
	ulFee3=CONINVALIDVAL;	ulFee4=CONINVALIDVAL;
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,24);
	pucRetBuffer[m+3]=4;					//����

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			if(pstHisRunData->ulHisDirVarTotal != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarTotal != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirVarTotal > pstHisRunData->ulHisLastDayDirVarTotal)
					ulFeeT=pstHisRunData->ulHisDirVarTotal - pstHisRunData->ulHisLastDayDirVarTotal;
				else
					ulFeeT=0;
			}
			if(pstHisRunData->ulHisDirVarFee1 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarFee1 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirVarFee1 > pstHisRunData->ulHisLastDayDirVarFee1)
					ulFee1=pstHisRunData->ulHisDirVarFee1 - pstHisRunData->ulHisLastDayDirVarFee1;
				else
					ulFee1=0;
			}
			if(pstHisRunData->ulHisDirVarFee2 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarFee2 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirVarFee2 > pstHisRunData->ulHisLastDayDirVarFee2)
					ulFee2=pstHisRunData->ulHisDirVarFee2 - pstHisRunData->ulHisLastDayDirVarFee2;
				else
					ulFee2=0;
			}
			if(pstHisRunData->ulHisDirVarFee3 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarFee3 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirVarFee3 > pstHisRunData->ulHisLastDayDirVarFee3)
					ulFee3=pstHisRunData->ulHisDirVarFee3 - pstHisRunData->ulHisLastDayDirVarFee3;
				else
					ulFee3=0;
			}
			if(pstHisRunData->ulHisDirVarFee4 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarFee4 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirVarFee4 > pstHisRunData->ulHisLastDayDirVarFee4)
					ulFee4=pstHisRunData->ulHisDirVarFee4 - pstHisRunData->ulHisLastDayDirVarFee4;
				else
					ulFee4=0;
			}
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			pucRetBuffer[m++]=4;
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
		}
		free_mem_block(mem_ptr);//
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//		if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_DIRVAR)
//		{
//			if(pstHisPulseRunData->ulHisPowTotal != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowTotal != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowTotal > pstHisPulseRunData->ulHisLastDayPowTotal)
//					ulFeeT=pstHisPulseRunData->ulHisPowTotal - pstHisPulseRunData->ulHisLastDayPowTotal;
//				else
//					ulFeeT=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee1 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee1 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee1 > pstHisPulseRunData->ulHisLastDayPowFee1)
//					ulFee1=pstHisPulseRunData->ulHisPowFee1 - pstHisPulseRunData->ulHisLastDayPowFee1;
//				else
//					ulFee1=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee2 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee2 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee2 > pstHisPulseRunData->ulHisLastDayPowFee2)
//					ulFee2=pstHisPulseRunData->ulHisPowFee2 - pstHisPulseRunData->ulHisLastDayPowFee2;
//				else
//					ulFee2=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee3 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee3 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee3 > pstHisPulseRunData->ulHisLastDayPowFee3)
//					ulFee3=pstHisPulseRunData->ulHisPowFee3 - pstHisPulseRunData->ulHisLastDayPowFee3;
//				else
//					ulFee3=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee4 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee4 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee4 > pstHisPulseRunData->ulHisLastDayPowFee4)
//					ulFee4=pstHisPulseRunData->ulHisPowFee4 - pstHisPulseRunData->ulHisLastDayPowFee4;
//				else
//					ulFee4=0;
//			}
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			if(pstHisRunData->ulHisDirVarTotal != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarTotal != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirVarTotal > pstHisRunData->ulHisLastDayDirVarTotal)
//					ulFeeT=pstHisRunData->ulHisDirVarTotal - pstHisRunData->ulHisLastDayDirVarTotal;
//				else
//					ulFeeT=0;
//			}
//			if(pstHisRunData->ulHisDirVarFee1 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarFee1 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirVarFee1 > pstHisRunData->ulHisLastDayDirVarFee1)
//					ulFee1=pstHisRunData->ulHisDirVarFee1 - pstHisRunData->ulHisLastDayDirVarFee1;
//				else
//					ulFee1=0;
//			}
//			if(pstHisRunData->ulHisDirVarFee2 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarFee2 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirVarFee2 > pstHisRunData->ulHisLastDayDirVarFee2)
//					ulFee2=pstHisRunData->ulHisDirVarFee2 - pstHisRunData->ulHisLastDayDirVarFee2;
//				else
//					ulFee2=0;
//			}
//			if(pstHisRunData->ulHisDirVarFee3 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarFee3 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirVarFee3 > pstHisRunData->ulHisLastDayDirVarFee3)
//					ulFee3=pstHisRunData->ulHisDirVarFee3 - pstHisRunData->ulHisLastDayDirVarFee3;
//				else
//					ulFee3=0;
//			}
//			if(pstHisRunData->ulHisDirVarFee4 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarFee4 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirVarFee4 > pstHisRunData->ulHisLastDayDirVarFee4)
//					ulFee4=pstHisRunData->ulHisDirVarFee4 - pstHisRunData->ulHisLastDayDirVarFee4;
//				else
//					ulFee4=0;
//			}
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}
//		free_mem_block(mem_ptr);//
//	}
	else
		return RET_ERROR;

	(*pusPos)=m;
	return ucRet;
}

//F7	�ն��ᷴ���й����������ܡ�����1~M��		
unsigned char Main_Get_ReqData_2_F7(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	unsigned long ulFeeT,ulFee1,ulFee2,ulFee3,ulFee4;
	STHISRUNDATA *pstHisRunData;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

	ulFeeT=CONINVALIDVAL;	ulFee1=CONINVALIDVAL;	ulFee2=CONINVALIDVAL;	
	ulFee3=CONINVALIDVAL;	ulFee4=CONINVALIDVAL;
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,24);
	pucRetBuffer[m+3]=4;					//����

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			if(pstHisRunData->ulHisRevPowTotal != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowTotal != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevPowTotal > pstHisRunData->ulHisLastDayRevPowTotal)
					ulFeeT=pstHisRunData->ulHisRevPowTotal - pstHisRunData->ulHisLastDayRevPowTotal;
				else
					ulFeeT=0;
			}
			if(pstHisRunData->ulHisRevPowFee1 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowFee1 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevPowFee1 > pstHisRunData->ulHisLastDayRevPowFee1)
					ulFee1=pstHisRunData->ulHisRevPowFee1 - pstHisRunData->ulHisLastDayRevPowFee1;
				else
					ulFee1=0;
			}
			if(pstHisRunData->ulHisRevPowFee2 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowFee2 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevPowFee2 > pstHisRunData->ulHisLastDayRevPowFee2)
					ulFee2=pstHisRunData->ulHisRevPowFee2 - pstHisRunData->ulHisLastDayRevPowFee2;
				else
					ulFee2=0;
			}
			if(pstHisRunData->ulHisRevPowFee3 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowFee3 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevPowFee3 > pstHisRunData->ulHisLastDayRevPowFee3)
					ulFee3=pstHisRunData->ulHisRevPowFee3 - pstHisRunData->ulHisLastDayRevPowFee3;
				else
					ulFee3=0;
			}
			if(pstHisRunData->ulHisRevPowFee4 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowFee4 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevPowFee4 > pstHisRunData->ulHisLastDayRevPowFee4)
					ulFee4=pstHisRunData->ulHisRevPowFee4 - pstHisRunData->ulHisLastDayRevPowFee4;
				else
					ulFee4=0;
			}
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			pucRetBuffer[m++]=4;
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
		}
		free_mem_block(mem_ptr);//
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//		if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_REVPOW)
//		{
//			if(pstHisPulseRunData->ulHisPowTotal != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowTotal != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowTotal > pstHisPulseRunData->ulHisLastDayPowTotal)
//					ulFeeT=pstHisPulseRunData->ulHisPowTotal - pstHisPulseRunData->ulHisLastDayPowTotal;
//				else
//					ulFeeT=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee1 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee1 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee1 > pstHisPulseRunData->ulHisLastDayPowFee1)
//					ulFee1=pstHisPulseRunData->ulHisPowFee1 - pstHisPulseRunData->ulHisLastDayPowFee1;
//				else
//					ulFee1=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee2 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee2 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee2 > pstHisPulseRunData->ulHisLastDayPowFee2)
//					ulFee2=pstHisPulseRunData->ulHisPowFee2 - pstHisPulseRunData->ulHisLastDayPowFee2;
//				else
//					ulFee2=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee3 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee3 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee3 > pstHisPulseRunData->ulHisLastDayPowFee3)
//					ulFee3=pstHisPulseRunData->ulHisPowFee3 - pstHisPulseRunData->ulHisLastDayPowFee3;
//				else
//					ulFee3=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee4 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee4 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee4 > pstHisPulseRunData->ulHisLastDayPowFee4)
//					ulFee4=pstHisPulseRunData->ulHisPowFee4 - pstHisPulseRunData->ulHisLastDayPowFee4;
//				else
//					ulFee4=0;
//			}
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			if(pstHisRunData->ulHisRevPowTotal != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowTotal != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevPowTotal > pstHisRunData->ulHisLastDayRevPowTotal)
//					ulFeeT=pstHisRunData->ulHisRevPowTotal - pstHisRunData->ulHisLastDayRevPowTotal;
//				else
//					ulFeeT=0;
//			}
//			if(pstHisRunData->ulHisRevPowFee1 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowFee1 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevPowFee1 > pstHisRunData->ulHisLastDayRevPowFee1)
//					ulFee1=pstHisRunData->ulHisRevPowFee1 - pstHisRunData->ulHisLastDayRevPowFee1;
//				else
//					ulFee1=0;
//			}
//			if(pstHisRunData->ulHisRevPowFee2 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowFee2 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevPowFee2 > pstHisRunData->ulHisLastDayRevPowFee2)
//					ulFee2=pstHisRunData->ulHisRevPowFee2 - pstHisRunData->ulHisLastDayRevPowFee2;
//				else
//					ulFee2=0;
//			}
//			if(pstHisRunData->ulHisRevPowFee3 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowFee3 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevPowFee3 > pstHisRunData->ulHisLastDayRevPowFee3)
//					ulFee3=pstHisRunData->ulHisRevPowFee3 - pstHisRunData->ulHisLastDayRevPowFee3;
//				else
//					ulFee3=0;
//			}
//			if(pstHisRunData->ulHisRevPowFee4 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowFee4 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevPowFee4 > pstHisRunData->ulHisLastDayRevPowFee4)
//					ulFee4=pstHisRunData->ulHisRevPowFee4 - pstHisRunData->ulHisLastDayRevPowFee4;
//				else
//					ulFee4=0;
//			}
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}
//		free_mem_block(mem_ptr);//
//	}
	else
		return RET_ERROR;

	(*pusPos)=m;
	return ucRet;
}

//F8	�ն��ᷴ���޹����������ܡ�����1~M��		
unsigned char Main_Get_ReqData_2_F8(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	unsigned long ulFeeT,ulFee1,ulFee2,ulFee3,ulFee4;
	STHISRUNDATA *pstHisRunData;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

	ulFeeT=CONINVALIDVAL;	ulFee1=CONINVALIDVAL;	ulFee2=CONINVALIDVAL;	
	ulFee3=CONINVALIDVAL;	ulFee4=CONINVALIDVAL;
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,24);
	pucRetBuffer[m+3]=4;					//����

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���		
		
		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			if(pstHisRunData->ulHisRevVarTotal != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarTotal != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevVarTotal > pstHisRunData->ulHisLastDayRevVarTotal)
					ulFeeT=pstHisRunData->ulHisRevVarTotal - pstHisRunData->ulHisLastDayRevVarTotal;
				else
					ulFeeT=0;
			}
			if(pstHisRunData->ulHisRevVarFee1 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarFee1 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevVarFee1 > pstHisRunData->ulHisLastDayRevVarFee1)
					ulFee1=pstHisRunData->ulHisRevVarFee1 - pstHisRunData->ulHisLastDayRevVarFee1;
				else
					ulFee1=0;
			}
			if(pstHisRunData->ulHisRevVarFee2 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarFee2 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevVarFee2 > pstHisRunData->ulHisLastDayRevVarFee2)
					ulFee2=pstHisRunData->ulHisRevVarFee2 - pstHisRunData->ulHisLastDayRevVarFee2;
				else
					ulFee2=0;
			}
			if(pstHisRunData->ulHisRevVarFee3 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarFee3 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevVarFee3 > pstHisRunData->ulHisLastDayRevVarFee3)
					ulFee3=pstHisRunData->ulHisRevVarFee3 - pstHisRunData->ulHisLastDayRevVarFee3;
				else
					ulFee3=0;
			}
			if(pstHisRunData->ulHisRevVarFee4 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarFee4 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevVarFee4 > pstHisRunData->ulHisLastDayRevVarFee4)
					ulFee4=pstHisRunData->ulHisRevVarFee4 - pstHisRunData->ulHisLastDayRevVarFee4;
				else
					ulFee4=0;
			}
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			pucRetBuffer[m++]=4;
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
		}
		free_mem_block(mem_ptr);//
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVVAR);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//		if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_REVVAR)
//		{
//			if(pstHisPulseRunData->ulHisPowTotal != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowTotal != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowTotal > pstHisPulseRunData->ulHisLastDayPowTotal)
//					ulFeeT=pstHisPulseRunData->ulHisPowTotal - pstHisPulseRunData->ulHisLastDayPowTotal;
//				else
//					ulFeeT=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee1 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee1 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee1 > pstHisPulseRunData->ulHisLastDayPowFee1)
//					ulFee1=pstHisPulseRunData->ulHisPowFee1 - pstHisPulseRunData->ulHisLastDayPowFee1;
//				else
//					ulFee1=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee2 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee2 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee2 > pstHisPulseRunData->ulHisLastDayPowFee2)
//					ulFee2=pstHisPulseRunData->ulHisPowFee2 - pstHisPulseRunData->ulHisLastDayPowFee2;
//				else
//					ulFee2=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee3 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee3 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee3 > pstHisPulseRunData->ulHisLastDayPowFee3)
//					ulFee3=pstHisPulseRunData->ulHisPowFee3 - pstHisPulseRunData->ulHisLastDayPowFee3;
//				else
//					ulFee3=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee4 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee4 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee4 > pstHisPulseRunData->ulHisLastDayPowFee4)
//					ulFee4=pstHisPulseRunData->ulHisPowFee4 - pstHisPulseRunData->ulHisLastDayPowFee4;
//				else
//					ulFee4=0;
//			}
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			if(pstHisRunData->ulHisRevVarTotal != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarTotal != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevVarTotal > pstHisRunData->ulHisLastDayRevVarTotal)
//					ulFeeT=pstHisRunData->ulHisRevVarTotal - pstHisRunData->ulHisLastDayRevVarTotal;
//				else
//					ulFeeT=0;
//			}
//			if(pstHisRunData->ulHisRevVarFee1 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarFee1 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevVarFee1 > pstHisRunData->ulHisLastDayRevVarFee1)
//					ulFee1=pstHisRunData->ulHisRevVarFee1 - pstHisRunData->ulHisLastDayRevVarFee1;
//				else
//					ulFee1=0;
//			}
//			if(pstHisRunData->ulHisRevVarFee2 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarFee2 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevVarFee2 > pstHisRunData->ulHisLastDayRevVarFee2)
//					ulFee2=pstHisRunData->ulHisRevVarFee2 - pstHisRunData->ulHisLastDayRevVarFee2;
//				else
//					ulFee2=0;
//			}
//			if(pstHisRunData->ulHisRevVarFee3 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarFee3 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevVarFee3 > pstHisRunData->ulHisLastDayRevVarFee3)
//					ulFee3=pstHisRunData->ulHisRevVarFee3 - pstHisRunData->ulHisLastDayRevVarFee3;
//				else
//					ulFee3=0;
//			}
//			if(pstHisRunData->ulHisRevVarFee4 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarFee4 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevVarFee4 > pstHisRunData->ulHisLastDayRevVarFee4)
//					ulFee4=pstHisRunData->ulHisRevVarFee4 - pstHisRunData->ulHisLastDayRevVarFee4;
//				else
//					ulFee4=0;
//			}
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}
//		free_mem_block(mem_ptr);//
//	}
	else
		return RET_ERROR;

	(*pusPos)=m;
	return ucRet;
}

//F9	�����ն���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��		
unsigned char Main_Get_ReqData_2_F9(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	unsigned char i;
//	unsigned long *pulTmp;
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,94);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;	//����

			m=Main_Get_ReqData_2_Dirkwh(pucRetBuffer,m,pstHisRunData);

		}
		free_mem_block(mem_ptr);//
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,0);
//		return RET_ERROR;
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���
//
//		ucRet=DataProc_GetData_TerDayFrozen(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;	//����
//
//			m=Main_Get_ReqData_2_Dirkwh(pucRetBuffer,m,pstHisRunData);
//
//		}
//		free_mem_block(mem_ptr);//
//	}
	else
		return RET_ERROR;

	(*pusPos)=m;
	return ucRet;
}

//F10	�����ն��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��		
unsigned char Main_Get_ReqData_2_F10(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	unsigned char i;
//	unsigned long *pulTmp;
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,94);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;		//����

			m=Main_Get_ReqData_2_Revkwh(pucRetBuffer,m,pstHisRunData);
		
		}
		free_mem_block(mem_ptr);//
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,0);
//		return RET_ERROR;
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���
//
//		ucRet=DataProc_GetData_TerDayFrozen(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;		//����
//
//			m=Main_Get_ReqData_2_Revkwh(pucRetBuffer,m,pstHisRunData);
//
//		}
//		free_mem_block(mem_ptr);//
//	}
	else
		return RET_ERROR;

	(*pusPos)=m;
	return ucRet;
}

//F11	�����ն���������/�޹��������������ʱ�䣨�ܡ�����1~M��		
unsigned char Main_Get_ReqData_2_F11(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
	unsigned char i;
	unsigned long *pulTmp;
	PSTDATETIME pstdatetimetemp;
	
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,79);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;	//����

			pulTmp = &pstHisRunData->ulHisDirPowMaxDemand;
			for(i=0;i<5;i++)
			{
				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
				pulTmp ++;
			}
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemand);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee1);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee2);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee3);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee4);
			pstdatetimetemp = &pstHisRunData->stHisDirPowMaxDemandTime;
			for(i=0;i<5;i++)
			{
				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
				pstdatetimetemp ++;
			}
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTime);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee1);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee2);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee3);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee4);
			pulTmp = &pstHisRunData->ulHisDirVarMaxDemand;
			for(i=0;i<5;i++)
			{
				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
				pulTmp ++;
			}
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemand);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee1);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee2);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee3);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee4);
			pstdatetimetemp = &pstHisRunData->stHisDirVarMaxDemandTime;
			for(i=0;i<5;i++)
			{
				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
				pstdatetimetemp ++;
			}
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTime);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee1);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee2);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee3);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee4);

		}
		free_mem_block(mem_ptr);//
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,0);
//		return RET_ERROR;
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//���������ݻ���
//
//		ucRet=DataProc_GetData_TerDayFrozen(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;	//����
//
//			pulTmp = &pstHisRunData->ulHisDirPowMaxDemand;
//			for(i=0;i<5;i++)
//			{
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
//				pulTmp ++;
//			}
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemand);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee1);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee2);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee3);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee4);
//			pstdatetimetemp = &pstHisRunData->stHisDirPowMaxDemandTime;
//			for(i=0;i<5;i++)
//			{
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
//				pstdatetimetemp ++;
//			}
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTime);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee1);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee2);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee3);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee4);
//			pulTmp = &pstHisRunData->ulHisDirVarMaxDemand;
//			for(i=0;i<5;i++)
//			{
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
//				pulTmp ++;
//			}
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemand);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee1);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee2);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee3);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee4);
//			pstdatetimetemp = &pstHisRunData->stHisDirVarMaxDemandTime;
//			for(i=0;i<5;i++)
//			{
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
//				pstdatetimetemp ++;
//			}
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTime);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee1);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee2);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee3);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee4);
//
//		}
//		free_mem_block(mem_ptr);//
//	}
	else
		return RET_ERROR;

	(*pusPos)=m;
	return ucRet;
}

//F12	�����ն��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��		
unsigned char Main_Get_ReqData_2_F12(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
	unsigned char i;
	unsigned long *pulTmp;
	PSTDATETIME pstdatetimetemp;
	
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,79);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			pulTmp = &pstHisRunData->ulHisRevPowMaxDemand;
			for(i=0;i<5;i++)
			{
				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
				pulTmp ++;
			}
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemand);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee1);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee2);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee3);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee4);
			pstdatetimetemp = &pstHisRunData->stHisRevPowMaxDemandTime;
			for(i=0;i<5;i++)
			{
				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
				pstdatetimetemp ++;
			}
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTime);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee1);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee2);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee3);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee4);
			pulTmp = &pstHisRunData->ulHisRevVarMaxDemand;
			for(i=0;i<5;i++)
			{
				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
				pulTmp ++;
			}
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemand);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee1);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee2);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee3);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee4);
			pstdatetimetemp = &pstHisRunData->stHisRevVarMaxDemandTime;
			for(i=0;i<5;i++)
			{
				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
				pstdatetimetemp ++;
			}
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTime);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee1);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee2);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee3);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee4);

		}
		free_mem_block(mem_ptr);//
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,0);
//		return RET_ERROR;
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_TerDayFrozen(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			pulTmp = &pstHisRunData->ulHisRevPowMaxDemand;
//			for(i=0;i<5;i++)
//			{
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
//				pulTmp ++;
//			}
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemand);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee1);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee2);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee3);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee4);
//			pstdatetimetemp = &pstHisRunData->stHisRevPowMaxDemandTime;
//			for(i=0;i<5;i++)
//			{
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
//				pstdatetimetemp ++;
//			}
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTime);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee1);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee2);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee3);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee4);
//			pulTmp = &pstHisRunData->ulHisRevVarMaxDemand;
//			for(i=0;i<5;i++)
//			{
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
//				pulTmp ++;
//			}
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemand);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee1);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee2);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee3);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee4);
//			pstdatetimetemp = &pstHisRunData->stHisRevVarMaxDemandTime;
//			for(i=0;i<5;i++)
//			{
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
//				pstdatetimetemp ++;
//			}
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTime);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee1);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee2);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee3);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee4);
//
//		}
//		free_mem_block(mem_ptr);//
//	}
	else
		return RET_ERROR;

	(*pusPos)=m;
	return ucRet;
}

//F17	�¶���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��		
unsigned char Main_Get_ReqData_2_F17(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;
//	unsigned char i;
//	unsigned long *pulTmp;
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	stTempTime.ucDay=28;
	memset(pucRetBuffer+m,0xEE,93);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=Main_Get_ReqData_2_Dirkwh(pucRetBuffer,m,pstHisRunData);
		}
		free_mem_block(mem_ptr);//
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;					//����
//			if(stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//			{
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowTotal);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee1);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee2);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee3);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee4);
//			}else
//			{
//				m+=25;
//			}
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//			ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_DIRVAR)
//			{
//				m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowTotal);
//				m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee1);
//				m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee2);
//				m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee3);
//				m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee4);
//			}else
//			{
//				m+=20;
//			}
//			m+=40;
//			ucRet=RET_SUCCESS;		//�ָ�
//		}else
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//			ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			if(ucRet==RET_SUCCESS)
//			{
//				m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//				stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//				m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//				pucRetBuffer[m++]=4;					//����
//				if(stPulsePara.ucAttr==CON_PULSEATTR_DIRVAR)
//				{
//					m+=25;
//					m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowTotal);
//					m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee1);
//					m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee2);
//					m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee3);
//					m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee4);
//					m+=40;
//				}else
//				{
//					m+=85;
//				}
//			}
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;					//����
//			m=Main_Get_ReqData_2_Dirkwh(pucRetBuffer,m,pstHisRunData);
//		}
//		free_mem_block(mem_ptr);//
//	}
	else
		return RET_ERROR;

	(*pusPos)=m;
	return ucRet;
}

//F18	�¶��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��		
unsigned char Main_Get_ReqData_2_F18(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
    unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;
//	unsigned char i;
//	unsigned long *pulTmp;
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	stTempTime.ucDay=28;
	memset(pucRetBuffer+m,0xEE,93);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=Main_Get_ReqData_2_Revkwh(pucRetBuffer,m,pstHisRunData);
		}
		free_mem_block(mem_ptr);//
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;					//����
//			if(stPulsePara.ucAttr==CON_PULSEATTR_REVPOW)
//			{
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowTotal);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee1);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee2);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee3);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee4);
//			}else
//			{
//				m+=25;
//			}
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVVAR);
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//			ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_REVVAR)
//			{
//				m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowTotal);
//				m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee1);
//				m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee2);
//				m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee3);
//				m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee4);
//			}else
//			{
//				m+=20;
//			}
//			m+=40;
//			ucRet=RET_SUCCESS;		//�ָ�
//		}else
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVVAR);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//			ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			if(ucRet==RET_SUCCESS)
//			{
//				m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//				stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//				m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//				pucRetBuffer[m++]=4;					//����
//				if(stPulsePara.ucAttr==CON_PULSEATTR_REVVAR)
//				{
//					m+=25;
//					m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowTotal);
//					m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee1);
//					m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee2);
//					m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee3);
//					m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee4);
//				m+=40;
//				}else
//				{
//					m+=85;
//				}
//			}
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;					//����
//			m=Main_Get_ReqData_2_Revkwh(pucRetBuffer,m,pstHisRunData);
//		}
//		free_mem_block(mem_ptr);//
//	}
	else
		return RET_ERROR;

	(*pusPos)=m;
	return ucRet;
}

//F19	�¶���������/�޹��������������ʱ�䣨�ܡ�����1~M��		
unsigned char Main_Get_ReqData_2_F19(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;
	unsigned char i;
	unsigned long *pulTmp;
	PSTDATETIME pstdatetimetemp;
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	stTempTime.ucDay=28;
	memset(pucRetBuffer+m,0xEE,78);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			pulTmp = &pstHisRunData->ulHisDirPowMaxDemand;
			for(i=0;i<5;i++)
			{
				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
				pulTmp ++;
			}
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemand);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee1);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee2);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee3);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee4);
			pstdatetimetemp = &pstHisRunData->stHisDirPowMaxDemandTime;
			for(i=0;i<5;i++)
			{
				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
				pstdatetimetemp ++;
			}
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTime);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee1);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee2);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee3);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee4);
			pulTmp = &pstHisRunData->ulHisDirVarMaxDemand;
			for(i=0;i<5;i++)
			{
				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
				pulTmp ++;
			}
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemand);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee1);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee2);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee3);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee4);
			pstdatetimetemp = &pstHisRunData->stHisDirVarMaxDemandTime;
			for(i=0;i<5;i++)
			{
				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
				pstdatetimetemp ++;
			}
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTime);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee1);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee2);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee3);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee4);
		}
		free_mem_block(mem_ptr);
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;					//����
//			if(stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//			{
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemand);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee1);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee2);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee3);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee4);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTime);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee1);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee2);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee3);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee4);
//			}else
//			{
//				m+=35;
//			}
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//			ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_DIRVAR)
//			{
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemand);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee1);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee2);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee3);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee4);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTime);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee1);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee2);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee3);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee4);
//			}else
//			{
//				m+=35;
//			}
//			ucRet=RET_SUCCESS;		//�ָ�
//		}else
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//			ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			if(ucRet==RET_SUCCESS)
//			{
//				m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//				stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//				m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//				pucRetBuffer[m++]=4;					//����
//				if(stPulsePara.ucAttr==CON_PULSEATTR_DIRVAR)
//				{
//					m+=35;
//					m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemand);
//					m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee1);
//					m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee2);
//					m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee3);
//					m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee4);
//					m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTime);
//					m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee1);
//					m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee2);
//					m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee3);
//					m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee4);
//				}else
//				{
//					m+=70;
//				}
//			}
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;					//����
//			pulTmp = &pstHisRunData->ulHisDirPowMaxDemand;
//			for(i=0;i<5;i++)
//			{
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
//				pulTmp ++;
//			}
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemand);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee1);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee2);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee3);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee4);
//			pstdatetimetemp = &pstHisRunData->stHisDirPowMaxDemandTime;
//			for(i=0;i<5;i++)
//			{
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
//				pstdatetimetemp ++;
//			}
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTime);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee1);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee2);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee3);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee4);
//			pulTmp = &pstHisRunData->ulHisDirVarMaxDemand;
//			for(i=0;i<5;i++)
//			{
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
//				pulTmp ++;
//			}
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemand);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee1);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee2);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee3);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee4);
//			pstdatetimetemp = &pstHisRunData->stHisDirVarMaxDemandTime;
//			for(i=0;i<5;i++)
//			{
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
//				pstdatetimetemp ++;
//			}
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTime);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee1);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee2);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee3);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee4);
//		}
//		free_mem_block(mem_ptr);
//	}
	else
		return RET_ERROR;

	(*pusPos)=m;
	return ucRet;
}

//F20	�¶��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��		
unsigned char Main_Get_ReqData_2_F20(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;
	unsigned char i;
	unsigned long *pulTmp;
	PSTDATETIME pstdatetimetemp;
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	stTempTime.ucDay=28;
	memset(pucRetBuffer+m,0xEE,78);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			pulTmp = &pstHisRunData->ulHisRevPowMaxDemand;
			for(i=0;i<5;i++)
			{
				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
				pulTmp ++;
			}
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemand);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee1);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee2);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee3);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee4);
			pstdatetimetemp = &pstHisRunData->stHisRevPowMaxDemandTime;
			for(i=0;i<5;i++)
			{
				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
				pstdatetimetemp ++;
			}
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTime);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee1);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee2);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee3);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee4);
			pulTmp = &pstHisRunData->ulHisRevVarMaxDemand;
			for(i=0;i<5;i++)
			{
				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
				pulTmp ++;
			}
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemand);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee1);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee2);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee3);
		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee4);
			pstdatetimetemp = &pstHisRunData->stHisRevVarMaxDemandTime;
			for(i=0;i<5;i++)
			{
				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
				pstdatetimetemp ++;
			}
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTime);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee1);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee2);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee3);
		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee4);

		}
		free_mem_block(mem_ptr);
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;					//����
//			if(stPulsePara.ucAttr==CON_PULSEATTR_REVPOW)
//			{
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemand);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee1);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee2);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee3);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee4);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTime);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee1);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee2);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee3);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee4);
//			}else
//			{
//				m+=35;
//			}
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVPOW);
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//			ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_REVVAR)
//			{
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemand);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee1);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee2);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee3);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee4);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTime);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee1);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee2);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee3);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee4);
//			}else
//			{
//				m+=35;
//			}
//		}else
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVVAR);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//			ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			if(ucRet==RET_SUCCESS)
//			{
//				m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//				stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//				m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//				pucRetBuffer[m++]=4;					//����
//				if(stPulsePara.ucAttr==CON_PULSEATTR_REVVAR)
//				{
//					m+=35;
//					m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemand);
//					m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee1);
//					m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee2);
//					m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee3);
//					m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee4);
//					m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTime);
//					m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee1);
//					m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee2);
//					m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee3);
//					m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee4);
//				}else
//				{
//					m+=70;
//				}
//			}
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;					//����
//			pulTmp = &pstHisRunData->ulHisRevPowMaxDemand;
//			for(i=0;i<5;i++)
//			{
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
//				pulTmp ++;
//			}
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemand);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee1);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee2);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee3);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee4);
//			pstdatetimetemp = &pstHisRunData->stHisRevPowMaxDemandTime;
//			for(i=0;i<5;i++)
//			{
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
//				pstdatetimetemp ++;
//			}
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTime);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee1);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee2);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee3);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee4);
//			pulTmp = &pstHisRunData->ulHisRevVarMaxDemand;
//			for(i=0;i<5;i++)
//			{
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,*pulTmp);
//				pulTmp ++;
//			}
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemand);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee1);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee2);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee3);
//		//	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee4);
//			pstdatetimetemp = &pstHisRunData->stHisRevVarMaxDemandTime;
//			for(i=0;i<5;i++)
//			{
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,*pstdatetimetemp);
//				pstdatetimetemp ++;
//			}
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTime);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee1);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee2);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee3);
//		//	m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee4);
//
//		}
//		free_mem_block(mem_ptr);
//	}
	else
		return RET_ERROR;

	(*pusPos)=m;
	return ucRet;
}

//F21	�¶��������й����������ܡ�����1~M��		
unsigned char Main_Get_ReqData_2_F21(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{//�����·�֮��
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned char ucFlag1;
	unsigned short m=(*pusPos);
	unsigned long ulFeeT,ulFee1,ulFee2,ulFee3,ulFee4;
	STHISRUNDATA *pstHisRunData;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

	ulFeeT=CONINVALIDVAL;	ulFee1=CONINVALIDVAL;	ulFee2=CONINVALIDVAL;	
	ulFee3=CONINVALIDVAL;	ulFee4=CONINVALIDVAL;
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	stTempTime.ucDay=28;
	memset(pucRetBuffer+m,0xEE,23);
	pucRetBuffer[m+2]=4;					//����

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucFlag1=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucFlag1==RET_SUCCESS)
		{
			if(pstHisRunData->ulHisDirPowTotal != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowTotal != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirPowTotal > pstHisRunData->ulHisLastDayDirPowTotal)
					ulFeeT=pstHisRunData->ulHisDirPowTotal - pstHisRunData->ulHisLastDayDirPowTotal;
				else
					ulFeeT=0;
			}
			if(pstHisRunData->ulHisDirPowFee1 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowFee1 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirPowFee1 > pstHisRunData->ulHisLastDayDirPowFee1)
					ulFee1=pstHisRunData->ulHisDirPowFee1 - pstHisRunData->ulHisLastDayDirPowFee1;
				else
					ulFee1=0;
			}
			if(pstHisRunData->ulHisDirPowFee2 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowFee2 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirPowFee2 > pstHisRunData->ulHisLastDayDirPowFee2)
					ulFee2=pstHisRunData->ulHisDirPowFee2 - pstHisRunData->ulHisLastDayDirPowFee2;
				else
					ulFee2=0;
			}
			if(pstHisRunData->ulHisDirPowFee3 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowFee3 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirPowFee3 > pstHisRunData->ulHisLastDayDirPowFee3)
					ulFee3=pstHisRunData->ulHisDirPowFee3 - pstHisRunData->ulHisLastDayDirPowFee3;
				else
					ulFee3=0;
			}
			if(pstHisRunData->ulHisDirPowFee4 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowFee4 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirPowFee4 > pstHisRunData->ulHisLastDayDirPowFee4)
					ulFee4=pstHisRunData->ulHisDirPowFee4 - pstHisRunData->ulHisLastDayDirPowFee4;
				else
					ulFee4=0;
			}
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			pucRetBuffer[m++]=4;
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
		}
		free_mem_block(mem_ptr);
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucFlag1=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//		if(ucFlag1==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//		{
//			if(pstHisPulseRunData->ulHisPowTotal != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowTotal != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowTotal > pstHisPulseRunData->ulHisLastDayPowTotal)
//					ulFeeT=pstHisPulseRunData->ulHisPowTotal - pstHisPulseRunData->ulHisLastDayPowTotal;
//				else
//					ulFeeT=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee1 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee1 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee1 > pstHisPulseRunData->ulHisLastDayPowFee1)
//					ulFee1=pstHisPulseRunData->ulHisPowFee1 - pstHisPulseRunData->ulHisLastDayPowFee1;
//				else
//					ulFee1=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee2 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee2 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee2 > pstHisPulseRunData->ulHisLastDayPowFee2)
//					ulFee2=pstHisPulseRunData->ulHisPowFee2 - pstHisPulseRunData->ulHisLastDayPowFee2;
//				else
//					ulFee2=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee3 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee3 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee3 > pstHisPulseRunData->ulHisLastDayPowFee3)
//					ulFee3=pstHisPulseRunData->ulHisPowFee3 - pstHisPulseRunData->ulHisLastDayPowFee3;
//				else
//					ulFee3=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee4 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee4 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee4 > pstHisPulseRunData->ulHisLastDayPowFee4)
//					ulFee4=pstHisPulseRunData->ulHisPowFee4 - pstHisPulseRunData->ulHisLastDayPowFee4;
//				else
//					ulFee4=0;
//			}
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucFlag1=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucFlag1==RET_SUCCESS)
//		{
//			if(pstHisRunData->ulHisDirPowTotal != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowTotal != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirPowTotal > pstHisRunData->ulHisLastDayDirPowTotal)
//					ulFeeT=pstHisRunData->ulHisDirPowTotal - pstHisRunData->ulHisLastDayDirPowTotal;
//				else
//					ulFeeT=0;
//			}
//			if(pstHisRunData->ulHisDirPowFee1 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowFee1 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirPowFee1 > pstHisRunData->ulHisLastDayDirPowFee1)
//					ulFee1=pstHisRunData->ulHisDirPowFee1 - pstHisRunData->ulHisLastDayDirPowFee1;
//				else
//					ulFee1=0;
//			}
//			if(pstHisRunData->ulHisDirPowFee2 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowFee2 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirPowFee2 > pstHisRunData->ulHisLastDayDirPowFee2)
//					ulFee2=pstHisRunData->ulHisDirPowFee2 - pstHisRunData->ulHisLastDayDirPowFee2;
//				else
//					ulFee2=0;
//			}
//			if(pstHisRunData->ulHisDirPowFee3 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowFee3 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirPowFee3 > pstHisRunData->ulHisLastDayDirPowFee3)
//					ulFee3=pstHisRunData->ulHisDirPowFee3 - pstHisRunData->ulHisLastDayDirPowFee3;
//				else
//					ulFee3=0;
//			}
//			if(pstHisRunData->ulHisDirPowFee4 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirPowFee4 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirPowFee4 > pstHisRunData->ulHisLastDayDirPowFee4)
//					ulFee4=pstHisRunData->ulHisDirPowFee4 - pstHisRunData->ulHisLastDayDirPowFee4;
//				else
//					ulFee4=0;
//			}
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}
//		free_mem_block(mem_ptr);
//	}
	else
		return RET_ERROR;

	if(ucFlag1!=RET_SUCCESS)
	{
		m+=23;
	}
	
	(*pusPos)=m;
	return ucRet;
}

//F22	�¶��������޹����������ܡ�����1~M��		
unsigned char Main_Get_ReqData_2_F22(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned char ucFlag1;
	unsigned short m=(*pusPos);
	unsigned long ulFeeT,ulFee1,ulFee2,ulFee3,ulFee4;
	STHISRUNDATA *pstHisRunData;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

	ulFeeT=CONINVALIDVAL;	ulFee1=CONINVALIDVAL;	ulFee2=CONINVALIDVAL;	
	ulFee3=CONINVALIDVAL;	ulFee4=CONINVALIDVAL;
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	stTempTime.ucDay=28;
	memset(pucRetBuffer+m,0xEE,23);
	pucRetBuffer[m+2]=4;					//����

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucFlag1=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucFlag1==RET_SUCCESS)
		{
			if(pstHisRunData->ulHisDirVarTotal != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarTotal != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirVarTotal > pstHisRunData->ulHisLastDayDirVarTotal)
					ulFeeT=pstHisRunData->ulHisDirVarTotal - pstHisRunData->ulHisLastDayDirVarTotal;
				else
					ulFeeT=0;
			}
			if(pstHisRunData->ulHisDirVarFee1 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarFee1 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirVarFee1 > pstHisRunData->ulHisLastDayDirVarFee1)
					ulFee1=pstHisRunData->ulHisDirVarFee1 - pstHisRunData->ulHisLastDayDirVarFee1;
				else
					ulFee1=0;
			}
			if(pstHisRunData->ulHisDirVarFee2 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarFee2 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirVarFee2 > pstHisRunData->ulHisLastDayDirVarFee2)
					ulFee2=pstHisRunData->ulHisDirVarFee2 - pstHisRunData->ulHisLastDayDirVarFee2;
				else
					ulFee2=0;
			}
			if(pstHisRunData->ulHisDirVarFee3 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarFee3 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirVarFee3 > pstHisRunData->ulHisLastDayDirVarFee3)
					ulFee3=pstHisRunData->ulHisDirVarFee3 - pstHisRunData->ulHisLastDayDirVarFee3;
				else
					ulFee3=0;
			}
			if(pstHisRunData->ulHisDirVarFee4 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarFee4 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisDirVarFee4 > pstHisRunData->ulHisLastDayDirVarFee4)
					ulFee4=pstHisRunData->ulHisDirVarFee4 - pstHisRunData->ulHisLastDayDirVarFee4;
				else
					ulFee4=0;
			}
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			pucRetBuffer[m++]=4;
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
		}
		free_mem_block(mem_ptr);
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucFlag1=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//		if(ucFlag1==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_DIRVAR)
//		{
//			if(pstHisPulseRunData->ulHisPowTotal != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowTotal != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowTotal > pstHisPulseRunData->ulHisLastDayPowTotal)
//					ulFeeT=pstHisPulseRunData->ulHisPowTotal - pstHisPulseRunData->ulHisLastDayPowTotal;
//				else
//					ulFeeT=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee1 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee1 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee1 > pstHisPulseRunData->ulHisLastDayPowFee1)
//					ulFee1=pstHisPulseRunData->ulHisPowFee1 - pstHisPulseRunData->ulHisLastDayPowFee1;
//				else
//					ulFee1=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee2 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee2 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee2 > pstHisPulseRunData->ulHisLastDayPowFee2)
//					ulFee2=pstHisPulseRunData->ulHisPowFee2 - pstHisPulseRunData->ulHisLastDayPowFee2;
//				else
//					ulFee2=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee3 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee3 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee3 > pstHisPulseRunData->ulHisLastDayPowFee3)
//					ulFee3=pstHisPulseRunData->ulHisPowFee3 - pstHisPulseRunData->ulHisLastDayPowFee3;
//				else
//					ulFee3=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee4 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee4 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee4 > pstHisPulseRunData->ulHisLastDayPowFee4)
//					ulFee4=pstHisPulseRunData->ulHisPowFee4 - pstHisPulseRunData->ulHisLastDayPowFee4;
//				else
//					ulFee4=0;
//			}
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucFlag1=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucFlag1==RET_SUCCESS)
//		{
//			if(pstHisRunData->ulHisDirVarTotal != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarTotal != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirVarTotal > pstHisRunData->ulHisLastDayDirVarTotal)
//					ulFeeT=pstHisRunData->ulHisDirVarTotal - pstHisRunData->ulHisLastDayDirVarTotal;
//				else
//					ulFeeT=0;
//			}
//			if(pstHisRunData->ulHisDirVarFee1 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarFee1 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirVarFee1 > pstHisRunData->ulHisLastDayDirVarFee1)
//					ulFee1=pstHisRunData->ulHisDirVarFee1 - pstHisRunData->ulHisLastDayDirVarFee1;
//				else
//					ulFee1=0;
//			}
//			if(pstHisRunData->ulHisDirVarFee2 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarFee2 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirVarFee2 > pstHisRunData->ulHisLastDayDirVarFee2)
//					ulFee2=pstHisRunData->ulHisDirVarFee2 - pstHisRunData->ulHisLastDayDirVarFee2;
//				else
//					ulFee2=0;
//			}
//			if(pstHisRunData->ulHisDirVarFee3 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarFee3 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirVarFee3 > pstHisRunData->ulHisLastDayDirVarFee3)
//					ulFee3=pstHisRunData->ulHisDirVarFee3 - pstHisRunData->ulHisLastDayDirVarFee3;
//				else
//					ulFee3=0;
//			}
//			if(pstHisRunData->ulHisDirVarFee4 != CONINVALIDVAL && pstHisRunData->ulHisLastDayDirVarFee4 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisDirVarFee4 > pstHisRunData->ulHisLastDayDirVarFee4)
//					ulFee4=pstHisRunData->ulHisDirVarFee4 - pstHisRunData->ulHisLastDayDirVarFee4;
//				else
//					ulFee4=0;
//			}
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}
//		free_mem_block(mem_ptr);
//	}
	else
		return RET_ERROR;

	if(ucFlag1!=RET_SUCCESS)
	{
		m+=23;
	}
	
	(*pusPos)=m;
	return ucRet;
}

//F23	�¶��ᷴ���й����������ܡ�����1~M��		
unsigned char Main_Get_ReqData_2_F23(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned char ucFlag1;
	unsigned short m=(*pusPos);
	unsigned long ulFeeT,ulFee1,ulFee2,ulFee3,ulFee4;
	STHISRUNDATA *pstHisRunData;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

	ulFeeT=CONINVALIDVAL;	ulFee1=CONINVALIDVAL;	ulFee2=CONINVALIDVAL;	
	ulFee3=CONINVALIDVAL;	ulFee4=CONINVALIDVAL;
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	stTempTime.ucDay=28;
	memset(pucRetBuffer+m,0xEE,23);
	pucRetBuffer[m+2]=4;					//����

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucFlag1=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucFlag1==RET_SUCCESS)
		{
			if(pstHisRunData->ulHisRevPowTotal != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowTotal != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevPowTotal > pstHisRunData->ulHisLastDayRevPowTotal)
					ulFeeT=pstHisRunData->ulHisRevPowTotal - pstHisRunData->ulHisLastDayRevPowTotal;
				else
					ulFeeT=0;
			}
			if(pstHisRunData->ulHisRevPowFee1 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowFee1 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevPowFee1 > pstHisRunData->ulHisLastDayRevPowFee1)
					ulFee1=pstHisRunData->ulHisRevPowFee1 - pstHisRunData->ulHisLastDayRevPowFee1;
				else
					ulFee1=0;
			}
			if(pstHisRunData->ulHisRevPowFee2 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowFee2 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevPowFee2 > pstHisRunData->ulHisLastDayRevPowFee2)
					ulFee2=pstHisRunData->ulHisRevPowFee2 - pstHisRunData->ulHisLastDayRevPowFee2;
				else
					ulFee2=0;
			}
			if(pstHisRunData->ulHisRevPowFee3 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowFee3 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevPowFee3 > pstHisRunData->ulHisLastDayRevPowFee3)
					ulFee3=pstHisRunData->ulHisRevPowFee3 - pstHisRunData->ulHisLastDayRevPowFee3;
				else
					ulFee3=0;
			}
			if(pstHisRunData->ulHisRevPowFee4 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowFee4 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevPowFee4 > pstHisRunData->ulHisLastDayRevPowFee4)
					ulFee4=pstHisRunData->ulHisRevPowFee4 - pstHisRunData->ulHisLastDayRevPowFee4;
				else
					ulFee4=0;
			}
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			pucRetBuffer[m++]=4;
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
		}
		free_mem_block(mem_ptr);
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucFlag1=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//		if(ucFlag1==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_REVPOW)
//		{
//			if(pstHisPulseRunData->ulHisPowTotal != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowTotal != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowTotal > pstHisPulseRunData->ulHisLastDayPowTotal)
//					ulFeeT=pstHisPulseRunData->ulHisPowTotal - pstHisPulseRunData->ulHisLastDayPowTotal;
//				else
//					ulFeeT=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee1 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee1 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee1 > pstHisPulseRunData->ulHisLastDayPowFee1)
//					ulFee1=pstHisPulseRunData->ulHisPowFee1 - pstHisPulseRunData->ulHisLastDayPowFee1;
//				else
//					ulFee1=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee2 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee2 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee2 > pstHisPulseRunData->ulHisLastDayPowFee2)
//					ulFee2=pstHisPulseRunData->ulHisPowFee2 - pstHisPulseRunData->ulHisLastDayPowFee2;
//				else
//					ulFee2=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee3 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee3 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee3 > pstHisPulseRunData->ulHisLastDayPowFee3)
//					ulFee3=pstHisPulseRunData->ulHisPowFee3 - pstHisPulseRunData->ulHisLastDayPowFee3;
//				else
//					ulFee3=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee4 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee4 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee4 > pstHisPulseRunData->ulHisLastDayPowFee4)
//					ulFee4=pstHisPulseRunData->ulHisPowFee4 - pstHisPulseRunData->ulHisLastDayPowFee4;
//				else
//					ulFee4=0;
//			}
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucFlag1=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucFlag1==RET_SUCCESS)
//		{
//			if(pstHisRunData->ulHisRevPowTotal != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowTotal != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevPowTotal > pstHisRunData->ulHisLastDayRevPowTotal)
//					ulFeeT=pstHisRunData->ulHisRevPowTotal - pstHisRunData->ulHisLastDayRevPowTotal;
//				else
//					ulFeeT=0;
//			}
//			if(pstHisRunData->ulHisRevPowFee1 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowFee1 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevPowFee1 > pstHisRunData->ulHisLastDayRevPowFee1)
//					ulFee1=pstHisRunData->ulHisRevPowFee1 - pstHisRunData->ulHisLastDayRevPowFee1;
//				else
//					ulFee1=0;
//			}
//			if(pstHisRunData->ulHisRevPowFee2 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowFee2 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevPowFee2 > pstHisRunData->ulHisLastDayRevPowFee2)
//					ulFee2=pstHisRunData->ulHisRevPowFee2 - pstHisRunData->ulHisLastDayRevPowFee2;
//				else
//					ulFee2=0;
//			}
//			if(pstHisRunData->ulHisRevPowFee3 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowFee3 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevPowFee3 > pstHisRunData->ulHisLastDayRevPowFee3)
//					ulFee3=pstHisRunData->ulHisRevPowFee3 - pstHisRunData->ulHisLastDayRevPowFee3;
//				else
//					ulFee3=0;
//			}
//			if(pstHisRunData->ulHisRevPowFee4 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevPowFee4 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevPowFee4 > pstHisRunData->ulHisLastDayRevPowFee4)
//					ulFee4=pstHisRunData->ulHisRevPowFee4 - pstHisRunData->ulHisLastDayRevPowFee4;
//				else
//					ulFee4=0;
//			}
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}
//		free_mem_block(mem_ptr);
//	}
	else
		return RET_ERROR;

	if(ucFlag1!=RET_SUCCESS)
	{
		m+=23;
	}
	
	(*pusPos)=m;
	return ucRet;
}

//F24	�¶��ᷴ���޹����������ܡ�����1~M��		
unsigned char Main_Get_ReqData_2_F24(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned char ucFlag1;
	unsigned short m=(*pusPos);
	unsigned long ulFeeT,ulFee1,ulFee2,ulFee3,ulFee4;
	STHISRUNDATA *pstHisRunData;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

	ulFeeT=CONINVALIDVAL;	ulFee1=CONINVALIDVAL;	ulFee2=CONINVALIDVAL;	
	ulFee3=CONINVALIDVAL;	ulFee4=CONINVALIDVAL;
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	stTempTime.ucDay=28;
	memset(pucRetBuffer+m,0xEE,23);
	pucRetBuffer[m+2]=4;					//����

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucFlag1=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucFlag1==RET_SUCCESS)
		{
			if(pstHisRunData->ulHisRevVarTotal != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarTotal != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevVarTotal > pstHisRunData->ulHisLastDayRevVarTotal)
					ulFeeT=pstHisRunData->ulHisRevVarTotal - pstHisRunData->ulHisLastDayRevVarTotal;
				else
					ulFeeT=0;
			}
			if(pstHisRunData->ulHisRevVarFee1 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarFee1 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevVarFee1 > pstHisRunData->ulHisLastDayRevVarFee1)
					ulFee1=pstHisRunData->ulHisRevVarFee1 - pstHisRunData->ulHisLastDayRevVarFee1;
				else
					ulFee1=0;
			}
			if(pstHisRunData->ulHisRevVarFee2 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarFee2 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevVarFee2 > pstHisRunData->ulHisLastDayRevVarFee2)
					ulFee2=pstHisRunData->ulHisRevVarFee2 - pstHisRunData->ulHisLastDayRevVarFee2;
				else
					ulFee2=0;
			}
			if(pstHisRunData->ulHisRevVarFee3 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarFee3 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevVarFee3 > pstHisRunData->ulHisLastDayRevVarFee3)
					ulFee3=pstHisRunData->ulHisRevVarFee3 - pstHisRunData->ulHisLastDayRevVarFee3;
				else
					ulFee3=0;
			}
			if(pstHisRunData->ulHisRevVarFee4 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarFee4 != CONINVALIDVAL)
			{
				if(pstHisRunData->ulHisRevVarFee4 > pstHisRunData->ulHisLastDayRevVarFee4)
					ulFee4=pstHisRunData->ulHisRevVarFee4 - pstHisRunData->ulHisLastDayRevVarFee4;
				else
					ulFee4=0;
			}
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			pucRetBuffer[m++]=4;
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
		}
		free_mem_block(mem_ptr);
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVVAR);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucFlag1=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//		if(ucFlag1==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_REVVAR)
//		{
//			if(pstHisPulseRunData->ulHisPowTotal != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowTotal != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowTotal > pstHisPulseRunData->ulHisLastDayPowTotal)
//					ulFeeT=pstHisPulseRunData->ulHisPowTotal - pstHisPulseRunData->ulHisLastDayPowTotal;
//				else
//					ulFeeT=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee1 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee1 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee1 > pstHisPulseRunData->ulHisLastDayPowFee1)
//					ulFee1=pstHisPulseRunData->ulHisPowFee1 - pstHisPulseRunData->ulHisLastDayPowFee1;
//				else
//					ulFee1=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee2 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee2 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee2 > pstHisPulseRunData->ulHisLastDayPowFee2)
//					ulFee2=pstHisPulseRunData->ulHisPowFee2 - pstHisPulseRunData->ulHisLastDayPowFee2;
//				else
//					ulFee2=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee3 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee3 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee3 > pstHisPulseRunData->ulHisLastDayPowFee3)
//					ulFee3=pstHisPulseRunData->ulHisPowFee3 - pstHisPulseRunData->ulHisLastDayPowFee3;
//				else
//					ulFee3=0;
//			}
//			if(pstHisPulseRunData->ulHisPowFee4 != CONINVALIDVAL && pstHisPulseRunData->ulHisLastDayPowFee4 != CONINVALIDVAL)
//			{
//				if(pstHisPulseRunData->ulHisPowFee4 > pstHisPulseRunData->ulHisLastDayPowFee4)
//					ulFee4=pstHisPulseRunData->ulHisPowFee4 - pstHisPulseRunData->ulHisLastDayPowFee4;
//				else
//					ulFee4=0;
//			}
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucFlag1=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucFlag1==RET_SUCCESS)
//		{
//			if(pstHisRunData->ulHisRevVarTotal != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarTotal != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevVarTotal > pstHisRunData->ulHisLastDayRevVarTotal)
//					ulFeeT=pstHisRunData->ulHisRevVarTotal - pstHisRunData->ulHisLastDayRevVarTotal;
//				else
//					ulFeeT=0;
//			}
//			if(pstHisRunData->ulHisRevVarFee1 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarFee1 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevVarFee1 > pstHisRunData->ulHisLastDayRevVarFee1)
//					ulFee1=pstHisRunData->ulHisRevVarFee1 - pstHisRunData->ulHisLastDayRevVarFee1;
//				else
//					ulFee1=0;
//			}
//			if(pstHisRunData->ulHisRevVarFee2 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarFee2 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevVarFee2 > pstHisRunData->ulHisLastDayRevVarFee2)
//					ulFee2=pstHisRunData->ulHisRevVarFee2 - pstHisRunData->ulHisLastDayRevVarFee2;
//				else
//					ulFee2=0;
//			}
//			if(pstHisRunData->ulHisRevVarFee3 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarFee3 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevVarFee3 > pstHisRunData->ulHisLastDayRevVarFee3)
//					ulFee3=pstHisRunData->ulHisRevVarFee3 - pstHisRunData->ulHisLastDayRevVarFee3;
//				else
//					ulFee3=0;
//			}
//			if(pstHisRunData->ulHisRevVarFee4 != CONINVALIDVAL && pstHisRunData->ulHisLastDayRevVarFee4 != CONINVALIDVAL)
//			{
//				if(pstHisRunData->ulHisRevVarFee4 > pstHisRunData->ulHisLastDayRevVarFee4)
//					ulFee4=pstHisRunData->ulHisRevVarFee4 - pstHisRunData->ulHisLastDayRevVarFee4;
//				else
//					ulFee4=0;
//			}
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}
//		free_mem_block(mem_ptr);
//	}
	else
		return RET_ERROR;

	if(ucFlag1!=RET_SUCCESS)
	{
		m+=23;
	}
	
	(*pusPos)=m;
	return ucRet;
}

//F25	�շ�������й����ʼ�����ʱ�䡢�й�����Ϊ��ʱ��		
unsigned char Main_Get_ReqData_2_F25(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,35);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPow.lVal);
			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPow.stDateTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPowA.lVal);
			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPowA.stDateTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPowB.lVal);
			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPowB.stDateTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPowC.lVal);
			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPowC.stDateTime);
			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisZeroTimeInstantPow),2);m+=2;
			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisZeroTimeInstantPowA),2);m+=2;
			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisZeroTimeInstantPowB),2);m+=2;
			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisZeroTimeInstantPowC),2);m+=2;
		}
		free_mem_block(mem_ptr);
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//		if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//		{//�й�����
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->stHisMaxInstantPow.lVal);
//			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisPulseRunData->stHisMaxInstantPow.stDateTime);
//			m+=18;				//���಻֧��
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisPulseRunData->usHisZeroTimeInstantPow),2);m+=2;
//			memset(pucRetBuffer+m,0x00,6);			//bin��ʽ��ЧӦ����д0x00��������0xEE 
//		}else
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVPOW);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//			ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_REVPOW)
//			{//�й�����
//				m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->stHisMaxInstantPow.lVal);
//				m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisPulseRunData->stHisMaxInstantPow.stDateTime);
//				m+=18;				//���಻֧��
//				memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisPulseRunData->usHisZeroTimeInstantPow),2);m+=2;
//				memset(pucRetBuffer+m,0x00,6);			//bin��ʽ��ЧӦ����д0x00��������0xEE 
//			}
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPow.lVal);
//			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPow.stDateTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPowA.lVal);
//			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPowA.stDateTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPowB.lVal);
//			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPowB.stDateTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPowC.lVal);
//			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPowC.stDateTime);
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisZeroTimeInstantPow),2);m+=2;
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisZeroTimeInstantPowA),2);m+=2;
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisZeroTimeInstantPowB),2);m+=2;
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisZeroTimeInstantPowC),2);m+=2;
//		}
//		free_mem_block(mem_ptr);
//	}
	else
		return RET_ERROR;

	(*pusPos)=m;
	return ucRet;
}

//F26	���ܼ������������������ʱ��		
unsigned char Main_Get_ReqData_2_F26(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,27);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxDemand.lVal);
			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxDemand.stDateTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxDemandA.lVal);
			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxDemandA.stDateTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxDemandB.lVal);
			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxDemandB.stDateTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxDemandC.lVal);
			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxDemandC.stDateTime);
		}
		free_mem_block(mem_ptr);
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//		if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//		{//�й��������޷���
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->stHisMaxDemand.lVal);
//			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisPulseRunData->stHisMaxDemand.stDateTime);
//			m+=18;
//		}else
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVPOW);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//			ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_REVPOW)
//			{//�й��������޷���
//				m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->stHisMaxDemand.lVal);
//				m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisPulseRunData->stHisMaxDemand.stDateTime);
//				m+=18;
//			}
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxDemand.lVal);
//			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxDemand.stDateTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxDemandA.lVal);
//			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxDemandA.stDateTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxDemandB.lVal);
//			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxDemandB.stDateTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxDemandC.lVal);
//			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxDemandC.stDateTime);
//		}
//		free_mem_block(mem_ptr);
//	}
	else
		return RET_ERROR;

	(*pusPos)=m;
	return ucRet;
}

//F27	�յ�ѹͳ������		
unsigned char Main_Get_ReqData_2_F27(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,69);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			m=Main_Get_ReqData_2_F27F35_Aux(m,pucRetBuffer,pstHisRunData);
		}
		free_mem_block(mem_ptr);
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,0);
//		return RET_ERROR;
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=Main_Get_ReqData_2_F27F35_Aux(m,pucRetBuffer,pstHisRunData);
//		}
//		free_mem_block(mem_ptr);
//	}
	else
		return RET_ERROR;

	(*pusPos)=m;
	return ucRet;
}

unsigned short Main_Get_ReqData_2_F27F35_Aux(unsigned short usPos,unsigned char *pucRetBuffer,STHISRUNDATA *pstHisRunData)
{
	unsigned short m=usPos;
	unsigned long ulTmp;
	memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisErrUpTimeVolA),2);	m+=2;
	memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisErrDownTimeVolA),2);	m+=2;
	memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisNorUpTimeVolA),2);	m+=2;
	memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisNorDownTimeVolA),2);	m+=2;
	memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisNormalTimeVolA),2);	m+=2;
	memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisErrUpTimeVolB),2);	m+=2;
	memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisErrDownTimeVolB),2);	m+=2;
	memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisNorUpTimeVolB),2);	m+=2;
	memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisNorDownTimeVolB),2);	m+=2;
	memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisNormalTimeVolB),2);	m+=2;
	memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisErrUpTimeVolC),2);	m+=2;
	memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisErrDownTimeVolC),2);	m+=2;
	memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisNorUpTimeVolC),2);	m+=2;
	memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisNorDownTimeVolC),2);	m+=2;
	memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisNormalTimeVolC),2);	m+=2;
	if(pstHisRunData->stHisMaxVolA.lVal!=CONINVALIDVAL)
		pstHisRunData->stHisMaxVolA.lVal *= 10;
	m=SetMainComm_Buf_lVal_2B(pucRetBuffer,m,pstHisRunData->stHisMaxVolA.lVal);
	m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxVolA.stDateTime);
	if(pstHisRunData->stHisMinVolA.lVal!=CONINVALIDVAL)
		pstHisRunData->stHisMinVolA.lVal *= 10;
	m=SetMainComm_Buf_lVal_2B(pucRetBuffer,m,pstHisRunData->stHisMinVolA.lVal);
	m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMinVolA.stDateTime);

	if(pstHisRunData->stHisMaxVolB.lVal!=CONINVALIDVAL)
		pstHisRunData->stHisMaxVolB.lVal *= 10;
	m=SetMainComm_Buf_lVal_2B(pucRetBuffer,m,pstHisRunData->stHisMaxVolB.lVal);
	m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxVolB.stDateTime);
	if(pstHisRunData->stHisMinVolB.lVal!=CONINVALIDVAL)
		pstHisRunData->stHisMinVolB.lVal *= 10;
	m=SetMainComm_Buf_lVal_2B(pucRetBuffer,m,pstHisRunData->stHisMinVolB.lVal);
	m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMinVolB.stDateTime);
	
	if(pstHisRunData->stHisMaxVolC.lVal!=CONINVALIDVAL)
		pstHisRunData->stHisMaxVolC.lVal *= 10;
	m=SetMainComm_Buf_lVal_2B(pucRetBuffer,m,pstHisRunData->stHisMaxVolC.lVal);
	m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxVolC.stDateTime);
	if(pstHisRunData->stHisMinVolC.lVal!=CONINVALIDVAL)
		pstHisRunData->stHisMinVolC.lVal *= 10;
	m=SetMainComm_Buf_lVal_2B(pucRetBuffer,m,pstHisRunData->stHisMinVolC.lVal);
	m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMinVolC.stDateTime);

	ulTmp=pstHisRunData->usHisNorUpTimeVolA+pstHisRunData->usHisNorDownTimeVolA+pstHisRunData->usHisNormalTimeVolA;
	if(ulTmp!=0)
		ulTmp=pstHisRunData->ulHisSumVolA*10/ulTmp;
	m=SetMainComm_Buf_lVal_2B(pucRetBuffer,m,ulTmp);
	ulTmp=pstHisRunData->usHisNorUpTimeVolB+pstHisRunData->usHisNorDownTimeVolB+pstHisRunData->usHisNormalTimeVolB;
	if(ulTmp!=0)
		ulTmp=pstHisRunData->ulHisSumVolB*10/ulTmp;
	m=SetMainComm_Buf_lVal_2B(pucRetBuffer,m,ulTmp);
	ulTmp=pstHisRunData->usHisNorUpTimeVolC+pstHisRunData->usHisNorDownTimeVolC+pstHisRunData->usHisNormalTimeVolC;
	if(ulTmp!=0)
		ulTmp=pstHisRunData->ulHisSumVolC*10/ulTmp;
	m=SetMainComm_Buf_lVal_2B(pucRetBuffer,m,ulTmp);
	return m;
}

//F28	�ղ�ƽ���Խ���ۼ�ʱ��		changed for test 09-11-25
unsigned char Main_Get_ReqData_2_F28(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	unsigned short usValTmp;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,17);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisUnbalanceTimeCurr),2);	m+=2;
			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisUnbalanceTimeVolt),2);	m+=2;
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,0);
//		return RET_ERROR;
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisUnbalanceTimeCurr),2);	m+=2;
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisUnbalanceTimeVolt),2);	m+=2;
//		}
//	}
	else
		return RET_ERROR;

	if(pstHisRunData->usHisUnbalanceTimeCurr == 0)
	{
		memset(pucRetBuffer+m,0x00,5);	m += 5;
	}
	else
	{
		usValTmp = (unsigned short)HEX_TO_BCD(pstHisRunData->stHisMaxUnBalanceCur.lVal, 2);
		memcpy(pucRetBuffer+m,&usValTmp,2);
		m += 2;
		m = SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxUnBalanceCur.stDateTime);
/*		pucRetBuffer[m++] = 0x00;
		pucRetBuffer[m++] = 0x01;
		pucRetBuffer[m++] = 0x09;
		pucRetBuffer[m++] = 0x12;
		pucRetBuffer[m++] = (unsigned char)HEX_TO_BCD(stTempTime.ucDay, 1);*/
	}
	if(pstHisRunData->usHisUnbalanceTimeVolt == 0)
	{
		memset(pucRetBuffer+m,0x00,5);	m += 5;
	}
	else
	{
		usValTmp = (unsigned short)HEX_TO_BCD(pstHisRunData->stHisMaxUnBalanceVol.lVal, 2);
		memcpy(pucRetBuffer+m,&usValTmp,2);
		m += 2;
		m = SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxUnBalanceVol.stDateTime);
/*		pucRetBuffer[m++] = 0x50;
		pucRetBuffer[m++] = 0x02;
		pucRetBuffer[m++] = 0x45;
		pucRetBuffer[m++] = 0x23;
		pucRetBuffer[m++] = (unsigned char)HEX_TO_BCD(stTempTime.ucDay, 1);*/
	}
    free_mem_block(mem_ptr);//
    
	(*pusPos)=m;
	return ucRet;
}

//F29	�յ���Խ��ͳ��		
unsigned char Main_Get_ReqData_2_F29(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,37);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			m=Main_Get_ReqData_2_F29F37_Aux(m,pucRetBuffer,pstHisRunData);
		}
		free_mem_block(mem_ptr);
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,0);
//		return RET_ERROR;
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=Main_Get_ReqData_2_F29F37_Aux(m,pucRetBuffer,pstHisRunData);
//		}
//		free_mem_block(mem_ptr);
//	}
	else
		return RET_ERROR;

	(*pusPos)=m;
	return ucRet;
}

unsigned short Main_Get_ReqData_2_F29F37_Aux(unsigned short usPos,unsigned char *pucRetBuffer,STHISRUNDATA *pstHisRunData)
{
	unsigned short m=usPos;
	memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisErrUpTimeCurrA),2);	m+=2;
	memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisNorUpTimeCurrA),2);	m+=2;
	memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisErrUpTimeCurrB),2);	m+=2;
	memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisNorUpTimeCurrB),2);	m+=2;
	memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisErrUpTimeCurrC),2);	m+=2;
	memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisNorUpTimeCurrC),2);	m+=2;
	memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisNorUpTimeCurrN),2);	m+=2;
	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxCurrA.lVal*10);
	m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxCurrA.stDateTime);
	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxCurrB.lVal*10);
	m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxCurrB.stDateTime);
	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxCurrC.lVal*10);
	m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxCurrC.stDateTime);
	memset(pucRetBuffer+m,0x00,6);
	m += 6;
	m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxCurrN.lVal*10);
	m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxCurrN.stDateTime);
	return m;
}

//F30	�����ڹ���Խ���ۼ�ʱ��		
unsigned char Main_Get_ReqData_2_F30(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,7);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisErrUpTimeSPower),2);	m+=2;
			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisNorUpTimeSPower),2);	m+=2;
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,0);
//		return RET_ERROR;
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisErrUpTimeSPower),2);	m+=2;
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisNorUpTimeSPower),2);	m+=2;
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);
	(*pusPos)=m;
	return ucRet;
}

//F31	�յ��ܱ��������		
unsigned char Main_Get_ReqData_2_F31(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,60);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
        
		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			m=SetMainComm_Buf_lVal_2B(pucRetBuffer,m,pstHisRunData->usHisVolBreakNum);
			m=SetMainComm_Buf_lVal_2B(pucRetBuffer,m,pstHisRunData->usHisVolABreakNum);
			m=SetMainComm_Buf_lVal_2B(pucRetBuffer,m,pstHisRunData->usHisVolBBreakNum);
			m=SetMainComm_Buf_lVal_2B(pucRetBuffer,m,pstHisRunData->usHisVolCBreakNum);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->usHisVolBreakTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->usHisVolABreakTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->usHisVolBBreakTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->usHisVolCBreakTime);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisLastVolBreakBegTime);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisLastVolABreakBegTime);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisLastVolBBreakBegTime);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisLastVolCBreakBegTime);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisLastVolBreakEndTime);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisLastVolABreakEndTime);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisLastVolBBreakEndTime);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisLastVolCBreakEndTime);
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,0);
//		return RET_ERROR;
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			m=SetMainComm_Buf_lVal_2B(pucRetBuffer,m,pstHisRunData->usHisVolBreakNum);
//			m=SetMainComm_Buf_lVal_2B(pucRetBuffer,m,pstHisRunData->usHisVolABreakNum);
//			m=SetMainComm_Buf_lVal_2B(pucRetBuffer,m,pstHisRunData->usHisVolBBreakNum);
//			m=SetMainComm_Buf_lVal_2B(pucRetBuffer,m,pstHisRunData->usHisVolCBreakNum);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->usHisVolBreakTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->usHisVolABreakTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->usHisVolBBreakTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->usHisVolCBreakTime);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisLastVolBreakBegTime);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisLastVolABreakBegTime);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisLastVolBBreakBegTime);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisLastVolCBreakBegTime);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisLastVolBreakEndTime);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisLastVolABreakEndTime);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisLastVolBBreakEndTime);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisLastVolCBreakEndTime);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);
	(*pusPos)=m;
	return ucRet;
}

//F33	�·�������й����ʼ�����ʱ�䡢�����й�����Ϊ��ʱ��		
unsigned char Main_Get_ReqData_2_F33(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	stTempTime.ucDay=28;
	memset(pucRetBuffer+m,0xEE,34);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPow.lVal);
			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPow.stDateTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPowA.lVal);
			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPowA.stDateTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPowB.lVal);
			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPowB.stDateTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPowC.lVal);
			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPowC.stDateTime);
			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisZeroTimeInstantPow),2);m+=2;
			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisZeroTimeInstantPowA),2);m+=2;
			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisZeroTimeInstantPowB),2);m+=2;
			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisZeroTimeInstantPowC),2);m+=2;
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//		if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//		{//�й�����
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->stHisMaxInstantPow.lVal);
//			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisPulseRunData->stHisMaxInstantPow.stDateTime);
//			m+=18;				//���಻֧��
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisPulseRunData->usHisZeroTimeInstantPow),2);m+=2;
//			memset(pucRetBuffer+m,0x00,6);			//bin��ʽ��ЧӦ����д0x00��������0xEE 
//		}else
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVPOW);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//			ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_REVPOW)
//			{//�й�����
//				m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->stHisMaxInstantPow.lVal);
//				m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisPulseRunData->stHisMaxInstantPow.stDateTime);
//				m+=18;				//���಻֧��
//				memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisPulseRunData->usHisZeroTimeInstantPow),2);m+=2;
//				memset(pucRetBuffer+m,0x00,6);			//bin��ʽ��ЧӦ����д0x00��������0xEE 
//			}
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPow.lVal);
//			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPow.stDateTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPowA.lVal);
//			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPowA.stDateTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPowB.lVal);
//			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPowB.stDateTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPowC.lVal);
//			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxInstantPowC.stDateTime);
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisZeroTimeInstantPow),2);m+=2;
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisZeroTimeInstantPowA),2);m+=2;
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisZeroTimeInstantPowB),2);m+=2;
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisZeroTimeInstantPowC),2);m+=2;
//		}
//	}   
    else
		return RET_ERROR;

    free_mem_block(mem_ptr);
	(*pusPos)=m;
	return ucRet;
}

//F34	�·����й��������������ʱ�䡢���й��������������ʱ��		
unsigned char Main_Get_ReqData_2_F34(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	stTempTime.ucDay=28;
	memset(pucRetBuffer+m,0xEE,26);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxDemand.lVal);
			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxDemand.stDateTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxDemandA.lVal);
			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxDemandA.stDateTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxDemandB.lVal);
			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxDemandB.stDateTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxDemandC.lVal);
			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxDemandC.stDateTime);
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//		if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//		{//�й��������޷���
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->stHisMaxDemand.lVal);
//			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisPulseRunData->stHisMaxDemand.stDateTime);
//			m+=18;
//		}else
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVPOW);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//			ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_REVPOW)
//			{//�й��������޷���
//				m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->stHisMaxDemand.lVal);
//				m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisPulseRunData->stHisMaxDemand.stDateTime);
//				m+=18;
//			}
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;
//
//		ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxDemand.lVal);
//			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxDemand.stDateTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxDemandA.lVal);
//			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxDemandA.stDateTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxDemandB.lVal);
//			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxDemandB.stDateTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->stHisMaxDemandC.lVal);
//			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisRunData->stHisMaxDemandC.stDateTime);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}

//F35	�µ�ѹͳ������		
unsigned char Main_Get_ReqData_2_F35(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	stTempTime.ucDay=28;
	memset(pucRetBuffer+m,0xEE,68);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			m=Main_Get_ReqData_2_F27F35_Aux(m,pucRetBuffer,pstHisRunData);
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,0);
//		return RET_ERROR;
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=Main_Get_ReqData_2_F27F35_Aux(m,pucRetBuffer,pstHisRunData);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);
	(*pusPos)=m;
	return ucRet;
}

//F36	�²�ƽ���Խ���ۼ�ʱ��		changed for test 09-11-25
unsigned char Main_Get_ReqData_2_F36(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	stTempTime.ucDay=28;
	memset(pucRetBuffer+m,0xEE,18);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisUnbalanceTimeCurr),2);	m+=2;
			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisUnbalanceTimeVolt),2);	m+=2;
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,0);
//		return RET_ERROR;
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;
//
//		ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisUnbalanceTimeCurr),2);	m+=2;
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisUnbalanceTimeVolt),2);	m+=2;
//		}
//	}
	else
		return RET_ERROR;
	
	if(pstHisRunData->usHisUnbalanceTimeCurr == 0)
	{
		memset(pucRetBuffer+m,0x00,6);	m += 6;
	}
	else
	{
		pucRetBuffer[m++] = 0x00;
		pucRetBuffer[m++] = 0x01;
		pucRetBuffer[m++] = 0x09;
		pucRetBuffer[m++] = 0x12;
		pucRetBuffer[m++] = 0x30;
		pucRetBuffer[m++] = (unsigned char)HEX_TO_BCD(stTempTime.ucMonth, 1);
	}
	if(pstHisRunData->usHisUnbalanceTimeVolt == 0)
	{
		memset(pucRetBuffer+m,0x00,6);	m += 6;
	}
	else
	{
		pucRetBuffer[m++] = 0x50;
		pucRetBuffer[m++] = 0x02;
		pucRetBuffer[m++] = 0x45;
		pucRetBuffer[m++] = 0x23;
		pucRetBuffer[m++] = 0x30;
		pucRetBuffer[m++] = (unsigned char)HEX_TO_BCD(stTempTime.ucMonth, 1);
	}

    free_mem_block(mem_ptr);
	(*pusPos)=m;
	return ucRet;
}

//F37	�µ���Խ��ͳ��		
unsigned char Main_Get_ReqData_2_F37(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	stTempTime.ucDay=28;
	memset(pucRetBuffer+m,0xEE,36);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			m=Main_Get_ReqData_2_F29F37_Aux(m,pucRetBuffer,pstHisRunData);
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,0);
//		return RET_ERROR;
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=Main_Get_ReqData_2_F29F37_Aux(m,pucRetBuffer,pstHisRunData);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);
	(*pusPos)=m;
	return ucRet;
}

//F38	�����ڹ���Խ���ۼ�ʱ��		
unsigned char Main_Get_ReqData_2_F38(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	stTempTime.ucDay=28;
	memset(pucRetBuffer+m,0xEE,6);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;	

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisErrUpTimeSPower),2);	m+=2;
			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisNorUpTimeSPower),2);	m+=2;
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,0);
//		return RET_ERROR;
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;	
//
//		ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisErrUpTimeSPower),2);	m+=2;
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisNorUpTimeSPower),2);	m+=2;
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);
	(*pusPos)=m;
	return ucRet;
}

//F43	�չ������������ۼ�ʱ��		
unsigned char Main_Get_ReqData_2_F43(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,9);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisPowRateSect1Time),2);	m+=2;
			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisPowRateSect2Time),2);	m+=2;
			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisPowRateSect3Time),2);	m+=2;
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,0);
//		return RET_ERROR;
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisPowRateSect1Time),2);	m+=2;
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisPowRateSect2Time),2);	m+=2;
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisPowRateSect3Time),2);	m+=2;
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);
	(*pusPos)=m;
	return ucRet;
}

//F44	�¹������������ۼ�ʱ��		
unsigned char Main_Get_ReqData_2_F44(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	stTempTime.ucDay=28;
	memset(pucRetBuffer+m,0xEE,8);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisPowRateSect1Time),2);	m+=2;
			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisPowRateSect2Time),2);	m+=2;
			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisPowRateSect3Time),2);	m+=2;
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,0);
//		return RET_ERROR;
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisPowRateSect1Time),2);	m+=2;
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisPowRateSect2Time),2);	m+=2;
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisPowRateSect3Time),2);	m+=2;
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}

//F49	�ն��չ���ʱ�䡢�ո�λ�ۼƴ���		
unsigned char Main_Get_ReqData_2_F49(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,7);

//	pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
    pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

	ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
	if(ucRet==RET_SUCCESS)
	{
		m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
		memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisTerPowerOnTime),2);	m+=2;
		memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisTerResetNum),2);		m+=2;
	}

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}

//F50	�ն��տ���ͳ������		
//unsigned char Main_Get_ReqData_2_F50(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
//{	
//    unsigned char *mem_ptr=NULL,mem_type;
//	unsigned char ucRet=RET_SUCCESS;
//	STDATETIME stTempTime;
//	unsigned short m=(*pusPos);
//	STHISRUNDATA *pstHisRunData;
//
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
//	memset(pucRetBuffer+m,0xEE,7);
//
////	pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//    if(mem_ptr == NULL)
//    {
//        return RET_ERROR;
//    }
//    pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//	ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//	if(ucRet==RET_SUCCESS)
//	{
//		m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//		pucRetBuffer[m++]=(unsigned char)(pstHisRunData->usHisMonthEnergyCtrlTripNum);
//		pucRetBuffer[m++]=(unsigned char)(pstHisRunData->usHisBuyEnergyCtrlTripNum);
//		pucRetBuffer[m++]=(unsigned char)(pstHisRunData->usHisPowerCtrlTripNum);
//		pucRetBuffer[m++]=(unsigned char)(pstHisRunData->usHisRemoteCtrlTripNum);
//	}
//
//    free_mem_block(mem_ptr);//
//	(*pusPos)=m;
//	return ucRet;
//}

//F51	�ն��¹���ʱ�䡢�¸�λ�ۼƴ���		
unsigned char Main_Get_ReqData_2_F51(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	stTempTime.ucDay=28;
	memset(pucRetBuffer+m,0xEE,6);

//	pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
    pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

	ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
	if(ucRet==RET_SUCCESS)
	{
		m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
		memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisTerPowerOnTime),2);	m+=2;
		memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisTerResetNum),2);		m+=2;
	}

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}

//F52	�ն��¿���ͳ������		
//unsigned char Main_Get_ReqData_2_F52(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
//{	
//    unsigned char *mem_ptr=NULL,mem_type;
//	unsigned char ucRet=RET_SUCCESS;
//	STDATETIME stTempTime;
//	unsigned short m=(*pusPos);
//	STHISRUNDATA *pstHisRunData;
//
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
//	stTempTime.ucDay=28;
//	memset(pucRetBuffer+m,0xEE,6);
//
////	pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//    if(mem_ptr == NULL)
//    {
//        return RET_ERROR;
//    }
//    pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//	ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//	if(ucRet==RET_SUCCESS)
//	{
//		m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//		pucRetBuffer[m++]=(unsigned char)(pstHisRunData->usHisMonthEnergyCtrlTripNum);
//		pucRetBuffer[m++]=(unsigned char)(pstHisRunData->usHisBuyEnergyCtrlTripNum);
//		pucRetBuffer[m++]=(unsigned char)(pstHisRunData->usHisPowerCtrlTripNum);
//		pucRetBuffer[m++]=(unsigned char)(pstHisRunData->usHisRemoteCtrlTripNum);
//	}
//
//    free_mem_block(mem_ptr);//
//	(*pusPos)=m;
//	return ucRet;
//}

//F53	�ն�����վ��ͨ������	
unsigned char Main_Get_ReqData_2_F53(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,7);

//	pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
    pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

	ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
	if(ucRet==RET_SUCCESS)
	{
		m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
		memcpy(pucRetBuffer+m,&(pstHisRunData->ulHisFluxBytes),4);
		m+=4;
	}
	ucRet=RET_SUCCESS;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}

//F54	�ն�����վ��ͨ������		
unsigned char Main_Get_ReqData_2_F54(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{	
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime,stCurrTime;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	unsigned long ulFluxBytes=0;

	stCurrTime=GetCurrentTime();

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	stTempTime.ucDay=28;
	memset(pucRetBuffer+m,0xEE,6);

//	pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
    pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

	ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
	if(ucRet==RET_SUCCESS)
	{
		m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
		ulFluxBytes=pstHisRunData->ulHisFluxBytes;
		if(stCurrTime.ucYear==stTempTime.ucYear && stCurrTime.ucMonth==stTempTime.ucMonth)
		{//�ӵ�������
			stTempTime=stCurrTime;
			stTempTime.ucSecond=59;
			stTempTime.ucMinute=59;
			stTempTime.ucHour=23;
			memset(pucRetBuffer+m,0xEE,7);
//			pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
            mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
            if(mem_ptr == NULL)
            {
                return RET_ERROR;
            }
            pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
            
			ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
			if(ucRet==RET_SUCCESS)
			{
				ulFluxBytes += pstHisRunData->ulHisFluxBytes;
			}
		}
		memcpy(pucRetBuffer+m,&ulFluxBytes,4);
		m+=4;
	}
	ucRet=RET_SUCCESS;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}

//F57	�ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��		
//unsigned char Main_Get_ReqData_2_F57(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
//{	
//	unsigned char ucRet=RET_SUCCESS;
//	STDATETIME stTempTime;
//	unsigned char ucSumGNo;
//	unsigned short m=(*pusPos);
//	unsigned short usDT2;
//	STHISSUMGROUPRUNDATA *pstHisSumGRunData;
//	ucSumGNo=ucDAp;
//	if(ucSumGNo>=CON_SUMGROUP_NUM)
//		return RET_ERROR;
//
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
//	memset(pucRetBuffer+m,0xEE,15);
//
//	pstHisSumGRunData=(STHISSUMGROUPRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//	ucRet=DataProc_GetSumGData_Day(ucSumGNo,stTempTime,pstHisSumGRunData);
//	if(ucRet==RET_SUCCESS)
//	{
//		m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//		if(pstHisSumGRunData->fSumGroupMaxInstantPow!=CONINVALIDVAL)
//			usDT2=Float_To_DT2(pstHisSumGRunData->fSumGroupMaxInstantPow/10000);
//		else
//			usDT2=0xEEEE;
//		pucRetBuffer[m++]=(unsigned char)usDT2;
//		pucRetBuffer[m++]=(unsigned char)(usDT2>>8);
//		m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisSumGRunData->stSumGroupMaxTimeInstantPow);
//		if(pstHisSumGRunData->fSumGroupMinInstantPow!=CONINVALIDVAL)
//			usDT2=Float_To_DT2(pstHisSumGRunData->fSumGroupMinInstantPow/10000);
//		else
//			usDT2=0xEEEE;
//		pucRetBuffer[m++]=(unsigned char)usDT2;
//		pucRetBuffer[m++]=(unsigned char)(usDT2>>8);
//		m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisSumGRunData->stSumGroupMinTimeInstantPow);
//		memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisSumGRunData->usSumGroupZeroTimeInstantPow),2);m+=2;
//	}
//	(*pusPos)=m;
//	return ucRet;
//}

//F58	�ܼ������ۼ��й����������ܡ�����1~M��		
//unsigned char Main_Get_ReqData_2_F58(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
//{	
//	unsigned char ucRet=RET_SUCCESS;
//	STDATETIME stTempTime;
//	unsigned char ucSumGNo;
//	unsigned char ucFlag1;
//	unsigned short m=(*pusPos);
//	float fFeeT,fFee1,fFee2,fFee3,fFee4;
//	unsigned long ulDT3;
//	STHISSUMGROUPRUNDATA *pstHisSumGRunData;
//	ucSumGNo=ucDAp;
//	if(ucSumGNo>=CON_SUMGROUP_NUM)
//		return RET_ERROR;
//	fFeeT=CONINVALIDVAL;	fFee1=CONINVALIDVAL;	fFee2=CONINVALIDVAL;	
//	fFee3=CONINVALIDVAL;	fFee4=CONINVALIDVAL;
//
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
//	memset(pucRetBuffer+m,0xEE,24);
//	pucRetBuffer[m+3]=4;					//����
//
//	pstHisSumGRunData=(STHISSUMGROUPRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//	ucFlag1=DataProc_GetSumGData_Day(ucSumGNo,stTempTime,pstHisSumGRunData);
//	if(ucFlag1==RET_SUCCESS)
//	{
//		if(pstHisSumGRunData->fSumGroupPowTotal != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayPowTotal != CONINVALIDVAL)
//		{
//			if(pstHisSumGRunData->fSumGroupPowTotal > pstHisSumGRunData->fSumGroupLastDayPowTotal)
//				fFeeT=pstHisSumGRunData->fSumGroupPowTotal - pstHisSumGRunData->fSumGroupLastDayPowTotal;
//			else
//				fFeeT=0;
//		}
//
//		if(pstHisSumGRunData->fSumGroupPowFee1 != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayPowFee1 != CONINVALIDVAL)
//		{
//			if(pstHisSumGRunData->fSumGroupPowFee1 > pstHisSumGRunData->fSumGroupLastDayPowFee1)
//				fFee1=pstHisSumGRunData->fSumGroupPowFee1 - pstHisSumGRunData->fSumGroupLastDayPowFee1;
//			else
//				fFee1=0;
//		}
//		if(pstHisSumGRunData->fSumGroupPowFee2 != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayPowFee2 != CONINVALIDVAL)
//		{
//			if(pstHisSumGRunData->fSumGroupPowFee2 > pstHisSumGRunData->fSumGroupLastDayPowFee2)
//				fFee2=pstHisSumGRunData->fSumGroupPowFee2 - pstHisSumGRunData->fSumGroupLastDayPowFee2;
//			else
//				fFee2=0;
//		}
//		if(pstHisSumGRunData->fSumGroupPowFee3 != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayPowFee3 != CONINVALIDVAL)
//		{
//			if(pstHisSumGRunData->fSumGroupPowFee3 > pstHisSumGRunData->fSumGroupLastDayPowFee3)
//				fFee3=pstHisSumGRunData->fSumGroupPowFee3 - pstHisSumGRunData->fSumGroupLastDayPowFee3;
//			else
//				fFee3=0;
//		}
//		if(pstHisSumGRunData->fSumGroupPowFee4 != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayPowFee4 != CONINVALIDVAL)
//		{
//			if(pstHisSumGRunData->fSumGroupPowFee4 > pstHisSumGRunData->fSumGroupLastDayPowFee4)
//				fFee4=pstHisSumGRunData->fSumGroupPowFee4 - pstHisSumGRunData->fSumGroupLastDayPowFee4;
//			else
//				fFee4=0;
//		}
//		m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//		pucRetBuffer[m++]=4;					//����
//		ulDT3=Float_To_DT3_Div100(fFeeT);
//		memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//		ulDT3=Float_To_DT3_Div100(fFee1);
//		memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//		ulDT3=Float_To_DT3_Div100(fFee2);
//		memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//		ulDT3=Float_To_DT3_Div100(fFee3);
//		memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//		ulDT3=Float_To_DT3_Div100(fFee4);
//		memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	}
//	(*pusPos)=m;
//	return ucRet;
//}

//F59	�ܼ������ۼ��޹����������ܡ�����1~M��		
//unsigned char Main_Get_ReqData_2_F59(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
//{	
//	unsigned char ucRet=RET_SUCCESS;
//	STDATETIME stTempTime;
//	unsigned char ucSumGNo;
//	unsigned char ucFlag1;
//	unsigned short m=(*pusPos);
//	float fFeeT,fFee1,fFee2,fFee3,fFee4;
//	unsigned long ulDT3;
//	STHISSUMGROUPRUNDATA *pstHisSumGRunData;
//	ucSumGNo=ucDAp;
//	if(ucSumGNo>=CON_SUMGROUP_NUM)
//		return RET_ERROR;
//	fFeeT=CONINVALIDVAL;	fFee1=CONINVALIDVAL;	fFee2=CONINVALIDVAL;	
//	fFee3=CONINVALIDVAL;	fFee4=CONINVALIDVAL;
//
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
//	memset(pucRetBuffer+m,0xEE,24);
//	pucRetBuffer[m+3]=4;					//����
//
//	pstHisSumGRunData=(STHISSUMGROUPRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//	ucFlag1=DataProc_GetSumGData_Day(ucSumGNo,stTempTime,pstHisSumGRunData);
//	if(ucFlag1==RET_SUCCESS)
//	{
//		if(pstHisSumGRunData->fSumGroupVarTotal != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayVarTotal != CONINVALIDVAL)
//		{
//			if(pstHisSumGRunData->fSumGroupVarTotal > pstHisSumGRunData->fSumGroupLastDayVarTotal)
//				fFeeT=pstHisSumGRunData->fSumGroupVarTotal - pstHisSumGRunData->fSumGroupLastDayVarTotal;
//			else
//				fFeeT=0;
//		}
//
//		if(pstHisSumGRunData->fSumGroupVarFee1 != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayVarFee1 != CONINVALIDVAL)
//		{
//			if(pstHisSumGRunData->fSumGroupVarFee1 > pstHisSumGRunData->fSumGroupLastDayVarFee1)
//				fFee1=pstHisSumGRunData->fSumGroupVarFee1 - pstHisSumGRunData->fSumGroupLastDayVarFee1;
//			else
//				fFee1=0;
//		}
//		if(pstHisSumGRunData->fSumGroupVarFee2 != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayVarFee2 != CONINVALIDVAL)
//		{
//			if(pstHisSumGRunData->fSumGroupVarFee2 > pstHisSumGRunData->fSumGroupLastDayVarFee2)
//				fFee2=pstHisSumGRunData->fSumGroupVarFee2 - pstHisSumGRunData->fSumGroupLastDayVarFee2;
//			else
//				fFee2=0;
//		}
//		if(pstHisSumGRunData->fSumGroupVarFee3 != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayVarFee3 != CONINVALIDVAL)
//		{
//			if(pstHisSumGRunData->fSumGroupVarFee3 > pstHisSumGRunData->fSumGroupLastDayVarFee3)
//				fFee3=pstHisSumGRunData->fSumGroupVarFee3 - pstHisSumGRunData->fSumGroupLastDayVarFee3;
//			else
//				fFee3=0;
//		}
//		if(pstHisSumGRunData->fSumGroupVarFee4 != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayVarFee4 != CONINVALIDVAL)
//		{
//			if(pstHisSumGRunData->fSumGroupVarFee4 > pstHisSumGRunData->fSumGroupLastDayVarFee4)
//				fFee4=pstHisSumGRunData->fSumGroupVarFee4 - pstHisSumGRunData->fSumGroupLastDayVarFee4;
//			else
//				fFee4=0;
//		}
//		m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//		pucRetBuffer[m++]=4;					//����
//		ulDT3=Float_To_DT3_Div100(fFeeT);
//		memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//		ulDT3=Float_To_DT3_Div100(fFee1);
//		memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//		ulDT3=Float_To_DT3_Div100(fFee2);
//		memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//		ulDT3=Float_To_DT3_Div100(fFee3);
//		memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//		ulDT3=Float_To_DT3_Div100(fFee4);
//		memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	}
//	(*pusPos)=m;
//	return ucRet;
//}

//F60	�ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��		
//unsigned char Main_Get_ReqData_2_F60(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
//{	
//	unsigned char ucRet=RET_SUCCESS;
//	STDATETIME stTempTime;
//	unsigned char ucSumGNo;
//	unsigned short m=(*pusPos);
//	unsigned short usDT2;
//	STHISSUMGROUPRUNDATA *pstHisSumGRunData;
//	ucSumGNo=ucDAp;
//	if(ucSumGNo>=CON_SUMGROUP_NUM)
//		return RET_ERROR;
//
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
//	stTempTime.ucDay=28;
//	memset(pucRetBuffer+m,0xEE,14);
//
//	pstHisSumGRunData=(STHISSUMGROUPRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//	ucRet=DataProc_GetSumGData_Month(ucSumGNo,stTempTime,pstHisSumGRunData);
//	if(ucRet==RET_SUCCESS)
//	{
//		m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//		if(pstHisSumGRunData->fSumGroupMaxInstantPow!=CONINVALIDVAL)
//			usDT2=Float_To_DT2(pstHisSumGRunData->fSumGroupMaxInstantPow/10000);
//		else
//			usDT2=0xEEEE;
//		pucRetBuffer[m++]=(unsigned char)usDT2;
//		pucRetBuffer[m++]=(unsigned char)(usDT2>>8);
//		m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisSumGRunData->stSumGroupMaxTimeInstantPow);
//		if(pstHisSumGRunData->fSumGroupMinInstantPow!=CONINVALIDVAL)
//			usDT2=Float_To_DT2(pstHisSumGRunData->fSumGroupMinInstantPow/10000);
//		else
//			usDT2=0xEEEE;
//		pucRetBuffer[m++]=(unsigned char)usDT2;
//		pucRetBuffer[m++]=(unsigned char)(usDT2>>8);
//		m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisSumGRunData->stSumGroupMinTimeInstantPow);
//		memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisSumGRunData->usSumGroupZeroTimeInstantPow),2);m+=2;
//	}
//	(*pusPos)=m;
//	return ucRet;
//}

//F61	�ܼ������ۼ��й����������ܡ�����1~M��
//unsigned char Main_Get_ReqData_2_F61(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
//{	
//	unsigned char ucRet=RET_SUCCESS;
//	STDATETIME stTempTime;
//	unsigned char ucSumGNo;
//	unsigned char ucFlag1;
//	unsigned short m=(*pusPos);
//	float fFeeT,fFee1,fFee2,fFee3,fFee4;
//	unsigned long ulDT3;
//	STHISSUMGROUPRUNDATA *pstHisSumGRunData;
//	ucSumGNo=ucDAp;
//	if(ucSumGNo>=CON_SUMGROUP_NUM)
//		return RET_ERROR;
//	fFeeT=CONINVALIDVAL;	fFee1=CONINVALIDVAL;	fFee2=CONINVALIDVAL;	
//	fFee3=CONINVALIDVAL;	fFee4=CONINVALIDVAL;
//
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
//	stTempTime.ucDay=28;
//	memset(pucRetBuffer+m,0xEE,23);
//	pucRetBuffer[m+2]=4;					//����
//
//	pstHisSumGRunData=(STHISSUMGROUPRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//	ucFlag1=DataProc_GetSumGData_Month(ucSumGNo,stTempTime,pstHisSumGRunData);
//	if(ucFlag1==RET_SUCCESS)
//	{
//		if(pstHisSumGRunData->fSumGroupPowTotal != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayPowTotal != CONINVALIDVAL)
//		{
//			if(pstHisSumGRunData->fSumGroupPowTotal > pstHisSumGRunData->fSumGroupLastDayPowTotal)
//				fFeeT=pstHisSumGRunData->fSumGroupPowTotal - pstHisSumGRunData->fSumGroupLastDayPowTotal;
//			else
//				fFeeT=0;
//		}
//
//		if(pstHisSumGRunData->fSumGroupPowFee1 != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayPowFee1 != CONINVALIDVAL)
//		{
//			if(pstHisSumGRunData->fSumGroupPowFee1 > pstHisSumGRunData->fSumGroupLastDayPowFee1)
//				fFee1=pstHisSumGRunData->fSumGroupPowFee1 - pstHisSumGRunData->fSumGroupLastDayPowFee1;
//			else
//				fFee1=0;
//		}
//		if(pstHisSumGRunData->fSumGroupPowFee2 != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayPowFee2 != CONINVALIDVAL)
//		{
//			if(pstHisSumGRunData->fSumGroupPowFee2 > pstHisSumGRunData->fSumGroupLastDayPowFee2)
//				fFee2=pstHisSumGRunData->fSumGroupPowFee2 - pstHisSumGRunData->fSumGroupLastDayPowFee2;
//			else
//				fFee2=0;
//		}
//		if(pstHisSumGRunData->fSumGroupPowFee3 != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayPowFee3 != CONINVALIDVAL)
//		{
//			if(pstHisSumGRunData->fSumGroupPowFee3 > pstHisSumGRunData->fSumGroupLastDayPowFee3)
//				fFee3=pstHisSumGRunData->fSumGroupPowFee3 - pstHisSumGRunData->fSumGroupLastDayPowFee3;
//			else
//				fFee3=0;
//		}
//		if(pstHisSumGRunData->fSumGroupPowFee4 != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayPowFee4 != CONINVALIDVAL)
//		{
//			if(pstHisSumGRunData->fSumGroupPowFee4 > pstHisSumGRunData->fSumGroupLastDayPowFee4)
//				fFee4=pstHisSumGRunData->fSumGroupPowFee4 - pstHisSumGRunData->fSumGroupLastDayPowFee4;
//			else
//				fFee4=0;
//		}
//		m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//		pucRetBuffer[m++]=4;					//����
//		ulDT3=Float_To_DT3_Div100(fFeeT);
//		memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//		ulDT3=Float_To_DT3_Div100(fFee1);
//		memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//		ulDT3=Float_To_DT3_Div100(fFee2);
//		memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//		ulDT3=Float_To_DT3_Div100(fFee3);
//		memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//		ulDT3=Float_To_DT3_Div100(fFee4);
//		memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	}
//	(*pusPos)=m;
//	return ucRet;
//}

//F62	�ܼ������ۼ��޹����������ܡ�����1~M��		
//unsigned char Main_Get_ReqData_2_F62(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
//{	
//	unsigned char ucRet=RET_SUCCESS;
//	STDATETIME stTempTime;
//	unsigned char ucSumGNo;
//	unsigned char ucFlag1;
//	unsigned short m=(*pusPos);
//	float fFeeT,fFee1,fFee2,fFee3,fFee4;
//	unsigned long ulDT3;
//	STHISSUMGROUPRUNDATA *pstHisSumGRunData;
//	ucSumGNo=ucDAp;
//	if(ucSumGNo>=CON_SUMGROUP_NUM)
//		return RET_ERROR;
//	fFeeT=CONINVALIDVAL;	fFee1=CONINVALIDVAL;	fFee2=CONINVALIDVAL;	
//	fFee3=CONINVALIDVAL;	fFee4=CONINVALIDVAL;
//
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
//	stTempTime.ucDay=28;
//	memset(pucRetBuffer+m,0xEE,23);
//	pucRetBuffer[m+2]=4;					//����
//
//	pstHisSumGRunData=(STHISSUMGROUPRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//	ucFlag1=DataProc_GetSumGData_Month(ucSumGNo,stTempTime,pstHisSumGRunData);
//	if(ucFlag1==RET_SUCCESS)
//	{
//		if(pstHisSumGRunData->fSumGroupVarTotal != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayVarTotal != CONINVALIDVAL)
//		{
//			if(pstHisSumGRunData->fSumGroupVarTotal > pstHisSumGRunData->fSumGroupLastDayVarTotal)
//				fFeeT=pstHisSumGRunData->fSumGroupVarTotal - pstHisSumGRunData->fSumGroupLastDayVarTotal;
//			else
//				fFeeT=0;
//		}
//
//		if(pstHisSumGRunData->fSumGroupVarFee1 != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayVarFee1 != CONINVALIDVAL)
//		{
//			if(pstHisSumGRunData->fSumGroupVarFee1 > pstHisSumGRunData->fSumGroupLastDayVarFee1)
//				fFee1=pstHisSumGRunData->fSumGroupVarFee1 - pstHisSumGRunData->fSumGroupLastDayVarFee1;
//			else
//				fFee1=0;
//		}
//		if(pstHisSumGRunData->fSumGroupVarFee2 != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayVarFee2 != CONINVALIDVAL)
//		{
//			if(pstHisSumGRunData->fSumGroupVarFee2 > pstHisSumGRunData->fSumGroupLastDayVarFee2)
//				fFee2=pstHisSumGRunData->fSumGroupVarFee2 - pstHisSumGRunData->fSumGroupLastDayVarFee2;
//			else
//				fFee2=0;
//		}
//		if(pstHisSumGRunData->fSumGroupVarFee3 != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayVarFee3 != CONINVALIDVAL)
//		{
//			if(pstHisSumGRunData->fSumGroupVarFee3 > pstHisSumGRunData->fSumGroupLastDayVarFee3)
//				fFee3=pstHisSumGRunData->fSumGroupVarFee3 - pstHisSumGRunData->fSumGroupLastDayVarFee3;
//			else
//				fFee3=0;
//		}
//		if(pstHisSumGRunData->fSumGroupVarFee4 != CONINVALIDVAL && pstHisSumGRunData->fSumGroupLastDayVarFee4 != CONINVALIDVAL)
//		{
//			if(pstHisSumGRunData->fSumGroupVarFee4 > pstHisSumGRunData->fSumGroupLastDayVarFee4)
//				fFee4=pstHisSumGRunData->fSumGroupVarFee4 - pstHisSumGRunData->fSumGroupLastDayVarFee4;
//			else
//				fFee4=0;
//		}
//		m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//		pucRetBuffer[m++]=4;					//����
//		ulDT3=Float_To_DT3_Div100(fFeeT);
//		memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//		ulDT3=Float_To_DT3_Div100(fFee1);
//		memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//		ulDT3=Float_To_DT3_Div100(fFee2);
//		memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//		ulDT3=Float_To_DT3_Div100(fFee3);
//		memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//		ulDT3=Float_To_DT3_Div100(fFee4);
//		memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	}
//	(*pusPos)=m;
//	return ucRet;
//}

//F65	�ܼ��鳬���ʶ�ֵ�����ۼ�ʱ�䡢���ۼƵ�����		
//unsigned char Main_Get_ReqData_2_F65(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
//{
//	unsigned char ucRet=RET_SUCCESS;
////	STDATETIME stTempTime;
//	unsigned char ucSumGNo;
//	unsigned short m=(*pusPos);
//	unsigned long ulDT3=0;
//	STHISSUMGROUPRUNDATA *pstHisSumGRunData;
//	ucSumGNo=ucDAp;
//	if(ucSumGNo>=CON_SUMGROUP_NUM)
//		return RET_ERROR;
//
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
//	stTempTime.ucDay=28;
//	memset(pucRetBuffer+m,0xEE,8);
//	memset(pucRetBuffer+m+2,0x00,2);
//
//	pstHisSumGRunData=(STHISSUMGROUPRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//	ucRet=DataProc_GetSumGData_Month(ucSumGNo,stTempTime,pstHisSumGRunData);
//	if(ucRet==RET_SUCCESS)
//	{
//		m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//		memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisSumGRunData->usSumGroupPowerOverTime),2);m+=2;
//		ulDT3=Float_To_DT3_Div100(pstHisSumGRunData->fSumGroupPowerOverAccValTotal);
//		memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	}
//	(*pusPos)=m;
//	return ucRet;
//}

//F66	�ܼ��鳬�µ�������ֵ�����ۼ�ʱ�䡢�ۼƵ�����		
//unsigned char Main_Get_ReqData_2_F66(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	STDATETIME stTempTime;
//	unsigned char ucSumGNo;
//	unsigned short m=(*pusPos);
//	unsigned long ulDT3=0;
//	STHISSUMGROUPRUNDATA *pstHisSumGRunData;
//	ucSumGNo=ucDAp;
//	if(ucSumGNo>=CON_SUMGROUP_NUM)
//		return RET_ERROR;
//
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
//	stTempTime.ucDay=28;
//	memset(pucRetBuffer+m,0xEE,8);
//	memset(pucRetBuffer+m+2,0x00,2);
//
//	pstHisSumGRunData=(STHISSUMGROUPRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//	ucRet=DataProc_GetSumGData_Month(ucSumGNo,stTempTime,pstHisSumGRunData);
//	if(ucRet==RET_SUCCESS)
//	{
//		m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//		memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisSumGRunData->usSumGroupEnergyOverTime),2);m+=2;
//		ulDT3=Float_To_DT3_Div100(pstHisSumGRunData->fSumGroupEnergyOverAccValTotal);
//		memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	}
//	(*pusPos)=m;
// 	return ucRet;
// }


//F73	�ܼ����й���������	
//unsigned char Main_Get_ReqData_2_F73(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
//{
//	unsigned char ucRet=RET_SUCCESS;	
//	STDATETIME stTmpTime;
//	unsigned char *pucBuffer;
//	unsigned char ucDataLen;
//	unsigned char i,ucM,ucPointNum;
//	unsigned char ucGroupNo,ucTaskNo=0xFF;
//	unsigned short m=(*pusPos),pusPosBak;
//	unsigned char ucDesBuf[4];
//	unsigned short usDT2=0;
//	unsigned short usDI;
//	float fVal=0;
//	
//	ucGroupNo = ucDAp;
//	if(ucGroupNo>=CON_SUMGROUP_NUM)
//		return RET_ERROR;
//	usDI=GetInstantPowDI_byGroupNo(ucGroupNo);
//	ucTaskNo=CON_GRTASK_SUMGROUP_INSTANTPOW_TASKNO;
//
//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;
//
//	stTmpTime=(*pstStartTime);
//	stTmpTime.ucSecond=0;
//	if(Is_DateTime_Availble(&stTmpTime)==RET_ERROR)
//		return RET_ERROR;
//	ucM=ucDataInter;
//	if((ucM>4) || (ucM==0))
//		return RET_ERROR;
//	ucM=1<<(ucM-1);							//�ܶȴ���ת��Ϊ�ܶ���ֵ
//	ucPointNum=Get_CurvePointNum_ByTime(pstStartTime,pstEndTime,ucM);
//	pusPosBak=m;
//	SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stTmpTime);
//	pucRetBuffer[m+5]=ucDataInter;
//	pucRetBuffer[m+6]=1;					//Ĭ��Ϊһ��
//	memset(pucRetBuffer+m+7,0xEE,2);		//���ݸ�ʽ02Ϊ2�ֽ�
//	ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stTmpTime,ucPointNum,ucM,&ucDataLen,usDI);	//��Ч������Ч���ݲ���
//	ucPointNum=pucBuffer[0];
//	if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
//	{
//		m+=7;								//���ݿ�ʼλ��
//		pucBuffer+=2;
//		for(i=0;i<ucPointNum;i++)
//		{
//			if((m+4)>448)					//���������
//				break;
//			memset(ucDesBuf,0,4);
//			memcpy(ucDesBuf,pucBuffer,ucDataLen);
//			pucBuffer+=ucDataLen;
//			if(ucDesBuf[0]!=CON_GRTASK_DATA_INVALID)
//			{
//				memcpy(&fVal,ucDesBuf,4);
//				fVal/=10000;					//�й�0.0001kW --�� kW
//				usDT2=Float_To_DT2(fVal);
//				pucRetBuffer[m]=(unsigned char)usDT2;
//				pucRetBuffer[m+1]=(unsigned char)(usDT2>>8);
//			}else
//				memset(pucRetBuffer+m,0xEE,2);
//			m+=2;
//		}
//		pucRetBuffer[pusPosBak+6]=i;			//ʵ�ʵ���
//		(*pusPos)=m;
//	}
// 	return ucRet;
// }

//F74	�ܼ����޹���������
//unsigned char Main_Get_ReqData_2_F74(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
//{
//	unsigned char ucRet=RET_SUCCESS;	
//	STDATETIME stTmpTime;
//	unsigned char *pucBuffer;
//	unsigned char ucDataLen;
//	unsigned char i,ucM,ucPointNum;
//	unsigned char ucGroupNo,ucTaskNo=0xFF;
//	unsigned short m=(*pusPos),pusPosBak;
//	unsigned char ucDesBuf[4];
//	unsigned short usDT2=0;
//	unsigned short usDI;
//	float fVal=0;
//	
//	ucGroupNo = ucDAp;
//	if(ucGroupNo>=CON_SUMGROUP_NUM)
//		return RET_ERROR;
//	usDI=GetInstantVarDI_byGroupNo(ucGroupNo);
//	ucTaskNo=CON_GRTASK_SUMGROUP_INSTANTVAR_TASKNO;
//
//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;
//
//	stTmpTime=(*pstStartTime);
//	stTmpTime.ucSecond=0;
//	if(Is_DateTime_Availble(&stTmpTime)==RET_ERROR)
//		return RET_ERROR;
//	ucM=ucDataInter;
//	if((ucM>4) || (ucM==0))
//		return RET_ERROR;
//	ucM=1<<(ucM-1);							//�ܶȴ���ת��Ϊ�ܶ���ֵ
//	ucPointNum=Get_CurvePointNum_ByTime(pstStartTime,pstEndTime,ucM);
//	pusPosBak=m;
//	SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stTmpTime);
//	pucRetBuffer[m+5]=ucDataInter;
//	pucRetBuffer[m+6]=1;					//Ĭ��Ϊһ��
//	memset(pucRetBuffer+m+7,0xEE,2);		//���ݸ�ʽ02Ϊ2�ֽ�
//	ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stTmpTime,ucPointNum,ucM,&ucDataLen,usDI);	//��Ч������Ч���ݲ���
//	ucPointNum=pucBuffer[0];
//	if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
//	{
//		m+=7;								//���ݿ�ʼλ��
//		pucBuffer+=2;
//		for(i=0;i<ucPointNum;i++)
//		{
//			if((m+4)>448)					//���������
//				break;
//			memset(ucDesBuf,0,4);
//			memcpy(ucDesBuf,pucBuffer,ucDataLen);
//			pucBuffer+=ucDataLen;
//			if(ucDesBuf[0]!=CON_GRTASK_DATA_INVALID)
//			{
//				memcpy(&fVal,ucDesBuf,4);
//				fVal/=100;						//�޹�0.01kvar --�� kvar
//				usDT2=Float_To_DT2(fVal);
//				pucRetBuffer[m]=(unsigned char)usDT2;
//				pucRetBuffer[m+1]=(unsigned char)(usDT2>>8);
//			}else
//				memset(pucRetBuffer+m,0xEE,2);
//			m+=2;
//		}
//		pucRetBuffer[pusPosBak+6]=i;			//ʵ�ʵ���
//		(*pusPos)=m;
//	}
//	return ucRet;
//}

//F75	�ܼ����й�����������	
//unsigned char Main_Get_ReqData_2_F75(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
//{
//	unsigned char ucRet=RET_SUCCESS;	
//	unsigned char ucGroupNo;
//	unsigned short usDI;
//	ucGroupNo = ucDAp;
//	if(ucGroupNo>=CON_SUMGROUP_NUM)
//		return RET_ERROR;
//	usDI=GetPowDI_byGroupNo(ucGroupNo);
//	ucRet=Main_Get_ReqData_2_SumGroup_Pow(ucGroupNo,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,usDI,CON_STATUS_YES);
//	return ucRet;
//}

//F76	�ܼ����޹�����������
//unsigned char Main_Get_ReqData_2_F76(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
//{
//	unsigned char ucRet=RET_SUCCESS;	
//	unsigned char ucGroupNo;
//	unsigned short usDI;
//	ucGroupNo = ucDAp;
//	if(ucGroupNo>=CON_SUMGROUP_NUM)
//		return RET_ERROR;
//	usDI=GetVarDI_byGroupNo(ucGroupNo);
//	ucRet=Main_Get_ReqData_2_SumGroup_Pow(ucGroupNo,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,usDI,CON_STATUS_NO);
//	return ucRet;
//}

//F81  ���й���������
unsigned char Main_Get_ReqData_2_F81(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_InstantPow(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPINSTANTPOW);
	return ucRet;
}

//F82  A���й���������
unsigned char Main_Get_ReqData_2_F82(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_InstantPow(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPINSTANTPOWA);
	return ucRet;
}

//F83  B���й���������
unsigned char Main_Get_ReqData_2_F83(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_InstantPow(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPINSTANTPOWB);
	return ucRet;
}

//F84  C���й���������
unsigned char Main_Get_ReqData_2_F84(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_InstantPow(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPINSTANTPOWC);
	return ucRet;
}

//F85  ���޹���������
unsigned char Main_Get_ReqData_2_F85(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_InstantPow(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPINSTANTVAR);
	return ucRet;
}

//F86  A���޹���������
unsigned char Main_Get_ReqData_2_F86(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_InstantPow(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPINSTANTVARA);
	return ucRet;
}

//F87  B���޹���������
unsigned char Main_Get_ReqData_2_F87(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_InstantPow(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPINSTANTVARB);
	return ucRet;
}

//F88  C���޹���������
unsigned char Main_Get_ReqData_2_F88(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_InstantPow(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPINSTANTVARB);
	return ucRet;
}

//F89	A���ѹ����	
unsigned char Main_Get_ReqData_2_F89(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_Volt(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPVOLA);
	return ucRet;
}

//F90	B���ѹ����	
unsigned char Main_Get_ReqData_2_F90(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_Volt(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPVOLB);
	return ucRet;
}

//F91	C���ѹ����	
unsigned char Main_Get_ReqData_2_F91(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_Volt(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPVOLC);
	return ucRet;
}

//F92	A���������	
unsigned char Main_Get_ReqData_2_F92(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_Curr(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPCURA);
	return ucRet;
}

//F93	B���������	
unsigned char Main_Get_ReqData_2_F93(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_Curr(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPCURB);
	return ucRet;
}

//F94	C���������	
unsigned char Main_Get_ReqData_2_F94(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_Curr(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPCURC);
	return ucRet;
}

//F95	�����������
unsigned char Main_Get_ReqData_2_F95(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=RET_ERROR;
	return ucRet;
}

//F97	�����й��ܵ�����		
unsigned char Main_Get_ReqData_2_F97(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;	
	ucRet=Main_Get_ReqData_2_PowVal(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPDIRPOWTOTAL);
	return ucRet;
}

//F98	�����޹��ܵ�����		
unsigned char Main_Get_ReqData_2_F98(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;	
	ucRet=Main_Get_ReqData_2_PowVal(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPDIRVARTOTAL);
	return ucRet;
}

//F99	�����й��ܵ�����		
unsigned char Main_Get_ReqData_2_F99(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;	
	ucRet=Main_Get_ReqData_2_PowVal(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPUNDIRPOWTOTAL);
	return ucRet;
}

//F100	�����޹��ܵ�����		
unsigned char Main_Get_ReqData_2_F100(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;	
	ucRet=Main_Get_ReqData_2_PowVal(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPUNDIRVARTOTAL);
	return ucRet;
}

//F101	�����й��ܵ���ʾֵ����
unsigned char Main_Get_ReqData_2_F101(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_PowViewData(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPDIRPOWTOTAL);
	return ucRet;
}

//F102	�����޹��ܵ���ʾֵ����
unsigned char Main_Get_ReqData_2_F102(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_PowViewData(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPDIRVARTOTAL);
	return ucRet;
}

//F103	�����й��ܵ���ʾֵ����
unsigned char Main_Get_ReqData_2_F103(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_PowViewData(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPUNDIRPOWTOTAL);
	return ucRet;
}

//F104	�����޹��ܵ���ʾֵ����
unsigned char Main_Get_ReqData_2_F104(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_PowViewData(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPUNDIRVARTOTAL);
	return ucRet;
}

//F105	�ܹ�������		
unsigned char Main_Get_ReqData_2_F105(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_PowRate(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPPOWRATE);
	return ucRet;
}

//F106	A�๦������		
unsigned char Main_Get_ReqData_2_F106(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_PowRate(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPPOWRATEA);
	return ucRet;
}

//F107	B�๦������		
unsigned char Main_Get_ReqData_2_F107(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_PowRate(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPPOWRATEB);
	return ucRet;
}

//F108	C�๦������		
unsigned char Main_Get_ReqData_2_F108(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_PowRate(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPPOWRATEC);
	return ucRet;
}

//F109	C�๦������		
unsigned char Main_Get_ReqData_2_F109(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;
//	ucRet=Main_Get_ReqData_2_Volt_Phase(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPPOWRATEC);
	return ucRet;
}

//F110	C�๦������		
unsigned char Main_Get_ReqData_2_F110(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;
//	ucRet=Main_Get_ReqData_2_Curr_Phase(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPPOWRATEC);
	return ucRet;
}


//F145	һ�����޹�����		
unsigned char Main_Get_ReqData_2_F145(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_PowViewData(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPVAR1);
	return ucRet;
}

//F146	�������޹�����		
unsigned char Main_Get_ReqData_2_F146(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_PowViewData(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPVAR2);
	return ucRet;
}

//F147	�������޹�����		
unsigned char Main_Get_ReqData_2_F147(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_PowViewData(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPVAR3);
	return ucRet;
}

//F148	�������޹�����		
unsigned char Main_Get_ReqData_2_F148(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_PowViewData(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPVAR4);
	return ucRet;
}
//azh 170923
//F219	��������������������ݵ�Ԫ��ʽ
unsigned char Main_Get_ReqData_2_F219(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_GroupCurveData(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,0x0000);

	return ucRet;
}
unsigned char Main_Get_ReqData_2_PhasePowData(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet = RET_SUCCESS,i=0;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	STHISRUNDATA *pstHisRunData;
	unsigned short m =(*pusPos);
	unsigned long *pulTmp;
	
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	if(usDI == CON_DI_TYPE_MPDIRPOWA ||usDI == CON_DI_TYPE_MPCOMVAR1A)
		memset(pucRetBuffer+m,0xEE,23);
	else
		memset(pucRetBuffer+m,0xEE,20);
		
	
	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			if(usDI == CON_DI_TYPE_MPDIRPOWA)
				pulTmp = &(pstHisRunData->ulHisDirPowA);
			else if(usDI == CON_DI_TYPE_MPUNDIRPOWA)
				pulTmp = &(pstHisRunData->ulHisUnDirPowA);
			else if(usDI == CON_DI_TYPE_MPCOMVAR1A)
				pulTmp = &(pstHisRunData->ulHisDirVarA);
			else
				pulTmp = &(pstHisRunData->ulHisUnDirVarA);
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			for(i=0;i<3;i++)
			{
				if(usDI == CON_DI_TYPE_MPDIRPOWA ||usDI == CON_DI_TYPE_MPCOMVAR1A)
				{
					m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,*pulTmp);
					pulTmp ++;
				}else
				{
					m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,*pulTmp);
					pulTmp ++;
				}
			}
		}
	}else
	{
		if(usDI == CON_DI_TYPE_MPDIRPOWA ||usDI == CON_DI_TYPE_MPCOMVAR1A)
			m += 23;
		else
			m += 20;		
	}

    free_mem_block(mem_ptr);//
	*pusPos = m;
	return ucRet;	
}
unsigned char Main_Get_ReqData_2_PhasePowMonData(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet = RET_SUCCESS,i=0;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	STHISRUNDATA *pstHisRunData;
	unsigned short m =(*pusPos);
	unsigned long *pulTmp;
	
//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	stTempTime.ucDay = 28;
	if(usDI == CON_DI_TYPE_MPDIRPOWA ||usDI == CON_DI_TYPE_MPCOMVAR1A)
		memset(pucRetBuffer+m,0xEE,22);
	else
		memset(pucRetBuffer+m,0xEE,19);
	
	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			if(usDI == CON_DI_TYPE_MPDIRPOWA)
				pulTmp = &(pstHisRunData->ulHisDirPowA);
			else if(usDI == CON_DI_TYPE_MPUNDIRPOWA)
				pulTmp = &(pstHisRunData->ulHisUnDirPowA);
			else if(usDI == CON_DI_TYPE_MPCOMVAR1A)
				pulTmp = &(pstHisRunData->ulHisDirVarA);
			else
				pulTmp = &(pstHisRunData->ulHisUnDirVarA);
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Tm
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			for(i=0;i<3;i++)
			{
				if(usDI == CON_DI_TYPE_MPDIRPOWA ||usDI == CON_DI_TYPE_MPCOMVAR1A)
				{
					m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,*pulTmp);
					pulTmp ++;
				}else
				{
					m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,*pulTmp);
					pulTmp ++;
				}
			}
		}
	}else
	{
		if(usDI == CON_DI_TYPE_MPDIRPOWA ||usDI == CON_DI_TYPE_MPCOMVAR1A)
			m += 22;
		else
			m += 19;
	}
	
    free_mem_block(mem_ptr);//
	*pusPos = m;
	return ucRet;	
}

//F161	�ն��������й�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F161(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,34);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowTotal);
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowFee1);
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowFee2);
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowFee3);
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowFee4);
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			if(stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//			{
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowTotal);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee1);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee2);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee3);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee4);
//			}
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//        
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowTotal);
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowFee1);
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowFee2);
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowFee3);
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowFee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}


//F162	�ն��������޹�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F162(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,29);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarTotal);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarFee1);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarFee2);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarFee3);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarFee4);
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			if(stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//			{
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowTotal);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee1);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee2);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee3);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee4);
//			}
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarTotal);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarFee1);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarFee2);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarFee3);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarFee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//    
	(*pusPos)=m;
	return ucRet;
}


//F163	�ն��ᷴ���й�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F163(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,34);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowTotal);
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowFee1);
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowFee2);
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowFee3);
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowFee4);
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			if(stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//			{
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowTotal);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee1);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee2);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee3);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee4);
//			}
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowTotal);
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowFee1);
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowFee2);
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowFee3);
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowFee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}


//F164	�ն��ᷴ���޹�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F164(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,29);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarTotal);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarFee1);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarFee2);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarFee3);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarFee4);
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVVAR);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			if(stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//			{
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowTotal);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee1);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee2);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee3);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee4);
//			}
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarTotal);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarFee1);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarFee2);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarFee3);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarFee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}


//F165	�ն���һ�����޹�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F165(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,29);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1Fee1);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1Fee2);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1Fee3);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1Fee4);
		}
	}	
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1Fee1);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1Fee2);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1Fee3);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1Fee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}


//F166	�ն���������޹�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F166(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,29);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2Fee1);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2Fee2);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2Fee3);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2Fee4);
		}
	}	
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//        
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2Fee1);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2Fee2);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2Fee3);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2Fee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}

//F167	�ն����������޹�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F167(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,29);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3Fee1);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3Fee2);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3Fee3);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3Fee4);
		}
	}	
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3Fee1);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3Fee2);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3Fee3);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3Fee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}

//F168	�ն����������޹�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F168(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,29);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4Fee1);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4Fee2);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4Fee3);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4Fee4);
		}
	}	
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//	
//	
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4Fee1);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4Fee2);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4Fee3);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4Fee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}


//F169	�����ն��������й�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F169(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,34);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowTotal);
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowFee1);
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowFee2);
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowFee3);
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowFee4);
		}
	}	
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_TerDayFrozen(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowTotal);
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowFee1);
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowFee2);
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowFee3);
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowFee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}


//F170	�����ն��������޹�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F170(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,29);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarTotal);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarFee1);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarFee2);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarFee3);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarFee4);
		}
	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_TerDayFrozen(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarTotal);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarFee1);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarFee2);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarFee3);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarFee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}


//F171	�����ն��ᷴ���й�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F171(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,34);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowTotal);
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowFee1);
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowFee2);
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowFee3);
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowFee4);
		}
	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_TerDayFrozen(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowTotal);
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowFee1);
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowFee2);
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowFee3);
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowFee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}


//F172	�����ն��ᷴ���޹�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F172(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,29);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarTotal);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarFee1);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarFee2);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarFee3);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarFee4);
		}
	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_TerDayFrozen(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarTotal);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarFee1);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarFee2);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarFee3);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarFee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}


//F173	�����ն���һ�����޹�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F173(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,29);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1Fee1);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1Fee2);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1Fee3);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1Fee4);
		}
	}	
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_TerDayFrozen(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1Fee1);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1Fee2);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1Fee3);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1Fee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}


//F174	�����ն���������޹�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F174(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,29);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2Fee1);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2Fee2);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2Fee3);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2Fee4);
		}
	}	
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_TerDayFrozen(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2Fee1);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2Fee2);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2Fee3);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2Fee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}

//F175	�����ն����������޹�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F175(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,29);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3Fee1);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3Fee2);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3Fee3);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3Fee4);
		}
	}	
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_TerDayFrozen(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3Fee1);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3Fee2);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3Fee3);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3Fee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}

//F176	�����ն����������޹�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F176(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,29);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4Fee1);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4Fee2);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4Fee3);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4Fee4);
		}
	}	
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_TerDayFrozen(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4Fee1);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4Fee2);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4Fee3);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4Fee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}


//F177	�¶��������й�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F177(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,33);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowTotal);
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowFee1);
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowFee2);
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowFee3);
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowFee4);
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			if(stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//			{
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowTotal);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee1);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee2);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee3);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee4);
//			}
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowTotal);
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowFee1);
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowFee2);
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowFee3);
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisDirPowFee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}


//F178	�¶��������޹�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F178(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,28);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarTotal);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarFee1);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarFee2);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarFee3);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarFee4);
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			if(stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//			{
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowTotal);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee1);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee2);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee3);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee4);
//			}
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarTotal);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarFee1);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarFee2);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarFee3);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVarFee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}


//F179	�¶��ᷴ���й�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F179(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,33);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowTotal);
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowFee1);
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowFee2);
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowFee3);
			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowFee4);
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			if(stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//			{
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowTotal);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee1);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee2);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee3);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee4);
//			}
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowTotal);
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowFee1);
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowFee2);
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowFee3);
//			m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisRunData->ulHisRevPowFee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}


//F180	�¶��ᷴ���޹�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F180(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,28);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarTotal);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarFee1);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarFee2);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarFee3);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarFee4);
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVVAR);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			if(stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//			{
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowTotal);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee1);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee2);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee3);
//				m=SetMainComm_Buf_lVal_5B_DT14(pucRetBuffer,m,pstHisPulseRunData->ulHisPowFee4);
//			}
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarTotal);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarFee1);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarFee2);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarFee3);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisRevVarFee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}


//F181	�¶���һ�����޹�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F181(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,28);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1Fee1);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1Fee2);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1Fee3);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1Fee4);
		}
	}	
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1Fee1);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1Fee2);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1Fee3);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar1Fee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}


//F182	�¶���������޹�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F182(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,28);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2Fee1);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2Fee2);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2Fee3);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2Fee4);
		}
	}	
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2Fee1);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2Fee2);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2Fee3);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar2Fee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}

//F183	�¶����������޹�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F183(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,28);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3Fee1);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3Fee2);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3Fee3);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3Fee4);
		}
	}	
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3Fee1);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3Fee2);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3Fee3);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar3Fee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}

//F184	�¶����������޹�����ʾֵ���ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F184(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,28);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4Fee1);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4Fee2);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4Fee3);
			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4Fee4);
		}
	}	
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4Fee1);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4Fee2);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4Fee3);
//			m=SetMainComm_Buf_lVal_4B(pucRetBuffer,m,pstHisRunData->ulHisDirVar4Fee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}



//F185	�ն��������й��������������ʱ�䣨�ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F185(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,44);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemand);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee1);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee1);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee2);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee2);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee3);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee3);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee4);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee4);
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			if(stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//			{
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemand);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTime);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee1);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee1);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee2);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee2);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee3);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee3);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee4);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee4);
//			}
//		}	
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemand);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee1);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee1);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee2);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee2);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee3);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee3);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee4);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}

//F186	�ն��������޹��������������ʱ�䣨�ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F186(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,44);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemand);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee1);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee1);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee2);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee2);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee3);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee3);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee4);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee4);
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			if(stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//			{
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemand);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTime);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee1);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee1);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee2);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee2);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee3);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee3);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee4);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee4);
//			}
//		}	
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemand);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee1);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee1);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee2);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee2);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee3);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee3);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee4);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}
//F187	�ն��ᷴ���й��������������ʱ�䣨�ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F187(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,44);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemand);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee1);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee1);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee2);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee2);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee3);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee3);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee4);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee4);
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			if(stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//			{
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemand);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTime);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee1);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee1);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee2);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee2);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee3);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee3);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee4);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee4);
//			}
//		}	
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemand);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee1);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee1);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee2);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee2);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee3);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee3);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee4);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}
//F188	�ն��ᷴ���޹��������������ʱ�䣨�ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F188(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,44);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemand);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee1);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee1);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee2);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee2);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee3);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee3);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee4);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee4);
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVVAR);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			if(stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//			{
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemand);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTime);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee1);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee1);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee2);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee2);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee3);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee3);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee4);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee4);
//			}
//		}	
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemand);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee1);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee1);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee2);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee2);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee3);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee3);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee4);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}


//F189	�����ն��������й��������������ʱ�䣨�ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F189(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,44);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemand);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee1);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee1);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee2);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee2);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee3);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee3);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee4);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee4);
		}
	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_TerDayFrozen(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemand);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee1);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee1);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee2);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee2);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee3);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee3);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee4);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}

//F190	�����ն��������޹��������������ʱ�䣨�ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F190(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,44);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemand);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee1);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee1);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee2);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee2);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee3);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee3);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee4);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee4);
		}
	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_TerDayFrozen(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemand);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee1);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee1);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee2);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee2);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee3);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee3);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee4);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}
//F191	�����ն��ᷴ���й��������������ʱ�䣨�ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F191(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,44);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemand);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee1);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee1);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee2);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee2);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee3);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee3);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee4);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee4);
		}
	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_TerDayFrozen(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemand);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee1);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee1);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee2);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee2);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee3);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee3);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee4);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}
//F192	�����ն��ᷴ���޹��������������ʱ�䣨�ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F192(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,44);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemand);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee1);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee1);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee2);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee2);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee3);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee3);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee4);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee4);
		}
	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_TerDayFrozen(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//����ʱ��Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemand);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee1);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee1);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee2);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee2);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee3);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee3);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee4);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}


//F193	�¶��������й��������������ʱ�䣨�ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F193(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,44);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Td  changed for test 09-11-25
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemand);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee1);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee1);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee2);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee2);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee3);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee3);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee4);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee4);
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Tdchanged for test 09-11-25
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			if(stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//			{
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemand);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTime);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee1);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee1);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee2);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee2);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee3);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee3);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee4);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee4);
//			}
//		}	
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Tdchanged for test 09-11-25
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemand);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee1);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee1);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee2);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee2);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee3);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee3);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirPowMaxDemandFee4);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirPowMaxDemandTimeFee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}

//F194	�¶��������޹��������������ʱ�䣨�ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F194(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,44);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Tdchanged for test 09-11-25
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemand);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee1);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee1);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee2);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee2);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee3);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee3);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee4);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee4);
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Tdchanged for test 09-11-25
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			if(stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//			{
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemand);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTime);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee1);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee1);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee2);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee2);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee3);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee3);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee4);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee4);
//			}
//		}	
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Tdchanged for test 09-11-25
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemand);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee1);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee1);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee2);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee2);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee3);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee3);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisDirVarMaxDemandFee4);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisDirVarMaxDemandTimeFee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}
//F195	�¶��ᷴ���й��������������ʱ�䣨�ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F195(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,44);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Tdchanged for test 09-11-25
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemand);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee1);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee1);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee2);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee2);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee3);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee3);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee4);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee4);
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVPOW);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Tdchanged for test 09-11-25
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			if(stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//			{
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemand);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTime);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee1);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee1);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee2);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee2);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee3);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee3);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee4);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee4);
//			}
//		}	
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Tdchanged for test 09-11-25
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemand);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee1);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee1);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee2);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee2);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee3);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee3);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevPowMaxDemandFee4);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevPowMaxDemandTimeFee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}
//F196	�¶��ᷴ���޹��������������ʱ�䣨�ܡ�����1~M��	
unsigned char Main_Get_ReqData_2_F196(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTempTime;
	unsigned char ucMeterNo,ucMPNo,ucMPType;
	unsigned short m=(*pusPos);
	STHISRUNDATA *pstHisRunData;
	STMETERPARA	 stMeterPara;
//	STHISPULSERUNDATA *pstHisPulseRunData;
//	STPULSEPARA  stPulsePara;

//	stTempTime=(*pstDataTime);
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
    format_day_time(&stTempTime, pstDataTime);
	memset(pucRetBuffer+m,0xEE,44);

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Tdchanged for test 09-11-25
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemand);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTime);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee1);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee1);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee2);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee2);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee3);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee3);
			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee4);
			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee4);
		}
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVVAR);
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Tdchanged for test 09-11-25
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			if(stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//			{
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemand);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTime);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee1);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee1);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee2);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee2);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee3);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee3);
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->ulHisPowMaxDemandFee4);
//				m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisPulseRunData->stHisPowMaxDemandTimeFee4);
//			}
//		}	
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//���������ݻ���
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//����ʱ��Tdchanged for test 09-11-25
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemand);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTime);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee1);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee1);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee2);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee2);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee3);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee3);
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisRunData->ulHisRevVarMaxDemandFee4);
//			m=SetMainComm_Buf_DateTime_MDHM(pucRetBuffer,m,pstHisRunData->stHisRevVarMaxDemandTimeFee4);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}


unsigned char Get_CurvePointNum_ByTime(STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucM)
{//���ݿ�ʼʱ��ͽ���ʱ���������ܶȣ�15���ӵı��������������ݵ���
	unsigned char ucPointNum;
	unsigned long ulStart,ulEnd,ulTmp;
	ulStart=TimeConver(pstStartTime,CON_TIMEUNIT_MIN);
	ulEnd=TimeConver(pstEndTime,CON_TIMEUNIT_MIN);
	if(ulStart<ulEnd)
	{
		ulTmp=(ulEnd-ulStart)/(15*ucM);	//15�����ܶȣ���һ��
		if(ulTmp>96)
			ucPointNum=96;
		else
			ucPointNum=(unsigned char)ulTmp;
	}else
	{
		ucPointNum=1;
	}
	return ucPointNum;
}

unsigned char Main_Get_ReqData_2_InstantPow(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI)
{//����
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;	
	STDATETIME stTmpTime;
	unsigned char *pucBuffer;
	unsigned char ucDataLen;
	unsigned char i,ucM,ucPointNum;
//	unsigned char ucMPNo,ucMeterNo,ucTaskNo=0xFF,ucMPType;
	unsigned char ucTaskNo=0xFF;
	unsigned short m=(*pusPos),pusPosBak;
	unsigned char ucDesBuf[4];
//	unsigned char ucPulseType;
	
//azh 170925	
    if(RET_ERROR == Main_Get_ReqData_2_Check(ucDAp))
    {
        return RET_ERROR;
    }    
//	ucMPNo=ucDAp;
//	ucMPType=RunPara_GetMPType(ucMPNo);
//	if(ucMPType==CON_MPTYPE_METER)
//	{
//		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
//		if(ucMeterNo>=CON_METER_NUM)
//			return RET_ERROR;
//		ucTaskNo=ucMeterNo;
//	}
////	else if(ucMPType==CON_MPTYPE_PULSE)
////	{
////		ucPulseType=RunPara_GetPulseType_ByDI(usDI);
////		if(ucPulseType!=CON_PULSEATTR_INVALID)
////		{
////			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,ucPulseType);
////			ucTaskNo=Task_GetTaskNo_byPulseNo(ucPulseNo);
////		}
////		else
////		{
////			ucTaskNo=CON_GRTASK_NUM;			//��Ч
////		}
////	}
////	else if(ucMPType==CON_MPTYPE_TER)
////		ucTaskNo=Task_GetTaskNo_Ter();
//	if(ucTaskNo>=CON_GRTASK_NUM)
//		return RET_ERROR;

//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;

	stTmpTime=(*pstStartTime);
	stTmpTime.ucSecond=0;
	if(Is_DateTime_Availble(&stTmpTime)==RET_ERROR)
		return RET_ERROR;
	ucM=ucDataInter;
	if((ucM>4) || (ucM==0))
		return RET_ERROR;
	ucM=1<<(ucM-1);							//�ܶȴ���ת��Ϊ�ܶ���ֵ
	ucPointNum=Get_CurvePointNum_ByTime(pstStartTime,pstEndTime,ucM);
	pusPosBak=m;
	SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stTmpTime);
	pucRetBuffer[m+5]=ucDataInter;
	pucRetBuffer[m+6]=1;					//Ĭ��Ϊһ��
	memset(pucRetBuffer+m+7,0xEE,3);		//���ݸ�ʽ09Ϊ3�ֽ�

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucBuffer=(unsigned char *)mem_ptr;			//���������� 
	           
	ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stTmpTime,ucPointNum,ucM,&ucDataLen,usDI);	//��Ч������Ч���ݲ���
	ucPointNum=pucBuffer[0];
	if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
	{
		m+=7;
		pucBuffer+=2;
		for(i=0;i<ucPointNum;i++)
		{
			if((m+4)>448)					//���������
				break;
			memset(ucDesBuf,0,4);
			memcpy(ucDesBuf,pucBuffer,ucDataLen);
			pucBuffer+=ucDataLen;
			if(ucDataLen==2)
			{
				if(ucDesBuf[0]==0xEE)
				{
					memset(pucRetBuffer+m,0xEE,3);
				}else
				{
					pucRetBuffer[m]=0;
					if(ucDesBuf[1]>0x79)
						ucDesBuf[1]=0x79;			//���ݸ�ʽ9��Ŀǰ���������ݣ����ֵ79.9999
					memcpy(pucRetBuffer+m+1,ucDesBuf,2);		//�޹�2�ֽڣ�0 ������ֽ�
				}
			}else
			{
				if(ucDesBuf[0]==0xEE)
				{
					memset(pucRetBuffer+m,0xEE,3);
				}else
				{
					if(ucDesBuf[2]>0x79)
						ucDesBuf[2]=0x79;			//���ݸ�ʽ9��Ŀǰ���������ݣ����ֵ79.9999
					memcpy(pucRetBuffer+m,ucDesBuf,3);			//�й�3�ֽ�
				}
			}
			m+=3;
		}
		pucRetBuffer[pusPosBak+6]=i;			//ʵ�ʵ���
		(*pusPos)=m;
	}

    free_mem_block(mem_ptr);//
	return ucRet;
}
unsigned char Main_Get_ReqData_2_Volt(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI)
{//��ѹ
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;	
	STDATETIME stTmpTime;
	unsigned char *pucBuffer;
	unsigned char ucDataLen;
	unsigned char i,ucM,ucPointNum;
//	unsigned char ucMPNo,ucMeterNo,ucTaskNo=0xFF,ucMPType;
	unsigned char ucTaskNo=0xFF;
	unsigned short m=(*pusPos),pusPosBak;
	unsigned long ulVal;

//azh 170925	
    if(RET_ERROR == Main_Get_ReqData_2_Check(ucDAp))
    {
        return RET_ERROR;
    }    
//	ucMPNo=ucDAp;
//	ucMPType=RunPara_GetMPType(ucMPNo);
//	if(ucMPType==CON_MPTYPE_METER)
//	{
//		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
//		if(ucMeterNo>=CON_METER_NUM)
//			return RET_ERROR;
//		ucTaskNo=ucMeterNo;
//	}
////	else if(ucMPType==CON_MPTYPE_TER)
////		ucTaskNo=Task_GetTaskNo_Ter();
//	if(ucTaskNo>=CON_GRTASK_NUM)
//		return RET_ERROR;

//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;

	stTmpTime=(*pstStartTime);
	stTmpTime.ucSecond=0;
	if(Is_DateTime_Availble(&stTmpTime)==RET_ERROR)
		return RET_ERROR;
	ucM=ucDataInter;
	if((ucM>4) || (ucM==0))
		return RET_ERROR;
	ucM=1<<(ucM-1);							//�ܶȴ���ת��Ϊ�ܶ���ֵ
	ucPointNum=Get_CurvePointNum_ByTime(pstStartTime,pstEndTime,ucM);
	pusPosBak=m;
	SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stTmpTime);
	pucRetBuffer[m+5]=ucDataInter;
	pucRetBuffer[m+6]=1;					//Ĭ��Ϊһ��
	memset(pucRetBuffer+m+7,0xEE,2);

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucBuffer=(unsigned char *)mem_ptr;			//����������        

	ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stTmpTime,ucPointNum,ucM,&ucDataLen,usDI);	//��Ч������Ч���ݲ���
	ucPointNum=pucBuffer[0];
	if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
	{
		m+=7;
		pucBuffer+=2;
		for(i=0;i<ucPointNum;i++)
		{
			if((m+4)>448)					//���������
				break;
			ulVal=0;
			memcpy((unsigned char *)&ulVal,pucBuffer,ucDataLen);
		//	if((unsigned char)ulVal!=0xEE)
		//		ulVal=(ulVal<<4);				//���ݸ�ʽ08ת��Ϊ���ݸ�ʽ07
			pucBuffer+=ucDataLen;
			memcpy(pucRetBuffer+m,(unsigned char *)&ulVal,2);
			m+=2;
		}
		pucRetBuffer[pusPosBak+6]=i;			//ʵ�ʵ���
		(*pusPos)=m;
	}

    free_mem_block(mem_ptr);//
	return ucRet;
}

unsigned char Main_Get_ReqData_2_Curr(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI)
{//����
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;	
	STDATETIME stTmpTime;
	unsigned char *pucBuffer;
	unsigned char ucDataLen;
	unsigned char i,ucM,ucPointNum;
//	unsigned char ucMPNo,ucMeterNo,ucTaskNo=0xFF,ucMPType;
	unsigned char ucTaskNo=0xFF;
	unsigned short m=(*pusPos),pusPosBak;
	unsigned long ulVal;
	
//azh 170925	
    if(RET_ERROR == Main_Get_ReqData_2_Check(ucDAp))
    {
        return RET_ERROR;
    }    
//	ucMPNo=ucDAp;
//	ucMPType=RunPara_GetMPType(ucMPNo);
//	if(ucMPType==CON_MPTYPE_METER)
//	{
//		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
//		if(ucMeterNo>=CON_METER_NUM)
//			return RET_ERROR;
//		ucTaskNo=ucMeterNo;
//	}
////	else if(ucMPType==CON_MPTYPE_TER)
////		ucTaskNo=Task_GetTaskNo_Ter();
//	if(ucTaskNo>=CON_GRTASK_NUM)
//		return RET_ERROR;

//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;

	stTmpTime=(*pstStartTime);
	stTmpTime.ucSecond=0;
	if(Is_DateTime_Availble(&stTmpTime)==RET_ERROR)
		return RET_ERROR;
	ucM=ucDataInter;
	if((ucM>4) || (ucM==0))
		return RET_ERROR;
	ucM=1<<(ucM-1);							//�ܶȴ���ת��Ϊ�ܶ���ֵ
	ucPointNum=Get_CurvePointNum_ByTime(pstStartTime,pstEndTime,ucM);
	pusPosBak=m;
	SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stTmpTime);
	pucRetBuffer[m+5]=ucDataInter;
	pucRetBuffer[m+6]=1;					//Ĭ��Ϊһ��
	memset(pucRetBuffer+m+7,0xEE,2);

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucBuffer=(unsigned char *)mem_ptr;			//����������     

	ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stTmpTime,ucPointNum,ucM,&ucDataLen,usDI);	//��Ч������Ч���ݲ���
	ucPointNum=pucBuffer[0];
	if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
	{
		m+=7;
		pucBuffer+=2;
		for(i=0;i<ucPointNum;i++)
		{
			if((m+4)>448)					//���������
				break;
			ulVal=0;
			memcpy((unsigned char *)&ulVal,pucBuffer,ucDataLen);
			pucBuffer+=ucDataLen;
			memcpy(pucRetBuffer+m,(unsigned char *)&ulVal,2);
			m+=2;
		}
		pucRetBuffer[pusPosBak+6]=i;			//ʵ�ʵ���
		(*pusPos)=m;
	}
    free_mem_block(mem_ptr);//
	return ucRet;
}

/*
unsigned char Main_Get_ReqData_2_Volt_Phase(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI)
{//��ѹ���
	unsigned char ucRet=RET_SUCCESS;	
	STDATETIME stTmpTime;
	unsigned char *pucBuffer;
	unsigned char ucDataLen;
	unsigned char i,ucM,ucPointNum;
	unsigned char ucMPNo,ucMeterNo,ucTaskNo=0xFF,ucMPType;
	unsigned short m=(*pusPos),pusPosBak;
	unsigned long ulVal;
	
	ucMPNo=ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
		if(ucMeterNo>=CON_METER_NUM)
			return RET_ERROR;
		ucTaskNo=ucMeterNo;
	}else if(ucMPType==CON_MPTYPE_TER)
		ucTaskNo=Task_GetTaskNo_Ter();
	if(ucTaskNo>=CON_GRTASK_NUM)
		return RET_ERROR;

	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;

	stTmpTime=(*pstStartTime);
	stTmpTime.ucSecond=0;
	if(Is_DateTime_Availble(&stTmpTime)==RET_ERROR)
		return RET_ERROR;
	ucM=ucDataInter;
	if((ucM>4) || (ucM==0))
		return RET_ERROR;
	ucM=1<<(ucM-1);							//�ܶȴ���ת��Ϊ�ܶ���ֵ
	ucPointNum=Get_CurvePointNum_ByTime(pstStartTime,pstEndTime,ucM);
	pusPosBak=m;
	SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stTmpTime);
	pucRetBuffer[m+5]=ucDataInter;
	pucRetBuffer[m+6]=1;					//Ĭ��Ϊһ��
	memset(pucRetBuffer+m+7,0xEE,2);
	ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stTmpTime,ucPointNum,ucM,&ucDataLen,usDI);	//��Ч������Ч���ݲ���
	ucPointNum=pucBuffer[0];
	if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
	{
		m+=7;
		pucBuffer+=2;
		for(i=0;i<ucPointNum;i++)
		{
			if((m+4)>448)					//���������
				break;
			ulVal=0;
			memcpy((unsigned char *)&ulVal,pucBuffer,ucDataLen);
			pucBuffer+=ucDataLen;
			memcpy(pucRetBuffer+m,(unsigned char *)&ulVal,2);
			m+=2;
		}
		pucRetBuffer[pusPosBak+6]=i;			//ʵ�ʵ���
		(*pusPos)=m;
	}
	return ucRet;
}

unsigned char Main_Get_ReqData_2_Curr_Phase(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI)
{//�������
	unsigned char ucRet=RET_SUCCESS;	
	STDATETIME stTmpTime;
	unsigned char *pucBuffer;
	unsigned char ucDataLen;
	unsigned char i,ucM,ucPointNum;
	unsigned char ucMPNo,ucMeterNo,ucTaskNo=0xFF,ucMPType;
	unsigned short m=(*pusPos),pusPosBak;
	unsigned long ulVal;
	
	ucMPNo=ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
		if(ucMeterNo>=CON_METER_NUM)
			return RET_ERROR;
		ucTaskNo=ucMeterNo;
	}else if(ucMPType==CON_MPTYPE_TER)
		ucTaskNo=Task_GetTaskNo_Ter();
	if(ucTaskNo>=CON_GRTASK_NUM)
		return RET_ERROR;

	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;

	stTmpTime=(*pstStartTime);
	stTmpTime.ucSecond=0;
	if(Is_DateTime_Availble(&stTmpTime)==RET_ERROR)
		return RET_ERROR;
	ucM=ucDataInter;
	if((ucM>4) || (ucM==0))
		return RET_ERROR;
	ucM=1<<(ucM-1);							//�ܶȴ���ת��Ϊ�ܶ���ֵ
	ucPointNum=Get_CurvePointNum_ByTime(pstStartTime,pstEndTime,ucM);
	pusPosBak=m;
	SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stTmpTime);
	pucRetBuffer[m+5]=ucDataInter;
	pucRetBuffer[m+6]=1;					//Ĭ��Ϊһ��
	memset(pucRetBuffer+m+7,0xEE,2);
	ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stTmpTime,ucPointNum,ucM,&ucDataLen,usDI);	//��Ч������Ч���ݲ���
	ucPointNum=pucBuffer[0];
	if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
	{
		m+=7;
		pucBuffer+=2;
		for(i=0;i<ucPointNum;i++)
		{
			if((m+4)>448)					//���������
				break;
			ulVal=0;
			memcpy((unsigned char *)&ulVal,pucBuffer,ucDataLen);
			pucBuffer+=ucDataLen;
			memcpy(pucRetBuffer+m,(unsigned char *)&ulVal,2);
			m+=2;
		}
		pucRetBuffer[pusPosBak+6]=i;			//ʵ�ʵ���
		(*pusPos)=m;
	}
	return ucRet;
}
*/
unsigned char Main_Get_ReqData_2_PowVal(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI)
{//������
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;	
	STDATETIME stTmpTime;
	unsigned char *pucBuffer;
	unsigned char ucDataLen;
	unsigned char i,ucM,ucPointNum;
//	unsigned char ucMPNo,ucMeterNo,ucTaskNo=0xFF,ucMPType;
	unsigned char ucTaskNo=0xFF;
	unsigned short m=(*pusPos),pusPosBak;
	unsigned char ucDesBuf[4];
	unsigned long ulTmp,ulValBase,ulValThis;
//	unsigned char ucPulseType;
//azh 170925	
    if(RET_ERROR == Main_Get_ReqData_2_Check(ucDAp))
    {
        return RET_ERROR;
    }    	
//	ucMPNo=ucDAp;
//	ucMPType=RunPara_GetMPType(ucMPNo);
//	if(ucMPType==CON_MPTYPE_METER)
//	{
//		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
//		if(ucMeterNo>=CON_METER_NUM)
//			return RET_ERROR;
//		ucTaskNo=ucMeterNo;
//	}
////	else if(ucMPType==CON_MPTYPE_PULSE)
////	{
////		ucPulseType=RunPara_GetPulseType_ByDI(usDI);
////		if(ucPulseType!=CON_PULSEATTR_INVALID)
////		{
////			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,ucPulseType);
////			ucTaskNo=Task_GetTaskNo_byPulseNo(ucPulseNo);
////		}
////		else
////		{
////			ucTaskNo=CON_GRTASK_NUM;			//��Ч
////		}
////	}
////	else if(ucMPType==CON_MPTYPE_TER)
////		ucTaskNo=Task_GetTaskNo_Ter();
//	if(ucTaskNo>=CON_GRTASK_NUM)
//		return RET_ERROR;

//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;

	stTmpTime=(*pstStartTime);
	stTmpTime.ucSecond=0;
	if(Is_DateTime_Availble(&stTmpTime)==RET_ERROR)
		return RET_ERROR;
	ucM=ucDataInter;
	if((ucM>4) || (ucM==0))
		return RET_ERROR;
	ucM=1<<(ucM-1);							//�ܶȴ���ת��Ϊ�ܶ���ֵ
	ucPointNum=Get_CurvePointNum_ByTime(pstStartTime,pstEndTime,ucM);
	pusPosBak=m;
	SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stTmpTime);
	pucRetBuffer[m+5]=ucDataInter;
	pucRetBuffer[m+6]=1;					//Ĭ��Ϊһ��
	memset(pucRetBuffer+m+7,0xEE,4);
	ucPointNum=ucPointNum+1;				//��һ��
	SubTime(&stTmpTime,(unsigned char)(15*ucM),CON_TIMEUNIT_MIN);

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucBuffer=(unsigned char *)mem_ptr;			//����������       

	ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stTmpTime,ucPointNum,ucM,&ucDataLen,usDI);	//��Ч������Ч���ݲ���
	ucPointNum=pucBuffer[0]-1;
	if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
	{
		m+=7;
		pucBuffer+=2;
		memset(ucDesBuf,0,4);
		memcpy(ucDesBuf,pucBuffer,ucDataLen);
		if(ucDesBuf[0]==CON_GRTASK_DATA_INVALID)
			ulValBase=CONINVALIDVAL;
		else
			ulValBase=BCD_TO_HEX_4(ucDesBuf);
		pucBuffer+=ucDataLen;
		for(i=0;i<ucPointNum;i++)
		{
			if((m+4)>448)					//���������
				break;
			memset(ucDesBuf,0,4);
			memcpy(ucDesBuf,pucBuffer,ucDataLen);
			if(ucDesBuf[0]==CON_GRTASK_DATA_INVALID)
				ulValThis=CONINVALIDVAL;
			else
				ulValThis=BCD_TO_HEX_4(ucDesBuf);
			if(ulValThis!=CONINVALIDVAL && ulValBase!=CONINVALIDVAL && ucDesBuf[0]!=CON_GRTASK_DATA_INVALID)
			{
				if(ulValThis>ulValBase)
					ulTmp=ulValThis-ulValBase;
				else
					ulTmp=0;
				SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulTmp);	
			}else
			{
				memset(pucRetBuffer+m,0xEE,4);	
			}
			ulValBase=ulValThis;
			m+=4;
			pucBuffer+=ucDataLen;
		}
		pucRetBuffer[pusPosBak+6]=i;			//ʵ�ʵ���
		(*pusPos)=m;
	}

    free_mem_block(mem_ptr);//
	return ucRet;
}

unsigned char Main_Get_ReqData_2_PowViewData(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI)
{//������ʾֵ
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTmpTime;
	unsigned char *pucBuffer;
	unsigned char ucDataLen;
	unsigned char i,ucM,ucPointNum;
//	unsigned char ucMPNo,ucMeterNo,ucTaskNo=0xFF,ucMPType;
	unsigned char ucTaskNo=0xFF;
	unsigned short m=(*pusPos),pusPosBak;
//	unsigned char ucPulseType;
//azh 170925	
    if(RET_ERROR == Main_Get_ReqData_2_Check(ucDAp))
    {
        return RET_ERROR;
    }    	
//	ucMPNo=ucDAp;
//	ucMPType=RunPara_GetMPType(ucMPNo);
//	if(ucMPType==CON_MPTYPE_METER)
//	{
//		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
//		if(ucMeterNo>=CON_METER_NUM)
//			return RET_ERROR;
//		ucTaskNo=ucMeterNo;
//	}
////	else if(ucMPType==CON_MPTYPE_PULSE)
////	{
////		ucPulseType=RunPara_GetPulseType_ByDI(usDI);
////		if(ucPulseType!=CON_PULSEATTR_INVALID)
////		{
////			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,ucPulseType);
////			ucTaskNo=Task_GetTaskNo_byPulseNo(ucPulseNo);
////		}
////		else
////		{
////			ucTaskNo=CON_GRTASK_NUM;			//��Ч
////		}
////	}
////	else if(ucMPType==CON_MPTYPE_TER)
////		ucTaskNo=Task_GetTaskNo_Ter();
//	if(ucTaskNo>=CON_GRTASK_NUM)
//		return RET_ERROR;

//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;

	stTmpTime=(*pstStartTime);
	stTmpTime.ucSecond=0;
	if(Is_DateTime_Availble(&stTmpTime)==RET_ERROR)
		return RET_ERROR;
	ucM=ucDataInter;
	if((ucM>4) || (ucM==0))
		return RET_ERROR;
	ucM=1<<(ucM-1);							//�ܶȴ���ת��Ϊ�ܶ���ֵ
	ucPointNum=Get_CurvePointNum_ByTime(pstStartTime,pstEndTime,ucM);
	pusPosBak=m;
	SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stTmpTime);
	pucRetBuffer[m+5]=ucDataInter;
	pucRetBuffer[m+6]=1;					//Ĭ��Ϊһ��
	memset(pucRetBuffer+m+7,0xEE,4);

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucBuffer=(unsigned char *)mem_ptr;			//����������        

	ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stTmpTime,ucPointNum,ucM,&ucDataLen,usDI);	//��Ч������Ч���ݲ���
	ucPointNum=pucBuffer[0];
	if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
	{
		m+=7;
		pucBuffer+=2;
		for(i=0;i<ucPointNum;i++)
		{
			if((m+4)>448)					//���������
				break;
			memcpy(pucRetBuffer+m,pucBuffer,ucDataLen);
			m+=4;
			pucBuffer+=ucDataLen;
		}
		pucRetBuffer[pusPosBak+6]=i;			//ʵ�ʵ���
		(*pusPos)=m;
	}

    free_mem_block(mem_ptr);//
	return ucRet;
}

unsigned char Main_Get_ReqData_2_PowRate(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI)
{//��������
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTmpTime;
	unsigned char *pucBufferP,*pucBufferQ;
	unsigned char ucDataLenP,ucDataLenQ;
	unsigned char i,ucM,ucPointNumP,ucPointNumQ;
	unsigned char ucMPNo,ucMeterNo,ucTaskNo=0xFF,ucTaskNo2=0xFF,ucMPType;
	unsigned short m=(*pusPos),pusPosBak;
	unsigned long ulPow,ulVar,ulRate,ulTmp;
	unsigned short usDIP,usDIQ;
	
	ucMPNo=ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
		if(ucMeterNo>=CON_METER_NUM)
			return RET_ERROR;
		ucTaskNo=ucMeterNo;
		ucTaskNo2=ucTaskNo;//----------------
	}
//	else if(ucMPType==CON_MPTYPE_PULSE)
//	{
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRPOW);
//		ucTaskNo=Task_GetTaskNo_byPulseNo(ucPulseNo);
//		ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
//		ucTaskNo2=Task_GetTaskNo_byPulseNo(ucPulseNo);
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
//		ucTaskNo=Task_GetTaskNo_Ter();
//		ucTaskNo2=ucTaskNo;
//	}
	if(ucTaskNo>=CON_GRTASK_NUM)
		return RET_ERROR;

//	pucBufferP=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;
//	pucBufferQ=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF+CON_BUFFER_CLASS3_DIFF;

	stTmpTime=(*pstStartTime);
	stTmpTime.ucSecond=0;
	if(Is_DateTime_Availble(&stTmpTime)==RET_ERROR)
		return RET_ERROR;
	ucM=ucDataInter;
	if((ucM>4) || (ucM==0))
		return RET_ERROR;
	ucM=1<<(ucM-1);							//�ܶȴ���ת��Ϊ�ܶ���ֵ
	ucPointNumP=Get_CurvePointNum_ByTime(pstStartTime,pstEndTime,ucM);
	ucPointNumQ=ucPointNumP;
	switch(usDI)
	{
	case CON_DI_TYPE_MPPOWRATE:
		usDIP=CON_DI_TYPE_MPINSTANTPOW;
		usDIQ=CON_DI_TYPE_MPINSTANTVAR;
		break;
	case CON_DI_TYPE_MPPOWRATEA:
		usDIP=CON_DI_TYPE_MPINSTANTPOWA;
		usDIQ=CON_DI_TYPE_MPINSTANTVARA;
		break;
	case CON_DI_TYPE_MPPOWRATEB:
		usDIP=CON_DI_TYPE_MPINSTANTPOWB;
		usDIQ=CON_DI_TYPE_MPINSTANTVARB;
		break;
	case CON_DI_TYPE_MPPOWRATEC:
		usDIP=CON_DI_TYPE_MPINSTANTPOWC;
		usDIQ=CON_DI_TYPE_MPINSTANTVARC;
		break;
	default:
		usDIP=CON_DI_TYPE_MPINSTANTPOW;
		usDIQ=CON_DI_TYPE_MPINSTANTVAR;
		break;
	}
	pusPosBak=m;
	SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stTmpTime);
	pucRetBuffer[m+5]=ucDataInter;
	pucRetBuffer[m+6]=1;					//Ĭ��Ϊһ��
	memset(pucRetBuffer+m+7,0xEE,2);

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucBufferP=(unsigned char *)mem_ptr;
	pucBufferQ=(unsigned char *)mem_ptr+768;

	ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBufferP,&stTmpTime,ucPointNumP,ucM,&ucDataLenP,usDIP);	//��Ч������Ч���ݲ���
	ucPointNumP=pucBufferP[0];
	if(ucRet!=RET_ERROR && ucPointNumP!=0 && ucDataLenP!=0)
	{
		ucRet=Task_QueryRetVal_ByDI(ucTaskNo2,pucBufferQ,&stTmpTime,ucPointNumQ,ucM,&ucDataLenQ,usDIQ);	//��Ч������Ч���ݲ���
		ucPointNumQ=pucBufferQ[0];
		if(ucRet!=RET_ERROR && ucPointNumQ==ucPointNumP && ucDataLenQ!=0)
		{
			m+=7;
			pucBufferP+=2;
			pucBufferQ+=2;
			for(i=0;i<ucPointNumP;i++)
			{
				if((m+4)>448)					//���������
					break;
				ulPow=0;
				ulVar=0;
				memcpy((unsigned char *)&ulTmp,pucBufferP,ucDataLenP);
				if((unsigned char)ulTmp==CON_GRTASK_DATA_INVALID)
					ulPow=CONINVALIDVAL;
				else
					ulPow=BCD_TO_HEX_3((unsigned char *)&ulTmp);
				memcpy((unsigned char *)&ulTmp,pucBufferQ,ucDataLenQ);
				if((unsigned char)ulTmp==CON_GRTASK_DATA_INVALID)
					ulVar=CONINVALIDVAL;
				else
					ulVar=BCD_TO_HEX_2((unsigned char *)&ulTmp);
				if(ulPow!=CONINVALIDVAL && ulVar!=CONINVALIDVAL)
				{
					ulRate=GetPQRate(ulPow,ulVar);
					ulRate=ulRate*10;
				}else
					ulRate=CONINVALIDVAL;
				SetMainComm_Buf_lVal_2B(pucRetBuffer,m,ulRate);
				m+=2;
				pucBufferP+=ucDataLenP;
				pucBufferQ+=ucDataLenQ;
			}
			pucRetBuffer[pusPosBak+6]=i;			//ʵ�ʵ���
			(*pusPos)=m;
		}
	}

    free_mem_block(mem_ptr);//
	return ucRet;
}

unsigned char Main_Get_ReqData_2_SumGroup_Pow(unsigned char ucGroupNo,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI,unsigned char ucPowFlag)
{//�ܼӵ���
	unsigned char ucRet=RET_SUCCESS;
//	STDATETIME stTmpTime;
//	unsigned char *pucBuffer;
//	unsigned char ucDataLen;
//	unsigned char i,ucM,ucPointNum;
//	unsigned char ucTaskNo=0xFF;
//	unsigned short m=(*pusPos),pusPosBak;
//	unsigned char ucDesBuf[4];
//	unsigned short usDT2=0;
//	unsigned long ulTmp=0;
//	float fValBase,fValThis;
//
//	if(ucGroupNo>=CON_SUMGROUP_NUM)
//		return RET_ERROR;
//	if(ucPowFlag==CON_STATUS_YES)
//		ucTaskNo=CON_GRTASK_SUMGROUP_POW_TASKNO;
//	else
//		ucTaskNo=CON_GRTASK_SUMGROUP_VAR_TASKNO;
//
//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;
//
//	stTmpTime=(*pstStartTime);
//	stTmpTime.ucSecond=0;
//	if(Is_DateTime_Availble(&stTmpTime)==RET_ERROR)
//		return RET_ERROR;
//	ucM=ucDataInter;
//	if((ucM>4) || (ucM==0))
//		return RET_ERROR;
//	ucM=1<<(ucM-1);							//�ܶȴ���ת��Ϊ�ܶ���ֵ
//	ucPointNum=Get_CurvePointNum_ByTime(pstStartTime,pstEndTime,ucM);
//	pusPosBak=m;
//	SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stTmpTime);
//	pucRetBuffer[m+5]=ucDataInter;
//	pucRetBuffer[m+6]=1;					//Ĭ��Ϊһ��
//	memset(pucRetBuffer+m+7,0xEE,4);		//���ݸ�ʽ03Ϊ4�ֽ�
//	ucPointNum=ucPointNum+1;				//��һ��
//	SubTime(&stTmpTime,(unsigned char)(15*ucM),CON_TIMEUNIT_MIN);
//	ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stTmpTime,ucPointNum,ucM,&ucDataLen,usDI);	//��Ч������Ч���ݲ���
//	ucPointNum=pucBuffer[0]-1;
//	if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
//	{
//		m+=7;
//		pucBuffer+=2;
//		memset(ucDesBuf,0,4);
//		memcpy(ucDesBuf,pucBuffer,ucDataLen);
//		if(ucDesBuf[0]==CON_GRTASK_DATA_INVALID)
//			fValBase=CONINVALIDVAL;
//		else
//			memcpy(&fValBase,ucDesBuf,4);
//		pucBuffer+=ucDataLen;
//		for(i=0;i<ucPointNum;i++)
//		{
//			if((m+4)>448)					//���������
//				break;
//			memset(ucDesBuf,0,4);
//			memcpy(ucDesBuf,pucBuffer,ucDataLen);
//			if(ucDesBuf[0]==CON_GRTASK_DATA_INVALID)
//				fValThis=CONINVALIDVAL;
//			else
//				memcpy(&fValThis,ucDesBuf,4);
//			if(fValThis!=CONINVALIDVAL && fValBase!=CONINVALIDVAL && ucDesBuf[0]!=CON_GRTASK_DATA_INVALID)
//			{
//				if(fValThis>fValBase)
//					ulTmp=Float_To_DT3_Div100(fValThis-fValBase);
//				else
//					ulTmp=0;
//				memcpy(pucRetBuffer+m,&ulTmp,4);	
//			}else
//			{
//				memset(pucRetBuffer+m,0xEE,4);	
//			}
//			fValBase=fValThis;
//			m+=4;
//			pucBuffer+=ucDataLen;
//		}
//		pucRetBuffer[pusPosBak+6]=i;			//ʵ�ʵ���
//		(*pusPos)=m;
//	}
	return ucRet;
}
//azh 170923
unsigned char Main_Get_ReqData_2_GroupCurveData(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI)
{//������ʾֵ
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	STDATETIME stTmpTime;
	unsigned char *pucBuffer;
	unsigned char ucDataLen;
	unsigned char i,ucM,ucPointNum;
//	unsigned char ucMPNo,ucMeterNo,ucTaskNo=0xFF,ucMPType;
	unsigned char ucTaskNo=0xFF;
	unsigned short m=(*pusPos),pusPosBak;
	unsigned char ucDesBuf[4];
//	unsigned long ulVal;

//	unsigned char ucPulseType;
//azh 170925	
    if(RET_ERROR == Main_Get_ReqData_2_Check(ucDAp))
    {
        return RET_ERROR;
    }    	
//	ucMPNo=ucDAp;
//	ucMPType=RunPara_GetMPType(ucMPNo);
//	if(ucMPType==CON_MPTYPE_METER)
//	{
//		ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
//		if(ucMeterNo>=CON_METER_NUM)
//			return RET_ERROR;
//		ucTaskNo=ucMeterNo;
//	}
////	else if(ucMPType==CON_MPTYPE_PULSE)
////	{
////		ucPulseType=RunPara_GetPulseType_ByDI(usDI);
////		if(ucPulseType!=CON_PULSEATTR_INVALID)
////		{
////			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,ucPulseType);
////			ucTaskNo=Task_GetTaskNo_byPulseNo(ucPulseNo);
////		}
////		else
////		{
////			ucTaskNo=CON_GRTASK_NUM;			//��Ч
////		}
////	}
////	else if(ucMPType==CON_MPTYPE_TER)
////		ucTaskNo=Task_GetTaskNo_Ter();
//	if(ucTaskNo>=CON_GRTASK_NUM)
//		return RET_ERROR;

//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;

	stTmpTime=(*pstStartTime);
	stTmpTime.ucSecond=0;
	if(Is_DateTime_Availble(&stTmpTime)==RET_ERROR)
		return RET_ERROR;
	ucM=ucDataInter;
	if((ucM>4) || (ucM==0))
		return RET_ERROR;
	ucM=1<<(ucM-1);							//�ܶȴ���ת��Ϊ�ܶ���ֵ
	ucPointNum=Get_CurvePointNum_ByTime(pstStartTime,pstEndTime,ucM);
	pusPosBak=m;
	SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stTmpTime);
	pucRetBuffer[m+5]=ucDataInter;
	pucRetBuffer[m+6]=1;					//Ĭ��Ϊһ��
	memset(pucRetBuffer+m+7,0xEE,33);

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucBuffer=(unsigned char *)mem_ptr;			//����������        

	ucRet=Task_QueryRetVal_ByGroupDI(ucTaskNo,pucBuffer,&stTmpTime,ucPointNum,ucM,&ucDataLen,usDI);	//��Ч������Ч���ݲ���
	ucPointNum=pucBuffer[0];
	if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
	{
		m+=7;
		pucBuffer+=2;
		for(i=0;i<ucPointNum;i++)
		{
			if((m+33)>448)					//���������
				break;

//�й�����
			memset(ucDesBuf,0,4);
			memcpy(ucDesBuf,pucBuffer,3);
			pucBuffer+=3;
			if(ucDesBuf[0]==0xEE)
			{
				memset(pucRetBuffer+m,0xEE,3);
			}else
			{
				if(ucDesBuf[2]>0x79)
					ucDesBuf[2]=0x79;			//���ݸ�ʽ9��Ŀǰ���������ݣ����ֵ79.9999
				memcpy(pucRetBuffer+m,ucDesBuf,3);			//�й�3�ֽ�
			}
			m+=3;
//�޹�����
			memset(ucDesBuf,0,4);
			memcpy(ucDesBuf,pucBuffer,2);
			pucBuffer+=2;
			if(ucDesBuf[0]==0xEE)
			{
				memset(pucRetBuffer+m,0xEE,3);
			}else
			{
				pucRetBuffer[m]=0;
				if(ucDesBuf[1]>0x79)
					ucDesBuf[1]=0x79;			//���ݸ�ʽ9��Ŀǰ���������ݣ����ֵ79.9999
				memcpy(pucRetBuffer+m+1,ucDesBuf,2);		//�޹�2�ֽڣ�0 ������ֽ�
			}
			m+=3;
//ABC���ѹ
			memcpy(pucRetBuffer+m,pucBuffer,6);
			pucBuffer+=6;
			m+=6;
//ABC�����
			memcpy(pucRetBuffer+m,pucBuffer,9);
			pucBuffer+=9;
			m+=9;
//�����й���һ�������޹�������ʾֵ
			memcpy(pucRetBuffer+m,pucBuffer,12);
			pucBuffer+=12;
			m+=12;

		}
		pucRetBuffer[pusPosBak+6]=i;			//ʵ�ʵ���
		(*pusPos)=m;
	}

    free_mem_block(mem_ptr);//
	return ucRet;
}

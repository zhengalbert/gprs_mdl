/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Job_Manage.c
 Description    : 1������/2�����ݵ��������͡����ݲ�ѯ����
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-02-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"


static unsigned char g_ucJobExeNo[2];

static unsigned char  Job_TaskGetParam_Data(STDATAUNITID stJobPara,unsigned char ucRepType,unsigned char * ucRetBuf,STDATETIME stDataBeginTime,STDATETIME stDataEndTime,unsigned char ucDataInter);
static unsigned char Get_Class2Data_TaskParam(STDATAUNITID stDUID,unsigned char * ucRetBuf,STDATETIME stDataBeginTime,STDATETIME stDataEndTime,unsigned char ucDataInter);
static STDATETIME Job_Adj_DataTime(STDATETIME *psBaseTime,unsigned char ucTimeInter,unsigned char ucTimeUnit,unsigned char ucResendDataInter);

/******************************************************************************
** ��������: void SaveJobRunInfo(unsigned char ucJobClassNo, unsigned char ucJobNo)
** ��������: 1�ࡢ2���������в����ı��浽EEPROM,������Ҫ�������´��ϱ�ʱ��
** ��ڲ���: ucJobClassNo: ��������0:1�� 1:2�� ucJobNo:�����0~CONDATAJOBNUM(16)
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void SaveJobRunInfo(unsigned char ucJobClassNo, unsigned char ucJobNo)
{
	unsigned char buf[CON_JOBRUNINFO_PER_LEN];
	unsigned long uladdr;
	STJOBPARA	*pstJobPara_Data=NULL;
	
    if((ucJobClassNo<2) && (ucJobNo<CONDATAJOBNUM))
    {
        pstJobPara_Data = RunPara_GetJobPara(ucJobNo, ucJobClassNo);
        memcpy(buf, &pstJobPara_Data->stNextExecute,sizeof(STDATETIME));

        buf[CON_JOBRUNINFO_PER_LEN-1] = addverify(buf, (CON_JOBRUNINFO_PER_LEN-1));
        if(ucJobClassNo==0)
        {
            uladdr = CON_JOB1RUNINFO_ADDR;
        }
        else
        {
            uladdr = CON_JOB2RUNINFO_ADDR;
        }
        uladdr += CON_JOBRUNINFO_PER_LEN*ucJobNo;
        file_write(uladdr, buf, CON_JOBRUNINFO_PER_LEN);
    }
}
/******************************************************************************
** ��������: void GetJobRunInfoFormEE (void)
** ��������: 1�ࡢ2���������в����ı��浽EEPROM,������Ҫ�������´��ϱ�ʱ��
** ��ڲ���: ucJobClassNo: ��������0:1�� 1:2�� ucJobNo:�����0~CONDATAJOBNUM(16)
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void GetJobRunInfoFormEE (unsigned char ucJobClassNo, unsigned char ucJobNo)
{
	unsigned char buf[CON_JOBRUNINFO_PER_LEN],crc;
	unsigned long uladdr;
	STJOBPARA	*pstJobPara_Data=NULL;
	STDATETIME  stCurrTime;
	
    if((ucJobClassNo<2) && (ucJobNo<CONDATAJOBNUM))
    {
        if(ucJobClassNo==0)
        {
            uladdr = CON_JOB1RUNINFO_ADDR;
        }
        else
        {
            uladdr = CON_JOB2RUNINFO_ADDR;
        }
        uladdr += CON_JOBRUNINFO_PER_LEN*ucJobNo;
        file_read(uladdr, buf, CON_JOBRUNINFO_PER_LEN);
        crc = addverify(buf, (CON_JOBRUNINFO_PER_LEN-1));
        
        pstJobPara_Data = RunPara_GetJobPara(ucJobNo, ucJobClassNo);
        if(crc == buf[CON_JOBRUNINFO_PER_LEN-1])
        {
            pstJobPara_Data = RunPara_GetJobPara(ucJobNo, ucJobClassNo);
            memcpy(&pstJobPara_Data->stNextExecute, buf, sizeof(STDATETIME));
        }
        else
        {
            stCurrTime=GetCurrentTime();
            memcpy(&pstJobPara_Data->stNextExecute, (U8*)&stCurrTime, sizeof(STDATETIME));
        }
    }
}

void Job_ProcData_Timer(void)
{
	Job_ProcData(0);//1 1������������
	Job_ProcData(1);//2 2������������
}

//1������������
unsigned char Job_ProcData(unsigned char ucJobClassNo)
{
	unsigned char  mem_type,mem1_type,*pret_mem = NULL,*mem_ptr = NULL;//azh
	unsigned char ucRet=RET_SUCCESS;
	unsigned char iucDUIDNum;
	STDATETIME  stCurrTime=GetCurrentTime();
	unsigned long lCurrTime,lNextExeTime,ulNum;

	unsigned char ucDUIDNum=0,ilen=0;
	unsigned char ucLink=11;				//����Ҫ��վȷ��
	unsigned char ucCommType=CON_TERMSG_SOUR_RCOMM_TCP;
	STJOBPARA	*pstJobPara_Data1;//stJobPara_Data1;
//	unsigned char    iFrameFlag=0x60,iFrameCount=0;
	unsigned char ucRetTerMsgNum;
	unsigned char ucRepType=CON_TERMSG_REQDATA_1;
	STDATETIME	stDataBeginTime,stDataEndTime;
	unsigned char *ucBuffer;//ucBuffer[512];
//	STLINKPARA stLinkPara = RunPara_GetLinkPara();

	unsigned char k,ucHours=0;

	ucCommType=Main_Get_Comm_Type();
//azh û�м���������ģʽ
//	if((ucCommType!=CON_TERMSG_SOUR_RCOMM_SMS && stLinkPara.ucSlaveFlag == CON_LINK_MASTER)) 
//azh	    ||(stLinkPara.ucSlaveFlag == CON_LINK_SALVE && Link_IsFull_SendWaitList() != RET_SUCCESS))
    if(ucCommType!=CON_TERMSG_SOUR_RCOMM_SMS) 
    {
		if(ucJobClassNo==0)
			ucRepType=CON_TERMSG_REQDATA_1;
		if(ucJobClassNo==1)
			ucRepType=CON_TERMSG_REQDATA_2;
		g_ucJobExeNo[ucJobClassNo]++;
		if(g_ucJobExeNo[ucJobClassNo]>=CONDATAJOBNUM)
			g_ucJobExeNo[ucJobClassNo]=0;
		pstJobPara_Data1=RunPara_GetJobPara(g_ucJobExeNo[ucJobClassNo],ucJobClassNo);
		while(pstJobPara_Data1->ucActive!=CON_JOB_ACTIVE && (g_ucJobExeNo[ucJobClassNo]+1)<CONDATAJOBNUM)
		{
			g_ucJobExeNo[ucJobClassNo]++;
			pstJobPara_Data1=RunPara_GetJobPara(g_ucJobExeNo[ucJobClassNo],ucJobClassNo);
		}
		if(pstJobPara_Data1->ucActive==CON_JOB_ACTIVE)
		{//1�δ���1��job
			stCurrTime=GetCurrentTime();
			lCurrTime=TimeConver(&stCurrTime,CON_TIMEUNIT_SEC);
			lNextExeTime=TimeConver(&pstJobPara_Data1->stNextExecute,CON_TIMEUNIT_SEC);
			if(lCurrTime>=lNextExeTime)//����ʱ�䵽
			{
				Clr_Group_RetData();
				Clr_Group_RetExtData();
				stDataBeginTime=Job_Adj_DataTime(&(pstJobPara_Data1->stNextExecute),
					pstJobPara_Data1->ucTimeInter,
					pstJobPara_Data1->ucTimeUnit,
					pstJobPara_Data1->ucResendDataInter);	//2������������Ҫ��ע���ݷ���������ʱ��
				ulNum=TimeConverToSecond(pstJobPara_Data1->ucTimeInter,pstJobPara_Data1->ucTimeUnit);
				if(ulNum==0)
					ulNum=60;		//1����
				ulNum=(lCurrTime-lNextExeTime)/ulNum+1;					//�´�ִ��ʱ��
				AddTime(&pstJobPara_Data1->stNextExecute,ulNum*pstJobPara_Data1->ucTimeInter,pstJobPara_Data1->ucTimeUnit);
				stDataEndTime=Job_Adj_DataTime(&(pstJobPara_Data1->stNextExecute),
					pstJobPara_Data1->ucTimeInter,
					pstJobPara_Data1->ucTimeUnit,
					pstJobPara_Data1->ucResendDataInter);	//2������������Ҫ��ע���ݷ���������ʱ��
//				RunPara_SetJobPara(ucJobClassNo,g_ucJobExeNo[ucJobClassNo],&stJobPara_Data1);//azh ǰ�����ȫ�ֱ�����ָ�� ���Բ����ٱ�������
                SaveJobRunInfo(ucJobClassNo,g_ucJobExeNo[ucJobClassNo]);//azh ���������´�����ʱ��
                
				if(pstJobPara_Data1->ucTimeUnit==CON_TIMEUNIT_MON)		//���������͵ĳ���������
					gucJobMonthUpFlag=CON_STATUS_YES;
				else
					gucJobMonthUpFlag=CON_STATUS_NO;
				
				if(pstJobPara_Data1->ucDUIDNum>=CONDATAUNITNUM)
					ucDUIDNum=CONDATAUNITNUM;
				else
					ucDUIDNum=pstJobPara_Data1->ucDUIDNum;

                pret_mem = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
                if(pret_mem == NULL)
                {
                    return RET_ERROR;
                }    
                mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem1_type, GET_MEM_ERR0);
                if(pret_mem == NULL)
                {
                    return RET_ERROR;
                }
                ucBuffer = mem_ptr;    
				for(iucDUIDNum=0;iucDUIDNum<ucDUIDNum;iucDUIDNum++)
				{
					ilen=Job_TaskGetParam_Data(pstJobPara_Data1->stDUID[iucDUIDNum], ucRepType,ucBuffer,stDataBeginTime,stDataEndTime,pstJobPara_Data1->ucResendDataInter);
					ucHours=1;
					if(RunPara_GetTerAdjPara_Job1HourDataExt()==CON_STATUS_YES)
					{
						if(ucJobClassNo==0)	//1������
						{
							if(pstJobPara_Data1->stDUID[iucDUIDNum].ucDT2>=0x0A && pstJobPara_Data1->stDUID[iucDUIDNum].ucDT2<=0x0E)	//Сʱ���᷶Χ
							{
								if(pstJobPara_Data1->ucTimeUnit==CON_TIMEUNIT_HOUR)
								{
									ucHours=pstJobPara_Data1->ucTimeInter;
									if(ucHours==0)
										ucHours=1;
									if(ucHours>8)				//�������8��
										ucHours=8;
								}
							}
						}
					}
					for(k=0;k<ucHours;k++)
					{
						gucJob1HoursFlag=ucHours-1-k;			//0,1,,ucHours-1
						Job_Data_Explain(ucBuffer,ilen,ucRepType,&ucRetTerMsgNum);
						Main_GetSendData(ucRetTerMsgNum,0xBB,0xBB,pret_mem);
					}
					gucJob1HoursFlag=0xFF;			//������Ч
				}
				gucJobMonthUpFlag=CON_STATUS_NO;				//��;Main_GetSendData�����ã�������
				//���͵��˿�
				Main_SendComm_Recv(ucCommType,ucLink,CON_STATUS_YES);
				free_mem_block(pret_mem);//Mem_1536_Byte	
				free_mem_block(mem_ptr);//Mem_1536_Byte	
			}
		}
	}
	return ucRet;
}

unsigned char  Job_TaskGetParam_Data(STDATAUNITID stDUID,unsigned char ucRepType,unsigned char * ucRetBuf,STDATETIME stDataBeginTime,STDATETIME stDataEndTime,unsigned char ucDataInter)
{
	unsigned char ucRetLen=0;
	switch(ucRepType)
	{
	case CON_TERMSG_REQDATA_1:
		ucRetBuf[0]=stDUID.ucDA1;
		ucRetBuf[1]=stDUID.ucDA2;
		ucRetBuf[2]=stDUID.ucDT1;
		ucRetBuf[3]=stDUID.ucDT2;
		ucRetLen=4;
		break;
	case CON_TERMSG_REQDATA_2:
		ucRetLen=Get_Class2Data_TaskParam(stDUID,ucRetBuf,stDataBeginTime,stDataEndTime,ucDataInter);
		break;
	default: 
		break;
	}
	return ucRetLen;
}
unsigned char Get_Class2Data_TaskParam(STDATAUNITID stDUID,unsigned char * ucRetBuf,STDATETIME stDataBeginTime,STDATETIME stDataEndTime,unsigned char ucDataInter)
{
	unsigned char ucDT1,ucDT2,ucDA1,ucDA2;
	unsigned char pucDTList[8],pucDAList[8];
	unsigned char ucCount=0,ucDACount=0,i,j;
	unsigned char ucPos=0,ucM;
	unsigned char ucDay,ucMonth,ucYear,ucHour,ucMinute;
	ucRetBuf[ucPos++]=stDUID.ucDA1;
	ucRetBuf[ucPos++]=stDUID.ucDA2;
	ucRetBuf[ucPos++]=stDUID.ucDT1;
	ucRetBuf[ucPos++]=stDUID.ucDT2;
	memset(pucDTList,0xFF,8);
	memset(pucDAList,0xFF,8);
	ucMinute=(unsigned char)HEX_TO_BCD(stDataBeginTime.ucMinute,1);
	ucHour=(unsigned char)HEX_TO_BCD(stDataBeginTime.ucHour,1);
	ucDay=(unsigned char)HEX_TO_BCD(stDataBeginTime.ucDay,1);
	ucMonth=(unsigned char)HEX_TO_BCD(stDataBeginTime.ucMonth,1);
	ucYear=(unsigned char)HEX_TO_BCD(stDataBeginTime.ucYear,1);
	
	ucDT1 = stDUID.ucDT1;	
	ucDT2 = stDUID.ucDT2;
	if(ucDT2<31 && ucDT1>0)							//>=31δ������������
	{
		for(i=0;i<8;i++)
		{
			if((ucDT1 & 0x01) == 0x01)
			{
				pucDTList[ucCount++] =i+ucDT2*0x08+1;	//��1��ʼ
			}
			ucDT1 = ucDT1>>1;
		}
	}
	
	ucDA2 = stDUID.ucDA2;
	ucDA1 = stDUID.ucDA1;
	for(j=0;j<8;j++)
	{
		if((ucDA1 & 0x01) == 0x01)
		{
			pucDAList[ucDACount++] =j+(ucDA2-1)*0x08+1;				      
			if(ucDACount>=8)					//���8��
				break;
		}
		ucDA1 = ucDA1>>1;
	}
	if(pucDAList[0]==0xFF)
	{
		pucDAList[0] =CON_MPNO_TER_FIXED;
		ucDACount++;
	}
	
	for(j=0;j<ucDACount;j++)
	{
		for(i=0;i<ucCount;i++)
		{
			switch(pucDTList[i])
			{
			case	1:		//F1	�ն���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
			case	2:		//F2	�ն��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
			case	3:		//F3	�ն���������/�޹��������������ʱ�䣨�ܡ�����1~M��
			case	4:		//F4	�ն��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��
			case	5:		//F5	�ն��������й����������ܡ�����1~M��
			case	6:		//F6	�ն��������޹����������ܡ�����1~M��
			case	7:		//F7	�ն��ᷴ���й����������ܡ�����1~M��
			case	8:		//F8	�ն��ᷴ���޹����������ܡ�����1~M��
			case	9:		//F9	�����ն���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
			case	10:		//F10	�����ն��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
			case	11:		//F11	�����ն���������/�޹��������������ʱ�䣨�ܡ�����1~M��
			case	12:		//F12	�����ն��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��
			case	25:		//F25	�շ�������й����ʼ�����ʱ�䡢�й�����Ϊ��ʱ��
			case	26:		//F26	���ܼ������������������ʱ��
			case	27:		//F27	�յ�ѹͳ������
			case	28:		//F28	�ղ�ƽ���Խ���ۼ�ʱ��
			case	29:		//F29	�յ���Խ��ͳ��
			case	30:		//F30	�����ڹ���Խ���ۼ�ʱ��
			case	31:		//F31	�յ��ܱ��������
			case	41:		//F41	������Ͷ���ۼ�ʱ��ʹ���
			case	42:		//F42	�ա��µ������ۼƲ������޹�������
			case	43:		//F43	�չ������������ۼ�ʱ��
			case	49:		//F49	�ն��չ���ʱ�䡢�ո�λ�ۼƴ���
			case	50:		//F50	�ն��տ���ͳ������
			case	57:		//F57	�ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��
			case	58:		//F58	�ܼ������ۼ��й����������ܡ�����1~M��
			case	59:		//F59	�ܼ������ۼ��޹����������ܡ�����1~M��
			case	113:		//F113	A��2~19��г�����������ֵ������ʱ��
			case	114:		//F114	B��2~19��г�����������ֵ������ʱ��
			case	115:		//F115	C��2~19��г�����������ֵ������ʱ��
			case	116:		//F116	A��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
			case	117:		//F117	B��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
			case	118:		//F118	C��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
			case	121:		//F121	A��г��Խ����ͳ������
			case	122:		//F122	B��г��Խ����ͳ������
			case	123:		//F123	C��г��Խ����ͳ������
			case	129:		//F129	ֱ��ģ����Խ�����ۼ�ʱ�䡢���/��Сֵ������ʱ��
			case 	153:
			case 	154:
			case 	155:
			case 	156:
//azh			    
			case    161:
			case    162:
			case    163:
			case    164:
			case    165:
			case    166:
			case    167:
			case    168:
			case    169:
			case    170:
			case    171:
			case    172:
			case    173:
			case    174:
			case    175:
			case    176:
			case    185:
			case    186:
			case    187:
			case    188:
			case    189:
			case    190:
			case    191:
			case    192:
			    			    			        
				if(ucPos+3>512)
					break;
				ucRetBuf[ucPos++]=ucDay;
				ucRetBuf[ucPos++]=ucMonth;
				ucRetBuf[ucPos++]=ucYear;
				break;
			case	17:		//F17	�¶���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
			case	18:		//F18	�¶��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
			case	19:		//F19	�¶���������/�޹��������������ʱ�䣨�ܡ�����1~M��
			case	20:		//F20	�¶��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��
			case	21:		//F21	�¶��������й����������ܡ�����1~M��
			case	22:		//F22	�¶��������޹����������ܡ�����1~M��
			case	23:		//F23	�¶��ᷴ���й����������ܡ�����1~M��
			case	24:		//F24	�¶��ᷴ���޹����������ܡ�����1~M��
			case	33:		//F33	�·�������й����ʼ�����ʱ�䡢�����й�����Ϊ��ʱ��
			case	34:		//F34	�·����й��������������ʱ�䡢���й��������������ʱ��
			case	35:		//F35	�µ�ѹͳ������
			case	36:		//F36	�²�ƽ���Խ���ۼ�ʱ��
			case	37:		//F37	�µ���Խ��ͳ��
			case	38:		//F38	�����ڹ���Խ���ۼ�ʱ��
			case	44:		//F44	�¹������������ۼ�ʱ��
			case	51:		//F51	�ն��¹���ʱ�䡢�¸�λ�ۼƴ���
			case	52:		//F52	�ն��¿���ͳ������
			case	60:		//F60	�ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��
			case	61:		//F61	�ܼ������ۼ��й����������ܡ�����1~M��
			case	62:		//F62	�ܼ������ۼ��޹����������ܡ�����1~M��				
			case	65:		//F65	�ܼ��鳬���ʶ�ֵ�����ۼ�ʱ�䡢���ۼƵ�����
			case	66:		//F66	�ܼ��鳬�µ�������ֵ�����ۼ�ʱ�䡢�ۼƵ�����
			case	130:		//F130	ֱ��ģ����Խ�����ۼ�ʱ�䡢���/��Сֵ������ʱ��
			case 	157:
			case 	158:
			case 	159:
			case 	160:				
				if(ucPos+2>512)
					break;
				ucRetBuf[ucPos++]=ucMonth;
				ucRetBuf[ucPos++]=ucYear;
				break;
			case	73:		//F73	�ܼ����й���������
			case	74:		//F74	�ܼ����޹���������
			case	75:		//F75	�ܼ����й�����������
			case	76:		//F76	�ܼ����޹�����������
			case	81:		//F81	�й���������
			case	82:		//F82	A���й���������
			case	83:		//F83	B���й���������
			case	84:		//F84	C���й���������
			case	85:		//F85	�޹���������
			case	86:		//F86	A���޹���������
			case	87:		//F87	B���޹���������
			case	88:		//F88	C���޹���������
			case	89:		//F89	A���ѹ����
			case	90:		//F90	B���ѹ����
			case	91:		//F91	C���ѹ����
			case	92:		//F92	A���������
			case	93:		//F93	B���������
			case	94:		//F94	C���������
			case	95:		//F95	�����������
			case	97:		//F97	�����й��ܵ�����
			case	98:		//F98	�����޹��ܵ�����
			case	99:		//F99	�����й��ܵ�����
			case	100:		//F100	�����޹��ܵ�����
			case	101:		//F101	�����й��ܵ���ʾֵ
			case	102:		//F102	�����޹��ܵ���ʾֵ
			case	103:		//F103	�����й��ܵ���ʾֵ
			case	104:		//F104	�����޹��ܵ���ʾֵ
			case	105:		//F105	�ܹ�������
			case	106:		//F106	A�๦������
			case	107:		//F107	B�๦������
			case	108:		//F108	C�๦������
			case	138:		//F138	ֱ��ģ������������
				if(ucPos+7>512)
					break;
				ucRetBuf[ucPos++]=ucMinute;
				ucRetBuf[ucPos++]=ucHour;
				ucRetBuf[ucPos++]=ucDay;
				ucRetBuf[ucPos++]=ucMonth;
				ucRetBuf[ucPos++]=ucYear;
				ucM=ucDataInter;
				if((ucM>4) || (ucM==0))
					return RET_ERROR;
				ucM=1<<(ucM-1);	
				ucRetBuf[ucPos++]=ucDataInter;
				ucRetBuf[ucPos++]=Get_CurvePointNum_ByTime(&stDataBeginTime,&stDataEndTime,ucM);
				break;
				
			case	32:		//F32	����
				break;
			}
		}
	}
	return ucPos;
}
void TimeChange_JobManage(STDATETIME stOldTime,STDATETIME stNewTime)
{//ʱ�ӱ仯����
	unsigned char i=0;
	STJOBPARA	*pstJobPara_Data;
	for(i=0;i<CONDATAJOBNUM;i++)
	{
		pstJobPara_Data=RunPara_GetJobPara(i,0);
		pstJobPara_Data->stNextExecute=RunPara_GetJobNextTime(
									pstJobPara_Data->stStartActive,
									pstJobPara_Data->ucTimeInter,
									pstJobPara_Data->ucTimeUnit);
//		RunPara_SetJobPara(0,i,&stJobPara_Data);
        SaveJobRunInfo(0,i);//azh ���������´�����ʱ��
		//HdConvertWatchDog();
		pstJobPara_Data=RunPara_GetJobPara(i,1);
		pstJobPara_Data->stNextExecute=RunPara_GetJobNextTime(
									pstJobPara_Data->stStartActive,
									pstJobPara_Data->ucTimeInter,
									pstJobPara_Data->ucTimeUnit);
//		RunPara_SetJobPara(1,i,&stJobPara_Data);
        SaveJobRunInfo(1,i);//azh ���������´�����ʱ��
	}
}

/*void TimeChange_JobManage(STDATETIME stOldTime,STDATETIME stNewTime)
{//ʱ�ӱ仯����
	unsigned char i=0,j=0;
	unsigned long ulOldTime,ulNewTime,ulDiff;
	STJOBPARA	stJobPara_Data;
	ulOldTime=TimeConver(&stOldTime,CON_TIMEUNIT_SEC);
	ulNewTime=TimeConver(&stNewTime,CON_TIMEUNIT_SEC);
	if(ulOldTime>ulNewTime)
	{//ʱ����ɸ�
		ulDiff=ulOldTime-ulNewTime;
		for(i=0;i<CONDATAJOBNUM;i++)
		{
			stJobPara_Data=RunPara_GetJobPara(i,0);
			SubTime(&(stJobPara_Data.stNextExecute),ulDiff,CON_TIMEUNIT_SEC);
			RunPara_SetJobPara(0,i,&stJobPara_Data);
		}
		for(i=0;i<CONDATAJOBNUM;i++)
		{
			stJobPara_Data=RunPara_GetJobPara(i,1);
			SubTime(&(stJobPara_Data.stNextExecute),ulDiff,CON_TIMEUNIT_SEC);
			RunPara_SetJobPara(1,i,&stJobPara_Data);
		}
	}else if(ulNewTime>ulOldTime)
	{//ʱ�����¸�
		ulDiff=ulNewTime-ulOldTime;
		for(i=0;i<CONDATAJOBNUM;i++)
		{
			stJobPara_Data=RunPara_GetJobPara(i,0);
			AddTime(&(stJobPara_Data.stNextExecute),ulDiff,CON_TIMEUNIT_SEC);
			RunPara_SetJobPara(0,i,&stJobPara_Data);
		}
		for(i=0;i<CONDATAJOBNUM;i++)
		{
			stJobPara_Data=RunPara_GetJobPara(i,1);
			AddTime(&(stJobPara_Data.stNextExecute),ulDiff,CON_TIMEUNIT_SEC);
			RunPara_SetJobPara(1,i,&stJobPara_Data);
		}
	}
}
*/


STDATETIME Job_Adj_DataTime(STDATETIME *psBaseTime,unsigned char ucTimeInter,unsigned char ucTimeUnit,unsigned char ucResendDataInter)
{//��ǰʱ�����Ϊ��ʼʱ�䣬Ȼ���ȥ����ʱ����
	STDATETIME stDateTime;
	stDateTime=(*psBaseTime);
	switch(ucTimeUnit)
	{
	case CON_TIMEUNIT_MON:		//��
		stDateTime.ucDay=1;
	case CON_TIMEUNIT_DAY:		//��
		stDateTime.ucHour=0;
	case CON_TIMEUNIT_HOUR:		//ʱ
		stDateTime.ucMinute=0;
	case CON_TIMEUNIT_MIN:		//��
		stDateTime.ucSecond=0;
		SubTime(&stDateTime,ucTimeInter,ucTimeUnit);
		break;
	default :
		break;
	}
	if(ucTimeUnit==CON_TIMEUNIT_DAY)
	{
		switch(ucResendDataInter)
		{
		case 1:
			AddTime(&stDateTime,15,CON_TIMEUNIT_MIN);
			break;
		case 2:
			AddTime(&stDateTime,30,CON_TIMEUNIT_MIN);
			break;
		case 3:
			AddTime(&stDateTime,60,CON_TIMEUNIT_MIN);
			break;
		}
	}
	return stDateTime;
}

//����2����������Ϊ�������ͳ����ն������ݲ���Ҫ����
//�������ʱ��������Ϊ���ڣ�ʱ��Ϊ����1��0ʱ0��0��
void Job_Adjust_MonthUp_FreeseDayData(STDATETIME *psTime,unsigned char ucMPNo)
{
	STDATETIME stCurrTime,stTmp;
//	STFREEZEDAY stFreezeDayPara;
	PSTFREEZEDAY pstFreezeDayPara;
	unsigned long ulCurr,ulTmp;
	unsigned char i;

	if(gucJobMonthUpFlag==CON_STATUS_YES)
	{
		stCurrTime=GetCurrentTime();
		stTmp=GetCurrentTime();
		pstFreezeDayPara=RunPara_GetMPFreezeDay(ucMPNo);
		stTmp.ucDay=pstFreezeDayPara->stFreezeDay[0].ucDay;
		stTmp.ucHour=pstFreezeDayPara->stFreezeDay[0].ucHour;
		stTmp.ucMinute=pstFreezeDayPara->stFreezeDay[0].ucMinute;
		stTmp.ucSecond=pstFreezeDayPara->stFreezeDay[0].ucSecond;

		if(Is_DateTime_Availble(&stTmp)==RET_SUCCESS)
		{
			ulCurr=TimeConver(&stCurrTime,CON_TIMEUNIT_SEC);
			ulTmp=TimeConver(&stTmp,CON_TIMEUNIT_SEC);
			if(ulCurr<=ulTmp)
			{//��һ��������ʱ��û������ѡ���������һ��������
				for(i=0;i<3;i++)		//����3��������
				{
					if(pstFreezeDayPara->stFreezeDay[i].ucDay>31)
					{
						break;
					}
				}
				if(i>0)
					i=i-1;			//ѡ����Ч������
				psTime->ucDay=pstFreezeDayPara->stFreezeDay[i].ucDay;
				psTime->ucHour=pstFreezeDayPara->stFreezeDay[i].ucHour;
				psTime->ucMinute=pstFreezeDayPara->stFreezeDay[i].ucMinute;
				psTime->ucSecond=pstFreezeDayPara->stFreezeDay[i].ucSecond;
			}
			else
			{//����ѡ��һ��������
				(*psTime)=stTmp;
				for(i=1;i<3;i++)
				{
					if(pstFreezeDayPara->stFreezeDay[i].ucDay>31)
					{
						break;
					}
					else
					{
						stTmp.ucDay=pstFreezeDayPara->stFreezeDay[i].ucDay;
						ulTmp=TimeConver(&stTmp,CON_TIMEUNIT_SEC);
						if(ulCurr<=ulTmp)
							break;
						else
							(*psTime)=stTmp;
					}
				}
				if(Is_DateTime_Availble(psTime)!=RET_SUCCESS)
					psTime->ucDay=28;			//ǿ��ת��
			}
		}
	}
}
//�����������
unsigned char Job_GetData_Data1(STDATAUNITID stDUID,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	unsigned char ucDTF;
	unsigned short usDAp;
	unsigned short usItemLabel=0,usDAList[8];
	unsigned char	ucDTList[8];

	pucRetBuffer[m++]=stDUID.ucDA1;
	pucRetBuffer[m++]=stDUID.ucDA2;
	pucRetBuffer[m++]=stDUID.ucDT1;
	pucRetBuffer[m++]=stDUID.ucDT2;

	memset(ucDTList,0xFF,8);
	memset(usDAList,0xFF,8*2);
//	Main_GetDAList(pucRetBuffer+m-4,4,0,usDAList);			//��ȡ��Ϣ���б�
//	Main_GetDTList(pucRetBuffer+m-4,4,0,ucDTList);			//��ȡ��Ϣ���б�
    Main_GetDADTList(pucRetBuffer+m-4, usDAList, ucDTList);	//��ȡ��Ϣ�㡢��Ϣ���б�	
    
	usDAp=usDAList[0];
	ucDTF=ucDTList[0];
	
	usItemLabel=GetItemLabelbyDT(CON_TERMSG_REQDATA_1,ucDTF);
	switch(usItemLabel)
	{
	case	CON_DATA_REQ1_F1:	//F1	�ն˰汾��Ϣ
		ucRet=Main_Get_ReqData_1_F1((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F2:	//F2	�ն�����ʱ��
		ucRet=Main_Get_ReqData_1_F2((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F3:	//F3	�ն˲���״̬
		ucRet=Main_Get_ReqData_1_F3((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F4:	//F4	�ն�ͨ��״̬
		ucRet=Main_Get_ReqData_1_F4((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F5:	//F5	�ն˿�������״̬
		ucRet=Main_Get_ReqData_1_F5((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F6:	//F6	�ն˵�ǰ����״̬
		ucRet=Main_Get_ReqData_1_F6((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F7:	//F7	�ն��¼���������ǰֵ
		ucRet=Main_Get_ReqData_1_F7((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F8:	//F8	�ն��¼���־״̬
		ucRet=Main_Get_ReqData_1_F8((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F9:	//F9	�ն�״̬������λ��־
		ucRet=Main_Get_ReqData_1_F9((unsigned char)usDAp,pucRetBuffer,&m);
		break;
//	case	CON_DATA_REQ1_F17:	//F17	��ǰ�ܼ��й�����
//		ucRet=Main_Get_ReqData_1_F17((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F18:	//F18	��ǰ�ܼ��޹�����
//		ucRet=Main_Get_ReqData_1_F18((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F19:	//F19	�����ܼ��й����������ܡ�����1~M��
//		ucRet=Main_Get_ReqData_1_F19((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F20:	//F20	�����ܼ��޹����������ܡ�����1~M��
//		ucRet=Main_Get_ReqData_1_F20((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F21:	//F21	�����ܼ��й����������ܡ�����1~M��
//		ucRet=Main_Get_ReqData_1_F21((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F22:	//F22	�����ܼ��޹����������ܡ�����1~M��
//		ucRet=Main_Get_ReqData_1_F22((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F23:	//F23	�ն˵�ǰʣ��������ѣ�
//		ucRet=Main_Get_ReqData_1_F23((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F24:	//F24	��ǰ�����¸��ؿغ��ܼ��й����ʶ���ֵ
//		ucRet=Main_Get_ReqData_1_F24((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
	case	CON_DATA_REQ1_F25:	//F25	��ǰ���༰����/�޹����ʡ����������������ѹ���������������
		ucRet=Main_Get_ReqData_1_F25((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F26:	//F26	A��B��C�������ͳ�����ݼ����һ�ζ����¼
		ucRet=Main_Get_ReqData_1_F26((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F27:	//F27	���ܱ�����ʱ�ӡ���̴����������һ�β���ʱ��
		ucRet=Main_Get_ReqData_1_F27((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F28:	//F28	�������״̬�ּ����λ��־
		ucRet=Main_Get_ReqData_1_F28((unsigned char)usDAp,pucRetBuffer,&m);
		break;
/*	case 	CON_DATA_REQ1_F31:
		ucRet = Main_Get_ReqData_1_F31((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case 	CON_DATA_REQ1_F32:
		ucRet = Main_Get_ReqData_1_F32((unsigned char)usDAp,pucRetBuffer,&m);
		break;*/
	case	CON_DATA_REQ1_F33:	//F33	��ǰ������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
		ucRet=Main_Get_ReqData_1_F33((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F34:	//F34	��ǰ������/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
		ucRet=Main_Get_ReqData_1_F34((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F35:	//F35	����������/�޹��������������ʱ�䣨�ܡ�����1~M��
		ucRet=Main_Get_ReqData_1_F35((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F36:	//F36	���·�����/�޹��������������ʱ�䣨�ܡ�����1~M��
		ucRet=Main_Get_ReqData_1_F36((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F37:	//F37	����������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
		ucRet=Main_Get_ReqData_1_F37((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F38:	//F38	���·�����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
		ucRet=Main_Get_ReqData_1_F38((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F39:	//F39	����������/�޹��������������ʱ�䣨�ܡ�����1~M��
		ucRet=Main_Get_ReqData_1_F39((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F40:	//F40	���·�����/�޹��������������ʱ�䣨�ܡ�����1~M��
		ucRet=Main_Get_ReqData_1_F40((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F41:	//F41	���������й����������ܡ�����1~M��
		ucRet=Main_Get_ReqData_1_F41((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F42:	//F42	���������޹����������ܡ�����1~M��
		ucRet=Main_Get_ReqData_1_F42((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F43:	//F43	���շ����й����������ܡ�����1~M��
		ucRet=Main_Get_ReqData_1_F43((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F44:	//F44	���շ����޹����������ܡ�����1~M��
		ucRet=Main_Get_ReqData_1_F44((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F45:	//F45	���������й����������ܡ�����1~M��
		ucRet=Main_Get_ReqData_1_F45((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F46:	//F46	���������޹����������ܡ�����1~M��
		ucRet=Main_Get_ReqData_1_F46((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F47:	//F47	���·����й����������ܡ�����1~M��
		ucRet=Main_Get_ReqData_1_F47((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F48:	//F48	���·����޹����������ܡ�����1~M��
		ucRet=Main_Get_ReqData_1_F48((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F49:	//F49	��ǰ��ѹ��������λ��
		ucRet=Main_Get_ReqData_1_F49((unsigned char)usDAp,pucRetBuffer,&m);
		break;
//	case	CON_DATA_REQ1_F57:	//F57	��ǰA��B��C�����ѹ������2~N��г����Чֵ
//		ucRet=Main_Get_ReqData_1_F57((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F58:	//F58	��ǰA��B��C�����ѹ������2~N��г��������
//		ucRet=Main_Get_ReqData_1_F58((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F65:	//F65	��ǰ������Ͷ��״̬
//		ucRet=Main_Get_ReqData_1_F65((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F66:	//F66	��ǰ�������ۼƲ���Ͷ��ʱ��ʹ���
//		ucRet=Main_Get_ReqData_1_F66((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F67:	//F67	���ա����µ������ۼƲ������޹�������
//		ucRet=Main_Get_ReqData_1_F67((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F73:	//F73	ֱ��ģ����ʵʱ����
//		ucRet=Main_Get_ReqData_1_F73((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F81:	//F81	Сʱ�����ܼ��й�����
//		ucRet=Main_Get_ReqData_1_F81((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F82:	//F82	Сʱ�����ܼ��޹�����
//		ucRet=Main_Get_ReqData_1_F82((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F83:	//F83	Сʱ�����ܼ��й��ܵ�����
//		ucRet=Main_Get_ReqData_1_F83((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F84:	//F84	Сʱ�����ܼ��޹��ܵ�����
//		ucRet=Main_Get_ReqData_1_F84((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
	case	CON_DATA_REQ1_F89:	//F89	Сʱ�����й�����
		ucRet=Main_Get_ReqData_1_F89((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F90:	//F90	Сʱ����A���й�����
		ucRet=Main_Get_ReqData_1_F90((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F91:	//F91	Сʱ����B���й�����
		ucRet=Main_Get_ReqData_1_F91((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F92:	//F92	Сʱ����C���й�����
		ucRet=Main_Get_ReqData_1_F92((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F93:	//F93	Сʱ�����޹�����
		ucRet=Main_Get_ReqData_1_F93((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F94:	//F94	Сʱ����A���޹�����
		ucRet=Main_Get_ReqData_1_F94((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F95:	//F95	Сʱ����B���޹�����
		ucRet=Main_Get_ReqData_1_F95((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F96:	//F96	Сʱ����C���޹�����
		ucRet=Main_Get_ReqData_1_F96((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F97:	//F97	Сʱ����A���ѹ
		ucRet=Main_Get_ReqData_1_F97((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F98:	//F98	Сʱ����B���ѹ
		ucRet=Main_Get_ReqData_1_F98((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F99:	//F99	Сʱ����C���ѹ
		ucRet=Main_Get_ReqData_1_F99((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F100:	//F100	Сʱ����A�����
		ucRet=Main_Get_ReqData_1_F100((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F101:	//F101	Сʱ����B�����
		ucRet=Main_Get_ReqData_1_F101((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F102:	//F102	Сʱ����C�����
		ucRet=Main_Get_ReqData_1_F102((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F103:	//F103	Сʱ�����������
		ucRet=Main_Get_ReqData_1_F103((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F105:	//F105	Сʱ���������й��ܵ�����
		ucRet=Main_Get_ReqData_1_F105((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F106:	//F106	Сʱ���������޹��ܵ�����
		ucRet=Main_Get_ReqData_1_F106((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F107:	//F107	Сʱ���ᷴ���й��ܵ�����
		ucRet=Main_Get_ReqData_1_F107((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F108:	//F108	Сʱ���ᷴ���޹��ܵ�����
		ucRet=Main_Get_ReqData_1_F108((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F109:	//F109	Сʱ���������й��ܵ���ʾֵ
		ucRet=Main_Get_ReqData_1_F109((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F110:	//F110	Сʱ���������޹��ܵ���ʾֵ
		ucRet=Main_Get_ReqData_1_F110((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F111:	//F111	Сʱ���ᷴ���й��ܵ���ʾֵ
		ucRet=Main_Get_ReqData_1_F111((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F112:	//F112	Сʱ���ᷴ���޹��ܵ���ʾֵ
		ucRet=Main_Get_ReqData_1_F112((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F113:	//F113	Сʱ�����ܹ�������
		ucRet=Main_Get_ReqData_1_F113((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F114:	//F114	Сʱ����A�๦������
		ucRet=Main_Get_ReqData_1_F114((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F115:	//F115	Сʱ����B�๦������
		ucRet=Main_Get_ReqData_1_F115((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F116:	//F116	Сʱ����C�๦������
		ucRet=Main_Get_ReqData_1_F116((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F121:	//F121	Сʱ����ֱ��ģ����
//		ucRet=Main_Get_ReqData_1_F121((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F129:	
//azh 170921
    case    CON_DATA_REQ1_F177:
		ucRet=Main_Get_ReqData_1_F129((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F130:	
		ucRet=Main_Get_ReqData_1_F130((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F131:	
		ucRet=Main_Get_ReqData_1_F131((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F132:	
		ucRet=Main_Get_ReqData_1_F132((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F133:	
		ucRet=Main_Get_ReqData_1_F133((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F134:	
		ucRet=Main_Get_ReqData_1_F134((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F135:	
		ucRet=Main_Get_ReqData_1_F135((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F136:	
		ucRet=Main_Get_ReqData_1_F136((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F137:	
//azh 170921
    case    CON_DATA_REQ1_F178:
		ucRet=Main_Get_ReqData_1_F137((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F138:	
		ucRet=Main_Get_ReqData_1_F138((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F139:	
		ucRet=Main_Get_ReqData_1_F139((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F140:	
		ucRet=Main_Get_ReqData_1_F140((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F141:	
		ucRet=Main_Get_ReqData_1_F141((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F142:	
		ucRet=Main_Get_ReqData_1_F142((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F143:	
		ucRet=Main_Get_ReqData_1_F143((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F144:	
		ucRet=Main_Get_ReqData_1_F144((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F145:	
		ucRet=Main_Get_ReqData_1_F145((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F146:	
		ucRet=Main_Get_ReqData_1_F146((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F147:	
		ucRet=Main_Get_ReqData_1_F147((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F148:	
		ucRet=Main_Get_ReqData_1_F148((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F149:	
		ucRet=Main_Get_ReqData_1_F149((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F150:	
		ucRet=Main_Get_ReqData_1_F150((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F151:	
		ucRet=Main_Get_ReqData_1_F151((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F152:	
		ucRet=Main_Get_ReqData_1_F152((unsigned char)usDAp,pucRetBuffer,&m);
		break;
//azh 170921
    case    CON_DATA_REQ1_F162:
        ucRet=Main_Get_ReqData_1_F162((unsigned char)usDAp,pucRetBuffer,&m);
        break;
	default:
		break;
	}
	(*pusPos)=m;
	return ucRet;
}

//�����������
unsigned char Job_GetData_Data2(STDATAUNITID stDUID,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_ERROR;//RET_SUCCESS;//azh 130306
	unsigned short m=(*pusPos);
	unsigned char ucDTF;
	unsigned short usDAp;
	unsigned short usItemLabel=0,usDAList[8];
	unsigned char	ucDTList[8];

	pucRetBuffer[m++]=stDUID.ucDA1;
	pucRetBuffer[m++]=stDUID.ucDA2;
	pucRetBuffer[m++]=stDUID.ucDT1;
	pucRetBuffer[m++]=stDUID.ucDT2;

	memset(ucDTList,0xFF,8);
	memset(usDAList,0xFF,8*2);
//	Main_GetDAList(pucRetBuffer+m-4,4,0,usDAList);			//��ȡ��Ϣ���б�
//	Main_GetDTList(pucRetBuffer+m-4,4,0,ucDTList);			//��ȡ��Ϣ���б�
    Main_GetDADTList(pucRetBuffer+m-4, usDAList, ucDTList);	//��ȡ��Ϣ�㡢��Ϣ���б�	

	usDAp=usDAList[0];
	ucDTF=ucDTList[0];

	usItemLabel=GetItemLabelbyDT(CON_TERMSG_REQDATA_2,ucDTF);
	switch(usItemLabel)
	{
	case	CON_DATA_REQ2_F1:	//F1	�ն���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��	
		ucRet=Main_Get_ReqData_2_F1((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F2:	//F2	�ն��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��	
		ucRet=Main_Get_ReqData_2_F2((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F3:	//F3	�ն���������/�޹��������������ʱ�䣨�ܡ�����1~M��	
		ucRet=Main_Get_ReqData_2_F3((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F4:	//F4	�ն��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��	
		ucRet=Main_Get_ReqData_2_F4((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F5:	//F5	�ն��������й����������ܡ�����1~M��	
		ucRet=Main_Get_ReqData_2_F5((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F6:	//F6	�ն��������޹����������ܡ�����1~M��	
		ucRet=Main_Get_ReqData_2_F6((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F7:	//F7	�ն��ᷴ���й����������ܡ�����1~M��	
		ucRet=Main_Get_ReqData_2_F7((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F8:	//F8	�ն��ᷴ���޹����������ܡ�����1~M��	
		ucRet=Main_Get_ReqData_2_F8((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F9:	//F9	�����ն���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��	
		ucRet=Main_Get_ReqData_2_F9((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F10:	//F10	�����ն��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��	
		ucRet=Main_Get_ReqData_2_F10((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F11:	//F11	�����ն���������/�޹��������������ʱ�䣨�ܡ�����1~M��	
		ucRet=Main_Get_ReqData_2_F11((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F12:	//F12	�����ն��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��	
		ucRet=Main_Get_ReqData_2_F12((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F17:	//F17	�¶���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��	
		ucRet=Main_Get_ReqData_2_F17((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F18:	//F18	�¶��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��	
		ucRet=Main_Get_ReqData_2_F18((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F19:	//F19	�¶���������/�޹��������������ʱ�䣨�ܡ�����1~M��	
		ucRet=Main_Get_ReqData_2_F19((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F20:	//F20	�¶��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��	
		ucRet=Main_Get_ReqData_2_F20((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F21:	//F21	�¶��������й����������ܡ�����1~M��	
		ucRet=Main_Get_ReqData_2_F21((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F22:	//F22	�¶��������޹����������ܡ�����1~M��	
		ucRet=Main_Get_ReqData_2_F22((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F23:	//F23	�¶��ᷴ���й����������ܡ�����1~M��	
		ucRet=Main_Get_ReqData_2_F23((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F24:	//F24	�¶��ᷴ���޹����������ܡ�����1~M��	
		ucRet=Main_Get_ReqData_2_F24((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F25:	//F25	�շ�������й����ʼ�����ʱ�䡢�й�����Ϊ��ʱ��	
		ucRet=Main_Get_ReqData_2_F25((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F26:	//F26	���ܼ������������������ʱ��	
		ucRet=Main_Get_ReqData_2_F26((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F27:	//F27	�յ�ѹͳ������	
		ucRet=Main_Get_ReqData_2_F27((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F28:	//F28	�ղ�ƽ���Խ���ۼ�ʱ��	
		ucRet=Main_Get_ReqData_2_F28((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F29:	//F29	�յ���Խ��ͳ��	
		ucRet=Main_Get_ReqData_2_F29((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F30:	//F30	�����ڹ���Խ���ۼ�ʱ��	
		ucRet=Main_Get_ReqData_2_F30((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F31:	//F31	�յ��ܱ��������	
	//	ucRet=Main_Get_ReqData_2_F31(usDAp,pucRetBuffer,&m,pstStartTime);
		ucRet=RET_ERROR;
		break;
	case	CON_DATA_REQ2_F32:	//F32	����	
		ucRet=Main_Get_ReqData_2_F31((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);	
		break;
	case	CON_DATA_REQ2_F33:	//F33	�·�������й����ʼ�����ʱ�䡢�����й�����Ϊ��ʱ��	
		ucRet=Main_Get_ReqData_2_F33((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F34:	//F34	�·����й��������������ʱ�䡢���й��������������ʱ��	
		ucRet=Main_Get_ReqData_2_F34((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F35:	//F35	�µ�ѹͳ������	
		ucRet=Main_Get_ReqData_2_F35((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F36:	//F36	�²�ƽ���Խ���ۼ�ʱ��	
		ucRet=Main_Get_ReqData_2_F36((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F37:	//F37	�µ���Խ��ͳ��	
		ucRet=Main_Get_ReqData_2_F37((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F38:	//F38	�����ڹ���Խ���ۼ�ʱ��	
		ucRet=Main_Get_ReqData_2_F38((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
//	case	CON_DATA_REQ2_F41:	//F41	������Ͷ���ۼ�ʱ��ʹ���	
//		ucRet=RET_ERROR;
//		break;
//	case	CON_DATA_REQ2_F42:	//F42	�ա��µ������ۼƲ������޹�������	
//		ucRet=RET_ERROR;
//		break;
	case	CON_DATA_REQ2_F43:	//F43	�չ������������ۼ�ʱ��	
		ucRet=Main_Get_ReqData_2_F43((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F44:	//F44	�¹������������ۼ�ʱ��	
		ucRet=Main_Get_ReqData_2_F44((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F49:	//F49	�ն��չ���ʱ�䡢�ո�λ�ۼƴ���	
		ucRet=Main_Get_ReqData_2_F49((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F50:	//F50	�ն��տ���ͳ������	
//		ucRet=Main_Get_ReqData_2_F50((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F51:	//F51	�ն��¹���ʱ�䡢�¸�λ�ۼƴ���	
		ucRet=Main_Get_ReqData_2_F51((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
//	case	CON_DATA_REQ2_F52:	//F52	�ն��¿���ͳ������	
////		ucRet=Main_Get_ReqData_2_F52((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
//		break;
//	case	CON_DATA_REQ2_F57:	//F57	�ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��	
////		ucRet=Main_Get_ReqData_2_F57((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
//		break;
//	case	CON_DATA_REQ2_F58:	//F58	�ܼ������ۼ��й����������ܡ�����1~M��	
////		ucRet=Main_Get_ReqData_2_F58((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
//		break;
//	case	CON_DATA_REQ2_F59:	//F59	�ܼ������ۼ��޹����������ܡ�����1~M��	
////		ucRet=Main_Get_ReqData_2_F59((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
//		break;
//	case	CON_DATA_REQ2_F60:	//F60	�ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��	
////		ucRet=Main_Get_ReqData_2_F60((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
//		break;
//	case	CON_DATA_REQ2_F61:	//F61	�ܼ������ۼ��й����������ܡ�����1~M��	
////		ucRet=Main_Get_ReqData_2_F61((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
//		break;
//	case	CON_DATA_REQ2_F62:	//F62	�ܼ������ۼ��޹����������ܡ�����1~M��	
////		ucRet=Main_Get_ReqData_2_F62((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
//		break;
//	case	CON_DATA_REQ2_F65:	//F65	�ܼ��鳬���ʶ�ֵ�����ۼ�ʱ�䡢���ۼƵ�����	
////		ucRet=Main_Get_ReqData_2_F65((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
//		break;
//	case	CON_DATA_REQ2_F66:	//F66	�ܼ��鳬�µ�������ֵ�����ۼ�ʱ�䡢�ۼƵ�����	
////		ucRet=Main_Get_ReqData_2_F66((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
//		break;
//	case	CON_DATA_REQ2_F73:	//F73	�ܼ����й���������	
////		ucRet=Main_Get_ReqData_2_F73((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
//		break;
//	case	CON_DATA_REQ2_F74:	//F74	�ܼ����޹���������	
////		ucRet=Main_Get_ReqData_2_F74((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
//		break;
//	case	CON_DATA_REQ2_F75:	//F75	�ܼ����й�����������	
////		ucRet=Main_Get_ReqData_2_F75((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
//		break;
//	case	CON_DATA_REQ2_F76:	//F76	�ܼ����޹�����������	
////		ucRet=Main_Get_ReqData_2_F76((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
//		break;
	case	CON_DATA_REQ2_F81:	//F81	�й���������	
		ucRet=Main_Get_ReqData_2_F81((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F82:	//F82	A���й���������	
		ucRet=Main_Get_ReqData_2_F82((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F83:	//F83	B���й���������	
		ucRet=Main_Get_ReqData_2_F83((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F84:	//F84	C���й���������	
		ucRet=Main_Get_ReqData_2_F84((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F85:	//F85	�޹���������	
		ucRet=Main_Get_ReqData_2_F85((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F86:	//F86	A���޹���������	
		ucRet=Main_Get_ReqData_2_F86((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F87:	//F87	B���޹���������	
		ucRet=Main_Get_ReqData_2_F87((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F88:	//F88	C���޹���������	
		ucRet=Main_Get_ReqData_2_F88((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F89:	//F89	A���ѹ����	
		ucRet=Main_Get_ReqData_2_F89((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F90:	//F90	B���ѹ����	
		ucRet=Main_Get_ReqData_2_F90((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F91:	//F91	C���ѹ����	
		ucRet=Main_Get_ReqData_2_F91((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F92:	//F92	A���������	
		ucRet=Main_Get_ReqData_2_F92((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F93:	//F93	B���������	
		ucRet=Main_Get_ReqData_2_F93((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F94:	//F94	C���������	
		ucRet=Main_Get_ReqData_2_F94((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F95:	//F95	�����������	
		ucRet=Main_Get_ReqData_2_F95((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F97:	//F97	�����й��ܵ�����	
		ucRet=Main_Get_ReqData_2_F97((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F98:	//F98	�����޹��ܵ�����	
		ucRet=Main_Get_ReqData_2_F98((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F99:	//F99	�����й��ܵ�����	
		ucRet=Main_Get_ReqData_2_F99((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F100:	//F100	�����޹��ܵ�����	
		ucRet=Main_Get_ReqData_2_F100((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F101:	//F101	�����й��ܵ���ʾֵ	
		ucRet=Main_Get_ReqData_2_F101((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F102:	//F102	�����޹��ܵ���ʾֵ	
		ucRet=Main_Get_ReqData_2_F102((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F103:	//F103	�����й��ܵ���ʾֵ	
		ucRet=Main_Get_ReqData_2_F103((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F104:	//F104	�����޹��ܵ���ʾֵ	
		ucRet=Main_Get_ReqData_2_F104((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F105:	//F105	�ܹ�������	
		ucRet=Main_Get_ReqData_2_F105((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F106:	//F106	A�๦������	
		ucRet=Main_Get_ReqData_2_F106((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F107:	//F107	B�๦������	
		ucRet=Main_Get_ReqData_2_F107((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F108:	//F108	C�๦������	
		ucRet=Main_Get_ReqData_2_F108((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F109:	
		ucRet=Main_Get_ReqData_2_F109((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F110:	
		ucRet=Main_Get_ReqData_2_F110((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
//	case	CON_DATA_REQ2_F113:	//F113	A��2~19��г�����������ֵ������ʱ��	
//		ucRet=RET_ERROR;
//		break;
//	case	CON_DATA_REQ2_F114:	//F114	B��2~19��г�����������ֵ������ʱ��	
//		ucRet=RET_ERROR;
//		break;
//	case	CON_DATA_REQ2_F115:	//F115	C��2~19��г�����������ֵ������ʱ��	
//		ucRet=RET_ERROR;
//		break;
//	case	CON_DATA_REQ2_F116:	//F116	A��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��	
//		ucRet=RET_ERROR;
//		break;
//	case	CON_DATA_REQ2_F117:	//F117	B��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��	
//		ucRet=RET_ERROR;
//		break;
//	case	CON_DATA_REQ2_F118:	//F118	C��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��	
//		ucRet=RET_ERROR;
//		break;
//	case	CON_DATA_REQ2_F121:	//F121	A��г��Խ����ͳ������	
//		ucRet=RET_ERROR;
//		break;
//	case	CON_DATA_REQ2_F122:	//F122	B��г��Խ����ͳ������	
//		ucRet=RET_ERROR;
//		break;
//	case	CON_DATA_REQ2_F123:	//F123	C��г��Խ����ͳ������	
//		ucRet=RET_ERROR;
//		break;
//	case	CON_DATA_REQ2_F129:	//F129	ֱ��ģ����Խ�����ۼ�ʱ�䡢���/��Сֵ������ʱ��	
//		ucRet=RET_ERROR;
//		break;
//	case	CON_DATA_REQ2_F130:	//F130	ֱ��ģ����Խ�����ۼ�ʱ�䡢���/��Сֵ������ʱ��	
//		ucRet=RET_ERROR;
//		break;
//	case	CON_DATA_REQ2_F138:	//F138	ֱ��ģ������������	
//		ucRet=RET_ERROR;
//		break;
	case	CON_DATA_REQ2_F145:	
		ucRet=Main_Get_ReqData_2_F145((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F146:	
		ucRet=Main_Get_ReqData_2_F146((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F147:	
		ucRet=Main_Get_ReqData_2_F147((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F148:	
		ucRet=Main_Get_ReqData_2_F148((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F153:	
		ucRet=Main_Get_ReqData_2_PhasePowData((unsigned char)usDAp,pucRetBuffer,&m,CON_DI_TYPE_MPDIRPOWA,pstStartTime);
		break;
	case	CON_DATA_REQ2_F154:	
		ucRet=Main_Get_ReqData_2_PhasePowData((unsigned char)usDAp,pucRetBuffer,&m,CON_DI_TYPE_MPUNDIRPOWA,pstStartTime);
		break;
	case	CON_DATA_REQ2_F155:	
		ucRet=Main_Get_ReqData_2_PhasePowData((unsigned char)usDAp,pucRetBuffer,&m,CON_DI_TYPE_MPCOMVAR1A,pstStartTime);
		break;
	case	CON_DATA_REQ2_F156:	
		ucRet=Main_Get_ReqData_2_PhasePowData((unsigned char)usDAp,pucRetBuffer,&m,CON_DI_TYPE_MPCOMVAR2A,pstStartTime);
		break;
	case	CON_DATA_REQ2_F157:	
		ucRet=Main_Get_ReqData_2_PhasePowMonData((unsigned char)usDAp,pucRetBuffer,&m,CON_DI_TYPE_MPDIRPOWA,pstStartTime);
		break;
	case	CON_DATA_REQ2_F158:	
		ucRet=Main_Get_ReqData_2_PhasePowMonData((unsigned char)usDAp,pucRetBuffer,&m,CON_DI_TYPE_MPUNDIRPOWA,pstStartTime);
		break;
	case	CON_DATA_REQ2_F159:	
		ucRet=Main_Get_ReqData_2_PhasePowMonData((unsigned char)usDAp,pucRetBuffer,&m,CON_DI_TYPE_MPCOMVAR1A,pstStartTime);
		break;
	case	CON_DATA_REQ2_F160:	
		ucRet=Main_Get_ReqData_2_PhasePowMonData((unsigned char)usDAp,pucRetBuffer,&m,CON_DI_TYPE_MPCOMVAR2A,pstStartTime);
		break;
	case	CON_DATA_REQ2_F161:	
		ucRet=Main_Get_ReqData_2_F161((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);//,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F162:	
		ucRet=Main_Get_ReqData_2_F162((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F163:	
		ucRet=Main_Get_ReqData_2_F163((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F164:	
		ucRet=Main_Get_ReqData_2_F164((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F165:	
		ucRet=Main_Get_ReqData_2_F165((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F166:	
		ucRet=Main_Get_ReqData_2_F166((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F167:	
		ucRet=Main_Get_ReqData_2_F167((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F168:	
		ucRet=Main_Get_ReqData_2_F168((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F169:	
		ucRet=Main_Get_ReqData_2_F169((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F170:	
		ucRet=Main_Get_ReqData_2_F170((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F171:	
		ucRet=Main_Get_ReqData_2_F171((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F172:	
		ucRet=Main_Get_ReqData_2_F172((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F173:	
		ucRet=Main_Get_ReqData_2_F173((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F174:	
		ucRet=Main_Get_ReqData_2_F174((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F175:	
		ucRet=Main_Get_ReqData_2_F175((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F176:	
		ucRet=Main_Get_ReqData_2_F176((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F177:	
		ucRet=Main_Get_ReqData_2_F177((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F178:	
		ucRet=Main_Get_ReqData_2_F178((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F179:	
		ucRet=Main_Get_ReqData_2_F179((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F180:	
		ucRet=Main_Get_ReqData_2_F180((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F181:	
		ucRet=Main_Get_ReqData_2_F181((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F182:	
		ucRet=Main_Get_ReqData_2_F182((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F183:	
		ucRet=Main_Get_ReqData_2_F183((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F184:	
		ucRet=Main_Get_ReqData_2_F184((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F185:	
		ucRet=Main_Get_ReqData_2_F185((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F186:	
		ucRet=Main_Get_ReqData_2_F186((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F187:	
		ucRet=Main_Get_ReqData_2_F187((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F188:	
		ucRet=Main_Get_ReqData_2_F188((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F189:	
		ucRet=Main_Get_ReqData_2_F189((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F190:	
		ucRet=Main_Get_ReqData_2_F190((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F191:	
		ucRet=Main_Get_ReqData_2_F191((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F192:	
		ucRet=Main_Get_ReqData_2_F192((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F193:	
		ucRet=Main_Get_ReqData_2_F193((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F194:	
		ucRet=Main_Get_ReqData_2_F194((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F195:	
		ucRet=Main_Get_ReqData_2_F195((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F196:	
		ucRet=Main_Get_ReqData_2_F196((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
//azh 170923
	case	CON_DATA_REQ2_F219:	//F219	��������������������ݵ�Ԫ��ʽ
		ucRet=Main_Get_ReqData_2_F219((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	default:
	    ucRet=RET_ERROR;
		break;
	}
	(*pusPos)=m;
	return ucRet;
}



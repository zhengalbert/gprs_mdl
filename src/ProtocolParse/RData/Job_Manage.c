/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Job_Manage.c
 Description    : 1类数据/2类数据的任务上送、数据查询管理
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"


static unsigned char g_ucJobExeNo[2];

static unsigned char  Job_TaskGetParam_Data(STDATAUNITID stJobPara,unsigned char ucRepType,unsigned char * ucRetBuf,STDATETIME stDataBeginTime,STDATETIME stDataEndTime,unsigned char ucDataInter);
static unsigned char Get_Class2Data_TaskParam(STDATAUNITID stDUID,unsigned char * ucRetBuf,STDATETIME stDataBeginTime,STDATETIME stDataEndTime,unsigned char ucDataInter);
static STDATETIME Job_Adj_DataTime(STDATETIME *psBaseTime,unsigned char ucTimeInter,unsigned char ucTimeUnit,unsigned char ucResendDataInter);

/******************************************************************************
** 函数名称: void SaveJobRunInfo(unsigned char ucJobClassNo, unsigned char ucJobNo)
** 函数功能: 1类、2类任务运行参数的保存到EEPROM,这里主要是任务下次上报时间
** 入口参数: ucJobClassNo: 任务类型0:1类 1:2类 ucJobNo:任务号0~CONDATAJOBNUM(16)
** 返 回 值: 无
** 说    明: 无
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
** 函数名称: void GetJobRunInfoFormEE (void)
** 函数功能: 1类、2类任务运行参数的保存到EEPROM,这里主要是任务下次上报时间
** 入口参数: ucJobClassNo: 任务类型0:1类 1:2类 ucJobNo:任务号0~CONDATAJOBNUM(16)
** 返 回 值: 无
** 说    明: 无
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
	Job_ProcData(0);//1 1类数据任务处理
	Job_ProcData(1);//2 2类数据任务处理
}

//1类数据任务处理
unsigned char Job_ProcData(unsigned char ucJobClassNo)
{
	unsigned char  mem_type,mem1_type,*pret_mem = NULL,*mem_ptr = NULL;//azh
	unsigned char ucRet=RET_SUCCESS;
	unsigned char iucDUIDNum;
	STDATETIME  stCurrTime=GetCurrentTime();
	unsigned long lCurrTime,lNextExeTime,ulNum;

	unsigned char ucDUIDNum=0,ilen=0;
	unsigned char ucLink=11;				//不需要主站确认
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
//azh 没有级联都有主模式
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
		{//1次处理1个job
			stCurrTime=GetCurrentTime();
			lCurrTime=TimeConver(&stCurrTime,CON_TIMEUNIT_SEC);
			lNextExeTime=TimeConver(&pstJobPara_Data1->stNextExecute,CON_TIMEUNIT_SEC);
			if(lCurrTime>=lNextExeTime)//上送时间到
			{
				Clr_Group_RetData();
				Clr_Group_RetExtData();
				stDataBeginTime=Job_Adj_DataTime(&(pstJobPara_Data1->stNextExecute),
					pstJobPara_Data1->ucTimeInter,
					pstJobPara_Data1->ucTimeUnit,
					pstJobPara_Data1->ucResendDataInter);	//2类数据任务需要关注数据发生结束等时间
				ulNum=TimeConverToSecond(pstJobPara_Data1->ucTimeInter,pstJobPara_Data1->ucTimeUnit);
				if(ulNum==0)
					ulNum=60;		//1分钟
				ulNum=(lCurrTime-lNextExeTime)/ulNum+1;					//下次执行时间
				AddTime(&pstJobPara_Data1->stNextExecute,ulNum*pstJobPara_Data1->ucTimeInter,pstJobPara_Data1->ucTimeUnit);
				stDataEndTime=Job_Adj_DataTime(&(pstJobPara_Data1->stNextExecute),
					pstJobPara_Data1->ucTimeInter,
					pstJobPara_Data1->ucTimeUnit,
					pstJobPara_Data1->ucResendDataInter);	//2类数据任务需要关注数据发生结束等时间
//				RunPara_SetJobPara(ucJobClassNo,g_ucJobExeNo[ucJobClassNo],&stJobPara_Data1);//azh 前面就是全局变量的指针 所以不用再保存设置
                SaveJobRunInfo(ucJobClassNo,g_ucJobExeNo[ucJobClassNo]);//azh 保存任务下次运行时间
                
				if(pstJobPara_Data1->ucTimeUnit==CON_TIMEUNIT_MON)		//处理按月上送的抄表日数据
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
						if(ucJobClassNo==0)	//1类任务
						{
							if(pstJobPara_Data1->stDUID[iucDUIDNum].ucDT2>=0x0A && pstJobPara_Data1->stDUID[iucDUIDNum].ucDT2<=0x0E)	//小时冻结范围
							{
								if(pstJobPara_Data1->ucTimeUnit==CON_TIMEUNIT_HOUR)
								{
									ucHours=pstJobPara_Data1->ucTimeInter;
									if(ucHours==0)
										ucHours=1;
									if(ucHours>8)				//限制最多8个
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
					gucJob1HoursFlag=0xFF;			//设置无效
				}
				gucJobMonthUpFlag=CON_STATUS_NO;				//中途Main_GetSendData被利用，最后清除
				//发送到端口
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
	if(ucDT2<31 && ucDT1>0)							//>=31未定义数据内容
	{
		for(i=0;i<8;i++)
		{
			if((ucDT1 & 0x01) == 0x01)
			{
				pucDTList[ucCount++] =i+ucDT2*0x08+1;	//从1开始
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
			if(ucDACount>=8)					//最多8点
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
			case	1:		//F1	日冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
			case	2:		//F2	日冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
			case	3:		//F3	日冻结正向有/无功最大需量及发生时间（总、费率1~M）
			case	4:		//F4	日冻结反向有/无功最大需量及发生时间（总、费率1~M）
			case	5:		//F5	日冻结正向有功电能量（总、费率1~M）
			case	6:		//F6	日冻结正向无功电能量（总、费率1~M）
			case	7:		//F7	日冻结反向有功电能量（总、费率1~M）
			case	8:		//F8	日冻结反向无功电能量（总、费率1~M）
			case	9:		//F9	抄表日冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
			case	10:		//F10	抄表日冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
			case	11:		//F11	抄表日冻结正向有/无功最大需量及发生时间（总、费率1~M）
			case	12:		//F12	抄表日冻结反向有/无功最大需量及发生时间（总、费率1~M）
			case	25:		//F25	日分相最大有功功率及发生时间、有功功率为零时间
			case	26:		//F26	日总及分相最大需量及发生时间
			case	27:		//F27	日电压统计数据
			case	28:		//F28	日不平衡度越限累计时间
			case	29:		//F29	日电流越限统计
			case	30:		//F30	日视在功率越限累计时间
			case	31:		//F31	日电能表断相数据
			case	41:		//F41	电容器投入累计时间和次数
			case	42:		//F42	日、月电容器累计补偿的无功电能量
			case	43:		//F43	日功率因数区段累计时间
			case	49:		//F49	终端日供电时间、日复位累计次数
			case	50:		//F50	终端日控制统计数据
			case	57:		//F57	总加组日最大、最小有功功率及其发生时间，有功功率为零日累计时间
			case	58:		//F58	总加组日累计有功电能量（总、费率1~M）
			case	59:		//F59	总加组日累计无功电能量（总、费率1~M）
			case	113:		//F113	A相2~19次谐波电流日最大值及发生时间
			case	114:		//F114	B相2~19次谐波电流日最大值及发生时间
			case	115:		//F115	C相2~19次谐波电流日最大值及发生时间
			case	116:		//F116	A相2~19次谐波电压含有率及总畸变率日最大值及发生时间
			case	117:		//F117	B相2~19次谐波电压含有率及总畸变率日最大值及发生时间
			case	118:		//F118	C相2~19次谐波电压含有率及总畸变率日最大值及发生时间
			case	121:		//F121	A相谐波越限日统计数据
			case	122:		//F122	B相谐波越限日统计数据
			case	123:		//F123	C相谐波越限日统计数据
			case	129:		//F129	直流模拟量越限日累计时间、最大/最小值及发生时间
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
			case	17:		//F17	月冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
			case	18:		//F18	月冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
			case	19:		//F19	月冻结正向有/无功最大需量及发生时间（总、费率1~M）
			case	20:		//F20	月冻结反向有/无功最大需量及发生时间（总、费率1~M）
			case	21:		//F21	月冻结正向有功电能量（总、费率1~M）
			case	22:		//F22	月冻结正向无功电能量（总、费率1~M）
			case	23:		//F23	月冻结反向有功电能量（总、费率1~M）
			case	24:		//F24	月冻结反向无功电能量（总、费率1~M）
			case	33:		//F33	月分相最大有功功率及发生时间、分相有功功率为零时间
			case	34:		//F34	月分相有功最大需量及发生时间、总有功最大需量及发生时间
			case	35:		//F35	月电压统计数据
			case	36:		//F36	月不平衡度越限累计时间
			case	37:		//F37	月电流越限统计
			case	38:		//F38	月视在功率越限累计时间
			case	44:		//F44	月功率因数区段累计时间
			case	51:		//F51	终端月供电时间、月复位累计次数
			case	52:		//F52	终端月控制统计数据
			case	60:		//F60	总加组月最大、最小有功功率及其发生时间，有功功率为零月累计时间
			case	61:		//F61	总加组月累计有功电能量（总、费率1~M）
			case	62:		//F62	总加组月累计无功电能量（总、费率1~M）				
			case	65:		//F65	总加组超功率定值的月累计时间、月累计电能量
			case	66:		//F66	总加组超月电能量定值的月累计时间、累计电能量
			case	130:		//F130	直流模拟量越限月累计时间、最大/最小值及发生时间
			case 	157:
			case 	158:
			case 	159:
			case 	160:				
				if(ucPos+2>512)
					break;
				ucRetBuf[ucPos++]=ucMonth;
				ucRetBuf[ucPos++]=ucYear;
				break;
			case	73:		//F73	总加组有功功率曲线
			case	74:		//F74	总加组无功功率曲线
			case	75:		//F75	总加组有功电能量曲线
			case	76:		//F76	总加组无功电能量曲线
			case	81:		//F81	有功功率曲线
			case	82:		//F82	A相有功功率曲线
			case	83:		//F83	B相有功功率曲线
			case	84:		//F84	C相有功功率曲线
			case	85:		//F85	无功功率曲线
			case	86:		//F86	A相无功功率曲线
			case	87:		//F87	B相无功功率曲线
			case	88:		//F88	C相无功功率曲线
			case	89:		//F89	A相电压曲线
			case	90:		//F90	B相电压曲线
			case	91:		//F91	C相电压曲线
			case	92:		//F92	A相电流曲线
			case	93:		//F93	B相电流曲线
			case	94:		//F94	C相电流曲线
			case	95:		//F95	零序电流曲线
			case	97:		//F97	正向有功总电能量
			case	98:		//F98	正向无功总电能量
			case	99:		//F99	反向有功总电能量
			case	100:		//F100	反向无功总电能量
			case	101:		//F101	正向有功总电能示值
			case	102:		//F102	正向无功总电能示值
			case	103:		//F103	反向有功总电能示值
			case	104:		//F104	反向无功总电能示值
			case	105:		//F105	总功率因数
			case	106:		//F106	A相功率因数
			case	107:		//F107	B相功率因数
			case	108:		//F108	C相功率因数
			case	138:		//F138	直流模拟量数据曲线
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
				
			case	32:		//F32	备用
				break;
			}
		}
	}
	return ucPos;
}
void TimeChange_JobManage(STDATETIME stOldTime,STDATETIME stNewTime)
{//时钟变化处理
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
        SaveJobRunInfo(0,i);//azh 保存任务下次运行时间
		//HdConvertWatchDog();
		pstJobPara_Data=RunPara_GetJobPara(i,1);
		pstJobPara_Data->stNextExecute=RunPara_GetJobNextTime(
									pstJobPara_Data->stStartActive,
									pstJobPara_Data->ucTimeInter,
									pstJobPara_Data->ucTimeUnit);
//		RunPara_SetJobPara(1,i,&stJobPara_Data);
        SaveJobRunInfo(1,i);//azh 保存任务下次运行时间
	}
}

/*void TimeChange_JobManage(STDATETIME stOldTime,STDATETIME stNewTime)
{//时钟变化处理
	unsigned char i=0,j=0;
	unsigned long ulOldTime,ulNewTime,ulDiff;
	STJOBPARA	stJobPara_Data;
	ulOldTime=TimeConver(&stOldTime,CON_TIMEUNIT_SEC);
	ulNewTime=TimeConver(&stNewTime,CON_TIMEUNIT_SEC);
	if(ulOldTime>ulNewTime)
	{//时间向旧改
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
	{//时间向新改
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
{//当前时间调整为开始时间，然后减去任务时间间隔
	STDATETIME stDateTime;
	stDateTime=(*psBaseTime);
	switch(ucTimeUnit)
	{
	case CON_TIMEUNIT_MON:		//月
		stDateTime.ucDay=1;
	case CON_TIMEUNIT_DAY:		//日
		stDateTime.ucHour=0;
	case CON_TIMEUNIT_HOUR:		//时
		stDateTime.ucMinute=0;
	case CON_TIMEUNIT_MIN:		//分
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

//仅仅2类任务以月为周期上送抄表日冻结数据才需要调整
//待处理的时间是以月为周期，时间为上月1日0时0分0秒
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
			{//第一个抄表日时间没到，则选择上月最后一个抄表日
				for(i=0;i<3;i++)		//限制3个抄表日
				{
					if(pstFreezeDayPara->stFreezeDay[i].ucDay>31)
					{
						break;
					}
				}
				if(i>0)
					i=i-1;			//选择有效抄表日
				psTime->ucDay=pstFreezeDayPara->stFreezeDay[i].ucDay;
				psTime->ucHour=pstFreezeDayPara->stFreezeDay[i].ucHour;
				psTime->ucMinute=pstFreezeDayPara->stFreezeDay[i].ucMinute;
				psTime->ucSecond=pstFreezeDayPara->stFreezeDay[i].ucSecond;
			}
			else
			{//本月选择一个抄表日
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
					psTime->ucDay=28;			//强制转换
			}
		}
	}
}
//获得任务数据
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
//	Main_GetDAList(pucRetBuffer+m-4,4,0,usDAList);			//获取信息点列表
//	Main_GetDTList(pucRetBuffer+m-4,4,0,ucDTList);			//获取信息类列表
    Main_GetDADTList(pucRetBuffer+m-4, usDAList, ucDTList);	//获取信息点、信息类列表	
    
	usDAp=usDAList[0];
	ucDTF=ucDTList[0];
	
	usItemLabel=GetItemLabelbyDT(CON_TERMSG_REQDATA_1,ucDTF);
	switch(usItemLabel)
	{
	case	CON_DATA_REQ1_F1:	//F1	终端版本信息
		ucRet=Main_Get_ReqData_1_F1((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F2:	//F2	终端日历时钟
		ucRet=Main_Get_ReqData_1_F2((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F3:	//F3	终端参数状态
		ucRet=Main_Get_ReqData_1_F3((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F4:	//F4	终端通信状态
		ucRet=Main_Get_ReqData_1_F4((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F5:	//F5	终端控制设置状态
		ucRet=Main_Get_ReqData_1_F5((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F6:	//F6	终端当前控制状态
		ucRet=Main_Get_ReqData_1_F6((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F7:	//F7	终端事件计数器当前值
		ucRet=Main_Get_ReqData_1_F7((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F8:	//F8	终端事件标志状态
		ucRet=Main_Get_ReqData_1_F8((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F9:	//F9	终端状态量及变位标志
		ucRet=Main_Get_ReqData_1_F9((unsigned char)usDAp,pucRetBuffer,&m);
		break;
//	case	CON_DATA_REQ1_F17:	//F17	当前总加有功功率
//		ucRet=Main_Get_ReqData_1_F17((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F18:	//F18	当前总加无功功率
//		ucRet=Main_Get_ReqData_1_F18((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F19:	//F19	当日总加有功电能量（总、费率1~M）
//		ucRet=Main_Get_ReqData_1_F19((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F20:	//F20	当日总加无功电能量（总、费率1~M）
//		ucRet=Main_Get_ReqData_1_F20((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F21:	//F21	当月总加有功电能量（总、费率1~M）
//		ucRet=Main_Get_ReqData_1_F21((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F22:	//F22	当月总加无功电能量（总、费率1~M）
//		ucRet=Main_Get_ReqData_1_F22((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F23:	//F23	终端当前剩余电量（费）
//		ucRet=Main_Get_ReqData_1_F23((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F24:	//F24	当前功率下浮控控后总加有功功率冻结值
//		ucRet=Main_Get_ReqData_1_F24((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
	case	CON_DATA_REQ1_F25:	//F25	当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流
		ucRet=Main_Get_ReqData_1_F25((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F26:	//F26	A、B、C三相断相统计数据及最近一次断相记录
		ucRet=Main_Get_ReqData_1_F26((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F27:	//F27	电能表日历时钟、编程次数及其最近一次操作时间
		ucRet=Main_Get_ReqData_1_F27((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F28:	//F28	电表运行状态字及其变位标志
		ucRet=Main_Get_ReqData_1_F28((unsigned char)usDAp,pucRetBuffer,&m);
		break;
/*	case 	CON_DATA_REQ1_F31:
		ucRet = Main_Get_ReqData_1_F31((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case 	CON_DATA_REQ1_F32:
		ucRet = Main_Get_ReqData_1_F32((unsigned char)usDAp,pucRetBuffer,&m);
		break;*/
	case	CON_DATA_REQ1_F33:	//F33	当前正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
		ucRet=Main_Get_ReqData_1_F33((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F34:	//F34	当前反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
		ucRet=Main_Get_ReqData_1_F34((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F35:	//F35	当月正向有/无功最大需量及发生时间（总、费率1~M）
		ucRet=Main_Get_ReqData_1_F35((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F36:	//F36	当月反向有/无功最大需量及发生时间（总、费率1~M）
		ucRet=Main_Get_ReqData_1_F36((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F37:	//F37	上月正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
		ucRet=Main_Get_ReqData_1_F37((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F38:	//F38	上月反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
		ucRet=Main_Get_ReqData_1_F38((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F39:	//F39	上月正向有/无功最大需量及发生时间（总、费率1~M）
		ucRet=Main_Get_ReqData_1_F39((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F40:	//F40	上月反向有/无功最大需量及发生时间（总、费率1~M）
		ucRet=Main_Get_ReqData_1_F40((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F41:	//F41	当日正向有功电能量（总、费率1~M）
		ucRet=Main_Get_ReqData_1_F41((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F42:	//F42	当日正向无功电能量（总、费率1~M）
		ucRet=Main_Get_ReqData_1_F42((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F43:	//F43	当日反向有功电能量（总、费率1~M）
		ucRet=Main_Get_ReqData_1_F43((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F44:	//F44	当日反向无功电能量（总、费率1~M）
		ucRet=Main_Get_ReqData_1_F44((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F45:	//F45	当月正向有功电能量（总、费率1~M）
		ucRet=Main_Get_ReqData_1_F45((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F46:	//F46	当月正向无功电能量（总、费率1~M）
		ucRet=Main_Get_ReqData_1_F46((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F47:	//F47	当月反向有功电能量（总、费率1~M）
		ucRet=Main_Get_ReqData_1_F47((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F48:	//F48	当月反向无功电能量（总、费率1~M）
		ucRet=Main_Get_ReqData_1_F48((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F49:	//F49	当前电压、电流相位角
		ucRet=Main_Get_ReqData_1_F49((unsigned char)usDAp,pucRetBuffer,&m);
		break;
//	case	CON_DATA_REQ1_F57:	//F57	当前A、B、C三相电压、电流2~N次谐波有效值
//		ucRet=Main_Get_ReqData_1_F57((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F58:	//F58	当前A、B、C三相电压、电流2~N次谐波含有率
//		ucRet=Main_Get_ReqData_1_F58((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F65:	//F65	当前电容器投切状态
//		ucRet=Main_Get_ReqData_1_F65((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F66:	//F66	当前电容器累计补偿投入时间和次数
//		ucRet=Main_Get_ReqData_1_F66((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F67:	//F67	当日、当月电容器累计补偿的无功电能量
//		ucRet=Main_Get_ReqData_1_F67((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F73:	//F73	直流模拟量实时数据
//		ucRet=Main_Get_ReqData_1_F73((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F81:	//F81	小时冻结总加有功功率
//		ucRet=Main_Get_ReqData_1_F81((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F82:	//F82	小时冻结总加无功功率
//		ucRet=Main_Get_ReqData_1_F82((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F83:	//F83	小时冻结总加有功总电能量
//		ucRet=Main_Get_ReqData_1_F83((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
//	case	CON_DATA_REQ1_F84:	//F84	小时冻结总加无功总电能量
//		ucRet=Main_Get_ReqData_1_F84((unsigned char)usDAp,pucRetBuffer,&m);
//		break;
	case	CON_DATA_REQ1_F89:	//F89	小时冻结有功功率
		ucRet=Main_Get_ReqData_1_F89((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F90:	//F90	小时冻结A相有功功率
		ucRet=Main_Get_ReqData_1_F90((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F91:	//F91	小时冻结B相有功功率
		ucRet=Main_Get_ReqData_1_F91((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F92:	//F92	小时冻结C相有功功率
		ucRet=Main_Get_ReqData_1_F92((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F93:	//F93	小时冻结无功功率
		ucRet=Main_Get_ReqData_1_F93((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F94:	//F94	小时冻结A相无功功率
		ucRet=Main_Get_ReqData_1_F94((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F95:	//F95	小时冻结B相无功功率
		ucRet=Main_Get_ReqData_1_F95((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F96:	//F96	小时冻结C相无功功率
		ucRet=Main_Get_ReqData_1_F96((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F97:	//F97	小时冻结A相电压
		ucRet=Main_Get_ReqData_1_F97((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F98:	//F98	小时冻结B相电压
		ucRet=Main_Get_ReqData_1_F98((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F99:	//F99	小时冻结C相电压
		ucRet=Main_Get_ReqData_1_F99((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F100:	//F100	小时冻结A相电流
		ucRet=Main_Get_ReqData_1_F100((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F101:	//F101	小时冻结B相电流
		ucRet=Main_Get_ReqData_1_F101((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F102:	//F102	小时冻结C相电流
		ucRet=Main_Get_ReqData_1_F102((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F103:	//F103	小时冻结零序电流
		ucRet=Main_Get_ReqData_1_F103((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F105:	//F105	小时冻结正向有功总电能量
		ucRet=Main_Get_ReqData_1_F105((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F106:	//F106	小时冻结正向无功总电能量
		ucRet=Main_Get_ReqData_1_F106((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F107:	//F107	小时冻结反向有功总电能量
		ucRet=Main_Get_ReqData_1_F107((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F108:	//F108	小时冻结反向无功总电能量
		ucRet=Main_Get_ReqData_1_F108((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F109:	//F109	小时冻结正向有功总电能示值
		ucRet=Main_Get_ReqData_1_F109((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F110:	//F110	小时冻结正向无功总电能示值
		ucRet=Main_Get_ReqData_1_F110((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F111:	//F111	小时冻结反向有功总电能示值
		ucRet=Main_Get_ReqData_1_F111((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F112:	//F112	小时冻结反向无功总电能示值
		ucRet=Main_Get_ReqData_1_F112((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F113:	//F113	小时冻结总功率因数
		ucRet=Main_Get_ReqData_1_F113((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F114:	//F114	小时冻结A相功率因数
		ucRet=Main_Get_ReqData_1_F114((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F115:	//F115	小时冻结B相功率因数
		ucRet=Main_Get_ReqData_1_F115((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F116:	//F116	小时冻结C相功率因数
		ucRet=Main_Get_ReqData_1_F116((unsigned char)usDAp,pucRetBuffer,&m);
		break;
	case	CON_DATA_REQ1_F121:	//F121	小时冻结直流模拟量
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

//获得任务数据
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
//	Main_GetDAList(pucRetBuffer+m-4,4,0,usDAList);			//获取信息点列表
//	Main_GetDTList(pucRetBuffer+m-4,4,0,ucDTList);			//获取信息类列表
    Main_GetDADTList(pucRetBuffer+m-4, usDAList, ucDTList);	//获取信息点、信息类列表	

	usDAp=usDAList[0];
	ucDTF=ucDTList[0];

	usItemLabel=GetItemLabelbyDT(CON_TERMSG_REQDATA_2,ucDTF);
	switch(usItemLabel)
	{
	case	CON_DATA_REQ2_F1:	//F1	日冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）	
		ucRet=Main_Get_ReqData_2_F1((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F2:	//F2	日冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）	
		ucRet=Main_Get_ReqData_2_F2((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F3:	//F3	日冻结正向有/无功最大需量及发生时间（总、费率1~M）	
		ucRet=Main_Get_ReqData_2_F3((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F4:	//F4	日冻结反向有/无功最大需量及发生时间（总、费率1~M）	
		ucRet=Main_Get_ReqData_2_F4((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F5:	//F5	日冻结正向有功电能量（总、费率1~M）	
		ucRet=Main_Get_ReqData_2_F5((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F6:	//F6	日冻结正向无功电能量（总、费率1~M）	
		ucRet=Main_Get_ReqData_2_F6((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F7:	//F7	日冻结反向有功电能量（总、费率1~M）	
		ucRet=Main_Get_ReqData_2_F7((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F8:	//F8	日冻结反向无功电能量（总、费率1~M）	
		ucRet=Main_Get_ReqData_2_F8((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F9:	//F9	抄表日冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）	
		ucRet=Main_Get_ReqData_2_F9((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F10:	//F10	抄表日冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）	
		ucRet=Main_Get_ReqData_2_F10((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F11:	//F11	抄表日冻结正向有/无功最大需量及发生时间（总、费率1~M）	
		ucRet=Main_Get_ReqData_2_F11((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F12:	//F12	抄表日冻结反向有/无功最大需量及发生时间（总、费率1~M）	
		ucRet=Main_Get_ReqData_2_F12((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F17:	//F17	月冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）	
		ucRet=Main_Get_ReqData_2_F17((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F18:	//F18	月冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）	
		ucRet=Main_Get_ReqData_2_F18((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F19:	//F19	月冻结正向有/无功最大需量及发生时间（总、费率1~M）	
		ucRet=Main_Get_ReqData_2_F19((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F20:	//F20	月冻结反向有/无功最大需量及发生时间（总、费率1~M）	
		ucRet=Main_Get_ReqData_2_F20((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F21:	//F21	月冻结正向有功电能量（总、费率1~M）	
		ucRet=Main_Get_ReqData_2_F21((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F22:	//F22	月冻结正向无功电能量（总、费率1~M）	
		ucRet=Main_Get_ReqData_2_F22((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F23:	//F23	月冻结反向有功电能量（总、费率1~M）	
		ucRet=Main_Get_ReqData_2_F23((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F24:	//F24	月冻结反向无功电能量（总、费率1~M）	
		ucRet=Main_Get_ReqData_2_F24((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F25:	//F25	日分相最大有功功率及发生时间、有功功率为零时间	
		ucRet=Main_Get_ReqData_2_F25((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F26:	//F26	日总及分相最大需量及发生时间	
		ucRet=Main_Get_ReqData_2_F26((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F27:	//F27	日电压统计数据	
		ucRet=Main_Get_ReqData_2_F27((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F28:	//F28	日不平衡度越限累计时间	
		ucRet=Main_Get_ReqData_2_F28((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F29:	//F29	日电流越限统计	
		ucRet=Main_Get_ReqData_2_F29((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F30:	//F30	日视在功率越限累计时间	
		ucRet=Main_Get_ReqData_2_F30((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F31:	//F31	日电能表断相数据	
	//	ucRet=Main_Get_ReqData_2_F31(usDAp,pucRetBuffer,&m,pstStartTime);
		ucRet=RET_ERROR;
		break;
	case	CON_DATA_REQ2_F32:	//F32	备用	
		ucRet=Main_Get_ReqData_2_F31((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);	
		break;
	case	CON_DATA_REQ2_F33:	//F33	月分相最大有功功率及发生时间、分相有功功率为零时间	
		ucRet=Main_Get_ReqData_2_F33((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F34:	//F34	月分相有功最大需量及发生时间、总有功最大需量及发生时间	
		ucRet=Main_Get_ReqData_2_F34((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F35:	//F35	月电压统计数据	
		ucRet=Main_Get_ReqData_2_F35((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F36:	//F36	月不平衡度越限累计时间	
		ucRet=Main_Get_ReqData_2_F36((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F37:	//F37	月电流越限统计	
		ucRet=Main_Get_ReqData_2_F37((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F38:	//F38	月视在功率越限累计时间	
		ucRet=Main_Get_ReqData_2_F38((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
//	case	CON_DATA_REQ2_F41:	//F41	电容器投入累计时间和次数	
//		ucRet=RET_ERROR;
//		break;
//	case	CON_DATA_REQ2_F42:	//F42	日、月电容器累计补偿的无功电能量	
//		ucRet=RET_ERROR;
//		break;
	case	CON_DATA_REQ2_F43:	//F43	日功率因数区段累计时间	
		ucRet=Main_Get_ReqData_2_F43((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F44:	//F44	月功率因数区段累计时间	
		ucRet=Main_Get_ReqData_2_F44((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F49:	//F49	终端日供电时间、日复位累计次数	
		ucRet=Main_Get_ReqData_2_F49((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F50:	//F50	终端日控制统计数据	
//		ucRet=Main_Get_ReqData_2_F50((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
	case	CON_DATA_REQ2_F51:	//F51	终端月供电时间、月复位累计次数	
		ucRet=Main_Get_ReqData_2_F51((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
		break;
//	case	CON_DATA_REQ2_F52:	//F52	终端月控制统计数据	
////		ucRet=Main_Get_ReqData_2_F52((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
//		break;
//	case	CON_DATA_REQ2_F57:	//F57	总加组日最大、最小有功功率及其发生时间，有功功率为零日累计时间	
////		ucRet=Main_Get_ReqData_2_F57((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
//		break;
//	case	CON_DATA_REQ2_F58:	//F58	总加组日累计有功电能量（总、费率1~M）	
////		ucRet=Main_Get_ReqData_2_F58((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
//		break;
//	case	CON_DATA_REQ2_F59:	//F59	总加组日累计无功电能量（总、费率1~M）	
////		ucRet=Main_Get_ReqData_2_F59((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
//		break;
//	case	CON_DATA_REQ2_F60:	//F60	总加组月最大、最小有功功率及其发生时间，有功功率为零月累计时间	
////		ucRet=Main_Get_ReqData_2_F60((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
//		break;
//	case	CON_DATA_REQ2_F61:	//F61	总加组月累计有功电能量（总、费率1~M）	
////		ucRet=Main_Get_ReqData_2_F61((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
//		break;
//	case	CON_DATA_REQ2_F62:	//F62	总加组月累计无功电能量（总、费率1~M）	
////		ucRet=Main_Get_ReqData_2_F62((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
//		break;
//	case	CON_DATA_REQ2_F65:	//F65	总加组超功率定值的月累计时间、月累计电能量	
////		ucRet=Main_Get_ReqData_2_F65((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
//		break;
//	case	CON_DATA_REQ2_F66:	//F66	总加组超月电能量定值的月累计时间、累计电能量	
////		ucRet=Main_Get_ReqData_2_F66((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime);
//		break;
//	case	CON_DATA_REQ2_F73:	//F73	总加组有功功率曲线	
////		ucRet=Main_Get_ReqData_2_F73((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
//		break;
//	case	CON_DATA_REQ2_F74:	//F74	总加组无功功率曲线	
////		ucRet=Main_Get_ReqData_2_F74((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
//		break;
//	case	CON_DATA_REQ2_F75:	//F75	总加组有功电能量曲线	
////		ucRet=Main_Get_ReqData_2_F75((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
//		break;
//	case	CON_DATA_REQ2_F76:	//F76	总加组无功电能量曲线	
////		ucRet=Main_Get_ReqData_2_F76((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
//		break;
	case	CON_DATA_REQ2_F81:	//F81	有功功率曲线	
		ucRet=Main_Get_ReqData_2_F81((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F82:	//F82	A相有功功率曲线	
		ucRet=Main_Get_ReqData_2_F82((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F83:	//F83	B相有功功率曲线	
		ucRet=Main_Get_ReqData_2_F83((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F84:	//F84	C相有功功率曲线	
		ucRet=Main_Get_ReqData_2_F84((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F85:	//F85	无功功率曲线	
		ucRet=Main_Get_ReqData_2_F85((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F86:	//F86	A相无功功率曲线	
		ucRet=Main_Get_ReqData_2_F86((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F87:	//F87	B相无功功率曲线	
		ucRet=Main_Get_ReqData_2_F87((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F88:	//F88	C相无功功率曲线	
		ucRet=Main_Get_ReqData_2_F88((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F89:	//F89	A相电压曲线	
		ucRet=Main_Get_ReqData_2_F89((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F90:	//F90	B相电压曲线	
		ucRet=Main_Get_ReqData_2_F90((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F91:	//F91	C相电压曲线	
		ucRet=Main_Get_ReqData_2_F91((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F92:	//F92	A相电流曲线	
		ucRet=Main_Get_ReqData_2_F92((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F93:	//F93	B相电流曲线	
		ucRet=Main_Get_ReqData_2_F93((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F94:	//F94	C相电流曲线	
		ucRet=Main_Get_ReqData_2_F94((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F95:	//F95	零序电流曲线	
		ucRet=Main_Get_ReqData_2_F95((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F97:	//F97	正向有功总电能量	
		ucRet=Main_Get_ReqData_2_F97((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F98:	//F98	正向无功总电能量	
		ucRet=Main_Get_ReqData_2_F98((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F99:	//F99	反向有功总电能量	
		ucRet=Main_Get_ReqData_2_F99((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F100:	//F100	反向无功总电能量	
		ucRet=Main_Get_ReqData_2_F100((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F101:	//F101	正向有功总电能示值	
		ucRet=Main_Get_ReqData_2_F101((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F102:	//F102	正向无功总电能示值	
		ucRet=Main_Get_ReqData_2_F102((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F103:	//F103	反向有功总电能示值	
		ucRet=Main_Get_ReqData_2_F103((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F104:	//F104	反向无功总电能示值	
		ucRet=Main_Get_ReqData_2_F104((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F105:	//F105	总功率因数	
		ucRet=Main_Get_ReqData_2_F105((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F106:	//F106	A相功率因数	
		ucRet=Main_Get_ReqData_2_F106((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F107:	//F107	B相功率因数	
		ucRet=Main_Get_ReqData_2_F107((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F108:	//F108	C相功率因数	
		ucRet=Main_Get_ReqData_2_F108((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F109:	
		ucRet=Main_Get_ReqData_2_F109((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	case	CON_DATA_REQ2_F110:	
		ucRet=Main_Get_ReqData_2_F110((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
//	case	CON_DATA_REQ2_F113:	//F113	A相2~19次谐波电流日最大值及发生时间	
//		ucRet=RET_ERROR;
//		break;
//	case	CON_DATA_REQ2_F114:	//F114	B相2~19次谐波电流日最大值及发生时间	
//		ucRet=RET_ERROR;
//		break;
//	case	CON_DATA_REQ2_F115:	//F115	C相2~19次谐波电流日最大值及发生时间	
//		ucRet=RET_ERROR;
//		break;
//	case	CON_DATA_REQ2_F116:	//F116	A相2~19次谐波电压含有率及总畸变率日最大值及发生时间	
//		ucRet=RET_ERROR;
//		break;
//	case	CON_DATA_REQ2_F117:	//F117	B相2~19次谐波电压含有率及总畸变率日最大值及发生时间	
//		ucRet=RET_ERROR;
//		break;
//	case	CON_DATA_REQ2_F118:	//F118	C相2~19次谐波电压含有率及总畸变率日最大值及发生时间	
//		ucRet=RET_ERROR;
//		break;
//	case	CON_DATA_REQ2_F121:	//F121	A相谐波越限日统计数据	
//		ucRet=RET_ERROR;
//		break;
//	case	CON_DATA_REQ2_F122:	//F122	B相谐波越限日统计数据	
//		ucRet=RET_ERROR;
//		break;
//	case	CON_DATA_REQ2_F123:	//F123	C相谐波越限日统计数据	
//		ucRet=RET_ERROR;
//		break;
//	case	CON_DATA_REQ2_F129:	//F129	直流模拟量越限日累计时间、最大/最小值及发生时间	
//		ucRet=RET_ERROR;
//		break;
//	case	CON_DATA_REQ2_F130:	//F130	直流模拟量越限月累计时间、最大/最小值及发生时间	
//		ucRet=RET_ERROR;
//		break;
//	case	CON_DATA_REQ2_F138:	//F138	直流模拟量数据曲线	
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
	case	CON_DATA_REQ2_F219:	//F219	测量点组合数据曲线数据单元格式
		ucRet=Main_Get_ReqData_2_F219((unsigned char)usDAp,pucRetBuffer,&m,pstStartTime,pstEndTime,ucDataInter);
		break;
	default:
	    ucRet=RET_ERROR;
		break;
	}
	(*pusPos)=m;
	return ucRet;
}



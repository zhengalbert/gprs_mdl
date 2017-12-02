/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Alarm_Manage.c
 Description    : 告警处理
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

static unsigned char ucTimer15Flag,ucForeJudgeFlag[CONALRFORELISTNUM];
//static unsigned short usTimerStartFlag;
static unsigned short usForeStartFlag[CONALRFORELISTNUM];
static unsigned short usEnsureStarFlag[CONALRENSURELISTNUM];
//static unsigned char g_ucAlarmExeNo;
static unsigned char g_ucForeAlrRdMeterNo,g_ucEnsuAlrRdMeterNo;
//static STALRCODEINFO stNewestAlarmCode;
//static unsigned char ucSwitchChange;

static unsigned char Alarm_CopyMeter_Timer_Orderly(void);
static unsigned char Alarm_CopyMeter_Timer_ForeJudge(void);
static unsigned char Alarm_CopyMeter_Timer_TakeData(void);
static unsigned char Alarm_ForeJudge(void);
static unsigned char Alarm_EnsureJudge(unsigned char ucAlrForeNo);

//static unsigned char Alarm_GetCmdFlag_From_DataItem(STCMDFLAG	*pstIsSendFlag,STDATAITEM_ALR *pstDataItem_alr,unsigned char ucDataItemNum);
//static unsigned char Alarm_Check_CopyMeter_AllFinished(void);
//static unsigned char Clear_CmdFlag_Yes(STCMDFLAG	*pstIsSendFlag);
//static unsigned char Clear_CmdFlag_No(STCMDFLAG	*pstIsSendFlag);
//static unsigned char Clear_CmdFlag_Big(STCMDFLAG	*pstIsCmdFlag);
//static unsigned char ClearRS485TaskCmdList(STRS485TASKCMDLIST *pstRS485TaskCmdList);
//初始化函数
unsigned char Alarm_Init_All(void)
{
	unsigned char ucRet=RET_SUCCESS;
	ClearDateTime(&g_stTerPowerChangeTime);
	Alarm_Init(CON_PARA_INIT_ALL_CLR);
	Alarm_InitAlrHappenWord();
	return ucRet;
}

unsigned char Alarm_Init(unsigned char ucFlag)
{
	unsigned char ucRet=RET_SUCCESS;
    unsigned char i=0;
	
//	for(i=0;i<CON_METER_NUM;i++)
//	{
//		Clear_CmdFlag_Big(&(g_stCmdFlag[i]));
//		g_stCmdFlag[i].ucCopyDayTimeFlag=CONCOMMAND_SML;				//科陆地台体不支持大项命令
//		g_stCmdFlag[i].ucPulseConstFlag=CONCOMMAND_SML;					//科陆地台体不支持大项命令
//		g_stCmdFlag[i].ucLastVolOffBegTimeFlag=CONCOMMAND_SML;			//断时开始时间小项
//		g_stCmdFlag[i].ucLastVolOffEndTimeFlag=CONCOMMAND_SML;			//断时结束时间小项
//		//add by cfh 10-01-19
//		g_stCmdFlag[i].ucLostVolNumFlag=CONCOMMAND_SML;					//断时次数小项
//		g_stCmdFlag[i].ucLostVolTimeFlag=CONCOMMAND_SML;				//断时累计时间小项
//		g_stCmdFlag[i].ucDateTimeFlag=CONCOMMAND_SML;					//日期时间小项
//	//	g_stCmdFlag[i].ucDateTimeFlag=CONCOMMAND_SML;					//日期时间小项
//	}
	
	for(i=0;i<CONALRFORELISTNUM;i++)
		Clear_AlrForeJudge(&(stAlrForeJudgeList[i]));
	//HdConvertWatchDog();						//喂狗
	for(i=0;i<CONALRENSURELISTNUM;i++)
		Clear_AlrEnsureJudge(&(stAlrEnsureJudgeList[i]));
//	
	ucTimer15Flag=(unsigned char)CONINVALIDVAL;
	for(i=0;i<CONALRFORELISTNUM;i++)
		ucForeJudgeFlag[i]=(unsigned char)CONINVALIDVAL;
//	
//	usTimerStartFlag=CON_STATUS_NO_2BYTE;
	usTimerFinishFlag=CON_STATUS_NO_2BYTE;
	for(i=0;i<CONALRFORELISTNUM;i++)
		usForeStartFlag[i]=CON_STATUS_INIT;//CON_STATUS_NO_2BYTE;
	for(i=0;i<CONALRENSURELISTNUM;i++)
		usEnsureStarFlag[i]=CON_STATUS_INIT;//CON_STATUS_NO_2BYTE;

	memset(&ucMeterStateChangeFlag,0,sizeof(ucMeterStateChangeFlag));
	Alarm_InitAlrStatus();
//	Alarm_GetAlrRecordList();
    Alarm_InitManageInfo(ucFlag);//azh替换上面这个函数

//	stNewestAlarmCode.ucMPNo = 0;
//	stNewestAlarmCode.usAlrCode = 0;
//	g_ucAlarmExeNo = 0;
    g_ucForeAlrRdMeterNo = 0;
    g_ucEnsuAlrRdMeterNo = 0;
	memset(ucMeterCopyErrTimes,0,CON_MAX_MPNUM);
	memset(ucMeterCopyErrAlrFlags,CON_STATUS_NO,CON_MAX_MPNUM);
	memset(usTimerCopyFlag,CON_STATUS_NO_2BYTE,CON_METER_NUM);				//抄表标志清除
	//HdConvertWatchDog();						//喂狗
//	Alarm_Init_Ter();
//	Alarm_Init_CTCheck();

//	Alarm_Init_SumG();
//	Init_Switch_Change_Info();
    g_ucTerDP_State = CON_TERDP_STATE_NORMAL;
	return ucRet;
}
/******************************************************************************
** 函数名称: unsigned char Alarm_CopyMeter_Timer(void)
** 函数功能: 定时 告警 曲线任务抄表控制(控制发送到串口抄表任务的队列)
**           stGRTaskExeInfo.ucTaskValidFlag: 任务执行标志 有任务在执行其他抄表任务都发不出去
**           gucOneLoopFlag 这个函数每个循环只能执行一个抄表任务 而且从 最前 到最后优先级判断
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
//定时抄表，1则15分钟一点，2则根据预判值进行定时抄表
unsigned char Alarm_CopyMeter_Timer(void)
{
	unsigned char ucRet=RET_SUCCESS;
	
	gucOneLoopFlag = 0;
	if(usAlrActionLockTimer>=CON_ALRACTION_METERCOPY)
	{
		Alarm_CopyMeter_Timer_Orderly();//定时抄表 优先级最高
		Alarm_CopyMeter_Timer_ForeJudge();//预判的告警抄读用于确认告警的抄表 //优先级中
		Alarm_CopyMeter_Timer_TakeData();//告警确认后所带数据抄表 //优先级低
	}
	if(usAlrActionLockTimer<=CON_ALRACTION_ALRUP)
		usAlrActionLockTimer++;

    CTask_Proc_Down_Timer();//曲线任务
	
	return ucRet;
}

unsigned char Alarm_CopyMeter_Timer_Orderly(void)//azh 定时数据读取
{//常规的定时任务读取
	unsigned char ucRet=RET_SUCCESS,i=0;
	unsigned char ucMPNo=0;
	unsigned char ucMPType=CON_MPTYPE_NULL;
	STDATETIME	stDateTime;
//	STCMDFLAG	stIsSendFlag;
	unsigned char ucCopyInter;
	
	ucCopyInter=RunPara_GetMeterCopyInter();		//抄表间隔
	stDateTime=GetCurrentTime();
	if(((stDateTime.ucMinute/ucCopyInter)!=ucTimer15Flag)||((stDateTime.ucHour == 23)&&(stDateTime.ucMinute >= 57)))	//日冻结数据抄表
	{
		if(stDateTime.ucSecond>12 && stDateTime.ucSecond<55)
		{
			ucTimer15Flag = stDateTime.ucMinute/ucCopyInter;
			for(i=0;i<CON_METER_NUM;i++)
			{
				ucMPNo	 = RunPara_GetMeter_MPNo(i);
				ucMPType = RunPara_GetMPType(ucMPNo);
				if(ucMPType == CON_MPTYPE_METER)
					usTimerCopyFlag[i]=CON_STATUS_YES_2BYTE;
				else
					usTimerCopyFlag[i]=CON_STATUS_NO_2BYTE;
			}
//			usTimerStartFlag=CON_STATUS_YES_2BYTE;
		}
	}
	
//azh 先查看是否启动定时数据抄读
    if((stGRTaskExeInfo.ucTaskValidFlag == CON_TASK_RETDATA_INVALID) && (gucOneLoopFlag == 0))
    {    
    	for(i=0;i<CON_METER_NUM;i++)
    	{
    		if(usTimerCopyFlag[i]==CON_STATUS_YES_2BYTE)
    		{
    //			ucPortNo=RunPara_GetMeterPara_PortNo(i);
    //			if(ucPortNo==0)
    //			{
    				ucMPNo	 = RunPara_GetMeter_MPNo(i);
    				ucMPType = RunPara_GetMPType(ucMPNo);
    				if(ucMPType == CON_MPTYPE_METER)
    				{    
    					Meter_FillTimerCommand(CONRS485SOURTIMER, ucMPNo);
    				}
    				usTimerCopyFlag[i]=CON_STATUS_NO_2BYTE;				//第i块表抄表命令已经填写完成
        			stGRTaskExeInfo.ucTaskValidFlag=CON_TASK_RETDATA_VALID;			//任务执行标志
        			gucOneLoopFlag = 1;
        			break;//一个测量执行一次
    //			}
    		}
    	}
    	if(i >= CON_METER_NUM)
    	{
    	    if(stGRTaskExeInfo.ucTaskValidFlag==CON_TASK_RETDATA_INVALID)			//任务执行标志
    	    {
    	        usTimerFinishFlag = CON_STATUS_YES_2BYTE;//定时刷新任务抄读完成
    	    }
    	}
    }	
	
	

	return ucRet;
}

//unsigned char Alarm_Check_CopyMeter_AllFinished(void)
//{//检查是否所有表抄表命令已经填写完成
//	unsigned char ucRet=CON_STATUS_YES;
//	unsigned char i;
//	unsigned char ucPortNo;
//	for(i=0;i<CON_METER_NUM;i++)
//	{
//		ucPortNo=RunPara_GetMeterPara_PortNo(i);
//		if(ucPortNo==0)
//		{
//			if(usTimerCopyFlag[i]==CON_STATUS_YES_2BYTE)
//			{
//				ucRet=CON_STATUS_NO;									//还有表抄表命令未填充完毕
//				break;
//			}
//		}
//	}
//	return ucRet;
//}

unsigned char Alarm_CopyMeter_Timer_ForeJudge(void)
{//告警预判后的任务读取,对于总加组的告警，需要召唤所有的测量点的同一个量
	unsigned char	ucRet=RET_SUCCESS;
	STDATETIME		stDateTime;
//	STCMDFLAG		stIsSendFlag;
	unsigned char	ucAlrCycle=0;//,i=0;
	unsigned char	ucMPType=CON_MPTYPE_NULL;
	unsigned char	ucMeterNo;
	unsigned char	ucMPNo=0;

    if((stGRTaskExeInfo.ucTaskValidFlag != CON_TASK_RETDATA_INVALID) || (gucOneLoopFlag == 1))
    {
        return ucRet;
    }
	
	stDateTime=GetCurrentTime();
//	for(i=0;i<CONALRFORELISTNUM;i++)//azh 没有发送缓冲 所以一个抄读完了 再抄下一个
//	{//告警预判定时召唤数据
		if(g_ucForeAlrRdMeterNo >= CONALRFORELISTNUM)
		{
		     g_ucForeAlrRdMeterNo = 0;   
		}
		
		if(stAlrForeJudgeList[g_ucForeAlrRdMeterNo].usAlrCode != CONALRCODE_INVALID)
		{
			if(usForeStartFlag[g_ucForeAlrRdMeterNo]==CON_STATUS_FINISH)//抄表完成 
			{
    			g_ucForeAlrRdMeterNo++;//等到下一轮再来执行
    			if(g_ucForeAlrRdMeterNo >= CONALRFORELISTNUM)
        		{
        		     g_ucForeAlrRdMeterNo = 0;   
        		}
			}
			else if(usForeStartFlag[g_ucForeAlrRdMeterNo]==CON_STATUS_INIT)//还没抄表
			{
    			ucAlrCycle = stAlrForeJudgeList[g_ucForeAlrRdMeterNo].ucAlrJudgeCycle;
    			ucAlrCycle = (unsigned char)Adjust_Zero(stAlrForeJudgeList[g_ucForeAlrRdMeterNo].ucAlrJudgeCycle);
    			
    			if((stDateTime.ucMinute / ucAlrCycle) != ucForeJudgeFlag[g_ucForeAlrRdMeterNo])
    			{//上次没有发送完，则等待；注意不能无限等待
    				if(stDateTime.ucSecond>5 && stDateTime.ucSecond<55)
    				{
    					ucForeJudgeFlag[g_ucForeAlrRdMeterNo] = (stDateTime.ucMinute / ucAlrCycle);
    					
//    					ucMPNo = Alarm_GetCmdFlag_From_DataItem(&stIsSendFlag , stAlrForeJudgeList[g_ucForeAlrRdMeterNo].stAlrDataItem[0].stDataItem_alr,CONALRITEMNUM);
    					ucMPNo = stAlrForeJudgeList[g_ucForeAlrRdMeterNo].ucMPNo;
    					ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
    					ucMPType = RunPara_GetMPType(ucMPNo);
    					if( ucMPType == CON_MPTYPE_METER && ucMPNo < CON_MAX_MPNUM && ucMeterNo < CON_METER_NUM)
    					{
//     						Meter_FillTimerCommand((unsigned char)(CONRS485SOUR_ALRAFFIRMTASK+g_ucForeAlrRdMeterNo),	
//     							g_stCmdFlag[ucMeterNo],stIsSendFlag,ucMPNo,1);
//azh
     						Meter_FillTimerCommand((unsigned char)(CONRS485SOUR_ALRAFFIRMTASK+g_ucForeAlrRdMeterNo),ucMPNo);
						    usForeStartFlag[g_ucForeAlrRdMeterNo]=CON_STATUS_START;//azh 启动抄表 空闲时不一定所以数据项抄完!
						    stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_VALID;
						    gucOneLoopFlag = 1;
    					}else
    					{
    						ucForeJudgeFlag[g_ucForeAlrRdMeterNo]=(unsigned char)CONINVALIDVAL;
    						Clear_AlrForeJudge(&(stAlrForeJudgeList[g_ucForeAlrRdMeterNo]));
    					}
    				}
    			}			    
			}
			//else //正在抄表过程
		}else
		{
			ucForeJudgeFlag[g_ucForeAlrRdMeterNo]=(unsigned char)CONINVALIDVAL;
			Clear_AlrForeJudge(&(stAlrForeJudgeList[g_ucForeAlrRdMeterNo]));
    		usForeStartFlag[g_ucForeAlrRdMeterNo]=CON_STATUS_INIT;
			g_ucForeAlrRdMeterNo++;
			if(g_ucForeAlrRdMeterNo >= CONALRFORELISTNUM)
    		{
    		     g_ucForeAlrRdMeterNo = 0;   
    		}
		}
//	}
	return ucRet;
}

unsigned char Alarm_CopyMeter_Timer_TakeData(void)
{//告警携带数据读取
	unsigned char	ucRet=RET_SUCCESS;
//	STCMDFLAG		stIsSendFlag;
	unsigned char	ucMeterNo;//,i=0;
	unsigned char	ucMPNo=0;
	unsigned char	ucMPType=CON_MPTYPE_NULL;

    if((stGRTaskExeInfo.ucTaskValidFlag != CON_TASK_RETDATA_INVALID) || (gucOneLoopFlag == 1))
    {
        return ucRet;
    }	
//	for(i=0;i<CONALRENSURELISTNUM;i++)
//	{//发送告警补召数据的命令
		if(g_ucEnsuAlrRdMeterNo >= CONALRENSURELISTNUM)
		{
		     g_ucEnsuAlrRdMeterNo = 0;   
		}
		
		if(stAlrEnsureJudgeList[g_ucEnsuAlrRdMeterNo].usAlrCode != CONALRCODE_INVALID )
		{
			if(usEnsureStarFlag[g_ucEnsuAlrRdMeterNo]==CON_STATUS_FINISH)//抄表完成 
			{
    			g_ucEnsuAlrRdMeterNo++;//等到下一轮再来执行
    			if(g_ucEnsuAlrRdMeterNo >= CONALRENSURELISTNUM)
        		{
        		     g_ucEnsuAlrRdMeterNo = 0;   
        		}
			}
			else if(usEnsureStarFlag[g_ucEnsuAlrRdMeterNo]==CON_STATUS_INIT)//还没抄表
			{
//     			ucMPNo = Alarm_GetCmdFlag_From_DataItem(&stIsSendFlag,stAlrEnsureJudgeList[g_ucEnsuAlrRdMeterNo].stAlrDataItem,CONALRTAKEITEMNUM);
    			ucMPNo = stAlrEnsureJudgeList[g_ucEnsuAlrRdMeterNo].ucMPNo;
    			ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
    			ucMPType	= RunPara_GetMPType(ucMPNo);
    			if(ucMPType == CON_MPTYPE_METER && ucMeterNo < CON_METER_NUM)
    			{
//     				Meter_FillTimerCommand((unsigned char)(CONRS485SOURALR_1+g_ucEnsuAlrRdMeterNo),g_stCmdFlag[ucMeterNo],stIsSendFlag,ucMPNo,1);
                    Meter_FillTimerCommand((unsigned char)(CONRS485SOURALR_1+g_ucEnsuAlrRdMeterNo),ucMPNo);
    				usEnsureStarFlag[g_ucEnsuAlrRdMeterNo]=CON_STATUS_START;//azh 改成数据项抄读完成后设置标志
				    stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_VALID;
				    gucOneLoopFlag = 1;    				
    			}else
    				Clear_AlrEnsureJudge(&(stAlrEnsureJudgeList[g_ucEnsuAlrRdMeterNo]));
    		}
		}
		else
		{
			usEnsureStarFlag[g_ucEnsuAlrRdMeterNo] = CON_STATUS_INIT;
			g_ucEnsuAlrRdMeterNo++;
			if(g_ucEnsuAlrRdMeterNo >= CONALRENSURELISTNUM)
    		{
    		     g_ucEnsuAlrRdMeterNo = 0;   
    		}		    
		}
//	}
	return ucRet;
}

//判断定时抄表是否结束，并启动告警预判
unsigned char Alarm_Manage(void)
{
	unsigned char	ucRet=RET_SUCCESS,i=0;
	unsigned short	usAlrCode=CONALRCODE_INVALID;
//	unsigned char	ucAlrExeNo = 0;
//	STMPRATEVAL		stMPRateVal;
//	STADEMETERVAR	stAdeMeterValTmp;
//	STADEMETERENERGYVAR	stAdeMeterEnergyValTmp;
//	STADEERRSTAT	stAdeErrStatTmp;
//	STADEDEMAND		*pstAdeDemand;
//	unsigned char ucMPNo;
	
//	if((g_ucAlarmExeNo % CON_ALR_TIMEINTER)==0)			//5秒处理一次
//	{
//		ucAlrExeNo = g_ucAlarmExeNo/CON_ALR_TIMEINTER;
//		if(ucAlrExeNo < 8)
//		{
//			if(ucAlrExeNo == 0 || ucAlrExeNo == 2 || ucAlrExeNo == 4 || ucAlrExeNo == 6)		
//			{//10秒处理一次
//				ucMPNo=RunPara_GetTer_MPNo();
//				stMPRateVal = RunPara_GetMPRateVal(ucMPNo);
//				ADE_Get_MeterVal(&stAdeMeterValTmp,&stAdeMeterEnergyValTmp,&stAdeErrStatTmp,stMPRateVal.ucMPLineType);
//				pstAdeDemand=ADEGetMaxDemandAndTime();
//				Fill_AlarmTer_Manage(&stAdeMeterValTmp,&stAdeMeterEnergyValTmp,&stAdeErrStatTmp,pstAdeDemand);
//			}
//			Alarm_Manage_Ter(ucAlrExeNo);
//			Alarm_Manage_SumGroup(ucAlrExeNo);
//		}
//	}
//	g_ucAlarmExeNo++;
//	if(g_ucAlarmExeNo/CON_ALR_TIMEINTER >=8)
//		g_ucAlarmExeNo = 0;
	
 	Alarm_Manage_Ter_QuickPart();						//快速检测状态变位异常

//	Alarm_Manage_CTCheck();			

//	if( RS485_Check_TaskSource(CONRS485SOURTIMER,0)==RET_RS485_TASKSOURCE_NULL 
//		&& RS485_Check_TaskSource(CONRS485SOURTIMER,1)==RET_RS485_TASKSOURCE_NULL 
//		&& usTimerStartFlag==CON_STATUS_YES_2BYTE
//		&& Alarm_Check_CopyMeter_AllFinished()==CON_STATUS_YES )
    if(usTimerFinishFlag==CON_STATUS_YES_2BYTE)//azh 定时抄表一轮结束
	{//当队列中再无定时命令时，如已经开始填写，则可进行告警预判
		Alarm_ForeJudge();//通过定时抄表读到的数据 预判告警 并发动确认告警的抄表行为
	//	Alarm_Meter_State();		//状态位变位告警还存在些问题，先不做 cfh 10-04-03
//		usTimerStartFlag=CON_STATUS_NO_2BYTE;
		usTimerFinishFlag = CON_STATUS_NO_2BYTE;
	}
	
	for(i=0;i<CONALRFORELISTNUM;i++)
	{
//		if( RS485_Check_TaskSource((unsigned char)(CONRS485SOUR_ALRAFFIRMTASK+i),0)==RET_RS485_TASKSOURCE_NULL 
//			&& RS485_Check_TaskSource((unsigned char)(CONRS485SOUR_ALRAFFIRMTASK+i),1)==RET_RS485_TASKSOURCE_NULL 
//			&& usForeStartFlag[i]==CON_STATUS_YES_2BYTE)
        //azh 确认告警抄读的数据完成
        if(usForeStartFlag[i]==CON_STATUS_FINISH)
		{//当队列中再无定时命令时，如已经开始填写，则可进行告警预判
			Alarm_EnsureJudge(i);//确认告警 并发起携带数据的抄表行为
			usForeStartFlag[i]=CON_STATUS_INIT;//CON_STATUS_NO_2BYTE;
		}
	}
	
	for(i=0;i<CONALRENSURELISTNUM;i++)
	{
//		if(( RS485_Check_TaskSource((unsigned char)(CONRS485SOURALR_1+i),0)==RET_RS485_TASKSOURCE_NULL )
//			&& (RS485_Check_TaskSource((unsigned char)(CONRS485SOURALR_1+i),1)==RET_RS485_TASKSOURCE_NULL) 
//			&& (usEnsureStarFlag[i]==CON_STATUS_YES_2BYTE))
        if(usEnsureStarFlag[i]==CON_STATUS_FINISH)//告警上报所携带数据抄读完成
		{
			usAlrCode =stAlrEnsureJudgeList[i].usAlrCode;
			if(usAlrCode != CONALRCODE_INVALID)
			{//将确诊队列中的内容根据要求确定组帧上送，填写入告警事件队列
				if( Get_Shield_ACD_byAlrCode(usAlrCode,CON_ALR_SHIELDFLAG) == CON_ALR_SHIELD_NOT )	//告警不屏蔽，记录
				{//记录的上报，不记录的不上报
					AddToAlrRecordList(&(stAlrEnsureJudgeList[i]),CONALR_UPFLAG_NEED);		//存入事件队列

//					stNewestAlarmCode.ucMPNo	= stAlrEnsureJudgeList[i].ucMPNo;
//					stNewestAlarmCode.usAlrCode = Alarm_GetAlrERCNo(stAlrEnsureJudgeList[i].usAlrCode);

//					if(DP_Get_TerDP_State()!=CON_TERDP_STATE_DOWNLOAD)		//程序下装需要占用告警存储空间
//					{
						if(Get_Shield_ACD_byAlrCode(usAlrCode,CON_ALR_ACDFLAG) == CON_ALR_ACD)		//重要事件
						{
							Alarm_SetFrameACD(CON_STATUS_YES_2BYTE);
							//g_ucEventCount_Important++;
							//if(g_ucEventCount_Important>CONALRRECORDNUM_IMPORTANT)
							//	g_ucEventCount_Important=0;
							//azh 在上面加增加记录的时候已处理所以直接赋值
                            g_ucEventCount_Important = stAlrRecordInfo.stAlrRecordInfo_Imp.ucList_Head_P;
						}else		//一般告警
						{
							//g_ucEventCount_Normal++;
							//if(g_ucEventCount_Normal>CONALRRECORDNUM_NORMAL)
							//	g_ucEventCount_Normal=0;
							g_ucEventCount_Normal = stAlrRecordInfo.stAlrRecordInfo_Norm.ucList_Head_P;
						}
//					}
				}
				Clear_AlrEnsureJudge(&(stAlrEnsureJudgeList[i]));
				usEnsureStarFlag[i]=CON_STATUS_INIT;//CON_STATUS_NO_2BYTE;
			}
			else
			{
			    usEnsureStarFlag[i]=CON_STATUS_INIT;//CON_STATUS_NO_2BYTE;
			}
		}
	}
	
	Alarm_UpSend_Manage(CON_ALR_ACD);			//重要告警上送管理
	
	return ucRet;
}
//unsigned char Clear_CmdFlag_Yes(STCMDFLAG	*pstIsSendFlag)
//{//清除所有命令均发送
//	unsigned char ucRet=RET_SUCCESS;
//	memset((unsigned char*)pstIsSendFlag,CONCOMMAND_SEND,sizeof(STCMDFLAG));	
//	return ucRet;
//}

//unsigned char Clear_CmdFlag_No(STCMDFLAG	*pstIsSendFlag)
//{//清除所有命令均不发送
//	unsigned char ucRet=RET_SUCCESS;
//	memset((unsigned char*)pstIsSendFlag,CONCOMMAND_NOSEND,sizeof(STCMDFLAG));
//	return ucRet;
//}

//unsigned char Clear_CmdFlag_Big(STCMDFLAG	*pstIsCmdFlag)
//{//清除命令标志均为大项命令
//	unsigned char ucRet=RET_SUCCESS;
//	memset((unsigned char*)pstIsCmdFlag,CONCOMMAND_BIG,sizeof(STCMDFLAG));
//	return ucRet;
//}

//unsigned char ClearRS485TaskCmdList(STRS485TASKCMDLIST *pstRS485TaskCmdList)
//{
//	unsigned char ucRet=RET_SUCCESS,i=0;
//	for(i=0;i<CON_CMDLIST_NUM;i++)
//	{
//		pstRS485TaskCmdList->stRS485TaskCmd[i].ucReportType	 =CON_RS485_REPORT_TYPE_IDLE;
//		pstRS485TaskCmdList->stRS485TaskCmd[i].usDataType	 =CON_DI_TYPE_INVALID;
//	}
//	return ucRet;
//}

unsigned char Alarm_InitAlrStatus(void)
{//初始化告警状态
	unsigned char ucRet=RET_SUCCESS;
	unsigned char i;
	for(i=0;i<CON_MAX_MPNUM;i++)
		memset(&(stAlarmStatus[i]),CONALR_STAT_HAPPEN_NOT,sizeof(STALARMSTAT));
//	for(i=0;i<CON_SUMGROUP_NUM;i++)
//		memset(&(stSumAlarmStatus[i]),CONALR_STAT_HAPPEN_NOT,sizeof(STSUMALARMSTAT));
	return ucRet;
}

unsigned char Alarm_InitAlrHappenWord(void)
{//初始化告警发生状态
	unsigned char ucRet=RET_SUCCESS;
	memset(&stAlrHappenWord,CON_ALR_HAPPEN_NOT,sizeof(STALRCATEGORY));
	return ucRet;
}

/*********************************************************************************************************/
//预警、确诊队列操作函数
unsigned char Alarm_ForeJudge(void)
{
	unsigned char	ucRet=RET_SUCCESS;
	Alarm_ForeJudge_Meter();
	return ucRet;
}

unsigned char Clear_AlrForeJudge(STALRFOREJUDGE	*pstAlrForeJudge)
{//以告警编码作为此结构是否有效的标记，因此，主要是将告警编码置为0xBBBB即可
	unsigned char ucRet=RET_SUCCESS,i=0,j=0;
	pstAlrForeJudge->usAlrCode			=CONALRCODE_INVALID;
	pstAlrForeJudge->ucMPNo				=CON_MPNO_INVALID;
	pstAlrForeJudge->ucAlrJudgeTimes	=0;
	pstAlrForeJudge->ucAlrJudgeCount	=0;
	pstAlrForeJudge->ucAlrJudgeCycle	=1;
	pstAlrForeJudge->ucAlrDataItemNum	=0;
	for(i=0;i<CONALRFOREREPLYNUM;i++)
	{
		ClearDateTime(&(pstAlrForeJudge->stAlrDataItem[i].stDateTime));
		for(j=0;j<CONALRITEMNUM;j++)
			ClearDataItem_alr(&(pstAlrForeJudge->stAlrDataItem[i].stDataItem_alr[j]));
	}
	
	return ucRet;
}

//填写告警预判列表
unsigned char Alarm_FillForeJudgeList(unsigned short usAlrCode,unsigned char ucMPNo)
{
	unsigned char ucRet=RET_ERROR,i=0;
	
	for(i=0;i<CONALRFORELISTNUM;i++)
	{
		if(stAlrForeJudgeList[i].usAlrCode==CONALRCODE_INVALID)
		{
			ucRet = Alarm_GetForeJudgeInfo(usAlrCode,ucMPNo,&(stAlrForeJudgeList[i]));
			if(ucRet == RET_ERROR)
				Clear_AlrForeJudge(&(stAlrForeJudgeList[i]));
			else
				ucRet=RET_SUCCESS;

			usForeStartFlag[i]=CON_STATUS_INIT;//CON_STATUS_NO_2BYTE;
			break;
		}
	}
	return ucRet;
}

unsigned char Alarm_CheckForeJudgeList(unsigned short usAlrCode,unsigned char ucMPNo)
{
	unsigned char ucRet=RET_ERROR,i=0;
	if(usAlrCode!=CONALRCODE_INVALID && ucMPNo != CON_MPNO_INVALID)
	{
		for(i=0;i<CONALRFORELISTNUM;i++)
		{
			if((stAlrForeJudgeList[i].ucMPNo==ucMPNo) && (stAlrForeJudgeList[i].usAlrCode==usAlrCode))
			{
				ucRet=RET_SUCCESS;
				break;
			}
		}
	}
	return ucRet;
}

unsigned char Alarm_EnsureJudge(unsigned char ucAlrForeNo)
{//告警确认(总加组无此函数)
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucMPNo;
	unsigned char ucMPType= CON_MPTYPE_NULL;

	if(ucAlrForeNo < CONALRFORELISTNUM)
	{
		if(stAlrForeJudgeList[ucAlrForeNo].usAlrCode != CONALRCODE_INVALID)
		{
			ucMPNo= stAlrForeJudgeList[ucAlrForeNo].ucMPNo;
			ucMPType = RunPara_GetMPType(ucMPNo);

			if( ucMPType == CON_MPTYPE_METER)
			{
				ucRet = Alarm_EnsureJudge_Meter(ucAlrForeNo);
				if(ucRet == RET_ENSURE_FINISH || ucRet == RET_ERROR)
					Clear_AlrForeJudge(&(stAlrForeJudgeList[ucAlrForeNo]));
			}else
				Clear_AlrForeJudge(&(stAlrForeJudgeList[ucAlrForeNo]));
		}
	}

	return ucRet;
}

unsigned char Clear_AlrEnsureJudge(STALRENSUREJUDGE	*pstAlrEnsureJudge)
{//以告警编码作为此结构是否有效的标记，因此，主要是将告警编码置为0xBBBB即可
	unsigned char ucRet=RET_SUCCESS,i=0;
	pstAlrEnsureJudge->usAlrCode	=	CONALRCODE_INVALID;
	pstAlrEnsureJudge->ucMPNo		=	CON_MPNO_INVALID;
	ClearDateTime(&(pstAlrEnsureJudge->stDateTime));
	
	pstAlrEnsureJudge->ucAlrDataItemNum	=0;
	for(i=0;i<CONALRTAKEITEMNUM;i++)
		ClearDataItem_alr(&(pstAlrEnsureJudge->stAlrDataItem[i]));
	return ucRet;
}

//填写告警确诊列表
unsigned char Alarm_FillEnsureJudgeList(unsigned short usAlrCode,unsigned char ucMPNo)
{
	unsigned char ucRet=0xFF,i=0;
	
	for(i=0;i<CONALRENSURELISTNUM;i++)
	{
		if(stAlrEnsureJudgeList[i].usAlrCode==CONALRCODE_INVALID)
		{
			ucRet = Alarm_GetEnsureJudgeInfo(usAlrCode,ucMPNo,&(stAlrEnsureJudgeList[i]));
			if(ucRet == RET_DATA_AVAILABLE)		//数据已经有效，则无须进行召唤//azh有些告警上报无须携带数据项
			{
				usEnsureStarFlag[i]=CON_STATUS_FINISH;//CON_STATUS_YES_2BYTE;
				ucRet = i;
			}
			else if(ucRet == RET_SUCCESS)
			{
				usEnsureStarFlag[i]=CON_STATUS_INIT;//CON_STATUS_NO_2BYTE;
				ucRet = i;
			}else
			{
				Clear_AlrEnsureJudge(&(stAlrEnsureJudgeList[i]));
				usEnsureStarFlag[i]=CON_STATUS_INIT;//CON_STATUS_NO_2BYTE;
				ucRet = 0xFF;
			}
			break;
		}
	}
	return ucRet;
}

unsigned char Alarm_CheckEnsureJudgeList(unsigned short usAlrCode,unsigned char ucMPNo)
{
	unsigned char ucRet=RET_ERROR,i=0;
	if(usAlrCode!=CONALRCODE_INVALID && ucMPNo != CON_MPNO_INVALID)
	{
		for(i=0;i<CONALRENSURELISTNUM;i++)
		{
			if((stAlrEnsureJudgeList[i].ucMPNo==ucMPNo) && (stAlrEnsureJudgeList[i].usAlrCode==usAlrCode))
			{
				ucRet=RET_SUCCESS;
				break;
			}
		}
	}
	return ucRet;
}

/*********************************************************************************************************/
//根据数据项列表，获取下发的列表
//unsigned char Alarm_GetCmdFlag_From_DataItem(STCMDFLAG	*pstIsSendFlag,STDATAITEM_ALR *pstDataItem_alr,unsigned char ucDataItemNum)
//{
//	unsigned char i=0;
//	unsigned char ucMPNo=CON_MPNO_INVALID;
//	unsigned char ucMPType=CON_MPTYPE_NULL;
//	
//	Clear_CmdFlag_No(pstIsSendFlag);
//	
//	for(i=0;i<ucDataItemNum;i++)
//	{
//		ucMPType = RunPara_GetMPType(pstDataItem_alr->ucMPNo);
//		if( ucMPType == CON_MPTYPE_METER)
//		{
//			ucMPNo = pstDataItem_alr->ucMPNo;
//			switch(pstDataItem_alr->usItemLabel)
//			{
//			case	CON_DI_TYPE_MPDIRPOWTOTAL	:	//正向有功总电能
//			case	CON_DI_TYPE_MPDIRPOWPLUS	:	//正向有功尖电能
//			case	CON_DI_TYPE_MPDIRPOWAPEX	:	//正向有功峰电能
//			case	CON_DI_TYPE_MPDIRPOWFLAT	:	//正向有功平电能
//			case	CON_DI_TYPE_MPDIRPOWVALE	:	//正向有功谷电能
//			case	CON_DI_TYPE_MPDIRPOW_BLOCK	:	//正向有功电能数据块
//				pstIsSendFlag->ucDirPowerFlag=CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPDIRPOWTOTAL_LM:
//			case	CON_DI_TYPE_MPDIRPOWPLUS_LM	:
//			case	CON_DI_TYPE_MPDIRPOWAPEX_LM	:
//			case	CON_DI_TYPE_MPDIRPOWFLAT_LM	:
//			case	CON_DI_TYPE_MPDIRPOWVALE_LM	:
//			case	CON_DI_TYPE_MPDIRPOW_LM_BLOCK:
//				pstIsSendFlag->ucDirPowerLmFlag=CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPUNDIRPOWTOTAL	:
//			case	CON_DI_TYPE_MPUNDIRPOWPLUS	:
//			case	CON_DI_TYPE_MPUNDIRPOWAPEX	:
//			case	CON_DI_TYPE_MPUNDIRPOWFLAT	:
//			case	CON_DI_TYPE_MPUNDIRPOWVALE	:
//			case	CON_DI_TYPE_MPUNDIRPOW_BLOCK:
//				pstIsSendFlag->ucUnDirPowerFlag=CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPDIRVARTOTAL	:
//			case	CON_DI_TYPE_MPDIRVARPLUS	:
//			case	CON_DI_TYPE_MPDIRVARAPEX	:
//			case	CON_DI_TYPE_MPDIRVARFLAT	:
//			case	CON_DI_TYPE_MPDIRVARVALE	:
//			case	CON_DI_TYPE_MPDIRVAR_BLOCK	:
//				pstIsSendFlag->ucDirVarFlag=CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPUNDIRVARTOTAL	:
//			case	CON_DI_TYPE_MPUNDIRVARPLUS	:
//			case	CON_DI_TYPE_MPUNDIRVARAPEX	:
//			case	CON_DI_TYPE_MPUNDIRVARFLAT	:
//			case	CON_DI_TYPE_MPUNDIRVARVALE	:
//			case	CON_DI_TYPE_MPUNDIRVAR_BLOCK:
//				pstIsSendFlag->ucUnDirVarFlag=CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPVAR1		:
//			case	CON_DI_TYPE_MPVAR1_FEE1	:
//			case	CON_DI_TYPE_MPVAR1_FEE2	:
//			case	CON_DI_TYPE_MPVAR1_FEE3	:
//			case	CON_DI_TYPE_MPVAR1_FEE4	:
//			case	CON_DI_TYPE_MPVAR1_BLOCK:
//				pstIsSendFlag->ucVar1Flag=CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPVAR2		:
//			case	CON_DI_TYPE_MPVAR2_FEE1	:
//			case	CON_DI_TYPE_MPVAR2_FEE2	:
//			case	CON_DI_TYPE_MPVAR2_FEE3	:
//			case	CON_DI_TYPE_MPVAR2_FEE4	:
//			case	CON_DI_TYPE_MPVAR2_BLOCK:
//				pstIsSendFlag->ucVar2Flag=CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPVAR3		:
//			case	CON_DI_TYPE_MPVAR3_FEE1	:
//			case	CON_DI_TYPE_MPVAR3_FEE2	:
//			case	CON_DI_TYPE_MPVAR3_FEE3	:
//			case	CON_DI_TYPE_MPVAR3_FEE4	:
//			case	CON_DI_TYPE_MPVAR3_BLOCK:
//				pstIsSendFlag->ucVar3Flag=CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPVAR4		:
//			case	CON_DI_TYPE_MPVAR4_FEE1	:
//			case	CON_DI_TYPE_MPVAR4_FEE2	:
//			case	CON_DI_TYPE_MPVAR4_FEE3	:
//			case	CON_DI_TYPE_MPVAR4_FEE4	:
//			case	CON_DI_TYPE_MPVAR4_BLOCK:
//				pstIsSendFlag->ucVar4Flag=CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPVOLA		:
//			case	CON_DI_TYPE_MPVOLB		:
//			case	CON_DI_TYPE_MPVOLC		:
//			case	CON_DI_TYPE_MPVOL_BLOCK	:
//				pstIsSendFlag->ucVolFlag=CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPCURA				:	
//			case	CON_DI_TYPE_MPCURB				:
//			case	CON_DI_TYPE_MPCURC				:
//			case	CON_DI_TYPE_MPCUR_BLOCK			:
//				pstIsSendFlag->ucCurFlag=CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPINSTANTPOW		:
//			case	CON_DI_TYPE_MPINSTANTPOWA		:
//			case	CON_DI_TYPE_MPINSTANTPOWB		:
//			case	CON_DI_TYPE_MPINSTANTPOWC		:
//			case	CON_DI_TYPE_MPINSTANTPOW_BLOCK	:
//				pstIsSendFlag->ucInstantPowFlag=CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPINSTANTVAR		:
//			case	CON_DI_TYPE_MPINSTANTVARA		:
//			case	CON_DI_TYPE_MPINSTANTVARB		:
//			case	CON_DI_TYPE_MPINSTANTVARC		:
//			case	CON_DI_TYPE_MPINSTANTVAR_BLOCK	:
//				pstIsSendFlag->ucInstantVarFlag=CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPDIRPOWMAXDEMAND		:
//			case	CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE1	:
//			case	CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE2	:
//			case	CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE3	:
//			case	CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE4	:
//			case	CON_DI_TYPE_MPDIRPOWMAXDEMAND_BLOCK	:
//				pstIsSendFlag->ucDirPowerMaxDemandFlag=CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME		:
//			case	CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE1	:
//			case	CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE2	:
//			case	CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE3	:
//			case	CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE4	:
//			case	CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_BLOCK	:
//				pstIsSendFlag->ucDirPowerMaxDemandTimeFlag=CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPUNDIRPOWMAXDEMAND			:
//			case	CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE1	:
//			case	CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE2	:
//			case	CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE3	:
//			case	CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE4	:
//			case	CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_BLOCK	:
//				pstIsSendFlag->ucUnDirPowerMaxDemandFlag=CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME		:
//			case	CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE1:
//			case	CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE2:
//			case	CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE3:
//			case	CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE4:
//			case	CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_BLOCK:
//				pstIsSendFlag->ucUnDirPowerMaxDemandTimeFlag=CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPLATELYPROGTIME			:
//			case	CON_DI_TYPE_MPLATELYMAXDEMANDCLEARTIME	:
//			case	CON_DI_TYPE_MPPROGTIMETIMES				:
//			case	CON_DI_TYPE_MPMAXDEMANDCLEARTIMES		:
//			case	CON_DI_TYPE_MPB21F_BLOCK				:
//				pstIsSendFlag->ucProgramTimeFlag=CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPPOWCONST					:
//			case	CON_DI_TYPE_MPVARCONST					:
//			case	CON_DI_TYPE_MPCONST_BLOCK				:
//				pstIsSendFlag->ucPulseConstFlag=CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPMETERSTATFLAG				:
//			case	CON_DI_TYPE_MPCO2F_BLOCK				:
//				pstIsSendFlag->ucMeterStatFlag=CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPCOPYDAYTIME				:
//			case	CON_DI_TYPE_MPC11F_BLOCK				:
//				pstIsSendFlag->ucCopyDayTimeFlag=CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPLOSTVOLNUM				:
//			case	CON_DI_TYPE_MPB31F_BLOCK				:
//				pstIsSendFlag->ucLostVolNumFlag=CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPFEERATE1		:
//			case	CON_DI_TYPE_MPFEERATE2		:
//			case	CON_DI_TYPE_MPFEERATE3		:
//			case	CON_DI_TYPE_MPFEERATE4		:
//			case	CON_DI_TYPE_MPFEERATE5		:
//			case	CON_DI_TYPE_MPFEERATE6		:
//			case	CON_DI_TYPE_MPFEERATE7		:
//			case	CON_DI_TYPE_MPFEERATE8		:
//			case	CON_DI_TYPE_MPFEERATE_BLOCK	:
//				pstIsSendFlag->ucFeeRateFlag=CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPDATEANDWEEK	:
//			case	CON_DI_TYPE_MPTIME			:
//			case	CON_DI_TYPE_MPDATETIME_BLOCK	:
//				pstIsSendFlag->ucDateTimeFlag =CONCOMMAND_SEND;
//				break;
//				
//			case	CON_DI_TYPE_MPPROGRAMNUM	:
//				pstIsSendFlag->ucProgramNumFlag =CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPPROGRAMTIME:
//				pstIsSendFlag->ucLastProgramTimeFlag =CONCOMMAND_SEND;
//				break;			
//			case	CON_DI_TYPE_MPMETERCLRNUM :
//				pstIsSendFlag->ucMeterclrNumFlag =CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPMETERCLRTIME :
//				pstIsSendFlag->ucLastMeterclrTimeFlag =CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPDEMODECLRNUM :
//				pstIsSendFlag->ucDemondclrNumFlag =CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPDEMODECLRTIME :
//				pstIsSendFlag->ucLastDemondclrTimeFlag =CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPEVENTCLRNUM:
//				pstIsSendFlag->ucEventclrNumFlag=CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPEVENTCLRTIME:
//				pstIsSendFlag->ucLastEventclrTimeFlag =CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPJIAOSHINUM:
//				pstIsSendFlag->ucJiaoshiNumFlag =CONCOMMAND_SEND;
//				break;
//			case	CON_DI_TYPE_MPJIAOSHITIME:
//				pstIsSendFlag->ucLastJiaoshiTimeFlag =CONCOMMAND_SEND;
//				break;
//			default:
//				break;
//			}
//		}
//		pstDataItem_alr+=1;
//	}
//	
//	return ucMPNo;
//}

unsigned char Get_Shield_ACD_byAlrCode(unsigned short usAlrCode,unsigned char ucFlag)
{
	unsigned char ucRet=0;
// 	unsigned char ucGroupNo=0;
	STALRCATEGORY *pstAlrShieldACDWord;
	pstAlrShieldACDWord	= RunPara_GetAlrShieldACDWord(ucFlag);
		
	switch(usAlrCode)
	{
	case CONALRCODE_ALLDATAINIT	:	//ERC1：数据初始化
	case CONALRCODE_SOFTWARECHANGE:	//ERC1：程序更新
		ucRet = pstAlrShieldACDWord->ucTerDataInit;
		break;
	case CONALRCODE_PARA_LOST	:	//ERC2：参数丢失
		ucRet = pstAlrShieldACDWord->ucTerParaLost;
		break;
	case CONALRCODE_PARA_MODIFY	:	//ERC3：重要参数变更
		ucRet = pstAlrShieldACDWord->ucTerParaChange;
		break;
//	case	CONALRCODE_VAOVERUP	:	//电压越限_Va
//	case	CONALRCODE_VBOVERUP	:	//电压越限_Vb
//	case	CONALRCODE_VCOVERUP	:	//电压越限_Vc
//	case	CONALRCODE_VAOVERDOWN:	//电压越限_Va(下限)
//	case	CONALRCODE_VBOVERDOWN:	//电压越限_Vb(下限)
//	case	CONALRCODE_VCOVERDOWN:	//电压越限_Vc(下限)
//		ucRet = pstAlrShieldACDWord->ucVolOverUp;
//		break;
//	case	CONALRCODE_IAOVERUP	:	//电流越限_Ia
//	case	CONALRCODE_IBOVERUP	:	//电流越限_Ib
//	case	CONALRCODE_ICOVERUP	:	//电流越限_Ic
//	case	CONALRCODE_IAERROVERUP:	//电流越上上限_Ia
//	case	CONALRCODE_IBERROVERUP:	//电流越上上限_Ib
//	case	CONALRCODE_ICERROVERUP:	//电流越上上限_Ic
//		ucRet = pstAlrShieldACDWord->ucCurOverUp;
//		break;
//	case	CONALRCODE_VABREAK:		//A电压断相
//	case	CONALRCODE_VBBREAK:		//B电压断相
//	case	CONALRCODE_VCBREAK:		//C电压断相
//	case	CONALRCODE_VALACK:		//A电压缺相
//	case	CONALRCODE_VBLACK:		//B电压缺相
//	case	CONALRCODE_VCLACK:		//C电压缺相
//		ucRet = pstAlrShieldACDWord->ucTVErr;
//		break;
//	case	CONALRCODE_POLREVERSEA:	//反极性
//	case	CONALRCODE_POLREVERSEB:
//	case	CONALRCODE_POLREVERSEC:
//	case	CONALRCODE_SHORTCIRA1:	//CT一次侧短路
//	case	CONALRCODE_SHORTCIRB1:
//	case	CONALRCODE_SHORTCIRC1:
//	case	CONALRCODE_SHORTCIRA2:	//CT二次侧短路
//	case	CONALRCODE_SHORTCIRB2:
//	case	CONALRCODE_SHORTCIRC2:
//	case	CONALRCODE_OPENCIRA2:	//CT二次侧开路
//	case	CONALRCODE_OPENCIRB2:
//	case	CONALRCODE_OPENCIRC2:
//		ucRet = pstAlrShieldACDWord->ucTAErr;
//		break;
//	case	CONALRCODE_VOLCIRREVERSE:	//电压回路逆相序
//		ucRet = pstAlrShieldACDWord->ucPhaseErr;
//		break;		
//	case	CONALRCODE_SPOWNORUP:	//视在功率越限
//	case	CONALRCODE_SPOWERRUP:	//视在功率越上上限
//		ucRet = pstAlrShieldACDWord->ucSPowOverUp;
//		break;
//	case	CONALRCODE_IUNBLANCE:	//电流不平衡越限
//	case	CONALRCODE_UUNBLANCE:	//电压不平衡越限
//		ucRet = pstAlrShieldACDWord->ucUnbalanceOver;
//		break;
//	case	CONALRCODE_PROGRAMTIME:		//编程时间更改
//	case	CONALRCODE_DEMANDTIMECLR:	//需量清零时间更改
//	case	CONALRCODE_PULSECONST:		//脉冲常数
//	case	CONALRCODE_COPYDAYTIME:		//抄表日
//	case	CONALRCODE_FEERATE:			//费率更改
//		ucRet = pstAlrShieldACDWord->ucMeterParaChange;
//		break;
	case	CONALRCODE_METERBATTLOW:	//电表电池电压低
	case	CONALRCODE_DEMANDTIMENUM:	//需量清零次数变化
	case	CONALRCODE_PROGRAMNUM:		//编程次数变化
	case	CONALRCODE_LOSTVOLNUM:		//断相次数
		ucRet = pstAlrShieldACDWord->ucMeterErrInfo;
		break;
//	case	CONALRCODE_METERFLY:		//表计飞走
//		ucRet = pstAlrShieldACDWord->ucMeterFly;
//		break;
//	case	CONALRCODE_METERSTOP:		//表计停走
//		ucRet = pstAlrShieldACDWord->ucMeterStop;
//		break;
//	case	CONALRCODE_METERDOWN:		//示度下降
//		ucRet = pstAlrShieldACDWord->ucMeterDown;
//		break;
	case	CONALRCODE_METERTIMEERR:	//电表时钟异常
		ucRet = pstAlrShieldACDWord->ucMeterTimeErr;
		break;
// 	case	CONALRCODE_SUMINSTANTPOWUP1:	case	CONALRCODE_SUMINSTANTPOWUP2:
// 	case	CONALRCODE_SUMINSTANTPOWUP3:	case	CONALRCODE_SUMINSTANTPOWUP4:	
// 	case	CONALRCODE_SUMINSTANTPOWUP5:	case	CONALRCODE_SUMINSTANTPOWUP6:
// 	case	CONALRCODE_SUMINSTANTPOWUP7:	case	CONALRCODE_SUMINSTANTPOWUP8:
// 	case	CONALRCODE_SUMINSTANTPOWDOWN1:	case	CONALRCODE_SUMINSTANTPOWDOWN2:
// 	case	CONALRCODE_SUMINSTANTPOWDOWN3:	case	CONALRCODE_SUMINSTANTPOWDOWN4:
// 	case	CONALRCODE_SUMINSTANTPOWDOWN5:	case	CONALRCODE_SUMINSTANTPOWDOWN6:
// 	case	CONALRCODE_SUMINSTANTPOWDOWN7:	case	CONALRCODE_SUMINSTANTPOWDOWN8:
// 		ucRet = pstAlrShieldACDWord->ucSumInstantPowOver[0];
// 		break;
// 	case	CONALRCODE_SUMCOMPOVERLIMIT:	//总加组差动越限
// 	case	CONALRCODE_SUMCOMPOVERLIMIT+1:
// 	case	CONALRCODE_SUMCOMPOVERLIMIT+2:
// 	case	CONALRCODE_SUMCOMPOVERLIMIT+3:
// 	case	CONALRCODE_SUMCOMPOVERLIMIT+4:
// 	case	CONALRCODE_SUMCOMPOVERLIMIT+5:
// 	case	CONALRCODE_SUMCOMPOVERLIMIT+6:
// 	case	CONALRCODE_SUMCOMPOVERLIMIT+7:
// 		ucRet = pstAlrShieldACDWord->ucSumCompOverLimit;
// 		break;
// 	case	CONALRCODE_SUMINSTANTVARUP1:	case	CONALRCODE_SUMINSTANTVARUP2:
// 	case	CONALRCODE_SUMINSTANTVARUP3:	case	CONALRCODE_SUMINSTANTVARUP4:
// 	case	CONALRCODE_SUMINSTANTVARUP5:	case	CONALRCODE_SUMINSTANTVARUP6:
// 	case	CONALRCODE_SUMINSTANTVARUP7:	case	CONALRCODE_SUMINSTANTVARUP8:
// 	case	CONALRCODE_SUMINSTANTVARDOWN1:	case	CONALRCODE_SUMINSTANTVARDOWN2:
// 	case	CONALRCODE_SUMINSTANTVARDOWN3:	case	CONALRCODE_SUMINSTANTVARDOWN4:	
// 	case	CONALRCODE_SUMINSTANTVARDOWN5:	case	CONALRCODE_SUMINSTANTVARDOWN6:
// 	case	CONALRCODE_SUMINSTANTVARDOWN7:	case	CONALRCODE_SUMINSTANTVARDOWN8:
// 		ucRet = pstAlrShieldACDWord->ucSumInstantVarOver[0];
// 		break;
// 	case	CONALRCODE_SUMPOWUP1:	case	CONALRCODE_SUMPOWUP2:
// 	case	CONALRCODE_SUMPOWUP3:	case	CONALRCODE_SUMPOWUP4:	
// 	case	CONALRCODE_SUMPOWUP5:	case	CONALRCODE_SUMPOWUP6:
// 	case	CONALRCODE_SUMPOWUP7:	case	CONALRCODE_SUMPOWUP8:
// 	case	CONALRCODE_SUMPOWDOWN1:	case	CONALRCODE_SUMPOWDOWN2:
// 	case	CONALRCODE_SUMPOWDOWN3:	case	CONALRCODE_SUMPOWDOWN4:
// 	case	CONALRCODE_SUMPOWDOWN5:	case	CONALRCODE_SUMPOWDOWN6:
// 	case	CONALRCODE_SUMPOWDOWN7:	case	CONALRCODE_SUMPOWDOWN8:
// 		ucRet = pstAlrShieldACDWord->ucSumPowValOver[0];
// 		break;
// 	case	CONALRCODE_SUMVARUP1:	case	CONALRCODE_SUMVARUP2:
// 	case	CONALRCODE_SUMVARUP3:	case	CONALRCODE_SUMVARUP4:
// 	case	CONALRCODE_SUMVARUP5:	case	CONALRCODE_SUMVARUP6:
// 	case	CONALRCODE_SUMVARUP7:	case	CONALRCODE_SUMVARUP8:
// 	case	CONALRCODE_SUMVARDOWN1:	case	CONALRCODE_SUMVARDOWN2:
// 	case	CONALRCODE_SUMVARDOWN3:	case	CONALRCODE_SUMVARDOWN4:	
// 	case	CONALRCODE_SUMVARDOWN5:	case	CONALRCODE_SUMVARDOWN6:
// 	case	CONALRCODE_SUMVARDOWN7:	case	CONALRCODE_SUMVARDOWN8:
// 		ucRet = pstAlrShieldACDWord->ucSumVarValOver[0];
// 		break;
//	case	CONALRCODE_DOOROPEN:
//	case	CONALRCODE_DOORCLOSE:
//	case	CONALRCODE_SWITCH0OPEN:
//	case	CONALRCODE_SWITCH0CLOSE:
//	case	CONALRCODE_SWITCH1OPEN:
//	case	CONALRCODE_SWITCH1CLOSE:
//	case	CONALRCODE_SWITCH2OPEN:
//	case	CONALRCODE_SWITCH2CLOSE:
//	case	CONALRCODE_SWITCH3OPEN:
//	case	CONALRCODE_SWITCH3CLOSE:
//	case	CONALRCODE_SWITCH4OPEN:
//	case	CONALRCODE_SWITCH4CLOSE:
//	case	CONALRCODE_SWITCH5OPEN:
//	case	CONALRCODE_SWITCH5CLOSE:
//	case	CONALRCODE_SWITCH6OPEN:
//	case	CONALRCODE_SWITCH6CLOSE:
//		ucRet = pstAlrShieldACDWord->ucStatChange;
//		break;
	case	CONALRCODE_POWERON:
	case	CONALRCODE_POWEROFF:
		ucRet = pstAlrShieldACDWord->ucTerPowOff;
		break;
	case	CONALRCODE_TERERR_RAM:				//终端主板内存故障
	case	CONALRCODE_TERERR_CLOCK:			//时钟故障
	case	CONALRCODE_TERERR_MAINCOMM:			//主板通信故障
	case	CONALRCODE_TERERR_485ERROR:			//485抄表故障
	case	CONALRCODE_TERERR_DISPLAY:			//显示板故障
		ucRet = pstAlrShieldACDWord->ucTerErr;
		break;
	case	CONALRCODE_COMM_PASSWORDERR:		//密码错误
		ucRet = pstAlrShieldACDWord->ucPassErr;
		break;
//	case	CONALRCODE_REMOTECTRL:				//遥控跳闸记录
//		ucRet = pstAlrShieldACDWord->ucRemoteCtrl;
//		break;
//	case	CONALRCODE_POWERCTRL:				//功控跳闸记录
//		ucRet = pstAlrShieldACDWord->ucPowerCtrl;
//		break;
//	case	CONALRCODE_ENERCTRL:				//电控跳闸记录
//		ucRet = pstAlrShieldACDWord->ucEnergyCtrl;
//		break;
//	case	CONALRCODE_BUYPOWPARA:				//购电参数设置
//		ucRet = pstAlrShieldACDWord->ucBuyPowPara;
//		break;
//	case	CONALRCODE_ENERALR:					//ERC23 电控告警事件
//		ucRet = pstAlrShieldACDWord->ucEnerAlr;
//		break;
	case	CONALRCODE_METERCOPY:				//ERC31 485抄表失败事件记录
		ucRet = pstAlrShieldACDWord->ucMeterCopy;
		break;
	case	CONALRCODE_FLUXLIMIT:				//ERC32 月通信超流量记录
		ucRet = pstAlrShieldACDWord->ucFluxLimit;
		break;
//	case   CONALRCODE_POWDIFFOVER:// ERC28	电能量超差记录
//		ucRet = pstAlrShieldACDWord->ucPowDiffOver;
//		break;
	case   CONALRCODE_METERSTATECHANGE:// ERC33
		ucRet = pstAlrShieldACDWord->ucMeterState;
		break;
	default:
		if(ucFlag == CON_ALR_SHIELDFLAG)
			ucRet = CON_ALR_SHIELD;
		else
			ucRet = CON_ALR_ACD_NOT;
		break;
	}
	return ucRet;
}
/*
unsigned char Get_ACD_byAlrCode(unsigned short usAlrCode)
{
	unsigned char ucRet=0,ucGroupNo=0;
	STALRCATEGORY	stAlrACDWord;
	stAlrACDWord	= RunPara_GetAlrACDWord();
	switch(usAlrCode)
	{
	case CONALRCODE_ALLDATAINIT	:	//ERC1：数据初始化
	case CONALRCODE_SOFTWARECHANGE	:
		ucRet=stAlrACDWord.ucTerDataInit;
		break;
	case CONALRCODE_PARA_LOST	:	//ERC2：参数丢失
		ucRet=stAlrACDWord.ucTerParaLost;
		break;
	case CONALRCODE_PARA_MODIFY	:	//ERC3：重要参数变更
		ucRet=stAlrACDWord.ucTerParaChange;
		break;
	case	CONALRCODE_VAOVERUP	:	//电压越限_Va
	case	CONALRCODE_VBOVERUP	:	//电压越限_Vb
	case	CONALRCODE_VCOVERUP	:	//电压越限_Vc
	case	CONALRCODE_VAOVERDOWN:	//电压越限_Va(下限)
	case	CONALRCODE_VBOVERDOWN:	//电压越限_Vb(下限)
	case	CONALRCODE_VCOVERDOWN:	//电压越限_Vc(下限)
		ucRet=stAlrACDWord.ucVolOverUp;
		break;
	case	CONALRCODE_IAOVERUP	:	//电流越限_Ia
	case	CONALRCODE_IBOVERUP	:	//电流越限_Ib
	case	CONALRCODE_ICOVERUP	:	//电流越限_Ic
	case	CONALRCODE_IAERROVERUP:	//电流越上上限_Ia
	case	CONALRCODE_IBERROVERUP:	//电流越上上限_Ib
	case	CONALRCODE_ICERROVERUP:	//电流越上上限_Ic
		ucRet=stAlrACDWord.ucCurOverUp;
		break;
	case	CONALRCODE_VABREAK:		//A电压断相
	case	CONALRCODE_VBBREAK:		//B电压断相
	case	CONALRCODE_VCBREAK:		//C电压断相
	case	CONALRCODE_VALACK:		//A电压缺相
	case	CONALRCODE_VBLACK:		//B电压缺相
	case	CONALRCODE_VCLACK:		//C电压缺相
		ucRet = stAlrACDWord.ucTVErr;
		break;
	case	CONALRCODE_POLREVERSEA:	//反极性
	case	CONALRCODE_POLREVERSEB:
	case	CONALRCODE_POLREVERSEC:
	case	CONALRCODE_SHORTCIRA1:	//CT一次侧短路
	case	CONALRCODE_SHORTCIRB1:
	case	CONALRCODE_SHORTCIRC1:
	case	CONALRCODE_SHORTCIRA2:	//CT二次侧短路
	case	CONALRCODE_SHORTCIRB2:
	case	CONALRCODE_SHORTCIRC2:
	case	CONALRCODE_OPENCIRA2:	//CT二次侧开路
	case	CONALRCODE_OPENCIRB2:
	case	CONALRCODE_OPENCIRC2:
		ucRet = stAlrACDWord.ucTAErr;
		break;		
	case	CONALRCODE_VOLCIRREVERSE:		//电压回路逆相序
		ucRet = stAlrACDWord.ucPhaseErr;
		break;		
	case	CONALRCODE_SPOWNORUP:	//视在功率越限
	case	CONALRCODE_SPOWERRUP:	//视在功率越上上限
		ucRet=stAlrACDWord.ucSPowOverUp;
		break;
	case	CONALRCODE_IUNBLANCE:	//电流不平衡越限
	case	CONALRCODE_UUNBLANCE:	//电压不平衡越限
		ucRet=stAlrACDWord.ucUnbalanceOver;
		break;
	case	CONALRCODE_PROGRAMTIME:		//编程时间更改
	case	CONALRCODE_DEMANDTIMECLR:	//需量清零时间更改
	case	CONALRCODE_PULSECONST:		//脉冲常数
	case	CONALRCODE_COPYDAYTIME:		//抄表日
	case	CONALRCODE_FEERATE:			//费率更改
		ucRet = stAlrACDWord.ucMeterParaChange;
		break;
	case	CONALRCODE_METERBATTLOW:	//电表电池电压低
	case	CONALRCODE_DEMANDTIMENUM:	//需量清零次数变化
	case	CONALRCODE_PROGRAMNUM:		//编程次数变化
	case	CONALRCODE_LOSTVOLNUM:		//断相次数
		ucRet = stAlrACDWord.ucMeterErrInfo;
		break;
	case	CONALRCODE_METERFLY:		//表计飞走
		ucRet = stAlrACDWord.ucMeterFly;
		break;
	case	CONALRCODE_METERSTOP:		//表计停走
		ucRet = stAlrACDWord.ucMeterStop;
		break;
	case	CONALRCODE_METERDOWN:		//示度下降
		ucRet = stAlrACDWord.ucMeterDown;
		break;
	case	CONALRCODE_METERTIMEERR:
		ucRet = stAlrACDWord.ucMeterTimeErr;
		break;
	case	CONALRCODE_SUMCOMPOVERLIMIT:
	case	CONALRCODE_SUMCOMPOVERLIMIT+1:
	case	CONALRCODE_SUMCOMPOVERLIMIT+2:
	case	CONALRCODE_SUMCOMPOVERLIMIT+3:
	case	CONALRCODE_SUMCOMPOVERLIMIT+4:
	case	CONALRCODE_SUMCOMPOVERLIMIT+5:
	case	CONALRCODE_SUMCOMPOVERLIMIT+6:
	case	CONALRCODE_SUMCOMPOVERLIMIT+7:
		ucRet = stAlrACDWord.ucSumCompOverLimit;
		break;
	case	CONALRCODE_SUMINSTANTPOWUP1:	case	CONALRCODE_SUMINSTANTPOWUP2:
	case	CONALRCODE_SUMINSTANTPOWUP3:	case	CONALRCODE_SUMINSTANTPOWUP4:	
	case	CONALRCODE_SUMINSTANTPOWUP5:	case	CONALRCODE_SUMINSTANTPOWUP6:
	case	CONALRCODE_SUMINSTANTPOWUP7:	case	CONALRCODE_SUMINSTANTPOWUP8:
	case	CONALRCODE_SUMINSTANTPOWDOWN1:	case	CONALRCODE_SUMINSTANTPOWDOWN2:
	case	CONALRCODE_SUMINSTANTPOWDOWN3:	case	CONALRCODE_SUMINSTANTPOWDOWN4:
	case	CONALRCODE_SUMINSTANTPOWDOWN5:	case	CONALRCODE_SUMINSTANTPOWDOWN6:
	case	CONALRCODE_SUMINSTANTPOWDOWN7:	case	CONALRCODE_SUMINSTANTPOWDOWN8:
		ucRet = stAlrACDWord.ucSumInstantPowOver[0];
		break;
	case	CONALRCODE_SUMINSTANTVARUP1:	case	CONALRCODE_SUMINSTANTVARUP2:
	case	CONALRCODE_SUMINSTANTVARUP3:	case	CONALRCODE_SUMINSTANTVARUP4:
	case	CONALRCODE_SUMINSTANTVARUP5:	case	CONALRCODE_SUMINSTANTVARUP6:
	case	CONALRCODE_SUMINSTANTVARUP7:	case	CONALRCODE_SUMINSTANTVARUP8:
	case	CONALRCODE_SUMINSTANTVARDOWN1:	case	CONALRCODE_SUMINSTANTVARDOWN2:
	case	CONALRCODE_SUMINSTANTVARDOWN3:	case	CONALRCODE_SUMINSTANTVARDOWN4:	
	case	CONALRCODE_SUMINSTANTVARDOWN5:	case	CONALRCODE_SUMINSTANTVARDOWN6:
	case	CONALRCODE_SUMINSTANTVARDOWN7:	case	CONALRCODE_SUMINSTANTVARDOWN8:
		ucRet = stAlrACDWord.ucSumInstantVarOver[0];
		break;
	case	CONALRCODE_SUMPOWUP1:	case	CONALRCODE_SUMPOWUP2:
	case	CONALRCODE_SUMPOWUP3:	case	CONALRCODE_SUMPOWUP4:	
	case	CONALRCODE_SUMPOWUP5:	case	CONALRCODE_SUMPOWUP6:
	case	CONALRCODE_SUMPOWUP7:	case	CONALRCODE_SUMPOWUP8:
	case	CONALRCODE_SUMPOWDOWN1:	case	CONALRCODE_SUMPOWDOWN2:
	case	CONALRCODE_SUMPOWDOWN3:	case	CONALRCODE_SUMPOWDOWN4:
	case	CONALRCODE_SUMPOWDOWN5:	case	CONALRCODE_SUMPOWDOWN6:
	case	CONALRCODE_SUMPOWDOWN7:	case	CONALRCODE_SUMPOWDOWN8:
		ucRet = stAlrACDWord.ucSumPowValOver[0];
		break;
	case	CONALRCODE_SUMVARUP1:	case	CONALRCODE_SUMVARUP2:
	case	CONALRCODE_SUMVARUP3:	case	CONALRCODE_SUMVARUP4:
	case	CONALRCODE_SUMVARUP5:	case	CONALRCODE_SUMVARUP6:
	case	CONALRCODE_SUMVARUP7:	case	CONALRCODE_SUMVARUP8:
	case	CONALRCODE_SUMVARDOWN1:	case	CONALRCODE_SUMVARDOWN2:
	case	CONALRCODE_SUMVARDOWN3:	case	CONALRCODE_SUMVARDOWN4:	
	case	CONALRCODE_SUMVARDOWN5:	case	CONALRCODE_SUMVARDOWN6:
	case	CONALRCODE_SUMVARDOWN7:	case	CONALRCODE_SUMVARDOWN8:
		ucRet = stAlrACDWord.ucSumVarValOver[0];
		break;
	case	CONALRCODE_DOOROPEN:
	case	CONALRCODE_DOORCLOSE:
	case	CONALRCODE_SWITCH0OPEN:
	case	CONALRCODE_SWITCH0CLOSE:
	case	CONALRCODE_SWITCH1OPEN:
	case	CONALRCODE_SWITCH1CLOSE:
	case	CONALRCODE_SWITCH2OPEN:
	case	CONALRCODE_SWITCH2CLOSE:
	case	CONALRCODE_SWITCH3OPEN:
	case	CONALRCODE_SWITCH3CLOSE:
	case	CONALRCODE_SWITCH4OPEN:
	case	CONALRCODE_SWITCH4CLOSE:
	case	CONALRCODE_SWITCH5OPEN:
	case	CONALRCODE_SWITCH5CLOSE:
	case	CONALRCODE_SWITCH6OPEN:
	case	CONALRCODE_SWITCH6CLOSE:
		ucRet = stAlrACDWord.ucStatChange;
		break;
	case	CONALRCODE_POWERON:
	case	CONALRCODE_POWEROFF:
		ucRet = stAlrACDWord.ucTerPowOff;
		break;
	case	CONALRCODE_TERERR_RAM:				//终端主板内存故障
	case	CONALRCODE_TERERR_CLOCK:			//时钟故障
	case	CONALRCODE_TERERR_MAINCOMM:			//主板通信故障
	case	CONALRCODE_TERERR_485ERROR:			//485抄表故障
	case	CONALRCODE_TERERR_DISPLAY:			//显示板故障
		ucRet = stAlrACDWord.ucTerErr;
		break;
	case	CONALRCODE_COMM_PASSWORDERR:		//密码错误
		ucRet = stAlrACDWord.ucPassErr;
		break;
	case	CONALRCODE_REMOTECTRL:				//遥控跳闸记录
		ucRet = stAlrACDWord.ucRemoteCtrl;
		break;
	case	CONALRCODE_POWERCTRL:				//功控跳闸记录
		ucRet = stAlrACDWord.ucPowerCtrl;
		break;
	case	CONALRCODE_ENERCTRL:				//电控跳闸记录
		ucRet = stAlrACDWord.ucEnergyCtrl;
		break;
	case	CONALRCODE_BUYPOWPARA:				//购电参数设置
		ucRet = stAlrACDWord.ucBuyPowPara;
		break;
	case	CONALRCODE_ENERALR:					//ERC23 电控告警事件
		ucRet = stAlrACDWord.ucEnerAlr;
		break;
	case	CONALRCODE_METERCOPY:				//ERC31 485抄表失败事件记录
		ucRet = stAlrACDWord.ucMeterCopy;
		break;
	case	CONALRCODE_FLUXLIMIT:				//ERC32 月通信超流量记录
		ucRet = stAlrACDWord.ucFluxLimit;
		break;
	case	CONALRCODE_METERSTATECHANGE:		//ERC33
		ucRet = stAlrACDWord.ucMeterState;
		break;
	case   CONALRCODE_POWDIFFOVER:// ERC28	电能量超差记录
		ucRet = stAlrACDWord.ucPowDiffOver;
		break;
	default:
		ucRet = CON_ALR_ACD_NOT;
		break;
	}
	return ucRet;
}
*/
//获取最新的告警编码
//void Get_AlrCode_Newest(unsigned char ucResetFlag,unsigned short *pusAlrCode,unsigned char *pucMPNo)
//{
//	(*pusAlrCode) = stNewestAlarmCode.usAlrCode;
//	(*pucMPNo)	  = stNewestAlarmCode.ucMPNo;
//	if(ucResetFlag == 0x55)
//	{
//		stNewestAlarmCode.ucMPNo = 0;
//		stNewestAlarmCode.usAlrCode = 0;
//	}
//}
/***************************************************************************************/
//获取不平衡度
STHISVAL	GetUnBalanceVal(STHISVAL stHisA,STHISVAL stHisB,STHISVAL stHisC,unsigned char ucMeterType)
{
	STHISVAL		stHisVal;
	unsigned long	lMaxVal=0;
	lMaxVal=GetUnBalanceVal_byVal(stHisA.lVal,stHisB.lVal,stHisC.lVal,ucMeterType);
	if(lMaxVal==0xFFFFFFFF)
		ClearHisVal(&stHisVal);
	else
	{
		stHisVal = stHisA;		//获取历史值的时间等量
		stHisVal.lVal = lMaxVal;
	}
	return stHisVal;
}

//获取不平衡度
unsigned long GetUnBalanceVal_byVal(unsigned long ulValA,unsigned long ulValB,unsigned long ulValC,unsigned char ucMeterType)
{
	unsigned long	lMaxVal=0,lTmp=0;
	unsigned long	lRatingVal=0;
	if(	ulValA!=CONINVALIDVAL && ulValB!=CONINVALIDVAL && ulValC!=CONINVALIDVAL)
	{
		if(ucMeterType==CON_METERTYPE_3P3L)
		{
			lRatingVal = (ulValA +ulValC)/2;
			if(lRatingVal !=0)
			{
				lTmp = makelabs(ulValA-lRatingVal)*1000 / lRatingVal;		// 99.9%
				if(lTmp > lMaxVal)
					lMaxVal = lTmp;
				
				lTmp = makelabs(ulValC-lRatingVal)*1000 / lRatingVal;
				if(lTmp > lMaxVal)
					lMaxVal = lTmp;
			}else
				lMaxVal = 0x00;
		}else
		{
			lRatingVal = (ulValA +ulValB +ulValC)/3;
			if(lRatingVal != 0)
			{
				lTmp = makelabs(ulValA-lRatingVal)*1000 / lRatingVal;
				if(lTmp > lMaxVal)
					lMaxVal = lTmp;
				
				lTmp = makelabs(ulValB-lRatingVal)*1000 / lRatingVal;
				if(lTmp > lMaxVal)
					lMaxVal = lTmp;
				
				lTmp = makelabs(ulValC-lRatingVal)*1000 / lRatingVal;
				if(lTmp > lMaxVal)
					lMaxVal = lTmp;
			}else
				lMaxVal = 0x00;
		}
	}else
		lMaxVal=0xFFFFFFFF;
	return lMaxVal;
}

//unsigned char Get_CurrentDoorStat()
//{
//	unsigned char	ucRet=0;
//	unsigned char	ucTmp= 0;
//	STSWITCHPARA	stSwitchPara;
//
//	ucTmp = Door_stat_get();
//
//	stSwitchPara= RunPara_GetSwitchPara(7);
//
//	if(stSwitchPara.ucAttr == CON_SWITCHATTR_TYPEA)
//	{
//		if(ucTmp == 0x00)
//			ucRet = (CON_SWITCH_STAT_OPEN);
//		else
//			ucRet = (CON_SWITCH_STAT_CLOSE);
//	}
//	else if(stSwitchPara.ucAttr == CON_SWITCHATTR_TYPEB)
//	{
//		if(ucTmp == 0x00)
//			ucRet = (CON_SWITCH_STAT_CLOSE);
//		else
//			ucRet = (CON_SWITCH_STAT_OPEN);	
//	}
//	else
//	{
//		ucRet = (CON_SWITCH_STAT_OPEN);	
//	}
//	return ucRet;
//}

//unsigned char Get_CurrentSwitchStat(unsigned char ucSwitchNo)
//{
//	unsigned char	ucRet=CON_SWITCH_STAT_OPEN;
//	unsigned char	ucPinAddr3 = 0x00;
//	STSWITCHPARA	stSwitchPara;
//	unsigned char	ucPulseNo;
//
//	if(ucSwitchNo<4)
//	{
//		ucPinAddr3 = Get_SwitchInStatByte();				//输入断开，值为1
//		stSwitchPara= RunPara_GetSwitchPara(ucSwitchNo);
//		if(stSwitchPara.ucAttr == CON_SWITCHATTR_TYPEA)		//A型
//		{
//			if( (ucPinAddr3 &  (0x01<<ucSwitchNo)) == 0x00)
//				ucRet = (CON_SWITCH_STAT_CLOSE);
//			else
//				ucRet = (CON_SWITCH_STAT_OPEN);
//		}else if(stSwitchPara.ucAttr == CON_SWITCHATTR_TYPEB)	
//		{
//			if( (ucPinAddr3 &  (0x01<<ucSwitchNo)) == 0x00)
//				ucRet = (CON_SWITCH_STAT_OPEN);
//			else
//				ucRet = (CON_SWITCH_STAT_CLOSE);	
//		}else
//		{
//			ucRet = (CON_SWITCH_STAT_OPEN);
//		}
//	}
//	/*
//	else if(ucSwitchNo==6)
//	{//第三路状态量代替
//		ucPinAddr3 = Get_SwitchInStatByte();				//输入断开，值为1
//		stSwitchPara= RunPara_GetSwitchPara(ucSwitchNo);
//		if(stSwitchPara.ucAttr == CON_SWITCHATTR_TYPEA)		//A型
//		{
//			if( (ucPinAddr3 &  0x40) == 0x00)
//				ucRet = (CON_SWITCH_STAT_CLOSE);
//			else
//				ucRet = (CON_SWITCH_STAT_OPEN);
//		}else if(stSwitchPara.ucAttr == CON_SWITCHATTR_TYPEB)
//		{
//			if( (ucPinAddr3 &  0x40) == 0x00)
//				ucRet = (CON_SWITCH_STAT_OPEN);
//			else
//				ucRet = (CON_SWITCH_STAT_CLOSE);	
//		}else
//		{
//			ucRet = (CON_SWITCH_STAT_OPEN);
//		}
//	}else if(ucSwitchNo==4 || ucSwitchNo==5)
//	{
//		ucPulseNo=ucSwitchNo-4;
//		ucRet=Get_PulseSwtich_InStat(ucPulseNo);
//	}
//	*/
//	return ucRet;
//}

//unsigned char Get_Switch_Change_Info(void)
//{
//	return ucSwitchChange;
//}

//unsigned char Set_Switch_Change_Info(unsigned char ucInfo)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	ucSwitchChange=ucInfo;
//	return ucRet;
//}

//unsigned char Init_Switch_Change_Info(void)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	ucSwitchChange=0;
//	return ucRet;
//}

STDATETIME Get_TerPowerChangeTime(void)
{
	return g_stTerPowerChangeTime;
}

unsigned char Set_TerPowerChangeTime(STDATETIME *pstTime)
{
	unsigned char ucRet=RET_SUCCESS;
	g_stTerPowerChangeTime=(*pstTime);
	return ucRet;
}

void TimeChange_AlarmManage(STDATETIME stOldTime,STDATETIME stNewTime)
{//时钟变化处理
//azh
    // 	unsigned long ulOldTime,ulNewTime,ulDiff=0;
// 	ulOldTime=TimeConver(&stOldTime,CON_TIMEUNIT_SEC);
// 	ulNewTime=TimeConver(&stNewTime,CON_TIMEUNIT_SEC);
// 	if(ulOldTime>ulNewTime)
// 	{//时间向旧改
// 		ulDiff=ulOldTime-ulNewTime;
// 	}else//(ulNewTime>ulOldTime)
// 	{//时间向新改
// 		ulDiff=ulNewTime-ulOldTime;
// 	}
}

void CopyMeter_Timer_StartNow(void)
{//时间标志设置为无效，将立即重新抄表
	ucTimer15Flag=(unsigned char)CONINVALIDVAL;
}

void Set_AlrActionLockTimer(unsigned short usLockTimer)
{
	usAlrActionLockTimer=usLockTimer;
}



void Alarm_Meter_State(void)
{
// 	unsigned char i,ucMPNo,ucMPType;
// 	for(i=0;i<CON_METER_NUM;i++)
// 	{
// 		ucMPNo	 = RunPara_GetMeter_MPNo(i);
// 		ucMPType = RunPara_GetMPType(ucMPNo);
// 		if(ucMPType == CON_MPTYPE_METER)
// 			Alarm_ForeJudge_MeterState(ucMPNo);
// 	}
}


//unsigned char Alarm_ForeJudge_MeterState(unsigned char ucMPNo)
//{//表计状态字变位
//	unsigned char ucRet=RET_SUCCESS;
//	STHISSTATE	  stHisState1,stHisState2;
//	unsigned char ucMeter_No = RunPara_GetMeterNo_byMPNo(ucMPNo);
//	unsigned char i,ucChangeflag = 0;
//	for(i=1;i<8;i++)
//	{
//		stHisState1 = Get_History_MeterStatFlag_byMPNo(ucMPNo,0,i);
//		stHisState2 = Get_History_MeterStatFlag_byMPNo(ucMPNo,1,i);
//		ucMeterStateChangeFlag[ucMeter_No][i-1] = 0;
//		if((CONINVALIDVAL != stHisState1.ucStateFlag) && (CONINVALIDVAL != stHisState2.ucStateFlag))
//		{
//			ucMeterStateChangeFlag[ucMeter_No][i-1] = stHisState1.ucStateFlag ^ stHisState2.ucStateFlag;
//			if(ucMeterStateChangeFlag[ucMeter_No][i-1] != 0)//有变化
//			{
//				ucChangeflag = 1;
//			}
//		}
//	}
//	if(ucChangeflag ==1)
//	{
//		Alarm_FillEnsureJudgeList(CONALRCODE_METERSTATECHANGE,ucMPNo);
//	}
//	return ucRet;
//}

/******************************************************************************
** 函数名称: unsigned char Alarm_FillRS485MeterDi(unsigned char ucTaskSource)
** 函数功能: 填写告警相关的抄表数据标识
** 入口参数:
** 返 回 值:
**
** 说    明: 
******************************************************************************/
unsigned char Alarm_FillRS485MeterDi (unsigned char ucTaskSource)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char i=0,j=0,alm_no=0;
	unsigned short usDI=0;
	
	STDIINFO stDIInfoTmp;
    switch(ucTaskSource)
    {
    	case CONRS485SOUR_ALRAFFIRMTASK:					//报警确认任务0
    	case CONRS485SOUR_ALRAFFIRMTASK+1:					//报警确认任务1
    	case CONRS485SOUR_ALRAFFIRMTASK+2:					//报警确认任务2
    	case CONRS485SOUR_ALRAFFIRMTASK+3:					//报警确认任务3
    	case CONRS485SOUR_ALRAFFIRMTASK+4:					//报警确认任务4
    	case CONRS485SOUR_ALRAFFIRMTASK+5:					//报警确认任务5
    	case CONRS485SOUR_ALRAFFIRMTASK+6:					//报警确认任务6
    	case CONRS485SOUR_ALRAFFIRMTASK+7:					//报警确认任务7
    	case CONRS485SOUR_ALRAFFIRMTASK+8:					//报警确认任务8
    	case CONRS485SOUR_ALRAFFIRMTASK+9:					//报警确认任务9
    	{
    	    alm_no = ucTaskSource - CONRS485SOUR_ALRAFFIRMTASK;
    	    if(stAlrForeJudgeList[alm_no].ucAlrDataItemNum > CONALRITEMNUM)
    	    {
    	        stAlrForeJudgeList[alm_no].ucAlrDataItemNum = CONALRITEMNUM;
    	    }
        	for(i=0;i<stAlrForeJudgeList[alm_no].ucAlrDataItemNum;i++)
        	{
        		usDI=stAlrForeJudgeList[alm_no].stAlrDataItem[0].stDataItem_alr[i].usItemLabel;
        		
        		memset(&stDIInfoTmp,0,sizeof(STDIINFO));
        		stDIInfoTmp=Main_DI_Data_Query_Manage( usDI );			//查询

        		stSendDIList.ucDINum = 0;
        		if( stDIInfoTmp.ucHave==CON_DI_HAVE && 
        			stDIInfoTmp.ucType==CON_DI_TYPE_DATA && stDIInfoTmp.ucMPType==CON_DI_MPTYPE_NORMAL )
        		{
        			if( stDIInfoTmp.ucReadMeterEnabel==CON_DI_OPERATE_ENABLE )
        			{
        				for( j=0;j<stSendDIList.ucDINum;j++ )
        				{
        					if( stSendDIList.usDILabel[j]==stDIInfoTmp.usBigDI )
        						break;		//在输出列表找到当前待加入的数据项的大项命令，则拒绝该数据项；找不到，则加入大项目命令
        				}
        				if( j>=stSendDIList.ucDINum )
        				{
        					stSendDIList.usDILabel[stSendDIList.ucDINum++] = stDIInfoTmp.usBigDI;
        				}
        			}
        		}
        	}
    	    stSendDIList.ucDIPtr = 0;
            stSendDIList.ucReSendnum = 0;
    	    
    	    break;   
    	}    
//抄读告警携带数据
    	case CONRS485SOURALR_1:								//报警补招数据0
    	case CONRS485SOURALR_1+1:							//报警补招数据1
    	case CONRS485SOURALR_1+2:							//报警补招数据2
    	case CONRS485SOURALR_1+3:							//报警补招数据3
    	case CONRS485SOURALR_1+4:							//报警补招数据4
    	case CONRS485SOURALR_1+5:							//报警补招数据5
    	case CONRS485SOURALR_1+6:							//报警补招数据6
    	case CONRS485SOURALR_1+7:							//报警补招数据7
    	case CONRS485SOURALR_1+8:							//报警补招数据8
    	case CONRS485SOURALR_1+9:							//报警补招数据9
    	{
    	    alm_no = ucTaskSource - CONRS485SOURALR_1;
    	    if(stAlrEnsureJudgeList[alm_no].ucAlrDataItemNum > CONALRTAKEITEMNUM)
    	    {
    	        stAlrEnsureJudgeList[alm_no].ucAlrDataItemNum = CONALRTAKEITEMNUM;
    	    }
        	for(i=0;i<stAlrEnsureJudgeList[alm_no].ucAlrDataItemNum;i++)
        	{
        		usDI=stAlrEnsureJudgeList[alm_no].stAlrDataItem[i].usItemLabel;
        		
        		memset(&stDIInfoTmp,0,sizeof(STDIINFO));
        		stDIInfoTmp=Main_DI_Data_Query_Manage( usDI );			//查询

        		stSendDIList.ucDINum = 0;
        		if( stDIInfoTmp.ucHave==CON_DI_HAVE && 
        			stDIInfoTmp.ucType==CON_DI_TYPE_DATA && stDIInfoTmp.ucMPType==CON_DI_MPTYPE_NORMAL )
        		{
        			if( stDIInfoTmp.ucReadMeterEnabel==CON_DI_OPERATE_ENABLE )
        			{
        				for( j=0;j<stSendDIList.ucDINum;j++ )
        				{
        					if( stSendDIList.usDILabel[j]==stDIInfoTmp.usBigDI )
        						break;		//在输出列表找到当前待加入的数据项的大项命令，则拒绝该数据项；找不到，则加入大项目命令
        				}
        				if( j>=stSendDIList.ucDINum )
        				{
        					stSendDIList.usDILabel[stSendDIList.ucDINum++] = stDIInfoTmp.usBigDI;
        				}
        			}
        		}
        	}
    	    stSendDIList.ucDIPtr = 0;
            stSendDIList.ucReSendnum = 0;
    	    
    	    break;   
    	}
    }
	
	return ucRet;
}
//告警任务抄完数据后置结束或其他标志
void Set_Alarm_CopyMeter_Flag(unsigned char uctasktype, unsigned char uctaskno, unsigned short flag)
{
    unsigned char taskno;
    
    if(uctasktype == CON_RS485ASKOPESOUR_ALARMFORE)
    {
       	taskno = uctaskno - CONRS485SOUR_ALRAFFIRMTASK;
       	if(taskno < CONALRFORELISTNUM)
		{
		     usForeStartFlag[taskno] = flag;  
		}
    }
    else if(uctasktype == CON_RS485ASKOPESOUR_ALARMUP)
    {
       	taskno = uctaskno - CONRS485SOURALR_1;
       	if(taskno < CONALRENSURELISTNUM)
		{
		     usEnsureStarFlag[taskno] = flag;  
		}               	            
    }
}
//清除告警任务队列
void Clr_Alarm_CopyMeter_Flag(unsigned char uctasktype, unsigned char uctaskno)
{
    unsigned char taskno;
    
    if(uctasktype == CON_RS485ASKOPESOUR_ALARMFORE)
    {
       	taskno = uctaskno - CONRS485SOUR_ALRAFFIRMTASK;
       	if(taskno < CONALRFORELISTNUM)
		{
		     Clear_AlrForeJudge(&(stAlrForeJudgeList[taskno]));
		     usForeStartFlag[taskno] = CON_STATUS_INIT;  
		}
    }
    else if(uctasktype == CON_RS485ASKOPESOUR_ALARMUP)
    {
       	taskno = uctaskno - CONRS485SOURALR_1;
       	if(taskno < CONALRENSURELISTNUM)
		{
		     Clear_AlrEnsureJudge(&(stAlrEnsureJudgeList[taskno]));
		     usEnsureStarFlag[taskno] = CON_STATUS_INIT;  
		}               	            
    }
}
//读出上次掉电时间
void Alarm_Get_Last_TerTime(void)
{
    unsigned char buf[PAR_TER_LAST_TIME_LEN];
    
    file_read(PAR_TER_LAST_TIME_ADDR, buf, PAR_TER_LAST_TIME_LEN); 
    if(buf[PAR_TER_LAST_TIME_LEN-1] == addverify(buf, (PAR_TER_LAST_TIME_LEN-1)))
    {
        memcpy(&g_stTerPowerChangeTime, buf, sizeof(STDATETIME));    
    }
    else
    {
        memset(&g_stTerPowerChangeTime, 0xee, sizeof(STDATETIME));
    }    
}

//保存上次掉电时间
void Alarm_Save_Last_TerTime(void)
{
    unsigned char buf[PAR_TER_LAST_TIME_LEN];

    g_stTerPowerChangeTime = GetCurrentTime();
    memcpy(buf, &g_stTerPowerChangeTime, sizeof(STDATETIME));   

    buf[PAR_TER_LAST_TIME_LEN-1] = addverify(buf, (PAR_TER_LAST_TIME_LEN-1));    
    file_write(PAR_TER_LAST_TIME_ADDR, buf, PAR_TER_LAST_TIME_LEN); 
}
//对抄表进程的监控 如果长时间占用超过5分钟 就强制释放 使其他进程能抄表
void MeterCopy_Proc_Check(void)
{
    if(stGRTaskExeInfo.ucTaskValidFlag == CON_TASK_RETDATA_VALID)
    {
        stGRTaskExeInfo.usValidCnt++;
    }
    else
    {
        stGRTaskExeInfo.usValidCnt = 0;
    }
    if(stGRTaskExeInfo.usValidCnt >= CON_METERCOPY_CHECK_TIME)//5分钟 单位ms
    {
        stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
        stGRTaskExeInfo.usValidCnt = 0;
    }
    
}

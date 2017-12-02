/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : CTask_Manage.c
 Description    : 曲线数据模块任务管理
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-08-15    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

static unsigned char Task_Check_Task_BaseInfo(unsigned char ucTaskNo);		//检查任务基本信息
//-----------------------------------
//azh 二类曲线数据
const U16 curve_task_di[CON_TASK_CURVE_DI_NUM] =
{
    CON_DI_TYPE_MPDIRPOWTOTAL,		//正向有功总电能
    CON_DI_TYPE_MPDIRVARTOTAL,		//正向无功总电能
    CON_DI_TYPE_MPUNDIRPOWTOTAL,	//反向有功总电能
    CON_DI_TYPE_MPUNDIRVARTOTAL,	//反向无功总电能
    CON_DI_TYPE_MPVAR1,				//一象限无功总电能
    CON_DI_TYPE_MPVAR4,				//四象限无功总电能
    CON_DI_TYPE_MPVAR2,				//二象限无功总电能
    CON_DI_TYPE_MPVAR3,				//三象限无功总电能
    CON_DI_TYPE_MPINSTANTPOW,		//瞬时有功功率
    CON_DI_TYPE_MPINSTANTPOWA,		//A相瞬时有功功率
    CON_DI_TYPE_MPINSTANTPOWB,		//B相瞬时有功功率
    CON_DI_TYPE_MPINSTANTPOWC,		//C相瞬时有功功率
    CON_DI_TYPE_MPINSTANTVAR,		//瞬时无功功率
    CON_DI_TYPE_MPINSTANTVARA,		//A相瞬时无功功率
    CON_DI_TYPE_MPINSTANTVARB,		//B相瞬时无功功率
    CON_DI_TYPE_MPINSTANTVARC,		//C相瞬时无功功率
    CON_DI_TYPE_MPVOLA,				//A相电压
    CON_DI_TYPE_MPVOLB,				//B相电压
    CON_DI_TYPE_MPVOLC,				//C相电压
    CON_DI_TYPE_MPCURA,				//A相电流
    CON_DI_TYPE_MPCURB,				//B相电流
    CON_DI_TYPE_MPCURC				//C相电流
};

//任务控制初始化
//检查DATAFLASH存储的任务信息，有效则更新当前任务控制数据区，无效则初始化DATAFLASH和当前数据区
unsigned char Task_Manage_Init(void)
{
	unsigned char ucRet=RET_SUCCESS;
//	unsigned char i;
//	ucRet=TaskCheckSumGroupIsValid();					//检查总加任务
//	if(ucRet==RET_ERROR)
//		SetDefaultSumGroupTask();
//	for(i=0;i<CON_PULSE_NUM;i++)
//	{
//		if(RunPara_GetPulse_MPNo(i)!=CON_MPNO_INVALID)
//		{
//			ucRet=TaskCheckPulseIsValid(i);				//检查脉冲任务
//			if(ucRet==RET_ERROR)
//				SetDefaultPulseTask(i);
//		}
//	}
    PowOn_Get_Curve_Task_Para();//azh 上电从eeprom中导入运行相关参数

	if(RunPara_GetTer_MPNo()!=CON_MPNO_INVALID)
	{
		ucRet=TaskCheckTerIsValid();					//检查终端任务
		if(ucRet==RET_ERROR)
			SetDefaultTerTask();
	}
	ucRet=Task_CheckAllTaskInfo();						//矫正任务信息，返回结果：是否需要存储更新DATAFLASH
	Task_GetTaskRunInfo();								//获得任务运行信息
	return ucRet;
}

//矫正任务信息
unsigned char Task_CheckAllTaskInfo(void)
{
	unsigned char ucRet=RET_ERROR;
	unsigned char i;
	for(i=0;i<CON_GRTASK_NUM;i++)
	{
		if(stGRDefineArea.stGRTask[i].ucActive==CON_GRTASK_ACTIVE)
		{//先做任务有效性检查,后做有效配置
			if(Is_DateTime_Availble(&stGRDefineArea.stGRTask[i].stStartActive)==RET_ERROR)
			{    
				stGRDefineArea.stGRTask[i].ucActive=CON_GRTASK_NOACTIVE;
				Save_Curve_Task_Para(i, CON_TASK_ACTIVE_OFST, CON_TASK_PARA_SINGLE_SAVE);	
			}
			if(stGRDefineArea.stGRTask[i].stTITaskInter.ucTimeUnit>CON_TIMEUNIT_MON ||
				stGRDefineArea.stGRTask[i].stTITaskInter.ucTimeUnit<CON_TIMEUNIT_SEC)
			{	
				stGRDefineArea.stGRTask[i].ucActive=CON_GRTASK_NOACTIVE;
				Save_Curve_Task_Para(i, CON_TASK_ACTIVE_OFST, CON_TASK_PARA_SINGLE_SAVE);	
			}
		}
		else if(stGRDefineArea.stGRTask[i].ucActive==CON_GRTASK_NOACTIVE)
		{
			;//任务不清空，任务暂停状态
		}
		else
		{
			memset(&stGRDefineArea.stGRTask[i],CON_GRTASK_DATA_INVALID,sizeof(STGRTASK));
			stGRDefineArea.stGRTask[i].ucActive=CON_GRTASK_NOACTIVE;
			Save_Curve_Task_Para(i, CON_TASK_BLK_OFST, CON_TASK_PARA_BLOCK_SAVE);//azh
			memset(&stGRDefineArea.stGRTaskInfo[i],CON_GRTASK_DATA_INVALID,sizeof(STGRTASKINFO));
			Save_Curve_Task_Para(i, CON_TASKIF_BLK_OFST, CON_TASK_PARA_BLOCK_SAVE);
			ucRet=RET_SUCCESS;		//表示有修正内容需要到DATAFLASH
		}
	}
	
	return ucRet;
}

//获得任务运行信息
unsigned char Task_GetTaskRunInfo(void)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char i;
	unsigned long ulTimeInter;
	//根据任务配置信息和任务主信息初始化任务的运行信息
	for(i=0;i<CON_GRTASK_NUM;i++)
	{
		if(stGRDefineArea.stGRTask[i].ucActive==CON_GRTASK_ACTIVE)
		{
			stGRDefineArea.stGRTaskRunInfo[i].usHeadPos=stGRDefineArea.stGRTaskInfo[i].usExedNum % stGRDefineArea.stGRTask[i].usSavePointNum;
			//获得终端掉电前的时间，用于填充掉电期间的任务时间点的无效数据
			stGRDefineArea.stGRTaskRunInfo[i].stNextExeTime=stGRDefineArea.stGRTask[i].stStartActive;
//azh 160311---			
			if(stGRDefineArea.stGRTaskInfo[i].usExedNum >= stGRDefineArea.stGRTaskInfo[i].usTailPos)
			{
			    ulTimeInter = stGRDefineArea.stGRTaskInfo[i].usExedNum - stGRDefineArea.stGRTaskInfo[i].usTailPos;
			}
			else
			{
			    ulTimeInter = stGRDefineArea.stGRTaskInfo[i].usExedNum + stGRDefineArea.stGRTask[i].usSavePointNum - stGRDefineArea.stGRTaskInfo[i].usTailPos;
			}        
			ulTimeInter=(unsigned long)(ulTimeInter*stGRDefineArea.stGRTask[i].stTITaskInter.ucTimeInter);
//azh 160311---			
//			ulTimeInter=(unsigned long)(stGRDefineArea.stGRTaskInfo[i].usExedNum*stGRDefineArea.stGRTask[i].stTITaskInter.ucTimeInter);
			AddTime(&(stGRDefineArea.stGRTaskRunInfo[i].stNextExeTime),
				ulTimeInter,
				stGRDefineArea.stGRTask[i].stTITaskInter.ucTimeUnit);
			//获得当前上送时间
			stGRDefineArea.stGRTaskRunInfo[i].stNextUpTime=GetTaskNextUpTime(i);
			Save_Curve_Task_Para(i, CON_TASKRIF_BLK_OFST, CON_TASK_PARA_BLOCK_SAVE);//azh 保存到EE
		}
	}
	return ucRet;
}

//配置任务，由于总的任务数量、回应数据的空间分配都是固定的，因此，就无需进行任务的创建等工作。
//	任务的数据区56K24个任务
//  建立任务运行信息
//	更新stGRDefineArea
unsigned char Task_Config(unsigned char ucTaskNo,unsigned char ucActive)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long ulDestAddr = 0;
	unsigned long ulNum_Task = 0;
	if(ucTaskNo>=CON_GRTASK_NUM)
		return RET_ERROR;
	
	//由GRTASK获得GRTASKINFO、GRTASKRUNINFO
	stGRDefineArea.stGRTask[ucTaskNo].ucActive=ucActive;
	stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum=0;
	stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos=0;
	stGRDefineArea.stGRTaskRunInfo[ucTaskNo].stNextExeTime=GetTaskNextExeTime(ucTaskNo);
	stGRDefineArea.stGRTaskRunInfo[ucTaskNo].stNextUpTime=GetTaskNextUpTime(ucTaskNo);
	stGRDefineArea.stGRTaskRunInfo[ucTaskNo].usHeadPos=0;
	stGRDefineArea.stGRTaskInfo[ucTaskNo].ulTailTime=TimeConver(&stGRDefineArea.stGRTaskRunInfo[ucTaskNo].stNextExeTime,CON_TIMEUNIT_SEC);
	stGRDefineArea.stGRTask[ucTaskNo].stStartActive=stGRDefineArea.stGRTaskRunInfo[ucTaskNo].stNextExeTime;
	stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem=Task_GetPerTimeSpaceLittle(ucTaskNo);

	ulNum_Task = (MCP_FLASH_BLOCK_SIZE / ((unsigned long)(stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem))) * SECTORNUM_PERTASK;
	stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum = (unsigned short)ulNum_Task;
	ulDestAddr = (MCP_FLASH_TASK_START_BLOCK + ucTaskNo * SECTORNUM_PERTASK) * MCP_FLASH_BLOCK_SIZE;
//	Flash_Eraseblock_MCP(ulDestAddr);
	MCP_Flash_Sector_Erase(ulDestAddr);
	//HdConvertWatchDog();
//azh 任务相关运行参数保存到EE
    Save_Curve_Task_Para(ucTaskNo, CON_TASK_BLK_OFST, CON_TASK_PARA_BLOCK_SAVE);
    Save_Curve_Task_Para(ucTaskNo, CON_TASKIF_BLK_OFST, CON_TASK_PARA_BLOCK_SAVE);
    Save_Curve_Task_Para(ucTaskNo, CON_TASKRIF_BLK_OFST, CON_TASK_PARA_BLOCK_SAVE);	
    
	return ucRet;
}

//清除任务
unsigned char Task_Clear(unsigned char ucTaskNo)
{
	unsigned char	ucRet=RET_ERROR;
	unsigned char	ucTaskType=CON_GRTASK_TYPE_INVALID;
	unsigned long	ulDestAddr;
	if(ucTaskNo>=CON_GRTASK_NUM)
		return RET_ERROR;
	
	ucTaskType=stGRDefineArea.stGRTask[ucTaskNo].ucTaskType;
	if( ucTaskType!=CON_GRTASK_TYPE_INVALID )
	{
		ulDestAddr = (MCP_FLASH_TASK_START_BLOCK + ucTaskNo * SECTORNUM_PERTASK) * MCP_FLASH_BLOCK_SIZE;
//		Flash_Eraseblock_MCP(ulDestAddr);
		MCP_Flash_Sector_Erase(ulDestAddr);
		//HdConvertWatchDog();
		stGRDefineArea.stGRTask[ucTaskNo].ucTaskType		=CON_GRTASK_TYPE_INVALID;		//设置任务为无效
		stGRDefineArea.stGRTask[ucTaskNo].ucActive			=CON_GRTASK_NOACTIVE;			//设置任务运行状态为不活动
		stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum=0;
		stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos=0;
		stGRDefineArea.stGRTaskInfo[ucTaskNo].ulTailTime=0;
//azh 任务相关运行参数保存到EE
        Save_Curve_Task_Para(ucTaskNo, CON_TASK_BLK_OFST, CON_TASK_PARA_BLOCK_SAVE);
        Save_Curve_Task_Para(ucTaskNo, CON_TASKIF_BLK_OFST, CON_TASK_PARA_BLOCK_SAVE);			
	}
	return ucRet;
}

//获取任务单时间点数据长度
unsigned short Task_GetPerTimeSpace(unsigned char ucTaskNo)
{
	unsigned char	i=0;
	unsigned short	usDILabel=0,usPerTaskRetMem=0;
	STGRTASK		*pstGRTask;
	STDIINFO		stDIInfoTmp;
	
	if(ucTaskNo<CON_GRTASK_NUM)
	{
		pstGRTask=&stGRDefineArea.stGRTask[ucTaskNo];
		
		switch(pstGRTask->ucTaskType)
		{
		case CON_GRTASK_TYPE_NORMAL:				//普通任务类型:（数据标识(2)+数据字节数(1)+数据内容(x)）*数据项数(x)
			for(i=0;i<pstGRTask->ucDataItemNum;i++)
			{
//				usDILabel=pstGRTask->ucDataArea[i*2]+(pstGRTask->ucDataArea[i*2+1])*0x100;
				usDILabel = curve_task_di[i];
				stDIInfoTmp=Main_DI_Data_Query_Manage( usDILabel );	//查询数据项
				if( stDIInfoTmp.ucHave	==CON_DI_HAVE && stDIInfoTmp.ucMPType==CON_DI_MPTYPE_NORMAL && 
					stDIInfoTmp.ucType	==CON_DI_TYPE_DATA )	//要求是电表数据
					usPerTaskRetMem+=(3+stDIInfoTmp.ucContentLen);
				else 
					usPerTaskRetMem+=3+1;
			}
			break;
		case CON_GRTASK_TYPE_RELAY:					//中继任务类型:数据标识(2)+数据字节数(1)+数据内容(96)
			usPerTaskRetMem=96+3;
			break;
		default:	
			break;
		}
	}
	
	//	if((usPerTaskRetMem%16)!=0)
	//		usPerTaskRetMem=(usPerTaskRetMem/16+1)*16;		//向16归整,也即是如果<16,则=16
	return usPerTaskRetMem;
}

//获取任务单时间点数据长度
//根据设计，定义数据点不包含时间，可以根据位置和起始时间计算数据点时间
unsigned short Task_GetPerTimeSpaceLittle(unsigned char ucTaskNo)
{
	unsigned char	i=0;
	unsigned short	usDILabel=0,usPerTaskRetMem=0;
	STGRTASK		*pstGRTask;
	STDIINFO		stDIInfoTmp;
	
	if(ucTaskNo<CON_GRTASK_NUM)
	{
		pstGRTask=&stGRDefineArea.stGRTask[ucTaskNo];
		
		switch(pstGRTask->ucTaskType)
		{
		case CON_GRTASK_TYPE_NORMAL:				//普通任务类型:（数据内容(x)
			for(i=0;i<pstGRTask->ucDataItemNum;i++)
			{
//				usDILabel=pstGRTask->ucDataArea[i*2]+(pstGRTask->ucDataArea[i*2+1])*0x100;
                usDILabel = curve_task_di[i];
				stDIInfoTmp=Main_DI_Data_Query_Manage( usDILabel );	//查询数据项
				if( stDIInfoTmp.ucHave	==CON_DI_HAVE && stDIInfoTmp.ucMPType==CON_DI_MPTYPE_NORMAL && 
					stDIInfoTmp.ucType	==CON_DI_TYPE_DATA )	//要求是电表数据
					usPerTaskRetMem+=stDIInfoTmp.ucContentLen;
			}
			break;
		case CON_GRTASK_TYPE_RELAY:					//中继任务类型:数据内容(96)
			usPerTaskRetMem=96;
			break;
		default:	
			break;
		}
	}
	
	return usPerTaskRetMem;
}

//任务回应数据存储
//任务一个时间点的数据完整后数据存入DATAFLASH
unsigned char Task_SaveRetVal(unsigned char ucTaskNo,unsigned char *pucRetData,unsigned short usRetDataLen)
{
	unsigned char	ucRet=RET_ERROR;
	unsigned short	usLen=0;
//	unsigned short	usBeginPage=0,usBeginPos=0;
    unsigned short	uspointnum_perblock=0,ustaskpoint=0;
	unsigned long	ulDataAddr=0,ultemp;
//	unsigned char	ucDownloadSource=RunPara_GetTerAdjPara_DownloadSource();
	
	//如果烧程序，不存储
//	if((ucDownloadSource==CON_TERDP_SOURCE_RS232 || ucDownloadSource==CON_TERDP_SOURCE_REMOTE) && DP_Get_TerDP_State()==CON_TERDP_STATE_DOWNLOAD)
//		return ucRet;
	//如果是停电期间，则不存储
//	if(g_ucNoPowerFlag==CON_NOPOWER_STATUS_HAPPEN)
//		return ucRet;
	usLen=stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem;
	ultemp = MCP_FLASH_BLOCK_SIZE / usLen;
	uspointnum_perblock = (unsigned short)ultemp;
	ustaskpoint = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum % stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum;
//	usLen=stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem;
	if(usRetDataLen>usLen)
		return ucRet;
	
	//计算存储位置，获得任务数据在DATAFALSH中的地址
//	usBeginPage	= CON_DFPAGE_HEAD_DATABLOCK + CON_DFPAGE_DATABLOCK_PAGENUM*ucTaskNo;
//	ulDataAddr	=stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem*(stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum % stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum);
//	usBeginPage	=usBeginPage+(unsigned short)(ulDataAddr/FPAGE_SIZE);
//	usBeginPos	=(unsigned short)(ulDataAddr % FPAGE_SIZE);
	
	ulDataAddr  = (MCP_FLASH_TASK_START_BLOCK + ucTaskNo * SECTORNUM_PERTASK + ustaskpoint / uspointnum_perblock) * MCP_FLASH_BLOCK_SIZE ;
	ulDataAddr += usLen *(ustaskpoint % uspointnum_perblock);
	//ucRet=Flash_SetMainMemory_MCP(ulDataAddr,pucRetData,usLen);
	ucRet = MCP_Flash_Write(ulDataAddr,pucRetData,usLen);
	if(ucRet==NO_ERROR)
		ucRet=RET_SUCCESS;
	
	return ucRet;
}

//任务回应数据查询获取
//注：pucRetData头两字节存放实际点数，后面顺序存放数据特征：点长度固定，点内数据项位置不固定
//注：上层函数应该考虑点数不能超过太多
unsigned char Task_QueryRetVal(unsigned char ucTaskNo,unsigned char *pucRetData,STDATETIME *pstStartTime,unsigned short usPointNum,unsigned char ucResendDataInter,unsigned short *pusOnePointLen)
{
	unsigned char	ucRet=RET_SUCCESS;
	unsigned short	i,usLen;
	unsigned short	usPointNumTemp;
	unsigned short	usOutputPos,usOffset;//azh 160311---
	unsigned short  usBufferMaxPoint;
	unsigned char	*pucBuffer=NULL;
//	unsigned short	usStartPage,usBeginPage,usBeginPos;
	unsigned short	ustasknum_perblock;
	unsigned long	ulDataAddr;
	unsigned long	ulTemp;
//	STGRTASK		*pstGRTask;
	STDATETIME		stTempTime;

	((unsigned short *)pucRetData)[0]=0;			//数据块存放点数，占用前2个字节
	if(ucTaskNo>=CON_GRTASK_NUM)
		return RET_ERROR;
	
//	pstGRTask=&(stGRDefineArea.stGRTask[ucTaskNo]);
	if(ucResendDataInter==0)			//上送频率不能为0，修正为默认值1
		ucResendDataInter=1;
	
	//判断开始时间是否有效，应该不小于数据块最早数据时间
	stTempTime=(*pstStartTime);
	AddTime(&stTempTime,stGRDefineArea.stGRTask[ucTaskNo].stTITaskStart.ucTimeInter,stGRDefineArea.stGRTask[ucTaskNo].stTITaskStart.ucTimeUnit);	//存储时间为数据冻结要求时间+存储延迟时间
	if(stGRDefineArea.stGRTaskInfo[ucTaskNo].ulTailTime>(TimeConver(&stTempTime,CON_TIMEUNIT_SEC)))
		return RET_ERROR;
	
	if(stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem==0)
		usBufferMaxPoint=0xFFFF;
	else
		usBufferMaxPoint=(CON_TASK_DATA_MAXBUFFER-2)/stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem;
	usPointNumTemp=usPointNum;
	if(usPointNum>usBufferMaxPoint)
		usPointNumTemp=usBufferMaxPoint;			//读一次点数受数据缓冲区限制，上层函数可以分批读取
	
	pucBuffer=pucRetData+2;
	usLen=stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem;

	ustasknum_perblock = MCP_FLASH_BLOCK_SIZE / usLen;

	
	memset(pucBuffer,CON_GRTASK_DATA_INVALID,CON_TASK_DATA_MAXBUFFER-2);
	//根据任务数据块索引获得的数据块地址取数据
//	usStartPage = CON_DFPAGE_HEAD_DATABLOCK + CON_DFPAGE_DATABLOCK_PAGENUM*ucTaskNo;
	//根据读取的时间计算相对于DATAFLASH数据块开始的点位置，可能误差一个点，需要修正
	ulTemp=TimeConver(&stTempTime,stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit)
		- TimeConver(&(stGRDefineArea.stGRTask[ucTaskNo].stStartActive), stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit);
	usOutputPos=(unsigned short)(ulTemp/stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeInter);
//azh 160311---
    //目前保存的有效数据点数
	if(stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum >= stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos)
	{
	    usBufferMaxPoint = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum - stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos;
	}
	else
	{
	    usBufferMaxPoint = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum + stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum - stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos;
	}        
    
    if(usOutputPos < usBufferMaxPoint)//azh 160311 读取点还在有效数据范围之内
    {
        usOutputPos = usOutputPos + stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos;
        if(usOutputPos >= stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum)
        {
            usOutputPos -= stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum;//调整所读时间起点
        }    

        if(usOutputPos <= stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum)//设置到最新数据点
        {    
            usBufferMaxPoint = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum;
        }
        else
        {   //azh 为了循环时++好比较溢出,这样只要小于就可以判断有没有溢出，要不然有个数据变小的问题
            usBufferMaxPoint = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum + stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum;
        }    
        usOffset = usOutputPos;
        
    	for(i=0;i<usPointNumTemp;i++)
    	{
//    		if(usOutputPos>=stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum || (pucBuffer+usLen-pucRetData)>CON_TASK_DATA_MAXBUFFER)
    		if(usOutputPos>=usBufferMaxPoint || (pucBuffer+usLen-pucRetData)>CON_TASK_DATA_MAXBUFFER)
    		{
    			ucRet=RET_SUCCESS;
    			break;
    		}
            
    		ulDataAddr = (MCP_FLASH_TASK_START_BLOCK + ucTaskNo * SECTORNUM_PERTASK)*MCP_FLASH_BLOCK_SIZE ;
//    		ulDataAddr 	= ulDataAddr + (usOutputPos / ustasknum_perblock) * MCP_FLASH_BLOCK_SIZE + usLen*(usOutputPos % ustasknum_perblock);
    		ulDataAddr 	= ulDataAddr + (usOffset / ustasknum_perblock) * MCP_FLASH_BLOCK_SIZE + usLen*(usOffset % ustasknum_perblock);

    		if( MCP_Flash_Read(ulDataAddr,pucBuffer,usLen) != NO_ERROR)
    		{
    			ucRet=RET_ERROR;
    			break;
    		}
    		pucBuffer+=usLen;
    		usOutputPos+=ucResendDataInter;//azh可以超出一个循环

    		usOffset = usOutputPos;
    		if(usOffset >= stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum)
    		{    
    		    //azh用于DATAFLASH寻址不能超出一个循环
    		    usOffset = usOutputPos - stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum;
    		}
//azh 160311---    		
    	}
    }
	if(ucRet==RET_SUCCESS)
		((unsigned short *)pucRetData)[0]=i;		//数据块存放点数，占用前2个字节
	
	(*pusOnePointLen)=stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem;//azh 要返回是每个数据点的长度
	return ucRet;
}

unsigned char Task_QueryRetVal_ByDI(unsigned char ucTaskNo,unsigned char *pucRetData,STDATETIME *pstStartTime,unsigned short usPointNum,unsigned char ucResendDataInter,unsigned char *pucDataLen,unsigned short usDI)
{
	unsigned char	ucRet=RET_SUCCESS;
	unsigned short	i,usLen;
	unsigned short	usPointNumTemp,usNumTemp;
	unsigned short	usOutputPos,usOffset;//azh 160311---
	unsigned short  usBufferMaxPoint;
	unsigned char	*pucBuffer=NULL;
//	unsigned short	usStartPage,usBeginPage,usBeginPos;
	unsigned short	ustasknum_perblock;
	unsigned long	ulDataAddr;
	unsigned long	ulTemp;
//	STGRTASK		*pstGRTask;
	STDATETIME		stTempTime,stTempTailTime;
	unsigned char	ucDataLen=0;
	unsigned long	ulVal;
	STDIINFO		stDIInfoTmp;
	
	((unsigned short *)pucRetData)[0]=usPointNum;			//数据块存放点数，占用前2个字节
	if(ucTaskNo>=CON_GRTASK_NUM)
		return RET_ERROR;
	
//	pstGRTask=&(stGRDefineArea.stGRTask[ucTaskNo]);
	if(ucResendDataInter==0)			//上送频率不能为0，修正为默认值1
		ucResendDataInter=1;
	
	if(stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem==0)
		usBufferMaxPoint=0xFFFF;
	else
//		usBufferMaxPoint=(CON_TASK_DATA_MAXBUFFER-2-stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem)/16;		//16为估计 
//azh 140918  因为传进来的内存块缓冲是1536的，曲线任务数据会借用一下缓冲，虽然任务数据最大不超过512，但不影响返回96点曲线数据
        usBufferMaxPoint=(1024-2-stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem)/8;//azh 140906//16;		//16为估计
	usPointNumTemp=usPointNum;
	if(usPointNum>usBufferMaxPoint)
		usPointNumTemp=usBufferMaxPoint;			//读一次点数受数据缓冲区限制，上层函数可以分批读取
	
	pucBuffer=pucRetData+2;
	usLen=stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem;
	ustasknum_perblock = MCP_FLASH_BLOCK_SIZE / usLen;
	memset(pucBuffer,CON_GRTASK_DATA_INVALID,CON_TASK_DATA_MAXBUFFER-2);

	stDIInfoTmp=Main_DI_Data_Query_Manage(usDI);
	ucDataLen=stDIInfoTmp.ucContentLen;
	//判断开始时间是否有效，应该不小于数据块最早数据时间
	stTempTime=(*pstStartTime);
//	AddTime(&stTempTime,stGRDefineArea.stGRTask[ucTaskNo].stTITaskStart.ucTimeInter,stGRDefineArea.stGRTask[ucTaskNo].stTITaskStart.ucTimeUnit);	//存储时间为数据冻结要求时间+存储延迟时间100330
	stTempTailTime=TimeConverToDataTime(stGRDefineArea.stGRTaskInfo[ucTaskNo].ulTailTime);
	if(stGRDefineArea.stGRTaskInfo[ucTaskNo].ulTailTime>(TimeConver(&stTempTime,CON_TIMEUNIT_SEC)))
	{
		usNumTemp=Get_CurvePointNum_ByTime(&stTempTime,&stTempTailTime,ucResendDataInter);
		if(usPointNumTemp<=usNumTemp)
		{
			pucBuffer+=(usPointNumTemp*ucDataLen);
			usPointNumTemp=0;											//点数为0，则将不读取DATAFLASH
		}else
		{
			pucBuffer+=(usNumTemp*ucDataLen);								//填充无效数据
			usPointNumTemp-=usNumTemp;
		}
		stTempTime=stTempTailTime;										//修正时间
	}
	//根据任务数据块索引获得的数据块地址取数据
//	usStartPage = CON_DFPAGE_HEAD_DATABLOCK + CON_DFPAGE_DATABLOCK_PAGENUM*ucTaskNo;
	//根据读取的时间计算相对于DATAFLASH数据块开始的点位置，可能误差一个点，需要修正
	ulTemp=TimeConver(&stTempTime,stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit)
		- TimeConver(&(stGRDefineArea.stGRTask[ucTaskNo].stStartActive), stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit);
	usOutputPos=(unsigned short)(ulTemp/stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeInter);
//azh 160311---
    //目前保存的有效数据点数
	if(stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum >= stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos)
	{
	    usBufferMaxPoint = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum - stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos;
	}
	else
	{
	    usBufferMaxPoint = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum + stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum - stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos;
	}        
    
    if(usOutputPos < usBufferMaxPoint)//azh 160311 读取点还在有效数据范围之内
    {
        usOutputPos = usOutputPos + stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos;
        if(usOutputPos >= stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum)
        {
            usOutputPos -= stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum;//调整所读时间起点
        }    

        if(usOutputPos <= stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum)//usBufferMaxPoint设置到最新数据点
        {    
            usBufferMaxPoint = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum;
        }
        else
        {   //azh 为了循环时++好比较溢出,这样只要小于就可以判断有没有溢出，要不然有个数据变小的问题
            usBufferMaxPoint = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum + stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum;
        }    
        usOffset = usOutputPos;

    	for(i=0;i<usPointNumTemp;i++)
    	{
//    		if(usOutputPos>=stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum || (pucBuffer+usLen-pucRetData)>CON_TASK_DATA_MAXBUFFER)
    		if(usOutputPos>=usBufferMaxPoint || (pucBuffer+usLen-pucRetData)>CON_TASK_DATA_MAXBUFFER)
    		{
    			ucRet=RET_SUCCESS;
    			break;
    		}
    		ulDataAddr = (MCP_FLASH_TASK_START_BLOCK + ucTaskNo * SECTORNUM_PERTASK)*MCP_FLASH_BLOCK_SIZE ;
//    		ulDataAddr 	= ulDataAddr + (usOutputPos / ustasknum_perblock) * MCP_FLASH_BLOCK_SIZE + usLen*(usOutputPos % ustasknum_perblock);
            ulDataAddr 	= ulDataAddr + (usOffset / ustasknum_perblock) * MCP_FLASH_BLOCK_SIZE + usLen*(usOffset % ustasknum_perblock);

            if( MCP_Flash_Read(ulDataAddr,pucBuffer,usLen) != NO_ERROR)
    		{
    			ucRet=RET_ERROR;
    			break;
    		}
    		Get_TaskData((unsigned char *)&ulVal,pucBuffer,ucTaskNo,usDI);
    		memcpy(pucBuffer,(unsigned char *)&ulVal,ucDataLen);
    		pucBuffer+=ucDataLen;
    		usOutputPos+=ucResendDataInter;
    		memset(pucBuffer,CON_GRTASK_DATA_INVALID,usLen);

    		usOffset = usOutputPos;
    		if(usOffset >= stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum)
    		{    
    		    //azh用于DATAFLASH寻址不能超出一个循环
    		    usOffset = usOutputPos - stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum;
    		}
    	}
    }
//azh 160311---	
	(*pucDataLen)=ucDataLen;//azh 要返回是每个数据点数据项的长度
	return ucRet;
}
//azh 170925 对应376.1-2013版的二类数据F219:测量点组合数据曲线数据
unsigned char Task_QueryRetVal_ByGroupDI(unsigned char ucTaskNo,unsigned char *pucRetData,STDATETIME *pstStartTime,unsigned short usPointNum,unsigned char ucResendDataInter,unsigned char *pucDataLen,unsigned short usDI)
{
	unsigned char	ucRet=RET_SUCCESS,mem_type;
	unsigned short	i,usLen;
	unsigned short	usPointNumTemp,usNumTemp;
	unsigned short	usOutputPos,usOffset;//azh 160311---
	unsigned short  usBufferMaxPoint;
	unsigned char	*pucBuffer=NULL,*ptr_tmp;
//	unsigned short	usStartPage,usBeginPage,usBeginPos;
	unsigned short	ustasknum_perblock;
	unsigned long	ulDataAddr;
	unsigned long	ulTemp;
//	STGRTASK		*pstGRTask;
	STDATETIME		stTempTime,stTempTailTime;
	unsigned char	ucDataLen=0;
//	unsigned long	ulVal;
//	STDIINFO		stDIInfoTmp;
	
	((unsigned short *)pucRetData)[0]=usPointNum;			//数据块存放点数，占用前2个字节
	if(ucTaskNo>=CON_GRTASK_NUM)
		return RET_ERROR;
	
//	pstGRTask=&(stGRDefineArea.stGRTask[ucTaskNo]);
	if(ucResendDataInter==0)			//上送频率不能为0，修正为默认值1
		ucResendDataInter=1;
	
	if(stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem==0)
		usBufferMaxPoint=0xFFFF;
	else
//		usBufferMaxPoint=(CON_TASK_DATA_MAXBUFFER-2-stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem)/16;		//16为估计 
//azh 140906  因为传进来的内存块缓冲是1536的，曲线任务数据会借用一下缓冲，虽然任务数据最大不超过512，但不影响返回96点曲线数据
        usBufferMaxPoint=(1024-2-stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem)/8;//azh 140906//16;		//16为估计
	usPointNumTemp=usPointNum;
	if(usPointNum>usBufferMaxPoint)
		usPointNumTemp=usBufferMaxPoint;			//读一次点数受数据缓冲区限制，上层函数可以分批读取
	
	pucBuffer=pucRetData+2;
	usLen=stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem;
	ustasknum_perblock = MCP_FLASH_BLOCK_SIZE / usLen;
	memset(pucBuffer,CON_GRTASK_DATA_INVALID,CON_TASK_DATA_MAXBUFFER-2);

//	stDIInfoTmp=Main_DI_Data_Query_Manage(usDI);
//	ucDataLen=stDIInfoTmp.ucContentLen;
    ucDataLen = 32;//azh 170925 二类数据F219专用，所又这里长度写死，上行时实际格式转换后无功功率是3B还多一个字节

	//判断开始时间是否有效，应该不小于数据块最早数据时间
	stTempTime=(*pstStartTime);
//	AddTime(&stTempTime,stGRDefineArea.stGRTask[ucTaskNo].stTITaskStart.ucTimeInter,stGRDefineArea.stGRTask[ucTaskNo].stTITaskStart.ucTimeUnit);	//存储时间为数据冻结要求时间+存储延迟时间100330
	stTempTailTime=TimeConverToDataTime(stGRDefineArea.stGRTaskInfo[ucTaskNo].ulTailTime);
	if(stGRDefineArea.stGRTaskInfo[ucTaskNo].ulTailTime>(TimeConver(&stTempTime,CON_TIMEUNIT_SEC)))
	{
		usNumTemp=Get_CurvePointNum_ByTime(&stTempTime,&stTempTailTime,ucResendDataInter);
		if(usPointNumTemp<=usNumTemp)
		{
			pucBuffer+=(usPointNumTemp*ucDataLen);
			usPointNumTemp=0;											//点数为0，则将不读取DATAFLASH
		}else
		{
			pucBuffer+=(usNumTemp*ucDataLen);								//填充无效数据
			usPointNumTemp-=usNumTemp;
		}
		stTempTime=stTempTailTime;										//修正时间
	}
	//根据任务数据块索引获得的数据块地址取数据
//	usStartPage = CON_DFPAGE_HEAD_DATABLOCK + CON_DFPAGE_DATABLOCK_PAGENUM*ucTaskNo;
	//根据读取的时间计算相对于DATAFLASH数据块开始的点位置，可能误差一个点，需要修正
	ulTemp=TimeConver(&stTempTime,stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit)
		- TimeConver(&(stGRDefineArea.stGRTask[ucTaskNo].stStartActive), stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit);
	usOutputPos=(unsigned short)(ulTemp/stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeInter);
//azh 160311---
    //目前保存的有效数据点数
	if(stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum >= stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos)
	{
	    usBufferMaxPoint = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum - stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos;
	}
	else
	{
	    usBufferMaxPoint = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum + stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum - stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos;
	}        
    
    if(usOutputPos < usBufferMaxPoint)//azh 160311 读取点还在有效数据范围之内
    {
        usOutputPos = usOutputPos + stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos;
        if(usOutputPos >= stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum)
        {
            usOutputPos -= stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum;//调整所读时间起点
        }    

        if(usOutputPos <= stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum)//usBufferMaxPoint设置到最新数据点
        {    
            usBufferMaxPoint = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum;
        }
        else
        {   //azh 为了循环时++好比较溢出,这样只要小于就可以判断有没有溢出，要不然有个数据变小的问题
            usBufferMaxPoint = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum + stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum;
        }    
        usOffset = usOutputPos;

//azh 170925
        ptr_tmp = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR0);
        if(ptr_tmp == NULL)
        {
            return RET_ERROR;
        }

    	for(i=0;i<usPointNumTemp;i++)
    	{
//    		if(usOutputPos>=stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum || (pucBuffer+usLen-pucRetData)>CON_TASK_DATA_MAXBUFFER)
    		if(usOutputPos>=usBufferMaxPoint || (pucBuffer+usLen-pucRetData)>CON_TASK_DATA_MAXBUFFER)
    		{
    			ucRet=RET_SUCCESS;
    			break;
    		}
    		ulDataAddr = (MCP_FLASH_TASK_START_BLOCK + ucTaskNo * SECTORNUM_PERTASK)*MCP_FLASH_BLOCK_SIZE ;
//    		ulDataAddr 	= ulDataAddr + (usOutputPos / ustasknum_perblock) * MCP_FLASH_BLOCK_SIZE + usLen*(usOutputPos % ustasknum_perblock);
            ulDataAddr 	= ulDataAddr + (usOffset / ustasknum_perblock) * MCP_FLASH_BLOCK_SIZE + usLen*(usOffset % ustasknum_perblock);

            if( MCP_Flash_Read(ulDataAddr,pucBuffer,usLen) != NO_ERROR)
    		{
    			ucRet=RET_ERROR;
    			break;
    		}
//azh--170925
//    		Get_TaskData((unsigned char *)&ulVal,pucBuffer,ucTaskNo,usDI);
    		//curve_task_di[]根据曲线数据表格直接取出数据
            memcpy(ptr_tmp, pucBuffer, usLen);
    		memcpy(pucBuffer, ptr_tmp+32, 3);//CON_DI_TYPE_MPINSTANTPOW,		//瞬时有功功率3B
    		pucBuffer+=3;
    		memcpy(pucBuffer, ptr_tmp+44, 2);//CON_DI_TYPE_MPINSTANTVAR,		//瞬时无功功率2B
    		pucBuffer+=2;
    		memcpy(pucBuffer, ptr_tmp+52, 6);//CON_DI_TYPE_MPVOLA,				//ABC相电压数据块6B
    		pucBuffer+=6;
    		memcpy(pucBuffer, ptr_tmp+58, 9);//CON_DI_TYPE_MPVOLA,				//ABC相电流数据块9B
    		pucBuffer+=9;
    		memcpy(pucBuffer, ptr_tmp, 4);//CON_DI_TYPE_MPDIRPOWTOTAL,		//正向有功总电能4B
    		pucBuffer+=4;
    		memcpy(pucBuffer, ptr_tmp+16, 4);//CON_DI_TYPE_MPVAR1,				//一象限无功总电能4B
    		pucBuffer+=4;
    		memcpy(pucBuffer, ptr_tmp+20, 4);//CON_DI_TYPE_MPVAR4,				//四象限无功总电能4B
    		pucBuffer+=4;
//--
    		usOutputPos+=ucResendDataInter;
    		memset(pucBuffer,CON_GRTASK_DATA_INVALID,usLen);

    		usOffset = usOutputPos;
    		if(usOffset >= stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum)
    		{    
    		    //azh用于DATAFLASH寻址不能超出一个循环
    		    usOffset = usOutputPos - stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum;
    		}
    	}
//azh 170925
        free_mem_block(ptr_tmp);//MEM_256_TYPE
    	
    }
//azh 160311---	
	(*pucDataLen)=ucDataLen;//azh 要返回是每个数据点数据项的长度
	return ucRet;
}


//任务回应数据查询获取，获取最近几点
//注：pucRetData头两字节存放实际点数，后面顺序存放数据特征：点长度固定，点内数据项位置不固定
//unsigned char Task_QueryRetVal_Newest(unsigned char ucTaskNo,unsigned char *pucRetData,unsigned short usPointNum,unsigned char ucResendDataInter,STDATETIME *pstNewestTime,unsigned short *pusOnePointLen)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned short i;
//	unsigned short usPointNumTemp,usLen;
//	unsigned short usBufferMaxPoint;
//	unsigned short usOutputPos;
//	unsigned char *pucBuffer;
////	unsigned short usStartPage,usBeginPage,usBeginPos;
//	unsigned long ulDataAddr;
////	STGRTASK *pstGRTask;
//	
//	((unsigned short *)pucRetData)[0]=0;			//数据块存放点数，占用前2个字节
//	if(ucTaskNo>=CON_GRTASK_NUM)
//		return RET_ERROR;
////	pstGRTask=&(stGRDefineArea.stGRTask[ucTaskNo]);
//	if(ucResendDataInter==0)						//上送频率不能为0，修正为默认值1
//		ucResendDataInter=1;
//	//判断长度是否有效，应该小于开始时间到最新数据点时间之间的数据点数/点间隔。
//	if(	((usPointNum*ucResendDataInter)>stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum)|| 
//		((usPointNum*ucResendDataInter)>stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum))
//		return RET_ERROR;
//	
//	if(stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem==0)
//		usBufferMaxPoint=0xFFFF;
//	else
//		usBufferMaxPoint=(CON_TASK_DATA_MAXBUFFER-2)/stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem;
//	usPointNumTemp=usPointNum;
//	if(usPointNum>usBufferMaxPoint)
//		usPointNumTemp=usBufferMaxPoint;			//读一次点数受数据缓冲区限制，上层函数可以分批读取
//	
//	pucBuffer=pucRetData+2;
//	usLen=stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem;
//	memset(pucBuffer,CON_GRTASK_DATA_INVALID,CON_TASK_DATA_MAXBUFFER-2);
////	usStartPage=CON_DFPAGE_HEAD_DATABLOCK + CON_DFPAGE_DATABLOCK_PAGENUM*ucTaskNo;
//	usOutputPos=stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum-usPointNumTemp*ucResendDataInter+ucResendDataInter-1;	//usPointNumTemp,保证最新一点被读取，usPointNum无法保证
//	//不按照批读取，按照点来读取，读取速度也有保证，对于间隔读取也容易实现
////	for(i=0;i<usPointNumTemp;i++)
////	{
////		if(usOutputPos>=stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum || (pucBuffer+usLen-pucRetData)>CON_TASK_DATA_MAXBUFFER)
////		{
////			ucRet=RET_SUCCESS;
////			break;
////		}
////		ulDataAddr=usLen*(usOutputPos % stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum);
////		usBeginPage=usStartPage+(unsigned short)(ulDataAddr/FPAGE_SIZE);
////		usBeginPos=(unsigned short)(ulDataAddr % FPAGE_SIZE);
////		if(Flash_GetMainMemory(usBeginPage,usBeginPos,usLen,pucBuffer)==EF_FLASH)
////		{
////			ucRet=RET_ERROR;
////			break;
////		}
////		pucBuffer+=usLen;
////		usOutputPos+=ucResendDataInter;
////	}
//	if(ucRet==RET_SUCCESS)
//	{
//		((unsigned short *)pucRetData)[0]=i;		//数据块存放点数，占用前2个字节
//		//计算最新点数据点时间
//		*pstNewestTime=stGRDefineArea.stGRTask[ucTaskNo].stStartActive;		//开始时间 加 已经执行的时间
//		AddTime(pstNewestTime,
//			(stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum-1) * stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeInter,
//			stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit);
//		(*pusOnePointLen)=stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem;
//	}
//	return ucRet;
//}

//根据执行时间间隔和上送时间间隔以及上送的选择频率获取任务上送的点数(主动上送)
//上送间隔为月的任务，不处理连续上送的情况（也即是，采样间隔为日，上送间隔为月的情况，不予处理），只能处理月、月的情况
//unsigned short Task_GetUpSendPointNum(unsigned char ucTaskNo)
//{
//	unsigned short	usPointNum=0;
//	unsigned char	ucResendDataInter;
//	unsigned long	ulTmp1,ulTmp2;
//	
//	if(ucTaskNo>=CON_GRTASK_NUM)
//		return usPointNum;
//	if( stGRDefineArea.stGRTask[ucTaskNo].stTIReSendInter.ucTimeInter==0)
//		return usPointNum;
//	
//	ucResendDataInter=stGRDefineArea.stGRTask[ucTaskNo].ucResendDataInter;
//	if(ucResendDataInter==0)
//		ucResendDataInter=1;
//	if(	stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit	==	stGRDefineArea.stGRTask[ucTaskNo].stTIReSendInter.ucTimeUnit)
//		usPointNum	=	stGRDefineArea.stGRTask[ucTaskNo].stTIReSendInter.ucTimeInter / stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeInter;
//	else
//	{
//		ulTmp1	=	TimeInfoConver(stGRDefineArea.stGRTask[ucTaskNo].stTIReSendInter.ucTimeInter,stGRDefineArea.stGRTask[ucTaskNo].stTIReSendInter.ucTimeUnit);
//		ulTmp2	=	TimeInfoConver(stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeInter,stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit);
//		usPointNum	=	(unsigned short)(ulTmp1/ulTmp2);
//	}
//	
//	usPointNum=usPointNum/ucResendDataInter;
//	if(usPointNum==0)
//		usPointNum=1;		//至少上送一点
//	
//	return usPointNum;
//}

//获得执行时间
STDATETIME GetTaskNextExeTime(unsigned char ucTaskNo)
{
	STDATETIME stNextExeTime;
	unsigned long ulCurrTime,ulNextExeTime,ulTemp;
	stNextExeTime=GetCurrentTime();
	if(Is_DateTime_Availble(&stNextExeTime)!=RET_SUCCESS)
	{    
		stNextExeTime.ucYear = 0x0c;//azh 由于有的时候上电还没有同步到时间所以第一次跟一个初始值
		stNextExeTime.ucMonth = 0x0c;
		stNextExeTime.ucDay = 0x01;
		return stNextExeTime;
	}
	ulCurrTime=TimeConver(&stNextExeTime,CON_TIMEUNIT_SEC);		//获得当前时间
	
	stNextExeTime=Task_FixNextTime(stNextExeTime,	stGRDefineArea.stGRTask[ucTaskNo].stTITaskStart,
		stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter);
	
	ulNextExeTime=TimeConver(&stNextExeTime,CON_TIMEUNIT_SEC);
	if(stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeInter==0)
		return stNextExeTime;
	ulTemp=ulNextExeTime;
	while((ulCurrTime+20)>=ulNextExeTime)
	{
		//调整下次执行时间为至少当前时间20秒之后
		AddTime(&stNextExeTime,
			(unsigned long)stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeInter,
			stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit);
		ulNextExeTime=TimeConver(&stNextExeTime,CON_TIMEUNIT_SEC);		//获得当前时间
		if(ulTemp==ulNextExeTime)
			break;
		else
			ulTemp=ulNextExeTime;
	}
	return stNextExeTime;
}

//获得上送时间
STDATETIME GetTaskNextUpTime(unsigned char ucTaskNo)
{
	STDATETIME stNextUpTime;
	unsigned long ulCurrTime,ulNextUpTime,ulTemp;
	stNextUpTime=GetCurrentTime();
	if(Is_DateTime_Availble(&stNextUpTime)!=RET_SUCCESS)
		return stNextUpTime;
	ulCurrTime=TimeConver(&stNextUpTime,CON_TIMEUNIT_SEC);		//获得当前时间
	
	stNextUpTime=Task_FixNextTime(stNextUpTime,	stGRDefineArea.stGRTask[ucTaskNo].stTIResendStart,
		stGRDefineArea.stGRTask[ucTaskNo].stTIReSendInter);
	
	ulNextUpTime=TimeConver(&stNextUpTime,CON_TIMEUNIT_SEC);		
	if(stGRDefineArea.stGRTask[ucTaskNo].stTIReSendInter.ucTimeInter!=0)
	{
		ulTemp=ulNextUpTime;
		while((ulCurrTime+20)>=ulNextUpTime)
		{//调整下次上送时间为至少当前时间20秒之后
			AddTime(&stNextUpTime,
				(unsigned long)stGRDefineArea.stGRTask[ucTaskNo].stTIReSendInter.ucTimeInter,
				stGRDefineArea.stGRTask[ucTaskNo].stTIReSendInter.ucTimeUnit);
			ulNextUpTime=TimeConver(&stNextUpTime,CON_TIMEUNIT_SEC);		//获得当前时间
			if(ulTemp==ulNextUpTime)
				break;
			else
				ulTemp=ulNextUpTime;
		}
	}
	return stNextUpTime;
}

STDATETIME	Task_FixNextTime(STDATETIME stDateTime,STTIMEINFO stStartTimeInfo,STTIMEINFO stInterTimeInfo)
{
	if( stStartTimeInfo.ucTimeUnit==CON_TIMEUNIT_MIN )
	{//分
		stDateTime.ucMinute	=stStartTimeInfo.ucTimeInter;
		if( stInterTimeInfo.ucTimeUnit==CON_TIMEUNIT_DAY )
		{//上送间隔单位为日
			stDateTime.ucHour	=0;
		}
		else if( stInterTimeInfo.ucTimeUnit==CON_TIMEUNIT_MON )
		{//上送间隔单位为月
			stDateTime.ucDay	=1;
			stDateTime.ucHour	=0;
		}
	}
	else if( stStartTimeInfo.ucTimeUnit==CON_TIMEUNIT_HOUR )
	{//时
		stDateTime.ucHour	=stStartTimeInfo.ucTimeInter;
		stDateTime.ucMinute	=0;
		if( stInterTimeInfo.ucTimeUnit==CON_TIMEUNIT_MON )
		{//上送间隔单位为月
			stDateTime.ucDay=1;
		}
	}
	else if( stStartTimeInfo.ucTimeUnit==CON_TIMEUNIT_DAY )
	{//天
		stDateTime.ucDay	=stStartTimeInfo.ucTimeInter;
		stDateTime.ucHour	=0;
		stDateTime.ucMinute	=0;
	}
	else if( stStartTimeInfo.ucTimeUnit==CON_TIMEUNIT_MON )
	{//月
		stDateTime.ucMonth	=stStartTimeInfo.ucTimeInter;
		stDateTime.ucDay	=1;
		stDateTime.ucHour	=0;
		stDateTime.ucMinute	=0;
	}
	stDateTime.ucSecond		=0;
	return stDateTime;
}

/****************************************从任务中取值**********************************************/
unsigned char Get_TaskData(unsigned char *pucDes,unsigned char *pucSrc,unsigned char ucTaskNo,unsigned short usDI)
{
	unsigned char ucDataLen=0;
	unsigned char i;
	unsigned short usPos=0;
	unsigned short usDITemp;
	STDIINFO stDIInfoTmp;
	
	if(ucTaskNo>=CON_GRTASK_NUM)
		return ucDataLen;
	for(i=0;i<stGRDefineArea.stGRTask[ucTaskNo].ucDataItemNum;i++)
	{
//		usDITemp=stGRDefineArea.stGRTask[ucTaskNo].ucDataArea[2*i]+0x100*stGRDefineArea.stGRTask[ucTaskNo].ucDataArea[2*i+1];
        usDITemp = curve_task_di[i];
		memset(&stDIInfoTmp,0,sizeof(STDIINFO));
		stDIInfoTmp=Main_DI_Data_Query_Manage(usDITemp);			//查询
		if(usDITemp==usDI)
		{
			ucDataLen=stDIInfoTmp.ucContentLen;	
			memcpy(pucDes,pucSrc+usPos,ucDataLen);
			break;
		}
		usPos+=stDIInfoTmp.ucContentLen;
	}
	if(ucDataLen==0)
	{
		stDIInfoTmp=Main_DI_Data_Query_Manage(usDI);
		ucDataLen=stDIInfoTmp.ucContentLen;
		memset(pucDes,CON_GRTASK_DATA_INVALID,ucDataLen);
	}
	if(pucDes[0]==CON_GRTASK_DATA_INVALID || pucDes[0]==CON_DATAFLSH_INIT_DATA)
		memset(pucDes,CON_GRTASK_DATA_INVALID,ucDataLen);
	
	return ucDataLen;
}

unsigned char Convert_Hex_To_TaskData(unsigned short usDI_Sml,unsigned char *pucSrc,unsigned char *pucDes)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long ulTempSrc,ulTempDes;
	long lVal=0;
	STDIINFO stDIInfoTmp;
	stDIInfoTmp=Main_DI_Data_Query_Manage(usDI_Sml);			//查询
	if(stDIInfoTmp.ucHave==CON_DI_NOHAVE)
	{
		pucDes[0]=0xEE;
		return RET_ERROR;
	}
	switch(stDIInfoTmp.usBigDI)
	{
	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_BLOCK:				//需量发生时间
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_BLOCK:
	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_BLOCK:
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_BLOCK:
		pucDes[0]=(unsigned char)HEX_TO_BCD(pucSrc[4],1);
		pucDes[1]=(unsigned char)HEX_TO_BCD(pucSrc[3],1);
		pucDes[2]=(unsigned char)HEX_TO_BCD(pucSrc[2],1);
		pucDes[3]=(unsigned char)HEX_TO_BCD(pucSrc[1],1);
		break;
	case CON_DI_TYPE_MPFEERATE_BLOCK:							//费率	
		memcpy(pucDes,pucSrc,stDIInfoTmp.ucContentLen);
		break;
	default:													//数字
		if(pucSrc[0]==0xBB)
			pucDes[0]=0xEE;
		else
		{
			memcpy((unsigned char *)&lVal,pucSrc,4);
			ulTempSrc=makelabs(lVal);
			ulTempSrc=pucSrc[0]+0x100*pucSrc[1]+0x10000*pucSrc[2]+0x1000000*pucSrc[3];
			ulTempDes=HEX_TO_BCD(ulTempSrc,stDIInfoTmp.ucContentLen);
			memcpy(pucDes,&ulTempDes,stDIInfoTmp.ucContentLen);
		}
	}
	
	return ucRet;
}

//检查表配置任务是否有效
unsigned char TaskCheckMeterIsValid(unsigned char ucMeterNo)
{
	unsigned char ucRet=RET_SUCCESS;
	if(stGRDefineArea.stGRTask[ucMeterNo].ucActive==CON_GRTASK_NOACTIVE)
		return RET_ERROR;
	if(Task_Check_Task_BaseInfo(ucMeterNo)==RET_ERROR)
		ucRet=RET_ERROR;
	return ucRet;
}

//表8块，抄所有表数据，分别占用1个任务
unsigned char DeleteDefaultMeterTask(unsigned char ucMeterNo)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucTaskNo;
	ucTaskNo=ucMeterNo;
	Task_Clear(ucTaskNo);
	return ucRet;
}

//生成默认表任务//azh 曲线任务 保存到FLASH
unsigned char SetDefaultMeterTask(unsigned char ucMeterNo)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucTaskNo;
//	unsigned char m;
	STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
	STGRTASK *pstGRTaskTmp;

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pstGRTaskTmp = (STGRTASK *)mem_ptr; 
	memset(pstGRTaskTmp,0,sizeof(STGRTASK));
	pstGRTaskTmp->ucTaskType=CON_GRTASK_TYPE_NORMAL;
	pstGRTaskTmp->ucMPNo=RunPara_GetMeter_MPNo(ucMeterNo);
/*	if(stTerAdjPara.ucRunMode==0xA5)			//测试模式
		pstGRTaskTmp->stTITaskStart.ucTimeInter=2;    //注意数据时间点
	else
		pstGRTaskTmp->stTITaskStart.ucTimeInter=5;    //正常模式*/
	
	pstGRTaskTmp->stTITaskStart.ucTimeInter=0;
	pstGRTaskTmp->stTITaskStart.ucTimeUnit=CON_TIMEUNIT_MIN;
	pstGRTaskTmp->stTITaskInter.ucTimeInter=15;
	pstGRTaskTmp->stTITaskInter.ucTimeUnit=CON_TIMEUNIT_MIN;
	pstGRTaskTmp->stTIResendStart.ucTimeInter=1;					
	pstGRTaskTmp->stTIResendStart.ucTimeUnit=CON_TIMEUNIT_MIN;
	pstGRTaskTmp->stTIReSendInter.ucTimeInter=0;					//0表示不主动上送
	pstGRTaskTmp->stTIReSendInter.ucTimeUnit=0;
	pstGRTaskTmp->ucResendDataInter=1;							//上送数据的频率		
	pstGRTaskTmp->usPerTaskRetMem=0;								//单个任务需要的内存数
	pstGRTaskTmp->usSavePointNum=CON_GRTASK_DATAPOINT_NUM;		//任务的保存点数15分钟一点
	pstGRTaskTmp->usExeNum=0;
	//	pstGRTaskTmp->stRelayTaskPara;								//中继参数
	
// 	m=0;
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPDIRPOWTOTAL;		//正向有功总电能
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPDIRPOWTOTAL>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPDIRVARTOTAL;		//正向无功总电能
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPDIRVARTOTAL>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPUNDIRPOWTOTAL;		//反向有功总电能
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPUNDIRPOWTOTAL>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPUNDIRVARTOTAL;		//反向无功总电能
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPUNDIRVARTOTAL>>8);
// 	
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVAR1;				//一象限无功总电能
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVAR1>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVAR4;				//四象限无功总电能
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVAR4>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVAR2;				//二象限无功总电能
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVAR2>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVAR3;				//三象限无功总电能
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVAR3>>8);
// 	
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTPOW;		//瞬时有功功率
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTPOW>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTPOWA;		//A相瞬时有功功率
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTPOWA>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTPOWB;		//B相瞬时有功功率
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTPOWB>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTPOWC;		//C相瞬时有功功率
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTPOWC>>8);
// 	
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTVAR;		//瞬时无功功率
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTVAR>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTVARA;		//A相瞬时无功功率
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTVARA>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTVARB;		//B相瞬时无功功率
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTVARB>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTVARC;		//C相瞬时无功功率
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTVARC>>8);
// 	
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVOLA;				//A相电压
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVOLA>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVOLB;				//B相电压
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVOLB>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVOLC;				//C相电压
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVOLC>>8);
// 	
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPCURA;				//A相电流
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPCURA>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPCURB;				//B相电流
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPCURB>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPCURC;				//C相电流
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPCURC>>8);
	
	pstGRTaskTmp->ucDataItemNum=CON_TASK_CURVE_DI_NUM;//m>>1;								//普通任务的数据项数量
	ucTaskNo=ucMeterNo;
    if(ucTaskNo < CON_GRTASK_NUM)//azh 130127
    {
    	memcpy(&stGRDefineArea.stGRTask[ucTaskNo],pstGRTaskTmp,sizeof(STGRTASK));
    	Save_Curve_Task_Para(ucTaskNo, CON_TASK_BLK_OFST, CON_TASK_PARA_BLOCK_SAVE);//AZH
    
    	Task_Config(ucTaskNo,CON_GRTASK_ACTIVE);
    }
	
    free_mem_block(mem_ptr);//
	return ucRet;
}

//检查总加组任务是否有效
//unsigned char TaskCheckSumGroupIsValid(void)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	if(stGRDefineArea.stGRTask[CON_GRTASK_SUMGROUP_INSTANTPOW_TASKNO].ucActive==CON_GRTASK_NOACTIVE)
//		return RET_ERROR;
//	if(Task_Check_Task_BaseInfo(CON_GRTASK_SUMGROUP_INSTANTPOW_TASKNO)==RET_ERROR)
//		return RET_ERROR;
//	if(stGRDefineArea.stGRTask[CON_GRTASK_SUMGROUP_INSTANTVAR_TASKNO].ucActive==CON_GRTASK_NOACTIVE)
//		return RET_ERROR;
//	if(Task_Check_Task_BaseInfo(CON_GRTASK_SUMGROUP_INSTANTVAR_TASKNO)==RET_ERROR)
//		return RET_ERROR;
//	if(stGRDefineArea.stGRTask[CON_GRTASK_SUMGROUP_POW_TASKNO].ucActive==CON_GRTASK_NOACTIVE)
//		return RET_ERROR;
//	if(Task_Check_Task_BaseInfo(CON_GRTASK_SUMGROUP_POW_TASKNO)==RET_ERROR)
//		return RET_ERROR;
//	if(stGRDefineArea.stGRTask[CON_GRTASK_SUMGROUP_VAR_TASKNO].ucActive==CON_GRTASK_NOACTIVE)
//		return RET_ERROR;
//	if(Task_Check_Task_BaseInfo(CON_GRTASK_SUMGROUP_VAR_TASKNO)==RET_ERROR)
//		return RET_ERROR;
//	return ucRet;
//}

//生成总加默认任务
//unsigned char SetDefaultSumGroupTask(void)
//{
//	unsigned char ucRet=RET_SUCCESS;
//azh	unsigned char ucTaskNo;
//	unsigned char m;
//	STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
//
//	memset(&stGRTaskTmp,0,sizeof(STGRTASK));
//	stGRTaskTmp.ucTaskType=CON_GRTASK_TYPE_NORMAL;
//	stGRTaskTmp.ucMPNo=CON_MPNO_SUMG_FIXED;
///*	if(stTerAdjPara.ucRunMode==0xA5)				//测试模式
//		stGRTaskTmp.stTITaskStart.ucTimeInter=2;
//	else
//		stGRTaskTmp.stTITaskStart.ucTimeInter=5;	//正常模式*/
//	stGRTaskTmp.stTITaskStart.ucTimeInter=0;			//100330
//	stGRTaskTmp.stTITaskStart.ucTimeUnit=CON_TIMEUNIT_MIN;
//	stGRTaskTmp.stTITaskInter.ucTimeInter=15;
//	stGRTaskTmp.stTITaskInter.ucTimeUnit=CON_TIMEUNIT_MIN;
//	stGRTaskTmp.stTIResendStart.ucTimeInter=1;					
//	stGRTaskTmp.stTIResendStart.ucTimeUnit=CON_TIMEUNIT_MIN;
//	stGRTaskTmp.stTIReSendInter.ucTimeInter=0;					//0表示不主动上送
//	stGRTaskTmp.stTIReSendInter.ucTimeUnit=0;
//	stGRTaskTmp.ucResendDataInter=1;							//上送数据的频率		
//	stGRTaskTmp.usPerTaskRetMem=0;								//单个任务需要的内存数
//	stGRTaskTmp.usSavePointNum=CON_GRTASK_DATAPOINT_NUM;
//	stGRTaskTmp.usExeNum=0;
//	m=0;
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTPOW1;		//有功功率总加组1
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTPOW1>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTPOW2;		//有功功率总加组2
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTPOW2>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTPOW3;		//有功功率总加组3
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTPOW3>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTPOW4;		//有功功率总加组4
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTPOW4>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTPOW5;		//有功功率总加组5
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTPOW5>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTPOW6;		//有功功率总加组6
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTPOW6>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTPOW7;		//有功功率总加组7
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTPOW7>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTPOW8;		//有功功率总加组8
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTPOW8>>8);
//
//	stGRTaskTmp.ucDataItemNum=m>>1;	
//	ucTaskNo=CON_GRTASK_SUMGROUP_INSTANTPOW_TASKNO;
//	memcpy(&stGRDefineArea.stGRTask[ucTaskNo],&stGRTaskTmp,sizeof(STGRTASK));
//	Task_Config(ucTaskNo,CON_GRTASK_ACTIVE);
//	
//	m=0;
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTVAR1;		//无功功率总加组1
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTVAR1>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTVAR2;		//无功功率总加组2
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTVAR2>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTVAR3;		//无功功率总加组3
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTVAR3>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTVAR4;		//无功功率总加组4
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTVAR4>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTVAR5;		//无功功率总加组5
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTVAR5>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTVAR6;		//无功功率总加组6
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTVAR6>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTVAR7;		//无功功率总加组7
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTVAR7>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTVAR8;		//无功功率总加组8
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTVAR8>>8);
//
//	stGRTaskTmp.ucDataItemNum=m>>1;	
//	ucTaskNo=CON_GRTASK_SUMGROUP_INSTANTVAR_TASKNO;
//	memcpy(&stGRDefineArea.stGRTask[ucTaskNo],&stGRTaskTmp,sizeof(STGRTASK));
//	Task_Config(ucTaskNo,CON_GRTASK_ACTIVE);
//	
//	m=0;
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMPOW1;		//有功总加组1
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMPOW1>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMPOW2;		//有功总加组2
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMPOW2>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMPOW3;		//有功总加组3
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMPOW3>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMPOW4;		//有功总加组4
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMPOW4>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMPOW5;		//有功总加组5
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMPOW5>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMPOW6;		//有功总加组6
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMPOW6>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMPOW7;		//有功总加组7
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMPOW7>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMPOW8;		//有功总加组8
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMPOW8>>8);
//
//	stGRTaskTmp.ucDataItemNum=m>>1;	
//	ucTaskNo=CON_GRTASK_SUMGROUP_POW_TASKNO;
//	memcpy(&stGRDefineArea.stGRTask[ucTaskNo],&stGRTaskTmp,sizeof(STGRTASK));
//	Task_Config(ucTaskNo,CON_GRTASK_ACTIVE);
//
//	m=0;
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMVAR1;		//无功总加组1
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMVAR1>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMVAR2;		//无功总加组2
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMVAR2>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMVAR3;		//无功总加组3
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMVAR3>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMVAR4;		//无功总加组4
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMVAR4>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMVAR5;		//无功总加组5
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMVAR5>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMVAR6;		//无功总加组6
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMVAR6>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMVAR7;		//无功总加组7
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMVAR7>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMVAR8;		//无功总加组8
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMVAR8>>8);
//
//	stGRTaskTmp.ucDataItemNum=m>>1;	
//	ucTaskNo=CON_GRTASK_SUMGROUP_VAR_TASKNO;
//	memcpy(&stGRDefineArea.stGRTask[ucTaskNo],&stGRTaskTmp,sizeof(STGRTASK));
//	Task_Config(ucTaskNo,CON_GRTASK_ACTIVE);
//
//	return ucRet;
//}

//删除总加组任务是否有效
//unsigned char DeleteDefaultSumGroupTask(void)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucTaskNo;
//	ucTaskNo=CON_GRTASK_SUMGROUP_INSTANTPOW_TASKNO;
//	Task_Clear(ucTaskNo);
//	ucTaskNo=CON_GRTASK_SUMGROUP_INSTANTVAR_TASKNO;
//	Task_Clear(ucTaskNo);
//	ucTaskNo=CON_GRTASK_SUMGROUP_POW_TASKNO;
//	Task_Clear(ucTaskNo);
//	ucTaskNo=CON_GRTASK_SUMGROUP_VAR_TASKNO;
//	Task_Clear(ucTaskNo);
//	return ucRet;
//}

//检查脉冲任务是否有效
//unsigned char TaskCheckPulseIsValid(unsigned char ucPulseNo)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucTaskNo;
//	ucTaskNo=Task_GetTaskNo_byPulseNo(ucPulseNo);
//	if(ucTaskNo<CON_GRTASK_NUM)
//	{
//		if(stGRDefineArea.stGRTask[ucTaskNo].ucActive==CON_GRTASK_NOACTIVE)
//			ucRet=RET_ERROR;
//		if(Task_Check_Task_BaseInfo(ucTaskNo)==RET_ERROR)
//			ucRet=RET_ERROR;
//	}else 
//		ucRet=RET_ERROR;
//	return ucRet;
//}

//生成脉冲默认任务
//unsigned char SetDefaultPulseTask(unsigned char ucPulseNo)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucTaskNo;
//	unsigned char m;
//	memset(&stGRTaskTmp,0,sizeof(STGRTASK));
//	stGRTaskTmp.ucTaskType=CON_GRTASK_TYPE_NORMAL;
//	stGRTaskTmp.ucMPNo=RunPara_GetPulse_MPNo(ucPulseNo);
//	stGRTaskTmp.stTITaskStart.ucTimeInter=1;
//	stGRTaskTmp.stTITaskStart.ucTimeUnit=CON_TIMEUNIT_MIN;
//	stGRTaskTmp.stTITaskInter.ucTimeInter=15;
//	stGRTaskTmp.stTITaskInter.ucTimeUnit=CON_TIMEUNIT_MIN;
//	stGRTaskTmp.stTIResendStart.ucTimeInter=1;					
//	stGRTaskTmp.stTIResendStart.ucTimeUnit=CON_TIMEUNIT_MIN;
//	stGRTaskTmp.stTIReSendInter.ucTimeInter=0;					//0表示不主动上送
//	stGRTaskTmp.stTIReSendInter.ucTimeUnit=0;
//	stGRTaskTmp.ucResendDataInter=1;							//上送数据的频率		
//	stGRTaskTmp.usPerTaskRetMem=0;								//单个任务需要的内存数
//	stGRTaskTmp.usSavePointNum=CON_GRTASK_DATAPOINT_NUM;
//	stGRTaskTmp.usExeNum=0;
//	m=0;
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPDIRPOWTOTAL;		//正向有功总电能
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPDIRPOWTOTAL>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPDIRVARTOTAL;		//正向无功总电能
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPDIRVARTOTAL>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPUNDIRPOWTOTAL;		//反向有功总电能
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPUNDIRPOWTOTAL>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPUNDIRVARTOTAL;		//反向无功总电能
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPUNDIRVARTOTAL>>8);
//
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTPOW;		//瞬时有功功率
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTPOW>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTVAR;		//瞬时无功功率
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTVAR>>8);
//	stGRTaskTmp.ucDataItemNum=m>>1;	
//	ucTaskNo=Task_GetTaskNo_byPulseNo(ucPulseNo);
//	if(ucTaskNo<CON_GRTASK_NUM)
//	{
//		memcpy(&stGRDefineArea.stGRTask[ucTaskNo],&stGRTaskTmp,sizeof(STGRTASK));
//		Task_Config(ucTaskNo,CON_GRTASK_ACTIVE);
//	}else
//		ucRet=RET_ERROR;
//	
//	return ucRet;
//}

//删除脉冲任务
//unsigned char DeleteDefaultPulseTask(unsigned char ucPulseNo)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucTaskNo;
//	ucTaskNo=Task_GetTaskNo_byPulseNo(ucPulseNo);
//	Task_Clear(ucTaskNo);
//	return ucRet;
//}

//检查终端任务是否有效
unsigned char TaskCheckTerIsValid(void)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucTaskNo;
	ucTaskNo=Task_GetTaskNo_Ter();
	if(ucTaskNo<CON_GRTASK_NUM)
	{
		if(stGRDefineArea.stGRTask[ucTaskNo].ucActive==CON_GRTASK_NOACTIVE)
			ucRet=RET_ERROR;
		if(Task_Check_Task_BaseInfo(ucTaskNo)==RET_ERROR)
			ucRet=RET_ERROR;
	}else 
		ucRet=RET_ERROR;
	return ucRet;
}

//生成终端默认任务
unsigned char SetDefaultTerTask(void)
{
	unsigned char ucRet=RET_SUCCESS;
//azh 一个测量点一个曲线任务130123
//    unsigned char *mem_ptr=NULL,mem_type;
//	unsigned char ucTaskNo;
////	unsigned char m;
//	STGRTASK *pstGRTaskTmp;
//
//    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//    if(mem_ptr == NULL)
//    {
//        return RET_ERROR;
//    }
//    pstGRTaskTmp = (STGRTASK *)mem_ptr; 
//	memset(pstGRTaskTmp,0,sizeof(STGRTASK));
//
//	pstGRTaskTmp->ucTaskType=CON_GRTASK_TYPE_NORMAL;
//	pstGRTaskTmp->ucMPNo=RunPara_GetTer_MPNo();
//	pstGRTaskTmp->stTITaskStart.ucTimeInter=1;
//	pstGRTaskTmp->stTITaskStart.ucTimeUnit=CON_TIMEUNIT_MIN;
//	pstGRTaskTmp->stTITaskInter.ucTimeInter=15;
//	pstGRTaskTmp->stTITaskInter.ucTimeUnit=CON_TIMEUNIT_MIN;
//	pstGRTaskTmp->stTIResendStart.ucTimeInter=1;					
//	pstGRTaskTmp->stTIResendStart.ucTimeUnit=CON_TIMEUNIT_MIN;
//	pstGRTaskTmp->stTIReSendInter.ucTimeInter=0;					//0表示不主动上送
//	pstGRTaskTmp->stTIReSendInter.ucTimeUnit=0;
//	pstGRTaskTmp->ucResendDataInter=1;							//上送数据的频率		
//	pstGRTaskTmp->usPerTaskRetMem=0;								//单个任务需要的内存数
//	pstGRTaskTmp->usSavePointNum=CON_GRTASK_DATAPOINT_NUM;
//	pstGRTaskTmp->usExeNum=0;
////	m=0;
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPDIRPOWTOTAL;		//正向有功总电能
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPDIRPOWTOTAL>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPDIRVARTOTAL;		//正向无功总电能
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPDIRVARTOTAL>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPUNDIRPOWTOTAL;		//反向有功总电能
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPUNDIRPOWTOTAL>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPUNDIRVARTOTAL;		//反向无功总电能
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPUNDIRVARTOTAL>>8);
////
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVAR1;				//一象限无功总电能
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVAR1>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVAR4;				//四象限无功总电能
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVAR4>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVAR2;				//二象限无功总电能
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVAR2>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVAR3;				//三象限无功总电能
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVAR3>>8);
////	
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTPOW;		//瞬时有功功率
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTPOW>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTPOWA;		//A相瞬时有功功率
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTPOWA>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTPOWB;		//B相瞬时有功功率
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTPOWB>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTPOWC;		//C相瞬时有功功率
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTPOWC>>8);
////	
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTVAR;		//瞬时无功功率
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTVAR>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTVARA;		//A相瞬时无功功率
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTVARA>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTVARB;		//B相瞬时无功功率
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTVARB>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTVARC;		//C相瞬时无功功率
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTVARC>>8);
////
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVOLA;				//A相电压
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVOLA>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVOLB;				//B相电压
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVOLB>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVOLC;				//C相电压
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVOLC>>8);
////	
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPCURA;				//A相电流
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPCURA>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPCURB;				//B相电流
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPCURB>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPCURC;				//C相电流
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPCURC>>8);
//	pstGRTaskTmp->ucDataItemNum=CON_TASK_CURVE_DI_NUM;//m>>1;	
//	ucTaskNo=Task_GetTaskNo_Ter();
//	if(ucTaskNo<CON_GRTASK_NUM)
//	{
//		memcpy(&stGRDefineArea.stGRTask[ucTaskNo],pstGRTaskTmp,sizeof(STGRTASK));
//		Save_Curve_Task_Para(ucTaskNo, CON_TASK_BLK_OFST, CON_TASK_PARA_BLOCK_SAVE);
//		Task_Config(ucTaskNo,CON_GRTASK_ACTIVE);
//	}else
//		ucRet=RET_ERROR;
	
	return ucRet;
}

//删除终端任务
unsigned char DeleteDefaultTerTask(void)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucTaskNo;
	ucTaskNo=Task_GetTaskNo_Ter();
	Task_Clear(ucTaskNo);
	return ucRet;
}

//获得任务号
unsigned char Task_GetTaskNo_byMPNo(unsigned char ucMPNo,unsigned char ucPos)
{
	unsigned char ucTaskNo=CON_GRTASK_NUM,i=0;
	unsigned char ucOffset=0;
	for(i=0;i<CON_GRTASK_NUM;i++)
	{
		if(stGRDefineArea.stGRTask[i].ucMPNo==ucMPNo && stGRDefineArea.stGRTask[i].ucActive==CON_GRTASK_ACTIVE)
		{
			if(ucOffset==ucPos)
			{
				ucTaskNo=i;
				break;
			}
			ucOffset++;
		}
	}
	return ucTaskNo;
}

//脉冲对应获得任务号
//目前限制为前两路脉冲提供任务存储空间
//unsigned char Task_GetTaskNo_byPulseNo(unsigned char ucPulseNo)
//{
//	unsigned char ucTaskNo=CON_GRTASK_NUM;
//	switch(ucPulseNo)
//	{
//	case 0:
//		ucTaskNo=CON_GRTASK_PULSE_1_TASKNO;
//		break;
//	case 1:
//		ucTaskNo=CON_GRTASK_PULSE_2_TASKNO;
//		break;
//	case 2:
//		ucTaskNo=CON_GRTASK_PULSE_3_TASKNO;
//		break;
//	case 3:
//		ucTaskNo=CON_GRTASK_PULSE_4_TASKNO;
//		break;
//	}
//	return ucTaskNo;
//}

//终端对应获得任务号
unsigned char Task_GetTaskNo_Ter(void)
{
	return CON_GRTASK_TER_TASKNO;
}

//检查任务基本参数
unsigned char Task_Check_Task_BaseInfo(unsigned char ucTaskNo)
{
	unsigned char ucRet=RET_SUCCESS;
	STGRTASK * pstGRTask;
	if(ucTaskNo < CON_GRTASK_NUM)
	{
		pstGRTask=(STGRTASK *)&(stGRDefineArea.stGRTask[ucTaskNo]);
		if(pstGRTask->stTITaskStart.ucTimeUnit!=CON_TIMEUNIT_MIN)
			ucRet=RET_ERROR;
		if(pstGRTask->stTITaskInter.ucTimeInter!=15)
			ucRet=RET_ERROR;
		if(pstGRTask->stTITaskInter.ucTimeUnit!=CON_TIMEUNIT_MIN)
			ucRet=RET_ERROR;
		if(pstGRTask->stTIResendStart.ucTimeInter!=1)
			ucRet=RET_ERROR;
		if(pstGRTask->stTIResendStart.ucTimeUnit!=CON_TIMEUNIT_MIN)
			ucRet=RET_ERROR;
		if(pstGRTask->stTIReSendInter.ucTimeInter!=0)
			ucRet=RET_ERROR;
		if(pstGRTask->stTIReSendInter.ucTimeUnit!=0)
			ucRet=RET_ERROR;
	}else
		ucRet=RET_ERROR;
	return ucRet;
}

/******************************************************************************
** 函数名称: void Save_Curve_Task_Para(unsigned char ucTaskNo, unsigned char ucOffsetAddr, unsigned char type)
** 函数功能: 保存曲线(需要保存)任务运行参数
** 入口参数: ucOffsetAddr：参数偏移地址 type：保存参数块还是单项参数
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void Save_Curve_Task_Para(unsigned char ucTaskNo, unsigned char ucOffsetAddr, unsigned char type)
{
    unsigned char offset=0,len,block_len;
    unsigned char *mem_ptr=NULL,mem_type;
    unsigned long block_addr;
    
    if((ucTaskNo >= CON_GRTASK_NUM) || (ucOffsetAddr > CON_TASKINFO_BLKADDR_END))
    {
        return;    
    }    

    mem_ptr = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR0);  
    if(mem_ptr == NULL)
    {
        return;
    }     
    if(ucOffsetAddr < CON_TASKIF_BLK_OFST)
    {    
         offset = CON_TASK_BLK_OFST; 
        block_len = CON_TASK_BLKCRC_LENTH;
    }
    else if(ucOffsetAddr < CON_TASKRIF_BLK_OFST) 
    {    
        offset = CON_TASKIF_BLK_OFST; 
        block_len = CON_TASKIF_BLKCRC_LENTH;
    }
    else
    {    
        offset = CON_TASKRIF_BLK_OFST; 
        block_len = CON_TASKRIF_BLKCRC_LENTH;
    }
    block_addr = CON_GRTASK_PARA_BASE_ADDR + CON_GRTASK_PER_SPACE*ucTaskNo + offset;
    file_read(block_addr, mem_ptr, block_len);        
    //校验出错要默认参数处理!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
               
    if(type==CON_TASK_PARA_BLOCK_SAVE)//数据块的时候 传入的是数据块偏移首地址
    {    
        switch(ucOffsetAddr)
        {
            case CON_TASK_BLK_OFST:                    //ucTaskType//任务类型        
                offset = ucOffsetAddr - CON_TASK_TYPE_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTask[ucTaskNo].ucTaskType, CON_TASK_TYPE_LENTH);
                offset += CON_TASK_TYPE_LENTH;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTask[ucTaskNo].ucMPNo, CON_TASK_MPNO_LENTH);                
                offset += CON_TASK_MPNO_LENTH;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum, CON_TASK_SAVENUM_LENTH);
                offset += CON_TASK_SAVENUM_LENTH;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem, CON_TASK_PERRETLEN_LENTH);
                offset += CON_TASK_PERRETLEN_LENTH;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTask[ucTaskNo].ucActive, CON_TASK_ACTIVE_LENTH);
                offset += CON_TASK_ACTIVE_LENTH;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTask[ucTaskNo].stStartActive, CON_TASK_STARTACTIVE_LENTH);
//                len = CON_TASK_BLKCRC_LENTH;            //(13+1)//1crc
                break;    
            case CON_TASKIF_BLK_OFST:
                offset = ucOffsetAddr - CON_TASKIF_EXEDNUM_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum, CON_TASKIF_EXEDNUM_LENTH);
                offset += CON_TASKIF_EXEDNUM_LENTH;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos, CON_TASKIF_TAILPOS_LENTH);
                offset += CON_TASKIF_TAILPOS_LENTH;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTaskInfo[ucTaskNo].ulTailTime, CON_TASKIF_TAILTIME_LENTH);
//                len = CON_TASKIF_BLKCRC_LENTH;          //(8+1)//1crc
                break;
            case CON_TASKRIF_BLK_OFST:
                offset = ucOffsetAddr - CON_TASKRIF_NEXTEXTIME_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTaskRunInfo[ucTaskNo].stNextExeTime, CON_TASKRIF_NEXTEXTIME_LENTH);
                offset += CON_TASKRIF_NEXTEXTIME_LENTH;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTaskRunInfo[ucTaskNo].stNextUpTime, CON_TASKRIF_NEXTUPTIME_LENTH);
                offset += CON_TASKRIF_NEXTUPTIME_LENTH;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTaskRunInfo[ucTaskNo].usHeadPos, CON_TASKRIF_HEADPOS_LENTH);
//                len = CON_TASKRIF_BLKCRC_LENTH;         //(14+1)//1crc
                break;
            default:
                free_mem_block(mem_ptr);
                return;    
        }
    }
    else
    {    
        switch(ucOffsetAddr)
        {
            case CON_TASK_TYPE_OFST:              //ucTaskType//任务类型
                len = CON_TASK_TYPE_LENTH; 
                offset = ucOffsetAddr - CON_TASK_TYPE_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTask[ucTaskNo].ucTaskType, len);
                break;
            case CON_TASK_MPNO_OFST:              //ucMPNo普通任务的测量点号&中继任务的端口号
                len = CON_TASK_MPNO_LENTH;
                offset = ucOffsetAddr - CON_TASK_TYPE_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTask[ucTaskNo].ucMPNo, len);
                break;
            case CON_TASK_SAVENUM_OFST:           //usSavePointNum任务的保存点数(不使用设定值，使用自身计算得的值)
                len = CON_TASK_SAVENUM_LENTH;
                offset = ucOffsetAddr - CON_TASK_TYPE_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum, len);
                break;
            case CON_TASK_PERRETLEN_OFST:         //usPerTaskRetMem任务执行一次所虚的字节数
                len = CON_TASK_PERRETLEN_LENTH;
                offset = ucOffsetAddr - CON_TASK_TYPE_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem, len);
                break;
            case CON_TASK_ACTIVE_OFST:            //ucActive//任务状态：激活
                len = CON_TASK_ACTIVE_LENTH;
                offset = ucOffsetAddr - CON_TASK_TYPE_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTask[ucTaskNo].ucActive, len);
                break;
            case CON_TASK_STARTACTIVE_OFST:       //stStartActive//任务开始活动时间
                len = CON_TASK_STARTACTIVE_LENTH;
                offset = ucOffsetAddr - CON_TASK_TYPE_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTask[ucTaskNo].stStartActive, len);
                break;
//
            case CON_TASKIF_EXEDNUM_OFST:         //usExedNum//任务执行了次数
                len = CON_TASKIF_EXEDNUM_LENTH;
                offset = ucOffsetAddr - CON_TASKIF_EXEDNUM_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum, len);
                break;
            case CON_TASKIF_TAILPOS_OFST:         //usTailPos//最早的数据点，数据区循环存储usSavePointNum个点
                len = CON_TASKIF_TAILPOS_LENTH;
                offset = ucOffsetAddr - CON_TASKIF_EXEDNUM_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos, len);
                break;      
            case CON_TASKIF_TAILTIME_OFST:        //ulTailTime//最早的数据点对应的时间，从1990年开始，单位：秒
                len = CON_TASKIF_TAILTIME_LENTH;
                offset = ucOffsetAddr - CON_TASKIF_EXEDNUM_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTaskInfo[ucTaskNo].ulTailTime, len);
                break;
//
            case CON_TASKRIF_NEXTEXTIME_OFST:     //stNextExeTime//下次执行时间(任务)
                len = CON_TASKRIF_NEXTEXTIME_LENTH;
                offset = ucOffsetAddr - CON_TASKRIF_NEXTEXTIME_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTaskRunInfo[ucTaskNo].stNextExeTime, len);
                break;
            case CON_TASKRIF_NEXTUPTIME_OFST:     //stNextUpTime//下次数据上行时间
                len = CON_TASKRIF_NEXTUPTIME_LENTH;
                offset = ucOffsetAddr - CON_TASKRIF_NEXTEXTIME_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTaskRunInfo[ucTaskNo].stNextUpTime, len);
                break;
            case CON_TASKRIF_HEADPOS_OFST:        //usHeadPos//当前存储的点号
                len = CON_TASKRIF_HEADPOS_LENTH;
                offset = ucOffsetAddr - CON_TASKRIF_NEXTEXTIME_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTaskRunInfo[ucTaskNo].usHeadPos, len);
                break;
            default:
                free_mem_block(mem_ptr);
                return;    
        }
    }
    mem_ptr[block_len-1] = addverify(mem_ptr, (block_len-1));
    file_write(block_addr, mem_ptr, block_len); 
    free_mem_block(mem_ptr);    
}

/******************************************************************************
** 函数名称: void PowOn_Get_Curve_Task_Para(void)
** 函数功能: 上电从eeprom中导入运行相关参数
** 入口参数: 
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void PowOn_Get_Curve_Task_Para(void)
{
    unsigned char ucTaskNo,blok_len;
    unsigned char *mem_ptr=NULL,mem_type;
    unsigned long block_addr;

    mem_ptr = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR0);  
    if(mem_ptr == NULL)
    {
        return;
    }         
    blok_len = CON_TASK_BLKCRC_LENTH + CON_TASKIF_BLKCRC_LENTH + CON_TASKRIF_BLKCRC_LENTH;
    memset(&stGRDefineArea.stGRTaskRetVal, 0, sizeof(STGRTASKRETVAL));
    for(ucTaskNo=0; ucTaskNo<CON_GRTASK_NUM; ucTaskNo++)
    {
        memset(&stGRDefineArea.stGRTask[ucTaskNo], 0, sizeof(STGRTASK));
        memset(&stGRDefineArea.stGRTaskInfo[ucTaskNo], 0, sizeof(STGRTASKINFO));
        memset(&stGRDefineArea.stGRTaskRunInfo[ucTaskNo], 0, sizeof(STGRTASKRUNINFO));
        block_addr = CON_GRTASK_PARA_BASE_ADDR + CON_GRTASK_PER_SPACE*ucTaskNo;
        file_read(block_addr, mem_ptr, blok_len);
//只有所有参数校验码正确才要导入数据
        if(mem_ptr[CON_TASK_BLK_OFST+CON_TASK_BLKCRC_LENTH-1] != addverify(mem_ptr+CON_TASK_BLK_OFST, (CON_TASK_BLKCRC_LENTH-1)))
        {
            continue;    
        }    
        if(mem_ptr[CON_TASKIF_BLK_OFST+CON_TASKIF_BLKCRC_LENTH-1] != addverify(mem_ptr+CON_TASKIF_BLK_OFST, (CON_TASKIF_BLKCRC_LENTH-1)))
        {
            continue;    
        }            
        if(mem_ptr[CON_TASKRIF_BLK_OFST+CON_TASKRIF_BLKCRC_LENTH-1] != addverify(mem_ptr+CON_TASKRIF_BLK_OFST, (CON_TASKRIF_BLKCRC_LENTH-1)))
        {
            continue;    
        }
        memcpy((unsigned char*)&stGRDefineArea.stGRTask[ucTaskNo].ucTaskType, mem_ptr+CON_TASK_TYPE_OFST, CON_TASK_TYPE_LENTH);
        memcpy((unsigned char*)&stGRDefineArea.stGRTask[ucTaskNo].ucMPNo, mem_ptr+CON_TASK_MPNO_OFST, CON_TASK_MPNO_LENTH);                
        memcpy((unsigned char*)&stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum, mem_ptr+CON_TASK_SAVENUM_OFST, CON_TASK_SAVENUM_LENTH);
        memcpy((unsigned char*)&stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem, mem_ptr+CON_TASK_PERRETLEN_OFST, CON_TASK_PERRETLEN_LENTH);
        memcpy((unsigned char*)&stGRDefineArea.stGRTask[ucTaskNo].ucActive, mem_ptr+CON_TASK_ACTIVE_OFST, CON_TASK_ACTIVE_LENTH);
        memcpy((unsigned char*)&stGRDefineArea.stGRTask[ucTaskNo].stStartActive, mem_ptr+CON_TASK_STARTACTIVE_OFST, CON_TASK_STARTACTIVE_LENTH);
//
        memcpy((unsigned char*)&stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum, mem_ptr+CON_TASKIF_EXEDNUM_OFST, CON_TASKIF_EXEDNUM_LENTH);
        memcpy((unsigned char*)&stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos, mem_ptr+CON_TASKIF_TAILPOS_OFST, CON_TASKIF_TAILPOS_LENTH);
        memcpy((unsigned char*)&stGRDefineArea.stGRTaskInfo[ucTaskNo].ulTailTime, mem_ptr+CON_TASKIF_TAILTIME_OFST, CON_TASKIF_TAILTIME_LENTH);
//
        memcpy((unsigned char*)&stGRDefineArea.stGRTaskRunInfo[ucTaskNo].stNextExeTime, mem_ptr+CON_TASKRIF_NEXTEXTIME_OFST, CON_TASKRIF_NEXTEXTIME_LENTH);
        memcpy((unsigned char*)&stGRDefineArea.stGRTaskRunInfo[ucTaskNo].stNextUpTime, mem_ptr+CON_TASKRIF_NEXTUPTIME_OFST, CON_TASKRIF_NEXTUPTIME_LENTH);
        memcpy((unsigned char*)&stGRDefineArea.stGRTaskRunInfo[ucTaskNo].usHeadPos, mem_ptr+CON_TASKRIF_HEADPOS_OFST, CON_TASKRIF_HEADPOS_LENTH);
//没有保存的参数是固定值
        stGRDefineArea.stGRTask[ucTaskNo].stTITaskStart.ucTimeInter=0;
        stGRDefineArea.stGRTask[ucTaskNo].stTITaskStart.ucTimeUnit=CON_TIMEUNIT_MIN;
        stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeInter=15;
        stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit=CON_TIMEUNIT_MIN;
        stGRDefineArea.stGRTask[ucTaskNo].stTIResendStart.ucTimeInter=1;
        stGRDefineArea.stGRTask[ucTaskNo].stTIResendStart.ucTimeUnit=CON_TIMEUNIT_MIN;
        stGRDefineArea.stGRTask[ucTaskNo].stTIReSendInter.ucTimeInter=0;
        stGRDefineArea.stGRTask[ucTaskNo].stTIReSendInter.ucTimeUnit=0;        
        stGRDefineArea.stGRTask[ucTaskNo].ucResendDataInter=1;
        stGRDefineArea.stGRTask[ucTaskNo].usExeNum=0;        
        stGRDefineArea.stGRTask[ucTaskNo].ucDataItemNum=CON_TASK_CURVE_DI_NUM;
    }    
    free_mem_block(mem_ptr);    
}

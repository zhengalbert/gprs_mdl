/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : CTask_Manage.c
 Description    : ��������ģ���������
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-08-15    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

static unsigned char Task_Check_Task_BaseInfo(unsigned char ucTaskNo);		//������������Ϣ
//-----------------------------------
//azh ������������
const U16 curve_task_di[CON_TASK_CURVE_DI_NUM] =
{
    CON_DI_TYPE_MPDIRPOWTOTAL,		//�����й��ܵ���
    CON_DI_TYPE_MPDIRVARTOTAL,		//�����޹��ܵ���
    CON_DI_TYPE_MPUNDIRPOWTOTAL,	//�����й��ܵ���
    CON_DI_TYPE_MPUNDIRVARTOTAL,	//�����޹��ܵ���
    CON_DI_TYPE_MPVAR1,				//һ�����޹��ܵ���
    CON_DI_TYPE_MPVAR4,				//�������޹��ܵ���
    CON_DI_TYPE_MPVAR2,				//�������޹��ܵ���
    CON_DI_TYPE_MPVAR3,				//�������޹��ܵ���
    CON_DI_TYPE_MPINSTANTPOW,		//˲ʱ�й�����
    CON_DI_TYPE_MPINSTANTPOWA,		//A��˲ʱ�й�����
    CON_DI_TYPE_MPINSTANTPOWB,		//B��˲ʱ�й�����
    CON_DI_TYPE_MPINSTANTPOWC,		//C��˲ʱ�й�����
    CON_DI_TYPE_MPINSTANTVAR,		//˲ʱ�޹�����
    CON_DI_TYPE_MPINSTANTVARA,		//A��˲ʱ�޹�����
    CON_DI_TYPE_MPINSTANTVARB,		//B��˲ʱ�޹�����
    CON_DI_TYPE_MPINSTANTVARC,		//C��˲ʱ�޹�����
    CON_DI_TYPE_MPVOLA,				//A���ѹ
    CON_DI_TYPE_MPVOLB,				//B���ѹ
    CON_DI_TYPE_MPVOLC,				//C���ѹ
    CON_DI_TYPE_MPCURA,				//A�����
    CON_DI_TYPE_MPCURB,				//B�����
    CON_DI_TYPE_MPCURC				//C�����
};

//������Ƴ�ʼ��
//���DATAFLASH�洢��������Ϣ����Ч����µ�ǰ�����������������Ч���ʼ��DATAFLASH�͵�ǰ������
unsigned char Task_Manage_Init(void)
{
	unsigned char ucRet=RET_SUCCESS;
//	unsigned char i;
//	ucRet=TaskCheckSumGroupIsValid();					//����ܼ�����
//	if(ucRet==RET_ERROR)
//		SetDefaultSumGroupTask();
//	for(i=0;i<CON_PULSE_NUM;i++)
//	{
//		if(RunPara_GetPulse_MPNo(i)!=CON_MPNO_INVALID)
//		{
//			ucRet=TaskCheckPulseIsValid(i);				//�����������
//			if(ucRet==RET_ERROR)
//				SetDefaultPulseTask(i);
//		}
//	}
    PowOn_Get_Curve_Task_Para();//azh �ϵ��eeprom�е���������ز���

	if(RunPara_GetTer_MPNo()!=CON_MPNO_INVALID)
	{
		ucRet=TaskCheckTerIsValid();					//����ն�����
		if(ucRet==RET_ERROR)
			SetDefaultTerTask();
	}
	ucRet=Task_CheckAllTaskInfo();						//����������Ϣ�����ؽ�����Ƿ���Ҫ�洢����DATAFLASH
	Task_GetTaskRunInfo();								//�������������Ϣ
	return ucRet;
}

//����������Ϣ
unsigned char Task_CheckAllTaskInfo(void)
{
	unsigned char ucRet=RET_ERROR;
	unsigned char i;
	for(i=0;i<CON_GRTASK_NUM;i++)
	{
		if(stGRDefineArea.stGRTask[i].ucActive==CON_GRTASK_ACTIVE)
		{//����������Ч�Լ��,������Ч����
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
			;//������գ�������ͣ״̬
		}
		else
		{
			memset(&stGRDefineArea.stGRTask[i],CON_GRTASK_DATA_INVALID,sizeof(STGRTASK));
			stGRDefineArea.stGRTask[i].ucActive=CON_GRTASK_NOACTIVE;
			Save_Curve_Task_Para(i, CON_TASK_BLK_OFST, CON_TASK_PARA_BLOCK_SAVE);//azh
			memset(&stGRDefineArea.stGRTaskInfo[i],CON_GRTASK_DATA_INVALID,sizeof(STGRTASKINFO));
			Save_Curve_Task_Para(i, CON_TASKIF_BLK_OFST, CON_TASK_PARA_BLOCK_SAVE);
			ucRet=RET_SUCCESS;		//��ʾ������������Ҫ��DATAFLASH
		}
	}
	
	return ucRet;
}

//�������������Ϣ
unsigned char Task_GetTaskRunInfo(void)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char i;
	unsigned long ulTimeInter;
	//��������������Ϣ����������Ϣ��ʼ�������������Ϣ
	for(i=0;i<CON_GRTASK_NUM;i++)
	{
		if(stGRDefineArea.stGRTask[i].ucActive==CON_GRTASK_ACTIVE)
		{
			stGRDefineArea.stGRTaskRunInfo[i].usHeadPos=stGRDefineArea.stGRTaskInfo[i].usExedNum % stGRDefineArea.stGRTask[i].usSavePointNum;
			//����ն˵���ǰ��ʱ�䣬�����������ڼ������ʱ������Ч����
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
			//��õ�ǰ����ʱ��
			stGRDefineArea.stGRTaskRunInfo[i].stNextUpTime=GetTaskNextUpTime(i);
			Save_Curve_Task_Para(i, CON_TASKRIF_BLK_OFST, CON_TASK_PARA_BLOCK_SAVE);//azh ���浽EE
		}
	}
	return ucRet;
}

//�������������ܵ�������������Ӧ���ݵĿռ���䶼�ǹ̶��ģ���ˣ��������������Ĵ����ȹ�����
//	�����������56K24������
//  ��������������Ϣ
//	����stGRDefineArea
unsigned char Task_Config(unsigned char ucTaskNo,unsigned char ucActive)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned long ulDestAddr = 0;
	unsigned long ulNum_Task = 0;
	if(ucTaskNo>=CON_GRTASK_NUM)
		return RET_ERROR;
	
	//��GRTASK���GRTASKINFO��GRTASKRUNINFO
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
//azh ����������в������浽EE
    Save_Curve_Task_Para(ucTaskNo, CON_TASK_BLK_OFST, CON_TASK_PARA_BLOCK_SAVE);
    Save_Curve_Task_Para(ucTaskNo, CON_TASKIF_BLK_OFST, CON_TASK_PARA_BLOCK_SAVE);
    Save_Curve_Task_Para(ucTaskNo, CON_TASKRIF_BLK_OFST, CON_TASK_PARA_BLOCK_SAVE);	
    
	return ucRet;
}

//�������
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
		stGRDefineArea.stGRTask[ucTaskNo].ucTaskType		=CON_GRTASK_TYPE_INVALID;		//��������Ϊ��Ч
		stGRDefineArea.stGRTask[ucTaskNo].ucActive			=CON_GRTASK_NOACTIVE;			//������������״̬Ϊ���
		stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum=0;
		stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos=0;
		stGRDefineArea.stGRTaskInfo[ucTaskNo].ulTailTime=0;
//azh ����������в������浽EE
        Save_Curve_Task_Para(ucTaskNo, CON_TASK_BLK_OFST, CON_TASK_PARA_BLOCK_SAVE);
        Save_Curve_Task_Para(ucTaskNo, CON_TASKIF_BLK_OFST, CON_TASK_PARA_BLOCK_SAVE);			
	}
	return ucRet;
}

//��ȡ����ʱ������ݳ���
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
		case CON_GRTASK_TYPE_NORMAL:				//��ͨ��������:�����ݱ�ʶ(2)+�����ֽ���(1)+��������(x)��*��������(x)
			for(i=0;i<pstGRTask->ucDataItemNum;i++)
			{
//				usDILabel=pstGRTask->ucDataArea[i*2]+(pstGRTask->ucDataArea[i*2+1])*0x100;
				usDILabel = curve_task_di[i];
				stDIInfoTmp=Main_DI_Data_Query_Manage( usDILabel );	//��ѯ������
				if( stDIInfoTmp.ucHave	==CON_DI_HAVE && stDIInfoTmp.ucMPType==CON_DI_MPTYPE_NORMAL && 
					stDIInfoTmp.ucType	==CON_DI_TYPE_DATA )	//Ҫ���ǵ������
					usPerTaskRetMem+=(3+stDIInfoTmp.ucContentLen);
				else 
					usPerTaskRetMem+=3+1;
			}
			break;
		case CON_GRTASK_TYPE_RELAY:					//�м���������:���ݱ�ʶ(2)+�����ֽ���(1)+��������(96)
			usPerTaskRetMem=96+3;
			break;
		default:	
			break;
		}
	}
	
	//	if((usPerTaskRetMem%16)!=0)
	//		usPerTaskRetMem=(usPerTaskRetMem/16+1)*16;		//��16����,Ҳ�������<16,��=16
	return usPerTaskRetMem;
}

//��ȡ����ʱ������ݳ���
//������ƣ��������ݵ㲻����ʱ�䣬���Ը���λ�ú���ʼʱ��������ݵ�ʱ��
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
		case CON_GRTASK_TYPE_NORMAL:				//��ͨ��������:����������(x)
			for(i=0;i<pstGRTask->ucDataItemNum;i++)
			{
//				usDILabel=pstGRTask->ucDataArea[i*2]+(pstGRTask->ucDataArea[i*2+1])*0x100;
                usDILabel = curve_task_di[i];
				stDIInfoTmp=Main_DI_Data_Query_Manage( usDILabel );	//��ѯ������
				if( stDIInfoTmp.ucHave	==CON_DI_HAVE && stDIInfoTmp.ucMPType==CON_DI_MPTYPE_NORMAL && 
					stDIInfoTmp.ucType	==CON_DI_TYPE_DATA )	//Ҫ���ǵ������
					usPerTaskRetMem+=stDIInfoTmp.ucContentLen;
			}
			break;
		case CON_GRTASK_TYPE_RELAY:					//�м���������:��������(96)
			usPerTaskRetMem=96;
			break;
		default:	
			break;
		}
	}
	
	return usPerTaskRetMem;
}

//�����Ӧ���ݴ洢
//����һ��ʱ�����������������ݴ���DATAFLASH
unsigned char Task_SaveRetVal(unsigned char ucTaskNo,unsigned char *pucRetData,unsigned short usRetDataLen)
{
	unsigned char	ucRet=RET_ERROR;
	unsigned short	usLen=0;
//	unsigned short	usBeginPage=0,usBeginPos=0;
    unsigned short	uspointnum_perblock=0,ustaskpoint=0;
	unsigned long	ulDataAddr=0,ultemp;
//	unsigned char	ucDownloadSource=RunPara_GetTerAdjPara_DownloadSource();
	
	//����ճ��򣬲��洢
//	if((ucDownloadSource==CON_TERDP_SOURCE_RS232 || ucDownloadSource==CON_TERDP_SOURCE_REMOTE) && DP_Get_TerDP_State()==CON_TERDP_STATE_DOWNLOAD)
//		return ucRet;
	//�����ͣ���ڼ䣬�򲻴洢
//	if(g_ucNoPowerFlag==CON_NOPOWER_STATUS_HAPPEN)
//		return ucRet;
	usLen=stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem;
	ultemp = MCP_FLASH_BLOCK_SIZE / usLen;
	uspointnum_perblock = (unsigned short)ultemp;
	ustaskpoint = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum % stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum;
//	usLen=stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem;
	if(usRetDataLen>usLen)
		return ucRet;
	
	//����洢λ�ã��������������DATAFALSH�еĵ�ַ
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

//�����Ӧ���ݲ�ѯ��ȡ
//ע��pucRetDataͷ���ֽڴ��ʵ�ʵ���������˳���������������㳤�ȹ̶�������������λ�ò��̶�
//ע���ϲ㺯��Ӧ�ÿ��ǵ������ܳ���̫��
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

	((unsigned short *)pucRetData)[0]=0;			//���ݿ��ŵ�����ռ��ǰ2���ֽ�
	if(ucTaskNo>=CON_GRTASK_NUM)
		return RET_ERROR;
	
//	pstGRTask=&(stGRDefineArea.stGRTask[ucTaskNo]);
	if(ucResendDataInter==0)			//����Ƶ�ʲ���Ϊ0������ΪĬ��ֵ1
		ucResendDataInter=1;
	
	//�жϿ�ʼʱ���Ƿ���Ч��Ӧ�ò�С�����ݿ���������ʱ��
	stTempTime=(*pstStartTime);
	AddTime(&stTempTime,stGRDefineArea.stGRTask[ucTaskNo].stTITaskStart.ucTimeInter,stGRDefineArea.stGRTask[ucTaskNo].stTITaskStart.ucTimeUnit);	//�洢ʱ��Ϊ���ݶ���Ҫ��ʱ��+�洢�ӳ�ʱ��
	if(stGRDefineArea.stGRTaskInfo[ucTaskNo].ulTailTime>(TimeConver(&stTempTime,CON_TIMEUNIT_SEC)))
		return RET_ERROR;
	
	if(stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem==0)
		usBufferMaxPoint=0xFFFF;
	else
		usBufferMaxPoint=(CON_TASK_DATA_MAXBUFFER-2)/stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem;
	usPointNumTemp=usPointNum;
	if(usPointNum>usBufferMaxPoint)
		usPointNumTemp=usBufferMaxPoint;			//��һ�ε��������ݻ��������ƣ��ϲ㺯�����Է�����ȡ
	
	pucBuffer=pucRetData+2;
	usLen=stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem;

	ustasknum_perblock = MCP_FLASH_BLOCK_SIZE / usLen;

	
	memset(pucBuffer,CON_GRTASK_DATA_INVALID,CON_TASK_DATA_MAXBUFFER-2);
	//�����������ݿ�������õ����ݿ��ַȡ����
//	usStartPage = CON_DFPAGE_HEAD_DATABLOCK + CON_DFPAGE_DATABLOCK_PAGENUM*ucTaskNo;
	//���ݶ�ȡ��ʱ����������DATAFLASH���ݿ鿪ʼ�ĵ�λ�ã��������һ���㣬��Ҫ����
	ulTemp=TimeConver(&stTempTime,stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit)
		- TimeConver(&(stGRDefineArea.stGRTask[ucTaskNo].stStartActive), stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit);
	usOutputPos=(unsigned short)(ulTemp/stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeInter);
//azh 160311---
    //Ŀǰ�������Ч���ݵ���
	if(stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum >= stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos)
	{
	    usBufferMaxPoint = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum - stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos;
	}
	else
	{
	    usBufferMaxPoint = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum + stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum - stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos;
	}        
    
    if(usOutputPos < usBufferMaxPoint)//azh 160311 ��ȡ�㻹����Ч���ݷ�Χ֮��
    {
        usOutputPos = usOutputPos + stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos;
        if(usOutputPos >= stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum)
        {
            usOutputPos -= stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum;//��������ʱ�����
        }    

        if(usOutputPos <= stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum)//���õ��������ݵ�
        {    
            usBufferMaxPoint = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum;
        }
        else
        {   //azh Ϊ��ѭ��ʱ++�ñȽ����,����ֻҪС�ھͿ����ж���û�������Ҫ��Ȼ�и����ݱ�С������
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
    		usOutputPos+=ucResendDataInter;//azh���Գ���һ��ѭ��

    		usOffset = usOutputPos;
    		if(usOffset >= stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum)
    		{    
    		    //azh����DATAFLASHѰַ���ܳ���һ��ѭ��
    		    usOffset = usOutputPos - stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum;
    		}
//azh 160311---    		
    	}
    }
	if(ucRet==RET_SUCCESS)
		((unsigned short *)pucRetData)[0]=i;		//���ݿ��ŵ�����ռ��ǰ2���ֽ�
	
	(*pusOnePointLen)=stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem;//azh Ҫ������ÿ�����ݵ�ĳ���
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
	
	((unsigned short *)pucRetData)[0]=usPointNum;			//���ݿ��ŵ�����ռ��ǰ2���ֽ�
	if(ucTaskNo>=CON_GRTASK_NUM)
		return RET_ERROR;
	
//	pstGRTask=&(stGRDefineArea.stGRTask[ucTaskNo]);
	if(ucResendDataInter==0)			//����Ƶ�ʲ���Ϊ0������ΪĬ��ֵ1
		ucResendDataInter=1;
	
	if(stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem==0)
		usBufferMaxPoint=0xFFFF;
	else
//		usBufferMaxPoint=(CON_TASK_DATA_MAXBUFFER-2-stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem)/16;		//16Ϊ���� 
//azh 140918  ��Ϊ���������ڴ�黺����1536�ģ������������ݻ����һ�»��壬��Ȼ����������󲻳���512������Ӱ�췵��96����������
        usBufferMaxPoint=(1024-2-stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem)/8;//azh 140906//16;		//16Ϊ����
	usPointNumTemp=usPointNum;
	if(usPointNum>usBufferMaxPoint)
		usPointNumTemp=usBufferMaxPoint;			//��һ�ε��������ݻ��������ƣ��ϲ㺯�����Է�����ȡ
	
	pucBuffer=pucRetData+2;
	usLen=stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem;
	ustasknum_perblock = MCP_FLASH_BLOCK_SIZE / usLen;
	memset(pucBuffer,CON_GRTASK_DATA_INVALID,CON_TASK_DATA_MAXBUFFER-2);

	stDIInfoTmp=Main_DI_Data_Query_Manage(usDI);
	ucDataLen=stDIInfoTmp.ucContentLen;
	//�жϿ�ʼʱ���Ƿ���Ч��Ӧ�ò�С�����ݿ���������ʱ��
	stTempTime=(*pstStartTime);
//	AddTime(&stTempTime,stGRDefineArea.stGRTask[ucTaskNo].stTITaskStart.ucTimeInter,stGRDefineArea.stGRTask[ucTaskNo].stTITaskStart.ucTimeUnit);	//�洢ʱ��Ϊ���ݶ���Ҫ��ʱ��+�洢�ӳ�ʱ��100330
	stTempTailTime=TimeConverToDataTime(stGRDefineArea.stGRTaskInfo[ucTaskNo].ulTailTime);
	if(stGRDefineArea.stGRTaskInfo[ucTaskNo].ulTailTime>(TimeConver(&stTempTime,CON_TIMEUNIT_SEC)))
	{
		usNumTemp=Get_CurvePointNum_ByTime(&stTempTime,&stTempTailTime,ucResendDataInter);
		if(usPointNumTemp<=usNumTemp)
		{
			pucBuffer+=(usPointNumTemp*ucDataLen);
			usPointNumTemp=0;											//����Ϊ0���򽫲���ȡDATAFLASH
		}else
		{
			pucBuffer+=(usNumTemp*ucDataLen);								//�����Ч����
			usPointNumTemp-=usNumTemp;
		}
		stTempTime=stTempTailTime;										//����ʱ��
	}
	//�����������ݿ�������õ����ݿ��ַȡ����
//	usStartPage = CON_DFPAGE_HEAD_DATABLOCK + CON_DFPAGE_DATABLOCK_PAGENUM*ucTaskNo;
	//���ݶ�ȡ��ʱ����������DATAFLASH���ݿ鿪ʼ�ĵ�λ�ã��������һ���㣬��Ҫ����
	ulTemp=TimeConver(&stTempTime,stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit)
		- TimeConver(&(stGRDefineArea.stGRTask[ucTaskNo].stStartActive), stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit);
	usOutputPos=(unsigned short)(ulTemp/stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeInter);
//azh 160311---
    //Ŀǰ�������Ч���ݵ���
	if(stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum >= stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos)
	{
	    usBufferMaxPoint = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum - stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos;
	}
	else
	{
	    usBufferMaxPoint = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum + stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum - stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos;
	}        
    
    if(usOutputPos < usBufferMaxPoint)//azh 160311 ��ȡ�㻹����Ч���ݷ�Χ֮��
    {
        usOutputPos = usOutputPos + stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos;
        if(usOutputPos >= stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum)
        {
            usOutputPos -= stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum;//��������ʱ�����
        }    

        if(usOutputPos <= stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum)//usBufferMaxPoint���õ��������ݵ�
        {    
            usBufferMaxPoint = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum;
        }
        else
        {   //azh Ϊ��ѭ��ʱ++�ñȽ����,����ֻҪС�ھͿ����ж���û�������Ҫ��Ȼ�и����ݱ�С������
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
    		    //azh����DATAFLASHѰַ���ܳ���һ��ѭ��
    		    usOffset = usOutputPos - stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum;
    		}
    	}
    }
//azh 160311---	
	(*pucDataLen)=ucDataLen;//azh Ҫ������ÿ�����ݵ�������ĳ���
	return ucRet;
}
//azh 170925 ��Ӧ376.1-2013��Ķ�������F219:���������������������
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
	
	((unsigned short *)pucRetData)[0]=usPointNum;			//���ݿ��ŵ�����ռ��ǰ2���ֽ�
	if(ucTaskNo>=CON_GRTASK_NUM)
		return RET_ERROR;
	
//	pstGRTask=&(stGRDefineArea.stGRTask[ucTaskNo]);
	if(ucResendDataInter==0)			//����Ƶ�ʲ���Ϊ0������ΪĬ��ֵ1
		ucResendDataInter=1;
	
	if(stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem==0)
		usBufferMaxPoint=0xFFFF;
	else
//		usBufferMaxPoint=(CON_TASK_DATA_MAXBUFFER-2-stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem)/16;		//16Ϊ���� 
//azh 140906  ��Ϊ���������ڴ�黺����1536�ģ������������ݻ����һ�»��壬��Ȼ����������󲻳���512������Ӱ�췵��96����������
        usBufferMaxPoint=(1024-2-stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem)/8;//azh 140906//16;		//16Ϊ����
	usPointNumTemp=usPointNum;
	if(usPointNum>usBufferMaxPoint)
		usPointNumTemp=usBufferMaxPoint;			//��һ�ε��������ݻ��������ƣ��ϲ㺯�����Է�����ȡ
	
	pucBuffer=pucRetData+2;
	usLen=stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem;
	ustasknum_perblock = MCP_FLASH_BLOCK_SIZE / usLen;
	memset(pucBuffer,CON_GRTASK_DATA_INVALID,CON_TASK_DATA_MAXBUFFER-2);

//	stDIInfoTmp=Main_DI_Data_Query_Manage(usDI);
//	ucDataLen=stDIInfoTmp.ucContentLen;
    ucDataLen = 32;//azh 170925 ��������F219ר�ã��������ﳤ��д��������ʱʵ�ʸ�ʽת�����޹�������3B����һ���ֽ�

	//�жϿ�ʼʱ���Ƿ���Ч��Ӧ�ò�С�����ݿ���������ʱ��
	stTempTime=(*pstStartTime);
//	AddTime(&stTempTime,stGRDefineArea.stGRTask[ucTaskNo].stTITaskStart.ucTimeInter,stGRDefineArea.stGRTask[ucTaskNo].stTITaskStart.ucTimeUnit);	//�洢ʱ��Ϊ���ݶ���Ҫ��ʱ��+�洢�ӳ�ʱ��100330
	stTempTailTime=TimeConverToDataTime(stGRDefineArea.stGRTaskInfo[ucTaskNo].ulTailTime);
	if(stGRDefineArea.stGRTaskInfo[ucTaskNo].ulTailTime>(TimeConver(&stTempTime,CON_TIMEUNIT_SEC)))
	{
		usNumTemp=Get_CurvePointNum_ByTime(&stTempTime,&stTempTailTime,ucResendDataInter);
		if(usPointNumTemp<=usNumTemp)
		{
			pucBuffer+=(usPointNumTemp*ucDataLen);
			usPointNumTemp=0;											//����Ϊ0���򽫲���ȡDATAFLASH
		}else
		{
			pucBuffer+=(usNumTemp*ucDataLen);								//�����Ч����
			usPointNumTemp-=usNumTemp;
		}
		stTempTime=stTempTailTime;										//����ʱ��
	}
	//�����������ݿ�������õ����ݿ��ַȡ����
//	usStartPage = CON_DFPAGE_HEAD_DATABLOCK + CON_DFPAGE_DATABLOCK_PAGENUM*ucTaskNo;
	//���ݶ�ȡ��ʱ����������DATAFLASH���ݿ鿪ʼ�ĵ�λ�ã��������һ���㣬��Ҫ����
	ulTemp=TimeConver(&stTempTime,stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit)
		- TimeConver(&(stGRDefineArea.stGRTask[ucTaskNo].stStartActive), stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit);
	usOutputPos=(unsigned short)(ulTemp/stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeInter);
//azh 160311---
    //Ŀǰ�������Ч���ݵ���
	if(stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum >= stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos)
	{
	    usBufferMaxPoint = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum - stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos;
	}
	else
	{
	    usBufferMaxPoint = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum + stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum - stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos;
	}        
    
    if(usOutputPos < usBufferMaxPoint)//azh 160311 ��ȡ�㻹����Ч���ݷ�Χ֮��
    {
        usOutputPos = usOutputPos + stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos;
        if(usOutputPos >= stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum)
        {
            usOutputPos -= stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum;//��������ʱ�����
        }    

        if(usOutputPos <= stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum)//usBufferMaxPoint���õ��������ݵ�
        {    
            usBufferMaxPoint = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum;
        }
        else
        {   //azh Ϊ��ѭ��ʱ++�ñȽ����,����ֻҪС�ھͿ����ж���û�������Ҫ��Ȼ�и����ݱ�С������
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
    		//curve_task_di[]�����������ݱ���ֱ��ȡ������
            memcpy(ptr_tmp, pucBuffer, usLen);
    		memcpy(pucBuffer, ptr_tmp+32, 3);//CON_DI_TYPE_MPINSTANTPOW,		//˲ʱ�й�����3B
    		pucBuffer+=3;
    		memcpy(pucBuffer, ptr_tmp+44, 2);//CON_DI_TYPE_MPINSTANTVAR,		//˲ʱ�޹�����2B
    		pucBuffer+=2;
    		memcpy(pucBuffer, ptr_tmp+52, 6);//CON_DI_TYPE_MPVOLA,				//ABC���ѹ���ݿ�6B
    		pucBuffer+=6;
    		memcpy(pucBuffer, ptr_tmp+58, 9);//CON_DI_TYPE_MPVOLA,				//ABC��������ݿ�9B
    		pucBuffer+=9;
    		memcpy(pucBuffer, ptr_tmp, 4);//CON_DI_TYPE_MPDIRPOWTOTAL,		//�����й��ܵ���4B
    		pucBuffer+=4;
    		memcpy(pucBuffer, ptr_tmp+16, 4);//CON_DI_TYPE_MPVAR1,				//һ�����޹��ܵ���4B
    		pucBuffer+=4;
    		memcpy(pucBuffer, ptr_tmp+20, 4);//CON_DI_TYPE_MPVAR4,				//�������޹��ܵ���4B
    		pucBuffer+=4;
//--
    		usOutputPos+=ucResendDataInter;
    		memset(pucBuffer,CON_GRTASK_DATA_INVALID,usLen);

    		usOffset = usOutputPos;
    		if(usOffset >= stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum)
    		{    
    		    //azh����DATAFLASHѰַ���ܳ���һ��ѭ��
    		    usOffset = usOutputPos - stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum;
    		}
    	}
//azh 170925
        free_mem_block(ptr_tmp);//MEM_256_TYPE
    	
    }
//azh 160311---	
	(*pucDataLen)=ucDataLen;//azh Ҫ������ÿ�����ݵ�������ĳ���
	return ucRet;
}


//�����Ӧ���ݲ�ѯ��ȡ����ȡ�������
//ע��pucRetDataͷ���ֽڴ��ʵ�ʵ���������˳���������������㳤�ȹ̶�������������λ�ò��̶�
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
//	((unsigned short *)pucRetData)[0]=0;			//���ݿ��ŵ�����ռ��ǰ2���ֽ�
//	if(ucTaskNo>=CON_GRTASK_NUM)
//		return RET_ERROR;
////	pstGRTask=&(stGRDefineArea.stGRTask[ucTaskNo]);
//	if(ucResendDataInter==0)						//����Ƶ�ʲ���Ϊ0������ΪĬ��ֵ1
//		ucResendDataInter=1;
//	//�жϳ����Ƿ���Ч��Ӧ��С�ڿ�ʼʱ�䵽�������ݵ�ʱ��֮������ݵ���/������
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
//		usPointNumTemp=usBufferMaxPoint;			//��һ�ε��������ݻ��������ƣ��ϲ㺯�����Է�����ȡ
//	
//	pucBuffer=pucRetData+2;
//	usLen=stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem;
//	memset(pucBuffer,CON_GRTASK_DATA_INVALID,CON_TASK_DATA_MAXBUFFER-2);
////	usStartPage=CON_DFPAGE_HEAD_DATABLOCK + CON_DFPAGE_DATABLOCK_PAGENUM*ucTaskNo;
//	usOutputPos=stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum-usPointNumTemp*ucResendDataInter+ucResendDataInter-1;	//usPointNumTemp,��֤����һ�㱻��ȡ��usPointNum�޷���֤
//	//����������ȡ�����յ�����ȡ����ȡ�ٶ�Ҳ�б�֤�����ڼ����ȡҲ����ʵ��
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
//		((unsigned short *)pucRetData)[0]=i;		//���ݿ��ŵ�����ռ��ǰ2���ֽ�
//		//�������µ����ݵ�ʱ��
//		*pstNewestTime=stGRDefineArea.stGRTask[ucTaskNo].stStartActive;		//��ʼʱ�� �� �Ѿ�ִ�е�ʱ��
//		AddTime(pstNewestTime,
//			(stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum-1) * stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeInter,
//			stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit);
//		(*pusOnePointLen)=stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem;
//	}
//	return ucRet;
//}

//����ִ��ʱ����������ʱ�����Լ����͵�ѡ��Ƶ�ʻ�ȡ�������͵ĵ���(��������)
//���ͼ��Ϊ�µ����񣬲������������͵������Ҳ���ǣ��������Ϊ�գ����ͼ��Ϊ�µ���������账������ֻ�ܴ����¡��µ����
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
//		usPointNum=1;		//��������һ��
//	
//	return usPointNum;
//}

//���ִ��ʱ��
STDATETIME GetTaskNextExeTime(unsigned char ucTaskNo)
{
	STDATETIME stNextExeTime;
	unsigned long ulCurrTime,ulNextExeTime,ulTemp;
	stNextExeTime=GetCurrentTime();
	if(Is_DateTime_Availble(&stNextExeTime)!=RET_SUCCESS)
	{    
		stNextExeTime.ucYear = 0x0c;//azh �����е�ʱ���ϵ绹û��ͬ����ʱ�����Ե�һ�θ�һ����ʼֵ
		stNextExeTime.ucMonth = 0x0c;
		stNextExeTime.ucDay = 0x01;
		return stNextExeTime;
	}
	ulCurrTime=TimeConver(&stNextExeTime,CON_TIMEUNIT_SEC);		//��õ�ǰʱ��
	
	stNextExeTime=Task_FixNextTime(stNextExeTime,	stGRDefineArea.stGRTask[ucTaskNo].stTITaskStart,
		stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter);
	
	ulNextExeTime=TimeConver(&stNextExeTime,CON_TIMEUNIT_SEC);
	if(stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeInter==0)
		return stNextExeTime;
	ulTemp=ulNextExeTime;
	while((ulCurrTime+20)>=ulNextExeTime)
	{
		//�����´�ִ��ʱ��Ϊ���ٵ�ǰʱ��20��֮��
		AddTime(&stNextExeTime,
			(unsigned long)stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeInter,
			stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit);
		ulNextExeTime=TimeConver(&stNextExeTime,CON_TIMEUNIT_SEC);		//��õ�ǰʱ��
		if(ulTemp==ulNextExeTime)
			break;
		else
			ulTemp=ulNextExeTime;
	}
	return stNextExeTime;
}

//�������ʱ��
STDATETIME GetTaskNextUpTime(unsigned char ucTaskNo)
{
	STDATETIME stNextUpTime;
	unsigned long ulCurrTime,ulNextUpTime,ulTemp;
	stNextUpTime=GetCurrentTime();
	if(Is_DateTime_Availble(&stNextUpTime)!=RET_SUCCESS)
		return stNextUpTime;
	ulCurrTime=TimeConver(&stNextUpTime,CON_TIMEUNIT_SEC);		//��õ�ǰʱ��
	
	stNextUpTime=Task_FixNextTime(stNextUpTime,	stGRDefineArea.stGRTask[ucTaskNo].stTIResendStart,
		stGRDefineArea.stGRTask[ucTaskNo].stTIReSendInter);
	
	ulNextUpTime=TimeConver(&stNextUpTime,CON_TIMEUNIT_SEC);		
	if(stGRDefineArea.stGRTask[ucTaskNo].stTIReSendInter.ucTimeInter!=0)
	{
		ulTemp=ulNextUpTime;
		while((ulCurrTime+20)>=ulNextUpTime)
		{//�����´�����ʱ��Ϊ���ٵ�ǰʱ��20��֮��
			AddTime(&stNextUpTime,
				(unsigned long)stGRDefineArea.stGRTask[ucTaskNo].stTIReSendInter.ucTimeInter,
				stGRDefineArea.stGRTask[ucTaskNo].stTIReSendInter.ucTimeUnit);
			ulNextUpTime=TimeConver(&stNextUpTime,CON_TIMEUNIT_SEC);		//��õ�ǰʱ��
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
	{//��
		stDateTime.ucMinute	=stStartTimeInfo.ucTimeInter;
		if( stInterTimeInfo.ucTimeUnit==CON_TIMEUNIT_DAY )
		{//���ͼ����λΪ��
			stDateTime.ucHour	=0;
		}
		else if( stInterTimeInfo.ucTimeUnit==CON_TIMEUNIT_MON )
		{//���ͼ����λΪ��
			stDateTime.ucDay	=1;
			stDateTime.ucHour	=0;
		}
	}
	else if( stStartTimeInfo.ucTimeUnit==CON_TIMEUNIT_HOUR )
	{//ʱ
		stDateTime.ucHour	=stStartTimeInfo.ucTimeInter;
		stDateTime.ucMinute	=0;
		if( stInterTimeInfo.ucTimeUnit==CON_TIMEUNIT_MON )
		{//���ͼ����λΪ��
			stDateTime.ucDay=1;
		}
	}
	else if( stStartTimeInfo.ucTimeUnit==CON_TIMEUNIT_DAY )
	{//��
		stDateTime.ucDay	=stStartTimeInfo.ucTimeInter;
		stDateTime.ucHour	=0;
		stDateTime.ucMinute	=0;
	}
	else if( stStartTimeInfo.ucTimeUnit==CON_TIMEUNIT_MON )
	{//��
		stDateTime.ucMonth	=stStartTimeInfo.ucTimeInter;
		stDateTime.ucDay	=1;
		stDateTime.ucHour	=0;
		stDateTime.ucMinute	=0;
	}
	stDateTime.ucSecond		=0;
	return stDateTime;
}

/****************************************��������ȡֵ**********************************************/
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
		stDIInfoTmp=Main_DI_Data_Query_Manage(usDITemp);			//��ѯ
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
	stDIInfoTmp=Main_DI_Data_Query_Manage(usDI_Sml);			//��ѯ
	if(stDIInfoTmp.ucHave==CON_DI_NOHAVE)
	{
		pucDes[0]=0xEE;
		return RET_ERROR;
	}
	switch(stDIInfoTmp.usBigDI)
	{
	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_BLOCK:				//��������ʱ��
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_BLOCK:
	case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_BLOCK:
	case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_BLOCK:
		pucDes[0]=(unsigned char)HEX_TO_BCD(pucSrc[4],1);
		pucDes[1]=(unsigned char)HEX_TO_BCD(pucSrc[3],1);
		pucDes[2]=(unsigned char)HEX_TO_BCD(pucSrc[2],1);
		pucDes[3]=(unsigned char)HEX_TO_BCD(pucSrc[1],1);
		break;
	case CON_DI_TYPE_MPFEERATE_BLOCK:							//����	
		memcpy(pucDes,pucSrc,stDIInfoTmp.ucContentLen);
		break;
	default:													//����
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

//�������������Ƿ���Ч
unsigned char TaskCheckMeterIsValid(unsigned char ucMeterNo)
{
	unsigned char ucRet=RET_SUCCESS;
	if(stGRDefineArea.stGRTask[ucMeterNo].ucActive==CON_GRTASK_NOACTIVE)
		return RET_ERROR;
	if(Task_Check_Task_BaseInfo(ucMeterNo)==RET_ERROR)
		ucRet=RET_ERROR;
	return ucRet;
}

//��8�飬�����б����ݣ��ֱ�ռ��1������
unsigned char DeleteDefaultMeterTask(unsigned char ucMeterNo)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucTaskNo;
	ucTaskNo=ucMeterNo;
	Task_Clear(ucTaskNo);
	return ucRet;
}

//����Ĭ�ϱ�����//azh �������� ���浽FLASH
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
/*	if(stTerAdjPara.ucRunMode==0xA5)			//����ģʽ
		pstGRTaskTmp->stTITaskStart.ucTimeInter=2;    //ע������ʱ���
	else
		pstGRTaskTmp->stTITaskStart.ucTimeInter=5;    //����ģʽ*/
	
	pstGRTaskTmp->stTITaskStart.ucTimeInter=0;
	pstGRTaskTmp->stTITaskStart.ucTimeUnit=CON_TIMEUNIT_MIN;
	pstGRTaskTmp->stTITaskInter.ucTimeInter=15;
	pstGRTaskTmp->stTITaskInter.ucTimeUnit=CON_TIMEUNIT_MIN;
	pstGRTaskTmp->stTIResendStart.ucTimeInter=1;					
	pstGRTaskTmp->stTIResendStart.ucTimeUnit=CON_TIMEUNIT_MIN;
	pstGRTaskTmp->stTIReSendInter.ucTimeInter=0;					//0��ʾ����������
	pstGRTaskTmp->stTIReSendInter.ucTimeUnit=0;
	pstGRTaskTmp->ucResendDataInter=1;							//�������ݵ�Ƶ��		
	pstGRTaskTmp->usPerTaskRetMem=0;								//����������Ҫ���ڴ���
	pstGRTaskTmp->usSavePointNum=CON_GRTASK_DATAPOINT_NUM;		//����ı������15����һ��
	pstGRTaskTmp->usExeNum=0;
	//	pstGRTaskTmp->stRelayTaskPara;								//�м̲���
	
// 	m=0;
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPDIRPOWTOTAL;		//�����й��ܵ���
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPDIRPOWTOTAL>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPDIRVARTOTAL;		//�����޹��ܵ���
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPDIRVARTOTAL>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPUNDIRPOWTOTAL;		//�����й��ܵ���
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPUNDIRPOWTOTAL>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPUNDIRVARTOTAL;		//�����޹��ܵ���
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPUNDIRVARTOTAL>>8);
// 	
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVAR1;				//һ�����޹��ܵ���
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVAR1>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVAR4;				//�������޹��ܵ���
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVAR4>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVAR2;				//�������޹��ܵ���
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVAR2>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVAR3;				//�������޹��ܵ���
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVAR3>>8);
// 	
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTPOW;		//˲ʱ�й�����
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTPOW>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTPOWA;		//A��˲ʱ�й�����
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTPOWA>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTPOWB;		//B��˲ʱ�й�����
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTPOWB>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTPOWC;		//C��˲ʱ�й�����
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTPOWC>>8);
// 	
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTVAR;		//˲ʱ�޹�����
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTVAR>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTVARA;		//A��˲ʱ�޹�����
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTVARA>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTVARB;		//B��˲ʱ�޹�����
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTVARB>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTVARC;		//C��˲ʱ�޹�����
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTVARC>>8);
// 	
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVOLA;				//A���ѹ
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVOLA>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVOLB;				//B���ѹ
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVOLB>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVOLC;				//C���ѹ
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVOLC>>8);
// 	
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPCURA;				//A�����
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPCURA>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPCURB;				//B�����
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPCURB>>8);
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPCURC;				//C�����
// 	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPCURC>>8);
	
	pstGRTaskTmp->ucDataItemNum=CON_TASK_CURVE_DI_NUM;//m>>1;								//��ͨ���������������
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

//����ܼ��������Ƿ���Ч
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

//�����ܼ�Ĭ������
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
///*	if(stTerAdjPara.ucRunMode==0xA5)				//����ģʽ
//		stGRTaskTmp.stTITaskStart.ucTimeInter=2;
//	else
//		stGRTaskTmp.stTITaskStart.ucTimeInter=5;	//����ģʽ*/
//	stGRTaskTmp.stTITaskStart.ucTimeInter=0;			//100330
//	stGRTaskTmp.stTITaskStart.ucTimeUnit=CON_TIMEUNIT_MIN;
//	stGRTaskTmp.stTITaskInter.ucTimeInter=15;
//	stGRTaskTmp.stTITaskInter.ucTimeUnit=CON_TIMEUNIT_MIN;
//	stGRTaskTmp.stTIResendStart.ucTimeInter=1;					
//	stGRTaskTmp.stTIResendStart.ucTimeUnit=CON_TIMEUNIT_MIN;
//	stGRTaskTmp.stTIReSendInter.ucTimeInter=0;					//0��ʾ����������
//	stGRTaskTmp.stTIReSendInter.ucTimeUnit=0;
//	stGRTaskTmp.ucResendDataInter=1;							//�������ݵ�Ƶ��		
//	stGRTaskTmp.usPerTaskRetMem=0;								//����������Ҫ���ڴ���
//	stGRTaskTmp.usSavePointNum=CON_GRTASK_DATAPOINT_NUM;
//	stGRTaskTmp.usExeNum=0;
//	m=0;
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTPOW1;		//�й������ܼ���1
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTPOW1>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTPOW2;		//�й������ܼ���2
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTPOW2>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTPOW3;		//�й������ܼ���3
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTPOW3>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTPOW4;		//�й������ܼ���4
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTPOW4>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTPOW5;		//�й������ܼ���5
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTPOW5>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTPOW6;		//�й������ܼ���6
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTPOW6>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTPOW7;		//�й������ܼ���7
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTPOW7>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTPOW8;		//�й������ܼ���8
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTPOW8>>8);
//
//	stGRTaskTmp.ucDataItemNum=m>>1;	
//	ucTaskNo=CON_GRTASK_SUMGROUP_INSTANTPOW_TASKNO;
//	memcpy(&stGRDefineArea.stGRTask[ucTaskNo],&stGRTaskTmp,sizeof(STGRTASK));
//	Task_Config(ucTaskNo,CON_GRTASK_ACTIVE);
//	
//	m=0;
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTVAR1;		//�޹������ܼ���1
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTVAR1>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTVAR2;		//�޹������ܼ���2
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTVAR2>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTVAR3;		//�޹������ܼ���3
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTVAR3>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTVAR4;		//�޹������ܼ���4
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTVAR4>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTVAR5;		//�޹������ܼ���5
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTVAR5>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTVAR6;		//�޹������ܼ���6
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTVAR6>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTVAR7;		//�޹������ܼ���7
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTVAR7>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMINSTANTVAR8;		//�޹������ܼ���8
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMINSTANTVAR8>>8);
//
//	stGRTaskTmp.ucDataItemNum=m>>1;	
//	ucTaskNo=CON_GRTASK_SUMGROUP_INSTANTVAR_TASKNO;
//	memcpy(&stGRDefineArea.stGRTask[ucTaskNo],&stGRTaskTmp,sizeof(STGRTASK));
//	Task_Config(ucTaskNo,CON_GRTASK_ACTIVE);
//	
//	m=0;
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMPOW1;		//�й��ܼ���1
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMPOW1>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMPOW2;		//�й��ܼ���2
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMPOW2>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMPOW3;		//�й��ܼ���3
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMPOW3>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMPOW4;		//�й��ܼ���4
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMPOW4>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMPOW5;		//�й��ܼ���5
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMPOW5>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMPOW6;		//�й��ܼ���6
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMPOW6>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMPOW7;		//�й��ܼ���7
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMPOW7>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMPOW8;		//�й��ܼ���8
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMPOW8>>8);
//
//	stGRTaskTmp.ucDataItemNum=m>>1;	
//	ucTaskNo=CON_GRTASK_SUMGROUP_POW_TASKNO;
//	memcpy(&stGRDefineArea.stGRTask[ucTaskNo],&stGRTaskTmp,sizeof(STGRTASK));
//	Task_Config(ucTaskNo,CON_GRTASK_ACTIVE);
//
//	m=0;
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMVAR1;		//�޹��ܼ���1
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMVAR1>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMVAR2;		//�޹��ܼ���2
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMVAR2>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMVAR3;		//�޹��ܼ���3
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMVAR3>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMVAR4;		//�޹��ܼ���4
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMVAR4>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMVAR5;		//�޹��ܼ���5
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMVAR5>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMVAR6;		//�޹��ܼ���6
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMVAR6>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMVAR7;		//�޹��ܼ���7
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMVAR7>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_SUMVAR8;		//�޹��ܼ���8
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_SUMVAR8>>8);
//
//	stGRTaskTmp.ucDataItemNum=m>>1;	
//	ucTaskNo=CON_GRTASK_SUMGROUP_VAR_TASKNO;
//	memcpy(&stGRDefineArea.stGRTask[ucTaskNo],&stGRTaskTmp,sizeof(STGRTASK));
//	Task_Config(ucTaskNo,CON_GRTASK_ACTIVE);
//
//	return ucRet;
//}

//ɾ���ܼ��������Ƿ���Ч
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

//������������Ƿ���Ч
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

//��������Ĭ������
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
//	stGRTaskTmp.stTIReSendInter.ucTimeInter=0;					//0��ʾ����������
//	stGRTaskTmp.stTIReSendInter.ucTimeUnit=0;
//	stGRTaskTmp.ucResendDataInter=1;							//�������ݵ�Ƶ��		
//	stGRTaskTmp.usPerTaskRetMem=0;								//����������Ҫ���ڴ���
//	stGRTaskTmp.usSavePointNum=CON_GRTASK_DATAPOINT_NUM;
//	stGRTaskTmp.usExeNum=0;
//	m=0;
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPDIRPOWTOTAL;		//�����й��ܵ���
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPDIRPOWTOTAL>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPDIRVARTOTAL;		//�����޹��ܵ���
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPDIRVARTOTAL>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPUNDIRPOWTOTAL;		//�����й��ܵ���
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPUNDIRPOWTOTAL>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPUNDIRVARTOTAL;		//�����޹��ܵ���
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPUNDIRVARTOTAL>>8);
//
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTPOW;		//˲ʱ�й�����
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTPOW>>8);
//	stGRTaskTmp.ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTVAR;		//˲ʱ�޹�����
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

//ɾ����������
//unsigned char DeleteDefaultPulseTask(unsigned char ucPulseNo)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucTaskNo;
//	ucTaskNo=Task_GetTaskNo_byPulseNo(ucPulseNo);
//	Task_Clear(ucTaskNo);
//	return ucRet;
//}

//����ն������Ƿ���Ч
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

//�����ն�Ĭ������
unsigned char SetDefaultTerTask(void)
{
	unsigned char ucRet=RET_SUCCESS;
//azh һ��������һ����������130123
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
//	pstGRTaskTmp->stTIReSendInter.ucTimeInter=0;					//0��ʾ����������
//	pstGRTaskTmp->stTIReSendInter.ucTimeUnit=0;
//	pstGRTaskTmp->ucResendDataInter=1;							//�������ݵ�Ƶ��		
//	pstGRTaskTmp->usPerTaskRetMem=0;								//����������Ҫ���ڴ���
//	pstGRTaskTmp->usSavePointNum=CON_GRTASK_DATAPOINT_NUM;
//	pstGRTaskTmp->usExeNum=0;
////	m=0;
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPDIRPOWTOTAL;		//�����й��ܵ���
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPDIRPOWTOTAL>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPDIRVARTOTAL;		//�����޹��ܵ���
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPDIRVARTOTAL>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPUNDIRPOWTOTAL;		//�����й��ܵ���
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPUNDIRPOWTOTAL>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPUNDIRVARTOTAL;		//�����޹��ܵ���
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPUNDIRVARTOTAL>>8);
////
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVAR1;				//һ�����޹��ܵ���
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVAR1>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVAR4;				//�������޹��ܵ���
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVAR4>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVAR2;				//�������޹��ܵ���
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVAR2>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVAR3;				//�������޹��ܵ���
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVAR3>>8);
////	
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTPOW;		//˲ʱ�й�����
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTPOW>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTPOWA;		//A��˲ʱ�й�����
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTPOWA>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTPOWB;		//B��˲ʱ�й�����
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTPOWB>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTPOWC;		//C��˲ʱ�й�����
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTPOWC>>8);
////	
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTVAR;		//˲ʱ�޹�����
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTVAR>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTVARA;		//A��˲ʱ�޹�����
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTVARA>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTVARB;		//B��˲ʱ�޹�����
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTVARB>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPINSTANTVARC;		//C��˲ʱ�޹�����
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPINSTANTVARC>>8);
////
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVOLA;				//A���ѹ
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVOLA>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVOLB;				//B���ѹ
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVOLB>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPVOLC;				//C���ѹ
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPVOLC>>8);
////	
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPCURA;				//A�����
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPCURA>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPCURB;				//B�����
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)(CON_DI_TYPE_MPCURB>>8);
////	pstGRTaskTmp->ucDataArea[m++]=(unsigned char)CON_DI_TYPE_MPCURC;				//C�����
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

//ɾ���ն�����
unsigned char DeleteDefaultTerTask(void)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned char ucTaskNo;
	ucTaskNo=Task_GetTaskNo_Ter();
	Task_Clear(ucTaskNo);
	return ucRet;
}

//��������
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

//�����Ӧ��������
//Ŀǰ����Ϊǰ��·�����ṩ����洢�ռ�
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

//�ն˶�Ӧ��������
unsigned char Task_GetTaskNo_Ter(void)
{
	return CON_GRTASK_TER_TASKNO;
}

//��������������
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
** ��������: void Save_Curve_Task_Para(unsigned char ucTaskNo, unsigned char ucOffsetAddr, unsigned char type)
** ��������: ��������(��Ҫ����)�������в���
** ��ڲ���: ucOffsetAddr������ƫ�Ƶ�ַ type����������黹�ǵ������
** �� �� ֵ: ��
** ˵    ��: ��
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
    //У�����ҪĬ�ϲ�������!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
               
    if(type==CON_TASK_PARA_BLOCK_SAVE)//���ݿ��ʱ�� ����������ݿ�ƫ���׵�ַ
    {    
        switch(ucOffsetAddr)
        {
            case CON_TASK_BLK_OFST:                    //ucTaskType//��������        
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
            case CON_TASK_TYPE_OFST:              //ucTaskType//��������
                len = CON_TASK_TYPE_LENTH; 
                offset = ucOffsetAddr - CON_TASK_TYPE_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTask[ucTaskNo].ucTaskType, len);
                break;
            case CON_TASK_MPNO_OFST:              //ucMPNo��ͨ����Ĳ������&�м�����Ķ˿ں�
                len = CON_TASK_MPNO_LENTH;
                offset = ucOffsetAddr - CON_TASK_TYPE_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTask[ucTaskNo].ucMPNo, len);
                break;
            case CON_TASK_SAVENUM_OFST:           //usSavePointNum����ı������(��ʹ���趨ֵ��ʹ����������õ�ֵ)
                len = CON_TASK_SAVENUM_LENTH;
                offset = ucOffsetAddr - CON_TASK_TYPE_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum, len);
                break;
            case CON_TASK_PERRETLEN_OFST:         //usPerTaskRetMem����ִ��һ��������ֽ���
                len = CON_TASK_PERRETLEN_LENTH;
                offset = ucOffsetAddr - CON_TASK_TYPE_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem, len);
                break;
            case CON_TASK_ACTIVE_OFST:            //ucActive//����״̬������
                len = CON_TASK_ACTIVE_LENTH;
                offset = ucOffsetAddr - CON_TASK_TYPE_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTask[ucTaskNo].ucActive, len);
                break;
            case CON_TASK_STARTACTIVE_OFST:       //stStartActive//����ʼ�ʱ��
                len = CON_TASK_STARTACTIVE_LENTH;
                offset = ucOffsetAddr - CON_TASK_TYPE_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTask[ucTaskNo].stStartActive, len);
                break;
//
            case CON_TASKIF_EXEDNUM_OFST:         //usExedNum//����ִ���˴���
                len = CON_TASKIF_EXEDNUM_LENTH;
                offset = ucOffsetAddr - CON_TASKIF_EXEDNUM_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum, len);
                break;
            case CON_TASKIF_TAILPOS_OFST:         //usTailPos//��������ݵ㣬������ѭ���洢usSavePointNum����
                len = CON_TASKIF_TAILPOS_LENTH;
                offset = ucOffsetAddr - CON_TASKIF_EXEDNUM_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos, len);
                break;      
            case CON_TASKIF_TAILTIME_OFST:        //ulTailTime//��������ݵ��Ӧ��ʱ�䣬��1990�꿪ʼ����λ����
                len = CON_TASKIF_TAILTIME_LENTH;
                offset = ucOffsetAddr - CON_TASKIF_EXEDNUM_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTaskInfo[ucTaskNo].ulTailTime, len);
                break;
//
            case CON_TASKRIF_NEXTEXTIME_OFST:     //stNextExeTime//�´�ִ��ʱ��(����)
                len = CON_TASKRIF_NEXTEXTIME_LENTH;
                offset = ucOffsetAddr - CON_TASKRIF_NEXTEXTIME_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTaskRunInfo[ucTaskNo].stNextExeTime, len);
                break;
            case CON_TASKRIF_NEXTUPTIME_OFST:     //stNextUpTime//�´���������ʱ��
                len = CON_TASKRIF_NEXTUPTIME_LENTH;
                offset = ucOffsetAddr - CON_TASKRIF_NEXTEXTIME_OFST;
                memcpy(mem_ptr+offset, (unsigned char*)&stGRDefineArea.stGRTaskRunInfo[ucTaskNo].stNextUpTime, len);
                break;
            case CON_TASKRIF_HEADPOS_OFST:        //usHeadPos//��ǰ�洢�ĵ��
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
** ��������: void PowOn_Get_Curve_Task_Para(void)
** ��������: �ϵ��eeprom�е���������ز���
** ��ڲ���: 
** �� �� ֵ: ��
** ˵    ��: ��
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
//ֻ�����в���У������ȷ��Ҫ��������
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
//û�б���Ĳ����ǹ̶�ֵ
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
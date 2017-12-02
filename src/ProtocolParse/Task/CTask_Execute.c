/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : CTask_Execute.c
 Description    : ��������ģ������ִ��
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-08-15    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

//////////////////////////////// ����ʵ�� //////////////////////////////////////
extern const U16 curve_task_di[CON_TASK_CURVE_DI_NUM];
//���ߴ���ģ���ʼ��
unsigned char CTask_Proc_Init()
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Task_Manage_Init();									//�����ʼ��
    memset(&stGRTaskExeInfo, 0, sizeof(STGRTASKEXEINFO));
	stGRTaskExeInfo.ucTaskValidFlag=CON_TASK_RETDATA_INVALID;	//����״̬��Ϣ��ʼ��
	return ucRet;
}

//���������б��е��������ҵ����ʵ���Ҫ����������ʱ������д���
//��������
//����������������
//���»��������������
//�л������ִ��ʱ�䵽������������ִ��
//Ϊ�������񷵻����ݵĴ�����һ��ִֻ��һ������֪���������������
unsigned char CTask_Proc_Down_Timer()
{
	unsigned char ucRet=RET_ERROR;

	Task_Proc();//azh ���������ڳ������ݺ�

	return ucRet;
}

//��ʱ��������Ӧ�����е����ݣ�����Ҫ����ʱ������Ӧ�����ݼ�������ϴ�����վ
//azh ���������������������� �����Ĳ�����
//unsigned char CTask_Proc_Up_Timer()
//{
//	unsigned char ucRet=RET_SUCCESS,i;
//	for(i=0;i<CON_GRTASK_NUM;i++)
//	{
//		if(Task_RequireUp(i)==RET_GRTASK_NEEDPROC)
//			Task_UpSendRecord(i);
//	}
//	return ucRet;
//}

//����������еõ����񣬽���ת����дRS485���Ͷ��� 
unsigned char Task_Proc()
{
	unsigned char	ucRet=RET_SUCCESS,i=0;

//��������������
    if(stGRTaskExeInfo.ucTaskValidFlag == CON_TASK_RETDATA_INVALID)
    {    

    	for(i=0;i<CON_GRTASK_NUM;i++)
    	{
    		stGRTaskExeInfo.ucNowExeTaskNo++;
    		if(stGRTaskExeInfo.ucNowExeTaskNo>=CON_GRTASK_NUM)
    			stGRTaskExeInfo.ucNowExeTaskNo=0;
    		
    		if( Task_RequireProc(stGRTaskExeInfo.ucNowExeTaskNo)==RET_GRTASK_NEEDPROC )
    		{
    			stGRTaskExeInfo.ucTaskValidFlag=CON_TASK_RETDATA_VALID;			//����ִ�б�־
    			gucOneLoopFlag = 1;
    			memset(&stGRDefineArea.stGRTaskRetVal,0,sizeof(STGRTASKRETVAL));
    			
    			if( stGRDefineArea.stGRTask[stGRTaskExeInfo.ucNowExeTaskNo].ucTaskType==CON_GRTASK_TYPE_RELAY )
    			{//	
    				;//Task_Proc_Relay(stGRTaskExeInfo.ucNowExeTaskNo);			//�м�������
    				stGRTaskExeInfo.ucTaskValidFlag=CON_TASK_RETDATA_INVALID;//azh
    			}
    			else if( stGRDefineArea.stGRTask[stGRTaskExeInfo.ucNowExeTaskNo].ucTaskType==CON_GRTASK_TYPE_NORMAL )
    			{
    				Task_Proc_Normal(stGRTaskExeInfo.ucNowExeTaskNo);			//��ͨ������
    			}
    			else
    			{
    			    stGRTaskExeInfo.ucTaskValidFlag=CON_TASK_RETDATA_INVALID;
    			}
    			break;
    		}
    	}
    }
	return ucRet;
}

//�����м�����
//unsigned char Task_Proc_Relay(unsigned char ucTaskNo)
//{
//	return RET_SUCCESS;
//}

//������ͨ����
unsigned char Task_Proc_Normal(unsigned char ucTaskNo)
{
	unsigned char	ucRet=RET_SUCCESS,ucMPNo=0,i=0;
	unsigned short	usDI;
	STDIINFO		stDIInfoTmp;
	unsigned char	ucMPType = CON_MPTYPE_NULL;
	
	ucMPNo=stGRDefineArea.stGRTask[ucTaskNo].ucMPNo;//������

	stGRDefineArea.stGRTaskRetVal.ucTaskNo		=ucTaskNo;
	stGRDefineArea.stGRTaskRetVal.stRetDateTime	=GetCurrentTime();
	stGRDefineArea.stGRTaskRetVal.ucDINum		=stGRDefineArea.stGRTask[ucTaskNo].ucDataItemNum;
	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
	{
//		usDI=stGRDefineArea.stGRTask[ucTaskNo].ucDataArea[i*2]+stGRDefineArea.stGRTask[ucTaskNo].ucDataArea[i*2+1]*0x100;
        usDI = curve_task_di[i];
		stDIInfoTmp=Main_DI_Data_Query_Manage(usDI);
		stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI		=usDI;
		stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDILen	=stDIInfoTmp.ucContentLen;
//		stGRDefineArea.stGRTaskRetVal.stDIVal[i].ucDIVAL[0]	=CON_GRTASK_DATA_INVALID;
	}
    memset(stGRDefineArea.stGRTaskRetVal.ucTaskData, CON_GRTASK_DATA_INVALID, CON_GRTASK_TaskDateMaxDateLen);//512
	
	ucMPType=RunPara_GetMPType(ucMPNo);
//	if(ucMPNo==CON_MPNO_SUMG_FIXED)								//�������ն�������������
//		GetData_From_Ter_SumG();
//	else
//	{
		if( ucMPType == CON_MPTYPE_METER )
		{
			for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
			{
				ucRet = Fill_TaskData_Manage(stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI,CONRS485SOUR_GRTASK_UNREAD,ucMPNo);
//azh �����������������ֱ�ӴӶ�ʱˢ�������������� �����������������ȡ����  �Ͳ�������ȥ����
//				if(ucRet!=RET_SUCCESS)
//				{
//					stDIInfoTmp=Main_DI_Data_Query_Manage(stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI);
//					if( stDIInfoTmp.ucHave==CON_DI_HAVE && stDIInfoTmp.ucType==CON_DI_TYPE_DATA && 
//						stDIInfoTmp.ucMPType==CON_DI_MPTYPE_NORMAL && stDIInfoTmp.ucReadMeterEnabel==CON_DI_OPERATE_ENABLE &&
//						stDIInfoTmp.usBigDI != CON_DI_TYPE_INVALID)
//					{
//						Fill_MeterTask(stDIInfoTmp.usBigDI,CONRS485SOUR_GRTASK,ucMPNo,ucRS485TaskGroupListNo);
//					}
//				}
			}
			Fill_TaskData();//azh ��Ϊ�Ǵ�ȫ���������ȡ����� ����������ֱ�Ӵ����� ������Լ������� ���ڳ������һ������ʱ����
            stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
//		}
//		else if(ucMPType==CON_MPTYPE_PULSE || ucMPType==CON_MPTYPE_TER)	//�ն˻�������ֱ�Ӷ�ȡ
//		{
//			for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//				ucRet = Fill_TaskData_Manage(stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI,CONRS485SOUR_GRTASK_UNREAD,ucMPNo);
//		}
	}
	else
	{
	    stGRTaskExeInfo.ucTaskValidFlag=CON_TASK_RETDATA_INVALID;    
	}
	
	return ucRet;
}

//�ѻ�Ӧ������д������Ĵ洢��(Flash��)
unsigned char Fill_TaskData()
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char	ucRet=RET_SUCCESS;
	unsigned char	ucTaskNo=0;
	unsigned char 	*pucGRTaskRetBuf;
	unsigned short	usGRTaskRetBufLen=0;
//	unsigned long 	ulTemp;
	STDATETIME 		stTempTime;
	unsigned short 	usNumtask_Per_Block ;
	unsigned short 	usExedNum;
	unsigned long	ulTime_Tmp;
	ucTaskNo=stGRDefineArea.stGRTaskRetVal.ucTaskNo;
	
	if( ucTaskNo<CON_GRTASK_NUM && 
		stGRDefineArea.stGRTask[ucTaskNo].ucTaskType!=CON_GRTASK_TYPE_INVALID && 
		stGRDefineArea.stGRTask[ucTaskNo].ucActive==CON_GRTASK_ACTIVE )
	{
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }	
		pucGRTaskRetBuf=(unsigned char *)mem_ptr;			//����������
		memset(pucGRTaskRetBuf,0xEE,512);
		
		usGRTaskRetBufLen=Make_TaskRet_String(&(stGRDefineArea.stGRTaskRetVal),pucGRTaskRetBuf);
		
		//�������񷵻�����
		Task_SaveRetVal(ucTaskNo,pucGRTaskRetBuf,usGRTaskRetBufLen);
		//����������Ϣ����
		stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum++;
		Save_Curve_Task_Para(ucTaskNo, CON_TASKIF_EXEDNUM_OFST, CON_TASK_PARA_SINGLE_SAVE);
		
		usNumtask_Per_Block = MCP_FLASH_BLOCK_SIZE / stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem ;//һ���ܷż���
		usExedNum = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum % (usNumtask_Per_Block * SECTORNUM_PERTASK);
		
		if(usExedNum % usNumtask_Per_Block == 0)
		{//һ��BLOCK�Ѿ�д��������һ����� azh ������ʵֻ��һ��sector
//			Flash_Eraseblock_MCP((MCP_FLASH_TASK_START_BLOCK + ucTaskNo * SECTORNUM_PERTASK + (usExedNum / usNumtask_Per_Block)) * MCP_FLASH_BLOCK_SIZE);
            MCP_Flash_Sector_Erase((MCP_FLASH_TASK_START_BLOCK + ucTaskNo * SECTORNUM_PERTASK + (usExedNum / usNumtask_Per_Block)) * MCP_FLASH_BLOCK_SIZE);
			//HdConvertWatchDog();
			if((stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos / usNumtask_Per_Block) == (usExedNum / usNumtask_Per_Block))
			{	//βָ����Ҫ�����Ŀ����Ҫ������
				stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos = ((usExedNum / usNumtask_Per_Block + 1) % SECTORNUM_PERTASK) * usNumtask_Per_Block;
				//���ݿ��׵��Ӧʱ�����
				stTempTime=TimeConverToDataTime(stGRDefineArea.stGRTaskInfo[ucTaskNo].ulTailTime);
				ulTime_Tmp = (unsigned long)stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeInter;
				ulTime_Tmp = ulTime_Tmp * usNumtask_Per_Block;
				AddTime( &stTempTime,ulTime_Tmp,stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit );
				stGRDefineArea.stGRTaskInfo[ucTaskNo].ulTailTime=TimeConver(&stTempTime,CON_TIMEUNIT_SEC);
				Save_Curve_Task_Para(ucTaskNo, CON_TASKIF_BLK_OFST, CON_TASK_PARA_BLOCK_SAVE);
//azh 171011����ʾ�ն��Ϻϲ�����
                //azh 160311--- һblock������������ݿ�ʼ��ЧҲ��������Ч�����׵�ʱ��
    			AddTime(&stGRDefineArea.stGRTask[ucTaskNo].stStartActive,
    				ulTime_Tmp,	stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit);
    			Save_Curve_Task_Para(ucTaskNo, CON_TASK_STARTACTIVE_OFST, CON_TASK_PARA_SINGLE_SAVE);
                //azh 160311
        		if(stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum>=stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum)			//ִ�д����ܴ����Ҫ����һ��
        		{
        			stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum = 0;//д��һȦ�󣬻ظ�
        			Save_Curve_Task_Para(ucTaskNo, CON_TASKIF_EXEDNUM_OFST, CON_TASK_PARA_SINGLE_SAVE);
        		}
        		//azh 160311---
			}
		}
//azh 160311---		
//		if(stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum>0xFFF0)			//ִ�д����ܴ����Ҫ����һ��
//		{
//			ulTemp=(unsigned long)stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum;
//			stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum %= stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum;
//			Save_Curve_Task_Para(ucTaskNo, CON_TASKIF_EXEDNUM_OFST, CON_TASK_PARA_SINGLE_SAVE);
//			ulTemp=ulTemp-stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum;
//			AddTime(&stGRDefineArea.stGRTask[ucTaskNo].stStartActive,
//				(ulTemp*stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeInter),
//				stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit);
//			Save_Curve_Task_Para(ucTaskNo, CON_TASK_STARTACTIVE_OFST, CON_TASK_PARA_SINGLE_SAVE);
//		}
		
	    free_mem_block(mem_ptr);//
	}
	return ucRet;
}

unsigned short	Make_TaskRet_String(STGRTASKRETVAL *pstGRTaskRetVal,unsigned char *pucGRTaskRetBuf)
{
	unsigned short	m=0,i=0;
	for(i=0;i<pstGRTaskRetVal->ucDINum;i++)//azh û�б���ʱ����?
	{
		if(i >= CON_GRTASK_DINUM)
			break;
		if(m>=512)
		{
			m=CON_GRTASK_TaskDateMaxDateLen;//0xffff;
			break;
		}
//		memcpy(pucGRTaskRetBuf+m,pstGRTaskRetVal->stDIVal[i].ucDIVAL,pstGRTaskRetVal->stDIVal[i].ucDILen);
		m+=pstGRTaskRetVal->stDIVal[i].ucDILen;
	}
	memcpy(pucGRTaskRetBuf,pstGRTaskRetVal->ucTaskData, m);//azh �����ʱ���Ѿ���һ�������� 

	return m;
}

//�����Ӧ���ݲ�ѯ�����ͣ���дTerMsg
//unsigned char Fill_TaskData_to_TerMsg(STTERMSG *pstTerMsg,unsigned char ucTaskNo,unsigned short usPos,unsigned char ucSendTaskType)
//{
//	unsigned char ucRet=RET_SUCCESS;
///*
//	unsigned int nDataAddr=0;
//	unsigned char i=0,j=0,k=0;
//	unsigned char ucDataLen=0;
//	
//	STDATAITEM stDataItemTmp;
//	
//	stTerMsgList.ucTerMsgNum=0;
//	
//	if( pstTerMsg!=NULL && usPos<stGRDefineArea.stGRTaskList.stGRTask[ucTaskNo].usSavePointNum )
//	{
//		//������ʼ��ַ
//		nDataAddr=stGRDefineArea.stGRTaskList.stGRTaskInfo[ucTaskNo].nDataAddr+
//			usPos*stGRDefineArea.stGRTaskList.stGRTaskInfo[ucTaskNo].usPerTaskRetMem;		
//		//����ʱ��
//		memcpy( &(pstTerMsg->stOtherInfo.stStartTime),stGRDefineArea.stGRTaskRetVal.ucRetVal+nDataAddr,sizeof(STDATETIME) );		
//		pstTerMsg->stOtherInfo.ucDataNum=1;//����
//		nDataAddr+=6;	
//		//��������
//		if( stGRDefineArea.stGRTaskList.stGRTask[ucTaskNo].ucTaskType==CON_GRTASK_TYPE_RELAY )//�м�
//		{
//			ClearDataItem(&stDataItemTmp);
//			//��ʶ
//			stDataItemTmp.usItemLabel	=stGRDefineArea.stGRTaskRetVal.ucRetVal[nDataAddr++];
//			stDataItemTmp.usItemLabel	+=stGRDefineArea.stGRTaskRetVal.ucRetVal[nDataAddr++]*0x100;
//			//����
//			stDataItemTmp.ucDataLen		=stGRDefineArea.stGRTaskRetVal.ucRetVal[nDataAddr++];
//			//����
//			for(i=0;i<(stDataItemTmp.ucDataLen);i++)
//			{
//				if(i<=CON_DI_DATA_LEN)
//					stDataItemTmp.ucDataContent[i]=stGRDefineArea.stGRTaskRetVal.ucRetVal[nDataAddr++];
//			}
//			pstTerMsg->stDataItem[k++]=stDataItemTmp;
//		}
//		else if( stGRDefineArea.stGRTaskList.stGRTask[ucTaskNo].ucTaskType==CON_GRTASK_TYPE_NORMAL )//��ͨ����
//		{			
//			for(i=0;i<stGRDefineArea.stGRTaskList.stGRTask[ucTaskNo].ucDataItemNum;i++)
//			{
//				ClearDataItem( &stDataItemTmp );
//				//��ʶ
//				stDataItemTmp.usItemLabel	=stGRDefineArea.stGRTaskRetVal.ucRetVal[nDataAddr++];
//				stDataItemTmp.usItemLabel	+=stGRDefineArea.stGRTaskRetVal.ucRetVal[nDataAddr++]*0x100;
//				//����
//				stDataItemTmp.ucDataLen		=stGRDefineArea.stGRTaskRetVal.ucRetVal[nDataAddr++];
//				//����
//				for(j=0;j<stDataItemTmp.ucDataLen;j++)
//					stDataItemTmp.ucDataContent[j]=stGRDefineArea.stGRTaskRetVal.ucRetVal[nDataAddr++];
//				if( (ucDataLen+stDataItemTmp.ucDataLen)>96 || k>=16 )
//				{
//					pstTerMsg->ucDataItemNum=k;
//					AddTerMsgList(pstTerMsg,ucSendTaskType);
//					
//					//��ʼ��:ֻ����������������������г�ʼ��
//					ucDataLen=0;k=0;
//					pstTerMsg->ucDataItemNum=0;
//					for( j=0;j<CON_TERMSG_DI_NUM;j++ )
//						ClearDataItem( &(pstTerMsg->stDataItem[j]) );
//					pstTerMsg->stDataItem[k++]=stDataItemTmp;
//					ucDataLen+=stDataItemTmp.ucDataLen;
//				}
//				else
//				{
//					pstTerMsg->stDataItem[k++]=stDataItemTmp;
//					ucDataLen+=stDataItemTmp.ucDataLen;
//				}
//			}//for 
//		}
//		if( k>0 )
//		{
//			pstTerMsg->ucDataItemNum=k;
//			AddTerMsgList(pstTerMsg,ucSendTaskType);
//		}
//		if( stTerMsgList.ucTerMsgNum>0 )    
//		{
//			switch( ucSendTaskType )
//			{
//			case 00:
//				AddTerMsgList_to_SendTaskList();
//				break;
//			case 01:
//				AddTerMsgList_to_RS485UPSendList();
//				break;
//			default:break;
//			}
//		}
//	}*/
//	return ucRet;
//}

//��ѯ�����Ƿ���Ҫ����
//	ͣ�����Ҫ�洢��Ӧʱ����������Чֵ
unsigned char Task_RequireProc(unsigned char ucTaskNo)
{
	unsigned char ucRet=RET_SUCCESS,mem_type;
	unsigned long lCurrTime=0,lNextExeTime=0;
	unsigned char *pucGRTaskRetBuf,*mem_ptr;
	unsigned short usGRTaskRetBufLen=0;
//	unsigned long ulTemp;
	
	STDATETIME stNextExeTime,stTempTime;
//azh 160311---
	unsigned short 	usNumtask_Per_Block ;
	unsigned short 	usExedNum;
	unsigned long	ulTime_Tmp;
//azh 160311---
	STDATETIME stCurrentTime=GetCurrentTime();
	
	if( stGRDefineArea.stGRTask[ucTaskNo].ucTaskType!=CON_GRTASK_TYPE_INVALID && 
		stGRDefineArea.stGRTask[ucTaskNo].ucActive==CON_GRTASK_ACTIVE )
	{//��ѯ�����Ƿ���Ч,�Ƿ񼤻�
		lCurrTime=TimeConver(&stCurrentTime,CON_TIMEUNIT_SEC);
		lNextExeTime=TimeConver(&(stGRDefineArea.stGRTaskRunInfo[ucTaskNo].stNextExeTime),CON_TIMEUNIT_SEC);
		if(lCurrTime>=lNextExeTime)		//�жϻ�׼ʱ��
		{//10�������ڲ�ִ������������һ��ִ��ʱ��ִ��
			stNextExeTime=stGRDefineArea.stGRTaskRunInfo[ucTaskNo].stNextExeTime;
			if( stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit>CON_TIMEUNIT_MIN )
			{
				AddTime(&stNextExeTime,10,CON_TIMEUNIT_MIN);
			}
			else if( stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit==CON_TIMEUNIT_MIN )
			{
				if( stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeInter>=10 )
					AddTime(&stNextExeTime,10,CON_TIMEUNIT_MIN);
				else
					AddTime( &stNextExeTime,(unsigned long)stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeInter,
					stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit );
			}
			if( lCurrTime>(TimeConver(&stNextExeTime,CON_TIMEUNIT_SEC)) )
			{//�����´�ִ��ʱ��
				AddTime( &(stGRDefineArea.stGRTaskRunInfo[ucTaskNo].stNextExeTime),
					(unsigned long)stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeInter,
					stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit );
				Save_Curve_Task_Para(ucTaskNo, CON_TASKRIF_NEXTEXTIME_OFST, CON_TASK_PARA_SINGLE_SAVE);

				//����������Ч����
                mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
                if(mem_ptr == NULL)
                {
                    return RET_ERROR;
                }	
				pucGRTaskRetBuf=(unsigned char *)mem_ptr;			//����������
				usGRTaskRetBufLen=stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem;
				memset(pucGRTaskRetBuf,CON_GRTASK_DATA_INVALID,usGRTaskRetBufLen);
				Task_SaveRetVal(ucTaskNo,pucGRTaskRetBuf,usGRTaskRetBufLen);
				free_mem_block(mem_ptr);//
					
				stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum++;
				Save_Curve_Task_Para(ucTaskNo, CON_TASKIF_EXEDNUM_OFST, CON_TASK_PARA_SINGLE_SAVE);
//azh 171011 ����ʾ�նˣ����ѹ�����Ϲ�����
//azh 160311---
        		usNumtask_Per_Block = MCP_FLASH_BLOCK_SIZE / stGRDefineArea.stGRTask[ucTaskNo].usPerTaskRetMem ;//һ���ܷż���
        		usExedNum = stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum % (usNumtask_Per_Block * SECTORNUM_PERTASK);
        		
        		if(usExedNum % usNumtask_Per_Block == 0)//ÿ����׵�
        		{//һ��BLOCK�Ѿ�д��������һ����� azh ������ʵֻ��һ��sector
        //			Flash_Eraseblock_MCP((MCP_FLASH_TASK_START_BLOCK + ucTaskNo * SECTORNUM_PERTASK + (usExedNum / usNumtask_Per_Block)) * MCP_FLASH_BLOCK_SIZE);
                    MCP_Flash_Sector_Erase((MCP_FLASH_TASK_START_BLOCK + ucTaskNo * SECTORNUM_PERTASK + (usExedNum / usNumtask_Per_Block)) * MCP_FLASH_BLOCK_SIZE);
        			//HdConvertWatchDog();
        			if((stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos / usNumtask_Per_Block) == (usExedNum / usNumtask_Per_Block))
        			{	//βָ����Ҫ�����Ŀ����Ҫ������
        				stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos = ((usExedNum / usNumtask_Per_Block + 1) % SECTORNUM_PERTASK) * usNumtask_Per_Block;
        				//���ݿ��׵��Ӧʱ�����
        				stTempTime=TimeConverToDataTime(stGRDefineArea.stGRTaskInfo[ucTaskNo].ulTailTime);
        				ulTime_Tmp = (unsigned long)stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeInter;
        				ulTime_Tmp = ulTime_Tmp * usNumtask_Per_Block;//160310 ������һ��block4K�ĵ�����ʱ��
        				AddTime( &stTempTime,ulTime_Tmp,stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit );
        				stGRDefineArea.stGRTaskInfo[ucTaskNo].ulTailTime=TimeConver(&stTempTime,CON_TIMEUNIT_SEC);
        				Save_Curve_Task_Para(ucTaskNo, CON_TASKIF_BLK_OFST, CON_TASK_PARA_BLOCK_SAVE);
        
                        //azh 160311--- һblock������������ݿ�ʼ��ЧҲ��������Ч�����׵�ʱ��
            			AddTime(&stGRDefineArea.stGRTask[ucTaskNo].stStartActive,
            				ulTime_Tmp,	stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit);
            			Save_Curve_Task_Para(ucTaskNo, CON_TASK_STARTACTIVE_OFST, CON_TASK_PARA_SINGLE_SAVE);
                        //azh 160311
                		if(stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum>=stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum)			//ִ�д����ܴ����Ҫ����һ��
                		{
                			stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum = 0;//д��һȦ�󣬻ظ�
                			Save_Curve_Task_Para(ucTaskNo, CON_TASKIF_EXEDNUM_OFST, CON_TASK_PARA_SINGLE_SAVE);
                		}
                		//azh 160311---
        			}
        		}


//azh 160311---
//				if((stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum % stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum)==stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos)
//				{//���ݿ��׵�λ�ø���
//					stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos++;
//					if(stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos==stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum)
//						stGRDefineArea.stGRTaskInfo[ucTaskNo].usTailPos=0;
//					//���ݿ��׵��Ӧʱ�����
//					stTempTime=TimeConverToDataTime(stGRDefineArea.stGRTaskInfo[ucTaskNo].ulTailTime);
//					AddTime( &stTempTime,
//						(unsigned long)stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeInter,
//						stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit );
//					stGRDefineArea.stGRTaskInfo[ucTaskNo].ulTailTime=TimeConver(&stTempTime,CON_TIMEUNIT_SEC);
//					Save_Curve_Task_Para(ucTaskNo, CON_TASKIF_BLK_OFST, CON_TASK_PARA_BLOCK_SAVE);
//				}
//				if(stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum>0xFFF0)			//ִ�д����ܴ����Ҫ����һ��
//				{
//					ulTemp=(unsigned long)stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum;
//					stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum %= stGRDefineArea.stGRTask[ucTaskNo].usSavePointNum;
//					Save_Curve_Task_Para(ucTaskNo, CON_TASKIF_EXEDNUM_OFST, CON_TASK_PARA_SINGLE_SAVE);
//					ulTemp=ulTemp-stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum;
//					AddTime(&stGRDefineArea.stGRTask[ucTaskNo].stStartActive,
//						(ulTemp*stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeInter),
//						stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit);
//					Save_Curve_Task_Para(ucTaskNo, CON_TASK_STARTACTIVE_OFST, CON_TASK_PARA_SINGLE_SAVE);
//				}
				//ע�⻹û�г���һ��������ͣ���ˣ����������Ϊͣ���Ǵ��ϴ�������������
			}else
			{
				stGRDefineArea.stGRTaskRetVal.stRetDateTime=GetCurrentTime();
				//�����´�ִ��ʱ��
				AddTime( &(stGRDefineArea.stGRTaskRunInfo[ucTaskNo].stNextExeTime),
					(unsigned long)stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeInter,
					stGRDefineArea.stGRTask[ucTaskNo].stTITaskInter.ucTimeUnit );
				Save_Curve_Task_Para(ucTaskNo, CON_TASKRIF_NEXTEXTIME_OFST, CON_TASK_PARA_SINGLE_SAVE);
				ucRet=RET_GRTASK_NEEDPROC;
			}
		}
	}
	return ucRet;
}

//����ֽ�,��ѯ��������
//	��ѯ����ʱ�������ͣ���Ӧ���ݳ�������Ϊ0
//unsigned char Task_Resolve(STSENDDILIST *pstSendDIList,unsigned char ucTaskNo)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucLabelPos=0,i=0,j=0;
//	unsigned short usDI=0;
//	
//	STDIINFO stDIInfoTmp;
//	
//	memset(pstSendDIList,0,sizeof(STSENDDILIST));
//	
//	for(i=0;i<stGRDefineArea.stGRTask[ucTaskNo].ucDataItemNum;i++)
//	{
////		usDI=stGRDefineArea.stGRTask[ucTaskNo].ucDataArea[i*2]+stGRDefineArea.stGRTask[ucTaskNo].ucDataArea[i*2+1]*0x100;
//        usDI = curve_task_di[i];    
//		memset(&stDIInfoTmp,0,sizeof(STDIINFO));
//		stDIInfoTmp=Main_DI_Data_Query_Manage( usDI );			//��ѯ
//		
//		if( stDIInfoTmp.ucHave==CON_DI_HAVE && 
//			stDIInfoTmp.ucType==CON_DI_TYPE_DATA && stDIInfoTmp.ucMPType==CON_DI_MPTYPE_NORMAL )
//		{
//			if( stDIInfoTmp.ucReadMeterEnabel==CON_DI_OPERATE_ENABLE )
//			{
//				for( j=0;j<ucLabelPos;j++ )
//				{
//					if( pstSendDIList->usDILabel[j]==stDIInfoTmp.usBigDI )
//						break;		//������б��ҵ���ǰ�������������Ĵ��������ܾ���������Ҳ�������������Ŀ����
//				}
//				if( j>=ucLabelPos )
//					pstSendDIList->usDILabel[ucLabelPos++]=stDIInfoTmp.usBigDI;
//			}
//		}
//	}
//	pstSendDIList->ucDINum=ucLabelPos;
//	
//	return ucRet;
//}

//ִ������ֽ��������Ͷ���
//unsigned char Fill_MeterTask(unsigned short usDI,unsigned char ucTaskSour,unsigned char ucMPNo,unsigned char ucRS485TaskGroupListNo)
//{
//	unsigned char ucRet=RET_SUCCESS;
//// 	unsigned char ucPortNo;
////	SetRS485TaskPer(&stRS485TaskTmp,ucTaskSour,CON_SEQNO_NULL,CON_RS485_REPORT_TYPE_GET,usDI,ucMPNo,NULL);
////	ucPortNo=RunPara_GetMeterPara_PortNo_byMPNo(ucMPNo);
////azh	RS485_FillCommand(&stRS485TaskTmp,ucRS485TaskGroupListNo,ucPortNo);
//	return ucRet;
//}
//
////�ն��Լ�������ܼ�����
//unsigned char GetData_From_Ter_SumG()
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned char i=0;
//	for(i=0;i<stGRDefineArea.stGRTaskRetVal.ucDINum;i++)
//		ucRet = Fill_TaskData_Manage(stGRDefineArea.stGRTaskRetVal.stDIVal[i].usDI,CONRS485SOUR_GRTASK_UNREAD,CON_MPNO_SUMG_FIXED);
//
//	return ucRet;
//}

/***************************************************************************************/
//azh ����������
//��ѯ���񷵻ص������Ƿ���Ҫ����
//unsigned char Task_RequireUp(unsigned char ucTaskNo)
//{
//	unsigned char	ucRet=RET_GRTASK_NONEEDPROC;
//	unsigned long	lCurrTime=0,lNextUpTime=0;
//	
//	STDATETIME stCurrentTime;
//	
//	if( stGRDefineArea.stGRTask[ucTaskNo].ucTaskType!=CON_GRTASK_TYPE_INVALID && 
//		stGRDefineArea.stGRTask[ucTaskNo].ucActive==CON_GRTASK_ACTIVE )
//	{
//		if( stGRDefineArea.stGRTask[ucTaskNo].stTIReSendInter.ucTimeInter!=0 &&
//			stGRDefineArea.stGRTask[ucTaskNo].stTIReSendInter.ucTimeUnit!=0 )
//		{
//			//��Ҫ��������
//			stCurrentTime=GetCurrentTime();
//			lCurrTime=TimeConver(&stCurrentTime,CON_TIMEUNIT_SEC);
//			lNextUpTime=TimeConver(&(stGRDefineArea.stGRTaskRunInfo[ucTaskNo].stNextUpTime),CON_TIMEUNIT_SEC);
//			
//			if( lCurrTime>=lNextUpTime )//�Ƚ�����ʱ��
//			{
//				ucRet=RET_GRTASK_NEEDPROC;
//				AddTime( &(stGRDefineArea.stGRTaskRunInfo[ucTaskNo].stNextUpTime),
//					(unsigned long)stGRDefineArea.stGRTask[ucTaskNo].stTIReSendInter.ucTimeInter,
//					stGRDefineArea.stGRTask[ucTaskNo].stTIReSendInter.ucTimeUnit );
//			}
//		}
//	}
//	return ucRet;
//}

//azh ��������������
//������������
//unsigned char Task_UpSendRecord(unsigned char ucTaskNo)
//{
//	unsigned char	ucRet=RET_SUCCESS;
////    unsigned char *mem_ptr=NULL,mem_type;
////	unsigned short	usPointNum;
////	unsigned short	usLen=0;
////	unsigned char	*pucGRTaskUpBuf;
////	STDATETIME stTimeTemp;
////	
////	if(ucTaskNo>=CON_GRTASK_NUM)
////		return RET_ERROR;
////	
////	//�����������͵ĵ���
////	usPointNum=Task_GetUpSendPointNum(ucTaskNo);
////	if(usPointNum==0)
////		return ucRet;
////
////    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
////    if(mem_ptr == NULL)
////    {
////        return RET_ERROR;
////    }	
////	pucGRTaskUpBuf=(unsigned char *)mem_ptr;			//����������
////	
////	//��ȡ���µ����ݵ���ʱ������
////	Task_QueryRetVal_Newest(ucTaskNo,pucGRTaskUpBuf,usPointNum,stGRDefineArea.stGRTask[ucTaskNo].ucResendDataInter,&stTimeTemp,&usLen);
////	if(usPointNum!=(unsigned short)(pucGRTaskUpBuf[0]+0x100*pucGRTaskUpBuf[1]))
////	{    
////		free_mem_block(mem_ptr);//
////		return RET_ERROR;
////	}
////	
////	//��������ɱ�������ģ�������ַ�
////	
////	//������ʱ�������������
////	Fill_TaskData_to_TerMsg(&stTerMsgTmp,1,0,1);
////	
////	//������Զִ�е�����Ҫ�ж������Ƿ�ִ����
////	if(stGRDefineArea.stGRTask[ucTaskNo].usExeNum!=0)
////	{
////		//������ϲ����������ע�����ִ�д����Ѿ������趨��usExeNum
////		if(stGRDefineArea.stGRTaskInfo[ucTaskNo].usExedNum>=stGRDefineArea.stGRTask[ucTaskNo].usExeNum)
////			Task_Clear(ucTaskNo);
////	}
////
////    free_mem_block(mem_ptr);//
//	return ucRet;
//}

//void TimeChange_CTaskProc(STDATETIME stOldTime,STDATETIME stNewTime)
//{
//	unsigned long ulOldTime,ulNewTime,ulDiff=0;
//	ulOldTime=TimeConver(&stOldTime,CON_TIMEUNIT_SEC);
//	ulNewTime=TimeConver(&stNewTime,CON_TIMEUNIT_SEC);
//	if(ulOldTime>ulNewTime)
//	{//ʱ����ɸ�
//		ulDiff=ulOldTime-ulNewTime;
//	}else if(ulNewTime>ulOldTime)
//	{//ʱ�����¸�
//		ulDiff=ulNewTime-ulOldTime;
//	}
//	if(ulDiff>CON_GRTASK_DATA_TOTAL_SECONDS)			//����ȫ�����ݵ�ʱ�������ʼ��
//		CTask_Rebuild_Task();					
//}


//�����ؽ�
void CTask_Rebuild_Task(void)
{
//	STMETERPARA   stMeterPara;
	unsigned char ucMPNo,ucMPType,i;
//	SetDefaultSumGroupTask();
	for(i=0;i<CON_METER_NUM;i++)
	{//���ó�������
		ucMPNo=RunPara_GetMeter_MPNo(i);
		ucMPType=RunPara_GetMPType(ucMPNo);
		if(ucMPType==CON_MPTYPE_METER)
		{
			if(stTerRunPara.stMeterPara[i].ucProtocol==CON_PROTOCOL_INVALID)
				DeleteDefaultMeterTask(i);
			else
				SetDefaultMeterTask(i);
		}else
			DeleteDefaultMeterTask(i);
		//HdConvertWatchDog();						//ι��
	}
	SetDefaultTerTask();
	//HdConvertWatchDog();						//ι��
//	for(i=0;i<CON_PULSE_NUM;i++)
//	{
//		if(RunPara_GetPulse_MPNo(i)!=CON_MPNO_INVALID)
//			SetDefaultPulseTask(i);
//		else
//			DeleteDefaultPulseTask(i);
//		//HdConvertWatchDog();						//ι��
//	}
    stGRTaskExeInfo.ucTaskValidFlag=CON_TASK_RETDATA_INVALID;	//����״̬��Ϣ��ʼ��
    stGRTaskExeInfo.usValidCnt = 0;
}

//������
//unsigned char CTask_Check_Task(void)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	STMETERPARA   stMeterPara;
//	unsigned char ucMPNo,ucMPType,i;
//	STDATETIME stTimeCurr=GetCurrentTime();
//	if(stTimeCurr.ucSecond==30)
//	{//����Ϊÿ1���Ӽ��һ�Σ�
//		//����30���ʱ���飬������30���ʱ���������飬Ҳ�����ڵ�ǰ������û�м��
////		if(TaskCheckSumGroupIsValid()==RET_ERROR)			//����ܼ�����
////			ucRet=RET_ERROR;
//		for(i=0;i<CON_METER_NUM;i++)
//		{
//			ucMPNo=RunPara_GetMeter_MPNo(i);				
//			ucMPType=RunPara_GetMPType(ucMPNo);
//			if(ucMPType==CON_MPTYPE_METER)
//			{
//				stMeterPara=RunPara_GetMeterPara(i);
//				if(stMeterPara.ucProtocol!=CON_PROTOCOL_INVALID)
//				{
//					if(TaskCheckMeterIsValid(i)==RET_ERROR)	//��鳭������
//					{
//						ucRet=RET_ERROR;
//						break;
//					}
//				}
//			}
//		}
//		if(TaskCheckTerIsValid()==RET_ERROR)				//����ն�����
//			ucRet=RET_ERROR;
////		for(i=0;i<CON_PULSE_NUM;i++)
////		{
////			if(RunPara_GetPulse_MPNo(i)!=CON_MPNO_INVALID)
////			{
////				if(TaskCheckPulseIsValid(i)==RET_ERROR)		//�����������
////				{
////					ucRet=RET_ERROR;
////					break;
////				}
////			}
////		}
//	}
//	return ucRet;
//}
//azh ��д��ʱˢ����������ݱ�ʶ
void Timer_FillRS485MeterDi(void)
{
	unsigned char i,ucDIPtr=0;

	//�����й�����
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPDIRPOW_BLOCK;
	//�����й�����	
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPUNDIRPOW_BLOCK;
	//�����޹�����
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPDIRVAR_BLOCK;
	//�����޹�����
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPUNDIRVAR_BLOCK;
	//һ�����޹�	
	//�������޹�	
	//�������޹�
	//�������޹�
	for( i = 0 ; i < 4 ; i ++ )
	{
		stSendDIList.usDILabel[ucDIPtr++] = (unsigned short)(CON_DI_TYPE_MPVAR1_BLOCK + 0x10 * i);
	}

	//�����й��������
    stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPDIRPOWMAXDEMAND_BLOCK;
	//�����й������������ʱ��	
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_BLOCK;
	//�����й��������
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_BLOCK;
	//�����й������������ʱ��
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_BLOCK;

	//�����޹��������
    stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPDIRVARMAXDEMAND_BLOCK;
	//�����޹������������ʱ��	
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_BLOCK;
	//�����޹��������
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPUNDIRVARMAXDEMAND_BLOCK;
	//�����޹������������ʱ��
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_BLOCK;	

	//�����й�����(����)
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPDIRPOW_LM_BLOCK;
	//�����й�����(����)
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPUNDIRPOW_LM_BLOCK;
	//�����޹�����(����)
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPDIRVAR_LM_BLOCK;
	//�����޹�����(����)
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPUNDIRVAR_LM_BLOCK;
	//һ�����޹�(����)	
	//�������޹�(����)	
	//�������޹�(����)
	//�������޹�(����)
	for( i = 0 ; i < 4 ; i ++ )
	{
		stSendDIList.usDILabel[ucDIPtr++] = (unsigned short)(CON_DI_TYPE_MPVAR1_LM_BLOCK + 0x10 * i);
	}

	//�����й��������(����)
    stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_BLOCK;
	//�����й������������ʱ��(����)	
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_BLOCK;
	//�����й��������(����)
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_BLOCK;
	//�����й������������ʱ��(����)
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_BLOCK;

	//�����޹��������(����)
    stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPDIRVARMAXDEMAND_LM_BLOCK;
	//�����޹������������ʱ��(����)	
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_LM_BLOCK;
	//�����޹��������(����)
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPUNDIRVARMAXDEMAND_LM_BLOCK;
	//�����޹������������ʱ��(����)
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_LM_BLOCK;	

	//UABC
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPVOL_BLOCK;
	//IABC	
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPCUR_BLOCK;
	//˲ʱ�й�
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPINSTANTPOW_BLOCK;
	//˲ʱ�޹�
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPINSTANTVAR_BLOCK;
	//��������
//	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPPOWERFACTOR_BLOCK;

	//���һ�α��ʱ��
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPLATELYPROGTIME;
	//���һ�������������ʱ��
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPLATELYMAXDEMANDCLEARTIME;
	//���峣��
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPPOWCONST;
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPVARCONST;

	//�������״̬��
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPMETERSTATFLAG;//CON_DI_TYPE_MPCO2F_BLOCK;
	//������
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPC11F_BLOCK;

    //�������
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPLOSTVOLANUM;
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPLOSTVOLBNUM;
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPLOSTVOLCNUM;
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPLOSTVOLNUM;
    //�ۼƶ���ʱ��
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPLOSTVOLATIME;
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPLOSTVOLBTIME;
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPLOSTVOLCTIME;
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPLOSTVOLTIME;
    //�ۼƶ���ʱ��
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPLOSTVOLATIME;
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPLOSTVOLBTIME;
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPLOSTVOLCTIME;
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPLOSTVOLTIME;
    //���һ�ζ������ʱ��
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPLASTVOLAOFFENDTIME;
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPLASTVOLBOFFENDTIME;
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPLASTVOLCOFFENDTIME;
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPLASTVOLOFFENDTIME;

	//����ʱ��
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPFEERATE_BLOCK;
	
	//���ʱ���
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPDATEANDWEEK;
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPTIME;

	//��̴���
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPPROGTIMETIMES;
	//���һ�α��ʱ��
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPPROGRAMTIME;

	//��������������
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPDEMODECLRNUM;
	//���һ����������ʱ��
	stSendDIList.usDILabel[ucDIPtr++] = CON_DI_TYPE_MPDEMODECLRTIME;

//------    
    stSendDIList.ucDINum = ucDIPtr;
    stSendDIList.ucDIPtr = 0;
    stSendDIList.ucReSendnum = 0;
}
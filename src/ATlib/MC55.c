/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : MC55.c
 Description    : ������
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2011-09-23      azh           the original version
******************************************************************************/
#include "..\inc\global_config.h"

#include "MC55_MODULE.H"
#include "MC55_AT_MANAGE.H"
//#include "..\include_public\include_files.h"
//#include "..\MAIN\Main_includefile.h"

const unsigned char STR_AT_CMD_MODULERESET[]={"MODULE RESET!>>>>>>>>\r\n"};

//extern void DbgCommFillMonInfo(unsigned char *pBuf,unsigned short usLen);
/********************************************************************
	��������:	PIN_Timer
	����	:	�ܽŶ�ʱ�����������������
	��ڲ���:
	���ڲ���:
*********************************************************************/

void PIN_Timer(void)
{
//	unsigned int uRegValue = 0;
	
	if(stPinSate.bOldMBPWCTL != stPinSate.bMBPWCTL)
	{	
		stPinSate.usMBACKCnt = 0;
		stPinSate.usMBPWCTLCnt = 0;
		stPinSate.bOldMBPWCTL = stPinSate.bMBPWCTL;
	}
	if (stPinSate.usMBPWCTLCnt < 0xffff)
		stPinSate.usMBPWCTLCnt++;
	
	if( stPinSate.usMBPWCTLCnt == CON_PIN_DELAY_MBPWCTL) 
	{
		if( stPinSate.bMBPWCTL == 1 )
		{
			stAtManageState.bIsPowerOff = 0; 
			stPinSate.usMBPWCTLCnt = 0;
		}else
		{
			stAtManageState.bIsPowerOff = 1;
			stAtManageState.uPowerOffTimer = CON_TIMEER_POWEROFF;
		}
	}
	if( (usRCommModResetTimer!=0xFFFF)&&(usRCommModResetTimer!=0) )//azh 111108
	{
      usRCommModResetTimer--;	
	}
}
void DebugPrint(unsigned char* pstr, unsigned short usLen)
{
    if(gucAtCmd_WatchOut_Flag == CON_ATCMD_OUT_ENABLE)
    {
	    debug_come_send_frame(pstr, usLen);
	}

/*
//#ifdef _MC55_DEBUG
	if(stModuleComParam.usTimerParam == 31)
	{
//		DbgCommFillMonInfo(pstr, usLen);//qxf ;��Ϊ�ڲ�����ȥ����
	}
//#endif // _DEBUG
*/
}



/*********************************************************************************************************
** ��������: GPRS_TargetInit
** ��������: Ŀ����ʼ�����룬����Ҫ�ĵط����ã�������Ҫ�ı�
**
********************************************************************************************************/
void GPRS_TargetInit(void)
{
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
	FIO2DIR |= (P2_VGPRS_ON);
	FIO1DIR |= (P1_CPU_EVERGOFF | P1_CPU_IGT);
#else// CODE_VER_TYPE==H_METER_PRJ ��ϸ�ѹ��	//WLS_CUR_PRJ ������߲ɼ���
	FIO0DIR |= (P0_C_GPRSON | P0_CPU_EVERGOFF | P0_CPU_IGT);
//	FIO1DIR |= (P1_GNET_LED | P1_GDATA_LED);
#endif
	
	NETLED_CLR;
	CLR_LOCAL_STATE(CON_GPRS_LED_STATE);
	DATALED_CLR;
	PIN_POWERON_CLR;
	PIN_IGT_CLR;
	PIN_EMERGOFF_CLR;
	//Uart_InitChBaud( 0,57600,CON_UART_EPN_OFF,8,CON_UART_STPB_1);//��MC55ͨ�Ŷ˿ڳ�ʼ��
	//azh 110926 ��UART_init();�г�ʼ��	

	//��ʱ��0��ʼ��
	//10ms�ж�һ��
}
void Init_CommPara_MC55(void)
{
	Reload_SCA_Para();	
	Reload_Chinfo_Para();
	Reload_PINPUK_Para();
	Reload_PDP_Para();
	Reload_ReLink_Para();
}

/********************************************************************
	��������:	Init	
	����	:	��ʼ������
	��ڲ���:	
	���ڲ���:	
*********************************************************************/

void MC55_SysInit(void)
{
	memset(&stModuleRunState,0,sizeof(ST_MODULE_RUN_STATE));
	stModuleRunState.ucHardWareVer = CON_HARDWARE_VER;
	stModuleRunState.ucSortWareVer = CON_SOFTWARE_VER;
	stModuleRunState.ucCurCommChannel = 0;
	stModuleRunState.ucSignIntension = 1;
	
	stPinSate.bIsMBACKPulse = 0;
	stPinSate.bIsMBPWCTLPulse = 0;
	stPinSate.bMBACK = 0;
	stPinSate.bMBPWCTL = 0;
	stPinSate.bOldMBACK = 0;
	stPinSate.bOldMBPWCTL = 0;
	stPinSate.usMBACKCnt = 0;
	stPinSate.usMBPWCTLCnt = 0;


	stPinSate.bMBPWCTL = 1;		//Զ������ģ�鸴λʹ��//azh???

//	GPRS_TargetInit();
	
	AT_Init();
}



void MC55_Init(void)
{
	MC55_SysInit();
	Init_CommPara_MC55();
	DebugPrint((unsigned char*)STR_AT_CMD_MODULERESET, sizeof(STR_AT_CMD_MODULERESET));
	ucLink_Time=0;
//azh
	stModuleComParam.stCh[0].ucType = TYPE_COMUNNICATION_TCP;
	RComm_Initial();//azh 110928
	
}


//void Mc55_Timer(void)
void Mc55_Run(void)
{
	//�ֻ�ģ��ͨѸ�������

	if(((ucLink_Time /10)&0x01) == 0x01)
	{
		DATALED_SET;
	}
	else
	{
		DATALED_CLR;
	}

	AT_Manage();

}



/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : MC55.c
 Description    : 主函数
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2011-09-23      azh           the original version
******************************************************************************/
#include "..\inc\global_config.h"

#include "MC55_MODULE.H"
#include "MC55_AT_MANAGE.H"
//#include "..\include_public\include_files.h"
//#include "..\MAIN\Main_includefile.h"

const unsigned char STR_AT_CMD_MODULERESET[]={"MODULE RESET!>>>>>>>>\r\n"};

//extern void DbgCommFillMonInfo(unsigned char *pBuf,unsigned short usLen);
/********************************************************************
	函数名称:	PIN_Timer
	功能	:	管脚定时函数，控制输出脉冲
	入口参数:
	出口参数:
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
//		DbgCommFillMonInfo(pstr, usLen);//qxf ;因为内部串口去掉了
	}
//#endif // _DEBUG
*/
}



/*********************************************************************************************************
** 函数名称: GPRS_TargetInit
** 功能描述: 目标板初始化代码，在需要的地方调用，根据需要改变
**
********************************************************************************************************/
void GPRS_TargetInit(void)
{
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
	FIO2DIR |= (P2_VGPRS_ON);
	FIO1DIR |= (P1_CPU_EVERGOFF | P1_CPU_IGT);
#else// CODE_VER_TYPE==H_METER_PRJ 配合高压表	//WLS_CUR_PRJ 配合无线采集器
	FIO0DIR |= (P0_C_GPRSON | P0_CPU_EVERGOFF | P0_CPU_IGT);
//	FIO1DIR |= (P1_GNET_LED | P1_GDATA_LED);
#endif
	
	NETLED_CLR;
	CLR_LOCAL_STATE(CON_GPRS_LED_STATE);
	DATALED_CLR;
	PIN_POWERON_CLR;
	PIN_IGT_CLR;
	PIN_EMERGOFF_CLR;
	//Uart_InitChBaud( 0,57600,CON_UART_EPN_OFF,8,CON_UART_STPB_1);//与MC55通信端口初始化
	//azh 110926 在UART_init();中初始化	

	//定时器0初始化
	//10ms中断一次
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
	函数名称:	Init	
	功能	:	初始化函数
	入口参数:	
	出口参数:	
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


	stPinSate.bMBPWCTL = 1;		//远程无线模块复位使能//azh???

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
	//手机模块通迅处理程序

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



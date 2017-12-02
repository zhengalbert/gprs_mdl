/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_Ver.c
 Description    : 终端版本信息
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-08-15    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

extern unsigned char g_ucVerNo;
//azh 170920
const unsigned char gucRmoteVerInfo[46]={			
	'1','0','0','0',
	'T','E','L','I','T','8','6','8',
	'1','.','0','0',
	0x18,0x09,0x17,
	'1','.','0','0',
	0x18,0x09,0x17,
	'0','1','5','7','8','9','1','5','2','7',
	'1','7','8','3','1','6','4','2','4','9',
	};
const unsigned char gucLocalVerInfo[15]={			
	0x00,0x00,0x00,0x00,0x00,0x00,
	'1','0',
	'0','0',
	0x18,0x09,0x17,
	0x10,0x00,
    };	
	
	
//这样处理程序代码固定不变
const unsigned char gucTerVerInfo[7]=
	{			
	CON_TER_SOFTWAREVER_0,
	CON_TER_SOFTWAREVER_1,
	CON_TER_SOFTWAREVER_2,
	CON_TER_SOFTWAREVER_3,
	CON_TER_SOFTWARE_DAY,
	CON_TER_SOFTWARE_MON,
	CON_TER_SOFTWARE_YEAR 
	};

const unsigned char gucTerVerInfo_Test[7]={			//测试用
	CON_TER_SOFTWAREVER_0,
	CON_TER_SOFTWAREVER_1,
	CON_TER_SOFTWAREVER_2,
	0x36,
	0x25,
	0x09,
	0x09 };

	
const unsigned char gucInnerVerInfo[4]=
	{
	CON_TER_INNERVER_0,
	CON_TER_INNERVER_1,
	CON_TER_INNERVER_2,
	CON_TER_INNERVER_3 
	};

const unsigned char gucTerHardwareVer[7]=
	{
	CON_TER_HARDWAREVER_0,
	CON_TER_HARDWAREVER_1,
	CON_TER_HARDWAREVER_2,
	CON_TER_HARDWAREVER_3,
	CON_TER_HARDWARE_DAY,
	CON_TER_HARDWARE_MON,
	CON_TER_HARDWARE_YEAR
	};

const unsigned char gucTerProtoolVer[4]=
{
	0x32,
	0x30,
	0x30,
	0x37,
};

void RunPara_GetTerVerInfo(unsigned char *pucVerInfo)
{
	if(g_ucVerNo != 2)
	{
		pucVerInfo[0]=gucTerVerInfo[0];
		pucVerInfo[1]=gucTerVerInfo[1];
		pucVerInfo[2]=gucTerVerInfo[2];
		pucVerInfo[3]=gucTerVerInfo[3];
		pucVerInfo[4]=gucTerVerInfo[4];
		pucVerInfo[5]=gucTerVerInfo[5];
		pucVerInfo[6]=gucTerVerInfo[6];
	}
	else
	{
		pucVerInfo[0]=gucTerVerInfo_Test[0];
		pucVerInfo[1]=gucTerVerInfo_Test[1];
		pucVerInfo[2]=gucTerVerInfo_Test[2];
		pucVerInfo[3]=gucTerVerInfo_Test[3];
		pucVerInfo[4]=gucTerVerInfo_Test[4];
		pucVerInfo[5]=gucTerVerInfo_Test[5];
		pucVerInfo[6]=gucTerVerInfo_Test[6];
	}
}
void RunPara_GetTerInnerVerInfo(unsigned char *pucVerInfo)
{
	pucVerInfo[0]=gucInnerVerInfo[0];
	pucVerInfo[1]=gucInnerVerInfo[1];
	pucVerInfo[2]=gucInnerVerInfo[2];
	pucVerInfo[3]=gucInnerVerInfo[3];
}

void RunPara_GetTerHardwareVerInfo(unsigned char *pucVerInfo)
{
	pucVerInfo[0]=gucTerHardwareVer[0];
	pucVerInfo[1]=gucTerHardwareVer[1];
	pucVerInfo[2]=gucTerHardwareVer[2];
	pucVerInfo[3]=gucTerHardwareVer[3];
	pucVerInfo[4]=gucTerHardwareVer[4];
	pucVerInfo[5]=gucTerHardwareVer[5];
	pucVerInfo[6]=gucTerHardwareVer[6];
}

void RunPara_GetTerProtoolVerInfo(unsigned char *pucVerInfo)
{
	pucVerInfo[0]=gucTerProtoolVer[0];
	pucVerInfo[1]=gucTerProtoolVer[1];
	pucVerInfo[2]=gucTerProtoolVer[2];
	pucVerInfo[3]=gucTerProtoolVer[3];
}

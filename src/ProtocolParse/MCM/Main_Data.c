/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_Data.c
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
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

//azh 为了节省空间
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
//azh 170925 为了压缩代码空间
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
//填写数据时标及费率数 azh 已经被软件自动优化
//unsigned short Fill_DataTdFee(U8 ucMeterNo, U8 *pucRetBuffer,U16 usPos,STDATETIME stRMeterTime, STDATETIME stRecordTime)
//{
//    STMETERPARA	 stMeterPara;
//
//	usPos=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,usPos,stRMeterTime);		//数据时标Td
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
	case CON_TERMSG_ACK:			//确认否认
		ucDT =GetDTbyItemLabel_Ack(usItemLabel);
		break;
	case CON_TERMSG_RESET:			//复位命令接收
		ucDT =GetDTbyItemLabel_RST(usItemLabel);
		break;
	case CON_TERMSG_CONNECT:		//链路接口检测
	case CON_TERMSG_RELAYCMD:		//中继站命令
		break;
	case CON_TERMSG_SETPARA:		//参数设置
	case CON_TERMSG_GETPARA:		//查询终端参数
		ucDT =GetDTbyItemLabel_Para(usItemLabel);
		break;
	case CON_TERMSG_CONFIGURATION:	//请求终端配置
		ucDT =GetDTbyItemLabel_Config(usItemLabel);
		break;
	case CON_TERMSG_LOGIN:			//身份认证及密钥协商（AFN=06H）
		ucDT =GetDTbyItemLabel_Login(usItemLabel);
		break;
	case CON_TERMSG_CONTROL:		//控制命令
		ucDT =GetDTbyItemLabel_Ctrl(usItemLabel);
		break;
	case CON_TERMSG_DATAREQ_SLAVE:
		break;
	case CON_TERMSG_TASKDATA:		//任务数据查询
		ucDT =GetDTbyItemLabel_TaskData(usItemLabel);
		break;
	case CON_TERMSG_REQDATA_1:		//请求1类数据（实时数据）
		ucDT =GetDTbyItemLabel_ReqData_1(usItemLabel);
		break;
	case CON_TERMSG_REQDATA_2:		//请求2类数据（冻结数据）
		ucDT =GetDTbyItemLabel_ReqData_2(usItemLabel);
		break;
	case CON_TERMSG_REQDATA_3:		//请求3类数据（事件）
		ucDT =GetDTbyItemLabel_ReqData_3(usItemLabel);
		break;
	case CON_TERMSG_FILETRANS:		//文件传输
		ucDT =GetDTbyItemLabel_FileTrans(usItemLabel);
		break;
	case CON_TERMSG_DATATRANS:		//数据转发
		ucDT =GetDTbyItemLabel_DataTrans(usItemLabel);
		break;
	case CON_TERMSG_EXTCMD:			//扩展命令
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
	case CON_TERMSG_ACK:			//确认否认
		usItemLabel = GetItemLabelbyDT_Ack(ucDT);
		break;
	case CON_TERMSG_RESET:			//复位命令接收
		usItemLabel = GetItemLabelbyDT_RST(ucDT);
		break;
	case CON_TERMSG_CONNECT:		//链路接口检测
	case CON_TERMSG_RELAYCMD:		//中继站命令
		break;
	case CON_TERMSG_SETPARA:		//参数设置
	case CON_TERMSG_GETPARA:		//查询终端参数
		usItemLabel=GetItemLabelbyDT_Para(ucDT);
		break;
	case CON_TERMSG_LOGIN:			//身份认证及密钥协商（AFN=06H）
		usItemLabel=GetItemLabelbyDT_Login(ucDT);
		break;
	case CON_TERMSG_DATAREQ_SLAVE:
		break;
	case CON_TERMSG_CONFIGURATION:	//请求终端配置
		usItemLabel=GetItemLabelbyDT_Config(ucDT);
		break;
	case CON_TERMSG_CONTROL:		//控制命令
		usItemLabel=GetItemLabelbyDT_Ctrl(ucDT);
		break;
	case CON_TERMSG_TASKDATA:		//任务数据查询
		usItemLabel=GetItemLabelbyDT_TaskData(ucDT);
		break;
	case CON_TERMSG_REQDATA_1:		//请求1类数据（实时数据）
		usItemLabel=GetItemLabelbyDT_ReqData_1(ucDT);
		break;
	case CON_TERMSG_REQDATA_2:		//请求2类数据（冻结数据）
		usItemLabel=GetItemLabelbyDT_ReqData_2(ucDT);
		break;
	case CON_TERMSG_REQDATA_3:		//请求3类数据（事件）
		usItemLabel=GetItemLabelbyDT_ReqData_3(ucDT);
		break;
	case CON_TERMSG_FILETRANS:		//文件传输
		usItemLabel=GetItemLabelbyDT_FileTrans(ucDT);
		break;
	case CON_TERMSG_DATATRANS:		//数据转发
		usItemLabel=GetItemLabelbyDT_DataTrans(ucDT);
		break;
	case CON_TERMSG_EXTCMD:			//扩展命令
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
	case	CON_ACK_F1:		ucDT=1;	break;		//F1:全部确认
	case	CON_ACK_F2:		ucDT=2;	break;		//F2:全部否认
	case	CON_ACK_F3:		ucDT=3;	break;		//F3:按数据项进行确认否认
	}
	return ucDT;
}

unsigned char GetDTbyItemLabel_RST(unsigned short usItemLabel)
{
	unsigned char ucDT =0xFF;
	switch(usItemLabel)
	{
		case	CON_RST_F1_HARD:		ucDT=1;	break;		//F1:硬件初始化
		case	CON_RST_F2_DATA:		ucDT=2;	break;		//F2:数据区初始化
		case	CON_RST_F3_ALL:			ucDT=3;	break;		//F3:全体数据
		case 	CON_RST_F4_EXCECOMM:	ucDT = 4;break;	//F4:通讯参数之外的数据及参数初始化
	}
	return ucDT;
}

unsigned char GetDTbyItemLabel_Para(unsigned short usItemLabel)
{
	unsigned char ucDT=0xFF;
/*
	switch(usItemLabel)
	{
	case	CON_PARA_F1_COMMPARA:		ucDT = 1	;break;	//F1：终端通信参数设置
	case	CON_PARA_F2_RELAYPARA:		ucDT = 2	;break;	//F2：终端中继转发设置
	case	CON_PARA_F3_STAIPADDR:		ucDT = 3	;break;	//F3：主站IP地址和端口
	case	CON_PARA_F4_STAPHONENO:		ucDT = 4	;break;	//F4：主站电话号码和短信中心号码
	case	CON_PARA_F5_TERENCRYPT:		ucDT = 5	;break;	//F5：终端密码设置
	case	CON_PARA_F6_TERGROUPADDR:	ucDT = 6	;break;	//F6：终端组地址设置
	case	CON_PARA_F7_IPADDR:		    ucDT = 7	;break;	//F7：终端抄表日设置
	case	CON_PARA_F8_ALRCTLWORD:		ucDT = 8	;break;	//F8：终端事件记录配置设置
	case	CON_PARA_F9_CONFIGNUM:		ucDT = 9	;break;	//F9：终端配置数量表
	case	CON_PARA_F10_METERPARA:		ucDT = 10	;break;	//F10：终端电能表/交流采样装置配置参数
//	case	CON_PARA_F11_PULSEPARA:		ucDT = 11	;break;	//F11：终端脉冲配置参数
//	case	CON_PARA_F12_DINPUTPARA:	ucDT = 12	;break;	//F12：终端状态量输入参数
//	case	CON_PARA_F13_ANALOGPARA:	ucDT = 13	;break;	//F13：终端电压/电流模拟量配置参数
//	case	CON_PARA_F14_SUMGPARA:		ucDT = 14	;break;	//F14：终端总加组配置参数
//	case	CON_PARA_F15_POWVALDIFF:	ucDT = 15	;break;	//F15：有功总电能量差动越限事件参数设置
	case	CON_PARA_F16_PDPINFO:		ucDT = 16	;break;	//F16：虚拟专网用户名、密码
//	case	CON_PARA_F17_KEEPPOWER:		ucDT = 17	;break;	//F17：终端保安定值
//	case	CON_PARA_F18_POWCTRLTIMEPER:ucDT = 18	;break;	//F18：终端功控时段
//	case	CON_PARA_F19_POWCTRLCOEF:	ucDT = 19	;break;	//F19：终端时段功控定值浮动系数
//	case	CON_PARA_F20_ENERCTRLCOEF:	ucDT = 20	;break;	//F20：终端月电能量控定值浮动系数
//	case	CON_PARA_F21_PERTIMEFEE:	ucDT = 21	;break;	//F21：终端电能量费率时段和费率数
//	case	CON_PARA_F22_FEEPARA:		ucDT = 22	;break;	//F22：终端电能量费率
//	case	CON_PARA_F23_URGEALARMPARA:	ucDT = 23	;break;	//F23：终端催费告警参数
	case	CON_PARA_F24_COPYINTER:		ucDT = 24	;break;	//F24：终端抄表间隔设置
	case	CON_PARA_F25_MPBASEPARA:	ucDT = 25	;break;	//F25：测量点基本参数
	case	CON_PARA_F26_MPLIMIT:		ucDT = 26	;break;	//F26：测量点限值参数
	case	CON_PARA_F27_MPFREEZEPARA:	ucDT = 27	;break;	//F27：测量点数据冻结参数
	case	CON_PARA_F28_MPPOWRATEPARA:	ucDT = 28	;break;	//F28：测量点功率因数分段限值
	case	CON_PARA_F29_METERADDR:		ucDT = 29	;break;	//F29：
	case 	CON_PARA_F30_METERCOPYFLAG:	ucDT = 30;	break;
	case 	CON_PARA_F31_SLAVENODE:		ucDT = 31;	break;
	case 	CON_PARA_F34_PORTPROPERTY:		ucDT = 34;break;
	case 	CON_PARA_F35_EMPHAFLAG:		ucDT = 35;break;
	case	CON_PARA_F33_SUMGFREEZEPARA:ucDT = 33	;break;	//F33：总加组数据冻结参数
	case 	CON_PARA_F37_LINKPARA:			ucDT = 37;break;//F37:级联通信参数
	case	CON_PARA_F38_JOB1PARA:			ucDT = 38;break;//F38:一类数据配置设置
	case 	CON_PARA_F39_JOB2PARA:			ucDT = 39;break;//F39:二类数据配置设置
//	case	CON_PARA_F41_TIMEPERSETTING:ucDT = 41	;break;	//F41：时段功控定值
//	case	CON_PARA_F42_RESTCTRL:		ucDT = 42	;break;	//F42：厂休功控参数
//	case	CON_PARA_F43_POWERSLIP:		ucDT = 43	;break;	//F43：功率控制的功率计算滑差时间
//	case	CON_PARA_F44_SALESTOP:		ucDT = 44	;break;	//F44：营业报停控参数
//	case	CON_PARA_F45_POWERCTRLTURN:	ucDT = 45	;break;	//F45：功控轮次设定
//	case	CON_PARA_F46_MONTHENERCTRL:	ucDT = 46	;break;	//F46：月电量控定值
//	case	CON_PARA_F47_BUYENERCTRL:	ucDT = 47	;break;	//F47：购电量控参数
//	case	CON_PARA_F48_ENERCTRLTURN:	ucDT = 48	;break;	//F48：电控轮次设定
//	case	CON_PARA_F49_CTRLALRTIME:	ucDT = 49	;break;	//F49：功控告警时间
	case	CON_PARA_F57_SOUNDALARM:	ucDT = 57	;break;	//F57：终端声音告警允许∕禁止设置
	case	CON_PARA_F58_KEEPAUTO:		ucDT = 58	;break;	//F58：终端自动保电参数
	case	CON_PARA_F59_MTRERRORPARA:	ucDT = 59	;break;	//F59：电能表异常判别阈值设定
//	case	CON_PARA_F60_HARMONPARA:	ucDT = 60	;break;	//F60：谐波限值
//	case	CON_PARA_F61_DCAINPARA:		ucDT = 61	;break;	//F61：直流模拟量接入参数
	case	CON_PARA_F62_COMMTYPE:		ucDT = 62	;break;	//F62 虚拟专网工作方式
	case	CON_PARA_F63_FLUXLIMIT:		ucDT = 36	;break;	//F63 日通信流量门限设置
	case	CON_PARA_F65_UPDATA1PARA:	ucDT = 65	;break;	//F65：定时发送1类数据任务设置
	case	CON_PARA_F66_UPDATA2PARA:	ucDT = 66	;break;	//F66：定时发送2类数据任务设置
	case	CON_PARA_F67_UPDATA1CTRL:	ucDT = 67	;break;	//F67：定时发送1类数据任务启动/停止设置
	case	CON_PARA_F68_UPDATA2CTRL:	ucDT = 68	;break;	//F68：定时发送2类数据任务启动/停止设置
	case	CON_PARA_F73_CAPPARA:		ucDT = 73	;break;	//F73：电容器参数
//	case	CON_PARA_F74_CAPRUNPARA:	ucDT = 74	;break;	//F74：电容器投切运行参数
//	case	CON_PARA_F75_CAPRELAYPARA:	ucDT = 75	;break;	//F75：电容器保护参数
//	case	CON_PARA_F76_CAPCTRL:		ucDT = 76	;break;	//F76：电容器投切控制方式
//	case	CON_PARA_F81_DCARATE:		ucDT = 81	;break;	//F81：直流模拟量输入变比
//	case	CON_PARA_F82_DCALIMIT:		ucDT = 82	;break;	//F82：直流模拟量限值
//	case	CON_PARA_F83_DCAFREESE:		ucDT = 83	;break;	//F83：直流模拟量冻结参数
	case	CON_PARA_F89_ADDR:			ucDT = 89	;break;	//F89  终端地址
	case	CON_PARA_F207_ADDR:			ucDT = 207	;break;	
	}
*/  

    if(usItemLabel > CON_PARA_BEG_CODE)
    {
        ucDT = usItemLabel - CON_PARA_BEG_CODE;
        if(protocol_para_fn_valid[(ucDT-1)/32]&(1<<((ucDT-1)%32))==0)//azh 无效则置0xff
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
	case 	CON_CONFIG_F1_VERSION:		ucDT = 1	;break;	//F1配置终端版本信息
	case  	CON_CONFIG_F2_INTERFACE:	ucDT = 2	;break;	//F2配置终端输入、输出及通信端口配置
	case 	CON_CONFIG_F3_OTHER:		ucDT = 3	;break;	//F3配置终端其他配置
	case 	CON_CONFIG_F4_PARA:			ucDT = 4	;break;	//F4配置终端参数配置（本终端软硬件版本支持的参数）
	case 	CON_CONFIG_F5_CTRL:			ucDT = 5	;break;	//F5配置终端控制配置（本终端软硬件版本支持的控制命令）
	case 	CON_CONFIG_F6_DATA1:		ucDT = 6	;break;	//F6配置终端1类数据配置（本终端软硬件版本支持的1类数据）
	case 	CON_CONFIG_F7_DATA2:		ucDT = 7	;break;	//F7配置终端2类数据配置（本终端软硬件版本支持的2类数据）
	case 	CON_CONFIG_F8_ERC:			ucDT = 8	;break;	//F8配置终端事件记录配置（本终端软硬件版本支持的事件记录）
//azh 170920
	case 	CON_CONFIG_F9_REMOTEVER:	ucDT = 9	;break;	//F9远程通信模块版本信息
	case 	CON_CONFIG_F10_LOCALVER:	ucDT = 10   ;break;	//F10本地通信模块版本信息	   
	} 
	return ucDT;
}


unsigned char GetDTbyItemLabel_Login(unsigned short usItemLabel)
{
	unsigned char ucDT=0xFF;
	switch(usItemLabel)
	{
	case 	CON_LOGIN_F1_REQUEST:		ucDT = 1	;break;	//F1身份认证请求
	case  	CON_LOGIN_F2_ANSWER:		ucDT = 2	;break;	//F2身份认证响应
	} 
	return ucDT;
}


unsigned char GetDTbyItemLabel_Ctrl(unsigned short usItemLabel)
{
	unsigned char ucDT=0xFF;
/*
	switch(usItemLabel)
	{
//	case CON_CTRL_F1_YK_OPEN:		ucDT = 1	;break;	//F1遥控跳闸
//	case CON_CTRL_F2_YK_CLOSE:		ucDT = 2	;break;	//F2遥控允许合闸
//	case CON_CTRL_F9_TIMEPER_ON:	ucDT = 9	;break;	//F9时段功控投入
//	case CON_CTRL_F10_RESTCTRL_ON:	ucDT = 10	;break;//F10厂休功控投入
//	case CON_CTRL_F11_SALESTOP_ON:	ucDT = 11	;break;//F11营业报停功控投入
//	case CON_CTRL_F12_POWERLOWER_ON:ucDT = 12	;break;//F12当前功率下浮控投入
//	case CON_CTRL_F15_MONTHENER_ON:	ucDT = 15	;break;//F15月电控投入
//	case CON_CTRL_F16_BUYENER_ON:	ucDT = 16	;break;//F16购电控投入
//	case CON_CTRL_F17_TIMEPER_OFF:	ucDT = 17	;break;//F17时段功控解除
//	case CON_CTRL_F18_RESTCTRL_OFF:	ucDT = 18	;break;//F18厂休功控解除
//	case CON_CTRL_F19_SALESTOP_OFF:	ucDT = 19	;break;//F19营业报停功控解除
//	case CON_CTRL_F20_POWERLOWER_OFF:ucDT = 20	;break;//F20当前功率下浮控解除
//	case CON_CTRL_F23_MONTHENER_OFF:ucDT = 23	;break;//F23月电控解除
//	case CON_CTRL_F24_BUYENER_OFF:	ucDT = 24	;break;//F24购电控解除
//	case CON_CTRL_F25_KEEPPOWER_ON:	ucDT = 25	;break;//F25终端保电投入
//	case CON_CTRL_F26_URGEALARM_ON:	ucDT = 26	;break;//F26催费告警投入
	case CON_CTRL_F27_TALK_ON:		ucDT = 27	;break;//F27允许终端与主站通话
	case CON_CTRL_F28_REMOVE_ON:	ucDT = 28	;break;//F28终端剔除投入
	case CON_CTRL_F29_UPACTIVE_ON:	ucDT = 29	;break;//F29允许终端主动上报
	case CON_CTRL_F31_SETTIME:		ucDT = 31	;break;//F31对时命令
	case CON_CTRL_F32_MESSAGE:		ucDT = 32	;break;//F32中文信息
//	case CON_CTRL_F33_KEEPPOWER_OFF:ucDT = 33	;break;//F33终端保电解除
//	case CON_CTRL_F34_URGEFEE_OFF:	ucDT = 34	;break;//F34催费告警解除
	case CON_CTRL_F35_TALK_OFF:		ucDT = 35	;break;//F35禁止终端与主站通话
	case CON_CTRL_F36_REMOVE_OFF:	ucDT = 36	;break;//F36终端剔除解除
	case CON_CTRL_F37_UPACTIVE_OFF:	ucDT = 37	;break;//F37禁止终端主动上报
	case CON_CTRL_F38_SMS_ACTIVE:	ucDT = 38	;break;//F38短信激活
	case CON_CTRL_F39_DISCONECT:	ucDT = 39	;break;//F38短信激活
//	case CON_CTRL_F41_CAPCTRL_ON:	ucDT = 41	;break;//F41电容控制投入
//	case CON_CTRL_F42_CAPCTRL_OFF:	ucDT = 42	;break;//F42电容控制解除
	case CON_CTRL_F49:	ucDT = 49	;break;//F42电容控制解除
	case CON_CTRL_F50:	ucDT = 50	;break;//F42电容控制解除
	case CON_CTRL_F51:	ucDT = 51	;break;//F42电容控制解除
//	case CON_CTRL_F88_TIMEREVISE:	ucDT = 88	;break;//F88精确对时命令
	}
*/
    if(usItemLabel > CON_CTRL_BEG_CODE)
    {
        ucDT = usItemLabel - CON_CTRL_BEG_CODE;
        if(protocol_ctrl_fn_valid[(ucDT-1)/32]&(1<<((ucDT-1)%32))==0)//azh 无效则置0xff
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
	case	CON_TASKDATA_REQ_F1:		ucDT = 1	;break;	//F1 1类数据
	case	CON_TASKDATA_REQ_F2:		ucDT = 2	;break;	//F2 2类数据
	}
	return ucDT;
}

unsigned char GetDTbyItemLabel_ReqData_1(unsigned short usItemLabel)
{
	unsigned char ucDT=0xFF;
/*
	switch(usItemLabel)
	{
	case	CON_DATA_REQ1_F1:	ucDT = 1	;break;	//F1	终端版本信息
	case	CON_DATA_REQ1_F2:	ucDT = 2	;break;	//F2	终端日历时钟
	case	CON_DATA_REQ1_F3:	ucDT = 3	;break;	//F3	终端参数状态
	case	CON_DATA_REQ1_F4:	ucDT = 4	;break;	//F4	终端通信状态
	case	CON_DATA_REQ1_F5:	ucDT = 5	;break;	//F5	终端控制设置状态
	case	CON_DATA_REQ1_F6:	ucDT = 6	;break;	//F6	终端当前控制状态
	case	CON_DATA_REQ1_F7:	ucDT = 7	;break;	//F7	终端事件计数器当前值
	case	CON_DATA_REQ1_F8:	ucDT = 8	;break;	//F8	终端事件标志状态
	case	CON_DATA_REQ1_F9:	ucDT = 9	;break;	//F9	终端状态量及变位标志
	case	CON_DATA_REQ1_F10:	ucDT = 10	;break;	//F10	终端与主站当日、月通信流量
	case	CON_DATA_REQ1_F11:	ucDT = 11	;break;	//F11	当前信号强度
//	case	CON_DATA_REQ1_F17:	ucDT = 17	;break;	//F17	当前总加有功功率
//	case	CON_DATA_REQ1_F18:	ucDT = 18	;break;	//F18	当前总加无功功率
//	case	CON_DATA_REQ1_F19:	ucDT = 19	;break;	//F19	当日总加有功电能量（总、费率1~M）
//	case	CON_DATA_REQ1_F20:	ucDT = 20	;break;	//F20	当日总加无功电能量（总、费率1~M）
//	case	CON_DATA_REQ1_F21:	ucDT = 21	;break;	//F21	当月总加有功电能量（总、费率1~M）
//	case	CON_DATA_REQ1_F22:	ucDT = 22	;break;	//F22	当月总加无功电能量（总、费率1~M）
//	case	CON_DATA_REQ1_F23:	ucDT = 23	;break;	//F23	终端当前剩余电量（费）
//	case	CON_DATA_REQ1_F24:	ucDT = 24	;break;	//F24	当前功率下浮控控后总加有功功率冻结值
	case	CON_DATA_REQ1_F25:	ucDT = 25	;break;	//F25	当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流
	case	CON_DATA_REQ1_F26:	ucDT = 26	;break;	//F26	A、B、C三相断相统计数据及最近一次断相记录
	case	CON_DATA_REQ1_F27:	ucDT = 27	;break;	//F27	电能表日历时钟、编程次数及其最近一次操作时间
	case	CON_DATA_REQ1_F28:	ucDT = 28	;break;	//F28	电表运行状态字及其变位标志
	case 	CON_DATA_REQ1_F31:  ucDT = 31	;break;
	case 	CON_DATA_REQ1_F32:	ucDT = 32	;break;
	case	CON_DATA_REQ1_F33:	ucDT = 33	;break;	//F33	当前正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
	case	CON_DATA_REQ1_F34:	ucDT = 34	;break;	//F34	当前反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
	case	CON_DATA_REQ1_F35:	ucDT = 35	;break;	//F35	当月正向有/无功最大需量及发生时间（总、费率1~M）
	case	CON_DATA_REQ1_F36:	ucDT = 36	;break;	//F36	当月反向有/无功最大需量及发生时间（总、费率1~M）
	case	CON_DATA_REQ1_F37:	ucDT = 37	;break;	//F37	上月正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
	case	CON_DATA_REQ1_F38:	ucDT = 38	;break;	//F38	上月反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
	case	CON_DATA_REQ1_F39:	ucDT = 39	;break;	//F39	上月正向有/无功最大需量及发生时间（总、费率1~M）
	case	CON_DATA_REQ1_F40:	ucDT = 40	;break;	//F40	上月反向有/无功最大需量及发生时间（总、费率1~M）
	case	CON_DATA_REQ1_F41:	ucDT = 41	;break;	//F41	当日正向有功电能量（总、费率1~M）
	case	CON_DATA_REQ1_F42:	ucDT = 42	;break;	//F42	当日正向无功电能量（总、费率1~M）
	case	CON_DATA_REQ1_F43:	ucDT = 43	;break;	//F43	当日反向有功电能量（总、费率1~M）
	case	CON_DATA_REQ1_F44:	ucDT = 44	;break;	//F44	当日反向无功电能量（总、费率1~M）
	case	CON_DATA_REQ1_F45:	ucDT = 45	;break;	//F45	当月正向有功电能量（总、费率1~M）
	case	CON_DATA_REQ1_F46:	ucDT = 46	;break;	//F46	当月正向无功电能量（总、费率1~M）
	case	CON_DATA_REQ1_F47:	ucDT = 47	;break;	//F47	当月反向有功电能量（总、费率1~M）
	case	CON_DATA_REQ1_F48:	ucDT = 48	;break;	//F48	当月反向无功电能量（总、费率1~M）
	case	CON_DATA_REQ1_F49:	ucDT = 49	;break;	//F49	当前电压、电流相位角
//	case	CON_DATA_REQ1_F57:	ucDT = 57	;break;	//F57	当前A、B、C三相电压、电流2~N次谐波有效值
//	case	CON_DATA_REQ1_F58:	ucDT = 58	;break;	//F58	当前A、B、C三相电压、电流2~N次谐波含有率
//	case	CON_DATA_REQ1_F65:	ucDT = 65	;break;	//F65	当前电容器投切状态
//	case	CON_DATA_REQ1_F66:	ucDT = 66	;break;	//F66	当前电容器累计补偿投入时间和次数
//	case	CON_DATA_REQ1_F67:	ucDT = 67	;break;	//F67	当日、当月电容器累计补偿的无功电能量
//	case	CON_DATA_REQ1_F73:	ucDT = 73	;break;	//F73	直流模拟量实时数据
//	case	CON_DATA_REQ1_F81:	ucDT = 81	;break;	//F81	小时冻结总加有功功率
//	case	CON_DATA_REQ1_F82:	ucDT = 82	;break;	//F82	小时冻结总加无功功率
//	case	CON_DATA_REQ1_F83:	ucDT = 83	;break;	//F83	小时冻结总加有功总电能量
//	case	CON_DATA_REQ1_F84:	ucDT = 84	;break;	//F84	小时冻结总加无功总电能量
	case	CON_DATA_REQ1_F89:	ucDT = 89	;break;	//F89	小时冻结有功功率
	case	CON_DATA_REQ1_F90:	ucDT = 90	;break;	//F90	小时冻结A相有功功率
	case	CON_DATA_REQ1_F91:	ucDT = 91	;break;	//F91	小时冻结B相有功功率
	case	CON_DATA_REQ1_F92:	ucDT = 92	;break;	//F92	小时冻结C相有功功率
	case	CON_DATA_REQ1_F93:	ucDT = 93	;break;	//F93	小时冻结无功功率
	case	CON_DATA_REQ1_F94:	ucDT = 94	;break;	//F94	小时冻结A相无功功率
	case	CON_DATA_REQ1_F95:	ucDT = 95	;break;	//F95	小时冻结B相无功功率
	case	CON_DATA_REQ1_F96:	ucDT = 96	;break;	//F96	小时冻结C相无功功率
	case	CON_DATA_REQ1_F97:	ucDT = 97	;break;	//F97	小时冻结A相电压
	case	CON_DATA_REQ1_F98:	ucDT = 98	;break;	//F98	小时冻结B相电压
	case	CON_DATA_REQ1_F99:	ucDT = 99	;break;	//F99	小时冻结C相电压
	case	CON_DATA_REQ1_F100:	ucDT = 100	;break;	//F100	小时冻结A相电流
	case	CON_DATA_REQ1_F101:	ucDT = 101	;break;	//F101	小时冻结B相电流
	case	CON_DATA_REQ1_F102:	ucDT = 102	;break;	//F102	小时冻结C相电流
	case	CON_DATA_REQ1_F103:	ucDT = 103	;break;	//F103	小时冻结零序电流
	case	CON_DATA_REQ1_F105:	ucDT = 105	;break;	//F105	小时冻结正向有功总电能量
	case	CON_DATA_REQ1_F106:	ucDT = 106	;break;	//F106	小时冻结正向无功总电能量
	case	CON_DATA_REQ1_F107:	ucDT = 107	;break;	//F107	小时冻结反向有功总电能量
	case	CON_DATA_REQ1_F108:	ucDT = 108	;break;	//F108	小时冻结反向无功总电能量
	case	CON_DATA_REQ1_F109:	ucDT = 109	;break;	//F109	小时冻结正向有功总电能示值
	case	CON_DATA_REQ1_F110:	ucDT = 110	;break;	//F110	小时冻结正向无功总电能示值
	case	CON_DATA_REQ1_F111:	ucDT = 111	;break;	//F111	小时冻结反向有功总电能示值
	case	CON_DATA_REQ1_F112:	ucDT = 112	;break;	//F112	小时冻结反向无功总电能示值
	case	CON_DATA_REQ1_F113:	ucDT = 113	;break;	//F113	小时冻结总功率因数
	case	CON_DATA_REQ1_F114:	ucDT = 114	;break;	//F114	小时冻结A相功率因数
	case	CON_DATA_REQ1_F115:	ucDT = 115	;break;	//F115	小时冻结B相功率因数
	case	CON_DATA_REQ1_F116:	ucDT = 116	;break;	//F116	小时冻结C相功率因数
	case	CON_DATA_REQ1_F121:	ucDT = 121	;break;	//F121	小时冻结直流模拟量
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
        if(protocol_req1_fn_valid[(ucDT-1)/32]&(1<<((ucDT-1)%32))==0)//azh 无效则置0xff
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
	case	CON_DATA_REQ2_F1:	ucDT = 1	;break;	//F1	日冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
	case	CON_DATA_REQ2_F2:	ucDT = 2	;break;	//F2	日冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
	case	CON_DATA_REQ2_F3:	ucDT = 3	;break;	//F3	日冻结正向有/无功最大需量及发生时间（总、费率1~M）
	case	CON_DATA_REQ2_F4:	ucDT = 4	;break;	//F4	日冻结反向有/无功最大需量及发生时间（总、费率1~M）
	case	CON_DATA_REQ2_F5:	ucDT = 5	;break;	//F5	日冻结正向有功电能量（总、费率1~M）
	case	CON_DATA_REQ2_F6:	ucDT = 6	;break;	//F6	日冻结正向无功电能量（总、费率1~M）
	case	CON_DATA_REQ2_F7:	ucDT = 7	;break;	//F7	日冻结反向有功电能量（总、费率1~M）
	case	CON_DATA_REQ2_F8:	ucDT = 8	;break;	//F8	日冻结反向无功电能量（总、费率1~M）
	case	CON_DATA_REQ2_F9:	ucDT = 9	;break;	//F9	抄表日冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
	case	CON_DATA_REQ2_F10:	ucDT = 10	;break;	//F10	抄表日冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
	case	CON_DATA_REQ2_F11:	ucDT = 11	;break;	//F11	抄表日冻结正向有/无功最大需量及发生时间（总、费率1~M）
	case	CON_DATA_REQ2_F12:	ucDT = 12	;break;	//F12	抄表日冻结反向有/无功最大需量及发生时间（总、费率1~M）
	case	CON_DATA_REQ2_F17:	ucDT = 17	;break;	//F17	月冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
	case	CON_DATA_REQ2_F18:	ucDT = 18	;break;	//F18	月冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
	case	CON_DATA_REQ2_F19:	ucDT = 19	;break;	//F19	月冻结正向有/无功最大需量及发生时间（总、费率1~M）
	case	CON_DATA_REQ2_F20:	ucDT = 20	;break;	//F20	月冻结反向有/无功最大需量及发生时间（总、费率1~M）
	case	CON_DATA_REQ2_F21:	ucDT = 21	;break;	//F21	月冻结正向有功电能量（总、费率1~M）
	case	CON_DATA_REQ2_F22:	ucDT = 22	;break;	//F22	月冻结正向无功电能量（总、费率1~M）
	case	CON_DATA_REQ2_F23:	ucDT = 23	;break;	//F23	月冻结反向有功电能量（总、费率1~M）
	case	CON_DATA_REQ2_F24:	ucDT = 24	;break;	//F24	月冻结反向无功电能量（总、费率1~M）
	case	CON_DATA_REQ2_F25:	ucDT = 25	;break;	//F25	日分相最大有功功率及发生时间、有功功率为零时间
	case	CON_DATA_REQ2_F26:	ucDT = 26	;break;	//F26	日总及分相最大需量及发生时间
	case	CON_DATA_REQ2_F27:	ucDT = 27	;break;	//F27	日电压统计数据
	case	CON_DATA_REQ2_F28:	ucDT = 28	;break;	//F28	日不平衡度越限累计时间
	case	CON_DATA_REQ2_F29:	ucDT = 29	;break;	//F29	日电流越限统计
	case	CON_DATA_REQ2_F30:	ucDT = 30	;break;	//F30	日视在功率越限累计时间
	case	CON_DATA_REQ2_F31:	ucDT = 31	;break;	//F31	日电能表断相数据
	case	CON_DATA_REQ2_F32:	ucDT = 32	;break;	//F32	备用
	case	CON_DATA_REQ2_F33:	ucDT = 33	;break;	//F33	月分相最大有功功率及发生时间、分相有功功率为零时间
	case	CON_DATA_REQ2_F34:	ucDT = 34	;break;	//F34	月分相有功最大需量及发生时间、总有功最大需量及发生时间
	case	CON_DATA_REQ2_F35:	ucDT = 35	;break;	//F35	月电压统计数据
	case	CON_DATA_REQ2_F36:	ucDT = 36	;break;	//F36	月不平衡度越限累计时间
	case	CON_DATA_REQ2_F37:	ucDT = 37	;break;	//F37	月电流越限统计
	case	CON_DATA_REQ2_F38:	ucDT = 38	;break;	//F38	月视在功率越限累计时间
//	case	CON_DATA_REQ2_F41:	ucDT = 41	;break;	//F41	电容器投入累计时间和次数
//	case	CON_DATA_REQ2_F42:	ucDT = 42	;break;	//F42	日、月电容器累计补偿的无功电能量
	case	CON_DATA_REQ2_F43:	ucDT = 43	;break;	//F43	日功率因数区段累计时间
	case	CON_DATA_REQ2_F44:	ucDT = 44	;break;	//F44	月功率因数区段累计时间
	case	CON_DATA_REQ2_F49:	ucDT = 49	;break;	//F49	终端日供电时间、日复位累计次数
//	case	CON_DATA_REQ2_F50:	ucDT = 50	;break;	//F50	终端日控制统计数据
	case	CON_DATA_REQ2_F51:	ucDT = 51	;break;	//F51	终端月供电时间、月复位累计次数
//	case	CON_DATA_REQ2_F52:	ucDT = 52	;break;	//F52	终端月控制统计数据
	case	CON_DATA_REQ2_F53:	ucDT = 53	;break;	//F53	终端与主站日通信流量
	case	CON_DATA_REQ2_F54:	ucDT = 54	;break;	//F54	终端与主站月通信流量
//	case	CON_DATA_REQ2_F57:	ucDT = 57	;break;	//F57	总加组日最大、最小有功功率及其发生时间，有功功率为零日累计时间
//	case	CON_DATA_REQ2_F58:	ucDT = 58	;break;	//F58	总加组日累计有功电能量（总、费率1~M）
//	case	CON_DATA_REQ2_F59:	ucDT = 59	;break;	//F59	总加组日累计无功电能量（总、费率1~M）
//	case	CON_DATA_REQ2_F60:	ucDT = 60	;break;	//F60	总加组月最大、最小有功功率及其发生时间，有功功率为零月累计时间
//	case	CON_DATA_REQ2_F61:	ucDT = 61	;break;	//F61	总加组月累计有功电能量（总、费率1~M）
//	case	CON_DATA_REQ2_F62:	ucDT = 62	;break;	//F62	总加组月累计无功电能量（总、费率1~M）
//	case	CON_DATA_REQ2_F65:	ucDT = 65	;break;	//F65	总加组超功率定值的月累计时间、月累计电能量
//	case	CON_DATA_REQ2_F66:	ucDT = 66	;break;	//F66	总加组超月电能量定值的月累计时间、累计电能量
//	case	CON_DATA_REQ2_F73:	ucDT = 73	;break;	//F73	总加组有功功率曲线
//	case	CON_DATA_REQ2_F74:	ucDT = 74	;break;	//F74	总加组无功功率曲线
//	case	CON_DATA_REQ2_F75:	ucDT = 75	;break;	//F75	总加组有功电能量曲线
//	case	CON_DATA_REQ2_F76:	ucDT = 76	;break;	//F76	总加组无功电能量曲线
	case	CON_DATA_REQ2_F81:	ucDT = 81	;break;	//F81	有功功率曲线
	case	CON_DATA_REQ2_F82:	ucDT = 82	;break;	//F82	A相有功功率曲线
	case	CON_DATA_REQ2_F83:	ucDT = 83	;break;	//F83	B相有功功率曲线
	case	CON_DATA_REQ2_F84:	ucDT = 84	;break;	//F84	C相有功功率曲线
	case	CON_DATA_REQ2_F85:	ucDT = 85	;break;	//F85	无功功率曲线
	case	CON_DATA_REQ2_F86:	ucDT = 86	;break;	//F86	A相无功功率曲线
	case	CON_DATA_REQ2_F87:	ucDT = 87	;break;	//F87	B相无功功率曲线
	case	CON_DATA_REQ2_F88:	ucDT = 88	;break;	//F88	C相无功功率曲线
	case	CON_DATA_REQ2_F89:	ucDT = 89	;break;	//F89	A相电压曲线
	case	CON_DATA_REQ2_F90:	ucDT = 90	;break;	//F90	B相电压曲线
	case	CON_DATA_REQ2_F91:	ucDT = 91	;break;	//F91	C相电压曲线
	case	CON_DATA_REQ2_F92:	ucDT = 92	;break;	//F92	A相电流曲线
	case	CON_DATA_REQ2_F93:	ucDT = 93	;break;	//F93	B相电流曲线
	case	CON_DATA_REQ2_F94:	ucDT = 94	;break;	//F94	C相电流曲线
	case	CON_DATA_REQ2_F95:	ucDT = 95	;break;	//F95	零序电流曲线
	case	CON_DATA_REQ2_F97:	ucDT = 97	;break;	//F97	正向有功总电能量
	case	CON_DATA_REQ2_F98:	ucDT = 98	;break;	//F98	正向无功总电能量
	case	CON_DATA_REQ2_F99:	ucDT = 99	;break;	//F99	反向有功总电能量
	case	CON_DATA_REQ2_F100:	ucDT = 100	;break;	//F100	反向无功总电能量
	case	CON_DATA_REQ2_F101:	ucDT = 101	;break;	//F101	正向有功总电能示值
	case	CON_DATA_REQ2_F102:	ucDT = 102	;break;	//F102	正向无功总电能示值
	case	CON_DATA_REQ2_F103:	ucDT = 103	;break;	//F103	反向有功总电能示值
	case	CON_DATA_REQ2_F104:	ucDT = 104	;break;	//F104	反向无功总电能示值
	case	CON_DATA_REQ2_F105:	ucDT = 105	;break;	//F105	总功率因数
	case	CON_DATA_REQ2_F106:	ucDT = 106	;break;	//F106	A相功率因数
	case	CON_DATA_REQ2_F107:	ucDT = 107	;break;	//F107	B相功率因数
	case	CON_DATA_REQ2_F108:	ucDT = 108	;break;	//F108	C相功率因数
	case	CON_DATA_REQ2_F109:	ucDT = 109	;break;	
	case	CON_DATA_REQ2_F110:	ucDT = 110	;break;	
//	case	CON_DATA_REQ2_F113:	ucDT = 113	;break;	//F113	A相2~19次谐波电流日最大值及发生时间
//	case	CON_DATA_REQ2_F114:	ucDT = 114	;break;	//F114	B相2~19次谐波电流日最大值及发生时间
//	case	CON_DATA_REQ2_F115:	ucDT = 115	;break;	//F115	C相2~19次谐波电流日最大值及发生时间
//	case	CON_DATA_REQ2_F116:	ucDT = 116	;break;	//F116	A相2~19次谐波电压含有率及总畸变率日最大值及发生时间
//	case	CON_DATA_REQ2_F117:	ucDT = 117	;break;	//F117	B相2~19次谐波电压含有率及总畸变率日最大值及发生时间
//	case	CON_DATA_REQ2_F118:	ucDT = 118	;break;	//F118	C相2~19次谐波电压含有率及总畸变率日最大值及发生时间
//	case	CON_DATA_REQ2_F121:	ucDT = 121	;break;	//F121	A相谐波越限日统计数据
//	case	CON_DATA_REQ2_F122:	ucDT = 122	;break;	//F122	B相谐波越限日统计数据
//	case	CON_DATA_REQ2_F123:	ucDT = 123	;break;	//F123	C相谐波越限日统计数据
//	case	CON_DATA_REQ2_F129:	ucDT = 129	;break;	//F129	直流模拟量越限日累计时间、最大/最小值及发生时间
//	case	CON_DATA_REQ2_F130:	ucDT = 130	;break;	//F130	直流模拟量越限月累计时间、最大/最小值及发生时间
//	case	CON_DATA_REQ2_F138:	ucDT = 138	;break;	//F138	直流模拟量数据曲线
	case	CON_DATA_REQ2_F145:	ucDT = 145	;break;	//F145	一象限无功总电能示值曲线
	case	CON_DATA_REQ2_F146:	ucDT = 146	;break;	//F146	四象限无功总电能示值曲线
	case	CON_DATA_REQ2_F147:	ucDT = 147	;break;	//F147	二象限无功总电能示值曲线
	case	CON_DATA_REQ2_F148:	ucDT = 148	;break;	//F148	三象限无功总电能示值曲线
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
        if(protocol_req2_fn_valid[(ucDT-1)/32]&(1<<((ucDT-1)%32))==0)//azh 无效则置0xff
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
	case	CON_DATA_REQ3_F1:		ucDT =	1	;break;	//重要事件						
	case	CON_DATA_REQ3_F2:		ucDT =	2	;break;	//一般事件
	}
	return ucDT;
}

 unsigned char GetDTbyItemLabel_ExtCmd(unsigned short usItemLabel)
 {
 	unsigned char ucDT=0xFF;
	switch(usItemLabel)
	{
//	case	CON_EXTCMD_F1:			ucDT=1		;break;	//F1 内存读						
//	case	CON_EXTCMD_F2:			ucDT=2		;break;	//F2 内存写
//	case	CON_EXTCMD_F3:			ucDT=3		;break;	//F3 DATAFLASH读
//	case	CON_EXTCMD_F4:			ucDT=4		;break;	//F4 DATAFLASH写
//	case	CON_EXTCMD_F5:			ucDT=5		;break;	//F5 看门狗测试，让程序进入死循环
//	case	CON_EXTCMD_F6:			ucDT=6		;break;	//F6 手机模块通讯检测
//	case	CON_EXTCMD_F7:			ucDT=7		;break;	//F7 终端状态检测
	case	CON_EXTCMD_F10:		ucDT=10		;break;	//F10 终端地址10/16进制切换
//	case	CON_EXTCMD_F11:		ucDT=11		;break;	//F11 中文信息读取
//	case	CON_EXTCMD_F21:		ucDT=21		;break;	//F21 GR任务读					
//	case	CON_EXTCMD_F22:		ucDT=22		;break;	//F22 GR任务配置
//	case	CON_EXTCMD_F23:		ucDT=23		;break;	//F23 GR任务信息读
//	case	CON_EXTCMD_F24:		ucDT=24		;break;	//F24 GR任务信息保存
//	case	CON_EXTCMD_F31:		ucDT=31		;break;	//F31 脉冲需量清零					
//	case	CON_EXTCMD_F33:		ucDT=33		;break;	//F33 脉冲底度设置内容全
//	case	CON_EXTCMD_F40:		ucDT=40		;break;	//F40 设置内表数据
//	case	CON_EXTCMD_F41:		ucDT=41		;break;	//F41 读取内表数据
//	case	CON_EXTCMD_F51:		ucDT=51		;break;	//F51 终端地址					
//	case	CON_EXTCMD_F52:		ucDT=52		;break;	//F52 终端序号读
//	case	CON_EXTCMD_F53:		ucDT=53		;break;	//F53 终端地址行政区号写
//	case	CON_EXTCMD_F54:		ucDT=54		;break;	//F54 终端序号写
//	case	CON_EXTCMD_F71:		ucDT=71		;break;	//F71 蜂鸣器控制
//	case	CON_EXTCMD_F72:		ucDT=72		;break;	//F72 修改时钟
//	case	CON_EXTCMD_F73:		ucDT=73		;break;	//F73 查询通讯波特率
//	case	CON_EXTCMD_F74:		ucDT=74		;break;	//F74 修改通讯波特率
//	case	CON_EXTCMD_F75:		ucDT=75		;break;	//F75 读取液晶轮显显示参数
//	case	CON_EXTCMD_F76:		ucDT=76		;break;	//F76 读取液晶操作显示参数
//	case	CON_EXTCMD_F77:		ucDT=77		;break;	//F77 读取液晶特殊显示参数
//	case	CON_EXTCMD_F78:		ucDT=78		;break;	//F78 修改液晶轮显显示参数
//	case	CON_EXTCMD_F79:		ucDT=79		;break;	//F79 修改液晶操作显示参数
//	case	CON_EXTCMD_F80:		ucDT=80		;break;	//F80 修改液晶特殊显示参数
//	case	CON_EXTCMD_F81:		ucDT=81		;break;	//F81 启用SIM卡的pin
//	case	CON_EXTCMD_F82:		ucDT=82		;break;	//F82 修改SIM卡的pin
//	case	CON_EXTCMD_F83:		ucDT=83		;break;	//F83 用PUK设置SIM卡的pin
//	case	CON_EXTCMD_F84:		ucDT=84		;break;	//F84 SIM卡的pin相关状态
//	case	CON_EXTCMD_F85:		ucDT=85		;break;	//F85 GSM/GPRS切换
//	case	CON_EXTCMD_F86:		ucDT=86		;break;	//F86 读功率控制恢复时间长度
//	case	CON_EXTCMD_F87:		ucDT=87		;break;	//F87 写功率控制恢复时间长度
//	case	CON_EXTCMD_F89:		ucDT=89		;break;	//F89 读功率控制越限确认时间
//	case	CON_EXTCMD_F90:		ucDT=90		;break;	//F90 写功率控制越限确认时间
//	case	CON_EXTCMD_F91:		ucDT=91		;break;	//F91 读自定义调整参数
//	case	CON_EXTCMD_F92:		ucDT=92		;break;	//F92 写自定义调整参数
//	case	CON_EXTCMD_F93:		ucDT=93		;break;	//F93 读取终端电压两位小数电流四位小数
//	case	CON_EXTCMD_F94:		ucDT=94		;break;	//F94 任务初始化
//	case	CON_EXTCMD_F95:		ucDT=95		;break;	//F95 8025时钟矫正
//	case	CON_EXTCMD_F96:		ucDT=96		;break;	//F96 液晶对比度设置
//	case	CON_EXTCMD_F97:		ucDT=97		;break;	//F97 设置表计通迅名和密码
//	case	CON_EXTCMD_F98:		ucDT=98		;break;	//F98 读取表计通迅名和密码
//	case	CON_EXTCMD_F99:		ucDT=99		;break;	//F99 设置GPRS接入的PDP用户名、密码
//	case	CON_EXTCMD_F100:	ucDT=100	;break;	//F100 读取GPRS接入的PDP用户名、密码
//	case	CON_EXTCMD_F101:	ucDT=101	;break;	//F101 设置GPRS重连间隔时间
//	case	CON_EXTCMD_F102:	ucDT=102	;break;	//F102 读取GPRS重连间隔时间
//	case	CON_EXTCMD_F109:	ucDT=109	;break;	//F109 设置终端工作模式
//	case	CON_EXTCMD_F110:	ucDT=110	;break;	//F110 读取终端工作模式
//	case	CON_EXTCMD_F111:	ucDT=111	;break;	//F111 设置CT类型
//	case	CON_EXTCMD_F112:	ucDT=112	;break;	//F112 读取CT类型
//	case	CON_EXTCMD_F113:	ucDT=113	;break;	//F113 设置自学习有效标志
//	case	CON_EXTCMD_F114:	ucDT=114	;break;	//F114 读取自学习有效标志
//	case	CON_EXTCMD_F115:	ucDT=115	;break;	//F115
//	case	CON_EXTCMD_F116:	ucDT=116	;break;	//F116
//	case	CON_EXTCMD_F117:	ucDT=117	;break;	//F117
//	case	CON_EXTCMD_F118:	ucDT=118	;break;	//F118
//	case	CON_EXTCMD_F119:	ucDT=119	;break;	//F119
//	case	CON_EXTCMD_F120:	ucDT=120	;break;	//F120 设置AES加密密钥
//	case	CON_EXTCMD_F121:	ucDT=121	;break;	//F121 读取AES加密密钥	
//	case	CON_EXTCMD_F122:	ucDT=122	;break; //F122 投入接解除断线保电控制
//	case	CON_EXTCMD_F123:	ucDT=123	;break; //F122 投入接解除断线保电控制
//	case	CON_EXTCMD_F124:	ucDT=124	;break;	//F124 读取校表EEPROM的参数
//	case	CON_EXTCMD_F125:	ucDT=125	;break;	//F125 设置校表EEPROM的参数
//	case 	CON_EXTCMD_F126:	ucDT=126	;break;	
	}
 	return ucDT;
 }

unsigned char GetDTbyItemLabel_DataTrans(unsigned short usItemLabel)
{
	unsigned char ucDT=0xFF;
	switch(usItemLabel)
	{
	case	CON_DATATRANS_F1:		ucDT=1		;break;	//F1 转发命令
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
	case	CON_FILETRANS_F1:		ucDT=1		;break;	//F1 文件传输方式1						
	case	CON_FILETRANS_F100:		ucDT=100	;break;	//F100 程序下装命令					
	}
	return ucDT;
}

unsigned short GetItemLabelbyDT_Ack(unsigned char ucDT)
{
	unsigned short usItemLabel = CON_DI_TYPE_INVALID;
	switch(ucDT)
	{
	case	1:	usItemLabel = CON_ACK_F1;	break;		//F1:全部确认
	case	2:	usItemLabel = CON_ACK_F2;	break;		//F2:全部否认
	case	3:	usItemLabel = CON_ACK_F3;	break;		//F3:按数据项进行确认否认
	}
	return usItemLabel;
}

unsigned short GetItemLabelbyDT_RST(unsigned char ucDT)
{
	unsigned short usItemLabel = CON_DI_TYPE_INVALID;
	switch(ucDT)
	{
	case	1:	usItemLabel = CON_RST_F1_HARD;	break;		//F1:硬件初始化
	case	2:	usItemLabel = CON_RST_F2_DATA;	break;		//F2:数据区初始化
	case	3:	usItemLabel = CON_RST_F3_ALL;	break;		//F3:全体数据
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
	case	1:	usItemLabel = CON_PARA_F1_COMMPARA;		break;	//F1：终端通信参数设置
	case	2:	usItemLabel = CON_PARA_F2_RELAYPARA;	break;	//F2：终端中继转发设置
	case	3:	usItemLabel = CON_PARA_F3_STAIPADDR;	break;	//F3：主站IP地址和端口
	case	4:	usItemLabel = CON_PARA_F4_STAPHONENO;	break;	//F4：主站电话号码和短信中心号码
	case	5:	usItemLabel = CON_PARA_F5_TERENCRYPT;	break;	//F5：终端密码设置
	case	6:	usItemLabel = CON_PARA_F6_TERGROUPADDR;	break;	//F6：终端组地址设置
	case	7:	usItemLabel = CON_PARA_F7_IPADDR;		break;	//F7：终端抄表日设置
	case	8:	usItemLabel = CON_PARA_F8_ALRCTLWORD;	break;	//F8：终端事件记录配置设置
	case	9:	usItemLabel = CON_PARA_F9_CONFIGNUM;	break;	//F9：终端配置数量表
	case	10:	usItemLabel = CON_PARA_F10_METERPARA;	break;	//F10：终端电能表/交流采样装置配置参数
//	case	11:	usItemLabel = CON_PARA_F11_PULSEPARA;	break;	//F11：终端脉冲配置参数
//	case	12:	usItemLabel = CON_PARA_F12_DINPUTPARA;	break;	//F12：终端状态量输入参数
//	case	13:	usItemLabel = CON_PARA_F13_ANALOGPARA;	break;	//F13：终端电压/电流模拟量配置参数
//	case	14:	usItemLabel = CON_PARA_F14_SUMGPARA;	break;	//F14：终端总加组配置参数
//	case	15:	usItemLabel = CON_PARA_F15_POWVALDIFF;	break;	//F15：有功总电能量差动越限事件参数设置
	case	16:	usItemLabel = CON_PARA_F16_PDPINFO;	break;	//F16：虚拟专网用户名、密码
//	case	17:	usItemLabel = CON_PARA_F17_KEEPPOWER;	break;	//F17：终端保安定值
//	case	18:	usItemLabel = CON_PARA_F18_POWCTRLTIMEPER;	break;	//F18：终端功控时段
//	case	19:	usItemLabel = CON_PARA_F19_POWCTRLCOEF;	break;	//F19：终端时段功控定值浮动系数
//	case	20:	usItemLabel = CON_PARA_F20_ENERCTRLCOEF;	break;	//F20：终端月电能量控定值浮动系数
//	case	21:	usItemLabel = CON_PARA_F21_PERTIMEFEE;	break;	//F21：终端电能量费率时段和费率数
//	case	22:	usItemLabel = CON_PARA_F22_FEEPARA;		break;	//F22：终端电能量费率
//	case	23:	usItemLabel = CON_PARA_F23_URGEALARMPARA;	break;	//F23：终端催费告警参数
	case	24:	usItemLabel = CON_PARA_F24_COPYINTER;	break;	//F24：终端抄表间隔设置
	case	25:	usItemLabel = CON_PARA_F25_MPBASEPARA;	break;	//F25：测量点基本参数
	case	26:	usItemLabel = CON_PARA_F26_MPLIMIT;		break;	//F26：测量点限值参数
	case	27:	usItemLabel = CON_PARA_F27_MPFREEZEPARA;	break;	//F27：测量点数据冻结参数
	case	28:	usItemLabel = CON_PARA_F28_MPPOWRATEPARA;	break;	//F28：测量点功率因数分段限值
	case	29:	usItemLabel = CON_PARA_F29_METERADDR;	break;
	case 	30:	usItemLabel = CON_PARA_F30_METERCOPYFLAG;	break;	//F30:
	case 	31: usItemLabel = CON_PARA_F31_SLAVENODE;		break;
	case 	34:	usItemLabel = CON_PARA_F34_PORTPROPERTY;break;
	case 	35:	usItemLabel = CON_PARA_F35_EMPHAFLAG;	break;
//	case	33:	usItemLabel = CON_PARA_F33_SUMGFREEZEPARA;	break;	//F33：总加组数据冻结参数
	case 	37:	usItemLabel = CON_PARA_F37_LINKPARA;			break;	//F37:级联通信参数
	case 	38:	usItemLabel = CON_PARA_F38_JOB1PARA;			break;	//F38:一类数据配置设置
	case 	39:  usItemLabel = CON_PARA_F39_JOB2PARA;			break;	//F39:二类数据配置设置
//	case	41:	usItemLabel = CON_PARA_F41_TIMEPERSETTING;	break;	//F41：时段功控定值
//	case	42:	usItemLabel = CON_PARA_F42_RESTCTRL;	break;	//F42：厂休功控参数
//	case	43:	usItemLabel = CON_PARA_F43_POWERSLIP;	break;	//F43：功率控制的功率计算滑差时间
//	case	44:	usItemLabel = CON_PARA_F44_SALESTOP;	break;	//F44：营业报停控参数
//	case	45:	usItemLabel = CON_PARA_F45_POWERCTRLTURN;	break;	//F45：功控轮次设定
//	case	46:	usItemLabel = CON_PARA_F46_MONTHENERCTRL;	break;	//F46：月电量控定值
//	case	47:	usItemLabel = CON_PARA_F47_BUYENERCTRL;	break;	//F47：购电量控参数
//	case	48:	usItemLabel = CON_PARA_F48_ENERCTRLTURN;	break;	//F48：电控轮次设定
//	case	49:	usItemLabel = CON_PARA_F49_CTRLALRTIME;	break;	//F49：功控告警时间
	case	57:	usItemLabel = CON_PARA_F57_SOUNDALARM;	break;	//F57：终端声音告警允许∕禁止设置
//	case	58:	usItemLabel = CON_PARA_F58_KEEPAUTO;	break;	//F58：终端自动保电参数
	case	59:	usItemLabel = CON_PARA_F59_MTRERRORPARA;	break;	//F59：电能表异常判别阈值设定
//	case	60:	usItemLabel = CON_PARA_F60_HARMONPARA;	break;	//F60：谐波限值
//	case	61:	usItemLabel = CON_PARA_F61_DCAINPARA;	break;	//F61：直流模拟量接入参数
	case	62:	usItemLabel = CON_PARA_F62_COMMTYPE;	break;	//F62 虚拟专网工作方式
	case	36:	usItemLabel = CON_PARA_F63_FLUXLIMIT;	break;	//F63 日通信流量门限设置
	case	65:	usItemLabel = CON_PARA_F65_UPDATA1PARA;	break;	//F65：定时发送1类数据任务设置
	case	66:	usItemLabel = CON_PARA_F66_UPDATA2PARA;	break;	//F66：定时发送2类数据任务设置
	case	67:	usItemLabel = CON_PARA_F67_UPDATA1CTRL;	break;	//F67：定时发送1类数据任务启动/停止设置
	case	68:	usItemLabel = CON_PARA_F68_UPDATA2CTRL;	break;	//F68：定时发送2类数据任务启动/停止设置
	case	73:	usItemLabel = CON_PARA_F73_CAPPARA;		break;	//F73：电容器参数
//	case	74:	usItemLabel = CON_PARA_F74_CAPRUNPARA;	break;	//F74：电容器投切运行参数
//	case	75:	usItemLabel = CON_PARA_F75_CAPRELAYPARA;	break;	//F75：电容器保护参数
//	case	76:	usItemLabel = CON_PARA_F76_CAPCTRL;		break;	//F76：电容器投切控制方式
//	case	81:	usItemLabel = CON_PARA_F81_DCARATE;		break;	//F81：直流模拟量输入变比
//	case	82:	usItemLabel = CON_PARA_F82_DCALIMIT;	break;	//F82：直流模拟量限值
//	case	83:	usItemLabel = CON_PARA_F83_DCAFREESE;	break;	//F83：直流模拟量冻结参数
	case	89:	usItemLabel = CON_PARA_F89_ADDR;	    break;	//F89：终端地址信息
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
	case 1:		usItemLabel = CON_CONFIG_F1_VERSION;			break;//F1配置终端版本信息
	case 2:		usItemLabel = CON_CONFIG_F2_INTERFACE;			break;//F2配置终端输入、输出及通信端口配置
	case 3:		usItemLabel = CON_CONFIG_F3_OTHER;				break;//F3配置终端其他配置
	case 4:		usItemLabel = CON_CONFIG_F4_PARA;				break;//F4配置终端参数配置（本终端软硬件版本支持的参数）
	case 5:		usItemLabel = CON_CONFIG_F5_CTRL;				break;//F5配置终端控制配置（本终端软硬件版本支持的控制命令）
	case 6:		usItemLabel = CON_CONFIG_F6_DATA1;				break;//F6配置终端1类数据配置（本终端软硬件版本支持的1类数据）
	case 7:		usItemLabel = CON_CONFIG_F7_DATA2;				break;//F7配置终端2类数据配置（本终端软硬件版本支持的2类数据）
	case 8:		usItemLabel = CON_CONFIG_F8_ERC;				break;//F8配置终端事件记录配置（本终端软硬件版本支持的事件记录）
//azh 170920
	case 9:		usItemLabel = CON_CONFIG_F9_REMOTEVER;			break;//F9远程通信模块版本信息    
	case 10:	usItemLabel = CON_CONFIG_F10_LOCALVER;			break;//F10本地通信模块版本信息    	    
	}
	return usItemLabel;
}


unsigned short GetItemLabelbyDT_Login(unsigned char ucDT)
{
	unsigned short usItemLabel=CON_DI_TYPE_INVALID;
	switch(ucDT)
	{
	case 1:		usItemLabel = CON_LOGIN_F1_REQUEST;			break;//F1身份认证请求
	case 2:		usItemLabel = CON_LOGIN_F2_ANSWER;			break;//F2身份认证响应
	}
	return usItemLabel;
}


unsigned short GetItemLabelbyDT_Ctrl(unsigned char ucDT)
{
	unsigned short usItemLabel=CON_DI_TYPE_INVALID;
/*
	switch(ucDT)
	{
//	case 1:		usItemLabel = CON_CTRL_F1_YK_OPEN;			break;//F1遥控跳闸
//	case 2:		usItemLabel = CON_CTRL_F2_YK_CLOSE;			break;//F2遥控允许合闸
//	case 9:		usItemLabel = CON_CTRL_F9_TIMEPER_ON;		break;//F9时段功控投入
//	case 10:	usItemLabel = CON_CTRL_F10_RESTCTRL_ON;		break;//F10厂休功控投入
//	case 11:	usItemLabel = CON_CTRL_F11_SALESTOP_ON;		break;//F11营业报停功控投入
//	case 12:	usItemLabel = CON_CTRL_F12_POWERLOWER_ON;	break;//F12当前功率下浮控投入
//	case 15:	usItemLabel = CON_CTRL_F15_MONTHENER_ON;	break;//F15月电控投入
//	case 16:	usItemLabel = CON_CTRL_F16_BUYENER_ON;		break;//F16购电控投入
//	case 17:	usItemLabel = CON_CTRL_F17_TIMEPER_OFF;		break;//F17时段功控解除
//	case 18:	usItemLabel = CON_CTRL_F18_RESTCTRL_OFF;	break;//F18厂休功控解除
//	case 19:	usItemLabel = CON_CTRL_F19_SALESTOP_OFF;	break;//F19营业报停功控解除
//	case 20:	usItemLabel = CON_CTRL_F20_POWERLOWER_OFF;	break;//F20当前功率下浮控解除
//	case 23:	usItemLabel = CON_CTRL_F23_MONTHENER_OFF;	break;//F23月电控解除
//	case 24:	usItemLabel = CON_CTRL_F24_BUYENER_OFF;		break;//F24购电控解除
//	case 25:	usItemLabel = CON_CTRL_F25_KEEPPOWER_ON;	break;//F25终端保电投入
//	case 26:	usItemLabel = CON_CTRL_F26_URGEALARM_ON;	break;//F26催费告警投入
	case 27:	usItemLabel = CON_CTRL_F27_TALK_ON;			break;//F27允许终端与主站通话
//	case 28:	usItemLabel = CON_CTRL_F28_REMOVE_ON;		break;//F28终端剔除投入
	case 29:	usItemLabel = CON_CTRL_F29_UPACTIVE_ON;		break;//F29允许终端主动上报
	case 31:	usItemLabel = CON_CTRL_F31_SETTIME;			break;//F31对时命令
	case 32:	usItemLabel = CON_CTRL_F32_MESSAGE;			break;//F32中文信息
//	case 33:	usItemLabel = CON_CTRL_F33_KEEPPOWER_OFF;	break;//F33终端保电解除
//	case 34:	usItemLabel = CON_CTRL_F34_URGEFEE_OFF;		break;//F34催费告警解除
	case 35:	usItemLabel = CON_CTRL_F35_TALK_OFF;		break;//F35禁止终端与主站通话
//	case 36:	usItemLabel = CON_CTRL_F36_REMOVE_OFF;		break;//F36终端剔除解除
	case 37:	usItemLabel = CON_CTRL_F37_UPACTIVE_OFF;	break;//F37禁止终端主动上报
	case 39:	usItemLabel = CON_CTRL_F39_DISCONECT;		break;//F38短信激活
//	case 41:	usItemLabel = CON_CTRL_F41_CAPCTRL_ON;		break;//F41电容控制投入
//	case 42:	usItemLabel = CON_CTRL_F42_CAPCTRL_OFF;		break;//F42电容控制解除
//	case 88:	usItemLabel = CON_CTRL_F88_TIMEREVISE;		break;//F88精确对时命令

	case 38:	usItemLabel = CON_CTRL_F38_SMS_ACTIVE;		break;//F38		changed for test 09-11-25
//	case 49:	usItemLabel = CON_CTRL_F49;		break;//F41电容控制投入
//	case 50:	usItemLabel = CON_CTRL_F50;		break;//F41电容控制投入
//	case 51:	usItemLabel = CON_CTRL_F51;		break;//F41电容控制投入
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
	case	1:		usItemLabel = CON_TASKDATA_REQ_F1;		break;	//F1 1类数据
	case	2:		usItemLabel = CON_TASKDATA_REQ_F2;		break;	//F2 2类数据
	}
	return usItemLabel;
}

unsigned short GetItemLabelbyDT_ReqData_1(unsigned char ucDT)
{
	unsigned short usItemLabel=CON_DI_TYPE_INVALID;
/*
	switch(ucDT)
	{
	case	1:	usItemLabel = 	CON_DATA_REQ1_F1	;break;	//F1	终端版本信息
	case	2:	usItemLabel = 	CON_DATA_REQ1_F2	;break;	//F2	终端日历时钟
	case	3:	usItemLabel = 	CON_DATA_REQ1_F3	;break;	//F3	终端参数状态
	case	4:	usItemLabel = 	CON_DATA_REQ1_F4	;break;	//F4	终端通信状态
//	case	5:	usItemLabel = 	CON_DATA_REQ1_F5	;break;	//F5	终端控制设置状态
//	case	6:	usItemLabel = 	CON_DATA_REQ1_F6	;break;	//F6	终端当前控制状态
	case	7:	usItemLabel = 	CON_DATA_REQ1_F7	;break;	//F7	终端事件计数器当前值
	case	8:	usItemLabel = 	CON_DATA_REQ1_F8	;break;	//F8	终端事件标志状态
	case	9:	usItemLabel = 	CON_DATA_REQ1_F9	;break;	//F9	终端状态量及变位标志
	case	10:	usItemLabel = 	CON_DATA_REQ1_F10	;break;	//F10	终端与主站当日、月通信流量
	case	11:	usItemLabel = 	CON_DATA_REQ1_F11	;break;	//F11	当前信号强度
//	case	17:	usItemLabel = 	CON_DATA_REQ1_F17	;break;	//F17	当前总加有功功率
//	case	18:	usItemLabel = 	CON_DATA_REQ1_F18	;break;	//F18	当前总加无功功率
//	case	19:	usItemLabel = 	CON_DATA_REQ1_F19	;break;	//F19	当日总加有功电能量（总、费率1~M）
//	case	20:	usItemLabel = 	CON_DATA_REQ1_F20	;break;	//F20	当日总加无功电能量（总、费率1~M）
//	case	21:	usItemLabel = 	CON_DATA_REQ1_F21	;break;	//F21	当月总加有功电能量（总、费率1~M）
//	case	22:	usItemLabel = 	CON_DATA_REQ1_F22	;break;	//F22	当月总加无功电能量（总、费率1~M）
//	case	23:	usItemLabel = 	CON_DATA_REQ1_F23	;break;	//F23	终端当前剩余电量（费）
//	case	24:	usItemLabel = 	CON_DATA_REQ1_F24	;break;	//F24	当前功率下浮控控后总加有功功率冻结值
	case	25:	usItemLabel = 	CON_DATA_REQ1_F25	;break;	//F25	当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流
	case	26:	usItemLabel = 	CON_DATA_REQ1_F26	;break;	//F26	A、B、C三相断相统计数据及最近一次断相记录
	case	27:	usItemLabel = 	CON_DATA_REQ1_F27	;break;	//F27	电能表日历时钟、编程次数及其最近一次操作时间
	case	28:	usItemLabel = 	CON_DATA_REQ1_F28	;break;	//F28	电表运行状态字及其变位标志
	case 	31:	usItemLabel = 	CON_DATA_REQ1_F31	;break;
	case 	32:	usItemLabel = 	CON_DATA_REQ1_F32	;break;
	case	33:	usItemLabel = 	CON_DATA_REQ1_F33	;break;	//F33	当前正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
	case	34:	usItemLabel = 	CON_DATA_REQ1_F34	;break;	//F34	当前反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
	case	35:	usItemLabel = 	CON_DATA_REQ1_F35	;break;	//F35	当月正向有/无功最大需量及发生时间（总、费率1~M）
	case	36:	usItemLabel = 	CON_DATA_REQ1_F36	;break;	//F36	当月反向有/无功最大需量及发生时间（总、费率1~M）
	case	37:	usItemLabel = 	CON_DATA_REQ1_F37	;break;	//F37	上月正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
	case	38:	usItemLabel = 	CON_DATA_REQ1_F38	;break;	//F38	上月反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
	case	39:	usItemLabel = 	CON_DATA_REQ1_F39	;break;	//F39	上月正向有/无功最大需量及发生时间（总、费率1~M）
	case	40:	usItemLabel = 	CON_DATA_REQ1_F40	;break;	//F40	上月反向有/无功最大需量及发生时间（总、费率1~M）
	case	41:	usItemLabel = 	CON_DATA_REQ1_F41	;break;	//F41	当日正向有功电能量（总、费率1~M）
	case	42:	usItemLabel = 	CON_DATA_REQ1_F42	;break;	//F42	当日正向无功电能量（总、费率1~M）
	case	43:	usItemLabel = 	CON_DATA_REQ1_F43	;break;	//F43	当日反向有功电能量（总、费率1~M）
	case	44:	usItemLabel = 	CON_DATA_REQ1_F44	;break;	//F44	当日反向无功电能量（总、费率1~M）
	case	45:	usItemLabel = 	CON_DATA_REQ1_F45	;break;	//F45	当月正向有功电能量（总、费率1~M）
	case	46:	usItemLabel = 	CON_DATA_REQ1_F46	;break;	//F46	当月正向无功电能量（总、费率1~M）
	case	47:	usItemLabel = 	CON_DATA_REQ1_F47	;break;	//F47	当月反向有功电能量（总、费率1~M）
	case	48:	usItemLabel = 	CON_DATA_REQ1_F48	;break;	//F48	当月反向无功电能量（总、费率1~M）
	case	49:	usItemLabel = 	CON_DATA_REQ1_F49	;break;	//F49	当前电压、电流相位角
	case	57:	usItemLabel = 	CON_DATA_REQ1_F57	;break;	//F57	当前A、B、C三相电压、电流2~N次谐波有效值
	case	58:	usItemLabel = 	CON_DATA_REQ1_F58	;break;	//F58	当前A、B、C三相电压、电流2~N次谐波含有率
//	case	65:	usItemLabel = 	CON_DATA_REQ1_F65	;break;	//F65	当前电容器投切状态
//	case	66:	usItemLabel = 	CON_DATA_REQ1_F66	;break;	//F66	当前电容器累计补偿投入时间和次数
//	case	67:	usItemLabel = 	CON_DATA_REQ1_F67	;break;	//F67	当日、当月电容器累计补偿的无功电能量
//	case	73:	usItemLabel = 	CON_DATA_REQ1_F73	;break;	//F73	直流模拟量实时数据
//	case	81:	usItemLabel = 	CON_DATA_REQ1_F81	;break;	//F81	小时冻结总加有功功率
//	case	82:	usItemLabel = 	CON_DATA_REQ1_F82	;break;	//F82	小时冻结总加无功功率
//	case	83:	usItemLabel = 	CON_DATA_REQ1_F83	;break;	//F83	小时冻结总加有功总电能量
//	case	84:	usItemLabel = 	CON_DATA_REQ1_F84	;break;	//F84	小时冻结总加无功总电能量
	case	89:	usItemLabel = 	CON_DATA_REQ1_F89	;break;	//F89	小时冻结有功功率
	case	90:	usItemLabel = 	CON_DATA_REQ1_F90	;break;	//F90	小时冻结A相有功功率
	case	91:	usItemLabel = 	CON_DATA_REQ1_F91	;break;	//F91	小时冻结B相有功功率
	case	92:	usItemLabel = 	CON_DATA_REQ1_F92	;break;	//F92	小时冻结C相有功功率
	case	93:	usItemLabel = 	CON_DATA_REQ1_F93	;break;	//F93	小时冻结无功功率
	case	94:	usItemLabel = 	CON_DATA_REQ1_F94	;break;	//F94	小时冻结A相无功功率
	case	95:	usItemLabel = 	CON_DATA_REQ1_F95	;break;	//F95	小时冻结B相无功功率
	case	96:	usItemLabel = 	CON_DATA_REQ1_F96	;break;	//F96	小时冻结C相无功功率
	case	97:	usItemLabel = 	CON_DATA_REQ1_F97	;break;	//F97	小时冻结A相电压
	case	98:	usItemLabel = 	CON_DATA_REQ1_F98	;break;	//F98	小时冻结B相电压
	case	99:	usItemLabel = 	CON_DATA_REQ1_F99	;break;	//F99	小时冻结C相电压
	case	100:	usItemLabel = 	CON_DATA_REQ1_F100	;break;	//F100	小时冻结A相电流
	case	101:	usItemLabel = 	CON_DATA_REQ1_F101	;break;	//F101	小时冻结B相电流
	case	102:	usItemLabel = 	CON_DATA_REQ1_F102	;break;	//F102	小时冻结C相电流
	case	103:	usItemLabel = 	CON_DATA_REQ1_F103	;break;	//F103	小时冻结零序电流
	case	105:	usItemLabel = 	CON_DATA_REQ1_F105	;break;	//F105	小时冻结正向有功总电能量
	case	106:	usItemLabel = 	CON_DATA_REQ1_F106	;break;	//F106	小时冻结正向无功总电能量
	case	107:	usItemLabel = 	CON_DATA_REQ1_F107	;break;	//F107	小时冻结反向有功总电能量
	case	108:	usItemLabel = 	CON_DATA_REQ1_F108	;break;	//F108	小时冻结反向无功总电能量
	case	109:	usItemLabel = 	CON_DATA_REQ1_F109	;break;	//F109	小时冻结正向有功总电能示值
	case	110:	usItemLabel = 	CON_DATA_REQ1_F110	;break;	//F110	小时冻结正向无功总电能示值
	case	111:	usItemLabel = 	CON_DATA_REQ1_F111	;break;	//F111	小时冻结反向有功总电能示值
	case	112:	usItemLabel = 	CON_DATA_REQ1_F112	;break;	//F112	小时冻结反向无功总电能示值
	case	113:	usItemLabel = 	CON_DATA_REQ1_F113	;break;	//F113	小时冻结总功率因数
	case	114:	usItemLabel = 	CON_DATA_REQ1_F114	;break;	//F114	小时冻结A相功率因数
	case	115:	usItemLabel = 	CON_DATA_REQ1_F115	;break;	//F115	小时冻结B相功率因数
	case	116:	usItemLabel = 	CON_DATA_REQ1_F116	;break;	//F116	小时冻结C相功率因数
	case	121:	usItemLabel = 	CON_DATA_REQ1_F121	;break;	//F121	小时冻结直流模拟量
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
	case	1:	usItemLabel = 	CON_DATA_REQ2_F1	;break;	//F1	日冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
	case	2:	usItemLabel = 	CON_DATA_REQ2_F2	;break;	//F2	日冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
	case	3:	usItemLabel = 	CON_DATA_REQ2_F3	;break;	//F3	日冻结正向有/无功最大需量及发生时间（总、费率1~M）
	case	4:	usItemLabel = 	CON_DATA_REQ2_F4	;break;	//F4	日冻结反向有/无功最大需量及发生时间（总、费率1~M）
	case	5:	usItemLabel = 	CON_DATA_REQ2_F5	;break;	//F5	日冻结正向有功电能量（总、费率1~M）
	case	6:	usItemLabel = 	CON_DATA_REQ2_F6	;break;	//F6	日冻结正向无功电能量（总、费率1~M）
	case	7:	usItemLabel = 	CON_DATA_REQ2_F7	;break;	//F7	日冻结反向有功电能量（总、费率1~M）
	case	8:	usItemLabel = 	CON_DATA_REQ2_F8	;break;	//F8	日冻结反向无功电能量（总、费率1~M）
	case	9:	usItemLabel = 	CON_DATA_REQ2_F9	;break;	//F9	抄表日冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
	case	10:	usItemLabel = 	CON_DATA_REQ2_F10	;break;	//F10	抄表日冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
	case	11:	usItemLabel = 	CON_DATA_REQ2_F11	;break;	//F11	抄表日冻结正向有/无功最大需量及发生时间（总、费率1~M）
	case	12:	usItemLabel = 	CON_DATA_REQ2_F12	;break;	//F12	抄表日冻结反向有/无功最大需量及发生时间（总、费率1~M）
	case	17:	usItemLabel = 	CON_DATA_REQ2_F17	;break;	//F17	月冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
	case	18:	usItemLabel = 	CON_DATA_REQ2_F18	;break;	//F18	月冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
	case	19:	usItemLabel = 	CON_DATA_REQ2_F19	;break;	//F19	月冻结正向有/无功最大需量及发生时间（总、费率1~M）
	case	20:	usItemLabel = 	CON_DATA_REQ2_F20	;break;	//F20	月冻结反向有/无功最大需量及发生时间（总、费率1~M）
	case	21:	usItemLabel = 	CON_DATA_REQ2_F21	;break;	//F21	月冻结正向有功电能量（总、费率1~M）
	case	22:	usItemLabel = 	CON_DATA_REQ2_F22	;break;	//F22	月冻结正向无功电能量（总、费率1~M）
	case	23:	usItemLabel = 	CON_DATA_REQ2_F23	;break;	//F23	月冻结反向有功电能量（总、费率1~M）
	case	24:	usItemLabel = 	CON_DATA_REQ2_F24	;break;	//F24	月冻结反向无功电能量（总、费率1~M）
	case	25:	usItemLabel = 	CON_DATA_REQ2_F25	;break;	//F25	日分相最大有功功率及发生时间、有功功率为零时间
	case	26:	usItemLabel = 	CON_DATA_REQ2_F26	;break;	//F26	日总及分相最大需量及发生时间
	case	27:	usItemLabel = 	CON_DATA_REQ2_F27	;break;	//F27	日电压统计数据
	case	28:	usItemLabel = 	CON_DATA_REQ2_F28	;break;	//F28	日不平衡度越限累计时间
	case	29:	usItemLabel = 	CON_DATA_REQ2_F29	;break;	//F29	日电流越限统计
	case	30:	usItemLabel = 	CON_DATA_REQ2_F30	;break;	//F30	日视在功率越限累计时间
	case	31:	usItemLabel = 	CON_DATA_REQ2_F31	;break;	//F31	日电能表断相数据
	case	32:	usItemLabel = 	CON_DATA_REQ2_F32	;break;	//F32	备用
	case	33:	usItemLabel = 	CON_DATA_REQ2_F33	;break;	//F33	月分相最大有功功率及发生时间、分相有功功率为零时间
	case	34:	usItemLabel = 	CON_DATA_REQ2_F34	;break;	//F34	月分相有功最大需量及发生时间、总有功最大需量及发生时间
	case	35:	usItemLabel = 	CON_DATA_REQ2_F35	;break;	//F35	月电压统计数据
	case	36:	usItemLabel = 	CON_DATA_REQ2_F36	;break;	//F36	月不平衡度越限累计时间
	case	37:	usItemLabel = 	CON_DATA_REQ2_F37	;break;	//F37	月电流越限统计
	case	38:	usItemLabel = 	CON_DATA_REQ2_F38	;break;	//F38	月视在功率越限累计时间
//	case	41:	usItemLabel = 	CON_DATA_REQ2_F41	;break;	//F41	电容器投入累计时间和次数
//	case	42:	usItemLabel = 	CON_DATA_REQ2_F42	;break;	//F42	日、月电容器累计补偿的无功电能量
	case	43:	usItemLabel = 	CON_DATA_REQ2_F43	;break;	//F43	日功率因数区段累计时间
	case	44:	usItemLabel = 	CON_DATA_REQ2_F44	;break;	//F44	月功率因数区段累计时间
	case	49:	usItemLabel = 	CON_DATA_REQ2_F49	;break;	//F49	终端日供电时间、日复位累计次数
//	case	50:	usItemLabel = 	CON_DATA_REQ2_F50	;break;	//F50	终端日控制统计数据
	case	51:	usItemLabel = 	CON_DATA_REQ2_F51	;break;	//F51	终端月供电时间、月复位累计次数
//	case	52:	usItemLabel = 	CON_DATA_REQ2_F52	;break;	//F52	终端月控制统计数据
	case	53:	usItemLabel = 	CON_DATA_REQ2_F53	;break;	//F53	终端与主站日通信流量
	case	54:	usItemLabel = 	CON_DATA_REQ2_F54	;break;	//F54	终端与主站月通信流量
//	case	57:	usItemLabel = 	CON_DATA_REQ2_F57	;break;	//F57	总加组日最大、最小有功功率及其发生时间，有功功率为零日累计时间
//	case	58:	usItemLabel = 	CON_DATA_REQ2_F58	;break;	//F58	总加组日累计有功电能量（总、费率1~M）
//	case	59:	usItemLabel = 	CON_DATA_REQ2_F59	;break;	//F59	总加组日累计无功电能量（总、费率1~M）
//	case	60:	usItemLabel = 	CON_DATA_REQ2_F60	;break;	//F60	总加组月最大、最小有功功率及其发生时间，有功功率为零月累计时间
//	case	61:	usItemLabel = 	CON_DATA_REQ2_F61	;break;	//F61	总加组月累计有功电能量（总、费率1~M）
//	case	62:	usItemLabel = 	CON_DATA_REQ2_F62	;break;	//F62	总加组月累计无功电能量（总、费率1~M）
//	case	65:	usItemLabel = 	CON_DATA_REQ2_F65	;break;	//F65	总加组超功率定值的月累计时间、月累计电能量
//	case	66:	usItemLabel = 	CON_DATA_REQ2_F66	;break;	//F66	总加组超月电能量定值的月累计时间、累计电能量
//	case	73:	usItemLabel = 	CON_DATA_REQ2_F73	;break;	//F73	总加组有功功率曲线
//	case	74:	usItemLabel = 	CON_DATA_REQ2_F74	;break;	//F74	总加组无功功率曲线
//	case	75:	usItemLabel = 	CON_DATA_REQ2_F75	;break;	//F75	总加组有功电能量曲线
//	case	76:	usItemLabel = 	CON_DATA_REQ2_F76	;break;	//F76	总加组无功电能量曲线
	case	81:	usItemLabel = 	CON_DATA_REQ2_F81	;break;	//F81	有功功率曲线
	case	82:	usItemLabel = 	CON_DATA_REQ2_F82	;break;	//F82	A相有功功率曲线
	case	83:	usItemLabel = 	CON_DATA_REQ2_F83	;break;	//F83	B相有功功率曲线
	case	84:	usItemLabel = 	CON_DATA_REQ2_F84	;break;	//F84	C相有功功率曲线
	case	85:	usItemLabel = 	CON_DATA_REQ2_F85	;break;	//F85	无功功率曲线
	case	86:	usItemLabel = 	CON_DATA_REQ2_F86	;break;	//F86	A相无功功率曲线
	case	87:	usItemLabel = 	CON_DATA_REQ2_F87	;break;	//F87	B相无功功率曲线
	case	88:	usItemLabel = 	CON_DATA_REQ2_F88	;break;	//F88	C相无功功率曲线
	case	89:	usItemLabel = 	CON_DATA_REQ2_F89	;break;	//F89	A相电压曲线
	case	90:	usItemLabel = 	CON_DATA_REQ2_F90	;break;	//F90	B相电压曲线
	case	91:	usItemLabel = 	CON_DATA_REQ2_F91	;break;	//F91	C相电压曲线
	case	92:	usItemLabel = 	CON_DATA_REQ2_F92	;break;	//F92	A相电流曲线
	case	93:	usItemLabel = 	CON_DATA_REQ2_F93	;break;	//F93	B相电流曲线
	case	94:	usItemLabel = 	CON_DATA_REQ2_F94	;break;	//F94	C相电流曲线
	case	95:	usItemLabel = 	CON_DATA_REQ2_F95	;break;	//F95	零序电流曲线
	case	97:	usItemLabel = 	CON_DATA_REQ2_F97	;break;	//F97	正向有功总电能量
	case	98:	usItemLabel = 	CON_DATA_REQ2_F98	;break;	//F98	正向无功总电能量
	case	99:	usItemLabel = 	CON_DATA_REQ2_F99	;break;	//F99	反向有功总电能量
	case	100:	usItemLabel = 	CON_DATA_REQ2_F100	;break;	//F100	反向无功总电能量
	case	101:	usItemLabel = 	CON_DATA_REQ2_F101	;break;	//F101	正向有功总电能示值
	case	102:	usItemLabel = 	CON_DATA_REQ2_F102	;break;	//F102	正向无功总电能示值
	case	103:	usItemLabel = 	CON_DATA_REQ2_F103	;break;	//F103	反向有功总电能示值
	case	104:	usItemLabel = 	CON_DATA_REQ2_F104	;break;	//F104	反向无功总电能示值
	case	105:	usItemLabel = 	CON_DATA_REQ2_F105	;break;	//F105	总功率因数
	case	106:	usItemLabel = 	CON_DATA_REQ2_F106	;break;	//F106	A相功率因数
	case	107:	usItemLabel = 	CON_DATA_REQ2_F107	;break;	//F107	B相功率因数
	case	108:	usItemLabel = 	CON_DATA_REQ2_F108	;break;	//F108	C相功率因数
	case	109:	usItemLabel = 	CON_DATA_REQ2_F109	;break;	
	case	110:	usItemLabel = 	CON_DATA_REQ2_F110	;break;	
//	case	113:	usItemLabel = 	CON_DATA_REQ2_F113	;break;	//F113	A相2~19次谐波电流日最大值及发生时间
//	case	114:	usItemLabel = 	CON_DATA_REQ2_F114	;break;	//F114	B相2~19次谐波电流日最大值及发生时间
//	case	115:	usItemLabel = 	CON_DATA_REQ2_F115	;break;	//F115	C相2~19次谐波电流日最大值及发生时间
//	case	116:	usItemLabel = 	CON_DATA_REQ2_F116	;break;	//F116	A相2~19次谐波电压含有率及总畸变率日最大值及发生时间
//	case	117:	usItemLabel = 	CON_DATA_REQ2_F117	;break;	//F117	B相2~19次谐波电压含有率及总畸变率日最大值及发生时间
//	case	118:	usItemLabel = 	CON_DATA_REQ2_F118	;break;	//F118	C相2~19次谐波电压含有率及总畸变率日最大值及发生时间
//	case	121:	usItemLabel = 	CON_DATA_REQ2_F121	;break;	//F121	A相谐波越限日统计数据
//	case	122:	usItemLabel = 	CON_DATA_REQ2_F122	;break;	//F122	B相谐波越限日统计数据
//	case	123:	usItemLabel = 	CON_DATA_REQ2_F123	;break;	//F123	C相谐波越限日统计数据
//	case	129:	usItemLabel = 	CON_DATA_REQ2_F129	;break;	//F129	直流模拟量越限日累计时间、最大/最小值及发生时间
//	case	130:	usItemLabel = 	CON_DATA_REQ2_F130	;break;	//F130	直流模拟量越限月累计时间、最大/最小值及发生时间
//	case	138:	usItemLabel = 	CON_DATA_REQ2_F138	;break;	//F138	直流模拟量数据曲线
	case	145:	usItemLabel = 	CON_DATA_REQ2_F145	;break;	//F145	一象限无功总电能示值曲线
	case	146:	usItemLabel = 	CON_DATA_REQ2_F146	;break;	//F146	四象限无功总电能示值曲线
	case	147:	usItemLabel = 	CON_DATA_REQ2_F147	;break;	//F147	二象限无功总电能示值曲线
	case	148:	usItemLabel = 	CON_DATA_REQ2_F148	;break;	//F148	三象限无功总电能示值曲线
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
	case	1:		usItemLabel=	CON_DATA_REQ3_F1	;break;	//重要事件						
	case	2:		usItemLabel=	CON_DATA_REQ3_F2	;break;	//一般事件
	}
	return usItemLabel;
}

 unsigned short GetItemLabelbyDT_ExtCmd(unsigned char ucDT)
 {
 	unsigned short usItemLabel=CON_DI_TYPE_INVALID;
	switch(ucDT)
	{
//	case	1:		usItemLabel=	CON_EXTCMD_F1	;break;	//F1 内存读						
//	case	2:		usItemLabel=	CON_EXTCMD_F2	;break;	//F2 内存写
//	case	3:		usItemLabel=	CON_EXTCMD_F3	;break;	//F3 DATAFLASH读
//	case	4:		usItemLabel=	CON_EXTCMD_F4	;break;	//F4 DATAFLASH写
//	case	5:		usItemLabel=	CON_EXTCMD_F5	;break;	//F5 看门狗测试，让程序进入死循环
//	case	6:		usItemLabel=	CON_EXTCMD_F6	;break;	//F6 手机模块通讯检测
//	case	7:		usItemLabel=	CON_EXTCMD_F7	;break;	//F7 终端状态检测
	case	10:		usItemLabel=	CON_EXTCMD_F10	;break;	//F10 终端地址10/16进制切换
//	case	11:		usItemLabel=	CON_EXTCMD_F11	;break;	//F11 中文信息读取
//	case	21:		usItemLabel=	CON_EXTCMD_F21	;break;	//F21 GR任务读					
//	case	22:		usItemLabel=	CON_EXTCMD_F22	;break;	//F22 GR任务配置
//	case	23:		usItemLabel=	CON_EXTCMD_F23	;break;	//F23 GR任务信息读
//	case	24:		usItemLabel=	CON_EXTCMD_F24	;break;	//F24 GR任务信息保存
//	case	31:		usItemLabel=	CON_EXTCMD_F31	;break;	//F31 脉冲需量清零					
//	case	33:		usItemLabel=	CON_EXTCMD_F33	;break;	//F33 脉冲底度设置内容全
//	case	40:		usItemLabel=	CON_EXTCMD_F40	;break;	//F40 内表数据设置
//	case	41:		usItemLabel=	CON_EXTCMD_F41	;break;	//F41 内表数据读取
//	case	51:		usItemLabel=	CON_EXTCMD_F51	;break;	//F51 终端地址					
//	case	52:		usItemLabel=	CON_EXTCMD_F52	;break;	//F52 终端序号读
//	case	53:		usItemLabel=	CON_EXTCMD_F53	;break;	//F53 终端地址行政区号写
//	case	54:		usItemLabel=	CON_EXTCMD_F54	;break;	//F54 终端序号写
//	case	71:		usItemLabel=	CON_EXTCMD_F71	;break;	//F71 蜂鸣器控制
//	case	72:		usItemLabel=	CON_EXTCMD_F72	;break;	//F72 修改时钟
//	case	73:		usItemLabel=	CON_EXTCMD_F73	;break;	//F73 查询通讯波特率
//	case	74:		usItemLabel=	CON_EXTCMD_F74	;break;	//F74 修改通讯波特率
//	case	75:		usItemLabel=	CON_EXTCMD_F75	;break;	//F75 读取液晶轮显显示参数
//	case	76:		usItemLabel=	CON_EXTCMD_F76	;break;	//F76 读取液晶操作显示参数
//	case	77:		usItemLabel=	CON_EXTCMD_F77	;break;	//F77 读取液晶特殊显示参数
//	case	78:		usItemLabel=	CON_EXTCMD_F78	;break;	//F78 修改液晶轮显显示参数
//	case	79:		usItemLabel=	CON_EXTCMD_F79	;break;	//F79 修改液晶操作显示参数
//	case	80:		usItemLabel=	CON_EXTCMD_F80	;break;	//F80 修改液晶特殊显示参数
//	case	81:		usItemLabel=	CON_EXTCMD_F81	;break;	//F81 启用SIM卡的pin
//	case	82:		usItemLabel=	CON_EXTCMD_F82	;break;	//F82 修改SIM卡的pin
//	case	83:		usItemLabel=	CON_EXTCMD_F83	;break;	//F83 用PUK设置SIM卡的pin
//	case	84:		usItemLabel=	CON_EXTCMD_F84	;break;	//F84 SIM卡的pin相关状态
//	case	85:		usItemLabel=	CON_EXTCMD_F85	;break;	//F85 GSM/GPRS切换
//	case	86:		usItemLabel=	CON_EXTCMD_F86	;break;	//F86 读功率控制恢复时间长度
//	case	87:		usItemLabel=	CON_EXTCMD_F87	;break;	//F87 写功率控制恢复时间长度
//	case	89:		usItemLabel=	CON_EXTCMD_F89	;break;	//F89 读功率控制越限确认时间
//	case	90:		usItemLabel=	CON_EXTCMD_F90	;break;	//F90 写功率控制越限确认时间
//	case	91:		usItemLabel=	CON_EXTCMD_F91	;break;	//F91 读自定义调整参数
//	case	92:		usItemLabel=	CON_EXTCMD_F92	;break;	//F92 写自定义调整参数
//	case	93:		usItemLabel=	CON_EXTCMD_F93	;break;	//F93 读取终端电压两位小数电流四位小数
//	case	94:		usItemLabel=	CON_EXTCMD_F94	;break;	//F94 任务初始化
//	case	95:		usItemLabel=	CON_EXTCMD_F95	;break;	//F95 8025时钟矫正
//	case	96:		usItemLabel=	CON_EXTCMD_F96	;break;	//F96 液晶对比度设置
//	case	97:		usItemLabel=	CON_EXTCMD_F97	;break;	//F97 设置表计通迅名和密码
//	case	98:		usItemLabel=	CON_EXTCMD_F98	;break;	//F98 读取表计通迅名和密码
//	case	99:		usItemLabel=	CON_EXTCMD_F99	;break;	//F99 设置GPRS接入的PDP用户名、密码
//	case	100:	usItemLabel=	CON_EXTCMD_F100	;break;	//F100 读取GPRS接入的PDP用户名、密码
//	case	101:	usItemLabel=	CON_EXTCMD_F101	;break;	//F101 设置GPRS重连间隔时间
//	case	102:	usItemLabel=	CON_EXTCMD_F102	;break;	//F102 读取GPRS重连间隔时间
//	case	109:	usItemLabel=	CON_EXTCMD_F109	;break;	//F109 设置终端工作模式
//	case	110:	usItemLabel=	CON_EXTCMD_F110	;break;	//F110 读取终端工作模式
//	case	111:	usItemLabel=	CON_EXTCMD_F111	;break;	//F111 设置CT类型
//	case	112:	usItemLabel=	CON_EXTCMD_F112	;break;	//F112 读取CT类型
//	case	113:	usItemLabel=	CON_EXTCMD_F113	;break;	//F113 设置自学习有效标志
//	case	114:	usItemLabel=	CON_EXTCMD_F114	;break;	//F114 读取自学习有效标志
//	case	115:	usItemLabel=	CON_EXTCMD_F115	;break;	
//	case	116:	usItemLabel=	CON_EXTCMD_F116	;break;	
//	case	117:	usItemLabel=	CON_EXTCMD_F117	;break;	
//	case	118:	usItemLabel=	CON_EXTCMD_F118	;break;	
//	case	119:	usItemLabel=	CON_EXTCMD_F119	;break;	
//	case	120:	usItemLabel=	CON_EXTCMD_F120	;break;	//F120 设置AES加密密码
//	case	121:	usItemLabel=	CON_EXTCMD_F121	;break;	//F121 读取AES加密密码
//	case	122: 	usItemLabel= 	CON_EXTCMD_F122	;break; //F122 投入接解除断线保电控制
//	case	123: 	usItemLabel= 	CON_EXTCMD_F123	;break; //F123 读取温度
//	case	124:	usItemLabel=	CON_EXTCMD_F124 ;break; //F124 读取校表EEPROM的参数
//	case	125:	usItemLabel=	CON_EXTCMD_F125 ;break; //F125 设置校表EEPROM的参数
//	case 	126:	usItemLabel= 	CON_EXTCMD_F126; break;
	}
 	return usItemLabel;
 }

unsigned short GetItemLabelbyDT_DataTrans(unsigned char ucDT)
{
	unsigned short usItemLabel=CON_DI_TYPE_INVALID;
	switch(ucDT)
	{
	case	1:		usItemLabel=	CON_DATATRANS_F1	;break;	//F1 数据转发
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
	case	1:		usItemLabel=	CON_FILETRANS_F1	;break;	//F1 文件传输方式1
	case	100:	usItemLabel=	CON_FILETRANS_F100	;break;	//F100 程序下装命令
	}
	return usItemLabel;
}

unsigned char Check_DataType_1(unsigned char *pucBuffer)
{
	if(Check_BCD_Data(pucBuffer[0])==RET_ERROR)				//DT1.byte1	BCD码
		return RET_ERROR;
	if(Check_BCD_Data((unsigned char)(pucBuffer[1]&0x0F))==RET_ERROR)		//DT1.byte2	BCD码
		return RET_ERROR;
	return RET_SUCCESS;
}

unsigned char Check_DataType_4(unsigned char *pucBuffer)
{
	if(Check_BCD_Data(pucBuffer[0])==RET_ERROR)				//DT4.byte1	BCD码
		return RET_ERROR;
	return RET_SUCCESS;
}

unsigned char Check_DataType_5(unsigned char *pucBuffer)
{
	if(Check_BCD_Data(pucBuffer[0])==RET_ERROR)				//DT5.byte1	BCD码
		return RET_ERROR;
	if(Check_BCD_Data(pucBuffer[1])==RET_ERROR)				//DT5.byte2	BCD码
		return RET_ERROR;
	if(Check_BCD_Data(pucBuffer[2])==RET_ERROR)				//DT5.byte3	BCD码
		return RET_ERROR;
	if(Check_BCD_Data((unsigned char)(pucBuffer[3]&0x0F))==RET_ERROR)		//DT5.byte4	BCD码
		return RET_ERROR;
	return RET_SUCCESS;
}

unsigned char Check_DataType_11(unsigned char *pucBuffer)
{
	if(Check_BCD_Data(pucBuffer[0])==RET_ERROR)				//DT11.byte1	BCD码
		return RET_ERROR;
	if(Check_BCD_Data(pucBuffer[1])==RET_ERROR)				//DT11.byte2	BCD码
		return RET_ERROR;
	return RET_SUCCESS;
}

//F1	终端版本信息
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

//F2	终端日历时钟
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

//F3	终端参数状态
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

//F4	终端通信状态
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

//F5	终端控制设置状态
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
//	//保电、剔除和催费告警投入状态
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
//	//总加组有效标志位
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
//	//功控定值方案号及功控时段有效标志
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
//	//功控状态
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
//	//电控状态
//	ucTmp=0;
//	stEnergyCtrl=RunPara_GetEnergyCtrl(ucSumGroupNo);
//	if(stEnergyCtrl.ucEnergyCtrlType==CON_ENERGYCTRL_MON)
//		ucTmp |= 0x01;
//	if(stEnergyCtrl.ucEnergyCtrlType==CON_ENERGYCTRL_BUY)
//		ucTmp |= 0x02;
//	pucRetBuffer[m++]=ucTmp;
//	//功控轮次状态
//	ucTmp=0;
//	stCtrlTurn=RunPara_GetPowerCtrlTurn(ucSumGroupNo);
//	for(i=0;i<CON_PARA_SWITCHOUT_NUM;i++)
//	{
//		if(stCtrlTurn.ucAvailbleFlag[i]==CON_AVALIBLE_FLAG)
//			ucTmp |= (1<<i);
//	}
//	pucRetBuffer[m++]=ucTmp;
//	//电控轮次状态
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

//F6	终端当前控制状态
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
//			ucVal |= (1<<i);								//遥控跳闸输出状态
//	}
//	pucRetBuffer[m++]=ucVal;
//
//	stLoadCtrlWordPara=RunPara_GetLoadCtrlWordPara();
//	if(stLoadCtrlWordPara.ucUrgeFeeCtrl==CON_AVALIBLE_FLAG)	//当前催费告警状态
//		pucRetBuffer[m++] = 0x01;				
//	else
//		pucRetBuffer[m++] = 0x00;
//
//	ucVal = (unsigned char)(1<<ucPowerCtrlSumGNo);			//总加组有效标志	
//	ucVal |= (unsigned char)(1<<ucEnergyCtrlSumGNo);
//	pucRetBuffer[m++]=ucVal;
//	if(ucPowerCtrlSumGNo!=ucEnergyCtrlSumGNo)
//	{
//		if(ucPowerCtrlSumGNo<ucEnergyCtrlSumGNo)
//		{
//			Main_Get_ReqData_1_F6_SumGroupPer(pucRetBuffer,&m,ucPowerCtrlSumGNo,0xAA);			//AA功控
//			Main_Get_ReqData_1_F6_SumGroupPer(pucRetBuffer,&m,ucEnergyCtrlSumGNo,0x55);			//55电控
//		}else
//		{
//			Main_Get_ReqData_1_F6_SumGroupPer(pucRetBuffer,&m,ucEnergyCtrlSumGNo,0x55);			//55电控
//			Main_Get_ReqData_1_F6_SumGroupPer(pucRetBuffer,&m,ucPowerCtrlSumGNo,0xAA);			//AA功控
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
//	stSafeFixVal=RunPara_GetCtrlSafeFixVal();				//当前功控定值
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
//	//下浮控浮动系数
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
//	//功控跳闸输出状态
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
//	//月电控跳闸输出状态
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
//	//功控跳闸告警状态
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
//	//电控跳闸告警状态
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

//F7	终端事件计数器当前值
unsigned char Main_Get_ReqData_1_F7(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	pucRetBuffer[m++]=g_ucEventCount_Important;
	pucRetBuffer[m++]=g_ucEventCount_Normal;
	(*pusPos)=m;
	return ucRet;
}

//F8	终端事件标志状态
unsigned char Main_Get_ReqData_1_F8(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
//	unsigned char i=0;
	unsigned short m=(*pusPos);
//	STALRCATEGORY	stAlrHappenWord;
//	stAlrHappenWord = Alarm_Get_Happen_word();//azh 已经是全局变量

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

//F9	终端状态量及变位标志
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

//F10	终端与主站当日、月通信流量
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
//	pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
    pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存
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
//F12	控制输出回路开关接入状态量及变位标志
unsigned char Main_Get_ReqData_1_F12(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);

	memset(pucRetBuffer+m,0,2);//azh 空
	m+=2;
	ucRet=RET_SUCCESS;
	(*pusPos)=m;
	return ucRet;
}
//F13	搜索到的电表信息
unsigned char Main_Get_ReqData_1_F13(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);

	memset(pucRetBuffer+m,0,4);//azh 空
	m+=4;
	ucRet=RET_SUCCESS;
	(*pusPos)=m;
	return ucRet;
}
//F14	文件传输未收到数据段
unsigned char Main_Get_ReqData_1_F14(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);

	memset(pucRetBuffer+m,0,130);//azh 空
	m+=130;
	ucRet=RET_SUCCESS;
	(*pusPos)=m;
	return ucRet;
}
////F11	当前信号强度
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
////F16	终端载波/无线抄表失败标志表
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
		pucRetBuffer[m++] = 0;	//置"0"：表示正常或该测量点是采用非载波通信方式抄表的电表。
	}
	(*pusPos)=m;
	*/
//	return ucRet;
//}
//
////F17	当前总加有功功率
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
////F18	当前总加无功功率
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
////F19	当日总加有功电能量（总、费率1~M）
//unsigned char Main_Get_ReqData_1_F19(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{//当前电量与昨天电量之差
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
//	pstHisSumGRunData=(STHISSUMGROUPRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//	pucRetBuffer[m++]=4;								//费率数
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
////F20	当日总加无功电能量（总、费率1~M）
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
//	pstHisSumGRunData=(STHISSUMGROUPRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//	pucRetBuffer[m++]=4;								//费率数
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
////F21	当月总加有功电能量（总、费率1~M）
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
//	pucRetBuffer[m++]=4;								//费率数
//	memset(pucRetBuffer+m,0xEE,20);
//	fFeeT=CONINVALIDVAL;	fFee1=CONINVALIDVAL;	fFee2=CONINVALIDVAL;	
//	fFee3=CONINVALIDVAL;	fFee4=CONINVALIDVAL;
//	stTempTime=GetCurrentTime();
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
//	stTempTime.ucDay=28;
//	pstHisSumGRunData=(STHISSUMGROUPRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
////F22	当月总加无功电能量（总、费率1~M）
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
//	pucRetBuffer[m++]=4;								//费率数
//	memset(pucRetBuffer+m,0xEE,20);
//	fFeeT=CONINVALIDVAL;	fFee1=CONINVALIDVAL;	fFee2=CONINVALIDVAL;	
//	fFee3=CONINVALIDVAL;	fFee4=CONINVALIDVAL;
//	stTempTime=GetCurrentTime();
//	stTempTime.ucSecond=59;
//	stTempTime.ucMinute=59;
//	stTempTime.ucHour=23;
//	stTempTime.ucDay=28;
//	pstHisSumGRunData=(STHISSUMGROUPRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
////F23	终端当前剩余电量（费）
////华北，仅仅针对购电控
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
//				ulVal=9999999;				//限制最大9999999
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
////F24	当前功率下浮控控后总加有功功率冻结值
//unsigned char Main_Get_ReqData_1_F24(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{//需要按照总加组号
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

//F25	当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流,视在功率
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
			//azh 140610这里读出的功率小数位没对齐要注意
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
				stHisVal.lVal = GetPQRate(stHisVal.lVal,stHisVal1.lVal);//azh 140610此函数里有功是除掉100的 注意	
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
	//零序电流计算，假设正常相序间隔120度
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

//F26	A、B、C三相断相统计数据及最近一次断相记录
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

//F27	电能表日历时钟及电能表状态信息
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
//F162	电能表日历时钟
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

//F28	电能表日历时钟及电能表状态信息
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
			pucRetBuffer[m++] = ucMeterStateChangeFlag[ucmeterno][i];		//状态变位目前都写为0，后面进行更改	cfh_090821
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
//F33	当前正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
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

//F34	当前反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
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

//F35	当月正向有/无功最大需量及发生时间（总、费率1~M）
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

//F36	当月反向有/无功最大需量及发生时间（总、费率1~M）
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

//F37	上月正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
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

//F38	上月反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
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

//F39	上月正向有/无功最大需量及发生时间（总、费率1~M）
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
		if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)			//上月
			ClearDateTime(&(stHisTime.stHisTime));
		m=SetMainComm_Buf_DateTime_MDHM( pucRetBuffer ,m, stHisTime.stHisTime);
	}
/*	stHisTime = Get_History_DirPowMaxDemandTimeLm_byMPNo(ucMPNo,0,0);
	if(stHisTime.stHisTime.ucMonth==stCurr.ucMonth)			//上月
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

//F40	上月反向有/无功最大需量及发生时间（总、费率1~M）
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

//F41	当日正向有功电能量（总、费率1~M）
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
			pucRetBuffer[m++]=4;		//费率
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
		}else
		{
			pucRetBuffer[m++]=4;					//固定四费率
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
//			pucRetBuffer[m++]=4;		//费率
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}else
//		{
//			pucRetBuffer[m++]=4;					//固定四费率
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
//			pucRetBuffer[m++]=4;		//费率
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}else
//		{
//			pucRetBuffer[m++]=4;					//固定四费率
//			memset(pucRetBuffer+m,0xEE,20);
//			m+=20;
//		}
//		free_mem_block(mem_ptr);//
//	}

	(*pusPos)=m;
	return ucRet;
}

//F42	当日正向无功电能量（总、费率1~M）
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
			pucRetBuffer[m++]=4;		//费率
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
		}else
		{
			pucRetBuffer[m++]=4;					//固定四费率
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
//			pucRetBuffer[m++]=4;		//费率
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}else
//		{
//			pucRetBuffer[m++]=4;					//固定四费率
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
//			pucRetBuffer[m++]=4;		//费率
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}else
//		{
//			pucRetBuffer[m++]=4;					//固定四费率
//			memset(pucRetBuffer+m,0xEE,20);
//			m+=20;
//		}
//		free_mem_block(mem_ptr);//
//	}
	
	(*pusPos)=m;
	return ucRet;
}

//F43	当日反向有功电能量（总、费率1~M）
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
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存
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
			pucRetBuffer[m++]=4;		//费率
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
		}else
		{
			pucRetBuffer[m++]=4;					//固定四费率
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
//			pucRetBuffer[m++]=4;		//费率
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}else
//		{
//			pucRetBuffer[m++]=4;					//固定四费率
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
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存
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
//			pucRetBuffer[m++]=4;		//费率
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}else
//		{
//			pucRetBuffer[m++]=4;					//固定四费率
//			memset(pucRetBuffer+m,0xEE,20);
//			m+=20;
//		}
//		free_mem_block(mem_ptr);//
//	}
	
	(*pusPos)=m;
	return ucRet;
}

//F44	当日反向无功电能量（总、费率1~M）
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
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存
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
			pucRetBuffer[m++]=4;		//费率
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
		}else
		{
			pucRetBuffer[m++]=4;					//固定四费率
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
//			pucRetBuffer[m++]=4;		//费率
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}else
//		{
//			pucRetBuffer[m++]=4;					//固定四费率
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
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存
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
//			pucRetBuffer[m++]=4;		//费率
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFeeT);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee1);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee2);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee3);
//			m=SetMainComm_Buf_lVal_4B_DT13(pucRetBuffer,m,ulFee4);
//		}else
//		{
//			pucRetBuffer[m++]=4;					//固定四费率
//			memset(pucRetBuffer+m,0xEE,20);
//			m+=20;
//		}
//		free_mem_block(mem_ptr);//
//	}
	
	(*pusPos)=m;
	return ucRet;
}

//F45	当月正向有功电能量（总、费率1~M）
unsigned char Main_Get_ReqData_1_F45(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	unsigned short usLen=0;
	unsigned char ucBuff[24];
	STDATETIME stTempTime;
	memset(ucBuff,0x00,24);
	stTempTime=GetCurrentTime();
	ucRet=Main_Get_ReqData_2_F21(ucDAp,ucBuff,&usLen,&stTempTime);			//F21   月冻结正向有功电能量（总、费率1~M）
	usLen -= 2;								//减去时间数据标识的长度
	memcpy(pucRetBuffer+m,ucBuff+2,usLen);
	(*pusPos)=m+usLen;				
	return ucRet;
}

//F46	当月正向无功电能量（总、费率1~M）
unsigned char Main_Get_ReqData_1_F46(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	unsigned short usLen=0;
	unsigned char ucBuff[24];
	STDATETIME stTempTime;
	memset(ucBuff,0x00,24);
	stTempTime=GetCurrentTime();
	ucRet=Main_Get_ReqData_2_F22(ucDAp,ucBuff,&usLen,&stTempTime);			//F22	月冻结正向无功电能量（总、费率1~M）
	usLen -= 2;								//减去时间数据标识的长度
	memcpy(pucRetBuffer+m,ucBuff+2,usLen);
	(*pusPos)=m+usLen;				
	return ucRet;
}

//F47	当月反向有功电能量（总、费率1~M）
unsigned char Main_Get_ReqData_1_F47(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	unsigned short usLen=0;
	unsigned char ucBuff[24];
	STDATETIME stTempTime;
	memset(ucBuff,0x00,24);
	stTempTime=GetCurrentTime();
	ucRet=Main_Get_ReqData_2_F23(ucDAp,ucBuff,&usLen,&stTempTime);			//F23	月冻结反向有功电能量（总、费率1~M）
	usLen -= 2;								//减去时间数据标识的长度
	memcpy(pucRetBuffer+m,ucBuff+2,usLen);
	(*pusPos)=m+usLen;				
	return ucRet;
}

//F48	当月反向无功电能量（总、费率1~M）
unsigned char Main_Get_ReqData_1_F48(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	unsigned short usLen=0;
	unsigned char ucBuff[24];
	STDATETIME stTempTime;
	memset(ucBuff,0x00,24);
	stTempTime=GetCurrentTime();
	ucRet=Main_Get_ReqData_2_F24(ucDAp,ucBuff,&usLen,&stTempTime);			//F24	月冻结反向无功电能量（总、费率1~M）	
	usLen -= 2;								//减去时间数据标识的长度
	memcpy(pucRetBuffer+m,ucBuff+2,usLen);
	(*pusPos)=m+usLen;				
	return ucRet;
}

//F49	当前电压、电流相位角
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

////F57	当前A、B、C三相电压、电流2~N次谐波有效值
//unsigned char Main_Get_ReqData_1_F57(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	ucRet=RET_ERROR;
//	return ucRet;
//}
//
////F58	当前A、B、C三相电压、电流2~N次谐波含有率
//unsigned char Main_Get_ReqData_1_F58(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	ucRet=RET_ERROR;
//	return ucRet;
//}
//
////F65	当前电容器投切状态
//unsigned char Main_Get_ReqData_1_F65(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	ucRet=RET_ERROR;
//	return ucRet;
//}
//
////F66	当前电容器累计补偿投入时间和次数
//unsigned char Main_Get_ReqData_1_F66(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	ucRet=RET_ERROR;
//	return ucRet;
//}
//
////F67	当日、当月电容器累计补偿的无功电能量
//unsigned char Main_Get_ReqData_1_F67(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	ucRet=RET_ERROR;
//	return ucRet;
//}
//
////F73	直流模拟量实时数据
//unsigned char Main_Get_ReqData_1_F73(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	ucRet=RET_ERROR;
//	return ucRet;
//}
//
////F81	小时冻结总加有功功率
//unsigned char Main_Get_ReqData_1_F81(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned short m=(*pusPos);
//	unsigned char i;
//	unsigned char ucPointNum;			//点数
//	unsigned char ucM=0;				//密度
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
//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;			//工作数据区
//
//	ucM=GetFreeseMByGroupNo(ucGroupNo);
//	if((ucM>4) || (ucM==0))
//		return RET_ERROR;
//	ucM=1<<(ucM-1);							//密度代号转换为密度数值
//	ucPointNum=4/ucM;
//	stStartTime=GetLastOneHourStartTime((unsigned char)(15*ucM));
//	if(gucJob1HoursFlag>0 && gucJob1HoursFlag<8)
//		SubTime(&stStartTime,gucJob1HoursFlag,CON_TIMEUNIT_HOUR);
//	
//	pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//第一字节冻结密度BCD码	数据时间（小时）BCD码十位	数据时间（小时）BCD码个位
//	pucRetBuffer[m+1]=ucM;
//	memset(pucRetBuffer+m+2,0xEE,2*ucPointNum);		//数据格式02为2字节
//	(*pusPos)=m+2+2*ucPointNum;
//	Task_QueryRetVal(ucTaskNo,pucBuffer,&stStartTime,(unsigned short)ucPointNum,ucM,&usOnePointLen);
//	if(((pucBuffer[0]+0x100*pucBuffer[1])!=0) && (usOnePointLen!=0))
//	{
//		pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//第一字节冻结密度BCD码	数据时间（小时）BCD码十位	数据时间（小时）BCD码个位
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
//				fVal/=10000;					//有功0.0001kW --》 kW
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
////F82	小时冻结总加无功功率
//unsigned char Main_Get_ReqData_1_F82(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned short m=(*pusPos);
//	unsigned char i;
//	unsigned char ucPointNum;			//点数
//	unsigned char ucM=0;				//密度
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
//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;			//工作数据区
//	ucM=GetFreeseMByGroupNo(ucGroupNo);
//	if((ucM>4) || (ucM==0))
//		return RET_ERROR;
//	ucM=1<<(ucM-1);							//密度代号转换为密度数值
//	ucPointNum=4/ucM;
//	stStartTime=GetLastOneHourStartTime((unsigned char)(15*ucM));
//	if(gucJob1HoursFlag>0 && gucJob1HoursFlag<8)
//		SubTime(&stStartTime,gucJob1HoursFlag,CON_TIMEUNIT_HOUR);
//	
//	pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//第一字节冻结密度BCD码	数据时间（小时）BCD码十位	数据时间（小时）BCD码个位
//	pucRetBuffer[m+1]=ucM;
//	memset(pucRetBuffer+m+2,0xEE,2*ucPointNum);		//数据格式02为2字节
//	(*pusPos)=m+2+2*ucPointNum;
//	Task_QueryRetVal(ucTaskNo,pucBuffer,&stStartTime,(unsigned short)ucPointNum,ucM,&usOnePointLen);
//	if(((pucBuffer[0]+0x100*pucBuffer[1])!=0) && (usOnePointLen!=0))
//	{
//		pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//第一字节冻结密度BCD码	数据时间（小时）BCD码十位	数据时间（小时）BCD码个位
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
//				fVal/=100;						//无功0.01kvar --》 kvar
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
////F83	小时冻结总加有功总电能量
//unsigned char Main_Get_ReqData_1_F83(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned short m=(*pusPos);
//	unsigned char i;
//	unsigned char ucPointNum,ucPointNumTmp;			//点数
//	unsigned char ucM=0;				//密度
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
//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;			//工作数据区
//	ucM=GetFreeseMByGroupNo(ucGroupNo);
//	if((ucM>4) || (ucM==0))
//		return RET_ERROR;
//	ucM=1<<(ucM-1);							//密度代号转换为密度数值
//	ucPointNum=4/ucM;						
//	ucPointNumTmp=ucPointNum+1;				//加一个点
//	stStartTime=GetLastOneHourStartTime_Diff(0);
//	if(gucJob1HoursFlag>0 && gucJob1HoursFlag<8)
//		SubTime(&stStartTime,gucJob1HoursFlag,CON_TIMEUNIT_HOUR);
//	pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//第一字节冻结密度BCD码	数据时间（小时）BCD码十位	数据时间（小时）BCD码个位
//	pucRetBuffer[m+1]=ucM;
//	memset(pucRetBuffer+m+2,0xEE,4*ucPointNum);		//数据格式03为4字节
//	(*pusPos)=m+4*ucPointNum+2;
//	Task_QueryRetVal(ucTaskNo,pucBuffer,&stStartTime,(unsigned short)ucPointNumTmp,ucM,&usOnePointLen);
//	if(((pucBuffer[0]+0x100*pucBuffer[1])!=0) && (usOnePointLen!=0))
//	{
//		pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//第一字节冻结密度BCD码	数据时间（小时）BCD码十位	数据时间（小时）BCD码个位
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
////F84	小时冻结总加无功总电能量
//unsigned char Main_Get_ReqData_1_F84(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	unsigned short m=(*pusPos);
//	unsigned char i;
//	unsigned char ucPointNum,ucPointNumTmp;			//点数
//	unsigned char ucM=0;				//密度
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
//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;			//工作数据区
//	ucM=GetFreeseMByGroupNo(ucGroupNo);
//	if((ucM>4) || (ucM==0))
//		return RET_ERROR;
//	ucM=1<<(ucM-1);							//密度代号转换为密度数值
//	ucPointNum=4/ucM;						
//	ucPointNumTmp=ucPointNum+1;				//加一个点
//	stStartTime=GetLastOneHourStartTime_Diff(0);
//	if(gucJob1HoursFlag>0 && gucJob1HoursFlag<8)
//		SubTime(&stStartTime,gucJob1HoursFlag,CON_TIMEUNIT_HOUR);
//	pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//第一字节冻结密度BCD码	数据时间（小时）BCD码十位	数据时间（小时）BCD码个位
//	pucRetBuffer[m+1]=ucM;
//	memset(pucRetBuffer+m+2,0xEE,4*ucPointNum);		//数据格式03为4字节
//	(*pusPos)=m+4*ucPointNum+2;
//	Task_QueryRetVal(ucTaskNo,pucBuffer,&stStartTime,(unsigned short)ucPointNumTmp,ucM,&usOnePointLen);
//	if(((pucBuffer[0]+0x100*pucBuffer[1])!=0) && (usOnePointLen!=0))
//	{
//		pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//第一字节冻结密度BCD码	数据时间（小时）BCD码十位	数据时间（小时）BCD码个位
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

//F89	小时冻结有功功率
unsigned char Main_Get_ReqData_1_F89(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_InstantPow(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPINSTANTPOW);
	return ucRet;
}

//F90	小时冻结A相有功功率
unsigned char Main_Get_ReqData_1_F90(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_InstantPow(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPINSTANTPOWA);
	return ucRet;
}

//F91	小时冻结B相有功功率
unsigned char Main_Get_ReqData_1_F91(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_InstantPow(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPINSTANTPOWB);
	return ucRet;
}

//F92	小时冻结C相有功功率
unsigned char Main_Get_ReqData_1_F92(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_InstantPow(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPINSTANTPOWC);
	return ucRet;
}

//F93	小时冻结无功功率
unsigned char Main_Get_ReqData_1_F93(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_InstantPow(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPINSTANTVAR);
	return ucRet;
}

//F94	小时冻结A相无功功率
unsigned char Main_Get_ReqData_1_F94(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_InstantPow(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPINSTANTVARA);
	return ucRet;
}

//F95	小时冻结B相无功功率
unsigned char Main_Get_ReqData_1_F95(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_InstantPow(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPINSTANTVARB);
	return ucRet;
}

//F96	小时冻结C相无功功率
unsigned char Main_Get_ReqData_1_F96(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_InstantPow(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPINSTANTVARC);
	return ucRet;
}

//F97	小时冻结A相电压
unsigned char Main_Get_ReqData_1_F97(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_Volt(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPVOLA);
	return ucRet;
}

//F98	小时冻结B相电压
unsigned char Main_Get_ReqData_1_F98(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_Volt(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPVOLB);
	return ucRet;
}

//F99	小时冻结C相电压
unsigned char Main_Get_ReqData_1_F99(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_Volt(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPVOLC);
	return ucRet;
}

//F100	小时冻结A相电流
unsigned char Main_Get_ReqData_1_F100(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_Curr(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPCURA);
	return ucRet;
}

//F101	小时冻结B相电流
unsigned char Main_Get_ReqData_1_F101(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_Curr(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPCURB);
	return ucRet;
}

//F102	小时冻结C相电流
unsigned char Main_Get_ReqData_1_F102(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_Curr(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPCURC);
	return ucRet;
}

//F103	小时冻结零序电流
unsigned char Main_Get_ReqData_1_F103(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=RET_ERROR;
	return ucRet;
}

//F105	小时冻结正向有功总电能量
unsigned char Main_Get_ReqData_1_F105(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_PowVal(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPDIRPOWTOTAL);
	return ucRet;
}

//F106	小时冻结正向无功总电能量
unsigned char Main_Get_ReqData_1_F106(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_PowVal(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPDIRVARTOTAL);
	return ucRet;
}

//F107	小时冻结反向有功总电能量
unsigned char Main_Get_ReqData_1_F107(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_PowVal(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPUNDIRPOWTOTAL);
	return ucRet;
}

//F108	小时冻结反向无功总电能量
unsigned char Main_Get_ReqData_1_F108(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_PowVal(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPUNDIRVARTOTAL);
	return ucRet;
}

//F109	小时冻结正向有功总电能示值
unsigned char Main_Get_ReqData_1_F109(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_PowViewData(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPDIRPOWTOTAL);
	return ucRet;
}

//F110	小时冻结正向无功总电能示值
unsigned char Main_Get_ReqData_1_F110(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_PowViewData(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPDIRVARTOTAL);
	return ucRet;
}

//F111	小时冻结反向有功总电能示值
unsigned char Main_Get_ReqData_1_F111(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_PowViewData(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPUNDIRPOWTOTAL);
	return ucRet;
}

//F112	小时冻结反向无功总电能示值
unsigned char Main_Get_ReqData_1_F112(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_PowViewData(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPUNDIRVARTOTAL);
	return ucRet;
}

//F113	小时冻结总功率因数
unsigned char Main_Get_ReqData_1_F113(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_PowRate(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPPOWRATE);
	return ucRet;
}

//F114	小时冻结A相功率因数
unsigned char Main_Get_ReqData_1_F114(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_PowRate(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPPOWRATEA);
	return ucRet;
}

//F115	小时冻结B相功率因数
unsigned char Main_Get_ReqData_1_F115(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_PowRate(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPPOWRATEB);
	return ucRet;
}

//F116	小时冻结C相功率因数
unsigned char Main_Get_ReqData_1_F116(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_1_PowRate(ucDAp,pucRetBuffer,pusPos,CON_DI_TYPE_MPPOWRATEC);
	return ucRet;
}

//F121	小时冻结直流模拟量
//unsigned char Main_Get_ReqData_1_F121(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	ucRet=RET_ERROR;
//	return ucRet;
//}


//F129	当前正向有功电能示值、（总、费率1~M）
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

//F130	当前正向无功电能示值（总、费率1~M）
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


//F131	当前反向有功电能示值（总、费率1~M）
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


//F132	当前反向无功电能示值（总、费率1~M）
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


//F133	当前一象限无功电能示值（总、费率1~M）
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

//F134	当前二象限无功电能示值（总、费率1~M）
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

//F135	当前三象限无功电能示值（总、费率1~M）
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

//F136	当前四象限无功电能示值（总、费率1~M）
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


//F137	上月正向有功电能示值（总、费率1~M）
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

//F138	上月正向无功电能示值（总、费率1~M）
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


//F139	上月反向有功电能示值（总、费率1~M）
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


//F140	上月反向无功电能示值（总、费率1~M）
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


//F141	上月一象限无功电能示值（总、费率1~M）
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


//F142	上月二象限无功电能示值（总、费率1~M）
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

//F143	上月三象限无功电能示值（总、费率1~M）
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

//F144	上月四象限无功电能示值（总、费率1~M）
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


//F145	当月正向有功最大需量及发生时间（总、费率1~M）
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


//F146	当月正向无功最大需量及发生时间（总、费率1~M）
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


//F147	当月反向有功最大需量及发生时间（总、费率1~M）
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


//F148	当月反向无功最大需量及发生时间（总、费率1~M）
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


//F149	上月正向有功最大需量及发生时间（总、费率1~M）
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


//F150	上月正向无功最大需量及发生时间（总、费率1~M）
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


//F151	上月反向有功最大需量及发生时间（总、费率1~M）
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


//F152	上月反向无功最大需量及发生时间（总、费率1~M）
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

//读取上一整点的功率
unsigned char Main_Get_ReqData_1_InstantPow(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	unsigned char i;
	unsigned char ucPointNum;			//点数
	unsigned char ucM=0;				//密度
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
//			ucTaskNo=CON_GRTASK_NUM;			//无效
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//		ucTaskNo=Task_GetTaskNo_Ter();
	if(ucTaskNo>=CON_GRTASK_NUM)
		return RET_ERROR;
		
//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;			//工作数据区
	ucM=GetFreeseMByMPNo(ucMPNo);
	if((ucM>4) || (ucM==0))
		return RET_ERROR;
	ucM=1<<(ucM-1);							//密度代号转换为密度数值
	ucPointNum=4/ucM;
	stStartTime=GetLastOneHourStartTime((unsigned char)(15*ucM));
	if(gucJob1HoursFlag>0 && gucJob1HoursFlag<8)
		SubTime(&stStartTime,gucJob1HoursFlag,CON_TIMEUNIT_HOUR);
//	pucRetBuffer[m]=(ucM<<6)+(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//第一字节冻结密度BCD码	数据时间（小时）BCD码十位	数据时间（小时）BCD码个位
	pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//第一字节冻结密度BCD码	数据时间（小时）BCD码十位	数据时间（小时）BCD码个位
	pucRetBuffer[m+1]=ucM;
	memset(pucRetBuffer+m+2,0xEE,3*ucPointNum);
	(*pusPos)=m+3*ucPointNum+2;

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }	
	pucBuffer=(unsigned char *)mem_ptr;			//工作数据区
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
						ucDesBuf[1]=0x79;			//数据格式9，目前不处理负数据，最大值79.9999
					memcpy(pucRetBuffer+m+1,ucDesBuf,2);		//无功2字节，0 补齐低字节
				}
			}else
			{
				if(ucDesBuf[0]==0xEE)
				{
					memset(pucRetBuffer+m,0xEE,3);
				}else
				{
					if(ucDesBuf[2]>0x79)
						ucDesBuf[2]=0x79;			//数据格式9，目前不处理负数据，最大值79.9999
					memcpy(pucRetBuffer+m,ucDesBuf,3);			//有功3字节
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

//读取上一整点的电压
unsigned char Main_Get_ReqData_1_Volt(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	unsigned char i;
	unsigned char ucPointNum;			//点数
	unsigned char ucM=0;				//密度
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
	
//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;			//工作数据区
	ucM=GetFreeseMByMPNo(ucMPNo);
	if((ucM>4) || (ucM==0))
		return RET_ERROR;
	ucM=1<<(ucM-1);							//密度代号转换为密度数值
	ucPointNum=4/ucM;
	stStartTime=GetLastOneHourStartTime((unsigned char)(15*ucM));
	if(gucJob1HoursFlag>0 && gucJob1HoursFlag<8)
		SubTime(&stStartTime,gucJob1HoursFlag,CON_TIMEUNIT_HOUR);
	pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//第一字节冻结密度BCD码	数据时间（小时）BCD码十位	数据时间（小时）BCD码个位
	pucRetBuffer[m+1]=ucM;
	memset(pucRetBuffer+m+2,0xEE,2*ucPointNum);
	(*pusPos)=m+2*ucPointNum+2;

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucBuffer=(unsigned char *)mem_ptr;			//工作数据区
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
		//		ulVal=(ulVal<<4);				//数据格式08转换为数据格式07
			memcpy(pucRetBuffer+m,(unsigned char *)&ulVal,2);
			m+=2;
			pucBuffer+=usOnePointLen;
		}
		(*pusPos)=m;
	}
	free_mem_block(mem_ptr);//
	return ucRet;
}

//读取上一整点的电流
unsigned char Main_Get_ReqData_1_Curr(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	unsigned char i;
	unsigned char ucPointNum;			//点数
	unsigned char ucM=0;				//密度
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
	
//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;			//工作数据区
	ucM=GetFreeseMByMPNo(ucMPNo);
	if((ucM>4) || (ucM==0))
		return RET_ERROR;
	ucM=1<<(ucM-1);							//密度代号转换为密度数值
	ucPointNum=4/ucM;
	stStartTime=GetLastOneHourStartTime((unsigned char)(15*ucM));
	if(gucJob1HoursFlag>0 && gucJob1HoursFlag<8)
		SubTime(&stStartTime,gucJob1HoursFlag,CON_TIMEUNIT_HOUR);
	pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//第一字节冻结密度BCD码	数据时间（小时）BCD码十位	数据时间（小时）BCD码个位
	pucRetBuffer[m+1]=ucM;
	memset(pucRetBuffer+m+2,0xEE,3*ucPointNum);
	(*pusPos)=m+3*ucPointNum+2;

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucBuffer=(unsigned char *)mem_ptr;			//工作数据区       	
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

//读取上一整点的电量
unsigned char Main_Get_ReqData_1_PowVal(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	unsigned char i;
	unsigned char ucPointNum,ucPointNumTmp;			//点数
	unsigned char ucM=0;				//密度
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
//			ucTaskNo=CON_GRTASK_NUM;			//无效
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//		ucTaskNo=Task_GetTaskNo_Ter();
	if(ucTaskNo>=CON_GRTASK_NUM)
		return RET_ERROR;
	
//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;			//工作数据区
	ucM=GetFreeseMByMPNo(ucMPNo);
	if((ucM>4) || (ucM==0))
		return RET_ERROR;
	ucM=1<<(ucM-1);							//密度代号转换为密度数值
	ucPointNum=4/ucM;
	ucPointNumTmp=ucPointNum+1;
	stStartTime=GetLastOneHourStartTime(0);
	if(gucJob1HoursFlag>0 && gucJob1HoursFlag<8)
		SubTime(&stStartTime,gucJob1HoursFlag,CON_TIMEUNIT_HOUR);
	pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//第一字节冻结密度BCD码	数据时间（小时）BCD码十位	数据时间（小时）BCD码个位
	pucRetBuffer[m+1]=ucM;
	(*pusPos)=m+4*ucPointNum+2;

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucBuffer=(unsigned char *)mem_ptr;			//工作数据区  
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

//读取上一整点的电量示值
unsigned char Main_Get_ReqData_1_PowViewData(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI)
{
    unsigned char *mem_ptr=NULL,mem_type;
	unsigned char ucRet=RET_SUCCESS;
	unsigned short m=(*pusPos);
	unsigned char i;
	unsigned char ucPointNum;			//点数
	unsigned char ucM=0;				//密度
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
//			ucTaskNo=CON_GRTASK_NUM;			//无效
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//		ucTaskNo=Task_GetTaskNo_Ter();
	if(ucTaskNo>=CON_GRTASK_NUM)
		return RET_ERROR;
	
//	pucBuffer=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;			//工作数据区
	ucM=GetFreeseMByMPNo(ucMPNo);
	if((ucM>4) || (ucM==0))
		return RET_ERROR;
	ucM=1<<(ucM-1);							//密度代号转换为密度数值
	ucPointNum=4/ucM;
	stStartTime=GetLastOneHourStartTime((unsigned char)(15*ucM));
	if(gucJob1HoursFlag>0 && gucJob1HoursFlag<8)
		SubTime(&stStartTime,gucJob1HoursFlag,CON_TIMEUNIT_HOUR);
//	pucRetBuffer[m]=(ucM<<6)+(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//第一字节冻结密度BCD码	数据时间（小时）BCD码十位	数据时间（小时）BCD码个位
	pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//第一字节冻结密度BCD码	数据时间（小时）BCD码十位	数据时间（小时）BCD码个位
	pucRetBuffer[m+1]=ucM;
	memset(pucRetBuffer+m+2,0xEE,4*ucPointNum);
	(*pusPos)=m+4*ucPointNum+2;

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucBuffer=(unsigned char *)mem_ptr;			//工作数据区        
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

//读取上一整点的功率因数
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
	ucM=1<<(ucM-1);							//密度代号转换为密度数值
	ucPointNumP=4/ucM;
	ucPointNumQ=ucPointNumP;
	stStartTime=GetLastOneHourStartTime((unsigned char)(15*ucM));
	if(gucJob1HoursFlag>0 && gucJob1HoursFlag<8)
		SubTime(&stStartTime,gucJob1HoursFlag,CON_TIMEUNIT_HOUR);
//	pucRetBuffer[m]=(ucM<<6)+(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//第一字节冻结密度BCD码	数据时间（小时）BCD码十位	数据时间（小时）BCD码个位
	pucRetBuffer[m]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);	//第一字节冻结密度BCD码	数据时间（小时）BCD码十位	数据时间（小时）BCD码个位
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

	ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBufferP,&stStartTime,ucPointNumP,ucM,&ucDataLenP,usDIP);	//无效点以无效数据补齐
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


//====================  二类数据 ===================
//F1	日冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存
        
		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			ucRet=RET_SUCCESS;		//恢复
//		}else
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//			ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			if(ucRet==RET_SUCCESS)
//			{
//				m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F2	日冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			ucRet=RET_SUCCESS;		//恢复
//		}else
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVVAR);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//			ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			if(ucRet==RET_SUCCESS)
//			{
//				m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F3	日冻结正向有/无功最大需量及发生时间（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			ucRet=RET_SUCCESS;		//恢复
//		}else
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//			ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			if(ucRet==RET_SUCCESS)
//			{
//				m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F4	日冻结反向有/无功最大需量及发生时间（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			ucRet=RET_SUCCESS;		//恢复
//		}else
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVVAR);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//			ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			if(ucRet==RET_SUCCESS)
//			{
//				m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存
//
//		ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F5	日冻结正向有功电能量（总、费率1~M）		
unsigned char Main_Get_ReqData_2_F5(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{//这一天减上一天
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
	pucRetBuffer[m+3]=4;					//费率

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存

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
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F6	日冻结正向无功电能量（总、费率1~M）		
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
	pucRetBuffer[m+3]=4;					//费率

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存

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
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F7	日冻结反向有功电能量（总、费率1~M）		
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
	pucRetBuffer[m+3]=4;					//费率

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存

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
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F8	日冻结反向无功电能量（总、费率1~M）		
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
	pucRetBuffer[m+3]=4;					//费率

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存		
		
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
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F9	抄表日冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）		
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;	//费率

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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存
//
//		ucRet=DataProc_GetData_TerDayFrozen(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;	//费率
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

//F10	抄表日冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）		
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;		//费率

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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存
//
//		ucRet=DataProc_GetData_TerDayFrozen(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;		//费率
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

//F11	抄表日冻结正向有/无功最大需量及发生时间（总、费率1~M）		
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
			stMeterPara=RunPara_GetMeterPara(ucMeterNo);
			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
			pucRetBuffer[m++]=stMeterPara.ucFeeNum;	//费率

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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
//        if(mem_ptr == NULL)
//        {
//            return RET_ERROR;
//        }
//        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//第三级数据缓存
//
//		ucRet=DataProc_GetData_TerDayFrozen(stTempTime,pstHisRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;	//费率
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

//F12	抄表日冻结反向有/无功最大需量及发生时间（总、费率1~M）		
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F17	月冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）		
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;					//费率
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
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			ucRet=RET_SUCCESS;		//恢复
//		}else
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//			ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			if(ucRet==RET_SUCCESS)
//			{
//				m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
//				stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//				m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//				pucRetBuffer[m++]=4;					//费率
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;					//费率
//			m=Main_Get_ReqData_2_Dirkwh(pucRetBuffer,m,pstHisRunData);
//		}
//		free_mem_block(mem_ptr);//
//	}
	else
		return RET_ERROR;

	(*pusPos)=m;
	return ucRet;
}

//F18	月冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）		
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;					//费率
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
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			ucRet=RET_SUCCESS;		//恢复
//		}else
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVVAR);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//			ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			if(ucRet==RET_SUCCESS)
//			{
//				m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
//				stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//				m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//				pucRetBuffer[m++]=4;					//费率
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;					//费率
//			m=Main_Get_ReqData_2_Revkwh(pucRetBuffer,m,pstHisRunData);
//		}
//		free_mem_block(mem_ptr);//
//	}
	else
		return RET_ERROR;

	(*pusPos)=m;
	return ucRet;
}

//F19	月冻结正向有/无功最大需量及发生时间（总、费率1~M）		
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;					//费率
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
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			ucRet=RET_SUCCESS;		//恢复
//		}else
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//			ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			if(ucRet==RET_SUCCESS)
//			{
//				m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
//				stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//				m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//				pucRetBuffer[m++]=4;					//费率
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;					//费率
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

//F20	月冻结反向有/无功最大需量及发生时间（总、费率1~M）		
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//			pucRetBuffer[m++]=4;					//费率
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
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//			ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			if(ucRet==RET_SUCCESS)
//			{
//				m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
//				stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//				m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisPulseRunData->stRecordTime);
//				pucRetBuffer[m++]=4;					//费率
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
//			m=SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,pstHisRunData->stRecordTime);
//			pucRetBuffer[m++]=4;					//费率
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

//F21	月冻结正向有功电能量（总、费率1~M）		
unsigned char Main_Get_ReqData_2_F21(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime)
{//两个月份之差
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
	pucRetBuffer[m+2]=4;					//费率

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F22	月冻结正向无功电能量（总、费率1~M）		
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
	pucRetBuffer[m+2]=4;					//费率

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F23	月冻结反向有功电能量（总、费率1~M）		
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
	pucRetBuffer[m+2]=4;					//费率

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F24	月冻结反向无功电能量（总、费率1~M）		
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
	pucRetBuffer[m+2]=4;					//费率

	ucMPNo = ucDAp;
	ucMPType=RunPara_GetMPType(ucMPNo);
	if(ucMPType==CON_MPTYPE_METER)
	{
		ucMeterNo=RunPara_GetMPDevNo(ucMPNo);
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F25	日分相最大有功功率及发生时间、有功功率为零时间		
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//		if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//		{//有功功率
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->stHisMaxInstantPow.lVal);
//			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisPulseRunData->stHisMaxInstantPow.stDateTime);
//			m+=18;				//分相不支持
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisPulseRunData->usHisZeroTimeInstantPow),2);m+=2;
//			memset(pucRetBuffer+m,0x00,6);			//bin格式无效应该填写0x00，而不是0xEE 
//		}else
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVPOW);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//			ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_REVPOW)
//			{//有功功率
//				m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->stHisMaxInstantPow.lVal);
//				m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisPulseRunData->stHisMaxInstantPow.stDateTime);
//				m+=18;				//分相不支持
//				memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisPulseRunData->usHisZeroTimeInstantPow),2);m+=2;
//				memset(pucRetBuffer+m,0x00,6);			//bin格式无效应该填写0x00，而不是0xEE 
//			}
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F26	日总及分相最大需量及发生时间		
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//		if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//		{//有功需量，无分相
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->stHisMaxDemand.lVal);
//			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisPulseRunData->stHisMaxDemand.stDateTime);
//			m+=18;
//		}else
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVPOW);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//			ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_REVPOW)
//			{//有功需量，无分相
//				m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->stHisMaxDemand.lVal);
//				m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisPulseRunData->stHisMaxDemand.stDateTime);
//				m+=18;
//			}
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F27	日电压统计数据		
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F28	日不平衡度越限累计时间		changed for test 09-11-25
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F29	日电流越限统计		
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F30	日视在功率越限累计时间		
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F31	日电能表断相数据		
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//
        
		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F33	月分相最大有功功率及发生时间、分相有功功率为零时间		
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//		if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//		{//有功功率
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->stHisMaxInstantPow.lVal);
//			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisPulseRunData->stHisMaxInstantPow.stDateTime);
//			m+=18;				//分相不支持
//			memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisPulseRunData->usHisZeroTimeInstantPow),2);m+=2;
//			memset(pucRetBuffer+m,0x00,6);			//bin格式无效应该填写0x00，而不是0xEE 
//		}else
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVPOW);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//			ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_REVPOW)
//			{//有功功率
//				m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->stHisMaxInstantPow.lVal);
//				m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisPulseRunData->stHisMaxInstantPow.stDateTime);
//				m+=18;				//分相不支持
//				memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisPulseRunData->usHisZeroTimeInstantPow),2);m+=2;
//				memset(pucRetBuffer+m,0x00,6);			//bin格式无效应该填写0x00，而不是0xEE 
//			}
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F34	月分相有功最大需量及发生时间、总有功最大需量及发生时间		
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//		if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_DIRPOW)
//		{//有功需量，无分相
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
//			m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->stHisMaxDemand.lVal);
//			m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisPulseRunData->stHisMaxDemand.stDateTime);
//			m+=18;
//		}else
//		{
//			ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVPOW);
//			pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//			ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//			stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//			if(ucRet==RET_SUCCESS && stPulsePara.ucAttr==CON_PULSEATTR_REVPOW)
//			{//有功需量，无分相
//				m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
//				m=SetMainComm_Buf_lVal_3B(pucRetBuffer,m,pstHisPulseRunData->stHisMaxDemand.lVal);
//				m=SetMainComm_Buf_DateTime_DHM(pucRetBuffer,m,pstHisPulseRunData->stHisMaxDemand.stDateTime);
//				m+=18;
//			}
//		}
//	}
//	else if(ucMPType==CON_MPTYPE_TER)
//	{
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F35	月电压统计数据		
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
//			m=Main_Get_ReqData_2_F27F35_Aux(m,pucRetBuffer,pstHisRunData);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);
	(*pusPos)=m;
	return ucRet;
}

//F36	月不平衡度越限累计时间		changed for test 09-11-25
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F37	月电流越限统计		
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
//			m=Main_Get_ReqData_2_F29F37_Aux(m,pucRetBuffer,pstHisRunData);
//		}
//	}
	else
		return RET_ERROR;

    free_mem_block(mem_ptr);
	(*pusPos)=m;
	return ucRet;
}

//F38	月视在功率越限累计时间		
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;	

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F43	日功率因数区段累计时间		
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F44	月功率因数区段累计时间		
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F49	终端日供电时间、日复位累计次数		
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

//	pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
    pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

	ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
	if(ucRet==RET_SUCCESS)
	{
		m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
		memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisTerPowerOnTime),2);	m+=2;
		memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisTerResetNum),2);		m+=2;
	}

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}

//F50	终端日控制统计数据		
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
////	pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//		m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F51	终端月供电时间、月复位累计次数		
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

//	pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
    pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

	ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
	if(ucRet==RET_SUCCESS)
	{
		m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
		memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisTerPowerOnTime),2);	m+=2;
		memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisRunData->usHisTerResetNum),2);		m+=2;
	}

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}

//F52	终端月控制统计数据		
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
////	pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//		m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F53	终端与主站日通信流量	
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

//	pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
    pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

	ucRet=DataProc_GetData_Day_Ter(stTempTime,pstHisRunData);
	if(ucRet==RET_SUCCESS)
	{
		m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
		memcpy(pucRetBuffer+m,&(pstHisRunData->ulHisFluxBytes),4);
		m+=4;
	}
	ucRet=RET_SUCCESS;

    free_mem_block(mem_ptr);//
	(*pusPos)=m;
	return ucRet;
}

//F54	终端与主站月通信流量		
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

//	pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
    pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

	ucRet=DataProc_GetData_Month_Ter(stTempTime,pstHisRunData);
	if(ucRet==RET_SUCCESS)
	{
		m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
		ulFluxBytes=pstHisRunData->ulHisFluxBytes;
		if(stCurrTime.ucYear==stTempTime.ucYear && stCurrTime.ucMonth==stTempTime.ucMonth)
		{//加当天流量
			stTempTime=stCurrTime;
			stTempTime.ucSecond=59;
			stTempTime.ucMinute=59;
			stTempTime.ucHour=23;
			memset(pucRetBuffer+m,0xEE,7);
//			pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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

//F57	总加组日最大、最小有功功率及其发生时间，有功功率为零日累计时间		
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
//	pstHisSumGRunData=(STHISSUMGROUPRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//	ucRet=DataProc_GetSumGData_Day(ucSumGNo,stTempTime,pstHisSumGRunData);
//	if(ucRet==RET_SUCCESS)
//	{
//		m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F58	总加组日累计有功电能量（总、费率1~M）		
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
//	pucRetBuffer[m+3]=4;					//费率
//
//	pstHisSumGRunData=(STHISSUMGROUPRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//		m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
//		pucRetBuffer[m++]=4;					//费率
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

//F59	总加组日累计无功电能量（总、费率1~M）		
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
//	pucRetBuffer[m+3]=4;					//费率
//
//	pstHisSumGRunData=(STHISSUMGROUPRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//		m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
//		pucRetBuffer[m++]=4;					//费率
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

//F60	总加组月最大、最小有功功率及其发生时间，有功功率为零月累计时间		
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
//	pstHisSumGRunData=(STHISSUMGROUPRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//	ucRet=DataProc_GetSumGData_Month(ucSumGNo,stTempTime,pstHisSumGRunData);
//	if(ucRet==RET_SUCCESS)
//	{
//		m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F61	总加组月累计有功电能量（总、费率1~M）
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
//	pucRetBuffer[m+2]=4;					//费率
//
//	pstHisSumGRunData=(STHISSUMGROUPRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//		m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
//		pucRetBuffer[m++]=4;					//费率
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

//F62	总加组月累计无功电能量（总、费率1~M）		
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
//	pucRetBuffer[m+2]=4;					//费率
//
//	pstHisSumGRunData=(STHISSUMGROUPRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//		m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
//		pucRetBuffer[m++]=4;					//费率
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

//F65	总加组超功率定值的月累计时间、月累计电能量		
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
//	pstHisSumGRunData=(STHISSUMGROUPRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//	ucRet=DataProc_GetSumGData_Month(ucSumGNo,stTempTime,pstHisSumGRunData);
//	if(ucRet==RET_SUCCESS)
//	{
//		m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
//		memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisSumGRunData->usSumGroupPowerOverTime),2);m+=2;
//		ulDT3=Float_To_DT3_Div100(pstHisSumGRunData->fSumGroupPowerOverAccValTotal);
//		memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	}
//	(*pusPos)=m;
//	return ucRet;
//}

//F66	总加组超月电能量定值的月累计时间、累计电能量		
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
//	pstHisSumGRunData=(STHISSUMGROUPRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//	ucRet=DataProc_GetSumGData_Month(ucSumGNo,stTempTime,pstHisSumGRunData);
//	if(ucRet==RET_SUCCESS)
//	{
//		m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
//		memcpy(pucRetBuffer+m,(unsigned char *)&(pstHisSumGRunData->usSumGroupEnergyOverTime),2);m+=2;
//		ulDT3=Float_To_DT3_Div100(pstHisSumGRunData->fSumGroupEnergyOverAccValTotal);
//		memcpy(pucRetBuffer+m,&ulDT3,4);	m+=4;
//	}
//	(*pusPos)=m;
// 	return ucRet;
// }


//F73	总加组有功功率曲线	
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
//	ucM=1<<(ucM-1);							//密度代号转换为密度数值
//	ucPointNum=Get_CurvePointNum_ByTime(pstStartTime,pstEndTime,ucM);
//	pusPosBak=m;
//	SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stTmpTime);
//	pucRetBuffer[m+5]=ucDataInter;
//	pucRetBuffer[m+6]=1;					//默认为一点
//	memset(pucRetBuffer+m+7,0xEE,2);		//数据格式02为2字节
//	ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stTmpTime,ucPointNum,ucM,&ucDataLen,usDI);	//无效点以无效数据补齐
//	ucPointNum=pucBuffer[0];
//	if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
//	{
//		m+=7;								//数据开始位置
//		pucBuffer+=2;
//		for(i=0;i<ucPointNum;i++)
//		{
//			if((m+4)>448)					//最多有限制
//				break;
//			memset(ucDesBuf,0,4);
//			memcpy(ucDesBuf,pucBuffer,ucDataLen);
//			pucBuffer+=ucDataLen;
//			if(ucDesBuf[0]!=CON_GRTASK_DATA_INVALID)
//			{
//				memcpy(&fVal,ucDesBuf,4);
//				fVal/=10000;					//有功0.0001kW --》 kW
//				usDT2=Float_To_DT2(fVal);
//				pucRetBuffer[m]=(unsigned char)usDT2;
//				pucRetBuffer[m+1]=(unsigned char)(usDT2>>8);
//			}else
//				memset(pucRetBuffer+m,0xEE,2);
//			m+=2;
//		}
//		pucRetBuffer[pusPosBak+6]=i;			//实际点数
//		(*pusPos)=m;
//	}
// 	return ucRet;
// }

//F74	总加组无功功率曲线
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
//	ucM=1<<(ucM-1);							//密度代号转换为密度数值
//	ucPointNum=Get_CurvePointNum_ByTime(pstStartTime,pstEndTime,ucM);
//	pusPosBak=m;
//	SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stTmpTime);
//	pucRetBuffer[m+5]=ucDataInter;
//	pucRetBuffer[m+6]=1;					//默认为一点
//	memset(pucRetBuffer+m+7,0xEE,2);		//数据格式02为2字节
//	ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stTmpTime,ucPointNum,ucM,&ucDataLen,usDI);	//无效点以无效数据补齐
//	ucPointNum=pucBuffer[0];
//	if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
//	{
//		m+=7;								//数据开始位置
//		pucBuffer+=2;
//		for(i=0;i<ucPointNum;i++)
//		{
//			if((m+4)>448)					//最多有限制
//				break;
//			memset(ucDesBuf,0,4);
//			memcpy(ucDesBuf,pucBuffer,ucDataLen);
//			pucBuffer+=ucDataLen;
//			if(ucDesBuf[0]!=CON_GRTASK_DATA_INVALID)
//			{
//				memcpy(&fVal,ucDesBuf,4);
//				fVal/=100;						//无功0.01kvar --》 kvar
//				usDT2=Float_To_DT2(fVal);
//				pucRetBuffer[m]=(unsigned char)usDT2;
//				pucRetBuffer[m+1]=(unsigned char)(usDT2>>8);
//			}else
//				memset(pucRetBuffer+m,0xEE,2);
//			m+=2;
//		}
//		pucRetBuffer[pusPosBak+6]=i;			//实际点数
//		(*pusPos)=m;
//	}
//	return ucRet;
//}

//F75	总加组有功电能量曲线	
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

//F76	总加组无功电能量曲线
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

//F81  总有功功率曲线
unsigned char Main_Get_ReqData_2_F81(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_InstantPow(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPINSTANTPOW);
	return ucRet;
}

//F82  A相有功功率曲线
unsigned char Main_Get_ReqData_2_F82(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_InstantPow(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPINSTANTPOWA);
	return ucRet;
}

//F83  B相有功功率曲线
unsigned char Main_Get_ReqData_2_F83(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_InstantPow(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPINSTANTPOWB);
	return ucRet;
}

//F84  C相有功功率曲线
unsigned char Main_Get_ReqData_2_F84(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_InstantPow(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPINSTANTPOWC);
	return ucRet;
}

//F85  总无功功率曲线
unsigned char Main_Get_ReqData_2_F85(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_InstantPow(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPINSTANTVAR);
	return ucRet;
}

//F86  A相无功功率曲线
unsigned char Main_Get_ReqData_2_F86(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_InstantPow(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPINSTANTVARA);
	return ucRet;
}

//F87  B相无功功率曲线
unsigned char Main_Get_ReqData_2_F87(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_InstantPow(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPINSTANTVARB);
	return ucRet;
}

//F88  C相无功功率曲线
unsigned char Main_Get_ReqData_2_F88(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_InstantPow(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPINSTANTVARB);
	return ucRet;
}

//F89	A相电压曲线	
unsigned char Main_Get_ReqData_2_F89(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_Volt(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPVOLA);
	return ucRet;
}

//F90	B相电压曲线	
unsigned char Main_Get_ReqData_2_F90(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_Volt(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPVOLB);
	return ucRet;
}

//F91	C相电压曲线	
unsigned char Main_Get_ReqData_2_F91(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_Volt(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPVOLC);
	return ucRet;
}

//F92	A相电流曲线	
unsigned char Main_Get_ReqData_2_F92(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_Curr(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPCURA);
	return ucRet;
}

//F93	B相电流曲线	
unsigned char Main_Get_ReqData_2_F93(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_Curr(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPCURB);
	return ucRet;
}

//F94	C相电流曲线	
unsigned char Main_Get_ReqData_2_F94(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_Curr(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPCURC);
	return ucRet;
}

//F95	零序电流曲线
unsigned char Main_Get_ReqData_2_F95(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=RET_ERROR;
	return ucRet;
}

//F97	正向有功总电能量		
unsigned char Main_Get_ReqData_2_F97(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;	
	ucRet=Main_Get_ReqData_2_PowVal(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPDIRPOWTOTAL);
	return ucRet;
}

//F98	正向无功总电能量		
unsigned char Main_Get_ReqData_2_F98(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;	
	ucRet=Main_Get_ReqData_2_PowVal(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPDIRVARTOTAL);
	return ucRet;
}

//F99	反向有功总电能量		
unsigned char Main_Get_ReqData_2_F99(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;	
	ucRet=Main_Get_ReqData_2_PowVal(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPUNDIRPOWTOTAL);
	return ucRet;
}

//F100	反向无功总电能量		
unsigned char Main_Get_ReqData_2_F100(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;	
	ucRet=Main_Get_ReqData_2_PowVal(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPUNDIRVARTOTAL);
	return ucRet;
}

//F101	正向有功总电能示值曲线
unsigned char Main_Get_ReqData_2_F101(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_PowViewData(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPDIRPOWTOTAL);
	return ucRet;
}

//F102	正向无功总电能示值曲线
unsigned char Main_Get_ReqData_2_F102(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_PowViewData(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPDIRVARTOTAL);
	return ucRet;
}

//F103	反向有功总电能示值曲线
unsigned char Main_Get_ReqData_2_F103(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_PowViewData(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPUNDIRPOWTOTAL);
	return ucRet;
}

//F104	反向无功总电能示值曲线
unsigned char Main_Get_ReqData_2_F104(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_PowViewData(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPUNDIRVARTOTAL);
	return ucRet;
}

//F105	总功率因数		
unsigned char Main_Get_ReqData_2_F105(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_PowRate(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPPOWRATE);
	return ucRet;
}

//F106	A相功率因数		
unsigned char Main_Get_ReqData_2_F106(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_PowRate(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPPOWRATEA);
	return ucRet;
}

//F107	B相功率因数		
unsigned char Main_Get_ReqData_2_F107(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_PowRate(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPPOWRATEB);
	return ucRet;
}

//F108	C相功率因数		
unsigned char Main_Get_ReqData_2_F108(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_PowRate(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPPOWRATEC);
	return ucRet;
}

//F109	C相功率因数		
unsigned char Main_Get_ReqData_2_F109(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;
//	ucRet=Main_Get_ReqData_2_Volt_Phase(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPPOWRATEC);
	return ucRet;
}

//F110	C相功率因数		
unsigned char Main_Get_ReqData_2_F110(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;
//	ucRet=Main_Get_ReqData_2_Curr_Phase(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPPOWRATEC);
	return ucRet;
}


//F145	一象限无功曲线		
unsigned char Main_Get_ReqData_2_F145(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_PowViewData(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPVAR1);
	return ucRet;
}

//F146	二象限无功曲线		
unsigned char Main_Get_ReqData_2_F146(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_PowViewData(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPVAR2);
	return ucRet;
}

//F147	三象限无功曲线		
unsigned char Main_Get_ReqData_2_F147(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_PowViewData(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPVAR3);
	return ucRet;
}

//F148	四象限无功曲线		
unsigned char Main_Get_ReqData_2_F148(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter)
{	
	unsigned char ucRet=RET_SUCCESS;
	ucRet=Main_Get_ReqData_2_PowViewData(ucDAp,pucRetBuffer,pusPos,pstStartTime,pstEndTime,ucDataInter,CON_DI_TYPE_MPVAR4);
	return ucRet;
}
//azh 170923
//F219	测量点组合数据曲线数据单元格式
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Tm
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

//F161	日冻结正向有功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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


//F162	日冻结正向无功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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


//F163	日冻结反向有功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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


//F164	日冻结反向无功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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


//F165	日冻结一象限无功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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


//F166	日冻结二象限无功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F167	日冻结三象限无功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F168	日冻结四象限无功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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


//F169	抄表日冻结正向有功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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


//F170	抄表日冻结正向无功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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


//F171	抄表日冻结反向有功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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


//F172	抄表日冻结反向无功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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


//F173	抄表日冻结一象限无功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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


//F174	抄表日冻结二象限无功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F175	抄表日冻结三象限无功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F176	抄表日冻结四象限无功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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


//F177	月冻结正向有功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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


//F178	月冻结正向无功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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


//F179	月冻结反向有功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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


//F180	月冻结反向无功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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


//F181	月冻结一象限无功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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


//F182	月冻结二象限无功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F183	月冻结三象限无功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F184	月冻结四象限无功电能示值（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td
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



//F185	日冻结正向有功最大需量及发生时间（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F186	日冻结正向无功最大需量及发生时间（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//F187	日冻结反向有功最大需量及发生时间（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//F188	日冻结反向无功最大需量及发生时间（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Day(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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


//F189	抄表日冻结正向有功最大需量及发生时间（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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

//F190	抄表日冻结正向无功最大需量及发生时间（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//F191	抄表日冻结反向有功最大需量及发生时间（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
//F192	抄表日冻结反向无功最大需量及发生时间（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_DayFrozen(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YMD(pucRetBuffer,m,stTempTime);		//数据时标Td
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


//F193	月冻结正向有功最大需量及发生时间（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Td  changed for test 09-11-25
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Tdchanged for test 09-11-25
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Tdchanged for test 09-11-25
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

//F194	月冻结正向无功最大需量及发生时间（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Tdchanged for test 09-11-25
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Tdchanged for test 09-11-25
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Tdchanged for test 09-11-25
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
//F195	月冻结反向有功最大需量及发生时间（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Tdchanged for test 09-11-25
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Month_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Tdchanged for test 09-11-25
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Tdchanged for test 09-11-25
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
//F196	月冻结反向无功最大需量及发生时间（总、费率1~M）	
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
//		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr == NULL)
        {
            return RET_ERROR;
        }
        pstHisRunData=(STHISRUNDATA *)mem_ptr;		//

		ucRet=DataProc_GetData_Month(ucMeterNo,stTempTime,pstHisRunData);
		if(ucRet==RET_SUCCESS)
		{
			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Tdchanged for test 09-11-25
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
//		pstHisPulseRunData=(STHISPULSERUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
//		ucRet=DataProc_GetData_Day_Pulse(ucPulseNo,stTempTime,pstHisPulseRunData);
//		if(ucRet==RET_SUCCESS)
//		{
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Tdchanged for test 09-11-25
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
////		pstHisRunData=(STHISRUNDATA *)(CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF);		//第三级数据缓存
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
//			m=SetMainComm_Buf_DateTime_YM(pucRetBuffer,m,stTempTime);		//数据时标Tdchanged for test 09-11-25
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
{//根据开始时间和结束时间与数据密度（15分钟的倍数）来计算数据点数
	unsigned char ucPointNum;
	unsigned long ulStart,ulEnd,ulTmp;
	ulStart=TimeConver(pstStartTime,CON_TIMEUNIT_MIN);
	ulEnd=TimeConver(pstEndTime,CON_TIMEUNIT_MIN);
	if(ulStart<ulEnd)
	{
		ulTmp=(ulEnd-ulStart)/(15*ucM);	//15分钟密度，进一法
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
{//功率
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
////			ucTaskNo=CON_GRTASK_NUM;			//无效
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
	ucM=1<<(ucM-1);							//密度代号转换为密度数值
	ucPointNum=Get_CurvePointNum_ByTime(pstStartTime,pstEndTime,ucM);
	pusPosBak=m;
	SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stTmpTime);
	pucRetBuffer[m+5]=ucDataInter;
	pucRetBuffer[m+6]=1;					//默认为一点
	memset(pucRetBuffer+m+7,0xEE,3);		//数据格式09为3字节

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucBuffer=(unsigned char *)mem_ptr;			//工作数据区 
	           
	ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stTmpTime,ucPointNum,ucM,&ucDataLen,usDI);	//无效点以无效数据补齐
	ucPointNum=pucBuffer[0];
	if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
	{
		m+=7;
		pucBuffer+=2;
		for(i=0;i<ucPointNum;i++)
		{
			if((m+4)>448)					//最多有限制
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
						ucDesBuf[1]=0x79;			//数据格式9，目前不处理负数据，最大值79.9999
					memcpy(pucRetBuffer+m+1,ucDesBuf,2);		//无功2字节，0 补齐低字节
				}
			}else
			{
				if(ucDesBuf[0]==0xEE)
				{
					memset(pucRetBuffer+m,0xEE,3);
				}else
				{
					if(ucDesBuf[2]>0x79)
						ucDesBuf[2]=0x79;			//数据格式9，目前不处理负数据，最大值79.9999
					memcpy(pucRetBuffer+m,ucDesBuf,3);			//有功3字节
				}
			}
			m+=3;
		}
		pucRetBuffer[pusPosBak+6]=i;			//实际点数
		(*pusPos)=m;
	}

    free_mem_block(mem_ptr);//
	return ucRet;
}
unsigned char Main_Get_ReqData_2_Volt(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI)
{//电压
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
	ucM=1<<(ucM-1);							//密度代号转换为密度数值
	ucPointNum=Get_CurvePointNum_ByTime(pstStartTime,pstEndTime,ucM);
	pusPosBak=m;
	SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stTmpTime);
	pucRetBuffer[m+5]=ucDataInter;
	pucRetBuffer[m+6]=1;					//默认为一点
	memset(pucRetBuffer+m+7,0xEE,2);

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucBuffer=(unsigned char *)mem_ptr;			//工作数据区        

	ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stTmpTime,ucPointNum,ucM,&ucDataLen,usDI);	//无效点以无效数据补齐
	ucPointNum=pucBuffer[0];
	if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
	{
		m+=7;
		pucBuffer+=2;
		for(i=0;i<ucPointNum;i++)
		{
			if((m+4)>448)					//最多有限制
				break;
			ulVal=0;
			memcpy((unsigned char *)&ulVal,pucBuffer,ucDataLen);
		//	if((unsigned char)ulVal!=0xEE)
		//		ulVal=(ulVal<<4);				//数据格式08转换为数据格式07
			pucBuffer+=ucDataLen;
			memcpy(pucRetBuffer+m,(unsigned char *)&ulVal,2);
			m+=2;
		}
		pucRetBuffer[pusPosBak+6]=i;			//实际点数
		(*pusPos)=m;
	}

    free_mem_block(mem_ptr);//
	return ucRet;
}

unsigned char Main_Get_ReqData_2_Curr(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI)
{//电流
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
	ucM=1<<(ucM-1);							//密度代号转换为密度数值
	ucPointNum=Get_CurvePointNum_ByTime(pstStartTime,pstEndTime,ucM);
	pusPosBak=m;
	SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stTmpTime);
	pucRetBuffer[m+5]=ucDataInter;
	pucRetBuffer[m+6]=1;					//默认为一点
	memset(pucRetBuffer+m+7,0xEE,2);

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucBuffer=(unsigned char *)mem_ptr;			//工作数据区     

	ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stTmpTime,ucPointNum,ucM,&ucDataLen,usDI);	//无效点以无效数据补齐
	ucPointNum=pucBuffer[0];
	if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
	{
		m+=7;
		pucBuffer+=2;
		for(i=0;i<ucPointNum;i++)
		{
			if((m+4)>448)					//最多有限制
				break;
			ulVal=0;
			memcpy((unsigned char *)&ulVal,pucBuffer,ucDataLen);
			pucBuffer+=ucDataLen;
			memcpy(pucRetBuffer+m,(unsigned char *)&ulVal,2);
			m+=2;
		}
		pucRetBuffer[pusPosBak+6]=i;			//实际点数
		(*pusPos)=m;
	}
    free_mem_block(mem_ptr);//
	return ucRet;
}

/*
unsigned char Main_Get_ReqData_2_Volt_Phase(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI)
{//电压相角
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
	ucM=1<<(ucM-1);							//密度代号转换为密度数值
	ucPointNum=Get_CurvePointNum_ByTime(pstStartTime,pstEndTime,ucM);
	pusPosBak=m;
	SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stTmpTime);
	pucRetBuffer[m+5]=ucDataInter;
	pucRetBuffer[m+6]=1;					//默认为一点
	memset(pucRetBuffer+m+7,0xEE,2);
	ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stTmpTime,ucPointNum,ucM,&ucDataLen,usDI);	//无效点以无效数据补齐
	ucPointNum=pucBuffer[0];
	if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
	{
		m+=7;
		pucBuffer+=2;
		for(i=0;i<ucPointNum;i++)
		{
			if((m+4)>448)					//最多有限制
				break;
			ulVal=0;
			memcpy((unsigned char *)&ulVal,pucBuffer,ucDataLen);
			pucBuffer+=ucDataLen;
			memcpy(pucRetBuffer+m,(unsigned char *)&ulVal,2);
			m+=2;
		}
		pucRetBuffer[pusPosBak+6]=i;			//实际点数
		(*pusPos)=m;
	}
	return ucRet;
}

unsigned char Main_Get_ReqData_2_Curr_Phase(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI)
{//电流相角
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
	ucM=1<<(ucM-1);							//密度代号转换为密度数值
	ucPointNum=Get_CurvePointNum_ByTime(pstStartTime,pstEndTime,ucM);
	pusPosBak=m;
	SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stTmpTime);
	pucRetBuffer[m+5]=ucDataInter;
	pucRetBuffer[m+6]=1;					//默认为一点
	memset(pucRetBuffer+m+7,0xEE,2);
	ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stTmpTime,ucPointNum,ucM,&ucDataLen,usDI);	//无效点以无效数据补齐
	ucPointNum=pucBuffer[0];
	if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
	{
		m+=7;
		pucBuffer+=2;
		for(i=0;i<ucPointNum;i++)
		{
			if((m+4)>448)					//最多有限制
				break;
			ulVal=0;
			memcpy((unsigned char *)&ulVal,pucBuffer,ucDataLen);
			pucBuffer+=ucDataLen;
			memcpy(pucRetBuffer+m,(unsigned char *)&ulVal,2);
			m+=2;
		}
		pucRetBuffer[pusPosBak+6]=i;			//实际点数
		(*pusPos)=m;
	}
	return ucRet;
}
*/
unsigned char Main_Get_ReqData_2_PowVal(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI)
{//电能量
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
////			ucTaskNo=CON_GRTASK_NUM;			//无效
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
	ucM=1<<(ucM-1);							//密度代号转换为密度数值
	ucPointNum=Get_CurvePointNum_ByTime(pstStartTime,pstEndTime,ucM);
	pusPosBak=m;
	SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stTmpTime);
	pucRetBuffer[m+5]=ucDataInter;
	pucRetBuffer[m+6]=1;					//默认为一点
	memset(pucRetBuffer+m+7,0xEE,4);
	ucPointNum=ucPointNum+1;				//加一点
	SubTime(&stTmpTime,(unsigned char)(15*ucM),CON_TIMEUNIT_MIN);

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucBuffer=(unsigned char *)mem_ptr;			//工作数据区       

	ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stTmpTime,ucPointNum,ucM,&ucDataLen,usDI);	//无效点以无效数据补齐
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
			if((m+4)>448)					//最多有限制
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
		pucRetBuffer[pusPosBak+6]=i;			//实际点数
		(*pusPos)=m;
	}

    free_mem_block(mem_ptr);//
	return ucRet;
}

unsigned char Main_Get_ReqData_2_PowViewData(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI)
{//电能量示值
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
////			ucTaskNo=CON_GRTASK_NUM;			//无效
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
	ucM=1<<(ucM-1);							//密度代号转换为密度数值
	ucPointNum=Get_CurvePointNum_ByTime(pstStartTime,pstEndTime,ucM);
	pusPosBak=m;
	SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stTmpTime);
	pucRetBuffer[m+5]=ucDataInter;
	pucRetBuffer[m+6]=1;					//默认为一点
	memset(pucRetBuffer+m+7,0xEE,4);

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucBuffer=(unsigned char *)mem_ptr;			//工作数据区        

	ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stTmpTime,ucPointNum,ucM,&ucDataLen,usDI);	//无效点以无效数据补齐
	ucPointNum=pucBuffer[0];
	if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
	{
		m+=7;
		pucBuffer+=2;
		for(i=0;i<ucPointNum;i++)
		{
			if((m+4)>448)					//最多有限制
				break;
			memcpy(pucRetBuffer+m,pucBuffer,ucDataLen);
			m+=4;
			pucBuffer+=ucDataLen;
		}
		pucRetBuffer[pusPosBak+6]=i;			//实际点数
		(*pusPos)=m;
	}

    free_mem_block(mem_ptr);//
	return ucRet;
}

unsigned char Main_Get_ReqData_2_PowRate(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI)
{//功率因数
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
	ucM=1<<(ucM-1);							//密度代号转换为密度数值
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
	pucRetBuffer[m+6]=1;					//默认为一点
	memset(pucRetBuffer+m+7,0xEE,2);

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucBufferP=(unsigned char *)mem_ptr;
	pucBufferQ=(unsigned char *)mem_ptr+768;

	ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBufferP,&stTmpTime,ucPointNumP,ucM,&ucDataLenP,usDIP);	//无效点以无效数据补齐
	ucPointNumP=pucBufferP[0];
	if(ucRet!=RET_ERROR && ucPointNumP!=0 && ucDataLenP!=0)
	{
		ucRet=Task_QueryRetVal_ByDI(ucTaskNo2,pucBufferQ,&stTmpTime,ucPointNumQ,ucM,&ucDataLenQ,usDIQ);	//无效点以无效数据补齐
		ucPointNumQ=pucBufferQ[0];
		if(ucRet!=RET_ERROR && ucPointNumQ==ucPointNumP && ucDataLenQ!=0)
		{
			m+=7;
			pucBufferP+=2;
			pucBufferQ+=2;
			for(i=0;i<ucPointNumP;i++)
			{
				if((m+4)>448)					//最多有限制
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
			pucRetBuffer[pusPosBak+6]=i;			//实际点数
			(*pusPos)=m;
		}
	}

    free_mem_block(mem_ptr);//
	return ucRet;
}

unsigned char Main_Get_ReqData_2_SumGroup_Pow(unsigned char ucGroupNo,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI,unsigned char ucPowFlag)
{//总加电量
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
//	ucM=1<<(ucM-1);							//密度代号转换为密度数值
//	ucPointNum=Get_CurvePointNum_ByTime(pstStartTime,pstEndTime,ucM);
//	pusPosBak=m;
//	SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stTmpTime);
//	pucRetBuffer[m+5]=ucDataInter;
//	pucRetBuffer[m+6]=1;					//默认为一点
//	memset(pucRetBuffer+m+7,0xEE,4);		//数据格式03为4字节
//	ucPointNum=ucPointNum+1;				//加一点
//	SubTime(&stTmpTime,(unsigned char)(15*ucM),CON_TIMEUNIT_MIN);
//	ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stTmpTime,ucPointNum,ucM,&ucDataLen,usDI);	//无效点以无效数据补齐
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
//			if((m+4)>448)					//最多有限制
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
//		pucRetBuffer[pusPosBak+6]=i;			//实际点数
//		(*pusPos)=m;
//	}
	return ucRet;
}
//azh 170923
unsigned char Main_Get_ReqData_2_GroupCurveData(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI)
{//电能量示值
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
////			ucTaskNo=CON_GRTASK_NUM;			//无效
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
	ucM=1<<(ucM-1);							//密度代号转换为密度数值
	ucPointNum=Get_CurvePointNum_ByTime(pstStartTime,pstEndTime,ucM);
	pusPosBak=m;
	SetMainComm_Buf_DateTime_YMDHM(pucRetBuffer,m,stTmpTime);
	pucRetBuffer[m+5]=ucDataInter;
	pucRetBuffer[m+6]=1;					//默认为一点
	memset(pucRetBuffer+m+7,0xEE,33);

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
	pucBuffer=(unsigned char *)mem_ptr;			//工作数据区        

	ucRet=Task_QueryRetVal_ByGroupDI(ucTaskNo,pucBuffer,&stTmpTime,ucPointNum,ucM,&ucDataLen,usDI);	//无效点以无效数据补齐
	ucPointNum=pucBuffer[0];
	if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
	{
		m+=7;
		pucBuffer+=2;
		for(i=0;i<ucPointNum;i++)
		{
			if((m+33)>448)					//最多有限制
				break;

//有功功率
			memset(ucDesBuf,0,4);
			memcpy(ucDesBuf,pucBuffer,3);
			pucBuffer+=3;
			if(ucDesBuf[0]==0xEE)
			{
				memset(pucRetBuffer+m,0xEE,3);
			}else
			{
				if(ucDesBuf[2]>0x79)
					ucDesBuf[2]=0x79;			//数据格式9，目前不处理负数据，最大值79.9999
				memcpy(pucRetBuffer+m,ucDesBuf,3);			//有功3字节
			}
			m+=3;
//无功功率
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
					ucDesBuf[1]=0x79;			//数据格式9，目前不处理负数据，最大值79.9999
				memcpy(pucRetBuffer+m+1,ucDesBuf,2);		//无功2字节，0 补齐低字节
			}
			m+=3;
//ABC相电压
			memcpy(pucRetBuffer+m,pucBuffer,6);
			pucBuffer+=6;
			m+=6;
//ABC相电流
			memcpy(pucRetBuffer+m,pucBuffer,9);
			pucBuffer+=9;
			m+=9;
//正向有功、一四象限无功电能量示值
			memcpy(pucRetBuffer+m,pucBuffer,12);
			pucBuffer+=12;
			m+=12;

		}
		pucRetBuffer[pusPosBak+6]=i;			//实际点数
		(*pusPos)=m;
	}

    free_mem_block(mem_ptr);//
	return ucRet;
}

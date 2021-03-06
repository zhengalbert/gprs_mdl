/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_Data_Info.c
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

static unsigned char Main_DI_Req1_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,unsigned char * pucRecvBuffer);
static unsigned char Main_DI_Req2_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,unsigned char * pucRecvBuffer);
static unsigned char Main_DI_Req3_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI);
/**************************************************************************
*
*
*修改记录:2006-06-09 修改参数传入,将原来的数据一个一个查询的方式改为switch
*
****************************************************************************/
unsigned char Main_Comm_Data_Query_Manage(STDIINFO *stRetDIInfo,unsigned char ucRepType,unsigned short usDI,unsigned char *pucRecvBuffer)
{
	//STDIINFO stRetDIInfo;
	unsigned char	ucRet=RET_SUCCESS;
	ClearDIInfo( stRetDIInfo );
	switch(ucRepType)
{
	case CON_TERMSG_REQDATA_1:
       	ucRet=Main_DI_Req1_Query_Manage(stRetDIInfo,usDI,pucRecvBuffer);
		break;
	case CON_TERMSG_REQDATA_2:
       	ucRet=Main_DI_Req2_Query_Manage(stRetDIInfo,usDI,pucRecvBuffer);
		break;
	case CON_TERMSG_REQDATA_3:
       	ucRet=Main_DI_Req3_Query_Manage(stRetDIInfo,usDI);
		break;
	default:
		ucRet=RET_ERROR;
		break;
	}
	return ucRet;
}

unsigned char  Main_DI_Req1_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,unsigned char* pucRecvBuffer)
{//这些数据的长度基本,回返回数据长度
	//STDIINFO stRetDIInfo;
	unsigned char	  ucRet=RET_SUCCESS;
	unsigned char	ucNum1=0;//ucNum=0,
	
//	ucNum = pucRecvBuffer[0];
//	ucNum1=(ucNum>>6)&0xf3;
	ucNum1=4;	//固定4费率
	ClearDIInfo( stRetDIInfo );
	
	if(usDI < CON_DATA_REQ1_BEG_CODE || usDI > CON_DATA_REQ1_END_CODE)
	{
		ucRet=RET_ERROR;
		return ucRet;
	}
	SetDIInfo(stRetDIInfo,
		CON_DI_HAVE,									
		CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
		CON_DI_OPERATE_DISABLE,	CON_DI_SETPURVIEW_ADMIN,
		CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_DISABLE,	
		CON_DI_LEN_CHANGED,0,	CON_DI_TYPE_INVALID);

	switch(usDI)
	{
	case	CON_DATA_REQ1_F1:		//F1	终端版本信息
		stRetDIInfo->ucContentLen=30;
		break;
	case	CON_DATA_REQ1_F2:		//F2	终端日历时钟
		stRetDIInfo->ucContentLen=6;
		break;
	case	CON_DATA_REQ1_F3:		//F3	终端参数状态
		stRetDIInfo->ucContentLen=31;
		break;
	case	CON_DATA_REQ1_F4:		//F4	终端通信状态
		stRetDIInfo->ucContentLen=1;
		break;
	case	CON_DATA_REQ1_F5:		//F5	终端控制设置状态
		stRetDIInfo->ucContentLen=6*CON_SUMGROUP_NUM+2;
		break;
	case	CON_DATA_REQ1_F6:		//F6	终端当前控制状态
		stRetDIInfo->ucContentLen=8*CON_SUMGROUP_NUM+3;
		break;
	case	CON_DATA_REQ1_F7:		//F7	终端事件计数器当前值
		stRetDIInfo->ucContentLen=2;
		break;
	case	CON_DATA_REQ1_F8:		//F8	终端事件标志状态
		stRetDIInfo->ucContentLen=8;
		break;
	case	CON_DATA_REQ1_F9:		//F9	终端状态量及变位标志
		stRetDIInfo->ucContentLen=2;
		break;
	case	CON_DATA_REQ1_F10:		//F10	终端与主站当日、月通信流量
		stRetDIInfo->ucContentLen=8;
		break;
	case	CON_DATA_REQ1_F11:		//F11	当前信号强度
		stRetDIInfo->ucContentLen=1;
		break;
//azh 170921
	case	CON_DATA_REQ1_F12:		//F12	控制输出回路开关接入状态量及变位标志
		stRetDIInfo->ucContentLen=2;
		break;
	case	CON_DATA_REQ1_F13:		//F13	搜索到的电表信息
		stRetDIInfo->ucContentLen=4;
		break;
	case	CON_DATA_REQ1_F14:		//F14	文件传输未收到数据段
		stRetDIInfo->ucContentLen=130;
		break;		
//	case	CON_DATA_REQ1_F17:		//F17	当前总加有功功率
//	case	CON_DATA_REQ1_F18:		//F18	当前总加无功功率
//		stRetDIInfo->ucContentLen=2;
//		break;
//	case	CON_DATA_REQ1_F19:		//F19	当日总加有功电能量（总、费率1~M）
//	case	CON_DATA_REQ1_F20:		//F20	当日总加无功电能量（总、费率1~M）
//	case	CON_DATA_REQ1_F21:		//F21	当月总加有功电能量（总、费率1~M）
//	case	CON_DATA_REQ1_F22:		//F22	当月总加无功电能量（总、费率1~M）
//		stRetDIInfo->ucContentLen=4*(4+1)+1;
//		break;
//	case	CON_DATA_REQ1_F23:		//F23	终端当前剩余电量（费）
//		stRetDIInfo->ucContentLen=4;
//		break;
//	case	CON_DATA_REQ1_F24:		//F24	当前功率下浮控控后总加有功功率冻结值
//		stRetDIInfo->ucContentLen=2;
//		break;
	case	CON_DATA_REQ1_F25:		//F25	当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流
		stRetDIInfo->ucContentLen=67;
		break;
	case	CON_DATA_REQ1_F26:		//F26	A、B、C三相断相统计数据及最近一次断相记录
		stRetDIInfo->ucContentLen=57;
		break;
	case	CON_DATA_REQ1_F27:		//F27	电能表日历时钟、编程次数及其最近一次操作时间
		stRetDIInfo->ucContentLen=60;
		break;
	case	CON_DATA_REQ1_F28:		//F28	电表运行状态字及其变位标志
		stRetDIInfo->ucContentLen=33;
		break;
	case 	CON_DATA_REQ1_F31:
	case 	CON_DATA_REQ1_F32:
		stRetDIInfo->ucContentLen = 59;
	case	CON_DATA_REQ1_F33:		//F33	当前正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
	case	CON_DATA_REQ1_F34:		//F34	当前反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
		stRetDIInfo->ucContentLen=(5+4+4+4)*(4+1)+6;
		break;
	case	CON_DATA_REQ1_F35:		//F35	当月正向有/无功最大需量及发生时间（总、费率1~M）
	case	CON_DATA_REQ1_F36:		//F36	当月反向有/无功最大需量及发生时间（总、费率1~M）
		stRetDIInfo->ucContentLen=(3+4+3+4)*(4+1)+6;
		break;
	case	CON_DATA_REQ1_F37:		//F37	上月正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
	case	CON_DATA_REQ1_F38:		//F38	上月反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
		stRetDIInfo->ucContentLen=(5+4+4+4)*(4+1)+6;
		break;
	case	CON_DATA_REQ1_F39:		//F39	上月正向有/无功最大需量及发生时间（总、费率1~M）
	case	CON_DATA_REQ1_F40:		//F40	上月反向有/无功最大需量及发生时间（总、费率1~M）
		stRetDIInfo->ucContentLen=(3+4+3+4)*(4+1)+6;
		break;
	case	CON_DATA_REQ1_F41:		//F41	当日正向有功电能量（总、费率1~M）
	case	CON_DATA_REQ1_F42:		//F42	当日正向无功电能量（总、费率1~M）
	case	CON_DATA_REQ1_F43:		//F43	当日反向有功电能量（总、费率1~M）
	case	CON_DATA_REQ1_F44:		//F44	当日反向无功电能量（总、费率1~M）
	case	CON_DATA_REQ1_F45:		//F45	当月正向有功电能量（总、费率1~M）
	case	CON_DATA_REQ1_F46:		//F46	当月正向无功电能量（总、费率1~M）
	case	CON_DATA_REQ1_F47:		//F47	当月反向有功电能量（总、费率1~M）
	case	CON_DATA_REQ1_F48:		//F48	当月反向无功电能量（总、费率1~M）
		stRetDIInfo->ucContentLen=4*(4+1)+1;
		break;
	case	CON_DATA_REQ1_F49:		//F49	当前电压、电流相位角
		stRetDIInfo->ucContentLen=12;
		break;
//	case	CON_DATA_REQ1_F57:		//F57	当前A、B、C三相电压、电流2~N次谐波有效值
//		stRetDIInfo->ucContentLen=(ucNum-1)*12;
//		break;
//	case	CON_DATA_REQ1_F58:		//F58	当前A、B、C三相电压、电流2~N次谐波含有率
//		stRetDIInfo->ucContentLen=(ucNum-1)*12+6;
//		break;
//	case	CON_DATA_REQ1_F65:		//F65	当前电容器投切状态
//		stRetDIInfo->ucContentLen=3;
//		break;
//	case	CON_DATA_REQ1_F66:		//F66	当前电容器累计补偿投入时间和次数
//		stRetDIInfo->ucContentLen=72;
//		break;
//	case	CON_DATA_REQ1_F67:		//F67	当日、当月电容器累计补偿的无功电能量
//		stRetDIInfo->ucContentLen=8;
//		break;
//	case	CON_DATA_REQ1_F73:		//F73	直流模拟量实时数据
//		stRetDIInfo->ucContentLen=8;
//		break;
//	case	CON_DATA_REQ1_F81:		//F81	小时冻结总加有功功率
//	case	CON_DATA_REQ1_F82:		//F82	小时冻结总加无功功率
//		stRetDIInfo->ucContentLen=ucNum1*2+2;
//		break;
//	case	CON_DATA_REQ1_F83:		//F83	小时冻结总加有功总电能量
//	case	CON_DATA_REQ1_F84:		//F84	小时冻结总加无功总电能量
//		stRetDIInfo->ucContentLen=ucNum1*4+2;
//		break;
	case	CON_DATA_REQ1_F89:		//F89	小时冻结有功功率
	case	CON_DATA_REQ1_F90:		//F90	小时冻结A相有功功率
	case	CON_DATA_REQ1_F91:		//F91	小时冻结B相有功功率
	case	CON_DATA_REQ1_F92:		//F92	小时冻结C相有功功率
	case	CON_DATA_REQ1_F93:		//F93	小时冻结无功功率
	case	CON_DATA_REQ1_F94:		//F94	小时冻结A相无功功率
	case	CON_DATA_REQ1_F95:		//F95	小时冻结B相无功功率
	case	CON_DATA_REQ1_F96:		//F96	小时冻结C相无功功率
		stRetDIInfo->ucContentLen=ucNum1*3+2;
		break;
	case	CON_DATA_REQ1_F97:		//F97	小时冻结A相电压
	case	CON_DATA_REQ1_F98:		//F98	小时冻结B相电压
	case	CON_DATA_REQ1_F99:		//F99	小时冻结C相电压
		stRetDIInfo->ucContentLen=ucNum1*2+2;
		break;
	case	CON_DATA_REQ1_F100:		//F100	小时冻结A相电流
	case	CON_DATA_REQ1_F101:		//F101	小时冻结B相电流
	case	CON_DATA_REQ1_F102:		//F102	小时冻结C相电流
	case	CON_DATA_REQ1_F103:		//F103	小时冻结零序电流
		stRetDIInfo->ucContentLen=ucNum1*3+2;
		break;
	case	CON_DATA_REQ1_F105:		//F105	小时冻结正向有功总电能量
	case	CON_DATA_REQ1_F106:		//F106	小时冻结正向无功总电能量
	case	CON_DATA_REQ1_F107:		//F107	小时冻结反向有功总电能量
	case	CON_DATA_REQ1_F108:		//F108	小时冻结反向无功总电能量
	case	CON_DATA_REQ1_F109:		//F109	小时冻结正向有功总电能示值
	case	CON_DATA_REQ1_F110:		//F110	小时冻结正向无功总电能示值
	case	CON_DATA_REQ1_F111:		//F111	小时冻结反向有功总电能示值
	case	CON_DATA_REQ1_F112:		//F112	小时冻结反向无功总电能示值
		stRetDIInfo->ucContentLen=ucNum1*4+2;
		break;
	case	CON_DATA_REQ1_F113:		//F113	小时冻结总功率因数
	case	CON_DATA_REQ1_F114:		//F114	小时冻结A相功率因数
	case	CON_DATA_REQ1_F115:		//F115	小时冻结B相功率因数
	case	CON_DATA_REQ1_F116:		//F116	小时冻结C相功率因数
		stRetDIInfo->ucContentLen=ucNum1*2+2;
		break;
//azh 170921
	case	CON_DATA_REQ1_F162:		//F162	电能表日历时钟
		stRetDIInfo->ucContentLen=12;
		break;
//	case	CON_DATA_REQ1_F121:		//F121	小时冻结直流模拟量
//		stRetDIInfo->ucContentLen=CON_DI_LEN_CHANGED;
//		break;
	case	CON_DATA_REQ1_F129:
	case	CON_DATA_REQ1_F131:
	case	CON_DATA_REQ1_F137:
	case	CON_DATA_REQ1_F139:
//azh 170921
	case	CON_DATA_REQ1_F177://当前组合有功电能示值（总、费率1～M）
	case	CON_DATA_REQ1_F178://上1结算日组合有功电能示值（总、费率1～M）    
		stRetDIInfo->ucContentLen=31;
		break;
	case	CON_DATA_REQ1_F130:
	case	CON_DATA_REQ1_F132:
	case	CON_DATA_REQ1_F133:
	case	CON_DATA_REQ1_F134:
	case	CON_DATA_REQ1_F135:
	case	CON_DATA_REQ1_F136:
	case	CON_DATA_REQ1_F138:
	case	CON_DATA_REQ1_F140:
	case	CON_DATA_REQ1_F141:
	case	CON_DATA_REQ1_F142:
	case	CON_DATA_REQ1_F143:
	case	CON_DATA_REQ1_F144:
		stRetDIInfo->ucContentLen=26;
		break;
	case	CON_DATA_REQ1_F145:
	case	CON_DATA_REQ1_F146:
	case	CON_DATA_REQ1_F147:
	case	CON_DATA_REQ1_F148:
	case	CON_DATA_REQ1_F149:
	case	CON_DATA_REQ1_F150:
	case	CON_DATA_REQ1_F151:
	case	CON_DATA_REQ1_F152:
		stRetDIInfo->ucContentLen=41;
		break;
	default:
		ucRet=RET_ERROR;
		ClearDIInfo( stRetDIInfo );
		break;
	}	
	return ucRet;
}

unsigned char  Main_DI_Req2_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,unsigned char * pucRecvBuffer)
{
	//STDIINFO stRetDIInfo;
	unsigned char	ucNum=0;
//	unsigned char	ucNum1=0;
	unsigned char	ucRet=RET_SUCCESS;
	
	ucNum = pucRecvBuffer[6];
	//ucNum1=(ucNum>>6)&0xf3;
	ClearDIInfo( stRetDIInfo );
	
	if(usDI < CON_DATA_REQ2_BEG_CODE || usDI > CON_DATA_REQ2_END_CODE)
	{
		ucRet=RET_ERROR;
		return ucRet;
	}
	
	SetDIInfo(stRetDIInfo,
		CON_DI_HAVE,									
		CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
		CON_DI_OPERATE_DISABLE,	CON_DI_SETPURVIEW_ADMIN,
		CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_DISABLE,	
		CON_DI_LEN_CHANGED,0,	CON_DI_TYPE_INVALID);
	
	switch(usDI)
	{
	case	CON_DATA_REQ2_F1:		//F1	日冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
	case	CON_DATA_REQ2_F2:		//F2	日冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
		stRetDIInfo->ucContentLen=(5+4+4+4)*(4+1)+9;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F3:		//F3	日冻结正向有/无功最大需量及发生时间（总、费率1~M）
	case	CON_DATA_REQ2_F4:		//F4	日冻结反向有/无功最大需量及发生时间（总、费率1~M）
		stRetDIInfo->ucContentLen=(3+4+3+4)*(4+1)+9;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F5:		//F5	日冻结正向有功电能量（总、费率1~M）
	case	CON_DATA_REQ2_F6:		//F6	日冻结正向无功电能量（总、费率1~M）
	case	CON_DATA_REQ2_F7:		//F7	日冻结反向有功电能量（总、费率1~M）
	case	CON_DATA_REQ2_F8:		//F8	日冻结反向无功电能量（总、费率1~M）
		stRetDIInfo->ucContentLen=4*(4+1)+3+1;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F9:		//F9	抄表日冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
	case	CON_DATA_REQ2_F10:		//F10	抄表日冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
		stRetDIInfo->ucContentLen=(5+4+4+4)*(4+1)+9;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F11:		//F11	抄表日冻结正向有/无功最大需量及发生时间（总、费率1~M）
	case	CON_DATA_REQ2_F12:		//F12	抄表日冻结反向有/无功最大需量及发生时间（总、费率1~M）
		stRetDIInfo->ucContentLen=(3+4+3+4)*(4+1)+9;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F17:		//F17	月冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
	case	CON_DATA_REQ2_F18:		//F18	月冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
		stRetDIInfo->ucContentLen=(5+4+4+4)*(4+1)+8;
		stRetDIInfo->ucCommandLen=2;
		break;									
	case	CON_DATA_REQ2_F19:		//F19	月冻结正向有/无功最大需量及发生时间（总、费率1~M）
	case	CON_DATA_REQ2_F20:		//F20	月冻结反向有/无功最大需量及发生时间（总、费率1~M）
		stRetDIInfo->ucContentLen=(3+4+3+4)*(4+1)+8;
		stRetDIInfo->ucCommandLen=2;
		break;									
	case	CON_DATA_REQ2_F21:		//F21	月冻结正向有功电能量（总、费率1~M）
	case	CON_DATA_REQ2_F22:		//F22	月冻结正向无功电能量（总、费率1~M）
	case	CON_DATA_REQ2_F23:		//F23	月冻结反向有功电能量（总、费率1~M）
	case	CON_DATA_REQ2_F24:		//F24	月冻结反向无功电能量（总、费率1~M）
		stRetDIInfo->ucContentLen=4*(4+1)+2+1;
		stRetDIInfo->ucCommandLen=2;
		break;									
	case	CON_DATA_REQ2_F25:		//F25	日分相最大有功功率及发生时间、有功功率为零时间
		stRetDIInfo->ucContentLen=35;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F26:		//F26	日总及分相最大需量及发生时间
		stRetDIInfo->ucContentLen=27;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F27:		//F27	日电压统计数据
		stRetDIInfo->ucContentLen=69;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F28:		//F28	日不平衡度越限累计时间
		stRetDIInfo->ucContentLen=7;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F29:		//F29	日电流越限统计
		stRetDIInfo->ucContentLen=37;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F30:		//F30	日视在功率越限累计时间
		stRetDIInfo->ucContentLen=7;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F32:		//F31	日电能表断相数据
		stRetDIInfo->ucContentLen=60;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F31:		//F32	备用
		stRetDIInfo->ucContentLen=2;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F33:		//F33	月分相最大有功功率及发生时间、分相有功功率为零时间
		stRetDIInfo->ucContentLen=34;
		stRetDIInfo->ucCommandLen=2;
		break;									
	case	CON_DATA_REQ2_F34:		//F34	月分相有功最大需量及发生时间、总有功最大需量及发生时间
		stRetDIInfo->ucContentLen=26;
		stRetDIInfo->ucCommandLen=2;
		break;									
	case	CON_DATA_REQ2_F35:		//F35	月电压统计数据
		stRetDIInfo->ucContentLen=68;
		stRetDIInfo->ucCommandLen=2;
		break;									
	case	CON_DATA_REQ2_F36:		//F36	月不平衡度越限累计时间
		stRetDIInfo->ucContentLen=6;
		stRetDIInfo->ucCommandLen=2;
		break;									
	case	CON_DATA_REQ2_F37:		//F37	月电流越限统计
		stRetDIInfo->ucContentLen=36;
		stRetDIInfo->ucCommandLen=2;
		break;									
	case	CON_DATA_REQ2_F38:		//F38	月视在功率越限累计时间
		stRetDIInfo->ucContentLen=6;
		stRetDIInfo->ucCommandLen=2;
		break;									
//	case	CON_DATA_REQ2_F41:		//F41	日冻结电容器投入累计时间和次数
//		stRetDIInfo->ucContentLen=75;
//		stRetDIInfo->ucCommandLen=3;
//		break;									
//	case	CON_DATA_REQ2_F42:		//F42	日、月电容器累计补偿的无功电能量
//		stRetDIInfo->ucContentLen=11;
//		stRetDIInfo->ucCommandLen=3;
//		break;									
	case	CON_DATA_REQ2_F43:		//F43	日功率因数区段累计时间
		stRetDIInfo->ucContentLen=9;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F44:		//F44	月功率因数区段累计时间
		stRetDIInfo->ucContentLen=8;
		stRetDIInfo->ucCommandLen=2;
		break;									
	case	CON_DATA_REQ2_F49:		//F49	终端日供电时间、日复位累计次数
		stRetDIInfo->ucContentLen=7;
		stRetDIInfo->ucCommandLen=3;
		break;									
//	case	CON_DATA_REQ2_F50:		//F50	终端日控制统计数据
//		stRetDIInfo->ucContentLen=7;
//		stRetDIInfo->ucCommandLen=3;
//		break;									
	case	CON_DATA_REQ2_F51:		//F51	终端月供电时间、月复位累计次数
		stRetDIInfo->ucContentLen=6;
		stRetDIInfo->ucCommandLen=2;
		break;									
//	case	CON_DATA_REQ2_F52:		//F52	终端月控制统计数据
//		stRetDIInfo->ucContentLen=6;
//		stRetDIInfo->ucCommandLen=2;
//		break;									
	case	CON_DATA_REQ2_F53:		//F53	终端与主站日通信流量
		stRetDIInfo->ucContentLen=7;
		stRetDIInfo->ucCommandLen=3;
		break;									
	case	CON_DATA_REQ2_F54:		//F54	终端与主站月通信流量
		stRetDIInfo->ucContentLen=6;
		stRetDIInfo->ucCommandLen=2;
		break;									
//	case	CON_DATA_REQ2_F57:		//F57	总加组日最大、最小有功功率及其发生时间，有功功率为零日累计时间
//		stRetDIInfo->ucContentLen=15;
//		stRetDIInfo->ucCommandLen=3;
//		break;									
//	case	CON_DATA_REQ2_F58:		//F58	总加组日累计有功电能量（总、费率1~M）
//	case	CON_DATA_REQ2_F59:		//F59	总加组日累计无功电能量（总、费率1~M）
//		stRetDIInfo->ucContentLen=4*(4+1)+4;
//		stRetDIInfo->ucCommandLen=3;
//		break;									
//	case	CON_DATA_REQ2_F60:		//F60	总加组月最大、最小有功功率及其发生时间，有功功率为零月累计时间
//		stRetDIInfo->ucContentLen=14;
//		stRetDIInfo->ucCommandLen=2;
//		break;									
//	case	CON_DATA_REQ2_F61:		//F61	总加组月累计有功电能量（总、费率1~M）
//	case	CON_DATA_REQ2_F62:		//F62	总加组月累计无功电能量（总、费率1~M）
//		stRetDIInfo->ucContentLen=4*(4+1)+3;
//		stRetDIInfo->ucCommandLen=2;
//		break;									
//	case	CON_DATA_REQ2_F65:		//F65	总加组超功率定值的月累计时间、月累计电能量
//	case	CON_DATA_REQ2_F66:		//F66	总加组超月电能量定值的月累计时间、累计电能量
//		stRetDIInfo->ucContentLen=8;
//		stRetDIInfo->ucCommandLen=2;
//		break;									
//	case	CON_DATA_REQ2_F73:		//F73	总加组有功功率曲线
//	case	CON_DATA_REQ2_F74:		//F74	总加组无功功率曲线
//		stRetDIInfo->ucCommandLen=7;
//		stRetDIInfo->ucContentLen=ucNum*2+7;
//		break;							
//	case	CON_DATA_REQ2_F75:		//F75	总加组有功电能量曲线
//	case	CON_DATA_REQ2_F76:		//F76	总加组无功电能量曲线
//		stRetDIInfo->ucCommandLen=7;
//		stRetDIInfo->ucContentLen=ucNum*4+7;
//		break;							
	case	CON_DATA_REQ2_F81:		//F81	有功功率曲线
	case	CON_DATA_REQ2_F82:		//F82	A相有功功率曲线
	case	CON_DATA_REQ2_F83:		//F83	B相有功功率曲线
	case	CON_DATA_REQ2_F84:		//F84	C相有功功率曲线
	case	CON_DATA_REQ2_F85:		//F85	无功功率曲线
	case	CON_DATA_REQ2_F86:		//F86	A相无功功率曲线
	case	CON_DATA_REQ2_F87:		//F87	B相无功功率曲线
	case	CON_DATA_REQ2_F88:		//F88	C相无功功率曲线
		stRetDIInfo->ucCommandLen=7;
		stRetDIInfo->ucContentLen=ucNum*3+7;
		break;							
	case	CON_DATA_REQ2_F89:		//F89	A相电压曲线
	case	CON_DATA_REQ2_F90:		//F90	B相电压曲线
	case	CON_DATA_REQ2_F91:		//F91	C相电压曲线
	case	CON_DATA_REQ2_F92:		//F92	A相电流曲线
	case	CON_DATA_REQ2_F93:		//F93	B相电流曲线
	case	CON_DATA_REQ2_F94:		//F94	C相电流曲线
	case	CON_DATA_REQ2_F95:		//F95	零序电流曲线
		stRetDIInfo->ucCommandLen=7;
		stRetDIInfo->ucContentLen=ucNum*2+7;
		break;							
	case	CON_DATA_REQ2_F97:		//F97	正向有功总电能量
	case	CON_DATA_REQ2_F98:		//F98	正向无功总电能量
	case	CON_DATA_REQ2_F99:		//F99	反向有功总电能量
	case	CON_DATA_REQ2_F100:		//F100	反向无功总电能量
	case	CON_DATA_REQ2_F101:		//F101	正向有功总电能示值
	case	CON_DATA_REQ2_F102:		//F102	正向无功总电能示值
	case	CON_DATA_REQ2_F103:		//F103	反向有功总电能示值
	case	CON_DATA_REQ2_F104:		//F104	反向无功总电能示值
		stRetDIInfo->ucCommandLen=7;
		stRetDIInfo->ucContentLen=ucNum*4+7;
		break;							
	case	CON_DATA_REQ2_F105:		//F105	总功率因数
	case	CON_DATA_REQ2_F106:		//F106	A相功率因数
	case	CON_DATA_REQ2_F107:		//F107	B相功率因数
	case	CON_DATA_REQ2_F108:		//F108	C相功率因数
		stRetDIInfo->ucCommandLen=7;
		stRetDIInfo->ucContentLen=ucNum*2+7;
		break;		
	case	CON_DATA_REQ2_F109:	
	case	CON_DATA_REQ2_F110:	
		stRetDIInfo->ucCommandLen=7;
		stRetDIInfo->ucContentLen=ucNum*6+7;
		break;	
//	case	CON_DATA_REQ2_F113:		//F113	A相2~19次谐波电流日最大值及发生时间
//	case	CON_DATA_REQ2_F114:		//F114	B相2~19次谐波电流日最大值及发生时间
//	case	CON_DATA_REQ2_F115:		//F115	C相2~19次谐波电流日最大值及发生时间
//		stRetDIInfo->ucContentLen=117;
//		stRetDIInfo->ucCommandLen=3;
//		break;									
//	case	CON_DATA_REQ2_F116:		//F116	A相2~19次谐波电压含有率及总畸变率日最大值及发生时间
//	case	CON_DATA_REQ2_F117:		//F117	B相2~19次谐波电压含有率及总畸变率日最大值及发生时间
//	case	CON_DATA_REQ2_F118:		//F118	C相2~19次谐波电压含有率及总畸变率日最大值及发生时间
//		stRetDIInfo->ucContentLen=117;
//		stRetDIInfo->ucCommandLen=3;
//		break;									
//	case	CON_DATA_REQ2_F121:		//F121	A相谐波越限日统计数据
//	case	CON_DATA_REQ2_F122:		//F122	B相谐波越限日统计数据
//	case	CON_DATA_REQ2_F123:		//F123	C相谐波越限日统计数据
//		stRetDIInfo->ucContentLen=80;
//		stRetDIInfo->ucCommandLen=3;
//		break;									
//	case	CON_DATA_REQ2_F129:		//F129	直流模拟量越限日累计时间、最大/最小值及发生时间
//		stRetDIInfo->ucContentLen=17;
//		stRetDIInfo->ucCommandLen=3;
//		break;									
//	case	CON_DATA_REQ2_F130:		//F130	直流模拟量越限月累计时间、最大/最小值及发生时间
//		stRetDIInfo->ucContentLen=16;
//		stRetDIInfo->ucCommandLen=2;
//		break;									
//	case	CON_DATA_REQ2_F138:		//F138	直流模拟量数据曲线
//		stRetDIInfo->ucCommandLen=7;
//		break;							
	case	CON_DATA_REQ2_F145:		//F145	一象限无功总电能示值曲线
	case	CON_DATA_REQ2_F146:		//F146	四象限无功总电能示值曲线
	case	CON_DATA_REQ2_F147:		//F147	二象限无功总电能示值曲线
	case	CON_DATA_REQ2_F148:		//F148	三象限无功总电能示值曲线
		stRetDIInfo->ucCommandLen=7;
		stRetDIInfo->ucContentLen=ucNum*4+7;
		break;	
	case 	CON_DATA_REQ2_F153:
	case 	CON_DATA_REQ2_F155:
		stRetDIInfo->ucCommandLen=3;
		stRetDIInfo->ucContentLen=23;
		break;
	case 	CON_DATA_REQ2_F157:
	case 	CON_DATA_REQ2_F159:
		stRetDIInfo->ucCommandLen=2;
		stRetDIInfo->ucContentLen=22;
		break;
		
	case 	CON_DATA_REQ2_F154:
	case 	CON_DATA_REQ2_F156:
		stRetDIInfo->ucCommandLen=3;
		stRetDIInfo->ucContentLen=20;
		break;
	case 	CON_DATA_REQ2_F158:
	case 	CON_DATA_REQ2_F160:
		stRetDIInfo->ucCommandLen=2;
		stRetDIInfo->ucContentLen=19;
		break;
		
	case	CON_DATA_REQ2_F161:		
	case	CON_DATA_REQ2_F163:	
	case	CON_DATA_REQ2_F169:		
	case	CON_DATA_REQ2_F171:
		stRetDIInfo->ucCommandLen=3;
		stRetDIInfo->ucContentLen=34;
		break;
	case	CON_DATA_REQ2_F162:		
	case	CON_DATA_REQ2_F164:	
	case	CON_DATA_REQ2_F165:		
	case	CON_DATA_REQ2_F166:	
	case	CON_DATA_REQ2_F167:		
	case	CON_DATA_REQ2_F168:	
	case	CON_DATA_REQ2_F170:		
	case	CON_DATA_REQ2_F172:	
	case	CON_DATA_REQ2_F173:		
	case	CON_DATA_REQ2_F174:	
	case	CON_DATA_REQ2_F175:		
	case	CON_DATA_REQ2_F176:	
		stRetDIInfo->ucCommandLen=3;
		stRetDIInfo->ucContentLen=29;
		break;
	case	CON_DATA_REQ2_F177:		
	case	CON_DATA_REQ2_F179:	
		stRetDIInfo->ucCommandLen=2;
		stRetDIInfo->ucContentLen=34;
		break;
	case	CON_DATA_REQ2_F178:		
	case	CON_DATA_REQ2_F180:	
	case	CON_DATA_REQ2_F181:		
	case	CON_DATA_REQ2_F182:	
	case	CON_DATA_REQ2_F183:		
	case	CON_DATA_REQ2_F184:	
		stRetDIInfo->ucCommandLen=2;
		stRetDIInfo->ucContentLen=29;
		break;
	case	CON_DATA_REQ2_F185:		
	case	CON_DATA_REQ2_F186:	
	case	CON_DATA_REQ2_F187:		
	case	CON_DATA_REQ2_F188:	
	case	CON_DATA_REQ2_F189:		
	case	CON_DATA_REQ2_F190:	
	case	CON_DATA_REQ2_F191:	
	case	CON_DATA_REQ2_F192:	
		stRetDIInfo->ucCommandLen=3;
		stRetDIInfo->ucContentLen=44;
		break;
	case	CON_DATA_REQ2_F193:		
	case	CON_DATA_REQ2_F194:	
	case	CON_DATA_REQ2_F195:	
	case	CON_DATA_REQ2_F196:	
		stRetDIInfo->ucCommandLen=2;
		stRetDIInfo->ucContentLen=43;
		break;
//azh 170925
	case	CON_DATA_REQ2_F219:		//F219	测量点组合数据曲线数据单元格式
		stRetDIInfo->ucCommandLen=7;
		stRetDIInfo->ucContentLen=ucNum*33+7;
		break;							
	default:
		ClearDIInfo( stRetDIInfo );
		ucRet=RET_ERROR;
		break;
	}
	return ucRet;
}

unsigned char  Main_DI_Req3_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI)
{
	unsigned char	ucRet=RET_ERROR;
	
	ClearDIInfo( stRetDIInfo );
	
	if(usDI < CON_DATA_REQ3_BEG_CODE || usDI > CON_DATA_REQ3_END_CODE)
		return ucRet;
	
	SetDIInfo(stRetDIInfo,
		CON_DI_HAVE,									
		CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
		CON_DI_OPERATE_DISABLE,	CON_DI_SETPURVIEW_ADMIN,
		CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_DISABLE,	
		CON_DI_LEN_CHANGED,2,	CON_DI_TYPE_INVALID);
	
	switch(usDI)
	{
	case	CON_DATA_REQ3_F1:	//F1	重要事件	
	case	CON_DATA_REQ3_F2:	//F2	一般事件	
       	ucRet=RET_SUCCESS;
		break;
	default:
		ClearDIInfo( stRetDIInfo );
	}
	return ucRet;
}

/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : DI_List.h
 Description    : 数据项标识
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/

#ifndef _DI_LIST_H_
#define _DI_LIST_H_
//#include "public.h"

#define	CONTERMSGNUM					8		//8个信息点
#define	CONTERMSGRETNUM					8		//8个处理返回信息点

#define	CON_TERMSG_SERVER_SENDACK		1		//发送/确认
#define	CON_TERMSG_SERVER_SENDNACK		4		//发送/不回答

#define	CON_TERMSG_TYPE_INVALID			0xFF
//主站下行报文
#define	CON_TERMSG_ACK				0x00		//确认
#define	CON_TERMSG_RESET				0x01		//复位命令接收
#define	CON_TERMSG_CONNECT			0x02		//链路接口检测
#define	CON_TERMSG_RELAYCMD			0x03		//中继站命令
#define	CON_TERMSG_SETPARA			0x04		//参数设置
#define	CON_TERMSG_CONTROL			0x05		//控制命令
#define   CON_TERMSG_LOGIN				0x06		//身份认证及密钥协商
#define 	CON_TERMSG_DATAREQ_SLAVE	0x08		//请求被级联终端主动上报
#define   CON_TERMSG_CONFIGURATION	0x09		//终端配置
#define	CON_TERMSG_GETPARA			0x0A		//查询终端参数
#define	CON_TERMSG_TASKDATA			0x0B		//任务数据查询
#define	CON_TERMSG_REQDATA_1		0x0C		//请求1类数据（实时数据）
#define	CON_TERMSG_REQDATA_2		0x0D		//请求2类数据（冻结数据）
#define	CON_TERMSG_REQDATA_3		0x0E		//请求3类数据（事件）
#define	CON_TERMSG_FILETRANS		0x0F		//文件传输
#define	CON_TERMSG_DATATRANS		0x10		//数据转发
#define	CON_TERMSG_EXTCMD			0xC0		//扩展命令（用户自定义）

//信息数据项
#define	CON_INFO_DI_TYPE_TIME			0x0101		//此数据点时间
#define	CON_INFO_DI_TYPE_DINUM			0x0102		//数据点数
#define	CON_INFO_DI_TYPE_DIINTER		0x0103		//数据时间间隔
#define	CON_INFO_DI_TYPE_ALRCODE		0x0104		//告警编码

//确认结果
#define	CON_NACK_ERR_NORMAL				0		//正确，无错误
#define	CON_NACK_ERR_NOTEXE				1		//数据出错，不执行
#define	CON_NACK_ERR_PASSWORD			2		//口令错
#define	CON_NACK_ERR_NOVALIDDATA		3		//无有效数据
#define CON_ACK_NEEDMETERACK			0x10	//需要读取表计数据后回应


/*******************************数据项标识****************************************/
#define CON_DI_TYPE_INVALID								0xBBBB//无效数据标识
#define CON_DI_TYPE_MPRELAY								0x0000//测量点中继命令
#define CON_DI_TYPE_EXT									0xCDCD//扩展命令数据标识
#define	CON_DI_TYPE_MPALLDI								0xFFFF//所有

///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPDIRPOWTOTAL						0x9010//正向有功总电能
#define CON_DI_TYPE_MPDIRPOWPLUS						0x9011//正向有功尖电能
#define CON_DI_TYPE_MPDIRPOWAPEX						0x9012//正向有功峰电能
#define CON_DI_TYPE_MPDIRPOWFLAT						0x9013//正向有功平电能
#define CON_DI_TYPE_MPDIRPOWVALE						0x9014//正向有功谷电能
#define CON_DI_TYPE_MPDIRPOW_BLOCK						0x901F//正向有功电能数据块

#define CON_DI_TYPE_MPUNDIRPOWTOTAL						0x9020//反向有功总电能
#define CON_DI_TYPE_MPUNDIRPOWPLUS						0x9021//反向有功尖电能
#define CON_DI_TYPE_MPUNDIRPOWAPEX						0x9022//反向有功峰电能
#define CON_DI_TYPE_MPUNDIRPOWFLAT						0x9023//反向有功平电能
#define CON_DI_TYPE_MPUNDIRPOWVALE						0x9024//反向有功谷电能
#define CON_DI_TYPE_MPUNDIRPOW_BLOCK 					0x902F//反向有功电能数据块
///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPDIRPOWTOTAL_LM					0x9410//正向有功总电能（上月）
#define CON_DI_TYPE_MPDIRPOWPLUS_LM						0x9411//正向有功尖电能（上月）
#define CON_DI_TYPE_MPDIRPOWAPEX_LM						0x9412//正向有功峰电能（上月）
#define CON_DI_TYPE_MPDIRPOWFLAT_LM						0x9413//正向有功平电能（上月）
#define CON_DI_TYPE_MPDIRPOWVALE_LM						0x9414//正向有功谷电能（上月）
#define CON_DI_TYPE_MPDIRPOW_LM_BLOCK					0x941F//正向有功电能数据块（上月）

#define CON_DI_TYPE_MPUNDIRPOWTOTAL_LM					0x9420//反向有功总电能（上月）
#define CON_DI_TYPE_MPUNDIRPOWPLUS_LM					0x9421//反向有功尖电能（上月）
#define CON_DI_TYPE_MPUNDIRPOWAPEX_LM					0x9422//反向有功峰电能（上月）
#define CON_DI_TYPE_MPUNDIRPOWFLAT_LM					0x9423//反向有功平电能（上月）
#define CON_DI_TYPE_MPUNDIRPOWVALE_LM					0x9424//反向有功谷电能（上月）
#define CON_DI_TYPE_MPUNDIRPOW_LM_BLOCK 				0x942F//反向有功电能数据块（上月）
///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPDIRVARTOTAL						0x9110//正向无功总电能
#define CON_DI_TYPE_MPDIRVARPLUS						0x9111//正向无功尖电能
#define CON_DI_TYPE_MPDIRVARAPEX						0x9112//正向无功峰电能
#define CON_DI_TYPE_MPDIRVARFLAT						0x9113//正向无功平电能
#define CON_DI_TYPE_MPDIRVARVALE						0x9114//正向无功谷电能
#define CON_DI_TYPE_MPDIRVAR_BLOCK						0x911F//正向无功电能数据块

#define CON_DI_TYPE_MPUNDIRVARTOTAL						0x9120//反向无功总电能
#define CON_DI_TYPE_MPUNDIRVARPLUS						0x9121//反向无功尖电能
#define CON_DI_TYPE_MPUNDIRVARAPEX						0x9122//反向无功峰电能
#define CON_DI_TYPE_MPUNDIRVARFLAT						0x9123//反向无功平电能
#define CON_DI_TYPE_MPUNDIRVARVALE						0x9124//反向无功谷电能
#define CON_DI_TYPE_MPUNDIRVAR_BLOCK					0x912F//反向无功电能数据块
///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPDIRVARTOTAL_LM					0x9510//正向无功总电能（上月）
#define CON_DI_TYPE_MPDIRVARPLUS_LM						0x9511//正向无功尖电能（上月）
#define CON_DI_TYPE_MPDIRVARAPEX_LM						0x9512//正向无功峰电能（上月）
#define CON_DI_TYPE_MPDIRVARFLAT_LM						0x9513//正向无功平电能（上月）
#define CON_DI_TYPE_MPDIRVARVALE_LM						0x9514//正向无功谷电能（上月）
#define CON_DI_TYPE_MPDIRVAR_LM_BLOCK					0x951F//正向无功电能数据块（上月）

#define CON_DI_TYPE_MPUNDIRVARTOTAL_LM					0x9520//反向无功总电能（上月）
#define CON_DI_TYPE_MPUNDIRVARPLUS_LM					0x9521//反向无功尖电能（上月）
#define CON_DI_TYPE_MPUNDIRVARAPEX_LM					0x9522//反向无功峰电能（上月）
#define CON_DI_TYPE_MPUNDIRVARFLAT_LM					0x9523//反向无功平电能（上月）
#define CON_DI_TYPE_MPUNDIRVARVALE_LM					0x9524//反向无功谷电能（上月）
#define CON_DI_TYPE_MPUNDIRVAR_LM_BLOCK					0x952F//反向无功电能数据块（上月）
/////////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPVAR1								0x9130//一象限无功总电能
#define CON_DI_TYPE_MPVAR1_FEE1							0x9131//费率1
#define CON_DI_TYPE_MPVAR1_FEE2							0x9132//费率2
#define CON_DI_TYPE_MPVAR1_FEE3							0x9133//费率3
#define CON_DI_TYPE_MPVAR1_FEE4							0x9134//费率4
#define CON_DI_TYPE_MPVAR1_BLOCK						0x913F//一象限无功电能数据块

#define CON_DI_TYPE_MPVAR4								0x9140//四象限无功总电能
#define CON_DI_TYPE_MPVAR4_FEE1							0x9141//费率1
#define CON_DI_TYPE_MPVAR4_FEE2							0x9142//费率2
#define CON_DI_TYPE_MPVAR4_FEE3							0x9143//费率3
#define CON_DI_TYPE_MPVAR4_FEE4							0x9144//费率4
#define CON_DI_TYPE_MPVAR4_BLOCK						0x914F//一象限无功电能数据块

#define CON_DI_TYPE_MPVAR2								0x9150//二象限无功总电能
#define CON_DI_TYPE_MPVAR2_FEE1							0x9151//费率1
#define CON_DI_TYPE_MPVAR2_FEE2							0x9152//费率2
#define CON_DI_TYPE_MPVAR2_FEE3							0x9153//费率3
#define CON_DI_TYPE_MPVAR2_FEE4							0x9154//费率4
#define CON_DI_TYPE_MPVAR2_BLOCK						0x915F//二象限无功电能数据块

#define CON_DI_TYPE_MPVAR3								0x9160//三象限无功总电能
#define CON_DI_TYPE_MPVAR3_FEE1							0x9161//费率1
#define CON_DI_TYPE_MPVAR3_FEE2							0x9162//费率2
#define CON_DI_TYPE_MPVAR3_FEE3							0x9163//费率3
#define CON_DI_TYPE_MPVAR3_FEE4							0x9164//费率4
#define CON_DI_TYPE_MPVAR3_BLOCK						0x916F//三象限无功电能数据块
///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPVAR1_LM							0x9530//一象限无功总电能（上月）
#define CON_DI_TYPE_MPVAR1_LM_FEE1						0x9531//费率1
#define CON_DI_TYPE_MPVAR1_LM_FEE2						0x9532//费率2
#define CON_DI_TYPE_MPVAR1_LM_FEE3						0x9533//费率3
#define CON_DI_TYPE_MPVAR1_LM_FEE4						0x9534//费率4
#define CON_DI_TYPE_MPVAR1_LM_BLOCK						0x953F

#define CON_DI_TYPE_MPVAR4_LM							0x9540//四象限无功总电能（上月）
#define CON_DI_TYPE_MPVAR4_LM_FEE1						0x9541//费率1
#define CON_DI_TYPE_MPVAR4_LM_FEE2						0x9542//费率2
#define CON_DI_TYPE_MPVAR4_LM_FEE3						0x9543//费率3
#define CON_DI_TYPE_MPVAR4_LM_FEE4						0x9544//费率4
#define CON_DI_TYPE_MPVAR4_LM_BLOCK						0x954F

#define CON_DI_TYPE_MPVAR2_LM							0x9550//二象限无功总电能（上月）
#define CON_DI_TYPE_MPVAR2_LM_FEE1						0x9551//费率1
#define CON_DI_TYPE_MPVAR2_LM_FEE2						0x9552//费率2
#define CON_DI_TYPE_MPVAR2_LM_FEE3						0x9553//费率3
#define CON_DI_TYPE_MPVAR2_LM_FEE4						0x9554//费率4
#define CON_DI_TYPE_MPVAR2_LM_BLOCK						0x955F

#define CON_DI_TYPE_MPVAR3_LM							0x9560//三象限无功总电能（上月）
#define CON_DI_TYPE_MPVAR3_LM_FEE1						0x9561//费率1
#define CON_DI_TYPE_MPVAR3_LM_FEE2						0x9562//费率2
#define CON_DI_TYPE_MPVAR3_LM_FEE3						0x9563//费率3
#define CON_DI_TYPE_MPVAR3_LM_FEE4						0x9564//费率4
#define CON_DI_TYPE_MPVAR3_LM_BLOCK						0x956F
///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPDIRPOWMAXDEMAND					0xA010//正向有功总最大需量
#define CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE1				0xA011//费率1
#define CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE2				0xA012//费率2
#define CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE3				0xA013//费率3
#define CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE4				0xA014//费率4
#define CON_DI_TYPE_MPDIRPOWMAXDEMAND_BLOCK				0xA01F

#define CON_DI_TYPE_MPUNDIRPOWMAXDEMAND					0xA020//反向有功总最大需量
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE1			0xA021//费率1
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE2			0xA022//费率2
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE3			0xA023//费率3
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE4			0xA024//费率4
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_BLOCK			0xA02F

#define CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME				0xB010//正向有功总最大需量发生时间
#define CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE1			0xB011//费率1
#define CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE2			0xB012//费率2
#define CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE3			0xB013//费率3
#define CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE4			0xB014//费率4
#define CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_BLOCK			0xB01F

#define CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME				0xB020//反向有功总最大需量发生时间
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE1		0xB021//费率1
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE2		0xB022//费率2
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE3		0xB023//费率3
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE4		0xB024//费率4
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_BLOCK		0xB02F

#define CON_DI_TYPE_MPDIRVARMAXDEMAND					0xA110//正向无功总最大需量
#define CON_DI_TYPE_MPDIRVARMAXDEMAND_FEE1				0xA111//费率1
#define CON_DI_TYPE_MPDIRVARMAXDEMAND_FEE2				0xA112//费率2
#define CON_DI_TYPE_MPDIRVARMAXDEMAND_FEE3				0xA113//费率3
#define CON_DI_TYPE_MPDIRVARMAXDEMAND_FEE4				0xA114//费率4
#define CON_DI_TYPE_MPDIRVARMAXDEMAND_BLOCK				0xA11F

#define CON_DI_TYPE_MPUNDIRVARMAXDEMAND					0xA120//反向无功总最大需量
#define CON_DI_TYPE_MPUNDIRVARMAXDEMAND_FEE1			0xA121//费率1
#define CON_DI_TYPE_MPUNDIRVARMAXDEMAND_FEE2			0xA122//费率2
#define CON_DI_TYPE_MPUNDIRVARMAXDEMAND_FEE3			0xA123//费率3
#define CON_DI_TYPE_MPUNDIRVARMAXDEMAND_FEE4			0xA124//费率4
#define CON_DI_TYPE_MPUNDIRVARMAXDEMAND_BLOCK			0xA12F

#define CON_DI_TYPE_MPDIRVARMAXDEMANDTIME				0xB110//正向无功总最大需量发生时间
#define CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_FEE1			0xB111//费率1
#define CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_FEE2			0xB112//费率2
#define CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_FEE3			0xB113//费率3
#define CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_FEE4			0xB114//费率4
#define CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_BLOCK			0xB11F

#define CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME				0xB120//反向无功总最大需量发生时间
#define CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_FEE1		0xB121//费率1
#define CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_FEE2		0xB122//费率2
#define CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_FEE3		0xB123//费率3
#define CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_FEE4		0xB124//费率4
#define CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_BLOCK		0xB12F

///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM				0xA410//正向有功总最大需量（上月）
#define CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE1			0xA411//费率1
#define CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE2			0xA412//费率2
#define CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE3			0xA413//费率3
#define CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE4			0xA414//费率4
#define CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_BLOCK			0xA41F
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM				0xA420//反向有功总最大需量（上月）
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE1			0xA421//费率1
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE2			0xA422//费率2
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE3			0xA423//费率3
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE4			0xA424//费率4
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_BLOCK		0xA42F

#define CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM			0xB410//正向有功总最大需量发生时间（上月）
#define CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE1		0xB411//费率1
#define CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE2		0xB412//费率2
#define CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE3		0xB413//费率3
#define CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE4		0xB414//费率4
#define CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_BLOCK		0xB41F
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM			0xB420//反向有功总最大需量发生时间（上月）
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE1		0xB421//费率1
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE2		0xB422//费率2
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE3		0xB423//费率3
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE4		0xB424//费率4
#define CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_BLOCK	0xB42F

#define CON_DI_TYPE_MPDIRVARMAXDEMAND_LM				0xA510//正向无功总最大需量（上月）
#define CON_DI_TYPE_MPDIRVARMAXDEMAND_LM_FEE1			0xA511//费率1
#define CON_DI_TYPE_MPDIRVARMAXDEMAND_LM_FEE2			0xA512//费率2
#define CON_DI_TYPE_MPDIRVARMAXDEMAND_LM_FEE3			0xA513//费率3
#define CON_DI_TYPE_MPDIRVARMAXDEMAND_LM_FEE4			0xA514//费率4
#define CON_DI_TYPE_MPDIRVARMAXDEMAND_LM_BLOCK			0xA51F
#define CON_DI_TYPE_MPUNDIRVARMAXDEMAND_LM				0xA520//反向无功总最大需量（上月）
#define CON_DI_TYPE_MPUNDIRVARMAXDEMAND_LM_FEE1			0xA521//费率1
#define CON_DI_TYPE_MPUNDIRVARMAXDEMAND_LM_FEE2			0xA522//费率2
#define CON_DI_TYPE_MPUNDIRVARMAXDEMAND_LM_FEE3			0xA523//费率3
#define CON_DI_TYPE_MPUNDIRVARMAXDEMAND_LM_FEE4			0xA524//费率4
#define CON_DI_TYPE_MPUNDIRVARMAXDEMAND_LM_BLOCK		0xA52F

#define CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_LM			0xB510//正向无功总最大需量发生时间（上月）
#define CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_LM_FEE1		0xB511//费率1
#define CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_LM_FEE2		0xB512//费率2
#define CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_LM_FEE3		0xB513//费率3
#define CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_LM_FEE4		0xB514//费率4
#define CON_DI_TYPE_MPDIRVARMAXDEMANDTIME_LM_BLOCK		0xB51F
#define CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_LM			0xB520//反向无功总最大需量发生时间（上月）
#define CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_LM_FEE1		0xB521//费率1
#define CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_LM_FEE2		0xB522//费率2
#define CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_LM_FEE3		0xB523//费率3
#define CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_LM_FEE4		0xB524//费率4
#define CON_DI_TYPE_MPUNDIRVARMAXDEMANDTIME_LM_BLOCK	0xB52F

///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPVOLA								0xB611//A相电压
#define CON_DI_TYPE_MPVOLB								0xB612//B相电压
#define CON_DI_TYPE_MPVOLC								0xB613//C相电压
#define CON_DI_TYPE_MPVOL_BLOCK							0xB61F//电压数据块

#define CON_DI_TYPE_MPCURA								0xB621//A相电流
#define CON_DI_TYPE_MPCURB								0xB622//B相电流
#define CON_DI_TYPE_MPCURC								0xB623//C相电流 
#define CON_DI_TYPE_MPCUR_BLOCK							0xB62F//电流数据块

#define CON_DI_TYPE_MPINSTANTPOW						0xB630//瞬时有功功率
#define CON_DI_TYPE_MPINSTANTPOWA						0xB631//A相有功功率
#define CON_DI_TYPE_MPINSTANTPOWB						0xB632//B相有功功率
#define CON_DI_TYPE_MPINSTANTPOWC						0xB633//C相有功功率
#define CON_DI_TYPE_MPINSTANTPOW_BLOCK					0xB63F//

#define CON_DI_TYPE_MPINSTANTVAR						0xB640//瞬时无功功率
#define CON_DI_TYPE_MPINSTANTVARA						0xB641//A相无功功率
#define CON_DI_TYPE_MPINSTANTVARB						0xB642//B相无功功率
#define CON_DI_TYPE_MPINSTANTVARC						0xB643//C相无功功率
#define CON_DI_TYPE_MPINSTANTVAR_BLOCK					0xB64F//

#define	CON_DI_TYPE_MPPOWRATE							0xB650//总功率因素
#define	CON_DI_TYPE_MPPOWRATEA							0xB651//A相功率因素
#define	CON_DI_TYPE_MPPOWRATEB							0xB652//B相功率因素
#define	CON_DI_TYPE_MPPOWRATEC							0xB653//C相功率因素
#define	CON_DI_TYPE_MPPOWRATE_BLOCK						0xB65F//
///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_SUMINSTANTPOW1						0x8F50//有功功率总加组1
#define CON_DI_TYPE_SUMINSTANTPOW2						0x8F51//有功功率总加组2
#define CON_DI_TYPE_SUMINSTANTPOW3						0x8F52//有功功率总加组3
#define CON_DI_TYPE_SUMINSTANTPOW4						0x8F53//有功功率总加组4
#define CON_DI_TYPE_SUMINSTANTPOW5						0x8F54//有功功率总加组5
#define CON_DI_TYPE_SUMINSTANTPOW6						0x8F55//有功功率总加组6
#define CON_DI_TYPE_SUMINSTANTPOW7						0x8F56//有功功率总加组7
#define CON_DI_TYPE_SUMINSTANTPOW8						0x8F57//有功功率总加组8

#define CON_DI_TYPE_SUMINSTANTVAR1						0x8F60//无功功率总加组1
#define CON_DI_TYPE_SUMINSTANTVAR2						0x8F61//无功功率总加组2
#define CON_DI_TYPE_SUMINSTANTVAR3						0x8F62//无功功率总加组3
#define CON_DI_TYPE_SUMINSTANTVAR4						0x8F63//无功功率总加组4
#define CON_DI_TYPE_SUMINSTANTVAR5						0x8F64//无功功率总加组5
#define CON_DI_TYPE_SUMINSTANTVAR6						0x8F65//无功功率总加组6
#define CON_DI_TYPE_SUMINSTANTVAR7						0x8F66//无功功率总加组7
#define CON_DI_TYPE_SUMINSTANTVAR8						0x8F67//无功功率总加组8

#define CON_DI_TYPE_SUMPOW1								0x8F70//有功总加组1
#define CON_DI_TYPE_SUMPOW2								0x8F71//有功总加组2
#define CON_DI_TYPE_SUMPOW3								0x8F72//有功总加组3
#define CON_DI_TYPE_SUMPOW4								0x8F73//有功总加组4
#define CON_DI_TYPE_SUMPOW5								0x8F74//有功总加组5
#define CON_DI_TYPE_SUMPOW6								0x8F75//有功总加组6
#define CON_DI_TYPE_SUMPOW7								0x8F76//有功总加组7
#define CON_DI_TYPE_SUMPOW8								0x8F77//有功总加组8

#define CON_DI_TYPE_SUMVAR1								0x8F80//无功总加组1
#define CON_DI_TYPE_SUMVAR2								0x8F81//无功总加组2
#define CON_DI_TYPE_SUMVAR3								0x8F82//无功总加组3
#define CON_DI_TYPE_SUMVAR4								0x8F83//无功总加组4
#define CON_DI_TYPE_SUMVAR5								0x8F84//无功总加组5
#define CON_DI_TYPE_SUMVAR6								0x8F85//无功总加组6
#define CON_DI_TYPE_SUMVAR7								0x8F86//无功总加组7
#define CON_DI_TYPE_SUMVAR8								0x8F87//无功总加组8

///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPPOWCONST							0xC030//有功常数
#define CON_DI_TYPE_MPVARCONST							0xC031//无功常数
#define CON_DI_TYPE_MPCONST_BLOCK						0xC03F//

#define CON_DI_TYPE_MPFEERATE1							0xC331//费率1
#define CON_DI_TYPE_MPFEERATE2							0xC332//费率2
#define CON_DI_TYPE_MPFEERATE3							0xC333//费率3
#define CON_DI_TYPE_MPFEERATE4							0xC334//费率4
#define CON_DI_TYPE_MPFEERATE5							0xC335//费率5
#define CON_DI_TYPE_MPFEERATE6							0xC336//费率6
#define CON_DI_TYPE_MPFEERATE7							0xC337//费率7
#define CON_DI_TYPE_MPFEERATE8							0xC338//费率8
#define CON_DI_TYPE_MPFEERATE_BLOCK						0xC33F	
///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPDATEANDWEEK						0xC010//日期及周次
#define CON_DI_TYPE_MPTIME								0xC011//时间
#define CON_DI_TYPE_MPDATETIME_BLOCK					0xC01F//日期时间数据块
///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPMETERSTATFLAG						0xC020//电表运行状态字
#define CON_DI_TYPE_MPNETSTATFLAG						0xC021//电网运行状态字
#define CON_DI_TYPE_MPCO2F_BLOCK						0xC02F//状态字数据块
///////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPCOPYDAYTIME						0xC117//抄表日
#define CON_DI_TYPE_MPC11F_BLOCK						0xC11F
//////////////////////////////////////////////////////////////////////////////////////////////
#define CON_DI_TYPE_MPLATELYPROGTIME					0xB210//最近一次编程时间
#define CON_DI_TYPE_MPLATELYMAXDEMANDCLEARTIME			0xB211//最近一次最大需量清零时间
#define CON_DI_TYPE_MPPROGTIMETIMES						0xB212//编程次数
#define CON_DI_TYPE_MPMAXDEMANDCLEARTIMES				0xB213//最大需量清零次数
#define CON_DI_TYPE_MPBATWORKTIME						0xB214//电池工作时间
#define CON_DI_TYPE_MPB21F_BLOCK						0xB21F

#define CON_DI_TYPE_MPLOSTVOLNUM						0xB310
#define CON_DI_TYPE_MPLOSTVOLANUM						0xB311
#define CON_DI_TYPE_MPLOSTVOLBNUM						0xB312
#define CON_DI_TYPE_MPLOSTVOLCNUM						0xB313
#define CON_DI_TYPE_MPB31F_BLOCK						0xB31F		//断相次数  用大项

#define CON_DI_TYPE_MPLOSTVOLTIME						0xB320
#define CON_DI_TYPE_MPLOSTVOLATIME						0xB321
#define CON_DI_TYPE_MPLOSTVOLBTIME						0xB322
#define CON_DI_TYPE_MPLOSTVOLCTIME						0xB323
#define CON_DI_TYPE_MPB32F_BLOCK						0xB32F		//累计断相时间  用大项

#define CON_DI_TYPE_MPLASTVOLOFFBEGTIME					0xB330
#define CON_DI_TYPE_MPLASTVOLAOFFBEGTIME				0xB331
#define CON_DI_TYPE_MPLASTVOLBOFFBEGTIME				0xB332
#define CON_DI_TYPE_MPLASTVOLCOFFBEGTIME				0xB333
#define CON_DI_TYPE_MPB33F_BLOCK						0xB33F		//最近一次断相起始时刻		用小项

#define CON_DI_TYPE_MPLASTVOLOFFENDTIME					0xB340
#define CON_DI_TYPE_MPLASTVOLAOFFENDTIME				0xB341
#define CON_DI_TYPE_MPLASTVOLBOFFENDTIME				0xB342
#define CON_DI_TYPE_MPLASTVOLCOFFENDTIME				0xB343
#define CON_DI_TYPE_MPB34F_BLOCK						0xB34F		//最近一次断相结束时刻		用小项

//湖北扩展
#define CON_DI_TYPE_HB_DIRVARTOTAL						0xE410		//当前无功电能绝对值和（1+4象限）
#define CON_DI_TYPE_HB_UNDIRVARTOTAL					0xE411		//当前无功电能绝对值和（2+3象限）
#define CON_DI_TYPE_HB_DIRVARTOTAL_LM					0xE430		//上月无功电能绝对值和（1+4象限）
#define CON_DI_TYPE_HB_UNDIRVARTOTAL_LM					0xE441		//上月无功电能绝对值和（2+3象限）


//07规约扩展
#define CON_DI_TYPE_MPVOLPHASEA							0x0101//A相电压相角
#define CON_DI_TYPE_MPVOLPHASEB							0x0102//B相电压相角
#define CON_DI_TYPE_MPVOLPHASEC							0x0103//C相电压相角
#define CON_DI_TYPE_MPVOLPHASE_BLOCK					0x01FF//电压相角数据块

#define CON_DI_TYPE_MPCURPHASEA							0x0201//A相电流相角
#define CON_DI_TYPE_MPCURPHASEB							0x0202//B相电流相角
#define CON_DI_TYPE_MPCURPHASEC							0x0203//C相电流相角
#define CON_DI_TYPE_MPCURPHASE_BLOCK					0x02FF//电流相角数据块


#define CON_DI_TYPE_MPPROGRAMNUM						0X3000//编程总次数
#define CON_DI_TYPE_MPPROGRAMTIME						0X3001//最近一次编程时间

#define CON_DI_TYPE_MPMETERCLRNUM						0X3010//电表清0总次数
#define CON_DI_TYPE_MPMETERCLRTIME						0X3011//最近一次电表清0时间

#define CON_DI_TYPE_MPDEMODECLRNUM						0X3020//需量清0总次数
#define CON_DI_TYPE_MPDEMODECLRTIME						0X3021//最近一次需量清0时间

#define CON_DI_TYPE_MPEVENTCLRNUM						0X3030//事件清0总次数
#define CON_DI_TYPE_MPEVENTCLRTIME						0X3031//最近一次事件清0时间

#define CON_DI_TYPE_MPJIAOSHINUM						0X3040//校时总次数
#define CON_DI_TYPE_MPJIAOSHITIME						0X3041//最近一次校时时间

#define CON_DI_TYPE_MPCUFE_GA							0x3050
#define CON_DI_TYPE_MPCUFE_BA							0x3051
#define CON_DI_TYPE_MPCUFE_RA							0x3052
#define CON_DI_TYPE_MPCUFE_XA							0x3053
#define CON_DI_TYPE_MPCUFE_GB							0x3054
#define CON_DI_TYPE_MPCUFE_BB							0x3055
#define CON_DI_TYPE_MPCUFE_RB							0x3056
#define CON_DI_TYPE_MPCUFE_XB							0x3057
#define CON_DI_TYPE_MPCUFE_GC							0x3058
#define CON_DI_TYPE_MPCUFE_BC							0x3059
#define CON_DI_TYPE_MPCUFE_RC							0x305A
#define CON_DI_TYPE_MPCUFE_XC							0x305B
#define CON_DI_TYPE_MPCUFE							0x305F//

#define CON_DI_TYPE_MPDIRPOWA						0x3060
#define CON_DI_TYPE_MPUNDIRPOWA						0x3061
#define CON_DI_TYPE_MPCOMVAR1A						0x3062
#define CON_DI_TYPE_MPCOMVAR2A						0x3063

#define CON_DI_TYPE_MPDIRPOWB							0x3064
#define CON_DI_TYPE_MPUNDIRPOWB						0x3065
#define CON_DI_TYPE_MPCOMVAR1B						0x3066
#define CON_DI_TYPE_MPCOMVAR2B						0x3067

#define CON_DI_TYPE_MPDIRPOWC							0x3068
#define CON_DI_TYPE_MPUNDIRPOWC						0x3069
#define CON_DI_TYPE_MPCOMVAR1C						0x306A
#define CON_DI_TYPE_MPCOMVAR2C						0x306B

#define CON_DI_TYPE_MPDIRPOWA_LM							0x3070
#define CON_DI_TYPE_MPUNDIRPOWA_LM						0x3071
#define CON_DI_TYPE_MPCOMVAR1A_LM						0x3072
#define CON_DI_TYPE_MPCOMVAR2A_LM						0x3073

#define CON_DI_TYPE_MPDIRPOWB_LM							0x3074
#define CON_DI_TYPE_MPUNDIRPOWB_LM						0x3075
#define CON_DI_TYPE_MPCOMVAR1B_LM						0x3076
#define CON_DI_TYPE_MPCOMVAR2B_LM						0x3077

#define CON_DI_TYPE_MPDIRPOWC_LM							0x3078
#define CON_DI_TYPE_MPUNDIRPOWC_LM						0x3079
#define CON_DI_TYPE_MPCOMVAR1C_LM						0x307A
#define CON_DI_TYPE_MPCOMVAR2C_LM						0x307B

/*************************数据项错误编码常量**************************************/
#define CON_ERRCODE_NO				0		//正确
#define CON_ERRCODE_RELAYOVERTIME	1		//中继超时
#define CON_ERRCODE_PARA			2		//设置内容非法
#define CON_ERRCODE_PASSWORD		3		//密码权限不足
#define CON_ERRCODE_DI				4		//无此项数据
#define CON_ERRCODE_INVALID			5
#define CON_ERRCODE_TIMEOVER		6		//实时设置参数超时

#define	CON_PURVIEW_ADMIN	0x55
#define	CON_PURVIEW_SET		0xAA
#define	CON_PURVIEW_READ	0x00		//只读

/*************************结构体定义*************************************/
//以数据项可建立一张表：以数据标识为主键，通过主键查询可以得到是否有此测量点，内容长度，属于哪类测量点、大项命令、是否可设等

#define CON_DI_HAVE				0x55
#define CON_DI_NOHAVE			0xAA
//类型信息
#define CON_DI_TYPE_PARA		0x55
#define CON_DI_TYPE_DATA		0xAA
//所属测量点类型信息
#define CON_DI_MPTYPE_TERSELF	0x01		//终端
#define CON_DI_MPTYPE_NORMAL	0x10		//测量点
//是否可设置
#define CON_DI_OPERATE_ENABLE	0x55
#define CON_DI_OPERATE_DISABLE	0xAA
#define CON_DI_OPERATE_INVALID	0xEE
//设置权限
#define CON_DI_SETPURVIEW_NORMAL	0x55
#define CON_DI_SETPURVIEW_ADMIN		0xAA
#define CON_DI_SETPURVIEW_INVALID	0xFF

#define	CON_DI_LEN_CHANGED			0xFF	//变长

typedef struct _STDIINFO			//测量点信息
{
	unsigned char ucHave;			//查询结果：有/无
	unsigned char ucType;			//类型：参数/数据
	unsigned char ucMPType;			//测量点类型：终端/测量点
	unsigned char ucSetEnable;		//设置：是/否
	unsigned char ucPurView;		//设置操作权限：低级/高级
	unsigned char ucGetEnable;		//读取：是/否
	unsigned char ucReadMeterEnabel;//可以抄表：是/否 否：表示在任务数据中，此数据可从历史值中获取
	unsigned char ucContentLen;		//内容长度
	unsigned char ucCommandLen;		//读取此项数据的命令参数长度
	unsigned short usBigDI;			//大项命令
}STDIINFO;

//波特率
#define CON_PORT_BAUDRATE_300		0x01
#define CON_PORT_BAUDRATE_600		0x02
#define CON_PORT_BAUDRATE_1200		0x04
#define CON_PORT_BAUDRATE_2400		0x08
#define CON_PORT_BAUDRATE_4800		0x10
#define CON_PORT_BAUDRATE_9600		0x20

#endif

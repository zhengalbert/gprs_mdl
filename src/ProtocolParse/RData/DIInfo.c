/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : DIInfo.c
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"
//获取各种电表提供量值的信息，作为终端本身来讲，可提供总电量等内容，但不能提供各费率电量
STDIINFO Main_DI_Data_Query_Manage(unsigned short usDI)
{
	STDIINFO stRetDIInfo;
	ClearDIInfo( &stRetDIInfo );

	if( usDI/0x100==0x90 )
	{
		switch( usDI )
		{
		case CON_DI_TYPE_MPDIRPOWTOTAL:						//0x9010正向有功总电能
		case CON_DI_TYPE_MPDIRPOWPLUS:						//0x9011正向有功尖电能
		case CON_DI_TYPE_MPDIRPOWAPEX:						//0x9012正向有功峰电能
		case CON_DI_TYPE_MPDIRPOWFLAT:						//0x9013正向有功平电能
		case CON_DI_TYPE_MPDIRPOWVALE:						//0x9014正向有功谷电能
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,		
				4,0,					CON_DI_TYPE_MPDIRPOW_BLOCK);
			break;
		case CON_DI_TYPE_MPDIRPOW_BLOCK:					//0x901F正向有功电能数据块
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				20,	0,					CON_DI_TYPE_MPDIRPOW_BLOCK);
			break;
			///////////////////////////////////////////////////////////////////////////////
		case CON_DI_TYPE_MPUNDIRPOWTOTAL:					//0x9020反向有功总电能
		case CON_DI_TYPE_MPUNDIRPOWPLUS:					//0x9021反向有功尖电能
		case CON_DI_TYPE_MPUNDIRPOWAPEX:					//0x9022反向有功峰电能
		case CON_DI_TYPE_MPUNDIRPOWFLAT:					//0x9023反向有功平电能
		case CON_DI_TYPE_MPUNDIRPOWVALE:					//0x9024反向有功谷电能
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,					CON_DI_TYPE_MPUNDIRPOW_BLOCK);
			break;
		case CON_DI_TYPE_MPUNDIRPOW_BLOCK: 					//0x902F反向有功电能数据块
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				20,0,					CON_DI_TYPE_MPUNDIRPOW_BLOCK);
			break;
		default:
			break;
		}
	}
	else if( usDI/0x100==0x91 )
	{
		switch( usDI )
		{
		case CON_DI_TYPE_MPDIRVARTOTAL:						//0x9110//正向无功总电能
		case CON_DI_TYPE_MPDIRVARPLUS:						//0x9111//正向无功尖电能
		case CON_DI_TYPE_MPDIRVARAPEX:						//0x9112//正向无功峰电能
		case CON_DI_TYPE_MPDIRVARFLAT:						//0x9113//正向无功平电能
		case CON_DI_TYPE_MPDIRVARVALE:						//0x9114//正向无功谷电能
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,					CON_DI_TYPE_MPDIRVAR_BLOCK);
			break;
		case CON_DI_TYPE_MPDIRVAR_BLOCK:					//0x911F//正向无功电能数据块
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE, CON_DI_OPERATE_ENABLE,	
				20,0,					CON_DI_TYPE_MPDIRVAR_BLOCK);
			break;
		case CON_DI_TYPE_MPUNDIRVARTOTAL:					//0x9120//反向无功总电能
		case CON_DI_TYPE_MPUNDIRVARPLUS:					//0x9121//反向无功尖电能
		case CON_DI_TYPE_MPUNDIRVARAPEX:					//0x9122//反向无功峰电能
		case CON_DI_TYPE_MPUNDIRVARFLAT:					//0x9123//反向无功平电能
		case CON_DI_TYPE_MPUNDIRVARVALE:					//0x9124//反向无功谷电能
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,					CON_DI_TYPE_MPUNDIRVAR_BLOCK);
			break;
		case CON_DI_TYPE_MPUNDIRVAR_BLOCK:					//0x912F//反向无功电能数据块
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE, CON_DI_OPERATE_ENABLE,	
				20,0,					CON_DI_TYPE_MPUNDIRVAR_BLOCK);
			break;
		case CON_DI_TYPE_MPVAR1:							//0x9130一象限无功总电能
		case CON_DI_TYPE_MPVAR1_FEE1:						//0x9131费率1
		case CON_DI_TYPE_MPVAR1_FEE2:						//0x9132费率2
		case CON_DI_TYPE_MPVAR1_FEE3:						//0x9133费率3
		case CON_DI_TYPE_MPVAR1_FEE4:						//0x9134费率4
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,					CON_DI_TYPE_MPVAR1_BLOCK);
			break;
		case CON_DI_TYPE_MPVAR1_BLOCK:						//0x913F
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE, CON_DI_OPERATE_ENABLE,	
				20,0,					CON_DI_TYPE_MPVAR1_BLOCK);
			break;
			///////////////////////////////////////////////////////////////////////////
		case CON_DI_TYPE_MPVAR4:							//0x9140四象限无功总电能
		case CON_DI_TYPE_MPVAR4_FEE1:						//0x9141费率1
		case CON_DI_TYPE_MPVAR4_FEE2:						//0x9142费率2
		case CON_DI_TYPE_MPVAR4_FEE3:						//0x9143费率3
		case CON_DI_TYPE_MPVAR4_FEE4:						//0x9144费率4
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,					CON_DI_TYPE_MPVAR4_BLOCK);
			break;
		case CON_DI_TYPE_MPVAR4_BLOCK:						//0x914F
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				20,0,					CON_DI_TYPE_MPVAR4_BLOCK);
			break;
			//////////////////////////////////////////////////////////////////////////
		case CON_DI_TYPE_MPVAR2:							//0x9150二象限无功总电能
		case CON_DI_TYPE_MPVAR2_FEE1:						//0x9151费率1
		case CON_DI_TYPE_MPVAR2_FEE2:						//0x9152费率2
		case CON_DI_TYPE_MPVAR2_FEE3:						//0x9153费率3
		case CON_DI_TYPE_MPVAR2_FEE4:						//0x9154费率4
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,					CON_DI_TYPE_MPVAR2_BLOCK);
			break;
		case CON_DI_TYPE_MPVAR2_BLOCK:						//0x915F
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				20,0,					CON_DI_TYPE_MPVAR2_BLOCK);
			break;
		case CON_DI_TYPE_MPVAR3:							//0x9160三象限无功总电能
		case CON_DI_TYPE_MPVAR3_FEE1:						//0x9161费率1
		case CON_DI_TYPE_MPVAR3_FEE2:						//0x9162费率2
		case CON_DI_TYPE_MPVAR3_FEE3:						//0x9163费率3
		case CON_DI_TYPE_MPVAR3_FEE4:						//0x9164费率4
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,					CON_DI_TYPE_MPVAR3_BLOCK);
			break;
		case CON_DI_TYPE_MPVAR3_BLOCK:						//0x916F
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				20,0,					CON_DI_TYPE_MPVAR3_BLOCK);
			break;
		default:
			break;
		}
	}
	else if( usDI/0x100==0x94 )
	{
		switch( usDI )
		{
		case CON_DI_TYPE_MPDIRPOWTOTAL_LM:						//0x9410正向有功总电能（上月）
		case CON_DI_TYPE_MPDIRPOWPLUS_LM:						//0x9411正向有功尖电能（上月）
		case CON_DI_TYPE_MPDIRPOWAPEX_LM:						//0x9412正向有功峰电能（上月）
		case CON_DI_TYPE_MPDIRPOWFLAT_LM:						//0x9413正向有功平电能（上月）
		case CON_DI_TYPE_MPDIRPOWVALE_LM:						//0x9414正向有功谷电能（上月）
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,					CON_DI_TYPE_MPDIRPOW_LM_BLOCK);
			break;
		case CON_DI_TYPE_MPDIRPOW_LM_BLOCK:						//0x941F正向有功电能数据块（上月）
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				20,0,					CON_DI_TYPE_MPDIRPOW_LM_BLOCK);
			break;
			///////////////////////////////////////////////////////////////////////////////////
		case CON_DI_TYPE_MPUNDIRPOWTOTAL_LM:					//0x9420反向有功总电能（上月）
		case CON_DI_TYPE_MPUNDIRPOWPLUS_LM:						//0x9421反向有功尖电能（上月）
		case CON_DI_TYPE_MPUNDIRPOWAPEX_LM:						//0x9422反向有功峰电能（上月）
		case CON_DI_TYPE_MPUNDIRPOWFLAT_LM:						//0x9423反向有功平电能（上月）
		case CON_DI_TYPE_MPUNDIRPOWVALE_LM:						//0x9424反向有功谷电能（上月）
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,						CON_DI_TYPE_MPUNDIRPOW_LM_BLOCK);
			break;
		case CON_DI_TYPE_MPUNDIRPOW_LM_BLOCK: 					//0x942F反向有功电能数据块（上月）
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				20,0,						CON_DI_TYPE_MPUNDIRPOW_LM_BLOCK);
			break;
		default:
			break;
		}
	}
	else if( usDI/0x100==0x95 )	
	{
		switch( usDI )
		{
		case CON_DI_TYPE_MPDIRVARTOTAL_LM:						//0x9510正向无功总电能（上月）
		case CON_DI_TYPE_MPDIRVARPLUS_LM:						//0x9511正向无功尖电能（上月）
		case CON_DI_TYPE_MPDIRVARAPEX_LM:						//0x9512正向无功峰电能（上月）
		case CON_DI_TYPE_MPDIRVARFLAT_LM:						//0x9513正向无功平电能（上月）
		case CON_DI_TYPE_MPDIRVARVALE_LM:						//0x9514正向无功谷电能（上月）
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,					CON_DI_TYPE_MPDIRVAR_LM_BLOCK);
			break;
		case CON_DI_TYPE_MPDIRVAR_LM_BLOCK:						//0x951F正向无功电能数据块（上月）
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				20,0,					CON_DI_TYPE_MPDIRVAR_LM_BLOCK);
			break;
			///////////////////////////////////////////////////////////////////////////////////
		case CON_DI_TYPE_MPUNDIRVARTOTAL_LM:					//0x9520反向无功总电能（上月）
		case CON_DI_TYPE_MPUNDIRVARPLUS_LM:						//0x9521反向无功尖电能（上月）
		case CON_DI_TYPE_MPUNDIRVARAPEX_LM:						//0x9522反向无功峰电能（上月）
		case CON_DI_TYPE_MPUNDIRVARFLAT_LM:						//0x9523反向无功平电能（上月）
		case CON_DI_TYPE_MPUNDIRVARVALE_LM:						//0x9524反向无功谷电能（上月）
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,						CON_DI_TYPE_MPUNDIRVAR_LM_BLOCK);
			break;
		case CON_DI_TYPE_MPUNDIRVAR_LM_BLOCK: 					//0x952F反向无功电能数据块（上月）
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				20,0,						CON_DI_TYPE_MPUNDIRVAR_LM_BLOCK);
			break;
		case CON_DI_TYPE_MPVAR1_LM:							//0x9530//一象限无功总电能（上月）
		case CON_DI_TYPE_MPVAR1_LM_FEE1:					//0x9531//费率1
		case CON_DI_TYPE_MPVAR1_LM_FEE2:					//0x9532//费率2
		case CON_DI_TYPE_MPVAR1_LM_FEE3:					//0x9533//费率3
		case CON_DI_TYPE_MPVAR1_LM_FEE4:					//0x9534//费率4
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,						CON_DI_TYPE_MPVAR1_LM_BLOCK);
			break;
		case CON_DI_TYPE_MPVAR1_LM_BLOCK:					//0x953F
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				20,0,						CON_DI_TYPE_MPVAR1_LM_BLOCK);
			break;
		case CON_DI_TYPE_MPVAR4_LM:							//0x9540//四象限无功总电能（上月）
		case CON_DI_TYPE_MPVAR4_LM_FEE1:					//0x9541//费率1
		case CON_DI_TYPE_MPVAR4_LM_FEE2:					//0x9542//费率2
		case CON_DI_TYPE_MPVAR4_LM_FEE3:					//0x9543//费率3
		case CON_DI_TYPE_MPVAR4_LM_FEE4:					//0x9544//费率4
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,						CON_DI_TYPE_MPVAR4_LM_BLOCK);
			break;
		case CON_DI_TYPE_MPVAR4_LM_BLOCK:					//0x954F
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				20,0,						CON_DI_TYPE_MPVAR4_LM_BLOCK);
			break;
		case CON_DI_TYPE_MPVAR2_LM:							//0x9550//二象限无功总电能（上月）
		case CON_DI_TYPE_MPVAR2_LM_FEE1:					//0x9551//费率1
		case CON_DI_TYPE_MPVAR2_LM_FEE2:					//0x9552//费率2
		case CON_DI_TYPE_MPVAR2_LM_FEE3:					//0x9553//费率3
		case CON_DI_TYPE_MPVAR2_LM_FEE4:					//0x9554//费率4
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,						CON_DI_TYPE_MPVAR2_LM_BLOCK);
			break;
		case CON_DI_TYPE_MPVAR2_LM_BLOCK:					//0x955F
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				20,0,						CON_DI_TYPE_MPVAR2_LM_BLOCK);
			break;
		case CON_DI_TYPE_MPVAR3_LM:							//0x9560//三象限无功总电能（上月）
		case CON_DI_TYPE_MPVAR3_LM_FEE1:					//0x9561//费率1
		case CON_DI_TYPE_MPVAR3_LM_FEE2:					//0x9562//费率2
		case CON_DI_TYPE_MPVAR3_LM_FEE3:					//0x9563//费率3
		case CON_DI_TYPE_MPVAR3_LM_FEE4:					//0x9564//费率4
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,						CON_DI_TYPE_MPVAR3_LM_BLOCK);
			break;
		case CON_DI_TYPE_MPVAR3_LM_BLOCK:					//0x956F
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				20,0,						CON_DI_TYPE_MPVAR3_LM_BLOCK);
			break;
		default:
			break;
		}
	}
	else if( usDI/0x100==0xA0 )
	{
		switch( usDI )
		{
		case CON_DI_TYPE_MPDIRPOWMAXDEMAND:						//0xA010正向有功总最大需量
		case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE1:				//0xA011费率1
		case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE2:				//0xA012费率2
		case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE3:				//0xA013费率3
		case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE4:				//0xA014费率4
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				3,0,					CON_DI_TYPE_MPDIRPOWMAXDEMAND_BLOCK);
			break;
		case CON_DI_TYPE_MPDIRPOWMAXDEMAND_BLOCK:				//0xA01F
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				15,0,					CON_DI_TYPE_MPDIRPOWMAXDEMAND_BLOCK);
			break;
			///////////////////////////////////////////////////////////////////////////////////
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND:					//0xA020//反向有功总最大需量
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE1:				//0xA021//费率1
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE2:				//0xA022//费率2
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE3:				//0xA023//费率3
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE4:				//0xA024//费率4
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				3,0,					CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_BLOCK);
			break;
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_BLOCK:				//0xA02F
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				15,0,					CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_BLOCK);
			break;
		default:
			break;
		}
	}
	else if( usDI/0x100==0xA4 )
	{
		switch( usDI )
		{
		case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM:				//0xA410//正向有功总最大需量（上月）
		case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE1:			//0xA411//费率1
		case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE2:			//0xA412//费率2
		case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE3:			//0xA413//费率3
		case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE4:			//0xA414//费率4
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				3,0,					CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_BLOCK);
			break;
		case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_BLOCK:			//0xA41F
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				15,	0,					CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_BLOCK);
			break;
			///////////////////////////////////////////////////////////////////////////////////////
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM:				//0xA420//反向有功总最大需量（上月）
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE1:		//0xA421//费率1
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE2:		//0xA422//费率2
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE3:		//0xA423//费率3
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE4:		//0xA424//费率4
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				3,0,					CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_BLOCK);
			break;
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_BLOCK:		//0xA42F
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				15,0,					CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_BLOCK);
			break;
		default:
			break;
		}
	}
	else if( usDI/0x100==0xB0 )
	{
		switch( usDI )
		{
		case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME:					//0xB010//正向有功总最大需量发生时间
		case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE1:			//0xB011//费率1
		case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE2:			//0xB012//费率2
		case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE3:			//0xB013//费率3
		case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE4:			//0xB014//费率4
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,						CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_BLOCK);
			break;
		case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_BLOCK:			//0xB01F
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				20,0,						CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_BLOCK);
			break;
			///////////////////////////////////////////////////////////////////////////////
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME:				//0xB020//反向有功总最大需量发生时间
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE1:		//0xB021//费率1
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE2:		//0xB022//费率2
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE3:		//0xB023//费率3
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE4:		//0xB024//费率4
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,						CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_BLOCK);
			break;
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_BLOCK:		//0xB02F
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				20,0,						CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_BLOCK);
			break;
		default:
			break;
		}
	}
	else if( usDI/0x100==0xB4 )
	{
		switch( usDI )
		{
		case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM:			//0xB410//正向有功总最大需量发生时间（上月）
		case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE1:		//0xB411//费率1
		case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE2:		//0xB412//费率2
		case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE3:		//0xB413//费率3
		case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE4:		//0xB414//费率4
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,						CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_BLOCK);
			break;
		case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_BLOCK:		//0xB41F
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				20,0,						CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_BLOCK);
			break;
			//////////////////////////////////////////////////////////////////////
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM:		////反向有功总最大需量发生时间（上月）
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE1:	//	0xB421//费率1
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE2:	//	0xB422//费率2
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE3:	//	0xB423//费率3
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE4:	//	0xB424//费率4
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,						CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_BLOCK);
			break;
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_BLOCK:	//0xB42F
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				20,0,						CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_BLOCK);
			break;
		default:
			break;
		}
	}
	else if( usDI/0x100==0xB6 )
	{
		switch( usDI )
		{
		case CON_DI_TYPE_MPVOLA:								//0xB611A相电压
		case CON_DI_TYPE_MPVOLB:								//0xB612B相电压
		case CON_DI_TYPE_MPVOLC:								//0xB613C相电压
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				2,0,					usDI);
			break;
		case CON_DI_TYPE_MPVOL_BLOCK:							//0xB61F电压数据块
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				6,0,					CON_DI_TYPE_MPVOL_BLOCK);
			break;
			/////////////////////////////////////////////////////////////////
		case CON_DI_TYPE_MPCURA:								//0xB621A相电流
		case CON_DI_TYPE_MPCURB:								//0xB622B相电流
		case CON_DI_TYPE_MPCURC:								//0xB623C相电流 
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				3,0,					usDI);
			break;
		case CON_DI_TYPE_MPCUR_BLOCK:							//0xB62F电流数据块
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				9,0,					CON_DI_TYPE_MPCUR_BLOCK);
			break;
			//////////////////////////////////////////////////////////////////////////
		case CON_DI_TYPE_MPINSTANTPOW:						//0xB630//瞬时有功功率
		case CON_DI_TYPE_MPINSTANTPOWA:						//0xB631//A相有功功率
		case CON_DI_TYPE_MPINSTANTPOWB:						//0xB632//B相有功功率
		case CON_DI_TYPE_MPINSTANTPOWC:						//0xB633//C相有功功率
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				3,0,					usDI);
			break;
		case CON_DI_TYPE_MPINSTANTPOW_BLOCK:					//0xB63F//
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				12,0,					CON_DI_TYPE_MPINSTANTPOW_BLOCK);
			break;
			////////////////////////////////////////////////////////////////
		case CON_DI_TYPE_MPINSTANTVAR:						//0xB640//瞬时无功功率
		case CON_DI_TYPE_MPINSTANTVARA:						//0xB641//A相无功功率
		case CON_DI_TYPE_MPINSTANTVARB:						//0xB642//B相无功功率
		case CON_DI_TYPE_MPINSTANTVARC:						//0xB643//C相无功功率
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				2,0,					usDI);
			break;
		case CON_DI_TYPE_MPINSTANTVAR_BLOCK:					//0xB64F//
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				8,0,					CON_DI_TYPE_MPINSTANTVAR_BLOCK);
			break;
		case CON_DI_TYPE_MPPOWRATE:							//0xB650//总功率因素
		case CON_DI_TYPE_MPPOWRATEA:						//0xB651//A相功率因素
		case CON_DI_TYPE_MPPOWRATEB:						//0xB652//B相功率因素
		case CON_DI_TYPE_MPPOWRATEC:						//0xB653//C相功率因素
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				2,0,					usDI);
			break;
		case CON_DI_TYPE_MPPOWRATE_BLOCK:					//0xB65F//
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				8,0,					CON_DI_TYPE_MPPOWRATE_BLOCK);
			break;
			default:
				break;
		}
	}
	else if( usDI/0x100==0xC0 )
	{
		switch( usDI )
		{
		case CON_DI_TYPE_MPPOWCONST:							//0xC030//有功常数
		case CON_DI_TYPE_MPVARCONST:							//0xC031//无功常数
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				3,0,					usDI);
			break;
		case CON_DI_TYPE_MPCONST_BLOCK:
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				6,0,					CON_DI_TYPE_MPCONST_BLOCK);
			break;
		default:break;
		}
	}
	else if( usDI/0x100==0xC3 )
	{
		switch( usDI )
		{
		case CON_DI_TYPE_MPFEERATE1:							//0xC331	//费率1
		case CON_DI_TYPE_MPFEERATE2:							//0xC332	//费率2
		case CON_DI_TYPE_MPFEERATE3:							//0xC333	//费率3
		case CON_DI_TYPE_MPFEERATE4:							//0xC334	//费率4
		case CON_DI_TYPE_MPFEERATE5:							//0xC335	//费率5
		case CON_DI_TYPE_MPFEERATE6:							//0xC336	//费率6
		case CON_DI_TYPE_MPFEERATE7:							//0xC337	//费率7
		case CON_DI_TYPE_MPFEERATE8:							//0xC338	//费率8
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				3,0,					CON_DI_TYPE_MPFEERATE_BLOCK);
			break;
		case CON_DI_TYPE_MPFEERATE_BLOCK:						//0xC33F
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				24,0,					CON_DI_TYPE_MPFEERATE_BLOCK);
			break;			
		default:break;
		}
	}
	else if( usDI/0x100==0x8F )
	{
		switch( usDI )
		{
//azh 170420 代码空间不够
//		case CON_DI_TYPE_SUMINSTANTPOW1:						//0x8F50//有功功率总加组1
//		case CON_DI_TYPE_SUMINSTANTPOW2:						//0x8F51//有功功率总加组2
//		case CON_DI_TYPE_SUMINSTANTPOW3:						//0x8F52//有功功率总加组3
//		case CON_DI_TYPE_SUMINSTANTPOW4:						//0x8F53//有功功率总加组4
//		case CON_DI_TYPE_SUMINSTANTPOW5:						//0x8F54//有功功率总加组5
//		case CON_DI_TYPE_SUMINSTANTPOW6:						//0x8F55//有功功率总加组6
//		case CON_DI_TYPE_SUMINSTANTPOW7:						//0x8F56//有功功率总加组7
//		case CON_DI_TYPE_SUMINSTANTPOW8:						//0x8F57//有功功率总加组8
//			SetDIInfo(&stRetDIInfo,
//				CON_DI_HAVE,									
//				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
//				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
//				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
//				4,0,					usDI);
//			break;
//		case CON_DI_TYPE_SUMINSTANTVAR1:					//0x8F60//无功功率总加组1
//		case CON_DI_TYPE_SUMINSTANTVAR2:					//0x8F61//无功功率总加组2
//		case CON_DI_TYPE_SUMINSTANTVAR3:					//0x8F62//无功功率总加组3
//		case CON_DI_TYPE_SUMINSTANTVAR4:					//0x8F63//无功功率总加组4
//		case CON_DI_TYPE_SUMINSTANTVAR5:					//0x8F64//无功功率总加组5
//		case CON_DI_TYPE_SUMINSTANTVAR6:					//0x8F65//无功功率总加组6
//		case CON_DI_TYPE_SUMINSTANTVAR7:					//0x8F66//无功功率总加组7
//		case CON_DI_TYPE_SUMINSTANTVAR8:					//0x8F67//无功功率总加组8
//			SetDIInfo(&stRetDIInfo,
//				CON_DI_HAVE,									
//				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
//				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
//				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
//				4,0,					usDI);
//			break;
//		case CON_DI_TYPE_SUMPOW1:							//0x8F70//有功总加组1
//		case CON_DI_TYPE_SUMPOW2:							//0x8F71//有功总加组2
//		case CON_DI_TYPE_SUMPOW3:							//0x8F72//有功总加组3
//		case CON_DI_TYPE_SUMPOW4:							//0x8F73//有功总加组4
//		case CON_DI_TYPE_SUMPOW5:							//0x8F74//有功总加组5
//		case CON_DI_TYPE_SUMPOW6:							//0x8F75//有功总加组6
//		case CON_DI_TYPE_SUMPOW7:							//0x8F76//有功总加组7
//		case CON_DI_TYPE_SUMPOW8:							//0x8F77//有功总加组8
//		case CON_DI_TYPE_SUMVAR1:						//0x8F80//无功总加组1
//		case CON_DI_TYPE_SUMVAR2:						//0x8F81//无功总加组2
//		case CON_DI_TYPE_SUMVAR3:						//0x8F82//无功总加组3
//		case CON_DI_TYPE_SUMVAR4:						//0x8F83//无功总加组4
//		case CON_DI_TYPE_SUMVAR5:						//0x8F84//无功总加组5
//		case CON_DI_TYPE_SUMVAR6:						//0x8F85//无功总加组6
//		case CON_DI_TYPE_SUMVAR7:						//0x8F86//无功总加组7
//		case CON_DI_TYPE_SUMVAR8:						//0x8F87//无功总加组8
//			SetDIInfo(&stRetDIInfo,
//				CON_DI_HAVE,									
//				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
//				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
//				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
//				4,0,					usDI);
//			break;
		default:break;
		}
	}
	return stRetDIInfo;
}

//unsigned short GetInstantPowDI_byGroupNo(unsigned char ucGroupNo)
//{
//	unsigned short usDI=CON_DI_TYPE_INVALID;
//	switch(ucGroupNo)
//	{
//	case 0:
//		usDI=CON_DI_TYPE_SUMINSTANTPOW1;                        //0x8F50//有功功率总加组1
//		break;
//	case 1:
//		usDI=CON_DI_TYPE_SUMINSTANTPOW2;						//0x8F51//有功功率总加组2
//		break;
//	case 2:
//		usDI=CON_DI_TYPE_SUMINSTANTPOW3;						//0x8F52//有功功率总加组3
//		break;
//	case 3:
//		usDI=CON_DI_TYPE_SUMINSTANTPOW4;						//0x8F53//有功功率总加组4
//		break;
//	case 4:
//		usDI=CON_DI_TYPE_SUMINSTANTPOW5;						//0x8F54//有功功率总加组5
//		break;
//	case 5:
//		usDI=CON_DI_TYPE_SUMINSTANTPOW6;						//0x8F55//有功功率总加组6
//		break;
//	case 6:
//		usDI=CON_DI_TYPE_SUMINSTANTPOW7;						//0x8F56//有功功率总加组7
//		break;
//	case 7:
//		usDI=CON_DI_TYPE_SUMINSTANTPOW8;						//0x8F57//有功功率总加组8
//		break;
//	default:
//		break;
//	}
//	return usDI;
//}
//
//unsigned short GetInstantVarDI_byGroupNo(unsigned char ucGroupNo)
//{
//	unsigned short usDI=CON_DI_TYPE_INVALID;
//	switch(ucGroupNo)
//	{
//	case 0:
//		usDI=CON_DI_TYPE_SUMINSTANTVAR1;					//0x8F60//无功功率总加组1
//		break;
//	case 1:
//		usDI=CON_DI_TYPE_SUMINSTANTVAR2;					//0x8F61//无功功率总加组2
//		break;
//	case 2:
//		usDI=CON_DI_TYPE_SUMINSTANTVAR3;					//0x8F62//无功功率总加组3
//		break;
//	case 3:
//		usDI=CON_DI_TYPE_SUMINSTANTVAR4;					//0x8F63//无功功率总加组4
//		break;
//	case 4:
//		usDI=CON_DI_TYPE_SUMINSTANTVAR5;					//0x8F64//无功功率总加组5
//		break;
//	case 5:
//		usDI=CON_DI_TYPE_SUMINSTANTVAR6;					//0x8F65//无功功率总加组6
//		break;
//	case 6:
//		usDI=CON_DI_TYPE_SUMINSTANTVAR7;					//0x8F66//无功功率总加组7
//		break;
//	case 7:
//		usDI=CON_DI_TYPE_SUMINSTANTVAR8;					//0x8F67//无功功率总加组8
//		break;
//	default:
//		break;
//	}
//	return usDI;
//}

//unsigned short GetPowDI_byGroupNo(unsigned char ucGroupNo)
//{
//	unsigned short usDI=CON_DI_TYPE_INVALID;
//	switch(ucGroupNo)
//	{
//	case 0:
//		usDI=CON_DI_TYPE_SUMPOW1;							//0x8F70//有功总加组1
//		break;
//	case 1:
//		usDI=CON_DI_TYPE_SUMPOW2;							//0x8F71//有功总加组2
//		break;
//	case 2:
//		usDI=CON_DI_TYPE_SUMPOW3;							//0x8F72//有功总加组3
//		break;
//	case 3:
//		usDI=CON_DI_TYPE_SUMPOW4;							//0x8F73//有功总加组4
//		break;
//	case 4:
//		usDI=CON_DI_TYPE_SUMPOW5;							//0x8F74//有功总加组5
//		break;
//	case 5:
//		usDI=CON_DI_TYPE_SUMPOW6;							//0x8F75//有功总加组6
//		break;
//	case 6:
//		usDI=CON_DI_TYPE_SUMPOW7;							//0x8F76//有功总加组7
//		break;
//	case 7:
//		usDI=CON_DI_TYPE_SUMPOW8;							//0x8F77//有功总加组8
//		break;
//	default:
//		break;
//	}
//	return usDI;
//}

//unsigned short GetVarDI_byGroupNo(unsigned char ucGroupNo)
//{
//	unsigned short usDI=CON_DI_TYPE_INVALID;
//	switch(ucGroupNo)
//	{
//	case 0:
//		usDI=CON_DI_TYPE_SUMVAR1;						//0x8F80//无功总加组1
//		break;
//	case 1:
//		usDI=CON_DI_TYPE_SUMVAR2;						//0x8F81//无功总加组2
//		break;
//	case 2:
//		usDI=CON_DI_TYPE_SUMVAR3;						//0x8F82//无功总加组3
//		break;
//	case 3:
//		usDI=CON_DI_TYPE_SUMVAR4;						//0x8F83//无功总加组4
//		break;
//	case 4:
//		usDI=CON_DI_TYPE_SUMVAR5;						//0x8F84//无功总加组5
//		break;
//	case 5:
//		usDI=CON_DI_TYPE_SUMVAR6;						//0x8F85//无功总加组6
//		break;
//	case 6:
//		usDI=CON_DI_TYPE_SUMVAR7;						//0x8F86//无功总加组7
//		break;
//	case 7:
//		usDI=CON_DI_TYPE_SUMVAR8;						//0x8F87//无功总加组8
//		break;
//	default:
//		break;
//	}
//	return usDI;
//}

/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : DIInfo.c
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-02-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"
//��ȡ���ֵ���ṩ��ֵ����Ϣ����Ϊ�ն˱������������ṩ�ܵ��������ݣ��������ṩ�����ʵ���
STDIINFO Main_DI_Data_Query_Manage(unsigned short usDI)
{
	STDIINFO stRetDIInfo;
	ClearDIInfo( &stRetDIInfo );

	if( usDI/0x100==0x90 )
	{
		switch( usDI )
		{
		case CON_DI_TYPE_MPDIRPOWTOTAL:						//0x9010�����й��ܵ���
		case CON_DI_TYPE_MPDIRPOWPLUS:						//0x9011�����й������
		case CON_DI_TYPE_MPDIRPOWAPEX:						//0x9012�����й������
		case CON_DI_TYPE_MPDIRPOWFLAT:						//0x9013�����й�ƽ����
		case CON_DI_TYPE_MPDIRPOWVALE:						//0x9014�����й��ȵ���
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,		
				4,0,					CON_DI_TYPE_MPDIRPOW_BLOCK);
			break;
		case CON_DI_TYPE_MPDIRPOW_BLOCK:					//0x901F�����й��������ݿ�
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				20,	0,					CON_DI_TYPE_MPDIRPOW_BLOCK);
			break;
			///////////////////////////////////////////////////////////////////////////////
		case CON_DI_TYPE_MPUNDIRPOWTOTAL:					//0x9020�����й��ܵ���
		case CON_DI_TYPE_MPUNDIRPOWPLUS:					//0x9021�����й������
		case CON_DI_TYPE_MPUNDIRPOWAPEX:					//0x9022�����й������
		case CON_DI_TYPE_MPUNDIRPOWFLAT:					//0x9023�����й�ƽ����
		case CON_DI_TYPE_MPUNDIRPOWVALE:					//0x9024�����й��ȵ���
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,					CON_DI_TYPE_MPUNDIRPOW_BLOCK);
			break;
		case CON_DI_TYPE_MPUNDIRPOW_BLOCK: 					//0x902F�����й��������ݿ�
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
		case CON_DI_TYPE_MPDIRVARTOTAL:						//0x9110//�����޹��ܵ���
		case CON_DI_TYPE_MPDIRVARPLUS:						//0x9111//�����޹������
		case CON_DI_TYPE_MPDIRVARAPEX:						//0x9112//�����޹������
		case CON_DI_TYPE_MPDIRVARFLAT:						//0x9113//�����޹�ƽ����
		case CON_DI_TYPE_MPDIRVARVALE:						//0x9114//�����޹��ȵ���
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,					CON_DI_TYPE_MPDIRVAR_BLOCK);
			break;
		case CON_DI_TYPE_MPDIRVAR_BLOCK:					//0x911F//�����޹��������ݿ�
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE, CON_DI_OPERATE_ENABLE,	
				20,0,					CON_DI_TYPE_MPDIRVAR_BLOCK);
			break;
		case CON_DI_TYPE_MPUNDIRVARTOTAL:					//0x9120//�����޹��ܵ���
		case CON_DI_TYPE_MPUNDIRVARPLUS:					//0x9121//�����޹������
		case CON_DI_TYPE_MPUNDIRVARAPEX:					//0x9122//�����޹������
		case CON_DI_TYPE_MPUNDIRVARFLAT:					//0x9123//�����޹�ƽ����
		case CON_DI_TYPE_MPUNDIRVARVALE:					//0x9124//�����޹��ȵ���
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,					CON_DI_TYPE_MPUNDIRVAR_BLOCK);
			break;
		case CON_DI_TYPE_MPUNDIRVAR_BLOCK:					//0x912F//�����޹��������ݿ�
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE, CON_DI_OPERATE_ENABLE,	
				20,0,					CON_DI_TYPE_MPUNDIRVAR_BLOCK);
			break;
		case CON_DI_TYPE_MPVAR1:							//0x9130һ�����޹��ܵ���
		case CON_DI_TYPE_MPVAR1_FEE1:						//0x9131����1
		case CON_DI_TYPE_MPVAR1_FEE2:						//0x9132����2
		case CON_DI_TYPE_MPVAR1_FEE3:						//0x9133����3
		case CON_DI_TYPE_MPVAR1_FEE4:						//0x9134����4
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
		case CON_DI_TYPE_MPVAR4:							//0x9140�������޹��ܵ���
		case CON_DI_TYPE_MPVAR4_FEE1:						//0x9141����1
		case CON_DI_TYPE_MPVAR4_FEE2:						//0x9142����2
		case CON_DI_TYPE_MPVAR4_FEE3:						//0x9143����3
		case CON_DI_TYPE_MPVAR4_FEE4:						//0x9144����4
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
		case CON_DI_TYPE_MPVAR2:							//0x9150�������޹��ܵ���
		case CON_DI_TYPE_MPVAR2_FEE1:						//0x9151����1
		case CON_DI_TYPE_MPVAR2_FEE2:						//0x9152����2
		case CON_DI_TYPE_MPVAR2_FEE3:						//0x9153����3
		case CON_DI_TYPE_MPVAR2_FEE4:						//0x9154����4
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
		case CON_DI_TYPE_MPVAR3:							//0x9160�������޹��ܵ���
		case CON_DI_TYPE_MPVAR3_FEE1:						//0x9161����1
		case CON_DI_TYPE_MPVAR3_FEE2:						//0x9162����2
		case CON_DI_TYPE_MPVAR3_FEE3:						//0x9163����3
		case CON_DI_TYPE_MPVAR3_FEE4:						//0x9164����4
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
		case CON_DI_TYPE_MPDIRPOWTOTAL_LM:						//0x9410�����й��ܵ��ܣ����£�
		case CON_DI_TYPE_MPDIRPOWPLUS_LM:						//0x9411�����й�����ܣ����£�
		case CON_DI_TYPE_MPDIRPOWAPEX_LM:						//0x9412�����й�����ܣ����£�
		case CON_DI_TYPE_MPDIRPOWFLAT_LM:						//0x9413�����й�ƽ���ܣ����£�
		case CON_DI_TYPE_MPDIRPOWVALE_LM:						//0x9414�����й��ȵ��ܣ����£�
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,					CON_DI_TYPE_MPDIRPOW_LM_BLOCK);
			break;
		case CON_DI_TYPE_MPDIRPOW_LM_BLOCK:						//0x941F�����й��������ݿ飨���£�
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				20,0,					CON_DI_TYPE_MPDIRPOW_LM_BLOCK);
			break;
			///////////////////////////////////////////////////////////////////////////////////
		case CON_DI_TYPE_MPUNDIRPOWTOTAL_LM:					//0x9420�����й��ܵ��ܣ����£�
		case CON_DI_TYPE_MPUNDIRPOWPLUS_LM:						//0x9421�����й�����ܣ����£�
		case CON_DI_TYPE_MPUNDIRPOWAPEX_LM:						//0x9422�����й�����ܣ����£�
		case CON_DI_TYPE_MPUNDIRPOWFLAT_LM:						//0x9423�����й�ƽ���ܣ����£�
		case CON_DI_TYPE_MPUNDIRPOWVALE_LM:						//0x9424�����й��ȵ��ܣ����£�
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,						CON_DI_TYPE_MPUNDIRPOW_LM_BLOCK);
			break;
		case CON_DI_TYPE_MPUNDIRPOW_LM_BLOCK: 					//0x942F�����й��������ݿ飨���£�
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
		case CON_DI_TYPE_MPDIRVARTOTAL_LM:						//0x9510�����޹��ܵ��ܣ����£�
		case CON_DI_TYPE_MPDIRVARPLUS_LM:						//0x9511�����޹�����ܣ����£�
		case CON_DI_TYPE_MPDIRVARAPEX_LM:						//0x9512�����޹�����ܣ����£�
		case CON_DI_TYPE_MPDIRVARFLAT_LM:						//0x9513�����޹�ƽ���ܣ����£�
		case CON_DI_TYPE_MPDIRVARVALE_LM:						//0x9514�����޹��ȵ��ܣ����£�
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,					CON_DI_TYPE_MPDIRVAR_LM_BLOCK);
			break;
		case CON_DI_TYPE_MPDIRVAR_LM_BLOCK:						//0x951F�����޹��������ݿ飨���£�
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				20,0,					CON_DI_TYPE_MPDIRVAR_LM_BLOCK);
			break;
			///////////////////////////////////////////////////////////////////////////////////
		case CON_DI_TYPE_MPUNDIRVARTOTAL_LM:					//0x9520�����޹��ܵ��ܣ����£�
		case CON_DI_TYPE_MPUNDIRVARPLUS_LM:						//0x9521�����޹�����ܣ����£�
		case CON_DI_TYPE_MPUNDIRVARAPEX_LM:						//0x9522�����޹�����ܣ����£�
		case CON_DI_TYPE_MPUNDIRVARFLAT_LM:						//0x9523�����޹�ƽ���ܣ����£�
		case CON_DI_TYPE_MPUNDIRVARVALE_LM:						//0x9524�����޹��ȵ��ܣ����£�
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				4,0,						CON_DI_TYPE_MPUNDIRVAR_LM_BLOCK);
			break;
		case CON_DI_TYPE_MPUNDIRVAR_LM_BLOCK: 					//0x952F�����޹��������ݿ飨���£�
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_DISABLE,	CON_DI_OPERATE_ENABLE,	
				20,0,						CON_DI_TYPE_MPUNDIRVAR_LM_BLOCK);
			break;
		case CON_DI_TYPE_MPVAR1_LM:							//0x9530//һ�����޹��ܵ��ܣ����£�
		case CON_DI_TYPE_MPVAR1_LM_FEE1:					//0x9531//����1
		case CON_DI_TYPE_MPVAR1_LM_FEE2:					//0x9532//����2
		case CON_DI_TYPE_MPVAR1_LM_FEE3:					//0x9533//����3
		case CON_DI_TYPE_MPVAR1_LM_FEE4:					//0x9534//����4
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
		case CON_DI_TYPE_MPVAR4_LM:							//0x9540//�������޹��ܵ��ܣ����£�
		case CON_DI_TYPE_MPVAR4_LM_FEE1:					//0x9541//����1
		case CON_DI_TYPE_MPVAR4_LM_FEE2:					//0x9542//����2
		case CON_DI_TYPE_MPVAR4_LM_FEE3:					//0x9543//����3
		case CON_DI_TYPE_MPVAR4_LM_FEE4:					//0x9544//����4
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
		case CON_DI_TYPE_MPVAR2_LM:							//0x9550//�������޹��ܵ��ܣ����£�
		case CON_DI_TYPE_MPVAR2_LM_FEE1:					//0x9551//����1
		case CON_DI_TYPE_MPVAR2_LM_FEE2:					//0x9552//����2
		case CON_DI_TYPE_MPVAR2_LM_FEE3:					//0x9553//����3
		case CON_DI_TYPE_MPVAR2_LM_FEE4:					//0x9554//����4
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
		case CON_DI_TYPE_MPVAR3_LM:							//0x9560//�������޹��ܵ��ܣ����£�
		case CON_DI_TYPE_MPVAR3_LM_FEE1:					//0x9561//����1
		case CON_DI_TYPE_MPVAR3_LM_FEE2:					//0x9562//����2
		case CON_DI_TYPE_MPVAR3_LM_FEE3:					//0x9563//����3
		case CON_DI_TYPE_MPVAR3_LM_FEE4:					//0x9564//����4
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
		case CON_DI_TYPE_MPDIRPOWMAXDEMAND:						//0xA010�����й����������
		case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE1:				//0xA011����1
		case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE2:				//0xA012����2
		case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE3:				//0xA013����3
		case CON_DI_TYPE_MPDIRPOWMAXDEMAND_FEE4:				//0xA014����4
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
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND:					//0xA020//�����й����������
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE1:				//0xA021//����1
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE2:				//0xA022//����2
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE3:				//0xA023//����3
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_FEE4:				//0xA024//����4
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
		case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM:				//0xA410//�����й���������������£�
		case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE1:			//0xA411//����1
		case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE2:			//0xA412//����2
		case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE3:			//0xA413//����3
		case CON_DI_TYPE_MPDIRPOWMAXDEMAND_LM_FEE4:			//0xA414//����4
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
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM:				//0xA420//�����й���������������£�
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE1:		//0xA421//����1
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE2:		//0xA422//����2
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE3:		//0xA423//����3
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMAND_LM_FEE4:		//0xA424//����4
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
		case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME:					//0xB010//�����й��������������ʱ��
		case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE1:			//0xB011//����1
		case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE2:			//0xB012//����2
		case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE3:			//0xB013//����3
		case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_FEE4:			//0xB014//����4
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
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME:				//0xB020//�����й��������������ʱ��
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE1:		//0xB021//����1
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE2:		//0xB022//����2
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE3:		//0xB023//����3
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_FEE4:		//0xB024//����4
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
		case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM:			//0xB410//�����й��������������ʱ�䣨���£�
		case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE1:		//0xB411//����1
		case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE2:		//0xB412//����2
		case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE3:		//0xB413//����3
		case CON_DI_TYPE_MPDIRPOWMAXDEMANDTIME_LM_FEE4:		//0xB414//����4
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
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM:		////�����й��������������ʱ�䣨���£�
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE1:	//	0xB421//����1
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE2:	//	0xB422//����2
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE3:	//	0xB423//����3
		case CON_DI_TYPE_MPUNDIRPOWMAXDEMANDTIME_LM_FEE4:	//	0xB424//����4
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
		case CON_DI_TYPE_MPVOLA:								//0xB611A���ѹ
		case CON_DI_TYPE_MPVOLB:								//0xB612B���ѹ
		case CON_DI_TYPE_MPVOLC:								//0xB613C���ѹ
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				2,0,					usDI);
			break;
		case CON_DI_TYPE_MPVOL_BLOCK:							//0xB61F��ѹ���ݿ�
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				6,0,					CON_DI_TYPE_MPVOL_BLOCK);
			break;
			/////////////////////////////////////////////////////////////////
		case CON_DI_TYPE_MPCURA:								//0xB621A�����
		case CON_DI_TYPE_MPCURB:								//0xB622B�����
		case CON_DI_TYPE_MPCURC:								//0xB623C����� 
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				3,0,					usDI);
			break;
		case CON_DI_TYPE_MPCUR_BLOCK:							//0xB62F�������ݿ�
			SetDIInfo(&stRetDIInfo,
				CON_DI_HAVE,									
				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
				9,0,					CON_DI_TYPE_MPCUR_BLOCK);
			break;
			//////////////////////////////////////////////////////////////////////////
		case CON_DI_TYPE_MPINSTANTPOW:						//0xB630//˲ʱ�й�����
		case CON_DI_TYPE_MPINSTANTPOWA:						//0xB631//A���й�����
		case CON_DI_TYPE_MPINSTANTPOWB:						//0xB632//B���й�����
		case CON_DI_TYPE_MPINSTANTPOWC:						//0xB633//C���й�����
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
		case CON_DI_TYPE_MPINSTANTVAR:						//0xB640//˲ʱ�޹�����
		case CON_DI_TYPE_MPINSTANTVARA:						//0xB641//A���޹�����
		case CON_DI_TYPE_MPINSTANTVARB:						//0xB642//B���޹�����
		case CON_DI_TYPE_MPINSTANTVARC:						//0xB643//C���޹�����
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
		case CON_DI_TYPE_MPPOWRATE:							//0xB650//�ܹ�������
		case CON_DI_TYPE_MPPOWRATEA:						//0xB651//A�๦������
		case CON_DI_TYPE_MPPOWRATEB:						//0xB652//B�๦������
		case CON_DI_TYPE_MPPOWRATEC:						//0xB653//C�๦������
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
		case CON_DI_TYPE_MPPOWCONST:							//0xC030//�й�����
		case CON_DI_TYPE_MPVARCONST:							//0xC031//�޹�����
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
		case CON_DI_TYPE_MPFEERATE1:							//0xC331	//����1
		case CON_DI_TYPE_MPFEERATE2:							//0xC332	//����2
		case CON_DI_TYPE_MPFEERATE3:							//0xC333	//����3
		case CON_DI_TYPE_MPFEERATE4:							//0xC334	//����4
		case CON_DI_TYPE_MPFEERATE5:							//0xC335	//����5
		case CON_DI_TYPE_MPFEERATE6:							//0xC336	//����6
		case CON_DI_TYPE_MPFEERATE7:							//0xC337	//����7
		case CON_DI_TYPE_MPFEERATE8:							//0xC338	//����8
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
//azh 170420 ����ռ䲻��
//		case CON_DI_TYPE_SUMINSTANTPOW1:						//0x8F50//�й������ܼ���1
//		case CON_DI_TYPE_SUMINSTANTPOW2:						//0x8F51//�й������ܼ���2
//		case CON_DI_TYPE_SUMINSTANTPOW3:						//0x8F52//�й������ܼ���3
//		case CON_DI_TYPE_SUMINSTANTPOW4:						//0x8F53//�й������ܼ���4
//		case CON_DI_TYPE_SUMINSTANTPOW5:						//0x8F54//�й������ܼ���5
//		case CON_DI_TYPE_SUMINSTANTPOW6:						//0x8F55//�й������ܼ���6
//		case CON_DI_TYPE_SUMINSTANTPOW7:						//0x8F56//�й������ܼ���7
//		case CON_DI_TYPE_SUMINSTANTPOW8:						//0x8F57//�й������ܼ���8
//			SetDIInfo(&stRetDIInfo,
//				CON_DI_HAVE,									
//				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
//				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
//				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
//				4,0,					usDI);
//			break;
//		case CON_DI_TYPE_SUMINSTANTVAR1:					//0x8F60//�޹������ܼ���1
//		case CON_DI_TYPE_SUMINSTANTVAR2:					//0x8F61//�޹������ܼ���2
//		case CON_DI_TYPE_SUMINSTANTVAR3:					//0x8F62//�޹������ܼ���3
//		case CON_DI_TYPE_SUMINSTANTVAR4:					//0x8F63//�޹������ܼ���4
//		case CON_DI_TYPE_SUMINSTANTVAR5:					//0x8F64//�޹������ܼ���5
//		case CON_DI_TYPE_SUMINSTANTVAR6:					//0x8F65//�޹������ܼ���6
//		case CON_DI_TYPE_SUMINSTANTVAR7:					//0x8F66//�޹������ܼ���7
//		case CON_DI_TYPE_SUMINSTANTVAR8:					//0x8F67//�޹������ܼ���8
//			SetDIInfo(&stRetDIInfo,
//				CON_DI_HAVE,									
//				CON_DI_TYPE_DATA,		CON_DI_MPTYPE_NORMAL,	
//				CON_DI_OPERATE_INVALID, CON_DI_SETPURVIEW_ADMIN,
//				CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_ENABLE,	
//				4,0,					usDI);
//			break;
//		case CON_DI_TYPE_SUMPOW1:							//0x8F70//�й��ܼ���1
//		case CON_DI_TYPE_SUMPOW2:							//0x8F71//�й��ܼ���2
//		case CON_DI_TYPE_SUMPOW3:							//0x8F72//�й��ܼ���3
//		case CON_DI_TYPE_SUMPOW4:							//0x8F73//�й��ܼ���4
//		case CON_DI_TYPE_SUMPOW5:							//0x8F74//�й��ܼ���5
//		case CON_DI_TYPE_SUMPOW6:							//0x8F75//�й��ܼ���6
//		case CON_DI_TYPE_SUMPOW7:							//0x8F76//�й��ܼ���7
//		case CON_DI_TYPE_SUMPOW8:							//0x8F77//�й��ܼ���8
//		case CON_DI_TYPE_SUMVAR1:						//0x8F80//�޹��ܼ���1
//		case CON_DI_TYPE_SUMVAR2:						//0x8F81//�޹��ܼ���2
//		case CON_DI_TYPE_SUMVAR3:						//0x8F82//�޹��ܼ���3
//		case CON_DI_TYPE_SUMVAR4:						//0x8F83//�޹��ܼ���4
//		case CON_DI_TYPE_SUMVAR5:						//0x8F84//�޹��ܼ���5
//		case CON_DI_TYPE_SUMVAR6:						//0x8F85//�޹��ܼ���6
//		case CON_DI_TYPE_SUMVAR7:						//0x8F86//�޹��ܼ���7
//		case CON_DI_TYPE_SUMVAR8:						//0x8F87//�޹��ܼ���8
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
//		usDI=CON_DI_TYPE_SUMINSTANTPOW1;                        //0x8F50//�й������ܼ���1
//		break;
//	case 1:
//		usDI=CON_DI_TYPE_SUMINSTANTPOW2;						//0x8F51//�й������ܼ���2
//		break;
//	case 2:
//		usDI=CON_DI_TYPE_SUMINSTANTPOW3;						//0x8F52//�й������ܼ���3
//		break;
//	case 3:
//		usDI=CON_DI_TYPE_SUMINSTANTPOW4;						//0x8F53//�й������ܼ���4
//		break;
//	case 4:
//		usDI=CON_DI_TYPE_SUMINSTANTPOW5;						//0x8F54//�й������ܼ���5
//		break;
//	case 5:
//		usDI=CON_DI_TYPE_SUMINSTANTPOW6;						//0x8F55//�й������ܼ���6
//		break;
//	case 6:
//		usDI=CON_DI_TYPE_SUMINSTANTPOW7;						//0x8F56//�й������ܼ���7
//		break;
//	case 7:
//		usDI=CON_DI_TYPE_SUMINSTANTPOW8;						//0x8F57//�й������ܼ���8
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
//		usDI=CON_DI_TYPE_SUMINSTANTVAR1;					//0x8F60//�޹������ܼ���1
//		break;
//	case 1:
//		usDI=CON_DI_TYPE_SUMINSTANTVAR2;					//0x8F61//�޹������ܼ���2
//		break;
//	case 2:
//		usDI=CON_DI_TYPE_SUMINSTANTVAR3;					//0x8F62//�޹������ܼ���3
//		break;
//	case 3:
//		usDI=CON_DI_TYPE_SUMINSTANTVAR4;					//0x8F63//�޹������ܼ���4
//		break;
//	case 4:
//		usDI=CON_DI_TYPE_SUMINSTANTVAR5;					//0x8F64//�޹������ܼ���5
//		break;
//	case 5:
//		usDI=CON_DI_TYPE_SUMINSTANTVAR6;					//0x8F65//�޹������ܼ���6
//		break;
//	case 6:
//		usDI=CON_DI_TYPE_SUMINSTANTVAR7;					//0x8F66//�޹������ܼ���7
//		break;
//	case 7:
//		usDI=CON_DI_TYPE_SUMINSTANTVAR8;					//0x8F67//�޹������ܼ���8
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
//		usDI=CON_DI_TYPE_SUMPOW1;							//0x8F70//�й��ܼ���1
//		break;
//	case 1:
//		usDI=CON_DI_TYPE_SUMPOW2;							//0x8F71//�й��ܼ���2
//		break;
//	case 2:
//		usDI=CON_DI_TYPE_SUMPOW3;							//0x8F72//�й��ܼ���3
//		break;
//	case 3:
//		usDI=CON_DI_TYPE_SUMPOW4;							//0x8F73//�й��ܼ���4
//		break;
//	case 4:
//		usDI=CON_DI_TYPE_SUMPOW5;							//0x8F74//�й��ܼ���5
//		break;
//	case 5:
//		usDI=CON_DI_TYPE_SUMPOW6;							//0x8F75//�й��ܼ���6
//		break;
//	case 6:
//		usDI=CON_DI_TYPE_SUMPOW7;							//0x8F76//�й��ܼ���7
//		break;
//	case 7:
//		usDI=CON_DI_TYPE_SUMPOW8;							//0x8F77//�й��ܼ���8
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
//		usDI=CON_DI_TYPE_SUMVAR1;						//0x8F80//�޹��ܼ���1
//		break;
//	case 1:
//		usDI=CON_DI_TYPE_SUMVAR2;						//0x8F81//�޹��ܼ���2
//		break;
//	case 2:
//		usDI=CON_DI_TYPE_SUMVAR3;						//0x8F82//�޹��ܼ���3
//		break;
//	case 3:
//		usDI=CON_DI_TYPE_SUMVAR4;						//0x8F83//�޹��ܼ���4
//		break;
//	case 4:
//		usDI=CON_DI_TYPE_SUMVAR5;						//0x8F84//�޹��ܼ���5
//		break;
//	case 5:
//		usDI=CON_DI_TYPE_SUMVAR6;						//0x8F85//�޹��ܼ���6
//		break;
//	case 6:
//		usDI=CON_DI_TYPE_SUMVAR7;						//0x8F86//�޹��ܼ���7
//		break;
//	case 7:
//		usDI=CON_DI_TYPE_SUMVAR8;						//0x8F87//�޹��ܼ���8
//		break;
//	default:
//		break;
//	}
//	return usDI;
//}

/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : sync_data.c
 Description    : ͨ��zigbeeͨ��ͬ����ѹ���ܱ�����
-------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��    1.00        2011-07-17    azh          the original version
******************************************************************************/
#include    "..\inc\global_config.h"

//------------------------------ ˽�б������� ---------------------------------

//------------------------------ ˽�к������� -------------------------------

//-------------------------------��������-------------------------------------
//ͬ���õ�ǰ�������� ���ܳ���30�� ��Ϊ���ǰ�����һ����֮��ͬ����� ��ǰ����1�����յ�����

//���ڻ�����չ��Լ�Ķ���
//������Ϊ�˷�����֡���� �����ڲ����� 
//DI3=0x99:��������չҪ���⴦��
//DI2=������չ��Լ��id4
//DI1=0:����
//DI0=0:��ǰ DI0=n:��n�������� ��Ӧ��չ��Լ��di6
#if CODE_VER_TYPE==H_METER_PRJ				//��ϸ�ѹ��
#ifdef ZIGBEE_TEST
const U32 sync_minute_data_tab[SYNC_MINUTE_MAX] =
{
    0x99000000,     //��ǰ���ڼ�ʱ�� �˴�Ϊ��չ���ݱ�ʶ
    //0x07000001 0x00 0x09 0x00, ʵ������֡�е���չ��ʶ
    0x0000ff00,     //(��ǰ)����й��������ݿ�
    0x0001ff00,     //(��ǰ)�����й��������ݿ�
    0x0002ff00,     //(��ǰ)�����й��������ݿ�
    0x0003ff00,     //(��ǰ)����޹�1�������ݿ�
    0x0004ff00,     //(��ǰ)����޹�2�������ݿ�
    0x0005ff00,     //(��ǰ)��һ�����޹��������ݿ�
    0x0006ff00,     //(��ǰ)�ڶ������޹��������ݿ�
    0x0007ff00,     //(��ǰ)���������޹��������ݿ�
    0x0008ff00,     //(��ǰ)���������޹��������ݿ�
    0x0009ff00,     //(��ǰ)�������ڵ������ݿ�
    0x000Aff00,     //(��ǰ)�������ڵ������ݿ�
    0x99010000,     //(��ǰ)�����ܡ�A��B��C�������ݿ�   �����ݱ�ʶΪ��չ
    //0x07000001 0x01 0x09 0x00, ʵ������֡�е���չ��ʶ
    0x99020000,     //(��ǰ)A��B��C��������ݿ� �����ݱ�ʶΪ��չ
    //0x07000001 0x02 0x09 0x00, ʵ������֡�е���չ��ʶ

//ͬ���õ�ǰ�������������ʱ��
    0x0101ff00,     //(��ǰ)�����й��������������ʱ�����ݿ�
    0x0102ff00,     //(��ǰ)�����й��������������ʱ�����ݿ�
    0x0103ff00,     //(��ǰ)����޹�1�������������ʱ�����ݿ�
    0x0104ff00,     //(��ǰ)����޹�2�������������ʱ�����ݿ�
    0x0105ff00,     //(��ǰ)��һ�����޹��������������ʱ�����ݿ�
    0x0106ff00,     //(��ǰ)�ڶ������޹��������������ʱ�����ݿ�
    0x0107ff00,     //(��ǰ)���������޹��������������ʱ�����ݿ�
    0x0108ff00,     //(��ǰ)���������޹��������������ʱ�����ݿ�
    0x0109ff00,     //(��ǰ)���������������������ʱ�����ݿ�
    0x010Aff00,     //(��ǰ)���������������������ʱ�����ݿ�
    0x99030000,     //(��ǰ)A���������������ʱ�����ݿ�   �����ݱ�ʶΪ��չ 
    //0x07000001 0x03 0x09 0x00, ʵ������֡�е���չ��ʶ 
    0x99040000,     //(��ǰ)B���������������ʱ�����ݿ�   �����ݱ�ʶΪ��չ
    //0x07000001 0x04 0x09 0x00, ʵ������֡�е���չ��ʶ
    0x99050000,     //(��ǰ)C���������������ʱ�����ݿ�   �����ݱ�ʶΪ��չ
    //0x07000001 0x05 0x09 0x00, ʵ������֡�е���չ��ʶ
};
#endif//ZIGBEE_TEST
#endif//H_METER_PRJ
/******************************************************************************
** ��������: void sync_data_init(void)
** ��������:
** ��ڲ���:
** �� �� ֵ:
**
** ˵    ��: �ϵ��ʼ�����ͬ���õ��Ŀ���״̬������
******************************************************************************/
void sync_data_init (void)
{
    U8 i;

    Dmd_Up_Flg = 0;
    Eng_Up_Flg = 0;

    for(i=0; i<7; i++)
    {
        gMeter_State[i] = 0;//���״̬��
    }
    gucPoweron = 1;//���ϵ��־
}
/******************************************************************************
** ��������: U8 check_zigbee_broadcase_flag(void)
** ��������:
** ��ڲ���:
** �� �� ֵ:
**
** ˵    ��: ���zigbee�ǹ㲥�����ȥ ��ֻͬ��ʱ������ͬ������� ������з�æ
******************************************************************************/
#ifdef ZIGBEE_TEST
U8 check_zigbee_broadcase_flag(void)
{
	U8 i;
	
    for(i=0; i<PAR_HVMETER_ZIGBEE_ADR_LEN; i++)
    {
        if(gMatch_Zigbee_Addr[i] != 0xff)//��Ϊ�ϵ�ʱ�ĳ�ʼ����ȫ0��
        {
            break;
        }
    }
    if(i >= PAR_HVMETER_ZIGBEE_ADR_LEN)
    {
        return 1;//�����ȫFF ������ÿ��ͬ��ʱ�� ����ͬ������Ͳ����� ������з�æ
    }
    else
    {
    	return 0;
    }	
}
//���zigbeeģ����������߱���ģʽ
//azh 171017 ����ʽ��ѹ��GPRSģ�飬�������й�Լ������
//zigbee��ַ�Ͱ󶨵Ĳ�һ��������Ҳ����FF(û��ǰ)��Ҳ����zigbee��Ĺ㲥��ַ����ô��ȥĬ�Ͻ���֧�ֱ������й�Լ
U8 check_zigbee_addr_unmatch(U8 *buf)
{
	U8 i,ret=RET_ERROR;

    if(gucGetZigbeeAddrFlag !=1 )//�����ڰ�zigbee MAC��ַ״̬
    {
        for(i=0; i<PAR_HVMETER_ZIGBEE_ADR_LEN; i++)
        {
            if(gMatch_Zigbee_Addr[i] != buf[i])
            {
                ret = RET_SUCCESS;//�󶨵�zigbee MAC��ַ���յ���zigbee MAC��ַ����
                break;
            }
        }
    }
    return ret;    
}
#endif//ZIGBEE_TEST
/******************************************************************************
** ��������: U8 dis_ctrl_sync (void)
** ��������:
** ��ڲ���:
** �� �� ֵ:
**
** ˵    ��: ���Ի����ʾ��������ʾǰʵʱͬ������
******************************************************************************/
#ifdef LCD_VALID
U8 dis_ctrl_sync (void)
{
    if( ((Dis_Ctrl.di & 0xff0000ff) == 0) || ((Dis_Ctrl.di & 0xff0000ff) == 0x01000000)//��ǰ���������ػᶨʱͬ��
        || ((Dis_Ctrl.di & 0xffffff00) == 0x04000500) //���״̬��
        || (Dis_Ctrl.di == 0x04000101) || (Dis_Ctrl.di == 0x04000102))//��ǰ����ʱ�䱾�ػᶨʱͬ��
    {
       set_di_sync_finish(0);//���ñ����ݱ�ʶ ֱ���ñ�־
       return __FALSE;
    }
	if(CON_CPU_NORM_MODE == cpu_run_mode())//�е�״̬����ϵͳ����
	{
    	assemble_sync_send_645frame(Dis_Ctrl.di);//����ʾ����ʾ���ݱ�ʶ
    }

    return __TRUE;
}
#endif
/******************************************************************************
** ��������: void sync_sec_data(void)
** ��������:
** ��ڲ���:
** �� �� ֵ:
**
** ˵    ��: ÿ������д��ͬ������֡��������ȥ
******************************************************************************/
void sync_sec_data (void)
{
//debug
//  assemble_sync_send_645frame(0x02010100);//A���ѹ
//  assemble_sync_send_645frame(0x02010200);//B���ѹ
//  assemble_sync_send_645frame(0x02010300);//C���ѹ

    assemble_sync_send_645frame(0x040005ff);//���״̬�����ݿ�
//  assemble_sync_send_645frame(0x040001ff);//���ڼ�ʱ��

//  if(gSync_CurEnergy.Sync_Sn >= SYNC_ENG_MAX)
//  {
//      gSync_CurEnergy.Sync_Sn = 0;
//  }
//  assemble_sync_send_645frame(sync_cur_energy_tab[gSync_CurEnergy.Sync_Sn]);//��ǰĳ��������ݿ�
//  gSync_CurEnergy.Sync_Sn++;
}
/******************************************************************************
** ��������: void sync_minute_data(U8 sec)
** ��������:
** ��ڲ���:
** �� �� ֵ:
**
** ˵    ��: ÿ���ӵ�0�뿪ʼ��ǰSYNC_MINUTE_MAX����д����ͬ������֡��������ȥ
******************************************************************************/
void sync_minute_data (U8 sec)
{
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
	//azh 171011 ����ʽ��ѹ��ģ�飬��ͨ��ͬ���������󶨵ģ�����ֻҪУ����Ծ�Ҫȥ��
//	#ifdef ZIGBEE_TEST
//	if(check_zigbee_broadcase_flag()==1)
//	{
//		return;	//azh 140517
//	}	
//	#endif//ZIGBEE_TEST
	if(sec==1)//�ڵ�ÿ���ӵ�1��ȥ���������������
	{	
    	assemble_sync_send_645frame(0x99000000);//���ϵ��ʱ�������
    }
#else
    if(sec < SYNC_MINUTE_MAX)
    {
        if(gucPoweron == 1)
        {
            gucPoweron = 0;
            assemble_sync_send_645frame(sync_minute_data_tab[0]);//���ϵ��ʱ�������
        }
        else
        {        
            assemble_sync_send_645frame(sync_minute_data_tab[sec]);//��ǰĳ��ʱ�䡢����������������ʱ�����ݿ�
        }
    }
#endif
/*
    if(gSync_Minute.Sn >= 60)
    {
        gSync_Minute.Sn = 0;
    }
    if(gSync_Minute.Sn < SYNC_MINUTE_MAX)
    {
        assemble_sync_send_645frame(sync_minute_data_tab[gSync_Minute.Sn]);//��ǰĳ��ʱ�䡢����������������ʱ�����ݿ�
    }
    gSync_Minute.Sn++;
*/
}
/******************************************************************************
** ��������: void sync_hour_data(U8 sec)
** ��������:
** ��ڲ���:
** �� �� ֵ:
**
** ˵    ��: ��дСʱͬ������֡��������ȥ ÿСʱ�ĵ�1���ӵ�30����SYNC_HOUR_MAX��ͬ������
******************************************************************************/
#if CODE_VER_TYPE==H_METER_PRJ				//��ϸ�ѹ��
void sync_hour_data (U8 sec)
{
    if((sec >= 31) && (sec < (30 + SYNC_HOUR_MAX)))//�ӵ�30�뿪ʼͬ��ÿСʱ������ ����Ҫȥ����һ��ʱ�����ݴ�31��ʼ
    {
        assemble_sync_send_645frame(sync_minute_data_tab[sec-30]+1);//��һ������(di0=1)ĳ��ʱ�䡢����������������ʱ�����ݿ�
    }
}
#endif
/******************************************************************************
** ��������: S8 assemble_sync_send_645frame(U32 di)
** ��������:
** ��ڲ���:
** �� �� ֵ:
**
** ˵    ��: ������������ݱ�ʶ���645����ͬ������֡
******************************************************************************/
S8 assemble_sync_send_645frame (U32 di)
{
    U8  i;
//  U8  adr[6];
    U8  mem_type;
    U8  *p_send;
    U8  *mem_ptr;

/////����debug ͸������
//return __FALSE;
#ifdef ZIGBEE_TEST
    if(gucZigbee_Disable_Flag == CON_ZIGBEE_DISABLE)//�ֶ������ֹzigbeeͬ��
    {
        return __FALSE;
    }    
//azh 171011 ����ʾ�������ѱ����ܣ�����ʽ��ѹ��GPRSģ��ֻ�������ַ��ʱ���ȥ��zigbeeģ���ַ
    #ifdef LCD_VALID
    	if((__FALSE == check_match_valid()) || (CON_SEARCH_RUN_STATE == Dis_Search_Ctrl.state))//���ƥ���ַ���� ���ᷢͬ��֡
    	{
    		return __FALSE;
    	}
    #else//����ʽ��ѹ��GPRSģ�飬������ʾ��������ֵ�����ƶ�ʽ�õ�GPRSģ��
    	if(__FALSE == check_match_valid())//���ƥ���ַ���� ���ᷢͬ��֡
    	{
    		return __FALSE;
    	}
    #endif
#endif
    for(i=0; i<10; i++)
    {
        mem_ptr = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR16);
        if(NULL != mem_ptr)
        {
            break;
        }
        os_dly_wait(2);
    }
    if(i >= 10)
    {
    	setmemgetnullid(GET_MEM_ERR0, gusCurMemErrFunName);	 
    	return __FALSE;
    }
    p_send = mem_ptr + DATA_OFFSET;

    i = 0;
    p_send[i++] = 0x68;
    //memcpy(p_send+i, adr, 6);

    //debug!!
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
	#ifdef ZIGBEE_TEST//azh 171111 ����ʽ��ѹ��GPRSģ��
    memcpy(p_send+i, gMatch_HVMeter_Addr, 6);
	#else//�ƶ�ʽ���ڲ�ͨ�ſ�
	memset(p_send+i, 0xaa, 6);
	#endif//ZIGBEE_TEST

#else
    memcpy(p_send+i, gMatch_HVMeter_Addr, 6);
#endif
    i += 6;

    p_send[i++] = 0x68;
    p_send[i++] = 0x11;
	if((di & 0xff000000) == 0x99000000)
	{
		p_send[i++] = 0x07;//len

	    p_send[i++] = 0x01 + 0x33;//��չ��ʶ
	    p_send[i++] = 0x00 + 0x33;
	    p_send[i++] = 0x00 + 0x33;
	    p_send[i++] = 0x07 + 0x33;
	    p_send[i++] = ((di>>16)&0xff) + 0x33;//��չ�����
	    p_send[i++] = 0x09 + 0x33;//������ʾ�ն�
	    p_send[i++] = (di&0xff) + 0x33;//0:��ǰ n:��n������
	}
	else
	{
	    p_send[i++] = 0x04;//len
	    p_send[i++] = (di&0xff) + 0x33;
	    p_send[i++] = ((di>>8)&0xff) + 0x33;
	    p_send[i++] = ((di>>16)&0xff) + 0x33;
	    p_send[i++] = ((di>>24)&0xff) + 0x33;
    }
    p_send[i++] = cal_frmcs(p_send, i);
    p_send[i++] = 0x16;

    //debug
    //memset(gAim_Addr, 0xff, 8);
//    gAim_Addr[0] = 0xFA;
//    gAim_Addr[1] = 0x37;
//    gAim_Addr[2] = 0x4E;
//    gAim_Addr[3] = 0x00;
//    gAim_Addr[4] = 0x00;
//    gAim_Addr[5] = 0x6F;
//    gAim_Addr[6] = 0x0D;
//    gAim_Addr[7] = 0x00;

    memcpy(&mem_ptr[ZIGBEE_RET_ADDR_OFFSET], gMatch_Zigbee_Addr, 8);//zigbee������Ŀ���ַ
//    send_protocol_frame(mem_ptr, ZIGBEE_CHL, i, MEM_256_TYPE);
    send_wait_rev_frame(mem_ptr, ZIGBEE_CHL, i, MEM_256_TYPE, CON_TER_SYNFLASH);
    return __TRUE;

}
/******************************************************************************
    End Of File
******************************************************************************/
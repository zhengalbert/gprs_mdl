/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name    : timing.c
 Description  : ��ʱ��������
-------------------------------------------------------------------------------
 Modification History:
// <No.>  <version>   <time>    <author>    <contents>
   2��
   1��    1.00      2011-07-28  azh      the original version
******************************************************************************/
#include  "..\inc\global_config.h"


extern U16 os_time;                                     //ʹ��ϵͳ����
//------------------------------ ˽�б������� ---------------------------------

//------------------------------ ˽�к������� -------------------------------
//static  void    system_timer (void);
//static  U32     get_max_second_index (void);
//static  void    record_power_outages_event (void);
#if CODE_VER_TYPE==H_METER_PRJ				//��ϸ�ѹ��
static  void reflash_exrom_data(U8 sec_tag);
#endif
/******************************************************************************
** ��������: void rollback_flash_flag (void)
** ��������: zigbee����ͨ�ŵ��շ�ָʾ
**           ����zigbee���ͺ���� 500ms
**           ����zigbee�����̵���
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
//#ifdef ZIGBEE_TEST
//void zigbee_led_flash (void)
//{
//    if(zigbee_led_time[0] > 0)
//    {
//        zigbee_led_time[0]--;//���̵���500ms
//        WLS_GLED_ON();
//    }
//    else
//    {
//        WLS_GLED_OFF();
//    }
//    if(zigbee_led_time[1] > 0)
//    {
//        zigbee_led_time[1]--;//�������500ms
//        WLS_RLED_ON();
//    }
//    else
//    {
//        WLS_RLED_OFF();
//    }
//}
//#endif

//ģ��汾 ���صư�������ں��ڲ�ͨ�ſ�	
//#ifdef GPRS_MODULE_PRJ
void local_led_flash (void)
{
    if(local_led_time[0] > 0)
    {
        local_led_time[0]--;//���̵���500ms
        WLS_GLED_ON();
    }
    else
    {
        WLS_GLED_OFF();
    }
    if(local_led_time[1] > 0)
    {
        local_led_time[1]--;//�������500ms
        WLS_RLED_ON();
    }
    else
    {
        WLS_RLED_OFF();
    }
}
//#endif  
/******************************************************************************
 Function name:  __task void timing_task (void)
 Author       :
 Description  : run per 100ms
 Input        :  None
 Return       :  None
���нӵ�/? ÿ�붼���Լ��4�����±���
 *****************************************************************************/
__task void timing_task (void)
{
    U32 loop_cnt = 0;
    U8  msec_cnt = 0;
    U8  sec_cnt = 0;

#ifdef ZIGBEE_TEST
    U16 tmp;                                            //�˴���U8����ʡ�ڴ�, �ܿ��ܴ���Ч�ʸ���, ����U16��Ϊ��os_timeһ��
#endif
//    Second_Index = 0;                           //�ϵ��ӡ�0���뿪ʼ
//    reflash_system_RTC();                       //��ʼ��ʵʱʱ��

//    Power_Up_Record_Delay = 0;
 ////////   get_match_addr();//azh 120516

    PowOn_Para_Init_All();//�ϵ��ʼ����
    Init_Main_Comm();//azh
    MC55_Init();
//
#ifdef LCD_VALID
    lcd_reset();
    lcd_init(0);
#endif    
#ifdef HARD_RTC
    sys_rtc_init();
#endif

#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//����CPU_GPRSģ��
    init_search_data();
    get_flash_default_flag();
#endif
    file_read(ENG_STORE_START_ADDR, gEngArray, ENG_RAM_ARRAY_MAX);	//�˾�����main.c��line291����ʱ���hardfault����
	RunData_Init();
	CTask_Proc_Init();//azh

    Alarm_Init(CON_PARA_INIT_POW_ON);
    
    DataProc_Stat_TerResetNum_Inc();            //��¼�ն˸�λ����

    for(;;)
    {
        sign_task_schedule(TIMING_TASK_PRIO);
#ifdef PD_MANAGE_VER12
        batt_charge_mange();    //������V1.2����4.8V��س�����
#else
        battery_charge_mange(); //����V1.3������������ѹ��������2.4V��4.8V��س�����
#endif
        before_pd_pwr_manage();
/*
    if(1 == is_run_position())              //���������ڵ���
    {
      SET_TEST_LED_REV;
    }
*/
        PIN_Timer();                  //azh ԭ�������������10ms���õ�
        AT_Timer();

        if((loop_cnt % 5) == 0)             //GPRSʱ�����
        {
            HeartBeat_Manage();             //��������
            MC55_Comm_Timer();              //azh 110928 500ms����һ��
            DATALED_CLR;
        }

        if(msec_cnt == 0)                //ÿ���Ӷ�ʱ
        {
            msec_cnt = 0;
            sec_cnt++;
            if(!(sec_cnt % 60))
            {
                sec_cnt = 0;
                Min_Cnt++;
#ifdef HARD_RTC
                check_sys_rtc();
#endif
            }
#if CODE_VER_TYPE==H_METER_PRJ				//��ϸ�ѹ��
            reflash_exrom_data(sec_cnt);  //������ݶ�ʱˢ�´���
#endif
      //ÿ����ͬ����ʱ���Ժ� ÿ���Լ�����(Ӳ����Ƭ��ʱ�Ӷ�������
            if(software_rtc_run())                          //��������RTC, ��������Ҫ����
            {
                update_date();                              //��������
            }
//            if(Calendar.Sec == 0)         //��ÿ���ӵĵ�0�룬�������Ч����ʱ���ж�
//            {
//
//            }
//            else if(Calendar.Sec == 3)            //��ÿ���ӵĵ�5��������ǰ��������¼
//            {
//
//            }
//            record_power_outages_event(); //��¼һ�����µ��¼�
//            system_timer();
#ifdef ZIGBEE_TEST
            if(0 != GangMarkTime)                       //�нӵ�/?
            {
                tmp = os_time - GangMarkTime;
                tmp /= (1000 / 10);//SYS_TICK);         //Tick�ú��Ǹ���ϰ��
                if((0 != tmp) && ((tmp % 2) == 0))
                {
                    send_identity_frame();              //����һ�α���
                }
                if(tmp >= 8)                            //�а���˵����3����
                {
                    GangMarkTime = 0;                   //�Ժ��ٱ�����
                }
            }
#endif
        }
        else if(msec_cnt == 1)//ÿ��� ��1��100msȥ��
        {
#if CODE_VER_TYPE==H_METER_PRJ				//��ϸ�ѹ��
            sync_sec_data();
#endif
        }
        else if(msec_cnt == 3)//ÿ��� ��3��100msȥ��
        {
#if CODE_VER_TYPE!=WLS_CUR_PRJ			//������߲ɼ���
            sync_minute_data(Calendar.Sec);//ÿ���ӵ�ÿ��ͬ��һ��
#endif
        }
        else if(msec_cnt == 4)//ÿ��� ��4��100msȥ��
        {
#if CODE_VER_TYPE==H_METER_PRJ				//��ϸ�ѹ��
            if(Calendar.Min == 1)//ÿСʱ�ĵ�һ����
            {
                sync_hour_data(Calendar.Sec);//ÿСʱ�ĵ�һ���ӵĺ�30��ͬ����һ����������
            }
#endif
        }

        msec_cnt++;
        if(msec_cnt >= 10)
        {
            msec_cnt = 0;
        }
//azh 171011 ��ͳһ�ĺ���ȥ���Ƶ�
//#ifdef ZIGBEE_TEST
//        zigbee_led_flash();
//#endif

//#ifdef GPRS_MODULE_PRJ
		local_led_flash();
//#endif
        program_package_code();//azh 111223 ������״̬��ѯ
        soft_reset_function();//��ѯ����������λ

        debug_handle_interval();

        MeterCopy_Proc_Check();//130125

        loop_cnt++;
        os_dly_wait(OS_DLY_100MS);
    }
}

/******************************************************************************
 Function name:  void reflash_exrom_data(U8 sec_tag)
 Author       :  rosoon
 Description  :  ��ʱ�������������,���������ͬ��
 Input        :  sec_tag ʱ�꣬��������Ϊʱ����ж���
 Return       :  ��
******************************************************************************/
#if CODE_VER_TYPE==H_METER_PRJ				//��ϸ�ѹ��
void reflash_exrom_data (U8 sec_tag)
{
    U8 *buf = NULL;
    U8 mem_type;
    U16 len;
    U32 addr_base,offset,addr;

    if(0 == (Min_Cnt % DATA_SAVE_TIME))//ÿ���DATA_SAVE_TIME���Ӵ���һ��
    {
        buf = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR25);
        if(NULL == buf)
        {
//          memcpy(gusCurMemErrFunName, "reflash_", 8);
            setmemgetnullid(GET_MEM_ERR25, gusCurMemErrFunName);
            return;           //�����ڴ�ʧ��
        }
        switch(sec_tag)
        {
        case 10://��ǰ����й��ܼ������ʵ��ܴ���
            addr_base = ENG_STORE_START_ADDR;
            offset = 0 * TOTAL_PHASE_ENERGY_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_ENERGY_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC�ȶ�:����ͬ,����ת��
            {
                file_write(addr, buf, len);
            }
            break;
        case 11://��ǰ�����й��ܼ������ʵ��ܴ���
            addr_base = ENG_STORE_START_ADDR;
            offset = 1 * TOTAL_PHASE_ENERGY_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_ENERGY_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC�ȶ�:����ͬ,����ת��
            {
                file_write(addr, buf, len);
            }
            break;
        case 12://��ǰ�����й��ܼ������ʵ��ܴ���
            addr_base = ENG_STORE_START_ADDR;
            offset = 2 * TOTAL_PHASE_ENERGY_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_ENERGY_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC�ȶ�:����ͬ,����ת��
            {
                file_write(addr, buf, len);
            }
            break;
        case 13://��ǰ����޹� 1�������ʵ��ܴ���
            addr_base = ENG_STORE_START_ADDR;
            offset = 3 * TOTAL_PHASE_ENERGY_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_ENERGY_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC�ȶ�:����ͬ,����ת��
            {
                file_write(addr, buf, len);
            }
            break;
        case 14://��ǰ����޹� 2�������ʵ��ܴ���
            addr_base = ENG_STORE_START_ADDR;
            offset = 4 * TOTAL_PHASE_ENERGY_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_ENERGY_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC�ȶ�:����ͬ,����ת��
            {
                file_write(addr, buf, len);
            }
            break;
        case 15://��ǰ��һ�����޹��ܼ������ʵ��ܴ���
            addr_base = ENG_STORE_START_ADDR;
            offset = 5 * TOTAL_PHASE_ENERGY_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_ENERGY_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC�ȶ�:����ͬ,����ת��
            {
                file_write(addr, buf, len);
            }
            break;
        case 16://��ǰ�ڶ������޹��ܼ������ʵ��ܴ���
            addr_base = ENG_STORE_START_ADDR;
            offset = 6 * TOTAL_PHASE_ENERGY_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_ENERGY_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC�ȶ�:����ͬ,����ת��
            {
                file_write(addr, buf, len);
            }
            break;
        case 17://��ǰ���������޹��ܼ������ʵ��ܴ���
            addr_base = ENG_STORE_START_ADDR;
            offset = 7 * TOTAL_PHASE_ENERGY_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_ENERGY_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC�ȶ�:����ͬ,����ת��
            {
                file_write(addr, buf, len);
            }
            break;
        case 18://��ǰ���������޹��ܼ������ʵ��ܴ���
            addr_base = ENG_STORE_START_ADDR;
            offset = 8 * TOTAL_PHASE_ENERGY_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_ENERGY_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC�ȶ�:����ͬ,����ת��
            {
                file_write(addr, buf, len);
            }
            break;
        case 19://��ǰ���������ܼ������ʵ��ܴ���
            addr_base = ENG_STORE_START_ADDR;
            offset = 9 * TOTAL_PHASE_ENERGY_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_ENERGY_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC�ȶ�:����ͬ,����ת��
            {
                file_write(addr, buf, len);
            }
            break;
        case 20://��ǰ���������ܼ������ʵ��ܴ���
            addr_base = ENG_STORE_START_ADDR;
            offset = 10 * TOTAL_PHASE_ENERGY_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_ENERGY_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC�ȶ�:����ͬ,����ת��
            {
                file_write(addr, buf, len);
            }
            break;
        case 21://����������ݿ鴦��
            addr_base = ENG_STORE_START_ADDR;
            offset = ENG_MERGED_PHASE_TOT_LEN;
            addr = addr_base + offset;
            len = ENG_SEPARATE_PHASE_TOT_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC�ȶ�:����ͬ,����ת��
            {
                file_write(addr, buf, len);
            }
            break;
        case 25://�ϡ�����������������ݿ�
            addr_base = ENG_STORE_START_ADDR;
            offset = ENG_MERGED_PHASE_TOT_LEN + ENG_SEPARATE_PHASE_TOT_LEN;
            addr = addr_base + offset;
            len = ENG_ALL_RELATOTP_TOT_LEN;
            memcpy(buf,gEngArray + offset, len);
            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC�ȶ�:����ͬ,����ת��
            {
                file_write(addr, buf, len);
            }
            break;


        case 30://��ǰ�����й����������������ʱ�䴦��
            addr_base = DMD_STORE_START_ADDR;
            offset = 0 * TOTAL_PHASE_DEMAND_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_DEMAND_LEN;
            memcpy(buf,gDmdArray + offset, len);
//            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC�ȶ�:����ͬ,����ת��
//            {
                file_write(addr, buf, len);
//            }
            break;
        case 31://��ǰ�����й����������������ʱ�䴦��
            addr_base = DMD_STORE_START_ADDR;
            offset = 1 * TOTAL_PHASE_DEMAND_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_DEMAND_LEN;
            memcpy(buf,gDmdArray + offset, len);
//            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC�ȶ�:����ͬ,����ת��
//            {
                file_write(addr, buf, len);
//            }
            break;
        case 32://��ǰ����޹� 1�������������ʱ�䴦��
            addr_base = DMD_STORE_START_ADDR;
            offset = 2 * TOTAL_PHASE_DEMAND_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_DEMAND_LEN;
            memcpy(buf,gDmdArray + offset, len);
//            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC�ȶ�:����ͬ,����ת��
//            {
                file_write(addr, buf, len);
//            }
            break;
        case 33://��ǰ����޹� 2�������������ʱ�䴦��
            addr_base = DMD_STORE_START_ADDR;
            offset = 3 * TOTAL_PHASE_DEMAND_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_DEMAND_LEN;
            memcpy(buf,gDmdArray + offset, len);
//            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC�ȶ�:����ͬ,����ת��
//            {
                file_write(addr, buf, len);
//            }
            break;
        case 34://��ǰ��һ�����޹����������������ʱ�䴦��
            addr_base = DMD_STORE_START_ADDR;
            offset = 4 * TOTAL_PHASE_DEMAND_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_DEMAND_LEN;
            memcpy(buf,gDmdArray + offset, len);
//            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC�ȶ�:����ͬ,����ת��
//            {
                file_write(addr, buf, len);
//            }
            break;
        case 35://��ǰ�ڶ������޹����������������ʱ�䴦��
            addr_base = DMD_STORE_START_ADDR;
            offset = 5 * TOTAL_PHASE_DEMAND_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_DEMAND_LEN;
            memcpy(buf,gDmdArray + offset, len);
//            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC�ȶ�:����ͬ,����ת��
//            {
                file_write(addr, buf, len);
//            }
            break;
        case 36://��ǰ���������޹����������������ʱ�䴦��
            addr_base = DMD_STORE_START_ADDR;
            offset = 6 * TOTAL_PHASE_DEMAND_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_DEMAND_LEN;
            memcpy(buf,gDmdArray + offset, len);
//            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC�ȶ�:����ͬ,����ת��
//            {
                file_write(addr, buf, len);
//            }
            break;
        case 37://��ǰ���������޹����������������ʱ�䴦��
            addr_base = DMD_STORE_START_ADDR;
            offset = 7 * TOTAL_PHASE_DEMAND_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_DEMAND_LEN;
            memcpy(buf,gDmdArray + offset, len);
//            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC�ȶ�:����ͬ,����ת��
//            {
                file_write(addr, buf, len);
//            }
            break;
        case 38://��ǰ�����������������������ʱ�䴦��
            addr_base = DMD_STORE_START_ADDR;
            offset = 8 * TOTAL_PHASE_DEMAND_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_DEMAND_LEN;
            memcpy(buf,gDmdArray + offset, len);
//            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC�ȶ�:����ͬ,����ת��
//            {
                file_write(addr, buf, len);
//            }
            break;
        case 39://��ǰ�����������������������ʱ�䴦��
            addr_base = DMD_STORE_START_ADDR;
            offset = 9 * TOTAL_PHASE_DEMAND_LEN;
            addr = addr_base + offset;
            len = TOTAL_PHASE_DEMAND_LEN;
            memcpy(buf,gDmdArray + offset, len);
//            if(((U16)byte_merge1(buf + len - CRC_LEN , 2) == calc_crc16(buf, len - CRC_LEN, 0xffff)))    //CRC�ȶ�:����ͬ,����ת��
//            {
                file_write(addr, buf, len);
//            }
            break;
        case 40://��ǰA��������������޹�1��2���������޹����������������������������ʱ��鴦��

            break;
        case 45://��ǰC��������������޹�1��2���������޹����������������������������ʱ��鴦��

            break;
        case 50://��ǰB��������������޹�1��2���������޹����������������������������ʱ��鴦��

            break;
        default:

            break;
        }
        free_mem_block(buf);//�ͷ��ڴ��
    }
}
#endif
/******************************************************************************
 Function name:  void reflash_system_RTC (void)
 Author       :  snap.gao
 Description  :  ������������һ��ʵʱʱ��
 Input        :  ��
 Return       :  ��
 *****************************************************************************/
/*
void reflash_system_RTC (void)
{
  second_index_2_RTC(Second_Index, &Calendar);        //������������һ��ʵʱʱ��
  update_day_of_week();
  Current_Date = char_2_BCD(Calendar.Date) | (char_2_BCD(Calendar.Month)<<8) | (int_2_BCD(Calendar.Year)<<16);
  Current_Time = char_2_BCD(Calendar.Second) | (char_2_BCD(Calendar.Minute)<<8) | (char_2_BCD(Calendar.Hour)<<16) | (char_2_BCD(Calendar.Day)<<24);
}
*/
/******************************************************************************
 Function name:  void system_timer (void)
 Author       :  snap.gao
 Description  :  Ҫ��ȷ��ÿ���ӵ���һ�Σ�����ϵͳ��ʱ�ӣ���λ��ӡ�0����ʼ����
         ��ؼĴ�����Calendar.Year
               Calendar.Month
               Calendar.Date
               Calendar.Hour
               Calendar.Minute
               Calendar.Second
               Calendar.Day
               Second_Index
 Input        :  ��
 Return       :  ��
 *****************************************************************************/
/*
void system_timer (void)
{
  tsk_lock();
  Second_Index++;
  if(software_rtc_run() == 1)     //����ʱ�䣬����������������
  {
    reflash_system_RTC();       //����ʱ������������һ��ʵʱʱ��
  }
  else
  {
    Current_Time = char_2_BCD(Calendar.Second) | (char_2_BCD(Calendar.Minute)<<8) | (char_2_BCD(Calendar.Hour)<<16) | (char_2_BCD(Calendar.Day)<<24);
  }
  tsk_unlock();
}
*/

/******************************************************************************
 Function name:  U32 get_max_second_index (void)
 Author       :  snap.gao
 Description  :  �ڼ�¼��������������Ѱ��һ�����ֵ
 Input        :  ��
 Return       :  �ҵ������������ֵ������ʱ���ء�0��
 *****************************************************************************/
/*
U32 get_max_second_index (void)
{
  U32 *si, max_si;
  U8  *buf, i;

  buf = _alloc_box(Mem_64_Byte);
  if(buf == NULL)
  {
    return 0;
  }

  max_si = 0;
  read_data(SECOND_INDEX_RECORD, buf, SECOND_INDEX_RECORD_LEN);       //��ȡ��������¼����
  for(i = 0; i < SECOND_INDEX_RECORD_LEN / 4; i++)
  {
    si = (U32*)&buf[i * 4];
    if(*si == 0xffffffff)
    {
      continue;
    }
    max_si = max(*si, max_si);
  }
  _free_box(Mem_64_Byte, buf);            //�ͷ��ڴ��
  return max_si;
}
*/
/******************************************************************************
 Function name:  void record_power_outages_event (void)
 Author       :  snap.gao
 Description  :  ��¼һ�����µ��¼���ֻ��¼POR��BOD�¼����ϵ��¼�����������Чʱ����
 Input        :  ��
 Return       :  ��
 *****************************************************************************/
/*
void record_power_outages_event (void)
{
  U16 point;
  U8  buf[POWER_OUTAGES_RECORD_LEN];
  U32 si;

  if((Reset_Source_Copy == MCU_RST_POR) || (Reset_Source_Copy == MCU_RST_BOD))        //�ж��Ƿ��¼�µ��¼�
  {
    read_data(POWER_OUTAGES_POINT, (U8*)&point, POWER_OUTAGES_POINT_LEN);           //��ȡ���µ��¼�ĵ�ǰָ��
    if((point & ~0x8000) >= POWER_OUTAGES_NUM)
    {
      point = 0;                              //ָ���������Ϊ��0��
    }
    si = get_max_second_index();
    if(si != 0)
    {
      mem_copy(buf, (U8*)&si, 4);
      buf[4] = 0;                             //�µ��¼���ʶ
      write_data(POWER_OUTAGES + (point & 0x7fff) * POWER_OUTAGES_RECORD_LEN, buf, POWER_OUTAGES_RECORD_LEN);     //��¼һ������
      point = get_next_record_point(POWER_OUTAGES_NUM, point);
      write_data(POWER_OUTAGES_POINT, (U8*)&point, POWER_OUTAGES_POINT_LEN);      //ˢ�����µ��¼�ĵ�ǰָ��
    }
    Reset_Source_Copy = 0xff;                   //��ʶ�Ѿ���¼���µ磬������Ҫ��¼�ϵ�
  }
  else if(Reset_Source_Copy != 0xff)
  {
    Reset_Source_Copy = 0;
  }

  if(Second_Index <= VALID_SECOND_INDEX)          //�жϵ�ǰʱ���Ƿ���Ч
  {
    Power_Up_Record_Delay = Second_Index;
    return;
  }

  if(Reset_Source_Copy == 0xff)                   //�ж��Ƿ��¼�ϵ��¼�
  {
    read_data(POWER_OUTAGES_POINT, (U8*)&point, POWER_OUTAGES_POINT_LEN);           //��ȡ���µ��¼�ĵ�ǰָ��
    if((point & ~0x8000) >= POWER_OUTAGES_NUM)
    {
      point = 0;                              //ָ���������Ϊ��0��
    }
    Power_Up_Record_Delay = Second_Index - Power_Up_Record_Delay;
    mem_copy(buf, (U8*)&Power_Up_Record_Delay, 4);
    buf[4] = Reset_Source;                      //�ϵ��¼���ʶ��POR��BOD
    Reset_Source_Copy = 0;
    write_data(POWER_OUTAGES + (point & 0x7fff) * POWER_OUTAGES_RECORD_LEN, buf, POWER_OUTAGES_RECORD_LEN);     //��¼һ������
    point = get_next_record_point(POWER_OUTAGES_NUM, point);
    write_data(POWER_OUTAGES_POINT, (U8*)&point, POWER_OUTAGES_POINT_LEN);          //ˢ�����µ��¼�ĵ�ǰָ��
  }
}
*/
/******************************************************************************
              End Of File
 *****************************************************************************/
/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : event.c
 Description    : �¼���¼��ص�һЩ�ӳ���
 Related Files  : event.h
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��    1.00        2010-09-27    wwllzz          the original version
******************************************************************************/

#include "..\inc\global_config.h"
#include    "..\src\bsp\event_define_d.h"	            //�й��¼���Ķ���

//-------------------------------- �������� -----------------------------------

//------------------------------ ˽�б������� ---------------------------------

//------------------------------ ˽�к������� ---------------------------------
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//����CPU_GPRSģ��





/******************************************************************************
����: void record_energy_zero_event(U8 buf[])
���: buf[]-�㹻106�ֽڵĻ�����, �����ߴ������buf[6..9]
����: ��
����: ���ӵ�������ܴ���, ��дһ����¼, ��������ǰ����
******************************************************************************/
//void record_energy_zero_event (U8 buf[])
//{
//    U32 tmp;
//    U32 i;
//    U8 k[4];
//
//    tmp = 0;                                            //�ȶ���ԭ����
//    file_read(ENERGY_ZERO_COUNT_ADDR, (U8 *)(&tmp), 3);
//    if(bcd_to_bytes((U8 *)(&tmp), 3) == 0)              //ת��Ϊhex, �������Ƿ���Ч
//    {
//        tmp = 0;
//    }
////    get_on_chip_time(buf);                              //��֯��¼, ����ʱ��
//    for(i = 0; i < 6; i++)                              //ϵ�е���, û�зַ��ʵ�, û�������������������
//    {
//        if(i < 2)
//        {
//            file_read(ENG_POSP_00010000_00_ADDR + i * ENG_POSP_00010000_TOT_LEN, k, 4);
//        }
//        else
//        {
//            file_read(ENG_POSP_00010000_00_ADDR + (i + 2) * ENG_POSP_00010000_TOT_LEN, k, 4);
//        }
//        energy_to_format_data(k, buf + 10 + i * 4, 1);
//    }
//    for(i = 0; i < 6; i++)                              //A��
//    {
//        if(i < 2)
//        {
//            file_read(ENG_APOSP_00150000_00_ADDR + i * 4, k, 4);
//        }
//        else
//        {
//            file_read(ENG_APOSP_00150000_00_ADDR + 8 + i * 4, k, 4);
//        }
//        energy_to_format_data(k, buf + 10 + 24 + i * 4, 1);
//    }
//    fill_char(buf + 10 + 24 * 2, 24, GW645_2007_CHAR);  //B��
//    for(i = 0; i < 6; i++)                              //C��
//    {
//        if(i < 2)
//        {
//            file_read(ENG_CPOSP_003D0000_00_ADDR + i * 4, k, 4);
//        }
//        else
//        {
//            file_read(ENG_CPOSP_003D0000_00_ADDR + 8 + i * 4, k, 4);
//        }
//        energy_to_format_data(k, buf + 10 + 24 * 3 + i * 4, 1);
//    }
//    file_write(ENERGY_ZERO_RECORD_ADDR + (tmp & 0xf) * ENERGY_ZERO_RECORD_LEN, buf, ENERGY_ZERO_RECORD_LEN);   //д���¼
//    tmp = bcd_to_bytes((U8 *)(&tmp), 3);                //�ܴ���++
//    tmp++;
//    bytes_to_bcd(tmp, (U8 *)(&tmp), 6);
//    file_write(ENERGY_ZERO_COUNT_ADDR, (U8 *)(&tmp), 3);
//}

/******************************************************************************
����: void record_demand_zero_event(U8 buf[])
���: buf[]-�㹻202�ֽڵĻ�����, �����ߴ������buf[6..9]
����: ��
����: �������������ܴ���, ��дһ����¼, ����ִ����������ǰ����
******************************************************************************/
//void record_demand_zero_event (U8 buf[])
//{
//    U32 tmp;
//    U32 i;
//
//    tmp = 0;                                            //����
//    file_read(EVET_DMDCLR_03300200_00_ADDR, (U8 *)(&tmp), 3);
//    if(bcd_to_bytes((U8 *)(&tmp), 3) == 0)              //���������Ƿ���Ч
//    {
//        tmp = 0;
//    }
////    get_on_chip_time(buf);                              //��֯��¼�ķ���ʱ��
//    for(i = 0; i < 6; i++)                              //û�зַ��ʵ�, û����Ϻ�����
//    {
//        if(i < 2)
//        {
//            file_read(DMD_POSP_01010000_00_ADDR + i * TOTAL_PHASE_DEMAND_LEN, buf + 10 + i * 8, 8);
//        }
//        else
//        {
//            file_read(DMD_POSP_01010000_00_ADDR + (i + 2) * TOTAL_PHASE_DEMAND_LEN, buf + 10 + i * 8, 8);
//        }
//    }
//    for(i = 0; i < 6; i++)                              //A��
//    {
//        if(i < 2)
//        {
//            file_read(DMD_APOSP_01150000_00_ADDR + i * 8, buf + 10 + 48 + i * 8, 8);
//        }
//        else
//        {
//            file_read(DMD_APOSP_01150000_00_ADDR + (i + 2) * 8, buf + 10 + 48 + i * 8, 8);
//        }
//    }
//    fill_char(buf + 10 + 48 * 2, 48, GW645_2007_CHAR);  //B��
//    for(i = 0; i < 6; i++)                              //C��
//    {
//        if(i < 2)
//        {
//            file_read(DMD_CPOSP_013D0000_00_ADDR + i * 8, buf + 10 + 48 * 3 + i * 8, 8);
//        }
//        else
//        {
//            file_read(DMD_CPOSP_013D0000_00_ADDR + (i + 2) * 8, buf + 10 + 48 * 3 + i * 8, 8);
//        }
//    }
//    file_write(EVET_DMDCLR_03300201_00_OFFSET_ADDR + (tmp & 0xf) * EVET_DMDCLR_03300201_00_OFFSET_BLOCKLEN, buf, EVET_DMDCLR_03300201_00_OFFSET_BLOCKLEN);
//    tmp = bcd_to_bytes((U8 *)(&tmp), 3);                //�ܴ���++
//    tmp++;
//    bytes_to_bcd(tmp, (U8 *)(&tmp), 6);
//    file_write(EVET_DMDCLR_03300200_00_ADDR, (U8 *)(&tmp), 3);
//}

/******************************************************************************
����: void record_event_zero_event(U8 buf[])
���: buf[]-�㹻14�ֽڵĻ�����, �����ߴ������buf[6..9], ��ʶ����buf[10..13]
����: ��
����: �������¼�����, ��дһ����¼, ��������¼������
******************************************************************************/
//void record_event_zero_event (U8 buf[])
//{
//    U32 tmp;
//
//    tmp = 0;
//    file_read(EVENT_ZERO_COUNT_ADDR, (U8 *)(&tmp), 3);
//    if(bcd_to_bytes((U8 *)(&tmp), 3) == 0)              //���������Ƿ���Ч
//    {
//        tmp = 0;
//    }
////    get_on_chip_time(buf);                              //��֯��¼�ķ���ʱ��
//    file_write(EVENT_ZERO_RECORD_ADDR + (tmp & 0xf) * EVENT_ZERO_RECORD_LEN, buf, EVENT_ZERO_RECORD_LEN);
//    tmp = bcd_to_bytes((U8 *)(&tmp), 3);                //�ܴ���++
//    tmp++;
//    bytes_to_bcd(tmp, (U8 *)(&tmp), 6);
//    file_write(EVENT_ZERO_COUNT_ADDR, (U8 *)(&tmp), 3);
//}

/******************************************************************************
����: void record_check_time_event(U8 buf[])
���: buf[]-�㹻16�ֽڵĻ�����, �����ߴ��뱣����buf[0..3], Уʱǰʱ��buf[4..9]
����: ��
����: ����Уʱ����, ��һ��Уʱ��¼, ����Уʱ��ɺ����
******************************************************************************/
//void record_check_time_event (U8 buf[])
//{
//    U32 tmp;
//
//    tmp = 0;
//    file_read(CHECK_TIME_COUNT_ADDR, (U8 *)(&tmp), 3);
//    if(bcd_to_bytes((U8 *)(&tmp), 3) == 0)              //���������Ƿ���Ч
//    {
//        tmp = 0;
//    }
////    get_on_chip_time(buf + 10);                         //Уʱ��ʱ��
//    file_write(CHECK_TIME_RECORD_ADDR + (tmp & 0xf) * CHECK_TIME_RECORD_LEN, buf, CHECK_TIME_RECORD_LEN);
//    tmp = bcd_to_bytes((U8 *)(&tmp), 3);                //�ܴ���++
//    tmp++;
//    bytes_to_bcd(tmp, (U8 *)(&tmp), 6);
//    file_write(CHECK_TIME_COUNT_ADDR, (U8 *)(&tmp), 3);
//}

/******************************************************************************
����: void record_table_program_event(U8 buf[])
���: buf[]-�㹻346�ֽڵĻ�����, �����ߴ�����buf[6..9]
����: ��
����: ����ʱ�α��̴���, ��һ����¼, ��Ҫ���޸�ʱ�α�ǰ����
******************************************************************************/
void record_table_program_event (U8 buf[])
{
//    U32 tmp;
//
//    tmp = 0;
//    file_read(TABLE_PROGRAM_COUNT_ADDR, (U8 *)(&tmp), 3);
//    if(bcd_to_bytes((U8 *)(&tmp), 3) == 0)              //���������Ƿ���Ч
//    {
//        tmp = 0;
//    }
////    get_on_chip_time(buf);                              //����ʱ��
//    file_read(FEE_TABLE_ONE1_ADDR, buf + 10, FEE_TABLE_ONE1_LEN * FEE_TABLE_NUM);   // ʱ��+�����ߴ���+��һ��8����ʱ�α�
//    file_write(TABLE_PROGRAM_RECORD_ADDR + (tmp & 0xf) * TABLE_PROGRAM_RECORD_LEN, buf, 10 + FEE_TABLE_ONE1_LEN * FEE_TABLE_NUM);
//    file_read(FEE_TABLE_TWO1_ADDR, buf, FEE_TABLE_ONE1_LEN * FEE_TABLE_NUM);        // �ڶ���8����ʱ�α�, ����buf��
//    file_write(TABLE_PROGRAM_RECORD_ADDR + 10 + FEE_TABLE_ONE1_LEN * FEE_TABLE_NUM + (tmp & 0xf) * TABLE_PROGRAM_RECORD_LEN, buf, FEE_TABLE_ONE1_LEN * FEE_TABLE_NUM);
//    tmp = bcd_to_bytes((U8 *)(&tmp), 3);                //�ܴ���++
//    tmp++;
//    bytes_to_bcd(tmp, (U8 *)(&tmp), 6);
//    file_write(TABLE_PROGRAM_COUNT_ADDR, (U8 *)(&tmp), 3);
}

/******************************************************************************
����: void record_zone_program_event(U8 buf[])
���: buf[]-�㹻94�ֽڵĻ�����, �����ߴ�����buf[6..9]
����: ��
����: ����ʱ�����̴���, ��һ����¼, ��Ҫ���޸�ʱ����ǰ����
******************************************************************************/
void record_zone_program_event (U8 buf[])
{
//    U32 tmp;
//
//    tmp = 0;
//    file_read(ZONE_PROGRAM_COUNT_ADDR, (U8 *)(&tmp), 3);
//    if(bcd_to_bytes((U8 *)(&tmp), 3) == 0)              //���������Ƿ���Ч
//    {
//        tmp = 0;
//    }
////    get_on_chip_time(buf);                              //����ʱ��
//    file_read(ZONE_TABLE_ONE_ADDR, buf + 10, ZONE_TABLE_ONE_LEN);
//    file_read(ZONE_TABLE_TWO_ADDR, buf + 10 + ZONE_TABLE_ONE_LEN, ZONE_TABLE_ONE_LEN);
//    file_write(ZONE_PROGRAM_RECORD_ADDR + (tmp & 0xf) * ZONE_PROGRAM_RECORD_LEN, buf, ZONE_PROGRAM_RECORD_LEN);
//    tmp = bcd_to_bytes((U8 *)(&tmp), 3);                //�ܴ���++
//    tmp++;
//    bytes_to_bcd(tmp, (U8 *)(&tmp), 6);
//    file_write(ZONE_PROGRAM_COUNT_ADDR, (U8 *)(&tmp), 3);
}

/******************************************************************************
����: void record_weekday_program_event(U8 buf[], U32 count_addr)
���: buf[]-�㹻11�ֽڵĻ�����, �����ߴ�����buf[6..9], ������buf[10]
    count_addr-���ֲ�ͬ����, ������������:
    WEEKDAY_PROGRAM_COUNT_ADDR ���������ձ�̴���, ��һ����¼, ��Ҫ���޸�����������ǰ����
    COMB_Y_PROGRAM_COUNT_ADDR �����й���Ϸ�ʽ��̴���, ��һ����¼, ��Ҫ���޸��й���Ϸ�ʽ����ǰ����
    COMB_1_PROGRAM_COUNT_ADDR �����޹���Ϸ�ʽ1��̴���, ��һ����¼, ��Ҫ���޸��޹���Ϸ�ʽ1����ǰ����
    COMB_2_PROGRAM_COUNT_ADDR �����޹���Ϸ�ʽ2��̴���, ��һ����¼, ��Ҫ���޸��޹���Ϸ�ʽ2����ǰ����
����: ��
����: ��Щ�¼���¼�ǳ�С, ����������
******************************************************************************/
//void record_weekday_program_event (U8 buf[], U32 count_addr)
//{
//    U32 tmp;
//
//    tmp = 0;
//    file_read(count_addr, (U8 *)(&tmp), 3);
//    if(bcd_to_bytes((U8 *)(&tmp), 3) == 0)              //���������Ƿ���Ч
//    {
//        tmp = 0;
//    }
////    get_on_chip_time(buf);                              //����ʱ��, ���������
//    file_write(count_addr + 3 + (tmp & 0xf) * 11, buf, 11);
//    tmp = bcd_to_bytes((U8 *)(&tmp), 3);                //�ܴ���++
//    tmp++;
//    bytes_to_bcd(tmp, (U8 *)(&tmp), 6);
//    file_write(count_addr, (U8 *)(&tmp), 3);
//}

/******************************************************************************
����: void record_holiday_program_event(U8 buf[])
���: buf[]-�㹻264�ֽڵĻ�����, �����ߴ�����buf[6..9]
����: ��
����: ���ӽڼ��ձ�̴���, ��һ����¼, ��Ҫ���޸Ľڼ�������ǰ����
******************************************************************************/
void record_holiday_program_event (U8 buf[])
{
//    U32 tmp;
//
//    tmp = 0;
//    file_read(HOLIDAY_PROGRAM_COUNT_ADDR, (U8 *)(&tmp), 3);
//    if(bcd_to_bytes((U8 *)(&tmp), 3) == 0)              //���������Ƿ���Ч
//    {
//        tmp = 0;
//    }
////    get_on_chip_time(buf);                              //����ʱ��
//    file_read(HOLIDAY_TABLE_ADDR, buf + 10, 254);
//    file_write(HOLIDAY_PROGRAM_RECORD_ADDR + (tmp & 0xf) * HOLIDAY_PROGRAM_RECORD_LEN, buf, 10 + 254);
//    file_read(HOLIDAY_TABLE_ADDR + 254, buf + 10, 254);
//    file_write(HOLIDAY_PROGRAM_RECORD_ADDR + 10 + 254 + (tmp & 0xf) * HOLIDAY_PROGRAM_RECORD_LEN, buf + 10, 254);
//    file_read(HOLIDAY_TABLE_ADDR + 254 * 2, buf + 10, 254);
//    file_write(HOLIDAY_PROGRAM_RECORD_ADDR + 10 + 254 * 2 + (tmp & 0xf) * HOLIDAY_PROGRAM_RECORD_LEN, buf + 10, 254);
//    file_read(HOLIDAY_TABLE_ADDR + 254 * 3, buf + 10, 254);
//    file_write(HOLIDAY_PROGRAM_RECORD_ADDR + 10 + 254 * 3 + (tmp & 0xf) * HOLIDAY_PROGRAM_RECORD_LEN, buf + 10, 254);
//    tmp = bcd_to_bytes((U8 *)(&tmp), 3);                //�ܴ���++
//    tmp++;
//    bytes_to_bcd(tmp, (U8 *)(&tmp), 6);
//    file_write(HOLIDAY_PROGRAM_COUNT_ADDR, (U8 *)(&tmp), 3);
}

/******************************************************************************
����: void record_account_day_program_event(U8 buf[])
���: buf[]-�㹻16�ֽڵĻ�����, �����ߴ�����buf[6..9]
����: ��
����: ���ӽ����ձ�̴���, ��һ�������ձ�̼�¼
******************************************************************************/
void record_account_day_program_event (U8 buf[])
{
//    U32 tmp;
//
//    tmp = 0;
//    file_read(ACCOUNT_DAY_PROGRAM_COUNT_ADDR, (U8 *)(&tmp), 3);
//    if(bcd_to_bytes((U8 *)(&tmp), 3) == 0)              //���������Ƿ���Ч
//    {
//        tmp = 0;
//    }
////    get_on_chip_time(buf);                              //����ʱ��, ���������
//    file_read(PAR_FIRACCNT_04000B01_00_ADDR, buf + 10, 6);
//    file_write(ACCOUNT_DAY_PROGRAM_COUNT_ADDR + 3 + (tmp & 0xf) * 16, buf, 16);
//    tmp = bcd_to_bytes((U8 *)(&tmp), 3);                //�ܴ���++
//    tmp++;
//    bytes_to_bcd(tmp, (U8 *)(&tmp), 6);
//    file_write(ACCOUNT_DAY_PROGRAM_COUNT_ADDR, (U8 *)(&tmp), 3);
}

/******************************************************************************
����: void restore_second(void)
���: ��, ȫ��Calendar
����: ��
����: ����ǰ�������ʱ�̱��浽FRAM, BCD��
******************************************************************************/
//void restore_second (void)
//{
//    U8 buf[6];
//
////    get_on_chip_time(buf);
//    file_write(BY_SECOND_RESTORE, buf, 6);              //д��FRAM
//}

/******************************************************************************
����: void save_down_time(void)
���: ��
����: ��
����: ������翪ʼ�ͽ�����¼, ���Կ�ʼ�ͽ�����ͬһʱ�̵ļ�¼, ����λԭ��
    ��λ-��RSID�в����±�ʶ, �������ֳ���, ����¼��/�����¼�
    ����ĸ�λ��ʱû����������ͼ�¼, ֻ�򵥵�ȥ��
    POR-�ϵ縴λ
    EXTR-RESET���ű����͵ĸ�λ, �������ⲿ���Ź���λ
    WDTR-оƬ�ڲ����Ź������ĸ�λ
    BODR-����Դ������ɵĸ�λ
******************************************************************************/
//void save_down_time (void)
//{
//    U8 buf[12];
//    U32 i;
//
//    if(RSID == 0)                                       //�����λ
//    {
//        return;
//    }
//    RSID = 0xf;                                         //��ʱ���������λԭ��
//    file_read(BY_SECOND_RESTORE, buf, 6);
////    get_on_chip_time(buf + 6);
//    for(i = 1; i < 6; i++)
//    {
//        if(buf[i] != buf[i + 6])
//        {
//            break;
//        }
//    }
//    if(i != 6)                                          //��ʼ�ͽ�������ͬһ����(ʱ�̲�ͬ)
//    {
//        i = 0;
//        file_read(EVET_POWDNTIME_03110000_00_ADDR, (U8 *)(&i), 3);//�������
//        if(bcd_to_bytes((U8 *)(&i), 3) == 0)            //���������Ƿ���Ч
//        {
//            i = 0;
//        }
//        file_write(EVET_POWDN_03110001_00_OFFSET_ADDR + (i & 0xf) * 12, buf, 12);
//        i = bcd_to_bytes((U8 *)(&i), 3);                //��������
//        i++;
//        bytes_to_bcd(i, (U8 *)(&i), 6);
//        file_write(EVET_POWDNTIME_03110000_00_ADDR, (U8 *)(&i), 3);
//    }
//}

/******************************************************************************
����: void record_event_open_cover(void)
���: ��
����: ��
����: ��
******************************************************************************/
//void record_event_open_cover (void)
//{
//    U8 oc_flg;
//    U8 i,j,tmp_buf[7]={0xFF,0xFF,0xFF};
//    U32 cnt1,cnt2,addr;
//
///*  ֮ǰ���뽫oc_flg & cnt1 & cnt2���ӳ����ˢ��0xff
//    file_write(EEPROM_GFLG_ADDR, tmp_buf, 1);
//    file_write(EVET_OPNCOVER_03300D00_00_ADDR, tmp_buf, 3);
//    file_write(EVET_OPNTMNBX_03300E00_00_ADDR, tmp_buf, 3);
//*/
//    file_read(EEPROM_GFLG_ADDR, tmp_buf, 1);
//    file_read(EVET_OPNCOVER_03300D00_00_ADDR, tmp_buf + 1, 3);
//    file_read(EVET_OPNTMNBX_03300E00_00_ADDR, tmp_buf + 4, 3);
//    oc_flg = tmp_buf[0];
//    cnt1 = byte_merge1(tmp_buf + 1, EVET_OPNCOVER_03300D00_00_LEN);
//    cnt2 = byte_merge1(tmp_buf + 4, EVET_OPNTMNBX_03300E00_00_LEN);
//    if((oc_flg == 0xFF) && (cnt1 == 0xFFFFFF) && (cnt2 == 0xFFFFFF))
//    {
//        if((0 == (PORT0_IS_HIGH(P0_KEYG))) && (0 == (PORT0_IS_HIGH(P0_KEYD))))
//        {
//            tmp_buf[0] = tmp_buf[1] = tmp_buf[2] = 0;
//            file_write(EEPROM_GFLG_ADDR, tmp_buf, 1);               //oc_flgӳ����������
//            file_write(EVET_OPNCOVER_03300D00_00_ADDR, tmp_buf, 3); //���ϸ�cnt1ӳ����������
//            file_write(EVET_OPNTMNBX_03300E00_00_ADDR, tmp_buf, 3); //����ť��cnt2ӳ����������
//        }
//        else
//        {
//            return;
//        }
//    }
//
//    file_read(EEPROM_GFLG_ADDR, &oc_flg, 1);  //��ȡ1�ֽ��ϴο��ر�Ǽ���ť��״̬��Ϣ
//
//    SET_PORT0_IN(P0_KEYG);                    //����p0.20Ϊ�����[���ϸǼ�ⰴť]
//    if((!(oc_flg & OC_COVER_FLG)) && (PORT0_IS_HIGH(P0_KEYG)))  //���ϸ�
//    {
//        oc_flg |= OC_COVER_FLG;                 //��ʶΪ����
//        file_write(EEPROM_GFLG_ADDR, &oc_flg, 1);                   //���汾�ο��Ǳ�ʶ
////        ALARM_LED_ON();     //DBG
//        //����(����ʱ��)��¼����
//        file_read(EVET_OPNCOVER_03300D00_00_ADDR, tmp_buf, EVET_OPNCOVER_03300D00_00_LEN);//��ȡ���ֽ��ܴ���
//        cnt1 = byte_merge1(tmp_buf, EVET_OPNCOVER_03300D00_00_LEN);//�����ܴ���
//        if(cnt1 == 0xFFFFFF)
//        {
//            cnt1 = 0x000001;                     //��Ϊ��һ�ֵĵ�һ��
//        }
//        else
//        {
//            cnt1++;
//        }
//        for(i = 0; i < 3; i++)
//        {
//            tmp_buf[i] = (cnt1 >> (i * 8));
//        }
//        file_write(EVET_OPNCOVER_03300D00_00_ADDR, tmp_buf, EVET_OPNCOVER_03300D00_00_LEN);//�����ܴ���(������ƫ��ָ��)
//        cnt1 %= EVET_OPNCOVER_03300D01_00_OFFSET_MAXNUM;//�����ǰ�������(10��)����
//        addr = cnt1 * EVET_OPNCOVER_03300D01_00_OFFSET_BLOCKLEN + EVET_OPNCOVER_03300D01_00_HEAD_ADDR;//������ο������ݱ�����ʼ��ַ
//        get_software_rtc(tmp_buf);              //��ȡ���Ƿ���ʱ��
//		convert_to_bcd(tmp_buf, 7);//azh
//		for(i = 0; i < 3; i++)
//        {
//            tmp_buf[i + 3] = tmp_buf[i + 4];    //ɾ���ܴ����ݲ�������ǰ��
//        }
//        file_write(addr, tmp_buf, 6);           //���濪�Ƿ���ʱ��
//        //��¼��ʱ�������ݼ�:���������й��ܵ��ܡ��������޹��ܵ���
//        addr += 12;                             //�������ʱ�������ݴ洢ƫ�Ƶ�ַ
//        for(i = 0; i < 6; i++)
//        {
//            j = (i > 1? 3 : 1);
//            file_write(addr + i * 4, (U8 *)&gEngArray[6 * (i + j)], ENERGY_LEN);
//        }
////        file_write(addr, gEngArray + 6 * 1, ENERGY_LEN);       //�����й��ܵ���
////        file_write(addr + 4, gEngArray + 6 * 2, ENERGY_LEN);   //�����й��ܵ���
////        file_write(addr + 8, gEngArray + 6 * 5, ENERGY_LEN);   //��һ�����޹��ܵ���
////        file_write(addr + 12, gEngArray + 6 * 6, ENERGY_LEN);   //�ڶ������޹��ܵ���
////        file_write(addr + 16, gEngArray + 6 * 7, ENERGY_LEN);   //���������޹��ܵ���
////        file_write(addr + 20, gEngArray + 6 * 8, ENERGY_LEN);   //���������޹��ܵ���
//    }
//    else if((oc_flg & OC_COVER_FLG) && (!PORT0_IS_HIGH(P0_KEYG)))    //���ϸ�
//    {
//        oc_flg &= ~OC_COVER_FLG;            //��ʶΪ�ظ�
//        file_write(EEPROM_GFLG_ADDR, &oc_flg, 1);                   //���汾�ιظǱ�ʶ
//        ALARM_LED_OFF();     //DBG
//        //�ظ�(����ʱ��)��¼����
//        file_read(EVET_OPNCOVER_03300D00_00_ADDR, tmp_buf, EVET_OPNCOVER_03300D00_00_LEN);//��ȡ���ֽ��ܴ���
//        cnt1 = byte_merge1(tmp_buf, EVET_OPNCOVER_03300D00_00_LEN);//�����ܴ���
//
//        cnt1 %= EVET_OPNCOVER_03300D01_00_OFFSET_MAXNUM;//�����ǰ�������(10��)����
//        addr = cnt1 * EVET_OPNCOVER_03300D01_00_OFFSET_BLOCKLEN + EVET_OPNCOVER_03300D01_00_HEAD_ADDR;//������ο������ݱ�����ʼ��ַ
//        get_software_rtc(tmp_buf);              //��ȡ���Ƿ���ʱ��
//		convert_to_bcd(tmp_buf, 7);//azh
//		for(i = 0; i < 3; i++)
//        {
//            tmp_buf[i + 3] = tmp_buf[i + 4];    //ɾ���ܴ����ݲ�������ǰ��
//        }
//        file_write(addr + 6, tmp_buf, 6);       //���濪�ǽ���ʱ��
//        //��¼��ʱ�������ݼ�:���������й��ܵ��ܡ��������޹��ܵ���
//        addr += (12 + 4 * 6);                   //����ո�ʱ�������ݴ洢ƫ�Ƶ�ַ
//        for(i = 0; i < 6; i++)
//        {
//            j = (i > 1? 3 : 1);
//            file_write(addr + i * 4, (U8 *)&gEngArray[6 * (i + j)], ENERGY_LEN);
//        }
//    }
//
//    SET_PORT0_IN(P0_KEYD);                    //����p0.21Ϊ�����[����ť�м�ⰴť]
//    if((!(oc_flg & OC_TMNBX_FLG)) && (PORT0_IS_HIGH(P0_KEYD)))  //����ť��
//    {
//        oc_flg |= OC_TMNBX_FLG;                 //��ʶΪ����ť��
//        file_write(EEPROM_GFLG_ADDR, &oc_flg, 1);                   //���汾�ο���ť�б�ʶ
////        ALARM_LED_ON();     //DBG
//        //����ť��(����ʱ��)��¼����
//        file_read(EVET_OPNTMNBX_03300E00_00_ADDR, tmp_buf, EVET_OPNTMNBX_03300E00_00_LEN);//��ȡ���ֽ��ܴ���
//        cnt2 = byte_merge1(tmp_buf, EVET_OPNTMNBX_03300E00_00_LEN);//����ť���ܴ���
//        if(cnt2 == 0xFFFFFF)
//        {
//            cnt2 = 0x000001;                     //��Ϊ��һ�ֵĵ�һ��
//        }
//        else
//        {
//            cnt2++;
//        }
//        for(i = 0; i < 3; i++)
//        {
//            tmp_buf[i] = (cnt2 >> (i * 8));
//        }
//        file_write(EVET_OPNTMNBX_03300E00_00_ADDR, tmp_buf, EVET_OPNTMNBX_03300E00_00_LEN);//�����ܴ���(������ƫ��ָ��)
//        cnt2 %= EVET_OPNTMNBX_03300E01_00_OFFSET_MAXNUM;//�����ǰ����ť�����(10��)����
//        addr = cnt2 * EVET_OPNTMNBX_03300E01_00_OFFSET_BLOCKLEN + EVET_OPNTMNBX_03300E01_00_HEAD_ADDR;//������ο���ť�����ݱ�����ʼ��ַ
//        get_software_rtc(tmp_buf);              //��ȡ���Ƿ���ʱ��
//		convert_to_bcd(tmp_buf, 7);//azh
//		for(i = 0; i < 3; i++)
//        {
//            tmp_buf[i + 3] = tmp_buf[i + 4];    //ɾ���ܴ����ݲ�������ǰ��
//        }
//        file_write(addr, tmp_buf, 6);           //���濪�Ƿ���ʱ��
//        //��¼��ʱ�������ݼ�:���������й��ܵ��ܡ��������޹��ܵ���
//        addr += 12;                             //�������ť��ʱ�������ݴ洢ƫ�Ƶ�ַ
//        for(i = 0; i < 6; i++)
//        {
//            j = (i > 1? 3 : 1);
//            file_write(addr + i * 4, (U8 *)&gEngArray[6 * (i + j)], ENERGY_LEN);
//        }
//    }
//    else if((oc_flg & OC_TMNBX_FLG) && (!PORT0_IS_HIGH(P0_KEYD)))    //�ض�ť��
//    {
//        oc_flg &= ~OC_TMNBX_FLG;                //��ʶΪ�ض�ť��
//        file_write(EEPROM_GFLG_ADDR, &oc_flg, 1);                   //���汾�ιض�ť�б�ʶ
//        ALARM_LED_OFF();     //DBG
//        //�ض�ť��(����ʱ��)��¼����
//        file_read(EVET_OPNTMNBX_03300E00_00_ADDR, tmp_buf, EVET_OPNTMNBX_03300E00_00_LEN);//��ȡ���ֽ��ܴ���
//        cnt2 = byte_merge1(tmp_buf, EVET_OPNTMNBX_03300E00_00_LEN);//����ť���ܴ���
//
//        cnt2 %= EVET_OPNTMNBX_03300E01_00_OFFSET_MAXNUM;//�����ǰ����ť�����(10��)����
//        addr = cnt2 * EVET_OPNTMNBX_03300E01_00_OFFSET_BLOCKLEN + EVET_OPNTMNBX_03300E01_00_HEAD_ADDR;//������ο���ť�����ݱ�����ʼ��ַ
//        get_software_rtc(tmp_buf);              //��ȡ����ť�з���ʱ��
//		convert_to_bcd(tmp_buf, 7);//azh
//		for(i = 0; i < 3; i++)
//        {
//            tmp_buf[i + 3] = tmp_buf[i + 4];    //ɾ���ܴ����ݲ�������ǰ��
//        }
//        file_write(addr + 6, tmp_buf, 6);       //���濪��ť�н���ʱ��
//        //��¼��ʱ�������ݼ�:���������й��ܵ��ܡ��������޹��ܵ���
//        addr += (12 + 4 * 6);                   //����ն�ť��ʱ�������ݴ洢ƫ�Ƶ�ַ
//        for(i = 0; i < 6; i++)
//        {
//            j = (i > 1? 3 : 1);
//            file_write(addr + i * 4, (U8 *)&gEngArray[6 * (i + j)], ENERGY_LEN);
//        }
//    }
//}

/******************************************************************************
����: void record_event_power_down(void)
���: ��
����: ��
����: ��
******************************************************************************/
//void record_event_power_down (void)
//{
//    static U8 pd_flg = 0;
//    U8 i,tmp_buf[7];
//    U32 cnt,addr;
//
//    SET_PORT0_IN(P0_PD_CHK);            //����p0.29Ϊ�����
//    if(((pd_flg & 0x0f) == 0) && PORT0_IS_HIGH(P0_PD_CHK))        //�������ź�����
//    {
//        os_dly_wait(OS_DLY_2MS);        //�ȴ�
//        if(PORT0_IS_HIGH(P0_PD_CHK))
//        {
//            pd_flg |= (1 << 0);         //δ�����ʶ
//        }
//    }
//    else if(((pd_flg & 0x0f) == 1) && ((PORT0_IS_HIGH(P0_PD_CHK) == 0)))//���ֵ���
//    {
//        os_dly_wait(OS_DLY_2MS);        //�ȴ�
//        if((PORT0_IS_HIGH(P0_PD_CHK) == 0))
//        {
//            pd_flg |= (1 << 1);                     //�����ʶ
//
//            file_read(EVET_POWDNTIME_03110000_00_ADDR, tmp_buf, EVET_POWDNTIME_03110000_00_LEN);//��ȡ���ֽ��ܴ���
//            cnt = byte_merge(tmp_buf, EVET_POWDNTIME_03110000_00_LEN);//�����ܴ���
//            if(cnt == 0xFFFFFF)
//            {
//                cnt = 0x000000;                     //��Ϊ��һ�ֵĵ�һ��
//            }
//            else if(cnt  < 0xFFFFFF)
//            {
//                cnt++;
//            }
//            for(i = 0; i < 3; i++)
//            {
//                tmp_buf[i] = (cnt >> (i * 8));
//            }
//            file_write(EVET_POWDNTIME_03110000_00_ADDR, tmp_buf, EVET_POWDNTIME_03110000_00_LEN);//�����ܴ���(������ƫ��ָ��)
//            cnt %= EVET_POWDN_03110001_00_OFFSET_MAXNUM;//�����ǰ�������(10��)����
//            addr = cnt * EVET_POWDN_03110001_00_OFFSET_BLOCKLEN + EVET_POWDN_03110001_00_HEAD_ADDR;//������ε������ݱ�����ʼ��ַ
//            get_software_rtc(tmp_buf);              //��ȡ���緢��ʱ��
//			convert_to_bcd(tmp_buf, 7);//azh
//			for(i = 0; i < 3; i++)
//            {
//                tmp_buf[i + 3] = tmp_buf[i + 4];    //ɾ���ܴ����ݲ�������ǰ��
//            }
//            file_write(addr, tmp_buf, 6);           //������緢��ʱ��
//        }
//    }
//    else if(((pd_flg & 0x0f) == 3) && PORT0_IS_HIGH(P0_PD_CHK))//�����ϵ�
//    {
//        os_dly_wait(OS_DLY_2MS);        //�ȴ�
//        if(PORT0_IS_HIGH(P0_PD_CHK))
//        {
//            pd_flg &= 0xf0;                         //��ʶ��λ
//
//            file_read(EVET_POWDNTIME_03110000_00_ADDR, tmp_buf, EVET_POWDNTIME_03110000_00_LEN);//��ȡ���ֽ��ܴ���
//            cnt = byte_merge(tmp_buf, EVET_POWDNTIME_03110000_00_LEN);//�����ܴ���
//
//            cnt %= EVET_POWDN_03110001_00_OFFSET_MAXNUM;//�����ǰ�������(10��)����
//            addr = cnt * EVET_POWDN_03110001_00_OFFSET_BLOCKLEN + EVET_POWDN_03110001_00_HEAD_ADDR;//������ε������ݱ�����ʼ��ַ
//            get_software_rtc(tmp_buf);              //��ȡ�������ʱ��
//			convert_to_bcd(tmp_buf, 7);//azh
//			for(i = 0; i < 3; i++)
//            {
//                tmp_buf[i + 3] = tmp_buf[i + 4];    //ɾ���ܴ����ݲ�������ǰ��
//            }
//            file_write(addr + 6, tmp_buf, 6);       //����������ʱ��
//        }
//    }
//}

#endif//CODE_VER_TYPE!=GPRS_MODULE_PRJ
/******************************************************************************
                            End Of File
******************************************************************************/

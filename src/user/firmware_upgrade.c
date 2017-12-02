/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : firmware_upgrade.c
 Description    : ���ڴ�������, �󲿷ֺ�����������, ����ʱ��ϴ�����������������
 Related Files  : firmware_upgrade.h file_head_info.h
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��    1.10        2011-01-18    wwllzz          ��������4K������
   1��    1.00        2010-09-16    wwllzz          the original version
******************************************************************************/
#ifndef DISPLAY_TERMINAL
#include "global_config.h"
#else   //DISPLAY_TERMINAL
#include "..\inc\global_config.h"
#define Sys_Tick OS_Time
#endif  //DISPLAY_TERMINAL

extern const U8 version_info[];

extern U8 Sample_Data[];//���崦��U32 ����ļ��õ�ʱ���ǰ�U8���� Ҫע��
//#define Sample_Data ((U8*)SampleData)
//-------------------------------- �������� -----------------------------------
#ifdef IDEA_ACC
static const U8 VERSION_INFO_S[] = {"����˵��ط���64�ֽڹ̶����Ƚϼ�, �Ժ�ͳ���汾��û��"};
static const U8 HUA_CAI[] = {"����"};
static const U8 XIAO_PIP[] = {"������׶Ⱥ�У���õ�����˶���������Ҫ��52��"};
static const U8 DATE_PIP[] = {"����20�ֽ�ʱ�����������ӵ�60�ֽ�ʱ�䴩�����������"};
#endif  //IDEA_ACC

//#ifdef PRINT_SLAVE_DBG
//#define DBGTHR U1THR                                    //�ӵ�Ԫ���Դ���
//static const U8 Ascii[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
//#endif  //PRINT_SLAVE_DBG

//------------------------------ ˽�б������� ---------------------------------
static U32 param_table[5];                              //����FLASH������������������
static U32 result_table[5];                             //����FLASH������������ֵ��
static U32 last_addr;                                   //����(����)�Ŀ�ʼ��ַ, 256����
static U32 data_code[256/4];                            //����(����)������
#ifdef FIRST_RESPONSE
static U32 last_pa;                                     //�ϴα�̵�256�ֽڿ��׵�ַ
static U32 data_code_len;                               //���������볤��
static U32 upgrade_sign;                                //��ѭ����λ��ʶ����
    //bit0----����PROGRAM_CODE_AFTER_ANSWER, ��ʶ��Ҫ��¼������֡��������
    //bit1-bit2----���ں���2λ��ʶ
    //bit3----����λ
#endif  //FIRST_RESPONSE
#ifndef DISPLAY_TERMINAL
static U32 run_normal_ram;                              //���򲿷���Ч���б�ʶ
                                    //bit0-Zigbee��ͨ��
                                    //bit1-��Aģ����ͨ��
                                    //bit2-������������
                                    //bit3~bit31-��ʱ����
#else//DISPLAY_TERMINAL
static U32 run_normal_ram;                              //���򲿷���Ч���б�ʶ, ��ʾ�ն�
                                    //bit0-Zigbee��ͨ��
                                    //bit1-GPRS��ͨ��
                                    //bit2-��ʾ����
                                    //bit3~bit31-��ʱ����
#endif//DISPLAY_TERMINAL
static U32 soft_reset_count;                            //��λ�ü�����, ����Tickֵ
static U32 tmp_tick;                                    //�ϴ�tickֵ

//------------------------------ ˽�к������� ---------------------------------
static void write_data_FW(U32 flash_address, U32 *flash_data_buf, U32 count);
static void erase_sector(U32 start_sector, U32 end_sector);
static void prepare_sector(U32 start_sector, U32 end_sector);
#ifndef DISPLAY_TERMINAL
static void iap_entry(U32 param_tab[], U32 result_tab[]);
#endif//DISPLAY_TERMINAL
static U32 address_to_section_number(U32 addr);
static U32 is_run_position(void);
static void program_run_using_flag(void *p);
static U32 check_code_crc(U32 c);
#ifdef IDEA_ACC
static U32 get_aa_addr(U32 zone);
#endif  //IDEA_ACC

/**
 * @brief  Set the Fault Mask value
 *
 * @param  faultMask  faultMask value
 *
 * Set the fault mask register
 */
#ifndef DISPLAY_TERMINAL
__asm void __set_FAULTMASK(U32 faultMask)
{
  msr faultmask, r0
  bx lr
}
#endif  //DISPLAY_TERMINAL

/******************************************************************************
����: void write_data_FW(U32 flash_address, U32 *flash_data_buf, U32 count)
���: flash_address FLASH��ʼ��ַ, ����256�ֽڶ���
      flash_data_buf���뱣���ַ, ����4�ֽ�(��)����
      count��256,512,1024,4096֮һ
����: �� ȫ�ֽ����result_table[0..5]
����: ��дFLASH. ʹ��ȫ��param_table��
******************************************************************************/
void write_data_FW (U32 flash_address, U32 *flash_data_buf, U32 count)
{
    if(flash_address < 4096)                            //��ֹ��boot�����
    {
        return;
    }
    if((flash_address >= CODE_START_ADDRSS) && (flash_address <= CODE_END_ADDRESS))
    {
        return;                                         //Ҫд����������������, ɶҲ�����ķ���
    }
    param_table[0] = COPY_RAM_TO_FLASH;                 //������д
    param_table[1] = flash_address;
    param_table[2] = (U32)flash_data_buf;
    param_table[3] = count;
    param_table[4] = FCCLK / 1024;
    iap_entry(param_table, result_table);
}

/******************************************************************************
����: void erase_sector(U32 start_sector, U32 end_sector)
���: start_sector��ʼ������, end_sector����������
����: �� ȫ�ֽ����result_table[0..5]
����: ������������. ʹ��ȫ��param_table��
******************************************************************************/
void erase_sector (U32 start_sector, U32 end_sector)
{
    if(start_sector == 0)                               //��ֹ��boot�����
    {
        return;
    }
    if((start_sector >= BLOCK_START) && (start_sector <= BLOCK_END))
    {
        return;                                         //Ҫ�����Ŀ��ڵ�ǰ������
    }
    param_table[0] = ERASE_SECTOR;
    param_table[1] = start_sector;
    param_table[2] = end_sector;
    param_table[3] = FCCLK / 1024;
    iap_entry(param_table, result_table);
}

/******************************************************************************
����: void prepare_sector(U32 start_sector, U32 end_sector)
���: start_sector��ʼ������, end_sector����������
����: �� ʹ��ȫ��param_table��
����: ָ����������. ʹ��ȫ��param_table��
******************************************************************************/
void prepare_sector (U32 start_sector, U32 end_sector)
{
    if(start_sector == 0)                               //��ֹ��boot�����
    {
        return;
    }
    if((start_sector >= BLOCK_START) && (start_sector <= BLOCK_END))
    {
        return;                                         //Ҫָ�����ڵ�ǰ������
    }
    param_table[0] = PREPARE_SECTOR_FOR_WRITE;
    param_table[1] = start_sector;
    param_table[2] = end_sector;
    param_table[3] = FCCLK / 1024;
    iap_entry(param_table, result_table);
}

/******************************************************************************
����: void iap_entry(U32 param_tab[], U32 result_tab[])
����: ����оƬ����Boot�ӳ���Ĺ淶���д��
���: param_tab��ڲ�����, result_tab�ӳ���������
����: ��, result_tab�а����������
******************************************************************************/
void iap_entry (U32 param_tab[], U32 result_tab[])
{
#ifndef DISPLAY_TERMINAL
    void (*iap)(U32 [], U32 []);
    U32 tmp;
    iap = (void (*)(U32 [], U32 []))IAP_LOCATION;

    __disable_irq();                                    //���ж�
    if((COPY_RAM_TO_FLASH == param_tab[0]) || (ERASE_SECTOR == param_tab[0]))
    {
        tmp = T1TC;
        iap(param_tab, result_tab);
        tmp = (T1TC - tmp);                             //ʵ�ʹ۲���ʱ��Լ1mS, �����ͱ�̲��
        tmp /= SYS_TICK_ON_T1;
        if(0 != tmp)
        {
            Sys_Tick += tmp;
        }
        AD_Ctr[AD1].CrdyFail = AD_Ctr[AD2].CrdyFail = 0;//��������
    }
    else
    {
        iap(param_tab, result_tab);
    }
    __enable_irq();                                     //���ж�
#else   //DISPLAY_TERMINAL
    iap_programe(param_tab, result_tab);
#endif  //DISPLAY_TERMINAL
}

/******************************************************************************
����: U32 address_to_section_number(U32 addr)
���: addr��������е�ַ
����: ��8λΪ������, bit8��ַ����Χ, bit9��ַ����256����, bit11����ȷ����(bit8/9Ϊ0)
    bit10������ʼ
����: �����ַ��Ӧ��������, ��0��ʼ. ֻ���Ҫ����д��Ĵ�������б�
******************************************************************************/
U32 address_to_section_number (U32 addr)
{
    U32 tmp;

    tmp = is_run_position();                            //�б�ǰ����������
    if(tmp == 1)                                        //��ǰ�����ڵ���
    {
        if((addr < HIGH_CODE_START) || (addr > HIGH_CODE_END))  //�����ڸߴ�����
        {
            return 1 << 8;
        }
    }
    else if(tmp == 2)                                   //��ǰ���������ڸ���
    {
        if((addr < LOW_CODE_START) || (addr > LOW_CODE_END))
        {
            return 1 << 8;
        }
    }
    else                                                //��ǰ��������������
    {
        return 1 << 8;
    }
    if((addr & 0xff) != 0)                              //��ʼ��ַ����256�ֽڶ���
    {
        return 1 << 9;
    }
    if(addr < 0x10000)                                  //ǰ64K��ÿ��4K��
    {
        tmp = addr >> 12;                               //������
        if((addr & 0xff) != 0)                          //û��256�ֽڶ���
        {
            tmp |= (1 << 9);
        }
        if((addr & 0xfff) == 0)                         //һ��Ŀ�ʼ
        {
            tmp |= (1 << 10);
        }
    }
    else
    {
        tmp = 0x10 + ((addr - 0x10000) >> 15);          //������
        if((addr & 0xff) != 0)                          //û��256�ֽڶ���
        {
            tmp |= (1 << 9);
        }
        if((addr & 0x7fff) == 0)                        //һ��Ŀ�ʼ
        {
            tmp |= (1 << 10);
        }
    }
    return tmp;
}

#ifndef FIRST_RESPONSE
/******************************************************************************
����: U8 write_package_code(U8 buf[])
���: buf-ָ��ӵ��Ĵ��뱨��֡�ĳ����ֽ�λ��, ��buf[15]��ʼΪ��ַ
����: 0-�ɹ�, ����Ϊ����(����)����, bit3-CRCУ�����, bit1-�õ�ַ��ֹд, bit5-�װ���ַ����
    bit6-����̫С
����: ���Ĵ������Ļ�����, ���ȱ������20, ��Ҫ���ر���. �ڲ��Ա��ĵ����ݽ�������
    ƴ��, ����256�ֽڽ���һ��д�����ͬʱ���в���.
    68+ ͨ�ŵ�ַ6+ 68+ C+ ����1+ �����ʶ��4+����4+�����ߴ���4+��չ��ʶ��2+ ��ַ4+����192+CRC2+ CS+16
    CRC�Ǹ��ֽ���ǰ ʹ��Sample_Data
******************************************************************************/
//U8 write_package_code (U8 buf[])
//{
//    U32 tmp, i, len;
//    U32 addr;
//    U8 *p;
//
//    if(buf[0] < 21)                                     //�����ܰ�������, �ѱ�֤ buf[0] >= 20
//    {
//        return (1 << 6);
//    }
//    tmp = calc_crc16(buf + 15, buf[0] - 16, 0xffff);    //CRC����, ����0xffffΪ��ֵ
//    if(tmp != (buf[buf[0]] + buf[buf[0] - 1] * 256))    //CRC����
//    {
//        return (1 << 3);
//    }
//    addr = (buf[15] << 24) + (buf[16] << 16) + (buf[17] << 8) + buf[18];    //���ֽ���ǰ
//    if(address_to_section_number(addr) & (1 << 8))      //��ַ����Ŀǰ��������Χ
//    {
//        return (1 << 1);
//    }
//    if(last_addr == 0)                                  //Ҫ�������װ�
//    {
//        p = Sample_Data;
//        memcpy(p, (void *)RUN_USING_FLAG, 4096);        //����ȫ����������
//        if(is_run_position() == 1)                      //��ַ������ȷ��ֵ
//        {
//            if(addr != HIGH_CODE_START)                 //��һ���ĵ�ַ����
//            {
//                return (1 << 5);
//            }
//            last_addr = HIGH_CODE_START;                //���������Ŀ�ʼ��ַ
//            p[4] |= 0x1a;                               //��ʶ�������´���, ����CRCû��֤, û���й�
//        }
//        else
//        {
//            if(addr != LOW_CODE_START)                  //��һ���ĵ�ַ����
//            {
//                return (1 << 5);
//            }
//            last_addr = LOW_CODE_START;
//            p[4] &= ~0x10;                              //���´���Ϊ����
//            p[4] |= 0x05;                               //����û���й�, CRCҲδ��֤
//        }
//        program_run_using_flag(p);                      //��д��ʶ��
//        for(i = 0; i < 256 / 4; i++)                    //��仺����
//        {
//            data_code[i] = 0xffffffff;
//        }
//    }
//    else
//    {
//        if((last_addr & 0xffffff00) != (addr & 0xffffff00))             //��Ҫ��д�µ�256�ֽ�ҳ��
//        {
//            len = buf[0] - 20;                          //�ȼ���Ƿ�Ϊ�ط�֡, �ط����˳�
//            p = (U8 *)data_code;
//            for(i = 0; i < len; i++)
//            {
//                if(((addr & 0xff) + i) < 256)           //�����Ѿ�д��FLASH
//                {
//                    if(buf[19 + i] != *((U8 *)(addr + i)))  //�����в�ͬ��
//                    {
//                        break;
//                    }
//                }
//                else                                    //���뻹�ڻ�����
//                {
//                    if(buf[19 + i] != p[(addr + i) & 0xff])
//                    {
//                        break;
//                    }
//                }
//            }
//            if(i == len)                                //����ͬ
//            {
//                if(last_addr == (addr + len))           //�����ط���֡
//                {
//                    return 0;
//                }
//            }
//            if(last_addr & 0xff)                        //�����ڲ�������ҳ����Ҫ��д
//            {
//                tmp = address_to_section_number(last_addr & 0xffffff00);
//                if(tmp & (1 << 10))                     //������ʼ
//                {
//                    prepare_sector(tmp & 0xff, tmp & 0xff);             //ָ��ҳ��
//                    erase_sector(tmp & 0xff, tmp & 0xff);               //����ҳ��
//                }
//                prepare_sector(tmp & 0xff, tmp & 0xff); //ָ��ҳ��
//                write_data_FW(last_addr & 0xffffff00, data_code, 256);
//            }
//            for(i = 0; i < 256 / 4; i++)                //��仺����
//            {
//                data_code[i] = 0xffffffff;
//            }
//            if((last_addr & 0xffff8000) != (addr & 0xffff8000)) //��������Ϣ��
//            {
//                for(i = 0; i < 64 / 4; i++)             //���¿�ʼ�ĳ����Ӧ���Ƿ��ѱ�������
//                {
//                    if(*((U32 *)(addr + i)) != 0xffffffff)
//                    {
//                        break;
//                    }
//                }
//                if(i != 64 / 4)                         //����Ҫ����, Ŀǰֻ�������ļ���Ϣ��
//                {
//                    tmp = address_to_section_number(addr & 0xffff8000);
//                    prepare_sector(tmp & 0xff, tmp & 0xff);
//                    erase_sector(tmp & 0xff, tmp & 0xff);
//                }
//            }
//            last_addr = addr & 0xffffff00;              //ͬ����ַ
//        }
//    }
//    p = (U8 *)data_code;
//    len = buf[0] - 20;                                  //��֤buf[0]����20
//    for(i = 0; i < len; i++)
//    {
//        if((((addr + i) & 0xff)) == (256 - 1))
//        {
//            p[256 - 1] = buf[19 + i];                   //���ݷŵ�������
//            tmp = address_to_section_number(last_addr & 0xffffff00);    //����������, ����ҳ��д��
//            if(tmp & (1 << 10))                         //������ʼ
//            {
//                prepare_sector(tmp & 0xff, tmp & 0xff); //ָ��ҳ��
//                erase_sector(tmp & 0xff, tmp & 0xff);   //����ҳ��
//            }
//            prepare_sector(tmp & 0xff, tmp & 0xff);     //ָ��ҳ��
//            write_data_FW(last_addr & 0xffffff00, data_code, 256);
//
//            for(tmp = 0; tmp < 256 / 4; tmp++)          //���������
//            {
//                data_code[tmp] = 0xffffffff;
//            }
//            addr += 256;
//            last_addr += 256;
//        }
//        else                                            //ֻ��Ҫ���ݸ�ֵ
//        {
//            p[(addr + i) & 0xff] = buf[19 + i];
//        }
//    }
//    last_addr = (last_addr & 0xffffff00) | ((addr + i) & 0xff); //���ϻ����ڵ�ƫ����
//    return 0;
//}
#endif  //FIRST_RESPONSE

/******************************************************************************
����: U8 activate_FW(U8 index)
���: index-0 the binary last loaded, �������ص�
            1 the last but one binary loaded, �ɵ�
            2 �л�������
            3 �л�������
            other : rejected
����: 1-�ɹ�, 0-ʧ��, 2-���踴λ, 3-�����߼�©��, Ϊ����0,2,3����(1 << 4)
����: �����������ָ�� �յ�����ָ���, �����жϴ������������ȷ��, ��index����ʶ�л�
    ��Ҫ�л��Ĵ���У�鲻ͨ��, �򷵻�ʧ��, �������л�
˵��: �����л��ڸ�λ�����, ����־��д�ڱ��ӳ���ʵ��
      ����1��������־��д���, ʹ��Sample_Data
      ϵͳ��������ݱ���ȷ����վ�յ���Ӧ���Ͽ����ӵȺ�, ��ִ�и�λ
******************************************************************************/
U8 activate_FW (U8 index)
{
    U32 tmp;
    U8 *p;

    last_addr = 0;                                      //����ȷ�ϳ�����������
    p = (U8*)Sample_Data;
    memcpy(p, (void *)RUN_USING_FLAG, FLAG_FLASH_MAX_READ); //����4K�ֽ�
    tmp = is_run_position();                            //��Ӧ����У���Ƿ���ȷ
    if(tmp == 1)                                        //��ǰ�����ڵ���
    {
        if(p[0] == 1)                                   //Ҫ�����е���
        {
            if((index == 2) ||                          //��ǰ�����ڵ���, ��ʶҪ�����е���, Ҫ���л�������
                ((index == 0) && ((p[4] & (1 << 4)) == 0)) ||   //������������, Ҫ���µ�
                ((index == 1) && ((p[4] & (1 << 4)) != 0)))     //������������, Ҫ��ɵ�
            {
                return (1 << 4);                        //return 0;//���趯��, ��Ӧʧ��
            }
            if((index == 3) ||                          //��ǰ�����ڵ���, ��ʶҪ�����е���, Ҫ���л�������
                ((index == 0) && ((p[4] & (1 << 4)) != 0)) ||   //�����������ص�, Ҫ���µ�
                ((index == 1) && ((p[4] & (1 << 4)) == 0)))     //�����������ص�, Ҫ��ɵ�
            {
                if(((p[4] & 0xa) != 0xa) || check_code_crc(1))  //������CRC, ���й����߱�ʶCRC��ȷ��Ҳ����
                {
                    if((p[4] & (1 << 3)) != 0)          //����CRCû��ʶ��ȷ
                    {
                        p[4] &= ~(1 << 3);
                    }
                    p[0] = 2;                           //�е�����
                    program_run_using_flag(p);
                    soft_reset_count = SOFT_RESET_DELAY_TIME;
                    tmp_tick = Sys_Tick;                //��ʼ��λʱ��
                    return 1;
                }
                return (1 << 3);                        //return 0;
            }
        }
        else                                            //Ҫ�����и���
        {
            if((index == 2) ||                          //��ǰ�����ڵ���, ��ʶҪ�����и���, Ҫ���л�������, ֻ��Ҫ���ı�ʶ
                ((index == 0) && ((p[4] & (1 << 4)) == 0)) ||   //������������, Ҫ���µ�
                ((index == 1) && ((p[4] & (1 << 4)) != 0)))     //������������, Ҫ��ɵ�
            {
                p[0] = 1;                               //�޸����б�ʶ
                program_run_using_flag(p);
                return (1 << 4);                        //return 2;//�����л�
            }
            if((index == 3) ||                          //��ǰ�����ڵ���, ��ʶҪ�����и���, Ҫ���л�������
                ((index == 0) && ((p[4] & (1 << 4)) != 0)) ||   //�����������ص�, Ҫ���µ�
                ((index == 1) && ((p[4] & (1 << 4)) == 0)))     //�����������ص�, Ҫ��ɵ�
            {
                if(((p[4] & 0xa) != 0xa) || check_code_crc(1))  //������CRC, ���й����߱�ʶCRC��ȷ��Ҳ����
                {
                    if((p[4] & (1 << 3)) != 0)          //����CRC��ʶû����
                    {
                        p[4] &= ~(1 << 3);              //��ʶ����CRC����
                        program_run_using_flag(p);
                    }
                    soft_reset_count = SOFT_RESET_DELAY_TIME;
                    tmp_tick = Sys_Tick;                //��ʼ��λʱ��
                    return 1;                           //ֱ����λ������
                }
                return (1 << 3);                        //return 0;//����CRC�쳣
            }
        }
    }
    else if(tmp == 2)                                   //��ǰ�����ڸ���
    {
        if(p[0] == 2)                                   //Ҫ�����и���
        {
            if((index == 3) ||                          //��ǰ���и���, ��ʶҪ�����и���, Ҫ���л�������
                ((index == 0) && ((p[4] & (1 << 4)) != 0)) ||   //�����������ص�, Ҫ���µ�
                ((index == 1) && ((p[4] & (1 << 4)) == 0)))     //�����������ص�, Ҫ��ɵ�
            {
                return (1 << 4);                        //return 0;//���趯��, ��Ӧʧ��
            }
            if((index == 2) ||                          //��ǰ���и���, ��ʶҪ�����и���, Ҫ���л�������
                ((index == 0) && ((p[4] & (1 << 4)) == 0)) ||   //������������, Ҫ���µ�
                ((index == 1) && ((p[4] & (1 << 4)) != 0)))     //������������, Ҫ��ɵ�
            {
                if(((p[4] & 0x5) != 0x5) || check_code_crc(0))  //������CRC, ���й����߱�ʶCRC��ȷ��Ҳ����
                {
                    if((p[4] & (1 << 2)) != 0)          //����CRCû��ʶ��ȷ
                    {
                        p[4] &= ~(1 << 2);
                    }
                    p[0] = 1;                           //�е�����
                    program_run_using_flag(p);
                    soft_reset_count = SOFT_RESET_DELAY_TIME;
                    tmp_tick = Sys_Tick;                //��ʼ��λʱ��
                    return 1;
                }
                return (1 << 3);                        //return 0;
            }
        }
        else                                            //Ҫ�����е���
        {
            if((index == 3) ||                          //��ǰ�����ڸ���, ��ʶҪ�����е���, Ҫ���л�������
                ((index == 0) && ((p[4] & (1 << 4)) != 0)) ||   //�����������ص�, Ҫ���µ�
                ((index == 1) && ((p[4] & (1 << 4)) == 0)))     //�����������ص�, Ҫ��ɵ�
            {
                p[0] = 1;                               //�޸����б�ʶ
                program_run_using_flag(p);
                return (1 << 4);                        //return 2;//�����л�
            }
            if((index == 2) ||                          //��ǰ���и���, ��ʶҪ�����е���, Ҫ���л�������
                ((index == 0) && ((p[4] & (1 << 4)) == 0)) ||   //������������, Ҫ���µ�
                ((index == 1) && ((p[4] & (1 << 4)) != 0)))     //������������, Ҫ��ɵ�
            {
                if(((p[4] & 0x5) != 0x5) || check_code_crc(0))  //������CRC, ���й����߱�ʶCRC��ȷ��Ҳ����
                {
                    if((p[4] & (1 << 2)) != 0)          //����û��CRC������ʶ
                    {
                        p[4] &= ~(1 << 2);
                        program_run_using_flag(p);
                    }
                    soft_reset_count = SOFT_RESET_DELAY_TIME;
                    tmp_tick = Sys_Tick;                //��ʼ��λʱ��
                    return 1;
                }
                return (1 << 3);                        //return 0;
            }
        }
    }
    else                                                //��ǰ����״̬����
    {
        return (1 << 4);                                //return 0;
    }
    return (1 << 4);                                    //return 3;
}

/******************************************************************************
����: S32 get_monitoring_status(U8 addr_buf[])
���: addr_buf[]-��Ϣ֡�����׵�ַ
����: �������ݵĳ���
����: ��ȡ��ǰд�����ĵ�ַ, �������������Ч, ҲΪ�ϵ�������, �汾��Ϣ, ��д�����
******************************************************************************/
S32 get_monitoring_status (U8 addr_buf[])
{
#ifdef FIRST_RESPONSE
    U32 addr;
    U8 m;
#endif  //FIRST_RESPONSE

    memcpy(addr_buf, (void *)0xfc0, 16);                //Boot��Ϣ16�ֽ�
#ifndef DISPLAY_TERMINAL
    memcpy(addr_buf + 16, (void *)0x1ffc0, 64);         //��һ��Ӧ�ó���汾��ʶ
#else//DISPLAY_TERMINAL
    #if CODE_MEM_SIZE == 512
        memcpy(addr_buf + 16, (void *)0x3ffc0, 64);         //��һ��Ӧ�ó���汾��ʶ
    #else//256
        memcpy(addr_buf + 16, (void *)0x1ffc0, 64);         //��һ��Ӧ�ó���汾��ʶ
    #endif
#endif//DISPLAY_TERMINAL
#if CODE_MEM_SIZE == 512
        memcpy(addr_buf + 16 + 64, (void *)0x7ffc0, 64);    //�ڶ���Ӧ�ó���汾��ʶ
#else//256
    memcpy(addr_buf + 16 + 64, (void *)0x3ffc0, 64);    //�ڶ���Ӧ�ó���汾��ʶ
#endif
    addr_buf[16 + 64 * 2] = ((*((U8 *)0x1004)) & 0x1f) | (is_run_position() << 5);
                                                        //bit0Ϊ0��ʾ��������Ч����,
                                                        //bit1Ϊ0��ʾ��������Ч����.
                                                        //bit2Ϊ0��ʾ���������ش���CRC��֤��ȷ,
                                                        //bit3Ϊ0��ʾ���������ش���CRC��֤��ȷ.
                                                        //bit4Ϊ0��ʾ�������ص�Ϊ����, 1-���µ��Ǹ���
                                                        //bit5-6��ʾ��ǰ����״̬ 1-����, 2-����, 0-�쳣
#ifndef FIRST_RESPONSE
    addr_buf[16 + 1 + 64 * 2] = last_addr >> 24;        //�ϴ������صĴ����ַ
    addr_buf[16 + 2 + 64 * 2] = last_addr >> 16;
    addr_buf[16 + 3 + 64 * 2] = last_addr >> 8;
    addr_buf[16 + 4 + 64 * 2] = last_addr;
#else   //FIRST_RESPONSE
    addr = last_addr + data_code_len;                   //�ϴ������صĴ����ַ
    addr_buf[16 + 1 + 64 * 2] = addr >> 24;
    addr_buf[16 + 2 + 64 * 2] = addr >> 16;
    addr_buf[16 + 3 + 64 * 2] = addr >> 8;
    addr_buf[16 + 4 + 64 * 2] = addr;
    for(addr = LOW_CODE_END - 64; addr != LOW_CODE_START; addr--)   //������ǰ��ff����
    {
        m = *((U8 *)addr);
        if(m != 0xff)
        {
            break;
        }
    }
    addr_buf[16 + 5 + 64 * 2] = addr >> 24;             //���ط�FF�ĵ�ַ
    addr_buf[16 + 6 + 64 * 2] = addr >> 16;
    addr_buf[16 + 7 + 64 * 2] = addr >> 8;
    addr_buf[16 + 8 + 64 * 2] = addr;

    for(addr = HIGH_CODE_END - 64; addr != HIGH_CODE_START; addr--) //������ǰ��ff����
    {
        m = *((U8 *)addr);
        if(m != 0xff)
        {
            break;
        }
    }
    addr_buf[16 + 9 + 64 * 2] = addr >> 24;             //���ط�FF�ĵ�ַ
    addr_buf[16 + 10 + 64 * 2] = addr >> 16;
    addr_buf[16 + 11 + 64 * 2] = addr >> 8;
    addr_buf[16 + 12 + 64 * 2] = addr;
#endif  //FIRST_RESPONSE
    return 149 + 8;
}

/******************************************************************************
����: U32 is_run_position(void)
���: ��
����: 1-����, 2-����, 0-�쳣, ������Ч
����: ����VTOR��ֵ�������׵�ַ�ķ�Χ ��ǰPC�б�ǰ����������
******************************************************************************/
U32 is_run_position (void)
{
    U32 tmp;

    if((version_info[14] != 'H') && (version_info[14] != 'L'))
    {
        return 0;
    }
#ifdef DISPLAY_TERMINAL
    tmp = operation_at_admin(GET_RUN_ADDR_ADMIN);//Ҫ�����ж��л�����Ȩģʽ ���ܶ���
#else//DISPLAY_TERMINAL
    tmp = VTOR;                                         //�������λ��
#endif//DISPLAY_TERMINAL
    if((HIGH_CODE_START <= tmp) && (tmp <= HIGH_CODE_END))      //�������ڸ���
    {
        tmp = *((U32 *)(tmp + 4));                      //������ڵ�ַ
        if((HIGH_CODE_START <= tmp) && (tmp <= HIGH_CODE_END))
        {
            tmp = __current_pc();                       //��ǰPCֵ
            if((HIGH_CODE_START <= tmp) && (tmp <= HIGH_CODE_END))
            {
                return 2;                               //ȷ�������ڸ���
            }
        }
        return 0;
    }
    else if((LOW_CODE_START <= tmp) && (tmp <= LOW_CODE_END))   //�������ڵ���
    {
        tmp = *((U32 *)(tmp + 4));                      //������ڵ�ַ
        if((LOW_CODE_START <= tmp) && (tmp <= LOW_CODE_END))
        {
            tmp = __current_pc();                       //��ǰPCֵ
            if((LOW_CODE_START <= tmp) && (tmp <= LOW_CODE_END))
            {
                return 1;
            }
        }
        return 0;
    }
    return 0;
}

/******************************************************************************
����: void program_run_using_flag(void *p)
���: p-���ݻ������׵�ַ
����: ��
����: �����������ݱ�̵�RUN_USING_FLAG��ʼ�ĳ����ַ��, ��4K�ֽ�
******************************************************************************/
void program_run_using_flag (void *p)
{
    prepare_sector(1, 1);                           //д�ر�ʶ
    erase_sector(1, 1);
    prepare_sector(1, 1);
    write_data_FW(RUN_USING_FLAG, (U32 *)p, FLAG_FLASH_MAX_READ);   //д�������ֽ�
}

/******************************************************************************
����: U32 check_code_crc(U32 c)
���: c-0��ʾ����, 1��ʾ����
����: 1-CRCУ����ȷ, 0-����
����: �Դ���������CRC�����Լ��, ������Ӧ�ó���������, �����汾��Ϣ��
******************************************************************************/
U32 check_code_crc (U32 c)
{
    U32 l, index;
    U32 tmp;
    U8 m;

    if(c)                                               //����
    {
        for(index = HIGH_CODE_END - 64; index != HIGH_CODE_START; index--)
        {
            m = *((U8 *)index);
            if(m != 0xff)
            {
                if(m != 0xaa)                           //��β�Ϳ�ʼ�ҵ��ĵ�һ������aa
                {
                    return 0;
                }
                break;
            }
        }
        if(index == HIGH_CODE_START)
        {
            return 0;
        }
        l = index - HIGH_CODE_START + 1 - 3;
        tmp = calc_crc16((U8 *)HIGH_CODE_START, l, 0xffff);
//        if(l < 0x10000)
//        {
//            tmp = calc_crc16((U8 *)HIGH_CODE_START, l, 0xffff);
//        }
//        else                                            //���볬��64K, Ŀǰ֧�ֽӽ�128K
//        {
//            tmp = calc_crc16((U8 *)HIGH_CODE_START, 0xffff, 0xffff);
//            tmp = calc_crc16((U8 *)(HIGH_CODE_START+0xffff), l - 0xffff, tmp);
//        }
        tmp = calc_crc16((U8 *)(HIGH_CODE_END - 64 + 1), 64, tmp);
        if(tmp == ((*((U8 *)(index - 2))) * 256 + *((U8 *)(index - 1))))
        {
            return 1;
        }
    }
    else
    {
        for(index = LOW_CODE_END - 64; index != LOW_CODE_START; index--)
        {
            m = *((U8 *)index);
            if(m != 0xff)
            {
                if(m != 0xaa)                           //��β�Ϳ�ʼ�ҵ��ĵ�һ������aa
                {
                    return 0;
                }
                break;
            }
        }
        if(index == LOW_CODE_START)
        {
            return 0;
        }
        l = index - LOW_CODE_START + 1 - 3;
        tmp = calc_crc16((U8 *)LOW_CODE_START, l, 0xffff);
//        if(l < 0x10000)
//        {
//            tmp = calc_crc16((U8 *)LOW_CODE_START, l, 0xffff);
//        }
//        else                                            //���볤�ȳ���64K, Ŀǰ֧�ֽӽ�128K
//        {
//            tmp = calc_crc16((U8 *)LOW_CODE_START, 0xffff, 0xffff);
//            tmp = calc_crc16((U8 *)(LOW_CODE_START+0xffff), l - 0xffff, tmp);
//        }
        tmp = calc_crc16((U8 *)(LOW_CODE_END - 64 + 1), 64, tmp);
        if(tmp == ((*((U8 *)(index - 2))) * 256 + *((U8 *)(index - 1))))
        {
            return 1;
        }
    }
    return 0;
}

/******************************************************************************
����: void set_normal_run_flag(U32 c)
���: c- bit0-Zigbee��ͨ��, bit1-��Aģ����ͨ��, bit2-������������
    ���Ƽ�һ�δ��붼�Ѿ���������
����: ��
����: ��鵱ǰ������������Ч���б�ʶ, û���õı�ʶ���ж�������, ʹ��Sample_Data
******************************************************************************/
void set_normal_run_flag (U32 c)
{
    U8 *p;

    run_normal_ram |= c;
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
	if((run_normal_ram & 0x3) != 0x3)
#else
    if((run_normal_ram & 0x7) != 0x7)
#endif
    {
        return;
    }
    p = (U8*)Sample_Data;
    p[4] = *((U8 *)CRC_AND_RUN_INVALID_FLAG);
    if(is_run_position() == 1)                          //�������ڵ���
    {
        if((p[4] & (1 << 0)) == 0)                      //�Ѿ���ʶ����
        {
            return;
        }
        memcpy(p, (void *)RUN_USING_FLAG, FLAG_FLASH_MAX_READ); //������������
        p[4] &= ~(1 << 0);
        program_run_using_flag(p);
        return;
    }
    if((p[4] & (1 << 1)) == 0)                          //�����ѱ�ʶ��Ч����
    {
        return;
    }
    memcpy(p, (void *)RUN_USING_FLAG, FLAG_FLASH_MAX_READ);     //������������
    p[4] &= ~(1 << 1);                                  //д����Ч���б�ʶ
    program_run_using_flag(p);
    return;
}

/******************************************************************************
����: void soft_reset_function(void)
���: ��
����: ��
����: ��soft_reset_countҪ�󵹼�ʱ׼����λ
******************************************************************************/
void soft_reset_function (void)
{
    U32 tmp;

    if(soft_reset_count == 0)                           //û�и�λҪ��
    {
        return;
    }
    tmp = Sys_Tick - tmp_tick;                          //��ȥ��ʱ��
    tmp_tick = Sys_Tick;
    if(tmp < soft_reset_count)                          //û����λʱ��
    {
        soft_reset_count -= tmp;
        return;
    }
    soft_reset_count = 0;
#ifndef DISPLAY_TERMINAL
    __disable_irq();
    STCTRL = 0x04;                                      //�ر�STI
    CLRENA0 = 0xffff8000;                               //STI���϶��ر�
    ((U32 *)0xE000E180)[1] = 0xffffffff;
    ((U32 *)0xE000E180)[2] = 0xffffffff;
    ((U32 *)0xE000E180)[3] = 0xffffffff;
    __enable_irq();                                     //���ж�, ��û�������ж���
    __set_FAULTMASK(1);
    tmp = RSID;                                         //�ڴ�֮ǰ������Ӧ�Ѽ�¼��������λԭ��
    RSID = tmp & 0x0f;                                  //�帴λԭ���CPU�Ĵ���
    chip_reset(SOFT_RESET);
#else//DISPLAY_TERMINAL
    operation_at_admin(SOFT_RST_ADMIN); //������ж�ȥ���� �����λ����
#endif//DISPLAY_TERMINAL
}

#ifdef FIRST_RESPONSE
/******************************************************************************
����: U32 check_package_code(U8 buf[], U8 com, U32 len)
���: buf-ָ��ʼΪ��ַ com-���ں� len-���ĳ���
����: 0-��ȷ����, ����Ϊ����(����)����, bit3-CRCУ�����, bit1-�õ�ַ��ֹд, bit5-�װ���ַ����
    bit6-����̫С, bit7-����æ,����д
����: ���մ���, ������ϸ�Ĵ��뱣����Sample_Data + 4096 �Ļ�����, �����Ժ�д��
    68+ ͨ�ŵ�ַ6+ 68+ C+ ����1+ �����ʶ��4+����4+�����ߴ���4+��չ��ʶ��2+ ��ַ4+����192+CRC2+ CS+16
    CRC�Ǹ��ֽ���ǰ ʹ��Sample_Data, ��Ҫ͸�����������չ������2�ֽڳ���, 2�ֽ�ID
    ֱ�Ӵ���len, ���ٰ�����Լ��Ϣ, �׵�ַ��Ϊֱ���ǻ������׵�ַ 14/03-2011
    ����ǩ������ֱ�ӱ������ٻ�Ӧ
******************************************************************************/
U32 check_package_code (U8 buf[], U8 com, U32 len)
{
    U32 tmp, i;
    U32 addr;
    U8 *p;

    if(len < 7)                                         //�����ܰ�������, �ѱ�֤ len > 6
    {
        return (1 << 6);
    }
    tmp = calc_crc16(buf, len - 2, 0xffff);             //CRC����, ����0xffffΪ��ֵ
    if(tmp != (buf[len - 1] + buf[len - 2] * 256))      //CRC����, CRC���ֽ���ǰ
    {
        return (1 << 3);
    }

    addr = (buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + buf[3];    //���ֽ���ǰ
    if(address_to_section_number(addr) & (1 << 8))      //��ַ����Ŀǰ��������Χ
    {
#ifdef IDEA_ACC
        if((last_addr == 0) && (29 == len))             //��ʱ������ǩ��֡
        {
            if(addr < CODE_START_ADDRSS)                //���ܿ���ǩ��
            {
                return (1 << 1);
            }
            if(addr > CODE_END_ADDRESS)
            {
                return (1 << 1);
            }
            if((addr & 0xff) != 0)                      //���ַ�϶�����
            {
                return (1 << 1);
            }
            for(i = 0; i < 64; i++)                     //���ַ������������
            {
                if(*((U32 *)(addr + i * 4)) != -1u)
                {
                    break;
                }
                *((U32 *)(data_code + i)) = -1u;        //��data_code[]ΪFF
            }
            if(i != 64)
            {
                return (1 << 1);
            }
            memcpy(data_code, buf + 4, 23);             //����23�ֽڹ�ȥ
            tmp = 0x10 + ((addr - 0x10000) >> 15);      //������, Ŀǰ����º���, �����С�Ͳ���

            param_table[0] = PREPARE_SECTOR_FOR_WRITE;  //����ֻ�ܵ������±�д����
            param_table[1] = param_table[2] = tmp;
            param_table[3] = FCCLK / 1024;
            iap_entry(param_table, result_table);

            param_table[0] = COPY_RAM_TO_FLASH;         //���FLASH
            param_table[1] = addr;
            param_table[2] = (U32)data_code;
            param_table[3] = 256;
            param_table[4] = FCCLK / 1024;
            iap_entry(param_table, result_table);
            code_signature();                           //����ɨ��ǩ��
            buf[4] = buf[5] = 0;                        //����Ҫ�ȴ�
            return 0;
        }
#endif  //IDEA_ACC
        return (1 << 1);
    }

    p = (U8 *)data_code;
    if(last_addr == 0)                                  //Ҫ�������װ�
    {
        if(is_run_position() == 1)                      //�����ڵ���
        {
            if(addr != HIGH_CODE_START)                 //Ҫ�´����׵�ַ���Ǹ����׵�ַ
            {
                return (1 << 5);
            }
        }
        else
        {
            if(addr != LOW_CODE_START)                  //Ҫ�´����׵�ַ���ǵ����׵�ַ
            {
                return (1 << 5);
            }
        }
        data_code_len = 0;                              //���������볤����0
        last_pa = 0;
    }
    else if((last_addr & 0xffffff00) != (addr & 0xffffff00))
    {
        if((last_addr + data_code_len) != addr)         //���ǹ��ص�����֡
        {
            for(i = 0; i < (len - 6); i++)              //�Ƿ�Ϊ�ط�֡
            {
                if(((addr & 0xff) + i) < 256)           //�Ѿ�д�뵽FLASH��
                {
                    if(buf[4 + i] != *((U8 *)(addr + i)))   //�����в�һ����
                    {
                        break;
                    }
                }
                else                                    //���뻹�ڻ�������
                {
                    if(buf[4 + i] != Sample_Data[FLAG_FLASH_MAX_READ + ((addr + i) & 0xff)])
                    {
                        break;
                    }
                }
            }
            if(i == (len - 6))                          //���붼��ͬ
            {
                if(addr != UPGRADE_INFO_ADDR)           //ֻ����������Ϣ֡�Ƿֿ��, ���������ط�֡
                {
                    return 0;
                }
            }
            for(i = 0; i < 256 / 4; i++)                //��һ��δ��256�ֽڵ�ҳ����Ҫ��д, ������ϻ���
            {
                data_code[i] = 0xffffffff;
            }
            last_pa = last_addr & 0xffffff00;           //δ��������׵�ַ, ������ᵽ256������
            if((data_code_len + (last_addr & 0xff)) <= 256)
            {
                for(i = 0; i < data_code_len; i++)
                {
                    p[(last_addr & 0xff) + i] = Sample_Data[FLAG_FLASH_MAX_READ + i];
                }
            }
            data_code_len = 0;                          //����(��)�������Ĵ��붼���Ƴ�
        }
        else                                            //������֡, ����ǰ֡��û���ü����
        {
            if(TEST_FLAG_BIT(upgrade_sign, PROGRAM_CODE_AFTER_ANSWER))
            {
                return (1 << 7);                        //����æ, ��ʱ������֡
            }
        }
    }
    if(((last_addr + data_code_len) & 0xff) == (addr & 0xff))
    {
        for(i = 0; i < (len - 6); i++)
        {
            Sample_Data[FLAG_FLASH_MAX_READ + data_code_len + i] = buf[4 + i];
        }
        data_code_len += (len - 6);                     //���������볤��
    }
    else if(UPGRADE_INFO_ADDR == addr)                  //��ַ������, ��������������Ϣ��
    {
        if(((last_pa + 256) & (~0xff)) != (addr & (~0xff))) //���256�ֽڿ��ڲ�������Ч����
        {
            for(i = 0; i < 256; i++)                        //���ff, ֻ��������64�ֽ���Ϣ��
            {
                Sample_Data[FLAG_FLASH_MAX_READ + i] = 0xff;
            }
        }
        else                                            //������鵽ĩβ�����FF
        {
            for(i = data_code_len; i < 256; i++)
            {
                Sample_Data[FLAG_FLASH_MAX_READ + i] = 0xff;
            }
        }
        for(i = 0; i < (len - 6); i++)                  //��64�ֽڵİ汾��Ϣ��������
        {
             Sample_Data[FLAG_FLASH_MAX_READ + (addr & 0xff) + i] = buf[4 + i];
        }
        data_code_len = 256;
    }
    last_addr = addr & 0xffffff00;
    buf[4] = buf[5] = 0;                                //һ������ȴ�
    if(data_code_len >= 256)                            //����256�ֽڵĴ����д��һ��
    {
        if(TEST_FLAG_BIT(upgrade_sign, PROGRAM_CODE_AFTER_ANSWER) == 0) //�ϻ�����Ϊ��
        {
            upgrade_sign &= ~COM_NUM_ON_BIT;            //��Ǵ��ں��Ӻ�д��
            SET_FLAG_BIT(upgrade_sign, PROGRAM_CODE_AFTER_ANSWER|(com << 1));
        }
        else
        {
            return (1 << 7);                            //ǰ֡ûд��, �µ�֡������, ��ʶæ
        }
        if(last_pa == 0)                                //�װ�Ҫ����
        {
            buf[4] = 160;
        }
        else if(address_to_section_number(last_pa & ~(0xffu)) & (1u << 10))
        {
            buf[4] = 160;                               //��Ҫ������
        }
        else                                            //ֻ��Ҫ���
        {
            buf[4] = 30;
        }
    }
    return 0;
}

/******************************************************************************
����: void program_package_code(void)
���: ��
����: ��
����: ��ѯ����Ӧ֡������Ϻ�, ����256�ֽڵĴ���д��FLASH
    ���ʱ�������Ҳ������
******************************************************************************/
void program_package_code (void)
{
#ifdef IDEA_ACC
    static U8 sec;
#endif  //IDEA_ACC
    U8 *p;
    U32 tmp, i;

#ifdef IDEA_ACC
    if(sec != Calendar.Sec)                             //ǩ����ʶ�����ڼ��
    {
        if(Random_Status.life)                          //�����
        {
            Random_Status.life--;
        }
        if(Sys_Class & (1 << ENABLE_UPDATE_CODE))       //����
        {
            if(Random_Status.upgrade_life)
            {
                Random_Status.upgrade_life--;
            }
            else
            {
                Sys_Class &= ~(1 << ENABLE_UPDATE_CODE);
            }
        }
        if(Sys_Class & (1 << ENABLE_CLEAR_METER))       //����
        {
            if(Random_Status.clear_life)
            {
                Random_Status.clear_life--;
            }
            else
            {
                Sys_Class &= ~(1 << ENABLE_CLEAR_METER);
            }
        }
        if(Sys_Class & (1 << ENABLE_PROGRAM))           //���
        {
            if(Random_Status.programe_life)
            {
                Random_Status.programe_life--;
            }
            else
            {
                Sys_Class &= ~(1 << ENABLE_PROGRAM);
            }
        }
        sec = Calendar.Sec;
    }
#endif  //IDEA_ACC

//#ifdef PRINT_SLAVE_DBG
//    if(0 != CrdyCount)
//    {
//        DBGTHR = '\r';
//        DBGTHR = '\n';
//        DBGTHR = Ascii[(CrdyCount >> 28) & 0xf];
//        DBGTHR = Ascii[(CrdyCount >> 24) & 0xf];
//        DBGTHR = Ascii[(CrdyCount >> 20) & 0xf];
//        DBGTHR = Ascii[(CrdyCount >> 16) & 0xf];
//        DBGTHR = Ascii[(CrdyCount >> 12) & 0xf];
//        DBGTHR = Ascii[(CrdyCount >> 8) & 0xf];
//        DBGTHR = Ascii[(CrdyCount >> 4) & 0xf];
//        DBGTHR = Ascii[CrdyCount & 0xf];
//        CrdyCount = 0;
//    }
//#endif  //PRINT_SLAVE_DBG

    if(TEST_FLAG_BIT(upgrade_sign, PROGRAM_CODE_AFTER_ANSWER) == 0) //û��Ҫд��
    {
        return;
    }
#ifdef DISPLAY_TERMINAL
    if(uart_send_idle((upgrade_sign >> 1) & 0x03, QUERY) == __FAIL)      //����æ�ڷ���
    {
        return;
    }
#else//DISPLAY_TERMINAL
    if(uart_open((upgrade_sign >> 1) & 0x03, QUERY) == __FAIL)      //����æ�ڷ���
    {
        return;
    }
#endif  //DISPLAY_TERMINAL
    CLR_FLAG_BIT(upgrade_sign, PROGRAM_CODE_AFTER_ANSWER);
    p = (U8*)Sample_Data;
    if(last_pa == 0)                                    //������մ����ַΪ0, Ҫ��̵����װ�, �ȸ�д�����ʶ��
    {
        memcpy(p, (void *)RUN_USING_FLAG, FLAG_FLASH_MAX_READ); //����ȫ����������, ��д��ʶ��
        if(is_run_position() == 1)                      //�����ڵ���, �����������õ�ַ
        {
            last_pa = HIGH_CODE_START;                  //�����ַ����ͨ��ǰ��ļ��
            p[4] |= 0x1a;                               //��ʶ�������´���, ����CRCû��֤, û���й�
        }
        else
        {
            last_pa = LOW_CODE_START;
            p[4] &= ~0x10;                              //���´���Ϊ����
            p[4] |= 0x05;                               //����û���й�, CRCҲδ��֤
        }
        program_run_using_flag(p);                      //��д��ʶ��
    }
    else if((last_pa + 256) != last_addr)               //��������Ϣ��, ��ǰ����ͬ�ϴ�д���ַ������                                               //������֡
    {
        tmp = address_to_section_number(last_pa & 0xffffff00);  //��β���ϵĲ�����֡д��
        if(tmp & (1 << 10))                             //������ʼ
        {
            prepare_sector(tmp & 0xff, tmp & 0xff);     //ָ��ҳ��
            erase_sector(tmp & 0xff, tmp & 0xff);       //����ҳ��
        }
        prepare_sector(tmp & 0xff, tmp & 0xff);         //ָ��ҳ��
        write_data_FW(last_pa & 0xffffff00, data_code, 256);
        for(i = 0; i < 256 / 4; i++)                    //�����뻺����
        {
            data_code[i] = 0xffffffff;
        }
        if((last_pa & 0xffff8000) != (last_addr & 0xffff8000))  //������һ֡�Ƿ���Ҫ����, ���ǵ�������Ϣ����
        {
            for(i = 0; i < 256 / 4; i++)                //���¿�ʼ�ĳ����Ӧ���Ƿ��ѱ�������
            {
                if(*((U32 *)(last_addr + i * 4)) != 0xffffffff)
                {
                    break;
                }
            }
            if(i != 256 / 4)                            //����Ҫ����, Ŀǰֻ�������ļ���Ϣ��
            {
                tmp = address_to_section_number(last_addr & 0xffff8000);
                prepare_sector(tmp & 0xff, tmp & 0xff);
                erase_sector(tmp & 0xff, tmp & 0xff);
            }
        }
    }
    while(data_code_len >= 256)                         //д������֡
    {
        p = (U8 *)data_code;
        tmp = i = last_addr & 0xff;                     //����Ҫ��д����
        while(i < 256)
        {
            p[i] = Sample_Data[FLAG_FLASH_MAX_READ + i - tmp];
            i++;
        }
        data_code_len -= (256 - tmp);                   //���ȼ�����
        for(i = 0; i < data_code_len; i++)              //����Ĵ���ǰ��
        {
            Sample_Data[FLAG_FLASH_MAX_READ + i] = Sample_Data[FLAG_FLASH_MAX_READ + 256 + i - tmp];
        }
        last_pa = last_addr & 0xffffff00;               //����̵�ַһ��256����
        last_addr += (256 - tmp);                       //��ַҲ����
        tmp = address_to_section_number(last_pa);
        if(tmp & (1 << 10))                             //������ʼ
        {
            prepare_sector(tmp & 0xff, tmp & 0xff);     //ָ��ҳ��
            erase_sector(tmp & 0xff, tmp & 0xff);       //����ҳ��
        }
        prepare_sector(tmp & 0xff, tmp & 0xff);         //ָ��ҳ��
        write_data_FW(last_pa, data_code, 256);
    }
    if(last_addr > UPGRADE_INFO_ADDR + 63)              //������Ͻ���ַ����
    {
        last_pa = 0;
    }
}

/******************************************************************************
����: U32 upgrading_on(void)
���: ��ǰ���뻺���ַ�ͳ���
����: 1 - ��������, 0 - ����
����: �б�ǰ�Ƿ���������, Ϊ�����������
******************************************************************************/
//U32 upgrading_on (void)
//{
//    if((last_addr <= CODE_END_ADDRESS) && (last_addr >= CODE_START_ADDRSS))
//    {
//        return 1;
//    }
//    return 0;
//}

#endif  //FIRST_RESPONSE

#ifdef IDEA_ACC
/******************************************************************************
����: void code_signature(void)
���: ��, ��ȡ���ֳ�������ǩ��оƬ
����: ��, ȫ�ֱ���Sys_Class
����: ��֤�����ǩ��оƬ�����󲻿ɷָ���
******************************************************************************/
void code_signature (void)
{
    U8 buf[64];
    U32 addr;
    U8 tmp;

    tmp = is_run_position();                            //��֤����CRC
    if(tmp == 0)                                        //����������������
    {
        Sys_Class |= (1u << SELF_CODE_SIGNATURE_FAIL);
        return;
    }
    if(check_code_crc(tmp - 1) != 1)                    //CRC������
    {
        Sys_Class |= (1u << SELF_CODE_SIGNATURE_FAIL);
        return;
    }
    memcpy(buf, HUA_CAI, 4);                            //����, 4�ֽ�
//    memcpy(buf + 4, (void *)0, 8);                      //ȡBOOT���������������, 16�ֽ�
    *((U32 *)(buf + 4)) = *((U32 *)0);                  //60020010 51010000
    *((U32 *)(buf + 8)) = *((U32 *)4);
    memcpy(buf + 12, (void *)CODE_START_ADDRSS, 8);
    file_read(DS2460_HEAD_ADDR + 0xf0, buf + 20, 8);    //DS2460оƬ��Ψһ��, 8�ֽ�
    memcpy(buf + 28, version_info, 16);                 //�汾��Ϣ��, 16�ֽ�
    memcpy(buf + 44, (void *)(CODE_START_ADDRSS + 0x3c), 4);  //TICK(3c) GPDMA(a8) T1(48)�ж���ڵ�ַ, 12+8�ֽ�
    memcpy(buf + 48, (void *)(CODE_START_ADDRSS + 0xa8), 4);
    memcpy(buf + 52, (void *)(CODE_START_ADDRSS + 0x48), 4 + 8);
    file_write(DS2460_HEAD_ADDR + 0, buf, 64);
    buf[0] = 0x8c;
    file_write(DS2460_HEAD_ADDR + 0x5c, buf, 1);
    delay(3300);
    addr = get_aa_addr(tmp);
    if(addr == 0)
    {
        Sys_Class |= (1u << SELF_CODE_SIGNATURE_FAIL);
        return;
    }
    memcpy(buf, (void *)(addr - 22), 20);
    file_write(DS2460_HEAD_ADDR + 0x5e, buf, 20);
    Sys_Class &= ~(1u << SELF_CODE_SIGNATURE_FAIL);
    if(I2C_Mem[I2C1].Error != NO_ERROR)
    {
        Sys_Class |= (1u << SELF_CODE_SIGNATURE_FAIL);  //ǩ����֤ûͨ��
    }
}

/******************************************************************************
����: void get_random(void)
���: ��, ֱ�ӱ����ھ�̬�ṹ��
����: ��, ֱ�Ӵ浽buf��8�ֽ�
����: �ڽ����ⲿ���ĺ˶�ǰ�����������, ��Ҫ�ȴ�ɢ���������, ����3.5mS
******************************************************************************/
//void get_random (void)
//{
//    U8 buf[64];
//
//                                                        //�����8�ֽڵĲ���������:
//    memcpy(buf, "��ˮ��", 8);
//    file_write(DS2460_HEAD_ADDR + 0x54, buf, 8);        //˽��Կ
//    get_on_chip_rtc(buf);                               //��ǰʱ��, 6�ֽ�, ���ڵ�λ�÷���, hex��ʽ��
//    buf[3] = buf[6];
//    buf[6] = Sys_Tick;                                  //Tick��, 2�ֽ�
//    buf[7] = Sys_Tick >> 8;
//#ifdef TEST_WWLLZZ
//    *((S32 *)(buf + 8)) = AD_Ctr[AD2].ha[0];            //��ʷAD2"����"��������, 8�ֽ�
//    *((S32 *)(buf + 12)) = AD_Ctr[AD2].ha[1];
//#endif  //TEST_WWLLZZ
//    file_read(DS2460_HEAD_ADDR + 0xf0, buf + 16, 8);    //DS2460оƬ��Ψһ��, 8�ֽ�
//    *((U32 *)(buf + 24)) = T1TC;                        //T1ֵ, 4�ֽ�
//    *((U32 *)(buf + 28)) = T0TC;                        //T0ֵ, 4�ֽ�
//    *((U32 *)(buf + 32)) = T2TC;                        //T2ֵ, 4�ֽ�
//    buf[36] = buf[3];                                   //������ʱ, 4�ֽ�
//    buf[37] = buf[5];
//    buf[38] = buf[4];
//    buf[39] = buf[2];
//    *((U32 *)(buf + 40)) = Data_From_CS5463[PHASE][ORG_TEMPERATURE];  //ԭʼ�¶�ֵ, 4�ֽ�
//    memcpy(buf + 44, "�㽭���ɿƼ�", 12);               //�㽭���ɿƼ�, 12�ֽ�
//#ifdef TEST_WWLLZZ
//    *((S32 *)(buf + 56)) = AD_Ctr[AD1].ha[0];           //��ʷAD1"����"��������, 8�ֽ�
//    *((S32 *)(buf + 60)) = AD_Ctr[AD1].ha[1];
//#endif  //TEST_WWLLZZ
//    file_write(DS2460_HEAD_ADDR + 0, buf, 64);
//    buf[0] = 0x84;                                      //��S0, ���㵽out
//    file_write(DS2460_HEAD_ADDR + 0x5c, buf, 1);
//    delay(3500);
//    file_read(DS2460_HEAD_ADDR + 0x40, Random_Status.random, 8);
//    if(I2C_Mem[I2C1].Error != NO_ERROR)
//    {
//        Random_Status.life = 0;                         //DS2460оƬ�й���
//    }
//    else
//    {
//        Random_Status.life = RANDOM_LIFE;
//    }
//}

/******************************************************************************
����: U32 signature_secret(U32 index, U8 buf[])
���: index - Ҫ��֤���뼶��, UPDATE_CLASS����, CLEAR_METER_CLASS����, PROGRAM_CLASS���. buf - ���Ƚ�20�ֽ�����
����: 1�˶���ȷ, 0����ȷ
����: �˶�Լ������ǩ��
******************************************************************************/
//U32 signature_secret (U32 index, U8 buf[])
//{
//    U8 p[64];                                           //����Ƕ�������ӳ���, �Ժ���ע��
//    U32 i;
//
//    if(Random_Status.life == 0)                         //���������
//    {
//        return 0;
//    }
//    Random_Status.life = 0;                             //�ͽ�����һ����֤, �Ժ��ʧЧ
//    if(index == UPDATE_CLASS)                           //����
//    {
//        memcpy(p, VERSION_INFO_S, 56);
////        memcpy(p + Calendar.Date, (void *)(Random_Status.random), 8);    //�ŵ�λ�������ڱ仯, �������Ż��ǻ���
//        for(i = 0; i < 8; i++)                          //��ܱ������Ż�����
//        {
//            p[Calendar.Date + i] = Random_Status.random[i];
//        }
//        file_read(DS2460_HEAD_ADDR + 0xf0, p + 56, 8);
//        file_write(DS2460_HEAD_ADDR + 0, p, 64);
//        p[0] = 0x8c;                                    //��E1���㵽out
//        file_write(DS2460_HEAD_ADDR + 0x5c, p, 1);
//        delay(3500);
//        i = file_write(DS2460_HEAD_ADDR + 0x5e, buf, 20);
//        if(i == NO_ERROR)
//        {
//            Sys_Class |= (1u << ENABLE_UPDATE_CODE);
//            Random_Status.upgrade_life = UPGRADE_LIFE;
//            return 1;
//        }
//    }
//    memcpy(p, HUA_CAI, 4);
//    if(index == CLEAR_METER_CLASS)                      //����
//    {
//        memcpy(p + 4, Random_Status.random, 8);         //Լ���������
//        memcpy(p + 12, XIAO_PIP, 44);
//        p[12] ^= RTCYEAR;                               //������������Ϣ
//        p[17] ^= RTCMONTH;
//        p[38] ^= RTCDOM;
//        file_read(DS2460_HEAD_ADDR + 0xf0, p + 56, 8);
//        file_write(DS2460_HEAD_ADDR + 0, p, 64);
//        p[0] = 0x94;                                    //��E2�����out
//        file_write(DS2460_HEAD_ADDR + 0x5c, p, 1);
//        delay(3500);
//        i = file_write(DS2460_HEAD_ADDR + 0x5e, buf, 20);   //�˶�20�ֽ�ǩ��
//        if(i == NO_ERROR)
//        {
//            Sys_Class |= (1u << ENABLE_CLEAR_METER);       //������׶�
//            Random_Status.clear_life = CLEAR_LIFE;
//            return 1;
//        }
//    }
//    if(index == PROGRAM_CLASS)                          //���
//    {
//        memcpy(p + 4, DATE_PIP, 52);                    //������ʱ�������20�ֽ�
//        p[12] ^= RTCYEAR;                               //������������Ϣ
//        p[17] ^= RTCMONTH;
//        p[38] += RTCDOM;
//        memcpy(p + 24, Random_Status.random, 8);        //�����
//        file_read(DS2460_HEAD_ADDR + 0xf0, p + 56, 8);
//        file_write(DS2460_HEAD_ADDR + 0, p, 64);
//        p[0] = 0x9c;                                    //��E3���㵽out
//        file_write(DS2460_HEAD_ADDR + 0x5c, p, 1);
//        delay(3500);
//        i = file_write(DS2460_HEAD_ADDR + 0x5e, buf, 20);   //�˶�20�ֽ�ǩ��
//        if(i == NO_ERROR)
//        {
//            Sys_Class |= (1u << ENABLE_PROGRAM); //������
//            Random_Status.programe_life = PROGRAM_LIFE;
//            return 1;
//        }
//    }
//    return 0;
//}

/******************************************************************************
����: U32 get_aa_addr(U32 zone)
���: zone - ������, 1-����, 2-����, ������Ч
����: ��ַ
����: ��������ִ�д��뿪ʼ��ǰ�ҵ�һ������ff��λ��
******************************************************************************/
U32 get_aa_addr (U32 zone)
{
    U32 i;

    if(zone == 2)                                       //����
    {
        for(i = HIGH_CODE_END - 64 - 3; i != HIGH_CODE_START; i -= 4)
        {
            if(*((U32 *)i) != 0xffffffff)
            {
BYTE_SEARCH_ADDR:
                i += 3;
                if(*((U8 *)i) == 0xaa)
                {
                    return i;
                }
                i--;
                if(*((U8 *)i) == 0xaa)
                {
                    return i;
                }
                i--;
                if(*((U8 *)i) == 0xaa)
                {
                    return i;
                }
                i--;
                if(*((U8 *)i) == 0xaa)
                {
                    return i;
                }
                return 0;
            }
        }
    }
    if(zone == 1)                                       //����
    {
        for(i = LOW_CODE_END - 64 - 3; i != LOW_CODE_START; i -= 4)
        {
            if(*((U32 *)i) != 0xffffffff)
            {
                goto BYTE_SEARCH_ADDR;
            }
        }
    }
    return 0;
}
#endif  //IDEA_ACC


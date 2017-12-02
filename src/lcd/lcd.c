/******************************************************************************
 Copyright (C) 2011  R&D Institute of HuaCai., Ltd.
 Module         : lcd display function
 File Name      : lcd_display.c
 Description    :
 Author         :
 Build Date     :
 others         :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��     1.00       2011-05-09     azh       create
******************************************************************************/

/*---------------------------------------------------------------------------*/
#include    "..\inc\global_config.h"
#include    "..\src\bsp\event_define_d.h"               //�й��¼���Ķ���

#ifdef LCD_VALID
#define DATA_SOURCE_NOT_LOCAL//���û�ж�����ȫ����ȡ����EEPROM�������

//------------------------------ ˽�б������� ---------------------------------
U8 lcd_seg_buf[LCD_BUF_LENTH];   //nbytes CMD + seg0~seg31


/*private functions defined in this file:------------------------------------*/
static S8 fill_dis_fix_b8_data(U32 di, DIS_CTRL_TCB *pdistable, U8 *segbuf);
static S8 fill_dis_fix_l8_data(U32 di, U8 *segbuf);
static U16 get_dis_sn_bydi (U32 di, U8 lsn, DIS_CTRL_TCB const *pdistable, U16 max_num);
static S8 get_dis_info (U8 *di_buf, U8 *fix_data, DIS_CTRL_TCB *pdis_tcb, U8 *segbuf);

void get_dis_code(U8 mode);
static U8 judge_data_sign(U8 *buf, DIS_CTRL_TCB *pdistable);
static void lcd_backlight_ctrl(U8 mode);
static void fill_search_addr_segbuf(U8 *segbuf);
static void fill_search_set_state_segbuf(U8 *segbuf);
//gprs �ź�ǿ�ȱ��
const U8 gprs_signal_dis_tab[6][5] =
{
    0, 0, 0, 0, 0,   //���ź�
    SIGNAL_DIS_I1, 0, 0, 0, 0,   //1���ź�
    SIGNAL_DIS_I1, SIGNAL_DIS_I2, 0, 0, 0,   //2���ź�
    SIGNAL_DIS_I1, SIGNAL_DIS_I2, SIGNAL_DIS_I3, 0, 0,   //3���ź�
    SIGNAL_DIS_I1, SIGNAL_DIS_I2, SIGNAL_DIS_I3, SIGNAL_DIS_I4, 0,   //4���ź�
    SIGNAL_DIS_I1, SIGNAL_DIS_I2, SIGNAL_DIS_I3, SIGNAL_DIS_I4, SIGNAL_DIS_I5,   //5���ź�
};
//7�����״̬�� ��Ҫ�ж���˸��״̬λ
//��7��״̬���е��ܵ�λ���
const U8 meter_state_flash_bit_tab[CON_FLASH_BIT_MAX_NUM] =
{
    2, 3,   //ʱ�ӵ��Ƿѹ ������Ƿѹ
    48, 51, //A��ʧѹ ʧ��
    64, 67, //B��ʧѹ ʧ��
    80, 83, //C��ʧѹ ʧ��
    96,     //97  //��ѹ ���������� �����ǹ���һ����ʾ�ַ�������
};
const U8 meter_state_flash_bit_spectab[CON_FLASH_BIT_MAX_NUM] =
{
    0,  0,
    55, 56, //A����� ����
    71, 72, //B����� ����
    87, 88, //C����� ����
    0,
};
//Ĭ�ϻ�����״̬��ʾ��� 0������ʾ 1����ʾ
const U8 flash_bit_default_value_tab[CON_FLASH_BIT_MAX_NUM] =
{
    0, 0,         //ʱ�ӵ��Ƿѹ ������Ƿѹ Ĭ�ϻ�����״̬����ʾ
    0x81, 0x81,   //A��ʧѹ ʧ�� Ĭ�ϻ�����״̬��ʾ ���λ��1˵�������ж�һ��״̬����ȷ�� �����
    0x81, 0x81,   //B��ʧѹ ʧ�� Ĭ�ϻ�����״̬��ʾ
    0x81, 0x81,   //C��ʧѹ ʧ�� Ĭ�ϻ�����״̬��ʾ
    0,            //��ѹ ���������� Ĭ�ϻ�����״̬����ʾ
};
const DIS_CTRL_TCB disctrbydi_spec_di304table[2] =
{
    {//04000401
        0x04000401, //byte 0  0xDI3DI2DI1XX ��ʶ
        PAR_HVMADDR_04000401_00_ADDR, //byte 1  0xAdr3Adr2Adr1Adr0 ������
        0x00, //byte 2 0xLitSn �������ݵĲ�����  �������ݲ�����
        0x00, //byte 3 0xSpecialCtrl
        0x0000, // byte 4 0xFixCharType �̶�������ʾ�������FixCharTable����±�
         ((PAR_HVMADDR_04000401_00_LEN*2) << 4) + PAR_HVMADDR_04000401_00_LEN,//byte 5 0xDataLen ������
        0xff, //byte 6 0xOffsetTabSn

    },
    {//04000401
        0x04000401, //byte 0  0xDI3DI2DI1XX ��ʶ
        PAR_HVMADDR_04000401_01_ADDR, //byte 1  0xAdr3Adr2Adr1Adr0 ������
        0x01, //byte 2 0xLitSn �������ݵĲ�����  �������ݲ�����
        0x00, //byte 3 0xSpecialCtrl
        0x0000, // byte 4 0xFixCharType �̶�������ʾ�������FixCharTable����±�
         ((PAR_HVMADDR_04000401_01_LEN*2) << 4) + PAR_HVMADDR_04000401_01_LEN,//byte 5 0xDataLen ������
        0xff, //byte 6 0xOffsetTabSn

    },
};
//5�ֽ� С���������ֽ�
const U8 default_auto_dis_tab[DEFAULT_AUTO_DIS_MAX_NUM][5] =
{
    0x01, 0x01, 0x00, 0x04, 0x00,   //0x04000101 00 ��ǰ����
    0x02, 0x01, 0x00, 0x04, 0x00,   //0x04000102 00 ��ǰʱ��

    0x00, 0x00, 0x00, 0x00, 0x00,   //0x00000000 00 ��ǰ����й��ܵ���
    0x00, 0x00, 0x01, 0x00, 0x00,   //0x00010000 00 ��ǰ�����й��ܵ���
    0x00, 0x01, 0x01, 0x00, 0x00,   //0x00010100 00 ��ǰ�����й������
    0x00, 0x02, 0x01, 0x00, 0x00,   //0x00010200 00 ��ǰ�����й������
    0x00, 0x03, 0x01, 0x00, 0x00,   //0x00010300 00 ��ǰ�����й�ƽ����
    0x00, 0x04, 0x01, 0x00, 0x00,   //0x00010400 00 ��ǰ�����й��ȵ���

    0x00, 0x00, 0x01, 0x01, 0x00,   //0x01010000 00 ��ǰ�����й����������

    0x00, 0x00, 0x05, 0x00, 0x00,   //0x00050000 00 ��ǰ��1�����޹��ܵ���
    0x00, 0x00, 0x06, 0x00, 0x00,   //0x00060000 00 ��ǰ��2�����޹��ܵ���
    0x00, 0x00, 0x07, 0x00, 0x00,   //0x00070000 00 ��ǰ��3�����޹��ܵ���
    0x00, 0x00, 0x08, 0x00, 0x00,   //0x00080000 00 ��ǰ��4�����޹��ܵ���
    0x00, 0x00, 0x02, 0x00, 0x00,   //0x00020000 00 ��ǰ�����й��ܵ���
    0x00, 0x01, 0x02, 0x00, 0x00,   //0x00020100 00 ��ǰ�����й������
    0x00, 0x02, 0x02, 0x00, 0x00,   //0x00020200 00 ��ǰ�����й������
    0x00, 0x03, 0x02, 0x00, 0x00,   //0x00020300 00 ��ǰ�����й�ƽ����
    0x00, 0x04, 0x02, 0x00, 0x00,   //0x00020400 00 ��ǰ�����й��ȵ���

};
#if CODE_VER_TYPE==WLS_CUR_PRJ			//������߲ɼ���
//�̶���ʾ���漸�������߲ɼ����汾��
const U8 default_key_dis_tab[DEFAULT_KEY_DIS_MAX_NUM][5] =
{
    0x01, 0x01, 0x00, 0x04, 0x00,    //0x04000101 00 ��ǰ����
    0x02, 0x01, 0x00, 0x04, 0x00,    //0x04000102 00 ��ǰʱ��

//���߲ɼ�����ʾ����
    0x01, 0x04, 0x00, 0x04, 0x00,   //0x04000401 00 ���ܱ�ͨ�ŵ�ַ����ţ���8λ

    0x00, 0x01, 0x02, 0x02, 0x00,   //0x02020100 00 A�����
    0x00, 0x02, 0x02, 0x02, 0x00,   //0x02020200 00 B�����
    0x00, 0x03, 0x02, 0x02, 0x00,   //0x02020300 00 C�����
    0x00, 0x00, 0x05, 0x02, 0x00,   //0x02050000 00 ˲ʱ�����ڹ���

//�����ܱ���ʾ����
    0x01, 0x04, 0x00, 0x04, 0x01,   //0x04000401 01 ���ܱ�ͨ�ŵ�ַ����ţ���4λ
    0x01, 0x04, 0x00, 0x04, 0x00,   //0x04000401 00 ���ܱ�ͨ�ŵ�ַ����ţ���8λ
//    0x00, 0x00, 0x00, 0x00, 0x00,    //0x00000000 00 ��ǰ����й��ܵ���
    0x00, 0x00, 0x01, 0x00, 0x00,    //0x00010000 00 ��ǰ�����й��ܵ���

    0x00, 0x00, 0x02, 0x00, 0x00,   //0x00020000 00 ��ǰ�����й��ܵ���

    0x00, 0x00, 0x03, 0x00, 0x00,   //0x00030000 00 ��ǰ����޹�1�ܵ���//�����޹��� �����޹���
    0x00, 0x00, 0x04, 0x00, 0x00,   //0x00040000 00 ��ǰ����޹�2�ܵ���

//    0x00, 0x00, 0x05, 0x00, 0x00,   //0x00050000 00 ��ǰ��1�����޹��ܵ���
//    0x00, 0x00, 0x06, 0x00, 0x00,   //0x00060000 00 ��ǰ��2�����޹��ܵ���
//    0x00, 0x00, 0x07, 0x00, 0x00,   //0x00070000 00 ��ǰ��3�����޹��ܵ���
//    0x00, 0x00, 0x08, 0x00, 0x00,   //0x00080000 00 ��ǰ��4�����޹��ܵ���

    0x00, 0x01, 0x01, 0x02, 0x00,   //0x02010100 00 A���ѹ
    0x00, 0x02, 0x01, 0x02, 0x00,   //0x02010200 00 B���ѹ
    0x00, 0x03, 0x01, 0x02, 0x00,   //0x02010300 00 C���ѹ
    0x00, 0x01, 0x02, 0x02, 0x00,   //0x02020100 00 A�����
    0x00, 0x02, 0x02, 0x02, 0x00,   //0x02020200 00 B�����
    0x00, 0x03, 0x02, 0x02, 0x00,   //0x02020300 00 C�����

    0x00, 0x00, 0x03, 0x02, 0x00,   //0x02030000 00 ˲ʱ���й�����
    0x00, 0x00, 0x04, 0x02, 0x00,   //0x02040000 00 ˲ʱ���޹�����
};


#else
const U8 default_key_dis_tab[DEFAULT_KEY_DIS_MAX_NUM][5] =
{
    0x01, 0x01, 0x00, 0x04, 0x00,    //0x04000101 00 ��ǰ����
    0x02, 0x01, 0x00, 0x04, 0x00,    //0x04000102 00 ��ǰʱ��

    0x00, 0x00, 0x00, 0x00, 0x00,    //0x00000000 00 ��ǰ����й��ܵ���
    0x00, 0x00, 0x01, 0x00, 0x00,    //0x00010000 00 ��ǰ�����й��ܵ���
    0x00, 0x01, 0x01, 0x00, 0x00,    //0x00010100 00 ��ǰ�����й������
    0x00, 0x02, 0x01, 0x00, 0x00,    //0x00010200 00 ��ǰ�����й������
    0x00, 0x03, 0x01, 0x00, 0x00,    //0x00010300 00 ��ǰ�����й�ƽ����
    0x00, 0x04, 0x01, 0x00, 0x00,    //0x00010400 00 ��ǰ�����й��ȵ���

    0x00, 0x00, 0x01, 0x01, 0x00,   //0x01010000 00 ��ǰ�����й����������
    0x00, 0x00, 0x01, 0x01, 0x02,   //0x01010000 01 ��ǰ�����й������������������
    0x00, 0x00, 0x01, 0x01, 0x01,   //0x01010000 02 ��ǰ�����й��������������ʱ��

    0x00, 0x00, 0x02, 0x00, 0x00,   //0x00020000 00 ��ǰ�����й��ܵ���
    0x00, 0x01, 0x02, 0x00, 0x00,   //0x00020100 00 ��ǰ�����й������
    0x00, 0x02, 0x02, 0x00, 0x00,   //0x00020200 00 ��ǰ�����й������
    0x00, 0x03, 0x02, 0x00, 0x00,   //0x00020300 00 ��ǰ�����й�ƽ����
    0x00, 0x04, 0x02, 0x00, 0x00,   //0x00020400 00 ��ǰ�����й��ȵ���

    0x00, 0x00, 0x02, 0x01, 0x00,   //0x01020000 00 ��ǰ�����й����������
    0x00, 0x00, 0x02, 0x01, 0x02,   //0x01020000 01 ��ǰ�����й������������������
    0x00, 0x00, 0x02, 0x01, 0x01,   //0x01020000 02 ��ǰ�����й��������������ʱ��

    0x00, 0x00, 0x05, 0x00, 0x00,   //0x00050000 00 ��ǰ��1�����޹��ܵ���
    0x00, 0x00, 0x06, 0x00, 0x00,   //0x00060000 00 ��ǰ��2�����޹��ܵ���
    0x00, 0x00, 0x07, 0x00, 0x00,   //0x00070000 00 ��ǰ��3�����޹��ܵ���
    0x00, 0x00, 0x08, 0x00, 0x00,   //0x00080000 00 ��ǰ��4�����޹��ܵ���

    0x01, 0x00, 0x01, 0x00, 0x00,   //0x00010001 00 ��1�������й��ܵ���
    0x01, 0x01, 0x01, 0x00, 0x00,   //0x00010101 00 ��1�������й������
    0x01, 0x02, 0x01, 0x00, 0x00,   //0x00010201 00 ��1�������й������
    0x01, 0x03, 0x01, 0x00, 0x00,   //0x00010301 00 ��1�������й�ƽ����
    0x01, 0x04, 0x01, 0x00, 0x00,   //0x00010401 00 ��1�������й��ȵ���

    0x01, 0x00, 0x01, 0x01, 0x00,   //0x01010001 00 ��1�������й����������
    0x01, 0x00, 0x01, 0x01, 0x02,   //0x01010001 01 ��1�������й������������������
    0x01, 0x00, 0x01, 0x01, 0x01,   //0x01010001 02 ��1�������й��������������ʱ��

    0x01, 0x00, 0x02, 0x00, 0x00,   //0x00020001 00 ��1�·����й��ܵ���
    0x01, 0x01, 0x02, 0x00, 0x00,   //0x00020101 00 ��1�·����й������
    0x01, 0x02, 0x02, 0x00, 0x00,   //0x00020201 00 ��1�·����й������
    0x01, 0x03, 0x02, 0x00, 0x00,   //0x00020301 00 ��1�·����й�ƽ����
    0x01, 0x04, 0x02, 0x00, 0x00,   //0x00020401 00 ��1�·����й��ȵ���

    0x01, 0x00, 0x02, 0x01, 0x00,   //0x01020001 00 ��1�·����й����������
    0x01, 0x00, 0x02, 0x01, 0x02,   //0x01020001 01 ��1�·����й������������������
    0x01, 0x00, 0x02, 0x01, 0x01,   //0x01020001 02 ��1�·����й��������������ʱ��

    0x01, 0x00, 0x05, 0x00, 0x00,   //0x00050001 00 ��1�µ�1�����޹��ܵ���
    0x01, 0x00, 0x06, 0x00, 0x00,   //0x00060001 00 ��1�µ�2�����޹��ܵ���
    0x01, 0x00, 0x07, 0x00, 0x00,   //0x00070001 00 ��1�µ�3�����޹��ܵ���
    0x01, 0x00, 0x08, 0x00, 0x00,   //0x00080001 00 ��1�µ�4�����޹��ܵ���

    0x01, 0x04, 0x00, 0x04, 0x00,   //0x04000401 00 ���ܱ�ͨ�ŵ�ַ����ţ���8λ
    0x01, 0x04, 0x00, 0x04, 0x01,   //0x04000401 01 ���ܱ�ͨ�ŵ�ַ����ţ���4λ
//    0x09, 0x04, 0x00, 0x04, 0x00,   //0x04000409 00 �й����峣��
//    0x0a, 0x04, 0x00, 0x04, 0x00,   //0x0400040a 00 �޹����峣��
    0x00, 0x01, 0x01, 0x02, 0x00,   //0x02010100 00 A���ѹ
    0x00, 0x02, 0x01, 0x02, 0x00,   //0x02010200 00 B���ѹ
    0x00, 0x03, 0x01, 0x02, 0x00,   //0x02010300 00 C���ѹ
    0x00, 0x01, 0x02, 0x02, 0x00,   //0x02020100 00 A�����
    0x00, 0x02, 0x02, 0x02, 0x00,   //0x02020200 00 B�����
    0x00, 0x03, 0x02, 0x02, 0x00,   //0x02020300 00 C�����
    0x00, 0x00, 0x03, 0x02, 0x00,   //0x02030000 00 ˲ʱ���й�����
    0x00, 0x01, 0x03, 0x02, 0x00,   //0x02030100 00 ˲ʱA���й�����
    0x00, 0x02, 0x03, 0x02, 0x00,   //0x02030200 00 ˲ʱB���й�����
    0x00, 0x03, 0x03, 0x02, 0x00,   //0x02030300 00 ˲ʱC���й�����
    0x00, 0x00, 0x06, 0x02, 0x00,   //0x02060000 00 ˲ʱ�ܹ�������
    0x00, 0x01, 0x06, 0x02, 0x00,   //0x02060100 00 ˲ʱA�๦������
    0x00, 0x02, 0x06, 0x02, 0x00,   //0x02060200 00 ˲ʱB�๦������
    0x00, 0x03, 0x06, 0x02, 0x00,   //0x02060300 00 ˲ʱC�๦������
};
#endif

//�¼���¼���ݱ�ʶ
//{ID,SNUM,CNT}
static const DIS_AUX_CTRL evet_lsn_tab[DIS_AUX_EVET_NUM] =
{
    {0x03010000,0,6},               //A,B,C��ʧѹ�ܴ��������ۼ�ʱ��
    {0x03010101,0,4},               //A��ʧѹ��¼:����&����ʱ��
    {0x03010201,0,4},               //B��ʧѹ��¼:����&����ʱ��
    {0x03010301,0,4},               //C��ʧѹ��¼:����&����ʱ��

    {0x03020000,0,6},               //A,B,C��Ƿѹ�ܴ��������ۼ�ʱ��
    {0x03020101,0,4},               //A��Ƿѹ��¼:����&����ʱ��
    {0x03020201,0,4},               //B��Ƿѹ��¼:����&����ʱ��
    {0x03020301,0,4},               //C��Ƿѹ��¼:����&����ʱ��

    {0x03030000,0,6},               //A,B,C���ѹ�ܴ��������ۼ�ʱ��
    {0x03030101,0,4},               //A���ѹ��¼:����&����ʱ��
    {0x03030201,0,4},               //B���ѹ��¼:����&����ʱ��
    {0x03030301,0,4},               //C���ѹ��¼:����&����ʱ��

    {0x03040000,0,6},               //A,B,C������ܴ��������ۼ�ʱ��
    {0x03040101,0,4},               //A������¼:����&����ʱ��
    {0x03040201,0,4},               //B������¼:����&����ʱ��
    {0x03040301,0,4},               //C������¼:����&����ʱ��

    {0x03050000,0,2},               //ȫʧѹ�ܴ��������ۼ�ʱ��
    {0x03050001,0,5},               //ȫʧѹ����ʱ�̣�����ֵ������ʱ��

    {0x03060000,0,2},               //������Դʧ���ܴ��������ۼ�ʱ��
    {0x03060001,0,4},               //������Դʧ�緢��ʱ�̣�����ʱ��

    {0x03070000,0,2},               //��ѹ�������ܴ��������ۼ�ʱ��
    {0x03070001,0,4},               //��ѹ�������¼������ʱ�̣�����ʱ��

    {0x03080000,0,2},               //�����������ܴ��������ۼ�ʱ��
    {0x03080001,0,4},               //�����������¼������ʱ�̣�����ʱ��

    {0x03090000,0,2},               //��ѹ��ƽ���ܴ��������ۼ�ʱ��
    {0x03090001,0,4},               //��ѹ��ƽ���¼������ʱ�̣�����ʱ��

    {0x030A0000,0,2},               //������ƽ���ܴ��������ۼ�ʱ��
    {0x030A0001,0,4},               //������ƽ���¼������ʱ�̣�����ʱ��

    {0x030B0000,0,6},               //A,B,C��ʧ���ܴ��������ۼ�ʱ��
    {0x030B0101,0,4},               //A��ʧ����¼:����&����ʱ��
    {0x030B0201,0,4},               //B��ʧ����¼:����&����ʱ��
    {0x030B0301,0,4},               //C��ʧ����¼:����&����ʱ��

    {0x030C0000,0,6},               //A,B,C������ܴ��������ۼ�ʱ��
    {0x030C0101,0,4},               //A�������¼:����&����ʱ��
    {0x030C0201,0,4},               //B�������¼:����&����ʱ��
    {0x030C0301,0,4},               //C�������¼:����&����ʱ��

    {0x030D0000,0,6},               //A,B,C������ܴ��������ۼ�ʱ��
    {0x030D0101,0,4},               //A�������¼:����&����ʱ��
    {0x030D0201,0,4},               //B�������¼:����&����ʱ��
    {0x030D0301,0,4},               //C�������¼:����&����ʱ��

    {0x030E0000,0,6},               //A,B,C�ೱ�������ܴ��������ۼ�ʱ��
    {0x030E0101,0,4},               //A�ೱ�������¼:����&����ʱ��
    {0x030E0201,0,4},               //B�ೱ�������¼:����&����ʱ��
    {0x030E0301,0,4},               //C�ೱ�������¼:����&����ʱ��

    {0x030F0000,0,6},               //A,B,C������ܴ��������ۼ�ʱ��
    {0x030F0101,0,4},               //A����ؼ�¼:����&����ʱ��
    {0x030F0201,0,4},               //B����ؼ�¼:����&����ʱ��
    {0x030F0301,0,4},               //C����ؼ�¼:����&����ʱ��

    {0x03100000,0,9},               //��ѹ�ϸ���ͳ�����ݿ�

    {0x03100100,0,9},               //A���ѹ�ϸ���ͳ�����ݿ�
    {0x03100200,0,9},               //B���ѹ�ϸ���ͳ�����ݿ�
    {0x03100300,0,9},               //C���ѹ�ϸ���ͳ�����ݿ�

    {0x03110001,0,4},               //�����¼������ʱ�̣�����ʱ��

    {0x03120002,0,6},               //�����������ݿ�

    {0x03120101,0,7},               //�����й��������޼�¼������ʱ�̣�����ʱ��
                                    //�ڼ������й��������������ʱ��

    {0x03300001,0,3},               //��̼�¼������ʱ�̣������ߴ���

    {0x03300101,0,3},               //��������¼������ʱ�̣������ߴ���

    {0x03300201,0,3},               //���������¼������ʱ�̣������ߴ���

    {0x03300301,0,3},               //�¼������¼������ʱ�̣������ߴ���

    {0x03300401,0,5},               //Уʱ��¼�������ߴ��룬Уʱǰ����ʱ��

    {0x03300501,0,3},               //ʱ�α��̼�¼������ʱ�̣������ߴ���

    {0x03300601,0,3},               //ʱ�����̼�¼������ʱ�̣������ߴ���

    {0x03300701,0,3},               //�����ձ�̼�¼������ʱ�̣������ߴ���

    {0x03300801,0,3},               //�ڼ��ձ�̼�¼������ʱ�̣������ߴ���

    {0x03300901,0,3},               //�й���Ϸ�ʽ��̼�¼������ʱ�̣������ߴ���

    {0x03300A01,0,3},               //�޹���Ϸ�ʽ1��̼�¼������ʱ�̣������ߴ���

    {0x03300B01,0,3},               //�޹���Ϸ�ʽ2��̼�¼������ʱ�̣������ߴ���

    {0x03300C01,0,3},               //�����ձ�̼�¼������ʱ�̣������ߴ���

    {0x03300D01,0,4},               //�Ὺ��Ǽ�¼������ʱ�̣�����ʱ��
};

//�α������ݱ�ʶ
//{ID,SNUM,CNT}
static const DIS_AUX_CTRL para_lsn_tab[DIS_AUX_PARA_NUM] =
{
    {0x04000106,0,2},               //����ʱ�����л�ʱ��
    {0x04000107,0,2},               //������ʱ�α��л�ʱ��

    {0x04000401,0,2},               //ͨ�ŵ�ַ
    {0x04000402,0,2},               //���
};
/******************************************************************************
** ��������: void set_rate_char(U8 rate, U8 *fix_data)
** ��������:
** ��ڲ���: rate=0:�� 1:��......
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void set_rate_char (U8 rate, U8 *fix_data)//
{
    switch(rate)
    {
        case 0://��
            fix_data[1] |= BY1_B6;
            break;
        case 1://��
            fix_data[1] |= BY1_B7;
            break;
        case 2:
            fix_data[2] |= BY2_B0;
            break;
        case 3:
            fix_data[2] |= BY2_B1;
            break;
        case 4:
            fix_data[2] |= BY2_B2;
            break;
    }
}
/******************************************************************************
** ��������: void set_totabc_char(U8 phase, U8 *fix_data)
** ��������:
** ��ڲ���: phase=0:�� 1:A��......
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void set_totabc_char (U8 phase, U8 *fix_data)
{
    switch(phase)
    {
        case 0://��
            fix_data[1] |= BY1_B6;
            break;
        case 1://A
            fix_data[2] |= BY2_B3;
            break;
        case 2:
            fix_data[2] |= BY2_B4;
            break;
        case 3:
            fix_data[2] |= BY2_B5;
            break;
    }
}
/******************************************************************************
** ��������: void set_eng_dmd_char (U8 *indi, U8 *fix_data, U8 *segbuf)
** ��������:
** ��ڲ���:
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void set_eng_dmd_char (U8 *indi, U8 *fix_data, U8 *segbuf)
{
    U8 dix;

    if((indi[4]!=0) && (indi[4]!=1))
    {
        return;
    }
    fix_data[3] |= BY3_B5;//��

    if(indi[1] == 0 )//�ǵ�ǰ ��ʾ��ǰ
    {
        fix_data[0] |= (BY0_B0 |BY0_B2);//��ǰ��
    }
    else//���ǵ�ǰ ���ϼ�����
    {
        fix_data[0] |= (BY0_B1 |BY0_B2);//����
        fill_data_last8_segbuf(indi[1], segbuf);//n
    }

    if(indi[4] == 0)
    {
//�������õ��ַ����
        fix_data[3] |= BY3_B4;//��
        fix_data[5] |= BY5_B4;//С����xxxxxx.xx
        if(indi[3] <= 0x80)
        {
            if(indi[3] <= 0x0A)//����
            {
                if(indi[2] > RATE_NUM)
                {
                    return;
                }
                set_rate_char(indi[2], fix_data);//�ܼ��ƽ��
                dix = indi[3];
            }
            else//ABC
            {
                dix = indi[3] - 0x15;
                if(dix != 0)
                {
                    set_totabc_char((U8)(dix/0x14), fix_data);
                }
                dix %= 0x14;
            }
            switch(dix)//��λ+����
            {
                case 0x00://����й�
                    fix_data[0] |= BY0_B3;//���
                    fix_data[1] |= (BY1_B0 | BY1_B1);//�й�
                    fix_data[6] |= CON_UNIT_KWH;
                    break;
                case 0x01://�����й�
                    fix_data[0] |= (BY0_B5 | BY0_B6);//����
                    fix_data[1] |= (BY1_B0 | BY1_B1);//�й�
                    fix_data[6] |= CON_UNIT_KWH;

                    break;
                case 0x02://�����й�
                    fix_data[0] |= (BY0_B4 | BY0_B6);//����
                    fix_data[1] |= (BY1_B0 | BY1_B1);//�й�
                    fix_data[6] |= CON_UNIT_KWH;
                    break;
                case 0x03://����޹�1
                case 0x04://����޹�2
                    fix_data[0] |= BY0_B3;//���
                    fix_data[0] |= BY0_B7;//��
                    fix_data[1] |= BY1_B1;//��
                    fix_data[6] |= CON_UNIT_KVARH;
                    break;
                case 0x05://һ�����޹�
                    fix_data[0] |= BY0_B7;//��
                    fix_data[1] |= BY1_B1;//��
                    fix_data[1] |= BY1_B2;//һ����
                    fix_data[6] |= CON_UNIT_KVARH;
                    break;
                case 0x06://�������޹�
                    fix_data[0] |= BY0_B7;//��
                    fix_data[1] |= BY1_B1;//��
                    fix_data[1] |= (BY1_B2 | BY1_B3);//������
                    fix_data[6] |= CON_UNIT_KVARH;
                    break;
                case 0x07://�������޹�
                    fix_data[0] |= BY0_B7;//��
                    fix_data[1] |= BY1_B1;//��
                    fix_data[1] |= (BY1_B2 | BY1_B3 | BY1_B4);//������
                    fix_data[6] |= CON_UNIT_KVARH;
                    break;
                case 0x08://�������޹�
                    fix_data[0] |= BY0_B7;//��
                    fix_data[1] |= BY1_B1;//��
                    fix_data[1] |= (BY1_B4 | BY1_B5);//������
                    fix_data[6] |= CON_UNIT_KVARH;
                    break;
                case 0x09://��������
                    fix_data[0] |= (BY0_B5 | BY0_B6);//����
                    fix_data[6] |= CON_UNIT_KVAH;
                    break;
                case 0x0A://��������
                    fix_data[0] |= (BY0_B4 | BY0_B6);//����
                    fix_data[6] |= CON_UNIT_KVAH;
                    break;
            }
        }
        else
        {
            dix = indi[3];
            if(dix >= 0x80)//��ABC����
            {
                dix -= 0x80;
                //��ABC
                set_totabc_char((U8)(dix/0x14), fix_data);
                switch(dix%0x14)
                {
                    case 0x00://�����ܵ���
                    case 0x05://ͭ���й��ܵ��ܲ�����
                    case 0x06://�����й��ܵ��ܲ�����
                        fix_data[1] |= (BY1_B0 | BY1_B1);//�й�
                        fix_data[6] |= CON_UNIT_KWH;
                        break;
                    case 0x01://�����й������ܵ���
                    case 0x03://�����й�г���ܵ���
                        fix_data[0] |= (BY0_B5 | BY0_B6);//����
                        fix_data[1] |= (BY1_B0 | BY1_B1);//�й�
                        fix_data[6] |= CON_UNIT_KWH;
                        break;
                    case 0x02://�����й������ܵ���
                    case 0x04://�����й�г���ܵ���
                        fix_data[0] |= (BY0_B4 | BY0_B6);//����
                        fix_data[1] |= (BY1_B0 | BY1_B1);//�й�
                        fix_data[6] |= CON_UNIT_KWH;
                        break;
                }
            }
        }
    }
//�������õ��ַ����
    else
    {
        fix_data[3] |= BY3_B3;//��

        //С����
        if(indi[0] == 0)//С�����������С����XX.XXXX
        {
            fix_data[5] |= BY5_B2;
        }
        else if(indi[0] == 1)//С�����ʱ�䣺ʱ:��
        {
            fix_data[6] |= BY6_B7;
            fix_data[5] |= BY5_B4;
            fix_data[4] |= (BY4_B4 | BY4_B5);//ʱ��
        }
        else//С�����ʱ�䣺��.��.��
        {
            fix_data[5] |= (BY5_B2 | BY5_B4);
            fix_data[4] |= (BY4_B4 | BY4_B5);//ʱ��
        }
        if(indi[3] <= 0x80)
        {
            if(indi[3] <= 0x0A)//����
            {
                if(indi[2] > RATE_NUM)
                {
                    return;
                }
                set_rate_char(indi[2], fix_data);//�ܼ��ƽ��
                dix = indi[3];
            }
            else//ABC
            {
                dix = indi[3] - 0x15;
                if(dix != 0)
                {
                    set_totabc_char((U8)(dix/0x14), fix_data);
                }
                dix %= 0x14;
            }
            switch(dix)//��λ+����
            {
                case 0x00://����й�
                    fix_data[0] |= BY0_B3;//���
                    fix_data[1] |= (BY1_B0 | BY1_B1);//�й�
                    if(indi[0] == 0)
                    {
                        fix_data[6] |= CON_UNIT_KW;
                    }
                    break;
                case 0x01://�����й�
                    fix_data[0] |= (BY0_B5 | BY0_B6);//����
                    fix_data[1] |= (BY1_B0 | BY1_B1);//�й�
                    if(indi[0] == 0)
                    {
                        fix_data[6] |= CON_UNIT_KWH;
                    }
                    break;
                case 0x02://�����й�
                    fix_data[0] |= (BY0_B4 | BY0_B6);//����
                    fix_data[1] |= (BY1_B0 | BY1_B1);//�й�
                    if(indi[0] == 0)
                    {
                        fix_data[6] |= CON_UNIT_KWH;
                    }
                    break;
                case 0x03://����޹�1
                case 0x04://����޹�2
                    fix_data[0] |= BY0_B3;//���
                    fix_data[0] |= BY0_B7;//��
                    fix_data[1] |= BY1_B1;//��
                    if(indi[0] == 0)
                    {
                        fix_data[6] |= CON_UNIT_KVAR;
                    }
                    break;
                case 0x05://һ�����޹�
                    fix_data[0] |= BY0_B7;//��
                    fix_data[1] |= BY1_B1;//��
                    fix_data[1] |= BY1_B2;//һ����
                    if(indi[0] == 0)
                    {
                        fix_data[6] |= CON_UNIT_KVAR;
                    }
                    break;
                case 0x06://�������޹�
                    fix_data[0] |= BY0_B7;//��
                    fix_data[1] |= BY1_B1;//��
                    fix_data[1] |= (BY1_B2 | BY1_B3);//������
                    if(indi[0] == 0)
                    {
                        fix_data[6] |= CON_UNIT_KVAR;
                    }
                    break;
                case 0x07://�������޹�
                    fix_data[0] |= BY0_B7;//��
                    fix_data[1] |= BY1_B1;//��
                    fix_data[1] |= (BY1_B2 | BY1_B3 | BY1_B4);//������
                    if(indi[0] == 0)
                    {
                        fix_data[6] |= CON_UNIT_KVAR;
                    }
                    break;
                case 0x08://�������޹�
                    fix_data[0] |= BY0_B7;//��
                    fix_data[1] |= BY1_B1;//��
                    fix_data[1] |= (BY1_B4 | BY1_B5);//������
                    if(indi[0] == 0)
                    {
                        fix_data[6] |= CON_UNIT_KVAR;
                    }
                    break;
                case 0x09://��������
                    fix_data[0] |= (BY0_B5 | BY0_B6);//����
                    if(indi[0] == 0)
                    {
                        fix_data[6] |= CON_UNIT_KVA;
                    }
                    break;
                case 0x0A://��������
                    fix_data[0] |= (BY0_B4 | BY0_B6);//����
                    if(indi[0] == 0)
                    {
                        fix_data[6] |= CON_UNIT_KVA;
                    }
                    break;
            }
        }
    }
}
/******************************************************************************
** ��������: void set_event_char(U8 *indi, U8 *fix_data, U8 *segbuf)
** ��������:
** ��ڲ���:
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void set_event_char (U8 *indi, U8 *fix_data, U8 *segbuf)
{
//  U8 dix;

    if(indi[3] <= 0x04)///��ѹ���ABC����¼�/ʧѹ��Ƿѹ����ѹ������
    {
        if(indi[1] == 0)//������ʱ��//ABC
        {
            set_totabc_char((U8)((indi[0]/2)+1), fix_data);
        }
        else
        {
            set_totabc_char(indi[2], fix_data);//ABC
            fix_data[0] |= BY0_B1;//��
            fill_data_last8_segbuf(indi[1], segbuf);//n
        }
    }
    else if(indi[3] <= 0x0A)//û��ABC����¼�
    {
        fix_data[0] |= BY0_B1;//��
        fill_data_last8_segbuf(indi[1], segbuf);//n
    }
    else if(indi[3] <= 0x0F)//�������ABC����¼�
    {
        if(indi[1] == 0)//������ʱ��//ABC
        {
            set_totabc_char((U8)((indi[0]/2)+1), fix_data);
        }
        else
        {
            set_totabc_char(indi[2], fix_data);//ABC
            fix_data[0] |= BY0_B1;//��
            fill_data_last8_segbuf(indi[1], segbuf);//n
        }
    }
    else if(indi[3] == 0x10)//��ѹ�ϸ���di0=0:i��ǰ di0=1:�ϣ���
    {
        if(indi[1] == 0)
        {
            fix_data[0] |= (BY0_B0 |BY0_B2);//��ǰ��
        }
        else
        {
            fix_data[0] |= (BY0_B1 |BY0_B2);//����
            fill_data_last8_segbuf(indi[1], segbuf);//n
        }
        if(indi[2] != 0)
        {
            set_totabc_char(indi[2], fix_data);//ABC
        }
    }
    else
    {
        if(indi[1] != 0)
        {
            fix_data[0] |= BY0_B1;//��
            fill_data_last8_segbuf(indi[1], segbuf);//n
        }
    }
}
/******************************************************************************
** ��������: void set_gwevent_char(U8 *indi, U8 *fix_data, U8 *segbuf)
** ��������: ������չ���¼����ݱ�ʶDI3=0x10~0x1F
** ��ڲ���:
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void set_gwevent_char (U8 *indi, U8 *fix_data, U8 *segbuf)
{
//  U8 dix;

    if(indi[4] <= 0x13)///��ѹ���ABC����¼�//ʧѹ Ƿѹ ��ѹ ����
    {
        if(indi[4] == 0x10)//ʧѹ�е�����
        {
            if(indi[3] == 0)
            {
                fix_data[0] |= BY0_B0;//��ǰ
            }
            else if(indi[2] == 0)
            {
                fix_data[0] |= BY0_B0;//��ǰ
                set_totabc_char(indi[3], fix_data);//ABC
            }
            else if(indi[1] != 0)
            {
                set_totabc_char(indi[3], fix_data);//ABC
                fix_data[0] |= BY0_B1;//��
                fill_data_last8_segbuf(indi[1], segbuf);//n
            }
        }
        else
        {
            if(indi[2] == 0)
            {
                fix_data[0] |= BY0_B0;//��ǰ
                set_totabc_char(indi[3], fix_data);//ABC
            }
            else if(indi[1] != 0)
            {
                set_totabc_char(indi[3], fix_data);//ABC
                fix_data[0] |= BY0_B1;//��
                fill_data_last8_segbuf(indi[1], segbuf);//n
            }
        }
    }
    else if(indi[4] <= 0x17)//������ ��ƽ��
    {
        if(indi[2] == 0)
        {
            fix_data[0] |= BY0_B0;//��ǰ
        }
        else if(indi[1] != 0)
        {
            fix_data[0] |= BY0_B1;//��
            fill_data_last8_segbuf(indi[1], segbuf);//n
        }
    }
    else if(indi[3] <= 0x1C)//�������ABC����¼�
    {
        if(indi[2] == 0)
        {
            fix_data[0] |= BY0_B0;//��ǰ
            set_totabc_char(indi[3], fix_data);//ABC
        }
        else if(indi[1] != 0)
        {
            set_totabc_char(indi[3], fix_data);//ABC
            fix_data[0] |= BY0_B1;//��
            fill_data_last8_segbuf(indi[1], segbuf);//n
        }
    }
    else if(indi[3] == 0x1f)//�ܹ�������������
    {
        if(indi[2] == 0)
        {
            fix_data[0] |= BY0_B0;//��ǰ
        }
        else if(indi[1] != 0)
        {
            fix_data[0] |= BY0_B1;//��
            fill_data_last8_segbuf(indi[1], segbuf);//n
        }
    }

}
/******************************************************************************
** ��������: void set_time_char(U8 *indi, U8 *fix_data, U8 *segbuf)
** ��������:
** ��ڲ���:
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void set_time_char (U8 *indi, U8 *fix_data, U8 *segbuf)
{
    U32 di;

    di = indi[1] + (indi[2]<<8) + (indi[3]<<16) + (indi[4]<<24);
    if(di == 0x04000101)//���� YY.MM.DD.WW
    {
        fix_data[5] |= (BY5_B4 | BY5_B2 | BY5_B0);
    }
    else if(di == 0x04000102)//ʱ�� hh:mm:ss
    {
        fix_data[5] |= (BY5_B4 | BY5_B2);
        fix_data[6] |= (BY6_B6 | BY6_B7);
    }
}
/******************************************************************************
** ��������: S8 get_dis_info (U8 *di_buf, U8 *fix_data, DIS_CTRL_TCB *pdis_tcb, U8 *segbuf)
** ��������:
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
S8 get_dis_info (U8 *di_buf, U8 *fix_data, DIS_CTRL_TCB *pdis_tcb, U8 *segbuf)
{
//  U8  lsn;
    U8  dix;
    U16 sn;
    U32 di_temp;
    U32 di;
#ifndef DATA_SOURCE_NOT_LOCAL
    U32 offset;
    U32 addr;
    U32 err;
    ADDR_OFFSET_TCB offset_tcb;
#endif//DATA_SOURCE_NOT_LOCAL

    di = di_buf[1] + (di_buf[2]<<8) + (di_buf[3]<<16) + (di_buf[4]<<24);

#if CODE_VER_TYPE==WLS_CUR_PRJ			//������߲ɼ���
    get_dis_temp_data(di, di_buf[0]);//azh ���߲ɼ����汾
#endif

    switch(di_buf[4])
    {
        case 0x00:
        {
            if(di_buf[3] <= 0x0A)
            {
                di_temp = di&0xffff0000;
            }
            else
            {
                 di_temp = di&0xffffff00;
            }
            //����ӱ��������
            sn = get_dis_sn_bydi(di_temp, di_buf[0], disctrbydi_di300table, DI3_00_TABLE_MAXNUM);
            if(sn == 0xffff)
            {
                return __FALSE;
            }
            *pdis_tcb = disctrbydi_di300table[sn];
            if(di_buf[3] <= 0x0A)
            {
                if(di_buf[2] > RATE_NUM)
                {
                    return __FALSE;
                }
                pdis_tcb->addr +=  ((pdis_tcb->datalen&0x0f)*di_buf[2]);//����λ��С�����ݳ���
            }
#ifdef DATA_SOURCE_NOT_LOCAL

	#if CODE_VER_TYPE==WLS_CUR_PRJ			//������߲ɼ���
	//azh 130928 ���߲ɼ����汾 ��ǰҲ��ʾ��ʱ��ȫ�ֱ��������
			pdis_tcb->addr = (U32)gTmpArray + 5;//ת��ȫ����ʱRAM��ַL+DI+DATA
            if((memcmp(gTmpArray+1,di_buf+1,4) != 0) || (gTmpArray[0] < ((pdis_tcb->datalen&0x0f)+4)))//�ȶ����ݱ�ʶ�Ƿ���ͬ
            {
            	memset(gTmpArray+5, 0xee, (pdis_tcb->datalen&0x0f));
            }
	#else
            if(di_buf[1] ==0 )////���ǵ�ǰ ���ϼ�����
            {
                pdis_tcb->addr = pdis_tcb->addr - ENG_STORE_START_ADDR + (U32)gEngArray;//ת��ȫ��RAM��ַ
            }
#endif
#endif
            if(di_buf[1] !=0 )////���ǵ�ǰ ���ϼ�����
            {
#ifdef DATA_SOURCE_NOT_LOCAL
                pdis_tcb->addr = (U32)gTmpArray + 5;//ת��ȫ����ʱRAM��ַL+DI+DATA
                if((memcmp(gTmpArray+1,di_buf+1,4) != 0) || (gTmpArray[0] < ((pdis_tcb->datalen&0x0f)+4)))//�ȶ����ݱ�ʶ�Ƿ���ͬ
                {
                    memset(gTmpArray+5, 0xee, (pdis_tcb->datalen&0x0f));
                }
#else
                offset = pdis_tcb->addr - ENG_COMPOSP_00000000_00_ADDR;//�����ƫ�Ƶ�ַ
                offset_tcb.head_addr = ENG_LASTXMONTH_00000000_00_HEAD_ADDR;
                offset_tcb.offset_addr = ENG_LASTXMONTH_00000000_00_OFFSET_ADDR;
                offset_tcb.block_len = ENG_LASTXMONTH_00000000_00_OFFSET_BLOCKLEN;
                offset_tcb.offset_len = ENG_LASTXMONTH_00000000_00_OFFSET_LEN;
                offset_tcb.max_num = ENG_LASTXMONTH_00000000_00_OFFSET_MAXNUM;

                err = get_addr_offset((U8)(di_buf[1]-1), &offset_tcb, &addr);
                if(err != NO_ERROR)
                {
                    return  __FALSE;
                }
                pdis_tcb->addr = addr + offset;
#endif
            }
            set_eng_dmd_char(di_buf, fix_data, segbuf);//����й��ɵĺ�����ʾ����
            break;
        }
        case 0x01:
        {
            if(di_buf[3] <= 0x0A)
            {
                di_temp = di&0xffff0000;
            }
            else
            {
                 di_temp = di&0xffffff00;
            }
            //����ӱ��������
            sn = get_dis_sn_bydi(di_temp, di_buf[0], disctrbydi_di301table, DI3_01_TABLE_MAXNUM);
            if(sn == 0xffff)
            {
                return __FALSE;
            }
            *pdis_tcb = disctrbydi_di301table[sn];
            if(di_buf[3] <= 0x0A)
            {
                if(di_buf[2] > RATE_NUM)
                {
                    return __FALSE;
                }
                pdis_tcb->addr +=  ((pdis_tcb->datalen&0x0f)*di_buf[2]);//����λ��С�����ݳ���
            }
#ifdef DATA_SOURCE_NOT_LOCAL
            if(di_buf[1] ==0 )////���ǵ�ǰ ���ϼ�����
            {
                pdis_tcb->addr = pdis_tcb->addr - DMD_POSP_01010000_00_ADDR + (U32)gDmdArray;//ת��ȫ��RAM��ַ
            }
#endif
            if(di_buf[1] !=0 )////���ǵ�ǰ ���ϼ�����
            {
#ifdef DATA_SOURCE_NOT_LOCAL
                pdis_tcb->addr = (U32)gTmpArray + 5;//ת��ȫ����ʱRAM��ַL+DI+DATA
                if((memcmp(gTmpArray+1,di_buf+1,4) != 0) || (gTmpArray[0] < ((pdis_tcb->datalen&0x0f)+4)))//�ȶ����ݱ�ʶ�Ƿ���ͬ
                {
                    memset(gTmpArray+5, 0xee, (pdis_tcb->datalen&0x0f));
                }
                if(di_buf[0] != 0)//���ǲ�ֵĵ�һ��
                {
                    if(sn >= di_buf[0])
                    {
                        pdis_tcb->addr += (disctrbydi_di301table[sn].addr - disctrbydi_di301table[sn - di_buf[0]].addr);
                    }
                }
#else
                offset = pdis_tcb->addr - DMD_POSP_01010000_00_ADDR;//�����ƫ�Ƶ�ַ
                offset_tcb.head_addr = DMD_LASTXMONTH_01010001_00_HEAD_ADDR;
                offset_tcb.offset_addr = DMD_LASTXMONTH_01010001_00_OFFSET_ADDR;
                offset_tcb.block_len = DMD_LASTXMONTH_01010001_00_OFFSET_BLOCKLEN;
                offset_tcb.offset_len = DMD_LASTXMONTH_01010001_00_OFFSET_LEN;
                offset_tcb.max_num = DMD_LASTXMONTH_01010001_00_OFFSET_MAXNUM;

                err = get_addr_offset((U8)(di_buf[1]-1), &offset_tcb, &addr);
                if(err != NO_ERROR)
                {
                    return  __FALSE;
                }
                pdis_tcb->addr = addr + offset;
#endif
            }
            set_eng_dmd_char (di_buf, fix_data, segbuf);//����й��ɵĺ�����ʾ����
            break;
        }
        case 0x02:
        {
            //˲ʱ������Ҫ���� ��ȫ�г�����
            if(di_buf[3] <= 0x0B)
            {
                set_totabc_char(di_buf[2], fix_data);//��ABC
            }
            sn = get_dis_sn_bydi(di, di_buf[0], disctrbydi_di302table, DI3_02_TABLE_MAXNUM);
            if(sn == 0xffff)
            {
                return __FALSE;
            }
            *pdis_tcb = disctrbydi_di302table[sn];
#ifdef DATA_SOURCE_NOT_LOCAL
            pdis_tcb->addr = (U32)gTmpArray + 5;//ת��ȫ����ʱRAM��ַL+DI+DATA
            if((memcmp(gTmpArray+1,di_buf+1,4) != 0) || (gTmpArray[0] < ((pdis_tcb->datalen&0x0f)+4)))//�ȶ����ݱ�ʶ�Ƿ���ͬ
            {
                memset(gTmpArray+5, 0xee, (pdis_tcb->datalen&0x0f));
            }
            if(di_buf[0] != 0)//���ǲ�ֵĵ�һ��
            {
                if(sn >= di_buf[0])
                {
                    pdis_tcb->addr += (disctrbydi_di302table[sn].addr - disctrbydi_di302table[sn - di_buf[0]].addr);
                }
            }
#endif
            break;
        }
        case 0x03:
        {
            if(di_buf[3] == 0x10)//ֻ�кϸ����¼��Ǵ�di0=0�������Ҳֻ��
            {
                di_temp = di&0xffffff00;
            }
            else
            {
                di_temp = (di&0xffffff00);
                if(di_buf[1] != 0)//�����¼������ҵ�di0=0,di0=1
                {
                    di_temp += 1;
                }
            }
            //�����ӱ��������
            sn = get_dis_sn_bydi(di_temp, di_buf[0], disctrbydi_di303table, DI3_03_TABLE_MAXNUM);
            if(sn == 0xffff)
            {
                return __FALSE;
            }
            *pdis_tcb = disctrbydi_di303table[sn];
#ifdef DATA_SOURCE_NOT_LOCAL
            pdis_tcb->addr = (U32)gTmpArray + 5;//ת��ȫ����ʱRAM��ַL+DI+DATA
            if((memcmp(gTmpArray+1,di_buf+1,4) != 0) || (gTmpArray[0] < ((pdis_tcb->datalen&0x0f)+4)))//�ȶ����ݱ�ʶ�Ƿ���ͬ
            {
                memset(gTmpArray+5, 0xee, (pdis_tcb->datalen&0x0f));
            }
            if(di_buf[0] != 0)//���ǲ�ֵĵ�һ��
            {
                if(sn >= di_buf[0])
                {
                    pdis_tcb->addr += (disctrbydi_di303table[sn].addr - disctrbydi_di303table[sn - di_buf[0]].addr);
                }
            }
#else
            if((di_buf[3] == 0x10) || (di_buf[1] != 0))
            {
                if(di_buf[3] == 0x10)//ֻ�кϸ����¼��Ǵ�di0=0�������Ҳֻ��
                {
                    dix = di_buf[1];
                }
                else
                {
                    dix = di_buf[1] - 1;
                }
                if(pdis_tcb->offsettabsn != 0xff)//��ƫ�Ʊ�����
                {
                    sn = pdis_tcb->offsettabsn;
                    offset = pdis_tcb->addr - dis_exdctr_offsetbysn_table[sn].head_addr;//�����ƫ�Ƶ�ַ

                    offset_tcb.head_addr = dis_exdctr_offsetbysn_table[sn].head_addr;
                    offset_tcb.offset_addr = dis_exdctr_offsetbysn_table[sn].offset_addr;
                    offset_tcb.block_len = dis_exdctr_offsetbysn_table[sn].block_len;
                    offset_tcb.offset_len = dis_exdctr_offsetbysn_table[sn].offset_len;
                    offset_tcb.max_num = dis_exdctr_offsetbysn_table[sn].max_num;

                    err = get_addr_offset(dix, &offset_tcb, &addr);
                    if(err != NO_ERROR)
                    {
                        return  __FALSE;
                    }
                    pdis_tcb->addr = addr + offset;
                }
            }
#endif
            set_event_char(di_buf, fix_data, segbuf);
            break;
        }
        case 0x04:
        {
            dix = di_buf[0];
            if((di_buf[3] == 0x01) || (di_buf[3] == 0x02))//��һ����ʱ��ʱ�α�
            {
                di_temp = 0x00010000;//ֻ�е�һ��ʱ����1ʱ�α�
                if(di_buf[0] != 0 )
                {
                    di_temp |= 0x00000001;
                }
                di_temp |= (di&0xffffff00);
                dix = 0;
            }
            else if((di_buf[3] == 0x03) || (di_buf[3] == 0x04))//�������� �Զ�ѭ����ʾ ����ѭ����ʾ ����д�ŵ��׵�ַ
            {
                    di_temp = 1;
                    di_temp |= (di&0xffffff00);
                    dix = 0;
            }
            else
            {
                di_temp = di;
            }
            //�����ӱ��������
            sn = get_dis_sn_bydi(di_temp, dix, disctrbydi_di304table, DI3_04_TABLE_MAXNUM);
            if(sn == 0xffff)
            {
                if(di != 0x04000401)
                {
                   return __FALSE;
                }
                else
                {
                    sn = 0;
                }
            }
            *pdis_tcb = disctrbydi_di304table[sn];
#ifdef DATA_SOURCE_NOT_LOCAL
            if(di == 0x04000401)//����ȫ�ֱ�����ͨ�ŵ�ַ������ݱ�ʶ���д�
            {
                if(di_buf[0]>1)
                {
                  return __FALSE;
                }
                *pdis_tcb = disctrbydi_spec_di304table[di_buf[0]];
                pdis_tcb->addr = (U32)gTmpArray + 5;//ת��ȫ����ʱRAM��ַL+DI+DATA
                if((memcmp(gTmpArray+1,di_buf+1,4) != 0) || (gTmpArray[0] < ((pdis_tcb->datalen&0x0f)+4)))//�ȶ����ݱ�ʶ�Ƿ���ͬ
                {
                    memset(gTmpArray+5, 0xee, (pdis_tcb->datalen&0x0f));
                }
                if(di_buf[0] != 0)//�Ǹ�2�ֽ�
                {
                    pdis_tcb->addr += 4;
                }
            }
            else
            {
                pdis_tcb->addr = (U32)gTmpArray + 5;//ת��ȫ����ʱRAM��ַL+DI+DATA
                if((memcmp(gTmpArray+1,di_buf+1,4) != 0) || (gTmpArray[0] < ((pdis_tcb->datalen&0x0f)+4)))//�ȶ����ݱ�ʶ�Ƿ���ͬ
                {
                    memset(gTmpArray+5, 0xee, (pdis_tcb->datalen&0x0f));
                }
                if(di_buf[0] != 0)//���ǲ�ֵĵ�һ��
                {
                    if(sn >= di_buf[0])
                    {
                        pdis_tcb->addr += (disctrbydi_di304table[sn].addr - disctrbydi_di304table[sn - di_buf[0]].addr);
                    }
                }
            }
#else
            if((di_buf[3] == 0x01) || (di_buf[3] == 0x02))//��һ����ʱ��ʱ�α�
            {
                addr = pdis_tcb->addr + (di_buf[3]-1) * PAR_T1_TOT_LEN;
                offset = di_buf[0] * (pdis_tcb->datalen&0x0f);
                pdis_tcb->addr = addr + offset;
                fill_data_last8_segbuf(di_buf[0], segbuf);//n
            }
            else if((di_buf[3] == 0x03) || (di_buf[3] == 0x04))//�������� �Զ�ѭ����ʾ ����ѭ����ʾ
            {
                offset = di_buf[0] * (pdis_tcb->datalen&0x0f);
                pdis_tcb->addr += offset;
                fill_data_last8_segbuf(di_buf[0], segbuf);//n
            }
 #endif
            set_time_char(di_buf, fix_data, segbuf);
            break;
        }
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x17:
        case 0x18:
        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x1C:
//        case 0x1D://��բ
//        case 0x1E://��բ
        case 0x1F:
        {
            di_temp = di;
            if(di_buf[2] >= 0x01)//ֻ�кϸ����¼��Ǵ�di0=0�������Ҳֻ��
            {
                if((di_buf[4] != 0x10) || ((di_buf[4] == 0x10) && (di_buf[3] != 0)))//ʧѹ�е�����
                {
                    di_temp = (di & 0xffffff00) + 1;
                }
            }
            //�����ӱ��������
            sn = get_dis_sn_bydi(di_temp, di_buf[0], disctrbydi_di310table, DI3_10_TABLE_MAXNUM);
            if(sn == 0xffff)
            {
                return __FALSE;
            }
            *pdis_tcb = disctrbydi_di310table[sn];
#ifdef DATA_SOURCE_NOT_LOCAL
            pdis_tcb->addr = (U32)gTmpArray + 5;//ת��ȫ����ʱRAM��ַL+DI+DATA
            if((memcmp(gTmpArray+1,di_buf+1,4) != 0) || (gTmpArray[0] < ((pdis_tcb->datalen&0x0f)+4)))//�ȶ����ݱ�ʶ�Ƿ���ͬ
            {
                memset(gTmpArray+5, 0xee, (pdis_tcb->datalen&0x0f));
            }
            if(di_buf[0] != 0)//���ǲ�ֵĵ�һ��
            {
                if(sn >= di_buf[0])
                {
                    pdis_tcb->addr += (disctrbydi_di303table[sn].addr - disctrbydi_di303table[sn - di_buf[0]].addr);
                }
            }
#else
            if((di_buf[2] >= 0x01) && (di_buf[1] != 0))//��1~10����
            {
                dix = di_buf[1] - 1;

                if(pdis_tcb->offsettabsn != 0xff)//��ƫ�Ʊ�����
                {
                    sn = pdis_tcb->offsettabsn;
                    offset = pdis_tcb->addr - dis_exdctr_offsetbysn_table[sn].head_addr;//�����ƫ�Ƶ�ַ

                    offset_tcb.head_addr = dis_exdctr_offsetbysn_table[sn].head_addr;
                    offset_tcb.offset_addr = dis_exdctr_offsetbysn_table[sn].offset_addr;
                    offset_tcb.block_len = dis_exdctr_offsetbysn_table[sn].block_len;
                    offset_tcb.offset_len = dis_exdctr_offsetbysn_table[sn].offset_len;
                    offset_tcb.max_num = dis_exdctr_offsetbysn_table[sn].max_num;

                    err = get_addr_offset(dix, &offset_tcb, &addr);
                    if(err != NO_ERROR)
                    {
                        return  __FALSE;
                    }
                    pdis_tcb->addr = addr + offset;
                }
            }
#endif
            set_gwevent_char(di_buf, fix_data, segbuf);
            break;
        }
        default:
        {
            return __FALSE;
        }
    }
    return __TRUE;
}


/******************************************************************************
** ��������: S8 fill_dis_data(void)
** ��������: ��д�̶���ʾ������
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
S8 fill_dis_data (void)
{
    S8  err;
    U8  i;
    U8  pos;
    U8  disdi[5];
    U8  data[8];
    U16 sn;
    U32 di;
    U32 dis_code;
    DIS_CTRL_TCB dis_tcb;

    if(Dis_Ctrl.cur_mode == DIS_MODE_ALLON)//�ϵ�ȫ���Լ�
    {
        lcd_on_all_dis();
        return  __TRUE;
    }

//��ʼ��ȫ����ʾ������
    for(i=0; i<LCD_BUF_LENTH; i++)
    {
        lcd_seg_buf[i] = 0;
    }

    pos = 0;
    lcd_seg_buf[LCD_BUF_LENTH-1] = 0;//��ʾ�������������� �Ϳ���0.5sˢ����
    lcd_seg_buf[pos++] = ICSET1;
//  lcd_seg_buf[pos++] = APCTL;
    lcd_seg_buf[pos++] = DISCTL22;
//  lcd_seg_buf[pos++] = EVRSET;
    lcd_seg_buf[pos++] = ADSET;

    pos = CON_DIS_DATA_OFFSET;//3
//--------------------------------------------------
    memset(data, 0, 8);//�й����ֽ���ʾλ��־����

    fill_gprs_signal_strenth_segbuf(lcd_seg_buf+pos);//gprs �ź�ǿ��

    if(Dis_Ctrl.cur_mode == DIS_MODE_ERRON)//���ش���
    {
        di = get_meter_serious_err_id();

        fill_dis_err_page((U8*)&di, lcd_seg_buf+pos);
        //��д��ʾ���Ա�����
        dis_code = di;
        fill_dis_fix_l8_data(dis_code, lcd_seg_buf+pos);
        //���½�88��ʾС���
        dis_code = 0;
        fill_data_d88_segbuf(dis_code, lcd_seg_buf+pos);
    }
    else if(Dis_Ctrl.cur_mode == DIS_MODE_MATCH)//������ ���״̬
    {
        Dis_Ctrl.had_write = 1;
        dis_code = Dis_Search_Ctrl.totnum;
        if(Dis_Search_Ctrl.state == CON_SEARCH_RUN_STATE)
        {
            fill_special_b8segbuf(1, lcd_seg_buf+pos);//��ʾ�����������SEArch--
        }
        else if(Dis_Search_Ctrl.state == CON_SEARCH_DISPLY_STATE)
        {
            fill_search_addr_segbuf(lcd_seg_buf+pos);//��дҪ��ʾ�ı��ַ
        }
        else if(Dis_Search_Ctrl.state == CON_SEARCH_FINISH_STATE)
        {
            fill_search_set_state_segbuf(lcd_seg_buf+pos);//��ʾSET-Good or SET-Err
        }
        fill_data_d88_segbuf(dis_code, lcd_seg_buf+pos);//�������ı�����

        //��д����̶��ַ�
        fill_dis_special_fix_char(lcd_seg_buf+pos);//����

        //������˸��־ �������ʾ����
        fill_dis_flash_flag(lcd_seg_buf+pos);
        //������ʾ������Ϊ32�ֽ�
        pos += 32;
        lcd_write_data(BU97950_HEAD_ADDR, lcd_seg_buf, pos);
    }
    else
    {
//���¼������ʾ���ݵ�ַ ������ЩҪ��ʾ���й��ɵ��ַ����ڱ�־���� �����ȫ����ʾ����
        memset(&dis_tcb, 0, sizeof(DIS_CTRL_TCB));

        if(Dis_Ctrl.cur_mode == DIS_MODE_AUTO)
        {
            disdi[0] = Dis_Ctrl.exd_lsn[DIS_MODE_AUTO];
        }
        else//���Ի򳤰�����
        {
            disdi[0] = Dis_Ctrl.exd_lsn[DIS_MODE_SKEY];
        }
        di = Dis_Ctrl.di;

        for(i=0;i<4;i++)
        {
            disdi[i+1] = (di>>(8*i));
        }

        err = get_dis_info(&disdi[0], &data[0], &dis_tcb, lcd_seg_buf+pos);
        if(err == __FALSE)
        {
            //return __FALSE;
            goto id_dis_err_handle;
        }
    //��ʼ��д��ʾ������
        err = fill_dis_fix_b8_data(di, &dis_tcb, lcd_seg_buf+pos);
        if(err == __FALSE)
        {
            //return  __FALSE;
            goto id_dis_err_handle;
        }
    //��д��ʾ���Ա�����
    /*
        if(Dis_Ctrl.cur_mode == DIS_MODE_AUTO)
        {
            dis_code = di;
        }
        else//���Ի򳤰�����
        {
            dis_code = Dis_Ctrl.dis_bsn[DIS_MODE_SKEY]+1;
            dis_code = hex_to_bcd_in8 (dis_code);
        }
    */
        dis_code = di;
        fill_dis_fix_l8_data(dis_code, lcd_seg_buf+pos);

    //���½�88��ʾС���
#if CODE_VER_TYPE==WLS_CUR_PRJ			//������߲ɼ���
//azh 130930��ʱ������������ʾ
        dis_code = get_dis_mp();
#else
        dis_code = HEX_TO_BCD(disdi[0], 1);
#endif
        fill_data_d88_segbuf(dis_code, lcd_seg_buf+pos);

    //��д��Ҫ��ָ�й��ɵ���Щ�ַ� ��û�ж����ڱ�����
        fill_dis_fix_char(&data[0], lcd_seg_buf+pos);

    //��д�̶��ַ�
        sn = dis_tcb.fixchartype;
        memcpy(data, fixchar_table[sn],8);
        fill_dis_fix_char(&data[0], lcd_seg_buf+pos);
    }

//����ʱ �������û��Ҫ��ʾ
    if(CON_CPU_NORM_MODE == cpu_run_mode())//�е�״̬����ϵͳ����
    {
//��д����̶��ַ�
        fill_dis_special_fix_char(lcd_seg_buf+pos);//����

//������˸��־ �������ʾ����
        fill_dis_flash_flag(lcd_seg_buf+pos);
//������ʾ������Ϊ32�ֽ�
    }

    pos += 32;
    lcd_write_data(BU97950_HEAD_ADDR, lcd_seg_buf, pos);

    return __TRUE;

//������ û�����ݱ�ʶ������������
id_dis_err_handle:
    data[0] = 0xee;
    data[1] = 0xee;
    fill_data_b8segbuf(data, 4, lcd_seg_buf+pos);

//��д��ʾ���Ա�����
/*
    if(Dis_Ctrl.cur_mode == DIS_MODE_AUTO)
    {
        dis_code = di;
    }
    else//���Ի򳤰�����
    {
        dis_code = Dis_Ctrl.dis_bsn[DIS_MODE_SKEY]+1;
        dis_code = hex_to_bcd_in8 (dis_code);
    }
*/
    dis_code = di;
    fill_dis_fix_l8_data(dis_code, lcd_seg_buf+pos);

//���½�88��ʾС���
    dis_code = HEX_TO_BCD(disdi[0], 1);
    fill_data_d88_segbuf(dis_code, lcd_seg_buf+pos);
//��д����̶��ַ�
    fill_dis_special_fix_char(lcd_seg_buf+pos);//����

//������˸��־ �������ʾ����
    fill_dis_flash_flag(lcd_seg_buf+pos);
//������ʾ������Ϊ32�ֽ�
    pos += 32;
    lcd_write_data(BU97950_HEAD_ADDR, lcd_seg_buf, pos);
    return __TRUE;
}
/******************************************************************************
** ��������: U8 judge_data_sign(U8 *buf, DIS_CTRL_TCB *pdistable)
** ��������: �������ݱ�ʶ����չ����� �жϴ��������Ƿ����з����� �Ǹ��� �򷵻ظ��ŵ�λ��
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
U8 judge_data_sign(U8 *buf, DIS_CTRL_TCB *pdistable)
{
    U8 r_valu = 0xff;
    U8 dix;
    U8 len;
    U8 data;

    dix = (pdistable->di>>24);//di3
    len = pdistable->datalen&0x0f;
    if((len > 4) || (len == 0))
    {
        return r_valu;
    }
    data = buf[len - 1];
    if((data == 0xee) || ((data & 0x80) == 0))//����Ч���� �����з�����������
    {
        return r_valu;
    }

    switch(dix)
    {
        case 00:
            dix = (pdistable->di>>16);//di2
            if((dix == 0) || (dix == 3) || (dix == 4) || (dix == 0x17) || (dix == 0x18)//��ϵ����з�����
                 || (dix == 0x2b) || (dix == 0x2c) || (dix == 0x3f) || (dix == 0x40))
            {
                r_valu = 8;//-xxxxxx.xx
            }
            break;
        case 01:
            dix = (pdistable->di>>16);//di2
            if((dix == 3) || (dix == 4) || (dix == 0x17) || (dix == 0x18)//��ϵ����з�����
                 || (dix == 0x2b) || (dix == 0x2c) || (dix == 0x3f) || (dix == 0x40))
            {
                if(pdistable->litsn == 0)//ֻ������������
                {
                    r_valu = 6;//-xx.xxxx
                }
            }
            break;
        case 02:
            dix = (pdistable->di>>16);//di2
            if((dix == 2) ||(dix == 3) ||(dix == 4))//���� ˲ʱ���޹���
            {
                r_valu = 6;//-xxx.xxx -xx.xxxx
            }
            else if(dix == 6)//��������
            {
                r_valu = 4;//-x.xxx
            }
            else if(dix == 0x80)//һ�����й���ƽ������ ��ǰ�й����� ��ǰ�޹�����
            {
                dix = (pdistable->di>>8);//di1
                if((dix == 3) ||(dix == 4) ||(dix == 5))
                {
                    r_valu = 6;//-xx.xxxx
                }
            }
            break;
    }
    if(r_valu <= 8)//�������λ
    {
        buf[len - 1] &= 0x7f;
    }
    return r_valu;
}
/******************************************************************************
** ��������: S8 fill_dis_fix_b8_data(U32 di, DIS_CTRL_TCB *pdistable, U8 *segbuf)
** ��������: ��Ҫ��ʾ�����ݷ�����ʾ������8�����������
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
S8 fill_dis_fix_b8_data (U32 di, DIS_CTRL_TCB *pdistable, U8 *segbuf)
{
    U8 mem_type;
    U8 i;
    U8 *buf;
    U8 len;
    U32 data;
    U32 addr;

    data = 0;
    len = pdistable->datalen&0x0f;

    if(len > 4)
    {
        return  __FALSE;
    }

    //B1~B0: �洢ģʽ ��ַ�ڴ���ʱ�Ѿ������
    switch((pdistable->specialctrl&0x03))
    {
        case 0:     //EEPROM
        {
            addr = pdistable->addr;
            break;
        }
        case 1:     //RAM
        {
            addr = pdistable->addr;
            break;
        }
        case 2:     //EEPROM��ָ��ƫ�Ƶ�
            addr = pdistable->addr;
            break;
        default:
            return  __FALSE;
    }
    buf = alloc_comm_mem(MEM_64_TYPE, &mem_type, GET_MEM_ERR7);
    if(buf  != NULL)
    {
        file_read(addr, buf, len);
    }
    else
    {
//          memcpy(gusCurMemErrFunName, "fill_dis", 8);
        setmemgetnullid(GET_MEM_ERR7, gusCurMemErrFunName);
        return  __FALSE;
    }

    //B3~B2:���ݸ�ʽ
    //B4=0: ֱ����ʾ��=1:����Ҫת����ʽ��ʾ

    if((pdistable->specialctrl&0x10) != 0)
    {
        switch(((pdistable->specialctrl>>2)&0x03))
        {
            case 0: //���ֽ�תBCD
            {
                data = byte_merge(buf, len);//�ϲ�����ֽ����Ϊһ��U32��
                //B7=0: �޷�������=1:�з�����
                if((pdistable->specialctrl&0x80) != 0)//�з�����
                {
                    //ת�ɾ���ֵ����ס���ű�־������ʾ
                    //todo
                }
                bytes_to_bcd(data, buf, 8);//����8λBCD��
                len = 8;
                break;
            }
            case 1: //���ֽ�תBCD
            {
                convert_to_bcd(buf, len);
                len >>=1;
                break;
            }
            case 2: //���ֽ�ת����8λ  Ҫƴ��4�ֽ� ���ں��ͳһ����LCD����
            {
                data = buf[0];
                for(i=0; i<8; i++)
                {
                    if((i&2) == 0)
                    {
                        buf[i/2] = 0;
                    }
                    if((data>>i)&0x01)
                    {
                        buf[i/2] += (0x01<<((i%2)*4));
                    }
                }
                len = 8;
                break;
            }
            default:
            {
                free_mem_block(buf);//Mem_64_Byte
                return __FALSE;
            }
        }
    }
    else
    {
        //pdistable->datalen>>4 //8�ĸ���
        len <<= 1;
    }
    i = judge_data_sign(buf, pdistable);
    //��buf���������д��ȫ����ʾ������
    if(i <= 8)
    {
        fill_data_sign(i, segbuf);//��д���ݵķ���λ
    }
    //���� ʱ��Ҫ���⴦��
    if((di == 0x04000101) || (di == 0x04000102))
    {
        get_dis_date_time(di, buf, &len);
    }
    fill_data_b8segbuf(buf, len, segbuf);

    free_mem_block(buf);//Mem_64_Byte

    return __TRUE;
}
/******************************************************************************
** ��������: U16 get_dis_lsn_info (U32 di, DIS_AUX_CTRL const *pdistable, U16 max_num)
** ��������:
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
U16 get_dis_lsn_info (U32 di, DIS_AUX_CTRL const *pdistable, U16 max_num)
{
    U16 i;

    for(i=0; i<max_num; i++)
    {
        if(pdistable[i].di == di)
        {
            return i;
        }
    }
    return 0xffff;
}
/******************************************************************************
** ��������: void get_dis_sn_bydi (U32 di, U8 lsn, DIS_CTRL_TCB const *pdistable, U16 max_num)
** ��������:
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
U16 get_dis_sn_bydi (U32 di, U8 lsn, DIS_CTRL_TCB const *pdistable, U16 max_num)
{
    U16 sn;
    U16 i;

    //�������ö��ַ������ѱ�
    sn = 0xffff;
    for(i=0; i<max_num; i++)
    {
        if(pdistable[i].di == di)
        {
            if(pdistable[i].litsn == lsn)
            {
                sn = i;
                break;
            }
        }
    }
    return sn;
}
/******************************************************************************
** ��������: S8 fill_dis_fix_l8_data(U32 di, U8 *segbuf)
** ��������: ��Ҫ��ʾ�����Ա������ʾ��ŷ���8��С��������
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
S8 fill_dis_fix_l8_data (U32 di, U8 *segbuf)
{
    fill_data_l8segbuf((U8* )(&di), 8, segbuf);

    return __TRUE;
}
/******************************************************************************
** ��������: void handle_page_sn(U8 mode, U8 direct)
** ��������: ��ʾģʽ�л�
** ��ڲ���:
**
**
** �� �� ֵ:
** ˵    ��: ÿ100ms����
******************************************************************************/
void handle_page_sn(U8 mode, U8 direct)
{
    if(mode >= DIS_MODE_NUM)
    {
        mode = DIS_MODE_AUTO;
    }
    if(mode == DIS_MODE_LKEY)
    {
        mode = DIS_MODE_SKEY;
    }
    switch(direct)
    {
        case DN_SN_DIRECT:
        {
            Dis_Ctrl.flash_flag |= LSN_CHG_SN_FLAG;
            if(Dis_Ctrl.lsn_maxn[mode] <= 1)
            {
                Dis_Ctrl.flash_flag |= BSN_CHG_SN_FLAG;
                Dis_Ctrl.exd_lsn[mode] = 0;
                Dis_Ctrl.dis_bsn[mode]++;
                if(Dis_Ctrl.dis_bsn[mode] >= Dis_Para.maxn[mode])
                {
                    Dis_Ctrl.dis_bsn[mode] = 0;
                    if(mode == DIS_MODE_AUTO)
                    {
                        pd_autodis_finish = 1;//�Զ�ѭ����ʾ��һ���� �ڵ���ģʽ������ Ҫ������ȵ���
                    }
                }
            }
            else
            {
                Dis_Ctrl.exd_lsn[mode]++;
                if(Dis_Ctrl.exd_lsn[mode] >= Dis_Ctrl.lsn_maxn[mode])
                {
                    Dis_Ctrl.exd_lsn[mode] = 0;
                    Dis_Ctrl.dis_bsn[mode]++;
                    Dis_Ctrl.flash_flag |= BSN_CHG_SN_FLAG;
                    if(Dis_Ctrl.dis_bsn[mode] >= Dis_Para.maxn[mode])
                    {
                        Dis_Ctrl.dis_bsn[mode] = 0;
                    }
                }
            }
            break;
        }
        case UP_SN_DIRECT:
        {
            Dis_Ctrl.flash_flag |= LSN_CHG_SN_FLAG;
            if(Dis_Ctrl.lsn_maxn[mode] <= 1)
            {
                Dis_Ctrl.flash_flag |= BSN_CHG_SN_FLAG;
                Dis_Ctrl.exd_lsn[mode] = 0;
                if(Dis_Ctrl.dis_bsn[mode] > 0)
                {
                    Dis_Ctrl.dis_bsn[mode]--;
                }
                else
                {
                    Dis_Ctrl.dis_bsn[mode] = Dis_Para.maxn[mode] - 1;
                }
            }
            else
            {
                if(Dis_Ctrl.exd_lsn[mode] > 0)
                {
                    Dis_Ctrl.exd_lsn[mode]--;
                }
                else
                {
                    Dis_Ctrl.flash_flag |= BSN_CHG_SN_FLAG;
                    if(Dis_Ctrl.dis_bsn[mode] > 0)
                    {
                        Dis_Ctrl.dis_bsn[mode]--;
                    }
                    else
                    {
                        Dis_Ctrl.dis_bsn[mode] = Dis_Para.maxn[mode] - 1;
                    }
                }
            }
        }
    }
}
/******************************************************************************
** ��������: void lcd_mode_ctrl(void)
** ��������: ��ʾģʽ�л�
** ��ڲ���:
**
**
** �� �� ֵ:
** ˵    ��: ÿ100ms����
******************************************************************************/
void lcd_mode_ctrl (void)
{
    if(CON_CPU_NORM_MODE == cpu_run_mode())//�е�״̬
    {
        if(__TRUE == check_prog_key_state())//��̼�״̬�仯
        {
            Dis_Ctrl.flash_flag |= MOD_CHG_SN_FLAG;//Ϊ��ʵʱ��ʾ ��Ϊģʽ�б仯
        }

        judge_search_state();
    }

    switch(Dis_Ctrl.cur_mode)
    {
        case DIS_MODE_MATCH://������ ���״̬
        {
            handle_search_state();
            break;
        }
        case DIS_MODE_ALLON://�ϵ�ȫ��
        {
            if(Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP))
            {
                Dis_Ctrl.lpress_ct = DIS_TIME_STLPKEY;
                Dis_Ctrl.cur_mode = DIS_MODE_SKEY;
                Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_SKEY];
                Dis_Ctrl.flash_flag |= MOD_CHG_SN_FLAG;
                if(Key_CurState & CON_KEYBIT_UP)
                {
                    Dis_Ctrl.pre_key = CON_KEYBIT_UP;
                    Dis_Ctrl.cur_diret = UP_SN_DIRECT;
                }
                else
                {
                    Dis_Ctrl.pre_key = CON_KEYBIT_DN;
                    Dis_Ctrl.cur_diret = DN_SN_DIRECT;
                }
            }
            else
            {
                if(Dis_Ctrl.t_count > 0)
                {
                    Dis_Ctrl.t_count--;
                }
                if(Dis_Ctrl.t_count == 0)
                {

                    Dis_Ctrl.cur_mode = DIS_MODE_AUTO;
                    Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_AUTO];
                    Dis_Ctrl.flash_flag |= MOD_CHG_SN_FLAG;
                    Dis_Ctrl.cur_diret = DN_SN_DIRECT;
                }
                Dis_Ctrl.pre_key = CON_KEYBIT_NO;
            }
            break;
        }
        case DIS_MODE_ERRON://���ش��� ͣ�ڴ����� ����������ʾ����
        {
            if(Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP))
            {
                Dis_Ctrl.lpress_ct = DIS_TIME_STLPKEY;
                Dis_Ctrl.cur_mode = DIS_MODE_SKEY;
                Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_SKEY];
                Dis_Ctrl.flash_flag |= MOD_CHG_SN_FLAG;
                if(Key_CurState & CON_KEYBIT_UP)
                {
                    Dis_Ctrl.pre_key = CON_KEYBIT_UP;
                    Dis_Ctrl.cur_diret = UP_SN_DIRECT;
                }
                else
                {
                    Dis_Ctrl.pre_key = CON_KEYBIT_DN;
                    Dis_Ctrl.cur_diret = DN_SN_DIRECT;
                }
            }
            else
            {
                if(__TRUE != check_meter_serious_happened())//������ �����Զ�����
                {
                    Dis_Ctrl.cur_mode = DIS_MODE_AUTO;
                    Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_AUTO];
                    Dis_Ctrl.flash_flag |= MOD_CHG_SN_FLAG;
                    Dis_Ctrl.cur_diret = DN_SN_DIRECT;
                    Dis_Ctrl.pre_key = CON_KEYBIT_NO;
                }
            }
            break;
        }
        case DIS_MODE_AUTO:
        {
            if(Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP))
            {
                Dis_Ctrl.lpress_ct = DIS_TIME_STLPKEY;
                Dis_Ctrl.cur_mode = DIS_MODE_SKEY;
                Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_SKEY];
                Dis_Ctrl.flash_flag |= MOD_CHG_SN_FLAG;
                if(Key_CurState & CON_KEYBIT_UP)
                {
//���Զ�����ʾ�����Ե�һ���й�ȥ����ԭ������űȽϺ���
/*
                    if(Dis_Ctrl.dis_bsn[DIS_MODE_SKEY] > 0)
                    {
                        Dis_Ctrl.dis_bsn[DIS_MODE_SKEY]--;
                    }
                    else
                    {
                        Dis_Ctrl.dis_bsn[DIS_MODE_SKEY] = Dis_Para.maxn[DIS_MODE_SKEY] - 1;
                    }
*/
                    Dis_Ctrl.pre_key = CON_KEYBIT_UP;
                    Dis_Ctrl.cur_diret = UP_SN_DIRECT;
                }
                else
                {
/*                  Dis_Ctrl.dis_bsn[DIS_MODE_SKEY]++;
                    if(Dis_Ctrl.dis_bsn[DIS_MODE_SKEY] >= Dis_Para.maxn[DIS_MODE_SKEY])
                    {
                        Dis_Ctrl.dis_bsn[DIS_MODE_SKEY] = 0;
                    }
*/                  Dis_Ctrl.pre_key = CON_KEYBIT_DN;
                    Dis_Ctrl.cur_diret = DN_SN_DIRECT;
                }
            }
            else
            {
                if(Dis_Ctrl.t_count > 0)
                {
                    Dis_Ctrl.t_count--;
                }
                if(Dis_Ctrl.t_count == 0)
                {
                    Dis_Ctrl.cur_diret = DN_SN_DIRECT;
                    handle_page_sn(DIS_MODE_AUTO, DN_SN_DIRECT);
                    Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_AUTO];
                }
                Dis_Ctrl.pre_key = CON_KEYBIT_NO;
            }
            break;
        }
        case DIS_MODE_SKEY:
        {
            if(Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP))
            {
                if(Dis_Ctrl.pre_key == CON_KEYBIT_NO)//����ģʽ�ٴΰ��¼�
                {
                    if(Key_CurState & CON_KEYBIT_UP)
                    {
                        Dis_Ctrl.cur_diret = UP_SN_DIRECT;
                        handle_page_sn(DIS_MODE_SKEY, UP_SN_DIRECT);
                        Dis_Ctrl.pre_key = CON_KEYBIT_UP;
                    }
                    else
                    {
                        Dis_Ctrl.cur_diret = DN_SN_DIRECT;
                        handle_page_sn(DIS_MODE_SKEY, DN_SN_DIRECT);
                        Dis_Ctrl.pre_key = CON_KEYBIT_DN;
                    }

                }
                else//��������ʱ��ʼ
                {
                    if(CON_CPU_NORM_MODE == cpu_run_mode())//�е�״̬���г���ģʽ
                    {
                        if(Dis_Ctrl.lpress_ct > 0)
                        {
                            Dis_Ctrl.lpress_ct--;
                        }
                        if(Dis_Ctrl.lpress_ct == 0)
                        {
                            if((Key_CurState == CON_KEYBIT_DN) && (Dis_Ctrl.pre_key == CON_KEYBIT_DN))
                            {
                                Dis_Ctrl.cur_diret = DN_SN_DIRECT;
                                Dis_Ctrl.cur_mode = DIS_MODE_LKEY;
                                Dis_Ctrl.lpress_ct = DIS_TIME_LPKEY;
                            }
                            else if((Key_CurState == CON_KEYBIT_UP) && (Dis_Ctrl.pre_key == CON_KEYBIT_UP))
                            {
                                Dis_Ctrl.cur_diret = UP_SN_DIRECT;
                                Dis_Ctrl.cur_mode = DIS_MODE_LKEY;
                                Dis_Ctrl.lpress_ct = DIS_TIME_LPKEY;
                            }
                            else
                            {
                                Dis_Ctrl.lpress_ct = DIS_TIME_STLPKEY;
                                Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_SKEY];
                                Dis_Ctrl.pre_key = (Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP));
                            }
                        }
                    }
                    else//�ڵ������ ��ʱ�ᶪ�����������ж� �������ﴦ���
                    {
                        if(PORT0_IS_HIGH(P0_KEYUP))
                        {
                            Key_CurState &= ~CON_KEYBIT_UP;
                        }
                        if(PORT0_IS_HIGH(P0_KEYDOWN))
                        {
                            Key_CurState &= ~CON_KEYBIT_DN;
                        }
                    }

                }
            }
            else//�������ɿ�
            {
                if(Dis_Ctrl.pre_key & (CON_KEYBIT_DN | CON_KEYBIT_UP))//���ɿ�����ʱ��ż�ʱ
                {
                    Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_SKEY];
                }
                Dis_Ctrl.lpress_ct = DIS_TIME_STLPKEY;
                if(Dis_Ctrl.t_count > 0)
                {
                    Dis_Ctrl.t_count--;
                }
                else
                {
                    Dis_Ctrl.cur_mode = DIS_MODE_AUTO;
                    Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_AUTO];
                    Dis_Ctrl.flash_flag |= MOD_CHG_SN_FLAG;
                    Dis_Ctrl.cur_diret = DN_SN_DIRECT;
                }
                Dis_Ctrl.pre_key = CON_KEYBIT_NO;
            }

            break;
        }
        case DIS_MODE_LKEY:
        {
            if(Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP))
            {
                if(Dis_Ctrl.lpress_ct > 0)
                {
                    Dis_Ctrl.lpress_ct--;
                }
                if(Dis_Ctrl.lpress_ct == 0)
                {
                    if(Dis_Ctrl.pre_key & CON_KEYBIT_UP)
                    {
                        Dis_Ctrl.cur_diret = UP_SN_DIRECT;
                        handle_page_sn(DIS_MODE_SKEY, UP_SN_DIRECT);
                        Dis_Ctrl.pre_key = CON_KEYBIT_UP;
                    }

                    else
                    {
                        Dis_Ctrl.cur_diret = DN_SN_DIRECT;
                        handle_page_sn(DIS_MODE_SKEY, DN_SN_DIRECT);
                        Dis_Ctrl.pre_key = CON_KEYBIT_DN;
                    }
                    Dis_Ctrl.lpress_ct = DIS_TIME_LPKEY;
                }
            }
            else//���������ɿ� �ɳ���ģʽ���밴��ģʽ ����ֲ���������ģʽ�л�
            {
                Dis_Ctrl.lpress_ct = DIS_TIME_STLPKEY;
                Dis_Ctrl.cur_mode = DIS_MODE_SKEY;
                Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_SKEY];
                Dis_Ctrl.pre_key = CON_KEYBIT_NO;
            }
            break;
        }
    }
    if(Dis_Ctrl.cur_mode == DIS_MODE_AUTO)
    {
        if(__TRUE == check_meter_serious_happened())//������ �����Զ�����
        {
            Dis_Ctrl.cur_mode = DIS_MODE_ERRON;
            Dis_Ctrl.flash_flag |= MOD_CHG_SN_FLAG;
        }
    }
    if(Dis_Ctrl.flash_flag & (BSN_CHG_SN_FLAG | MOD_CHG_SN_FLAG))//���ϵ�����Ż�ģʽ�б仯 //PON_INT_SN_FLAG
    {
        get_dis_code(Dis_Ctrl.cur_mode);//�õ���ʾ���� ������Dis_Ctrl��ʾ���ƽṹ����
    }

    lcd_backlight_ctrl(Dis_Ctrl.cur_mode);//�������
}
/******************************************************************************
** ��������: void lcd_backlight_ctrl(U8 mode)
** ��������: ������� �ϵ缰�������ӳ�15��رձ��� �ⲿ�����������
** ��ڲ���:
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void lcd_backlight_ctrl(U8 mode)
{
    if(PORT0_IS_HIGH(P0_PD_CHK) != 0)   //�������ź�����//�ⲿ�е�
    {
        if((mode == DIS_MODE_ALLON) ||(mode == DIS_MODE_SKEY) ||(mode == DIS_MODE_LKEY))
        {
            BGLED_ON();//��Һ�������Դ
            gBackLight_On_Time = DIS_TIME_BLIGHT;//10s
        }
        else if((mode == DIS_MODE_MATCH) && ((Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP)) != 0))
        {
            BGLED_ON();//��Һ�������Դ
            gBackLight_On_Time = DIS_TIME_BLIGHT;//10s
        }
        else
        {
            if(gBackLight_On_Time > DIS_TIME_BLIGHT)
            {
                gBackLight_On_Time = DIS_TIME_BLIGHT;
            }
            if(gBackLight_On_Time > 0)
            {
                gBackLight_On_Time--;
            }
            else
            {
                BGLED_OFF();
            }
        }
    }

}
/******************************************************************************
** ��������: void set_dis_para_flag (U8 id)
** ��������: �ж����Ի��߼��Բ����Ƿ���Ч(��ʾ���뼰��ʾʱ�������ù�����Ϊ��Ч)
** ��ڲ���: �� ��־
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void set_dis_para_flag (U8 id)
{
    U8  data[PAR_DIS_VALID_FLAG_LEN];


//    switch(id)
//    {
//      case 0://��������
//          sn = 0;
//          break;
//      case 1://����ʱ��
//          sn = 1;
//           break;
//      case 2://���Ա���
//          sn = 2;
//          break;
//      case 3://��������
//          sn = 3;
//          break;
//      case 4://���Ա���
//          sn = 4;
//          break;
//      }
    if(id < (PAR_DIS_VALID_FLAG_LEN-1))
    {
        file_read(PAR_DIS_VALID_FLAG_ADDR, data, PAR_DIS_VALID_FLAG_LEN);
        if(data[PAR_DIS_VALID_FLAG_LEN-1] != addverify(data, (PAR_DIS_VALID_FLAG_LEN-1)))
        {
            memset(data, 0, (PAR_DIS_VALID_FLAG_LEN-1));
        }
        data[id] = CON_DIS_PARA_VALID;
        data[PAR_DIS_VALID_FLAG_LEN-1] = addverify(data, (PAR_DIS_VALID_FLAG_LEN-1));
        file_write(PAR_DIS_VALID_FLAG_ADDR, data, PAR_DIS_VALID_FLAG_LEN);
    }
}
/******************************************************************************
** ��������: U8 check_dis_para_valid(U8 mode)
** ��������: �ж����Ի��߼��Բ����Ƿ���Ч(��ʾ���뼰��ʾʱ�������ù�����Ϊ��Ч)
** ��ڲ���: �� mode=DIS_MODE_AUTO:���Բ��� mode=DIS_MODE_SKEY:��ѯ���Բ���
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
U8 check_dis_para_valid (U8 mode)
{
    U8  data[PAR_DIS_VALID_FLAG_LEN];

    file_read(PAR_DIS_VALID_FLAG_ADDR, data, PAR_DIS_VALID_FLAG_LEN);
    if(data[PAR_DIS_VALID_FLAG_LEN-1] == addverify(data, (PAR_DIS_VALID_FLAG_LEN-1)))
    {
      switch(mode)
      {
        case DIS_MODE_AUTO:
            //�������� ����ʱ�� ���Ա���
            if((data[0] == CON_DIS_PARA_VALID) && (data[1] == CON_DIS_PARA_VALID) && (data[2] == CON_DIS_PARA_VALID))
            {
               return CON_DIS_PARA_VALID;
            }
            break;
        case DIS_MODE_SKEY:
            //�������������Ա���
            if((data[3] == CON_DIS_PARA_VALID) && (data[4] == CON_DIS_PARA_VALID))//�����˳�ʱ��û�еط�������Ĭ��ֵ15��
            {
               return CON_DIS_PARA_VALID;
            }
            break;
      }
    }
    return CON_DIS_PARA_INVALID;
}
/******************************************************************************
** ��������: void lcd_init(U8 mode)
** ��������: ��ʾ���Ƽ����ݳ�ʼ�� �жϼ��Ի����Բ����Ƿ���Ч ��Ч�Ͷ�Ĭ����ʾ����
** ��ڲ���: �� mode=0:�ϵ��ʼ�� mode=1:�������ú����
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void lcd_init(U8 mode)
{
    U8 i;

#if CODE_VER_TYPE==WLS_CUR_PRJ			//������߲ɼ���
       Dis_Para.maxn[DIS_MODE_AUTO] = DEFAULT_AUTO_DIS_MAX_NUM;
       Dis_Para.dly[DIS_MODE_AUTO] = DIS_TIME_AUTO;
       Dis_Para.para_valid[DIS_MODE_AUTO] = CON_DIS_PARA_INVALID;

       Dis_Para.maxn[DIS_MODE_SKEY] = DEFAULT_KEY_DIS_MAX_NUM;
       Dis_Para.dly[DIS_MODE_SKEY] = DIS_TIME_SKEY;
       Dis_Para.para_valid[DIS_MODE_SKEY] = CON_DIS_PARA_INVALID;
#else
    U8 rev_buff[5];

    file_read(PAR_LPDISCNT_04000301_00_ADDR, rev_buff, 5);//���ζ�ȡ����ѭ��&������ز�������μ�PAR_LPDISCNT_04000301_00_ADDR���崦

    if(CON_DIS_PARA_VALID == check_dis_para_valid(DIS_MODE_AUTO))
    {
        Dis_Para.maxn[DIS_MODE_AUTO] = bcd_to_hex(rev_buff[0]);
        Dis_Para.dly[DIS_MODE_AUTO] = (bcd_to_hex(rev_buff[1]) * 10);//
        if(Dis_Para.dly[DIS_MODE_AUTO] > 30)
        {
           Dis_Para.dly[DIS_MODE_AUTO] = DIS_TIME_AUTO;
        }
        Dis_Para.para_valid[DIS_MODE_AUTO] = CON_DIS_PARA_VALID;
    }
    else
    {
       Dis_Para.maxn[DIS_MODE_AUTO] = DEFAULT_AUTO_DIS_MAX_NUM;
       Dis_Para.dly[DIS_MODE_AUTO] = DIS_TIME_AUTO;
       Dis_Para.para_valid[DIS_MODE_AUTO] = CON_DIS_PARA_INVALID;
    }

    if(CON_DIS_PARA_VALID == check_dis_para_valid(DIS_MODE_SKEY))
    {
        Dis_Para.maxn[DIS_MODE_SKEY] = bcd_to_hex(rev_buff[PAR_KEYDIS_04000305_00_ADDR - PAR_LPDISCNT_04000301_00_ADDR]);
        Dis_Para.dly[DIS_MODE_SKEY] = DIS_TIME_SKEY; //������ʾ�˳�ʱ�� û�����õĵط� ��Ĭ��15��
        Dis_Para.para_valid[DIS_MODE_SKEY] = CON_DIS_PARA_VALID;
    }
    else
    {
       Dis_Para.maxn[DIS_MODE_SKEY] = DEFAULT_KEY_DIS_MAX_NUM;
       Dis_Para.dly[DIS_MODE_SKEY] = DIS_TIME_SKEY;
       Dis_Para.para_valid[DIS_MODE_SKEY] = CON_DIS_PARA_INVALID;
    }
#endif


    for(i=0; i<2; i++)
    {
        Dis_Ctrl.dis_bsn[i] = 0;
        Dis_Ctrl.exd_lsn[i] = 0;
        Dis_Ctrl.lsn_maxn[i] = 1;
        if(Dis_Para.dly[i] == 0)
        {
           Dis_Para.dly[i] = 1;
        }
    }
/*
    file_read(PAR_LPDISCNT_04000301_00_ADDR, rev_buff, 5);//���ζ�ȡ����ѭ��&������ز�������μ�PAR_LPDISCNT_04000301_00_ADDR���崦
    for(i=0; i<2; i++)
    {
        Dis_Para.maxn[i] = bcd_to_hex(rev_buff[i * (PAR_KEYDIS_04000305_00_ADDR - PAR_LPDISCNT_04000301_00_ADDR)]);
        if(Dis_Para.maxn[i] == 0xff)//���������Чʱ,ȡĬ��ֵ
        {
            Dis_Para.maxn[i] = DEBUG_DIS_MAX_NUM;
        }
        Dis_Ctrl.dis_bsn[i] = 0;
        Dis_Ctrl.exd_lsn[i] = 0;
        Dis_Ctrl.lsn_maxn[i] = 1;
    }
    Dis_Para.dly[DIS_MODE_AUTO] = (bcd_to_hex(rev_buff[1]) * 10);//
    Dis_Para.dly[DIS_MODE_SKEY] = (bcd_to_hex(rev_buff[1]) * 10);//����ʱ���Լδ��,��ʱ�����Թ���
    if(rev_buff[1] == 0xff)//���������Чʱ,ȡĬ��ֵ
    {
        Dis_Para.dly[DIS_MODE_AUTO] = DIS_TIME_AUTO;
        Dis_Para.dly[DIS_MODE_SKEY] = DIS_TIME_SKEY;
    }
*/
    Dis_Ctrl.cur_mode = DIS_MODE_AUTO;
    Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_AUTO];
    Dis_Ctrl.pre_key = CON_KEYBIT_NO;
    Dis_Ctrl.cur_diret = DN_SN_DIRECT;

    if(mode == 0)
    {
        Dis_Ctrl.flash_flag = PON_INT_SN_FLAG;
        if(CON_CPU_NORM_MODE == cpu_run_mode())//�е�״̬����ϵͳ����
        {
            Dis_Ctrl.cur_mode = DIS_MODE_ALLON;
            Dis_Ctrl.t_count = 60;//�ϵ�ȫ��ʾ6s
            lcd_seg_buf[LCD_BUF_LENTH-1] = 0x5a;//��ʾ�������������� �Ϳ���0.5sˢ����
        }
    }
    Dis_Ctrl.lpress_ct = DIS_TIME_STLPKEY;

    Dis_Ctrl.dis_dly_time = 0;
    Dis_Ctrl.had_write = 0;

    get_dis_code(Dis_Ctrl.cur_mode);
}
/******************************************************************************
** ��������: void get_dis_code(U8 mode)
** ��������: �õ���ʾ���� �����µ�Dis_Ctrl��ʾ���ƽṹ����
**           �ǲ���4�ֽ���ʾ���� ת����5�ֽڱ���
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ֻ���ڴ����ʾ��ű仯ʱ�Ž���
******************************************************************************/
void get_dis_code (U8 mode)
{
    U32 di;
    U8  i;
    U16 sn;
    U8  dix;
    U8  lsn;
    U8  rev_buff[5];

    if(mode == DIS_MODE_ALLON)
    {
        mode = DIS_MODE_AUTO;
    }
    else if(mode == DIS_MODE_ERRON)
    {
        mode = DIS_MODE_SKEY;
    }

    if(mode == DIS_MODE_AUTO)
    {
        mode = DIS_MODE_AUTO;
        sn = Dis_Ctrl.dis_bsn[mode];//��ȡ��ǰ��ʾ�������
        if(sn > Dis_Para.maxn[mode])
        {
            sn = 0;
            Dis_Ctrl.dis_bsn[mode] = 0;
        }

#if CODE_VER_TYPE==WLS_CUR_PRJ			//������߲ɼ���
//��ʾ�̶����Ա��
          if(sn > DEFAULT_AUTO_DIS_MAX_NUM)
          {
              Dis_Para.maxn[mode] = DEFAULT_AUTO_DIS_MAX_NUM;
              sn = 0;
              Dis_Ctrl.dis_bsn[mode] = 0;
          }
           memcpy(rev_buff, &default_key_dis_tab[sn][0], 5);
#else
        if(Dis_Para.para_valid[DIS_MODE_AUTO] == CON_DIS_PARA_VALID)
        {
            file_read(PAR_LOOPDIS1_04040101_00_ADDR + sn * 5, rev_buff, PAR_LOOPDIS1_04040101_TOT_LEN); //��ȡ��ʾ��5�ֽ�DI
        }
        else
        {
            if(sn > DEFAULT_AUTO_DIS_MAX_NUM)
            {
                Dis_Para.maxn[mode] = DEFAULT_AUTO_DIS_MAX_NUM;
                sn = 0;
                Dis_Ctrl.dis_bsn[mode] = 0;
            }
            memcpy(rev_buff, &default_auto_dis_tab[sn][0], 5);
        }
#endif

    }
    else
    {
        mode = DIS_MODE_SKEY;
        sn = Dis_Ctrl.dis_bsn[mode];//��ȡ��ǰ��ʾ�������
        if(sn > Dis_Para.maxn[mode])
        {
            sn = 0;
            Dis_Ctrl.dis_bsn[mode] = 0;
        }
#if CODE_VER_TYPE==WLS_CUR_PRJ			//������߲ɼ���
//��ʾ�̶����Ա��
          if(sn > DEFAULT_KEY_DIS_MAX_NUM)
          {
              Dis_Para.maxn[mode] = DEFAULT_AUTO_DIS_MAX_NUM;
              sn = 0;
              Dis_Ctrl.dis_bsn[mode] = 0;
          }
          memcpy(rev_buff, &default_key_dis_tab[sn][0], 5);
#else
        if(Dis_Para.para_valid[DIS_MODE_SKEY] == CON_DIS_PARA_VALID)
        {
            file_read(PAR_KEYDIS1_04040201_00_ADDR + sn * 5, rev_buff, PAR_KEYDIS1_04040201_TOT_LEN);   //��ȡ��ʾ��5�ֽ�DI
        }
        else
        {
            if(sn > DEFAULT_KEY_DIS_MAX_NUM)
            {
                Dis_Para.maxn[mode] = DEFAULT_AUTO_DIS_MAX_NUM;
                sn = 0;
                Dis_Ctrl.dis_bsn[mode] = 0;
            }
            memcpy(rev_buff, &default_key_dis_tab[sn][0], 5);
        }
#endif        
    }
//�����϶�����չ���������ֽ�
    lsn = rev_buff[4];  //��ȡ��չDI(С���)
    dix = rev_buff[3];  //��ȡ��չDI3
    di = 0;
    for(i = 0; i < 4; i++)
    {
        di += (rev_buff[i] << 8*i);
    }

    Dis_Ctrl.di = di;
    if(lsn == 0xff)//�ǲ���4�ֽ���ʾ���� ת����5�ֽڱ���
    {
        Dis_Ctrl.exd_lsn[mode] = 0;//Ĭ��ֵ
        Dis_Ctrl.lsn_maxn[mode] = 1;
        switch(dix)
        {
            case 0x00:
            case 0x02:
            {
                break;
            }
            case 0x01:
            {
                Dis_Ctrl.exd_lsn[mode] = 0;
                Dis_Ctrl.lsn_maxn[mode] = 3;
                if(Dis_Ctrl.cur_diret == UP_SN_DIRECT)
                {
                    Dis_Ctrl.exd_lsn[mode] = Dis_Ctrl.lsn_maxn[mode] - 1;
                }
                break;
            }
            case 0x03:
            {
                if((Dis_Ctrl.di&0x00ff0000) == 0x00100000)
                {
                    di = (Dis_Ctrl.di&0xffffff00);
                }
                else
                {
                    di = (Dis_Ctrl.di&0xffffff00);
                    if((Dis_Ctrl.di&0xff) != 0)//��di0=1��ʼ������Ϊ1ȥ����
                    {
                        di += 0x01;
                    }
                }
                sn = get_dis_lsn_info(di, evet_lsn_tab, DIS_AUX_EVET_NUM);
                if(sn != 0xffff)
                {
                    Dis_Ctrl.exd_lsn[mode] = evet_lsn_tab[sn].sn;
                    Dis_Ctrl.lsn_maxn[mode] = evet_lsn_tab[sn].maxn;
                    if(Dis_Ctrl.cur_diret == UP_SN_DIRECT)
                    {
                        Dis_Ctrl.exd_lsn[mode] = Dis_Ctrl.lsn_maxn[mode] - 1;
                    }
                }
                break;
            }
            case 0x04:
            {
                sn = get_dis_lsn_info(Dis_Ctrl.di, para_lsn_tab, DIS_AUX_PARA_NUM);
                if(sn != 0xffff)
                {
                    Dis_Ctrl.exd_lsn[mode] = para_lsn_tab[sn].sn;
                    Dis_Ctrl.lsn_maxn[mode] = para_lsn_tab[sn].maxn;
                    if(Dis_Ctrl.cur_diret == UP_SN_DIRECT)
                    {
                        Dis_Ctrl.exd_lsn[mode] = Dis_Ctrl.lsn_maxn[mode] - 1;
                    }
                }
                break;
            }
        }
    }
    else//����չ��5�ֽ���ʾ����
    {
        Dis_Ctrl.exd_lsn[mode] = lsn;//
        Dis_Ctrl.lsn_maxn[mode] = 1;
    }
}
/******************************************************************************
** ��������: void fill_dis_flash_data (void)
** ��������: �õ���ʾ���� ��д��˸�ַ�
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void fill_dis_flash_data (void)
{
    if(Dis_Ctrl.cur_mode == DIS_MODE_ALLON)
    {
        return;
    }
    //todo
    //��ˢ�������˸��־ Ȼ����ϴ�д��������ԭ�ⲻ��һ��д������оƬ
    fill_dis_flash_flag(lcd_seg_buf+CON_DIS_DATA_OFFSET);//������˸��־ �������ʾ����

    lcd_write_data(BU97950_HEAD_ADDR, lcd_seg_buf, 32+CON_DIS_DATA_OFFSET);
}
/******************************************************************************
** ��������: void get_flash_default_flag(void)
** ��������: ����ʵʱ�ĵ��״̬�� ת���ɶ�Ӧ����˸��־0.5��ˢ��
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void get_flash_default_flag (void)
{
    U8 i;

    gState_Flash_Flag = 0;
    for(i=0; i<CON_FLASH_BIT_MAX_NUM; i++)
    {
        gState_Flash_Flag |= ((flash_bit_default_value_tab[i] & 0x01) << i);
    }
}
/******************************************************************************
** ��������: void rollback_flash_flag (void)
** ��������: ����ʵʱ�ĵ��״̬�� ת���ɶ�Ӧ����˸��־0.5�� ȡ��
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void rollback_flash_flag (void)
{
    U8 i,flag;
    U8 sn;
    U8 bit_num;

    for(i=0; i<CON_FLASH_BIT_MAX_NUM; i++)
    {
        if(96 == meter_state_flash_bit_tab[i])//�ж�������
        {
            flag = ((gMeter_State[6] & 0x01) | ((gMeter_State[6]>>1) & 0x01));//��ѹ�͵������������һ���ж�
            if(flag == 1)
            {
                if(gState_Flash_Flag & (1 << i))
                {
                    gState_Flash_Flag &= ~(1 << i);
                }
                else
                {
                    gState_Flash_Flag |= (1 << i);
                }
            }
            else
            {
                gState_Flash_Flag &= ~(1 << i);
                gState_Flash_Flag |= ((flash_bit_default_value_tab[i] & 0x01) << i);
            }
        }
        else
        {
            sn = meter_state_flash_bit_tab[i] / 16;
            bit_num = meter_state_flash_bit_tab[i] % 16;
            if(gMeter_State[sn] & (1 << bit_num))//�����쳣ʱ
            {
                if(gState_Flash_Flag & (1 << i))
                {
                    gState_Flash_Flag &= ~(1 << i);
                }
                else
                {
                    gState_Flash_Flag |= (1 << i);
                }
            }
            else//״̬����ʱ(�����ʧѹ�� �����ж��Ƿ������)
            {
                if(flash_bit_default_value_tab[i] & 0x80)//�����λ��λ �򻹵��ж�һ��״̬ �����
                {
                    sn = meter_state_flash_bit_spectab[i] / 16;
                    bit_num = meter_state_flash_bit_spectab[i] % 16;
                    if(gMeter_State[sn] & (1 << bit_num))//�����쳣ʱ �ж��� ���������
                    {
                        gState_Flash_Flag &= ~(1 << i);//���಻��ʾ
                    }
                    else
                    {
                        gState_Flash_Flag &= ~(1 << i);
                        gState_Flash_Flag |= ((flash_bit_default_value_tab[i] & 0x01) << i);
                    }
                }
                else
                {
                    gState_Flash_Flag &= ~(1 << i);
                    gState_Flash_Flag |= (flash_bit_default_value_tab[i] << i);
                }
            }
        }
    }
    if(Meter_LED_Cnt[RS4851_CHL] > 0)//�绰��1
    {
        if(gState_Com_Flag & CON_COM_RS4851_FLASH_BIT)
        {
            gState_Com_Flag &= ~CON_COM_RS4851_FLASH_BIT;
        }
        else
        {
            gState_Com_Flag |= CON_COM_RS4851_FLASH_BIT;
        }
        if(Meter_LED_Cnt[RS4851_CHL] > LED_FLASH_TIMES)
        {
            Meter_LED_Cnt[RS4851_CHL] = LED_FLASH_TIMES;
        }
        Meter_LED_Cnt[RS4851_CHL]--;
    }
    else
    {
        gState_Com_Flag &= ~CON_COM_RS4851_FLASH_BIT;
    }
    if(Meter_LED_Cnt[RS4852_CHL] > 0)//�绰��2
    {
        if(gState_Com_Flag & CON_COM_RS4852_FLASH_BIT)
        {
            gState_Com_Flag &= ~CON_COM_RS4852_FLASH_BIT;
        }
        else
        {
            gState_Com_Flag |= CON_COM_RS4852_FLASH_BIT;
        }
        if(Meter_LED_Cnt[RS4852_CHL] > LED_FLASH_TIMES)
        {
            Meter_LED_Cnt[RS4852_CHL] = LED_FLASH_TIMES;
        }
        Meter_LED_Cnt[RS4852_CHL]--;
    }
    else
    {
        gState_Com_Flag &= ~CON_COM_RS4852_FLASH_BIT;
    }

//azh 120314 ���ӵ��1����� �����2ͣ�Ե�ص���˸
//���״̬��0������(��) 1��Ƿѹ(��˸) 2��û�ӵ��(��)
#ifdef PD_MANAGE_VER12
    flag = get_batt_state(BAT_GPRS);    //������V1.2����4.8V��س�����
#else
    flag = get_battery_state(BAT_GPRS); //����V1.3������������ѹ��������2.4V��4.8V��س�����
#endif
    if(flag==BAT_LOW)//Ƿѹ
    {
        if(gState_Com_Flag & CON_COM_GPRS_BAT_BIT)
        {
            gState_Com_Flag &= ~CON_COM_GPRS_BAT_BIT;
        }
        else
        {
            gState_Com_Flag |= CON_COM_GPRS_BAT_BIT;
        }        
    }
    else if(flag==BAT_NO)//û�ӵ��
    {
        gState_Com_Flag |= CON_COM_GPRS_BAT_BIT;
    }
    else//����
    {
        gState_Com_Flag &= ~CON_COM_GPRS_BAT_BIT;
    }            
//
#ifdef PD_MANAGE_VER12
    flag = get_batt_state(BAT_PDB);    //������V1.2����4.8V��س�����(��ͣ�Ե������ν)
#else
    flag = get_battery_state(BAT_PDB); //����V1.3������������ѹ��������2.4V��4.8V��س�����(��ͣ�Ե������ν)
#endif
    if(flag==BAT_LOW)//Ƿѹ
    {
        if(gState_Com_Flag & CON_COM_DIS_BAT_BIT)
        {
            gState_Com_Flag &= ~CON_COM_DIS_BAT_BIT;
        }
        else
        {
            gState_Com_Flag |= CON_COM_DIS_BAT_BIT;
        }        
    }
    else if(flag==BAT_NO)//û�ӵ��
    {
        gState_Com_Flag |= CON_COM_DIS_BAT_BIT;
    }
    else//����
    {
        gState_Com_Flag &= ~CON_COM_DIS_BAT_BIT;
    }            
}

/******************************************************************************
 Function name:  void lcd_display_flash(U8 cnt)
 Author       :
 Description  :
 Input        :  None
 Return       :  None
 *****************************************************************************/
void lcd_display_flash (U8 cnt)
{

//    lcd_mode_ctrl();//���Ի���Կ��ƴ���
    if((Dis_Ctrl.cur_mode == DIS_MODE_ALLON) || (Dis_Ctrl.cur_mode == DIS_MODE_ERRON)
        || (Dis_Ctrl.cur_mode == DIS_MODE_MATCH))
    {
        if(((cnt%10) == 0) || ((Dis_Ctrl.flash_flag & BSN_CHG_SN_FLAG) == BSN_CHG_SN_FLAG))
        {
            fill_dis_data();
            if((Dis_Ctrl.flash_flag & BSN_CHG_SN_FLAG) == BSN_CHG_SN_FLAG)
            {
                Dis_Ctrl.flash_flag &= ~BSN_CHG_SN_FLAG;
            }
        }
        return;
    }
#if CODE_VER_TYPE==H_METER_PRJ				//��ϸ�ѹ��
    if(((cnt%10) == 0) ||
       (Dis_Ctrl.flash_flag & (PON_INT_SN_FLAG | LSN_CHG_SN_FLAG | MOD_CHG_SN_FLAG)))//1�뵽�����б仯��������ʾ����
    {
        Dis_Ctrl.dis_dly_time = 0x83;//bit7�Ǳ�־ bit6~bit0��dly���� ����ʱ100ms ������ͬ������ȥˢ����ʾ
        Dis_Ctrl.di_finish = 0;
        Dis_Ctrl.di_resend_cnt = 0;
        if(__FALSE == dis_ctrl_sync())//������ü�ʱͬ�� �����ֱ����ʾ������ʱ
        {
            Dis_Ctrl.dis_dly_time = 0x80;
        }
//        fill_dis_data();
//        Dis_Ctrl.flash_flag &= ~(PON_INT_SN_FLAG | LSN_CHG_SN_FLAG);
    }
    if(Dis_Ctrl.dis_dly_time & 0x80)
    {
        if(Dis_Ctrl.dis_dly_time == 0x80)
        {
            if(Dis_Ctrl.di_finish || (Dis_Ctrl.di_resend_cnt == 0)) //�س�Ч������ ���һֱ�ڷ�����
            {
                fill_dis_data();
                Dis_Ctrl.dis_dly_time = 0;
                Dis_Ctrl.had_write = 1;
            }
            else
            {
                if(Dis_Ctrl.di_resend_cnt>0)
                {
                    Dis_Ctrl.dis_dly_time = 0x82;//bit7�Ǳ�־ bit6~bit0��dly���� ����ʱ100ms ������ͬ������ȥˢ����ʾ
                    Dis_Ctrl.di_finish = 0;
                    if(__FALSE == dis_ctrl_sync())//������ü�ʱͬ�� �����ֱ����ʾ������ʱ
                    {
                        Dis_Ctrl.dis_dly_time = 0x80;
                    }
                    Dis_Ctrl.di_resend_cnt--;
                }
            }
            Dis_Ctrl.di_finish = 0;
        }
        else
        {
            Dis_Ctrl.dis_dly_time--;
        }
    }
#else			//������߲ɼ���
    if(((cnt%10) == 0) ||
       (Dis_Ctrl.flash_flag & (PON_INT_SN_FLAG | LSN_CHG_SN_FLAG | MOD_CHG_SN_FLAG)))//1�뵽�����б仯��������ʾ����
    {
        Dis_Ctrl.dis_dly_time = 0;//bit7�Ǳ�־ bit6~bit0��dly���� ����ʱ100ms ������ͬ������ȥˢ����ʾ
        Dis_Ctrl.di_finish = 0;
        Dis_Ctrl.di_resend_cnt = 0;

        fill_dis_data();
        Dis_Ctrl.flash_flag &= ~(PON_INT_SN_FLAG | LSN_CHG_SN_FLAG);
    }    
#endif    
    Dis_Ctrl.flash_flag = 0;//���־

}

/******************************************************************************
 Function name:  __task void lcd_display_task (void)
 Author       :
 Description  :
 Input        :  None
 Return       :  None
 *****************************************************************************/
__task void lcd_display_task (void)
{
    U8  flash_ct;
    U16 cnt;

    cnt = 0;
    flash_ct = 0;

    for(;;)
    {
        sign_task_schedule(LCD_DISPLAY_PRIO);

        lcd_mode_ctrl();//�ϵ�ȫ�����Ի���Ե�ģʽ�����ִ���

        if(flash_ct >= 5)//��˸��־����ǰ��ˢ�� �ź���Ļ��п��ܻ���ͣ��
        {
            rollback_flash_flag();
        }
        lcd_display_flash(cnt);
//      SET_PORT0_REV(P0_GRNU_LED);
        if(flash_ct >= 5)
        {
            if((Dis_Ctrl.had_write == 0) && (lcd_seg_buf[LCD_BUF_LENTH-1] != 0x5a))//��һ��100msдLCDǰ����д�� ����Ͳ�����ˢ����
            {
                fill_dis_flash_data();//0.5��ˢ����˸��־
            }
            flash_ct = 0;
        }
        Dis_Ctrl.had_write = 0;

        os_dly_wait(OS_DLY_100MS);//OS_DLY_500MS

        flash_ct++;
        cnt++;
    }
}
/******************************************************************************
** ��������: void check_meter_serious_happened(void)
** ��������: ��ʾ�Լ������롣�������һ����������ʾ����������ͣ���ڸô�����
**           ��������ʾ���Ըı䵱ǰ����,����ʾ����ѡ�
**           ����������ٰ������й��ϣ� ʱ�Ӵ��� �洢�����ϻ���
** ��ڲ���:
**
**
** �� �� ֵ: __TRUE: �����ش�����
** ˵    ��: ÿ100ms����
******************************************************************************/
U8 check_meter_serious_happened (void)
{
    //todo
    //......

//  return  __TRUE;//�����ش�����
    return  __FALSE;//�����ش�����
}
/******************************************************************************
** ��������: U8 get_meter_serious_err_id(void)
** ��������: �õ�����������
** ��ڲ���:
**
**
** �� �� ֵ: ����������
** ˵    ��:
******************************************************************************/
U8 get_meter_serious_err_id (void)
{
    U8 id = 1;
    //todo
    //......

    return  id;//�����ش�����
}
/******************************************************************************
** ��������: void fill_dis_err_page(U8 *errn, U8 *segbuf)
** ��������: ���ݴ������ ��д��������ʾ����
** ��ڲ���: ����������
**
**
** �� �� ֵ:
** ˵    ��:
******************************************************************************/
void fill_dis_err_page (U8 *errn, U8 *segbuf)
{
    fill_err_b8segbuf(errn, 2, segbuf);//errx
}

/******************************************************************************
** ��������: void fill_search_addr_segbuf(U8 *segbuf)
** ��������: ��������ṹ�е���ʾ��� ��ʾ�������ı��ַ ��2�ֽ���ʾ��С8�� ��4�ֽ���ʾ�ڴ�8��
**           ��Ҫ��ʾ���������ı�ŵĸ�2�ֽ���ʾ��С8���ĵ�4λ С8����4λ���S01-xxxx
** ��ڲ���:
**
**
** �� �� ֵ:
** ˵    ��:
******************************************************************************/
void fill_search_addr_segbuf (U8 *segbuf)
{
    U8 temp;

    if((Dis_Search_Ctrl.totnum == 0) || (Dis_Search_Ctrl.dis_sn >= Dis_Search_Ctrl.totnum))
    {
        memset(&Dis_Search_Ctrl.addr[0][0], 0xee, 6);
        Dis_Search_Ctrl.dis_sn = 0;
    }

    fill_data_b8segbuf(&Dis_Search_Ctrl.addr[Dis_Search_Ctrl.dis_sn][0], 8, segbuf);
    fill_data_l8segbuf(&Dis_Search_Ctrl.addr[Dis_Search_Ctrl.dis_sn][4], 4, segbuf);//С8����4λ���S01-xxxx
    temp = HEX_TO_BCD((Dis_Search_Ctrl.dis_sn + 1), 1);
    fill_dis_ssn_l8_data(temp, segbuf);//Ssn-
}
/******************************************************************************
** ��������: void fill_search_set_state_segbuf(U8 *segbuf)
** ��������: ������ַ��Ч ��������ʾ SET-Good ������ʾ SET-Err
**
** ��ڲ���:
**
**
** �� �� ֵ:
** ˵    ��:
******************************************************************************/
void fill_search_set_state_segbuf (U8 *segbuf)
{
    if(Dis_Search_Ctrl.totnum == 0)
    {
        fill_special_b8segbuf(3, segbuf);//SET-Err
    }
    else
    {
        fill_special_b8segbuf(2, segbuf);//SET-Good
    }
    fill_dis_ssn_l8_data((Dis_Search_Ctrl.dis_sn + 1), segbuf);
}
/******************************************************************************
** ��������: void fill_gprs_signal_strenth_segbuf (U8 *segbuf)
** ��������: ��дGPRS�ź�ǿ�ȣ�0~31 5�ȷݼ�5����ʾ �����99��������ֵ����ʾ�źŷ���
**
** ��ڲ���:
**
**
** �� �� ֵ:
** ˵    ��:
******************************************************************************/
void fill_gprs_signal_strenth_segbuf (U8 *segbuf)
{
    U8 i,value=0;

    if(stModuleRunState.ucSignIntension<10)
    {
        value = 1;
    }
    else if(stModuleRunState.ucSignIntension<16)
    {
        value = 2;
    }
    else if(stModuleRunState.ucSignIntension<19)
    {
        value = 3;
    }
    else if(stModuleRunState.ucSignIntension<23)
    {
        value = 4;
    }
    else if(stModuleRunState.ucSignIntension<32)
    {
        value = 5;
    }
    if(stModuleRunState.bIsGPRSConnected == 0)//�����߾�����
    {
       value = 1; 
    }    
    if(value)
    {
        for(i=0; i<5; i++)
        {
            segbuf[gprs_signal_dis_tab[value][i] / 8] |= 1 << (gprs_signal_dis_tab[value][i] % 8);
        }
    }
}
/******************************************************************************
** ��������: set_di_sync_finish (U8 flag)
** ��������: ����ʾ���ݶ�ȡ�ɹ���־
**
** ��ڲ���: 0:�ǲ���Ҫֱ��ȥ������������ֱ������ɱ�־ 1:Ҫ��ʱȥ�������� ��Ҫ�ȶ�һ�¶��������ݱ�ʶ��ȷ��������
**
**
** �� �� ֵ:
** ˵    ��:
******************************************************************************/
void set_di_sync_finish(U8 flag)
{
    if(flag == 1)
    {
        if(0 == memcmp(gTmpArray+1, (U8*)&Dis_Ctrl.di, 4))
        {
            Dis_Ctrl.di_finish = 1;
        }
    }
    else
    {
       Dis_Ctrl.di_finish = 1;
    }
}


#if CODE_VER_TYPE==WLS_CUR_PRJ			//������߲ɼ���
/******************************************************************************
** ��������: U8 get_dis_mp(void)
** ��������: ��ȡҪ��ʾ�Ĳ������
** ��ڲ���: 
**
**
** �� �� ֵ:
** ˵    ��:
******************************************************************************/
U8 get_dis_mp(void)//�������
{
	U8 sn,mode;
	
	mode = Dis_Ctrl.cur_mode;
	if((mode!=DIS_MODE_AUTO) && (mode!=DIS_MODE_SKEY))
	{
		mode = DIS_MODE_AUTO;
	}	
	
	sn = Dis_Ctrl.dis_bsn[mode];//��ȡ��ǰ��ʾ�������

	if((sn >= 2) && (sn <= 6))
	{
		return 1;//������1
	}
	else if(sn > 6)
	{
		return 2;//������2
	}
	else
	{	
		return 0;//������0
	}
}

/******************************************************************************
** ��������: void get_dis_temp_data(U32 di, U8 lsn)
** ��������: ����ʾ֮ǰ��Ҫ��ʾ�����ݱ��浽һ��ȫ�ֻ���
**
** ��ڲ���: 
**
**
** �� �� ֵ:
** ˵    ��:
******************************************************************************/
void get_dis_temp_data(U32 di, U8 lsn)
{
	U8 mp,m;
	STHISVAL stHisVal;
		
	mp = get_dis_mp();
	
	memcpy(gTmpArray+1, (U8*)&di, 4);
	gTmpArray[0] = 4;//���ݳ���(�������ݱ�ʶ����)
	 
	switch(di)
	{
		case 0x04000401:  //���ܱ�ͨ�ŵ�ַ����ţ���8λ: 
			if((mp == 1) && (lsn == 0))//���߲ɼ�����ַ
			{
				rw_mp_para(1, 0x8902, gTmpArray+5, &m, 'r');
				gTmpArray[0] += 4;
			}
			else//�����ܱ� ���������ط�Ĭ�ϴ���
			{
				rw_mp_para(2, 0x8902, gTmpArray+5, &m, 'r');
				gTmpArray[0] += 6;
			}	
			break;	
		case 0x02020100:	//A����� 
			if(mp == 1)//���߲ɼ���
			{
				memcpy(gTmpArray+5, &Current_Detector_Data[0].Current[0], 3);
				gTmpArray[0] += 3;			
			}
			else//�����ܱ� ���������ط�Ĭ�ϴ���
			{
				stHisVal = Get_History_I_byMPNo(2,1,(unsigned char)(0xb621&0x0f));
				stHisVal.lVal = stHisVal.lVal*10;//xx.xx -> xxx.xxx
				Fill_HisUnsignVal(stHisVal,gTmpArray+5,3);//2);
				gTmpArray[0] += 3;
			}	
			break;	
		case 0x02020200:	//B�����: 
			if(mp == 1)//���߲ɼ���
			{
				memcpy(gTmpArray+5, &Current_Detector_Data[1].Current[0], 3);
				gTmpArray[0] += 3;						
			}
			else//�����ܱ� ���������ط�Ĭ�ϴ���
			{
				stHisVal = Get_History_I_byMPNo(2,1,(unsigned char)(0xb622&0x0f));//1��ʾͬһ�γ��������
				stHisVal.lVal = stHisVal.lVal*10;//xx.xx -> xxx.xxx
				Fill_HisUnsignVal(stHisVal,gTmpArray+5,3);//2);
				gTmpArray[0] += 3;
			}	
			break;		
		case 0x02020300:	//C�����: 
			if(mp == 1)//���߲ɼ���
			{
				memcpy(gTmpArray+5, &Current_Detector_Data[2].Current[0], 3);
				gTmpArray[0] += 3;						
			}
			else//�����ܱ� ���������ط�Ĭ�ϴ���
			{
				stHisVal = Get_History_I_byMPNo(2,1,(unsigned char)(0xb623&0x0f));
				stHisVal.lVal = stHisVal.lVal*10;//xx.xx -> xxx.xxx
				Fill_HisUnsignVal(stHisVal,gTmpArray+5,3);//2);
				gTmpArray[0] += 3;
			}	
			break;	
		case 0x02050000:	//˲ʱ�����ڹ���: 
			if(mp == 1)//���߲ɼ���
			{
				memcpy(gTmpArray+5, (U8*)&Real_Power, 3);
				gTmpArray[0] += 3;					
			}
			else//�����ܱ� ���������ط�Ĭ�ϴ���
			{
				
			}	
			break;	
	
		case 0x00010000:	//��ǰ�����й��ܵ���: 
			stHisVal = Get_History_DirPower_byMPNo(2,1,(unsigned char)(0x9010&0x0f));
			Fill_HisUnsignVal(stHisVal,gTmpArray+5,4);
			gTmpArray[0] += 4;
			break;	
	
		case 0x00020000:	//��ǰ�����й��ܵ���: 
			stHisVal = Get_History_UnDirPower_byMPNo(2,1,(unsigned char)(0x9020&0x0f));
			Fill_HisUnsignVal(stHisVal,gTmpArray+5,4);
			gTmpArray[0] += 4;
			break;	
		case 0x00030000:	//��ǰ����޹�1�ܵ���//�����޹��� �����޹���: 
			stHisVal = Get_History_DirVar_byMPNo(2,1,(unsigned char)(0x9110&0x0f));
			Fill_HisUnsignVal(stHisVal,gTmpArray+5,4);
			gTmpArray[0] += 4;
			break;	
		case 0x00040000:	//��ǰ����޹�2�ܵ���: 
			stHisVal = Get_History_UnDirVar_byMPNo(2,1,(unsigned char)(0x9120&0x0f));
			Fill_HisUnsignVal(stHisVal,gTmpArray+5,4);
			gTmpArray[0] += 4;
			break;	
		case 0x02010100:	//A���ѹ: 
			stHisVal = Get_History_Vol_byMPNo(2,1,(unsigned char)(0xb611&0x0f));
			stHisVal.lVal = stHisVal.lVal*10;//xxx -> xxx.x(97����07��ʽ��ʾ)
			Fill_HisUnsignVal(stHisVal,gTmpArray+5,2);
			gTmpArray[0] += 2;
			break;	
		case 0x02010200:	//B���ѹ: 
			stHisVal = Get_History_Vol_byMPNo(2,1,(unsigned char)(0xb612&0x0f));
			stHisVal.lVal = stHisVal.lVal*10;//xxx -> xxx.x
			Fill_HisUnsignVal(stHisVal,gTmpArray+5,2);
			gTmpArray[0] += 2;
			break;	
		case 0x02010300:	//C���ѹ: 
			stHisVal = Get_History_Vol_byMPNo(2,1,(unsigned char)(0xb613&0x0f));
			stHisVal.lVal = stHisVal.lVal*10;//xxx -> xxx.x
			Fill_HisUnsignVal(stHisVal,gTmpArray+5,2);
			gTmpArray[0] += 2;
			break;	
		case 0x02030000:	//˲ʱ���й�����: 
			stHisVal = Get_History_InstantPow_byMPNo(2,1,(unsigned char)(0xb630&0x0f));
			Fill_HisUnsignVal(stHisVal,gTmpArray+5,3);//xx.xxxx
			gTmpArray[0] += 3;
			break;	
		case 0x02040000:	//˲ʱ���޹�����: 
			stHisVal = Get_History_InstantVar_byMPNo(2,1,(unsigned char)(0xb640&0x0f));
			stHisVal.lVal = stHisVal.lVal*100;//xx.xx -> xx.xxxx
			Fill_HisUnsignVal(stHisVal,gTmpArray+5,3);//2);
			gTmpArray[0] += 3;
			break;	
		case 0x02060000:	//˲ʱ�ܹ�������: 
			break;	
	}

}
#endif//WLS_CUR_PRJ

#endif//LCD_VALID
/******************************************************************************
                            End Of File
*****************************************************************************/

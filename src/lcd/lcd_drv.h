/******************************************************************************
 Copyright (C) 2011  R&D Institute of HuaCai Co.,Ltd.
 File Name  : eeprom.h
 Description: ������LCD����оƬ BU97950�����궨��
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��    1.00        2011-06-12      azh           the original version
******************************************************************************/

#ifndef __LCD_DRV_H
#define __LCD_DRV_H

#ifdef LCD_VALID
//----------------------------- macro definition ------------------------------
#define BU97950_HEAD_ADDR            0x80000000     //������ʾ����оƬBU97950�ڲ���ʾRAM���߼���ʼ��ַ
#define BU97950_CAPACITY             (36)           //��������оƬBU97950������35Bytes

#define BU97950_ADDR            0x7C                //BU97950 slave address

#define ICSET      0xF6     //CLOSE Display; soft reset;Frame mode;
#define ICSET1     0xF5     //ON display;;Frame mode;
#define ICSET2     0xF1     //ON display;;Line mode;


#define DISCTL0     0xE2       //Normal   80Hz
#define DISCTL00    0xE0       //PS1      80Hz
#define DISCTL01    0xE1       //PS2      80Hz
#define DISCTL02    0xE3       //HP      80Hz

#define DISCTL1     0xE6       //Normal   71Hz
#define DISCTL10    0xE4       //PS1      71Hz
#define DISCTL11    0xE5       //PS2      71Hz
#define DISCTL12    0xE7       //HP       71Hz

#define DISCTL2     0xEA       //Normal   64Hz
#define DISCTL20    0xE8       //PS1      64Hz
#define DISCTL21    0xE9       //PS2      64Hz
#define DISCTL22    0xEB       //HP       64Hz

#define DISCTL3    0xEE       //Normal    50Hz
#define DISCTL30   0xEC       //PS1       50Hz
#define DISCTL31   0xED       //PS2       50Hz
#define DISCTL32   0xEF       //HP        50Hz

#define EVRSET     0xD0       //VCC = 5V,Vping = 3.2V;

#define ADSET      0x00       //ram��ʼ��ַ

#define APCTL      0xF8       //Normal
#define APCTL1     0xFA       //ALL ON Display
#define APCTL2     0xF9       //ALL OFF Display

//��ʾ����оƬ����ʾRAM��󳤶�
#define LCD_BUF_LENTH           40  //40bytes

//�������ݱ�ʶ���λ����ʾ���
#define POWER_DI3_00_TABLE      0x00
#define DEMAND_DI3_01_TABLE     0x01
#define REALVAR_DI3_02_TABLE    0x02
#define EVENT_DI3_03_TABLE      0x03
#define PARA_DI3_04_TABLE       0x04

//8�ַ�����Һ����ƴ��16λ��
//�����λ��ǰ ����������ʱ���ǵ�λ��ǰ!!!
#define LCD_8A      (1u<<15) //(1u<<8)
#define LCD_8B      (1u<<14) //(1u<<9)
#define LCD_8C      (1u<<13)//(1u<<10)
#define LCD_8D      (1u<<12)//(1u<<11)
#define LCD_8E      (1u<<5) //(1u<<2)
#define LCD_8F      (1u<<7) //(1u<<0)
#define LCD_8G      (1u<<6) //(1u<<1)

#define LCD_CHAR_0  (LCD_8A+LCD_8B+LCD_8C+LCD_8D+LCD_8E+LCD_8F)
#define LCD_CHAR_1  (LCD_8B+LCD_8C)
#define LCD_CHAR_2  (LCD_8A+LCD_8B+LCD_8G+LCD_8E+LCD_8D)
#define LCD_CHAR_3  (LCD_8A+LCD_8B+LCD_8G+LCD_8C+LCD_8D)
#define LCD_CHAR_4  (LCD_8B+LCD_8C+LCD_8G+LCD_8F)
#define LCD_CHAR_5  (LCD_8A+LCD_8C+LCD_8D+LCD_8F+LCD_8G)
#define LCD_CHAR_6  (LCD_8A+LCD_8C+LCD_8D+LCD_8E+LCD_8F+LCD_8G)
#define LCD_CHAR_7  (LCD_8A+LCD_8B+LCD_8C)
#define LCD_CHAR_8  (LCD_8A+LCD_8B+LCD_8C+LCD_8D+LCD_8E+LCD_8F+LCD_8G)
#define LCD_CHAR_9  (LCD_8A+LCD_8B+LCD_8C+LCD_8D+LCD_8F+LCD_8G)
#define LCD_CHAR_A  (LCD_8A+LCD_8B+LCD_8C+LCD_8E+LCD_8F+LCD_8G)
#define LCD_CHAR_b  (LCD_8C+LCD_8D+LCD_8E+LCD_8F+LCD_8G)
#define LCD_CHAR_C  (LCD_8A+LCD_8D+LCD_8E+LCD_8F)
#define LCD_CHAR_d  (LCD_8B+LCD_8C+LCD_8D+LCD_8E+LCD_8G)
#define LCD_CHAR_E  (LCD_8A+LCD_8D+LCD_8E+LCD_8F+LCD_8G)
#define LCD_CHAR_F  (LCD_8A+LCD_8E+LCD_8F+LCD_8G)
//speci
#define LCD_CHAR_r  (LCD_8E+LCD_8G)
#define LCD_CHAR_S  (LCD_8A+LCD_8F+LCD_8G+LCD_8C+LCD_8D)
#define LCD_CHAR_A  (LCD_8A+LCD_8B+LCD_8C+LCD_8E+LCD_8F+LCD_8G)
#define LCD_CHAR_c  (LCD_8D+LCD_8E+LCD_8G)
#define LCD_CHAR_h  (LCD_8C+LCD_8E+LCD_8F+LCD_8G)
#define LCD_CHAR__  LCD_8G//-
#define LCD_CHAR_T  (LCD_8A+LCD_8E+LCD_8F)
#define LCD_CHAR_G  (LCD_8A+LCD_8C+LCD_8D+LCD_8E+LCD_8F+LCD_8G)
#define LCD_CHAR_o  (LCD_8C+LCD_8D+LCD_8E+LCD_8G)
//----------------------------- type definition -------------------------------
//0xLitSn,
    //B3~B0: ͬһ���ݱ�ʶ�µ������ݲ�����
    //B4~B7: ͬһ���ݱ�ʶ�¶������ݵĲ�����

//0xSpecialCtrl
    //B1~B0: �洢ģʽ
        //=0: EEPROM;
        //=1:RAM;
        //=2:EEPROM��ָ��ƫ�Ƶ�
        //=3:����
    //B3~B2
        //=0: ���ֽ�תBCD,
        //=1: ���ֽ�תBCD
        //=2: ���ֽ�ת����8λ
    //B4=0: ֱ����ʾ��=1:����Ҫת����ʽ��ʾ
    // B6~B5: ����
    //B7=0: �޷�������=1:�з�����

//0xFixCharType,
    //�̶�������ʾ�������FixCharTable����±�

//0xDataLen,
    //B3~B0:����ԭʼ���ȣ�B4~B7:Ҫ��ʾ��8�ĸ���+1

//0xOffsetTabSn,
    // ��Դ�ƫ��ָ�뱣���¼���¼���¼���¼��Ϊ��ѭ������ģ���������һ�ű�������¼�ƫ��ָ�뱣��ĵ�ַ�����ȡ�

typedef struct{
    U32 di;         //��ʶ����
    U32 addr;       //�洢��ַ
    U8  litsn;      //��������
    U8  specialctrl;
    U16 fixchartype;//azh 110802
//  U8  fixchartype;
    U8  datalen;
    U8  offsettabsn;
} DIS_CTRL_TCB;

//typedef struct
//{
//    U32 hdaddr;           //���ݻ����׵�ַ
//    U32 ofaddr;           //4bytes ƫ��ָ����EEPROM�еı����ַ
//    U16 blocklen;     //2bytesÿ����¼���ݿ��ܳ���
//    U8  offsetmaxnum; //1byte��¼������������
//    U8  offsetdatalen;    //1byteƫ��ָ��ĳ���
//} DIS_OFFSET_CTRL_TCB;

//SEG0~SEG31 COM0~COM7  SEGX_COMY:X*8+(7-Y) ĳ���ֽ���ȫ����ʾRAM�е�λ�������fixchar_table��Ӧλ
//�����λ��ǰ ����������ʱ���ǵ�λ��ǰ!!!
#define B00_SEG03_COM7  (3*8+0)     //��ǰ
#define B01_SEG03_COM6  (3*8+1)     //��
#define B02_SEG06_COM7  (6*8+0)     //��
#define B03_SEG07_COM7  (7*8+0)     //���
#define B04_SEG08_COM7  (8*8+0)     //��
#define B05_SEG09_COM5  (9*8+2)     //��
#define B06_SEG09_COM6  (9*8+1)     //��
#define B07_SEG09_COM7  (9*8+0)     //��

#define B08_SEG10_COM6  (10*8+1)    //��
#define B09_SEG10_COM7  (10*8+0)    //��
#define B10_SEG11_COM7  (11*8+0)    //I1(������)
#define B11_SEG11_COM6  (11*8+1)    //I2
#define B12_SEG12_COM6  (12*8+1)    //I3
#define B13_SEG12_COM7  (12*8+0)    //V2
#define B14_SEG13_COM7  (13*8+0)    //��
#define B15_SEG13_COM6  (13*8+1)    //��

#define B16_SEG30_COM7  (30*8+0)    //��
#define B17_SEG30_COM6  (30*8+1)    //ƽ
#define B18_SEG31_COM6  (31*8+1)    //��
#define B19_SEG06_COM6  (6*8+1)     //A��
#define B20_SEG06_COM5  (6*8+2)     //B��
#define B21_SEG06_COM4  (6*8+3)     //C��
#define B22_SEG07_COM4  (7*8+3)     //N��
#define B23_SEG07_COM5  (7*8+2)     //COS

#define B24_SEG07_COM6  (7*8+1)     //�Ƕ�Five
#define B25_SEG08_COM6  (8*8+1)     //����
#define B26_SEG08_COM5  (8*8+2)     //ʣ��
#define B27_SEG09_COM4  (9*8+3)     //��
#define B28_SEG10_COM4  (10*8+3)    //��
#define B29_SEG10_COM5  (10*8+2)    //��
#define B30_SEG11_COM5  (11*8+2)    //��
#define B31_SEG11_COM4  (11*8+3)    //��

#define B32_SEG12_COM5  (12*8+2)    //ʧ
#define B33_SEG13_COM4  (13*8+3)    //ѹ
#define B34_SEG13_COM5  (13*8+2)    //��
#define B35_SEG30_COM5  (30*8+2)    //����
#define B36_SEG30_COM4  (30*8+3)    //ʱ
#define B37_SEG31_COM4  (31*8+3)    //��
#define B38_SEG31_COM5  (31*8+2)    //��
#define B39_SEG02_COM3  (2*8+4)     //P2С����8.8888888

#define B40_SEG04_COM3  (4*8+4)     //P3С����88.888888
#define B41_SEG06_COM3  (6*8+4)     //P4С����888.88888
#define B42_SEG08_COM3  (8*8+4)     //P5С����8888.8888
#define B43_SEG10_COM3  (10*8+4)    //P6С����88888.888
#define B44_SEG12_COM3  (12*8+4)    //P7С����888888.88
#define B45_SEG30_COM3  (30*8+4)    //P8С����8888888.8
#define B46_SEG27_COM7  (27*8+0)    //��
#define B47_SEG26_COM7  (26*8+0)    //Ԫ

#define B48_SEG29_COM7  (29*8+0)    //kV //kWAh�еĶ�
#define B49_SEG28_COM7  (28*8+0)    //V1
#define B50_SEG27_COM6  (27*8+1)    //A1
#define B51_SEG26_COM6  (26*8+1)    //h2//
#define B52_SEG29_COM6  (29*8+1)    //kvar //kvarh�еĶ�
#define B53_SEG28_COM6  (28*8+1)    //h1
#define B54_SEG08_COM4  (8*8+3)     //col1//ʱ���ϵ�С����
#define B55_SEG12_COM4  (12*8+3)    //col2

//����������ʾ���ͱ������ֽ���ȫ��SEGBUF�е�λ��
#define SPE_SEG18_COM3  (18*8+4)    //P9
#define SPE_SEG22_COM3  (22*8+4)    //P10
#define SPE_SEG26_COM3  (26*8+4)    //P11
//��̷���
#define PROG_SEG21_COM4     (21*8+3)  //I8:��̷���
//�绰ͨ�ŷ���
#define PHONE_SEG20_COM6   (20*8+1)  //�绰�����ǹ��õ�
#define PHONE1_SEG20_COM5   (20*8+2)  //�绰�ϵ�1
#define PHONE2_SEG20_COM4   (20*8+3)  //�绰�ϵ�2
//�������
#define SPE_SEG01_COM6   (1*8+1)     //cor ����ʮ��
#define COR1_SEG02_COM7  (2*8+0)     //cor1 ����
#define COR2_SEG01_COM7  (1*8+0)     //cor2 ����
#define COR3_SEG01_COM5  (1*8+2)     //cor3 ����
#define COR4_SEG02_COM6  (2*8+1)     //cor4 ����
//��ǰ����ʱ�α�
#define SPE_SEG24_COM5   (24*8+2)     //N7:ʱ�α��һ��
#define SPE_SEG24_COM4   (24*8+3)     //N8:ʱ�α�ڶ���
//��˸��ʵʱ����
#define RATE1_SEG00_COM7  (0*8+0)     //��
#define RATE2_SEG00_COM6  (0*8+1)     //��
#define RATE3_SEG18_COM4  (18*8+3)    //ƽ
#define RATE4_SEG01_COM4  (1*8+3)     //��
//��Щ�漰���״̬�ֶ�Ҫ��˸���ַ�
#define FLASH_SEG23_COM4    (23*8+3)    //N9 ʱ�ӵ��
#define FLASH_SEG23_COM5    (23*8+2)    //N10 ͣ�糭����
#define FLASH_SEG29_COM4    (29*8+3)    //Ua
#define FLASH_SEG29_COM5    (29*8+2)    //Ub
#define FLASH_SEG28_COM5    (28*8+2)    //Uc
#define FLASH_SEG27_COM4    (27*8+3)    //Ia
#define FLASH_SEG26_COM4    (26*8+3)    //Ib
#define FLASH_SEG25_COM4    (25*8+3)    //Ic
#define FLASH_SEG28_COM4    (28*8+3)    //������

#define FLASH_SEG27_COM5    (27*8+2)    //-(Ia)
#define FLASH_SEG26_COM5    (26*8+2)    //-(Ib)
#define FLASH_SEG25_COM5    (25*8+2)    //-(Ic)

//����λ
#define SEG_SIGN_0          (30*8+6)    //-
#define SEG_SIGN_1          (17*8+6)    //-x
#define SEG_SIGN_2          (10*8+6)    //-xx
#define SEG_SIGN_3          (8*8+6)     //-xxx
#define SEG_SIGN_4          (6*8+6)     //-xxxx
#define SEG_SIGN_5          (4*8+6)     //-xxxxx
#define SEG_SIGN_6          (2*8+6)     //-xxxxxx
#define SEG_SIGN_7          (0*8+6)     //-xxxxxxx
#define SEG_SIGN_8          (0*8+4)     //-xxxxxxxx

//GPRS�ź�ǿ��
#define SIGNAL_DIS_I1       (18*8+1)
#define SIGNAL_DIS_I2       (18*8+2)
#define SIGNAL_DIS_I3       (19*8+3)
#define SIGNAL_DIS_I4       (19*8+2)
#define SIGNAL_DIS_I5       (19*8+1)
//---------------------------
//���������ֽ��ڱ��fixchar_table�ж�Ӧ��λ���
#define BY0_B0  (1<<0)  //��ǰ
#define BY0_B1  (1<<1)  //��
#define BY0_B2  (1<<2)  //��
#define BY0_B3  (1<<3)  //���
#define BY0_B4  (1<<4)  //��
#define BY0_B5  (1<<5)  //��
#define BY0_B6  (1<<6)  //��
#define BY0_B7  (1<<7)  //��

#define BY1_B0  (1<<0)  //��
#define BY1_B1  (1<<1)  //��
#define BY1_B2  (1<<2)  //I1(������)
#define BY1_B3  (1<<3)  //I2
#define BY1_B4  (1<<4)  //I3
#define BY1_B5  (1<<5)  //V2
#define BY1_B6  (1<<6)  //��
#define BY1_B7  (1<<7)  //��

#define BY2_B0  (1<<0)  //��
#define BY2_B1  (1<<1)  //ƽ
#define BY2_B2  (1<<2)  //��
#define BY2_B3  (1<<3)  //A��
#define BY2_B4  (1<<4)  //B��
#define BY2_B5  (1<<5)  //C��
#define BY2_B6  (1<<6)  //N��
#define BY2_B7  (1<<7)  //COS

#define BY3_B0  (1<<0)  //�Ƕ�Five
#define BY3_B1  (1<<1)  //����
#define BY3_B2  (1<<2)  //ʣ��
#define BY3_B3  (1<<3)  //��
#define BY3_B4  (1<<4)  //��
#define BY3_B5  (1<<5)  //��
#define BY3_B6  (1<<6)  //��
#define BY3_B7  (1<<7)  //��

#define BY4_B0  (1<<0)  //ʧ
#define BY4_B1  (1<<1)  //ѹ
#define BY4_B2  (1<<2)  //��
#define BY4_B3  (1<<3)  //����
#define BY4_B4  (1<<4)  //ʱ
#define BY4_B5  (1<<5)  //��
#define BY4_B6  (1<<6)  //��
#define BY4_B7  (1<<7)  //P2С����8.8888888

#define BY5_B0  (1<<0)  //P3С����88.888888
#define BY5_B1  (1<<1)  //P4С����888.88888
#define BY5_B2  (1<<2)  //P5С����8888.8888
#define BY5_B3  (1<<3)  //P6С����88888.888
#define BY5_B4  (1<<4)  //P7С����888888.88
#define BY5_B5  (1<<5)  //P8С����8888888.8
#define BY5_B6  (1<<6)  //��
#define BY5_B7  (1<<7)  //Ԫ

#define BY6_B0  (1<<0)  //kV //kWAh�еĶ�
#define BY6_B1  (1<<1)  //V1
#define BY6_B2  (1<<2)  //A1
#define BY6_B3  (1<<3)  //h2//
#define BY6_B4  (1<<4)  //kvar //kvarh�еĶ�
#define BY6_B5  (1<<5)  //h1
#define BY6_B6  (1<<6)  //col1//ʱ���ϵ�С����
#define BY6_B7  (1<<7)  //col2

#define CON_UNIT_KWH    (BY6_B0 | BY6_B1 | BY6_B3)
#define CON_UNIT_KVAH   (BY6_B0 | BY6_B2 | BY6_B3)
#define CON_UNIT_KVARH  (BY6_B4 | BY6_B5)
#define CON_UNIT_KW     (BY6_B0 | BY6_B1)
#define CON_UNIT_KVA    (BY6_B0 | BY6_B2)
#define CON_UNIT_KVAR   (BY6_B4)
//----------------------------- type definition -------------------------------

//-------------------------- functions declaration ----------------------------
__EXTERN void lcd_reset(void);
__EXTERN void lcd_on_all_dis(void);
__EXTERN S32 lcd_write_data(U32 obj_addr, U8 *lcdbuf, U16 len);
__EXTERN S32 lcd_write_command(U32 obj_addr, U8 *lcdbuf, U16 len);
__EXTERN S8 fill_dis_fix_char(U8 *inbuf, U8 *segbuf);
__EXTERN S8 fill_dis_special_fix_char(U8 *segbuf);
__EXTERN void fill_data_b8segbuf(U8 *datbuf, U8 len, U8 *segbuf);
__EXTERN void fill_data_l8segbuf(U8 *datbuf, U8 len, U8 *segbuf);
__EXTERN void fill_data_last8_segbuf (U8 num, U8 *segbuf);
__EXTERN void fill_data_d88_segbuf(U8 data, U8 *segbuf);
__EXTERN void fill_dis_flash_flag(U8 *segbuf);
__EXTERN void fill_data_sign(U8 index, U8 *segbuf);
__EXTERN void fill_err_b8segbuf(U8 *datbuf, U8 len, U8 *segbuf);
__EXTERN void fill_dis_ssn_l8_data(U8 sn, U8 *segbuf);
__EXTERN void fill_special_b8segbuf(U8 index, U8 *segbuf);
__EXTERN void lcd_dis_off(void);
//--------------------------- variable declaration ----------------------------
#endif

#endif//__LCD_DRV_H

/******************************************************************************
    End Of File
******************************************************************************/

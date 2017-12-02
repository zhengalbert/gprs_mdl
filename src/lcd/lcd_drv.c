/******************************************************************************
 Copyright (C) 2011  R&D Institute of HuaCai Co.,Ltd.
 File Name      : i2c1.c
 Description    : ������LCD����оƬ BU97950��������
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��    1.00        2011-06-12      azh           the original version
******************************************************************************/
#include "..\inc\global_config.h"

#ifdef LCD_VALID
extern U8 lcd_seg_buf[LCD_BUF_LENTH];//azh 120313
//-------------------------------- �������� -----------------------------------

//----------------------------- type definition -------------------------------
//0xLitSn,
    //B3~B0: ͬһ���ݱ�ʶ�µ������ݲ�����
    //B4~B7: ͬһ���ݱ�ʶ�¶������ݵĲ�����

//0xSpecialCtrl
    //B1~B0
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

//-----------------------------------------
const U8 fixchar_setbit_segbuf[64] =
{
    B00_SEG03_COM7, //��ǰ
    B01_SEG03_COM6, //��
    B02_SEG06_COM7, //��
    B03_SEG07_COM7, //���
    B04_SEG08_COM7, //��
    B05_SEG09_COM5, //��
    B06_SEG09_COM6, //��
    B07_SEG09_COM7, //��

    B08_SEG10_COM6, //��
    B09_SEG10_COM7, //��
    B10_SEG11_COM7, //I1(������)
    B11_SEG11_COM6, //I2
    B12_SEG12_COM6, //I3
    B13_SEG12_COM7, //V2
    B14_SEG13_COM7, //��
    B15_SEG13_COM6, //��

    B16_SEG30_COM7, //��
    B17_SEG30_COM6, //ƽ
    B18_SEG31_COM6, //��
    B19_SEG06_COM6, //A��
    B20_SEG06_COM5, //B��
    B21_SEG06_COM4, //C��
    B22_SEG07_COM4, //N��
    B23_SEG07_COM5, //COS

    B24_SEG07_COM6, //�Ƕ�Five
    B25_SEG08_COM6, //����
    B26_SEG08_COM5, //ʣ��
    B27_SEG09_COM4, //��
    B28_SEG10_COM4, //��
    B29_SEG10_COM5, //��
    B30_SEG11_COM5, //��
    B31_SEG11_COM4, //��

    B32_SEG12_COM5, //ʧ
    B33_SEG13_COM4, //ѹ
    B34_SEG13_COM5, //��
    B35_SEG30_COM5, //����
    B36_SEG30_COM4, //ʱ
    B37_SEG31_COM4, //��
    B38_SEG31_COM5, //��
    B39_SEG02_COM3, //P2С����8.8888888

    B40_SEG04_COM3, //P3С����88.888888
    B41_SEG06_COM3, //P4С����888.88888
    B42_SEG08_COM3, //P5С����8888.8888
    B43_SEG10_COM3, //P6С����88888.888
    B44_SEG12_COM3, //P7С����888888.88
    B45_SEG30_COM3, //P8С����8888888.8
    B46_SEG27_COM7, //��
    B47_SEG26_COM7, //Ԫ

    B48_SEG29_COM7, //kV //kWAh�еĶ�
    B49_SEG28_COM7, //V1
    B50_SEG27_COM6, //A1
    B51_SEG26_COM6, //h2//
    B52_SEG29_COM6, //kvar //kvarh�еĶ�
    B53_SEG28_COM6, //h1
    B54_SEG08_COM4, //col1//ʱ���ϵ�С����
    B55_SEG12_COM4  //col2
};
static const U16 lcd_8_char[16] =
{
    LCD_CHAR_0, LCD_CHAR_1, LCD_CHAR_2, LCD_CHAR_3, LCD_CHAR_4,
    LCD_CHAR_5, LCD_CHAR_6, LCD_CHAR_7, LCD_CHAR_8, LCD_CHAR_9,
    LCD_CHAR_A, LCD_CHAR_b, LCD_CHAR_C, LCD_CHAR_d, LCD_CHAR_E, LCD_CHAR_F
};
static const U16 lcd_err_char[3] =
{
    LCD_CHAR_E, LCD_CHAR_r, LCD_CHAR_r
};
static const U16 lcd_search_char[8] =
{
    LCD_CHAR_S, LCD_CHAR_E, LCD_CHAR_A, LCD_CHAR_r, LCD_CHAR_c, LCD_CHAR_h, LCD_CHAR__, LCD_CHAR__
};
//SET-Err
static const U16 lcd_seterr_char[7] =
{
    LCD_CHAR_S, LCD_CHAR_E, LCD_CHAR_T, LCD_CHAR__, LCD_CHAR_E, LCD_CHAR_r, LCD_CHAR_r
};
//SET-Good
static const U16 lcd_setgood_char[8] =
{
    LCD_CHAR_S, LCD_CHAR_E, LCD_CHAR_T, LCD_CHAR__, LCD_CHAR_G, LCD_CHAR_o, LCD_CHAR_o, LCD_CHAR_d
};
//������8����ʾȫ�ֻ�����(16bit)�е�λ��
static const U8 lcd_b8seg_index[8] =
{
    30, 12, 10, 8, 6, 4, 2, 0
};
//��ʾ������8����ʾȫ�ֻ�����(16bit)�е�λ��
static const U8 lcd_l8seg_index[8] =
{
    28, 26, 24, 22, 20, 18, 16, 14
};
const U8 flashchar_setbit_segbuf[CON_FLASH_BIT_MAX_NUM] =
{
    FLASH_SEG23_COM4,   //N9 ʱ�ӵ��
    FLASH_SEG23_COM5,   //N10 ͣ�糭����
    FLASH_SEG29_COM4,   //Ua
    FLASH_SEG27_COM4,   //Ia
    FLASH_SEG29_COM5,   //Ub
    FLASH_SEG26_COM4,   //Ib
    FLASH_SEG28_COM5,   //Uc
    FLASH_SEG25_COM4,   //Ic
    FLASH_SEG28_COM4,   //������
};
static const U8 lcd_sign_char[9] =
{
    SEG_SIGN_0, SEG_SIGN_1, SEG_SIGN_2, SEG_SIGN_3, SEG_SIGN_4,
    SEG_SIGN_5, SEG_SIGN_6, SEG_SIGN_7, SEG_SIGN_8
};
//------------------------------ ˽�б������� ---------------------------------

//------------------------------ ˽�к������� ---------------------------------
//static S32 lcd_read_data(U32 obj_addr, U8 *lcdbuf, U16 len);

/******************************************************************************
** ��������: void lcd_on_all_dis (void)
** ��������: initial bu97950 ���ڵ�����ʾ
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void lcd_on_all_dis (void)
{
    U8 i;
//    S8 err;
    U16 len;
    U32 lcd_addr;

    len = 0;
    lcd_addr = BU97950_HEAD_ADDR;
    lcd_seg_buf[len++] = ICSET1;
    lcd_seg_buf[len++] = DISCTL0;//DISCTL22;
//  lcd_seg_buf[len++] = EVRSET;
    lcd_seg_buf[len++] = ADSET;

    for(i=0;i<32;i++)
    {
        lcd_seg_buf[len++] = 0xff;              //���RAM;
    }

    lcd_write_command(lcd_addr, lcd_seg_buf, len);
}
/******************************************************************************
** ��������: void lcd_pd_mode (void)
** ��������: CPU����ģʽ�� ��ע������Ҳ�������ģʽ
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void lcd_pd_mode (void)
{
    U8 lcd_buf[6];
    U8 len;
    U32 lcd_addr;

    len = 0;
    lcd_addr = BU97950_HEAD_ADDR;
    lcd_buf[len++] = ICSET1;
    lcd_buf[len++] = DISCTL30;

    lcd_write_command(lcd_addr, lcd_buf, len);

}
/******************************************************************************
** ��������: void lcd_dis_off (void)
** ��������: CPU����ģʽ�� ����ʾ
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void lcd_dis_off (void)
{
    U8 lcd_buf[2];
    U8 len;
    U32 lcd_addr;

    len = 0;
    lcd_addr = BU97950_HEAD_ADDR;
    lcd_buf[len++] = ICSET;//APCTL1;

    lcd_write_command(lcd_addr, lcd_buf, len);

}
/******************************************************************************
** ��������: void lcd_reset (void)
** ��������: initial bu97950
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void lcd_reset (void)
{
    U8 i;
//    S8 err;
    U16 len;
    U32 lcd_addr;

    len = 0;
    lcd_addr = BU97950_HEAD_ADDR;
    lcd_seg_buf[len++] = ICSET;
    lcd_seg_buf[len++] = DISCTL0;
    lcd_seg_buf[len++] = ADSET;

    for(i=0;i<32;i++)
    {
        lcd_seg_buf[len++] = 0x00;              //���RAM;
    }

    lcd_write_command(lcd_addr, lcd_seg_buf, len);
}

/******************************************************************************
** ��������: S32 lcd_write_command (U32 obj_addr, U8 *lcdbuf, U8 len)
** ��������: write cmd to bu97950
** ��ڲ���: obj_addr : object logic adrress
**           *lcdbuf  : pointer for the receive buffer
**           len      : length of bytes for operation
** �� �� ֵ: indicating the result of the operation
**              1: normal
**             -1: length overflow, not allowed
**             -2: object memory not found
**             -3: waiting time overflow
**        -others: state error
** ˵    ��: ��
******************************************************************************/
S32 lcd_write_command (U32 obj_addr, U8 *lcdbuf, U16 len)
{
    S32 ret;

    ret = write_i2c0(obj_addr, lcdbuf, len);
    if((ret != 1) && (CON_CPU_NORM_MODE == cpu_run_mode()))//azh 130120
    {
        Alarm_EnsureJudge_TerErr_Display();
    }

    return ret;
}

/******************************************************************************
** ��������: S32 lcd_write_data (U32 obj_addr, U8 *lcdbuf, U8 len)
** ��������: write data from bu97950
** ��ڲ���: obj_addr : object logic adrress
**           *lcdbuf  : pointer for the receive buffer
**           len      : length of bytes for operation
** �� �� ֵ: indicating the result of the operation
**              1: normal
**             -1: length overflow, not allowed
**             -2: object memory not found
**             -3: waiting time overflow
**        -others: state error
** ˵    ��: ��
******************************************************************************/
S32 lcd_write_data (U32 obj_addr, U8 *lcdbuf, U16 len)
{
    S32 ret;

    ret = write_i2c0(obj_addr, lcdbuf, len);

    //if(ret == 1)
    if((ret == 1) && (CON_CPU_NORM_MODE == cpu_run_mode()))//120308Ҫ���е������� Ҫ���ͽ�hardfault�쳣��
    {
        set_normal_run_flag(1 << 2);                    //������Ч��ʶ���֮һ��ʾͨ������ȷ��
    }
    return ret;
}

/******************************************************************************
** ��������: S32 lcd_read_data (U32 obj_addr, U8 *lcdbuf, U8 len)
** ��������: read data from bu97950
** ��ڲ���: obj_addr : object logic adrress
**           *lcdbuf  : pointer for the receive buffer
**           len      : length of bytes for operation
** �� �� ֵ: indicating the result of the operation
**              1: normal
**             -1: length overflow, not allowed
**             -2: object memory not found
**             -3: waiting time overflow
**        -others: state error
** ˵    ��: ��
******************************************************************************/
/*
S32 lcd_read_data (U32 obj_addr, U8 *lcdbuf, U16 len)
{
    S32 ret;

    ret = read_i2c0(obj_addr, lcdbuf, len);

    return ret;
}
*/

/******************************************************************************
** ��������: void fill_data_segbuf(U8 *datbuf, U8 len, U8 *segbuf)
** ��������: ��Ҫ��ʾ���ݴ�ŵ�ȫ����ʾRAM
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void fill_data_b8segbuf (U8 *datbuf, U8 len, U8 *segbuf)
{
    U8 i;
    U8 data;

    data = 0;
    if(len > 8) len = 8;
    for(i=0; i<len; i++)
    {
        if((i%2) == 0) data = datbuf[i/2];
        segbuf[lcd_b8seg_index[i]] |= lcd_8_char[data&0x0f];
        segbuf[lcd_b8seg_index[i]+1] |= (lcd_8_char[data&0x0f]>>8);
        data >>= 4;
    }
}
/******************************************************************************
** ��������: void fill_err_b8segbuf(U8 *datbuf, U8 len, U8 *segbuf)
** ��������: ��Ҫ��ʾ���ݴ�ŵ�ȫ����ʾRAM
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void fill_err_b8segbuf (U8 *datbuf, U8 len, U8 *segbuf)
{
    U8 i;

    if(len > 4) len = 4;
    fill_data_b8segbuf(datbuf, len, segbuf);
    for(i=0; i<3; i++)//Err
    {
        segbuf[lcd_b8seg_index[3-i-1+len]] |= lcd_err_char[i];
        segbuf[lcd_b8seg_index[3-i-1+len]+1] |= (lcd_err_char[i]>>8);
    }
}
/******************************************************************************
** ��������: void fill_special_b8segbuf(U8 index, U8 *segbuf)
** ��������: 1:SEArch-- 2:"SET-Good" 3:SET-Err
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void fill_special_b8segbuf(U8 index, U8 *segbuf)
{
    U8 i;

    switch(index)
    {
        case 1:
        {
            for(i=0; i<8; i++)//SEArch--
            {
                segbuf[lcd_b8seg_index[8-i-1]] |= lcd_search_char[i];
                segbuf[lcd_b8seg_index[8-i-1]+1] |= (lcd_search_char[i]>>8);
            }
            break;
        }
        case 2:
        {
            for(i=0; i<8; i++)//SET-Good
            {
                segbuf[lcd_b8seg_index[8-i-1]] |= lcd_setgood_char[i];
                segbuf[lcd_b8seg_index[8-i-1]+1] |= (lcd_setgood_char[i]>>8);
            }
            break;
        }
        case 3:
        {
            for(i=0; i<7; i++)//SET-Err
            {
                segbuf[lcd_b8seg_index[7-i-1]] |= lcd_seterr_char[i];
                segbuf[lcd_b8seg_index[7-i-1]+1] |= (lcd_seterr_char[i]>>8);
            }
            break;
        }
    }
}
/******************************************************************************
** ��������: void fill_data_l8segbuf(U8 *datbuf, U8 len, U8 *segbuf)
** ��������: ��Ҫ��ʾ�����ŵ�ȫ����ʾRAM
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void fill_data_l8segbuf (U8 *datbuf, U8 len, U8 *segbuf)
{
    U8 i;
    U8 data;

    data = 0;
    i = 0;
    if(i > 8) i = 8;
    for(i=0; i<len; i++)
    {
        if((i%2) == 0) data = datbuf[i/2];
        segbuf[lcd_l8seg_index[i]] |= lcd_8_char[data&0x0f];
        segbuf[lcd_l8seg_index[i]+1] |= (lcd_8_char[data&0x0f]>>8);
        data >>= 4;
    }
    //if �Զ����� ��Ҫ��ʾ������ ������ʾΪ���Ա���
    if(Dis_Ctrl.cur_mode == DIS_MODE_AUTO)
    {
        segbuf[SPE_SEG18_COM3 / 8] |= 1 << (SPE_SEG18_COM3 % 8);
        segbuf[SPE_SEG22_COM3 / 8] |= 1 << (SPE_SEG22_COM3 % 8);
        segbuf[SPE_SEG26_COM3 / 8] |= 1 << (SPE_SEG26_COM3 % 8);
    }

    //if ���� ��Ҫ��ʾС���� ������ʾΪ���
}
/******************************************************************************
** ��������: void fill_data_last8_segbuf(U8 num, U8 *segbuf)
** ��������: ��Ҫ�������xx,��ʾ���ݴ�ŵ�ȫ����ʾRAM
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void fill_data_last8_segbuf (U8 num, U8 *segbuf)
{
    num = HEX_TO_BCD(num, 1);
    segbuf[4] |= (lcd_8_char[num&0x0f]>>4);//����4λ��Ϊ��������8��ʾ��ͬ
    segbuf[5] |= (lcd_8_char[num&0x0f]>>12);
    num >>= 4;
    if(num >=1 )
    {
        segbuf[4] |= 1;//0BC seg
    }
}
/******************************************************************************
** ��������: void fill_data_d88_segbuf(U8 data, U8 *segbuf)
** ��������: �����½ǵĶ���88��ʾ���ݴ�ŵ�ȫ����ʾRAM
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void fill_data_d88_segbuf (U8 data, U8 *segbuf)
{
    data = HEX_TO_BCD(data, 1);

    segbuf[16] |= (lcd_8_char[data&0x0f]>>4);//����4λ��Ϊ��������8��ʾ��ͬ
    segbuf[17] |= (lcd_8_char[data&0x0f]>>12);
    data >>= 4;
    segbuf[14] |= (lcd_8_char[data&0x0f]>>4);
    segbuf[15] |= (lcd_8_char[data&0x0f]>>12);
}
/******************************************************************************
** ��������: S8 fill_dis_fix_char(U8 disfixsn, U8 *segbuf)
** ��������: ͨ���̶��ַ���ʾ������,��д��ʾ������
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
S8 fill_dis_fix_char (U8 *inbuf, U8 *segbuf)
{
    U8 i;
    U8 sn;
    U8 b;
    U8 data;

    for(i=0; i< 8; i++)
    {
        if(inbuf[i] > 0)//����
        {
            data = inbuf[i];
            for(b=0; b<8; b++)
            {
                if((data&(1<<b)) > 0)
                {
                    sn = i*8 + b;
                    if(sn>=64)
                    {
                        break;
                    }
                    sn = fixchar_setbit_segbuf[sn];//�ѱ������������λת��ȫ��SEGBUF�е�λ���
                    segbuf[0 + sn / 8] |= 1 << (sn % 8);
                }
            }
        }
    }
    return __TRUE;
}
/******************************************************************************
** ��������: void fill_dis_flash_flag (U8 *segbuf)
** ��������: ������˸��־ �������ʾ����
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void fill_dis_flash_flag (U8 *segbuf)
{
    U8 i;
    U8 sn;

    for(i=0; i< CON_FLASH_BIT_MAX_NUM; i++)
    {
        sn = flashchar_setbit_segbuf[i];//�ѱ�־״̬����������λת��ȫ��SEGBUF�е�λ���
        if((gState_Flash_Flag & (1<<i)) > 0)//����
        {
            segbuf[sn / 8] |= 1 << (sn % 8);
            if((sn == FLASH_SEG27_COM4) && (gMeter_State[3] & 0x40))//A���������
            {
                segbuf[FLASH_SEG27_COM5 / 8] |= 1 << (FLASH_SEG27_COM5 % 8);
            }
            else if((sn == FLASH_SEG26_COM4) && (gMeter_State[4] & 0x40))//B���������
            {
                segbuf[FLASH_SEG26_COM5 / 8] |= 1 << (FLASH_SEG26_COM5 % 8);
            }
            else if((sn == FLASH_SEG25_COM4) && (gMeter_State[5] & 0x40))//C���������
            {
                segbuf[FLASH_SEG25_COM5 / 8] |= 1 << (FLASH_SEG25_COM5 % 8);
            }
        }
        else
        {
            segbuf[sn / 8] &= ~(1 << (sn % 8));
            if((sn == FLASH_SEG27_COM4) && (gMeter_State[3] & 0x40))//A���������
            {
                segbuf[FLASH_SEG27_COM5 / 8] &= ~(1 << (FLASH_SEG27_COM5 % 8));
            }
            else if((sn == FLASH_SEG26_COM4) && (gMeter_State[4] & 0x40))//B���������
            {
                segbuf[FLASH_SEG26_COM5 / 8] &= ~(1 << (FLASH_SEG26_COM5 % 8));
            }
            else if((sn == FLASH_SEG25_COM4) && (gMeter_State[5] & 0x40))//C���������
            {
                segbuf[FLASH_SEG25_COM5 / 8] &= ~(1 << (FLASH_SEG25_COM5 % 8));
            }
        }
    }
    if(gState_Com_Flag & CON_COM_RS4851_FLASH_BIT)//�绰��1 ��Ӧrs485_1ͨ��״̬
    {
        segbuf[PHONE1_SEG20_COM5 / 8] |= 1 << (PHONE1_SEG20_COM5 % 8);
    }
    else
    {
        segbuf[PHONE1_SEG20_COM5 / 8] &= ~(1 << (PHONE1_SEG20_COM5 % 8));
    }
    if(gState_Com_Flag & CON_COM_RS4852_FLASH_BIT)//�绰��2 ��Ӧrs485_2ͨ��״̬
    {
        segbuf[PHONE2_SEG20_COM4 / 8] |= 1 << (PHONE2_SEG20_COM4 % 8);
    }
    else
    {
        segbuf[PHONE2_SEG20_COM4 / 8] &= ~(1 << (PHONE2_SEG20_COM4 % 8));
    }
    //�绰�����ǹ��õ� ����������� 1 2�����·485
    if((gState_Com_Flag & CON_COM_RS4851_FLASH_BIT) || (gState_Com_Flag & CON_COM_RS4852_FLASH_BIT))
    {
        segbuf[PHONE_SEG20_COM6 / 8] |= 1 << (PHONE_SEG20_COM6 % 8);
    }
    else
    {
        segbuf[PHONE_SEG20_COM6 / 8] &= ~(1 << (PHONE_SEG20_COM6 % 8));
    }

//azh 120314 ���ӵ��1����� �����2ͣ�Ե�ص���˸
//���״̬��0������(��) 1��Ƿѹ(��˸) 2��û�ӵ��(��)
    if(gState_Com_Flag & CON_COM_GPRS_BAT_BIT)//���1 �����״̬
    {
        segbuf[FLASH_SEG23_COM4 / 8] |= 1 << (FLASH_SEG23_COM4 % 8);
    }
    else
    {
        segbuf[FLASH_SEG23_COM4 / 8] &= ~(1 << (FLASH_SEG23_COM4 % 8));
    }
    if(gState_Com_Flag & CON_COM_DIS_BAT_BIT)//�绰��2 ��Ӧrs485_2ͨ��״̬
    {
        segbuf[FLASH_SEG23_COM5 / 8] |= 1 << (FLASH_SEG23_COM5 % 8);
    }
    else
    {
        segbuf[FLASH_SEG23_COM5 / 8] &= ~(1 << (FLASH_SEG23_COM5 % 8));
    }
}
/******************************************************************************
** ��������: S8 fill_dis_special_fix_char(U8 *segbuf)
** ��������:
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
S8 fill_dis_special_fix_char (U8 *segbuf)
{
    U8 flag;

    segbuf[SPE_SEG01_COM6 / 8] |= (1 << (SPE_SEG01_COM6 % 8));//cor ����ʮ��
    //���ݵ��״̬�����λ
    //�С��޹����ʷ���ȷ������
    //Bit5:�޹����ʷ��� Bit4:�й����ʷ��� (0����1����)
    flag = ((gMeter_State[0] >> 4) & 0x03);
    switch(flag)
    {
        case 0://һ����
            segbuf[COR1_SEG02_COM7 / 8] |= (1 << (COR1_SEG02_COM7 % 8));
            break;
        case 1://������
            segbuf[COR2_SEG01_COM7 / 8] |= (1 << (COR2_SEG01_COM7 % 8));
            break;
        case 2://������
            segbuf[COR3_SEG01_COM5 / 8] |= (1 << (COR3_SEG01_COM5 % 8));
            break;
        case 3://������
            segbuf[COR4_SEG02_COM6 / 8] |= (1 << (COR4_SEG02_COM6 % 8));
            break;
    }
    //ʵʱ���� �ڵ��״̬��3 ��bit7 bit6 bit5
    //flag = ((gMeter_State[2] >> 5) & 0x07);
    flag = (gMeter_State[3] & 0x07);
    if(flag < 4)
    {
        switch(flag)
        {
            case 0://��
                segbuf[RATE1_SEG00_COM7 / 8] |= (1 << (RATE1_SEG00_COM7 % 8));
                break;
            case 1://��
                segbuf[RATE2_SEG00_COM6 / 8] |= (1 << (RATE2_SEG00_COM6 % 8));
                break;
            case 2://ƽ
                segbuf[RATE3_SEG18_COM4 / 8] |= (1 << (RATE3_SEG18_COM4 % 8));
                break;
            case 3://��
                segbuf[RATE4_SEG01_COM4 / 8] |= (1 << (RATE4_SEG01_COM4 % 8));
                break;
        }
    }
    //���״̬��3��bit0:��ǰ����ʱ�α� (0��һ�ף�1�ڶ���)
    if(gMeter_State[2] & 0x01)//�ڶ���
    {
        segbuf[SPE_SEG24_COM4 / 8] |= (1 << (SPE_SEG24_COM4 % 8));
    }
    else//��һ��
    {
        segbuf[SPE_SEG24_COM5 / 8] |= (1 << (SPE_SEG24_COM5 % 8));
    }
//��̼�״̬��ʾ
    if(__TRUE == prog_key_enable())
    {
        segbuf[PROG_SEG21_COM4 / 8] |= (1 << (PROG_SEG21_COM4 % 8));
    }
    return __TRUE;
}
/******************************************************************************
** ��������: void fill_data_sign(U8 index, U8 *segbuf)
** ��������: ��д���ݵķ���λ
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void fill_data_sign (U8 index, U8 *segbuf)
{
    segbuf[lcd_sign_char[index] / 8] |= (1 << (lcd_sign_char[index] % 8));//����λ"-"
}
/******************************************************************************
** ��������: void fill_dis_ssn_l8_data(U8 sn, U8 *segbuf)
** ��������: С8����4λ���S01-
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void fill_dis_ssn_l8_data(U8 sn, U8 *segbuf)
{
//S
    segbuf[lcd_l8seg_index[7]] |= LCD_CHAR_S;
    segbuf[lcd_l8seg_index[7]+1] |= (LCD_CHAR_S>>8);
//sn
    segbuf[lcd_l8seg_index[5]] |= lcd_8_char[sn&0x0f];
    segbuf[lcd_l8seg_index[5]+1] |= (lcd_8_char[sn&0x0f]>>8);
    sn >>= 4;
    segbuf[lcd_l8seg_index[6]] |= lcd_8_char[sn&0x0f];
    segbuf[lcd_l8seg_index[6]+1] |= (lcd_8_char[sn&0x0f]>>8);
//-
    segbuf[lcd_l8seg_index[4]] |= LCD_CHAR__;
    segbuf[lcd_l8seg_index[4]+1] |= (LCD_CHAR__>>8);
}
#endif//LCD_VALID
/******************************************************************************
    End Of File
******************************************************************************/

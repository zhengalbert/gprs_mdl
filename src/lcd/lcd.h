/******************************************************************************
 Copyright (C) 2011  R&D Institute of HuaCai Co., Ltd.
 Module         : lcd display function head file
 File Name      : lcd_display.h
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

#ifndef __LCD_DISPLAY_H
#define __LCD_DISPLAY_H

#ifdef LCD_VALID
/*----------------------------- macro definition ----------------------------*/
#define CON_DIS_PARA_INVALID  0
#define CON_DIS_PARA_VALID    0xA1

#define DN_SN_DIRECT        0//�ϴ����·�
#define UP_SN_DIRECT        1//�ϴ����Ϸ�

#define PON_INT_SN_FLAG     (1<<0)
#define BSN_CHG_SN_FLAG     (1<<1)
#define LSN_CHG_SN_FLAG     (1<<2)
#define MOD_CHG_SN_FLAG     (1<<3)

#define DIS_AUX_EVET_NUM    69
#define DIS_AUX_PARA_NUM    4

//Ĭ����ʾ����������±�
#if CODE_VER_TYPE==WLS_CUR_PRJ			//������߲ɼ���
	#define DEFAULT_AUTO_DIS_MAX_NUM   21//18//
	#define DEFAULT_KEY_DIS_MAX_NUM    21//
#else
#define DEFAULT_AUTO_DIS_MAX_NUM   18//
#define DEFAULT_KEY_DIS_MAX_NUM    59//
#endif

#define DIS_MODE_NUM        3//��ʾģʽ
//���㿪ʼ Ҫ��Ϊ������±��
#define DIS_MODE_AUTO       0//�Զ�����ģʽ
#define DIS_MODE_SKEY       1//��ͨ����
#define DIS_MODE_LKEY       2//��������ٷ�ҳģʽ
#define DIS_MODE_ERRON      3//����ʱ ͣ�ڴ����� ���ǰ���ȥ��
#define DIS_MODE_ALLON      4//�ϵ�ȫ��ʾģʽ
#define DIS_MODE_MATCH      5//�������ַ �����ʾ״̬

#define DIS_TIME_AUTO       30//100ms unit
#define DIS_TIME_SKEY       150//30//������ʾ�˳�ʱ��
#define DIS_TIME_STLPKEY    20//��һ�ν��볤����������ģʽ����Ҫ��ʱ��
#define DIS_TIME_LPKEY      5//����ģʽ�°�������ʱ��0.5S��
#define DIS_TIME_BLIGHT     100//100ms unit 10S
//-----------------------------------------------------------------------------
// ������״̬���ж�Ӧ��LCD����Ҫ��˸���ַ�
//-----------------------------------------------------------------------------
#define CON_DIS_DATA_OFFSET         3  //��ʾ��������������ı���
#define CON_FLASH_BIT_MAX_NUM       9  //��Ҫ��˸��״̬��
//���״̬������Ҫ��˸�ķ���gState_Flash_Flag
#define CON_RTCBAT_FLASH_BIT        (1<<0)  //ʱ�ӵ��Ƿѹ��˸λ
#define CON_RMTBAT_FLASH_BIT        (1<<1)  //������Ƿѹ
#define CON_ALOWVOLT_FLASH_BIT      (1<<2)  //A��ʧѹ ����
#define CON_ALOWCURT_FLASH_BIT      (1<<3)  //A��ʧ�� ����
#define CON_BLOWVOLT_FLASH_BIT      (1<<4)  //B��ʧѹ ����
#define CON_BLOWCURT_FLASH_BIT      (1<<5)  //B��ʧ�� ����
#define CON_CLOWVOLT_FLASH_BIT      (1<<6)  //C��ʧѹ ����
#define CON_CLOWCURT_FLASH_BIT      (1<<7)  //C��ʧ�� ����
#define CON_ERRPHASE_FLASH_BIT      (1<<8)  //��ѹ,����������
//ͨ����˸����gState_Com_Flag
#define CON_COM_RS4851_FLASH_BIT    (1<<0)  //�绰��1 ��Ӧrs485_1ͨ��״̬
#define CON_COM_RS4852_FLASH_BIT    (1<<1) //�绰��2 ��Ӧrs485_2ͨ��״̬
#define CON_COM_GPRS_BAT_BIT        (1<<2) //ģ������ ��˸��־
#define CON_COM_DIS_BAT_BIT         (1<<3) //ͣ���� ��˸��־
/*----------------------------- type definition -----------------------------*/
typedef struct _DIS_PARA
{
    U8  maxn[2];        //������ʾģʽ�������������������
    U8  dly[2];         //��ǰ�Զ��������
    U8  para_valid[2];  //��ʾ���� ��Ч�� ����ⲿû������ ������Ĭ����ʾ���
}DIS_PARA;

typedef struct _DIS_CTRL
{
    U8  had_write;      //100msѭ������д��һ��
    U8  dis_dly_time;   //��ͬ���������� Ȼ����ʱ100ms��ȥˢ����ʾ
    U8  pre_key;        //�ϴΰ��µļ�ֵ
    U8  cur_diret;      //�ϴΰ����������ڴ���С�����Ϸ�
    U8  cur_mode;       //��ǰģʽ
    U8  dis_bsn[2];     //��ǰ�����
    U8  exd_lsn[2];     //��չ�õ�С���
    U8  lsn_maxn[2];    //С�������
    U8  flash_flag;     //���ȵ�1���Ҫˢ��
    U8  di_finish;      //��ʾʱ�������ݱ�ʶ���
    U8  di_resend_cnt;  //������ط�����
    U16 t_count;        //������
    U16 lpress_ct;      //������ģʽ����
    U32 di;
}DIS_CTRL;

typedef struct{
    U32 di;         //645��Լ��ʶ����
    U8  sn;       //�������ʼ���
    U8  maxn;        //��ֺ�����Ŀ��
} DIS_AUX_CTRL;


/*-------------------------- functions declaration --------------------------*/
__EXTERN    __task void lcd_display_task (void);
__EXTERN S8 fill_dis_data_buf(void);
__EXTERN void lcd_init(U8 mode);
__EXTERN U8 get_meter_serious_err_id(void);
__EXTERN void fill_dis_err_page(U8 *errn, U8 *segbuf);
__EXTERN U8 check_meter_serious_happened(void);
__EXTERN void fill_dis_err_page(U8 *errn, U8 *segbuf);

__EXTERN void lcd_mode_ctrl(void);
__EXTERN void rollback_flash_flag (void);
__EXTERN void lcd_display_flash(U8 cnt);
__EXTERN void fill_dis_flash_data(void);
__EXTERN void get_flash_default_flag(void);
__EXTERN void get_dis_code(U8 mode);
__EXTERN S8 fill_dis_data(void);
__EXTERN void lcd_reset(void);
__EXTERN void lcd_pd_mode (void);
__EXTERN void set_dis_para_flag(U8 id);
__EXTERN U8 check_dis_para_valid(U8 mode);
__EXTERN void fill_gprs_signal_strenth_segbuf(U8 *segbuf);
__EXTERN void set_di_sync_finish(U8 flag);
	#if CODE_VER_TYPE==WLS_CUR_PRJ			//������߲ɼ���
		__EXTERN U8 get_dis_mp(void);
		__EXTERN void get_dis_temp_data(U32 di, U8 lsn);
	#endif
/*--------------------------- variable declaration --------------------------*/
__EXTERN DIS_CTRL Dis_Ctrl;
__EXTERN DIS_PARA Dis_Para;

__EXTERN U16 gBackLight_On_Time;
__EXTERN U16 gState_Flash_Flag;             //��Ӧ���״̬����Ҫ��˸�ķ������Ǵ���
__EXTERN U8  gState_Com_Flag;               //485ͨ����˸���� �����绰��
#endif//LCD_VALID

#endif//__LCD_DISPLAY_H
/******************************************************************************
                            End Of File
 *****************************************************************************/

/******************************************************************************
 Copyright (C) 2011  R&D Institute of Reallin Co., Ltd.
 Module         : ad on chip samplefunction
 File Name      : ad_ain.h
 Description    : LPC17ϵ��оƬӲ��Ƭ��AD�źŲ���
 Author         :
 Build Date     :
 others         :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��     1.00       2011-08-05     rosoon     the original version
******************************************************************************/

#ifndef __AD_AIN_H
#define __AD_AIN_H

//----------------------------- macro definition ------------------------------
//AD0CR�Ĵ�����ؿ��ƹ���λ�趨ֵ�궨��
#define AD_CH_SEL(n)                n << 0  //bit[7:0]��Ӧλд1Ϊʹ��ͨ��
#define AD_CLK_DIV                  19 << 8 //bit[15:8]��ֵ��1Ϊ��Ƶϵ��
#define AD_BURST(b)                 b << 16 //bit[16]Ĭ��ֵ0
#define AD_PDN(v)                   v << 21 //bit[21]:1����,0����
#define AD_START(s)                 s << 24 //bit[26:24]:1��������,0������
#define AD_EDGE                     0 << 27 //bit[27]��AD_STARTȡ2~7ʱ��Ч

//AD0INTEN�Ĵ�����ؿ��ƹ���λ�趨ֵ�궨��
#define AD_GINTEN                   0 << 8

//AD0GDR�Ĵ�����ؿ��ƹ���λ�趨ֵ�궨��
#define AD_RESULT                   4
#define AD_CHN                      24
#define AD_OVERRUN                  30
#define AD_DONE                     31
#define AD_MAX                      4

//�û�Ӧ�������궨��:�͹��Ĳ���
#define ONE_MINUTE                  60                  //1����(60��)
#define PD_AD_SAMP_GAP              10 * ONE_MINUTE     //�͹��Ĺ���AD��ز������

//�û�Ӧ�������궨��:GPRS��س�������
#define SCAN_TIME                   15                  //��ʱɨ��ʱ����15min
#define FCG_TIME                    (750 + 50)          //������ۼ�ʱ+ͣ��ʱ��,��λ:����(50��׷�ӵ�ͣ��ʱ���С����FCG_CNT)
//FCG_TIME:[(Cbat/Icharge)/Echarge = (600mAh/60mA)/0.8 = 12.5h = 750 min],��������ʱ��,����׷��ͣ��ʱ��
#define FCG_CNT                     (FCG_TIME / SCAN_TIME)  //��������(Ҳ����ͣ�������,��Ҫ��ͣ��ʱ��׷�ӵ�FCG_TIME��)

#define SCG_TIME                    30                  //��������Ϊ���������ܼ�ʱ,��λ:����
//SCG_TIME:[(Cbat/Icharge)/Echarge = (600mAh/60mA)/0.8 = 12.5h = 750 min]
#define SMP_POWER_CNT               10                  //��ֵ����(2��)�ݼ�������(2^SMP_POWER_CNT)�ε�ƽ��ֵ

#define BAT_ERR_VOLT                10                  //�ȶ������������:8����[2^12*(0.008/3.3)]
#define NO_BAT_LOW_VOLT             2730                //0xAAA:2.2V[2^12*(2.2/3.3)](��Ӧʵ��ֵ4.4V)
#define NO_BAT_HIG_VOLT             3970                //0xF82:3.2V[2^12*(3.2/3.3)](��Ӧʵ��ֵ6.4V)
#define ON_BAT_MIN_VOLT             2854                //0xB26:2.3V[2^12*(2.3/3.3)](��Ӧʵ��ֵ4.6V)
#define ON_BAT_MAX_VOLT             3850                //0xECE:3.1V[2^12*(3.1/3.3)](��Ӧʵ��ֵ6.2V)
#define ON_BAT_FCL_VOLT             3230                //0xC9E:2.6V[2^12*(2.6/3.3)](��Ӧʵ��ֵ5.2V),������ߵ�ѹ
#define ON_BAT_FCH_VOLT             3480                //0xE10:2.8V[2^12*(2.8/3.3)](��Ӧʵ��ֵ5.6V),������ߵ�ѹ

//GPRSģ�������б�궨��
#define EVEO_LIMIT_VOLT             124                 //�¼������ѹ��Χ��ֵ:100mV[2^12*(0.1/3.3)]
#define EVEO_ONE_SIXTH              682                 //�����̵�ѹ(3.3V)������֮һ(Ĭ��ֵ)
#define EVEO_ONE_FIFTH              819                 //�����̵�ѹ(3.3V)�����֮һ
#define EVEO_ONE_QUARTER            1024                //�����̵�ѹ(3.3V)���ķ�֮һ
#define EVEO_ONE_THIRD              1365                //�����̵�ѹ(3.3V)������֮һ
#define EVEO_ONE_HALF               2048                //�����̵�ѹ(3.3V)�Ķ���֮һ

//��������battery_charge_mange(void)��غ궨��
#define REF_V0_1        124     //0x07C:0.1V[2^12*(0.1/3.3)](��Ӧʵ��ֵ0.2V)
#define REF_V0_5        620     //0x26C:0.5V[2^12*(0.5/3.3)](��Ӧʵ��ֵ1.0V)
#define REF_V1_0        1241    //0x4D9:1.0V[2^12*(1.0/3.3)](��Ӧʵ��ֵ2.0V)
#define REF_V1_2        1490    //0x5D2:1.2V[2^12*(1.2/3.3)](��Ӧʵ��ֵ2.4V)
#define REF_V1_4        1738    //0x6CA:1.4V[2^12*(1.4/3.3)](��Ӧʵ��ֵ2.8V)
#define REF_V1_5        1862    //0x746:1.5V[2^12*(1.5/3.3)](��Ӧʵ��ֵ3.0V)
#define REF_V2_4        2979    //0xBA3:2.4V[2^12*(2.4/3.3)](��Ӧʵ��ֵ4.8V)
#define REF_V2_5        3103    //0xC1F:2.5V[2^12*(2.5/3.3)](��Ӧʵ��ֵ5.0V)
#define REF_V2_6        3227    //0xC9B:2.6V[2^12*(2.6/3.3)](��Ӧʵ��ֵ5.2V)
#define REF_V2_8        3475    //0xD93:2.8V[2^12*(2.8/3.3)](��Ӧʵ��ֵ5.6V)
#define REF_V2_9        3600    //0xE10:2.9V[2^12*(2.9/3.3)](��Ӧʵ��ֵ5.8V)
#define REF_V3_0        3724    //0xE8C:3.0V[2^12*(3.0/3.3)](��Ӧʵ��ֵ6.0V)
#define FCG_GATE_HIGH   REF_V2_9//δ����̬�����乤��ɨ��Vbat < 2.9Vʱ������䣬>= 2.9Vʱ�л�Ϊ����̬
#define FCG_GATE_LOW    REF_V2_6//����̬��  ���乤��ɨ��Vbat < 2.6Vʱ�л�Ϊδ����̬
#define SCG_GATE_MIN    REF_V2_6//����̬��  ���乤��ɨ��Vbat > 2.6Vʱ��������
#define SCG_NO_BAT      REF_V2_8//���(����ʱ)�����޵��ʱ��������,Vbat > 5.6V����Ϊ�޵��
#define FCG_NO_BAT      REF_V3_0//���(���  )�����޵��ʱ��������,Vbat > 3.0V����Ϊ�޵��
#define CRG_STA_FLG     1 << 1  //RTCGPREG0��bit1λ,Ϊ1��ʾ���δ�䱥��,���ڱ�ʶ������δ����״̬�л�

#define BAT_24V_MIN     REF_V0_5//��ѹ����:�ж�2.4V�͵��ʱBAT����ʵ�����޵�ѹ1.0V
#define BAT_24V_MAX     REF_V1_5//��ѹ����:�ж�2.4V�͵��ʱBAT����ʵ�����޵�ѹ3.0V
#define BAT_48V_MIN     REF_V1_5//��ѹ����:�ж�4.8V�͵��ʱBAT����ʵ�����޵�ѹ3.0V
#define BAT_48V_MAX     REF_V3_0//��ѹ����:�ж�4.8V�͵��ʱBAT����ʵ�����޵�ѹ6.0V
#define BAT_24V_LOW     REF_V1_2//��ѹ����:�ж�2.4V�͵��Ƿѹ���� V < 2.4VΪǷѹ
#define BAT_48V_LOW     REF_V2_4//��ѹ����:�ж�4.8V�͵��Ƿѹ���� V < 4.8VΪǷѹ

#define RATED_VOLT_FLG_BIT  16  //GPRS��ض��ѹ��ʶ��Bat_Stat_Flag�е���ʼλbit19:16
#define RATED_VOLT_FLG_24V  1   //1����2.4V���
#define RATED_VOLT_FLG_48V  0   //0����4.8V���������Ч
//----------------------------- type definition -------------------------------
typedef enum
{
    AD_BAT_GPRS = (1 << 0),     //AD0[0],GPRS���
    AD_BAT_RTC = (1 << 1),      //AD0[1],RTC���
    AD_BAT_PDB = (1 << 2),      //AD0[2],ͣ�Ե��
    AD_EVE_TYP = (1 << 3),      //AD0[3],GPRSģ��EVENTOUT�������
}AD_AIN_CHL;

typedef enum
{
    BAT_GPRS,  //AD0[0],GPRS���(���ֵ4.8V - ��ѹ��)ȡ1/2
    BAT_RTC,   //AD0[1],RTC���(���ֵ3.6V - ��ѹ��)ȡ1/2
    BAT_PDB,   //AD0[2],ͣ�Ե��(���ֵ 2 * 3V )ȡ1/2
    EVE_TYP,   //AD0[3],EVENTOUT�������:3.3V/N,Nȡ1~6,����ͬ�ͺ�GPRSģ��
}SMP_TYPE;

typedef enum
{
    BAT_NORMAL,     //��ص�ѹ����
    BAT_LOW,        //���Ƿѹ
    BAT_NO,         //�޵��
    BAT_UNKNOWN = 0x0F,   //���״̬δ֪
}BAT_STAT;

typedef enum
{
    GPRS_MC52I,     //������52I
    GPRS_MC53I,     //������53I
    GPRS_MC54I,     //������54I
    GPRS_MC55I,     //������55I
    GPRS_MC56I,     //������56I
    GPRS_UNKNOWN = 0x0F,   //δ֪ģ��
}GPRS_TYPE;
//GPRS��س��״̬�ṹ��
typedef enum
{
    DIS_CRG,        //���� 0
    SLW_CRG,        //���� 1
    FST_CRG,        //��� 2
    CRG_STA_FLG_BIT = 20,   //���״̬��ʶ��Bat_Stat_Flag�е���ʼλbit23:20
}BAT_CRG_STAT;

//-------------------------- functions declaration ----------------------------
__EXTERN void ad_ain_init(void);
__EXTERN void pd_bat_mange(void);
__EXTERN void bat_charge_mange(void);//ԭ����V1.2����ĳ�ʼ�汾�����Ѳ�����
__EXTERN void batt_charge_mange(void);//������V1.2����,���4.8V��س�����
__EXTERN U8 get_batt_state(U8 bat_num);//������V1.2����,���4.8V���״̬����
__EXTERN void battery_charge_mange(void);//����V1.3����������,����2.4V��4.8V��س�����
__EXTERN U8 get_battery_state(U8 bat_num);//����V1.3����������,����2.4V��4.8V���״̬����
//__EXTERN

//--------------------------- variable declaration ----------------------------
__EXTERN U16 Ad_Data[AD_MAX];   //ADת������Ĵ���
__EXTERN U32 Bat_Stat_Flag;     //���״̬��ʶ
//bit[3:0]  :GPRS���״̬ BAT_NORMAL:����  BAT_LOW:Ƿѹ   BAT_NO:û�ӵ�� BAT_UNKNOWN:δ֪״̬
//bit[7:4]  :RTC ���״̬ BAT_NORMAL:����  BAT_LOW:Ƿѹ   BAT_NO:û�ӵ�� BAT_UNKNOWN:δ֪״̬
//bit[11:8] :ͣ�Ե��״̬ BAT_NORMAL:����  BAT_LOW:Ƿѹ   BAT_NO:û�ӵ�� BAT_UNKNOWN:δ֪״̬
//bit[15:12]:GPRSģ������ 0:MC52I 1:������ 2:������ 3:������ 4:������ 0xFF:δ֪״̬
//bit[19:16]:GPRS��ض��ѹ����(Ϊ�˼���2.4V��4.8V������) 1 : 2.4V���;  0 : 4.8V���;������Ч
//bit[23:20]:���״̬ 0 : ����; 1 : ����; 2 : ���; ������Ч
//bit[31:24]:����
//__EXTERN
//__EXTERN

#endif//__AD_AIN_H

/******************************************************************************
    End Of File
******************************************************************************/

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
#include    "..\inc\global_config.h"

//-------------------------------- �������� -----------------------------------

//------------------------------ ˽�б������� ---------------------------------
U8 smp_ok_flg;          //�ȶ�������ɱ�ʶbit[2:0]�ֱ��ӦAD[2:0]
U16 smp_cnt[AD_MAX];    //����������
U32 smp_sum[AD_MAX];    //����ۼ�ֵ
U32 bat_stat_flg = 0xFFFFFFFF;   //���״̬��ʶ
//bit[3:0]  :GPRS���״̬ BAT_NORMAL:����  BAT_LOW:Ƿѹ   BAT_NO:û�ӵ�� BAT_UNKNOWN:δ֪״̬
//bit[7:4]  :RTC ���״̬ BAT_NORMAL:����  BAT_LOW:Ƿѹ   BAT_NO:û�ӵ�� BAT_UNKNOWN:δ֪״̬
//bit[11:8] :ͣ�Ե��״̬ BAT_NORMAL:����  BAT_LOW:Ƿѹ   BAT_NO:û�ӵ�� BAT_UNKNOWN:δ֪״̬
//bit[15:12]:GPRSģ������ 0:MC52I 1:������ 2:������ 3:������ 4:������ 0xFF:δ֪״̬
//bit[31:16]:����
//------------------------------ ˽�к������� ---------------------------------
static U8 get_charge_state(void);
static U8 get_gprs_type(U16 eveo_volt);
//------------------------------ ����ԭ�Ͷ��� ---------------------------------
/******************************************************************************
** ��������: void ad_ain_init(void)
** ��������: Զ����ͨ�ſڳ�ʼ�����ܺ���
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void ad_ain_init (void)
{

//IO�ڳ�ʼ��:ע����main.c�￪������жϴ���
    PCONP |= (1 << PCAD);                       //ʹ��ADC����ʱ��
    PCLKSEL0 |= (FPCLK_AD_DIV << 24);           //��������AD��Ƶϵ��1/20

#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
    PINMODE_OD1 &= ~(P0_BAT_AD);  //AD0[3:0]ͨ������(p0.26:23)�ǿ�©

    if((PINSEL1 & (0x03u << 14))!= (P0_23_FNUC << 14))   //AD������������
    {
        PINSEL1 &= ~(0x03u << 14);//��������bits
        PINSEL1 |= (P0_23_FNUC << 14);//����ADģʽ����
        PINMODE1 &= ~(0x03u << 14);//��������bits
        PINMODE1 = (P0_23_FNUC << 14);//PIN_NO_PULL
    }
#else//H_METER_PRJ,WLS_CUR_PRJ
    PINMODE_OD1 &= ~(P0_BAT_AD | P0_BAT1_AD | P0_BAT2_AD | P0_EVENTOUT);  //AD0[3:0]ͨ������(p0.26:23)�ǿ�©

    if((PINSEL1 & (0x03u << 14))!= (P0_23_FNUC << 14)
      || (PINSEL1 & (0x03u << 16))!= (P0_24_FNUC << 16)
      || (PINSEL1 & (0x03u << 18))!= (P0_25_FNUC << 18)
      || (PINSEL1 & (0x03u << 20))!= (P0_26_FNUC << 20))   //AD������������
    {
        PINSEL1 &= ~((0x03u << 14) | (0x03u << 16) | (0x03u << 18) | (0x03u << 20));//��������bits
        PINSEL1 |= ((P0_23_FNUC << 14) | (P0_24_FNUC << 16) | (P0_25_FNUC << 18) | (P0_26_FNUC << 20));//����ADģʽ����
        PINMODE1 &= ~((0x03u << 14) | (0x03u << 16) | (0x03u << 18) | (0x03u << 20));//��������bits
        PINMODE1 = ((P0_23_FNUC << 14) | (P0_24_FNUC << 16) | (P0_25_FNUC << 18) | (P0_26_FNUC << 20));//PIN_NO_PULL
    }
#endif    
    AD0CR = (AD_CH_SEL(0xF) | AD_CLK_DIV | AD_BURST(1) | AD_PDN(1) | AD_START(0));//����A/D���ƼĴ���

    smp_sum[BAT_GPRS] = smp_sum[BAT_RTC] = smp_sum[BAT_PDB] = smp_sum[EVE_TYP] = 0;
    smp_ok_flg = 0;
    Bat_Stat_Flag = 0xFFFFFFFF;         //��ʼֵ

    ADINTEN = (1 << AD_GINTEN);         //ʹ��ADDR��ȫ��DONE��־�����ж�
}

/******************************************************************************
** ��������: S8 get_ad_ain_data(U8 *buf)
** ��������: Զ����ͨ�ſ��ն��ֽ����ݹ��ܺ���
** ��ڲ���: buf �յ����ݻ���ָ��
** �� �� ֵ: 1 ��������֡
**          -1 ����Ч����
** ˵    ��: ���øú�����,��������Ч,��buf[0]Ϊ����֡����,buf[n:1]��Ч����֡
******************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		
S8 get_ad_ain_data (U8 *buf)
{
    U8 cnt;

    cnt = Ir_Ctl[IR_RX].Dat_Buf[IR_LS];                 //��ȡ������Ϣ

    Ir_Ctl[IR_RX].Dat_Tok += 1;                         //�����ۼ�
    if(Ir_Ctl[IR_RX].Dat_Tok == cnt)                    //���Ʋ�ͬ��
    {
        cnt = Ir_Ctl[IR_RX].Dat_Buf[IR_FI];             //��ȡ֡����Ϣ
        if((cnt > IR_FI) && (cnt < IR_LS))              //����֡���
        {
            memcpy(buf, Ir_Ctl[IR_RX].Dat_Buf, cnt + 1);//����֡������Ϣ
            Ir_Ctl[IR_RX].Dat_Buf[IR_FI] =  0;          //֡������Ϣ����
            return 1;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        Ir_Ctl[IR_RX].Dat_Tok = cnt;                     //����ͬ��,Ϊ�´�׼��
        return -1;
    }
}
#endif
/******************************************************************************
** ��������: void pd_bat_mange(void)
** ��������: �͹��Ĺ������AD�������ܺ���
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ֻ�ڵ���͹���ģʽ�µ���,
             ����2.4V��4.8V������
******************************************************************************/
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
void pd_bat_mange (void)
{
    U8 i;
    U16 gate_volt;
static U16 ad_result;

//--------------------------------AD��ʼ������ز���--------------------------
    if(RTCGPREG3 < PD_AD_SAMP_GAP)
    {
        return;                                 //�������ʱ�䲻�㣬ֱ���˳�
    }
    RTCGPREG3 = 0;                              //����������
    PCONP |= (1 << PCAD);                       //ʹ��ADC����ʱ��
    PCLKSEL0 |= (FPCLK_AD_DIV << 24);           //��������AD��Ƶϵ��1/20

    PINMODE_OD1 &= ~(P0_BAT_AD | P0_BAT1_AD | P0_BAT2_AD | P0_EVENTOUT);  //AD0[3:0]ͨ������(p0.26:23)�ǿ�©

    if((PINSEL1 & (0x03u << 14))!= (P0_23_FNUC << 14)
      || (PINSEL1 & (0x03u << 16))!= (P0_24_FNUC << 16)
      || (PINSEL1 & (0x03u << 18))!= (P0_25_FNUC << 18)
      || (PINSEL1 & (0x03u << 20))!= (P0_26_FNUC << 20))    //AD������������
    {
        PINSEL1 &= ~((0x03u << 14) | (0x03u << 16) | (0x03u << 18) | (0x03u << 20));//��������bits
        PINSEL1 |= ((P0_23_FNUC << 14) | (P0_24_FNUC << 16) | (P0_25_FNUC << 18) | (P0_26_FNUC << 20));//����ADģʽ����
        PINMODE1 &= ~((0x03u << 14) | (0x03u << 16) | (0x03u << 18) | (0x03u << 20));//��������bits
        PINMODE1 = ((P0_23_FNUC << 14) | (P0_24_FNUC << 16) | (P0_25_FNUC << 18) | (P0_26_FNUC << 20));//PIN_NO_PULL
    }
    AD0CR = (AD_CH_SEL(AD_BAT_GPRS) | AD_CLK_DIV | AD_BURST(0) | AD_PDN(1) | AD_START(1));//����A/D���ƼĴ���[��������GPRS���]
    while((ADSTAT & (AD_BAT_GPRS)) == 0);   //��ȡAD0STAT��ͨ��0��Done(������һ�β�������)
    ad_result = 0;                          //����
    for(i = 0; i < 4; i++)                  //��ͣģʽ����4��
    {
        AD0CR |= AD_START(1);               //[��������GPRS���]
        while((ADSTAT & (AD_BAT_GPRS)) == 0);    //��ȡAD0STAT��ͨ��0��Done
        ad_result += (ADDR1 >> 4) & 0xfff;  //�ۼ�
    }
    ad_result >>= 2;                        //�����4��ƽ��ֵ
//--------------------------------��ع������--------------------------
    Ad_Data[BAT_GPRS] = ad_result;  //azh
    if((ad_result >= BAT_48V_MIN) && (ad_result < BAT_48V_MAX))//����4.8V���
    {
        gate_volt = ON_BAT_MIN_VOLT;
    }
    else if((ad_result >= BAT_24V_MIN) && (ad_result < BAT_24V_MAX))//����2.4V���
    {
        gate_volt = (ON_BAT_MIN_VOLT >> 1);
    }
    else        //�޵��
    {
        gate_volt = (ON_BAT_MIN_VOLT >> 1);
    }
    if(ad_result <= gate_volt)         //��ֹGPRS�ɳ��ع����𻵿���
    {
        BAT_OFF();  //�ر�GPRS��أ��л�Ϊͣ�Ե�ع���
    }
    else//110309 azh
    {
        BAT_ON();  //�ر�GPRS��أ��л�Ϊͣ�Ե�ع���
        PIN_POWERON_CLR;//�ڵ��������ر�GPRSģ���Դ
    }
    PCONP &= ~(1 << PCAD);                  //����ADC����ʱ��
}
#endif
/******************************************************************************
** ��������: void ADC_IRQHandler(void)
** ��������: AD�жϷ����ӳ���
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void ADC_IRQHandler (void)
{
    U8 chn;                                             //adͨ����
    U16 result;                                         //ת�����
    U32 ad_gdr;

    ad_gdr = AD0GDR;                                    //��ȡ�Ĵ��������OVERRUN��DOENλ
    if(ad_gdr & (1u << AD_OVERRUN))
    {
        ADSTAT;                                         //״̬����
    }
    else if(ad_gdr & (1u << AD_DONE))                   //ADͨ��ת������ж�
    {
        chn = (ad_gdr >> AD_CHN) & 0x07;
        result = (ad_gdr >> AD_RESULT) & 0x0FFF;
        smp_sum[chn] += result;
        smp_cnt[chn]++;
        ADSTAT;                                         //״̬����
        if(smp_cnt[chn] >= power2(SMP_POWER_CNT))
        {
            smp_cnt[chn] = 0;
            Ad_Data[chn] = (smp_sum[chn] >> SMP_POWER_CNT);
            smp_sum[chn] = 0;
            if((max(Ad_Data[chn], result) - min(Ad_Data[chn], result)) < BAT_ERR_VOLT)
            {
                smp_ok_flg |= (1 << chn);
            }
        }
    }
}

/******************************************************************************
** ��������: void bat_charge_mange(void)
** ��������: GPRS��س������ܺ���
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: �ڷǵ��繤���¼��GPRS��ص����ޡ������䡢���䡢���Ŵ���
             ��ʼ�汾,������,�������������ع���ʧ�أ�����Ƶ���л�״̬
******************************************************************************/
void bat_charge_mange (void)
{
    U16 vbat;
static U16 fcg_timer,scg_timer;             //��������Ӷ�ʱ��

    if(!PORT0_IS_HIGH(P0_PD_CHK))           //���繤��,���账��
    {
        CHARGE_DIS();                       //������
        FAST_CHARGE_OFF();                  //���ر�
        return;
    }

    if(!(smp_ok_flg & (1 << BAT_GPRS)))     //AD����������Ч
    {
        return;
    }
    smp_ok_flg &= ~(1 << BAT_GPRS);

    vbat = Ad_Data[BAT_GPRS];
    switch(get_charge_state())
    {
    case 1:                                 //��ǰ��������״̬
        if((vbat < ON_BAT_MIN_VOLT) || (vbat > (ON_BAT_MAX_VOLT - 10 * BAT_ERR_VOLT)))    //�����𻵻��޵��
        {
            CHARGE_DIS();                   //����
            FAST_CHARGE_OFF();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
            WGP_OFF();//DBG
#endif
        }
        else if((vbat <= ON_BAT_FCL_VOLT) || (vbat <= ON_BAT_FCH_VOLT) && (((U16)(Min_Cnt + 1 - scg_timer + 0xFFFF)) >= SCG_TIME))
        {//���������Ҫ����,���1����,�����������е����ʱ����
            CHARGE_EN();                    //���
            FAST_CHARGE_ON();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
//            WGP_ON();//DBG
#endif
            fcg_timer = Min_Cnt;            //��ȡ�����������ʱ��ǰ����ʱ��
        }
        break;

    case 2:                                 //��ǰ���ڿ��״̬
        if((vbat < ON_BAT_MIN_VOLT) || (vbat > NO_BAT_HIG_VOLT))    //�����𻵻��޵��
        {
            CHARGE_DIS();                   //����
            FAST_CHARGE_OFF();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
            WGP_OFF();//DBG
#endif
        }
        else if((vbat >= ON_BAT_MAX_VOLT) || (((U16)(Min_Cnt + 1 - fcg_timer + 0xFFFF)) >= FCG_TIME))
        {//������ʱ����Ϊ����,���1����,�����ɿ���е�����ʱ����
            CHARGE_EN();                    //����
            FAST_CHARGE_OFF();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
            WGP_OFF();//DBG
#endif
            scg_timer = Min_Cnt;            //��ȡ�ɿ���������ʱ��ǰ����ʱ��,�Ա�����ʱ�䴰��,��ֹ��������
        }
        break;

    case 3:             //��ǰ���ڳ������ҿ��ر�״̬
        if((vbat >= ON_BAT_MIN_VOLT) && (vbat <= ON_BAT_MAX_VOLT))
        {
            CHARGE_EN();                    //����
            FAST_CHARGE_OFF();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
            WGP_OFF();//DBG
#endif
            scg_timer = (U16)(Min_Cnt - SCG_TIME + 0xFFFF);  //Ϊ�ɽ���->����->�������,�����ʱ�䴰������
        }
        break;

    default:
        break;
    }
}

/******************************************************************************
** ��������: U8 get_charge_state(void)
** ��������: ��ǰ���״̬���ܺ���
** ��ڲ���: ��
** �� �� ֵ: 1:����; 2:���; 3:������
** ˵    ��:
******************************************************************************/
U8 get_charge_state (void)
{
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
    if(PORT2_IS_HIGH(P2_CHAG_ON))    //���ʹ��
    {
        if(PORT2_IS_HIGH(P2_FAST_OFF))
#else        
    if(PORT3_IS_HIGH(P3_CHAG_ON))    //���ʹ��
    {
        if(PORT3_IS_HIGH(P3_FAST_OFF))
#endif
        {
            return 1;                //����״̬
        }
        else
        {
            return 2;                //���״̬
        }
    }
    else                             //������
    {
        return 3;
    }
}

/******************************************************************************
** ��������: void batt_charge_mange(void)
** ��������: GPRS��س������ܺ���
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: �ڷǵ��繤���¼��GPRS��ص����ޡ������䡢���䡢���Ŵ���
             �����4.8V��س�����(�����������,����Ƶ���л����״̬)
******************************************************************************/
void batt_charge_mange (void)
{
    U16 tmp;
    U16 vbat;
static U8 fcg_cnt = 0;          //��������
static U8 save_time = 0;        //����⵽�е��ʱΪѸ�ٽ����������Ҫ��Լ��ʱ��:������
static U16 bat_on_flg = 1;      //GPRS������ޱ�ʶ
//bit0: Ϊ1 �е��(Ĭ��)��Ϊ0 �޵��
//bit1:7����
//bit15:8  ��ʱ�������������ж����ߵ�صĵ�ѹ���
static U16 scan_timer = 0xFFFF; //��ֹͬһ��ʱ�걻���ʹ�ã�Ӧ�ý�ʹ��һ�μ���

    if(!PORT0_IS_HIGH(P0_PD_CHK))           //���繤��,���账��
    {
        CHARGE_DIS();                       //������
        FAST_CHARGE_OFF();                  //���ر�
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ	
        WGP_OFF();//DBG
        DBG_LED_OFF();//DBG
#endif
        return;
    }

    if(!(smp_ok_flg & (1 << BAT_GPRS)))     //AD����������Ч
    {
        return;
    }
    else                                    //������Ч����������޵�ض�̬�ж�
    {
        smp_ok_flg &= ~(1 << BAT_GPRS);     //��ʶ��λ

        vbat = Ad_Data[BAT_GPRS];
        if(vbat > FCG_NO_BAT)               //�޵��ʱ����Ϊ�����Ը����б�������
        {
            CHARGE_EN();                    //���ʹ��
            FAST_CHARGE_OFF();              //���ر�
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ	
            WGP_OFF();//DBG
            DBG_LED_OFF();//DBG
            ALARM_LED_ON();//DBG:�޵�ر���
#endif
            bat_on_flg &= ~(1 << 0);        //�޵�ر�ʶ
            bat_stat_flg &= ~(0xFF << (BAT_GPRS * 4));  //���λ������
            bat_stat_flg |= (BAT_NO << (BAT_GPRS * 4)); //����GPRS���״̬���Ա�����������ѯ
            return;
        }
        else if(0 == (bat_on_flg & 1))      //�е��ʱ������һ�μ���
        {
            bat_on_flg |= (1 << 0);         //���е��
            save_time = Min_Cnt % SCAN_TIME;//����Ӧ��Լ��ʱ��
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
            ALARM_LED_OFF();//DBG:�޵�ر������
#endif
        }
#if CODE_VER_TYPE==GPRS_MODULE_PRJ
        if((bat_on_flg & 1) && (PORT2_IS_HIGH(P2_CHAG_ON)))//�е�������,���ټ���ص�ѹ�Ա�����LCD��ʾ���״̬
#else        
        if((bat_on_flg & 1) && (PORT3_IS_HIGH(P3_CHAG_ON)))//�е�������,���ټ���ص�ѹ�Ա�����LCD��ʾ���״̬
#endif
        {
            if((bat_on_flg >> 8) > 2)           //���е�غ���ʱ��ȡAD�������ݱȽ��ȶ��ɿ�
            {
                bat_on_flg &= ~(0xFF << 8);     //����������
                vbat = Ad_Data[BAT_GPRS];
                if(vbat > REF_V2_4)             //GPRS��ص�ѹ > 4.8V Ϊ����
                {
                    bat_stat_flg &= ~(0x0F << (BAT_GPRS * 4));      //���λ������
                    bat_stat_flg |= (BAT_NORMAL << (BAT_GPRS * 4)); //����GPRS���״̬���Ա�����������ѯ
                }
                else if(vbat <= REF_V2_4)       //GPRS��ص�ѹ <= 4.8V ΪǷѹ
                {
                    bat_stat_flg &= ~(0x0F << (BAT_GPRS * 4));      //���λ������
                    bat_stat_flg |= (BAT_LOW << (BAT_GPRS * 4));    //����GPRS���״̬���Ա�����������ѯ
                }
            }
            bat_on_flg += (1 << 8);
        }
    }

    if(scan_timer != Min_Cnt)
    {
        scan_timer = Min_Cnt;
    }
    else
    {
        return;                 //ͬһ����ʱ���ʹ��һ��
    }

    tmp = (scan_timer - save_time) % SCAN_TIME;//ÿSCAN_TIME����ͣ��һ���ӣ�ÿSCAN_TIME+1���Ӽ��һ��Vbat
    if(0 == tmp)                //��ʱ�л�Ϊ����״̬����ȷɨ��Vbatֵ�Ա������Ƶ�س��
    {
        CHARGE_DIS();           //����
        FAST_CHARGE_OFF();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
        WGP_OFF();//DBG
        DBG_LED_OFF();//DBG
#endif
    }
    else if(1 == tmp)           //״̬�л�1���Ӻ���ɨ�����ֵ�Ƚ��ȶ�
    {
        vbat = Ad_Data[BAT_GPRS];
        if(CRG_STA_FLG & RTCGPREG0)  //���δ��������
        {
            if(vbat < FCG_GATE_HIGH)
            {
                fcg_cnt++;
                if(fcg_cnt < ((U16)FCG_CNT))
                {
                    CHARGE_EN();        //���
                    FAST_CHARGE_ON();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
//                    WGP_ON();//DBG
#endif
                }
                else
                {
                    CHARGE_EN();        //��Ϊ����
                    FAST_CHARGE_OFF();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
                    DBG_LED_ON();//DBG
#endif
                    RTCGPREG0 &= ~((U32)CRG_STA_FLG);  //��ʶ���㣬�лص�����̬
                }
            }
            else
            {
                CHARGE_EN();        //��Ϊ����
                FAST_CHARGE_OFF();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
                DBG_LED_ON();//DBG
#endif                
                RTCGPREG0 &= ~((U32)CRG_STA_FLG);  //��ʶ���㣬�лص�����̬
            }
        }
        else    //�������������
        {
            if(vbat <= FCG_GATE_LOW)
            {
                CHARGE_EN();        //��Ϊ���
                FAST_CHARGE_ON();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
//                WGP_ON();//DBG
#endif
                RTCGPREG0 |= CRG_STA_FLG;    //��ʶ��λ,�л���δ����̬
            }
            else if(vbat > SCG_GATE_MIN)
            {
                CHARGE_EN();        //����
                FAST_CHARGE_OFF();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
                DBG_LED_ON();//DBG
#endif
                RTCGPREG0 &= ~((U32)CRG_STA_FLG);  //��ʶ���㣬���ֱ���̬,�൱�ڳ�ʼ��
            }
        }
    }
}

/******************************************************************************
** ��������: void battery_charge_mange(void)
** ��������: GPRS��س������ܺ���
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: �ڷǵ��繤���¼��GPRS��ص����ޡ������䡢���䡢���Ŵ���
             ����ѹӲ�������ϼ���2.4V��4.8V��س�����(������V1.3������Ӳ���汾)
******************************************************************************/
void battery_charge_mange (void)
{
    U8 volt_type;
    U16 tmp;
    U16 vbat;
static U8 fcg_cnt = 0xFF;       //��������,��һ��fcg_cnt++��ת����0��ʼ����
static U8 save_time = 0;        //����⵽�е��ʱΪѸ�ٽ����������Ҫ��Լ��ʱ��:������
static U16 scan_timer = 0xFFFF; //��ֹͬһ��ʱ�걻���ʹ�ã�Ӧ�ý�ʹ��һ�μ���
    
    if(!PORT0_IS_HIGH(P0_PD_CHK))           //���繤��,���账��
    {
        CHARGE_DIS();                       //������
        FAST_CHARGE_OFF();                  //���ر�
#if CODE_VER_TYPE==GPRS_MODULE_PRJ
		RUN_LED_OFF(); 
#else//H_METER_PRJ,WLS_CUR_PRJ
        YGP_OFF();                          //��ʡ����
        ALARM_LED_OFF();                    //��ʡ����
        WGP_OFF();//DBG
        DBG_LED_OFF();//DBG
#endif
        Bat_Stat_Flag &= ~(0x0F << CRG_STA_FLG_BIT);    //���λ������
        Bat_Stat_Flag |= (DIS_CRG << CRG_STA_FLG_BIT);  //����GPRS���״̬���Ա�����������ѯ
        return;
    }

    if(!(smp_ok_flg & (1 << BAT_GPRS)))     //AD����������Ч
    {
        return;
    }
    else                                    //������Ч����������޵�ض�̬�ж�
    {
        smp_ok_flg &= ~(1 << BAT_GPRS);     //��ʶ��λ

        vbat = Ad_Data[BAT_GPRS];
        if((vbat >= BAT_24V_MIN) && (vbat < BAT_24V_MAX))       //����2.4V���
        {
            Bat_Stat_Flag &= ~(0x0F << RATED_VOLT_FLG_BIT);     //���λ������
            Bat_Stat_Flag |= (RATED_VOLT_FLG_24V << RATED_VOLT_FLG_BIT);    //����GPRS���״̬���Ա�����������ѯ
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
            ALARM_LED_OFF();                //�޵�ر�������
#endif
            if(vbat > BAT_24V_LOW)          //����
            {
                Bat_Stat_Flag &= ~(0x0F << (BAT_GPRS * 4));     //���λ������
                Bat_Stat_Flag |= (BAT_NORMAL << (BAT_GPRS * 4));//����GPRS���״̬���Ա�����������ѯ
            }
            else                            //Ƿѹ
            {
                Bat_Stat_Flag &= ~(0x0F << (BAT_GPRS * 4));     //���λ������
                Bat_Stat_Flag |= (BAT_LOW << (BAT_GPRS * 4));   //����GPRS���״̬���Ա�����������ѯ
            }
        }
        else if((vbat >= BAT_48V_MIN) && (vbat <= BAT_48V_MAX)) //����4.8V���
        {
            if((Bat_Stat_Flag & (1 << CRG_STA_FLG_BIT)) && (vbat > SCG_NO_BAT))//��������ʱ���ֵ�ص�ѹ����SCG_NO_BAT,���ж�Ϊ�޵��
            {
                goto __no_bat;          //�����޵�ش���
            }
            Bat_Stat_Flag &= ~(0x0F << RATED_VOLT_FLG_BIT);     //���λ������
            Bat_Stat_Flag |= (RATED_VOLT_FLG_48V << RATED_VOLT_FLG_BIT);    //����GPRS���״̬���Ա�����������ѯ
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
            ALARM_LED_OFF();                //�޵�ر�������
#endif
            if(vbat > BAT_48V_LOW)          //����
            {
                Bat_Stat_Flag &= ~(0x0F << (BAT_GPRS * 4));     //���λ������
                Bat_Stat_Flag |= (BAT_NORMAL << (BAT_GPRS * 4));//����GPRS���״̬���Ա�����������ѯ
            }
            else                            //Ƿѹ
            {
                Bat_Stat_Flag &= ~(0x0F << (BAT_GPRS * 4));     //���λ������
                Bat_Stat_Flag |= (BAT_LOW << (BAT_GPRS * 4));   //����GPRS���״̬���Ա�����������ѯ
            }
        }
        else        //�������(vbat <= 1.0V || vbat > 6.0V)����Ϊ�޵��
        {
__no_bat:   CHARGE_DIS();                   //������
            FAST_CHARGE_OFF();              //���ر�
            Bat_Stat_Flag &= ~(0x0F << (BAT_GPRS * 4));     //���λ������
            Bat_Stat_Flag |= (BAT_NO << (BAT_GPRS * 4));    //����GPRS���״̬���Ա�����������ѯ
            Bat_Stat_Flag |= (0x0F << RATED_VOLT_FLG_BIT);  //������ͱ�ʶ��λΪ��Ч
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
            ALARM_LED_ON();                 //�޵�ر�������
            WGP_OFF();//DBG
            DBG_LED_OFF();//DBG
#endif            
            Bat_Stat_Flag &= ~(0x0F << CRG_STA_FLG_BIT);    //���λ������
            Bat_Stat_Flag |= (DIS_CRG << CRG_STA_FLG_BIT);  //����GPRS���״̬���Ա�����������ѯ
            fcg_cnt = 0xFF;     //��������λ
            
            save_time = Min_Cnt % SCAN_TIME;//�����⵽��ش��޵���Ӧ��Լ��ʱ��,��Ѹ����������������,ͬʱ�����޵��ʱ����else if(1 == tmp)�����
        }
    }
    //-----------------------------�е��ʱ������---------------------------------------------
    if(scan_timer != Min_Cnt)
    {
        scan_timer = Min_Cnt;
    }
    else
    {
        return;                 //ͬһ����ʱ���ʹ��һ��
    }
    volt_type = (U8)(0x01 & (Bat_Stat_Flag >> RATED_VOLT_FLG_BIT)); //��ȡ���������Ϣ,�Ա���������Ӧ����ѡ��
    tmp = (scan_timer - save_time) % SCAN_TIME;//ÿSCAN_TIME����ͣ��һ���ӣ�ÿSCAN_TIME+1���Ӽ��һ��Vbat
    if(0 == tmp)                //��ʱ�л�Ϊ����״̬����ȷɨ��Vbatֵ�Ա������Ƶ�س��
    {
        CHARGE_DIS();           //����
        FAST_CHARGE_OFF();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
        WGP_OFF();//DBG
        DBG_LED_OFF();//DBG
#endif        
        Bat_Stat_Flag &= ~(0x0F << CRG_STA_FLG_BIT);    //���λ������
        Bat_Stat_Flag |= (DIS_CRG << CRG_STA_FLG_BIT);  //����GPRS���״̬���Ա�����������ѯ
    }
    else if(1 == tmp)           //״̬�л�1���Ӻ���ɨ�����ֵ�Ƚ��ȶ�
    {
        vbat = Ad_Data[BAT_GPRS];
        if(CRG_STA_FLG & RTCGPREG0)  //���δ��������
        {
            if(vbat < (FCG_GATE_HIGH >> volt_type))
            {
                fcg_cnt++;
                if(fcg_cnt <= ((U16)FCG_CNT))
                {
                    CHARGE_EN();    //���
                    FAST_CHARGE_ON();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
//                    WGP_ON();//DBG
#endif                    
                    Bat_Stat_Flag &= ~(0x0F << CRG_STA_FLG_BIT);    //���λ������
                    Bat_Stat_Flag |= (FST_CRG << CRG_STA_FLG_BIT);  //����GPRS���״̬���Ա�����������ѯ
                }
                else
                {
                    fcg_cnt = 0xFF;     //��������λ
                    save_time = 0;      //��λ
                    CHARGE_EN();        //��Ϊ����
                    FAST_CHARGE_OFF();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
                    DBG_LED_ON();//DBG
#endif                    
                    Bat_Stat_Flag &= ~(0x0F << CRG_STA_FLG_BIT);    //���λ������
                    Bat_Stat_Flag |= (SLW_CRG << CRG_STA_FLG_BIT);  //����GPRS���״̬���Ա�����������ѯ
                    RTCGPREG0 &= ~((U32)CRG_STA_FLG);  //��ʶ���㣬�лص�����̬
                }
            }
            else
            {
                fcg_cnt = 0xFF;     //��������λ
                save_time = 0;      //��λ
                CHARGE_EN();        //��Ϊ����
                FAST_CHARGE_OFF();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
                DBG_LED_ON();//DBG
#endif                
                Bat_Stat_Flag &= ~(0x0F << CRG_STA_FLG_BIT);    //���λ������
                Bat_Stat_Flag |= (SLW_CRG << CRG_STA_FLG_BIT);  //����GPRS���״̬���Ա�����������ѯ
                RTCGPREG0 &= ~((U32)CRG_STA_FLG);  //��ʶ���㣬�лص�����̬
            }
        }
        else    //�������������
        {
            if(vbat <= (FCG_GATE_LOW >> volt_type))
            {
                CHARGE_EN();        //��Ϊ���
                FAST_CHARGE_ON();
                fcg_cnt++;
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
//                WGP_ON();//DBG
#endif                
                Bat_Stat_Flag &= ~(0x0F << CRG_STA_FLG_BIT);    //���λ������
                Bat_Stat_Flag |= (FST_CRG << CRG_STA_FLG_BIT);  //����GPRS���״̬���Ա�����������ѯ
                RTCGPREG0 |= CRG_STA_FLG;    //��ʶ��λ,�л���δ����̬
            }
            else if(vbat > (SCG_GATE_MIN >> volt_type))
            {
                CHARGE_EN();        //����
                FAST_CHARGE_OFF();
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ
                DBG_LED_ON();//DBG
#endif                
                Bat_Stat_Flag &= ~(0x0F << CRG_STA_FLG_BIT);    //���λ������
                Bat_Stat_Flag |= (SLW_CRG << CRG_STA_FLG_BIT);  //����GPRS���״̬���Ա�����������ѯ
                RTCGPREG0 &= ~((U32)CRG_STA_FLG);  //��ʶ���㣬���ֱ���̬,�൱�ڳ�ʼ��
            }
        }
    }
}

/******************************************************************************
** ��������: U8 get_batt_state(U8 bat_num)
** ��������: ������(��GPRSģ������)����״̬��ȡ���ܺ���
** ��ڲ���: bat_num ���(��GPRSģ������)��ȡֵ����ad_ain.h��ö��SMP_TYPE������
             ����BAT_GPRS��BAT_RTC��BAT_PDB��EVE_TYP
** �� �� ֵ: ���״̬: BAT_NORMAL:����  BAT_LOW:Ƿѹ   BAT_NO:û�ӵ�� BAT_UNKNOWN:δ֪״̬
             GPRS����: 0:MC52I 1:������ 2:������ 3:������ 4:������ GPRS_UNKNOWN:δ֪״̬
** ˵    ��: ���ڵ������״̬��GPRSģ�������ж�,������0xFF����δ��������״̬�ж�
             �����4.8V��س�����
******************************************************************************/
U8 get_batt_state (U8 bat_num)
{
    U8 state;
    U16 vbat;

    switch(bat_num)
    {
    case BAT_GPRS:

        break;
    case BAT_RTC:
        if(smp_ok_flg & (1 << BAT_RTC))  //AD����������Ч
        {
            smp_ok_flg &= ~(1 << BAT_RTC);     //��ʶ��λ
            vbat = Ad_Data[BAT_RTC];
            if(vbat < REF_V0_1)         //RTC��ص�ѹ < 0.2V Ϊ�޵��
            {
                bat_stat_flg &= ~(0x0F << (BAT_RTC * 4));       //���λ������
                bat_stat_flg |= (BAT_NO << (BAT_RTC * 4));      //����RTC���״̬���Ա�����������ѯ
            }
            else if(vbat < REF_V1_4)    //RTC��ص�ѹ < 2.8V + 0.7V(��ѹ��)ΪǷѹ
            {
                bat_stat_flg &= ~(0x0F << (BAT_RTC * 4));       //���λ������
                bat_stat_flg |= (BAT_LOW << (BAT_RTC * 4));     //����RTC���״̬���Ա�����������ѯ
            }
            else                        //RTC��ص�ѹ >= 2.8V + 0.7V(��ѹ��)Ϊ����
            {
                bat_stat_flg &= ~(0x0F << (BAT_RTC * 4));       //���λ������
                bat_stat_flg |= (BAT_NORMAL << (BAT_RTC * 4));  //����RTC���״̬���Ա�����������ѯ
            }
        }
        break;
    case BAT_PDB:
        if(smp_ok_flg & (1 << BAT_PDB)) //AD����������Ч
        {
            smp_ok_flg &= ~(1 << BAT_PDB);      //��ʶ��λ
            vbat = Ad_Data[BAT_PDB];
            if(vbat < REF_V0_1)         //ͣ�Ե�ص�ѹ < 0.2V Ϊ�޵��
            {
                bat_stat_flg &= ~(0x0F << (BAT_PDB * 4));       //���λ������
                bat_stat_flg |= (BAT_NO << (BAT_PDB * 4));      //����ͣ�Ե��״̬���Ա�����������ѯ
            }
            else if(vbat < REF_V2_8)    //ͣ�Ե�ص�ѹ < 5.6V  ΪǷѹ
            {
                bat_stat_flg &= ~(0x0F << (BAT_PDB * 4));       //���λ������
                bat_stat_flg |= (BAT_LOW << (BAT_PDB * 4));     //����ͣ�Ե��״̬���Ա�����������ѯ
            }
            else                        //ͣ�Ե�ص�ѹ >= 5.6V Ϊ����
            {
                bat_stat_flg &= ~(0x0F << (BAT_PDB * 4));       //���λ������
                bat_stat_flg |= (BAT_NORMAL << (BAT_PDB * 4));  //����ͣ�Ե��״̬���Ա�����������ѯ
            }
        }
        break;
    case EVE_TYP:
        if(smp_ok_flg & (1 << EVE_TYP)) //AD����������Ч
        {
            smp_ok_flg &= ~(1 << EVE_TYP);      //��ʶ��λ
            vbat = Ad_Data[EVE_TYP];
            state = get_gprs_type(vbat);
            bat_stat_flg &= ~(0x0F << (EVE_TYP * 4)); //���λ������
            bat_stat_flg |= (state << (EVE_TYP * 4)); //����GPRSģ���������Ա�����������ѯ
        }
        break;
    default:
        return 0xEE;    //��ڲ�������
    }

	state = (U8)((bat_stat_flg >> (bat_num * 4)) & 0x0F);
    return state;
}

/******************************************************************************
** ��������: U8 get_battery_state(U8 bat_num)
** ��������: ������(��GPRSģ������)����״̬��ȡ���ܺ���
** ��ڲ���: bat_num ���(��GPRSģ������)��ȡֵ����ad_ain.h��ö��SMP_TYPE������
             ����BAT_GPRS��BAT_RTC��BAT_PDB��EVE_TYP
** �� �� ֵ: ���״̬: BAT_NORMAL:����  BAT_LOW:Ƿѹ   BAT_NO:û�ӵ�� BAT_UNKNOWN:δ֪״̬
             GPRS����: 0:MC52I 1:������ 2:������ 3:������ 4:������ GPRS_UNKNOWN:δ֪״̬
** ˵    ��: ���ڵ������״̬��GPRSģ�������ж�,������0xFF����δ��������״̬�ж�
             ����2.4V��4.8V��س�����(������V1.3������Ӳ���汾)
******************************************************************************/
U8 get_battery_state (U8 bat_num)
{
    U8 state;
    U16 vbat;

    switch(bat_num)
    {
    case BAT_GPRS:

        break;
    case BAT_RTC:
        if(smp_ok_flg & (1 << BAT_RTC))  //AD����������Ч
        {
            smp_ok_flg &= ~(1 << BAT_RTC);     //��ʶ��λ
            vbat = Ad_Data[BAT_RTC];
            if(vbat < REF_V0_1)         //RTC��ص�ѹ < 0.2V Ϊ�޵��
            {
                Bat_Stat_Flag &= ~(0x0F << (BAT_RTC * 4));  //���λ������
                Bat_Stat_Flag |= (BAT_NO << (BAT_RTC * 4)); //����RTC���״̬���Ա�����������ѯ
            }
            else if(vbat < REF_V1_4)    //RTC��ص�ѹ < 2.8V + 0.7V(��ѹ��)ΪǷѹ
            {
                Bat_Stat_Flag &= ~(0x0F << (BAT_RTC * 4));  //���λ������
                Bat_Stat_Flag |= (BAT_LOW << (BAT_RTC * 4));//����RTC���״̬���Ա�����������ѯ
            }
            else                        //RTC��ص�ѹ >= 2.8V + 0.7V(��ѹ��)Ϊ����
            {
                Bat_Stat_Flag &= ~(0x0F << (BAT_RTC * 4));      //���λ������
                Bat_Stat_Flag |= (BAT_NORMAL << (BAT_RTC * 4)); //����RTC���״̬���Ա�����������ѯ
            }
        }
        break;
    case BAT_PDB:
        if(smp_ok_flg & (1 << BAT_PDB)) //AD����������Ч
        {
            smp_ok_flg &= ~(1 << BAT_PDB);      //��ʶ��λ
            vbat = Ad_Data[BAT_PDB];
            if(vbat < REF_V0_1)         //ͣ�Ե�ص�ѹ < 0.2V Ϊ�޵��
            {
                Bat_Stat_Flag &= ~(0x0F << (BAT_PDB * 4));  //���λ������
                Bat_Stat_Flag |= (BAT_NO << (BAT_PDB * 4)); //����ͣ�Ե��״̬���Ա�����������ѯ
            }
            else if(vbat < REF_V2_8)    //ͣ�Ե�ص�ѹ < 5.6V  ΪǷѹ
            {
                Bat_Stat_Flag &= ~(0x0F << (BAT_PDB * 4));  //���λ������
                Bat_Stat_Flag |= (BAT_LOW << (BAT_PDB * 4));//����ͣ�Ե��״̬���Ա�����������ѯ
            }
            else                        //ͣ�Ե�ص�ѹ >= 5.6V Ϊ����
            {
                Bat_Stat_Flag &= ~(0x0F << (BAT_PDB * 4));      //���λ������
                Bat_Stat_Flag |= (BAT_NORMAL << (BAT_PDB * 4)); //����ͣ�Ե��״̬���Ա�����������ѯ
            }
        }
        break;
    case EVE_TYP:
        if(smp_ok_flg & (1 << EVE_TYP)) //AD����������Ч
        {
            smp_ok_flg &= ~(1 << EVE_TYP);      //��ʶ��λ
            vbat = Ad_Data[EVE_TYP];
            state = get_gprs_type(vbat);
            Bat_Stat_Flag &= ~(0x0F << (EVE_TYP * 4));  //���λ������
            Bat_Stat_Flag |= (state << (EVE_TYP * 4));  //����GPRSģ���������Ա�����������ѯ
        }
        break;
    default:
        return 0xEE;    //��ڲ�������
    }

	state = (U8)((Bat_Stat_Flag >> (bat_num * 4)) & 0x0F);
    return state;
}

/******************************************************************************
** ��������: U8 get_gprs_type(U16 eveo_volt)
** ��������: ��ȡGPRSģ�����͹��ܺ���
** ��ڲ���: eveo_volt �¼������ѹֵ�������ж����ȶ���õ�GPRSģ������
** �� �� ֵ: GPRSģ������:��ad_ain.hö��GPRS_TYPE����,������Ч
** ˵    ��: ��ȡGPRSģ���ͺ���Ϣ,����������,Ŀǰ������һ�֣�����������
******************************************************************************/
U8 get_gprs_type(U16 eveo_volt)
{
    if((eveo_volt > EVEO_ONE_SIXTH - EVEO_LIMIT_VOLT) && (eveo_volt < EVEO_ONE_SIXTH + EVEO_LIMIT_VOLT))
    {
        return GPRS_MC52I;
    }
    else if((eveo_volt > EVEO_ONE_FIFTH - EVEO_LIMIT_VOLT) && (eveo_volt < EVEO_ONE_FIFTH + EVEO_LIMIT_VOLT))
    {
        return GPRS_MC53I;
    }
    else if((eveo_volt > EVEO_ONE_QUARTER - EVEO_LIMIT_VOLT) && (eveo_volt < EVEO_ONE_QUARTER + EVEO_LIMIT_VOLT))
    {
        return GPRS_MC54I;
    }
    else if((eveo_volt > EVEO_ONE_THIRD - EVEO_LIMIT_VOLT) && (eveo_volt < EVEO_ONE_THIRD + EVEO_LIMIT_VOLT))
    {
        return GPRS_MC55I;
    }
    else if((eveo_volt > EVEO_ONE_HALF - EVEO_LIMIT_VOLT) && (eveo_volt < EVEO_ONE_HALF + EVEO_LIMIT_VOLT))
    {
        return GPRS_MC56I;
    }
    else
    {
        return 0x0F;    //δ֪
    }
}
/******************************************************************************
    End Of File
******************************************************************************/


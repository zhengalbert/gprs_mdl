/******************************************************************************
 Copyright (C) 2011  R&D Institute of Reallin Co., Ltd.
 Module         : far infrared mamage function
 File Name      : far_infrared.c
 Description    : LPC17ϵ��оƬӲ��IO��ģ�⴮��Զ����ͨ��
 Author         :
 Build Date     :
 others         :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��     1.00       2011-07-30     rosoon     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

//-------------------------------- �������� -----------------------------------
#define CON_IR_PWM_FREQ     ((FPCLK_PWM1 / (PWM1PR + 1)) / 38000)
//------------------------------ ˽�б������� ---------------------------------

//------------------------------ ˽�к������� ---------------------------------


//------------------------------ ����ԭ�Ͷ��� ---------------------------------
/******************************************************************************
** ��������: void far_infrared_init(void)
** ��������: Զ����ͨ�ſڳ�ʼ�����ܺ���
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void far_infrared_init (void)
{
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//����CPU_GPRSģ��
//38k�������P1.26 ��Ҫ��!!!!

    PCONP |= ((1 << PCGPIO) | (1 << PCPWM1));   //ʹ��GPIOʱ����PWM1ʱ��
    PCLKSEL0 |= (FPCLK_PWM1_DIV << 12);         //��������PWM1��Ƶϵ��1/8

    PINMODE_OD1 &= ~P1_PWM_38K;                 //IRDA_TXD(p1.26):�ǿ�©

    PINSEL3 &= ~(0x03u << 20);                   //��������bits
    PINSEL3 |= (P1_26_FNUC << 20);               //PWM1[1]����ʱ���ó�GPIO,��������Խ�ʡ����

    PINMODE3 &= (0x03u << 20);                           //��������bits
    PINMODE3 = (P1_26_MODE << 20);                        //PIN_NO_PULL

//    SET_PORT1_OUT(P1_PWM_38K);                  //�����
//    SET_PORT1_LOW(P1_PWM_38K);                  //�����(�����ܺ�) 

    PWM1PR = 29;                                //Ԥ��Ƶ:������Ƶ�ʽ���30(PWM1PR+1)��Ƶ[384kHz]
    PWM1MR0 = ((FPCLK_PWM1 / (PWM1PR + 1)) / FAR_INFRARED_CARRIER); //PWM���Ƶ��38kHz

    PWM1MR6 = PWM1MR0 >> 1;                     //PWM���Ƶ��38kHz�İ�����ռ�ձ�
    PWM1MCR |=(1 << 1);             //PWMMR0/PWMMR1��PWMTC��ֵ��ƥ��ʱ�����жϣ��ҽ�PWMMR0ƥ��ʱPWMTC��λ
    PWM1CTCR = 0;                               //PWM�������ƼĴ�����λ
    PWM1PCR = (1 << 6) | (1 << 14);             //PWM���ƼĴ�����λ
    PWM1TCR = (1 << 3) + 1;                     //PWMģʽʹ��
    PWM1LER = 1 + (1 << 6);                                //ʹ��PWMƥ��0/1����
//
//    PWM1MR6 = PWM1MR0 >> 2;                     //PWM���Ƶ��38kHz��4��֮1����ռ�ձ�
//    PWM1MCR |= (3 << 0) | (1 << 3);             //PWMMR0/PWMMR1��PWMTC��ֵ��ƥ��ʱ�����жϣ��ҽ�PWMMR0ƥ��ʱPWMTC��λ
//    PWM1CTCR = 0;                               //PWM�������ƼĴ�����λ
//    PWM1PCR = 0;                                //PWM���ƼĴ�����λ
////    PWM1TCR = (1 << 0) | (1 << 3);              //PWM��ʱ�����ƼĴ���:PWM��ʱ����������Ԥ��Ƶ��������PWMģʽʹ��
////    PWM1TCR = (1 << 3);                         //PWMģʽʹ��
//    PWM1LER = 3;                                //ʹ��PWMƥ��0/1����
//    PWM1TCR = (1 << 1);                         //��λ
#else
    IR_VDD_ON();                                //ʹ�ܺ����Դ
//IO�ڳ�ʼ�������Ϳ�&���տ�(����IO���ж�ģʽ����)[ע����main.c�￪������жϴ���]
    PCONP |= ((1 << PCGPIO) | (1 << PCPWM1));   //ʹ��GPIOʱ����PWM1ʱ��
    PCLKSEL0 |= (FPCLK_PWM1_DIV << 12);         //��������PWM1��Ƶϵ��1/8

    PINMODE_OD1 &= ~P1_PWM_38K;                 //IRDA_TXD(p1.18):�ǿ�©
    PINMODE_OD2 &= ~P2_IRRXD;                   //IRDA_RXD(p2.11):�ǿ�©

    PINSEL3 &= ~(0x03u << 4);                   //��������bits
    PINSEL3 |= (P1_18_GPIO << 4);               //PWM1[1]����ʱ���ó�GPIO,��������Խ�ʡ����
    SET_PORT1_OUT(P1_PWM_38K);                  //�����
    SET_PORT1_LOW(P1_PWM_38K);                  //�����(�����ܺ�) 
    
//    if((PINSEL3 & (0x03u << 4))!= (P1_18_FNUC << 4))        //IRDA_TXD(p1.18):����ΪPWM��������
//    {
//        PINSEL3 &= ~(0x03u << 4);               //��������bits
//        PINSEL3 |= (P1_18_FNUC << 4);           //PWM1[1]
//        PINMODE3 &= (0x03u << 4);               //��������bits
//        PINMODE3 = (P1_18_MODE << 4);           //PIN_NO_PULL
//    }

    if((PINSEL4 & (0x03u << 22))!= (P2_11_FNUC << 22))      //IRDA_RXD(p2.11):����ΪGPIO��������
    {
        PINSEL4 &= ~(0x03u << 22);              //��������bits
        PINSEL4 |= (P2_11_FNUC << 22);          //GPIO
        PINMODE4 &= (0x03u << 22);              //��������bits
        PINMODE4 = (P2_11_MODE << 4);           //PIN_NO_PULL
    }
    SET_PORT2_IN(P2_IRRXD);                     //�����

//    IO2IntEnr |= P2_IRRXD;                    //P2_IRRXD(p2.11)���ó�����������IO�ж�
    IO2IntEnf |= P2_IRRXD;                      //P2_IRRXD(p2.11)���ó��½�������IO�ж�
    IO2IntClr = P2_IRRXD;                       //����жϱ�־

//--------------------------------��ʼ��PWM1[1]ģ�鹦��------------------------
    PWM1PR = 29;                                //Ԥ��Ƶ:������Ƶ�ʽ���30(PWM1PR+1)��Ƶ[384kHz]
    PWM1MR0 = ((FPCLK_PWM1 / (PWM1PR + 1)) / FAR_INFRARED_CARRIER); //PWM���Ƶ��38kHz
    PWM1MR1 = PWM1MR0 >> 2;                     //PWM���Ƶ��38kHz��4��֮1����ռ�ձ�
    PWM1MCR |= (3 << 0) | (1 << 3);             //PWMMR0/PWMMR1��PWMTC��ֵ��ƥ��ʱ�����жϣ��ҽ�PWMMR0ƥ��ʱPWMTC��λ
    PWM1CTCR = 0;                               //PWM�������ƼĴ�����λ
    PWM1PCR = 0;                                //PWM���ƼĴ�����λ
//    PWM1TCR = (1 << 0) | (1 << 3);              //PWM��ʱ�����ƼĴ���:PWM��ʱ����������Ԥ��Ƶ��������PWMģʽʹ��
//    PWM1TCR = (1 << 3);                         //PWMģʽʹ��
    PWM1LER = 3;                                //ʹ��PWMƥ��0/1����
    PWM1TCR = (1 << 1);                         //��λ
//--------------------------------��ʼ��FAR_INFRARED_STRUCT--------------------
    Ir_Ctl[IR_RX].Bit_Cnt = 0;
    Ir_Ctl[IR_RX].Bit_Sum = 0;
    Ir_Ctl[IR_RX].Dat_Len = 0;
    Ir_Ctl[IR_RX].Dat_Tmp = 0;
    Ir_Ctl[IR_RX].Dat_Tok = 0;          //���Ƴ�ֵ0
    Ir_Ctl[IR_RX].Dat_Buf[IR_LS] = 0;   //������Ϣ��ֵ0
    Ir_Ctl[IR_RX].Dat_Buf[IR_FI + 1] = 0;   //����֡���ֽ�(��ߴ���״̬��)����

    Ir_Ctl[IR_TX].Bit_Cnt = 11;
    Ir_Ctl[IR_TX].Bit_Sum = 0;
    Ir_Ctl[IR_TX].Dat_Len = 0;
    Ir_Ctl[IR_TX].Dat_Tmp = 0;
    Ir_Rxbyte_Timeout_Cnt[0] = Ir_Rxbyte_Timeout_Cnt[1] = 0;
    Ir_Occupy_State = IR_RX;            //Ĭ��Ϊ��������ռ��״̬
#endif  
}

/******************************************************************************
** ��������: void PWM1_IRQHandler(void)
** ��������: PWM1�жϷ����ӳ���
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: 1��MR0ƥ���ж�,����PWM1[1]��������
**           2��MR1ƥ���ж�,����PWM1[1]����ռ�ձ�
******************************************************************************/
void PWM1_IRQHandler (void)
{
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
	PWM1IR = 0x073F;        //��ȫ����ʶ
#else//H_METER_PRJ,WLS_CUR_PRJ
    U16 int_flg;

    int_flg = PWM1IR;
	  if(int_flg & (1 << MR0_BIT))                        //PWM1����(1/38kHz)�ж�
    {
        PWM1IR = (1 << MR0_BIT);                        //���жϱ�־
        PWM1MR0 = CON_IR_PWM_FREQ;                      //PWM���Ƶ��38kHz
    }
    if(int_flg & (1 << MR1_BIT))                        //��תλ��(ռ�ձ�)�ж�
    {
        PWM1IR = (1 << MR1_BIT);                        //���жϱ�־
        PWM1MR1 = CON_IR_PWM_FREQ >> 2;                 //PWM���Ƶ��38kHz��4��֮1����ռ�ձ�
    }
    else
    {
        PWM1IR = 0x073F;        //��ȫ����ʶ
    }
#endif
}  

#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//������ϴ�CPU_GPRSģ��
/******************************************************************************
** ��������: S8 get_far_infrared_data(U8 *buf)
** ��������: Զ����ͨ�ſ��ն��ֽ����ݹ��ܺ���
** ��ڲ���: buf �յ����ݻ���ָ��
** �� �� ֵ: 1 ��������֡
**          -1 ����Ч����
** ˵    ��: ���øú�����,��������Ч,��buf[0]Ϊ����֡����,buf[n:1]��Ч����֡
******************************************************************************/
S8 get_far_infrared_data (U8 *buf)
{
    U8 cnt;

    cnt = Ir_Ctl[IR_RX].Dat_Buf[IR_LS];                 //��ȡ������Ϣ

    Ir_Ctl[IR_RX].Dat_Tok += 1;                         //�����ۼ�
    if(Ir_Ctl[IR_RX].Dat_Tok == cnt)                    //����ͬ��ʱ
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
    else                        //����ʧ��ʱ
    {
        Ir_Ctl[IR_RX].Dat_Tok = cnt;                     //����ͬ��,Ϊ�´�׼��
        return -1;
    }
}

/******************************************************************************
** ��������: void send_far_infrared_data(U8 *buf, U8 cnt)
** ��������: Զ����ͨ�ſڷ����ֽ����ݹ��ܺ���
** ��ڲ���: buf ����������ָ��
**           cnt ���������ֽ���
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void send_far_infrared_data (U8 *buf, U8 cnt)
{
    U8 i;

    if((PINSEL3 & (0x03u << 4))!= (P1_18_FNUC << 4))        //IRDA_TXD(p1.18):����ΪPWM��������
    {
        PINSEL3 &= ~(0x03u << 4);               //��������bits
        PINSEL3 |= (P1_18_FNUC << 4);           //PWM1[1]
        PINMODE3 &= (0x03u << 4);               //��������bits
        PINMODE3 = (P1_18_MODE << 4);           //PIN_NO_PULL
    }

    PWM1TCR = ((1 << 0) | (1 << 3));				//PWM��ʱ�����ƼĴ���:PWM��ʱ����������Ԥ��Ƶ��������PWMģʽʹ��
    IO2IntEnf &= ~P2_IRRXD;                         //�з�������ʱ��ֹ�����жϱ����Է�����

    T1MR1 = T1TC + T1_MAT_FAR_INFRARED;             //����ƥ��ֵ
    T1MCR |= 1 << 3;                                //����MR1ƥ������ж�[��Զ���ⷢ��ͨ��λ��ʱ]

    T1MR3 = T1TC + FAR_INFRARED_TX_BYTE_TIMING;     //����ƥ��ֵ
    T1MCR |= 1 << 9;                                //����MR3ƥ������ж�[��Զ���ⷢ���ֽڼ����ʱ]

    Ir_Ctl[IR_TX].Dat_Len = cnt;
    i = 0;
    while(cnt)
    {
        Ir_Ctl[IR_TX].Dat_Buf[i++] = buf[--cnt];
    }
}

/******************************************************************************
** ��������: void far_infrared_rx_drv(void)
** ��������: Զ����ͨ�ſ��жϽ��ն��ֽ������������ܺ���
** ��ڲ���: ��
** �� �� ֵ: ��
**
** ˵    ��: ��TIMERx�Ĳ�����λ��ʱƥ���жϴ�����
******************************************************************************/
void far_infrared_rx_drv (void)
{
    if(Ir_Ctl[IR_RX].Bit_Cnt < 10)
    {
        Ir_Ctl[IR_RX].Mat_Val = T1TC + T1_MAT_FAR_INFRARED;
        T1MR0 = Ir_Ctl[IR_RX].Mat_Val;          //�´�ʱ�̼���
    }

    if(0 == Ir_Ctl[IR_RX].Bit_Cnt)              //��ʼλ��֤
    {   //SET_PORT1_REV(P1_DBG_LED);             //debug,�鿴����ʱ��
        SET_PORT2_IN(P2_IRRXD);                 //�����
        if(PORT2_IS_HIGH(P2_IRRXD))
        {
            Ir_Ctl[IR_RX].Dat_Tmp = 0xEE;       //��ʼλ����;
        }
        Ir_Ctl[IR_RX].Bit_Sum = 0;              //����
        Ir_Ctl[IR_RX].Bit_Cnt++;                //�Ӽ���
    }
    else if(Ir_Ctl[IR_RX].Bit_Cnt < 9)          //����λ����
    {   //SET_PORT1_REV(P1_DBG_LED);             //debug,�鿴����ʱ��
        SET_PORT2_IN(P2_IRRXD);                 //�����
        if(PORT2_IS_HIGH(P2_IRRXD))
        {
            Ir_Ctl[IR_RX].Dat_Tmp |= (1 << (Ir_Ctl[IR_RX].Bit_Cnt - 1));//��������λ
            Ir_Ctl[IR_RX].Bit_Sum++;            //У���
        }
        else
        {
            Ir_Ctl[IR_RX].Dat_Tmp &= ~(1 << Ir_Ctl[IR_RX].Bit_Cnt);//��������λ;
        }
        Ir_Ctl[IR_RX].Bit_Cnt++;                    //�Ӽ���
    }
    else if(9 == Ir_Ctl[IR_RX].Bit_Cnt)             //żУ����֤
    {   //SET_PORT1_REV(P1_DBG_LED);             //debug,�鿴����ʱ��

        SET_PORT2_IN(P2_IRRXD);                     //�����
        if((PORT2_IS_HIGH(P2_IRRXD) && (!(Ir_Ctl[IR_RX].Bit_Sum % 2)))
          || (!PORT2_IS_HIGH(P2_IRRXD) && (Ir_Ctl[IR_RX].Bit_Sum % 2)))
        {
            Ir_Ctl[IR_RX].Dat_Tmp = 0xED;           //ż�������;
        }
        Ir_Ctl[IR_RX].Bit_Cnt++;                    //�Ӽ���
    }
    else if(10 == Ir_Ctl[IR_RX].Bit_Cnt)            //��ֹͣλ��
    {
        //SET_PORT1_REV(P1_DBG_LED);               //debug,�鿴����ʱ��
        SET_PORT2_IN(P2_IRRXD);                     //�����
        if(!PORT2_IS_HIGH(P2_IRRXD))
        {
            Ir_Ctl[IR_RX].Dat_Tmp = 0xEB;           //ֹͣλ����;
        }
        else
        {
            Ir_Rxbyte_Timeout_Cnt[0]++;             //��ʱ�Ӽ���
            T1MR2 = T1TC + FAR_INFRARED_RX_BYTE_OVERTIME;  //�����ֽڽ��ճ�ʱƥ��ֵ
            T1MCR |= (1 << 6);                      //����MR2ƥ������ж�[���ֽڽ��ճ�ʱƥ�䶨ʱ]
        }
        Ir_Ctl[IR_RX].Bit_Cnt = 0;                  //����
        Ir_Ctl[IR_RX].Dat_Buf[++Ir_Ctl[IR_RX].Dat_Len] = Ir_Ctl[IR_RX].Dat_Tmp;//����Ir_Ctl[IR_RX].Dat_Buf[0]������Ir_Ctl[IR_RX].Dat_Len��ֵ
        Ir_Ctl[IR_RX].Dat_Tmp = 0;

        IO2IntClr = P2_IRRXD;                       //��������[����Ҫ]
        IO2IntEnf |= P2_IRRXD;                      //P2_IRRXD(p2.11)�ָ��½�������IO�ж�
        T1MCR &= ~(1 << 0);                         //��ֹMR0ƥ������ж�
    }
}
/******************************************************************************
** ��������: void far_infrared_tx_drv(void)
** ��������: Զ����ͨ�ſ��жϷ��Ͷ��ֽ������������ܺ���
** ��ڲ���: ��
** �� �� ֵ: ��
**
** ˵    ��: ��TIMERx�Ĳ�����λ��ʱƥ���жϴ�����
******************************************************************************/
void far_infrared_tx_drv (void)
{
    U8 i;

    if(Ir_Ctl[IR_TX].Dat_Len)                       //��������δ����
    {
        if(Ir_Ctl[IR_TX].Bit_Cnt == 11)             //ÿ����1���ֽ�ǰԤ����
        {
            Ir_Ctl[IR_TX].Dat_Tmp = (Ir_Ctl[IR_TX].Dat_Buf[Ir_Ctl[IR_TX].Dat_Len - 1] << 1);    //���ش�������&��ʼλ0

            Ir_Ctl[IR_TX].Bit_Sum = 0;
            for(i = 1; i < 9; i++)                  //��������λbit[9:1]У���
            {
                Ir_Ctl[IR_TX].Bit_Sum += ((Ir_Ctl[IR_TX].Dat_Tmp >> i) & 1);
            }
            if((Ir_Ctl[IR_TX].Bit_Sum % 2))
            {
                Ir_Ctl[IR_TX].Dat_Tmp |= (0x03 << 9);       //����żУ��λ&ֹͣλ
            }
            else
            {
                Ir_Ctl[IR_TX].Dat_Tmp |= (0x02 << 9);       //����żУ��λ&ֹͣλ
            }
        }
        if(Ir_Ctl[IR_TX].Bit_Cnt)                   //1���ֽ���δ����
        {
            if((Ir_Ctl[IR_TX].Dat_Tmp >> (11 - Ir_Ctl[IR_TX].Bit_Cnt)) & 1)   //��������λΪ1
            {
                PWM1PCR = 0;                        //PWM���ƼĴ���:����PWM1���
            }
            else
            {
                //���38kHZ��PWMԶ������Ʋ�
                PWM1PCR = (1 << 9);                 //PWM���ƼĴ���:ʹ��PWM1���
            }
            Ir_Ctl[IR_TX].Bit_Cnt--;                //λ���ݼ�����

            Ir_Ctl[IR_TX].Mat_Val = T1TC + T1_MAT_FAR_INFRARED;
            T1MR1 = Ir_Ctl[IR_TX].Mat_Val;          //�´�ʱ�̼���,Ϊ��һλ��׼��
        }
        else                                        //1���ֽڷ������
        {
            PWM1PCR = 0;                            //PWM���ƼĴ���:����PWM1���
            Ir_Ctl[IR_TX].Dat_Tmp = 0;              //��������
            Ir_Ctl[IR_TX].Bit_Cnt = 11;             //λ����������ԭ
            Ir_Ctl[IR_TX].Dat_Len--;                //������
        }
    }
}

/******************************************************************************
** ��������: void far_infrared_rx_timeout(void)
** ��������: Զ����ͨ�ſ��ֽڽ��ճ�ʱ�����ܺ���
** ��ڲ���: ��
** �� �� ֵ: ��
**
** ˵    ��: ��TIMERx���ֽڽ��ճ�ʱ��ʱƥ���жϴ�����
******************************************************************************/
void far_infrared_rx_timeout (void)
{
    U8 cnt,mem_type;
    U8 *mem_ptr;
    U32 mr_val;

    cnt = Ir_Rxbyte_Timeout_Cnt[0] + 256 - Ir_Rxbyte_Timeout_Cnt[1];    //��ʱ�ڼ���յ��ֽ���
    if(cnt >= 2)                //δ���ֳ�ʱ
    {
        mr_val = T1TC + FAR_INFRARED_RX_BYTE_OVERTIME;
        T1MR2 = mr_val;         //ˢ���ֽڽ��ճ�ʱƥ��ֵ
    }
    else                        //��ʱ������Ϊ��ǰ֡��������ȫ�����
    {
        T1MCR &= ~(1 << 6);                     //��ֹMR2ƥ������ж�
        Ir_Ctl[IR_RX].Dat_Buf[IR_FI] = Ir_Ctl[IR_RX].Dat_Len;//���浱ǰ���������ܳ���
        Ir_Ctl[IR_RX].Dat_Len = 0;              //��λ
        Ir_Ctl[IR_RX].Dat_Buf[IR_LS]++;         //������Ϣ
//���ͺ���������ݵ�Ĭ�Ͻ�������        
        if(Ir_Ctl[IR_RX].Dat_Buf[IR_FI] >= 5)
        {
            mem_ptr = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR28);
            if(mem_ptr != NULL)
            {
                mem_ptr[DATA_LEN_OFFSET] = Ir_Ctl[IR_RX].Dat_Buf[0];
                mem_ptr[DATA_LEN_OFFSET + 1] = 0;           //���ó��� ����Ϊ���ֽ� ���ֽ�����
                mem_ptr[COM_NUM_OFFSET] = IR_SOFT_CHL;      //���ô��ں�
                memcpy(mem_ptr+DATA_OFFSET, Ir_Ctl[IR_RX].Dat_Buf+1, Ir_Ctl[IR_RX].Dat_Buf[0]);
     //           mem_ptr = _alloc_box(Mem_256_Byte);//Ir_Ctl[IR_RX].Dat_Buf;
                isr_mbx_send(Mbx_Comm_Data, (void*)mem_ptr);
            }
            else
            {
            	setmemgetnullid(GET_MEM_ERR0, gusCurMemErrFunName);
            }	
        }            
        Ir_Ctl[IR_RX].Dat_Buf[IR_FI] =  0;//
    }
    Ir_Rxbyte_Timeout_Cnt[1] = Ir_Rxbyte_Timeout_Cnt[0];    //������ͬ��
}

/******************************************************************************
** ��������: void far_infrared_tx_timing(void)
** ��������: Զ����ͨ�ſ��ֽڷ��Ͷ�ʱ�����ܺ���
** ��ڲ���: ��
** �� �� ֵ: ��
**
** ˵    ��: ��TIMERx���ֽڽ��ճ�ʱ��ʱƥ���жϴ�����
******************************************************************************/
void far_infrared_tx_timing (void)
{
    if(Ir_Ctl[IR_TX].Dat_Len)                       //��������δ����
    {
        T1MR1 = T1TC + T1_MAT_FAR_INFRARED;         //����ƥ��ֵ
        T1MCR |= 1 << 3;                            //����MR1ƥ������ж�[��Զ���ⷢ��ͨ��λ��ʱ]

        T1MR3 = T1TC + FAR_INFRARED_TX_BYTE_TIMING; //����ƥ��ֵ
        T1MCR |= 1 << 9;                            //����MR3ƥ������ж�[��Զ���ⷢ���ֽڼ����ʱ]
    }
    else                                            //���ͻ������������
    {
        PWM1TCR = (1 << 1);                         //��λ(�ر�PWMģʽ������PWM��ʱ��)
        PWM1PCR = 0;                                //PWM���ƼĴ���:����PWM1���
        T1MCR &= ~(1 << 3);                         //��ֹMR1ƥ������ж�
        T1MCR &= ~(1 << 9);                         //��ֹMR3ƥ������ж�

        IO2IntClr = P2_IRRXD;                       //��������[����Ҫ]
        IO2IntEnf |= P2_IRRXD;                      //����������������ָ�P2_IRRXD(p2.11)�½�������IO�ж�
//----------------------�ָ�GPIOģʽ�������--------------------------------------------        
        PINSEL3 &= ~(0x03u << 4);                   //��������bits
        PINSEL3 |= (P1_18_GPIO << 4);               //PWM1[1]����ʱ���ó�GPIO,��������Խ�ʡ����
        SET_PORT1_OUT(P1_PWM_38K);                  //�����
        SET_PORT1_LOW(P1_PWM_38K);                  //�����(�����ܺ�)
    }
}
#endif

/******************************************************************************
    End Of File
******************************************************************************/

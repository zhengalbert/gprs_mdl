/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Run_Para_Export.h
 Description    : �������в��� ���ݽṹ����
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-08-05    azh     the original version
******************************************************************************/

#ifndef _RUN_PARA_EXPORT_H_
#define _RUN_PARA_EXPORT_H_

    //�ն���Ϣ
#ifdef ZIGBEE_TEST//azh 171011 �����ʽ��ѹ��GPRSģ��
    #define CON_TER_MANUFACTORNO_0  0x31                        //1700�������̱��  4�ֽ�
    #define CON_TER_MANUFACTORNO_1  0x37
    #define CON_TER_MANUFACTORNO_2  0x30
    #define CON_TER_MANUFACTORNO_3  0x30
    #define CON_TER_DEVNO_0 'M'
    #define CON_TER_DEVNO_1 'd'
    #define CON_TER_DEVNO_2 'l'
    #define CON_TER_DEVNO_3 '-'//
    #define CON_TER_DEVNO_4 '-'
    #define CON_TER_DEVNO_5 'H'
    #define CON_TER_DEVNO_6 'm'
    #define CON_TER_DEVNO_7 't'
#else
    #define CON_TER_MANUFACTORNO_0  0x31                        //1700�������̱��  4�ֽ�
    #define CON_TER_MANUFACTORNO_1  0x37
    #define CON_TER_MANUFACTORNO_2  0x30
    #define CON_TER_MANUFACTORNO_3  0x30
    #define CON_TER_DEVNO_0 'M'
    #define CON_TER_DEVNO_1 'd'
    #define CON_TER_DEVNO_2 'l'
    #define CON_TER_DEVNO_3 '-'//
    #define CON_TER_DEVNO_4 '-'
    #define CON_TER_DEVNO_5 'G'
    #define CON_TER_DEVNO_6 'm'
    #define CON_TER_DEVNO_7 'l'
#endif//ZIGBEE_TEST

//�������ݱ�ʶ
#define CON_PARA_BEG_CODE       0x1000  //������ʼ����
#define CON_PARA_END_CODE       0x14FF  //������ֹ����
#define CON_CTRL_BEG_CODE       0x1500  //������ʼ����
#define CON_CTRL_END_CODE       0x16FF  //������ֹ����
#define CON_CONFIG_BEG_CODE     0x1700  //�ն�������ʼ����
#define CON_CONFIG_END_CODE     0x17FF  //�ն�������ʼ����
#define CON_LOGIN_BEG_CODE      0x1800  //�����֤��ʼ����
#define CON_LOGIN_END_CODE      0x18FF  //�����֤��ʼ����

#define CON_PARA_F1_COMMPARA        CON_PARA_BEG_CODE+1     //F1���ն�ͨ�Ų�������
#define CON_PARA_F2_RELAYPARA       CON_PARA_BEG_CODE+2     //F2���ն��м�ת������
#define CON_PARA_F3_STAIPADDR       CON_PARA_BEG_CODE+3     //F3����վIP��ַ�Ͷ˿�
#define CON_PARA_F4_STAPHONENO      CON_PARA_BEG_CODE+4     //F4����վ�绰����Ͷ������ĺ���
#define CON_PARA_F5_TERENCRYPT      CON_PARA_BEG_CODE+5     //F5���ն���������
#define CON_PARA_F6_TERGROUPADDR    CON_PARA_BEG_CODE+6     //F6���ն����ַ����
#define CON_PARA_F7_IPADDR          CON_PARA_BEG_CODE+7     //F7���ն˳���������
#define CON_PARA_F8_ALRCTLWORD      CON_PARA_BEG_CODE+8     //F8���ն��¼���¼��������
#define CON_PARA_F9_CONFIGNUM       CON_PARA_BEG_CODE+9     //F9���ն�����������
#define CON_PARA_F10_METERPARA      CON_PARA_BEG_CODE+10    //F10���ն˵��ܱ�/��������װ�����ò���
#define CON_PARA_F11_PULSEPARA      CON_PARA_BEG_CODE+11    //F11���ն��������ò���
#define CON_PARA_F12_DINPUTPARA     CON_PARA_BEG_CODE+12    //F12���ն�״̬���������
#define CON_PARA_F13_ANALOGPARA     CON_PARA_BEG_CODE+13    //F13���ն˵�ѹ/����ģ�������ò���
#define CON_PARA_F14_SUMGPARA       CON_PARA_BEG_CODE+14    //F14���ն��ܼ������ò���
#define CON_PARA_F15_POWVALDIFF     CON_PARA_BEG_CODE+15    //F15���й��ܵ������Խ���¼���������
#define CON_PARA_F16_PDPINFO        CON_PARA_BEG_CODE+16    //F16������ר���û���������
#define CON_PARA_F17_KEEPPOWER      CON_PARA_BEG_CODE+17    //F17���ն˱�����ֵ
#define CON_PARA_F18_POWCTRLTIMEPER CON_PARA_BEG_CODE+18    //F18���ն˹���ʱ��
#define CON_PARA_F19_POWCTRLCOEF    CON_PARA_BEG_CODE+19    //F19���ն�ʱ�ι��ض�ֵ����ϵ��
#define CON_PARA_F20_ENERCTRLCOEF   CON_PARA_BEG_CODE+20    //F20���ն��µ������ض�ֵ����ϵ��
#define CON_PARA_F21_PERTIMEFEE     CON_PARA_BEG_CODE+21    //F21���ն˵���������ʱ�κͷ�����
#define CON_PARA_F22_FEEPARA        CON_PARA_BEG_CODE+22    //F22���ն˵���������
#define CON_PARA_F23_URGEALARMPARA  CON_PARA_BEG_CODE+23    //F23���ն˴߷Ѹ澯����
#define CON_PARA_F24_COPYINTER      CON_PARA_BEG_CODE+24    //F24���ն˳���������
#define CON_PARA_F25_MPBASEPARA     CON_PARA_BEG_CODE+25    //F25���������������
#define CON_PARA_F26_MPLIMIT        CON_PARA_BEG_CODE+26    //F26����������ֵ����
#define CON_PARA_F27_MPFREEZEPARA   CON_PARA_BEG_CODE+27    //F27�����������ݶ������
#define CON_PARA_F28_MPPOWRATEPARA  CON_PARA_BEG_CODE+28    //F28�������㹦�������ֶ���ֵ
#define CON_PARA_F29_METERADDR      CON_PARA_BEG_CODE+29    //F29���ն˵��ص��ܱ���ʾ��
#define CON_PARA_F30_METERCOPYFLAG  CON_PARA_BEG_CODE+30        //F30  �ն�Ͷ��/ͣ��
#define CON_PARA_F31_SLAVENODE      CON_PARA_BEG_CODE+31
#define CON_PARA_F33_SUMGFREEZEPARA CON_PARA_BEG_CODE+33    //F33���ܼ������ݶ������
#define CON_PARA_F34_PORTPROPERTY   CON_PARA_BEG_CODE+34    //F34
#define CON_PARA_F35_EMPHAFLAG      CON_PARA_BEG_CODE+35    //F35
#define CON_PARA_F37_LINKPARA       CON_PARA_BEG_CODE+37    //F37:����ͨ�Ų���
#define CON_PARA_F38_JOB1PARA       CON_PARA_BEG_CODE+38        //F38һ��������������
#define CON_PARA_F39_JOB2PARA       CON_PARA_BEG_CODE+39        //F39����������������
#define CON_PARA_F41_TIMEPERSETTING CON_PARA_BEG_CODE+41    //F41��ʱ�ι��ض�ֵ
#define CON_PARA_F42_RESTCTRL       CON_PARA_BEG_CODE+42    //F42�����ݹ��ز���
#define CON_PARA_F43_POWERSLIP      CON_PARA_BEG_CODE+43    //F43�����ʿ��ƵĹ��ʼ��㻬��ʱ��
#define CON_PARA_F44_SALESTOP       CON_PARA_BEG_CODE+44    //F44��Ӫҵ��ͣ�ز���
#define CON_PARA_F45_POWERCTRLTURN  CON_PARA_BEG_CODE+45    //F45�������ִ��趨
#define CON_PARA_F46_MONTHENERCTRL  CON_PARA_BEG_CODE+46    //F46���µ����ض�ֵ
#define CON_PARA_F47_BUYENERCTRL    CON_PARA_BEG_CODE+47    //F47���������ز���
#define CON_PARA_F48_ENERCTRLTURN   CON_PARA_BEG_CODE+48    //F48������ִ��趨
#define CON_PARA_F49_CTRLALRTIME    CON_PARA_BEG_CODE+49    //F49�����ظ澯ʱ��
#define CON_PARA_F57_SOUNDALARM     CON_PARA_BEG_CODE+57    //F57���ն������澯����M��ֹ����
#define CON_PARA_F58_KEEPAUTO       CON_PARA_BEG_CODE+58    //F58���ն��Զ��������
#define CON_PARA_F59_MTRERRORPARA   CON_PARA_BEG_CODE+59    //F59�����ܱ��쳣�б���ֵ�趨
#define CON_PARA_F60_HARMONPARA     CON_PARA_BEG_CODE+60    //F60��г����ֵ
#define CON_PARA_F61_DCAINPARA      CON_PARA_BEG_CODE+61    //F61��ֱ��ģ�����������
#define CON_PARA_F62_COMMTYPE       CON_PARA_BEG_CODE+62    //F62 ����ר��������ʽ
#define CON_PARA_F63_FLUXLIMIT      CON_PARA_BEG_CODE+36    //F63 ��ͨ��������������
#define CON_PARA_F65_UPDATA1PARA    CON_PARA_BEG_CODE+65    //F65����ʱ����1��������������
#define CON_PARA_F66_UPDATA2PARA    CON_PARA_BEG_CODE+66    //F66����ʱ����2��������������
#define CON_PARA_F67_UPDATA1CTRL    CON_PARA_BEG_CODE+67    //F67����ʱ����1��������������/ֹͣ����
#define CON_PARA_F68_UPDATA2CTRL    CON_PARA_BEG_CODE+68    //F68����ʱ����2��������������/ֹͣ����
#define CON_PARA_F73_CAPPARA        CON_PARA_BEG_CODE+73    //F73������������
#define CON_PARA_F74_CAPRUNPARA     CON_PARA_BEG_CODE+74    //F74��������Ͷ�����в���
#define CON_PARA_F75_CAPRELAYPARA   CON_PARA_BEG_CODE+75    //F75����������������
#define CON_PARA_F76_CAPCTRL        CON_PARA_BEG_CODE+76    //F76��������Ͷ�п��Ʒ�ʽ
#define CON_PARA_F81_DCARATE        CON_PARA_BEG_CODE+81    //F81��ֱ��ģ����������
#define CON_PARA_F82_DCALIMIT       CON_PARA_BEG_CODE+82    //F82��ֱ��ģ������ֵ
#define CON_PARA_F83_DCAFREESE      CON_PARA_BEG_CODE+83    //F83��ֱ��ģ�����������
#define CON_PARA_F89_ADDR           CON_PARA_BEG_CODE+89    //F89���ն˵�ַ����
//azh170916
#define CON_PARA_F90_WIRELESS       CON_PARA_BEG_CODE+90    //F90����������ͨ�Ų���
#define CON_PARA_F91_LOCATION       CON_PARA_BEG_CODE+91    //F91���ն˵���λ����Ϣ
#define CON_PARA_F150_SNSTATE       CON_PARA_BEG_CODE+150   //F150��������״̬

#define CON_PARA_F207_ADDR          CON_PARA_BEG_CODE+207   //F207���ն˵�ַ����

#define CON_CTRL_F1_YK_OPEN         CON_CTRL_BEG_CODE+1     //F1ң����բ
#define CON_CTRL_F2_YK_CLOSE        CON_CTRL_BEG_CODE+2     //F2ң�������բ
#define CON_CTRL_F9_TIMEPER_ON      CON_CTRL_BEG_CODE+9     //F9ʱ�ι���Ͷ��
#define CON_CTRL_F10_RESTCTRL_ON    CON_CTRL_BEG_CODE+10    //F10���ݹ���Ͷ��
#define CON_CTRL_F11_SALESTOP_ON    CON_CTRL_BEG_CODE+11    //F11Ӫҵ��ͣ����Ͷ��
#define CON_CTRL_F12_POWERLOWER_ON  CON_CTRL_BEG_CODE+12    //F12��ǰ�����¸���Ͷ��
#define CON_CTRL_F15_MONTHENER_ON   CON_CTRL_BEG_CODE+15    //F15�µ��Ͷ��
#define CON_CTRL_F16_BUYENER_ON     CON_CTRL_BEG_CODE+16    //F16�����Ͷ��
#define CON_CTRL_F17_TIMEPER_OFF    CON_CTRL_BEG_CODE+17    //F17ʱ�ι��ؽ��
#define CON_CTRL_F18_RESTCTRL_OFF   CON_CTRL_BEG_CODE+18    //F18���ݹ��ؽ��
#define CON_CTRL_F19_SALESTOP_OFF   CON_CTRL_BEG_CODE+19    //F19Ӫҵ��ͣ���ؽ��
#define CON_CTRL_F20_POWERLOWER_OFF CON_CTRL_BEG_CODE+20    //F20��ǰ�����¸��ؽ��
#define CON_CTRL_F23_MONTHENER_OFF  CON_CTRL_BEG_CODE+23    //F23�µ�ؽ��
#define CON_CTRL_F24_BUYENER_OFF    CON_CTRL_BEG_CODE+24    //F24����ؽ��
#define CON_CTRL_F25_KEEPPOWER_ON   CON_CTRL_BEG_CODE+25    //F25�ն˱���Ͷ��
#define CON_CTRL_F26_URGEALARM_ON   CON_CTRL_BEG_CODE+26    //F26�߷Ѹ澯Ͷ��
#define CON_CTRL_F27_TALK_ON        CON_CTRL_BEG_CODE+27    //F27�����ն�����վͨ��
#define CON_CTRL_F28_REMOVE_ON      CON_CTRL_BEG_CODE+28    //F28�ն��޳�Ͷ��
#define CON_CTRL_F29_UPACTIVE_ON    CON_CTRL_BEG_CODE+29    //F29�����ն������ϱ�
//azh 170920
#define CON_CTRL_F30_SETMETERTIME_SWITCH    CON_CTRL_BEG_CODE+30    //F30�ն˶Ե��ܱ��ʱ���ܵĿ�����ر�

#define CON_CTRL_F31_SETTIME        CON_CTRL_BEG_CODE+31    //F31��ʱ����
#define CON_CTRL_F32_MESSAGE        CON_CTRL_BEG_CODE+32    //F32������Ϣ
#define CON_CTRL_F33_KEEPPOWER_OFF  CON_CTRL_BEG_CODE+33    //F33�ն˱�����
#define CON_CTRL_F34_URGEFEE_OFF    CON_CTRL_BEG_CODE+34    //F34�߷Ѹ澯���
#define CON_CTRL_F35_TALK_OFF       CON_CTRL_BEG_CODE+35    //F35��ֹ�ն�����վͨ��
#define CON_CTRL_F36_REMOVE_OFF     CON_CTRL_BEG_CODE+36    //F36�ն��޳����
#define CON_CTRL_F37_UPACTIVE_OFF   CON_CTRL_BEG_CODE+37    //F37��ֹ�ն������ϱ�
#define CON_CTRL_F38_SMS_ACTIVE     CON_CTRL_BEG_CODE+38    //F38���ż���
#define CON_CTRL_F39_DISCONECT      CON_CTRL_BEG_CODE+39    //F39�Ͽ�����
#define CON_CTRL_F41_CAPCTRL_ON     CON_CTRL_BEG_CODE+41    //F41���ݿ���Ͷ��
#define CON_CTRL_F42_CAPCTRL_OFF    CON_CTRL_BEG_CODE+42    //F42���ݿ��ƽ��

#define CON_CTRL_F49                CON_CTRL_BEG_CODE+49
#define CON_CTRL_F50                CON_CTRL_BEG_CODE+50
#define CON_CTRL_F51                CON_CTRL_BEG_CODE+51
//#define CON_CTRL_F88_TIMEREVISE       CON_PARA_BEG_CODE+88    //F88����ȷ��ʱ����//ɽ����Լ��
//azh 170920
#define CON_CTRL_F149_METERSELF_SWITCH       CON_CTRL_BEG_CODE+149    //F149���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
#define CON_CTRL_F150_RESETSNSTATE_SWITCH    CON_CTRL_BEG_CODE+150    //F150��λ��������������־

#define CON_CONFIG_F1_VERSION       CON_CONFIG_BEG_CODE+1   //F1�����ն˰汾��Ϣ
#define CON_CONFIG_F2_INTERFACE     CON_CONFIG_BEG_CODE+2   //F2�����ն����롢�����ͨ�Ŷ˿�����
#define CON_CONFIG_F3_OTHER         CON_CONFIG_BEG_CODE+3   //F3�����ն���������
#define CON_CONFIG_F4_PARA          CON_CONFIG_BEG_CODE+4   //F4�����ն˲������ã����ն���Ӳ���汾֧�ֵĲ�����
#define CON_CONFIG_F5_CTRL          CON_CONFIG_BEG_CODE+5   //F5�����ն˿������ã����ն���Ӳ���汾֧�ֵĿ������
#define CON_CONFIG_F6_DATA1         CON_CONFIG_BEG_CODE+6   //F6�����ն�1���������ã����ն���Ӳ���汾֧�ֵ�1�����ݣ�
#define CON_CONFIG_F7_DATA2         CON_CONFIG_BEG_CODE+7   //F7�����ն�2���������ã����ն���Ӳ���汾֧�ֵ�2�����ݣ�
#define CON_CONFIG_F8_ERC           CON_CONFIG_BEG_CODE+8   //F8�����ն��¼���¼���ã����ն���Ӳ���汾֧�ֵ��¼���¼��
//azh 170920
#define CON_CONFIG_F9_REMOTEVER     CON_CONFIG_BEG_CODE+9   //F9Զ��ͨ��ģ��汾��Ϣ
#define CON_CONFIG_F10_LOCALVER     CON_CONFIG_BEG_CODE+10  //F10����ͨ��ģ��汾��Ϣ

#define CON_LOGIN_F1_REQUEST        CON_LOGIN_BEG_CODE+1    //F1�����֤����
#define CON_LOGIN_F2_ANSWER         CON_LOGIN_BEG_CODE+2    //F2�����֤��Ӧ


/*
���ն˵����ݷ�Ϊ���ࣺ
1. �ն��ڲ������ݣ���Щ����������ͨ�Ź�Լ�޹أ�������901F��
2. ��Լ����Ҫ�����ݣ���Щ����һ����1�����ݹ���
*/
/******************************************���������****************************************/
#define CON_MAX_MPNUM               2//15
#define CON_METER_NUM               1//8
#define CON_PULSE_NUM               0//8
#define CON_SWITCHIN_NUM            0//8        //����������������1·�Žڵ㣬4·ң����
#define CON_SWITCHOUT_NUM           0//4        //��������
#define CON_PARA_SWITCHOUT_NUM      0/4     //���������������ã�
#define CON_ANALOG_NUM              0//8

#define     CON_PORT_NUM                    2
#define     CON_COMMPORT_NUM            2

#define CON_RATEVOL_3P4L1           220
#define CON_RATEVOL_3P3L            100
#define CON_RATEVOL_3P4L2           57

#define CON_MPTYPE_NULL             0x00    //  ����������Ϊ������
#define CON_MPTYPE_TER              0x01    //  ����������Ϊ�ն�
#define CON_MPTYPE_METER            0x02    //  ����������Ϊ���
#define CON_MPTYPE_PULSE            0x03    //  ����������Ϊ������

#define CON_MPTYPE_SUMG             0x04    //  �ܼ�

#define CON_MPNO_INVALID            0xFF    // ��ЧMPNo
#define CON_MPNO_TER_FIXED          0xFE    //  �ն˹̶�
#define CON_MPNO_TER                0xFE//0//8 //  �ն�
//#define   CON_MPNO_SUMG_FIXED         63      //  �ܼ�����Ĳ����㣬�̶�
#define CON_MPNO_SUMG_FIXED         40      //  �ܼ�����Ĳ����㣬�̶�
#define CON_METER_BASE_MPNO         1       //  ��Ʋ�������ʼ���
#define CON_PULSE_BASE_MPNO         32      //  �����������ʼ���

#define CON_MAX_MTRFREEZENUM        75      //  ���Ķ�����
#define CON_MAX_SUMGFREEZENUM       10      //  �����ܼ��鶳����

#define CON_TIMEINTER_NOT           0x00    //  ������
#define CON_TIMEINTER_15MIN         0x01    //  15����
#define CON_TIMEINTER_30MIN         0x02    //  30����
#define CON_TIMEINTER_60MIN         0x03    //  60����
#define CON_TIMEINTER_INVALID       0x0F    //  ��Ч

#define CON_MAX_FREEZEDAYNUM            31
#define CON_MAX_DIRPOWTOTAL_COMPNUM     8   //����С���ܼ�����

//ģ�����
#define CON_MAX_GPRSPDPUSERNAME_LEN     32//16      changed for test 09-11-25
#define CON_MAX_GPRAPDPPASSWORE_LEN     32//16      changed for test 09-11-25

//AES�������
#define CON_MAX_AESCIPHER_LEN           3

#define CON_NEEDZIP                     0x00
#define CON_NOTCIPHERZIP                0x01
#define CON_NEEDCIPHER                  0x02
#define CON_NEEDCIPHERZIP               0x03
typedef struct _STRMWORKZONE    //����ʱ��
{
    unsigned char usRMTStart[2];//ʱ��
    unsigned char usRMTEnd[2];//ʱ��
}STRMWORKZONE;

typedef struct _STCOMPORTPARA   //ͨ�Ŷ˿ں�
{
    unsigned short usRMRunCtrlWord;//�������п�����
    unsigned long  ulRMDate;//������-����
    unsigned char  ucRMTime[2];//������-ʱ��
    unsigned char  ucRMInval;//������ʱ��
    unsigned char  ucRMBcastTInval[3];//�Ե��㲥Уʱ��ʱʱ�� ��ʱ��
    unsigned char  ucRMWorkZoneNum;
    STRMWORKZONE stRMWorkZone[24];
}STCOMPORTPARA, *PSTCOMPORTPARA;
typedef struct _STRMRUNPARA //F33 �ն˳������в�������
{
    unsigned char  ucComPortEBit;//���ڱ�־λ
    STCOMPORTPARA stComPortPara[CON_PORT_NUM];
}STRMRUNPARA, *PSTRMRUNPARA;

typedef struct _STPORTPROP      //�˿�����
{
    unsigned char ucBaudRate;
    unsigned char ucParity;
    unsigned char ucByteSize;
    unsigned char ucStopBits;
    unsigned long ulBaudRate;
}STPORTPROP;
typedef struct _STPORTLIST      //�˿��б�
{
    unsigned char ucNo[CON_COMMPORT_NUM];
    STPORTPROP stPortProp[CON_COMMPORT_NUM];
    STRMRUNPARA stRMRunPara;
}STPORTLIST;

typedef struct _STSTAT32BYTE
{
    unsigned char ucState[32];
}STSTAT32BYTE;

typedef struct _STMPRATEVAL
{
    unsigned char   ucMPLineType;           //���߷�ʽ
    unsigned char   ucMPPhaseNo;
    unsigned short  usRateVol;              //���ѹ  //0.1  V
    unsigned short  usRateCur;              //�����  //0.01 A
    unsigned long   ulRateP;                //�����  //0.0001    KVA
}STMPRATEVAL;

typedef struct _STMPALTERRATE
{
    unsigned short  usVolAlterRate;         //��ѹ��·���
    unsigned short  usCurAlterRate;         //������·���
}STMPALTERRATE;

typedef struct _STMPLIMIT                   //��������ֵ
{
    unsigned short  usVolUpLimit;           //��ѹ�ϸ�����  ������ʽXXX
    unsigned short  usVolDownLimit;         //��ѹ�ϸ�����  ������ʽXXX
    unsigned short  usVolBreakLimit;        //��������      ������ʽXXX
    unsigned short  usVolErrUpLimit;        //��ѹ�쳣����  ������ʽXXX
    unsigned short  usVolErrDownLimit;      //��ѹ�쳣����  ������ʽXXX

    unsigned short  usVolUpLimit_2;         //��ѹ�ϸ�����  С����ʽXXX.X
    unsigned short  usVolDownLimit_2;       //��ѹ�ϸ�����  С����ʽXXX.X
    unsigned short  usVolBreakLimit_2;      //��������      С����ʽXXX.X
    unsigned short  usVolErrUpLimit_2;      //��ѹ�쳣����  С����ʽXXX.X
    unsigned short  usVolErrDownLimit_2;    //��ѹ�쳣����  С����ʽXXX.X

    unsigned short  usCurErrUpLimit;        //�����쳣����  С����ʽXX.XX
    unsigned short  usCurUpLimit;           //�����ϸ�����  С����ʽXX.XX
    unsigned short  usCur0UpLimit;          //���ߵ�������  С����ʽXX.XX
    unsigned long   ulPowErrUpLimit;        //�����쳣����                  .XXXX
    unsigned long   ulPowUpLimit;           //���ʺϸ�����                  .XXXX
    unsigned short  usVolUnBalance;         //��ѹ��ƽ����ֵ            .X
    unsigned short  usCurUnBalance;         //������ƽ����ֵ            .X
    unsigned short  usLostVolTime;          //����ʧѹʱ��

    unsigned char   ucVolErrUpTime;         //Խ�޳���ʱ��
    unsigned char   ucVolErrDownTime;       //Խ�޳���ʱ��
    unsigned char   ucCurErrUpTime;         //Խ�޳���ʱ��
    unsigned char   ucCurUpTime;            //Խ�޳���ʱ��
    unsigned char   ucCur0UpTime;           //Խ�޳���ʱ��
    unsigned char   ucPowErrUpTime;         //Խ�޳���ʱ��
    unsigned char   ucPowUpTime;            //Խ�޳���ʱ��
    unsigned char   ucVolUnBalanceTime;     //Խ�޳���ʱ��
    unsigned char   ucCurUnBalanceTime;     //Խ�޳���ʱ��

    unsigned short  ucVolErrUpBack;         //Խ�޳���ʱ��
    unsigned short  ucVolErrDownBack;       //Խ�޳���ʱ��
    unsigned short  ucCurErrUpBack;         //Խ�޳���ʱ��
    unsigned short  ucCurUpBack;            //Խ�޳���ʱ��
    unsigned short  ucCur0UpBack;           //Խ�޳���ʱ��
    unsigned short  ucPowErrUpBack;         //Խ�޳���ʱ��
    unsigned short  ucPowUpBack;            //Խ�޳���ʱ��
    unsigned short  ucVolUnBalanceBack;     //Խ�޳���ʱ��
    unsigned short  ucCurUnBalanceBack;     //Խ�޳���ʱ��
}STMPLIMIT;

typedef struct _STMPFREEZEPARA
{
    unsigned char   ucItem;                 //1�����ݱ�ʶ��F89--F120
    unsigned char   ucFreezeInter;
}STMPFREEZEPARA;

typedef struct  _STFREEZEDAY
{
    STDATETIME  stFreezeDay[CON_MAX_FREEZEDAYNUM];
}STFREEZEDAY, *PSTFREEZEDAY;

typedef struct  _STPOWRATEPARA
{
    unsigned short  usPowRateLimit1;        //���������ֶ���ֵ1
    unsigned short  usPowRateLimit2;        //���������ֶ���ֵ2
}STPOWRATEPARA;

typedef struct _STCUFELOSTPARA
{
    unsigned short R;           //����
    unsigned short X;           //�翹
    unsigned short G;           //�絼
    unsigned short B;           //����
}STCUFELOSTPARA;
typedef struct  _STTONGSUNTIANSUNPARA
{
    STCUFELOSTPARA  stCuFeLostParaA;
    STCUFELOSTPARA  stCuFeLostParaB;
    STCUFELOSTPARA  stCuFeLostParaC;
}STTONGSUNTIANSUNPARA;

typedef struct _STMPINFO
{
    unsigned char   ucMPType;               //1
    unsigned char   ucDevNo[4];             //�������豸���(��ţ������)��һ����������������4·��ͬ���Ե����幹��
    STMPRATEVAL     stMPRateVal;            //3
    STMPALTERRATE   stMPAlterRate;          //4
    STMPLIMIT       stMPLimit;              //30
    STMPFREEZEPARA  stMPFreezePara[CON_MAX_MTRFREEZENUM];   //150
    STFREEZEDAY     stFreezeDay;            //186
    STPOWRATEPARA   stPowRatePara;          //4
    STTONGSUNTIANSUNPARA    stTongsunTiansun;//24
}STMPINFO;

/******************************************�ն����ø���****************************************/
typedef struct  _STCONFIGNUM
{
    unsigned char   ucMeterNum;         //���ܱ�/��������װ�������ܿ���
    unsigned char   ucPulseNum;         //��������·��
    unsigned char   ucAnalogNum;        //��ѹ/����ģ����������·��
    unsigned char   ucSumGNum;          //�ܼ���������
}STCONFIGNUM;
/******************************************�������ò���****************************************/
#define CON_PULSEATTR_INVALID   0x00    //��Ч
#define CON_PULSEATTR_DIRPOW    0x01    //�����й�
#define CON_PULSEATTR_REVPOW    0x02    //�����й�
#define CON_PULSEATTR_DIRVAR    0x03    //�����޹�
#define CON_PULSEATTR_REVVAR    0x04    //�����޹�

typedef struct  _STPULSEPARA
{
    unsigned char   ucAttr;             //����
    unsigned char   ucPortNo;           //�˿ں�
    unsigned short  usConst;            //���峣��
}STPULSEPARA;
/******************************************״̬�����ò���****************************************/
//0~3:��Ӧ�ڼ̵������ĸ�ң�����룬7���Žڵ�
#define CON_SWITCHATTR_INVALID  0x00    //��Ч
#define CON_SWITCHATTR_TYPEA    0x01    //��A�ഥ�� һ��
#define CON_SWITCHATTR_TYPEB    0x02    //��B�ഥ�� �෴
#define CON_SWITCHALARM_IMP     0x55    //��Ҫ
#define CON_SWITCHALARM_NOR     0xAA    //һ��

typedef struct _STSWITCHPARA
{
    unsigned char   ucAttr;         //���ԣ���Ч��ʾ������
    unsigned char   ucAlarm;        //��λ�澯����Ҫ�¼�����һ���¼�
}STSWITCHPARA;
typedef struct _STDT2
{
    double dVal;
    unsigned short usDT2;
}STDT2;
typedef struct _STANALOGRATE
{
    STDT2   stStart;
    STDT2   stEnd;
}STANALOGRATE;
typedef struct _STANALOGLIMIT
{
    STDT2 stUpLimit;
    STDT2   stDownLimit;
}STANALOGLIMIT;
/******************************************ģ�������ò���****************************************/
#define CON_ANALOGTYPE_VA       0x00
#define CON_ANALOGTYPE_VB       0x01
#define CON_ANALOGTYPE_VC       0x02
#define CON_ANALOGTYPE_IA       0x03
#define CON_ANALOGTYPE_IB       0x04
#define CON_ANALOGTYPE_IC       0x05

typedef struct _STANALOGPARA
{
    unsigned char ucAvailable;
    unsigned char   ucMPNo;         //�������
    unsigned char   ucAttr;         //���ԣ�����
    unsigned char ucM;
    STANALOGRATE stAnalogRate;
    STANALOGLIMIT stAnalogLimit;
}STANALOGPARA;

//������Ϣ
#define     CON_MESSAGE_DATALEN             16//200             //������Ϣ�������ֵ 200
#define     CON_MESSAGE_LIFETIME            600             //5����
#define     CON_MESSAGE_NUM                 5               //������Ϣ������ɽ������������������ź��岻ͬ
#define     CON_MESSAGE_TYPE_IMPT           1               //��Ҫ��Ϣ
#define     CON_MESSAGE_TYPE_NORM           0               //������Ϣ
typedef struct _STSHORTMESSAGE
{
    unsigned char   ucAvailble;         //��Ч��־
    unsigned char   ucType;             //����
    unsigned char   ucNo;               //���
    unsigned char   ucLength;           //����
    unsigned char   ucPos;              //��ǰλ��
    unsigned char   ucMsgStatusFlag;    //��Ϣ״̬��־
    unsigned short  usLifeTime;         //��Ч�ڣ���λ0.5��
    STDATETIME      stDateTimeTemp;     //������Ϣ����ʱ��
    unsigned char   ucMsg[CON_MESSAGE_DATALEN]; //������Ϣ
}STSHORTMESSAGE;
/******************************************���ܱ����****************************************/
//�ⲿ����ͨ�Ź�Լ���ͣ���ſ��Ը����ⲿҪ����ģ���¼��DATAFLASH CON_DFPAGE_HEAD_RESERVE1λ�ÿ�ʼ80�ֽ�
#define     CON_EXT_PROTOCOL_INVALID    0       //��Ч
#define     CON_EXT_PROTOCOL_DL645      1       //645��Լ
#define     CON_EXT_PROTOCOL_SAMPLE     2       //�����ɼ�

#define     CON_EXT_PROTOCOL_WS         3       //��ʤv4.x
#define     CON_EXT_PROTOCOL_LANDD      4       //������D��
#define     CON_EXT_PROTOCOL_ABB_R      5       //ABB����
//#define       CON_EXT_PROTOCOL_GENIUS     6       //����
#define     CON_EXT_PROTOCOL_AS1_DL645  6       //645��Լ����һ
#define     CON_EXT_PROTOCOL_HND        7       //������
#define     CON_EXT_PROTOCOL_LANDB      8       //������B��

#define     CON_EXT_PROTOCOL_AS2_DL645  15      //645��Լ���ƶ�(���棨���׶๦�ܣ�)
#define     CON_EXT_PROTOCOL_HB_PT      17      //����֮��ͨ����ʽ���ܱ�
#define     CON_EXT_PROTOCOL_HB_JM      18      //����֮���񸴷��ʵ��ܱ�

#define     CON_EXT_PROTOCOL_KL         9       //��½ (645)
#define     CON_EXT_PROTOCOL_LD         11      //���� (645)
#define     CON_EXT_PROTOCOL_SKD        12      //��ƴ� (645)
#define     CON_EXT_PROTOCOL_JSLY       13      //�������� (645)
#define     CON_EXT_PROTOCOL_HND645     14      //������ (645)
#define     CON_EXT_PROTOCOL_DFDZ       15      //�������� (645)
#define     CON_EXT_PROTOCOL_JQ         16      //��ȸ (645)
#define     CON_EXT_PROTOCOL_HLi        17      //�㽭���� (645)

//#define       CON_EXT_PROTOCOL_AS1_DL645  18      //645��Լ����һ
//#define       CON_EXT_PROTOCOL_AS2_DL645  19      //645��Լ���ƶ�

#define     CON_EXT_PROTOCOL_DL64507    30


#define CON_METERTYPE_3P3L      0x01
#define CON_METERTYPE_3P4L      0x02
#define CON_METERTYPE_1P        0x03

#define     CON_METER_PHASE_INCERTERN   0
#define     CON_METER_PHASEA                1
#define     CON_METER_PHASEB                2
#define     CON_METER_PHASEC                3

#define CON_METER_ADDRLEN       6       //����ַ����
#define CON_METER_PASSLEN       8       //���볤��
#define CON_METER_NAMELEN       8       //ͨѸ������
#define CON_COLLECT_ADDR_LEN    6
typedef struct _STMETERADDR
{
    unsigned char ucMeterAddr[CON_METER_ADDRLEN];
}STMETERADDR;
typedef struct _STMETERPASS
{
    unsigned char ucMeterPass[CON_METER_PASSLEN];
}STMETERPASS;
typedef struct _STMETERCOMMNAME
{
    unsigned char ucMeterCommName[CON_METER_NAMELEN];
}STMETERCOMMNAME;
typedef struct _STMETERPARA
{
    unsigned char ucProtocol;           //��Լ����
    unsigned char ucPort;               //��1��ʼ��1��2�����0��Ϊ1
    unsigned char ucBaudCode;
    unsigned long ulBaudRate;           //������
    unsigned char ucEPN;                //У��λ
    unsigned char ucDataBit;            //����λ
    unsigned char ucStopBit;            //ֹͣλ
    unsigned char ucSetFeeNum;
    unsigned char ucFeeNum;             //���ʸ���
    unsigned char ucIntNum;             //����λ����
    unsigned char ucFloatNum;           //С��λ����
//  unsigned char ucPulseNo[CON_PULSE_NUM]; //��Ӧ��������
    STMETERADDR   stMeterAddr;          //����ַ
    STMETERPASS   stPassword;           //ͨ������
    STMETERCOMMNAME stCommName;         //ͨѸ��
    unsigned char ucMeterJuNum[12];     //���ֺţ�12�ֽڵ�ASCII��
    unsigned char ucCollectAddr[CON_COLLECT_ADDR_LEN];  //�ɼ�����ַ
    unsigned char ucBigClass;
    unsigned char ucLittleClass;
    unsigned char ucEmpFlag;
}STMETERPARA;

/******************************************�ܼ������****************************************/
#define CON_SUMGROUP_NUM        8
#define CON_SUMGOPER_ADD        0x55
#define CON_SUMGOPER_SUB        0xAA

typedef struct _STSUMGFREEZEPARA
{
    unsigned char   ucFreezeInter;
}STSUMGFREEZEPARA;
typedef struct _STSUMVALOPER
{
    unsigned char       ucMPSum[CON_MAX_MPNUM];     // �������Ĳ�����
    unsigned char       ucMPOper[CON_MAX_MPNUM];    // �����
}STSUMVALOPER;
typedef struct _STSUMGLIMIT
{
    unsigned long       ulSumGUpLimit;
    unsigned long       ulSumGDownLimit;
}STSUMGLIMIT;
typedef struct _STSUMVALPARAPER
{
    STSUMVALOPER        stDir;
    STSUMVALOPER        stRev;
    STSUMGFREEZEPARA    stFreeze;           // �ܼ��鶳�����
    STSUMGLIMIT         stLimit;
}STSUMVALPARAPER;
typedef struct _STSUMGROUPPARA
{
    unsigned char ucAvailbleFlag;
    STSUMVALPARAPER stInstantPow;
    STSUMVALPARAPER stPowVal;
    STSUMVALPARAPER stInstantVar;
    STSUMVALPARAPER stVarVal;
}STSUMGROUPPARA;

#define CON_SUMGTYPE_INSTANTPOW_DIR     0x01
#define CON_SUMGTYPE_INSTANTPOW_REV     0x02
#define CON_SUMGTYPE_POWVAL_DIR         0x03
#define CON_SUMGTYPE_POWVAL_REV         0x04
#define CON_SUMGTYPE_INSTANTVAR_DIR     0x05
#define CON_SUMGTYPE_INSTANTVAR_REV     0x06
#define CON_SUMGTYPE_VARVAL_DIR         0x07
#define CON_SUMGTYPE_VARVAL_REV         0x08
/******************************************ͨ�Ų���****************************************/
#define CON_SERVER_NULL                 0x00    //���¼�
#define CON_SERVER_NUM                  8

#define CON_ONLINE_MASK                 0xb3    //ģʽ������
#define CON_ONLINE_TCP                  0       //ͨ�ŷ�ʽ
#define CON_ONLINE_UDP                  0x80

#define CON_ONLINE_TYPE_MASK            0X30    //������ʽ������
#define CON_ONLINE_MIX                  0       //������ʽ
#define CON_ONLINE_CLIENT               0X10
#define CON_ONLINE_SERVER               0X20

#define CON_ONLINE_MODE_MASK            0X03    //ģʽ������
#define CON_ONLINE_ALLWAYS              1       //��������
#define CON_ONLINE_SMS_UP               2       //���ż����������
#define CON_ONLINE_TIME_UP              3       //ʱ������ģʽ
#define CON_ONLINE_INVALID              0       //��Ч�Ĺ���ģʽ


#define CON_ONLINE_SMS_ACTIVE           1       //���ż���
//#define CON_ONLINE_TER_ACTIVE         2       //�ն���������

typedef struct  _STTERCOMMPARA
{
    unsigned char   ucDelay;                    //�ն���������ʱʱ�䣨��λ��20ms��
    unsigned char   ucMaxOvertimeOrig;          //ԭ��վ�������ʱʱ�䣨���ӣ�
    unsigned char   ucReSendtimes;              //�ط�����
    unsigned short  usACKOvertime;              //ȷ�ϣ�CON=1����ʱ����λ��s��
    unsigned char   ucServerNeedAck[CON_SERVER_NUM];//0,1�¼��Ƿ���Ҫȷ�ϣ�2:һ������3:��������
                                                //��Ҫ��վȷ�ϵķ���8��ͨ�ŷ���
}STTERCOMMPARA;

typedef struct  _STTERCOMMPARA_INNER
{
    unsigned char   ucRCommMode;                //ͨ��ģ�鹤��ģʽ
    unsigned short  usReConnectInter;           //�������ߵ��������������λ��
    unsigned char   ucReConnectTimes;           //���������ز�����
    unsigned char   ucStandbyTime;              //������������߳���ʱ�䣬��λ����
    unsigned char   ucTimeInt[3];               //ʱ������ģʽ��ÿ��BIT��ʾһ��Сʱ�㣬Ϊ1��ʾ�������ߣ�Ϊ0��ʾ��������
}STTERCOMMPARA_INNER;

typedef struct _STHEARTBEAT                     //����
{
    unsigned char   ucHeartBeat;
}STHEARTBEAT;

//��վIP��ַ�Ͷ˿�
#define CON_MAXAPNLEN                   20
typedef struct _STIPADDR
{
    unsigned char  ucIPAddr[4];         //IP��ַ1-4��
    unsigned short usIPPort;            //IP��ַ�˿ں�
}STIPADDR;
#define CON_USRNAME_LEN     20
#define CON_PASSWORD_LEN        20
typedef struct _STAGENTSERVER
{
    unsigned char ucServeType;      //��������
    unsigned char ucLinkType;           //���ӷ�ʽ
    unsigned char ucUsrNameLen;
    unsigned char ucUsrName[CON_USRNAME_LEN];
    unsigned char ucPwdLen;
    unsigned char ucPwd[CON_PASSWORD_LEN];
    STIPADDR stServeIP;             //IP���˿�
}STAGENTSERVER;
typedef struct  _STSTAIPADDR
{
    STIPADDR    stBaseIP;               //����IP��ַ
    STIPADDR    stBackIP;               //����IP��ַ
    char        cAPN[CON_MAXAPNLEN];    //APN
}STSTAIPADDR;

typedef struct _STLOCALIP
{
    STIPADDR    stLocalIP;
    STIPADDR    stSubMask;
    STIPADDR    stGateWay;              //����
    STAGENTSERVER   stAgentServer;          //���������
}STLOCALIP;
//��վ�绰���롢�������ĺ���
#define  CON_PHONENOLEN                 8
typedef struct _STPHONENO
{
    unsigned char   ucPhoneNo[CON_PHONENOLEN];
}STPHONENO;
typedef struct  _STSTAPHONENO
{
    STPHONENO   stPhoneNo;          //��վ�绰����
    STPHONENO   stSMSCenter;        //�������ĺ���
}STSTAPHONENO;

#define CON_SERVE_TYPE_NO               0x00
#define CON_SERVE_TYPE_HTTP         0x01
#define CON_SERVE_SOCKS4                0x02
#define CON_SERVE_SOCKS5                0x03

#define CON_SERVE_NOVERIFY              0
#define CON_SERVE_VERIFY                1

//��������
#define CON_ENCRYPT_INVALID         0xFF        //���������Ч
#define CON_ENCRYPT_DIR             0x00        //ֱ������  ������Ϊ����֤����
#define CON_ENCRYPT_CRC             0x01        //CRC����
#define CON_MAX_PASSERR_TIMES       10          //��������������
#define CON_PASSERR_LOCK_TIME       86400       //���һ�ο������ʼ�ı���ʱ�䣬��λ��

typedef struct  _STTERENCRYPT
{
    unsigned char ucAlgoCode;           //�����㷨����
    unsigned char ucPassword[4];        //�������
    unsigned char ucPassword_New[16];   //16�ֽ�����
    unsigned char ucAdminPassword[4];   //�澯�������
    unsigned char ucErrPassword[4];     //����Ŀ������
    unsigned char ucErrTimes;           //�������
    unsigned long ulLockTime;           //��������ʱ��
}STTERENCRYPT;

//��������
#define CON_REGIONADDRLEN               2
//�ն˵�ַ
#define CON_TERDEVADDRLEN               2
typedef struct _STTERREGIONADDR
{
    unsigned char ucRegionAddr[CON_REGIONADDRLEN];
}STTERREGIONADDR;
typedef struct _STTERDEVADDR
{
    unsigned char ucDevAddr[CON_TERDEVADDRLEN];
}STTERDEVADDR;

//�ն����ַ
#define CON_GROUPADDRLEN                2
#define CON_GROUPADDRNUM                8
typedef struct _STTERGROUPADDR
{
    unsigned char ucGroupAddr[CON_GROUPADDRLEN];
}STTERGROUPADDR;
typedef struct  _STTERGROUPADDRLIST         //�ն����ַ�б�
{
    STTERGROUPADDR  stGroupAddr[CON_GROUPADDRNUM];
}STTERGROUPADDRLIST;

#define     CON_TERCOMMSTAT_AUTOUP          0x55
#define     CON_TERCOMMSTAT_AUTOUP_NO       0xAA
#define     CON_TERCOMMSTAT_TALK            0x55
#define     CON_TERCOMMSTAT_TALK_NO         0xAA

typedef struct _STTERCOMMSTAT
{
    unsigned char   ucTerAutoUpFlag;
    unsigned char   ucTerTalkFlag;
}STTERCOMMSTAT;
/******************************************�澯����****************************************/
//�¼��澯����
#define     CON_ALR_SHIELD          0x55
#define     CON_ALR_SHIELD_NOT      0xAA
#define     CON_ALR_ACD             0x55
#define     CON_ALR_ACD_NOT         0xAA
#define     CON_ALR_HAPPEN          0x55
#define     CON_ALR_HAPPEN_NOT      0xAA

#define         CON_ALR_SHIELDFLAG      0xAA
#define         CON_ALR_ACDFLAG         0x55

typedef struct _STALRCATEGORY
{
    unsigned char   ucTerDataInit;          //���ݳ�ʼ��
    unsigned char   ucTerParaLost;          //������ʧ
    unsigned char   ucTerParaChange;        //��Ҫ�����仯

    unsigned char   ucStatChange;           //״̬���仯
    unsigned char   ucRemoteCtrl;           //ң��
    unsigned char   ucPowerCtrl;            //����
    unsigned char   ucEnergyCtrl;           //���

    unsigned char   ucMeterParaChange;      //��Ʋ����仯
    unsigned char   ucTAErr;                //TA�쳣
    unsigned char   ucTVErr;                //TV�쳣
    unsigned char   ucPhaseErr;             //��λ�쳣

    unsigned char   ucMeterTimeErr;         //���ʱ���쳣
    unsigned char   ucMeterErrInfo;         //��ƹ�����Ϣ

    unsigned char   ucTerPowOff;            //�ն�ͣ��
    unsigned char   ucHarmWaveOver;         //г��Խ��
    unsigned char   ucDirSimuValOver;       //ֱ��ģ����Խ��
    unsigned char   ucUnbalanceOver;        //��ƽ���Խ��

    unsigned char   ucCapLockSelf;          //������Ͷ������
    unsigned char   ucBuyPowPara;           //�����������
    unsigned char   ucPassErr;              //�������
    unsigned char   ucTerErr;               //�ն˹���

    unsigned char   ucVolOverUp;            //��ѹԽ��
    unsigned char   ucCurOverUp;            //����Խ��
    unsigned char   ucSPowOverUp;           //���ڹ���Խ��
    unsigned char   ucSPowErrUp;            //���ڹ���Խ������

    unsigned char   ucMeterDown;            //���ʾ���½�
    unsigned char   ucPowDiffOver;          //����������
    unsigned char   ucMeterFly;             //��Ʒ���
    unsigned char   ucMeterStop;            //���ͣ��

    unsigned char   ucEnerAlr;              //ERC23 ��ظ澯�¼�
    unsigned char   ucMeterCopy;            //ERC31 485����ʧ���¼���¼
    unsigned char   ucFluxLimit;            //ERC32 ��ͨ�ų�������¼

    unsigned char   ucMeterState;           //ERC33 ���״̬�ֱ�λ

//    unsigned char   ucSumInstantPowOver[CON_SUMGROUP_NUM];  //�й������ܼ���Խ��
//    unsigned char   ucSumInstantVarOver[CON_SUMGROUP_NUM];  //�޹������ܼ���Խ��
//    unsigned char   ucSumPowValOver[CON_SUMGROUP_NUM];      //�й������ܼ���Խ��
//    unsigned char   ucSumVarValOver[CON_SUMGROUP_NUM];      //�޹������ܼ���Խ��
//    unsigned char   ucSumCompOverLimit;                     //�Խ��
}STALRCATEGORY;

typedef struct _STTERALARMJUDGECOUNT
{
    unsigned char ucAlrCount_PolarityReverseA;  //A�����������
    unsigned char ucAlrCount_PolarityReverseB;  //B�����������
    unsigned char ucAlrCount_PolarityReverseC;  //C�����������

//  unsigned char ucAlrCount_ShortCircuitA1;    //A��1�β�CT��·
//  unsigned char ucAlrCount_ShortCircuitB1;    //B��1�β�CT��·
//  unsigned char ucAlrCount_ShortCircuitC1;    //C��1�β�CT��·
//
//  unsigned char ucAlrCount_ShortCircuitA2;    //A��2�β�CT��·
//  unsigned char ucAlrCount_ShortCircuitB2;    //B��2�β�CT��·
//  unsigned char ucAlrCount_ShortCircuitC2;    //C��2�β�CT��·
//
//  unsigned char ucAlrCount_OpenCircuitA2;     //A��2�β�CT��·
//  unsigned char ucAlrCount_OpenCircuitB2;     //B��2�β�CT��·
//  unsigned char ucAlrCount_OpenCircuitC2;     //C��2�β�CT��·

    unsigned char ucAlrCount_VolABreak;         //A���ѹ����
    unsigned char ucAlrCount_VolBBreak;         //B���ѹ����
    unsigned char ucAlrCount_VolCBreak;         //C���ѹ����

    unsigned char ucAlrCount_VolALack;          //A���ѹȱ��
    unsigned char ucAlrCount_VolBLack;          //B���ѹȱ��
    unsigned char ucAlrCount_VolCLack;          //C���ѹȱ��

    unsigned char ucAlrCount_VolCirReverse;     //��ѹ��·������

    unsigned char ucAlrCount_IUnBalance;        //������ƽ���Խ��
    unsigned char ucAlrCount_UUnBalance;        //��ѹ��ƽ���Խ��

    unsigned char ucAlrCount_VolAErrDown;       //A���ѹԽ������
    unsigned char ucAlrCount_VolBErrDown;       //B���ѹԽ������
    unsigned char ucAlrCount_VolCErrDown;       //C���ѹԽ������

    unsigned char ucAlrCount_VolAErrUp;         //A���ѹԽ������
    unsigned char ucAlrCount_VolBErrUp;         //B���ѹԽ������
    unsigned char ucAlrCount_VolCErrUp;         //C���ѹԽ������

    unsigned char ucAlrCount_CurAErrUp;         //A�����Խ������
    unsigned char ucAlrCount_CurBErrUp;         //B�����Խ������
    unsigned char ucAlrCount_CurCErrUp;         //C�����Խ������

    unsigned char ucAlrCount_CurANorUp;         //A�����Խ����
    unsigned char ucAlrCount_CurBNorUp;         //B�����Խ����
    unsigned char ucAlrCount_CurCNorUp;         //C�����Խ����

    unsigned char ucAlrCount_SPowNorUp;         //���ڹ���Խ����
    unsigned char ucAlrCount_SPowErrUp;         //���ڹ���Խ������
}STTERALARMJUDGECOUNT;

typedef struct  _STALRCTRLWORD
{
    STALRCATEGORY   stAlrShield;        //�澯�����֣�0:���¼���¼��1�����¼���¼
    STALRCATEGORY   stAlrACD;           //��Ҫ�¼����
}STALRCTRLWORD;         //112 byte
/*************************************���ƶ�ֵ******************************************/
#define CONSWITCHPERTIMENUM     16      //ʱ����
#define CONSWITCHCURVENUM       8       //8�����ʶ�ֵ����
#define CONSWITCHCURVE_RESERVE0     0   //0����1    �����ȼ�
#define CONSWITCHCURVE_POWLOWER     1   //1��ǰ�����¸���
#define CONSWITCHCURVE_SALESTOP     2   //2Ӫҵ��ͣ����
#define CONSWITCHCURVE_RESTCTRL     3   //3���ݿ�
#define CONSWITCHCURVE_TIMEPER1     4   //4ʱ�ο���1
#define CONSWITCHCURVE_TIMEPER2     5   //5ʱ�ο���2
#define CONSWITCHCURVE_TIMEPER3     6   //6ʱ�ο���3
#define CONSWITCHCURVE_RESERVE7     7   //7����2    �����ȼ�

#define CON_POWER_BIGVAL        999999

#define CON_AVALIBLE_FLAG       0x55
#define CON_INVALIBLE_FLAG      0xAA

#define CON_ENERGYCTRL_MON      0x11    //�µ���
#define CON_ENERGYCTRL_BUY      0x22    //������
#define CON_ENERGYCTRL_INVALID  0xBB    //��Ч

#define CON_ENERGYFLASH_ADD     0x55    //׷��
#define CON_ENERGYFLASH_NEW     0xAA    //����

#define CON_CTRLFEE_NUM         48
#define CON_CTRLFEETIME_NUM     48      //30����һ�Σ�48��

#define CON_CYCLETYPE_DAY       0x11
#define CON_CYCLETYPE_WEEK      0x22
#define CON_CYCLETYPE_MONTH     0x33

#define CONCYCLEDAYNUM          31

#define CON_BUYBILL_NUM         1       //������ʷ���絥��1��
//#define CON_BUYBILL_NUM           10      //������ʷ���絥��10��

typedef struct _STLOADFIXVAL
{
    double          dFixVal;            //��ֵ
    unsigned short  usFloatCoef;        //����ϵ��
    unsigned short  usDT2;              //��ʽ����
}STLOADFIXVAL;

typedef struct _STPOWERCTRLPERTIME      //���ʿ���ʱ����Ϣ
{
    unsigned char ucAvailbleFlag;       //��Ч��־
    unsigned char ucCtrlFlag;           //�Ƿ���Ʊ�־
    unsigned char ucOnFlag;             //Ͷ���־
    unsigned char ucBeginHour;          //ʱ�ο�ʼʱ��,ʱ
    unsigned char ucBeginMinute;        //ʱ�ο�ʼʱ��,��
    STLOADFIXVAL  stLoadFixVal;         //���ʶ�ֵ��һ�β࣬��λkW��
}STPOWERCTRLPERTIME;

typedef struct _STPOWERCTRLPROJ
{
    unsigned char   ucProjAvailbleFlag;
    unsigned char   ucCycleType;        //�տ��ơ��ܿ��ơ��¿��Ƶ�
    unsigned short  usCycleNum;         //�������ں����
    unsigned char   ucAvailbleFlag[CONCYCLEDAYNUM];
    STDATETIME  stStartTime;            //�ƻ���ʼʱ��
    STDATETIME  stEndTime;              //�ƻ�����ʱ��
}STPOWERCTRLPROJ;

typedef struct _STPOWERCTRLTIME         //���ʿ���ʱ����
{
    unsigned char       ucAvailbleFlag; //Ͷ���־
    STPOWERCTRLPERTIME  stPowerCtrlPer[CONSWITCHPERTIMENUM];
    unsigned char       ucTimerPerFlag[12];
}STPOWERCTRLTIME;

// typedef struct _STPOWERCTRLHOLDTIME
// {
//  unsigned short      usJumpTime[CON_PARA_SWITCHOUT_NUM];         //�����趨ֵ��բ�Ĺ����ɳ���ʱ��
//  unsigned short      usRevTime[CON_PARA_SWITCHOUT_NUM];          //�ָ�����ʱ��
//  unsigned short      usJumpAlrTime[CON_PARA_SWITCHOUT_NUM];      //��բ�澯ʱ��
//  unsigned short      usRevAlrTime[CON_PARA_SWITCHOUT_NUM];       //�ָ��澯ʱ��
// }STPOWERCTRLHOLDTIME;

// typedef struct _STCTRLTURN
// {
//  unsigned char       ucAvailbleFlag[CON_PARA_SWITCHOUT_NUM];
// }STCTRLTURN;

typedef struct _STENERGYCTRL
{
    unsigned char       ucEnergyCtrlType;   //��ǰ�������
    unsigned char       ucTurnFlag;         //�ִα�־
    unsigned char       ucAlrStatus;        //�澯״̬
    unsigned char       ucReserve;
    STLOADFIXVAL        stAlrFixVal;        //�澯��ֵ
    STLOADFIXVAL        stJumpFixVal;       //��բ��ֵ
    STLOADFIXVAL        stMonthEnergy;      //�µ���
    STLOADFIXVAL        stBuyEnergy;        //�������
    STLOADFIXVAL        stTurnFixVal;       //�ִε���
    STLOADFIXVAL        stTurnAlrFixVal;    //�ִθ澯��ֵ
    STLOADFIXVAL        stTurnJumpFixVal;   //�ִ���բ��ֵ
}STENERGYCTRL;

typedef struct _STMONENERGYREC              //�µ�����¼
{
    STLOADFIXVAL        stMonFixVal;
}STMONENERGYREC;

typedef struct _STBUYENERGYREC              //���һ�ι�������¼
{
    unsigned long       ulBuyBillNo;        //���絥��
    unsigned char       ucFlashFlag;        //ˢ�¡�׷�ӱ��
    STLOADFIXVAL        stBuyFixVal;        //���ι�����
    STLOADFIXVAL        stAlrFixVal;        //�澯��ֵ
    STLOADFIXVAL        stJumpFixVal;       //��բ��ֵ
    STLOADFIXVAL        stTurnFixVal;       //�ִε���
    double              dLeftVal_befor;     //����ǰʣ�����
    double              dLeftVal_after;     //�����ʣ�����
}STBUYENERGYREC;

typedef struct _STCTRLFEERATE
{
    unsigned char ucFeeNum;                     //���ʱ���
    unsigned char ucUnitFlag;
    unsigned long ulFeeRate[CON_CTRLFEE_NUM];   //����ֵ,��λ��
}STCTRLFEERATE;

typedef struct _STCTRLFEERATETIME
{
    unsigned char ucFeeNum;
    unsigned char ucFeeRateNo[CON_CTRLFEETIME_NUM]; //����ʱ�����
}STCTRLFEERATETIME;

//typedef struct _STCTRLSUMGROUPPARA    //�ܼ�����Ʋ���
//{
//  STPOWERCTRLPROJ     stPowerCtrlProj[CONSWITCHCURVENUM]; //���ʿ��Ʒ���
//  STPOWERCTRLTIME     stPowerCtrl[CONSWITCHCURVENUM];     //���ʶ�ֵ����
//  STPOWERCTRLHOLDTIME stPowerHoldTime;                    //����ʱ��
//  STCTRLTURN          stPowerCtrlTurn;                    //������Ч���
//  STENERGYCTRL        stEnergyCtrl;                       //��ص�ǰ����
//  STMONENERGYREC      stMonEnergyRec;                     //�µ�����¼
//  STBUYENERGYREC      stBuyEnergyRec;                     //��������¼
//  STCTRLTURN          stEnergyCtrlTurn;                   //�����Ч���
//}STCTRLSUMGROUPPARA;

typedef struct _STBUYBILLREC
{
    unsigned long       ulBuyBillNo;        //��ʷ���絥��
    unsigned char       ucFlashFlag;
}STBUYBILLREC;

// typedef struct _STSWITCHCTRLPARA
// {
//  unsigned char       ucIsCheckGprs;  //GPRSδ��¼����
//  unsigned char       ucGprsKeepPow;  //GPRSδ��¼����
//  unsigned char       ucKeepPower;            //������  CON_AVALIBLE_FLAG/CON_INVALIBLE_FLAG
//  unsigned char       ucSwitchOut;            //�����Ƿ�������ܱ������п���
//  STLOADFIXVAL        stSafeFixVal;           //������ֵ
//  STCTRLSUMGROUPPARA  stCtrlSumGroupPara[CON_SUMGROUP_NUM];   //�����ܼ������
//  STCTRLFEERATE       stCtrlFeeRate;          //����
//  STCTRLFEERATETIME   stCtrlFeeRateTime;      //����ʱ����Ϣ
//  STBUYBILLREC        stBuyBillRec[CON_BUYBILL_NUM];
// }STSWITCHCTRLPARA;

typedef struct _STLOADCTRLWORDPARA              //���ɿ�����
{
    unsigned char   ucTimePerCtrl[CON_SUMGROUP_NUM];
    unsigned char   ucRestCtrl[CON_SUMGROUP_NUM];
    unsigned char   ucSaleStopCtrl[CON_SUMGROUP_NUM];
    unsigned char   ucPowerLowerCtrl[CON_SUMGROUP_NUM];
    unsigned char   ucMonthEnerCtrl[CON_SUMGROUP_NUM];
    unsigned char   ucBuyEnerCtrl[CON_SUMGROUP_NUM];
    unsigned char   ucUrgeFeeCtrl;
}STLOADCTRLWORDPARA;

/******************************************��������****************************************/
typedef struct  _STTERAUXPARA
{
    unsigned char ucManufactor[4];              //���̴���  ASCII   4
    unsigned char ucDevNo[8];                   //�豸���  ASCII   8
    unsigned char ucSoftwareVer[4];             //�ն�����汾��    ASCII   4
    unsigned char ucSoftwareDate[3];            //�ն�����������ڣ�������  ���ݸ�ʽ20  3
    unsigned char ucConfigInfo[11];             //�ն�����������Ϣ��    ASCII   11
    unsigned char ucSoftwareVerOld[4];          //�ն�����汾��    ASCII   4
    unsigned char ucUserDefPassword[4];         //�û��Զ������    HEX
    unsigned char ucProtoolVer[4];              //�ն�ͨ�Ź�Լ�汾�� ASCII 4
    unsigned char ucHardwareVer[4];             //�ն�Ӳ���汾��    ASCII  4
    unsigned char ucHardwareDate[3];            //�ն�Ӳ���������ڣ�������  ���ݸ�ʽ20  3
    unsigned char ucLoginPass[2];
}STTERAUXPARA;          //30 byte

#define CON_TER_MANUFACTOR      0x99            //�������� 1�ֽ�
//#define   CON_FRM_DP_START        0xD1            //�������ģ�����Ӧ����    C1H
//#define   CON_FRM_DP_DATA         0xD5            //���ݱ���  C5H
//#define   CON_FRM_DP_QUERY        0xD7            //��ѯ���ģ���ѯӦ����    C7H
//#define   CON_FRM_DP_SET          0xD8            //���ñ��ģ�����Ӧ����    C8H
//#define   CON_FRM_DP_SET_TEST     0xD9            //���ñ��ģ�����Ӧ����    C8H
//#define   CON_FRM_DP_END          0xDA            //�������ģ�����Ӧ����    CAH
//#define   CON_FRM_DP_EXECUTE      0xDC            //��װ���ģ���װӦ����    CCH
//#define   CON_FRM_DP_FINISHED     0xDD            //��װ��ɱ���  CDH

#define CON_FRM_DP_QUERY_VER    0xF000          //��ѯ�汾��
#define CON_FRM_DP_DATA         0xF001          //���ݱ���
#define CON_FRM_DP_EXECUTE      0xF002          //�л�ִ������

#define CON_FRM_DP_TYPE_PROG    0xDD            //��ѯ��������
#define CON_FRM_DP_TYPE_PARA    0x11            //��ѯ��������
#define CON_FRM_DP_TYPE_PGPA    0xFF            //��ѯ��������+��������
#define CON_FRM_DP_TYPE_MODULE  0x22            //ͨѶģ�������װ


#define CON_DOWNLOADBITMAP_SIZE 256             //��װ��ͼ
#define CON_TERDP_STATE_DOWNLOAD    0xAA        //������װ״̬
#define CON_TERDP_STATE_NORMAL      0x55        //��������״̬

#define CON_TERDP_SOURCE_RS232      0x11        //������װ�����ڴ�����
#define CON_TERDP_SOURCE_REMOTE     0x22        //������װ��Զ��������
#define CON_TERDP_SOURCE_INVALID    0xFF        //������װ����Ч��Դ
/******************************************�����������Ͳ���****************************************/
//#define       CONDATA1JOBNUM      64              //1�������������
#define     CONDATAJOBNUM       16//64              //1�����������,�޸ĸ�ֵʱ���޸�var_define_x.h��eeprom�����������Ŀǰ��16
#define     CONDATAUNITNUM      16//18//    4       //�����������ݵ�Ԫ������EEPROM���������32��

#define     CON_JOB_ACTIVE      0x55
#define     CON_JOB_NOACTIVE    0xAA

#define     CON_JOB_DATA1       0x11            //��������1������
#define     CON_JOB_DATA2       0x22            //��������2������

typedef struct _STDATAUNITID
{
    unsigned char ucDA1;
    unsigned char ucDA2;
    unsigned char ucDT1;
    unsigned char ucDT2;
}STDATAUNITID;

typedef struct _STJOBPARA
{
    unsigned char ucActive;
    unsigned char ucTimeInter;              //ʱ����
    unsigned char ucTimeUnit;               //ʱ�䵥λ
    STDATETIME    stStartActive;            //���ͻ�׼ʱ��
    STDATETIME    stNextExecute;            //�´�ִ��ʱ��
    unsigned char ucResendDataInter;        //���߳�ȡ���ݱ���
    unsigned char ucDUIDNum;                //���ݵ�Ԫ����
    STDATAUNITID  stDUID[CONDATAUNITNUM];   //���ݵ�Ԫ
}STJOBPARA;
#define CON_LITUSR_NUM      8//16//azh
#define CON_UPUSR_NUM       4//16
typedef struct _STLITTYPE
{
    unsigned char ucFnGroupNum;
    unsigned char ucBitFlag[32];
}STLITTYPE;
typedef struct _STLITUSR
{
    STLITTYPE stLitType[CON_LITUSR_NUM];
}STUPUSR;
typedef struct _STJOBFNPARA
{
    STUPUSR     stUpUsr[CON_UPUSR_NUM];
}STJOBFNPARA;

/******************************************�ն�һ�����****************************************/
#define     CON_POWERCTRL_TIMEREC_END       0xFF    //����240��ʾ����ʱ�ν���
#define     CON_POWERCTRL_TIMEREC_MAXMIN    240     //�ͷ���ͣ���0���ӵ�240���ӣ�����240��ʾ����ʱ�ν���

typedef struct _STTERNORMALPARA
{
    unsigned char ucCopyInter;                      //�ն˳�����ʱ�䣬��λ������
    unsigned char ucReserve1;
    unsigned char ucCopyFlag;
    unsigned char ucSamplePort;                     //�ɼ��˿�
    unsigned char ucDataTypeInfo;                   //���ݸ�ʽ��Ϣ
    unsigned char ucCommAddr[6];                    //ͨѶ��ַ��������
    unsigned char ucCommPass[6];                    //ͨѶ���룬������
}STTERNORMALPARA;

/******************************************�ն��������****************************************/
typedef struct _STPOWDIFFLIMIT
{
    unsigned char ucSumGNo;                         //�й��ܵ���������
    unsigned char ucSumGNoComp;                     //�Ա����
    unsigned char ucSumGNoReff;                     //�ο�
    unsigned char ucFlag;
    unsigned char ucDiffLimit;
    double        dDiffLimitVal;
    unsigned char ucSign;
}STPOWDIFFLIMIT;

typedef struct _STGPRSPDPPARA
{
    unsigned char ucPDPUseFlag;                     //����PDP�����־
    unsigned char ucPDPUserName[CON_MAX_GPRSPDPUSERNAME_LEN];
    unsigned char ucPDPPassWord[CON_MAX_GPRAPDPPASSWORE_LEN];
}STGPRSPDPPARA;
typedef struct _STGPRSRELINKTIME
{
    unsigned short usRelinkTime;
}STGPRSRELINKTIME;
typedef struct _STRCOMMPARA
{
    STGPRSPDPPARA       stGprsPDPPara;
    STGPRSRELINKTIME    stGprsRelinkTime;
}STRCOMMPARA;

#define CON_SLAVE_NUM           3
#define CON_TRANS_ADDR_NUM          3

#define CON_LINK_SALVE          1
#define CON_LINK_MASTER     0
#define CON_LINK_INVALID        0xFF
typedef struct _STSLAVEADDR
{
    STTERREGIONADDR     stTerRegionAddr;            //��������
    STTERDEVADDR        stTerAddr;                  //�ն˵�ַ
}STSLAVEADDR;
typedef struct _STLINKREMOVE
{
    unsigned char ucFlag;
    unsigned char ucTransNum;
    STTERDEVADDR stTransAddr[CON_TRANS_ADDR_NUM];
}STLINKREMOVE;
typedef struct _STLINKPARA
{
    unsigned char ucPortNo;
    STPORTPROP stLinkPortPara;
    unsigned char ucRecvOverTime;
    unsigned char ucByteOverTime;
    unsigned char ucResendTime;
    unsigned char ucInterTime;
    unsigned char ucSlaveFlag;
    unsigned char ucSlaveNum;
    STLINKREMOVE    stLinkRemove;
    STSLAVEADDR stSlaveAddr[CON_SLAVE_NUM];
}STLINKPARA;

typedef struct _STTERMISCPARA
{

    unsigned char ucAlarmSoundTime[3];                  //��Ӧʱ���
    unsigned char ucKeepAuto;                           //�Զ�����

    unsigned char ucMeterOverDiff;                      //���ܱ�����ֵ
    unsigned char ucMeterFly;
    unsigned char ucMeterStop;
    unsigned char ucMeterTimeDiff;                      //���ܱ�Уʱ��ֵ

//azh   STPOWDIFFLIMIT stPowDiffLimit[CON_MAX_DIRPOWTOTAL_COMPNUM];             //�й��ܵ������Խ���¼���������

    unsigned char ucUrgeFeeTime[3];                 //��Ӧʱ���
    unsigned char ucFeeNum;

    unsigned char ucFeePara[24];
    unsigned char ucPowCtrlCoef;
    unsigned char ucEnerCtrlCoef;
    unsigned char ucCtrlTime[12];

    unsigned char ucTalkOn;                             //�����ն�����վͨ����־
    unsigned char ucRemoveOn;                           //�ն��޳�Ͷ��
    unsigned char ucUpActiveOn;                         //�����ն������ϱ�
    unsigned char ucTerAddIsDecType;                    //�ն˵�ַ��ʾ10����

    unsigned long ulBoadrate[MAX_PORT_NUM];             //������
    unsigned char ucPinOld[4];                          //SIM��PIN
    unsigned char ucPinNew[4];                          //SIM����PIN������֤
    unsigned char ucPuk[8];                             //SIM��PUK
    unsigned short usUsePin;                            //SIM��PIN����

    unsigned char ucIPInfoType[2];                      //ͨѶͨ������
    unsigned char ucPowerCtrlTimeRecMinute;             //���ʿ��ƻָ�ʱ�䳤��
    unsigned char ucProgramKeyIsSet;                    //�ն˱�̰����Ѿ�����

    unsigned char ucRcommBaseType;                      //ͨѶͨ�����ͣ����ŷ�ʽ����IPͨѶ��ʽ
    unsigned char ucShowRCommDebug;                     //��ʾͨѶģ�������Ϣ
    unsigned long ulFluxLimit;                          //��ͨѶ��������

    STRCOMMPARA   stRCommPara;                          //ģ�����
    STREMOTERUNINFO stRCommRunInfo;                     //ģ��������Ϣ
}STTERMISCPARA;
//���ֽ�����212���ڼӱ���ʱҪע���ջ����Ŀ��ܣ��Ƿ����ȫ�ֱ���

typedef struct _STTERADJPARA
{
    unsigned char ucPowerOnCtrlLockLeftTime;            //�����ϵ����ʱ��
    unsigned char ucSumGFromZero;                       //�ܼ����0��ʼ���Ǵ�1��ʼ
    unsigned char ucEnerBuyUnit;                        //���������ƻ��߹���ѿ��Ƶ�λԪ/kwh
    unsigned char ucDownProgramSource;                  //������װ���ͣ���Ҫ��ӦֹͣһЩ����

//  unsigned char ucDPDataTranErrNeedEcho;              //������װ���ݴ��������Ҫ��Ӧ
    unsigned char ucJob1HourDataExt;                    //1�������Сʱ����������չ
    unsigned char ucMPNoFromZero;                       //�澯�������0��ʼ���Ǵ�1��ʼ
    unsigned char ucLoginNeedAck;                       //��¼55��Ҫȷ�ϣ�Ĭ��0����Ҫȷ�ϣ�����ʼ��
    unsigned char ucViewData_1;                         //��ʾ1�β����ݣ�Ĭ��0��ʾ2�β࣬1Ϊ��ʾ1�β࣬������ʾ2�β�

    unsigned char ucBeatHeartNeedAck;                   //����55��Ҫȷ�ϣ�Ĭ��0����Ҫȷ�ϣ�����ʼ��
    unsigned char ucPulseDemandCyc;                     //���������������ڣ���λ���ӣ�1-15����
    unsigned char ucMeterFEActive;                      //��������ǰ���ַ����ñ�־
    unsigned char ucMeterFENum;                         //��������ǰ���ַ�FE�ĸ���

    unsigned char ucRunMode;                            //����ģʽ�����ԣ�����
    unsigned char ucEnerBuyIsNotFee;                    //�������ر�־��55�����������������
}STTERADJPARA;


///////////////////////////CT������////////////////////////////////
typedef struct _STCTVALUE
{
    unsigned char   ucValueA;
    unsigned char   ucValueB;
    unsigned char   ucValueC;
}STCTVALUE;
typedef struct _STCTLIMITVALUE
{
    STCTVALUE stShortCircuit1;
    STCTVALUE stOpenCircuit2;
}STCTLIMITVALUE;
typedef struct _STCTCHECKPARA
{
    STCTLIMITVALUE stCTLimitValue;
    unsigned char  ucCTType;
    unsigned char  ucTerWorkMode;
    unsigned char  ucSlfStudyFlag;
}STCTCHECKPARA;
typedef struct _STCURALRCTRLTIME
{
    unsigned char ucFirstALRTime;
    unsigned char ucSecondALRTime;
    unsigned char ucThirdALRTime;
    unsigned char ucFourthALRTime;
    unsigned char ucCurCTLTime;
}STCURALRCTRLTIME;
typedef struct _STALRJUDGETIME
{
    unsigned char ucHappedTime;//�¼������ж�ʱ�䣨Bin������
    unsigned char ucResumeTime;//�¼��ָ��ж�ʱ�䣨Bin������
    unsigned char ucMpNo;       //���ߡ�ͣ���¼��Ƚϲ����㣨bin��
}STALRJUDGETIME;
typedef struct _STHARMVALUPPARA
{
    unsigned short usHarmValUpTotal;
    unsigned short usHarmValUpOdd;
    unsigned short usHarmValUpEven;
    unsigned short usHarmValUpLimit2;
    unsigned short usHarmValUpLimit4;
    unsigned short usHarmValUpLimit6;
    unsigned short usHarmValUpLimit8;
    unsigned short usHarmValUpLimit10;
    unsigned short usHarmValUpLimit12;
    unsigned short usHarmValUpLimit14;
    unsigned short usHarmValUpLimit16;
    unsigned short usHarmValUpLimit18;
    unsigned short usHarmValUpLimit3;
    unsigned short usHarmValUpLimit5;
    unsigned short usHarmValUpLimit7;
    unsigned short usHarmValUpLimit9;
    unsigned short usHarmValUpLimit11;
    unsigned short usHarmValUpLimit13;
    unsigned short usHarmValUpLimit15;
    unsigned short usHarmValUpLimit17;
    unsigned short usHarmValUpLimit19;
}STHARMVALUPPARA;
typedef struct _STHARMVALUPLIMIT
{
    STHARMVALUPPARA stVolRateLimit;
    STHARMVALUPPARA stCurRmsLimit;
}STHARMVALUPLIMIT;
typedef struct _AESCIPHER
{
    unsigned char ucAesLength;
    unsigned char ucAesKey[CON_MAX_AESCIPHER_LEN];
}STAESKEY;

/******************************************�ն˲���****************************************/
typedef struct  _STTERRUNPARA
{
    STTERCOMMPARA       stTerCommPara;              //F1���ն�����ͨ�ſ�ͨ�Ų�������
    STHEARTBEAT         stHeartBeat;                //F1:����
    STSTAIPADDR         stStaIPAddr;                //F3��վ��ַ
    STSTAPHONENO        stStaPhoneNo;               //F4����վ�绰����Ͷ������ĺ���
    STTERENCRYPT        stTerEncrypt;               //�����㷨
    STLOCALIP           stLocalIP;                  //F7���ն�IP��ַ�Ͷ˿�
    STTERGROUPADDRLIST  stTerGroupAddrList;         //F6���ն����ַ����
    STTERCOMMSTAT       stTerCommStat;              //F5���ն�����ͨ����Ϣ��֤��������
    STALRCTRLWORD       stAlrCtrlWord;              //F9���ն��¼���¼��������
    STTERALARMJUDGECOUNT    stTerAlrMaxCount;       //
    STMPINFO            stMPInfo[CON_MAX_MPNUM];    //
    STCONFIGNUM         stConfigNum;                //F10���ն˵��ܱ�/��������װ�����ò���
//  STPULSEPARA         stPulsePara[CON_PULSE_NUM]; //
    STMETERPARA         stMeterPara[CON_METER_NUM]; //
//  STSUMGROUPPARA      stSumGroupPara[CON_SUMGROUP_NUM];   //
//  STSWITCHPARA        stSwitchPara[CON_SWITCHIN_NUM];     //
//  STANALOGPARA        stAnalogPara[CON_ANALOG_NUM];       //
//  STSWITCHCTRLPARA    stSwitchCtrlPara;
    STLINKPARA          stLinkPara;

    STTERAUXPARA        stTerAuxPara;
    STTERREGIONADDR     stTerRegionAddr;            //��������
    STTERDEVADDR        stTerAddr;                  //�ն˵�ַ
//  unsigned char       ucTer_Meter_Addr[12];           //�ն˵��ص��ܱ���ʾ��

    STJOBPARA           stJobPara_Data1[CONDATAJOBNUM];     //1����������
    STJOBPARA           stJobPara_Data2[CONDATAJOBNUM];     //2����������
    STJOBFNPARA         stJobFnPara1;                           //�û�����ż�С��Ŷ�Ӧ��һ������
    STJOBFNPARA         stJobFnPara2;                           //�û�����ż�С��Ŷ�Ӧ�Ķ�������
//  STLCDSHOWPARA       stLCDShowPara;              //Һ����ʾ����
    STTERNORMALPARA     stTerNormalPara;

    STTERMISCPARA       stTerMiscPara;              //�������
    STTERADJPARA        stTerAdjPara;               //�ն˵��ڲ���
//  STCTCHECKPARA       stCTCheckPara;              //CT���

//  STLOADCTRLWORDPARA  stLoadCtrlWordPara;         //���ɿ�����
    STCURALRCTRLTIME    stCurAlrCtrlTime;
    STALRJUDGETIME      stAlarmJudgeTime;       //ȱ�ࡢ������¼��ж���ʱ�����ߡ�ͣ���¼��Ƚϲ�����
    STHARMVALUPLIMIT    stHarmValUpLimit;
    STAESKEY            stAesKey;                   //AES��Կ
    STTERCOMMPARA_INNER     stTerCommPara_Inner;
    STPORTLIST      stPortList;
//azh 170919
	unsigned char       ucWireless[2];              //F90������ͨ�Ų���
    unsigned char       ucLocation[10];             //F91���ն˵���λ��
	unsigned char       ucSnstate[2];               //F150��������״̬ //ֻ֧��8�������㣬���Ҳ�����
//azh 170920	
	unsigned char       ucSetMeterTimeOn;               //F30���ն˶Ե��ܱ��ʱ���ܵĿ�����ر�
	unsigned char       ucMeterSelfOn;               //F149�����ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
	
    unsigned char       ucClrFlg[10];               //��־�ź��棬�����仯�͵����³�ʼ��
}STTERRUNPARA;


typedef struct  _STTERRUNPARA_NOUSE
{
    unsigned char ucF73[48];
    unsigned char ucF74[10];
    unsigned char ucF75[16];
    unsigned char ucF76;
//  unsigned char ucF81[4];
//  unsigned char ucF82[4];
//  unsigned char ucF83;
}STTERRUNPARA_NOUSE;

/******************************************�ն˲�����ʼ��****************************************/
__EXTERN unsigned char RunPara_Init_All(unsigned char ucClrFlag);
//__EXTERN unsigned char RunPara_Valid_Check(void);

// __EXTERN U8  RunPara_InitTerCommPara(STTERCOMMPARA *pstTerCommpara);
// __EXTERN U8  RunPara_InitHeartBeat(STHEARTBEAT *pstHeartBeat);
__EXTERN U8 RunPara_InitTerCommPara(U8 ucClrFlag);
//__EXTERN U8  RunPara_InitStaIPAddr(STSTAIPADDR *pstStaIPAddr);
__EXTERN U8 RunPara_InitStaIPAddr(U8 ucClrFlag);
__EXTERN U8  RunPara_InitStaPhoneNo(STSTAPHONENO *pstStaPhoneNo);
__EXTERN U8  RunPara_InitLocalIP(STLOCALIP*pstLocalIP);
__EXTERN U8  RunPara_InitTerEncrypt(STTERENCRYPT *pstTerEncrypt);
__EXTERN U8  RunPara_InitTerGroupAddrList(STTERGROUPADDRLIST *pstTerGroupAddrList);
__EXTERN U8  RunPara_InitCommStat(STTERCOMMSTAT *pstTerCommStat);
__EXTERN U8  RunPara_InitAlrCtrlWord(STALRCTRLWORD *pstAlrCtrlWord);
//__EXTERN U8    RunPara_initAlrMaxCount(STTERALARMJUDGECOUNT *pstTerAlrMaxCount);

__EXTERN void   RunPara_InitMPRateVal(STMPRATEVAL *pstMPRateVal);
__EXTERN void   RunPara_InitMPAlterRate(STMPALTERRATE *pstMPAlterRate);
__EXTERN void   RunPara_InitMPLimit(STMPLIMIT *pstMPLimit);
__EXTERN void   RunPara_InitMPFreezePara(STMPFREEZEPARA *pstMPFreezePara);
__EXTERN void   RunPara_InitMPFreezeDay(STFREEZEDAY *pstFreezeDay);
__EXTERN void   RunPara_InitMPPowRatePara(STPOWRATEPARA *pstPowRatePara);
__EXTERN void   RunPara_InitMPTongsun(STTONGSUNTIANSUNPARA *pstTongsun);
__EXTERN unsigned char  RunPara_InitMPInfo(STMPINFO *pstMPInfo);
//__EXTERN unsigned char  RunPara_SetTerVolLackAlrMaxCount(unsigned char ucCount);
__EXTERN unsigned short RunPara_GetMPLimit_usLostVolTime(unsigned char ucMPNo);

__EXTERN unsigned char  RunPara_InitConfigNum(STCONFIGNUM *pstConfigNum);
__EXTERN unsigned char RunPara_InitCuFeLostPara(STCUFELOSTPARA *pstCuFeLostPara);
//__EXTERN unsigned char    RunPara_InitPulsePara(STPULSEPARA *pstPulsePara);
__EXTERN unsigned char  RunPara_InitMeterPara(STMETERPARA *pstMeterPara);
//__EXTERN unsigned char    RunPara_InitSumGroupPara(STSUMGROUPPARA *pstSumGroupPara);
//__EXTERN unsigned char    RunPara_InitSwitchPara(STSWITCHPARA* pstSwitchPara);
//__EXTERN unsigned char    RunPara_InitAnalogPara(STANALOGPARA* pstAnalogPara);
//__EXTERN unsigned char    RunPara_InitSwitchCtrlPara(STSWITCHCTRLPARA *pstSwitchCtrlPara);
//__EXTERN unsigned char    RunPara_InitLoadCtrlWordPara(STLOADCTRLWORDPARA *pstLoadCtrlWordPara);

//__EXTERN U8 RunPara_InitDT2(STDT2 *pstDT2);

__EXTERN unsigned char RunPara_InitLinkPara(STLINKPARA *pstLinkPara);
__EXTERN unsigned char RunPara_InitRemove(STLINKREMOVE  *pstLinkRemove);
__EXTERN unsigned char RunPara_InitPortPara(STPORTPROP *pstPortProp);
__EXTERN unsigned char  RunPara_InitAuxPara(STTERAUXPARA *pstTerAuxPara);
__EXTERN unsigned char  RunPara_InitTerDevAddr(STTERDEVADDR *pstTerAddr);
__EXTERN unsigned char  RunPara_InitTerRegionAddr(STTERREGIONADDR *pstTerRegionAddr);
__EXTERN U8  RunPara_InitTerDev_Region_RegionAddr(U8 ucClrFlag);

__EXTERN unsigned char  RunPara_InitJobPara(STJOBPARA *pstJobPara);
__EXTERN U8 RunPara_InitJobPara_eeprom(U8 ucjob_num, U8 ucjob_class, U8 ucflag);    //
__EXTERN unsigned char RunPara_InitJobFnPara(STJOBFNPARA *pstJobFnPara);
//__EXTERN unsigned char RunPara_InitHarmLimit(STHARMVALUPPARA *pstHarmLimitPara);
__EXTERN void RunPara_InitJobRunInfo(U8 ucflag);
__EXTERN unsigned char RunPara_InitJobRunInfoPer(STJOBPARA *pstJobPara);
__EXTERN STDATETIME RunPara_GetJobNextTime(STDATETIME stActiveTime,unsigned char ucTimeInter,unsigned char ucTimeUnit);
__EXTERN unsigned char RunPara_InitPortPropList(STPORTLIST *pstPortPropList, U8 ucflag);
__EXTERN unsigned char RunPara_InitPortProp(STPORTPROP *pstPortProp);
__EXTERN unsigned char RunPara_InitRMRunPara(U8 ucflag);

__EXTERN unsigned char  RunPara_InitTerNormalPara(STTERNORMALPARA *pstTerNormalPara);
__EXTERN unsigned char  RunPara_InitTerMiscPara(STTERMISCPARA *pstTerMiscPara);
__EXTERN unsigned char  RunPara_InitTerAdjPara(STTERADJPARA *pstTerAdjPara);
//__EXTERN unsigned char    RunPara_InitCTCheckPara(STCTCHECKPARA *pstCTCheckPara);
//__EXTERN unsigned char    RunPara_First_InitTerPara(void);
__EXTERN unsigned char  RunPara_InitMeterProtocolInfo(void);

/******************************************�ն˲�����ȡ****************************************/
__EXTERN STTERCOMMPARA  RunPara_GetTerCommPara(void);
__EXTERN STHEARTBEAT        RunPara_GetHeartBeat(void);
__EXTERN STSTAIPADDR        RunPara_GetStaIPAddr(void);
__EXTERN STSTAPHONENO   RunPara_GetStaPhoneNo(void);
__EXTERN STLOCALIP  RunPara_GetLocalIP(void);
__EXTERN STTERENCRYPT   RunPara_GetTerEncrypt(void);
__EXTERN STTERGROUPADDRLIST RunPara_GetTerGroupAddrList(void);
//__EXTERN STTERCOMMSTAT    RunPara_GetTerCommStat(void);
//__EXTERN STGPRSPDPPARA*   RunPara_GetGprsPdpPara(void);
__EXTERN unsigned char  RunPara_GetTerCommPara_RCommMode(void);

__EXTERN STALRCATEGORY*  RunPara_GetAlrShieldACDWord(unsigned char ucNo);
__EXTERN STTERALARMJUDGECOUNT   RunPara_GetAlrMaxCount(void);
//__EXTERN STALRCATEGORY  Alarm_Get_Happen_word(void);
__EXTERN unsigned char  Alarm_Set_Happen_word(STALRCATEGORY *pstAlrHappenWord);
__EXTERN unsigned char  Alarm_Set_AlrCate_word(STALRCATEGORY *pstAlrHappenWord,unsigned char ucAlrERCCode,unsigned char ucSetVal);
__EXTERN unsigned char  RunPara_GetMPType(unsigned char ucMPNo);
__EXTERN unsigned char  RunPara_GetMPDevNo(unsigned char ucMPNo);
__EXTERN unsigned char  RunPara_GetMPNo_byDevNo(unsigned char ucDevNo,unsigned char ucMPType);
__EXTERN STMPRATEVAL        RunPara_GetMPRateVal(unsigned char ucMPNo);
__EXTERN STMPALTERRATE  RunPara_GetMPAlterRate(unsigned char ucMPNo);
__EXTERN STMPLIMIT      RunPara_GetMPLimit(unsigned char ucMPNo);
//__EXTERN STMPFREEZEPARA*  RunPara_GetMPFreezePara(unsigned char ucMPNo);
//__EXTERN STFREEZEDAY        RunPara_GetMPFreezeDay(unsigned char ucMPNo);
__EXTERN PSTFREEZEDAY        RunPara_GetMPFreezeDay(unsigned char ucMPNo);
__EXTERN STPOWRATEPARA  RunPara_GetMPPowRatePara(unsigned char ucMPNo);
__EXTERN STTONGSUNTIANSUNPARA RunPara_GetMPTongsun(unsigned char ucMPNo);
__EXTERN STLINKPARA RunPara_GetLinkPara(void);
//__EXTERN STSLAVEADDR RunPara_GetSlaveAddr(U8 ucSlaveNo);
__EXTERN STLINKREMOVE RunPara_GetLinkRemove(void);
__EXTERN STPORTLIST RunPara_GetPortPropList(void);
__EXTERN STPORTPROP RunPara_GetPortProp(U8 ucPort);
__EXTERN STRMRUNPARA *RunPara_GetRMRunPara(void);

__EXTERN STCONFIGNUM        RunPara_GetConfigNum(void);
//__EXTERN STPULSEPARA      RunPara_GetPulsePara(U8 ucPulseNo);
__EXTERN STMETERPARA        RunPara_GetMeterPara(U8 ucMeterNo);
//__EXTERN U8    RunPara_GetMeterPara_Proto(U8 ucMeterNo);
__EXTERN STMETERPARA        RunPara_GetMeterPara_byMPNo(U8 ucMPNo);
//__EXTERN unsigned long RunPara_GetMeterPara_Boadrate_byMPNo(U8 ucMPNo);
__EXTERN U8  RunPara_GetMeterPara_PortNo(U8 ucMeterNo);
__EXTERN U8  RunPara_GetMeterPara_PortNo_byMPNo(U8 ucMPNo);
//__EXTERN U8 RunPara_GetMeterPara_EmphaFlag(U8 ucMeterNo);

//__EXTERN STSUMGROUPPARA   RunPara_GetSumGroupPara(U8 ucSumGNo);
//__EXTERN STSUMGROUPPARA   *RunPara_GetSumGroupParaPtr(U8 ucSumGNo);
//__EXTERN STSWITCHPARA RunPara_GetSwitchPara(U8 ucSwitchNo);
//__EXTERN STANALOGPARA RunPara_GetAnalogPara(U8 ucAnalogNo);
//__EXTERN U8    RunPara_GetAnalogPortNo(U8 ucMPNo,U8 ucAttr);

//__EXTERN STPOWERCTRLPROJ RunPara_GetPowCtrlProj(U8 ucCurveNo,U8 ucSumGroupNo);
//__EXTERN STLOADFIXVAL RunPara_GetPowFixVal(U8 *pucPowCurveNo,U8 *pucEndHour,U8 *pucEndMinute,U8 ucSumGroupNo);
//__EXTERN STPOWERCTRLHOLDTIME RunPara_GetPowCtrlHoldTime(U8 ucSumGroupNo);
//__EXTERN STCTRLTURN       RunPara_GetPowerCtrlTurn(U8 ucSumGroupNo);
//__EXTERN STENERGYCTRL RunPara_GetEnergyCtrl(U8 ucSumGroupNo);
//__EXTERN STMONENERGYREC   RunPara_GetMonEnergyRec(U8 ucSumGroupNo);
//__EXTERN STBUYENERGYREC   RunPara_GetBuyEnergyRec(U8 ucSumGroupNo);
//__EXTERN STCTRLTURN       RunPara_GetEnergyCtrlTurn(U8 ucSumGroupNo);
//__EXTERN U8    RunPara_GetInfo_IsEnerTotalOnly(U8 ucSumGroupNo);
//__EXTERN U8    RunPara_Manage_BuyBillNo(unsigned long ulBuyBillNo,U8 ucFlashFlag);

//__EXTERN STCTRLFEERATE    RunPara_GetCtrlFeeRate(void);
//__EXTERN STCTRLFEERATETIME    RunPara_GetCtrlFeeRateTime(void);
//__EXTERN STLOADFIXVAL RunPara_GetCtrlSafeFixVal(void);
//__EXTERN U8   RunPara_GetSwitchOutFlag(void);
//__EXTERN STLOADCTRLWORDPARA   RunPara_GetLoadCtrlWordPara(void);
//__EXTERN STPOWERCTRLTIME RunPara_GetPowCtrlTimePer(U8 ucCurveNo,U8 ucSumGroupNo);
//__EXTERN U8    RunPara_GetKeepPower(void);
//__EXTERN U8    RunPara_GetGprsKeepPow(void);
//__EXTERN U8    RunPara_GetIsCheckGprs(void);

//__EXTERN U8    RunPara_GetLoadCtrl_UrgeFee(void);

__EXTERN STTERAUXPARA   RunPara_GetTerAuxPara(void);
__EXTERN STTERDEVADDR   RunPara_GetTerDevAddr(void);
__EXTERN STTERREGIONADDR    RunPara_GetTerRegionAddr(void);

__EXTERN STJOBPARA*      RunPara_GetJobPara(U8 ucJobNo,U8 ucJobClassNo);
__EXTERN STUPUSR RunPara_GetUpUsrJobPara(U8 ucJobNo,U8 ucUpUsr);

__EXTERN STTERNORMALPARA    RunPara_GetTerNormalPara(void);
__EXTERN STTERMISCPARA  RunPara_GetTerMiscPara(void);
//__EXTERN unsigned long    RunPara_GetUartBoad(U8 ucUartID);
__EXTERN STTERADJPARA   RunPara_GetTerAdjPara(void);
//__EXTERN STCTCHECKPARA    RunPara_GetCTCheckPara(void);
__EXTERN STALRJUDGETIME RunPara_GetAlrJuDgeTime(void);
__EXTERN STCURALRCTRLTIME   RunPara_GetAlrCtrlTime(void);
//__EXTERN STHARMVALUPLIMIT RunPara_GetHarmValUpLimit(void);
//__EXTERN U8   RunPara_GetTerAdjPara_DownloadSource(void);
__EXTERN U8   RunPara_GetTerAdjPara_Job1HourDataExt(void);
__EXTERN U8   RunPara_GetMeterCopyInter(void);
__EXTERN U8 RunPara_GetMeterCopyCopyFlag(void);
__EXTERN U8   RunPara_GetTerMiscParaStatus_ProgramKeyIsSet(void);
__EXTERN U8   RunPara_GetTerAdjPara_EnerBuyIsNotFee(void);

__EXTERN U8  RunPara_GetTerMisc_MeterTimeDiff(void);
//__EXTERN U8   RunPara_GetTerAdj_ViewData_1_Flag(void);
__EXTERN U8   RunPara_GetTerAdj_MPNoFromZero(void);
//__EXTERN unsigned short  RunPara_GetMP_VolAlterRate(U8 ucMPNo);
//__EXTERN unsigned short  RunPara_GetMP_CurAlterRate(U8 ucMPNo);
//__EXTERN unsigned long   RunPara_GetMP_PowerAlterRate(U8 ucMPNo);
//__EXTERN U8    RunPara_GetTerMisc_RemoveOn(void);
//__EXTERN U8   RunPara_GetSwitchPara_PulseIsTypeSwitch(U8 ucPulseNo);
//__EXTERN U8    RunPara_GetTerMisc_RCommRunStat(void);
//__EXTERN U8    RunPara_GetTerMisc_GprsLoginStat(void);
//__EXTERN U8    RunPara_GetTerMisc_TerAddIsDecType(void);
__EXTERN U8  RunPara_GetTerMisc_DefaultIPType(void);
__EXTERN U8  RunPara_GetTerMisc_ucRcommBaseType(void);
__EXTERN unsigned long  RunPara_GetTerMisc_FluxLimit(void);
//azh 170919
__EXTERN U8*   RunPara_GetTerMisc_Wireless(void);
__EXTERN U8*   RunPara_GetTerMisc_TerLocation(void);
__EXTERN U8*   RunPara_GetTerMisc_Snstate(void);

//__EXTERN U8    RunPara_GetTerMisc_ShowRCommDebug(void);
//__EXTERN U8    RunPara_GetTerMisc_RcommType(U8 *pucIPInfoType0,U8 *pucIPInfoType1,U8 *pucRcommBaseType);
//__EXTERN U8    RunPara_GetTerMisc_PowDiffLimit_ucSumGNo(U8 ucSeqNo);

/******************************************�ն˲�������****************************************/
__EXTERN U8 RunPara_SetTerCommPara(STTERCOMMPARA *pstTerCommPara);
__EXTERN U8 RunPara_SetHeartBeat(STHEARTBEAT *pstHeartBeat);
__EXTERN U8 RunPara_SetStaIPAddr(STSTAIPADDR *pstStaIPAddr);
__EXTERN U8 RunPara_SetStaPhoneNo(STSTAPHONENO *pstStaPhoneNo);
__EXTERN U8 RunPara_SetTerLocalIP(STLOCALIP  *pstLocalIP);
__EXTERN U8 RunPara_SetTerEncrypt(STTERENCRYPT *pstTerEncrypt);
__EXTERN U8 RunPara_SetTerGroupAddrList(STTERGROUPADDRLIST *pstTerGroupAddr);
//__EXTERN U8 RunPara_SetTerCommStat(STTERCOMMSTAT *pstTerCommStat);
//__EXTERN U8 RunPara_SetGprsPdpPara(STGPRSPDPPARA *pstGprsPdpPara);
__EXTERN U8 RunPara_SetAlrCtrlWord(STALRCTRLWORD *pstAlrCtrlWord);
__EXTERN U8 RunPara_SetAlrMaxCount(STTERALARMJUDGECOUNT *pstAlrJudgeCount);
//__EXTERN U8 RunPara_SetRcommType(U8 ucIPInfoType0,U8 ucIPInfoType1,U8 ucRcommBaseType);

__EXTERN U8 RunPara_SetMPType(U8 ucMPNo,U8 ucMPType);
__EXTERN U8 RunPara_SetMPDevNo(U8 ucMPNo,U8 ucDevNo);
__EXTERN U8 RunPara_SetMPRateVal(U8 ucMPNo,STMPRATEVAL *pstMPRateVal);
__EXTERN U8 RunPara_SetMPAlterRate(U8 ucMPNo,STMPALTERRATE *pstMPAlterRate);
__EXTERN U8 RunPara_SetMPLimit(U8 ucMPNo,STMPLIMIT    *pstMPLimit);
//__EXTERN U8 RunPara_SetMPFreezePara(U8 ucMPNo,STMPFREEZEPARA    *pstMPFreezePara);
__EXTERN U8 RunPara_SetMPFreezeDay(U8 ucMPNo,STFREEZEDAY  *pstFreezeDay);
__EXTERN U8 RunPara_SetMPPowRatePara(U8 ucMPNo,STPOWRATEPARA  *pstPowRatePara);
__EXTERN U8 RunPara_SetMPTongsun(U8 ucMPNo,STTONGSUNTIANSUNPARA *pstTongsun);

//__EXTERN U8 RunPara_SetPulseMPDevNo(U8 ucMPNo,U8 ucDevNo,U8 ucPulseType);
//__EXTERN U8 RunPara_CheckPulseMPDevNo_IsNone(U8 ucMPNo);

//__EXTERN U8 RunPara_SetConfigNum(STCONFIGNUM *pstConfigNum);
//__EXTERN U8 RunPara_SetPulsePara(U8 ucPulseNo,STPULSEPARA *pstPulsePara);
__EXTERN U8 RunPara_SetMeterPara(U8 ucMeterNo,STMETERPARA *pstMeterPara);
//__EXTERN U8 RunPara_SetSumGroupPara(U8 ucSumGNo,STSUMGROUPPARA *pstSumGPara);
//__EXTERN U8 RunPara_SetSumGroupAvailableFalg(U8 ucSumGNo,U8 ucAvailableFlag);
//__EXTERN U8 RunPara_SetSumGroupParaOper(U8 ucSumGNo,STSUMGROUPPARA *pstSumGPara);
//__EXTERN U8 RunPara_SetSumGroupParaLimit(U8 ucSumGNo,STSUMGROUPPARA *pstSumGPara);
//__EXTERN U8 RunPara_SetSumGroupParaFreeze(U8 ucSumGNo,STSUMGROUPPARA *pstSumGPara);
//__EXTERN U8 RunPara_SetSumGroupOper(U8 ucNo,STSUMVALOPER *pstSumGValOper,U8 ucMPNo,U8 ucOper);
//__EXTERN U8 RunPara_SetSwitchPara(U8 ucSwitchNo,STSWITCHPARA *pstSwitchPara);
//__EXTERN U8 RunPara_SetAnalogPara(U8 ucAnalogNo,STANALOGPARA *pstAnalogPara);
__EXTERN U8 RunPara_SetLinkPara(STLINKPARA *pstLinkPara);
__EXTERN U8 RunPara_SetLinkRemove(STLINKREMOVE *pstLinkRemove);

//__EXTERN U8 RunPara_SetPowCtrlProj(U8 ucCurveNo,STPOWERCTRLPROJ *pstPowerCtrlProj,U8 ucSumGroupNo);
//__EXTERN U8 RunPara_SetPowCtrlProj_Availble(U8 ucFlag,U8 ucCurveNo,U8 ucSumGroupNo);
//__EXTERN U8 RunPara_SetPowerCtrlAvailble(U8 ucCurveNo,U8 ucFlag,U8 ucSumGroupNo);
//__EXTERN U8 RunPara_SetCtrlSafeFixVal(STLOADFIXVAL *pstSaveFixVal);
//__EXTERN U8 RunPara_SetCtrlFeeRate(STCTRLFEERATE *pstCtrlFeeRate);
//__EXTERN U8 RunPara_SetCtrlFeeRateTime(STCTRLFEERATETIME *pstCtrlFeeRateTime);
//__EXTERN U8 RunPara_SetLoadCtrlWordPara(STLOADCTRLWORDPARA *pstLoadCtrlWordPara);
//__EXTERN U8 RunPara_SetPowCtrlTimePer(U8 ucCurveNo,STPOWERCTRLTIME *pstPowerCtrlTime,U8 ucSumGroupNo);
//__EXTERN U8 RunPara_SetPowCtrlTimePer_Availble(U8 ucFlag,U8 ucCurveNo,U8 ucSumGroupNo);
//__EXTERN U8 RunPara_SetEnergyCtrl(STENERGYCTRL *pstEnergyCtrl,U8 ucSumGroupNo);
//__EXTERN U8 RunPara_SetPowerCtrlTurn(STCTRLTURN *pstPowerCtrlTurn,U8 ucSumGroupNo);
//__EXTERN U8 RunPara_SetEnergyCtrlTurn(STCTRLTURN *pstEnergyCtrlTurn,U8 ucSumGroupNo);
//__EXTERN U8 RunPara_SetPowCtrlHoldTime(STPOWERCTRLHOLDTIME *pstPowerHoldTime,U8 ucSumGroupNo);
//__EXTERN U8 RunPara_SetKeepPower(U8 ucFlag);
//__EXTERN U8 RunPara_SetGprsCheck(U8 ucFlag);
//__EXTERN U8 RunPara_SetGprsKeepPow(U8 ucFlag);
//__EXTERN U8 RunPara_SetBuyEnergyRec(STBUYENERGYREC *pstBuyEnergyRec,U8 ucSumGroupNo);
//__EXTERN U8 RunPara_SetMonEnergyRec(STMONENERGYREC *pstMonEnergyRec,U8 ucSumGroupNo);

__EXTERN U8 RunPara_SetTerAuxPara(STTERAUXPARA *pstTerAuxPara);
__EXTERN U8 RunPara_SetTerDevAddr(STTERDEVADDR *pstTerAddr);
__EXTERN U8 RunPara_SetTerRegionAddr(STTERREGIONADDR *pstTerRegionAddr);

__EXTERN U8 RunPara_SetJobPara(U8 ucJobClassNo,U8 ucJobNo,STJOBPARA *pstJobPara);
__EXTERN U8 RunPara_SetUpUsrJob(U8 ucJobNo,U8 ucUpUsr,STUPUSR *pstUpUsr);

__EXTERN U8 RunPara_SetPortPropList(STPORTLIST *pstPortList);
__EXTERN U8 RunPara_SetPortProp(STPORTPROP *pstPortProp,U8 ucPort);
//__EXTERN U8 RunPara_SetRMRunPara(STRMRUNPARA *pstRMRunPara);

__EXTERN U8 RunPara_SetTerNormalPara(STTERNORMALPARA *pstTerNormalPara);
__EXTERN U8 RunPara_SetCopyFlag(U8 ucCopyFlag);
__EXTERN U8 RunPara_SetTerMiscPara(STTERMISCPARA *pstTerMiscPara);
__EXTERN U8 RunPara_SetTerAdjPara(STTERADJPARA *pstTerAdjPara);
//__EXTERN U8 RunPara_SetCTCheckPara(STCTCHECKPARA *pstCTCheckPara);
//__EXTERN U8 RunPara_SetTerAdjPara_DownloadSource(U8 ucSource);
//__EXTERN U8 RunPara_SetUartBoad(U8 ucUartID,unsigned long ulBoad);
//__EXTERN U8 RunPara_SetTerMiscParaStatus_ProgramKeyIsSet(U8 ucFlag);
//__EXTERN U8 RunPara_SetTerMiscPara_TerAddIsDecType(U8 ucFlag);
//__EXTERN U8 RunPara_SetAlrJuDgeTime(STALRJUDGETIME *pstAlarmJudgeTime);
//__EXTERN U8    RunPara_SetAlrCtrlTime(STCURALRCTRLTIME *pstCurAlrCtrlTime);
//__EXTERN U8    RunPara_SetHarmValUpLimit(STHARMVALUPLIMIT *pstHarmValUpLimit);
//__EXTERN U8 RunPara_SetTerAdjPara_Change_ViewData_1(void);
//__EXTERN U8  RunPara_SetAesKey(STAESKEY *pstAesKey);       //AES����
__EXTERN U8 RunPara_SetTerMiscPara_FluxLimit(unsigned long ulVal);

//azh 170919
__EXTERN U8 RunPara_SetTerMisc_Wireless(unsigned char* ucVal);
__EXTERN U8 RunPara_SetTerMisc_TerLocation(unsigned char* ucVal);
__EXTERN U8 RunPara_SetTerMisc_Snstate(unsigned char* ucVal);

//__EXTERN U8 RunPara_SetTerMiscPara_ShowRCommDebug(U8 ucVal);

//__EXTERN STAESKEY RunPara_GetAesKey(void);                //AES����
__EXTERN U8 RunPara_GetMeter_MPNo(U8 ucMeterNo);
__EXTERN U8 RunPara_GetPulse_MPNo(U8 ucPulseNo);
__EXTERN U8 RunPara_GetTer_MPNo(void);
__EXTERN U8 RunPara_GetMeterNo_byMPNo(U8 ucMPNo);

//__EXTERN U8 RunPara_GetPulseNo_byMPNo(U8 ucMPNo,U8 ucPulseType);

//__EXTERN U8 RunPara_GetDirPowPulseNo_byMPNo(U8 ucMPNo);
//__EXTERN U8 RunPara_GetDirVarPulseNo_byMPNo(U8 ucMPNo);
//__EXTERN U8 RunPara_GetUnDirPowPulseNo_byMPNo(U8 ucMPNo);
__EXTERN U8 RunPara_GetUnDirVarPulseNo_byMPNo(U8 ucMPNo);

//__EXTERN U8 RunPara_GetPulseType_ByDI(unsigned short usDI);

__EXTERN U8  Main_DI_Para_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,U8 *pucRecvBuffer);
__EXTERN U8  Main_DI_ParaRead_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,U8 *pucRecvBuffer);
__EXTERN U8  Main_DI_Ctrl_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,U8 ucNum4);
__EXTERN U8  Main_DI_ExtCmd_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,U8 ucNum2);
__EXTERN U8  Main_DI_DataTrans_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,U8 *pucBuffer);
__EXTERN U8  Main_DI_FileTrans_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI);

__EXTERN void RunPara_ClearPassErrTimes(void);
__EXTERN U8 RunPara_GetPassErrTimes(void);
__EXTERN void RunPara_AddPassErrTimes(void);
__EXTERN U8 RunPara_PassLockIsOverTime(void);

__EXTERN U8 RunPara_Set_LCDCycleMPNo(U8 ucMPNo);
//__EXTERN U8 RunPara_GetCtrlFeeNo_ByTime(STDATETIME *pstDateTime);

__EXTERN U8 RunPara_GetExternMeterProtocol(U8 MeterProtocol);
__EXTERN U8 RunPara_GetInnerMeterProtocol(U8 MeterProtocol);
__EXTERN U8 RunPara_GetDefaultMeterCommPara(STMETERPARA *pstMeterPara,U8 ucInnerMeterProtocol);
__EXTERN U8 RunPara_GetMeterProtocolInfo(U8 *pucInfoBuff);

__EXTERN void RunPara_GetTerVerInfo(U8 *pucVerInfo);
__EXTERN void RunPara_GetTerInnerVerInfo(U8 *pucVerInfo);
__EXTERN void RunPara_GetTerHardwareVerInfo(U8 *pucVerInfo);
__EXTERN void RunPara_GetTerProtoolVerInfo(U8 *pucVerInfo);

__EXTERN unsigned long RunPara_Get_Relay_Boadrate(U8 ucBoadInfo);
//__EXTERN void RunPara_Get_Relay_CommInfo(U8 ucCommInfo,U8 *pucEPN,U8 *pucDataBit,U8 *pucStopBit);

__EXTERN U8 RunPara_Init_TerCommPara_Inner(STTERCOMMPARA_INNER *pstTerCommpara_Inner);
__EXTERN U8 RunPara_SetTerCommPara_Inner(STTERCOMMPARA_INNER *pstTerCommPara_Inner);
__EXTERN STTERCOMMPARA_INNER    RunPara_GetTerCommPara_Inner(void);
//__EXTERN unsigned short RunPara_GetCommMode(void);
__EXTERN unsigned short RunPara_GetReConnectTime(void);
__EXTERN U8  RunPara_GetTerCommPara_ReConnectTimes(void);
__EXTERN unsigned short RunPara_GetTerCommPara_StandbyTime(void);

//__EXTERN void SetBuyBillRec(STBUYBILLREC *pstBuyBillRec,unsigned long ulBuyBillNo,U8 ucFlashFlag);
//__EXTERN STBUYBILLREC RunPara_GetBuyBillRec_byBillNo(U8 ucBillNo);
//__EXTERN U8 RunPara_SetBuyBillRec_byBillNo(STBUYBILLREC *pstBuyBillRec,U8 ucBillNo);
__EXTERN U8  Main_DI_Login_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,U8 ucNum4);
__EXTERN U8  Main_DI_Config_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,U8 ucNum4);
__EXTERN U8  Main_DI_Taskdata_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,U8 ucNum4);

__EXTERN U8 PowOn_Para_Init_All(void);
#endif

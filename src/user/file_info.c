/******************************************************************************
 Copyright (C) 2012  R&D Institute of HuaCai Co.,Ltd.
 File Name      : file_info.c
 Description    : �汾��Ϣ
 Related Files  :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��    1.00        2011-12-18    wwllzz          the original version
******************************************************************************/
#include "..\inc\global_config.h"

//-------------------------------- �������� -----------------------------------
const U8 version_info[39] __attribute__((used)) =       //�汾��Ϣ
{
    0x30 + (VER_MAIN >> 8),                             //���汾��
    0x30 + (VER_MAIN & 0xf),
    '.',
    0x30 + (VER_PARTICULAR >> 8),                       //�ΰ汾��
    0x30 + (VER_PARTICULAR & 0xf),
    ' ',
    0x30 + ((ISSUE_YMD_V >> 20) & 0xf),                 //��������
    0x30 + ((ISSUE_YMD_V >> 16) & 0xf),
    '-',
    0x30 + ((ISSUE_YMD_V >> 12) & 0xf),
    0x30 + ((ISSUE_YMD_V >> 8) & 0xf),
    '-',
    0x30 + ((ISSUE_YMD_V >> 4) & 0xf),
    0x30 + (ISSUE_YMD_V & 0xf),

#ifdef __RELEASE_H
    'H',                                                //����
#endif  //__RELEASE_H
#ifdef __RELEASE_L
    'L',                                                //����
#endif  //__RELEASE_L

    '2',                                                //��1 �ն�2

#if CODE_VER_TYPE==H_METER_PRJ				//��ϸ�ѹ��
//    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'G', 'W', 'D', 'i', 's', '-', 'T', 'Y', 'T', 'Y', '0', '0', 'G', '0', ' '
//	#ifdef MC55_GPRS
//    ' ', ' ', ' ', ' ', 'S', '5', '2', 'i', 'G', 'W', 'D', 'i', 's', '-', 'T', 'Y', 'T', 'Y', '0', '1', 'S', '1', ' '
//	#endif//MC55_GPRS
//	#ifdef TELIT_GPRS
//    ' ', ' ', ' ', ' ', 'T', '8', '6', '8', 'G', 'W', 'D', 'i', 's', '-', 'T', 'Y', 'T', 'Y', '0', '1', 'T', '0', ' '
//	#endif//TELIT_GPRS
    ' ', ' ', ' ', ' ', 'G', 'M', 'i', 'x', 'G', 'W', 'D', 'i', 's', '-', 'T', 'Y', 'T', 'Y', '0', '1', 'G', '0', ' '
#elif CODE_VER_TYPE==WLS_CUR_PRJ			//������߲ɼ���
//    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'G', 'W', 'D', 'i', 's', '-', 'W', 'C', 'u', 'r', '0', '0', 'G', '1', ' '
//	#ifdef MC55_GPRS
//    ' ', ' ', ' ', ' ', 'S', '5', '2', 'i', 'G', 'W', 'D', 'i', 's', '-', 'W', 'C', 'u', 'r', '0', '0', 'S', '1', ' '
//	#endif//MC55_GPRS
//	#ifdef TELIT_GPRS
//    ' ', ' ', ' ', ' ', 'T', '8', '6', '8', 'G', 'W', 'D', 'i', 's', '-', 'W', 'C', 'u', 'r', '0', '0', 'T', '0', ' '
//	#endif//TELIT_GPRS
    ' ', ' ', ' ', ' ', 'G', 'M', 'i', 'x', 'G', 'W', 'D', 'i', 's', '-', 'W', 'C', 'u', 'r', '0', '0', 'G', '0', ' '
#elif CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
    //    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'G', 'W', 'M', 'd', 'l', '-', 'T', 'Y', 'T', 'Y', '0', '4', 'G', '0', ' '
    //	#ifdef MC55_GPRS
    //    ' ', ' ', ' ', ' ', 'S', '5', '2', 'i', 'G', 'W', 'M', 'd', 'l', '-', 'T', 'Y', 'T', 'Y', '0', '4', 'S', '1', ' '
    //	#endif//MC55_GPRS
    //	#ifdef TELIT_GPRS
    //    ' ', ' ', ' ', ' ', 'T', '8', '6', '8', 'G', 'W', 'M', 'd', 'l', '-', 'T', 'Y', 'T', 'Y', '0', '4', 'T', '0', ' '
    //	#endif//TELIT_GPRS
    #ifdef ZIGBEE_TEST//azh 171011 �����ʽ��ѹ��GPRSģ��
        ' ', ' ', ' ', ' ', 'G', 'M', 'i', 'x', 'G', 'W', 'M', 'd', 'l', '-', 'H', 'm', 't', 'Y', '0', '4', 'G', '0', ' '
    #else    
        ' ', ' ', ' ', ' ', 'G', 'M', 'i', 'x', 'G', 'W', 'M', 'd', 'l', '-', 'Y', 'd', 's', 'Y', '0', '4', 'G', '0', ' '  
    #endif//      
#endif
};
//azh ���8λ(�����ڶ�λ��ʼ)���������б�zzxxnnpn(��ĸ) ֻ������ͬ�� ��λ���������֧������
//zz: ����ʡ�� 2λ��ĸ ͨ������TY
//xx  �� 2λ��ĸ ȫ��ͨ������TY
//nn: ��Ʒ��� 2λ����  
//pn: Э��  1λ��ĸ+1λ���� N(����) G(����)
//azh 140918 pn���¶��� ��Ϊ�����������Ϣ�Ժ��漸λ ����GPRS��Ϣ�ӵ����2λpn
//pn: p=S:SIEMENS-MC52i,  p=T:TELIT868
//    n=0��Ƭ��1�����

static const char d[] __attribute__((used)) = {"����" __DATE__ " " __TIME__};


/******************************************************************************
 Copyright (C) 2012  R&D Institute of HuaCai Co.,Ltd.
 File Name      : data_section.c
 Description    : ������Ʊ�ʶ��Ӧ�ó���������
 Related Files  :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2011-12-18    wwllzz          the original version
******************************************************************************/
#include "..\inc\global_config.h"
//#include    "..\src\bsp\event_define_d.h"               //�й��¼���Ķ���

//-------------------------------- �������� -----------------------------------
#ifdef __RELEASE_H
const U32 RUNNING_FLAG __attribute__((section(".ARM.__at_0x1000"))) __attribute__((used)) = 2;            //Ҫ�����д���H
const U8 FW_CRC_CORRECT_FLAG __attribute__((section(".ARM.__at_0x1004"))) __attribute__((used)) = 0xF5;   //�������й���CRC��ȷ
//bit0Ϊ0��ʾ��������Ч����,
//bit1Ϊ0��ʾ��������Ч����.
//bit2Ϊ0��ʾ���������ش���CRC��֤��ȷ,
//bit3Ϊ0��ʾ���������ش���CRC��֤��ȷ.
//bit4Ϊ1��ʾ��������Ϊ����
#endif  //__RELEASE_H

#ifdef __RELEASE_L
const U32 RUNNING_FLAG __attribute__((section(".ARM.__at_0x1000"))) __attribute__((used)) = 1;            //Ҫ�����д���L
const U8 FW_CRC_CORRECT_FLAG __attribute__((section(".ARM.__at_0x1004"))) __attribute__((used)) = 0xEA;   //�������й���CRC��ȷ
//bit0Ϊ0��ʾ��������Ч����,
//bit1Ϊ0��ʾ��������Ч����.
//bit2Ϊ0��ʾ���������ش���CRC��֤��ȷ,
//bit3Ϊ0��ʾ���������ش���CRC��֤��ȷ.
//bit4Ϊ0��ʾ��������Ϊ����
#endif  //__RELEASE_L

//AZH
#pragma arm section rwdata = "RW_IRAM2"
#pragma pack(4)
//U8 Sample_Data[4096 * 2];                               //ֻ��Ҫ����4K�ڴ�, ����һ���͹���

#ifdef DISPLAY_TERMINAL_2
U32 Sample_Data[1024 * 2];
#else   //DISPLAY_TERMINAL_2
U32 Sample_Data[256 * 3 / 4];                           //Ŀǰ��Щ�ڴ��㹻

#pragma arm section rwdata

static const U32 fil[1024 - 2] __attribute__((used)) = {    //��0���
	0
};
#endif  //DISPLAY_TERMINAL_2


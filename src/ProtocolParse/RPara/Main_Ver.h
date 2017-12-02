/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_Ver.h
 Description    : �ն˰汾��Ϣ
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-08-15    azh     the original version
******************************************************************************/
#ifndef _MAIN_VER_H_
#define _MAIN_VER_H_

#ifdef ZIGBEE_TEST//azh 171011 �����ʽ��ѹ��GPRSģ��
    //�ն˰汾��Ϣ
    #define CON_TER_SOFTWAREVER_0       '1'//0x30                       //����汾��4�ֽ�
    #define CON_TER_SOFTWAREVER_1       '.'//0x32
    #define CON_TER_SOFTWAREVER_2       '0'//0x32
    #define CON_TER_SOFTWAREVER_3       '0'//0x35
    #define CON_TER_SOFTWARE_DAY        0x20//0x05//0x04                    //�ն�����������ڣ�������
    #define CON_TER_SOFTWARE_MON        0x10//0x04//0x05
    #define CON_TER_SOFTWARE_YEAR       0x17
    
    #define CON_TER_INNERVER_0          0x42                    //�ڲ��汾�� BF--
    #define CON_TER_INNERVER_1          0x46
    #define CON_TER_INNERVER_2          0x30
    #define CON_TER_INNERVER_3          0x31

	#define CON_TER_HARDWAREVER_0       '1'//0x30                       //Ӳ�����汾��4�ֽ�
	#define CON_TER_HARDWAREVER_1       '.'//0x31
	#define CON_TER_HARDWAREVER_2       '0'//0x41
	#define CON_TER_HARDWAREVER_3       '0'//0x50
	#define CON_TER_HARDWARE_DAY        0x22//0x04                  //�ն�Ӳ���������ڣ�������
	#define CON_TER_HARDWARE_MON        0x07//0x05
	#define CON_TER_HARDWARE_YEAR       0x17
#else
    //�ն˰汾��Ϣ
    #define CON_TER_SOFTWAREVER_0       '1'//0x30                       //����汾��4�ֽ�
    #define CON_TER_SOFTWAREVER_1       '.'//0x32
    #define CON_TER_SOFTWAREVER_2       '4'//0x32
    #define CON_TER_SOFTWAREVER_3       '0'//0x35
    #define CON_TER_SOFTWARE_DAY        0x11//0x05//0x04                    //�ն�����������ڣ�������
    #define CON_TER_SOFTWARE_MON        0x10//0x04//0x05
    #define CON_TER_SOFTWARE_YEAR       0x17
    
    #define CON_TER_INNERVER_0          0x42                    //�ڲ��汾�� BF--
    #define CON_TER_INNERVER_1          0x46
    #define CON_TER_INNERVER_2          0x30
    #define CON_TER_INNERVER_3          0x31

	#define CON_TER_HARDWAREVER_0       '1'//0x30                       //Ӳ�����汾��4�ֽ�
	#define CON_TER_HARDWAREVER_1       '.'//0x31
	#define CON_TER_HARDWAREVER_2       '2'//0x41
	#define CON_TER_HARDWAREVER_3       '0'//0x50
	#define CON_TER_HARDWARE_DAY        0x31//0x04                  //�ն�Ӳ���������ڣ�������
	#define CON_TER_HARDWARE_MON        0x03//0x05
	#define CON_TER_HARDWARE_YEAR       0x14
#endif//ZIGBEE_TEST




#define CON_LCD_AREA_0              'B'                     //Һ����ʾ�汾�Ÿ�����������
#define CON_LCD_AREA_1              'F'

#endif

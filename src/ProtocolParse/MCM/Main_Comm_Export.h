/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_Comm_Export.c
 Description    :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-02-05    azh     the original version
******************************************************************************/

#ifndef _MAIN_COMM_EXPORT_H_
#define _MAIN_COMM_EXPORT_H_

#define     CON_DATA_REQ1_BEG_CODE  0x2000
#define     CON_DATA_REQ1_END_CODE  0x20FF
#define     CON_DATA_REQ2_BEG_CODE  0x2100
#define     CON_DATA_REQ2_END_CODE  0x21FF
#define     CON_DATA_REQ3_BEG_CODE  0x2200
#define     CON_DATA_REQ3_END_CODE  0x22FF
#define     CON_RST_BEG_CODE        0x2500
#define     CON_RST_END_CODE        0x251F
#define     CON_ACK_BEG_CODE        0x2540
#define     CON_ACK_END_CODE        0x255F
#define     CON_TASKDATA_REQ_BEG_CODE   0x2600
#define     CON_TASKDATA_REQ_END_CODE   0x26FF
#define     CON_EXTCMD_BEG_CODE 0x2700
#define     CON_EXTCMD_END_CODE 0x27FF
#define     CON_DATATRANS_BEG_CODE  0x2800
#define     CON_DATATRANS_END_CODE  0x28FF
#define     CON_FILETRANS_BEG_CODE  0x2900
#define     CON_FILETRANS_END_CODE  0x29FF

#define CON_DATA_REQ1_F1        CON_DATA_REQ1_BEG_CODE+1    //F1    �ն˰汾��Ϣ
#define CON_DATA_REQ1_F2        CON_DATA_REQ1_BEG_CODE+2    //F2    �ն�����ʱ��
#define CON_DATA_REQ1_F3        CON_DATA_REQ1_BEG_CODE+3    //F3    �ն˲���״̬
#define CON_DATA_REQ1_F4        CON_DATA_REQ1_BEG_CODE+4    //F4    �ն�ͨ��״̬
#define CON_DATA_REQ1_F5        CON_DATA_REQ1_BEG_CODE+5    //F5    �ն˿�������״̬
#define CON_DATA_REQ1_F6        CON_DATA_REQ1_BEG_CODE+6    //F6    �ն˵�ǰ����״̬
#define CON_DATA_REQ1_F7        CON_DATA_REQ1_BEG_CODE+7    //F7    �ն��¼���������ǰֵ
#define CON_DATA_REQ1_F8        CON_DATA_REQ1_BEG_CODE+8    //F8    �ն��¼���־״̬
#define CON_DATA_REQ1_F9        CON_DATA_REQ1_BEG_CODE+9    //F9    �ն�״̬������λ��־
#define CON_DATA_REQ1_F10       CON_DATA_REQ1_BEG_CODE+10   //F10   �ն�����վ���ա���ͨ������
#define CON_DATA_REQ1_F11       CON_DATA_REQ1_BEG_CODE+11   //F11   ��ǰ�ź�ǿ��
//azh 170921
#define CON_DATA_REQ1_F12      CON_DATA_REQ1_BEG_CODE+12    //F12   ���������·���ؽ���״̬������λ��־
#define CON_DATA_REQ1_F13      CON_DATA_REQ1_BEG_CODE+13    //F13   �������ĵ����Ϣ
#define CON_DATA_REQ1_F14      CON_DATA_REQ1_BEG_CODE+14    //F14   �ļ�����δ�յ����ݶ�

#define CON_DATA_REQ1_F17       CON_DATA_REQ1_BEG_CODE+17   //F17   ��ǰ�ܼ��й�����
#define CON_DATA_REQ1_F18       CON_DATA_REQ1_BEG_CODE+18   //F18   ��ǰ�ܼ��޹�����
#define CON_DATA_REQ1_F19       CON_DATA_REQ1_BEG_CODE+19   //F19   �����ܼ��й����������ܡ�����1~M��
#define CON_DATA_REQ1_F20       CON_DATA_REQ1_BEG_CODE+20   //F20   �����ܼ��޹����������ܡ�����1~M��
#define CON_DATA_REQ1_F21       CON_DATA_REQ1_BEG_CODE+21   //F21   �����ܼ��й����������ܡ�����1~M��
#define CON_DATA_REQ1_F22       CON_DATA_REQ1_BEG_CODE+22   //F22   �����ܼ��޹����������ܡ�����1~M��
#define CON_DATA_REQ1_F23       CON_DATA_REQ1_BEG_CODE+23   //F23   �ն˵�ǰʣ��������ѣ�
#define CON_DATA_REQ1_F24       CON_DATA_REQ1_BEG_CODE+24   //F24   ��ǰ�����¸��ؿغ��ܼ��й����ʶ���ֵ
#define CON_DATA_REQ1_F25       CON_DATA_REQ1_BEG_CODE+25   //F25   ��ǰ���༰����/�޹����ʡ����������������ѹ���������������
#define CON_DATA_REQ1_F26       CON_DATA_REQ1_BEG_CODE+26   //F26   A��B��C�������ͳ�����ݼ����һ�ζ����¼
#define CON_DATA_REQ1_F27       CON_DATA_REQ1_BEG_CODE+27   //F27   ���ܱ�����ʱ�ӡ���̴����������һ�β���ʱ��
#define CON_DATA_REQ1_F28       CON_DATA_REQ1_BEG_CODE+28   //F28   �������״̬�ּ����λ��־
#define CON_DATA_REQ1_F31       CON_DATA_REQ1_BEG_CODE+31   //F31
#define CON_DATA_REQ1_F32       CON_DATA_REQ1_BEG_CODE+32   //F32
#define CON_DATA_REQ1_F33       CON_DATA_REQ1_BEG_CODE+33   //F33   ��ǰ������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
#define CON_DATA_REQ1_F34       CON_DATA_REQ1_BEG_CODE+34   //F34   ��ǰ������/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
#define CON_DATA_REQ1_F35       CON_DATA_REQ1_BEG_CODE+35   //F35   ����������/�޹��������������ʱ�䣨�ܡ�����1~M��
#define CON_DATA_REQ1_F36       CON_DATA_REQ1_BEG_CODE+36   //F36   ���·�����/�޹��������������ʱ�䣨�ܡ�����1~M��
#define CON_DATA_REQ1_F37       CON_DATA_REQ1_BEG_CODE+37   //F37   ����������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
#define CON_DATA_REQ1_F38       CON_DATA_REQ1_BEG_CODE+38   //F38   ���·�����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
#define CON_DATA_REQ1_F39       CON_DATA_REQ1_BEG_CODE+39   //F39   ����������/�޹��������������ʱ�䣨�ܡ�����1~M��
#define CON_DATA_REQ1_F40       CON_DATA_REQ1_BEG_CODE+40   //F40   ���·�����/�޹��������������ʱ�䣨�ܡ�����1~M��
#define CON_DATA_REQ1_F41       CON_DATA_REQ1_BEG_CODE+41   //F41   ���������й����������ܡ�����1~M��
#define CON_DATA_REQ1_F42       CON_DATA_REQ1_BEG_CODE+42   //F42   ���������޹����������ܡ�����1~M��
#define CON_DATA_REQ1_F43       CON_DATA_REQ1_BEG_CODE+43   //F43   ���շ����й����������ܡ�����1~M��
#define CON_DATA_REQ1_F44       CON_DATA_REQ1_BEG_CODE+44   //F44   ���շ����޹����������ܡ�����1~M��
#define CON_DATA_REQ1_F45       CON_DATA_REQ1_BEG_CODE+45   //F45   ���������й����������ܡ�����1~M��
#define CON_DATA_REQ1_F46       CON_DATA_REQ1_BEG_CODE+46   //F46   ���������޹����������ܡ�����1~M��
#define CON_DATA_REQ1_F47       CON_DATA_REQ1_BEG_CODE+47   //F47   ���·����й����������ܡ�����1~M��
#define CON_DATA_REQ1_F48       CON_DATA_REQ1_BEG_CODE+48   //F48   ���·����޹����������ܡ�����1~M��
#define CON_DATA_REQ1_F49       CON_DATA_REQ1_BEG_CODE+49   //F49   ��ǰ��ѹ��������λ��
#define CON_DATA_REQ1_F57       CON_DATA_REQ1_BEG_CODE+57   //F57   ��ǰA��B��C�����ѹ������2~N��г����Чֵ
#define CON_DATA_REQ1_F58       CON_DATA_REQ1_BEG_CODE+58   //F58   ��ǰA��B��C�����ѹ������2~N��г��������
#define CON_DATA_REQ1_F65       CON_DATA_REQ1_BEG_CODE+65   //F65   ��ǰ������Ͷ��״̬
#define CON_DATA_REQ1_F66       CON_DATA_REQ1_BEG_CODE+66   //F66   ��ǰ�������ۼƲ���Ͷ��ʱ��ʹ���
#define CON_DATA_REQ1_F67       CON_DATA_REQ1_BEG_CODE+67   //F67   ���ա����µ������ۼƲ������޹�������
#define CON_DATA_REQ1_F73       CON_DATA_REQ1_BEG_CODE+73   //F73   ֱ��ģ����ʵʱ����
#define CON_DATA_REQ1_F81       CON_DATA_REQ1_BEG_CODE+81   //F81   Сʱ�����ܼ��й�����
#define CON_DATA_REQ1_F82       CON_DATA_REQ1_BEG_CODE+82   //F82   Сʱ�����ܼ��޹�����
#define CON_DATA_REQ1_F83       CON_DATA_REQ1_BEG_CODE+83   //F83   Сʱ�����ܼ��й��ܵ�����
#define CON_DATA_REQ1_F84       CON_DATA_REQ1_BEG_CODE+84   //F84   Сʱ�����ܼ��޹��ܵ�����
#define CON_DATA_REQ1_F89       CON_DATA_REQ1_BEG_CODE+89   //F89   Сʱ�����й�����
#define CON_DATA_REQ1_F90       CON_DATA_REQ1_BEG_CODE+90   //F90   Сʱ����A���й�����
#define CON_DATA_REQ1_F91       CON_DATA_REQ1_BEG_CODE+91   //F91   Сʱ����B���й�����
#define CON_DATA_REQ1_F92       CON_DATA_REQ1_BEG_CODE+92   //F92   Сʱ����C���й�����
#define CON_DATA_REQ1_F93       CON_DATA_REQ1_BEG_CODE+93   //F93   Сʱ�����޹�����
#define CON_DATA_REQ1_F94       CON_DATA_REQ1_BEG_CODE+94   //F94   Сʱ����A���޹�����
#define CON_DATA_REQ1_F95       CON_DATA_REQ1_BEG_CODE+95   //F95   Сʱ����B���޹�����
#define CON_DATA_REQ1_F96       CON_DATA_REQ1_BEG_CODE+96   //F96   Сʱ����C���޹�����
#define CON_DATA_REQ1_F97       CON_DATA_REQ1_BEG_CODE+97   //F97   Сʱ����A���ѹ
#define CON_DATA_REQ1_F98       CON_DATA_REQ1_BEG_CODE+98   //F98   Сʱ����B���ѹ
#define CON_DATA_REQ1_F99       CON_DATA_REQ1_BEG_CODE+99   //F99   Сʱ����C���ѹ
#define CON_DATA_REQ1_F100      CON_DATA_REQ1_BEG_CODE+100  //F100  Сʱ����A�����
#define CON_DATA_REQ1_F101      CON_DATA_REQ1_BEG_CODE+101  //F101  Сʱ����B�����
#define CON_DATA_REQ1_F102      CON_DATA_REQ1_BEG_CODE+102  //F102  Сʱ����C�����
#define CON_DATA_REQ1_F103      CON_DATA_REQ1_BEG_CODE+103  //F103  Сʱ�����������
#define CON_DATA_REQ1_F105      CON_DATA_REQ1_BEG_CODE+105  //F105  Сʱ���������й��ܵ�����
#define CON_DATA_REQ1_F106      CON_DATA_REQ1_BEG_CODE+106  //F106  Сʱ���������޹��ܵ�����
#define CON_DATA_REQ1_F107      CON_DATA_REQ1_BEG_CODE+107  //F107  Сʱ���ᷴ���й��ܵ�����
#define CON_DATA_REQ1_F108      CON_DATA_REQ1_BEG_CODE+108  //F108  Сʱ���ᷴ���޹��ܵ�����
#define CON_DATA_REQ1_F109      CON_DATA_REQ1_BEG_CODE+109  //F109  Сʱ���������й��ܵ���ʾֵ
#define CON_DATA_REQ1_F110      CON_DATA_REQ1_BEG_CODE+110  //F110  Сʱ���������޹��ܵ���ʾֵ
#define CON_DATA_REQ1_F111      CON_DATA_REQ1_BEG_CODE+111  //F111  Сʱ���ᷴ���й��ܵ���ʾֵ
#define CON_DATA_REQ1_F112      CON_DATA_REQ1_BEG_CODE+112  //F112  Сʱ���ᷴ���޹��ܵ���ʾֵ
#define CON_DATA_REQ1_F113      CON_DATA_REQ1_BEG_CODE+113  //F113  Сʱ�����ܹ�������
#define CON_DATA_REQ1_F114      CON_DATA_REQ1_BEG_CODE+114  //F114  Сʱ����A�๦������
#define CON_DATA_REQ1_F115      CON_DATA_REQ1_BEG_CODE+115  //F115  Сʱ����B�๦������
#define CON_DATA_REQ1_F116      CON_DATA_REQ1_BEG_CODE+116  //F116  Сʱ����C�๦������
#define CON_DATA_REQ1_F121      CON_DATA_REQ1_BEG_CODE+121  //F121  Сʱ����ֱ��ģ����

#define CON_DATA_REQ1_F129      CON_DATA_REQ1_BEG_CODE+129
#define CON_DATA_REQ1_F130      CON_DATA_REQ1_BEG_CODE+130
#define CON_DATA_REQ1_F131      CON_DATA_REQ1_BEG_CODE+131
#define CON_DATA_REQ1_F132      CON_DATA_REQ1_BEG_CODE+132
#define CON_DATA_REQ1_F133      CON_DATA_REQ1_BEG_CODE+133
#define CON_DATA_REQ1_F134      CON_DATA_REQ1_BEG_CODE+134
#define CON_DATA_REQ1_F135      CON_DATA_REQ1_BEG_CODE+135
#define CON_DATA_REQ1_F136      CON_DATA_REQ1_BEG_CODE+136
#define CON_DATA_REQ1_F137      CON_DATA_REQ1_BEG_CODE+137
#define CON_DATA_REQ1_F138      CON_DATA_REQ1_BEG_CODE+138
#define CON_DATA_REQ1_F139      CON_DATA_REQ1_BEG_CODE+139
#define CON_DATA_REQ1_F140      CON_DATA_REQ1_BEG_CODE+140
#define CON_DATA_REQ1_F141      CON_DATA_REQ1_BEG_CODE+141
#define CON_DATA_REQ1_F142      CON_DATA_REQ1_BEG_CODE+142
#define CON_DATA_REQ1_F143      CON_DATA_REQ1_BEG_CODE+143
#define CON_DATA_REQ1_F144      CON_DATA_REQ1_BEG_CODE+144
#define CON_DATA_REQ1_F145      CON_DATA_REQ1_BEG_CODE+145
#define CON_DATA_REQ1_F146      CON_DATA_REQ1_BEG_CODE+146
#define CON_DATA_REQ1_F147      CON_DATA_REQ1_BEG_CODE+147
#define CON_DATA_REQ1_F148      CON_DATA_REQ1_BEG_CODE+148
#define CON_DATA_REQ1_F149      CON_DATA_REQ1_BEG_CODE+149
#define CON_DATA_REQ1_F150      CON_DATA_REQ1_BEG_CODE+150
#define CON_DATA_REQ1_F151      CON_DATA_REQ1_BEG_CODE+151
#define CON_DATA_REQ1_F152      CON_DATA_REQ1_BEG_CODE+152
#define CON_DATA_REQ1_F161      CON_DATA_REQ1_BEG_CODE+161
//azh 170921
#define CON_DATA_REQ1_F162      CON_DATA_REQ1_BEG_CODE+162  //F162   ���ܱ�����ʱ��
#define CON_DATA_REQ1_F177      CON_DATA_REQ1_BEG_CODE+177  //F177   ��ǰ����й�����ʾֵ���ܡ�����1��M��
#define CON_DATA_REQ1_F178      CON_DATA_REQ1_BEG_CODE+178  //F178   ��1����������й�����ʾֵ���ܡ�����1��M��

#define CON_DATA_REQ2_F1        CON_DATA_REQ2_BEG_CODE+1    //F1    ������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
#define CON_DATA_REQ2_F2        CON_DATA_REQ2_BEG_CODE+2    //F2    ������/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
#define CON_DATA_REQ2_F3        CON_DATA_REQ2_BEG_CODE+3    //F3    ������/�޹��������������ʱ�䣨�ܡ�����1~M��
#define CON_DATA_REQ2_F4        CON_DATA_REQ2_BEG_CODE+4    //F4    ������/�޹��������������ʱ�䣨�ܡ�����1~M��
#define CON_DATA_REQ2_F5        CON_DATA_REQ2_BEG_CODE+5    //F5    �����й����������ܡ�����1~M��
#define CON_DATA_REQ2_F6        CON_DATA_REQ2_BEG_CODE+6    //F6    �����޹����������ܡ�����1~M��
#define CON_DATA_REQ2_F7        CON_DATA_REQ2_BEG_CODE+7    //F7    �����й����������ܡ�����1~M��
#define CON_DATA_REQ2_F8        CON_DATA_REQ2_BEG_CODE+8    //F8    �����޹����������ܡ�����1~M��
#define CON_DATA_REQ2_F9        CON_DATA_REQ2_BEG_CODE+9    //F9    ������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
#define CON_DATA_REQ2_F10       CON_DATA_REQ2_BEG_CODE+10   //F10   ������/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
#define CON_DATA_REQ2_F11       CON_DATA_REQ2_BEG_CODE+11   //F11   ������/�޹��������������ʱ�䣨�ܡ�����1~M��
#define CON_DATA_REQ2_F12       CON_DATA_REQ2_BEG_CODE+12   //F12   ������/�޹��������������ʱ�䣨�ܡ�����1~M��
#define CON_DATA_REQ2_F17       CON_DATA_REQ2_BEG_CODE+17   //F17   ������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
#define CON_DATA_REQ2_F18       CON_DATA_REQ2_BEG_CODE+18   //F18   ������/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
#define CON_DATA_REQ2_F19       CON_DATA_REQ2_BEG_CODE+19   //F19   ������/�޹��������������ʱ�䣨�ܡ�����1~M��
#define CON_DATA_REQ2_F20       CON_DATA_REQ2_BEG_CODE+20   //F20   ������/�޹��������������ʱ�䣨�ܡ�����1~M��
#define CON_DATA_REQ2_F21       CON_DATA_REQ2_BEG_CODE+21   //F21   �����й����������ܡ�����1~M��
#define CON_DATA_REQ2_F22       CON_DATA_REQ2_BEG_CODE+22   //F22   �����޹����������ܡ�����1~M��
#define CON_DATA_REQ2_F23       CON_DATA_REQ2_BEG_CODE+23   //F23   �����й����������ܡ�����1~M��
#define CON_DATA_REQ2_F24       CON_DATA_REQ2_BEG_CODE+24   //F24   �����޹����������ܡ�����1~M��
#define CON_DATA_REQ2_F25       CON_DATA_REQ2_BEG_CODE+25   //F25   �շ�������й����ʼ�����ʱ�䡢�й�����Ϊ��ʱ��
#define CON_DATA_REQ2_F26       CON_DATA_REQ2_BEG_CODE+26   //F26   ���ܼ������������������ʱ��
#define CON_DATA_REQ2_F27       CON_DATA_REQ2_BEG_CODE+27   //F27   �յ�ѹͳ������
#define CON_DATA_REQ2_F28       CON_DATA_REQ2_BEG_CODE+28   //F28   �ղ�ƽ���Խ���ۼ�ʱ��
#define CON_DATA_REQ2_F29       CON_DATA_REQ2_BEG_CODE+29   //F29   �յ���Խ��ͳ��
#define CON_DATA_REQ2_F30       CON_DATA_REQ2_BEG_CODE+30   //F30   �����ڹ���Խ���ۼ�ʱ��
#define CON_DATA_REQ2_F31       CON_DATA_REQ2_BEG_CODE+31   //F31   �յ��ܱ���������
#define CON_DATA_REQ2_F32       CON_DATA_REQ2_BEG_CODE+32   //F32   ����
#define CON_DATA_REQ2_F33       CON_DATA_REQ2_BEG_CODE+33   //F33   �·�������й����ʼ�����ʱ�䡢�����й�����Ϊ��ʱ��
#define CON_DATA_REQ2_F34       CON_DATA_REQ2_BEG_CODE+34   //F34   �·����й��������������ʱ�䡢���й��������������ʱ��
#define CON_DATA_REQ2_F35       CON_DATA_REQ2_BEG_CODE+35   //F35   �µ�ѹͳ������
#define CON_DATA_REQ2_F36       CON_DATA_REQ2_BEG_CODE+36   //F36   �²�ƽ���Խ���ۼ�ʱ��
#define CON_DATA_REQ2_F37       CON_DATA_REQ2_BEG_CODE+37   //F37   �µ���Խ��ͳ��
#define CON_DATA_REQ2_F38       CON_DATA_REQ2_BEG_CODE+38   //F38   �����ڹ���Խ���ۼ�ʱ��
#define CON_DATA_REQ2_F41       CON_DATA_REQ2_BEG_CODE+41   //F41   ������Ͷ���ۼ�ʱ��ʹ���
#define CON_DATA_REQ2_F42       CON_DATA_REQ2_BEG_CODE+42   //F42   �ա��µ������ۼƲ������޹�������
#define CON_DATA_REQ2_F43       CON_DATA_REQ2_BEG_CODE+43   //F43   �չ������������ۼ�ʱ��
#define CON_DATA_REQ2_F44       CON_DATA_REQ2_BEG_CODE+44   //F44   �¹������������ۼ�ʱ��
#define CON_DATA_REQ2_F49       CON_DATA_REQ2_BEG_CODE+49   //F49   �ն��չ���ʱ�䡢�ո�λ�ۼƴ���
#define CON_DATA_REQ2_F50       CON_DATA_REQ2_BEG_CODE+50   //F50   �ն��տ���ͳ������
#define CON_DATA_REQ2_F51       CON_DATA_REQ2_BEG_CODE+51   //F51   �ն��¹���ʱ�䡢�¸�λ�ۼƴ���
#define CON_DATA_REQ2_F52       CON_DATA_REQ2_BEG_CODE+52   //F52   �ն��¿���ͳ������
#define CON_DATA_REQ2_F53       CON_DATA_REQ2_BEG_CODE+53   //F53   �ն�����վ��ͨ������
#define CON_DATA_REQ2_F54       CON_DATA_REQ2_BEG_CODE+54   //F54   �ն�����վ��ͨ������
#define CON_DATA_REQ2_F57       CON_DATA_REQ2_BEG_CODE+57   //F57   �ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��
#define CON_DATA_REQ2_F58       CON_DATA_REQ2_BEG_CODE+58   //F58   �ܼ������ۼ��й����������ܡ�����1~M��
#define CON_DATA_REQ2_F59       CON_DATA_REQ2_BEG_CODE+59   //F59   �ܼ������ۼ��޹����������ܡ�����1~M��
#define CON_DATA_REQ2_F60       CON_DATA_REQ2_BEG_CODE+60   //F60   �ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��
#define CON_DATA_REQ2_F61       CON_DATA_REQ2_BEG_CODE+61   //F61   �ܼ������ۼ��й����������ܡ�����1~M��
#define CON_DATA_REQ2_F62       CON_DATA_REQ2_BEG_CODE+62   //F62   �ܼ������ۼ��޹����������ܡ�����1~M��
#define CON_DATA_REQ2_F65       CON_DATA_REQ2_BEG_CODE+65   //F65   �ܼ��鳬���ʶ�ֵ�����ۼ�ʱ�䡢���ۼƵ�����
#define CON_DATA_REQ2_F66       CON_DATA_REQ2_BEG_CODE+66   //F66   �ܼ��鳬�µ�������ֵ�����ۼ�ʱ�䡢�ۼƵ�����
#define CON_DATA_REQ2_F73       CON_DATA_REQ2_BEG_CODE+73   //F73   �ܼ����й���������
#define CON_DATA_REQ2_F74       CON_DATA_REQ2_BEG_CODE+74   //F74   �ܼ����޹���������
#define CON_DATA_REQ2_F75       CON_DATA_REQ2_BEG_CODE+75   //F75   �ܼ����й�����������
#define CON_DATA_REQ2_F76       CON_DATA_REQ2_BEG_CODE+76   //F76   �ܼ����޹�����������
#define CON_DATA_REQ2_F81       CON_DATA_REQ2_BEG_CODE+81   //F81   �й���������
#define CON_DATA_REQ2_F82       CON_DATA_REQ2_BEG_CODE+82   //F82   A���й���������
#define CON_DATA_REQ2_F83       CON_DATA_REQ2_BEG_CODE+83   //F83   B���й���������
#define CON_DATA_REQ2_F84       CON_DATA_REQ2_BEG_CODE+84   //F84   C���й���������
#define CON_DATA_REQ2_F85       CON_DATA_REQ2_BEG_CODE+85   //F85   �޹���������
#define CON_DATA_REQ2_F86       CON_DATA_REQ2_BEG_CODE+86   //F86   A���޹���������
#define CON_DATA_REQ2_F87       CON_DATA_REQ2_BEG_CODE+87   //F87   B���޹���������
#define CON_DATA_REQ2_F88       CON_DATA_REQ2_BEG_CODE+88   //F88   C���޹���������
#define CON_DATA_REQ2_F89       CON_DATA_REQ2_BEG_CODE+89   //F89   A���ѹ����
#define CON_DATA_REQ2_F90       CON_DATA_REQ2_BEG_CODE+90   //F90   B���ѹ����
#define CON_DATA_REQ2_F91       CON_DATA_REQ2_BEG_CODE+91   //F91   C���ѹ����
#define CON_DATA_REQ2_F92       CON_DATA_REQ2_BEG_CODE+92   //F92   A���������
#define CON_DATA_REQ2_F93       CON_DATA_REQ2_BEG_CODE+93   //F93   B���������
#define CON_DATA_REQ2_F94       CON_DATA_REQ2_BEG_CODE+94   //F94   C���������
#define CON_DATA_REQ2_F95       CON_DATA_REQ2_BEG_CODE+95   //F95   �����������
#define CON_DATA_REQ2_F97       CON_DATA_REQ2_BEG_CODE+97   //F97   �����й��ܵ�����
#define CON_DATA_REQ2_F98       CON_DATA_REQ2_BEG_CODE+98   //F98   �����޹��ܵ�����
#define CON_DATA_REQ2_F99       CON_DATA_REQ2_BEG_CODE+99   //F99   �����й��ܵ�����
#define CON_DATA_REQ2_F100      CON_DATA_REQ2_BEG_CODE+100  //F100  �����޹��ܵ�����
#define CON_DATA_REQ2_F101      CON_DATA_REQ2_BEG_CODE+101  //F101  �����й��ܵ���ʾֵ
#define CON_DATA_REQ2_F102      CON_DATA_REQ2_BEG_CODE+102  //F102  �����޹��ܵ���ʾֵ
#define CON_DATA_REQ2_F103      CON_DATA_REQ2_BEG_CODE+103  //F103  �����й��ܵ���ʾֵ
#define CON_DATA_REQ2_F104      CON_DATA_REQ2_BEG_CODE+104  //F104  �����޹��ܵ���ʾֵ
#define CON_DATA_REQ2_F105      CON_DATA_REQ2_BEG_CODE+105  //F105  �ܹ�������
#define CON_DATA_REQ2_F106      CON_DATA_REQ2_BEG_CODE+106  //F106  A�๦������
#define CON_DATA_REQ2_F107      CON_DATA_REQ2_BEG_CODE+107  //F107  B�๦������
#define CON_DATA_REQ2_F108      CON_DATA_REQ2_BEG_CODE+108  //F108  C�๦������
#define CON_DATA_REQ2_F109      CON_DATA_REQ2_BEG_CODE+109
#define CON_DATA_REQ2_F110      CON_DATA_REQ2_BEG_CODE+110
#define CON_DATA_REQ2_F113      CON_DATA_REQ2_BEG_CODE+113  //F113  A��2~19��г�����������ֵ������ʱ��
#define CON_DATA_REQ2_F114      CON_DATA_REQ2_BEG_CODE+114  //F114  B��2~19��г�����������ֵ������ʱ��
#define CON_DATA_REQ2_F115      CON_DATA_REQ2_BEG_CODE+115  //F115  C��2~19��г�����������ֵ������ʱ��
#define CON_DATA_REQ2_F116      CON_DATA_REQ2_BEG_CODE+116  //F116  A��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
#define CON_DATA_REQ2_F117      CON_DATA_REQ2_BEG_CODE+117  //F117  B��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
#define CON_DATA_REQ2_F118      CON_DATA_REQ2_BEG_CODE+118  //F118  C��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
#define CON_DATA_REQ2_F121      CON_DATA_REQ2_BEG_CODE+121  //F121  A��г��Խ����ͳ������
#define CON_DATA_REQ2_F122      CON_DATA_REQ2_BEG_CODE+122  //F122  B��г��Խ����ͳ������
#define CON_DATA_REQ2_F123      CON_DATA_REQ2_BEG_CODE+123  //F123  C��г��Խ����ͳ������
#define CON_DATA_REQ2_F129      CON_DATA_REQ2_BEG_CODE+129  //F129  ֱ��ģ����Խ�����ۼ�ʱ�䡢���/��Сֵ������ʱ��
#define CON_DATA_REQ2_F130      CON_DATA_REQ2_BEG_CODE+130  //F130  ֱ��ģ����Խ�����ۼ�ʱ�䡢���/��Сֵ������ʱ��
#define CON_DATA_REQ2_F138      CON_DATA_REQ2_BEG_CODE+138  //F138  ֱ��ģ������������
#define CON_DATA_REQ2_F145      CON_DATA_REQ2_BEG_CODE+145  //F145  һ�����޹��ܵ���ʾֵ����
#define CON_DATA_REQ2_F146      CON_DATA_REQ2_BEG_CODE+146  //F146  �������޹��ܵ���ʾֵ����
#define CON_DATA_REQ2_F147      CON_DATA_REQ2_BEG_CODE+147  //F147  �������޹��ܵ���ʾֵ����
#define CON_DATA_REQ2_F148      CON_DATA_REQ2_BEG_CODE+148  //F148  �������޹��ܵ���ʾֵ����

#define CON_DATA_REQ2_F153      CON_DATA_REQ2_BEG_CODE+153  //F148  �������޹��ܵ���ʾֵ����
#define CON_DATA_REQ2_F154      CON_DATA_REQ2_BEG_CODE+154  //F148  �������޹��ܵ���ʾֵ����
#define CON_DATA_REQ2_F155      CON_DATA_REQ2_BEG_CODE+155  //F148  �������޹��ܵ���ʾֵ����
#define CON_DATA_REQ2_F156      CON_DATA_REQ2_BEG_CODE+156  //F148  �������޹��ܵ���ʾֵ����
#define CON_DATA_REQ2_F157      CON_DATA_REQ2_BEG_CODE+157  //F148  �������޹��ܵ���ʾֵ����
#define CON_DATA_REQ2_F158      CON_DATA_REQ2_BEG_CODE+158  //F148  �������޹��ܵ���ʾֵ����
#define CON_DATA_REQ2_F159      CON_DATA_REQ2_BEG_CODE+159  //F148  �������޹��ܵ���ʾֵ����
#define CON_DATA_REQ2_F160      CON_DATA_REQ2_BEG_CODE+160  //F148  �������޹��ܵ���ʾֵ����

#define CON_DATA_REQ2_F161      CON_DATA_REQ2_BEG_CODE+161
#define CON_DATA_REQ2_F162      CON_DATA_REQ2_BEG_CODE+162
#define CON_DATA_REQ2_F163      CON_DATA_REQ2_BEG_CODE+163
#define CON_DATA_REQ2_F164      CON_DATA_REQ2_BEG_CODE+164
#define CON_DATA_REQ2_F165      CON_DATA_REQ2_BEG_CODE+165
#define CON_DATA_REQ2_F166      CON_DATA_REQ2_BEG_CODE+166
#define CON_DATA_REQ2_F167      CON_DATA_REQ2_BEG_CODE+167
#define CON_DATA_REQ2_F168      CON_DATA_REQ2_BEG_CODE+168
#define CON_DATA_REQ2_F169      CON_DATA_REQ2_BEG_CODE+169
#define CON_DATA_REQ2_F170      CON_DATA_REQ2_BEG_CODE+170
#define CON_DATA_REQ2_F171      CON_DATA_REQ2_BEG_CODE+171
#define CON_DATA_REQ2_F172      CON_DATA_REQ2_BEG_CODE+172
#define CON_DATA_REQ2_F173      CON_DATA_REQ2_BEG_CODE+173
#define CON_DATA_REQ2_F174      CON_DATA_REQ2_BEG_CODE+174
#define CON_DATA_REQ2_F175      CON_DATA_REQ2_BEG_CODE+175
#define CON_DATA_REQ2_F176      CON_DATA_REQ2_BEG_CODE+176
#define CON_DATA_REQ2_F177      CON_DATA_REQ2_BEG_CODE+177
#define CON_DATA_REQ2_F178      CON_DATA_REQ2_BEG_CODE+178
#define CON_DATA_REQ2_F179      CON_DATA_REQ2_BEG_CODE+179
#define CON_DATA_REQ2_F180      CON_DATA_REQ2_BEG_CODE+180
#define CON_DATA_REQ2_F181      CON_DATA_REQ2_BEG_CODE+181
#define CON_DATA_REQ2_F182      CON_DATA_REQ2_BEG_CODE+182
#define CON_DATA_REQ2_F183      CON_DATA_REQ2_BEG_CODE+183
#define CON_DATA_REQ2_F184      CON_DATA_REQ2_BEG_CODE+184
#define CON_DATA_REQ2_F185      CON_DATA_REQ2_BEG_CODE+185
#define CON_DATA_REQ2_F186      CON_DATA_REQ2_BEG_CODE+186
#define CON_DATA_REQ2_F187      CON_DATA_REQ2_BEG_CODE+187
#define CON_DATA_REQ2_F188      CON_DATA_REQ2_BEG_CODE+188
#define CON_DATA_REQ2_F189      CON_DATA_REQ2_BEG_CODE+189
#define CON_DATA_REQ2_F190      CON_DATA_REQ2_BEG_CODE+190
#define CON_DATA_REQ2_F191      CON_DATA_REQ2_BEG_CODE+191
#define CON_DATA_REQ2_F192      CON_DATA_REQ2_BEG_CODE+192
#define CON_DATA_REQ2_F193      CON_DATA_REQ2_BEG_CODE+193
#define CON_DATA_REQ2_F194      CON_DATA_REQ2_BEG_CODE+194
#define CON_DATA_REQ2_F195      CON_DATA_REQ2_BEG_CODE+195
#define CON_DATA_REQ2_F196      CON_DATA_REQ2_BEG_CODE+196
//azh 170925
#define CON_DATA_REQ2_F219      CON_DATA_REQ2_BEG_CODE+219//F219	��������������������ݵ�Ԫ��ʽ

#define CON_DATA_REQ3_F1        CON_DATA_REQ3_BEG_CODE+1    //F1    ��Ҫ�¼�
#define CON_DATA_REQ3_F2        CON_DATA_REQ3_BEG_CODE+2    //F2    һ���¼�

#define CON_TASKDATA_REQ_F1     CON_TASKDATA_REQ_BEG_CODE+1 //F1    1������
#define CON_TASKDATA_REQ_F2		CON_TASKDATA_REQ_BEG_CODE+2	//F2	2������//CON_TASKDATA_REQ_END_CODE+2	//F2	2������

#define CON_ACK_F1              CON_ACK_BEG_CODE+1          //F1    ȫ��ȷ��
#define CON_ACK_F2              CON_ACK_BEG_CODE+2          //F2    ȫ������
#define CON_ACK_F3              CON_ACK_BEG_CODE+3          //F3    �����������ȷ�Ϸ���

#define CON_RST_F1_HARD         CON_RST_BEG_CODE+1          //F1    Ӳ����ʼ��
#define CON_RST_F2_DATA         CON_RST_BEG_CODE+2          //F2    ��������ʼ��
#define CON_RST_F3_ALL          CON_RST_BEG_CODE+3          //F3    ������ȫ����������ʼ��
#define CON_RST_F4_EXCECOMM     CON_RST_BEG_CODE+4          //��ͨѶ����֮������ݳ�ʼ��

#define CON_EXTCMD_F1           CON_EXTCMD_BEG_CODE+1       //F1 �ڴ��
#define CON_EXTCMD_F2           CON_EXTCMD_BEG_CODE+2       //F2 �ڴ�д
#define CON_EXTCMD_F3           CON_EXTCMD_BEG_CODE+3       //F3 DATAFLASH��
#define CON_EXTCMD_F4           CON_EXTCMD_BEG_CODE+4       //F4 DATAFLASHд
#define CON_EXTCMD_F5           CON_EXTCMD_BEG_CODE+5       //F5 ���Ź����ԣ��ó��������ѭ��
#define CON_EXTCMD_F6           CON_EXTCMD_BEG_CODE+6       //F6 �ֻ�ģ��ͨѶ���
#define CON_EXTCMD_F7           CON_EXTCMD_BEG_CODE+7       //F7 �ն�״̬���
#define CON_EXTCMD_F10          CON_EXTCMD_BEG_CODE+10      //F10 �ն˵�ַ10/16�����л�
#define CON_EXTCMD_F11          CON_EXTCMD_BEG_CODE+11      //F11 ������Ϣ��ȡ
#define CON_EXTCMD_F21          CON_EXTCMD_BEG_CODE+21      //F21 GR�����
#define CON_EXTCMD_F22          CON_EXTCMD_BEG_CODE+22      //F22 GR��������
#define CON_EXTCMD_F23          CON_EXTCMD_BEG_CODE+23      //F23 GR������Ϣ��
#define CON_EXTCMD_F24          CON_EXTCMD_BEG_CODE+24      //F24 GR������Ϣ����
#define CON_EXTCMD_F31          CON_EXTCMD_BEG_CODE+31      //F31 ������������
#define CON_EXTCMD_F33          CON_EXTCMD_BEG_CODE+33      //F33 ����׶���������ȫ
#define CON_EXTCMD_F40          CON_EXTCMD_BEG_CODE+40      //F40 �ڱ���������
#define CON_EXTCMD_F41          CON_EXTCMD_BEG_CODE+41      //F41 �ڱ����ݶ�ȡ
#define CON_EXTCMD_F51          CON_EXTCMD_BEG_CODE+51      //F51 �ն˵�ַ�������Ŷ�
#define CON_EXTCMD_F52          CON_EXTCMD_BEG_CODE+52      //F52 �ն���Ŷ�
#define CON_EXTCMD_F53          CON_EXTCMD_BEG_CODE+53      //F53 �ն˵�ַ��������д
#define CON_EXTCMD_F54          CON_EXTCMD_BEG_CODE+54      //F54 �ն����д
#define CON_EXTCMD_F71          CON_EXTCMD_BEG_CODE+71      //F71 ����������
#define CON_EXTCMD_F72          CON_EXTCMD_BEG_CODE+72      //F72 �޸�ʱ��
#define CON_EXTCMD_F73          CON_EXTCMD_BEG_CODE+73      //F73 ��ѯͨѶ������
#define CON_EXTCMD_F74          CON_EXTCMD_BEG_CODE+74      //F74 �޸�ͨѶ������
#define CON_EXTCMD_F75          CON_EXTCMD_BEG_CODE+75      //F75 ��ȡҺ��������ʾ����
#define CON_EXTCMD_F76          CON_EXTCMD_BEG_CODE+76      //F76 ��ȡҺ��������ʾ����
#define CON_EXTCMD_F77          CON_EXTCMD_BEG_CODE+77      //F77 ��ȡҺ��������ʾ����
#define CON_EXTCMD_F78          CON_EXTCMD_BEG_CODE+78      //F78 �޸�Һ��������ʾ����
#define CON_EXTCMD_F79          CON_EXTCMD_BEG_CODE+79      //F79 �޸�Һ��������ʾ����
#define CON_EXTCMD_F80          CON_EXTCMD_BEG_CODE+80      //F80 �޸�Һ��������ʾ����

#define CON_EXTCMD_F81          CON_EXTCMD_BEG_CODE+81      //F81 ����SIM����pin
#define CON_EXTCMD_F82          CON_EXTCMD_BEG_CODE+82      //F82 �޸�SIM����pin
#define CON_EXTCMD_F83          CON_EXTCMD_BEG_CODE+83      //F83 ��PUK����SIM����pin
#define CON_EXTCMD_F84          CON_EXTCMD_BEG_CODE+84      //F84 SIM����pin���״̬
#define CON_EXTCMD_F85          CON_EXTCMD_BEG_CODE+85      //F85 GSM/GPRS�л�
#define CON_EXTCMD_F86          CON_EXTCMD_BEG_CODE+86      //F86 �����ʿ��ƻָ�ʱ�䳤��
#define CON_EXTCMD_F87          CON_EXTCMD_BEG_CODE+87      //F87 д���ʿ��ƻָ�ʱ�䳤��
#define CON_EXTCMD_F89          CON_EXTCMD_BEG_CODE+89      //F89 �����ʿ���Խ��ȷ��ʱ��
#define CON_EXTCMD_F90          CON_EXTCMD_BEG_CODE+90      //F90 д���ʿ���Խ��ȷ��ʱ��
#define CON_EXTCMD_F91          CON_EXTCMD_BEG_CODE+91      //F91 ���Զ����������
#define CON_EXTCMD_F92          CON_EXTCMD_BEG_CODE+92      //F92 д�Զ����������
#define CON_EXTCMD_F93          CON_EXTCMD_BEG_CODE+93      //F93 ��ȡ�ն˵�ѹ��λС��������λС��
#define CON_EXTCMD_F94          CON_EXTCMD_BEG_CODE+94      //F94 �����ʼ��
#define CON_EXTCMD_F95          CON_EXTCMD_BEG_CODE+95      //F95 8025ʱ�ӽ���
#define CON_EXTCMD_F96          CON_EXTCMD_BEG_CODE+96      //F96 Һ���Աȶ�����
#define CON_EXTCMD_F97          CON_EXTCMD_BEG_CODE+97      //F97 ���ñ���ͨѸ��������
#define CON_EXTCMD_F98          CON_EXTCMD_BEG_CODE+98      //F98 ��ȡ�����û���������
#define CON_EXTCMD_F99          CON_EXTCMD_BEG_CODE+99      //F99 ����GPRS�����PDP�û���������
#define CON_EXTCMD_F100         CON_EXTCMD_BEG_CODE+100     //F100 ��ȡGPRS�����PDP�û���������
#define CON_EXTCMD_F101         CON_EXTCMD_BEG_CODE+101     //F101 ����GPRS�������ʱ��
#define CON_EXTCMD_F102         CON_EXTCMD_BEG_CODE+102     //F102 ��ȡGPRS�������ʱ��
#define CON_EXTCMD_F109         CON_EXTCMD_BEG_CODE+109     //F109 �����ն˹���ģʽ
#define CON_EXTCMD_F110         CON_EXTCMD_BEG_CODE+110     //F110 ��ȡ�ն˹���ģʽ
#define CON_EXTCMD_F111         CON_EXTCMD_BEG_CODE+111     //F111 ����CT����
#define CON_EXTCMD_F112         CON_EXTCMD_BEG_CODE+112     //F112 ��ȡCT����
#define CON_EXTCMD_F113         CON_EXTCMD_BEG_CODE+113     //F113 ������ѧϰ��Ч��־
#define CON_EXTCMD_F114         CON_EXTCMD_BEG_CODE+114     //F114 ��ȡ��ѧϰ��Ч��־
#define CON_EXTCMD_F115         CON_EXTCMD_BEG_CODE+115     //F115 ���ö��β࿪·��ֵ
#define CON_EXTCMD_F116         CON_EXTCMD_BEG_CODE+116     //F116 ��ȡ���β࿪·��ֵ
#define CON_EXTCMD_F117         CON_EXTCMD_BEG_CODE+117     //F117 ����һ�β��·��ֵ
#define CON_EXTCMD_F118         CON_EXTCMD_BEG_CODE+118     //F118 ��ȡһ�β��·��ֵ
#define CON_EXTCMD_F119         CON_EXTCMD_BEG_CODE+119     //F119 ��ȡ�迹ֵ
#define CON_EXTCMD_F120         CON_EXTCMD_BEG_CODE+120     //F120 ����AES������Կ
#define CON_EXTCMD_F121         CON_EXTCMD_BEG_CODE+121     //F121 ��ȡAES������Կ
#define CON_EXTCMD_F122         CON_EXTCMD_BEG_CODE+122     //F122 ��ȡ������������ն˴��ڱ���״̬
#define CON_EXTCMD_F123         CON_EXTCMD_BEG_CODE+123     //F123 ��ȡ�¶�

#define CON_EXTCMD_F124         CON_EXTCMD_BEG_CODE+124     //F124 ��ȡУ��EEPROM�Ĳ���
#define CON_EXTCMD_F125         CON_EXTCMD_BEG_CODE+125     //F125 ����У��EEPROM�Ĳ���
#define CON_EXTCMD_F126         CON_EXTCMD_BEG_CODE+126     //�����¼����


#define CON_DATATRANS_F1        CON_DATATRANS_BEG_CODE+1    //F1 ת������
#define CON_DATATRANS_F9        CON_DATATRANS_BEG_CODE+9
#define CON_DATATRANS_F10       CON_DATATRANS_BEG_CODE+10
#define CON_DATATRANS_F11       CON_DATATRANS_BEG_CODE+11
#define CON_FILETRANS_F1        CON_FILETRANS_BEG_CODE+1    //F1 �ļ�ת��
#define CON_FILETRANS_F100      CON_FILETRANS_BEG_CODE+100  //F100 ������װ����

#define     CON_MAINCOMMINFO_NUM    20
#define     CON_INVALID_SEQNO       0xFF
typedef struct _STMAINCOMMINFO
{
    unsigned char ucSEQ;
    unsigned char ucMSA;
    unsigned char ucTPvInfo[6];
}STMAINCOMMINFO;

typedef struct _STMAINCOMMINFOLIST
{
    unsigned char   ucSeqNo[CON_MAINCOMMINFO_NUM];
    STMAINCOMMINFO  stMainCommInfo[CON_MAINCOMMINFO_NUM];
}STMAINCOMMINFOLIST;


__EXTERN unsigned short     g_usNeedDebugCommInfo;              //��Ҫ��ʾ������Ϣ
__EXTERN STRMSENDTASKLIST	stRMSendTaskList;				//������ʱ����
//---------------------------------------------------------------------
__EXTERN unsigned char Init_Main_Comm(void);
//���մ��������
__EXTERN unsigned char Main_Recv_Proc(unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char ucMsgSource);
//���յ��ı���֡��ʽ���
__EXTERN unsigned char Main_Protocol_CheckFrame(unsigned char *pucRecvBuffer,unsigned short usLen);
//���н��ͣ���֡ͷ֡β
__EXTERN unsigned char Main_SendExplain(unsigned char ucMsgType,unsigned char *pucSrcBuffer,unsigned char *pucSendBuffer,unsigned short *pusLen,STMAINCOMMINFO  *pstMainCommInfo,unsigned char ucUpActive,unsigned char ucConFlag,unsigned char ucFrameFlag);

//��д����
__EXTERN unsigned char Main_FillUpData(unsigned char *pucSendBuffer,unsigned short usLen,unsigned char ucMsgSource,unsigned char ucLinkFN);
//Զ��ͨ�Ž��մ���

__EXTERN void TerSeq_SetCount(unsigned char ucCount);
__EXTERN void Main_Set_MsgSource(unsigned char ucMsgSource);

__EXTERN unsigned char Main_Get_MsgSource(void);
__EXTERN unsigned char Check_CurrentLoginSeq_IsOk(void);

__EXTERN unsigned char Main_GetCommInfoList(unsigned char ucSeqNo,STMAINCOMMINFO *pstMainCommInfo);


__EXTERN unsigned long GetPQRate_Var4B_01(unsigned long lPower,unsigned long lVar); //�õ���������ֵ����ʽxxx.xxx
__EXTERN unsigned long Calc_In(unsigned long ulVala,unsigned long ulValb,unsigned long ulValc);

__EXTERN unsigned char Main_SendComm_DownloadProgram(unsigned char *pucSrcBuf,unsigned short usLen,unsigned char ucMsgSource,unsigned char ucFlag);

__EXTERN unsigned short SetMainComm_Buf_DateTime_YMDHMS(unsigned char *pucBuffer,unsigned short usPos,STDATETIME stDateTime);
__EXTERN unsigned short SetMainComm_Buf_DateTime_YMDHM(unsigned char *pucBuffer,unsigned short usPos,STDATETIME stDateTime);
__EXTERN unsigned short SetMainComm_Buf_DateTime_MDHM(unsigned char *pucBuffer,unsigned short usPos,STDATETIME stDateTime);
__EXTERN unsigned short SetMainComm_Buf_DateTime_HMS(unsigned char *pucBuffer,unsigned short usPos,STDATETIME stDateTime);
__EXTERN unsigned short SetMainComm_Buf_DateTime_DHM(unsigned char *pucBuffer,unsigned short usPos,STDATETIME stDateTime);
__EXTERN unsigned short SetMainComm_Buf_DateTime_YMD(unsigned char *pucBuffer,unsigned short usPos,STDATETIME stDateTime);
__EXTERN unsigned short SetMainComm_Buf_DateTime_YMDW(unsigned char *pucBuffer,unsigned short usPos,STDATETIME stDateTime);
__EXTERN unsigned short SetMainComm_Buf_DateTime_YM(unsigned char *pucBuffer,unsigned short usPos,STDATETIME stDateTime);

__EXTERN unsigned short  SetMainComm_Buf_lVal_2B(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal);
__EXTERN unsigned short  SetMainComm_Buf_lVal_3B(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal);
__EXTERN unsigned short  SetMainComm_Buf_lVal_4B(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal);
//__EXTERN unsigned short  SetMainComm_Buf_lVal_2B_DT07(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal);
__EXTERN unsigned short  SetMainComm_Buf_lVal_5B_DT14(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal);
__EXTERN unsigned short  SetMainComm_Buf_lVal_4B_DT13(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal);
__EXTERN unsigned short  SetMainComm_Buf_lVal_3B_DT09(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal,unsigned char ucSign);
__EXTERN unsigned short  SetMainComm_Buf_lVal_2B_DT05(unsigned char *pucBuffer,unsigned short usPos,short sVal);
__EXTERN unsigned short  SetMainComm_Buf_lVal_2B_Sign(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal,unsigned char ucSign);
__EXTERN unsigned short     SetMainComm_Buf_lVal_3B_Sign(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal,unsigned char ucSign);
__EXTERN unsigned char Main_GetXishu(unsigned char *ucBuffer,unsigned short *pusVal);

__EXTERN unsigned char Check_Data_DT05(unsigned char *pucData);

__EXTERN unsigned char Check_Data_DT07(unsigned char *pucData);
__EXTERN unsigned char Check_Data_DT23(unsigned char *pucData);

// __EXTERN unsigned char   Main_GetDTList(U8 *pucRecvBuffer, U8 *pucDTList);
__EXTERN U8 Main_GetDADTList(U8 *pucRecvBuffer, U16 *pusDAList, U8 *pucDTList);
__EXTERN unsigned char  Main_GetItemLabelList(unsigned char ucRepType,unsigned char *pucDTList,unsigned short *pusItemLabelList);
// __EXTERN U8  Main_GetDAList(U8 *pucRecvBuffer, U16 *pusDAList);
__EXTERN unsigned char  Get_DA_by_MPNo(unsigned char ucMPNo,unsigned char *pucDA1,unsigned char *pucDA2);

__EXTERN unsigned char  Get_DT_by_ItemLabel(unsigned char ucRepType,unsigned short usItemLabel,unsigned char *pucDT1,unsigned char *pucDT2);
__EXTERN unsigned short GetItemLabelbyDT(unsigned char ucRepType,unsigned char ucDT);

__EXTERN STDATETIME GetLastOneHourStartTime(unsigned char ucMinOffset);
//__EXTERN STDATETIME GetLastOneHourStartTime_Diff(unsigned char ucMinOffset);
__EXTERN unsigned char GetFreeseMByMPNo(unsigned char ucMPNo);
__EXTERN unsigned char GetFreeseMByGroupNo(unsigned char ucMPNo);
__EXTERN void Adjust_Curve_Td_Starttime(unsigned char *pucBuff,unsigned char ucM);

__EXTERN unsigned char Main_Get_ReqData_1_F1(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);   //F1    �ն˰汾��Ϣ
__EXTERN unsigned char Main_Get_ReqData_1_F2(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);   //F2    �ն�����ʱ��
__EXTERN unsigned char Main_Get_ReqData_1_F3(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);   //F3    �ն˲���״̬
__EXTERN unsigned char Main_Get_ReqData_1_F4(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);   //F4    �ն�ͨ��״̬
__EXTERN unsigned char Main_Get_ReqData_1_F5(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);   //F5    �ն˿�������״̬
__EXTERN unsigned char Main_Get_ReqData_1_F6(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);   //F6    �ն˵�ǰ����״̬
__EXTERN unsigned char Main_Get_ReqData_1_F7(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);   //F7    �ն��¼���������ǰֵ
__EXTERN unsigned char Main_Get_ReqData_1_F8(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);   //F8    �ն��¼���־״̬
__EXTERN unsigned char Main_Get_ReqData_1_F9(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);   //F9    �ն�״̬������λ��־
__EXTERN unsigned char Main_Get_ReqData_1_F10(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F10   �ն�����վ���ա���ͨ������
__EXTERN unsigned char Main_Get_ReqData_1_F11(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F11   ��ǰ�ź�ǿ��
//azh 170921
__EXTERN unsigned char Main_Get_ReqData_1_F12(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F12   ���������·���ؽ���״̬������λ��־
__EXTERN unsigned char Main_Get_ReqData_1_F13(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F13   �������ĵ����Ϣ
__EXTERN unsigned char Main_Get_ReqData_1_F14(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F14   �ļ�����δ�յ����ݶ�

__EXTERN unsigned char Main_Get_ReqData_1_F17(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F17   ��ǰ�ܼ��й�����
__EXTERN unsigned char Main_Get_ReqData_1_F18(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F18   ��ǰ�ܼ��޹�����
__EXTERN unsigned char Main_Get_ReqData_1_F19(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F19   �����ܼ��й����������ܡ�����1~M��
__EXTERN unsigned char Main_Get_ReqData_1_F20(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F20   �����ܼ��޹����������ܡ�����1~M��
__EXTERN unsigned char Main_Get_ReqData_1_F21(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F21   �����ܼ��й����������ܡ�����1~M��
__EXTERN unsigned char Main_Get_ReqData_1_F22(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F22   �����ܼ��޹����������ܡ�����1~M��
__EXTERN unsigned char Main_Get_ReqData_1_F23(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F23   �ն˵�ǰʣ��������ѣ�
__EXTERN unsigned char Main_Get_ReqData_1_F24(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F24   ��ǰ�����¸��ؿغ��ܼ��й����ʶ���ֵ
__EXTERN unsigned char Main_Get_ReqData_1_F25(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F25   ��ǰ���༰����/�޹����ʡ����������������ѹ���������������
__EXTERN unsigned char Main_Get_ReqData_1_F26(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F26   A��B��C�������ͳ�����ݼ����һ�ζ����¼
__EXTERN unsigned char Main_Get_ReqData_1_F27(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F27   ���ܱ�����ʱ�ӡ���̴����������һ�β���ʱ��
__EXTERN unsigned char Main_Get_ReqData_1_F28(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F28   �������״̬�ּ����λ��־
__EXTERN unsigned char Main_Get_ReqData_1_F31(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F32(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);

__EXTERN unsigned char Main_Get_ReqData_1_F33(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F33   ��ǰ������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
__EXTERN unsigned char Main_Get_ReqData_1_F34(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F34   ��ǰ������/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
__EXTERN unsigned char Main_Get_ReqData_1_F35(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F35   ����������/�޹��������������ʱ�䣨�ܡ�����1~M��
__EXTERN unsigned char Main_Get_ReqData_1_F36(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F36   ���·�����/�޹��������������ʱ�䣨�ܡ�����1~M��
__EXTERN unsigned char Main_Get_ReqData_1_F37(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F37   ����������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
__EXTERN unsigned char Main_Get_ReqData_1_F38(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F38   ���·�����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
__EXTERN unsigned char Main_Get_ReqData_1_F39(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F39   ����������/�޹��������������ʱ�䣨�ܡ�����1~M��
__EXTERN unsigned char Main_Get_ReqData_1_F40(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F40   ���·�����/�޹��������������ʱ�䣨�ܡ�����1~M��
__EXTERN unsigned char Main_Get_ReqData_1_F41(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F41   ���������й����������ܡ�����1~M��
__EXTERN unsigned char Main_Get_ReqData_1_F42(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F42   ���������޹����������ܡ�����1~M��
__EXTERN unsigned char Main_Get_ReqData_1_F43(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F43   ���շ����й����������ܡ�����1~M��
__EXTERN unsigned char Main_Get_ReqData_1_F44(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F44   ���շ����޹����������ܡ�����1~M��
__EXTERN unsigned char Main_Get_ReqData_1_F45(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F45   ���������й����������ܡ�����1~M��
__EXTERN unsigned char Main_Get_ReqData_1_F46(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F46   ���������޹����������ܡ�����1~M��
__EXTERN unsigned char Main_Get_ReqData_1_F47(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F47   ���·����й����������ܡ�����1~M��
__EXTERN unsigned char Main_Get_ReqData_1_F48(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F48   ���·����޹����������ܡ�����1~M��
__EXTERN unsigned char Main_Get_ReqData_1_F49(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F49   ��ǰ��ѹ��������λ��
__EXTERN unsigned char Main_Get_ReqData_1_F57(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F57   ��ǰA��B��C�����ѹ������2~N��г����Чֵ
__EXTERN unsigned char Main_Get_ReqData_1_F58(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F58   ��ǰA��B��C�����ѹ������2~N��г��������
__EXTERN unsigned char Main_Get_ReqData_1_F65(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F65   ��ǰ������Ͷ��״̬
__EXTERN unsigned char Main_Get_ReqData_1_F66(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F66   ��ǰ�������ۼƲ���Ͷ��ʱ��ʹ���
__EXTERN unsigned char Main_Get_ReqData_1_F67(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F67   ���ա����µ������ۼƲ������޹�������
__EXTERN unsigned char Main_Get_ReqData_1_F73(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F73   ֱ��ģ����ʵʱ����
__EXTERN unsigned char Main_Get_ReqData_1_F81(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F81   Сʱ�����ܼ��й�����
__EXTERN unsigned char Main_Get_ReqData_1_F82(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F82   Сʱ�����ܼ��޹�����
__EXTERN unsigned char Main_Get_ReqData_1_F83(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F83   Сʱ�����ܼ��й��ܵ�����
__EXTERN unsigned char Main_Get_ReqData_1_F84(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F84   Сʱ�����ܼ��޹��ܵ�����
__EXTERN unsigned char Main_Get_ReqData_1_F89(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F89   Сʱ�����й�����
__EXTERN unsigned char Main_Get_ReqData_1_F90(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F90   Сʱ����A���й�����
__EXTERN unsigned char Main_Get_ReqData_1_F91(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F91   Сʱ����B���й�����
__EXTERN unsigned char Main_Get_ReqData_1_F92(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F92   Сʱ����C���й�����
__EXTERN unsigned char Main_Get_ReqData_1_F93(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F93   Сʱ�����޹�����
__EXTERN unsigned char Main_Get_ReqData_1_F94(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F94   Сʱ����A���޹�����
__EXTERN unsigned char Main_Get_ReqData_1_F95(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F95   Сʱ����B���޹�����
__EXTERN unsigned char Main_Get_ReqData_1_F96(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F96   Сʱ����C���޹�����
__EXTERN unsigned char Main_Get_ReqData_1_F97(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F97   Сʱ����A���ѹ
__EXTERN unsigned char Main_Get_ReqData_1_F98(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F98   Сʱ����B���ѹ
__EXTERN unsigned char Main_Get_ReqData_1_F99(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F99   Сʱ����C���ѹ
__EXTERN unsigned char Main_Get_ReqData_1_F100(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F100  Сʱ����A�����
__EXTERN unsigned char Main_Get_ReqData_1_F101(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F101  Сʱ����B�����
__EXTERN unsigned char Main_Get_ReqData_1_F102(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F102  Сʱ����C�����
__EXTERN unsigned char Main_Get_ReqData_1_F103(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F103  Сʱ�����������
__EXTERN unsigned char Main_Get_ReqData_1_F105(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F105  Сʱ���������й��ܵ�����
__EXTERN unsigned char Main_Get_ReqData_1_F106(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F106  Сʱ���������޹��ܵ�����
__EXTERN unsigned char Main_Get_ReqData_1_F107(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F107  Сʱ���ᷴ���й��ܵ�����
__EXTERN unsigned char Main_Get_ReqData_1_F108(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F108  Сʱ���ᷴ���޹��ܵ�����
__EXTERN unsigned char Main_Get_ReqData_1_F109(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F109  Сʱ���������й��ܵ���ʾֵ
__EXTERN unsigned char Main_Get_ReqData_1_F110(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F110  Сʱ���������޹��ܵ���ʾֵ
__EXTERN unsigned char Main_Get_ReqData_1_F111(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F111  Сʱ���ᷴ���й��ܵ���ʾֵ
__EXTERN unsigned char Main_Get_ReqData_1_F112(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F112  Сʱ���ᷴ���޹��ܵ���ʾֵ
__EXTERN unsigned char Main_Get_ReqData_1_F113(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F113  Сʱ�����ܹ�������
__EXTERN unsigned char Main_Get_ReqData_1_F114(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F114  Сʱ����A�๦������
__EXTERN unsigned char Main_Get_ReqData_1_F115(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F115  Сʱ����B�๦������
__EXTERN unsigned char Main_Get_ReqData_1_F116(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos); //F116  Сʱ����C�๦������
//__EXTERN unsigned char Main_Get_ReqData_1_F121(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);   //F121  Сʱ����ֱ��ģ����
__EXTERN unsigned char Main_Get_ReqData_1_F129(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F130(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F131(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F132(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F133(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F134(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F135(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F136(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F137(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F138(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F139(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F140(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F141(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F142(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F143(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F144(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F145(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F146(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F147(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F148(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F149(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F150(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F151(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_F152(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);
__EXTERN unsigned char Main_Get_ReqData_1_InstantPow(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI);
__EXTERN unsigned char Main_Get_ReqData_1_Volt(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI);
__EXTERN unsigned char Main_Get_ReqData_1_Curr(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI);
__EXTERN unsigned char Main_Get_ReqData_1_PowVal(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI);
__EXTERN unsigned char Main_Get_ReqData_1_PowViewData(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI);
__EXTERN unsigned char Main_Get_ReqData_1_PowRate(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI);
//azh 170921
__EXTERN unsigned char Main_Get_ReqData_1_F162(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos);  //F162   ���ܱ�����ʱ��

__EXTERN unsigned char Get_CurvePointNum_ByTime(STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucM);

__EXTERN unsigned char Main_Get_ReqData_2_F1(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F2(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F3(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F4(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F5(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F6(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F7(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F8(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F9(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F10(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F11(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F12(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F17(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F18(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F19(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F20(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F21(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F22(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F23(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F24(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F25(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F26(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F27(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F28(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F29(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F30(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F31(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F33(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F34(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F35(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F36(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F37(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F38(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F43(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F44(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F49(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
//__EXTERN unsigned char Main_Get_ReqData_2_F50(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F51(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
//__EXTERN unsigned char Main_Get_ReqData_2_F52(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F53(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F54(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
//__EXTERN unsigned char Main_Get_ReqData_2_F57(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
//__EXTERN unsigned char Main_Get_ReqData_2_F58(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
//__EXTERN unsigned char Main_Get_ReqData_2_F59(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
//__EXTERN unsigned char Main_Get_ReqData_2_F60(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
//__EXTERN unsigned char Main_Get_ReqData_2_F61(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
//__EXTERN unsigned char Main_Get_ReqData_2_F62(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
//__EXTERN unsigned char Main_Get_ReqData_2_F65(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
//__EXTERN unsigned char Main_Get_ReqData_2_F66(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
//__EXTERN unsigned char Main_Get_ReqData_2_F73(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
//__EXTERN unsigned char Main_Get_ReqData_2_F74(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
//__EXTERN unsigned char Main_Get_ReqData_2_F75(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
//__EXTERN unsigned char Main_Get_ReqData_2_F76(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F81(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F82(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F83(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F84(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F85(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F86(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F87(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F88(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F89(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F90(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F91(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F92(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F93(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F94(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F95(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F97(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F98(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F99(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F100(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F101(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F102(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F103(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F104(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F105(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F106(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F107(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F108(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F109(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F110(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
//azh 170923
__EXTERN unsigned char Main_Get_ReqData_2_F219(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);

__EXTERN unsigned char Main_Get_ReqData_2_InstantPow(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI);
__EXTERN unsigned char Main_Get_ReqData_2_Volt(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI);
__EXTERN unsigned char Main_Get_ReqData_2_Curr(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI);
__EXTERN unsigned char Main_Get_ReqData_2_PowVal(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI);
__EXTERN unsigned char Main_Get_ReqData_2_PowViewData(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI);
__EXTERN unsigned char Main_Get_ReqData_2_PowRate(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI);
__EXTERN unsigned char Main_Get_ReqData_2_SumGroup_Pow(unsigned char ucGroupNo,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI,unsigned char ucPowFlag);
//azh 170923
__EXTERN unsigned char Main_Get_ReqData_2_GroupCurveData(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter,unsigned short usDI);

__EXTERN unsigned char Main_Get_ReqData_2_PhasePowData(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_PhasePowMonData(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,unsigned short usDI,STDATETIME *pstDataTime);

__EXTERN unsigned short Main_Get_ReqData_2_F27F35_Aux(unsigned short usPos,unsigned char *pucRetBuffer,STHISRUNDATA *pstHisRunData);
__EXTERN unsigned short Main_Get_ReqData_2_F29F37_Aux(unsigned short usPos,unsigned char *pucRetBuffer,STHISRUNDATA *pstHisRunData);
__EXTERN unsigned char Job_Data_Explain(unsigned char *pucBuffer,unsigned char usLen,unsigned char ucRepType,
                               unsigned char *pucRetTerMsgNum);
__EXTERN unsigned char Main_GetSendData(unsigned char ucRetTerMsgNum,unsigned char ucMSA,unsigned char ucSEQ, unsigned char *pret_mem);
__EXTERN unsigned char  Clr_Group_RetData(void);
__EXTERN unsigned char Clr_Group_RetExtData(void);//�������
__EXTERN unsigned char  Main_SendComm_Recv(unsigned char ucMsgSource,unsigned char ucLinkFN,unsigned char ucUpActive);
__EXTERN unsigned char Main_RComm_UpStart(void);
__EXTERN void MainComm_StartUp(unsigned char ucFlag);
__EXTERN unsigned char Main_RComm_RingStart(void);
__EXTERN void Main_RComm_UpStop(void);
__EXTERN unsigned char Main_Init_LoginCount(void);
__EXTERN unsigned char Main_RComm_SetStandBy_Time(void);

__EXTERN void Main_RComm_OverFlux_Logout(void);
__EXTERN void Main_RComm_Set_OverFlux_Logout(unsigned char ucCount);
__EXTERN unsigned char Main_RComm_Get_OverFlux_Logout(void);
__EXTERN unsigned char Main_RComm_Get_OverFlux_AlrStatus(void);
__EXTERN unsigned char Main_Fill_TmpUpSendTask(unsigned char *pucSendBuffer,unsigned short usLen,unsigned char ucLinkFN);

__EXTERN unsigned char Main_Get_TmpUpSendTask(void);

#endif
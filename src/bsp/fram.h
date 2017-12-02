/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : fram.h
 Description    : �洢��FRAM�ĵ�ַ�궨��
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��    1.00        2010-07-23      snap           the original version
******************************************************************************/

#ifndef __FRAM_H
#define __FRAM_H

//---------------------------- ��ʹ��new_convert���߿����ַʱ��Ҫ�õ�eeprom.h energy.h�к�
//azh 110527 ��energy.h��������
#define PHASE_A                         0           //A�����ݣ������±�ĺ궨��
#define PHASE_C                         1           //C�����ݣ������±�ĺ궨��
#define PHASE_TOTAL                     2           //�������ݣ������±�ĺ궨�� 
#define ARRAY_NUM                       (PHASE_TOTAL + 1)       //������ĸ�����������ά����ĵ�һ���±�
#define LITTLE_ENERGY_NUM               14          //��������'Little_Energy'�����ݸ���

//----------------------------- macro definition ------------------------------
#define FRAM_HEAD_ADDR                      0x60000000          //����FRAM���߼���ʼ��ַ
#define FRAM_CAPACITY                       (32 * 1024)         //����FRAM������32KBytes

#define FRAM_ADDR                           0xa0                //FRAM slave address

//azh 110926
//#define RATE_NUM                            4                   //������
//#define ENERGY_LEN                          4                   //�������������ݵĳ���
//#define CRC_LEN                             2                   //CRCУ��ĳ���

//-----------------------------------------------------------------------------
// С��0.01MW.h�ĵ��������ݵı�����
//-----------------------------------------------------------------------------
#define LITTLE_ENERGY_ADDR                  FRAM_HEAD_ADDR      //С��0.01MW.h�ĵ�����
#define LITTLE_ENERGY_LEN                   (LITTLE_ENERGY_NUM * 4 * ARRAY_NUM)

//-----------------------------------------------------------------------------
// �������ĺ궨���м䲻�ܼ����������ݵĺ궨�壬���ܵߵ�˳�򣬷�����ɳ���Ѱַ����
// COMB_ACTIVE_ENERGY_ADDR -> PHASE_C_ENERGY_ADDR
// 4�ֽڶ����Ʋ����ʽ����λ��ǰ
//-----------------------------------------------------------------------------
// ����������ĺ궨��
//-----------------------------------------------------------------------------
//azh 110926
//#define TOTAL_PHASE_ENERGY_LEN              ((RATE_NUM + 1) * ENERGY_LEN + CRC_LEN)                             //������������ݼ��ϵĳ���

#define COMB_ACTIVE_ENERGY_ADDR             (LITTLE_ENERGY_ADDR + LITTLE_ENERGY_LEN)                            //����й�������
//��
//����1
//����2
//...
//����n(nȡ����RATE_NUM)
//CRC(2Bytes)
#define POS_ACTIVE_ENERGY_ADDR              (COMB_ACTIVE_ENERGY_ADDR + TOTAL_PHASE_ENERGY_LEN)                  //�����й�������
#define NEG_ACTIVE_ENERGY_ADDR              (POS_ACTIVE_ENERGY_ADDR + TOTAL_PHASE_ENERGY_LEN)                   //�����й�������
#define COMB_1_REACTIVE_ENERGY_ADDR         (NEG_ACTIVE_ENERGY_ADDR + TOTAL_PHASE_ENERGY_LEN)                   //���1�޹�������
#define COMB_2_REACTIVE_ENERGY_ADDR         (COMB_1_REACTIVE_ENERGY_ADDR + TOTAL_PHASE_ENERGY_LEN)              //���2�޹�������
#define QUADRANT_1_REACTIVE_ENERGY_ADDR     (COMB_2_REACTIVE_ENERGY_ADDR + TOTAL_PHASE_ENERGY_LEN)              //����1�޹�������
#define QUADRANT_2_REACTIVE_ENERGY_ADDR     (QUADRANT_1_REACTIVE_ENERGY_ADDR + TOTAL_PHASE_ENERGY_LEN)          //����2�޹�������
#define QUADRANT_3_REACTIVE_ENERGY_ADDR     (QUADRANT_2_REACTIVE_ENERGY_ADDR + TOTAL_PHASE_ENERGY_LEN)          //����3�޹�������
#define QUADRANT_4_REACTIVE_ENERGY_ADDR     (QUADRANT_3_REACTIVE_ENERGY_ADDR + TOTAL_PHASE_ENERGY_LEN)          //����4�޹�������
#define POS_APPARENT_ENERGY_ADDR            (QUADRANT_4_REACTIVE_ENERGY_ADDR + TOTAL_PHASE_ENERGY_LEN)          //�������ڵ�����
#define NEG_APPARENT_ENERGY_ADDR            (POS_APPARENT_ENERGY_ADDR + TOTAL_PHASE_ENERGY_LEN)                 //�������ڵ�����

//-----------------------------------------------------------------------------
// ������������г���������ĺ궨��
// �����ܵ���
// �����й������ܵ���
// �����й������ܵ���
// �����й�г���ܵ���
// �����й�г���ܵ���
// CRC(2Bytes)
//-----------------------------------------------------------------------------
#define RELATED_ENERGY_ADDR                 (NEG_APPARENT_ENERGY_ADDR + TOTAL_PHASE_ENERGY_LEN)                 //����������
#define POS_FUND_ACTIVE_ENERGY_ADDR         (RELATED_ENERGY_ADDR + ENERGY_LEN)                                  //��������й�������
#define NEG_FUND_ACTIVE_ENERGY_ADDR         (POS_FUND_ACTIVE_ENERGY_ADDR + ENERGY_LEN)                          //��������й�������
#define POS_HARMONIC_ACTIVE_ENERGY_ADDR     (NEG_FUND_ACTIVE_ENERGY_ADDR + ENERGY_LEN)                          //����г���й�������
#define NEG_HARMONIC_ACTIVE_ENERGY_ADDR     (POS_HARMONIC_ACTIVE_ENERGY_ADDR + ENERGY_LEN)                      //����г���й�������
#define RELATED_ENERGY_CRC                  (NEG_HARMONIC_ACTIVE_ENERGY_ADDR + ENERGY_LEN)                      //����������CRC
//#define RELATED_ENERGY_LEN                  (5 * ENERGY_LEN + CRC_LEN)                                          //�������������ݼ��ϵĳ���

//-----------------------------------------------------------------------------
// ����������ĺ궨��
//-----------------------------------------------------------------------------
//azh 110926
//#define SINGLE_PHASE_ENERGY_LEN             (10 * ENERGY_LEN + CRC_LEN)                                         //������������ݼ��ϵĳ���
//#define SINGLE_PHASE_ALL_ENERGY_LEN         (SINGLE_PHASE_ENERGY_LEN + RELATED_ENERGY_LEN)                      //�������е��������ݼ��ϵĳ���
#define PHASE_A_ENERGY_ADDR                 (RELATED_ENERGY_CRC + CRC_LEN)                                      //A�������
//�����й�����
//�����й�����
//����޹�1����
//����޹�2����
//��һ�����޹�����
//�ڶ������޹�����
//���������޹�����
//���������޹�����
//�������ڵ���
//�������ڵ���
//CRC(2Bytes)
#define PHASE_A_RELATED_ENERGY_ADDR         (PHASE_A_ENERGY_ADDR + SINGLE_PHASE_ENERGY_LEN)                     //A�����������
//��������
//�����й���������
//�����й���������
//�����й�г������
//�����й�г������
//CRC(2Bytes)
//---------------------------- ��ʹ��new_convert���߿����ַʱ��Ҫ�Ķ�, ȷ�����������ֱ���
#ifdef PHASE_3_LINE_4
#define PHASE_B_ENERGY_ADDR                 (PHASE_A_RELATED_ENERGY_ADDR + RELATED_ENERGY_LEN)                  //B�������
#define PHASE_B_RELATED_ENERGY_ADDR         (PHASE_B_ENERGY_ADDR + SINGLE_PHASE_ENERGY_LEN)                     //B�����������
#define PHASE_C_ENERGY_ADDR                 (PHASE_B_RELATED_ENERGY_ADDR + RELATED_ENERGY_LEN)                  //C�������
#define PHASE_C_RELATED_ENERGY_ADDR         (PHASE_C_ENERGY_ADDR + SINGLE_PHASE_ENERGY_LEN)                     //C�����������
#else//PHASE_3_LINE_4
#define PHASE_C_ENERGY_ADDR                 (PHASE_A_RELATED_ENERGY_ADDR + RELATED_ENERGY_LEN)                  //C�������
#define PHASE_C_RELATED_ENERGY_ADDR         (PHASE_C_ENERGY_ADDR + SINGLE_PHASE_ENERGY_LEN)                     //C�����������
#endif//PHASE_3_LINE_4

//-----------------------------------------------------------------------------
// ���������ݵĺ궨�壬���������뵱ǰ������������һ����
//-----------------------------------------------------------------------------
#define ENERGY_BACKUP_NUM                   3           //���������ݵĸ���
#define ENERGY_BACKUP_LEN                   (PHASE_C_RELATED_ENERGY_ADDR + RELATED_ENERGY_LEN - COMB_ACTIVE_ENERGY_ADDR)

#define ENERGY_BACKUP_POINTER               (PHASE_C_RELATED_ENERGY_ADDR + RELATED_ENERGY_LEN)                  //��ǰ���������ݵ�ָ�룬����ָ�����¼�¼���һ���ռ�¼�����ϵļ�¼
#define ENERGY_BACKUP_POINTER_LEN           1
#define ENERGY_BACKUP_ADDR                  (ENERGY_BACKUP_POINTER + ENERGY_BACKUP_POINTER_LEN)                 //������������

//-----------------------------------------------------------------------------
// �����ݶ�����صĺ궨��
// ���������ա���ʱ/˲ʱ���ᡢʱ�α��л����ᡢʱ���л�����
//-----------------------------------------------------------------------------
#define MONTH_OF_ACCOUNT_DAY1_RECORD        (ENERGY_BACKUP_ADDR + ENERGY_BACKUP_LEN * ENERGY_BACKUP_NUM)        //��1���������һ�ζ�������ݵ��·�ֵ
#define MONTH_OF_ACCOUNT_DAY_RECORD_LEN     1
#define MONTH_OF_ACCOUNT_DAY2_RECORD        (MONTH_OF_ACCOUNT_DAY1_RECORD + MONTH_OF_ACCOUNT_DAY_RECORD_LEN)    //��2���������һ�ζ�������ݵ��·�ֵ
#define MONTH_OF_ACCOUNT_DAY3_RECORD        (MONTH_OF_ACCOUNT_DAY2_RECORD + MONTH_OF_ACCOUNT_DAY_RECORD_LEN)    //��3���������һ�ζ�������ݵ��·�ֵ

#define ACCOUNT_DAY_DATA_POINTER            (MONTH_OF_ACCOUNT_DAY3_RECORD + MONTH_OF_ACCOUNT_DAY_RECORD_LEN)    //�����ն������ݵ�ָ�룬����ָ�����¼�¼���һ���ռ�¼�����ϵļ�¼
#define ACCOUNT_DAY_DATA_POINTER_LEN        1
#define TIMING_FREEZE_DATA_POINTER          (ACCOUNT_DAY_DATA_POINTER + ACCOUNT_DAY_DATA_POINTER_LEN)           //��ʱ�������ݵ�ָ�룬����ָ�����¼�¼���һ���ռ�¼�����ϵļ�¼
#define FREEZE_DATA_POINTER_LEN             1
#define INSTANT_FREEZE_DATA_POINTER         (TIMING_FREEZE_DATA_POINTER + FREEZE_DATA_POINTER_LEN)              //˲ʱ�������ݵ�ָ�룬����ָ�����¼�¼���һ���ռ�¼�����ϵļ�¼
#define TIME_CHANGE_FREEZE_DATA_POINTER     (INSTANT_FREEZE_DATA_POINTER + FREEZE_DATA_POINTER_LEN)             //ʱ�α��л��������ݵ�ָ�룬����ָ�����¼�¼���һ���ռ�¼�����ϵļ�¼
#define ZONE_CHANGE_FREEZE_DATA_POINTER     (TIME_CHANGE_FREEZE_DATA_POINTER + FREEZE_DATA_POINTER_LEN)         //ʱ���л��������ݵ�ָ�룬����ָ�����¼�¼���һ���ռ�¼�����ϵļ�¼
#define LOAD_PROFILE_RECORD_POINTER         (ZONE_CHANGE_FREEZE_DATA_POINTER + FREEZE_DATA_POINTER_LEN)         //�����������ݵ�ָ�룬����ָ�����¼�¼���һ���ռ�¼�����ϵļ�¼
#define LOAD_PROFILE_RECORD_POINTER_LEN     4

#define BY_SECOND_RESTORE                   (LOAD_PROFILE_RECORD_POINTER + LOAD_PROFILE_RECORD_POINTER_LEN)     //ÿ��д����ʱ������
#define BY_SECOND_RESTORE_LEN               6

//#define FRAM_LAST_ADDR                      (BY_SECOND_RESTORE + BY_SECOND_RESTORE_LEN)
/*/-----------------------------------------------------------------------------
// �ϵ�ض�CS5463�����Ĵ���ԭʼ(�ۼ�)ֵ��FRAM�б����߼���ַ�궨��
// ˵��: ������ʱ���ֵ��罫sum_buf[12]�����ݽ����б𱣴���FRAM, �ϵ��Ҫ�б�����
//-----------------------------------------------------------------------------
#define MEASURE_DATA_LEN                    (12 * 4 + 2)                    //DATA(48) + CRC(2)
#define MEASURE_SAVE_ADDR                   (LOAD_PROFILE_RECORD_POINTER + LOAD_PROFILE_RECORD_POINTER_LEN)

#define FRAM_LAST_ADDR                      (MEASURE_SAVE_ADDR + MEASURE_DATA_LEN)
*/

//----------------------------- type definition -------------------------------

//-------------------------- functions declaration ----------------------------

//--------------------------- variable declaration ----------------------------

#endif//__FRAM_H

/******************************************************************************
    End Of File
******************************************************************************/

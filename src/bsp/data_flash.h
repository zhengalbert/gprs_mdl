/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : data_flash.h
 Description    : ������DataFlash�����ݵĵ�ַ�궨���ͷ�ļ�
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��    1.00        2010-07-26      snap           the original version
******************************************************************************/

#ifndef __DATA_FLASH_H
#define __DATA_FLASH_H

//---------------------------- ��ʹ��new_convert���߿����ַʱ��Ҫ�õ�fram.h eeprom.h energy.h�к�, ʹ��ʱע��ͬ��������ͬ��
//#define RATE_NUM                            4                   //������
//#define ENERGY_LEN                          4                   //�������������ݵĳ���
//#define CRC_LEN                             2                   //CRCУ��ĳ���
//#define TOTAL_PHASE_ENERGY_LEN              ((RATE_NUM + 1) * ENERGY_LEN + CRC_LEN)                             //������������ݼ��ϵĳ���
//#define DEMAND_LEN                          8                   //�����������ݵĳ���
//#define TOTAL_PHASE_DEMAND_LEN              ((RATE_NUM + 1) * DEMAND_LEN)                                       //�����������ݼ��ϵĳ���
//#define SINGLE_PHASE_ENERGY_LEN             (10 * ENERGY_LEN + CRC_LEN)                                         //������������ݼ��ϵĳ���
//#define SINGLE_PHASE_ALL_ENERGY_LEN         (SINGLE_PHASE_ENERGY_LEN + RELATED_ENERGY_LEN)                      //�������е��������ݼ��ϵĳ���
//#define SINGLE_PHASE_DEMAND_LEN             (10 * DEMAND_LEN)                                                   //�����������ݼ��ϵĳ���


//----------------------------- macro definition ------------------------------
#define DATAFLASH1_HEAD_ADDR    0x70000000          //�����һƬDataFlash���߼���ʼ��ַ
#define DATAFLASH2_HEAD_ADDR    0x71000000          //����ڶ�ƬDataFlash���߼���ʼ��ַ

//-----------------------------------------------------------------------------
// �洢��ʽ��4�ֽ�ʮ��������ʽ����λ��ǰ
// 4�����������������ʱ��Ҫ����ĵ���������Ϊ360�ֽ�
// 4�����������������ʱ��Ҫ����ĵ���������Ϊ420�ֽ�
// ���ݵ��е�����˳���뵱ǰ������һ�£�
//  1������й���+���ʵ�����------------(1 + 4) * 4 �ֽ�
//  2�������й���+���ʵ�����------------(1 + 4) * 4 �ֽ�
//  3�������й���+���ʵ�����------------(1 + 4) * 4 �ֽ�
//  4������޹�1��+���ʵ�����-----------(1 + 4) * 4 �ֽ�
//  5������޹�2��+���ʵ�����-----------(1 + 4) * 4 �ֽ�
//  6����һ�����޹���+���ʵ�����--------(1 + 4) * 4 �ֽ�
//  7���ڶ������޹���+���ʵ�����--------(1 + 4) * 4 �ֽ�
//  8�����������޹���+���ʵ�����--------(1 + 4) * 4 �ֽ�
//  9�����������޹���+���ʵ�����--------(1 + 4) * 4 �ֽ�
// 10������������+���ʵ�����------------(1 + 4) * 4 �ֽ�
// 11������������+���ʵ�����------------(1 + 4) * 4 �ֽ�
// 12��������������г��������----------- 5 * 4 �ֽ�
// 13��A�������------------------------10 * 4 �ֽ�
// 14��B���������������������߱�----10 * 4 �ֽ�
// 15��C�������------------------------10 * 4 �ֽ�
//-----------------------------------------------------------------------------
// �洢��ʽ������ʱ�� + �������
// ����ʱ�����ݣ�mm���֣�hh��ʱ��DD���գ�MM���£�YY���꣩��ʮ���Ƹ�ʽ
// ����������ݣ�3�ֽ�ʮ���Ƹ�ʽ����λ��ǰ
// 4�����������������ʱ��Ҫ�������������Ϊ560�ֽ�
// 4�����������������ʱ��Ҫ�������������Ϊ640�ֽ�
// ���ݵ��е�����˳���뵱ǰ�������һ�£�
// 16�������й���+�����������������ʱ��------------(1 + 4) * 8 �ֽ�
// 17�������й���+�����������������ʱ��------------(1 + 4) * 8 �ֽ�
// 18������޹�1��+�����������������ʱ��-----------(1 + 4) * 8 �ֽ�
// 19������޹�2��+�����������������ʱ��-----------(1 + 4) * 8 �ֽ�
// 20��һ�����޹���+�����������������ʱ��----------(1 + 4) * 8 �ֽ�
// 21���������޹���+�����������������ʱ��----------(1 + 4) * 8 �ֽ�
// 22���������޹���+�����������������ʱ��----------(1 + 4) * 8 �ֽ�
// 23���������޹���+�����������������ʱ��----------(1 + 4) * 8 �ֽ�
// 24������������+�����������������ʱ��------------(1 + 4) * 8 �ֽ�
// 25������������+�����������������ʱ��------------(1 + 4) * 8 �ֽ�
// 26��A���������������ʱ��------------------------10 * 8 �ֽ�
// 27��B���������������ʱ�䣨������������߱�----10 * 8 �ֽ�
// 28��C���������������ʱ��------------------------10 * 8 �ֽ�
//-----------------------------------------------------------------------------
// �����ն������ݱ������ĺ궨��
//-----------------------------------------------------------------------------
#ifdef PHASE_3_LINE_4
#define ACCOUNT_DAY_ENERGY_LEN                 ((TOTAL_PHASE_ENERGY_LEN - CRC_LEN) * 11 + \
                                                (RELATED_ENERGY_LEN - CRC_LEN) +          \
                                                (SINGLE_PHASE_ALL_ENERGY_LEN - 2 * CRC_LEN) * 3)                    //���������յ�������������ʵ�ʳ��ȣ��������ߣ�
#define ACCOUNT_DAY_DEMAND_LEN                  (TOTAL_PHASE_DEMAND_LEN * 10 + SINGLE_PHASE_DEMAND_LEN * 3)         //������������  ����������ʵ�ʳ��ȣ��������ߣ�
#else//PHASE_3_LINE_4
#define ACCOUNT_DAY_ENERGY_LEN                 ((TOTAL_PHASE_ENERGY_LEN - CRC_LEN) * 11 + \
                                                (RELATED_ENERGY_LEN - CRC_LEN) +          \
                                                (SINGLE_PHASE_ALL_ENERGY_LEN - 2 * CRC_LEN) * 2)                    //���������յ�������������ʵ�ʳ��ȣ��������ߣ�
#define ACCOUNT_DAY_DEMAND_LEN                  (TOTAL_PHASE_DEMAND_LEN * 10 + SINGLE_PHASE_DEMAND_LEN * 2)         //������������  ����������ʵ�ʳ��ȣ��������ߣ�
#endif//PHASE_3_LINE_4
//----------------------- ��ʹ��new_convert���߿����ַʱ��Ҫע�͵�ǰ���#if METER_TYPEϵ����, ע�����������ͬ��
//#define ACCOUNT_DAY_ENERGY_LEN                 ((TOTAL_PHASE_ENERGY_LEN - CRC_LEN) * 11 + (RELATED_ENERGY_LEN - CRC_LEN) + (SINGLE_PHASE_ALL_ENERGY_LEN - 2 * CRC_LEN) * 2)                    //���������յ�������������ʵ�ʳ��ȣ��������ߣ�
//#define ACCOUNT_DAY_DEMAND_LEN                  (TOTAL_PHASE_DEMAND_LEN * 10 + SINGLE_PHASE_DEMAND_LEN * 2)         //������������  ����������ʵ�ʳ��ȣ��������ߣ�


#define ACCOUNT_DAY_ENERGY_STORE_LEN            ACCOUNT_DAY_ENERGY_LEN                                              //���������յ������������ݴ洢ռ�ô洢������
#define ACCOUNT_DAY_DEMAND_STORE_LEN            ACCOUNT_DAY_DEMAND_LEN                                              //������������  ���������ݴ洢ռ�ô洢������
#define SINGLE_ACCOUNT_DAY_DATA_LEN             (ACCOUNT_DAY_ENERGY_STORE_LEN + ACCOUNT_DAY_DEMAND_STORE_LEN)       //���������������������ݴ洢ռ�ô洢������
#define ACCOUNT_DAY_DATA_NUM                    12                                                                  //�����ն������ݵ�����
#define ACCOUNT_DAY_BUF_LEN                     (SINGLE_ACCOUNT_DAY_DATA_LEN * ACCOUNT_DAY_DATA_NUM)                //�����ն������ݱ������ĳ���
#define ACCOUNT_DAY_DATA_ADDR                   DATAFLASH1_HEAD_ADDR                                                //�����ն��������׵�ַ
#define ACCOUNT_DAY_ENERGY_ADDR                 DATAFLASH1_HEAD_ADDR                                                //�����յ��������������׵�ַ
#define ACCOUNT_DAY_DEMAND_ADDR                 (ACCOUNT_DAY_ENERGY_ADDR + ACCOUNT_DAY_ENERGY_STORE_LEN)            //��������  �����������׵�ַ

//-----------------------------------------------------------------------------
// �洢��ʽ��4�ֽ�ʮ��������ʽ����λ��ǰ
// 4�����������Ҫ����ĵ�������������˲ʱ������Ϊ269�ֽ�
// ���ݵ��е�����˳��
//  1�������й���+���ʵ�����----------------(1 + 4) * 4 �ֽ�
//  2�������й���+���ʵ�����----------------(1 + 4) * 4 �ֽ�
//  3������޹�1��+���ʵ�����---------------(1 + 4) * 4 �ֽ�
//  4������޹�2��+���ʵ�����---------------(1 + 4) * 4 �ֽ�
//  5����һ�����޹���+���ʵ�����------------(1 + 4) * 4 �ֽ�
//  6���ڶ������޹���+���ʵ�����------------(1 + 4) * 4 �ֽ�
//  7�����������޹���+���ʵ�����------------(1 + 4) * 4 �ֽ�
//  8�����������޹���+���ʵ�����------------(1 + 4) * 4 �ֽ�
//  9�������й���+�����������������ʱ��----(1 + 4) * 8 �ֽ�
// 10�������й���+�����������������ʱ��----(1 + 4) * 8 �ֽ�
// 11����������
//       ���й�����-------------------------3 �ֽ�
//       A���й�����------------------------3 �ֽ�
//       B���й�����------------------------3 �ֽ�
//       C���й�����------------------------3 �ֽ�
//       ���޹�����-------------------------3 �ֽ�
//       A���޹�����------------------------3 �ֽ�
//       B���޹�����------------------------3 �ֽ�
//       C���޹�����------------------------3 �ֽ�
//-----------------------------------------------------------------------------
// ��ʱ�������ݱ������ĺ궨��
//-----------------------------------------------------------------------------
#define SINGLE_FREEZE_DATA_LEN                  (5 + (TOTAL_PHASE_ENERGY_LEN - CRC_LEN) * 8 + \
                                                TOTAL_PHASE_DEMAND_LEN * 2 + 3 * 8)                                 //������������ʵ�ʳ���
//#define SINGLE_FREEZE_DATA_LEN                  (5 + (TOTAL_PHASE_ENERGY_LEN - CRC_LEN) * 8 + TOTAL_PHASE_DEMAND_LEN * 2 + 3 * 8)//������������ʵ�ʳ���
#define SINGLE_FREEZE_STORE_LEN                 SINGLE_FREEZE_DATA_LEN                                              //�����������ݴ洢ռ�ô洢������
#define TIMING_FREEZE_DATA_NUM                  12                                                                  //��ʱ�������ݵ�����
#define TIMING_FREEZE_BUF_LEN                   (SINGLE_FREEZE_STORE_LEN * TIMING_FREEZE_DATA_NUM)                  //��ʱ�������ݱ������ĳ���
#define TIMING_FREEZE_DATA_ADDR                 (ACCOUNT_DAY_DATA_ADDR + ACCOUNT_DAY_BUF_LEN)                       //��ʱ���������׵�ַ

//-----------------------------------------------------------------------------
// ˲ʱ�������ݱ������ĺ궨��
//-----------------------------------------------------------------------------
#define INSTANT_FREEZE_DATA_NUM                 3                                                                   //˲ʱ�������ݵ�����
#define INSTANT_FREEZE_DATA_LEN                 (SINGLE_FREEZE_STORE_LEN * INSTANT_FREEZE_DATA_NUM)                 //˲ʱ�������ݱ������ĳ���
#define INSTANT_FREEZE_DATA_ADDR                (TIMING_FREEZE_DATA_ADDR + TIMING_FREEZE_BUF_LEN)                   //˲ʱ���������׵�ַ

//-----------------------------------------------------------------------------
// ʱ�α��л��������ݱ������ĺ궨��
//-----------------------------------------------------------------------------
#define TIME_CHANGE_FREEZE_DATA_NUM             2                                                                   //ʱ�α��л��������ݵ�����
#define TIME_CHANGE_FREEZE_BUF_LEN              (SINGLE_FREEZE_STORE_LEN * TIME_CHANGE_FREEZE_DATA_NUM)             //ʱ�α��л��������ݱ������ĳ���
#define TIME_CHANGE_FREEZE_DATA_ADDR            (INSTANT_FREEZE_DATA_ADDR + INSTANT_FREEZE_DATA_LEN)                //ʱ�α��л����������׵�ַ

//-----------------------------------------------------------------------------
// ��ʱ���л��������ݱ������ĺ궨��
//-----------------------------------------------------------------------------
#define ZONE_CHANGE_FREEZE_DATA_NUM             2                                                                   //��ʱ���л��������ݵ�����
#define ZONE_CHANGE_FREEZE_BUF_LEN              (SINGLE_FREEZE_STORE_LEN * ZONE_CHANGE_FREEZE_DATA_NUM)             //��ʱ���л��������ݱ������ĳ���
#define ZONE_CHANGE_FREEZE_DATA_ADDR            (TIME_CHANGE_FREEZE_DATA_ADDR + TIME_CHANGE_FREEZE_BUF_LEN)         //��ʱ���л����������׵�ַ

//-----------------------------------------------------------------------------
// �����������ݱ������ĺ궨��
// ���ɼ�¼����˳�򣺣���92�ֽڣ�
// 1����ѹ��������Ƶ��
//      A��B��C ���ѹ��ÿ�� 2 �ֽڣ��� 6 �ֽڣ���λ��0.1kV��
//      A��B��C �������ÿ�� 3 �ֽڣ��� 9 �ֽڣ���λ��0.001A��
//      Ƶ�ʣ�2 �ֽڣ���λ��0.01Hz��
// 2���С��޹�����
//      �ܼ�A��B��C ���й����ʣ�ÿ�� 3 �ֽڣ��� 12 �ֽڣ���λ��0.0001MW��
//      �ܼ�A��B��C ���޹����ʣ�ÿ�� 3 �ֽڣ��� 12 �ֽڣ���λ��0.0001Mvar��
// 3����������
//      �ܼ� A��B��C �๦��������ÿ�� 2 �ֽڣ��� 8 �ֽڣ���λ��0.001��
// 4���С��޹��ܵ���
//      �����й��ܵ��ܣ�4 �ֽڣ���λ��0.01MWh��
//      �����й��ܵ��ܣ�4 �ֽڣ���λ��0.01MWh��
//      ����޹� 1 �ܵ��ܣ�4 �ֽڣ���λ��0.01Mvarh��
//      ����޹� 2 �ܵ��ܣ�4 �ֽڣ���λ��0.01Mvarh��
// 5���������޹��ܵ���
//      ��һ�����޹��ܵ��ܣ�4 �ֽڣ���λ��0.01Mvarh��
//      �ڶ������޹��ܵ��ܣ�4 �ֽڣ���λ��0.01Mvarh��
//      ���������޹��ܵ��ܣ�4 �ֽڣ���λ��0.01Mvarh��
//      ���������޹��ܵ��ܣ�4 �ֽڣ���λ��0.01Mvarh��
// 6����ǰ����
//      ��ǰ�й�������3 �ֽڣ���λ��0.0001MW��
//      ��ǰ�޹�������3 �ֽڣ���λ��0.0001Mvar��
//-----------------------------------------------------------------------------
#define LOAD_PROFILE_RECORD_NUM                 (60 * 24 * 7)                                                       //�����������ݵ����������1min��¼7��
#define LOAD_PROFILE_RECORD_LEN                 92                                                                  //����������������ʵ�ʳ���
#define LOAD_PROFILE_STORE_LEN                  92                                                                  //���������������ݴ洢ռ�ô洢������
#define LOAD_PROFILE_BUF_LEN                    (LOAD_PROFILE_STORE_LEN * LOAD_PROFILE_RECORD_NUM)                  //�����������ݱ������ĳ���
#define LOAD_PROFILE_DATA_ADDR                  (ZONE_CHANGE_FREEZE_DATA_ADDR + ZONE_CHANGE_FREEZE_BUF_LEN)         //�������������׵�ַ

#define ZONE_TABLE_UPDATE_TIME_ADDR             (LOAD_PROFILE_DATA_ADDR + LOAD_PROFILE_BUF_LEN)                     //����ʱ�����л�ʱ�� ��ʱ������ [����������]
#define ZONE_TABLE_UPDATE_TIME_LEN              5
#define FEE_TABLE_UPDATE_TIME_ADDR              (ZONE_TABLE_UPDATE_TIME_ADDR + ZONE_TABLE_UPDATE_TIME_LEN)          //������ʱ�α��л�ʱ�� ��ʱ������[����������]
#define FEE_TABLE_UPDATE_TIME_LEN               5
#define ZONE_TABLE_ONE_ADDR                     (FEE_TABLE_UPDATE_TIME_ADDR + FEE_TABLE_UPDATE_TIME_LEN)            //��һ��ʱ���� 14*3��Լ����ʱ��Ҫ����
#define ZONE_TABLE_ONE_LEN                      (14 * 3)                                                            //MMDDNN
#define FEE_TABLE_ONE1_ADDR                     (ZONE_TABLE_ONE_ADDR + ZONE_TABLE_ONE_LEN)                          //��һ��1��ʱ�α�����
#define FEE_TABLE_ONE1_LEN                      (14 * 3)                                                            //��ʱ�α�ĳ���
#define FEE_TABLE_NUM                           8                                                                   //ʱ�α���
#define ZONE_TABLE_TWO_ADDR                     (FEE_TABLE_ONE1_ADDR + FEE_TABLE_ONE1_LEN * FEE_TABLE_NUM)          //�ڶ���ʱ���� 14*3��Լ����ʱ��Ҫ����
#define FEE_TABLE_TWO1_ADDR                     (ZONE_TABLE_TWO_ADDR + ZONE_TABLE_ONE_LEN)                          //�ڶ���1��ʱ�α�����
#define HOLIDAY_TABLE_ADDR                      (FEE_TABLE_TWO1_ADDR + FEE_TABLE_ONE1_LEN * FEE_TABLE_NUM)          //��������
#define HOLIDAY_TABLE_LEN                       (254 * 4)
#define WEEKDAY_STATUS_ADDR                     (HOLIDAY_TABLE_ADDR + HOLIDAY_TABLE_LEN)                            //������״̬�� 1�ֽ� ������
#define WEEKDAY_TABLE_ADDR                      (WEEKDAY_STATUS_ADDR + 1)                                           //������ִ��ʱ�α�� 1�ֽ� ������

#define POS_ACTIVE_DEMAND_LIM_ADDR              (WEEKDAY_TABLE_ADDR + 1)                                            //�����й������������
#define POS_ACTIVE_DEMAND_LIM_LEN               3
#define NEG_ACTIVE_DEMAND_LIM_ADDR              (POS_ACTIVE_DEMAND_LIM_ADDR + POS_ACTIVE_DEMAND_LIM_LEN)            //�����й������������
#define QUADRANT_1_REACTIVE_DEMAND_LIM_ADDR     (NEG_ACTIVE_DEMAND_LIM_ADDR + POS_ACTIVE_DEMAND_LIM_LEN)            //��1�����޹���������
#define QUADRANT_2_REACTIVE_DEMAND_LIM_ADDR     (QUADRANT_1_REACTIVE_DEMAND_LIM_ADDR + POS_ACTIVE_DEMAND_LIM_LEN)   //��2�����޹���������
#define QUADRANT_3_REACTIVE_DEMAND_LIM_ADDR     (QUADRANT_2_REACTIVE_DEMAND_LIM_ADDR + POS_ACTIVE_DEMAND_LIM_LEN)   //��3�����޹���������
#define QUADRANT_4_REACTIVE_DEMAND_LIM_ADDR     (QUADRANT_3_REACTIVE_DEMAND_LIM_ADDR + POS_ACTIVE_DEMAND_LIM_LEN)   //��4�����޹���������

#define POS_ACTIVE_DEMAND_REC_ADDR              (QUADRANT_4_REACTIVE_DEMAND_LIM_ADDR + POS_ACTIVE_DEMAND_LIM_LEN)   //�����й����������¼1-10
#define POS_ACTIVE_DEMAND_REC_LEN               ((6 + 6 + 3 + 5) * 10)                                              //��������ʱ��(������)�������������ʱ��(û����) �μ���Լ03 12 01 01
#define NEG_ACTIVE_DEMAND_REC_ADDR              (POS_ACTIVE_DEMAND_REC_ADDR + POS_ACTIVE_DEMAND_REC_LEN)            //�����й����������¼1-10
#define QUADRANT_1_REACTIVE_DEMAND_REC_ADDR     (NEG_ACTIVE_DEMAND_REC_ADDR + POS_ACTIVE_DEMAND_REC_LEN)            //��1�����޹�������¼1-10
#define QUADRANT_2_REACTIVE_DEMAND_REC_ADDR     (QUADRANT_1_REACTIVE_DEMAND_REC_ADDR + POS_ACTIVE_DEMAND_REC_LEN)   //��2�����޹�������¼1-10
#define QUADRANT_3_REACTIVE_DEMAND_REC_ADDR     (QUADRANT_2_REACTIVE_DEMAND_REC_ADDR + POS_ACTIVE_DEMAND_REC_LEN)   //��3�����޹�������¼1-10
#define QUADRANT_4_REACTIVE_DEMAND_REC_ADDR     (QUADRANT_3_REACTIVE_DEMAND_REC_ADDR + POS_ACTIVE_DEMAND_REC_LEN)   //��4�����޹�������¼1-10

#define PROGRAM_RECORD_ADDR                     (QUADRANT_4_REACTIVE_DEMAND_REC_ADDR + POS_ACTIVE_DEMAND_REC_LEN)   //��̼�¼, 10��
#define PROGRAM_RECORD_LEN                      (6 + 4 + 4 * 10)

#define ENERGY_ZERO_COUNT_ADDR                  (PROGRAM_RECORD_ADDR + PROGRAM_RECORD_LEN * 10)                     //��������ܴ���
#define ENERGY_ZERO_COUNT_LEN                   3
#define ENERGY_ZERO_RECORD_ADDR                 (ENERGY_ZERO_COUNT_ADDR + ENERGY_ZERO_COUNT_LEN)                    //��������¼(��B������), 10��
#define ENERGY_ZERO_RECORD_LEN                  (6 + 4 + 4 * 6 + 4 * 6 * 3)

#define DEMAND_ZERO_RECORD_ADDR                 (ENERGY_ZERO_RECORD_ADDR + ENERGY_ZERO_RECORD_LEN * 10)             //��������¼(��B������), 10��
#define DEMAND_ZERO_RECORD_LEN                  (6 + 4 + (3 + 5) * 6 * 4)

#define EVENT_ZERO_COUNT_ADDR                   (DEMAND_ZERO_RECORD_ADDR + DEMAND_ZERO_RECORD_LEN * 10)             //�¼��������
#define EVENT_ZERO_COUNT_LEN                    3
#define EVENT_ZERO_RECORD_ADDR                  (EVENT_ZERO_COUNT_ADDR + EVENT_ZERO_COUNT_LEN)                      //�¼������¼, 10��
#define EVENT_ZERO_RECORD_LEN                   (6 + 4 + 4)

#define CHECK_TIME_COUNT_ADDR                   (EVENT_ZERO_RECORD_ADDR + EVENT_ZERO_RECORD_LEN * 10)               //Уʱ�ܴ���
#define CHECK_TIME_COUNT_LEN                    3
#define CHECK_TIME_RECORD_ADDR                  (CHECK_TIME_COUNT_ADDR + CHECK_TIME_COUNT_LEN)                      //Уʱ��¼, 10��
#define CHECK_TIME_RECORD_LEN                   (4 + 6 + 6)

#define TABLE_PROGRAM_COUNT_ADDR                (CHECK_TIME_RECORD_ADDR + CHECK_TIME_RECORD_LEN * 10)               //ʱ�α����ܴ���
#define TABLE_PROGRAM_COUNT_LEN                 3
#define TABLE_PROGRAM_RECORD_ADDR               (TABLE_PROGRAM_COUNT_ADDR + TABLE_PROGRAM_COUNT_LEN)                //ʱ�α��̼�¼, 10��
#define TABLE_PROGRAM_RECORD_LEN                (6 + 4 + 3 * 14 * 8 * 2)

#define ZONE_PROGRAM_COUNT_ADDR                 (TABLE_PROGRAM_RECORD_ADDR + TABLE_PROGRAM_RECORD_LEN * 10)         //ʱ������ܴ���
#define ZONE_PROGRAM_COUNT_LEN                  3
#define ZONE_PROGRAM_RECORD_ADDR                (ZONE_PROGRAM_COUNT_ADDR + ZONE_PROGRAM_COUNT_LEN)                  //ʱ����̼�¼, 10��
#define ZONE_PROGRAM_RECORD_LEN                 (6 + 4 + 3 * 14 * 2)

#define WEEKDAY_PROGRAM_COUNT_ADDR              (ZONE_PROGRAM_RECORD_ADDR + ZONE_PROGRAM_RECORD_LEN * 10)           //�����ձ���ܴ���
#define WEEKDAY_PROGRAM_COUNT_LEN               3
#define WEEKDAY_PROGRAM_RECORD_ADDR             (WEEKDAY_PROGRAM_COUNT_ADDR + WEEKDAY_PROGRAM_COUNT_LEN)            //�����ձ�̼�¼, 10��
#define WEEKDAY_PROGRAM_RECORD_LEN              (6 + 4 + 1)

#define HOLIDAY_PROGRAM_COUNT_ADDR              (WEEKDAY_PROGRAM_RECORD_ADDR + WEEKDAY_PROGRAM_RECORD_LEN * 10)     //�ڼ��ձ���ܴ���
#define HOLIDAY_PROGRAM_COUNT_LEN               3
#define HOLIDAY_PROGRAM_RECORD_ADDR             (HOLIDAY_PROGRAM_COUNT_ADDR + HOLIDAY_PROGRAM_COUNT_LEN)            //�ڼ��ձ�̼�¼, 10��
#define HOLIDAY_PROGRAM_RECORD_LEN              (6 + 4 + 4 * 254)

#define COMB_Y_PROGRAM_COUNT_ADDR               (HOLIDAY_PROGRAM_RECORD_ADDR + HOLIDAY_PROGRAM_RECORD_LEN * 10)     //�й���Ϸ�ʽ����ܴ���
#define COMB_Y_PROGRAM_COUNT_LEN                3
#define COMB_Y_PROGRAM_RECORD_ADDR              (COMB_Y_PROGRAM_COUNT_ADDR + COMB_Y_PROGRAM_COUNT_LEN)              //�й���Ϸ�ʽ��̼�¼, 10��
#define COMB_Y_PROGRAM_RECORD_LEN               (6 + 4 + 1)

#define COMB_1_PROGRAM_COUNT_ADDR               (COMB_Y_PROGRAM_RECORD_ADDR + COMB_Y_PROGRAM_RECORD_LEN * 10)       //�޹���Ϸ�ʽ1����ܴ���
#define COMB_1_PROGRAM_COUNT_LEN                3
#define COMB_1_PROGRAM_RECORD_ADDR              (COMB_1_PROGRAM_COUNT_ADDR + COMB_1_PROGRAM_COUNT_LEN)              //�޹���Ϸ�ʽ1��̼�¼, 10��
#define COMB_1_PROGRAM_RECORD_LEN               (6 + 4 + 1)

#define COMB_2_PROGRAM_COUNT_ADDR               (COMB_1_PROGRAM_RECORD_ADDR + COMB_1_PROGRAM_RECORD_LEN * 10)       //�޹���Ϸ�ʽ2����ܴ���
#define COMB_2_PROGRAM_COUNT_LEN                3
#define COMB_2_PROGRAM_RECORD_ADDR              (COMB_2_PROGRAM_COUNT_ADDR + COMB_2_PROGRAM_COUNT_LEN)              //�޹���Ϸ�ʽ2��̼�¼, 10��
#define COMB_2_PROGRAM_RECORD_LEN               (6 + 4 + 1)

#define ACCOUNT_DAY_PROGRAM_COUNT_ADDR          (COMB_2_PROGRAM_RECORD_ADDR + COMB_2_PROGRAM_RECORD_LEN * 10)       //�����ձ���ܴ���
#define ACCOUNT_DAY_PROGRAM_COUNT_LEN           3
#define ACCOUNT_DAY_PROGRAM_RECORD_ADDR         (ACCOUNT_DAY_PROGRAM_COUNT_ADDR + ACCOUNT_DAY_PROGRAM_COUNT_LEN)    //�����ձ�̼�¼, 10��
#define ACCOUNT_DAY_PROGRAM_RECORD_LEN          (6 + 4 + 2 * 3)

#define DATAFLASH1_LAST_ADDR                    (ACCOUNT_DAY_PROGRAM_RECORD_ADDR + ACCOUNT_DAY_PROGRAM_RECORD_LEN * 10) //������ �������ַ

//----------------------------- type definition -------------------------------

//-------------------------- functions declaration ----------------------------

//--------------------------- variable declaration ----------------------------

#endif//__DATA_FLASH_H

/******************************************************************************
    End Of File
******************************************************************************/

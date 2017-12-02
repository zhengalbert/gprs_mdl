/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name  : var_define.h
 Description: �洢��EEPROM�ĵ�ַ�궨�� ���¼�������ļ��Ƴ� �Ż������ٶ�
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��    1.00        2011-06-27      azh           the original version
******************************************************************************/

#ifndef __VAR_DEFINE_H
#define __VAR_DEFINE_H


//----------------------------- macro definition ------------------------------
#define EEPROM_HEAD_ADDR        0x61000000          //����EEPROM���߼���ʼ��ַ
#define EEPROM_CAPACITY         (64 * 1024)         //����EEPROM������64KBytes
#define EEPROM_LAST_WORD_ADDR   (EEPROM_HEAD_ADDR + EEPROM_CAPACITY - 5)//���1������Ϊȫ�ֱ�ʶ��ŵ�ַ
#define EEPROM_GFLG_ADDR        EEPROM_LAST_WORD_ADDR       //ȫ��������ʶ(U32)�洢��ַ
#define OC_COVER_FLG            (1 << 0)            //bit0:���ر�Ǳ�ʶ:  0 ���ϱ��,1 �򿪱��;
#define OC_TMNBX_FLG            (1 << 1)            //bit1:���ض�ť�б�ʶ:0 ���϶�ť��,1 �򿪶�ť��;
//bit2~31:����,������Ӧ��
#define EEPROM_ADDR                 0xa0//0xa2                //EEPROM slave address


#define RATE_NUM                    4                   //������
#define ENERGY_LEN                  4                   //�������������ݵĳ���
#define CRC_LEN                     2                   //CRCУ��ĳ���

//��������궨��
//#define LINE_LOSS                                         //��������궨�壬����ͭ����������Ĭ��Ϊ������
//#define PHASE_3_LINE_4                                    //METER_TYPE����,Ĭ��ΪPHASE_3_LINE_3
//-----------------------------------------------------------------------------
// ����������ĺ궨��
//-----------------------------------------------------------------------------
#define TOTAL_PHASE_ENERGY_LEN              ((RATE_NUM + 1) * ENERGY_LEN + CRC_LEN)                             //������������ݼ��ϵĳ���

//������������г���������ĺ궨��
//#ifdef LINE_LOSS//LINE_LOSS
#define RELATED_ENERGY_LEN                  (7 * ENERGY_LEN)//����ͭ������
//#else   //LINE_LOSS
//#define RELATED_ENERGY_LEN                  (5 * ENERGY_LEN)//������ͭ������
//#ifdef  //LINE_LOSS
//-----------------------------------------------------------------------------
// ����������ĺ궨��
//-----------------------------------------------------------------------------
#define SINGLE_PHASE_ENERGY_LEN             (10 * ENERGY_LEN + CRC_LEN)                                     //������������ݼ��ϵĳ���
#define SINGLE_PHASE_ALL_ENERGY_LEN         (SINGLE_PHASE_ENERGY_LEN + RELATED_ENERGY_LEN)                  //�������е��������ݼ��ϵĳ���
/*
//-----------------------------------------------------------------------------
// ���������ĺ궨��
//-----------------------------------------------------------------------------
//#define DEMAND_LEN                              (DEMAND_JUST_LEN+DEMAND_HM_LEN+DEMAND_YMD_LEN)//07��Լ�����������ݵĳ���
//#define DEMAND_JUST_LEN                         3                   //��������������������ʾ
//#define DEMAND_HM_LEN                           2                   //�������ݵ�ʱ��������ʾ
//#define DEMAND_YMD_LEN                          3                   //�������ݵ�������������ʾ
//#define TOTAL_PHASE_DEMAND_LEN                  ((RATE_NUM + 1) * DEMAND_LEN)                                       //�����������ݼ��ϵĳ���

//-----------------------------------------------------------------------------
// ���������ĺ궨��
//-----------------------------------------------------------------------------
//#define SINGLE_PHASE_DEMAND_LEN                 (10 * DEMAND_LEN)                                                   //�����������ݼ��ϵĳ���



*/

//-----------------------------------------------------------------------------
// �������ĺ궨���м䲻�ܼ����������ݵĺ궨�壬���ܵߵ�˳�򣬷�����ɳ���Ѱַ����
// COMB_ACTIVE_ENERGY_ADDR -> PHASE_C_ENERGY_ADDR
// 4�ֽڶ����Ʋ����ʽ����λ��ǰ
//-----------------------------------------------------------------------------
// ����������ĺ궨��
//-----------------------------------------------------------------------------
//#define RATE_NUM                    4                   //������
//#define ENERGY_LEN                  4                   //�������������ݵĳ���
//#define CRC_LEN                     2                   //CRCУ��ĳ���
//#define TOTAL_PHASE_ENERGY_LEN              ((RATE_NUM + 1) * ENERGY_LEN + CRC_LEN)                             //������������ݼ��ϵĳ���
//��
//����1
//����2
//...
//����n(nȡ����RATE_NUM)
//CRC(2Bytes)

#define ENG_STORE_START_ADDR                EEPROM_HEAD_ADDR
#define ENG_COMPOSP_00000000_00_ADDR        ENG_STORE_START_ADDR//����й�������
#define ENG_COMPOSP_00000000_00_LEN         ENERGY_LEN
#define ENG_COMPOSP_00000000_TOT_LEN        TOTAL_PHASE_ENERGY_LEN//+2B CRC

#define ENG_POSP_00010000_00_ADDR           (ENG_COMPOSP_00000000_00_ADDR+ENG_COMPOSP_00000000_TOT_LEN)//�����й�������
#define ENG_POSP_00010000_00_LEN            ENERGY_LEN
#define ENG_POSP_00010000_TOT_LEN           TOTAL_PHASE_ENERGY_LEN

#define ENG_NEGP_00020000_00_ADDR           (ENG_POSP_00010000_00_ADDR+ENG_POSP_00010000_TOT_LEN)//�����й�������
#define ENG_NEGP_00020000_00_LEN            ENERGY_LEN
#define ENG_NEGP_00020000_TOT_LEN           TOTAL_PHASE_ENERGY_LEN

#define ENG_COMQ1_00030000_00_ADDR          (ENG_NEGP_00020000_00_ADDR+ENG_NEGP_00020000_TOT_LEN)   //���1�޹�������
#define ENG_COMQ1_00030000_00_LEN           ENERGY_LEN
#define ENG_COMQ1_00030000_TOT_LEN          TOTAL_PHASE_ENERGY_LEN

#define ENG_COMQ2_00040000_00_ADDR          (ENG_COMQ1_00030000_00_ADDR+ENG_COMQ1_00030000_TOT_LEN)  //���2�޹�������
#define ENG_COMQ2_00040000_00_LEN           ENERGY_LEN
#define ENG_COMQ2_00040000_TOT_LEN          TOTAL_PHASE_ENERGY_LEN

#define ENG_REA1Q_00050000_00_ADDR          (ENG_COMQ2_00040000_00_ADDR+ENG_COMQ2_00040000_TOT_LEN) //����1�޹�������
#define ENG_REA1Q_00050000_00_LEN           ENERGY_LEN
#define ENG_REA1Q_00050000_TOT_LEN          TOTAL_PHASE_ENERGY_LEN

#define ENG_REA2Q_00060000_00_ADDR          (ENG_REA1Q_00050000_00_ADDR+ENG_REA1Q_00050000_TOT_LEN) //����2�޹�������
#define ENG_REA2Q_00060000_00_LEN           ENERGY_LEN
#define ENG_REA2Q_00060000_TOT_LEN          TOTAL_PHASE_ENERGY_LEN

#define ENG_REA3Q_00070000_00_ADDR          (ENG_REA2Q_00060000_00_ADDR+ENG_REA2Q_00060000_TOT_LEN) //����3�޹�������
#define ENG_REA3Q_00070000_00_LEN           ENERGY_LEN
#define ENG_REA3Q_00070000_TOT_LEN          TOTAL_PHASE_ENERGY_LEN

#define ENG_REA4Q_00080000_00_ADDR          (ENG_REA3Q_00070000_00_ADDR+ENG_REA3Q_00070000_TOT_LEN) //����4�޹�������
#define ENG_REA4Q_00080000_00_LEN           ENERGY_LEN
#define ENG_REA4Q_00080000_TOT_LEN          TOTAL_PHASE_ENERGY_LEN
//
#define ENG_POSS_00090000_00_ADDR           (ENG_REA4Q_00080000_00_ADDR+ENG_REA4Q_00080000_TOT_LEN) //�������ڵ�����
#define ENG_POSS_00090000_00_LEN            ENERGY_LEN
#define ENG_POSS_00090000_TOT_LEN           TOTAL_PHASE_ENERGY_LEN

#define ENG_NEGS_000A0000_00_ADDR           (ENG_POSS_00090000_00_ADDR+ENG_POSS_00090000_TOT_LEN)   //�������ڵ�����
#define ENG_NEGS_000A0000_00_LEN            ENERGY_LEN
#define ENG_NEGS_000A0000_TOT_LEN           TOTAL_PHASE_ENERGY_LEN

#define ENG_MERGED_PHASE_TOT_LEN            (TOTAL_PHASE_ENERGY_LEN * 11)

//-----------------------------------------------------------------------------
// ����������ĺ궨��
//-----------------------------------------------------------------------------
//#define SINGLE_PHASE_ENERGY_LEN             (10 * ENERGY_LEN + CRC_LEN)                                     //������������ݼ��ϵĳ���
//#define SINGLE_PHASE_ALL_ENERGY_LEN         (SINGLE_PHASE_ENERGY_LEN + RELATED_ENERGY_LEN)                  //�������е��������ݼ��ϵĳ���

//A�������
#define ENG_APOSP_00150000_00_ADDR          (ENG_NEGS_000A0000_00_ADDR + ENG_NEGS_000A0000_TOT_LEN)         //A�������й�������
#define ENG_APOSP_00150000_00_LEN           ENERGY_LEN

#define ENG_ANEGP_00160000_00_ADDR          (ENG_APOSP_00150000_00_ADDR + ENG_APOSP_00150000_00_LEN)        //A�෴���й�����
#define ENG_ANEGP_00160000_00_LEN           ENERGY_LEN

#define ENG_ACOMQ1_00170000_00_ADDR         (ENG_ANEGP_00160000_00_ADDR + ENG_ANEGP_00160000_00_LEN)        //A������޹�1����
#define ENG_ACOMQ1_00170000_00_LEN          ENERGY_LEN

#define ENG_ACOMQ2_00180000_00_ADDR         (ENG_ACOMQ1_00170000_00_ADDR + ENG_ACOMQ1_00170000_00_LEN)      //A������޹�2����
#define ENG_ACOMQ2_00180000_00_LEN          ENERGY_LEN

#define ENG_AREA1Q_00190000_00_ADDR         (ENG_ACOMQ2_00180000_00_ADDR + ENG_ACOMQ2_00180000_00_LEN)      //A���һ�����޹�����
#define ENG_AREA1Q_00190000_00_LEN           ENERGY_LEN

#define ENG_AREA2Q_001A0000_00_ADDR         (ENG_AREA1Q_00190000_00_ADDR + ENG_AREA1Q_00190000_00_LEN)      //A��ڶ������޹�����
#define ENG_AREA2Q_001A0000_00_LEN          ENERGY_LEN

#define ENG_AREA3Q_001B0000_00_ADDR          (ENG_AREA2Q_001A0000_00_ADDR + ENG_AREA2Q_001A0000_00_LEN)      //A����������޹�����
#define ENG_AREA3Q_001B0000_00_LEN           ENERGY_LEN

#define ENG_AREA4Q_001C0000_00_ADDR          (ENG_AREA3Q_001B0000_00_ADDR + ENG_AREA3Q_001B0000_00_LEN)      //A����������޹�����
#define ENG_AREA4Q_001C0000_00_LEN           ENERGY_LEN

#define ENG_APOSS_001D0000_00_ADDR          (ENG_AREA4Q_001C0000_00_ADDR + ENG_AREA4Q_001C0000_00_LEN)        //A���������ڵ�����
#define ENG_APOSS_001D0000_00_LEN           ENERGY_LEN

#define ENG_ANEGS_001E0000_00_ADDR          (ENG_APOSS_001D0000_00_ADDR + ENG_APOSS_001D0000_00_LEN)        //A�෴�����ڵ�����
#define ENG_ANEGS_001E0000_00_LEN           ENERGY_LEN

//B�������
#ifdef PHASE_3_LINE_4
#define ENG_BPOSP_00290000_00_ADDR          (ENG_ANEGS_001E0000_00_ADDR + ENG_ANEGS_001E0000_00_LEN)        //B�������й�������
#define ENG_BPOSP_00290000_00_LEN           ENERGY_LEN

#define ENG_BNEGP_002A0000_00_ADDR          (ENG_BPOSP_00290000_00_ADDR + ENG_BPOSP_00290000_00_LEN)        //B�෴���й�����
#define ENG_BNEGP_002A0000_00_LEN           ENERGY_LEN

#define ENG_BCOMQ1_002B0000_00_ADDR         (ENG_BNEGP_002A0000_00_ADDR + ENG_BNEGP_002A0000_00_LEN)        //B������޹�1����
#define ENG_BCOMQ1_002B0000_00_LEN          ENERGY_LEN

#define ENG_BCOMQ2_002C0000_00_ADDR         (ENG_BCOMQ1_002B0000_00_ADDR + ENG_BCOMQ1_002B0000_00_LEN)      //B������޹�2����
#define ENG_BCOMQ2_002C0000_00_LEN          ENERGY_LEN

#define ENG_BREA1Q_002D0000_00_ADDR         (ENG_BCOMQ2_002C0000_00_ADDR + ENG_BCOMQ2_002C0000_00_LEN)      //B���һ�����޹�����
#define ENG_BREA1Q_002D0000_00_LEN          ENERGY_LEN

#define ENG_BREA2Q_002E0000_00_ADDR         (ENG_BREA1Q_002D0000_00_ADDR + ENG_BREA1Q_002D0000_00_LEN)      //B��ڶ������޹�����
#define ENG_BREA2Q_002E0000_00_LEN          ENERGY_LEN

#define ENG_BREA3Q_002F0000_00_ADDR          (ENG_BREA2Q_002E0000_00_ADDR + ENG_BREA2Q_002E0000_00_LEN)      //B����������޹�����
#define ENG_BREA3Q_002F0000_00_LEN           ENERGY_LEN

#define ENG_BREA4Q_00300000_00_ADDR          (ENG_BREA3Q_002F0000_00_ADDR + ENG_BREA3Q_002F0000_00_LEN)      //B����������޹�����
#define ENG_BREA4Q_00300000_00_LEN           ENERGY_LEN

#define ENG_BPOSS_00310000_00_ADDR          (ENG_BREA4Q_00300000_00_ADDR + ENG_BREA4Q_00300000_00_LEN)        //B���������ڵ�����
#define ENG_BPOSS_00310000_00_LEN           ENERGY_LEN

#define ENG_BNEGS_00320000_00_ADDR          (ENG_BPOSS_00310000_00_ADDR + ENG_BPOSS_00310000_00_LEN)        //B�෴�����ڵ�����
#define ENG_BNEGS_00320000_00_LEN           ENERGY_LEN

//C�������
#define ENG_CPOSP_003D0000_00_ADDR          (ENG_BNEGS_00320000_00_ADDR + ENG_BNEGS_00320000_00_LEN)        //C�������й�������
#define ENG_CPOSP_003D0000_00_LEN           ENERGY_LEN
#else   //PHASE_3_LINE_4
//#define ENG_CPOSP_003D0000_00_ADDR          (ENG_ANEGS_001E0000_00_ADDR + ENG_ANEGS_001E0000_00_LEN)        //C�������й�������
//#define ENG_CPOSP_003D0000_00_LEN           ENERGY_LEN
#endif  //PHASE_3_LINE_4
#define ENG_CNEGP_003E0000_00_ADDR          (ENG_CPOSP_003D0000_00_ADDR + ENG_CPOSP_003D0000_00_LEN)        //C�෴���й�����
#define ENG_CNEGP_003E0000_00_LEN           ENERGY_LEN

#define ENG_CCOMQ1_003F0000_00_ADDR         (ENG_CNEGP_003E0000_00_ADDR + ENG_CNEGP_003E0000_00_LEN)        //C������޹�1����
#define ENG_CCOMQ1_003F0000_00_LEN          ENERGY_LEN

#define ENG_CCOMQ2_00400000_00_ADDR         (ENG_CCOMQ1_003F0000_00_ADDR + ENG_CCOMQ1_003F0000_00_LEN)      //C������޹�2����
#define ENG_CCOMQ2_00400000_00_LEN          ENERGY_LEN

#define ENG_CREA1Q_00410000_00_ADDR         (ENG_CCOMQ2_00400000_00_ADDR + ENG_CCOMQ2_00400000_00_LEN)      //C���һ�����޹�����
#define ENG_CREA1Q_00410000_00_LEN          ENERGY_LEN

#define ENG_CREA2Q_00420000_00_ADDR         (ENG_CREA1Q_00410000_00_ADDR + ENG_CREA1Q_00410000_00_LEN)      //C��ڶ������޹�����
#define ENG_CREA2Q_00420000_00_LEN          ENERGY_LEN

#define ENG_CREA3Q_00430000_00_ADDR         (ENG_CREA2Q_00420000_00_ADDR + ENG_CREA2Q_00420000_00_LEN)      //C����������޹�����
#define ENG_CREA3Q_00430000_00_LEN          ENERGY_LEN

#define ENG_CREA4Q_00440000_00_ADDR         (ENG_CREA3Q_00430000_00_ADDR + ENG_CREA3Q_00430000_00_LEN)        //C����������޹�����
#define ENG_CREA4Q_00440000_00_LEN          ENERGY_LEN

#define ENG_CPOSS_00450000_00_ADDR          (ENG_CREA4Q_00440000_00_ADDR + ENG_CREA4Q_00440000_00_LEN)        //C���������ڵ�����
#define ENG_CPOSS_00450000_00_LEN           ENERGY_LEN

#define ENG_CNEGS_00460000_00_ADDR          (ENG_CPOSS_00450000_00_ADDR + ENG_CPOSS_00450000_00_LEN)          //C�෴�����ڵ�����
#define ENG_CNEGS_00460000_00_LEN           ENERGY_LEN
#define ENG_CNEGS_00460000_TOT_LEN          (ENERGY_LEN + CRC_LEN)
#ifdef PHASE_3_LINE_4
#define ENG_SEPARATE_PHASE_TOT_LEN          ((ENERGY_LEN * 10) * 3 + CRC_LEN)
#else   //PHASE_3_LINE_4
//#define ENG_SEPARATE_PHASE_TOT_LEN          ((ENERGY_LEN * 10) * 2 + CRC_LEN)
#endif  //PHASE_3_LINE_4

//-----------------------------------------------------------------------------
// ������������г���������ĺ궨��(�ϡ�����):������չDI[]: 0x070001-01-09-00
// �����ܵ���
// �����й������ܵ���
// �����й������ܵ���
// �����й�г���ܵ���
// �����й�г���ܵ���

// ��������_A��
// �����й������ܵ���_A��
// �����й������ܵ���_A��
// �����й�г���ܵ���_A��
// �����й�г���ܵ���_A��

// ��������_B��
// �����й������ܵ���_B��
// �����й������ܵ���_B��
// �����й�г���ܵ���_B��
// �����й�г���ܵ���_B��

// ��������_C��
// �����й������ܵ���_C��
// �����й������ܵ���_C��
// �����й�г���ܵ���_C��
// �����й�г���ܵ���_C��

// CRC(2Bytes)
//-----------------------------------------------------------------------------
//����
#define ENG_RELATOTP_00800000_00_ADDR       (ENG_CNEGS_00460000_00_ADDR+ENG_CNEGS_00460000_TOT_LEN)         //����������
#define ENG_RELATOTP_00800000_00_LEN        ENERGY_LEN
#define ENG_RELAPOSBP_00810000_00_ADDR      (ENG_RELATOTP_00800000_00_ADDR+ENG_RELATOTP_00800000_00_LEN)    //��������й�������
#define ENG_RELAPOSBP_00810000_00_LEN       ENERGY_LEN
#define ENG_RELANEGBP_00820000_00_ADDR      (ENG_RELAPOSBP_00810000_00_ADDR+ENG_RELAPOSBP_00810000_00_LEN)  //��������й�������
#define ENG_RELANEGBP_00820000_00_LEN       ENERGY_LEN
#define ENG_RELAPOSHP_00830000_00_ADDR      (ENG_RELANEGBP_00820000_00_ADDR+ENG_RELANEGBP_00820000_00_LEN)  //����г���й�������
#define ENG_RELAPOSHP_00830000_00_LEN       ENERGY_LEN
#define ENG_RELANEGHP_00840000_00_ADDR      (ENG_RELAPOSHP_00830000_00_ADDR+ENG_RELAPOSHP_00830000_00_LEN)  //����г���й�������
#define ENG_RELANEGHP_00840000_00_LEN       ENERGY_LEN
//#ifdef LINE_LOSS//LINE_LOSS
#define ENG_RELACOPCP_00850000_00_ADDR      (ENG_RELANEGHP_00840000_00_ADDR+ENG_RELANEGHP_00840000_00_LEN)  //ͭ���й��ܵ��ܲ�����
#define ENG_RELACOPCP_00850000_00_LEN       ENERGY_LEN
#define ENG_RELAIROCP_00860000_00_ADDR      (ENG_RELACOPCP_00850000_00_ADDR+ENG_RELACOPCP_00850000_00_LEN)  //�����й��ܵ��ܲ�����
#define ENG_RELAIROCP_00860000_00_LEN       ENERGY_LEN
#define ENG_RELATOTP_00800000_TOT_LEN       RELATED_ENERGY_LEN                                              //�������������ݼ��ϵĳ���

//A��
#define ENG_ARELATOTP_00940000_00_ADDR      (ENG_RELAIROCP_00860000_00_ADDR + ENG_RELAIROCP_00860000_00_LEN)    //A�����������
#define ENG_ARELATOTP_00940000_00_LEN       ENERGY_LEN
//#else   //LINE_LOSS
//#define ENG_ARELATOTP_00940000_00_ADDR      (ENG_RELANEGHP_00840000_00_ADDR + ENG_RELANEGHP_00840000_00_LEN)    //A�����������
//#define ENG_ARELATOTP_00940000_00_LEN       ENERGY_LEN
//#endif  //LINE_LOSS
#define ENG_ARELAPOSBP_00950000_00_ADDR     (ENG_ARELATOTP_00940000_00_ADDR + ENG_ARELATOTP_00940000_00_LEN)    //A����������й�������
#define ENG_ARELAPOSBP_00950000_00_LEN      ENERGY_LEN
#define ENG_ARELANEGBP_00960000_00_ADDR     (ENG_ARELAPOSBP_00950000_00_ADDR + ENG_ARELAPOSBP_00950000_00_LEN)  //A�෴������й�������
#define ENG_ARELANEGBP_00960000_00_LEN      ENERGY_LEN
#define ENG_ARELAPOSHP_00970000_00_ADDR     (ENG_ARELANEGBP_00960000_00_ADDR + ENG_ARELANEGBP_00960000_00_LEN)  //A������г���й�������
#define ENG_ARELAPOSHP_00970000_00_LEN      ENERGY_LEN
#define ENG_ARELANEGHP_00980000_00_ADDR     (ENG_ARELAPOSHP_00970000_00_ADDR + ENG_ARELAPOSHP_00970000_00_LEN)  //A�෴��г���й�������
#define ENG_ARELANEGHP_00980000_00_LEN      ENERGY_LEN
//#ifdef LINE_LOSS//LINE_LOSS
#define ENG_ARELACOPCP_00990000_00_ADDR     (ENG_ARELANEGHP_00980000_00_ADDR + ENG_ARELANEGHP_00980000_00_LEN)  //A��ͭ���й����ܲ�����
#define ENG_ARELACOPCP_00990000_00_LEN      ENERGY_LEN
#define ENG_ARELAIROCP_009A0000_00_ADDR     (ENG_ARELACOPCP_00990000_00_ADDR + ENG_ARELACOPCP_00990000_00_LEN)  //A�������й����ܲ�����
#define ENG_ARELAIROCP_009A0000_00_LEN      ENERGY_LEN
//#endif  //LINE_LOSS
#define ENG_ARELATOTP_00940000_TOT_LEN      RELATED_ENERGY_LEN                                              //A��������������ݼ��ϵĳ���

//B��
#ifdef PHASE_3_LINE_4
//#ifdef LINE_LOSS//LINE_LOSS
#define ENG_BRELATOTP_00A80000_00_ADDR      (ENG_ARELAIROCP_009A0000_00_ADDR + ENG_ARELAIROCP_009A0000_00_LEN) //B�����������
#define ENG_BRELATOTP_00A80000_00_LEN       ENERGY_LEN
//#else   //LINE_LOSS
//#define ENG_BRELATOTP_00A80000_00_ADDR      (ENG_ARELANEGHP_00980000_00_ADDR + ENG_ARELANEGHP_00980000_00_LEN) //B�����������
//#define ENG_BRELATOTP_00A80000_00_LEN       ENERGY_LEN
//#endif   //LINE_LOSS
#define ENG_BRELAPOSBP_00A90000_00_ADDR     (ENG_BRELATOTP_00A80000_00_ADDR + ENG_BRELATOTP_00A80000_00_LEN)    //B����������й�������
#define ENG_BRELAPOSBP_00A90000_00_LEN      ENERGY_LEN
#define ENG_BRELANEGBP_00AA0000_00_ADDR     (ENG_BRELAPOSBP_00A90000_00_ADDR + ENG_BRELAPOSBP_00A90000_00_LEN)  //B�෴������й�������
#define ENG_BRELANEGBP_00AA0000_00_LEN      ENERGY_LEN
#define ENG_BRELAPOSHP_00AB0000_00_ADDR     (ENG_BRELANEGBP_00AA0000_00_ADDR + ENG_BRELANEGBP_00AA0000_00_LEN)  //B������г���й�������
#define ENG_BRELAPOSHP_00AB0000_00_LEN      ENERGY_LEN
#define ENG_BRELANEGHP_00AC0000_00_ADDR     (ENG_BRELAPOSHP_00AB0000_00_ADDR + ENG_BRELAPOSHP_00AB0000_00_LEN)  //B�෴��г���й�������
#define ENG_BRELANEGHP_00AC0000_00_LEN      ENERGY_LEN
//#ifdef LINE_LOSS//LINE_LOSS
#define ENG_BRELACOPCP_00AD0000_00_ADDR     (ENG_BRELANEGHP_00AC0000_00_ADDR + ENG_BRELANEGHP_00AC0000_00_LEN)  //B��ͭ���й����ܲ�����
#define ENG_BRELACOPCP_00AD0000_00_LEN      ENERGY_LEN
#define ENG_BRELAIROCP_00AE0000_00_ADDR     (ENG_BRELACOPCP_00AD0000_00_ADDR + ENG_BRELACOPCP_00AD0000_00_LEN)  //B�������й����ܲ�����
#define ENG_BRELAIROCP_00AE0000_00_LEN      ENERGY_LEN
//#endif   //LINE_LOSS
#define ENG_BRELATOTP_00A80000_TOT_LEN      RELATED_ENERGY_LEN                                                  //B��������������ݼ��ϵĳ���

//C��
//#ifdef LINE_LOSS//LINE_LOSS
#define ENG_CRELATOTP_00BC0000_00_ADDR      (ENG_BRELAIROCP_00AE0000_00_ADDR + ENG_BRELAIROCP_00AE0000_00_LEN)  //C�����������
#define ENG_CRELATOTP_00BC0000_00_LEN       ENERGY_LEN
//#else   //LINE_LOSS
//#define ENG_CRELATOTP_00BC0000_00_ADDR      (ENG_BRELANEGHP_00AC0000_00_ADDR + ENG_BRELANEGHP_00AC0000_00_LEN)  //C�����������
//#define ENG_CRELATOTP_00BC0000_00_LEN       ENERGY_LEN
//#endif  //LINE_LOSS

#else   //PHASE_3_LINE_4

//#ifdef LINE_LOSS//LINE_LOSS
//#define ENG_CRELATOTP_00BC0000_00_ADDR      (ENG_ARELAIROCP_009A0000_00_ADDR + ENG_ARELAIROCP_009A0000_00_LEN)  //C�����������
//#define ENG_CRELATOTP_00BC0000_00_LEN       ENERGY_LEN
//#else   //LINE_LOSS
//#define ENG_CRELATOTP_00BC0000_00_ADDR      (ENG_ARELANEGHP_00980000_00_ADDR + ENG_ARELANEGHP_00980000_00_LEN)  //C�����������
//#define ENG_CRELATOTP_00BC0000_00_LEN       ENERGY_LEN
//#endif  //LINE_LOSS

#endif  //PHASE_3_LINE_4

#define ENG_CRELAPOSBP_00BD0000_00_ADDR     (ENG_CRELATOTP_00BC0000_00_ADDR + ENG_CRELATOTP_00BC0000_00_LEN)    //C����������й�������
#define ENG_CRELAPOSBP_00BD0000_00_LEN      ENERGY_LEN
#define ENG_CRELANEGBP_00BE0000_00_ADDR     (ENG_CRELAPOSBP_00BD0000_00_ADDR + ENG_CRELAPOSBP_00BD0000_00_LEN)  //C�෴������й�������
#define ENG_CRELANEGBP_00BE0000_00_LEN      ENERGY_LEN
#define ENG_CRELAPOSHP_00BF0000_00_ADDR     (ENG_CRELANEGBP_00BE0000_00_ADDR + ENG_CRELANEGBP_00BE0000_00_LEN)  //C������г���й�������
#define ENG_CRELAPOSHP_00BF0000_00_LEN      ENERGY_LEN
#define ENG_CRELANEGHP_00C00000_00_ADDR     (ENG_CRELAPOSHP_00BF0000_00_ADDR + ENG_CRELAPOSHP_00BF0000_00_LEN)  //C�෴��г���й�������
#define ENG_CRELANEGHP_00C00000_00_LEN      ENERGY_LEN
#define ENG_CRELANEGHP_00C00000_TOT_LEN     (ENERGY_LEN + CRC_LEN)
//#ifdef LINE_LOSS//LINE_LOSS
#define ENG_CRELACOPCP_00C10000_00_ADDR     (ENG_CRELANEGHP_00C00000_00_ADDR + ENG_CRELANEGHP_00C00000_00_LEN)  //C��ͭ���й����ܲ�����
#define ENG_CRELACOPCP_00C10000_00_LEN      ENERGY_LEN
#define ENG_CRELAIROCP_00C20000_00_ADDR     (ENG_CRELACOPCP_00C10000_00_ADDR + ENG_CRELACOPCP_00C10000_00_LEN)  //C�������й����ܲ�����
#define ENG_CRELAIROCP_00C20000_00_LEN      ENERGY_LEN
#define ENG_CRELAIROCP_00C20000_TOT_LEN     (ENERGY_LEN + CRC_LEN)
//#endif  //LINE_LOSS
#define ENG_CRELATOTP_00BC0000_TOT_LEN      RELATED_ENERGY_LEN                                              //C��������������ݼ��ϵĳ���
#ifdef PHASE_3_LINE_4
#define ENG_ALL_RELATOTP_TOT_LEN            (RELATED_ENERGY_LEN * 4 + CRC_LEN)  //�ϡ�����������������ݿ��ܳ���
#else   //PHASE_3_LINE_4
//#define ENG_ALL_RELATOTP_TOT_LEN            (RELATED_ENERGY_LEN * 3 + CRC_LEN)  //�ϡ�����������������ݿ��ܳ���
#endif  //PHASE_3_LINE_4

#define ENG_RAM_ARRAY_MAX   (ENG_MERGED_PHASE_TOT_LEN + ENG_SEPARATE_PHASE_TOT_LEN + ENG_ALL_RELATOTP_TOT_LEN)//��ǰ����RAM��������[ÿ���ݿ�CRCռ1����]


//��1~12��������Ҫ���� ��ʼ[��δӦ�õ�DLT645]
#define ENG_LASTXMONTH_00000000_00_OFFSET_MAXNUM    12  //
//#ifdef LINE_LOSS//LINE_LOSS
#define ENG_LASTXMONTH_00000000_00_OFFSET_ADDR      (ENG_CRELAIROCP_00C20000_00_ADDR+ENG_CRELAIROCP_00C20000_TOT_LEN)//����ָ�뱣��ĵ�ַ[�ݲ�����ͭ����������]
//#else   //LINE_LOSS
//#define ENG_LASTXMONTH_00000000_00_OFFSET_ADDR      (ENG_CRELANEGHP_00C00000_00_ADDR+ENG_CRELANEGHP_00C00000_TOT_LEN)//����ָ�뱣��ĵ�ַ[�ݲ�����ͭ����������]
//#endif  //LINE_LOSS
#define ENG_LASTXMONTH_00000000_00_OFFSET_LEN       3 + CRC_LEN  //����ָ�볤��[CRC�����б�ָ�����Ч��]
#define ENG_LASTXMONTH_00000000_00_OFFSET_BLOCKLEN  ENG_RAM_ARRAY_MAX//���ε��������������ݳ���
#define ENG_LASTXMONTH_00000000_00_HEAD_ADDR        (ENG_LASTXMONTH_00000000_00_OFFSET_ADDR+ENG_LASTXMONTH_00000000_00_OFFSET_LEN)
#define ENG_LASTXMONTH_00000000_00_END_ADDR         (ENG_LASTXMONTH_00000000_00_HEAD_ADDR+ENG_LASTXMONTH_00000000_00_OFFSET_BLOCKLEN*ENG_LASTXMONTH_00000000_00_OFFSET_MAXNUM)
//��1~12��������Ҫ���� ����


/*
//---------------------------- ��ʹ��new_convert���߿����ַʱ��Ҫ�Ķ�, ȷ�����������ֱ���
//#if METER_TYPE == PHASE_3_LINE_4
//#define PHASE_B_ENERGY_ADDR                 (PHASE_A_RELATED_ENERGY_ADDR + RELATED_ENERGY_LEN)                  //B�������
//#define PHASE_B_RELATED_ENERGY_ADDR         (PHASE_B_ENERGY_ADDR + SINGLE_PHASE_ENERGY_LEN)                     //B�����������
//#define PHASE_C_ENERGY_ADDR                 (PHASE_B_RELATED_ENERGY_ADDR + RELATED_ENERGY_LEN)                  //C�������
//#define PHASE_C_RELATED_ENERGY_ADDR         (PHASE_C_ENERGY_ADDR + SINGLE_PHASE_ENERGY_LEN)                     //C�����������
//#else//METER_TYPE == PHASE_3_LINE_4
//#define PHASE_C_ENERGY_ADDR                 (PHASE_A_RELATED_ENERGY_ADDR + RELATED_ENERGY_LEN)                  //C�������
//#define PHASE_C_RELATED_ENERGY_ADDR         (PHASE_C_ENERGY_ADDR + SINGLE_PHASE_ENERGY_LEN)                     //C�����������
//#endif//METER_TYPE == PHASE_3_LINE_4

//-----------------------------------------------------------------------------
// ���������ݵĺ궨�壬���������뵱ǰ������������һ����
//-----------------------------------------------------------------------------
//#define ENERGY_BACKUP_NUM                   3           //���������ݵĸ���
//#define ENERGY_BACKUP_LEN                   (PHASE_C_RELATED_ENERGY_ADDR + RELATED_ENERGY_LEN - COMB_ACTIVE_ENERGY_ADDR)
//
//#define ENERGY_BACKUP_POINTER               (PHASE_C_RELATED_ENERGY_ADDR + RELATED_ENERGY_LEN)                  //��ǰ���������ݵ�ָ�룬����ָ�����¼�¼���һ���ռ�¼�����ϵļ�¼
//#define ENERGY_BACKUP_POINTER_LEN           1
//#define ENERGY_BACKUP_ADDR                  (ENERGY_BACKUP_POINTER + ENERGY_BACKUP_POINTER_LEN)                 //������������
*/

//-----------------------------------------------------------------------------
// �洢��ʽ������ʱ�� + �������
// ����ʱ�����ݣ�mm���֣�hh��ʱ��DD���գ�MM���£�YY���꣩��ʮ���Ƹ�ʽ
// ����������ݣ�3�ֽ�ʮ���Ƹ�ʽ����λ��ǰ
//-----------------------------------------------------------------------------
// ���������ĺ궨��
//-----------------------------------------------------------------------------
#define DEMAND_JUST_LEN                         3                   //��������������������ʾ
#define DEMAND_HM_LEN                           2                   //�������ݵ�ʱ��������ʾ
#define DEMAND_YMD_LEN                          3                   //�������ݵ�������������ʾ
#define DEMAND_LEN                              (DEMAND_JUST_LEN+DEMAND_HM_LEN+DEMAND_YMD_LEN)//07��Լ�����������ݵĳ���
#define TOTAL_PHASE_DEMAND_LEN                  ((RATE_NUM + 1) * DEMAND_LEN)  //�����������ݼ��ϵĳ���
//��
//����1
//����2
//...
//����n(nȡ����RATE_NUM)
#define DMD_STORE_START_ADDR                ENG_LASTXMONTH_00000000_00_END_ADDR //������ַ
#define DMD_POSP_01010000_00_ADDR           (DMD_STORE_START_ADDR)              //�����й�����
#define DMD_POSP_01010000_00_LEN            DEMAND_JUST_LEN
#define DMD_POSP_01010000_01_ADDR           (DMD_POSP_01010000_00_ADDR+DMD_POSP_01010000_00_LEN)//�����й�����:ʱ��
#define DMD_POSP_01010000_01_LEN            DEMAND_HM_LEN
#define DMD_POSP_01010000_02_ADDR           (DMD_POSP_01010000_01_ADDR+DMD_POSP_01010000_01_LEN)//�����й�����:������
#define DMD_POSP_01010000_02_LEN            DEMAND_YMD_LEN
#define DMD_POSP_01010000_LEN               DEMAND_LEN  //8
#define DMD_POSP_01010000_TOT_LEN           TOTAL_PHASE_DEMAND_LEN

#define DMD_NEGP_01020000_00_ADDR           (DMD_POSP_01010000_00_ADDR+DMD_POSP_01010000_TOT_LEN)//�����й�����
#define DMD_NEGP_01020000_00_LEN            DEMAND_JUST_LEN
#define DMD_NEGP_01020000_01_ADDR           (DMD_NEGP_01020000_00_ADDR+DMD_NEGP_01020000_00_LEN)//�����й�����:ʱ��
#define DMD_NEGP_01020000_01_LEN            DEMAND_HM_LEN
#define DMD_NEGP_01020000_02_ADDR           (DMD_NEGP_01020000_01_ADDR+DMD_NEGP_01020000_01_LEN)//�����й�����:������
#define DMD_NEGP_01020000_02_LEN            DEMAND_YMD_LEN
#define DMD_NEGP_01020000_LEN               DEMAND_LEN  //8
#define DMD_NEGP_01020000_TOT_LEN           TOTAL_PHASE_DEMAND_LEN

#define DMD_COMQ1_01030000_00_ADDR          (DMD_NEGP_01020000_00_ADDR+DMD_NEGP_01020000_TOT_LEN)//���1�޹�����
#define DMD_COMQ1_01030000_00_LEN           DEMAND_JUST_LEN
#define DMD_COMQ1_01030000_01_ADDR          (DMD_COMQ1_01030000_00_ADDR+DMD_COMQ1_01030000_00_LEN)//���1�޹�����:ʱ��
#define DMD_COMQ1_01030000_01_LEN           DEMAND_HM_LEN
#define DMD_COMQ1_01030000_02_ADDR          (DMD_COMQ1_01030000_01_ADDR+DMD_COMQ1_01030000_01_LEN)//���1�޹�����:������
#define DMD_COMQ1_01030000_02_LEN           DEMAND_YMD_LEN
#define DMD_COMQ1_01030000_LEN              DEMAND_LEN  //8
#define DMD_COMQ1_01030000_TOT_LEN          TOTAL_PHASE_DEMAND_LEN

#define DMD_COMQ2_01040000_00_ADDR          (DMD_COMQ1_01030000_00_ADDR+DMD_COMQ1_01030000_TOT_LEN)//���2�޹�����
#define DMD_COMQ2_01040000_00_LEN           DEMAND_JUST_LEN
#define DMD_COMQ2_01040000_01_ADDR          (DMD_COMQ2_01040000_00_ADDR+DMD_COMQ2_01040000_00_LEN)//���2�޹�����:ʱ��
#define DMD_COMQ2_01040000_01_LEN           DEMAND_HM_LEN
#define DMD_COMQ2_01040000_02_ADDR          (DMD_COMQ2_01040000_01_ADDR+DMD_COMQ2_01040000_01_LEN)//���2�޹�����:������
#define DMD_COMQ2_01040000_02_LEN           DEMAND_YMD_LEN
#define DMD_COMQ2_01040000_LEN              DEMAND_LEN  //8
#define DMD_COMQ2_01040000_TOT_LEN          TOTAL_PHASE_DEMAND_LEN

#define DMD_REQ1Q_01050000_00_ADDR          (DMD_COMQ2_01040000_00_ADDR+DMD_COMQ2_01040000_TOT_LEN)//����1�޹�����
#define DMD_REQ1Q_01050000_00_LEN           DEMAND_JUST_LEN
#define DMD_REQ1Q_01050000_01_ADDR          (DMD_REQ1Q_01050000_00_ADDR+DMD_REQ1Q_01050000_00_LEN)//����1�޹�����:ʱ��
#define DMD_REQ1Q_01050000_01_LEN           DEMAND_HM_LEN
#define DMD_REQ1Q_01050000_02_ADDR          (DMD_REQ1Q_01050000_01_ADDR+DMD_REQ1Q_01050000_01_LEN)//����1�޹�����:������
#define DMD_REQ1Q_01050000_02_LEN           DEMAND_YMD_LEN
#define DMD_REQ1Q_01050000_LEN              DEMAND_LEN  //8
#define DMD_REQ1Q_01050000_TOT_LEN          TOTAL_PHASE_DEMAND_LEN

#define DMD_REQ2Q_01060000_00_ADDR          (DMD_REQ1Q_01050000_00_ADDR+DMD_REQ1Q_01050000_TOT_LEN)//����2�޹�����
#define DMD_REQ2Q_01060000_00_LEN           DEMAND_JUST_LEN
#define DMD_REQ2Q_01060000_01_ADDR          (DMD_REQ2Q_01060000_00_ADDR+DMD_REQ2Q_01060000_00_LEN)//����2�޹�����:ʱ��
#define DMD_REQ2Q_01060000_01_LEN           DEMAND_HM_LEN
#define DMD_REQ2Q_01060000_02_ADDR          (DMD_REQ2Q_01060000_01_ADDR+DMD_REQ2Q_01060000_01_LEN)//����2�޹�����:������
#define DMD_REQ2Q_01060000_02_LEN           DEMAND_YMD_LEN
#define DMD_REQ2Q_01060000_LEN              DEMAND_LEN  //8
#define DMD_REQ2Q_01060000_TOT_LEN          TOTAL_PHASE_DEMAND_LEN

#define DMD_REQ3Q_01070000_00_ADDR          (DMD_REQ2Q_01060000_00_ADDR+DMD_REQ2Q_01060000_TOT_LEN)//����3�޹�����
#define DMD_REQ3Q_01070000_00_LEN           DEMAND_JUST_LEN
#define DMD_REQ3Q_01070000_01_ADDR          (DMD_REQ3Q_01070000_00_ADDR+DMD_REQ3Q_01070000_00_LEN)//����3�޹�����:ʱ��
#define DMD_REQ3Q_01070000_01_LEN           DEMAND_HM_LEN
#define DMD_REQ3Q_01070000_02_ADDR          (DMD_REQ3Q_01070000_01_ADDR+DMD_REQ3Q_01070000_01_LEN)//����3�޹�����:������
#define DMD_REQ3Q_01070000_02_LEN           DEMAND_YMD_LEN
#define DMD_REQ3Q_01070000_LEN              DEMAND_LEN  //8
#define DMD_REQ3Q_01070000_TOT_LEN          TOTAL_PHASE_DEMAND_LEN

#define DMD_REQ4Q_01080000_00_ADDR          (DMD_REQ3Q_01070000_00_ADDR+DMD_REQ3Q_01070000_TOT_LEN)//����4�޹�����
#define DMD_REQ4Q_01080000_00_LEN           DEMAND_JUST_LEN
#define DMD_REQ4Q_01080000_01_ADDR          (DMD_REQ4Q_01080000_00_ADDR+DMD_REQ4Q_01080000_00_LEN)//����4�޹�����:ʱ��
#define DMD_REQ4Q_01080000_01_LEN           DEMAND_HM_LEN
#define DMD_REQ4Q_01080000_02_ADDR          (DMD_REQ4Q_01080000_01_ADDR+DMD_REQ4Q_01080000_01_LEN)//����4�޹�����:������
#define DMD_REQ4Q_01080000_02_LEN           DEMAND_YMD_LEN
#define DMD_REQ4Q_01080000_LEN              DEMAND_LEN  //8
#define DMD_REQ4Q_01080000_TOT_LEN          TOTAL_PHASE_DEMAND_LEN

#define DMD_POSS_01090000_00_ADDR           (DMD_REQ4Q_01080000_00_ADDR+DMD_REQ4Q_01080000_TOT_LEN)//������������
#define DMD_POSS_01090000_00_LEN            DEMAND_JUST_LEN
#define DMD_POSS_01090000_01_ADDR           (DMD_POSS_01090000_00_ADDR+DMD_POSS_01090000_00_LEN)//������������:ʱ��
#define DMD_POSS_01090000_01_LEN            DEMAND_HM_LEN
#define DMD_POSS_01090000_02_ADDR           (DMD_POSS_01090000_01_ADDR+DMD_POSS_01090000_01_LEN)//������������:������
#define DMD_POSS_01090000_02_LEN            DEMAND_YMD_LEN
#define DMD_POSS_01090000_LEN               DEMAND_LEN  //8
#define DMD_POSS_01090000_TOT_LEN           TOTAL_PHASE_DEMAND_LEN

#define DMD_NEGS_010A0000_00_ADDR           (DMD_POSS_01090000_00_ADDR+DMD_POSS_01090000_TOT_LEN)//������������
#define DMD_NEGS_010A0000_00_LEN            DEMAND_JUST_LEN
#define DMD_NEGS_010A0000_01_ADDR           (DMD_NEGS_010A0000_00_ADDR+DMD_NEGS_010A0000_00_LEN)//������������:ʱ��
#define DMD_NEGS_010A0000_01_LEN            DEMAND_HM_LEN
#define DMD_NEGS_010A0000_02_ADDR           (DMD_NEGS_010A0000_01_ADDR+DMD_NEGS_010A0000_01_LEN)//������������:������
#define DMD_NEGS_010A0000_02_LEN            DEMAND_YMD_LEN
#define DMD_NEGS_010A0000_LEN               DEMAND_LEN  //8
#define DMD_NEGS_010A0000_TOT_LEN           TOTAL_PHASE_DEMAND_LEN

//-----------------------------------------------------------------------------
// ���������ĺ궨��
//-----------------------------------------------------------------------------
#define SINGLE_PHASE_DEMAND_LEN                 (10 * DEMAND_LEN)                                                   //�����������ݼ��ϵĳ���
//�����й�����
//�����й�����
//����޹�1����
//����޹�2����
//��һ�����޹�����
//�ڶ������޹�����
//���������޹�����
//���������޹�����
//������������
//������������
//------------------------------------------------------------------------------
//A������
#define DMD_APOSP_01150000_00_ADDR          (DMD_NEGS_010A0000_00_ADDR+DMD_NEGS_010A0000_TOT_LEN)//A�������й�����
#define DMD_APOSP_01150000_00_LEN           DEMAND_JUST_LEN
#define DMD_APOSP_01150000_01_ADDR          (DMD_APOSP_01150000_00_ADDR+DMD_APOSP_01150000_00_LEN)//A�������й�����:ʱ��
#define DMD_APOSP_01150000_01_LEN           DEMAND_HM_LEN
#define DMD_APOSP_01150000_02_ADDR          (DMD_APOSP_01150000_01_ADDR+DMD_APOSP_01150000_01_LEN)//A�������й�����:������
#define DMD_APOSP_01150000_02_LEN           DEMAND_YMD_LEN
#define DMD_APOSP_01150000_LEN              DEMAND_LEN  //8

#define DMD_ANEGP_01160000_00_ADDR          (DMD_APOSP_01150000_00_ADDR+DMD_APOSP_01150000_LEN)//A�෴���й�����
#define DMD_ANEGP_01160000_00_LEN           DEMAND_JUST_LEN
#define DMD_ANEGP_01160000_01_ADDR          (DMD_ANEGP_01160000_00_ADDR+DMD_ANEGP_01160000_00_LEN)//A�෴���й�����:ʱ��
#define DMD_ANEGP_01160000_01_LEN           DEMAND_HM_LEN
#define DMD_ANEGP_01160000_02_ADDR          (DMD_ANEGP_01160000_01_ADDR+DMD_ANEGP_01160000_01_LEN)//A�෴���й�����:������
#define DMD_ANEGP_01160000_02_LEN           DEMAND_YMD_LEN
#define DMD_ANEGP_01160000_LEN              DEMAND_LEN  //8

#define DMD_ACOMQ1_01170000_00_ADDR         (DMD_ANEGP_01160000_00_ADDR+DMD_ANEGP_01160000_LEN)//A�����1�޹�����
#define DMD_ACOMQ1_01170000_00_LEN          DEMAND_JUST_LEN
#define DMD_ACOMQ1_01170000_01_ADDR         (DMD_ACOMQ1_01170000_00_ADDR+DMD_ACOMQ1_01170000_00_LEN)//A�����1�޹�����:ʱ��
#define DMD_ACOMQ1_01170000_01_LEN          DEMAND_HM_LEN
#define DMD_ACOMQ1_01170000_02_ADDR         (DMD_ACOMQ1_01170000_01_ADDR+DMD_ACOMQ1_01170000_01_LEN)//A�����1�޹�����:������
#define DMD_ACOMQ1_01170000_02_LEN          DEMAND_YMD_LEN
#define DMD_ACOMQ1_01170000_LEN             DEMAND_LEN  //8

#define DMD_ACOMQ2_01180000_00_ADDR         (DMD_ACOMQ1_01170000_00_ADDR+DMD_ACOMQ1_01170000_LEN)//A�����2�޹�����
#define DMD_ACOMQ2_01180000_00_LEN          DEMAND_JUST_LEN
#define DMD_ACOMQ2_01180000_01_ADDR         (DMD_ACOMQ2_01180000_00_ADDR+DMD_ACOMQ2_01180000_00_LEN)//A�����2�޹�����:ʱ��
#define DMD_ACOMQ2_01180000_01_LEN          DEMAND_HM_LEN
#define DMD_ACOMQ2_01180000_02_ADDR         (DMD_ACOMQ2_01180000_01_ADDR+DMD_ACOMQ2_01180000_01_LEN)//A�����2�޹�����:������
#define DMD_ACOMQ2_01180000_02_LEN          DEMAND_YMD_LEN
#define DMD_ACOMQ2_01180000_LEN             DEMAND_LEN  //8

#define DMD_AREQ1Q_01190000_00_ADDR         (DMD_ACOMQ2_01180000_00_ADDR+DMD_ACOMQ2_01180000_LEN)//A������1�޹�����
#define DMD_AREQ1Q_01190000_00_LEN          DEMAND_JUST_LEN
#define DMD_AREQ1Q_01190000_01_ADDR         (DMD_AREQ1Q_01190000_00_ADDR+DMD_AREQ1Q_01190000_00_LEN)//A������1�޹�����:ʱ��
#define DMD_AREQ1Q_01190000_01_LEN          DEMAND_HM_LEN
#define DMD_AREQ1Q_01190000_02_ADDR         (DMD_AREQ1Q_01190000_01_ADDR+DMD_AREQ1Q_01190000_01_LEN)//A������1�޹�����:������
#define DMD_AREQ1Q_01190000_02_LEN          DEMAND_YMD_LEN
#define DMD_AREQ1Q_01190000_LEN             DEMAND_LEN  //8

#define DMD_AREQ2Q_011A0000_00_ADDR         (DMD_AREQ1Q_01190000_00_ADDR+DMD_AREQ1Q_01190000_LEN)//A������2�޹�����
#define DMD_AREQ2Q_011A0000_00_LEN          DEMAND_JUST_LEN
#define DMD_AREQ2Q_011A0000_01_ADDR         (DMD_AREQ2Q_011A0000_00_ADDR+DMD_AREQ2Q_011A0000_00_LEN)//A������2�޹�����:ʱ��
#define DMD_AREQ2Q_011A0000_01_LEN          DEMAND_HM_LEN
#define DMD_AREQ2Q_011A0000_02_ADDR         (DMD_AREQ2Q_011A0000_01_ADDR+DMD_AREQ2Q_011A0000_01_LEN)//A������2�޹�����:������
#define DMD_AREQ2Q_011A0000_02_LEN          DEMAND_YMD_LEN
#define DMD_AREQ2Q_011A0000_LEN             DEMAND_LEN  //8

#define DMD_AREQ3Q_011B0000_00_ADDR         (DMD_AREQ2Q_011A0000_00_ADDR+DMD_AREQ2Q_011A0000_LEN)//A������3�޹�����
#define DMD_AREQ3Q_011B0000_00_LEN          DEMAND_JUST_LEN
#define DMD_AREQ3Q_011B0000_01_ADDR         (DMD_AREQ3Q_011B0000_00_ADDR+DMD_AREQ3Q_011B0000_00_LEN)//A������3�޹�����:ʱ��
#define DMD_AREQ3Q_011B0000_01_LEN          DEMAND_HM_LEN
#define DMD_AREQ3Q_011B0000_02_ADDR         (DMD_AREQ3Q_011B0000_01_ADDR+DMD_AREQ3Q_011B0000_01_LEN)//A������3�޹�����:������
#define DMD_AREQ3Q_011B0000_02_LEN          DEMAND_YMD_LEN
#define DMD_AREQ3Q_011B0000_LEN             DEMAND_LEN  //8

#define DMD_AREQ4Q_011C0000_00_ADDR         (DMD_AREQ3Q_011B0000_00_ADDR+DMD_AREQ3Q_011B0000_LEN)//A������4�޹�����
#define DMD_AREQ4Q_011C0000_00_LEN          DEMAND_JUST_LEN
#define DMD_AREQ4Q_011C0000_01_ADDR         (DMD_AREQ4Q_011C0000_00_ADDR+DMD_AREQ4Q_011C0000_00_LEN)//A������4�޹�����:ʱ��
#define DMD_AREQ4Q_011C0000_01_LEN          DEMAND_HM_LEN
#define DMD_AREQ4Q_011C0000_02_ADDR         (DMD_AREQ4Q_011C0000_01_ADDR+DMD_AREQ4Q_011C0000_01_LEN)//A������4�޹�����:������
#define DMD_AREQ4Q_011C0000_02_LEN          DEMAND_YMD_LEN
#define DMD_AREQ4Q_011C0000_LEN             DEMAND_LEN  //8

#define DMD_APOSS_011D0000_00_ADDR          (DMD_AREQ4Q_011C0000_00_ADDR+DMD_AREQ4Q_011C0000_LEN)//A��������������
#define DMD_APOSS_011D0000_00_LEN           DEMAND_JUST_LEN
#define DMD_APOSS_011D0000_01_ADDR          (DMD_APOSS_011D0000_00_ADDR+DMD_APOSS_011D0000_00_LEN)//A��������������:ʱ��
#define DMD_APOSS_011D0000_01_LEN           DEMAND_HM_LEN
#define DMD_APOSS_011D0000_02_ADDR          (DMD_APOSS_011D0000_01_ADDR+DMD_APOSS_011D0000_01_LEN)//A��������������:������
#define DMD_APOSS_011D0000_02_LEN           DEMAND_YMD_LEN
#define DMD_APOSS_011D0000_LEN              DEMAND_LEN  //8

#define DMD_ANEGS_011E0000_00_ADDR          (DMD_APOSS_011D0000_00_ADDR+DMD_APOSS_011D0000_LEN)//A�෴����������
#define DMD_ANEGS_011E0000_00_LEN           DEMAND_JUST_LEN
#define DMD_ANEGS_011E0000_01_ADDR          (DMD_ANEGS_011E0000_00_ADDR+DMD_ANEGS_011E0000_00_LEN)//A�෴����������:ʱ��
#define DMD_ANEGS_011E0000_01_LEN           DEMAND_HM_LEN
#define DMD_ANEGS_011E0000_02_ADDR          (DMD_ANEGS_011E0000_01_ADDR+DMD_ANEGS_011E0000_01_LEN)//A�෴����������:������
#define DMD_ANEGS_011E0000_02_LEN           DEMAND_YMD_LEN
#define DMD_ANEGS_011E0000_LEN              DEMAND_LEN  //8
#define DMD_A_TOT_LEN                       SINGLE_PHASE_DEMAND_LEN        //A�������ܳ���

#ifdef PHASE_3_LINE_4
//B������
#define DMD_BPOSP_01290000_00_ADDR          (DMD_APOSP_01150000_00_ADDR+DMD_A_TOT_LEN)//B�������й�����
#define DMD_BPOSP_01290000_00_LEN           DEMAND_JUST_LEN
#define DMD_BPOSP_01290000_01_ADDR          (DMD_BPOSP_01290000_00_ADDR+DMD_BPOSP_01290000_00_LEN)//B�������й�����:ʱ��
#define DMD_BPOSP_01290000_01_LEN           DEMAND_HM_LEN
#define DMD_BPOSP_01290000_02_ADDR          (DMD_BPOSP_01290000_01_ADDR+DMD_BPOSP_01290000_01_LEN)//B�������й�����:������
#define DMD_BPOSP_01290000_02_LEN           DEMAND_YMD_LEN
#define DMD_BPOSP_01290000_LEN              DEMAND_LEN  //8

#define DMD_BNEGP_012A0000_00_ADDR          (DMD_BPOSP_01290000_00_ADDR+DMD_BPOSP_01290000_LEN)//B�෴���й�����
#define DMD_BNEGP_012A0000_00_LEN           DEMAND_JUST_LEN
#define DMD_BNEGP_012A0000_01_ADDR          (DMD_BNEGP_012A0000_00_ADDR+DMD_BNEGP_012A0000_00_LEN)//B�෴���й�����:ʱ��
#define DMD_BNEGP_012A0000_01_LEN           DEMAND_HM_LEN
#define DMD_BNEGP_012A0000_02_ADDR          (DMD_BNEGP_012A0000_01_ADDR+DMD_BNEGP_012A0000_01_LEN)//B�෴���й�����:������
#define DMD_BNEGP_012A0000_02_LEN           DEMAND_YMD_LEN
#define DMD_BNEGP_012A0000_LEN              DEMAND_LEN  //8

#define DMD_BCOMQ1_012B0000_00_ADDR         (DMD_BNEGP_012A0000_00_ADDR+DMD_BNEGP_012A0000_LEN)//B�����1�޹�����
#define DMD_BCOMQ1_012B0000_00_LEN          DEMAND_JUST_LEN
#define DMD_BCOMQ1_012B0000_01_ADDR         (DMD_BCOMQ1_012B0000_00_ADDR+DMD_BCOMQ1_012B0000_00_LEN)//B�����1�޹�����:ʱ��
#define DMD_BCOMQ1_012B0000_01_LEN          DEMAND_HM_LEN
#define DMD_BCOMQ1_012B0000_02_ADDR         (DMD_BCOMQ1_012B0000_01_ADDR+DMD_BCOMQ1_012B0000_01_LEN)//B�����1�޹�����:������
#define DMD_BCOMQ1_012B0000_02_LEN          DEMAND_YMD_LEN
#define DMD_BCOMQ1_012B0000_LEN             DEMAND_LEN  //8

#define DMD_BCOMQ2_012C0000_00_ADDR         (DMD_BCOMQ1_012B0000_00_ADDR+DMD_BCOMQ1_012B0000_LEN)//B�����2�޹�����
#define DMD_BCOMQ2_012C0000_00_LEN          DEMAND_JUST_LEN
#define DMD_BCOMQ2_012C0000_01_ADDR         (DMD_BCOMQ2_012C0000_00_ADDR+DMD_BCOMQ2_012C0000_00_LEN)//B�����2�޹�����:ʱ��
#define DMD_BCOMQ2_012C0000_01_LEN          DEMAND_HM_LEN
#define DMD_BCOMQ2_012C0000_02_ADDR         (DMD_BCOMQ2_012C0000_01_ADDR+DMD_BCOMQ2_012C0000_01_LEN)//B�����2�޹�����:������
#define DMD_BCOMQ2_012C0000_02_LEN          DEMAND_YMD_LEN
#define DMD_BCOMQ2_012C0000_LEN             DEMAND_LEN  //8

#define DMD_BREQ1Q_012D0000_00_ADDR         (DMD_BCOMQ2_012C0000_00_ADDR+DMD_BCOMQ2_012C0000_LEN)//B������1�޹�����
#define DMD_BREQ1Q_012D0000_00_LEN          DEMAND_JUST_LEN
#define DMD_BREQ1Q_012D0000_01_ADDR         (DMD_BREQ1Q_012D0000_00_ADDR+DMD_BREQ1Q_012D0000_00_LEN)//B������1�޹�����:ʱ��
#define DMD_BREQ1Q_012D0000_01_LEN          DEMAND_HM_LEN
#define DMD_BREQ1Q_012D0000_02_ADDR         (DMD_BREQ1Q_012D0000_01_ADDR+DMD_BREQ1Q_012D0000_01_LEN)//B������1�޹�����:������
#define DMD_BREQ1Q_012D0000_02_LEN          DEMAND_YMD_LEN
#define DMD_BREQ1Q_012D0000_LEN             DEMAND_LEN  //8

#define DMD_BREQ2Q_012E0000_00_ADDR         (DMD_BREQ1Q_012D0000_00_ADDR+DMD_BREQ1Q_012D0000_LEN)//B������2�޹�����
#define DMD_BREQ2Q_012E0000_00_LEN          DEMAND_JUST_LEN
#define DMD_BREQ2Q_012E0000_01_ADDR         (DMD_BREQ2Q_012E0000_00_ADDR+DMD_BREQ2Q_012E0000_00_LEN)//B������2�޹�����:ʱ��
#define DMD_BREQ2Q_012E0000_01_LEN          DEMAND_HM_LEN
#define DMD_BREQ2Q_012E0000_02_ADDR         (DMD_BREQ2Q_012E0000_01_ADDR+DMD_BREQ2Q_012E0000_01_LEN)//B������2�޹�����:������
#define DMD_BREQ2Q_012E0000_02_LEN          DEMAND_YMD_LEN
#define DMD_BREQ2Q_012E0000_LEN             DEMAND_LEN  //8

#define DMD_BREQ3Q_012F0000_00_ADDR         (DMD_BREQ2Q_012E0000_00_ADDR+DMD_BREQ2Q_012E0000_LEN)//B������3�޹�����
#define DMD_BREQ3Q_012F0000_00_LEN          DEMAND_JUST_LEN
#define DMD_BREQ3Q_012F0000_01_ADDR         (DMD_BREQ3Q_012F0000_00_ADDR+DMD_BREQ3Q_012F0000_00_LEN)//B������3�޹�����:ʱ��
#define DMD_BREQ3Q_012F0000_01_LEN          DEMAND_HM_LEN
#define DMD_BREQ3Q_012F0000_02_ADDR         (DMD_BREQ3Q_012F0000_01_ADDR+DMD_BREQ3Q_012F0000_01_LEN)//B������3�޹�����:������
#define DMD_BREQ3Q_012F0000_02_LEN          DEMAND_YMD_LEN
#define DMD_BREQ3Q_012F0000_LEN             DEMAND_LEN  //8

#define DMD_BREQ4Q_01300000_00_ADDR         (DMD_BREQ3Q_012F0000_00_ADDR+DMD_BREQ3Q_012F0000_LEN)//B������4�޹�����
#define DMD_BREQ4Q_01300000_00_LEN          DEMAND_JUST_LEN
#define DMD_BREQ4Q_01300000_01_ADDR         (DMD_BREQ4Q_01300000_00_ADDR+DMD_BREQ4Q_01300000_00_LEN)//B������4�޹�����:ʱ��
#define DMD_BREQ4Q_01300000_01_LEN          DEMAND_HM_LEN
#define DMD_BREQ4Q_01300000_02_ADDR         (DMD_BREQ4Q_01300000_01_ADDR+DMD_BREQ4Q_01300000_01_LEN)//B������4�޹�����:������
#define DMD_BREQ4Q_01300000_02_LEN          DEMAND_YMD_LEN
#define DMD_BREQ4Q_01300000_LEN             DEMAND_LEN  //8

#define DMD_BPOSS_01310000_00_ADDR          (DMD_BREQ4Q_01300000_00_ADDR+DMD_BREQ4Q_01300000_LEN)//B��������������
#define DMD_BPOSS_01310000_00_LEN           DEMAND_JUST_LEN
#define DMD_BPOSS_01310000_01_ADDR          (DMD_BPOSS_01310000_00_ADDR+DMD_BPOSS_01310000_00_LEN)//B��������������:ʱ��
#define DMD_BPOSS_01310000_01_LEN           DEMAND_HM_LEN
#define DMD_BPOSS_01310000_02_ADDR          (DMD_BPOSS_01310000_01_ADDR+DMD_BPOSS_01310000_01_LEN)//B��������������:������
#define DMD_BPOSS_01310000_02_LEN           DEMAND_YMD_LEN
#define DMD_BPOSS_01310000_LEN              DEMAND_LEN  //8

#define DMD_BNEGS_01320000_00_ADDR          (DMD_BPOSS_01310000_00_ADDR+DMD_BPOSS_01310000_LEN)//B�෴����������
#define DMD_BNEGS_01320000_00_LEN           DEMAND_JUST_LEN
#define DMD_BNEGS_01320000_01_ADDR          (DMD_BNEGS_01320000_00_ADDR+DMD_BNEGS_01320000_00_LEN)//B�෴����������:ʱ��
#define DMD_BNEGS_01320000_01_LEN           DEMAND_HM_LEN
#define DMD_BNEGS_01320000_02_ADDR          (DMD_BNEGS_01320000_01_ADDR+DMD_ANEGS_011E0000_01_LEN)//B�෴����������:������
#define DMD_BNEGS_01320000_02_LEN           DEMAND_YMD_LEN
#define DMD_BNEGS_01320000_LEN              DEMAND_LEN  //8
#define DMD_B_TOT_LEN                       SINGLE_PHASE_DEMAND_LEN        //B�������ܳ���
//C������
#define DMD_CPOSP_013D0000_00_ADDR          (DMD_BPOSP_01290000_00_ADDR+DMD_B_TOT_LEN)//C�������й�����
#else   //PHASE_3_LINE_4
//#define DMD_CPOSP_013D0000_00_ADDR          (DMD_APOSP_01150000_00_ADDR+DMD_A_TOT_LEN)//C�������й�����
#endif  //PHASE_3_LINE_4
#define DMD_CPOSP_013D0000_00_LEN           DEMAND_JUST_LEN
#define DMD_CPOSP_013D0000_01_ADDR          (DMD_CPOSP_013D0000_00_ADDR+DMD_CPOSP_013D0000_00_LEN)//C�������й�����:ʱ��
#define DMD_CPOSP_013D0000_01_LEN           DEMAND_HM_LEN
#define DMD_CPOSP_013D0000_02_ADDR          (DMD_CPOSP_013D0000_01_ADDR+DMD_CPOSP_013D0000_01_LEN)//C�������й�����:������
#define DMD_CPOSP_013D0000_02_LEN           DEMAND_YMD_LEN
#define DMD_CPOSP_013D0000_LEN              DEMAND_LEN  //8

#define DMD_CNEGP_013E0000_00_ADDR          (DMD_CPOSP_013D0000_00_ADDR+DMD_CPOSP_013D0000_LEN)//C�෴���й�����
#define DMD_CNEGP_013E0000_00_LEN           DEMAND_JUST_LEN
#define DMD_CNEGP_013E0000_01_ADDR          (DMD_CNEGP_013E0000_00_ADDR+DMD_CNEGP_013E0000_00_LEN)//C�෴���й�����:ʱ��
#define DMD_CNEGP_013E0000_01_LEN           DEMAND_HM_LEN
#define DMD_CNEGP_013E0000_02_ADDR          (DMD_CNEGP_013E0000_01_ADDR+DMD_CNEGP_013E0000_01_LEN)//C�෴���й�����:������
#define DMD_CNEGP_013E0000_02_LEN           DEMAND_YMD_LEN
#define DMD_CNEGP_013E0000_LEN              DEMAND_LEN  //8

#define DMD_CCOMQ1_013F0000_00_ADDR         (DMD_CNEGP_013E0000_00_ADDR+DMD_CNEGP_013E0000_LEN)//C�����1�޹�����
#define DMD_CCOMQ1_013F0000_00_LEN          DEMAND_JUST_LEN
#define DMD_CCOMQ1_013F0000_01_ADDR         (DMD_CCOMQ1_013F0000_00_ADDR+DMD_CCOMQ1_013F0000_00_LEN)//C�����1�޹�����:ʱ��
#define DMD_CCOMQ1_013F0000_01_LEN          DEMAND_HM_LEN
#define DMD_CCOMQ1_013F0000_02_ADDR         (DMD_CCOMQ1_013F0000_01_ADDR+DMD_CCOMQ1_013F0000_01_LEN)//C�����1�޹�����:������
#define DMD_CCOMQ1_013F0000_02_LEN          DEMAND_YMD_LEN
#define DMD_CCOMQ1_013F0000_LEN             DEMAND_LEN  //8

#define DMD_CCOMQ2_01400000_00_ADDR         (DMD_CCOMQ1_013F0000_00_ADDR+DMD_CCOMQ1_013F0000_LEN)//C�����2�޹�����
#define DMD_CCOMQ2_01400000_00_LEN          DEMAND_JUST_LEN
#define DMD_CCOMQ2_01400000_01_ADDR         (DMD_CCOMQ2_01400000_00_ADDR+DMD_CCOMQ2_01400000_00_LEN)//C�����2�޹�����:ʱ��
#define DMD_CCOMQ2_01400000_01_LEN          DEMAND_HM_LEN
#define DMD_CCOMQ2_01400000_02_ADDR         (DMD_CCOMQ2_01400000_01_ADDR+DMD_CCOMQ2_01400000_01_LEN)//C�����2�޹�����:������
#define DMD_CCOMQ2_01400000_02_LEN          DEMAND_YMD_LEN
#define DMD_CCOMQ2_01400000_LEN             DEMAND_LEN  //8

#define DMD_CREQ1Q_01410000_00_ADDR         (DMD_CCOMQ2_01400000_00_ADDR+DMD_CCOMQ2_01400000_LEN)//C������1�޹�����
#define DMD_CREQ1Q_01410000_00_LEN          DEMAND_JUST_LEN
#define DMD_CREQ1Q_01410000_01_ADDR         (DMD_CREQ1Q_01410000_00_ADDR+DMD_CREQ1Q_01410000_00_LEN)//C������1�޹�����:ʱ��
#define DMD_CREQ1Q_01410000_01_LEN          DEMAND_HM_LEN
#define DMD_CREQ1Q_01410000_02_ADDR         (DMD_CREQ1Q_01410000_01_ADDR+DMD_CREQ1Q_01410000_01_LEN)//C������1�޹�����:������
#define DMD_CREQ1Q_01410000_02_LEN          DEMAND_YMD_LEN
#define DMD_CREQ1Q_01410000_LEN             DEMAND_LEN  //8

#define DMD_CREQ2Q_01420000_00_ADDR         (DMD_CREQ1Q_01410000_00_ADDR+DMD_CREQ1Q_01410000_LEN)//C������2�޹�����
#define DMD_CREQ2Q_01420000_00_LEN          DEMAND_JUST_LEN
#define DMD_CREQ2Q_01420000_01_ADDR         (DMD_CREQ2Q_01420000_00_ADDR+DMD_CREQ2Q_01420000_00_LEN)//C������2�޹�����:ʱ��
#define DMD_CREQ2Q_01420000_01_LEN          DEMAND_HM_LEN
#define DMD_CREQ2Q_01420000_02_ADDR         (DMD_CREQ2Q_01420000_01_ADDR+DMD_CREQ2Q_01420000_01_LEN)//C������2�޹�����:������
#define DMD_CREQ2Q_01420000_02_LEN          DEMAND_YMD_LEN
#define DMD_CREQ2Q_01420000_LEN             DEMAND_LEN  //8

#define DMD_CREQ3Q_01430000_00_ADDR         (DMD_CREQ2Q_01420000_00_ADDR+DMD_CREQ2Q_01420000_LEN)//C������3�޹�����
#define DMD_CREQ3Q_01430000_00_LEN          DEMAND_JUST_LEN
#define DMD_CREQ3Q_01430000_01_ADDR         (DMD_CREQ3Q_01430000_00_ADDR+DMD_CREQ3Q_01430000_00_LEN)//C������3�޹�����:ʱ��
#define DMD_CREQ3Q_01430000_01_LEN          DEMAND_HM_LEN
#define DMD_CREQ3Q_01430000_02_ADDR         (DMD_CREQ3Q_01430000_01_ADDR+DMD_CREQ3Q_01430000_01_LEN)//C������3�޹�����:������
#define DMD_CREQ3Q_01430000_02_LEN          DEMAND_YMD_LEN
#define DMD_CREQ3Q_01430000_LEN             DEMAND_LEN  //8

#define DMD_CREQ4Q_01440000_00_ADDR         (DMD_CREQ3Q_01430000_00_ADDR+DMD_CREQ3Q_01430000_LEN)//C������4�޹�����
#define DMD_CREQ4Q_01440000_00_LEN          DEMAND_JUST_LEN
#define DMD_CREQ4Q_01440000_01_ADDR         (DMD_CREQ4Q_01440000_00_ADDR+DMD_CREQ4Q_01440000_00_LEN)//C������4�޹�����:ʱ��
#define DMD_CREQ4Q_01440000_01_LEN          DEMAND_HM_LEN
#define DMD_CREQ4Q_01440000_02_ADDR         (DMD_CREQ4Q_01440000_01_ADDR+DMD_CREQ4Q_01440000_01_LEN)//C������4�޹�����:������
#define DMD_CREQ4Q_01440000_02_LEN          DEMAND_YMD_LEN
#define DMD_CREQ4Q_01440000_LEN             DEMAND_LEN  //8

#define DMD_CPOSS_01450000_00_ADDR          (DMD_CREQ4Q_01440000_00_ADDR+DMD_CREQ4Q_01440000_LEN)//C��������������
#define DMD_CPOSS_01450000_00_LEN           DEMAND_JUST_LEN
#define DMD_CPOSS_01450000_01_ADDR          (DMD_CPOSS_01450000_00_ADDR+DMD_CPOSS_01450000_00_LEN)//C��������������:ʱ��
#define DMD_CPOSS_01450000_01_LEN           DEMAND_HM_LEN
#define DMD_CPOSS_01450000_02_ADDR          (DMD_CPOSS_01450000_01_ADDR+DMD_CPOSS_01450000_01_LEN)//C��������������:������
#define DMD_CPOSS_01450000_02_LEN           DEMAND_YMD_LEN
#define DMD_CPOSS_01450000_LEN              DEMAND_LEN  //8

#define DMD_CNEGS_01460000_00_ADDR          (DMD_CPOSS_01450000_00_ADDR+DMD_CPOSS_01450000_LEN)//C�෴����������
#define DMD_CNEGS_01460000_00_LEN           DEMAND_JUST_LEN
#define DMD_CNEGS_01460000_01_ADDR          (DMD_CNEGS_01460000_00_ADDR+DMD_CNEGS_01460000_00_LEN)//C�෴����������:ʱ��
#define DMD_CNEGS_01460000_01_LEN           DEMAND_HM_LEN
#define DMD_CNEGS_01460000_02_ADDR          (DMD_CNEGS_01460000_01_ADDR+DMD_CNEGS_01460000_01_LEN)//BC�෴����������:������
#define DMD_CNEGS_01460000_02_LEN           DEMAND_YMD_LEN
#define DMD_CNEGS_01460000_LEN              DEMAND_LEN  //8
#define DMD_C_TOT_LEN                       SINGLE_PHASE_DEMAND_LEN        //C�������ܳ���

#ifdef PHASE_3_LINE_4
#define DMD_RAM_ARRAY_MAX                   (TOTAL_PHASE_DEMAND_LEN * 10 + DEMAND_LEN * 10 * 3)  //��ǰ����RAM��������[������ʱ�֡������վ�ռ1����]
#else   //PHASE_3_LINE_4
//#define DMD_RAM_ARRAY_MAX                   (TOTAL_PHASE_DEMAND_LEN * 10 + DEMAND_LEN * 10 * 2)  //��ǰ����RAM��������[������ʱ�֡������վ�ռ1����]
#endif  //PHASE_3_LINE_4

//��1~12��������Ҫ���� ��ʼ[��δӦ�õ�DLT645]
#define DMD_LASTXMONTH_01010001_00_OFFSET_MAXNUM    12  //
#define DMD_LASTXMONTH_01010001_00_OFFSET_ADDR      (DMD_CPOSP_013D0000_00_ADDR+DMD_C_TOT_LEN)//����ָ�뱣��ĵ�ַ
#define DMD_LASTXMONTH_01010001_00_OFFSET_LEN       3 + CRC_LEN   //����ָ�볤��
#ifdef PHASE_3_LINE_4
#define DMD_LASTXMONTH_01010001_00_OFFSET_BLOCKLEN  (TOTAL_PHASE_DEMAND_LEN*10+SINGLE_PHASE_DEMAND_LEN*3)//�������������������ݳ���
#else   //PHASE_3_LINE_4
//#define DMD_LASTXMONTH_01010001_00_OFFSET_BLOCKLEN  (TOTAL_PHASE_DEMAND_LEN*10+SINGLE_PHASE_DEMAND_LEN*2)//�������������������ݳ���
#endif  //PHASE_3_LINE_4
#define DMD_LASTXMONTH_01010001_00_HEAD_ADDR        (DMD_LASTXMONTH_01010001_00_OFFSET_ADDR+DMD_LASTXMONTH_01010001_00_OFFSET_LEN)
#define DMD_LASTXMONTH_01010001_00_END_ADDR         (DMD_LASTXMONTH_01010001_00_HEAD_ADDR+DMD_LASTXMONTH_01010001_00_OFFSET_BLOCKLEN*DMD_LASTXMONTH_01010001_00_OFFSET_MAXNUM)
//��1~12��������Ҫ���� ����


//-----------------------------------------------------------------------------
// �α������壺
//
//-----------------------------------------------------------------------------
#define PAR_DATE_04000101_00_ADDR           DMD_LASTXMONTH_01010001_00_END_ADDR //�ܿ��¼��ݾӵĿռ�
//#define PAR_DATE_04000101_00_ADDR           (EVET_OPNTMNBX_END_ADDR)                                        //���ڼ�����(����0����������)[��δӦ�õ�DLT645]
#define PAR_DATE_04000101_00_LEN            4
#define PAR_TIME_04000102_00_ADDR           (PAR_DATE_04000101_00_ADDR + PAR_DATE_04000101_00_LEN)          //ʱ��[��δӦ�õ�DLT645]
#define PAR_TIME_04000102_00_LEN            3
#define PAR_DMDMAXP_04000103_00_ADDR        (PAR_TIME_04000102_00_ADDR + PAR_TIME_04000102_00_LEN)          //�����������
#define PAR_DMDMAXP_04000103_00_LEN         1
#define PAR_SLIDTIME_04000104_00_ADDR       (PAR_DMDMAXP_04000103_00_ADDR + PAR_DMDMAXP_04000103_00_LEN)    //����ʱ��
#define PAR_SLIDTIME_04000104_00_LEN        1
#define PAR_PLSWIDTH_04000105_00_ADDR       (PAR_SLIDTIME_04000104_00_ADDR + PAR_SLIDTIME_04000104_00_LEN)  //У��������
#define PAR_PLSWIDTH_04000105_00_LEN        2
#define PAR_TZSWTIME_04000106_00_ADDR       (PAR_PLSWIDTH_04000105_00_ADDR + PAR_PLSWIDTH_04000105_00_LEN)  //����ʱ�����л�ʱ��:ʱ��
#define PAR_TZSWTIME_04000106_00_LEN        2
#define PAR_TZSWTIME_04000106_01_ADDR       (PAR_TZSWTIME_04000106_00_ADDR + PAR_TZSWTIME_04000106_00_LEN)  //����ʱ�����л�ʱ��:������
#define PAR_TZSWTIME_04000106_01_LEN        3
#define PAR_TSSWTIME_04000107_00_ADDR       (PAR_TZSWTIME_04000106_01_ADDR + PAR_TZSWTIME_04000106_01_LEN)  //������ʱ�α��л�ʱ��:ʱ��
#define PAR_TSSWTIME_04000107_00_LEN        2
#define PAR_TSSWTIME_04000107_01_ADDR       (PAR_TSSWTIME_04000107_00_ADDR + PAR_TSSWTIME_04000107_00_LEN)  //������ʱ�α��л�ʱ��:������
#define PAR_TSSWTIME_04000107_01_LEN        3

#define PAR_TZAMOUNT_04000201_00_ADDR       (PAR_TSSWTIME_04000107_01_ADDR + PAR_TSSWTIME_04000107_01_LEN)  //��ʱ����p(��14)
#define PAR_TZAMOUNT_04000201_00_LEN        1
#define PAR_TSAMOUNT_04000202_00_ADDR       (PAR_TZAMOUNT_04000201_00_ADDR + PAR_TZAMOUNT_04000201_00_LEN)  //��ʱ�α���q(��8)
#define PAR_TSAMOUNT_04000202_00_LEN        1
#define PAR_TSSWCNT_04000203_00_ADDR        (PAR_TSAMOUNT_04000202_00_ADDR + PAR_TSAMOUNT_04000202_00_LEN)  //��ʱ����(ÿ���л���) m(��14)
#define PAR_TSSWCNT_04000203_00_LEN         1
#define PAR_FEERATE_04000204_00_ADDR        (PAR_TSSWCNT_04000203_00_ADDR + PAR_TSSWCNT_04000203_00_LEN)    //������k(��63)
#define PAR_FEERATE_04000204_00_LEN         1
#define PAR_HOLIDYS_04000205_00_ADDR        (PAR_FEERATE_04000204_00_ADDR + PAR_FEERATE_04000204_00_LEN)    //����������n(��254)
#define PAR_HOLIDYS_04000205_00_LEN         2
#define PAR_HMANLS_04000206_00_ADDR         (PAR_HOLIDYS_04000205_00_ADDR + PAR_HOLIDYS_04000205_00_LEN)    //г����������[��δӦ�õ�DLT645]
#define PAR_HMANLS_04000206_00_LEN          1
//{[��δӦ�õ�DLT645]
#define PAR_LPDISCNT_04000301_00_ADDR       (PAR_HMANLS_04000206_00_ADDR + PAR_HMANLS_04000206_00_LEN)      //�Զ�ѭ����ʾ����
#define PAR_LPDISCNT_04000301_00_LEN        1
#define PAR_DISTIME_04000302_00_ADDR        (PAR_LPDISCNT_04000301_00_ADDR + PAR_LPDISCNT_04000301_00_LEN)  //ÿ����ʾʱ��
#define PAR_DISTIME_04000302_00_LEN         1
#define PAR_ENGDECIM_04000303_00_ADDR       (PAR_DISTIME_04000302_00_ADDR + PAR_DISTIME_04000302_00_LEN)    //��ʾ����С��λ��
#define PAR_ENGDECIM_04000303_00_LEN        1
#define PAR_DMDDECIM_04000304_00_ADDR       (PAR_ENGDECIM_04000303_00_ADDR + PAR_ENGDECIM_04000303_00_LEN)  //��ʾ����(�������)С��λ��
#define PAR_DMDDECIM_04000304_00_LEN        1
#define PAR_KEYDIS_04000305_00_ADDR         (PAR_DMDDECIM_04000304_00_ADDR + PAR_DMDDECIM_04000304_00_LEN)  //����ѭ����ʾ����
#define PAR_KEYDIS_04000305_00_LEN          1
//}[��δӦ�õ�DLT645]
#define PAR_HVMADDR_04000401_00_ADDR        (PAR_KEYDIS_04000305_00_ADDR + PAR_KEYDIS_04000305_00_LEN)      //ͨ�ŵ�ַ:A0~A3
#define PAR_HVMADDR_04000401_00_LEN         4
#define PAR_HVMADDR_04000401_01_ADDR        (PAR_HVMADDR_04000401_00_ADDR + PAR_HVMADDR_04000401_00_LEN)    //ͨ�ŵ�ַ:A4~A5
#define PAR_HVMADDR_04000401_01_LEN         2
#define PAR_HVMADDR_04000401_TOT_LEN        (PAR_HVMADDR_04000401_00_LEN + PAR_HVMADDR_04000401_01_LEN)
#define PAR_HVMNUM_04000402_00_ADDR         (PAR_HVMADDR_04000401_00_ADDR + PAR_HVMADDR_04000401_TOT_LEN)   //���:N0~N3
#define PAR_HVMNUM_04000402_00_LEN          4
#define PAR_HVMNUM_04000402_01_ADDR         (PAR_HVMNUM_04000402_00_ADDR + PAR_HVMNUM_04000402_00_LEN)      //���:N4~N5
#define PAR_HVMNUM_04000402_01_LEN          2
#define PAR_HVMNUM_04000402_TOT_LEN         PAR_HVMNUM_04000402_00_LEN + PAR_HVMNUM_04000402_01_LEN
#define PAR_PROPTNUM_04000403_00_ADDR       (PAR_HVMNUM_04000402_00_ADDR + PAR_HVMNUM_04000402_TOT_LEN)     //�ʲ��������(ASCII��):N0~N3
#define PAR_PROPTNUM_04000403_00_LEN        4
#define PAR_PROPTNUM_04000403_01_ADDR       (PAR_PROPTNUM_04000403_00_ADDR + PAR_PROPTNUM_04000403_00_LEN)  //�ʲ��������(ASCII��):N4~N7
#define PAR_PROPTNUM_04000403_01_LEN        4
#define PAR_PROPTNUM_04000403_02_ADDR       (PAR_PROPTNUM_04000403_01_ADDR + PAR_PROPTNUM_04000403_01_LEN)  //�ʲ��������(ASCII��):N8~N11
#define PAR_PROPTNUM_04000403_02_LEN        4
#define PAR_PROPTNUM_04000403_03_ADDR       (PAR_PROPTNUM_04000403_02_ADDR + PAR_PROPTNUM_04000403_02_LEN)  //�ʲ��������(ASCII��):N12~N15
#define PAR_PROPTNUM_04000403_03_LEN        4
#define PAR_PROPTNUM_04000403_04_ADDR       (PAR_PROPTNUM_04000403_03_ADDR + PAR_PROPTNUM_04000403_03_LEN)  //�ʲ��������(ASCII��):N16~N19
#define PAR_PROPTNUM_04000403_04_LEN        4
#define PAR_PROPTNUM_04000403_05_ADDR       (PAR_PROPTNUM_04000403_04_ADDR + PAR_PROPTNUM_04000403_04_LEN)  //�ʲ��������(ASCII��):N20~N23
#define PAR_PROPTNUM_04000403_05_LEN        4
#define PAR_PROPTNUM_04000403_06_ADDR       (PAR_PROPTNUM_04000403_05_ADDR + PAR_PROPTNUM_04000403_05_LEN)  //�ʲ��������(ASCII��):N24~N27
#define PAR_PROPTNUM_04000403_06_LEN        4
#define PAR_PROPTNUM_04000403_07_ADDR       (PAR_PROPTNUM_04000403_06_ADDR + PAR_PROPTNUM_04000403_06_LEN)  //�ʲ��������(ASCII��):N28~N31
#define PAR_PROPTNUM_04000403_07_LEN        4
#define PAR_PROPTNUM_04000403_TOT_LEN       32
#define PAR_VRATING_04000404_00_ADDR        (PAR_PROPTNUM_04000403_00_ADDR + PAR_PROPTNUM_04000403_TOT_LEN) //���ѹ(ASCII��)ǰ4�ֽ�
#define PAR_VRATING_04000404_00_LEN         4
#define PAR_VRATING_04000404_01_ADDR        (PAR_VRATING_04000404_00_ADDR + PAR_VRATING_04000404_00_LEN)    //���ѹ(ASCII��)��2�ֽ�
#define PAR_VRATING_04000404_01_LEN         2
#define PAR_VRATING_04000404_TOT_LEN        (PAR_VRATING_04000404_00_LEN + PAR_VRATING_04000404_01_LEN)
#define PAR_IRATING_04000405_00_ADDR        (PAR_VRATING_04000404_00_ADDR + PAR_VRATING_04000404_TOT_LEN)   //�����/��������(ASCII��)ǰ4�ֽ�
#define PAR_IRATING_04000405_00_LEN         4
#define PAR_IRATING_04000405_01_ADDR        (PAR_IRATING_04000405_00_ADDR + PAR_IRATING_04000405_00_LEN)    //�����/��������(ASCII��)��2�ֽ�
#define PAR_IRATING_04000405_01_LEN         2
#define PAR_IRATING_04000405_TOT_LEN        (PAR_IRATING_04000405_00_LEN + PAR_IRATING_04000405_01_LEN)
#define PAR_IMAX_04000406_00_ADDR           (PAR_IRATING_04000405_00_ADDR + PAR_IRATING_04000405_TOT_LEN)   //������(ASCII��)ǰ4�ֽ�
#define PAR_IMAX_04000406_00_LEN            4
#define PAR_IMAX_04000406_01_ADDR           (PAR_IMAX_04000406_00_ADDR + PAR_IMAX_04000406_00_LEN)          //������(ASCII��)��2�ֽ�
#define PAR_IMAX_04000406_01_LEN            2
#define PAR_IMAX_04000406_TOT_LEN           (PAR_IMAX_04000406_00_LEN + PAR_IMAX_04000406_01_LEN)
#define PAR_PACCURACY_04000407_00_ADDR      (PAR_IMAX_04000406_00_ADDR + PAR_IMAX_04000406_TOT_LEN)         //�й�׼ȷ�ȵȼ�(ASCII��)
#define PAR_PACCURACY_04000407_00_LEN       4
#define PAR_QACCURACY_04000408_00_ADDR      (PAR_PACCURACY_04000407_00_ADDR + PAR_PACCURACY_04000407_00_LEN)//�޹�׼ȷ�ȵȼ�(ASCII��)
#define PAR_QACCURACY_04000408_00_LEN       4
#define PAR_PCONSTANT_04000409_00_ADDR      (PAR_QACCURACY_04000408_00_ADDR + PAR_QACCURACY_04000408_00_LEN)//����й�����
#define PAR_PCONSTANT_04000409_00_LEN       3
#define PAR_QCONSTANT_0400040A_00_ADDR      (PAR_PCONSTANT_04000409_00_ADDR + PAR_PCONSTANT_04000409_00_LEN)//�����й�����
#define PAR_QCONSTANT_0400040A_00_LEN       3
#define PAR_HVMTYPE_0400040B_00_ADDR        (PAR_QCONSTANT_0400040A_00_ADDR + PAR_QCONSTANT_0400040A_00_LEN)//����ͺ�(ASCII��):ǰ4�ֽ�
#define PAR_HVMTYPE_0400040B_00_LEN         4
#define PAR_HVMTYPE_0400040B_01_ADDR        (PAR_HVMTYPE_0400040B_00_ADDR + PAR_HVMTYPE_0400040B_00_LEN)    //����ͺ�(ASCII��):�м�4�ֽ�
#define PAR_HVMTYPE_0400040B_01_LEN         4
#define PAR_HVMTYPE_0400040B_02_ADDR        (PAR_HVMTYPE_0400040B_01_ADDR + PAR_HVMTYPE_0400040B_01_LEN)    //����ͺ�(ASCII��):��2�ֽ�
#define PAR_HVMTYPE_0400040B_02_LEN         2
#define PAR_HVMTYPE_0400040B_TOT_LEN        (PAR_HVMTYPE_0400040B_00_LEN + PAR_HVMTYPE_0400040B_01_LEN + PAR_HVMTYPE_0400040B_02_LEN)
#define PAR_HVMBIRTH_0400040C_00_ADDR       (PAR_HVMTYPE_0400040B_00_ADDR + PAR_HVMTYPE_0400040B_TOT_LEN)   //��������(ASCII��):ǰ4�ֽ�
#define PAR_HVMBIRTH_0400040C_00_LEN        4
#define PAR_HVMBIRTH_0400040C_01_ADDR       (PAR_HVMBIRTH_0400040C_00_ADDR + PAR_HVMBIRTH_0400040C_00_LEN)  //��������(ASCII��):�м�4�ֽ�
#define PAR_HVMBIRTH_0400040C_01_LEN        4
#define PAR_HVMBIRTH_0400040C_02_ADDR       (PAR_HVMBIRTH_0400040C_01_ADDR + PAR_HVMBIRTH_0400040C_01_LEN)  //��������(ASCII��):��2�ֽ�
#define PAR_HVMBIRTH_0400040C_02_LEN        2
#define PAR_HVMBIRTH_0400040C_TOT_LEN       (PAR_HVMBIRTH_0400040C_00_LEN + PAR_HVMBIRTH_0400040C_01_LEN + PAR_HVMBIRTH_0400040C_02_LEN)
#define PAR_PROTOVER_0400040D_00_ADDR       (PAR_HVMBIRTH_0400040C_00_ADDR + PAR_HVMBIRTH_0400040C_TOT_LEN) //Э��汾��(ASCII��):N0~N3
#define PAR_PROTOVER_0400040D_00_LEN        4
#define PAR_PROTOVER_0400040D_01_ADDR       (PAR_PROTOVER_0400040D_00_ADDR + PAR_PROTOVER_0400040D_00_LEN)  //Э��汾��(ASCII��):N4~N7
#define PAR_PROTOVER_0400040D_01_LEN        4
#define PAR_PROTOVER_0400040D_02_ADDR       (PAR_PROTOVER_0400040D_01_ADDR + PAR_PROTOVER_0400040D_01_LEN)  //Э��汾��(ASCII��):N8~N11
#define PAR_PROTOVER_0400040D_02_LEN        4
#define PAR_PROTOVER_0400040D_03_ADDR       (PAR_PROTOVER_0400040D_02_ADDR + PAR_PROTOVER_0400040D_02_LEN)  //Э��汾��(ASCII��):N12~N15
#define PAR_PROTOVER_0400040D_03_LEN        4
#define PAR_PROTOVER_0400040D_TOT_LEN       16
//{[��δӦ�õ�DLT645]
#define PAR_RUNSTAT1_04000501_00_ADDR       (PAR_PROTOVER_0400040D_00_ADDR + PAR_PROTOVER_0400040D_TOT_LEN) //�������״̬��1
#define PAR_RUNSTAT1_04000501_00_LEN        2
#define PAR_RUNSTAT2_04000502_00_ADDR       (PAR_RUNSTAT1_04000501_00_ADDR + PAR_RUNSTAT1_04000501_00_LEN)  //�������״̬��2
#define PAR_RUNSTAT2_04000502_00_LEN        2
#define PAR_RUNSTAT3_04000503_00_ADDR       (PAR_RUNSTAT2_04000502_00_ADDR + PAR_RUNSTAT2_04000502_00_LEN)  //�������״̬��3
#define PAR_RUNSTAT3_04000503_00_LEN        2
#define PAR_RUNSTAT4_04000504_00_ADDR       (PAR_RUNSTAT3_04000503_00_ADDR + PAR_RUNSTAT3_04000503_00_LEN)  //�������״̬��4
#define PAR_RUNSTAT4_04000504_00_LEN        2
#define PAR_RUNSTAT5_04000505_00_ADDR       (PAR_RUNSTAT4_04000504_00_ADDR + PAR_RUNSTAT4_04000504_00_LEN)  //�������״̬��5
#define PAR_RUNSTAT5_04000505_00_LEN        2
#define PAR_RUNSTAT6_04000506_00_ADDR       (PAR_RUNSTAT5_04000505_00_ADDR + PAR_RUNSTAT5_04000505_00_LEN)  //�������״̬��6
#define PAR_RUNSTAT6_04000506_00_LEN        2
#define PAR_RUNSTAT7_04000507_00_ADDR       (PAR_RUNSTAT6_04000506_00_ADDR + PAR_RUNSTAT6_04000506_00_LEN)  //�������״̬��7
#define PAR_RUNSTAT7_04000507_00_LEN        2
//}[��δӦ�õ�DLT645]
#define PAR_PCOMB_04000601_00_ADDR          (PAR_RUNSTAT7_04000507_00_ADDR + PAR_RUNSTAT7_04000507_00_LEN)  //�й���Ϸ�ʽ������
#define PAR_PCOMB_04000601_00_LEN           1
#define PAR_QCOMB1_04000602_00_ADDR         (PAR_PCOMB_04000601_00_ADDR + PAR_PCOMB_04000601_00_LEN)        //�޹���Ϸ�ʽ1������
#define PAR_QCOMB1_04000602_00_LEN          1
#define PAR_QCOMB2_04000603_00_ADDR         (PAR_QCOMB1_04000602_00_ADDR + PAR_QCOMB1_04000602_00_LEN)      //�޹���Ϸ�ʽ2������
#define PAR_QCOMB2_04000603_00_LEN          1

#define PAR_MDIRDABD_04000701_00_ADDR       (PAR_QCOMB2_04000603_00_ADDR + PAR_QCOMB2_04000603_00_LEN)      //�����ͺ�����ͨ������������[��δӦ�õ�DLT645]
#define PAR_MDIRDABD_04000701_00_LEN        1
#define PAR_CTIRDABD_04000702_00_ADDR       (PAR_MDIRDABD_04000701_00_ADDR + PAR_MDIRDABD_04000701_00_LEN)  //�Ӵ��ͺ�����ͨ������������[��δӦ�õ�DLT645]
#define PAR_CTIRDABD_04000702_00_LEN        1
#define PAR_PORT1BD_04000703_00_ADDR        (PAR_CTIRDABD_04000702_00_ADDR + PAR_CTIRDABD_04000702_00_LEN)  //ͨ�ſ�1ͨ������������[��δӦ�õ�DLT645]
#define PAR_PORT1BD_04000703_00_LEN         1
#define PAR_PORT2BD_04000704_00_ADDR        (PAR_PORT1BD_04000703_00_ADDR + PAR_PORT1BD_04000703_00_LEN)    //ͨ�ſ�2ͨ������������[��δӦ�õ�DLT645]
#define PAR_PORT2BD_04000704_00_LEN         1
#define PAR_PORT3BD_04000705_00_ADDR        (PAR_PORT2BD_04000704_00_ADDR + PAR_PORT2BD_04000704_00_LEN)    //ͨ�ſ�3ͨ������������[��δӦ�õ�DLT645]
#define PAR_PORT3BD_04000705_00_LEN         1

#define PAR_RESTDAY_04000801_00_ADDR        (PAR_PORT3BD_04000705_00_ADDR + PAR_PORT3BD_04000705_00_LEN)    //������������[��δӦ�õ�DLT645]
#define PAR_RESTDAY_04000801_00_LEN         1
#define PAR_RESTDAYTS_04000802_00_ADDR      (PAR_RESTDAY_04000801_00_ADDR + PAR_RESTDAY_04000801_00_LEN)    //�����ղ��õ���ʱ�α��[��δӦ�õ�DLT645]
#define PAR_RESTDAYTS_04000802_00_LEN       1

#define PAR_LOADRCDMD_04000901_00_ADDR      (PAR_RESTDAYTS_04000802_00_ADDR + PAR_RESTDAYTS_04000802_00_LEN)//���ɼ�¼ģʽ��
#define PAR_LOADRCDMD_04000901_00_LEN       1
#define PAR_FREEZEMD_04000902_00_ADDR       (PAR_LOADRCDMD_04000901_00_ADDR + PAR_LOADRCDMD_04000901_00_LEN)//��������ģʽ��
#define PAR_FREEZEMD_04000902_00_LEN        1

#define PAR_LOADRCDST_04000A01_00_ADDR      (PAR_FREEZEMD_04000902_00_ADDR + PAR_FREEZEMD_04000902_00_LEN)  //���ɼ�¼��ʼʱ��
#define PAR_LOADRCDST_04000A01_00_LEN       4
#define PAR_LOAD1INTV_04000A02_00_ADDR      (PAR_LOADRCDST_04000A01_00_ADDR + PAR_LOADRCDST_04000A01_00_LEN)//��1�ฺ�ɼ�¼���ʱ��
#define PAR_LOAD1INTV_04000A02_00_LEN       2
#define PAR_LOAD2INTV_04000A03_00_ADDR      (PAR_LOAD1INTV_04000A02_00_ADDR + PAR_LOAD1INTV_04000A02_00_LEN)//��2�ฺ�ɼ�¼���ʱ��
#define PAR_LOAD2INTV_04000A03_00_LEN       2
#define PAR_LOAD3INTV_04000A04_00_ADDR      (PAR_LOAD2INTV_04000A03_00_ADDR + PAR_LOAD2INTV_04000A03_00_LEN)//��3�ฺ�ɼ�¼���ʱ��
#define PAR_LOAD3INTV_04000A04_00_LEN       2
#define PAR_LOAD4INTV_04000A05_00_ADDR      (PAR_LOAD3INTV_04000A04_00_ADDR + PAR_LOAD3INTV_04000A04_00_LEN)//��4�ฺ�ɼ�¼���ʱ��
#define PAR_LOAD4INTV_04000A05_00_LEN       2
#define PAR_LOAD5INTV_04000A06_00_ADDR      (PAR_LOAD4INTV_04000A05_00_ADDR + PAR_LOAD4INTV_04000A05_00_LEN)//��5�ฺ�ɼ�¼���ʱ��
#define PAR_LOAD5INTV_04000A06_00_LEN       2
#define PAR_LOAD6INTV_04000A07_00_ADDR      (PAR_LOAD5INTV_04000A06_00_ADDR + PAR_LOAD5INTV_04000A06_00_LEN)//��6�ฺ�ɼ�¼���ʱ��
#define PAR_LOAD6INTV_04000A07_00_LEN       2

#define PAR_FIRACCNT_04000B01_00_ADDR       (PAR_LOAD6INTV_04000A07_00_ADDR + PAR_LOAD6INTV_04000A07_00_LEN)//ÿ�µ�1������
#define PAR_FIRACCNT_04000B01_00_LEN        2
#define PAR_SECACCNT_04000B02_00_ADDR       (PAR_LOAD5INTV_04000A06_00_ADDR + PAR_LOAD5INTV_04000A06_00_LEN)//ÿ�µ�2������
#define PAR_SECACCNT_04000B02_00_LEN        2
#define PAR_THIACCNT_04000B03_00_ADDR       (PAR_SECACCNT_04000B02_00_ADDR + PAR_SECACCNT_04000B02_00_LEN)  //ÿ�µ�3������
#define PAR_THIACCNT_04000B03_00_LEN        2

#define PAR_PRI0PSWD_04000C01_00_ADDR       (PAR_THIACCNT_04000B03_00_ADDR + PAR_THIACCNT_04000B03_00_LEN)  //0������
#define PAR_PRI0PSWD_04000C01_00_LEN        4
#define PAR_PRI1PSWD_04000C02_00_ADDR       (PAR_PRI0PSWD_04000C01_00_ADDR + PAR_PRI0PSWD_04000C01_00_LEN)  //1������
#define PAR_PRI1PSWD_04000C02_00_LEN        4
#define PAR_PRI2PSWD_04000C03_00_ADDR       (PAR_PRI1PSWD_04000C02_00_ADDR + PAR_PRI1PSWD_04000C02_00_LEN)  //2������
#define PAR_PRI2PSWD_04000C03_00_LEN        4
#define PAR_PRI3PSWD_04000C04_00_ADDR       (PAR_PRI2PSWD_04000C03_00_ADDR + PAR_PRI2PSWD_04000C03_00_LEN)  //3������
#define PAR_PRI3PSWD_04000C04_00_LEN        4
#define PAR_PRI4PSWD_04000C05_00_ADDR       (PAR_PRI3PSWD_04000C04_00_ADDR + PAR_PRI3PSWD_04000C04_00_LEN)  //4������
#define PAR_PRI4PSWD_04000C05_00_LEN        4
#define PAR_PRI5PSWD_04000C06_00_ADDR       (PAR_PRI4PSWD_04000C05_00_ADDR + PAR_PRI4PSWD_04000C05_00_LEN)  //5������
#define PAR_PRI5PSWD_04000C06_00_LEN        4
#define PAR_PRI6PSWD_04000C07_00_ADDR       (PAR_PRI5PSWD_04000C06_00_ADDR + PAR_PRI5PSWD_04000C06_00_LEN)  //6������
#define PAR_PRI6PSWD_04000C07_00_LEN        4
#define PAR_PRI7PSWD_04000C08_00_ADDR       (PAR_PRI6PSWD_04000C07_00_ADDR + PAR_PRI6PSWD_04000C07_00_LEN)  //7������
#define PAR_PRI7PSWD_04000C08_00_LEN        4
#define PAR_PRI8PSWD_04000C09_00_ADDR       (PAR_PRI7PSWD_04000C08_00_ADDR + PAR_PRI7PSWD_04000C08_00_LEN)  //8������
#define PAR_PRI8PSWD_04000C09_00_LEN        4
#define PAR_PRI9PSWD_04000C0A_00_ADDR       (PAR_PRI8PSWD_04000C09_00_ADDR + PAR_PRI8PSWD_04000C09_00_LEN)  //9������
#define PAR_PRI9PSWD_04000C0A_00_LEN        4
//{[��δӦ�õ�DLT645]
#define PAR_ACONDUCT_04000D01_00_ADDR       (PAR_PRI9PSWD_04000C0A_00_ADDR + PAR_PRI9PSWD_04000C0A_00_LEN)  //A��絼ϵ��
#define PAR_ACONDUCT_04000D01_00_LEN        2
#define PAR_ASUSCEPT_04000D02_00_ADDR       (PAR_ACONDUCT_04000D01_00_ADDR + PAR_ACONDUCT_04000D01_00_LEN)  //A�����ϵ��
#define PAR_ASUSCEPT_04000D02_00_LEN        2
#define PAR_ARESISTC_04000D03_00_ADDR       (PAR_ASUSCEPT_04000D02_00_ADDR + PAR_ASUSCEPT_04000D02_00_LEN)  //A�����ϵ��
#define PAR_ARESISTC_04000D03_00_LEN        2
#define PAR_AREACTC_04000D04_00_ADDR        (PAR_ARESISTC_04000D03_00_ADDR + PAR_ARESISTC_04000D03_00_LEN)  //A��翹ϵ��
#define PAR_AREACTC_04000D04_00_LEN         2
#define PAR_BCONDUCT_04000D05_00_ADDR       (PAR_AREACTC_04000D04_00_ADDR + PAR_AREACTC_04000D04_00_LEN)    //B��絼ϵ��
#define PAR_BCONDUCT_04000D05_00_LEN        2
#define PAR_BSUSCEPT_04000D06_00_ADDR       (PAR_BCONDUCT_04000D05_00_ADDR + PAR_BCONDUCT_04000D05_00_LEN)  //B�����ϵ��
#define PAR_BSUSCEPT_04000D06_00_LEN        2
#define PAR_BRESISTC_04000D07_00_ADDR       (PAR_BSUSCEPT_04000D06_00_ADDR + PAR_BSUSCEPT_04000D06_00_LEN)  //B�����ϵ��
#define PAR_BRESISTC_04000D07_00_LEN        2
#define PAR_BREACTC_04000D08_00_ADDR        (PAR_BRESISTC_04000D07_00_ADDR + PAR_BRESISTC_04000D07_00_LEN)  //B��翹ϵ��
#define PAR_BREACTC_04000D04_00_LEN         2
#define PAR_CCONDUCT_04000D09_00_ADDR       (PAR_BREACTC_04000D08_00_ADDR + PAR_BREACTC_04000D04_00_LEN)    //C��絼ϵ��
#define PAR_CCONDUCT_04000D09_00_LEN        2
#define PAR_CSUSCEPT_04000D0A_00_ADDR       (PAR_CCONDUCT_04000D09_00_ADDR + PAR_CCONDUCT_04000D09_00_LEN)  //C�����ϵ��
#define PAR_CSUSCEPT_04000D0A_00_LEN        2
#define PAR_CRESISTC_04000D0B_00_ADDR       (PAR_CSUSCEPT_04000D0A_00_ADDR + PAR_CSUSCEPT_04000D0A_00_LEN)  //C�����ϵ��
#define PAR_CRESISTC_04000D0B_00_LEN        2
#define PAR_CREACTC_04000D0C_00_ADDR        (PAR_CRESISTC_04000D0B_00_ADDR + PAR_CRESISTC_04000D0B_00_LEN)  //C��翹ϵ��
#define PAR_CREACTC_04000D0C_00_LEN         2
//}[��δӦ�õ�DLT645]
#define PAR_POSPTOP_04000E01_00_ADDR        (PAR_CREACTC_04000D0C_00_ADDR + PAR_CREACTC_04000D0C_00_LEN)    //�����й���������ֵ
#define PAR_POSPTOP_04000E01_00_LEN         3
#define PAR_NEGPTOP_04000E02_00_ADDR        (PAR_POSPTOP_04000E01_00_ADDR + PAR_POSPTOP_04000E01_00_LEN)    //�����й���������ֵ[��δӦ�õ�DLT645]
#define PAR_NEGPTOP_04000E02_00_LEN         3
#define PAR_VTOP_04000E03_00_ADDR           (PAR_NEGPTOP_04000E02_00_ADDR + PAR_NEGPTOP_04000E02_00_LEN)    //��ѹ����ֵ
#define PAR_VTOP_04000E03_00_LEN            2
#define PAR_VBOTTOM_04000E04_00_ADDR        (PAR_VTOP_04000E03_00_ADDR + PAR_VTOP_04000E03_00_LEN)          //��ѹ����ֵ
#define PAR_VBOTTOM_04000E04_00_LEN         2

#define PAR_T1TZ1SDTS_04010000_00_ADDR      (PAR_VBOTTOM_04000E04_00_ADDR + PAR_VBOTTOM_04000E04_00_LEN)    //��һ��ʱ��������:��1ʱ����ʼ���ڼ���ʱ�α��
#define PAR_T1TZ1SDTS_04010000_00_LEN       3
#define PAR_T1TZ2SDTS_04010000_01_ADDR      (PAR_T1TZ1SDTS_04010000_00_ADDR + PAR_T1TZ1SDTS_04010000_00_LEN)//��һ��ʱ��������:��2ʱ����ʼ���ڼ���ʱ�α��
#define PAR_T1TZ2SDTS_04010000_01_LEN       3
#define PAR_T1TZ3SDTS_04010000_02_ADDR      (PAR_T1TZ2SDTS_04010000_00_ADDR + PAR_T1TZ2SDTS_04010000_00_LEN)//��һ��ʱ��������:��3ʱ����ʼ���ڼ���ʱ�α��
#define PAR_T1TZ3SDTS_04010000_02_LEN       3
#define PAR_T1TZ4SDTS_04010000_03_ADDR      (PAR_T1TZ3SDTS_04010000_00_ADDR + PAR_T1T3SDTS_04010000_00_LEN)//��һ��ʱ��������:��4ʱ����ʼ���ڼ���ʱ�α��
#define PAR_T1TZ4SDTS_04010000_03_LEN       3
#define PAR_T1TZ5SDTS_04010000_04_ADDR      (PAR_T1TZ4SDTS_04010000_00_ADDR + PAR_T1TZ4SDTS_04010000_00_LEN)//��һ��ʱ��������:��5ʱ����ʼ���ڼ���ʱ�α��
#define PAR_T1TZ5SDTS_04010000_04_LEN       3
#define PAR_T1TZ6SDTS_04010000_05_ADDR      (PAR_T1TZ5SDTS_04010000_00_ADDR + PAR_T1TZ5SDTS_04010000_00_LEN)//��һ��ʱ��������:��6ʱ����ʼ���ڼ���ʱ�α��
#define PAR_T1TZ6SDTS_04010000_05_LEN       3
#define PAR_T1TZ7SDTS_04010000_06_ADDR      (PAR_T1TZ6SDTS_04010000_00_ADDR + PAR_T1TZ6SDTS_04010000_00_LEN)//��һ��ʱ��������:��7ʱ����ʼ���ڼ���ʱ�α��
#define PAR_T1TZ7SDTS_04010000_06_LEN       3
#define PAR_T1TZ8SDTS_04010000_07_ADDR      (PAR_T1TZ7SDTS_04010000_00_ADDR + PAR_T1TZ7SDTS_04010000_00_LEN)//��һ��ʱ��������:��8ʱ����ʼ���ڼ���ʱ�α��
#define PAR_T1TZ8SDTS_04010000_07_LEN       3
#define PAR_T1TZ9SDTS_04010000_08_ADDR      (PAR_T1TZ8SDTS_04010000_00_ADDR + PAR_T1TZ8SDTS_04010000_00_LEN)//��һ��ʱ��������:��9ʱ����ʼ���ڼ���ʱ�α��
#define PAR_T1TZ9SDTS_04010000_08_LEN       3
#define PAR_T1TZ10SDTS_04010000_09_ADDR     (PAR_T1TZ9SDTS_04010000_00_ADDR + PAR_T1TZ9SDTS_04010000_00_LEN)//��һ��ʱ��������:��10ʱ����ʼ���ڼ���ʱ�α��
#define PAR_T1TZ10SDTS_04010000_09_LEN      3
#define PAR_T1TZ11SDTS_04010000_0A_ADDR     (PAR_T1TZ10SDTS_04010000_00_ADDR + PAR_T1TZ10SDTS_04010000_00_LEN)  //��һ��ʱ��������:��11ʱ����ʼ���ڼ���ʱ�α��
#define PAR_T1TZ11SDTS_04010000_0A_LEN      3
#define PAR_T1TZ12SDTS_04010000_0B_ADDR     (PAR_T1TZ11SDTS_04010000_00_ADDR + PAR_T1TZ11SDTS_04010000_00_LEN)  //��һ��ʱ��������:��12ʱ����ʼ���ڼ���ʱ�α��
#define PAR_T1TZ12SDTS_04010000_0B_LEN      3
#define PAR_T1TZ13SDTS_04010000_0C_ADDR     (PAR_T1TZ12SDTS_04010000_00_ADDR + PAR_T1TZ12SDTS_04010000_00_LEN)  //��һ��ʱ��������:��13ʱ����ʼ���ڼ���ʱ�α��
#define PAR_T1TZ13SDTS_04010000_0C_LEN      3
#define PAR_T1TZ14SDTS_04010000_0D_ADDR     (PAR_T1TZ13SDTS_04010000_00_ADDR + PAR_T1TZ13SDTS_04010000_00_LEN)  //��һ��ʱ��������:��14ʱ����ʼ���ڼ���ʱ�α��
#define PAR_T1TZ14SDTS_04010000_0D_LEN      3
#define PAR_T1TZSDTS_04010000_TOT_LEN       (PAR_T1TZ1SDTS_04010000_00_LEN * 14)

#define PAR_T1TS1STFN_04010001_00_ADDR      (PAR_T1TZ1SDTS_04010000_00_ADDR + PAR_T1TZSDTS_04010000_TOT_LEN)    //��һ�׵�1��ʱ�α�����:��1ʱ����ʼʱ�估���ʺ�
#define PAR_T1TS1STFN_04010001_00_LEN       3
#define PAR_T1TS2STFN_04010001_01_ADDR      (PAR_T1TS1STFN_04010000_00_ADDR + PAR_T1TS1STFN_04010000_00_LEN)    //��һ�׵�1��ʱ�α�����:��2ʱ����ʼʱ�估���ʺ�
#define PAR_T1TS2STFN_04010001_01_LEN       3
#define PAR_T1TS3STFN_04010001_02_ADDR      (PAR_T1TS2STFN_04010000_01_ADDR + PAR_T1TS2STFN_04010000_01_LEN)    //��һ�׵�1��ʱ�α�����:��3ʱ����ʼʱ�估���ʺ�
#define PAR_T1TS3STFN_04010001_02_LEN       3
#define PAR_T1TS4STFN_04010001_03_ADDR      (PAR_T1TS3STFN_04010000_02_ADDR + PAR_T1TS3STFN_04010000_02_LEN)    //��һ�׵�1��ʱ�α�����:��4ʱ����ʼʱ�估���ʺ�
#define PAR_T1TS4STFN_04010001_03_LEN       3
#define PAR_T1TS5STFN_04010001_04_ADDR      (PAR_T1TS4STFN_04010000_03_ADDR + PAR_T1TS4STFN_04010000_03_LEN)    //��һ�׵�1��ʱ�α�����:��5ʱ����ʼʱ�估���ʺ�
#define PAR_T1TS5STFN_04010001_04_LEN       3
#define PAR_T1TS6STFN_04010001_05_ADDR      (PAR_T1TS5STFN_04010000_04_ADDR + PAR_T1TS5STFN_04010000_04_LEN)    //��һ�׵�1��ʱ�α�����:��6ʱ����ʼʱ�估���ʺ�
#define PAR_T1TS6STFN_04010001_05_LEN       3
#define PAR_T1TS7STFN_04010001_06_ADDR      (PAR_T1TS6STFN_04010000_05_ADDR + PAR_T1TS6STFN_04010000_05_LEN)    //��һ�׵�1��ʱ�α�����:��7ʱ����ʼʱ�估���ʺ�
#define PAR_T1TS7STFN_04010001_06_LEN       3
#define PAR_T1TS8STFN_04010001_07_ADDR      (PAR_T1TS7STFN_04010000_06_ADDR + PAR_T1TS7STFN_04010000_06_LEN)    //��һ�׵�1��ʱ�α�����:��8ʱ����ʼʱ�估���ʺ�
#define PAR_T1TS8STFN_04010001_07_LEN       3
#define PAR_T1TS9STFN_04010001_08_ADDR      (PAR_T1TS8STFN_04010000_07_ADDR + PAR_T1TS8STFN_04010000_07_LEN)    //��һ�׵�1��ʱ�α�����:��9ʱ����ʼʱ�估���ʺ�
#define PAR_T1TS9STFN_04010001_08_LEN       3
#define PAR_T1TS10STFN_04010001_09_ADDR     (PAR_T1TS9STFN_04010000_08_ADDR + PAR_T1TS9STFN_04010000_08_LEN)    //��һ�׵�1��ʱ�α�����:��10ʱ����ʼʱ�估���ʺ�
#define PAR_T1TS10STFN_04010001_09_LEN      3
#define PAR_T1TS11STFN_04010001_0A_ADDR     (PAR_T1TS10STFN_04010000_09_ADDR + PAR_T1TS10STFN_04010000_09_LEN)  //��һ�׵�1��ʱ�α�����:��11ʱ����ʼʱ�估���ʺ�
#define PAR_T1TS11STFN_04010001_0A_LEN      3
#define PAR_T1TS12STFN_04010001_0B_ADDR     (PAR_T1TS11STFN_04010000_0A_ADDR + PAR_T1TS11STFN_04010000_0A_LEN)  //��һ�׵�1��ʱ�α�����:��12ʱ����ʼʱ�估���ʺ�
#define PAR_T1TS12STFN_04010001_0B_LEN      3
#define PAR_T1TS13STFN_04010001_0C_ADDR     (PAR_T1TS12STFN_04010000_0B_ADDR + PAR_T1TS12STFN_04010000_0B_LEN)  //��һ�׵�1��ʱ�α�����:��13ʱ����ʼʱ�估���ʺ�
#define PAR_T1TS13STFN_04010001_0C_LEN      3
#define PAR_T1TS14STFN_04010001_0D_ADDR     (PAR_T1TS13STFN_04010000_0C_ADDR + PAR_T1TS13STFN_04010000_0C_LEN)  //��һ�׵�1��ʱ�α�����:��14ʱ����ʼʱ�估���ʺ�
#define PAR_T1TS14STFN_04010001_0D_LEN      3
#define PAR_T1TSSTFN_04010001_TOT_LEN       (PAR_T1TS1STFN_04010001_00_LEN * 14) //��һ��ʱ�α���������(1~14��ʱ��)�ܳ���

#define PAR_T1TS2ND_04010002_00_ADDR        (PAR_T1TS1STFN_04010001_00_ADDR + PAR_T1TSSTFN_04010001_TOT_LEN)    //��һ�׵�2��ʱ�α�����(��ʼ��ַ)
#define PAR_T1TS2ND_04010002_00_LEN         3
#define PAR_T1TS2ND_04010002_TOT_LEN        (PAR_T1TS2ND_04010002_00_LEN * 14)
#define PAR_T1TS3TH_04010002_00_ADDR        (PAR_T1TS2ND_04010002_00_ADDR + PAR_T1TS2ND_04010002_TOT_LEN)       //��һ�׵�3��ʱ�α�����(��ʼ��ַ)
#define PAR_T1TS3TH_04010002_00_LEN         3
#define PAR_T1TS3TH_04010002_TOT_LEN        (PAR_T1TS3TH_04010002_00_LEN * 14)
#define PAR_T1TS4TH_04010002_00_ADDR        (PAR_T1TS3TH_04010002_00_ADDR + PAR_T1TS3TH_04010002_TOT_LEN)       //��һ�׵�4��ʱ�α�����(��ʼ��ַ)
#define PAR_T1TS4TH_04010002_00_LEN         3
#define PAR_T1TS4TH_04010002_TOT_LEN        (PAR_T1TS4TH_04010002_00_LEN * 14)
#define PAR_T1TS5TH_04010002_00_ADDR        (PAR_T1TS4TH_04010002_00_ADDR + PAR_T1TS4TH_04010002_TOT_LEN)       //��һ�׵�5��ʱ�α�����(��ʼ��ַ)
#define PAR_T1TS5TH_04010002_00_LEN         3
#define PAR_T1TS5TH_04010002_TOT_LEN        (PAR_T1TS4TH_04010002_00_LEN * 14)
#define PAR_T1TS6TH_04010002_00_ADDR        (PAR_T1TS5TH_04010002_00_ADDR + PAR_T1TS5TH_04010002_TOT_LEN)       //��һ�׵�6��ʱ�α�����(��ʼ��ַ)
#define PAR_T1TS6TH_04010002_00_LEN         3
#define PAR_T1TS6TH_04010002_TOT_LEN        (PAR_T1TS6TH_04010002_00_LEN * 14)
#define PAR_T1TS7TH_04010002_00_ADDR        (PAR_T1TS6TH_04010002_00_ADDR + PAR_T1TS6TH_04010002_TOT_LEN)       //��һ�׵�7��ʱ�α�����(��ʼ��ַ)
#define PAR_T1TS7TH_04010002_00_LEN         3
#define PAR_T1TS7TH_04010002_TOT_LEN        (PAR_T1TS7TH_04010002_00_LEN * 14)
#define PAR_T1TS8TH_04010002_00_ADDR        (PAR_T1TS7TH_04010002_00_ADDR + PAR_T1TS7TH_04010002_TOT_LEN)       //��һ�׵�8��ʱ�α�����(��ʼ��ַ)
#define PAR_T1TS8TH_04010002_00_LEN         3
#define PAR_T1TS8TH_04010002_TOT_LEN        (PAR_T1TS8TH_04010002_00_LEN * 14)
//
#define PAR_T1_TOT_LEN                      (PAR_T1TZSDTS_04010000_TOT_LEN + PAR_T1TSSTFN_04010001_TOT_LEN * 8)

#define PAR_T2TZDAT_04020000_00_ADDR        (PAR_T1TZ1SDTS_04010000_00_ADDR + PAR_T1_TOT_LEN)                   //�ڶ���ʱ��������(��ʼ��ַ)
#define PAR_T2TZDAT_04020000_00_LEN         3
#define PAR_T2TZDAT_04020000_TOT_LEN        (PAR_T2TZDAT_04020000_00_LEN * 14)

#define PAR_T2TS1DAT_04020001_00_ADDR       (PAR_T2TZDAT_04020000_00_ADDR + PAR_T2TZDAT_04020000_TOT_LEN)       //�ڶ��׵�1��ʱ�α�����(��ʼ��ַ)
#define PAR_T2TS1DAT_04020001_00_LEN        3
#define PAR_T2TS1DAT_04020001_TOT_LEN       (PAR_T2TS1DAT_04020001_00_LEN * 14)

#define PAR_T2TSDAT_04020002_00_ADDR        (PAR_T2TS1DAT_04020001_00_ADDR + PAR_T2TS1DAT_04020001_TOT_LEN)     //�ڶ��׵�2~8��ʱ�α�����(��ʼ��ַ)
#define PAR_T2TSDAT_04020002_00_LEN         3
#define TZAMOUNT                            14                                                                  //ʱ����
#define TSAMOUNT                            (8 - 1)                                                             //��ʱ�α���(2~8)
#define PAR_T2TSDAT_04020002_TOT_LEN        (PAR_T2TSDAT_04020002_00_LEN * PAR_T2TSDAT_04020002_00_LEN * TSAMOUNT)  //��2~8��ʱ�α������ܳ���
//
#define PAR_T2_TOT_LEN                      (PAR_T2TZDAT_04020000_TOT_LEN + PAR_T2TS1DAT_04020001_TOT_LEN * 8)

#define PAR_HOLIDYFN_04030001_00_ADDR       (PAR_T2TZDAT_04020000_00_ADDR + PAR_T2_TOT_LEN)                     //��1(~254)�����������ڼ���ʱ�α��
#define PAR_HOLIDYFN_04030001_00_LEN        4
#define HOLIAMOUNT                          4//254   azh Ϊ������Щ�ռ� ���ﷴ��Ҳû�����                                                                 //����������
#define PAR_HOLIDYFN_04030001_TOT_LEN       (PAR_HOLIDYFN_04030001_00_LEN * HOLIAMOUNT)                         //�������������ܳ���
//{[��δӦ�õ�DLT645]
#define PAR_LOOPDIS1_04040101_00_ADDR       (PAR_HOLIDYFN_04030001_00_ADDR + PAR_HOLIDYFN_04030001_TOT_LEN)     //�Զ�ѭ����ʾ��1����ʾ������:DI3~DI0����
#define PAR_LOOPDIS1_04040101_00_LEN        4
#define PAR_LOOPDIS1_04040101_01_ADDR       (PAR_HOLIDYFN_04030001_00_ADDR + PAR_HOLIDYFN_04030001_TOT_LEN)     //�Զ�ѭ����ʾ��1����ʾ������:��չDI����
#define PAR_LOOPDIS1_04040101_01_LEN        1
#define PAR_LOOPDIS1_04040101_TOT_LEN       (PAR_LOOPDIS1_04040101_00_LEN + PAR_LOOPDIS1_04040101_01_LEN)
#define LOOPPAGE                            128//254   azh Ϊ������Щ�ռ� ���ﷴ��Ҳû�����                                                              //����ʾ����
#define PAR_LOOPDIS_04040101_TOT_LEN        (PAR_LOOPDIS1_04040101_TOT_LEN * LOOPPAGE)                          //ѭ�������ܳ���

#define PAR_KEYDIS1_04040201_00_ADDR        (PAR_LOOPDIS1_04040101_00_ADDR + PAR_LOOPDIS_04040101_TOT_LEN)      //����ѭ����ʾ��1����ʾ������:��չDI����
#define PAR_KEYDIS1_04040201_00_LEN         4
#define PAR_KEYDIS1_04040201_01_ADDR        (PAR_KEYDIS1_04040201_00_ADDR + PAR_KEYDIS1_04040201_00_LEN)        //����ѭ����ʾ��1����ʾ������:DI3~DI0����
#define PAR_KEYDIS1_04040201_01_LEN         1
#define PAR_KEYDIS1_04040201_TOT_LEN        (PAR_KEYDIS1_04040201_00_LEN + PAR_KEYDIS1_04040201_01_LEN)
#define KEYPAGE                             128//254                                                                 //����ʾ����
#define PAR_KEYDIS_04040201_TOT_LEN         (PAR_KEYDIS1_04040201_TOT_LEN * KEYPAGE)                            //���������ܳ���
//}[��δӦ�õ�DLT645]
#define PAR_SWVER_04800001_00_ADDR          (PAR_KEYDIS1_04040201_00_ADDR + PAR_KEYDIS_04040201_TOT_LEN)        //��������汾��(ASCII��):N0~N3
#define PAR_SWVER_04800001_00_LEN           4
#define PAR_SWVER_04800001_01_ADDR          (PAR_SWVER_04800001_00_ADDR + PAR_SWVER_04800001_00_LEN)            //��������汾��(ASCII��):N4~N7
#define PAR_SWVER_04800001_01_LEN           4
#define PAR_SWVER_04800001_02_ADDR          (PAR_SWVER_04800001_01_ADDR + PAR_SWVER_04800001_01_LEN)            //��������汾��(ASCII��):N8~N11
#define PAR_SWVER_04800001_02_LEN           4
#define PAR_SWVER_04800001_03_ADDR          (PAR_SWVER_04800001_02_ADDR + PAR_SWVER_04800001_02_LEN)            //��������汾��(ASCII��):N12~N15
#define PAR_SWVER_04800001_03_LEN           4
#define PAR_SWVER_04800001_04_ADDR          (PAR_SWVER_04800001_03_ADDR + PAR_SWVER_04800001_03_LEN)            //��������汾��(ASCII��):N16~N19
#define PAR_SWVER_04800001_04_LEN           4
#define PAR_SWVER_04800001_05_ADDR          (PAR_SWVER_04800001_04_ADDR + PAR_SWVER_04800001_04_LEN)            //��������汾��(ASCII��):N20~N23
#define PAR_SWVER_04800001_05_LEN           4
#define PAR_SWVER_04800001_06_ADDR          (PAR_SWVER_04800001_05_ADDR + PAR_SWVER_04800001_05_LEN)            //��������汾��(ASCII��):N24~N27
#define PAR_SWVER_04800001_06_LEN           4
#define PAR_SWVER_04800001_07_ADDR          (PAR_SWVER_04800001_06_ADDR + PAR_SWVER_04800001_06_LEN)            //��������汾��(ASCII��):N28~N31
#define PAR_SWVER_04800001_07_LEN           4
#define PAR_SWVER_04800001_TOT_LEN          (PAR_SWVER_04800001_00_LEN * 8)
#define PAR_HWVER_04800002_00_ADDR          (PAR_SWVER_04800001_00_ADDR + PAR_SWVER_04800001_TOT_LEN)           //����Ӳ���汾��(ASCII��):N0~N3
#define PAR_HWVER_04800002_00_LEN           4
#define PAR_HWVER_04800002_01_ADDR          (PAR_HWVER_04800002_00_ADDR + PAR_HWVER_04800002_00_LEN)            //����Ӳ���汾��(ASCII��):N4~N7
#define PAR_HWVER_04800002_01_LEN           4
#define PAR_HWVER_04800002_02_ADDR          (PAR_HWVER_04800002_01_ADDR + PAR_HWVER_04800002_01_LEN)            //����Ӳ���汾��(ASCII��):N8~N11
#define PAR_HWVER_04800002_02_LEN           4
#define PAR_HWVER_04800002_03_ADDR          (PAR_HWVER_04800002_02_ADDR + PAR_HWVER_04800002_02_LEN)            //����Ӳ���汾��(ASCII��):N12~N15
#define PAR_HWVER_04800002_03_LEN           4
#define PAR_HWVER_04800002_04_ADDR          (PAR_HWVER_04800002_03_ADDR + PAR_HWVER_04800002_03_LEN)            //����Ӳ���汾��(ASCII��):N16~N19
#define PAR_HWVER_04800002_04_LEN           4
#define PAR_HWVER_04800002_05_ADDR          (PAR_HWVER_04800002_04_ADDR + PAR_HWVER_04800002_04_LEN)            //����Ӳ���汾��(ASCII��):N20~N23
#define PAR_HWVER_04800002_05_LEN           4
#define PAR_HWVER_04800002_06_ADDR          (PAR_HWVER_04800002_05_ADDR + PAR_HWVER_04800002_05_LEN)            //����Ӳ���汾��(ASCII��):N24~N27
#define PAR_HWVER_04800002_06_LEN           4
#define PAR_HWVER_04800002_07_ADDR          (PAR_HWVER_04800002_06_ADDR + PAR_HWVER_04800002_06_LEN)            //����Ӳ���汾��(ASCII��):N28~N31
#define PAR_HWVER_04800002_07_LEN           4
#define PAR_HWVER_04800002_TOT_LEN          (PAR_HWVER_04800002_00_LEN * 8)

#define PAR_MFTNUM_04800003_00_ADDR         (PAR_HWVER_04800002_00_ADDR + PAR_HWVER_04800002_TOT_LEN)           //���ұ��(ASCII��):N0~N3
#define PAR_MFTNUM_04800003_00_LEN          4
#define PAR_MFTNUM_04800003_01_ADDR         (PAR_MFTNUM_04800003_00_ADDR + PAR_MFTNUM_04800003_00_LEN)            //���ұ��(ASCII��):N4~N7
#define PAR_MFTNUM_04800003_01_LEN          4
#define PAR_MFTNUM_04800003_02_ADDR         (PAR_MFTNUM_04800003_01_ADDR + PAR_MFTNUM_04800003_01_LEN)            //���ұ��(ASCII��):N8~N11
#define PAR_MFTNUM_04800003_02_LEN          4
#define PAR_MFTNUM_04800003_03_ADDR         (PAR_MFTNUM_04800003_02_ADDR + PAR_MFTNUM_04800003_02_LEN)            //���ұ��(ASCII��):N12~N15
#define PAR_MFTNUM_04800003_03_LEN          4
#define PAR_MFTNUM_04800003_04_ADDR         (PAR_MFTNUM_04800003_03_ADDR + PAR_MFTNUM_04800003_03_LEN)            //���ұ��(ASCII��):N16~N19
#define PAR_MFTNUM_04800003_04_LEN          4
#define PAR_MFTNUM_04800003_05_ADDR         (PAR_MFTNUM_04800003_04_ADDR + PAR_MFTNUM_04800003_04_LEN)            //���ұ��(ASCII��):N20~N23
#define PAR_MFTNUM_04800003_05_LEN          4
#define PAR_MFTNUM_04800003_06_ADDR         (PAR_MFTNUM_04800003_05_ADDR + PAR_MFTNUM_04800003_05_LEN)            //���ұ��(ASCII��):N24~N27
#define PAR_MFTNUM_04800003_06_LEN          4
#define PAR_MFTNUM_04800003_07_ADDR         (PAR_MFTNUM_04800003_06_ADDR + PAR_MFTNUM_04800003_06_LEN)            //���ұ��(ASCII��):N28~N31
#define PAR_MFTNUM_04800003_07_LEN          4
#define PAR_MFTNUM_04800003_TOT_LEN         (PAR_MFTNUM_04800003_00_LEN * 8)
//COME_ON
//�󶨵ĸ�ѹ���ַ����zigbeeģ���ַ
#define PAR_HVMETER_ADR_ADDR                (PAR_MFTNUM_04800003_07_ADDR +PAR_MFTNUM_04800003_07_LEN)
#define PAR_HVMETER_ADR_LEN                 6
#define PAR_HVMETER_ADR_CRC_LEN             (6 + 1)//+1bCRC
#define PAR_HVMETER_ZIGBEE_ADR_ADDR         (PAR_HVMETER_ADR_ADDR +PAR_HVMETER_ADR_CRC_LEN)
#define PAR_HVMETER_ZIGBEE_ADR_LEN          8
#define PAR_HVMETER_ZIGBEE_ADR_CRC_LEN      (8 + 1)//+1bCRC

//�й���ʾ������Ч�Ա�־����3��־�ֽںͼ���2��־�ֽ� 1�ֽڱ��� ֻҪ�����ù����ó���Ч
#define PAR_DIS_VALID_FLAG_ADDR             (PAR_HVMETER_ZIGBEE_ADR_ADDR +PAR_HVMETER_ZIGBEE_ADR_CRC_LEN)//��ʱ0x61004B1Fu
#define PAR_DIS_VALID_FLAG_LEN              (6+1)//6+1crc
//�ն��ϴε���ʱ��
#define PAR_TER_LAST_TIME_ADDR              (PAR_DIS_VALID_FLAG_ADDR+PAR_DIS_VALID_FLAG_LEN)
#define PAR_TER_LAST_TIME_LEN               (6+1)             

//azh ����ʱ �ѵ�ǰ�ն��ἰ�¶�������(�ṹ���ݣ�ֻ����һ��������)���浽eeprom �ϵ�ʱ����
#define CON_DAY_DATA_BASE_ADDR              (PAR_TER_LAST_TIME_ADDR+PAR_TER_LAST_TIME_LEN)
#define CON_DAY_DATA_BASE_BLOCK             1024//Ԥ����Щ�ֽ� ����ṹ
#define CON_MON_DATA_BASE_ADDR              (CON_DAY_DATA_BASE_ADDR+CON_DAY_DATA_BASE_BLOCK)
#define CON_MON_DATA_BASE_BLOCK             1024//Ԥ����Щ�ֽ� ����ṹ

//�ն˲��������ַ����궨��(Ԥ������ռ�PAR_DIS_VALID_FLAG_ADDR 0x61004B1Fu Լ200bytes)
#define TER_PAR_HEAD_ADDR                   0x61004c00
#define TER_PAR_BAK_LEN                     0x2000//ʵ��ռ�ÿռ�:0x1853

#define     F1_TERMINAL_COM_PARA_ADDR   (TER_PAR_HEAD_ADDR)                       // F1  �ն�ͨ�Ų���AFN=04
#define     F1_TERMINAL_COM_PARA_LEN    6
#define     CRC16_LEN    2
#define     F3_STAIPADDR_ADDR           F1_TERMINAL_COM_PARA_ADDR +  (F1_TERMINAL_COM_PARA_LEN + CRC16_LEN) * 2   // F3: ��վIP��ַ�Ͷ˿�
#define     F3_STAIPADDR_LEN            28
#define     F207_ADDR_ADDR              F3_STAIPADDR_ADDR +  (F3_STAIPADDR_LEN + 2) * 2                     //F207 ��������ַ��4���ֽ�
#define     F207_ADDR_LEN               4
#define     F4_STAPHONENO_ADDR          F207_ADDR_ADDR +   (F207_ADDR_LEN + 2) * 2                          //F4����վ�绰����Ͷ������ĺ���
#define     F4_STAPHONENO_LEN           16

#define     F8_TERUPCOMWORKING_ADDR     F4_STAPHONENO_ADDR +  (F4_STAPHONENO_LEN + 2) * 2                   //F8 �ն�����ͨ�Ź�����ʽ����̫ר��������ר����
#define     F8_TERUPCOMWORKING_LEN      8
#define     F9_EVENTRECORDCONFSET_ADDR  F8_TERUPCOMWORKING_ADDR +  (F8_TERUPCOMWORKING_LEN + 2) * 2         //F9���¼���¼��������
#define     F9_EVENTRECORDCONFSET_LEN   16
#define     F10_METERPARA_ADDR          F9_EVENTRECORDCONFSET_ADDR + (F9_EVENTRECORDCONFSET_LEN + 2) * 2    //F10���ն˵��ܱ�/��������װ�����ò���,���2+2040*27��Ŀǰ����8��������
#define     F10_METERPARA_LEN           (2 + 8 * 27)

#define     F25_MPBASEPARA_ADDR         F10_METERPARA_ADDR + (F10_METERPARA_LEN + 2) * 2                    //F25���������������
#define     F25_MPBASEPARA_LEN          (11 + 2) * 8

#define     F38_JOB1PARA_ADDR           F25_MPBASEPARA_ADDR + (F25_MPBASEPARA_LEN + 2) * 2                  //F38��1��������������
#define     F38_JOB1PARA_LEN            35                                                          //1�ֽڴ���ţ�1�ֽ�����������1�ֽ�С��ţ�1�ֽ���Ϣ��������31�ֽ���Ϣ���־

#define     F39_JOB2PARA_ADDR           F38_JOB1PARA_ADDR + (F38_JOB1PARA_LEN + 2) * 2                      //F39��2��������������
#define     F39_JOB2PARA_LEN            35

#define     F65_UPDATA1PARA_ADDR        F39_JOB2PARA_ADDR + (F39_JOB2PARA_LEN + 2) * 2                      //F65����ʱ�ϱ�1�������������ã����64��
#define     F65_UPDATA1DUID_NUM         32
#define     F65_UPDATA1PARA_LEN         F65_UPDATA1DUID_NUM * 4 + 9
#define     F65_UPDATA1JOB_NUM          16                                                                   //�ݶ����8������
#define     F66_UPDATA2PARA_ADDR        F65_UPDATA1PARA_ADDR + (F65_UPDATA1PARA_LEN + 2) * F65_UPDATA1JOB_NUM
#define     F66_UPDATA2DUID_NUM         32
#define     F66_UPDATA2PARA_LEN         F66_UPDATA2DUID_NUM * 4 + 9
#define     F66_UPDATA2JOB_NUM          16
#define     F67_UPDATA1CTRL_ADDR        F66_UPDATA2PARA_ADDR + (F66_UPDATA2PARA_LEN + 2) * F66_UPDATA2JOB_NUM //F67����ʱ����1��������������/ֹͣ����,55H������AAֹͣ��������Ч
#define     F67_UPDATA1CTRL_LEN         1
#define     F68_UPDATA2CTRL_ADDR        F67_UPDATA1CTRL_ADDR + F67_UPDATA1CTRL_LEN * F65_UPDATA1JOB_NUM //F68����ʱ����2��������������/ֹͣ����,55H������AAֹͣ��������Ч
#define     F68_UPDATA2CTRL_LEN         1

#define     F26_MPLIMIT_ADDR            F68_UPDATA2CTRL_ADDR + F68_UPDATA2CTRL_LEN * F66_UPDATA2JOB_NUM //F26����������ֵ����
#define     F26_MPLIMIT_LEN             57
#define     F27_COPIRONLOSSPARA_ADDR    F26_MPLIMIT_ADDR + F26_MPLIMIT_LEN + 2                     //F27��������ͭ���������
#define     F27_COPIRONLOSSPARA_LEN     24
#define     F29_METERADDR_ADDR_ADDR     F27_COPIRONLOSSPARA_ADDR + F27_COPIRONLOSSPARA_LEN + 2      //F29���ն˵��ص��ܱ���ʾ��
#define     F29_METERADDR_ADDR_LEN      12
#define     F30_METERCOPYFLAG_ADDR      F29_METERADDR_ADDR_ADDR + F29_METERADDR_ADDR_LEN + 2       //F30��̨�����г���ͣ��/Ͷ������
#define     F30_METERCOPYFLAG_LEN       1
#define     F33_SUMGFREEZEPARA_ADDR     F30_METERCOPYFLAG_ADDR + F30_METERCOPYFLAG_LEN + 2         //F33���ն˳������в�������
#define     F33_SUMGFREEZEPARA_LEN      4 * 24 + 13 + 1                                            //��һ�ֽ������ж��Ƿ�����Ч������
#define     F33_PORTNUM                 4
#define     F34_PORTPROPERTY_ADDR       F33_SUMGFREEZEPARA_ADDR + (F33_SUMGFREEZEPARA_LEN + 2) * F33_PORTNUM     //
#define     F34_PORTPROPERTY_LEN        15
#define     F37_LINKPARA_ADDR               F34_PORTPROPERTY_ADDR + F34_PORTPROPERTY_LEN + 2
#define     F37_LINKPARA_LEN            15
#define     F57_SOUND_PROHIBIT              (F37_LINKPARA_ADDR + F57_SOUND_PROHIBIT_LEN) + 2              // F57 �����澯����/��ֹ��־
#define     F57_SOUND_PROHIBIT_LEN          3
#define     F59_METER_EXCERPTION_LIMIT      (F57_SOUND_PROHIBIT + F57_SOUND_PROHIBIT_LEN) + 2            // F59 ���ܱ��쳣�ж���ֵ
#define     F59_METER_EXCERPTION_LIMIT_LEN  4

#define     F2_RELAY_TERMINAL_SET_ADDR  (F59_METER_EXCERPTION_LIMIT + F59_METER_EXCERPTION_LIMIT_LEN + 2)   //F2  �ն��м�ת������
#define     F2_RELAY_TERMINAL_SET_LEN      (16 * 2 + 1)
#define     F5_TERENCRYPT_ADDR          F2_RELAY_TERMINAL_SET_ADDR +  F2_RELAY_TERMINAL_SET_LEN + 2            //F5  �ն���������
#define     F5_TERENCRYPT_LEN           3
#define     F6_TERGROUPADDR_ADDR        F5_TERENCRYPT_ADDR +  F5_TERENCRYPT_LEN + 2                 //F6  �ն����ַ����
#define     F6_TERGROUPADDR_LEN         16
#define     F7_TERIPADDR_ADDR           F6_TERGROUPADDR_ADDR +  F6_TERGROUPADDR_LEN + 2            //F7 �ն�IP��ַ�Ͷ˿�
#define     F7_TERIPADDR_LEN            (24 + 20 + 20)
#define     F16_PDPINFO_ADDR            F7_TERIPADDR_ADDR + F7_TERIPADDR_LEN + 2                   //F16������ר���û���������
#define     F16_PDPINFO_LEN             64

#define CURRENT_LAST_ADDR               (F16_PDPINFO_ADDR + F16_PDPINFO_LEN + 2)
//�������ط�����ռ���� ���¿ռ�
#define CON_GRTASK_PARA_BASE_ADDR         0x61006c00  //temp
#define CON_GRTASK_PARA_BASE_BOLCK        0x200      //128*4(ÿ������һ��������)�ɱ���32���������

#define CON_DAY_MONTH_INFO_BASE_ADDR      (CON_GRTASK_PARA_BASE_ADDR+CON_GRTASK_PARA_BASE_BOLCK) //�����������������ݶ���������ز�������
#define CON_DAY_MONTH_INFO_BASE_BLOCK     (512*4)//(CON_DAYMONTH_DATA_LEN_MP*4) //4�����������Ԥ������Ŀռ�
//0x7600
#define CON_IMPALARM_INFO_BASE_ADDR        (CON_DAY_MONTH_INFO_BASE_ADDR+CON_DAY_MONTH_INFO_BASE_BLOCK)
#define CON_IMPALARM_AUXINFO_HEAD_ADDR     (CON_IMPALARM_INFO_BASE_ADDR+16)//ǰ��16���ֽ����ڱ���澯������Ϣ
#define CON_IMPALARM_INFO_BLOCK             1024//ע�� ʵ�ʵ�����256�� ����ܱ���250 ʵ�ʲű���120��
#define CON_NORMALARM_INFO_BASE_ADDR       (CON_IMPALARM_INFO_BASE_ADDR+CON_IMPALARM_INFO_BLOCK)
#define CON_NORMALARM_AUXINFO_HEAD_ADDR    (CON_NORMALARM_INFO_BASE_ADDR+16)
#define CON_NORMALARM_INFO_BLOCK            1024
//0x7E00
#define CON_DAY_LAST_TIME_ADDR            (CON_NORMALARM_INFO_BASE_ADDR+CON_NORMALARM_INFO_BLOCK) //�ն����ϴζ���ʱ��
#define CON_DAY_LAST_TIME_LEN             (6+1) //1byte crc
#define CON_MONTH_LAST_TIME_ADDR          (CON_DAY_LAST_TIME_ADDR+CON_DAY_LAST_TIME_LEN) //�¶����ϴζ���ʱ��
#define CON_MONTH_LAST_TIME_LEN           (6+1) //1byte crc
#define CON_FROZE_LAST_TIME_ADDR          (CON_MONTH_LAST_TIME_ADDR+CON_MONTH_LAST_TIME_LEN) //�����ն����ϴζ���ʱ��
#define CON_FROZE_LAST_TIME_LEN           (6+1) //1byte crc
#define CON_FROZE_COPY_LAST_TIME_ADDR     (CON_FROZE_LAST_TIME_ADDR+CON_FROZE_LAST_TIME_LEN) //�����շ�����ϴγ���ʱ��
#define CON_FROZE_COPY_LAST_TIME_LEN      (6+1) //1byte crc
//��Ҫ�Ǳ���һ�����񼰶�����������в���(Ŀǰ��Ҫָ�´���������ʱ��)
#define CON_JOBRUNINFO_PER_LEN              (6+1)//1crc
#define CON_JOB1RUNINFO_ADDR                (CON_FROZE_COPY_LAST_TIME_ADDR+CON_FROZE_COPY_LAST_TIME_LEN)
#define CON_JOB2RUNINFO_ADDR                (CON_JOB1RUNINFO_ADDR+CON_JOBRUNINFO_BLOCK_LEN)
#define CON_JOBRUNINFO_BLOCK_LEN            (CON_JOBRUNINFO_PER_LEN*32)//Ԥ��32������ ʵ��16

//-----------------------------------------------------------------------------
// ˲ʱ�����壺����ȫ�ֵ�����gVarArray[VAR_RAM_ARRAY_MAX]��
// ����ĵ�ַΪ��0x80000000 ��ʾRAM�����鱣������, ÿ������ռ��4�ֽ�
//-----------------------------------------------------------------------------
#define INTERNAL_RAM_HEAD_ADDR              0x80000000          //��ʾRAM�����鱣������

#define VAR_AVOLT_02010100_00_ADDR          INTERNAL_RAM_HEAD_ADDR          //A���ѹ
#define VAR_AVOLT_02010100_00_LEN           2
#define VAR_BVOLT_02010200_00_ADDR          (VAR_AVOLT_02010100_00_ADDR + 1)//B���ѹ
#define VAR_BVOLT_02010200_00_LEN           2
#define VAR_CVOLT_02010300_00_ADDR          (VAR_BVOLT_02010200_00_ADDR + 1)//C���ѹ
#define VAR_CVOLT_02010300_00_LEN           2

#define VAR_ACURRENT_02020100_00_ADDR       (VAR_CVOLT_02010300_00_ADDR + 1)    //A�����
#define VAR_ACURRENT_02020100_00_LEN        3
#define VAR_BCURRENT_02020200_00_ADDR       (VAR_ACURRENT_02020100_00_ADDR + 1) //B�����
#define VAR_BCURRENT_02020200_00_LEN        3
#define VAR_CCURRENT_02020300_00_ADDR       (VAR_BCURRENT_02020200_00_ADDR + 1) //C�����
#define VAR_CCURRENT_02020300_00_LEN        3

#define VAR_TOTPPOW_02030000_00_ADDR        (VAR_CCURRENT_02020300_00_ADDR + 1) //˲ʱ�й�����
#define VAR_TOTPPOW_02030000_00_LEN         3
#define VAR_APPOW_02030100_00_ADDR          (VAR_TOTPPOW_02030000_00_ADDR + 1)  //A��˲ʱ�й�����
#define VAR_APPOW_02030100_00_LEN           3
#define VAR_BPPOW_02030200_00_ADDR          (VAR_APPOW_02030100_00_ADDR + 1)    //B��˲ʱ�й�����
#define VAR_BPPOW_02030200_00_LEN           3
#define VAR_CPPOW_02030300_00_ADDR          (VAR_BPPOW_02030200_00_ADDR + 1)    //C��˲ʱ�й�����
#define VAR_CPPOW_02030300_00_LEN           3

#define VAR_TOTQPOW_02040000_00_ADDR        (VAR_CPPOW_02030300_00_ADDR + 1)    //˲ʱ�޹�����
#define VAR_TOTQPOW_02040000_00_LEN         3
#define VAR_AQPOW_02040100_00_ADDR          (VAR_TOTQPOW_02040000_00_ADDR + 1)  //A��˲ʱ�޹�����
#define VAR_AQPOW_02040100_00_LEN           3
#define VAR_BQPOW_02040200_00_ADDR          (VAR_AQPOW_02040100_00_ADDR + 1)    //B��˲ʱ�޹�����
#define VAR_BQPOW_02040200_00_LEN           3
#define VAR_CQPOW_02040300_00_ADDR          (VAR_BQPOW_02040200_00_ADDR + 1)    //C��˲ʱ�޹�����
#define VAR_CQPOW_02040300_00_LEN           3

#define VAR_TOTSPOW_02050000_00_ADDR        (VAR_CQPOW_02040300_00_ADDR + 1)    //˲ʱ���ڹ���
#define VAR_TOTSPOW_02050000_00_LEN         3
#define VAR_ASPOW_02050100_00_ADDR          (VAR_TOTSPOW_02050000_00_ADDR + 1)  //A��˲ʱ���ڹ���
#define VAR_ASPOW_02050100_00_LEN           3
#define VAR_BSPOW_02050200_00_ADDR          (VAR_ASPOW_02050100_00_ADDR + 1)    //B��˲ʱ���ڹ���
#define VAR_BSPOW_02050200_00_LEN           3
#define VAR_CSPOW_02050300_00_ADDR          (VAR_BSPOW_02050200_00_ADDR + 1)    //C��˲ʱ���ڹ���
#define VAR_CSPOW_02050300_00_LEN           3

#define VAR_TOTFACT_02060000_00_ADDR        (VAR_CSPOW_02050300_00_ADDR + 1)    //��������
#define VAR_TOTFACT_02060000_00_LEN         2
#define VAR_AFACT_02060100_00_ADDR          (VAR_TOTFACT_02060000_00_ADDR + 1)  //A�๦������
#define VAR_AFACT_02060100_00_LEN           2
#define VAR_BFACT_02060200_00_ADDR          (VAR_AFACT_02060100_00_ADDR + 1)    //B�๦������
#define VAR_BFACT_02060200_00_LEN           2
#define VAR_CFACT_02060300_00_ADDR          (VAR_BFACT_02060200_00_ADDR + 1)    //C�๦������
#define VAR_CFACT_02060300_00_LEN           2

#define VAR_AANGL_02070100_00_ADDR          (VAR_CFACT_02060300_00_ADDR + 1)    //A�����
#define VAR_AANGL_02070100_00_LEN           2
#define VAR_BANGL_02070200_00_ADDR          (VAR_AANGL_02070100_00_ADDR + 1)    //B�����
#define VAR_BANGL_02070200_00_LEN           2
#define VAR_CANGL_02070300_00_ADDR          (VAR_BANGL_02070200_00_ADDR + 1)    //C�����
#define VAR_CANGL_02070300_00_LEN           2

#define VAR_AVDIST_02080100_00_ADDR         (VAR_CANGL_02070300_00_ADDR + 1)    //A���ѹ����ʧ���(Distortion)
#define VAR_AVDIST_02080100_00_LEN          2
#define VAR_BVDIST_02080200_00_ADDR         (VAR_AVDIST_02080100_00_ADDR + 1)   //B���ѹ����ʧ���(Distortion)
#define VAR_BVDIST_02080200_00_LEN          2
#define VAR_CVDIST_02080300_00_ADDR         (VAR_BVDIST_02080200_00_ADDR + 1)   //C���ѹ����ʧ���(Distortion)
#define VAR_CVDIST_02080300_00_LEN          2

#define VAR_AIDIST_02090100_00_ADDR         (VAR_CVDIST_02080300_00_ADDR + 1)   //A���������ʧ���(Distortion)
#define VAR_AIDIST_02090100_00_LEN          2
#define VAR_BIDIST_02090200_00_ADDR         (VAR_AIDIST_02090100_00_ADDR + 1)   //B���������ʧ���(Distortion)
#define VAR_BIDIST_02090200_00_LEN          2
#define VAR_CIDIST_02090300_00_ADDR         (VAR_BIDIST_02090200_00_ADDR + 1)   //C���������ʧ���(Distortion)
#define VAR_CIDIST_02090300_00_LEN          2

#define VAR_AV1HARM_020A0101_00_ADDR        (VAR_CIDIST_02090300_00_ADDR + 1)   //A���ѹ1��г������(Harmonic)
#define VAR_AV1HARM_020A0101_00_LEN         2
#define VAR_AV2HARM_020A0102_00_ADDR        (VAR_AV1HARM_020A0101_00_ADDR + 1)  //A���ѹ2��г������(Harmonic)
#define VAR_AV2HARM_020A0102_00_LEN         2
#define VAR_AV3HARM_020A0103_00_ADDR        (VAR_AV2HARM_020A0102_00_ADDR + 1)  //A���ѹ3��г������(Harmonic)
#define VAR_AV3HARM_020A0103_00_LEN         2
#define VAR_AV4HARM_020A0104_00_ADDR        (VAR_AV3HARM_020A0103_00_ADDR + 1)  //A���ѹ4��г������(Harmonic)
#define VAR_AV4HARM_020A0104_00_LEN         2
#define VAR_AV5HARM_020A0105_00_ADDR        (VAR_AV4HARM_020A0104_00_ADDR + 1)  //A���ѹ5��г������(Harmonic)
#define VAR_AV5HARM_020A0105_00_LEN         2
#define VAR_AV6HARM_020A0106_00_ADDR        (VAR_AV5HARM_020A0105_00_ADDR + 1)  //A���ѹ6��г������(Harmonic)
#define VAR_AV6HARM_020A0106_00_LEN         2
#define VAR_AV7HARM_020A0107_00_ADDR        (VAR_AV6HARM_020A0106_00_ADDR + 1)  //A���ѹ7��г������(Harmonic)
#define VAR_AV7HARM_020A0107_00_LEN         2
#define VAR_AV8HARM_020A0108_00_ADDR        (VAR_AV7HARM_020A0107_00_ADDR + 1)  //A���ѹ8��г������(Harmonic)
#define VAR_AV8HARM_020A0108_00_LEN         2
#define VAR_AV9HARM_020A0109_00_ADDR        (VAR_AV8HARM_020A0108_00_ADDR + 1)  //A���ѹ9��г������(Harmonic)
#define VAR_AV9HARM_020A0109_00_LEN         2
#define VAR_AV10HARM_020A010A_00_ADDR       (VAR_AV9HARM_020A0109_00_ADDR + 1)  //A���ѹ10��г������(Harmonic)
#define VAR_AV10HARM_020A010A_00_LEN        2
#define VAR_AV11HARM_020A010B_00_ADDR       (VAR_AV10HARM_020A010A_00_ADDR + 1) //A���ѹ11��г������(Harmonic)
#define VAR_AV11HARM_020A010B_00_LEN        2
#define VAR_AV12HARM_020A010C_00_ADDR       (VAR_AV11HARM_020A010B_00_ADDR + 1) //A���ѹ12��г������(Harmonic)
#define VAR_AV12HARM_020A010C_00_LEN        2
#define VAR_AV13HARM_020A010D_00_ADDR       (VAR_AV12HARM_020A010C_00_ADDR + 1) //A���ѹ13��г������(Harmonic)
#define VAR_AV13HARM_020A010D_00_LEN        2
#define VAR_AV14HARM_020A010E_00_ADDR       (VAR_AV13HARM_020A010D_00_ADDR + 1) //A���ѹ14��г������(Harmonic)
#define VAR_AV14HARM_020A010E_00_LEN        2
#define VAR_AV15HARM_020A010F_00_ADDR       (VAR_AV14HARM_020A010E_00_ADDR + 1) //A���ѹ15��г������(Harmonic)
#define VAR_AV15HARM_020A010F_00_LEN        2
#define VAR_AV16HARM_020A0110_00_ADDR       (VAR_AV15HARM_020A010F_00_ADDR + 1) //A���ѹ16��г������(Harmonic)
#define VAR_AV16HARM_020A0110_00_LEN        2
#define VAR_AV17HARM_020A0111_00_ADDR       (VAR_AV16HARM_020A0110_00_ADDR + 1) //A���ѹ17��г������(Harmonic)
#define VAR_AV17HARM_020A0111_00_LEN        2
#define VAR_AV18HARM_020A0112_00_ADDR       (VAR_AV17HARM_020A0111_00_ADDR + 1) //A���ѹ18��г������(Harmonic)
#define VAR_AV18HARM_020A0112_00_LEN        2
#define VAR_AV19HARM_020A0113_00_ADDR       (VAR_AV18HARM_020A0112_00_ADDR + 1) //A���ѹ19��г������(Harmonic)
#define VAR_AV19HARM_020A0113_00_LEN        2
#define VAR_AV20HARM_020A0114_00_ADDR       (VAR_AV19HARM_020A0113_00_ADDR + 1) //A���ѹ20��г������(Harmonic)
#define VAR_AV20HARM_020A0114_00_LEN        2
#define VAR_AV21HARM_020A0115_00_ADDR       (VAR_AV20HARM_020A0114_00_ADDR + 1) //A���ѹ21��г������(Harmonic)
#define VAR_AV21HARM_020A0115_00_LEN        2

#define VAR_BV1HARM_020A0201_00_ADDR        (VAR_AV21HARM_020A0115_00_ADDR + 1) //B���ѹ1��г������(Harmonic)
#define VAR_BV1HARM_020A0201_00_LEN         2
#define VAR_BV2HARM_020A0202_00_ADDR        (VAR_BV1HARM_020A0201_00_ADDR + 1)  //B���ѹ2��г������(Harmonic)
#define VAR_BV2HARM_020A0202_00_LEN         2
#define VAR_BV3HARM_020A0203_00_ADDR        (VAR_BV2HARM_020A0202_00_ADDR + 1)  //B���ѹ3��г������(Harmonic)
#define VAR_BV3HARM_020A0203_00_LEN         2
#define VAR_BV4HARM_020A0204_00_ADDR        (VAR_BV3HARM_020A0203_00_ADDR + 1)  //B���ѹ4��г������(Harmonic)
#define VAR_BV4HARM_020A0204_00_LEN         2
#define VAR_BV5HARM_020A0205_00_ADDR        (VAR_BV4HARM_020A0204_00_ADDR + 1)  //B���ѹ5��г������(Harmonic)
#define VAR_BV5HARM_020A0205_00_LEN         2
#define VAR_BV6HARM_020A0206_00_ADDR        (VAR_BV5HARM_020A0205_00_ADDR + 1)  //B���ѹ6��г������(Harmonic)
#define VAR_BV6HARM_020A0206_00_LEN         2
#define VAR_BV7HARM_020A0207_00_ADDR        (VAR_BV6HARM_020A0206_00_ADDR + 1)  //B���ѹ7��г������(Harmonic)
#define VAR_BV7HARM_020A0207_00_LEN         2
#define VAR_BV8HARM_020A0208_00_ADDR        (VAR_BV7HARM_020A0207_00_ADDR + 1)  //B���ѹ8��г������(Harmonic)
#define VAR_BV8HARM_020A0208_00_LEN         2
#define VAR_BV9HARM_020A0209_00_ADDR        (VAR_BV8HARM_020A0208_00_ADDR + 1)  //B���ѹ9��г������(Harmonic)
#define VAR_BV9HARM_020A0209_00_LEN         2
#define VAR_BV10HARM_020A020A_00_ADDR       (VAR_BV9HARM_020A0209_00_ADDR + 1)  //B���ѹ10��г������(Harmonic)
#define VAR_BV10HARM_020A020A_00_LEN        2
#define VAR_BV11HARM_020A020B_00_ADDR       (VAR_BV10HARM_020A020A_00_ADDR + 1) //B���ѹ11��г������(Harmonic)
#define VAR_BV11HARM_020A020B_00_LEN        2
#define VAR_BV12HARM_020A020C_00_ADDR       (VAR_BV11HARM_020A020B_00_ADDR + 1) //B���ѹ12��г������(Harmonic)
#define VAR_BV12HARM_020A020C_00_LEN        2
#define VAR_BV13HARM_020A020D_00_ADDR       (VAR_BV12HARM_020A020C_00_ADDR + 1) //B���ѹ13��г������(Harmonic)
#define VAR_BV13HARM_020A020D_00_LEN        2
#define VAR_BV14HARM_020A020E_00_ADDR       (VAR_BV13HARM_020A020D_00_ADDR + 1) //B���ѹ14��г������(Harmonic)
#define VAR_BV14HARM_020A020E_00_LEN        2
#define VAR_BV15HARM_020A020F_00_ADDR       (VAR_BV14HARM_020A020E_00_ADDR + 1) //B���ѹ15��г������(Harmonic)
#define VAR_BV15HARM_020A020F_00_LEN        2
#define VAR_BV16HARM_020A0210_00_ADDR       (VAR_BV15HARM_020A020F_00_ADDR + 1) //B���ѹ16��г������(Harmonic)
#define VAR_BV16HARM_020A0210_00_LEN        2
#define VAR_BV17HARM_020A0211_00_ADDR       (VAR_BV16HARM_020A0210_00_ADDR + 1) //B���ѹ17��г������(Harmonic)
#define VAR_BV17HARM_020A0211_00_LEN        2
#define VAR_BV18HARM_020A0212_00_ADDR       (VAR_BV17HARM_020A0211_00_ADDR + 1) //B���ѹ18��г������(Harmonic)
#define VAR_BV18HARM_020A0212_00_LEN        2
#define VAR_BV19HARM_020A0213_00_ADDR       (VAR_BV18HARM_020A0212_00_ADDR + 1) //B���ѹ19��г������(Harmonic)
#define VAR_BV19HARM_020A0213_00_LEN        2
#define VAR_BV20HARM_020A0214_00_ADDR       (VAR_BV19HARM_020A0213_00_ADDR + 1) //B���ѹ20��г������(Harmonic)
#define VAR_BV20HARM_020A0214_00_LEN        2
#define VAR_BV21HARM_020A0215_00_ADDR       (VAR_BV20HARM_020A0214_00_ADDR + 1) //B���ѹ21��г������(Harmonic)
#define VAR_BV21HARM_020A0215_00_LEN        2

#define VAR_CV1HARM_020A0301_00_ADDR        (VAR_BV21HARM_020A0215_00_ADDR + 1) //C���ѹ1��г������(Harmonic)
#define VAR_CV1HARM_020A0301_00_LEN         2
#define VAR_CV2HARM_020A0302_00_ADDR        (VAR_CV1HARM_020A0301_00_ADDR + 1)  //C���ѹ2��г������(Harmonic)
#define VAR_CV2HARM_020A0302_00_LEN         2
#define VAR_CV3HARM_020A0303_00_ADDR        (VAR_CV2HARM_020A0302_00_ADDR + 1)  //C���ѹ3��г������(Harmonic)
#define VAR_CV3HARM_020A0303_00_LEN         2
#define VAR_CV4HARM_020A0304_00_ADDR        (VAR_CV3HARM_020A0303_00_ADDR + 1)  //C���ѹ4��г������(Harmonic)
#define VAR_CV4HARM_020A0304_00_LEN         2
#define VAR_CV5HARM_020A0305_00_ADDR        (VAR_CV4HARM_020A0304_00_ADDR + 1)  //C���ѹ5��г������(Harmonic)
#define VAR_CV5HARM_020A0305_00_LEN         2
#define VAR_CV6HARM_020A0306_00_ADDR        (VAR_CV5HARM_020A0305_00_ADDR + 1)  //C���ѹ6��г������(Harmonic)
#define VAR_CV6HARM_020A0306_00_LEN         2
#define VAR_CV7HARM_020A0307_00_ADDR        (VAR_CV6HARM_020A0306_00_ADDR + 1)  //C���ѹ7��г������(Harmonic)
#define VAR_CV7HARM_020A0307_00_LEN         2
#define VAR_CV8HARM_020A0308_00_ADDR        (VAR_CV7HARM_020A0307_00_ADDR + 1)  //C���ѹ8��г������(Harmonic)
#define VAR_CV8HARM_020A0308_00_LEN         2
#define VAR_CV9HARM_020A0309_00_ADDR        (VAR_CV8HARM_020A0308_00_ADDR + 1)  //C���ѹ9��г������(Harmonic)
#define VAR_CV9HARM_020A0309_00_LEN         2
#define VAR_CV10HARM_020A030A_00_ADDR       (VAR_CV9HARM_020A0309_00_ADDR + 1)  //C���ѹ10��г������(Harmonic)
#define VAR_CV10HARM_020A030A_00_LEN        2
#define VAR_CV11HARM_020A030B_00_ADDR       (VAR_CV10HARM_020A030A_00_ADDR + 1) //C���ѹ11��г������(Harmonic)
#define VAR_CV11HARM_020A030B_00_LEN        2
#define VAR_CV12HARM_020A030C_00_ADDR       (VAR_CV11HARM_020A030B_00_ADDR + 1) //C���ѹ12��г������(Harmonic)
#define VAR_CV12HARM_020A030C_00_LEN        2
#define VAR_CV13HARM_020A030D_00_ADDR       (VAR_CV12HARM_020A030C_00_ADDR + 1) //C���ѹ13��г������(Harmonic)
#define VAR_CV13HARM_020A030D_00_LEN        2
#define VAR_CV14HARM_020A030E_00_ADDR       (VAR_CV13HARM_020A030D_00_ADDR + 1) //C���ѹ14��г������(Harmonic)
#define VAR_CV14HARM_020A030E_00_LEN        2
#define VAR_CV15HARM_020A030F_00_ADDR       (VAR_CV14HARM_020A030E_00_ADDR + 1) //C���ѹ15��г������(Harmonic)
#define VAR_CV15HARM_020A030F_00_LEN        2
#define VAR_CV16HARM_020A0310_00_ADDR       (VAR_CV15HARM_020A030F_00_ADDR + 1) //C���ѹ16��г������(Harmonic)
#define VAR_CV16HARM_020A0310_00_LEN        2
#define VAR_CV17HARM_020A0311_00_ADDR       (VAR_CV16HARM_020A0310_00_ADDR + 1) //C���ѹ17��г������(Harmonic)
#define VAR_CV17HARM_020A0311_00_LEN        2
#define VAR_CV18HARM_020A0312_00_ADDR       (VAR_CV17HARM_020A0311_00_ADDR + 1) //C���ѹ18��г������(Harmonic)
#define VAR_CV18HARM_020A0312_00_LEN        2
#define VAR_CV19HARM_020A0313_00_ADDR       (VAR_CV18HARM_020A0312_00_ADDR + 1) //C���ѹ19��г������(Harmonic)
#define VAR_CV19HARM_020A0313_00_LEN        2
#define VAR_CV20HARM_020A0314_00_ADDR       (VAR_CV19HARM_020A0313_00_ADDR + 1) //C���ѹ20��г������(Harmonic)
#define VAR_CV20HARM_020A0314_00_LEN        2
#define VAR_CV21HARM_020A0315_00_ADDR       (VAR_CV20HARM_020A0314_00_ADDR + 1) //C���ѹ21��г������(Harmonic)
#define VAR_CV21HARM_020A0315_00_LEN        2

#define VAR_AI1HARM_020B0101_00_ADDR        (VAR_CV21HARM_020A0315_00_ADDR + 1) //A�����1��г������(Harmonic)
#define VAR_AI1HARM_020B0101_00_LEN         2
#define VAR_AI2HARM_020B0102_00_ADDR        (VAR_AI1HARM_020B0101_00_ADDR + 1)  //A�����2��г������(Harmonic)
#define VAR_AI2HARM_020B0102_00_LEN         2
#define VAR_AI3HARM_020B0103_00_ADDR        (VAR_AI2HARM_020B0102_00_ADDR + 1)  //A�����3��г������(Harmonic)
#define VAR_AI3HARM_020B0103_00_LEN         2
#define VAR_AI4HARM_020B0104_00_ADDR        (VAR_AI3HARM_020B0103_00_ADDR + 1)  //A�����4��г������(Harmonic)
#define VAR_AI4HARM_020B0104_00_LEN         2
#define VAR_AI5HARM_020B0105_00_ADDR        (VAR_AI4HARM_020B0104_00_ADDR + 1)  //A�����5��г������(Harmonic)
#define VAR_AI5HARM_020B0105_00_LEN         2
#define VAR_AI6HARM_020B0106_00_ADDR        (VAR_AI5HARM_020B0105_00_ADDR + 1)  //A�����6��г������(Harmonic)
#define VAR_AI6HARM_020B0106_00_LEN         2
#define VAR_AI7HARM_020B0107_00_ADDR        (VAR_AI6HARM_020B0106_00_ADDR + 1)  //A�����7��г������(Harmonic)
#define VAR_AI7HARM_020B0107_00_LEN         2
#define VAR_AI8HARM_020B0108_00_ADDR        (VAR_AI7HARM_020B0107_00_ADDR + 1)  //A�����8��г������(Harmonic)
#define VAR_AI8HARM_020B0108_00_LEN         2
#define VAR_AI9HARM_020B0109_00_ADDR        (VAR_AI8HARM_020B0108_00_ADDR + 1)  //A�����9��г������(Harmonic)
#define VAR_AI9HARM_020B0109_00_LEN         2
#define VAR_AI10HARM_020B010A_00_ADDR       (VAR_AI9HARM_020B0109_00_ADDR + 1)  //A�����10��г������(Harmonic)
#define VAR_AI10HARM_020B010A_00_LEN        2
#define VAR_AI11HARM_020B010B_00_ADDR       (VAR_AI10HARM_020B010A_00_ADDR + 1) //A�����11��г������(Harmonic)
#define VAR_AI11HARM_020B010B_00_LEN        2
#define VAR_AI12HARM_020B010C_00_ADDR       (VAR_AI11HARM_020B010B_00_ADDR + 1) //A�����12��г������(Harmonic)
#define VAR_AI12HARM_020B010C_00_LEN        2
#define VAR_AI13HARM_020B010D_00_ADDR       (VAR_AI12HARM_020B010C_00_ADDR + 1) //A�����13��г������(Harmonic)
#define VAR_AI13HARM_020B010D_00_LEN        2
#define VAR_AI14HARM_020B010E_00_ADDR       (VAR_AI13HARM_020B010D_00_ADDR + 1) //A�����14��г������(Harmonic)
#define VAR_AI14HARM_020B010E_00_LEN        2
#define VAR_AI15HARM_020B010F_00_ADDR       (VAR_AI14HARM_020B010E_00_ADDR + 1) //A�����15��г������(Harmonic)
#define VAR_AI15HARM_020B010F_00_LEN        2
#define VAR_AI16HARM_020B0110_00_ADDR       (VAR_AI15HARM_020B010F_00_ADDR + 1) //A�����16��г������(Harmonic)
#define VAR_AI16HARM_020B0110_00_LEN        2
#define VAR_AI17HARM_020B0111_00_ADDR       (VAR_AI16HARM_020B0110_00_ADDR + 1) //A�����17��г������(Harmonic)
#define VAR_AI17HARM_020B0111_00_LEN        2
#define VAR_AI18HARM_020B0112_00_ADDR       (VAR_AI17HARM_020B0111_00_ADDR + 1) //A�����18��г������(Harmonic)
#define VAR_AI18HARM_020B0112_00_LEN        2
#define VAR_AI19HARM_020B0113_00_ADDR       (VAR_AI18HARM_020B0112_00_ADDR + 1) //A�����19��г������(Harmonic)
#define VAR_AI19HARM_020B0113_00_LEN        2
#define VAR_AI20HARM_020B0114_00_ADDR       (VAR_AI19HARM_020B0113_00_ADDR + 1) //A�����20��г������(Harmonic)
#define VAR_AI20HARM_020B0114_00_LEN        2
#define VAR_AI21HARM_020B0115_00_ADDR       (VAR_AI20HARM_020B0114_00_ADDR + 1) //A�����21��г������(Harmonic)
#define VAR_AI21HARM_020B0115_00_LEN        2

#define VAR_BI1HARM_020B0201_00_ADDR        (VAR_AI21HARM_020B0115_00_ADDR + 1) //B�����1��г������(Harmonic)
#define VAR_BI1HARM_020B0201_00_LEN         2
#define VAR_BI2HARM_020B0202_00_ADDR        (VAR_BI1HARM_020B0201_00_ADDR + 1)  //B�����2��г������(Harmonic)
#define VAR_BI2HARM_020B0202_00_LEN         2
#define VAR_BI3HARM_020B0203_00_ADDR        (VAR_BI2HARM_020B0202_00_ADDR + 1)  //B�����3��г������(Harmonic)
#define VAR_BI3HARM_020B0203_00_LEN         2
#define VAR_BI4HARM_020B0204_00_ADDR        (VAR_BI3HARM_020B0203_00_ADDR + 1)  //B�����4��г������(Harmonic)
#define VAR_BI4HARM_020B0204_00_LEN         2
#define VAR_BI5HARM_020B0205_00_ADDR        (VAR_BI4HARM_020B0204_00_ADDR + 1)  //B�����5��г������(Harmonic)
#define VAR_BI5HARM_020B0205_00_LEN         2
#define VAR_BI6HARM_020B0206_00_ADDR        (VAR_BI5HARM_020B0205_00_ADDR + 1)  //B�����6��г������(Harmonic)
#define VAR_BI6HARM_020B0206_00_LEN         2
#define VAR_BI7HARM_020B0207_00_ADDR        (VAR_BI6HARM_020B0206_00_ADDR + 1)  //B�����7��г������(Harmonic)
#define VAR_BI7HARM_020B0207_00_LEN         2
#define VAR_BI8HARM_020B0208_00_ADDR        (VAR_BI7HARM_020B0207_00_ADDR + 1)  //B�����8��г������(Harmonic)
#define VAR_BI8HARM_020B0208_00_LEN         2
#define VAR_BI9HARM_020B0209_00_ADDR        (VAR_BI8HARM_020B0208_00_ADDR + 1)  //B�����9��г������(Harmonic)
#define VAR_BI9HARM_020B0209_00_LEN         2
#define VAR_BI10HARM_020B020A_00_ADDR       (VAR_BI9HARM_020B0209_00_ADDR + 1)  //B�����10��г������(Harmonic)
#define VAR_BI10HARM_020B020A_00_LEN        2
#define VAR_BI11HARM_020B020B_00_ADDR       (VAR_BI10HARM_020B020A_00_ADDR + 1) //B�����11��г������(Harmonic)
#define VAR_BI11HARM_020B020B_00_LEN        2
#define VAR_BI12HARM_020B020C_00_ADDR       (VAR_BI11HARM_020B020B_00_ADDR + 1) //B�����12��г������(Harmonic)
#define VAR_BI12HARM_020B020C_00_LEN        2
#define VAR_BI13HARM_020B020D_00_ADDR       (VAR_BI12HARM_020B020C_00_ADDR + 1) //B�����13��г������(Harmonic)
#define VAR_BI13HARM_020B020D_00_LEN        2
#define VAR_BI14HARM_020B020E_00_ADDR       (VAR_BI13HARM_020B020D_00_ADDR + 1) //B�����14��г������(Harmonic)
#define VAR_BI14HARM_020B020E_00_LEN        2
#define VAR_BI15HARM_020B020F_00_ADDR       (VAR_BI14HARM_020B020E_00_ADDR + 1) //B�����15��г������(Harmonic)
#define VAR_BI15HARM_020B020F_00_LEN        2
#define VAR_BI16HARM_020B0210_00_ADDR       (VAR_BI15HARM_020B020F_00_ADDR + 1) //B�����16��г������(Harmonic)
#define VAR_BI16HARM_020B0210_00_LEN        2
#define VAR_BI17HARM_020B0211_00_ADDR       (VAR_BI16HARM_020B0210_00_ADDR + 1) //B�����17��г������(Harmonic)
#define VAR_BI17HARM_020B0211_00_LEN        2
#define VAR_BI18HARM_020B0212_00_ADDR       (VAR_BI17HARM_020B0211_00_ADDR + 1) //B�����18��г������(Harmonic)
#define VAR_BI18HARM_020B0212_00_LEN        2
#define VAR_BI19HARM_020B0213_00_ADDR       (VAR_BI18HARM_020B0212_00_ADDR + 1) //B�����19��г������(Harmonic)
#define VAR_BI19HARM_020B0213_00_LEN        2
#define VAR_BI20HARM_020B0214_00_ADDR       (VAR_BI19HARM_020B0213_00_ADDR + 1) //B�����20��г������(Harmonic)
#define VAR_BI20HARM_020B0214_00_LEN        2
#define VAR_BI21HARM_020B0215_00_ADDR       (VAR_BI20HARM_020B0214_00_ADDR + 1) //B�����21��г������(Harmonic)
#define VAR_BI21HARM_020B0215_00_LEN        2

#define VAR_CI1HARM_020B0301_00_ADDR        (VAR_BI21HARM_020B0215_00_ADDR + 1) //C�����1��г������(Harmonic)
#define VAR_CI1HARM_020B0301_00_LEN         2
#define VAR_CI2HARM_020B0302_00_ADDR        (VAR_CI1HARM_020B0301_00_ADDR + 1)  //C�����2��г������(Harmonic)
#define VAR_CI2HARM_020B0302_00_LEN         2
#define VAR_CI3HARM_020B0303_00_ADDR        (VAR_CI2HARM_020B0302_00_ADDR + 1)  //C�����3��г������(Harmonic)
#define VAR_CI3HARM_020B0303_00_LEN         2
#define VAR_CI4HARM_020B0304_00_ADDR        (VAR_CI3HARM_020B0303_00_ADDR + 1)  //C�����4��г������(Harmonic)
#define VAR_CI4HARM_020B0304_00_LEN         2
#define VAR_CI5HARM_020B0305_00_ADDR        (VAR_CI4HARM_020B0304_00_ADDR + 1)  //C�����5��г������(Harmonic)
#define VAR_CI5HARM_020B0305_00_LEN         2
#define VAR_CI6HARM_020B0306_00_ADDR        (VAR_CI5HARM_020B0305_00_ADDR + 1)  //C�����6��г������(Harmonic)
#define VAR_CI6HARM_020B0306_00_LEN         2
#define VAR_CI7HARM_020B0307_00_ADDR        (VAR_CI6HARM_020B0306_00_ADDR + 1)  //C�����7��г������(Harmonic)
#define VAR_CI7HARM_020B0307_00_LEN         2
#define VAR_CI8HARM_020B0308_00_ADDR        (VAR_CI7HARM_020B0307_00_ADDR + 1)  //C�����8��г������(Harmonic)
#define VAR_CI8HARM_020B0308_00_LEN         2
#define VAR_CI9HARM_020B0309_00_ADDR        (VAR_CI8HARM_020B0308_00_ADDR + 1)  //C�����9��г������(Harmonic)
#define VAR_CI9HARM_020B0309_00_LEN         2
#define VAR_CI10HARM_020B030A_00_ADDR       (VAR_CI9HARM_020B0309_00_ADDR + 1)  //C�����10��г������(Harmonic)
#define VAR_CI10HARM_020B030A_00_LEN        2
#define VAR_CI11HARM_020B030B_00_ADDR       (VAR_CI10HARM_020B030A_00_ADDR + 1) //C�����11��г������(Harmonic)
#define VAR_CI11HARM_020B030B_00_LEN        2
#define VAR_CI12HARM_020B030C_00_ADDR       (VAR_CI11HARM_020B030B_00_ADDR + 1) //C�����12��г������(Harmonic)
#define VAR_CI12HARM_020B030C_00_LEN        2
#define VAR_CI13HARM_020B030D_00_ADDR       (VAR_CI12HARM_020B030C_00_ADDR + 1) //C�����13��г������(Harmonic)
#define VAR_CI13HARM_020B030D_00_LEN        2
#define VAR_CI14HARM_020B030E_00_ADDR       (VAR_CI13HARM_020B030D_00_ADDR + 1) //C�����14��г������(Harmonic)
#define VAR_CI14HARM_020B030E_00_LEN        2
#define VAR_CI15HARM_020B030F_00_ADDR       (VAR_CI14HARM_020B030E_00_ADDR + 1) //C�����15��г������(Harmonic)
#define VAR_CI15HARM_020B030F_00_LEN        2
#define VAR_CI16HARM_020B0310_00_ADDR       (VAR_CI15HARM_020B030F_00_ADDR + 1) //C�����16��г������(Harmonic)
#define VAR_CI16HARM_020B0310_00_LEN        2
#define VAR_CI17HARM_020B0311_00_ADDR       (VAR_CI16HARM_020B0310_00_ADDR + 1) //C�����17��г������(Harmonic)
#define VAR_CI17HARM_020B0311_00_LEN        2
#define VAR_CI18HARM_020B0312_00_ADDR       (VAR_CI17HARM_020B0311_00_ADDR + 1) //C�����18��г������(Harmonic)
#define VAR_CI18HARM_020B0312_00_LEN        2
#define VAR_CI19HARM_020B0313_00_ADDR       (VAR_CI18HARM_020B0312_00_ADDR + 1) //C�����19��г������(Harmonic)
#define VAR_CI19HARM_020B0313_00_LEN        2
#define VAR_CI20HARM_020B0314_00_ADDR       (VAR_CI19HARM_020B0313_00_ADDR + 1) //C�����20��г������(Harmonic)
#define VAR_CI20HARM_020B0314_00_LEN        2
#define VAR_CI21HARM_020B0315_00_ADDR       (VAR_CI20HARM_020B0314_00_ADDR + 1) //C�����21��г������(Harmonic)
#define VAR_CI21HARM_020B0315_00_LEN        2
#define VAR_IN_02800001_00_ADDR             (VAR_CI21HARM_020B0315_00_ADDR + 1) //���ߵ���

#define VAR_IN_02800001_00_LEN              3
#define VAR_FREQ_02800002_00_ADDR           (VAR_IN_02800001_00_ADDR + 1)       //����Ƶ��
#define VAR_FREQ_02800002_00_LEN            2
#define VAR_AVEPMIN_02800003_00_ADDR        (VAR_FREQ_02800002_00_ADDR + 1)     //һ�����й���ƽ������
#define VAR_AVEPMIN_02800003_00_LEN         3
#define VAR_DMDP_02800004_00_ADDR           (VAR_AVEPMIN_02800003_00_ADDR + 1)  //��ǰ�й�����
#define VAR_DMDP_02800004_00_LEN            3
#define VAR_DMDQ_02800005_00_ADDR           (VAR_DMDP_02800004_00_ADDR + 1)     //��ǰ�޹�����
#define VAR_DMDQ_02800005_00_LEN            3
#define VAR_DMDS_02800006_00_ADDR           (VAR_DMDQ_02800005_00_ADDR + 1)     //��ǰ��������
#define VAR_DMDS_02800006_00_LEN            3
#define VAR_TEMPIN_02800007_00_ADDR         (VAR_DMDS_02800006_00_ADDR + 1)     //�����¶�
#define VAR_TEMPIN_02800007_00_LEN          2
#define VAR_VRTCBAT_02800008_00_ADDR        (VAR_TEMPIN_02800007_00_ADDR + 1)   //ʱ�ӵ�ص�ѹ(�ڲ�)
#define VAR_VRTCBAT_02800008_00_LEN         2
#define VAR_VSYSBAT_02800009_00_ADDR        (VAR_VRTCBAT_02800008_00_ADDR + 1)  //ͣ�糭���ص�ѹ (�ⲿ)
#define VAR_VSYSBAT_02800009_00_LEN         2
#define VAR_TRTCBAT_0280000A_00_ADDR        (VAR_VSYSBAT_02800009_00_ADDR + 1)  //�ڲ���ع���ʱ��
#define VAR_TRTCBAT_0280000A_00_LEN         4

#define VAR_RAM_ARRAY_MAX                   (VAR_TRTCBAT_0280000A_00_ADDR - INTERNAL_RAM_HEAD_ADDR + 1 + 18)  //�����±��С
#define PAR_BASE_INDEX                      (VAR_RAM_ARRAY_MAX - 18)            //�����������α����������±�
//����18���������̶����[(����&�ܴα�ʶ������),(ʱ���ʶ������),(�������״̬��1��ʶ������),(����),(�������״̬��7��ʶ������)]

#endif//__VAR_DEFINE_H

/******************************************************************************
    End Of File
******************************************************************************/

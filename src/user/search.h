/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 Module         : search.cͷ�ļ�
 File Name      : search.h
 Description    :
 Author         :
 Build Date     :
 others         :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��     1.00       2011-08-15     azh            create
******************************************************************************/

#ifndef __SEARCH_H
#define __SEARCH_H

#ifdef ZIGBEE_TEST//azh 171011//CODE_VER_TYPE!=GPRS_MODULE_PRJ		//���Ǵ�CPU_GPRSģ��
/*----------------------------- macro definition ----------------------------*/
//-----------------------------------------------------------------------------
// ����������״̬��һЩ�궨��
//-----------------------------------------------------------------------------
#define CON_SEARCH_IDLE_STATE      0//����״̬
#define CON_SEARCH_RUN_STATE       1//������������״̬
#define CON_SEARCH_DISPLY_STATE    2//���������ʾ״̬
#define CON_SEARCH_FINISH_STATE    3//����,����������

#define CON_SEARCH_CMD_MAX          16
#define CON_SEARCH_ADDR_LEN         (6+8+2) //addr LEN(6)+ zigbee adr(8) + CRC(2)
#define DIS_SEARCH_JUDGE_TIME       20      //100ms unit 3S   ͬʱ��ס���·�ҳ����ʱ
#define DIS_SEARCH_INTERVAL_TIME    10      //100ms unit 3S    ���������ʱ��
#define DIS_SEARCH_KEY_TIME         3       //100ms unit 3S    ����ȷ��ʱ����ʱ
#define DIS_SEARCH_DIS_TIME         20       //100ms unit 3S    ��ʾ���״̬
/*----------------------------- type definition -----------------------------*/
typedef struct _DIS_SEARCH_CTRL
{
    U8  state;          //״̬��1�ѱ�״̬ 2���������ʾ״̬ 3����,����������
    U8  dis_sn;         //�����������ʾ���
    U8  totnum;         //�������������
    U8  cmd_ct;         //�ѱ������������
    U8  state_dly;      //����״̬��ʱ100ms unit
    U8  pre_key;        //���ڰ����ж���
    U8  addr[CON_SEARCH_CMD_MAX][CON_SEARCH_ADDR_LEN];      //����ͬʱ����8�����ַ������ʾ
}DIS_SEARCH_CTRL;

/*-------------------------- functions declaration --------------------------*/
__EXTERN    U8 analyse_identity_frame(U8 *in_ptr);
__EXTERN    void send_match_module_frame(void);
__EXTERN    U8 check_match_valid(void);
__EXTERN    void init_search_data(void);
__EXTERN    void set_search_data(void);
__EXTERN    void store_match_addr(U8 index);
__EXTERN    void handle_search_state(void);
__EXTERN void judge_search_state(void);
__EXTERN void send_identity_frame(void);
__EXTERN void store_zigbee_addr(U8 *addr);
__EXTERN void store_hvmeter_addr(U8 *addr);
__EXTERN void get_match_addr(void);
/*--------------------------- variable declaration --------------------------*/
__EXTERN DIS_SEARCH_CTRL Dis_Search_Ctrl;
__EXTERN U16 GangMarkTime;                              //�ӵ�/? �ȴ�OK�ڼ�Ķ�ʱ
#endif //CODE_VER_TYPE!=GPRS_MODULE_PRJ		//���Ǵ�CPU_GPRSģ��

#endif//__SEARCH_H
/******************************************************************************
                            End Of File
 *****************************************************************************/

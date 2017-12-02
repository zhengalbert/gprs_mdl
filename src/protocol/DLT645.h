/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : DLT645.H
 Description    : ����DLT645.C ʵ�ֹ�Լͨ��
-------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��    1.00        2010-09-01    LJL          the original version
******************************************************************************/
#ifndef _DLT645_H
#define _DLT645_H

#define FRAME_HEAD        0x68
#define ACCOUNT_TOTAL_PHASE_ENERGY_LEN      (RATE_NUM + 1) * ENERGY_LEN
#define MAX_DATA_LEN      180
#define AFTER_FRAM_NUM    3
#define PRG_EN_DLY_TIME   DLY_1MIN
#define SUPER_PASSWORD    0x88690188ul

//----------------------------- type definition -------------------------------
typedef struct
{
    U8 State;                           //��ǰ״̬����Ч=1����Ч =0
    U8 Id[4];                           //��ʶ�룬����ΪID0��ID1��ID2��ID3
    U8 Current_Seq;                     //��ǰ����֡֡�� �������0С��Total_Seq
    U16 Spare_Len;                      //ʣ��֡�ĳ���
    U32 Address;                        //��ǰ֡����Ҫ�ĵ�ַ
    U32 Tick;                           //ϵͳTick�Ļ���
    U8 State_Word_Bit;                  //ֻ�ڶ���֡�ж� 1��11����Ӧfreeze_state_table,11��ʾ�������
}AFTER_FRAME_STRUCT;

typedef enum
{
    NO_RECORD,                          //�ޱ�̼�¼
    OTHER_RECORD,                       //������̼�¼
    ZONE_RECORD,                        //ʱ������
    TIME_RECORD,                        //ʱ�α���
    HOLIDAY_RECORD,                     //�ڼ��ձ��
}PRG_RECORD_STATE;                      //��̼�¼״̬
//-------------------------- functions declaration ----------------------------
__EXTERN U8 cal_frmcs(U8 *buf, U16 len);
__EXTERN S8 deal_with_DLT645frame(U8 *InBuf);
__EXTERN S8 deal_with_irdaframe(U8 index, U8 com, U8 *rcv_buff);
__EXTERN void short_link_check(void);
__EXTERN void check_online(void);

//--------------------------- variable declaration ----------------------------
__EXTERN AFTER_FRAME_STRUCT After_Frame[AFTER_FRAM_NUM];//ͨѶ�õ��ڴ�鶨�壬������ú���alloc_comm_mem()���룬�������ж�������
//__EXTERN U32 deal_with_dis_upgrade_frame(PSTMDMSG pstMdMsg);
#endif  // _DLT645_H






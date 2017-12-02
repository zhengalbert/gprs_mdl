/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : DLT645.H
 Description    : 配套DLT645.C 实现规约通信
-------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2010-09-01    LJL          the original version
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
    U8 State;                           //当前状态，有效=1，无效 =0
    U8 Id[4];                           //标识码，依次为ID0、ID1、ID2、ID3
    U8 Current_Seq;                     //当前后续帧帧号 必须大于0小于Total_Seq
    U16 Spare_Len;                      //剩余帧的长度
    U32 Address;                        //当前帧号需要的地址
    U32 Tick;                           //系统Tick的缓存
    U8 State_Word_Bit;                  //只在冻结帧判断 1到11，对应freeze_state_table,11表示冻结变量
}AFTER_FRAME_STRUCT;

typedef enum
{
    NO_RECORD,                          //无编程记录
    OTHER_RECORD,                       //其他编程记录
    ZONE_RECORD,                        //时区表编程
    TIME_RECORD,                        //时段表编程
    HOLIDAY_RECORD,                     //节假日编程
}PRG_RECORD_STATE;                      //编程记录状态
//-------------------------- functions declaration ----------------------------
__EXTERN U8 cal_frmcs(U8 *buf, U16 len);
__EXTERN S8 deal_with_DLT645frame(U8 *InBuf);
__EXTERN S8 deal_with_irdaframe(U8 index, U8 com, U8 *rcv_buff);
__EXTERN void short_link_check(void);
__EXTERN void check_online(void);

//--------------------------- variable declaration ----------------------------
__EXTERN AFTER_FRAME_STRUCT After_Frame[AFTER_FRAM_NUM];//通讯用的内存块定义，必须调用函数alloc_comm_mem()申请，可以在中断中申请
//__EXTERN U32 deal_with_dis_upgrade_frame(PSTMDMSG pstMdMsg);
#endif  // _DLT645_H






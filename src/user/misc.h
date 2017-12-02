/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : misc.h
 Description    : 杂散函数的头文件
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   3、    2.00        2011-02-25     rosoon          合并使A&C共享
   2、    1.01        2010-09-07      snap           增加check_event_time_valid()函数避免与check_bcd_time_valid()函数冲突
   1、    1.00        2010-07-22      snap           the original version
******************************************************************************/

#ifndef __MISC_H
#define __MISC_H

//----------------------------- macro definition ------------------------------
#define GW645_2007_CHAR 0                               //目前645-2007要求的填充字符, 可移到对应协议文件中
//azh 110516
#define INVALID_TYPE                0                   // 无效内存块类型
#define VALID_INSIDE_SRAM           0x10000000          // 有效内部SRAM的开始地址
#define VALID_INSIDE_SRAM1          0x20000000          // 有效内部SRAM的开始地址 //azh

typedef enum
{
    GET_MEM_ERR0,
    GET_MEM_ERR1,
    GET_MEM_ERR2,
    GET_MEM_ERR3,
    GET_MEM_ERR4,
    GET_MEM_ERR5,
    GET_MEM_ERR6,
    GET_MEM_ERR7,    
    GET_MEM_ERR8,
    GET_MEM_ERR9,
    GET_MEM_ERR10,
    GET_MEM_ERR11,    
    GET_MEM_ERR12,
    GET_MEM_ERR13,
    GET_MEM_ERR14,
    GET_MEM_ERR15,    

    GET_MEM_ERR16,
    GET_MEM_ERR17,
    GET_MEM_ERR18,
    GET_MEM_ERR19,
    GET_MEM_ERR20,
    GET_MEM_ERR21,
    GET_MEM_ERR22,
    GET_MEM_ERR23,    
    GET_MEM_ERR24,
    GET_MEM_ERR25,
    GET_MEM_ERR26,//
    GET_MEM_ERR27,    
    GET_MEM_ERR28,
    GET_MEM_ERR29,
    GET_MEM_ERR30,
    GET_MEM_ERR31,    
}CON_GET_MEM_ERR;
//----------------------------- type definition -------------------------------
typedef struct{
    U32 head_addr;      //首地址
    U32 offset_addr;    //指针存放地址
    U16 block_len;      //每个数据块长度
    U8  max_num;        //最大条数
    U8  offset_len;     //指针保存长度
} ADDR_OFFSET_TCB;
//--------------------------- variable declaration ----------------------------
__EXTERN U32 gusMemErrId;
__EXTERN U8 gusCurMemErrFunName[8];
//__EXTERN U8 gusMemErrFunName[32][8];
//-------------------------- functions declaration ----------------------------
__EXTERN U8 next_event_record_point(U8 point, U8 total);
__EXTERN U8 hex_to_bcd_in8(U8 data);
__EXTERN U8 bcd_to_hex(U8 data);
__EXTERN S8 decrease_event_record_point(U8 point, U8 total, U8 num);
__EXTERN U16 calc_crc16(U8 *ptr, U32 len, U16 init_value);
__EXTERN U16 day_of_year(U8 year, U8 month, U8 day);
__EXTERN U32 bytes_to_bcd(U32 data, U8 *p, U8 len);
__EXTERN U32 bcd_to_bytes(U8 *p, U8 len);
__EXTERN U32 next_load_profile_point(U32 point, U32 total);
__EXTERN U32 check_event_time_valid(U8 *time);
__EXTERN U32 check_bcd_time_valid(U8 *time);
__EXTERN U32 check_hex_time_valid(U8 *time);
__EXTERN U32 max(U32 x, U32 y);
__EXTERN U32 min(U32 x, U32 y);
__EXTERN U32 energy_to_format_data(U8 *p1, U8 *p2, U8 flag);
__EXTERN U32 byte_merge(U8 *dat_buf, U8 num);
__EXTERN S32 decrease_load_profile_point(U32 point, U32 total, U32 num);
__EXTERN S32 compare_time(U8 *p1, U8 *p2);
__EXTERN void get_event_record_time(U8 *time);
__EXTERN void delay(U32 time);

__EXTERN void fill_exmemory(U32 addr, U8 flg, U8 len);
__EXTERN void convert_to_bcd(U8 *p, U8 len);
__EXTERN void convert_to_hex(U8 *p, U8 len);
__EXTERN void fill_char(U8 buf[], U32 len, U8 flag);
__EXTERN void cpu_regs_watch(void);

//azh 110516
__EXTERN U8 addverify(U8 *pverdata, U16 len);
__EXTERN U8 calc_BCC (U8 *ptr, U16 len);
__EXTERN U8 check_mem_type (void *mem);
__EXTERN S32 free_mem_block(void *mem);
__EXTERN S32 get_addr_offset(U8 last_num, ADDR_OFFSET_TCB *pst_addr, U32 *addr);
__EXTERN void second_index_2_RTC(U32 res, RTC_STRUCT *time);
__EXTERN U32 RTC_2_second_index(RTC_STRUCT time);
__EXTERN U8 compare_update_buf(U8 *pd, U8 *ps, U32 cnt);
//__EXTERN void debug_task(void);
__EXTERN U8 asc_to_hex(U8 data);
__EXTERN U32 HEX_TO_BCD_INT32U(U32 lHex, U8 ucSize);
__EXTERN U32 power2(U8 num);
__EXTERN U32 byte_merge1(U8 *dat_buf, U8 num);
__EXTERN void buf_add_33(U8 buf[], U16 len);
//__EXTERN U8* alloc_comm_mem (U8 type, U8 *real);
__EXTERN U8* alloc_comm_mem (U8 type, U8 *real, U8 id);
__EXTERN void setmemgetnullid(U8 id, U8 *funname);
__EXTERN U8 AddVerifyOffset(U8 *pVerData, U16 uiLen, U8 ucVerConst);

//--------------------------- variable declaration ----------------------------

#endif//__MISC_H

/******************************************************************************
    End Of File
******************************************************************************/

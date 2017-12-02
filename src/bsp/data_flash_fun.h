/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : data_flash_fun.h
 Description    : DataFlash读写函数的头文件
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2010-09-15      snap           the original version
******************************************************************************/

#ifndef __DATA_FLASH_FUN_H
#define __DATA_FLASH_FUN_H

//----------------------------- macro definition ------------------------------
#define DATAFLASH_CAPACITY         (2 * 1024 * 1024)
#define DATAFLASH_PAGE_SIZE        528
        
#define DATAFLASH_RD               0x00
#define DATAFLASH_WR               0x01
        
#define DATAFLASH_ENABLE           0x00
#define DATAFLASH_DISABLE          0x01

//----------------------------- type definition -------------------------------

//-------------------------- functions declaration ----------------------------
__EXTERN S32 read_dataflash(U32 obj_addr, U8 *rcv_buff, U16 len);
__EXTERN S32 write_dataflash(U32 obj_addr, U8 *send_buff, U16 len);

//--------------------------- variable declaration ----------------------------

#endif//__DATA_FLASH_FUN_H

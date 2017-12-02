/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : mxflash.h
 Description    : MXFlash读写函数的头文件
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2010-08-26      snap           the original version
******************************************************************************/

#ifndef __MXFLASH_H
#define __MXFLASH_H

//----------------------------- macro definition ------------------------------
#define MXFLASH1_HEAD_ADDR           0x72000000              //定义MXFLash的逻辑起始地址
#define MXFLASH2_HEAD_ADDR           0x73000000              //定义MXFLash的逻辑起始地址
//#define MXFLASH_CAPACITY            (16 * 1024 * 1024)      //定义MXFlash的容量16MBytes
#define MXFLASH_CAPACITY            (8 * 1024 * 1024)      //定义MXFlash的容量8MBytes
                               
#define MXFLASH_PAGE_SIZE           256                     //一页大小
#define MXFLASH_SECTOR_SIZE         (4 * 1024)              //一个扇区大小
                               
#define MXFLASH_RD                  0x00
#define MXFLASH_WR                  0x01
                               
#define MXFLASH_ENABLE              0x00
#define MXFLASH_DISABLE             0x01

//----------------------------- type definition -------------------------------
#define TEST_BUG_ADDR              MXFLASH2_HEAD_ADDR
#define TEST_BUG_LEN				6


//-------------------------- functions declaration ----------------------------
__EXTERN S32 read_mxflash(U32 obj_addr, U8 *rcv_buff, U16 len);
__EXTERN S32 write_mxflash(U32 obj_addr, U8 *send_buff, U16 len);
__EXTERN S32 mxflash_sector_erase(U32 obj_addr);

//--------------------------- variable declaration ----------------------------
__EXTERN    OS_MUT      Mut_SSP_EON_Flash;//azh 140918

#endif//__MXFLASH_H

/******************************************************************************
    End Of File
******************************************************************************/

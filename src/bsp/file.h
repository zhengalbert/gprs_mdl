/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : file.h
 Description    : 全局的读写函数的头文件
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2010-07-25      snap           the original version
******************************************************************************/

#ifndef __FILE_H
#define __FILE_H

//----------------------------- macro definition ------------------------------
#define FLASH_HEAD_ADDR         0x00000000          //定义处理器内部Flash的（物理）起始地址
#define FLASH_CAPACITY          (256 * 1024)        //定义内部Flash的容量256KBytes
                                
#define IRAM1_HEAD_ADDR         0x10000000          //定义处理器内部本地RAM的（物理）起始地址
#define IRAM1_CAPACITY          (32 * 1024)         //定义内部本地RAM的容量32KBytes
                                
#define IRAM2_HEAD_ADDR         0x2007c000          //定义处理器内部AHB RAM的（物理）起始地址
#define IRAM2_CAPACITY          (32 * 1024)         //定义内部AHB RAM的容量32KBytes
                                
#define GPIO_HEAD_ADDR          0x2009c000          //定义处理器内部GPIO寄存器的（物理）起始地址
#define GPIO_CAPACITY           (16 * 1024)         //定义内部GPIO寄存器的容量16KBytes
                                
#define APB_HEAD_ADDR           0x40000000          //定义处理器内部APB外设寄存器的（物理）起始地址
#define APB_CAPACITY            (1024 * 1024)       //定义内部APB外设寄存器的容量1MBytes
                                
#define AHB_HEAD_ADDR           0x50000000          //定义处理器内部AHB外设寄存器的（物理）起始地址
#define AHB_CAPACITY            (2048 * 1024)       //定义内部AHB外设寄存器的容量2MBytes
                                
#define KERNEL_HEAD_ADDR        0xe0000000          //定义处理器Cortex-M3相关寄存器的（物理）起始地址
#define KERNEL_CAPACITY         (1024 * 1024)       //定义Cortex-M3相关寄存器的容量1MBytes

//-----------------------------------------------------------------------------
// 定义存储器读写时发生的错误的宏定义
//-----------------------------------------------------------------------------
#define NO_ERROR                1           //正常
#define LEN_OVF                -1           //数据长度错误
#define ADDR_ERROR             -2           //器件地址错误
#define TIME_OVF               -3           //等待超时
#define STATE_ERR1             -4           //I2C总线状态错误
#define STATE_ERR2             -5           //I2C总线状态错误
#define STATE_ERR3             -6           //I2C总线状态错误
#define STATE_ERR4             -7           //I2C总线状态错误
#define STATE_ERR5             -8           //I2C总线状态错误
#define STATE_ERR6             -9           //I2C总线状态错误
#define STATE_ERR7             -10          //I2C总线状态错误
#define STATE_ERR8             -11          //I2C总线状态错误
#define STATE_ERR9             -12          //I2C总线状态错误
#define DATA_OVF               -13          //SSP数据接收溢出
#define WREN_LATCH             -14          //写入使能错误
#define DATA_MISMATCH          -15          //写入的数据与原始数据不匹配

//----------------------------- type definition -------------------------------

//-------------------------- functions declaration ----------------------------
__EXTERN S32 file_read (U32 obj_addr, U8 *rcv_buff, U16 len);
__EXTERN S32 file_write (U32 obj_addr, U8 *snt_buff, U16 len);

__EXTERN S32 MCP_Flash_Write(U32 obj_addr, U8 *rcv_buff, U16 len);
__EXTERN S32 MCP_Flash_Read(U32 obj_addr, U8 *rcv_buff, U16 len);
__EXTERN S32 MCP_Flash_Sector_Erase(U32 obj_addr);
//--------------------------- variable declaration ----------------------------

#endif//__FILE_H

/******************************************************************************
    End Of File
******************************************************************************/

/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : file.c
 Description    : 全局的读写函数，包括了所有存储器和RTC
 Related Files  : dataflash.c 调用了读写函数
                  mxflash.c 调用了读写函数
                  fram.c 调用了读写函数
                  i2c1.c 调用了读写函数
                  mram.c 调用了读写函数
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   3、    2.00        2011-02-25     rosoon         合并使A&C通用
   2、    1.01        2010-12-30     rosoon           reduce
   1、    1.00        2010-07-25      snap       the original version
******************************************************************************/

#include "..\inc\global_config.h"

//-------------------------------- 常数定义 -----------------------------------

//------------------------------ 私有变量声明 ---------------------------------

//------------------------------ 私有函数声明 ---------------------------------
/******************************************************************************
** 函数名称: S32 MCP_Flash_Write (U32 obj_addr, U8 *rcv_buff, U16 len)
** 函数功能: Flash芯片写处理 EN25Q64有二片 而程序里的编址是从0地址开始并且可以连续
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
S32 MCP_Flash_Write (U32 obj_addr, U8 *rcv_buff, U16 len)
{
    S32 err; 
    
    if(obj_addr >= MXFLASH_CAPACITY)//8M bytes EN25Q64
    {
        obj_addr = obj_addr - MXFLASH_CAPACITY + MXFLASH2_HEAD_ADDR;
    }
    else
    {
        obj_addr += MXFLASH1_HEAD_ADDR;
    }       
    err = file_write(obj_addr,rcv_buff,len);
    return err;
}
/******************************************************************************
** 函数名称: S32 MCP_Flash_Read (U32 obj_addr, U8 *rcv_buff, U16 len)
** 函数功能: Flash芯片读处理 EN25Q64有二片 而程序里的编址是从0地址开始并且可以连续
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
S32 MCP_Flash_Read (U32 obj_addr, U8 *rcv_buff, U16 len)
{
    S32 err; 

    if(obj_addr >= MXFLASH_CAPACITY)//8M bytes EN25Q64
    {
        obj_addr = obj_addr - MXFLASH_CAPACITY + MXFLASH2_HEAD_ADDR;
    }
    else
    {
        obj_addr += MXFLASH1_HEAD_ADDR;
    }       
    err = file_read(obj_addr,rcv_buff,len);
    return err;
}
/******************************************************************************
** 函数名称: S32 MCP_Flash_Sector_Erase (U32 obj_addr)
** 函数功能: Flash芯片擦除处理 EN25Q64有二片 而程序里的编址是从0地址开始并且可以连续
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
S32 MCP_Flash_Sector_Erase (U32 obj_addr)
{
    S32 err;

    if(obj_addr >= MXFLASH_CAPACITY)//8M bytes EN25Q64
    {
        obj_addr = obj_addr - MXFLASH_CAPACITY + MXFLASH2_HEAD_ADDR;
    }
    else
    {
        obj_addr += MXFLASH1_HEAD_ADDR;
    }       
    err = mxflash_sector_erase(obj_addr);//azh
    return err;
}

/******************************************************************************
** 函数名称: S32 file_read (U32 obj_addr, U8 *rcv_buff, U16 len)
** 函数功能: Be used to read data from all the memories on the system
**           including the FRAM, EEPROM, Datafalsh, MRAM, MXflash, RTC etc.
** 入口参数: obj_addr : object logic adrress
**           *rcv_buff: pointer for the receive buffer
**           len      : length of bytes for operation
** 返 回 值: indicating the result of the operation
**              1: normal
**             -1: length overflow, not allowed
**             -2: object memory not found
**             -3: waiting time overflow
**        -others: state error
** 说    明: logic adrress assignment
**           1、FRAM        ：0x6000 0000 ~ 0x6000 7fff -  32KBytes
**           2、EEPROM      ：0x6100 0000 ~ 0x6100 ffff -  64KBytes
**           3、MRAM        ：0x6200 0000 ~ 0x6200 7fff -  32KBytes
**           4、RTC(RX8025) ：0x6300 0000 ~ 0x6300 000f -   16Bytes
**           4、RTC(DS3231) ：0x6400 0000 ~ 0x6400 0012 -   19Bytes
**           5、备用        ：0x6500 0000 ~ 0x6fff ffff - 176MBytes
**
**           6、DATAFLASH(1)；0x7000 0000 ~ 0x701f ffff -   2MBytes
**              DATAFLASH(2)；0x7100 0000 ~ 0x711f ffff -   2MBytes
**           7、MXFLASH     ：0x7200 0000 ~ 0x72ff ffff -  16MBytes
**
**           8、CODE        ：0x0000 0000 ~ 0x0007 ffff - 256KBytes 程序代码空间
**			 9、LCD_RAM		：0x8000 0000 ~ 0x8000 0022 - 35bytes//azh BU97950显示RAM
******************************************************************************/
S32 file_read (U32 obj_addr, U8 *rcv_buff, U16 len)
{
    U32 obj_memory;
    S32 err;

    obj_memory = (obj_addr & 0xFF000000);
    switch(obj_memory)
    {
    case FRAM_HEAD_ADDR:
    case EEPROM_HEAD_ADDR:
    case RX8025_HEAD_ADDR:		   //azh 110526
//    case DS3231_HEAD_ADDR:
        err = read_i2c1(obj_addr, rcv_buff, len);
        break;
//    case DATAFLASH1_HEAD_ADDR:
//    case DATAFLASH2_HEAD_ADDR:
//        err = read_dataflash(obj_addr, rcv_buff, len);
//        break;
//    case MRAM_HEAD_ADDR:	//azh 110526
//        err = read_mram(obj_addr, rcv_buff, len);
//        break;
    case MXFLASH1_HEAD_ADDR:
    case MXFLASH2_HEAD_ADDR:
        err = read_mxflash(obj_addr, rcv_buff, len);
        break;
    case IRAM1_HEAD_ADDR:
        if(((obj_addr & 0x00FFFFFF) + len) > IRAM1_CAPACITY)
        {
            err = LEN_OVF;
        }
        else
        {
            memcpy(rcv_buff, (U8*)obj_addr, len);
            err =  NO_ERROR;
        }
        break;
    case IRAM2_HEAD_ADDR:
        if(((obj_addr & 0x00FFFFFF) + len) > IRAM2_CAPACITY)
        {
            err = LEN_OVF;
        }
        else
        {
            memcpy(rcv_buff, (U8*)obj_addr, len);
            err =  NO_ERROR;
        }
        break;
    case FLASH_HEAD_ADDR:
        if(((obj_addr & 0x00FFFFFF) + len) > FLASH_CAPACITY)
        {
            err = LEN_OVF;
        }
        else
        {
            memcpy(rcv_buff, (U8*)obj_addr, len);
            err =  NO_ERROR;
        }
        break;
    case INTERNAL_RAM_HEAD_ADDR://芯片RAM全局数组下标
    {
    	if(((obj_addr & 0x0000FFFF) + len) > VAR_RAM_ARRAY_MAX)
    	{
    		err = LEN_OVF;
    	}
    	else
        {
            memcpy(rcv_buff, (U8*)&gVarArray[obj_addr&0x0000FFFF], len);
            err =  NO_ERROR;
        }    	
    }
    break;	
    default:
        if((obj_addr >= IRAM2_HEAD_ADDR)
        && (obj_addr < (IRAM2_HEAD_ADDR + IRAM2_CAPACITY)))     //判断地址是否在AHB RAM范围
        {
            if(((obj_addr + len) > IRAM2_HEAD_ADDR)
            && ((obj_addr + len) < (IRAM2_HEAD_ADDR + IRAM2_CAPACITY)))
            {
                memcpy(rcv_buff, (U8*)obj_addr, len);
                return NO_ERROR;
            }
            return LEN_OVF;
        }
        return ADDR_ERROR;
    }
    return err;
}

/******************************************************************************
** 函数名称: S32 file_write (U32 obj_addr, U8 *snt_buff, U16 len)
** 函数功能: Be used to write data to all the memories on the system
**           including the FRAM, EEPROM, Datafalsh, MRAM, MXflash, RTC etc.
** 入口参数: obj_addr : object logic adrress
**           *snt_buff: pointer for the send buffer
**           len      : length of bytes for operation
** 返 回 值: indicating the result of the operation
**              1: normal
**             -1: length overflow, not allowed
**             -2: object memory not found
**             -3: waiting time overflow
**        -others: state error
** 说    明: logic adrress assignment
**           1、FRAM        ：0x6000 0000 ~ 0x6000 7fff -  32KBytes
**           2、EEPROM      ：0x6100 0000 ~ 0x6100 ffff -  64KBytes
**           3、MRAM        ：0x6200 0000 ~ 0x6200 7fff -  32KBytes
**           4、RTC(RX8025) ：0x6300 0000 ~ 0x6300 000f -   16Bytes
**           4、RTC(DS3231) ：0x6400 0000 ~ 0x6400 0012 -   19Bytes
**           5、备用        ：0x6500 0000 ~ 0x6fff ffff - 176MBytes
**
**           6、DATAFLASH(1)；0x7000 0000 ~ 0x701f ffff -   2MBytes
**              DATAFLASH(2)；0x7100 0000 ~ 0x711f ffff -   2MBytes
**           7、MXFLASH     ：0x7200 0000 ~ 0x72ff ffff -  16MBytes
******************************************************************************/
S32 file_write (U32 obj_addr, U8 *snt_buff, U16 len)
{
    U32 obj_memory;
    S32 err;

    obj_memory = (obj_addr & 0xFF000000);
    switch(obj_memory)
    {
    case FRAM_HEAD_ADDR:
    case EEPROM_HEAD_ADDR:
    case RX8025_HEAD_ADDR:	//azh 110526
//    case DS3231_HEAD_ADDR:
        err = write_i2c1(obj_addr, snt_buff, len);
        break;
//    case DATAFLASH1_HEAD_ADDR:
//    case DATAFLASH2_HEAD_ADDR:
//        err = write_dataflash(obj_addr, snt_buff, len);
//        break;
//    case MRAM_HEAD_ADDR:	//azh 110526
//        err = write_mram(obj_addr, snt_buff, len);
//        break;
    case MXFLASH1_HEAD_ADDR:
    case MXFLASH2_HEAD_ADDR:
        err = write_mxflash(obj_addr, snt_buff, len);
        break;
    case IRAM1_HEAD_ADDR:
        if(((obj_addr & 0x00FFFFFF) + len) > IRAM1_CAPACITY)
        {
            err = LEN_OVF;
        }
        else
        {
            memcpy((U8*)obj_addr, snt_buff, len);
            err =  NO_ERROR;
        }
        break;
    case IRAM2_HEAD_ADDR:
        if(((obj_addr & 0x00FFFFFF) + len) > IRAM2_CAPACITY)
        {
            err = LEN_OVF;
        }
        else
        {
            memcpy((U8*)obj_addr, snt_buff, len);
            err =  NO_ERROR;
        }
        break;
    case INTERNAL_RAM_HEAD_ADDR://芯片RAM全局数组下标
    {
    	if(((obj_addr & 0x0000FFFF) + len) > VAR_RAM_ARRAY_MAX)
    	{
    		err = LEN_OVF;
    	}
    	else
        {
            memcpy((U8*)&gVarArray[obj_addr&0x0000FFFF], snt_buff, len);
            err =  NO_ERROR;
        }    	
    }
    break;	
    default:
        if((obj_addr >= IRAM2_HEAD_ADDR)
        && (obj_addr < (IRAM2_HEAD_ADDR + IRAM2_CAPACITY)))     //判断地址是否在AHB RAM范围
        {
            if(((obj_addr + len) > IRAM2_HEAD_ADDR)
            && ((obj_addr + len) < (IRAM2_HEAD_ADDR + IRAM2_CAPACITY)))
            {
                memcpy((U8*)obj_addr, snt_buff, len);
                return NO_ERROR;
            }
            return LEN_OVF;
        }
        return ADDR_ERROR;
    }

    return err;
}

/******************************************************************************
    End Of File
******************************************************************************/

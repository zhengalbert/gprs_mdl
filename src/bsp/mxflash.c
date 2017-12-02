/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : mxflash.c
 Description    : MXFlash读写函数 azh EONFLASH 芯片也可兼容驱动 注意：擦除还得由应用层来处理
 Related Files  :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、    1.10        2011-06-24      azh            MX和EON的驱动可以兼容 现在由二片 函数入口参数加一个cs的参数
   1、    1.00        2010-08-26      snap           the original version
******************************************************************************/

#include "..\inc\global_config.h"

//-------------------------------- 常数定义 -----------------------------------
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
	#define sspx_operation  ssp1_operation
#else
	#define sspx_operation  ssp0_operation
	#define FIRST_CHIP_ADDR_CSX			//定义U8芯片开始保存地址

#endif
//------------------------------ 私有变量声明 ---------------------------------
static U8 mxflash_page_buf[MXFLASH_PAGE_SIZE];

//------------------------------ 私有函数声明 ---------------------------------
static U8 mxflash_get_status(U8 cs);
//static U8 mxflash_get_security_register(U8 cs);
static S32 mxflash_wait_rdy(U8 cs);
static S32 mxflash_write_enable(U8 cs);
static S32 mxflash_status_write(U8 cs, U8 byte);
static S32 mxflash_enter_write_status(U8 cs);
static S32 mxflash_exit_write_status(U8 cs);
//static S32 mxflash_clear_security_register(U8 cs);
static void mxflash_cs(U8 control, U8 cs);
static void mxflash_wp(U8 control);

/******************************************************************************
** 函数名称: S32 read_mxflash(U32 obj_addr, U8 *rcv_buff, U16 len)
** 函数功能: read data from MXFlash
** 入口参数: obj_addr : object logic adrress
**           *rcv_buff: pointer for the receive buffer
**           len      : length of bytes for operation
** 返 回 值: indicating the result of the operation
**              1: normal
**             -1: length overflow, not allowed
**             -2: object memory not found
**             -3: waiting time overflow
**        -others: state error
** 说    明: 无
******************************************************************************/
S32 read_mxflash (U32 obj_addr, U8 *rcv_buff, U16 len)
{
    U8 cs;
    U8 op_code[4];
    U8 try_num;
    U32 obj_memory;
    S32 err;

    obj_memory = obj_addr & 0xff000000;
    obj_addr &= 0x00ffffff;

    if((len == 0) || ((obj_addr + len) > MXFLASH_CAPACITY))
    {
        return LEN_OVF;
    }

    if(obj_memory == MXFLASH1_HEAD_ADDR)
    {
        cs = 1;
    }
    else if(obj_memory == MXFLASH2_HEAD_ADDR)
    {
        cs = 2;
    }
    else
    {
        return ADDR_ERROR;
    }

//azh 140918
	if(os_mut_wait(Mut_SSP_EON_Flash, OS_DLY_2S) == OS_R_TMO)
	{
		return TIME_OVF;
	}

    for(try_num = 0; try_num < 3; try_num++)    //发生错误时重试3次
    {
        err = mxflash_wait_rdy(cs);               //检查器件是否空闲
        if(err != NO_ERROR)
        {
            break;                              //等待超时，返回错误
        }

        mxflash_cs(MXFLASH_ENABLE, cs);
        op_code[0] = 0x03;                      //Continuous read command
        op_code[1] = (U8)(obj_addr >> 16);      //3字节地址
        op_code[2] = (U8)(obj_addr >> 8);
        op_code[3] = (U8)obj_addr;
        err = sspx_operation(op_code, op_code, 4, SSP_WR);
        if(err == NO_ERROR)
        {
            err = sspx_operation(rcv_buff, rcv_buff, len, SSP_RD);
        }
        mxflash_cs(MXFLASH_DISABLE, cs);

        if(err == NO_ERROR)
        {
            break;                              //读取未发生错误时退出循环
        }
    }
//azh 140918
	os_mut_release(Mut_SSP_EON_Flash);

    return err;
}

/******************************************************************************
** 函数名称: S32 write_mxflash(U32 obj_addr, U8 *send_buff, U16 len)
** 函数功能: write data to MXFlash
** 入口参数: obj_addr  : object logic adrress
**           *send_buff: pointer for the send buffer
**           len       : length of bytes for operation
** 返 回 值: indicating the result of the operation
**              1: normal
**             -1: length overflow, not allowed
**             -2: object memory not found
**             -3: waiting time overflow
**            -14: write enable latch status error
** 说    明: 数据写入必须在存储区被擦除的情况下进行
******************************************************************************/
S32 write_mxflash (U32 obj_addr, U8 *send_buff, U16 len)
{
    U8 cs;
    U8 op_code[4];
    U8 try_num;
    U8 page_num;
    U32 page_end_len;
    U32 obj_memory;
    U32 page_addr, byte_addr;
    S32 err;

    obj_memory = obj_addr & 0xff000000;
    obj_addr &= 0x00ffffff;

    if((len == 0) || ((obj_addr + len) > MXFLASH_CAPACITY))
    {
        return LEN_OVF;
    }

    if(obj_memory == MXFLASH1_HEAD_ADDR)
    {
        cs = 1;
    }
    else if(obj_memory == MXFLASH2_HEAD_ADDR)
    {
        cs = 2;
    }
    else
    {
        return ADDR_ERROR;
    }

//azh 140918
	if(os_mut_wait(Mut_SSP_EON_Flash, OS_DLY_2S) == OS_R_TMO)
	{
		return TIME_OVF;
	}


    err = NO_ERROR;
    byte_addr = (obj_addr % MXFLASH_PAGE_SIZE);                     //计算一页中的偏移地址
    page_addr = (obj_addr / MXFLASH_PAGE_SIZE);                     //计算页地址

    if(byte_addr != 0)                                              //判断写入的开始地址是否页对齐
    {
        if((byte_addr + len) > MXFLASH_PAGE_SIZE)                   //判断写入长度是否大于一页
        {
            page_end_len = 256;                                     //这一页需要写满
            page_num = (byte_addr + len) / MXFLASH_PAGE_SIZE;       //计算需要写的页数
            memset(mxflash_page_buf, 0xff, byte_addr);              //写入数据按页对齐，前面不足部分用'0xff'填充
            memcpy((mxflash_page_buf + byte_addr), send_buff, (MXFLASH_PAGE_SIZE - byte_addr));     //拷贝用户数据

            send_buff += (MXFLASH_PAGE_SIZE - byte_addr);           //数据缓冲区向后偏移写入第一页的数据长度
            len -= (MXFLASH_PAGE_SIZE - byte_addr);                 //总的数据长度减去写入第一页的数据长度
        }
        else
        {
            page_end_len = byte_addr + len;                         //计算实际需要写入的长度
            page_num = 0;
            memset(mxflash_page_buf, 0xff, byte_addr);              //写入数据按页对齐，前面不足部分用'0xff'填充
            memcpy((mxflash_page_buf + byte_addr), send_buff, len);                                 //拷贝用户数据
        }

        for(try_num = 0; try_num < 3; try_num++)                    //写入第一页的数据，发生错误时重试3次
        {
            err = mxflash_enter_write_status(cs);                     //检查器件是否进入写允许状态
            if(err != NO_ERROR)
            {
                continue;
            }

            mxflash_cs(MXFLASH_ENABLE, cs);
            op_code[0] = 0x02;                                      //页写入命令
            op_code[1] = (U8)(page_addr >> 8);                      //2字节页地址
            op_code[2] = (U8)page_addr;
            op_code[3] = 0;                                         //最低字节必须为'0'
            err = sspx_operation(op_code, op_code, 4, SSP_WR);
            if(err == NO_ERROR)
            {
                err = sspx_operation(mxflash_page_buf, mxflash_page_buf, page_end_len, SSP_WR);
            }
            mxflash_cs(MXFLASH_DISABLE, cs);

            if(err != NO_ERROR)
            {
                continue;                   //发生写错误，重试
            }
//azh ENQ64资料里没有看到这条命令
//            if(0 == (mxflash_get_security_register(cs) & (1 << 5)))
//            {
                break;                      //没有发生写错误，退出循环
//            }
//            err = DATA_MISMATCH;            //数据不匹配错误
//            mxflash_clear_security_register(cs);                      //清除错误标志
        }
        if(try_num >= 3)
        {
            goto write_error;               //3次重试失败，返回错误
        }

        page_addr++;                        //第一页数据已经写入，页地址加'1'
    }
    else                                    //写入的开始地址页对齐的情况
    {
        page_end_len = 256;                 //这一页需要写满
        page_num = len / MXFLASH_PAGE_SIZE;                         //计算需要写的页数
        if((len % MXFLASH_PAGE_SIZE) != 0)
        {
            page_num++;                     //最后还有不足一页的数据，所以写入的页数量加'1'
        }
    }

    if(len < MXFLASH_PAGE_SIZE)             //判断写入的下一页数据长度是否不足一页
    {
        page_end_len = len;                 //按实际长度写入
    }

    while(page_num)                         //写入数据只有第一页时不进这个循环
    {
        for(try_num = 0; try_num < 3; try_num++)                    //写入后续n页的数据，发生错误时重试3次
        {
            err = mxflash_enter_write_status(cs);                     //检查器件是否进入写允许状态
            if(err != NO_ERROR)
            {
                continue;
            }

            mxflash_cs(MXFLASH_ENABLE, cs);
            op_code[0] = 0x02;                                      //页写入命令
            op_code[1] = (U8)(page_addr >> 8);                      //2字节页地址
            op_code[2] = (U8)page_addr;
            op_code[3] = 0;                                         //最低字节必须为'0'
            err = sspx_operation(op_code, op_code, 4, SSP_WR);
            if(err == NO_ERROR)
            {
                err = sspx_operation(send_buff, send_buff, page_end_len, SSP_WR);
            }
            mxflash_cs(MXFLASH_DISABLE, cs);

            if(err != NO_ERROR)
            {
                continue;                   //发生写错误，重试
            }
//azh EON没有这条命令
//            if(0 == (mxflash_get_security_register(cs) & (1 << 5)))
//            {
                break;                      //没有发生写错误，退出循环
//            }
//            err = DATA_MISMATCH;            //数据不匹配错误
//            mxflash_clear_security_register(cs);                      //清除错误标志
//
//            if(try_num >= 2)
//            {
//                goto write_error;           //3次重试失败，返回错误
//            }
        }

        if(try_num >= 3)
        {
            goto write_error;           //3次重试失败，返回错误
        }

        page_addr++;                        //页地址加'1'
        send_buff += MXFLASH_PAGE_SIZE;     //数据缓冲区向后偏移一页的数据长度
        len -= MXFLASH_PAGE_SIZE;           //总的数据长度减去一页的数据长度
        if(len < MXFLASH_PAGE_SIZE)         //判断写入的下一页数据长度是否不足一页
        {
            page_end_len = len;             //按实际长度写入
        }
        page_num--;                         //需写入页数减'1'
    }

write_error:
    mxflash_exit_write_status(cs);            //退出写允许状态

//azh 140918
	os_mut_release(Mut_SSP_EON_Flash);

    return err;
}

/******************************************************************************
** 函数名称: S32 mxflash_enter_write_status(U8 cs)
** 函数功能: 通过写入'WREN'命令及设置'BP0~BP3 = 0'使MXFlash进入写允许状态
** 入口参数: 无
** 返 回 值: indicating the result of the operation
**              1: normal
**             -3: waiting time overflow
**            -14: write enable latch status error
** 说    明: 无
******************************************************************************/
S32 mxflash_enter_write_status (U8 cs)
{
    U8 try_num;

    for(try_num = 0; try_num < 3; try_num++)                    //发生错误时重试3次
    {
        mxflash_write_enable(cs);                                 //使能写：WEL = 1
        mxflash_status_write(cs, (1 << 1));                           //SRWD = 0; BP0~BP3 = 0; WEL = 1
        
        mxflash_write_enable(cs);                                 //使能写：WEL = 1
        if(mxflash_get_status(cs) == (1 << 1))                    //检查状态字：写入是否允许
        {
            return NO_ERROR;
        }
    }

    return WREN_LATCH;
}

/******************************************************************************
** 函数名称: S32 mxflash_exit_write_status(U8 cs)
** 函数功能: 通过设置'BP0~BP3 = 1'使MXFlash进入写禁止状态
** 入口参数: 无
** 返 回 值: indicating the result of the operation
**              1: normal
**             -3: waiting time overflow
** 说    明: 无
******************************************************************************/
S32 mxflash_exit_write_status (U8 cs)
{
    U8 try_num;
    S32 err;

    for(try_num = 0; try_num < 3; try_num++)        //发生错误时重试3次
    {
        mxflash_write_enable(cs);                     //使能写：WEL = 1
        mxflash_status_write(cs, ((1 << 7) | (0x0f << 2) | (1 << 1)));            //SRWD = 1; BP0~BP3 = 1; WEL = 1
        err = mxflash_wait_rdy(cs);                   //检查器件是否空闲
        if(err != NO_ERROR)
        {
            break;                                  //等待超时，返回错误
        }

        if(mxflash_get_status(cs) == ((1 << 7) | (0x0f << 2) | (1 << 1)))     //检查状态字：写入是否禁止
        {
            return NO_ERROR;
        }
    }

    return WREN_LATCH;
}

/******************************************************************************
** 函数名称: S32 mxflash_wait_rdy(U8 cs)
** 函数功能: 等待MXFlash就绪，若异常返回超时
** 入口参数: 无
** 返 回 值: indicating the result of the operation
**              1: normal
**             -3: waiting time overflow
** 说    明: 无
******************************************************************************/
S32 mxflash_wait_rdy (U8 cs)
{
    U16 k;

//    for(k = 0; k < 600; k++)
    for(k = 0; k < 150; k++)
    {
        if(0 == (mxflash_get_status(cs) & 0x01))      //检查WIP位
        {
            return NO_ERROR;                        //MXFlash is ready
        }
//        delay(1000);                                //延时1mS
//azh 140918 增加互斥量 可以系统挂起 利于清看门狗
        os_dly_wait(OS_DLY_10MS);
    }

    return TIME_OVF;                                //MXFlash is not ready
}

/******************************************************************************
** 函数名称: S32 mxflash_sector_erase(U32 obj_addr)
** 函数功能: 擦除MXFlash一个扇区，若异常返回超时
** 入口参数: obj_addr: 待擦除的扇区地址
** 返 回 值: indicating the result of the operation
**              1: normal
**             -3: waiting time overflow
** 说    明: 无
******************************************************************************/
S32 mxflash_sector_erase (U32 obj_addr)
{
    U8 cs;
    U8 op_code[4];
    U8 try_num;
    U32 obj_memory;
    S32 err;

    obj_memory = obj_addr & 0xff000000;
    obj_addr &= 0x00ffffff;

    if(obj_addr > MXFLASH_CAPACITY)
    {
        return ADDR_ERROR;
    }

    if(obj_memory == MXFLASH1_HEAD_ADDR)
    {
        cs = 1;
    }
    else if(obj_memory == MXFLASH2_HEAD_ADDR)
    {
        cs = 2;
    }
    else
    {
        return ADDR_ERROR;
    }

//azh 140918
	if(os_mut_wait(Mut_SSP_EON_Flash, OS_DLY_2S) == OS_R_TMO)
	{
		return TIME_OVF;
	}

//    obj_addr /= MXFLASH_SECTOR_SIZE;                //计算扇区地址

    for(try_num = 0; try_num < 3; try_num++)        //发生错误时重试3次
    {
        err = mxflash_enter_write_status(cs);         //检查器件是否进入写允许状态
        if(err != NO_ERROR)
        {
            continue;
        }

        mxflash_cs(MXFLASH_ENABLE, cs);
        op_code[0] = 0x20;                          //sector erase command
        op_code[1] = (U8)(obj_addr >> 16);
        op_code[2] = (U8)(obj_addr >> 8);
        op_code[3] = (U8)obj_addr;
        err = sspx_operation(op_code, op_code, 4, SSP_WR);
        mxflash_cs(MXFLASH_DISABLE, cs);

        if(err != NO_ERROR)
        {
            continue;                               //发生写错误，重试
        }
        break;//azh
//azh EON没有这条命令
//        if(0 == (mxflash_get_security_register(cs) & (1 << 6)))
//        {
//            break;                                  //没有发生错误，退出循环
//        }
//        err = DATA_MISMATCH;                        //数据不匹配错误
//        mxflash_clear_security_register(cs);          //清除错误标志
    }

    mxflash_exit_write_status(cs);                    //退出写允许状态

//azh 140918
	os_mut_release(Mut_SSP_EON_Flash);

    return err;
}

/******************************************************************************
** 函数名称: S32 mxflash_clear_security_register (U8 cs)
** 函数功能: 清除MXFlash的安全寄存器bit6、bit5
** 入口参数: 无
** 返 回 值: indicating the result of the operation
**              1: normal
**             -3: waiting time overflow
**        -others: state error
** 说    明: 无
******************************************************************************/
/*
S32 mxflash_clear_security_register (U8 cs)
{
    U8 buffer[1];
    U8 try_num;
    S32 err;

    for(try_num = 0; try_num < 3; try_num++)        //发生错误时重试3次
    {
        mxflash_cs(MXFLASH_ENABLE, cs);
        buffer[0] = 0x30;                           //clear security register command
        err = sspx_operation(buffer, buffer, 1, SSP_WR);
        mxflash_cs(MXFLASH_DISABLE, cs);

        if(err == NO_ERROR)
        {
            break;                                  //没有错误发生，退出循环
        }
    }

    return err;
}
*/
/******************************************************************************
** 函数名称: U8 mxflash_get_security_register(U8 cs)
** 函数功能: 读取MXFlash的安全寄存器
** 入口参数: 无
** 返 回 值: MXFlash的安全寄存器值
** 说    明: 在读取过程发生错误时状态字返回'0xff'
******************************************************************************/
/*
U8 mxflash_get_security_register (U8 cs)
{
    U8 buffer[1];
    U8 try_num;
    S32 err;

    for(try_num = 0; try_num < 3; try_num++)        //发生错误时重试3次
    {
        err = mxflash_wait_rdy(cs);                   //检查器件是否空闲
        if(err != NO_ERROR)
        {
            break;                                  //等待超时，返回错误
        }

        mxflash_cs(MXFLASH_ENABLE, cs);
        buffer[0] = 0x2b;                           //read the security register
        err = sspx_operation(buffer, buffer, 1, SSP_WR);
        if(err == NO_ERROR)
        {
            err = sspx_operation(buffer, buffer, 1, SSP_RD);
        }
        mxflash_cs(MXFLASH_DISABLE, cs);

        if(err == NO_ERROR)
        {
            return(buffer[0]);                      //读取中没有错误发生，返回寄存器值
        }
    }

    return 0xff;                                    //读取过程发生错误时返回'0xff'
}
*/
/******************************************************************************
** 函数名称: S32 mxflash_write_enable(U8 cs)
** 函数功能: 设置MXFlash进入写允许状态，WEL = 1
** 入口参数: 无
** 返 回 值: indicating the result of the operation
**              1: normal
**             -3: waiting time overflow
**        -others: state error
** 说    明: 无
******************************************************************************/
S32 mxflash_write_enable (U8 cs)
{
    U8 buffer[1];
    U8 try_num;
    S32 err;

    for(try_num = 0; try_num < 3; try_num++)        //发生错误时重试3次
    {
        err = mxflash_wait_rdy(cs);                   //检查器件是否空闲
        if(err != NO_ERROR)
        {
            break;                                  //等待超时，返回错误
        }

        mxflash_cs(MXFLASH_ENABLE, cs);
        buffer[0] = 0x06;                           //Write enable command
        err = sspx_operation(buffer, buffer, 1, SSP_WR);
        mxflash_cs(MXFLASH_DISABLE, cs);

        if(err == NO_ERROR)
        {
            break;                                  //没有错误发生，退出循环
        }
    }

    return err;
}

/******************************************************************************
** 函数名称: S32 mxflash_status_write(U8 cs, U8 byte)
** 函数功能: 写MXFlash的状态字
** 入口参数: byte: 要写入的状态字
** 返 回 值: indicating the result of the operation
**              1: normal
**             -3: waiting time overflow
**        -others: state error
** 说    明: 无
******************************************************************************/
S32 mxflash_status_write (U8 cs, U8 byte)
{
    U8 buffer[2];
    U8 try_num;
    S32 err;

    for(try_num = 0; try_num < 3; try_num++)        //发生错误时重试3次
    {
        err = mxflash_wait_rdy(cs);                   //检查器件是否空闲
        if(err != NO_ERROR)
        {
            break;                                  //等待超时，返回错误
        }
        mxflash_wp(MXFLASH_WR);                     //azh 要放在CS前
        mxflash_cs(MXFLASH_ENABLE, cs);
        buffer[0] = 0x01;                           //write the status

        buffer[1] = byte & (~(1 << 6));             //要写入的状态字，屏蔽bit6

        err = sspx_operation(buffer, buffer, 2, SSP_WR);
        mxflash_cs(MXFLASH_DISABLE, cs);
        mxflash_wp(MXFLASH_RD);
        
        if(err == NO_ERROR)
        {
            break;                                  //没有错误发生，退出循环
        }
    }

    return err;
}

/******************************************************************************
** 函数名称: U8 mxflash_get_status(U8 cs)
** 函数功能: 读取MXFlash的状态字
** 入口参数: 无
** 返 回 值: MXFLASH的状态字
** 说    明: 在读取过程发生错误时状态字返回'0'
******************************************************************************/
U8 mxflash_get_status (U8 cs)
{
    U8 buffer[1];
    U8 try_num;
    S32 err;

    for(try_num = 0; try_num < 3; try_num++)        //发生错误时重试3次
    {
        mxflash_cs(MXFLASH_ENABLE, cs);
        buffer[0] = 0x05;                           //read the status register
        err = sspx_operation(buffer, buffer, 1, SSP_WR);
        if(err == NO_ERROR)
        {
            err = sspx_operation(buffer, buffer, 1, SSP_RD);
        }
        mxflash_cs(MXFLASH_DISABLE, cs);

        if(err == NO_ERROR)
        {
            if(buffer[0] != 0xff)
            {
                return(buffer[0]);                  //读取中没有错误发生，返回寄存器值
            }
        }
    }

    return 0x00;                                    //读取过程发生错误时清除状态值
}

/******************************************************************************
** 函数名称: void mxflash_cs(U8 control, U8 cs)
** 函数功能: 根据参数使能或禁能MXFlash
** 入口参数: control: 方法--使能/禁能
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void mxflash_cs (U8 control, U8 cs)
{
/*
    PINSEL3 &= ~(0x03u << 0);           //先清除相关bits
    PINSEL3 |= (P1_16_FNUC << 0);       //configure the CS signal GPIO
    PINMODE3 &= ~(0x03u << 0);          //先清除相关bits
    PINMODE3 |= (P1_16_MODE << 0);      //configure the CS signal pull-up
    PINMODE_OD1 &= ~P1_MX_CS;           //设置引脚非开漏模式
    MXFLASH_CS_DIS();                   //禁止MXFlash的片选

    if(control == MXFLASH_ENABLE)
    {
        MXFLASH_CS_EN();                //允许MXFLASH的片选
    }
    else
    {
        MXFLASH_CS_DIS();               //禁止MXFLASH的片选
    }
*/

#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
    PINSEL0 &= ~(0x03u << 12);          //先清除相关bits
    PINSEL0 |= (P0_6_FNUC << 12);      //configure the CS signal GPIO
    PINMODE0 &= ~(0x03u << 12);         //先清除相关bits
    PINMODE0 |= (P0_6_MODE << 12);     //configure the CS signal pull-up
    PINMODE_OD0 &= ~P0_F1_CS1;          //设置引脚非开漏模式

    if(control == MXFLASH_ENABLE)
    {
        if(cs == 1)
        {
            FLS1_CS_EN();
        }
    }
    else
    {
        FLS1_CS_DIS();
    }
#else
    PINSEL3 &= ~(0x03u << 12);          //先清除相关bits
    PINSEL3 |= (P1_22_FNUC << 12);      //configure the CS signal GPIO
    PINMODE3 &= ~(0x03u << 12);         //先清除相关bits
    PINMODE3 |= (P1_22_MODE << 12);     //configure the CS signal pull-up
    PINMODE_OD1 &= ~P1_F1_CS1;          //设置引脚非开漏模式

    PINSEL3 &= ~(0x03u << 10);          //先清除相关bits
    PINSEL3 |= (P1_21_FNUC << 10);      //configure the CS signal GPIO
    PINMODE3 &= ~(0x03u << 10);         //先清除相关bits
    PINMODE3 |= (P1_21_MODE << 10);     //configure the CS signal pull-up
    PINMODE_OD1 &= ~P1_F1_CS2;          //设置引脚非开漏模式

    if(control == MXFLASH_ENABLE)
    {
#ifdef FIRST_CHIP_ADDR_CSX
        if(cs == 1)
        {
            FLS2_CS_EN();
            FLS1_CS_DIS();
        }
        else
        {
            FLS2_CS_DIS();//azh CS2 新版硬件选中的是U8 而且新版本二片FLASH只用了一个WP脚
            FLS1_CS_EN();
        }
#else
        if(cs == 1)
        {
            FLS1_CS_EN();
            FLS2_CS_DIS();
        }
        else
        {
            FLS1_CS_DIS();//azh CS2 新版硬件0x72xxxxxx 选中的是U8 而且新版本二片FLASH只用了一个WP脚
            FLS2_CS_EN();
        }
#endif//FIRST_CHIP_ADDR_CSX
    }
    else
    {
        FLS1_CS_DIS();
        FLS2_CS_DIS();
    }
#endif
}

/******************************************************************************
** 函数名称: void mxflash_wp(U8 control)
** 函数功能: 根据参数允许或禁止MXFlash写保护
** 入口参数: control: 方法--允许/禁止
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void mxflash_wp (U8 control)
{
/*
    PINSEL4 &= ~(0x03u << 4);           //先清除相关bits
    PINSEL4 |= (P2_2_FNUC << 4);        //configure the CS signal GPIO
    PINMODE4 &= ~(0x03u << 4);          //先清除相关bits
    PINMODE4 |= (P2_2_MODE << 4);       //configure the CS signal pull-down
    PINMODE_OD2 &= ~P2_MX_WP;           //设置引脚非开漏模式

    if(control == MXFLASH_WR)
    {
        MXFLASH_WR_EN();                //允许写入MXFLASH
    }
    else
    {
        MXFLASH_WR_DIS();               //禁止写入MXFLASH
    }
*/
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
    PINSEL0 &= ~(0x03u << 10);          //先清除相关bits
    PINSEL0 |= (P0_5_FNUC << 10);      //configure the WP signal GPIO
    PINMODE0 &= ~(0x03u << 10);         //先清除相关bits
    PINMODE0 |= (P0_5_MODE << 10);     //configure the WP signal pull-down
    PINMODE_OD0 &= ~(P0_F1_WP);         //设置引脚非开漏模式
#else
    PINSEL3 &= ~(0x03u << 20);          //先清除相关bits
    PINSEL3 |= (P1_26_FNUC << 20);      //configure the WP signal GPIO
    PINMODE3 &= ~(0x03u << 20);         //先清除相关bits
    PINMODE3 |= (P1_26_MODE << 20);     //configure the WP signal pull-down
    PINMODE_OD1 &= ~(P1_F1_WP);         //设置引脚非开漏模式
#endif

    if(control == MXFLASH_WR)
    {
        DF_WP_DIS();    //写保护禁能
    }
    else
    {
        DF_WP_EN();     //写保护使能
    }
}

/******************************************************************************
    End Of File
******************************************************************************/

/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : i2c1.c
 Description    : LPC17系列芯片硬件I2C1子程序
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2010-07-20      snap           the original version
******************************************************************************/

#include "..\inc\global_config.h"
//AT24C64单页容量32字节,256单页64字节，512单页128字节
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
	#define EEPROM_PAGE_SIZE    32//焊了24LC64
#else
	#define EEPROM_PAGE_SIZE    64//实际焊了512Kbit 考虑为换成256Kbit 所以页小一点不会出问题 130321
#endif
//-------------------------------- 常数定义 -----------------------------------

//------------------------------ 私有变量声明 ---------------------------------

//------------------------------ 私有函数声明 ---------------------------------
static S32 check_i2c_addr (U32 obj_addr, U32 obj_memory, U16 len);
static void i2c1_init(void);
static void i2c1_rst(void);

/******************************************************************************
** 函数名称: S32 read_i2c1(U32 obj_addr, U8 *rcv_buff, U16 len)
** 函数功能: read data from FRAM/EEPROM/RTC
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
S32 read_i2c1 (U32 obj_addr, U8 *rcv_buff, U16 len)
{
    U8 try_num,dlyct=10;
    U32 obj_memory;
    S32 wait_time;
    S32 res;

    obj_memory = (obj_addr & 0xFF000000);
    obj_addr &= 0x00FFFFFF;
    res = check_i2c_addr(obj_addr, obj_memory, len);                //检查地址和长度
    if(res != NO_ERROR)
    {
        return res;
    }

//azh 110527------------------------------
	if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
	{
	    if(len < 10)//azh 原来有的时候会超时
	    {
	    	dlyct = 10;
	    }
	    else
	    {
	    	dlyct = len;
	    }		
	    if(os_mut_wait(Mut_I2C1_Mem, dlyct * OS_DLY_10MS) == OS_R_TMO)
	    {
	        return(0x03);
	    }
	    if(obj_memory == EEPROM_HEAD_ADDR)
	    {
	        if(os_mut_wait(Mut_EEPROM, dlyct * OS_DLY_10MS) == OS_R_TMO)
	        {
	            return(0x03);
	        }
	    }
	}

//--------------------------------
    for(try_num = 0; try_num < 3; try_num++)
    {
        i2c1_init();

        if(obj_memory == FRAM_HEAD_ADDR)            //fill in the slave address and sub address
        {
            I2C_Mem[I2C1].Slave = FRAM_ADDR;
            I2C_Mem[I2C1].Addr_Num = 2;
//            I2C_Mem[I2C1].Addr_Num = 1;             //A相模块用的是FM24CL04，只有一个地址字节
        }
        else if(obj_memory == EEPROM_HEAD_ADDR)
        {
            I2C_Mem[I2C1].Slave = EEPROM_ADDR;
            I2C_Mem[I2C1].Addr_Num = 2;
        }
        else if(obj_memory == RX8025_HEAD_ADDR)
        {
            I2C_Mem[I2C1].Slave = RX8025_ADDR;
            I2C_Mem[I2C1].Addr_Num = 1;
        }
        else if(obj_memory == DS3231_HEAD_ADDR)
        {
            I2C_Mem[I2C1].Slave = DS3231_ADDR;
            I2C_Mem[I2C1].Addr_Num = 1;
        }
        else
        {
		    if(CON_CPU_NORM_MODE == cpu_run_mode())
		    {
			    if(obj_memory == EEPROM_HEAD_ADDR)
			    {
			        os_mut_release(Mut_EEPROM);
			    }
			    os_mut_release(Mut_I2C1_Mem);
			}
            return ADDR_ERROR;
        }

        I2C_Mem[I2C1].End_Flag = I2C_IDLE;
        I2C_Mem[I2C1].Operation = I2C_READ_OP;
        I2C_Mem[I2C1].Data_Ptr = rcv_buff;
        I2C_Mem[I2C1].Sub_Addr = obj_addr;
        I2C_Mem[I2C1].Num = len;
        I2C_Mem[I2C1].Error = NO_ERROR;

		if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
		{
			os_sem_init(Sem_I2C1_Mem_End, 0);//azh
		}

        I2C_Mem[I2C1].State = I2C_SLA_WR;

        I2C1CONSET = ((1 << 5) | (1 << 6));         //set STA: send start condition

		if(CON_CPU_NORM_MODE != cpu_run_mode())//掉电　不带系统运行模式
		{
	        wait_time = (len + 4) * 10 * (1000000 / I2C1_FREQ) * 10;    //等待10倍的理论通讯时间
	        while(wait_time--)                          //等待I2C操作完成
	        {
	            if(I2C_Mem[I2C1].End_Flag == I2C_END)
	            {
	                break;
	            }
	            delay(1);                               //延时约1uS
	        }
	        if(wait_time < 0)
	        {
	            I2C_Mem[I2C1].Error = TIME_OVF;         //等待超时
	        }

	        delay(10);                                   //等待停止位时序完成
	        if((I2C_Mem[I2C1].Error == NO_ERROR) || (try_num >= 2))     //没有错误发生或已经尝试3次
	        {
	            break;
	        }
	        i2c1_rst();
	        if((I2C_Mem[I2C1].Error == STATE_ERR7) && (obj_memory == EEPROM_HEAD_ADDR))     //EEPROM在写周期，因为在写入时序中是不等待写入结束的
	        {
	            delay(250);                            //等待EEPROM写入完成
	        }
	        else
	        {
	            delay(100);                             //如果读取不成功，则每间隔100us尝试1次，尝试3次
	        }
	    }
		else
		{
	        if(((os_sem_wait(Sem_I2C1_Mem_End, len * OS_DLY_10MS) != OS_R_TMO)
	           && (I2C_Mem[I2C1].Error == NO_ERROR))
	           || (try_num >= 2))
	        {
	            i2c1_rst();
	            break;
	        }
			//os_dly_wait(OS_DLY_5MS);//azh
            os_dly_wait(OS_DLY_10MS);//azh 
		}
    }

    i2c1_rst();
    delay(20);
//
	if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
	{
	    if(obj_memory == EEPROM_HEAD_ADDR)
	    {
	        os_mut_release(Mut_EEPROM);
	    }
	    os_mut_release(Mut_I2C1_Mem);
	}

    return(I2C_Mem[I2C1].Error);
}

/******************************************************************************
** 函数名称: S32 write_i2c1(U32 obj_addr, U8 *snt_buff, U16 len)
** 函数功能: write data to FRAM/EEPROM/RTC
** 入口参数: obj_addr : object logic adrress
**           *snt_buff: pointer for the send buffer
**           len      : length of bytes for operation
** 返 回 值: indicating the result of the operation
**              1: normal
**             -1: length overflow, not allowed
**             -2: object memory not found
**             -3: waiting time overflow
**        -others: state error
** 说    明: 无
******************************************************************************/
S32 write_i2c1 (U32 obj_addr, U8 *snt_buff, U16 len)
{
    U8 try_num,dlyct=10;
    U8 *page_buf;
    U16 page_len;
    U32 obj_memory;
    U32 page_addr;
    S32 wait_time;
    S32 res;

    obj_memory = (obj_addr & 0xFF000000);
    obj_addr &= 0x00FFFFFF;
    res = check_i2c_addr(obj_addr, obj_memory, len);                //检查地址和长度
    if(res != NO_ERROR)
    {
        return res;
    }
//azh 110527------------------------------
	if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
	{
	    if(len < 10)//azh 原来有的时候会超时
	    {
	    	dlyct = 10;
	    }
	    else
	    {
	    	dlyct = len;
	    }		
		if(os_mut_wait(Mut_I2C1_Mem, dlyct * OS_DLY_10MS) == OS_R_TMO)
		{
			return(0x03);
		}
		if(obj_memory == EEPROM_HEAD_ADDR)
		{
			if(os_mut_wait(Mut_EEPROM, dlyct * OS_DLY_10MS) == OS_R_TMO)
			{
				return(0x03);
			}
		}
	}
//--------------------------------
    while(len)
    {
        page_buf = snt_buff;
        page_addr = obj_addr;
        if(obj_memory == FRAM_HEAD_ADDR)
        {
            page_len = len;
        }
        else if(obj_memory == EEPROM_HEAD_ADDR)
        {
            page_len = EEPROM_PAGE_SIZE - (obj_addr & (EEPROM_PAGE_SIZE-1));
            if(page_len > len)
            {
                page_len = len;
            }
            snt_buff += page_len;
            obj_addr += page_len;
        }
        else if(obj_memory == RX8025_HEAD_ADDR)
        {
            page_len = len;
        }
        else if(obj_memory == DS3231_HEAD_ADDR)
        {
            page_len = len;
        }
        else
        {
            return ADDR_ERROR;
        }
        len -= page_len;

        for(try_num = 0; try_num < 3; try_num++)
        {
            i2c1_init();

            if(obj_memory == FRAM_HEAD_ADDR)        //fill in the slave address and sub address
            {
                I2C_Mem[I2C1].Slave = FRAM_ADDR;
                I2C_Mem[I2C1].Addr_Num = 2;
//                I2C_Mem[I2C1].Addr_Num = 1;         //A相模块用的是FM24CL04，只有一个地址字节
//                FRAM_WR_EN();                       //enable write operation
            }
            else if(obj_memory == EEPROM_HEAD_ADDR)
            {
                I2C_Mem[I2C1].Slave = EEPROM_ADDR;
                I2C_Mem[I2C1].Addr_Num = 2;
                EE_WP_DIS();                        //disable write operation protection
            }
            else if(obj_memory == RX8025_HEAD_ADDR)
            {
                I2C_Mem[I2C1].Slave = RX8025_ADDR;
                I2C_Mem[I2C1].Addr_Num = 1;
            }
            else if(obj_memory == DS3231_HEAD_ADDR)
            {
                I2C_Mem[I2C1].Slave = DS3231_ADDR;
                I2C_Mem[I2C1].Addr_Num = 1;
            }
            else
            {
			    if(obj_memory == EEPROM_HEAD_ADDR)
			    {
			        os_mut_release(Mut_EEPROM);
			    }
			    os_mut_release(Mut_I2C1_Mem);
				return ADDR_ERROR;
            }

            I2C_Mem[I2C1].End_Flag = I2C_IDLE;
            I2C_Mem[I2C1].Operation = I2C_WRITE_OP;
            I2C_Mem[I2C1].Data_Ptr = page_buf;
            I2C_Mem[I2C1].Sub_Addr = page_addr;
            I2C_Mem[I2C1].Num = page_len;
            I2C_Mem[I2C1].Error = NO_ERROR;
			
			if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
			{			
				os_sem_init(Sem_I2C1_Mem_End, 0);
			}
			
            I2C_Mem[I2C1].State = I2C_SLA_WR;
            I2C1CONSET = ((1 << 5) | (1 << 6));
			
			if(CON_CPU_NORM_MODE != cpu_run_mode())//掉电　不带系统运行模式
			{
	            wait_time = (page_len + 4) * 10 * (1000000 / I2C1_FREQ) * 10;   //等待10倍的理论通讯时间
	            while(wait_time--)                      //等待I2C操作完成
	            {
	                if(I2C_Mem[I2C1].End_Flag == I2C_END)
	                {
	                    break;
	                }
	                delay(1);                           //延时约1uS
	            }
	            if(wait_time < 0)
	            {
	                I2C_Mem[I2C1].Error = TIME_OVF;     //等待超时
	            }
	
	            delay(4);                               //等待停止位时序完成
	            if((I2C_Mem[I2C1].Error == NO_ERROR) || (try_num >= 2))         //没有错误发生或已经尝试3次
	            {
	                break;
	            }
	            i2c1_rst();
	            delay(250);                            //如果写入不成功，则每间隔5ms尝试1次，尝试3次
	        }
			else
			{
	            if(((os_sem_wait(Sem_I2C1_Mem_End, page_len * OS_DLY_10MS) != OS_R_TMO)
	               &&(I2C_Mem[I2C1].Error == NO_ERROR))
	               || (try_num >= 2))
	            {
	                i2c1_rst();
	                break;
	            }
	            os_dly_wait(OS_DLY_10MS);   //如果写入不成功，则每间隔10ms，尝试3次
	        }
        }

        i2c1_rst();
        if(CON_CPU_NORM_MODE != cpu_run_mode())//掉电　不带系统运行模式
        {
	        if((obj_memory == EEPROM_HEAD_ADDR) && (len != 0))
	        {
	            delay(250); //5000                           //EEPROM下次写入前等待当前写入完成，最后一次写入是不等待其完成的
	        }
	    }
		else//有电状态　带系统运行
		{
	        if((obj_memory == EEPROM_HEAD_ADDR) && (len != 0))
	        {
	            os_dly_wait(OS_DLY_10MS);
	        }
	    }

    }
    EE_WP_EN();                             //enable write operation protection
	
	if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
	{
	    os_mut_release(Mut_I2C1_Mem);
	    if(obj_memory == EEPROM_HEAD_ADDR)
	    {
	        os_dly_wait(OS_DLY_10MS);       //如果是写EEPROM函数，则写完后系统延时10ms
	        os_mut_release(Mut_EEPROM);
	    }
	}

    return(I2C_Mem[I2C1].Error);
}

/******************************************************************************
** 函数名称: S32 check_i2c_addr(U32 obj_addr, U32 obj_memory, U16 len)
** 函数功能: 检查I2C操作地址和数据长度的有效性
** 入口参数: obj_addr  : object real adrress
**           obj_memory: object memory chip
**           len       : length of bytes for operation
** 返 回 值: indicating the result of the address check
**              1: normal
**             -1: length overflow, not allowed
**             -2: object memory not found
** 说    明: 无
******************************************************************************/
S32 check_i2c_addr (U32 obj_addr, U32 obj_memory, U16 len)
{
    if((len == 0) || (len > 512))
    {
        return LEN_OVF;
    }

    if(obj_memory == FRAM_HEAD_ADDR)
    {
        if((obj_addr + len) > FRAM_CAPACITY)
        {
            return LEN_OVF;
        }
    }
    else if(obj_memory == EEPROM_HEAD_ADDR)
    {
        if((obj_addr + len) > EEPROM_CAPACITY)
        {
            return LEN_OVF;
        }
    }
    else if(obj_memory == RX8025_HEAD_ADDR)
    {
        if((obj_addr + len) > RX8025_CAPACITY)
        {
            return LEN_OVF;
        }
    }
    else if(obj_memory == DS3231_HEAD_ADDR)
    {
        if((obj_addr + len) > DS3231_CAPACITY)
        {
            return LEN_OVF;
        }
    }
    else
    {
        return ADDR_ERROR;
    }
    return NO_ERROR;
}

/******************************************************************************
** 函数名称: void i2c1_init(void)
** 函数功能: Initializes I2C1 peripheral control and registers to their default reset values
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void i2c1_init (void)
{
    PCONP |= (1 << PCI2C1);             //enable the clock
    i2c1_rst();                         //禁止I2C1及其中断

    if((PINSEL0 & ((0x03u << 0) | (0x03u << 2))) != ((P0_1_FNUC << 2) | (P0_0_FNUC << 0)))
    {
        PINSEL0 &= ~((0x03u << 0) | (0x03u << 2));              //先清除相关bits
        PINSEL0 |= ((P0_1_FNUC << 2) | (P0_0_FNUC << 0));       //configure the SCL1 and SDA1 as the alternate function 1 configuration
    }
    PINMODE0 &= ~((0x03u << 0) | (0x03u << 2));                 //先清除相关bits
    PINMODE0 |= ((P0_1_MODE << 2) | (P0_0_MODE << 0));          //SCL1 and SDA1 has neither pull-up nor pull-down registers
    PINMODE_OD0 |= (P0_SDA_E2 | P0_SCL_E2);                           //设置引脚开漏模式

#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
    if((PINSEL1 & (0x03u << 8)) != (P0_20_FNUC << 8))
    {
        PINSEL1 &= ~(0x03u << 8);       //先清除相关bits
        PINSEL1 |= (P0_20_FNUC << 8);   //configure the WP signal GPIO
    }
    PINMODE1 &= ~(0x03u << 8);          //先清除相关bits
    PINMODE1 |= (P0_20_MODE << 8);      //configure the WP signal pull-up
    PINMODE_OD0 &= ~P0_EE_WP;           //设置引脚非开漏模式
    EE_WP_EN();                         //enable write operation protection

    PCLKSEL1 &= ~(0x03u << 6);
    PCLKSEL1 |= (FPCLK_I2C1_DIV << 6);          //设置外设时钟分频系数
	
    I2C1SCLH = FPCLK_I2C1 / I2C1_FREQ / 2;      //init the speed of the I2C1 bus
    I2C1SCLL = FPCLK_I2C1 / I2C1_FREQ / 2;
#else
//#ifdef PHASE_C_MODULE
    if((PINSEL3 & (0x03u << 22)) != (P1_27_FNUC << 22))
    {
        PINSEL3 &= ~(0x03u << 22);       //先清除相关bits
        PINSEL3 |= (P1_27_FNUC << 22);   //configure the WP signal GPIO
    }
    PINMODE3 &= ~(0x03u << 22);          //先清除相关bits
    PINMODE3 |= (P1_27_MODE << 22);      //configure the WP signal pull-up
    PINMODE_OD1 &= ~P1_E2_WP;           //设置引脚非开漏模式
    EE_WP_EN();                         //enable write operation protection

	if(CON_CPU_NORM_MODE != cpu_run_mode())//掉电　不带系统运行模式
	{
	    PCLKSEL1 &= ~(0x03u << 6);
	    PCLKSEL1 |= (PD_FPCLK_I2C1_DIV << 6);          //设置外设时钟分频系数
	
	    I2C1SCLH = PD_FPCLK_I2C1 / I2C1_FREQ / 2;      //init the speed of the I2C1 bus
	    I2C1SCLL = PD_FPCLK_I2C1 / I2C1_FREQ / 2;
	}
	else//有电状态　带系统运行
	{
	    PCLKSEL1 &= ~(0x03u << 6);
	    PCLKSEL1 |= (FPCLK_I2C1_DIV << 6);          //设置外设时钟分频系数
	
	    I2C1SCLH = FPCLK_I2C1 / I2C1_FREQ / 2;      //init the speed of the I2C1 bus
	    I2C1SCLL = FPCLK_I2C1 / I2C1_FREQ / 2;
	}
#endif
//    ENABLE_ISR(NVIC_I2C1, PRIO_I2C1);           //set I2C1 interrupt parameter
//isr_set(NVIC_I2C1, PRIO_I2C1);
}

/******************************************************************************
** 函数名称: void i2c1_rst(void)
** 函数功能: disbale I2C1 functionality and interrupt
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void i2c1_rst (void)
{
//    DISABLE_ISR(NVIC_I2C1);             //disbale I2C1 interrupt
//isr_clr(NVIC_I2C1);
    I2C1CONCLR = 0x6C;                  //disable the I2C1 functionality, clear STA/SI/AA
}

/******************************************************************************
** 函数名称: void I2C1_IRQHandler(void)
** 函数功能: process the I2DAT reg
**           process the global variables and operations
**           clear the interrupt flag
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
**  2011-03-02 编译器报告堆栈最大深度20字节
******************************************************************************/
void I2C1_IRQHandler (void)
{
    switch(I2C1STAT & 0xF8)
    {
    case 0x08:                                          //start condition has been transmitted, waiting to send SLA+W
    case 0x10:                                          //repeated start has been sent, waiting to send SLA+R
        if(I2C_Mem[I2C1].State == I2C_SLA_WR)
        {
            I2C1DAT = (I2C_Mem[I2C1].Slave & 0xFE);     //send the SLA+W
            I2C1CONCLR = 0x28;                          //clear SI and STA
            if(I2C_Mem[I2C1].Addr_Num == 2)             //检查地址是双字节还是单字节
            {
                I2C_Mem[I2C1].State = I2C_SUBADDR_HIGH; //waiting to send the MSB of the sub address
            }
            else
            {
                I2C_Mem[I2C1].State = I2C_SUBADDR_LOW;  //waiting to send the LSB of the sub address
            }
        }
        else if(I2C_Mem[I2C1].State == I2C_SLA_RD)
        {
            I2C1DAT = (I2C_Mem[I2C1].Slave | 0x01);     //send the SLA+R
            I2C1CONCLR = 0x28;                          //clear SI and STA
            I2C_Mem[I2C1].State = I2C_RCV_DATA;         //waiting to send the MSB of the sub address
        }
        else
        {
            I2C_Mem[I2C1].Error = STATE_ERR1;           //state is abnormal
        }
        break;
    case 0x18:                                          //previous state is state 0x08, SLA+W has been transmitted, ACK received
    case 0x28:
        if(I2C_Mem[I2C1].State == I2C_SUBADDR_HIGH)
        {
            I2C1DAT = (U8)(I2C_Mem[I2C1].Sub_Addr >> 8);
            I2C1CONCLR = 0x28;                          //clear SI and STA
            I2C_Mem[I2C1].State = I2C_SUBADDR_LOW;
        }
        else if(I2C_Mem[I2C1].State == I2C_SUBADDR_LOW)
        {
            I2C1DAT = (U8)I2C_Mem[I2C1].Sub_Addr;
            if(I2C_Mem[I2C1].Operation == I2C_READ_OP)
            {
                I2C1CONCLR = 0x28;                      //clear SI and STA
                I2C_Mem[I2C1].State = I2C_SLA_SEND_RD;
            }
            else if(I2C_Mem[I2C1].Operation == I2C_WRITE_OP)
            {
                I2C1CONCLR = 0x28;                      //clear SI and STA
                I2C_Mem[I2C1].State = I2C_SEND_DATA;
            }
            else
            {
                I2C_Mem[I2C1].Error = STATE_ERR2;
            }
        }
        else if(I2C_Mem[I2C1].State == I2C_SLA_SEND_RD)
        {
            I2C1CONSET = (1 << 5);                      //STA again
            I2C1CONCLR = 0x08;                          //clear SI and STA
            I2C_Mem[I2C1].State = I2C_SLA_RD;           //waiting to send SLA+R
        }
        else if(I2C_Mem[I2C1].State == I2C_SEND_DATA)
        {
            if(I2C_Mem[I2C1].Num > 0)
            {
                I2C1DAT = *I2C_Mem[I2C1].Data_Ptr++;
                I2C_Mem[I2C1].Num--;
                I2C1CONCLR = 0x28;                      //clear SI and STA
            }
            else
            {
                I2C1CONSET = (1 << 4);                  //STO
                I2C1CONCLR = 0x28;                      //clear SI and STA
                I2C_Mem[I2C1].End_Flag = I2C_END;       //总线操作完成
				if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
				{              
                	isr_sem_send(Sem_I2C1_Mem_End);
                }
            }
        }
        else
        {
            I2C_Mem[I2C1].Error = STATE_ERR3;
        }
        break;

    case 0x40:                                          //SLA+R has been sent, config the AA to be sent
        if(I2C_Mem[I2C1].State == I2C_RCV_DATA)
        {
            if(I2C_Mem[I2C1].Num <= 1)
            {
                I2C1CONCLR = 0x04;
            }
            else
            {
                I2C1CONSET = 0x04;
            }
            I2C1CONCLR = 0x28;                          //clear SI and STA
        }
        else
        {
            I2C_Mem[I2C1].Error = STATE_ERR4;
        }
        break;
    case 0x50:                                          //data received
        if(I2C_Mem[I2C1].State == I2C_RCV_DATA)
        {
            if(I2C_Mem[I2C1].Num != 0x00)
            {
                *I2C_Mem[I2C1].Data_Ptr++ = I2C1DAT;
                I2C_Mem[I2C1].Num--;
                if(I2C_Mem[I2C1].Num == 1)
                {
                    I2C1CONCLR = 0x2C;                  //STA, SI, AA = 0
                }
                else
                {
                    I2C1CONSET = 0x04;                  //AA = 1
                    I2C1CONCLR = 0x28;
                }
            }
            else
            {
                I2C_Mem[I2C1].Error = LEN_OVF;
            }
        }
        else
        {
            I2C_Mem[I2C1].Error = STATE_ERR5;
        }
        break;
    case 0x58:                                          //last byte received
        if(I2C_Mem[I2C1].State == I2C_RCV_DATA)
        {
            if(I2C_Mem[I2C1].Num == 1)
            {
                I2C_Mem[I2C1].Num--;
                *I2C_Mem[I2C1].Data_Ptr++ = I2C1DAT;
                I2C1CONSET = (1 << 4);                  //STO
                I2C1CONCLR = 0x28;                      //clear SI and STA
                I2C_Mem[I2C1].End_Flag = I2C_END;       //总线操作完成
                if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
                {
                	isr_sem_send(Sem_I2C1_Mem_End);
                }
            }
            else
            {
                I2C_Mem[I2C1].Error = LEN_OVF;
            }
        }
        else
        {
            I2C_Mem[I2C1].Error = STATE_ERR6;
        }
        break;
    case 0x20:                                          //SLA+W is sent, !ACK received, P to be sent
    case 0x30:                                          //I2DAT is sent, !ACK received, P to be sent
    case 0x48:                                          //SLA+R is sent, !ACK received, P to be sent
        I2C1CONSET = (1 << 4);                          //STO
        I2C_Mem[I2C1].Error = STATE_ERR7;               //ACK received
        break;
    case 0x38:                                          // - SLA+R/W sent, arbitration lost
    case 0x68:                                          // -               arbitration lost
    case 0x78:                                          // -               arbitration lost
    case 0xB0:                                          // -               arbitration lost
        I2C_Mem[I2C1].Error = STATE_ERR8;               //arbitration lost
        break;
    default:
        I2C_Mem[I2C1].Error = STATE_ERR9;
        break;
    }

    if(I2C_Mem[I2C1].Error < 0)                         //Error occured and reset procedure
    {
        I2C1CONSET = (1 << 4);                          //STO
        I2C1CONCLR = 0x28;                              //clear SI and STA
        I2C_Mem[I2C1].End_Flag = I2C_END;               //总线操作完成
		if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
		{  
			isr_sem_send(Sem_I2C1_Mem_End);
		}
    }
}

/******************************************************************************
    End Of File
******************************************************************************/

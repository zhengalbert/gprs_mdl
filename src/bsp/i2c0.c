/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : i2c0.c
 Description    : LPC17系列芯片硬件I2C0子程序,用于驱动显示芯片BU97950
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2011-06-01      azh           the original version
******************************************************************************/

#include "..\inc\global_config.h"

//-------------------------------- 常数定义 -----------------------------------

//------------------------------ 私有变量声明 ---------------------------------

//------------------------------ 私有函数声明 ---------------------------------
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不带CPU_GPRS模块

static void i2c0_init(void);
static void i2c0_rst(void);
S32 check_i2c0_addr (U32 obj_addr, U32 obj_memory, U16 len);
/******************************************************************************
** 函数名称: S32 read_i2c0(U32 obj_addr, U8 *rcv_buff, U16 len)
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
S32 read_i2c0 (U32 obj_addr, U8 *rcv_buff, U16 len)
{
    U8 try_num;
    U32 obj_memory;
    S32 wait_time;
    S32 res;

    obj_memory = (obj_addr & 0xFF000000);
    obj_addr &= 0x000000FF;
    res = check_i2c0_addr(obj_addr, obj_memory, len);                //检查地址和长度
    if(res != NO_ERROR)
    {
        return res;
    }

//azh 110527------------------------------
	if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
	{
		if(os_mut_wait(Mut_I2C0_Mem,	len	* OS_DLY_10MS) == OS_R_TMO)
		{
			return(0x03);
		}
	}
	
//--------------------------------
    for(try_num = 0; try_num < 3; try_num++)
    {
        i2c0_init();

        if(obj_memory == BU97950_HEAD_ADDR)            //fill in the slave address and sub address
        {
            I2C_Mem[I2C0].Slave = BU97950_ADDR;
            I2C_Mem[I2C0].Addr_Num = 1;             //A相模块用的是FM24CL04，只有一个地址字节
        }
        else
        {
		    if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
		    {
		    	os_mut_release(Mut_I2C0_Mem);
		    }
            return ADDR_ERROR;
        }

        I2C_Mem[I2C0].End_Flag = I2C_IDLE;
        I2C_Mem[I2C0].Operation = I2C_READ_OP;
        I2C_Mem[I2C0].Data_Ptr = rcv_buff;
        I2C_Mem[I2C0].Sub_Addr = obj_addr;
        I2C_Mem[I2C0].Num = len;
        I2C_Mem[I2C0].Error = NO_ERROR;

		if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
		{
			os_sem_init(Sem_I2C0_Mem_End, 0);//azh
		}

        I2C_Mem[I2C0].State = I2C_SLA_WR;

        I2C0CONSET = ((1 << 5) | (1 << 6));         //set STA: send start condition

		if(CON_CPU_NORM_MODE != cpu_run_mode())//掉电　不带系统运行模式
		{
	        wait_time = (len + 4) * 10 * (1000000 / I2C0_FREQ) * 10;    //等待10倍的理论通讯时间
	        while(wait_time--)                          //等待I2C操作完成
	        {
	            if(I2C_Mem[I2C0].End_Flag == I2C_END)
	            {
	                break;
	            }
	            delay(1);                               //延时约1uS
	        }
	        if(wait_time < 0)
	        {
	            I2C_Mem[I2C0].Error = TIME_OVF;         //等待超时
	        }

	        delay(4);                                   //等待停止位时序完成
	        if((I2C_Mem[I2C0].Error == NO_ERROR) || (try_num >= 2))     //没有错误发生或已经尝试3次
	        {
	            break;
	        }
	        i2c0_rst();
	        if((I2C_Mem[I2C0].Error == STATE_ERR7) && (obj_memory == EEPROM_HEAD_ADDR))     //EEPROM在写周期，因为在写入时序中是不等待写入结束的
	        {
	            delay(250);                            //等待EEPROM写入完成
	        }
	        else
	        {
	            delay(50);                             //如果读取不成功，则每间隔100us尝试1次，尝试3次
	        }
	    }
        else//有电状态　带系统运行
        {
	        if(((os_sem_wait(Sem_I2C0_Mem_End, len * OS_DLY_10MS) != OS_R_TMO)
	           && (I2C_Mem[I2C0].Error == NO_ERROR))
	           || (try_num >= 2))
	        {
	            i2c0_rst();
	            break;
	        }
			os_dly_wait(OS_DLY_5MS);//azh
		}
    }

    i2c0_rst();
    delay(20);
//
    if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
    {
    	os_mut_release(Mut_I2C0_Mem);
    }

    return(I2C_Mem[I2C0].Error);
}

/******************************************************************************
** 函数名称: S32 write_i2c0(U32 obj_addr, U8 *snt_buff, U16 len)
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
S32 write_i2c0 (U32 obj_addr, U8 *snt_buff, U16 len)
{
    U8 try_num;
    U32 obj_memory;
    S32 wait_time;
    S32 res;

    obj_memory = (obj_addr & 0xFF000000);
    obj_addr &= 0x00FFFFFF;
    res = check_i2c0_addr(obj_addr, obj_memory, len);                //检查地址和长度
    if(res != NO_ERROR)
    {
        return res;
    }
//azh 110527------------------------------
	if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
	{
		if(os_mut_wait(Mut_I2C0_Mem, len * OS_DLY_10MS) == OS_R_TMO)
		{
			return(0x03);
		}
	}

//--------------------------------
//    while(len)
//    {
        for(try_num = 0; try_num < 3; try_num++)
        {
            i2c0_init();

            if(obj_memory == BU97950_HEAD_ADDR)        //fill in the slave address and sub address
            {
                I2C_Mem[I2C0].Slave = BU97950_ADDR;
                I2C_Mem[I2C0].Addr_Num = 1;
            }
            else
            {
			    if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
				{
			    	os_mut_release(Mut_I2C0_Mem);
			    }
				return ADDR_ERROR;
            }

            I2C_Mem[I2C0].End_Flag = I2C_IDLE;
            I2C_Mem[I2C0].Operation = I2C_WRITE_OP;
            I2C_Mem[I2C0].Data_Ptr = snt_buff+1;//snt_buff;	//LCD驱动芯片没有子地址,所以把里面的一个数据当作地址发送
            I2C_Mem[I2C0].Sub_Addr = snt_buff[0];//obj_addr;
            I2C_Mem[I2C0].Num = len-1;//len;
            I2C_Mem[I2C0].Error = NO_ERROR;
			
			if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
			{
				os_sem_init(Sem_I2C0_Mem_End, 0);
			}
			
            I2C_Mem[I2C0].State = I2C_SLA_WR;
            I2C0CONSET = ((1 << 5) | (1 << 6));

			if(CON_CPU_NORM_MODE != cpu_run_mode())//掉电　不带系统运行模式
			{
	            wait_time = (len + 4) * 10 * (1000000 / I2C0_FREQ) * 10;   //等待10倍的理论通讯时间
	            while(wait_time--)                      //等待I2C操作完成
	            {
	                if(I2C_Mem[I2C0].End_Flag == I2C_END)
	                {
	                    break;
	                }
	                delay(1);                           //延时约1uS
	            }
	            if(wait_time < 0)
	            {
	                I2C_Mem[I2C0].Error = TIME_OVF;     //等待超时
	            }
	
	            delay(10);                               //等待停止位时序完成
	            if((I2C_Mem[I2C0].Error == NO_ERROR) || (try_num >= 2))         //没有错误发生或已经尝试3次
	            {
	                break;
	            }
	            i2c0_rst();
	            delay(250);                            //如果写入不成功，则每间隔5ms尝试1次，尝试3次
	        }			
			else
			{
	            if(((os_sem_wait(Sem_I2C0_Mem_End, len * OS_DLY_10MS) != OS_R_TMO)
	               &&(I2C_Mem[I2C0].Error == NO_ERROR))
	               || (try_num >= 2))
	            {
	                i2c0_rst();
	                break;
	            }
	            os_dly_wait(OS_DLY_5MS);   //如果写入不成功，则每间隔5ms，尝试3次
	        }
        }

        i2c0_rst();
//    }
	if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
	{
    	os_mut_release(Mut_I2C0_Mem);
    }

    return(I2C_Mem[I2C0].Error);
}

/******************************************************************************
** 函数名称: S32 check_i2c0_addr(U32 obj_addr, U32 obj_memory, U16 len)
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
S32 check_i2c0_addr (U32 obj_addr, U32 obj_memory, U16 len)
{
    if((len == 0) || (len > 64))
    {
        return LEN_OVF;
    }

//    if(obj_memory == BU97950_HEAD_ADDR)//有可能夹杂着命令和数据所以长度不好定
//    {
//        if((obj_addr + len) > BU97950_CAPACITY)
//        {
//            return LEN_OVF;
//        }
//    }
//    else
//    {
//        return ADDR_ERROR;
//    }
    return NO_ERROR;
}

/******************************************************************************
** 函数名称: void i2c0_init(void)
** 函数功能: Initializes I2C0 peripheral control and registers to their default reset values
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void i2c0_init (void)
{
    PCONP |= (1 << PCI2C0);             //enable the clock
    i2c0_rst();                         //禁止I2C0及其中断

    if((PINSEL1 & ((0x03u << 24) | (0x03u << 22))) != ((P0_28_FNUC << 24) | (P0_27_FNUC << 22)))
    {
        PINSEL1 &= ~((0x03u << 24) | (0x03u << 22));              //先清除相关bits
        PINSEL1 |= ((P0_28_FNUC << 24) | (P0_27_FNUC << 22));     //configure the SCL0 and SDA0 as the alternate function 1 configuration
    }
//azh P0_27/28 no mode select
//    PINMODE1 &= ~((0x03u << 24) | (0x03u << 22));                 //先清除相关bits
//    PINMODE1 |= ((P0_28_MODE << 24) | (P0_27_MODE << 22));        //SCL0 and SDA0 has neither pull-up nor pull-down registers
//    PINMODE_OD0 |= (P0_SDA0 | P0_SCL0);                           //设置引脚开漏模式

	if(CON_CPU_NORM_MODE != cpu_run_mode())//掉电　不带系统运行模式
	{
	    PCLKSEL0 &= ~(0x03u << 14);
	    PCLKSEL0 |= (PD_FPCLK_I2C0_DIV << 14);         //设置外设时钟分频系数1/8
	
	    I2C0SCLH = PD_FPCLK_I2C0 / I2C0_FREQ / 2;      //init the speed of the I2C0 bus
	    I2C0SCLL = PD_FPCLK_I2C0 / I2C0_FREQ / 2;
	}
	else//有电状态　带系统运行
	{
	    PCLKSEL0 &= ~(0x03u << 14);
	    PCLKSEL0 |= (FPCLK_I2C0_DIV << 14);         //设置外设时钟分频系数1/8
	
	    I2C0SCLH = FPCLK_I2C0 / I2C0_FREQ / 2;      //init the speed of the I2C0 bus
	    I2C0SCLL = FPCLK_I2C0 / I2C0_FREQ / 2;		
	}

//    ENABLE_ISR(NVIC_I2C0, PRIO_I2C0);           //set I2C0 interrupt parameter
//isr_set(NVIC_I2C0, PRIO_I2C0);
}

/******************************************************************************
** 函数名称: void i2c0_rst(void)
** 函数功能: disbale I2C0 functionality and interrupt
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
******************************************************************************/
void i2c0_rst (void)
{
//    DISABLE_ISR(NVIC_I2C0);             //disbale I2C0 interrupt
//isr_clr(NVIC_I2C0);
    I2C0CONCLR = 0x6C;                  //disable the I2C0 functionality, clear STA/SI/AA
}

/******************************************************************************
** 函数名称: void I2C0_IRQHandler(void)
** 函数功能: process the I2DAT reg
**           process the global variables and operations
**           clear the interrupt flag
** 入口参数: 无
** 返 回 值: 无
** 说    明: 无
**  2011-03-02 编译器报告堆栈最大深度20字节
******************************************************************************/
void I2C0_IRQHandler (void)
{
    switch(I2C0STAT & 0xF8)
    {
    case 0x08:                                          //start condition has been transmitted, waiting to send SLA+W
    case 0x10:                                          //repeated start has been sent, waiting to send SLA+R
        if(I2C_Mem[I2C0].State == I2C_SLA_WR)
        {
            I2C0DAT = (I2C_Mem[I2C0].Slave & 0xFE);     //send the SLA+W
            I2C0CONCLR = 0x28;                          //clear SI and STA
            if(I2C_Mem[I2C0].Addr_Num == 2)             //检查地址是双字节还是单字节
            {
                I2C_Mem[I2C0].State = I2C_SUBADDR_HIGH; //waiting to send the MSB of the sub address
            }
            else
            {
                I2C_Mem[I2C0].State = I2C_SUBADDR_LOW;  //waiting to send the LSB of the sub address
            }
        }
        else if(I2C_Mem[I2C0].State == I2C_SLA_RD)
        {
            I2C0DAT = (I2C_Mem[I2C0].Slave | 0x01);     //send the SLA+R
            I2C0CONCLR = 0x28;                          //clear SI and STA
            I2C_Mem[I2C0].State = I2C_RCV_DATA;         //waiting to send the MSB of the sub address
        }
        else
        {
            I2C_Mem[I2C0].Error = STATE_ERR1;           //state is abnormal
        }
        break;
    case 0x18:                                          //previous state is state 0x08, SLA+W has been transmitted, ACK received
    case 0x28:
        if(I2C_Mem[I2C0].State == I2C_SUBADDR_HIGH)
        {
            I2C0DAT = (U8)(I2C_Mem[I2C0].Sub_Addr >> 8);
            I2C0CONCLR = 0x28;                          //clear SI and STA
            I2C_Mem[I2C0].State = I2C_SUBADDR_LOW;
        }
        else if(I2C_Mem[I2C0].State == I2C_SUBADDR_LOW)
        {
            I2C0DAT = (U8)I2C_Mem[I2C0].Sub_Addr;
            if(I2C_Mem[I2C0].Operation == I2C_READ_OP)
            {
                I2C0CONCLR = 0x28;                      //clear SI and STA
                I2C_Mem[I2C0].State = I2C_SLA_SEND_RD;
            }
            else if(I2C_Mem[I2C0].Operation == I2C_WRITE_OP)
            {
                I2C0CONCLR = 0x28;                      //clear SI and STA
                I2C_Mem[I2C0].State = I2C_SEND_DATA;
            }
            else
            {
                I2C_Mem[I2C0].Error = STATE_ERR2;
            }
        }
        else if(I2C_Mem[I2C0].State == I2C_SLA_SEND_RD)
        {
            I2C0CONSET = (1 << 5);                      //STA again
            I2C0CONCLR = 0x08;                          //clear SI and STA
            I2C_Mem[I2C0].State = I2C_SLA_RD;           //waiting to send SLA+R
        }
        else if(I2C_Mem[I2C0].State == I2C_SEND_DATA)
        {
            if(I2C_Mem[I2C0].Num > 0)
            {
                I2C0DAT = *I2C_Mem[I2C0].Data_Ptr++;
                I2C_Mem[I2C0].Num--;
                I2C0CONCLR = 0x28;                      //clear SI and STA
            }
            else
            {
                I2C0CONSET = (1 << 4);                  //STO
                I2C0CONCLR = 0x28;                      //clear SI and STA
                I2C_Mem[I2C0].End_Flag = I2C_END;       //总线操作完成
                if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
                {
                	isr_sem_send(Sem_I2C0_Mem_End);
                }
            }
        }
        else
        {
            I2C_Mem[I2C0].Error = STATE_ERR3;
        }
        break;

    case 0x40:                                          //SLA+R has been sent, config the AA to be sent
        if(I2C_Mem[I2C0].State == I2C_RCV_DATA)
        {
            if(I2C_Mem[I2C0].Num <= 1)
            {
                I2C0CONCLR = 0x04;
            }
            else
            {
                I2C0CONSET = 0x04;
            }
            I2C0CONCLR = 0x28;                          //clear SI and STA
        }
        else
        {
            I2C_Mem[I2C0].Error = STATE_ERR4;
        }
        break;
    case 0x50:                                          //data received
        if(I2C_Mem[I2C0].State == I2C_RCV_DATA)
        {
            if(I2C_Mem[I2C0].Num != 0x00)
            {
                *I2C_Mem[I2C0].Data_Ptr++ = I2C0DAT;
                I2C_Mem[I2C0].Num--;
                if(I2C_Mem[I2C0].Num == 1)
                {
                    I2C0CONCLR = 0x2C;                  //STA, SI, AA = 0
                }
                else
                {
                    I2C0CONSET = 0x04;                  //AA = 1
                    I2C0CONCLR = 0x28;
                }
            }
            else
            {
                I2C_Mem[I2C0].Error = LEN_OVF;
            }
        }
        else
        {
            I2C_Mem[I2C0].Error = STATE_ERR5;
        }
        break;
    case 0x58:                                          //last byte received
        if(I2C_Mem[I2C0].State == I2C_RCV_DATA)
        {
            if(I2C_Mem[I2C0].Num == 1)
            {
                I2C_Mem[I2C0].Num--;
                *I2C_Mem[I2C0].Data_Ptr++ = I2C0DAT;
                I2C0CONSET = (1 << 4);                  //STO
                I2C0CONCLR = 0x28;                      //clear SI and STA
                I2C_Mem[I2C0].End_Flag = I2C_END;       //总线操作完成
                if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
				{  
                	isr_sem_send(Sem_I2C0_Mem_End);
                }
            }
            else
            {
                I2C_Mem[I2C0].Error = LEN_OVF;
            }
        }
        else
        {
            I2C_Mem[I2C0].Error = STATE_ERR6;
        }
        break;
    case 0x20:                                          //SLA+W is sent, !ACK received, P to be sent
    case 0x30:                                          //I2DAT is sent, !ACK received, P to be sent
    case 0x48:                                          //SLA+R is sent, !ACK received, P to be sent
        I2C0CONSET = (1 << 4);                          //STO
        I2C_Mem[I2C0].Error = STATE_ERR7;               //ACK received
        break;
    case 0x38:                                          // - SLA+R/W sent, arbitration lost
    case 0x68:                                          // -               arbitration lost
    case 0x78:                                          // -               arbitration lost
    case 0xB0:                                          // -               arbitration lost
        I2C_Mem[I2C0].Error = STATE_ERR8;               //arbitration lost
        break;
    default:
        I2C_Mem[I2C0].Error = STATE_ERR9;
        break;
    }

    if(I2C_Mem[I2C0].Error < 0)                         //Error occured and reset procedure
    {
        I2C0CONSET = (1 << 4);                          //STO
        I2C0CONCLR = 0x28;                              //clear SI and STA
        I2C_Mem[I2C0].End_Flag = I2C_END;               //总线操作完成
		if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
		{          
        	isr_sem_send(Sem_I2C0_Mem_End);
        }
    }
}
#endif
/******************************************************************************
    End Of File
******************************************************************************/

/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : i2c1.h
 Description    : I2C1�ӳ����ͷ�ļ�
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��    1.00        2010-07-22      snap           the original version
******************************************************************************/

#ifndef __I2C1_H
#define __I2C1_H

//----------------------------- macro definition ------------------------------
#define  I2C_NUM                2//1           //����ʹ�õ�I2C�������������ı����������

#define  I2C1_FREQ              200000      //I2C1 bus speed is defined as 200kbps

//----------------------------- type definition -------------------------------
typedef enum
{
    I2C0,
    I2C1
}I2C_INDEX;                                 //��ö��������I2C_NUM��أ�����Ӧ��I2C_NUMһ��

typedef enum
{
    I2C_READ_OP,
    I2C_WRITE_OP
}I2C_OP;

//-----------------------------------------------------------------------------
// I2C state definition
//-----------------------------------------------------------------------------
typedef enum
{
    I2C_IDLE,
    I2C_SLA_WR,
    I2C_SLA_RD,
    I2C_SUBADDR_HIGH,
    I2C_SUBADDR_LOW,
    I2C_SLA_SEND_RD,
    I2C_SEND_DATA,
    I2C_RCV_DATA,
    I2C_END
}I2C_STATUS;

//-----------------------------------------------------------------------------
// I2Cȫ�ֱ����ṹ����
//-----------------------------------------------------------------------------
typedef struct
{
    U8 Slave;
    U8 Addr_Num;
    VU8 End_Flag;
    VU8 State;
    VU8 Operation;
    VU8 *Data_Ptr;
    U16 Sub_Addr;
    VU16 Num;
    VS32 Error;
}I2C_STRUCT;

//-------------------------- functions declaration ----------------------------
__EXTERN S32 read_i2c1(U32 obj_addr, U8 *rcv_buff, U16 len);
__EXTERN S32 write_i2c1(U32 obj_addr, U8 *snt_buff, U16 len);
__EXTERN void I2C1_IRQHandler (void);

//--------------------------- variable declaration ----------------------------
__EXTERN I2C_STRUCT I2C_Mem[I2C_NUM];

__EXTERN    OS_MUT      Mut_I2C1_Mem;
__EXTERN    OS_MUT      Mut_EEPROM;

__EXTERN    OS_SEM      Sem_I2C1_Mem_End;

#endif//__I2C1_H

/******************************************************************************
    End Of File
******************************************************************************/

/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : file.h
 Description    : ȫ�ֵĶ�д������ͷ�ļ�
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��    1.00        2010-07-25      snap           the original version
******************************************************************************/

#ifndef __FILE_H
#define __FILE_H

//----------------------------- macro definition ------------------------------
#define FLASH_HEAD_ADDR         0x00000000          //���崦�����ڲ�Flash�ģ�������ʼ��ַ
#define FLASH_CAPACITY          (256 * 1024)        //�����ڲ�Flash������256KBytes
                                
#define IRAM1_HEAD_ADDR         0x10000000          //���崦�����ڲ�����RAM�ģ�������ʼ��ַ
#define IRAM1_CAPACITY          (32 * 1024)         //�����ڲ�����RAM������32KBytes
                                
#define IRAM2_HEAD_ADDR         0x2007c000          //���崦�����ڲ�AHB RAM�ģ�������ʼ��ַ
#define IRAM2_CAPACITY          (32 * 1024)         //�����ڲ�AHB RAM������32KBytes
                                
#define GPIO_HEAD_ADDR          0x2009c000          //���崦�����ڲ�GPIO�Ĵ����ģ�������ʼ��ַ
#define GPIO_CAPACITY           (16 * 1024)         //�����ڲ�GPIO�Ĵ���������16KBytes
                                
#define APB_HEAD_ADDR           0x40000000          //���崦�����ڲ�APB����Ĵ����ģ�������ʼ��ַ
#define APB_CAPACITY            (1024 * 1024)       //�����ڲ�APB����Ĵ���������1MBytes
                                
#define AHB_HEAD_ADDR           0x50000000          //���崦�����ڲ�AHB����Ĵ����ģ�������ʼ��ַ
#define AHB_CAPACITY            (2048 * 1024)       //�����ڲ�AHB����Ĵ���������2MBytes
                                
#define KERNEL_HEAD_ADDR        0xe0000000          //���崦����Cortex-M3��ؼĴ����ģ�������ʼ��ַ
#define KERNEL_CAPACITY         (1024 * 1024)       //����Cortex-M3��ؼĴ���������1MBytes

//-----------------------------------------------------------------------------
// ����洢����дʱ�����Ĵ���ĺ궨��
//-----------------------------------------------------------------------------
#define NO_ERROR                1           //����
#define LEN_OVF                -1           //���ݳ��ȴ���
#define ADDR_ERROR             -2           //������ַ����
#define TIME_OVF               -3           //�ȴ���ʱ
#define STATE_ERR1             -4           //I2C����״̬����
#define STATE_ERR2             -5           //I2C����״̬����
#define STATE_ERR3             -6           //I2C����״̬����
#define STATE_ERR4             -7           //I2C����״̬����
#define STATE_ERR5             -8           //I2C����״̬����
#define STATE_ERR6             -9           //I2C����״̬����
#define STATE_ERR7             -10          //I2C����״̬����
#define STATE_ERR8             -11          //I2C����״̬����
#define STATE_ERR9             -12          //I2C����״̬����
#define DATA_OVF               -13          //SSP���ݽ������
#define WREN_LATCH             -14          //д��ʹ�ܴ���
#define DATA_MISMATCH          -15          //д���������ԭʼ���ݲ�ƥ��

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

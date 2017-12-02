/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : dataflash.c
 Description    : DataFlash�Ķ�д����
 Related Files  : ssp1.c ������SSP1��صĲ�������
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��    1.00        2010-07-26      snap           the original version
******************************************************************************/

#include "..\inc\global_config.h"

//-------------------------------- �������� -----------------------------------

//------------------------------ ˽�б������� ---------------------------------

//------------------------------ ˽�к������� ---------------------------------
static U8 dataflash_get_status(U8 cs);
static S32 dataflash_wait_rdy(U8 cs);
static S32 dataflash_page_read(U8 cs, U32 page_addr);
static S32 dataflash_page_write(U8 cs, U32 page_addr);
static S32 dataflash_buff_write(U8 cs, U32 byte_addr, U8 *send_buff, U16 len);
static S32 dataflash_compare (U8 cs, U32 page_addr);
static void dataflash_wp(U8 control);
static void dataflash_cs(U8 control, U8 cs);

/******************************************************************************
** ��������: S32 read_dataflash(U32 obj_addr, U8 *rcv_buff, U16 len)
** ��������: read data from DataFlash
** ��ڲ���: obj_addr : object logic adrress
**           *rcv_buff: pointer for the receive buffer
**           len      : length of bytes for operation��ȡֵ��1~DATAFLASH_PAGE_SIZE * 2
** �� �� ֵ: indicating the result of the operation
**              1: normal
**             -1: length overflow, not allowed
**             -2: object memory not found
**             -3: waiting time overflow
**        -others: state error
** ˵    ��: ��
******************************************************************************/
S32 read_dataflash (U32 obj_addr, U8 *rcv_buff, U16 len)
{
    U8 cs;
    U8 op_code[5];
    U8 try_num;
    U32 page_addr, byte_addr;
    U32 obj_memory;
    S32 err;

    obj_memory = obj_addr & 0xff000000;
    obj_addr &= 0x00ffffff;

    if((len == 0) || (len > DATAFLASH_PAGE_SIZE * 2))
    {
        return LEN_OVF;
    }

    if((obj_addr + len) > DATAFLASH_CAPACITY)
    {
        return LEN_OVF;
    }

    if(obj_memory == DATAFLASH1_HEAD_ADDR)
    {
        cs = 1;
    }
    else if(obj_memory == DATAFLASH2_HEAD_ADDR)
    {
        cs = 2;
    }
    else
    {
        return ADDR_ERROR;
    }

    page_addr = (obj_addr / DATAFLASH_PAGE_SIZE);       //Compute the page address
    byte_addr = (obj_addr % DATAFLASH_PAGE_SIZE);       //Compute the byte address

    for(try_num = 0; try_num < 3; try_num++)            //��������ʱ����3��
    {
        err = dataflash_wait_rdy(cs);                   //��������Ƿ����
        if(err != NO_ERROR)
        {
            break;                                      //�ȴ���ʱ�����ش���
        }

        dataflash_cs(DATAFLASH_ENABLE, cs);
        dataflash_wp(DATAFLASH_RD);

        op_code[0] = 0x0b;                              //Continuous array read
        op_code[1] = (U8)(page_addr >> 6);
        op_code[2] = (U8)(page_addr << 2) + (U8)(byte_addr >> 8);
        op_code[3] = (U8)(byte_addr);
        op_code[4] = 0x00;
        err = ssp0_operation(op_code, op_code, 5, SSP_WR);
        if(err == NO_ERROR)
        {
            err = ssp0_operation(rcv_buff, rcv_buff, len, SSP_RD);
        }
        dataflash_cs(DATAFLASH_DISABLE, cs);

        if(err == NO_ERROR)
        {
            break;                                      //��ȡδ��������ʱ�˳�ѭ��
        }
    }

    return err;
}

/******************************************************************************
** ��������: S32 write_dataflash(U32 obj_addr, U8 *send_buff, U16 len)
** ��������: write data to DataFlash
** ��ڲ���: obj_addr  : object logic adrress
**           *send_buff: pointer for the send buffer
**           len       : length of bytes for operation��ȡֵ��1~DATAFLASH_PAGE_SIZE * 2
** �� �� ֵ: indicating the result of the operation
**              1: normal
**             -1: length overflow, not allowed
**             -2: object memory not found
**             -3: waiting time overflow
**        -others: state error
** ˵    ��: ��
******************************************************************************/
S32 write_dataflash (U32 obj_addr, U8 *send_buff, U16 len)
{
    U8 cs;
    U8 page_num;
    U8 try_num;
    U32 page_addr, byte_addr;
    U32 obj_memory;
    U32 page_end_len;
    U32 page_start_len;
    S32 err;

    obj_memory = obj_addr & 0xff000000;
    obj_addr &= 0x00ffffff;

    if((len == 0) || (len > DATAFLASH_PAGE_SIZE * 2))
    {
        return LEN_OVF;
    }

    if((obj_addr + len) > DATAFLASH_CAPACITY)
    {
        return LEN_OVF;
    }

    if(obj_memory == DATAFLASH1_HEAD_ADDR)
    {
        cs = 1;
    }
    else if(obj_memory == DATAFLASH2_HEAD_ADDR)
    {
        cs = 2;
    }
    else
    {
        return ADDR_ERROR;
    }

    page_addr = (obj_addr / DATAFLASH_PAGE_SIZE);       //Compute the page address
    byte_addr = (obj_addr % DATAFLASH_PAGE_SIZE);       //Compute the byte address

    if(byte_addr + len > DATAFLASH_PAGE_SIZE)
    {
        page_end_len = DATAFLASH_PAGE_SIZE - byte_addr;
        page_start_len = (byte_addr + len) % DATAFLASH_PAGE_SIZE;
        page_num = (byte_addr + len) / DATAFLASH_PAGE_SIZE - 1;
    }
    else
    {
        page_end_len = len;
        page_start_len = 0;
        page_num = 0;
    }

    dataflash_wp(DATAFLASH_WR);

    for(try_num = 0; try_num < 3; try_num++)                //д���һҳ�����ݣ���������ʱ����3��
    {
        err = dataflash_page_read(cs, page_addr);
        if(err != NO_ERROR)
        {
            continue;                   //д�뷢����������
        }
        err = dataflash_buff_write(cs, byte_addr, send_buff, page_end_len);
        if(err != NO_ERROR)
        {
            continue;                   //д�뷢����������
        }
        err = dataflash_page_write(cs, page_addr);          //Write first page
        if(err == NO_ERROR)
        {
            break;                      //д��δ���������˳�ѭ��
        }
    }
    if(try_num >= 3)
    {
        goto write_error;               //3������ʧ�ܣ����ش���
    }

    page_addr++;                        //��һҳ�����Ѿ�д�룬ҳ��ַ��'1'
    send_buff += page_end_len;

    while(page_num)
    {
        for(try_num = 0; try_num < 3; try_num++)            //д���м伸ҳ�����ݣ���������ʱ����3��
        {
            err = dataflash_buff_write(cs, 0x00, send_buff, DATAFLASH_PAGE_SIZE);
            if(err != NO_ERROR)
            {
                continue;               //д�뷢����������
            }
            err = dataflash_page_write(cs, page_addr);      //Write the middle pages
            if(err == NO_ERROR)
            {
                break;                  //д��δ���������˳�ѭ��
            }
        }
        if(try_num >= 3)
        {
            goto write_error;           //3������ʧ�ܣ����ش���
        }

        page_addr++;
        send_buff += DATAFLASH_PAGE_SIZE;
        page_num--;
    }

    if(page_start_len)                  //�ж��Ƿ���ڲ���һҳ��������Ҫд��
    {
        for(try_num = 0; try_num < 3; try_num++)            //д�����һҳ�����ݣ���������ʱ����3��
        {
            err = dataflash_page_read(cs, page_addr);
            if(err != NO_ERROR)
            {
                continue;               //д�뷢����������
            }
            err = dataflash_buff_write(cs, 0x00, send_buff, page_start_len);
            if(err != NO_ERROR)
            {
                continue;               //д�뷢����������
            }
            err = dataflash_page_write(cs, page_addr);      //Write the last page
            if(err == NO_ERROR)
            {
                break;                  //д��δ���������˳�ѭ��
            }
        }
    }

write_error:
    dataflash_wp(DATAFLASH_RD);
    return err;
}

/******************************************************************************
** ��������: S32 dataflash_page_read(U8 cs, U32 page_addr)
** ��������: A page of data transferred from the main memory to buffer
** ��ڲ���: cs       : chip select
**           page_addr: page address of DataFlash
** �� �� ֵ: indicating the result of the operation
**              1: normal
**             -3: waiting time overflow
**        -others: state error
** ˵    ��: ��
******************************************************************************/
S32 dataflash_page_read (U8 cs, U32 page_addr)
{
    U8 op_code[4];
    U8 try_num;
    S32 err;

    for(try_num = 0; try_num < 3; try_num++)        //��������ʱ����3��
    {
        err = dataflash_wait_rdy(cs);               //��������Ƿ����
        if(err != NO_ERROR)
        {
            break;                                  //�ȴ���ʱ�����ش���
        }

        dataflash_cs(DATAFLASH_ENABLE, cs);
        op_code[0] = 0x53;                          //53H--read to buffer1; 55H--read to buffer2
        op_code[1] = (U8)(page_addr >> 6);          //page addr: xxAAAAAA AAAAAAxx xxxxxxxx; x--don't care bit
        op_code[2] = (U8)(page_addr << 2);
        op_code[3] = 0x00;                          //don't care byte
        err = ssp0_operation(op_code, op_code, 4, SSP_WR);
        dataflash_cs(DATAFLASH_DISABLE, cs);

        if(err == NO_ERROR)
        {
            break;                                  //��ȡδ��������ʱ�˳�ѭ��
        }
    }

    return err;
}

/******************************************************************************
** ��������: S32 dataflash_page_write(U8 cs, U32 page_addr)
** ��������: Data written into buffer be programmed into the main memory
** ��ڲ���: cs       : chip select
**           page_addr: page address of DataFlash
** �� �� ֵ: indicating the result of the operation
**              1: normal
**             -3: waiting time overflow
**        -others: state error
** ˵    ��: ��
******************************************************************************/
S32 dataflash_page_write (U8 cs, U32 page_addr)
{
    U8 op_code[4];
    U8 try_num;
    S32 err;

    for(try_num = 0; try_num < 3; try_num++)        //��������ʱ����3��
    {
        err = dataflash_wait_rdy(cs);               //��������Ƿ����
        if(err != NO_ERROR)
        {
            break;                                  //�ȴ���ʱ�����ش���
        }

        dataflash_cs(DATAFLASH_ENABLE, cs);
        op_code[0] = 0x83;                          //83H--buffer1 to main memory; 86H--buffer2 to main memory
        op_code[1] = (U8)(page_addr >> 6);          //page addr: xxAAAAAA AAAAAAxx xxxxxxxx; x--don't care bit
        op_code[2] = (U8)(page_addr << 2);
        op_code[3] = 0x00;                          //don't care byte
        err = ssp0_operation(op_code, op_code, 4, SSP_WR);
        dataflash_cs(DATAFLASH_DISABLE, cs);

        err = dataflash_compare(cs, page_addr);     //�ȶ�д�������
        if(err == NO_ERROR)
        {
            break;                                  //д��δ��������ʱ�˳�ѭ��
        }
    }

    return err;
}

/******************************************************************************
** ��������: S32 dataflash_buff_write(U8 cs, U32 byte_addr, U8 *send_buff, U16 len)
** ��������: To load data into the standard DataFlash buffer(528 bytes)
** ��ڲ���: cs        : chip select
**           byte_addr : ��һ��page�ڵĵ�ַ
**           *send_buff: ���ݻ���
**           len       : ���ݳ���
** �� �� ֵ: indicating the result of the operation
**              1: normal
**             -3: waiting time overflow
**        -others: state error
** ˵    ��: ��
******************************************************************************/
S32 dataflash_buff_write (U8 cs, U32 byte_addr, U8 *send_buff, U16 len)
{
    U8 op_code[4];
    U8 try_num;
    S32 err;

    for(try_num = 0; try_num < 3; try_num++)        //��������ʱ����3��
    {
        err = dataflash_wait_rdy(cs);
        if(err != NO_ERROR)
        {
            break;
        }

        dataflash_cs(DATAFLASH_ENABLE, cs);
        op_code[0] = 0x84;                          //84H--buffer1 write; 87H--buffer2 write
        op_code[1] = 0x00;                          //don't care byte
        op_code[2] = (U8)(byte_addr >> 8);
        op_code[3] = (U8)(byte_addr);
        err = ssp0_operation(op_code, op_code, 4, SSP_WR);
        if(err == NO_ERROR)
        {
            err = ssp0_operation(send_buff, send_buff, len, SSP_WR);
        }
        dataflash_cs(DATAFLASH_DISABLE, cs);

        if(err == NO_ERROR)
        {
            break;                                  //д��δ��������ʱ�˳�ѭ��
        }
    }

    return err;
}

/******************************************************************************
** ��������: S32 dataflash_compare(U8 cs, U32 page_addr)
** ��������: Data be programmed into the main memory compare with buffer
** ��ڲ���: cs       : chip select
**           page_addr: page address of DataFlash
** �� �� ֵ: indicating the result of the operation
**              1: normal
**             -3: waiting time overflow
**            -15: data mismatch
** ˵    ��: ��
******************************************************************************/
S32 dataflash_compare (U8 cs, U32 page_addr)
{
    U8 op_code[4];
    U8 try_num;
    S32 err;

    for(try_num = 0; try_num < 3; try_num++)        //��������ʱ����3��
    {
        err = dataflash_wait_rdy(cs);               //��������Ƿ����
        if(err != NO_ERROR)
        {
            break;                                  //�ȴ���ʱ�����ش���
        }

        dataflash_cs(DATAFLASH_ENABLE, cs);
        op_code[0] = 0x60;                          //60H--Main Memory Page to Buffer 1 Compare; 61H--Main Memory Page to Buffer 2 Compare
        op_code[1] = (U8)(page_addr >> 6);          //page addr: xxAAAAAA AAAAAAxx xxxxxxxx; x--don't care bit
        op_code[2] = (U8)(page_addr << 2);
        op_code[3] = 0x00;                          //don't care byte
        err = ssp0_operation(op_code, op_code, 4, SSP_WR);
        dataflash_cs(DATAFLASH_DISABLE, cs);

        err = dataflash_wait_rdy(cs);               //��������Ƿ����
        if(err != NO_ERROR)
        {
            break;                                  //�ȴ���ʱ�����ش���
        }

        if(0 == (dataflash_get_status(cs) & (1 << 6)))      //���bit6��COMP��־
        {
            return NO_ERROR;                        //д��δ�������󣬷�����ȷ
        }
        err = DATA_MISMATCH;                        //���ݲ�ƥ�����
    }

    return err;
}

/******************************************************************************
** ��������: S32 dataflash_get_status(U8 cs)
** ��������: ��ȡDataFlash��״̬��
** ��ڲ���: cs: chip select
** �� �� ֵ: DataFlash��״̬��
** ˵    ��: �ڶ�ȡ���̷�������ʱ״̬�ַ��ء�0��
******************************************************************************/
U8 dataflash_get_status (U8 cs)
{
    U8 buffer[1];
    U8 try_num;
    S32 err;

    for(try_num = 0; try_num < 3; try_num++)        //��������ʱ����3��
    {
        dataflash_cs(DATAFLASH_ENABLE, cs);
        buffer[0] = 0xd7;                           //read the status register
        err = ssp0_operation(buffer, buffer, 1, SSP_WR);
        if(err == NO_ERROR)
        {
            err = ssp0_operation(buffer, buffer, 1, SSP_RD);
        }
        dataflash_cs(DATAFLASH_DISABLE, cs);

        if((err == NO_ERROR) && (buffer[0] != 0xff))
        {
            return(buffer[0]);                      //��ȡ����δ�������󣬷��ؼĴ���ֵ
        }
    }

    return 0x00;                                    //��ȡ���̷�������ʱ���״ֵ̬
}

/******************************************************************************
** ��������: void dataflash_cs(U8 control, U8 cs)
** ��������: ���ݲ���ʹ�ܻ���ܵ�һ��ڶ�ƬDataFlash
** ��ڲ���: control: ����--ʹ��/����
**           cs     : chip select
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void dataflash_cs (U8 control, U8 cs)
{
    PINSEL3 &= ~(0x03u << 12);          //��������bits
    PINSEL3 |= (P1_22_FNUC << 12);      //configure the CS signal GPIO
    PINMODE3 &= ~(0x03u << 12);         //��������bits
    PINMODE3 |= (P1_22_MODE << 12);     //configure the CS signal pull-up

#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
    PINMODE_OD0 &= ~P0_F1_CS1;          //�������ŷǿ�©ģʽ
    if(control == DATAFLASH_ENABLE)
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

#else//H_METER_PRJ,WLS_CUR_PRJ
    PINMODE_OD1 &= ~P1_F1_CS1;          //�������ŷǿ�©ģʽ

    PINSEL3 &= ~(0x03u << 10);          //��������bits
    PINSEL3 |= (P1_21_FNUC << 10);      //configure the CS signal GPIO
    PINMODE3 &= ~(0x03u << 10);         //��������bits
    PINMODE3 |= (P1_21_MODE << 10);     //configure the CS signal pull-up
    PINMODE_OD1 &= ~P1_F1_CS2;          //�������ŷǿ�©ģʽ

    if(control == DATAFLASH_ENABLE)
    {
        if(cs == 1)
        {
            FLS1_CS_EN();
            FLS2_CS_DIS();
        }
        else
        {
            FLS1_CS_DIS();
            FLS2_CS_EN();
        }
    }
    else
    {
        FLS1_CS_DIS();
        FLS2_CS_DIS();
    }
#endif
}

/******************************************************************************
** ��������: S32 dataflash_wait_rdy(U8 cs)
** ��������: �ȴ�DataFlash���������쳣���س�ʱ
** ��ڲ���: cs: chip select
** �� �� ֵ: indicating the result of the operation
**              1: normal
**             -3: waiting time overflow
** ˵    ��: ��
******************************************************************************/
S32 dataflash_wait_rdy (U8 cs)
{
    U8 k;

    for(k = 0; k < 200; k++)
    {
        if(dataflash_get_status(cs) & 0x80)
        {
            return NO_ERROR;            //DataFlash is ready
        }
        delay(1000);                    //��ʱ1mS
    }

    return TIME_OVF;                    //DataFlash is not ready
}

/******************************************************************************
** ��������: void dataflash_wp(U8 control)
** ��������: ���ݲ���������ֹ��һ��ڶ�ƬDataFlashд����
** ��ڲ���: control: ����--����/��ֹ
**           cs     : chip select
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void dataflash_wp (U8 control)
{
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
    PINMODE_OD0 &= ~(P0_F1_WP); //�������ŷǿ�©ģʽ
#else//H_METER_PRJ,WLS_CUR_PRJ
    PINSEL3 &= ~(0x03u << 20);          //��������bits
    PINSEL3 |= (P1_26_FNUC << 20);       //configure the WP signal GPIO
    PINMODE3 &= ~(0x03u << 20);         //��������bits
    PINMODE3 |= (P1_26_MODE << 20);      //configure the WP signal pull-down
    
    PINMODE_OD1 &= ~(P1_F1_WP); //�������ŷǿ�©ģʽ
#endif
    if(control == DATAFLASH_WR)
    {
        DF_WP_DIS();            //д��������
    }
    else
    {
        DF_WP_EN();             //д����ʹ��
    }
}

/******************************************************************************
    End Of File
******************************************************************************/

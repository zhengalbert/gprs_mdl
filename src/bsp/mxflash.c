/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : mxflash.c
 Description    : MXFlash��д���� azh EONFLASH оƬҲ�ɼ������� ע�⣺����������Ӧ�ò�������
 Related Files  :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��    1.10        2011-06-24      azh            MX��EON���������Լ��� �����ɶ�Ƭ ������ڲ�����һ��cs�Ĳ���
   1��    1.00        2010-08-26      snap           the original version
******************************************************************************/

#include "..\inc\global_config.h"

//-------------------------------- �������� -----------------------------------
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
	#define sspx_operation  ssp1_operation
#else
	#define sspx_operation  ssp0_operation
	#define FIRST_CHIP_ADDR_CSX			//����U8оƬ��ʼ�����ַ

#endif
//------------------------------ ˽�б������� ---------------------------------
static U8 mxflash_page_buf[MXFLASH_PAGE_SIZE];

//------------------------------ ˽�к������� ---------------------------------
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
** ��������: S32 read_mxflash(U32 obj_addr, U8 *rcv_buff, U16 len)
** ��������: read data from MXFlash
** ��ڲ���: obj_addr : object logic adrress
**           *rcv_buff: pointer for the receive buffer
**           len      : length of bytes for operation
** �� �� ֵ: indicating the result of the operation
**              1: normal
**             -1: length overflow, not allowed
**             -2: object memory not found
**             -3: waiting time overflow
**        -others: state error
** ˵    ��: ��
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

    for(try_num = 0; try_num < 3; try_num++)    //��������ʱ����3��
    {
        err = mxflash_wait_rdy(cs);               //��������Ƿ����
        if(err != NO_ERROR)
        {
            break;                              //�ȴ���ʱ�����ش���
        }

        mxflash_cs(MXFLASH_ENABLE, cs);
        op_code[0] = 0x03;                      //Continuous read command
        op_code[1] = (U8)(obj_addr >> 16);      //3�ֽڵ�ַ
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
            break;                              //��ȡδ��������ʱ�˳�ѭ��
        }
    }
//azh 140918
	os_mut_release(Mut_SSP_EON_Flash);

    return err;
}

/******************************************************************************
** ��������: S32 write_mxflash(U32 obj_addr, U8 *send_buff, U16 len)
** ��������: write data to MXFlash
** ��ڲ���: obj_addr  : object logic adrress
**           *send_buff: pointer for the send buffer
**           len       : length of bytes for operation
** �� �� ֵ: indicating the result of the operation
**              1: normal
**             -1: length overflow, not allowed
**             -2: object memory not found
**             -3: waiting time overflow
**            -14: write enable latch status error
** ˵    ��: ����д������ڴ洢��������������½���
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
    byte_addr = (obj_addr % MXFLASH_PAGE_SIZE);                     //����һҳ�е�ƫ�Ƶ�ַ
    page_addr = (obj_addr / MXFLASH_PAGE_SIZE);                     //����ҳ��ַ

    if(byte_addr != 0)                                              //�ж�д��Ŀ�ʼ��ַ�Ƿ�ҳ����
    {
        if((byte_addr + len) > MXFLASH_PAGE_SIZE)                   //�ж�д�볤���Ƿ����һҳ
        {
            page_end_len = 256;                                     //��һҳ��Ҫд��
            page_num = (byte_addr + len) / MXFLASH_PAGE_SIZE;       //������Ҫд��ҳ��
            memset(mxflash_page_buf, 0xff, byte_addr);              //д�����ݰ�ҳ���룬ǰ�治�㲿����'0xff'���
            memcpy((mxflash_page_buf + byte_addr), send_buff, (MXFLASH_PAGE_SIZE - byte_addr));     //�����û�����

            send_buff += (MXFLASH_PAGE_SIZE - byte_addr);           //���ݻ��������ƫ��д���һҳ�����ݳ���
            len -= (MXFLASH_PAGE_SIZE - byte_addr);                 //�ܵ����ݳ��ȼ�ȥд���һҳ�����ݳ���
        }
        else
        {
            page_end_len = byte_addr + len;                         //����ʵ����Ҫд��ĳ���
            page_num = 0;
            memset(mxflash_page_buf, 0xff, byte_addr);              //д�����ݰ�ҳ���룬ǰ�治�㲿����'0xff'���
            memcpy((mxflash_page_buf + byte_addr), send_buff, len);                                 //�����û�����
        }

        for(try_num = 0; try_num < 3; try_num++)                    //д���һҳ�����ݣ���������ʱ����3��
        {
            err = mxflash_enter_write_status(cs);                     //��������Ƿ����д����״̬
            if(err != NO_ERROR)
            {
                continue;
            }

            mxflash_cs(MXFLASH_ENABLE, cs);
            op_code[0] = 0x02;                                      //ҳд������
            op_code[1] = (U8)(page_addr >> 8);                      //2�ֽ�ҳ��ַ
            op_code[2] = (U8)page_addr;
            op_code[3] = 0;                                         //����ֽڱ���Ϊ'0'
            err = sspx_operation(op_code, op_code, 4, SSP_WR);
            if(err == NO_ERROR)
            {
                err = sspx_operation(mxflash_page_buf, mxflash_page_buf, page_end_len, SSP_WR);
            }
            mxflash_cs(MXFLASH_DISABLE, cs);

            if(err != NO_ERROR)
            {
                continue;                   //����д��������
            }
//azh ENQ64������û�п�����������
//            if(0 == (mxflash_get_security_register(cs) & (1 << 5)))
//            {
                break;                      //û�з���д�����˳�ѭ��
//            }
//            err = DATA_MISMATCH;            //���ݲ�ƥ�����
//            mxflash_clear_security_register(cs);                      //��������־
        }
        if(try_num >= 3)
        {
            goto write_error;               //3������ʧ�ܣ����ش���
        }

        page_addr++;                        //��һҳ�����Ѿ�д�룬ҳ��ַ��'1'
    }
    else                                    //д��Ŀ�ʼ��ַҳ��������
    {
        page_end_len = 256;                 //��һҳ��Ҫд��
        page_num = len / MXFLASH_PAGE_SIZE;                         //������Ҫд��ҳ��
        if((len % MXFLASH_PAGE_SIZE) != 0)
        {
            page_num++;                     //����в���һҳ�����ݣ�����д���ҳ������'1'
        }
    }

    if(len < MXFLASH_PAGE_SIZE)             //�ж�д�����һҳ���ݳ����Ƿ���һҳ
    {
        page_end_len = len;                 //��ʵ�ʳ���д��
    }

    while(page_num)                         //д������ֻ�е�һҳʱ�������ѭ��
    {
        for(try_num = 0; try_num < 3; try_num++)                    //д�����nҳ�����ݣ���������ʱ����3��
        {
            err = mxflash_enter_write_status(cs);                     //��������Ƿ����д����״̬
            if(err != NO_ERROR)
            {
                continue;
            }

            mxflash_cs(MXFLASH_ENABLE, cs);
            op_code[0] = 0x02;                                      //ҳд������
            op_code[1] = (U8)(page_addr >> 8);                      //2�ֽ�ҳ��ַ
            op_code[2] = (U8)page_addr;
            op_code[3] = 0;                                         //����ֽڱ���Ϊ'0'
            err = sspx_operation(op_code, op_code, 4, SSP_WR);
            if(err == NO_ERROR)
            {
                err = sspx_operation(send_buff, send_buff, page_end_len, SSP_WR);
            }
            mxflash_cs(MXFLASH_DISABLE, cs);

            if(err != NO_ERROR)
            {
                continue;                   //����д��������
            }
//azh EONû����������
//            if(0 == (mxflash_get_security_register(cs) & (1 << 5)))
//            {
                break;                      //û�з���д�����˳�ѭ��
//            }
//            err = DATA_MISMATCH;            //���ݲ�ƥ�����
//            mxflash_clear_security_register(cs);                      //��������־
//
//            if(try_num >= 2)
//            {
//                goto write_error;           //3������ʧ�ܣ����ش���
//            }
        }

        if(try_num >= 3)
        {
            goto write_error;           //3������ʧ�ܣ����ش���
        }

        page_addr++;                        //ҳ��ַ��'1'
        send_buff += MXFLASH_PAGE_SIZE;     //���ݻ��������ƫ��һҳ�����ݳ���
        len -= MXFLASH_PAGE_SIZE;           //�ܵ����ݳ��ȼ�ȥһҳ�����ݳ���
        if(len < MXFLASH_PAGE_SIZE)         //�ж�д�����һҳ���ݳ����Ƿ���һҳ
        {
            page_end_len = len;             //��ʵ�ʳ���д��
        }
        page_num--;                         //��д��ҳ����'1'
    }

write_error:
    mxflash_exit_write_status(cs);            //�˳�д����״̬

//azh 140918
	os_mut_release(Mut_SSP_EON_Flash);

    return err;
}

/******************************************************************************
** ��������: S32 mxflash_enter_write_status(U8 cs)
** ��������: ͨ��д��'WREN'�������'BP0~BP3 = 0'ʹMXFlash����д����״̬
** ��ڲ���: ��
** �� �� ֵ: indicating the result of the operation
**              1: normal
**             -3: waiting time overflow
**            -14: write enable latch status error
** ˵    ��: ��
******************************************************************************/
S32 mxflash_enter_write_status (U8 cs)
{
    U8 try_num;

    for(try_num = 0; try_num < 3; try_num++)                    //��������ʱ����3��
    {
        mxflash_write_enable(cs);                                 //ʹ��д��WEL = 1
        mxflash_status_write(cs, (1 << 1));                           //SRWD = 0; BP0~BP3 = 0; WEL = 1
        
        mxflash_write_enable(cs);                                 //ʹ��д��WEL = 1
        if(mxflash_get_status(cs) == (1 << 1))                    //���״̬�֣�д���Ƿ�����
        {
            return NO_ERROR;
        }
    }

    return WREN_LATCH;
}

/******************************************************************************
** ��������: S32 mxflash_exit_write_status(U8 cs)
** ��������: ͨ������'BP0~BP3 = 1'ʹMXFlash����д��ֹ״̬
** ��ڲ���: ��
** �� �� ֵ: indicating the result of the operation
**              1: normal
**             -3: waiting time overflow
** ˵    ��: ��
******************************************************************************/
S32 mxflash_exit_write_status (U8 cs)
{
    U8 try_num;
    S32 err;

    for(try_num = 0; try_num < 3; try_num++)        //��������ʱ����3��
    {
        mxflash_write_enable(cs);                     //ʹ��д��WEL = 1
        mxflash_status_write(cs, ((1 << 7) | (0x0f << 2) | (1 << 1)));            //SRWD = 1; BP0~BP3 = 1; WEL = 1
        err = mxflash_wait_rdy(cs);                   //��������Ƿ����
        if(err != NO_ERROR)
        {
            break;                                  //�ȴ���ʱ�����ش���
        }

        if(mxflash_get_status(cs) == ((1 << 7) | (0x0f << 2) | (1 << 1)))     //���״̬�֣�д���Ƿ��ֹ
        {
            return NO_ERROR;
        }
    }

    return WREN_LATCH;
}

/******************************************************************************
** ��������: S32 mxflash_wait_rdy(U8 cs)
** ��������: �ȴ�MXFlash���������쳣���س�ʱ
** ��ڲ���: ��
** �� �� ֵ: indicating the result of the operation
**              1: normal
**             -3: waiting time overflow
** ˵    ��: ��
******************************************************************************/
S32 mxflash_wait_rdy (U8 cs)
{
    U16 k;

//    for(k = 0; k < 600; k++)
    for(k = 0; k < 150; k++)
    {
        if(0 == (mxflash_get_status(cs) & 0x01))      //���WIPλ
        {
            return NO_ERROR;                        //MXFlash is ready
        }
//        delay(1000);                                //��ʱ1mS
//azh 140918 ���ӻ����� ����ϵͳ���� �����忴�Ź�
        os_dly_wait(OS_DLY_10MS);
    }

    return TIME_OVF;                                //MXFlash is not ready
}

/******************************************************************************
** ��������: S32 mxflash_sector_erase(U32 obj_addr)
** ��������: ����MXFlashһ�����������쳣���س�ʱ
** ��ڲ���: obj_addr: ��������������ַ
** �� �� ֵ: indicating the result of the operation
**              1: normal
**             -3: waiting time overflow
** ˵    ��: ��
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

//    obj_addr /= MXFLASH_SECTOR_SIZE;                //����������ַ

    for(try_num = 0; try_num < 3; try_num++)        //��������ʱ����3��
    {
        err = mxflash_enter_write_status(cs);         //��������Ƿ����д����״̬
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
            continue;                               //����д��������
        }
        break;//azh
//azh EONû����������
//        if(0 == (mxflash_get_security_register(cs) & (1 << 6)))
//        {
//            break;                                  //û�з��������˳�ѭ��
//        }
//        err = DATA_MISMATCH;                        //���ݲ�ƥ�����
//        mxflash_clear_security_register(cs);          //��������־
    }

    mxflash_exit_write_status(cs);                    //�˳�д����״̬

//azh 140918
	os_mut_release(Mut_SSP_EON_Flash);

    return err;
}

/******************************************************************************
** ��������: S32 mxflash_clear_security_register (U8 cs)
** ��������: ���MXFlash�İ�ȫ�Ĵ���bit6��bit5
** ��ڲ���: ��
** �� �� ֵ: indicating the result of the operation
**              1: normal
**             -3: waiting time overflow
**        -others: state error
** ˵    ��: ��
******************************************************************************/
/*
S32 mxflash_clear_security_register (U8 cs)
{
    U8 buffer[1];
    U8 try_num;
    S32 err;

    for(try_num = 0; try_num < 3; try_num++)        //��������ʱ����3��
    {
        mxflash_cs(MXFLASH_ENABLE, cs);
        buffer[0] = 0x30;                           //clear security register command
        err = sspx_operation(buffer, buffer, 1, SSP_WR);
        mxflash_cs(MXFLASH_DISABLE, cs);

        if(err == NO_ERROR)
        {
            break;                                  //û�д��������˳�ѭ��
        }
    }

    return err;
}
*/
/******************************************************************************
** ��������: U8 mxflash_get_security_register(U8 cs)
** ��������: ��ȡMXFlash�İ�ȫ�Ĵ���
** ��ڲ���: ��
** �� �� ֵ: MXFlash�İ�ȫ�Ĵ���ֵ
** ˵    ��: �ڶ�ȡ���̷�������ʱ״̬�ַ���'0xff'
******************************************************************************/
/*
U8 mxflash_get_security_register (U8 cs)
{
    U8 buffer[1];
    U8 try_num;
    S32 err;

    for(try_num = 0; try_num < 3; try_num++)        //��������ʱ����3��
    {
        err = mxflash_wait_rdy(cs);                   //��������Ƿ����
        if(err != NO_ERROR)
        {
            break;                                  //�ȴ���ʱ�����ش���
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
            return(buffer[0]);                      //��ȡ��û�д����������ؼĴ���ֵ
        }
    }

    return 0xff;                                    //��ȡ���̷�������ʱ����'0xff'
}
*/
/******************************************************************************
** ��������: S32 mxflash_write_enable(U8 cs)
** ��������: ����MXFlash����д����״̬��WEL = 1
** ��ڲ���: ��
** �� �� ֵ: indicating the result of the operation
**              1: normal
**             -3: waiting time overflow
**        -others: state error
** ˵    ��: ��
******************************************************************************/
S32 mxflash_write_enable (U8 cs)
{
    U8 buffer[1];
    U8 try_num;
    S32 err;

    for(try_num = 0; try_num < 3; try_num++)        //��������ʱ����3��
    {
        err = mxflash_wait_rdy(cs);                   //��������Ƿ����
        if(err != NO_ERROR)
        {
            break;                                  //�ȴ���ʱ�����ش���
        }

        mxflash_cs(MXFLASH_ENABLE, cs);
        buffer[0] = 0x06;                           //Write enable command
        err = sspx_operation(buffer, buffer, 1, SSP_WR);
        mxflash_cs(MXFLASH_DISABLE, cs);

        if(err == NO_ERROR)
        {
            break;                                  //û�д��������˳�ѭ��
        }
    }

    return err;
}

/******************************************************************************
** ��������: S32 mxflash_status_write(U8 cs, U8 byte)
** ��������: дMXFlash��״̬��
** ��ڲ���: byte: Ҫд���״̬��
** �� �� ֵ: indicating the result of the operation
**              1: normal
**             -3: waiting time overflow
**        -others: state error
** ˵    ��: ��
******************************************************************************/
S32 mxflash_status_write (U8 cs, U8 byte)
{
    U8 buffer[2];
    U8 try_num;
    S32 err;

    for(try_num = 0; try_num < 3; try_num++)        //��������ʱ����3��
    {
        err = mxflash_wait_rdy(cs);                   //��������Ƿ����
        if(err != NO_ERROR)
        {
            break;                                  //�ȴ���ʱ�����ش���
        }
        mxflash_wp(MXFLASH_WR);                     //azh Ҫ����CSǰ
        mxflash_cs(MXFLASH_ENABLE, cs);
        buffer[0] = 0x01;                           //write the status

        buffer[1] = byte & (~(1 << 6));             //Ҫд���״̬�֣�����bit6

        err = sspx_operation(buffer, buffer, 2, SSP_WR);
        mxflash_cs(MXFLASH_DISABLE, cs);
        mxflash_wp(MXFLASH_RD);
        
        if(err == NO_ERROR)
        {
            break;                                  //û�д��������˳�ѭ��
        }
    }

    return err;
}

/******************************************************************************
** ��������: U8 mxflash_get_status(U8 cs)
** ��������: ��ȡMXFlash��״̬��
** ��ڲ���: ��
** �� �� ֵ: MXFLASH��״̬��
** ˵    ��: �ڶ�ȡ���̷�������ʱ״̬�ַ���'0'
******************************************************************************/
U8 mxflash_get_status (U8 cs)
{
    U8 buffer[1];
    U8 try_num;
    S32 err;

    for(try_num = 0; try_num < 3; try_num++)        //��������ʱ����3��
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
                return(buffer[0]);                  //��ȡ��û�д����������ؼĴ���ֵ
            }
        }
    }

    return 0x00;                                    //��ȡ���̷�������ʱ���״ֵ̬
}

/******************************************************************************
** ��������: void mxflash_cs(U8 control, U8 cs)
** ��������: ���ݲ���ʹ�ܻ����MXFlash
** ��ڲ���: control: ����--ʹ��/����
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void mxflash_cs (U8 control, U8 cs)
{
/*
    PINSEL3 &= ~(0x03u << 0);           //��������bits
    PINSEL3 |= (P1_16_FNUC << 0);       //configure the CS signal GPIO
    PINMODE3 &= ~(0x03u << 0);          //��������bits
    PINMODE3 |= (P1_16_MODE << 0);      //configure the CS signal pull-up
    PINMODE_OD1 &= ~P1_MX_CS;           //�������ŷǿ�©ģʽ
    MXFLASH_CS_DIS();                   //��ֹMXFlash��Ƭѡ

    if(control == MXFLASH_ENABLE)
    {
        MXFLASH_CS_EN();                //����MXFLASH��Ƭѡ
    }
    else
    {
        MXFLASH_CS_DIS();               //��ֹMXFLASH��Ƭѡ
    }
*/

#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
    PINSEL0 &= ~(0x03u << 12);          //��������bits
    PINSEL0 |= (P0_6_FNUC << 12);      //configure the CS signal GPIO
    PINMODE0 &= ~(0x03u << 12);         //��������bits
    PINMODE0 |= (P0_6_MODE << 12);     //configure the CS signal pull-up
    PINMODE_OD0 &= ~P0_F1_CS1;          //�������ŷǿ�©ģʽ

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
    PINSEL3 &= ~(0x03u << 12);          //��������bits
    PINSEL3 |= (P1_22_FNUC << 12);      //configure the CS signal GPIO
    PINMODE3 &= ~(0x03u << 12);         //��������bits
    PINMODE3 |= (P1_22_MODE << 12);     //configure the CS signal pull-up
    PINMODE_OD1 &= ~P1_F1_CS1;          //�������ŷǿ�©ģʽ

    PINSEL3 &= ~(0x03u << 10);          //��������bits
    PINSEL3 |= (P1_21_FNUC << 10);      //configure the CS signal GPIO
    PINMODE3 &= ~(0x03u << 10);         //��������bits
    PINMODE3 |= (P1_21_MODE << 10);     //configure the CS signal pull-up
    PINMODE_OD1 &= ~P1_F1_CS2;          //�������ŷǿ�©ģʽ

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
            FLS2_CS_DIS();//azh CS2 �°�Ӳ��ѡ�е���U8 �����°汾��ƬFLASHֻ����һ��WP��
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
            FLS1_CS_DIS();//azh CS2 �°�Ӳ��0x72xxxxxx ѡ�е���U8 �����°汾��ƬFLASHֻ����һ��WP��
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
** ��������: void mxflash_wp(U8 control)
** ��������: ���ݲ���������ֹMXFlashд����
** ��ڲ���: control: ����--����/��ֹ
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void mxflash_wp (U8 control)
{
/*
    PINSEL4 &= ~(0x03u << 4);           //��������bits
    PINSEL4 |= (P2_2_FNUC << 4);        //configure the CS signal GPIO
    PINMODE4 &= ~(0x03u << 4);          //��������bits
    PINMODE4 |= (P2_2_MODE << 4);       //configure the CS signal pull-down
    PINMODE_OD2 &= ~P2_MX_WP;           //�������ŷǿ�©ģʽ

    if(control == MXFLASH_WR)
    {
        MXFLASH_WR_EN();                //����д��MXFLASH
    }
    else
    {
        MXFLASH_WR_DIS();               //��ֹд��MXFLASH
    }
*/
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
    PINSEL0 &= ~(0x03u << 10);          //��������bits
    PINSEL0 |= (P0_5_FNUC << 10);      //configure the WP signal GPIO
    PINMODE0 &= ~(0x03u << 10);         //��������bits
    PINMODE0 |= (P0_5_MODE << 10);     //configure the WP signal pull-down
    PINMODE_OD0 &= ~(P0_F1_WP);         //�������ŷǿ�©ģʽ
#else
    PINSEL3 &= ~(0x03u << 20);          //��������bits
    PINSEL3 |= (P1_26_FNUC << 20);      //configure the WP signal GPIO
    PINMODE3 &= ~(0x03u << 20);         //��������bits
    PINMODE3 |= (P1_26_MODE << 20);     //configure the WP signal pull-down
    PINMODE_OD1 &= ~(P1_F1_WP);         //�������ŷǿ�©ģʽ
#endif

    if(control == MXFLASH_WR)
    {
        DF_WP_DIS();    //д��������
    }
    else
    {
        DF_WP_EN();     //д����ʹ��
    }
}

/******************************************************************************
    End Of File
******************************************************************************/

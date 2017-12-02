/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : search.c
 Description    : ���豸��Ϣ ������ѹ���ַ ƥ��󶨵ĵ�ַ
-------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��    1.00        2011-08-17    azh          the original version
******************************************************************************/
#include    "..\inc\global_config.h"

#ifdef ZIGBEE_TEST//azh 171011����ʽ��ѹ��GPRSģ��//CODE_VER_TYPE!=GPRS_MODULE_PRJ		//���Ǵ�CPU_GPRSģ��
#define HVMETER_ADDR_NO_PROKEY//����˺궨�� �����ƥ���ַ����Ҫ��̼� ֱ�Ӱ����Ϳ���ƥ��

extern U16 os_time;                                     //ʹ��ϵͳ����

//------------------------------ ˽�б������� ---------------------------------
static const U8 identity[] = "T=Diste N=???????????? A=???????????? P=645-07 V=1.0-0000\r\n";
static const U8 ASCII_table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
//------------------------------ ˽�к������� -------------------------------
//static U8 send_identity_frame(U8 *ptr);
static void add_in_dis_addr_list(U8 *addr);
static void set_search_data(void);
/******************************************************************************
** ��������: void add_in_dis_addr_list(U8 *addr)
** ��������: ���������ĸ�ѹ���ͨ�ŵ�ַ����zigbeeģ���ַ���浽����ʾ����
** ��ڲ���: �������ĸ�ѹ���ͨ�ŵ�ַ����zigbeeģ���ַ
** �� �� ֵ: ���������򳤶�
** ˵    ��: ��
******************************************************************************/
void add_in_dis_addr_list (U8 *addr)
{
    U8 i;

    if(Dis_Search_Ctrl.totnum < CON_SEARCH_CMD_MAX)
    {
	    for(i=0; i<Dis_Search_Ctrl.totnum; i++)
	    {
	        if(memcmp(addr, &Dis_Search_Ctrl.addr[i][0], 14) == 0)//�˵�ַ�ѱ���
	        {
	            return;
	        }
	    }
	    addr[14] = addverify(addr, 14);
	    memcpy(&Dis_Search_Ctrl.addr[Dis_Search_Ctrl.totnum][0], addr, CON_SEARCH_ADDR_LEN);
	    Dis_Search_Ctrl.totnum++;
	}
}
/******************************************************************************
** ��������: void send_identity_frame (void)
** ��������: ���͡�/?��ָ��Ļ�Ӧ֡
** ��ڲ���: ��
** �� �� ֵ: ���������򳤶�
** ˵    ��: ��, �ڲ����Է���ʹ��256�ֽ��ڴ��, û�еĻ������˴η���
******************************************************************************/
void send_identity_frame (void)
{
    U8 buf[6], i;
    U8 len;
    U8 *ptr;

    ptr = alloc_comm_mem(MEM_256_TYPE, &len, GET_MEM_ERR23);
    if(NULL == ptr)
    {
//	    memcpy(gusCurMemErrFunName, "send_ide", 8);
	   	setmemgetnullid(GET_MEM_ERR23, gusCurMemErrFunName); 
        return;                                         //�˴��ñ�ʶ���Ժ�...
    }

    memset(ptr + ZIGBEE_RET_ADDR_OFFSET, 0xff, 8);      //�㲥��ַ
    len = sizeof(identity) - 1;
    memcpy(ptr + DATA_OFFSET, identity, len);
//    file_read(PAR_HVMNUM_04000402_00_ADDR, buf, PAR_HVMNUM_04000402_TOT_LEN);
    memcpy(buf, gMatch_HVMeter_Addr, PAR_HVMETER_ADR_LEN);//azh 120316
    for(i = 0; i < 6; i++)
    {
        ptr[DATA_OFFSET + 10 + i * 2] = ASCII_table[buf[5 - i] >> 4];
        ptr[DATA_OFFSET + 11 + i * 2] = ASCII_table[buf[5 - i] & 0x0f];
    }
//    file_read(PAR_HVMADDR_04000401_00_ADDR, buf, PAR_HVMADDR_04000401_TOT_LEN);
    memset(buf, 0, PAR_HVMETER_ADR_LEN);
    memcpy(&buf[0], stTerRunPara.stTerAddr.ucDevAddr, 2);    
    buf[2] = stTerRunPara.stTerRegionAddr.ucRegionAddr[1];
    buf[3] = stTerRunPara.stTerRegionAddr.ucRegionAddr[0];
    for(i = 0; i < 6; i++)
    {
        ptr[DATA_OFFSET + 25 + i * 2] = ASCII_table[buf[5 - i] >> 4];
        ptr[DATA_OFFSET + 26 + i * 2] = ASCII_table[buf[5 - i] & 0x0f];
    }
    send_protocol_frame(ptr, ZIGBEE_CHL, len, MEM_256_TYPE);
}
/******************************************************************************
** ��������: U8 analyse_identity_frame(U8 *in_ptr)
** ��������: �ȶ�zigbeeĿ���ַ �����ȫ0��ÿ����ƥ��һ�� ��0��15���Ӽ��һ��
** ��ڲ���: �ӵ�ָ��Ļ���ָ��
** �� �� ֵ: ����֡��û�еõ�����__TRUE�Ѿ�����__FALSEû�д���
** ˵    ��: "T=Meter N=444444444444 A=111111111111 P=645-07 V=1.0-0000"(��������˳����)
******************************************************************************/
U8 analyse_identity_frame (U8 *in_ptr)
{
    U8 len, i;
    U8 data[CON_SEARCH_ADDR_LEN];
    U8 *ptr = NULL;

    len = in_ptr[0] + (in_ptr[1]<<8);
    if(len > 128)
    {
        return __FALSE;
    }

    memset(data, 0, CON_SEARCH_ADDR_LEN);
    memcpy(data, "T=Meter", 7);
//azh debug 
//    memcpy(data1, "T=Diste", 7);
//    if((memcmp(data, in_ptr+DATA_OFFSET, 7) == 0)||(memcmp(data1, in_ptr+DATA_OFFSET, 7) == 0))        //���豸��Ϣ    
    
    if(memcmp(data, in_ptr+DATA_OFFSET, 7) == 0)        //���豸��Ϣ
    {
#ifdef LCD_VALID
        if(CON_SEARCH_RUN_STATE != Dis_Search_Ctrl.state)
        {
            return __FALSE;                             //���ڲ��Ǳ��ն����ѱ�, �����ѱ���ɵĲ����
        }
//azh 
#endif//LCD_VALID
        memcpy(data, "A=", 2);
        for(i = 2; i < len; i++)
        {
            if(memcmp(data, in_ptr+DATA_OFFSET + i, 2) == 0)
            {
                ptr = in_ptr + DATA_OFFSET + 2 + i;
                break;
            }
        }
        if((i < len) && ((i + 12) < len))               //�ҵ���ʶ�ҵ�ַ����
        {
            memset(data, 0, 6);
            for(i = 0; i < 12; i++)
            {
                data[i / 2] |= ((asc_to_hex(ptr[11 - i]) & 0x0f) << (4 * (i % 2)));
            }
            memcpy(data + 6, in_ptr + ZIGBEE_RET_ADDR_OFFSET, 8);
            add_in_dis_addr_list(data);
            ptr = alloc_comm_mem(MEM_256_TYPE, &len, GET_MEM_ERR30);   //׼����ӦOK, ֻ����һ��ʹ��, ����ʹ�ú���
            if(NULL == ptr)                             //��ʱ���䲻���ڴ�, �ü�¼
            {
//	    		memcpy(gusCurMemErrFunName, "analyse_", 8);
	   			setmemgetnullid(GET_MEM_ERR30, gusCurMemErrFunName); 
                return __TRUE;
            }
            len = 2;                                    //������ӦOK
            memcpy(ptr + ZIGBEE_RET_ADDR_OFFSET, in_ptr + ZIGBEE_RET_ADDR_OFFSET, 8);
            ptr[DATA_OFFSET] = 'O';
            ptr[DATA_OFFSET + 1] = 'K';
            send_protocol_frame(ptr, ZIGBEE_CHL, len, MEM_256_TYPE);
            return __TRUE;
        }
    }
    else if(len == 2)                                   //���豸��Ϣ
    {
        if((in_ptr[DATA_OFFSET] == '/') && (in_ptr[DATA_OFFSET+1] == '?'))
        {
            send_identity_frame();                      //�㲥Ӧ��

            GangMarkTime = os_time;                     //��¼�ӵ�/?ʱ��
            if(0 == GangMarkTime)                       //0����������ʾû�ӵ��� ��1������
            {
                GangMarkTime = 1;
            }
            return __TRUE;                              //�ڴ���ں����ͷ�
        }
        if(0 != GangMarkTime)                           //֮ǰ�н�/?�����ڵȴ�OK��Ӧ��
        {
            if(('O' == in_ptr[DATA_OFFSET]) && ('K' == in_ptr[DATA_OFFSET + 1]))
            {
                GangMarkTime = 0;                       //��ʾ�յ�OK��
                return __TRUE;
            }
        }
    }

    return __FALSE;
}
/******************************************************************************
** ��������: U8 check_match_valid(void)
** ��������: �ȶ�zigbeeĿ���ַ �����ȫ0��ÿ����ƥ��һ�� ��0��15���Ӽ��һ��
** ��ڲ���: ֻ�з��ֱ��ַ���� �Ż�ȥ�������� zigbee��ַ���� �������� ����ͨ����Ӧ֡��ȡzigbee��ַ
** �� �� ֵ:
**
** ˵    ��:
******************************************************************************/
U8 check_match_valid (void)
{
    U8 i;

    for(i=0; i<PAR_HVMETER_ADR_LEN; i++)
    {
        if(gMatch_HVMeter_Addr[i] != 0)//��Ϊ�ϵ�ʱ�����������������ʼ����ȫ0�� ������������
        {
            break;
        }
    }
    if(i >= PAR_HVMETER_ADR_LEN)
    {
        return  __FALSE;//�����ȫ0 �Ǿ����Ѿ����������� ����û��ȷ��״̬
    }
    if(gMatch_HVMeter_Addr[PAR_HVMETER_ADR_LEN] != addverify(gMatch_HVMeter_Addr, PAR_HVMETER_ADR_LEN))
    {
        get_match_addr();//���´�EEPROM�ж�ȡƥ���ַ ������� ����������������
        return __FALSE;
    }

//    for(i=0; i<PAR_HVMETER_ZIGBEE_ADR_LEN; i++)
//    {
//        if(gMatch_Zigbee_Addr[i] != 0)//��Ϊ�ϵ�ʱ�ĳ�ʼ����ȫ0��
//        {
//            break;
//        }
//    }
//    if(i >= PAR_HVMETER_ZIGBEE_ADR_LEN)
//    {
//        return  __FALSE;//�����ȫ0 �Ǿ����Ѿ����������� ����û��ȷ��״̬
//    }

    for(i=0; i<PAR_HVMETER_ZIGBEE_ADR_LEN; i++)
    {
        if(gMatch_Zigbee_Addr[i] != 0xff)//��Ϊ�ϵ�ʱ�ĳ�ʼ����ȫ0��
        {
            break;
        }
    }
    if(i >= PAR_HVMETER_ZIGBEE_ADR_LEN)
    {
        gucGetZigbeeAddrFlag = 1;//�����ȫFF ���ǰ����ַ��Ч˵��Ҫ��ȡ��Ӧ֡�е�zigbee��ַ
    }
    if(gMatch_Zigbee_Addr[PAR_HVMETER_ZIGBEE_ADR_LEN] != addverify(gMatch_Zigbee_Addr, PAR_HVMETER_ZIGBEE_ADR_LEN))
    {
        get_match_addr();//���´�EEPROM�ж�ȡƥ���ַ ������� ����������������
        //return __FALSE;
        return __TRUE;//���ֻ��zigbee��ַ���� ��ͬ������÷� ����ͨ����ȡ��Ӧ֡�е�zigbee��ַ
    }

    return __TRUE;
}
/******************************************************************************
** ��������: void send_match_module_frame(void)
** ��������: �ϵ��Լ15����zigbee�㲥�����ѯ ��ַ��Ե�zigbeeģ�� ��Ը�ѹ��
** ��ڲ���:
** �� �� ֵ:
**
** ˵    ��:
******************************************************************************/
#ifdef LCD_VALID
void send_match_module_frame (void)
{
    U8  i;
    U8  mem_type;
    U8  *p_send;
    U8  *mem_ptr;

    mem_ptr = alloc_comm_mem(MEM_64_TYPE, &mem_type, GET_MEM_ERR24);
    if(NULL == mem_ptr)
    {
//	    memcpy(gusCurMemErrFunName, "send_mat", 8);
	   	setmemgetnullid(GET_MEM_ERR24, gusCurMemErrFunName); 
        return;
    }

    p_send = mem_ptr + DATA_OFFSET;

    i = 0;
    p_send[i++] = '/';
    p_send[i++] = '?';

    memset(&mem_ptr[ZIGBEE_RET_ADDR_OFFSET], 0xff, 8);//zigbee�㲥����ѯ�����豸��Ϣ
//    send_protocol_frame(mem_ptr, ZIGBEE_CHL, i, MEM_64_TYPE);
    send_wait_rev_frame(mem_ptr, ZIGBEE_CHL, i, MEM_64_TYPE, CON_TER_SYNFLASH);
}
#endif//LCD_VALID
/******************************************************************************
** ��������: void init_search_data(void)
** ��������: �ϵ��ʼ���Զ����ñ��ַ��ر���
** ��ڲ���:
**
**
** �� �� ֵ:
** ˵    ��:
******************************************************************************/
void init_search_data (void)
{
#ifdef LCD_VALID//��zigbeeģ�飬��������ʾ�������޷�ͨ����������ʾ���Ѹ�ѹ��
    Dis_Search_Ctrl.state = CON_SEARCH_IDLE_STATE;
    Dis_Search_Ctrl.totnum = 0;
    Dis_Search_Ctrl.dis_sn = 0;
    Dis_Search_Ctrl.pre_key = CON_KEYBIT_NO;
    memset(Dis_Search_Ctrl.addr, 0, (CON_SEARCH_CMD_MAX*CON_SEARCH_ADDR_LEN));
#endif//LCD_VALID

    gucGetZigbeeAddrFlag = 0;

    if(CON_CPU_NORM_MODE == cpu_run_mode())//�е�״̬����ϵͳ����
    {
        get_match_addr();
    }
}
/******************************************************************************
** ��������: void get_match_addr(void)
** ��������: ��EEPROM����ַ ������� ��������������� ������ȷ�����ַ ��Ҫ�ֹ����ȷ��
** ��ڲ���:
**
**
** �� �� ֵ:
** ˵    ��:
******************************************************************************/
void get_match_addr (void)
{
#ifdef ZIGBEE_TEST
    U8 crc,i;
//����жϸ�ѹ���ַ��zigbee��ַУ���벻�� �����������
    file_read(PAR_HVMETER_ADR_ADDR, gMatch_HVMeter_Addr, PAR_HVMETER_ADR_LEN+1);
    crc = addverify(gMatch_HVMeter_Addr, PAR_HVMETER_ADR_LEN);
    if(crc != gMatch_HVMeter_Addr[PAR_HVMETER_ADR_LEN])
    {
        set_search_data();//ֻ�и�ѹ���ַ���ԲŻ�ȥ����
        memset(gMatch_HVMeter_Addr, 0, 6);
    }

    file_read(PAR_HVMETER_ZIGBEE_ADR_ADDR, gMatch_Zigbee_Addr, PAR_HVMETER_ZIGBEE_ADR_LEN+1);
    crc = addverify(gMatch_Zigbee_Addr, PAR_HVMETER_ZIGBEE_ADR_LEN);
    #ifdef LCD_VALID
    if(crc != gMatch_Zigbee_Addr[PAR_HVMETER_ZIGBEE_ADR_LEN])
    {

//        set_search_data();
        gucGetZigbeeAddrFlag = 1;
        memset(gMatch_Zigbee_Addr, 0xff, 8);//���ڵ�һ֡zigbee�㲥��ȡ��ַ
    }
    #else// ����ǵ���ʽ��ѹ��ֻ�������ñ��ַʱȥ���������zigbee��ַУ�����ǶԵģ��������ϵ����ȥ����
    if(crc == gMatch_Zigbee_Addr[PAR_HVMETER_ZIGBEE_ADR_LEN])
    {
        for(i=0; i<PAR_HVMETER_ZIGBEE_ADR_LEN; i++)
        {
            if(gMatch_Zigbee_Addr[i] != 0xff)//��Ϊ�ϵ�ʱ�ĳ�ʼ����ȫ0��
            {
                break;
            }
        }
        if(i >= PAR_HVMETER_ZIGBEE_ADR_LEN)
        {
            gucGetZigbeeAddrFlag = 1;//�����ȫFF ���ǰ����ַ��Ч˵��Ҫ��ȡ��Ӧ֡�е�zigbee��ַ
        }
    }
    #endif//LCD_VALID

#endif    
}
/******************************************************************************
** ��������: void set_search_data(void)
** ��������: �����Զ�����������ز���
** ��ڲ���:
**
**
** �� �� ֵ:
** ˵    ��:
******************************************************************************/
void set_search_data (void)
{
#ifdef LCD_VALID////azh 171011ֻ�д���ʾ�Ĳ�Ҫȥ��ʼ��
//  if(CON_CPU_NORM_MODE == cpu_run_mode())//�е�״̬����ϵͳ����
//    {
        if(Dis_Ctrl.cur_mode == DIS_MODE_MATCH)
        {
            return;//�Ѿ���������
        }
        Dis_Ctrl.cur_mode = DIS_MODE_MATCH;

        Dis_Search_Ctrl.state = CON_SEARCH_RUN_STATE;
        Dis_Search_Ctrl.state_dly = DIS_SEARCH_INTERVAL_TIME;
        Dis_Search_Ctrl.pre_key = CON_KEYBIT_NO;
        Dis_Search_Ctrl.cmd_ct = CON_SEARCH_CMD_MAX;
        Dis_Search_Ctrl.totnum = 0;

        Dis_Search_Ctrl.dis_sn = 0;
        memset(Dis_Search_Ctrl.addr, 0, (CON_SEARCH_CMD_MAX*CON_SEARCH_ADDR_LEN));
//    }
#endif//#ifdef LCD_VALID
}
/******************************************************************************
** ��������: void judge_search_state(void)
** ��������: �ж�����Ǳ�̼����µ���� �������·�ҳ��ͬʱ���´���3�� ��100ms�����
** ��ڲ���:
**
**
** �� �� ֵ:
** ˵    ��:
******************************************************************************/
#ifdef LCD_VALID
void judge_search_state (void)
{
    if(Dis_Ctrl.cur_mode != DIS_MODE_MATCH)
    {
#ifdef HVMETER_ADDR_NO_PROKEY
            if((Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP)) == (CON_KEYBIT_DN | CON_KEYBIT_UP))//ͬʱ��ס���·�ҳ��
            {
                Dis_Search_Ctrl.state_dly++;
                if(Dis_Search_Ctrl.state_dly > DIS_SEARCH_JUDGE_TIME)
                {
                    set_search_data();
                }
            }
            else
            {
                Dis_Search_Ctrl.state_dly = 0;
            }
#else
        if(LOCAL_STATE_IS_HIG(CON_LOCAL_STATE_PROG))//��̼���־��������
        {
            if((Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP)) == (CON_KEYBIT_DN | CON_KEYBIT_UP))//ͬʱ��ס���·�ҳ��
            {
                Dis_Search_Ctrl.state_dly++;
                if(Dis_Search_Ctrl.state_dly > DIS_SEARCH_JUDGE_TIME)
                {
                    set_search_data();
                }
            }
            else
            {
                Dis_Search_Ctrl.state_dly = 0;
            }
        }
        else
        {
            Dis_Search_Ctrl.state_dly = 0;
        }
#endif//HVMETER_ADDR_NO_PROKEY
    }
}
#endif//LCD_VALID
/******************************************************************************
** ��������: void store_match_addr(U8 index)
** ��������: �����ѹ���ͨ�ŵ�ַ����zigbeeģ��MAC��ַ
**
** ��ڲ���:
**
**
** �� �� ֵ:
** ˵    ��:
******************************************************************************/
void store_match_addr (U8 index)
{
    if(index < Dis_Search_Ctrl.totnum)
    {
        memcpy(gMatch_HVMeter_Addr, &Dis_Search_Ctrl.addr[index][0], PAR_HVMETER_ADR_LEN);
        gMatch_HVMeter_Addr[PAR_HVMETER_ADR_LEN] = addverify(gMatch_HVMeter_Addr, PAR_HVMETER_ADR_LEN);
        file_write(PAR_HVMETER_ADR_ADDR, gMatch_HVMeter_Addr, PAR_HVMETER_ADR_LEN+1);

        memcpy(gMatch_Zigbee_Addr, &Dis_Search_Ctrl.addr[index][PAR_HVMETER_ADR_LEN], PAR_HVMETER_ZIGBEE_ADR_LEN);
        gMatch_Zigbee_Addr[PAR_HVMETER_ZIGBEE_ADR_LEN] = addverify(gMatch_Zigbee_Addr, PAR_HVMETER_ZIGBEE_ADR_LEN);
        file_write(PAR_HVMETER_ZIGBEE_ADR_ADDR, gMatch_Zigbee_Addr, PAR_HVMETER_ZIGBEE_ADR_LEN+1);
    }
}

/******************************************************************************
** ��������: void store_zigbee_addr(U8 *addr)
** ��������: �ر��ַ�Ѿ��е������ �����ѹ���zigbeeģ��MAC��ַ
**
** ��ڲ���:
**
**
** �� �� ֵ:
** ˵    ��:
******************************************************************************/
void store_zigbee_addr (U8 *addr)
{
    memcpy(gMatch_Zigbee_Addr, addr, PAR_HVMETER_ZIGBEE_ADR_LEN);
    gMatch_Zigbee_Addr[PAR_HVMETER_ZIGBEE_ADR_LEN] = addverify(gMatch_Zigbee_Addr, PAR_HVMETER_ZIGBEE_ADR_LEN);
    file_write(PAR_HVMETER_ZIGBEE_ADR_ADDR, gMatch_Zigbee_Addr, PAR_HVMETER_ZIGBEE_ADR_LEN+1);
}
/******************************************************************************
** ��������: void store_hvmeter_addr(U8 *addr)
** ��������: ͨ��ͨ�ſ����ñ��ַ 
**
** ��ڲ���:
**
**
** �� �� ֵ:
** ˵    ��:
******************************************************************************/
void store_hvmeter_addr (U8 *addr)
{
    memcpy(gMatch_HVMeter_Addr, addr, PAR_HVMETER_ADR_LEN);
    gMatch_HVMeter_Addr[PAR_HVMETER_ADR_LEN] = addverify(gMatch_HVMeter_Addr, PAR_HVMETER_ADR_LEN);
    file_write(PAR_HVMETER_ADR_ADDR, gMatch_HVMeter_Addr, PAR_HVMETER_ADR_LEN+1);
}
/******************************************************************************
** ��������: void handle_search_state(void)
** ��������: �������������״̬����ʾ����
**
** ��ڲ���:
**
**
** �� �� ֵ:
** ˵    ��:
******************************************************************************/
#ifdef LCD_VALID
void handle_search_state (void)
{
    switch(Dis_Search_Ctrl.state)
    {
        case CON_SEARCH_RUN_STATE://������: /? ״̬
        {
            if(Dis_Search_Ctrl.state_dly > DIS_SEARCH_INTERVAL_TIME)
            {
                Dis_Search_Ctrl.state_dly = DIS_SEARCH_INTERVAL_TIME;
            }
            if(Dis_Search_Ctrl.state_dly > 0)
            {
                Dis_Search_Ctrl.state_dly--;
            }
            else if(Dis_Search_Ctrl.state_dly == 0)
            {
                if(Dis_Search_Ctrl.cmd_ct > CON_SEARCH_CMD_MAX)
                {
                    Dis_Search_Ctrl.cmd_ct = CON_SEARCH_CMD_MAX;
                }
                if(Dis_Search_Ctrl.cmd_ct > 0)
                {
                    send_match_module_frame();
                    Dis_Search_Ctrl.state_dly = DIS_SEARCH_INTERVAL_TIME;
                    Dis_Search_Ctrl.cmd_ct--;
                }
                else if(Dis_Search_Ctrl.cmd_ct == 0)
                {
                    Dis_Search_Ctrl.state = CON_SEARCH_DISPLY_STATE;
                    Dis_Search_Ctrl.state_dly = DIS_SEARCH_KEY_TIME;
                    Dis_Search_Ctrl.pre_key = CON_KEYBIT_NO;
                }
            }
            break;
        }
        case CON_SEARCH_DISPLY_STATE://����ͽ��� ��ʾ�������ı��ַ ���԰�����ʾ ͬʱ���¾���ѡ��˱��
        {
            if((Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP)) == (CON_KEYBIT_DN | CON_KEYBIT_UP))//ͬʱ����
            {
                if(Dis_Search_Ctrl.state_dly > DIS_SEARCH_KEY_TIME)
                {
                    Dis_Search_Ctrl.state_dly = DIS_SEARCH_KEY_TIME;
                }
                if(Dis_Search_Ctrl.state_dly > 0)
                {
                    Dis_Search_Ctrl.state_dly--;
                }
                else
                {
                    store_match_addr(Dis_Search_Ctrl.dis_sn);//�����ѹ���ͨ�ŵ�ַ����zigbeeģ��MAC��ַ
                    Dis_Search_Ctrl.state = CON_SEARCH_FINISH_STATE;//���°���Ҫͬʱ�ͷź�
                    Dis_Search_Ctrl.state_dly = DIS_SEARCH_DIS_TIME;   
                    //azh 140919
			        RunPara_InitMeterPara(&stTerRunPara.stMeterPara[0]);
                    memcpy(stTerRunPara.stMeterPara[0].stMeterAddr.ucMeterAddr, gMatch_HVMeter_Addr, PAR_HVMETER_ADR_LEN);
                    file_write(F10_METERPARA_ADDR, (U8*)&stTerRunPara.stMeterPara[0], sizeof(STMETERPARA));	//��Ϊ�ǲ�����0�����Ե�ַû��ƫ��		        
                }
            }
            else
            {
                Dis_Search_Ctrl.state_dly = DIS_SEARCH_KEY_TIME;
                if((Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP)) == CON_KEYBIT_DN)
                {
                    Dis_Search_Ctrl.pre_key = CON_KEYBIT_DN;
                }
                else if((Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP)) == CON_KEYBIT_UP)
                {
                    Dis_Search_Ctrl.pre_key = CON_KEYBIT_UP;
                }
                else
                {
                    if(Dis_Search_Ctrl.pre_key == CON_KEYBIT_DN)
                    {
                        Dis_Search_Ctrl.dis_sn++;
                        if(Dis_Search_Ctrl.dis_sn >= Dis_Search_Ctrl.totnum)
                        {
                            Dis_Search_Ctrl.dis_sn = 0;
                        }
                        Dis_Ctrl.flash_flag |= BSN_CHG_SN_FLAG;
                    }
                    else if(Dis_Search_Ctrl.pre_key == CON_KEYBIT_UP)
                    {

                        if(Dis_Search_Ctrl.dis_sn > 0)
                        {
                            Dis_Search_Ctrl.dis_sn--;

                        }
                        else if(Dis_Search_Ctrl.dis_sn == 0)
                        {
                            Dis_Search_Ctrl.dis_sn = Dis_Search_Ctrl.totnum;
                            if(Dis_Search_Ctrl.totnum != 0)
                            {
                                Dis_Search_Ctrl.dis_sn--;
                            }
                        }
                        Dis_Ctrl.flash_flag |= BSN_CHG_SN_FLAG;
                    }
                    Dis_Search_Ctrl.pre_key = CON_KEYBIT_NO;
                }
            }
            break;
        }
        case CON_SEARCH_FINISH_STATE://������ַ�ͼ�����zigbee��ַ�� �����״̬ //���°���Ҫͬʱ�ͷź�
        {
            if((Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP)) != (CON_KEYBIT_DN | CON_KEYBIT_UP))
            {
                if(Dis_Search_Ctrl.state_dly > DIS_SEARCH_DIS_TIME)
                {
                    Dis_Search_Ctrl.state_dly = DIS_SEARCH_DIS_TIME;
                }
                if(Dis_Search_Ctrl.state_dly > 0)
                {
                    Dis_Search_Ctrl.state_dly--;
                }
                else
                {
                    init_search_data();

                    Dis_Ctrl.cur_mode = DIS_MODE_AUTO;
                    Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_AUTO];
                    Dis_Ctrl.flash_flag |= MOD_CHG_SN_FLAG;
                    Dis_Ctrl.cur_diret = DN_SN_DIRECT;
                    Dis_Ctrl.pre_key = CON_KEYBIT_NO;
                }
            }
            break;
        }
        case CON_SEARCH_IDLE_STATE:
            break;
        default:
        {
            init_search_data();
            break;
        }
    }
}
#endif//LCD_VALID
#endif//ZIGBEE_TEST
/******************************************************************************
                            End Of File
 *****************************************************************************/

/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : ProtocolParse.c
 Description    : ��վЭ�����
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-02-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

/******************************************************************************
** ��������: U8 vir_port_convert (U8 port)
** ��������: �˿�ת��
** ��ڲ���: U8 com : ʵ�ʴ���˿ں�
** �� �� ֵ: vport ����˿�����ֵ
**            
** ˵    ��:
******************************************************************************/
U8 com_to_vport_convert (U8 com)
{
    U8 vport;
    
    switch(com)
    {
        case RS4852_CHL:
            vport = CON_TERMSG_SOUR_RS232_0;
            break;    
        case RS4851_CHL:
            vport = CON_TERMSG_SOUR_RS232;
            break;    
        case IR_SOFT_CHL: 
            vport = CON_TERMSG_SOUR_IRDA;
            break;        
        case ZIGBEE_CHL:
            vport = CON_TERMSG_SOUR_ZIGBEE;//azh 171017 ����zigbee�������й�Լͨ��
			break;
        default: 
            vport = CON_TERMSG_SOUR_RS232;
            break;
    }
    return vport;
}
/******************************************************************************
** ��������: U8 vir_port_convert (U8 port)
** ��������: �˿�ת��
** ��ڲ���: vport ����˿�����ֵ
** �� �� ֵ: U8 port : ʵ�ʶ˿ں�
**            
** ˵    ��:
******************************************************************************/
U8 vport_to_com_convert (U8 vport)
{
    U8 com;
    
    switch(vport)
    {
        case CON_TERMSG_SOUR_RS232_0:
            com = RS4852_CHL;
            break;    
        case CON_TERMSG_SOUR_RS232:
            com = RS4851_CHL;
            break;    
        case CON_TERMSG_SOUR_IRDA: 
            com = IR_SOFT_CHL;
            break;        
        case CON_TERMSG_SOUR_ZIGBEE:
            com = ZIGBEE_CHL;//azh 171017 ����zigbee�������й�Լͨ�� 
            break;				
        default: 
            com = RS4851_CHL;
            break;
    }
    return com;
}
/******************************************************************************
** ��������: U8 LocalCom_UpSendData (unsigned char *pBuf,unsigned short usLen,unsigned char ucMsgSource)
** ��������: ����GPRS��Ķ˿��� �����˿����з������ݷ��ͺ���
** ��ڲ���: vport ����˿�����ֵ
** �� �� ֵ: U8 port : ʵ�ʶ˿ں�
**            
** ˵    ��:
******************************************************************************/
U8 LocalCom_UpSendData (unsigned char *pBuf,unsigned short usLen,unsigned char ucMsgSource)
{
    U8 com,mem_type;
    U8 *send_buf;
    
    com = vport_to_com_convert(ucMsgSource);

   	send_buf = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR18);
	if(send_buf	!= NULL)
	{
		memcpy(send_buf+DATA_OFFSET, pBuf, usLen);
		send_buf[COM_NUM_OFFSET] = com;
        #ifdef ZIGBEE_TEST		//��ϴ�CPU_GPRSģ��
        if(com == ZIGBEE_CHL)
        {
            memcpy(send_buf+ZIGBEE_RET_ADDR_OFFSET, gMatch_aim_addr, 8);//azh 171017 ����ʽ��ѹ��GPRSģ�飬�������й�Լ������
        }
        #endif
		send_protocol_frame(send_buf, com, usLen, MEM_1536_TYPE);
		return __TRUE;
    }
    else
    {
        setmemgetnullid(GET_MEM_ERR0, gusCurMemErrFunName);
        return __FALSE;    
    }    
}
/******************************************************************************
** ��������: S8 deal_local_GWframe(U8 *InBuf)
** ��������: �������ع������й�Լ֡
** ��ڲ���: U8 index : �ڴ���±�
** �� �� ֵ: -1 ������
**            1 ����������
** ˵    ��:
******************************************************************************/
S8 deal_local_GWframe (U8 *InBuf)
{
    U8 ucSource;
    U16 revmaxlen;

    revmaxlen = InBuf[DATA_LEN_OFFSET] + InBuf[DATA_LEN_OFFSET+1]*0x100;

    if(RET_SUCCESS == Main_Protocol_CheckFrame(InBuf + DATA_OFFSET, revmaxlen))
    {
        ucSource = convert_com_index(InBuf);
        ucSource = com_to_vport_convert(ucSource);

        Main_Recv_Proc( InBuf + DATA_OFFSET, revmaxlen ,ucSource);
    }
    else//������֡
    {
        return -1;        
    }        

    return 1;
}
/******************************************************************************
                            End Of File
 *****************************************************************************/

/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : ProtocolParse.c
 Description    : 主站协议解析
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

/******************************************************************************
** 函数名称: U8 vir_port_convert (U8 port)
** 函数功能: 端口转换
** 入口参数: U8 com : 实际传入端口号
** 返 回 值: vport 虚拟端口索引值
**            
** 说    明:
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
            vport = CON_TERMSG_SOUR_ZIGBEE;//azh 171017 增加zigbee解析上行规约通道
			break;
        default: 
            vport = CON_TERMSG_SOUR_RS232;
            break;
    }
    return vport;
}
/******************************************************************************
** 函数名称: U8 vir_port_convert (U8 port)
** 函数功能: 端口转换
** 入口参数: vport 虚拟端口索引值
** 返 回 值: U8 port : 实际端口号
**            
** 说    明:
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
            com = ZIGBEE_CHL;//azh 171017 增加zigbee解析上行规约通道 
            break;				
        default: 
            com = RS4851_CHL;
            break;
    }
    return com;
}
/******************************************************************************
** 函数名称: U8 LocalCom_UpSendData (unsigned char *pBuf,unsigned short usLen,unsigned char ucMsgSource)
** 函数功能: 除了GPRS外的端口外 其他端口上行返回数据发送函数
** 入口参数: vport 虚拟端口索引值
** 返 回 值: U8 port : 实际端口号
**            
** 说    明:
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
        #ifdef ZIGBEE_TEST		//配合带CPU_GPRS模块
        if(com == ZIGBEE_CHL)
        {
            memcpy(send_buf+ZIGBEE_RET_ADDR_OFFSET, gMatch_aim_addr, 8);//azh 171017 电子式高压表GPRS模块，被动上行规约回数据
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
** 函数名称: S8 deal_local_GWframe(U8 *InBuf)
** 函数功能: 解析本地国网上行规约帧
** 入口参数: U8 index : 内存块下标
** 返 回 值: -1 ：错误
**            1 ：正常处理
** 说    明:
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
    else//非南网帧
    {
        return -1;        
    }        

    return 1;
}
/******************************************************************************
                            End Of File
 *****************************************************************************/

/******************************************************************************
 Copyright (C) 2009  R&D Institute of HuaCai Co., Ltd.
 Module     : protocol analyse function
 File Name    : protocol.c
 Description  :
 Author     :
 Build Date   :
 others     :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>   <time>    <author>    <contents>
   2��     2.00     2011-05-19   azh  modfy
   1��     1.00     2009-06-30   snap.gao   create
******************************************************************************/
#define  __PROTOCOL_C
/*---------------------------------------------------------------------------*/
#include "..\inc\global_config.h"

/*private functions defined in this file:------------------------------------*/
static U8 Protocol_Explain (U8 *buf);

#if CODE_VER_TYPE==WLS_CUR_PRJ			//������߲ɼ���
static S8 deal_with_watch_frame(U8 *InBuf);
static U8 get_protocolno_by_id(U8 protoco_id);
static U8 check_ret_addr(U8 *mpaddr, U8 *retaddr);
static U16 get_97di_by07di(U32 di07);
static U16 get_protocol_di(U8 protocol_no, U8 *InBuf);
#endif

//azh ��ʾ�ն��ⲿ�˿��յ�645-07���� ��Ҫ�������� ������͸�������ݱ�ʶ
#define SELF_DI_NUM		7
#ifdef ZIGBEE_TEST
const U32 dis_self_handle_table[SELF_DI_NUM] =
{
	0x04000300,		//�������һ���ֽ�
//	0x04000301,		//�Զ�ѭ����ʾ����(0-99)    
//	0x04000302,		//ÿ����ʾʱ��              
//	0x04000303,		//��ʾ����С��λ��          
//	0x04000304,		//��ʾ����(�������)С��λ��	
//	0x04000305,		//����ѭ����ʾ����(0-99)    
//
	0x04000700,		//�������һ���ֽ�
//	0x04000701,		//�����ͺ�����ͨ������������
//	0x04000702,		//�Ӵ�ʽ������ͨ������������
//	0x04000703,		//ͨ�ſ�1ͨ������������       
//	0x04000704,		//ͨ�ſ�2ͨ������������       
//	0x04000705,		//ͨ�ſ�3ͨ������������       

	0x04000C00,		//�������һ���ֽ�
//	0x04000C01,		//0������	
////......
//	0x04000C0A,		//9������

	0x04040100,		//�������һ���ֽ�
//	0x04040101,		//�Զ�ѭ����ʾ��1����ʾ����
////......
//	0x04040163,		//�Զ�ѭ����ʾ��99����ʾ����	

	0x04040200,		//�������һ���ֽ�
//	0x04040201,		//�Զ�ѭ����ʾ��1����ʾ����
////......
//	0x04040263,		//�Զ�ѭ����ʾ��99����ʾ����

	0x03300D00,		//�������һ���ֽ�
//	0x03300D00,		//�������ܴ���
//	0x03300D01,		//(��1��)�����Ǽ�¼
////......
//	0x03300D0A,		//(��10��)�����Ǽ�¼

	0x03300E00,		//�������һ���ֽ�
//	0x03300E00,		//����ť���ܴ���
//	0x03300E01,		//(��1��)����ť�м�¼
////......
//	0x03300E0A,		//(��10��)����ť�м�¼	
};
/******************************************************************************
** ��������: void dis_self_handle_di (U8 com)
** ��������: ��ʼ���˿ڽ���״̬��
** ��ڲ���: zigbee�յ������ݶ���ʾ�ն˴��� ���غ󻹻ᴦ����ֻ�����յ��������֣�
** �� �� ֵ: __TRUE:��ʾ�ն˴���  __FALSE:͸������ѹ��
** ˵    ��:
******************************************************************************/
U8 dis_self_handle_di (U8 com, U8 *rev_buf)
{
	U8 i,ret=__FALSE;
	U32 uldi;
	
	if(com == ZIGBEE_CHL)
	{
		return __TRUE;
	}

	switch(rev_buf[8])
	{
		case 0x11:
		case 0x12:
		case 0x14:	
		    uldi = (rev_buf[10]-0x33) + ((rev_buf[11]-0x33)<<8) + ((rev_buf[12]-0x33)<<16) + ((rev_buf[13]-0x33)<<24);
		    for(i=0; i<SELF_DI_NUM; i++)
		    {
		        if((uldi&0xffffff00) == dis_self_handle_table[i])
		        {
		        	if(((uldi&0xffffff00) == 0x04000300) && ((uldi&0x000000ff)>5))//�����ݱ�ʶ���漸��Ҫ͸��
		        	{
		        		return ret;
		        	}
		       		return __TRUE;	
		        }	
		    }
		    break;
		//��ʾ�ն��Լ�����
		case 0x13:	//��ͨ�ŵ�ַ
		case 0x15:	//дͨ�ŵ�ַ
		case 0x17:	//����ͨ������
		case 0x18:	//�޸�����
		case 0x1D:	//�๦�ܶ��������������
			return __TRUE;;		    
		default:
			break;
	}
    return ret;
}
#endif//ZIGBEE_TEST
/******************************************************************************
** ��������: void init_analyse_manage (U8 com)
** ��������: ��ʼ���˿ڽ���״̬��
** ��ڲ���:
** �� �� ֵ:
** ˵    ��:
******************************************************************************/
void init_analyse_manage (U8 com)
{
    if(com < MAX_TOTPORT_NUM)
    {
        memset(&stProtocol_Analyse[com].stAnalyse_ctrl, 0, sizeof(STANALYSE_TCB));    //��ʼ�����������
    }
    stProtocol_Analyse[com].ucWR_State = CON_PROTOCOL_DEFAULT_HANDLE;
}
/******************************************************************************
 Function name:  U8 Fill_645RelaySendto_Mbx (U8 incom, U8 *send_buf)
 Author     :
 Description:  ���ⲿ�˿���485 �����յ���645����֡ ֱ��ͨ��Zigbeeͨ��ת������ѹ��������
 Input      :   send_buf ���ݷ��ͻ���Ӵ����յ���ԭʼ����֡
 Return     :  __TRUE�ɹ�������__FALSEû�з���
 *****************************************************************************/
U8 Fill_645RelaySendto_Mbx (U8 incom, U8 *send_buf)
{
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//����CPU_GPRSģ��
    U8 *p_send,mem_type;
    U16  usLen;

    p_send = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR16);
    if(NULL != p_send)
    {
        if(incom < MAX_TOTPORT_NUM)
        {
            usLen = send_buf[0];// + (send_buf[1]<<8);
            memcpy(p_send+DATA_OFFSET, send_buf+DATA_OFFSET, usLen);

            memcpy(&p_send[ZIGBEE_RET_ADDR_OFFSET], gMatch_Zigbee_Addr, 8);//zigbee������Ŀ���ַ
            p_send[CON_ID_OFFSET] = incom;
			
            send_wait_rev_frame(p_send, ZIGBEE_CHL, usLen, MEM_256_TYPE, CON_RS485_645RELAY);
        }
        else
        {
            free_mem_block(p_send);
        }
    }
    else
    {
    	setmemgetnullid(GET_MEM_ERR0, gusCurMemErrFunName);
    }	
#endif
    return 1;
}
/******************************************************************************
 Function name:  U8 Fill_RelaySendto_Mbx (U8 ucPort)
 Author     :
 Description:  �����м�����֡������֡�����ʹ�������
 Input      :   send_buf ���ݷ��ͻ���
                com ѡ���͵Ĵ���COM0��COM3����̫��COM4��COM5
                len ���ݳ���
                type �ڴ������
                mode_type ����������
 Return     :  __TRUE�ɹ�������__FALSEû�з���
 *****************************************************************************/
U8 Fill_RelaySendto_Mbx (U8 ucPort)
{
    U8 *p_send,mem_type;
    U8  com=0xff;

    p_send = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR16);
    if(NULL != p_send)
    {
//        ucLen = stRelaySend.ucDataLen;
        if(stRelayDataBuf.usDataLen < (MEM_256_SIZE - DATA_OFFSET))
        {
            switch(ucPort)//ת����
            {
                case 0:
                    com = ZIGBEE_CHL;
                    break;
                case 1:
                    com = RS4851_CHL;
                    break;
                case 2:
                    com = RS4852_CHL;
                    break;
                default:
                    com = 0xff;
                    break;
            }
            if(com != 0xff)
            {
                p_send[CON_MODE_OFFSET] = CON_RS485CACHE_RELAY;
//                memcpy(p_send+DATA_OFFSET, stRelaySend.ucDataArea, stRelaySend.ucDataLen);
                memcpy(p_send+DATA_OFFSET, stRelayDataBuf.ucDataArea, stRelayDataBuf.usDataLen);
                memcpy(&stProtocol_Analyse[com].stRelayPara, &stRelaySend.stRelayPara, sizeof(STRELAYTASKPARA));

#ifdef ZIGBEE_TEST
                if(com == ZIGBEE_CHL)
                {
                    memcpy(&p_send[ZIGBEE_RET_ADDR_OFFSET], gMatch_Zigbee_Addr, 8);//zigbee������Ŀ���ַ
                }
                else
                {
                    memset(&p_send[ZIGBEE_RET_ADDR_OFFSET], 0xff, 8);//zigbee������Ŀ���ַ
                }
#else
				memset(&p_send[ZIGBEE_RET_ADDR_OFFSET], 0xff, 8);//
#endif				

                send_wait_rev_frame(p_send, com, stRelayDataBuf.usDataLen, MEM_256_TYPE, CON_RS485CACHE_RELAY);
            }
            else
            {
                free_mem_block(p_send);
            }
        }
    }
    else
    {
    	setmemgetnullid(GET_MEM_ERR0, gusCurMemErrFunName);
    }	
    return 1;
}
/******************************************************************************
 Function name:  U8 send_wait_rev_frame (U8 *send_buf, U8 com, U16 len, U8 memtype, U8 mode_type)
 Author     :
 Description:  ����������������֡������֡��������
 Input      :   send_buf ���ݷ��ͻ���
                com ѡ���͵Ĵ���COM0��COM3����̫��COM4��COM5
                len ���ݳ���
                type �ڴ������
                mode_type ����������
 Return     :  __TRUE�ɹ�������__FALSEû�з���
 *****************************************************************************/
U8 send_wait_rev_frame (U8 *send_buf, U8 com, U16 len, U8 memtype, U8 mode_type)
{
  OS_RESULT send_status;

  if(com < MAX_TOTPORT_NUM)    //�Ǵ���
  {
    init_frame_info(send_buf);
    set_frame_len(send_buf, len);
    set_port_num(send_buf, com);
    send_buf[CON_MODE_OFFSET] = mode_type;
    switch(mode_type)
    {
        case CON_TER_SYNFLASH://��ʾͬ����������
        case CON_RS485CACHE_RELAY://�м�����
        case CON_RS485_645RELAY://645͸�������ѹ��	
            send_status = os_mbx_send(stProtocol_Analyse[com].Mbx_Send_HiBuf, (void*)send_buf, 0);
            break;

        case CON_RELAYSOUR_RELAYTASK://�м�����
        case CONRS485SOURTIMER://��ʱ������
        case CON_RS485ASKOPESOUR_TASK://���õ����񳭱�����
        case CON_RS485ASKOPESOUR_ALARMFORE://    
        case CON_RS485ASKOPESOUR_ALARMUP://�澯����ʵʱ���ݵĳ���
            send_status = os_mbx_send(stProtocol_Analyse[com].Mbx_Send_LoBuf, (void*)send_buf, 0);
            break;
        default:
            send_status = OS_R_TMO;
            break;
    }
    if(send_status != OS_R_OK)
    {
        free_mem_block(send_buf);
        return __FALSE;              //û�а����ݷ��������غ���Ҫ�������ͻ�����
    }
  }
  return __TRUE;
}
/******************************************************************************
** ��������: void Recv645RelayCmd_Explain_Manage (STPROTOCOL_ANALYSE *pstProtocolAnalyse,unsigned char *pucRecvBuffer,unsigned char TimeOutFlag)
** ��������: �ⲿ�˿���485 ���� ֱ���յ���645����͸��zigbeeͨ������ѹ��������
** ��ڲ���:
** �� �� ֵ:
** ˵    ��:
******************************************************************************/
void Recv645RelayCmd_Explain_Manage (STPROTOCOL_ANALYSE *pstProtocolAnalyse, unsigned char *pucRecvBuffer, unsigned char TimeOutFlag)
{
    U8 *p_send,mem_type;
    unsigned short usRecvLen;

    if(TimeOutFlag==CON_RELAYRET_OVERTIME)
    {
//͸������ѹ�� �����ʱ�Ƿ�Ҫ�ط�����
        return;
    }

    usRecvLen = pucRecvBuffer[0] + (pucRecvBuffer[1]<<8);
    if((usRecvLen<CON_DI_DATA_LEN) && (usRecvLen != 0))
    {
        if(pstProtocolAnalyse->stAnalyse_ctrl.ucSeq < MAX_TOTPORT_NUM)
        {	
		    p_send = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR16);
		    if(NULL != p_send)
		    {
		        memcpy(p_send+DATA_OFFSET, pucRecvBuffer+DATA_OFFSET, usRecvLen);
				send_protocol_frame(p_send, pstProtocolAnalyse->stAnalyse_ctrl.ucSeq, usRecvLen, MEM_256_TYPE);
		    }
		    else
		    {
		    	setmemgetnullid(GET_MEM_ERR0, gusCurMemErrFunName);
		    }	
    	}
    }
    return;
}
/******************************************************************************
** ��������: void RecvRelayCmd_Explain_Manage (STPROTOCOL_ANALYSE *pstProtocolAnalyse,unsigned char *pucRecvBuffer,unsigned char TimeOutFlag)
** ��������: �м������Ӧ֡
** ��ڲ���:
** �� �� ֵ:
** ˵    ��:
******************************************************************************/
void RecvRelayCmd_Explain_Manage (STPROTOCOL_ANALYSE *pstProtocolAnalyse, unsigned char *rev_msg, unsigned char TimeOutFlag)
{
    unsigned char ucRet,i=0,ucExplainResultNum;
    unsigned short usRecvLen;

    if(TimeOutFlag==CON_RS485RECVTYPE_SUCCESS)
    {
		ClearRS485Task(&stRS485RecvExpainVal);
        usRecvLen = rev_msg[0] + (rev_msg[1]<<8);
        ucRet=Meter_Protocol_RecvExplain(&stRelaySend,rev_msg+DATA_OFFSET,usRecvLen,TimeOutFlag,&ucExplainResultNum);

		if(ucRet == RET_SUCCESS && ucExplainResultNum !=0)
		{
			for(i=0;i<ucExplainResultNum;i++)
			{
				if(i > CONRECVEXPLAINNUM) break;
				if(Meter_Protocol_GetExplainResult(i,&stRS485RecvExpainVal)==RET_SUCCESS)
				{
					if(stRS485RecvExpainVal.ucReportType != CON_RS485_REPORT_TYPE_SPEC)
					{    
						Meter_Response_Manage(&stRelaySend);		//�����ݷ��أ��ǳ�ʱ
					}
				}
			}
		}
    }
    return;
}
/******************************************************************************
** ��������: unsigned char RecvRelayTask_Explain_Manage (STPROTOCOL_ANALYSE *pstProtocolAnalyse,unsigned char *pucRecvBuffer,unsigned char TimeOutFlag)
** ��������: �м����񷵻ش���
** ��ڲ���:
** �� �� ֵ:
** ˵    ��:
******************************************************************************/
unsigned char RecvRelayTask_Explain_Manage (STPROTOCOL_ANALYSE *pstProtocolAnalyse, unsigned char *pucRecvBuffer, unsigned char TimeOutFlag)
{
    unsigned char ucRet=RET_ERROR;
//    unsigned char i=0;
//    unsigned char ucBeginPos=0,ucEndPos=0;
//    unsigned short usRecvLen;
//
//    if(TimeOutFlag==CON_RELAYRET_OVERTIME)//��ʱ
//    {
//        stSendDIList.ucReSendnum++;
//        if(stSendDIList.ucReSendnum < CON_TASK_FRAME_RESEND_MAX)
//        {        
//            Fill_RS485RelaySendMbx(stGRTaskExeInfo.ucTaskNo, CON_RELAYSOUR_RELAYTASK);
//            ucRet = RET_SUCCESS;
//        }
//        else//����Ч����
//        {
//            //��֪������ ���Բ�����Ч����
//            stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
//            ucRet = RET_ERROR;
//        }
//        return ucRet;   
//    }
//
//    usRecvLen = pucRecvBuffer[0] + (pucRecvBuffer[1]<<8);
//    if((usRecvLen<CON_DI_DATA_LEN) && (usRecvLen != 0))
//    {
//        if( pstProtocolAnalyse->stRelayPara.ucCharacterChar==0x00 )
//        {//������Ϊ��,�����ǽ�ȡ
//            ucBeginPos=0;
//            ucEndPos=(unsigned char)usRecvLen;
//        }
//        else
//        {
//            for( i=0;i<((unsigned char)usRecvLen);i++)
//            {//����������
//                if( pucRecvBuffer[DATA_OFFSET+i]==pstProtocolAnalyse->stRelayPara.ucCharacterChar )
//                    break;
//            }
//
//            if( i>=((unsigned char)usRecvLen) )
//            {//û���ҵ�������
//                ucBeginPos=0xFF;
//            }
//            else
//            {
//                ucBeginPos=i;
//                if( ucBeginPos+(unsigned char)pstProtocolAnalyse->stRelayPara.usGetFrom>(unsigned char)usRecvLen )
//                    ucBeginPos=0xFF;
//                else
//                    ucBeginPos+=((unsigned char)pstProtocolAnalyse->stRelayPara.usGetFrom);
//
//                if( ucBeginPos!=0xFF )
//                {
//                    if( ((unsigned char)pstProtocolAnalyse->stRelayPara.usGetLen)==0 )
//                        ucEndPos=(unsigned char)usRecvLen;
//                    else
//                    {
//                        ucEndPos=ucBeginPos+(unsigned char)pstProtocolAnalyse->stRelayPara.usGetLen;
//                        if(ucEndPos>(unsigned char)usRecvLen)
//                            ucEndPos=(unsigned char)usRecvLen;
//                    }
//                }
//            }
//        }
//    }
//    else
//        ucBeginPos=0xFF;
//
//    if( ucBeginPos!=0xFF )
//    {
////azh    		stGRDefineArea.stGRTaskRetVal.stDIVal[0].ucDILen = ucEndPos-ucBeginPos;
////		    memcpy( stGRDefineArea.stGRTaskRetVal.ucTaskData,pucRecvBuffer+ucBeginPos+DATA_OFFSET,(ucEndPos-ucBeginPos));
//
//            Fill_TaskData();
//            stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
//            ucRet = RET_SUCCESS;
//    }
//    else//�������յ� ��������Ч��ֱ�ӽ���
//    {
//       ucRet = RET_ERROR;
//       stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID; //
//    } 
    return ucRet;
}
/******************************************************************************
 Function name:  __task void protocol_analyse_task (void)
 Author     :
 Description  :
 Input      :  None
 Return     :  None
 *****************************************************************************/
__task void protocol_analyse_task (void)
{
    U8 ucRet=RET_ERROR;
    U8 *comm_msg=NULL,*send_msg=NULL;
//    U8 *mem_ptr,mem_type;
    U8 i,com=0xff;
    U16 usLen;
//    OS_RESULT protocol_status;

    for(i=0; i<MAX_TOTPORT_NUM; i++)
    {
       init_analyse_manage(i);
    }

    for(;;)
    {
        sign_task_schedule(PROTOCOL_TASK_PRIO);

        comm_msg = NULL;
        os_mbx_wait(Mbx_Comm_Data, (void**)&comm_msg, OS_DLY_100MS);
        if(NULL != comm_msg)
        {
            com = convert_com_index(comm_msg);
        }
//��ʹ�ȴ������ڴ�鳬ʱҲҪ��ȥ��鿴��û��Ҫ���͵�
        for(i=0; i<MAX_TOTPORT_NUM; i++)
        {
            switch(stProtocol_Analyse[i].ucWR_State)
            {
//���ͺ�ȴ����մ���ģʽ
                case CON_PROTOCOL_WAITREV_HANDLE:
                {
                    if((NULL != comm_msg) && (i == com))//azh �ȵĶ˿������ݽ���
                    {
                        //azh 171017 ����ʽ��ѹ��GPRSģ�飬�������й�Լ������
                        #ifdef ZIGBEE_TEST
                        if(i == ZIGBEE_CHL)
                        {
                            //zigbee��ַ�Ͱ󶨵Ĳ�һ��������Ҳ����FF(û��ǰ)��Ҳ����zigbee��Ĺ㲥��ַ����ô��ȥĬ�Ͻ���֧�ֱ������й�Լ
                            if(RET_SUCCESS==check_zigbee_addr_unmatch(&comm_msg[ZIGBEE_RET_ADDR_OFFSET]))
                            {    
                                //Ĭ�Ͻ���
                                Protocol_Explain(comm_msg);           //����645��Լ�������й�Լ                            
                                break;//��������Ӱ��ԭ�е�״̬    
                            }
                        }    	
                        #endif//ZIGBEE_TEST
                        
                        switch(stProtocol_Analyse[i].stAnalyse_ctrl.ucCmdMode)
                        {
                            case CON_TER_SYNFLASH://��ʾͬ����������
                                //Ĭ�Ͻ���
                                Protocol_Explain(comm_msg);           //����645��Լ
                                break;
                            case CON_RS485CACHE_RELAY://�м�����
                                RecvRelayCmd_Explain_Manage(&stProtocol_Analyse[i], comm_msg, CON_RELAYRET_SUCCESS);
                                break;
                            case CON_RS485_645RELAY://�ⲿ�˿���485 ���� ֱ���յ���645����͸��zigbeeͨ������ѹ��������
                            	Recv645RelayCmd_Explain_Manage(&stProtocol_Analyse[i], comm_msg, CON_RELAYRET_SUCCESS);
                            	break;
                            case CON_RELAYSOUR_RELAYTASK://�м�����
                                RecvRelayTask_Explain_Manage(&stProtocol_Analyse[i], comm_msg, CON_RELAYRET_SUCCESS);
                                break;
                            case CONRS485SOURTIMER://��ʱ������
                                TimTaskCmdRevcFrameByDi(&stProtocol_Analyse[i], comm_msg, CON_RELAYRET_SUCCESS);
                                break;
                            case CON_RS485ASKOPESOUR_TASK://���õ����񳭱�����
//                                TaskCmdRevcFrameByDi(&stProtocol_Analyse[i], comm_msg, CON_RELAYRET_SUCCESS);
                                break;
                            case CON_RS485ASKOPESOUR_ALARMFORE:
                            case CON_RS485ASKOPESOUR_ALARMUP://�澯����ʵʱ���ݵĳ���
                                TimTaskCmdRevcFrameByDi(&stProtocol_Analyse[i], comm_msg, CON_RELAYRET_SUCCESS);
                                break;
                            default:
                                break;
                        }
                        init_analyse_manage(i);
                    }
                    else//��ʱ����  ������ڴ��ָ���ǿ� �����´��� 
                    {
                        stProtocol_Analyse[i].stAnalyse_ctrl.Tmp_Tick_Count = OS_Time;
                        //û��ʱ ������ȴ�
                        //�����ʱ ��λĬ��״̬ ȥȡ���������ڷ���
                        if(stProtocol_Analyse[i].stAnalyse_ctrl.Tmp_Tick_Count - stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Tick_Count > stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Dly)
                        {
                            switch(stProtocol_Analyse[i].stAnalyse_ctrl.ucCmdMode)
                            {
                                case CON_RS485CACHE_RELAY://�м�����
                                    RecvRelayCmd_Explain_Manage(&stProtocol_Analyse[i], NULL, CON_RELAYRET_OVERTIME);
                                    break;
	                            case CON_RS485_645RELAY://�ⲿ�˿���485 ���� ֱ���յ���645����͸��zigbeeͨ������ѹ��������
	                            	Recv645RelayCmd_Explain_Manage(&stProtocol_Analyse[i], NULL, CON_RELAYRET_OVERTIME);
	                            	break;
                                case CON_TER_SYNFLASH://��ʾͬ����������
                                    break;
                                case CONRS485SOURTIMER://��ʱ������
                                    TimTaskCmdRevcFrameByDi(&stProtocol_Analyse[i], NULL, CON_RS485RECVTYPE_RECVOVER);//���ճ�ʱ
                                    break;
                                case CON_RELAYSOUR_RELAYTASK://�м�����
                                    RecvRelayTask_Explain_Manage(&stProtocol_Analyse[i], NULL, CON_RELAYRET_OVERTIME);
                                    break;
                                case CON_RS485ASKOPESOUR_TASK://���õ����񳭱�����
                                    TaskCmdRevcFrameByDi(&stProtocol_Analyse[i], NULL, CON_RS485RECVTYPE_RECVOVER);//���ճ�ʱ
                                    break;
                                case CON_RS485ASKOPESOUR_ALARMFORE:
                                case CON_RS485ASKOPESOUR_ALARMUP://�澯����ʵʱ���ݵĳ���
                                    TimTaskCmdRevcFrameByDi(&stProtocol_Analyse[i], NULL, CON_RS485RECVTYPE_RECVOVER);//���ճ�ʱ
                                    break;
                                default:
                                    break;
                            }
                            init_analyse_manage(i);//��ʱĬ�ϴ���Ϊ��λ״̬�� Ҳ������ĳ�ʱҪ��д����֡��
                        }
                    }
                }
                    break;
//Ĭ��Ϊ����Ĭ�ϴ��� ���߲�ѯ�Ƿ��д�����
                case CON_PROTOCOL_DEFAULT_HANDLE:   //Ĭ�ϴ���
                    if((NULL != comm_msg) && (i == com))
                    {
                        //Ĭ�Ͻ���
                        Protocol_Explain(comm_msg);           //����645��Լ
                    }
                    else
                    {
                        //��鷢�Ͷ����Ƿ�������֡Ҫ����
                        send_msg = NULL;//azh 120413
                        os_mbx_wait(stProtocol_Analyse[i].Mbx_Send_HiBuf, (void**)&send_msg, 0);//�ȴӸ����ȼ����д���
                        if(NULL == send_msg)
                        {
                            os_mbx_wait(stProtocol_Analyse[i].Mbx_Send_LoBuf, (void**)&send_msg, 0);
                        }
                        //�����ݴ�����
                        if(NULL !=send_msg)
                        {
                            stProtocol_Analyse[i].stAnalyse_ctrl.ucCmdMode = send_msg[CON_MODE_OFFSET];
                            stProtocol_Analyse[i].stAnalyse_ctrl.ucPort = send_msg[COM_NUM_OFFSET];
                            stProtocol_Analyse[i].stAnalyse_ctrl.ucSeq = send_msg[CON_ID_OFFSET];
                            stProtocol_Analyse[i].stAnalyse_ctrl.ucOptype = send_msg[CON_RW_TYPE_OFFSET];
                            usLen = send_msg[DATA_LEN_OFFSET] + (send_msg[DATA_LEN_OFFSET+1]<<8);
                            switch(stProtocol_Analyse[i].stAnalyse_ctrl.ucCmdMode)
                            {
                                case CON_TER_SYNFLASH://��ʾͬ����������
                                    if(i != ZIGBEE_CHL)
                                    {
                                        free_mem_block(send_msg);
                                        break;
                                    }
                                    send_protocol_frame(send_msg, i, usLen, MEM_256_TYPE);//send_msg[REC_MEM_TYPE_OFFSET]);
                                    stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Tick_Count = OS_Time;     //����һ����ǰϵͳTick
                                    stProtocol_Analyse[i].ucWR_State = CON_PROTOCOL_WAITREV_HANDLE;
                                    //����ʼ����ʱ����

#ifdef ZIGBEE_TEST
                                    if(i == ZIGBEE_CHL)
																		{	
                                    stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Dly = 100;//azh 171010//8;//80ms
																		}
																		else
																		{
																			//�������Ͳ����ʶ����� ����������ڲ�ͨ��
																			stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Dly = 200;//100;//800ms	
																		}																			
#else
																		//�������Ͳ����ʶ����� ����������ڲ�ͨ��
																		stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Dly = 200;//100;//800ms	
#endif																		

                                    break;
                                case CON_RS485CACHE_RELAY://�м�����
                                    send_protocol_frame(send_msg, i, usLen, MEM_256_TYPE);//send_msg[REC_MEM_TYPE_OFFSET]);
                                    stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Tick_Count = OS_Time;     //����һ����ǰϵͳTick
                                    stProtocol_Analyse[i].ucWR_State = CON_PROTOCOL_WAITREV_HANDLE;
                                    //����ʼ����ʱ����
                                    if((stProtocol_Analyse[i].stRelayPara.ucRecvWaitTime&0x80) == 0x80)//10ms
                                    {
                                        stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Dly = (stProtocol_Analyse[i].stRelayPara.ucRecvWaitTime&0x7f)*100;//unit:1s
                                    }
                                    else
                                    {
                                        stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Dly = stProtocol_Analyse[i].stRelayPara.ucRecvWaitTime;//unit:10ms
                                    }
#ifdef ZIGBEE_TEST
                                    if(i == ZIGBEE_CHL)
                                    {
                                        if((stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Dly > 100)||(stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Dly==0))//max=3s
                                        {
                                           stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Dly = 100;
                                        }
                                    }
                                    else
                                    {
                                        if((stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Dly > 500)||(stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Dly==0))//max=3s
                                        {
                                           stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Dly = 500;
                                        }
                                    }
#else
                                    if(stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Dly > 800)//max=8s
                                    {
                                       stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Dly = 800;
                                    }
#endif

                                    break;
                                case CON_RS485_645RELAY://�ⲿ�˿���485 ���� ֱ���յ���645����͸��zigbeeͨ������ѹ��������
                                    stProtocol_Analyse[i].stAnalyse_ctrl.ucSeq = send_msg[CON_ID_OFFSET];//֡�з���ͨ����Ϣ
                                    if((usLen > 0) && (i == ZIGBEE_CHL))
                                    {
                                        send_protocol_frame(send_msg, i, usLen, MEM_256_TYPE);//send_msg[REC_MEM_TYPE_OFFSET]);
                                        stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Tick_Count = OS_Time;     //����һ����ǰϵͳTick
                                        stProtocol_Analyse[i].ucWR_State = CON_PROTOCOL_WAITREV_HANDLE;
                                        //����ʼ����ʱ����
                                        stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Dly = 100;//azh 171010//50;//500ms
                                    }
                                    else
                                    {
                                        free_mem_block(send_msg);
                                        init_analyse_manage(i);
                                    }

                                    break;
                                case CONRS485SOURTIMER://��ʱ������
                                case CON_RELAYSOUR_RELAYTASK://�м�����
                                case CON_RS485ASKOPESOUR_TASK://���õ����񳭱�����
                                case CON_RS485ASKOPESOUR_ALARMUP://�澯����ʵʱ���ݵĳ���
                                case CON_RS485ASKOPESOUR_ALARMFORE:////����ȷ�﷢��ĳ���
                                    ucRet = TaskCmdSendHandle(i, send_msg);
                                    if(RET_SUCCESS == ucRet)
                                    {    
                                        stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Tick_Count = OS_Time;     //����һ����ǰϵͳTick
                                        stProtocol_Analyse[i].ucWR_State = CON_PROTOCOL_WAITREV_HANDLE;
                                        //����ʼ����ʱ����
#ifdef ZIGBEE_TEST
                                        if(i == ZIGBEE_CHL)
                                        {    
                                            stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Dly = 100;//azh 171010//50;//500ms  
                                        }
                                        else
                                        {
                                            stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Dly = 200;//2s  
                                        }    
#else
                                        stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Dly = 200;//2s  
#endif//ZIGBEE_TEST
                                    }
                                    else
                                    {
                                        init_analyse_manage(i);
                                    }                                      
                                    free_mem_block(send_msg);
                                    break;
                                default:
                                    break;
                            }
                        }
//                        else
//                        {
//
//                        }

                    }


                    break;
                default:                //����״̬
                    init_analyse_manage(i);
                    break;
            }
        }
//�ͷ��ڴ��
        if(NULL != comm_msg)
        {
            free_mem_block(comm_msg);
//            comm_msg = NULL;
        }
  }
}

/******************************************************************************
 Function name:  U8 protocol_IEC1107 (U8 *buf)
 Author     :  azh 110523
 Description  :
 Input      :  ͨѶ����֡��ͷ������16�ֽ���Ϣ��
 Return     :  ������û�еõ�������__TRUE�Ѿ�������__FALSEû�д���
         ���������Ƿ�õ��������ڴ������ͷ�
 *****************************************************************************/
U8  Protocol_Explain(U8 *buf)
{
  U8  com;
  S8  err=-1;

  com = convert_com_index(buf);

  switch(com)
  {
    case RS4851_CHL:               //485����dlt645-07��Լ����
		err = deal_local_GWframe(buf);
		if(err != 1)
		{
			__nop();//��һ�׹�Լ��������
#if CODE_VER_TYPE==H_METER_PRJ				//��ϸ�ѹ��
			err = deal_with_DLT645frame(buf);
			if(err != 1)
			{
				Fill_645RelaySendto_Mbx(com, buf);//ֱ��͸��
			}	
#elif CODE_VER_TYPE==WLS_CUR_PRJ			//������߲ɼ���
			err = wls_protocol_analyse(buf);
#elif CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ�� ���ں��� 
			err = deal_with_DLT645frame(buf);
			if(err != 1)
			{
				__nop();//��һ�׹�Լ��������
			}	
#endif
		}
		break;
#if CODE_VER_TYPE==H_METER_PRJ				//��ϸ�ѹ��
    case ZIGBEE_CHL:               //Zigbee����dlt645-07��Լ����
		if(__FALSE == analyse_identity_frame(buf))//�ǲ���zigbee��ѯ�豸��Ϣ����֡
		{
			err = deal_with_DLT645frame(buf);
			if(err != 1)
			{
			  __nop();//��һ�׹�Լ
			}
		}
		break;
#elif CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��

//azh 171010 ��645����������ͬʱ�����ܽ������й�Լͨ�ű��ģ�����ʽ��ѹ����
	#ifdef ZIGBEE_TEST
    case ZIGBEE_CHL:
		if(__FALSE == analyse_identity_frame(buf))//�ǲ���zigbee��ѯ�豸��Ϣ����֡
		{
			err = deal_with_DLT645frame(buf);
			if(err != 1)
			{
				err = deal_local_GWframe(buf);
				if(err != 1)
				{
					__nop();//��һ�׹�Լ��������
				}
			}
		}
	break;
	#else
    case ZIGBEE_CHL:
		err = deal_with_DLT645frame(buf);
		if(err != 1)
		{
		  __nop();//��һ�׹�Լ
		}
	break;
	#endif//ZIGBEE_TEST

#elif CODE_VER_TYPE==WLS_CUR_PRJ			//������߲ɼ��� //С����ģ�� ���߲ɼ���
    case ZIGBEE_CHL:               
		err = wls_protocol_analyse(buf);
		if(err != 1)
		{
		  __nop();//��һ�׹�Լ
		}

		break;
#endif		
    case RS4852_CHL:               //485��ͨ��zigbeeֱ��ת����ȥ
#if CODE_VER_TYPE==WLS_CUR_PRJ			//������߲ɼ���
        err = deal_with_watch_frame(buf);//����485�����ϵ����� ������
#else
		err = deal_local_GWframe(buf);//���ⲻ֧��645���� ��Ϊ����󶨵ĸ�ѹ����ַ�ظ� ����ѹ��û�к���

		if(err != 1)
		{
			__nop();//��һ�׹�Լ��������
			err = deal_with_DLT645frame(buf);//�Ƿ���ЩҪ͸�� ��Щֱ�Ӷ���ʾ�ն� ��������������
	        if(err != 1)
	        {
	        	Fill_645RelaySendto_Mbx(com, buf);//ֱ��͸��
	        }	
		}
#endif
		break;
//    case GPRS_CHL:

//      break;
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//����CPU_GPRSģ��
    case IR_SOFT_CHL:
		err = deal_local_GWframe(buf);//���ⲻ֧��645���� ��Ϊ����󶨵ĸ�ѹ����ַ�ظ� ����ѹ��û�к���
		if(err != 1)
		{
			__nop();//��һ�׹�Լ��������
			err = deal_with_DLT645frame(buf);
			if(err != 1)
			{
				Fill_645RelaySendto_Mbx(com, buf);//ֱ��͸��
			}	        
		}
		break;
#endif
    default:
	      break;
}

////  free_mem_block(buf);
  return __TRUE;
}

/******************************************************************************
 Function name:  U8 send_IEC1107_frame (U8 *send_buf, U8 com, U16 len, U8 type)
 Author     :
 Description  :  ��������֡�����ú�����send_buf���ͷ�
 Input      :  send_buf ���ݷ��ͻ���
            com ѡ���͵Ĵ���COM0��COM3����̫��COM4��COM5
            len ���ݳ���
           type �ڴ������
 Return     :  __TRUE�ɹ�������__FALSEû�з���
 *****************************************************************************/
U8 send_protocol_frame (U8 *send_buf, U8 com, U16 len, U8 type)
{
  U16 time_out;

  if(len == 0)//azh
  {
    free_mem_block(send_buf);
    return __FALSE;
  }

  if((com < MAX_PORT_NUM) || (com == IR_SOFT_CHL))    //�Ǵ���
  {
    switch(com)
    {
      case 0:
        time_out = UART0_SRC_DLY;
        break;
      case 1:
        time_out = UART1_SRC_DLY;
        break;
      case 2:
        time_out = UART2_SRC_DLY;
        break;
      case 3:
        time_out = UART3_SRC_DLY;
        break;
      case IR_SOFT_CHL://����ģ�����⴦��
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//����CPU_GPRSģ��
        send_far_infrared_data(send_buf+DATA_OFFSET, (U8)len);
#endif
        free_mem_block(send_buf);
        return __TRUE;
      default:
        time_out = OS_DLY_1S;
        break;
    }
    //if(__SUCCEED == UART_open(com, OS_DLY_1S))
    if(__SUCCEED == UART_open(com, time_out))
    {
      if(__FAIL == UART_send(com, len, send_buf))
      {
        free_mem_block(send_buf);
        UART_close(com);
        return __FALSE;
      }
      UART_close(com);
    }
    else
    {
      free_mem_block(send_buf);
      return __FALSE;
    }
  }
  else
  {
    free_mem_block(send_buf);
    return __FALSE;
  }
  return __TRUE;
}

/******************************************************************************
 Function name:  U8 convert_com_index (U8 *buf)
 Author     :  snap.gao
 Description  :  ������ͨѶ֡�ж˿ں�ת��Ϊ״̬��ʹ�õ����
         ����0��3����̫���ڽ����Ŵ��ڣ���4����
 Input      :  ͨѶ����֡��ͷ������16�ֽ���Ϣ��
 Return     :  ״̬����Ӧ����ţ�����ʱ����0xff
 *****************************************************************************/
U8 convert_com_index (U8 *buf)
{
  U8 com;
/*
  if(buf[COM_NUM_OFFSET] == ETHERNET_PORT)    //�ж��ǲ��Ǵ���̫������֡
  {
    com = find_socket_index(buf[SOCKET_OFFSET], buf[SOCKET_TYPE_OFFSET]);
    if(com == 0xff)
    {
      return com;
    }
    com += MAX_PORT_NUM;            //��̫����״̬�������Ŵ��ڵ�
  }
  else if(buf[COM_NUM_OFFSET] < MAX_PORT_NUM)   //�ж��ǲ��ǴӴ�������֡
*/
  if(buf[COM_NUM_OFFSET] < MAX_TOTPORT_NUM)
  {
    com = buf[COM_NUM_OFFSET];
  }
  else    //�����ͨѶ�ӿڱ�ʶ
  {
    return 0xff;
  }
  return com;
}
//azh ����������Ϣ������������ʹ���
unsigned char Fill_TimeTaskSendMbx(unsigned char ucModeCtrl)
{
    unsigned char ucRet=RET_ERROR;
    unsigned char *mem_ptr,mem_type;

    mem_ptr = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr != NULL)
    {    
        memset(mem_ptr, 0, DATA_OFFSET);
        mem_ptr[CON_MODE_OFFSET] = ucModeCtrl;
        mem_ptr[CON_ID_OFFSET] = 0xff;//azh û��
//        stGRTaskExeInfo.unInCom = ZIGBEE_CHL;//azh Ҫ���ݲ����������õĶ˿���
        if(0xff == stGRTaskExeInfo.unInCom) 
        {
            free_mem_block(mem_ptr); 
            ucRet = RET_ERROR;   
        }
        else
        {    
            if(__TRUE == send_wait_rev_frame(mem_ptr, stGRTaskExeInfo.unInCom, 0, MEM_256_TYPE, ucModeCtrl))//�м�����   
            {
                ucRet = RET_SUCCESS; 
            }    
        } 
    }
    else
    {
    	ucRet = RET_ERROR;
    	setmemgetnullid(GET_MEM_ERR0, gusCurMemErrFunName);
    }	
    if(RET_ERROR == ucRet)//������� �嵱ǰִ�������
    {
        stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
        if((ucModeCtrl == CON_RS485ASKOPESOUR_ALARMFORE) || (ucModeCtrl == CON_RS485ASKOPESOUR_ALARMUP))
        {
            Clr_Alarm_CopyMeter_Flag(ucModeCtrl, stRS485TaskSend.ucTaskSource);
        }
    }    
    return ucRet;

}

//�м��������������ĵ����ȼ�����
unsigned char Fill_RS485RelaySendMbx(unsigned char ucTaskNo, unsigned char ucModeCtrl)	//���ӵ��м̷��Ͷ���
{
    unsigned char ucRet=RET_ERROR;
    unsigned char *mem_ptr,mem_type;
    
    mem_ptr = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr != NULL)
    {    
        memset(mem_ptr, 0, DATA_OFFSET);
        mem_ptr[CON_MODE_OFFSET] = ucModeCtrl;
        mem_ptr[CON_ID_OFFSET] = ucTaskNo;
        if(0xff == stGRTaskExeInfo.unInCom) 
        {
            free_mem_block(mem_ptr); 
            ucRet = RET_ERROR;   
        }
        else
        {    
            if(__TRUE == send_wait_rev_frame(mem_ptr, stGRTaskExeInfo.unInCom, 0, MEM_256_TYPE, ucModeCtrl))//�м�����   
            {
                ucRet = RET_SUCCESS; 
            }    
        } 
    }
    if(RET_ERROR == ucRet)//������� �嵱ǰִ�������
    {
        stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
    }    
    return ucRet;
    
}
//�����м�����
unsigned char Task_Proc_Relay(unsigned char ucTaskNo)
{
    unsigned char ucRet=RET_ERROR;
//azh ����Ҫ�������������д�м����� ִ���м�����
// 	STRS485RELAYINFO stRS485RelayInfoTmp;
// 	
// 	SetRS485RelayInfo( &stRS485RelayInfoTmp,0,
// 											0,
// 											stGRDefineArea.stGRTask[ucTaskNo].ucMPNo,
// 											stGRDefineArea.stGRTask[ucTaskNo].stRelayTaskPara,
// 											stGRDefineArea.stGRTask[ucTaskNo].ucDataItemNum,
// 											stGRDefineArea.stGRTask[ucTaskNo].ucDataArea );
// 	
// 	Fill_RS485RelaySendList( CON_RELAYSOUR_RELAYTASK,&stRS485RelayInfoTmp );	//���ӵ��м̷��Ͷ���

//azh    stGRTaskExeInfo.unInCom = ConvertToInerPort(GetTaskPararRelayPort(ucTaskNo));//azh �ڲ���Ӧ�Ķ˿ں� 
    
 	//�洢��������
 	stGRDefineArea.stGRTaskRetVal.ucTaskNo = stGRTaskExeInfo.ucNowExeTaskNo;
 	stGRDefineArea.stGRTaskRetVal.stRetDateTime = GetCurrentTime();
 	stGRDefineArea.stGRTaskRetVal.ucDINum = 1;
 	stGRDefineArea.stGRTaskRetVal.stDIVal[0].usDI = CON_DI_TYPE_MPRELAY;//
 	stGRDefineArea.stGRTaskRetVal.stDIVal[0].ucDILen = 0;
 	memset(stGRDefineArea.stGRTaskRetVal.ucTaskData, 0xff, CON_GRTASK_TaskDateMaxDateLen);
    
    memset( &stSendDIList,0,sizeof(stSendDIList) );
    stSendDIList.ucDINum = 1;
    stSendDIList.usDILabel[0] = CON_DI_TYPE_MPRELAY;   
    stSendDIList.ucReSendnum = 0;//�ط�����
    
    ucRet = Fill_RS485RelaySendMbx(ucTaskNo, CON_RELAYSOUR_RELAYTASK);	//���ӵ��м̷��Ͷ���

// 	
 	return ucRet;
}
//��ͨ�������������ĵ����ȼ�����
//unsigned char Fill_RS485AskOpeMbx(unsigned char ucTaskNo, unsigned char ucModeCtrl)	
//{
//    unsigned char ucRet=RET_ERROR;
//    unsigned char *mem_ptr,mem_type;
//    
//    mem_ptr = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR0);
//    if(mem_ptr != NULL)
//    {    
//        memset(mem_ptr, 0, DATA_OFFSET);
//        mem_ptr[CON_MODE_OFFSET] = ucModeCtrl;
//        mem_ptr[CON_ID_OFFSET] = ucTaskNo;
//        if(0xff != stGRTaskExeInfo.unInCom) 
//        {
//            if(__TRUE == send_wait_rev_frame(mem_ptr, stGRTaskExeInfo.unInCom, 0, MEM_256_TYPE, ucModeCtrl))//�м�����   
//            {
//                ucRet = RET_SUCCESS; 
//            }    
//        }
//        else
//        {
//            free_mem_block(mem_ptr); 
//        }    
//    }
//    else
//    {
//    	ucRet = RET_ERROR;
//    	setmemgetnullid(GET_MEM_ERR0, gusCurMemErrFunName);
//    }
//    if(RET_ERROR == ucRet)//������� �嵱ǰִ�������
//    {
//        stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
//    }    
//    return ucRet;//
//}

//azh 140919
//�����07��Լ���򷢳�ȥ�����ݱ�ʶ���յ������ݱ�ʶҪ�˶�һ����ͬ�ų��ɹ�
unsigned char Check_SeRe07DI(STRS485TASK *pstRS485SendTask, unsigned char *pucRecvBuffer,unsigned short usRecvLen)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char ucMeterNo,i,m=0,di[4];
    unsigned long usDI64507A=0,usDI64507B=0;
    
	ucMeterNo = Get_MeterNo_byMPNo(pstRS485SendTask->ucDevNo);
	if(ucMeterNo<CON_METER_NUM)
	{
		if(stMPNoAddrList.stMPNoAddrRec[ucMeterNo].ucProtocolNo == CON_PROTOCOL_DL64507)
		{
            ucRet = RET_ERROR;
            for(i=0; i<(unsigned char)usRecvLen; i++) 
            {
                if(pucRecvBuffer[i] == 0x68)
                {
                    m = i;
                    break;
                }
            }
        
        	if(m < usRecvLen)
        	{    
        	    for(i=0;i<4;i++)
        	    {
        	        di[i] = pucRecvBuffer[10+m+i]-0x33;
        	    }	
			    usDI64507A = di[0]+(di[1]<<8)+(di[2]<<16)+(di[3]<<24);
			    if(RET_SUCCESS==GetDI64507(&usDI64507B,pstRS485SendTask->usDataType))
			    {
			         if(usDI64507A == usDI64507B)
			         {
			            ucRet = RET_SUCCESS;
			         }
			    }
        	}
    
        }
    }
		return ucRet;
}

//�ڽ�����������ø��ݶ�ʱˢ����������ݱ�ʶ��д����֡
void TimTaskCmdRevcFrameByDi(STPROTOCOL_ANALYSE *pstProtocolAnalyse, unsigned char *rev_msg, unsigned char TimeOutFlag)
{
    unsigned char ucRsendFlag=0,i;
    unsigned char ucRet,ucExplainResultNum;
    unsigned short usRecvLen,usJumpLen;

    if(TimeOutFlag == CON_RS485RECVTYPE_SUCCESS)//���ճɹ�
    {    
        Alarm_EnsureJudge_MeterCopy(METER_NORMAL_FLAG,stRS485TaskSend.ucDevNo);
        //�յ����ݽ��������洦��(�����ݱ�ʶ)
        usRecvLen = rev_msg[0] + (rev_msg[1]<<8);
    
		ucRet=Meter_Protocol_CheckFrame(&stRS485TaskSend, rev_msg+DATA_OFFSET, usRecvLen, &usJumpLen);
		if( ucRet==RET_SUCCESS )
		{
		    //azh171019 ����ʽ��ѹ������Ҫ֧�ֱ��������й�Լ������Ҫ�����ǳ��ص����ݣ������յ����ݵ�ģ��MAC��ַ
	    #ifdef ZIGBEE_TEST
            if(gucGetZigbeeAddrFlag == 1)//��־��Ҫͨ��zigbee�㲥��ַ ��ȡzigbee��ַ������
            {
                store_zigbee_addr(&rev_msg[ZIGBEE_RET_ADDR_OFFSET]);
                gucGetZigbeeAddrFlag = 0;
            }	
        #endif//ZIGBEE_TEST    	    
		    //azh 140919
		    ucRet = Check_SeRe07DI(&stRS485TaskSend,rev_msg+DATA_OFFSET,usRecvLen);
		    if(ucRet==RET_SUCCESS)
		    {
		    //	֡��ʽ���ɹ�
            ucRet=Meter_Protocol_RecvExplain(&stRS485TaskSend,rev_msg+DATA_OFFSET,usRecvLen,TimeOutFlag,&ucExplainResultNum);

    		if(ucRet == RET_SUCCESS && ucExplainResultNum !=0)
    		{
    			for(i=0;i<ucExplainResultNum;i++)
    			{
    				if(i > CONRECVEXPLAINNUM) break;
    				if(Meter_Protocol_GetExplainResult(i,&stRS485RecvExpainVal)==RET_SUCCESS)
    				{
    					if(stRS485RecvExpainVal.ucReportType != CON_RS485_REPORT_TYPE_SPEC)
    					{    
    						Meter_Response_Manage(&stRS485TaskSend);		//�����ݷ��أ��ǳ�ʱ
    					}
    				}
    			}
    		}

    		//else//�յ����ݷ�����С��Ϊ0 �ʼ��ʼ��ʱ�Ѿ������Ч����
        	}
        	else
    		{
                ucRsendFlag = 1;//�����ݱ�ʶ��Ҫ�ط�		    
    		}    
		}
		else//������Ч��Ҫ���ط� ����ط�3��
		{
            ucRsendFlag = 1;//�����ݱ�ʶ��Ҫ�ط�		    
		}            
    }
    else//��ʱ �����rev_msg=NULL
    {
        ucRsendFlag = 1;
        Alarm_EnsureJudge_MeterCopy(METER_OVERTIME_FLAG,stRS485TaskSend.ucDevNo);
    }

    if(ucRsendFlag)
    {            
        stSendDIList.ucReSendnum++;   
        if(stSendDIList.ucReSendnum < CON_TASK_FRAME_RESEND_MAX)
        {
            Fill_TimeTaskSendMbx(pstProtocolAnalyse->stAnalyse_ctrl.ucCmdMode);//(CONRS485SOURTIMER); 
        }
        else
        {
     	    //���γ�ʱ�� ������Ч���� ��ʼ����ʱ��Ĭ�ϳ�ʼ������Ч���� �������ﲻ��Ҫ
     	    ucRsendFlag = 0;
     	    //������Ч����
     	    stRS485TaskSend.ucReportType = CON_RS485_REPORT_TYPE_SPEC;//��ʱ ��ʱ��һ�£����溯������õ�
            Meter_Response_Manage(&stRS485TaskSend);		//��ʱ����Ч����
            stRS485TaskSend.ucReportType = CON_RS485_REPORT_TYPE_GET;
        }
    }
//
    if(ucRsendFlag == 0)
    {    
        stSendDIList.ucDIPtr++;
        if(stSendDIList.ucDIPtr < stSendDIList.ucDINum)
        {    
     	    stSendDIList.ucReSendnum = 0;
     	    Fill_TimeTaskSendMbx(pstProtocolAnalyse->stAnalyse_ctrl.ucCmdMode);//(CONRS485SOURTIMER);
     	}
     	else//���������������ݳ������
     	{
   	        stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
            //����Ǹ澯 �������ɱ�־ �����ǿղ���
            Set_Alarm_CopyMeter_Flag(pstProtocolAnalyse->stAnalyse_ctrl.ucCmdMode, stRS485TaskSend.ucTaskSource, CON_STATUS_FINISH);
     	}
    }        
}
//�ڽ�����������ø�����ͨ��������ݱ�ʶ��д����֡
void TaskCmdRevcFrameByDi(STPROTOCOL_ANALYSE *pstProtocolAnalyse, unsigned char *rev_msg, unsigned char TimeOutFlag)
{
//    unsigned char ucRsendFlag=0,i;
//    unsigned char ucRet,ucExplainResultNum;
//    unsigned short usRecvLen,usJumpLen;
//
//    if(TimeOutFlag == CON_RS485RECVTYPE_SUCCESS)//���ճɹ�
//    {    
//        //�յ����ݽ��������洦��(�����ݱ�ʶ)
//        //todo
//        usRecvLen = rev_msg[0] + (rev_msg[1]<<8);
//
//  
//        ucRet=Meter_Protocol_CheckFrame(&stRS485TaskSend,rev_msg+DATA_OFFSET, usRecvLen, &usJumpLen);
//		if( ucRet==RET_SUCCESS )
//		{//	֡��ʽ���ɹ�
////			RS485_Recv_Manage( CON_RS485RECVTYPE_SUCCESS,&ucInitRS485CacheFlag );
//
//    		ucRet = Meter_Protocol_RecvExplain(&stRS485TaskSend,rev_msg+DATA_OFFSET,usRecvLen,TimeOutFlag,&ucExplainResultNum);
//    		if(ucRet == RET_SUCCESS && ucExplainResultNum !=0)
//    		{
////    			Alarm_EnsureJudge_MeterCopy(METER_NORMAL_FLAG,stRS485TaskSend.ucDevNo);
//    			for(i=0;i<CONRECVEXPLAINNUM;i++)
//    			{
//    				if(i > CONRECVEXPLAINNUM) break;
//    				if(Meter_Protocol_GetExplainResult(i,&stRS485RecvExpainVal)==RET_SUCCESS)
//    				{
//    					if(stRS485RecvExpainVal.ucReportType != CON_RS485_REPORT_TYPE_SPEC)
//    					{    
//    						Meter_Response_Manage(&stRS485TaskSend);		//�����ݷ��أ��ǳ�ʱ
//    					}
//    				}
//    			}
//    		}
//    		//else//�յ����ݷ�����С��Ϊ0 �ʼ��ʼ��ʱ�Ѿ������Ч����
//
//		}
//		else//������Ч��Ҫ���ط� ����ط�3��
//		{
//            ucRsendFlag = 1;//�����ݱ�ʶ��Ҫ�ط�		    
//		}            
//    }
//    else//��ʱ �����rev_msg=NULL
//    {
//        ucRsendFlag = 1;
////        Alarm_EnsureJudge_MeterCopy(METER_OVERTIME_FLAG,stRS485TaskSend.ucDevNo);
//    }
//
//    if(ucRsendFlag)
//    {            
//        stSendDIList.ucReSendnum++;   
//        if(stSendDIList.ucReSendnum < CON_TASK_FRAME_RESEND_MAX)
//        {
//            Fill_RS485AskOpeMbx(stGRTaskExeInfo.ucNowExeTaskNo, CON_RS485ASKOPESOUR_TASK);//���͵� 
//        }
//        else
//        {
//     	    //���γ�ʱ�� ������Ч���� ��ʼ����ʱ��Ĭ�ϳ�ʼ������Ч���� �������ﲻ��Ҫ
//     	    ucRsendFlag = 0;
//        }
//    }
////
//    if(ucRsendFlag == 0)
//    {    
//        stSendDIList.ucDIPtr++;
//        if(stSendDIList.ucDIPtr < stSendDIList.ucDINum)
//        {    
//     	    stSendDIList.ucReSendnum = 0;
//     	    Fill_RS485AskOpeMbx(stGRTaskExeInfo.ucNowExeTaskNo, CON_RS485ASKOPESOUR_TASK);//���͵�
//     	}
//     	else//�����������ݳ������
//     	{
//     	    Fill_TaskData();
//     	    stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
//     	}
//    }        
} 	
//�ڽ�������������м������������д����֡
unsigned char TaskRelaySendFrame(unsigned char com, unsigned char *revmbx)
{
    unsigned char ucRet=RET_ERROR;
//    unsigned char *ucBuffer,mem_type,ucTaskNo;
//    
//    ucTaskNo = revmbx[CON_ID_OFFSET];
//	ucBuffer = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR0);
//	if(ucBuffer != NULL)
//	{  
//        ucRet = GetTaskPararRelay(ucTaskNo, revmbx);
//        if(RET_SUCCESS == ucRet)
//        {
//            //baud = revmbx[12];//�м��������� ��������???
//            stProtocol_Analyse[com].stRelayPara.ucWaitTime = revmbx[14];//�м̳�ʱʱ�䣨�룩
//            stProtocol_Analyse[com].stRelayPara.ucCharacterChar = revmbx[15];//�����ַ�
//            stProtocol_Analyse[com].stRelayPara.usGetFrom = revmbx[16] + (revmbx[17]<<8);//��ȡ��ʼ
//            stProtocol_Analyse[com].stRelayPara.usGetLen = revmbx[18] + (revmbx[19]<<8);//��ȡ����
//            
//            if(revmbx[20] < CON_GRTASK_DATANUM)
//            {    
//        	    memcpy(ucBuffer+DATA_OFFSET, revmbx+21, revmbx[20]);//�м������
//    			if(com == ZIGBEE_CHL)
//    			{    
//    			    memcpy(&ucBuffer[ZIGBEE_RET_ADDR_OFFSET], gMatch_Zigbee_Addr, 8);
//    			}
//        	    send_protocol_frame(ucBuffer, com, revmbx[20], MEM_256_TYPE);
//        	    ucRet=RET_SUCCESS;
//        	}
//        	else
//        	{
//        	    ucRet = RET_ERROR;
//        	}    
//        }
//        if(RET_ERROR == ucRet)
//        {
//            free_mem_block(ucBuffer); 
//        }    
//	}
//	else
//	{	
//		ucRet = RET_ERROR;
//		setmemgetnullid(GET_MEM_ERR0, gusCurMemErrFunName);	
//	}	
//	if(RET_ERROR == ucRet)
//	{
//	   stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
//	}    

	return ucRet;    
}
//�ڽ�����������ø�����ͨ��������ݱ�ʶ��д����֡
unsigned char AlarmCmdSendFrameByDi(unsigned char com, unsigned char *revmbx)
{
    unsigned char ucMPType,*ucBuffer=NULL,mem_type,ucRet=RET_ERROR;
    unsigned short usLen;

 	ucMPType = RunPara_GetMPType(stRS485TaskSend.ucDevNo);
    
 	if( ucMPType == CON_MPTYPE_METER )
 	{
 		if(stSendDIList.ucDIPtr < stSendDIList.ucDINum)
 		{
            //�������ݱ�ʶ�������֡
            stRS485TaskSend.usDataType = stSendDIList.usDILabel[stSendDIList.ucDIPtr];//�������ʶ
    		
    		ucBuffer = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR0);
    		if(ucBuffer != NULL)
    		{    
//		        ucRet=Meter_Protocol_SendExplain(&stRS485TaskSend,ucBuffer+DATA_OFFSET,&usLen);
//        		if(ucRet==RET_SUCCESS)
//azh ucRet���ֵ��Ӧ�øǵ� ����Ҫ��
                if(RET_SUCCESS == Meter_Protocol_SendExplain(&stRS485TaskSend,ucBuffer+DATA_OFFSET,&usLen))
        		{
        			if(com == ZIGBEE_CHL)
        			{    
        			    memcpy(&ucBuffer[ZIGBEE_RET_ADDR_OFFSET], gMatch_Zigbee_Addr, 8);
        			}
        			send_protocol_frame(ucBuffer, com, usLen, MEM_256_TYPE);
                    ucRet = RET_SUCCESS;
        		}
        		else//������ݱ�ʶ��֧�� ������һ����ʶ
        		{
                    free_mem_block(ucBuffer);                		    
                    stSendDIList.ucDIPtr++;
                    if(stSendDIList.ucDIPtr < stSendDIList.ucDINum)
                    {    
                 	    stSendDIList.ucReSendnum = 0;
                 	    init_analyse_manage(com);//��Ҫ�ǰѽ���״̬���¸ĳɷ���״̬
                 	    Fill_TimeTaskSendMbx(stProtocol_Analyse[com].stAnalyse_ctrl.ucCmdMode);//(CONRS485SOURTIMER);
                 	}
                 	else//���������������ݳ������
                 	{
               	        stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
               	        Set_Alarm_CopyMeter_Flag(stProtocol_Analyse[com].stAnalyse_ctrl.ucCmdMode, stRS485TaskSend.ucTaskSource, CON_STATUS_FINISH);
                 	}
                    ucRet = RET_SUCCESS;
        		}    
        	}
        	else
        	{
        		stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
        		ucRet = RET_SUCCESS;
        		setmemgetnullid(GET_MEM_ERR0, gusCurMemErrFunName);
        		//����һ���س�
        		Set_Alarm_CopyMeter_Flag(stProtocol_Analyse[com].stAnalyse_ctrl.ucCmdMode, stRS485TaskSend.ucTaskSource, CON_STATUS_INIT);	
        	}	
 		}
 	}
 	if(RET_ERROR == ucRet)//Ϊ��ʹ������Ч ִ����������
 	{
        stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
        Clr_Alarm_CopyMeter_Flag(stProtocol_Analyse[com].stAnalyse_ctrl.ucCmdMode, stRS485TaskSend.ucTaskSource);
 	}    
 	
    return ucRet;
} 	
//�ڽ�����������ø�����ͨ��������ݱ�ʶ��д����֡
unsigned char TaskCmdSendFrameByDi(unsigned char com, unsigned char *revmbx)
{
    unsigned char ucMPType,*ucBuffer=NULL,mem_type,ucRet=RET_ERROR;
    unsigned short usLen;


 	ucMPType = RunPara_GetMPType(stRS485TaskSend.ucDevNo);
    
 	if( ucMPType == CON_MPTYPE_METER )
 	{
 		if(stSendDIList.ucDIPtr < stSendDIList.ucDINum)
 		{
            //�������ݱ�ʶ�������֡
            stRS485TaskSend.usDataType = stSendDIList.usDILabel[stSendDIList.ucDIPtr];//�������ʶ
    		
    		ucBuffer = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR0);
    		if(ucBuffer != NULL)
    		{    
//		        ucRet=Meter_Protocol_SendExplain(&stRS485TaskSend,ucBuffer+DATA_OFFSET,&usLen);
//        		if(ucRet==RET_SUCCESS)
                if(RET_SUCCESS == Meter_Protocol_SendExplain(&stRS485TaskSend,ucBuffer+DATA_OFFSET,&usLen))
        		{
        			if(com == ZIGBEE_CHL)
        			{    
        			    memcpy(&ucBuffer[ZIGBEE_RET_ADDR_OFFSET], gMatch_Zigbee_Addr, 8);
        			}
        			send_protocol_frame(ucBuffer, com, usLen, MEM_256_TYPE);
                    ucRet = RET_SUCCESS;
        		}
        		else//������ݱ�ʶ��֧�� ������һ����ʶ
        		{
                    free_mem_block(ucBuffer);                		    
                    stSendDIList.ucDIPtr++;
                    if(stSendDIList.ucDIPtr < stSendDIList.ucDINum)
                    {    
                 	    stSendDIList.ucReSendnum = 0;
                 	    init_analyse_manage(com);//��Ҫ�ǰѽ���״̬���¸ĳɷ���״̬
                 	    Fill_TimeTaskSendMbx(CONRS485SOURTIMER);
                 	}
                 	else//���������������ݳ������
                 	{
               	        stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
                 	}
                 	ucRet = RET_SUCCESS;
        		}    
        	}
        	else
        	{
        		ucRet = RET_ERROR;
        		setmemgetnullid(GET_MEM_ERR0, gusCurMemErrFunName);	
        	}	
 		}
 	}
 	if(RET_ERROR == ucRet)//Ϊ��ʹ������Ч ִ����������
 	{
        stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
//        if(stProtocol_Analyse[com].stAnalyse_ctrl.ucCmdMode == CONRS485SOURTIMER)
//        {
//            stTimTaskExeInfo.ucCurTmState = CON_TASK_RETDATA_INVALID; 
//        }    
 	}    
 	
    return ucRet;
} 	
//�ڽ��������������ͨ����,�м�����ȷ��ʹ���
unsigned char TaskCmdSendHandle(unsigned char com, unsigned char *revmbx)
{
    unsigned char ucRet=RET_ERROR;
    
    if(com < MAX_TOTPORT_NUM)
    {    
        switch(stProtocol_Analyse[com].stAnalyse_ctrl.ucCmdMode)
        {
            case CONRS485SOURTIMER://azh ��ʱˢ������
                ucRet = TaskCmdSendFrameByDi(com, revmbx);
                break;
            case CON_RELAYSOUR_RELAYTASK:
                ucRet = TaskRelaySendFrame(com, revmbx);
                break;
            case CON_RS485ASKOPESOUR_TASK:
                ucRet = TaskCmdSendFrameByDi(com, revmbx);
                break;
            case CON_RS485ASKOPESOUR_ALARMUP://�澯����ʵʱ���ݵĳ���
            case CON_RS485ASKOPESOUR_ALARMFORE:////����ȷ�﷢��ĳ���
                ucRet = AlarmCmdSendFrameByDi(com, revmbx);
                break;
            default:
                break;           
        }
    }
    return ucRet;
}
//��д��ʱ���ݳ��� 
unsigned char Meter_FillTimerCommand(unsigned char ucTaskSource, unsigned char ucMPNo)
{
	unsigned char ucRet=RET_SUCCESS,ucTaskType;
	unsigned char ucMeterNo,i;
	

    switch(ucTaskSource)
    {
        case CONRS485SOURTIMER:
        {    
            Timer_FillRS485MeterDi();//��дҪ���������ݱ�ʶ
            ucTaskType = ucTaskSource;
            break;
        }
//��������ȷ�ϸ澯������
    	case CONRS485SOUR_ALRAFFIRMTASK:					//����ȷ������0
    	case CONRS485SOUR_ALRAFFIRMTASK+1:					//����ȷ������1
    	case CONRS485SOUR_ALRAFFIRMTASK+2:					//����ȷ������2
    	case CONRS485SOUR_ALRAFFIRMTASK+3:					//����ȷ������3
    	case CONRS485SOUR_ALRAFFIRMTASK+4:					//����ȷ������4
    	case CONRS485SOUR_ALRAFFIRMTASK+5:					//����ȷ������5
    	case CONRS485SOUR_ALRAFFIRMTASK+6:					//����ȷ������6
    	case CONRS485SOUR_ALRAFFIRMTASK+7:					//����ȷ������7
    	case CONRS485SOUR_ALRAFFIRMTASK+8:					//����ȷ������8
    	case CONRS485SOUR_ALRAFFIRMTASK+9:					//����ȷ������9
    	{
    	    Alarm_FillRS485MeterDi(ucTaskSource);
    	    ucTaskType = CON_RS485ASKOPESOUR_ALARMFORE;//�������������� �����������
    	    break;   
    	}    
//�����澯Я������
    	case CONRS485SOURALR_1:								//������������0
    	case CONRS485SOURALR_1+1:							//������������1
    	case CONRS485SOURALR_1+2:							//������������2
    	case CONRS485SOURALR_1+3:							//������������3
    	case CONRS485SOURALR_1+4:							//������������4
    	case CONRS485SOURALR_1+5:							//������������5
    	case CONRS485SOURALR_1+6:							//������������6
    	case CONRS485SOURALR_1+7:							//������������7
    	case CONRS485SOURALR_1+8:							//������������8
    	case CONRS485SOURALR_1+9:							//������������9
    	{
    	    Alarm_FillRS485MeterDi(ucTaskSource);
            ucTaskType = CON_RS485ASKOPESOUR_ALARMUP;//�������������� �����������
    	    break;   
    	}
    	default:
        {
            stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
            return RET_ERROR;
    	}
    }
    	
    //azh ���ﲻ��Ҫ���ݱ�ʶ ����Ҫ�������ݱ�ʶ��stSendDIListȫ������
	SetRS485TaskPer(&stRS485TaskSend,ucTaskSource,CON_SEQNO_NULL,CON_RS485_REPORT_TYPE_GET,CON_DI_TYPE_INVALID,ucMPNo,NULL);
//	ucPortNo=RunPara_GetMeterPara_PortNo_byMPNo(ucMPNo);
//	ucRet=RS485_FillCommand(&stRS485Task,ucListNo,ucPortNo);


    ucMeterNo = Get_MeterNo_byMPNo(ucMPNo);
    if(ucMeterNo > CON_METER_NUM)
    {
        stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
        return RET_ERROR;
    }    
    if(ucMPNo == 0)//azh
    {    
        stGRTaskExeInfo.unInCom = ZIGBEE_CHL;//azh �������ڲ���Ӧ�Ķ˿ں� 
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
	#ifdef ZIGBEE_TEST//azh 171010
        stMPNoAddrList.stMPNoAddrRec[ucMeterNo].ucProtocolNo = CON_PROTOCOL_DL64507;//645��Լ2007
        memcpy(stMPNoAddrList.stMPNoAddrRec[ucMeterNo].ucMeterAddr, gMatch_HVMeter_Addr, 6);
        //��������㱻ɾ�� �ͻ����ѹ����ַΪȫ0 Ϊ�˲���ȥ����
        if((gMatch_HVMeter_Addr[0]!=0)||(gMatch_HVMeter_Addr[1]!=0)||(gMatch_HVMeter_Addr[2]!=0)
        ||(gMatch_HVMeter_Addr[3]!=0)||(gMatch_HVMeter_Addr[4]!=0)||(gMatch_HVMeter_Addr[5]!=0))
        {
            Fill_TimeTaskSendMbx(ucTaskType);
        }	
	#else
        stMPNoAddrList.stMPNoAddrRec[ucMeterNo].ucProtocolNo = CON_PROTOCOL_DL64507;//645��Լ2007
        memcpy(stMPNoAddrList.stMPNoAddrRec[ucMeterNo].ucMeterAddr, &stTerRunPara.stMeterPara[ucMPNo].stMeterAddr.ucMeterAddr[0], 6);
        //��������㱻ɾ�� �ͻ����ѹ����ַΪȫ0 Ϊ�˲���ȥ����
        for(i=0; i<6; i++)
        {
            if(stTerRunPara.stMeterPara[ucMPNo].stMeterAddr.ucMeterAddr[i] != 0)
            {	
            	Fill_TimeTaskSendMbx(ucTaskType);
            	break;
            }
        }
	#endif//ZIGBEE_TEST
#else
        stMPNoAddrList.stMPNoAddrRec[ucMeterNo].ucProtocolNo = CON_PROTOCOL_DL64507;//645��Լ2007
        memcpy(stMPNoAddrList.stMPNoAddrRec[ucMeterNo].ucMeterAddr, gMatch_HVMeter_Addr, 6);
        //��������㱻ɾ�� �ͻ����ѹ����ַΪȫ0 Ϊ�˲���ȥ����
        if((gMatch_HVMeter_Addr[0]!=0)||(gMatch_HVMeter_Addr[1]!=0)||(gMatch_HVMeter_Addr[2]!=0)
        ||(gMatch_HVMeter_Addr[3]!=0)||(gMatch_HVMeter_Addr[4]!=0)||(gMatch_HVMeter_Addr[5]!=0))
        {
            Fill_TimeTaskSendMbx(ucTaskType);
        }
#endif//CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
    }
    else
    {
        stGRTaskExeInfo.unInCom = RS4852_CHL;//azh �������ڲ���Ӧ�Ķ˿ں�  
//        ulBaudRate=Get_Correct_Boadrate(stTerRunPara.stMeterPara[ucMeterNo].ulBaudRate);
//        Uart_InitChBaud(ucChID,ulBaudRate,ucEPN,ucDataBit,ucStopBit);
        Fill_TimeTaskSendMbx(ucTaskType);

    }    	

	return ucRet;
}
/******************************************************************************
              End Of File
 *****************************************************************************/
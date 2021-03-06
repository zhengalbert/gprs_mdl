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
   2、     2.00     2011-05-19   azh  modfy
   1、     1.00     2009-06-30   snap.gao   create
******************************************************************************/
#define  __PROTOCOL_C
/*---------------------------------------------------------------------------*/
#include "..\inc\global_config.h"

/*private functions defined in this file:------------------------------------*/
static U8 Protocol_Explain (U8 *buf);

#if CODE_VER_TYPE==WLS_CUR_PRJ			//配合无线采集器
static S8 deal_with_watch_frame(U8 *InBuf);
static U8 get_protocolno_by_id(U8 protoco_id);
static U8 check_ret_addr(U8 *mpaddr, U8 *retaddr);
static U16 get_97di_by07di(U32 di07);
static U16 get_protocol_di(U8 protocol_no, U8 *InBuf);
#endif

//azh 显示终端外部端口收到645-07命令 需要自身处理 而不是透传的数据标识
#define SELF_DI_NUM		7
#ifdef ZIGBEE_TEST
const U32 dis_self_handle_table[SELF_DI_NUM] =
{
	0x04000300,		//缩略最后一个字节
//	0x04000301,		//自动循环显示屏数(0-99)    
//	0x04000302,		//每屏显示时间              
//	0x04000303,		//显示电能小数位数          
//	0x04000304,		//显示功率(最大需量)小数位数	
//	0x04000305,		//按键循环显示屏数(0-99)    
//
	0x04000700,		//缩略最后一个字节
//	0x04000701,		//调制型红外光口通信速率特征字
//	0x04000702,		//接触式红外光口通信速率特征字
//	0x04000703,		//通信口1通信速率特征字       
//	0x04000704,		//通信口2通信速率特征字       
//	0x04000705,		//通信口3通信速率特征字       

	0x04000C00,		//缩略最后一个字节
//	0x04000C01,		//0级密码	
////......
//	0x04000C0A,		//9级密码

	0x04040100,		//缩略最后一个字节
//	0x04040101,		//自动循环显示第1屏显示代码
////......
//	0x04040163,		//自动循环显示第99屏显示代码	

	0x04040200,		//缩略最后一个字节
//	0x04040201,		//自动循环显示第1屏显示代码
////......
//	0x04040263,		//自动循环显示第99屏显示代码

	0x03300D00,		//缩略最后一个字节
//	0x03300D00,		//开表盖总次数
//	0x03300D01,		//(上1次)开表盖记录
////......
//	0x03300D0A,		//(上10次)开表盖记录

	0x03300E00,		//缩略最后一个字节
//	0x03300E00,		//开端钮盒总次数
//	0x03300E01,		//(上1次)开端钮盒记录
////......
//	0x03300E0A,		//(上10次)开端钮盒记录	
};
/******************************************************************************
** 函数名称: void dis_self_handle_di (U8 com)
** 函数功能: 初始化端口解析状态机
** 入口参数: zigbee收到的数据都显示终端处理 返回后还会处理（只处理收到的命令字）
** 返 回 值: __TRUE:显示终端处理  __FALSE:透传到高压表
** 说    明:
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
		        	if(((uldi&0xffffff00) == 0x04000300) && ((uldi&0x000000ff)>5))//这数据标识后面几个要透传
		        	{
		        		return ret;
		        	}
		       		return __TRUE;	
		        }	
		    }
		    break;
		//显示终端自己处理
		case 0x13:	//读通信地址
		case 0x15:	//写通信地址
		case 0x17:	//更改通信速率
		case 0x18:	//修改密码
		case 0x1D:	//多功能端子输出控制命令
			return __TRUE;;		    
		default:
			break;
	}
    return ret;
}
#endif//ZIGBEE_TEST
/******************************************************************************
** 函数名称: void init_analyse_manage (U8 com)
** 函数功能: 初始化端口解析状态机
** 入口参数:
** 返 回 值:
** 说    明:
******************************************************************************/
void init_analyse_manage (U8 com)
{
    if(com < MAX_TOTPORT_NUM)
    {
        memset(&stProtocol_Analyse[com].stAnalyse_ctrl, 0, sizeof(STANALYSE_TCB));    //初始化命令返回邮箱
    }
    stProtocol_Analyse[com].ucWR_State = CON_PROTOCOL_DEFAULT_HANDLE;
}
/******************************************************************************
 Function name:  U8 Fill_645RelaySendto_Mbx (U8 incom, U8 *send_buf)
 Author     :
 Description:  从外部端口如485 红外收到的645命令帧 直接通过Zigbee通道转发到高压表抄数据
 Input      :   send_buf 数据发送缓冲从串口收到的原始命令帧
 Return     :  __TRUE成功发出，__FALSE没有发出
 *****************************************************************************/
U8 Fill_645RelaySendto_Mbx (U8 incom, U8 *send_buf)
{
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不带CPU_GPRS模块
    U8 *p_send,mem_type;
    U16  usLen;

    p_send = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR16);
    if(NULL != p_send)
    {
        if(incom < MAX_TOTPORT_NUM)
        {
            usLen = send_buf[0];// + (send_buf[1]<<8);
            memcpy(p_send+DATA_OFFSET, send_buf+DATA_OFFSET, usLen);

            memcpy(&p_send[ZIGBEE_RET_ADDR_OFFSET], gMatch_Zigbee_Addr, 8);//zigbee待发送目标地址
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
 Description:  发送中继命令帧到命令帧到发送处理队列
 Input      :   send_buf 数据发送缓冲
                com 选择发送的串口COM0－COM3，以太网COM4，COM5
                len 数据长度
                type 内存块类型
                mode_type 命令帖类型
 Return     :  __TRUE成功发出，__FALSE没有发出
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
            switch(ucPort)//转发口
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
                    memcpy(&p_send[ZIGBEE_RET_ADDR_OFFSET], gMatch_Zigbee_Addr, 8);//zigbee待发送目标地址
                }
                else
                {
                    memset(&p_send[ZIGBEE_RET_ADDR_OFFSET], 0xff, 8);//zigbee待发送目标地址
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
 Description:  发送主动发的命令帧到命令帧处理队列
 Input      :   send_buf 数据发送缓冲
                com 选择发送的串口COM0－COM3，以太网COM4，COM5
                len 数据长度
                type 内存块类型
                mode_type 命令帖类型
 Return     :  __TRUE成功发出，__FALSE没有发出
 *****************************************************************************/
U8 send_wait_rev_frame (U8 *send_buf, U8 com, U16 len, U8 memtype, U8 mode_type)
{
  OS_RESULT send_status;

  if(com < MAX_TOTPORT_NUM)    //是串口
  {
    init_frame_info(send_buf);
    set_frame_len(send_buf, len);
    set_port_num(send_buf, com);
    send_buf[CON_MODE_OFFSET] = mode_type;
    switch(mode_type)
    {
        case CON_TER_SYNFLASH://显示同步抄表命令
        case CON_RS485CACHE_RELAY://中继命令
        case CON_RS485_645RELAY://645透传命令到高压表	
            send_status = os_mbx_send(stProtocol_Analyse[com].Mbx_Send_HiBuf, (void*)send_buf, 0);
            break;

        case CON_RELAYSOUR_RELAYTASK://中继任务
        case CONRS485SOURTIMER://定时读数据
        case CON_RS485ASKOPESOUR_TASK://配置的任务抄表命令
        case CON_RS485ASKOPESOUR_ALARMFORE://    
        case CON_RS485ASKOPESOUR_ALARMUP://告警上送实时数据的抄表
            send_status = os_mbx_send(stProtocol_Analyse[com].Mbx_Send_LoBuf, (void*)send_buf, 0);
            break;
        default:
            send_status = OS_R_TMO;
            break;
    }
    if(send_status != OS_R_OK)
    {
        free_mem_block(send_buf);
        return __FALSE;              //没有把数据发出，返回后需要处理发送缓冲区
    }
  }
  return __TRUE;
}
/******************************************************************************
** 函数名称: void Recv645RelayCmd_Explain_Manage (STPROTOCOL_ANALYSE *pstProtocolAnalyse,unsigned char *pucRecvBuffer,unsigned char TimeOutFlag)
** 函数功能: 外部端口如485 红外 直接收到是645命令透传zigbee通道抄高压表的数据
** 入口参数:
** 返 回 值:
** 说    明:
******************************************************************************/
void Recv645RelayCmd_Explain_Manage (STPROTOCOL_ANALYSE *pstProtocolAnalyse, unsigned char *pucRecvBuffer, unsigned char TimeOutFlag)
{
    U8 *p_send,mem_type;
    unsigned short usRecvLen;

    if(TimeOutFlag==CON_RELAYRET_OVERTIME)
    {
//透传抄高压表 如果超时是否要重发命令
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
** 函数名称: void RecvRelayCmd_Explain_Manage (STPROTOCOL_ANALYSE *pstProtocolAnalyse,unsigned char *pucRecvBuffer,unsigned char TimeOutFlag)
** 函数功能: 中继命令回应帧
** 入口参数:
** 返 回 值:
** 说    明:
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
						Meter_Response_Manage(&stRelaySend);		//有数据返回，非超时
					}
				}
			}
		}
    }
    return;
}
/******************************************************************************
** 函数名称: unsigned char RecvRelayTask_Explain_Manage (STPROTOCOL_ANALYSE *pstProtocolAnalyse,unsigned char *pucRecvBuffer,unsigned char TimeOutFlag)
** 函数功能: 中继任务返回处理
** 入口参数:
** 返 回 值:
** 说    明:
******************************************************************************/
unsigned char RecvRelayTask_Explain_Manage (STPROTOCOL_ANALYSE *pstProtocolAnalyse, unsigned char *pucRecvBuffer, unsigned char TimeOutFlag)
{
    unsigned char ucRet=RET_ERROR;
//    unsigned char i=0;
//    unsigned char ucBeginPos=0,ucEndPos=0;
//    unsigned short usRecvLen;
//
//    if(TimeOutFlag==CON_RELAYRET_OVERTIME)//超时
//    {
//        stSendDIList.ucReSendnum++;
//        if(stSendDIList.ucReSendnum < CON_TASK_FRAME_RESEND_MAX)
//        {        
//            Fill_RS485RelaySendMbx(stGRTaskExeInfo.ucTaskNo, CON_RELAYSOUR_RELAYTASK);
//            ucRet = RET_SUCCESS;
//        }
//        else//填无效数据
//        {
//            //不知道长度 所以不填无效数据
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
//        {//特征码为零,不考虑截取
//            ucBeginPos=0;
//            ucEndPos=(unsigned char)usRecvLen;
//        }
//        else
//        {
//            for( i=0;i<((unsigned char)usRecvLen);i++)
//            {//查找特征码
//                if( pucRecvBuffer[DATA_OFFSET+i]==pstProtocolAnalyse->stRelayPara.ucCharacterChar )
//                    break;
//            }
//
//            if( i>=((unsigned char)usRecvLen) )
//            {//没有找到特征码
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
//    else//有数据收到 但是是无效的直接结束
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
//即使等待接收内存块超时也要进去查查看有没有要发送的
        for(i=0; i<MAX_TOTPORT_NUM; i++)
        {
            switch(stProtocol_Analyse[i].ucWR_State)
            {
//发送后等待接收处理模式
                case CON_PROTOCOL_WAITREV_HANDLE:
                {
                    if((NULL != comm_msg) && (i == com))//azh 等的端口有数据进来
                    {
                        //azh 171017 电子式高压表GPRS模块，被动上行规约回数据
                        #ifdef ZIGBEE_TEST
                        if(i == ZIGBEE_CHL)
                        {
                            //zigbee地址和绑定的不一样，而且也不是FF(没绑定前)，也不是zigbee层的广播地址，那么就去默认解析支持被动上行规约
                            if(RET_SUCCESS==check_zigbee_addr_unmatch(&comm_msg[ZIGBEE_RET_ADDR_OFFSET]))
                            {    
                                //默认解析
                                Protocol_Explain(comm_msg);           //处理645规约或者上行规约                            
                                break;//跳出，不影响原有的状态    
                            }
                        }    	
                        #endif//ZIGBEE_TEST
                        
                        switch(stProtocol_Analyse[i].stAnalyse_ctrl.ucCmdMode)
                        {
                            case CON_TER_SYNFLASH://显示同步抄表命令
                                //默认解析
                                Protocol_Explain(comm_msg);           //处理645规约
                                break;
                            case CON_RS485CACHE_RELAY://中继命令
                                RecvRelayCmd_Explain_Manage(&stProtocol_Analyse[i], comm_msg, CON_RELAYRET_SUCCESS);
                                break;
                            case CON_RS485_645RELAY://外部端口如485 红外 直接收到是645命令透传zigbee通道抄高压表的数据
                            	Recv645RelayCmd_Explain_Manage(&stProtocol_Analyse[i], comm_msg, CON_RELAYRET_SUCCESS);
                            	break;
                            case CON_RELAYSOUR_RELAYTASK://中继任务
                                RecvRelayTask_Explain_Manage(&stProtocol_Analyse[i], comm_msg, CON_RELAYRET_SUCCESS);
                                break;
                            case CONRS485SOURTIMER://定时读数据
                                TimTaskCmdRevcFrameByDi(&stProtocol_Analyse[i], comm_msg, CON_RELAYRET_SUCCESS);
                                break;
                            case CON_RS485ASKOPESOUR_TASK://配置的任务抄表命令
//                                TaskCmdRevcFrameByDi(&stProtocol_Analyse[i], comm_msg, CON_RELAYRET_SUCCESS);
                                break;
                            case CON_RS485ASKOPESOUR_ALARMFORE:
                            case CON_RS485ASKOPESOUR_ALARMUP://告警上送实时数据的抄表
                                TimTaskCmdRevcFrameByDi(&stProtocol_Analyse[i], comm_msg, CON_RELAYRET_SUCCESS);
                                break;
                            default:
                                break;
                        }
                        init_analyse_manage(i);
                    }
                    else//超时计算  传入的内存块指针是空 有重新处理 
                    {
                        stProtocol_Analyse[i].stAnalyse_ctrl.Tmp_Tick_Count = OS_Time;
                        //没超时 则继续等待
                        //如果超时 则复位默认状态 去取新命令用于发送
                        if(stProtocol_Analyse[i].stAnalyse_ctrl.Tmp_Tick_Count - stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Tick_Count > stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Dly)
                        {
                            switch(stProtocol_Analyse[i].stAnalyse_ctrl.ucCmdMode)
                            {
                                case CON_RS485CACHE_RELAY://中继命令
                                    RecvRelayCmd_Explain_Manage(&stProtocol_Analyse[i], NULL, CON_RELAYRET_OVERTIME);
                                    break;
	                            case CON_RS485_645RELAY://外部端口如485 红外 直接收到是645命令透传zigbee通道抄高压表的数据
	                            	Recv645RelayCmd_Explain_Manage(&stProtocol_Analyse[i], NULL, CON_RELAYRET_OVERTIME);
	                            	break;
                                case CON_TER_SYNFLASH://显示同步抄表命令
                                    break;
                                case CONRS485SOURTIMER://定时读数据
                                    TimTaskCmdRevcFrameByDi(&stProtocol_Analyse[i], NULL, CON_RS485RECVTYPE_RECVOVER);//接收超时
                                    break;
                                case CON_RELAYSOUR_RELAYTASK://中继任务
                                    RecvRelayTask_Explain_Manage(&stProtocol_Analyse[i], NULL, CON_RELAYRET_OVERTIME);
                                    break;
                                case CON_RS485ASKOPESOUR_TASK://配置的任务抄表命令
                                    TaskCmdRevcFrameByDi(&stProtocol_Analyse[i], NULL, CON_RS485RECVTYPE_RECVOVER);//接收超时
                                    break;
                                case CON_RS485ASKOPESOUR_ALARMFORE:
                                case CON_RS485ASKOPESOUR_ALARMUP://告警上送实时数据的抄表
                                    TimTaskCmdRevcFrameByDi(&stProtocol_Analyse[i], NULL, CON_RS485RECVTYPE_RECVOVER);//接收超时
                                    break;
                                default:
                                    break;
                            }
                            init_analyse_manage(i);//超时默认处理为复位状态机 也有特殊的超时要填写返回帧的
                        }
                    }
                }
                    break;
//默认为接收默认处理 或者查询是否有待发送
                case CON_PROTOCOL_DEFAULT_HANDLE:   //默认处理
                    if((NULL != comm_msg) && (i == com))
                    {
                        //默认解析
                        Protocol_Explain(comm_msg);           //处理645规约
                    }
                    else
                    {
                        //检查发送队列是否有命令帧要发送
                        send_msg = NULL;//azh 120413
                        os_mbx_wait(stProtocol_Analyse[i].Mbx_Send_HiBuf, (void**)&send_msg, 0);//先从高优先级队列处理
                        if(NULL == send_msg)
                        {
                            os_mbx_wait(stProtocol_Analyse[i].Mbx_Send_LoBuf, (void**)&send_msg, 0);
                        }
                        //有数据待发送
                        if(NULL !=send_msg)
                        {
                            stProtocol_Analyse[i].stAnalyse_ctrl.ucCmdMode = send_msg[CON_MODE_OFFSET];
                            stProtocol_Analyse[i].stAnalyse_ctrl.ucPort = send_msg[COM_NUM_OFFSET];
                            stProtocol_Analyse[i].stAnalyse_ctrl.ucSeq = send_msg[CON_ID_OFFSET];
                            stProtocol_Analyse[i].stAnalyse_ctrl.ucOptype = send_msg[CON_RW_TYPE_OFFSET];
                            usLen = send_msg[DATA_LEN_OFFSET] + (send_msg[DATA_LEN_OFFSET+1]<<8);
                            switch(stProtocol_Analyse[i].stAnalyse_ctrl.ucCmdMode)
                            {
                                case CON_TER_SYNFLASH://显示同步抄表命令
                                    if(i != ZIGBEE_CHL)
                                    {
                                        free_mem_block(send_msg);
                                        break;
                                    }
                                    send_protocol_frame(send_msg, i, usLen, MEM_256_TYPE);//send_msg[REC_MEM_TYPE_OFFSET]);
                                    stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Tick_Count = OS_Time;     //缓冲一个当前系统Tick
                                    stProtocol_Analyse[i].ucWR_State = CON_PROTOCOL_WAITREV_HANDLE;
                                    //并初始化超时参数

#ifdef ZIGBEE_TEST
                                    if(i == ZIGBEE_CHL)
																		{	
                                    stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Dly = 100;//azh 171010//8;//80ms
																		}
																		else
																		{
																			//其他类型波特率都不高 比如与基表内部通信
																			stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Dly = 200;//100;//800ms	
																		}																			
#else
																		//其他类型波特率都不高 比如与基表内部通信
																		stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Dly = 200;//100;//800ms	
#endif																		

                                    break;
                                case CON_RS485CACHE_RELAY://中继命令
                                    send_protocol_frame(send_msg, i, usLen, MEM_256_TYPE);//send_msg[REC_MEM_TYPE_OFFSET]);
                                    stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Tick_Count = OS_Time;     //缓冲一个当前系统Tick
                                    stProtocol_Analyse[i].ucWR_State = CON_PROTOCOL_WAITREV_HANDLE;
                                    //并初始化超时参数
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
                                case CON_RS485_645RELAY://外部端口如485 红外 直接收到是645命令透传zigbee通道抄高压表的数据
                                    stProtocol_Analyse[i].stAnalyse_ctrl.ucSeq = send_msg[CON_ID_OFFSET];//帧有返回通道信息
                                    if((usLen > 0) && (i == ZIGBEE_CHL))
                                    {
                                        send_protocol_frame(send_msg, i, usLen, MEM_256_TYPE);//send_msg[REC_MEM_TYPE_OFFSET]);
                                        stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Tick_Count = OS_Time;     //缓冲一个当前系统Tick
                                        stProtocol_Analyse[i].ucWR_State = CON_PROTOCOL_WAITREV_HANDLE;
                                        //并初始化超时参数
                                        stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Dly = 100;//azh 171010//50;//500ms
                                    }
                                    else
                                    {
                                        free_mem_block(send_msg);
                                        init_analyse_manage(i);
                                    }

                                    break;
                                case CONRS485SOURTIMER://定时读数据
                                case CON_RELAYSOUR_RELAYTASK://中继任务
                                case CON_RS485ASKOPESOUR_TASK://配置的任务抄表命令
                                case CON_RS485ASKOPESOUR_ALARMUP://告警上送实时数据的抄表
                                case CON_RS485ASKOPESOUR_ALARMFORE:////报警确诊发起的抄表
                                    ucRet = TaskCmdSendHandle(i, send_msg);
                                    if(RET_SUCCESS == ucRet)
                                    {    
                                        stProtocol_Analyse[i].stAnalyse_ctrl.Rec_Tick_Count = OS_Time;     //缓冲一个当前系统Tick
                                        stProtocol_Analyse[i].ucWR_State = CON_PROTOCOL_WAITREV_HANDLE;
                                        //并初始化超时参数
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
                default:                //空闲状态
                    init_analyse_manage(i);
                    break;
            }
        }
//释放内存块
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
 Input      :  通讯数据帧，头部包含16字节信息字
 Return     :  数据有没有得到处理，__TRUE已经处理，__FALSE没有处理
         无论数据是否得到处理，内存块均不释放
 *****************************************************************************/
U8  Protocol_Explain(U8 *buf)
{
  U8  com;
  S8  err=-1;

  com = convert_com_index(buf);

  switch(com)
  {
    case RS4851_CHL:               //485，按dlt645-07规约解析
		err = deal_local_GWframe(buf);
		if(err != 1)
		{
			__nop();//另一套规约上行南网
#if CODE_VER_TYPE==H_METER_PRJ				//配合高压表
			err = deal_with_DLT645frame(buf);
			if(err != 1)
			{
				Fill_645RelaySendto_Mbx(com, buf);//直接透传
			}	
#elif CODE_VER_TYPE==WLS_CUR_PRJ			//配合无线采集器
			err = wls_protocol_analyse(buf);
#elif CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块 用于红外 
			err = deal_with_DLT645frame(buf);
			if(err != 1)
			{
				__nop();//另一套规约上行南网
			}	
#endif
		}
		break;
#if CODE_VER_TYPE==H_METER_PRJ				//配合高压表
    case ZIGBEE_CHL:               //Zigbee，按dlt645-07规约解析
		if(__FALSE == analyse_identity_frame(buf))//是不是zigbee查询设备信息命令帧
		{
			err = deal_with_DLT645frame(buf);
			if(err != 1)
			{
			  __nop();//另一套规约
			}
		}
		break;
#elif CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块

//azh 171010 在645主动抄表的同时，还能接收上行规约通信报文（电子式高压表）
	#ifdef ZIGBEE_TEST
    case ZIGBEE_CHL:
		if(__FALSE == analyse_identity_frame(buf))//是不是zigbee查询设备信息命令帧
		{
			err = deal_with_DLT645frame(buf);
			if(err != 1)
			{
				err = deal_local_GWframe(buf);
				if(err != 1)
				{
					__nop();//另一套规约上行南网
				}
			}
		}
	break;
	#else
    case ZIGBEE_CHL:
		err = deal_with_DLT645frame(buf);
		if(err != 1)
		{
		  __nop();//另一套规约
		}
	break;
	#endif//ZIGBEE_TEST

#elif CODE_VER_TYPE==WLS_CUR_PRJ			//配合无线采集器 //小无线模块 无线采集器
    case ZIGBEE_CHL:               
		err = wls_protocol_analyse(buf);
		if(err != 1)
		{
		  __nop();//另一套规约
		}

		break;
#endif		
    case RS4852_CHL:               //485，通过zigbee直接转发出去
#if CODE_VER_TYPE==WLS_CUR_PRJ			//配合无线采集器
        err = deal_with_watch_frame(buf);//监听485总线上的数据 并保存
#else
		err = deal_local_GWframe(buf);//红外不支持645命令 因为会与绑定的高压表地址重复 但高压表没有红外

		if(err != 1)
		{
			__nop();//另一套规约上行南网
			err = deal_with_DLT645frame(buf);//是否有些要透传 有些直接对显示终端 包括参数和数据
	        if(err != 1)
	        {
	        	Fill_645RelaySendto_Mbx(com, buf);//直接透传
	        }	
		}
#endif
		break;
//    case GPRS_CHL:

//      break;
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不带CPU_GPRS模块
    case IR_SOFT_CHL:
		err = deal_local_GWframe(buf);//红外不支持645命令 因为会与绑定的高压表地址重复 但高压表没有红外
		if(err != 1)
		{
			__nop();//另一套规约上行南网
			err = deal_with_DLT645frame(buf);
			if(err != 1)
			{
				Fill_645RelaySendto_Mbx(com, buf);//直接透传
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
 Description  :  发送数据帧，调用函数后send_buf被释放
 Input      :  send_buf 数据发送缓冲
            com 选择发送的串口COM0－COM3，以太网COM4，COM5
            len 数据长度
           type 内存块类型
 Return     :  __TRUE成功发出，__FALSE没有发出
 *****************************************************************************/
U8 send_protocol_frame (U8 *send_buf, U8 com, U16 len, U8 type)
{
  U16 time_out;

  if(len == 0)//azh
  {
    free_mem_block(send_buf);
    return __FALSE;
  }

  if((com < MAX_PORT_NUM) || (com == IR_SOFT_CHL))    //是串口
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
      case IR_SOFT_CHL://红外模拟特殊处理
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不带CPU_GPRS模块
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
 Description  :  将数据通讯帧中端口号转换为状态机使用的序号
         串口0～3，以太网口紧接着串口，即4～？
 Input      :  通讯数据帧，头部包含16字节信息字
 Return     :  状态机对应的序号，错误时返回0xff
 *****************************************************************************/
U8 convert_com_index (U8 *buf)
{
  U8 com;
/*
  if(buf[COM_NUM_OFFSET] == ETHERNET_PORT)    //判断是不是从以太网来的帧
  {
    com = find_socket_index(buf[SOCKET_OFFSET], buf[SOCKET_TYPE_OFFSET]);
    if(com == 0xff)
    {
      return com;
    }
    com += MAX_PORT_NUM;            //以太网的状态机紧跟着串口的
  }
  else if(buf[COM_NUM_OFFSET] < MAX_PORT_NUM)   //判断是不是从串口来的帧
*/
  if(buf[COM_NUM_OFFSET] < MAX_TOTPORT_NUM)
  {
    com = buf[COM_NUM_OFFSET];
  }
  else    //错误的通讯接口标识
  {
    return 0xff;
  }
  return com;
}
//azh 发送启动信息到任务解析发送处理
unsigned char Fill_TimeTaskSendMbx(unsigned char ucModeCtrl)
{
    unsigned char ucRet=RET_ERROR;
    unsigned char *mem_ptr,mem_type;

    mem_ptr = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr != NULL)
    {    
        memset(mem_ptr, 0, DATA_OFFSET);
        mem_ptr[CON_MODE_OFFSET] = ucModeCtrl;
        mem_ptr[CON_ID_OFFSET] = 0xff;//azh 没用
//        stGRTaskExeInfo.unInCom = ZIGBEE_CHL;//azh 要根据测量点所配置的端口来
        if(0xff == stGRTaskExeInfo.unInCom) 
        {
            free_mem_block(mem_ptr); 
            ucRet = RET_ERROR;   
        }
        else
        {    
            if(__TRUE == send_wait_rev_frame(mem_ptr, stGRTaskExeInfo.unInCom, 0, MEM_256_TYPE, ucModeCtrl))//中继任务   
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
    if(RET_ERROR == ucRet)//如果出错 清当前执行任务号
    {
        stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
        if((ucModeCtrl == CON_RS485ASKOPESOUR_ALARMFORE) || (ucModeCtrl == CON_RS485ASKOPESOUR_ALARMUP))
        {
            Clr_Alarm_CopyMeter_Flag(ucModeCtrl, stRS485TaskSend.ucTaskSource);
        }
    }    
    return ucRet;

}

//中继任务填到解析任务的低优先级队列
unsigned char Fill_RS485RelaySendMbx(unsigned char ucTaskNo, unsigned char ucModeCtrl)	//添加到中继发送队列
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
            if(__TRUE == send_wait_rev_frame(mem_ptr, stGRTaskExeInfo.unInCom, 0, MEM_256_TYPE, ucModeCtrl))//中继任务   
            {
                ucRet = RET_SUCCESS; 
            }    
        } 
    }
    if(RET_ERROR == ucRet)//如果出错 清当前执行任务号
    {
        stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
    }    
    return ucRet;
    
}
//处理中继任务
unsigned char Task_Proc_Relay(unsigned char ucTaskNo)
{
    unsigned char ucRet=RET_ERROR;
//azh 这里要搞个控制命令填写中继命令 执行中继任务
// 	STRS485RELAYINFO stRS485RelayInfoTmp;
// 	
// 	SetRS485RelayInfo( &stRS485RelayInfoTmp,0,
// 											0,
// 											stGRDefineArea.stGRTask[ucTaskNo].ucMPNo,
// 											stGRDefineArea.stGRTask[ucTaskNo].stRelayTaskPara,
// 											stGRDefineArea.stGRTask[ucTaskNo].ucDataItemNum,
// 											stGRDefineArea.stGRTask[ucTaskNo].ucDataArea );
// 	
// 	Fill_RS485RelaySendList( CON_RELAYSOUR_RELAYTASK,&stRS485RelayInfoTmp );	//添加到中继发送队列

//azh    stGRTaskExeInfo.unInCom = ConvertToInerPort(GetTaskPararRelayPort(ucTaskNo));//azh 内部对应的端口号 
    
 	//存储返回数据
 	stGRDefineArea.stGRTaskRetVal.ucTaskNo = stGRTaskExeInfo.ucNowExeTaskNo;
 	stGRDefineArea.stGRTaskRetVal.stRetDateTime = GetCurrentTime();
 	stGRDefineArea.stGRTaskRetVal.ucDINum = 1;
 	stGRDefineArea.stGRTaskRetVal.stDIVal[0].usDI = CON_DI_TYPE_MPRELAY;//
 	stGRDefineArea.stGRTaskRetVal.stDIVal[0].ucDILen = 0;
 	memset(stGRDefineArea.stGRTaskRetVal.ucTaskData, 0xff, CON_GRTASK_TaskDateMaxDateLen);
    
    memset( &stSendDIList,0,sizeof(stSendDIList) );
    stSendDIList.ucDINum = 1;
    stSendDIList.usDILabel[0] = CON_DI_TYPE_MPRELAY;   
    stSendDIList.ucReSendnum = 0;//重发次数
    
    ucRet = Fill_RS485RelaySendMbx(ucTaskNo, CON_RELAYSOUR_RELAYTASK);	//添加到中继发送队列

// 	
 	return ucRet;
}
//普通任务填到解析任务的低优先级队列
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
//            if(__TRUE == send_wait_rev_frame(mem_ptr, stGRTaskExeInfo.unInCom, 0, MEM_256_TYPE, ucModeCtrl))//中继任务   
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
//    if(RET_ERROR == ucRet)//如果出错 清当前执行任务号
//    {
//        stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
//    }    
//    return ucRet;//
//}

//azh 140919
//如果是07规约，则发出去的数据标识和收到的数据标识要核对一下相同才出成功
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

//在解析任务里调用根据定时刷新任务的数据标识填写发送帧
void TimTaskCmdRevcFrameByDi(STPROTOCOL_ANALYSE *pstProtocolAnalyse, unsigned char *rev_msg, unsigned char TimeOutFlag)
{
    unsigned char ucRsendFlag=0,i;
    unsigned char ucRet,ucExplainResultNum;
    unsigned short usRecvLen,usJumpLen;

    if(TimeOutFlag == CON_RS485RECVTYPE_SUCCESS)//接收成功
    {    
        Alarm_EnsureJudge_MeterCopy(METER_NORMAL_FLAG,stRS485TaskSend.ucDevNo);
        //收到数据解析并保存处理(按数据标识)
        usRecvLen = rev_msg[0] + (rev_msg[1]<<8);
    
		ucRet=Meter_Protocol_CheckFrame(&stRS485TaskSend, rev_msg+DATA_OFFSET, usRecvLen, &usJumpLen);
		if( ucRet==RET_SUCCESS )
		{
		    //azh171019 电子式高压表由于要支持被动抄上行规约，所以要区分是抄回的数据，被动收到数据地模块MAC地址
	    #ifdef ZIGBEE_TEST
            if(gucGetZigbeeAddrFlag == 1)//标志是要通过zigbee广播地址 攻取zigbee地址并保存
            {
                store_zigbee_addr(&rev_msg[ZIGBEE_RET_ADDR_OFFSET]);
                gucGetZigbeeAddrFlag = 0;
            }	
        #endif//ZIGBEE_TEST    	    
		    //azh 140919
		    ucRet = Check_SeRe07DI(&stRS485TaskSend,rev_msg+DATA_OFFSET,usRecvLen);
		    if(ucRet==RET_SUCCESS)
		    {
		    //	帧格式检查成功
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
    						Meter_Response_Manage(&stRS485TaskSend);		//有数据返回，非超时
    					}
    				}
    			}
    		}

    		//else//收到数据分析在小项为0 最开始初始化时已经填成无效数据
        	}
        	else
    		{
                ucRsendFlag = 1;//本数据标识需要重发		    
    		}    
		}
		else//数据无效则要求重发 最多重发3次
		{
            ucRsendFlag = 1;//本数据标识需要重发		    
		}            
    }
    else//超时 传入的rev_msg=NULL
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
     	    //三次超时到 保存无效数据 初始化的时候默认初始化成无效数据 所以这里不需要
     	    ucRsendFlag = 0;
     	    //保存无效数据
     	    stRS485TaskSend.ucReportType = CON_RS485_REPORT_TYPE_SPEC;//超时 临时改一下，下面函数里会用到
            Meter_Response_Manage(&stRS485TaskSend);		//超时填无效数据
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
     	else//本测量点所有数据抄读完成
     	{
   	        stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
            //如果是告警 则会置完成标志 否则是空操作
            Set_Alarm_CopyMeter_Flag(pstProtocolAnalyse->stAnalyse_ctrl.ucCmdMode, stRS485TaskSend.ucTaskSource, CON_STATUS_FINISH);
     	}
    }        
}
//在解析任务里调用根据普通任务的数据标识填写发送帧
void TaskCmdRevcFrameByDi(STPROTOCOL_ANALYSE *pstProtocolAnalyse, unsigned char *rev_msg, unsigned char TimeOutFlag)
{
//    unsigned char ucRsendFlag=0,i;
//    unsigned char ucRet,ucExplainResultNum;
//    unsigned short usRecvLen,usJumpLen;
//
//    if(TimeOutFlag == CON_RS485RECVTYPE_SUCCESS)//接收成功
//    {    
//        //收到数据解析并保存处理(按数据标识)
//        //todo
//        usRecvLen = rev_msg[0] + (rev_msg[1]<<8);
//
//  
//        ucRet=Meter_Protocol_CheckFrame(&stRS485TaskSend,rev_msg+DATA_OFFSET, usRecvLen, &usJumpLen);
//		if( ucRet==RET_SUCCESS )
//		{//	帧格式检查成功
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
//    						Meter_Response_Manage(&stRS485TaskSend);		//有数据返回，非超时
//    					}
//    				}
//    			}
//    		}
//    		//else//收到数据分析在小项为0 最开始初始化时已经填成无效数据
//
//		}
//		else//数据无效则要求重发 最多重发3次
//		{
//            ucRsendFlag = 1;//本数据标识需要重发		    
//		}            
//    }
//    else//超时 传入的rev_msg=NULL
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
//            Fill_RS485AskOpeMbx(stGRTaskExeInfo.ucNowExeTaskNo, CON_RS485ASKOPESOUR_TASK);//发送到 
//        }
//        else
//        {
//     	    //三次超时到 保存无效数据 初始化的时候默认初始化成无效数据 所以这里不需要
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
//     	    Fill_RS485AskOpeMbx(stGRTaskExeInfo.ucNowExeTaskNo, CON_RS485ASKOPESOUR_TASK);//发送到
//     	}
//     	else//所有任务数据抄读完成
//     	{
//     	    Fill_TaskData();
//     	    stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
//     	}
//    }        
} 	
//在解析任务里调用中继任务的命令填写发送帧
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
//            //baud = revmbx[12];//中继任务波特率 后续处理???
//            stProtocol_Analyse[com].stRelayPara.ucWaitTime = revmbx[14];//中继超时时间（秒）
//            stProtocol_Analyse[com].stRelayPara.ucCharacterChar = revmbx[15];//特征字符
//            stProtocol_Analyse[com].stRelayPara.usGetFrom = revmbx[16] + (revmbx[17]<<8);//截取开始
//            stProtocol_Analyse[com].stRelayPara.usGetLen = revmbx[18] + (revmbx[19]<<8);//截取长度
//            
//            if(revmbx[20] < CON_GRTASK_DATANUM)
//            {    
//        	    memcpy(ucBuffer+DATA_OFFSET, revmbx+21, revmbx[20]);//中继命令长度
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
//在解析任务里调用根据普通任务的数据标识填写发送帧
unsigned char AlarmCmdSendFrameByDi(unsigned char com, unsigned char *revmbx)
{
    unsigned char ucMPType,*ucBuffer=NULL,mem_type,ucRet=RET_ERROR;
    unsigned short usLen;

 	ucMPType = RunPara_GetMPType(stRS485TaskSend.ucDevNo);
    
 	if( ucMPType == CON_MPTYPE_METER )
 	{
 		if(stSendDIList.ucDIPtr < stSendDIList.ucDINum)
 		{
            //根据数据标识组读命令帧
            stRS485TaskSend.usDataType = stSendDIList.usDILabel[stSendDIList.ucDIPtr];//数据项标识
    		
    		ucBuffer = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR0);
    		if(ucBuffer != NULL)
    		{    
//		        ucRet=Meter_Protocol_SendExplain(&stRS485TaskSend,ucBuffer+DATA_OFFSET,&usLen);
//        		if(ucRet==RET_SUCCESS)
//azh ucRet这个值不应该盖掉 后面要用
                if(RET_SUCCESS == Meter_Protocol_SendExplain(&stRS485TaskSend,ucBuffer+DATA_OFFSET,&usLen))
        		{
        			if(com == ZIGBEE_CHL)
        			{    
        			    memcpy(&ucBuffer[ZIGBEE_RET_ADDR_OFFSET], gMatch_Zigbee_Addr, 8);
        			}
        			send_protocol_frame(ucBuffer, com, usLen, MEM_256_TYPE);
                    ucRet = RET_SUCCESS;
        		}
        		else//如果数据标识不支持 继续下一个标识
        		{
                    free_mem_block(ucBuffer);                		    
                    stSendDIList.ucDIPtr++;
                    if(stSendDIList.ucDIPtr < stSendDIList.ucDINum)
                    {    
                 	    stSendDIList.ucReSendnum = 0;
                 	    init_analyse_manage(com);//主要是把接受状态重新改成发送状态
                 	    Fill_TimeTaskSendMbx(stProtocol_Analyse[com].stAnalyse_ctrl.ucCmdMode);//(CONRS485SOURTIMER);
                 	}
                 	else//本测量点所有数据抄读完成
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
        		//等下一轮重抄
        		Set_Alarm_CopyMeter_Flag(stProtocol_Analyse[com].stAnalyse_ctrl.ucCmdMode, stRS485TaskSend.ucTaskSource, CON_STATUS_INIT);	
        	}	
 		}
 	}
 	if(RET_ERROR == ucRet)//为了使任务无效 执行其他任务
 	{
        stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
        Clr_Alarm_CopyMeter_Flag(stProtocol_Analyse[com].stAnalyse_ctrl.ucCmdMode, stRS485TaskSend.ucTaskSource);
 	}    
 	
    return ucRet;
} 	
//在解析任务里调用根据普通任务的数据标识填写发送帧
unsigned char TaskCmdSendFrameByDi(unsigned char com, unsigned char *revmbx)
{
    unsigned char ucMPType,*ucBuffer=NULL,mem_type,ucRet=RET_ERROR;
    unsigned short usLen;


 	ucMPType = RunPara_GetMPType(stRS485TaskSend.ucDevNo);
    
 	if( ucMPType == CON_MPTYPE_METER )
 	{
 		if(stSendDIList.ucDIPtr < stSendDIList.ucDINum)
 		{
            //根据数据标识组读命令帧
            stRS485TaskSend.usDataType = stSendDIList.usDILabel[stSendDIList.ucDIPtr];//数据项标识
    		
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
        		else//如果数据标识不支持 继续下一个标识
        		{
                    free_mem_block(ucBuffer);                		    
                    stSendDIList.ucDIPtr++;
                    if(stSendDIList.ucDIPtr < stSendDIList.ucDINum)
                    {    
                 	    stSendDIList.ucReSendnum = 0;
                 	    init_analyse_manage(com);//主要是把接受状态重新改成发送状态
                 	    Fill_TimeTaskSendMbx(CONRS485SOURTIMER);
                 	}
                 	else//本测量点所有数据抄读完成
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
 	if(RET_ERROR == ucRet)//为了使任务无效 执行其他任务
 	{
        stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
//        if(stProtocol_Analyse[com].stAnalyse_ctrl.ucCmdMode == CONRS485SOURTIMER)
//        {
//            stTimTaskExeInfo.ucCurTmState = CON_TASK_RETDATA_INVALID; 
//        }    
 	}    
 	
    return ucRet;
} 	
//在解析任务里调用普通任务,中继任务等发送处理
unsigned char TaskCmdSendHandle(unsigned char com, unsigned char *revmbx)
{
    unsigned char ucRet=RET_ERROR;
    
    if(com < MAX_TOTPORT_NUM)
    {    
        switch(stProtocol_Analyse[com].stAnalyse_ctrl.ucCmdMode)
        {
            case CONRS485SOURTIMER://azh 定时刷新任务
                ucRet = TaskCmdSendFrameByDi(com, revmbx);
                break;
            case CON_RELAYSOUR_RELAYTASK:
                ucRet = TaskRelaySendFrame(com, revmbx);
                break;
            case CON_RS485ASKOPESOUR_TASK:
                ucRet = TaskCmdSendFrameByDi(com, revmbx);
                break;
            case CON_RS485ASKOPESOUR_ALARMUP://告警上送实时数据的抄表
            case CON_RS485ASKOPESOUR_ALARMFORE:////报警确诊发起的抄表
                ucRet = AlarmCmdSendFrameByDi(com, revmbx);
                break;
            default:
                break;           
        }
    }
    return ucRet;
}
//填写定时数据抄读 
unsigned char Meter_FillTimerCommand(unsigned char ucTaskSource, unsigned char ucMPNo)
{
	unsigned char ucRet=RET_SUCCESS,ucTaskType;
	unsigned char ucMeterNo,i;
	

    switch(ucTaskSource)
    {
        case CONRS485SOURTIMER:
        {    
            Timer_FillRS485MeterDi();//填写要抄读的数据标识
            ucTaskType = ucTaskSource;
            break;
        }
//抄读用于确认告警的数据
    	case CONRS485SOUR_ALRAFFIRMTASK:					//报警确认任务0
    	case CONRS485SOUR_ALRAFFIRMTASK+1:					//报警确认任务1
    	case CONRS485SOUR_ALRAFFIRMTASK+2:					//报警确认任务2
    	case CONRS485SOUR_ALRAFFIRMTASK+3:					//报警确认任务3
    	case CONRS485SOUR_ALRAFFIRMTASK+4:					//报警确认任务4
    	case CONRS485SOUR_ALRAFFIRMTASK+5:					//报警确认任务5
    	case CONRS485SOUR_ALRAFFIRMTASK+6:					//报警确认任务6
    	case CONRS485SOUR_ALRAFFIRMTASK+7:					//报警确认任务7
    	case CONRS485SOUR_ALRAFFIRMTASK+8:					//报警确认任务8
    	case CONRS485SOUR_ALRAFFIRMTASK+9:					//报警确认任务9
    	{
    	    Alarm_FillRS485MeterDi(ucTaskSource);
    	    ucTaskType = CON_RS485ASKOPESOUR_ALARMFORE;//这里是任务类型 不是任务序号
    	    break;   
    	}    
//抄读告警携带数据
    	case CONRS485SOURALR_1:								//报警补招数据0
    	case CONRS485SOURALR_1+1:							//报警补招数据1
    	case CONRS485SOURALR_1+2:							//报警补招数据2
    	case CONRS485SOURALR_1+3:							//报警补招数据3
    	case CONRS485SOURALR_1+4:							//报警补招数据4
    	case CONRS485SOURALR_1+5:							//报警补招数据5
    	case CONRS485SOURALR_1+6:							//报警补招数据6
    	case CONRS485SOURALR_1+7:							//报警补招数据7
    	case CONRS485SOURALR_1+8:							//报警补招数据8
    	case CONRS485SOURALR_1+9:							//报警补招数据9
    	{
    	    Alarm_FillRS485MeterDi(ucTaskSource);
            ucTaskType = CON_RS485ASKOPESOUR_ALARMUP;//这里是任务类型 不是任务序号
    	    break;   
    	}
    	default:
        {
            stGRTaskExeInfo.ucTaskValidFlag = CON_TASK_RETDATA_INVALID;
            return RET_ERROR;
    	}
    }
    	
    //azh 这里不需要数据标识 所以要抄的数据标识在stSendDIList全局数组
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
        stGRTaskExeInfo.unInCom = ZIGBEE_CHL;//azh 测量点内部对应的端口号 
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
	#ifdef ZIGBEE_TEST//azh 171010
        stMPNoAddrList.stMPNoAddrRec[ucMeterNo].ucProtocolNo = CON_PROTOCOL_DL64507;//645规约2007
        memcpy(stMPNoAddrList.stMPNoAddrRec[ucMeterNo].ucMeterAddr, gMatch_HVMeter_Addr, 6);
        //如果测量点被删除 就会清高压表地址为全0 为了不让去抄表
        if((gMatch_HVMeter_Addr[0]!=0)||(gMatch_HVMeter_Addr[1]!=0)||(gMatch_HVMeter_Addr[2]!=0)
        ||(gMatch_HVMeter_Addr[3]!=0)||(gMatch_HVMeter_Addr[4]!=0)||(gMatch_HVMeter_Addr[5]!=0))
        {
            Fill_TimeTaskSendMbx(ucTaskType);
        }	
	#else
        stMPNoAddrList.stMPNoAddrRec[ucMeterNo].ucProtocolNo = CON_PROTOCOL_DL64507;//645规约2007
        memcpy(stMPNoAddrList.stMPNoAddrRec[ucMeterNo].ucMeterAddr, &stTerRunPara.stMeterPara[ucMPNo].stMeterAddr.ucMeterAddr[0], 6);
        //如果测量点被删除 就会清高压表地址为全0 为了不让去抄表
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
        stMPNoAddrList.stMPNoAddrRec[ucMeterNo].ucProtocolNo = CON_PROTOCOL_DL64507;//645规约2007
        memcpy(stMPNoAddrList.stMPNoAddrRec[ucMeterNo].ucMeterAddr, gMatch_HVMeter_Addr, 6);
        //如果测量点被删除 就会清高压表地址为全0 为了不让去抄表
        if((gMatch_HVMeter_Addr[0]!=0)||(gMatch_HVMeter_Addr[1]!=0)||(gMatch_HVMeter_Addr[2]!=0)
        ||(gMatch_HVMeter_Addr[3]!=0)||(gMatch_HVMeter_Addr[4]!=0)||(gMatch_HVMeter_Addr[5]!=0))
        {
            Fill_TimeTaskSendMbx(ucTaskType);
        }
#endif//CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
    }
    else
    {
        stGRTaskExeInfo.unInCom = RS4852_CHL;//azh 测量点内部对应的端口号  
//        ulBaudRate=Get_Correct_Boadrate(stTerRunPara.stMeterPara[ucMeterNo].ulBaudRate);
//        Uart_InitChBaud(ucChID,ulBaudRate,ucEPN,ucDataBit,ucStopBit);
        Fill_TimeTaskSendMbx(ucTaskType);

    }    	

	return ucRet;
}
/******************************************************************************
              End Of File
 *****************************************************************************/

/******************************************************************************
 Copyright (C) 2009  R&D Institute of Reallin Co., Ltd.
 Module         : protocol analyse function head file
 File Name      : protocol.h
 Description    :
 Author         :
 Build Date     :
 others         :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、     1.00       2009-10-13     snap.gao       create
******************************************************************************/

#ifndef __PROTOCOL_H
#define __PROTOCOL_H


/*----------------------------- macro definition ----------------------------*/
#define     MAX_REPEAT_NUM      3                   // 定义命令重发允许次数
#define     WAIT_REC_CNT        OS_DLY_1_5S         // 定义等待接收数据的延时值
#define     WAIT_LOGOUT_CNT     OS_DLY_120S         // 定义等待退出登陆的延时值
#define     MAX_LIST_NUM        4                   // 定义准备发送的命令列表的组的数量最大值
#define     MAX_COMM_DATA       256                 // 定义发给表计的命令帧缓冲区的最大字节数

#define     DATA_MODE           '0'         //数据模式
#define     PRG_MODE            '1'         //编程模式，可读写
#define     MFT_MODE            '2'         //工厂模式

#define     WRONG_PWD           '!'         //密码错误
#define     READER_PWD          '0'         //‘0’级密码，允许读取
#define     OPERATOR_PWD        '1'         //‘1’级密码，允许编程
#define     MASTER_PWD          '2'         //‘2’级密码，允许清除负荷曲线及事件
#define     SUPPER_PWD          '3'         //‘3’级密码，允许干任何事情

#define     NO_COMM             0           //没有发送标志
#define     HAVE_COMM           1           //有发送标志

#define     READ_COMM           (1<<0)      //读命令
#define     PRG_COMM            (1<<1)      //写命令
//

#define CON_RS485RECVTYPE_SUCCESS	0	//
#define CON_RS485RECVTYPE_RECVOVER	1	//接收超时

#define CON_RELAYRET_SUCCESS		    0x00
#define CON_RELAYRET_OVERTIME		    0x01
//
#define CON_PROTOCOL_DEFAULT_HANDLE     0   //默认解析处理
#define CON_PROTOCOL_WAITREV_HANDLE     1   //发送等待接收处理

/*----------------------------- type definition -----------------------------*/
typedef struct _STRELAYPARA					//中继参数
{
	unsigned char	ucPortBaudRate;			//波特率,主要用于中继任务的
	unsigned char	ucWaitTimeReserve;
	unsigned char	ucWaitTime;				//中继超时时间（秒）
	unsigned char	ucCharacterChar;		//特征字符
	unsigned char	ucReserve;
	unsigned short	usGetFrom;				//截取开始
	unsigned short	usGetLen;				//截取长度
}STRELAYPARA;

typedef	struct _STANALYSE_TCB
{
    U8 ucCmdMode;                   //收到处理命令 
    U8 ucPort;                      //端口号：中继命令：则是返回的端口号  其他：下行端口号即哪个端口抄表    
    U8 ucSeq;                       //序号：中继命令：上行帧序号     其他抄表：测量点号 
    U8 ucOptype;                    //操作类型：读或设置
    U32 ulDi;                       //数据标识
    U32 Tmp_Tick_Count;             // 缓冲Tick值
    U32 Rec_Tick_Count;             //上次收到数据时检查到的Tick值，判断字节超时和帧超时，超时后认为通讯结束，采用设置值
    U32 Rec_Dly;                    //接收超时设定时间10ms unit
}STANALYSE_TCB;

typedef	struct _STPROTOCOL_ANALYSE
{
	U8 ucWR_State;					    //当前工作口号
    STANALYSE_TCB stAnalyse_ctrl;       //其他参数
//    STRELAYPARA stRelayPara;            //上行中继命令相关参数
    STRELAYTASKPARA stRelayPara;        //国网格式
    os_mbx_declare(Mbx_Send_HiBuf, 8);  // 高优先级发送队列
    os_mbx_declare(Mbx_Send_LoBuf, 8);  // 等待命令返回的邮箱
}STPROTOCOL_ANALYSE;

/*-------------------------- functions declaration --------------------------*/
__EXTERN    __task  void    protocol_analyse_task (void);
//__EXTERN    __task  void    protocol_status_task (void);

__EXTERN    U8 send_protocol_frame(U8 *send_buf, U8 com, U16 len, U8 type);
__EXTERN    U8 convert_com_index(U8 *buf);
__EXTERN    void init_analyse_manage(U8 com);
__EXTERN    U8 send_wait_rev_frame(U8 *send_buf, U8 com, U16 len, U8 memtype, U8 mode_type);

__EXTERN U8 Meter_FillTimerCommand(unsigned char ucTaskSource, unsigned char ucMPNo);

__EXTERN U8 Fill_RelaySendto_Mbx(U8 ucPort);//(STTERMSG *pstRecvTerMsg);
__EXTERN U8 Fill_645RelaySendto_Mbx(U8 incom, U8 *send_buf);
__EXTERN void RecvRelayCmd_Explain_Manage(STPROTOCOL_ANALYSE *pstProtocolAnalyse,unsigned char *pucRecvBuffer,unsigned char TimeOutFlag);
__EXTERN void Recv645RelayCmd_Explain_Manage(STPROTOCOL_ANALYSE *pstProtocolAnalyse,unsigned char *pucRecvBuffer,unsigned char TimeOutFlag);
__EXTERN U8 dis_self_handle_di(U8 com, U8 *rev_buf);
/*--------------------------- variable declaration --------------------------*/
__EXTERN STPROTOCOL_ANALYSE   stProtocol_Analyse[MAX_TOTPORT_NUM];
__EXTERN STRS485TASK    stRS485TaskSend,stRelaySend;//azh
__EXTERN STRELAYDATA    stRelayDataBuf;//临时放中继的收发帧 因为有升级帧比较长
    
#endif//__PROTOCOL_H
/******************************************************************************
                            End Of File
 *****************************************************************************/

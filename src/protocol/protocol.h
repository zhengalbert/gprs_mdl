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
   2��
   1��     1.00       2009-10-13     snap.gao       create
******************************************************************************/

#ifndef __PROTOCOL_H
#define __PROTOCOL_H


/*----------------------------- macro definition ----------------------------*/
#define     MAX_REPEAT_NUM      3                   // ���������ط��������
#define     WAIT_REC_CNT        OS_DLY_1_5S         // ����ȴ��������ݵ���ʱֵ
#define     WAIT_LOGOUT_CNT     OS_DLY_120S         // ����ȴ��˳���½����ʱֵ
#define     MAX_LIST_NUM        4                   // ����׼�����͵������б������������ֵ
#define     MAX_COMM_DATA       256                 // ���巢����Ƶ�����֡������������ֽ���

#define     DATA_MODE           '0'         //����ģʽ
#define     PRG_MODE            '1'         //���ģʽ���ɶ�д
#define     MFT_MODE            '2'         //����ģʽ

#define     WRONG_PWD           '!'         //�������
#define     READER_PWD          '0'         //��0�������룬�����ȡ
#define     OPERATOR_PWD        '1'         //��1�������룬������
#define     MASTER_PWD          '2'         //��2�������룬��������������߼��¼�
#define     SUPPER_PWD          '3'         //��3�������룬������κ�����

#define     NO_COMM             0           //û�з��ͱ�־
#define     HAVE_COMM           1           //�з��ͱ�־

#define     READ_COMM           (1<<0)      //������
#define     PRG_COMM            (1<<1)      //д����
//

#define CON_RS485RECVTYPE_SUCCESS	0	//
#define CON_RS485RECVTYPE_RECVOVER	1	//���ճ�ʱ

#define CON_RELAYRET_SUCCESS		    0x00
#define CON_RELAYRET_OVERTIME		    0x01
//
#define CON_PROTOCOL_DEFAULT_HANDLE     0   //Ĭ�Ͻ�������
#define CON_PROTOCOL_WAITREV_HANDLE     1   //���͵ȴ����մ���

/*----------------------------- type definition -----------------------------*/
typedef struct _STRELAYPARA					//�м̲���
{
	unsigned char	ucPortBaudRate;			//������,��Ҫ�����м������
	unsigned char	ucWaitTimeReserve;
	unsigned char	ucWaitTime;				//�м̳�ʱʱ�䣨�룩
	unsigned char	ucCharacterChar;		//�����ַ�
	unsigned char	ucReserve;
	unsigned short	usGetFrom;				//��ȡ��ʼ
	unsigned short	usGetLen;				//��ȡ����
}STRELAYPARA;

typedef	struct _STANALYSE_TCB
{
    U8 ucCmdMode;                   //�յ��������� 
    U8 ucPort;                      //�˿ںţ��м�������Ƿ��صĶ˿ں�  ���������ж˿ںż��ĸ��˿ڳ���    
    U8 ucSeq;                       //��ţ��м��������֡���     ��������������� 
    U8 ucOptype;                    //�������ͣ���������
    U32 ulDi;                       //���ݱ�ʶ
    U32 Tmp_Tick_Count;             // ����Tickֵ
    U32 Rec_Tick_Count;             //�ϴ��յ�����ʱ��鵽��Tickֵ���ж��ֽڳ�ʱ��֡��ʱ����ʱ����ΪͨѶ��������������ֵ
    U32 Rec_Dly;                    //���ճ�ʱ�趨ʱ��10ms unit
}STANALYSE_TCB;

typedef	struct _STPROTOCOL_ANALYSE
{
	U8 ucWR_State;					    //��ǰ�����ں�
    STANALYSE_TCB stAnalyse_ctrl;       //��������
//    STRELAYPARA stRelayPara;            //�����м�������ز���
    STRELAYTASKPARA stRelayPara;        //������ʽ
    os_mbx_declare(Mbx_Send_HiBuf, 8);  // �����ȼ����Ͷ���
    os_mbx_declare(Mbx_Send_LoBuf, 8);  // �ȴ�����ص�����
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
__EXTERN STRELAYDATA    stRelayDataBuf;//��ʱ���м̵��շ�֡ ��Ϊ������֡�Ƚϳ�
    
#endif//__PROTOCOL_H
/******************************************************************************
                            End Of File
 *****************************************************************************/

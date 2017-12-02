

#ifndef _MAIN_DOWN_EXPLAIN_H_
#define	_MAIN_DOWN_EXPLAIN_H_

#ifndef MAX_ALLITEMCONT_LEN
#define MAX_ALLITEMCONT_LEN    1536//azh 1024*4
#endif
//
__EXTERN STTERMSG	stTerMsgList[CONTERMSGNUM];		//接收报文队列
__EXTERN STTERMSG	stTerMsgTmp;						
__EXTERN STEXPLAINRETDATA	stExplainRetData;    

__EXTERN unsigned char	Main_Init_Protocol(void);
__EXTERN unsigned char	Main_Recv_Explain(unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned char *pucRetTerMsgNum,unsigned short *pusRetPos,unsigned char ucMsgSource,unsigned char ucJudgeFlag,unsigned char ucSeqNo);
__EXTERN unsigned char	Get_Ret_MsgType(unsigned char ucMsgType);
__EXTERN unsigned char	Main_InsertGroupData(unsigned char ucRetTerMsgNum, unsigned char *pret_mem);
__EXTERN unsigned char	Main_Get_CommandLen(STDIINFO *pstRetDIInfo,unsigned char ucRepType,unsigned short usItemLabel,unsigned char *pucRecvBuffer);
/*unsigned char	Main_Get_Spec_CommandLen(unsigned char ucRepType,unsigned short usItemLabel,unsigned char *pucBuffer);*/
__EXTERN unsigned short	Main_Get_DataLen(STDIINFO *stRetDIInfo,unsigned char ucRepType,unsigned short usItemLabel,unsigned char *pucRecvBuffer);	
__EXTERN unsigned char	Main_RecvProc_Judge(unsigned char *pucRecvBuffer,unsigned short usRecvLen,unsigned short *pusProcDataLen,unsigned char ucMsgSource);
__EXTERN unsigned char	Manage_Group_ExtData(void);
__EXTERN unsigned char	Clr_Group_RetData(void);
//void CRC16_Init_Table(void);
__EXTERN U8 Main_GetDADTList(U8 *pucRecvBuffer, U16 *pusDAList, U8 *pucDTList);

#endif


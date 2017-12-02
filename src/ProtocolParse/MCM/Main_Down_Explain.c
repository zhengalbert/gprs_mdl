/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_Down_Explain.c
 Description    : ������Լ��վ���б��Ĵ���
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-02-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

//U16   CRC16_REVERSE_TABLE[256];           //CRC16��ʽ��

static U8   Main_RecvProc_CtrlByte(U8 ucCtrlByte,U8 ucRepType);
static U8   Main_RecvProc_Addr(U8 *pucAddr,U8 ucAddrLen);
static U8   Main_RecvProc_SEQ(U8 ucSEQ,U8 *pucRecvBuffer,U16 usPos);
static U8   Main_RecvProc_Pass(U8 *pucRecvBuffer,U16 usDataLen,U8 ucSEQ);
static U8   Main_RecvProc_Len(U8 *pucRecvBuffer,U16 usDataLen,U8 ucSEQ);

static U8   Main_GetRepType(U8 *pucRecvBuffer,U16 usRecvLen);
static U8   Main_GetAuxDataLen(U8 ucRepType,U8 ucSeq);
static U8 Get_OneGroup_RetData(STTERMSG * pstTerMsg,U8 ucRetTerMsgNum, U8 *pret_mem);
static U8   SetRetData(U8 *pucBuffer,U16 usLen, U8 *pret_mem);
static U16  GetOlnyTerMsgLen(STTERMSG *pstTerMsg);
static U8   Get_Group_ExtData(STTERMSG * pstTerMsg,U8 ucRetTerMsgNum);
static U8   Clr_GroupExtData_AllACK(void);
static U8   Clr_GroupExtData_AllNAK(void);
static U8   Clr_GroupExtData_PartACK(U8 ucMsgType);
static U8   Is_GroupExtData_AllACK(void);
static U8   Is_GroupExtData_AllNAK(void);
static U8   Is_GroupExtData_PartNAK(void);
static U8   Is_Cmd_SetDateTime(U8 *pucAddrBuffer);
//void CRC16_Reverse_Generate_Table(U16 CRC16_Key);
//U16   CRC16_Reverse_Lookup_Table(U8* ptr, unsigned int length);

U8 Main_Init_Protocol(void)
{
    U8 ucRet= RET_SUCCESS,i=0;
	for(i=0;i<CONTERMSGNUM;i++)
		ClearTerMsg(&stTerMsgList[i]);
	memset(&(stExplainRetData),0,sizeof(stExplainRetData));
//	CRC16_Init_Table();
	return ucRet;
}

//void CRC16_Init_Table(void)
//{
//  U16 usCRC16_Key;
//	STTERENCRYPT  stTerEncrypt;
//	stTerEncrypt=RunPara_GetTerEncrypt();
//	if(stTerEncrypt.ucAlgoCode==CON_ENCRYPT_CRC)
//	{
//		usCRC16_Key=stTerEncrypt.ucPassword[0]+0x100*stTerEncrypt.ucPassword[1];
//		CRC16_Reverse_Generate_Table(usCRC16_Key);
//	}
//}

///********************************************************************
//	�������ƣ�		CRC16_Reverse_Generate_Table
//	���ú���:		
//	��    ��:		����CRC16(�����㷨)���ֽڼ������ʽ�����ұ�
//	��ڲ���:		
//					CRC16_Key	:	CRC����ʽ
//	�� �� ֵ��		
//
//	��������:		��������ʽ�������CRC16_REVERSE_TABLE[]
//*********************************************************************/
//void CRC16_Reverse_Generate_Table(U16 CRC16_Key)
//{
//  U16 i,j,CRC;
//  U8  data;
//	for( i=0; i<256; i++)
//	{
//      data = (U8)i;
//		CRC	 = 0;
//		for ( j=0; j<8; j++ )                                             
//		{                                                              
//			if ( ( data ^ CRC ) & 0x0001 )                                 
//				CRC = ( CRC >> 1 ) ^ CRC16_Key;                                 
//			else                                                           
//				CRC >>= 1;                                                  
//			data >>= 1;                                                    
//		}                                                              
//		CRC16_REVERSE_TABLE[i] = CRC;
//	}
//}
//
///********************************************************************
//	�������ƣ�		CRC16_Reverse_Lookup_Table
//	���ú���:		
//	��    ��:		���ֽڼ���CRC16(�����㷨) 
//	��ڲ���:		
//					ptr			:	��Ҫ�����������ָ��
//					length		:	���ݳ���(���ֽڵ�λ)
//
//	�� �� ֵ��		������ȷ	:	CRC��ʽ
//					��������	:	0
//
//	��������:		��Ҫ��ǰ��ʼ��CRC16����ʽ��CRC16_REVERSE_TABLE[]
//*********************************************************************/
//U16   CRC16_Reverse_Lookup_Table(U8* ptr, unsigned int length)
//{
//	unsigned int	CRC=0,i=0;
//  U8  lsb;
//	
//	if(length==0||ptr==NULL)
//		return 0;
//
//	while(length!=0)
//	{
//		lsb = CRC&0xff;
//		CRC >>=8;
//		CRC ^= CRC16_REVERSE_TABLE[ lsb^(*ptr) ];
//		ptr++;
//		length--;
//	}	
//	return CRC;
//}
U8 Job_Data_Explain(U8 *pucBuffer,U8 usLen,U8 ucRepType,
                               U8 *pucRetTerMsgNum)
{
//    U8 ucRetRepType=CON_TERMSG_TYPE_INVALID;
    U8 ucRet=RET_SUCCESS;
    U8  ucDTList[8];
    U16 usItemLabelList[8],usDAList[8];
    U16     i=0,j=0,m=0;
    U8  ucTerMsgListNum=0;
    U8  ucDataItemNum=0;
	STDIINFO        stRetDIInfo;
    U16 usDataItemLen=0;
    U8  ucLenChangFlag=CON_STATUS_NO;

	memset(ucDTList,0xFF,8);
	memset(usDAList,0xFF,8*2);
	memset(usItemLabelList,0xFF,8);
//  Main_GetDAList(pucBuffer, usDAList);            //��ȡ��Ϣ���б�
//  Main_GetDTList(pucBuffer, ucDTList);            //��ȡ��Ϣ���б�
	Main_GetDADTList(pucBuffer, usDAList, ucDTList);	//��ȡ��Ϣ�㡢��Ϣ���б�	
	Main_GetItemLabelList(ucRepType,ucDTList,usItemLabelList);
	m+=4;
	for(i=0;i<8;i++)
	{//������֯��˳������Ȱ�pn��С�����ٰ�Fn��С����Ĵ��򣬼������һ����Ϣ��pi��������Ϣ��Fn�Ĵ������ٽ�����һ��pi+1�Ĵ���
		if(usDAList[i]==0xFFFF)
		break;
		SetTerMsgPer(&stTerMsgList[ucTerMsgListNum],0xBB,0xBB,ucRepType);
		for(j=0;j<8;j++)
		{
			if(usItemLabelList[j]!=CON_DI_TYPE_INVALID)
			{
				ucDataItemNum=stTerMsgList[ucTerMsgListNum].ucDataItemNum;
//				ucRetRepType=Get_Ret_MsgType(ucRepType);
                usDataItemLen = Main_Get_CommandLen(&stRetDIInfo,ucRepType,usItemLabelList[j],(U8*)(pucBuffer+m));
                stTerMsgList[ucTerMsgListNum].stDataItem[ucDataItemNum].ucMPNo      =(U8)usDAList[i];
				stTerMsgList[ucTerMsgListNum].stDataItem[ucDataItemNum].usItemLabel	=usItemLabelList[j];
				stTerMsgList[ucTerMsgListNum].stDataItem[ucDataItemNum].ucDataContent=pucBuffer+m;
				stTerMsgList[ucTerMsgListNum].stDataItem[ucDataItemNum].usDataLen = usDataItemLen;				
				stTerMsgList[ucTerMsgListNum].ucDataItemNum++;
				m+=usDataItemLen;
				if(ucRepType==CON_TERMSG_REQDATA_3 || ucRepType==CON_TERMSG_FILETRANS 
					|| ucRepType==CON_TERMSG_DATATRANS || ucRepType==CON_TERMSG_EXTCMD 
					|| ucLenChangFlag==CON_STATUS_YES)//��������ȡ��
					break;									//��������һ��DT		//���ȳ������Ƴ��ȣ������Ǻ������ݣ����Ը���������Ϊ���һ��
			}
		}//DT
		ucTerMsgListNum++;
	}
	(*pucRetTerMsgNum)	= ucTerMsgListNum;
	
	return ucRet;
}

/******************************************************************************
** ��������: U8 Main_Recv_Explain(U8 *pucRecvBuffer,U16 usRecvLen, U8 *pucRetTerMsgNum,
								U16 *pusRetPos, U8 ucMsgSource, U8 ucJudgeFlag, U8 ucSeqNo)
** ��������: ���յ��ı��Ľ�����䱨�Ľṹ���ֽ�ɶ������һ��DADT���
** ��ڲ���:  pucRecvBuffer:���Վ�ͷ��ַ,usRecvLen:��֡���ȣ�
**            *pucRetTerMsgNum:����֡�����ĵ�ַ,������ı�õ�ַ�������
** 					  *pusRetPos:�����ʼ�ĵ�ַ,������ı�õ�ַ�������
							ucMsgSource:��Ϣ��Դ��
              ucJudgeFlag��      ucSeqNo      
** �� �� ֵ: 
** ˵    ��: ���յ��ı��Ľ�����䱨�Ľṹ���ֽ�ɶ������һ��DADT���
**           �ȷֽ��豸�㣬��ֽ���Ϣ�㣬������Ϣ���ѯ���ݵ㳤��
******************************************************************************/
U8 Main_Recv_Explain(U8 *pucRecvBuffer, U16 usRecvLen, U8 *pucRetTerMsgNum,
								U16 *pusRetPos, U8 ucMsgSource, U8 ucJudgeFlag,	U8 ucSeqNo)
{//���쳣�ж��У���ȷ�ϴ˴���RecvBufferΪ��ȫ�Ϸ�
	U8	ucRet=RET_SUCCESS;
	U8	i=0,j=0;
	U8	ucRepType=CON_TERMSG_TYPE_INVALID;
	U8	ucDTList[8];
	U16	usItemLabelList[8],usDAList[8];
	U16	usDataItemLen=0,usTmpLen;
	U8	ucTerMsgListNum=0;
	U8	ucDataItemNum=0;
	U16  m=0;
	U8	ucLenChangFlag=CON_STATUS_NO;
	STDIINFO      stRetDIInfo;
	
	memset(ucDTList,0xFF,8);
	memset(usDAList,0xFF,8*2);
	(*pucRetTerMsgNum)=0;
	for(i=0;i<CONTERMSGNUM;i++)
		ClearTerMsg(&stTerMsgList[i]);
	
	m=(*pusRetPos);
	ucRepType = pucRecvBuffer[12];													//��ñ�����������
// 	Main_GetDAList(pucRecvBuffer + m, usDAList);			//��ȡ��Ϣ���б�
// 	Main_GetDTList(pucRecvBuffer + m, ucDTList);			//��ȡ��Ϣ���б�
	Main_GetDADTList(pucRecvBuffer + m, usDAList, ucDTList);	//��ȡ��Ϣ�㡢��Ϣ���б�	
	Main_GetItemLabelList(ucRepType,ucDTList,usItemLabelList);
	m+=4;
	usTmpLen=usRecvLen-m-Main_GetAuxDataLen(ucRepType,pucRecvBuffer[13])-2;	//[13]=SEQ
	for(i=0;i<8;i++)
	{//������֯��˳������Ȱ�pn��С�����ٰ�Fn��С����Ĵ��򣬼������һ����Ϣ��pi��������Ϣ��Fn�Ĵ������ٽ�����һ��pi+1�Ĵ���
		if(usDAList[i]==0xFFFF)
			break;
		SetTerMsgPer(&stTerMsgList[ucTerMsgListNum],ucSeqNo,ucMsgSource,ucRepType);
		for(j=0;j<8;j++)
		{
			if(usItemLabelList[j]!=CON_DI_TYPE_INVALID)
			{
				ucDataItemNum=stTerMsgList[ucTerMsgListNum].ucDataItemNum;
                usDataItemLen = Main_Get_CommandLen(&stRetDIInfo,ucRepType,usItemLabelList[j],(U8*)(pucRecvBuffer+m));//,ucNum,ucNum2,ucNum3,ucNum4);
                stTerMsgList[ucTerMsgListNum].stDataItem[ucDataItemNum].ucMPNo      =(U8)usDAList[i];
				stTerMsgList[ucTerMsgListNum].stDataItem[ucDataItemNum].usItemLabel	=usItemLabelList[j];
				if(ucJudgeFlag == RET_PASS_ERR)
					stTerMsgList[ucTerMsgListNum].stDataItem[ucDataItemNum].ucExeRet=CON_NACK_ERR_PASSWORD;
				
				if(ucRepType==CON_TERMSG_FILETRANS)
				{						
					stTerMsgList[ucTerMsgListNum].stDataItem[ucDataItemNum].ucDataContent = pucRecvBuffer+m-2;
					memcpy(stTerMsgList[ucTerMsgListNum].stDataItem[ucDataItemNum].ucDataContent,&usTmpLen,2);
					stTerMsgList[ucTerMsgListNum].stDataItem[ucDataItemNum].usDataLen	=usTmpLen+2;
					usDataItemLen=usTmpLen;
				}else
				{
					stTerMsgList[ucTerMsgListNum].stDataItem[ucDataItemNum].ucDataContent=pucRecvBuffer+m;
					stTerMsgList[ucTerMsgListNum].stDataItem[ucDataItemNum].usDataLen	=usDataItemLen;
				}				
				
				stTerMsgList[ucTerMsgListNum].ucDataItemNum++;
				m+=usDataItemLen;
				if(ucRepType==CON_TERMSG_REQDATA_3 || ucRepType==CON_TERMSG_FILETRANS || 
					ucRepType==CON_TERMSG_DATATRANS || ucRepType==CON_TERMSG_EXTCMD || 
					ucLenChangFlag==CON_STATUS_YES)//��������ȡ��
					break;									//��������һ��DT		//���ȳ������Ƴ��ȣ������Ǻ������ݣ����Ը���������Ϊ���һ��
			}
		}//DT
		ucTerMsgListNum++;
		if(ucRepType==CON_TERMSG_REQDATA_3 || ucRepType==CON_TERMSG_FILETRANS || 
			ucRepType==CON_TERMSG_DATATRANS || ucRepType==CON_TERMSG_EXTCMD)
			break;											//��������һ��DA
	}//DA
	
	(*pucRetTerMsgNum)	= ucTerMsgListNum;
	if(ucLenChangFlag==CON_STATUS_YES)
		(*pusRetPos)=0xFFFF;
	else
		(*pusRetPos)=m;
	return ucRet;
}

//���յ��ı���֡��ʽ���
U8 Main_Protocol_CheckFrame(U8 *pucRecvBuffer,U16 usLen)
{
    U8 ucCheckSum;
    U16 i;
    U16 usDataLen;
	
	if(usLen<20)							//��С����Ϊ20�����κ���������
		return RET_TOO_LITTLE;
	if(pucRecvBuffer[0]!=0x68)				//֡ͷ
		return RET_HEAD_ERROR;
	if(pucRecvBuffer[5]!=0x68)
		return RET_ERROR;
	if((pucRecvBuffer[1]!=pucRecvBuffer[3]) || (pucRecvBuffer[2]!=pucRecvBuffer[4]))
		return RET_ERROR;
//	if((pucRecvBuffer[1] & 0x03) != 0x02)	//����96�汾��Լ
//		return RET_ERROR;
//	if((pucRecvBuffer[3] & 0x03) != 0x02)
//		return RET_ERROR;
	usDataLen=(pucRecvBuffer[1]>>2)+pucRecvBuffer[2]*0x40;
	if(usLen<(usDataLen+8))
		return RET_NOENOUGH_LONG;
	ucCheckSum=0;							//���У����
	for(i=0;i<usDataLen;i++)
		ucCheckSum+=pucRecvBuffer[i+6];
	if(ucCheckSum!=pucRecvBuffer[usDataLen+6])
		return RET_ERROR;
	if( pucRecvBuffer[usDataLen+7]!=0x16 )	//֡β
		return RET_ERROR;
//	(*pusRetPos)=(usDataLen+8);
	
	return RET_SUCCESS;
}

U8 Get_Ret_MsgType(U8 ucMsgType)
{
    U8 ucRet=CON_TERMSG_TYPE_INVALID;
	switch(ucMsgType)
	{
	case CON_TERMSG_ACK:			//ȷ�Ϸ���
		ucRet=CON_TERMSG_ACK;
		break;
	case CON_TERMSG_CONNECT:		//��·�ӿڼ��
		ucRet=CON_TERMSG_CONNECT;
		break;
	case CON_TERMSG_RELAYCMD:		//�м�
		ucRet=CON_TERMSG_RELAYCMD;
		break;
	case CON_TERMSG_RESET:			//��λ�������
	case CON_TERMSG_SETPARA:		//��������
	case CON_TERMSG_CONTROL:		//��������
		ucRet=CON_TERMSG_ACK;
		break;
	case CON_TERMSG_CONFIGURATION:	//�����ն�����
		ucRet=CON_TERMSG_CONFIGURATION;
		break;
	case CON_TERMSG_LOGIN:
		ucRet = CON_TERMSG_LOGIN;
		break;
	case CON_TERMSG_GETPARA:		//��ѯ�ն˲���
		ucRet=CON_TERMSG_GETPARA;
		break;
	case CON_TERMSG_TASKDATA:		//�������ݲ�ѯ
		ucRet=CON_TERMSG_TASKDATA;
		break;
	case CON_TERMSG_REQDATA_1:		//����1�����ݣ�ʵʱ���ݣ�
		ucRet=CON_TERMSG_REQDATA_1;
		break;
	case CON_TERMSG_REQDATA_2:		//����2�����ݣ��������ݣ�
		ucRet=CON_TERMSG_REQDATA_2;
		break;
	case CON_TERMSG_REQDATA_3:		//����3�����ݣ��¼���
		ucRet=CON_TERMSG_REQDATA_3;
		break;
	case CON_TERMSG_FILETRANS:		//�ļ�����
		ucRet=CON_TERMSG_FILETRANS;
		break;
	case CON_TERMSG_DATATRANS:		//����ת��
		ucRet=CON_TERMSG_DATATRANS;
		break;
	case CON_TERMSG_EXTCMD:			//��չ����
		ucRet=CON_TERMSG_EXTCMD;
		break;
	default:
		break;
	}
	return ucRet;
}

//������������
U8 Main_GetRepType(U8 *pucRecvBuffer,U16 usRecvLen)
{
    U8 ucRet=CON_TERMSG_TYPE_INVALID;
	switch(pucRecvBuffer[12])			//������
	{
	case 0x00:	ucRet=CON_TERMSG_ACK;			break;	//ȷ��		
	case 0x01:	ucRet=CON_TERMSG_RESET;			break;	//��λ�������		
	case 0x02:	ucRet=CON_TERMSG_CONNECT;		break;	//��·����		
	case 0x03:	ucRet=CON_TERMSG_RELAYCMD;		break;	//�м�����		
	case 0x04:	ucRet=CON_TERMSG_SETPARA;		break;	//��������	
	case 0x06:	ucRet=CON_TERMSG_LOGIN;			break;	//������֤����ԿЭ�̣�AFN=06H��
	case 0x09:	ucRet=CON_TERMSG_CONFIGURATION;	break;	//�����ն�����
	case 0x08:	ucRet = CON_TERMSG_DATAREQ_SLAVE;	break;
	case 0x05:	ucRet=CON_TERMSG_CONTROL;		break;	//��������		
	case 0x0A:	ucRet=CON_TERMSG_GETPARA;		break;	//��ѯ�ն˲���		
	case 0x0B:	ucRet=CON_TERMSG_TASKDATA;		break;	//�������ݲ�ѯ		
	case 0x0C:	ucRet=CON_TERMSG_REQDATA_1; 	break;	//����1�����ݣ�ʵʱ���ݣ�		
	case 0x0D:	ucRet=CON_TERMSG_REQDATA_2; 	break;	//����2�����ݣ��������ݣ�			
	case 0x0E:	ucRet=CON_TERMSG_REQDATA_3; 	break;	//����3�����ݣ��¼���		
	case 0x0F:	ucRet=CON_TERMSG_FILETRANS; 	break;	//�ļ�����		
	case 0x10:	ucRet=CON_TERMSG_DATATRANS; 	break;	//����ת��		
	case 0xC0:	ucRet=CON_TERMSG_EXTCMD;		break;	//��չ����			
	default:
		break;
	}
	return ucRet;
}

// U8   Main_GetDTList(U8 *pucRecvBuffer, U8 *pucDTList)
// {//��ȡ��Ϣ���б�
//  U8 ucRet=RET_SUCCESS,i=0;
//  U8 ucDT1,ucDT2;
//  U8 ucCount=0;
// 	
// 	ucDT1 = pucRecvBuffer[2];
// 	ucDT2 = pucRecvBuffer[3];
// 	if(ucDT2<31 && ucDT1>0)							//>=31δ����
// 	{
// 		for(i=0;i<8;i++)
// 		{
// 			if((ucDT1 & 0x01) == 0x01)
// 			{
// 				pucDTList[ucCount++] =i+ucDT2*0x08+1;	//��1��ʼ
// 			}
// 			ucDT1 = ucDT1>>1;
// 		}
// 	}
// 	return ucRet;	
// }

U8 Main_GetItemLabelList(U8 ucRepType,U8 *pucDTList,U16 *pusItemLabelList)
{
    U8 ucRet=RET_SUCCESS,i=0;
	for(i=0;i<8;i++)
		pusItemLabelList[i] = GetItemLabelbyDT(ucRepType,pucDTList[i]);
	
	return ucRet;
}
// /******************************************************************************
// ** ��������: U8	Main_GetDAList(U8 *pucRecvBuffer, U16 *pusDAList)
// ** ��������: ��ȡ��Ϣ���б�
// ** ��ڲ���: pucRecvBuffer:��Ϣ���ַ��pusDAList�������Ϣ�������
// **           �뿪ʼ���Ѽ�0x33��
// ** �� �� ֵ: RET_SUCCESS�����ɹ�
// ** ˵    ��: 
// ******************************************************************************/
// U8	Main_GetDAList(U8 *pucRecvBuffer, U16 *pusDAList)
// {//��ȡ��Ϣ���б�
//  U8 ucRet=RET_SUCCESS,i=0;
//  U8 ucDA1,ucDA2;
//  U8 ucCount=0;
// 	
// 	ucDA1 = pucRecvBuffer[0];
// 	ucDA2 = pucRecvBuffer[1];
// 	for(i=0;i<8;i++)
// 	{
// 		if((ucDA1 & 0x01) == 0x01)
// 		{
// 			pusDAList[ucCount++] =i+(ucDA2-1)*0x08;	//��0��ʼ
// 			if(ucCount>=8)					//���8��
// 				break;
// 		}
// 		ucDA1 = ucDA1>>1;
// 	}
// 	
// 	if(pusDAList[0]==0xFFFF)
// 		pusDAList[0] =CON_MPNO_TER_FIXED;
// 	
// 	return ucRet;	
// }

	
/******************************************************************************
** ��������: U8	Main_GetDADTList(U8 *pucRecvBuffer, U16 *pusDAList, U8 *pucDTList)
** ��������: ��ȡ��Ϣ�㡢��Ϣ���б�
** ��ڲ���: pucRecvBuffer:��Ϣ���ַ��pusDAList�������Ϣ������飬
**           pucDTList�������Ϣ�������
** �� �� ֵ: RET_SUCCESS�����ɹ�
** ˵    ��: 
******************************************************************************/
U8	Main_GetDADTList(U8 *pucRecvBuffer, U16 *pusDAList, U8 *pucDTList)
{
	U8 ucRet=RET_SUCCESS,i=0;
	U8 ucDA1,ucDA2,ucDT1, ucDT2;
	U8 ucDACnt=0;
	U8 ucDTCnt=0;
	
	ucDA1 = pucRecvBuffer[0];
	ucDA2 = pucRecvBuffer[1];
	ucDT1 = pucRecvBuffer[2];
	ucDT2 = pucRecvBuffer[3];	
	
	if(ucDT2<31 && ucDT1>0)							//>=31δ����
	{
	for(i=0;i<8;i++)
	{
			if((ucDT1 & 0x01) == 0x01)
			{
				pucDTList[ucDTCnt++] =i+ucDT2*0x08+1;	//��1��ʼ
			}
			ucDT1 >>= 1;			
		if((ucDA1 & 0x01) == 0x01)
		{
				pusDAList[ucDACnt++] =i+(ucDA2-1)*0x08;	//��0��ʼ
			}
			ucDA1 >>= 1;
		}
	}
	
	if(pusDAList[0]==0xFFFF)
		pusDAList[0] =CON_MPNO_TER_FIXED;
	
	return ucRet;	
}

//�����Ϣ������ԣ�Ŀǰֻ������Ϣ���������ݵ�ĳ���
U8 Main_Get_CommandLen(STDIINFO *pstRetDIInfo, U8 ucRepType, U16 usItemLabel, U8 *pucRecvBuffer)
{
	
    U8 ucLen=0;
    U8  ucNum2=0,ucNum4=0;
	
//	ucNum = pucRecvBuffer[0];
	ucNum2 = pucRecvBuffer[8];
//	ucNum3 = pucRecvBuffer[4];
	ucNum4 = pucRecvBuffer[1];
	ClearDIInfo(pstRetDIInfo);
	
	switch(ucRepType)
	{
	case CON_TERMSG_ACK:			//ȷ�Ϸ���
		if(usItemLabel==CON_ACK_F3)
			SetDIInfo(pstRetDIInfo,
			CON_DI_HAVE,									
			CON_DI_TYPE_PARA,		CON_DI_MPTYPE_TERSELF,	
			CON_DI_OPERATE_ENABLE,	CON_DI_SETPURVIEW_ADMIN,
			CON_DI_OPERATE_ENABLE,	CON_DI_OPERATE_DISABLE,	
			6,6,					CON_DI_TYPE_INVALID);			
		break;
	case CON_TERMSG_RESET:			//��λ�������
		break;
	case CON_TERMSG_RELAYCMD:		//�м�����
		break;
	case CON_TERMSG_SETPARA:		//��������
		Main_DI_Para_Query_Manage(pstRetDIInfo,usItemLabel,pucRecvBuffer);
		break;
	case CON_TERMSG_LOGIN:
		Main_DI_Login_Query_Manage(pstRetDIInfo,usItemLabel,ucNum4);//pucRecvBuffer);
		break;
	case CON_TERMSG_CONFIGURATION:		//��������
		Main_DI_Config_Query_Manage(pstRetDIInfo,usItemLabel,ucNum4);
		break;
	case CON_TERMSG_GETPARA:		//��ѯ�ն˲���
		Main_DI_ParaRead_Query_Manage(pstRetDIInfo,usItemLabel,pucRecvBuffer);	
		break;
	case CON_TERMSG_CONTROL:		//��������
		Main_DI_Ctrl_Query_Manage(pstRetDIInfo,usItemLabel,ucNum4);
		break;
	case CON_TERMSG_TASKDATA:		//��������
		Main_DI_Taskdata_Query_Manage(pstRetDIInfo,usItemLabel,ucNum4);
		break;
	case CON_TERMSG_REQDATA_1:		//����1�����ݣ�ʵʱ���ݣ�
	case CON_TERMSG_REQDATA_2:		//����2�����ݣ��������ݣ�
	case CON_TERMSG_REQDATA_3:		//����3�����ݣ��¼���
		Main_Comm_Data_Query_Manage(pstRetDIInfo,ucRepType,usItemLabel,pucRecvBuffer);
		break;
	case CON_TERMSG_FILETRANS:		//�ļ�����
		Main_DI_FileTrans_Query_Manage(pstRetDIInfo,usItemLabel);
		break;
	case CON_TERMSG_DATATRANS:		//����ת��
		Main_DI_DataTrans_Query_Manage(pstRetDIInfo,usItemLabel,pucRecvBuffer);
		break;
	case CON_TERMSG_EXTCMD:			//��չ����
		Main_DI_ExtCmd_Query_Manage(pstRetDIInfo,usItemLabel,ucNum2);
		break;
	}
	ucLen = pstRetDIInfo->ucCommandLen;			//ֻȷ��һ��
	return ucLen;
}


U16 Main_Get_DataLen(STDIINFO *stRetDIInfo,U8 ucRepType,U16 usItemLabel,U8 *pucRecvBuffer)
//U8 ucNum,U8 ucNum2,U8 ucNum3,U8 ucNum4)
{
	
    U16 ucLen=0;
	
    U8  ucNum2=0,ucNum4=0;
	
//	ucNum = pucRecvBuffer[0];
	ucNum2 = pucRecvBuffer[8];
//	ucNum3 = pucRecvBuffer[4];
	ucNum4 = pucRecvBuffer[1];
	ClearDIInfo( stRetDIInfo );
	
	switch(ucRepType)
	{
	case CON_TERMSG_ACK:			//ȷ�Ϸ���
		if(usItemLabel==CON_ACK_F3)
			ucLen =6;					//ֻȷ��һ��
		else
			ucLen =0;
		break;
	case CON_TERMSG_RESET:			//��λ�������
		ucLen =0;
		break;
	case CON_TERMSG_RELAYCMD:		//�м�����
		break;
	case CON_TERMSG_SETPARA:		//��������
		Main_DI_Para_Query_Manage(stRetDIInfo,usItemLabel,pucRecvBuffer);
		ucLen = stRetDIInfo->ucContentLen;
		break;
	case CON_TERMSG_LOGIN:
		Main_DI_Login_Query_Manage(stRetDIInfo,usItemLabel,ucNum4);//pucRecvBuffer);
		ucLen = stRetDIInfo->ucContentLen;
		break;
	case CON_TERMSG_CONFIGURATION:		//��������
		Main_DI_Config_Query_Manage(stRetDIInfo,usItemLabel,ucNum4);//pucRecvBuffer);
		ucLen = stRetDIInfo->ucContentLen;
		break;		
	case CON_TERMSG_GETPARA:		//��ѯ�ն˲���
		Main_DI_ParaRead_Query_Manage(stRetDIInfo,usItemLabel,pucRecvBuffer);	
		ucLen = stRetDIInfo->ucContentLen;
		break;
	case CON_TERMSG_CONTROL:		//��������
		Main_DI_Ctrl_Query_Manage(stRetDIInfo,usItemLabel,ucNum4);
		ucLen = stRetDIInfo->ucContentLen;
		break;
	case CON_TERMSG_TASKDATA:		//��������
		Main_DI_Taskdata_Query_Manage(stRetDIInfo,usItemLabel,ucNum4);
		break;
	case CON_TERMSG_REQDATA_1:		//����1�����ݣ�ʵʱ���ݣ�
	case CON_TERMSG_REQDATA_2:		//����2�����ݣ��������ݣ�
	case CON_TERMSG_REQDATA_3:		//����3�����ݣ��¼���
		Main_Comm_Data_Query_Manage(stRetDIInfo,ucRepType,usItemLabel,pucRecvBuffer);
		ucLen = stRetDIInfo->ucContentLen;
		break;
	case CON_TERMSG_FILETRANS:		//�ļ�����
		Main_DI_FileTrans_Query_Manage(stRetDIInfo,usItemLabel);
		ucLen = stRetDIInfo->ucContentLen;
		break;
	case CON_TERMSG_DATATRANS:		//����ת��
		Main_DI_DataTrans_Query_Manage(stRetDIInfo,usItemLabel,pucRecvBuffer);
		ucLen = stRetDIInfo->ucContentLen;
		break;
	case CON_TERMSG_EXTCMD:			//��չ����
		Main_DI_ExtCmd_Query_Manage(stRetDIInfo,usItemLabel,ucNum2);
		ucLen = stRetDIInfo->ucContentLen;
		break;
	}
	return ucLen;
}

//��ø�����Ϣ����
U8 Main_GetAuxDataLen(U8 ucRepType,U8 ucSeq)
{
    U8 ucLen=0;
	switch(ucRepType)									//��ÿ����
	{
	case CON_TERMSG_ACK:			//ȷ�Ϸ���
	case CON_TERMSG_RELAYCMD:		//�м�����
		break;
	case CON_TERMSG_RESET:			//��λ�������
	case CON_TERMSG_SETPARA:		//��������
	case CON_TERMSG_CONTROL:		//��������
	case CON_TERMSG_LOGIN:			//������֤����ԿЭ��
	case CON_TERMSG_DATATRANS:		//����ת��			//add by cfh 10-03-31
		ucLen += 16;
		break;
	case CON_TERMSG_GETPARA:		//��ѯ�ն˲���
	case CON_TERMSG_TASKDATA:		//�������ݲ�ѯ
	case CON_TERMSG_REQDATA_1:		//����1�����ݣ�ʵʱ���ݣ�
	case CON_TERMSG_REQDATA_2:		//����2�����ݣ��������ݣ�
	case CON_TERMSG_REQDATA_3:		//����3�����ݣ��¼���
	case CON_TERMSG_CONFIGURATION:	//�����ն�����
		break;
	case CON_TERMSG_FILETRANS:		//�ļ�����
//	
//		ucLen += 2;	//azh
		break;
	case CON_TERMSG_EXTCMD:			//��չ����
		ucLen += 2;
		break;
	default:
		break;
	}
	if((ucSeq & CON_FRAME_SEQ_TPV)==CON_FRAME_SEQ_TPV)			//ʱ�䳤��
		ucLen += 6;
	
	return ucLen;
}

/********************************************************************************************************/
//stExplainRetData��������
U8 Main_InsertGroupData(U8 ucRetTerMsgNum, U8 *pret_mem)
{//һ��DADT���
    U8 ucRet=RET_SUCCESS;
//  U8 ucNum=0,i=0;
//  U8 ucItemBegPos=0,ucMsgBegPos=0;


	Get_Group_ExtData(&stTerMsgList[0], ucRetTerMsgNum);
	ucRet =	Get_OneGroup_RetData(&stTerMsgList[0], ucRetTerMsgNum, pret_mem);
		
	return ucRet;
}

//��ȡ��չ��Ϣ(��Ҫ����ȷ��/���ϵĴ���)
U8 Get_Group_ExtData(STTERMSG * pstTerMsg,U8 ucRetTerMsgNum)
{
    U8 ucRet=RET_SUCCESS;
    U8 i=0,j=0;
    U8 ucMsgType=0;
    U8 ucACKFlag=CON_STATUS_YES,ucNAKFlag=CON_STATUS_YES;
	
	ucMsgType=pstTerMsg->ucMsgType;
	if(ucMsgType == CON_TERMSG_RESET || ucMsgType == CON_TERMSG_SETPARA	|| ucMsgType == CON_TERMSG_CONTROL
			|| ucMsgType == CON_TERMSG_DATATRANS || ucMsgType == CON_TERMSG_DATAREQ_SLAVE )
	{
		for(i=0;i<pstTerMsg->ucDataItemNum;i++)
		{//���ͽ��׼��
			for(j=0;j<ucRetTerMsgNum;j++)
			{
				if(stTerMsgList[j].stDataItem[i].ucExeRet==CON_NACK_ERR_NORMAL)
					ucNAKFlag=CON_STATUS_NO;
				else
					ucACKFlag=CON_STATUS_NO;
			}
		}
		if(ucACKFlag == CON_STATUS_NO && ucNAKFlag == CON_STATUS_NO)		//����ȷ��
			Clr_GroupExtData_PartACK(ucMsgType);
		else if(ucACKFlag == CON_STATUS_YES && ucNAKFlag == CON_STATUS_NO)	//ȫȷ��,���ж�֮ǰ������
		{
			if(Is_GroupExtData_AllNAK() == RET_SUCCESS)		//ȫ����
				Clr_GroupExtData_PartACK(ucMsgType);
			else if(Is_GroupExtData_AllACK() == RET_SUCCESS)		//ȫȷ��
				Clr_GroupExtData_AllACK();
			else if(Is_GroupExtData_PartNAK() == RET_SUCCESS)	//����ȷ��
				Clr_GroupExtData_PartACK(ucMsgType);
			else
				Clr_GroupExtData_AllACK();
		}
		else if(ucACKFlag == CON_STATUS_NO && ucNAKFlag == CON_STATUS_YES)
		{
			if(Is_GroupExtData_AllNAK() == RET_SUCCESS)		//ȫ����
				Clr_GroupExtData_AllNAK();
			else if(Is_GroupExtData_AllACK() == RET_SUCCESS)		//ȫȷ��
				Clr_GroupExtData_PartACK(ucMsgType);
			else if(Is_GroupExtData_PartNAK() == RET_SUCCESS)	//����ȷ��
				Clr_GroupExtData_PartACK(ucMsgType);
			else
				Clr_GroupExtData_AllNAK();
		}
	}
	return ucRet;
}

U8 Manage_Group_ExtData()
{
    U8 ucRet=RET_SUCCESS;

	if(Is_GroupExtData_AllNAK() == RET_SUCCESS || Is_GroupExtData_AllACK() == RET_SUCCESS)		//ȫ����/ȫȷ��
		Clr_Group_RetData();

	return ucRet;
}
U8 SetRetData(U8 *pucBuffer,U16 usLen, U8 *pret_mem)
{
    U8 ucNum=0, ucRet=RET_SUCCESS;
//  U8 *Pucstart,*Pucend=ucFrameDataContent+MAX_ALLITEMCONT_LEN;
    U8 *Pucstart,*Pucend=pret_mem+MAX_ALLITEMCONT_LEN;//azh
	ucNum=stExplainRetData.ucExplainRetDataNum;
	if(ucNum==0)
		stExplainRetData.stExplainRetData[ucNum].ucBuff=pret_mem;
	else
		stExplainRetData.stExplainRetData[ucNum].ucBuff=stExplainRetData.stExplainRetData[ucNum-1].ucBuff+stExplainRetData.stExplainRetData[ucNum-1].usLen;
	
	Pucstart=stExplainRetData.stExplainRetData[ucNum].ucBuff+usLen;
	if(Pucstart>Pucend)
		ucRet=RET_MEM_OVER;
	else
	{
		stExplainRetData.ucExplainRetDataNum++;
		if(usLen>=CON_SEND_BLOCK_MAX_SIZE-22)
			usLen=CON_SEND_BLOCK_MAX_SIZE-22-1;
		memcpy(stExplainRetData.stExplainRetData[ucNum].ucBuff,pucBuffer,usLen);
		stExplainRetData.stExplainRetData[ucNum].usLen=usLen;
	}
	if(stExplainRetData.ucExplainRetDataNum>=CON_EXPLAIN_RET_DATA_BLOCK_NUM)
		ucRet=RET_MEM_OVER;
	return ucRet;
}
U16 GetOlnyTerMsgLen(STTERMSG *pstTerMsg)
{
    U16 usLen=0,i=0;
	for(i=0;i<pstTerMsg->ucDataItemNum;i++)
	{
		usLen=usLen+pstTerMsg->stDataItem[i].usDataLen;
	}
	return usLen;
	
}
//׼����Ӧ��ʩ
U8 Get_OneGroup_RetData(STTERMSG * pstTerMsg,U8 ucRetTerMsgNum, U8 *pret_mem)
{//һ��DADT���
	U8 mem_type, *mem_ptr = NULL;
	U8	ucRet=RET_SUCCESS;
	U8	i=0,j=0;
	U8	ucDT1,ucDT2,ucDA1,ucDA2;
	U8	ucDT1bak,ucDT2bak,ucDA1bak,ucDA2bak;
	U16 m=0,usLen=0,usTempLen=0,ucPos;
    //U16   m=*pusLen,ucMaxLen=*pusLen;
	U8	ucMsgType=0;
//  U16 ucTerMsgdatalen=0;//ÿ�������������������
	U8 *pucBuffer;
	U8 ucFlag = 1;
	U8 uctmp=0;

    
    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }    
	pucBuffer = mem_ptr;
	memset(pucBuffer,0x00,560);//512//azh 170919
	ucMsgType=pstTerMsg->ucMsgType;
	m=0;
	pucBuffer[0]=0; 		pucBuffer[1]=0;			//��ʼ��DADT=00 00 00 00 
	pucBuffer[2]=0;			pucBuffer[3]=0;
	m+=4;
	ucPos=0;
	//�ò��ֿ��ܳ��ֵ������
	// 1��һ�������������������ܷ���
	// 2���ŵ��м�Ĳ�����û�з�������
	//for(j=*ucMsgBegPos;j<ucRetTerMsgNum;j++)
	usLen=GetOlnyTerMsgLen(pstTerMsg);
	for(j=0;j<ucRetTerMsgNum;j++)
	{
		for(i=0;i<pstTerMsg->ucDataItemNum;i++)
		{
			Get_DA_by_MPNo(pstTerMsg[j].stDataItem[i].ucMPNo,&ucDA1,&ucDA2);
			Get_DT_by_ItemLabel(pstTerMsg[j].ucMsgType, pstTerMsg[j].stDataItem[i].usItemLabel ,&ucDT1,&ucDT2);

			ucDA1bak=pucBuffer[0+ucPos];
			ucDA2bak=pucBuffer[1+ucPos];
			ucDT1bak=pucBuffer[2+ucPos];
			ucDT2bak=pucBuffer[3+ucPos];
			pucBuffer[0+ucPos] |= ucDA1; 			pucBuffer[1+ucPos] |= ucDA2;
			pucBuffer[2+ucPos] |= ucDT1;			pucBuffer[3+ucPos] |= ucDT2;
			if(ucMsgType == CON_TERMSG_LOGIN)
			{
				if(pstTerMsg[j].stDataItem[i].usItemLabel == CON_LOGIN_F1_REQUEST)
				{
					uctmp = 1;
				}
				else if(pstTerMsg[j].stDataItem[i].usItemLabel == CON_LOGIN_F2_ANSWER)
					uctmp = 2;
			}
					
			if(ucMsgType == CON_TERMSG_RESET || ucMsgType == CON_TERMSG_SETPARA || ucMsgType == CON_TERMSG_CONTROL 
				||	ucMsgType == CON_TERMSG_DATATRANS || ucMsgType == CON_TERMSG_FILETRANS || uctmp == 1)
			{
				memcpy(pucBuffer+m,&(stTerMsgList[j].stDataItem[i].ucExeRet),1);
				m++;
				ucRet=RET_SUCCESS;
			}else if(ucMsgType == CON_TERMSG_GETPARA || ucMsgType == CON_TERMSG_TASKDATA || ucMsgType == CON_TERMSG_REQDATA_1 
				|| ucMsgType == CON_TERMSG_REQDATA_2 || ucMsgType == CON_TERMSG_EXTCMD||ucMsgType == CON_TERMSG_REQDATA_3 || 
				ucMsgType == CON_TERMSG_CONFIGURATION || uctmp == 2)
			{
				if(stTerMsgList[j].stDataItem[i].ucExeRet==CON_NACK_ERR_NOTEXE)
				{
					pucBuffer[0+ucPos] = ucDA1bak;
					pucBuffer[1+ucPos] = ucDA2bak;
					if(pucBuffer[0+ucPos]==0)
						pucBuffer[1+ucPos]=0;
					pucBuffer[2+ucPos] = ucDT1bak;
					pucBuffer[3+ucPos] = ucDT2bak;
					if(pucBuffer[2+ucPos]==0)
						pucBuffer[3+ucPos]=0;
					ucRet=RET_SUCCESS;
					if(ucMsgType == CON_TERMSG_REQDATA_3)
						break;
				}
				else
				{
					memcpy(pucBuffer+m,&(pstTerMsg[j].stDataItem[i].ucDataContent[0]),pstTerMsg[j].stDataItem[i].usDataLen);
					m+=pstTerMsg[j].stDataItem[i].usDataLen;
					ucRet=RET_SUCCESS;
					//�������
					// 1  ��i+1>= pstTerMsg->ucDataItemNum;
					// 2. i+1<pstTerMsg->ucDataItemNum
					
					if(i+1<pstTerMsg->ucDataItemNum)
					{
						usTempLen=m+pstTerMsg[j].stDataItem[i+1].usDataLen;						
					}					
					// 1  ��i+1>= pstTerMsg->ucDataItemNum;
					else if(j+1 < ucRetTerMsgNum)
					{
						usTempLen=m+pstTerMsg[j+1].stDataItem[0].usDataLen;
					}
					else//azh
					{
						break;
					}
					
					
					if(usTempLen>=CON_SEND_BLOCK_MAX_SIZE-22)//�������ݿռ��Ѿ��� 
					{
						if(i+1<pstTerMsg->ucDataItemNum)					   	
							ucFlag=00;  //azhһ������������������
						
						ucRet=SetRetData(pucBuffer,m,pret_mem);
						memset(pucBuffer,0x00,560);//512);//azh 170919
						if((i+1==pstTerMsg->ucDataItemNum)&&(j+1==ucRetTerMsgNum))//���滹��������
							m=0;
						else
							m=4;
						ucPos=0;
					}				
				}				
			}
			else
			{
				ucRet =RET_ERROR;				
			}
			
			if(ucRet == RET_MEM_OVER||ucRet ==RET_ERROR)
			{
				break;
			}
		}//i
		
		//������ȴ�����󳤶�����Ҫ�µĿռ�	,˵����һ������������������Ҫ�ֿ����
		if(((m+usLen)>=(CON_SEND_BLOCK_MAX_SIZE-22))||(ucFlag==00))
		{
			ucPos=m;
			if(j+1<ucRetTerMsgNum)//���滹��������
			{
				
				usTempLen=m+pstTerMsg[j+1].stDataItem[0].usDataLen+4;
				if(usTempLen>=CON_SEND_BLOCK_MAX_SIZE-22)
				{
					ucRet=SetRetData(pucBuffer,m,pret_mem);
					m=4;
					ucPos=0;
				}
				else
					m=m+4;
			}			
			
		}
		if(ucRet == RET_MEM_OVER)
			break;
		ucFlag=01;
		
	}//j
	if(m!=0&&ucRet==RET_SUCCESS)
		ucRet=SetRetData(pucBuffer,m,pret_mem);

	free_mem_block(mem_ptr);//Mem_1536_Byte
	return ucRet;
}

U8 Clr_Group_RetData(void)//�������
{
    U8 ucRet=RET_SUCCESS,i=0;
	stExplainRetData.ucExplainRetDataNum=0;

	for(i=0;i<CON_EXPLAIN_RET_DATA_BLOCK_NUM;i++)
	{
		stExplainRetData.stExplainRetData[i].usLen =0;
	}
	return ucRet;
}
U8 Clr_Group_RetExtData(void)//�������
{
    U8 ucRet=RET_SUCCESS;
	stExplainRetData.stExplainExtData.usLen=0;
	return ucRet;
}
U8 Clr_GroupExtData_AllACK()
{
    U8 ucRet=RET_SUCCESS;
	stExplainRetData.stExplainExtData.usLen=4;
	stExplainRetData.stExplainExtData.ucBuff[0]=0;
	stExplainRetData.stExplainExtData.ucBuff[1]=0;
	stExplainRetData.stExplainExtData.ucBuff[2]=1;
	stExplainRetData.stExplainExtData.ucBuff[3]=0;
	return ucRet;
}

U8 Clr_GroupExtData_AllNAK()
{
    U8 ucRet=RET_SUCCESS;
	stExplainRetData.stExplainExtData.usLen=4;
	stExplainRetData.stExplainExtData.ucBuff[0]=0;
	stExplainRetData.stExplainExtData.ucBuff[1]=0;
	stExplainRetData.stExplainExtData.ucBuff[2]=2;
	stExplainRetData.stExplainExtData.ucBuff[3]=0;
	return ucRet;
}

U8 Clr_GroupExtData_PartACK(U8 ucMsgType)
{
    U8 ucRet=RET_SUCCESS;
	stExplainRetData.stExplainExtData.usLen=5;
	stExplainRetData.stExplainExtData.ucBuff[0]=0;
	stExplainRetData.stExplainExtData.ucBuff[1]=0;
	stExplainRetData.stExplainExtData.ucBuff[2]=4;
	stExplainRetData.stExplainExtData.ucBuff[3]=0;
	stExplainRetData.stExplainExtData.ucBuff[4]=ucMsgType;
	return ucRet;
}

U8 Is_GroupExtData_AllACK()
{
    U8 ucRet=RET_ERROR;
	if(stExplainRetData.stExplainExtData.usLen ==4 && stExplainRetData.stExplainExtData.ucBuff[2]==1)	//ȫȷ��
		ucRet= RET_SUCCESS;
	return ucRet;
}

U8 Is_GroupExtData_AllNAK()
{
    U8 ucRet=RET_ERROR;
	if(stExplainRetData.stExplainExtData.usLen ==4 && stExplainRetData.stExplainExtData.ucBuff[2]==2)	//ȫ����
		ucRet= RET_SUCCESS;
	return ucRet;
}

U8 Is_GroupExtData_PartNAK()
{
    U8 ucRet=RET_ERROR;
	if(stExplainRetData.stExplainExtData.usLen ==5 && stExplainRetData.stExplainExtData.ucBuff[2]==4)	//����ȷ��
		ucRet= RET_SUCCESS;
	return ucRet;
}


/********************************************************************************************************/
U8 Main_RecvProc_Judge(U8 *pucRecvBuffer,U16 usRecvLen,U16 *pusProcDataLen,U8 ucMsgSource)
{
    U8 ucRet=RET_SUCCESS;
    U8 ucRepType;
    U8 ucSEQ,ucCtrlByte;
    U16 usDataLen;

	usDataLen=(pucRecvBuffer[1]>>2)+pucRecvBuffer[2]*0x40;
	ucRepType	=Main_GetRepType(pucRecvBuffer,usDataLen);				//��ñ�����������

	ucCtrlByte = pucRecvBuffer[6];
	ucRet = Main_RecvProc_CtrlByte(ucCtrlByte,ucRepType);					//�������ж�

	if(ucRet == RET_SUCCESS)
        ucRet=Main_RecvProc_Addr((U8*)(pucRecvBuffer+7),5);     //��ַ

	ucSEQ=pucRecvBuffer[13];
	if(ucRet == RET_SUCCESS)
        ucRet=Main_RecvProc_SEQ(ucSEQ,pucRecvBuffer,(U16)(usDataLen+6/*֡ͷ����*/-6/*ʱ���򳤶�*/));

	if(ucRet == RET_SUCCESS)
		ucRet=Main_RecvProc_Len(pucRecvBuffer,usDataLen,ucSEQ);				//�ж��ܳ��ȣ��Լ�ͬһDTDA�����е�DT�������Ӧһ��

	if(ucRet == RET_SUCCESS)
		ucRet=Main_RecvProc_Pass(pucRecvBuffer,usDataLen,ucSEQ);

	(*pusProcDataLen)=usDataLen-Main_GetAuxDataLen(ucRepType,ucSEQ);
	return ucRet;
}

U8 Main_RecvProc_CtrlByte(U8 ucCtrlByte,U8 ucRepType)
{
//    U8 ucRet=RET_CTRLB_ERR;
//    U8 ucLinkFN;
//
//	if( (ucCtrlByte & CON_FRAME_CMD_DIR_UP) ==0)
//	{
//		ucLinkFN = (ucCtrlByte & 0x0F);
//		if( (ucCtrlByte & CON_FRAME_CMD_PRM_ORG) == CON_FRAME_CMD_PRM_ORG )
//		{
//			if(ucLinkFN==1)				//���ͨMȷ��
//			{
//				if(ucRepType==CON_TERMSG_RESET)
//					ucRet = RET_SUCCESS;
//			}
//			if(ucLinkFN==4)				//���ͨM�޻ش𣬿�������AFN
//			{
//				ucRet = RET_SUCCESS;
//			}
//			if(ucLinkFN==9)				//����M��Ӧ֡	��·����
//			{
//				if(ucRepType==CON_TERMSG_CONNECT)
//					ucRet = RET_SUCCESS;
//			}
//			if(ucLinkFN==10)			//����M��Ӧ֡	����1������
//			{
//				if(ucRepType==CON_TERMSG_SETPARA || ucRepType==CON_TERMSG_CONTROL || ucRepType==CON_TERMSG_LOGIN)
//					ucRet = RET_SUCCESS;
//			}
//			if(ucLinkFN==11)			//����M��Ӧ֡	����2������
//			{
//				if(ucRepType==CON_TERMSG_RELAYCMD || ucRepType==CON_TERMSG_GETPARA ||
//					ucRepType==CON_TERMSG_REQDATA_1 || ucRepType==CON_TERMSG_REQDATA_2 || ucRepType==CON_TERMSG_REQDATA_3 ||
//					ucRepType==CON_TERMSG_FILETRANS || ucRepType==CON_TERMSG_DATATRANS || ucRepType==CON_TERMSG_EXTCMD ||
//					ucRepType==CON_TERMSG_CONFIGURATION || ucRepType==CON_TERMSG_TASKDATA ||ucRepType == CON_TERMSG_DATAREQ_SLAVE)
//					ucRet = RET_SUCCESS;
//				if(ucRepType==CON_TERMSG_TASKDATA)		//�����������ݿ��Զ�ȡ
//					ucRet = RET_SUCCESS;
//			}
//		}else
//		{
//			if(ucLinkFN==0)				//ȷ��	�Ͽ�
//			{
//				if(ucRepType==CON_TERMSG_ACK)
//					ucRet = RET_SUCCESS;
//			}
//			if(ucLinkFN==8)				//��Ӧ֡	�û�����
//			{
//				if(ucRepType==CON_TERMSG_FILETRANS || ucRepType==CON_TERMSG_EXTCMD || ucRepType==CON_TERMSG_ACK)
//					ucRet = RET_SUCCESS;
//			}
//			if(ucLinkFN==9)				//��Ӧ֡	���ϣ������ٻ�������
//			{
//				if(ucRepType==CON_TERMSG_ACK)
//					ucRet = RET_SUCCESS;
//			}
//			if(ucLinkFN==11)			//��Ӧ֡	��·״̬
//			{
//				if(ucRepType==CON_TERMSG_CONNECT || ucRepType==CON_TERMSG_ACK)
//					ucRet = RET_SUCCESS;
//			}
//		}
//	}
	return RET_SUCCESS;//ucRet;//azh ���ﲻ�жϵ���ô�ϸ�
}

U8 Main_RecvProc_Addr(U8 *pucAddr,U8 ucAddrLen)
{
    U8 ucRet=RET_ADDR_ERR,i=0;
	STTERGROUPADDRLIST	stTerGAddr		=RunPara_GetTerGroupAddrList();
	STTERREGIONADDR		stTerRegionAddr	=RunPara_GetTerRegionAddr();
	STTERDEVADDR		stTerAddr		=RunPara_GetTerDevAddr();
	STTERMISCPARA		stTerMiscPara;

	stTerMiscPara=RunPara_GetTerMiscPara();

	if(pucAddr[0] == 0xFF && pucAddr[1] == 0xFF && pucAddr[2] == 0xFF && pucAddr[3] == 0xFF && (pucAddr[4]&0x01) == 0x01)
	{//ȫ�㲥
		if(stTerMiscPara.ucRemoveOn!=CON_AVALIBLE_FLAG || Is_Cmd_SetDateTime(pucAddr)==RET_SUCCESS)
			ucRet=RET_SUCCESS;
	}

	if(ucRet == RET_ADDR_ERR )
	{
		if( (pucAddr[0] == stTerRegionAddr.ucRegionAddr[0] && pucAddr[1] == stTerRegionAddr.ucRegionAddr[1]) &&
			(pucAddr[2] == 0xFF && pucAddr[3] == 0xFF /*&& (pucAddr[4]&0x01) == 0x01*/))	//Ӧ������cfh 10-04-04
		{//��㲥
			if(stTerMiscPara.ucRemoveOn!=CON_AVALIBLE_FLAG || Is_Cmd_SetDateTime(pucAddr)==RET_SUCCESS)
				ucRet=RET_SUCCESS;
		}
	}

	if(ucRet == RET_ADDR_ERR )
	{
		if(	(pucAddr[0] == stTerRegionAddr.ucRegionAddr[0] && pucAddr[1] == stTerRegionAddr.ucRegionAddr[1]) &&
			(pucAddr[2] == stTerAddr.ucDevAddr[0]		   && pucAddr[3] == stTerAddr.ucDevAddr[1]) &&
			(pucAddr[4] & 0x01)==0x00)
			ucRet=RET_SUCCESS;
	}

	if(ucRet == RET_ADDR_ERR )
	{
		if(	(pucAddr[0] == stTerRegionAddr.ucRegionAddr[0] && pucAddr[1] == stTerRegionAddr.ucRegionAddr[1]) &&
			(pucAddr[4] & 0x01)==0x01)
		{
			for(i=0;i<CON_GROUPADDRNUM;i++)
			{
				if((pucAddr[2] == stTerGAddr.stGroupAddr[i].ucGroupAddr[0]) && (pucAddr[3] == stTerGAddr.stGroupAddr[i].ucGroupAddr[1]))
					break;
			}
			if(i<CON_GROUPADDRNUM)
				ucRet=RET_SUCCESS;
			if(ucRet==RET_SUCCESS)
			{
				if(stTerMiscPara.ucRemoveOn==CON_AVALIBLE_FLAG)
				{
					if(Is_Cmd_SetDateTime(pucAddr)!=RET_SUCCESS)
						ucRet=RET_ADDR_ERR;									//���ַ����£��Ƕ�ʱ������Ҫ�޳�
				}
			}
		}
	}

//	if((pucAddr[4]>>1)==0)					//��վ��Ӧ�նˣ��õ�ַΪ0
//		ucRet=RET_ADDR_ERR;

	return ucRet;
}

U8 Main_RecvProc_SEQ(U8 ucSEQ,U8 *pucRecvBuffer,U16 usPos)
{
    U8 ucRet=RET_SEQ_ERR;
    U8 ucTimeInter;
	STDATETIME	stDateTime,stDateTime2,stCurrentTime;

	stCurrentTime = GetCurrentTime();

	if((ucSEQ & CON_FRAME_SEQ_TPV)==CON_FRAME_SEQ_TPV)
	{
        stDateTime.ucSecond =(U8)BCD_TO_HEX_1(pucRecvBuffer+usPos+1);
        stDateTime.ucMinute =(U8)BCD_TO_HEX_1(pucRecvBuffer+usPos+2);
        stDateTime.ucHour   =(U8)BCD_TO_HEX_1(pucRecvBuffer+usPos+3);
        stDateTime.ucDay    =(U8)BCD_TO_HEX_1(pucRecvBuffer+usPos+4);
		stDateTime.ucMonth	=stCurrentTime.ucMonth;
		stDateTime.ucYear	=stCurrentTime.ucYear;		//���¹ػ������⣬������һ֡Ӧ��û����
		stDateTime2=stDateTime;
		ucTimeInter=pucRecvBuffer[usPos+5];
		if(ucTimeInter!=0 && Is_DateTime_Availble(&stDateTime)==RET_SUCCESS)
		{
			AddTime(&stDateTime,ucTimeInter,CON_TIMEUNIT_MIN);
			if(TimeIsOver(&stDateTime,&stCurrentTime)==RET_SUCCESS)
				ucRet=RET_SUCCESS;
			else
				ucRet=RET_SEQ_ERR;
			if(ucRet==RET_SUCCESS)
			{
				AddTime(&stCurrentTime,ucTimeInter,CON_TIMEUNIT_MIN);
				if(TimeIsOver(&stCurrentTime,&stDateTime2)==RET_SUCCESS)
					ucRet=RET_SUCCESS;
				else
					ucRet=RET_SEQ_ERR;
			}
		}

		if(ucTimeInter == 0)	//��Tp�е�����������ʱʱ��Ϊ"0"����Ӷ�վ�������ж�
			ucRet=RET_SUCCESS;
	}else
		ucRet=RET_SUCCESS;

	return ucRet;
}

U8 Main_RecvProc_Pass(U8 *pucRecvBuffer,U16 usDataLen,U8 ucSEQ)
{
    U8 ucRet=RET_PASS_ERR;
    U8 ucRepType;
    U8 ucPass[2];
    U8 ucPassErrTimes=0;
//  U16 usCRC;
	STTERENCRYPT  stTerEncrypt;

	ucRepType	=Main_GetRepType(pucRecvBuffer,usDataLen);		//��ñ�����������
	stTerEncrypt=RunPara_GetTerEncrypt();

	if(	ucRepType==CON_TERMSG_RESET		|| ucRepType==CON_TERMSG_SETPARA || ucRepType==CON_TERMSG_CONTROL || 
		ucRepType==CON_TERMSG_FILETRANS	|| ucRepType==CON_TERMSG_DATATRANS || ucRepType==CON_TERMSG_EXTCMD ||
		ucRepType==CON_TERMSG_LOGIN)
	{
		if((ucSEQ & CON_FRAME_SEQ_TPV)==CON_FRAME_SEQ_TPV)
		{
			ucPass[0]=pucRecvBuffer[usDataLen-2];
			ucPass[1]=pucRecvBuffer[usDataLen-1];
		}else
		{
			ucPass[0]=pucRecvBuffer[usDataLen+4];
			ucPass[1]=pucRecvBuffer[usDataLen+5];
		}
		stTerEncrypt.ucErrPassword[0]=ucPass[0];
		stTerEncrypt.ucErrPassword[1]=ucPass[1];
		RunPara_SetTerEncrypt(&stTerEncrypt);
		if(stTerEncrypt.ucAlgoCode==CON_ENCRYPT_DIR)
		{//����֤����
			ucRet = RET_SUCCESS;
		}else if(stTerEncrypt.ucAlgoCode==CON_ENCRYPT_CRC)
		{//crc�����㷨
//azh			usCRC=CRC16_Reverse_Lookup_Table(pucRecvBuffer+6,12);			//�����뿪ʼ12�ֽ�
//			if(usCRC==(ucPass[0]+0x100*ucPass[1]))
				ucRet = RET_SUCCESS;
		}else			//�����㷨��Ч
			ucRet = RET_SUCCESS;
		if(ucRet==RET_SUCCESS)
		{
			ucPassErrTimes=RunPara_GetPassErrTimes();
			if(ucPassErrTimes!=0)
			{
				if(RunPara_PassLockIsOverTime()==RET_SUCCESS)
					RunPara_ClearPassErrTimes();
				if(ucPassErrTimes>CON_MAX_PASSERR_TIMES)
					ucRet=RET_PASS_ERR;
			}
		}else
			RunPara_AddPassErrTimes();
	}else
		ucRet=RET_SUCCESS;

	return ucRet;
}

U8 Main_RecvProc_Len(U8 *pucRecvBuffer,U16 usDataLen,U8 ucSEQ)
{
    U8  ucRet=RET_SUCCESS,i=0;
    U16 usProcDataLen,usPos=0;
    U8  ucRepType;//,ucNum,ucNum2,ucNum3,ucNum4;
    U8  ucDTList[8];
    U16 usItemLabelList[8],usDAList[8];
    U8  ucDataItemLen=0;
    U8  ucLenChangFlag=CON_STATUS_NO;
    U8  ucDA=0;
	STDIINFO        stRetDIInfo;
	ucRepType		=Main_GetRepType(pucRecvBuffer,usDataLen);		//��ñ�����������
	usProcDataLen	=usDataLen-Main_GetAuxDataLen(ucRepType,ucSEQ);

	usPos=14;
	while(usPos	< (usProcDataLen+6))
	{//�������ķֽ�Ϊ������DTDA�飬ÿһ�����ݽ��ͽ�������stTerMsgList
		memset(ucDTList,0xFF,8);
		memset(usDAList,0xFF,8*2);
		ucDataItemLen=0xFF;

// 		Main_GetDAList(pucRecvBuffer + usPos, usDAList);			//��ȡ��Ϣ���б�
// 		Main_GetDTList(pucRecvBuffer + usPos, ucDTList);			//��ȡ��Ϣ���б�
		Main_GetDADTList(pucRecvBuffer + usPos, usDAList, ucDTList);	//��ȡ��Ϣ�㡢��Ϣ���б�		
		Main_GetItemLabelList(ucRepType,ucDTList,usItemLabelList);

		for(ucDA=0;ucDA<8;ucDA++)
		{//������֯��˳������Ȱ�pn��С�����ٰ�Fn��С����Ĵ��򣬼������һ����Ϣ��pi��������Ϣ��Fn�Ĵ������ٽ�����һ��pi+1�Ĵ���
			if(usDAList[ucDA]==0xFFFF || ucRet==RET_LEN_ERR || ucLenChangFlag==CON_STATUS_YES)
				break;
			for(i=0;i<8;i++)
			{
				if(usItemLabelList[i]!=CON_DI_TYPE_INVALID)
				{
                    ucDataItemLen=Main_Get_CommandLen(&stRetDIInfo,ucRepType,usItemLabelList[i],(U8*)(pucRecvBuffer+usPos+4));//,ucNum,ucNum2,ucNum3,ucNum4);
					if(ucDataItemLen==CON_DI_LEN_CHANGED)
					{
						ucLenChangFlag=CON_STATUS_YES;
						break;
					}
					if((usPos+ucDataItemLen+2)>(usDataLen+8))
					{
						ucRet=RET_LEN_ERR;
						break;
					}
					usPos+=ucDataItemLen;
				}else
				{
					if(i==0)
						ucRet=RET_LEN_ERR;
					break;
				}
			}//i
			
		}//ucDA
		if(ucRet==RET_LEN_ERR || ucLenChangFlag==CON_STATUS_YES)
			break;
		usPos+=4;	//DADT����=4
	}
	if(usPos!=(usProcDataLen+6) && ucLenChangFlag!=CON_STATUS_YES)
	{
		if(ucRepType==CON_TERMSG_FILETRANS)				//�ļ����䲻�����жϳ���
			ucRet=RET_SUCCESS;
		else
			ucRet=RET_LEN_ERR;
	}		
	return ucRet;
}

U8 Get_DA_by_MPNo(U8 ucMPNo,U8 *pucDA1,U8 *pucDA2)
{
    U8 ucRet=RET_SUCCESS;
	if(ucMPNo==CON_MPNO_TER_FIXED)
	{
		(*pucDA1)=0;
		(*pucDA2)=0;
	}else
	{
		(*pucDA1)	= 1<<(ucMPNo&0x07);
		(*pucDA2)		= (ucMPNo>>3) + 1;
//		(*pucDA2)	= 1<<(ucMPNo&0x07);
	}
	return ucRet;
}

U8 Is_Cmd_SetDateTime(U8 *pucAddrBuffer)
{//����Ƿ��Ƕ�ʱ����
    U8 ucRet=RET_ERROR;
	if(pucAddrBuffer[5]==CON_TERMSG_CONTROL && pucAddrBuffer[9]==0x40 && pucAddrBuffer[10]==0x03)			//���ƣ�F31
		ucRet=RET_SUCCESS;
	return ucRet;
}

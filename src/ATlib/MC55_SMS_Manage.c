/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : MC55_SMS_Manage.c
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2011-09-23      azh           the original version
******************************************************************************/
#include "..\inc\global_config.h"

//#include "MC55_SMS_Manage.h"
//#include "MC55_AT_MANAGE.H"
#include "MC55_AT_MANAGE.H"
#include "MC55_BufferManage.h"
#include "MC55_TASK_MANAGE.h"
#include "MC55_AT_TASK.H"

#include "MC55_sms_manage.h"


const char AssiicTable[]={"0123456789ABCDEF"};
const char sms_Dnumber_too_long[]={"sms number too long"};
const char sms_Enumber_too_long[]={"sms number too long"};

unsigned short AT_SMS_Encode(PSTTC35TASK pstDataContent)
{
	unsigned int	ucLength = 0;
	unsigned int	i;
	unsigned char   j = 0;
	unsigned short	usTotalLength = 0;
	PST_SMS_PARAM	pstSmsParam = 0;
	PST_COMMUNICATION_CHANNEL	pstComChannel = NULL;

	stSMSBufer.usTPALen = 0;
	stSMSBufer.usLength = 0;
	
	if( pstDataContent->ucChNo >= 3 ||
		pstDataContent->ucDataLength == 0 
		//||pstDataContent->ucType != TYPE_COMUNNICATION_SMS ||
		//pstDataContent->unCurAddrInfo.ucType != TYPE_COMUNNICATION_SMS 
		)
	{
		stSMSBufer.ucData[usTotalLength++] = 0x1a;
		stSMSBufer.usLength = usTotalLength;
		return 0;
	}

	stSMSBufer.usLength = 0;
	stSMSBufer.ucIsValid = 0;

	//��������
	ucLength = stModuleComParam.stCommonParam.ucSCALen ;
	if( ucLength > 16 || ucLength ==0 )
		return 0;
	
	//�б�ͨ������
	pstComChannel = &(stModuleComParam.stCh[pstDataContent->ucChNo]);
	if( pstComChannel->ucType != TYPE_COMUNNICATION_SMS )
	{
		return 0;
	}
	//��ȡ���Ų���
	pstSmsParam = &(pstComChannel->unChInfo.stSMSParam);

	//��վ���볤��
	ucLength = pstSmsParam->ucLen;
	if( ucLength > 16 || ucLength ==0 )
		return 0;

	usTotalLength = 0;

	//1) SCA����
	ucLength =  stModuleComParam.stCommonParam.ucSCALen ;
	ucLength++;
	ucLength >>= 1;
	ucLength++;
	//ucLength = (ucLength + 1)>>1 + 1;	//����Ϊż�����㲹F
	//sprintf((char*)(&stSMSBufer.ucData[usTotalLength]), "%2x", ucLength ) ;	
	ByteToHexAssiic(&(stSMSBufer.ucData[usTotalLength]), (unsigned char)ucLength);
	usTotalLength += 2;

	//2) 0x91
	//sprintf((char*)(&stSMSBufer.ucData[usTotalLength]), "%2x", 0X91 );		
	ByteToHexAssiic(&stSMSBufer.ucData[usTotalLength], 0x91);
	usTotalLength += 2;

	//3) �������ĺ���
	usTotalLength += SMS_Encode_PhoneNumber(&stSMSBufer.ucData[usTotalLength],
											stModuleComParam.stCommonParam.SCA,
											stModuleComParam.stCommonParam.ucSCALen,j);
	
	
	stSMSBufer.usTPALen = usTotalLength;	//

	//4) 0x1100
	//sprintf((char*)(&stSMSBufer.ucData[usTotalLength]), "%4x", 0X1100 );	//0x1100

	ByteToHexAssiic(&stSMSBufer.ucData[usTotalLength], 0x11);
	usTotalLength += 2;	
	ByteToHexAssiic(&stSMSBufer.ucData[usTotalLength], 0x00);
	usTotalLength += 2;

	//5) TPA���� 0x0d
	if(pstSmsParam->ucCode[0]=='8' && pstSmsParam->ucCode[1]=='6')
	      j=2;
      else
             j=0;
	ucLength = pstSmsParam->ucLen -j;
	//sprintf((char*)(&stSMSBufer.ucData[usTotalLength]), "%2x", ucLength );	
	ByteToHexAssiic(&stSMSBufer.ucData[usTotalLength], (unsigned char)ucLength);
	usTotalLength += 2;	
	
	//6)�̶�: 0x81�ù��Ҹ�ʽ���룬����ǰ����86// 0x91�ù��ʸ�ʽ���� ����ǰ��86
			ByteToHexAssiic(&stSMSBufer.ucData[usTotalLength], 0x81);
	usTotalLength += 2;	

	//7) TPA����
	usTotalLength += SMS_Encode_PhoneNumber( &stSMSBufer.ucData[usTotalLength],
											pstSmsParam->ucCode,
											pstSmsParam->ucLen ,j);	
	
	//8) Э���ʶ(TP-PID) һ��Ϊ0x00(GSM��ͨ���ͣ��㵽�㷽ʽ)
	//	�û���Ϣ���뷽ʽ(TP-DCS) ��0x04��0x15 ��Ϊ8-bit����
	//	��Ч��(TP-VP)Ϊ5����
	//sprintf( (char*)(&stSMSBufer.ucData[usTotalLength]), "%6x", 0x00f5ff );
	//usTotalLength += 6;
	ByteToHexAssiic(&stSMSBufer.ucData[usTotalLength], 0x00);
	usTotalLength += 2;
	ByteToHexAssiic(&stSMSBufer.ucData[usTotalLength], 0x04);
	usTotalLength += 2;



	ByteToHexAssiic(&stSMSBufer.ucData[usTotalLength], 0xff);
	usTotalLength += 2;
	
	//9) ���ݳ���
	if(pstDataContent->ucDataLength>140)
		ucLength = 140;
	else
		ucLength = (unsigned char)pstDataContent->ucDataLength;

	ByteToHexAssiic(&stSMSBufer.ucData[usTotalLength], (unsigned char)ucLength);
	usTotalLength += 2;
	
	//10)  8-bit���뷽ʽ
	for(i=0;i<ucLength;i++)
	{
		ByteToHexAssiic(&stSMSBufer.ucData[usTotalLength], pstDataContent->pData[i]);
		usTotalLength += 2;
	}
	

	stSMSBufer.ucData[usTotalLength++] = 0x1a;
	
       stSMSBufer.usTPALen = usTotalLength-stSMSBufer.usTPALen -1 ;	//TPA���볤��
       stSMSBufer.usTPALen=stSMSBufer.usTPALen/2 ;
//       stSMSBufer.usTPALen = 15 +  ucLength;	//TPA���볤��
	
	stSMSBufer.usLength = usTotalLength;
	stSMSBufer.ucIsValid = 1;
	
	return usTotalLength;
}

unsigned char AT_SMS_Decode(unsigned char* pSrc, unsigned char* pDst, PST_SMS_PARAM pSmsCode)
{
	unsigned char ucTmp = 0;
	unsigned char i;
	unsigned char ucAnd=0;
	unsigned int  uSrcPtr = 0;
	
	ST_SMS_PARAM  stSmsRevCode ,stSmsSCACode;

	uSrcPtr = 0;

	AssiicHexToByte(&(pSrc[uSrcPtr]), &ucTmp);
	if(ucTmp<2 || ucTmp>9)
		return 0;
	ucTmp = (ucTmp-1)<<1;	

	uSrcPtr += 4;		//����0891

	stSmsSCACode.ucLen = SMS_Decode_PhoneNumber(stSmsSCACode.ucCode, &(pSrc[uSrcPtr]), ucTmp,ucAnd);
	uSrcPtr += ucTmp;

	AssiicHexToByte(&(pSrc[uSrcPtr]), &ucTmp);	//24
	uSrcPtr += 2;

	// ȡ�ظ�����
	AssiicHexToByte(&(pSrc[uSrcPtr]), &ucTmp);//09
	if(ucTmp&0x01) 
		ucTmp++;		//����F�ĳ���
	
	uSrcPtr += 4;		//����0d91

	 if(pSrc[uSrcPtr]=='6' && pSrc[uSrcPtr+1]=='8')
			ucAnd=0;
	else
	        ucAnd=2;
	stSmsRevCode.ucLen = SMS_Decode_PhoneNumber(stSmsRevCode.ucCode, &(pSrc[uSrcPtr]), ucTmp,ucAnd);
	uSrcPtr += ucTmp;

	// TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
	// ȡЭ���ʶ(TP-PID)
	uSrcPtr += 2;
	// ȡ���뷽ʽ(TP-DCS)
	uSrcPtr += 2;
	// ����ʱ����ַ���(TP_SCTS) 
	uSrcPtr += 14;

	// �û���Ϣ����(TP-UDL)
	AssiicHexToByte(&(pSrc[uSrcPtr]), &ucTmp);
	uSrcPtr += 2;
	if(ucTmp>140)
		ucTmp = 0;
	
	for(i = 0 ; i < ucTmp ; i++)
	{
		AssiicHexToByte(&(pSrc[uSrcPtr]), &pDst[i]);
		uSrcPtr += 2;
	}
	*pSmsCode = stSmsRevCode;

	return ucTmp;
}	

unsigned char SMS_Encode_PhoneNumber(unsigned char* pDst , unsigned char* pSrc , unsigned char ucLen,unsigned char Jump)
{
	unsigned char i = 0;
	if(ucLen>16)		//���ȴ���
	{
		DebugPrint((unsigned char*)sms_Enumber_too_long, sizeof(sms_Enumber_too_long));
		return 16;
	}
     if(Jump==0)    //û��86
	{
	/*�洢�ĵ绰����ΪASSIIC����,����ֻҪ����˳��F����*/
	for(i=0;i<ucLen;i+=2)
	{
		pDst[i] = pSrc[i+1];
		pDst[i+1] = pSrc[i];
	}
     	}
     else  if(Jump==2)   //��86
	{
	      for(i=0;i<ucLen-2;i+=2)
	       {
		   pDst[i] = pSrc[i+3];
		    pDst[i+1] = pSrc[i+2];
	       }
     	}

	if(ucLen & 0x01)
	{
		pDst[i-2]='F';
		ucLen++;
	}
	Jump =ucLen-Jump;
	return Jump;
}

unsigned char SMS_Decode_PhoneNumber(unsigned char* pDst , unsigned char* pSrc , unsigned char ucLen,unsigned char ucAnd)
{
	unsigned char i = 0;
	unsigned char ucRetLen = 0;

	ucRetLen = ucLen;
	
	if(ucRetLen>16)		//���ȴ���
	{
		DebugPrint((unsigned char*)sms_Dnumber_too_long, sizeof(sms_Dnumber_too_long));
		return 16;
	}
	
    if(ucAnd==0)
	{
	for( i = 0; i < ucLen; i += 2 )	/*�洢�ĵ绰����ΪASSIIC����*/
	{
		pDst[i] = pSrc[i+1];
		pDst[i+1] = pSrc[i];
	}
	if( pDst[ucLen-1] == 'F' || pDst[ucLen-1] == 'f')
	{
		ucRetLen--;
	}
}

   else  if(ucAnd==2)
{
		pDst[0] = '8';
		pDst[1] = '6';	
	  for( i = 2; i < ucLen+2; i += 2 )	/*�洢�ĵ绰����ΪASSIIC����*/
	{
		pDst[i] = pSrc[i-1];
		pDst[i+1] = pSrc[i-2];
	}
	ucRetLen += 2;
	ucLen = ucLen + 2;
	  if( pDst[ucLen-1] == 'F' || pDst[ucLen-1] == 'f')
	{
		ucRetLen--;
	  }
    	}	
	return ucRetLen;
	}

void ByteToHexAssiic(unsigned char* pDstStr , unsigned char ucSrcData)
	{
		pDstStr[0] = AssiicTable[(ucSrcData>>4)&0x0f];
		pDstStr[1] = AssiicTable[ucSrcData&0x0f];
}

//void ByteToBCDAssiic(unsigned char* pDstStr , unsigned char ucSrcData)
//{
//	unsigned char i=0 , k=0;
//	i=((ucSrcData>>4)&0x0f);
//	k=(ucSrcData&0x0f);
//	if(k>9)
//	{
//		k-=10;
//		i++;
//	}
//	pDstStr[0] = AssiicTable[i];
//	pDstStr[1] = AssiicTable[k];
//}

unsigned char AssiicHexToByte(unsigned char* pSrcStr , unsigned char* pucDstData)
{
	unsigned char ucRet = 1;
	unsigned char ch = 0;
	
	ch = pSrcStr[0];

	if(ch>='0' && ch <='9')
		*pucDstData = (ch-'0');
	else if(ch>='a' && ch<='f')
		*pucDstData = (ch-'a'+10);
	else if(ch>='A' && ch<='F' )
		*pucDstData = (ch-'A'+10);
	else{
			*pucDstData = 0;
			ucRet = 0;
	}
	*pucDstData <<= 4;
	ch = pSrcStr[1];

	if(ch>='0' && ch<='9')
		*pucDstData |= (ch-'0');
	else if(ch>='a' && ch<='f' )
		*pucDstData |= (ch-'a'+10);
	else if(ch>='A' && ch<='F' )
		*pucDstData |= (ch-'A'+10);
	else{
		*pucDstData = 0;
		ucRet = 0;
	}
	return ucRet;
}

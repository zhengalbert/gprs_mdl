/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : public.c
 Description    : 
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2011-09-23      azh           the original version
******************************************************************************/
#include "..\inc\global_config.h"
#include 	"absacc.h"


//void Delayms(int t)
//{
//	unsigned short ms=0x700;
//	unsigned short i,j;
//	for(j=0;j<t;++j)
//		for(i=0;i<ms;++i);
//}

unsigned long BCD_TO_HEX_4(unsigned char *pucBCD)
{
	unsigned char tmp;
	unsigned long lRetHex=0;
	
	tmp =pucBCD[3];
	tmp &=0xF0;
	tmp >>=4;
	lRetHex=tmp*10000000;
	
	tmp=pucBCD[3];
	tmp &=0x0F;
	lRetHex +=tmp*1000000;
	
	tmp =pucBCD[2];
	tmp &=0xF0;
	tmp >>=4;
	lRetHex +=tmp*100000;
	
	tmp=pucBCD[2];
	tmp &=0x0F;
	lRetHex +=tmp*10000;
	
	tmp =pucBCD[1];
	tmp &=0xF0;
	tmp >>=4;
	lRetHex +=tmp*1000;
	
	tmp=pucBCD[1];
	tmp &=0x0F;
	lRetHex +=tmp*100;
	
	tmp =pucBCD[0];
	tmp &=0xF0;
	tmp >>=4;
	lRetHex +=tmp*10;
	
	tmp=pucBCD[0];
	tmp &=0x0F;
	lRetHex +=tmp;
	
	return lRetHex;
}
unsigned long BCD_TO_HEX_2(unsigned char *pucBCD)
{
	unsigned char tmp;
	unsigned long	lRetHex=0;
	
	tmp =pucBCD[1];
	tmp &=0xF0;
	tmp >>=4;
	lRetHex +=tmp*1000;
	
	tmp=pucBCD[1];
	tmp &=0x0F;
	lRetHex +=tmp*100;
	
	tmp =pucBCD[0];
	tmp &=0xF0;
	tmp >>=4;
	lRetHex +=tmp*10;
	
	tmp=pucBCD[0];
	tmp &=0x0F;
	lRetHex +=tmp;
	
	return lRetHex;
}
unsigned long BCD_TO_HEX_3(unsigned char *pucBCD)
{
	unsigned char tmp;
	unsigned long	lRetHex=0;
	
	tmp =pucBCD[2];
	tmp &=0xF0;
	tmp >>=4;
	lRetHex +=tmp*100000;
	
	tmp=pucBCD[2];
	tmp &=0x0F;
	lRetHex +=tmp*10000;
	
	tmp =pucBCD[1];
	tmp &=0xF0;
	tmp >>=4;
	lRetHex +=tmp*1000;
	
	tmp=pucBCD[1];
	tmp &=0x0F;
	lRetHex +=tmp*100;
	
	tmp =pucBCD[0];
	tmp &=0xF0;
	tmp >>=4;
	lRetHex +=tmp*10;
	
	tmp=pucBCD[0];
	tmp &=0x0F;
	lRetHex +=tmp;
	
	return lRetHex;
}
unsigned long BCD_TO_HEX_1(unsigned char *pucBCD)
{
	unsigned char tmp;
	unsigned long	lRetHex=0;
	
	tmp =pucBCD[0];
	tmp &=0xF0;
	tmp >>=4;
	lRetHex +=tmp*10;
	
	tmp=pucBCD[0];
	tmp &=0x0F;
	lRetHex +=tmp;
	
	return lRetHex;
}

unsigned long	HEX_TO_BCD(unsigned long	lHex,unsigned char ucSize)
{
	unsigned long	lRetBCD=0;
	if(ucSize==1)
	{
		if(lHex>99)
			lHex=99;
		lRetBCD=lHex%10+(lHex%100)/10*0x10;
	}
	if(ucSize==2)
	{
		if(lHex>9999)
			lHex=9999;
		lRetBCD=lHex%10+(lHex%100)/10*0x10;
		lHex=lHex/100;
		lRetBCD=(lHex%10+(lHex%100)/10*0x10)*0x100+lRetBCD;
	}
	if(ucSize==3)
	{
		if(lHex>999999)
			lHex=999999;
		lRetBCD=lHex%10+(lHex%100)/10*0x10;
		lHex=lHex/100;
		lRetBCD=(lHex%10+(lHex%100)/10*0x10)*0x100+lRetBCD;
		lHex=lHex/100;
		lRetBCD=(lHex%10+lHex/10*0x10)*0x10000+lRetBCD;
	}
	if(ucSize==4)
	{
		if(lHex>99999999)
			lHex=99999999;
		lRetBCD=lHex%10+(lHex%100)/10*0x10;
		lHex=lHex/100;
		lRetBCD=(lHex%10+(lHex%100)/10*0x10)*0x100+lRetBCD;
		lHex=lHex/100;
		lRetBCD=(lHex%10+(lHex%100)/10*0x10)*0x10000+lRetBCD;
		lHex=lHex/100;
		lRetBCD=(lHex%10+(lHex%100)/10*0x10)*0x1000000+lRetBCD;
	}
	return lRetBCD;
}

//unsigned char	HEXCHAR_TO_BCDSTR(unsigned char ucHex,unsigned char *pstr)
//{
//	unsigned char m=0;
//	pstr[m++]=ucHex/100+'0';
//	ucHex=ucHex%100;
//	pstr[m++]=ucHex/10+'0';
//	ucHex=ucHex%10;
//	pstr[m++]=ucHex+'0';
//
//	return m;
//}
//
//unsigned char	HEXSHORT_TO_BCDSTR(unsigned short usHex,unsigned char *pstr)
//{
//	unsigned char m=0;
//	pstr[m++]=usHex/10000+'0';
//	usHex=usHex%10000;
//	pstr[m++]=usHex/1000+'0';
//	usHex=usHex%1000;
//	pstr[m++]=usHex/100+'0';
//	usHex=usHex%100;
//	pstr[m++]=usHex/10+'0';
//	usHex=usHex%10;
//	pstr[m++]=usHex+'0';
//
//	return m;
//}

unsigned long	BYTE_TO_LONG(unsigned char *pucByte)
{
	unsigned char i=0;
	unsigned long ulVal=0;
	for(i=0;i<4;i++)
		((unsigned char *)&ulVal)[i]=pucByte[i];
	return ulVal;
}

STDATETIME	BYTE_TO_DATETIME(unsigned char *pucByte)
{
	STDATETIME	stDateTime;
	stDateTime.ucYear	= pucByte[0];
	stDateTime.ucMonth	= pucByte[1];
	stDateTime.ucDay	= pucByte[2];
	stDateTime.ucHour	= pucByte[3];
	stDateTime.ucMinute = pucByte[4];
	stDateTime.ucSecond = pucByte[5];
	return stDateTime;
}

unsigned char Check_BCD_Data(unsigned char ucData)
{
	unsigned ucRet=RET_ERROR;
	if( (ucData&0xF0)<0xA0 && (ucData&0x0F)<0x0A )
		ucRet=RET_SUCCESS;
	return ucRet;
}
unsigned char Check_BCD_Data_Buffer(unsigned char *pucData,unsigned char ucLen)
{
//	unsigned ucRet=RET_ERROR;
	unsigned char i;
	for(i=0;i<ucLen;i++)
	{
		if(Check_BCD_Data(pucData[i]) == RET_ERROR)
			return RET_ERROR;
	}
	return RET_SUCCESS;
}
//unsigned char Check_BCD_MuiltData(unsigned char *pucData,unsigned char ucDataNum)
//{
//	unsigned char ucRet=RET_ERROR;
//	unsigned char i=0;
//	for( i=0;i<ucDataNum;i++ )
//	{
//		if( Check_BCD_Data( pucData[i] )==RET_ERROR )
//			break;
//	}
//	if( i>=ucDataNum )
//		ucRet=RET_SUCCESS;
//
//	return ucRet;
//}
//unsigned char CheckDateTime(STDATETIME *pstDateTime)
//{
//	unsigned char ucRet=RET_ERROR;
//	unsigned int nYear=0;
//
//	if(pstDateTime!=NULL)
//	{
//		if( (pstDateTime->ucHour)<24 && (pstDateTime->ucMinute)<60 && (pstDateTime->ucSecond)<60 )
//		{
//			if( (pstDateTime->ucYear)>=02 && (pstDateTime->ucYear<40) )
//			{
//				switch( pstDateTime->ucMonth )
//				{
//				case 1:
//					if( pstDateTime->ucDay<=31 && pstDateTime->ucDay!=0 )
//						ucRet=RET_SUCCESS;
//					break;
//				case 2:
//					nYear=2000+pstDateTime->ucYear;
//					if( nYear%4==0 && nYear%100!=0 )
//					{
//						if( pstDateTime->ucDay<=29 && pstDateTime->ucDay!=0 )
//							ucRet=RET_SUCCESS;
//					}
//					else
//					{
//						if( pstDateTime->ucDay<=28 && pstDateTime->ucDay!=0 )
//							ucRet=RET_SUCCESS;
//					}
//					break;
//				case 3:
//					if( pstDateTime->ucDay<=31 && pstDateTime->ucDay!=0 )
//						ucRet=RET_SUCCESS;
//					break;
//				case 4:
//					if( pstDateTime->ucDay<=30 && pstDateTime->ucDay!=0 )
//						ucRet=RET_SUCCESS;
//					break;
//				case 5:
//					if( pstDateTime->ucDay<=31 && pstDateTime->ucDay!=0 )
//						ucRet=RET_SUCCESS;
//					break;
//				case 6:
//					if( pstDateTime->ucDay<=30 && pstDateTime->ucDay!=0 )
//						ucRet=RET_SUCCESS;
//					break;
//				case 7:
//					if( pstDateTime->ucDay<=31 && pstDateTime->ucDay!=0 )
//						ucRet=RET_SUCCESS;
//					break;
//				case 8:
//					if( pstDateTime->ucDay<=31 && pstDateTime->ucDay!=0 )
//						ucRet=RET_SUCCESS;
//					break;
//				case 9:
//					if( pstDateTime->ucDay<=30 && pstDateTime->ucDay!=0 )
//						ucRet=RET_SUCCESS;
//					break;
//				case 0x0A:
//					if( pstDateTime->ucDay<=31 && pstDateTime->ucDay!=0 )
//						ucRet=RET_SUCCESS;
//					break;
//				case 0x0B:
//					if( pstDateTime->ucDay<=30 && pstDateTime->ucDay!=0 )
//						ucRet=RET_SUCCESS;
//					break;
//				case 0x0C:
//					if( pstDateTime->ucDay<=31 && pstDateTime->ucDay!=0 )
//						ucRet=RET_SUCCESS;
//					break;
//				default:break;
//				}
//			}
//		}
//	}
//	return ucRet;
//}
//
unsigned char ClearDateTime(STDATETIME *pstDateTime)
{
	unsigned char ucRet=RET_SUCCESS;
	if(pstDateTime!=NULL)
	{
		pstDateTime->ucYear=(unsigned char)CONINVALIDVAL;
		pstDateTime->ucMonth=(unsigned char)CONINVALIDVAL;
		pstDateTime->ucDay=(unsigned char)CONINVALIDVAL;
		pstDateTime->ucHour=(unsigned char)CONINVALIDVAL;
		pstDateTime->ucMinute=(unsigned char)CONINVALIDVAL;
		pstDateTime->ucSecond=(unsigned char)CONINVALIDVAL;
	}
	return ucRet;
}

unsigned char Get_Sum(unsigned char *pBuffer,unsigned long ulLen)
{
	unsigned long i=0;
	unsigned char ucChkSum=0;
	for(i=0;i<ulLen;i++)
		ucChkSum+=pBuffer[i];
	return ucChkSum;
}

unsigned long makelabs(long lVal)
{
	if(lVal < 0)
		return -1*lVal;
	else
		return lVal;
}

//unsigned char Is_In_Buffer(unsigned char *pBuffer,unsigned char ucLen,unsigned char ucVal)
//{
//	unsigned char ucRet=0xFF,i=0;
//	for(i=0;i<ucLen;i++)
//		if(pBuffer[i]==ucVal)
//			break;
//	if(i>=ucLen)
//		ucRet=0xFF;
//	else
//		ucRet=i;
//	return ucRet;
//}
//
//unsigned long Get_Correct_Boadrate(unsigned long ulBoadrate)
//{
//	if(ulBoadrate != 300 
//		&& ulBoadrate != 600 
//		&& ulBoadrate != 1200 
//		&& ulBoadrate != 2400 
//		&& ulBoadrate != 4800 
//		&& ulBoadrate != 9600 
//		&& ulBoadrate != 19200 
//		&& ulBoadrate != 28800 
//		&& ulBoadrate != 38400 
//		&& ulBoadrate != 57600 
//		&& ulBoadrate != 115200 )
//		return 1200;
//	return ulBoadrate;
//}




#define SAVE_PARA			0
#define SAVE_PARADATA		1


unsigned char BitChk( unsigned short  BitNum, unsigned char *pDate )
{
	unsigned short  i;
    unsigned short  j;
	i=BitNum/8;
	j=BitNum%8;

	if(pDate[i]&(1<<j))
        return 1;
    else
        return 0;
}

unsigned char ChkBitFlag(unsigned char *puc, unsigned short  nbit)
{
	if(BitChk(nbit, puc ) == 1 )
		return 1;   //有标志
	else
		return 0;
}

void BitFlagSet1( unsigned short BitNum, unsigned char *pDate )
{
    const unsigned char conaBit[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
	pDate[BitNum/8] |= conaBit[BitNum%8];
}

void SetBitFlag(unsigned char *puc, unsigned short n)
{
	BitFlagSet1(n, puc);
}
/*
功能：判断是否有效。如有效转换成BCD码输出；无效则输出相同字节的0xFF
*/
//unsigned char ConvertHisVal0(unsigned long ulVal,unsigned char *pucData,unsigned char ucLen)
//{
//	unsigned char ucRet=RET_ERROR;
//	unsigned char i=0,m=0;
//	unsigned long ulBCDVal=0;
//
//	if( pucData!=NULL && ucLen>0 && ucLen<5 )
//	{
//		ulBCDVal=HEX_TO_BCD( ulVal,ucLen );
//		for( i=0;i<ucLen;i++ )
//		{
//			pucData[m++]=(unsigned char)(ulBCDVal%0x100);
//			ulBCDVal=ulBCDVal/0x100;
//		}
//		ucRet=RET_SUCCESS;
//	}
//	return ucRet;
//}
//
//unsigned long GetPQRate_Var4B(unsigned long lPower,unsigned long lVar)	//得到功率因数值，格式xxx.xx
//{//无功4位小数
//	unsigned long lVal=100;
//	double dVal=0;
//	dVal=sqrt( (lPower*lPower+lVar*lVar) );
//	if( dVal!=0 )
//	{
//		lVal=(unsigned long)(lPower*100/dVal);
//		if( lVal>100 )
//			lVal=100;
//	}
//	return lVal;
//}
//
//unsigned long GetPQRate_Var4B_3P(unsigned long lPower,unsigned long lVar)	//得到功率因数值，格式x.xxx
//{//无功4位小数
//	unsigned long lVal=1000;
//	double dVal=0;
//	////////////数据过大，防止溢出
//	while( (lPower > lVar ? lPower : lVar) >  46000 )	//46000*46000*2差32位不多快溢出了
//	{
//		lPower /= 10;
//		lVar  /= 10;
//	}
//	////////////
//	dVal=sqrt( ((lPower*lPower+lVar*lVar)) );
//	if( dVal!=0 )
//	{
//		lVal=(unsigned long)(lPower*10000/dVal);
//		lVal = (lVal + 5)/10;
//		if( lVal>1000 )
//			lVal=1000;
//	}
//	return lVal;
//}

unsigned long	Adjust_Zero(unsigned long ulVal)
{
	if(ulVal == 0)
		ulVal=1;
	return ulVal;
}

unsigned long GetSPow(unsigned long lPower,unsigned long lVar)	//得到视在功率
{
	unsigned long lVal=100;
	double dVal=0;
	lPower=lPower/100;		
	dVal=sqrt( (lPower*lPower+lVar*lVar) );
	lVal = (unsigned long)(dVal *100);
	return lVal;
}
unsigned long GetPQRate(unsigned long lPower,unsigned long lVar)	//得到功率因数值，格式xxx.xx
{
	unsigned long lVal=100;
	double dVal=0;
	lPower=lPower/100;		
	dVal=sqrt( (lPower*lPower+lVar*lVar) );
	if( dVal!=0 )
	{
		lVal=(unsigned long)(lPower*100/dVal+0.5);
		if( lVal>100 )
			lVal=100;
	}
	return lVal;
}
unsigned char FillData_EE(unsigned char *pucData,unsigned char ucLen)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	if(pucData!=NULL)
	{
		pucData[0]=(unsigned char)CONINVALIDVAL;
		for(i=1;i<ucLen;i++)
			pucData[i]=0;
	}
	return ucRet;
}
unsigned char	Is_Data_AllEE(unsigned char *pucData,unsigned char ucLen)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	if(pucData!=NULL)
	{
		for(i=0;i<ucLen;i++)
		{
			if(pucData[i]!=(unsigned char)CONINVALIDVAL)//azh 140918
			{
				ucRet=RET_ERROR;
				break;
			}
		}
	}else
		ucRet=RET_ERROR;

	return ucRet;
}
unsigned char	Is_Data_EE(unsigned char *pucData,unsigned char ucLen)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	if(pucData!=NULL)
	{
		if(pucData[0]!=(unsigned char)CONINVALIDVAL)//azh 140918
			ucRet=RET_ERROR;
		else
		{
			for(i=1;i<ucLen;i++)
			{
				if(pucData[i]!=0x00)
				{
					ucRet=RET_ERROR;
					break;
				}
			}
		}
	}else
		ucRet=RET_ERROR;
	return ucRet;
}
unsigned char FillData_AllEE(unsigned char *pucData,unsigned char ucLen)
{
	unsigned char ucRet=RET_SUCCESS,i=0;
	if(pucData!=NULL)
	{
		for(i=0;i<ucLen;i++)
			pucData[i]=(unsigned char)CONINVALIDVAL;
	}	

	return ucRet;
}

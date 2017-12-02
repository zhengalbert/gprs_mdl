#ifndef _MAIN_DATA_H_
#define _MAIN_DATA_H_

#define		CON_DT_SG				0x10	//符号位	//0正 1负
#define		CON_DT1_10_E4			0x00	//10^4
#define		CON_DT1_10_E3			0x20	//10^3
#define		CON_DT1_10_E2			0x40	//100
#define		CON_DT1_10_E1			0x60	//10
#define		CON_DT1_10_E0			0x80	//1
#define		CON_DT1_10_EN1			0xA0	//0.1
#define		CON_DT1_10_EN2			0xC0	//10^-2
#define		CON_DT1_10_EN3			0xE0	//10^-3

#define		CON_SUMGFLAG_ENERGYCTRL	0x55	//电量控制
#define		CON_SUMGFLAG_POWERCTRL	0xAA	//功率控制
#define		CON_SUMGFLAG_CTRL_COMB	0xFF	//电量控制和功率控制

typedef	struct	_STDATATYPE_1
{
	unsigned char ucByte1;			//BYTE 1	BCD码十位	BCD码个位
	unsigned char ucByte2;			//BYTE 2	G3	G2	G1	SG	BCD码百位
}STDATATYPE_1;

#define	CON_DT5_KWH_CENT		0x00	//G=0，表示单位为KWH、厘；
#define	CON_DT5_MWH_YUAN		0x40	//G=1，表示单位为MWH、元。

typedef	struct	_STDATATYPE_5
{
	unsigned char ucByte1;			//BYTE 1	BCD码十位	BCD码个位
	unsigned char ucByte2;			//BYTE 2	BCD码千位	BCD码百位
	unsigned char ucByte3;			//BYTE 3	BCD码十万位	BCD码万位
	unsigned char ucByte4;			//BYTE 4	0 G	0 SG	BCD码百万位
}STDATATYPE_5;

typedef	struct	_STDATATYPE_6
{
	unsigned char ucByte1;			//BYTE 1	BCD码十位	BCD码个位
	unsigned char ucByte2;			//BYTE 2	BCD码千位	BCD码百位
}STDATATYPE_6;

typedef	struct	_STDATATYPE_7
{
	unsigned char ucByte1;			//BYTE 1	BCD码十位	BCD码个位
}STDATATYPE_7;

typedef	struct	_STDATATYPE_8
{
	unsigned char ucByte1;			//BYTE 1	BCD码十位	BCD码个位
}STDATATYPE_8;

typedef	struct	_STDATATYPE_9
{
	unsigned char ucByte1;			//BYTE 1	BCD码十位	BCD码个位
}STDATATYPE_9;

typedef	struct	_STDATATYPE_10
{
	unsigned char ucByte1;			//BYTE 1	BCD码十位	BCD码个位
}STDATATYPE_10;

typedef	struct	_STDATATYPE_11
{
	unsigned char ucByte1;			//BYTE 1	BCD码十分位	BCD码百分位
	unsigned char ucByte2;			//BYTE 2	BCD码十位	BCD码个位
}STDATATYPE_11;

typedef	struct	_STDATATYPE_12
{
	unsigned char ucByte1;			//BYTE 1	BCD码十位	BCD码个位
}STDATATYPE_12;

typedef	struct	_STDATATYPE_13
{
	unsigned char ucByte1;			//BYTE 1	BCD码十位	BCD码个位
}STDATATYPE_13;

typedef	struct	_STDATATYPE_14
{
	unsigned char ucByte1;			//BYTE 1	BCD码十位	BCD码个位
}STDATATYPE_14;

typedef	struct	_STDATATYPE_15
{
	unsigned char ucByte1;			//BYTE 1	BCD码十位	BCD码个位
}STDATATYPE_15;

typedef	struct	_STDATATYPE_16
{
	unsigned char ucByte1;			//BYTE 1	BCD码十位	BCD码个位
}STDATATYPE_16;

__EXTERN unsigned char gucJob1HoursFlag;

__EXTERN unsigned char	Check_DataType_1(unsigned char *pucBuffer);
__EXTERN unsigned char	Check_DataType_4(unsigned char *pucBuffer);
__EXTERN unsigned char	Check_DataType_5(unsigned char *pucBuffer);
__EXTERN unsigned char	Check_DataType_11(unsigned char *pucBuffer);

//__EXTERN unsigned long	Get_DataType1_Val_Point(unsigned long ulHex,unsigned char ucDataTypeSize);
//__EXTERN unsigned long	Get_DataType1_Val_Int(unsigned long ulHex,unsigned char ucDataTypeSize);

__EXTERN unsigned long GetDiffVal(unsigned long ulVal1,unsigned long ulVal2);

__EXTERN unsigned char Main_Get_ReqData_2_F161(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F162(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F163(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F164(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F165(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F166(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F167(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F168(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F169(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
 
__EXTERN unsigned char Main_Get_ReqData_2_F170(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F171(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F172(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F173(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F174(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F175(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F176(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F177(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F178(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F179(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
 
__EXTERN unsigned char Main_Get_ReqData_2_F180(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F181(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F182(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F183(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F184(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F185(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F186(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F187(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F188(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F189(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
 
__EXTERN unsigned char Main_Get_ReqData_2_F190(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F191(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F192(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F193(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F194(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F195(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);
__EXTERN unsigned char Main_Get_ReqData_2_F196(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstDataTime);

__EXTERN unsigned char Main_Get_ReqData_2_F145(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F146(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F147(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
__EXTERN unsigned char Main_Get_ReqData_2_F148(unsigned char ucDAp,unsigned char *pucRetBuffer,unsigned short *pusPos,STDATETIME *pstStartTime,STDATETIME *pstEndTime,unsigned char ucDataInter);
#endif	


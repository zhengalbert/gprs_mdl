/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_Response.h
 Description    :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/
#ifndef _MAIN_RESPONSE_H_
#define _MAIN_RESPONSE_H_

#define CON_ANSWER_YES  0x55
#define CON_ANSWER_NO   0xAA

#define CON_FRAME_CMD_DIR_UP    0x80            //DIR=0 表示此帧报文是由主站发出，下行报文。DIR=1   表示此帧报文是由终端发出，上行报文。
#define CON_FRAME_CMD_PRM_ORG   0x40            //PRM=0 表示此帧报文来自响应站（通信响应站）PRM=1   表示此帧报文来自原发站（通信启动站）
#define CON_FRAME_CMD_ACD_IMP   0x20            //ACD=1 表示终端有重要事件等待访问，应用层数据区附加事件计数器EC，提示主站事件发生的累积次数。ACD=0表示终端无事件数据等待访问
#define CON_FRAME_CMD_FCB       0x20            //FCB表示每个站连续的发送/确认或者请求/响应服务的变化位
#define CON_FRAME_CMD_FCV_VLD   0x10            //FCV=1：表示FCB位有效；FCV=0：表示FCB位无效。

#define CON_FRAME_SEQ_TPV   0x80            //1表示在附加信息域中带有时间标签Tp
#define CON_FRAME_SEQ_FIR   0x40            //1表示本报文分段是整个应用报文的第一个分段
#define CON_FRAME_SEQ_FIN   0x20            //1表示本报文分段是整个应用报文的最后一个分段
#define CON_FRAME_SEQ_CON   0x10            //1表示发送的应用报文期待从接收的应用报文中给予确认

#define CON_SEND_BLOCK_MAX_SIZE 560//512 //azh 170919
//#define CON_SEND_BLOCK_BUFFER_SIZE 132
#define CON_SEND_BLOCK_BUFFER_SIZE 255
#define CON_EXPLAIN_RET_DATA_BLOCK_NUM 18//1536=3*512 azh //18
//azh
typedef struct _STEXPLAINRETDATABLOCK
{
    unsigned short usLen;
    unsigned char *ucBuff;
}STEXPLAINRETDATABLOCK;

typedef struct _STEXPLAINRETDATAEXTBLOCK
{
    unsigned short usLen;
    unsigned char ucBuff[32];
}STEXPLAINRETDATAEXTBLOCK;

typedef struct _STEXPLAINRETDATA        //返回结果存放
{
    unsigned char ucExplainRetDataNum;
    STEXPLAINRETDATAEXTBLOCK stExplainExtData;
    STEXPLAINRETDATABLOCK stExplainRetData[CON_EXPLAIN_RET_DATA_BLOCK_NUM];
}STEXPLAINRETDATA;

//1层
__EXTERN unsigned char Main_Recv_Manage(STTERMSG    *pstRecvTerMsg,unsigned char ucDataItemNo,unsigned char ucMSA,unsigned char ucSEQ);
//2层
__EXTERN unsigned char Main_Recv_ACK(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo,unsigned char ucSEQ);
__EXTERN unsigned char Main_Recv_Reset(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo);
__EXTERN unsigned char Main_Recv_SetPara(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo);
__EXTERN unsigned char Main_Recv_Control(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo);
__EXTERN unsigned  char Main_Recv_DataReq(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo);
__EXTERN unsigned char Main_Recv_ReqTaskData(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo);
__EXTERN unsigned char Main_Recv_GetPara(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo);
__EXTERN unsigned char Main_Recv_ReqData_1(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo);
__EXTERN unsigned char Main_Recv_ReqData_2(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo);
__EXTERN unsigned char Main_Recv_ReqData_3(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo);
__EXTERN unsigned char Main_Recv_ExtCmd(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo);
__EXTERN unsigned char Main_Recv_RelayCmd(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo);
__EXTERN unsigned char Main_Recv_FileTrans(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo);
__EXTERN unsigned char Main_Recv_DataTrans(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo);

//3层
//================  Ack ======================
__EXTERN unsigned char  Main_Recv_ACK_F1(unsigned char *pucBuffer,unsigned char ucDAp,unsigned char ucSEQ);
__EXTERN unsigned char  Main_Recv_ACK_F2(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char  Main_Recv_ACK_F3(unsigned char *pucBuffer,unsigned char ucDAp,unsigned char ucSEQ);
//================  Reset ======================
__EXTERN unsigned char Main_Recv_Reset_Hardware(unsigned char ucMsgSource);
__EXTERN unsigned char Main_Recv_Reset_Data(unsigned char ucMsgSource);
__EXTERN unsigned char Main_Recv_Reset_All(unsigned char ucMsgSource,unsigned char ucClrFlag);
//================  Set Para ======================
__EXTERN unsigned char Main_Recv_Set_F1_CommPara(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F2_RelayPara(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F3_IP_Addr(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F4_Station_Number(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F5_TerEncrypt(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F6_TerGroupAddr(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F7_IPAddr(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F9_AlarmControlWord(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F9_ConfigNum(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F10_MeterPara(unsigned char *pucBuffer,unsigned char ucDAp);
//__EXTERN unsigned char Main_Recv_Set_F11_PulsePara(unsigned char *pucBuffer,unsigned char ucDAp);
//__EXTERN unsigned char Main_Recv_Set_F12_DInputPara(unsigned char *pucBuffer,unsigned char ucDAp);
//unsigned char Main_Recv_Set_F13_AnalogPara(unsigned char *pucBuffer,unsigned char ucDAp);
//unsigned char Main_Recv_Set_F14_SumGPara(unsigned char *pucBuffer,unsigned char ucDAp);
//unsigned char Main_Recv_Set_F15_PowValDiff(unsigned char *pucBuffer,unsigned char ucDAp);
//__EXTERN unsigned char Main_Recv_Set_F16_PDPInfo(unsigned char *pucBuffer,unsigned char ucDAp);
//unsigned char Main_Recv_Set_F17_KeepPower(unsigned char *pucBuffer,unsigned char ucDAp);
//unsigned char Main_Recv_Set_F18_PowCtrlTimePer(unsigned char *pucBuffer,unsigned char ucDAp);
//unsigned char Main_Recv_Set_F19_PowCtrlCoef(unsigned char *pucBuffer,unsigned char ucDAp);
//unsigned char Main_Recv_Set_F20_EnerCtrlCoef(unsigned char *pucBuffer,unsigned char ucDAp);
//__EXTERN unsigned char Main_Recv_Set_F21_PerTimeFee(unsigned char *pucBuffer,unsigned char ucDAp);
//__EXTERN unsigned char Main_Recv_Set_F22_FeePara(unsigned char *pucBuffer,unsigned char ucDAp);
//__EXTERN unsigned char Main_Recv_Set_F23_UrgeAlarmPara(unsigned char *pucBuffer,unsigned char ucDAp);
//unsigned char Main_Recv_Set_F24_CopyInter(unsigned char *pucBuffer,unsigned char ucDAp);

__EXTERN unsigned char Main_Recv_Set_F25_MPBasePara(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F26_MPLimit(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F27_MPCuFePara(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F28_MPPowRatePara(unsigned char *pucBuffer,unsigned char ucDAp);
//__EXTERN unsigned char Main_Recv_Set_F29_Ter_Meter_addr(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F30_CopyFlag(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F33_SumGFreezePara(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F34_PortProperty(unsigned char *pucBuffer,unsigned char ucDAp);
//__EXTERN unsigned char Main_Recv_Set_F35_EmphaFlag(unsigned char *pucBuffer,unsigned char ucDAp);
//__EXTERN unsigned char Main_Recv_Set_F37_LinkPara(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F38_Class(unsigned char *pucBuffer,unsigned char ucDAp,unsigned char ucJobNo);

//__EXTERN unsigned char Main_Recv_Set_TimePerSetting(unsigned char *pucBuffer,unsigned char *ucPos,unsigned char ucSumGroupNo,unsigned char ucCurveNo);
//unsigned char Main_Recv_Set_F41_TimePerSetting(unsigned char *pucBuffer,unsigned char ucDAp);
//unsigned char Main_Recv_Set_F42_RestCtrl(unsigned char *pucBuffer,unsigned char ucDAp);
//unsigned char Main_Recv_Set_F43_PowerSlip(unsigned char *pucBuffer,unsigned char ucDAp);
//unsigned char Main_Recv_Set_F44_SaleStop(unsigned char *pucBuffer,unsigned char ucDAp);
//unsigned char Main_Recv_Set_F45_PowerCtrlTrun(unsigned char *pucBuffer,unsigned char ucDAp);
//unsigned char Main_Recv_Set_F46_MonthEnerCtrl(unsigned char *pucBuffer,unsigned char ucDAp);
//unsigned char Main_Recv_Set_F47_BuyEnerCtrl(unsigned char *pucBuffer,unsigned char ucDAp);
//unsigned char Main_Recv_Set_F48_EnerCtrlTurn(unsigned char *pucBuffer,unsigned char ucDAp);
//unsigned char Main_Recv_Set_F49_CtrlAlrTime(unsigned char *pucBuffer,unsigned char ucDAp);

__EXTERN unsigned char Main_Recv_Set_F57_SoundAlarm(unsigned char *pucBuffer,unsigned char ucDAp);
//unsigned char Main_Recv_Set_F58_KeepAuto(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F59_MtErrorPara(unsigned char *pucBuffer,unsigned char ucDAp);
//__EXTERN unsigned char Main_Recv_Set_F60_HarmLimit(unsigned char *pucBuffer,unsigned char ucDAp);
//__EXTERN unsigned char Main_Recv_Set_F61_Aanalog(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F62_CommType(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F63_FluxLimit(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F65_UpData1Para(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F66_UpData2Para(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F67_UpData1Ctrl(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F68_UpData2Ctrl(unsigned char *pucBuffer,unsigned char ucDAp);
//__EXTERN unsigned char Main_Recv_Set_F81_DCRate(unsigned char *pucBuffer,unsigned char ucDAp);
//__EXTERN unsigned char Main_Recv_Set_F82_DCLimit(unsigned char *pucBuffer,unsigned char ucDAp);
//__EXTERN unsigned char Main_Recv_Set_F83_M(unsigned char *pucBuffer,unsigned char ucDAp);
//azh 170919
__EXTERN unsigned char Main_Recv_Set_F90_Wireless(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F91_Location(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Set_F150_Snstate(unsigned char *pucBuffer,unsigned char ucDAp);

__EXTERN unsigned char Main_Recv_Set_F207_Addr(unsigned char *pucBuffer,unsigned char ucDAp);


__EXTERN unsigned char Main_Recv_Set_F131_GRTask(unsigned char *pucBuffer,unsigned char ucDAp);

//================  Get Para ======================
__EXTERN unsigned char Main_Recv_Get_F1_CommPara(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F2_RelayPara(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F3_IP_Addr(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F4_Station_Number(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F5_TerEncrypt(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F6_TerGroupAddr(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F7_LocalIP(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F8_AlarmControlWord(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F9_ConfigNum(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F10_MeterPara(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F11_PulsePara(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F12_DInputPara(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F13_AnalogPara(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F14_SumGPara(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F15_PowValDiff(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F16_PDPInfo(STDATAITEM *pstDataItem,unsigned char ucDAp);
//__EXTERN unsigned char Main_Recv_Get_F17_KeepPower(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F18_PowCtrlTimePer(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F19_PowCtrlCoef(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F20_EnerCtrlCoef(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F21_PerTimeFee(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F22_FeePara(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F23_UrgeAlarmPara(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F24_CopyInter(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F25_MPBasePara(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F26_MPLimit(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F27_MPFreezePara(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F28_MPPowRatePara(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F29_Ter_Meter_addr(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F30_CopyFlag(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F33_SumGFreezePara(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F34_PortProp(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F35_EmphaFlag(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F37_LinkPara(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F38_JobPara(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned char ucJobNo);

//unsigned char Main_Recv_Get_F41_TimePerSetting(STDATAITEM *pstDataItem,unsigned char ucDAp);
//unsigned char Main_Recv_Get_F42_RestCtrl(STDATAITEM *pstDataItem,unsigned char ucDAp);
//unsigned char Main_Recv_Get_F43_PowerSlip(STDATAITEM *pstDataItem,unsigned char ucDAp);
//unsigned char Main_Recv_Get_F44_SaleStop(STDATAITEM *pstDataItem,unsigned char ucDAp);
//unsigned char Main_Recv_Get_F45_PowerCtrlTrun(STDATAITEM *pstDataItem,unsigned char ucDAp);
//unsigned char Main_Recv_Get_F46_MonthEnerCtrl(STDATAITEM *pstDataItem,unsigned char ucDAp);
//unsigned char Main_Recv_Get_F47_BuyEnerCtrl(STDATAITEM *pstDataItem,unsigned char ucDAp);
//unsigned char Main_Recv_Get_F48_EnerCtrlTurn(STDATAITEM *pstDataItem,unsigned char ucDAp);
//unsigned char Main_Recv_Get_F49_CtrlAlrTime(STDATAITEM *pstDataItem,unsigned char ucDAp);

__EXTERN unsigned char Main_Recv_Get_F57_SoundAlarm(STDATAITEM *pstDataItem,unsigned char ucDAp);
//__EXTERN unsigned char Main_Recv_Get_F58_KeepAuto(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F59_MtErrorPara(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F61_AnalogPara(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F62_CommType(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F63_FluxLimit(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F65_UpData1Para(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F66_UpData2Para(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F67_UpData1Ctrl(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F68_UpData2Ctrl(STDATAITEM *pstDataItem,unsigned char ucDAp);
//__EXTERN unsigned char Main_Recv_Get_F81_DCRate(STDATAITEM *pstDataItem,unsigned char ucDAp);
//__EXTERN unsigned char Main_Recv_Get_F82_DCLimit(STDATAITEM *pstDataItem,unsigned char ucDAp);
//__EXTERN unsigned char Main_Recv_Get_F83_M(STDATAITEM *pstDataItem,unsigned char ucDAp);

__EXTERN unsigned char Main_Recv_Get_F89_TerAddr(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F90_PhabreTimePara(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F91_AlrCtrlTime(STDATAITEM *pstDataItem,unsigned char ucDAp);

//azh 170919
__EXTERN unsigned char Main_Recv_Get_F90_Wireless(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F91_Location(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F150_Snstate(STDATAITEM *pstDataItem,unsigned char ucDAp);

//__EXTERN unsigned char Main_Recv_Get_F92_HarmValUpParam(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Get_F93_MESSAGE(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned char ucNum);

__EXTERN unsigned char Main_Recv_Get_F131_GRTask(STDATAITEM *pstDataItem,unsigned char ucDAp);


__EXTERN unsigned char Main_Recv_Config_Get_F1_Version(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Config_Get_F2_Interface(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Config_Get_F3_Other(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Config_Get_F4_Para(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Config_Get_F5_Ctrl(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Config_Get_F6_Data1(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Config_Get_F7_Data2(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Config_Get_F8_ERC(STDATAITEM *pstDataItem,unsigned char ucDAp);
//azh 170920
__EXTERN unsigned char Main_Recv_Config_Get_F9_REMOTEVER(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Config_Get_F10_LOCALVER(STDATAITEM *pstDataItem,unsigned char ucDAp);

__EXTERN unsigned char Main_Recv_Login_Set_F1_Password(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Login_Get_F2_Password(STDATAITEM *pstDataItem,unsigned char ucDAp);

//================  Control ======================
__EXTERN unsigned char Main_Recv_Ctrl_F1_YK_Open(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Ctrl_F2_YK_Close(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Ctrl_F9_TimePer_On(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Ctrl_F10_RestCtrl_On(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Ctrl_F11_SaleStop_On(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Ctrl_F12_PowerLower_On(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Ctrl_F15_MonthEner_On(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Ctrl_F16_BuyEner_On(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Ctrl_F17_TimePer_Off(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Ctrl_F18_RestCtrl_Off(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Ctrl_F19_SaleStop_Off(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Ctrl_F20_PowerLower_Off(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Ctrl_F23_MonthEner_Off(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Ctrl_F24_BuyEner_Off(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Ctrl_F25_KeepPower_On(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Ctrl_F26_UrgeAlarm_On(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Ctrl_F27_Talk_On(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Ctrl_F28_Remove_On(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Ctrl_F29_UpActive_On(unsigned char *pucBuffer,unsigned char ucDAp);
//azh 170920
__EXTERN unsigned char Main_Recv_Ctrl_F30_SetMeterTime_Switch(unsigned char *pucBuffer,unsigned char ucDAp);

__EXTERN unsigned char Main_Recv_Ctrl_F31_SetTerTime(unsigned char *pucBuffer,unsigned char ucDAp); //F31 对时命令
__EXTERN unsigned char Main_Recv_Ctrl_F32_Message(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Ctrl_F33_KeepPower_Off(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Ctrl_F34_UrgeAlarm_Off(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Ctrl_F35_Talk_Off(unsigned char *pucBuffer,unsigned char ucDAp);
//__EXTERN unsigned char Main_Recv_Ctrl_F36_Remove_Off(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Ctrl_F37_UpActive_Off(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Ctrl_F38_SMS_Active(unsigned char *pucBuffer,unsigned char ucDAp,unsigned char ucMsgSource);
//azh 170920
__EXTERN unsigned char Main_Recv_Ctrl_F149_MeterSelf_Switch(unsigned char *pucBuffer,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_Ctrl_F150_ResetSnState_Switch(unsigned char *pucBuffer,unsigned char ucDAp);
//================  TaskData ======================
__EXTERN unsigned char Main_Recv_ReqTaskData_F1(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F1    1类数据
__EXTERN unsigned char Main_Recv_ReqTaskData_F2(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F2    2类数据
//================  ReqData_2 ======================
__EXTERN unsigned char Main_Recv_ReqData_2_F1(STDATAITEM *pstDataItem,unsigned char ucDAp); //F1    日冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
__EXTERN unsigned char Main_Recv_ReqData_2_F2(STDATAITEM *pstDataItem,unsigned char ucDAp); //F2    日冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
__EXTERN unsigned char Main_Recv_ReqData_2_F3(STDATAITEM *pstDataItem,unsigned char ucDAp); //F3    日冻结正向有/无功最大需量及发生时间（总、费率1~M）
__EXTERN unsigned char Main_Recv_ReqData_2_F4(STDATAITEM *pstDataItem,unsigned char ucDAp); //F4    日冻结反向有/无功最大需量及发生时间（总、费率1~M）
__EXTERN unsigned char Main_Recv_ReqData_2_F5(STDATAITEM *pstDataItem,unsigned char ucDAp); //F5    日冻结正向有功电能量（总、费率1~M）
__EXTERN unsigned char Main_Recv_ReqData_2_F6(STDATAITEM *pstDataItem,unsigned char ucDAp); //F6    日冻结正向无功电能量（总、费率1~M）
__EXTERN unsigned char Main_Recv_ReqData_2_F7(STDATAITEM *pstDataItem,unsigned char ucDAp); //F7    日冻结反向有功电能量（总、费率1~M）
__EXTERN unsigned char Main_Recv_ReqData_2_F8(STDATAITEM *pstDataItem,unsigned char ucDAp); //F8    日冻结反向无功电能量（总、费率1~M）
__EXTERN unsigned char Main_Recv_ReqData_2_F9(STDATAITEM *pstDataItem,unsigned char ucDAp); //F9    抄表日冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
__EXTERN unsigned char Main_Recv_ReqData_2_F10(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F10   抄表日冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
__EXTERN unsigned char Main_Recv_ReqData_2_F11(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F11   抄表日冻结正向有/无功最大需量及发生时间（总、费率1~M）
__EXTERN unsigned char Main_Recv_ReqData_2_F12(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F12   抄表日冻结反向有/无功最大需量及发生时间（总、费率1~M）
__EXTERN unsigned char Main_Recv_ReqData_2_F17(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F17   月冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1~M）
__EXTERN unsigned char Main_Recv_ReqData_2_F18(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F18   月冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1~M）
__EXTERN unsigned char Main_Recv_ReqData_2_F19(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F19   月冻结正向有/无功最大需量及发生时间（总、费率1~M）
__EXTERN unsigned char Main_Recv_ReqData_2_F20(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F20   月冻结反向有/无功最大需量及发生时间（总、费率1~M）
__EXTERN unsigned char Main_Recv_ReqData_2_F21(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F21   月冻结正向有功电能量（总、费率1~M）
__EXTERN unsigned char Main_Recv_ReqData_2_F22(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F22   月冻结正向无功电能量（总、费率1~M）
__EXTERN unsigned char Main_Recv_ReqData_2_F23(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F23   月冻结反向有功电能量（总、费率1~M）
__EXTERN unsigned char Main_Recv_ReqData_2_F24(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F24   月冻结反向无功电能量（总、费率1~M）
__EXTERN unsigned char Main_Recv_ReqData_2_F25(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F25   日分相最大有功功率及发生时间、有功功率为零时间
__EXTERN unsigned char Main_Recv_ReqData_2_F26(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F26   日总及分相最大需量及发生时间
__EXTERN unsigned char Main_Recv_ReqData_2_F27(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F27   日电压统计数据
__EXTERN unsigned char Main_Recv_ReqData_2_F28(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F28   日不平衡度越限累计时间
__EXTERN unsigned char Main_Recv_ReqData_2_F29(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F29   日电流越限统计
__EXTERN unsigned char Main_Recv_ReqData_2_F30(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F30   日视在功率越限累计时间
__EXTERN unsigned char Main_Recv_ReqData_2_F31(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F31   日电能表断相数据
__EXTERN unsigned char Main_Recv_ReqData_2_F32(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F32   备用
__EXTERN unsigned char Main_Recv_ReqData_2_F33(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F33   月分相最大有功功率及发生时间、分相有功功率为零时间
__EXTERN unsigned char Main_Recv_ReqData_2_F34(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F34   月分相有功最大需量及发生时间、总有功最大需量及发生时间
__EXTERN unsigned char Main_Recv_ReqData_2_F35(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F35   月电压统计数据
__EXTERN unsigned char Main_Recv_ReqData_2_F36(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F36   月不平衡度越限累计时间
__EXTERN unsigned char Main_Recv_ReqData_2_F37(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F37   月电流越限统计
__EXTERN unsigned char Main_Recv_ReqData_2_F38(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F38   月视在功率越限累计时间
//__EXTERN unsigned char Main_Recv_ReqData_2_F41(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F41   电容器投入累计时间和次数
//__EXTERN unsigned char Main_Recv_ReqData_2_F42(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F42   日、月电容器累计补偿的无功电能量
__EXTERN unsigned char Main_Recv_ReqData_2_F43(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F43   日功率因数区段累计时间
__EXTERN unsigned char Main_Recv_ReqData_2_F44(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F44   月功率因数区段累计时间
__EXTERN unsigned char Main_Recv_ReqData_2_F49(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F49   终端日供电时间、日复位累计次数
//__EXTERN unsigned char Main_Recv_ReqData_2_F50(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F50   终端日控制统计数据
__EXTERN unsigned char Main_Recv_ReqData_2_F51(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F51   终端月供电时间、月复位累计次数
//__EXTERN unsigned char Main_Recv_ReqData_2_F52(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F52   终端月控制统计数据
__EXTERN unsigned char Main_Recv_ReqData_2_F53(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F53   终端与主站日通信流量
__EXTERN unsigned char Main_Recv_ReqData_2_F54(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F54   终端与主站月通信流量
//__EXTERN unsigned char Main_Recv_ReqData_2_F57(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F57   总加组日最大、最小有功功率及其发生时间，有功功率为零日累计时间
//__EXTERN unsigned char Main_Recv_ReqData_2_F58(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F58   总加组日累计有功电能量（总、费率1~M）
//__EXTERN unsigned char Main_Recv_ReqData_2_F59(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F59   总加组日累计无功电能量（总、费率1~M）
//__EXTERN unsigned char Main_Recv_ReqData_2_F60(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F60   总加组月最大、最小有功功率及其发生时间，有功功率为零月累计时间
//__EXTERN unsigned char Main_Recv_ReqData_2_F61(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F61   总加组月累计有功电能量（总、费率1~M）
//__EXTERN unsigned char Main_Recv_ReqData_2_F62(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F62   总加组月累计无功电能量（总、费率1~M）
//__EXTERN unsigned char Main_Recv_ReqData_2_F65(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F65   总加组超功率定值的月累计时间、月累计电能量
//__EXTERN unsigned char Main_Recv_ReqData_2_F66(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F66   总加组超月电能量定值的月累计时间、累计电能量
//__EXTERN unsigned char Main_Recv_ReqData_2_F73(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F73   总加组有功功率曲线
//__EXTERN unsigned char Main_Recv_ReqData_2_F74(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F74   总加组无功功率曲线
//__EXTERN unsigned char Main_Recv_ReqData_2_F75(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F75   总加组有功电能量曲线
//__EXTERN unsigned char Main_Recv_ReqData_2_F76(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F76   总加组无功电能量曲线
__EXTERN unsigned char Main_Recv_ReqData_2_F81(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F81   有功功率曲线
__EXTERN unsigned char Main_Recv_ReqData_2_F82(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F82   A相有功功率曲线
__EXTERN unsigned char Main_Recv_ReqData_2_F83(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F83   B相有功功率曲线
__EXTERN unsigned char Main_Recv_ReqData_2_F84(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F84   C相有功功率曲线
__EXTERN unsigned char Main_Recv_ReqData_2_F85(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F85   无功功率曲线
__EXTERN unsigned char Main_Recv_ReqData_2_F86(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F86   A相无功功率曲线
__EXTERN unsigned char Main_Recv_ReqData_2_F87(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F87   B相无功功率曲线
__EXTERN unsigned char Main_Recv_ReqData_2_F88(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F88   C相无功功率曲线
__EXTERN unsigned char Main_Recv_ReqData_2_F89(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F89   A相电压曲线
__EXTERN unsigned char Main_Recv_ReqData_2_F90(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F90   B相电压曲线
__EXTERN unsigned char Main_Recv_ReqData_2_F91(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F91   C相电压曲线
__EXTERN unsigned char Main_Recv_ReqData_2_F92(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F92   A相电流曲线
__EXTERN unsigned char Main_Recv_ReqData_2_F93(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F93   B相电流曲线
__EXTERN unsigned char Main_Recv_ReqData_2_F94(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F94   C相电流曲线
__EXTERN unsigned char Main_Recv_ReqData_2_F95(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F95   零序电流曲线
__EXTERN unsigned char Main_Recv_ReqData_2_F97(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F97   正向有功总电能量
__EXTERN unsigned char Main_Recv_ReqData_2_F98(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F98   正向无功总电能量
__EXTERN unsigned char Main_Recv_ReqData_2_F99(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F99   反向有功总电能量
__EXTERN unsigned char Main_Recv_ReqData_2_F100(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F100  反向无功总电能量
__EXTERN unsigned char Main_Recv_ReqData_2_F101(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F101  正向有功总电能示值
__EXTERN unsigned char Main_Recv_ReqData_2_F102(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F102  正向无功总电能示值
__EXTERN unsigned char Main_Recv_ReqData_2_F103(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F103  反向有功总电能示值
__EXTERN unsigned char Main_Recv_ReqData_2_F104(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F104  反向无功总电能示值
__EXTERN unsigned char Main_Recv_ReqData_2_F105(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F105  总功率因数
__EXTERN unsigned char Main_Recv_ReqData_2_F106(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F106  A相功率因数
__EXTERN unsigned char Main_Recv_ReqData_2_F107(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F107  B相功率因数
__EXTERN unsigned char Main_Recv_ReqData_2_F108(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F108  C相功率因数
//__EXTERN unsigned char Main_Recv_ReqData_2_F113(STDATAITEM *pstDataItem,unsigned char ucDAp); //F113  A相2~19次谐波电流日最大值及发生时间
//__EXTERN unsigned char Main_Recv_ReqData_2_F114(STDATAITEM *pstDataItem,unsigned char ucDAp); //F114  B相2~19次谐波电流日最大值及发生时间
//__EXTERN unsigned char Main_Recv_ReqData_2_F115(STDATAITEM *pstDataItem,unsigned char ucDAp); //F115  C相2~19次谐波电流日最大值及发生时间
//__EXTERN unsigned char Main_Recv_ReqData_2_F116(STDATAITEM *pstDataItem,unsigned char ucDAp); //F116  A相2~19次谐波电压含有率及总畸变率日最大值及发生时间
//__EXTERN unsigned char Main_Recv_ReqData_2_F117(STDATAITEM *pstDataItem,unsigned char ucDAp); //F117  B相2~19次谐波电压含有率及总畸变率日最大值及发生时间
//__EXTERN unsigned char Main_Recv_ReqData_2_F118(STDATAITEM *pstDataItem,unsigned char ucDAp); //F118  C相2~19次谐波电压含有率及总畸变率日最大值及发生时间
//__EXTERN unsigned char Main_Recv_ReqData_2_F121(STDATAITEM *pstDataItem,unsigned char ucDAp); //F121  A相谐波越限日统计数据
//__EXTERN unsigned char Main_Recv_ReqData_2_F122(STDATAITEM *pstDataItem,unsigned char ucDAp); //F122  B相谐波越限日统计数据
//__EXTERN unsigned char Main_Recv_ReqData_2_F123(STDATAITEM *pstDataItem,unsigned char ucDAp); //F123  C相谐波越限日统计数据
//__EXTERN unsigned char Main_Recv_ReqData_2_F129(STDATAITEM *pstDataItem,unsigned char ucDAp); //F129  直流模拟量越限日累计时间、最大/最小值及发生时间
//__EXTERN unsigned char Main_Recv_ReqData_2_F130(STDATAITEM *pstDataItem,unsigned char ucDAp); //F130  直流模拟量越限月累计时间、最大/最小值及发生时间
//__EXTERN unsigned char Main_Recv_ReqData_2_F138(STDATAITEM *pstDataItem,unsigned char ucDAp); //F138  直流模拟量数据曲线
__EXTERN unsigned char Main_Recv_ReqData_2_F145(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F145  一象限无功总电能示值曲线
__EXTERN unsigned char Main_Recv_ReqData_2_F146(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F146  四象限无功总电能示值曲线
__EXTERN unsigned char Main_Recv_ReqData_2_F147(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F147  二象限无功总电能示值曲线
__EXTERN unsigned char Main_Recv_ReqData_2_F148(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F148  三象限无功总电能示值曲线
__EXTERN unsigned char Main_Recv_ReqData_2_F153(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ReqData_2_F154(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ReqData_2_F155(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ReqData_2_F156(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ReqData_2_F157(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ReqData_2_F158(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ReqData_2_F159(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ReqData_2_F160(STDATAITEM *pstDataItem,unsigned char ucDAp);
//azh 170925
__EXTERN unsigned char Main_Recv_ReqData_2_F219(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F101  正向有功总电能示值

__EXTERN unsigned char Main_Recv_ReqData_2_InstantPow(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned short usDI);
__EXTERN unsigned char Main_Recv_ReqData_2_Volt(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned short usDI);
__EXTERN unsigned char Main_Recv_ReqData_2_Curr(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned short usDI);
__EXTERN unsigned char Main_Recv_ReqData_2_PowVal(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned short usDI);
__EXTERN unsigned char Main_Recv_ReqData_2_PowViewData(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned short usDI);
__EXTERN unsigned char Main_Recv_ReqData_2_PowRate(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned short usDI);
__EXTERN unsigned char Main_Recv_ReqData_2_SumGroup_Pow(STDATAITEM *pstDataItem,unsigned char ucGroupNo,unsigned short usDI,unsigned char ucPowFlag);
//azh 170925
__EXTERN unsigned char Main_Recv_ReqData_2_GroupCurveData(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned short usDI);

//================  ReqData_3 ======================
__EXTERN unsigned char Main_Recv_ReqData_3_Important(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned char *pucECLeftStart,unsigned char ucECLastEnd);
__EXTERN unsigned char Main_Recv_ReqData_3_Normal(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned char *pucECLeftStart,unsigned char ucECLastEnd);

//================  ExtCmd ======================
__EXTERN unsigned char Main_Recv_ExtCmd_F1_Mem_Read(STDATAITEM *pstDataItem,unsigned char ucDAp);       //F1 内存读
__EXTERN unsigned char Main_Recv_ExtCmd_F2_Mem_Write(STDATAITEM *pstDataItem,unsigned char ucDAp);      //F2 内存写
__EXTERN unsigned char Main_Recv_ExtCmd_F3_Dataflash_Read(STDATAITEM *pstDataItem,unsigned char ucDAp); //F3 DATAFLASH读
__EXTERN unsigned char Main_Recv_ExtCmd_F4_Dataflash_Write(STDATAITEM *pstDataItem,unsigned char ucDAp);//F4 DATAFLASH写
__EXTERN unsigned char Main_Recv_ExtCmd_F5_WdtTest_EnterDeadLoop(STDATAITEM *pstDataItem,unsigned char ucDAp);//F5 看门狗测试，让程序进入死循环
__EXTERN unsigned char Main_Recv_ExtCmd_F6_GetCommInfo(STDATAITEM *pstDataItem,unsigned char ucDAp);        //F6 手机模块通讯检测
__EXTERN unsigned char Main_Recv_ExtCmd_F7_GetTerErrInfo(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F7 终端状态检测
__EXTERN unsigned char Main_Recv_ExtCmd_F10_TerExtInfo(STDATAITEM *pstDataItem,unsigned char ucDAp);        //F10 终端扩展信息配置
__EXTERN unsigned char Main_Recv_ExtCmd_F11_GetShortMessage(STDATAITEM *pstDataItem,unsigned char ucDAp);//F11 中文信息读取
__EXTERN unsigned char Main_Recv_ExtCmd_F23_GRInfo_Read(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F23 GR信息读
__EXTERN unsigned char Main_Recv_ExtCmd_F24_GRInfo_Write(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F23 GR信息写
__EXTERN unsigned char Main_Recv_ExtCmd_F31_ClearDemand(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F31 脉冲需量清零
__EXTERN unsigned char Main_Recv_ExtCmd_F33_SetPowDataAll(STDATAITEM *pstDataItem,unsigned char ucDAp); //F33 脉冲底度设置内容全
__EXTERN unsigned char Main_Recv_ExtCmd_F40_7758SetMiscData(STDATAITEM *pstDataItem,unsigned char ucDAp);//F40 内表数据读取
__EXTERN unsigned char Main_Recv_ExtCmd_F41_7758GetMiscData(STDATAITEM *pstDataItem,unsigned char ucDAp);//F41 内表数据设置
__EXTERN unsigned char Main_Recv_ExtCmd_F51_Get_TerAddr(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F51 终端地址行政区号读
__EXTERN unsigned char Main_Recv_ExtCmd_F52_Get_TerSequNo(STDATAITEM *pstDataItem,unsigned char ucDAp); //F52 终端序号读
__EXTERN unsigned char Main_Recv_ExtCmd_F53_Set_TerAddr(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F53 终端地址行政区号写
__EXTERN unsigned char Main_Recv_ExtCmd_F54_Set_TerSequNo(STDATAITEM *pstDataItem,unsigned char ucDAp); //F54 终端序号写
__EXTERN unsigned char Main_Recv_ExtCmd_F71_Ctrl_Beep(STDATAITEM *pstDataItem,unsigned char ucDAp);     //F71 蜂鸣器控制
__EXTERN unsigned char Main_Recv_ExtCmd_F72_ChangeTime(STDATAITEM *pstDataItem,unsigned char ucDAp);//F72 修改时钟
__EXTERN unsigned char Main_Recv_ExtCmd_F73_Get_CommBoad(STDATAITEM *pstDataItem,unsigned char ucDAp);//F73 查询通讯波特率
__EXTERN unsigned char Main_Recv_ExtCmd_F74_Set_CommBoad(STDATAITEM *pstDataItem,unsigned char ucDAp);//F74 修改通讯波特率
//__EXTERN unsigned char Main_Recv_ExtCmd_F75_Get_LCDCycle(STDATAITEM *pstDataItem,unsigned char ucDAp);//F75 读取液晶轮显显示参数
//__EXTERN unsigned char Main_Recv_ExtCmd_F76_Get_LCDCtrl(STDATAITEM *pstDataItem,unsigned char ucDAp);//F76 读取液晶操作显示参数
//__EXTERN unsigned char Main_Recv_ExtCmd_F77_Get_LCDSpec(STDATAITEM *pstDataItem,unsigned char ucDAp);//F77 读取液晶特殊显示参数
//__EXTERN unsigned char Main_Recv_ExtCmd_F78_Set_LCDCycle(STDATAITEM *pstDataItem,unsigned char ucDAp);//F175 修改液晶轮显显示参数
//__EXTERN unsigned char Main_Recv_ExtCmd_F79_Set_LCDCtrl(STDATAITEM *pstDataItem,unsigned char ucDAp);//F176 修改液晶操作显示参数
//__EXTERN unsigned char Main_Recv_ExtCmd_F80_Set_LCDSpec(STDATAITEM *pstDataItem,unsigned char ucDAp);//F177 修改液晶特殊显示参数
__EXTERN unsigned char Main_Recv_ExtCmd_F81_Sim_StartPin(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F81 启用SIM卡的pin
__EXTERN unsigned char Main_Recv_ExtCmd_F82_Sim_SetPin(STDATAITEM *pstDataItem,unsigned char ucDAp);        //F82 修改SIM卡的pin
__EXTERN unsigned char Main_Recv_ExtCmd_F83_Sim_PukNewPin(STDATAITEM *pstDataItem,unsigned char ucDAp); //F83 用PUK设置SIM卡的pin
__EXTERN unsigned char Main_Recv_ExtCmd_F84_Sim_GetPinStat(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F84 SIM卡的pin相关状态
__EXTERN unsigned char Main_Recv_ExtCmd_F85_Set_GSM_GRPS(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F85 GSM/GPRS切换
__EXTERN unsigned char Main_Recv_ExtCmd_F86_Get_PowerCtrl_TimeRecType(STDATAITEM *pstDataItem,unsigned char ucDAp); //F86 读功率控制恢复时间长度
__EXTERN unsigned char Main_Recv_ExtCmd_F87_Set_PowerCtrl_TimeRecType(STDATAITEM *pstDataItem,unsigned char ucDAp); //F87 写功率控制恢复时间长度
__EXTERN unsigned char Main_Recv_ExtCmd_F89_Get_PowerCtrl_DelayTime(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F89 读功率控制越限确认时间
__EXTERN unsigned char Main_Recv_ExtCmd_F90_Set_PowerCtrl_DelayTime(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F90 写功率控制越限确认时间
__EXTERN unsigned char Main_Recv_ExtCmd_F91_Get_AdjPara(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F91 读自定义调整参数
__EXTERN unsigned char Main_Recv_ExtCmd_F92_Set_AdjPara(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F92 写自定义调整参数
__EXTERN unsigned char Main_Recv_ExtCmd_F93_Get_TerVoltCurr(STDATAITEM *pstDataItem,unsigned char ucDAp);//F93 读取终端电压两位小数电流四位小数
__EXTERN unsigned char Main_Recv_ExtCmd_F94_Init_CTask(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F94 任务初始化
__EXTERN unsigned char Main_Recv_ExtCmd_F95_Adj_RX8025(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F95 8025时钟矫正
__EXTERN unsigned char Main_Recv_ExtCmd_F96_Set_LCDContrast(STDATAITEM *pstDataItem,unsigned char ucDAp);//F96 液晶对比度设置
__EXTERN unsigned char Main_Recv_ExtCmd_F97_Set_MeterCommName(STDATAITEM *pstDataItem,unsigned char ucDAp);//F97 设置表计用户名
__EXTERN unsigned char Main_Recv_ExtCmd_F98_Get_MeterCommName(STDATAITEM *pstDataItem,unsigned char ucDAp); //F98 读取表计用户名
__EXTERN unsigned char Main_Recv_ExtCmd_F99_Set_GprsPDPPara(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F99 设置GPRS接入的PDP用户名、密码
__EXTERN unsigned char Main_Recv_ExtCmd_F100_Get_GprsPDPPara(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F100 读取GPRS接入的PDP用户名、密码
__EXTERN unsigned char Main_Recv_ExtCmd_F101_Set_GprsRelinkTime(STDATAITEM *pstDataItem,unsigned char ucDAp);//F101 设置GPRS重连间隔时间
__EXTERN unsigned char Main_Recv_ExtCmd_F102_Get_GprsRelinkTime(STDATAITEM *pstDataItem,unsigned char ucDAp);//F102 读取GPRS重连间隔时间
__EXTERN unsigned char Main_Recv_ExtCmd_F109_Set_TerWorkMode(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ExtCmd_F110_Get_TerWorkMode(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ExtCmd_F111_Set_CTType(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ExtCmd_F112_Get_CTType(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ExtCmd_F113_Set_SlfStudyFlag(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ExtCmd_F114_Get_SlfStudyFlag(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ExtCmd_F115_Set_Cir2OpenLimitVal(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ExtCmd_F116_Get_Cir2OpenLimitVal(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ExtCmd_F117_Set_Cir1ShortLimitVal(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ExtCmd_F118_Get_Cir1ShortLimitVal(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ExtCmd_F119_Get_CTCheckVal(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ExtCmd_F120_Set_AESKey(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ExtCmd_F121_Get_AESKey(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ExtCmd_F122_Get_KeepStatus(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ExtCmd_F123_Get_Temperature(STDATAITEM *pstDataItem,unsigned char ucDAp);


//================  DataTrans ====================
__EXTERN unsigned char Main_Recv_DataTrans_F1(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned char ucMsgSource,unsigned char ucSeqNo); //F1 数据转发
//unsigned char Main_Recv_DataTrans_F9(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned char ucMsgSource,unsigned char ucSeqNo);

//================  FileTrans ====================
__EXTERN unsigned char Main_Recv_FileTrans_F1(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned char ucMsgSource,unsigned char ucSeqNo); //F1 文件转发
__EXTERN unsigned char Main_Recv_FileTrans_F100(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned char ucMsgSource,unsigned char ucSeqNo);//F100 程序下装命令


//获得信息点的属性，目前只返回信息点下面数据点的长度
__EXTERN unsigned char Main_Get_DataItem_Info(unsigned char ucRepType,unsigned char ucDTF);
__EXTERN unsigned char Main_Get_DataItem_Info_ACK(unsigned char ucDTF);
__EXTERN unsigned char Main_Get_DataItem_Info_Reset(unsigned char ucDTF);
__EXTERN unsigned char Main_Get_DataItem_Info_SetPara(unsigned char ucDTF);
__EXTERN unsigned char Main_Get_DataItem_Info_Control(unsigned char ucDTF);
__EXTERN unsigned char Main_Get_DataItem_Info_GetPara(unsigned char ucDTF);
__EXTERN unsigned char Main_Get_DataItem_Info_ReqData_1(unsigned char ucDTF);
__EXTERN unsigned char Main_Get_DataItem_Info_ReqData_2(unsigned char ucDTF);
__EXTERN unsigned char Main_Get_DataItem_Info_ReqData_3(unsigned char ucDTF);
__EXTERN unsigned char Main_Get_DataItem_Info_ExtCmd(unsigned char ucDTF);
__EXTERN unsigned char Main_Get_DataItem_Info_Relay(unsigned char ucDTF);

//根据点号和信息项号获得数据单元标识
__EXTERN STDATAITEM *GetDataItemTemp(void);

//__EXTERN unsigned short Get_FreezeItem_byMtrComm(unsigned char ucVal);
//__EXTERN unsigned char Get_MtrComm_byFreezeItem(unsigned short usItem);
//__EXTERN unsigned char  Get_FreezeInter(unsigned char ucVal,unsigned char  *pucFreezeInter);
//__EXTERN unsigned char  Get_FreezeInterComm(unsigned char ucFreezeInter);

//__EXTERN unsigned char Check_DUID_IsSingle(STDATAUNITID stDUID);
__EXTERN unsigned char Check_IsOneBit(unsigned char ucData);
__EXTERN void GetData2_DayTime(unsigned char *pucBuffer, STDATETIME *pstDateTime);
__EXTERN void GetData2_MonthTime(unsigned char *pucBuffer, STDATETIME *pstDateTime);

__EXTERN unsigned char SetAlrCategoryVal(unsigned char *pucVal,unsigned char ucFlag,unsigned char ucSetVal1,unsigned char ucSetVal2);
__EXTERN unsigned char SetAlrCategoryComm(unsigned char ucVal,unsigned char *pucFlag,unsigned char ucPos,unsigned char ucSetVal1,unsigned char ucSetVal2);
__EXTERN STDATETIME GetLastOneHourStartTime(unsigned char ucMinOffset);
__EXTERN unsigned char GetFreeseMByMPNo(unsigned char ucMPNo);
__EXTERN unsigned char GetFreeseMByGroupNo(unsigned char ucMPNo);

__EXTERN unsigned char Main_GetPortCtrlByte(STPORTPROP *pstPortProp);
__EXTERN unsigned char  Main_GetPortProp(STPORTPROP *pstPortProp,unsigned char ucData);
__EXTERN unsigned short SetMainComm_Buf_lVal_5B_DT25(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal);
//程序下装数据传输
//unsigned char dl_Start(unsigned char *pucBuffer,unsigned char ucMsgSource,unsigned short usDataLen);
unsigned char dl_Data(unsigned char *pucBuffer,unsigned char ucMsgSource,unsigned short usDataLen);
unsigned char dl_Query(unsigned char *pucBuffer,unsigned char ucMsgSource,unsigned short usDataLen);
//unsigned char dl_Set(unsigned char *pucBuffer,unsigned char ucMsgSource,unsigned short usDataLen);
unsigned char dl_Execute(unsigned char *pucBuffer,unsigned char ucMsgSource,unsigned short usDataLen);
//unsigned char dl_End(unsigned char *pucBuffer,unsigned char ucMsgSource,unsigned short usDataLen);
//unsigned char dl_Finished(unsigned char *pucBuffer,unsigned char ucMsgSource,unsigned short usDataLen,unsigned char ucSuccessFlag);
__EXTERN unsigned char DP_Get_TerDP_State(void);
__EXTERN void DP_Set_TerDP_State(unsigned char ucState);
//__EXTERN unsigned short Float_To_DT2(float fVal);
#endif


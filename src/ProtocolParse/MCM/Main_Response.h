/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_Response.h
 Description    :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-02-05    azh     the original version
******************************************************************************/
#ifndef _MAIN_RESPONSE_H_
#define _MAIN_RESPONSE_H_

#define CON_ANSWER_YES  0x55
#define CON_ANSWER_NO   0xAA

#define CON_FRAME_CMD_DIR_UP    0x80            //DIR=0 ��ʾ��֡����������վ���������б��ġ�DIR=1   ��ʾ��֡���������ն˷��������б��ġ�
#define CON_FRAME_CMD_PRM_ORG   0x40            //PRM=0 ��ʾ��֡����������Ӧվ��ͨ����Ӧվ��PRM=1   ��ʾ��֡��������ԭ��վ��ͨ������վ��
#define CON_FRAME_CMD_ACD_IMP   0x20            //ACD=1 ��ʾ�ն�����Ҫ�¼��ȴ����ʣ�Ӧ�ò������������¼�������EC����ʾ��վ�¼��������ۻ�������ACD=0��ʾ�ն����¼����ݵȴ�����
#define CON_FRAME_CMD_FCB       0x20            //FCB��ʾÿ��վ�����ķ���/ȷ�ϻ�������/��Ӧ����ı仯λ
#define CON_FRAME_CMD_FCV_VLD   0x10            //FCV=1����ʾFCBλ��Ч��FCV=0����ʾFCBλ��Ч��

#define CON_FRAME_SEQ_TPV   0x80            //1��ʾ�ڸ�����Ϣ���д���ʱ���ǩTp
#define CON_FRAME_SEQ_FIR   0x40            //1��ʾ�����ķֶ�������Ӧ�ñ��ĵĵ�һ���ֶ�
#define CON_FRAME_SEQ_FIN   0x20            //1��ʾ�����ķֶ�������Ӧ�ñ��ĵ����һ���ֶ�
#define CON_FRAME_SEQ_CON   0x10            //1��ʾ���͵�Ӧ�ñ����ڴ��ӽ��յ�Ӧ�ñ����и���ȷ��

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

typedef struct _STEXPLAINRETDATA        //���ؽ�����
{
    unsigned char ucExplainRetDataNum;
    STEXPLAINRETDATAEXTBLOCK stExplainExtData;
    STEXPLAINRETDATABLOCK stExplainRetData[CON_EXPLAIN_RET_DATA_BLOCK_NUM];
}STEXPLAINRETDATA;

//1��
__EXTERN unsigned char Main_Recv_Manage(STTERMSG    *pstRecvTerMsg,unsigned char ucDataItemNo,unsigned char ucMSA,unsigned char ucSEQ);
//2��
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

//3��
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

__EXTERN unsigned char Main_Recv_Ctrl_F31_SetTerTime(unsigned char *pucBuffer,unsigned char ucDAp); //F31 ��ʱ����
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
__EXTERN unsigned char Main_Recv_ReqTaskData_F1(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F1    1������
__EXTERN unsigned char Main_Recv_ReqTaskData_F2(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F2    2������
//================  ReqData_2 ======================
__EXTERN unsigned char Main_Recv_ReqData_2_F1(STDATAITEM *pstDataItem,unsigned char ucDAp); //F1    �ն���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
__EXTERN unsigned char Main_Recv_ReqData_2_F2(STDATAITEM *pstDataItem,unsigned char ucDAp); //F2    �ն��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
__EXTERN unsigned char Main_Recv_ReqData_2_F3(STDATAITEM *pstDataItem,unsigned char ucDAp); //F3    �ն���������/�޹��������������ʱ�䣨�ܡ�����1~M��
__EXTERN unsigned char Main_Recv_ReqData_2_F4(STDATAITEM *pstDataItem,unsigned char ucDAp); //F4    �ն��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��
__EXTERN unsigned char Main_Recv_ReqData_2_F5(STDATAITEM *pstDataItem,unsigned char ucDAp); //F5    �ն��������й����������ܡ�����1~M��
__EXTERN unsigned char Main_Recv_ReqData_2_F6(STDATAITEM *pstDataItem,unsigned char ucDAp); //F6    �ն��������޹����������ܡ�����1~M��
__EXTERN unsigned char Main_Recv_ReqData_2_F7(STDATAITEM *pstDataItem,unsigned char ucDAp); //F7    �ն��ᷴ���й����������ܡ�����1~M��
__EXTERN unsigned char Main_Recv_ReqData_2_F8(STDATAITEM *pstDataItem,unsigned char ucDAp); //F8    �ն��ᷴ���޹����������ܡ�����1~M��
__EXTERN unsigned char Main_Recv_ReqData_2_F9(STDATAITEM *pstDataItem,unsigned char ucDAp); //F9    �����ն���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
__EXTERN unsigned char Main_Recv_ReqData_2_F10(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F10   �����ն��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
__EXTERN unsigned char Main_Recv_ReqData_2_F11(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F11   �����ն���������/�޹��������������ʱ�䣨�ܡ�����1~M��
__EXTERN unsigned char Main_Recv_ReqData_2_F12(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F12   �����ն��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��
__EXTERN unsigned char Main_Recv_ReqData_2_F17(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F17   �¶���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
__EXTERN unsigned char Main_Recv_ReqData_2_F18(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F18   �¶��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
__EXTERN unsigned char Main_Recv_ReqData_2_F19(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F19   �¶���������/�޹��������������ʱ�䣨�ܡ�����1~M��
__EXTERN unsigned char Main_Recv_ReqData_2_F20(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F20   �¶��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��
__EXTERN unsigned char Main_Recv_ReqData_2_F21(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F21   �¶��������й����������ܡ�����1~M��
__EXTERN unsigned char Main_Recv_ReqData_2_F22(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F22   �¶��������޹����������ܡ�����1~M��
__EXTERN unsigned char Main_Recv_ReqData_2_F23(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F23   �¶��ᷴ���й����������ܡ�����1~M��
__EXTERN unsigned char Main_Recv_ReqData_2_F24(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F24   �¶��ᷴ���޹����������ܡ�����1~M��
__EXTERN unsigned char Main_Recv_ReqData_2_F25(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F25   �շ�������й����ʼ�����ʱ�䡢�й�����Ϊ��ʱ��
__EXTERN unsigned char Main_Recv_ReqData_2_F26(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F26   ���ܼ������������������ʱ��
__EXTERN unsigned char Main_Recv_ReqData_2_F27(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F27   �յ�ѹͳ������
__EXTERN unsigned char Main_Recv_ReqData_2_F28(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F28   �ղ�ƽ���Խ���ۼ�ʱ��
__EXTERN unsigned char Main_Recv_ReqData_2_F29(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F29   �յ���Խ��ͳ��
__EXTERN unsigned char Main_Recv_ReqData_2_F30(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F30   �����ڹ���Խ���ۼ�ʱ��
__EXTERN unsigned char Main_Recv_ReqData_2_F31(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F31   �յ��ܱ��������
__EXTERN unsigned char Main_Recv_ReqData_2_F32(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F32   ����
__EXTERN unsigned char Main_Recv_ReqData_2_F33(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F33   �·�������й����ʼ�����ʱ�䡢�����й�����Ϊ��ʱ��
__EXTERN unsigned char Main_Recv_ReqData_2_F34(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F34   �·����й��������������ʱ�䡢���й��������������ʱ��
__EXTERN unsigned char Main_Recv_ReqData_2_F35(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F35   �µ�ѹͳ������
__EXTERN unsigned char Main_Recv_ReqData_2_F36(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F36   �²�ƽ���Խ���ۼ�ʱ��
__EXTERN unsigned char Main_Recv_ReqData_2_F37(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F37   �µ���Խ��ͳ��
__EXTERN unsigned char Main_Recv_ReqData_2_F38(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F38   �����ڹ���Խ���ۼ�ʱ��
//__EXTERN unsigned char Main_Recv_ReqData_2_F41(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F41   ������Ͷ���ۼ�ʱ��ʹ���
//__EXTERN unsigned char Main_Recv_ReqData_2_F42(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F42   �ա��µ������ۼƲ������޹�������
__EXTERN unsigned char Main_Recv_ReqData_2_F43(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F43   �չ������������ۼ�ʱ��
__EXTERN unsigned char Main_Recv_ReqData_2_F44(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F44   �¹������������ۼ�ʱ��
__EXTERN unsigned char Main_Recv_ReqData_2_F49(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F49   �ն��չ���ʱ�䡢�ո�λ�ۼƴ���
//__EXTERN unsigned char Main_Recv_ReqData_2_F50(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F50   �ն��տ���ͳ������
__EXTERN unsigned char Main_Recv_ReqData_2_F51(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F51   �ն��¹���ʱ�䡢�¸�λ�ۼƴ���
//__EXTERN unsigned char Main_Recv_ReqData_2_F52(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F52   �ն��¿���ͳ������
__EXTERN unsigned char Main_Recv_ReqData_2_F53(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F53   �ն�����վ��ͨ������
__EXTERN unsigned char Main_Recv_ReqData_2_F54(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F54   �ն�����վ��ͨ������
//__EXTERN unsigned char Main_Recv_ReqData_2_F57(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F57   �ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��
//__EXTERN unsigned char Main_Recv_ReqData_2_F58(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F58   �ܼ������ۼ��й����������ܡ�����1~M��
//__EXTERN unsigned char Main_Recv_ReqData_2_F59(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F59   �ܼ������ۼ��޹����������ܡ�����1~M��
//__EXTERN unsigned char Main_Recv_ReqData_2_F60(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F60   �ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��
//__EXTERN unsigned char Main_Recv_ReqData_2_F61(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F61   �ܼ������ۼ��й����������ܡ�����1~M��
//__EXTERN unsigned char Main_Recv_ReqData_2_F62(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F62   �ܼ������ۼ��޹����������ܡ�����1~M��
//__EXTERN unsigned char Main_Recv_ReqData_2_F65(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F65   �ܼ��鳬���ʶ�ֵ�����ۼ�ʱ�䡢���ۼƵ�����
//__EXTERN unsigned char Main_Recv_ReqData_2_F66(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F66   �ܼ��鳬�µ�������ֵ�����ۼ�ʱ�䡢�ۼƵ�����
//__EXTERN unsigned char Main_Recv_ReqData_2_F73(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F73   �ܼ����й���������
//__EXTERN unsigned char Main_Recv_ReqData_2_F74(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F74   �ܼ����޹���������
//__EXTERN unsigned char Main_Recv_ReqData_2_F75(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F75   �ܼ����й�����������
//__EXTERN unsigned char Main_Recv_ReqData_2_F76(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F76   �ܼ����޹�����������
__EXTERN unsigned char Main_Recv_ReqData_2_F81(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F81   �й���������
__EXTERN unsigned char Main_Recv_ReqData_2_F82(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F82   A���й���������
__EXTERN unsigned char Main_Recv_ReqData_2_F83(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F83   B���й���������
__EXTERN unsigned char Main_Recv_ReqData_2_F84(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F84   C���й���������
__EXTERN unsigned char Main_Recv_ReqData_2_F85(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F85   �޹���������
__EXTERN unsigned char Main_Recv_ReqData_2_F86(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F86   A���޹���������
__EXTERN unsigned char Main_Recv_ReqData_2_F87(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F87   B���޹���������
__EXTERN unsigned char Main_Recv_ReqData_2_F88(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F88   C���޹���������
__EXTERN unsigned char Main_Recv_ReqData_2_F89(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F89   A���ѹ����
__EXTERN unsigned char Main_Recv_ReqData_2_F90(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F90   B���ѹ����
__EXTERN unsigned char Main_Recv_ReqData_2_F91(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F91   C���ѹ����
__EXTERN unsigned char Main_Recv_ReqData_2_F92(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F92   A���������
__EXTERN unsigned char Main_Recv_ReqData_2_F93(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F93   B���������
__EXTERN unsigned char Main_Recv_ReqData_2_F94(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F94   C���������
__EXTERN unsigned char Main_Recv_ReqData_2_F95(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F95   �����������
__EXTERN unsigned char Main_Recv_ReqData_2_F97(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F97   �����й��ܵ�����
__EXTERN unsigned char Main_Recv_ReqData_2_F98(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F98   �����޹��ܵ�����
__EXTERN unsigned char Main_Recv_ReqData_2_F99(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F99   �����й��ܵ�����
__EXTERN unsigned char Main_Recv_ReqData_2_F100(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F100  �����޹��ܵ�����
__EXTERN unsigned char Main_Recv_ReqData_2_F101(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F101  �����й��ܵ���ʾֵ
__EXTERN unsigned char Main_Recv_ReqData_2_F102(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F102  �����޹��ܵ���ʾֵ
__EXTERN unsigned char Main_Recv_ReqData_2_F103(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F103  �����й��ܵ���ʾֵ
__EXTERN unsigned char Main_Recv_ReqData_2_F104(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F104  �����޹��ܵ���ʾֵ
__EXTERN unsigned char Main_Recv_ReqData_2_F105(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F105  �ܹ�������
__EXTERN unsigned char Main_Recv_ReqData_2_F106(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F106  A�๦������
__EXTERN unsigned char Main_Recv_ReqData_2_F107(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F107  B�๦������
__EXTERN unsigned char Main_Recv_ReqData_2_F108(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F108  C�๦������
//__EXTERN unsigned char Main_Recv_ReqData_2_F113(STDATAITEM *pstDataItem,unsigned char ucDAp); //F113  A��2~19��г�����������ֵ������ʱ��
//__EXTERN unsigned char Main_Recv_ReqData_2_F114(STDATAITEM *pstDataItem,unsigned char ucDAp); //F114  B��2~19��г�����������ֵ������ʱ��
//__EXTERN unsigned char Main_Recv_ReqData_2_F115(STDATAITEM *pstDataItem,unsigned char ucDAp); //F115  C��2~19��г�����������ֵ������ʱ��
//__EXTERN unsigned char Main_Recv_ReqData_2_F116(STDATAITEM *pstDataItem,unsigned char ucDAp); //F116  A��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
//__EXTERN unsigned char Main_Recv_ReqData_2_F117(STDATAITEM *pstDataItem,unsigned char ucDAp); //F117  B��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
//__EXTERN unsigned char Main_Recv_ReqData_2_F118(STDATAITEM *pstDataItem,unsigned char ucDAp); //F118  C��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
//__EXTERN unsigned char Main_Recv_ReqData_2_F121(STDATAITEM *pstDataItem,unsigned char ucDAp); //F121  A��г��Խ����ͳ������
//__EXTERN unsigned char Main_Recv_ReqData_2_F122(STDATAITEM *pstDataItem,unsigned char ucDAp); //F122  B��г��Խ����ͳ������
//__EXTERN unsigned char Main_Recv_ReqData_2_F123(STDATAITEM *pstDataItem,unsigned char ucDAp); //F123  C��г��Խ����ͳ������
//__EXTERN unsigned char Main_Recv_ReqData_2_F129(STDATAITEM *pstDataItem,unsigned char ucDAp); //F129  ֱ��ģ����Խ�����ۼ�ʱ�䡢���/��Сֵ������ʱ��
//__EXTERN unsigned char Main_Recv_ReqData_2_F130(STDATAITEM *pstDataItem,unsigned char ucDAp); //F130  ֱ��ģ����Խ�����ۼ�ʱ�䡢���/��Сֵ������ʱ��
//__EXTERN unsigned char Main_Recv_ReqData_2_F138(STDATAITEM *pstDataItem,unsigned char ucDAp); //F138  ֱ��ģ������������
__EXTERN unsigned char Main_Recv_ReqData_2_F145(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F145  һ�����޹��ܵ���ʾֵ����
__EXTERN unsigned char Main_Recv_ReqData_2_F146(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F146  �������޹��ܵ���ʾֵ����
__EXTERN unsigned char Main_Recv_ReqData_2_F147(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F147  �������޹��ܵ���ʾֵ����
__EXTERN unsigned char Main_Recv_ReqData_2_F148(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F148  �������޹��ܵ���ʾֵ����
__EXTERN unsigned char Main_Recv_ReqData_2_F153(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ReqData_2_F154(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ReqData_2_F155(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ReqData_2_F156(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ReqData_2_F157(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ReqData_2_F158(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ReqData_2_F159(STDATAITEM *pstDataItem,unsigned char ucDAp);
__EXTERN unsigned char Main_Recv_ReqData_2_F160(STDATAITEM *pstDataItem,unsigned char ucDAp);
//azh 170925
__EXTERN unsigned char Main_Recv_ReqData_2_F219(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F101  �����й��ܵ���ʾֵ

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
__EXTERN unsigned char Main_Recv_ExtCmd_F1_Mem_Read(STDATAITEM *pstDataItem,unsigned char ucDAp);       //F1 �ڴ��
__EXTERN unsigned char Main_Recv_ExtCmd_F2_Mem_Write(STDATAITEM *pstDataItem,unsigned char ucDAp);      //F2 �ڴ�д
__EXTERN unsigned char Main_Recv_ExtCmd_F3_Dataflash_Read(STDATAITEM *pstDataItem,unsigned char ucDAp); //F3 DATAFLASH��
__EXTERN unsigned char Main_Recv_ExtCmd_F4_Dataflash_Write(STDATAITEM *pstDataItem,unsigned char ucDAp);//F4 DATAFLASHд
__EXTERN unsigned char Main_Recv_ExtCmd_F5_WdtTest_EnterDeadLoop(STDATAITEM *pstDataItem,unsigned char ucDAp);//F5 ���Ź����ԣ��ó��������ѭ��
__EXTERN unsigned char Main_Recv_ExtCmd_F6_GetCommInfo(STDATAITEM *pstDataItem,unsigned char ucDAp);        //F6 �ֻ�ģ��ͨѶ���
__EXTERN unsigned char Main_Recv_ExtCmd_F7_GetTerErrInfo(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F7 �ն�״̬���
__EXTERN unsigned char Main_Recv_ExtCmd_F10_TerExtInfo(STDATAITEM *pstDataItem,unsigned char ucDAp);        //F10 �ն���չ��Ϣ����
__EXTERN unsigned char Main_Recv_ExtCmd_F11_GetShortMessage(STDATAITEM *pstDataItem,unsigned char ucDAp);//F11 ������Ϣ��ȡ
__EXTERN unsigned char Main_Recv_ExtCmd_F23_GRInfo_Read(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F23 GR��Ϣ��
__EXTERN unsigned char Main_Recv_ExtCmd_F24_GRInfo_Write(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F23 GR��Ϣд
__EXTERN unsigned char Main_Recv_ExtCmd_F31_ClearDemand(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F31 ������������
__EXTERN unsigned char Main_Recv_ExtCmd_F33_SetPowDataAll(STDATAITEM *pstDataItem,unsigned char ucDAp); //F33 ����׶���������ȫ
__EXTERN unsigned char Main_Recv_ExtCmd_F40_7758SetMiscData(STDATAITEM *pstDataItem,unsigned char ucDAp);//F40 �ڱ����ݶ�ȡ
__EXTERN unsigned char Main_Recv_ExtCmd_F41_7758GetMiscData(STDATAITEM *pstDataItem,unsigned char ucDAp);//F41 �ڱ���������
__EXTERN unsigned char Main_Recv_ExtCmd_F51_Get_TerAddr(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F51 �ն˵�ַ�������Ŷ�
__EXTERN unsigned char Main_Recv_ExtCmd_F52_Get_TerSequNo(STDATAITEM *pstDataItem,unsigned char ucDAp); //F52 �ն���Ŷ�
__EXTERN unsigned char Main_Recv_ExtCmd_F53_Set_TerAddr(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F53 �ն˵�ַ��������д
__EXTERN unsigned char Main_Recv_ExtCmd_F54_Set_TerSequNo(STDATAITEM *pstDataItem,unsigned char ucDAp); //F54 �ն����д
__EXTERN unsigned char Main_Recv_ExtCmd_F71_Ctrl_Beep(STDATAITEM *pstDataItem,unsigned char ucDAp);     //F71 ����������
__EXTERN unsigned char Main_Recv_ExtCmd_F72_ChangeTime(STDATAITEM *pstDataItem,unsigned char ucDAp);//F72 �޸�ʱ��
__EXTERN unsigned char Main_Recv_ExtCmd_F73_Get_CommBoad(STDATAITEM *pstDataItem,unsigned char ucDAp);//F73 ��ѯͨѶ������
__EXTERN unsigned char Main_Recv_ExtCmd_F74_Set_CommBoad(STDATAITEM *pstDataItem,unsigned char ucDAp);//F74 �޸�ͨѶ������
//__EXTERN unsigned char Main_Recv_ExtCmd_F75_Get_LCDCycle(STDATAITEM *pstDataItem,unsigned char ucDAp);//F75 ��ȡҺ��������ʾ����
//__EXTERN unsigned char Main_Recv_ExtCmd_F76_Get_LCDCtrl(STDATAITEM *pstDataItem,unsigned char ucDAp);//F76 ��ȡҺ��������ʾ����
//__EXTERN unsigned char Main_Recv_ExtCmd_F77_Get_LCDSpec(STDATAITEM *pstDataItem,unsigned char ucDAp);//F77 ��ȡҺ��������ʾ����
//__EXTERN unsigned char Main_Recv_ExtCmd_F78_Set_LCDCycle(STDATAITEM *pstDataItem,unsigned char ucDAp);//F175 �޸�Һ��������ʾ����
//__EXTERN unsigned char Main_Recv_ExtCmd_F79_Set_LCDCtrl(STDATAITEM *pstDataItem,unsigned char ucDAp);//F176 �޸�Һ��������ʾ����
//__EXTERN unsigned char Main_Recv_ExtCmd_F80_Set_LCDSpec(STDATAITEM *pstDataItem,unsigned char ucDAp);//F177 �޸�Һ��������ʾ����
__EXTERN unsigned char Main_Recv_ExtCmd_F81_Sim_StartPin(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F81 ����SIM����pin
__EXTERN unsigned char Main_Recv_ExtCmd_F82_Sim_SetPin(STDATAITEM *pstDataItem,unsigned char ucDAp);        //F82 �޸�SIM����pin
__EXTERN unsigned char Main_Recv_ExtCmd_F83_Sim_PukNewPin(STDATAITEM *pstDataItem,unsigned char ucDAp); //F83 ��PUK����SIM����pin
__EXTERN unsigned char Main_Recv_ExtCmd_F84_Sim_GetPinStat(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F84 SIM����pin���״̬
__EXTERN unsigned char Main_Recv_ExtCmd_F85_Set_GSM_GRPS(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F85 GSM/GPRS�л�
__EXTERN unsigned char Main_Recv_ExtCmd_F86_Get_PowerCtrl_TimeRecType(STDATAITEM *pstDataItem,unsigned char ucDAp); //F86 �����ʿ��ƻָ�ʱ�䳤��
__EXTERN unsigned char Main_Recv_ExtCmd_F87_Set_PowerCtrl_TimeRecType(STDATAITEM *pstDataItem,unsigned char ucDAp); //F87 д���ʿ��ƻָ�ʱ�䳤��
__EXTERN unsigned char Main_Recv_ExtCmd_F89_Get_PowerCtrl_DelayTime(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F89 �����ʿ���Խ��ȷ��ʱ��
__EXTERN unsigned char Main_Recv_ExtCmd_F90_Set_PowerCtrl_DelayTime(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F90 д���ʿ���Խ��ȷ��ʱ��
__EXTERN unsigned char Main_Recv_ExtCmd_F91_Get_AdjPara(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F91 ���Զ����������
__EXTERN unsigned char Main_Recv_ExtCmd_F92_Set_AdjPara(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F92 д�Զ����������
__EXTERN unsigned char Main_Recv_ExtCmd_F93_Get_TerVoltCurr(STDATAITEM *pstDataItem,unsigned char ucDAp);//F93 ��ȡ�ն˵�ѹ��λС��������λС��
__EXTERN unsigned char Main_Recv_ExtCmd_F94_Init_CTask(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F94 �����ʼ��
__EXTERN unsigned char Main_Recv_ExtCmd_F95_Adj_RX8025(STDATAITEM *pstDataItem,unsigned char ucDAp);    //F95 8025ʱ�ӽ���
__EXTERN unsigned char Main_Recv_ExtCmd_F96_Set_LCDContrast(STDATAITEM *pstDataItem,unsigned char ucDAp);//F96 Һ���Աȶ�����
__EXTERN unsigned char Main_Recv_ExtCmd_F97_Set_MeterCommName(STDATAITEM *pstDataItem,unsigned char ucDAp);//F97 ���ñ���û���
__EXTERN unsigned char Main_Recv_ExtCmd_F98_Get_MeterCommName(STDATAITEM *pstDataItem,unsigned char ucDAp); //F98 ��ȡ����û���
__EXTERN unsigned char Main_Recv_ExtCmd_F99_Set_GprsPDPPara(STDATAITEM *pstDataItem,unsigned char ucDAp);   //F99 ����GPRS�����PDP�û���������
__EXTERN unsigned char Main_Recv_ExtCmd_F100_Get_GprsPDPPara(STDATAITEM *pstDataItem,unsigned char ucDAp);  //F100 ��ȡGPRS�����PDP�û���������
__EXTERN unsigned char Main_Recv_ExtCmd_F101_Set_GprsRelinkTime(STDATAITEM *pstDataItem,unsigned char ucDAp);//F101 ����GPRS�������ʱ��
__EXTERN unsigned char Main_Recv_ExtCmd_F102_Get_GprsRelinkTime(STDATAITEM *pstDataItem,unsigned char ucDAp);//F102 ��ȡGPRS�������ʱ��
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
__EXTERN unsigned char Main_Recv_DataTrans_F1(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned char ucMsgSource,unsigned char ucSeqNo); //F1 ����ת��
//unsigned char Main_Recv_DataTrans_F9(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned char ucMsgSource,unsigned char ucSeqNo);

//================  FileTrans ====================
__EXTERN unsigned char Main_Recv_FileTrans_F1(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned char ucMsgSource,unsigned char ucSeqNo); //F1 �ļ�ת��
__EXTERN unsigned char Main_Recv_FileTrans_F100(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned char ucMsgSource,unsigned char ucSeqNo);//F100 ������װ����


//�����Ϣ������ԣ�Ŀǰֻ������Ϣ���������ݵ�ĳ���
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

//���ݵ�ź���Ϣ��Ż�����ݵ�Ԫ��ʶ
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
//������װ���ݴ���
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


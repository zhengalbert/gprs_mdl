/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_Response.c
 Description    : ��վ��Լ���Ĵ���
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-08-05    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

STDATAITEM stDataItemTemp;
static unsigned char Check_Data_DT25(unsigned char *pucData);
static unsigned char Main_Recv_Lonin(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo);
static unsigned char Main_Recv_Configuration(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo);
static unsigned char Main_Recv_Ctrl_F39_Disconect(unsigned char *pucBuffer,unsigned char ucDAp,unsigned char ucMsgSource);
static unsigned char Main_Recv_ReqData_2_F109(STDATAITEM *pstDataItem,unsigned char ucDAp);
static unsigned char Main_Recv_ReqData_2_F110(STDATAITEM *pstDataItem,unsigned char ucDAp);
static unsigned char Main_Recv_ReqData_2_F161_F168(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned short usDI);
static unsigned char Main_Recv_ReqData_2_F169_F176(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned short usDI);
static unsigned char Main_Recv_ReqData_2_F177_F184(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned short usDI);
static unsigned char Main_Recv_ReqData_2_F185_F196(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned short usDI);
//static unsigned char Main_Recv_Get_F73_F76(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned short usDI);
const unsigned char Config_Get_F2_Interface[41]={			
	0x02,0x04,0x00,0x04,0x08,0x00,0x00,0x04,0x00,0x04,
	0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0X02,
	0x01,0x00,0x80,0x25,0x00,0x00,0x08,0x00,0x00,0x04,0x00,0x04,
	0x02,0x00,0x80,0x25,0x00,0x00,0x08,0x00,0x00,0x04,0x00,0x04,
	};
const unsigned char Config_Get_F3_Other[19]={			
	64,0,4,18,0,4,1,1,1,1,1,31,12,3,0,0,0,3,0,
	};
const unsigned char Config_Get_F4_Para[10]={			
	0x09,0xFF,0xEF,0xFF,0x1F,0x79,0xFF,0x01,0x07,0x0F,
	};
const unsigned char Config_Get_F5_Ctrl[8]={			
	0x07,0x03,0xCF,0xCF,0xDF,0x7F,0x00,0x07,
	};	
const unsigned char Config_Get_F6_Data1[20]={			
	19,0xFE,0x03,0xFF,0x0F,0xFF,0xFF,0x00,0x00,0x00,
	0x00,0x0F,0xFF,0x3F,0xFF,0x0F,0x00,0xFF,0xFF,0xFF,
	};
const unsigned char Config_Get_F7_Data2[26]={			
	25,0xFF,0x0F,0xFF,0xBF,0x3F,0x03,0x3F,0x3F,0x03,
	0x0F,0xFF,0x3F,0xFF,0x0F,0x00,0x00,0x00,0x00,0x0F,
	0x00,0xFF,0xFF,0xFF,0xFF,0x0F,
	};	
const unsigned char Config_Get_F8_ERC[8]={			
	0xFF,0xCF,0xFD,0xFF,0x03,0x00,0x00,0x00,
	};	
//-------------    ��Լ�����һ�� �����  -----------------------//
//��վ������մ��������ucDataItemNo��
unsigned char Main_Recv_Manage(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo,unsigned char ucMSA,unsigned char ucSEQ)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char ucMsgType=0;
    unsigned char ucTmp[4];
    if(pstRecvTerMsg!=NULL)
    {
        ucMsgType=pstRecvTerMsg->ucMsgType;
        switch(ucMsgType)
        {
        case CON_TERMSG_ACK:            //ȷ�Ϸ���
            ucRet=Main_Recv_ACK(pstRecvTerMsg,ucDataItemNo,ucSEQ);
            break;
        case CON_TERMSG_RESET:          //��λ�������
            ucRet=Main_Recv_Reset(pstRecvTerMsg,ucDataItemNo);
            break;
        case CON_TERMSG_SETPARA:        //��������
            ucRet=Main_Recv_SetPara(pstRecvTerMsg,ucDataItemNo);
            break;
        case CON_TERMSG_LOGIN:          //�����֤����ԿЭ��
            ucRet=Main_Recv_Lonin(pstRecvTerMsg,ucDataItemNo);
            break;
        case CON_TERMSG_CONTROL:        //��������
            ucRet=Main_Recv_Control(pstRecvTerMsg,ucDataItemNo);
            break;
        case CON_TERMSG_DATAREQ_SLAVE:
            ucRet = Main_Recv_DataReq(pstRecvTerMsg,ucDataItemNo);
            break;
        case CON_TERMSG_CONFIGURATION:  //�����ն�����
            ucRet=Main_Recv_Configuration(pstRecvTerMsg,ucDataItemNo);
            break;
        case CON_TERMSG_GETPARA:        //��ѯ�ն˲���
            ucRet=Main_Recv_GetPara(pstRecvTerMsg,ucDataItemNo);
            break;
        case CON_TERMSG_TASKDATA:       //�������ݲ�ѯ
            ucRet=Main_Recv_ReqTaskData(pstRecvTerMsg,ucDataItemNo);
            break;
        case CON_TERMSG_REQDATA_1:      //����1�����ݣ�ʵʱ���ݣ�
            ucRet=Main_Recv_ReqData_1(pstRecvTerMsg,ucDataItemNo);
            break;
        case CON_TERMSG_REQDATA_2:      //����2�����ݣ��������ݣ�
            ucRet=Main_Recv_ReqData_2(pstRecvTerMsg,ucDataItemNo);
            break;
        case CON_TERMSG_REQDATA_3:      //����3�����ݣ��¼���
            ucRet=Main_Recv_ReqData_3(pstRecvTerMsg,ucDataItemNo);
            break;
        case CON_TERMSG_EXTCMD:         //��չ����
            ucRet=Main_Recv_ExtCmd(pstRecvTerMsg,ucDataItemNo);
            break;
        case CON_TERMSG_RELAYCMD:       //�м�վ����
            ucRet=Main_Recv_RelayCmd(pstRecvTerMsg,ucDataItemNo);
            break;
        case CON_TERMSG_FILETRANS:      //�ļ�����
            ucRet=Main_Recv_FileTrans(pstRecvTerMsg,ucDataItemNo);
            break;
        case CON_TERMSG_DATATRANS:      //����ת��
            ucRet=Main_Recv_DataTrans(pstRecvTerMsg,ucDataItemNo);
            break;
        default:
            ucRet=RET_ERROR;
            break;
        }
        if(ucMsgType==CON_TERMSG_SETPARA && ucRet==RET_SUCCESS)                 //��������澯
        {
            memset(ucTmp,0x00,4);
            Get_DA_by_MPNo(pstRecvTerMsg->stDataItem[ucDataItemNo].ucMPNo,&(ucTmp[0]),&(ucTmp[1]));
            Get_DT_by_ItemLabel(pstRecvTerMsg->ucMsgType,pstRecvTerMsg->stDataItem[ucDataItemNo].usItemLabel,&(ucTmp[2]),&(ucTmp[3]));
            Alarm_EnsureJudge_ParaModify_IDOnly(ucTmp,ucMSA);
        }
    }
    return ucRet;
}

//-------------    ��Լ����ڶ��� ���������  -----------------------//
//ȷ�Ϸ���
unsigned char   Main_Recv_ACK(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo,unsigned char ucSEQ)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char ucDAp;
    unsigned short usItemLabel;
//    unsigned short usPos=0;
    STDATAITEM *pstDataItem;
    pstDataItem=&pstRecvTerMsg->stDataItem[ucDataItemNo];
    ucDAp=pstDataItem->ucMPNo;
    usItemLabel=pstDataItem->usItemLabel;
    ucSEQ =( 0x0F & ucSEQ );
    switch(usItemLabel)     //Fn
    {
    case    CON_ACK_F1:     //F1    ȫ��ȷ��
        ucRet=Main_Recv_ACK_F1(pstDataItem->ucDataContent,ucDAp,ucSEQ);
        break;
    case    CON_ACK_F2:     //F2    ȫ������
        ucRet=Main_Recv_ACK_F2(pstDataItem->ucDataContent,ucDAp);
        break;
    case    CON_ACK_F3:     //F3    �����ݱ�ʶȷ��
        ucRet=Main_Recv_ACK_F3(pstDataItem->ucDataContent,ucDAp,ucSEQ);
        break;
    default:
        break;
    }
    return ucRet;
}

unsigned char   Main_Recv_ACK_F1(unsigned char *pucBuffer,unsigned char ucDAp,unsigned char ucSEQ)
{//ȫ��ȷ��
    unsigned char ucRet=RET_SUCCESS;

    if(Check_CurrentLoginSeq_IsOk()==RET_SUCCESS)
    {
        Main_RComm_SetStandBy_Time();
        CommConnect_SetNeedLoginFlag(CON_STATUS_NO);        //��¼��ȷ�ϣ�����Ҫ�ٵ�¼
    }
    else
        CommConnect_SetBeatHeartNoAckCount(0);                      //���������Ǹ��ۼƹ��̣��ﵽһ��������ͨѶ�͸�λ�����Կ��Բ��ж�seq
    Alarm_DownRecv_Manage(ucSEQ,CON_ALR_ACD);
    Alarm_DownRecv_Manage(ucSEQ,CON_ALR_ACD_NOT);

    return ucRet;
}

unsigned char   Main_Recv_ACK_F2(unsigned char *pucBuffer,unsigned char ucDAp)
{//ȫ������
    unsigned char ucRet=RET_SUCCESS;
    return ucRet;
}

//�յ�ȷ��֡�����ñ��Ϊȷ��״̬
//�յ�����֡�����ñ��Ϊ����״̬
//ĳЩȷ��֡��Ҫ���ͣ��Ƚ��鷳
unsigned char   Main_Recv_ACK_F3(unsigned char *pucBuffer,unsigned char ucDAp,unsigned char ucSEQ)
{//�����ݱ�ʶȷ��
    unsigned char ucRet=RET_SUCCESS;
    unsigned char ucAckMsgType;
    unsigned char ucResult;
    unsigned char ucAlrType=0;
    ucAckMsgType=pucBuffer[0] & 0x0F;
    ucResult=pucBuffer[5];
    switch(ucAckMsgType)
    {
    case CON_TERMSG_CONNECT:                        //��¼ȷ��
        if(pucBuffer[3]==0x01 && pucBuffer[4]==0x00)
        {//��¼
            Main_RComm_SetStandBy_Time();
            if(ucResult==CON_NACK_ERR_NORMAL)
            {
                if(Check_CurrentLoginSeq_IsOk()==RET_SUCCESS)
                {
                    CommConnect_SetNeedLoginFlag(CON_STATUS_NO);        //��¼��ȷ�ϣ�����Ҫ�ٵ�¼
                }
            }
        }else if(pucBuffer[3]==0x04 && pucBuffer[4]==0x00)
        {//����
            if(ucResult==CON_NACK_ERR_NORMAL)
            {
                CommConnect_SetBeatHeartNoAckCount(0);                      //���������Ǹ��ۼƹ��̣��ﵽһ��������ͨѶ�͸�λ�����Կ��Բ��ж�seq
            }
        }
        break;
    case CON_TERMSG_REQDATA_3:                      //�澯�������͵�ȷ��
//         Main_RComm_SetStandBy_Time();
         if(pucBuffer[3]==0x01 && pucBuffer[4]==0x00 &&  pucBuffer[5]==0x00 ) //��Ҫ�¼�
             ucAlrType=CON_ALR_ACD;
         else if(pucBuffer[3]==0x02 && pucBuffer[4]==0x00 && pucBuffer[5]==0x00 ) //һ���¼�
             ucAlrType=CON_ALR_ACD_NOT;
         Alarm_DownRecv_Manage(ucSEQ,ucAlrType);
         break;
    default:
        break;
    }

    return ucRet;
}

//��λ����
unsigned char Main_Recv_Reset(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo)
{
    unsigned char ucRet=RET_ERROR;
    unsigned short usItemLabel;
    STDATAITEM *pstDataItem;

    pstDataItem=&pstRecvTerMsg->stDataItem[ucDataItemNo];
    usItemLabel=pstDataItem->usItemLabel;
    if(pstDataItem->ucExeRet!=CON_NACK_ERR_PASSWORD)
    {
        switch(usItemLabel)     //Fn
        {
        case CON_RST_F1_HARD:       //Ӳ����ʼ��
            ucRet=Main_Recv_Reset_Hardware(pstRecvTerMsg->ucMsgSource);
            break;
        case CON_RST_F2_DATA:       //��������ʼ������Ӳ����ʼ��
            ucRet=Main_Recv_Reset_Data(pstRecvTerMsg->ucMsgSource);
            break;
        case CON_RST_F3_ALL:        //������ȫ����������ʼ������Ӳ����ʼ��(azh ͨ�Ų���Ҳ�ǲ���� Ϊ�˰�ȫ)
            ucRet=Main_Recv_Reset_All(pstRecvTerMsg->ucMsgSource,CON_PARA_INIT_ALL_CLR);//0x55);
            break;
        case CON_RST_F4_EXCECOMM:   //��ͨѶ����֮������ݳ�ʼ��
            ucRet=Main_Recv_Reset_All(pstRecvTerMsg->ucMsgSource,CON_PARA_INIT_ALL_CLR);//0xAA);
            break;
        default:
            break;
        }
        if(ucRet==RET_ERROR)
            pstDataItem->ucExeRet=CON_NACK_ERR_NOTEXE;
        else
            pstDataItem->ucExeRet=CON_NACK_ERR_NORMAL;
    }

    return ucRet;
}

//�������ô������
unsigned char Main_Recv_SetPara(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo)
{
    U8 ucRet=RET_ERROR;
    U8 *pucBuffer=NULL;
    U8 ucDAp, mem_type;
    U16 usItemLabel, crc16;
    STDATAITEM *pstDataItem;
    U32 obj_addr;
        U8 *pret_mem = NULL;

    obj_addr = 0;
    pstDataItem=&pstRecvTerMsg->stDataItem[ucDataItemNo];
    pucBuffer=pstDataItem->ucDataContent;
    ucDAp=pstDataItem->ucMPNo;
    usItemLabel=pstDataItem->usItemLabel;
    if((pstRecvTerMsg->ucMsgSource==CON_TERMSG_SOUR_RCOMM_SMS)
        && (RunPara_GetTerCommPara_RCommMode()==CON_ONLINE_SMS_UP)
        && (usItemLabel!=CON_PARA_F62_COMMTYPE))
    {//����ģʽ�¶��ź�ֻ��F62
        pstDataItem->ucExeRet=CON_NACK_ERR_NOTEXE;
    }
    else if(pstDataItem->ucExeRet!=CON_NACK_ERR_PASSWORD)
    {
        switch(usItemLabel)
        {
        case CON_PARA_F1_COMMPARA:      //F1�ն�ͨ�Ų���
            ucRet=Main_Recv_Set_F1_CommPara(pucBuffer,ucDAp);
            pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
            obj_addr = F1_TERMINAL_COM_PARA_ADDR;
            break;
        case CON_PARA_F2_RELAYPARA:
            ucRet = Main_Recv_Set_F2_RelayPara(pucBuffer,ucDAp);
            break;
        case CON_PARA_F3_STAIPADDR:     //F3��վIP��ַ�Ͷ˿�
            ucRet=Main_Recv_Set_F3_IP_Addr(pucBuffer,ucDAp);
            pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
            obj_addr = F3_STAIPADDR_ADDR;
            break;
        case CON_PARA_F4_STAPHONENO:    //F4��վ�绰���룬�������ĺ���
            ucRet=Main_Recv_Set_F4_Station_Number(pucBuffer,ucDAp);
            pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
            obj_addr = F4_STAPHONENO_ADDR;
            break;
        case CON_PARA_F5_TERENCRYPT:    //F5��������
            ucRet=Main_Recv_Set_F5_TerEncrypt(pucBuffer,ucDAp);
            pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
            break;
        case CON_PARA_F6_TERGROUPADDR:  //F6���ն����ַ����
            ucRet=Main_Recv_Set_F6_TerGroupAddr(pucBuffer,ucDAp);
            pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
            break;
        case CON_PARA_F7_IPADDR:        //F7���ն�IP��ַ�Ͷ˿�
            ucRet=Main_Recv_Set_F7_IPAddr(pucBuffer,ucDAp);
            pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
            break;
        case CON_PARA_F8_ALRCTLWORD:    //F8���ն�����ͨ�Ź�����ʽ
            ucRet=Main_Recv_Set_F62_CommType(pucBuffer,ucDAp);
            pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
            obj_addr = F8_TERUPCOMWORKING_ADDR;
            break;
        case CON_PARA_F9_CONFIGNUM:     //F9���ն��¼���¼����
            ucRet=Main_Recv_Set_F9_AlarmControlWord(pucBuffer,ucDAp);
            pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
            obj_addr = F9_EVENTRECORDCONFSET_ADDR;
            break;
        case CON_PARA_F10_METERPARA:    //F10���ն˵��ܱ�/��������װ�����ò���
            ucRet=Main_Recv_Set_F10_MeterPara(pucBuffer,ucDAp);
            pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
            break;
//      case CON_PARA_F11_PULSEPARA:    //F11���ն��������ò���
//          ucRet=Main_Recv_Set_F11_PulsePara(pucBuffer,ucDAp);
//          pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
//          break;
//      case CON_PARA_F12_DINPUTPARA:   //F12���ն�״̬���������
//          ucRet=Main_Recv_Set_F12_DInputPara(pucBuffer,ucDAp);
//          pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
//          break;
//      case CON_PARA_F13_ANALOGPARA:   //F13���ն˵�ѹ/����ģ�������ò���
//          ucRet=Main_Recv_Set_F13_AnalogPara(pucBuffer,ucDAp);
//          pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
//          break;
//      case CON_PARA_F14_SUMGPARA:     //F14���ն��ܼ������ò���
//          ucRet=Main_Recv_Set_F14_SumGPara(pucBuffer,ucDAp);
//          pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
//          break;
//      case CON_PARA_F15_POWVALDIFF:   //F15���й��ܵ������Խ���¼���������
//          ucRet=Main_Recv_Set_F15_PowValDiff(pucBuffer,ucDAp);
//          pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
//          break;
//        case CON_PARA_F16_PDPINFO:      //F16������ר���û���������
//            ucRet=Main_Recv_Set_F16_PDPInfo(pucBuffer,ucDAp);
//            pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
//            break;
//      case CON_PARA_F17_KEEPPOWER:    //F17���ն˱�����ֵ
//          ucRet=Main_Recv_Set_F17_KeepPower(pucBuffer,ucDAp);
//          pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
//          break;
//      case CON_PARA_F18_POWCTRLTIMEPER:   //F18���ն˹���ʱ��
//          ucRet=Main_Recv_Set_F18_PowCtrlTimePer(pucBuffer,ucDAp);
//          pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
//          break;
//      case CON_PARA_F19_POWCTRLCOEF:  //F19���ն�ʱ�ι��ض�ֵ����ϵ��
//          ucRet=Main_Recv_Set_F19_PowCtrlCoef(pucBuffer,ucDAp);
//          pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
//          break;
//      case CON_PARA_F20_ENERCTRLCOEF: //F20���ն��µ������ض�ֵ����ϵ��
//          ucRet=Main_Recv_Set_F20_EnerCtrlCoef(pucBuffer,ucDAp);
//          pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
//          break;
//      case CON_PARA_F21_PERTIMEFEE:   //F21���ն˵���������ʱ�κͷ�����
//          ucRet=Main_Recv_Set_F21_PerTimeFee(pucBuffer,ucDAp);
//          pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
//          break;
//      case CON_PARA_F22_FEEPARA:      //F22���ն˵���������
//          ucRet=Main_Recv_Set_F22_FeePara(pucBuffer,ucDAp);
//          pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
//          break;
//      case CON_PARA_F23_URGEALARMPARA://F23���ն˴߷Ѹ澯����
//          ucRet=Main_Recv_Set_F23_UrgeAlarmPara(pucBuffer,ucDAp);
//          pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
//          break;
//        case CON_PARA_F24_COPYINTER:    //F24���ն˳���������
//          ucRet=Main_Recv_Set_F24_CopyInter(pucBuffer,ucDAp);
//            pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
//            break;
        case CON_PARA_F25_MPBASEPARA:   //F25���������������
            ucRet=Main_Recv_Set_F25_MPBasePara(pucBuffer,ucDAp);
            obj_addr = F25_MPBASEPARA_ADDR;
            break;
        case CON_PARA_F26_MPLIMIT:      //F26����������ֵ����
            ucRet=Main_Recv_Set_F26_MPLimit(pucBuffer,ucDAp);
            break;
        case CON_PARA_F27_MPFREEZEPARA: //F27��������ͭ���������
            ucRet=Main_Recv_Set_F27_MPCuFePara(pucBuffer,ucDAp);
            break;
        case CON_PARA_F28_MPPOWRATEPARA://F28�������㹦�������ֶ���ֵ
            ucRet=Main_Recv_Set_F28_MPPowRatePara(pucBuffer,ucDAp);
            break;
//        case CON_PARA_F29_METERADDR:    //F29:
//            ucRet=Main_Recv_Set_F29_Ter_Meter_addr(pucBuffer,ucDAp);
//            break;
         case CON_PARA_F30_METERCOPYFLAG:
            ucRet = Main_Recv_Set_F30_CopyFlag(pucBuffer,ucDAp);
            break;
      case CON_PARA_F33_SUMGFREEZEPARA://F33���ն˳������в���
          ucRet=Main_Recv_Set_F33_SumGFreezePara(pucBuffer,ucDAp);		//�����ڲ��浽EEPROM��
          break;
        case CON_PARA_F34_PORTPROPERTY: //F34
            ucRet = Main_Recv_Set_F34_PortProperty(pucBuffer,ucDAp);
            break;
//        case CON_PARA_F35_EMPHAFLAG:
//            ucRet = Main_Recv_Set_F35_EmphaFlag(pucBuffer,ucDAp);
//            break;
//        case CON_PARA_F37_LINKPARA:
//            ucRet = Main_Recv_Set_F37_LinkPara(pucBuffer,ucDAp);
//            pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
//            break;
        case CON_PARA_F38_JOB1PARA:
            ucRet = Main_Recv_Set_F38_Class(pucBuffer,ucDAp,0);
            pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
            obj_addr = F38_JOB1PARA_ADDR;
            break;
        case CON_PARA_F39_JOB2PARA:
            ucRet = Main_Recv_Set_F38_Class(pucBuffer,ucDAp,1);
            pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
            obj_addr = F39_JOB2PARA_ADDR;
            break;
//      case CON_PARA_F41_TIMEPERSETTING:   //F41��ʱ�ι��ض�ֵ
//          ucRet=Main_Recv_Set_F41_TimePerSetting(pucBuffer,ucDAp);
//          break;
//      case CON_PARA_F42_RESTCTRL:         //F42�����ݹ��ز���
//          ucRet=Main_Recv_Set_F42_RestCtrl(pucBuffer,ucDAp);
//          break;
//      case CON_PARA_F43_POWERSLIP:        //F43�����ʿ��ƵĹ��ʼ��㻬��ʱ��
//          ucRet=Main_Recv_Set_F43_PowerSlip(pucBuffer,ucDAp);
//          break;
//      case CON_PARA_F44_SALESTOP:         //F44��Ӫҵ��ͣ�ز���
//          ucRet=Main_Recv_Set_F44_SaleStop(pucBuffer,ucDAp);
//          break;
//      case CON_PARA_F45_POWERCTRLTURN:    //F45�������ִ��趨
//          ucRet=Main_Recv_Set_F45_PowerCtrlTrun(pucBuffer,ucDAp);
//          break;
//      case CON_PARA_F46_MONTHENERCTRL:    //F46���µ����ض�ֵ
//          ucRet=Main_Recv_Set_F46_MonthEnerCtrl(pucBuffer,ucDAp);
//          break;
//      case CON_PARA_F47_BUYENERCTRL:      //F47���������ز���
//          ucRet=Main_Recv_Set_F47_BuyEnerCtrl(pucBuffer,ucDAp);
//          break;
//      case CON_PARA_F48_ENERCTRLTURN:     //F48������ִ��趨
//          ucRet=Main_Recv_Set_F48_EnerCtrlTurn(pucBuffer,ucDAp);
//          break;
//      case CON_PARA_F49_CTRLALRTIME:      //F49�����ظ澯ʱ��
//          ucRet=Main_Recv_Set_F49_CtrlAlrTime(pucBuffer,ucDAp);
//          break;
        case CON_PARA_F57_SOUNDALARM:       //F57���ն������澯����M��ֹ����
            ucRet=Main_Recv_Set_F57_SoundAlarm(pucBuffer,ucDAp);
            pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
            break;
//      case CON_PARA_F58_KEEPAUTO:         //F58���ն��Զ��������
//          ucRet=Main_Recv_Set_F58_KeepAuto(pucBuffer,ucDAp);
//          pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
//          break;
        case CON_PARA_F59_MTRERRORPARA:     //F59�����ܱ��쳣�б���ֵ�趨
            ucRet=Main_Recv_Set_F59_MtErrorPara(pucBuffer,ucDAp);
            pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
            break;
//      case CON_PARA_F60_HARMONPARA:
//          ucRet = Main_Recv_Set_F60_HarmLimit(pucBuffer,ucDAp);
//          pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
//          break;
//      case CON_PARA_F61_DCAINPARA:
//          ucRet = Main_Recv_Set_F61_Aanalog(pucBuffer,ucDAp);
//          pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
//          break;
        case CON_PARA_F62_COMMTYPE:     //F62 ����ר��������ʽ
            ucRet=Main_Recv_Set_F62_CommType(pucBuffer,ucDAp);
            pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
            break;
        case CON_PARA_F63_FLUXLIMIT:        //F36 ��ͨ��������������
            ucRet=Main_Recv_Set_F63_FluxLimit(pucBuffer,ucDAp);
            pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
            break;
        case CON_PARA_F65_UPDATA1PARA:      //F65����ʱ����1��������������
            ucRet=Main_Recv_Set_F65_UpData1Para(pucBuffer,ucDAp);
            if(ucRet == RET_SUCCESS)
            {
                obj_addr = F65_UPDATA1PARA_ADDR + (F65_UPDATA1PARA_LEN + 2) * ucDAp;
            }
            break;
        case CON_PARA_F66_UPDATA2PARA:      //F66����ʱ����2��������������
            ucRet=Main_Recv_Set_F66_UpData2Para(pucBuffer,ucDAp);
            if(ucRet == RET_SUCCESS)
            {
                obj_addr = F66_UPDATA2PARA_ADDR + (F66_UPDATA2PARA_LEN + 2) * ucDAp;
            }
            break;
        case CON_PARA_F67_UPDATA1CTRL:      //F67����ʱ����1��������������/ֹͣ����
            ucRet=Main_Recv_Set_F67_UpData1Ctrl(pucBuffer,ucDAp);//eeprom�Ѵ���
//             if(ucRet == RET_SUCCESS)
//             {
//                 obj_addr = F67_UPDATA1CTRL_ADDR + ucDAp * 3;
//             }
            break;
        case CON_PARA_F68_UPDATA2CTRL:      //F68����ʱ����2��������������/ֹͣ����
            ucRet=Main_Recv_Set_F68_UpData2Ctrl(pucBuffer,ucDAp);//eeprom�Ѵ���
//             if(ucRet == RET_SUCCESS)
//             {
//                 obj_addr = F68_UPDATA2CTRL_ADDR + ucDAp * 3;
//             }
            break;
//      case CON_PARA_F73_CAPPARA:
//      case CON_PARA_F74_CAPRUNPARA:
//      case CON_PARA_F75_CAPRELAYPARA:
//      case CON_PARA_F76_CAPCTRL:
//          ucRet=Main_Recv_Set_F73_F76(pucBuffer,ucDAp,usItemLabel);
//          break;
//      case CON_PARA_F81_DCARATE:
//          ucRet = Main_Recv_Set_F81_DCRate(pucBuffer,ucDAp);
//          break;
//      case CON_PARA_F82_DCALIMIT:
//          ucRet = Main_Recv_Set_F82_DCLimit(pucBuffer,ucDAp);
//          break;
//      case CON_PARA_F83_DCAFREESE:
//          ucRet = Main_Recv_Set_F83_M(pucBuffer,ucDAp);
//          break;

//azh 170919
        case CON_PARA_F90_WIRELESS:      //F90������ͨ�Ų���
            ucRet=Main_Recv_Set_F90_Wireless(pucBuffer,ucDAp);
            pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
            break;
        case CON_PARA_F91_LOCATION:      //F91���ն˵���λ��
            ucRet=Main_Recv_Set_F91_Location(pucBuffer,ucDAp);
            pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
            break;
        case CON_PARA_F150_SNSTATE:      //F150��������״̬
            ucRet=Main_Recv_Set_F150_Snstate(pucBuffer,ucDAp);
            pstDataItem->ucMPNo=CON_MPNO_TER_FIXED;
            break;
//

        case CON_PARA_F89_ADDR:
        case CON_PARA_F207_ADDR:
            ucRet=Main_Recv_Set_F207_Addr(pucBuffer,ucDAp);
            obj_addr = F207_ADDR_ADDR;
            break;
        default:
            ucRet=RET_ERROR;
            break;
        }
        if(ucRet==RET_ERROR)
            pstDataItem->ucExeRet=CON_NACK_ERR_NOTEXE;
        else if(ucRet==RET_SUCCESS)
        {
            pstDataItem->ucExeRet=CON_NACK_ERR_NORMAL;
            if(obj_addr)
            {
                            pret_mem = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR0);
                            if(pret_mem != NULL)
                            {
                                memcpy(pret_mem, pucBuffer, pstDataItem->usDataLen);
                                crc16 = calc_crc16(pret_mem, pstDataItem->usDataLen, 0xffff);      //�������ݵ�CRC
                                pret_mem[pstDataItem->usDataLen] = (crc16 >> 8);
                                pret_mem[pstDataItem->usDataLen + 1] = crc16;

                    file_write(obj_addr, pret_mem, (pstDataItem->usDataLen) + 2);
                    if(obj_addr < F65_UPDATA1PARA_ADDR)
                    {
                        obj_addr += (pstDataItem->usDataLen) + 2;
                        file_write(obj_addr, pret_mem, (pstDataItem->usDataLen) + 2);
                    }
                                free_mem_block(pret_mem);//MEM_256_TYPE
                            }

            }
        }
        if(ucRet==RET_SUCCESS)
        {
//          Beep_Start(CON_BEEP_TOTAL_PARACHG,
//          CON_BEEP_DELAY_PARACHG,
//          CON_BEEP_LAST_PARACHG,
//          CON_BEEP_INTER_PARACHG);                //������ز����޸���Ҫ����澯��Ҫ������ʾ
//          ucsave_flag = 20;
        }
    }

    return ucRet;
}



//�����ն����ô������
unsigned char Main_Recv_Configuration(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo)
{
    unsigned char ucRet=RET_ERROR;
//  unsigned char *pucBuffer=NULL;
    unsigned char ucDAp;
    unsigned short usItemLabel;
    STDATAITEM *pstDataItem;

    pstDataItem=&pstRecvTerMsg->stDataItem[ucDataItemNo];
//  pucBuffer=pstDataItem->ucDataContent;
    ucDAp=pstDataItem->ucMPNo;
    usItemLabel=pstDataItem->usItemLabel;


    switch(usItemLabel)
    {
    case CON_CONFIG_F1_VERSION:     //F1�ն˰汾��Ϣ
        ucRet=Main_Recv_Config_Get_F1_Version(pstDataItem,ucDAp);
        break;
    case CON_CONFIG_F2_INTERFACE:   //F2���롢�����ͨ�Ŷ˿�����
        ucRet=Main_Recv_Config_Get_F2_Interface(pstDataItem,ucDAp);
        break;
    case CON_CONFIG_F3_OTHER:       //F3��������
        ucRet=Main_Recv_Config_Get_F3_Other(pstDataItem,ucDAp);
        break;
    case CON_CONFIG_F4_PARA:        //F4�������ã����ն���Ӳ���汾֧�ֵĲ�����
        ucRet=Main_Recv_Config_Get_F4_Para(pstDataItem,ucDAp);
        break;
    case CON_CONFIG_F5_CTRL:        //F5�������ã����ն���Ӳ���汾֧�ֵĿ������
        ucRet=Main_Recv_Config_Get_F5_Ctrl(pstDataItem,ucDAp);
        break;
    case CON_CONFIG_F6_DATA1:       //F6��1���������ã����ն���Ӳ���汾֧�ֵ�1�����ݣ�
        ucRet=Main_Recv_Config_Get_F6_Data1(pstDataItem,ucDAp);
        break;
    case CON_CONFIG_F7_DATA2:       //F7��2���������ã����ն���Ӳ���汾֧�ֵ�2�����ݣ�
        ucRet=Main_Recv_Config_Get_F7_Data2(pstDataItem,ucDAp);
        break;
    case CON_CONFIG_F8_ERC:         //F8���¼���¼���ã����ն���Ӳ���汾֧�ֵ��¼���¼��
        ucRet=Main_Recv_Config_Get_F8_ERC(pstDataItem,ucDAp);
        break;
//azh 170920
    case CON_CONFIG_F9_REMOTEVER:         //F9Զ��ͨ��ģ��汾��Ϣ
        ucRet=Main_Recv_Config_Get_F9_REMOTEVER(pstDataItem,ucDAp);
        break;
    case CON_CONFIG_F10_LOCALVER:         //F10����ͨ��ģ��汾��Ϣ
        ucRet=Main_Recv_Config_Get_F10_LOCALVER(pstDataItem,ucDAp);
        break;
    default:
        break;
    }
    if(ucRet==RET_ERROR)
        pstDataItem->ucExeRet=CON_NACK_ERR_NOTEXE;
    else if(ucRet==RET_SUCCESS)
        pstDataItem->ucExeRet=CON_NACK_ERR_NORMAL;
//  if(ucRet==RET_SUCCESS)  //delete by cfh 10-03-31��ȡ�ն����ò���Ҫ���������
//  {
//      Beep_Start(CON_BEEP_TOTAL_PARACHG,
//      CON_BEEP_DELAY_PARACHG,
//      CON_BEEP_LAST_PARACHG,
//      CON_BEEP_INTER_PARACHG);                //������ز����޸���Ҫ����澯��Ҫ������ʾ
//      ucsave_flag = 1;
//  }

    return ucRet;
}


//�����֤����ԿЭ��
unsigned char Main_Recv_Lonin(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo)
{
    unsigned char ucRet=RET_ERROR;
//  unsigned char *pucBuffer=NULL;
    unsigned char ucDAp;
    unsigned short usItemLabel;
    STDATAITEM *pstDataItem;

    pstDataItem=&pstRecvTerMsg->stDataItem[ucDataItemNo];
//  pucBuffer=pstDataItem->ucDataContent;
    ucDAp=pstDataItem->ucMPNo;
    usItemLabel=pstDataItem->usItemLabel;


    switch(usItemLabel)
    {
    case CON_LOGIN_F1_REQUEST:
        ucRet=Main_Recv_Login_Set_F1_Password(pstDataItem,ucDAp);
        break;
    case CON_LOGIN_F2_ANSWER:
        ucRet=Main_Recv_Login_Get_F2_Password(pstDataItem,ucDAp);
        break;
    default:
        break;
    }
    if(ucRet==RET_ERROR)
        pstDataItem->ucExeRet=CON_NACK_ERR_NOTEXE;
    else if(ucRet==RET_SUCCESS)
        pstDataItem->ucExeRet=CON_NACK_ERR_NORMAL;
    if(ucRet==RET_SUCCESS)
    {
//      Beep_Start(CON_BEEP_TOTAL_PARACHG,
//      CON_BEEP_DELAY_PARACHG,
//      CON_BEEP_LAST_PARACHG,
//      CON_BEEP_INTER_PARACHG);                //������ز����޸���Ҫ����澯��Ҫ������ʾ
//      ucsave_flag = 20;
    }

    return ucRet;
}

//��������
unsigned char   Main_Recv_Control(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo)
{
    unsigned char ucRet=RET_ERROR;
    unsigned char *pucBuffer=NULL;
    unsigned char ucDAp;
//  unsigned char ucNeedAlarm=CON_STATUS_YES;
    unsigned short usItemLabel;
    STDATAITEM *pstDataItem;

    pstDataItem=&pstRecvTerMsg->stDataItem[ucDataItemNo];
    pucBuffer=pstDataItem->ucDataContent;
    ucDAp=pstDataItem->ucMPNo;
    usItemLabel=pstDataItem->usItemLabel;

    if((pstRecvTerMsg->ucMsgSource==CON_TERMSG_SOUR_RCOMM_SMS)
        //&& (RunPara_GetTerCommPara_RCommMode()==CON_ONLINE_SMS_UP)
        && (usItemLabel!=CON_CTRL_F38_SMS_ACTIVE))
    {//����ģʽ�¶��ź�ֻ��F62
        pstDataItem->ucExeRet=CON_NACK_ERR_NOTEXE;
    }
    else if(pstDataItem->ucExeRet!=CON_NACK_ERR_PASSWORD)
    {
        switch(usItemLabel)
        {
//        case CON_CTRL_F1_YK_OPEN:           //F1 ң����բ
//            ucRet=Main_Recv_Ctrl_F1_YK_Open(pucBuffer,ucDAp);
//            break;
//        case CON_CTRL_F2_YK_CLOSE:          //F2 ң�������բ
//            ucRet=Main_Recv_Ctrl_F2_YK_Close(pucBuffer,ucDAp);
//            break;
//        case CON_CTRL_F9_TIMEPER_ON:        //F9ʱ�ι���Ͷ��
//            ucRet=Main_Recv_Ctrl_F9_TimePer_On(pucBuffer,ucDAp);
//            break;
//        case CON_CTRL_F10_RESTCTRL_ON:      //F10���ݹ���Ͷ��
//            ucRet=Main_Recv_Ctrl_F10_RestCtrl_On(pucBuffer,ucDAp);
//            break;
//        case CON_CTRL_F11_SALESTOP_ON:      //F11Ӫҵ��ͣ����Ͷ��
//            ucRet=Main_Recv_Ctrl_F11_SaleStop_On(pucBuffer,ucDAp);
//            break;
//        case CON_CTRL_F12_POWERLOWER_ON:    //F12��ǰ�����¸���Ͷ��
//            ucRet=Main_Recv_Ctrl_F12_PowerLower_On(pucBuffer,ucDAp);
//            break;
//        case CON_CTRL_F15_MONTHENER_ON:     //F15�µ��Ͷ��
//            ucRet=Main_Recv_Ctrl_F15_MonthEner_On(pucBuffer,ucDAp);
//            break;
//        case CON_CTRL_F16_BUYENER_ON:       //F16�����Ͷ��
//            ucRet=Main_Recv_Ctrl_F16_BuyEner_On(pucBuffer,ucDAp);
//            break;
//        case CON_CTRL_F17_TIMEPER_OFF:      //F17ʱ�ι��ؽ��
//            ucRet=Main_Recv_Ctrl_F17_TimePer_Off(pucBuffer,ucDAp);
//            break;
//        case CON_CTRL_F18_RESTCTRL_OFF:     //F18���ݹ��ؽ��
//            ucRet=Main_Recv_Ctrl_F18_RestCtrl_Off(pucBuffer,ucDAp);
//            break;
//        case CON_CTRL_F19_SALESTOP_OFF:     //F19Ӫҵ��ͣ���ؽ��
//            ucRet=Main_Recv_Ctrl_F19_SaleStop_Off(pucBuffer,ucDAp);
//            break;
//        case CON_CTRL_F20_POWERLOWER_OFF:   //F20��ǰ�����¸��ؽ��
//            ucRet=Main_Recv_Ctrl_F20_PowerLower_Off(pucBuffer,ucDAp);
//            break;
//        case CON_CTRL_F23_MONTHENER_OFF:    //F23�µ�ؽ��
//            ucRet=Main_Recv_Ctrl_F23_MonthEner_Off(pucBuffer,ucDAp);
//            break;
//        case CON_CTRL_F24_BUYENER_OFF:      //F24����ؽ��
//            ucRet=Main_Recv_Ctrl_F24_BuyEner_Off(pucBuffer,ucDAp);
//            break;
//        case CON_CTRL_F25_KEEPPOWER_ON:     //F25�ն˱���Ͷ��
//            ucRet=Main_Recv_Ctrl_F25_KeepPower_On(pucBuffer,ucDAp);
//            break;
//        case CON_CTRL_F26_URGEALARM_ON:     //F26�߷Ѹ澯Ͷ��
//            ucRet=Main_Recv_Ctrl_F26_UrgeAlarm_On(pucBuffer,ucDAp);
//            break;
        case CON_CTRL_F27_TALK_ON:          //F27�����ն�����վͨ��
            ucRet=Main_Recv_Ctrl_F27_Talk_On(pucBuffer,ucDAp);
            break;
//        case CON_CTRL_F28_REMOVE_ON:        //F28�ն��޳�Ͷ��
//            ucRet=Main_Recv_Ctrl_F28_Remove_On(pucBuffer,ucDAp);
//            break;
        case CON_CTRL_F29_UPACTIVE_ON:      //F29�����ն������ϱ�
            ucRet=Main_Recv_Ctrl_F29_UpActive_On(pucBuffer,ucDAp);
            break;
//azh 170920
        case CON_CTRL_F30_SETMETERTIME_SWITCH:      //F29�����ն������ϱ�
            ucRet=Main_Recv_Ctrl_F30_SetMeterTime_Switch(pucBuffer,ucDAp);
            break;

        case CON_CTRL_F31_SETTIME:          //F31 ��ʱ����
            ucRet=Main_Recv_Ctrl_F31_SetTerTime(pucBuffer,ucDAp);
//azh           ucNeedAlarm=CON_STATUS_NO;
            break;
        case CON_CTRL_F32_MESSAGE:          //F32������Ϣ
//          ucRet=Main_Recv_Ctrl_F32_Message(pucBuffer,ucDAp);
            break;
//        case CON_CTRL_F33_KEEPPOWER_OFF:    //F33�ն˱�����
//            ucRet=Main_Recv_Ctrl_F33_KeepPower_Off(pucBuffer,ucDAp);
//            break;
//        case CON_CTRL_F34_URGEFEE_OFF:      //F34�߷Ѹ澯���
//            ucRet=Main_Recv_Ctrl_F34_UrgeAlarm_Off(pucBuffer,ucDAp);
//            break;
        case CON_CTRL_F35_TALK_OFF:         //F35��ֹ�ն�����վͨ��
            ucRet=Main_Recv_Ctrl_F35_Talk_Off(pucBuffer,ucDAp);
            break;
//        case CON_CTRL_F36_REMOVE_OFF:       //F36�ն��޳����
//            ucRet=Main_Recv_Ctrl_F36_Remove_Off(pucBuffer,ucDAp);
//            break;
        case CON_CTRL_F37_UPACTIVE_OFF:     //F37��ֹ�ն������ϱ�
            ucRet=Main_Recv_Ctrl_F37_UpActive_Off(pucBuffer,ucDAp);
            break;
        case CON_CTRL_F38_SMS_ACTIVE:       //F38���ż���
            ucRet=Main_Recv_Ctrl_F38_SMS_Active(pucBuffer,ucDAp,pstRecvTerMsg->ucMsgSource);
            break;
        case CON_CTRL_F39_DISCONECT:        //F38���ż���
            ucRet=Main_Recv_Ctrl_F39_Disconect(pucBuffer,ucDAp,pstRecvTerMsg->ucMsgSource);
            break;
        case CON_CTRL_F49:
        case CON_CTRL_F50:
        case CON_CTRL_F51:
            ucRet = RET_SUCCESS;
            break;
//azh 170920
        case CON_CTRL_F149_METERSELF_SWITCH:         //F149���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
            ucRet=Main_Recv_Ctrl_F149_MeterSelf_Switch(pucBuffer,ucDAp);
            break;
        case CON_CTRL_F150_RESETSNSTATE_SWITCH:      //F150��λ��������������־
            ucRet=Main_Recv_Ctrl_F150_ResetSnState_Switch(pucBuffer,ucDAp);
            break;
        default:
            break;
        }
        if(ucRet==RET_ERROR)
            pstDataItem->ucExeRet=CON_NACK_ERR_NOTEXE;
        else if(ucRet==RET_SUCCESS)
        {
//azh           ucsave_flag = 20;
//          if(ucNeedAlarm==CON_STATUS_YES)
//              Beep_Start(CON_BEEP_TOTAL_EXECTRL,
//              CON_BEEP_DELAY_EXECTRL,
//              CON_BEEP_LAST_EXECTRL,
//              CON_BEEP_INTER_EXECTRL);                //�����·�����Ҫ����澯��Ҫ������ʾ
            pstDataItem->ucExeRet=CON_NACK_ERR_NORMAL;
        }
    }

    return ucRet;
}
unsigned  char Main_Recv_DataReq(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo)
{
    unsigned char ucRet=RET_SUCCESS;
//  unsigned short usItemLabel,usPos=0;
//  STDATAITEM *pstDataItem;
//  STLINKSENDWAITTASKMANAGE stLinkWaitMng;
//
//  pstDataItem=&pstRecvTerMsg->stDataItem[ucDataItemNo];
//
//  if(Link_IsEmpty_SendWaitList()== RET_TASK_EMPTY)
//      pstDataItem->ucExeRet = CON_NACK_ERR_NOTEXE;
//  else
//  {
//      stLinkWaitMng.ucUpSendFlag = CON_STATUS_YES;
//      stLinkWaitMng.ucWaitTime = 0xFF;
//      LinkComm_SetWaitManage(&stLinkWaitMng);
//      pstDataItem->ucExeRet = CON_NACK_ERR_NORMAL;
//  }
    return ucRet;
}
//�������ݲ�ѯ
unsigned char   Main_Recv_ReqTaskData(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char ucDAp;
    unsigned short usItemLabel;
    STDATAITEM *pstDataItem;

    pstDataItem=&pstRecvTerMsg->stDataItem[ucDataItemNo];
    ucDAp=pstDataItem->ucMPNo;
    usItemLabel=pstDataItem->usItemLabel;

    switch(usItemLabel)     //Fn
    {
    case    CON_TASKDATA_REQ_F1:    //F1    1�����������ѯ
        ucRet=Main_Recv_ReqTaskData_F1(pstDataItem,ucDAp);
        break;
    case    CON_TASKDATA_REQ_F2:    //F2    2�����������ѯ
        ucRet=Main_Recv_ReqTaskData_F2(pstDataItem,ucDAp);
        break;
    default:
        break;
    }
    if(ucRet==RET_ERROR)
        pstDataItem->ucExeRet=CON_NACK_ERR_NOTEXE;
    else
        pstDataItem->ucExeRet=CON_NACK_ERR_NORMAL;
    return ucRet;
}

//����1�����ݣ�ʵʱ���ݣ�
unsigned char   Main_Recv_ReqData_1(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo)
{
    unsigned char ucRet=RET_ERROR;//ucRet=RET_SUCCESS;
    unsigned char ucDAp;
    unsigned short usItemLabel,usPos=0;
    STDATAITEM *pstDataItem;

    pstDataItem=&pstRecvTerMsg->stDataItem[ucDataItemNo];
    ucDAp=pstDataItem->ucMPNo;
    usItemLabel=pstDataItem->usItemLabel;

    switch(usItemLabel)     //Fn
    {
    case    CON_DATA_REQ1_F1:   //F1    �ն˰汾��Ϣ
        ucRet=Main_Get_ReqData_1_F1(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F2:   //F2    �ն�����ʱ��
        ucRet=Main_Get_ReqData_1_F2(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F3:   //F3    �ն˲���״̬
        ucRet=Main_Get_ReqData_1_F3(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F4:   //F4    �ն�ͨ��״̬
        ucRet=Main_Get_ReqData_1_F4(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F5:   //F5    �ն˿�������״̬
        ucRet=Main_Get_ReqData_1_F5(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F6:   //F6    �ն˵�ǰ����״̬
        ucRet=Main_Get_ReqData_1_F6(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F7:   //F7    �ն��¼���������ǰֵ
        ucRet=Main_Get_ReqData_1_F7(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F8:   //F8    �ն��¼���־״̬
        ucRet=Main_Get_ReqData_1_F8(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F9:   //F9    �ն�״̬������λ��־
        ucRet=Main_Get_ReqData_1_F9(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F10:  //F10   �ն�����վ���ա���ͨ������
        ucRet=Main_Get_ReqData_1_F10(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
//azh 170921
    case    CON_DATA_REQ1_F12:  //F12   ���������·���ؽ���״̬������λ��־
        ucRet=Main_Get_ReqData_1_F12(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F13:  //F13   �������ĵ����Ϣ
        ucRet=Main_Get_ReqData_1_F13(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F14:  //F14   �ļ�����δ�յ����ݶ�
        ucRet=Main_Get_ReqData_1_F14(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
//    case    CON_DATA_REQ1_F11:  //F11   ��ǰ�ź�ǿ��
//        ucRet=Main_Get_ReqData_1_F11(ucDAp,pstDataItem->ucDataContent,&usPos);
//        break;
//    case    CON_DATA_REQ1_F17:  //F17   ��ǰ�ܼ��й�����
//        ucRet=Main_Get_ReqData_1_F17(ucDAp,pstDataItem->ucDataContent,&usPos);
//        break;
//    case    CON_DATA_REQ1_F18:  //F18   ��ǰ�ܼ��޹�����
//        ucRet=Main_Get_ReqData_1_F18(ucDAp,pstDataItem->ucDataContent,&usPos);
//        break;
//    case    CON_DATA_REQ1_F19:  //F19   �����ܼ��й����������ܡ�����1~M��
//        ucRet=Main_Get_ReqData_1_F19(ucDAp,pstDataItem->ucDataContent,&usPos);
//        break;
//    case    CON_DATA_REQ1_F20:  //F20   �����ܼ��޹����������ܡ�����1~M��
//        ucRet=Main_Get_ReqData_1_F20(ucDAp,pstDataItem->ucDataContent,&usPos);
//        break;
//    case    CON_DATA_REQ1_F21:  //F21   �����ܼ��й����������ܡ�����1~M��
//        ucRet=Main_Get_ReqData_1_F21(ucDAp,pstDataItem->ucDataContent,&usPos);
//        break;
//    case    CON_DATA_REQ1_F22:  //F22   �����ܼ��޹����������ܡ�����1~M��
//        ucRet=Main_Get_ReqData_1_F22(ucDAp,pstDataItem->ucDataContent,&usPos);
//        break;
//    case    CON_DATA_REQ1_F23:  //F23   �ն˵�ǰʣ��������ѣ�
//        ucRet=Main_Get_ReqData_1_F23(ucDAp,pstDataItem->ucDataContent,&usPos);
//        break;
//    case    CON_DATA_REQ1_F24:  //F24   ��ǰ�����¸��ؿغ��ܼ��й����ʶ���ֵ
//        ucRet=Main_Get_ReqData_1_F24(ucDAp,pstDataItem->ucDataContent,&usPos);
//        break;
    case    CON_DATA_REQ1_F25:  //F25   ��ǰ���༰����/�޹����ʡ����������������ѹ���������������
        ucRet=Main_Get_ReqData_1_F25(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F26:  //F26   A��B��C�������ͳ�����ݼ����һ�ζ����¼
        ucRet=Main_Get_ReqData_1_F26(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F27:  //F27   ���ܱ�����ʱ�ӡ���̴����������һ�β���ʱ��
        ucRet=Main_Get_ReqData_1_F27(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F28:  //F28   �������״̬�ּ����λ��־
        ucRet=Main_Get_ReqData_1_F28(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F31:
        ucRet = Main_Get_ReqData_1_F31(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F32:
        ucRet = Main_Get_ReqData_1_F32(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F33:  //F33   ��ǰ������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
        ucRet=Main_Get_ReqData_1_F33(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F34:  //F34   ��ǰ������/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
        ucRet=Main_Get_ReqData_1_F34(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F35:  //F35   ����������/�޹��������������ʱ�䣨�ܡ�����1~M��
        ucRet=Main_Get_ReqData_1_F35(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F36:  //F36   ���·�����/�޹��������������ʱ�䣨�ܡ�����1~M��
        ucRet=Main_Get_ReqData_1_F36(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F37:  //F37   ����������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
        ucRet=Main_Get_ReqData_1_F37(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F38:  //F38   ���·�����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
        ucRet=Main_Get_ReqData_1_F38(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F39:  //F39   ����������/�޹��������������ʱ�䣨�ܡ�����1~M��
        ucRet=Main_Get_ReqData_1_F39(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F40:  //F40   ���·�����/�޹��������������ʱ�䣨�ܡ�����1~M��
        ucRet=Main_Get_ReqData_1_F40(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F41:  //F41   ���������й����������ܡ�����1~M��
        ucRet=Main_Get_ReqData_1_F41(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F42:  //F42   ���������޹����������ܡ�����1~M��
        ucRet=Main_Get_ReqData_1_F42(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F43:  //F43   ���շ����й����������ܡ�����1~M��
        ucRet=Main_Get_ReqData_1_F43(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F44:  //F44   ���շ����޹����������ܡ�����1~M��
        ucRet=Main_Get_ReqData_1_F44(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F45:  //F45   ���������й����������ܡ�����1~M��
        ucRet=Main_Get_ReqData_1_F45(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F46:  //F46   ���������޹����������ܡ�����1~M��
        ucRet=Main_Get_ReqData_1_F46(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F47:  //F47   ���·����й����������ܡ�����1~M��
        ucRet=Main_Get_ReqData_1_F47(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F48:  //F48   ���·����޹����������ܡ�����1~M��
        ucRet=Main_Get_ReqData_1_F48(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F49:  //F49   ��ǰ��ѹ��������λ��
        ucRet=Main_Get_ReqData_1_F49(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
//    case    CON_DATA_REQ1_F57:  //F57   ��ǰA��B��C�����ѹ������2~N��г����Чֵ
//        ucRet=Main_Get_ReqData_1_F57(ucDAp,pstDataItem->ucDataContent,&usPos);
//        break;
//    case    CON_DATA_REQ1_F58:  //F58   ��ǰA��B��C�����ѹ������2~N��г��������
//        ucRet=Main_Get_ReqData_1_F58(ucDAp,pstDataItem->ucDataContent,&usPos);
//        break;
//    case    CON_DATA_REQ1_F65:  //F65   ��ǰ������Ͷ��״̬
//        ucRet=Main_Get_ReqData_1_F65(ucDAp,pstDataItem->ucDataContent,&usPos);
//        break;
//    case    CON_DATA_REQ1_F66:  //F66   ��ǰ�������ۼƲ���Ͷ��ʱ��ʹ���
//        ucRet=Main_Get_ReqData_1_F66(ucDAp,pstDataItem->ucDataContent,&usPos);
//        break;
//    case    CON_DATA_REQ1_F67:  //F67   ���ա����µ������ۼƲ������޹�������
//        ucRet=Main_Get_ReqData_1_F67(ucDAp,pstDataItem->ucDataContent,&usPos);
//        break;
//    case    CON_DATA_REQ1_F73:  //F73   ֱ��ģ����ʵʱ����
//        ucRet=Main_Get_ReqData_1_F73(ucDAp,pstDataItem->ucDataContent,&usPos);
//        break;
//    case    CON_DATA_REQ1_F81:  //F81   Сʱ�����ܼ��й�����
//        ucRet=Main_Get_ReqData_1_F81(ucDAp,pstDataItem->ucDataContent,&usPos);
//        break;
//    case    CON_DATA_REQ1_F82:  //F82   Сʱ�����ܼ��޹�����
//        ucRet=Main_Get_ReqData_1_F82(ucDAp,pstDataItem->ucDataContent,&usPos);
//        break;
//    case    CON_DATA_REQ1_F83:  //F83   Сʱ�����ܼ��й��ܵ�����
//        ucRet=Main_Get_ReqData_1_F83(ucDAp,pstDataItem->ucDataContent,&usPos);
//        break;
//    case    CON_DATA_REQ1_F84:  //F84   Сʱ�����ܼ��޹��ܵ�����
//        ucRet=Main_Get_ReqData_1_F84(ucDAp,pstDataItem->ucDataContent,&usPos);
//        break;
    case    CON_DATA_REQ1_F89:  //F89   Сʱ�����й�����
        ucRet=Main_Get_ReqData_1_F89(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F90:  //F90   Сʱ����A���й�����
        ucRet=Main_Get_ReqData_1_F90(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F91:  //F91   Сʱ����B���й�����
        ucRet=Main_Get_ReqData_1_F91(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F92:  //F92   Сʱ����C���й�����
        ucRet=Main_Get_ReqData_1_F92(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F93:  //F93   Сʱ�����޹�����
        ucRet=Main_Get_ReqData_1_F93(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F94:  //F94   Сʱ����A���޹�����
        ucRet=Main_Get_ReqData_1_F94(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F95:  //F95   Сʱ����B���޹�����
        ucRet=Main_Get_ReqData_1_F95(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F96:  //F96   Сʱ����C���޹�����
        ucRet=Main_Get_ReqData_1_F96(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F97:  //F97   Сʱ����A���ѹ
        ucRet=Main_Get_ReqData_1_F97(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F98:  //F98   Сʱ����B���ѹ
        ucRet=Main_Get_ReqData_1_F98(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F99:  //F99   Сʱ����C���ѹ
        ucRet=Main_Get_ReqData_1_F99(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F100: //F100  Сʱ����A�����
        ucRet=Main_Get_ReqData_1_F100(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F101: //F101  Сʱ����B�����
        ucRet=Main_Get_ReqData_1_F101(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F102: //F102  Сʱ����C�����
        ucRet=Main_Get_ReqData_1_F102(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F103: //F103  Сʱ�����������
        ucRet=Main_Get_ReqData_1_F103(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F105: //F105  Сʱ���������й��ܵ�����
        ucRet=Main_Get_ReqData_1_F105(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F106: //F106  Сʱ���������޹��ܵ�����
        ucRet=Main_Get_ReqData_1_F106(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F107: //F107  Сʱ���ᷴ���й��ܵ�����
        ucRet=Main_Get_ReqData_1_F107(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F108: //F108  Сʱ���ᷴ���޹��ܵ�����
        ucRet=Main_Get_ReqData_1_F108(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F109: //F109  Сʱ���������й��ܵ���ʾֵ
        ucRet=Main_Get_ReqData_1_F109(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F110: //F110  Сʱ���������޹��ܵ���ʾֵ
        ucRet=Main_Get_ReqData_1_F110(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F111: //F111  Сʱ���ᷴ���й��ܵ���ʾֵ
        ucRet=Main_Get_ReqData_1_F111(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F112: //F112  Сʱ���ᷴ���޹��ܵ���ʾֵ
        ucRet=Main_Get_ReqData_1_F112(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F113: //F113  Сʱ�����ܹ�������
        ucRet=Main_Get_ReqData_1_F113(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F114: //F114  Сʱ����A�๦������
        ucRet=Main_Get_ReqData_1_F114(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F115: //F115  Сʱ����B�๦������
        ucRet=Main_Get_ReqData_1_F115(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F116: //F116  Сʱ����C�๦������
        ucRet=Main_Get_ReqData_1_F116(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
//    case    CON_DATA_REQ1_F121: //F121  Сʱ����ֱ��ģ����
//      ucRet=Main_Get_ReqData_1_F121(ucDAp,pstDataItem->ucDataContent,&usPos);
//        break;
    case    CON_DATA_REQ1_F129:
//azh 170921
    case    CON_DATA_REQ1_F177:
        ucRet=Main_Get_ReqData_1_F129(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F130:
        ucRet=Main_Get_ReqData_1_F130(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F131:
        ucRet=Main_Get_ReqData_1_F131(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F132:
        ucRet=Main_Get_ReqData_1_F132(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F133:
        ucRet=Main_Get_ReqData_1_F133(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F134:
        ucRet=Main_Get_ReqData_1_F134(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F135:
        ucRet=Main_Get_ReqData_1_F135(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F136:
        ucRet=Main_Get_ReqData_1_F136(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F137:
//azh 170921
    case    CON_DATA_REQ1_F178:
        ucRet=Main_Get_ReqData_1_F137(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F138:
        ucRet=Main_Get_ReqData_1_F138(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F139:
        ucRet=Main_Get_ReqData_1_F139(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F140:
        ucRet=Main_Get_ReqData_1_F140(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F141:
        ucRet=Main_Get_ReqData_1_F141(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F142:
        ucRet=Main_Get_ReqData_1_F142(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F143:
        ucRet=Main_Get_ReqData_1_F143(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F144:
        ucRet=Main_Get_ReqData_1_F144(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F145:
        ucRet=Main_Get_ReqData_1_F145(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F146:
        ucRet=Main_Get_ReqData_1_F146(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F147:
        ucRet=Main_Get_ReqData_1_F147(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F148:
        ucRet=Main_Get_ReqData_1_F148(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F149:
        ucRet=Main_Get_ReqData_1_F149(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F150:
        ucRet=Main_Get_ReqData_1_F150(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F151:
        ucRet=Main_Get_ReqData_1_F151(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    case    CON_DATA_REQ1_F152:
        ucRet=Main_Get_ReqData_1_F152(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
//azh 170921
    case    CON_DATA_REQ1_F162:
        ucRet=Main_Get_ReqData_1_F162(ucDAp,pstDataItem->ucDataContent,&usPos);
        break;
    default:
        ucRet=RET_ERROR;
        break;
    }
    pstDataItem->usDataLen=usPos;
    if(ucRet==RET_ERROR)
        pstDataItem->ucExeRet=CON_NACK_ERR_NOTEXE;
    else
        pstDataItem->ucExeRet=CON_NACK_ERR_NORMAL;
    return ucRet;
}

//����2�����ݣ��������ݣ�
unsigned char   Main_Recv_ReqData_2(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo)
{
    unsigned char ucRet=RET_ERROR;//RET_SUCCESS;//azh 130306
    unsigned char ucDAp;
    unsigned short usItemLabel;
    STDATAITEM *pstDataItem;
//  STDATETIME stStartTime,stEndTime;
//  unsigned char ucDataInter=1;
//  stStartTime=GetCurrentTime();
//  stEndTime=stStartTime;

    pstDataItem=&pstRecvTerMsg->stDataItem[ucDataItemNo];
    ucDAp=pstDataItem->ucMPNo;
    usItemLabel=pstDataItem->usItemLabel;

    switch(usItemLabel)     //Fn
    {
    case    CON_DATA_REQ2_F1:   //F1    �ն���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
        ucRet=Main_Recv_ReqData_2_F1(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F2:   //F2    �ն��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
        ucRet=Main_Recv_ReqData_2_F2(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F3:   //F3    �ն���������/�޹��������������ʱ�䣨�ܡ�����1~M��
        ucRet=Main_Recv_ReqData_2_F3(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F4:   //F4    �ն��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��
        ucRet=Main_Recv_ReqData_2_F4(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F5:   //F5    �ն��������й����������ܡ�����1~M��
        ucRet=Main_Recv_ReqData_2_F5(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F6:   //F6    �ն��������޹����������ܡ�����1~M��
        ucRet=Main_Recv_ReqData_2_F6(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F7:   //F7    �ն��ᷴ���й����������ܡ�����1~M��
        ucRet=Main_Recv_ReqData_2_F7(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F8:   //F8    �ն��ᷴ���޹����������ܡ�����1~M��
        ucRet=Main_Recv_ReqData_2_F8(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F9:   //F9    �����ն���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
        ucRet=Main_Recv_ReqData_2_F9(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F10:  //F10   �����ն��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
        ucRet=Main_Recv_ReqData_2_F10(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F11:  //F11   �����ն���������/�޹��������������ʱ�䣨�ܡ�����1~M��
        ucRet=Main_Recv_ReqData_2_F11(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F12:  //F12   �����ն��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��
        ucRet=Main_Recv_ReqData_2_F12(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F17:  //F17   �¶���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
        ucRet=Main_Recv_ReqData_2_F17(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F18:  //F18   ������/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
        ucRet=Main_Recv_ReqData_2_F18(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F19:  //F19   ������/�޹��������������ʱ�䣨�ܡ�����1~M��
        ucRet=Main_Recv_ReqData_2_F19(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F20:  //F20   �¶��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��
        ucRet=Main_Recv_ReqData_2_F20(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F21:  //F21   �¶��������й����������ܡ�����1~M��
        ucRet=Main_Recv_ReqData_2_F21(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F22:  //F22   �¶��������޹����������ܡ�����1~M��
        ucRet=Main_Recv_ReqData_2_F22(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F23:  //F23   �¶��ᷴ���й����������ܡ�����1~M��
        ucRet=Main_Recv_ReqData_2_F23(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F24:  //F24   �¶��ᷴ���޹����������ܡ�����1~M��
        ucRet=Main_Recv_ReqData_2_F24(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F25:  //F25   �շ�������й����ʼ�����ʱ�䡢�й�����Ϊ��ʱ��
        ucRet=Main_Recv_ReqData_2_F25(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F26:  //F26   ���ܼ������������������ʱ��
        ucRet=Main_Recv_ReqData_2_F26(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F27:  //F27   �յ�ѹͳ������
        ucRet=Main_Recv_ReqData_2_F27(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F28:  //F28   �ղ�ƽ���Խ���ۼ�ʱ��
        ucRet=Main_Recv_ReqData_2_F28(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F29:  //F29   �յ���Խ��ͳ��
        ucRet=Main_Recv_ReqData_2_F29(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F30:  //F30   �����ڹ���Խ���ۼ�ʱ��
        ucRet=Main_Recv_ReqData_2_F30(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F31:  //F31   �յ��ܱ��������
        ucRet=Main_Recv_ReqData_2_F32(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F32:  //F32   ����
        ucRet=Main_Recv_ReqData_2_F31(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F33:  //F33   �·�������й����ʼ�����ʱ�䡢�����й�����Ϊ��ʱ��
        ucRet=Main_Recv_ReqData_2_F33(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F34:  //F34   �·����й��������������ʱ�䡢���й��������������ʱ��
        ucRet=Main_Recv_ReqData_2_F34(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F35:  //F35   �µ�ѹͳ������
        ucRet=Main_Recv_ReqData_2_F35(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F36:  //F36   �²�ƽ���Խ���ۼ�ʱ��
        ucRet=Main_Recv_ReqData_2_F36(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F37:  //F37   �µ���Խ��ͳ��
        ucRet=Main_Recv_ReqData_2_F37(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F38:  //F38   �����ڹ���Խ���ۼ�ʱ��
        ucRet=Main_Recv_ReqData_2_F38(pstDataItem,ucDAp);
        break;
//    case    CON_DATA_REQ2_F41:  //F41   ������Ͷ���ۼ�ʱ��ʹ���
////      ucRet=Main_Recv_ReqData_2_F41(pstDataItem,ucDAp);
//        break;
//    case    CON_DATA_REQ2_F42:  //F42   �ա��µ������ۼƲ������޹�������
////      ucRet=Main_Recv_ReqData_2_F42(pstDataItem,ucDAp);
//        break;
    case    CON_DATA_REQ2_F43:  //F43   �չ������������ۼ�ʱ��
        ucRet=Main_Recv_ReqData_2_F43(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F44:  //F44   �¹������������ۼ�ʱ��
        ucRet=Main_Recv_ReqData_2_F44(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F49:  //F49   �ն��չ���ʱ�䡢�ո�λ�ۼƴ���
        ucRet=Main_Recv_ReqData_2_F49(pstDataItem,ucDAp);
        break;
//    case    CON_DATA_REQ2_F50:  //F50   �ն��տ���ͳ������
////        ucRet=Main_Recv_ReqData_2_F50(pstDataItem,ucDAp);
//        break;
    case    CON_DATA_REQ2_F51:  //F51   �ն��¹���ʱ�䡢�¸�λ�ۼƴ���
        ucRet=Main_Recv_ReqData_2_F51(pstDataItem,ucDAp);
        break;
//    case    CON_DATA_REQ2_F52:  //F52   �ն��¿���ͳ������
////        ucRet=Main_Recv_ReqData_2_F52(pstDataItem,ucDAp);
//        break;
    case    CON_DATA_REQ2_F53:  //F53   �ն�����վ��ͨ������
        ucRet=Main_Recv_ReqData_2_F53(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F54:  //F54   �ն�����վ��ͨ������
        ucRet=Main_Recv_ReqData_2_F54(pstDataItem,ucDAp);
        break;
//    case    CON_DATA_REQ2_F57:  //F57   �ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��
////      ucRet=Main_Recv_ReqData_2_F57(pstDataItem,ucDAp);
//        break;
//    case    CON_DATA_REQ2_F58:  //F58   �ܼ������ۼ��й����������ܡ�����1~M��
////      ucRet=Main_Recv_ReqData_2_F58(pstDataItem,ucDAp);
//        break;
//    case    CON_DATA_REQ2_F59:  //F59   �ܼ������ۼ��޹����������ܡ�����1~M��
////      ucRet=Main_Recv_ReqData_2_F59(pstDataItem,ucDAp);
//        break;
//    case    CON_DATA_REQ2_F60:  //F60   �ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��
////      ucRet=Main_Recv_ReqData_2_F60(pstDataItem,ucDAp);
//        break;
//    case    CON_DATA_REQ2_F61:  //F61   �ܼ������ۼ��й����������ܡ�����1~M��
////      ucRet=Main_Recv_ReqData_2_F61(pstDataItem,ucDAp);
//        break;
//    case    CON_DATA_REQ2_F62:  //F62   �ܼ������ۼ��޹����������ܡ�����1~M��
////      ucRet=Main_Recv_ReqData_2_F62(pstDataItem,ucDAp);
//        break;
//    case    CON_DATA_REQ2_F65:  //F65   �ܼ��鳬���ʶ�ֵ�����ۼ�ʱ�䡢���ۼƵ�����
////      ucRet=Main_Recv_ReqData_2_F65(pstDataItem,ucDAp);
//        break;
//    case    CON_DATA_REQ2_F66:  //F66   �ܼ��鳬�µ�������ֵ�����ۼ�ʱ�䡢�ۼƵ�����
////      ucRet=Main_Recv_ReqData_2_F66(pstDataItem,ucDAp);
//        break;
//    case    CON_DATA_REQ2_F73:  //F73   �ܼ����й���������
////      ucRet=Main_Recv_ReqData_2_F73(pstDataItem,ucDAp);
//        break;
//    case    CON_DATA_REQ2_F74:  //F74   �ܼ����޹���������
////      ucRet=Main_Recv_ReqData_2_F74(pstDataItem,ucDAp);
//        break;
//    case    CON_DATA_REQ2_F75:  //F75   �ܼ����й�����������
////      ucRet=Main_Recv_ReqData_2_F75(pstDataItem,ucDAp);
//        break;
//    case    CON_DATA_REQ2_F76:  //F76   �ܼ����޹�����������
////      ucRet=Main_Recv_ReqData_2_F76(pstDataItem,ucDAp);
//        break;
    case    CON_DATA_REQ2_F81:  //F81   �й���������
        ucRet=Main_Recv_ReqData_2_F81(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F82:  //F82   A���й���������
        ucRet=Main_Recv_ReqData_2_F82(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F83:  //F83   B���й���������
        ucRet=Main_Recv_ReqData_2_F83(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F84:  //F84   C���й���������
        ucRet=Main_Recv_ReqData_2_F84(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F85:  //F85   �޹���������
        ucRet=Main_Recv_ReqData_2_F85(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F86:  //F86   A���޹���������
        ucRet=Main_Recv_ReqData_2_F86(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F87:  //F87   B���޹���������
        ucRet=Main_Recv_ReqData_2_F87(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F88:  //F88   C���޹���������
        ucRet=Main_Recv_ReqData_2_F88(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F89:  //F89   A���ѹ����
        ucRet=Main_Recv_ReqData_2_F89(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F90:  //F90   B���ѹ����
        ucRet=Main_Recv_ReqData_2_F90(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F91:  //F91   C���ѹ����
        ucRet=Main_Recv_ReqData_2_F91(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F92:  //F92   A���������
        ucRet=Main_Recv_ReqData_2_F92(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F93:  //F93   B���������
        ucRet=Main_Recv_ReqData_2_F93(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F94:  //F94   C���������
        ucRet=Main_Recv_ReqData_2_F94(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F95:  //F95   �����������
        ucRet=Main_Recv_ReqData_2_F95(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F97:  //F97   �����й��ܵ�����
        ucRet=Main_Recv_ReqData_2_F97(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F98:  //F98   �����޹��ܵ�����
        ucRet=Main_Recv_ReqData_2_F98(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F99:  //F99   �����й��ܵ�����
        ucRet=Main_Recv_ReqData_2_F99(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F100: //F100  �����޹��ܵ�����
        ucRet=Main_Recv_ReqData_2_F100(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F101: //F101  �����й��ܵ���ʾֵ
        ucRet=Main_Recv_ReqData_2_F101(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F102: //F102  �����޹��ܵ���ʾֵ
        ucRet=Main_Recv_ReqData_2_F102(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F103: //F103  �����й��ܵ���ʾֵ
        ucRet=Main_Recv_ReqData_2_F103(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F104: //F104  �����޹��ܵ���ʾֵ
        ucRet=Main_Recv_ReqData_2_F104(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F105: //F105  �ܹ�������
        ucRet=Main_Recv_ReqData_2_F105(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F106: //F106  A�๦������
        ucRet=Main_Recv_ReqData_2_F106(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F107: //F107  B�๦������
        ucRet=Main_Recv_ReqData_2_F107(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F108: //F108  C�๦������
        ucRet=Main_Recv_ReqData_2_F108(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F109:
        ucRet=Main_Recv_ReqData_2_F109(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F110:
        ucRet=Main_Recv_ReqData_2_F110(pstDataItem,ucDAp);
        break;
//  case    CON_DATA_REQ2_F113: //F113  A��2~19��г�����������ֵ������ʱ��
//      ucRet=Main_Recv_ReqData_2_F113(pstDataItem,ucDAp);
//      break;
//  case    CON_DATA_REQ2_F114: //F114  B��2~19��г�����������ֵ������ʱ��
//      ucRet=Main_Recv_ReqData_2_F114(pstDataItem,ucDAp);
//      break;
//  case    CON_DATA_REQ2_F115: //F115  C��2~19��г�����������ֵ������ʱ��
//      ucRet=Main_Recv_ReqData_2_F115(pstDataItem,ucDAp);
//      break;
//  case    CON_DATA_REQ2_F116: //F116  A��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
//      ucRet=Main_Recv_ReqData_2_F116(pstDataItem,ucDAp);
//      break;
//  case    CON_DATA_REQ2_F117: //F117  B��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
//      ucRet=Main_Recv_ReqData_2_F117(pstDataItem,ucDAp);
//      break;
//  case    CON_DATA_REQ2_F118: //F118  C��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
//      ucRet=Main_Recv_ReqData_2_F118(pstDataItem,ucDAp);
//      break;
//  case    CON_DATA_REQ2_F121: //F121  A��г��Խ����ͳ������
//      ucRet=Main_Recv_ReqData_2_F121(pstDataItem,ucDAp);
//      break;
//  case    CON_DATA_REQ2_F122: //F122  B��г��Խ����ͳ������
//      ucRet=Main_Recv_ReqData_2_F122(pstDataItem,ucDAp);
//      break;
//  case    CON_DATA_REQ2_F123: //F123  C��г��Խ����ͳ������
//      ucRet=Main_Recv_ReqData_2_F123(pstDataItem,ucDAp);
//      break;
//  case    CON_DATA_REQ2_F129: //F129  ֱ��ģ����Խ�����ۼ�ʱ�䡢���/��Сֵ������ʱ��
//      ucRet=Main_Recv_ReqData_2_F129(pstDataItem,ucDAp);
//      break;
//  case    CON_DATA_REQ2_F130: //F130  ֱ��ģ����Խ�����ۼ�ʱ�䡢���/��Сֵ������ʱ��
//      ucRet=Main_Recv_ReqData_2_F130(pstDataItem,ucDAp);
//      break;
//  case    CON_DATA_REQ2_F138: //F138  ֱ��ģ������������
//      ucRet=Main_Recv_ReqData_2_F138(pstDataItem,ucDAp);
//      break;
    case    CON_DATA_REQ2_F145: //F145  һ�����޹��ܵ���ʾֵ����
        ucRet=Main_Recv_ReqData_2_F145(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F146: //F146  �������޹��ܵ���ʾֵ����
        ucRet=Main_Recv_ReqData_2_F146(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F147: //F147  �������޹��ܵ���ʾֵ����
        ucRet=Main_Recv_ReqData_2_F147(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F148: //F148  �������޹��ܵ���ʾֵ����
        ucRet=Main_Recv_ReqData_2_F148(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F153:
        ucRet = Main_Recv_ReqData_2_F153(pstDataItem,ucDAp);
        break;
    case    CON_DATA_REQ2_F154:
        ucRet = Main_Recv_ReqData_2_F154(pstDataItem,ucDAp);
        break;
//azh 170831
//    case    CON_DATA_REQ2_F155:
//        ucRet = Main_Recv_ReqData_2_F155(pstDataItem,ucDAp);
//        break;
//    case    CON_DATA_REQ2_F156:
//        ucRet = Main_Recv_ReqData_2_F156(pstDataItem,ucDAp);
//        break;
//    case    CON_DATA_REQ2_F157:
//        ucRet = Main_Recv_ReqData_2_F157(pstDataItem,ucDAp);
//        break;
//    case    CON_DATA_REQ2_F158:
//        ucRet = Main_Recv_ReqData_2_F158(pstDataItem,ucDAp);
//        break;
//    case    CON_DATA_REQ2_F159:
//        ucRet = Main_Recv_ReqData_2_F159(pstDataItem,ucDAp);
//        break;
//    case    CON_DATA_REQ2_F160:
//        ucRet = Main_Recv_ReqData_2_F160(pstDataItem,ucDAp);
//        break;
    case    CON_DATA_REQ2_F161:
    case    CON_DATA_REQ2_F162:
    case    CON_DATA_REQ2_F163:
    case    CON_DATA_REQ2_F164:
    case    CON_DATA_REQ2_F165:
    case    CON_DATA_REQ2_F166:
    case    CON_DATA_REQ2_F167:
    case    CON_DATA_REQ2_F168:
        ucRet=Main_Recv_ReqData_2_F161_F168(pstDataItem,ucDAp,usItemLabel);
        break;
    case    CON_DATA_REQ2_F169:
    case    CON_DATA_REQ2_F170:
    case    CON_DATA_REQ2_F171:
    case    CON_DATA_REQ2_F172:
    case    CON_DATA_REQ2_F173:
    case    CON_DATA_REQ2_F174:
    case    CON_DATA_REQ2_F175:
    case    CON_DATA_REQ2_F176:
        ucRet=Main_Recv_ReqData_2_F169_F176(pstDataItem,ucDAp,usItemLabel);
        break;
    case    CON_DATA_REQ2_F177:
    case    CON_DATA_REQ2_F178:
    case    CON_DATA_REQ2_F179:
    case    CON_DATA_REQ2_F180:
    case    CON_DATA_REQ2_F181:
    case    CON_DATA_REQ2_F182:
    case    CON_DATA_REQ2_F183:
    case    CON_DATA_REQ2_F184:
        ucRet=Main_Recv_ReqData_2_F177_F184(pstDataItem,ucDAp,usItemLabel);
        break;
    case    CON_DATA_REQ2_F185:
    case    CON_DATA_REQ2_F186:
    case    CON_DATA_REQ2_F187:
    case    CON_DATA_REQ2_F188:
    case    CON_DATA_REQ2_F189:
    case    CON_DATA_REQ2_F190:
    case    CON_DATA_REQ2_F191:
    case    CON_DATA_REQ2_F192:
    case    CON_DATA_REQ2_F193:
    case    CON_DATA_REQ2_F194:
    case    CON_DATA_REQ2_F195:
    case    CON_DATA_REQ2_F196:
        ucRet=Main_Recv_ReqData_2_F185_F196(pstDataItem,ucDAp,usItemLabel);
        break;
//azh 170923
	case	CON_DATA_REQ2_F219:	//F219	��������������������ݵ�Ԫ��ʽ
		ucRet=Main_Recv_ReqData_2_F219(pstDataItem,ucDAp);
		break;
    default:
        ucRet=RET_ERROR;//azh 130308
        break;
    }
    if(ucRet==RET_ERROR)
        pstDataItem->ucExeRet=CON_NACK_ERR_NOTEXE;
    else
        pstDataItem->ucExeRet=CON_NACK_ERR_NORMAL;
    return ucRet;
}

//����3�����ݣ��¼���
unsigned char   Main_Recv_ReqData_3(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo)
{
    unsigned char ucRet=RET_SUCCESS,i=0;
    unsigned char ucDAp;
    unsigned short usItemLabel;
    unsigned char ucECLeftStart=0,ucECEnd=0;
    STDATAITEM *pstDataItem;

    pstDataItem=&pstRecvTerMsg->stDataItem[ucDataItemNo];
    ucDAp=pstDataItem->ucMPNo;
    usItemLabel=pstDataItem->usItemLabel;

    ucECLeftStart=pstDataItem->ucDataContent[0];
    ucECEnd=pstDataItem->ucDataContent[1];
    if(ucECLeftStart==ucECEnd)
    {
        ucRet=RET_ERROR;
    }
    if(ucRet==RET_SUCCESS)
    {
        switch(usItemLabel)     //Fn
        {
        case CON_DATA_REQ3_F1:      //��Ҫ�¼�
              i = ucDataItemNo;
//            for(i=0;i<CON_TERMSG_DI_NUM;i++)
//            {
                ucRet=Main_Recv_ReqData_3_Important(&(pstRecvTerMsg->stDataItem[i]),ucDAp,&ucECLeftStart,ucECEnd);
//                if(ucRet!=RET_SUCCESS || ucECLeftStart==ucECEnd)
//                    break;

//azh ������λ��������� ���һ�¶�����������¼ ��֡��������
//                if(i <(CON_TERMSG_DI_NUM-1))
//                {    
//    				pstRecvTerMsg->stDataItem[i+1] = pstRecvTerMsg->stDataItem[i];
//    				pstRecvTerMsg->stDataItem[i+1].ucDataContent += pstRecvTerMsg->stDataItem[i].usDataLen;
//    				pstRecvTerMsg->stDataItem[i+1].ucDataContent[0] = ucECLeftStart;
//    				pstRecvTerMsg->stDataItem[i+1].ucDataContent[1] = ucECEnd;
//    				pstRecvTerMsg->stDataItem[i+1].usDataLen = 2;
//    				pstRecvTerMsg->ucDataItemNum++;
//                    break;
//    			}
//    			else
//    			{
//    			    break;    
//    			}
                    
//            }
            break;
        case CON_DATA_REQ3_F2:      //һ���¼�
            i = ucDataItemNo;
//            for(i=0;i<CON_TERMSG_DI_NUM;i++)
//            {
                ucRet=Main_Recv_ReqData_3_Normal(&(pstRecvTerMsg->stDataItem[i]),ucDAp,&ucECLeftStart,ucECEnd);
//                if(ucRet!=RET_SUCCESS || ucECLeftStart==ucECEnd)
//                    break;
//            }
            break;
        default:
            break;
        }
    }
    if(ucRet==RET_ERROR)
    {
        pstDataItem->ucExeRet=CON_NACK_ERR_NOTEXE;
        pstDataItem->usDataLen=0;
    }
    else
        pstDataItem->ucExeRet=CON_NACK_ERR_NORMAL;
    return ucRet;
}

//�ն˲�����ѯ
unsigned char Main_Recv_GetPara(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo)
{
    unsigned char ucRet=RET_ERROR;//RET_SUCCESS;
//  unsigned char *pucBuffer=NULL;
    unsigned char ucDAp;
    unsigned short usItemLabel;
    STDATAITEM *pstDataItem;
    STDIINFO       stRetDIInfo;
//  unsigned char ucNum=0;

    pstDataItem=&pstRecvTerMsg->stDataItem[ucDataItemNo];
//  ucNum=pstDataItem->ucDataContent[0];                //ĳЩ��Ҫ��������n
    ucDAp=pstDataItem->ucMPNo;
    usItemLabel=pstDataItem->usItemLabel;

    switch(usItemLabel)     //Fn
    {
    case CON_PARA_F1_COMMPARA:      //F1�ն�ͨѶ����
        ucRet=Main_Recv_Get_F1_CommPara(pstDataItem,ucDAp);
        break;
    case CON_PARA_F2_RELAYPARA:
        ucRet = Main_Recv_Get_F2_RelayPara(pstDataItem,ucDAp);
        break;
    case CON_PARA_F3_STAIPADDR:     //F3����ͨ��IP��ַ
        ucRet=Main_Recv_Get_F3_IP_Addr(pstDataItem,ucDAp);
        break;
    case CON_PARA_F4_STAPHONENO:    //F4������վ����Ͷ������ĺ���
        ucRet=Main_Recv_Get_F4_Station_Number(pstDataItem,ucDAp);
        break;
    case CON_PARA_F5_TERENCRYPT:    //F5����
        ucRet=Main_Recv_Get_F5_TerEncrypt(pstDataItem,ucDAp);
        break;
    case CON_PARA_F6_TERGROUPADDR:  //F6���ն����ַ����
        ucRet=Main_Recv_Get_F6_TerGroupAddr(pstDataItem,ucDAp);
        break;
    case CON_PARA_F7_IPADDR:        //F7���ն˳���������
        ucRet=Main_Recv_Get_F7_LocalIP(pstDataItem,ucDAp);
        break;
    case CON_PARA_F8_ALRCTLWORD:    //F8���ն��¼���¼��������
        ucRet=Main_Recv_Get_F62_CommType(pstDataItem,ucDAp);
        break;
    case CON_PARA_F9_CONFIGNUM:     //F9���ն�����������
        ucRet=Main_Recv_Get_F8_AlarmControlWord(pstDataItem,ucDAp);
        break;
    case CON_PARA_F10_METERPARA:    //F10���ն˵��ܱ�/��������װ�����ò���
        ucRet=Main_Recv_Get_F10_MeterPara(pstDataItem,ucDAp);
        break;
//  case CON_PARA_F11_PULSEPARA:    //F11���ն��������ò���
//      ucRet=Main_Recv_Get_F11_PulsePara(pstDataItem,ucDAp);
//      break;
//  case CON_PARA_F12_DINPUTPARA:   //F12���ն�״̬���������
//      ucRet=Main_Recv_Get_F12_DInputPara(pstDataItem,ucDAp);
//      break;
//  case CON_PARA_F13_ANALOGPARA:   //F13���ն˵�ѹ/����ģ�������ò���
//      ucRet=Main_Recv_Get_F13_AnalogPara(pstDataItem,ucDAp);
//      break;
//  case CON_PARA_F14_SUMGPARA:     //F14���ն��ܼ������ò���
//      ucRet=Main_Recv_Get_F14_SumGPara(pstDataItem,ucDAp);
//      break;
//  case CON_PARA_F15_POWVALDIFF:   //F15���й��ܵ������Խ���¼���������
//      ucRet=Main_Recv_Get_F15_PowValDiff(pstDataItem,ucDAp);
//      break;
    case CON_PARA_F16_PDPINFO:      //F16������ר���û���������
        ucRet=Main_Recv_Get_F16_PDPInfo(pstDataItem,ucDAp);
        break;
//  case CON_PARA_F17_KEEPPOWER:    //F17���ն˱�����ֵ
//      ucRet=Main_Recv_Get_F17_KeepPower(pstDataItem,ucDAp);
//      break;
//  case CON_PARA_F18_POWCTRLTIMEPER:   //F18���ն˹���ʱ��
//      ucRet=Main_Recv_Get_F18_PowCtrlTimePer(pstDataItem,ucDAp);
//      break;
//  case CON_PARA_F19_POWCTRLCOEF:      //F19���ն�ʱ�ι��ض�ֵ����ϵ��
//      ucRet=Main_Recv_Get_F19_PowCtrlCoef(pstDataItem,ucDAp);
//      break;
//  case CON_PARA_F20_ENERCTRLCOEF:     //F20���ն��µ������ض�ֵ����ϵ��
//      ucRet=Main_Recv_Get_F20_EnerCtrlCoef(pstDataItem,ucDAp);
//      break;
//  case CON_PARA_F21_PERTIMEFEE:       //F21���ն˵���������ʱ�κͷ�����
//      ucRet=Main_Recv_Get_F21_PerTimeFee(pstDataItem,ucDAp);
//      break;
//  case CON_PARA_F22_FEEPARA:          //F22���ն˵���������
//      ucRet=Main_Recv_Get_F22_FeePara(pstDataItem,ucDAp);
//      break;
//  case CON_PARA_F23_URGEALARMPARA:    //F23���ն˴߷Ѹ澯����
//      ucRet=Main_Recv_Get_F23_UrgeAlarmPara(pstDataItem,ucDAp);
//      break;
    case CON_PARA_F24_COPYINTER:    //F24���ն˳���������
        ucRet=Main_Recv_Get_F24_CopyInter(pstDataItem,ucDAp);
        break;
    case CON_PARA_F25_MPBASEPARA:   //F25���������������
        ucRet=Main_Recv_Get_F25_MPBasePara(pstDataItem,ucDAp);
        break;
    case CON_PARA_F26_MPLIMIT:      //F26����������ֵ����
        ucRet=Main_Recv_Get_F26_MPLimit(pstDataItem,ucDAp);
        break;
    case CON_PARA_F27_MPFREEZEPARA: //F27�����������ݶ������
        ucRet=Main_Recv_Get_F27_MPFreezePara(pstDataItem,ucDAp);
        break;
    case CON_PARA_F28_MPPOWRATEPARA:    //F28�������㹦�������ֶ���ֵ
        ucRet=Main_Recv_Get_F28_MPPowRatePara(pstDataItem,ucDAp);
        break;
    case CON_PARA_F29_METERADDR:    //F29:
        ucRet=Main_Recv_Get_F29_Ter_Meter_addr(pstDataItem,ucDAp);
        break;
    case CON_PARA_F30_METERCOPYFLAG:
        ucRet = Main_Recv_Get_F30_CopyFlag(pstDataItem,ucDAp);
        break;
  case CON_PARA_F33_SUMGFREEZEPARA:   //F33���ܼ������ݶ������
      ucRet=Main_Recv_Get_F33_SumGFreezePara(pstDataItem,ucDAp);
      break;
    case CON_PARA_F34_PORTPROPERTY:
        ucRet = Main_Recv_Get_F34_PortProp(pstDataItem,ucDAp);
        break;
//    case CON_PARA_F35_EMPHAFLAG:
//        ucRet = Main_Recv_Get_F35_EmphaFlag(pstDataItem,ucDAp);
//        break;
    case CON_PARA_F37_LINKPARA:
        ucRet = Main_Recv_Get_F37_LinkPara(pstDataItem,ucDAp);
        break;
    case CON_PARA_F38_JOB1PARA:
        ucRet = Main_Recv_Get_F38_JobPara(pstDataItem,ucDAp,0);
        break;
    case CON_PARA_F39_JOB2PARA:
        ucRet = Main_Recv_Get_F38_JobPara(pstDataItem,ucDAp,1);
        break;
//  case CON_PARA_F41_TIMEPERSETTING:   //F41��ʱ�ι��ض�ֵ
//      ucRet=Main_Recv_Get_F41_TimePerSetting(pstDataItem,ucDAp);
//      break;
//  case CON_PARA_F42_RESTCTRL:         //F42�����ݹ��ز���
//      ucRet=Main_Recv_Get_F42_RestCtrl(pstDataItem,ucDAp);
//      break;
//  case CON_PARA_F43_POWERSLIP:        //F43�����ʿ��ƵĹ��ʼ��㻬��ʱ��
//      ucRet=Main_Recv_Get_F43_PowerSlip(pstDataItem,ucDAp);
//      break;
//  case CON_PARA_F44_SALESTOP:         //F44��Ӫҵ��ͣ�ز���
//      ucRet=Main_Recv_Get_F44_SaleStop(pstDataItem,ucDAp);
//      break;
//  case CON_PARA_F45_POWERCTRLTURN:    //F45�������ִ��趨
//      ucRet=Main_Recv_Get_F45_PowerCtrlTrun(pstDataItem,ucDAp);
//      break;
//  case CON_PARA_F46_MONTHENERCTRL:    //F46���µ����ض�ֵ
//      ucRet=Main_Recv_Get_F46_MonthEnerCtrl(pstDataItem,ucDAp);
//      break;
//  case CON_PARA_F47_BUYENERCTRL:      //F47���������ز���
//      ucRet=Main_Recv_Get_F47_BuyEnerCtrl(pstDataItem,ucDAp);
//      break;
//  case CON_PARA_F48_ENERCTRLTURN:     //F48������ִ��趨
//      ucRet=Main_Recv_Get_F48_EnerCtrlTurn(pstDataItem,ucDAp);
//      break;
//  case CON_PARA_F49_CTRLALRTIME:      //F49�����ظ澯ʱ��
//      ucRet=Main_Recv_Get_F49_CtrlAlrTime(pstDataItem,ucDAp);
//      break;
    case CON_PARA_F57_SOUNDALARM:       //F57���ն������澯����M��ֹ����
        ucRet=Main_Recv_Get_F57_SoundAlarm(pstDataItem,ucDAp);
        break;
//  case CON_PARA_F58_KEEPAUTO:         //F58���ն��Զ��������
//      ucRet=Main_Recv_Get_F58_KeepAuto(pstDataItem,ucDAp);
//      break;
    case CON_PARA_F59_MTRERRORPARA:     //F59�����ܱ��쳣�б���ֵ�趨
        ucRet=Main_Recv_Get_F59_MtErrorPara(pstDataItem,ucDAp);
        break;
//  case CON_PARA_F60_HARMONPARA:
//      ucRet = Main_Recv_Get_F92_HarmValUpParam(pstDataItem,ucDAp);
//      break;
    case CON_PARA_F61_DCAINPARA:
        ucRet = Main_Recv_Get_F61_AnalogPara(pstDataItem,ucDAp);
        break;
    case CON_PARA_F62_COMMTYPE:         //F62 ����ר��������ʽ
        ucRet=Main_Recv_Get_F62_CommType(pstDataItem,ucDAp);
        break;
    case CON_PARA_F63_FLUXLIMIT:            //F63 ��ͨ��������������
        ucRet=Main_Recv_Get_F63_FluxLimit(pstDataItem,ucDAp);
        break;
    case CON_PARA_F65_UPDATA1PARA:      //F65����ʱ����1��������������
        ucRet=Main_Recv_Get_F65_UpData1Para(pstDataItem,ucDAp);
        break;
    case CON_PARA_F66_UPDATA2PARA:      //F66����ʱ����2��������������
        ucRet=Main_Recv_Get_F66_UpData2Para(pstDataItem,ucDAp);
        break;
    case CON_PARA_F67_UPDATA1CTRL:      //F67����ʱ����1��������������/ֹͣ����
        ucRet=Main_Recv_Get_F67_UpData1Ctrl(pstDataItem,ucDAp);
        break;
    case CON_PARA_F68_UPDATA2CTRL:      //F68����ʱ����2��������������/ֹͣ����
        ucRet=Main_Recv_Get_F68_UpData2Ctrl(pstDataItem,ucDAp);
        break;
//  case CON_PARA_F73_CAPPARA:
//  case CON_PARA_F74_CAPRUNPARA:
//  case CON_PARA_F75_CAPRELAYPARA:
//  case CON_PARA_F76_CAPCTRL:
//      ucRet=Main_Recv_Get_F73_F76(pstDataItem,ucDAp,usItemLabel);
//      break;
//  case CON_PARA_F81_DCARATE:
//      ucRet = Main_Recv_Get_F81_DCRate(pstDataItem,ucDAp);
//      break;
//  case CON_PARA_F82_DCALIMIT:
//      ucRet = Main_Recv_Get_F82_DCLimit(pstDataItem,ucDAp);
//      break;
//  case CON_PARA_F83_DCAFREESE:
//      ucRet = Main_Recv_Get_F83_M(pstDataItem,ucDAp);
//      break;

//azh 170919
    case CON_PARA_F90_WIRELESS:      //F90������ͨ�Ų���
        ucRet=Main_Recv_Get_F90_Wireless(pstDataItem,ucDAp);
        break;
    case CON_PARA_F91_LOCATION:      //F91���ն˵���λ��
        ucRet=Main_Recv_Get_F91_Location(pstDataItem,ucDAp);
        break;
    case CON_PARA_F150_SNSTATE:      //F150��������״̬
        ucRet=Main_Recv_Get_F150_Snstate(pstDataItem,ucDAp);
        break;

    case CON_PARA_F89_ADDR:
    case CON_PARA_F207_ADDR:        //F68����ʱ����2��������������/ֹͣ����
        ucRet=Main_Recv_Get_F89_TerAddr(pstDataItem,ucDAp);
        break;
    default:
        ucRet=RET_ERROR;
        break;
    }

    if(ucRet==RET_ERROR)
    {
        pstDataItem->ucExeRet=CON_NACK_ERR_NOTEXE;
//      pstDataItem->ucExeRet=CON_NACK_ERR_NORMAL;
        pstDataItem->usDataLen=Main_Get_CommandLen(&stRetDIInfo,CON_TERMSG_SETPARA,usItemLabel,(unsigned char*)(pstDataItem->ucDataContent));
        memset(pstDataItem->ucDataContent,0xEE,pstDataItem->usDataLen);                                 //���������ڣ�������Ч����
    }else
    {
        pstDataItem->ucExeRet=CON_NACK_ERR_NORMAL;
    }

    return ucRet;
}

//��չ����
unsigned char Main_Recv_ExtCmd(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo)
{//�Զ��壬��������һ��DTDADG��������Ϊ������
    unsigned char ucRet=RET_SUCCESS;
    unsigned char ucDAp;
//  unsigned char ucNum,ucNum2;
    unsigned short usItemLabel;
    STDATAITEM *pstDataItem;
//  STTERAUXPARA stTerAuxPara;
//
//  if(ucDataItemNo!=0)
//      ucRet=RET_ERROR;
//
    if(ucRet==RET_SUCCESS)
    {
        pstDataItem=&pstRecvTerMsg->stDataItem[ucDataItemNo];
        ucDAp=pstDataItem->ucMPNo;
        usItemLabel=pstDataItem->usItemLabel;
//      ucNum=pstDataItem->ucDataContent[0];                //ĳЩ��Ҫ��������n
//      ucNum2=pstDataItem->ucDataContent[6];               //ĳЩ��Ҫ��������n
//      stTerAuxPara = RunPara_GetTerAuxPara();
//      if(memcmp(pstDataItem->ucDataContent,stTerAuxPara.ucManufactor,4)!=0)
//          ucRet=RET_ERROR;
    }
//azh
    if(ucRet==RET_SUCCESS)
    {
        switch(usItemLabel)     //Fn
        {
//      case CON_EXTCMD_F1:         //F1 �ڴ��
//          ucRet=Main_Recv_ExtCmd_F1_Mem_Read(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F2:         //F2 �ڴ�д
//          ucRet=Main_Recv_ExtCmd_F2_Mem_Write(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F3:         //F3 DATAFLASH��
//          ucRet=Main_Recv_ExtCmd_F3_Dataflash_Read(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F4:         //F4 DATAFLASHд
//          ucRet=Main_Recv_ExtCmd_F4_Dataflash_Write(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F5:         //F5 ���Ź����ԣ��ó��������ѭ��
//          ucRet=Main_Recv_ExtCmd_F5_WdtTest_EnterDeadLoop(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F6:         //F6 �ֻ�ģ��ͨѶ���
//          ucRet=Main_Recv_ExtCmd_F6_GetCommInfo(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F7:         //F7 �ն�״̬���
//          ucRet=Main_Recv_ExtCmd_F7_GetTerErrInfo(pstDataItem,ucDAp);
//          break;
        case CON_EXTCMD_F10:        //F10 �ն˵�ַ10/16�����л�
            ucRet=Main_Recv_ExtCmd_F10_TerExtInfo(pstDataItem,ucDAp);
            break;
//      case CON_EXTCMD_F11:        //F11 ������Ϣ��ȡ
//          ucRet=Main_Recv_ExtCmd_F11_GetShortMessage(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F21:        //F21 GR�����
//          ucRet=Main_Recv_Get_F131_GRTask(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F22:        //F22 GR��������
//          ucRet=Main_Recv_Set_F131_GRTask(pstDataItem->ucDataContent,ucDAp);
//          break;
//      case CON_EXTCMD_F23:        //F23 GR������Ϣ��
//          ucRet=Main_Recv_ExtCmd_F23_GRInfo_Read(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F24:        //F24 GR������Ϣ����
//          ucRet=Main_Recv_ExtCmd_F24_GRInfo_Write(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F31:        //F31 ������������
//          ucRet=Main_Recv_ExtCmd_F31_ClearDemand(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F33:        //F33 ����׶���������ȫ
//          ucRet=Main_Recv_ExtCmd_F33_SetPowDataAll(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F40:        //F40 �ڱ���������
//          ucRet=Main_Recv_ExtCmd_F40_7758SetMiscData(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F41:        //F41 �ڱ����ݶ�ȡ
//          ucRet=Main_Recv_ExtCmd_F41_7758GetMiscData(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F51:        //F51 �ն˵�ַ
//          ucRet=Main_Recv_ExtCmd_F51_Get_TerAddr(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F52:        //F52 �ն���Ŷ�
//          ucRet=Main_Recv_ExtCmd_F52_Get_TerSequNo(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F53:        //F53 �ն˵�ַ��������д
//          ucRet=Main_Recv_ExtCmd_F53_Set_TerAddr(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F54:        //F54 �ն����д
//          ucRet=Main_Recv_ExtCmd_F54_Set_TerSequNo(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F71:        //F71 ����������
//          ucRet=Main_Recv_ExtCmd_F71_Ctrl_Beep(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F72:        //F72 �޸�ʱ��
//          ucRet=Main_Recv_ExtCmd_F72_ChangeTime(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F73:        //F73 ��ѯͨѶ������
//          ucRet=Main_Recv_ExtCmd_F73_Get_CommBoad(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F74:        //F74 �޸�ͨѶ������
//          ucRet=Main_Recv_ExtCmd_F74_Set_CommBoad(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F75:        //F75 ��ȡҺ��������ʾ����
//          ucRet=Main_Recv_ExtCmd_F75_Get_LCDCycle(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F76:        //F76 ��ȡҺ��������ʾ����
//          ucRet=Main_Recv_ExtCmd_F76_Get_LCDCtrl(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F77:        //F77 ��ȡҺ��������ʾ����
//          ucRet=Main_Recv_ExtCmd_F77_Get_LCDSpec(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F78:        //F78 �޸�Һ��������ʾ����
//          ucRet=Main_Recv_ExtCmd_F78_Set_LCDCycle(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F79:        //F79 �޸�Һ��������ʾ����
//          ucRet=Main_Recv_ExtCmd_F79_Set_LCDCtrl(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F80:        //F80 �޸�Һ��������ʾ����
//          ucRet=Main_Recv_ExtCmd_F80_Set_LCDSpec(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F81:        //F81 ����SIM����pin
//          ucRet=Main_Recv_ExtCmd_F81_Sim_StartPin(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F82:        //F82 �޸�SIM����pin
//          ucRet=Main_Recv_ExtCmd_F82_Sim_SetPin(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F83:        //F83 ��PUK����SIM����pin
//          ucRet=Main_Recv_ExtCmd_F83_Sim_PukNewPin(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F84:        //F84 SIM����pin���״̬
//          ucRet=Main_Recv_ExtCmd_F84_Sim_GetPinStat(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F85:        //F85 GSM/GPRS�л�
//          ucRet=Main_Recv_ExtCmd_F85_Set_GSM_GRPS(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F86:        //F86 �����ʿ��ƻָ�ʱ�䳤��
//          ucRet=Main_Recv_ExtCmd_F86_Get_PowerCtrl_TimeRecType(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F87:        //F87 д���ʿ��ƻָ�ʱ�䳤��
//          ucRet=Main_Recv_ExtCmd_F87_Set_PowerCtrl_TimeRecType(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F89:        //F89 �����ʿ���Խ��ȷ��ʱ��
//          ucRet=Main_Recv_ExtCmd_F89_Get_PowerCtrl_DelayTime(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F90:        //F90 д���ʿ���Խ��ȷ��ʱ��
//          ucRet=Main_Recv_ExtCmd_F90_Set_PowerCtrl_DelayTime(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F91:        //F91 ���Զ����������
//          ucRet=Main_Recv_ExtCmd_F91_Get_AdjPara(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F92:        //F92 д�Զ����������
//          ucRet=Main_Recv_ExtCmd_F92_Set_AdjPara(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F93:        //F93 ��ȡ�ն˵�ѹ��λС��������λС��
//          ucRet=Main_Recv_ExtCmd_F93_Get_TerVoltCurr(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F94:        //F94 �����ʼ��
//          ucRet=Main_Recv_ExtCmd_F94_Init_CTask(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F95:        //F95 8025ʱ�ӽ���
//          ucRet=Main_Recv_ExtCmd_F95_Adj_RX8025(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F96:        //F96 Һ���Աȶ�����
//          ucRet=Main_Recv_ExtCmd_F96_Set_LCDContrast(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F97:        //F97 ���ñ���û���������
//          ucRet=Main_Recv_ExtCmd_F97_Set_MeterCommName(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F98:        //F98 ��ȡ����û���������
//          ucRet=Main_Recv_ExtCmd_F98_Get_MeterCommName(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F99:        //F99 ����GPRS�����PDP�û���������
//          ucRet=Main_Recv_ExtCmd_F99_Set_GprsPDPPara(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F100:       //F100 ��ȡGPRS�����PDP�û���������
//          ucRet=Main_Recv_ExtCmd_F100_Get_GprsPDPPara(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F101:       //F101 ����GPRS�������ʱ��
//          ucRet=Main_Recv_ExtCmd_F101_Set_GprsRelinkTime(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F102:       //F102 ��ȡGPRS�������ʱ��
//          ucRet=Main_Recv_ExtCmd_F102_Get_GprsRelinkTime(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F109:       //F109 �����ն˹���ģʽ
//          ucRet=Main_Recv_ExtCmd_F109_Set_TerWorkMode(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F110:       //F110 ��ȡ�ն˹���ģʽ
//          ucRet=Main_Recv_ExtCmd_F110_Get_TerWorkMode(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F111:       //F111 ����CT����
//          ucRet=Main_Recv_ExtCmd_F111_Set_CTType(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F112:       //F112 ��ȡCT����
//          ucRet=Main_Recv_ExtCmd_F112_Get_CTType(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F113:       //F113 ������ѧϰ��Ч��־
//          ucRet=Main_Recv_ExtCmd_F113_Set_SlfStudyFlag(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F114:       //F114 ��ȡ��ѧϰ��Ч��־
//          ucRet=Main_Recv_ExtCmd_F114_Get_SlfStudyFlag(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F115:
//          ucRet=Main_Recv_ExtCmd_F115_Set_Cir2OpenLimitVal(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F116:
//          ucRet=Main_Recv_ExtCmd_F116_Get_Cir2OpenLimitVal(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F117:
//          ucRet=Main_Recv_ExtCmd_F117_Set_Cir1ShortLimitVal(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F118:
//          ucRet=Main_Recv_ExtCmd_F118_Get_Cir1ShortLimitVal(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F119:
//          ucRet=Main_Recv_ExtCmd_F119_Get_CTCheckVal(pstDataItem,ucDAp);
//          break;
//      case CON_EXTCMD_F120:
//          ucRet = Main_Recv_ExtCmd_F120_Set_AESKey(pstDataItem,ucDAp); //F120 ����aes������Կ
//          break;
//      case CON_EXTCMD_F121:
//          ucRet = Main_Recv_ExtCmd_F121_Get_AESKey(pstDataItem,ucDAp); //F121 ��ȡaes������Կ
//          break;
//      case    CON_EXTCMD_F122:
//          ucRet = Main_Recv_ExtCmd_F122_Get_KeepStatus(pstDataItem,ucDAp); //F122���ý��������߱���
//          break;
//      case    CON_EXTCMD_F123:
//          ucRet = Main_Recv_ExtCmd_F123_Get_Temperature(pstDataItem,ucDAp); //F123��ȡ�¶�
//          break;
//      case    CON_EXTCMD_F124:
//          ucRet = Main_Recv_ExtCmd_F124_Get_Ade7758_Para(pstDataItem,ucDAp); //F124 ��ȡУ��EEPROM�Ĳ���
//          break;
//      case    CON_EXTCMD_F125:
//          ucRet = Main_Recv_ExtCmd_F125_Set_Ade7758_Para(pstDataItem,ucDAp); //F125 ����У��EEPROM�Ĳ���
//          break;
//      case    CON_EXTCMD_F126:
//          ucRet = Main_Recv_ExtCmd_F126_Para(pstDataItem,ucDAp);
//          break;
        default:
            break;
        }
    }
    if(ucRet==RET_ERROR)
    {
        pstDataItem->ucExeRet=CON_NACK_ERR_NOTEXE;
        pstDataItem->ucDataContent[4]=CON_NACK_ERR_NOTEXE;
        pstDataItem->usDataLen=5;
    }
    else
    {
        switch(usItemLabel)
        {
        case CON_EXTCMD_F10:
//        case CON_EXTCMD_F31:
//        case CON_EXTCMD_F33:
//        case CON_EXTCMD_F40:
//        case CON_EXTCMD_F53:
//        case CON_EXTCMD_F54:
//        case CON_EXTCMD_F74:
//        case CON_EXTCMD_F78:
//        case CON_EXTCMD_F79:
//        case CON_EXTCMD_F80:
//        case CON_EXTCMD_F85:
//        case CON_EXTCMD_F87:
//        case CON_EXTCMD_F90:
//        case CON_EXTCMD_F92:
//        case CON_EXTCMD_F94:
//        case CON_EXTCMD_F96:
//        case CON_EXTCMD_F97:
//        case CON_EXTCMD_F99:
//        case CON_EXTCMD_F101:
//        case CON_EXTCMD_F109:
//        case CON_EXTCMD_F120:
//        case CON_EXTCMD_F122:
            break;
        }
        pstDataItem->ucExeRet=CON_NACK_ERR_NORMAL;
    }
    return ucRet;
}

//�м�վ����
unsigned char Main_Recv_RelayCmd(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo)
{
    unsigned char ucRet=RET_SUCCESS;

    return ucRet;
}

//�ļ�����
unsigned char Main_Recv_FileTrans(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char ucDAp;
    unsigned short usItemLabel;
    STDATAITEM *pstDataItem;
    pstDataItem=&pstRecvTerMsg->stDataItem[ucDataItemNo];
    ucDAp=pstDataItem->ucMPNo;
    usItemLabel=pstDataItem->usItemLabel;
    if(pstDataItem->ucExeRet!=CON_NACK_ERR_PASSWORD)
    {
        switch(usItemLabel)     //Fn
        {
        case CON_FILETRANS_F1:      //F1 ת������
            ucRet=Main_Recv_FileTrans_F1(pstDataItem,ucDAp,pstRecvTerMsg->ucMsgSource,pstRecvTerMsg->ucSeqNo);
            break;
        case CON_FILETRANS_F100:    //F100 ������װ����
            ucRet=Main_Recv_FileTrans_F100(pstDataItem,ucDAp,pstRecvTerMsg->ucMsgSource,pstRecvTerMsg->ucSeqNo);
            break;
        default:
            break;
        }
        if(ucRet==RET_ERROR)
        {
            pstDataItem->ucExeRet=CON_NACK_ERR_NOTEXE;
            pstDataItem->ucDataContent[0]=CON_NACK_ERR_NOTEXE;
            pstDataItem->usDataLen=1;
        }
        else
            pstDataItem->ucExeRet=CON_NACK_ERR_NORMAL;
    }else
    {
        pstDataItem->ucDataContent[0]=CON_NACK_ERR_PASSWORD;
        pstDataItem->usDataLen=1;
    }
    return ucRet;
}

//����ת��
unsigned char Main_Recv_DataTrans(STTERMSG *pstRecvTerMsg,unsigned char ucDataItemNo)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char ucDAp;
    unsigned short usItemLabel;
    STDATAITEM *pstDataItem;
    pstDataItem=&pstRecvTerMsg->stDataItem[ucDataItemNo];
    ucDAp=pstDataItem->ucMPNo;
    usItemLabel=pstDataItem->usItemLabel;
    if(pstDataItem->ucExeRet!=CON_NACK_ERR_PASSWORD)
    {
        switch(usItemLabel)     //Fn
        {
        case CON_DATATRANS_F1:      //ת������
            ucRet=Main_Recv_DataTrans_F1(pstDataItem,ucDAp,pstRecvTerMsg->ucMsgSource,pstRecvTerMsg->ucSeqNo);
            break;
//      case CON_DATATRANS_F9:
//          ucRet = Main_Recv_DataTrans_F9(pstDataItem,ucDAp,pstRecvTerMsg->ucMsgSource,pstRecvTerMsg->ucSeqNo);
//          break;
        default:
            break;
        }
        if(ucRet==RET_ERROR)
        {
            pstDataItem->ucExeRet=CON_NACK_ERR_NOTEXE;
            pstDataItem->ucDataContent[0]=CON_NACK_ERR_NOTEXE;
            pstDataItem->usDataLen=1;
        }
        else
            pstDataItem->ucExeRet=CON_NACK_ERR_NORMAL;
    }else
    {
        pstDataItem->ucDataContent[0]=CON_NACK_ERR_PASSWORD;
        pstDataItem->usDataLen=1;
    }
    return ucRet;
}

//-------------    ��Լ���������  ��������� -----------------------//
//Ӳ����ʼ��
unsigned char Main_Recv_Reset_Hardware(unsigned char ucMsgSource)
{
    unsigned char ucRet=RET_SUCCESS;
//  STDATETIME stNewTime=GetCurrentTime();
//  ModifyCurrentTime(&stNewTime);
    DataProc_Stat_TerResetNum_Inc();            //��¼�ն˸�λ����
//
//  if(ucMsgSource==CON_TERMSG_SOUR_RCOMM_SMS)
//      g_ucDPEnd_RestartTime=30;   //���Ÿ�λ��Ҫ�ȴ����ű�ɾ�������Ϊ15��
//  else
//      g_ucDPEnd_RestartTime=7;    //---3.5s
//  //  boot();                     //��������
    os_evt_set(EXTCMD_RST_FLAG, System_Manage_Task_ID);

    return ucRet;
}

//��������ʼ��
unsigned char Main_Recv_Reset_Data(unsigned char ucMsgSource)
{
    unsigned char ucRet=RET_SUCCESS;
    STTERAUXPARA    stTerAuxPara=RunPara_GetTerAuxPara();

    memcpy(stTerAuxPara.ucSoftwareVerOld,stTerAuxPara.ucSoftwareVer,4);
    RunPara_SetTerAuxPara(&stTerAuxPara);
    //���ڸ澯�ȵ�ǰ���ݳ�ʼ��
    //��ʷ���ݳ�ʼ�������Կ���ʵ�֣�����ʱ��ָ�뼴��
    RunData_Init_All();
//azh
//  CTask_Rebuild_Task();
//
//  Pulse_InitMeterData();          //������������
//
//  Alarm_InitAlrRecordInfo();
//  Alarm_InitAlrStatus();
//  Alarm_InitAlrHappenWord();
//
//  Alarm_SetFrameACD(CON_STATUS_NO_2BYTE);
//  g_ucEventCount_Important=0;
//  g_ucEventCount_Normal=0;
  Alarm_EnsureJudge_AllDataInit();    //ERC1�����ݳ�ʼ����¼

//  Meter_Init(RunPara_GetUartBoad(1),0);           //�����ʼ��
//  Meter_Init(RunPara_GetUartBoad(4),1);
    return ucRet;
}

//������ȫ����������ʼ��
unsigned char Main_Recv_Reset_All(unsigned char ucMsgSource,unsigned char ucClrFlag)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char ucSoftwareVerOld[4];
    STTERAUXPARA    stTerAuxPara=RunPara_GetTerAuxPara();
    if(ucMsgSource==CON_TERMSG_SOUR_RCOMM_SMS)
        return RET_ERROR;               //���ų�ʼ������F3������

    memcpy(ucSoftwareVerOld,stTerAuxPara.ucSoftwareVer,4);
//  ADEInit_All();
    RunPara_Init_All(ucClrFlag);                    //���в�����ʼ��
    stTerAuxPara=RunPara_GetTerAuxPara();
    memcpy(stTerAuxPara.ucSoftwareVerOld,stTerAuxPara.ucSoftwareVer,4);
    RunPara_SetTerAuxPara(&stTerAuxPara);
    RunData_Init_All();                 //ȫ����������ʼ��

    DP_Set_TerDP_State(CON_TERDP_STATE_NORMAL);
//  TC35_ModuleDown_SetDownState(CON_TERDP_STATE_NORMAL);

//  Alarm_InitAlrRecordInfo();
//  Alarm_InitAlrStatus();
//  Alarm_InitAlrHappenWord();
//  Alarm_SetFrameACD(CON_STATUS_NO_2BYTE);
//  g_ucEventCount_Important=0;
//  g_ucEventCount_Normal=0;
  Alarm_EnsureJudge_AllDataInit();    //ERC1�����ݳ�ʼ����¼

//  Meter_Init(RunPara_GetUartBoad(1),0);           //�����ʼ��
//  Meter_Init(RunPara_GetUartBoad(4),1);
    return ucRet;
}
//F1�ն�ͨѶ��������
unsigned char Main_Recv_Set_F1_CommPara(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned short m=0;

    STTERCOMMPARA   stTerCommPara=RunPara_GetTerCommPara();
    STHEARTBEAT     stHeartBeat=RunPara_GetHeartBeat();

    stTerCommPara.ucDelay = pucBuffer[m++];
    stTerCommPara.ucMaxOvertimeOrig =pucBuffer[m++];
    stTerCommPara.usACKOvertime =pucBuffer[m] + (pucBuffer[m+1]&0x0F)*0x100;
    stTerCommPara.ucReSendtimes =pucBuffer[m+1]>>4;
    stTerCommPara.ucReSendtimes &=0x03;
    m+=2;
    if((pucBuffer[m]&0x01)==0x01)           //һ������
        stTerCommPara.ucServerNeedAck[2]=CON_STATUS_YES;
    else
        stTerCommPara.ucServerNeedAck[2]=CON_STATUS_NO;
    if((pucBuffer[m]&0x02)==0x02)           //��������
        stTerCommPara.ucServerNeedAck[3]=CON_STATUS_YES;
    else
        stTerCommPara.ucServerNeedAck[3]=CON_STATUS_NO;
    if((pucBuffer[m]&0x04)==0x04)           //��������
    {
        stTerCommPara.ucServerNeedAck[0]=CON_STATUS_YES;
        stTerCommPara.ucServerNeedAck[1]=CON_STATUS_YES;
    }
    else
    {
        stTerCommPara.ucServerNeedAck[0]=CON_STATUS_NO;
        stTerCommPara.ucServerNeedAck[1]=CON_STATUS_NO;
    }
    m++;
    stHeartBeat.ucHeartBeat = pucBuffer[m];
    if(stHeartBeat.ucHeartBeat==0)
        stHeartBeat.ucHeartBeat=1;

    RunPara_SetTerCommPara(&stTerCommPara);
    RunPara_SetHeartBeat(&stHeartBeat);

    return ucRet;
}
unsigned char Main_Recv_Set_F2_RelayPara(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet = RET_SUCCESS,ucNum = 0,i;
    unsigned short m =0;
    STLINKREMOVE stLinkRemove = RunPara_GetLinkRemove();

    if((pucBuffer[m] & 0x80)==0x80)
        stLinkRemove.ucFlag = CON_AVALIBLE_FLAG;
    else
        stLinkRemove.ucFlag = CON_INVALIBLE_FLAG;

    ucNum = pucBuffer[m]&0x7F;
    if(ucNum > CON_TRANS_ADDR_NUM)
        ucNum = CON_TRANS_ADDR_NUM;
    m++;
    for(i=0;i<ucNum;i++)
    {
        stLinkRemove.stTransAddr[i].ucDevAddr[0] = pucBuffer[m++];
        stLinkRemove.stTransAddr[i].ucDevAddr[1] = pucBuffer[m++];
    }
    stLinkRemove.ucTransNum = ucNum;
    RunPara_SetLinkRemove(&stLinkRemove);
    return ucRet;
}
//F3����ͨ��IP��ַ
unsigned char Main_Recv_Set_F3_IP_Addr(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned short m=0,i=0;
    STSTAIPADDR stStaIPAddr;
    STMODPARACHINFO stModParaChInfoTmp;

    Get_ModChInfo(&stModParaChInfoTmp);
    for(i=0;i<4;i++)                                        //����ַ
        stStaIPAddr.stBaseIP.ucIPAddr[i]=pucBuffer[m++];
    stStaIPAddr.stBaseIP.usIPPort=pucBuffer[m]+pucBuffer[m+1]*0x100;
    m += 2;

    for(i=0;i<4;i++)                                        //����ַ
        stStaIPAddr.stBackIP.ucIPAddr[i]=pucBuffer[m++];
    stStaIPAddr.stBackIP.usIPPort=pucBuffer[m]+pucBuffer[m+1]*0x100;
    m += 2;

    memcpy(stStaIPAddr.cAPN,pucBuffer+m,16);
    RunPara_SetStaIPAddr(&stStaIPAddr);
    Main_RComm_Init_LoginSleepCount();
    stModParaChInfoTmp.bIsAPNChanged = 1;
    stModParaChInfoTmp.bIsCHInfoChanged = 1;
    Set_ModChInfo(&stModParaChInfoTmp);
    //RComm_SetStartFlag();
    ucIPFrist = 0;
    Reload_Chinfo_Para();       //��ʼ��ģ��ͨѶ����
    return ucRet;
}

//F4������վ����Ͷ������ĺ���
unsigned char Main_Recv_Set_F4_Station_Number(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned short m=0,i=0;
    unsigned char ucTemp;
    STSTAPHONENO stStaPhoneNo;
    STMODPARACHINFO stModParaChInfoTmp;

    RunPara_InitStaPhoneNo(&stStaPhoneNo);

    for(i=0;i<16;i++)
    {
        ucTemp=(pucBuffer[i] & 0x0F);           //ÿλ���ݷ�Χ0----B��0-9�������0----9��AΪ"��"��������ͣ�٣���ʱ����BΪ"#"��FΪ��Ч���
        if((ucTemp>0x0B) && (ucTemp<0x0F))
            return RET_ERROR;
        ucTemp=(pucBuffer[i] >> 4);
        if((ucTemp>0x0B) && (ucTemp<0x0F))
            return RET_ERROR;
    }

    memcpy(stStaPhoneNo.stPhoneNo.ucPhoneNo,pucBuffer+m,8);
    m+=8;
    memcpy(stStaPhoneNo.stSMSCenter.ucPhoneNo,pucBuffer+m,8);

    stModParaChInfoTmp.bIsSMSCenterChanged = 1;         //100416����������Ϣ���������õ�ͨѶģ����
    stModParaChInfoTmp.bIsSMSNoChanged = 1;
    Set_ModChInfo(&stModParaChInfoTmp);
    RunPara_SetStaPhoneNo(&stStaPhoneNo);
    Reload_SCA_Para();
    return ucRet;
}

//F5��������
unsigned char Main_Recv_Set_F5_TerEncrypt(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS;
//  unsigned short  m=0;
//  STTERENCRYPT    stTerEncrypt;
//  stTerEncrypt=RunPara_GetTerEncrypt();
//
//  stTerEncrypt.ucAlgoCode=pucBuffer[m++];
//  stTerEncrypt.ucPassword[0]=pucBuffer[m++];
//  stTerEncrypt.ucPassword[1]=pucBuffer[m++];
//
//  RunPara_SetTerEncrypt(&stTerEncrypt);
//  CRC16_Init_Table();
    return ucRet;
}

//F6�ն����ַ
unsigned char Main_Recv_Set_F6_TerGroupAddr(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned short m=0,i=0;
    STTERGROUPADDRLIST  stTerGroupAddrList;
    RunPara_InitTerGroupAddrList(&stTerGroupAddrList);

    for(i=0;i<CON_GROUPADDRNUM;i++)
    {
        stTerGroupAddrList.stGroupAddr[i].ucGroupAddr[0] = pucBuffer[m++];
        stTerGroupAddrList.stGroupAddr[i].ucGroupAddr[1] = pucBuffer[m++];
    }

    RunPara_SetTerGroupAddrList(&stTerGroupAddrList);
    return ucRet;
}
//F7�ն˱���IP��ַ�����������
unsigned char Main_Recv_Set_F7_IPAddr(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char  ucRet=RET_SUCCESS;
    unsigned short m=0,i;
    STLOCALIP stLocalIP;

    for(i=0;i<4;i++)
        stLocalIP.stLocalIP.ucIPAddr[i] = pucBuffer[m++];
    for(i=0;i<4;i++)
        stLocalIP.stSubMask.ucIPAddr[i] = pucBuffer[m++];
    for(i=0;i<4;i++)
        stLocalIP.stGateWay.ucIPAddr[i] = pucBuffer[m++];
    stLocalIP.stAgentServer.ucServeType = pucBuffer[m++];
    if(stLocalIP.stAgentServer.ucServeType != CON_SERVE_TYPE_NO && stLocalIP.stAgentServer.ucServeType != CON_SERVE_TYPE_HTTP &&
        stLocalIP.stAgentServer.ucServeType != CON_SERVE_SOCKS4 && stLocalIP.stAgentServer.ucServeType != CON_SERVE_SOCKS5)
        return RET_ERROR;
    for(i=0;i<4;i++)
        stLocalIP.stAgentServer.stServeIP.ucIPAddr[i] = pucBuffer[m++];
    stLocalIP.stAgentServer.stServeIP.usIPPort = pucBuffer[m] + (pucBuffer[m+1]<<8);
    m += 2;
    stLocalIP.stAgentServer.ucLinkType = pucBuffer[m++];
    if(stLocalIP.stAgentServer.ucLinkType != CON_SERVE_NOVERIFY && stLocalIP.stAgentServer.ucLinkType != CON_SERVE_VERIFY)
        return RET_ERROR;
    stLocalIP.stAgentServer.ucUsrNameLen = pucBuffer[m++];
    if(stLocalIP.stAgentServer.ucUsrNameLen != 0)
    {
        memcpy(stLocalIP.stAgentServer.ucUsrName,pucBuffer+m,stLocalIP.stAgentServer.ucUsrNameLen);
        m += stLocalIP.stAgentServer.ucUsrNameLen;
    }
    stLocalIP.stAgentServer.ucPwdLen = pucBuffer[m++];
    if(stLocalIP.stAgentServer.ucPwdLen != 0)
    {
        memcpy(stLocalIP.stAgentServer.ucPwd,pucBuffer+m,stLocalIP.stAgentServer.ucPwdLen);
        m += stLocalIP.stAgentServer.ucPwdLen;
    }
    stLocalIP.stLocalIP.usIPPort = pucBuffer[m] + (pucBuffer[m+1]<<8);
    RunPara_SetTerLocalIP(&stLocalIP);

    return ucRet;
}
//F9�¼��澯����
unsigned char Main_Recv_Set_F9_AlarmControlWord(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned short m=0;
    STALRCTRLWORD   stAlrCtrlWord;
    RunPara_InitAlrCtrlWord(&stAlrCtrlWord);

    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucTerDataInit      ,(unsigned char)(pucBuffer[m] & 0x01),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucTerParaLost      ,(unsigned char)(pucBuffer[m] & 0x02),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucTerParaChange    ,(unsigned char)(pucBuffer[m] & 0x04),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucStatChange       ,(unsigned char)(pucBuffer[m] & 0x08),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucRemoteCtrl       ,(unsigned char)(pucBuffer[m] & 0x10),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucPowerCtrl        ,(unsigned char)(pucBuffer[m] & 0x20),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucEnergyCtrl       ,(unsigned char)(pucBuffer[m] & 0x40),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucMeterParaChange  ,(unsigned char)(pucBuffer[m] & 0x80),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);

    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucTAErr            ,(unsigned char)(pucBuffer[m+1] & 0x01),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucTVErr            ,(unsigned char)(pucBuffer[m+1] & 0x02),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucPhaseErr         ,(unsigned char)(pucBuffer[m+1] & 0x04),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucMeterTimeErr     ,(unsigned char)(pucBuffer[m+1] & 0x08),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucMeterErrInfo     ,(unsigned char)(pucBuffer[m+1] & 0x10),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucTerPowOff        ,(unsigned char)(pucBuffer[m+1] & 0x20),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucHarmWaveOver     ,(unsigned char)(pucBuffer[m+1] & 0x40),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucDirSimuValOver   ,(unsigned char)(pucBuffer[m+1] & 0x80),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);

    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucUnbalanceOver    ,(unsigned char)(pucBuffer[m+2] & 0x01),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucCapLockSelf      ,(unsigned char)(pucBuffer[m+2] & 0x02),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucBuyPowPara       ,(unsigned char)(pucBuffer[m+2] & 0x04),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucPassErr          ,(unsigned char)(pucBuffer[m+2] & 0x08),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucTerErr           ,(unsigned char)(pucBuffer[m+2] & 0x10),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
//    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucSumCompOverLimit ,(unsigned char)(pucBuffer[m+2] & 0x20),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucEnerAlr          ,(unsigned char)(pucBuffer[m+2] & 0x40),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucVolOverUp        ,(unsigned char)(pucBuffer[m+2] & 0x80),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);

    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucCurOverUp        ,(unsigned char)(pucBuffer[m+3] & 0x01),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucSPowOverUp       ,(unsigned char)(pucBuffer[m+3] & 0x02),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucMeterDown        ,(unsigned char)(pucBuffer[m+3] & 0x04),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucPowDiffOver      ,(unsigned char)(pucBuffer[m+3] & 0x08),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucMeterFly         ,(unsigned char)(pucBuffer[m+3] & 0x10),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucMeterStop        ,(unsigned char)(pucBuffer[m+3] & 0x20),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucMeterCopy        ,(unsigned char)(pucBuffer[m+3] & 0x40),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrShield.ucFluxLimit        ,(unsigned char)(pucBuffer[m+3] & 0x80),CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);

    m+=8;
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucTerDataInit     ,(unsigned char)(pucBuffer[m] & 0x01),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucTerParaLost     ,(unsigned char)(pucBuffer[m] & 0x02),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucTerParaChange   ,(unsigned char)(pucBuffer[m] & 0x04),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucStatChange      ,(unsigned char)(pucBuffer[m] & 0x08),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucRemoteCtrl      ,(unsigned char)(pucBuffer[m] & 0x10),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucPowerCtrl       ,(unsigned char)(pucBuffer[m] & 0x20),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucEnergyCtrl      ,(unsigned char)(pucBuffer[m] & 0x40),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucMeterParaChange ,(unsigned char)(pucBuffer[m] & 0x80),CON_ALR_ACD_NOT,CON_ALR_ACD);

    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucTAErr           ,(unsigned char)(pucBuffer[m+1] & 0x01),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucTVErr           ,(unsigned char)(pucBuffer[m+1] & 0x02),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucPhaseErr        ,(unsigned char)(pucBuffer[m+1] & 0x04),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucMeterTimeErr    ,(unsigned char)(pucBuffer[m+1] & 0x08),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucMeterErrInfo    ,(unsigned char)(pucBuffer[m+1] & 0x10),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucTerPowOff       ,(unsigned char)(pucBuffer[m+1] & 0x20),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucHarmWaveOver    ,(unsigned char)(pucBuffer[m+1] & 0x40),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucDirSimuValOver  ,(unsigned char)(pucBuffer[m+1] & 0x80),CON_ALR_ACD_NOT,CON_ALR_ACD);

    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucUnbalanceOver   ,(unsigned char)(pucBuffer[m+2] & 0x01),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucCapLockSelf     ,(unsigned char)(pucBuffer[m+2] & 0x02),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucBuyPowPara      ,(unsigned char)(pucBuffer[m+2] & 0x04),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucPassErr         ,(unsigned char)(pucBuffer[m+2] & 0x08),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucTerErr          ,(unsigned char)(pucBuffer[m+2] & 0x10),CON_ALR_ACD_NOT,CON_ALR_ACD);
//    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucSumCompOverLimit,(unsigned char)(pucBuffer[m+2] & 0x20),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucEnerAlr         ,(unsigned char)(pucBuffer[m+2] & 0x40),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucVolOverUp       ,(unsigned char)(pucBuffer[m+2] & 0x80),CON_ALR_ACD_NOT,CON_ALR_ACD);

    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucCurOverUp       ,(unsigned char)(pucBuffer[m+3] & 0x01),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucSPowOverUp      ,(unsigned char)(pucBuffer[m+3] & 0x02),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucMeterDown       ,(unsigned char)(pucBuffer[m+3] & 0x04),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucPowDiffOver     ,(unsigned char)(pucBuffer[m+3] & 0x08),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucMeterFly        ,(unsigned char)(pucBuffer[m+3] & 0x10),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucMeterStop       ,(unsigned char)(pucBuffer[m+3] & 0x20),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucMeterCopy       ,(unsigned char)(pucBuffer[m+3] & 0x40),CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryVal(&stAlrCtrlWord.stAlrACD.ucFluxLimit       ,(unsigned char)(pucBuffer[m+3] & 0x80),CON_ALR_ACD_NOT,CON_ALR_ACD);

    RunPara_SetAlrCtrlWord(&stAlrCtrlWord);

    return ucRet;
}

//F10���ն˵��ܱ�/��������װ�ò���
unsigned char Main_Recv_Set_F10_MeterPara(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS,i=0;
    unsigned short m=0;
    unsigned char ucMeterNum=0,ucMPNo=0,ucMPNoOld=0,ucMeterNo=0;
//    unsigned char ucMPType=CON_MPTYPE_NULL;
    unsigned char ucFlag=CON_STATUS_NO;
    STMETERPARA stMeterPara;                    //������
    STDATETIME  stCurrentTime=GetCurrentTime(); //ȡ��ǰʱ��
    STTERNORMALPARA stTerNormalPara=RunPara_GetTerNormalPara();
//    unsigned char  ucTerMPNo=0,ucTerNo=0;

    ucMeterNum = pucBuffer[m++] ;
    if((ucMeterNum > CON_METER_NUM) || (pucBuffer[m++] != 0))//��������װ�����������ж�
        ucRet = RET_ERROR;
    else
    {
        for(i=0;i<ucMeterNum;i++)
        {
            ucMeterNo=pucBuffer[m]-1;           //���ţ����ܱ�/��������װ�����
            if(pucBuffer[m]>CON_METER_NUM || pucBuffer[m+1] != 0)
            {
                ucRet=RET_ERROR;
                break;
            }
            ucMPNo=pucBuffer[m+2];              //��������
            if(pucBuffer[m+2]>CON_MAX_MPNUM || pucBuffer[m+3] != 0)
            {
                ucRet=RET_ERROR;
                break;
            }
            if((pucBuffer[m+4] & 0x01f) == 0)   //�˿ں�
            {
                ucRet=RET_ERROR;
                break;
            }
            memcpy(stMeterPara.stMeterAddr.ucMeterAddr,pucBuffer+m+6,6);    //��ΪABB���ַ��16���Ƶģ����Բ�����BCD����ĵ�ַ
            if(i<(ucMeterNum-1) && ucMeterNo>=pucBuffer[m+27])              //�����ж�װ����Ҫ�������
            {
                ucRet=RET_ERROR;
                break;
            }
            m+=27;
        }
        if(ucRet==RET_SUCCESS)
        {
            m=2;
            for(i=0;i<ucMeterNum;i++)
            {
                ucMeterNo=pucBuffer[m++]-1;                 //�ⲿ��1��ʼ���ڲ���0��ʼ
                m++;
//              ucMPNo=pucBuffer[m++]-1;                    //�ⲿ��1��ʼ���ڲ���0��ʼ
                ucMPNo = pucBuffer[m++];                    //�п���Ϊ0��ɾ�������ڲ��ټ�
                m++;
                stMeterPara=RunPara_GetMeterPara(ucMeterNo);
                stMeterPara.ucPort=pucBuffer[m++]-1;
//                if(pucBuffer[m]==CON_EXT_PROTOCOL_SAMPLE)
//                {//�����ɼ�װ��
//                    if(ucMPNo != 0)
//                    {
//                        ucMPNo -= 1;
//                    //  stTerNormalPara.ucSamplePort=stMeterPara.ucPort;
//                        stTerNormalPara.ucSamplePort=1;
//                        m++;        //ͨ��Э������
//                        memcpy(stTerNormalPara.ucCommAddr,pucBuffer+m,6);       m+=6;
//                        memcpy(stTerNormalPara.ucCommPass,pucBuffer+m,6);       m+=6;
//                        m ++;       //���ʸ���
//                        stTerNormalPara.ucDataTypeInfo=pucBuffer[m++];//�й�����ʾֵ����λ��С��λ����
//                        RunPara_SetTerNormalPara(&stTerNormalPara);
//
//                        ucTerMPNo   = RunPara_GetTer_MPNo();
//                        ucTerNo = RunPara_GetMPDevNo(ucTerMPNo);
//                        if(ucTerNo!=ucMeterNo)
//                        {//�豸�ű仯��ɾ���ϵĽ����ɼ�
//                            RunPara_SetMPType(ucTerMPNo,CON_MPTYPE_NULL);
//                        }
//
//                        ucMPNoOld=RunPara_GetMPNo_byDevNo(ucMeterNo,CON_MPTYPE_TER);
//                        if(ucMPNoOld!=CON_MPNO_INVALID)
//                            RunPara_SetMPType(ucMPNoOld,CON_MPTYPE_NULL);
//                        RunPara_SetMPType(ucMPNo,CON_MPTYPE_TER);
//                        RunPara_SetMPDevNo(ucMPNo,ucMeterNo);
//    //                  if(ucMPNoOld!=ucMPNo)
//    //                      SetDefaultTerTask();                //������仯����Ҫ��������GR����
//
////azh                       SetDefaultTerTask();                //���þͳ�ʼ��
//
// //                       DataProc_TerDayData_Stat(stCurrentTime);
////                        DataProc_TerMonthData_Stat(stCurrentTime);
//                        m += 7;
//                    }else       //ɾ��
//                    {
//                        ucTerMPNo   = RunPara_GetTer_MPNo();
//                        ucTerNo = RunPara_GetMPDevNo(ucTerMPNo);
//                        if(ucTerNo!=ucMeterNo)
//                            RunPara_SetMPType(ucTerMPNo,CON_MPTYPE_NULL);
//                        ucMPNoOld=RunPara_GetMPNo_byDevNo(ucMeterNo,CON_MPTYPE_TER);
//                        if(ucMPNoOld!=CON_MPNO_INVALID)
//                            RunPara_SetMPType(ucMPNoOld,CON_MPTYPE_NULL);
//                    }
//                }else
//                {//���ܱ�
//azh �������Ȳ����ǽ��ɲ����� ����Ѳ�����1�̶���Ϊ��ѹ�������
                    if(ucMPNo != 0)
                    {
                        ucMPNo -= 1;
                        stMeterPara.ucBaudCode = (stMeterPara.ucPort >> 5);
                        stMeterPara.ucPort = (stMeterPara.ucPort & 0x01F);
//azh 141009
//                        if(stMeterPara.ucPort==2)       //��1��ʼת��Ϊ��0��ʼ
//                            stMeterPara.ucPort=1;
//                        else
//                            stMeterPara.ucPort=0;
                        stMeterPara.ucProtocol=RunPara_GetInnerMeterProtocol(pucBuffer[m]);
                        RunPara_GetDefaultMeterCommPara(&stMeterPara,stMeterPara.ucProtocol);
                        if(stMeterPara.ucBaudCode!=0)                           //0Ĭ��
                            stMeterPara.ulBaudRate=RunPara_Get_Relay_Boadrate(stMeterPara.ucBaudCode);
                        m++;
                        memcpy(stMeterPara.stMeterAddr.ucMeterAddr,pucBuffer+m,6);      m+=6;
                        memcpy(stMeterPara.stPassword.ucMeterPass,pucBuffer+m,6);       m+=6;
                        stMeterPara.ucSetFeeNum   = pucBuffer[m++];
                        stMeterPara.ucFeeNum   = 4;
                        stMeterPara.ucFloatNum  = (pucBuffer[m]&0x03)+1;
                        stMeterPara.ucIntNum = ((pucBuffer[m]>>2)&0x03)+4;
                        m++;
    //                  memcpy(stMeterPara.ucMeterJuNum,pucBuffer+m,7);
    //                  m+=7;
                        memcpy(stMeterPara.ucCollectAddr,pucBuffer+m,CON_COLLECT_ADDR_LEN);
                        m += CON_COLLECT_ADDR_LEN;
                        stMeterPara.ucBigClass = (pucBuffer[m]&0xF0)>>4;
                        stMeterPara.ucLittleClass = pucBuffer[m]&0x0F;
                        m ++;
                        ucMPNoOld=RunPara_GetMPNo_byDevNo(ucMeterNo,CON_MPTYPE_METER);
                        if(ucMPNoOld!=CON_MPNO_INVALID)
                            RunPara_SetMPType(ucMPNoOld,CON_MPTYPE_NULL);
//                        ucMPType=RunPara_GetMPType(ucMPNo);
//azh                        if(ucMPType==CON_MPTYPE_TER)                        //�����Ҫ�����ն�ģ������Ӧ�Ĳ������
//                            RunPara_SetMPType(CON_MPNO_TER,CON_MPTYPE_TER);
                        RunPara_SetMPType(ucMPNo,CON_MPTYPE_METER);
                        RunPara_SetMPDevNo(ucMPNo,ucMeterNo);
                        RunPara_SetMeterPara(ucMeterNo,&stMeterPara);
                        //���ó�������
                       if(stMeterPara.ucProtocol==CON_PROTOCOL_INVALID)
                          DeleteDefaultMeterTask(ucMeterNo);
                      else
                          SetDefaultMeterTask(ucMeterNo);
                        DataProc_DayData_Stat(stCurrentTime);                           //������ͳ�ƣ�ÿ���Ӵ���
                        DataProc_DayFrozenData_Stat(stCurrentTime);
//                      DataProc_SumGDayData_Stat(stCurrentTime);
                        DataProc_MonthData_Stat(stCurrentTime);                         //������ͳ�ƣ���������ͳ�Ƴ�ʼ��֮ǰ
//                      DataProc_SumGMonthData_Stat(stCurrentTime);
                        RunData_Meter_Init(ucMeterNo);                                  //��ǰ���ݳ�ʼ��
                        ucFlag=CON_STATUS_YES;
#if CODE_VER_TYPE==H_METER_PRJ		//��ϸ�ѹ��
						if((ucMPNo==0)&&(ucMeterNo==0))//azh �ⲿ������ǵ����źͲ�����ͬΪ1��ʱ����Ϊ�󶨵ĸ�ѹ��
                        {
                            init_search_data();//������Զ��ѱ�״̬�͵�������ʾ״̬
                            memcpy(gMatch_HVMeter_Addr, stMeterPara.stMeterAddr.ucMeterAddr, PAR_HVMETER_ADR_LEN);//�����ȫ00 �򲻻�ȥzigbee���� ���Ҽ��������Ƕ� �Ͳ��������ѱ�
                            gMatch_HVMeter_Addr[PAR_HVMETER_ADR_LEN] = addverify(gMatch_HVMeter_Addr, PAR_HVMETER_ADR_LEN);
                            file_write(PAR_HVMETER_ADR_ADDR, gMatch_HVMeter_Addr, PAR_HVMETER_ADR_LEN+1);
                            memset(gMatch_Zigbee_Addr, 0xff, PAR_HVMETER_ZIGBEE_ADR_LEN);//���ڵ�һ֡zigbee�㲥��ȡ��ַ
                            gMatch_Zigbee_Addr[PAR_HVMETER_ZIGBEE_ADR_LEN] = addverify(gMatch_Zigbee_Addr, PAR_HVMETER_ZIGBEE_ADR_LEN);
                            file_write(PAR_HVMETER_ZIGBEE_ADR_ADDR, gMatch_Zigbee_Addr, PAR_HVMETER_ZIGBEE_ADR_LEN+1);                            
                            Save_MP_ParaEE(ucMeterNo,&stMeterPara);//azh 140313
                        }    
#else
//GPRSģ�� 171011
	#ifdef ZIGBEE_TEST
					if((ucMPNo==0)&&(ucMeterNo==0))//azh �ⲿ������ǵ����źͲ�����ͬΪ1��ʱ����Ϊ�󶨵ĸ�ѹ��
                    {
                        init_search_data();//������Զ��ѱ�״̬�͵�������ʾ״̬
                        memcpy(gMatch_HVMeter_Addr, stMeterPara.stMeterAddr.ucMeterAddr, PAR_HVMETER_ADR_LEN);//�����ȫ00 �򲻻�ȥzigbee���� ���Ҽ��������Ƕ� �Ͳ��������ѱ�
                        gMatch_HVMeter_Addr[PAR_HVMETER_ADR_LEN] = addverify(gMatch_HVMeter_Addr, PAR_HVMETER_ADR_LEN);
                        file_write(PAR_HVMETER_ADR_ADDR, gMatch_HVMeter_Addr, PAR_HVMETER_ADR_LEN+1);
                        memset(gMatch_Zigbee_Addr, 0xff, PAR_HVMETER_ZIGBEE_ADR_LEN);//���ڵ�һ֡zigbee�㲥��ȡ��ַ
                        gucGetZigbeeAddrFlag = 1;//azh 171019
                        gMatch_Zigbee_Addr[PAR_HVMETER_ZIGBEE_ADR_LEN] = addverify(gMatch_Zigbee_Addr, PAR_HVMETER_ZIGBEE_ADR_LEN);
                        file_write(PAR_HVMETER_ZIGBEE_ADR_ADDR, gMatch_Zigbee_Addr, PAR_HVMETER_ZIGBEE_ADR_LEN+1);                            
                    }
	
	#endif//ZIGBEE_TEST
						Save_MP_ParaEE(ucMeterNo,&stMeterPara);//azh 140313

#endif
                    }else           //ɾ��
                    {
                        ucMPNoOld=RunPara_GetMPNo_byDevNo(ucMeterNo,CON_MPTYPE_METER);
                        if(ucMPNoOld!=CON_MPNO_INVALID)
                        {    
                            RunPara_SetMPType(ucMPNoOld,CON_MPTYPE_NULL);
                        }
#ifdef ZIGBEE_TEST//CODE_VER_TYPE==H_METER_PRJ		//��ϸ�ѹ�� //azh 171011 ��ϴ�zigbeeģ��� �����ʽ��ѹ��
                        if(ucMeterNo==0)//azh ɾ���Ļ� �Ѹ�ѹ���ַ��� zigbee���ᷢ������
                        {
                            memset(gMatch_HVMeter_Addr, 0, PAR_HVMETER_ADR_LEN);//�����ȫ00 �򲻻�ȥzigbee���� ���Ҽ��������Ƕ� �Ͳ��������ѱ�
                            gMatch_HVMeter_Addr[PAR_HVMETER_ADR_LEN] = addverify(gMatch_HVMeter_Addr, PAR_HVMETER_ADR_LEN);
                            file_write(PAR_HVMETER_ADR_ADDR, gMatch_HVMeter_Addr, PAR_HVMETER_ADR_LEN+1);
                            memset(gMatch_Zigbee_Addr, 0xff, PAR_HVMETER_ZIGBEE_ADR_LEN);//���ڵ�һ֡zigbee�㲥��ȡ��ַ
                            gMatch_Zigbee_Addr[PAR_HVMETER_ZIGBEE_ADR_LEN] = addverify(gMatch_Zigbee_Addr, PAR_HVMETER_ZIGBEE_ADR_LEN);
                            file_write(PAR_HVMETER_ZIGBEE_ADR_ADDR, gMatch_Zigbee_Addr, PAR_HVMETER_ZIGBEE_ADR_LEN+1);
                            RunData_Meter_Init(ucMeterNo);                                  //��ǰ���ݳ�ʼ��
                            
                            memcpy(stMeterPara.stMeterAddr.ucMeterAddr,gMatch_HVMeter_Addr,6);
                            RunPara_SetMeterPara(ucMeterNo,&stMeterPara);
                            Save_MP_ParaEE(ucMeterNo,&stMeterPara);//azh 140313
                        }        
#endif//ZIGBEE_TEST
                    }
//                }
            }
            if(ucFlag==CON_STATUS_YES)
            {
//azh       Meter_Init(stMeterPara.ulBaudRate,0);                                   //��ʼ�����ڲ���Ϊ���һ���
//          Meter_Init(stMeterPara.ulBaudRate,1);
//azh               
				CopyMeter_Timer_StartNow();
            }
        }
    }

    return ucRet;
}

////F11���ն��������ò���
//unsigned char Main_Recv_Set_F11_PulsePara(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//	unsigned char ucTmp,i=0,j=0;
//	unsigned short m=0;
//	unsigned char ucPulseNum=0,ucMPNo=0,ucMPNoOld=0,ucPulseNo=0,ucMPNoBak=CON_MPNO_INVALID;
//	STPULSEPARA	stPulsePara;
//	STDATETIME	stCurrentTime=GetCurrentTime();
//	unsigned char ucOldAttr,ucAttrBak=CON_PULSEATTR_INVALID;
//	
//	ucPulseNum = pucBuffer[m++];
//	if(ucPulseNum > CON_PULSE_NUM)
//		ucRet=RET_ERROR;
//	else
//	{
//		for(i=0;i<ucPulseNum;i++)
//		{
//			ucPulseNo=pucBuffer[m]-1;
//			//			if(pucBuffer[m]==0 || pucBuffer[m]>CON_PULSE_NUM)
//			if(pucBuffer[m]>CON_PULSE_NUM)		//�˿ں�0��ʾɾ����Ӧ���������������
//			{
//				ucRet=RET_ERROR;
//				break;
//			}
//			ucMPNo=pucBuffer[m+1];
//			if(pucBuffer[m+1]==0 || pucBuffer[m+1]>CON_MAX_MPNUM)
//			{
//				ucRet=RET_ERROR;
//				break;
//			}
//			if(i<(ucPulseNum-1) && pucBuffer[m]!=0 && pucBuffer[m+5]!=0 && ucPulseNo>=pucBuffer[m+5])
//			{
//				ucRet=RET_ERROR;
//				break;
//			}
//			m+=5;
//		}
//		if(ucRet==RET_SUCCESS)
//		{
//			m=1;
//			for(i=0;i<ucPulseNum;i++)
//			{
//				ucPulseNo=pucBuffer[m++]-1;
//				ucMPNo=pucBuffer[m++]-1;
//				stPulsePara=RunPara_GetPulsePara(ucPulseNo);
//				stPulsePara.ucPortNo=ucPulseNo;
//				
//				ucOldAttr=stPulsePara.ucAttr;
//				//�����������				
//				ucTmp=pucBuffer[m++]&0x03;
//				if(ucTmp == 0x00)
//					stPulsePara.ucAttr=CON_PULSEATTR_DIRPOW;
//				else if(ucTmp == 0x01)
//					stPulsePara.ucAttr=CON_PULSEATTR_DIRVAR;
//				else if(ucTmp == 0x02)
//					stPulsePara.ucAttr=CON_PULSEATTR_REVPOW;
//				else
//					stPulsePara.ucAttr=CON_PULSEATTR_REVVAR;
//				stPulsePara.usConst = pucBuffer[m] + pucBuffer[m+1]*0x100;
//				m+=2;
//				if(ucPulseNo==0xFF)								//�˿ں�0����ɾ����Ӧ������������������
//				{
//					if(RunPara_GetMPType(ucMPNo)==CON_MPTYPE_PULSE)
//					{	
//						RunPara_SetPulseMPDevNo(ucMPNo,0xFF,stPulsePara.ucAttr);
//						if(RunPara_CheckPulseMPDevNo_IsNone(ucMPNo)==CON_STATUS_YES)
//							RunPara_SetMPType(ucMPNo,CON_MPTYPE_NULL);
//					}
//				}else
//				{
//					ucMPNoOld=RunPara_GetPulse_MPNo(ucPulseNo);
//					if(ucMPNoOld!=CON_MPNO_INVALID)
//					{
//						if(ucMPNoOld!=ucMPNoBak)
//						{
//							RunPara_SetMPType(ucMPNoOld,CON_MPTYPE_NULL);
//							RunPara_SetPulseMPDevNo(ucMPNoOld,0xFF,ucOldAttr);			//ɾ����λ��
//						}else if(ucOldAttr!=ucAttrBak)
//							RunPara_SetPulseMPDevNo(ucMPNoOld,0xFF,ucOldAttr);			//ɾ����λ��
//					}
//					if(RunPara_GetMPType(ucMPNo)!=CON_MPTYPE_PULSE)
//					{//ԭ����������壬������豸��
//						RunPara_SetPulseMPDevNo(ucMPNo,0xFF,CON_PULSEATTR_DIRPOW);
//						RunPara_SetPulseMPDevNo(ucMPNo,0xFF,CON_PULSEATTR_REVPOW);
//						RunPara_SetPulseMPDevNo(ucMPNo,0xFF,CON_PULSEATTR_DIRVAR);
//						RunPara_SetPulseMPDevNo(ucMPNo,0xFF,CON_PULSEATTR_REVVAR);
//					}
//					RunPara_SetMPType(ucMPNo,CON_MPTYPE_PULSE);
//					RunPara_SetPulseMPDevNo(ucMPNo,ucPulseNo,stPulsePara.ucAttr);
//					RunPara_SetPulsePara(ucPulseNo,&stPulsePara);
////azh					if(ucMPNoOld!=ucMPNo)						//������仯����Ҫ��������GR�������峣���仯����Ҫ������������
////						SetDefaultPulseTask(ucPulseNo);
//					
//					for(j=0;j<5;j++)
//					{
//						Set_PulseMeterBase_DirPow(ucPulseNo,j,0);		//��ʼ�����õ׶�Ϊ0
//						Set_PulseMeterBase_DirVar(ucPulseNo,j,0);
//						Set_PulseMeterBase_RevPow(ucPulseNo,j,0);
//						Set_PulseMeterBase_RevVar(ucPulseNo,j,0);
//					}
//					Set_PulseCount_AllSubNow(ucPulseNo);
//					Make_PulseMonthChange(ucPulseNo);
//					
//					DataProc_PulseDayData_Stat(stCurrentTime);
//					DataProc_PulseMonthData_Stat(stCurrentTime);
//					
//					ucMPNoBak=ucMPNo;
//					ucAttrBak=stPulsePara.ucAttr;
//				}
//			}
//		}
//	}
//    return ucRet;
//}
//
////F12���ն�״̬���������
//unsigned char Main_Recv_Set_F12_DInputPara(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned short m=0;
//  unsigned char ucConnect,ucAttr;
//  unsigned char ucPulseNo,j=0,i=0;
//  STSWITCHPARA stSwitchPara;
//  STDATETIME  stCurrentTime=GetCurrentTime();
//  ucConnect=pucBuffer[0];
//  ucAttr=pucBuffer[1];
//  ucAlarm=pucBuffer[2];
//  ucAlarm=0XFF;
//  for(i=0;i<8;i++)
//  {
//      if((ucConnect&0x01)==0x01)
//      {
//          if((ucAttr&0x01)==0x01)
//              stSwitchPara.ucAttr=CON_SWITCHATTR_TYPEA;       //a
//          else
//              stSwitchPara.ucAttr=CON_SWITCHATTR_TYPEB;       //b
//          if((ucAlarm&0x01)==0x01)
//              stSwitchPara.ucAlarm=CON_SWITCHALARM_IMP;
//          else
//              stSwitchPara.ucAlarm=CON_SWITCHALARM_NOR;
//          //���帴����·
//          if(i>=4)
//          {
//              ucPulseNo=i-4;
//              for(j=0;j<5;j++)
//              {
//                  Set_PulseMeterBase_DirPow(ucPulseNo,j,0);       //��ʼ�����õ׶�Ϊ0
//                  Set_PulseMeterBase_DirVar(ucPulseNo,j,0);
//                  Set_PulseMeterBase_RevPow(ucPulseNo,j,0);
//                  Set_PulseMeterBase_RevVar(ucPulseNo,j,0);
//              }
//              Set_PulseCount_AllSubNow(ucPulseNo);
//              Make_PulseMonthChange(ucPulseNo);
//
//              DataProc_PulseDayData_Stat(stCurrentTime);
//              DataProc_PulseMonthData_Stat(stCurrentTime);
//          }
//      }else
//      {
//          stSwitchPara.ucAttr=CON_SWITCHATTR_INVALID;
//          stSwitchPara.ucAlarm=CON_SWITCHALARM_IMP;
//      }
//      Set_Switch_Change_Info(0);          //100331���ù�״̬��״̬��λ����
//      RunPara_SetSwitchPara(i,&stSwitchPara);
//      ucConnect=ucConnect>>1;
//      ucAttr=ucAttr>>1;
//      ucAlarm=ucAlarm>>1;
//  }
//    return ucRet;
//}
//
////F13���ն˵�ѹ/����ģ�������ò���
//unsigned char Main_Recv_Set_F13_AnalogPara(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned short m=1;
//  unsigned char ucAnalogNum=0,i,ucPortNo,ucPortOldNo;
//  STANALOGPARA stAnalogPara;
//  ucAnalogNum=pucBuffer[0];
//  if(ucAnalogNum==0 || ucAnalogNum>8)
//      ucRet=RET_ERROR;
//  if(ucRet==RET_SUCCESS)
//  {
//      for(i=0;i<ucAnalogNum;i++)
//      {
//          if(pucBuffer[m]>CON_ANALOG_NUM)     //��1��ʼ��0��ʾɾ��
//          {
//              ucRet=RET_ERROR;
//              break;
//          }
//          if(i<(ucAnalogNum-1) && pucBuffer[m]!=0 && pucBuffer[m+3]!=0 && pucBuffer[m]>=pucBuffer[m+3])
//          {
//              ucRet=RET_ERROR;
//              break;
//          }
//          m+=3;
//      }
//  }
//  if(ucRet==RET_SUCCESS)
//  {
//      m=1;
//      for(i=0;i<ucAnalogNum;i++)
//      {
//          ucPortNo=pucBuffer[m++]-1;                          //�ڲ���0��ʼ
//          if(ucPortNo==0xFF)
//          {
//              stAnalogPara.ucMPNo=pucBuffer[m++];
//              stAnalogPara.ucAttr=pucBuffer[m++]%6;               //����0--5��Ч
//              ucPortOldNo=RunPara_GetAnalogPortNo(stAnalogPara.ucMPNo,stAnalogPara.ucAttr);
//              if(ucPortOldNo!=0xFF)
//              {//�ҵ���ɾ��
//                  stAnalogPara.ucMPNo=CON_MPNO_INVALID;
//                  RunPara_SetAnalogPara(ucPortOldNo,&stAnalogPara);
//              }
//          }else
//          {
//              stAnalogPara.ucMPNo=pucBuffer[m++];
//              stAnalogPara.ucAttr=pucBuffer[m++]%6;               //����0--5��Ч
//              RunPara_SetAnalogPara(ucPortNo,&stAnalogPara);
//          }
//      }
//  }
//    return ucRet;
//}
//
////F14���ն��ܼ������ò���
//unsigned char Main_Recv_Set_F14_SumGPara(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char   ucRet=RET_SUCCESS;
//  unsigned short  m=0;
//  unsigned char   ucSumGNum=0,ucSumGNo=0,i=0,j=0;
//  unsigned char   ucMPNum=0,ucMPNo=0,ucTmp;
//  unsigned char   ucDirNo=0,ucRevNo=0,ucOper=0;
//
//  STSUMGROUPPARA  stSumGParaTmp;
//  ucSumGNum = pucBuffer[m++];
//  if(ucSumGNum > CON_SUMGROUP_NUM)
//      ucRet = RET_ERROR;
//  else
//  {
//      for(i=0;i<ucSumGNum;i++)
//      {
//          if(pucBuffer[m]>CON_SUMGROUP_NUM)               //1--8
//          {
//              ucRet=RET_ERROR;
//              break;
//          }
//          ucSumGNo=pucBuffer[m++]-1;
//          RunPara_InitSumGroupPara(&stSumGParaTmp);
//          ucMPNum=pucBuffer[m++];
//          ucDirNo=0;
//          ucRevNo=0;
//          for(j=0;j<ucMPNum;j++)
//          {
//              ucTmp=pucBuffer[m++];
//              ucMPNo=(ucTmp&0x3F);
//              if((ucTmp&0x40)==0)
//              {
//                  if((ucTmp&0x80)==0)
//                      ucOper=CON_SUMGOPER_ADD;
//                  else
//                      ucOper=CON_SUMGOPER_SUB;
//                  RunPara_SetSumGroupOper(ucDirNo,&(stSumGParaTmp.stInstantPow.stDir),ucMPNo,ucOper);
//                  RunPara_SetSumGroupOper(ucDirNo,&(stSumGParaTmp.stInstantVar.stDir),ucMPNo,ucOper);
//                  RunPara_SetSumGroupOper(ucDirNo,&(stSumGParaTmp.stPowVal.stDir),ucMPNo,ucOper);
//                  RunPara_SetSumGroupOper(ucDirNo,&(stSumGParaTmp.stVarVal.stDir),ucMPNo,ucOper);
//                  ucDirNo++;
//              }else
//              {
//                  if((ucTmp&0x80)==0)
//                      ucOper=CON_SUMGOPER_ADD;
//                  else
//                      ucOper=CON_SUMGOPER_SUB;
//                  RunPara_SetSumGroupOper(ucRevNo,&(stSumGParaTmp.stInstantPow.stRev),ucMPNo,ucOper);
//                  RunPara_SetSumGroupOper(ucRevNo,&(stSumGParaTmp.stInstantVar.stRev),ucMPNo,ucOper);
//                  RunPara_SetSumGroupOper(ucRevNo,&(stSumGParaTmp.stPowVal.stRev),ucMPNo,ucOper);
//                  RunPara_SetSumGroupOper(ucRevNo,&(stSumGParaTmp.stVarVal.stRev),ucMPNo,ucOper);
//                  ucRevNo++;
//              }
//          }
//          RunPara_SetSumGroupParaOper(ucSumGNo,&stSumGParaTmp);
//          RunPara_SetSumGroupAvailableFalg(ucSumGNo,CON_AVALIBLE_FLAG);
//      }
//  }
//    return ucRet;
//}
//
////F15���й��ܵ������Խ���¼���������
//unsigned char Main_Recv_Set_F15_PowValDiff(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char   ucRet=RET_SUCCESS;//i=0;
//  unsigned char   m=0,ucNum;
//  STTERMISCPARA   stTerMiscPara;
//  ucNum=pucBuffer[m++];
//  if(ucNum>8 || ucNum==0)
//      ucRet = RET_ERROR;
//  else
//  {
//      for(i=0;i<ucNum;i++)
//      {
//          m+=5;
//          if((Check_BCD_Data(pucBuffer[m])!=RET_SUCCESS) ||
//              (Check_BCD_Data(pucBuffer[m+1])!=RET_SUCCESS) ||
//              (Check_BCD_Data(pucBuffer[m+2])!=RET_SUCCESS) ||
//              (Check_BCD_Data((unsigned char)(pucBuffer[m+3]&0x0F))!=RET_SUCCESS))
//          {
//              ucRet=RET_ERROR;
//              break;
//          }
//          m+=4;
//      }
//      if(ucRet==RET_SUCCESS)
//      {
//          m=1;
//          stTerMiscPara=RunPara_GetTerMiscPara();
//          for(i=0;i<ucNum;i++)
//          {
//              stTerMiscPara.stPowDiffLimit[i].ucSumGNo=pucBuffer[m++]-1;
//              stTerMiscPara.stPowDiffLimit[i].ucSumGNoComp=pucBuffer[m++]-1;
//              stTerMiscPara.stPowDiffLimit[i].ucSumGNoReff=pucBuffer[m++]-1;
//              stTerMiscPara.stPowDiffLimit[i].ucFlag=pucBuffer[m++]&0x83;
//              stTerMiscPara.stPowDiffLimit[i].ucDiffLimit=pucBuffer[m++];
//              stTerMiscPara.stPowDiffLimit[i].dDiffLimitVal=DT3_To_Double(pucBuffer+m);
//              stTerMiscPara.stPowDiffLimit[i].ucSign = pucBuffer[m+3]&0x40;
//              m+=4;
//
//              if(stTerMiscPara.stPowDiffLimit[i].ucSumGNoComp == 0xFF ||stTerMiscPara.stPowDiffLimit[i].ucSumGNoReff == 0xFF)     //Ϊ0ɾ��
//                  stTerMiscPara.stPowDiffLimit[i].ucSumGNo=0xFF;
//          }
//          for(i=ucNum;i<CON_MAX_DIRPOWTOTAL_COMPNUM;i++)
//              stTerMiscPara.stPowDiffLimit[i].ucSumGNo=0xFF;
//          ucRet=RunPara_SetTerMiscPara(&stTerMiscPara);
//      }
//  }
//    return ucRet;
//}
//
////F16������ר���û���������
//unsigned char Main_Recv_Set_F16_PDPInfo(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//    STTERMISCPARA stTerMiscPara=RunPara_GetTerMiscPara();
//    memset(stTerMiscPara.stRCommPara.stGprsPDPPara.ucPDPUserName,0x00,CON_MAX_GPRSPDPUSERNAME_LEN);
//    memset(stTerMiscPara.stRCommPara.stGprsPDPPara.ucPDPPassWord,0x00,CON_MAX_GPRAPDPPASSWORE_LEN);
//    memcpy(stTerMiscPara.stRCommPara.stGprsPDPPara.ucPDPUserName,pucBuffer,CON_MAX_GPRSPDPUSERNAME_LEN);
//    memcpy(stTerMiscPara.stRCommPara.stGprsPDPPara.ucPDPPassWord,pucBuffer+CON_MAX_GPRSPDPUSERNAME_LEN,CON_MAX_GPRAPDPPASSWORE_LEN);
//
//    if((stTerMiscPara.stRCommPara.stGprsPDPPara.ucPDPUserName[0]>0x20)          //���ݵ�һ���ֽڽ����ж��Ƿ�����
//        && (stTerMiscPara.stRCommPara.stGprsPDPPara.ucPDPUserName[0]<0x7F))
//        stTerMiscPara.stRCommPara.stGprsPDPPara.ucPDPUseFlag=0x01;      //1����
//    else
//        stTerMiscPara.stRCommPara.stGprsPDPPara.ucPDPUseFlag=0x00;      //0����
//
//    RunPara_SetTerMiscPara(&stTerMiscPara);
//
//    return ucRet;
//}

////F17���ն˱�����ֵ
//unsigned char Main_Recv_Set_F17_KeepPower(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  STLOADFIXVAL    stSafeFixVal;
//  float fVal;
//  unsigned short usDT2;
//  ucRet=Check_Data_DT02(pucBuffer);
//  if(ucRet==RET_SUCCESS)
//  {
//      stSafeFixVal.usFloatCoef=100;
//      usDT2=pucBuffer[0]+0x100*pucBuffer[1];
//      fVal=DT2_To_Float(usDT2);
//
//      stSafeFixVal.dFixVal=fVal;
//      stSafeFixVal.usDT2=usDT2;
//      ucRet=RunPara_SetCtrlSafeFixVal(&stSafeFixVal);
//  }
//  return ucRet;
//}
//
////F18���ն˹���ʱ��
//unsigned char Main_Recv_Set_F18_PowCtrlTimePer(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned char i=0,j=0,h=0,ucTimerPerCount=0;
//  unsigned char m=0;
//  unsigned char ucTmp,ucFlag1,ucFlag2;
//  STPOWERCTRLTIME stPowerCtrlTime,stPowerCtrlTimeTmp;
//  ucTimerPerCount=0;
//  ucFlag1=0xFF;
//  for(i=0;i<12;i++)
//  {
//      ucTmp=pucBuffer[m];
//      for(j=0;j<4;j++)
//      {
//          ucFlag2=ucTmp&0x03;
//          if(ucFlag1!=ucFlag2)
//          {
//              stPowerCtrlTimeTmp.stPowerCtrlPer[ucTimerPerCount].ucAvailbleFlag=CON_AVALIBLE_FLAG;
//              if(ucFlag2==0 || ucFlag2==3)
//                  stPowerCtrlTimeTmp.stPowerCtrlPer[ucTimerPerCount].ucCtrlFlag=CON_INVALIBLE_FLAG;
//              else
//                  stPowerCtrlTimeTmp.stPowerCtrlPer[ucTimerPerCount].ucCtrlFlag=CON_AVALIBLE_FLAG;
//              stPowerCtrlTimeTmp.stPowerCtrlPer[ucTimerPerCount].ucBeginHour=(i<<1)+(j>>1);
//              stPowerCtrlTimeTmp.stPowerCtrlPer[ucTimerPerCount].ucBeginMinute=(j&0x01)*30;
//              ucTimerPerCount++;
//              ucFlag1=ucFlag2;
//          }
//          if(ucTimerPerCount>=CONSWITCHPERTIMENUM)            //ֻ���ǰ16��ʱ��
//              break;
//          ucTmp=(ucTmp>>2);
//      }
//      m++;
//      if(ucTimerPerCount>=CONSWITCHPERTIMENUM)
//          break;
//  }
//  for(j=ucTimerPerCount;j<CONSWITCHPERTIMENUM;j++)            //δ��������ʱ������Ϊ��Ч
//  {
//      stPowerCtrlTimeTmp.stPowerCtrlPer[j].ucAvailbleFlag=CON_INVALIBLE_FLAG;
//      stPowerCtrlTimeTmp.stPowerCtrlPer[j].ucCtrlFlag=CON_INVALIBLE_FLAG;
//  }
//  for(h=0;h<CON_SUMGROUP_NUM;h++)
//  {
//      for(i=CONSWITCHCURVE_TIMEPER1;i<=CONSWITCHCURVE_TIMEPER3;i++)
//      {
//          stPowerCtrlTime=RunPara_GetPowCtrlTimePer(i,h);
//          for(j=0;j<CONSWITCHPERTIMENUM;j++)
//          {//�����޸Ĺ������ߵ�ʱ����Ϣ
//              stPowerCtrlTime.stPowerCtrlPer[j].ucAvailbleFlag=stPowerCtrlTimeTmp.stPowerCtrlPer[j].ucAvailbleFlag;
//              stPowerCtrlTime.stPowerCtrlPer[j].ucCtrlFlag=stPowerCtrlTimeTmp.stPowerCtrlPer[j].ucCtrlFlag;
//              stPowerCtrlTime.stPowerCtrlPer[j].ucBeginHour=stPowerCtrlTimeTmp.stPowerCtrlPer[j].ucBeginHour;
//              stPowerCtrlTime.stPowerCtrlPer[j].ucBeginMinute=stPowerCtrlTimeTmp.stPowerCtrlPer[j].ucBeginMinute;
//          }
//          memcpy(stPowerCtrlTime.ucTimerPerFlag,pucBuffer,12);
//          RunPara_SetPowCtrlTimePer(i,&stPowerCtrlTime,h);
//      }
//  }
//  return ucRet;
//}
//
////F19���ն�ʱ�ι��ض�ֵ����ϵ��
//unsigned char Main_Recv_Set_F19_PowCtrlCoef(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned char i=0,j=0,h=0,ucTmp=0;
//  STPOWERCTRLTIME stPowerCtrlTime;
//  unsigned short usCoef;
//  ucTmp=(0x7F&pucBuffer[0]);
//  ucRet=Check_BCD_Data(ucTmp);
//  if(ucRet==RET_SUCCESS)
//  {
//      ucTmp=(unsigned char)BCD_TO_HEX_1(&ucTmp);
//      if((pucBuffer[0]&0x80)==0x80)
//          usCoef=100-ucTmp;
//      else
//          usCoef=100+ucTmp;
//      for(h=0;h<CON_SUMGROUP_NUM;h++)
//      {
//          for(i=CONSWITCHCURVE_TIMEPER1;i<=CONSWITCHCURVE_TIMEPER3;i++)
//          {
//              stPowerCtrlTime=RunPara_GetPowCtrlTimePer(i,h);
//              for(j=0;j<CONSWITCHPERTIMENUM;j++)
//                  stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.usFloatCoef=usCoef;      //�����޸�ʱ�ι��ʿ������ߵĸ���ϵ����Ϣ
//              RunPara_SetPowCtrlTimePer(i,&stPowerCtrlTime,h);
//          }
//      }
//  }
//  return ucRet;
//}
//
////F20���ն��µ������ض�ֵ����ϵ��
//unsigned char Main_Recv_Set_F20_EnerCtrlCoef(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned char ucTmp;
//  unsigned char i=0;
//  unsigned short usCoef;
//  STENERGYCTRL stEnergyCtrl;
//  ucTmp=(0x7F&pucBuffer[0]);
//  ucRet=Check_BCD_Data(ucTmp);
//  if(ucRet==RET_SUCCESS)
//  {
//      ucTmp=(unsigned char)BCD_TO_HEX_1(&ucTmp);
//      if((pucBuffer[0]&0x80)==0x80)
//          usCoef=100-ucTmp;
//      else
//          usCoef=100+ucTmp;
//      for(i=0;i<CON_SUMGROUP_NUM;i++)
//      {
//          stEnergyCtrl=RunPara_GetEnergyCtrl(i);
//          stEnergyCtrl.stMonthEnergy.usFloatCoef=usCoef;
//          RunPara_SetEnergyCtrl(&stEnergyCtrl,i);
//      }
//  }
//  return ucRet;
//}
//
////F21���ն˵���������ʱ�κͷ�����
//unsigned char Main_Recv_Set_F21_PerTimeFee(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned char m=0;
//  unsigned char ucNum,ucTmp,ucNo,ucFeeNum,i=0;
//  STCTRLFEERATETIME   stCtrlFeeRateTime;
//  ucFeeNum=pucBuffer[48];
//  if(ucFeeNum>CON_CTRLFEE_NUM)
//      ucRet=RET_ERROR;
//  ucNum=CON_CTRLFEETIME_NUM;
//  for(i=0;i<ucNum;i++)
//  {
//      ucTmp=pucBuffer[m];
//      if(ucTmp>=ucFeeNum )
//      {
//          ucRet=RET_ERROR;
//          break;
//      }
//      m++;
//  }
//  if(ucRet==RET_SUCCESS)
//  {
//      m=0;
//      ucNo=0;
//      for(i=0;i<ucNum;i++)
//      {
//
//          stCtrlFeeRateTime.ucFeeRateNo[ucNo++] = pucBuffer[m++];
//      }
//      stCtrlFeeRateTime.ucFeeNum=pucBuffer[m];
//      RunPara_SetCtrlFeeRateTime(&stCtrlFeeRateTime);
//      //�����ڱ�Ĳ���
//      Ter_ADESetFeeTimePer(&stCtrlFeeRateTime);
//  }
//    return ucRet;
//}
//
////F22���ն˵���������
//unsigned char Main_Recv_Set_F22_FeePara(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//    unsigned char i=0;
//  unsigned char m=0;
//  unsigned char ucTmp[4];
//  STCTRLFEERATE   stCtrlFeeRate;
//  unsigned long ulVal;
//  stCtrlFeeRate.ucFeeNum = pucBuffer[m++];
//  for(i=0;i<stCtrlFeeRate.ucFeeNum;i++)
//  {
//      if(Check_Data_DT03(pucBuffer+m)==RET_ERROR)
//      {
//          ucRet=RET_ERROR;
//          break;
//      }
//      m+=4;
//  }
//  if(ucRet==RET_SUCCESS)
//  {
//      m=1;
//      for(i=0;i<stCtrlFeeRate.ucFeeNum;i++)
//      {
//          memcpy(ucTmp,pucBuffer+m,4);
//          ucTmp[3]=ucTmp[3]&0x0F;
//          ulVal=BCD_TO_HEX_4(ucTmp);
//          if((pucBuffer[m+3]&0x40)==0x40)
//          {
//              ulVal*=1000;
//              stCtrlFeeRate.ucUnitFlag = 1;
//          }else
//              stCtrlFeeRate.ucUnitFlag = 0;
//
//          stCtrlFeeRate.ulFeeRate[i]=ulVal;
//          m+=4;
//      }
//      ucRet=RunPara_SetCtrlFeeRate(&stCtrlFeeRate);
//  }
//     return ucRet;
// }

////F23���ն˴߷Ѹ澯����
//unsigned char Main_Recv_Set_F23_UrgeAlarmPara(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//    STTERMISCPARA   stTerMiscPara;
//    stTerMiscPara=RunPara_GetTerMiscPara();
//    memcpy(stTerMiscPara.ucUrgeFeeTime,pucBuffer,3);
//    ucRet=RunPara_SetTerMiscPara(&stTerMiscPara);
//    return ucRet;
//}
//
////F24���ն˳���������
//unsigned char Main_Recv_Set_F24_CopyInter(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	STTERNORMALPARA	stTerNormalPara=RunPara_GetTerNormalPara();
//	STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
//	
//	if(stTerNormalPara.ucCopyInter>2 && pucBuffer[0]<=2)
//	{
//		if(stTerAdjPara.ucRunMode!=0xA5)
//		{
//			stTerAdjPara.ucRunMode=0xA5;
//			RunPara_SetTerAdjPara(&stTerAdjPara);
//			CTask_Rebuild_Task();
//		}
//	}
//	if(stTerNormalPara.ucCopyInter<=2 && pucBuffer[0]>2)
//	{
//		if(stTerAdjPara.ucRunMode==0xA5)
//		{
//			stTerAdjPara.ucRunMode=0x00;
//			RunPara_SetTerAdjPara(&stTerAdjPara);
//			CTask_Rebuild_Task();
//		}
//	}
//	stTerNormalPara.ucCopyInter=pucBuffer[0];
//	/*�������ȡ������ͬ������ʹ���������������
//	if(stTerNormalPara.ucCopyInter>=15)
//	stTerNormalPara.ucCopyInter=15;
//	else if(stTerNormalPara.ucCopyInter<=3)		//�˴�����С�ڣ���С�ڣ���Ӱ����ͣ�ߡ����ߵı���
//	stTerNormalPara.ucCopyInter=3;
//	*/
//	ucRet=RunPara_SetTerNormalPara(&stTerNormalPara);
//	if(ucRet==RET_SUCCESS)
//	{
//		;
//		//���³�����
//	}
//	return ucRet;
//}

//F25�������������
unsigned char Main_Recv_Set_F25_MPBasePara(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS,ucTmp;
    unsigned short m=0;
    unsigned short usTmp;
    unsigned char ucMPNo=0;
    STMPALTERRATE   stMPAlterRate;
    STMPRATEVAL     stMPRateVal;
//  unsigned char ucSumGNo;
//  STENERGYCTRL  stEnergyCtrl;

    ucMPNo= ucDAp;
    if(ucMPNo >= CON_MAX_MPNUM)
        ucRet =RET_ERROR;
    if(ucRet==RET_SUCCESS)
    {
        ucRet=Check_Data_DT07(pucBuffer+4);
        if(ucRet==RET_SUCCESS)
            ucRet=Check_BCD_Data(pucBuffer[6]);
        if(ucRet==RET_SUCCESS)
            ucRet=Check_Data_DT23(pucBuffer+7);
    }
    if(ucRet==RET_SUCCESS)
    {
        stMPAlterRate.usVolAlterRate = pucBuffer[m]+0x100*pucBuffer[m+1];   m+=2;
        stMPAlterRate.usCurAlterRate = pucBuffer[m]+0x100*pucBuffer[m+1];   m+=2;
        usTmp=(unsigned short)BCD_TO_HEX_2(pucBuffer+m);    m+=2;
        stMPRateVal.usRateVol=usTmp;        //0.1  V
        usTmp=(unsigned short)BCD_TO_HEX_1(pucBuffer+m);    m+=1;   //0.1A
        stMPRateVal.usRateCur=usTmp*10;     //0.01A
        stMPRateVal.ulRateP=BCD_TO_HEX_3(pucBuffer+m);      m+=3;   //0.0001
        ucTmp = pucBuffer[m]&0x03;
        if(ucTmp ==0x01)
            stMPRateVal.ucMPLineType=CON_METERTYPE_3P3L;
        else if(ucTmp==0x02)
            stMPRateVal.ucMPLineType=CON_METERTYPE_3P4L;
        else if(ucTmp ==0x03)
            stMPRateVal.ucMPLineType =CON_METERTYPE_1P;
        else
            ucRet=RET_ERROR;
        if(stMPRateVal.ucMPLineType == CON_METERTYPE_1P)
        {
            ucTmp = (pucBuffer[m] >>2)&0x03;
            if(ucTmp == 0)
                stMPRateVal.ucMPPhaseNo = CON_METER_PHASE_INCERTERN;
            else if(ucTmp == 0x01)
                stMPRateVal.ucMPPhaseNo = CON_METER_PHASEA;
            else if(ucTmp == 0x02)
                stMPRateVal.ucMPPhaseNo = CON_METER_PHASEB;
            else
                stMPRateVal.ucMPPhaseNo = CON_METER_PHASEC;
        }
        if(ucRet==RET_SUCCESS)
        {
            RunPara_SetMPRateVal(ucMPNo,&stMPRateVal);
            RunPara_SetMPAlterRate(ucMPNo,&stMPAlterRate);
//azh
//          ucSumGNo=Load_GetEnergyCtrl_SumGNo();
//          if(ucSumGNo>=CON_SUMGROUP_NUM)
//          {
//              ucSumGNo=0;
//              Load_SetEnergyCtrl_SumGNo(ucSumGNo);
//          }
//          stEnergyCtrl=RunPara_GetEnergyCtrl(ucSumGNo);
//          if(stEnergyCtrl.ucEnergyCtrlType==CON_ENERGYCTRL_MON)
//          {
//              Add_History_SumValPowValueGroup();      //�ܼ����������¼���
//              Load_SetEnergy_StaPowVal_LM();
//          }
//          else
//          {
//              stEnergyCtrl.stBuyEnergy.dFixVal = Load_Energy_LeftPowVal_CalcBuy(&stEnergyCtrl,ucSumGNo);
//              RunPara_SetEnergyCtrl(&stEnergyCtrl,ucSumGNo);
//              Add_History_SumValPowValueGroup();      //�ܼ����������¼���
//              Load_SetEnergy_StaPowVal();
//          }
        }
    }
    return ucRet;
}

//F26��������ֵ
unsigned char Main_Recv_Set_F26_MPLimit(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS,i=0;
    unsigned short m=0;
    unsigned char ucMPNo=0;
//  unsigned short usTmp=0;
//  unsigned char sign;
    STMPLIMIT   stMPLimit;
//  unsigned char  ucJudgeCount=0,ucMPType=0;
    STTERALARMJUDGECOUNT stMaxCount = RunPara_GetAlrMaxCount();

    ucMPNo= ucDAp;
    if(ucMPNo >= CON_MAX_MPNUM)
        ucRet =RET_ERROR;
    if(ucRet==RET_SUCCESS)
    {
        for(i=0;i<3;i++)
        {
            ucRet=Check_Data_DT07(pucBuffer+m);
            m+=2;
            if(ucRet==RET_ERROR)
                break;
        }
        if(ucRet==RET_SUCCESS)
        {
            for(i=0;i<2;i++)
            {
                ucRet=Check_Data_DT07(pucBuffer+m);
                m+=3;
                if(ucRet==RET_SUCCESS)
                {
                    ucRet=Check_Data_DT05(pucBuffer+m);
                    m+=2;
                    if(ucRet==RET_ERROR)
                        break;
                }
                else
                    break;
            }
        }
        if(ucRet==RET_SUCCESS)
        {
            for(i=0;i<3;i++)
            {
                ucRet=Check_Data_DT25(pucBuffer+m);
                m+=4;
                if(ucRet==RET_SUCCESS)
                {
                    ucRet=Check_Data_DT05(pucBuffer+m);
                    m+=2;
                    if(ucRet==RET_ERROR)
                        break;
                }
                else
                    break;
            }
        }
        if(ucRet==RET_SUCCESS)
        {
            for(i=0;i<2;i++)
            {
                ucRet=Check_Data_DT23(pucBuffer+m);
                m+=4;
                if(ucRet==RET_SUCCESS)
                {
                    ucRet=Check_Data_DT05(pucBuffer+m);
                    m+=2;
                    if(ucRet==RET_ERROR)
                        break;
                }
                else
                    break;
            }
        }
        if(ucRet==RET_SUCCESS)
        {
            for(i=0;i<2;i++)
            {
                ucRet=Check_Data_DT05(pucBuffer+m);
                m+=3;
                if(ucRet==RET_SUCCESS)
                {
                    ucRet=Check_Data_DT05(pucBuffer+m);
                    m+=2;
                    if(ucRet==RET_ERROR)
                        break;
                }
                else
                    break;
            }
        }
    }
    if(ucRet==RET_SUCCESS)
    {
        m=0;

        stMPLimit.usVolUpLimit  =(unsigned short)BCD_TO_HEX_2(pucBuffer+m);
        stMPLimit.usVolUpLimit_2 = stMPLimit.usVolUpLimit;
        stMPLimit.usVolUpLimit /= 10;
        m+=2;
        stMPLimit.usVolDownLimit  =(unsigned short)BCD_TO_HEX_2(pucBuffer+m);
        stMPLimit.usVolDownLimit_2 = stMPLimit.usVolDownLimit;
        stMPLimit.usVolDownLimit /= 10;
        m+=2;
        stMPLimit.usVolBreakLimit    =(unsigned short)BCD_TO_HEX_2(pucBuffer+m);
        stMPLimit.usVolBreakLimit_2 = stMPLimit.usVolBreakLimit;
        stMPLimit.usVolBreakLimit /= 10;
        m+=2;

        stMPLimit.usVolErrUpLimit   =(unsigned short)BCD_TO_HEX_2(pucBuffer+m);
        stMPLimit.usVolErrUpLimit_2 = stMPLimit.usVolErrUpLimit;
        stMPLimit.usVolErrUpLimit /= 10;
        m+=2;
        stMPLimit.ucVolErrUpTime = pucBuffer[m++] ;
        stMPLimit.ucVolErrUpTime *= 3;
        Main_GetXishu(pucBuffer+m,&(stMPLimit.ucVolErrUpBack));
        m+=2;

        stMPLimit.usVolErrDownLimit=(unsigned short)BCD_TO_HEX_2(pucBuffer+m);
        stMPLimit.usVolErrDownLimit_2 = stMPLimit.usVolErrDownLimit;
        stMPLimit.usVolErrDownLimit /= 10;
        m+=2;
        stMPLimit.ucVolErrDownTime = pucBuffer[m++];
        stMPLimit.ucVolErrDownTime *= 3;
        Main_GetXishu(pucBuffer+m,&(stMPLimit.ucVolErrDownBack));
        m+=2;

        pucBuffer[m+2]&=0x7F;
        stMPLimit.usCurErrUpLimit   =(unsigned short)BCD_TO_HEX_3(pucBuffer+m);
        m+=3;
        stMPLimit.usCurErrUpLimit /= 10;
        stMPLimit.ucCurErrUpTime = pucBuffer[m++];
        stMPLimit.ucCurErrUpTime *= 3;
        Main_GetXishu(pucBuffer+m, &(stMPLimit.ucCurErrUpBack));
//      stMPLimit.ucCurErrUpBack =(unsigned short)BCD_TO_HEX_2(pucBuffer+m);
        m+=2;

        pucBuffer[m+2]&=0x7F;
        stMPLimit.usCurUpLimit      =(unsigned short)BCD_TO_HEX_3(pucBuffer+m);
        stMPLimit.usCurUpLimit  /= 10;
        m+=3;
        stMPLimit.ucCurUpTime = pucBuffer[m++];
        stMPLimit.ucCurUpTime *= 3;
        Main_GetXishu(pucBuffer+m,&(stMPLimit.ucCurUpBack));
//      stMPLimit.ucCurUpBack =(unsigned short)BCD_TO_HEX_2(pucBuffer+m);
        m+=2;

        pucBuffer[m+2]&=0x7F;
        stMPLimit.usCur0UpLimit     =(unsigned short)BCD_TO_HEX_3(pucBuffer+m);
        m+=3;
        stMPLimit.usCur0UpLimit /= 10;
        stMPLimit.ucCur0UpTime = pucBuffer[m++];
        stMPLimit.ucCur0UpTime *=3;
        Main_GetXishu(pucBuffer+m, &(stMPLimit.ucCur0UpBack));
//      stMPLimit.ucCur0UpBack =(unsigned short)BCD_TO_HEX_2(pucBuffer+m);
        m+=2;

        stMPLimit.ulPowErrUpLimit   =BCD_TO_HEX_3(pucBuffer+m);
        m+=3;
        stMPLimit.ucPowErrUpTime = pucBuffer[m++];
        stMPLimit.ucPowErrUpTime *=3;
        Main_GetXishu(pucBuffer+m, &(stMPLimit.ucPowErrUpBack));
//      stMPLimit.ucPowErrUpBack =(unsigned short)BCD_TO_HEX_2(pucBuffer+m);
        m+=2;

        stMPLimit.ulPowUpLimit      =BCD_TO_HEX_3(pucBuffer+m);
        m+=3;
        stMPLimit.ucPowUpTime = pucBuffer[m++];
        stMPLimit.ucPowUpTime *=3;
        Main_GetXishu(pucBuffer+m, &(stMPLimit.ucPowUpBack));
//      stMPLimit.ucPowUpBack =(unsigned short)BCD_TO_HEX_2(pucBuffer+m);
        m+=2;

        pucBuffer[m+1]&=0x7F;
        stMPLimit.usVolUnBalance    =(unsigned short)BCD_TO_HEX_2(pucBuffer+m);
        m+=2;
        stMPLimit.ucVolUnBalanceTime  = pucBuffer[m++];
        stMPLimit.ucVolUnBalanceTime  *= 3;
        Main_GetXishu(pucBuffer+m, &(stMPLimit.ucVolUnBalanceBack));
//      stMPLimit.ucVolUnBalanceBack  = (unsigned short)BCD_TO_HEX_2(pucBuffer+m);
        m+=2;

        pucBuffer[m+1]&=0x7F;
        stMPLimit.usCurUnBalance    =(unsigned short)BCD_TO_HEX_2(pucBuffer+m);
        m+=2;
        stMPLimit.ucCurUnBalanceTime  = pucBuffer[m++];
        stMPLimit.ucCurUnBalanceTime  *= 3;
        Main_GetXishu(pucBuffer+m, &(stMPLimit.ucCurUnBalanceBack));
//      stMPLimit.ucCurUnBalanceBack  = (unsigned short)BCD_TO_HEX_2(pucBuffer+m);
        m+=2;

        stMPLimit.usLostVolTime     =pucBuffer[m];
        m+=1;
        RunPara_SetMPLimit(ucMPNo,&stMPLimit);
//      ucMPType=RunPara_GetMPType(ucMPNo);
//      if(ucMPType==CON_MPTYPE_TER)
//      {//��������ʧѹʱ��
//          ucJudgeCount=(unsigned char)(stMPLimit.usLostVolTime)*3;        //��λ20��
//          RunPara_SetTerVolLackAlrMaxCount(ucJudgeCount);
//
//          stMaxCount.ucAlrCount_CurAErrUp = stMPLimit.ucCurErrUpTime;
//          stMaxCount.ucAlrCount_CurBErrUp = stMPLimit.ucCurErrUpTime;
//          stMaxCount.ucAlrCount_CurCErrUp = stMPLimit.ucCurErrUpTime;
//
//          stMaxCount.ucAlrCount_VolAErrUp = stMPLimit.ucVolErrUpTime;
//          stMaxCount.ucAlrCount_VolBErrUp = stMPLimit.ucVolErrUpTime;
//          stMaxCount.ucAlrCount_VolCErrUp = stMPLimit.ucVolErrUpTime;
//
//          stMaxCount.ucAlrCount_VolAErrDown= stMPLimit.ucVolErrDownTime;
//          stMaxCount.ucAlrCount_VolBErrDown= stMPLimit.ucVolErrDownTime;
//          stMaxCount.ucAlrCount_VolCErrDown= stMPLimit.ucVolErrDownTime;
//
//          stMaxCount.ucAlrCount_CurANorUp = stMPLimit.ucCurUpTime;
//          stMaxCount.ucAlrCount_CurBNorUp = stMPLimit.ucCurUpTime;
//          stMaxCount.ucAlrCount_CurCNorUp = stMPLimit.ucCurUpTime;
//
//          stMaxCount.ucAlrCount_SPowErrUp = stMPLimit.ucPowErrUpTime;
//          stMaxCount.ucAlrCount_SPowNorUp = stMPLimit.ucPowUpTime;
//          stMaxCount.ucAlrCount_UUnBalance = stMPLimit.ucVolUnBalanceTime;
//          stMaxCount.ucAlrCount_IUnBalance = stMPLimit.ucCurUnBalanceTime;
//          RunPara_SetAlrMaxCount(&stMaxCount);
//      }//����Ԥ��ʱ��øò���
    }

    return ucRet;
}

//F27������ͭ���������
unsigned char Main_Recv_Set_F27_MPCuFePara(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS;
    unsigned char   i=0;
    unsigned short  m=0;
    unsigned char   ucMPNo=0;
    STTONGSUNTIANSUNPARA sttongsuntmp;
    ucMPNo= ucDAp;
    if(ucMPNo >= CON_MAX_MPNUM)
        ucRet =RET_ERROR;
    else
    {
        for(i=0;i<24;i++)
        {
            ucRet = Check_BCD_Data(pucBuffer[i]);
            if(ucRet == RET_ERROR)
                break;
        }
        if(ucRet == RET_SUCCESS)
        {
            sttongsuntmp.stCuFeLostParaA.R = (unsigned short)BCD_TO_HEX_2(pucBuffer+m);
            m += 2;
            sttongsuntmp.stCuFeLostParaA.X = (unsigned short)BCD_TO_HEX_2(pucBuffer+m);
            m += 2;
            sttongsuntmp.stCuFeLostParaA.G = (unsigned short)BCD_TO_HEX_2(pucBuffer+m);
            m += 2;
            sttongsuntmp.stCuFeLostParaA.B = (unsigned short)BCD_TO_HEX_2(pucBuffer+m);
            m += 2;
            sttongsuntmp.stCuFeLostParaB.R = (unsigned short)BCD_TO_HEX_2(pucBuffer+m);
            m += 2;
            sttongsuntmp.stCuFeLostParaB.X = (unsigned short)BCD_TO_HEX_2(pucBuffer+m);
            m += 2;
            sttongsuntmp.stCuFeLostParaB.G = (unsigned short)BCD_TO_HEX_2(pucBuffer+m);
            m += 2;
            sttongsuntmp.stCuFeLostParaB.B = (unsigned short)BCD_TO_HEX_2(pucBuffer+m);
            m += 2;
            sttongsuntmp.stCuFeLostParaC.R = (unsigned short)BCD_TO_HEX_2(pucBuffer+m);
            m += 2;
            sttongsuntmp.stCuFeLostParaC.X = (unsigned short)BCD_TO_HEX_2(pucBuffer+m);
            m += 2;
            sttongsuntmp.stCuFeLostParaC.G = (unsigned short)BCD_TO_HEX_2(pucBuffer+m);
            m += 2;
            sttongsuntmp.stCuFeLostParaC.B = (unsigned short)BCD_TO_HEX_2(pucBuffer+m);
            m += 2;
        }
    }

    if(ucRet==RET_SUCCESS)
        RunPara_SetMPTongsun(ucMPNo,&sttongsuntmp);
    return ucRet;
}

//F28���������ֶ���ֵ����
unsigned char Main_Recv_Set_F28_MPPowRatePara(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS;
    unsigned short  m=0;
    unsigned char   ucMPNo=0;
    STPOWRATEPARA stPowRatePara;
    ucMPNo= ucDAp;
    if(ucMPNo >= CON_MAX_MPNUM)
        ucRet =RET_ERROR;
    else
    {
        ucRet=Check_Data_DT05(pucBuffer+m);
        if(ucRet==RET_SUCCESS)
            ucRet=Check_Data_DT05(pucBuffer+m);
        if(ucRet==RET_SUCCESS)
        {
            m=0;
            RunPara_InitMPPowRatePara(&stPowRatePara);
            pucBuffer[m+1]&=0x7F;
            stPowRatePara.usPowRateLimit1=(unsigned short)BCD_TO_HEX_2(pucBuffer+m);
            m+=2;
            pucBuffer[m+1]&=0x7F;
            stPowRatePara.usPowRateLimit2=(unsigned short)BCD_TO_HEX_2(pucBuffer+m);
            if(stPowRatePara.usPowRateLimit1>stPowRatePara.usPowRateLimit2)
                ucRet=RET_ERROR;
        }
    }
    if(ucRet==RET_SUCCESS)
        RunPara_SetMPPowRatePara(ucMPNo,&stPowRatePara);
    return ucRet;
}


////F29���ն˵��ص��ܱ���ʾ��
//unsigned char Main_Recv_Set_F29_Ter_Meter_addr(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char   ucRet=RET_SUCCESS;
////  unsigned short  m=0;
//    unsigned char ucMPNo = 0,ucMeterNo = 0,ucMPType = CON_MPTYPE_NULL;
//    STMETERPARA stMeterPara;
//
//    ucMPNo = ucDAp;
//    ucMPType = RunPara_GetMPType(ucMPNo);
//    if(ucMPType == CON_MPTYPE_METER)
//    {
//        ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
//        stMeterPara = RunPara_GetMeterPara(ucMeterNo);
//        memcpy(stMeterPara.ucMeterJuNum,pucBuffer,12);
//        RunPara_SetMeterPara(ucMeterNo,&stMeterPara);
//    }
//    return ucRet;
//}
unsigned char Main_Recv_Set_F30_CopyFlag(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet = RET_SUCCESS;
    unsigned char ucCopyFlag;

    ucCopyFlag = pucBuffer[0];

    if(ucCopyFlag == 0 || ucCopyFlag == 1)
    {
        if(ucCopyFlag ==0)
            ucCopyFlag = CON_STATUS_YES;
        else
            ucCopyFlag = CON_STATUS_NO;
        RunPara_SetCopyFlag(ucCopyFlag);
    }else
        ucRet = RET_ERROR;
    return ucRet;
}
unsigned char Main_Recv_Set_F34_PortProperty(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS,i=0;
    unsigned short m=0;
    unsigned char  ucPort;
    unsigned char  ucPortNum_tmp,ucDataTmp;//azh
    STPORTPROP stPortProp;
    STPORTLIST stPortPropList = RunPara_GetPortPropList();

    ucPortNum_tmp = pucBuffer[m++];
    if(ucPortNum_tmp>CON_COMMPORT_NUM)
        ucPortNum_tmp = CON_COMMPORT_NUM;//���֧��8���˿�
    for(i=0;i<ucPortNum_tmp;i++)
    {
        ucPort = pucBuffer[m];
        if(ucPort ==0 || ucPort > CON_COMMPORT_NUM)
        {
            ucRet = RET_ERROR;
            break;
        }
        m += 6;
    }
    if(ucRet == RET_SUCCESS)
    {
        m =1;
        for(i=0;i<ucPortNum_tmp;i++)
        {
            ucPort = pucBuffer[m++];
            ucPort -= 1;
            ucDataTmp = pucBuffer[m++];
            Main_GetPortProp(&stPortProp, ucDataTmp);
            memcpy(&(stPortProp.ulBaudRate),pucBuffer+m,4);
            m += 4;
            stPortPropList.stPortProp[ucPort] = stPortProp;
        }
        RunPara_SetPortPropList(&stPortPropList);
    }
    return ucRet;
}
//unsigned char Main_Recv_Set_F35_EmphaFlag(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet = RET_SUCCESS,i=0,j=0;
//    unsigned char ucMeterNum,ucMeterNo=0,ucMeterTable[CON_METER_NUM];
//    unsigned short m = 0;
//    STMETERPARA stMeterPara;
//
//    ucMeterNum = pucBuffer[m++];
//    if(ucMeterNum > CON_METER_NUM)
//        ucMeterNum = CON_METER_NUM;
//
//    for(i=0;i<ucMeterNum;i++)
//    {
//        ucMeterNo = pucBuffer[m];
//        if(ucMeterNo <= CON_METER_NUM && pucBuffer[m+1] == 0)
//            ucMeterTable[i] = ucMeterNo;
//        else
//        {
//            ucRet = RET_ERROR;
//            break;
//        }
//        m += 2;
//    }
//    if(ucRet == RET_SUCCESS)
//    {
//        for(i=0;i<CON_METER_NUM;i++)
//        {
//            stMeterPara = RunPara_GetMeterPara(i);
//            for(j=0;j<ucMeterNum;j++)
//            {
//                if((i+1) == ucMeterTable[j])
//                {
//                    stMeterPara.ucEmpFlag = CON_STATUS_YES;
//                    break;
//                }
//            }
//            if(j >= ucMeterNum)
//                stMeterPara.ucEmpFlag = CON_STATUS_NO;
//            RunPara_SetMeterPara(i, &stMeterPara);
//        }
//    }
//    return ucRet;
//}
//F33�ն����в���
unsigned char Main_Recv_Set_F33_SumGFreezePara(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS;
  unsigned char   i=0,n=0,j=0;

  unsigned short crc16,m=0;
  unsigned char  ucPort,tmp[2],sp;
  unsigned long  ulVal=0;
  STFREEZEDAY    stFreezeDay;
  unsigned char  ucPortNum_tmp,ucZoneNum;//azh
  PSTRMRUNPARA    pstRMRunPara;
  PSTCOMPORTPARA pstComPortPara;
  STDATETIME stDateTime = GetCurrentTime();
  STTERNORMALPARA stTerNormalPara=RunPara_GetTerNormalPara();

  pstRMRunPara=RunPara_GetRMRunPara();
  ucPortNum_tmp = pucBuffer[m++];
  //���ݼ��
  if(ucPortNum_tmp>CON_PORT_NUM) ucPortNum_tmp = CON_PORT_NUM;//���֧��8���˿�
  for(i=0; i<ucPortNum_tmp; i++)
  {
      if( (pucBuffer[m]==0) || (pucBuffer[m]>CON_PORT_NUM) )
          return RET_ERROR;
      m += 7;
      if( Check_BCD_Data_Buffer(&pucBuffer[m],2)==RET_ERROR ) return RET_ERROR;
      m += 3;
      if( Check_BCD_Data_Buffer(&pucBuffer[m],3)==RET_ERROR ) return RET_ERROR;
      m += 3;
      ucZoneNum = pucBuffer[m++];
      if(ucZoneNum >24)
          return RET_ERROR;
      if( Check_BCD_Data_Buffer(&pucBuffer[m],(unsigned char)(ucZoneNum*4))==RET_ERROR )
          return RET_ERROR;
      m += (4*ucZoneNum);
  }
  //��ֵ
  m = 1;
  for(i=0; i<ucPortNum_tmp; i++)
  {
	  sp = m;
      ucPort = pucBuffer[m++];
      pstComPortPara = &pstRMRunPara->stComPortPara[ucPort-1];
      pstComPortPara->usRMRunCtrlWord = pucBuffer[m] + pucBuffer[m+1]*0x100;
      m += 2;
      pstComPortPara->ulRMDate = pucBuffer[m] + (pucBuffer[m+1]<<8) + (pucBuffer[m+2]<<16) + (pucBuffer[m+3]<<24);
      m += 4;
      pstComPortPara->ucRMTime[1] = (unsigned char)BCD_TO_HEX_1(pucBuffer+m);
      pstComPortPara->ucRMTime[0] = (unsigned char)BCD_TO_HEX_1(pucBuffer+m+1);
      m += 2;
      pstComPortPara->ucRMInval = pucBuffer[m];
      m += 1;
      pstComPortPara->ucRMBcastTInval[2] = (unsigned char)BCD_TO_HEX_1(pucBuffer+m);
      pstComPortPara->ucRMBcastTInval[1] = (unsigned char)BCD_TO_HEX_1(pucBuffer+m+1);
      pstComPortPara->ucRMBcastTInval[0] = (unsigned char)BCD_TO_HEX_1(pucBuffer+m+2);
      m += 3;
      ucZoneNum = pucBuffer[m];
      pstComPortPara->ucRMWorkZoneNum = ucZoneNum;
      m += 1;
      if(ucZoneNum!=0)
      {
          for(j=0; j<ucZoneNum; j++)
          {
              pstComPortPara->stRMWorkZone[j].usRMTStart[1] = (unsigned char)BCD_TO_HEX_1(pucBuffer+m);
              pstComPortPara->stRMWorkZone[j].usRMTStart[0] = (unsigned char)BCD_TO_HEX_1(pucBuffer+m+1);
              pstComPortPara->stRMWorkZone[j].usRMTEnd[1] = (unsigned char)BCD_TO_HEX_1(pucBuffer+m+2);
              pstComPortPara->stRMWorkZone[j].usRMTEnd[0] = (unsigned char)BCD_TO_HEX_1(pucBuffer+m+3);
              m += 4;
          }
      }
      SetBitFlag(&pstRMRunPara->ucComPortEBit, (unsigned short)(ucPort-1));
	  pucBuffer[sp] = 0xaa;
	  crc16 = calc_crc16(pucBuffer + sp, ucZoneNum * 4 + 14, 0xffff);      //����ͨ�Ų������ݵ�CRC
	  tmp[0] = pucBuffer[m];												//���������ֽ�����CRC���Ȱ������ֽڴ�����
	  tmp[1] = pucBuffer[m + 1];
	  pucBuffer[m] = crc16 >> 8;
	  pucBuffer[m + 1] = crc16;
		file_write(F33_SUMGFREEZEPARA_ADDR + (ucPort - 1) * F33_SUMGFREEZEPARA_LEN, pucBuffer + sp, m + 2 - sp);    //����Ч�Ĳ���д��EEPROM��
	  pucBuffer[sp] = ucPort;												//��ԭ���޸ĵ�ֵ
	  pucBuffer[m] = tmp[0];												//��ԭ���޸ĵ�ֵ
	  pucBuffer[m + 1] = tmp[1];	  										//��ԭ���޸ĵ�ֵ
  }
//azh �������������޸�
//  stTerNormalPara.ucCopyInter = pstComPortPara->ucRMInval ;
//  if(stTerNormalPara.ucCopyInter>=15)
//      stTerNormalPara.ucCopyInter=15;
//  else if(stTerNormalPara.ucCopyInter<=2)     //�˴�����С�ڣ���С�ڣ���Ӱ����ͣ�ߡ����ߵı���
//      stTerNormalPara.ucCopyInter=2;
//  RunPara_SetTerNormalPara(&stTerNormalPara);
  //cfh 10-04-27 ���ӳ����յ����ù���
  RunPara_InitMPFreezeDay(&stFreezeDay);
  m=4;//ָ�򳭱���
//    if((pucBuffer[m+3] & 0x80)==0x80)
//        return RET_ERROR;
  if(Check_BCD_Data(pucBuffer[m+4])==RET_ERROR || BCD_TO_HEX_1(&pucBuffer[m+4])>59)
      return RET_ERROR;
  if(Check_BCD_Data(pucBuffer[m+5])==RET_ERROR || BCD_TO_HEX_1(&pucBuffer[m+5])>23)
      return RET_ERROR;

  ulVal = pucBuffer[m+0] + pucBuffer[m+1]*0x100 + pucBuffer[m+2]*0x10000 + pucBuffer[m+3]*0x1000000;
  for(i=0;i<CON_MAX_FREEZEDAYNUM;i++)
  {
      if((ulVal&0x01)==0x01)
      {
          stFreezeDay.stFreezeDay[n].ucYear   =0x10;
          stFreezeDay.stFreezeDay[n].ucMonth  =0x01;
          stFreezeDay.stFreezeDay[n].ucDay    =i+1;
          stFreezeDay.stFreezeDay[n].ucHour   =(unsigned char)BCD_TO_HEX_1(pucBuffer+m+5);
          stFreezeDay.stFreezeDay[n].ucMinute =(unsigned char)BCD_TO_HEX_1(pucBuffer+m+4);
          stFreezeDay.stFreezeDay[n].ucSecond =0;
          n++;
      }
      ulVal=ulVal>>1;
  }
  for(i=0;i<CON_MAX_MPNUM;i++)
      RunPara_SetMPFreezeDay((unsigned char)i,&stFreezeDay);
    return ucRet;
}
//unsigned char Main_Recv_Set_F37_LinkPara(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet = RET_SUCCESS,i=0,ucDataTmp= 0;
//    unsigned short m =0;
//    STLINKPARA  stLinkParaTmp;
////  unsigned long ulBaudRate;
//
//    stLinkParaTmp.ucPortNo = pucBuffer[m++];
//    if(stLinkParaTmp.ucPortNo == 0)
//        stLinkParaTmp.ucSlaveFlag = CON_LINK_INVALID;
//    ucDataTmp = pucBuffer[m++];
//    if((ucDataTmp & 0x03)>3)
//        ucRet = RET_ERROR;
//    if(pucBuffer[5] > 60)
//        ucRet = RET_ERROR;
//    ucDataTmp = pucBuffer[6];
//    if(((ucDataTmp &0x80)== 0 && (ucDataTmp &0x0F)>3)||
//        ((ucDataTmp &0x80)==1 && (ucDataTmp & 0x0F)!=1))
//        ucRet = RET_ERROR;
//    for(i=0;i<(ucDataTmp&0x0F);i++)
//    {
//        m =7;
//        if(Check_BCD_Data(pucBuffer[m]) != RET_SUCCESS ||Check_BCD_Data(pucBuffer[m+1])!= RET_SUCCESS)
//        {
//            ucRet = RET_ERROR;
//            break;
//        }
//        m += 4;
//    }
//    if(ucRet == RET_SUCCESS)
//    {
//        m = 0;
//        stLinkParaTmp.ucPortNo = pucBuffer[m++];
//        ucDataTmp = pucBuffer[m++];
//        Main_GetPortProp(&(stLinkParaTmp.stLinkPortPara), ucDataTmp);
//        stLinkParaTmp.ucRecvOverTime = pucBuffer[m++];
//        stLinkParaTmp.ucByteOverTime = pucBuffer[m++];
//        stLinkParaTmp.ucResendTime = pucBuffer[m++];
//        stLinkParaTmp.ucInterTime = pucBuffer[m++];
//        if(stLinkParaTmp.ucPortNo != 0)
//            stLinkParaTmp.ucSlaveFlag = pucBuffer[m]>>7;
//        stLinkParaTmp.ucSlaveNum = pucBuffer[m]&0x0F;
//        m ++;
//        for(i=0;i<stLinkParaTmp.ucSlaveNum;i++)
//        {
//            stLinkParaTmp.stSlaveAddr[i].stTerRegionAddr.ucRegionAddr[1] = pucBuffer[m++];
//            stLinkParaTmp.stSlaveAddr[i].stTerRegionAddr.ucRegionAddr[0] = pucBuffer[m++];
//            stLinkParaTmp.stSlaveAddr[i].stTerAddr.ucDevAddr[1] = pucBuffer[m++];
//            stLinkParaTmp.stSlaveAddr[i].stTerAddr.ucDevAddr[0] = pucBuffer[m++];
//        }
//        RunPara_SetLinkPara(&stLinkParaTmp);
//    }
//    return ucRet;
//}
unsigned char Main_Recv_Set_F38_Class(unsigned char *pucBuffer,unsigned char ucDAp,unsigned char ucJobNo)
{
    unsigned char ucRet = RET_SUCCESS;
    unsigned short m = 0;
    STUPUSR stUpUsr;
    unsigned char i,ucNum,ucNum1,ucUpUsr,ucLitUsr;

    ucUpUsr = pucBuffer[m++];       //�����
    if(ucUpUsr >= CON_UPUSR_NUM)
        ucRet = RET_ERROR;
    ucNum = pucBuffer[m++];     //����
    if(ucNum > CON_LITUSR_NUM)
        ucRet = RET_ERROR;
    for(i=0;i<ucNum;i++)
    {
        ucLitUsr = pucBuffer[m++];      //С���
        if(ucLitUsr >= CON_LITUSR_NUM)
            ucRet = RET_ERROR;
        ucNum1= pucBuffer[m++];     //��Ϣ������
        if(ucNum1 > 31 )
            ucRet = RET_ERROR;
        m += ucNum1;
    }
    if(ucRet == RET_SUCCESS)
    {
        m = 0;
        memset(&stUpUsr,0,sizeof(STUPUSR));
        ucUpUsr = pucBuffer[m++];
        ucNum = pucBuffer[m++];
        for(i=0;i<ucNum;i++)
        {
            ucLitUsr = pucBuffer[m++];
            ucNum1 = pucBuffer[m++];
            stUpUsr.stLitType[ucLitUsr].ucFnGroupNum = ucNum1;
            memcpy(stUpUsr.stLitType[ucLitUsr].ucBitFlag,pucBuffer+m,ucNum1);
            m += ucNum1;
        }
        ucRet =  RunPara_SetUpUsrJob(ucJobNo, ucUpUsr, &stUpUsr);
    }
    return ucRet;
}
////F41��ʱ�ι��ض�ֵ
//unsigned char Main_Recv_Set_F41_TimePerSetting(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned char m=0,ucTmp,i,j,ucPos;
//  unsigned char ucTimePerFlag,ucProjectFlag;
//  unsigned char ucSumGroupNo=0;
//  unsigned short usDT2=0;
//  float fVal;
//  unsigned char ucNum;
//  STPOWERCTRLTIME stPowerCtrlTime;
//  STPOWERCTRLPROJ stPowerCtrlProj;
//  unsigned char mBak;
//  //�����Ч��
//  ucSumGroupNo=ucDAp;
//  if(ucSumGroupNo>=CON_SUMGROUP_NUM)
//      ucRet=RET_ERROR;
//
//  ucProjectFlag=pucBuffer[0];
//  if(ucProjectFlag&0x07==0)
//      ucRet=RET_ERROR;
//  if(ucRet==RET_SUCCESS)
//  {
//      m=1;
//      if((ucProjectFlag&0x01)==0x01)
//      {
//          stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER1,ucSumGroupNo);
//          ucTimePerFlag=pucBuffer[m++];
//          ucNum=0;
//          for(i=0;i<8;i++)
//          {
//              if((ucTimePerFlag&(1<<i))!=0)
//                  ucNum++;
//          }
//          if(ucRet==RET_SUCCESS)
//          {
//              for(i=0;i<ucNum;i++)
//              {
//                  ucRet=Check_Data_DT02(pucBuffer+m);     m+=2;
//                  if(ucRet==RET_ERROR)
//                      break;
//              }
//          }
//      }
//      if((ucProjectFlag&0x02)==0x02 && ucRet==RET_SUCCESS)
//      {
//          stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER2,ucSumGroupNo);
//          ucTimePerFlag=pucBuffer[m++];
//          ucNum=0;
//          for(i=0;i<8;i++)
//          {
//              if((ucTimePerFlag&(1<<i))!=0)
//                  ucNum++;
//          }
//          if(ucRet==RET_SUCCESS)
//          {
//              for(i=0;i<ucNum;i++)
//              {
//                  ucRet=Check_Data_DT02(pucBuffer+m);     m+=2;
//                  if(ucRet==RET_ERROR)
//                      break;
//              }
//          }
//      }
//      if((ucProjectFlag&0x04)==0x04 && ucRet==RET_SUCCESS)
//      {
//          stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER3,ucSumGroupNo);
//          ucTimePerFlag=pucBuffer[m++];
//          ucNum=0;
//          for(i=0;i<8;i++)
//          {
//              if((ucTimePerFlag&(1<<i))!=0)
//                  ucNum++;
//          }
//          if(ucRet==RET_SUCCESS)
//          {
//              for(i=0;i<ucNum;i++)
//              {
//                  ucRet=Check_Data_DT02(pucBuffer+m);     m+=2;
//                  if(ucRet==RET_ERROR)
//                      break;
//              }
//          }
//      }
//  }
//  if(ucRet==RET_SUCCESS)
//  {
//      m=1;
//      if((ucProjectFlag&0x01)==0x01)
//      {
//          Main_Recv_Set_TimePerSetting(pucBuffer,&m,ucSumGroupNo,CONSWITCHCURVE_TIMEPER1);
///*            stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_TIMEPER1,ucSumGroupNo);
//          stPowerCtrlProj.ucCycleType=CON_CYCLETYPE_DAY;
//          stPowerCtrlProj.ucAvailbleFlag[0]=CON_AVALIBLE_FLAG;
//          stPowerCtrlProj.usCycleNum=0xFFFF;
//          stPowerCtrlProj.stStartTime=GetCurrentTime();
//          stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER1,ucSumGroupNo);
//          ucTimePerFlag=pucBuffer[m++];
//          mBak=m;
//          ucPos=0;
//          for(i=0;i<8;i++)
//          {
//              for(j=ucPos;j<CONSWITCHPERTIMENUM;j++)
//              {
//                  if(stPowerCtrlTime.stPowerCtrlPer[j].ucCtrlFlag==CON_AVALIBLE_FLAG)
//                  {
//                      ucTmp=(1<<i);
//                      if((ucTimePerFlag&ucTmp)==ucTmp)
//                      {
//                          usDT2=pucBuffer[m]+0x100*pucBuffer[m+1];    m+=2;
//                          fVal=DT2_To_Float(usDT2);
//                          if(fVal<0)
//                              fVal=0-fVal;
//                          stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.dFixVal=fVal;
//                          stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.usDT2=usDT2;
//                      }
//                      else
//                      {
//                          stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.dFixVal=CON_POWER_BIGVAL;
//                          stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.usDT2=0xEEEE;
//                      }
//                      ucPos=j+1;
//                      break;
//                  }
//              }
//          }
//          for(i=0;i<8;i++)
//          {
//              ucTmp=(1<<i);
//              if((ucTimePerFlag&ucTmp)==ucTmp)
//              {
//                  mBak+=2;
//              }
//          }
//          m=mBak;
//          if(ucTimePerFlag==0)                    //ʱ����Ϊ0��ɾ������
//          {
//              stPowerCtrlProj.ucProjAvailbleFlag=CON_INVALIBLE_FLAG;
//          }
//          RunPara_SetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER1,&stPowerCtrlTime,ucSumGroupNo);
//          RunPara_SetPowCtrlProj(CONSWITCHCURVE_TIMEPER1,&stPowerCtrlProj,ucSumGroupNo);*/
//
//      }
//      else        //ɽ�������ʱ�β�����ɾ��
//      {
//          stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_TIMEPER1,ucSumGroupNo);
//          stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER1,ucSumGroupNo);
//          stPowerCtrlProj.ucProjAvailbleFlag=CON_INVALIBLE_FLAG;
//          for(j=0;j<CONSWITCHPERTIMENUM;j++)
//          {
//              if(stPowerCtrlTime.stPowerCtrlPer[j].ucCtrlFlag==CON_AVALIBLE_FLAG)
//              {
//                  stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.dFixVal=CON_POWER_BIGVAL;
//                  stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.usDT2=0xEEEE;
//              }
//          }
//
//          RunPara_SetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER1,&stPowerCtrlTime,ucSumGroupNo);
//          RunPara_SetPowCtrlProj(CONSWITCHCURVE_TIMEPER1,&stPowerCtrlProj,ucSumGroupNo);
//      }
//
//      if((ucProjectFlag&0x02)==0x02 && ucRet==RET_SUCCESS)
//      {
//          Main_Recv_Set_TimePerSetting(pucBuffer,&m,ucSumGroupNo,CONSWITCHCURVE_TIMEPER2);
///*            stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_TIMEPER2,ucSumGroupNo);
//          stPowerCtrlProj.ucCycleType=CON_CYCLETYPE_DAY;
//          stPowerCtrlProj.ucAvailbleFlag[0]=CON_AVALIBLE_FLAG;
//          stPowerCtrlProj.usCycleNum=0xFFFF;
//          stPowerCtrlProj.stStartTime=GetCurrentTime();
//          stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER2,ucSumGroupNo);
//          ucTimePerFlag=pucBuffer[m++];
//          mBak=m;
//          ucPos=0;
//          for(i=0;i<8;i++)
//          {
//              for(j=ucPos;j<CONSWITCHPERTIMENUM;j++)
//              {
//                  if(stPowerCtrlTime.stPowerCtrlPer[j].ucCtrlFlag==CON_AVALIBLE_FLAG)
//                  {
//                      ucTmp=(1<<i);
//                      if((ucTimePerFlag&ucTmp)==ucTmp)
//                      {
//                          usDT2=pucBuffer[m]+0x100*pucBuffer[m+1];    m+=2;
//                          fVal=DT2_To_Float(usDT2);
//                          if(fVal<0)
//                              fVal=0-fVal;
//                          stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.dFixVal=fVal;
//                          stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.usDT2=usDT2;
//                      }
//                      else
//                      {
//                          stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.dFixVal=CON_POWER_BIGVAL;
//                          stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.usDT2=0xEEEE;
//                      }
//                      ucPos=j+1;
//                      break;
//                  }
//              }
//          }
//          for(i=0;i<8;i++)
//          {
//              ucTmp=(1<<i);
//              if((ucTimePerFlag&ucTmp)==ucTmp)
//              {
//                  mBak+=2;
//              }
//          }
//          m=mBak;
//          if(ucTimePerFlag==0)                    //ʱ����Ϊ0��ɾ������
//          {
//              stPowerCtrlProj.ucProjAvailbleFlag=CON_INVALIBLE_FLAG;
//          }
//          RunPara_SetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER2,&stPowerCtrlTime,ucSumGroupNo);
//          RunPara_SetPowCtrlProj(CONSWITCHCURVE_TIMEPER2,&stPowerCtrlProj,ucSumGroupNo);*/
//      }
//      else//ɽ�������ʱ�β�����ɾ��
//      {
//          stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_TIMEPER2,ucSumGroupNo);
//          stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER2,ucSumGroupNo);
//          stPowerCtrlProj.ucProjAvailbleFlag=CON_INVALIBLE_FLAG;
//          for(j=0;j<CONSWITCHPERTIMENUM;j++)
//          {
//              if(stPowerCtrlTime.stPowerCtrlPer[j].ucCtrlFlag==CON_AVALIBLE_FLAG)
//              {
//                  stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.dFixVal=CON_POWER_BIGVAL;
//                  stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.usDT2=0xEEEE;
//              }
//          }
//          RunPara_SetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER2,&stPowerCtrlTime,ucSumGroupNo);
//          RunPara_SetPowCtrlProj(CONSWITCHCURVE_TIMEPER2,&stPowerCtrlProj,ucSumGroupNo);
//      }
//
//      if((ucProjectFlag&0x04)==0x04 && ucRet==RET_SUCCESS)
//      {
//          Main_Recv_Set_TimePerSetting(pucBuffer,&m,ucSumGroupNo,CONSWITCHCURVE_TIMEPER3);
///*            stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_TIMEPER3,ucSumGroupNo);
//          stPowerCtrlProj.ucCycleType=CON_CYCLETYPE_DAY;
//          stPowerCtrlProj.ucAvailbleFlag[0]=CON_AVALIBLE_FLAG;
//          stPowerCtrlProj.usCycleNum=0xFFFF;
//          stPowerCtrlProj.stStartTime=GetCurrentTime();
//          stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER3,ucSumGroupNo);
//          ucTimePerFlag=pucBuffer[m++];
//          mBak=m;
//          ucPos=0;
//          for(i=0;i<8;i++)
//          {
//              for(j=ucPos;j<CONSWITCHPERTIMENUM;j++)
//              {
//                  if(stPowerCtrlTime.stPowerCtrlPer[j].ucCtrlFlag==CON_AVALIBLE_FLAG)
//                  {
//                      ucTmp=(1<<i);
//                      if((ucTimePerFlag&ucTmp)==ucTmp)
//                      {
//                          usDT2=pucBuffer[m]+0x100*pucBuffer[m+1];    m+=2;
//                          fVal=DT2_To_Float(usDT2);
//                          if(fVal<0)
//                              fVal=0-fVal;
//                          stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.dFixVal=fVal;
//                          stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.usDT2=usDT2;
//                      }
//                      else
//                      {
//                          stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.dFixVal=CON_POWER_BIGVAL;
//                          stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.usDT2=0xEEEE;
//                      }
//                      ucPos=j+1;
//                      break;
//                  }
//              }
//          }
//          for(i=0;i<8;i++)
//          {
//              ucTmp=(1<<i);
//              if((ucTimePerFlag&ucTmp)==ucTmp)
//              {
//                  mBak+=2;
//              }
//          }
//          m=mBak;
//          if(ucTimePerFlag==0)                    //ʱ����Ϊ0��ɾ������
//          {
//              stPowerCtrlProj.ucProjAvailbleFlag=CON_INVALIBLE_FLAG;
//          }
//          RunPara_SetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER3,&stPowerCtrlTime,ucSumGroupNo);
//          RunPara_SetPowCtrlProj(CONSWITCHCURVE_TIMEPER3,&stPowerCtrlProj,ucSumGroupNo);*/
//      }
//      else                                        //ɽ�������ʱ�β�����ɾ��
//      {
//          stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_TIMEPER3,ucSumGroupNo);
//          stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER3,ucSumGroupNo);
//          stPowerCtrlProj.ucProjAvailbleFlag=CON_INVALIBLE_FLAG;
//          for(j=0;j<CONSWITCHPERTIMENUM;j++)
//          {
//              if(stPowerCtrlTime.stPowerCtrlPer[j].ucCtrlFlag==CON_AVALIBLE_FLAG)
//              {
//                  stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.dFixVal=CON_POWER_BIGVAL;
//                  stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.usDT2=0xEEEE;
//              }
//          }
//
//          RunPara_SetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER3,&stPowerCtrlTime,ucSumGroupNo);
//          RunPara_SetPowCtrlProj(CONSWITCHCURVE_TIMEPER3,&stPowerCtrlProj,ucSumGroupNo);
//      }
//  }
//    return ucRet;
//}
//unsigned char Main_Recv_Set_TimePerSetting(unsigned char *pucBuffer,unsigned char *ucPos,unsigned char ucSumGroupNo,unsigned char ucCurveNo)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned char m=0,ucTmp,i,j;
//  unsigned char ucTimePerFlag,ucProjectFlag;
//  unsigned short usDT2;
//  float fVal;
//  unsigned char ucNum,ucPosTmp;
//  STPOWERCTRLTIME stPowerCtrlTime;
//  STPOWERCTRLPROJ stPowerCtrlProj;
//  unsigned char mBak;
//
//  m  = *ucPos;
//  stPowerCtrlProj=RunPara_GetPowCtrlProj(ucCurveNo,ucSumGroupNo);
//  stPowerCtrlProj.ucCycleType=CON_CYCLETYPE_DAY;
//  stPowerCtrlProj.ucAvailbleFlag[0]=CON_AVALIBLE_FLAG;
//  stPowerCtrlProj.usCycleNum=0xFFFF;
//  stPowerCtrlProj.stStartTime=GetCurrentTime();
//  stPowerCtrlTime=RunPara_GetPowCtrlTimePer(ucCurveNo,ucSumGroupNo);
//  ucTimePerFlag=pucBuffer[m++];
//  mBak=m;
//  ucPosTmp=0;
//  for(i=0;i<8;i++)
//  {
//      for(j=ucPosTmp;j<CONSWITCHPERTIMENUM;j++)
//      {
//          if(stPowerCtrlTime.stPowerCtrlPer[j].ucCtrlFlag==CON_AVALIBLE_FLAG)
//          {
//              ucTmp=(1<<i);
//              if((ucTimePerFlag&ucTmp)==ucTmp)
//              {
//                  usDT2=pucBuffer[m]+0x100*pucBuffer[m+1];    m+=2;
//                  fVal=DT2_To_Float(usDT2);
//                  if(fVal<0)
//                      fVal=0-fVal;
//                  stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.dFixVal=fVal;
//                  stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.usDT2=usDT2;
//              }
//              else
//              {
//                  stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.dFixVal=CON_POWER_BIGVAL;
//                  stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.usDT2=0xEEEE;
//              }
//              ucPosTmp=j+1;
//              break;
//          }
//      }
//  }
//  for(i=0;i<8;i++)
//  {
//      ucTmp=(1<<i);
//      if((ucTimePerFlag&ucTmp)==ucTmp)
//      {
//          mBak+=2;
//      }
//  }
//  m=mBak;
//  if(ucTimePerFlag==0)                    //ʱ����Ϊ0��ɾ������
//  {
//      stPowerCtrlProj.ucProjAvailbleFlag=CON_INVALIBLE_FLAG;
//  }
//  RunPara_SetPowCtrlTimePer(ucCurveNo,&stPowerCtrlTime,ucSumGroupNo);
//  RunPara_SetPowCtrlProj(ucCurveNo,&stPowerCtrlProj,ucSumGroupNo);
//
//  *ucPos = m;
//    return ucRet;
//}
////F42�����ݹ��ز���
//unsigned char Main_Recv_Set_F42_RestCtrl(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned char m=0,i,ucTmp;
//  unsigned char ucSumGroupNo=0;
//  unsigned short usDT2=0;
//  float fVal;
//  STPOWERCTRLTIME stPowerCtrlTime;
//  STDATETIME stCurrentTime=GetCurrentTime();
//  STPOWERCTRLPROJ stPowerCtrlProj;
//
//  ucSumGroupNo=ucDAp;
//  if(ucSumGroupNo>=CON_SUMGROUP_NUM)
//      ucRet=RET_ERROR;
//  if(Check_Data_DT02(pucBuffer)==RET_ERROR)               //�������
//      ucRet=RET_ERROR;
//  if(Check_BCD_Data(pucBuffer[2])==RET_ERROR || Check_BCD_Data(pucBuffer[3])==RET_ERROR)
//      ucRet=RET_ERROR;
//  if(pucBuffer[4]==0 || pucBuffer[4]>48)
//      ucRet=RET_ERROR;
//  if((pucBuffer[5]&0x01)==0x01 || (pucBuffer[5]&0xFE)==0x00)
//      ucRet=RET_ERROR;
//  if(ucRet==RET_SUCCESS)
//  {
//      stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_RESTCTRL,ucSumGroupNo);
//      usDT2=pucBuffer[m]+0x100*pucBuffer[m+1];    m+=2;
//      fVal=DT2_To_Float(usDT2);
//      if(fVal<0)
//          fVal=0-fVal;
//      stPowerCtrlTime.stPowerCtrlPer[0].ucAvailbleFlag=CON_AVALIBLE_FLAG;
//      stPowerCtrlTime.stPowerCtrlPer[0].ucCtrlFlag=CON_AVALIBLE_FLAG;
//      stPowerCtrlTime.stPowerCtrlPer[0].stLoadFixVal.dFixVal=fVal;
//      stPowerCtrlTime.stPowerCtrlPer[0].stLoadFixVal.usDT2=usDT2;
//      stPowerCtrlTime.stPowerCtrlPer[0].ucBeginMinute=(unsigned char)BCD_TO_HEX_1(pucBuffer+2);
//      stPowerCtrlTime.stPowerCtrlPer[0].ucBeginHour=(unsigned char)BCD_TO_HEX_1(pucBuffer+3);
//      stCurrentTime.ucMinute=stPowerCtrlTime.stPowerCtrlPer[0].ucBeginMinute;
//      stCurrentTime.ucHour=stPowerCtrlTime.stPowerCtrlPer[0].ucBeginHour;
//      AddTime(&stCurrentTime,(30*pucBuffer[4]),CON_TIMEUNIT_MIN);
//      stPowerCtrlTime.stPowerCtrlPer[1].ucAvailbleFlag=CON_AVALIBLE_FLAG;
//      stPowerCtrlTime.stPowerCtrlPer[1].ucCtrlFlag=CON_INVALIBLE_FLAG;
//      stPowerCtrlTime.stPowerCtrlPer[1].ucBeginMinute=stCurrentTime.ucMinute;
//      stPowerCtrlTime.stPowerCtrlPer[1].ucBeginHour=stCurrentTime.ucHour;
//      for(i=2;i<CONSWITCHPERTIMENUM;i++)
//      {
//          stPowerCtrlTime.stPowerCtrlPer[i].ucAvailbleFlag=CON_INVALIBLE_FLAG;
//          stPowerCtrlTime.stPowerCtrlPer[i].ucCtrlFlag=CON_INVALIBLE_FLAG;
//      }
//      RunPara_SetPowCtrlTimePer(CONSWITCHCURVE_RESTCTRL,&stPowerCtrlTime,ucSumGroupNo);
//      stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_RESTCTRL,ucSumGroupNo);
//      stPowerCtrlProj.ucCycleType=CON_CYCLETYPE_WEEK;
//      stPowerCtrlProj.usCycleNum=0xFFFF;
////        stPowerCtrlProj.stStartTime=GetCurrentTime();           //Ϊ���̨�壬��ʼʱ����ǰ��03��1��1��
//      stPowerCtrlProj.stStartTime.ucYear=3;
//      stPowerCtrlProj.stStartTime.ucMonth=1;
//      stPowerCtrlProj.stStartTime.ucDay=1;
//      stPowerCtrlProj.stStartTime.ucHour=0;
//      stPowerCtrlProj.stStartTime.ucMinute=0;
//      stPowerCtrlProj.stStartTime.ucSecond=0;
//      for(i=0;i<7;i++)
//      {//�޸����ڵ���Ϣ
//          ucTmp=(2<<i);
//          if((pucBuffer[5]&ucTmp)==0)
//              stPowerCtrlProj.ucAvailbleFlag[i]=CON_INVALIBLE_FLAG;
//          else
//              stPowerCtrlProj.ucAvailbleFlag[i]=CON_AVALIBLE_FLAG;
//      }
//      RunPara_SetPowCtrlProj(CONSWITCHCURVE_RESTCTRL,&stPowerCtrlProj,ucSumGroupNo);
//  }
//    return ucRet;
//}
//
////F43�����ʿ��ƵĹ��ʼ��㻬��ʱ��
//unsigned char Main_Recv_Set_F43_PowerSlip(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned char ucTmp,i;
//  unsigned char ucSumGroupNo=0;
//
//  STPOWERCTRLHOLDTIME stPowerHoldTime;
//  ucTmp=pucBuffer[0];
//  if(ucTmp==0 || ucTmp>60)
//      ucRet=RET_ERROR;
//
//  ucSumGroupNo=ucDAp;
//  if(ucSumGroupNo>=CON_SUMGROUP_NUM)
//      ucRet=RET_ERROR;
//
//  if(ucRet==RET_SUCCESS)
//  {
//      stPowerHoldTime=RunPara_GetPowCtrlHoldTime(ucSumGroupNo);
//      for(i=0;i<CON_PARA_SWITCHOUT_NUM;i++)
//      {
//          stPowerHoldTime.usJumpTime[i]=pucBuffer[0]*120;         //��λ1����ת��Ϊ��λ0.5��
//          stPowerHoldTime.usRevTime[i]=pucBuffer[0]*120;
//      }
//      RunPara_SetPowCtrlHoldTime(&stPowerHoldTime,ucSumGroupNo);
//  }
//    return ucRet;
//}
//
////F44��Ӫҵ��ͣ�ز���
//unsigned char Main_Recv_Set_F44_SaleStop(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned char m=0,i;
//  unsigned char ucSumGroupNo=0;
//  unsigned short usDT2=0;
//  float fVal;
//  unsigned long ulStart,ulEnd;
//  STPOWERCTRLTIME stPowerCtrlTime;
//  STPOWERCTRLPROJ stPowerCtrlProj;
//  STDATETIME stCurrentTime=GetCurrentTime();
//
//  ucSumGroupNo=ucDAp;
//  if(ucSumGroupNo>=CON_SUMGROUP_NUM)
//      ucRet=RET_ERROR;
//  if(Check_BCD_Data(pucBuffer[m])==RET_ERROR || Check_BCD_Data(pucBuffer[m+1])==RET_ERROR || Check_BCD_Data(pucBuffer[m+2])==RET_ERROR)
//      ucRet=RET_ERROR;
//  m+=3;
//  if(Check_BCD_Data(pucBuffer[m])==RET_ERROR || Check_BCD_Data(pucBuffer[m+1])==RET_ERROR || Check_BCD_Data(pucBuffer[m+2])==RET_ERROR)
//      ucRet=RET_ERROR;
//  m+=3;
//  if(Check_Data_DT02(pucBuffer+m)==RET_ERROR)
//      ucRet=RET_ERROR;
//  if(ucRet==RET_SUCCESS)
//  {
//      m=0;
//      stCurrentTime.ucSecond=0;
//      stCurrentTime.ucMinute=0;
//      stCurrentTime.ucHour=0;
//      stCurrentTime.ucDay=(unsigned char)BCD_TO_HEX_1(pucBuffer+m);   m++;
//      stCurrentTime.ucMonth=(unsigned char)BCD_TO_HEX_1(pucBuffer+m); m++;
//      stCurrentTime.ucYear=(unsigned char)BCD_TO_HEX_1(pucBuffer+m);  m++;
//      if(Is_DateTime_Availble(&stCurrentTime)==RET_ERROR)
//          ucRet=RET_ERROR;
//      ulStart=TimeConver(&stCurrentTime,CON_TIMEUNIT_DAY);
//      stCurrentTime.ucDay=(unsigned char)BCD_TO_HEX_1(pucBuffer+m);   m++;
//      stCurrentTime.ucMonth=(unsigned char)BCD_TO_HEX_1(pucBuffer+m); m++;
//      stCurrentTime.ucYear=(unsigned char)BCD_TO_HEX_1(pucBuffer+m);  m++;
//      if(Is_DateTime_Availble(&stCurrentTime)==RET_ERROR)
//          ucRet=RET_ERROR;
//      ulEnd=TimeConver(&stCurrentTime,CON_TIMEUNIT_DAY);
//      if(ulStart>ulEnd)
//          ucRet=RET_ERROR;
//  }
//  if(ucRet==RET_SUCCESS)
//  {
//      m=0;
//      stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_SALESTOP,ucSumGroupNo);
//      stCurrentTime.ucDay=(unsigned char)BCD_TO_HEX_1(pucBuffer+m);   m++;
//      stCurrentTime.ucMonth=(unsigned char)BCD_TO_HEX_1(pucBuffer+m); m++;
//      stCurrentTime.ucYear=(unsigned char)BCD_TO_HEX_1(pucBuffer+m);  m++;
//      stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_SALESTOP,ucSumGroupNo);
//      stPowerCtrlProj.stStartTime=stCurrentTime;
//      stCurrentTime.ucDay=(unsigned char)BCD_TO_HEX_1(pucBuffer+m);   m++;
//      stCurrentTime.ucMonth=(unsigned char)BCD_TO_HEX_1(pucBuffer+m); m++;
//      stCurrentTime.ucYear=(unsigned char)BCD_TO_HEX_1(pucBuffer+m);  m++;
//      stPowerCtrlProj.stEndTime=stCurrentTime;
//      stPowerCtrlProj.ucAvailbleFlag[0]=CON_AVALIBLE_FLAG;
//      stPowerCtrlProj.ucCycleType=CON_CYCLETYPE_DAY;
//      ulStart=TimeConver(&stPowerCtrlProj.stStartTime,CON_TIMEUNIT_DAY);
//      ulEnd=TimeConver(&stPowerCtrlProj.stEndTime,CON_TIMEUNIT_DAY);
//      stPowerCtrlProj.usCycleNum=(unsigned short)(ulEnd-ulStart);
//      RunPara_SetPowCtrlProj(CONSWITCHCURVE_SALESTOP,&stPowerCtrlProj,ucSumGroupNo);
//      usDT2=pucBuffer[m]+0x100*pucBuffer[m+1];    m+=2;
//      fVal=DT2_To_Float(usDT2);
//      if(fVal<0)
//          fVal=0-fVal;
//      stPowerCtrlTime.stPowerCtrlPer[0].ucAvailbleFlag=CON_AVALIBLE_FLAG;
//      stPowerCtrlTime.stPowerCtrlPer[0].ucCtrlFlag=CON_AVALIBLE_FLAG;
//      stPowerCtrlTime.stPowerCtrlPer[0].stLoadFixVal.dFixVal=fVal;
//      stPowerCtrlTime.stPowerCtrlPer[0].stLoadFixVal.usDT2=usDT2;
//      stPowerCtrlTime.stPowerCtrlPer[0].ucBeginMinute=0;
//      stPowerCtrlTime.stPowerCtrlPer[0].ucBeginHour=0;
//      stPowerCtrlTime.stPowerCtrlPer[1].ucAvailbleFlag=CON_AVALIBLE_FLAG;
//      stPowerCtrlTime.stPowerCtrlPer[1].ucCtrlFlag=CON_INVALIBLE_FLAG;
//      stPowerCtrlTime.stPowerCtrlPer[1].ucBeginMinute=0;
//      stPowerCtrlTime.stPowerCtrlPer[1].ucBeginHour=0;
//      for(i=2;i<CONSWITCHPERTIMENUM;i++)
//      {
//          stPowerCtrlTime.stPowerCtrlPer[i].ucAvailbleFlag=CON_INVALIBLE_FLAG;
//          stPowerCtrlTime.stPowerCtrlPer[i].ucCtrlFlag=CON_INVALIBLE_FLAG;
//      }
//      RunPara_SetPowCtrlTimePer(CONSWITCHCURVE_SALESTOP,&stPowerCtrlTime,ucSumGroupNo);
//  }
//    return ucRet;
//}
//
//F45�������ִ��趨
//unsigned char Main_Recv_Set_F45_PowerCtrlTrun(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned char i,ucTmp;
//  unsigned char ucSumGroupNo=0;
//  STCTRLTURN  stPowerCtrlTurn;
//
//  ucSumGroupNo=ucDAp;
//  if(ucSumGroupNo>=CON_SUMGROUP_NUM)
//      return RET_ERROR;
//
//  stPowerCtrlTurn=RunPara_GetPowerCtrlTurn(ucSumGroupNo);
//  ucTmp=pucBuffer[0];
//  for(i=0;i<CON_PARA_SWITCHOUT_NUM;i++)
//  {
//      if((ucTmp&0x01)==0x01)
//          stPowerCtrlTurn.ucAvailbleFlag[i]=CON_AVALIBLE_FLAG;
//      else
//          stPowerCtrlTurn.ucAvailbleFlag[i]=CON_INVALIBLE_FLAG;
//      ucTmp=(ucTmp>>1);
//  }
//  ucRet=RunPara_SetPowerCtrlTurn(&stPowerCtrlTurn,ucSumGroupNo);
//  return ucRet;
//}

//F46���µ����ض�ֵ
//unsigned char Main_Recv_Set_F46_MonthEnerCtrl(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  STMONENERGYREC stMonEnergyRec;
//  STENERGYCTRL  stEnergyCtrl;
//  unsigned char ucSumGroupNo;
//  ucSumGroupNo=ucDAp;
//  if(ucSumGroupNo>=CON_SUMGROUP_NUM)
//      ucRet=RET_ERROR;
//  if(Check_Data_DT03(pucBuffer)==RET_ERROR)
//      ucRet=RET_ERROR;
//  if((pucBuffer[3]&0x10)==0x10)
//      ucRet=RET_ERROR;                        //����������
//  if(ucRet==RET_SUCCESS)
//  {
//      stMonEnergyRec=RunPara_GetMonEnergyRec(ucSumGroupNo);
//      stMonEnergyRec.stMonFixVal.dFixVal=DT3_To_Double(pucBuffer);
//      stMonEnergyRec.stMonFixVal.usDT2=pucBuffer[3]&0x40;
//
//      RunPara_SetMonEnergyRec(&stMonEnergyRec,ucSumGroupNo);
//      //����
//      stEnergyCtrl=RunPara_GetEnergyCtrl(ucSumGroupNo);
//      if(stEnergyCtrl.ucEnergyCtrlType==CON_ENERGYCTRL_MON)
//      {
//          stEnergyCtrl.stMonthEnergy.dFixVal=stMonEnergyRec.stMonFixVal.dFixVal;
//          stEnergyCtrl.stAlrFixVal.dFixVal=stEnergyCtrl.stMonthEnergy.dFixVal*20/100;             //�ﵽ80%�澯
//          stEnergyCtrl.stJumpFixVal.dFixVal=0;
//      }
//      RunPara_SetEnergyCtrl(&stEnergyCtrl,ucSumGroupNo);
//  }
//  return ucRet;
//}

//F47���������ز���
//unsigned char Main_Recv_Set_F47_BuyEnerCtrl(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned char m=0;
//  double dLeftPowVal1,dLeftPowVal2;
//  STBUYENERGYREC stBuyEnergyRec;
//  STENERGYCTRL stEnergyCtrl;
//  unsigned char ucSumGroupNo;
//  unsigned char i=0;
//  ucSumGroupNo=ucDAp;
//  if(ucSumGroupNo>=CON_SUMGROUP_NUM)
//      ucRet=RET_ERROR;
//  if(pucBuffer[4]!=0x55 && pucBuffer[4]!=0xAA)
//      ucRet=RET_ERROR;
//  m+=5;
//  if(Check_Data_DT03(pucBuffer+m)==RET_ERROR)
//      ucRet=RET_ERROR;
//  //  if((pucBuffer[m+3]&0x10)==0x10)
//  //      ucRet=RET_ERROR;                        //����������
//  m+=4;
//  if(Check_Data_DT03(pucBuffer+m)==RET_ERROR)
//      ucRet=RET_ERROR;
//  m+=4;
//  if(Check_Data_DT03(pucBuffer+m)==RET_ERROR)
//      ucRet=RET_ERROR;
////    m+=4;
////    if(Check_Data_DT03(pucBuffer+m)==RET_ERROR)
////        ucRet=RET_ERROR;
////    if((pucBuffer[m+3]&0x10)==0x10)             //�ִε�������Ϊ��
////        ucRet=RET_ERROR;                        //����������
//  if(ucRet==RET_SUCCESS)
//  {
//      m=0;
//      stBuyEnergyRec=RunPara_GetBuyEnergyRec(ucSumGroupNo);
//      memcpy(&(stBuyEnergyRec.ulBuyBillNo),pucBuffer,4);
//      m+=4;
//      stBuyEnergyRec.ucFlashFlag=pucBuffer[m++];
//      stBuyEnergyRec.stBuyFixVal.usDT2    =pucBuffer[m+3]&0x40;
//      stBuyEnergyRec.stBuyFixVal.dFixVal=DT3_To_Double(pucBuffer+m);
//      m+=4;
//      stBuyEnergyRec.stAlrFixVal.usDT2    =pucBuffer[m+3]&0x40;
//      stBuyEnergyRec.stAlrFixVal.dFixVal=DT3_To_Double(pucBuffer+m);
//      m+=4;
//      stBuyEnergyRec.stJumpFixVal.usDT2   =pucBuffer[m+3]&0x40;
//      stBuyEnergyRec.stJumpFixVal.dFixVal=DT3_To_Double(pucBuffer+m);
//  //  m+=4;
//  //  stBuyEnergyRec.stTurnFixVal.usDT2   =pucBuffer[m+3]&0x40;
//  //  stBuyEnergyRec.stTurnFixVal.dFixVal=DT3_To_Double(pucBuffer+m);
//
//      if(stBuyEnergyRec.stAlrFixVal.dFixVal<stBuyEnergyRec.stJumpFixVal.dFixVal)
//          ucRet=RET_ERROR;
//      if(stBuyEnergyRec.ucFlashFlag==CON_ENERGYFLASH_NEW && stBuyEnergyRec.stBuyFixVal.dFixVal<stBuyEnergyRec.stAlrFixVal.dFixVal)
//          ucRet=RET_ERROR;
//      if(ucRet==RET_SUCCESS)
//          ucRet=RunPara_Manage_BuyBillNo(stBuyEnergyRec.ulBuyBillNo,stBuyEnergyRec.ucFlashFlag);
//      if(ucRet==RET_SUCCESS)
//      {
//          //����
//          stEnergyCtrl=RunPara_GetEnergyCtrl(ucSumGroupNo);
//          dLeftPowVal1 = Load_Energy_LeftPowVal_CalcBuy(&stEnergyCtrl,ucSumGroupNo);
//          if(stEnergyCtrl.ucEnergyCtrlType==CON_ENERGYCTRL_BUY)
//          {
//              if(stBuyEnergyRec.ucFlashFlag==CON_ENERGYFLASH_NEW)
//              {
//                  stEnergyCtrl.stBuyEnergy.dFixVal=stBuyEnergyRec.stBuyFixVal.dFixVal;
//              }
//              else
//              {
//                  stEnergyCtrl.stBuyEnergy.dFixVal = dLeftPowVal1 + stBuyEnergyRec.stBuyFixVal.dFixVal;
//              }
//              Load_SetEnergy_StaPowVal();
//              stEnergyCtrl.stAlrFixVal.dFixVal=stBuyEnergyRec.stAlrFixVal.dFixVal;
//              stEnergyCtrl.stJumpFixVal.dFixVal=stBuyEnergyRec.stJumpFixVal.dFixVal;
//              stEnergyCtrl.stTurnFixVal.dFixVal=stBuyEnergyRec.stTurnFixVal.dFixVal;
//          }else
//          {//δͶ��
//              if(stBuyEnergyRec.ucFlashFlag==CON_ENERGYFLASH_NEW)
//                  stEnergyCtrl.stBuyEnergy.dFixVal = stBuyEnergyRec.stBuyFixVal.dFixVal;
//              else
//                  stEnergyCtrl.stBuyEnergy.dFixVal += stBuyEnergyRec.stBuyFixVal.dFixVal;
//          }
//          RunPara_SetEnergyCtrl(&stEnergyCtrl,ucSumGroupNo);
//
//          dLeftPowVal2 = Load_Energy_LeftPowVal_CalcBuy(&stEnergyCtrl,ucSumGroupNo);
//          Alarm_EnsureJudge_BuyPowEnerRec((unsigned char *)&stBuyEnergyRec,dLeftPowVal1,dLeftPowVal2);
//
//          stBuyEnergyRec.dLeftVal_befor=dLeftPowVal1;
//          stBuyEnergyRec.dLeftVal_after=dLeftPowVal2;
//          RunPara_SetBuyEnergyRec(&stBuyEnergyRec,ucSumGroupNo);
//      }
//  }
//  return ucRet;
//}

//F48������ִ��趨
//unsigned char Main_Recv_Set_F48_EnerCtrlTurn(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned char i,ucTmp;
//  unsigned char ucSumGNo=0;
//  STCTRLTURN  stEnergyCtrlTurn;
//
//  ucSumGNo=ucDAp;
//  if(ucSumGNo>=CON_SUMGROUP_NUM)
//      return  RET_ERROR;
//  stEnergyCtrlTurn=RunPara_GetEnergyCtrlTurn(ucSumGNo);
//  ucTmp=pucBuffer[0];
//  for(i=0;i<CON_PARA_SWITCHOUT_NUM;i++)
//  {
//      if((ucTmp&0x01)==0x01)
//          stEnergyCtrlTurn.ucAvailbleFlag[i]=CON_AVALIBLE_FLAG;
//      else
//          stEnergyCtrlTurn.ucAvailbleFlag[i]=CON_INVALIBLE_FLAG;
//      ucTmp=(ucTmp>>1);
//  }
//  ucRet=RunPara_SetEnergyCtrlTurn(&stEnergyCtrlTurn,ucSumGNo);
//  return ucRet;
//}

//F49�����ظ澯ʱ��
//unsigned char Main_Recv_Set_F49_CtrlAlrTime(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned char i=0;
//  unsigned char ucCtrlTurn;
//  STPOWERCTRLHOLDTIME stPowerHoldTime;
//  ucCtrlTurn=ucDAp;
//  if(ucCtrlTurn>=CON_PARA_SWITCHOUT_NUM)
//      ucRet=RET_ERROR;
//  if(ucRet==RET_SUCCESS)
//  {
//      if(pucBuffer[0]<1)
//          pucBuffer[0]=1;             //���Ʋ�С��1
//      for(i=0;i<CON_SUMGROUP_NUM;i++)
//      {
//          stPowerHoldTime=RunPara_GetPowCtrlHoldTime(i);
//          stPowerHoldTime.usJumpAlrTime[ucCtrlTurn]=pucBuffer[0]*120;         //��λ1����ת��Ϊ��λ0.5��
//          stPowerHoldTime.usRevAlrTime[ucCtrlTurn]=pucBuffer[0]*120;
//          ucRet=RunPara_SetPowCtrlHoldTime(&stPowerHoldTime,i);
//      }
//  }
//  return ucRet;
//}

//F57���ն������澯����M��ֹ����
unsigned char Main_Recv_Set_F57_SoundAlarm(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STTERMISCPARA  stTerMiscPara;
    stTerMiscPara=RunPara_GetTerMiscPara();
    stTerMiscPara.ucAlarmSoundTime[0]=pucBuffer[0];
    stTerMiscPara.ucAlarmSoundTime[1]=pucBuffer[1];
    stTerMiscPara.ucAlarmSoundTime[2]=pucBuffer[2];
    ucRet=RunPara_SetTerMiscPara(&stTerMiscPara);
//azh   if(ucRet==RET_SUCCESS)
//      Beep_ExternPara_Change();
    return ucRet;
}

//F58���ն��Զ��������
//unsigned char Main_Recv_Set_F58_KeepAuto(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  STTERMISCPARA  stTerMiscPara;
//  stTerMiscPara=RunPara_GetTerMiscPara();
//  stTerMiscPara.ucKeepAuto=pucBuffer[0];
//  ucRet=RunPara_SetTerMiscPara(&stTerMiscPara);
//  return ucRet;
//}

//F59�����ܱ��쳣�б���ֵ�趨
unsigned char Main_Recv_Set_F59_MtErrorPara(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STTERMISCPARA stTerMiscPara;
    stTerMiscPara=RunPara_GetTerMiscPara();
    if((Check_BCD_Data(pucBuffer[0])==RET_SUCCESS) && (Check_BCD_Data(pucBuffer[1])==RET_SUCCESS))
    {
        stTerMiscPara.ucMeterOverDiff=(unsigned char)BCD_TO_HEX_1(pucBuffer);
        stTerMiscPara.ucMeterFly=(unsigned char)BCD_TO_HEX_1(pucBuffer+1);
        stTerMiscPara.ucMeterStop=pucBuffer[2];
        stTerMiscPara.ucMeterTimeDiff=pucBuffer[3];
        ucRet=RunPara_SetTerMiscPara(&stTerMiscPara);
    }else
        ucRet=RET_ERROR;
    return ucRet;
}
//unsigned char Main_Recv_Set_F60_HarmLimit(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet = RET_SUCCESS;
//  unsigned char i;
//  unsigned short m =0;
//  unsigned short *pusVal=NULL;
//  STHARMVALUPLIMIT stHarmValLimit;
//
//  for(i=0;i<80;i++)
//  {
//      if(Check_BCD_Data(pucBuffer[m++]) != RET_SUCCESS)
//      {
//          ucRet = RET_ERROR;
//          break;
//      }
//  }
//  if(ucRet == RET_SUCCESS)
//  {
//      m = 0;
//      pusVal = &stHarmValLimit.stVolRateLimit.usHarmValUpTotal;
//      for(i=0;i<21;i++)
//      {
//          *pusVal = (unsigned short)BCD_TO_HEX_2(pucBuffer+m);
//          m += 2;
//          pusVal ++;
//      }
//      stHarmValLimit.stCurRmsLimit.usHarmValUpTotal = (unsigned short)BCD_TO_HEX_2(pucBuffer+m);
//      m += 2;
//      pusVal = &stHarmValLimit.stCurRmsLimit.usHarmValUpLimit2;
//      for(i=0;i<18;i++)
//      {
//          *pusVal = (unsigned short)BCD_TO_HEX_2(pucBuffer+m);
//          m += 2;
//          pusVal ++;
//      }
//      RunPara_SetHarmValUpLimit(&stHarmValLimit);
//  }
//    return ucRet;
//}
//unsigned char Main_Recv_Set_F61_Aanalog(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//  unsigned char ucRet = RET_SUCCESS;
//  unsigned char ucDataTmp,i;
//  unsigned char ucTmp;
//  unsigned short m= 0;
//  STANALOGPARA stAnalogPara;
//
//  ucDataTmp = pucBuffer[m++];
//  for(i=0;i<CON_ANALOG_NUM;i++)
//  {
//      stAnalogPara = RunPara_GetAnalogPara(i);
//      ucTmp = (1<<i);
//      if((ucDataTmp & ucTmp) == ucTmp)
//          stAnalogPara.ucAvailable = CON_AVALIBLE_FLAG;
//      else
//          stAnalogPara.ucAvailable = CON_INVALIBLE_FLAG;
//      RunPara_SetAnalogPara(i, &stAnalogPara);
//  }
//  return ucRet;
//}
//F62 ����ר��������ʽ
unsigned char Main_Recv_Set_F62_CommType(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned short m=0;

    unsigned char ucTmp,ucTmp1;
    STTERMISCPARA   stTerMiscPara;

    STTERCOMMPARA_INNER     stTerCommPara_Inner=RunPara_GetTerCommPara_Inner();
    stTerMiscPara=RunPara_GetTerMiscPara();

//  ucTmp=pucBuffer[m++];
    ucTmp1=pucBuffer[m++];
    if((ucTmp1 & 0x80) == 0x80)
    {
        stTerMiscPara.ucRcommBaseType = CON_TC35COMMTYPE_UDP;
        stTerMiscPara.ucIPInfoType[0] = CON_TC35COMMTYPE_UDP;
        stTerMiscPara.ucIPInfoType[1] = CON_TC35COMMTYPE_TCP;
    }
    else
    {
        stTerMiscPara.ucRcommBaseType = CON_TC35COMMTYPE_TCP;
        stTerMiscPara.ucIPInfoType[0] = CON_TC35COMMTYPE_TCP;
        stTerMiscPara.ucIPInfoType[1] = CON_TC35COMMTYPE_UDP;
    }
    ucTmp=ucTmp1&0x03;
    if((ucTmp==CON_ONLINE_ALLWAYS) || (ucTmp==CON_ONLINE_SMS_UP) || (ucTmp==CON_ONLINE_TIME_UP))        //����2��ͨѶ��ʽ
    {
        stTerCommPara_Inner.ucRCommMode=ucTmp;
        stTerCommPara_Inner.usReConnectInter=pucBuffer[m]+0x100*pucBuffer[m+1];
        m+=2;
        stTerCommPara_Inner.ucReConnectTimes=pucBuffer[m++];
        stTerCommPara_Inner.ucStandbyTime=pucBuffer[m++];
        memcpy(stTerCommPara_Inner.ucTimeInt,pucBuffer+m,3);
        RunPara_SetTerCommPara_Inner(&stTerCommPara_Inner);
        if(ucTmp==CON_ONLINE_SMS_UP)//��Ҫ���������
            Main_RComm_UpStop();
        else
            Main_RComm_UpStart();
        Main_Init_LoginCount();

        RunPara_SetTerMiscPara(&stTerMiscPara);
    }else
        ucRet=RET_ERROR;

    return ucRet;
}

//F63 ��ͨ��������������
unsigned char Main_Recv_Set_F63_FluxLimit(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned long ulOldVal=0,ulNewVal=0;
    ulOldVal=RunPara_GetTerMisc_FluxLimit();
    memcpy(&ulNewVal,pucBuffer,4);
    RunPara_SetTerMiscPara_FluxLimit(ulNewVal);
    DataProc_Stat_FluxBytes_LimitChange(ulOldVal,ulNewVal);
    return ucRet;
}
//F65����ʱ����1��������������
unsigned char Main_Recv_Set_F65_UpData1Para(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS,i=0;
    unsigned char   ucJobNo,ucTmp=0;
    STJOBPARA   *pstJobPara_Data1;//stJobPara_Data1;
    unsigned short  m=0;
    if(ucRet==RET_SUCCESS)
    {
        ucJobNo=ucDAp;
        if(ucJobNo>=CONDATAJOBNUM)
            ucRet =RET_ERROR;
        for(i=0;i<6;i++)
        {
            ucTmp=pucBuffer[m+1+i];             //ʱ��
            if(i==4)                            //�������ڸ���Ҫ����
                ucTmp=ucTmp&0x1F;
            if(Check_BCD_Data(ucTmp)!=RET_SUCCESS)
            {
                ucRet=RET_ERROR;
                break;
            }
        }
    }
    if(ucRet==RET_SUCCESS)
    {
        pstJobPara_Data1=RunPara_GetJobPara(ucJobNo,0);
        ucTmp=pucBuffer[m++];
        pstJobPara_Data1->ucTimeInter=ucTmp&0x3F;
        ucTmp=(ucTmp>>6);
        switch(ucTmp)
        {
        case 0:
            pstJobPara_Data1->ucTimeUnit = CON_TIMEUNIT_MIN;
            break;
        case 1:
            pstJobPara_Data1->ucTimeUnit = CON_TIMEUNIT_HOUR;
            break;
        case 2:
            pstJobPara_Data1->ucTimeUnit = CON_TIMEUNIT_DAY;
            break;
        case 3:
            pstJobPara_Data1->ucTimeUnit = CON_TIMEUNIT_MON;
            break;
        default:
            pstJobPara_Data1->ucTimeUnit=CON_TIMEUNIT_MIN;
            break;
        }
        pstJobPara_Data1->stStartActive.ucSecond=(unsigned char)BCD_TO_HEX_1(&pucBuffer[m++]);
        pstJobPara_Data1->stStartActive.ucMinute=(unsigned char)BCD_TO_HEX_1(&pucBuffer[m++]);
        pstJobPara_Data1->stStartActive.ucHour=(unsigned char)BCD_TO_HEX_1(&pucBuffer[m++]);
        pstJobPara_Data1->stStartActive.ucDay=(unsigned char)BCD_TO_HEX_1(&pucBuffer[m++]);
        ucTmp=pucBuffer[m]&0x1F;
        pstJobPara_Data1->stStartActive.ucMonth=(unsigned char)BCD_TO_HEX_1(&ucTmp);
        ucTmp=pucBuffer[m++]>>5;
        pstJobPara_Data1->stStartActive.ucYear=(unsigned char)BCD_TO_HEX_1(&pucBuffer[m++]);
    //  if(ucTmp!=GetCurrentWeek(&stJobPara_Data1.stStartActive) && ucTmp!=0)
    //      ucRet=RET_ERROR;            //changed for test 09-11-25
        if(ucRet==RET_SUCCESS)
        {
            pstJobPara_Data1->ucResendDataInter=pucBuffer[m++];
            pstJobPara_Data1->ucDUIDNum=pucBuffer[m++];
            if(pstJobPara_Data1->ucDUIDNum>CONDATAUNITNUM)
            {
                pstJobPara_Data1->ucDUIDNum = CONDATAUNITNUM;
            }
//            if(pstJobPara_Data1->ucDUIDNum<=CONDATAUNITNUM)
//            {
                for(i=0;i<pstJobPara_Data1->ucDUIDNum;i++)
                {
                    pstJobPara_Data1->stDUID[i].ucDA1=pucBuffer[m++];
                    pstJobPara_Data1->stDUID[i].ucDA2=pucBuffer[m++];
                    pstJobPara_Data1->stDUID[i].ucDT1=pucBuffer[m++];
                    pstJobPara_Data1->stDUID[i].ucDT2=pucBuffer[m++];
                    //if(Check_DUID_IsSingle(stJobPara_Data1.stDUID[i])!=RET_SUCCESS)
                    //{
                    //  ucRet=RET_ERROR;
                    //  break;
                    //}
                }

//            }else
//                ucRet=RET_ERROR;
        }
        if(ucRet==RET_SUCCESS)
        {
            RunPara_InitJobRunInfoPer(pstJobPara_Data1);
//            RunPara_SetJobPara(0,ucJobNo,&stJobPara_Data1);
            SaveJobRunInfo(0,ucJobNo);//azh ���������´�����ʱ��
        }
    }

    return ucRet;
}

//F66����ʱ����2��������������
unsigned char Main_Recv_Set_F66_UpData2Para(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS,i=0;
    unsigned char   ucJobNo,ucTmp=0;
    STJOBPARA   *pstJobPara_Data2;//stJobPara_Data2;
    unsigned short  m=0;
    if(ucRet==RET_SUCCESS)
    {
        ucJobNo=ucDAp;
        if(ucJobNo>=CONDATAJOBNUM)
            ucRet =RET_ERROR;
        for(i=0;i<6;i++)
        {
            ucTmp=pucBuffer[m+1+i];
            if(i==4)                            //�������ڸ���Ҫ����
                ucTmp=ucTmp&0x1F;
            if(Check_BCD_Data(ucTmp)!=RET_SUCCESS)
            {
                ucRet=RET_ERROR;
                break;
            }
        }
    }
    if(ucRet==RET_SUCCESS)
    {
        pstJobPara_Data2=RunPara_GetJobPara(ucJobNo,1);
        ucTmp=pucBuffer[m++];
        pstJobPara_Data2->ucTimeInter=ucTmp&0x3F;
        ucTmp=(ucTmp>>6);
        switch(ucTmp)
        {
        case 0:
            pstJobPara_Data2->ucTimeUnit=CON_TIMEUNIT_MIN;
            break;
        case 1:
            pstJobPara_Data2->ucTimeUnit=CON_TIMEUNIT_HOUR;
            break;
        case 2:
            pstJobPara_Data2->ucTimeUnit=CON_TIMEUNIT_DAY;
            break;
        case 3:
            pstJobPara_Data2->ucTimeUnit=CON_TIMEUNIT_MON;
            break;
        default:
            pstJobPara_Data2->ucTimeUnit=CON_TIMEUNIT_MIN;
            break;
        }
        pstJobPara_Data2->stStartActive.ucSecond=(unsigned char)BCD_TO_HEX_1(&pucBuffer[m++]);
        pstJobPara_Data2->stStartActive.ucMinute=(unsigned char)BCD_TO_HEX_1(&pucBuffer[m++]);
        pstJobPara_Data2->stStartActive.ucHour=(unsigned char)BCD_TO_HEX_1(&pucBuffer[m++]);
        pstJobPara_Data2->stStartActive.ucDay=(unsigned char)BCD_TO_HEX_1(&pucBuffer[m++]);
        ucTmp=pucBuffer[m]&0x1F;
        pstJobPara_Data2->stStartActive.ucMonth=(unsigned char)BCD_TO_HEX_1(&ucTmp);
        ucTmp=pucBuffer[m++]>>5;
        pstJobPara_Data2->stStartActive.ucYear=(unsigned char)BCD_TO_HEX_1(&pucBuffer[m++]);
//      if(ucTmp!=GetCurrentWeek(&stJobPara_Data2.stStartActive) && ucTmp!=0)
//          ucRet=RET_ERROR;        //changed for test 09-11-25
        if(ucRet==RET_SUCCESS)
        {
            pstJobPara_Data2->ucResendDataInter=pucBuffer[m++];
            pstJobPara_Data2->ucDUIDNum=pucBuffer[m++];
            if(pstJobPara_Data2->ucDUIDNum>CONDATAUNITNUM)
            {
                pstJobPara_Data2->ucDUIDNum = CONDATAUNITNUM;
            }
//            if(stJobPara_Data2.ucDUIDNum<=CONDATAUNITNUM)
//            {
                for(i=0;i<pstJobPara_Data2->ucDUIDNum;i++)
                {
                    pstJobPara_Data2->stDUID[i].ucDA1=pucBuffer[m++];
                    pstJobPara_Data2->stDUID[i].ucDA2=pucBuffer[m++];
                    pstJobPara_Data2->stDUID[i].ucDT1=pucBuffer[m++];
                    pstJobPara_Data2->stDUID[i].ucDT2=pucBuffer[m++];
                    //if(Check_DUID_IsSingle(stJobPara_Data2.stDUID[i])!=RET_SUCCESS)
                    //{
                    //  ucRet=RET_ERROR;
                    //  break;
                    //}
                }
//            }else
//                ucRet=RET_ERROR;
        }
        if(ucRet==RET_SUCCESS)
        {
            RunPara_InitJobRunInfoPer(pstJobPara_Data2);
//            RunPara_SetJobPara(1,ucJobNo,&stJobPara_Data2);//azh �Ѿ����õ�ȫ�ֱ���
            SaveJobRunInfo(1,ucJobNo);//azh ���������´�����ʱ��
        }
    }
    return ucRet;
}

//F67����ʱ����1��������������/ֹͣ����
unsigned char Main_Recv_Set_F67_UpData1Ctrl(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS;
    unsigned char   ucJobNo,ucTmp=0;
    STJOBPARA   *pstJobPara_Data1;//stJobPara_Data1;
    if(ucRet==RET_SUCCESS)
    {
        ucJobNo=ucDAp;
        if(ucJobNo>=CONDATAJOBNUM)
            ucRet =RET_ERROR;
    }
    if(ucRet==RET_SUCCESS)
    {
        pstJobPara_Data1=RunPara_GetJobPara(ucJobNo,0);
        ucTmp=pucBuffer[0];
		file_write(F67_UPDATA1CTRL_ADDR + ucJobNo, pucBuffer, 1);    //д��EEPROM
        if(ucTmp==0x55 && pstJobPara_Data1->ucActive!=CON_JOB_ACTIVE)
        {
            pstJobPara_Data1->ucActive=CON_JOB_ACTIVE;
            RunPara_InitJobRunInfoPer(pstJobPara_Data1);
//            RunPara_SetJobPara(0,ucJobNo,&stJobPara_Data1);
            SaveJobRunInfo(0,ucJobNo);//azh ���������´�����ʱ��
        }else if(ucTmp==0xAA && pstJobPara_Data1->ucActive!=CON_JOB_NOACTIVE)
        {
            pstJobPara_Data1->ucActive=CON_JOB_NOACTIVE;
            RunPara_InitJobRunInfoPer(pstJobPara_Data1);
//            RunPara_SetJobPara(0,ucJobNo,&stJobPara_Data1);
            SaveJobRunInfo(0,ucJobNo);//azh ���������´�����ʱ��
        }
    }
    return ucRet;
}

//F68����ʱ����2��������������/ֹͣ����
unsigned char Main_Recv_Set_F68_UpData2Ctrl(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS;
    unsigned char   ucJobNo,ucTmp=0;
    STJOBPARA   *pstJobPara_Data2;
    if(ucRet==RET_SUCCESS)
    {
        ucJobNo=ucDAp;
        if(ucJobNo>=CONDATAJOBNUM)
            ucRet =RET_ERROR;
    }
    if(ucRet==RET_SUCCESS)
    {
        pstJobPara_Data2=RunPara_GetJobPara(ucJobNo,1);
        ucTmp=pucBuffer[0];
		file_write(F68_UPDATA2CTRL_ADDR + ucJobNo, pucBuffer, 1);    //д��EEPROM
        if(ucTmp==0x55 && pstJobPara_Data2->ucActive!=CON_JOB_ACTIVE)
        {
            pstJobPara_Data2->ucActive=CON_JOB_ACTIVE;
            RunPara_InitJobRunInfoPer(pstJobPara_Data2);
//            RunPara_SetJobPara(1,ucJobNo,&stJobPara_Data2);
            SaveJobRunInfo(1,ucJobNo);//azh ���������´�����ʱ��
        }else if(ucTmp==0xAA && pstJobPara_Data2->ucActive!=CON_JOB_NOACTIVE)
        {
            pstJobPara_Data2->ucActive=CON_JOB_NOACTIVE;
            RunPara_InitJobRunInfoPer(pstJobPara_Data2);
//            RunPara_SetJobPara(1,ucJobNo,&stJobPara_Data2);
            SaveJobRunInfo(1,ucJobNo);//azh ���������´�����ʱ��
        }
    }
    return ucRet;
}


//add by cfh 10-04-03   F73-F76
//unsigned char Main_Recv_Set_F73_F76(unsigned char *pucBuffer,unsigned char ucDAp,unsigned short usDI)
//{
//  unsigned char   ucRet=RET_SUCCESS;
//  switch(usDI)
//  {
//      case CON_PARA_F73_CAPPARA:
//          memcpy(stTerRunPara_Nouse.ucF73,pucBuffer,48);
//          break;
//      case CON_PARA_F74_CAPRUNPARA:
//          memcpy(stTerRunPara_Nouse.ucF74,pucBuffer,10);
//          break;
//      case CON_PARA_F75_CAPRELAYPARA:
//          memcpy(stTerRunPara_Nouse.ucF75,pucBuffer,16);
//          break;
//      case CON_PARA_F76_CAPCTRL:
//  //      memcpy(stTerRunPara_Nouse.ucF76,pucBuffer,48);
//          stTerRunPara_Nouse.ucF76 = pucBuffer[0];
//          break;
//  }
//  return ucRet;
//}
//
//unsigned char Main_Recv_Set_F81_DCRate(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//  unsigned char ucRet = RET_SUCCESS;
//  unsigned char ucAnalogNo=0;
//  unsigned short usDT2=0,m=0;
//  float fVal = 0;
//  STANALOGPARA    stAnalogPara;
//  ucAnalogNo = ucDAp;
//  if(ucAnalogNo >= CON_ANALOG_NUM)
//      ucRet = RET_ERROR;
//  if(ucRet == RET_SUCCESS)
//  {
//      m = 0;
//      ucRet = Check_Data_DT02(pucBuffer+m);
//      m += 2;
//      if(ucRet == RET_SUCCESS)
//      {
//          ucRet = Check_Data_DT02(pucBuffer+m);
//          m += 2;
//
//      }
//  }
//  if(ucRet == RET_SUCCESS)
//  {
//      stAnalogPara = RunPara_GetAnalogPara(ucAnalogNo);
//      m = 0;
//      usDT2 = pucBuffer[m] + (pucBuffer[m+1]<<8);
//      m += 2;
//      fVal = DT2_To_Float(usDT2);
//      stAnalogPara.stAnalogRate.stStart.usDT2 = usDT2;
//      stAnalogPara.stAnalogRate.stStart.dVal = fVal;
//      usDT2 = pucBuffer[m] + (pucBuffer[m+1]<<8);
//      m += 2;
//      fVal = DT2_To_Float(usDT2);
//      stAnalogPara.stAnalogRate.stEnd.usDT2 = usDT2;
//      stAnalogPara.stAnalogRate.stEnd.dVal = fVal;
//      RunPara_SetAnalogPara(ucAnalogNo, &stAnalogPara);
//  }
//  return ucRet;
//}
//unsigned char Main_Recv_Set_F82_DCLimit(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//  unsigned char ucRet = RET_SUCCESS;
//  unsigned char ucAnalogNo=0;
//  unsigned short usDT2=0,m=0;
//  float fVal = 0;
//  STANALOGPARA    stAnalogPara;
//  ucAnalogNo = ucDAp;
//  if(ucAnalogNo >= CON_ANALOG_NUM)
//      ucRet = RET_ERROR;
//  if(ucRet == RET_SUCCESS)
//  {
//      m = 0;
//      ucRet = Check_Data_DT02(pucBuffer+m);
//      m += 2;
//      if(ucRet == RET_SUCCESS)
//      {
//          ucRet = Check_Data_DT02(pucBuffer+m);
//          m += 2;
//
//      }
//  }
//  if(ucRet == RET_SUCCESS)
//  {
//      stAnalogPara = RunPara_GetAnalogPara(ucAnalogNo);
//      m = 0;
//      usDT2 = pucBuffer[m] + (pucBuffer[m+1]<<8);
//      m += 2;
//      fVal = DT2_To_Float(usDT2);
//      stAnalogPara.stAnalogLimit.stUpLimit.usDT2= usDT2;
//      stAnalogPara.stAnalogLimit.stUpLimit.dVal = fVal;
//      usDT2 = pucBuffer[m] + (pucBuffer[m+1]<<8);
//      m += 2;
//      fVal = DT2_To_Float(usDT2);
//      stAnalogPara.stAnalogLimit.stDownLimit.usDT2 = usDT2;
//      stAnalogPara.stAnalogLimit.stDownLimit.dVal = fVal;
//      RunPara_SetAnalogPara(ucAnalogNo, &stAnalogPara);
//  }
//  return ucRet;
//}
//unsigned char Main_Recv_Set_F83_M(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//  unsigned char ucRet = RET_SUCCESS;
//  unsigned char ucAnalogNo=0;
//  STANALOGPARA    stAnalogPara;
//  ucAnalogNo = ucDAp;
//  if(ucAnalogNo >= CON_ANALOG_NUM)
//      ucRet = RET_ERROR;
//  if(ucRet == RET_SUCCESS)
//  {
////        if(((pucBuffer[0] >=0) && (pucBuffer[0]<=3))||(pucBuffer[0] == 254)||pucBuffer[0] == 255)
//        if((pucBuffer[0]<=3)||(pucBuffer[0] == 254)||pucBuffer[0] == 255)
//          ucRet = RET_SUCCESS;
//  }
//  if(ucRet == RET_SUCCESS)
//  {
//      stAnalogPara = RunPara_GetAnalogPara(ucAnalogNo);
//      stAnalogPara.ucM = pucBuffer[0];
//      RunPara_SetAnalogPara(ucAnalogNo, &stAnalogPara);
//  }
//  return ucRet;
//
//}
//F207
unsigned char Main_Recv_Set_F207_Addr(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   ucJobNo,ucTmp=0;
    unsigned short  m=0;
    STTERDEVADDR    stTerAddr=RunPara_GetTerDevAddr();
    STTERREGIONADDR stTerRegionAddr=RunPara_GetTerRegionAddr();

    stTerRegionAddr.ucRegionAddr[0] =pucBuffer[m++];
    stTerRegionAddr.ucRegionAddr[1] =pucBuffer[m++];
    stTerAddr.ucDevAddr[0] =pucBuffer[m++];
    stTerAddr.ucDevAddr[1] =pucBuffer[m++];
    if((Check_BCD_Data(stTerRegionAddr.ucRegionAddr[0])==RET_SUCCESS)
        && (Check_BCD_Data(stTerRegionAddr.ucRegionAddr[1])==RET_SUCCESS))
    {
        RunPara_SetTerRegionAddr(&stTerRegionAddr);
        RunPara_SetTerDevAddr(&stTerAddr);

    }else
        ucRet=RET_ERROR;
    return ucRet;
}
//azh 170919
//F90 ����ͨ�Ų���
unsigned char Main_Recv_Set_F90_Wireless(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    RunPara_SetTerMisc_Wireless(pucBuffer);
    return ucRet;
}
//F91���ն˵���λ��
unsigned char Main_Recv_Set_F91_Location(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    RunPara_SetTerMisc_TerLocation(pucBuffer);
    return ucRet;
}
//F150��������״̬
unsigned char Main_Recv_Set_F150_Snstate(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    RunPara_SetTerMisc_Snstate(pucBuffer);
    return ucRet;
}

////F131 GR��������
//unsigned char Main_Recv_Set_F131_GRTask(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
    /*  unsigned char ucTaskNo=0;
    unsigned char ucActive=CON_GRTASK_ACTIVE;
    unsigned char ucTmp=0;
    unsigned char m=0;
    ucTaskNo=ucDAp;
    if(ucTaskNo>=CON_GRTASK_NUM)
    return RET_ERROR;
    memset(&stGRTaskTmp,0,sizeof(STGRTASK));
    ucTmp=pucBuffer[m];
    if(ucTmp!=CON_GRTASK_TYPE_NORMAL && ucTmp!=CON_GRTASK_TYPE_RELAY)
    return RET_ERROR;
    stGRTaskTmp.ucTaskType=pucBuffer[m++];
    stGRTaskTmp.ucMPNo=pucBuffer[m++];
    stGRTaskTmp.usSavePointNum=pucBuffer[m]+0x100*pucBuffer[m+1];
    m+=2;
    stGRTaskTmp.usExeNum=pucBuffer[m]+0x100*pucBuffer[m+1];
    m+=2;
    stGRTaskTmp.ucDataItemNum=pucBuffer[m++];               //Ӧ����m=6λ��
    ucActive=pucBuffer[m++];
    if(ucActive!=CON_GRTASK_ACTIVE)
    ucActive=CON_GRTASK_NOACTIVE;
    stGRTaskTmp.stTITaskStart.ucTimeInter=pucBuffer[m++];
    stGRTaskTmp.stTITaskStart.ucTimeUnit=pucBuffer[m++];
    stGRTaskTmp.stTITaskInter.ucTimeInter=pucBuffer[m++];
    stGRTaskTmp.stTITaskInter.ucTimeUnit=pucBuffer[m++];
    stGRTaskTmp.stTIResendStart.ucTimeInter=pucBuffer[m++];
    stGRTaskTmp.stTIResendStart.ucTimeUnit=pucBuffer[m++];
    stGRTaskTmp.stTIReSendInter.ucTimeInter=pucBuffer[m++];
    stGRTaskTmp.stTIReSendInter.ucTimeUnit=pucBuffer[m++];
    stGRTaskTmp.ucResendDataInter=pucBuffer[m++];
    stGRTaskTmp.stRelayTaskPara.ucPortBaudRate=pucBuffer[m++];      //�����ʴ���
    stGRTaskTmp.stRelayTaskPara.ucRecvWaitTime=pucBuffer[m++];      //�м�������ճ�ʱʱ��(��)
    stGRTaskTmp.stRelayTaskPara.ucByteWaitTime=pucBuffer[m++];      //�м������ֽڳ�ʱʱ��(��)
    stGRTaskTmp.stRelayTaskPara.ucCharacterChar=pucBuffer[m++];     //�����ַ�
    stGRTaskTmp.stRelayTaskPara.ucReserve=pucBuffer[m++];           //�����ַ�
    stGRTaskTmp.stRelayTaskPara.usGetFrom=pucBuffer[m]+0x100*pucBuffer[m+1];//��ȡ��ʼ
    m+=2;
    stGRTaskTmp.stRelayTaskPara.usGetLen=pucBuffer[m]+0x100*pucBuffer[m+1];//��ȡ����
    m+=2;
    memcpy(stGRTaskTmp.ucDataArea,pucBuffer+m,2*stGRTaskTmp.ucDataItemNum);
    m+=2*stGRTaskTmp.ucDataItemNum;
    if(pucBuffer[m]!=0xcc && ucTaskNo<CON_GRTASK_USERDEF_START_TASKNO)
    return RET_ERROR;
    memcpy(&stGRDefineArea.stGRTask[ucTaskNo],&stGRTaskTmp,sizeof(STGRTASK));
    Task_Config(ucTaskNo,ucActive);

    */  
//return ucRet;
//}

//��==================
//F1�ն�ͨѶ��������
unsigned char Main_Recv_Get_F1_CommPara(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char *pucBuffer = pstDataItem->ucDataContent;
    unsigned short m=0;

    STTERCOMMPARA   stTerCommPara;
    STHEARTBEAT     stHeartBeat;

    stTerCommPara= RunPara_GetTerCommPara();
    stHeartBeat  = RunPara_GetHeartBeat();

    pucBuffer[m++] =stTerCommPara.ucDelay;
    pucBuffer[m++] =stTerCommPara.ucMaxOvertimeOrig;
    pucBuffer[m++] =(unsigned char)stTerCommPara.usACKOvertime%0x100;
    pucBuffer[m++] =(unsigned char)(stTerCommPara.usACKOvertime/0x100 + stTerCommPara.ucReSendtimes*0x10);

    if( stTerCommPara.ucServerNeedAck[0] == CON_STATUS_YES || stTerCommPara.ucServerNeedAck[1] == CON_STATUS_YES)
        pucBuffer[m] |= 0x04;
    else
        pucBuffer[m] &= 0xFB;
    if( stTerCommPara.ucServerNeedAck[2] == CON_STATUS_YES)
        pucBuffer[m] |= 0x01;
    else
        pucBuffer[m] &= 0xFE;
    if( stTerCommPara.ucServerNeedAck[3] == CON_STATUS_YES)
        pucBuffer[m] |= 0x02;
    else
        pucBuffer[m] &= 0xFD;
    m++;
    pucBuffer[m++]=stHeartBeat.ucHeartBeat;

    pstDataItem->usDataLen =m;
    return ucRet;
}
unsigned char Main_Recv_Get_F2_RelayPara(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet = RET_SUCCESS,ucTmp = 0,i=0;
    unsigned short m = 0;
    unsigned char *pucBuffer = pstDataItem->ucDataContent;
    STLINKREMOVE stLinkRemove = RunPara_GetLinkRemove();

    if(stLinkRemove.ucFlag == CON_AVALIBLE_FLAG)
        ucTmp = 0x80;
    if(stLinkRemove.ucTransNum > CON_TRANS_ADDR_NUM)
        stLinkRemove.ucTransNum = CON_TRANS_ADDR_NUM;
    ucTmp += stLinkRemove.ucTransNum;
    pucBuffer[m++] = ucTmp;

    for(i=0;i<stLinkRemove.ucTransNum;i++)
    {
        pucBuffer[m++] =stLinkRemove.stTransAddr[i].ucDevAddr[0];
        pucBuffer[m++] =stLinkRemove.stTransAddr[i].ucDevAddr[1];
    }
    pstDataItem->usDataLen = m;
    return ucRet;
}
//F3����ͨ��IP��ַ
unsigned char Main_Recv_Get_F3_IP_Addr(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char *pucBuffer = pstDataItem->ucDataContent;
    unsigned short m=0,i=0;

    STSTAIPADDR stStaIPAddr;
    stStaIPAddr=RunPara_GetStaIPAddr();

    for(i=0;i<4;i++)                                        //����ַ
        pucBuffer[m++] = stStaIPAddr.stBaseIP.ucIPAddr[i];
    pucBuffer[m++] = stStaIPAddr.stBaseIP.usIPPort%0x100;
    pucBuffer[m++] = stStaIPAddr.stBaseIP.usIPPort/0x100;

    for(i=0;i<4;i++)                                        //����ַ
        pucBuffer[m++] = stStaIPAddr.stBackIP.ucIPAddr[i];
    pucBuffer[m++] = stStaIPAddr.stBackIP.usIPPort%0x100;
    pucBuffer[m++] = stStaIPAddr.stBackIP.usIPPort/0x100;

//  for(i=0;i<4;i++)                                        //����
//      pucBuffer[m++] = stStaIPAddr.stGateWay.ucIPAddr[i];
//  pucBuffer[m++] = stStaIPAddr.stGateWay.usIPPort%0x100;
//  pucBuffer[m++] = stStaIPAddr.stGateWay.usIPPort/0x100;

//  for(i=0;i<4;i++)                                        //���������
//      pucBuffer[m++] = stStaIPAddr.stAgentServer.ucIPAddr[i];
//  pucBuffer[m++] = stStaIPAddr.stAgentServer.usIPPort%0x100;
//  pucBuffer[m++] = stStaIPAddr.stAgentServer.usIPPort/0x100;

    memcpy(pucBuffer+m,stStaIPAddr.cAPN,16);
    m+=16;

    pstDataItem->usDataLen=m;
    return ucRet;
}

//F4 ��վ����Ͷ������ĺ���
unsigned char Main_Recv_Get_F4_Station_Number(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char *pucBuffer = pstDataItem->ucDataContent;
    unsigned short m=0;
    STSTAPHONENO stStaPhoneNo;
    stStaPhoneNo = RunPara_GetStaPhoneNo();

    memcpy(pucBuffer+m,stStaPhoneNo.stPhoneNo.ucPhoneNo,8);
    m+=8;
    memcpy(pucBuffer+m,stStaPhoneNo.stSMSCenter.ucPhoneNo,8);
    m+=8;


    pstDataItem->usDataLen=m;
    return ucRet;
}

//F5 ��������
unsigned char Main_Recv_Get_F5_TerEncrypt(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS;
    unsigned char   *pucBuffer = pstDataItem->ucDataContent;
    unsigned short  m=0;
    STTERENCRYPT    stTerEncrypt;
    stTerEncrypt = RunPara_GetTerEncrypt();
    pucBuffer[m++] =stTerEncrypt.ucAlgoCode;
    pucBuffer[m++] =stTerEncrypt.ucPassword[0];
    pucBuffer[m++] =stTerEncrypt.ucPassword[1];
    pstDataItem->usDataLen=m;
    return ucRet;
}

//F6�ն����ַ
unsigned char Main_Recv_Get_F6_TerGroupAddr(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char   *pucBuffer = pstDataItem->ucDataContent;
    unsigned short m=0,i=0;
    STTERGROUPADDRLIST  stTerGroupAddrList;
    stTerGroupAddrList = RunPara_GetTerGroupAddrList();

    for(i=0;i<CON_GROUPADDRNUM;i++)
    {
        pucBuffer[m++] =stTerGroupAddrList.stGroupAddr[i].ucGroupAddr[0] ;
        pucBuffer[m++] =stTerGroupAddrList.stGroupAddr[i].ucGroupAddr[1] ;
    }

    pstDataItem->usDataLen=m;
    return ucRet;
}

//F7����IP��ַ�����������
unsigned char Main_Recv_Get_F7_LocalIP(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char  ucRet=RET_SUCCESS;
    unsigned char  *pucBuffer = pstDataItem->ucDataContent;
    unsigned short m=0,i=0;
    STLOCALIP stLocalIP = RunPara_GetLocalIP();

    for(i=0;i<4;i++)
        pucBuffer[m++] = stLocalIP.stLocalIP.ucIPAddr[i];
    for(i=0;i<4;i++)
        pucBuffer[m++] = stLocalIP.stSubMask.ucIPAddr[i];
    for(i=0;i<4;i++)
        pucBuffer[m++] = stLocalIP.stGateWay.ucIPAddr[i];
    pucBuffer[m++] = stLocalIP.stAgentServer.ucServeType;
    for(i=0;i<4;i++)
        pucBuffer[m++] = stLocalIP.stAgentServer.stServeIP.ucIPAddr[i];
    pucBuffer[m++] = (unsigned char)(stLocalIP.stAgentServer.stServeIP.usIPPort);
    pucBuffer[m++] = (unsigned char)(stLocalIP.stAgentServer.stServeIP.usIPPort>>8);
    pucBuffer[m++] = stLocalIP.stAgentServer.ucLinkType;
    pucBuffer[m++] = stLocalIP.stAgentServer.ucUsrNameLen;
    memcpy(pucBuffer+m,stLocalIP.stAgentServer.ucUsrName,stLocalIP.stAgentServer.ucUsrNameLen);
    m += stLocalIP.stAgentServer.ucUsrNameLen;
    pucBuffer[m++] = stLocalIP.stAgentServer.ucPwdLen;
    memcpy(pucBuffer+m,stLocalIP.stAgentServer.ucPwd,stLocalIP.stAgentServer.ucPwdLen);
    m += stLocalIP.stAgentServer.ucPwdLen;
    pucBuffer[m++] = (unsigned char)(stLocalIP.stLocalIP.usIPPort);
    pucBuffer[m++] = (unsigned char)(stLocalIP.stLocalIP.usIPPort>>8);

    pstDataItem->usDataLen = m;
    return ucRet;
}

//F8�¼��澯����
unsigned char Main_Recv_Get_F8_AlarmControlWord(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char  ucRet=RET_SUCCESS;
    unsigned char  *pucBuffer = pstDataItem->ucDataContent;
    unsigned short m=0;
//  unsigned short i=0;
    STALRCATEGORY   *pstAlrShieldACDWord;
    pstAlrShieldACDWord = RunPara_GetAlrShieldACDWord(CON_ALR_SHIELDFLAG);
    memset(pucBuffer+m,0,16);

    SetAlrCategoryComm(pstAlrShieldACDWord->ucTerDataInit     ,&pucBuffer[m],0,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucTerParaLost     ,&pucBuffer[m],1,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucTerParaChange   ,&pucBuffer[m],2,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);

    SetAlrCategoryComm(pstAlrShieldACDWord->ucStatChange      ,&pucBuffer[m],3,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucRemoteCtrl      ,&pucBuffer[m],4,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucPowerCtrl       ,&pucBuffer[m],5,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucEnergyCtrl      ,&pucBuffer[m],6,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);

    SetAlrCategoryComm(pstAlrShieldACDWord->ucMeterParaChange ,&pucBuffer[m],7,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucTAErr           ,&pucBuffer[m+1],0,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucTVErr           ,&pucBuffer[m+1],1,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucPhaseErr        ,&pucBuffer[m+1],2,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);

    SetAlrCategoryComm(pstAlrShieldACDWord->ucMeterTimeErr    ,&pucBuffer[m+1],3,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucMeterErrInfo    ,&pucBuffer[m+1],4,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);

    SetAlrCategoryComm(pstAlrShieldACDWord->ucTerPowOff       ,&pucBuffer[m+1],5,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucHarmWaveOver    ,&pucBuffer[m+1],6,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucDirSimuValOver  ,&pucBuffer[m+1],7,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucUnbalanceOver   ,&pucBuffer[m+2],0,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);

    SetAlrCategoryComm(pstAlrShieldACDWord->ucCapLockSelf     ,&pucBuffer[m+2],1,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucBuyPowPara      ,&pucBuffer[m+2],2,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucPassErr         ,&pucBuffer[m+2],3,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucTerErr          ,&pucBuffer[m+2],4,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);

    SetAlrCategoryComm(pstAlrShieldACDWord->ucEnerAlr         ,&pucBuffer[m+2],6,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucVolOverUp       ,&pucBuffer[m+2],7,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);

    SetAlrCategoryComm(pstAlrShieldACDWord->ucCurOverUp       ,&pucBuffer[m+3],0,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucSPowOverUp      ,&pucBuffer[m+3],1,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucMeterDown       ,&pucBuffer[m+3],2,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucPowDiffOver     ,&pucBuffer[m+3],3,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucMeterFly        ,&pucBuffer[m+3],4,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucMeterStop       ,&pucBuffer[m+3],5,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucMeterCopy       ,&pucBuffer[m+3],6,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucFluxLimit       ,&pucBuffer[m+3],7,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);

//    SetAlrCategoryComm(pstAlrShieldACDWord->ucSumCompOverLimit,&pucBuffer[m+2],5,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    //  for(i=0;i<CON_SUMGROUP_NUM;i++)
    //  {
    //      SetAlrCategoryComm((pstAlrShieldACDWord->ucSumInstantPowOver[i]),&pucBuffer[m+2],5,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    //      SetAlrCategoryComm((pstAlrShieldACDWord->ucSumInstantVarOver[i]),&pucBuffer[m+2],5,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    //      SetAlrCategoryComm((pstAlrShieldACDWord->ucSumPowValOver[i])    ,&pucBuffer[m+2],6,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    //      SetAlrCategoryComm((pstAlrShieldACDWord->ucSumVarValOver[i])    ,&pucBuffer[m+2],6,CON_ALR_SHIELD,CON_ALR_SHIELD_NOT);
    //  }

    m+=4;
    pucBuffer[m++] = 0;
    pucBuffer[m++] = 0;
    pucBuffer[m++] = 0;
    pucBuffer[m++] = 0;

    pstAlrShieldACDWord = RunPara_GetAlrShieldACDWord(CON_ALR_ACDFLAG);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucTerDataInit ,&pucBuffer[m],0,CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucTerParaLost ,&pucBuffer[m],1,CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucTerParaChange   ,&pucBuffer[m],2,CON_ALR_ACD_NOT,CON_ALR_ACD);

    SetAlrCategoryComm(pstAlrShieldACDWord->ucStatChange      ,&pucBuffer[m],3,CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucRemoteCtrl      ,&pucBuffer[m],4,CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucPowerCtrl       ,&pucBuffer[m],5,CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucEnergyCtrl      ,&pucBuffer[m],6,CON_ALR_ACD_NOT,CON_ALR_ACD);

    SetAlrCategoryComm(pstAlrShieldACDWord->ucMeterParaChange ,&pucBuffer[m],7,CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucTAErr           ,&pucBuffer[m+1],0,CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucTVErr           ,&pucBuffer[m+1],1,CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucPhaseErr        ,&pucBuffer[m+1],2,CON_ALR_ACD_NOT,CON_ALR_ACD);

    SetAlrCategoryComm(pstAlrShieldACDWord->ucMeterTimeErr    ,&pucBuffer[m+1],3,CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucMeterErrInfo    ,&pucBuffer[m+1],4,CON_ALR_ACD_NOT,CON_ALR_ACD);

    SetAlrCategoryComm(pstAlrShieldACDWord->ucTerPowOff       ,&pucBuffer[m+1],5,CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucHarmWaveOver    ,&pucBuffer[m+1],6,CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucDirSimuValOver  ,&pucBuffer[m+1],7,CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucUnbalanceOver   ,&pucBuffer[m+2],0,CON_ALR_ACD_NOT,CON_ALR_ACD);

    SetAlrCategoryComm(pstAlrShieldACDWord->ucCapLockSelf     ,&pucBuffer[m+2],1,CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucBuyPowPara      ,&pucBuffer[m+2],2,CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucPassErr         ,&pucBuffer[m+2],3,CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucTerErr          ,&pucBuffer[m+2],4,CON_ALR_ACD_NOT,CON_ALR_ACD);

    SetAlrCategoryComm(pstAlrShieldACDWord->ucEnerAlr         ,&pucBuffer[m+2],6,CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucVolOverUp       ,&pucBuffer[m+2],7,CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucCurOverUp       ,&pucBuffer[m+3],0,CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucSPowOverUp      ,&pucBuffer[m+3],1,CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucMeterDown       ,&pucBuffer[m+3],2,CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucPowDiffOver     ,&pucBuffer[m+3],3,CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucMeterFly        ,&pucBuffer[m+3],4,CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucMeterStop       ,&pucBuffer[m+3],5,CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucMeterCopy       ,&pucBuffer[m+3],6,CON_ALR_ACD_NOT,CON_ALR_ACD);
    SetAlrCategoryComm(pstAlrShieldACDWord->ucFluxLimit       ,&pucBuffer[m+3],7,CON_ALR_ACD_NOT,CON_ALR_ACD);

//    SetAlrCategoryComm(pstAlrShieldACDWord->ucSumCompOverLimit,&pucBuffer[m+2],5,CON_ALR_ACD_NOT,CON_ALR_ACD);
    //  for(i=0;i<CON_SUMGROUP_NUM;i++)
    //  {
    //      SetAlrCategoryComm((pstAlrShieldACDWord->ucSumInstantPowOver[i]),&pucBuffer[m+2],5,CON_ALR_ACD_NOT,CON_ALR_ACD);
    //      SetAlrCategoryComm((pstAlrShieldACDWord->ucSumInstantVarOver[i]),&pucBuffer[m+2],5,CON_ALR_ACD_NOT,CON_ALR_ACD);
    //      SetAlrCategoryComm((pstAlrShieldACDWord->ucSumPowValOver[i])    ,&pucBuffer[m+2],6,CON_ALR_ACD_NOT,CON_ALR_ACD);
    //      SetAlrCategoryComm((pstAlrShieldACDWord->ucSumVarValOver[i])    ,&pucBuffer[m+2],6,CON_ALR_ACD_NOT,CON_ALR_ACD);
    //  }
    m+=4;
    pucBuffer[m++] = 0;
    pucBuffer[m++] = 0;
    pucBuffer[m++] = 0;
    pucBuffer[m++] = 0;

    pstDataItem->usDataLen=m;

    return ucRet;
}

//F9���ն�����������
unsigned char Main_Recv_Get_F9_ConfigNum(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char *pucBuffer = pstDataItem->ucDataContent;
    unsigned short m=0;
    STCONFIGNUM stConfigNum;
    stConfigNum=RunPara_GetConfigNum();
    pucBuffer[m++]=stConfigNum.ucMeterNum+1;        //�̶������ɼ�Ҫ1��
    pucBuffer[m++]=stConfigNum.ucPulseNum;
    pucBuffer[m++]=stConfigNum.ucAnalogNum;
    pucBuffer[m++]=stConfigNum.ucSumGNum;
    pstDataItem->usDataLen=m;
    return ucRet;
}

//F10���ն˵��ܱ�/��������װ�ò���
unsigned char Main_Recv_Get_F10_MeterPara(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char  ucRet=RET_SUCCESS,i=0;
    unsigned char  *pucBuffer = pstDataItem->ucDataContent;
    unsigned short m=0;
    unsigned char  ucMPNo=0,ucRead_Num = 0;
//  unsigned char  ucMeterNo=0,ucMeterNum = 0;
    unsigned char  ucTerMPNo=0,ucTerNo=0;
    unsigned char  ucMPType=CON_MPTYPE_NULL;
    unsigned char  ucMeterNoTable[CON_METER_NUM];
    unsigned char  ucMeterNum_tmp;
    STMETERPARA stMeterPara;
    STTERNORMALPARA stTerNormalPara=RunPara_GetTerNormalPara();

    ucRead_Num = pucBuffer[m++];
    if(ucRead_Num > CON_METER_NUM)  return RET_ERROR;
    if(pucBuffer[m++] != 0)         return RET_ERROR;

    for(i=0;i<ucRead_Num ;i++)
    {
        ucMeterNoTable[i] = pucBuffer[m++];
        if(ucMeterNoTable[i] > CON_METER_NUM)   return RET_ERROR;
        if(pucBuffer[m++] != 0)                 return RET_ERROR;
    }
    m=2;
    ucTerMPNo   = RunPara_GetTer_MPNo();
    ucTerNo = RunPara_GetMPDevNo(ucTerMPNo);
    for(i=0;i<ucRead_Num;i++)
    {
        ucMeterNum_tmp = ucMeterNoTable[i]-1;
        ucMPNo      = RunPara_GetMeter_MPNo(ucMeterNum_tmp);
        ucMPType    = RunPara_GetMPType(ucMPNo);
        if(ucMPType == CON_MPTYPE_METER)
        {
            stMeterPara = RunPara_GetMeterPara(ucMeterNum_tmp);
//          ucMeterNo   = RunPara_GetMPDevNo(ucMPNo);
            pucBuffer[m++] = ucMeterNoTable[i];
            pucBuffer[m++] = 0;
            pucBuffer[m++] = ucMPNo+1;
            pucBuffer[m++] = 0;
//            pucBuffer[m++] = stMeterPara.ucPort+2+(stMeterPara.ucBaudCode << 5);        //��0��ʼ����ת��Ϊ��1��ʼ
            pucBuffer[m++] = stMeterPara.ucPort+1+(stMeterPara.ucBaudCode << 5);        //��0��ʼ����ת��Ϊ��1��ʼ
            pucBuffer[m++] = RunPara_GetExternMeterProtocol(stMeterPara.ucProtocol);
            memcpy(pucBuffer+m,stMeterPara.stMeterAddr.ucMeterAddr,6);
            m+=6;
            memcpy(pucBuffer+m,stMeterPara.stPassword.ucMeterPass,6);
            m+=6;
            pucBuffer[m++] = stMeterPara.ucSetFeeNum;
            pucBuffer[m++] = stMeterPara.ucFloatNum-1 + ((stMeterPara.ucIntNum-4)<<2);
            memcpy(pucBuffer+m,stMeterPara.ucCollectAddr,6);
            m+=6;
            pucBuffer[m++] = (stMeterPara.ucBigClass <<4) + stMeterPara.ucLittleClass;
        }else if(ucTerNo == (ucMeterNoTable[i]-1))
        {
            pucBuffer[m++] = ucMeterNoTable[i];
            pucBuffer[m++] = 0;
            pucBuffer[m++] = ucTerMPNo+1;
            pucBuffer[m++] = 0;
            pucBuffer[m++] = stTerNormalPara.ucSamplePort;
            pucBuffer[m++] = CON_EXT_PROTOCOL_SAMPLE;           //ucProtocol
            memcpy(pucBuffer+m,stTerNormalPara.ucCommAddr,6);
            m+=6;
            memcpy(pucBuffer+m,stTerNormalPara.ucCommPass,6);
            m+=6;
            pucBuffer[m++] = 4;
//          pucBuffer[m++] =stTerNormalPara.ucDataTypeInfo >> 4;
            pucBuffer[m++] =stTerNormalPara.ucDataTypeInfo;     //4���ʣ�����6λС��2λ
            memset(pucBuffer+m,0x00,6);
            m+=6;
            pucBuffer[m++] = 0;
        }
        else
        {
            memset(pucBuffer+m,0xEE,27);
            m+=27;
        }
    }
//  if(ucTerNo>=CON_METER_NUM)
//  {
//      ucMeterNum++;
//      pucBuffer[m++] = ucTerNo+1;
//      pucBuffer[m++] = ucTerMPNo+1;
//      pucBuffer[m++] = stTerNormalPara.ucSamplePort;
//      pucBuffer[m++] = CON_EXT_PROTOCOL_SAMPLE;           //ucProtocol
//      memcpy(pucBuffer+m,stTerNormalPara.ucCommAddr,6);
//      m+=6;
//      memcpy(pucBuffer+m,stTerNormalPara.ucCommPass,6);
//      m+=6;
//      pucBuffer[m++] =stTerNormalPara.ucDataTypeInfo;     //4���ʣ�����6λС��2λ
//  }
    pucBuffer[0] = ucRead_Num;
    pucBuffer[1] = 0;
    if(ucRead_Num==0)
    {
        memset(pucBuffer+2,0xEE,27);
        m=29;
    }
    pstDataItem->usDataLen=m;
    return ucRet;
}

////F11���ն��������ò���
//unsigned char Main_Recv_Get_F11_PulsePara(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char  ucRet=RET_SUCCESS;
//  unsigned char  *pucBuffer = pstDataItem->ucDataContent;
//  unsigned short m=0;
//  unsigned char  ucPulseNum=0,ucMPNo=0,i=0;
//  STPULSEPARA stPulsePara;
//  unsigned char  ucMPType;
//  unsigned char  ucPulse_Num,ucPulse_Table[CON_PULSE_NUM];
//  unsigned char  ucMeterNum_tmp;
//  ucPulse_Num = pucBuffer[m++];
//  if(ucPulse_Num > CON_PULSE_NUM) ucPulse_Num = CON_PULSE_NUM;
//  memcpy(ucPulse_Table,pucBuffer+m,ucPulse_Num);
//  m=1;
//  for(i=0;i<ucPulse_Num;i++)
//  {
//      ucMeterNum_tmp = ucPulse_Table[i]-1;
//      stPulsePara = RunPara_GetPulsePara(ucMeterNum_tmp);
//
//      ucMPNo      = RunPara_GetPulse_MPNo(ucMeterNum_tmp);
//      ucMPType    = RunPara_GetMPType(ucMPNo);
//      if(ucMPType == CON_MPTYPE_PULSE && stPulsePara.ucAttr != CON_PULSEATTR_INVALID)
//      {
//          ucPulseNum++;
//      //  pucBuffer[m++] = i+1;
//          pucBuffer[m++] = ucMeterNum_tmp+1;          //cfh 10-04-27
//          pucBuffer[m++] = ucMPNo+1;
//          if(stPulsePara.ucAttr == CON_PULSEATTR_DIRPOW)
//              pucBuffer[m++] = 0x00;
//          else if(stPulsePara.ucAttr == CON_PULSEATTR_DIRVAR)
//              pucBuffer[m++] = 0x01;
//          else if(stPulsePara.ucAttr == CON_PULSEATTR_REVPOW)
//              pucBuffer[m++] = 0x02;
//          else
//              pucBuffer[m++] = 0x03;
//
//          pucBuffer[m++] = stPulsePara.usConst % 0x100;
//          pucBuffer[m++] = stPulsePara.usConst / 0x100;
//      }
//  }
//  pucBuffer[0] = ucPulseNum;
//
//  pstDataItem->usDataLen=m;
//    return ucRet;
//}
//
////F12���ն�״̬���������
//unsigned char Main_Recv_Get_F12_DInputPara(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned char *pucBuffer = pstDataItem->ucDataContent;
//  unsigned short m=0;
//  unsigned char ucConnect=0,ucAttr=0,i=0;
//    unsigned char ucAlarm=0;
//  STSWITCHPARA stSwitchPara;
//  for(i=0;i<8;i++)
//  {
//      stSwitchPara=RunPara_GetSwitchPara(i);
//      if(stSwitchPara.ucAttr==CON_SWITCHATTR_TYPEA)
//      {
//          ucConnect |= 0x80;
//          ucAttr |= 0x80;
//          if(stSwitchPara.ucAlarm==CON_SWITCHALARM_IMP)
//              ucAlarm |= 0x80;
//
//      }else if(stSwitchPara.ucAttr==CON_SWITCHATTR_TYPEB)
//      {
//          ucConnect |= 0x80;
//          if(stSwitchPara.ucAlarm==CON_SWITCHALARM_IMP)
//              ucAlarm |= 0x80;
//      }
//      if(i<7)
//      {
//          ucConnect=ucConnect>>1;
//          ucAttr=ucAttr>>1;
//          ucAlarm=ucAlarm>>1;
//      }
//  }
//  pucBuffer[m++]=ucConnect;
//  pucBuffer[m++]=ucAttr;
//  pucBuffer[m++]=ucAlarm;
//  pstDataItem->usDataLen=m;
//    return ucRet;
//}
//
////F13���ն˵�ѹ/����ģ�������ò���
//unsigned char Main_Recv_Get_F13_AnalogPara(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned char *pucBuffer = pstDataItem->ucDataContent;
//  unsigned short m=0;
//  unsigned char ucAnalogNum=0,i,ucAnalog_Num,ucAnalog_Table[CON_ANALOG_NUM];
//  STANALOGPARA stAnalogPara;
//  unsigned char ucAnalogNo;
//
//  ucAnalog_Num = pucBuffer[m++];
//  if(ucAnalog_Num >CON_ANALOG_NUM)
//      ucAnalog_Num = CON_ANALOG_NUM;
//  memcpy(ucAnalog_Table,pucBuffer+m,ucAnalog_Num);
//  m = 1;
//  for(i=0;i<ucAnalog_Num;i++)
//  {
//      ucAnalogNo = ucAnalog_Table[i] - 1;
//      stAnalogPara=RunPara_GetAnalogPara(ucAnalogNo);
//      if(stAnalogPara.ucMPNo!=CON_MPNO_INVALID)
//      {
//          pucBuffer[m++]=ucAnalogNo+1;                            //��1��ʼ
//          pucBuffer[m++]=stAnalogPara.ucMPNo;
//          pucBuffer[m++]=stAnalogPara.ucAttr%6;
//          ucAnalogNum++;
//      }
//  }
//  pucBuffer[0] = ucAnalogNum;
//  pstDataItem->usDataLen=m;
//    return ucRet;
//}
//
////F14���ն��ܼ������ò���
//unsigned char Main_Recv_Get_F14_SumGPara(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent;
//  unsigned char   *pucMPNum=NULL,ucMPNum;
//  unsigned short  m=0;
//  unsigned char   ucSumGNum=0,ucPos=0,i=0,j=0;
//  STSUMGROUPPARA  stSumGParaTmp;
//  unsigned char   ucSumGP_Num,ucSumGP_Table[CON_SUMGROUP_NUM];
//  unsigned char   ucMeterNum_tmp;
//  ucSumGP_Num = pucBuffer[0];
//  if(ucSumGP_Num > CON_SUMGROUP_NUM)  return RET_ERROR;
//  memcpy(ucSumGP_Table,pucBuffer+1,ucSumGP_Num);
//  m=1;
//  for(i=0;i<ucSumGP_Num;i++)
//  {
//      ucMeterNum_tmp = ucSumGP_Table[i]-1;
//      stSumGParaTmp=RunPara_GetSumGroupPara(ucMeterNum_tmp);
//      if(stSumGParaTmp.ucAvailbleFlag==CON_AVALIBLE_FLAG)
//      {
//          pucBuffer[m++]=ucSumGP_Table[i];
//          pucMPNum=pucBuffer+m;   m++;
//          ucMPNum=0;
//          for(j=0;j<CON_MAX_MPNUM;j++)
//          {
//              ucPos=Is_In_Buffer(stSumGParaTmp.stInstantPow.stDir.ucMPSum,CON_MAX_MPNUM,j);
//              if(ucPos != 0xFF && ucPos < CON_MAX_MPNUM)
//              {
//                  if(stSumGParaTmp.stInstantPow.stDir.ucMPOper[ucPos]==CON_SUMGOPER_ADD)
//                      pucBuffer[m++]=j;
//                  else
//                      pucBuffer[m++]=j+0x80;
//                  ucMPNum++;
//              }else
//              {
//                  ucPos=Is_In_Buffer(stSumGParaTmp.stInstantPow.stRev.ucMPSum,CON_MAX_MPNUM,j);
//                  if(ucPos != 0xFF && ucPos < CON_MAX_MPNUM)
//                  {
//                      if(stSumGParaTmp.stInstantPow.stRev.ucMPOper[ucPos]==CON_SUMGOPER_ADD)
//                          pucBuffer[m++]=j+0x40;
//                      else
//                          pucBuffer[m++]=j+0xC0;
//                      ucMPNum++;
//                  }
//              }
//          }
//          (*pucMPNum)=ucMPNum;
//          ucSumGNum++;
//      }
//  }
//
//
//  pucBuffer[0] = ucSumGP_Num;
//
//  pstDataItem->usDataLen=m;
//    return ucRet;
//}
//
////F15���й��ܵ������Խ���¼���������
//unsigned char Main_Recv_Get_F15_PowValDiff(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   m=1,i=0;
//  unsigned long   ulVal=0;
//  STTERMISCPARA   stTerMiscPara;
//  stTerMiscPara=RunPara_GetTerMiscPara();
//  for(i=0;i<CON_MAX_DIRPOWTOTAL_COMPNUM;i++)
//  {
//      if(stTerMiscPara.stPowDiffLimit[i].ucSumGNo<CON_SUMGROUP_NUM)
//      {
//          pstDataItem->ucDataContent[m++]=stTerMiscPara.stPowDiffLimit[i].ucSumGNo+1;
//          pstDataItem->ucDataContent[m++]=stTerMiscPara.stPowDiffLimit[i].ucSumGNoComp+1;
//          pstDataItem->ucDataContent[m++]=stTerMiscPara.stPowDiffLimit[i].ucSumGNoReff+1;
//          pstDataItem->ucDataContent[m++]=stTerMiscPara.stPowDiffLimit[i].ucFlag;
//          pstDataItem->ucDataContent[m++]=stTerMiscPara.stPowDiffLimit[i].ucDiffLimit;
//          ulVal=Float_To_DT3(stTerMiscPara.stPowDiffLimit[i].dDiffLimitVal,stTerMiscPara.stPowDiffLimit[i].ucSign);
//          memcpy(pstDataItem->ucDataContent+m,(unsigned char *)&(ulVal),4); m+=4;
//      }else
//          break;
//  }
//  pstDataItem->ucDataContent[0]=i;
//  pstDataItem->usDataLen=m;
//    return ucRet;
//}

//F16������ר���û���������
unsigned char Main_Recv_Get_F16_PDPInfo(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   m=0;
    STTERMISCPARA   stTerMiscPara=RunPara_GetTerMiscPara();
    memcpy(pstDataItem->ucDataContent,stTerMiscPara.stRCommPara.stGprsPDPPara.ucPDPUserName,CON_MAX_GPRSPDPUSERNAME_LEN);
    memcpy(pstDataItem->ucDataContent+CON_MAX_GPRSPDPUSERNAME_LEN,stTerMiscPara.stRCommPara.stGprsPDPPara.ucPDPPassWord,CON_MAX_GPRAPDPPASSWORE_LEN);
    pstDataItem->usDataLen=CON_MAX_GPRSPDPUSERNAME_LEN+CON_MAX_GPRAPDPPASSWORE_LEN;
    return ucRet;
}

//F17���ն˱�����ֵ
//unsigned char Main_Recv_Get_F17_KeepPower(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char   ucRet=RET_SUCCESS;
//    unsigned char   m=0;
//    float fVal;
//    unsigned short usDT2;
//    STLOADFIXVAL    stSafeFixVal;
//    stSafeFixVal=RunPara_GetCtrlSafeFixVal();
//    fVal=(float)stSafeFixVal.dFixVal;
//    usDT2=Float_To_DT2(fVal);
//    usDT2=stSafeFixVal.usDT2;
//    pstDataItem->ucDataContent[m++]=(unsigned char)usDT2;
//    pstDataItem->ucDataContent[m++]=(unsigned char)(usDT2>>8);
//    pstDataItem->usDataLen=m;
//    return ucRet;
//}

////F18���ն˹���ʱ��
//unsigned char Main_Recv_Get_F18_PowCtrlTimePer(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
////  unsigned char ucSumGNo=Load_GetPowerCtrl_SumGNo();
////  STPOWERCTRLTIME stPowerCtrlTime;
////  if(ucSumGNo>=CON_SUMGROUP_NUM)
////  {
////      ucSumGNo=0;
////      Load_SetPowerCtrl_SumGNo(ucSumGNo);
////  }
////
////  stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER1,ucSumGNo);
////  memcpy(pstDataItem->ucDataContent,stPowerCtrlTime.ucTimerPerFlag,12);
////  pstDataItem->usDataLen=12;
//    return ucRet;
//}
//
////F19���ն�ʱ�ι��ض�ֵ����ϵ��
//unsigned char Main_Recv_Get_F19_PowCtrlCoef(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
////  unsigned char m=0;
////  unsigned char ucTmp;
////  STPOWERCTRLTIME stPowerCtrlTime;
////  unsigned short usCoef;
////  unsigned char ucSumGNo=Load_GetPowerCtrl_SumGNo();
////  if(ucSumGNo>=CON_SUMGROUP_NUM)
////  {
////      ucSumGNo=0;
////      Load_SetPowerCtrl_SumGNo(ucSumGNo);
////  }
////
////  stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER1,ucSumGNo);
////  usCoef=stPowerCtrlTime.stPowerCtrlPer[0].stLoadFixVal.usFloatCoef;
////  if(usCoef<100)
////  {
////      ucTmp=(unsigned char)(100-usCoef);
////      ucTmp=(unsigned char)HEX_TO_BCD(ucTmp,1);
////      pstDataItem->ucDataContent[m++]=ucTmp|0x80;
////  }else
////  {
////      ucTmp=(unsigned char)(usCoef-100);
////      ucTmp=(unsigned char)HEX_TO_BCD(ucTmp,1);
////      pstDataItem->ucDataContent[m++]=ucTmp;
////  }
////  pstDataItem->usDataLen=m;
//    return ucRet;
//}
//
////F20���ն��µ������ض�ֵ����ϵ��
//unsigned char Main_Recv_Get_F20_EnerCtrlCoef(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
////  unsigned char m=0;
////  unsigned char ucTmp;
////  STENERGYCTRL stEnergyCtrl;
////  unsigned short usCoef;
////  unsigned char ucSumGNo=0;
////  ucSumGNo=Load_GetEnergyCtrl_SumGNo();
////  if(ucSumGNo>=CON_SUMGROUP_NUM)
////  {
////      ucSumGNo=0;
////      Load_SetPowerCtrl_SumGNo(ucSumGNo);
////  }
////  stEnergyCtrl=RunPara_GetEnergyCtrl(ucSumGNo);
////  usCoef=stEnergyCtrl.stMonthEnergy.usFloatCoef;
////  if(usCoef<100)
////  {
////      ucTmp=(unsigned char)(100-usCoef);
////      ucTmp=(unsigned char)HEX_TO_BCD(ucTmp,1);
////      pstDataItem->ucDataContent[m++]=ucTmp|0x80;
////  }else
////  {
////      ucTmp=(unsigned char)(usCoef-100);
////      ucTmp=(unsigned char)HEX_TO_BCD(ucTmp,1);
////      pstDataItem->ucDataContent[m++]=ucTmp;
////  }
////  pstDataItem->usDataLen=m;
//    return ucRet;
//}
//
////F21���ն˵���������ʱ�κͷ�����
//unsigned char Main_Recv_Get_F21_PerTimeFee(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char   ucRet=RET_SUCCESS;
////  STCTRLFEERATETIME   stCtrlFeeRateTime;
////  unsigned short  m=0;
////  unsigned char ucNum,ucNo,ucTmp,i=0;
////  ucNum=CON_CTRLFEETIME_NUM;
////  ucNo=0;
////  stCtrlFeeRateTime=RunPara_GetCtrlFeeRateTime();
////  for(i=0;i<CON_CTRLFEETIME_NUM;i++)
////  {
////      ucTmp=(stCtrlFeeRateTime.ucFeeRateNo[ucNo++]&0x0F);
////      ucTmp+=(stCtrlFeeRateTime.ucFeeRateNo[ucNo++]<<4);
////      pstDataItem->ucDataContent[m++]=stCtrlFeeRateTime.ucFeeRateNo[ucNo++];      //100323  ÿ���һ���ֽ�
////  }
////  pstDataItem->ucDataContent[m++]=stCtrlFeeRateTime.ucFeeNum;
////  pstDataItem->usDataLen=m;
//    return ucRet;
//}
//
////F22���ն˵���������
//unsigned char Main_Recv_Get_F22_FeePara(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char   ucRet=RET_SUCCESS;
////  unsigned char   i=0;
////  unsigned short  m=0;
////  STCTRLFEERATE   stCtrlFeeRate;
////  stCtrlFeeRate=RunPara_GetCtrlFeeRate();
////  pstDataItem->ucDataContent[m++] = stCtrlFeeRate.ucFeeNum;
////  for(i=0;i<stCtrlFeeRate.ucFeeNum;i++)
////  {
////      if(stCtrlFeeRate.ucUnitFlag > 0 )
////      {
////          stCtrlFeeRate.ulFeeRate[i] /= 1000;
////          m=SetMainComm_Buf_lVal_4B(pstDataItem->ucDataContent,m,stCtrlFeeRate.ulFeeRate[i]);
////          pstDataItem->ucDataContent[m-1] |=0x40;
////
////      }else
////          m=SetMainComm_Buf_lVal_4B(pstDataItem->ucDataContent,m,stCtrlFeeRate.ulFeeRate[i]);
////  }
////  pstDataItem->usDataLen=m;
//    return ucRet;
//}
//
////F23���ն˴߷Ѹ澯����
//unsigned char Main_Recv_Get_F23_UrgeAlarmPara(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char   ucRet=RET_SUCCESS;
////  unsigned char   m=0;
////  STTERMISCPARA   stTerMiscPara;
////  stTerMiscPara=RunPara_GetTerMiscPara();
////  pstDataItem->ucDataContent[m++]=stTerMiscPara.ucUrgeFeeTime[0];
////  pstDataItem->ucDataContent[m++]=stTerMiscPara.ucUrgeFeeTime[1];
////  pstDataItem->ucDataContent[m++]=stTerMiscPara.ucUrgeFeeTime[2];
////  pstDataItem->usDataLen=m;
//    return ucRet;
//}

//F24���ն˳���������
unsigned char Main_Recv_Get_F24_CopyInter(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS;
    unsigned char   m=0;
    STTERNORMALPARA stTerNormalPara;
    stTerNormalPara=RunPara_GetTerNormalPara();
    pstDataItem->ucDataContent[m++]=stTerNormalPara.ucCopyInter;
    pstDataItem->usDataLen=m;
    return ucRet;
}

//F25�������������
unsigned char Main_Recv_Get_F25_MPBasePara(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS;
    unsigned char   *pucBuffer = pstDataItem->ucDataContent;
    unsigned short  m=0;
    unsigned short  usTmp=0;
    unsigned char   ucMPNo=0;
    unsigned long   ulTmp;
    STMPALTERRATE   stMPAlterRate;
    STMPRATEVAL     stMPRateVal;

    ucMPNo= ucDAp;
    if(ucMPNo >= CON_MAX_MPNUM)
        ucRet =RET_ERROR;
    else
    {
        stMPAlterRate = RunPara_GetMPAlterRate(ucMPNo);
        pucBuffer[m++] = stMPAlterRate.usVolAlterRate%0x100;
        pucBuffer[m++] = stMPAlterRate.usVolAlterRate/0x100;
        pucBuffer[m++] = stMPAlterRate.usCurAlterRate%0x100;
        pucBuffer[m++] = stMPAlterRate.usCurAlterRate/0x100;
        stMPRateVal=RunPara_GetMPRateVal(ucMPNo);
        usTmp=stMPRateVal.usRateVol;        //0.1  V
        usTmp = (unsigned short)HEX_TO_BCD(usTmp,2);
        pucBuffer[m++] = usTmp%0x100;
        pucBuffer[m++] = usTmp/0x100;
        usTmp=stMPRateVal.usRateCur/10;
        usTmp = (unsigned short)HEX_TO_BCD(usTmp,2);
        pucBuffer[m++] = usTmp%0x100;
        ulTmp = HEX_TO_BCD(stMPRateVal.ulRateP,3);
        pucBuffer[m++] = (unsigned char)(ulTmp%0x100);
        pucBuffer[m++] = (unsigned char)((ulTmp/0x100)%0x100);
        pucBuffer[m++] = (unsigned char)((ulTmp/0x100)/0x100);

        if(stMPRateVal.ucMPLineType==CON_METERTYPE_3P3L)
            pucBuffer[m++] =0x01;
        else if(stMPRateVal.ucMPLineType ==CON_METERTYPE_3P4L)
            pucBuffer[m++] =0x02;
        else if(stMPRateVal.ucMPLineType ==CON_METERTYPE_1P)
        {
            pucBuffer[m] =0x03;
            pucBuffer[m] = pucBuffer[m] + (stMPRateVal.ucMPPhaseNo << 2);
        }
        else
            pucBuffer[m++] =0x00;
    }
    pstDataItem->usDataLen=m;
    return ucRet;
}

//F26��������ֵ
unsigned char Main_Recv_Get_F26_MPLimit(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char *pucBuffer = pstDataItem->ucDataContent;
    unsigned short m=0;
    unsigned char  ucMPNo=0;
    short usTmp=0;
//  unsigned long  ulTmp=0;
    STMPLIMIT      stMPLimit;

    ucMPNo= ucDAp;
    if(ucMPNo >= CON_MAX_MPNUM)
        ucRet =RET_ERROR;
    else
    {
        stMPLimit = RunPara_GetMPLimit(ucMPNo);

        m = SetMainComm_Buf_lVal_2B(pucBuffer,m, stMPLimit.usVolUpLimit_2);

        m = SetMainComm_Buf_lVal_2B(pucBuffer,m, stMPLimit.usVolDownLimit_2);

        m = SetMainComm_Buf_lVal_2B(pucBuffer,m, stMPLimit.usVolBreakLimit_2);

        m = SetMainComm_Buf_lVal_2B(pucBuffer,m, stMPLimit.usVolErrUpLimit_2);
        pucBuffer[m++] = stMPLimit.ucVolErrUpTime/3;
        usTmp =  stMPLimit.ucVolErrUpBack - 1000 ;
        m = SetMainComm_Buf_lVal_2B_DT05(pucBuffer, m, usTmp);

        m = SetMainComm_Buf_lVal_2B(pucBuffer,m, stMPLimit.usVolErrDownLimit_2);
        pucBuffer[m++] = stMPLimit.ucVolErrDownTime/3;
        usTmp = stMPLimit.ucVolErrDownBack - 1000;
        m = SetMainComm_Buf_lVal_2B_DT05(pucBuffer,m,usTmp);

        stMPLimit.usCurErrUpLimit *= 10;
        m = SetMainComm_Buf_lVal_3B(pucBuffer, m, stMPLimit.usCurErrUpLimit);
        pucBuffer[m++] = stMPLimit.ucCurErrUpTime/3;
        usTmp = stMPLimit.ucCurErrUpBack - 1000;
        m = SetMainComm_Buf_lVal_2B_DT05(pucBuffer,m,usTmp);

        stMPLimit.usCurUpLimit *= 10;
        m = SetMainComm_Buf_lVal_3B(pucBuffer, m, stMPLimit.usCurUpLimit);
        pucBuffer[m++] = stMPLimit.ucCurUpTime/3;
        usTmp = stMPLimit.ucCurUpBack - 1000;
        m = SetMainComm_Buf_lVal_2B_DT05(pucBuffer,m,usTmp);

        stMPLimit.usCur0UpLimit *= 10;
        m = SetMainComm_Buf_lVal_3B(pucBuffer, m, stMPLimit.usCur0UpLimit);
        pucBuffer[m++] = stMPLimit.ucCur0UpTime/3;
        usTmp = stMPLimit.ucCur0UpBack - 1000;
        m = SetMainComm_Buf_lVal_2B_DT05(pucBuffer,m,usTmp);

        m = SetMainComm_Buf_lVal_3B(pucBuffer, m, stMPLimit.ulPowErrUpLimit);
        pucBuffer[m++] = stMPLimit.ucPowErrUpTime/3;
        usTmp = stMPLimit.ucPowErrUpBack - 1000;
        m = SetMainComm_Buf_lVal_2B_DT05(pucBuffer,m,usTmp);

        m = SetMainComm_Buf_lVal_3B(pucBuffer, m, stMPLimit.ulPowUpLimit);
        pucBuffer[m++] = stMPLimit.ucPowUpTime/3;
        usTmp = stMPLimit.ucPowUpBack - 1000;
        m = SetMainComm_Buf_lVal_2B_DT05(pucBuffer,m,usTmp);

        m = SetMainComm_Buf_lVal_2B(pucBuffer, m, stMPLimit.usVolUnBalance);
        pucBuffer[m++] = stMPLimit.ucVolUnBalanceTime/3;
        usTmp = stMPLimit.ucVolUnBalanceBack - 1000;
        m = SetMainComm_Buf_lVal_2B_DT05(pucBuffer,m,usTmp);

        m = SetMainComm_Buf_lVal_2B(pucBuffer, m, stMPLimit.usCurUnBalance);
        pucBuffer[m++] = stMPLimit.ucCurUnBalanceTime/3;
        usTmp = stMPLimit.ucCurUnBalanceBack - 1000;
        m = SetMainComm_Buf_lVal_2B_DT05(pucBuffer,m,usTmp);

        pucBuffer[m++] = (unsigned char)stMPLimit.usLostVolTime;

        pstDataItem->usDataLen=m;
    }

    return ucRet;
}

//F27�����㶳�����
unsigned char Main_Recv_Get_F27_MPFreezePara(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   i=0;
    unsigned char   *pucBuffer = pstDataItem->ucDataContent;
    unsigned short  m=0;
    unsigned char   ucMPNo=0;
    unsigned short usValTmp = 0;
    STTONGSUNTIANSUNPARA stTongsuntmp;
    ucMPNo= ucDAp;
    if(ucMPNo >= CON_MAX_MPNUM)
        ucRet = RET_ERROR;
    else
    {
        stTongsuntmp = RunPara_GetMPTongsun(ucMPNo);
        usValTmp = (unsigned short)HEX_TO_BCD(stTongsuntmp.stCuFeLostParaA.R, 2);
        memcpy(pucBuffer+m,&usValTmp,2);
        m += 2;
        usValTmp = (unsigned short)HEX_TO_BCD(stTongsuntmp.stCuFeLostParaA.X, 2);
        memcpy(pucBuffer+m,&usValTmp,2);
        m += 2;
        usValTmp = (unsigned short)HEX_TO_BCD(stTongsuntmp.stCuFeLostParaA.G, 2);
        memcpy(pucBuffer+m,&usValTmp,2);
        m += 2;
        usValTmp = (unsigned short)HEX_TO_BCD(stTongsuntmp.stCuFeLostParaA.B, 2);
        memcpy(pucBuffer+m,&usValTmp,2);
        m += 2;
        usValTmp = (unsigned short)HEX_TO_BCD(stTongsuntmp.stCuFeLostParaB.R, 2);
        memcpy(pucBuffer+m,&usValTmp,2);
        m += 2;
        usValTmp = (unsigned short)HEX_TO_BCD(stTongsuntmp.stCuFeLostParaB.X, 2);
        memcpy(pucBuffer+m,&usValTmp,2);
        m += 2;
        usValTmp = (unsigned short)HEX_TO_BCD(stTongsuntmp.stCuFeLostParaB.G, 2);
        memcpy(pucBuffer+m,&usValTmp,2);
        m += 2;
        usValTmp = (unsigned short)HEX_TO_BCD(stTongsuntmp.stCuFeLostParaB.B, 2);
        memcpy(pucBuffer+m,&usValTmp,2);
        m += 2;
        usValTmp = (unsigned short)HEX_TO_BCD(stTongsuntmp.stCuFeLostParaC.R, 2);
        memcpy(pucBuffer+m,&usValTmp,2);
        m += 2;
        usValTmp = (unsigned short)HEX_TO_BCD(stTongsuntmp.stCuFeLostParaC.X, 2);
        memcpy(pucBuffer+m,&usValTmp,2);
        m += 2;
        usValTmp = (unsigned short)HEX_TO_BCD(stTongsuntmp.stCuFeLostParaC.G, 2);
        memcpy(pucBuffer+m,&usValTmp,2);
        m += 2;
        usValTmp = (unsigned short)HEX_TO_BCD(stTongsuntmp.stCuFeLostParaC.B, 2);
        memcpy(pucBuffer+m,&usValTmp,2);
        m += 2;
//      pucBuffer[m++] = stTongsuntmp.
//      memcpy(pucBuffer,&stTongsuntmp,24);
        pstDataItem->usDataLen=24;
    }
    return ucRet;
}

//F28���������ֶ���ֵ����
unsigned char Main_Recv_Get_F28_MPPowRatePara(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS;
    unsigned char   *pucBuffer = pstDataItem->ucDataContent;
    unsigned short  m=0;
    unsigned char   ucMPNo=0;
    unsigned short  usTmp;
    STPOWRATEPARA stPowRatePara;

    ucMPNo= ucDAp;
    if(ucMPNo >= CON_MAX_MPNUM)
        ucRet =RET_ERROR;
    else
    {
        stPowRatePara = RunPara_GetMPPowRatePara(ucMPNo);
        usTmp = (unsigned short)HEX_TO_BCD(stPowRatePara.usPowRateLimit1,2);
        pucBuffer[m++] = (unsigned char)(usTmp%0x100);
        pucBuffer[m++] = (unsigned char)(usTmp/0x100);
        usTmp = (unsigned short)HEX_TO_BCD(stPowRatePara.usPowRateLimit2,2);
        pucBuffer[m++] = (unsigned char)(usTmp%0x100);
        pucBuffer[m++] = (unsigned char)(usTmp/0x100);
        pstDataItem->usDataLen=m;
    }
    return ucRet;
}


unsigned char Main_Recv_Get_F29_Ter_Meter_addr(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS;
    unsigned char   *pucBuffer = pstDataItem->ucDataContent;
    unsigned char ucMPNo = 0,ucMPType = CON_MPTYPE_NULL;
    STMETERPARA stMeterPara;

    ucMPNo = ucDAp;
    ucMPType = RunPara_GetMPType(ucMPNo);
    if(ucMPType == CON_MPTYPE_METER)
    {
        stMeterPara = RunPara_GetMeterPara_byMPNo(ucMPNo);
        memcpy(pucBuffer,stMeterPara.ucMeterJuNum,12);
        pstDataItem->usDataLen=12;
    }else
        ucRet = RET_ERROR;

    return ucRet;
}
unsigned char Main_Recv_Get_F30_CopyFlag(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet = RET_SUCCESS;
    unsigned char ucFlag = 0;
    unsigned char *pucBuffer = pstDataItem->ucDataContent;

    ucFlag = RunPara_GetMeterCopyCopyFlag();
    if(ucFlag == CON_STATUS_NO)
        ucFlag = 1;
    else
        ucFlag = 0;
    pucBuffer[0] = ucFlag;
    pstDataItem->usDataLen = 1;
    return ucRet ;
}
//F33�ܼ��鶳�����
unsigned char Main_Recv_Get_F33_SumGFreezePara(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
  unsigned char   ucRet=RET_SUCCESS,i=0,n=0,j=0;

  unsigned char  *pucBuffer = pstDataItem->ucDataContent;
  unsigned short m=0;
  unsigned char  ucPortTable[CON_PORT_NUM];
  unsigned char  ucPortNum_tmp;//azh
  PSTRMRUNPARA    pstRMRunPara;
  PSTCOMPORTPARA pstComPortPara;

  pstRMRunPara=RunPara_GetRMRunPara();
  ucPortNum_tmp = pucBuffer[m++];
  if(ucPortNum_tmp>CON_PORT_NUM) ucPortNum_tmp = CON_PORT_NUM;//���֧��8���˿�
  for(i=0; i<ucPortNum_tmp; i++)
  {
      if( (pucBuffer[m]>0) && (pucBuffer[m]<=CON_PORT_NUM) )
          ucPortTable[i] = pucBuffer[m++];
      else return RET_ERROR;
  }
  m = 0;
  pucBuffer[m++] = ucPortNum_tmp;
  for(i=0; i<ucPortNum_tmp; i++)
  {
      pucBuffer[m++] = ucPortTable[i];//�ڲ��˿ڴ�0��ʼ
      if(ChkBitFlag(&pstRMRunPara->ucComPortEBit, (unsigned short)(ucPortTable[i]-1)))
      {
          pstComPortPara = &pstRMRunPara->stComPortPara[ucPortTable[i]-1];
          pucBuffer[m++] = pstComPortPara->usRMRunCtrlWord&0xff;
          pucBuffer[m++] = (pstComPortPara->usRMRunCtrlWord>>8)&0xff;
          pucBuffer[m++] = pstComPortPara->ulRMDate&0xff;
          pucBuffer[m++] = (pstComPortPara->ulRMDate>>8)&0xff;
          pucBuffer[m++] = (pstComPortPara->ulRMDate>>16)&0xff;
          pucBuffer[m++] = (pstComPortPara->ulRMDate>>24)&0xff;
          pucBuffer[m++] = (unsigned char)HEX_TO_BCD(pstComPortPara->ucRMTime[1], 1) ;
          pucBuffer[m++] = (unsigned char)HEX_TO_BCD(pstComPortPara->ucRMTime[0], 1);
          pucBuffer[m++] = pstComPortPara->ucRMInval;
          pucBuffer[m++] = (unsigned char)HEX_TO_BCD(pstComPortPara->ucRMBcastTInval[2], 1);
          pucBuffer[m++] = (unsigned char)HEX_TO_BCD(pstComPortPara->ucRMBcastTInval[1],1);
          pucBuffer[m++] = (unsigned char)HEX_TO_BCD(pstComPortPara->ucRMBcastTInval[0],1);
          if( pstComPortPara->ucRMWorkZoneNum>0 )
          {
              if(pstComPortPara->ucRMWorkZoneNum>24) pstComPortPara->ucRMWorkZoneNum = 24;
              pucBuffer[m++] = pstComPortPara->ucRMWorkZoneNum;
              n = pstComPortPara->ucRMWorkZoneNum;
              for(j=0; j<n; j++)
              {
                  pucBuffer[m++] = (unsigned char)HEX_TO_BCD(pstComPortPara->stRMWorkZone[j].usRMTStart[1],1);
                  pucBuffer[m++] = (unsigned char)HEX_TO_BCD(pstComPortPara->stRMWorkZone[j].usRMTStart[0],1);
                  pucBuffer[m++] = (unsigned char)HEX_TO_BCD(pstComPortPara->stRMWorkZone[j].usRMTEnd[1],1);
                  pucBuffer[m++] = (unsigned char)HEX_TO_BCD(pstComPortPara->stRMWorkZone[j].usRMTEnd[0],1);
              }
          }
          else pucBuffer[m++] = pstComPortPara->ucRMWorkZoneNum;//0
      }
      else//��������EE,����ʱ��Ϊ0��
      {
          memset(pucBuffer+m, 0xee, 12);
          m += 12;
          pucBuffer[m++] = 0;//����ʱ��
      }
  }
  pstDataItem->usDataLen=m;

  return ucRet;
}
unsigned char Main_Recv_Get_F34_PortProp(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS,i;
    unsigned char   *pucBuffer = pstDataItem->ucDataContent;
    unsigned short  m=0;
    unsigned char ucPortNum,ucPort,ucPortTable[CON_COMMPORT_NUM];
    STPORTLIST stPortList = RunPara_GetPortPropList();

    ucPortNum = pucBuffer[m++];
    if(ucPortNum >CON_COMMPORT_NUM)
        ucPortNum = CON_COMMPORT_NUM;
    for(i=0;i<ucPortNum;i++)
    {
        ucPort = pucBuffer[m++];
        if(ucPort >0 && ucPort <= CON_COMMPORT_NUM)
            ucPortTable[i] = ucPort;
        else
        {
            ucRet = RET_ERROR;
            break;
        }
    }
    if(ucRet == RET_SUCCESS)
    {
        m = 0;
        pucBuffer[m++] = ucPortNum;
        for(i=0;i<ucPortNum;i++)
        {
            pucBuffer[m++] = ucPortTable[i];
            ucPort = ucPortTable[i]-1;
            pucBuffer[m++] = Main_GetPortCtrlByte(&(stPortList.stPortProp[ucPort]));
            memcpy(pucBuffer+m,&(stPortList.stPortProp[ucPort].ulBaudRate),4);
            m += 4;
        }
    }
    pstDataItem->usDataLen = m;
    return ucRet;
}
//unsigned char Main_Recv_Get_F35_EmphaFlag(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char ucRet = RET_SUCCESS;
//  unsigned char ucMeter_Num = 0,i;
//  unsigned char *pucBuffer = pstDataItem->ucDataContent;
//  unsigned char ucFlag;
//  unsigned short m = 1;
//
//  for(i=0;i<CON_METER_NUM;i++)
//  {
//      ucFlag = RunPara_GetMeterPara_EmphaFlag(i);
//      if(ucFlag == CON_STATUS_YES)
//      {
//          pucBuffer[m++] = i+1;
//          pucBuffer[m++]  = 0;
//          ucMeter_Num ++;
//      }
//  }
//  pucBuffer[0] = ucMeter_Num;
//  pstDataItem->usDataLen = m;
//
//    return ucRet;
//}
unsigned char Main_Recv_Get_F37_LinkPara(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS,i;
    unsigned char   *pucBuffer = pstDataItem->ucDataContent;
    unsigned short  m=0;
    STLINKPARA stLinkPara = RunPara_GetLinkPara();
    unsigned char ucData = 0;

    pucBuffer[m++] = stLinkPara.ucPortNo;
    pucBuffer[m++] = Main_GetPortCtrlByte(&(stLinkPara.stLinkPortPara));
    pucBuffer[m++] = stLinkPara.ucRecvOverTime;
    pucBuffer[m++] = stLinkPara.ucByteOverTime;
    pucBuffer[m++] = stLinkPara.ucResendTime;
    pucBuffer[m++] = stLinkPara.ucInterTime;
    if(stLinkPara.ucSlaveFlag != CON_LINK_INVALID)
        ucData += stLinkPara.ucSlaveFlag <<7;
    ucData += stLinkPara.ucSlaveNum;
    pucBuffer[m++] = ucData;
    for(i=0;i<stLinkPara.ucSlaveNum;i++)
    {
        pucBuffer[m++] = stLinkPara.stSlaveAddr[i].stTerRegionAddr.ucRegionAddr[1];
        pucBuffer[m++] = stLinkPara.stSlaveAddr[i].stTerRegionAddr.ucRegionAddr[0];
        pucBuffer[m++] = stLinkPara.stSlaveAddr[i].stTerAddr.ucDevAddr[1];
        pucBuffer[m++] = stLinkPara.stSlaveAddr[i].stTerAddr.ucDevAddr[0];
    }
    pstDataItem->usDataLen = m;
    return ucRet;
}
unsigned char Main_Recv_Get_F38_JobPara(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned char ucJobNo)
{
    unsigned char   ucRet=RET_SUCCESS,i;
    unsigned char   *pucBuffer = pstDataItem->ucDataContent;
    unsigned short  m=0;
    unsigned char ucLitUsrTable[16];
    unsigned char ucUpUsrNo,ucLitUsrNo,ucLitUsrNum;
    STUPUSR stUpUsr;

    ucUpUsrNo = pucBuffer[m++];
    if(ucUpUsrNo >= CON_UPUSR_NUM)
        ucRet = RET_ERROR;
    if(ucRet == RET_SUCCESS)
    {
        ucLitUsrNum = pucBuffer[m++];
        if(ucLitUsrNum >= CON_LITUSR_NUM)
            ucRet = RET_ERROR;
        else
        {
            for(i=0;i<ucLitUsrNum;i++)
            {
                if(pucBuffer[m] > CON_LITUSR_NUM)
                {
                    ucRet = RET_ERROR;
                    break;
                }
                else
                    ucLitUsrTable[i] = pucBuffer[m];
                m ++;
            }
        }
    }
    if(ucRet == RET_SUCCESS)
    {
        m =0;
        stUpUsr = RunPara_GetUpUsrJobPara(ucJobNo, ucUpUsrNo);
        pucBuffer[m++] = ucUpUsrNo;
        pucBuffer[m++] = ucLitUsrNum;
        for(i=0;i<ucLitUsrNum;i++)
        {
            ucLitUsrNo = ucLitUsrTable[i];
            pucBuffer[m++] = ucLitUsrNo;
            pucBuffer[m++] = stUpUsr.stLitType[ucLitUsrNo].ucFnGroupNum;
            memcpy(pucBuffer+m,stUpUsr.stLitType[ucLitUsrNo].ucBitFlag,stUpUsr.stLitType[ucLitUsrNo].ucFnGroupNum);
            m += stUpUsr.stLitType[ucLitUsrNo].ucFnGroupNum;
        }
    }
    pstDataItem->usDataLen = m;
    return ucRet;
}
////F41��ʱ�ι��ض�ֵ
//unsigned char Main_Recv_Get_F41_TimePerSetting(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent;
//  unsigned short  m=0;
//  unsigned char ucTimePerFlag=0,ucProjectFlag=0;
//  STPOWERCTRLTIME stPowerCtrlTime;
//  unsigned short usDT2=0;
//  float fVal;
//  unsigned char ucNum,ucPos;
//  unsigned char i,j;
//  unsigned char ucSumGroupNo=0;
//
//  ucSumGroupNo=ucDAp;
//  if(ucSumGroupNo>=CON_SUMGROUP_NUM)
//      return RET_ERROR;
//
//  pucBuffer[m++]=ucProjectFlag;
//  pucBuffer[m++]=0;
//  ucNum=0;
//  ucPos=0;
//  ucTimePerFlag=0;
//  stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER1,ucSumGroupNo);
//  for(i=0;i<8;i++)
//  {
//      for(j=ucPos;j<CONSWITCHPERTIMENUM;j++)
//      {
//          if(stPowerCtrlTime.stPowerCtrlPer[j].ucCtrlFlag==CON_AVALIBLE_FLAG)
//          {
//              if(fabs(stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.dFixVal-CON_POWER_BIGVAL)>CON_MIN_FLOAT_VAL)
//              {
//                  fVal=(float)stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.dFixVal;
//                  ucTimePerFlag|=(1<<i);
//                  usDT2=Float_To_DT2(fVal);
//                  usDT2=stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.usDT2;
//                  pucBuffer[m++]=(unsigned char)usDT2;
//                  pucBuffer[m++]=(unsigned char)(usDT2>>8);
//                  ucNum++;
//              }
//              ucPos=j+1;
//              break;
//          }
//      }
//  }
//  if(ucTimePerFlag!=0)
//  {
//      ucProjectFlag|=0x01;
//      pucBuffer[m-ucNum-ucNum-1]=ucTimePerFlag;
//  }else
//      m--;
//
//  pucBuffer[m++]=0;
//  ucNum=0;
//  ucPos=0;
//  ucTimePerFlag=0;
//  stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER2,ucSumGroupNo);
//  for(i=0;i<8;i++)
//  {
//      for(j=ucPos;j<CONSWITCHPERTIMENUM;j++)
//      {
//          if(stPowerCtrlTime.stPowerCtrlPer[j].ucCtrlFlag==CON_AVALIBLE_FLAG)
//          {
//              if(fabs(stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.dFixVal-CON_POWER_BIGVAL)>CON_MIN_FLOAT_VAL)
//              {
//                  fVal=(float)stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.dFixVal;
//                  ucTimePerFlag|=(1<<i);
//                  usDT2=Float_To_DT2(fVal);
//                  usDT2=stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.usDT2;
//                  pucBuffer[m++]=(unsigned char)usDT2;
//                  pucBuffer[m++]=(unsigned char)(usDT2>>8);
//                  ucNum++;
//              }
//              ucPos=j+1;
//              break;
//          }
//      }
//  }
//  if(ucTimePerFlag!=0)
//  {
//      ucProjectFlag|=0x02;
//      pucBuffer[m-ucNum-ucNum-1]=ucTimePerFlag;
//  }else
//      m--;
//
//  pucBuffer[m++]=0;
//  ucNum=0;
//  ucPos=0;
//  ucTimePerFlag=0;
//  stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER3,ucSumGroupNo);
//  for(i=0;i<8;i++)
//  {
//      for(j=ucPos;j<CONSWITCHPERTIMENUM;j++)
//      {
//          if(stPowerCtrlTime.stPowerCtrlPer[j].ucCtrlFlag==CON_AVALIBLE_FLAG)
//          {
//              if(fabs(stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.dFixVal-CON_POWER_BIGVAL)>CON_MIN_FLOAT_VAL)
//              {
//                  fVal=(float)stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.dFixVal;
//                  ucTimePerFlag|=(1<<i);
//                  usDT2=Float_To_DT2(fVal);
//                  usDT2=stPowerCtrlTime.stPowerCtrlPer[j].stLoadFixVal.usDT2;
//                  pucBuffer[m++]=(unsigned char)usDT2;
//                  pucBuffer[m++]=(unsigned char)(usDT2>>8);
//                  ucNum++;
//              }
//              ucPos=j+1;
//              break;
//          }
//      }
//  }
//  if(ucTimePerFlag!=0)
//  {
//      ucProjectFlag|=0x04;
//      pucBuffer[m-ucNum-ucNum-1]=ucTimePerFlag;
//  }else
//      m--;
//  if(ucProjectFlag!=0)
//  {
//      pucBuffer[0]=ucProjectFlag;
//  }else
//  {
//      ucRet=RET_ERROR;
//      m=0;
//  }
//  pstDataItem->usDataLen=m;
//  return ucRet;
//}
//
////F42�����ݹ��ز���
//unsigned char Main_Recv_Get_F42_RestCtrl(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   ucTmp,i;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent;
//  unsigned short  m=0;
//  unsigned short usDT2=0;
//  float fVal;
//  STPOWERCTRLTIME stPowerCtrlTime;
//  STPOWERCTRLPROJ stPowerCtrlProj;
//  long lVal;
//  unsigned char ucSumGroupNo=0;
//
//  ucSumGroupNo=ucDAp;
//  if(ucSumGroupNo>=CON_SUMGROUP_NUM)
//      return RET_ERROR;
//
//  stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_RESTCTRL,ucSumGroupNo);
//  fVal=(float)stPowerCtrlTime.stPowerCtrlPer[0].stLoadFixVal.dFixVal;
//  usDT2=Float_To_DT2(fVal);
//  usDT2=stPowerCtrlTime.stPowerCtrlPer[0].stLoadFixVal.usDT2;
//  pucBuffer[m++]=(unsigned char)usDT2;
//  pucBuffer[m++]=(unsigned char)(usDT2>>8);
//  pucBuffer[m++]=(unsigned char)HEX_TO_BCD(stPowerCtrlTime.stPowerCtrlPer[0].ucBeginMinute,1);
//  pucBuffer[m++]=(unsigned char)HEX_TO_BCD(stPowerCtrlTime.stPowerCtrlPer[0].ucBeginHour,1);
//  lVal=stPowerCtrlTime.stPowerCtrlPer[1].ucBeginMinute + stPowerCtrlTime.stPowerCtrlPer[1].ucBeginHour*60
//      - stPowerCtrlTime.stPowerCtrlPer[0].ucBeginMinute - stPowerCtrlTime.stPowerCtrlPer[0].ucBeginHour*60;
//  if(lVal>0)
//      pucBuffer[m++]=(unsigned char)(lVal/30);
//  else
//      pucBuffer[m++]=(unsigned char)((lVal+24*60)/30);
//  stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_RESTCTRL,ucSumGroupNo);
//  ucTmp = 0;
//  for(i=0;i<7;i++)
//  {
//
//      if(stPowerCtrlProj.ucAvailbleFlag[i]==CON_AVALIBLE_FLAG)
//          ucTmp|=(2<<i);
//  }
//  pucBuffer[m++] = ucTmp;
//
//  pstDataItem->usDataLen=m;
//  return ucRet;
//}
//
////F43�����ʿ��ƵĹ��ʼ��㻬��ʱ��
//unsigned char Main_Recv_Get_F43_PowerSlip(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent;
//  unsigned char   ucSumGroupNo=0;
//  unsigned short  m=0;
//  STPOWERCTRLHOLDTIME stPowerHoldTime;
//
//  ucSumGroupNo=ucDAp;
//  if(ucSumGroupNo>=CON_SUMGROUP_NUM)
//      return  RET_ERROR;
//
//  stPowerHoldTime=RunPara_GetPowCtrlHoldTime(ucSumGroupNo);
//  pucBuffer[m++]=(unsigned char)(stPowerHoldTime.usJumpTime[0]/120);
//  pstDataItem->usDataLen=m;
//  return ucRet;
//}
//
////F44��Ӫҵ��ͣ�ز���
//unsigned char Main_Recv_Get_F44_SaleStop(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent;
//  unsigned short  m=0;
//  unsigned short usDT2=0;
//  float fVal;
//  STPOWERCTRLTIME stPowerCtrlTime;
//  STPOWERCTRLPROJ stPowerCtrlProj;
//  unsigned char   ucSumGroupNo=0;
//
//  ucSumGroupNo=ucDAp;
//  if(ucSumGroupNo>=CON_SUMGROUP_NUM)
//      return  RET_ERROR;
//
//  stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_SALESTOP,ucSumGroupNo);
//  stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_SALESTOP,ucSumGroupNo);
//  m=SetMainComm_Buf_DateTime_YMD(pucBuffer,m,stPowerCtrlProj.stStartTime);
//  m=SetMainComm_Buf_DateTime_YMD(pucBuffer,m,stPowerCtrlProj.stEndTime);
//  fVal=(float)stPowerCtrlTime.stPowerCtrlPer[0].stLoadFixVal.dFixVal;
//  usDT2=Float_To_DT2(fVal);
//  usDT2=stPowerCtrlTime.stPowerCtrlPer[0].stLoadFixVal.usDT2;
//  pucBuffer[m++]=(unsigned char)usDT2;
//  pucBuffer[m++]=(unsigned char)(usDT2>>8);
//  pstDataItem->usDataLen=m;
//  return ucRet;
//}
//
////F45�������ִ��趨
//unsigned char Main_Recv_Get_F45_PowerCtrlTrun(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent;
//  unsigned char   m=0;
//  unsigned char i,ucTmp;
//  unsigned char ucSumGNo=0;
//  STCTRLTURN  stPowerCtrlTurn;
//
//  ucSumGNo=ucDAp;
//  if(ucSumGNo>=CON_SUMGROUP_NUM)
//      return  RET_ERROR;
//
//  stPowerCtrlTurn=RunPara_GetPowerCtrlTurn(ucSumGNo);
//  ucTmp=0;
//  for(i=CON_PARA_SWITCHOUT_NUM;i>0;i--)
//  {
//      if(stPowerCtrlTurn.ucAvailbleFlag[i-1]==CON_AVALIBLE_FLAG)
//          ucTmp |= 0x01;
//      if(i>1)
//          ucTmp=(ucTmp<<1);
//  }
//  pucBuffer[m++]=ucTmp;
//  pstDataItem->usDataLen=m;
//  return ucRet;
//}
//
////F46���µ����ض�ֵ
//unsigned char Main_Recv_Get_F46_MonthEnerCtrl(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent;
//  unsigned short  m=0;
//  unsigned long   ulDT3;
//  unsigned char ucSumGNo=0;
//  STMONENERGYREC stMonEnergyRec;
//
//  ucSumGNo=ucDAp;
//  if(ucSumGNo>=CON_SUMGROUP_NUM)
//      return  RET_ERROR;
//
//  stMonEnergyRec=RunPara_GetMonEnergyRec(ucSumGNo);
//  ulDT3=Float_To_DT3(stMonEnergyRec.stMonFixVal.dFixVal,(unsigned char)(stMonEnergyRec.stMonFixVal.usDT2));
//  memcpy(pucBuffer+m,&(ulDT3),4); m+=4;
//  pstDataItem->usDataLen=m;
//  return ucRet;
//}
//
////F47���������ز���
//unsigned char Main_Recv_Get_F47_BuyEnerCtrl(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent;
//  unsigned short  m=0;
//  unsigned long   ulDT3;
//  STBUYENERGYREC stBuyEnergyRec;
//  unsigned char ucSumGNo;
//  ucSumGNo=ucDAp;
//  if(ucSumGNo>=CON_SUMGROUP_NUM)
//      return RET_ERROR;
//
//  stBuyEnergyRec=RunPara_GetBuyEnergyRec(ucSumGNo);
//  memcpy(pucBuffer+m,&(stBuyEnergyRec.ulBuyBillNo),4);
//  m+=4;
//  if(stBuyEnergyRec.ucFlashFlag==CON_ENERGYFLASH_NEW)
//      pucBuffer[m++]=0xAA;
//  else
//      pucBuffer[m++]=0x55;
//  ulDT3=Float_To_DT3(stBuyEnergyRec.stBuyFixVal.dFixVal,(unsigned char)stBuyEnergyRec.stBuyFixVal.usDT2);
//  memcpy(pucBuffer+m,&(ulDT3),4); m+=4;
//  ulDT3=Float_To_DT3(stBuyEnergyRec.stAlrFixVal.dFixVal,(unsigned char)stBuyEnergyRec.stAlrFixVal.usDT2);
//  memcpy(pucBuffer+m,&(ulDT3),4); m+=4;
//  ulDT3=Float_To_DT3(stBuyEnergyRec.stJumpFixVal.dFixVal,(unsigned char)stBuyEnergyRec.stJumpFixVal.usDT2);
//  memcpy(pucBuffer+m,&(ulDT3),4); m+=4;
////    ulDT3=Float_To_DT3(stBuyEnergyRec.stTurnFixVal.dFixVal,(unsigned char)stBuyEnergyRec.stTurnFixVal.usDT2);
////    memcpy(pucBuffer+m,&(ulDT3),4); m+=4;
//  pstDataItem->usDataLen=m;
//  return ucRet;
//}
//
////F48������ִ��趨
//unsigned char Main_Recv_Get_F48_EnerCtrlTurn(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent;
//  unsigned char   m=0;
//  unsigned char i,ucTmp;
//  unsigned char ucSumGNo=0;
//  STCTRLTURN  stEnergyCtrlTurn;
//  ucSumGNo=ucDAp;
//  if(ucSumGNo>=CON_SUMGROUP_NUM)
//      return RET_ERROR;
//
//  stEnergyCtrlTurn=RunPara_GetEnergyCtrlTurn(ucSumGNo);
//  ucTmp=0;
//  for(i=CON_PARA_SWITCHOUT_NUM;i>0;i--)
//  {
//      if(stEnergyCtrlTurn.ucAvailbleFlag[i-1]==CON_AVALIBLE_FLAG)
//          ucTmp |= 0x01;
//      if(i>1)
//          ucTmp=(ucTmp<<1);
//  }
//  pucBuffer[m++]=ucTmp;
//  pstDataItem->usDataLen=m;
//  return ucRet;
//}
//
////F49�����ظ澯ʱ��
//unsigned char Main_Recv_Get_F49_CtrlAlrTime(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned char *pucBuffer = pstDataItem->ucDataContent;
//  unsigned char m=0;
//  unsigned char ucCtrlTurn=0,ucSumGNo=0;
//  STPOWERCTRLHOLDTIME stPowerHoldTime;
//  ucCtrlTurn=ucDAp;
//  if(ucCtrlTurn>=CON_PARA_SWITCHOUT_NUM)
//      ucRet=RET_ERROR;
//
//  ucSumGNo=Load_GetPowerCtrl_SumGNo();
//  if(ucSumGNo>=CON_SUMGROUP_NUM)
//  {
//      ucSumGNo=0;
//      Load_SetPowerCtrl_SumGNo(ucSumGNo);
//  }
//  if(ucRet==RET_SUCCESS)
//  {
//      stPowerHoldTime=RunPara_GetPowCtrlHoldTime(ucSumGNo);
//      pucBuffer[m++]=(unsigned char)(stPowerHoldTime.usJumpAlrTime[ucCtrlTurn]/120);
//  }
//
//  pstDataItem->usDataLen=m;
//  return ucRet;
//}

//F57���ն������澯����M��ֹ����
unsigned char Main_Recv_Get_F57_SoundAlarm(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS;
    unsigned char   m=0;
    STTERMISCPARA  stTerMiscPara;
    stTerMiscPara=RunPara_GetTerMiscPara();
    pstDataItem->ucDataContent[m++]=stTerMiscPara.ucAlarmSoundTime[0];
    pstDataItem->ucDataContent[m++]=stTerMiscPara.ucAlarmSoundTime[1];
    pstDataItem->ucDataContent[m++]=stTerMiscPara.ucAlarmSoundTime[2];
    pstDataItem->usDataLen=m;
    return ucRet;
}

//F58���ն��Զ��������
//unsigned char Main_Recv_Get_F58_KeepAuto(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   m=0;
//  STTERMISCPARA  stTerMiscPara;
//  stTerMiscPara=RunPara_GetTerMiscPara();
//  pstDataItem->ucDataContent[m++]=stTerMiscPara.ucKeepAuto;
//  pstDataItem->usDataLen=m;
//  return ucRet;
//}

//F59�����ܱ��쳣�б���ֵ�趨
unsigned char Main_Recv_Get_F59_MtErrorPara(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS;
    unsigned char   m=0;
    STTERMISCPARA  stTerMiscPara;
    stTerMiscPara=RunPara_GetTerMiscPara();
    pstDataItem->ucDataContent[m++]=(unsigned char)HEX_TO_BCD(stTerMiscPara.ucMeterOverDiff,1);
    pstDataItem->ucDataContent[m++]=(unsigned char)HEX_TO_BCD(stTerMiscPara.ucMeterFly,1);
    pstDataItem->ucDataContent[m++]=stTerMiscPara.ucMeterStop;
    pstDataItem->ucDataContent[m++]=stTerMiscPara.ucMeterTimeDiff;
    pstDataItem->usDataLen=m;
    return ucRet;
}
unsigned char Main_Recv_Get_F61_AnalogPara(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet = RET_SUCCESS;
//  unsigned char m = 0,ucData = 0,i;
//  STANALOGPARA stAnalogPara;
//
//  for(i=0;i<CON_ANALOG_NUM;i++)
//  {
//      stAnalogPara = RunPara_GetAnalogPara(i);
//      if(stAnalogPara.ucAvailable == CON_AVALIBLE_FLAG)
//          ucData |= (1<<i);
//  }
//  pstDataItem->ucDataContent[m++] = ucData;
//  pstDataItem->usDataLen = 1;
    return ucRet;
}
//F62 ����ר��������ʽ
unsigned char Main_Recv_Get_F62_CommType(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS;
    unsigned char ucRcommBaseType =  0;
    unsigned char   m=0;
    STTERCOMMPARA_INNER stTerCommPara_In;
    stTerCommPara_In=RunPara_GetTerCommPara_Inner();
    ucRcommBaseType = RunPara_GetTerMisc_ucRcommBaseType();
    if(ucRcommBaseType == CON_TC35COMMTYPE_UDP)
        ucRcommBaseType = (1<<7);
    else
        ucRcommBaseType = 0;
    pstDataItem->ucDataContent[m++]=stTerCommPara_In.ucRCommMode +ucRcommBaseType;
    pstDataItem->ucDataContent[m++]=(unsigned char)stTerCommPara_In.usReConnectInter;
    pstDataItem->ucDataContent[m++]=(unsigned char)(stTerCommPara_In.usReConnectInter>>8);
    pstDataItem->ucDataContent[m++]=stTerCommPara_In.ucReConnectTimes;
    pstDataItem->ucDataContent[m++]=stTerCommPara_In.ucStandbyTime;
    memcpy(&pstDataItem->ucDataContent[m],stTerCommPara_In.ucTimeInt,3);
    m+=3;
    pstDataItem->usDataLen=m;
    return ucRet;
}

//F63 ��ͨ��������������
unsigned char Main_Recv_Get_F63_FluxLimit(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS;
    unsigned long ulVal;
    ulVal=RunPara_GetTerMisc_FluxLimit();
    memcpy(pstDataItem->ucDataContent,&ulVal,4);
    pstDataItem->usDataLen=4;
    return ucRet;
}
//F65����ʱ����1��������������
unsigned char Main_Recv_Get_F65_UpData1Para(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS,i=0;
    unsigned char   ucJobNo,ucTmp=0;
    STJOBPARA   *pstJobPara_Data1;
    unsigned short  m=0;
    if(ucRet==RET_SUCCESS)
    {
        ucJobNo=ucDAp;
        if(ucJobNo>=CONDATAJOBNUM)
            ucRet =RET_ERROR;
    }
    if(ucRet==RET_SUCCESS)
    {
        pstJobPara_Data1=RunPara_GetJobPara(ucJobNo,0);
        switch(pstJobPara_Data1->ucTimeUnit)
        {
        case CON_TIMEUNIT_MIN:
            ucTmp=0;
            break;
        case CON_TIMEUNIT_HOUR:
            ucTmp=1;
            break;
        case CON_TIMEUNIT_DAY:
            ucTmp=2;
            break;
        case CON_TIMEUNIT_MON:
            ucTmp=3;
            break;
        default:
            ucTmp=0;
            break;
        }
        pstDataItem->ucDataContent[m++]=(ucTmp<<6)+(pstJobPara_Data1->ucTimeInter&0x3F);
        //changed for test 09-11-25
        m=SetMainComm_Buf_DateTime_HMS(pstDataItem->ucDataContent,m,pstJobPara_Data1->stStartActive);
        m=SetMainComm_Buf_DateTime_YMDW(pstDataItem->ucDataContent,m,pstJobPara_Data1->stStartActive);

    //  pstDataItem->ucDataContent[m++]=(unsigned char)HEX_TO_BCD(stJobPara_Data1.stStartActive.ucSecond,1);
    //  pstDataItem->ucDataContent[m++]=(unsigned char)HEX_TO_BCD(stJobPara_Data1.stStartActive.ucMinute,1);
    //  pstDataItem->ucDataContent[m++]=(unsigned char)HEX_TO_BCD(stJobPara_Data1.stStartActive.ucHour,1);
    //  pstDataItem->ucDataContent[m++]=(unsigned char)HEX_TO_BCD(stJobPara_Data1.stStartActive.ucDay,1);
    //  pstDataItem->ucDataContent[m++]=(unsigned char)HEX_TO_BCD(stJobPara_Data1.stStartActive.ucMonth,1);
    //  pstDataItem->ucDataContent[m++]=(unsigned char)HEX_TO_BCD(stJobPara_Data1.stStartActive.ucYear,1);
        pstDataItem->ucDataContent[m++]=pstJobPara_Data1->ucResendDataInter;
        if(pstJobPara_Data1->ucDUIDNum<=CONDATAUNITNUM)
        {
            pstDataItem->ucDataContent[m++]=pstJobPara_Data1->ucDUIDNum;
            for(i=0;i<pstJobPara_Data1->ucDUIDNum;i++)
            {
                pstDataItem->ucDataContent[m++]=pstJobPara_Data1->stDUID[i].ucDA1;
                pstDataItem->ucDataContent[m++]=pstJobPara_Data1->stDUID[i].ucDA2;
                pstDataItem->ucDataContent[m++]=pstJobPara_Data1->stDUID[i].ucDT1;
                pstDataItem->ucDataContent[m++]=pstJobPara_Data1->stDUID[i].ucDT2;
            }
        }
        else
        {
            pstDataItem->ucDataContent[m++]=1;
            pstDataItem->ucDataContent[m++]=0;
            pstDataItem->ucDataContent[m++]=0;
            pstDataItem->ucDataContent[m++]=0;
            pstDataItem->ucDataContent[m++]=0;
        }
        pstDataItem->usDataLen=m;
    }

    return ucRet;
}

//F66����ʱ����2��������������
unsigned char Main_Recv_Get_F66_UpData2Para(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS,i=0;
    unsigned char   ucJobNo,ucTmp=0;
    STJOBPARA   *pstJobPara_Data2;
    unsigned short  m=0;
    if(ucRet==RET_SUCCESS)
    {
        ucJobNo=ucDAp;
        if(ucJobNo>=CONDATAJOBNUM)
            ucRet =RET_ERROR;
    }
    if(ucRet==RET_SUCCESS)
    {
        pstJobPara_Data2=RunPara_GetJobPara(ucJobNo,1);
        switch(pstJobPara_Data2->ucTimeUnit)
        {
        case CON_TIMEUNIT_MIN:
            ucTmp=0;
            break;
        case CON_TIMEUNIT_HOUR:
            ucTmp=1;
            break;
        case CON_TIMEUNIT_DAY:
            ucTmp=2;
            break;
        case CON_TIMEUNIT_MON:
            ucTmp=3;
            break;
        default:
            ucTmp=0;
            break;
        }
        pstDataItem->ucDataContent[m++]=(ucTmp<<6)+(pstJobPara_Data2->ucTimeInter&0x3F);
        //changed for test 09-11-25
        m=SetMainComm_Buf_DateTime_HMS(pstDataItem->ucDataContent,m,pstJobPara_Data2->stStartActive);
        m=SetMainComm_Buf_DateTime_YMDW(pstDataItem->ucDataContent,m,pstJobPara_Data2->stStartActive);

//      pstDataItem->ucDataContent[m++]=(unsigned char)HEX_TO_BCD(stJobPara_Data2.stStartActive.ucSecond,1);
//      pstDataItem->ucDataContent[m++]=(unsigned char)HEX_TO_BCD(stJobPara_Data2.stStartActive.ucMinute,1);
//      pstDataItem->ucDataContent[m++]=(unsigned char)HEX_TO_BCD(stJobPara_Data2.stStartActive.ucHour,1);
//      pstDataItem->ucDataContent[m++]=(unsigned char)HEX_TO_BCD(stJobPara_Data2.stStartActive.ucDay,1);
//      pstDataItem->ucDataContent[m++]=(unsigned char)HEX_TO_BCD(stJobPara_Data2.stStartActive.ucMonth,1);
//      pstDataItem->ucDataContent[m++]=(unsigned char)HEX_TO_BCD(stJobPara_Data2.stStartActive.ucYear,1);
        pstDataItem->ucDataContent[m++]=pstJobPara_Data2->ucResendDataInter;
        if(pstJobPara_Data2->ucDUIDNum<=CONDATAUNITNUM)
        {
            pstDataItem->ucDataContent[m++]=pstJobPara_Data2->ucDUIDNum;
            for(i=0;i<pstJobPara_Data2->ucDUIDNum;i++)
            {
                pstDataItem->ucDataContent[m++]=pstJobPara_Data2->stDUID[i].ucDA1;
                pstDataItem->ucDataContent[m++]=pstJobPara_Data2->stDUID[i].ucDA2;
                pstDataItem->ucDataContent[m++]=pstJobPara_Data2->stDUID[i].ucDT1;
                pstDataItem->ucDataContent[m++]=pstJobPara_Data2->stDUID[i].ucDT2;
            }
        }
        else
        {
            pstDataItem->ucDataContent[m++]=1;
            pstDataItem->ucDataContent[m++]=0;
            pstDataItem->ucDataContent[m++]=0;
            pstDataItem->ucDataContent[m++]=0;
            pstDataItem->ucDataContent[m++]=0;
        }
        pstDataItem->usDataLen=m;
    }

    return ucRet;
}

//F67����ʱ����1��������������/ֹͣ����
unsigned char Main_Recv_Get_F67_UpData1Ctrl(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS;
    unsigned char   ucJobNo;
    STJOBPARA   *pstJobPara_Data1;
    if(ucRet==RET_SUCCESS)
    {
        ucJobNo=ucDAp;
        if(ucJobNo>=CONDATAJOBNUM)
            ucRet =RET_ERROR;
    }
    if(ucRet==RET_SUCCESS)
    {
        pstJobPara_Data1=RunPara_GetJobPara(ucJobNo,0);
        if(pstJobPara_Data1->ucActive==CON_JOB_ACTIVE)
            pstDataItem->ucDataContent[0]=0x55;
        else
            pstDataItem->ucDataContent[0]=0xAA;
        pstDataItem->usDataLen=1;
    }
    return ucRet;
}

//F68����ʱ����2��������������/ֹͣ����
unsigned char Main_Recv_Get_F68_UpData2Ctrl(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS;
    unsigned char   ucJobNo;
    STJOBPARA   *pstJobPara_Data2;
    if(ucRet==RET_SUCCESS)
    {
        ucJobNo=ucDAp;
        if(ucJobNo>=CONDATAJOBNUM)
            ucRet =RET_ERROR;
    }
    if(ucRet==RET_SUCCESS)
    {
        pstJobPara_Data2=RunPara_GetJobPara(ucJobNo,1);
        if(pstJobPara_Data2->ucActive==CON_JOB_ACTIVE)
            pstDataItem->ucDataContent[0]=0x55;
        else
            pstDataItem->ucDataContent[0]=0xAA;
        pstDataItem->usDataLen=1;
    }
    return ucRet;
}


//add by cfh 10-04-03   F73-F76
// unsigned char Main_Recv_Get_F73_F76(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned short usDI)
// {
//  unsigned char   ucRet=RET_SUCCESS;
//  switch(usDI)
//  {
//      case CON_PARA_F73_CAPPARA:
//          memcpy(pstDataItem->ucDataContent,stTerRunPara_Nouse.ucF73,48);
//          pstDataItem->usDataLen=48;
//          break;
//      case CON_PARA_F74_CAPRUNPARA:
//          memcpy(pstDataItem->ucDataContent,stTerRunPara_Nouse.ucF74,10);
//          pstDataItem->usDataLen=10;
//          break;
//      case CON_PARA_F75_CAPRELAYPARA:
//          memcpy(pstDataItem->ucDataContent,stTerRunPara_Nouse.ucF75,16);
//          pstDataItem->usDataLen=16;
//          break;
//      case CON_PARA_F76_CAPCTRL:
//  //      memcpy(stTerRunPara_Nouse.ucF76,pucBuffer,48);
//          pstDataItem->ucDataContent[0] = stTerRunPara_Nouse.ucF76;
//          pstDataItem->usDataLen=1;
//          break;
//  }
//  return ucRet;
// }


// unsigned char Main_Recv_Get_F81_DCRate(STDATAITEM *pstDataItem,unsigned char ucDAp)
// {
//  unsigned char ucRet = RET_SUCCESS;
//  unsigned char ucAnalogNo;
//  unsigned short m =0,usDT2;
//  unsigned char *pucBuffer = pstDataItem->ucDataContent;
//  STANALOGPARA stAnalogPara;
//
//
//  ucAnalogNo = ucDAp;
//  if(ucAnalogNo >= CON_ANALOG_NUM)
//      ucRet = RET_ERROR;
//  if(ucRet == RET_SUCCESS)
//  {
//      stAnalogPara = RunPara_GetAnalogPara( ucAnalogNo);
//      usDT2 = stAnalogPara.stAnalogRate.stStart.usDT2;
//      pucBuffer[m++] = (unsigned char)usDT2;
//      pucBuffer[m++] = (unsigned char)(usDT2>>8);
//
//      usDT2 = stAnalogPara.stAnalogRate.stEnd.usDT2;
//      pucBuffer[m++] = (unsigned char)usDT2;
//      pucBuffer[m++] = (unsigned char)(usDT2>>8);
//      pstDataItem->usDataLen = m;
//  }
//  return ucRet;
// }
// unsigned char Main_Recv_Get_F82_DCLimit(STDATAITEM *pstDataItem,unsigned char ucDAp)
// {
//  unsigned char ucRet = RET_SUCCESS;
//  unsigned char ucAnalogNo;
//  unsigned short m =0,usDT2;
//  unsigned char *pucBuffer = pstDataItem->ucDataContent;
//  STANALOGPARA stAnalogPara;
//
//
//  ucAnalogNo = ucDAp;
//  if(ucAnalogNo >= CON_ANALOG_NUM)
//      ucRet = RET_ERROR;
//  if(ucRet == RET_SUCCESS)
//  {
//      stAnalogPara = RunPara_GetAnalogPara( ucAnalogNo);
//      usDT2 = stAnalogPara.stAnalogLimit.stUpLimit.usDT2;
//      pucBuffer[m++] = (unsigned char)usDT2;
//      pucBuffer[m++] = (unsigned char)(usDT2>>8);
//
//      usDT2 = stAnalogPara.stAnalogLimit.stDownLimit.usDT2;
//      pucBuffer[m++] = (unsigned char)usDT2;
//      pucBuffer[m++] = (unsigned char)(usDT2>>8);
//      pstDataItem->usDataLen = m;
//  }
//  return ucRet;
// }
// unsigned char Main_Recv_Get_F83_M(STDATAITEM *pstDataItem,unsigned char ucDAp)
// {
//  unsigned char ucRet = RET_SUCCESS;
//  unsigned char ucAnalogNo;
//  unsigned char *pucBuffer = pstDataItem->ucDataContent;
//  STANALOGPARA stAnalogPara;
//
//  ucAnalogNo = ucDAp;
//  if(ucAnalogNo >= CON_ANALOG_NUM)
//      ucRet = RET_ERROR;
//  if(ucRet == RET_SUCCESS)
//  {
//      stAnalogPara = RunPara_GetAnalogPara( ucAnalogNo);
//      pucBuffer[0] = (unsigned char)stAnalogPara.ucM;
//      pstDataItem->usDataLen = 1;
//  }
//  return ucRet;
//
// }
unsigned char Main_Recv_Get_F89_TerAddr(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS;
    unsigned char   m=0;
    STTERDEVADDR    stTerAddr=RunPara_GetTerDevAddr();
    STTERREGIONADDR stTerRegionAddr=RunPara_GetTerRegionAddr();
    pstDataItem->ucDataContent[m++] =stTerRegionAddr.ucRegionAddr[0];
    pstDataItem->ucDataContent[m++] =stTerRegionAddr.ucRegionAddr[1];
    pstDataItem->ucDataContent[m++] =stTerAddr.ucDevAddr[0];
    pstDataItem->ucDataContent[m++] =stTerAddr.ucDevAddr[1];

    pstDataItem->usDataLen =4;
    return ucRet;
}

//azh 170919 
//F90������ͨ�Ų���
unsigned char Main_Recv_Get_F90_Wireless(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char   *ptr,ucRet=RET_SUCCESS;
//    unsigned long ulVal;
    ptr=RunPara_GetTerMisc_Wireless();
    memcpy(pstDataItem->ucDataContent,ptr,2);
    pstDataItem->usDataLen=2;
    return ucRet;
}
//F91���ն˵���λ��
unsigned char Main_Recv_Get_F91_Location(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char   *ptr,ucRet=RET_SUCCESS;
//    unsigned long ulVal;
    ptr=RunPara_GetTerMisc_TerLocation();
    memcpy(pstDataItem->ucDataContent,ptr,10);
    pstDataItem->usDataLen=10;
    return ucRet;
}
//F150��������״̬
unsigned char Main_Recv_Get_F150_Snstate(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char   *ptr,ucRet=RET_SUCCESS;
//    unsigned long ulVal;
    ptr=RunPara_GetTerMisc_Snstate();
    memset(pstDataItem->ucDataContent, 0, 512);
    memcpy(pstDataItem->ucDataContent,ptr,1);//azh �����ֽ��ǿղ�֧�ֻ᲻�������
    pstDataItem->usDataLen=512;
    return ucRet;
}

//unsigned char Main_Recv_Get_F90_PhabreTimePara(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char   ucRet=RET_SUCCESS;
//    unsigned char   m=0;
//    STALRJUDGETIME stAlrJudgeTime=RunPara_GetAlrJuDgeTime();
//    pstDataItem->ucDataContent[m++]=stAlrJudgeTime.ucHappedTime;
//    pstDataItem->ucDataContent[m++]=stAlrJudgeTime.ucResumeTime;
//    pstDataItem->ucDataContent[m++]=stAlrJudgeTime.ucMpNo;
//    pstDataItem->usDataLen  = 3;
//    return ucRet;
//}
////F91��ǰ�����¸��صĸ澯ʱ��Ϳ���ʱ��
//unsigned char Main_Recv_Get_F91_AlrCtrlTime(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char   ucRet=RET_SUCCESS;
//    unsigned char   m=0;
//    STCURALRCTRLTIME    stCurAlrCtrlTime = RunPara_GetAlrCtrlTime();
//    pstDataItem->ucDataContent[m++] = stCurAlrCtrlTime.ucFirstALRTime;
//    pstDataItem->ucDataContent[m++] = stCurAlrCtrlTime.ucSecondALRTime;
//    pstDataItem->ucDataContent[m++] = stCurAlrCtrlTime.ucThirdALRTime;
//    pstDataItem->ucDataContent[m++] = stCurAlrCtrlTime.ucFourthALRTime;
//    pstDataItem->ucDataContent[m++] = stCurAlrCtrlTime.ucCurCTLTime;
//    pstDataItem->usDataLen  = 5;
//    return ucRet;
//}
//F92 �����ɼ��ĸ���г����ѹ���������޲���
//unsigned char Main_Recv_Get_F92_HarmValUpParam(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned char m=0,i=0;
//  unsigned short usTmp=0;
//  STHARMVALUPLIMIT    stHarmValUpLimit=RunPara_GetHarmValUpLimit();
//  for(i=0;i<21;i++)
//  {
//      usTmp = (unsigned short)HEX_TO_BCD(*(&stHarmValUpLimit.stVolRateLimit.usHarmValUpTotal+i),2);
//      pstDataItem->ucDataContent[m++] = (unsigned char)(usTmp%0x100);
//      pstDataItem->ucDataContent[m++] = (unsigned char)(usTmp/0x100);
//  }
//  usTmp = (unsigned short)HEX_TO_BCD(*(&stHarmValUpLimit.stCurRmsLimit.usHarmValUpTotal),2);
//  pstDataItem->ucDataContent[m++] = (unsigned char)(usTmp%0x100);
//  pstDataItem->ucDataContent[m++] = (unsigned char)(usTmp/0x100);
//  for(i=0;i<18;i++)
//  {
//      usTmp = (unsigned short)HEX_TO_BCD(*(&stHarmValUpLimit.stCurRmsLimit.usHarmValUpLimit2+i),2);
//      pstDataItem->ucDataContent[m++] = (unsigned char)(usTmp%0x100);
//      pstDataItem->ucDataContent[m++] = (unsigned char)(usTmp/0x100);
//  }
//  pstDataItem->usDataLen=m;
//
//  return ucRet;
//}
//F93������Ϣͨ���ͻ����ݽӿڲ�ѯ
unsigned char Main_Recv_Get_F93_MESSAGE(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned char ucNum)
{
    unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   ucMsgtype,ucMsgNo;
//  unsigned short  m=0;
//  STSHORTMESSAGE *pstShortMessage;
//  ucMsgNo=(ucNum & 0x0F);
//  if(ucMsgNo > 5)             //�洢ÿ���������5��������Ϣ
//      ucMsgNo=5;
//  if(ucMsgNo==0)
//      return RET_ERROR;
//  ucMsgtype=(ucNum >> 4);
//  if(ucMsgtype < 1 || ucMsgtype >2)
//      return RET_ERROR;
//  if(ucMsgtype == 1)              //��Ҫ��Ϣ
//      ucMsgtype=CON_MESSAGE_TYPE_IMPT;
//  else
//      ucMsgtype=CON_MESSAGE_TYPE_NORM;
//  pstShortMessage=LCD_GetHisMessage((unsigned char)(ucMsgNo-1),ucMsgtype);
//  m=SetMainComm_Buf_DateTime_HMS(pstDataItem->ucDataContent,m,pstShortMessage->stDateTimeTemp);
//  m=SetMainComm_Buf_DateTime_YMDW(pstDataItem->ucDataContent,m,pstShortMessage->stDateTimeTemp);
//  pstDataItem->ucDataContent[m++]=ucNum;
//  if(pstShortMessage->ucLength>CON_MESSAGE_DATALEN)
//      pstShortMessage->ucLength=CON_MESSAGE_DATALEN;          //�����쳣������
//  pstDataItem->ucDataContent[m++]=pstShortMessage->ucLength;
//  memcpy(pstDataItem->ucDataContent+m,pstShortMessage->ucMsg,pstShortMessage->ucLength);
//  m+=pstShortMessage->ucLength;
//  pstDataItem->usDataLen=m;
    return ucRet;
}

//F131 GR��������
unsigned char Main_Recv_Get_F131_GRTask(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS;
    /*  unsigned char   *pucBuffer = pstDataItem->ucDataContent;
    unsigned char   ucTaskNo=0;
    unsigned char   m=0;
    ucTaskNo=ucDAp;
    if(ucTaskNo>=CON_GRTASK_NUM)
    return RET_ERROR;

      memcpy(&stGRTaskTmp,&stGRDefineArea.stGRTask[ucTaskNo],sizeof(STGRTASK));
      if(stGRTaskTmp.ucTaskType!=CON_GRTASK_TYPE_NORMAL && stGRTaskTmp.ucTaskType!=CON_GRTASK_TYPE_RELAY)
      return RET_ERROR;

        pucBuffer[m++]=stGRTaskTmp.ucTaskType;
        pucBuffer[m++]=stGRTaskTmp.ucMPNo;
        pucBuffer[m++]=(unsigned char)stGRTaskTmp.usSavePointNum;
        pucBuffer[m++]=(unsigned char)(stGRTaskTmp.usSavePointNum>>8);
        pucBuffer[m++]=(unsigned char)stGRTaskTmp.usExeNum;
        pucBuffer[m++]=(unsigned char)(stGRTaskTmp.usExeNum>>8);
        pucBuffer[m++]=stGRTaskTmp.ucDataItemNum;
        if(stGRTaskTmp.ucActive!=CON_GRTASK_ACTIVE)
        pucBuffer[m++]=CON_GRTASK_NOACTIVE;
        else
        pucBuffer[m++]=CON_GRTASK_ACTIVE;
        pucBuffer[m++]=stGRTaskTmp.stTITaskStart.ucTimeInter;
        pucBuffer[m++]=stGRTaskTmp.stTITaskStart.ucTimeUnit;
        pucBuffer[m++]=stGRTaskTmp.stTITaskInter.ucTimeInter;
        pucBuffer[m++]=stGRTaskTmp.stTITaskInter.ucTimeUnit;
        pucBuffer[m++]=stGRTaskTmp.stTIResendStart.ucTimeInter;
        pucBuffer[m++]=stGRTaskTmp.stTIResendStart.ucTimeUnit;
        pucBuffer[m++]=stGRTaskTmp.stTIReSendInter.ucTimeInter;
        pucBuffer[m++]=stGRTaskTmp.stTIReSendInter.ucTimeUnit;
        pucBuffer[m++]=stGRTaskTmp.ucResendDataInter;
        pucBuffer[m++]=stGRTaskTmp.stRelayTaskPara.ucPortBaudRate;
        pucBuffer[m++]=stGRTaskTmp.stRelayTaskPara.ucRecvWaitTime;
        pucBuffer[m++]=stGRTaskTmp.stRelayTaskPara.ucByteWaitTime;
        pucBuffer[m++]=stGRTaskTmp.stRelayTaskPara.ucCharacterChar;
        pucBuffer[m++]=stGRTaskTmp.stRelayTaskPara.ucReserve;
        pucBuffer[m++]=(unsigned char)stGRTaskTmp.stRelayTaskPara.usGetFrom;
        pucBuffer[m++]=(unsigned char)(stGRTaskTmp.stRelayTaskPara.usGetFrom>>8);
        pucBuffer[m++]=(unsigned char)stGRTaskTmp.stRelayTaskPara.usGetLen;
        pucBuffer[m++]=(unsigned char)(stGRTaskTmp.stRelayTaskPara.usGetLen>>8);
        if(stGRTaskTmp.ucDataItemNum>CON_GRTASK_DINUM)
        stGRTaskTmp.ucDataItemNum=CON_GRTASK_DINUM;
        memcpy(pucBuffer+m,stGRTaskTmp.ucDataArea,2*stGRTaskTmp.ucDataItemNum);
        m+=2*stGRTaskTmp.ucDataItemNum;
        pstDataItem->usDataLen=m;
    */  return ucRet;
}



//F1 �ն˰汾��Ϣ
unsigned char Main_Recv_Config_Get_F1_Version(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char m=0;
    unsigned char ucVerInfo[7];
    STTERAUXPARA stTerAuxPara;
    unsigned char *pucRetBuffer = pstDataItem->ucDataContent;

    stTerAuxPara = RunPara_GetTerAuxPara();
    memcpy(pucRetBuffer+m,stTerAuxPara.ucManufactor,4);     m+=4;
    memcpy(pucRetBuffer+m,stTerAuxPara.ucDevNo,8);          m+=8;
    RunPara_GetTerVerInfo(ucVerInfo);
    memcpy(pucRetBuffer+m,ucVerInfo,7);                     m+=7;
    memcpy(pucRetBuffer+m,stTerAuxPara.ucConfigInfo,11);    m+=11;
    RunPara_GetTerProtoolVerInfo(ucVerInfo);
    memcpy(pucRetBuffer+m,ucVerInfo,4);                     m+=4;
    RunPara_GetTerHardwareVerInfo(ucVerInfo);
    memcpy(pucRetBuffer+m,ucVerInfo,7);                     m+=7;
    pstDataItem->usDataLen=m;
    return ucRet;
}

//F2 �ն����롢�����ͨ�Ŷ˿�����
unsigned char Main_Recv_Config_Get_F2_Interface(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char m=0;
    unsigned char *pucRetBuffer = pstDataItem->ucDataContent;

//    pucRetBuffer[m++] = 0x02;       //����������·��
//    pucRetBuffer[m++] = 0x04;//0x02;        //����������·��
//    pucRetBuffer[m++] = 0x00;       //ֱ��ģ��������·(��֧��)
//    pucRetBuffer[m++] = 0x04;//0x02;        //���������·�����ִΣ�
//    pucRetBuffer[m++] = 0x08;       //֧�ֵĳ����ܱ�/��������װ��������
//    pucRetBuffer[m++] = 0x00;
//    pucRetBuffer[m++] = 0x00;       //֧�ֵ��ն�����ͨ�������ջ������ֽ���1024
//    pucRetBuffer[m++] = 0x04;
//    pucRetBuffer[m++] = 0x00;       //֧�ֵ��ն�����ͨ������ͻ������ֽ���1024
//    pucRetBuffer[m++] = 0x04;
//    memset(pucRetBuffer+m,0xee,6);  //MAC��ַ
//    m += 6;
//    pucRetBuffer[m++] = 0x02;       //ͨ�Ŷ˿�����3
//    //��һ·
//    pucRetBuffer[m++] = 0x01;
//    pucRetBuffer[m++] = 0x00;
//    pucRetBuffer[m++] = 0x80;       //��߲�����9600
//    pucRetBuffer[m++] = 0x25;
//    pucRetBuffer[m++] = 0x00;
//    pucRetBuffer[m++] = 0x00;
//    pucRetBuffer[m++] = 0x08;       //֧��8���
//    pucRetBuffer[m++] = 0x00;
//    pucRetBuffer[m++] = 0x00;       //���ջ���������1024
//    pucRetBuffer[m++] = 0x04;
//    pucRetBuffer[m++] = 0x00;       //���ͻ���������1024
//    pucRetBuffer[m++] = 0x04;
//    //�ڶ�·
//    pucRetBuffer[m++] = 0x02;
//    pucRetBuffer[m++] = 0x00;
//    pucRetBuffer[m++] = 0x80;       //��߲�����9600
//    pucRetBuffer[m++] = 0x25;
//    pucRetBuffer[m++] = 0x00;
//    pucRetBuffer[m++] = 0x00;
//    pucRetBuffer[m++] = 0x08;       //֧��8���
//    pucRetBuffer[m++] = 0x00;
//    pucRetBuffer[m++] = 0x00;       //���ջ���������1024
//    pucRetBuffer[m++] = 0x04;
//    pucRetBuffer[m++] = 0x00;       //���ͻ���������1024
//    pucRetBuffer[m++] = 0x04;
//    //����·
////  pucRetBuffer[m++] = 0x62;
////  pucRetBuffer[m++] = 0x00;
////  pucRetBuffer[m++] = 57;         //��߲�����57.6K
////  pucRetBuffer[m++] = 0x04;
////  pucRetBuffer[m++] = 0x01;       //֧��1���豸
////  pucRetBuffer[m++] = 0x00;
////  pucRetBuffer[m++] = 0x00;       //���ջ���������1024
////  pucRetBuffer[m++] = 0x04;
////  pucRetBuffer[m++] = 0x00;       //���ͻ���������1024
//  pucRetBuffer[m++] = 0x04;
    memcpy(pucRetBuffer+m, Config_Get_F2_Interface, 41);
    m += 41;

    pstDataItem->usDataLen=m;
    return ucRet;
}

//F3 �ն���������
unsigned char Main_Recv_Config_Get_F3_Other(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char m=0;
    unsigned char *pucRetBuffer = pstDataItem->ucDataContent;

//    pucRetBuffer[m++] = 64;
//    pucRetBuffer[m++] = 0;      //֧�ֵĲ����������� 64��
//    pucRetBuffer[m++] = 4;      //֧�ֵ��ܼ���������� 8��
//    pucRetBuffer[m++] = 18;     //֧�ֵ����������� 18��
//    pucRetBuffer[m++] = 0;      //֧�ֵ��й��ܵ��������������� 0
//    pucRetBuffer[m++] = 4;      //֧�ֵ��������� 4
//    pucRetBuffer[m++] = 1;      //֧�ֵĲ�����������󶳽��ܶ� 15����һ��
//    pucRetBuffer[m++] = 1;      //֧�ֵ��ܼ����й�����������󶳽��ܶ� 15����һ��
//    pucRetBuffer[m++] = 1;      //֧�ֵ��ܼ����޹�����������󶳽��ܶ� 15����һ��
//    pucRetBuffer[m++] = 1;      //֧�ֵ��ܼ����й�������������󶳽��ܶ� 15����һ��
//    pucRetBuffer[m++] = 1;      //֧�ֵ��ܼ����޹�������������󶳽��ܶ� 15����һ��
//    pucRetBuffer[m++] = 31;     //֧�ֵ���������������� 30
//    pucRetBuffer[m++] = 12;     //֧�ֵ���������������� 30
//    pucRetBuffer[m++] = 3;      //֧�ֵ�ʱ�ι��ض�ֵ����������
//    pucRetBuffer[m++] = 0;      //֧�ֵ�г��������г������
//    pucRetBuffer[m++] = 0;      //֧�ֵ��޹��������������������
//    pucRetBuffer[m++] = 0;      //֧�ֵļ��г����ص㻧��໧��
//    pucRetBuffer[m++] = 0x03;   //֧�ִ��û���Ϊ��֧��
//    pucRetBuffer[m++] = 0;
    memcpy(pucRetBuffer+m, Config_Get_F3_Other, 19);
    m += 19;
    memset(pucRetBuffer+m,0x00,16);//С�û���ȫΪ0
    m += 16;
    pstDataItem->usDataLen=m;
    return ucRet;
}


//F4 �ն˲�������
unsigned char Main_Recv_Config_Get_F4_Para(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char m=0;
    unsigned char *pucRetBuffer = pstDataItem->ucDataContent;

//    pucRetBuffer[m++] = 9;
//    pucRetBuffer[m++] = 0xFF;       //  F8  F7  F6  F5  F4  F3  F2  F1
//    pucRetBuffer[m++] = 0XEF;       //  F16 F15 F14 F   F12 F11 F10 F9
//    pucRetBuffer[m++] = 0XFF;       //  F24 F23 F22 F21 F20 F19 F18 F17
//    pucRetBuffer[m++] = 0X1F;       //  F   F   F   F29 F28 F27 F26 F25
//    pucRetBuffer[m++] = 0X79;       //  F   F39 F38 F37 F36 F   F   F33
//    pucRetBuffer[m++] = 0XFF;       //  F48 F47 F46 F45 F44 F43 F42 F41
//    pucRetBuffer[m++] = 1;          //  F   F   F   F   F   F   F   F49
//    pucRetBuffer[m++] = 0X07;       //  F   F   F   F   F   F59 F58 F57
//    pucRetBuffer[m++] = 0X0F;       //  F   F   F   F   F68 F67 F66 F65
    memcpy(pucRetBuffer+m, Config_Get_F4_Para, 10);
    m += 10;
    
    pstDataItem->usDataLen=m;
    return ucRet;
}

//F5 �ն˿�������
unsigned char Main_Recv_Config_Get_F5_Ctrl(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char m=0;
    unsigned char *pucRetBuffer = pstDataItem->ucDataContent;

//    pucRetBuffer[m++] = 7;
//    pucRetBuffer[m++] = 0x03;       //  F   F   F   F   F   F   F2  F1
//    pucRetBuffer[m++] = 0XCF;       //  F16 F15 F   F   F12 F11 F10 F9
//    pucRetBuffer[m++] = 0XCF;       //  F24 F23 F   F   F20 F19 F18 F17
//    pucRetBuffer[m++] = 0XDF;       //  F32 F31 F   F29 F28 F27 F26 F25
//    pucRetBuffer[m++] = 0X7F;       //  F   F39 F38 F37 F36 F35 F34 F33
//    pucRetBuffer[m++] = 0X00;       //  F   F   F   F   F   F   F   F
//    pucRetBuffer[m++] = 0X07;       //  F   F   F   F   F   F51 F50 F49
    memcpy(pucRetBuffer+m, Config_Get_F5_Ctrl, 8);
    m += 8;

    pstDataItem->usDataLen=m;
    return ucRet;
}


//F6 �ն�һ����������
unsigned char Main_Recv_Config_Get_F6_Data1(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char m=0,i;
    unsigned char *pucRetBuffer = pstDataItem->ucDataContent;

    pucRetBuffer[m++] = 0x03;   //֧�ִ��û���Ϊ��֧��
    pucRetBuffer[m++] = 0;
    for(i=0;i<2;i++)
    {
//        pucRetBuffer[m++] = 19;
//        pucRetBuffer[m++] = 0xfe;       //  F8  F7  F6  F5  F4  F3  F2  F
//        pucRetBuffer[m++] = 0X03;       //  F   F   F   F   F   F   F10 F9
//        pucRetBuffer[m++] = 0XFF;       //  F24 F23 F22 F21 F20 F19 F18 F17
//        pucRetBuffer[m++] = 0X0F;       //  F   F   F   F   F28 F27 F26 F25
//        pucRetBuffer[m++] = 0XFF;       //  F40 F39 F38 F37 F36 F35 F34 F33
//        pucRetBuffer[m++] = 0XFF;       //  F48 F47 F46 F45 F44 F43 F42 F41
//        pucRetBuffer[m++] = 0X00;       //  F   F   F   F   F   F   F   F
//        pucRetBuffer[m++] = 0X00;       //  F   F   F   F   F   F   F   F
//        pucRetBuffer[m++] = 0X00;       //  F   F   F   F   F   F   F   F
//        pucRetBuffer[m++] = 0X00;       //  F   F   F   F   F   F   F   F
//        pucRetBuffer[m++] = 0X0F;       //  F   F   F   F   F84 F83 F82 F81
//        pucRetBuffer[m++] = 0XFF;       //  F96 F95 F94 F93 F92 F91 F90 F89
//        pucRetBuffer[m++] = 0X3F;       //  F   F   F102F101F100F99 F98 F97
//        pucRetBuffer[m++] = 0XFF;       //  F112F111F110F109F108F107F106F105
//        pucRetBuffer[m++] = 0X0F;       //  F   F   F   F   F116F115F114F113
//        pucRetBuffer[m++] = 0X00;       //  F   F   F   F   F   F   F   F
//        pucRetBuffer[m++] = 0XFF;       //  F136F135F134F133F132F131F130F129
//        pucRetBuffer[m++] = 0XFF;       //  F144F143F142F141F140F139F138F137
//        pucRetBuffer[m++] = 0XFF;       //  F152F151F150F149F148F147F146F145
        memcpy(pucRetBuffer+m, Config_Get_F6_Data1, 20);
        m += 20;
    }
    pstDataItem->usDataLen=m;
    return ucRet;
}

//F7 �ն˶�����������
unsigned char Main_Recv_Config_Get_F7_Data2(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char m=0,i=0;
    unsigned char *pucRetBuffer = pstDataItem->ucDataContent;

    pucRetBuffer[m++] = 0x03;   //֧�ִ��û���Ϊ��֧��
    pucRetBuffer[m++] = 0;
    for(i=0;i<2;i++)
    {
//        pucRetBuffer[m++] = 25;
//        pucRetBuffer[m++] = 0xff;       //  F8  F7  F6  F5  F4  F3  F2  F1
//        pucRetBuffer[m++] = 0X0f;       //  F   F   F   F   F12 F11 F10 F9
//        pucRetBuffer[m++] = 0XFF;       //  F24 F23 F22 F21 F20 F19 F18 F17
//        pucRetBuffer[m++] = 0XbF;       //  F32 F   F30 F29 F28 F27 F26 F25
//        pucRetBuffer[m++] = 0X3F;       //  F   F   F38 F37 F36 F35 F34 F33
//        pucRetBuffer[m++] = 0X03;       //  F   F   F   F   F   F   F42 F41
//        pucRetBuffer[m++] = 0X3f;       //  F   F   F54 F53 F52 F51 F50 F49
//        pucRetBuffer[m++] = 0X3f;       //  F   F   F62 F61 F60 F59 F58 F57
//        pucRetBuffer[m++] = 0X03;       //  F   F   F   F   F   F   F66 F65
//        pucRetBuffer[m++] = 0X0f;       //  F   F   F   F   F76 F75 F74 F73
//        pucRetBuffer[m++] = 0XfF;       //  F88 F87 F86 F85 F84 F83 F82 F81
//        pucRetBuffer[m++] = 0X3F;       //  F   F   F94 F93 F92 F91 F90 F89
//        pucRetBuffer[m++] = 0XfF;       //  F104F103F102F101F100F99 F98 F97
//        pucRetBuffer[m++] = 0X0F;       //  F   F   F   F   F108F107F106F105
//        pucRetBuffer[m++] = 0X00;       //  F   F   F   F   F   F   F   F
//        pucRetBuffer[m++] = 0X00;       //  F   F   F   F   F   F   F   F
//        pucRetBuffer[m++] = 0X00;       //  F   F   F   F   F   F   F   F
//        pucRetBuffer[m++] = 0X00;       //  F   F   F   F   F   F   F   F
//        pucRetBuffer[m++] = 0X0f;       //  F   F   F   F   F148F147F146F145
//        pucRetBuffer[m++] = 0X00;       //  F   F   F   F   F   F   F   F
//        pucRetBuffer[m++] = 0XFF;       //  F168F167F166F165F164F163F162F161
//        pucRetBuffer[m++] = 0XFF;       //  F176F175F174F173F172F171F170F169
//        pucRetBuffer[m++] = 0XFF;       //  F184F183F182F181F180F179F178F177
//        pucRetBuffer[m++] = 0XFF;       //  F192F191F190F189F188F187F186F185
//        pucRetBuffer[m++] = 0X0f;       //  F   F   F   F   F196F195F194F193
        memcpy(pucRetBuffer+m, Config_Get_F7_Data2, 26);
        m += 26;
    }
    pstDataItem->usDataLen=m;
    return ucRet;
}


//F8 �ն��¼���¼����
unsigned char Main_Recv_Config_Get_F8_ERC(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char m=0;
    unsigned char *pucRetBuffer = pstDataItem->ucDataContent;

////  pucRetBuffer[m++] = 5;

//    pucRetBuffer[m++] = 0xff;       //  F8  F7  F6  F5  F4  F3  F2  F1
//    pucRetBuffer[m++] = 0Xcf;       //  F   F   F14 F13 F12 F11 F10 F9
//    pucRetBuffer[m++] = 0XFd;       //  F24 F23 F22 F21 F20 F19 F   F17
//    pucRetBuffer[m++] = 0XfF;       //  F32 F31 F30 F29 F28 F27 F26 F25
//    pucRetBuffer[m++] = 0X03;       //  F   F   F   F   F   F   F34 F33
//    pucRetBuffer[m++] = 0;
//    pucRetBuffer[m++] = 0;
//    pucRetBuffer[m++] = 0;
    memcpy(pucRetBuffer+m, Config_Get_F8_ERC, 8);
    m += 8;
    
    pstDataItem->usDataLen=m;
    return ucRet;
}

//azh 170920
//F9Զ��ͨ��ģ��汾��Ϣ
extern const unsigned char gucRmoteVerInfo[46];
unsigned char Main_Recv_Config_Get_F9_REMOTEVER(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char   *pucRetBuffer = pstDataItem->ucDataContent;

    memcpy(pucRetBuffer, gucRmoteVerInfo, 46);
    pstDataItem->usDataLen=46;
    return ucRet;
}
//F10����ͨ��ģ��汾��Ϣ
extern const unsigned char gucLocalVerInfo[15];
unsigned char Main_Recv_Config_Get_F10_LOCALVER(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char   *pucRetBuffer = pstDataItem->ucDataContent;

    memcpy(pucRetBuffer, gucLocalVerInfo, 15);
    pstDataItem->usDataLen=15;
    return ucRet;
}


unsigned char Main_Recv_Login_Set_F1_Password(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS,i;
    unsigned short  m=0;
    STTERENCRYPT    stTerEncrypt;
    unsigned char   *pucBuffer = pstDataItem->ucDataContent;
    stTerEncrypt=RunPara_GetTerEncrypt();
    for(i=0;i<16;i++)
        stTerEncrypt.ucPassword_New[i] = pucBuffer[m++];
    RunPara_SetTerEncrypt(&stTerEncrypt);
    return ucRet;

}

unsigned char Main_Recv_Login_Get_F2_Password(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char   ucRet=RET_SUCCESS;
    unsigned char   *pucBuffer = pstDataItem->ucDataContent;
    unsigned short  m=0;
    unsigned char   i;
    STTERENCRYPT    stTerEncrypt;
    stTerEncrypt = RunPara_GetTerEncrypt();
    for(i=0;i<16;i++)
        pucBuffer[m++] =stTerEncrypt.ucPassword_New[i];
    pstDataItem->usDataLen=m;
    return ucRet;
}

////F1 ң����բ
//unsigned char Main_Recv_Ctrl_F1_YK_Open(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned char ucCtrlTurn;
//  unsigned char ucTmp;
//  unsigned short usJumpTimeCount,usJumpTimeAlrCount;
//  unsigned long ulJumpTimeLastSecond;
//  ucCtrlTurn=ucDAp;
//  if(ucCtrlTurn>=CON_SWITCHOUT_NUM)
//      ucRet=RET_ERROR;
//  if(RunPara_GetKeepPower()==CON_AVALIBLE_FLAG)       //����
//      ucRet=RET_ERROR;
//  if(ucRet==RET_SUCCESS)
//  {
//      ucTmp=pucBuffer[0]&0x0F;
//      ulJumpTimeLastSecond=ucTmp*1800;                //��λ0.5Сʱת��Ϊ��λ1��
//      ucTmp=pucBuffer[0]>>4;
//      usJumpTimeAlrCount=ucTmp*120;                   //��λ1����ת��Ϊ��λ0.5��
//      usJumpTimeCount=1;
//      Load_RemoteResetRevPara(ucCtrlTurn);
//      Load_RemoteSetJumpPara(ucCtrlTurn,usJumpTimeCount,usJumpTimeAlrCount,ulJumpTimeLastSecond);
//      Load_RemoteResetJumpCount(ucCtrlTurn);
//  }
//    return ucRet;
//}
//
////F2 ң�������բ
//unsigned char Main_Recv_Ctrl_F2_YK_Close(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned char ucCtrlTurn;
//  unsigned short usRevTimeCount,usRevTimeAlrCount;
//  ucCtrlTurn=ucDAp;
//  if(ucCtrlTurn>=CON_SWITCHOUT_NUM)
//      ucRet=RET_ERROR;
//  if(ucRet==RET_SUCCESS)
//  {
//      usRevTimeCount=1;
//      usRevTimeAlrCount=0;
//      Load_RemoteResetJumpPara(ucCtrlTurn);
//      Load_RemoteSetRevPara(ucCtrlTurn,usRevTimeCount,usRevTimeAlrCount);
//      Load_RemoteResetRevCount(ucCtrlTurn);
//  }
//    return ucRet;
//}
//
////F9ʱ�ι���Ͷ��
//unsigned char Main_Recv_Ctrl_F9_TimePer_On(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned char ucSumGroupNo,i;
//  unsigned char ucProjectNo,ucTimePerFlag,ucTmp,ucFlag;
//  STPOWERCTRLTIME stPowerCtrlTime;
//  STPOWERCTRLPROJ stPowerCtrlProj;
//  ucSumGroupNo=ucDAp;
//  if(ucSumGroupNo>=CON_SUMGROUP_NUM)
//      ucRet=RET_ERROR;
//  ucTmp=pucBuffer[1];
//  if(ucTmp>2)
//      ucRet=RET_ERROR;
////    if(RunPara_GetKeepPower()==CON_AVALIBLE_FLAG)       //����  //����Ҫ�󱣵�ʱ����Ͷ��
////        ucRet=RET_ERROR;
//  if(ucRet==RET_SUCCESS)
//  {
//      Load_SetPowerCtrl_SumGNo(ucSumGroupNo);             //���ù��ʿ����õ��ܼ���
//      ucTimePerFlag=pucBuffer[0];
//      if(ucTmp==0)
//      {
//          ucProjectNo=CONSWITCHCURVE_TIMEPER1;
//          stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER2,ucSumGroupNo);
//          stPowerCtrlTime.ucAvailbleFlag=CON_INVALIBLE_FLAG;
//          RunPara_SetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER2,&stPowerCtrlTime,ucSumGroupNo);
//          stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER3,ucSumGroupNo);
//          stPowerCtrlTime.ucAvailbleFlag=CON_INVALIBLE_FLAG;
//          RunPara_SetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER3,&stPowerCtrlTime,ucSumGroupNo);
//          Load_Ctrl_Reset((unsigned char)(CONSWITCH_SOUR_POWER+CONSWITCHCURVE_TIMEPER2));
//          Load_Ctrl_Reset((unsigned char)(CONSWITCH_SOUR_POWER+CONSWITCHCURVE_TIMEPER3));
//          stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_TIMEPER2,ucSumGroupNo);
//          stPowerCtrlProj.ucProjAvailbleFlag=CON_INVALIBLE_FLAG;
//          RunPara_SetPowCtrlProj(CONSWITCHCURVE_TIMEPER2,&stPowerCtrlProj,ucSumGroupNo);
//          stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_TIMEPER3,ucSumGroupNo);
//          stPowerCtrlProj.ucProjAvailbleFlag=CON_INVALIBLE_FLAG;
//          RunPara_SetPowCtrlProj(CONSWITCHCURVE_TIMEPER3,&stPowerCtrlProj,ucSumGroupNo);
//      }
//      if(ucTmp==1)
//      {
//          ucProjectNo=CONSWITCHCURVE_TIMEPER2;
//          stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER1,ucSumGroupNo);
//          stPowerCtrlTime.ucAvailbleFlag=CON_INVALIBLE_FLAG;
//          RunPara_SetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER1,&stPowerCtrlTime,ucSumGroupNo);
//          stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER3,ucSumGroupNo);
//          stPowerCtrlTime.ucAvailbleFlag=CON_INVALIBLE_FLAG;
//          RunPara_SetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER3,&stPowerCtrlTime,ucSumGroupNo);
//          Load_Ctrl_Reset((unsigned char)(CONSWITCH_SOUR_POWER+CONSWITCHCURVE_TIMEPER1));
//          Load_Ctrl_Reset((unsigned char)(CONSWITCH_SOUR_POWER+CONSWITCHCURVE_TIMEPER3));
//          stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_TIMEPER1,ucSumGroupNo);
//          stPowerCtrlProj.ucProjAvailbleFlag=CON_INVALIBLE_FLAG;
//          RunPara_SetPowCtrlProj(CONSWITCHCURVE_TIMEPER1,&stPowerCtrlProj,ucSumGroupNo);
//          stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_TIMEPER3,ucSumGroupNo);
//          stPowerCtrlProj.ucProjAvailbleFlag=CON_INVALIBLE_FLAG;
//          RunPara_SetPowCtrlProj(CONSWITCHCURVE_TIMEPER3,&stPowerCtrlProj,ucSumGroupNo);
//      }
//      if(ucTmp==2)
//      {
//          ucProjectNo=CONSWITCHCURVE_TIMEPER3;
//          stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER1,ucSumGroupNo);
//          stPowerCtrlTime.ucAvailbleFlag=CON_INVALIBLE_FLAG;
//          RunPara_SetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER1,&stPowerCtrlTime,ucSumGroupNo);
//          stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER2,ucSumGroupNo);
//          stPowerCtrlTime.ucAvailbleFlag=CON_INVALIBLE_FLAG;
//          RunPara_SetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER2,&stPowerCtrlTime,ucSumGroupNo);
//          Load_Ctrl_Reset((unsigned char)(CONSWITCH_SOUR_POWER+CONSWITCHCURVE_TIMEPER1));
//          Load_Ctrl_Reset((unsigned char)(CONSWITCH_SOUR_POWER+CONSWITCHCURVE_TIMEPER2));
//          stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_TIMEPER1,ucSumGroupNo);
//          stPowerCtrlProj.ucProjAvailbleFlag=CON_INVALIBLE_FLAG;
//          RunPara_SetPowCtrlProj(CONSWITCHCURVE_TIMEPER1,&stPowerCtrlProj,ucSumGroupNo);
//          stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_TIMEPER2,ucSumGroupNo);
//          stPowerCtrlProj.ucProjAvailbleFlag=CON_INVALIBLE_FLAG;
//          RunPara_SetPowCtrlProj(CONSWITCHCURVE_TIMEPER2,&stPowerCtrlProj,ucSumGroupNo);
//      }
//      stPowerCtrlTime=RunPara_GetPowCtrlTimePer(ucProjectNo,ucSumGroupNo);
//      ucFlag=0;
//      for(i=0;i<CONSWITCHPERTIMENUM;i++)
//      {
//          if(stPowerCtrlTime.stPowerCtrlPer[i].ucAvailbleFlag==CON_AVALIBLE_FLAG && stPowerCtrlTime.stPowerCtrlPer[i].ucCtrlFlag==CON_AVALIBLE_FLAG)
//          {//��Ч��8��ʱ��
//              ucTmp=(1<<ucFlag);
//              if((ucTimePerFlag&ucTmp)==ucTmp)
//                  stPowerCtrlTime.stPowerCtrlPer[i].ucOnFlag=CON_AVALIBLE_FLAG;
//              else
//                  stPowerCtrlTime.stPowerCtrlPer[i].ucOnFlag=CON_INVALIBLE_FLAG;
//              ucFlag++;
//              if(ucFlag==8)
//                  break;
//          }
//      }
//      stPowerCtrlTime.ucAvailbleFlag=CON_AVALIBLE_FLAG;
//      RunPara_SetPowCtrlTimePer(ucProjectNo,&stPowerCtrlTime,ucSumGroupNo);
//      stPowerCtrlProj=RunPara_GetPowCtrlProj(ucProjectNo,ucSumGroupNo);
//      stPowerCtrlProj.ucProjAvailbleFlag=CON_AVALIBLE_FLAG;
//      stPowerCtrlProj.stStartTime = GetCurrentTime();
//      RunPara_SetPowCtrlProj(ucProjectNo,&stPowerCtrlProj,ucSumGroupNo);
//      ucTimeChange[ucProjectNo]=CONINVALIDVAL;
//      CopyMeter_Timer_StartNow();
//  }
//    return ucRet;
//}
//
////F10���ݹ���Ͷ��
//unsigned char Main_Recv_Ctrl_F10_RestCtrl_On(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned char ucSumGroupNo;
//  STPOWERCTRLPROJ stPowerCtrlProj;
//  ucSumGroupNo=ucDAp;
//  if(ucSumGroupNo>=CON_SUMGROUP_NUM)
//      ucRet=RET_ERROR;
////    if(RunPara_GetKeepPower()==CON_AVALIBLE_FLAG)       //����  //����Ҫ�󱣵�ʱ����Ͷ��
////        ucRet=RET_ERROR;
//  if(ucRet==RET_SUCCESS)
//  {
//      Load_SetPowerCtrl_SumGNo(ucSumGroupNo);             //���ù��ʿ����õ��ܼ���
//      stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_RESTCTRL,ucSumGroupNo);
//      stPowerCtrlProj.ucProjAvailbleFlag=CON_AVALIBLE_FLAG;
//      stPowerCtrlProj.stStartTime = GetCurrentTime();
//      RunPara_SetPowCtrlProj(CONSWITCHCURVE_RESTCTRL,&stPowerCtrlProj,ucSumGroupNo);
//      stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_RESTCTRL,ucSumGroupNo);
//      ucTimeChange[CONSWITCHCURVE_RESTCTRL]=CONINVALIDVAL;
//      CopyMeter_Timer_StartNow();
//  }
//    return ucRet;
//}
//
////F11Ӫҵ��ͣ����Ͷ��
//unsigned char Main_Recv_Ctrl_F11_SaleStop_On(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned char ucSumGroupNo;
//  STPOWERCTRLPROJ stPowerCtrlProj;
//  ucSumGroupNo=ucDAp;
//  if(ucSumGroupNo>=CON_SUMGROUP_NUM)
//      ucRet=RET_ERROR;
////    if(RunPara_GetKeepPower()==CON_AVALIBLE_FLAG)       //����  //����Ҫ�󱣵�ʱ����Ͷ��
////        ucRet=RET_ERROR;
//  if(ucRet==RET_SUCCESS)
//  {
//      Load_SetPowerCtrl_SumGNo(ucSumGroupNo);             //���ù��ʿ����õ��ܼ���
//      stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_SALESTOP,ucSumGroupNo);
//      stPowerCtrlProj.ucProjAvailbleFlag=CON_AVALIBLE_FLAG;
//      //stPowerCtrlProj.stStartTime = GetCurrentTime();       //ע����ע��Ӱ��
//      RunPara_SetPowCtrlProj(CONSWITCHCURVE_SALESTOP,&stPowerCtrlProj,ucSumGroupNo);
//      ucTimeChange[CONSWITCHCURVE_SALESTOP]=CONINVALIDVAL;
//      CopyMeter_Timer_StartNow();
//  }
//    return ucRet;
//}
//
////F12��ǰ�����¸���Ͷ��
//unsigned char Main_Recv_Ctrl_F12_PowerLower_On(unsigned char *pucBuffer,unsigned char ucDAp)
//{//���������Լ8���ֽ�
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned char ucSumGroupNo;
//  unsigned char ucTmp,i;
//  unsigned short usCoef;
//  STPOWERCTRLTIME stPowerCtrlTime;
//  STPOWERCTRLPROJ stPowerCtrlProj;
//  double  dCurrInstantPow = 0;
//  STDATETIME stDateTime;
//  ucSumGroupNo=ucDAp;
//  if(ucSumGroupNo>=CON_SUMGROUP_NUM)
//      ucRet=RET_ERROR;
//  if(pucBuffer[0]==0 || pucBuffer[0]>60)
//      ucRet=RET_ERROR;
//  if(pucBuffer[2]<5 || pucBuffer[2]>60)
//      ucRet=RET_ERROR;
//  ucTmp=(pucBuffer[1]&0x7F);
//  if(Check_BCD_Data(ucTmp)!=RET_SUCCESS)
//      ucRet=RET_ERROR;
//  if(pucBuffer[3]==0)
//      ucRet=RET_ERROR;                    //����ʱ�����ٱ�֤2����Ҫ�澯
////    if(RunPara_GetKeepPower()==CON_AVALIBLE_FLAG)       //����  //����Ҫ�󱣵�ʱ����Ͷ��
////        ucRet=RET_ERROR;
//  if(ucRet==RET_SUCCESS)
//  {
//      Load_SetPowerCtrl_SumGNo(ucSumGroupNo);             //���ù��ʿ����õ��ܼ���
//      ucTmp=(pucBuffer[1]&0x7F);
//      ucTmp=(unsigned char)BCD_TO_HEX_1(&ucTmp);
//      if((pucBuffer[1]&0x80)==0x80)
//          usCoef=100-ucTmp;
//      else
//          usCoef=100+ucTmp;
//      dCurrInstantPow = Load_GetCurrentInstantPow(ucSumGroupNo);
//      stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_POWLOWER,ucSumGroupNo);
//      stPowerCtrlTime.ucAvailbleFlag=CON_AVALIBLE_FLAG;
//      stPowerCtrlTime.stPowerCtrlPer[0].ucAvailbleFlag=CON_AVALIBLE_FLAG;
//      stPowerCtrlTime.stPowerCtrlPer[0].ucCtrlFlag=CON_AVALIBLE_FLAG;
//      stPowerCtrlTime.stPowerCtrlPer[0].stLoadFixVal.usFloatCoef=usCoef;
//      if(dCurrInstantPow==CONINVALIDVAL)
//          stPowerCtrlTime.stPowerCtrlPer[0].stLoadFixVal.dFixVal=CONINVALIDVAL;
//      else
//          stPowerCtrlTime.stPowerCtrlPer[0].stLoadFixVal.dFixVal=dCurrInstantPow;
//      stPowerCtrlTime.stPowerCtrlPer[0].ucBeginMinute=0;
//      stPowerCtrlTime.stPowerCtrlPer[0].ucBeginHour=0;
//      stPowerCtrlTime.stPowerCtrlPer[1].ucAvailbleFlag=CON_AVALIBLE_FLAG;
//      stPowerCtrlTime.stPowerCtrlPer[1].ucCtrlFlag=CON_INVALIBLE_FLAG;
////        stPowerCtrlTime.stPowerCtrlPer[1].ucBeginMinute=0;
////        stPowerCtrlTime.stPowerCtrlPer[1].ucBeginHour=0;
//      stDateTime=GetCurrentTime();
//      if(pucBuffer[3]==0)             //0��ʾ�������ƣ�ʵ���Ͽ���128Сʱ
//          pucBuffer[3]=0xFF;
//      if((stDateTime.ucHour*60+stDateTime.ucMinute+pucBuffer[3]*30)>=(23*60+59))      //�ڶ���
//      {
//          stDateTime.ucHour=23;
//          stDateTime.ucMinute=59;
//      }else
//          AddTime(&stDateTime,(pucBuffer[3]*30),CON_TIMEUNIT_MIN);
//      stPowerCtrlTime.stPowerCtrlPer[1].ucBeginMinute=stDateTime.ucMinute;
//      stPowerCtrlTime.stPowerCtrlPer[1].ucBeginHour=stDateTime.ucHour;
//      for(i=2;i<CONSWITCHPERTIMENUM;i++)
//      {
//          stPowerCtrlTime.stPowerCtrlPer[i].ucAvailbleFlag=CON_INVALIBLE_FLAG;
//          stPowerCtrlTime.stPowerCtrlPer[i].ucCtrlFlag=CON_INVALIBLE_FLAG;
//      }
//      RunPara_SetPowCtrlTimePer(CONSWITCHCURVE_POWLOWER,&stPowerCtrlTime,ucSumGroupNo);
//      stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_POWLOWER,ucSumGroupNo);
//      stPowerCtrlProj.ucProjAvailbleFlag=CON_AVALIBLE_FLAG;
//      stPowerCtrlProj.ucAvailbleFlag[0]=CON_AVALIBLE_FLAG;
//      stPowerCtrlProj.ucCycleType=CON_CYCLETYPE_DAY;
//      stPowerCtrlProj.usCycleNum=0xFFFF;
//      stPowerCtrlProj.stStartTime=GetCurrentTime();
//      RunPara_SetPowCtrlProj(CONSWITCHCURVE_POWLOWER,&stPowerCtrlProj,ucSumGroupNo);
//      ucTimeChange[CONSWITCHCURVE_POWLOWER]=CONINVALIDVAL;
//      stDateTime=GetCurrentTime();
//      AddTime(&stDateTime,pucBuffer[2],CON_TIMEUNIT_MIN);
//      Set_PowLower_FreeseTime(&stDateTime,pucBuffer[2],pucBuffer[0]);
//      CopyMeter_Timer_StartNow();
//      Load_PowLowerCtrlPara_Set(pucBuffer[3],pucBuffer[4],pucBuffer[5],pucBuffer[6],pucBuffer[7]);
//
//
//
//  }
//    return ucRet;
//}
//
////F15�µ��Ͷ��
//unsigned char Main_Recv_Ctrl_F15_MonthEner_On(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned char ucSumGroupNo=0;
//  unsigned char i=0;
//  STENERGYCTRL  stEnergyCtrl;
//  STMONENERGYREC  stMonEnergyRec;
//  ucSumGroupNo=ucDAp;
//  if(ucSumGroupNo>=CON_SUMGROUP_NUM)
//      ucRet=RET_ERROR;
////    if(RunPara_GetKeepPower()==CON_AVALIBLE_FLAG)       //����  //����Ҫ�󱣵�ʱ����Ͷ��
////        ucRet=RET_ERROR;
//  if(ucRet==RET_SUCCESS)
//  {
//      Load_SetEnergyCtrl_SumGNo(ucSumGroupNo);                //���õ��������õ��ܼ���
//      stEnergyCtrl=RunPara_GetEnergyCtrl(ucSumGroupNo);
//      stMonEnergyRec=RunPara_GetMonEnergyRec(ucSumGroupNo);
//      if(stEnergyCtrl.ucEnergyCtrlType==CON_ENERGYCTRL_BUY)
//          Load_Ctrl_Reset(CONSWITCH_SOUR_ENERGY);                 //����������
//      stEnergyCtrl.stMonthEnergy.dFixVal=stMonEnergyRec.stMonFixVal.dFixVal;
//      stEnergyCtrl.stAlrFixVal.dFixVal=stMonEnergyRec.stMonFixVal.dFixVal*20/100;         //�ﵽ80%�澯
//      stEnergyCtrl.stAlrFixVal.usFloatCoef=stEnergyCtrl.stMonthEnergy.usFloatCoef;
//      stEnergyCtrl.stJumpFixVal.dFixVal=0;
//      stEnergyCtrl.stJumpFixVal.usFloatCoef=stEnergyCtrl.stMonthEnergy.usFloatCoef;
//      stEnergyCtrl.stTurnAlrFixVal.dFixVal=stEnergyCtrl.stAlrFixVal.dFixVal;
//      stEnergyCtrl.stTurnAlrFixVal.usFloatCoef=stEnergyCtrl.stAlrFixVal.usFloatCoef;
//      stEnergyCtrl.stTurnJumpFixVal.dFixVal=0;
//      stEnergyCtrl.stTurnJumpFixVal.usFloatCoef=stEnergyCtrl.stJumpFixVal.usFloatCoef;
//      stEnergyCtrl.ucEnergyCtrlType=CON_ENERGYCTRL_MON;
//      stEnergyCtrl.ucTurnFlag=0;
//      RunPara_SetEnergyCtrl(&stEnergyCtrl,ucSumGroupNo);
//      Load_SetEnergy_StaPowVal_LM();
//      for(i=0;i<CON_SUMGROUP_NUM;i++)
//      {
//          if(i!=ucSumGroupNo)
//              stHisSumGPowTotal_Sta.stSumValGroup.fSumVal[i]=CONINVALIDVAL;
//      }
//  }
//    return ucRet;
//}
//
////F16�����Ͷ��
//unsigned char Main_Recv_Ctrl_F16_BuyEner_On(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned char ucSumGroupNo;
//  unsigned char i=0;
//  STENERGYCTRL  stEnergyCtrl;
//  STBUYENERGYREC  stBuyEnergyRec;
//  ucSumGroupNo=ucDAp;
//  if(ucSumGroupNo>=CON_SUMGROUP_NUM)
//      ucRet=RET_ERROR;
////    if(RunPara_GetKeepPower()==CON_AVALIBLE_FLAG)       //����  //����Ҫ�󱣵�ʱ����Ͷ��
////        ucRet=RET_ERROR;
//  if(ucRet==RET_SUCCESS)
//  {
//      Load_SetEnergyCtrl_SumGNo(ucSumGroupNo);                //���õ��������õ��ܼ���
//      stEnergyCtrl=RunPara_GetEnergyCtrl(ucSumGroupNo);
//      stBuyEnergyRec=RunPara_GetBuyEnergyRec(ucSumGroupNo);
//      if(stEnergyCtrl.ucEnergyCtrlType==CON_ENERGYCTRL_MON)
//          Load_Ctrl_Reset(CONSWITCH_SOUR_ENERGY);                 //����µ����
//      stEnergyCtrl.stBuyEnergy.dFixVal=Load_Energy_LeftPowVal_CalcBuy(&stEnergyCtrl,ucSumGroupNo);
//      stEnergyCtrl.stAlrFixVal.dFixVal=stBuyEnergyRec.stAlrFixVal.dFixVal;
//      stEnergyCtrl.stAlrFixVal.usFloatCoef=stEnergyCtrl.stBuyEnergy.usFloatCoef;
//      stEnergyCtrl.stJumpFixVal.dFixVal=stBuyEnergyRec.stJumpFixVal.dFixVal;
//      stEnergyCtrl.stJumpFixVal.usFloatCoef=stEnergyCtrl.stBuyEnergy.usFloatCoef;
//      stEnergyCtrl.stTurnAlrFixVal.dFixVal=stEnergyCtrl.stAlrFixVal.dFixVal;
//      stEnergyCtrl.stTurnAlrFixVal.usFloatCoef=stEnergyCtrl.stAlrFixVal.usFloatCoef;
//      stEnergyCtrl.stTurnJumpFixVal.dFixVal=stEnergyCtrl.stJumpFixVal.dFixVal;
//      stEnergyCtrl.stTurnJumpFixVal.usFloatCoef=stEnergyCtrl.stJumpFixVal.usFloatCoef;
//      stEnergyCtrl.stTurnFixVal.dFixVal=stBuyEnergyRec.stTurnFixVal.dFixVal;
//      stEnergyCtrl.ucEnergyCtrlType=CON_ENERGYCTRL_BUY;
//      stEnergyCtrl.ucTurnFlag=0;
//      RunPara_SetEnergyCtrl(&stEnergyCtrl,ucSumGroupNo);
//      Load_SetEnergy_StaPowVal();
//      for(i=0;i<CON_SUMGROUP_NUM;i++)
//      {
//          if(i!=ucSumGroupNo)
//          {
//              stHisSumGPowFee1_Sta.stSumValGroup.fSumVal[i]=CONINVALIDVAL;
//              stHisSumGPowFee2_Sta.stSumValGroup.fSumVal[i]=CONINVALIDVAL;
//              stHisSumGPowFee3_Sta.stSumValGroup.fSumVal[i]=CONINVALIDVAL;
//              stHisSumGPowFee4_Sta.stSumValGroup.fSumVal[i]=CONINVALIDVAL;
//          }
//      }
//  }
//    return ucRet;
//}
//
////F17ʱ�ι��ؽ��
//unsigned char Main_Recv_Ctrl_F17_TimePer_Off(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned char ucSumGroupNo;
//  STPOWERCTRLTIME stPowerCtrlTime;
//  STPOWERCTRLPROJ stPowerCtrlProj;
//  ucSumGroupNo=ucDAp;
//  if(ucSumGroupNo>=CON_SUMGROUP_NUM)
//      ucRet=RET_ERROR;
//  if(ucRet==RET_SUCCESS)
//  {
//      stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER1,ucSumGroupNo);
//      stPowerCtrlTime.ucAvailbleFlag=CON_INVALIBLE_FLAG;
//      RunPara_SetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER1,&stPowerCtrlTime,ucSumGroupNo);
//      stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER2,ucSumGroupNo);
//      stPowerCtrlTime.ucAvailbleFlag=CON_INVALIBLE_FLAG;
//      RunPara_SetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER2,&stPowerCtrlTime,ucSumGroupNo);
//      stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER3,ucSumGroupNo);
//      stPowerCtrlTime.ucAvailbleFlag=CON_INVALIBLE_FLAG;
//      RunPara_SetPowCtrlTimePer(CONSWITCHCURVE_TIMEPER3,&stPowerCtrlTime,ucSumGroupNo);
//      Load_Ctrl_Reset((unsigned char)(CONSWITCH_SOUR_POWER+CONSWITCHCURVE_TIMEPER1));
//      Load_Ctrl_Reset((unsigned char)(CONSWITCH_SOUR_POWER+CONSWITCHCURVE_TIMEPER2));
//      Load_Ctrl_Reset((unsigned char)(CONSWITCH_SOUR_POWER+CONSWITCHCURVE_TIMEPER3));
//      stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_TIMEPER1,ucSumGroupNo);
//      stPowerCtrlProj.ucProjAvailbleFlag=CON_INVALIBLE_FLAG;
//      RunPara_SetPowCtrlProj(CONSWITCHCURVE_TIMEPER1,&stPowerCtrlProj,ucSumGroupNo);
//      stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_TIMEPER2,ucSumGroupNo);
//      stPowerCtrlProj.ucProjAvailbleFlag=CON_INVALIBLE_FLAG;
//      RunPara_SetPowCtrlProj(CONSWITCHCURVE_TIMEPER2,&stPowerCtrlProj,ucSumGroupNo);
//      stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_TIMEPER3,ucSumGroupNo);
//      stPowerCtrlProj.ucProjAvailbleFlag=CON_INVALIBLE_FLAG;
//      RunPara_SetPowCtrlProj(CONSWITCHCURVE_TIMEPER3,&stPowerCtrlProj,ucSumGroupNo);
//  }
//    return ucRet;
//}
//
////F18���ݹ��ؽ��
//unsigned char Main_Recv_Ctrl_F18_RestCtrl_Off(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned char ucSumGroupNo;
//  STPOWERCTRLTIME stPowerCtrlTime;
//  STPOWERCTRLPROJ stPowerCtrlProj;
//  ucSumGroupNo=ucDAp;
//  if(ucSumGroupNo>=CON_SUMGROUP_NUM)
//      ucRet=RET_ERROR;
//  if(ucRet==RET_SUCCESS)
//  {
//      stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_RESTCTRL,ucSumGroupNo);
//      stPowerCtrlTime.ucAvailbleFlag=CON_INVALIBLE_FLAG;
//      RunPara_SetPowCtrlTimePer(CONSWITCHCURVE_RESTCTRL,&stPowerCtrlTime,ucSumGroupNo);
//      Load_Ctrl_Reset((unsigned char)(CONSWITCH_SOUR_POWER+CONSWITCHCURVE_RESTCTRL));
//      stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_RESTCTRL,ucSumGroupNo);
//      stPowerCtrlProj.ucProjAvailbleFlag=CON_INVALIBLE_FLAG;
//      RunPara_SetPowCtrlProj(CONSWITCHCURVE_RESTCTRL,&stPowerCtrlProj,ucSumGroupNo);
//  }
//    return ucRet;
//}
//
////F19Ӫҵ��ͣ���ؽ��
//unsigned char Main_Recv_Ctrl_F19_SaleStop_Off(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned char ucSumGroupNo;
//  STPOWERCTRLTIME stPowerCtrlTime;
//  STPOWERCTRLPROJ stPowerCtrlProj;
//  ucSumGroupNo=ucDAp;
//  if(ucSumGroupNo>=CON_SUMGROUP_NUM)
//      ucRet=RET_ERROR;
//  if(ucRet==RET_SUCCESS)
//  {
//      stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_SALESTOP,ucSumGroupNo);
//      stPowerCtrlTime.ucAvailbleFlag=CON_INVALIBLE_FLAG;
//      RunPara_SetPowCtrlTimePer(CONSWITCHCURVE_SALESTOP,&stPowerCtrlTime,ucSumGroupNo);
//      Load_Ctrl_Reset((unsigned char)(CONSWITCH_SOUR_POWER+CONSWITCHCURVE_SALESTOP));
//      stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_SALESTOP,ucSumGroupNo);
//      stPowerCtrlProj.ucProjAvailbleFlag=CON_INVALIBLE_FLAG;
//      RunPara_SetPowCtrlProj(CONSWITCHCURVE_SALESTOP,&stPowerCtrlProj,ucSumGroupNo);
//  }
//    return ucRet;
//}
//
////F20��ǰ�����¸��ؽ��
//unsigned char Main_Recv_Ctrl_F20_PowerLower_Off(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned char ucSumGroupNo;
//  STPOWERCTRLTIME stPowerCtrlTime;
//  STPOWERCTRLPROJ stPowerCtrlProj;
//  ucSumGroupNo=ucDAp;
//  if(ucSumGroupNo>=CON_SUMGROUP_NUM)
//      ucRet=RET_ERROR;
//  if(ucRet==RET_SUCCESS)
//  {
//      stPowerCtrlTime=RunPara_GetPowCtrlTimePer(CONSWITCHCURVE_POWLOWER,ucSumGroupNo);
//      stPowerCtrlTime.ucAvailbleFlag=CON_INVALIBLE_FLAG;
//      RunPara_SetPowCtrlTimePer(CONSWITCHCURVE_POWLOWER,&stPowerCtrlTime,ucSumGroupNo);
//      Load_Ctrl_Reset((unsigned char)(CONSWITCH_SOUR_POWER+CONSWITCHCURVE_POWLOWER));
//      stPowerCtrlProj=RunPara_GetPowCtrlProj(CONSWITCHCURVE_POWLOWER,ucSumGroupNo);
//      stPowerCtrlProj.ucProjAvailbleFlag=CON_INVALIBLE_FLAG;
//      RunPara_SetPowCtrlProj(CONSWITCHCURVE_POWLOWER,&stPowerCtrlProj,ucSumGroupNo);
//  }
//    return ucRet;
//}
//
////F23�µ�ؽ��
//unsigned char Main_Recv_Ctrl_F23_MonthEner_Off(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned char ucSumGroupNo=0;
//  STENERGYCTRL  stEnergyCtrl;
//  ucSumGroupNo=ucDAp;
//  if(ucSumGroupNo>=CON_SUMGROUP_NUM)
//      ucRet=RET_ERROR;
//  if(ucRet==RET_SUCCESS)
//  {
//      stEnergyCtrl=RunPara_GetEnergyCtrl(ucSumGroupNo);
//      if(stEnergyCtrl.ucEnergyCtrlType==CON_ENERGYCTRL_MON)
//      {
//          Load_Ctrl_Reset(CONSWITCH_SOUR_ENERGY);                 //����µ����
//          stEnergyCtrl.ucEnergyCtrlType=CON_ENERGYCTRL_INVALID;
//          RunPara_SetEnergyCtrl(&stEnergyCtrl,ucSumGroupNo);
//      }
//  }
//    return ucRet;
//}
//
////F24����ؽ��
//unsigned char Main_Recv_Ctrl_F24_BuyEner_Off(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned char ucSumGroupNo=0;
//  STENERGYCTRL  stEnergyCtrl;
//  ucSumGroupNo=ucDAp;
//  if(ucSumGroupNo>=CON_SUMGROUP_NUM)
//      ucRet=RET_ERROR;
//  if(ucRet==RET_SUCCESS)
//  {
//      stEnergyCtrl=RunPara_GetEnergyCtrl(ucSumGroupNo);
//      if(stEnergyCtrl.ucEnergyCtrlType==CON_ENERGYCTRL_BUY)
//      {
//          stEnergyCtrl.stBuyEnergy.dFixVal=Load_Energy_LeftPowVal_CalcBuy(&stEnergyCtrl,ucSumGroupNo);    //ʣ���������
//          Load_Ctrl_Reset(CONSWITCH_SOUR_ENERGY);                 //����������
//          stEnergyCtrl.ucEnergyCtrlType=CON_ENERGYCTRL_INVALID;
//          RunPara_SetEnergyCtrl(&stEnergyCtrl,ucSumGroupNo);
//      }
//  }
//    return ucRet;
//}
//
////F25�ն˱���Ͷ��
//unsigned char Main_Recv_Ctrl_F25_KeepPower_On(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  STDATETIME stCurrentTime;
//  if(pucBuffer[0]>48)
//      ucRet=RET_ERROR;
//  if(ucRet==RET_SUCCESS)
//  {
//      RunPara_SetKeepPower(CON_AVALIBLE_FLAG);
//      if(pucBuffer[0]==0)
//          ClearDateTime(&stCurrentTime);
//      else
//      {
//          stCurrentTime=GetCurrentTime();
//          AddTime(&stCurrentTime,30*pucBuffer[0],CON_TIMEUNIT_MIN);
//      }
//      Set_KeepPowEnd_Time(&stCurrentTime);
//  }
//    return ucRet;
//}
//
////F26�߷Ѹ澯Ͷ��
//unsigned char Main_Recv_Ctrl_F26_UrgeAlarm_On(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  STLOADCTRLWORDPARA  stLoadCtrlWordPara;
//  stLoadCtrlWordPara=RunPara_GetLoadCtrlWordPara();
//  stLoadCtrlWordPara.ucUrgeFeeCtrl=CON_AVALIBLE_FLAG;
//  ucRet=RunPara_SetLoadCtrlWordPara(&stLoadCtrlWordPara);
//    return ucRet;
//}

//F27�����ն�����վͨ��
unsigned char Main_Recv_Ctrl_F27_Talk_On(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STTERMISCPARA stTerMiscPara;
    stTerMiscPara=RunPara_GetTerMiscPara();
    stTerMiscPara.ucTalkOn=CON_AVALIBLE_FLAG;
    RunPara_SetTerMiscPara(&stTerMiscPara);
    return ucRet;
}

////F28�ն��޳�Ͷ��
//unsigned char Main_Recv_Ctrl_F28_Remove_On(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//    STTERMISCPARA stTerMiscPara;
//    stTerMiscPara=RunPara_GetTerMiscPara();
//    stTerMiscPara.ucRemoveOn=CON_AVALIBLE_FLAG;
//    RunPara_SetTerMiscPara(&stTerMiscPara);
//    return ucRet;
//}

//F29�����ն������ϱ�
unsigned char Main_Recv_Ctrl_F29_UpActive_On(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STTERMISCPARA stTerMiscPara;
    stTerMiscPara=RunPara_GetTerMiscPara();
    stTerMiscPara.ucUpActiveOn=CON_AVALIBLE_FLAG;
    RunPara_SetTerMiscPara(&stTerMiscPara);
    return ucRet;
}
//azh 170920
//F30�ն˶Ե��ܱ��ʱ���ܵĿ�����ر�
unsigned char Main_Recv_Ctrl_F30_SetMeterTime_Switch(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;

    stTerRunPara.ucSetMeterTimeOn = pucBuffer[0];//azh ʵ�ʲ�������
    return ucRet;
}

//F31��ʱ����
unsigned char Main_Recv_Ctrl_F31_SetTerTime(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char ucTemp;
    unsigned char ucWeek;
    unsigned short m=0;
    unsigned char ucMsgSource;
    STDATETIME stDateTimeTemp;
//    STDATETIME stOrigTime;
//  stOrigTime=GetCurrentTime();
    if(Check_BCD_Data(pucBuffer[m++])==RET_ERROR) return RET_ERROR;
    if(Check_BCD_Data(pucBuffer[m++])==RET_ERROR) return RET_ERROR;
    if(Check_BCD_Data(pucBuffer[m++])==RET_ERROR) return RET_ERROR;
    if(Check_BCD_Data(pucBuffer[m++])==RET_ERROR) return RET_ERROR;
    if(Check_BCD_Data((unsigned char)(pucBuffer[m++] & 0x1F))==RET_ERROR) return RET_ERROR; //����-�� D7D6D5-D4D3D2D1D0
    if(Check_BCD_Data(pucBuffer[m++])==RET_ERROR) return RET_ERROR;

    m=0;
    stDateTimeTemp.ucSecond=(unsigned char)BCD_TO_HEX_1(&pucBuffer[m++]);
    stDateTimeTemp.ucMinute=(unsigned char)BCD_TO_HEX_1(&pucBuffer[m++]);
    stDateTimeTemp.ucHour=(unsigned char)BCD_TO_HEX_1(&pucBuffer[m++]);
    stDateTimeTemp.ucDay=(unsigned char)BCD_TO_HEX_1(&pucBuffer[m++]);
    ucTemp=pucBuffer[m] & 0x1F;
    stDateTimeTemp.ucMonth=(unsigned char)BCD_TO_HEX_1(&ucTemp);
    ucTemp=pucBuffer[m++] >> 5;
    ucWeek=(unsigned char)BCD_TO_HEX_1(&ucTemp);
    stDateTimeTemp.ucYear=(unsigned char)BCD_TO_HEX_1(&pucBuffer[m++]);
    if(Is_DateTime_Availble(&stDateTimeTemp)==RET_ERROR)
        return RET_ERROR;
    if(GetCurrentWeek(&stDateTimeTemp)!=ucWeek && ucWeek!=0)
        return RET_ERROR;

    ucMsgSource=Main_Get_MsgSource();
    if(ucMsgSource==CON_TERMSG_SOUR_RCOMM_TCP || ucMsgSource==CON_TERMSG_SOUR_RCOMM_UDP)
        AddTime(&stDateTimeTemp,2,CON_TIMEUNIT_SEC);        //�Ӷ���
    else
        AddTime(&stDateTimeTemp,1,CON_TIMEUNIT_SEC);        //��һ��
    //����ʱ��
    SetDateTime(&stDateTimeTemp);       //�޸�8025ʱ��

    //����ʱ����������Ϣ
//azh   TimeChange_PulseHisData(stOrigTime,stDateTimeTemp);
//  TimeChange_JobManage(stOrigTime,stDateTimeTemp);
//  TimeChange_CTaskProc(stOrigTime,stDateTimeTemp);    //change for test 09-11-24
//  TimeChange_AlarmManage(stOrigTime,stDateTimeTemp);

//  Beep_ExternPara_Change();
    return ucRet;
}

//F32������Ϣ
//unsigned char Main_Recv_Ctrl_F32_Message(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned char ucType=0,ucNo=0,ucLength=0;
//  STDATETIME stDateTimeTemp;
//  stDateTimeTemp=GetCurrentTime();
//
//  ucType=(unsigned char)(pucBuffer[0] >> 4);
//  ucNo=(unsigned char)(pucBuffer[0] & 0x0F);
//  if(ucNo>CON_MESSAGE_NUM)        //0��ʾFIFO��1-5��5��
//      return RET_ERROR;
//  ucLength=pucBuffer[1];
//
//  if((ucType==0) || (ucType==1))
//  {//��������0��1��������
//      if(ucLength>200)
//          ucRet=RET_ERROR;
//      else
//          LCD_SetNewMessage(pucBuffer+2,ucType,ucNo,ucLength,stDateTimeTemp);
//  }
//  else
//      ucRet=RET_ERROR;
//  return ucRet;
//}

//F33�ն˱�����
unsigned char Main_Recv_Ctrl_F33_KeepPower_Off(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
//  STDATETIME stCurrentTime;
//  RunPara_SetKeepPower(CON_INVALIBLE_FLAG);
//  ClearDateTime(&stCurrentTime);
//  Set_KeepPowEnd_Time(&stCurrentTime);
    return ucRet;
}

////F34�߷Ѹ澯���
//unsigned char Main_Recv_Ctrl_F34_UrgeAlarm_Off(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
////  STLOADCTRLWORDPARA  stLoadCtrlWordPara;
////  stLoadCtrlWordPara=RunPara_GetLoadCtrlWordPara();
////  stLoadCtrlWordPara.ucUrgeFeeCtrl=CON_INVALIBLE_FLAG;
////  ucRet=RunPara_SetLoadCtrlWordPara(&stLoadCtrlWordPara);
//    return ucRet;
//}

//F35��ֹ�ն�����վͨ��
unsigned char Main_Recv_Ctrl_F35_Talk_Off(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STTERMISCPARA stTerMiscPara;
    stTerMiscPara=RunPara_GetTerMiscPara();
    stTerMiscPara.ucTalkOn=CON_INVALIBLE_FLAG;
    RunPara_SetTerMiscPara(&stTerMiscPara);
    return ucRet;
}

////F36�ն��޳����
//unsigned char Main_Recv_Ctrl_F36_Remove_Off(unsigned char *pucBuffer,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//    STTERMISCPARA stTerMiscPara;
//    stTerMiscPara=RunPara_GetTerMiscPara();
//    stTerMiscPara.ucRemoveOn=CON_INVALIBLE_FLAG;
//    RunPara_SetTerMiscPara(&stTerMiscPara);
//    return ucRet;
//}

//F37��ֹ�ն������ϱ�
unsigned char Main_Recv_Ctrl_F37_UpActive_Off(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STTERMISCPARA stTerMiscPara;
    stTerMiscPara=RunPara_GetTerMiscPara();
    stTerMiscPara.ucUpActiveOn=CON_INVALIBLE_FLAG;
    RunPara_SetTerMiscPara(&stTerMiscPara);
    return ucRet;
}

//F38���ż���
unsigned char Main_Recv_Ctrl_F38_SMS_Active(unsigned char *pucBuffer,unsigned char ucDAp,unsigned char ucMsgSource)
{
    unsigned char   ucRet=RET_SUCCESS;
    //�ӳ�15�룬�ȴ����ŷ�������λ0.5��
//  CommConnect_SetNeedLoginFlag(CON_STATUS_YES);    //changed for test 09-11-25
//  MainComm_StartUp(CON_ONLINE_SMS_ACTIVE);
    return ucRet;
}


//F38���ż���
unsigned char Main_Recv_Ctrl_F39_Disconect(unsigned char *pucBuffer,unsigned char ucDAp,unsigned char ucMsgSource)
{
    unsigned char   ucRet=RET_SUCCESS;
    //�ӳ�15�룬�ȴ����ŷ�������λ0.5��
    CommConnect_SetNeedLoginFlag(CON_STATUS_NO);
//  MainComm_StartUp(CON_ONLINE_SMS_ACTIVE);
    return ucRet;
}
//azh 170920
//F149���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
unsigned char Main_Recv_Ctrl_F149_MeterSelf_Switch(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;

    stTerRunPara.ucMeterSelfOn = pucBuffer[0];//azh ʵ�ʲ�������
    return ucRet;
}
//F150��λ��������������־
unsigned char Main_Recv_Ctrl_F150_ResetSnState_Switch(unsigned char *pucBuffer,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;

//azh ʵ�ʲ�������
    return ucRet;
}
//================  TaskData ======================
//F1    1�����������ѯ
unsigned char Main_Recv_ReqTaskData_F1(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char ucJobNo=0,ucDUIDNum=0,i=0;
    STJOBPARA   *pstJobPara_Data1;
    unsigned short usPos=0;
    if(ucRet==RET_SUCCESS)
    {
        ucJobNo=ucDAp;
        if(ucJobNo>=CONDATAJOBNUM)
            ucRet=RET_ERROR;
    }
    if(ucRet==RET_SUCCESS)
    {
        pstJobPara_Data1=RunPara_GetJobPara(ucJobNo,0);
        if(pstJobPara_Data1->ucActive==CON_JOB_ACTIVE)
        {
            if(pstJobPara_Data1->ucDUIDNum>=CONDATAUNITNUM)
                ucDUIDNum=CONDATAUNITNUM;
            else
                ucDUIDNum=pstJobPara_Data1->ucDUIDNum;
            for(i=0;i<ucDUIDNum;i++)
            {
               ucRet=Job_GetData_Data1(pstJobPara_Data1->stDUID[i],pstDataItem->ucDataContent,&usPos);
                if(ucRet==RET_ERROR)
                {
                    usPos-=4;
                    break;
                }
            }
        }else
            ucRet=RET_ERROR;
    }
    pstDataItem->usDataLen=usPos;
    return ucRet;
}

//F2    2�����������ѯ
unsigned char Main_Recv_ReqTaskData_F2(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char ucJobNo=0,ucDUIDNum=0,i=0;
    STJOBPARA   *pstJobPara_Data2;
    unsigned short usPos=0;
    STDATETIME  stDataBeginTime;
    STDATETIME	stDataEndTime;
    unsigned char m=0;
    unsigned char *pucBuffer = pstDataItem->ucDataContent;
    if(ucRet==RET_SUCCESS)
    {
        ucJobNo=ucDAp;
        if(ucJobNo>=CONDATAJOBNUM)
            ucRet=RET_ERROR;
    }

    if(Check_BCD_Data(pucBuffer[m++])==RET_ERROR) return RET_ERROR;
    if(Check_BCD_Data(pucBuffer[m++])==RET_ERROR) return RET_ERROR;
    if(Check_BCD_Data(pucBuffer[m++])==RET_ERROR) return RET_ERROR;
    if(Check_BCD_Data(pucBuffer[m++])==RET_ERROR) return RET_ERROR;
    if(Check_BCD_Data(pucBuffer[m++])==RET_ERROR) return RET_ERROR;
    m=0;
    stDataBeginTime.ucSecond=0;
    stDataBeginTime.ucMinute=(unsigned char)BCD_TO_HEX_1(&pucBuffer[m++]);
    stDataBeginTime.ucHour=(unsigned char)BCD_TO_HEX_1(&pucBuffer[m++]);
    stDataBeginTime.ucDay=(unsigned char)BCD_TO_HEX_1(&pucBuffer[m++]);
    stDataBeginTime.ucMonth=(unsigned char)BCD_TO_HEX_1(&pucBuffer[m++]);
    stDataBeginTime.ucYear=(unsigned char)BCD_TO_HEX_1(&pucBuffer[m++]);
    if(Is_DateTime_Availble(&stDataBeginTime)==RET_ERROR)
        return RET_ERROR;

    if(ucRet==RET_SUCCESS)
    {
        pstJobPara_Data2=RunPara_GetJobPara(ucJobNo,1);
        if(pstJobPara_Data2->ucActive==CON_JOB_ACTIVE)
        {
			stDataEndTime = GetCurrentTime();
            if(pstJobPara_Data2->ucDUIDNum>=CONDATAUNITNUM)
                ucDUIDNum=CONDATAUNITNUM;
            else
                ucDUIDNum=pstJobPara_Data2->ucDUIDNum;
            for(i=0;i<ucDUIDNum;i++)
            {
				ucRet=Job_GetData_Data2(pstJobPara_Data2->stDUID[i],pstDataItem->ucDataContent,&usPos,&stDataBeginTime,&stDataEndTime,pstJobPara_Data2->ucResendDataInter);			
                if(ucRet==RET_ERROR)
                {
                    usPos-=4;
                    break;
                }
            }
        }else
            ucRet=RET_ERROR;
    }
    pstDataItem->usDataLen=usPos;
    return ucRet;
}

//��������
//�����һ������ʼʱ��,15�֡�30�֡�45�֡�0��
STDATETIME GetLastOneHourStartTime(unsigned char ucMinOffset)
{
    STDATETIME stDateTime;
    unsigned long ulTimeSecond;
    stDateTime=GetCurrentTime();
    stDateTime.ucSecond=0;
    stDateTime.ucMinute=0;
    ulTimeSecond=TimeConver(&stDateTime,CON_TIMEUNIT_SEC);
    ulTimeSecond=ulTimeSecond-3600+ucMinOffset*60;
    stDateTime=TimeConverToDataTime(ulTimeSecond);
    return stDateTime;
}

////�����һ������ʼʱ��,15�֡�30�֡�45�֡�0��
////�ټ�ȥһ���������Ҫ���ڵ������ļ���
//STDATETIME GetLastOneHourStartTime_Diff(unsigned char ucMinOffset)
//{
//    STDATETIME stDateTime;
//    stDateTime=GetCurrentTime();
//    stDateTime.ucSecond=0;
//    stDateTime.ucMinute=0;
//    SubTime(&stDateTime,3600,CON_TIMEUNIT_SEC);
//    return stDateTime;
//}

unsigned char GetFreeseMByMPNo(unsigned char ucMPNo)
{
    unsigned char ucFreeseM=1;

    return ucFreeseM;
}

unsigned char GetFreeseMByGroupNo(unsigned char ucMPNo)
{
    unsigned char ucFreeseM=1;

    return ucFreeseM;
}

void Adjust_Curve_Td_Starttime(unsigned char *pucBuff,unsigned char ucM)
{
    unsigned char ucMin,m,ulTimeInter;
    STDATETIME      stStartTime;
    ucMin=(unsigned char)BCD_TO_HEX_1(pucBuff);
    m=0;
    stStartTime.ucSecond=0;
    stStartTime.ucMinute=(unsigned char)BCD_TO_HEX_1(&(pucBuff[m++]));
    stStartTime.ucHour=(unsigned char)BCD_TO_HEX_1(&(pucBuff[m++]));
    stStartTime.ucDay=(unsigned char)BCD_TO_HEX_1(&(pucBuff[m++]));
    stStartTime.ucMonth=(unsigned char)BCD_TO_HEX_1(&(pucBuff[m++]));
    stStartTime.ucYear=(unsigned char)BCD_TO_HEX_1(&(pucBuff[m++]));
    switch(ucM)
    {
    case 1:
        ulTimeInter =15;
        break;
    case 2:
        ulTimeInter =30;
        break;
    case 4:
        ulTimeInter =60;
        break;
    default:
        ulTimeInter =15;
        break;
    }
    if(ucMin%ulTimeInter!=0)
    {
        ucMin=ucMin/ulTimeInter;
        ucMin=ucMin*ulTimeInter;
        m=0;
        stStartTime.ucMinute = ucMin;
        AddTime(&stStartTime,ulTimeInter,CON_TIMEUNIT_MIN);
        pucBuff[m++]=(unsigned char)HEX_TO_BCD(stStartTime.ucMinute,1);
        pucBuff[m++]=(unsigned char)HEX_TO_BCD(stStartTime.ucHour,1);
        pucBuff[m++]=(unsigned char)HEX_TO_BCD(stStartTime.ucDay,1);
        pucBuff[m++]=(unsigned char)HEX_TO_BCD(stStartTime.ucMonth,1);
        pucBuff[m++]=(unsigned char)HEX_TO_BCD(stStartTime.ucYear,1);
    }
}

//================  ReqData_2 ======================

//F1    �ն���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
unsigned char Main_Recv_ReqData_2_F1(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F1(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=9+85;
    return ucRet;
}

//F2    �ն��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
unsigned char Main_Recv_ReqData_2_F2(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F2(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=9+85;
    return ucRet;
}

//F3    �ն���������/�޹��������������ʱ�䣨�ܡ�����1~M��
unsigned char Main_Recv_ReqData_2_F3(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F3(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=9+70;
    return ucRet;
}

//F4    �ն��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��
unsigned char Main_Recv_ReqData_2_F4(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F4(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=9+70;
    return ucRet;
}

//F5    �ն��������й����������ܡ�����1~M��
unsigned char Main_Recv_ReqData_2_F5(STDATAITEM *pstDataItem,unsigned char ucDAp)
{//��һ�����һ��
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F5(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=4+20;
    return ucRet;
}

//F6    �ն��������޹����������ܡ�����1~M��
unsigned char Main_Recv_ReqData_2_F6(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F6(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=4+20;
    return ucRet;
}

//F7    �ն��ᷴ���й����������ܡ�����1~M��
unsigned char Main_Recv_ReqData_2_F7(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F7(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=4+20;
    return ucRet;
}

//F8    �ն��ᷴ���޹����������ܡ�����1~M��
unsigned char Main_Recv_ReqData_2_F8(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F8(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=4+20;
    return ucRet;
}

//F9    �����ն���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
unsigned char Main_Recv_ReqData_2_F9(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    //����������ͣ����ݵ�ǰʱ��У�������������ʱ��
    Job_Adjust_MonthUp_FreeseDayData(&stTempTime,ucDAp);        //����2����������Ϊ�������ͳ����ն������ݲ���Ҫ����
    ucRet=Main_Get_ReqData_2_F9(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=9+85;
    return ucRet;
}

//F10   �����ն��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
unsigned char Main_Recv_ReqData_2_F10(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    //����������ͣ����ݵ�ǰʱ��У�������������ʱ��
    Job_Adjust_MonthUp_FreeseDayData(&stTempTime,ucDAp);        //����2����������Ϊ�������ͳ����ն������ݲ���Ҫ����
    ucRet=Main_Get_ReqData_2_F10(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=9+85;
    return ucRet;
}

//F11   �����ն���������/�޹��������������ʱ�䣨�ܡ�����1~M��
unsigned char Main_Recv_ReqData_2_F11(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    //����������ͣ����ݵ�ǰʱ��У�������������ʱ��
    Job_Adjust_MonthUp_FreeseDayData(&stTempTime,ucDAp);        //����2����������Ϊ�������ͳ����ն������ݲ���Ҫ����
    ucRet=Main_Get_ReqData_2_F11(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=9+70;
    return ucRet;
}

//F12   �����ն��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��
unsigned char Main_Recv_ReqData_2_F12(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    //����������ͣ����ݵ�ǰʱ��У�������������ʱ��
    Job_Adjust_MonthUp_FreeseDayData(&stTempTime,ucDAp);        //����2����������Ϊ�������ͳ����ն������ݲ���Ҫ����
    ucRet=Main_Get_ReqData_2_F12(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=9+70;
    return ucRet;
}

//F17   �¶���������/�޹�����ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1~M��
unsigned char Main_Recv_ReqData_2_F17(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F17(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=8+85;
    return ucRet;
}

//F18   �¶��ᷴ����/�޹�����ʾֵ����/�������޹�����ʾֵ���ܡ�����1~M��
unsigned char Main_Recv_ReqData_2_F18(STDATAITEM *pstDataItem,unsigned char ucDAp)
{   unsigned char ucRet=RET_SUCCESS;
STDATETIME stTempTime;
unsigned short m=0;
GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
ucRet=Main_Get_ReqData_2_F18(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
pstDataItem->usDataLen=8+85;
return ucRet;
}

//F19   �¶���������/�޹��������������ʱ�䣨�ܡ�����1~M��
unsigned char Main_Recv_ReqData_2_F19(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F19(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=8+70;
    return ucRet;
}

//F20   �¶��ᷴ����/�޹��������������ʱ�䣨�ܡ�����1~M��
unsigned char Main_Recv_ReqData_2_F20(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F20(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=8+70;
    return ucRet;
}

//F21   �¶��������й����������ܡ�����1~M��
unsigned char Main_Recv_ReqData_2_F21(STDATAITEM *pstDataItem,unsigned char ucDAp)
{//�����·�֮��
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F21(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=3+20;
    return ucRet;
}

//F22   �¶��������޹����������ܡ�����1~M��
unsigned char Main_Recv_ReqData_2_F22(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    stTempTime.ucSecond=59;
    GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F22(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=3+20;
    return ucRet;
}

//F23   �¶��ᷴ���й����������ܡ�����1~M��
unsigned char Main_Recv_ReqData_2_F23(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F23(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=3+20;
    return ucRet;
}

//F24   �¶��ᷴ���޹����������ܡ�����1~M��
unsigned char Main_Recv_ReqData_2_F24(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F24(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=3+20;
    return ucRet;
}

//F25   �շ�������й����ʼ�����ʱ�䡢�й�����Ϊ��ʱ��
unsigned char Main_Recv_ReqData_2_F25(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F25(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=3+32;
    return ucRet;
}

//F26   ���ܼ������������������ʱ��
unsigned char Main_Recv_ReqData_2_F26(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F26(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=3+24;
    return ucRet;
}

//F27   �յ�ѹͳ������
unsigned char Main_Recv_ReqData_2_F27(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F27(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=3+66;
    return ucRet;
}

//F28   �ղ�ƽ���Խ���ۼ�ʱ��
unsigned char Main_Recv_ReqData_2_F28(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F28(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=3+14;        //changed for test 09-11-25
    return ucRet;
}

//F29   �յ���Խ��ͳ��
unsigned char Main_Recv_ReqData_2_F29(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F29(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=3+38;        //changed for test 09-11-25
    return ucRet;
}

//F30   �����ڹ���Խ���ۼ�ʱ��
unsigned char Main_Recv_ReqData_2_F30(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F30(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=3+4;
    return ucRet;
}

//F31   �յ��ܱ��������
unsigned char Main_Recv_ReqData_2_F31(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F31(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=3+57;
    return ucRet;
}

//F32   ����
unsigned char Main_Recv_ReqData_2_F32(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=RET_ERROR;
    return ucRet;
}

//F33   �·�������й����ʼ�����ʱ�䡢�����й�����Ϊ��ʱ��
unsigned char Main_Recv_ReqData_2_F33(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F33(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=2+32;
    return ucRet;
}

//F34   �·����й��������������ʱ�䡢���й��������������ʱ��
unsigned char Main_Recv_ReqData_2_F34(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F34(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=2+24;
    return ucRet;
}

//F35   �µ�ѹͳ������
unsigned char Main_Recv_ReqData_2_F35(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F35(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=2+66;
    return ucRet;
}

//F36   �²�ƽ���Խ���ۼ�ʱ��
unsigned char Main_Recv_ReqData_2_F36(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F36(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=2+16;        //changed for test 09-11-25
    return ucRet;
}

//F37   �µ���Խ��ͳ��
unsigned char Main_Recv_ReqData_2_F37(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F37(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=2+38;        //changed for test 09-11-25
    return ucRet;
}

//F38   �����ڹ���Խ���ۼ�ʱ��
unsigned char Main_Recv_ReqData_2_F38(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F38(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=2+4;
    return ucRet;
}

//F41   ������Ͷ���ۼ�ʱ��ʹ���
//unsigned char Main_Recv_ReqData_2_F41(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  ucRet=RET_ERROR;
//  return ucRet;
//}

//F42   �ա��µ������ۼƲ������޹�������
//unsigned char Main_Recv_ReqData_2_F42(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  ucRet=RET_ERROR;
//  return ucRet;
//}

//F43   �չ������������ۼ�ʱ��
unsigned char Main_Recv_ReqData_2_F43(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F43(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=3+6;
    return ucRet;
}

//F44   �¹������������ۼ�ʱ��
unsigned char Main_Recv_ReqData_2_F44(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F44(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=2+6;
    return ucRet;
}

//F49   �ն��չ���ʱ�䡢�ո�λ�ۼƴ���
unsigned char Main_Recv_ReqData_2_F49(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F49(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=3+4;
    return ucRet;
}

//F50   �ն��տ���ͳ������
//unsigned char Main_Recv_ReqData_2_F50(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//    STDATETIME stTempTime;
//    unsigned short m=0;
//    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
//    ucRet=Main_Get_ReqData_2_F50(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
//    pstDataItem->usDataLen=3+4;
//    return ucRet;
//}

//F51   �ն��¹���ʱ�䡢�¸�λ�ۼƴ���
unsigned char Main_Recv_ReqData_2_F51(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F51(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=2+4;
    return ucRet;
}

//F52   �ն��¿���ͳ������
//unsigned char Main_Recv_ReqData_2_F52(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//    STDATETIME stTempTime;
//    unsigned short m=0;
//    GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
//    ucRet=Main_Get_ReqData_2_F52(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
//    pstDataItem->usDataLen=2+4;
//    return ucRet;
//}

//F53   �ն�����վ��ͨ������
unsigned char Main_Recv_ReqData_2_F53(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F53(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=3+4;
    return ucRet;
}
//F54   �ն�����վ��ͨ������
unsigned char Main_Recv_ReqData_2_F54(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet=Main_Get_ReqData_2_F54(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
    pstDataItem->usDataLen=2+4;
    return ucRet;
}

//F57   �ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��
//unsigned char Main_Recv_ReqData_2_F57(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  STDATETIME stTempTime;
//  unsigned short m=0;
//  GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
//  ucRet=Main_Get_ReqData_2_F57(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
//  pstDataItem->usDataLen=3+12;
//  return ucRet;
//}
//
////F58 �ܼ������ۼ��й����������ܡ�����1~M��
//unsigned char Main_Recv_ReqData_2_F58(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  STDATETIME stTempTime;
//  unsigned short m=0;
//  GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
//  ucRet=Main_Get_ReqData_2_F58(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
//  pstDataItem->usDataLen=4+20;
//  return ucRet;
//}

//F59   �ܼ������ۼ��޹����������ܡ�����1~M��
//unsigned char Main_Recv_ReqData_2_F59(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  STDATETIME stTempTime;
//  unsigned short m=0;
//  GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
//  ucRet=Main_Get_ReqData_2_F59(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
//  pstDataItem->usDataLen=4+20;
//  return ucRet;
//}

//F60   �ܼ����������С�й����ʼ��䷢��ʱ�䣬�й�����Ϊ�����ۼ�ʱ��
//unsigned char Main_Recv_ReqData_2_F60(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  STDATETIME stTempTime;
//  unsigned short m=0;
//  GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
//  ucRet=Main_Get_ReqData_2_F60(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
//  pstDataItem->usDataLen=2+12;
//  return ucRet;
//}

//F61   �ܼ������ۼ��й����������ܡ�����1~M��
//unsigned char Main_Recv_ReqData_2_F61(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  STDATETIME stTempTime;
//  unsigned short m=0;
//  GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
//  ucRet=Main_Get_ReqData_2_F61(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
//  pstDataItem->usDataLen=3+20;
//  return ucRet;
//}

//F62   �ܼ������ۼ��޹����������ܡ�����1~M��
//unsigned char Main_Recv_ReqData_2_F62(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  STDATETIME stTempTime;
//  unsigned short m=0;
//  GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
//  ucRet=Main_Get_ReqData_2_F62(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
//  pstDataItem->usDataLen=3+20;
//  return ucRet;
//}

//F65   �ܼ��鳬���ʶ�ֵ�����ۼ�ʱ�䡢���ۼƵ�����
//unsigned char Main_Recv_ReqData_2_F65(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  STDATETIME stTempTime;
//  unsigned short m=0;
//  GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
//  ucRet=Main_Get_ReqData_2_F65(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
//  pstDataItem->usDataLen=2+6;
//  return ucRet;
//}

//F66   �ܼ��鳬�µ�������ֵ�����ۼ�ʱ�䡢�ۼƵ�����
//unsigned char Main_Recv_ReqData_2_F66(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  STDATETIME stTempTime;
//  unsigned short m=0;
//  GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
//  ucRet=Main_Get_ReqData_2_F66(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
//  pstDataItem->usDataLen=2+6;
//  return ucRet;
//}

//F73   �ܼ����й���������
//unsigned char Main_Recv_ReqData_2_F73(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned short m=0;
//  unsigned char i;
//  unsigned char ucPointNum;           //����
//  unsigned char ucM=0;                //�ܶ�
//  unsigned char ucDataLen=0;
//  STDATAITEM *pstDataItemTemp;
//  STDATETIME stStartTime;
//  unsigned char *pucBuffer;
//  unsigned char ucGroupNo,ucTaskNo;
//  unsigned char ucTmp[4];
//  unsigned short usDT2=0;
//  unsigned short usDI;
//  float fVal=0;
//
//  ucGroupNo = ucDAp;
//  if(ucGroupNo>=CON_SUMGROUP_NUM)
//      return RET_ERROR;
//  usDI=GetInstantPowDI_byGroupNo(ucGroupNo);
//  ucTaskNo=CON_GRTASK_SUMGROUP_INSTANTPOW_TASKNO;
//
//  pucBuffer=(unsigned char *)CTaskWorkBuffer;
//  pstDataItemTemp=GetDataItemTemp();
//  ClearDataItem(pstDataItemTemp);
//  memcpy((unsigned char *)pstDataItemTemp,(unsigned char *)pstDataItem,sizeof(STDATAITEM));
//  //һ����pstDataItemTemp����в�ѯ����
//  //���ؽ����pstDataItem
//
//  m=0;
//  stStartTime.ucSecond=0;
//  stStartTime.ucMinute=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
//  stStartTime.ucHour=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
//  stStartTime.ucDay=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
//  stStartTime.ucMonth=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
//  stStartTime.ucYear=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
//  if(Is_DateTime_Availble(&stStartTime)==RET_ERROR)
//      return RET_ERROR;
//  ucM=pstDataItemTemp->ucDataContent[m++];
//  if((ucM>4) || (ucM==0))
//      return RET_ERROR;
//  ucPointNum=pstDataItemTemp->ucDataContent[m++];
//  ucM=1<<(ucM-1);                         //�ܶȴ���ת��Ϊ�ܶ���ֵ
//  Adjust_Curve_Td_Starttime(pstDataItemTemp->ucDataContent,ucM);
//  memcpy(pstDataItem->ucDataContent,pstDataItemTemp->ucDataContent,7);        //����ʱ��Td  7�ֽ�
//  m=7;
//  memset(pstDataItem->ucDataContent+m,0xEE,2);        //���ݸ�ʽ02Ϊ2�ֽ�
//  pstDataItem->ucDataContent[6]=1;
//  pstDataItem->usDataLen=m+2;
//  ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stStartTime,ucPointNum,ucM,&ucDataLen,usDI);    //��Ч������Ч���ݲ���
//  ucPointNum=pucBuffer[0];
//  if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
//  {
//      pucBuffer+=2;
//      for(i=0;i<ucPointNum;i++)
//      {
//          if((m+4)>448)                   //���������
//              break;
//          memset(ucTmp,0,4);
//          memcpy(ucTmp,pucBuffer,ucDataLen);
//          pucBuffer+=ucDataLen;
//          if(ucTmp[0]!=CON_GRTASK_DATA_INVALID)
//          {
//              memcpy(&fVal,ucTmp,4);
//              fVal/=10000;                    //�й�0.0001kW --�� kW
//              usDT2=Float_To_DT2(fVal);
//              pstDataItem->ucDataContent[m]=(unsigned char)usDT2;
//              pstDataItem->ucDataContent[m+1]=(unsigned char)(usDT2>>8);
//          }else
//              memset(pstDataItem->ucDataContent+m,0xEE,2);
//          m+=2;
//      }
//      pstDataItem->ucDataContent[6]=i;
//      pstDataItem->usDataLen=m;
//  }
//    return ucRet;
//}
//
////F74   �ܼ����޹���������
//unsigned char Main_Recv_ReqData_2_F74(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned short m=0;
//  unsigned char i;
//  unsigned char ucPointNum;           //����
//  unsigned char ucM=0;                //�ܶ�
//  unsigned char ucDataLen=0;
//  STDATAITEM *pstDataItemTemp;
//  STDATETIME stStartTime;
//  unsigned char *pucBuffer;
//  unsigned char ucGroupNo,ucTaskNo;
//  unsigned char ucTmp[4];
//  unsigned short usDT2=0;
//  float fVal=0;
//  unsigned short usDI;
//
//  ucGroupNo = ucDAp;
//  if(ucGroupNo>=CON_SUMGROUP_NUM)
//      return RET_ERROR;
//  usDI=GetInstantVarDI_byGroupNo(ucGroupNo);
//  ucTaskNo=CON_GRTASK_SUMGROUP_INSTANTVAR_TASKNO;
//
//  pucBuffer=(unsigned char *)CTaskWorkBuffer;
//  pstDataItemTemp=GetDataItemTemp();
//  ClearDataItem(pstDataItemTemp);
//  memcpy((unsigned char *)pstDataItemTemp,(unsigned char *)pstDataItem,sizeof(STDATAITEM));
//  //һ����pstDataItemTemp����в�ѯ����
//  //���ؽ����pstDataItem
//
//  m=0;
//  stStartTime.ucSecond=0;
//  stStartTime.ucMinute=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
//  stStartTime.ucHour=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
//  stStartTime.ucDay=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
//  stStartTime.ucMonth=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
//  stStartTime.ucYear=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
//  if(Is_DateTime_Availble(&stStartTime)==RET_ERROR)
//      return RET_ERROR;
//  ucM=pstDataItemTemp->ucDataContent[m++];
//  if((ucM>4) || (ucM==0))
//      return RET_ERROR;
//  ucPointNum=pstDataItemTemp->ucDataContent[m++];
//  ucM=1<<(ucM-1);                         //�ܶȴ���ת��Ϊ�ܶ���ֵ
//  Adjust_Curve_Td_Starttime(pstDataItemTemp->ucDataContent,ucM);
//  memcpy(pstDataItem->ucDataContent,pstDataItemTemp->ucDataContent,7);        //����ʱ��Td  7�ֽ�
//  m=7;
//  memset(pstDataItem->ucDataContent+m,0xEE,2);        //���ݸ�ʽ02Ϊ2�ֽ�
//  pstDataItem->ucDataContent[6]=1;
//  pstDataItem->usDataLen=m+2;
//  ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stStartTime,ucPointNum,ucM,&ucDataLen,usDI);    //��Ч������Ч���ݲ���
//  ucPointNum=pucBuffer[0];
//  if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
//  {
//      pucBuffer+=2;
//      for(i=0;i<ucPointNum;i++)
//      {
//          if((m+4)>448)                   //���������
//              break;
//          memset(ucTmp,0,4);
//          memcpy(ucTmp,pucBuffer,ucDataLen);
//          pucBuffer+=ucDataLen;
//          if(ucTmp[0]!=CON_GRTASK_DATA_INVALID)
//          {
//              memcpy(&fVal,ucTmp,4);
//              fVal/=100;                      //�޹�0.01kvar --�� kvar
//              usDT2=Float_To_DT2(fVal);
//              pstDataItem->ucDataContent[m]=(unsigned char)usDT2;
//              pstDataItem->ucDataContent[m+1]=(unsigned char)(usDT2>>8);
//          }else
//              memset(pstDataItem->ucDataContent+m,0xEE,2);
//          m+=2;
//      }
//      pstDataItem->ucDataContent[6]=i;
//      pstDataItem->usDataLen=m;
//  }
//    return ucRet;
//}
//
////F75   �ܼ����й�����������
//unsigned char Main_Recv_ReqData_2_F75(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned char ucGroupNo;
//  unsigned short usDI;
//
//  ucGroupNo = ucDAp;
//  if(ucGroupNo>=CON_SUMGROUP_NUM)
//      return RET_ERROR;
//  usDI=GetPowDI_byGroupNo(ucGroupNo);
//  ucRet=Main_Recv_ReqData_2_SumGroup_Pow(pstDataItem,ucGroupNo,usDI,CON_STATUS_YES);
//    return ucRet;
//}
//
////F76   �ܼ����޹�����������
//unsigned char Main_Recv_ReqData_2_F76(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned char ucGroupNo;
//  unsigned short usDI;
//  ucGroupNo = ucDAp;
//  if(ucGroupNo>=CON_SUMGROUP_NUM)
//      return RET_ERROR;
//  usDI=GetVarDI_byGroupNo(ucGroupNo);
//  ucRet=Main_Recv_ReqData_2_SumGroup_Pow(pstDataItem,ucGroupNo,usDI,CON_STATUS_NO);
//    return ucRet;
//}

//F81   �й���������
unsigned char Main_Recv_ReqData_2_F81(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_InstantPow(pstDataItem,ucDAp,CON_DI_TYPE_MPINSTANTPOW);
    return ucRet;
}

//F82   A���й���������
unsigned char Main_Recv_ReqData_2_F82(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_InstantPow(pstDataItem,ucDAp,CON_DI_TYPE_MPINSTANTPOWA);
    return ucRet;
}

//F83   B���й���������
unsigned char Main_Recv_ReqData_2_F83(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_InstantPow(pstDataItem,ucDAp,CON_DI_TYPE_MPINSTANTPOWB);
    return ucRet;
}

//F84   C���й���������
unsigned char Main_Recv_ReqData_2_F84(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_InstantPow(pstDataItem,ucDAp,CON_DI_TYPE_MPINSTANTPOWC);
    return ucRet;
}

//F85   �޹���������
unsigned char Main_Recv_ReqData_2_F85(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_InstantPow(pstDataItem,ucDAp,CON_DI_TYPE_MPINSTANTVAR);
    return ucRet;
}

//F86   A���޹���������
unsigned char Main_Recv_ReqData_2_F86(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_InstantPow(pstDataItem,ucDAp,CON_DI_TYPE_MPINSTANTVARA);
    return ucRet;
}

//F87   B���޹���������
unsigned char Main_Recv_ReqData_2_F87(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_InstantPow(pstDataItem,ucDAp,CON_DI_TYPE_MPINSTANTVARB);
    return ucRet;
}

//F88   C���޹���������
unsigned char Main_Recv_ReqData_2_F88(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_InstantPow(pstDataItem,ucDAp,CON_DI_TYPE_MPINSTANTVARC);
    return ucRet;
}

//F89   A���ѹ����
unsigned char Main_Recv_ReqData_2_F89(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_Volt(pstDataItem,ucDAp,CON_DI_TYPE_MPVOLA);
    return ucRet;
}

//F90   B���ѹ����
unsigned char Main_Recv_ReqData_2_F90(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_Volt(pstDataItem,ucDAp,CON_DI_TYPE_MPVOLB);
    return ucRet;
}

//F91   C���ѹ����
unsigned char Main_Recv_ReqData_2_F91(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_Volt(pstDataItem,ucDAp,CON_DI_TYPE_MPVOLC);
    return ucRet;
}

//F92   A���������
unsigned char Main_Recv_ReqData_2_F92(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_Curr(pstDataItem,ucDAp,CON_DI_TYPE_MPCURA);
    return ucRet;
}

//F93   B���������
unsigned char Main_Recv_ReqData_2_F93(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_Curr(pstDataItem,ucDAp,CON_DI_TYPE_MPCURB);
    return ucRet;
}

//F94   C���������
unsigned char Main_Recv_ReqData_2_F94(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_Curr(pstDataItem,ucDAp,CON_DI_TYPE_MPCURC);
    return ucRet;
}

//F95   �����������
unsigned char Main_Recv_ReqData_2_F95(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=RET_ERROR;
    return ucRet;
}

//F97   �����й��ܵ�����
unsigned char Main_Recv_ReqData_2_F97(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_PowVal(pstDataItem,ucDAp,CON_DI_TYPE_MPDIRPOWTOTAL);
    return ucRet;
}

//F98   �����޹��ܵ�����
unsigned char Main_Recv_ReqData_2_F98(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_PowVal(pstDataItem,ucDAp,CON_DI_TYPE_MPDIRVARTOTAL);
    return ucRet;
}

//F99   �����й��ܵ�����
unsigned char Main_Recv_ReqData_2_F99(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_PowVal(pstDataItem,ucDAp,CON_DI_TYPE_MPUNDIRPOWTOTAL);
    return ucRet;
}

//F100  �����޹��ܵ�����
unsigned char Main_Recv_ReqData_2_F100(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_PowVal(pstDataItem,ucDAp,CON_DI_TYPE_MPUNDIRVARTOTAL);
    return ucRet;
}

//F101  �����й��ܵ���ʾֵ
unsigned char Main_Recv_ReqData_2_F101(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_PowViewData(pstDataItem,ucDAp,CON_DI_TYPE_MPDIRPOWTOTAL);
    return ucRet;
}

//F102  �����޹��ܵ���ʾֵ
unsigned char Main_Recv_ReqData_2_F102(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_PowViewData(pstDataItem,ucDAp,CON_DI_TYPE_MPDIRVARTOTAL);
    return ucRet;
}

//F103  �����й��ܵ���ʾֵ
unsigned char Main_Recv_ReqData_2_F103(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_PowViewData(pstDataItem,ucDAp,CON_DI_TYPE_MPUNDIRPOWTOTAL);
    return ucRet;
}

//F104  �����޹��ܵ���ʾֵ
unsigned char Main_Recv_ReqData_2_F104(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_PowViewData(pstDataItem,ucDAp,CON_DI_TYPE_MPUNDIRVARTOTAL);
    return ucRet;
}

//F105  �ܹ�������
unsigned char Main_Recv_ReqData_2_F105(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_PowRate(pstDataItem,ucDAp,CON_DI_TYPE_MPPOWRATE);
    return ucRet;
}

//F106  A�๦������
unsigned char Main_Recv_ReqData_2_F106(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_PowRate(pstDataItem,ucDAp,CON_DI_TYPE_MPPOWRATEA);
    return ucRet;
}

//F107  B�๦������
unsigned char Main_Recv_ReqData_2_F107(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_PowRate(pstDataItem,ucDAp,CON_DI_TYPE_MPPOWRATEB);
    return ucRet;
}

//F108  C�๦������
unsigned char Main_Recv_ReqData_2_F108(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_PowRate(pstDataItem,ucDAp,CON_DI_TYPE_MPPOWRATEC);
    return ucRet;
}

//F109  ��ѹ���
unsigned char Main_Recv_ReqData_2_F109(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=RET_ERROR;
    return ucRet;
}

//F110  �������
unsigned char Main_Recv_ReqData_2_F110(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=RET_ERROR;
    return ucRet;
}

////F113    A��2~19��г�����������ֵ������ʱ��
//unsigned char Main_Recv_ReqData_2_F113(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  ucRet=RET_ERROR;
//  return ucRet;
//}
//
////F114    B��2~19��г�����������ֵ������ʱ��
//unsigned char Main_Recv_ReqData_2_F114(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  ucRet=RET_ERROR;
//  return ucRet;
//}
//
////F115    C��2~19��г�����������ֵ������ʱ��
//unsigned char Main_Recv_ReqData_2_F115(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  ucRet=RET_ERROR;
//  return ucRet;
//}
//
////F116    A��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
//unsigned char Main_Recv_ReqData_2_F116(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  ucRet=RET_ERROR;
//  return ucRet;
//}
//
////F117    B��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
//unsigned char Main_Recv_ReqData_2_F117(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  ucRet=RET_ERROR;
//  return ucRet;
//}
//
////F118    C��2~19��г����ѹ�����ʼ��ܻ����������ֵ������ʱ��
//unsigned char Main_Recv_ReqData_2_F118(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  ucRet=RET_ERROR;
//  return ucRet;
//}
//
////F121    A��г��Խ����ͳ������
//unsigned char Main_Recv_ReqData_2_F121(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  ucRet=RET_ERROR;
//  return ucRet;
//}
//
////F122    B��г��Խ����ͳ������
//unsigned char Main_Recv_ReqData_2_F122(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  ucRet=RET_ERROR;
//  return ucRet;
//}
//
////F123    C��г��Խ����ͳ������
//unsigned char Main_Recv_ReqData_2_F123(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  ucRet=RET_ERROR;
//  return ucRet;
//}
//
////F129    ֱ��ģ����Խ�����ۼ�ʱ�䡢���/��Сֵ������ʱ��
//unsigned char Main_Recv_ReqData_2_F129(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  ucRet=RET_ERROR;
//  return ucRet;
//}
//
////F130    ֱ��ģ����Խ�����ۼ�ʱ�䡢���/��Сֵ������ʱ��
//unsigned char Main_Recv_ReqData_2_F130(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  ucRet=RET_ERROR;
//  return ucRet;
//}
//
////F138    ֱ��ģ������������
//unsigned char Main_Recv_ReqData_2_F138(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  ucRet=RET_ERROR;
//  return ucRet;
//}

//F145  һ�����޹��ܵ���ʾֵ����
unsigned char Main_Recv_ReqData_2_F145(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_PowViewData(pstDataItem,ucDAp,CON_DI_TYPE_MPVAR1);
    return ucRet;
}

//F146  �������޹��ܵ���ʾֵ����
unsigned char Main_Recv_ReqData_2_F146(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_PowViewData(pstDataItem,ucDAp,CON_DI_TYPE_MPVAR4);
    return ucRet;
}

//F147  �������޹��ܵ���ʾֵ����
unsigned char Main_Recv_ReqData_2_F147(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_PowViewData(pstDataItem,ucDAp,CON_DI_TYPE_MPVAR2);
    return ucRet;
}

//F148  �������޹��ܵ���ʾֵ����
unsigned char Main_Recv_ReqData_2_F148(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_PowViewData(pstDataItem,ucDAp,CON_DI_TYPE_MPVAR3);
    return ucRet;
}

unsigned char Main_Recv_ReqData_2_F153(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet = RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m =0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet = Main_Get_ReqData_2_PhasePowData(ucDAp,pstDataItem->ucDataContent,&m, CON_DI_TYPE_MPDIRPOWA,&stTempTime);
    pstDataItem->usDataLen = m;
    return ucRet;
}
unsigned char Main_Recv_ReqData_2_F154(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet = RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m =0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    ucRet = Main_Get_ReqData_2_PhasePowData(ucDAp,pstDataItem->ucDataContent,&m,CON_DI_TYPE_MPUNDIRPOWA,&stTempTime);
    pstDataItem->usDataLen = m;
    return ucRet;
}
//azh 170831
//unsigned char Main_Recv_ReqData_2_F155(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char ucRet = RET_SUCCESS;
//    STDATETIME stTempTime;
//    unsigned short m =0;
//    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
//    ucRet = Main_Get_ReqData_2_PhasePowData(ucDAp,pstDataItem->ucDataContent,&m,CON_DI_TYPE_MPCOMVAR1A,&stTempTime);
//    pstDataItem->usDataLen = m;
//    return ucRet;
//}
//unsigned char Main_Recv_ReqData_2_F156(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char ucRet = RET_SUCCESS;
//    STDATETIME stTempTime;
//    unsigned short m =0;
//    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
//    ucRet = Main_Get_ReqData_2_PhasePowData(ucDAp,pstDataItem->ucDataContent,&m,CON_DI_TYPE_MPCOMVAR2A,&stTempTime);
//    pstDataItem->usDataLen = m;
//    return ucRet;
//}
//unsigned char Main_Recv_ReqData_2_F157(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char ucRet = RET_SUCCESS;
//    STDATETIME stTempTime;
//    unsigned short m =0;
//    GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
//    ucRet = Main_Get_ReqData_2_PhasePowMonData(ucDAp, pstDataItem->ucDataContent, &m, CON_DI_TYPE_MPDIRPOWA, &stTempTime);
//    pstDataItem->usDataLen = m;
//    return ucRet;
//}
//unsigned char Main_Recv_ReqData_2_F158(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char ucRet = RET_SUCCESS;
//    STDATETIME stTempTime;
//    unsigned short m =0;
//    GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
//    ucRet = Main_Get_ReqData_2_PhasePowMonData(ucDAp, pstDataItem->ucDataContent, &m, CON_DI_TYPE_MPUNDIRPOWA, &stTempTime);
//    pstDataItem->usDataLen = m;
//    return ucRet;
//}
//unsigned char Main_Recv_ReqData_2_F159(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char ucRet = RET_SUCCESS;
//    STDATETIME stTempTime;
//    unsigned short m =0;
//    GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
//    ucRet = Main_Get_ReqData_2_PhasePowMonData(ucDAp, pstDataItem->ucDataContent, &m, CON_DI_TYPE_MPCOMVAR1A, &stTempTime);
//    pstDataItem->usDataLen = m;
//    return ucRet;
//}
//unsigned char Main_Recv_ReqData_2_F160(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//    unsigned char ucRet = RET_SUCCESS;
//    STDATETIME stTempTime;
//    unsigned short m =0;
//    GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
//    ucRet = Main_Get_ReqData_2_PhasePowMonData(ucDAp, pstDataItem->ucDataContent, &m, CON_DI_TYPE_MPCOMVAR2A, &stTempTime);
//    pstDataItem->usDataLen = m;
//    return ucRet;
//}

//F161
unsigned char Main_Recv_ReqData_2_F161_F168(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned short usDI)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);

    switch(usDI)
    {
    case CON_DATA_REQ2_F161:
        ucRet=Main_Get_ReqData_2_F161(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=34;
        break;
    case CON_DATA_REQ2_F162:
        ucRet=Main_Get_ReqData_2_F162(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=29;
        break;
    case CON_DATA_REQ2_F163:
        ucRet=Main_Get_ReqData_2_F163(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=34;
        break;
    case CON_DATA_REQ2_F164:
        ucRet=Main_Get_ReqData_2_F164(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=29;
        break;
    case CON_DATA_REQ2_F165:
        ucRet=Main_Get_ReqData_2_F165(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=29;
        break;
    case CON_DATA_REQ2_F166:
        ucRet=Main_Get_ReqData_2_F166(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=29;
        break;
    case CON_DATA_REQ2_F167:
        ucRet=Main_Get_ReqData_2_F167(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=29;
        break;
    case CON_DATA_REQ2_F168:
        ucRet=Main_Get_ReqData_2_F168(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=29;
        break;
    }
    return ucRet;
}

//F161
unsigned char Main_Recv_ReqData_2_F169_F176(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned short usDI)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    //����������ͣ����ݵ�ǰʱ��У�������������ʱ��
    Job_Adjust_MonthUp_FreeseDayData(&stTempTime,ucDAp);        //����2����������Ϊ�������ͳ����ն������ݲ���Ҫ����
    switch(usDI)
    {
    case CON_DATA_REQ2_F169:
        ucRet=Main_Get_ReqData_2_F169(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=34;
        break;
    case CON_DATA_REQ2_F170:
        ucRet=Main_Get_ReqData_2_F170(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=29;
        break;
    case CON_DATA_REQ2_F171:
        ucRet=Main_Get_ReqData_2_F171(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=34;
        break;
    case CON_DATA_REQ2_F172:
        ucRet=Main_Get_ReqData_2_F172(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=29;
        break;
    case CON_DATA_REQ2_F173:
        ucRet=Main_Get_ReqData_2_F173(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=29;
        break;
    case CON_DATA_REQ2_F174:
        ucRet=Main_Get_ReqData_2_F174(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=29;
        break;
    case CON_DATA_REQ2_F175:
        ucRet=Main_Get_ReqData_2_F175(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=29;
        break;
    case CON_DATA_REQ2_F176:
        ucRet=Main_Get_ReqData_2_F176(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=29;
        break;
    }
    return ucRet;
}

//F161      //cfh 10-04-27F177-F184�����һ���ֽڡ��ĵ�
unsigned char Main_Recv_ReqData_2_F177_F184(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned short usDI)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
    switch(usDI)
    {
    case CON_DATA_REQ2_F177:
        ucRet=Main_Get_ReqData_2_F177(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=33;
        break;
    case CON_DATA_REQ2_F178:
        ucRet=Main_Get_ReqData_2_F178(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=28;
        break;
    case CON_DATA_REQ2_F179:
        ucRet=Main_Get_ReqData_2_F179(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=33;
        break;
    case CON_DATA_REQ2_F180:
        ucRet=Main_Get_ReqData_2_F180(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=28;
        break;
    case CON_DATA_REQ2_F181:
        ucRet=Main_Get_ReqData_2_F181(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=28;
        break;
    case CON_DATA_REQ2_F182:
        ucRet=Main_Get_ReqData_2_F182(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=28;
        break;
    case CON_DATA_REQ2_F183:
        ucRet=Main_Get_ReqData_2_F183(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=28;
        break;
    case CON_DATA_REQ2_F184:
        ucRet=Main_Get_ReqData_2_F184(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=28;
        break;
    }
    return ucRet;
}


//F161
unsigned char Main_Recv_ReqData_2_F185_F196(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned short usDI)
{
    unsigned char ucRet=RET_SUCCESS;
    STDATETIME stTempTime;
    unsigned short m=0;
    if((usDI >= CON_DATA_REQ2_F185) && (usDI <= CON_DATA_REQ2_F188))
        GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
    else if((usDI >= CON_DATA_REQ2_F189) && (usDI <= CON_DATA_REQ2_F192))
    {
        GetData2_DayTime(pstDataItem->ucDataContent,&stTempTime);
        //����������ͣ����ݵ�ǰʱ��У�������������ʱ��
        Job_Adjust_MonthUp_FreeseDayData(&stTempTime,ucDAp);        //����2����������Ϊ�������ͳ����ն������ݲ���Ҫ����
    }
    else
        GetData2_MonthTime(pstDataItem->ucDataContent,&stTempTime);
    switch(usDI)
    {
    case CON_DATA_REQ2_F185:
        ucRet=Main_Get_ReqData_2_F185(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=44;
        break;
    case CON_DATA_REQ2_F186:
        ucRet=Main_Get_ReqData_2_F186(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=44;
        break;
    case CON_DATA_REQ2_F187:
        ucRet=Main_Get_ReqData_2_F187(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=44;
        break;
    case CON_DATA_REQ2_F188:
        ucRet=Main_Get_ReqData_2_F188(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=44;
        break;
    case CON_DATA_REQ2_F189:
        ucRet=Main_Get_ReqData_2_F189(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=44;
        break;
    case CON_DATA_REQ2_F190:
        ucRet=Main_Get_ReqData_2_F190(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=44;
        break;
    case CON_DATA_REQ2_F191:
        ucRet=Main_Get_ReqData_2_F191(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=44;
        break;
    case CON_DATA_REQ2_F192:
        ucRet=Main_Get_ReqData_2_F192(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=44;
        break;
    case CON_DATA_REQ2_F193:
        ucRet=Main_Get_ReqData_2_F193(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=43;
        break;
    case CON_DATA_REQ2_F194:
        ucRet=Main_Get_ReqData_2_F194(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=43;
        break;
    case CON_DATA_REQ2_F195:
        ucRet=Main_Get_ReqData_2_F195(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=43;
        break;
    case CON_DATA_REQ2_F196:
        ucRet=Main_Get_ReqData_2_F196(ucDAp,pstDataItem->ucDataContent,&m,&stTempTime);
        pstDataItem->usDataLen=43;
        break;
    }

    return ucRet;
}
//F219  �����й��ܵ���ʾֵ
unsigned char Main_Recv_ReqData_2_F219(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    ucRet=Main_Recv_ReqData_2_GroupCurveData(pstDataItem,ucDAp,0x0000);
    return ucRet;
}
//˲ʱ�й�
unsigned char Main_Recv_ReqData_2_InstantPow(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned short usDI)
{
    unsigned char *mem_ptr=NULL,mem_type;
    unsigned char ucRet=RET_SUCCESS;
    STDATAITEM *pstDataItemTemp;
    STDATETIME stStartTime;
    unsigned char *pucBuffer;
    unsigned char ucDataLen;
    unsigned char i,ucM,ucPointNum;
    unsigned char ucMPNo,ucMeterNo,ucTaskNo=0xFF,ucMPType;//,ucPulseNo;
    unsigned short m=0;
    unsigned char ucDesBuf[4];
//  unsigned char ucPulseType;

    ucMPNo=ucDAp;
    ucMPType=RunPara_GetMPType(ucMPNo);
    if(ucMPType==CON_MPTYPE_METER)
    {
        ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
        if(ucMeterNo>=CON_METER_NUM)
            return RET_ERROR;
        ucTaskNo=ucMeterNo;
    }
//  else if(ucMPType==CON_MPTYPE_PULSE)
//  {
//      ucPulseType=RunPara_GetPulseType_ByDI(usDI);
//      if(ucPulseType!=CON_PULSEATTR_INVALID)
//      {
//          ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,ucPulseType);
//          ucTaskNo=Task_GetTaskNo_byPulseNo(ucPulseNo);
//      }
//      else
//      {
//          ucTaskNo=CON_GRTASK_NUM;            //��Ч
//      }
//  }
//  else if(ucMPType==CON_MPTYPE_TER)
//      ucTaskNo=Task_GetTaskNo_Ter();
    if(ucTaskNo>=CON_GRTASK_NUM)
        return RET_ERROR;

//  pucBuffer=(unsigned char *)CTaskWorkBuffer;
    pstDataItemTemp=GetDataItemTemp();
    ClearDataItem(pstDataItemTemp);
    memcpy((unsigned char *)pstDataItemTemp,(unsigned char *)pstDataItem,sizeof(STDATAITEM));
    //һ����pstDataItemTemp����в�ѯ����
    //���ؽ����pstDataItem

    m=0;
    stStartTime.ucSecond=0;
    stStartTime.ucMinute=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucHour=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucDay=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucMonth=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucYear=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    if(Is_DateTime_Availble(&stStartTime)==RET_ERROR)
        return RET_ERROR;
    ucM=pstDataItemTemp->ucDataContent[m++];
    if((ucM>4) || (ucM==0))
        return RET_ERROR;
    ucPointNum=pstDataItemTemp->ucDataContent[m++];
    ucM=1<<(ucM-1);                         //�ܶȴ���ת��Ϊ�ܶ���ֵ
    Adjust_Curve_Td_Starttime(pstDataItemTemp->ucDataContent,ucM);
    memcpy(pstDataItem->ucDataContent,pstDataItemTemp->ucDataContent,7);        //����ʱ��Td  7�ֽ�
    m=7;
    memset(pstDataItem->ucDataContent+m,0xEE,3);
    pstDataItem->ucDataContent[6]=1;
    pstDataItem->usDataLen=m+3;

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
    pucBuffer=mem_ptr;

    ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stStartTime,ucPointNum,ucM,&ucDataLen,usDI);    //��Ч������Ч���ݲ���
    ucPointNum=pucBuffer[0];
    if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
    {
        pucBuffer+=2;
        for(i=0;i<ucPointNum;i++)
        {
            if((m+4)>448)                   //���������
                break;
            memset(ucDesBuf,0,4);
            memcpy(ucDesBuf,pucBuffer,ucDataLen);
            pucBuffer+=ucDataLen;
            if(ucDataLen==2)
            {
                if(ucDesBuf[0]==0xEE)
                {
                    memset(pstDataItem->ucDataContent+m,0xEE,3);        //�޹�2�ֽڣ�0 ������ֽ�
                }else
                {
                    pstDataItem->ucDataContent[m]=0;
                    if(ucDesBuf[1]>0x79)
                        ucDesBuf[1]=0x79;           //���ݸ�ʽ9��Ŀǰ���������ݣ����ֵ79.9999
                    memcpy(pstDataItem->ucDataContent+m+1,ucDesBuf,2);      //�޹�2�ֽڣ�0 ������ֽ�
                }
            }else
            {
                if(ucDesBuf[0]==0xEE)
                {
                    memset(pstDataItem->ucDataContent+m,0xEE,3);
                }else
                {
                    if(ucDesBuf[2]>0x79)
                        ucDesBuf[2]=0x79;           //���ݸ�ʽ9��Ŀǰ���������ݣ����ֵ79.9999
                    memcpy(pstDataItem->ucDataContent+m,ucDesBuf,3);            //�й�3�ֽ�
                }
            }
            m+=3;
        }
        pstDataItem->ucDataContent[6]=i;
        pstDataItem->usDataLen=m;
    }
    free_mem_block(mem_ptr);//

    return ucRet;
}

//��ѹ
unsigned char Main_Recv_ReqData_2_Volt(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned short usDI)
{
    unsigned char *mem_ptr=NULL,mem_type;
    unsigned char ucRet=RET_SUCCESS;
    STDATAITEM *pstDataItemTemp;
    STDATETIME stStartTime;
    unsigned char *pucBuffer;
    unsigned char ucDataLen;
    unsigned char i,ucM,ucPointNum;
    unsigned char ucMPNo,ucMeterNo,ucTaskNo=0xFF,ucMPType;
    unsigned short m=0;
    unsigned long ulVal;

    ucMPNo=ucDAp;
    ucMPType=RunPara_GetMPType(ucMPNo);
    if(ucMPType==CON_MPTYPE_METER)
    {
        ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
        if(ucMeterNo>=CON_METER_NUM)
            return RET_ERROR;
        ucTaskNo=ucMeterNo;
    }
//  else if(ucMPType==CON_MPTYPE_TER)
//      ucTaskNo=Task_GetTaskNo_Ter();
    if(ucTaskNo>=CON_GRTASK_NUM)
        return RET_ERROR;

//  pucBuffer=(unsigned char *)CTaskWorkBuffer;
    pstDataItemTemp=GetDataItemTemp();
    ClearDataItem(pstDataItemTemp);
    memcpy((unsigned char *)pstDataItemTemp,(unsigned char *)pstDataItem,sizeof(STDATAITEM));
    //һ����pstDataItemTemp����в�ѯ����
    //���ؽ����pstDataItem

    m=0;
    stStartTime.ucSecond=0;
    stStartTime.ucMinute=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucHour=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucDay=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucMonth=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucYear=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    if(Is_DateTime_Availble(&stStartTime)==RET_ERROR)
        return RET_ERROR;
    ucM=pstDataItemTemp->ucDataContent[m++];
    if((ucM>4) || (ucM==0))
        return RET_ERROR;
    ucPointNum=pstDataItemTemp->ucDataContent[m++];
    ucM=1<<(ucM-1);                         //�ܶȴ���ת��Ϊ�ܶ���ֵ
    Adjust_Curve_Td_Starttime(pstDataItemTemp->ucDataContent,ucM);
///////////    memcpy(pstDataItem->ucDataContent,pstDataItemTemp->ucDataContent,7);        //����ʱ��Td  7�ֽ�
    m=7;
    memset(pstDataItem->ucDataContent+m,0xEE,2);
    pstDataItem->ucDataContent[6]=1;
    pstDataItem->usDataLen=m+2;

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
    pucBuffer = mem_ptr;
    ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stStartTime,ucPointNum,ucM,&ucDataLen,usDI);    //��Ч������Ч���ݲ���
    ucPointNum=pucBuffer[0];
    if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
    {
        pucBuffer+=2;
        for(i=0;i<ucPointNum;i++)
        {
            if((m+4)>448)                   //���������
                break;
            ulVal=0;
            memcpy((unsigned char *)&ulVal,pucBuffer,ucDataLen);
        //  if((unsigned char)ulVal!=0xEE)
        //      ulVal=(ulVal<<4);               //���ݸ�ʽ08ת��Ϊ���ݸ�ʽ07
            memcpy(pstDataItem->ucDataContent+m,(unsigned char *)&ulVal,2);
            pucBuffer+=ucDataLen;
            m+=2;
        }
        pstDataItem->ucDataContent[6]=i;
        pstDataItem->usDataLen=m;
    }
    free_mem_block(mem_ptr);//
    return ucRet;
}

//����
unsigned char Main_Recv_ReqData_2_Curr(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned short usDI)
{
    unsigned char *mem_ptr=NULL,mem_type;
    unsigned char ucRet=RET_SUCCESS;
    STDATAITEM *pstDataItemTemp;
    STDATETIME stStartTime;
    unsigned char *pucBuffer;
    unsigned char ucDataLen;
    unsigned char i,ucM,ucPointNum;
    unsigned char ucMPNo,ucMeterNo,ucTaskNo=0xFF,ucMPType;
    unsigned short m=0;
    unsigned long ulVal;

    ucMPNo=ucDAp;
    ucMPType=RunPara_GetMPType(ucMPNo);
    if(ucMPType==CON_MPTYPE_METER)
    {
        ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
        if(ucMeterNo>=CON_METER_NUM)
            return RET_ERROR;
        ucTaskNo=ucMeterNo;
    }
//  else if(ucMPType==CON_MPTYPE_TER)
//      ucTaskNo=Task_GetTaskNo_Ter();
    if(ucTaskNo>=CON_GRTASK_NUM)
        return RET_ERROR;

//  pucBuffer=(unsigned char *)CTaskWorkBuffer;
    pstDataItemTemp=GetDataItemTemp();
    ClearDataItem(pstDataItemTemp);
    memcpy((unsigned char *)pstDataItemTemp,(unsigned char *)pstDataItem,sizeof(STDATAITEM));
    //һ����pstDataItemTemp����в�ѯ����
    //���ؽ����pstDataItem

    m=0;
    stStartTime.ucSecond=0;
    stStartTime.ucMinute=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucHour=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucDay=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucMonth=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucYear=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    if(Is_DateTime_Availble(&stStartTime)==RET_ERROR)
        return RET_ERROR;
    ucM=pstDataItemTemp->ucDataContent[m++];
    if((ucM>4) || (ucM==0))
        return RET_ERROR;
    ucPointNum=pstDataItemTemp->ucDataContent[m++];
    ucM=1<<(ucM-1);                         //�ܶȴ���ת��Ϊ�ܶ���ֵ
    Adjust_Curve_Td_Starttime(pstDataItemTemp->ucDataContent,ucM);
    memcpy(pstDataItem->ucDataContent,pstDataItemTemp->ucDataContent,7);        //����ʱ��Td  7�ֽ�
    m=7;
    memset(pstDataItem->ucDataContent+m,0xEE,2);
    pstDataItem->ucDataContent[6]=1;
    pstDataItem->usDataLen=m+2;

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
    pucBuffer = mem_ptr;
    ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stStartTime,ucPointNum,ucM,&ucDataLen,usDI);    //��Ч������Ч���ݲ���
    ucPointNum=pucBuffer[0];
    if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
    {
        pucBuffer+=2;
        for(i=0;i<ucPointNum;i++)
        {
            if((m+4)>448)                   //���������
                break;
            ulVal=0;
            memcpy((unsigned char *)&ulVal,pucBuffer,ucDataLen);
            memcpy(pstDataItem->ucDataContent+m,(unsigned char *)&ulVal,ucDataLen/*2*/);//changed for test 09-11-25
            pucBuffer+=ucDataLen;
            m+=ucDataLen;
        }
        pstDataItem->ucDataContent[6]=i;
        pstDataItem->usDataLen=m;
    }
    free_mem_block(mem_ptr);//
    return ucRet;
}


//����
unsigned char Main_Recv_ReqData_2_PowVal(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned short usDI)
{
    unsigned char *mem_ptr=NULL,mem_type;
    unsigned char ucRet=RET_SUCCESS;
    STDATAITEM *pstDataItemTemp;
    STDATETIME stStartTime;
    unsigned char *pucBuffer;
    unsigned char ucDataLen;
    unsigned char i,ucM,ucPointNum;
    unsigned char ucMPNo,ucMeterNo,ucTaskNo=0xFF,ucMPType;//,ucPulseNo;
    unsigned short m=0;
    unsigned char ucTmp[4];
    unsigned long ulTmp,ulValBase,ulValThis;
//  unsigned char ucPulseType;

    ucMPNo=ucDAp;
    ucMPType=RunPara_GetMPType(ucMPNo);
    if(ucMPType==CON_MPTYPE_METER)
    {
        ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
        if(ucMeterNo>=CON_METER_NUM)
            return RET_ERROR;
        ucTaskNo=ucMeterNo;
    }
//  else if(ucMPType==CON_MPTYPE_PULSE)
//  {
//      ucPulseType=RunPara_GetPulseType_ByDI(usDI);
//      if(ucPulseType!=CON_PULSEATTR_INVALID)
//      {
//          ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,ucPulseType);
//          ucTaskNo=Task_GetTaskNo_byPulseNo(ucPulseNo);
//      }
//      else
//      {
//          ucTaskNo=CON_GRTASK_NUM;            //��Ч
//      }
//  }
//  else if(ucMPType==CON_MPTYPE_TER)
//      ucTaskNo=Task_GetTaskNo_Ter();
    if(ucTaskNo>=CON_GRTASK_NUM)
        return RET_ERROR;

//  pucBuffer=(unsigned char *)CTaskWorkBuffer;
    pstDataItemTemp=GetDataItemTemp();
    ClearDataItem(pstDataItemTemp);
    memcpy((unsigned char *)pstDataItemTemp,(unsigned char *)pstDataItem,sizeof(STDATAITEM));
    //һ����pstDataItemTemp����в�ѯ����
    //���ؽ����pstDataItem

    m=0;
    stStartTime.ucSecond=0;
    stStartTime.ucMinute=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucHour=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucDay=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucMonth=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucYear=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    if(Is_DateTime_Availble(&stStartTime)==RET_ERROR)
        return RET_ERROR;
    ucM=pstDataItemTemp->ucDataContent[m++];
    if((ucM>4) || (ucM==0))
        return RET_ERROR;
    ucPointNum=pstDataItemTemp->ucDataContent[m++];
    ucPointNum=ucPointNum+1;
    ucM=1<<(ucM-1);                         //�ܶȴ���ת��Ϊ�ܶ���ֵ
    SubTime(&stStartTime,(unsigned char)(15*ucM),CON_TIMEUNIT_MIN);
    Adjust_Curve_Td_Starttime(pstDataItemTemp->ucDataContent,ucM);
    memcpy(pstDataItem->ucDataContent,pstDataItemTemp->ucDataContent,7);        //����ʱ��Td  7�ֽ�
    m=7;
    memset(pstDataItem->ucDataContent+m,0xEE,4);
    pstDataItem->ucDataContent[6]=1;
    pstDataItem->usDataLen=m+4;

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
    pucBuffer = mem_ptr;
    ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stStartTime,ucPointNum,ucM,&ucDataLen,usDI);    //��Ч������Ч���ݲ���
    ucPointNum=pucBuffer[0]-1;
    if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
    {
        pucBuffer+=2;
        memset(ucTmp,0,4);
        memcpy(ucTmp,pucBuffer,ucDataLen);
        if(ucTmp[0]==CON_GRTASK_DATA_INVALID)
            ulValBase=CONINVALIDVAL;
        else
            ulValBase=BCD_TO_HEX_4(ucTmp);
        pucBuffer+=ucDataLen;
        for(i=0;i<ucPointNum;i++)
        {
            if((m+4)>448)                   //���������
                break;
            memset(ucTmp,0,4);
            memcpy(ucTmp,pucBuffer,ucDataLen);
            if(ucTmp[0]==CON_GRTASK_DATA_INVALID)
                ulValThis=CONINVALIDVAL;
            else
                ulValThis=BCD_TO_HEX_4(ucTmp);
            if(ulValThis!=CONINVALIDVAL && ulValBase!=CONINVALIDVAL && ucTmp[0]!=CON_GRTASK_DATA_INVALID)
            {
                if(ulValThis>ulValBase)
                    ulTmp=ulValThis-ulValBase;
                else
                {
                    ulValThis=ulValBase;
                    ulTmp=0;
                }
                SetMainComm_Buf_lVal_4B_DT13(pstDataItem->ucDataContent,m,ulTmp);
            }else
            {
                memset(pstDataItem->ucDataContent+m,0xEE,4);
            }
            ulValBase=ulValThis;
            m+=4;
            pucBuffer+=ucDataLen;
        }
        pstDataItem->ucDataContent[6]=i;
        pstDataItem->usDataLen=m;
    }
    free_mem_block(mem_ptr);//
    return ucRet;
}

//����ʾֵ
unsigned char Main_Recv_ReqData_2_PowViewData(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned short usDI)
{
    unsigned char *mem_ptr=NULL,mem_type;
    unsigned char ucRet=RET_SUCCESS;
    STDATAITEM *pstDataItemTemp;
    STDATETIME stStartTime;
    unsigned char *pucBuffer;
    unsigned char ucDataLen;
    unsigned char i,ucM,ucPointNum;
    unsigned char ucMPNo,ucMeterNo,ucTaskNo=0xFF,ucMPType;//,ucPulseNo;
    unsigned short m=0;
//  unsigned char ucPulseType;

    ucMPNo=ucDAp;
    ucMPType=RunPara_GetMPType(ucMPNo);
    if(ucMPType==CON_MPTYPE_METER)
    {
        ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
        if(ucMeterNo>=CON_METER_NUM)
            return RET_ERROR;
        ucTaskNo=ucMeterNo;
    }
//  else if(ucMPType==CON_MPTYPE_PULSE)
//  {
//      ucPulseType=RunPara_GetPulseType_ByDI(usDI);
//      if(ucPulseType!=CON_PULSEATTR_INVALID)
//      {
//          ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,ucPulseType);
//          ucTaskNo=Task_GetTaskNo_byPulseNo(ucPulseNo);
//      }
//      else
//      {
//          ucTaskNo=CON_GRTASK_NUM;            //��Ч
//      }
//  }
//  else if(ucMPType==CON_MPTYPE_TER)
//      ucTaskNo=Task_GetTaskNo_Ter();
    if(ucTaskNo>=CON_GRTASK_NUM)
        return RET_ERROR;

//  pucBuffer=(unsigned char *)CTaskWorkBuffer;
    pstDataItemTemp=GetDataItemTemp();
    ClearDataItem(pstDataItemTemp);
    memcpy((unsigned char *)pstDataItemTemp,(unsigned char *)pstDataItem,sizeof(STDATAITEM));
    //һ����pstDataItemTemp����в�ѯ����
    //���ؽ����pstDataItem

    m=0;
    stStartTime.ucSecond=0;
    stStartTime.ucMinute=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucHour=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucDay=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucMonth=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucYear=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    if(Is_DateTime_Availble(&stStartTime)==RET_ERROR)
        return RET_ERROR;
    ucM=pstDataItemTemp->ucDataContent[m++];
    if((ucM>4) || (ucM==0))
        return RET_ERROR;
    ucPointNum=pstDataItemTemp->ucDataContent[m++];
    ucM=1<<(ucM-1);                         //�ܶȴ���ת��Ϊ�ܶ���ֵ
    Adjust_Curve_Td_Starttime(pstDataItemTemp->ucDataContent,ucM);
    memcpy(pstDataItem->ucDataContent,pstDataItemTemp->ucDataContent,7);        //����ʱ��Td  7�ֽ�
    m=7;
    memset(pstDataItem->ucDataContent+m,0xEE,4);
    pstDataItem->ucDataContent[6]=1;
    pstDataItem->usDataLen=m+4;

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
    pucBuffer = mem_ptr;
    ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stStartTime,ucPointNum,ucM,&ucDataLen,usDI);    //��Ч������Ч���ݲ���
    ucPointNum=pucBuffer[0];
    if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
    {
        pucBuffer+=2;
        for(i=0;i<ucPointNum;i++)
        {
            if((m+4)>448)                   //���������
                break;
            memcpy(pstDataItem->ucDataContent+m,pucBuffer,ucDataLen);
            m+=4;
            pucBuffer+=ucDataLen;
        }
        pstDataItem->ucDataContent[6]=i;
        pstDataItem->usDataLen=m;
    }
    free_mem_block(mem_ptr);//
    return ucRet;
}

//��������
unsigned char Main_Recv_ReqData_2_PowRate(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned short usDI)
{
    unsigned char *mem_ptr=NULL,mem_type;
    unsigned char ucRet=RET_SUCCESS;
    STDATAITEM *pstDataItemTemp;
    STDATETIME stStartTime;
    unsigned char *pucBufferP,*pucBufferQ;
    unsigned char ucDataLenP,ucDataLenQ;
    unsigned char i,ucM,ucPointNumP,ucPointNumQ;
    unsigned char ucMPNo,ucMeterNo,ucTaskNo=0xFF,ucTaskNo2=0xFF,ucMPType;//,ucPulseNo
    unsigned short m=0;
    unsigned long ulPow,ulVar,ulRate,ulTmp;
    unsigned short usDIP,usDIQ;

    ucMPNo=ucDAp;
    ucMPType=RunPara_GetMPType(ucMPNo);
    if(ucMPType==CON_MPTYPE_METER)
    {
        ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
        if(ucMeterNo>=CON_METER_NUM)
            return RET_ERROR;
        ucTaskNo=ucMeterNo;
        ucTaskNo2=ucTaskNo;
    }
//  else if(ucMPType==CON_MPTYPE_PULSE)
//  {
//      ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRPOW);
//      ucTaskNo=Task_GetTaskNo_byPulseNo(ucPulseNo);
//      ucPulseNo=RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
//      ucTaskNo2=Task_GetTaskNo_byPulseNo(ucPulseNo);
//  }
//  else if(ucMPType==CON_MPTYPE_TER)
//  {
//      ucTaskNo=Task_GetTaskNo_Ter();
//      ucTaskNo2=ucTaskNo;
//  }
    if(ucTaskNo>=CON_GRTASK_NUM)
        return RET_ERROR;

//  pucBufferP=(unsigned char *)CTaskWorkBuffer;
//  pucBufferQ=(unsigned char *)CTaskWorkBuffer+CON_BUFFER_CLASS3_DIFF;
    pstDataItemTemp=GetDataItemTemp();
    ClearDataItem(pstDataItemTemp);
    memcpy((unsigned char *)pstDataItemTemp,(unsigned char *)pstDataItem,sizeof(STDATAITEM));
    //һ����pstDataItemTemp����в�ѯ����
    //���ؽ����pstDataItem

    m=0;
    stStartTime.ucSecond=0;
    stStartTime.ucMinute=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucHour=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucDay=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucMonth=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucYear=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    if(Is_DateTime_Availble(&stStartTime)==RET_ERROR)
        return RET_ERROR;
    ucM=pstDataItemTemp->ucDataContent[m++];
    if((ucM>4) || (ucM==0))
        return RET_ERROR;
    ucPointNumP=pstDataItemTemp->ucDataContent[m++];
    ucPointNumQ=ucPointNumP;
    ucM=1<<(ucM-1);                         //�ܶȴ���ת��Ϊ�ܶ���ֵ
    switch(usDI)
    {
    case CON_DI_TYPE_MPPOWRATE:
        usDIP=CON_DI_TYPE_MPINSTANTPOW;
        usDIQ=CON_DI_TYPE_MPINSTANTVAR;
        break;
    case CON_DI_TYPE_MPPOWRATEA:
        usDIP=CON_DI_TYPE_MPINSTANTPOWA;
        usDIQ=CON_DI_TYPE_MPINSTANTVARA;
        break;
    case CON_DI_TYPE_MPPOWRATEB:
        usDIP=CON_DI_TYPE_MPINSTANTPOWB;
        usDIQ=CON_DI_TYPE_MPINSTANTVARB;
        break;
    case CON_DI_TYPE_MPPOWRATEC:
        usDIP=CON_DI_TYPE_MPINSTANTPOWC;
        usDIQ=CON_DI_TYPE_MPINSTANTVARC;
        break;
    default:
        usDIP=CON_DI_TYPE_MPINSTANTPOW;
        usDIQ=CON_DI_TYPE_MPINSTANTVAR;
        break;
    }
    Adjust_Curve_Td_Starttime(pstDataItemTemp->ucDataContent,ucM);
    memcpy(pstDataItem->ucDataContent,pstDataItemTemp->ucDataContent,7);        //����ʱ��Td  7�ֽ�
    m=7;
    memset(pstDataItem->ucDataContent+m,0xEE,2);
    pstDataItem->ucDataContent[6]=1;
    pstDataItem->usDataLen=m+2;

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
    pucBufferP=(unsigned char *)mem_ptr;
    pucBufferQ=(unsigned char *)mem_ptr+768;
    ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBufferP,&stStartTime,ucPointNumP,ucM,&ucDataLenP,usDIP);    //��Ч������Ч���ݲ���
    ucPointNumP=pucBufferP[0];
    if(ucRet!=RET_ERROR && ucPointNumP!=0 && ucDataLenP!=0)
    {
        ucRet=Task_QueryRetVal_ByDI(ucTaskNo2,pucBufferQ,&stStartTime,ucPointNumQ,ucM,&ucDataLenQ,usDIQ);   //��Ч������Ч���ݲ���
        ucPointNumQ=pucBufferQ[0];
        if(ucRet!=RET_ERROR && ucPointNumQ==ucPointNumP && ucDataLenQ!=0)
        {
            pucBufferP+=2;
            pucBufferQ+=2;
            for(i=0;i<ucPointNumP;i++)
            {
                if((m+4)>448)                   //���������
                    break;
                ulPow=0;
                ulVar=0;
                memcpy((unsigned char *)&ulTmp,pucBufferP,ucDataLenP);
                if((unsigned char)ulTmp==CON_GRTASK_DATA_INVALID)
                    ulPow=CONINVALIDVAL;
                else
                    ulPow=BCD_TO_HEX_3((unsigned char *)&ulTmp);
                memcpy((unsigned char *)&ulTmp,pucBufferQ,ucDataLenQ);
                if((unsigned char)ulTmp==CON_GRTASK_DATA_INVALID)
                    ulVar=CONINVALIDVAL;
                else
                    ulVar=BCD_TO_HEX_2((unsigned char *)&ulTmp);
                if(ulPow!=CONINVALIDVAL && ulVar!=CONINVALIDVAL)
                {
                    ulRate=GetPQRate(ulPow,ulVar);
                    ulRate=ulRate*10;
                }else
                    ulRate=CONINVALIDVAL;
                m=SetMainComm_Buf_lVal_2B(pstDataItem->ucDataContent,m,ulRate);
                pucBufferP+=ucDataLenP;
                pucBufferQ+=ucDataLenQ;
            }
            pstDataItem->ucDataContent[6]=i;
            pstDataItem->usDataLen=m;
        }
    }
    free_mem_block(mem_ptr);//
    return ucRet;
}
//azh 170925
//���������������������
unsigned char Main_Recv_ReqData_2_GroupCurveData(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned short usDI)
{
    unsigned char *mem_ptr=NULL,mem_type;
    unsigned char ucRet=RET_SUCCESS;
    STDATAITEM *pstDataItemTemp;
    STDATETIME stStartTime;
    unsigned char *pucBuffer;
    unsigned char ucDataLen;
    unsigned char i,ucM,ucPointNum;
    unsigned char ucMPNo,ucMeterNo,ucTaskNo=0xFF,ucMPType;//,ucPulseNo;
    unsigned short m=0;
    unsigned char ucDesBuf[4];
//  unsigned char ucPulseType;

    ucMPNo=ucDAp;
    ucMPType=RunPara_GetMPType(ucMPNo);
    if(ucMPType==CON_MPTYPE_METER)
    {
        ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
        if(ucMeterNo>=CON_METER_NUM)
            return RET_ERROR;
        ucTaskNo=ucMeterNo;
    }

    if(ucTaskNo>=CON_GRTASK_NUM)
        return RET_ERROR;

//  pucBuffer=(unsigned char *)CTaskWorkBuffer;
    pstDataItemTemp=GetDataItemTemp();
    ClearDataItem(pstDataItemTemp);
    memcpy((unsigned char *)pstDataItemTemp,(unsigned char *)pstDataItem,sizeof(STDATAITEM));
    //һ����pstDataItemTemp����в�ѯ����
    //���ؽ����pstDataItem

    m=0;
    stStartTime.ucSecond=0;
    stStartTime.ucMinute=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucHour=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucDay=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucMonth=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    stStartTime.ucYear=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
    if(Is_DateTime_Availble(&stStartTime)==RET_ERROR)
        return RET_ERROR;
    ucM=pstDataItemTemp->ucDataContent[m++];
    if((ucM>4) || (ucM==0))
        return RET_ERROR;
    ucPointNum=pstDataItemTemp->ucDataContent[m++];
    ucM=1<<(ucM-1);                         //�ܶȴ���ת��Ϊ�ܶ���ֵ
    Adjust_Curve_Td_Starttime(pstDataItemTemp->ucDataContent,ucM);
    memcpy(pstDataItem->ucDataContent,pstDataItemTemp->ucDataContent,7);        //����ʱ��Td  7�ֽ�
    m=7;
    memset(pstDataItem->ucDataContent+m,0xEE,33);
    pstDataItem->ucDataContent[6]=1;
    pstDataItem->usDataLen=m+33;

    mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
    if(mem_ptr == NULL)
    {
        return RET_ERROR;
    }
    pucBuffer = mem_ptr;
    ucRet=Task_QueryRetVal_ByGroupDI(ucTaskNo,pucBuffer,&stStartTime,ucPointNum,ucM,&ucDataLen,usDI);	//��Ч������Ч���ݲ���
    ucPointNum=pucBuffer[0];
    if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
    {
        pucBuffer+=2;
        for(i=0;i<ucPointNum;i++)
        {
            if((m+33)>448)                   //���������
                break;
//�й�����
			memset(ucDesBuf,0,4);
			memcpy(ucDesBuf,pucBuffer,3);
			pucBuffer+=3;
			if(ucDesBuf[0]==0xEE)
			{
				memset(pstDataItem->ucDataContent+m,0xEE,3);
			}else
			{
				if(ucDesBuf[2]>0x79)
					ucDesBuf[2]=0x79;			//���ݸ�ʽ9��Ŀǰ���������ݣ����ֵ79.9999
				memcpy(pstDataItem->ucDataContent+m,ucDesBuf,3);			//�й�3�ֽ�
			}
			m+=3;
//�޹�����
			memset(ucDesBuf,0,4);
			memcpy(ucDesBuf,pucBuffer,2);
			pucBuffer+=2;
			if(ucDesBuf[0]==0xEE)
			{
				memset(pstDataItem->ucDataContent+m,0xEE,3);
			}else
			{
				pstDataItem->ucDataContent[m]=0;
				if(ucDesBuf[1]>0x79)
					ucDesBuf[1]=0x79;			//���ݸ�ʽ9��Ŀǰ���������ݣ����ֵ79.9999
				memcpy(pstDataItem->ucDataContent+m+1,ucDesBuf,2);		//�޹�2�ֽڣ�0 ������ֽ�
			}
			m+=3;
//ABC���ѹ
			memcpy(pstDataItem->ucDataContent+m,pucBuffer,6);
			pucBuffer+=6;
			m+=6;
//ABC�����
			memcpy(pstDataItem->ucDataContent+m,pucBuffer,9);
			pucBuffer+=9;
			m+=9;
//�����й���һ�������޹�������ʾֵ
			memcpy(pstDataItem->ucDataContent+m,pucBuffer,12);
			pucBuffer+=12;
			m+=12;
        }
        pstDataItem->ucDataContent[6]=i;
        pstDataItem->usDataLen=m;
    }
    free_mem_block(mem_ptr);//
    return ucRet;
}


//�ܼ������������
//unsigned char Main_Recv_ReqData_2_SumGroup_Pow(STDATAITEM *pstDataItem,unsigned char ucGroupNo,unsigned short usDI,unsigned char ucPowFlag)
//{//���һ��
//    unsigned char ucRet=RET_SUCCESS;
//  unsigned short m=0;
//  unsigned char i;
//  unsigned char ucPointNum;           //����
//  unsigned char ucM=0;                //�ܶ�
//  unsigned char ucDataLen=0;
//  STDATAITEM *pstDataItemTemp;
//  STDATETIME stStartTime;
//  unsigned char *pucBuffer;
//  unsigned char ucTaskNo;
//  unsigned char ucTmp[4];
//  unsigned short usDT2=0;
//  unsigned long ulTmp=0;
//  float fValBase,fValThis;
//
//  if(ucGroupNo>=CON_SUMGROUP_NUM)
//      return RET_ERROR;
//  if(ucPowFlag==CON_STATUS_YES)
//      ucTaskNo=CON_GRTASK_SUMGROUP_POW_TASKNO;
//  else
//      ucTaskNo=CON_GRTASK_SUMGROUP_VAR_TASKNO;
//
//  pucBuffer=(unsigned char *)CTaskWorkBuffer;
//  pstDataItemTemp=GetDataItemTemp();
//  ClearDataItem(pstDataItemTemp);
//  memcpy((unsigned char *)pstDataItemTemp,(unsigned char *)pstDataItem,sizeof(STDATAITEM));
//  //һ����pstDataItemTemp����в�ѯ����
//  //���ؽ����pstDataItem
//
//  m=0;
//  stStartTime.ucSecond=0;
//  stStartTime.ucMinute=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
//  stStartTime.ucHour=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
//  stStartTime.ucDay=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
//  stStartTime.ucMonth=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
//  stStartTime.ucYear=(unsigned char)BCD_TO_HEX_1(&(pstDataItemTemp->ucDataContent[m++]));
//  if(Is_DateTime_Availble(&stStartTime)==RET_ERROR)
//      return RET_ERROR;
//  ucM=pstDataItemTemp->ucDataContent[m++];
//  if((ucM>4) || (ucM==0))
//      return RET_ERROR;
//  ucPointNum=pstDataItemTemp->ucDataContent[m++];
//  ucPointNum=ucPointNum+1;                //��һ��
//  ucM=1<<(ucM-1);                         //�ܶȴ���ת��Ϊ�ܶ���ֵ
//  SubTime(&stStartTime,(unsigned char)(15*ucM),CON_TIMEUNIT_MIN);
//  Adjust_Curve_Td_Starttime(pstDataItemTemp->ucDataContent,ucM);
//  memcpy(pstDataItem->ucDataContent,pstDataItemTemp->ucDataContent,7);        //����ʱ��Td  7�ֽ�
//  m=7;
//  memset(pstDataItem->ucDataContent+m,0xEE,4);        //���ݸ�ʽ03Ϊ4�ֽ�
//  pstDataItem->ucDataContent[6]=1;
//  pstDataItem->usDataLen=m+4;
//
//  ucRet=Task_QueryRetVal_ByDI(ucTaskNo,pucBuffer,&stStartTime,ucPointNum,ucM,&ucDataLen,usDI);    //��Ч������Ч���ݲ���
//  ucPointNum=pucBuffer[0]-1;
//  if(ucRet!=RET_ERROR && ucPointNum!=0 && ucDataLen!=0)
//  {
//      pucBuffer+=2;
//      memset(ucTmp,0,4);
//      memcpy(ucTmp,pucBuffer,ucDataLen);
//      if(ucTmp[0]==CON_GRTASK_DATA_INVALID)
//          fValBase=CONINVALIDVAL;
//      else
//          memcpy(&fValBase,ucTmp,4);
//      pucBuffer+=ucDataLen;
//      for(i=0;i<ucPointNum;i++)
//      {
//          if((m+4)>448)                   //���������
//              break;
//          memset(ucTmp,0,4);
//          memcpy(ucTmp,pucBuffer,ucDataLen);
//          if(ucTmp[0]==CON_GRTASK_DATA_INVALID)
//              fValThis=CONINVALIDVAL;
//          else
//              memcpy(&fValThis,ucTmp,4);
//          if(fValThis!=CONINVALIDVAL && fValBase!=CONINVALIDVAL && ucTmp[0]!=CON_GRTASK_DATA_INVALID)
//          {
//              if(fValThis>fValBase)
//                  ulTmp=Float_To_DT3_Div100(fValThis-fValBase);
//              else
//              {
//                  fValThis=fValBase;
//                  ulTmp=0;
//              }
//              memcpy(pstDataItem->ucDataContent+m,&ulTmp,4);
//          }else
//          {
//              memset(pstDataItem->ucDataContent+m,0xEE,4);
//          }
//          fValBase=fValThis;
//          m+=4;
//          pucBuffer+=ucDataLen;
//      }
//      pstDataItem->ucDataContent[6]=i;
//      pstDataItem->usDataLen=m;
//  }
//    return ucRet;
//}

//================  ReqData_3 ======================
//��Ҫ�¼�
unsigned char Main_Recv_ReqData_3_Important(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned char *pucECLeftStart,unsigned char ucECLastEnd)
{
    unsigned char ucRet=RET_SUCCESS;
  unsigned char ucECStart,ucECEnd,ucECNo,i=0,n=0;
  unsigned short m=0,mTmp=0,usPos=0;
//  STALRCATEGORY   stAlrHappenWord;
  unsigned char ucTmpAlrERCCode,ucMaxAlrNum=0;
//  stAlrHappenWord = Alarm_Get_Happen_word();//azh �Ѿ���ȫ�ֱ���

  ucMaxAlrNum = Alarm_GetRecordMaxNum(CON_ALR_ACD);
  ucECStart=(*pucECLeftStart);                            //�����¼���¼��ʼָ��Pm
  ucECEnd=ucECLastEnd;                                    //�����¼���¼����ָ��Pn

  ucECNo=ucECStart;
  if(ucECEnd>=ucMaxAlrNum)//CONALRRECORDNUM_IMPORTANT)
      ucECEnd=0;
  m=0;
  pstDataItem->ucDataContent[m++]=g_ucEventCount_Important;
  pstDataItem->ucDataContent[m++]=g_ucEventCount_Normal;
  pstDataItem->ucDataContent[m++]=ucECNo;
  pstDataItem->ucDataContent[m++]=ucECNo;
  while(ucECNo!=ucECEnd)
  {
      mTmp=m;
      ClearAlrRecord(&stAlrRecordTemp);
//      ucRet=Alarm_QueryRecord(ucECNo,&stAlrRecordTemp,CONALR_TYPE_IMPORTANT);
//      if(ucRet==RET_SUCCESS)
//AZH
      if(RET_SUCCESS == Alarm_QueryRecord(ucECNo,&stAlrRecordTemp,CONALR_TYPE_IMPORTANT))
      {
          for(i=0;i<stAlrRecordTemp.ucAlrDataItemNum;i++)
              mTmp+=stAlrRecordTemp.stAlrDataItem[i].usDataLen;
          if(mTmp+7>240)//CON_DI_DATA_LEN)                  //�ȼ�鳤��,�ܷ�ƴ���� //azh �������������ָ���
          {
              if(m==4)                                //һ�����Ų��£��쳣
                  ucRet=RET_ERROR;
              break;
          }
          pstDataItem->ucDataContent[m++]=stAlrRecordTemp.ucAlrERCCode;                       //��ø澯��Ϣ����
          pstDataItem->ucDataContent[m++]=0;                                                  //��Ϣ����
          usPos=m;
          ucTmpAlrERCCode=Alarm_GetAlrERCNo(CONALRCODE_POWERON);
          if(stAlrRecordTemp.ucAlrERCCode==ucTmpAlrERCCode)
          {
              memcpy((pstDataItem->ucDataContent+m),stAlrRecordTemp.stAlrDataItem[0].ucDataContent,
                  stAlrRecordTemp.stAlrDataItem[0].usDataLen);
              m+=stAlrRecordTemp.stAlrDataItem[0].usDataLen;
              memcpy((pstDataItem->ucDataContent+m),stAlrRecordTemp.stAlrDataItem[1].ucDataContent,
                  stAlrRecordTemp.stAlrDataItem[1].usDataLen);
              m+=stAlrRecordTemp.stAlrDataItem[1].usDataLen;
          }else
          {
              pstDataItem->ucDataContent[m++]=(unsigned char)HEX_TO_BCD(stAlrRecordTemp.stDateTime.ucMinute,1);
              pstDataItem->ucDataContent[m++]=(unsigned char)HEX_TO_BCD(stAlrRecordTemp.stDateTime.ucHour,1);
              pstDataItem->ucDataContent[m++]=(unsigned char)HEX_TO_BCD(stAlrRecordTemp.stDateTime.ucDay,1);
              pstDataItem->ucDataContent[m++]=(unsigned char)HEX_TO_BCD(stAlrRecordTemp.stDateTime.ucMonth,1);
              pstDataItem->ucDataContent[m++]=(unsigned char)HEX_TO_BCD(stAlrRecordTemp.stDateTime.ucYear,1);
              for(i=0;i<stAlrRecordTemp.ucAlrDataItemNum;i++)
              {
                  memcpy((pstDataItem->ucDataContent+m),stAlrRecordTemp.stAlrDataItem[i].ucDataContent,
                      stAlrRecordTemp.stAlrDataItem[i].usDataLen);
                  m+=stAlrRecordTemp.stAlrDataItem[i].usDataLen;
              }
          }

          pstDataItem->ucDataContent[usPos-1]=(unsigned char)(m-usPos);                                   //��Ϣ����
          Alarm_Set_AlrCate_word(&stAlrHappenWord,stAlrRecordTemp.ucAlrERCCode,CON_ALR_HAPPEN_NOT);
//          Alarm_Set_Happen_word(&stAlrHappenWord);
      }
      pstDataItem->ucDataContent[3]=ucECNo+1;
      ucECNo++;
      if(ucECNo>=ucMaxAlrNum)//CONALRRECORDNUM_IMPORTANT)
          ucECNo=0;

      n++;
      if(n>=8) break;//azh ���һ���Զ�8��    
  }
  if(ucRet==RET_SUCCESS)
  {
      pstDataItem->usDataLen=m;
      (*pucECLeftStart)=pstDataItem->ucDataContent[3];
  }

    return ucRet;
}

//һ���¼�
unsigned char Main_Recv_ReqData_3_Normal(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned char *pucECLeftStart,unsigned char ucECLastEnd)
{
    unsigned char ucRet=RET_SUCCESS;
  unsigned char ucECStart,ucECEnd,ucECNo,i=0,n=0;
  unsigned short m=0,mTmp=0,usPos=0;
//  STALRCATEGORY   stAlrHappenWord;
  unsigned char ucTmpAlrERCCode,ucMaxAlrNum=0;
//  stAlrHappenWord = Alarm_Get_Happen_word();

  ucMaxAlrNum = Alarm_GetRecordMaxNum(CON_ALR_ACD_NOT);
  ucECStart=(*pucECLeftStart);                            //�����¼���¼��ʼָ��Pm
  ucECEnd=ucECLastEnd;                                    //�����¼���¼����ָ��Pn

  ucECNo=ucECStart;
  if(ucECEnd>=ucMaxAlrNum)//CONALRRECORDNUM_NORMAL)
      ucECEnd=0;
  m=0;
  pstDataItem->ucDataContent[m++]=g_ucEventCount_Important;
  pstDataItem->ucDataContent[m++]=g_ucEventCount_Normal;
  pstDataItem->ucDataContent[m++]=ucECNo;
  pstDataItem->ucDataContent[m++]=ucECNo;
  while(ucECNo!=ucECEnd)
  {
      mTmp=m;
      ClearAlrRecord(&stAlrRecordTemp);
//      ucRet=Alarm_QueryRecord(ucECNo,&stAlrRecordTemp,CONALR_TYPE_NORMAL);
//      if(ucRet==RET_SUCCESS)
        if(RET_SUCCESS == Alarm_QueryRecord(ucECNo,&stAlrRecordTemp,CONALR_TYPE_NORMAL))      
        {
          for(i=0;i<stAlrRecordTemp.ucAlrDataItemNum;i++)
              mTmp+=stAlrRecordTemp.stAlrDataItem[i].usDataLen;
          if(mTmp+7>240)//CON_DI_DATA_LEN)                  //�ȼ�鳤��,�ܷ�ƴ����
          {
              if(m==4)                                //һ�����Ų��£��쳣
                  ucRet=RET_ERROR;
              break;
          }
          pstDataItem->ucDataContent[m++]=stAlrRecordTemp.ucAlrERCCode;                       //��ø澯��Ϣ����
          pstDataItem->ucDataContent[m++]=0;                                                  //��Ϣ����
          usPos=m;
          ucTmpAlrERCCode=Alarm_GetAlrERCNo(CONALRCODE_POWERON);
          if(stAlrRecordTemp.ucAlrERCCode==ucTmpAlrERCCode)
          {
              memcpy((pstDataItem->ucDataContent+m),stAlrRecordTemp.stAlrDataItem[0].ucDataContent,
                  stAlrRecordTemp.stAlrDataItem[0].usDataLen);
              m+=stAlrRecordTemp.stAlrDataItem[0].usDataLen;
              memcpy((pstDataItem->ucDataContent+m),stAlrRecordTemp.stAlrDataItem[1].ucDataContent,
                  stAlrRecordTemp.stAlrDataItem[1].usDataLen);
              m+=stAlrRecordTemp.stAlrDataItem[1].usDataLen;
          }else
          {
              pstDataItem->ucDataContent[m++]=(unsigned char)HEX_TO_BCD(stAlrRecordTemp.stDateTime.ucMinute,1);
              pstDataItem->ucDataContent[m++]=(unsigned char)HEX_TO_BCD(stAlrRecordTemp.stDateTime.ucHour,1);
              pstDataItem->ucDataContent[m++]=(unsigned char)HEX_TO_BCD(stAlrRecordTemp.stDateTime.ucDay,1);
              pstDataItem->ucDataContent[m++]=(unsigned char)HEX_TO_BCD(stAlrRecordTemp.stDateTime.ucMonth,1);
              pstDataItem->ucDataContent[m++]=(unsigned char)HEX_TO_BCD(stAlrRecordTemp.stDateTime.ucYear,1);
              for(i=0;i<stAlrRecordTemp.ucAlrDataItemNum;i++)
              {
                  memcpy((pstDataItem->ucDataContent+m),stAlrRecordTemp.stAlrDataItem[i].ucDataContent,
                      stAlrRecordTemp.stAlrDataItem[i].usDataLen);
                  m+=stAlrRecordTemp.stAlrDataItem[i].usDataLen;
              }
          }
          pstDataItem->ucDataContent[usPos-1]=(unsigned char)(m-usPos);                                   //��Ϣ����
          Alarm_Set_AlrCate_word(&stAlrHappenWord,stAlrRecordTemp.ucAlrERCCode,CON_ALR_HAPPEN_NOT);
//          Alarm_Set_Happen_word(&stAlrHappenWord);
      }
      pstDataItem->ucDataContent[3]=ucECNo+1;
      ucECNo++;
      if(ucECNo>=ucMaxAlrNum)//CONALRRECORDNUM_NORMAL)
          ucECNo=0;

      n++;
      if(n>=8) break;//azh ���һ���Զ�8��   
  }
  if(ucRet==RET_SUCCESS)
  {
      pstDataItem->usDataLen=m;
      (*pucECLeftStart)=pstDataItem->ucDataContent[3];
  }

    return ucRet;
}


//================  ExtCmd ======================
////F1 �ڴ��
//unsigned char Main_Recv_ExtCmd_F1_Mem_Read(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned char *pucBuffer,*pucContent;
//  unsigned char ucDataLen,m=0;
//  pucBuffer=pstDataItem->ucDataContent+4;
//  if(pucBuffer[m]==0x55 && pucBuffer[m+1]==0x66 && pucBuffer[m+2]==0x77 && pucBuffer[m+3]==0x88)
//  {
//      if(g_ucRAMReadTimes<5)
//  {
//          g_ucRAMReadTimes++;
//          if(g_ucRAMReadTimes==5)
//              g_usRAMWriteLifeCycle=1200;             //�ۼ�5�ζ��󣨷���������д��Ч10����
//      }
//      m+=4;
//      pucContent=(unsigned char *)(pucBuffer[m]+0x100*pucBuffer[m+1]+0x10000*pucBuffer[m+2]);
//      m+=3;
//      ucDataLen=pucBuffer[m++];
//      if(ucDataLen>128)
//          ucDataLen=128;
//      if((unsigned long)(pucContent+ucDataLen)<0xCFFFFF)
//      {
//          if(pucContent!=pucBuffer)
//          {
//              memcpy(pucBuffer+m,pucContent+0xc0000000,ucDataLen);
//          }
//      }else
//          ucRet=RET_ERROR;
//  }else
//      ucRet=RET_ERROR;
//  if(ucRet==RET_SUCCESS)
//      pstDataItem->usDataLen=ucDataLen+12;
//  else
//  {
//      pucBuffer[0]=CON_NACK_ERR_NOTEXE;
//  pstDataItem->usDataLen=5;
//  }
//  return ucRet;
//}
//
////F2 �ڴ�д�̶����16�ֽ�
//unsigned char Main_Recv_ExtCmd_F2_Mem_Write(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
////    unsigned char *pucBuffer,*pucContent;
////    unsigned char ucDataLen,m=0;
////    unsigned char i;
////    unsigned char *pucFlag;
////    STTERDEVADDR    stTerAddr=RunPara_GetTerDevAddr();
////    pucBuffer=pstDataItem->ucDataContent+4;
////    //  if(pucBuffer[m]==0x55 && pucBuffer[m+1]==0x66 && pucBuffer[m+2]==0x77 && pucBuffer[m+3]==0x88 && g_usRAMWriteLifeCycle!=0)
////    if(pucBuffer[m]==0x56 && pucBuffer[m+1]==0x78 && pucBuffer[m+2]==stTerAddr.ucDevAddr[0] && pucBuffer[m+3]==stTerAddr.ucDevAddr[1] && g_usRAMWriteLifeCycle!=0)
////    {
////        g_ucRAMReadTimes=0;
////        g_usRAMWriteLifeCycle=0;
////        m+=4;
////        pucContent=(unsigned char *)(pucBuffer[m]+0x100*pucBuffer[m+1]+0x10000*pucBuffer[m+2]);
////        m+=3;
////        ucDataLen=pucBuffer[m++];
////        if(ucDataLen>16)
////            ucDataLen=16;
////        if((unsigned long)(pucContent+ucDataLen)<0xCFFFFF)
////        {
////            if(pucContent!=pucBuffer)
////                memcpy(pucContent+0xc0000000,pucBuffer+m,ucDataLen);
////            if(memcmp(pucContent+0xc0000000,pucBuffer+m,ucDataLen)!=0)
////                ucRet=RET_ERROR;
////        }else
////            ucRet=RET_ERROR;
////    }else if(pucBuffer[m]==0x88 && pucBuffer[m+1]==0x77 && pucBuffer[m+2]==0x66 && pucBuffer[m+3]==0x55)
////    {
////        pucFlag=(unsigned char *)(RM_FLAG_START_ADDR+DP_DOWNLOAD_EXE_FLAG_ADDR);
////        for(i=0;i<4;i++)
////            pucFlag[i]=0x55;
////        for(i=4;i<8;i++)
////            pucFlag[i]=0xAA;
////        ((void(*)(void))0xbfc00000)();
////    }else
////        ucRet=RET_ERROR;
////    if(ucRet==RET_SUCCESS)
////        pucBuffer[0]=CON_NACK_ERR_NORMAL;
////    else
////        pucBuffer[0]=CON_NACK_ERR_NOTEXE;
////    pstDataItem->usDataLen=5;
//  return ucRet;
//}
//
////F3 DATAFLASH��
//unsigned char Main_Recv_ExtCmd_F3_Dataflash_Read(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
////    unsigned char *pucBuffer;
////    unsigned char ucDataLen,m=0;
////    unsigned long ulDataAddr;
////    unsigned short usBeginPage,usBeginPos;
////    pucBuffer=pstDataItem->ucDataContent+4;
////    if(pucBuffer[m]==0x55 && pucBuffer[m+1]==0x66 && pucBuffer[m+2]==0x77 && pucBuffer[m+3]==0x88)
////    {
////        if(g_ucRAMReadTimes<5)
////        {
////            g_ucRAMReadTimes++;
////            if(g_ucRAMReadTimes==5)
////                g_usRAMWriteLifeCycle=1200;             //�ۼ�5�ζ��󣨷���������д��Ч10����
////        }
////        m+=4;
////        ulDataAddr=pucBuffer[m]+0x100*pucBuffer[m+1]+0x10000*pucBuffer[m+2];
////        m+=3;
////        ucDataLen=pucBuffer[m++];
////        if(ucDataLen>128)
////            ucDataLen=128;
////        usBeginPage =(unsigned short)(ulDataAddr/FPAGE_SIZE);
////        usBeginPos  =(unsigned short)(ulDataAddr % FPAGE_SIZE);
////        ucRet=Flash_GetMainMemory(usBeginPage,usBeginPos,ucDataLen,pucBuffer+m);
////        if(ucRet==EF_NORMAL)
////            ucRet=RET_SUCCESS;
////        else
////            ucRet=RET_ERROR;
////    }else
////        ucRet=RET_ERROR;
////    if(ucRet==RET_SUCCESS)
////        pstDataItem->usDataLen=ucDataLen+11;
////    else
////    {
////        pucBuffer[0]=CON_NACK_ERR_NOTEXE;
////        pstDataItem->usDataLen=5;
////    }
//  return ucRet;
//}
//
////F4 DATAFLASHд
//unsigned char Main_Recv_ExtCmd_F4_Dataflash_Write(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
////    unsigned char *pucBuffer;
////    unsigned char ucDataLen,m=0;
////    unsigned long ulDataAddr;
////    unsigned short usBeginPage,usBeginPos;
////    STTERDEVADDR    stTerAddr=RunPara_GetTerDevAddr();
////    pucBuffer=pstDataItem->ucDataContent+4;
////    //  if(pucBuffer[m]==0x55 && pucBuffer[m+1]==0x66 && pucBuffer[m+2]==0x77 && pucBuffer[m+3]==0x88 && g_usRAMWriteLifeCycle!=0)
////    if(pucBuffer[m]==0x56 && pucBuffer[m+1]==0x78 && pucBuffer[m+2]==stTerAddr.ucDevAddr[0] && pucBuffer[m+3]==stTerAddr.ucDevAddr[1] && g_usRAMWriteLifeCycle!=0)
////    {
////        g_ucRAMReadTimes=0;
////        g_usRAMWriteLifeCycle=0;
////        m+=4;
////        ulDataAddr=pucBuffer[m]+0x100*pucBuffer[m+1]+0x10000*pucBuffer[m+2];
////        m+=3;
////        ucDataLen=pucBuffer[m++];
////        if(ucDataLen>16)
////            ucDataLen=16;
////        usBeginPage =(unsigned short)(ulDataAddr/FPAGE_SIZE);
////        usBeginPos  =(unsigned short)(ulDataAddr % FPAGE_SIZE);
////        ucRet=Flash_SetMainMemory(pucBuffer+m,ucDataLen,usBeginPage,usBeginPos);
////        if(ucRet==EF_NORMAL)
////            ucRet=RET_SUCCESS;
////        else
////            ucRet=RET_ERROR;
////    }else
////        ucRet=RET_ERROR;
////    if(ucRet==RET_SUCCESS)
////        pucBuffer[0]=CON_NACK_ERR_NORMAL;
////    else
////        pucBuffer[0]=CON_NACK_ERR_NOTEXE;
////    pstDataItem->usDataLen=5;
//  return ucRet;
//}
//
////F5 ���Ź����ԣ��ó��������ѭ��
//unsigned char Main_Recv_ExtCmd_F5_WdtTest_EnterDeadLoop(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned char *pucBuffer;
//  unsigned char m=0;
//  volatile unsigned short usFlag;
//  usFlag=0x5A3C;
//  pucBuffer=pstDataItem->ucDataContent+4;
//  if(pucBuffer[m]==0x55 && pucBuffer[m+1]==0x66 && pucBuffer[m+2]==0x77 && pucBuffer[m+3]==0x88)
//  {
//      if(usFlag==0x5A3C)
//      {
////azh         __DI();
////            while(1);                   //��ѭ���������ⲿ���Ź���λ
//      }
//  }
//  return ucRet;
//      }
//
////F6 �ֻ�ģ��ͨѶ���
//unsigned char Main_Recv_ExtCmd_F6_GetCommInfo(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned char m=0;
//  unsigned char *pucBuffer = pstDataItem->ucDataContent+4;
//  STTERMISCPARA   stTerMiscPara=RunPara_GetTerMiscPara();
//  if(stTerMiscPara.stRCommRunInfo.ucGprsLoginStat==CON_STATUS_YES)
//      pucBuffer[m++]=1;       //ͨѶ���ͣ�G=1/S=0/X
//      else
//      pucBuffer[m++]=0;
//  if(stTerMiscPara.stRCommRunInfo.ucRCommStat==CON_STATUS_YES)
//  {
//      pucBuffer[m++]=0x55;                            //ģ��������55���쳣��AA
//      pucBuffer[m++]=stTerMiscPara.stRCommRunInfo.ucSignIntension;    //�ź�ǿ��
//  }else
//  {
//      pucBuffer[m++]=0xAA;
//      pucBuffer[m++]=0;
//  }
//  pucBuffer[m++]=stTerMiscPara.stRCommRunInfo.ucModSoftVer[2];    //ģ������汾��4321�ĵڶ��ֽ�
//  memset(pucBuffer+m,0x00,4);
//  pstDataItem->usDataLen=12;
//  return ucRet;
//  }
//
////F7 �ն�״̬���
////0Ϊ��������0����
//unsigned char Main_Recv_ExtCmd_F7_GetTerErrInfo(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned char m=0;
//  unsigned char *pucBuffer = pstDataItem->ucDataContent+4;
//  unsigned char ucFlag,i,ucMPNo,ucMPType;
//  STHISVAL stHisVal;
//  //1 �ն������ڴ����
//  //2 ʱ�ӹ���
//  pucBuffer[m++]=0;
//  pucBuffer[m++]=0;
//  //3 ����ͨ�Ź���
//  if(RunPara_GetTerMisc_RCommRunStat()==CON_STATUS_YES)
//      pucBuffer[m++]=0;           //0Ϊ����
//  else
//      pucBuffer[m++]=1;
//  //4 485�������
//  ucFlag=0;
//  for(i=0;i<CON_METER_NUM;i++)
//  {
//      ucMPNo      = RunPara_GetMeter_MPNo(i);
//      ucMPType    = RunPara_GetMPType(ucMPNo);
//      if(ucMPType == CON_MPTYPE_METER)
//      {
//          if(RunPara_GetMeterPara_Proto(i)!=CON_PROTOCOL_INVALID)
//          {
//              stHisVal = Get_History_DirPower_byMPNo(ucMPNo,0,0);
//              if(stHisVal.lVal == CONINVALIDVAL)
//                  break;
//          }
//      }
//  }
//  if(i < CON_METER_NUM)
//      ucFlag=1;
//  pucBuffer[m++]=ucFlag;
//  //5 ��ʾ�����
////    if(LCD_Check_Run_StateIsOk()==RET_SUCCESS)
//      pucBuffer[m++]=0;           //0Ϊ����
////    else
////        pucBuffer[m++]=1;
//  memset(pucBuffer+m,0x00,11);
//  pstDataItem->usDataLen=20;
//  return ucRet;
//}
//
////F10 �ն���չ��Ϣ����
unsigned char Main_Recv_ExtCmd_F10_TerExtInfo(STDATAITEM *pstDataItem,unsigned char ucDAp)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char *pucBuffer = pstDataItem->ucDataContent+4;
//  STTERMISCPARA   stTerMiscPara;
//  STTERADJPARA    stTerAdjPara;
    switch(pucBuffer[0])
    {
//  case 1:                                     //��ַ��ʾ��55��ʾ10���ƣ�AA��ʾ16����
//      stTerMiscPara=RunPara_GetTerMiscPara();
//      if(pucBuffer[1]==0x55)
//          stTerMiscPara.ucTerAddIsDecType=CON_STATUS_YES;
//      else
//          stTerMiscPara.ucTerAddIsDecType=CON_STATUS_NO;
//      RunPara_SetTerMiscPara(&stTerMiscPara);
//      pucBuffer[0]=CON_NACK_ERR_NORMAL;
//      break;
//  case 2:                                     //���������л�
//      stTerAdjPara=RunPara_GetTerAdjPara();
//      if(pucBuffer[1]==0xFF)//��ѯ
//          pucBuffer[0]=stTerAdjPara.ucEnerBuyIsNotFee;
//      else if(pucBuffer[1]==CON_STATUS_YES)//��ѯ
//          stTerAdjPara.ucEnerBuyIsNotFee=CON_STATUS_YES;
//  else
//          stTerAdjPara.ucEnerBuyIsNotFee=CON_STATUS_NO;
//      if(stTerAdjPara.ucEnerBuyIsNotFee==CON_STATUS_YES)
//          stTerAdjPara.ucEnerBuyUnit=0;
//  else
//          stTerAdjPara.ucEnerBuyUnit=1;
//      RunPara_SetTerAdjPara(&stTerAdjPara);
//      break;
    case 3:                                     //�Ƿ���ʾģ�������Ϣ
        pucBuffer[0]=CON_NACK_ERR_NORMAL;
        if(pucBuffer[1]==0x55)
//          RunPara_SetTerMiscPara_ShowRCommDebug(CON_STATUS_YES);
            gucAtCmd_WatchOut_Flag = CON_ATCMD_OUT_ENABLE;//��AT������
        else if(pucBuffer[1]==0xAA)
//          RunPara_SetTerMiscPara_ShowRCommDebug(CON_STATUS_NO);
            gucAtCmd_WatchOut_Flag = 0;
        else
        {
            if(gucAtCmd_WatchOut_Flag == CON_ATCMD_OUT_ENABLE)
            {
                pucBuffer[0]=CON_STATUS_YES;
            }
            else
            {
                pucBuffer[0]=CON_STATUS_NO;
            }
        }
        break;
//azh 2013-09-03 �������ݴ�ӡ��485-1������ԿڵĿ���
    case 4:                                     //�Ƿ���ʾģ�������Ϣ
        pucBuffer[0]=CON_NACK_ERR_NORMAL;
        if(pucBuffer[1]==0x55)
            g_usNeedDebugCommInfo = CON_STATUS_YES_2BYTE;
        else if(pucBuffer[1]==0xAA)
			g_usNeedDebugCommInfo = CON_STATUS_NO_2BYTE;
        else
        {
            if(g_usNeedDebugCommInfo == CON_STATUS_YES_2BYTE)
            {
                pucBuffer[0]=0x55;
            }
            else
            {
                pucBuffer[0]=0xaa;
            }
        }
        break;
//
//  default:
//      pucBuffer[0]=CON_NACK_ERR_NORMAL;
//      break;
    }
    pstDataItem->usDataLen=5;
    return ucRet;
}
//
////F11 ������Ϣ��ȡ
//unsigned char Main_Recv_ExtCmd_F11_GetShortMessage(STDATAITEM *pstDataItem,unsigned char ucDAp)
//      {
//  unsigned char ucRet=RET_SUCCESS;
////    unsigned char m=0,ucMsgNo;
////    unsigned char *pucBuffer = pstDataItem->ucDataContent+4;
////    STSHORTMESSAGE * stMsg;
////    ucMsgNo=pucBuffer[m++];
////    if(ucMsgNo<CON_MESSAGE_NUM)
////    {
////        pucBuffer[m++]=LCD_GetMessage_Num();
////        if(ucDAp==1)
////            stMsg=LCD_GetHisMessage(ucMsgNo,CON_MESSAGE_TYPE_NORM);         //��Ϣ��2��ȡһ����Ϣ
////        else
////            stMsg=LCD_GetHisMessage(ucMsgNo,CON_MESSAGE_TYPE_IMPT);         //������ȡ��Ҫ��Ϣ
////        if(stMsg==NULL || stMsg->ucAvailble!=CON_STATUS_YES)
////        {
////            pucBuffer[m++]=0;
////            pucBuffer[m++]=0;
////        }else
////        {
////            pucBuffer[m++]=(unsigned char)((stMsg->ucType<<4)+stMsg->ucNo);
////            if(stMsg->ucLength>CON_MESSAGE_DATALEN)
////                stMsg->ucLength=CON_MESSAGE_DATALEN;
////            pucBuffer[m++]=stMsg->ucLength;
////            memcpy(pucBuffer+m,stMsg->ucMsg,stMsg->ucLength);
////            m+=stMsg->ucLength;
////        }
////        pstDataItem->usDataLen=m+4;
////    }else
////    {
////        pucBuffer[0]=CON_NACK_ERR_NOTEXE;
////        pstDataItem->usDataLen=5;
////    }
//  return ucRet;
//}
//
////F23 GR��Ϣ��
//unsigned char Main_Recv_ExtCmd_F23_GRInfo_Read(STDATAITEM *pstDataItem,unsigned char ucDAp)
//      {
//  unsigned char   ucRet=RET_SUCCESS;
//  //  unsigned char   *pucWorkBuffer;
//  //  unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
//  //  pucWorkBuffer=(unsigned char *)CTaskWorkBuffer;
//  //  memcpy(pucBuffer,(unsigned char *)&(stGRDefineArea.stGRTaskInfo[0]),48);                    //����ǰ6����ǰ2���
//  //  memset(pucBuffer+48,0xFF,4);
//  //  memcpy(pucBuffer+52,(unsigned char *)&stAlrRecordInfo,16);          //�澯��Ϣ
//  //  pstDataItem->usDataLen =68+4;
//
//  pstDataItem->ucDataContent[4]=CON_NACK_ERR_NORMAL;
//  pstDataItem->usDataLen=5;
//  return ucRet;
//      }
//
////F24 GR��Ϣд
//unsigned char Main_Recv_ExtCmd_F24_GRInfo_Write(STDATAITEM *pstDataItem,unsigned char ucDAp)
//  {
//  unsigned char   ucRet=RET_SUCCESS;
//  pstDataItem->ucDataContent[4]=CON_NACK_ERR_NORMAL;
//      pstDataItem->usDataLen=5;
//  return ucRet;
//}
//
////F31 ������������
//unsigned char Main_Recv_ExtCmd_F31_ClearDemand(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{//ucDApΪ�����ţ���1��ʼ
//  unsigned char ucRet=RET_SUCCESS;
////    unsigned char ucPulseNo,i=0;
////    unsigned char *pucBuffer=pstDataItem->ucDataContent+4;
////    ucPulseNo=ucDAp;
////    if(ucPulseNo>=CON_PULSE_NUM)
////        return RET_ERROR;
////    for(i=0;i<CONPULSEHISDATANUM;i++)
////    {
////        Clear_PulseMaxDemand_DirPowTotal(ucPulseNo,i);
////        Clear_PulseMaxDemand_DirVarTotal(ucPulseNo,i);
////        Clear_PulseMaxDemand_RevPowTotal(ucPulseNo,i);
////        Clear_PulseMaxDemand_RevVarTotal(ucPulseNo,i);
////    }
////    Set_PulseCount_AllAsNow(ucPulseNo);
////    //������Ϣ
////    pucBuffer[0]=CON_NACK_ERR_NORMAL;
////    pstDataItem->usDataLen=5;
//  return ucRet;
//}
//
////F33 ����׶���������ȫ
//unsigned char Main_Recv_ExtCmd_F33_SetPowDataAll(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{//ucDApΪ�����ţ���1��ʼ
//  unsigned char ucRet=RET_SUCCESS;
////    unsigned char ucPulseNo,i=0;
////    unsigned char m=0;
////    unsigned char *pucBuffer=pstDataItem->ucDataContent+4;
////    unsigned long ulVal=0;
////    unsigned long ulVal1,ulVal2,ulVal3,ulVal4;
////    if(ucRet==RET_SUCCESS)
////    {
////        ucPulseNo=ucDAp;
////        if(ucPulseNo>=CON_PULSE_NUM)
////            ucRet=RET_ERROR;
////    }
////    if(ucRet==RET_SUCCESS)
////    {
////        for(i=0;i<16;i++)
////        {
////            if(Check_BCD_Data(pucBuffer[m+i])!=RET_SUCCESS)
////            {
////                ucRet=RET_ERROR;
////                break;
////            }
////        }
////    }
////    if(ucRet==RET_SUCCESS)
////    {
////        ulVal1=BCD_TO_HEX_4(pucBuffer+m);m+=4;
////        Set_PulseMeterBase_DirPow(ucPulseNo,1,ulVal1);
////        Set_PulseMeterBase_DirVar(ucPulseNo,1,ulVal1);
////        Set_PulseMeterBase_RevPow(ucPulseNo,1,ulVal1);
////        Set_PulseMeterBase_RevVar(ucPulseNo,1,ulVal1);
////
////        ulVal2=BCD_TO_HEX_4(pucBuffer+m);m+=4;
////        Set_PulseMeterBase_DirPow(ucPulseNo,2,ulVal2);
////        Set_PulseMeterBase_DirVar(ucPulseNo,2,ulVal2);
////        Set_PulseMeterBase_RevPow(ucPulseNo,2,ulVal2);
////        Set_PulseMeterBase_RevVar(ucPulseNo,2,ulVal2);
////
////        ulVal3=BCD_TO_HEX_4(pucBuffer+m);m+=4;
////        Set_PulseMeterBase_DirPow(ucPulseNo,3,ulVal3);
////        Set_PulseMeterBase_DirVar(ucPulseNo,3,ulVal3);
////        Set_PulseMeterBase_RevPow(ucPulseNo,3,ulVal3);
////        Set_PulseMeterBase_RevVar(ucPulseNo,3,ulVal3);
////
////        ulVal4=BCD_TO_HEX_4(pucBuffer+m);m+=4;
////        Set_PulseMeterBase_DirPow(ucPulseNo,4,ulVal4);
////        Set_PulseMeterBase_DirVar(ucPulseNo,4,ulVal4);
////        Set_PulseMeterBase_RevPow(ucPulseNo,4,ulVal4);
////        Set_PulseMeterBase_RevVar(ucPulseNo,4,ulVal4);
////
////        Set_PulseMeterBase_DirPow(ucPulseNo,0,(ulVal1+ulVal2+ulVal3+ulVal4)%100000000);
////        Set_PulseMeterBase_DirVar(ucPulseNo,0,(ulVal1+ulVal2+ulVal3+ulVal4)%100000000);
////        Set_PulseMeterBase_RevPow(ucPulseNo,0,(ulVal1+ulVal2+ulVal3+ulVal4)%100000000);
////        Set_PulseMeterBase_RevVar(ucPulseNo,0,(ulVal1+ulVal2+ulVal3+ulVal4)%100000000);
////
////        Clear_PulseMeterVal_LastMonth(ucPulseNo);       //������µ���
////        Set_PulseCount_AllSubNow(ucPulseNo);
////        Make_PulseMonthChange(ucPulseNo);               //�������µ���
////    }
////    //������Ϣ
////    if(ucRet==RET_SUCCESS)
////        pucBuffer[0]=CON_NACK_ERR_NORMAL;
////    else
////        pucBuffer[0]=CON_NACK_ERR_NOTEXE;
////    pstDataItem->usDataLen=5;
//  return ucRet;
//  }
////F40 �ڱ���������
//unsigned char Main_Recv_ExtCmd_F40_7758SetMiscData(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
////    unsigned char   m=0;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
////    unsigned char ucLen = pucBuffer[2];
////    unsigned short usItem ;
////    usItem = pucBuffer[1]*0x100 + pucBuffer[0];
////    ucRet = ADESetMiscData(usItem,&pucBuffer[3],ucLen);
////    if(ucRet==RET_SUCCESS)
////        pucBuffer[0]=CON_NACK_ERR_NORMAL;
////    else
////        pucBuffer[0]=CON_NACK_ERR_NOTEXE;
////    pstDataItem->usDataLen=5;
//  return ucRet;
//}
//// F41 ��ȡ�ڱ��������
//unsigned char Main_Recv_ExtCmd_F41_7758GetMiscData(STDATAITEM *pstDataItem,unsigned char ucDAp)
//  {
//  unsigned char ucRet=RET_SUCCESS;
////    unsigned char   m=0;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
////    unsigned short  usItem;
////    usItem = pucBuffer[1]*0x100 + pucBuffer[0];
////    ucRet = ADEGetMiscData(usItem,&pucBuffer[2],&m);
////    pstDataItem->usDataLen =m+6;
//  return ucRet;
//  }
////F51 �ն˵�ַ�������Ŷ�
//unsigned char Main_Recv_ExtCmd_F51_Get_TerAddr(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char m=0;
//  unsigned char *pucBuffer=pstDataItem->ucDataContent+4;
//  STTERDEVADDR    stTerAddr=RunPara_GetTerDevAddr();
//  STTERREGIONADDR stTerRegionAddr=RunPara_GetTerRegionAddr();
//  unsigned char ucVerInfo[4];
//
//  if(ucDAp==3)            //da= 08 01
//  {
//      RunPara_GetTerInnerVerInfo(ucVerInfo);
//      memcpy(pucBuffer+m,ucVerInfo,4);
//      m+=4;
//  }
//  else
//      {
//      pucBuffer[m++] =stTerRegionAddr.ucRegionAddr[0];
//      pucBuffer[m++] =stTerRegionAddr.ucRegionAddr[1];
//      pucBuffer[m++] =stTerAddr.ucDevAddr[0];
//      pucBuffer[m++] =stTerAddr.ucDevAddr[1];
//          }
//
//  pstDataItem->usDataLen =m+4;
//  return ucRet;
//      }
//
////F52 �ն���Ŷ�
//unsigned char Main_Recv_ExtCmd_F52_Get_TerSequNo(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer=pstDataItem->ucDataContent+4;
//  STTERAUXPARA stTerAuxPara=RunPara_GetTerAuxPara();
//  memcpy(pucBuffer,stTerAuxPara.ucDevNo,8);
//  pstDataItem->usDataLen=12;
//  return ucRet;
//  }
//
////F53 �ն˵�ַ��������д
//unsigned char Main_Recv_ExtCmd_F53_Set_TerAddr(STDATAITEM *pstDataItem,unsigned char ucDAp)
//  {
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   m=0;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
//  STTERDEVADDR    stTerAddr=RunPara_GetTerDevAddr();
//  STTERREGIONADDR stTerRegionAddr=RunPara_GetTerRegionAddr();
//
//
//  stTerRegionAddr.ucRegionAddr[0] =pucBuffer[m++];
//  stTerRegionAddr.ucRegionAddr[1] =pucBuffer[m++];
//  stTerAddr.ucDevAddr[0] =pucBuffer[m++];
//  stTerAddr.ucDevAddr[1] =pucBuffer[m++];
//  if((Check_BCD_Data(stTerRegionAddr.ucRegionAddr[0])==RET_SUCCESS)
//      && (Check_BCD_Data(stTerRegionAddr.ucRegionAddr[1])==RET_SUCCESS))
//  {
//      RunPara_SetTerRegionAddr(&stTerRegionAddr);
//      RunPara_SetTerDevAddr(&stTerAddr);
//      pucBuffer[0]=CON_NACK_ERR_NORMAL;
//  }else
//      pucBuffer[0]=CON_NACK_ERR_NOTEXE;
//  pstDataItem->usDataLen=5;
//
//
//
//  return ucRet;
//  }
//
////F54 �ն����д
//unsigned char Main_Recv_ExtCmd_F54_Set_TerSequNo(STDATAITEM *pstDataItem,unsigned char ucDAp)
//  {
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
//  STTERAUXPARA stTerAuxPara=RunPara_GetTerAuxPara();
//  memcpy(stTerAuxPara.ucDevNo,pucBuffer,8);
//  RunPara_SetTerAuxPara(&stTerAuxPara);
//  pucBuffer[0]=CON_NACK_ERR_NORMAL;
//  pstDataItem->usDataLen=5;
//  return ucRet;
//}
//
////F71 ����������
//unsigned char Main_Recv_ExtCmd_F71_Ctrl_Beep(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
////    unsigned short usTotal,usDelay,usLast,usInter;
////    if(pucBuffer[0]==0x55)
////    {
////        usTotal=pucBuffer[1]+0x100*pucBuffer[2];
////        usDelay=pucBuffer[3]+0x100*pucBuffer[4];
////        usLast =pucBuffer[5]+0x100*pucBuffer[6];
////        usInter=pucBuffer[7]+0x100*pucBuffer[8];
//////      Beep_Start(usTotal,usDelay,usLast,usInter);
////    }else
////        Beep_Stop();
////    pucBuffer[0]=CON_NACK_ERR_NORMAL;
////    pstDataItem->usDataLen=5;
//  return ucRet;
//}
//
////F72 �޸�ʱ��
//unsigned char Main_Recv_ExtCmd_F72_ChangeTime(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
////    STDATETIME stOrigTime,stCurrentTime;
////    unsigned long ulTime;
////    stCurrentTime=GetCurrentTime();
////    stOrigTime=stCurrentTime;
////    ulTime=TimeConver(&stCurrentTime,CON_TIMEUNIT_SEC);
////    if(pucBuffer[0]==0x55)
////        ulTime+=60*pucBuffer[1];        //����ʱ��
////    else
////        ulTime-=60*pucBuffer[1];        //����ʱ��
////    stCurrentTime=TimeConverToDataTime(ulTime);
////    SetDateTime(&stCurrentTime);        //����ʱ��
////    TimeChange_PulseHisData(stOrigTime,stCurrentTime);      //����ʱ����������Ϣ
////    TimeChange_JobManage(stOrigTime,stCurrentTime);
////    TimeChange_CTaskProc(stOrigTime,stCurrentTime);
////    TimeChange_AlarmManage(stOrigTime,stCurrentTime);
////    Beep_ExternPara_Change();
////    pucBuffer[0]=CON_NACK_ERR_NORMAL;
////    pstDataItem->usDataLen=5;
//  return ucRet;
//}
//
////F73 ��ѯͨѶ������
//unsigned char Main_Recv_ExtCmd_F73_Get_CommBoad(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
//  unsigned char   ucCommNo,ucMeterNo;
//  unsigned long   ulBoad;
//  STMETERPARA     stMeterPara;
//      ucCommNo=pucBuffer[0];
//  if(ucCommNo<4)
//      {
//      if(ucCommNo==1)
//          {
//              if(ucDAp<CON_METER_NUM)
//                  ucMeterNo=ucDAp;
//              else
//                  ucMeterNo=0;
//              stMeterPara=RunPara_GetMeterPara(ucMeterNo);
//          ulBoad=stMeterPara.ulBaudRate;
//          pucBuffer[5]=stMeterPara.ucEPN;
//          pucBuffer[6]=stMeterPara.ucDataBit;
//          pucBuffer[7]=stMeterPara.ucStopBit;
//      }else
//      {
//          ulBoad=RunPara_GetUartBoad(ucCommNo);
//          pucBuffer[5]=UART_EVEN_PARITY;//CON_UART_EPN_EVEN;
//          pucBuffer[6]=8;
//          pucBuffer[7]=UART_1_STOP_BIT;//CON_UART_STPB_1;
//          }
//      ulBoad=HEX_TO_BCD(ulBoad,4);
//      memcpy(pucBuffer+1,(unsigned char *)&ulBoad,4);
//      pstDataItem->usDataLen=12;
//  }else
//  {
//      pucBuffer[0]=CON_NACK_ERR_NOTEXE;
//  pstDataItem->usDataLen=5;
//  }
//  return ucRet;
//}
//
////F74 �޸�ͨѶ������
//unsigned char Main_Recv_ExtCmd_F74_Set_CommBoad(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
////    unsigned char   ucCommNo,ucMeterNo;
////    unsigned long   ulBoad;
////    unsigned char   ucEPN=1,ucDataBit=8,ucStopBit=1;
////    STTERMISCPARA   stTerMiscPara;
////    STMETERPARA     stMeterPara;
////    if(Check_BCD_Data(pucBuffer[1])==RET_SUCCESS &&
////        Check_BCD_Data(pucBuffer[2])==RET_SUCCESS &&
////        Check_BCD_Data(pucBuffer[3])==RET_SUCCESS &&
////        Check_BCD_Data(pucBuffer[4])==RET_SUCCESS)
////    {
////        ucCommNo=pucBuffer[0];
////        ulBoad=BCD_TO_HEX_4(pucBuffer+1);
////        ucEPN=pucBuffer[5];
////        ucDataBit=pucBuffer[6];
////        ucStopBit=pucBuffer[7];
////        if(ucCommNo<4 && ulBoad>290 && ulBoad<200000)
////        {
////            stTerMiscPara=RunPara_GetTerMiscPara();
////            stTerMiscPara.ulBoadrate[ucCommNo]=ulBoad;
////            switch(ucCommNo)
////            {
////            case 0:
////                if(ulBoad<9600)
////                    ulBoad=9600;
////                RComm_Initial(ulBoad);
////                break;
////            case 1:
////                if(ucDAp<CON_METER_NUM)
////                    ucMeterNo=ucDAp;
////                else
////                    ucMeterNo=0;
////                stMeterPara=RunPara_GetMeterPara(ucMeterNo);
////                stMeterPara.ulBaudRate=ulBoad;
////                stMeterPara.ucEPN=ucEPN;
////                stMeterPara.ucDataBit=ucDataBit;
////                stMeterPara.ucStopBit=ucStopBit;
////                RunPara_SetMeterPara(ucMeterNo,&stMeterPara);
////                Meter_Init(ulBoad,0);                               //�ڶ�·������
////                CopyMeter_Timer_StartNow();
////                break;
////            case 2:
////                if(ulBoad<1200)
////                    ulBoad=1200;
////                DbgComm_Init(ulBoad);
////                break;
////            }
////            stTerMiscPara.ulBoadrate[ucCommNo]=ulBoad;
////            RunPara_SetTerMiscPara(&stTerMiscPara);
////            pucBuffer[0]=CON_NACK_ERR_NORMAL;
////        }else
////            pucBuffer[0]=CON_NACK_ERR_NOTEXE;
////    }else
////        pucBuffer[0]=CON_NACK_ERR_NOTEXE;
////    pstDataItem->usDataLen=5;
//  return ucRet;
//}
//
//////F75 ��ȡҺ��������ʾ����
////unsigned char Main_Recv_ExtCmd_F75_Get_LCDCycle(STDATAITEM *pstDataItem,unsigned char ucDAp)
////{
////    unsigned char   ucRet=RET_SUCCESS,i=0;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
////    unsigned char   m=0;
////    STCYCLEMODEPARA stCycleModePara;
////    stCycleModePara=RunPara_GetCycleModePara();
////    pucBuffer[m++]=(unsigned char)stCycleModePara.usCycleTime;
////    pucBuffer[m++]=(unsigned char)(stCycleModePara.usCycleTime>>8);
////    for(i=0;i<CONCYCLEPAGENUM;i++)
////    {
////        pucBuffer[m++]=stCycleModePara.stCyclePage[i].ucMPNo+1;
////        pucBuffer[m++]=stCycleModePara.stCyclePage[i].ucPageNo;
////    }
////    pstDataItem->usDataLen=m+4;
////    return ucRet;
////}
//
//////F76 ��ȡҺ��������ʾ����
////unsigned char Main_Recv_ExtCmd_F76_Get_LCDCtrl(STDATAITEM *pstDataItem,unsigned char ucDAp)
////{
////    unsigned char   ucRet=RET_SUCCESS,i=0;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
////    unsigned char   m=0,ucStart;
////    STCTRLMODEPARA  stCtrlModePara;
////    ucStart=pucBuffer[m++];
////    if(ucStart>(CONCTRLPAGENUM-16))
////        ucStart=0;
////    stCtrlModePara=RunPara_GetCtrlModePara();
////    pucBuffer[m++]=(unsigned char)stCtrlModePara.usCtrlRemainTime;
////    pucBuffer[m++]=(unsigned char)(stCtrlModePara.usCtrlRemainTime>>8);
////    for(i=0;i<16;i++)
////    {
////        pucBuffer[m++]=stCtrlModePara.stCtrlPage[ucStart+i].ucMPNo+1;
////        pucBuffer[m++]=stCtrlModePara.stCtrlPage[ucStart+i].ucPageNo;
////    }
////    pucBuffer[0]=ucStart;
////    pstDataItem->usDataLen=m+4;
////    return ucRet;
////}
////
//////F77 ��ȡҺ��������ʾ����
////unsigned char Main_Recv_ExtCmd_F77_Get_LCDSpec(STDATAITEM *pstDataItem,unsigned char ucDAp)
////{
////    unsigned char   ucRet=RET_SUCCESS;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
////    unsigned char   m=0;
////    STSPECMODEPARA  stSpecModePara;
////    stSpecModePara=RunPara_GetSpecModePara();
////    pucBuffer[m++]=(unsigned char)stSpecModePara.usSpecRemainTime;
////    pucBuffer[m++]=(unsigned char)(stSpecModePara.usSpecRemainTime>>8);
////    pucBuffer[m++]=stSpecModePara.stSpecPage.ucMPNo+1;
////    pucBuffer[m++]=stSpecModePara.stSpecPage.ucPageNo;
////    pstDataItem->usDataLen=m+4;
////    return ucRet;
////}
////
//////F78 �޸�Һ��������ʾ����
////unsigned char Main_Recv_ExtCmd_F78_Set_LCDCycle(STDATAITEM *pstDataItem,unsigned char ucDAp)
////{
////    unsigned char   ucRet=RET_SUCCESS,i=0;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
////    unsigned char   m=0;
////    STCYCLEMODEPARA stCycleModePara;
////    stCycleModePara=RunPara_GetCycleModePara();
////    stCycleModePara.usCycleTime=pucBuffer[m]+0x100*pucBuffer[m+1];  m+=2;
////    for(i=0;i<CONCYCLEPAGENUM;i++)
////    {
////        if(pucBuffer[m]==0)
////            stCycleModePara.stCyclePage[i].ucMPNo=0;
////        else
////            stCycleModePara.stCyclePage[i].ucMPNo=pucBuffer[m]-1;       //�ڲ���0��ʼ
////        m++;
////        stCycleModePara.stCyclePage[i].ucPageNo=pucBuffer[m++];
////    }
////    ucRet=RunPara_SetCycleModePara(&stCycleModePara);
////    if(ucRet==RET_SUCCESS)
////        pucBuffer[0]=CON_NACK_ERR_NORMAL;
////    else
////        pucBuffer[0]=CON_NACK_ERR_NOTEXE;
////    pstDataItem->usDataLen=5;
////    return ucRet;
////}
////
//////F79 �޸�Һ��������ʾ����
////unsigned char Main_Recv_ExtCmd_F79_Set_LCDCtrl(STDATAITEM *pstDataItem,unsigned char ucDAp)
////{
////    unsigned char   ucRet=RET_SUCCESS,i=0;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
////    unsigned char   m=0,ucStart;
////    STCTRLMODEPARA  stCtrlModePara;
////    ucStart=pucBuffer[m++];
////    if(ucStart<=(CONCTRLPAGENUM-16))
////    {
////        stCtrlModePara=RunPara_GetCtrlModePara();
////        stCtrlModePara.usCtrlRemainTime=pucBuffer[m]+0x100*pucBuffer[m+1];  m+=2;
////        for(i=0;i<16;i++)                       //16�̶�
////        {
////            if(pucBuffer[m]==0)
////                stCtrlModePara.stCtrlPage[ucStart+i].ucMPNo=0;
////            else
////                stCtrlModePara.stCtrlPage[ucStart+i].ucMPNo=pucBuffer[m]-1;
////            m++;
////            stCtrlModePara.stCtrlPage[ucStart+i].ucPageNo=pucBuffer[m++];
////        }
////        ucRet=RunPara_SetCtrlModePara(&stCtrlModePara);
////    }else
////        ucRet=RET_ERROR;
////    if(ucRet==RET_SUCCESS)
////        pucBuffer[0]=CON_NACK_ERR_NORMAL;
////    else
////        pucBuffer[0]=CON_NACK_ERR_NOTEXE;
////    pstDataItem->usDataLen=5;
////    return ucRet;
////}
////
//////F80 �޸�Һ��������ʾ����
////unsigned char Main_Recv_ExtCmd_F80_Set_LCDSpec(STDATAITEM *pstDataItem,unsigned char ucDAp)
////{
////    unsigned char   ucRet=RET_SUCCESS;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
////    unsigned char   m=0;
////    STSPECMODEPARA  stSpecModePara;
////    stSpecModePara=RunPara_GetSpecModePara();
////    stSpecModePara.usSpecRemainTime=pucBuffer[m]+0x100*pucBuffer[m+1];  m+=2;
////    if(pucBuffer[m]==0)
////        stSpecModePara.stSpecPage.ucMPNo=0;
////    else
////        stSpecModePara.stSpecPage.ucMPNo=pucBuffer[m]-1;
////    m++;
////    stSpecModePara.stSpecPage.ucPageNo=pucBuffer[m++];
////    ucRet=RunPara_SetSpecModePara(&stSpecModePara);
////    if(ucRet==RET_SUCCESS)
////        pucBuffer[0]=CON_NACK_ERR_NORMAL;
////    else
////        pucBuffer[0]=CON_NACK_ERR_NOTEXE;
////    pstDataItem->usDataLen=5;
////    return ucRet;
////}
//
////F81 ����SIM����pin
//unsigned char Main_Recv_ExtCmd_F81_Sim_StartPin(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_ERROR;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
//  /*  STTERMISCPARA   stTerMiscPara=RunPara_GetTerMiscPara();
//
//    if(pucBuffer[0]==0x55 && pucBuffer[1]==0x55 && stTerMiscPara.usUsePin!=CON_STATUS_YES_2BYTE)
//{
//    stTerMiscPara.usUsePin=CON_STATUS_YES_2BYTE;
//    ucRet=RET_SUCCESS;
//    }else if(pucBuffer[0]==0xAA && pucBuffer[1]==0xAA  && stTerMiscPara.usUsePin!=CON_STATUS_NO_2BYTE)
//  {
//    stTerMiscPara.usUsePin=CON_STATUS_NO_2BYTE;
//    ucRet=RET_SUCCESS;
//  }
//  if(ucRet==RET_SUCCESS)
//    {
//    RunPara_SetTerMiscPara(&stTerMiscPara);
//    RComm_SetStartFlag();
//      pucBuffer[0]=CON_NACK_ERR_NORMAL;
//    }else
//  */      pucBuffer[0]=CON_NACK_ERR_NOTEXE;
//  pstDataItem->usDataLen=5;
//  return ucRet;
//}
//
////F82 �޸�SIM����pin
//unsigned char Main_Recv_ExtCmd_F82_Sim_SetPin(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
//  /*  unsigned char   m=0;
//  STTERMISCPARA   stTerMiscPara;
//  unsigned char   ucPinOld[4];
//
//    if(Check_BCD_Data(pucBuffer[0])==RET_SUCCESS &&
//    Check_BCD_Data(pucBuffer[1])==RET_SUCCESS &&
//    Check_BCD_Data(pucBuffer[2])==RET_SUCCESS &&
//    Check_BCD_Data(pucBuffer[3])==RET_SUCCESS)
//      {
//    stTerMiscPara=RunPara_GetTerMiscPara();
//    ucPinOld[0]=(unsigned char)(pucBuffer[0]>>4);
//    ucPinOld[1]=(unsigned char)(pucBuffer[0]&0x0F);
//    ucPinOld[2]=(unsigned char)(pucBuffer[1]>>4);
//    ucPinOld[3]=(unsigned char)(pucBuffer[1]&0x0F);
//    stTerMiscPara.ucPinNew[0]=(unsigned char)(pucBuffer[2]>>4);
//    stTerMiscPara.ucPinNew[1]=(unsigned char)(pucBuffer[2]&0x0F);
//    stTerMiscPara.ucPinNew[2]=(unsigned char)(pucBuffer[3]>>4);
//    stTerMiscPara.ucPinNew[3]=(unsigned char)(pucBuffer[3]&0x0F);
//    if(stTerMiscPara.ucPinNew[0]==9 && stTerMiscPara.ucPinNew[1]==9 && stTerMiscPara.ucPinNew[2]==9 && stTerMiscPara.ucPinNew[3]==9)
//    {//9999��ʾ����PIN�����޸�
//    if(stTerMiscPara.ucPinOld[0]==ucPinOld[0] && stTerMiscPara.ucPinOld[1]==ucPinOld[1] &&
//    stTerMiscPara.ucPinOld[2]==ucPinOld[2] && stTerMiscPara.ucPinOld[3]==ucPinOld[3])
//    {
//    memcpy(stTerMiscPara.ucPinOld,ucPinOld,4);
//    RunPara_SetTerMiscPara(&stTerMiscPara);
//    RComm_SetStartFlag();
//    pucBuffer[0]=CON_NACK_ERR_NORMAL;
//      }
//  }else
//    {
//    if(stTerMiscPara.ucPinOld[0]==ucPinOld[0] && stTerMiscPara.ucPinOld[1]==ucPinOld[1] &&
//    stTerMiscPara.ucPinOld[2]==ucPinOld[2] && stTerMiscPara.ucPinOld[3]==ucPinOld[3])
//    {
//    RunPara_SetTerMiscPara(&stTerMiscPara);
//    RComm_SetStartFlag();
//    RComm_PinSetFlag();
//      pucBuffer[0]=CON_NACK_ERR_NORMAL;
//    }else
//      pucBuffer[0]=CON_NACK_ERR_NOTEXE;
//    }
//    }else
//  */      pucBuffer[0]=CON_NACK_ERR_NOTEXE;
//  pstDataItem->usDataLen=5;
//  return ucRet;
//}
//
////F83 ��PUK����SIM����pin
//unsigned char Main_Recv_ExtCmd_F83_Sim_PukNewPin(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{//PUKֻ���벻����
//  unsigned char   ucRet=RET_SUCCESS;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
//  /*  unsigned char   m=0;
//  STTERMISCPARA   stTerMiscPara;
//
//    if(Check_BCD_Data(pucBuffer[0])==RET_SUCCESS &&
//    Check_BCD_Data(pucBuffer[1])==RET_SUCCESS &&
//    Check_BCD_Data(pucBuffer[2])==RET_SUCCESS &&
//    Check_BCD_Data(pucBuffer[3])==RET_SUCCESS &&
//    Check_BCD_Data(pucBuffer[4])==RET_SUCCESS &&
//    Check_BCD_Data(pucBuffer[5])==RET_SUCCESS)
//{
//    stTerMiscPara=RunPara_GetTerMiscPara();
//    stTerMiscPara.ucPuk[0]=(unsigned char)(pucBuffer[0]>>4);
//    stTerMiscPara.ucPuk[1]=(unsigned char)(pucBuffer[0]&0x0F);
//    stTerMiscPara.ucPuk[2]=(unsigned char)(pucBuffer[1]>>4);
//    stTerMiscPara.ucPuk[3]=(unsigned char)(pucBuffer[1]&0x0F);
//    stTerMiscPara.ucPuk[4]=(unsigned char)(pucBuffer[2]>>4);
//    stTerMiscPara.ucPuk[5]=(unsigned char)(pucBuffer[2]&0x0F);
//    stTerMiscPara.ucPuk[6]=(unsigned char)(pucBuffer[3]>>4);
//    stTerMiscPara.ucPuk[7]=(unsigned char)(pucBuffer[3]&0x0F);
//    RunPara_SetTerMiscPara(&stTerMiscPara);
//    RComm_SetStartFlag();
//      pucBuffer[0]=CON_NACK_ERR_NORMAL;
//    }else
//      pucBuffer[0]=CON_NACK_ERR_NOTEXE;
//  */  pstDataItem->usDataLen=5;
//  return ucRet;
//}
//
////F84 SIM����pin���״̬
//unsigned char Main_Recv_ExtCmd_F84_Sim_GetPinStat(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS,m=0;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
//  STTERMISCPARA   stTerMiscPara=RunPara_GetTerMiscPara();
//
//  pucBuffer[m++]=stTerMiscPara.stRCommRunInfo.ucPinStat;
//  pstDataItem->usDataLen=m+4;
//  return ucRet;
//}
//
////F85 GSM/GPRS�л�
//unsigned char Main_Recv_ExtCmd_F85_Set_GSM_GRPS(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
//  unsigned char   ucFlag=CON_STATUS_NO;
//
//  STTERMISCPARA   stTerMiscPara;
//
//  stTerMiscPara=RunPara_GetTerMiscPara();
//  if(pucBuffer[0]==0xFF)//��ȡͨ������
//  {
//      if(stTerMiscPara.ucRcommBaseType==CON_TC35COMMTYPE_TCP)
//          pucBuffer[0]=0x22;
//      else if(stTerMiscPara.ucRcommBaseType==CON_TC35COMMTYPE_UDP)
//          pucBuffer[0]=0x33;
//      else
//          pucBuffer[0]=0x11;
//  }
//  else//����ͨ������
//  {
//      if(pucBuffer[0]==0x11)          //GSM
//      {
//          stTerMiscPara.ucRcommBaseType=CON_TC35COMMTYPE_GSM;
//          ucFlag=CON_STATUS_YES;
//      }else if(pucBuffer[0]==0x22)    //GPRS TCP
//      {
//          stTerMiscPara.ucRcommBaseType=CON_TC35COMMTYPE_TCP;
//          ucFlag=CON_STATUS_YES;
//      }else if(pucBuffer[0]==0x33)    //GPRS UDP
//  {
//          stTerMiscPara.ucRcommBaseType=CON_TC35COMMTYPE_UDP;
//          ucFlag=CON_STATUS_YES;
//  }
//      if(ucFlag==CON_STATUS_YES)
//      {
//
//          RunPara_SetTerMiscPara(&stTerMiscPara);
//          Main_RComm_Init_LoginSleepCount();
////            RComm_SetStartFlag();
//  pucBuffer[0]=CON_NACK_ERR_NORMAL;
//      }else
//          pucBuffer[0]=CON_NACK_ERR_NOTEXE;
//  }
//  pstDataItem->usDataLen=5;
//  return ucRet;
//}
//
////F86 �����ʿ��ƻָ�ʱ�䳤��
//unsigned char Main_Recv_ExtCmd_F86_Get_PowerCtrl_TimeRecType(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
//  STTERMISCPARA   stTerMiscPara;
//  stTerMiscPara=RunPara_GetTerMiscPara();
//  pucBuffer[0]=stTerMiscPara.ucPowerCtrlTimeRecMinute;
//  pstDataItem->usDataLen=5;
//  return ucRet;
//}
//
////F87 д���ʿ��ƻָ�ʱ�䳤��
//unsigned char Main_Recv_ExtCmd_F87_Set_PowerCtrl_TimeRecType(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
//
//  STTERMISCPARA   stTerMiscPara;
//  stTerMiscPara=RunPara_GetTerMiscPara();
//  stTerMiscPara.ucPowerCtrlTimeRecMinute=pucBuffer[0];
//  if(stTerMiscPara.ucPowerCtrlTimeRecMinute>CON_POWERCTRL_TIMEREC_MAXMIN)
//      stTerMiscPara.ucPowerCtrlTimeRecMinute=CON_POWERCTRL_TIMEREC_END;
//  RunPara_SetTerMiscPara(&stTerMiscPara);
//          pucBuffer[0]=CON_NACK_ERR_NORMAL;
//  pstDataItem->usDataLen=5;
//  return ucRet;
//      }
//
////F89 �����ʿ���Խ��ȷ��ʱ��
//unsigned char Main_Recv_ExtCmd_F89_Get_PowerCtrl_DelayTime(STDATAITEM *pstDataItem,unsigned char ucDAp)
//  {
//  unsigned char   ucRet=RET_SUCCESS;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
////
////    unsigned char   ucSumGNo=0;
////    STPOWERCTRLHOLDTIME stPowerHoldTime;
////    if(ucSumGNo<CON_SUMGROUP_NUM)
////        ucSumGNo=ucDAp;
////    stPowerHoldTime=RunPara_GetPowCtrlHoldTime(ucSumGNo);
////    pucBuffer[0]=(unsigned char)(stPowerHoldTime.usJumpTime[0]/120);
////    pstDataItem->usDataLen=5;
//  return ucRet;
//  }
//
////F90 д���ʿ���Խ��ȷ��ʱ��
//unsigned char Main_Recv_ExtCmd_F90_Set_PowerCtrl_DelayTime(STDATAITEM *pstDataItem,unsigned char ucDAp)
//  {
//  unsigned char   ucRet=RET_SUCCESS;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
////    unsigned char   i=0;
////    STPOWERCTRLHOLDTIME stPowerHoldTime;
////    unsigned char   ucSumGNo=0;
////    if(ucSumGNo<CON_SUMGROUP_NUM)
////        ucSumGNo=ucDAp;
////    stPowerHoldTime=RunPara_GetPowCtrlHoldTime(ucSumGNo);
////    for(i=0;i<CON_PARA_SWITCHOUT_NUM;i++)
////    {
////        stPowerHoldTime.usJumpTime[i]=pucBuffer[0]*120;         //��λ1����ת��Ϊ��λ0.5��
////        stPowerHoldTime.usRevTime[i]=pucBuffer[0]*120;
////    }
////    RunPara_SetPowCtrlHoldTime(&stPowerHoldTime,ucSumGNo);
////    pucBuffer[0]=CON_NACK_ERR_NORMAL;
////    pstDataItem->usDataLen=5;
//  return ucRet;
//  }
//
////F91 ���Զ����������
//unsigned char Main_Recv_ExtCmd_F91_Get_AdjPara(STDATAITEM *pstDataItem,unsigned char ucDAp)
//  {
//  unsigned char   ucRet=RET_SUCCESS;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
////    unsigned short usTmp;
////    STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
////    usTmp=Get_CtrlLockTimer();
////    if(usTmp>255)
////        pucBuffer[0]=0xFF;
////    else
////        pucBuffer[0]=255-(unsigned char)usTmp;
////    pucBuffer[1]=stTerAdjPara.ucSumGFromZero;
////    pucBuffer[2]=stTerAdjPara.ucEnerBuyUnit;
////    pucBuffer[3]=stTerAdjPara.ucDownProgramSource;
////    pucBuffer[4]=stTerAdjPara.ucJob1HourDataExt;
////    pucBuffer[5]=stTerAdjPara.ucMPNoFromZero;
////    pucBuffer[6]=stTerAdjPara.ucLoginNeedAck;
////    pucBuffer[7]=stTerAdjPara.ucViewData_1;
////    pucBuffer[8]=stTerAdjPara.ucBeatHeartNeedAck;
////    pucBuffer[9]=stTerAdjPara.ucPulseDemandCyc;
////    pucBuffer[10]=0x00;
////    pucBuffer[11]=stTerAdjPara.ucRunMode;
////    pstDataItem->usDataLen=16;
//  return ucRet;
//}
//
////F92 д�Զ����������
//unsigned char Main_Recv_ExtCmd_F92_Set_AdjPara(STDATAITEM *pstDataItem,unsigned char ucDAp)
//      {
//  unsigned char   ucRet=RET_SUCCESS;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
////    unsigned short usTmp;
////    STTERADJPARA stTerAdjPara;
////    unsigned char   ucOldMode;
////    stTerAdjPara=RunPara_GetTerAdjPara();
////    if(pucBuffer[0]==0x66 && pucBuffer[1]==0x66)
////    {
////        if(pucBuffer[2]==0xFF)
////        {//��ѯ
////            if(stTerAdjPara.ucMeterFEActive==0x55)
////                pucBuffer[0]=stTerAdjPara.ucMeterFENum;
////            else
////                pucBuffer[0]=1;
////        }else
////        {//����
////            stTerAdjPara.ucMeterFEActive=0x55;
////            stTerAdjPara.ucMeterFENum=pucBuffer[2];
////            pucBuffer[0]=CON_NACK_ERR_NORMAL;
////        }
////    }else
////    {
////        stTerAdjPara.ucPowerOnCtrlLockLeftTime=pucBuffer[0];
////        usTmp=CONPOWERON_CTRLLOCK-stTerAdjPara.ucPowerOnCtrlLockLeftTime;
////        Set_CtrlLockTimer(usTmp);
////        if(pucBuffer[1]==1)
////            stTerAdjPara.ucSumGFromZero=1;
////        else
////            stTerAdjPara.ucSumGFromZero=0;
////        if(pucBuffer[2]==0)
////            stTerAdjPara.ucEnerBuyUnit=0;
////        else
////            stTerAdjPara.ucEnerBuyUnit=1;
////        if(pucBuffer[3]==CON_TERDP_SOURCE_RS232 || pucBuffer[3]==CON_TERDP_SOURCE_REMOTE)
////            stTerAdjPara.ucDownProgramSource=pucBuffer[3];
////        else
////            stTerAdjPara.ucDownProgramSource=CON_TERDP_SOURCE_INVALID;
////        if(pucBuffer[4]==0x55)
////            stTerAdjPara.ucJob1HourDataExt=CON_STATUS_YES;
////        else
////            stTerAdjPara.ucJob1HourDataExt=CON_STATUS_NO;
////        if(pucBuffer[5]==1)
////            stTerAdjPara.ucMPNoFromZero=1;
////        else
////            stTerAdjPara.ucMPNoFromZero=0;
////        if(pucBuffer[6]==0x55)
////            stTerAdjPara.ucLoginNeedAck=CON_STATUS_YES;
////        else
////            stTerAdjPara.ucLoginNeedAck=CON_STATUS_NO;
////        if(pucBuffer[7]==1)
////            stTerAdjPara.ucViewData_1=1;
////        else
////            stTerAdjPara.ucViewData_1=0;
////        if(pucBuffer[8]==0x55)
////            stTerAdjPara.ucBeatHeartNeedAck=CON_STATUS_YES;
////        else
////            stTerAdjPara.ucBeatHeartNeedAck=CON_STATUS_NO;
////        if(pucBuffer[9]>=1 && pucBuffer[9]<=15)
////            stTerAdjPara.ucPulseDemandCyc=pucBuffer[9];
////        ucOldMode=stTerAdjPara.ucRunMode;
////        stTerAdjPara.ucRunMode=pucBuffer[11];
////    }
////    RunPara_SetTerAdjPara(&stTerAdjPara);
////
////    if((ucOldMode==0xA5 && pucBuffer[11]!=0xA5)
////        || (ucOldMode!=0xA5 && pucBuffer[11]==0xA5))
////    {
////        CTask_Rebuild_Task();
////    }
////    pucBuffer[0]=CON_NACK_ERR_NORMAL;
////    pstDataItem->usDataLen=5;
//  return ucRet;
//}
//
////F93 ��ȡ�ն˵�ѹ��λС��������λС��
//unsigned char Main_Recv_ExtCmd_F93_Get_TerVoltCurr(STDATAITEM *pstDataItem,unsigned char ucDAp)
//          {
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
//  unsigned short  m=0;
//  unsigned char   ucTerMPNo=RunPara_GetTer_MPNo();
//  STHISVAL stHisVal;
//  stHisVal=Get_History_V_Disp_byMPNo(ucTerMPNo,0,1);
//  m=SetMainComm_Buf_lVal_3B(pucBuffer,m,stHisVal.lVal);
//  stHisVal=Get_History_V_Disp_byMPNo(ucTerMPNo,0,2);
//  m=SetMainComm_Buf_lVal_3B(pucBuffer,m,stHisVal.lVal);
//  stHisVal=Get_History_V_Disp_byMPNo(ucTerMPNo,0,3);
//  m=SetMainComm_Buf_lVal_3B(pucBuffer,m,stHisVal.lVal);
//  stHisVal=Get_History_I_Disp_byMPNo(ucTerMPNo,0,1);
//  m=SetMainComm_Buf_lVal_3B(pucBuffer,m,stHisVal.lVal);
//  stHisVal=Get_History_I_Disp_byMPNo(ucTerMPNo,0,2);
//  m=SetMainComm_Buf_lVal_3B(pucBuffer,m,stHisVal.lVal);
//  stHisVal=Get_History_I_Disp_byMPNo(ucTerMPNo,0,3);
//  m=SetMainComm_Buf_lVal_3B(pucBuffer,m,stHisVal.lVal);
//  pstDataItem->usDataLen=m+4;
//  return ucRet;
//}
//
////F94 �����ʼ��
//unsigned char Main_Recv_ExtCmd_F94_Init_CTask(STDATAITEM *pstDataItem,unsigned char ucDAp)
//              {
//  unsigned char ucRet=RET_SUCCESS;
////    unsigned char *pucBuffer;
////    unsigned char ucTmp,i,ucFlag;
////    unsigned char ucMPNo,ucMPType;
////    STMETERPARA   stMeterPara;
////    unsigned short  m=0;
////
////    pucBuffer=pstDataItem->ucDataContent+4;
////    if(pucBuffer[m]==0x55 && pucBuffer[m+1]==0x66 && pucBuffer[m+2]==0x77 && pucBuffer[m+3]==0x88)
////    {
////        m+=4;
////        ucTmp=pucBuffer[m++];               //���ܱ�
////        for(i=0;i<8;i++)
////        {
////            ucFlag=(unsigned char)(1<<i);
////            if((ucTmp&ucFlag)==ucFlag)
////            {
////                ucMPNo=RunPara_GetMeter_MPNo(i);
////                ucMPType=RunPara_GetMPType(ucMPNo);
////                if(ucMPType==CON_MPTYPE_METER)
////                {
////                    stMeterPara=RunPara_GetMeterPara(i);
////                    if(stMeterPara.ucProtocol==CON_PROTOCOL_INVALID)
////                        DeleteDefaultMeterTask(i);
////                    else
////                        SetDefaultMeterTask(i);
////                }
////            }
////        }
////        ucTmp=pucBuffer[m++];               //�ܼ�
////        if((ucTmp&0x01)==0x01)
////            SetDefaultSumGroupTask();
////        ucTmp=pucBuffer[m++];               //����
////        if((ucTmp&0x01)==0x01)
////            SetDefaultPulseTask(0);
////        if((ucTmp&0x02)==0x02)
////            SetDefaultPulseTask(1);
////        ucTmp=pucBuffer[m++];               //�ն�
////        if((ucTmp&0x01)==0x01)
////            SetDefaultTerTask();
////        ucTmp=pucBuffer[m++];               //��������
////    }else
////        ucRet=RET_ERROR;
////    if(ucRet==RET_SUCCESS)
////        pucBuffer[0]=CON_NACK_ERR_NORMAL;
////    else
////        pucBuffer[0]=CON_NACK_ERR_NOTEXE;
////    pstDataItem->usDataLen=5;
//  return ucRet;
//              }
//
////F95 8025ʱ�ӽ���  32768.00����λС��
//unsigned char Main_Recv_ExtCmd_F95_Adj_RX8025(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
//  /*
//  unsigned char   ulTmp;
//  if(Check_BCD_Data(pucBuffer[0])==RET_SUCCESS &&
//      Check_BCD_Data(pucBuffer[1])==RET_SUCCESS &&
//      Check_BCD_Data(pucBuffer[2])==RET_SUCCESS &&
//      Check_BCD_Data(pucBuffer[3])==RET_SUCCESS)
//  {
//      memcpy((unsigned char *)(&ulTmp),pucBuffer,4);
//      RX8025_Set_DOffset(ulTmp);
//  }else
//      ucRet=RET_ERROR;
//  */
//  pucBuffer[0]=CON_NACK_ERR_NORMAL;
//
//  pstDataItem->usDataLen=5;
//  return ucRet;
//          }
//
////F96 Һ���Աȶ�����
//unsigned char Main_Recv_ExtCmd_F96_Set_LCDContrast(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
////    if(pucBuffer[0]==0xFF)
////    {
////        pucBuffer[0]=LCD_Get_Contrast();
////    }else
////    {
////        LCD_Set_Contrast(pucBuffer[0]);
////        pucBuffer[0]=CON_NACK_ERR_NORMAL;
////    }
////    pstDataItem->usDataLen=5;
//  return ucRet;
//      }
////F97 ���ñ���û���������
//unsigned char Main_Recv_ExtCmd_F97_Set_MeterCommName(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
//  unsigned char   ucMeterNo=0;
//  STMETERPARA     stMeterPara;
//
//  if(ucDAp<CON_METER_NUM)
//  {
//      ucMeterNo=ucDAp;
//      stMeterPara=RunPara_GetMeterPara(ucMeterNo);
//      memcpy(stMeterPara.stCommName.ucMeterCommName,pucBuffer,CON_METER_NAMELEN);
//      memcpy(stMeterPara.stPassword.ucMeterPass,(pucBuffer+CON_METER_NAMELEN),CON_METER_NAMELEN);
//      ucRet=RunPara_SetMeterPara(ucMeterNo,&stMeterPara);
//  }else
//      ucRet=RET_ERROR;
//  if(ucRet==RET_SUCCESS)
//      pucBuffer[0]=CON_NACK_ERR_NORMAL;
//  else
//      pucBuffer[0]=CON_NACK_ERR_NOTEXE;
//  pstDataItem->usDataLen=5;
//  return ucRet;
//}
//
////F98 ��ѯͨѶ���ƺ�����
//unsigned char Main_Recv_ExtCmd_F98_Get_MeterCommName(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
//  unsigned char   ucMeterNo=0;
//  STMETERPARA     stMeterPara;
//
//  if(ucDAp<CON_METER_NUM)
//  {
//      ucMeterNo=ucDAp;
//      stMeterPara=RunPara_GetMeterPara(ucMeterNo);
//      memcpy(pucBuffer,stMeterPara.stCommName.ucMeterCommName,CON_METER_NAMELEN);
//      memcpy(pucBuffer+CON_METER_NAMELEN,stMeterPara.stPassword.ucMeterPass,CON_METER_NAMELEN);
//  }else
//  {
//      memset(pucBuffer,0xEE,CON_METER_NAMELEN+CON_METER_NAMELEN);
//  }
//  pstDataItem->usDataLen=CON_METER_NAMELEN+CON_METER_NAMELEN+4;
//
//  return ucRet;
//  }
//
////F99 ����GPRS�����PDP�û���������
//unsigned char Main_Recv_ExtCmd_F99_Set_GprsPDPPara(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
//  STTERMISCPARA   stTerMiscPara=RunPara_GetTerMiscPara();
//
//  memcpy(&(stTerMiscPara.stRCommPara.stGprsPDPPara.ucPDPUseFlag),pucBuffer,1);
//  memcpy(stTerMiscPara.stRCommPara.stGprsPDPPara.ucPDPUserName,pucBuffer+1,CON_MAX_GPRSPDPUSERNAME_LEN);
//  memcpy(stTerMiscPara.stRCommPara.stGprsPDPPara.ucPDPPassWord,pucBuffer+1+CON_MAX_GPRSPDPUSERNAME_LEN,CON_MAX_GPRAPDPPASSWORE_LEN);
//  ucRet=RunPara_SetTerMiscPara(&stTerMiscPara);
//
//  if(ucRet==RET_SUCCESS)
//      pucBuffer[0]=CON_NACK_ERR_NORMAL;
//  else
//      pucBuffer[0]=CON_NACK_ERR_NOTEXE;
//  pstDataItem->usDataLen=5;
//  return ucRet;
//}
//
////F100 ��ȡGPRS�����PDP�û���������
//unsigned char Main_Recv_ExtCmd_F100_Get_GprsPDPPara(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
//  STTERMISCPARA   stTerMiscPara=RunPara_GetTerMiscPara();
//
//  memcpy(pucBuffer,&(stTerMiscPara.stRCommPara.stGprsPDPPara),sizeof(stTerMiscPara.stRCommPara.stGprsPDPPara));
//
//  pstDataItem->usDataLen=sizeof(stTerMiscPara.stRCommPara.stGprsPDPPara)+4;
//
//  return ucRet;
//}
////F101 ����GPRS�������ʱ��
//unsigned char Main_Recv_ExtCmd_F101_Set_GprsRelinkTime(STDATAITEM *pstDataItem,unsigned char ucDAp)
//  {
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
//  STTERMISCPARA   stTerMiscPara=RunPara_GetTerMiscPara();
//
//  stTerMiscPara.stRCommPara.stGprsRelinkTime.usRelinkTime=(unsigned short)(pucBuffer[0]+pucBuffer[1]*0x100);
//  ucRet=RunPara_SetTerMiscPara(&stTerMiscPara);
//
//  if(ucRet==RET_SUCCESS)
//      pucBuffer[0]=CON_NACK_ERR_NORMAL;
//  else
//      pucBuffer[0]=CON_NACK_ERR_NOTEXE;
//  pstDataItem->usDataLen=5;
//  return ucRet;
//}
////F102 ��ȡGPRS�������ʱ��
//unsigned char Main_Recv_ExtCmd_F102_Get_GprsRelinkTime(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
//  unsigned short  usRelinkTime=0;
//  STTERMISCPARA   stTerMiscPara=RunPara_GetTerMiscPara();
//
//  usRelinkTime=stTerMiscPara.stRCommPara.stGprsRelinkTime.usRelinkTime;
//  pucBuffer[0]=(unsigned char)usRelinkTime;
//  pucBuffer[1]=(unsigned char)(usRelinkTime/0x100);
//
//  pstDataItem->usDataLen=2+4;
//
//  return ucRet;
//}
//unsigned char Main_Recv_ExtCmd_F109_Set_TerWorkMode(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
////    STCTCHECKPARA   stCTCheckPara=RunPara_GetCTCheckPara();
////
////    if( (pucBuffer[0]==CON_WORKMODE_NORMAL)||(pucBuffer[0]==CON_WORKMODE_TEST) )
////    {
////        stCTCheckPara.ucTerWorkMode=pucBuffer[0];
////        ucRet=RunPara_SetCTCheckPara(&stCTCheckPara);
////    }
////    else
////        ucRet=RET_ERROR;
////
////    if(ucRet==RET_SUCCESS)
////        pucBuffer[0]=CON_NACK_ERR_NORMAL;
////    else
////        pucBuffer[0]=CON_NACK_ERR_NOTEXE;
////    pstDataItem->usDataLen=5;
//  return ucRet;
//
//}
//unsigned char Main_Recv_ExtCmd_F110_Get_TerWorkMode(STDATAITEM *pstDataItem,unsigned char ucDAp)
//  {
//  unsigned char   ucRet=RET_SUCCESS;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
////    unsigned char   ucTerWorkMode=CON_WORKMODE_NORMAL;
////    STCTCHECKPARA   stCTCheckPara=RunPara_GetCTCheckPara();
////
////    ucTerWorkMode=stCTCheckPara.ucTerWorkMode;
////    pucBuffer[0]=ucTerWorkMode;
////
////    pstDataItem->usDataLen=1+4;
//
//  return ucRet;
//  }
//unsigned char Main_Recv_ExtCmd_F111_Set_CTType(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
////    STCTCHECKPARA   stCTCheckPara=RunPara_GetCTCheckPara();
////
////    if( (pucBuffer[0]==CON_CTTYPE_D)||(pucBuffer[0]==CON_CTTYPE_C) )
////    {
////        stCTCheckPara.ucCTType=pucBuffer[0];
////        ucRet=RunPara_SetCTCheckPara(&stCTCheckPara);
////    }
////    else
////        ucRet=RET_ERROR;
////
////    if(ucRet==RET_SUCCESS)
////        pucBuffer[0]=CON_NACK_ERR_NORMAL;
////    else
//      pucBuffer[0]=CON_NACK_ERR_NOTEXE;
//  pstDataItem->usDataLen=5;
//  return ucRet;
//
//}
//unsigned char Main_Recv_ExtCmd_F112_Get_CTType(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
////    unsigned char   ucRet=RET_SUCCESS;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
////    unsigned char   ucCTType=CON_CTTYPE_D;
////    STCTCHECKPARA   stCTCheckPara=RunPara_GetCTCheckPara();
//
////    ucCTType=stCTCheckPara.ucCTType;
////    pucBuffer[0]=ucCTType;
//
////    pstDataItem->usDataLen=1+4;
//
////    return ucRet;
//  unsigned char   ucRet=RET_SUCCESS,i;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
//  unsigned short  usPluseCount[4];
//
//  for(i=0;i<4;i++)
//      {
////        usPluseCount[i] = Get_PulseCount_Num(i);
////        usPluseCount[i] = (unsigned short)HEX_TO_BCD((unsigned long)usPluseCount[i], 2);
//      pucBuffer[2*i+1] = usPluseCount[i] % 0x100;
//      pucBuffer[2*i]   = usPluseCount[i] / 0x100;
//  }
//
//  pstDataItem->usDataLen=8+4;
//
//  return ucRet;
//      }
//unsigned char Main_Recv_ExtCmd_F113_Set_SlfStudyFlag(STDATAITEM *pstDataItem,unsigned char ucDAp)
//      {
//  unsigned char   ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
////    STCTCHECKPARA   stCTCheckPara=RunPara_GetCTCheckPara();
//
////    if( (pucBuffer[0]==CON_AVALIBLE_FLAG)||(pucBuffer[0]==CON_INVALIBLE_FLAG) )
////    {
////        stCTCheckPara.ucSlfStudyFlag=pucBuffer[0];
////        ucRet=RunPara_SetCTCheckPara(&stCTCheckPara);
////    }
////    else
////        ucRet=RET_ERROR;
//
////    if(ucRet==RET_SUCCESS)
////        pucBuffer[0]=CON_NACK_ERR_NORMAL;
////    else
//  pucBuffer[0]=CON_NACK_ERR_NOTEXE;
//  pstDataItem->usDataLen=5;
//  return ucRet;
//
//  }
//unsigned char Main_Recv_ExtCmd_F114_Get_SlfStudyFlag(STDATAITEM *pstDataItem,unsigned char ucDAp)
//  {
//  unsigned char   ucRet=RET_SUCCESS;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
////    unsigned char   ucSlfStudyFlag=CON_AVALIBLE_FLAG;
////    STCTCHECKPARA   stCTCheckPara=RunPara_GetCTCheckPara();
//
////    ucSlfStudyFlag=stCTCheckPara.ucSlfStudyFlag;
////    pucBuffer[0]=ucSlfStudyFlag;
//
////    pstDataItem->usDataLen=1+4;
//
//  return ucRet;
//}
//unsigned char Main_Recv_ExtCmd_F115_Set_Cir2OpenLimitVal(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
////    STCTCHECKPARA   stCTCheckPara=RunPara_GetCTCheckPara();
////    /*
////    stCTCheckPara.stCTLimitValue.stOpenCircuit2.ucValueA=pucBuffer[0]+pucBuffer[1]*0x100;
////    stCTCheckPara.stCTLimitValue.stOpenCircuit2.ucValueB=pucBuffer[2]+pucBuffer[3]*0x100;
////    stCTCheckPara.stCTLimitValue.stOpenCircuit2.ucValueC=pucBuffer[4]+pucBuffer[5]*0x100;
////
////    ucRet=RunPara_SetCTCheckPara(&stCTCheckPara);
////    */
////    if(ucRet==RET_SUCCESS)
////        pucBuffer[0]=CON_NACK_ERR_NORMAL;
////    else
////        pucBuffer[0]=CON_NACK_ERR_NOTEXE;
////    pstDataItem->usDataLen=5;
//  return ucRet;
//
//}
//unsigned char Main_Recv_ExtCmd_F116_Get_Cir2OpenLimitVal(STDATAITEM *pstDataItem,unsigned char ucDAp)
//  {
//  unsigned char   ucRet=RET_SUCCESS;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4,ucPos=0;
////    STCTCHECKPARA   stCTCheckPara=RunPara_GetCTCheckPara();
////    pucBuffer[ucPos++]=stCTCheckPara.stCTLimitValue.stOpenCircuit2.ucValueA;
////    pucBuffer[ucPos++]=stCTCheckPara.stCTLimitValue.stOpenCircuit2.ucValueB;
////    pucBuffer[ucPos++]=stCTCheckPara.stCTLimitValue.stOpenCircuit2.ucValueC;
////    pstDataItem->usDataLen=ucPos+4;
//  return ucRet;
//  }
//unsigned char Main_Recv_ExtCmd_F117_Set_Cir1ShortLimitVal(STDATAITEM *pstDataItem,unsigned char ucDAp)
//  {
//  unsigned char   ucRet=RET_SUCCESS;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4;
////    STCTCHECKPARA   stCTCheckPara=RunPara_GetCTCheckPara();
////    /*
////    stCTCheckPara.stCTLimitValue.stShortCircuit1.usValueA=pucBuffer[0]+pucBuffer[1]*0x100;
////    stCTCheckPara.stCTLimitValue.stShortCircuit1.usValueB=pucBuffer[2]+pucBuffer[3]*0x100;
////    stCTCheckPara.stCTLimitValue.stShortCircuit1.usValueC=pucBuffer[4]+pucBuffer[5]*0x100;
////
////    ucRet=RunPara_SetCTCheckPara(&stCTCheckPara);
////    */
////    if(ucRet==RET_SUCCESS)
////        pucBuffer[0]=CON_NACK_ERR_NORMAL;
////    else
////        pucBuffer[0]=CON_NACK_ERR_NOTEXE;
////    pstDataItem->usDataLen=5;
//  return ucRet;
//
//  }
//unsigned char Main_Recv_ExtCmd_F118_Get_Cir1ShortLimitVal(STDATAITEM *pstDataItem,unsigned char ucDAp)
//  {
//  unsigned char   ucRet=RET_SUCCESS;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4,ucPos=0;
////    STCTCHECKPARA   stCTCheckPara=RunPara_GetCTCheckPara();
////    pucBuffer[ucPos++]=stCTCheckPara.stCTLimitValue.stShortCircuit1.ucValueA;
////    pucBuffer[ucPos++]=stCTCheckPara.stCTLimitValue.stShortCircuit1.ucValueB;
////    pucBuffer[ucPos++]=stCTCheckPara.stCTLimitValue.stShortCircuit1.ucValueC;
////    pstDataItem->usDataLen=ucPos+4;
//  pstDataItem->usDataLen=4;
//  return ucRet;
//  }
//
//unsigned char Main_Recv_ExtCmd_F119_Get_CTCheckVal(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char   ucRet=RET_SUCCESS;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4,ucPos=0;
////    STCTVALUE       stCTCheckVal=GetCTCheckVal();
////    pucBuffer[ucPos++]=(unsigned char)stCTCheckVal.ucValueA;
////    pucBuffer[ucPos++]=(unsigned char)stCTCheckVal.ucValueB;
////    pucBuffer[ucPos++]=(unsigned char)stCTCheckVal.ucValueC;
////    pstDataItem->usDataLen=ucPos+4;
//  return ucRet;
//}
////�Զ��������AES������Կ����
//unsigned char Main_Recv_ExtCmd_F120_Set_AESKey(STDATAITEM *pstDataItem,unsigned char ucDAp)
//  {
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned char *pucBuffer = pstDataItem->ucDataContent+4,ucPos=0;
//  STAESKEY    stAesKey;
//  stAesKey.ucAesLength    = pucBuffer[ucPos++];
//  memcpy(stAesKey.ucAesKey,&pucBuffer[ucPos],stAesKey.ucAesLength);
//  RunPara_SetAesKey(&stAesKey);
//  return ucRet;
//  }
////�Զ��������AES������Կ��ѯ
//unsigned char Main_Recv_ExtCmd_F121_Get_AESKey(STDATAITEM *pstDataItem,unsigned char ucDAp)
//  {
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned char   *pucBuffer = pstDataItem->ucDataContent+4,ucPos=0;
//  STAESKEY    stAesKey = RunPara_GetAesKey();
//  pucBuffer[ucPos++]=stAesKey.ucAesLength;
//  memcpy(&pucBuffer[ucPos],stAesKey.ucAesKey,stAesKey.ucAesLength);
//  pstDataItem->usDataLen=stAesKey.ucAesLength+5;
//  return ucRet;
//}
////���ý�����߱���.
//unsigned char Main_Recv_ExtCmd_F122_Get_KeepStatus(STDATAITEM *pstDataItem,unsigned char ucDAp)
//      {
//  unsigned char ucRet=RET_SUCCESS;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4,ucPos=0;
////    unsigned char ucFlag = 0;
////    if(pucBuffer[0]!=0xFF&&pucBuffer[0]!=0x11&&pucBuffer[0]!=0x22)
////        pucBuffer[0] = CON_NACK_ERR_NOTEXE;
////    else{
////        if(pucBuffer[0]==0xFF)
////        {
////            ucFlag = RunPara_GetIsCheckGprs();
////            if(ucFlag == CON_INVALIBLE_FLAG)
////                pucBuffer[0] = 0x22;
////            else
////                pucBuffer[0] = 0x11;
////        }
////        else
////        {
////            if(pucBuffer[0]==0x22)
////                RunPara_SetGprsCheck(CON_INVALIBLE_FLAG);
////            else
////                RunPara_SetGprsCheck(CON_AVALIBLE_FLAG);
////            pucBuffer[0] = CON_NACK_ERR_NORMAL;
////        }
////    }
////    pstDataItem->usDataLen=5;
//  return ucRet;
//      }
//unsigned char Main_Recv_ExtCmd_F123_Get_Temperature(STDATAITEM *pstDataItem,unsigned char ucDAp)
//{
//  unsigned char ucRet=RET_SUCCESS;
////    unsigned char   *pucBuffer = pstDataItem->ucDataContent+4,ucPos=0;
////    unsigned short ustemp;
////
////    if(ucDAp==1)
////    {
////        LCD_test_powerctrl();               //test
////    }
////    if(ucDAp==2)
////    {
////        LCD_Init_Hard();                    //test
////    }
////
////    ustemp = Lm75_Get_Temperature();
////
////    pucBuffer[0] = (unsigned char)ustemp;
////    pucBuffer[1] =(unsigned char)(ustemp>>8);
////    pstDataItem->usDataLen=6;
//  return ucRet;
//  }
//
////F124 ��У��EEPROMֵ
//unsigned char Main_Recv_ExtCmd_F124_Get_Ade7758_Para(STDATAITEM *pstDataItem,unsigned char ucDAp)
//  {
//  unsigned char ucRet=RET_SUCCESS;
////    unsigned char *pucBuffer;
////    unsigned char ucDataLen,m=0;
////    unsigned char ucMeter_Para_Addr,ucIIC_Addr;
////
////    pucBuffer=pstDataItem->ucDataContent+4;
////
////    ucMeter_Para_Addr = pucBuffer[0];
////
////    pucBuffer=pstDataItem->ucDataContent+5;
////    if(ucMeter_Para_Addr == CON_ADEPOWERTYPE_ADDR)
////    {
////        if(1 != IICRevByte(CON_ADEPOWERTYPE_ADDR, pucBuffer, 1))
////            pucBuffer[0] = CON_POWER_INVALID;
////        pucBuffer[1] = 0xbb;
////        pucBuffer[2] = 0xbb;
////        pucBuffer[3] = 0xbb;
////        pstDataItem->usDataLen = 9;
////    }
////    else if(ucMeter_Para_Addr == 0x15)
////    {
////        if(1 != IICRevByte(CON_WAVEMODE_ADDR, pucBuffer, 1))
////            pucBuffer[0] = CON_POWER_INVALID;
////        pucBuffer[1] = 0xbb;
////        pucBuffer[2] = 0xbb;
////        pucBuffer[3] = 0xbb;
////        pstDataItem->usDataLen = 9;
////    }
////    else
////    {
////        ucIIC_Addr = ADEGetParamAddr(ucMeter_Para_Addr,0,CON_POWER_INVALID,NULL);
////        IICRevByte(ucIIC_Addr, pucBuffer,4);
////        pstDataItem->usDataLen = 9;
////    }
//  return ucRet;
//
//  }
//
////F125 ����У��EEPROMֵ
//unsigned char Main_Recv_ExtCmd_F125_Set_Ade7758_Para(STDATAITEM *pstDataItem,unsigned char ucDAp)
//  {
//  unsigned char ucRet=RET_SUCCESS;
////    unsigned char *pucBuffer;
////    unsigned char ucDataLen,m=0;
////    unsigned char ucMeter_Para_Addr,ucIIC_Addr;
////    unsigned char ucTmp[4];
////
////    pucBuffer=pstDataItem->ucDataContent+4;
////
////    ucMeter_Para_Addr = pucBuffer[0];
////
////    memcpy(ucTmp , pucBuffer+1,4);
////    ucTmp[2] = 0;
////    ucTmp[3] = (unsigned char)(ucTmp[0] + ucTmp[1]);
////
////    if(ucMeter_Para_Addr == CON_ADEPOWERTYPE_ADDR)
////    {
////        IICSendByte(CON_ADEPOWERTYPE_ADDR , ucTmp , 1);
////    }
////    else if(ucMeter_Para_Addr == 0x15)
////    {
////        if((ucTmp[0] == 0) || (ucTmp[0] == 0x80))
////        {
////            IICSendByte(CON_WAVEMODE_ADDR, ucTmp , 1);
////        }
////    }
////    else
////    {
////        ucIIC_Addr = ADEGetParamAddr(ucMeter_Para_Addr,0,CON_POWER_INVALID,NULL);
////        IICSendByte(ucIIC_Addr, ucTmp,4);
////        ucIIC_Addr = ADEGetParamAddr(ucMeter_Para_Addr,1,CON_POWER_INVALID,NULL);
////        IICSendByte(ucIIC_Addr, ucTmp,4);
////        ADEReloadParam();
////    }
////    if(ucRet==RET_SUCCESS)
////        pucBuffer[0]=CON_NACK_ERR_NORMAL;
////    else
////        pucBuffer[0]=CON_NACK_ERR_NOTEXE;
////    pstDataItem->usDataLen=5;
//  return ucRet;
//}
////Һ�������¼����
//unsigned char Main_Recv_ExtCmd_F126_Para(STDATAITEM *pstDataItem,unsigned char ucDAp)
//  {
//  unsigned char ucRet=RET_SUCCESS;
////    unsigned char *pucBuffer;
////    unsigned char ucDataLen,m=0;
////    STTERAUXPARA stTerAuxPara = RunPara_GetTerAuxPara();
////
////    pucBuffer=pstDataItem->ucDataContent+4;
////    if(pucBuffer[0] == 0xFF)
////    {
////        memcpy(pucBuffer,stTerAuxPara.ucLoginPass,2);
////        pstDataItem->usDataLen = 6;
////    }else
////    {
////        memcpy(stTerAuxPara.ucLoginPass,pucBuffer,2);
////        RunPara_SetTerAuxPara(&stTerAuxPara);
////        pucBuffer[0] = CON_NACK_ERR_NORMAL;
////        pstDataItem->usDataLen = 5;
////    }
//  return ucRet;
//  }

//==========================================================
//F1 ����ת��
unsigned char Main_Recv_DataTrans_F1(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned char ucMsgSource,unsigned char ucSeqNo)
{
    unsigned char   ucRet=RET_SUCCESS;
    unsigned char   m=0;
    unsigned short   usDataLen=0;
    unsigned char   ucPortNo=0,ucCtrlWord=0;
    unsigned char   ucTmp;
//    STRS485TASK     stRS485Task;
    STRELAYTASKPARA stRelayTaskPara;
    unsigned char   *pucBuffer = pstDataItem->ucDataContent;
    if((pucBuffer[m] == 0) || (pucBuffer[m] > 31))
    {
      ucRet=RET_ERROR;
      pstDataItem->usDataLen=0;
      return ucRet;
    }
    ucPortNo=pucBuffer[m++]-1;  //cfh 10-04-04
    if(ucPortNo==2)             //��1��ʼת��Ϊ��0��ʼ
      ucPortNo=1;
    else
      ucPortNo=0; //azh zigbee
    ucCtrlWord=pucBuffer[m++];
    stRelayTaskPara.ucFnVal = 1;
    stRelayTaskPara.ucPortBaudRate=(ucCtrlWord>>5);
    //    stRelayTaskPara.ucRecvWaitTime=pucBuffer[m++]&0x7f;
    ucTmp = pucBuffer[m++];
//    if(ucTmp >= 0x80)  //azh 140521 ����͸��ʱ���ж�
//      ucTmp = 200;
    stRelayTaskPara.ucRecvWaitTime = ucTmp;     //add by cfh 10-04-01
    stRelayTaskPara.ucByteWaitTime=pucBuffer[m++];
    stRelayTaskPara.ucCharacterChar=0x00;
    stRelayTaskPara.ucReserve=0;
    stRelayTaskPara.usGetFrom=0;
    stRelayTaskPara.usGetLen=0;
    stRelayTaskPara.ucCommInfo=(unsigned char)(ucCtrlWord & 0x1F);
//    usDataLen=pucBuffer[m++];
//    m++;        //add by cfh 10-04-01
    usDataLen=pucBuffer[m]+(pucBuffer[m+1]<<8); 
    m += 2;    
    if((usDataLen!=0) && (usDataLen<=CONRS485BIGMAXLEN))
    {
        SetRS485TaskPer(&stRelaySend,ucMsgSource,ucSeqNo,CON_RS485_REPORT_TYPE_RELAY,ucPortNo,ucDAp,&stRelayTaskPara);
//        memcpy(stRelaySend.ucDataArea,pucBuffer+m,ucDataLen);
//        stRelaySend.ucDataLen=ucDataLen;
        memcpy(stRelayDataBuf.ucDataArea,pucBuffer+m,usDataLen);
        stRelayDataBuf.usDataLen = usDataLen;

//        ucRet=RS485_FillCommand(&stRelaySend,1,ucPortNo);               //1��ʾ�м̶���
//azh
        Fill_RelaySendto_Mbx(ucPortNo);
        
    }else
      ucRet=RET_ERROR;
    pstDataItem->usDataLen=0;
    return ucRet;
}
/*
unsigned char Main_Recv_DataTrans_F9(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned char ucMsgSource,unsigned char ucSeqNo)
{
    unsigned char   ucRet=RET_SUCCESS,i;
    unsigned char   m=0;
    unsigned char   ucPortNo=0,ucRelayNum = 0;
    STRS485TASK     stRS485Task;
    STRELAYTASKPARA stRelayTaskPara;
    unsigned char   *pucBuffer = pstDataItem->ucDataContent;
    ucPortNo=pucBuffer[m++];
    if(ucPortNo==2)             //��1��ʼת��Ϊ��0��ʼ
        ucPortNo=1;
    else
        ucPortNo=0;
    stRelayTaskPara.ucFnVal = 9;
    ucRelayNum = pucBuffer[m++];
    if(ucRelayNum != 0xFF)
    {
        for(i=0;i<ucRelayNum;i++)
        {
            memcpy(stRelayTaskPara.stRelayAddr[i].ucRelayAddr,pucBuffer+m,6);
            m += 6;
        }
    }
    memcpy(stRelayTaskPara.stDestAddr.ucRelayAddr,pucBuffer+m,6);
    m += 6;
    stRelayTaskPara.ucDIType = pucBuffer[m++] & 0x03;
    stRelayTaskPara.ulDI = pucBuffer[m]+(pucBuffer[m +1]>>8)+(pucBuffer[m +2]>>16)+(pucBuffer[m +3]>>24);
    SetRS485TaskPer(&stRS485Task,ucMsgSource,ucSeqNo,CON_RS485_REPORT_TYPE_RELAY,CON_DI_TYPE_MPRELAY,ucPortNo,&stRelayTaskPara);
    ucRet=RS485_FillCommand(&stRS485Task,1,ucPortNo);               //1��ʾ�м̶���

    pstDataItem->usDataLen = 0;
    return ucRet;
}*/
//F1 �ļ�ת��
unsigned char Main_Recv_FileTrans_F1(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned char ucMsgSource,unsigned char ucSeqNo)
{
    unsigned char   ucRet=RET_SUCCESS;
    ucRet=RET_ERROR;
    return ucRet;
}

//F100 ������װ����
unsigned char Main_Recv_FileTrans_F100(STDATAITEM *pstDataItem,unsigned char ucDAp,unsigned char ucMsgSource,unsigned char ucSeqNo)
{
    unsigned char   ucRet=RET_SUCCESS;
    unsigned short  m=0;
    unsigned char   *pucBuffer = pstDataItem->ucDataContent;
    unsigned short  usCmd;
    unsigned short  usTmpLen;
//  STTERENCRYPT  stTerEncrypt;
    usTmpLen=pucBuffer[0]+0x100*pucBuffer[1];
    m=2;
//  if(pucBuffer[m]!=CON_TER_MANUFACTOR)
//      ucRet=RET_ERROR;

//azh
//  if(ucRet==RET_SUCCESS)
//  {
//      stTerEncrypt=RunPara_GetTerEncrypt();
//      if(stTerEncrypt.ucAlgoCode!=pucBuffer[m+3] ||
//          stTerEncrypt.ucPassword[0]!=pucBuffer[m+4] ||
//          stTerEncrypt.ucPassword[1]!=pucBuffer[m+5])
//          ucRet=RET_ERROR;
//  }
    if(ucRet==RET_SUCCESS)
    {
        usCmd=pucBuffer[m]+(pucBuffer[m+1]<<8);
        switch(usCmd)
        {
//      case CON_FRM_DP_START:      //��������
//          ucRet=dl_Start(pucBuffer+2,ucMsgSource,usTmpLen);
//          break;
//      case CON_FRM_DP_DATA:       //���ݱ���
//          ucRet=dl_Data(pucBuffer+2,ucMsgSource,usTmpLen);
//          break;
//      case CON_FRM_DP_QUERY:      //��ѯ����
//          ucRet=dl_Query(pucBuffer+2,ucMsgSource,usTmpLen);
//          break;
//      case CON_FRM_DP_SET:        //���ñ���
//          ucRet=dl_Set(pucBuffer+2,ucMsgSource,usTmpLen);
//          break;
//      case CON_FRM_DP_EXECUTE:    //��װ����
//          ucRet=dl_Execute(pucBuffer+2,ucMsgSource,usTmpLen);
//          break;
//      case CON_FRM_DP_END:        //��������
//          ucRet=dl_End(pucBuffer+2,ucMsgSource,usTmpLen);
//          break;
//      case CON_FRM_DP_FINISHED:   //��ɣ����ԣ���������
//          ucRet=dl_Finished(pucBuffer+2,ucMsgSource,usTmpLen,CON_STATUS_YES);
//          break;
//      case CON_FRM_DP_SET_TEST:   //���Ժ������
//          ucRet=dl_Execute_Test(pucBuffer+2,ucMsgSource,usTmpLen);
//          break;
        case CON_FRM_DP_QUERY_VER:      //��ѯ�汾����
            ucRet=dl_Query(pucBuffer+2,ucMsgSource,usTmpLen);
            break;
        case CON_FRM_DP_DATA:       //���ݱ���
            ucRet=dl_Data(pucBuffer+2,ucMsgSource,usTmpLen);
            break;
        case CON_FRM_DP_EXECUTE:    //�л�����
            ucRet=dl_Execute(pucBuffer+2,ucMsgSource,usTmpLen);
            break;
        default:
            ucRet=RET_ERROR;
            break;
        }
    }
    return ucRet;
}

//================  �������� ======================

//�����ʱ���ݿ�
STDATAITEM *GetDataItemTemp()
{
    return (&stDataItemTemp);
}
unsigned char  Main_GetPortProp(STPORTPROP *pstPortProp,unsigned char ucData)
{
    unsigned char ucRet = RET_ERROR;
    if(pstPortProp != NULL)
    {
        pstPortProp->ucByteSize = (ucData & 0x03) +5;
        if((ucData & 0x08)==0x08)
        {
            if((ucData & 0x04)== 0x04)
                pstPortProp->ucParity = UART_ODD_PARITY;//CON_UART_EPN_ODD;
            else
                pstPortProp->ucParity = UART_EVEN_PARITY;//CON_UART_EPN_EVEN;
        }else
            pstPortProp->ucParity = UART_NO_PARITY;//CON_UART_EPN_OFF;

        if((ucData & 0x10) == 0x10)
            pstPortProp->ucStopBits = UART_2_STOP_BIT;//CON_UART_STPB_2;
        else
            pstPortProp->ucStopBits = UART_1_STOP_BIT;//CON_UART_STPB_1;

        ucData = ucData >>5;
        switch(ucData)
        {
            case 0:pstPortProp->ucBaudRate = 1;break;       //300
            case 1:pstPortProp->ucBaudRate = 2;break;       //600
            case 2:pstPortProp->ucBaudRate = 4;break;       //1200
            case 3:pstPortProp->ucBaudRate = 8;break;
            case 4:pstPortProp->ucBaudRate = 16;break;
            case 5:pstPortProp->ucBaudRate = 24;break;
            case 6:pstPortProp->ucBaudRate = 32;break;
            case 7:pstPortProp->ucBaudRate = 64;break;
            default:pstPortProp->ucBaudRate = 32;break;
        }
        ucRet = RET_SUCCESS;
    }
    return ucRet;
}
unsigned char Main_GetPortCtrlByte(STPORTPROP *pstPortProp)
{
    unsigned char ucData = 0;
    if(pstPortProp != NULL)
    {
        ucData += pstPortProp->ucByteSize - 5;
        if(pstPortProp->ucParity != UART_NO_PARITY)//CON_UART_EPN_OFF)
        {
            ucData += 0x08;
            if(pstPortProp->ucParity == UART_ODD_PARITY)//CON_UART_EPN_ODD)
                ucData += 0x04;
        }
        if(pstPortProp->ucStopBits == UART_2_STOP_BIT)//CON_UART_STPB_2)
            ucData += 0x10;
        switch(pstPortProp->ucBaudRate)
        {
            case 1:ucData += (0<<5);break;
            case 2:ucData += (1<<5);break;
            case 4:ucData += (2<<5);break;
            case 8:ucData += (3<<5);break;
            case 16:ucData += (4<<5);break;
            case 24:ucData += (5<<5);break;
            case 32:ucData += (6<<5);break;
            case 64:ucData += (7<<5);break;
            default:ucData += (6<<5);break;
        }
    }
    return ucData;
}
/*****************************************************************************************/
unsigned char SetAlrCategoryVal(unsigned char *pucVal,unsigned char ucFlag,unsigned char ucSetVal1,unsigned char ucSetVal2)
{
    unsigned char ucRet=RET_SUCCESS;
    if(ucFlag == 0x00)
        (*pucVal) = ucSetVal1;
    else
        (*pucVal) = ucSetVal2;

    return ucRet;
}

unsigned char SetAlrCategoryComm(unsigned char ucVal,unsigned char *pucFlag,unsigned char ucPos,unsigned char ucSetVal1,unsigned char ucSetVal2)
{
    unsigned char ucRet=RET_SUCCESS;
    if(ucVal == ucSetVal1)
        (*pucFlag) &= ~(1<<ucPos);

    if(ucVal == ucSetVal2)
        (*pucFlag) |= (1<<ucPos);

    return ucRet;
}

//float DT2_To_Float(unsigned short usDT2)
//{
//  unsigned short usTmp;
//  unsigned char ucS,i;
//  unsigned long ulTmp;
//  float fVal;
//  usTmp=usDT2 & 0x0FFF;
//  ulTmp=BCD_TO_HEX_2((unsigned char *)&usTmp);
//  fVal=(float)ulTmp;
//  fVal*=10000;
//  ucS=(unsigned char)(usDT2>>13);
//  for(i=0;i<ucS;i++)
//      fVal/=10;
//  if((usDT2 & 0x1000)==0x1000)
//      fVal=0-fVal;
//  return fVal;
//}

//unsigned short Float_To_DT2(float fVal)
//{
//  unsigned short  usRet=0;
//  unsigned short  usS=0,usBCD;
//  unsigned short  i;
//  float fTmp;
//  if(fVal==CONINVALIDVAL)
//      return 0xEEEE;
//  fTmp=fVal;
//  if(fTmp<0)
//  {
//      usS=0x1000;
//      fTmp=0-fVal;
//  }
//  fTmp/=10000;
//  if((fTmp+0.5)>1000)
//      usRet=0x999;
//  else
//  {
//      for(i=0;i<7;i++)
//      {
//          if((fTmp+0.5)>100)
//              break;
//          fTmp*=10;
//      }
//      usBCD=(unsigned short)(fTmp+0.5);
//      usBCD=(unsigned short)HEX_TO_BCD(usBCD,2);
//      if(usBCD!=0)
//          usRet=usBCD+(i<<13);
//  }
//  if(usRet!=0)
//      usRet+=usS;
//  return usRet;
//}

/*
unsigned long Float_To_DT3(float fVal,unsigned char ucSign)
{
    unsigned long   ulRet=0;
    unsigned long   ulFlag=0;
    if(fVal==CONINVALIDVAL)
        return 0xEEEEEEEE;
    if(fVal<0)
    {
        ulFlag |= 0x10000000;
        ulRet=(unsigned long)(0-fVal);
    }else
    {
        ulRet=(unsigned long)(fVal);
    }
    if((ucSign == 0x40)||(ulRet>9999999))
    {
        ulRet/=1000;
        ulFlag |= 0x40000000;
    }
    if(ulRet>9999999)
        ulRet=9999999;

    ulRet=HEX_TO_BCD(ulRet,4);
    ulRet|=ulFlag;
    return ulRet;
}

*/
//unsigned long Float_To_DT3(double dVal,unsigned char ucSign)
//{
//  unsigned long   ulRet=0;
//  unsigned long   ulFlag=0;
//  if(dVal==CONINVALIDVAL)
//      return 0xEEEEEEEE;
//  if(dVal<0)
//  {
//      ulFlag |= 0x10000000;
//      dVal=0-dVal;
//  }
//  if((ucSign == 0x40)||(dVal>9999999))
//  {
//      dVal/=1000;
//      ulFlag |= 0x40000000;
//  }
//  ulRet=(unsigned long)(dVal);
//  if(ulRet>9999999)
//      ulRet=9999999;
//
//  ulRet=HEX_TO_BCD(ulRet,4);
//  ulRet|=ulFlag;
//  return ulRet;
//}

//unsigned long Float_To_DT3_Div100(float fVal)
//{
//  unsigned long   ulRet=0;
//  if(fVal==CONINVALIDVAL)
//      return 0xEEEEEEEE;
//  fVal/=100;
//  ulRet=Float_To_DT3(fVal,0);
//  return ulRet;
//}

//double DT3_To_Double(unsigned char *pucDT3)
//{
//  double dVal;
//  unsigned char ucTmp[4];
//  unsigned long ulVal;
//  memcpy(ucTmp,pucDT3,4);
//  ucTmp[3]=ucTmp[3]&0x0F;
//  ulVal=BCD_TO_HEX_4(ucTmp);
//  dVal=ulVal;
//  if((pucDT3[3]&0x40)==0x40)
//      dVal=dVal*1000;
//  if((pucDT3[3]&0x10)==0x10)
//      dVal=0-dVal;
//  return dVal;
//}

//long  DT2_To_Hex(unsigned short usDT2,unsigned char ucPoint)
//{
//  long    lHex=0,i=0;
//  unsigned short  usTmp=0;
//  unsigned long   ulTmp1=1;
//
//  for(i=0;i<ucPoint;i++)
//      ulTmp1 = ulTmp1*10;
//
//  usTmp =usDT2&0x0FFF;
//  lHex = BCD_TO_HEX_2((unsigned char *)&usTmp);
//  usTmp =usDT2>>13;
//
//  if(usTmp == 0)
//      lHex = lHex * 10000 * ulTmp1;
//  else if(usTmp == 1)
//      lHex = lHex * 1000 * ulTmp1;
//  else if(usTmp == 2)
//      lHex = lHex * 100 * ulTmp1;
//  else if(usTmp == 3)
//      lHex = lHex * 10 * ulTmp1;
//  else if(usTmp == 4)
//      lHex = lHex * ulTmp1;
//  else if(usTmp == 5)
//      lHex = lHex * ulTmp1/10;
//  else if(usTmp == 6)
//      lHex = lHex * ulTmp1/100;
//  else if(usTmp == 7)
//      lHex = lHex * ulTmp1/1000;
//
//  if((usTmp&0x1000)!=0)
//      lHex = lHex * (-1);
//  return lHex;
//}

//unsigned short Hex_To_DT2(unsigned long ulHex,unsigned char ucPointSize,unsigned char ucSign)
//{
//  unsigned short  usRet=0;
//  unsigned char   ucG=0;
//  unsigned long   ulInt=0;
//  unsigned long   ulVal=0;
//
//  if(ucPointSize!=0 && ucPointSize <= 5)  //�ȵó�����
//      ulInt = Get_DataType1_Val_Int(ulHex,ucPointSize);
//
//  if(ulInt/10000 >= 100)
//  {
//      ucG   =  0;
//      ulVal =  ulInt/10000;
//  }
//  else if (ulInt/1000>= 100)
//  {
//      ucG = 1;
//      ulVal =  ulInt/1000;
//  }
//  else if (ulInt/100 >= 100)
//  {
//      ucG = 2;
//      ulVal =  ulInt/100;
//  }
//  else if (ulInt/10 >= 100)
//  {
//      ucG = 3;
//      ulVal =  ulInt/10;
//  }
//  else if (ulInt >= 100)
//  {
//      ucG = 4;
//      ulVal =  ulInt;
//  }else
//  {
//      ulInt = Get_DataType1_Val_Point(ulHex,ucPointSize);
//      if ( ulInt/100 >= 100)
//      {
//          ucG = 5;
//          ulVal = ulInt/100;
//      }else if(  ulInt/10 >= 100 )
//      {
//          ucG = 6;
//          ulVal = ulInt/10;
//      }else
//      {
//          ucG =7;
//          ulVal = ulInt;
//      }
//  }
//  usRet=(unsigned short)HEX_TO_BCD(ulVal,2);
//  usRet=usRet+(unsigned short)(ucG*0x2000);
//  if(ucSign==1)
//      usRet=usRet+0x0100;
//  return usRet;
//}

//unsigned long Get_DataType1_Val_Point(unsigned long ulHex,unsigned char ucDataTypeSize)
//{
//    unsigned long ulRet=0;
//    if(ucDataTypeSize == 1)
//        ulRet = ulHex * 100;
//    if(ucDataTypeSize == 2)
//        ulRet = ulHex * 10;
//    if(ucDataTypeSize == 3)
//        ulRet = ulHex;
//    if(ucDataTypeSize == 4)
//        ulRet = ulHex/10;
//    if(ucDataTypeSize == 5)
//        ulRet = ulHex/100;
//    return ulRet;
//}

//unsigned long Get_DataType1_Val_Int(unsigned long ulHex,unsigned char ucDataTypeSize)
//{
//    unsigned long ulRet=0;
//    if(ucDataTypeSize == 1)
//        ulRet = ulHex / 10;
//    if(ucDataTypeSize == 2)
//        ulRet = ulHex / 100;
//    if(ucDataTypeSize == 3)
//        ulRet = ulHex/1000;
//    if(ucDataTypeSize == 4)
//        ulRet = ulHex/10000;
//    if(ucDataTypeSize == 5)
//        ulRet = ulHex/100000;
//
//    return ulRet;
//}
unsigned char Main_GetXishu(unsigned char *ucBuffer,unsigned short *pusVal)
{
    unsigned char ucSign = 0;
    unsigned short usVal;

    if((ucBuffer[1] &0x80) == 0x80)
    {
        ucSign = 1;
        ucBuffer[1] &= 0x7F;
    }else
        ucSign = 0;
    usVal = BCD_TO_HEX_2(ucBuffer);
    if(ucSign == 1)
        usVal = 1000 - usVal;
    else
        usVal = 1000 + usVal;
    *pusVal = usVal;
    return RET_SUCCESS;
}

unsigned short SetMainComm_Buf_DateTime_YMDHMS(unsigned char *pucBuffer,unsigned short usPos,STDATETIME stDateTime)
{
    if(Is_DateTime_Availble(&(stDateTime))==RET_SUCCESS)
    {
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucSecond,1);
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucMinute,1);
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucHour,1);
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucDay,1);
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucMonth,1);
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucYear,1);
    }else
    {
        memset(pucBuffer+usPos,0xEE,6);
        usPos+=6;
    }
    return usPos;
}
unsigned short SetMainComm_Buf_DateTime_YMDHM(unsigned char *pucBuffer,unsigned short usPos,STDATETIME stDateTime)
{
    if(Is_DateTime_Availble(&(stDateTime))==RET_SUCCESS)
    {
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucMinute,1);
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucHour,1);
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucDay,1);
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucMonth,1);
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucYear,1);
    }else
    {
        memset(pucBuffer+usPos,0xEE,5);
        usPos+=5;
    }
    return usPos;
}
unsigned short SetMainComm_Buf_DateTime_MDHM(unsigned char *pucBuffer,unsigned short usPos,STDATETIME stDateTime)
{
    if((stDateTime.ucMonth==0)&&(stDateTime.ucDay==0)&&(stDateTime.ucHour==0)&&(stDateTime.ucMinute==0))//azh ����ʱ��Ϊ0���п��ܵ�
    {
        memset(pucBuffer+usPos,0,4);
        usPos+=4;        
    }    
//    if(Is_DateTime_Availble(&(stDateTime))==RET_SUCCESS)
    else if(Is_DateTime_Availble(&(stDateTime))==RET_SUCCESS)
    {
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucMinute,1);
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucHour,1);
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucDay,1);
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucMonth,1);
    }else
    {
        memset(pucBuffer+usPos,0xEE,4);
        usPos+=4;
    }
    return usPos;
}
unsigned short SetMainComm_Buf_DateTime_DHM(unsigned char *pucBuffer,unsigned short usPos,STDATETIME stDateTime)
{
    if(Is_DateTime_Availble(&(stDateTime))==RET_SUCCESS)
    {
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucMinute,1);
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucHour,1);
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucDay,1);
    }else
    {
        memset(pucBuffer+usPos,0xEE,3);
        usPos+=3;
    }
    return usPos;
}
unsigned short SetMainComm_Buf_DateTime_HMS(unsigned char *pucBuffer,unsigned short usPos,STDATETIME stDateTime)
{
    if(Is_DateTime_Availble(&(stDateTime))==RET_SUCCESS)
    {
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucSecond,1);
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucMinute,1);
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucHour,1);
    }else
    {
        memset(pucBuffer+usPos,0xEE,3);
        usPos+=3;
    }
    return usPos;
}
unsigned short SetMainComm_Buf_DateTime_YMD(unsigned char *pucBuffer,unsigned short usPos,STDATETIME stDateTime)
{
    if(Is_DateTime_Availble(&(stDateTime))==RET_SUCCESS)
    {
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucDay,1);
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucMonth,1);
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucYear,1);
    }else
    {
        memset(pucBuffer+usPos,0xEE,3);
        usPos+=3;
    }
    return usPos;
}
unsigned short SetMainComm_Buf_DateTime_YMDW(unsigned char *pucBuffer,unsigned short usPos,STDATETIME stDateTime)
{
    unsigned char ucWeek;
    ucWeek=GetCurrentWeek(&stDateTime);
    if(Is_DateTime_Availble(&(stDateTime))==RET_SUCCESS)
    {
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucDay,1);
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucMonth,1)+(unsigned char)(ucWeek<<5);
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucYear,1);
    }else
    {
        memset(pucBuffer+usPos,0xEE,3);
        usPos+=3;
    }
    return usPos;
}
unsigned short SetMainComm_Buf_DateTime_YM(unsigned char *pucBuffer,unsigned short usPos,STDATETIME stDateTime)
{
    if(Is_DateTime_Availble(&(stDateTime))==RET_SUCCESS)
    {
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucMonth,1);
        pucBuffer[usPos++] = (unsigned char)HEX_TO_BCD(stDateTime.ucYear,1);
    }else
    {
        memset(pucBuffer+usPos,0xEE,2);
        usPos+=2;
    }
    return usPos;
}
unsigned short SetMainComm_Buf_lVal_5B_DT25(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal)
{
    unsigned long ulTmp=0,ultmp2=0;
//    if(ulVal==CONINVALIDVAL)
    //azh 140918
    if((ulVal==CONINVALIDVAL)||(ulVal==0xffffffff))//azh Init_MeterHisData��Ϊ�˽�ʡ�ռ�ȫ����ֽ�CONINVALIDVAL
    {
        memset(pucBuffer+usPos,0xEE,5);
        usPos+=5;
    }else
    {
        ultmp2 = ulVal % 10;
        ultmp2 *= 10;
        ulVal = ulVal / 10;
        ulTmp=HEX_TO_BCD(ultmp2,1);
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);
        ulTmp=HEX_TO_BCD(ulVal,4);
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);  ulTmp/=0x100;
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);  ulTmp/=0x100;
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);  ulTmp/=0x100;
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);
    }
    return usPos;
}

unsigned short SetMainComm_Buf_lVal_2B(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal)
{
    unsigned long ulTmp=0;
    if((ulVal==CONINVALIDVAL)||((ulVal&0xff)==0xff)) //azh 140918
    {
        memset(pucBuffer+usPos,0xEE,2);
        usPos+=2;
    }else
    {
        ulTmp=HEX_TO_BCD(ulVal,2);
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);  ulTmp/=0x100;
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);
    }
    return usPos;
}

unsigned short SetMainComm_Buf_lVal_2B_Sign(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal,unsigned char ucSign)
{
    unsigned long ulTmp=0;
    if((ulVal==CONINVALIDVAL)||(ulVal==0xffffffff))//azh 140918
    {
        memset(pucBuffer+usPos,0xEE,2);
        usPos+=2;
    }else
    {
        ulTmp=HEX_TO_BCD(ulVal,2);
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);  ulTmp/=0x100;
        if(ucSign==1)   //1��
            pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100) + 0x80;
        else
            pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);
    }
    return usPos;
}

//unsigned short SetMainComm_Buf_lVal_2B_DT07(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal)
//{//��ѹ���ݸ�ʽת��Ϊ���ݸ�ʽ07
//    unsigned long ulTmp=0;
//    if((ulVal==CONINVALIDVAL)||(ulVal==0xffffffff))//azh 140918
//    {
//        memset(pucBuffer+usPos,0xEE,2);
//        usPos+=2;
//    }else
//    {
//    //  ulVal*=10;
//        ulTmp=HEX_TO_BCD(ulVal,2);
//        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);  ulTmp/=0x100;
//        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);
//    }
//    return usPos;
//}

unsigned short  SetMainComm_Buf_lVal_2B_DT05(unsigned char *pucBuffer,unsigned short usPos,short sVal)
{
    unsigned long ulTmp=0;
    if(sVal==0xEE)
    {
        memset(pucBuffer+usPos,0xEE,2);
        usPos+=2;
    }else
    {
        if(sVal>=0)
        {
            ulTmp=HEX_TO_BCD(sVal,2);
            pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);  ulTmp/=0x100;
            pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);
        }
        else
        {
            sVal=-sVal;
            ulTmp=HEX_TO_BCD(sVal,2);
            pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);  ulTmp/=0x100;
            pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100) | 0x80;       //��ʾ����
        }
    }
    return usPos;
}

unsigned short SetMainComm_Buf_lVal_3B(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal)
{
    unsigned long ulTmp=0;
    if((ulVal==CONINVALIDVAL)||(ulVal==0xffffffff))//azh 140918
    {
        memset(pucBuffer+usPos,0xEE,3);
        usPos+=3;
    }else
    {
        ulTmp=HEX_TO_BCD(ulVal,3);
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);  ulTmp/=0x100;
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);  ulTmp/=0x100;
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);
    }
    return usPos;
}

unsigned short SetMainComm_Buf_lVal_3B_Sign(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal,unsigned char ucSign)
{
    unsigned long ulTmp=0;
    if((ulVal==CONINVALIDVAL)||(ulVal==0xffffffff))//azh 140918
    {
        memset(pucBuffer+usPos,0xEE,3);
        usPos+=3;
    }else
    {
        ulTmp=HEX_TO_BCD(ulVal,3);
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);  ulTmp/=0x100;
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);  ulTmp/=0x100;
        if(ucSign == 1)
            pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100) | 0x80;
        else
            pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);
    }
    return usPos;
}

unsigned short SetMainComm_Buf_lVal_4B(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal)
{
    unsigned long ulTmp=0;
    if((ulVal==CONINVALIDVAL)||(ulVal==0xffffffff))//azh 140918
    {
        memset(pucBuffer+usPos,0xEE,4);
        usPos+=4;
    }else
    {
        ulTmp=HEX_TO_BCD(ulVal,4);
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);  ulTmp/=0x100;
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);  ulTmp/=0x100;
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);  ulTmp/=0x100;
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);
    }
    return usPos;
}

unsigned short SetMainComm_Buf_lVal_5B_DT14(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal)
{
    unsigned long ulTmp=0;
    if((ulVal==CONINVALIDVAL)||(ulVal==0xffffffff))//azh 140918
    {
        memset(pucBuffer+usPos,0xEE,5);
        usPos+=5;
    }else
    {
        ulTmp=HEX_TO_BCD(ulVal,4);
        pucBuffer[usPos++] = 0x00;
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);  ulTmp/=0x100;
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);  ulTmp/=0x100;
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);  ulTmp/=0x100;
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);
    }
    return usPos;
}

unsigned short SetMainComm_Buf_lVal_4B_DT13(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal)
{
    unsigned long ulTmp=0;
    if((ulVal==CONINVALIDVAL)||(ulVal==0xffffffff))//azh 140918
    {
        memset(pucBuffer+usPos,0xEE,4);
        usPos+=4;
    }else
    {
        ulTmp=HEX_TO_BCD(ulVal,4);
        pucBuffer[usPos++] = 0x00;
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);  ulTmp/=0x100;
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);  ulTmp/=0x100;
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);
    }
    return usPos;
}

unsigned short  SetMainComm_Buf_lVal_3B_DT09(unsigned char *pucBuffer,unsigned short usPos,unsigned long ulVal,unsigned char ucSign)
{
    unsigned long ulTmp=0;
    if((ulVal==CONINVALIDVAL)||(ulVal==0xffffffff))//azh 140918
    {
        memset(pucBuffer+usPos,0xEE,3);
        usPos+=3;
    }else
    {
        if(ulVal>799999)
            ulTmp=HEX_TO_BCD(799999,3);
        else
            ulTmp=HEX_TO_BCD(ulVal,3);
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);  ulTmp/=0x100;
        pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);  ulTmp/=0x100;
        if(ucSign==1)   //1��
            pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100)+0x80;
        else
            pucBuffer[usPos++] = (unsigned char)(ulTmp%0x100);
    }
    return usPos;
}

//unsigned long GetDiffVal(unsigned long ulVal1,unsigned long ulVal2)
//{
//    unsigned long ulTmp=0;
//    if(ulVal1==CONINVALIDVAL || (ulVal2&0xff)==CONINVALIDVAL)//azh 140918
//        ulTmp=CONINVALIDVAL;
//    else if(ulVal1<ulVal2)
//        ulTmp=0;
//    else
//        ulTmp=ulVal1-ulVal2;
//    return ulTmp;
//}

//unsigned char Check_Data_DT02(unsigned char *pucData)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  if(Check_BCD_Data(pucData[0])==RET_ERROR ||
//      Check_BCD_Data((unsigned char)(pucData[1]&0x0F))==RET_ERROR )
//      ucRet=RET_ERROR;
//  return ucRet;
//}

//unsigned char Check_Data_DT03(unsigned char *pucData)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  if(Check_BCD_Data(pucData[0])==RET_ERROR ||
//      Check_BCD_Data(pucData[1])==RET_ERROR ||
//      Check_BCD_Data(pucData[2])==RET_ERROR ||
//      Check_BCD_Data(pucData[3])==RET_ERROR || (pucData[3]&0xA0)!=0 )
//      ucRet=RET_ERROR;
//  return ucRet;
//}

unsigned char Check_Data_DT05(unsigned char *pucData)
{
    unsigned char ucRet=RET_ERROR;
    ucRet=Check_BCD_Data(pucData[0]);
    if(ucRet==RET_SUCCESS)
        ucRet=Check_BCD_Data((unsigned char)(pucData[1]&0x7F));
    return ucRet;
}

//unsigned char Check_Data_DT06(unsigned char *pucData)
//{
//  unsigned char ucRet=RET_ERROR;
//  ucRet=Check_BCD_Data(pucData[0]);
//  if(ucRet==RET_SUCCESS)
//      ucRet=Check_BCD_Data((unsigned char)(pucData[1]&0x7F));
//  return ucRet;
//}

unsigned char Check_Data_DT07(unsigned char *pucData)
{
    unsigned char ucRet=RET_ERROR;
    ucRet=Check_BCD_Data(pucData[0]);
    if(ucRet==RET_SUCCESS)
        ucRet=Check_BCD_Data(pucData[1]);
    return ucRet;
}

unsigned char Check_Data_DT23(unsigned char *pucData)
{
    unsigned char ucRet=RET_ERROR;
    ucRet=Check_BCD_Data(pucData[0]);
    if(ucRet==RET_SUCCESS)
        ucRet=Check_BCD_Data(pucData[1]);
    if(ucRet==RET_SUCCESS)
        ucRet=Check_BCD_Data(pucData[2]);
    return ucRet;
}

unsigned char Check_Data_DT25(unsigned char *pucData)
{
    unsigned char ucRet=RET_ERROR;
    ucRet=Check_BCD_Data(pucData[0]);
    if(ucRet==RET_SUCCESS)
        ucRet=Check_BCD_Data(pucData[1]);
    if(ucRet==RET_SUCCESS)
        ucRet=Check_BCD_Data((unsigned char)(pucData[2]&0x7f));
    return ucRet;
}

/*****************************************************************************************/
//unsigned short Get_FreezeItem_byMtrComm(unsigned char ucVal)
//{
//    unsigned short usItemLabel=CON_DI_TYPE_INVALID;
//    switch(ucVal)
//    {
//    case    89:     usItemLabel =CON_DI_TYPE_MPINSTANTPOW;  break;
//    case    90:     usItemLabel =CON_DI_TYPE_MPINSTANTPOWA; break;
//    case    91:     usItemLabel =CON_DI_TYPE_MPINSTANTPOWB; break;
//    case    92:     usItemLabel =CON_DI_TYPE_MPINSTANTPOWC; break;
//    case    93:     usItemLabel =CON_DI_TYPE_MPINSTANTVAR;  break;
//    case    94:     usItemLabel =CON_DI_TYPE_MPINSTANTVARA; break;
//    case    95:     usItemLabel =CON_DI_TYPE_MPINSTANTVARB; break;
//    case    96:     usItemLabel =CON_DI_TYPE_MPINSTANTVARC; break;
//    case    97:     usItemLabel =CON_DI_TYPE_MPVOLA;        break;
//    case    98:     usItemLabel =CON_DI_TYPE_MPVOLB;        break;
//    case    99:     usItemLabel =CON_DI_TYPE_MPVOLC;        break;
//    case    100:    usItemLabel =CON_DI_TYPE_MPCURA;        break;
//    case    101:    usItemLabel =CON_DI_TYPE_MPCURB;        break;
//    case    102:    usItemLabel =CON_DI_TYPE_MPCURC;        break;
//
//    case    109:    usItemLabel =CON_DI_TYPE_MPPOWRATE;     break;
//    case    110:    usItemLabel =CON_DI_TYPE_MPPOWRATEA;    break;
//    case    111:    usItemLabel =CON_DI_TYPE_MPPOWRATEB;    break;
//    case    112:    usItemLabel =CON_DI_TYPE_MPPOWRATEC;    break;
//    }
//    return usItemLabel;
//}

//unsigned char Get_MtrComm_byFreezeItem(unsigned short usItem)
//{
//    unsigned char ucVal=0xFF;
//    switch(usItem)
//    {
//    case    CON_DI_TYPE_MPINSTANTPOW:       ucVal=  89;     break;
//    case    CON_DI_TYPE_MPINSTANTPOWA:      ucVal=  90;     break;
//    case    CON_DI_TYPE_MPINSTANTPOWB:      ucVal=  91;     break;
//    case    CON_DI_TYPE_MPINSTANTPOWC:      ucVal=  92;     break;
//    case    CON_DI_TYPE_MPINSTANTVAR:       ucVal=  93;     break;
//    case    CON_DI_TYPE_MPINSTANTVARA:      ucVal=  94;     break;
//    case    CON_DI_TYPE_MPINSTANTVARB:      ucVal=  95;     break;
//    case    CON_DI_TYPE_MPINSTANTVARC:      ucVal=  96;     break;
//    case    CON_DI_TYPE_MPVOLA:             ucVal=  97;     break;
//    case    CON_DI_TYPE_MPVOLB:             ucVal=  98;     break;
//    case    CON_DI_TYPE_MPVOLC:             ucVal=  99;     break;
//    case    CON_DI_TYPE_MPCURA:             ucVal=  100;    break;
//    case    CON_DI_TYPE_MPCURB:             ucVal=  101;    break;
//    case    CON_DI_TYPE_MPCURC:             ucVal=  102;    break;
//
//    case    CON_DI_TYPE_MPPOWRATE:          ucVal=  109;    break;
//    case    CON_DI_TYPE_MPPOWRATEA:         ucVal=  110;    break;
//    case    CON_DI_TYPE_MPPOWRATEB:         ucVal=  111;    break;
//    case    CON_DI_TYPE_MPPOWRATEC:         ucVal=  112;    break;
//    }
//    return ucVal;
//}


//unsigned char Get_FreezeInter(unsigned char ucVal,unsigned char  *pucFreezeInter)
//{
//    unsigned char ucRet=RET_SUCCESS;
//    if(ucVal == 0x00)
//        (*pucFreezeInter)=CON_TIMEINTER_NOT;
//    else if(ucVal == 0x01)
//        (*pucFreezeInter)=CON_TIMEINTER_15MIN;
//    else if(ucVal == 0x02)
//        (*pucFreezeInter)=CON_TIMEINTER_30MIN;
//    else if(ucVal == 0x03)
//        (*pucFreezeInter)=CON_TIMEINTER_60MIN;
//    else
//    {
//        (*pucFreezeInter)=CON_TIMEINTER_INVALID;
//        ucRet=RET_ERROR;
//    }
//    return ucRet;
//}

//unsigned char Get_FreezeInterComm(unsigned char ucFreezeInter)
//{
//    unsigned char ucRet=0;
//    if(ucFreezeInter==CON_TIMEINTER_NOT)
//        ucRet =0;
//    else if(ucFreezeInter==CON_TIMEINTER_15MIN)
//        ucRet =1;
//    else if(ucFreezeInter==CON_TIMEINTER_30MIN)
//        ucRet =2;
//    else if(ucFreezeInter==CON_TIMEINTER_60MIN)
//        ucRet =3;
//    return ucRet;
//}

//unsigned char Check_DUID_IsSingle(STDATAUNITID stDUID)
//{
//    unsigned char ucRet=RET_SUCCESS;
//    if(stDUID.ucDT1==0)
//        return RET_ERROR;
//    if(stDUID.ucDA1!=0 || stDUID.ucDA2!=0)
//    {
//        if(stDUID.ucDA1==0 || stDUID.ucDA2==0)
//            ucRet=RET_ERROR;
//        else
//        {
//            ucRet=Check_IsOneBit(stDUID.ucDA1);
//            if(ucRet==RET_SUCCESS)
//                ucRet=Check_IsOneBit(stDUID.ucDA2);
//            if(ucRet==RET_SUCCESS)
//                ucRet=Check_IsOneBit(stDUID.ucDT1);
//        }
//    }
//    return ucRet;
//}

//unsigned char Check_IsOneBit(unsigned char ucData)
//{//����Ƿ�ֻ��һ��bit
//    unsigned char ucRet=RET_SUCCESS,i=0;
//    unsigned char ucTmp,ucFlag=0xAA;
//    ucTmp=ucData;
//    for(i=0;i<8;i++)
//    {
//        if((ucTmp&0x01)==0x01)
//        {
//            if(ucFlag==0xAA)
//                ucFlag=0x55;
//            else
//            {
//                ucRet=RET_ERROR;
//                break;
//            }
//        }
//        ucTmp=(ucTmp>>1);
//    }
//    if(ucFlag==0xAA)
//        ucRet=RET_ERROR;
//    return ucRet;
//}

void GetData2_DayTime(unsigned char *pucBuffer, STDATETIME *pstDateTime)
{//�������2�����ݵ���ʱ��
    pstDateTime->ucSecond=59;
    pstDateTime->ucMinute=59;
    pstDateTime->ucHour=23;
    pstDateTime->ucDay=(unsigned char)BCD_TO_HEX_1(pucBuffer);
    pstDateTime->ucMonth=(unsigned char)BCD_TO_HEX_1(pucBuffer+1);
    pstDateTime->ucYear=(unsigned char)BCD_TO_HEX_1(pucBuffer+2);
}

void GetData2_MonthTime(unsigned char *pucBuffer, STDATETIME *pstDateTime)
{//�������2�����ݵ���ʱ��
    pstDateTime->ucSecond=59;
    pstDateTime->ucMinute=59;
    pstDateTime->ucHour=23;
    pstDateTime->ucDay=28;
    pstDateTime->ucMonth=(unsigned char)BCD_TO_HEX_1(pucBuffer);
    pstDateTime->ucYear=(unsigned char)BCD_TO_HEX_1(pucBuffer+1);
}

////��������
//unsigned char dl_Start(unsigned char *pucBuffer,unsigned char ucMsgSource,unsigned short usDataLen)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned short m=0;
//  unsigned char *pucFlag;
//  unsigned char ucTemp,i;
//  unsigned char ucVerInfo[7];
//  unsigned char ucTerDP_State=DP_Get_TerDP_State();
//  STDATETIME stDateTime=GetCurrentTime();
//  //  if(usDataLen==64 && ucTerDP_State==CON_TERDP_STATE_NORMAL)
//  if(usDataLen==64)
//  {
//      //�����������
//      for(i = 0 ; i < 8 ; i ++)
//      {
//          Flash_Eraseblock_MCP((MCP_FLASH_PROGRAM_START_BLOCK + i) * EFLASH_BLOCKSIZE1);
//          //HdConvertWatchDog();
//      }
//      Flash_Eraseblock_MCP(RM_FLAG_START_ADDR);
//      //HdConvertWatchDog();
//      DP_Set_TerDP_State(CON_TERDP_STATE_DOWNLOAD);
//      memset(g_ucDownloadBitmap,0,CON_DOWNLOADBITMAP_SIZE);
//      m=6;
//      RunPara_GetTerVerInfo(ucVerInfo);
//      pucBuffer[m++]=ucVerInfo[0];
//      pucBuffer[m++]=ucVerInfo[1];
//      pucBuffer[m++]=ucVerInfo[2];
//      pucBuffer[m++]=ucVerInfo[3];
//  //  pucFlag=(unsigned char *)(RM_FLAG_START_ADDR+DP_DOWNLOAD_OLD_VERSION_ADDR);
//  //  pucFlag[0]=ucVerInfo[0];
//  //  pucFlag[1]=ucVerInfo[1];
//  //  pucFlag[2]=ucVerInfo[2];
//  //  pucFlag[3]=ucVerInfo[3];
//
//      m=22;
//      SetMainComm_Buf_DateTime_YMDHMS(pucBuffer,22,stDateTime);
//      pucFlag=(unsigned char *)(RM_FLAG_START_ADDR+DP_DOWNLOAD_FRAME_NUM_ADDR);
//      //memcpy(pucFlag,pucBuffer+28,2);               //������֡��
//      Flash_SetMainMemory_MCP(RM_FLAG_START_ADDR+DP_DOWNLOAD_FRAME_NUM_ADDR, pucBuffer+28, 2);
//      pucFlag=(unsigned char *)(RM_FLAG_START_ADDR+DP_DOWNLOAD_LENGTH_ADDR);
//      //memcpy(pucFlag,pucBuffer+30,4);               //�����ܳ��ȣ��������򳤶�
//      Flash_SetMainMemory_MCP(RM_FLAG_START_ADDR+DP_DOWNLOAD_LENGTH_ADDR, pucBuffer+30, 4);
//      pucFlag=(unsigned char *)(RM_FLAG_START_ADDR+DP_DOWNLOAD_START_ADDR);
//      //memcpy(pucFlag,pucBuffer+34,4);               //�������ݿ���ʼ��ַ
//      Flash_SetMainMemory_MCP(RM_FLAG_START_ADDR+DP_DOWNLOAD_START_ADDR, pucBuffer+34, 4);
//
//      if((CON_PORT70_NOPOWER & PORT70_NO_POWER_MASK)==0)
//          pucBuffer[38]=0x01;
//      else
//          pucBuffer[38]=0x00;
//      pucBuffer[39]=0x00;
//      memset(pucBuffer+40,0xEE,24);
//      Main_SendComm_DownloadProgram(pucBuffer,64,ucMsgSource,0);
//      if(ucMsgSource==CON_TERMSG_SOUR_RS232)
//          RunPara_SetTerAdjPara_DownloadSource(CON_TERDP_SOURCE_RS232);
//      else
//          RunPara_SetTerAdjPara_DownloadSource(CON_TERDP_SOURCE_REMOTE);
//  }else
//      ucRet=RET_ERROR;
//  return ucRet;
//}
////���ݱ���
//unsigned char dl_Data(unsigned char *pucBuffer,unsigned char ucMsgSource,unsigned short usDataLen)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned short m=0;
//  unsigned short usFrmNo=0,usFrmNum=0,usByteNo=0,usBitNo=0;
//  unsigned char ucBit;
//  unsigned char ucTerDP_State=DP_Get_TerDP_State();
//  unsigned short usLen,usBeginPage,usBeginPos,i;
//  unsigned char ucSum;
//  unsigned char *pucTemp=NULL;
//  unsigned char *pucFlag=(unsigned char *)(RM_FLAG_START_ADDR+DP_DOWNLOAD_FRAME_NUM_ADDR);
//  unsigned long   uldataaddr = 0;
//  if(ucTerDP_State==CON_TERDP_STATE_DOWNLOAD)
//  {
//      if( (pucBuffer[2]==CON_FRM_DP_TYPE_PROG)||
//          (pucBuffer[2]==CON_FRM_DP_TYPE_PARA)||
//          (pucBuffer[2]==CON_FRM_DP_TYPE_MODULE) )
//          ;
//      else
//          ucRet=RET_ERROR;
//
//      if(ucRet==RET_SUCCESS)
//      {
//          usFrmNum=pucFlag[1]*0x100+pucFlag[0];
//          usFrmNo=pucBuffer[28]+0x100*pucBuffer[29];
//          if(usFrmNum==usFrmNo+1)
//              usLen=pucBuffer[30]+0x100*pucBuffer[31];
//          else
//              usLen=512;
//
//          ucSum=0;
//          for(i=0;i<usLen;i++)
//              ucSum+=pucBuffer[32+i];
//          if(ucSum!=pucBuffer[32+usLen])
//              ucRet=RET_ERROR;
//
//          usByteNo=usFrmNo / 8;
//          usBitNo=usFrmNo & 0x7;
//          ucBit=(unsigned char)(1<<usBitNo);
//          if(ucRet==RET_SUCCESS && (g_ucDownloadBitmap[usByteNo] & ucBit)==0)
//          {
//              //usBeginPage=CON_DFPAGE_HEAD_PROGRAM+usFrmNo;      //һ֡�̶�512�ֽ��൱��һҳ
//              uldataaddr = MCP_FLASH_BLOCK_SIZE * MCP_FLASH_PROGRAM_START_BLOCK + usFrmNo * 512;                      //һ֡�̶�512�ֽ��൱��һҳ
//              //usBeginPos=0;
//              Flash_SetMainMemory_MCP(uldataaddr,pucBuffer+32,usLen);     //���������
//              pucTemp=(unsigned char *)CTaskWorkBuffer;
//              Flash_GetMainMemory_MCP(uldataaddr,pucTemp,usLen);          //��ȡ������
//              if(memcmp(pucBuffer+32,pucTemp,usLen)!=0)
//              {
//                  Flash_SetMainMemory_MCP(uldataaddr,pucBuffer+32,usLen); //��дһ��
//                  Flash_GetMainMemory_MCP(uldataaddr,pucTemp,usLen);          //��ȡ������
//                  if(memcmp(pucBuffer+32,pucTemp,usLen)!=0)
//                      ucRet=RET_ERROR;
//              }
//              if(ucRet==RET_SUCCESS)
//              {
//                  g_ucDownloadBitmap[usByteNo] |= ucBit;
//                  Main_SendComm_DownloadProgram(pucBuffer,usLen,ucMsgSource,0xAA);    //0xAA��ʾ���stMainCommInfo
//              }
//          }
//      }
//  }else
//      ucRet=RET_ERROR;
//  return ucRet;
//}
//���ݱ���
unsigned char dl_Data(unsigned char *pucBuffer,unsigned char ucMsgSource,unsigned short usDataLen)
{
    unsigned char ucRet=RET_SUCCESS,err,com;
    unsigned short m=0;

//  unsigned char ucTerDP_State=DP_Get_TerDP_State();
//  if(ucTerDP_State==CON_TERDP_STATE_DOWNLOAD)
//  {
        m = 2;
        com = vport_to_com_convert(ucMsgSource);
        err = check_package_code(pucBuffer+6, com, (usDataLen-6));         //д��������
        if(err == 0)
        {
            memcpy(pucBuffer+m, pucBuffer+6, 4);//���ش����ַ
            m += 4;
        }
        else
        {
            pucBuffer[m++] = err;
        }
        pucBuffer[0] = 0x01;
        pucBuffer[1] = 0xF0;
        Main_SendComm_DownloadProgram(pucBuffer,m,ucMsgSource,0);
//  }else
//      ucRet=RET_ERROR;
    return ucRet;
}
////��ѯ����
//unsigned char dl_Query(unsigned char *pucBuffer,unsigned char ucMsgSource,unsigned short usDataLen)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned short m=0;
//  STDATETIME stDateTime=GetCurrentTime();
//  unsigned char ucTerDP_State=DP_Get_TerDP_State();
//  unsigned char ucTemp;
//  unsigned char ucVerInfo[7];
//  if(usDataLen==33 && ucTerDP_State==CON_TERDP_STATE_DOWNLOAD)
//  {
//      m=6;
//      RunPara_GetTerVerInfo(ucVerInfo);
//      pucBuffer[m++]=ucVerInfo[0];
//      pucBuffer[m++]=ucVerInfo[1];
//      pucBuffer[m++]=ucVerInfo[2];
//      pucBuffer[m++]=ucVerInfo[3];
//      m=22;
//      SetMainComm_Buf_DateTime_YMDHMS(pucBuffer,22,stDateTime);
//      pucBuffer[28]=0;
//      if( (pucBuffer[2]==CON_FRM_DP_TYPE_PROG)||
//          (pucBuffer[2]==CON_FRM_DP_TYPE_PARA)||
//          (pucBuffer[2]==CON_FRM_DP_TYPE_MODULE) )
//          memcpy(pucBuffer+29,g_ucDownloadBitmap,CON_DOWNLOADBITMAP_SIZE);            //��������λͼ
//      else
//          ucRet=RET_ERROR;
//      if(ucRet==RET_SUCCESS)
//      {
//          if((CON_PORT70_NOPOWER & PORT70_NO_POWER_MASK)==0)
//              pucBuffer[285]=0x01;
//          else
//              pucBuffer[285]=0x00;
//          pucBuffer[286]=0x00;
//          memset(pucBuffer+287,0xEE,4);
//          Main_SendComm_DownloadProgram(pucBuffer,291,ucMsgSource,0);
//      }
//  }else
//      ucRet=RET_ERROR;
//  return ucRet;
//}
//��ѯ����
unsigned char dl_Query(unsigned char *pucBuffer,unsigned char ucMsgSource,unsigned short usDataLen)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned short m=0,usLen;
//  STDATETIME stDateTime=GetCurrentTime();
//  unsigned char ucTerDP_State=DP_Get_TerDP_State();


//  if(usDataLen==33 && ucTerDP_State==CON_TERDP_STATE_DOWNLOAD)
//  {
        pucBuffer[m++] = 0x00;
        pucBuffer[m++] = 0xF0;
        usLen = get_monitoring_status(pucBuffer+m);
        m += usLen;
        Main_SendComm_DownloadProgram(pucBuffer,m,ucMsgSource,0);
//  }
//  else
//      ucRet=RET_ERROR;
    return ucRet;
}
////���ñ���
//unsigned char dl_Set(unsigned char *pucBuffer,unsigned char ucMsgSource,unsigned short usDataLen)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  /*
//  unsigned short m=0;
//  unsigned short i,j;
//  STDATETIME stDateTime=GetCurrentTime();
//  unsigned char ucTerDP_State=DP_Get_TerDP_State();
//  unsigned char ucBit;
//  unsigned short usDownloadBitmapFirstBlock,usDownloadBitmapLastBlock,usBlock;
//  unsigned char *pucSrc=NULL;
//  unsigned long ulLen;
//  unsigned char ucFlag=0xFF;
//  unsigned short usPage,usDPLen;
//  unsigned long ulAddr;
//  unsigned char ucTemp;
//  unsigned char ucVerInfo[7];
//  if(usDataLen==289 && ucTerDP_State==CON_TERDP_STATE_DOWNLOAD)
//  {
//      m=6;
//      RunPara_GetTerVerInfo(ucVerInfo);
//      pucBuffer[m++]=ucVerInfo[0];
//      pucBuffer[m++]=ucVerInfo[1];
//      pucBuffer[m++]=ucVerInfo[2];
//      pucBuffer[m++]=ucVerInfo[3];
//      m=22;
//      SetMainComm_Buf_DateTime_YMDHMS(pucBuffer,22,stDateTime);
//      pucBuffer[28]=0;
//      if( (pucBuffer[2]==CON_FRM_DP_TYPE_PROG)||                                      //��������λͼ
//          (pucBuffer[2]==CON_FRM_DP_TYPE_PARA) )                                      //��������λͼ
//          memcpy(g_ucDownloadBitmap,pucBuffer+29,CON_DOWNLOADBITMAP_SIZE);
//      else                                                                            //ģ�������װ��������
//          ucRet=RET_ERROR;
//      usDownloadBitmapFirstBlock=0xFFFF;
//      usDownloadBitmapLastBlock=0xFFFF;
//      for(i=0;i<CON_DOWNLOADBITMAP_SIZE;i++)
//      {
//          for(j=0;j<8;j++)
//          {
//              ucBit=(1<<j);
//              if((g_ucDownloadBitmap[i]&ucBit)==0)
//              {
//                  usBlock=i*8+j;
//                  if(usDownloadBitmapFirstBlock==0xFFFF)
//                      usDownloadBitmapFirstBlock=usBlock;
//                  usDownloadBitmapLastBlock=usBlock;
//              }
//          }
//      }
//      if(pucBuffer[2]!=CON_FRM_DP_TYPE_MODULE)
//      {
//          pucSrc=(unsigned char *)(PM_START_ADDR);//���Ƶ�ǰ�������ݵ�DATAFLASH
//          ulLen=0;
//          ulAddr=MCP_FLASH_PROGRAM_START_BLOCK * MCP_FLASH_BLOCK_SIZE;
//          usDPLen=(DP_DATAFLASH_DATALEN>>1);      //һ�δ���16k
//          while(ulLen<(unsigned long)(PM_LENGTH+usDPLen))
//          {
//              ucFlag=Flash_SetMainMemory_MCP(ulAddr,pucSrc,usDPLen);
//              pucSrc+=usDPLen;
//              ulLen+=usDPLen;
//              usPage+=32;
//              if(ucFlag!=EF_NORMAL)
//                  break;
//          }
//          if(ucFlag==EF_NORMAL)
//          {
//              if(ulLen==PM_LENGTH)                //����洢28k
//                  ucFlag=Flash_SetMainMemory(pucSrc,0x7000,usPage,0);
//          }
//      }
//      if(ucRet==RET_SUCCESS && ucFlag==EF_NORMAL)
//      {
//          g_ucDownloadType=pucBuffer[2];
//          if((CON_PORT70_NOPOWER & PORT70_NO_POWER_MASK)==0)
//              pucBuffer[285]=0x01;
//          else
//              pucBuffer[285]=0x00;
//          pucBuffer[286]=0x00;
//          memset(pucBuffer+287,0xEE,4);
//          Main_SendComm_DownloadProgram(pucBuffer,291,ucMsgSource,0);
//      }else
//          memset(g_ucDownloadBitmap,0,CON_DOWNLOADBITMAP_SIZE);
//  }else
//      ucRet=RET_ERROR;
//  */
//  return ucRet;
//}
////��װ����
//unsigned char dl_Execute(unsigned char *pucBuffer,unsigned char ucMsgSource,unsigned short usDataLen)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned short m=0;
//  unsigned short i=0,usTemp,usBlock,usPos;
//  STDATETIME stDateTime=GetCurrentTime();
//  unsigned char ucTerDP_State=DP_Get_TerDP_State();
//  unsigned char pucFlag[16];
//  unsigned char ucErrFlag,ucTmp;
//  STDATETIME  stExeTime;
//  unsigned char ucVerInfo[7];
//  if(usDataLen==33 && ucTerDP_State==CON_TERDP_STATE_DOWNLOAD)
//  {
//      m=6;
//      RunPara_GetTerVerInfo(ucVerInfo);
//      pucBuffer[m++]=ucVerInfo[0];
//      pucBuffer[m++]=ucVerInfo[1];
//      pucBuffer[m++]=ucVerInfo[2];
//      pucBuffer[m++]=ucVerInfo[3];
//      m=22;
//      SetMainComm_Buf_DateTime_YMDHMS(pucBuffer,22,stDateTime);
//      pucBuffer[28]=0;
//      pucBuffer[29]=0;
//  //  pucFlag=(unsigned char *)(RM_FLAG_START_ADDR+DP_DOWNLOAD_FRAME_NUM_ADDR);
//      Flash_GetMainMemory_MCP(RM_FLAG_START_ADDR+DP_DOWNLOAD_FRAME_NUM_ADDR, pucFlag, 2);
//      usTemp=pucFlag[0]+0x100*pucFlag[1];
//      if(usTemp>(CON_DOWNLOADBITMAP_SIZE*8))
//          usTemp=CON_DOWNLOADBITMAP_SIZE*8;               //�쳣
//      usBlock=usTemp/8;
//      usPos=usTemp%8;
//      ucErrFlag=CON_STATUS_NO;
//      for(i=0;i<usBlock;i++)
//      {
//          if(g_ucDownloadBitmap[i]!=0xFF)
//              break;
//      }
//      if(i<usBlock)
//          ucErrFlag=CON_STATUS_YES;
//      else
//      {
//          ucTmp=0x01;
//          for(i=0;i<usPos;i++)
//          {
//              if((g_ucDownloadBitmap[usBlock] & ucTmp)==0)
//                  break;
//              ucTmp=(ucTmp<<1);
//          }
//          if(i<usPos)
//              ucErrFlag=CON_STATUS_YES;
//      }
//      if(ucErrFlag==CON_STATUS_YES)
//      {
//          if(pucBuffer[2]==CON_FRM_DP_TYPE_PROG)
//              pucBuffer[29]=0x13;             //��������λͼ
//          else if(pucBuffer[2]==CON_FRM_DP_TYPE_PARA)
//              pucBuffer[29]=0x14;             //��������λͼ
//          else if(pucBuffer[2]==CON_FRM_DP_TYPE_PGPA)
//              pucBuffer[29]=0x13;             //��������λͼ+��������λͼ
//          else if(pucBuffer[2]==CON_FRM_DP_TYPE_MODULE)
//              pucBuffer[29]=0x15;             //ģ�������װ
//          else ucRet=RET_ERROR;
//      }
//      if(ucRet==RET_SUCCESS)
//      {
//          g_ucDownloadType=pucBuffer[2];
//          memset(pucBuffer+30,0xEE,4);
//          Main_SendComm_DownloadProgram(pucBuffer,34,ucMsgSource,0);
//
//      //  pucFlag=(unsigned char *)(RM_FLAG_START_ADDR+DP_DOWNLOAD_FRAME_NUM_ADDR);
//          Flash_GetMainMemory_MCP(RM_FLAG_START_ADDR+DP_DOWNLOAD_FRAME_NUM_ADDR, pucFlag, 2);
//          usTemp=pucFlag[0]+0x100*pucFlag[1];
//          if(usTemp<=16)                                      //BOOTLOAD��������8k��16֡
//          {//����BOOTLOAD���±�־
//              //16�ֽڱ�־��0x50,0xA1,0x52,0xA3,0x54,0xA5,0x56,0xA7,0x58,0xA9,0x5A,0xAB,0x5C,0xAD,0x5E,0xAF
//              //pucFlag=(unsigned char *)(RM_FLAG_START_ADDR+DP_DOWNLOAD_NEWBOOT_FLAG);
//              for(i=0;i<8;i++)
//              {
//                  pucFlag[i+i]=0x50+i+i;
//                  pucFlag[i+i+1]=0xA0+i+i+1;
//              }
//              Flash_SetMainMemory_MCP(RM_FLAG_START_ADDR+DP_DOWNLOAD_NEWBOOT_FLAG, pucFlag, 16);
//          }
//          //��Ч��־ 0x50,0xA1,0x52,0xA3,0x54,0xA5,0x56,0xA7
//          //pucFlag=(unsigned char *)(RM_FLAG_START_ADDR+DP_DOWNLOAD_DATAFLASH_INFO); //дDATAFLASH�洢��ʼ���
//          pucFlag[0]=(unsigned char)MCP_FLASH_PROGRAM_START_BLOCK;
//          pucFlag[1]=(unsigned char)0;
//          pucFlag[2]=(unsigned char)MCP_FLASH_PROGRAM_START_BLOCK;
//          pucFlag[3]=(unsigned char)0;
//          for(i=0;i<4;i++)
//          {
//              pucFlag[4+i+i]=0x50+i+i;
//              pucFlag[4+i+i+1]=0xA0+i+i+1;
//          }
//          Flash_SetMainMemory_MCP(RM_FLAG_START_ADDR+DP_DOWNLOAD_DATAFLASH_INFO, pucFlag, 12);
//          stExeTime=GetCurrentTime();
//          if(ucErrFlag==CON_STATUS_NO)
//              AddTime(&stExeTime,10,CON_TIMEUNIT_SEC);        //10���ִ����װִ���ճ���
//          else
//              ClearDateTime(&stExeTime);
//      //  pucFlag=(unsigned char *)(RM_FLAG_START_ADDR+DP_DOWNLOAD_EXE_TIME_ADDR);
//          memcpy(pucFlag,&stExeTime,sizeof(STDATETIME));
//          memcpy(pucFlag+sizeof(STDATETIME),&stExeTime,sizeof(STDATETIME));
//          memcpy(pucFlag+sizeof(STDATETIME)+sizeof(STDATETIME),&stExeTime,sizeof(STDATETIME));
//          Flash_SetMainMemory_MCP(RM_FLAG_START_ADDR+DP_DOWNLOAD_EXE_TIME_ADDR, pucFlag, 3*sizeof(STDATETIME));
//
//      }
//  }else
//      ucRet=RET_ERROR;
//  return ucRet;
//}
//��װ����
unsigned char dl_Execute(unsigned char *pucBuffer,unsigned char ucMsgSource,unsigned short usDataLen)
{
    unsigned char ucRet=RET_SUCCESS,err;
    unsigned short m=0;
//  unsigned char ucTerDP_State=DP_Get_TerDP_State();

//  if(usDataLen==33 && ucTerDP_State==CON_TERDP_STATE_DOWNLOAD)
//  {
        m = 2;
        err = activate_FW(pucBuffer[6]);                //�л�����
        if(err != 1)                                    //1=�ɹ�
        {
            pucBuffer[m++] = err;//����������
        }
        pucBuffer[0] = 0x02;
        pucBuffer[1] = 0xF0;
        Main_SendComm_DownloadProgram(pucBuffer,m,ucMsgSource,0);

//  }
    //else
//      ucRet=RET_ERROR;
    return ucRet;
}
//
//
//
////��װ���Ĳ�����
//unsigned char dl_Execute_Test(unsigned char *pucBuffer,unsigned char ucMsgSource,unsigned short usDataLen)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned short m=0;
//  unsigned short i=0,usTemp,usBlock,usPos;
//  STDATETIME stDateTime=GetCurrentTime();
//  unsigned char ucTerDP_State=DP_Get_TerDP_State();
//  unsigned char pucFlag[16];
//  unsigned char ucErrFlag,ucTmp;
//  STDATETIME  stExeTime;
//  unsigned char ucVerInfo[7];
//  if(usDataLen==33 && ucTerDP_State==CON_TERDP_STATE_DOWNLOAD)
//  {
//      m=6;
//      RunPara_GetTerVerInfo(ucVerInfo);
//      pucBuffer[m++]=ucVerInfo[0];
//      pucBuffer[m++]=ucVerInfo[1];
//      pucBuffer[m++]=ucVerInfo[2];
//      pucBuffer[m++]=ucVerInfo[3];
//      m=22;
//      SetMainComm_Buf_DateTime_YMDHMS(pucBuffer,22,stDateTime);
//      pucBuffer[28]=0;
//      pucBuffer[29]=0;
//  //  pucFlag=(unsigned char *)(RM_FLAG_START_ADDR+DP_DOWNLOAD_FRAME_NUM_ADDR);
//      Flash_GetMainMemory_MCP(RM_FLAG_START_ADDR+DP_DOWNLOAD_FRAME_NUM_ADDR, pucFlag, 2);
//      usTemp=pucFlag[0]+0x100*pucFlag[1];
//
//      if(ucRet==RET_SUCCESS)
//      {
//          g_ucDownloadType=pucBuffer[2];
//          memset(pucBuffer+30,0xEE,4);
//          Main_SendComm_DownloadProgram(pucBuffer,34,ucMsgSource,0);
//
//          pucFlag[0]=0xaa;
//          pucFlag[1]=(unsigned char)0;
//          pucFlag[2]=0x55;
//          pucFlag[3]=(unsigned char)0;
//          for(i=0;i<4;i++)
//          {
//              pucFlag[4+i+i]=0x60+i+i;
//              pucFlag[4+i+i+1]=0xD0+i+i+1;
//          }
//          Flash_SetMainMemory_MCP(RM_FLAG_START_ADDR+DP_DOWNLOAD_DATAFLASH_INFO, pucFlag, 12);
//          stExeTime=GetCurrentTime();
//          AddTime(&stExeTime,10,CON_TIMEUNIT_SEC);        //10���ִ����װִ���ճ���
//      //  pucFlag=(unsigned char *)(RM_FLAG_START_ADDR+DP_DOWNLOAD_EXE_TIME_ADDR);
//          memcpy(pucFlag,&stExeTime,sizeof(STDATETIME));
//          memcpy(pucFlag+sizeof(STDATETIME),&stExeTime,sizeof(STDATETIME));
//          memcpy(pucFlag+sizeof(STDATETIME)+sizeof(STDATETIME),&stExeTime,sizeof(STDATETIME));
//          Flash_SetMainMemory_MCP(RM_FLAG_START_ADDR+DP_DOWNLOAD_EXE_TIME_ADDR, pucFlag, 3*sizeof(STDATETIME));
//
//      }
//  }else
//      ucRet=RET_ERROR;
//  return ucRet;
//}
//
//
////��������
//unsigned char dl_End(unsigned char *pucBuffer,unsigned char ucMsgSource,unsigned short usDataLen)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned short m=0;
//  unsigned char ucTemp;
//  STDATETIME stDateTime=GetCurrentTime();
//  unsigned char ucVerInfo[7];
//  if(usDataLen==32)
//  {
//      DP_Set_TerDP_State(CON_TERDP_STATE_NORMAL);
//  //  TC35_ModuleDown_SetDownState(CON_TERDP_STATE_NORMAL);
//      m=6;
//      RunPara_GetTerVerInfo(ucVerInfo);
//      pucBuffer[m++]=ucVerInfo[0];
//      pucBuffer[m++]=ucVerInfo[1];
//      pucBuffer[m++]=ucVerInfo[2];
//      pucBuffer[m++]=ucVerInfo[3];
//      m=22;
//      SetMainComm_Buf_DateTime_YMDHMS(pucBuffer,22,stDateTime);
//      if((CON_PORT70_NOPOWER & PORT70_NO_POWER_MASK)==0)
//          pucBuffer[28]=0x01;
//      else
//          pucBuffer[28]=0x00;
//      pucBuffer[29]=0x00;
//      memset(pucBuffer+30,0xEE,4);
//      Main_SendComm_DownloadProgram(pucBuffer,34,ucMsgSource,0);
//      RunPara_SetTerAdjPara_DownloadSource(CON_TERDP_SOURCE_INVALID);
//
//      g_ucDPEnd_RestartTime=30;   //---15s
////        DataProc_Init_All();                    //�ÿ���Ϣ������������װ��ʱ�������������Ѿ����ƻ�����Ҫ��ʼ��
//  }else
//      ucRet=RET_ERROR;
//  return ucRet;
//}
//
////��װ�������
//unsigned char dl_Finished(unsigned char *pucBuffer,unsigned char ucMsgSource,unsigned short usDataLen,unsigned char ucSuccessFlag)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned short m=0;
//  unsigned char ucTemp;
//  STDATETIME stDateTime=GetCurrentTime();
//  STTERENCRYPT  stTerEncrypt;
//  unsigned char ucVerInfo[7];
//  stTerEncrypt=RunPara_GetTerEncrypt();
//  if(pucBuffer[28]==0x00 && pucBuffer[29]==0 && pucBuffer[30]==0 && pucBuffer[31]==0)
//  {
//      if(usDataLen==32)
//      {
//          DP_Set_TerDP_State(CON_TERDP_STATE_NORMAL);
//      //  TC35_ModuleDown_SetDownState(CON_TERDP_STATE_NORMAL);
//          pucBuffer[m++]=CON_TER_MANUFACTOR;
//          pucBuffer[m++]=CON_FRM_DP_FINISHED;
//          pucBuffer[m++]=g_ucDownloadType;
//          pucBuffer[m++]=stTerEncrypt.ucAlgoCode;
//          pucBuffer[m++]=stTerEncrypt.ucPassword[0];
//          pucBuffer[m++]=stTerEncrypt.ucPassword[1];
//          RunPara_GetTerVerInfo(ucVerInfo);
//          pucBuffer[m++]=ucVerInfo[0];
//          pucBuffer[m++]=ucVerInfo[1];
//          pucBuffer[m++]=ucVerInfo[2];
//          pucBuffer[m++]=ucVerInfo[3];
//          memset(pucBuffer+m,'V',12);
//          m=22;
//          SetMainComm_Buf_DateTime_YMDHMS(pucBuffer,22,stDateTime);
//          if((CON_PORT70_NOPOWER & PORT70_NO_POWER_MASK)==0)
//          {
//              pucBuffer[28]=0x00;
//              pucBuffer[29]=0x00;
//          }else
//          {
//              pucBuffer[28]=0xAA;
//              pucBuffer[29]=0xAA;
//          }
//          memset(pucBuffer+30,0xEE,4);
//          Main_SendComm_DownloadProgram(pucBuffer,34,ucMsgSource,0x55);       //��֡�����Ϣ
//          RunPara_SetTerAdjPara_DownloadSource(CON_TERDP_SOURCE_INVALID);
//      }else
//          ucRet=RET_ERROR;
//  }
//  return ucRet;
//}
//
//void DP_DownloadFinishedExtCmd(unsigned char ucSuccessFlag)
//{
//  unsigned char ucMsgSource;
//  unsigned char ucWorkBuffer[36];
//  memset(ucWorkBuffer,0x00,36);
//  ucMsgSource=Main_Get_Comm_Type();
//  if(ucMsgSource!=CON_TERMSG_SOUR_RCOMM_TCP && ucMsgSource!=CON_TERMSG_SOUR_RCOMM_UDP )
//      ucMsgSource=CON_TERMSG_SOUR_RS232;
//  dl_Finished(ucWorkBuffer,ucMsgSource,32,ucSuccessFlag);
//}
//
unsigned char DP_Get_TerDP_State(void)
{//��ȡ�ն���װ״̬
    return g_ucTerDP_State;
}

void DP_Set_TerDP_State(unsigned char ucState)
{//�����ն�״̬
    g_ucTerDP_State=ucState;
}

//void Ter_ADESetFeeTimePer(STCTRLFEERATETIME *pstCtrlFeeRateTime)
//{
//  unsigned char ucBuff[64],ucFeeNo[12],ucMin[12],ucHour[12];
//  unsigned char i,ucFeeCount;
//
//  memset(ucBuff,0x00,36);
//  ucBuff[0]=1;    //�׷���Ϊ1
//  ucBuff[1]=1;    //��ʼΪ1��1��
//  ucBuff[2]=1;
//  ADESetMiscData(0xc32f,ucBuff,36);       //������
//
//  memset(ucFeeNo,0xFF,12);
//  memset(ucMin,0xFF,12);
//  memset(ucHour,0xFF,12);
//  ucFeeNo[0]=pstCtrlFeeRateTime->ucFeeRateNo[0]+1;
//  ucMin[0]=0;
//  ucHour[0]=0;
//  ucFeeCount=0;
//  for(i=0;i<47;i++)
//  {
//      if(pstCtrlFeeRateTime->ucFeeRateNo[i] != pstCtrlFeeRateTime->ucFeeRateNo[i+1])
//      {
//          ucFeeCount++;
//          if(ucFeeCount>=12)
//              break;
//          ucFeeNo[ucFeeCount]=pstCtrlFeeRateTime->ucFeeRateNo[i+1]+1;
//          ucMin[ucFeeCount]=((i+1)%2)*30;
//          ucHour[ucFeeCount]=(i+1)/2;
//      }
//  }
//
//  memset(ucBuff,0x00,36);
//  for(i=0;i<12;i++)
//  {
//      if(ucFeeNo[i]==0xFF)
//          break;
//      ucBuff[i+i+i]=ucFeeNo[i];
//      ucBuff[i+i+i+1]=(unsigned char)HEX_TO_BCD(ucMin[i],1);
//      ucBuff[i+i+i+2]=(unsigned char)HEX_TO_BCD(ucHour[i],1);
//  }
//  ADESetMiscData(0xc33f,ucBuff,36);   //�׷���1����ȡ4����
//  memset(ucBuff,0,64);
//  ADESetMiscData(0xbd01,ucBuff,64);       //�׶�����Ϊ��
//  ADESetMiscData(0xbd02,ucBuff,0);        //��������
//}

//unsigned long GetPQRate_Var4B_01(unsigned long lPower,unsigned long lVar)   //�õ���������ֵ����ʽxxx.xxx
//{//�޹�4λС��
//    unsigned long lVal=1000;
//    double dVal=0;
//    dVal=sqrt( (lPower*lPower+lVar*lVar) );
//    if( dVal!=0 )
//    {
//        lVal=(unsigned long)(lPower*1000/dVal+0.5);
//        if( lVal>1000 )
//            lVal=1000;
//    }
//    return lVal;
//}

unsigned long Calc_In(unsigned long ulVala,unsigned long ulValb,unsigned long ulValc)
{
    double dVal=0,dValx=0,dValy=0;
    double dVala=0,dValb=0,dValc=0;
    if(ulVala==CONINVALIDVAL && ulValc==CONINVALIDVAL && ulValc==CONINVALIDVAL)
        return CONINVALIDVAL;           //ȫ��Ч������Ч
    if(ulVala==CONINVALIDVAL || ulValc==CONINVALIDVAL || ulValc==CONINVALIDVAL)
        return 0;                       //ֻҪ��һ����Ч�򷵻�0
    //���������
    dVala=ulVala;
    dValb=ulValb;
    dValc=ulValc;
    dValx=dVala-0.5*dValb-0.5*dValc;
    dValy=0.866*(dValb-dValc);
    if(dValx<0)
        dValx=0-dValx;
    if(dValy<0)
        dValy=0-dValy;
    dVal=sqrt( (dValx*dValx+dValy*dValy) );
    return ((unsigned long)dVal);
}
////����ṹ
//unsigned char ClearRS485TaskNor(STRS485TASKNOR *pstDestTask)
//{
//	if(pstDestTask!=NULL)
//	{
//		memset((unsigned char *)pstDestTask,0,sizeof(STRS485TASKNOR));
//		pstDestTask->ucReportType=CON_RS485_REPORT_TYPE_IDLE;
//	}
//	return RET_SUCCESS;
//}


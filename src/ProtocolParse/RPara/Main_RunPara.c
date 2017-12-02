/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_RunPara.c
 Description    :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-08-15    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"


static unsigned char Clr_AlrCategory_Val(STALRCATEGORY *pstAlrCategory,unsigned char ucVal);


//�ϵ����в�����ʼ��
//-----------------------------------------------------------
unsigned char PowOn_Para_Init_All(void)
{
    unsigned char ucRet=RET_SUCCESS;

    RunPara_Init_All(CON_PARA_INIT_POW_ON); //�ϵ��ʼ��
    Alarm_Get_Last_TerTime();               //��ȡ�ϴ��ն�ʱ��

    if(Is_DateTime_Availble(&g_stTerPowerChangeTime) == RET_SUCCESS)
    {
        SetDateTime_SoftRTC(&g_stTerPowerChangeTime); //���ϴα����ʱ���ȵ�����
    }

    return ucRet;
}


//static void SetLoadFixVal(STLOADFIXVAL *pstLoadFixVal,    double  dFixVal,unsigned short usFloatCoef);
//���в�����ʼ����ȫ����ΪĬ��ֵ
unsigned char RunPara_Init_All(unsigned char ucClrFlag)
{
    U8 ucRet=RET_SUCCESS,i=0;
//azh ��ͨ�Ų���Ҳ����Ǻ�Σ�յ� ������������ⲿ�ֲ���������
//    if(ucClrFlag == 0x55)
//    {
//        RunPara_Init_TerCommPara_Inner(&stTerRunPara.stTerCommPara_Inner);  //F8���ն�����ͨ�Ź�����ʽ����̫ר��������ר����
//         RunPara_InitTerCommPara(&stTerRunPara.stTerCommPara);//F1���ն�����ͨ�ſ�ͨ�Ų������ã�����������

//         RunPara_InitHeartBeat(&stTerRunPara.stHeartBeat);//F1���ն�����
//        RunPara_InitStaIPAddr(&stTerRunPara.stStaIPAddr); //F3����վIP��ַ�Ͷ˿�
//        RunPara_InitStaPhoneNo(&stTerRunPara.stStaPhoneNo);                 //F4����վ�绰����Ͷ������ĺ���
//        RunPara_InitLocalIP(&(stTerRunPara.stLocalIP));                     //F7���ն�IP��ַ�Ͷ˿�
//    }
    RunPara_Init_TerCommPara_Inner(&stTerRunPara.stTerCommPara_Inner);      //F8���ն�����ͨ�Ź�����ʽ����̫ר��������ר����
    RunPara_InitTerCommPara(ucClrFlag);                                     //F1���ն�����ͨ�ſ�ͨ�Ų�������alan ��������
    RunPara_InitStaIPAddr(ucClrFlag);                                       //F3����վIP��ַ�Ͷ˿�alan
    RunPara_InitTerDev_Region_RegionAddr(ucClrFlag);                                 //F207 ��������ַ
    RunPara_InitTerEncrypt(&stTerRunPara.stTerEncrypt);                     //������֤���ƺ�û�õ�
    RunPara_InitTerGroupAddrList(&stTerRunPara.stTerGroupAddrList);         //F6: �ն����ַ
    RunPara_InitCommStat(&stTerRunPara.stTerCommStat);                      //F5���ն�����ͨ����Ϣ��֤��������
    RunPara_InitAlrCtrlWord(&stTerRunPara.stAlrCtrlWord);                   //F9���ն��¼���¼��������
    for(i=0;i<CON_MAX_MPNUM;i++)
        RunPara_InitMPInfo(&stTerRunPara.stMPInfo[i]);                      //F26:�����㼫�޲���
    stTerRunPara.stMPInfo[0].ucMPType=CON_MPTYPE_METER;//CON_MPTYPE_TER;          //����������Ϊ�ն�
    stTerRunPara.stMPInfo[0].ucDevNo[0]=0;                      //��һ���

    stTerRunPara.stMPInfo[1].ucMPType=CON_MPTYPE_METER;         //����������Ϊ����
    stTerRunPara.stMPInfo[1].ucDevNo[0]=1;                      //�����ɼ�װ��
//azh   if(ADECheckLineType_Is3P3L()==CON_STATUS_YES)               //��Ҫ��֤��ADEInit�����ȷ�Ľ�������
//      stTerRunPara.stMPInfo[0].stMPRateVal.ucMPLineType=CON_METERTYPE_3P3L;       //�ȳ�ʼ��Ϊ��������
    RunPara_InitConfigNum(&(stTerRunPara.stConfigNum));                     //F10���ն˵��ܱ�/��������װ�����ò���
//  for(i=0;i<CON_PULSE_NUM;i++)
//      RunPara_InitPulsePara(&stTerRunPara.stPulsePara[i]);
    for(i=0;i<CON_METER_NUM;i++)
    {
        RunPara_InitMeterPara(&stTerRunPara.stMeterPara[i]);
    }
//  for(i=0;i<CON_SUMGROUP_NUM;i++)
//      RunPara_InitSumGroupPara(&stTerRunPara.stSumGroupPara[i]);
//  for(i=0;i<CON_ANALOG_NUM;i++)
//      RunPara_InitAnalogPara(&stTerRunPara.stAnalogPara[i]);

    stTerRunPara.stMeterPara[0].ucProtocol = CON_PROTOCOL_DL64507;//�����������ʱ��Ϊ��Ҫ�ж�
    stTerRunPara.stMeterPara[0].stMeterAddr.ucMeterAddr[0]  = 0x01;
    stTerRunPara.stMeterPara[0].stMeterAddr.ucMeterAddr[1]  = 0x00;

	Init_MP_ParaEE(ucClrFlag);//azh 140313
//141009 azh ��uart����ͷ��ʼ�� ������E2�������֮ǰ
    status_mechanism_init(COM0);
    status_mechanism_init(COM1);
    status_mechanism_init(COM2);
    status_mechanism_init(COM3);
    
//  stTerRunPara.stPulsePara[0].ucAttr=CON_PULSEATTR_DIRPOW;
//  stTerRunPara.stPulsePara[0].usConst=1000;
//  stTerRunPara.stPulsePara[1].ucAttr=CON_PULSEATTR_DIRVAR;
//  stTerRunPara.stPulsePara[1].usConst=1000;
//  stTerRunPara.stPulsePara[2].ucAttr=CON_PULSEATTR_DIRPOW;
//  stTerRunPara.stPulsePara[2].usConst=2000;
//  stTerRunPara.stPulsePara[3].ucAttr=CON_PULSEATTR_DIRVAR;
//  stTerRunPara.stPulsePara[3].usConst=2000;
//
//  stTerRunPara.stSumGroupPara[0].ucAvailbleFlag=CON_AVALIBLE_FLAG;
//  stTerRunPara.stSumGroupPara[0].stInstantPow.stDir.ucMPSum[0]=0;
//  stTerRunPara.stSumGroupPara[0].stInstantPow.stDir.ucMPOper[0]=CON_SUMGOPER_ADD;

//  for(i=0;i<CON_SWITCHIN_NUM;i++)
//      RunPara_InitSwitchPara(&stTerRunPara.stSwitchPara[i]);
//  stTerRunPara.stSwitchPara[0].ucAttr = CON_SWITCHATTR_TYPEB;
//  stTerRunPara.stSwitchPara[1].ucAttr = CON_SWITCHATTR_TYPEB;
//  stTerRunPara.stSwitchPara[2].ucAttr = CON_SWITCHATTR_TYPEB;
//  stTerRunPara.stSwitchPara[3].ucAttr = CON_SWITCHATTR_TYPEB;
//  stTerRunPara.stSwitchPara[7].ucAttr = CON_SWITCHATTR_TYPEA;     //�Žڵ㣬Ĭ��ΪA
//  stTerRunPara.stSwitchPara[0].ucAlarm = CON_SWITCHALARM_IMP;
//  stTerRunPara.stSwitchPara[1].ucAlarm = CON_SWITCHALARM_IMP;
//  stTerRunPara.stSwitchPara[2].ucAlarm = CON_SWITCHALARM_IMP;
//  stTerRunPara.stSwitchPara[3].ucAlarm = CON_SWITCHALARM_IMP;
//  stTerRunPara.stSwitchPara[7].ucAlarm = CON_SWITCHALARM_IMP;     //�Žڵ�
    RunPara_InitAuxPara(&stTerRunPara.stTerAuxPara);            //�ն�������Ϣ��ʼ��

//  RunPara_InitTerDevAddr(&stTerRunPara.stTerAddr);
//  RunPara_InitTerRegionAddr(&stTerRunPara.stTerRegionAddr);

    for(i=0;i<CONDATAJOBNUM;i++)//���1����������
//        RunPara_InitJobPara(&stTerRunPara.stJobPara_Data1[i]);
        RunPara_InitJobPara_eeprom(i, 0, ucClrFlag);
    for(i=0;i<CONDATAJOBNUM;i++)//���2����������
//        RunPara_InitJobPara(&stTerRunPara.stJobPara_Data2[i]);
        RunPara_InitJobPara_eeprom(i, 1, ucClrFlag);

    RunPara_InitJobFnPara(&(stTerRunPara.stJobFnPara1));
    RunPara_InitJobFnPara(&(stTerRunPara.stJobFnPara2));
    RunPara_InitLinkPara(&(stTerRunPara.stLinkPara));
    RunPara_InitJobRunInfo(ucClrFlag);

//azh   RunPara_initAlrMaxCount(&(stTerRunPara.stTerAlrMaxCount));
//  RunPara_InitSwitchCtrlPara(&(stTerRunPara.stSwitchCtrlPara));
//  RunPara_InitLoadCtrlWordPara(&(stTerRunPara.stLoadCtrlWordPara));

    RunPara_InitTerMiscPara(&(stTerRunPara.stTerMiscPara));//azh
    RunPara_InitTerNormalPara(&(stTerRunPara.stTerNormalPara));
    RunPara_InitTerAdjPara(&(stTerRunPara.stTerAdjPara));
//  RunPara_InitCTCheckPara(&(stTerRunPara.stCTCheckPara));
    RunPara_InitPortPropList(&(stTerRunPara.stPortList), ucClrFlag);

//	CTask_Rebuild_Task();       //azh �ܳ�ʼ�����Ѿ�������

    //ADD BY CFH 10-04-03
//  memset( &stTerRunPara_Nouse,0,sizeof(stTerRunPara_Nouse));
    //cfh 10-04-15
    DP_Set_TerDP_State(CON_TERDP_STATE_NORMAL); //���������Ϊ����״̬

//azh   RunPara_InitMeterProtocolInfo();            //����Լ�������Ĭ��
//  Call_NOP();
//  Call_NOP();
//azh 170919    
    stTerRunPara.ucSnstate[0] = 0x01;
    
    return ucRet;
}




//���в������
//unsigned char RunPara_Valid_Check(void)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  if( (stTerRunPara.ucClrFlg[0]!=0x55)    || (stTerRunPara.ucClrFlg[1]!=0xAA) ||
//      (stTerRunPara.ucClrFlg[2]!=0x55)    || (stTerRunPara.ucClrFlg[3]!=0xAA) ||
//      (stTerRunPara.ucClrFlg[4]!=0x55)    || (stTerRunPara.ucClrFlg[5]!=0xAA) ||
//      (stTerRunPara.ucClrFlg[6]!=0x55)    || (stTerRunPara.ucClrFlg[7]!=0xAA) ||
//      (stTerRunPara.ucClrFlg[8]!=0x55)    || (stTerRunPara.ucClrFlg[9]!=0xAA))
//  {
//      stTerRunPara.ucClrFlg[0]=0x55;
//      stTerRunPara.ucClrFlg[1]=0xAA;
//      stTerRunPara.ucClrFlg[2]=0x55;
//      stTerRunPara.ucClrFlg[3]=0xAA;
//      stTerRunPara.ucClrFlg[4]=0x55;
//      stTerRunPara.ucClrFlg[5]=0xAA;
//      stTerRunPara.ucClrFlg[6]=0x55;
//      stTerRunPara.ucClrFlg[7]=0xAA;
//      stTerRunPara.ucClrFlg[8]=0x55;
//      stTerRunPara.ucClrFlg[9]=0xAA;
//      ucRet=RET_NEEDPROC;         //ĳЩ��ȡ������Ч�������Ҫ��������
//  }else
//  {
//      RunPara_InitJobRunInfo();
//  }
//  return ucRet;
//}


/*********************************************************************************************************************/
//F1���ն�����ͨ�ſ�ͨ�Ų������ã�����������
// unsigned char RunPara_InitTerCommPara(STTERCOMMPARA *pstTerCommpara)
// {
//     unsigned char ucRet=RET_SUCCESS,i=0;

//     pstTerCommpara->ucDelay=0;                      //����Ҫ��ʱ(���ڲ����õ�̨ͨ��)
//     pstTerCommpara->ucMaxOvertimeOrig=0x2;          //2����
//     pstTerCommpara->ucReSendtimes=0;                //0��
//     pstTerCommpara->usACKOvertime=120;              //2����
//     for(i=0;i<CON_SERVER_NUM;i++)
//         pstTerCommpara->ucServerNeedAck[i] = CON_STATUS_NO;
//     pstTerCommpara->ucServerNeedAck[0] = CON_STATUS_YES;
//     pstTerCommpara->ucServerNeedAck[1] = CON_STATUS_NO;
//     pstTerCommpara->ucServerNeedAck[2] = CON_STATUS_NO;
//     pstTerCommpara->ucServerNeedAck[3] = CON_STATUS_NO;

//     return ucRet;
// }
// //F1���ն�����
// unsigned char RunPara_InitHeartBeat(STHEARTBEAT *pstHeartBeat)
// {
//     unsigned char ucRet=RET_SUCCESS;
//     pstHeartBeat->ucHeartBeat = 2;                  //������� 2����
//     return ucRet;
// }
//F1���ն�����ͨ�ſ�ͨ�Ų�����ʼ�����ã���������    alan
/******************************************************************************
** ��������: U8 RunPara_InitTerCommPara(U8 ucClrFlag)
** ��������: �ն�����ͨ�ſ�ͨ�Ų�����ʼ�����ã���������
** ��ڲ���: ��
** �� �� ֵ: 0������������Ч��ȡ��1Ĭ���Զ���
** ˵    ��: alan
******************************************************************************/
U8 RunPara_InitTerCommPara (U8 ucClrFlag)
{
    U8 ucRet, i;
    U8 pret_mem[16];
    U16 crc16;

    ucRet = RET_SUCCESS;
    if(ucClrFlag == CON_PARA_INIT_ALL_CLR)
    {
        pret_mem[0] = 0xfe;
        pret_mem[1] = 0xfe;
        pret_mem[2] = 0xfe;
        pret_mem[3] = 0xfe;
        pret_mem[4] = 0xfe;
        pret_mem[5] = 0xfe;
        file_write(F1_TERMINAL_COM_PARA_ADDR + 6, pret_mem, 6);     //��crc�ĵ�
        stTerRunPara.stTerCommPara.ucDelay=0;                       //����Ҫ��ʱ(���ڲ����õ�̨ͨ��)
        stTerRunPara.stTerCommPara.ucMaxOvertimeOrig=0x2;           //2����
        stTerRunPara.stTerCommPara.ucReSendtimes=0;                 //0��
        stTerRunPara.stTerCommPara.usACKOvertime=120;               //2����
        for(i=0;i<CON_SERVER_NUM;i++)
        {
            stTerRunPara.stTerCommPara.ucServerNeedAck[i] = CON_STATUS_NO;
        }
        stTerRunPara.stTerCommPara.ucServerNeedAck[0] = CON_STATUS_YES;
        stTerRunPara.stTerCommPara.ucServerNeedAck[1] = CON_STATUS_NO;
        stTerRunPara.stTerCommPara.ucServerNeedAck[2] = CON_STATUS_NO;
        stTerRunPara.stTerCommPara.ucServerNeedAck[3] = CON_STATUS_NO;
        stTerRunPara.stHeartBeat.ucHeartBeat = 2;                  //������� 2����
    }
    else
    {
    file_read (F1_TERMINAL_COM_PARA_ADDR, pret_mem, 16);
    crc16 = calc_crc16(pret_mem, 6, 0xffff);      //����ͨ�Ų������ݵ�CRC
    if(crc16 == ((U16)pret_mem[6] << 8) + pret_mem[7])
    {
        Main_Recv_Set_F1_CommPara(pret_mem, 0);
    }
    else
    {
        crc16 = calc_crc16(pret_mem + 8, 6, 0xffff);      //������վIP��ַ�Ͷ˿ڵ�crc
        if(crc16 == ((U16)pret_mem[8 + 6] << 8) + pret_mem[8 + 7])
        {
            Main_Recv_Set_F1_CommPara(pret_mem + 8, 0);
        }
        else
        {
            stTerRunPara.stTerCommPara.ucDelay=0;                      //����Ҫ��ʱ(���ڲ����õ�̨ͨ��)
            stTerRunPara.stTerCommPara.ucMaxOvertimeOrig=0x2;          //2����
            stTerRunPara.stTerCommPara.ucReSendtimes=0;                //0��
            stTerRunPara.stTerCommPara.usACKOvertime=120;              //2����
    for(i=0;i<CON_SERVER_NUM;i++)
            {
                stTerRunPara.stTerCommPara.ucServerNeedAck[i] = CON_STATUS_NO;
            }
            stTerRunPara.stTerCommPara.ucServerNeedAck[0] = CON_STATUS_YES;
            stTerRunPara.stTerCommPara.ucServerNeedAck[1] = CON_STATUS_NO;
            stTerRunPara.stTerCommPara.ucServerNeedAck[2] = CON_STATUS_NO;
            stTerRunPara.stTerCommPara.ucServerNeedAck[3] = CON_STATUS_NO;
            stTerRunPara.stHeartBeat.ucHeartBeat = 2;                  //������� 2����
            ucRet = RET_ERROR;
        }
    }
    }
    return ucRet;
}
//F3����վIP��ַ�Ͷ˿�
// unsigned char RunPara_InitStaIPAddr(STSTAIPADDR *pstStaIPAddr)
// {
//     unsigned char ucRet=RET_SUCCESS;
//     pstStaIPAddr->stBaseIP.ucIPAddr[0]=218;                 //IP��ַ1�θ�λ
//     pstStaIPAddr->stBaseIP.ucIPAddr[1]=108;                 //2��
//     pstStaIPAddr->stBaseIP.ucIPAddr[2]=43;                  //3��
//     pstStaIPAddr->stBaseIP.ucIPAddr[3]=19;                  //4��
//     pstStaIPAddr->stBaseIP.usIPPort=1014;                       //�˿� 10000

//     pstStaIPAddr->stBackIP.ucIPAddr[0]=218;                 //����IP��ַ
//     pstStaIPAddr->stBackIP.ucIPAddr[1]=108;
//     pstStaIPAddr->stBackIP.ucIPAddr[2]=43;
//     pstStaIPAddr->stBackIP.ucIPAddr[3]=19;
//     pstStaIPAddr->stBackIP.usIPPort=1014;

// /*  pstStaIPAddr->stGateWay.ucIPAddr[0]=00;                 //����Ϊ��Ч
//     pstStaIPAddr->stGateWay.ucIPAddr[1]=00;
//     pstStaIPAddr->stGateWay.ucIPAddr[2]=00;
//     pstStaIPAddr->stGateWay.ucIPAddr[3]=00;
//     pstStaIPAddr->stGateWay.usIPPort=00;*/

//     memset(pstStaIPAddr->cAPN,0,CON_MAXAPNLEN);
//     strcpy(pstStaIPAddr->cAPN,"CMNET");

//     return ucRet;
// }
/******************************************************************************
** ��������: U8 RunPara_InitStaIPAddr(U8 ucClrFlag)
** ��������: F3����վIP��ַ�Ͷ˿ڳ�ʼ������
** ��ڲ���: ��
** �� �� ֵ: 0������������Ч��ȡ��1Ĭ���Զ���
** ˵    ��: alan
******************************************************************************/
U8 RunPara_InitStaIPAddr (U8 ucClrFlag)
{
    U8 ucRet;
    U8 mem_type;//pret_mem[60];
    U16 crc16;
    U8 *pret_mem = NULL;
    ucRet = RET_SUCCESS;
    pret_mem = alloc_comm_mem(MEM_64_TYPE, &mem_type, GET_MEM_ERR0);
    if(pret_mem == NULL)
    {
        stTerRunPara.stStaIPAddr.stBaseIP.ucIPAddr[0]=218;                 //IP��ַ1�θ�λ
        stTerRunPara.stStaIPAddr.stBaseIP.ucIPAddr[1]=108;                 //2��
        stTerRunPara.stStaIPAddr.stBaseIP.ucIPAddr[2]=43;                  //3��
        stTerRunPara.stStaIPAddr.stBaseIP.ucIPAddr[3]=19;                  //4��
        stTerRunPara.stStaIPAddr.stBaseIP.usIPPort=1014;                       //�˿� 10000

        stTerRunPara.stStaIPAddr.stBackIP.ucIPAddr[0]=218;                 //����IP��ַ
        stTerRunPara.stStaIPAddr.stBackIP.ucIPAddr[1]=108;
        stTerRunPara.stStaIPAddr.stBackIP.ucIPAddr[2]=43;
        stTerRunPara.stStaIPAddr.stBackIP.ucIPAddr[3]=19;
        stTerRunPara.stStaIPAddr.stBackIP.usIPPort=1014;

        memset(stTerRunPara.stStaIPAddr.cAPN,0,CON_MAXAPNLEN);
        strcpy(stTerRunPara.stStaIPAddr.cAPN,"CMNET");
        if(ucClrFlag == CON_PARA_INIT_ALL_CLR)
        {
            pret_mem[0] = 0xfe;
            pret_mem[1] = 0xfe;
            file_write(F3_STAIPADDR_ADDR + 28, pret_mem, 2);        //��crc�ĵ�
            file_write(F3_STAIPADDR_ADDR + 58, pret_mem, 2);        //��crc�ĵ�
        }
        ucRet = RET_ERROR;
    }
//azh �ܲ����嶼��EEPOM�Ӷ�ȡ����
//    if(ucClrFlag == CON_PARA_INIT_ALL_CLR)
//    {
//        pret_mem[0] = 0xfe;
//        pret_mem[1] = 0xfe;
//        file_write(F3_STAIPADDR_ADDR + 28, pret_mem, 2);        //��crc�ĵ�
//        file_write(F3_STAIPADDR_ADDR + 58, pret_mem, 2);        //��crc�ĵ�
//        free_mem_block(pret_mem);//MEM_64_TYPE
//        stTerRunPara.stStaIPAddr.stBaseIP.ucIPAddr[0]=218;                 //IP��ַ1�θ�λ
//        stTerRunPara.stStaIPAddr.stBaseIP.ucIPAddr[1]=108;                 //2��
//        stTerRunPara.stStaIPAddr.stBaseIP.ucIPAddr[2]=43;                  //3��
//        stTerRunPara.stStaIPAddr.stBaseIP.ucIPAddr[3]=19;                  //4��
//        stTerRunPara.stStaIPAddr.stBaseIP.usIPPort=1014;                       //�˿� 10000
//
//        stTerRunPara.stStaIPAddr.stBackIP.ucIPAddr[0]=218;                 //����IP��ַ
//        stTerRunPara.stStaIPAddr.stBackIP.ucIPAddr[1]=108;
//        stTerRunPara.stStaIPAddr.stBackIP.ucIPAddr[2]=43;
//        stTerRunPara.stStaIPAddr.stBackIP.ucIPAddr[3]=19;
//        stTerRunPara.stStaIPAddr.stBackIP.usIPPort=1014;
//
//        memset(stTerRunPara.stStaIPAddr.cAPN,0,CON_MAXAPNLEN);
//        strcpy(stTerRunPara.stStaIPAddr.cAPN,"CMNET");
//        return  RET_SUCCESS;
//    }
    file_read (F3_STAIPADDR_ADDR, pret_mem, 60);
    crc16 = calc_crc16(pret_mem, 28, 0xffff);      //����ͨ�Ų������ݵ�CRC
    if(crc16 == ((U16)pret_mem[28] << 8) + pret_mem[28 + 1])
    {
        Main_Recv_Set_F3_IP_Addr(pret_mem, 0);
        free_mem_block(pret_mem);//MEM_64_TYPE
    }
    else
    {
        crc16 = calc_crc16(pret_mem + 30, 28, 0xffff);      //������վIP��ַ�Ͷ˿ڵ�crc
        if(crc16 == ((U16)pret_mem[30 + 28] << 8) + pret_mem[30 + 28 + 1])
        {
            Main_Recv_Set_F3_IP_Addr(pret_mem + 30, 0);
            free_mem_block(pret_mem);//MEM_64_TYPE
}
        else
        {
            free_mem_block(pret_mem);//MEM_64_TYPE
            stTerRunPara.stStaIPAddr.stBaseIP.ucIPAddr[0]=218;                 //IP��ַ1�θ�λ
            stTerRunPara.stStaIPAddr.stBaseIP.ucIPAddr[1]=108;                 //2��
            stTerRunPara.stStaIPAddr.stBaseIP.ucIPAddr[2]=43;                  //3��
            stTerRunPara.stStaIPAddr.stBaseIP.ucIPAddr[3]=19;                  //4��
            stTerRunPara.stStaIPAddr.stBaseIP.usIPPort=1014;                       //�˿� 10000

            stTerRunPara.stStaIPAddr.stBackIP.ucIPAddr[0]=218;                 //����IP��ַ
            stTerRunPara.stStaIPAddr.stBackIP.ucIPAddr[1]=108;
            stTerRunPara.stStaIPAddr.stBackIP.ucIPAddr[2]=43;
            stTerRunPara.stStaIPAddr.stBackIP.ucIPAddr[3]=19;
            stTerRunPara.stStaIPAddr.stBackIP.usIPPort=1014;

            memset(stTerRunPara.stStaIPAddr.cAPN,0,CON_MAXAPNLEN);
            strcpy(stTerRunPara.stStaIPAddr.cAPN,"CMNET");

            ucRet = RET_ERROR;
        }
    }
    return ucRet;
}
////F7���ն�IP��ַ�Ͷ˿�
//unsigned char RunPara_InitLocalIP(STLOCALIP*pstLocalIP)
//{
//    unsigned char ucRet = RET_SUCCESS;
//
//    pstLocalIP->stAgentServer.ucLinkType = CON_SERVE_NOVERIFY;          //����Ҫ��֤
//    pstLocalIP->stAgentServer.ucServeType = CON_SERVE_TYPE_NO;      //����Ҫ����
//    pstLocalIP->stAgentServer.ucUsrNameLen = 0;
//    pstLocalIP->stAgentServer.ucPwdLen = 0;
//    memset(&(pstLocalIP->stAgentServer.stServeIP),0,sizeof(STIPADDR));
//
//    memset(&(pstLocalIP->stLocalIP),0,sizeof(STIPADDR));
//    memset(&(pstLocalIP->stSubMask),0,sizeof(STIPADDR));
//    memset(&(pstLocalIP->stGateWay),0,sizeof(STIPADDR));
//    return ucRet;
//}
//F4����վ�绰����Ͷ������ĺ���
unsigned char RunPara_InitStaPhoneNo(STSTAPHONENO   *pstStaPhoneNo)
{
    unsigned char ucRet=RET_SUCCESS;
    U8 pret_mem[36];
    U16 crc16;

    ucRet = RET_SUCCESS;
    file_read (F4_STAPHONENO_ADDR, pret_mem, 36);
    crc16 = calc_crc16(pret_mem, 16, 0xffff);      //����ͨ�Ų������ݵ�CRC
    if(crc16 == ((U16)pret_mem[16] << 8) + pret_mem[16 + 1])
    {
        Main_Recv_Set_F4_Station_Number(pret_mem, 0);
    }
    else
    {
        crc16 = calc_crc16(pret_mem + 18, 16, 0xffff);      //������վIP��ַ�Ͷ˿ڵ�crc
        if(crc16 == ((U16)pret_mem[18 + 16] << 8) + pret_mem[18 + 16 + 1])
        {
            Main_Recv_Set_F4_Station_Number(pret_mem + 18, 0);
        }
        else
        {
    pstStaPhoneNo->stPhoneNo.ucPhoneNo[0]=0x13;             //��վ�绰����13819619814
    pstStaPhoneNo->stPhoneNo.ucPhoneNo[1]=0x81;             //
    pstStaPhoneNo->stPhoneNo.ucPhoneNo[2]=0x96;
    pstStaPhoneNo->stPhoneNo.ucPhoneNo[3]=0x19;
    pstStaPhoneNo->stPhoneNo.ucPhoneNo[4]=0x81;
    pstStaPhoneNo->stPhoneNo.ucPhoneNo[5]=0x4F;
    pstStaPhoneNo->stPhoneNo.ucPhoneNo[6]=0xFF;
    pstStaPhoneNo->stPhoneNo.ucPhoneNo[7]=0xFF;
    pstStaPhoneNo->stSMSCenter.ucPhoneNo[0]=0x86;           //�������ĺ���
    pstStaPhoneNo->stSMSCenter.ucPhoneNo[1]=0x13;           //8613800100500
    pstStaPhoneNo->stSMSCenter.ucPhoneNo[2]=0x80;
    pstStaPhoneNo->stSMSCenter.ucPhoneNo[3]=0x01;
    pstStaPhoneNo->stSMSCenter.ucPhoneNo[4]=0x00;
    pstStaPhoneNo->stSMSCenter.ucPhoneNo[5]=0x50;
    pstStaPhoneNo->stSMSCenter.ucPhoneNo[6]=0x0F;
    pstStaPhoneNo->stSMSCenter.ucPhoneNo[7]=0xFF;
            ucRet = RET_ERROR;
        }
    }
    return ucRet;
}
//������֤
unsigned char RunPara_InitTerEncrypt(STTERENCRYPT *pstTerEncrypt)
{
    unsigned char ucRet=RET_SUCCESS;
    pstTerEncrypt->ucAlgoCode = CON_ENCRYPT_DIR;
    memset(pstTerEncrypt->ucPassword,0,4);
    memset(pstTerEncrypt->ucPassword_New,0,16);
    memset(pstTerEncrypt->ucAdminPassword,0,4);
    memset(pstTerEncrypt->ucErrPassword,0,4);
    pstTerEncrypt->ucErrTimes=0;
    pstTerEncrypt->ulLockTime=0;
    return ucRet;
}
//F6: �ն����ַ
unsigned char RunPara_InitTerGroupAddrList(STTERGROUPADDRLIST *pstTerGroupAddrList)
{
    unsigned char ucRet=RET_SUCCESS,i=0;

    for(i=0;i<CON_GROUPADDRNUM;i++)
        memset(pstTerGroupAddrList->stGroupAddr[i].ucGroupAddr,0,CON_GROUPADDRLEN);

    return ucRet;
}

unsigned char RunPara_InitCommStat(STTERCOMMSTAT *pstTerCommStat)
{
    unsigned char ucRet=RET_SUCCESS;
    pstTerCommStat->ucTerAutoUpFlag = CON_TERCOMMSTAT_AUTOUP;
    pstTerCommStat->ucTerTalkFlag   = CON_TERCOMMSTAT_TALK_NO;
    return ucRet;
}
//�澯������
unsigned char RunPara_InitAlrCtrlWord(STALRCTRLWORD *pstAlrCtrlWord)
{
    unsigned char ucRet=RET_SUCCESS;
    Clr_AlrCategory_Val(&(pstAlrCtrlWord->stAlrShield),CON_ALR_SHIELD_NOT);
    Clr_AlrCategory_Val(&(pstAlrCtrlWord->stAlrACD),CON_ALR_ACD);
    return ucRet;
}
//�澯����?����״̬��?
unsigned char Clr_AlrCategory_Val(STALRCATEGORY *pstAlrCategory,unsigned char ucVal)
{
    unsigned char ucRet=RET_SUCCESS;
    memset(pstAlrCategory,ucVal,sizeof(STALRCATEGORY));
    return ucRet;
}

//unsigned char RunPara_initAlrMaxCount(STTERALARMJUDGECOUNT *pstTerAlrMaxCount)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  pstTerAlrMaxCount->ucAlrCount_PolarityReverseA  = 3;    //A�����������
//  pstTerAlrMaxCount->ucAlrCount_PolarityReverseB  = 3;    //B�����������
//  pstTerAlrMaxCount->ucAlrCount_PolarityReverseC  = 3;    //C�����������
//
////    pstTerAlrMaxCount->ucAlrCount_ShortCircuitA1    = 9;    //A��1�β�CT��·
////    pstTerAlrMaxCount->ucAlrCount_ShortCircuitB1    = 9;    //B��1�β�CT��·
////    pstTerAlrMaxCount->ucAlrCount_ShortCircuitC1    = 9;    //C��1�β�CT��·
////
////    pstTerAlrMaxCount->ucAlrCount_ShortCircuitA2    = 9;    //A��2�β�CT��·
////    pstTerAlrMaxCount->ucAlrCount_ShortCircuitB2    = 9;    //B��2�β�CT��·
////    pstTerAlrMaxCount->ucAlrCount_ShortCircuitC2    = 9;    //C��2�β�CT��·
////
////    pstTerAlrMaxCount->ucAlrCount_OpenCircuitA2     = 9;    //A��2�β�CT��·
////    pstTerAlrMaxCount->ucAlrCount_OpenCircuitB2     = 9;    //B��2�β�CT��·
////    pstTerAlrMaxCount->ucAlrCount_OpenCircuitC2     = 9;    //C��2�β�CT��·
//
///*
//  pstTerAlrMaxCount->ucAlrCount_VolABreak         = 3;    //A���ѹ����
//  pstTerAlrMaxCount->ucAlrCount_VolBBreak         = 3;    //B���ѹ����
//  pstTerAlrMaxCount->ucAlrCount_VolCBreak         = 3;    //C���ѹ����
//
//  pstTerAlrMaxCount->ucAlrCount_VolALack          = 3;    //A���ѹȱ��
//  pstTerAlrMaxCount->ucAlrCount_VolBLack          = 3;    //B���ѹȱ��
//  pstTerAlrMaxCount->ucAlrCount_VolCLack          = 3;    //C���ѹȱ��
//*/
//  pstTerAlrMaxCount->ucAlrCount_VolABreak         = 2;    //A���ѹ����       //ԭ��1���ӣ��ָ�Ϊ5����
//  pstTerAlrMaxCount->ucAlrCount_VolBBreak         = 2;    //B���ѹ����
//  pstTerAlrMaxCount->ucAlrCount_VolCBreak         = 2;    //C���ѹ����
//
//  pstTerAlrMaxCount->ucAlrCount_VolALack          = 2;    //A���ѹȱ��
//  pstTerAlrMaxCount->ucAlrCount_VolBLack          = 2;    //B���ѹȱ��
//  pstTerAlrMaxCount->ucAlrCount_VolCLack          = 2;    //C���ѹȱ��
//
//  pstTerAlrMaxCount->ucAlrCount_VolCirReverse     = 5;    //��ѹ��·������
//
//  pstTerAlrMaxCount->ucAlrCount_IUnBalance        = 4;    //������ƽ���Խ��
//  pstTerAlrMaxCount->ucAlrCount_UUnBalance        = 2;    //��ѹ��ƽ���Խ��
//
//  pstTerAlrMaxCount->ucAlrCount_VolAErrDown       = 2;    //A���ѹԽ������
//  pstTerAlrMaxCount->ucAlrCount_VolBErrDown       = 2;    //B���ѹԽ������
//  pstTerAlrMaxCount->ucAlrCount_VolCErrDown       = 2;    //C���ѹԽ������
//
//  pstTerAlrMaxCount->ucAlrCount_VolAErrUp         = 2;    //A���ѹԽ������
//  pstTerAlrMaxCount->ucAlrCount_VolBErrUp         = 2;    //B���ѹԽ������
//  pstTerAlrMaxCount->ucAlrCount_VolCErrUp         = 2;    //C���ѹԽ������
//
//  pstTerAlrMaxCount->ucAlrCount_CurAErrUp         = 2;    //A�����Խ������
//  pstTerAlrMaxCount->ucAlrCount_CurBErrUp         = 2;    //B�����Խ������
//  pstTerAlrMaxCount->ucAlrCount_CurCErrUp         = 2;    //C�����Խ������
//
//  pstTerAlrMaxCount->ucAlrCount_CurANorUp         = 2;    //A�����Խ����
//  pstTerAlrMaxCount->ucAlrCount_CurBNorUp         = 2;    //B�����Խ����
//  pstTerAlrMaxCount->ucAlrCount_CurCNorUp         = 2;    //C�����Խ����
//
//  pstTerAlrMaxCount->ucAlrCount_SPowNorUp         = 2;    //���ڹ���Խ����
//  pstTerAlrMaxCount->ucAlrCount_SPowErrUp         = 2;    //���ڹ���Խ������
//
//  return ucRet;
//}
//�����ն˶���ȱ�����
//unsigned char   RunPara_SetTerVolLackAlrMaxCount(unsigned char ucCount)
//{
//    unsigned char ucRet=RET_SUCCESS;
//    if(ucCount==0)
//        ucCount=1;
//    stTerRunPara.stTerAlrMaxCount.ucAlrCount_VolABreak = ucCount;
//    stTerRunPara.stTerAlrMaxCount.ucAlrCount_VolBBreak = ucCount;
//    stTerRunPara.stTerAlrMaxCount.ucAlrCount_VolCBreak = ucCount;
//
//    stTerRunPara.stTerAlrMaxCount.ucAlrCount_VolALack = ucCount;
//    stTerRunPara.stTerAlrMaxCount.ucAlrCount_VolBLack = ucCount;
//    stTerRunPara.stTerAlrMaxCount.ucAlrCount_VolCLack = ucCount;
//    return ucRet;
//}
//F25�������������F26����������ֵ����
unsigned char RunPara_InitMPInfo(STMPINFO *pstMPInfo)
{
    unsigned char ucRet=RET_SUCCESS;

    pstMPInfo->ucMPType = CON_MPTYPE_NULL;
    RunPara_InitMPRateVal(&(pstMPInfo->stMPRateVal));       //���ѹ������������
    RunPara_InitMPAlterRate(&(pstMPInfo->stMPAlterRate));   //PT��CT���
    RunPara_InitMPLimit(&(pstMPInfo->stMPLimit));           //��������ֵ
    RunPara_InitMPFreezePara(&(pstMPInfo->stMPFreezePara[0]));
    RunPara_InitMPFreezeDay(&(pstMPInfo->stFreezeDay));
    RunPara_InitMPPowRatePara(&(pstMPInfo->stPowRatePara)); //F28�������㹦�������ֶ���ֵ
    RunPara_InitMPTongsun(&(pstMPInfo->stTongsunTiansun));  //ͭ������

    memset(pstMPInfo->ucDevNo,0xFF,4);

    return ucRet;
}
//F25���ѹ������������
void    RunPara_InitMPRateVal(STMPRATEVAL *pstMPRateVal)
{
    pstMPRateVal->ucMPLineType  = CON_METERTYPE_3P4L;
    pstMPRateVal->ucMPPhaseNo   = CON_METER_PHASE_INCERTERN;
    pstMPRateVal->usRateCur     = 500;          //0.01 A
    pstMPRateVal->usRateVol     = 2200;         //0.1  V
    pstMPRateVal->ulRateP       = 33000;        //0.0001 KVA

}
//F25PT��CT���
void RunPara_InitMPAlterRate(STMPALTERRATE *pstMPAlterRate)
{
    pstMPAlterRate->usVolAlterRate=1;
    pstMPAlterRate->usCurAlterRate=1;
}
//F26����������ֵ����
void RunPara_InitMPLimit(STMPLIMIT *pstMPLimit)
{
    pstMPLimit->usVolUpLimit        =230;
    pstMPLimit->usVolDownLimit      =210;
    pstMPLimit->usVolBreakLimit     =50;
    pstMPLimit->usVolErrUpLimit     =250;
    pstMPLimit->usVolErrDownLimit   =190;

    pstMPLimit->usVolUpLimit_2      =2300;
    pstMPLimit->usVolDownLimit_2    =2100;
    pstMPLimit->usVolBreakLimit_2   =500;
    pstMPLimit->usVolErrUpLimit_2   =2500;
    pstMPLimit->usVolErrDownLimit_2 =1900;

    pstMPLimit->usCurErrUpLimit     =1000;
    pstMPLimit->usCurUpLimit        =600;
    pstMPLimit->usCur0UpLimit       =1000;
    pstMPLimit->ulPowErrUpLimit     =30000;
    pstMPLimit->ulPowUpLimit        =10000;
    pstMPLimit->usVolUnBalance      =400;
    pstMPLimit->usCurUnBalance      =1000;
    pstMPLimit->usLostVolTime       =5;


    pstMPLimit->ucVolErrUpTime      =15;
    pstMPLimit->ucVolErrDownTime    =15;

    pstMPLimit->ucCurErrUpTime      =15;
    pstMPLimit->ucCurUpTime         =15;
    pstMPLimit->ucCur0UpTime        =15;
    pstMPLimit->ucPowErrUpTime      =15;
    pstMPLimit->ucPowUpTime         =15;
    pstMPLimit->ucVolUnBalanceTime  =15;
    pstMPLimit->ucCurUnBalanceTime  =15;

    pstMPLimit->ucVolErrUpBack      =900;
    pstMPLimit->ucVolErrDownBack    =1100;
    pstMPLimit->ucCurErrUpBack      =900;
    pstMPLimit->ucCurUpBack         =900;
    pstMPLimit->ucCur0UpBack        =900;
    pstMPLimit->ucPowErrUpBack      =900;
    pstMPLimit->ucPowUpBack         =900;
    pstMPLimit->ucVolUnBalanceBack  =900;
    pstMPLimit->ucCurUnBalanceBack  =900;
}
////ʧѹ����ʱ��
//unsigned short RunPara_GetMPLimit_usLostVolTime(unsigned char ucMPNo)
//{
//    if(ucMPNo>=CON_MAX_MPNUM)
//        ucMPNo=0;
//    return stTerRunPara.stMPInfo[ucMPNo].stMPLimit.usLostVolTime;
//}

void RunPara_InitMPFreezePara(STMPFREEZEPARA *pstMPFreezePara)
{//Ĭ�����ã����������
    unsigned char i=0;
    for(i=0;i<CON_MAX_MTRFREEZENUM;i++)
    {
        if((i+89)<=116 && (i+89)!=104)
        {
            pstMPFreezePara[i].ucFreezeInter =CON_TIMEINTER_15MIN;
            pstMPFreezePara[i].ucItem =i+89;
        }else
        {
            pstMPFreezePara[i].ucFreezeInter =CON_TIMEINTER_INVALID;
            pstMPFreezePara[i].ucItem =0xFF;
        }
    }
}
//������ʱ��
void RunPara_InitMPFreezeDay(STFREEZEDAY *pstFreezeDay)
{
    unsigned char i=0;
    for(i=0;i<CON_MAX_FREEZEDAYNUM;i++)
        ClearDateTime(&(pstFreezeDay->stFreezeDay[i]));
    pstFreezeDay->stFreezeDay[0].ucYear =0x05;
    pstFreezeDay->stFreezeDay[0].ucMonth=0x01;
    pstFreezeDay->stFreezeDay[0].ucDay  =28;
    pstFreezeDay->stFreezeDay[0].ucHour =2;
    pstFreezeDay->stFreezeDay[0].ucMinute=7;
    pstFreezeDay->stFreezeDay[0].ucSecond =0;
}
//���������ֶ���ֵ
void RunPara_InitMPPowRatePara(STPOWRATEPARA *pstPowRatePara)
{
    pstPowRatePara->usPowRateLimit1=850;            //85.0%
    pstPowRatePara->usPowRateLimit2=950;            //95.0%
}

//F27��������ͭ���������
void RunPara_InitMPTongsun(STTONGSUNTIANSUNPARA *pstTongsun)
{
    RunPara_InitCuFeLostPara(&(pstTongsun->stCuFeLostParaA));
    RunPara_InitCuFeLostPara(&(pstTongsun->stCuFeLostParaB));
    RunPara_InitCuFeLostPara(&(pstTongsun->stCuFeLostParaC));
}
//F27��������ͭ���������
unsigned char RunPara_InitCuFeLostPara(STCUFELOSTPARA *pstCuFeLostPara)
{
    unsigned char ucRet = RET_SUCCESS;

    pstCuFeLostPara->R = 0;
    pstCuFeLostPara->X = 0;
    pstCuFeLostPara->G = 0;
    pstCuFeLostPara->B = 0;
    return ucRet;
}
unsigned char RunPara_InitConfigNum(STCONFIGNUM *pstConfigNum)
{
    unsigned char ucRet=RET_SUCCESS;
    pstConfigNum->ucMeterNum=1;
    pstConfigNum->ucPulseNum=4;
    pstConfigNum->ucAnalogNum=0;
    pstConfigNum->ucSumGNum=1;
    return ucRet;
}

//unsigned char RunPara_InitPulsePara(STPULSEPARA *pstPulsePara)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	
//	pstPulsePara->ucAttr	=CON_PULSEATTR_INVALID;
//	pstPulsePara->ucPortNo	=0;
//	pstPulsePara->usConst	=3200;
//	
//	return ucRet;
//}

unsigned char RunPara_InitMeterPara(STMETERPARA *pstMeterPara)
{
    unsigned char ucRet=RET_SUCCESS;

   pstMeterPara->ucProtocol    = CON_PROTOCOL_DL64507;
    pstMeterPara->stMeterAddr.ucMeterAddr[0]    = 0x01;
    pstMeterPara->stMeterAddr.ucMeterAddr[1]    = 0x00;
    pstMeterPara->stMeterAddr.ucMeterAddr[2]    = 0x00;
    pstMeterPara->stMeterAddr.ucMeterAddr[3]    = 0x00;
    pstMeterPara->stMeterAddr.ucMeterAddr[4]    = 0x00;
    pstMeterPara->stMeterAddr.ucMeterAddr[5]    = 0x00;

    pstMeterPara->ucPort        = 0;
    pstMeterPara->ucBaudCode    = 3;
    pstMeterPara->ulBaudRate    = 2400;
    pstMeterPara->ucEPN         = UART_EVEN_PARITY;//CON_UART_EPN_EVEN;
    pstMeterPara->ucDataBit     = 8;
    pstMeterPara->ucStopBit     = UART_1_STOP_BIT;//CON_UART_STPB_1;
    pstMeterPara->ucFeeNum      = 4;
    pstMeterPara->ucSetFeeNum   = 4;
    pstMeterPara->ucFloatNum    = 2;
    pstMeterPara->ucIntNum      = 6;

    memset(&pstMeterPara->stPassword.ucMeterPass,0x00,CON_METER_PASSLEN);
    memset(&pstMeterPara->stCommName.ucMeterCommName,0x00,CON_METER_NAMELEN);
//  memset(&pstMeterPara->ucPulseNo,0x00,CON_PULSE_NUM);

    memset(&pstMeterPara->ucMeterJuNum,0x20,12); //add by cfh �ֺ�Ϊ��
    memset(&pstMeterPara->ucCollectAddr,0,CON_COLLECT_ADDR_LEN);
    pstMeterPara->ucBigClass = 0;
    pstMeterPara->ucLittleClass = 0;

    pstMeterPara->ucEmpFlag = CON_STATUS_NO;
    return ucRet;
}

//unsigned char RunPara_InitSumGroupPara(STSUMGROUPPARA *pstSumGroupPara)
//{
//  unsigned char ucRet=RET_SUCCESS,i=0;
//
//  pstSumGroupPara->ucAvailbleFlag=CON_INVALIBLE_FLAG;
//  for(i=0;i<CON_MAX_MPNUM;i++)
//  {
//      pstSumGroupPara->stInstantPow.stDir.ucMPSum[i]  = CON_MPNO_INVALID;
//      pstSumGroupPara->stInstantPow.stDir.ucMPOper[i] = 0;
//      pstSumGroupPara->stInstantPow.stRev.ucMPSum[i]  = CON_MPNO_INVALID;
//      pstSumGroupPara->stInstantPow.stRev.ucMPOper[i] = 0;
//
//      pstSumGroupPara->stPowVal.stDir.ucMPSum[i]      = CON_MPNO_INVALID;
//      pstSumGroupPara->stPowVal.stDir.ucMPOper[i]     = 0;
//      pstSumGroupPara->stPowVal.stRev.ucMPSum[i]      = CON_MPNO_INVALID;
//      pstSumGroupPara->stPowVal.stRev.ucMPOper[i]     = 0;
//
//      pstSumGroupPara->stInstantVar.stDir.ucMPSum[i]  = CON_MPNO_INVALID;
//      pstSumGroupPara->stInstantVar.stDir.ucMPOper[i] = 0;
//      pstSumGroupPara->stInstantVar.stRev.ucMPSum[i]  = CON_MPNO_INVALID;
//      pstSumGroupPara->stInstantVar.stRev.ucMPOper[i] = 0;
//
//      pstSumGroupPara->stVarVal.stDir.ucMPSum[i]      = CON_MPNO_INVALID;
//      pstSumGroupPara->stVarVal.stDir.ucMPOper[i]     = 0;
//      pstSumGroupPara->stVarVal.stRev.ucMPSum[i]      = CON_MPNO_INVALID;
//      pstSumGroupPara->stVarVal.stRev.ucMPOper[i]     = 0;
//  }
//
//  pstSumGroupPara->stInstantPow.stFreeze.ucFreezeInter    = CON_TIMEINTER_15MIN;
//  pstSumGroupPara->stInstantPow.stLimit.ulSumGUpLimit     = 0x7FFFFFFF;
//  pstSumGroupPara->stInstantPow.stLimit.ulSumGDownLimit   = 0;
//
//  pstSumGroupPara->stPowVal.stFreeze.ucFreezeInter        = CON_TIMEINTER_15MIN;
//  pstSumGroupPara->stPowVal.stLimit.ulSumGUpLimit         = 0x7FFFFFFF;
//  pstSumGroupPara->stPowVal.stLimit.ulSumGDownLimit       = 0;
//
//  pstSumGroupPara->stInstantVar.stFreeze.ucFreezeInter    = CON_TIMEINTER_15MIN;
//  pstSumGroupPara->stInstantVar.stLimit.ulSumGUpLimit     = 0x7FFFFFFF;
//  pstSumGroupPara->stInstantVar.stLimit.ulSumGDownLimit   = 0;
//
//  pstSumGroupPara->stVarVal.stFreeze.ucFreezeInter        = CON_TIMEINTER_15MIN;
//  pstSumGroupPara->stVarVal.stLimit.ulSumGUpLimit         = 0x7FFFFFFF;
//  pstSumGroupPara->stVarVal.stLimit.ulSumGDownLimit       = 0;
//
//  return ucRet;
//}
//unsigned char RunPara_InitSwitchPara(STSWITCHPARA* pstSwitchPara)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  pstSwitchPara->ucAttr=CON_PULSEATTR_INVALID;
//  pstSwitchPara->ucAlarm=CON_SWITCHALARM_IMP;
//  return ucRet;
//}

//unsigned char RunPara_InitAnalogPara(STANALOGPARA* pstAnalogPara)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  pstAnalogPara->ucAvailable = CON_INVALIBLE_FLAG;
//  pstAnalogPara->ucAttr=CON_ANALOGTYPE_VA;
//  pstAnalogPara->ucMPNo=CON_MPNO_INVALID;
//  pstAnalogPara->ucM = 1;
//  RunPara_InitDT2(&(pstAnalogPara->stAnalogRate.stStart));
//  RunPara_InitDT2(&(pstAnalogPara->stAnalogRate.stEnd));
//  RunPara_InitDT2(&(pstAnalogPara->stAnalogLimit.stUpLimit));
//  RunPara_InitDT2(&(pstAnalogPara->stAnalogLimit.stDownLimit));
//  return ucRet;
//}
//unsigned char RunPara_InitDT2(STDT2 *pstDT2)
//{
//  pstDT2->dVal = CONINVALIDVAL;
//  pstDT2->usDT2 = CONINVALIDVAL;
//  return RET_SUCCESS;
//}
unsigned char RunPara_InitLinkPara(STLINKPARA *pstLinkPara)
{
    unsigned char ucRet = RET_SUCCESS,i=0;

    pstLinkPara->ucPortNo = 0;
    pstLinkPara->ucByteOverTime = 0;
    pstLinkPara->ucRecvOverTime = 0;
    pstLinkPara->ucInterTime = 0;
    pstLinkPara->ucSlaveFlag = CON_LINK_INVALID;
    pstLinkPara->ucSlaveNum = 0;
    for(i=0;i<CON_SLAVE_NUM;i++)
        memset(&(pstLinkPara->stSlaveAddr[i]),0,sizeof(STSLAVEADDR));
    RunPara_InitPortPara(&(pstLinkPara->stLinkPortPara));
    RunPara_InitRemove(&(pstLinkPara->stLinkRemove));
    return ucRet;
}
unsigned char RunPara_InitRemove(STLINKREMOVE   *pstLinkRemove)
{
    unsigned char i =0;
    pstLinkRemove->ucFlag = CON_AVALIBLE_FLAG;
    pstLinkRemove->ucTransNum = 0;

    for(i=0;i<CON_TRANS_ADDR_NUM;i++)
        memset(&(pstLinkRemove->stTransAddr[i]),0,sizeof(STTERDEVADDR));

    return RET_SUCCESS;
}
/******************************************************************************
** ��������: U8 RunPara_InitPortPara(STPORTPROP *pstPortProp)
** ��������: ��ʼ���˿�����
** ��ڲ���: STPORTPROP *pstPortProp
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
U8 RunPara_InitPortPara (STPORTPROP *pstPortProp)
{
    U8 ucRet = RET_SUCCESS;

    pstPortProp->ucBaudRate = 6;            //9600
    pstPortProp->ucParity = UART_EVEN_PARITY;//CON_UART_EPN_EVEN;
    pstPortProp->ucStopBits = 1;
    pstPortProp->ucByteSize = 8;

    return ucRet;
}
/******************************************************************************
** ��������: U8 RunPara_InitAuxPara (STTERAUXPARA *pstTerAuxPara)
** ��������: ��ʼ����Ӳ������Լ�汾���������ڣ����ұ��
** ��ڲ���: STPORTPROP *pstPortProp
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
U8 RunPara_InitAuxPara (STTERAUXPARA *pstTerAuxPara)
{
    U8 ucRet=RET_SUCCESS;
    U8 i=0;
    U8 ucVerInfo[7];
    pstTerAuxPara->ucManufactor[0]=CON_TER_MANUFACTORNO_0;      //���̱�� "8000",ascii��
    pstTerAuxPara->ucManufactor[1]=CON_TER_MANUFACTORNO_1;
    pstTerAuxPara->ucManufactor[2]=CON_TER_MANUFACTORNO_2;
    pstTerAuxPara->ucManufactor[3]=CON_TER_MANUFACTORNO_3;

    RunPara_GetTerVerInfo(ucVerInfo);
    pstTerAuxPara->ucSoftwareVer[0]=ucVerInfo[0];
    pstTerAuxPara->ucSoftwareVer[1]=ucVerInfo[1];
    pstTerAuxPara->ucSoftwareVer[2]=ucVerInfo[2];
    pstTerAuxPara->ucSoftwareVer[3]=ucVerInfo[3];
    pstTerAuxPara->ucSoftwareDate[0]=ucVerInfo[4];
    pstTerAuxPara->ucSoftwareDate[1]=ucVerInfo[5];
    pstTerAuxPara->ucSoftwareDate[2]=ucVerInfo[6];

    RunPara_GetTerHardwareVerInfo(ucVerInfo);
    pstTerAuxPara->ucHardwareVer[0]=ucVerInfo[0];
    pstTerAuxPara->ucHardwareVer[1]=ucVerInfo[1];
    pstTerAuxPara->ucHardwareVer[2]=ucVerInfo[2];
    pstTerAuxPara->ucHardwareVer[3]=ucVerInfo[3];
    pstTerAuxPara->ucHardwareDate[0]=ucVerInfo[4];
    pstTerAuxPara->ucHardwareDate[1]=ucVerInfo[5];
    pstTerAuxPara->ucHardwareDate[2]=ucVerInfo[6];

    RunPara_GetTerProtoolVerInfo(ucVerInfo);
    pstTerAuxPara->ucProtoolVer[0]=ucVerInfo[0];
    pstTerAuxPara->ucProtoolVer[1]=ucVerInfo[1];
    pstTerAuxPara->ucProtoolVer[2]=ucVerInfo[2];
    pstTerAuxPara->ucProtoolVer[3]=ucVerInfo[3];

	pstTerAuxPara->ucDevNo[0] = CON_TER_DEVNO_0;
	pstTerAuxPara->ucDevNo[1] = CON_TER_DEVNO_1;
	pstTerAuxPara->ucDevNo[2] = CON_TER_DEVNO_2;
	pstTerAuxPara->ucDevNo[3] = CON_TER_DEVNO_3;
	pstTerAuxPara->ucDevNo[4] = CON_TER_DEVNO_4;
	pstTerAuxPara->ucDevNo[5] = CON_TER_DEVNO_5;				
	pstTerAuxPara->ucDevNo[6] = CON_TER_DEVNO_6;
	pstTerAuxPara->ucDevNo[7] = CON_TER_DEVNO_7;

    memset(pstTerAuxPara->ucConfigInfo,0x30,11);
    for(i=0;i<8;i++)                            //�豸����ж�
    {
        if((pstTerAuxPara->ucDevNo[i]>='a' && pstTerAuxPara->ucDevNo[i]<='z')
            || (pstTerAuxPara->ucDevNo[i]>='A' && pstTerAuxPara->ucDevNo[i]<='Z')
            || (pstTerAuxPara->ucDevNo[i]>='0' && pstTerAuxPara->ucDevNo[i]<='9')
            || (pstTerAuxPara->ucDevNo[i]=='-')
            || (pstTerAuxPara->ucDevNo[i]=='_')
            || (pstTerAuxPara->ucDevNo[i]=='*'))
        {
            ;
        }else
            pstTerAuxPara->ucDevNo[i]='0';
    }
    memset(pstTerAuxPara->ucLoginPass,0x11,2);

    return ucRet;
}

//unsigned char   RunPara_InitTerDevAddr(STTERDEVADDR *pstTerAddr)
//{
//    unsigned char ucRet=RET_SUCCESS;
//    pstTerAddr->ucDevAddr[0]=0x81;
//    pstTerAddr->ucDevAddr[1]=0x07;
//    return ucRet;
//}

unsigned char   RunPara_InitTerRegionAddr(STTERREGIONADDR *pstTerRegionAddr)
{
    unsigned char ucRet=RET_SUCCESS;
    pstTerRegionAddr->ucRegionAddr[0]=0x11;
    pstTerRegionAddr->ucRegionAddr[1]=0x11;
    return ucRet;
}

//��ʼ����������ַ��������������ն˵�ַ
U8 RunPara_InitTerDev_Region_RegionAddr (U8 ucClrFlag)
{
    U8 ucRet;
    U8 pret_mem[12];
    U16 crc16;

	if(ucClrFlag != CON_PARA_INIT_POW_ON)
	{
		return RET_ERROR;
	}
    ucRet = RET_SUCCESS;
    file_read(F207_ADDR_ADDR, pret_mem, 12);
    crc16 = calc_crc16(pret_mem, 4, 0xffff);      //����ͨ�Ų������ݵ�CRC
    if(crc16 == ((U16)pret_mem[4] << 8) + pret_mem[5])
    {
        if((pret_mem[0] < 0x9a) && ((pret_mem[0] & 0x0f) < 0x0a) && (pret_mem[1] < 0x9a) && ((pret_mem[1] & 0x0f) < 0x0a))
        {
            stTerRunPara.stTerRegionAddr.ucRegionAddr[0] = pret_mem[0];
            stTerRunPara.stTerRegionAddr.ucRegionAddr[1] = pret_mem[1];
        }
        else
        {
            stTerRunPara.stTerRegionAddr.ucRegionAddr[0] = 0x71;
            stTerRunPara.stTerRegionAddr.ucRegionAddr[1] = 0x05;
        }
        stTerRunPara.stTerAddr.ucDevAddr[0] = pret_mem[2];
        stTerRunPara.stTerAddr.ucDevAddr[1] = pret_mem[3];
    }
    else
    {
        crc16 = calc_crc16(pret_mem + 6, 4, 0xffff);      //������վIP��ַ�Ͷ˿ڵ�crc
        if(crc16 == ((U16)pret_mem[6 + 4] << 8) + pret_mem[6 + 5])
        {
        if((pret_mem[6] < 0x9a) && ((pret_mem[6] & 0x0f) < 0x0a) && (pret_mem[7] < 0x9a) && ((pret_mem[7] & 0x0f) < 0x0a))
        {
            stTerRunPara.stTerRegionAddr.ucRegionAddr[0] = pret_mem[6];
            stTerRunPara.stTerRegionAddr.ucRegionAddr[1] = pret_mem[7];
        }
        else
        {
            stTerRunPara.stTerRegionAddr.ucRegionAddr[0] = 0x71;
            stTerRunPara.stTerRegionAddr.ucRegionAddr[1] = 0x05;
        }
        stTerRunPara.stTerAddr.ucDevAddr[0] = pret_mem[8];
        stTerRunPara.stTerAddr.ucDevAddr[1] = pret_mem[9];
        }
        else
        {
            stTerRunPara.stTerRegionAddr.ucRegionAddr[0]=0x71;
            stTerRunPara.stTerRegionAddr.ucRegionAddr[1]=0x05;
            stTerRunPara.stTerAddr.ucDevAddr[0]=0x01;
            stTerRunPara.stTerAddr.ucDevAddr[1]=0x00;
            ucRet = RET_ERROR;
        }
    }
    return ucRet;
}
//unsigned char   RunPara_InitJobPara(STJOBPARA *pstJobPara)
//{
//    unsigned char ucRet=RET_SUCCESS;
//    STDATETIME  stCurrTime=GetCurrentTime();
//    stCurrTime.ucSecond=0;
//    stCurrTime.ucMinute=0;
//    stCurrTime.ucHour=0;
//    stCurrTime.ucDay=1;
//    pstJobPara->ucActive=CON_JOB_NOACTIVE;
//    pstJobPara->ucTimeInter=15;                 //ʱ����
//    pstJobPara->ucTimeUnit=CON_TIMEUNIT_MIN;    //ʱ�䵥λ
//    pstJobPara->stStartActive=stCurrTime;       //���ͻ�׼ʱ��
//    pstJobPara->ucResendDataInter=1;            //���߳�ȡ���ݱ���
//    pstJobPara->ucDUIDNum=0;                    //���ݵ�Ԫ����
//    pstJobPara->stDUID[0].ucDA1=0;              //���ݵ�Ԫ
//    pstJobPara->stDUID[0].ucDA2=0;
//    pstJobPara->stDUID[0].ucDT1=0;
//    pstJobPara->stDUID[0].ucDT2=0;
//    return ucRet;
//}

U8 RunPara_InitJobPara_eeprom(U8 ucjob_num, U8 ucjob_class, U8 ucflag)
{
    U8 ucRet=RET_SUCCESS,i=0, mem_type;
    U8 tmp;
    U16 crc16, len;
    U8 *pret_mem = NULL;
    U32 addr, Active_flag_addr;
    STJOBPARA *pstJobPara;
    STDATETIME  stCurrTime;

    if(ucjob_class == 0)
    {
        if(ucjob_num >= CONDATAJOBNUM)//F65_UPDATA1JOB_NUM)
        {
            return RET_ERROR;
        }
        pstJobPara = &stTerRunPara.stJobPara_Data1[ucjob_num];
        addr = F65_UPDATA1PARA_ADDR + (F65_UPDATA1PARA_LEN + 2) * ucjob_num;
        len = F65_UPDATA1PARA_LEN;
        Active_flag_addr = F67_UPDATA1CTRL_ADDR + ucjob_num;
    }
    else if(ucjob_class == 1)
    {
        if(ucjob_num >= CONDATAJOBNUM)//F66_UPDATA2JOB_NUM)
        {
            return RET_ERROR;
        }
        pstJobPara = &stTerRunPara.stJobPara_Data2[ucjob_num];
        addr = F66_UPDATA2PARA_ADDR + (F66_UPDATA2PARA_LEN + 2) * ucjob_num;
        len = F66_UPDATA2PARA_LEN;
        Active_flag_addr = F68_UPDATA2CTRL_ADDR + ucjob_num;
    }
    else
    {
        return RET_ERROR;
    }
    if(ucflag == CON_PARA_INIT_POW_ON)
    {
        pret_mem = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR0);
        if(pret_mem != NULL)
        {

            file_read (addr, pret_mem, len + 2);
            tmp = min(pret_mem[8],CONDATAUNITNUM) * 4 + 9;
            crc16 = calc_crc16(pret_mem, tmp, 0xffff);      //����ͨ�Ų������ݵ�CRC
            if(crc16 == ((U16)pret_mem[tmp] << 8) + pret_mem[tmp + 1])
            {
                file_read (Active_flag_addr, &tmp, 1);
                if(tmp == CON_JOB_ACTIVE)
                {
                    pstJobPara->ucActive = CON_JOB_ACTIVE;
                }
                else
                {
                pstJobPara->ucActive=CON_JOB_NOACTIVE;
                }
                pstJobPara->ucTimeInter = pret_mem[0]& 0x3f;        //ʱ����,��6λ
                pstJobPara->ucTimeUnit = (pret_mem[0] >> 6) + 2;    //ʱ�䵥λ,��2λ����Լ��0�������ӣ��ṹ����2�������ӣ�����+2
                pstJobPara->stStartActive.ucSecond = bcd_to_hex(pret_mem[1]);   //���ͻ�׼ʱ��
                pstJobPara->stStartActive.ucMinute = bcd_to_hex(pret_mem[2]);
                pstJobPara->stStartActive.ucHour   = bcd_to_hex(pret_mem[3]);
                pstJobPara->stStartActive.ucDay    = bcd_to_hex(pret_mem[4] & 0x3F);
                pstJobPara->stStartActive.ucMonth  = bcd_to_hex(pret_mem[5] & 0x1F);
                pstJobPara->stStartActive.ucYear   = bcd_to_hex(pret_mem[6]);
                pstJobPara->ucResendDataInter = pret_mem[7];            //���߳�ȡ���ݱ���
                pstJobPara->ucDUIDNum= min(pret_mem[8],CONDATAUNITNUM);     //���ݵ�Ԫ����
                for(i = 0; i < pstJobPara->ucDUIDNum; i++)
                {
                    pstJobPara->stDUID[i].ucDA1=pret_mem[9 + 4 * i];    //���ݵ�Ԫ
                    pstJobPara->stDUID[i].ucDA2=pret_mem[10 + 4 * i];
                    pstJobPara->stDUID[i].ucDT1=pret_mem[11 + 4 * i];
                    pstJobPara->stDUID[i].ucDT2=pret_mem[12 + 4 * i];
                }
                free_mem_block(pret_mem);//MEM_256_TYPE
                return ucRet;
            }

            free_mem_block(pret_mem);//MEM_256_TYPE
        }
    }
    else
    {
        i = 0xff;
        file_write(addr + 1, &i, 1);    //�����Ϊ0xff������crcУ����޷�ͨ��
    }
    stCurrTime=GetCurrentTime();
    stCurrTime.ucSecond=0;
    stCurrTime.ucMinute=0;
    stCurrTime.ucHour=0;
    stCurrTime.ucDay=1;
    pstJobPara->ucActive=CON_JOB_NOACTIVE;
    pstJobPara->ucTimeInter=15;                 //ʱ����
    pstJobPara->ucTimeUnit=CON_TIMEUNIT_MIN;    //ʱ�䵥λ
    pstJobPara->stStartActive=stCurrTime;       //���ͻ�׼ʱ��
    pstJobPara->ucResendDataInter=1;            //���߳�ȡ���ݱ���
    pstJobPara->ucDUIDNum=0;                    //���ݵ�Ԫ����
    pstJobPara->stDUID[0].ucDA1=0;              //���ݵ�Ԫ
    pstJobPara->stDUID[0].ucDA2=0;
    pstJobPara->stDUID[0].ucDT1=0;
    pstJobPara->stDUID[0].ucDT2=0;
    return ucRet;
}

unsigned char RunPara_InitJobFnPara(STJOBFNPARA *pstJobFnPara)
{
    unsigned char ucRet = RET_SUCCESS,i,j;

    for(i=0;i<CON_UPUSR_NUM;i++)
    {
        for(j=0;j<CON_LITUSR_NUM;j++)
        {
            memset(pstJobFnPara->stUpUsr[i].stLitType[j].ucBitFlag,0,32);
        }
    }
    return ucRet;
}
//unsigned char RunPara_InitHarmLimit(STHARMVALUPPARA *pstHarmLimitPara)
//{
//  unsigned char i = 0;
//  if(pstHarmLimitPara != NULL)
//  {
//      pstHarmLimitPara->usHarmValUpTotal = 50;
//      pstHarmLimitPara->usHarmValUpOdd = 50;
//      pstHarmLimitPara->usHarmValUpEven = 50;
//
//      for(i=0;i<18;i++)
//          (*(unsigned short *)(&(pstHarmLimitPara->usHarmValUpLimit2)+i)) = 50;
//  }
//  return RET_SUCCESS;
//}
void RunPara_InitJobRunInfo(U8 ucflag)//azh �ϵ��ȫ���ʼ��Ҫ�ֿ�
{
    unsigned char i=0;
    
    if(ucflag==CON_PARA_INIT_POW_ON)//�ϵ絼��
    {
        for(i=0;i<CONDATAJOBNUM;i++)
        {
            GetJobRunInfoFormEE(0,i);
            GetJobRunInfoFormEE(1,i);
        }        
    }
    else
    {
        for(i=0;i<CONDATAJOBNUM;i++)
        {
            RunPara_InitJobRunInfoPer(&stTerRunPara.stJobPara_Data1[i]);
            SaveJobRunInfo(0,i);//azh ���������´�����ʱ��
        }
        for(i=0;i<CONDATAJOBNUM;i++)
        {
            RunPara_InitJobRunInfoPer(&stTerRunPara.stJobPara_Data2[i]);
            SaveJobRunInfo(1,i);//azh ���������´�����ʱ��
        }
    }
}
//��ʼ������Ч��ʶ������´��ϱ���ʱ��
unsigned char RunPara_InitJobRunInfoPer(STJOBPARA *pstJobPara)
{
    unsigned char ucRet=RET_SUCCESS;
    if(pstJobPara->ucActive==CON_JOB_ACTIVE)
        pstJobPara->stNextExecute=RunPara_GetJobNextTime(
        pstJobPara->stStartActive,
        pstJobPara->ucTimeInter,
        pstJobPara->ucTimeUnit);
    return ucRet;
}
unsigned char RunPara_InitPortPropList(STPORTLIST *pstPortPropList, U8 ucflag)
{
    unsigned char ucRet=RET_SUCCESS,i;
    for(i=0;i<CON_COMMPORT_NUM;i++)
    {
        pstPortPropList->ucNo[i] = i;
        RunPara_InitPortProp(&(pstPortPropList->stPortProp[i]));
    }
    RunPara_InitRMRunPara(ucflag);
    return ucRet;
}
unsigned char RunPara_InitPortProp(STPORTPROP *pstPortProp)
{
    pstPortProp->ucBaudRate = 4;
    pstPortProp->ucByteSize = 8;
    pstPortProp->ucParity = UART_EVEN_PARITY;//CON_UART_EPN_EVEN;
    pstPortProp->ucStopBits = UART_1_STOP_BIT;//azh CON_UART_STPB_1;
    pstPortProp->ulBaudRate = 1200;
    return RET_SUCCESS;
}
//F33
unsigned char RunPara_InitRMRunPara(U8 ucflag)
{
    unsigned char i = 0,j=0,k=0,n=0;
    U8 mem_type;
    U8 tmp;
    U16 crc16;
    U8 *pret_mem = NULL;
    U32 ulVal=0;
	
	stTerRunPara.stPortList.stRMRunPara.ucComPortEBit  = 0x03;
    for(i=0;i<CON_PORT_NUM;i++)
    {
		if(ucflag == CON_PARA_INIT_POW_ON)
		{
			pret_mem = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR0);
			if(pret_mem != NULL)
			{
				file_read (F33_SUMGFREEZEPARA_ADDR + i * (F33_SUMGFREEZEPARA_LEN + 2), pret_mem, F33_SUMGFREEZEPARA_LEN + 2);
				tmp = min(pret_mem[13],24) * 4 + 14;
				crc16 = calc_crc16(pret_mem, tmp, 0xffff);      //����ͨ�Ų������ݵ�CRC
				if((pret_mem[0] == 0xaa) && (crc16 == (((U16)pret_mem[tmp] << 8) + pret_mem[tmp + 1])))
				{
					stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].usRMRunCtrlWord = pret_mem[1] + pret_mem[2]*0x100;
					stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ulRMDate = pret_mem[3] + (pret_mem[3+1]<<8) + (pret_mem[3+2]<<16) + (pret_mem[3+3]<<24);
					stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ucRMTime[1] = (U8)BCD_TO_HEX_1(pret_mem+7);
					stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ucRMTime[0] = (U8)BCD_TO_HEX_1(pret_mem+7+1);
					//���õĳ�����ʱ��Ҫ���ú�azh 
					if(i==0)
					{
                        ulVal =  stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ulRMDate; 
                        for(k=0;k<CON_MAX_FREEZEDAYNUM;k++)
                          {
                              if((ulVal&0x01)==0x01)
                              {
                                  stTerRunPara.stMPInfo[i].stFreezeDay.stFreezeDay[n].ucYear   =0x10;
                                  stTerRunPara.stMPInfo[i].stFreezeDay.stFreezeDay[n].ucMonth  =0x01;
                                  stTerRunPara.stMPInfo[i].stFreezeDay.stFreezeDay[n].ucDay    =k+1;
                                  stTerRunPara.stMPInfo[i].stFreezeDay.stFreezeDay[n].ucHour   =(U8)BCD_TO_HEX_1(pret_mem+7+1);
                                  stTerRunPara.stMPInfo[i].stFreezeDay.stFreezeDay[n].ucMinute =(U8)BCD_TO_HEX_1(pret_mem+7);
                                  stTerRunPara.stMPInfo[i].stFreezeDay.stFreezeDay[n].ucSecond =0;
                                  n++;
                              }
                              ulVal=ulVal>>1;
                          }					    
					}
					
					stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ucRMInval = pret_mem[9];
					stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ucRMBcastTInval[2] = (U8)BCD_TO_HEX_1(pret_mem+10);
					stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ucRMBcastTInval[1] = (U8)BCD_TO_HEX_1(pret_mem+10+1);
					stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ucRMBcastTInval[0] = (U8)BCD_TO_HEX_1(pret_mem+10+2);
					stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ucRMWorkZoneNum = pret_mem[13];
					
					tmp = 14;
					if(pret_mem[13]!=0)
					{
						for(j=0; j<pret_mem[13]; j++)
						{
							stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].stRMWorkZone[j].usRMTStart[1] = (U8)BCD_TO_HEX_1(pret_mem + tmp);
							stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].stRMWorkZone[j].usRMTStart[0] = (U8)BCD_TO_HEX_1(pret_mem + tmp + 1);
							stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].stRMWorkZone[j].usRMTEnd[1] = (U8)BCD_TO_HEX_1(pret_mem	+ tmp + 2);
							stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].stRMWorkZone[j].usRMTEnd[0] = (U8)BCD_TO_HEX_1(pret_mem + tmp + 3);
							tmp += 4;
						}
					}
				}
				else
				{
					stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].usRMRunCtrlWord = 0x01;
					stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ucRMWorkZoneNum = 1;
					stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ucRMInval = 5;       //cfh 10-04-27
					stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ucRMTime[0] = 1;     //cfh 10-04-27
					stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ucRMTime[1] = 0;     //cfh 10-04-27
					stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ulRMDate = 1;        //cfh 10-04-27
					stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ucRMBcastTInval[0] = 0;  //cfh 10-04-27
					stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ucRMBcastTInval[1] = 0;  //cfh 10-04-27
					stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ucRMBcastTInval[2] = 0;  //cfh 10-04-27
					for(j=0;j<stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ucRMWorkZoneNum;j++)
					{
						stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].stRMWorkZone[j].usRMTStart[0] = 0;
						stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].stRMWorkZone[j].usRMTStart[1] = 0;
						stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].stRMWorkZone[j].usRMTEnd[0] = 23;
						stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].stRMWorkZone[j].usRMTEnd[1] = 59;
					}		
				}
				free_mem_block(pret_mem);//MEM_256_TYPE					
				continue;
			}
		}
		else
		{
			j = 0x55;
			file_write(F33_SUMGFREEZEPARA_ADDR + i * (F33_SUMGFREEZEPARA_LEN + 2), &j, 1);    //����Ч��ʶ��Ϊ��Ч
		}
        stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].usRMRunCtrlWord = 0x01;
        stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ucRMWorkZoneNum = 1;
        stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ucRMInval = 5;       //cfh 10-04-27
        stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ucRMTime[0] = 1;     //cfh 10-04-27
        stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ucRMTime[1] = 0;     //cfh 10-04-27
        stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ulRMDate = 1;        //cfh 10-04-27
        stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ucRMBcastTInval[0] = 0;  //cfh 10-04-27
        stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ucRMBcastTInval[1] = 0;  //cfh 10-04-27
        stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ucRMBcastTInval[2] = 0;  //cfh 10-04-27
        for(j=0;j<stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].ucRMWorkZoneNum;j++)
        {
            stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].stRMWorkZone[j].usRMTStart[0] = 0;
            stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].stRMWorkZone[j].usRMTStart[1] = 0;
            stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].stRMWorkZone[j].usRMTEnd[0] = 23;
            stTerRunPara.stPortList.stRMRunPara.stComPortPara[i].stRMWorkZone[j].usRMTEnd[1] = 59;
        }
    }
    return RET_SUCCESS;
}
//��ȡ��һ�������ϱ���ʱ��
STDATETIME RunPara_GetJobNextTime(STDATETIME stActiveTime,unsigned char ucTimeInter,unsigned char ucTimeUnit)
{
    STDATETIME stCurrTime,stNextExeTime;
    unsigned long ulNextExeTime,ulCurrTime;
    unsigned long ulNum=0;
    stCurrTime=GetCurrentTime();
    stNextExeTime=stActiveTime;
    ulCurrTime=TimeConver(&stCurrTime,CON_TIMEUNIT_SEC);        //��õ�ǰʱ��
    ulNextExeTime=TimeConver(&stNextExeTime,CON_TIMEUNIT_SEC);
    ulNum=TimeConverToSecond(ucTimeInter,ucTimeUnit);
    if(ulNum==0)
        ulNum=60;           //����1����
    if(ulCurrTime>ulNextExeTime)
    {
        if(ucTimeUnit!=CON_TIMEUNIT_MON)
        {
            ulNum=(ulCurrTime-ulNextExeTime)/ulNum+1;
            AddTime(&stNextExeTime,ulNum*ucTimeInter,ucTimeUnit);
        }
        else
        {
            stNextExeTime.ucYear=stCurrTime.ucYear;
            stNextExeTime.ucMonth=stCurrTime.ucMonth;
            ulNextExeTime=TimeConver(&stNextExeTime,CON_TIMEUNIT_SEC);
            if(ulCurrTime>ulNextExeTime)
                AddTime(&stNextExeTime,1,CON_TIMEUNIT_MON);
            if(Is_DateTime_Availble(&stNextExeTime)!=RET_SUCCESS)
                stNextExeTime.ucDay=28;         //ǿ��ת��
        }
    }
    return stNextExeTime;
}

// unsigned char    RunPara_InitSwitchCtrlPara(STSWITCHCTRLPARA *pstSwitchCtrlPara)
// {
//     unsigned char ucRet= RET_SUCCESS;
//  unsigned char ucRet= RET_SUCCESS,i=0,j=0,h=0;
//  pstSwitchCtrlPara->ucKeepPower = CON_AVALIBLE_FLAG;
//  pstSwitchCtrlPara->ucGprsKeepPow = CON_INVALIBLE_FLAG;
//  pstSwitchCtrlPara->ucIsCheckGprs = CON_AVALIBLE_FLAG;
//  pstSwitchCtrlPara->ucSwitchOut = CON_AVALIBLE_FLAG;
//
//  pstSwitchCtrlPara->stSafeFixVal.usDT2=0x8100;
//  SetLoadFixVal(&(pstSwitchCtrlPara->stSafeFixVal),100,100);
//
//  for(h=0;h<CON_SUMGROUP_NUM;h++)
//  {
//      for(i=0;i<CONSWITCHCURVENUM;i++)
//      {
//          pstSwitchCtrlPara->stCtrlSumGroupPara[h].stPowerCtrlProj[i].ucProjAvailbleFlag=CON_INVALIBLE_FLAG;
//          pstSwitchCtrlPara->stCtrlSumGroupPara[h].stPowerCtrlProj[i].ucCycleType = CON_CYCLETYPE_DAY;
//          pstSwitchCtrlPara->stCtrlSumGroupPara[h].stPowerCtrlProj[i].usCycleNum  = 0;
//          for(j=0;j<CONCYCLEDAYNUM;j++)
//              pstSwitchCtrlPara->stCtrlSumGroupPara[h].stPowerCtrlProj[i].ucAvailbleFlag[j] = CON_INVALIBLE_FLAG;
//          ClearDateTime(&(pstSwitchCtrlPara->stCtrlSumGroupPara[h].stPowerCtrlProj[i].stStartTime));
//          ClearDateTime(&(pstSwitchCtrlPara->stCtrlSumGroupPara[h].stPowerCtrlProj[i].stEndTime));
//      }
//
//      for(i=0;i<CONSWITCHCURVENUM;i++)
//      {
//          pstSwitchCtrlPara->stCtrlSumGroupPara[h].stPowerCtrl[i].ucAvailbleFlag = CON_INVALIBLE_FLAG;
//          for(j=0;j<CONSWITCHPERTIMENUM;j++)
//          {
//              pstSwitchCtrlPara->stCtrlSumGroupPara[h].stPowerCtrl[i].stPowerCtrlPer[j].ucAvailbleFlag = CON_INVALIBLE_FLAG;
//              pstSwitchCtrlPara->stCtrlSumGroupPara[h].stPowerCtrl[i].stPowerCtrlPer[j].ucCtrlFlag = CON_INVALIBLE_FLAG;
//              pstSwitchCtrlPara->stCtrlSumGroupPara[h].stPowerCtrl[i].stPowerCtrlPer[j].ucOnFlag = CON_INVALIBLE_FLAG;
//              pstSwitchCtrlPara->stCtrlSumGroupPara[h].stPowerCtrl[i].stPowerCtrlPer[j].ucBeginHour      = j*3;
//              pstSwitchCtrlPara->stCtrlSumGroupPara[h].stPowerCtrl[i].stPowerCtrlPer[j].ucBeginMinute  = 0;
//              pstSwitchCtrlPara->stCtrlSumGroupPara[h].stPowerCtrl[i].stPowerCtrlPer[j].stLoadFixVal.usDT2=0x8100;
//              SetLoadFixVal(&(pstSwitchCtrlPara->stCtrlSumGroupPara[h].stPowerCtrl[i].stPowerCtrlPer[j].stLoadFixVal),100,100);
//          }
//          memset(pstSwitchCtrlPara->stCtrlSumGroupPara[h].stPowerCtrl[i].ucTimerPerFlag,0x00,12);     //ʱ���־��ʼ��
//      }
//      for(i=0;i<CON_PARA_SWITCHOUT_NUM;i++)
//      {
//          pstSwitchCtrlPara->stCtrlSumGroupPara[h].stPowerCtrlTurn.ucAvailbleFlag[i] = CON_INVALIBLE_FLAG;
///*            pstSwitchCtrlPara->stCtrlSumGroupPara[h].stPowerHoldTime.usJumpTime[i]  = 600;      //��λ0.5�룬���Գ������9Сʱ
//          pstSwitchCtrlPara->stCtrlSumGroupPara[h].stPowerHoldTime.usJumpAlrTime[i]   = 360;
//          pstSwitchCtrlPara->stCtrlSumGroupPara[h].stPowerHoldTime.usRevTime[i]       = 600;
//          pstSwitchCtrlPara->stCtrlSumGroupPara[h].stPowerHoldTime.usRevAlrTime[i]    = 360;
//*/
//          pstSwitchCtrlPara->stCtrlSumGroupPara[h].stPowerHoldTime.usJumpTime[i]      = 120;      //��λ0.5�룬���Գ������9Сʱ
//          pstSwitchCtrlPara->stCtrlSumGroupPara[h].stPowerHoldTime.usJumpAlrTime[i]   = 120;
//          pstSwitchCtrlPara->stCtrlSumGroupPara[h].stPowerHoldTime.usRevTime[i]       = 120;
//          pstSwitchCtrlPara->stCtrlSumGroupPara[h].stPowerHoldTime.usRevAlrTime[i]    = 120;
//      }
//      pstSwitchCtrlPara->stCtrlSumGroupPara[h].stEnergyCtrl.ucEnergyCtrlType = CON_ENERGYCTRL_INVALID;
//      pstSwitchCtrlPara->stCtrlSumGroupPara[h].stEnergyCtrl.ucTurnFlag = 0;
//
//      SetLoadFixVal(&(pstSwitchCtrlPara->stCtrlSumGroupPara[h].stEnergyCtrl.stBuyEnergy),0,100);
//      pstSwitchCtrlPara->stCtrlSumGroupPara[h].stEnergyCtrl.stBuyEnergy.usDT2=0x00;
//      SetLoadFixVal(&(pstSwitchCtrlPara->stCtrlSumGroupPara[h].stEnergyCtrl.stMonthEnergy),0,100);
//      SetLoadFixVal(&(pstSwitchCtrlPara->stCtrlSumGroupPara[h].stEnergyCtrl.stAlrFixVal),0,100);
//      SetLoadFixVal(&(pstSwitchCtrlPara->stCtrlSumGroupPara[h].stEnergyCtrl.stJumpFixVal),0,100);
//      SetLoadFixVal(&(pstSwitchCtrlPara->stCtrlSumGroupPara[h].stMonEnergyRec.stMonFixVal),0,100);
//      SetLoadFixVal(&(pstSwitchCtrlPara->stCtrlSumGroupPara[h].stEnergyCtrl.stTurnFixVal),0,100);
//
//      SetLoadFixVal(&(pstSwitchCtrlPara->stCtrlSumGroupPara[h].stEnergyCtrl.stTurnAlrFixVal),0,100);
//      SetLoadFixVal(&(pstSwitchCtrlPara->stCtrlSumGroupPara[h].stEnergyCtrl.stTurnJumpFixVal),0,100);
//
//      pstSwitchCtrlPara->stCtrlSumGroupPara[h].stBuyEnergyRec.ulBuyBillNo = 0;
//      pstSwitchCtrlPara->stCtrlSumGroupPara[h].stBuyEnergyRec.ucFlashFlag = CON_ENERGYFLASH_NEW;
//      SetLoadFixVal(&(pstSwitchCtrlPara->stCtrlSumGroupPara[h].stBuyEnergyRec.stBuyFixVal),0,100);
//      pstSwitchCtrlPara->stCtrlSumGroupPara[h].stBuyEnergyRec.stBuyFixVal.usDT2=0x00;
//      SetLoadFixVal(&(pstSwitchCtrlPara->stCtrlSumGroupPara[h].stBuyEnergyRec.stAlrFixVal),0,100);
//      SetLoadFixVal(&(pstSwitchCtrlPara->stCtrlSumGroupPara[h].stBuyEnergyRec.stJumpFixVal),0,100);
//      SetLoadFixVal(&(pstSwitchCtrlPara->stCtrlSumGroupPara[h].stBuyEnergyRec.stTurnFixVal),0,100);
//      pstSwitchCtrlPara->stCtrlSumGroupPara[h].stBuyEnergyRec.dLeftVal_befor=0;
//      pstSwitchCtrlPara->stCtrlSumGroupPara[h].stBuyEnergyRec.dLeftVal_after=0;
//      for(i=0;i<CON_PARA_SWITCHOUT_NUM;i++)
//          pstSwitchCtrlPara->stCtrlSumGroupPara[h].stEnergyCtrlTurn.ucAvailbleFlag[i] = CON_INVALIBLE_FLAG;
//  }
//
//  for(i=0;i<CON_CTRLFEE_NUM;i++)
//      pstSwitchCtrlPara->stCtrlFeeRate.ulFeeRate[i] = 1;                      //��������
//  pstSwitchCtrlPara->stCtrlFeeRate.ucFeeNum = 4;
//  pstSwitchCtrlPara->stCtrlFeeRate.ucUnitFlag = 0;        //��λ��
//  pstSwitchCtrlPara->stCtrlFeeRateTime.ucFeeNum=4;
//  for(i=0;i<CON_CTRLFEETIME_NUM;i++)
//      pstSwitchCtrlPara->stCtrlFeeRateTime.ucFeeRateNo[i] = (i/6)%4;
//  //���ݷ���ʱ�β��������ڱ�
////    Ter_ADESetFeeTimePer(&(pstSwitchCtrlPara->stCtrlFeeRateTime));
//
//  for(i=0;i<CON_BUYBILL_NUM;i++)
//      SetBuyBillRec(&(pstSwitchCtrlPara->stBuyBillRec[i]), 0xFFFFFFFF, 0xFF);
//
//  return ucRet;
// }

//void SetLoadFixVal(STLOADFIXVAL *pstLoadFixVal,   double  dFixVal,unsigned short usFloatCoef)
//{
//  pstLoadFixVal->dFixVal     = dFixVal;
//  pstLoadFixVal->usFloatCoef = usFloatCoef;
//}
//
//void SetBuyBillRec(STBUYBILLREC *pstBuyBillRec,unsigned long ulBuyBillNo,unsigned char ucFlashFlag)
//{
//  if(pstBuyBillRec != NULL)
//  {
//      pstBuyBillRec->ulBuyBillNo = ulBuyBillNo;
//      pstBuyBillRec->ucFlashFlag = ucFlashFlag;
//  }
//}
//unsigned char RunPara_InitLoadCtrlWordPara(STLOADCTRLWORDPARA *pstLoadCtrlWordPara)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned char i=0;
//  for(i=0;i<CON_SUMGROUP_NUM;i++)
//  {
//      pstLoadCtrlWordPara->ucTimePerCtrl[i]=CON_INVALIBLE_FLAG;
//      pstLoadCtrlWordPara->ucRestCtrl[i]=CON_INVALIBLE_FLAG;
//      pstLoadCtrlWordPara->ucSaleStopCtrl[i]=CON_INVALIBLE_FLAG;
//      pstLoadCtrlWordPara->ucPowerLowerCtrl[i]=CON_INVALIBLE_FLAG;
//      pstLoadCtrlWordPara->ucMonthEnerCtrl[i]=CON_INVALIBLE_FLAG;
//      pstLoadCtrlWordPara->ucBuyEnerCtrl[i]=CON_INVALIBLE_FLAG;
//      pstLoadCtrlWordPara->ucUrgeFeeCtrl=CON_INVALIBLE_FLAG;
//  }
//  return ucRet;
//}

unsigned char   RunPara_InitTerNormalPara(STTERNORMALPARA *pstTerNormalPara)
{
    unsigned char ucRet= RET_SUCCESS;
//  pstTerNormalPara->ucCopyInter=15;
    pstTerNormalPara->ucCopyInter=5;        //change for test 09-11-24
    pstTerNormalPara->ucSamplePort=1;
    pstTerNormalPara->ucCopyFlag = CON_STATUS_YES;
    pstTerNormalPara->ucDataTypeInfo=0x49;
    memset(pstTerNormalPara->ucCommAddr,0xAA,6);
    memset(pstTerNormalPara->ucCommPass,0x55,6);
    return ucRet;
}

unsigned char   RunPara_InitTerMiscPara(STTERMISCPARA *pstTerMiscPara)
{
    unsigned char ucRet=RET_SUCCESS;

    pstTerMiscPara->ucAlarmSoundTime[0]=0;
    pstTerMiscPara->ucAlarmSoundTime[1]=0;
    pstTerMiscPara->ucAlarmSoundTime[2]=0;

    pstTerMiscPara->ucKeepAuto=72;              //72Сʱ

    pstTerMiscPara->ucMeterOverDiff=80;         //8.0
    pstTerMiscPara->ucMeterFly=99;              //9.9
    pstTerMiscPara->ucMeterStop=10;             //10*15min
    pstTerMiscPara->ucMeterTimeDiff=10;         //10min

//  for(i=0;i<CON_MAX_DIRPOWTOTAL_COMPNUM;i++)
//      pstTerMiscPara->stPowDiffLimit[i].ucSumGNo=0xFF;
    pstTerMiscPara->ulBoadrate[0]=9600;
    pstTerMiscPara->ulBoadrate[1]=1200;
    if(pstTerMiscPara->ulBoadrate[2]!=38400 && pstTerMiscPara->ulBoadrate[2]!=19200)    //������װʱ����38400��19200��������ʼ��Ϊ9600
        pstTerMiscPara->ulBoadrate[2]=9600;
    pstTerMiscPara->ulBoadrate[3]=1200;

    pstTerMiscPara->ucTalkOn=CON_INVALIBLE_FLAG;
    pstTerMiscPara->ucRemoveOn=CON_INVALIBLE_FLAG;
    pstTerMiscPara->ucUpActiveOn=CON_AVALIBLE_FLAG;     //������ʼ���󣬸����澯Ӧ�����ͣ����Բ�����
    pstTerMiscPara->ucPowerCtrlTimeRecMinute=CON_POWERCTRL_TIMEREC_END;

    pstTerMiscPara->ucIPInfoType[0]=CON_TC35COMMTYPE_TCP;
    pstTerMiscPara->ucIPInfoType[1]=CON_TC35COMMTYPE_UDP;
    pstTerMiscPara->ucRcommBaseType=CON_TC35COMMTYPE_TCP;
    if((pstTerMiscPara->ucIPInfoType[0]!=CON_TC35COMMTYPE_INVALID)
        && (pstTerMiscPara->ucIPInfoType[0]!=CON_TC35COMMTYPE_TCP)
        && (pstTerMiscPara->ucIPInfoType[0]!=CON_TC35COMMTYPE_UDP))
    {
        pstTerMiscPara->ucIPInfoType[0]=CON_TC35COMMTYPE_TCP;
        pstTerMiscPara->ucIPInfoType[1]=CON_TC35COMMTYPE_UDP;
        pstTerMiscPara->ucRcommBaseType=CON_TC35COMMTYPE_TCP;
    }
    if((pstTerMiscPara->ucIPInfoType[1]!=CON_TC35COMMTYPE_INVALID)
        && (pstTerMiscPara->ucIPInfoType[1]!=CON_TC35COMMTYPE_TCP)
        && (pstTerMiscPara->ucIPInfoType[1]!=CON_TC35COMMTYPE_UDP))
    {
        pstTerMiscPara->ucIPInfoType[0]=CON_TC35COMMTYPE_TCP;
        pstTerMiscPara->ucIPInfoType[1]=CON_TC35COMMTYPE_UDP;
        pstTerMiscPara->ucRcommBaseType=CON_TC35COMMTYPE_TCP;
    }
    pstTerMiscPara->ucProgramKeyIsSet=CON_STATUS_NO;

    pstTerMiscPara->usUsePin=CON_STATUS_NO_2BYTE;
    memset(pstTerMiscPara->ucPinNew,0,4);
    memset(pstTerMiscPara->ucPinOld,0,4);
    memset(pstTerMiscPara->ucPuk,0,8);

    pstTerMiscPara->ucPinNew[0]=0x01;     pstTerMiscPara->ucPinNew[1]=0x02;
    pstTerMiscPara->ucPinNew[2]=0x03;     pstTerMiscPara->ucPinNew[3]=0x04;

    pstTerMiscPara->ucPinOld[0]=0x01;     pstTerMiscPara->ucPinOld[1]=0x02;
    pstTerMiscPara->ucPinOld[2]=0x03;     pstTerMiscPara->ucPinOld[3]=0x04;

    pstTerMiscPara->ucPuk[0]=0x01;        pstTerMiscPara->ucPuk[1]=0x02;
    pstTerMiscPara->ucPuk[2]=0x03;        pstTerMiscPara->ucPuk[3]=0x04;
    pstTerMiscPara->ucPuk[4]=0x05;        pstTerMiscPara->ucPuk[5]=0x06;
    pstTerMiscPara->ucPuk[6]=0x07;        pstTerMiscPara->ucPuk[7]=0x08;

    pstTerMiscPara->stRCommPara.stGprsPDPPara.ucPDPUseFlag=0;
    memset(pstTerMiscPara->stRCommPara.stGprsPDPPara.ucPDPUserName,0,sizeof(pstTerMiscPara->stRCommPara.stGprsPDPPara.ucPDPUserName));
    memset(pstTerMiscPara->stRCommPara.stGprsPDPPara.ucPDPPassWord,0,sizeof(pstTerMiscPara->stRCommPara.stGprsPDPPara.ucPDPPassWord));
    //add by cfh 090811

    pstTerMiscPara->stRCommPara.stGprsRelinkTime.usRelinkTime=60;
    pstTerMiscPara->stRCommRunInfo.ucCurrChanType=CON_TERMSG_SOUR_RCOMM_SMS;
    pstTerMiscPara->stRCommRunInfo.ucPinStat=CON_PIN_UNLOCK;
    pstTerMiscPara->stRCommRunInfo.ucSignIntension=0;
    pstTerMiscPara->stRCommRunInfo.ucRCommStat=CON_STATUS_NO;
    pstTerMiscPara->stRCommRunInfo.ucRCommStatBak1=CON_STATUS_NO;
    pstTerMiscPara->stRCommRunInfo.ucRCommStatBak2=CON_STATUS_YES;
    pstTerMiscPara->stRCommRunInfo.ucGprsLastLoginStat=CON_STATUS_NO;
    pstTerMiscPara->stRCommRunInfo.ucGprsLoginStat=CON_STATUS_NO;
    memset(pstTerMiscPara->stRCommRunInfo.ucModHardVer,0x00,4);
    memset(pstTerMiscPara->stRCommRunInfo.ucModSoftVer,0x00,4);

    pstTerMiscPara->ucUrgeFeeTime[0]=0x00;          //Ĭ��0800-2059�澯ʱ��
    pstTerMiscPara->ucUrgeFeeTime[1]=0xFF;
    pstTerMiscPara->ucUrgeFeeTime[2]=0x1F;
    pstTerMiscPara->ulFluxLimit=0;
    return ucRet;
}

unsigned char   RunPara_InitTerAdjPara(STTERADJPARA *pstTerAdjPara)
{
    unsigned char ucRet=RET_SUCCESS;
//	pstTerAdjPara->ucPowerOnCtrlLockLeftTime=255;
//  pstTerAdjPara->ucSumGFromZero=0;                //1��1��ʼ��0���0��ʼ��������1
//	pstTerAdjPara->ucSumGFromZero=1;				//1��1��ʼ��0���0��ʼ��������1
    pstTerAdjPara->ucMPNoFromZero=1;
//	pstTerAdjPara->ucMPNoFromZero=1;				
//	pstTerAdjPara->ucEnerBuyUnit=0;					//0��λΪkwh��������λΪԪ		//��������
//	pstTerAdjPara->ucDownProgramSource=CON_TERDP_SOURCE_INVALID;
//  pstTerAdjPara->ucJob1HourDataExt=CON_STATUS_YES;
    pstTerAdjPara->ucJob1HourDataExt=CON_STATUS_NO;     //20080909����Ҫ��ȡ��
//	pstTerAdjPara->ucPulseDemandCyc=15;
//	pstTerAdjPara->ucEnerBuyIsNotFee=CON_STATUS_YES;	//Ĭ�Ϲ���������
    return ucRet;
}
//unsigned char RunPara_InitCTCheckPara(STCTCHECKPARA *pstCTCheckPara)
//{
//  unsigned char ucRet=RET_SUCCESS;
//
//  pstCTCheckPara->ucCTType=CON_CTTYPE_D;
//  pstCTCheckPara->ucTerWorkMode=CON_WORKMODE_TEST;
//  pstCTCheckPara->ucSlfStudyFlag=CON_INVALIBLE_FLAG;
///*
//  pstCTCheckPara->stCTLimitValue.stShortCircuit1.usValueA=0x01;   //��ֹ����
//  pstCTCheckPara->stCTLimitValue.stShortCircuit1.usValueB=0x01;   //0x01;
//  pstCTCheckPara->stCTLimitValue.stShortCircuit1.usValueC=0x01;
//
//  pstCTCheckPara->stCTLimitValue.stOpenCircuit2.usValueA=0x0280;  //0xA0*4=0x280
//  pstCTCheckPara->stCTLimitValue.stOpenCircuit2.usValueB=0x0280;
//  pstCTCheckPara->stCTLimitValue.stOpenCircuit2.usValueC=0x0280;
//*/
//
//  return ucRet;
//}

//unsigned char RunPara_First_InitTerPara()
//{
    /*delete by cfh 09-08-08
    unsigned char ucRet=RET_SUCCESS;
    unsigned char ucVerInfo[7];
    stTerRunPara.stTerAdjPara.ucLoginNeedAck=CON_STATUS_NO;
    stTerRunPara.stTerAdjPara.ucBeatHeartNeedAck=CON_STATUS_NO;
    stTerRunPara.stTerAdjPara.ucViewData_1=0;
    stTerRunPara.stTerAdjPara.ucRunMode=0;                                  //����ģʽ
    stTerRunPara.stTerMiscPara.ulBoadrate[2]=1200;
    stTerRunPara.stTerMiscPara.ucTerAddIsDecType=CON_STATUS_NO;
    stTerRunPara.stTerMiscPara.ucIPInfoType[0]=CON_TC35COMMTYPE_TCP;
    stTerRunPara.stTerMiscPara.ucIPInfoType[1]=CON_TC35COMMTYPE_UDP;
    stTerRunPara.stTerMiscPara.ucRcommBaseType=CON_TC35COMMTYPE_TCP;
    stTerRunPara.stTerMiscPara.ulFluxLimit=0;
    stTerRunPara.stTerMiscPara.ucShowRCommDebug=CON_STATUS_NO;

    RunPara_InitStaIPAddr(&stTerRunPara.stStaIPAddr);
    RunPara_InitStaPhoneNo(&stTerRunPara.stStaPhoneNo);
    RunPara_InitTerDevAddr(&stTerRunPara.stTerAddr);
    RunPara_InitTerRegionAddr(&stTerRunPara.stTerRegionAddr);

    stTerRunPara.stTerAuxPara.ucManufactor[0]=CON_TER_MANUFACTORNO_0;       //���̱�� "8000"
    stTerRunPara.stTerAuxPara.ucManufactor[1]=CON_TER_MANUFACTORNO_1;
    stTerRunPara.stTerAuxPara.ucManufactor[2]=CON_TER_MANUFACTORNO_2;
    stTerRunPara.stTerAuxPara.ucManufactor[3]=CON_TER_MANUFACTORNO_3;
    RunPara_GetTerVerInfo(ucVerInfo);
    stTerRunPara.stTerAuxPara.ucSoftwareVer[0]=ucVerInfo[0];
    stTerRunPara.stTerAuxPara.ucSoftwareVer[1]=ucVerInfo[1];
    stTerRunPara.stTerAuxPara.ucSoftwareVer[2]=ucVerInfo[2];
    stTerRunPara.stTerAuxPara.ucSoftwareVer[3]=ucVerInfo[3];
    stTerRunPara.stTerAuxPara.ucSoftwareDate[0]=ucVerInfo[4];
    stTerRunPara.stTerAuxPara.ucSoftwareDate[1]=ucVerInfo[5];
    stTerRunPara.stTerAuxPara.ucSoftwareDate[2]=ucVerInfo[6];
    memset(stTerRunPara.stTerAuxPara.ucConfigInfo,0x30,11);
    memset(stTerRunPara.stTerAuxPara.ucDevNo,0x30,8);

    stTerRunPara.stTerMiscPara.stRCommPara.stGprsPDPPara.ucPDPUseFlag=0;            //����Ҫ��
    memset(stTerRunPara.stTerMiscPara.stRCommPara.stGprsPDPPara.ucPDPUserName,0,sizeof(stTerRunPara.stTerMiscPara.stRCommPara.stGprsPDPPara.ucPDPUserName));
    memset(stTerRunPara.stTerMiscPara.stRCommPara.stGprsPDPPara.ucPDPPassWord,0,sizeof(stTerRunPara.stTerMiscPara.stRCommPara.stGprsPDPPara.ucPDPPassWord));

    RunPara_InitMeterProtocolInfo();            //����Լ�������Ĭ��

    LCD_Set_Contrast(20);
    LCD_Set_ContrastArray();

//  #define RM_FLAG_START_ADDR              0x17FF00            //������װ���̱�־
//  memset((unsigned char *)(0x17FF00),0x00,0xF0);
    return ucRet;
    */
//  return 0;
//}

/*********************************************************************************************************************/
STTERCOMMPARA RunPara_GetTerCommPara()
{
    return stTerRunPara.stTerCommPara;
}

STHEARTBEAT RunPara_GetHeartBeat()
{
    return stTerRunPara.stHeartBeat;
}

STSTAIPADDR RunPara_GetStaIPAddr()
{
    return stTerRunPara.stStaIPAddr;
}

STSTAPHONENO RunPara_GetStaPhoneNo()
{
    return stTerRunPara.stStaPhoneNo;
}
STLOCALIP   RunPara_GetLocalIP()
{
    return stTerRunPara.stLocalIP;
}

STTERENCRYPT RunPara_GetTerEncrypt()
{
    return stTerRunPara.stTerEncrypt;
}

STTERGROUPADDRLIST RunPara_GetTerGroupAddrList()
{
    return stTerRunPara.stTerGroupAddrList;
}

//STTERCOMMSTAT RunPara_GetTerCommStat()
//{
//  return stTerRunPara.stTerCommStat;
//}

//STGPRSPDPPARA*    RunPara_GetGprsPdpPara()
//{
//  return &(stTerRunPara.stTerMiscPara.stRCommPara.stGprsPDPPara);
//}

STALRCATEGORY*   RunPara_GetAlrShieldACDWord(unsigned char ucFlag)
{
    if(ucFlag == CON_ALR_SHIELDFLAG)
        return &stTerRunPara.stAlrCtrlWord.stAlrShield;
    else
        return &stTerRunPara.stAlrCtrlWord.stAlrACD;

}
/*
STALRCATEGORY   RunPara_GetAlrACDWord()
{
    return stTerRunPara.stAlrCtrlWord.stAlrACD;
}
*/
STTERALARMJUDGECOUNT    RunPara_GetAlrMaxCount(void)
{
    return stTerRunPara.stTerAlrMaxCount;
}

unsigned char   RunPara_GetMPType(unsigned char ucMPNo)
{
    if(ucMPNo < CON_MAX_MPNUM)
    {       
		if(ucMPNo == 0)
        {        
            return CON_MPTYPE_METER;//azh 121105
        }
        else
        {    
        return stTerRunPara.stMPInfo[ucMPNo].ucMPType;
        }
    }
    else if(ucMPNo == CON_MPNO_TER)//azh 130123
    {
        return CON_MPTYPE_TER;
    }
    else
    {
        return CON_MPTYPE_NULL;
    }
}

unsigned char   RunPara_GetMPDevNo(unsigned char ucMPNo)
{
    if(ucMPNo < CON_MAX_MPNUM)
        return stTerRunPara.stMPInfo[ucMPNo].ucDevNo[0];
    else
        return 0xFF;
}

unsigned char   RunPara_GetMPNo_byDevNo(unsigned char ucDevNo,unsigned char ucMPType)
{
    unsigned char i;
    unsigned char ucMPNo=CON_MPNO_INVALID;
    for(i=0;i<CON_MAX_MPNUM;i++)
    {
        if(stTerRunPara.stMPInfo[i].ucMPType==ucMPType && stTerRunPara.stMPInfo[i].ucDevNo[0]==ucDevNo)
        {
            ucMPNo=i;
            break;
        }
    }
    return ucMPNo;
}

STMPRATEVAL     RunPara_GetMPRateVal(unsigned char ucMPNo)
{
    STMPRATEVAL     stMPRateVal;
    if(ucMPNo < CON_MAX_MPNUM)
        return stTerRunPara.stMPInfo[ucMPNo].stMPRateVal;
    else
    {
        RunPara_InitMPRateVal(&stMPRateVal);
        return stMPRateVal;
    }
}

STMPALTERRATE   RunPara_GetMPAlterRate(unsigned char ucMPNo)
{
    STMPALTERRATE   stMPAlterRate;
    if(ucMPNo < CON_MAX_MPNUM)
        return stTerRunPara.stMPInfo[ucMPNo].stMPAlterRate;
    else
    {
        RunPara_InitMPAlterRate(&stMPAlterRate);
        return stMPAlterRate;
    }
}

STMPLIMIT       RunPara_GetMPLimit(unsigned char ucMPNo)
{
    STMPLIMIT   stMPLimit;
    if(ucMPNo < CON_MAX_MPNUM)
        return stTerRunPara.stMPInfo[ucMPNo].stMPLimit;
    else
    {
        RunPara_InitMPLimit(&stMPLimit);
        return stMPLimit;
    }
}

//STMPFREEZEPARA*   RunPara_GetMPFreezePara(unsigned char ucMPNo)
//{
//  if(ucMPNo < CON_MAX_MPNUM)
//      return &(stTerRunPara.stMPInfo[ucMPNo].stMPFreezePara[0]);
//  else
//      return NULL;
//}

//STFREEZEDAY     RunPara_GetMPFreezeDay(unsigned char ucMPNo)
//{
//    STFREEZEDAY     stFreezeDay;
//    if(ucMPNo < CON_MAX_MPNUM)
//        return stTerRunPara.stMPInfo[ucMPNo].stFreezeDay;
//    else
//    {
//        RunPara_InitMPFreezeDay(&stFreezeDay);
//        return stFreezeDay;
//    }
//}
PSTFREEZEDAY RunPara_GetMPFreezeDay(unsigned char ucMPNo)
{
    if(ucMPNo < CON_MAX_MPNUM)
        return &stTerRunPara.stMPInfo[ucMPNo].stFreezeDay;
    else
    {
//        RunPara_InitMPFreezeDay(&stFreezeDay);
//        return stFreezeDay;
        return &stTerRunPara.stMPInfo[0].stFreezeDay;//azh
    }
}

STPOWRATEPARA   RunPara_GetMPPowRatePara(unsigned char ucMPNo)
{
    STPOWRATEPARA   stPowRatePara;
    if(ucMPNo < CON_MAX_MPNUM)
        return stTerRunPara.stMPInfo[ucMPNo].stPowRatePara;
    else
    {
        RunPara_InitMPPowRatePara(&stPowRatePara);
        return stPowRatePara;
    }
}


STTONGSUNTIANSUNPARA RunPara_GetMPTongsun(unsigned char ucMPNo)
{
    STTONGSUNTIANSUNPARA    stPowRatePara;
    if(ucMPNo < CON_MAX_MPNUM)
        return stTerRunPara.stMPInfo[ucMPNo].stTongsunTiansun;
    else
    {
        RunPara_InitMPTongsun(&stPowRatePara);
        return stPowRatePara;
    }
}

STLINKPARA RunPara_GetLinkPara()
{
    return stTerRunPara.stLinkPara;
}
STLINKREMOVE RunPara_GetLinkRemove()
{
    return stTerRunPara.stLinkPara.stLinkRemove;
}
//STSLAVEADDR RunPara_GetSlaveAddr(unsigned char ucSlaveNo)
//{
//  STSLAVEADDR stSlaveAddr;
//  if(ucSlaveNo < CON_SLAVE_NUM)
//      return stTerRunPara.stLinkPara.stSlaveAddr[ucSlaveNo];
//  else
//      memset(&stSlaveAddr,0xFF,sizeof(STSLAVEADDR));
//  return  stSlaveAddr;
//}
STPORTLIST RunPara_GetPortPropList()
{
    return stTerRunPara.stPortList;
}
STPORTPROP RunPara_GetPortProp(unsigned char ucPort)
{
    STPORTPROP  stPortProp;
    if(ucPort < CON_COMMPORT_NUM)
        return stTerRunPara.stPortList.stPortProp[ucPort];
    else
        RunPara_InitPortProp(&stPortProp);

    return stPortProp;
}
STRMRUNPARA *RunPara_GetRMRunPara()
{
  return &(stTerRunPara.stPortList.stRMRunPara);
}
STCONFIGNUM     RunPara_GetConfigNum()
{
    return stTerRunPara.stConfigNum;
}

//STPULSEPARA RunPara_GetPulsePara(unsigned char ucPulseNo)
//{
//  STPULSEPARA stPulsePara;
//  if(ucPulseNo < CON_PULSE_NUM)
//      return stTerRunPara.stPulsePara[ucPulseNo];
//  else
//  {
//      RunPara_InitPulsePara(&stPulsePara);
//      return stPulsePara;
//  }
//}

STMETERPARA RunPara_GetMeterPara(unsigned char ucMeterNo)
{
    STMETERPARA stMeterPara;
    if(ucMeterNo < CON_METER_NUM)
        return stTerRunPara.stMeterPara[ucMeterNo];
    else
    {
        RunPara_InitMeterPara(&stMeterPara);
        return stMeterPara;
    }
}

//unsigned char RunPara_GetMeterPara_Proto(unsigned char ucMeterNo)
//{
//  if(ucMeterNo < CON_METER_NUM)
//      return stTerRunPara.stMeterPara[ucMeterNo].ucProtocol;
//  else
//      return CON_PROTOCOL_INVALID;
//}

STMETERPARA RunPara_GetMeterPara_byMPNo(unsigned char ucMPNo)
{
    STMETERPARA stMeterPara;
    unsigned char ucMeterNo=0;
    if(ucMPNo < CON_MAX_MPNUM)
    {
        ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
        if(ucMeterNo<CON_METER_NUM)
            return stTerRunPara.stMeterPara[ucMeterNo];
    }
    RunPara_InitMeterPara(&stMeterPara);
    stMeterPara.ucFeeNum=4;
    stMeterPara.ucSetFeeNum=4;
    return stMeterPara;
}

unsigned char   RunPara_GetMeterPara_PortNo_byMPNo(unsigned char ucMPNo)
{
    unsigned char ucPortNo=0;
    unsigned char ucMeterNo=0;
    ucMeterNo=RunPara_GetMeterNo_byMPNo(ucMPNo);
    if(ucMeterNo<CON_METER_NUM)
        ucPortNo=stTerRunPara.stMeterPara[ucMeterNo].ucPort;
    if(ucPortNo!=1)
        ucPortNo=0;
    return ucPortNo;
}
//unsigned char RunPara_GetMeterPara_EmphaFlag(unsigned char ucMeterNo)
//{
//  if(ucMeterNo < CON_METER_NUM)
//      return stTerRunPara.stMeterPara[ucMeterNo].ucEmpFlag;
//  else
//      return CON_STATUS_NO;
//}
//unsigned char   RunPara_GetMeterPara_PortNo(unsigned char ucMeterNo)
//{
//    unsigned char ucPortNo=0;
//    if(ucMeterNo<CON_METER_NUM)
//        ucPortNo=stTerRunPara.stMeterPara[ucMeterNo].ucPort;
//    if(ucPortNo!=1)
//        ucPortNo=0;
//    return ucPortNo;
//}

//unsigned long RunPara_GetMeterPara_Boadrate_byMPNo(unsigned char ucMPNo)
//{
//  unsigned char ucMeterNo=0;
//  if(ucMPNo < CON_MAX_MPNUM)
//  {
//      ucMeterNo = RunPara_GetMeterNo_byMPNo(ucMPNo);
//      if(ucMeterNo<CON_METER_NUM)
//          return stTerRunPara.stMeterPara[ucMeterNo].ulBaudRate;
//  }
//  return 1200;
//}

//STSUMGROUPPARA RunPara_GetSumGroupPara(unsigned char ucSumGNo)
//{
//  STSUMGROUPPARA stSumGroupPara;
//  if(ucSumGNo < CON_SUMGROUP_NUM)
//      return stTerRunPara.stSumGroupPara[ucSumGNo];
//  else
//  {
//      RunPara_InitSumGroupPara(&stSumGroupPara);
//      return stSumGroupPara;
//  }
//}
//STSUMGROUPPARA    *RunPara_GetSumGroupParaPtr(unsigned char ucSumGNo)
//{
//  if(ucSumGNo < CON_SUMGROUP_NUM)
//      return (&stTerRunPara.stSumGroupPara[ucSumGNo]);
//  else
//      return (&stTerRunPara.stSumGroupPara[0]);
//}
//STSWITCHPARA  RunPara_GetSwitchPara(unsigned char ucSwitchNo)
//{
//  STSWITCHPARA    stSwitchPara;
//  if(ucSwitchNo < CON_SWITCHIN_NUM)
//      return stTerRunPara.stSwitchPara[ucSwitchNo];
//  else
//  {
//      RunPara_InitSwitchPara(&stSwitchPara);
//      return stSwitchPara;
//  }
//}

//STANALOGPARA  RunPara_GetAnalogPara(unsigned char ucAnalogNo)
//{
//  STANALOGPARA    stAnalogPara;
//  if(ucAnalogNo < CON_ANALOG_NUM)
//      return stTerRunPara.stAnalogPara[ucAnalogNo];
//  else
//  {
//      RunPara_InitAnalogPara(&stAnalogPara);
//      return stAnalogPara;
//  }
//}

//unsigned char RunPara_GetAnalogPortNo(unsigned char ucMPNo,unsigned char ucAttr)
//{
//  unsigned char ucPortNo=0xFF;
//  unsigned char i;
//  for(i=0;i<CON_ANALOG_NUM;i++)
//  {
//      if(stTerRunPara.stAnalogPara[i].ucMPNo==ucMPNo && (stTerRunPara.stAnalogPara[i].ucAttr%6)==ucAttr)
//      {
//          ucPortNo=i;
//          break;
//      }
//  }
//  return ucPortNo;
//}

//STPOWERCTRLPROJ RunPara_GetPowCtrlProj(unsigned char ucCurveNo,unsigned char ucSumGroupNo)
//{
//  return stTerRunPara.stSwitchCtrlPara.stCtrlSumGroupPara[ucSumGroupNo].stPowerCtrlProj[ucCurveNo];
//}
//STLOADFIXVAL  RunPara_GetPowFixVal(unsigned char *pucPowCurveNo,unsigned char *pucEndHour,unsigned char *pucEndMinute,unsigned char ucSumGroupNo)
//{
//  unsigned char i=0,j=0;
//  STPOWERCTRLTIME stPowerCtrl;
//  STPOWERCTRLPROJ stPowerCtrlProj;
//  STDATETIME    stCurrTime,stBegTime;
//  unsigned long ulCurrTime,ulBegTime,ulMinVal=0xFFFF;
//  STLOADFIXVAL  stLoadFixVal;
//  SetLoadFixVal(&(stLoadFixVal),CONINVALIDVAL,100);
//  stCurrTime = GetCurrentTime();
//  ulCurrTime = TimeConver(&stCurrTime,CON_TIMEUNIT_MIN);
//  stBegTime  = GetCurrentTime();
//  for(i=0;i<CONSWITCHCURVENUM;i++)
//  {
//      stPowerCtrlProj = stTerRunPara.stSwitchCtrlPara.stCtrlSumGroupPara[ucSumGroupNo].stPowerCtrlProj[i];
//      stPowerCtrl = stTerRunPara.stSwitchCtrlPara.stCtrlSumGroupPara[ucSumGroupNo].stPowerCtrl[i];
//      if(stPowerCtrlProj.ucProjAvailbleFlag == CON_AVALIBLE_FLAG)
//      {
//          if(stPowerCtrl.ucAvailbleFlag == CON_AVALIBLE_FLAG)
//          {
//              for(j=0;j<CONSWITCHPERTIMENUM;j++)
//              {
//                  if(stPowerCtrl.stPowerCtrlPer[j].ucAvailbleFlag == CON_AVALIBLE_FLAG)
//                  {
//                      stBegTime.ucHour   = stPowerCtrl.stPowerCtrlPer[j].ucBeginHour;
//                      stBegTime.ucMinute = stPowerCtrl.stPowerCtrlPer[j].ucBeginMinute;
//                      if(Is_DateTime_Availble(&stBegTime) == RET_SUCCESS)
//                      {
//                          ulBegTime = TimeConver(&stBegTime,CON_TIMEUNIT_MIN);
//                          if(ulCurrTime >= ulBegTime && (ulCurrTime - ulBegTime)<ulMinVal)
//                          {
//                              ulMinVal = ulCurrTime - ulBegTime;
//                              SetLoadFixVal(&stLoadFixVal,CON_POWER_BIGVAL,100);          //�����������ö�ֵΪ��ֵ
//                              if(stPowerCtrl.stPowerCtrlPer[j].ucCtrlFlag==CON_AVALIBLE_FLAG)
//                              {
//                                  if(stPowerCtrl.stPowerCtrlPer[j].ucOnFlag==CON_AVALIBLE_FLAG || (i<CONSWITCHCURVE_TIMEPER1 || i>CONSWITCHCURVE_TIMEPER3))
//                                      stLoadFixVal = stPowerCtrl.stPowerCtrlPer[j].stLoadFixVal;
//                              }
//                              if(j==(CONSWITCHPERTIMENUM-1))
//                              {
//                                  (*pucEndHour)=stPowerCtrl.stPowerCtrlPer[0].ucBeginHour;
//                                  (*pucEndMinute)=stPowerCtrl.stPowerCtrlPer[0].ucBeginMinute;
//                              }else
//                              {
//                                  (*pucEndHour)=stPowerCtrl.stPowerCtrlPer[j+1].ucBeginHour;
//                                  (*pucEndMinute)=stPowerCtrl.stPowerCtrlPer[j+1].ucBeginMinute;
//                              }
//                          }
//                      }
//                  }
//              }
//          }
//          break;                          //�������ȼ���
//      }
//  }
//  (*pucPowCurveNo)=i;
//  return stLoadFixVal;
//}
//STPOWERCTRLHOLDTIME RunPara_GetPowCtrlHoldTime(unsigned char ucSumGroupNo)
//{
//  return stTerRunPara.stSwitchCtrlPara.stCtrlSumGroupPara[ucSumGroupNo].stPowerHoldTime;
//}
//STCTRLTURN    RunPara_GetPowerCtrlTurn(unsigned char ucSumGroupNo)
//{
//  return stTerRunPara.stSwitchCtrlPara.stCtrlSumGroupPara[ucSumGroupNo].stPowerCtrlTurn;
//}
//STENERGYCTRL  RunPara_GetEnergyCtrl(unsigned char ucSumGroupNo)
//{
//  return stTerRunPara.stSwitchCtrlPara.stCtrlSumGroupPara[ucSumGroupNo].stEnergyCtrl;
//}
//STMONENERGYREC    RunPara_GetMonEnergyRec(unsigned char ucSumGroupNo)
//{
//  return stTerRunPara.stSwitchCtrlPara.stCtrlSumGroupPara[ucSumGroupNo].stMonEnergyRec;
//}
//STBUYENERGYREC    RunPara_GetBuyEnergyRec(unsigned char ucSumGroupNo)
//{
//  return stTerRunPara.stSwitchCtrlPara.stCtrlSumGroupPara[ucSumGroupNo].stBuyEnergyRec;
//}
/*
unsigned char   RunPara_Manage_BuyBillNo(unsigned long ulBuyBillNo)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char i;
    if(ulBuyBillNo!=0xFFFFFFFF)
    {//���
        for(i=0;i<CON_BUYBILL_NUM;i++)
        {
            if(stTerRunPara.stSwitchCtrlPara.ulBuyBillNoList[i]==ulBuyBillNo)
            {
                ucRet=RET_ERROR;
                break;
            }
        }
    }
    if(ucRet==RET_SUCCESS)
    {//����
        for(i=1;i<CON_BUYBILL_NUM;i++)
        {
            stTerRunPara.stSwitchCtrlPara.ulBuyBillNoList[CON_BUYBILL_NUM-i]=stTerRunPara.stSwitchCtrlPara.ulBuyBillNoList[CON_BUYBILL_NUM-i-1];
        }
        stTerRunPara.stSwitchCtrlPara.ulBuyBillNoList[0]=ulBuyBillNo;
    }
    return ucRet;
}
*/
//unsigned char RunPara_Manage_BuyBillNo(unsigned long ulBuyBillNo,unsigned char ucFlashFlag)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  STBUYBILLREC stBuyBillRec;
//  unsigned char i;
//
//  stBuyBillRec = RunPara_GetBuyBillRec_byBillNo(0);
//  if(ulBuyBillNo!=0xFFFFFFFF)
//  {//���
//      if(stBuyBillRec.ulBuyBillNo == ulBuyBillNo)
//          ucRet = RET_ERROR;
//  }else
//      ucRet = RET_ERROR;
//
//  if(ucRet==RET_SUCCESS)
//  {//����
//      for(i=1;i<CON_BUYBILL_NUM;i++)
//      {
//          stBuyBillRec = RunPara_GetBuyBillRec_byBillNo((unsigned char)(CON_BUYBILL_NUM-i-1));
//          RunPara_SetBuyBillRec_byBillNo(&stBuyBillRec, (unsigned char)(CON_BUYBILL_NUM-i));
////            stTerRunPara.stSwitchCtrlPara.ulBuyBillNoList[CON_BUYBILL_NUM-i]=stTerRunPara.stSwitchCtrlPara.ulBuyBillNoList[CON_BUYBILL_NUM-i-1];
//      }
//      stBuyBillRec.ulBuyBillNo = ulBuyBillNo;
//      stBuyBillRec.ucFlashFlag = ucFlashFlag;
//      RunPara_SetBuyBillRec_byBillNo(&stBuyBillRec, 0);
////        stTerRunPara.stSwitchCtrlPara.ulBuyBillNoList[0]=ulBuyBillNo;
//  }
//  return ucRet;
//}

//STCTRLTURN        RunPara_GetEnergyCtrlTurn(unsigned char ucSumGroupNo)
//{
//  return stTerRunPara.stSwitchCtrlPara.stCtrlSumGroupPara[ucSumGroupNo].stEnergyCtrlTurn;
//}
//unsigned char RunPara_GetInfo_IsEnerTotalOnly(unsigned char ucSumGroupNo)
//{
//  unsigned char ucRet=CON_STATUS_NO;
//  return ucRet;
//}
//STCTRLFEERATE RunPara_GetCtrlFeeRate()
//{
//  return stTerRunPara.stSwitchCtrlPara.stCtrlFeeRate;
//}
//STCTRLFEERATETIME RunPara_GetCtrlFeeRateTime()
//{
//  return stTerRunPara.stSwitchCtrlPara.stCtrlFeeRateTime;
//}
//STLOADFIXVAL  RunPara_GetCtrlSafeFixVal()
//{
//  return stTerRunPara.stSwitchCtrlPara.stSafeFixVal;
//}
//unsigned char RunPara_GetSwitchOutFlag()
//{
//  return CON_AVALIBLE_FLAG;
//}
//
//STLOADCTRLWORDPARA    RunPara_GetLoadCtrlWordPara()
//{
//  return stTerRunPara.stLoadCtrlWordPara;
//}
//
//STPOWERCTRLTIME RunPara_GetPowCtrlTimePer(unsigned char ucCurveNo,unsigned char ucSumGroupNo)
//{
//  return stTerRunPara.stSwitchCtrlPara.stCtrlSumGroupPara[ucSumGroupNo].stPowerCtrl[ucCurveNo];
//}
//
//unsigned char RunPara_GetKeepPower()
//{
//  return stTerRunPara.stSwitchCtrlPara.ucKeepPower;
//}
//unsigned char RunPara_GetIsCheckGprs()
//{
//  return stTerRunPara.stSwitchCtrlPara.ucIsCheckGprs;
//}
//unsigned char RunPara_GetGprsKeepPow()
//{
//  if(stTerRunPara.stSwitchCtrlPara.ucIsCheckGprs==CON_INVALIBLE_FLAG)//���GPRS������Ч
//      return CON_INVALIBLE_FLAG;
//  else
//      return stTerRunPara.stSwitchCtrlPara.ucGprsKeepPow;
//}
//
//unsigned char RunPara_GetLoadCtrl_UrgeFee()
//{
//  return stTerRunPara.stLoadCtrlWordPara.ucUrgeFeeCtrl;
//}
//�������̱��
STTERAUXPARA RunPara_GetTerAuxPara()
{
    STTERAUXPARA stTerAuxPara1;
    memcpy(&stTerAuxPara1,&stTerRunPara.stTerAuxPara,sizeof(STTERAUXPARA));
    stTerAuxPara1.ucManufactor[0]=CON_TER_MANUFACTORNO_0;
    stTerAuxPara1.ucManufactor[1]=CON_TER_MANUFACTORNO_1;
    stTerAuxPara1.ucManufactor[2]=CON_TER_MANUFACTORNO_2;
    stTerAuxPara1.ucManufactor[3]=CON_TER_MANUFACTORNO_3;
    return stTerAuxPara1;
}
//�����ն˵�ַ
STTERDEVADDR    RunPara_GetTerDevAddr()
{
    return stTerRunPara.stTerAddr;
}
//������������
STTERREGIONADDR RunPara_GetTerRegionAddr()
{
    return stTerRunPara.stTerRegionAddr;
}

STJOBPARA*       RunPara_GetJobPara(unsigned char ucJobNo,unsigned char ucJobClassNo)
{
    STJOBPARA *pstJobPara=NULL;
    if(ucJobNo < CONDATAJOBNUM)
    {
        switch(ucJobClassNo)
            {
            case 0:
                return &stTerRunPara.stJobPara_Data1[ucJobNo];//azh ̫ռram�ռ� ��ֹ��ջ���
            case 1:
//                return stTerRunPara.stJobPara_Data2[ucJobNo];
                return &stTerRunPara.stJobPara_Data2[ucJobNo];
            }

    }
//    RunPara_InitJobPara(&stJobPara);
    return pstJobPara;
}
STUPUSR RunPara_GetUpUsrJobPara(unsigned char ucJobNo,unsigned char ucUpUsr)
{
    STUPUSR stUpusr;
    if(ucUpUsr < CON_UPUSR_NUM)
    {
        switch(ucJobNo)
        {
            case 0:
                return stTerRunPara.stJobFnPara1.stUpUsr[ucUpUsr];
            case 1:
                return stTerRunPara.stJobFnPara2.stUpUsr[ucUpUsr];
        }
    }
    memset(&stUpusr,0,sizeof(STUPUSR));
    return stUpusr;
}
STTERNORMALPARA RunPara_GetTerNormalPara()
{
    return stTerRunPara.stTerNormalPara;
}

STTERMISCPARA   RunPara_GetTerMiscPara()
{
    return stTerRunPara.stTerMiscPara;
}
////unsigned char   RunPara_GetTerMisc_GprsLoginStat()
////{
////    return stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat;
////}
//unsigned long RunPara_GetUartBoad(unsigned char ucUartID)
//{
//  unsigned long ulBoadrate=1200;
//  if(ucUartID<MAX_PORT_NUM)
//      ulBoadrate=stTerRunPara.stTerMiscPara.ulBoadrate[ucUartID];
//  return ulBoadrate;
//}

STTERADJPARA    RunPara_GetTerAdjPara()
{
    return stTerRunPara.stTerAdjPara;
}
//STCTCHECKPARA RunPara_GetCTCheckPara()
//{
//  return stTerRunPara.stCTCheckPara;
//}
//unsigned char   RunPara_GetTerAdjPara_DownloadSource()
//{
//  return stTerRunPara.stTerAdjPara.ucDownProgramSource;
//}

//unsigned char   RunPara_GetTerAdjPara_EnerBuyIsNotFee()
//{
//  return stTerRunPara.stTerAdjPara.ucEnerBuyIsNotFee;
//}
unsigned char   RunPara_GetTerAdjPara_Job1HourDataExt()
{
    return stTerRunPara.stTerAdjPara.ucJob1HourDataExt;
}

unsigned char   RunPara_GetMeterCopyInter()
{
    if (stTerRunPara.stTerNormalPara.ucCopyInter<=3)
        return 3;
    else if (stTerRunPara.stTerNormalPara.ucCopyInter>=15)
        return 15;
    else
        return stTerRunPara.stTerNormalPara.ucCopyInter;
}
unsigned char RunPara_GetMeterCopyCopyFlag()
{
    return stTerRunPara.stTerNormalPara.ucCopyFlag;
}
unsigned char   RunPara_GetTerCommPara_RCommMode()
{
    return stTerRunPara.stTerCommPara_Inner.ucRCommMode;
}


//unsigned char   RunPara_GetTerMiscParaStatus_ProgramKeyIsSet()
//{
//    return stTerRunPara.stTerMiscPara.ucProgramKeyIsSet;
//}

//unsigned char   RunPara_GetTerAdj_ViewData_1_Flag()
//{
//  unsigned char ucRet=0;
//  STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
//  if(stTerAdjPara.ucViewData_1==1)
//      ucRet=1;
//  return ucRet;
//}
unsigned char   RunPara_GetTerAdj_MPNoFromZero()
{
	return stTerRunPara.stTerAdjPara.ucMPNoFromZero;
}

//unsigned short   RunPara_GetMP_VolAlterRate(unsigned char ucMPNo)
//{
//  unsigned short usRate=1;
//  STMPALTERRATE stMPAlterRate;
//  STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
//  if(stTerAdjPara.ucViewData_1==1)
//  {
//      stMPAlterRate=RunPara_GetMPAlterRate(ucMPNo);
//      usRate=stMPAlterRate.usVolAlterRate;
//  }
//  return usRate;
//}

////unsigned short   RunPara_GetMP_CurAlterRate(unsigned char ucMPNo)
////{
////    unsigned short usRate=1;
////    STMPALTERRATE stMPAlterRate;
////    STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
////    if(stTerAdjPara.ucViewData_1==1)
////    {
////        stMPAlterRate=RunPara_GetMPAlterRate(ucMPNo);
////        usRate=stMPAlterRate.usCurAlterRate;
////    }
////    return usRate;
////}

//unsigned long   RunPara_GetMP_PowerAlterRate(unsigned char ucMPNo)
//{
//  unsigned long ulRate=1;
//  STMPALTERRATE stMPAlterRate;
//  STTERADJPARA stTerAdjPara=RunPara_GetTerAdjPara();
//  if(stTerAdjPara.ucViewData_1==1)
//  {
//      stMPAlterRate=RunPara_GetMPAlterRate(ucMPNo);
//      ulRate=stMPAlterRate.usVolAlterRate*stMPAlterRate.usCurAlterRate;
//  }
//  return ulRate;
//}

//unsigned char   RunPara_GetTerMisc_MeterTimeDiff()
//{
//    return stTerRunPara.stTerMiscPara.ucMeterTimeDiff;
//}

//unsigned char RunPara_GetTerMisc_RemoveOn()
//{
//  return stTerRunPara.stTerMiscPara.ucRemoveOn;
//}

//unsigned char   RunPara_GetSwitchPara_PulseIsTypeSwitch(unsigned char ucPulseNo)
//{
//  unsigned char ucRet=CON_STATUS_NO;
///*
//  unsigned char ucSwitchNo;
//  if(ucPulseNo < 4)
//  {
//      ucSwitchNo=ucPulseNo+4;                     //���帴�õ�4[0-7]·���ؿ�ʼ��2·
//      if(stTerRunPara.stSwitchPara[ucSwitchNo].ucAttr!=CON_SWITCHATTR_INVALID)
//          ucRet=CON_STATUS_YES;
//  }
//*/
//  return ucRet;
//}

//unsigned char RunPara_GetTerMisc_RCommRunStat()
//{
//  return stTerRunPara.stTerMiscPara.stRCommRunInfo.ucRCommStat;
//}

//unsigned char RunPara_GetTerMisc_TerAddIsDecType()
//{
//  return stTerRunPara.stTerMiscPara.ucTerAddIsDecType;
//}

unsigned char   RunPara_GetTerMisc_DefaultIPType()
{
    return stTerRunPara.stTerMiscPara.ucIPInfoType[0];
}

unsigned char   RunPara_GetTerMisc_ucRcommBaseType()
{
    return stTerRunPara.stTerMiscPara.ucRcommBaseType;
}
unsigned long   RunPara_GetTerMisc_FluxLimit()
{
    return stTerRunPara.stTerMiscPara.ulFluxLimit;
}
//azh 170919
unsigned char*   RunPara_GetTerMisc_Wireless()
{
    return &stTerRunPara.ucWireless[0];
}
unsigned char*   RunPara_GetTerMisc_TerLocation()
{
    return &stTerRunPara.ucLocation[0];
}
unsigned char*   RunPara_GetTerMisc_Snstate()
{
    return &stTerRunPara.ucSnstate[0];
}

//unsigned char RunPara_GetTerMisc_ShowRCommDebug()
//{
//  return stTerRunPara.stTerMiscPara.ucShowRCommDebug;
//}

//unsigned char RunPara_GetTerMisc_RcommType(unsigned char *pucIPInfoType0,unsigned char *pucIPInfoType1,unsigned char *pucRcommBaseType)
//{
//  (*pucIPInfoType0)=stTerRunPara.stTerMiscPara.ucIPInfoType[0];
//  (*pucIPInfoType1)=stTerRunPara.stTerMiscPara.ucIPInfoType[1];
//  (*pucRcommBaseType)=stTerRunPara.stTerMiscPara.ucRcommBaseType;
//  return RET_SUCCESS;
//}

//unsigned char RunPara_GetTerMisc_PowDiffLimit_ucSumGNo(unsigned char ucSeqNo)
//{
//  unsigned char ucSumGNo=0;
//  if(ucSeqNo<CON_MAX_DIRPOWTOTAL_COMPNUM)
//      ucSumGNo=stTerRunPara.stTerMiscPara.stPowDiffLimit[ucSeqNo].ucSumGNo;
//  return ucSumGNo;
//}

/*********************************************************************************************************************/
unsigned char RunPara_SetTerCommPara(STTERCOMMPARA *pstTerCommPara)
{
    unsigned char ucRet=RET_SUCCESS;
    stTerRunPara.stTerCommPara = (*pstTerCommPara);
    return ucRet;
}

unsigned char RunPara_SetHeartBeat(STHEARTBEAT *pstHeartBeat)
{
    unsigned char ucRet=RET_SUCCESS;
    stTerRunPara.stHeartBeat = (*pstHeartBeat);
    return ucRet;
}

unsigned char RunPara_SetStaIPAddr(STSTAIPADDR *pstStaIPAddr)
{
    unsigned char ucRet=RET_SUCCESS;
    stTerRunPara.stStaIPAddr = (*pstStaIPAddr);
    return ucRet;
}

unsigned char RunPara_SetStaPhoneNo(STSTAPHONENO *pstStaPhoneNo)
{
    unsigned char ucRet=RET_SUCCESS;
    stTerRunPara.stStaPhoneNo = (*pstStaPhoneNo);
    return ucRet;
}

unsigned char RunPara_SetTerEncrypt(STTERENCRYPT *pstTerEncrypt)
{
    unsigned char ucRet=RET_SUCCESS;
    stTerRunPara.stTerEncrypt = (*pstTerEncrypt);
    return ucRet;
}
unsigned char RunPara_SetTerLocalIP(STLOCALIP   *pstLocalIP)
{
    unsigned char ucRet = RET_SUCCESS;
    stTerRunPara.stLocalIP = *pstLocalIP;
    return ucRet;
}
unsigned char RunPara_SetTerGroupAddrList(STTERGROUPADDRLIST *pstTerGroupAddr)
{
    unsigned char ucRet=RET_SUCCESS;
    stTerRunPara.stTerGroupAddrList = (*pstTerGroupAddr);
    return ucRet;
}

//unsigned char RunPara_SetTerCommStat(STTERCOMMSTAT *pstTerCommStat)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  stTerRunPara.stTerCommStat = (*pstTerCommStat);
//  return ucRet;
//}

//unsigned char RunPara_SetGprsPdpPara(STGPRSPDPPARA *pstGprsPdpPara)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  if((pstGprsPdpPara->ucPDPUserName[0]<=0x20) || (pstGprsPdpPara->ucPDPUserName[0]>=0x7F))
//      pstGprsPdpPara->ucPDPUseFlag=0x00;
//  if(pstGprsPdpPara->ucPDPUseFlag!=0x01)
//      pstGprsPdpPara->ucPDPUseFlag=0x00;              //����Ϊ0����1
//  stTerRunPara.stTerMiscPara.stRCommPara.stGprsPDPPara = (*pstGprsPdpPara);
//  return ucRet;
//}

//unsigned char RunPara_SetRcommType(unsigned char ucIPInfoType0,unsigned char ucIPInfoType1,unsigned char ucRcommBaseType)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  stTerRunPara.stTerMiscPara.ucIPInfoType[0]=ucIPInfoType0;
//  stTerRunPara.stTerMiscPara.ucIPInfoType[1]=ucIPInfoType1;
//  stTerRunPara.stTerMiscPara.ucRcommBaseType=ucRcommBaseType;
//  return ucRet;
//}

unsigned char RunPara_SetAlrCtrlWord(STALRCTRLWORD *pstAlrCtrlWord)
{
    unsigned char ucRet=RET_SUCCESS;
    stTerRunPara.stAlrCtrlWord = (*pstAlrCtrlWord);
    return ucRet;
}

unsigned char RunPara_SetAlrMaxCount(STTERALARMJUDGECOUNT *pstAlrJudgeCount)
{
    unsigned char ucRet=RET_SUCCESS;
    stTerRunPara.stTerAlrMaxCount= (*pstAlrJudgeCount);
    return ucRet;
}

unsigned char RunPara_SetMPType(unsigned char ucMPNo,unsigned char ucMPType)
{
    unsigned char ucRet=RET_SUCCESS;

    if(ucMPNo < CON_MAX_MPNUM)
        stTerRunPara.stMPInfo[ucMPNo].ucMPType = ucMPType;
    else
        ucRet = RET_ERROR;

    return ucRet;
}
unsigned char RunPara_SetMPDevNo(unsigned char ucMPNo,unsigned char ucDevNo)
{
    unsigned char ucRet=RET_SUCCESS;

    if(ucMPNo < CON_MAX_MPNUM)
        stTerRunPara.stMPInfo[ucMPNo].ucDevNo[0] = ucDevNo;
    else
        ucRet = RET_ERROR;

    return ucRet;
}

//unsigned char RunPara_SetPulseMPDevNo(unsigned char ucMPNo,unsigned char ucDevNo,unsigned char ucPulseType)
//{
//  unsigned char ucRet=RET_SUCCESS,ucPos;
//  if((ucPulseType==CON_PULSEATTR_DIRPOW)
//      || (ucPulseType==CON_PULSEATTR_REVPOW)
//      || (ucPulseType==CON_PULSEATTR_DIRVAR)
//      || (ucPulseType==CON_PULSEATTR_REVVAR) )
//      ucPos=((unsigned char)(ucPulseType-1) & 0x3);
//  else
//      ucPos=0;
//  if(ucMPNo < CON_MAX_MPNUM)
//      stTerRunPara.stMPInfo[ucMPNo].ucDevNo[ucPos] = ucDevNo;
//  else
//      ucRet = RET_ERROR;
//
//  return ucRet;
//}

//unsigned char RunPara_CheckPulseMPDevNo_IsNone(unsigned char ucMPNo)
//{
//  unsigned char ucRet=CON_STATUS_YES,i;
//
//  if(ucMPNo < CON_MAX_MPNUM)
//  {
//      for(i=0;i<4;i++)
//      {
//          if(stTerRunPara.stMPInfo[ucMPNo].ucDevNo[i] != 0xFF)
//          {
//              ucRet=CON_STATUS_NO;
//              break;
//          }
//      }
//  }
//  return ucRet;
//}

unsigned char RunPara_SetMPRateVal(unsigned char ucMPNo,STMPRATEVAL *pstMPRateVal)
{
    unsigned char ucRet=RET_SUCCESS;

    if(ucMPNo < CON_MAX_MPNUM)
        stTerRunPara.stMPInfo[ucMPNo].stMPRateVal = (*pstMPRateVal);
    else
        ucRet = RET_ERROR;

    return ucRet;
}
unsigned char RunPara_SetMPAlterRate(unsigned char ucMPNo,STMPALTERRATE *pstMPAlterRate)
{
    unsigned char ucRet=RET_SUCCESS;

    if(ucMPNo < CON_MAX_MPNUM)
        stTerRunPara.stMPInfo[ucMPNo].stMPAlterRate = (*pstMPAlterRate);
    else
        ucRet = RET_ERROR;

    return ucRet;
}

unsigned char RunPara_SetMPLimit(unsigned char ucMPNo,STMPLIMIT *pstMPLimit)
{
    unsigned char ucRet=RET_SUCCESS;

    if(ucMPNo < CON_MAX_MPNUM)
        stTerRunPara.stMPInfo[ucMPNo].stMPLimit = (*pstMPLimit);
    else
        ucRet = RET_ERROR;

    return ucRet;
}

//unsigned char RunPara_SetMPFreezePara(unsigned char ucMPNo,STMPFREEZEPARA *pstMPFreezePara)
//{
//  unsigned char ucRet=RET_SUCCESS,i=0;
//
//  if(ucMPNo < CON_MAX_MPNUM)
//  {
//      for(i=0;i<CON_MAX_MTRFREEZENUM;i++)
//          stTerRunPara.stMPInfo[ucMPNo].stMPFreezePara[i] = pstMPFreezePara[i];
//  }else
//      ucRet = RET_ERROR;
//
//  return ucRet;
//}

unsigned char RunPara_SetMPFreezeDay(unsigned char ucMPNo,STFREEZEDAY   *pstFreezeDay)
{
    unsigned char ucRet=RET_SUCCESS;

    if(ucMPNo < CON_MAX_MPNUM)
        stTerRunPara.stMPInfo[ucMPNo].stFreezeDay = (*pstFreezeDay);
    else
        ucRet = RET_ERROR;

    return ucRet;
}

unsigned char RunPara_SetMPPowRatePara(unsigned char ucMPNo,STPOWRATEPARA   *pstPowRatePara)
{
    unsigned char ucRet=RET_SUCCESS;
    if(ucMPNo < CON_MAX_MPNUM)
    {
        stTerRunPara.stMPInfo[ucMPNo].stPowRatePara = (*pstPowRatePara);
    }else
        ucRet = RET_ERROR;
    return ucRet;
}


unsigned char RunPara_SetMPTongsun(unsigned char ucMPNo,STTONGSUNTIANSUNPARA *pstTongsun)
{
    unsigned char ucRet=RET_SUCCESS;

    if(ucMPNo < CON_MAX_MPNUM)
        stTerRunPara.stMPInfo[ucMPNo].stTongsunTiansun= (*pstTongsun);
    else
        ucRet = RET_ERROR;

    return ucRet;
}

//unsigned char RunPara_SetConfigNum(STCONFIGNUM *pstConfigNum)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  stTerRunPara.stConfigNum = (*pstConfigNum);
//  return ucRet;
//}

//unsigned char RunPara_SetPulsePara(unsigned char ucPulseNo,STPULSEPARA *pstPulsePara)
//{
//  unsigned char ucRet=RET_SUCCESS;
//
//  if(ucPulseNo < CON_PULSE_NUM)
//      stTerRunPara.stPulsePara[ucPulseNo] = (*pstPulsePara);
//  else
//      ucRet=RET_ERROR;
//  return ucRet;
//}

unsigned char RunPara_SetMeterPara(unsigned char ucMeterNo,STMETERPARA *pstMeterPara)
{
    unsigned char ucRet=RET_SUCCESS;

    if(ucMeterNo < CON_METER_NUM)
        stTerRunPara.stMeterPara[ucMeterNo] = (*pstMeterPara);
    else
        ucRet = RET_ERROR;

    return ucRet;
}
//unsigned char RunPara_SetSumGroupPara(unsigned char ucSumGNo,STSUMGROUPPARA *pstSumGPara)
//{
//  unsigned char ucRet=RET_SUCCESS;
//
//  if(ucSumGNo < CON_SUMGROUP_NUM)
//      stTerRunPara.stSumGroupPara[ucSumGNo] = (*pstSumGPara);
//  else
//      ucRet = RET_ERROR;
//
//  return ucRet;
//}

//unsigned char RunPara_SetSumGroupAvailableFalg(unsigned char ucSumGNo,unsigned char ucAvailableFlag)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  if(ucSumGNo < CON_SUMGROUP_NUM)
//      stTerRunPara.stSumGroupPara[ucSumGNo].ucAvailbleFlag = ucAvailableFlag;
//  else
//      ucRet = RET_ERROR;
//  return ucRet;
//}

//unsigned char RunPara_SetSumGroupParaOper(unsigned char ucSumGNo,STSUMGROUPPARA *pstSumGPara)
//{
//  unsigned char ucRet=RET_SUCCESS;
//
//  if(ucSumGNo < CON_SUMGROUP_NUM)
//  {
//      stTerRunPara.stSumGroupPara[ucSumGNo].stInstantPow.stDir = pstSumGPara->stInstantPow.stDir;
//      stTerRunPara.stSumGroupPara[ucSumGNo].stInstantPow.stRev = pstSumGPara->stInstantPow.stRev;
//      stTerRunPara.stSumGroupPara[ucSumGNo].stInstantVar.stDir = pstSumGPara->stInstantVar.stDir;
//      stTerRunPara.stSumGroupPara[ucSumGNo].stInstantVar.stRev = pstSumGPara->stInstantVar.stRev;
//      stTerRunPara.stSumGroupPara[ucSumGNo].stPowVal.stDir = pstSumGPara->stPowVal.stDir;
//      stTerRunPara.stSumGroupPara[ucSumGNo].stPowVal.stRev = pstSumGPara->stPowVal.stRev;
//      stTerRunPara.stSumGroupPara[ucSumGNo].stVarVal.stDir = pstSumGPara->stVarVal.stDir;
//      stTerRunPara.stSumGroupPara[ucSumGNo].stVarVal.stRev = pstSumGPara->stVarVal.stRev;
//  }
//  else
//      ucRet = RET_ERROR;
//
//  return ucRet;
//}
//F8���ն�����ͨ�Ź�����ʽ����̫ר��������ר����
unsigned char RunPara_Init_TerCommPara_Inner(STTERCOMMPARA_INNER *pstTerCommpara_Inner)
{
    pstTerCommpara_Inner->ucRCommMode=CON_ONLINE_ALLWAYS;   //��������
    pstTerCommpara_Inner->usReConnectInter=90;          //90��
    pstTerCommpara_Inner->ucReConnectTimes=3;
    pstTerCommpara_Inner->ucStandbyTime=10;             //����10����
    memset(pstTerCommpara_Inner->ucTimeInt,0xff,3);
    return RET_SUCCESS;
}

unsigned char RunPara_SetTerCommPara_Inner(STTERCOMMPARA_INNER *pstTerCommPara_Inner)
{
    unsigned char ucRet=RET_SUCCESS;
    stTerRunPara.stTerCommPara_Inner = (*pstTerCommPara_Inner);
    return ucRet;
}

STTERCOMMPARA_INNER RunPara_GetTerCommPara_Inner()
{
    return stTerRunPara.stTerCommPara_Inner;
}

//unsigned char RunPara_SetSumGroupParaLimit(unsigned char ucSumGNo,STSUMGROUPPARA *pstSumGPara)
//{
//  unsigned char ucRet=RET_SUCCESS;
//
//  if(ucSumGNo < CON_SUMGROUP_NUM)
//  {
//      stTerRunPara.stSumGroupPara[ucSumGNo].stInstantPow.stLimit = pstSumGPara->stInstantPow.stLimit;
//      stTerRunPara.stSumGroupPara[ucSumGNo].stInstantVar.stLimit = pstSumGPara->stInstantVar.stLimit;
//      stTerRunPara.stSumGroupPara[ucSumGNo].stPowVal.stLimit = pstSumGPara->stPowVal.stLimit;
//      stTerRunPara.stSumGroupPara[ucSumGNo].stVarVal.stLimit = pstSumGPara->stVarVal.stLimit;
//  }
//  else
//      ucRet = RET_ERROR;
//
//  return ucRet;
//}
//
//unsigned char RunPara_SetSumGroupParaFreeze(unsigned char ucSumGNo,STSUMGROUPPARA *pstSumGPara)
//{
//  unsigned char ucRet=RET_SUCCESS;
//
//  if(ucSumGNo < CON_SUMGROUP_NUM)
//  {
//      stTerRunPara.stSumGroupPara[ucSumGNo].stInstantPow.stFreeze = pstSumGPara->stInstantPow.stFreeze;
//      stTerRunPara.stSumGroupPara[ucSumGNo].stInstantVar.stFreeze = pstSumGPara->stInstantVar.stFreeze;
//      stTerRunPara.stSumGroupPara[ucSumGNo].stPowVal.stFreeze = pstSumGPara->stPowVal.stFreeze;
//      stTerRunPara.stSumGroupPara[ucSumGNo].stVarVal.stFreeze = pstSumGPara->stVarVal.stFreeze;
//  }
//  else
//      ucRet = RET_ERROR;
//
//  return ucRet;
//}
//
//unsigned char RunPara_SetSumGroupOper(unsigned char ucNo,STSUMVALOPER *pstSumGValOper,unsigned char ucMPNo,unsigned char ucOper)
//{
//  unsigned char ucRet=RET_SUCCESS;
//
//  if(ucNo < CON_MAX_MPNUM)
//  {
//      pstSumGValOper->ucMPSum[ucNo]=ucMPNo;
//      pstSumGValOper->ucMPOper[ucNo]=ucOper;
//  }
//  else
//      ucRet = RET_ERROR;
//
//  return ucRet;
//}
//
//unsigned char RunPara_SetSwitchPara(unsigned char ucSwitchNo,STSWITCHPARA *pstSwitchPara)
//{
//  unsigned char ucRet=RET_SUCCESS;
//
//  if(ucSwitchNo < CON_SWITCHIN_NUM)
//      stTerRunPara.stSwitchPara[ucSwitchNo] = (*pstSwitchPara);
//  else
//      ucRet = RET_ERROR;
//
//  return ucRet;
//}

//unsigned char RunPara_SetAnalogPara(unsigned char ucAnalogNo,STANALOGPARA *pstAnalogPara)
//{
//  unsigned char ucRet=RET_SUCCESS;
//
//  if(ucAnalogNo < CON_ANALOG_NUM)
//      stTerRunPara.stAnalogPara[ucAnalogNo] = (*pstAnalogPara);
//  else
//      ucRet = RET_ERROR;
//
//  return ucRet;
//}
//unsigned char RunPara_SetLinkPara(STLINKPARA *pstLinkPara)
//{
//    if(pstLinkPara != NULL)
//        stTerRunPara.stLinkPara = *pstLinkPara;
//    return RET_SUCCESS;
//}
unsigned char RunPara_SetLinkRemove(STLINKREMOVE *pstLinkRemove)
{
    if(pstLinkRemove != NULL)
        stTerRunPara.stLinkPara.stLinkRemove = *pstLinkRemove;
    return RET_SUCCESS;
}
//unsigned char RunPara_SetPowerCtrlAvailble(unsigned char ucCurveNo,unsigned char ucFlag,unsigned char ucSumGroupNo)
//{
//  unsigned char ucRet=RET_SUCCESS;
//
//  if( (ucCurveNo < CONSWITCHCURVENUM)&&(ucSumGroupNo<CON_SUMGROUP_NUM) )
//      stTerRunPara.stSwitchCtrlPara.stCtrlSumGroupPara[ucSumGroupNo].stPowerCtrl[ucCurveNo].ucAvailbleFlag = ucFlag;
//  else
//      ucRet = RET_ERROR;
//  return ucRet;
//}
//
//unsigned char RunPara_SetCtrlSafeFixVal(STLOADFIXVAL *pstSaveFixVal)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  stTerRunPara.stSwitchCtrlPara.stSafeFixVal = (*pstSaveFixVal);
//  return ucRet;
//}

//unsigned char RunPara_SetCtrlFeeRate(STCTRLFEERATE *pstCtrlFeeRate)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  unsigned char i;
//  stTerRunPara.stSwitchCtrlPara.stCtrlFeeRate = (*pstCtrlFeeRate);
//  for(i=0;i<3;i++)
//  {
//      if(pstCtrlFeeRate->ulFeeRate[i]!=pstCtrlFeeRate->ulFeeRate[i+1])
//          break;
//  }
//  if(i!=3)                                //ǰ�ĸ����������һ�£������Ϊ����ѣ���λΪԪ
//  {
//      stTerRunPara.stTerAdjPara.ucEnerBuyUnit=1;
//      stTerRunPara.stTerAdjPara.ucEnerBuyIsNotFee=CON_STATUS_NO;  //�����
//  }
//  else if(pstCtrlFeeRate->ulFeeRate[0]==1)    //������ȣ���Ϊ1
//  {
//      stTerRunPara.stTerAdjPara.ucEnerBuyUnit=0;
//      stTerRunPara.stTerAdjPara.ucEnerBuyIsNotFee=CON_STATUS_YES; //������
//  }
//  return ucRet;
//}
//
//unsigned char RunPara_SetCtrlFeeRateTime(STCTRLFEERATETIME *pstCtrlFeeRateTime)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  stTerRunPara.stSwitchCtrlPara.stCtrlFeeRateTime = (*pstCtrlFeeRateTime);
//  return ucRet;
//}
//
//unsigned char RunPara_SetLoadCtrlWordPara(STLOADCTRLWORDPARA *pstLoadCtrlWordPara)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  stTerRunPara.stLoadCtrlWordPara = (*pstLoadCtrlWordPara);
//  return ucRet;
//}
//
//unsigned char RunPara_SetPowCtrlTimePer(unsigned char ucCurveNo,STPOWERCTRLTIME *pstPowerCtrlTime,unsigned char ucSumGroupNo)
//{
//  unsigned char ucRet=RET_ERROR;
//  if( (ucCurveNo<CONSWITCHCURVENUM)&&(ucSumGroupNo<CON_SUMGROUP_NUM) )
//  {
//      stTerRunPara.stSwitchCtrlPara.stCtrlSumGroupPara[ucSumGroupNo].stPowerCtrl[ucCurveNo] = (*pstPowerCtrlTime);
//      ucRet=RET_SUCCESS;
//  }
//  return ucRet;
//}
//unsigned char RunPara_SetPowCtrlTimePer_Availble(unsigned char ucFlag,unsigned char ucCurveNo,unsigned char ucSumGroupNo)
//{
//  unsigned char ucRet=RET_ERROR;
//  if( (ucCurveNo<CONSWITCHCURVENUM)&&(ucSumGroupNo<CON_SUMGROUP_NUM) )
//  {
//      stTerRunPara.stSwitchCtrlPara.stCtrlSumGroupPara[ucSumGroupNo].stPowerCtrl[ucCurveNo].ucAvailbleFlag = ucFlag;
//      ucRet=RET_SUCCESS;
//  }
//  return ucRet;
//}

//unsigned char RunPara_SetPowCtrlProj(unsigned char ucCurveNo,STPOWERCTRLPROJ *pstPowerCtrlProj,unsigned char ucSumGroupNo)
//{
//  unsigned char ucRet=RET_ERROR;
//  if( (ucCurveNo<CONSWITCHCURVENUM)&&(ucSumGroupNo<CON_SUMGROUP_NUM) )
//  {
//      stTerRunPara.stSwitchCtrlPara.stCtrlSumGroupPara[ucSumGroupNo].stPowerCtrlProj[ucCurveNo] = (*pstPowerCtrlProj);
//      ucRet=RET_SUCCESS;
//  }
//  return ucRet;
//}
//unsigned char RunPara_SetPowCtrlProj_Availble(unsigned char ucFlag,unsigned char ucCurveNo,unsigned char ucSumGroupNo)
//{
//  unsigned char ucRet=RET_ERROR;
//  if( (ucCurveNo<CONSWITCHCURVENUM)&&(ucSumGroupNo<CON_SUMGROUP_NUM) )
//  {
//      stTerRunPara.stSwitchCtrlPara.stCtrlSumGroupPara[ucSumGroupNo].stPowerCtrlProj[ucCurveNo].ucProjAvailbleFlag = ucFlag;
//      ucRet=RET_SUCCESS;
//  }
//  return ucRet;
//}
//
//unsigned char RunPara_SetEnergyCtrl(STENERGYCTRL *pstEnergyCtrl,unsigned char ucSumGroupNo)
//{
//  unsigned char ucRet=RET_ERROR;
//  if(ucSumGroupNo<CON_SUMGROUP_NUM)
//  {
//      stTerRunPara.stSwitchCtrlPara.stCtrlSumGroupPara[ucSumGroupNo].stEnergyCtrl = (*pstEnergyCtrl);
//      ucRet=RET_SUCCESS;
//  }
//  return ucRet;
//}
//
//unsigned char RunPara_SetBuyEnergyRec(STBUYENERGYREC *pstBuyEnergyRec,unsigned char ucSumGroupNo)
//{
//  unsigned char ucRet=RET_ERROR;
//  if(ucSumGroupNo<CON_SUMGROUP_NUM)
//  {
//      stTerRunPara.stSwitchCtrlPara.stCtrlSumGroupPara[ucSumGroupNo].stBuyEnergyRec = (*pstBuyEnergyRec);
//      ucRet=RET_SUCCESS;
//  }
//  return ucRet;
//}
//
//unsigned char RunPara_SetMonEnergyRec(STMONENERGYREC *pstMonEnergyRec,unsigned char ucSumGroupNo)
//{
//  unsigned char ucRet=RET_ERROR;
//  if(ucSumGroupNo<CON_SUMGROUP_NUM)
//  {
//      stTerRunPara.stSwitchCtrlPara.stCtrlSumGroupPara[ucSumGroupNo].stMonEnergyRec = (*pstMonEnergyRec);
//      ucRet=RET_SUCCESS;
//  }
//  return ucRet;
//}
//
//unsigned char RunPara_SetPowerCtrlTurn(STCTRLTURN *pstPowerCtrlTurn,unsigned char ucSumGroupNo)
//{
//  unsigned char ucRet=RET_ERROR;
//  if(ucSumGroupNo<CON_SUMGROUP_NUM)
//  {
//      stTerRunPara.stSwitchCtrlPara.stCtrlSumGroupPara[ucSumGroupNo].stPowerCtrlTurn = (*pstPowerCtrlTurn);
//      ucRet=RET_SUCCESS;
//  }
//  return ucRet;
//}
//
//unsigned char RunPara_SetEnergyCtrlTurn(STCTRLTURN *pstEnergyCtrlTurn,unsigned char ucSumGroupNo)
//{
//  unsigned char ucRet=RET_ERROR;
//  if(ucSumGroupNo<CON_SUMGROUP_NUM)
//  {
//      stTerRunPara.stSwitchCtrlPara.stCtrlSumGroupPara[ucSumGroupNo].stEnergyCtrlTurn = (*pstEnergyCtrlTurn);
//      ucRet=RET_SUCCESS;
//  }
//  return ucRet;
//}
//
//unsigned char RunPara_SetPowCtrlHoldTime(STPOWERCTRLHOLDTIME *pstPowerHoldTime,unsigned char ucSumGroupNo)
//{
//  unsigned char ucRet=RET_ERROR;
//  if(ucSumGroupNo<CON_SUMGROUP_NUM)
//  {
//      stTerRunPara.stSwitchCtrlPara.stCtrlSumGroupPara[ucSumGroupNo].stPowerHoldTime = (*pstPowerHoldTime);
//      ucRet=RET_SUCCESS;
//  }
//  return ucRet;
//}

//unsigned char RunPara_SetKeepPower(unsigned char ucFlag)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  stTerRunPara.stSwitchCtrlPara.ucKeepPower=ucFlag;
//  return ucRet;
//}
//unsigned char RunPara_SetGprsKeepPow(unsigned char ucFlag)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  //stTerRunPara.stSwitchCtrlPara.ucGprsKeepPow=ucFlag;
//  stTerRunPara.stSwitchCtrlPara.ucGprsKeepPow=CON_INVALIBLE_FLAG;
//  return ucRet;
//}
//unsigned char RunPara_SetGprsCheck(unsigned char ucFlag)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  stTerRunPara.stSwitchCtrlPara.ucIsCheckGprs=ucFlag;
//  return ucRet;
//}
unsigned char RunPara_SetTerAuxPara(STTERAUXPARA *pstTerAuxPara)
{
    unsigned char ucRet=RET_SUCCESS;
    stTerRunPara.stTerAuxPara = (*pstTerAuxPara);
    return ucRet;
}

unsigned char RunPara_SetTerDevAddr(STTERDEVADDR *pstTerAddr)
{
    unsigned char ucRet=RET_SUCCESS;
    stTerRunPara.stTerAddr = (*pstTerAddr);
    return ucRet;
}

unsigned char RunPara_SetTerRegionAddr(STTERREGIONADDR *pstTerRegionAddr)
{
    unsigned char ucRet=RET_SUCCESS;
    stTerRunPara.stTerRegionAddr = (*pstTerRegionAddr);
    return ucRet;
}



unsigned char RunPara_SetJobPara(unsigned char ucJobClassNo,unsigned char ucJobNo,STJOBPARA *pstJobPara)
{
    unsigned char ucRet=RET_SUCCESS;
    if(ucJobNo < CONDATAJOBNUM)
    {
        switch(ucJobClassNo)
        {
            case 0:
                stTerRunPara.stJobPara_Data1[ucJobNo] = (*pstJobPara);
                break;
            case 1:
                stTerRunPara.stJobPara_Data2[ucJobNo] = (*pstJobPara);
                break;
        }
    }
    else
        ucRet = RET_ERROR;
    return ucRet;
}
unsigned char RunPara_SetUpUsrJob(unsigned char ucJobNo,unsigned char ucUpUsr,STUPUSR *pstUpUsr)
{
    unsigned char ucRet = RET_SUCCESS;

    if(ucUpUsr < CON_UPUSR_NUM)
    {
        switch(ucJobNo)
        {
            case 0:
                stTerRunPara.stJobFnPara1.stUpUsr[ucUpUsr] = *pstUpUsr;
                break;
            case 1:
                stTerRunPara.stJobFnPara2.stUpUsr[ucUpUsr] = *pstUpUsr;
                break;
        }
    }else
        ucRet = RET_ERROR;
    return ucRet;
}

unsigned char RunPara_SetPortPropList(STPORTLIST *pstPortList)
{
    stTerRunPara.stPortList = *pstPortList;
    return RET_SUCCESS;
}
//unsigned char RunPara_SetPortProp(STPORTPROP *pstPortProp,unsigned char ucPort)
//{
//    if(ucPort < CON_COMMPORT_NUM)
//        stTerRunPara.stPortList.stPortProp[ucPort] = *pstPortProp;
//    return RET_SUCCESS;
//}
//unsigned char RunPara_SetRMRunPara(STRMRUNPARA *pstRMRunPara)
//{
//  if(pstRMRunPara != NULL)
//      stTerRunPara.stPortList.stRMRunPara = *pstRMRunPara;
//  return RET_SUCCESS;
//}
//
//unsigned char RunPara_SetTerNormalPara(STTERNORMALPARA *pstTerNormalPara)
//{
//    unsigned char ucRet=RET_SUCCESS;
//    stTerRunPara.stTerNormalPara = (*pstTerNormalPara);
//    return ucRet;
//}
unsigned char RunPara_SetCopyFlag(unsigned char ucCopyFlag)
{
    if(ucCopyFlag == CON_STATUS_NO || ucCopyFlag == CON_STATUS_YES)
        stTerRunPara.stTerNormalPara.ucCopyFlag = ucCopyFlag;
    return RET_SUCCESS;
}
unsigned char RunPara_SetTerMiscPara(STTERMISCPARA *pstTerMiscPara)
{
    unsigned char ucRet=RET_SUCCESS;
    stTerRunPara.stTerMiscPara = (*pstTerMiscPara);
    return ucRet;
}

//unsigned char RunPara_SetTerAdjPara(STTERADJPARA *pstTerAdjPara)
//{
//	unsigned char ucRet=RET_SUCCESS;
//	stTerRunPara.stTerAdjPara = (*pstTerAdjPara);
//	return ucRet;
//}
//unsigned char RunPara_SetCTCheckPara(STCTCHECKPARA *pstCTCheckPara)
//{
//  unsigned char ucRet=RET_SUCCESS;
//
//  if(pstCTCheckPara!=NULL)
//      stTerRunPara.stCTCheckPara = (*pstCTCheckPara);
//  else
//      ucRet=RET_ERROR;
//
//  return ucRet;
//}

//unsigned char RunPara_SetTerAdjPara_DownloadSource(unsigned char ucSource)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  stTerRunPara.stTerAdjPara.ucDownProgramSource = ucSource;
//  return ucRet;
//}

//unsigned char RunPara_SetTerAdjPara_Change_ViewData_1(void)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  if(stTerRunPara.stTerAdjPara.ucViewData_1==1)
//      stTerRunPara.stTerAdjPara.ucViewData_1=0;
//  else
//      stTerRunPara.stTerAdjPara.ucViewData_1=1;
//  return ucRet;
//}

//unsigned char RunPara_SetUartBoad(unsigned char ucUartID,unsigned long ulBoad)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  if(ucUartID<MAX_PORT_NUM)
//      stTerRunPara.stTerMiscPara.ulBoadrate[ucUartID]=Get_Correct_Boadrate(ulBoad);
//  else
//      ucRet = RET_ERROR;
//  return ucRet;
//}

//unsigned char RunPara_SetTerMiscParaStatus_ProgramKeyIsSet(unsigned char ucFlag)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  stTerRunPara.stTerMiscPara.ucProgramKeyIsSet=ucFlag;
//  return ucRet;
//}

//unsigned char RunPara_SetTerMiscPara_TerAddIsDecType(unsigned char ucFlag)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  stTerRunPara.stTerMiscPara.ucTerAddIsDecType=ucFlag;
//  return ucRet;
//}
unsigned char RunPara_SetTerMiscPara_FluxLimit(unsigned long ulVal)
{
    unsigned char ucRet=RET_SUCCESS;
    stTerRunPara.stTerMiscPara.ulFluxLimit=ulVal;
    return ucRet;
}

//azh 170919
unsigned char   RunPara_SetTerMisc_Wireless(unsigned char* ucVal)
{
    unsigned char ucRet=RET_SUCCESS;
    memcpy(stTerRunPara.ucWireless, ucVal, 2);//2byte
	return ucRet;
}
unsigned char   RunPara_SetTerMisc_TerLocation(unsigned char* ucVal)
{
    unsigned char ucRet=RET_SUCCESS;//10byte
	memcpy(stTerRunPara.ucLocation, ucVal, 10);
	return ucRet;
}
unsigned char   RunPara_SetTerMisc_Snstate(unsigned char* ucVal)
{
    unsigned char ucRet=RET_SUCCESS;
	memcpy(stTerRunPara.ucSnstate, ucVal, 2);//512byte//̫��ʵ��֧��2�ֽ�
	return ucRet;
}

//unsigned char RunPara_SetTerMiscPara_ShowRCommDebug(unsigned char ucVal)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  stTerRunPara.stTerMiscPara.ucShowRCommDebug=ucVal;
//  return ucRet;
//}

//unsigned char RunPara_SetAlrJuDgeTime(STALRJUDGETIME *pstAlarmJudgeTime)
//{
//  unsigned char ucRet=RET_SUCCESS;
//   stTerRunPara.stAlarmJudgeTime=(*pstAlarmJudgeTime);
//  return ucRet;
//}
//unsigned char RunPara_SetAlrCtrlTime(STCURALRCTRLTIME *pstCurAlrCtrlTime)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  stTerRunPara.stCurAlrCtrlTime   = (*pstCurAlrCtrlTime);
//  return ucRet;
//}

//unsigned char RunPara_SetHarmValUpLimit(STHARMVALUPLIMIT *pstHarmValUpLimit)
//{
//  unsigned char ucRet=RET_SUCCESS;
//  stTerRunPara.stHarmValUpLimit   =(*pstHarmValUpLimit);
//  return ucRet;
//}

//unsigned char  RunPara_SetAesKey(STAESKEY *pstAesKey)     //AES����
//{
//  unsigned char ucRet = RET_SUCCESS;
//  stTerRunPara.stAesKey   =(*pstAesKey);
//  return ucRet;
//}
/*********************************************************************************************************************/
//STAESKEY  RunPara_GetAesKey(void)                 ////AES����
//{
//  return stTerRunPara.stAesKey;
//}

//STCURALRCTRLTIME    RunPara_GetAlrCtrlTime(void)
//{
//    return stTerRunPara.stCurAlrCtrlTime;
//}
//STALRJUDGETIME RunPara_GetAlrJuDgeTime(void)
//{
//    return stTerRunPara.stAlarmJudgeTime;
//}
//STHARMVALUPLIMIT  RunPara_GetHarmValUpLimit(void)
//{
//  return stTerRunPara.stHarmValUpLimit;
//}

unsigned char RunPara_GetMeter_MPNo(unsigned char ucMeterNo)
{//�жϱ����Ƿ����
    unsigned char ucMPNo=CON_MPNO_INVALID;
    unsigned char i;
    if(ucMeterNo == 0)
    {
        return 0;
    }
    for(i=0;i<CON_MAX_MPNUM;i++)
    {
        if(stTerRunPara.stMPInfo[i].ucMPType==CON_MPTYPE_METER && stTerRunPara.stMPInfo[i].ucDevNo[0]==ucMeterNo)
        {
            ucMPNo=i;
            break;
        }
    }
    return ucMPNo;
}
//unsigned char RunPara_GetPulse_MPNo(unsigned char ucPulseNo)
//{//�ж������Ƿ����
//  unsigned char ucMPNo=CON_MPNO_INVALID;
//  unsigned char i;
//  for(i=0;i<CON_MAX_MPNUM;i++)
//  {
//      if(stTerRunPara.stMPInfo[i].ucMPType==CON_MPTYPE_PULSE)
//      {
//          if((stTerRunPara.stMPInfo[i].ucDevNo[0]==ucPulseNo)
//              || (stTerRunPara.stMPInfo[i].ucDevNo[1]==ucPulseNo)
//              || (stTerRunPara.stMPInfo[i].ucDevNo[2]==ucPulseNo)
//              || (stTerRunPara.stMPInfo[i].ucDevNo[3]==ucPulseNo) )
//          {
//              ucMPNo=i;
//              break;
//          }
//      }
//  }
//  return ucMPNo;
//}
unsigned char RunPara_GetTer_MPNo(void)
{//�ж��ն��Ƿ����
//  unsigned char ucMPNo=CON_MPNO_INVALID;
    unsigned char ucMPNo=CON_MPNO_TER;          //��֤�������ݲ��쳣������Ϊ63���ܼ�����ռ�ã�����ѡ��62
    unsigned char i;
    for(i=0;i<CON_MAX_MPNUM;i++)
    {
        if(stTerRunPara.stMPInfo[i].ucMPType==CON_MPTYPE_TER)
        {
            ucMPNo=i;
            break;
        }
    }
    if(i>=CON_MAX_MPNUM)
    {
        //stTerRunPara.stMPInfo[CON_MPNO_TER].ucMPType=CON_MPTYPE_TER;
        ucMPNo=CON_MPNO_TER;//�����ն˱���û�м��� ������������
    }

    return ucMPNo;
}
unsigned char RunPara_GetMeterNo_byMPNo(unsigned char ucMPNo)
{
    unsigned char ucMeterNo=0xFF;
	if(ucMPNo == 0)
	{
	    return 0;
	}    
    if(ucMPNo<CON_MAX_MPNUM)
    {
        if(stTerRunPara.stMPInfo[ucMPNo].ucMPType==CON_MPTYPE_METER)
            ucMeterNo=stTerRunPara.stMPInfo[ucMPNo].ucDevNo[0];
    }
    return ucMeterNo;
}
//unsigned char RunPara_GetPulseNo_byMPNo(unsigned char ucMPNo,unsigned char ucPulseType)
//{
//  unsigned char ucPulseNo=0xFF,ucPos;
//  if((ucPulseType==CON_PULSEATTR_DIRPOW)
//      || (ucPulseType==CON_PULSEATTR_REVPOW)
//      || (ucPulseType==CON_PULSEATTR_DIRVAR)
//      || (ucPulseType==CON_PULSEATTR_REVVAR) )
//      ucPos=((unsigned char)(ucPulseType-1) & 0x3);
//  else
//      ucPos=0;
//  if(ucMPNo<CON_MAX_MPNUM)
//  {
//      if(stTerRunPara.stMPInfo[ucMPNo].ucMPType==CON_MPTYPE_PULSE)
//          ucPulseNo=stTerRunPara.stMPInfo[ucMPNo].ucDevNo[ucPos];
//  }
//  return ucPulseNo;
//}
//unsigned char RunPara_GetDirPowPulseNo_byMPNo(unsigned char ucMPNo)
//{
//  unsigned char ucPulseNo=0xFF;
//  ucPulseNo = RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRPOW);
//  return ucPulseNo;
//}
//unsigned char RunPara_GetDirVarPulseNo_byMPNo(unsigned char ucMPNo)
//{
//  unsigned char ucPulseNo=0xFF;
//  ucPulseNo = RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_DIRVAR);
//  return ucPulseNo;
//}
//unsigned char RunPara_GetUnDirPowPulseNo_byMPNo(unsigned char ucMPNo)
//{
//  unsigned char ucPulseNo=0xFF;
//  ucPulseNo = RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVPOW);
//  return ucPulseNo;
//}
//unsigned char RunPara_GetUnDirVarPulseNo_byMPNo(unsigned char ucMPNo)
//{
//  unsigned char ucPulseNo=0xFF;
//  ucPulseNo = RunPara_GetPulseNo_byMPNo(ucMPNo,CON_PULSEATTR_REVVAR);
//  return ucPulseNo;
//}

/*********************************************************************************************************************/
void RunPara_ClearPassErrTimes()
{
    STTERENCRYPT  stTerEncrypt;
    stTerEncrypt=RunPara_GetTerEncrypt();
    stTerEncrypt.ucErrTimes=0;
    if(stTerEncrypt.ucAlgoCode!=CON_ENCRYPT_DIR && stTerEncrypt.ucAlgoCode!=CON_ENCRYPT_CRC)
        stTerEncrypt.ucAlgoCode=CON_ENCRYPT_INVALID;
    RunPara_SetTerEncrypt(&stTerEncrypt);
}

unsigned char RunPara_GetPassErrTimes()
{
    STTERENCRYPT  stTerEncrypt;
    stTerEncrypt=RunPara_GetTerEncrypt();
    return stTerEncrypt.ucErrTimes;
}

void RunPara_AddPassErrTimes()
{
    STTERENCRYPT  stTerEncrypt;
    STDATETIME    stCurrentTime;
    stTerEncrypt=RunPara_GetTerEncrypt();
    if(stTerEncrypt.ucErrTimes<=CON_MAX_PASSERR_TIMES)
        stTerEncrypt.ucErrTimes++;
    if(stTerEncrypt.ucErrTimes==CON_MAX_PASSERR_TIMES)
    {
        stCurrentTime=GetCurrentTime();
        stTerEncrypt.ulLockTime=TimeConver(&stCurrentTime,CON_TIMEUNIT_SEC);
    }
    RunPara_SetTerEncrypt(&stTerEncrypt);
}

unsigned char RunPara_PassLockIsOverTime()
{
    unsigned char ucRet=RET_ERROR;
    STTERENCRYPT  stTerEncrypt;
    STDATETIME    stCurrentTime;
    unsigned long ulCurrentTime;
    stTerEncrypt=RunPara_GetTerEncrypt();
    stCurrentTime=GetCurrentTime();
    ulCurrentTime=TimeConver(&stCurrentTime,CON_TIMEUNIT_SEC);
    if(ulCurrentTime>(stTerEncrypt.ulLockTime+CON_PASSERR_LOCK_TIME))
        ucRet=RET_SUCCESS;
    return ucRet;
}

//unsigned char     RunPara_GetCtrlFeeNo_ByTime(STDATETIME *pstDateTime)
//{//����ʱ���÷��ʱ�ţ���1��ʼ
//  unsigned char ucFeeNo=0xFF;
//  unsigned char ucTmp;
//  STCTRLFEERATETIME   stCtrlFeeRateTime;
//  if(Is_DateTime_Availble(pstDateTime)==RET_SUCCESS)
//  {
//      stCtrlFeeRateTime=RunPara_GetCtrlFeeRateTime();
//      ucTmp=pstDateTime->ucHour*2;
//      if(pstDateTime->ucMinute>=30)
//          ucTmp+=1;
//      ucFeeNo=stCtrlFeeRateTime.ucFeeRateNo[ucTmp]+1;
//  }
//  return ucFeeNo;
//}

//unsigned char RunPara_InitMeterProtocolInfo()
//{
//    unsigned char ucBuff[80],ucBuff2[80];
//    memset(ucBuff,0x00,80);
//    ucBuff[72]=0x55;                //8�ֽڱ�־
//    ucBuff[73]=0xAA;
//    ucBuff[74]=0x33;
//    ucBuff[75]=0xCC;
//    ucBuff[76]=0x5A;
//    ucBuff[77]=0xA5;
//    ucBuff[78]=0x3C;
//    ucBuff[79]=0xC3;
//    ucBuff[CON_PROTOCOL_DL645]      =CON_EXT_PROTOCOL_DL645;
//    ucBuff[CON_PROTOCOL_SAMPLE]     =CON_EXT_PROTOCOL_SAMPLE;
//    ucBuff[CON_PROTOCOL_ABB_R]      =CON_EXT_PROTOCOL_ABB_R;
//    ucBuff[CON_PROTOCOL_LANDD]      =CON_EXT_PROTOCOL_LANDD;
//    ucBuff[CON_PROTOCOL_LANDB]      =CON_EXT_PROTOCOL_LANDB;
////  ucBuff[CON_PROTOCOL_GENIUS]     =CON_EXT_PROTOCOL_GENIUS;
//    ucBuff[CON_PROTOCOL_WS]         =CON_EXT_PROTOCOL_WS;
//    ucBuff[CON_PROTOCOL_AS1_DL645]=CON_EXT_PROTOCOL_AS1_DL645;
//    ucBuff[CON_PROTOCOL_AS2_DL645]=CON_EXT_PROTOCOL_AS2_DL645;
//    ucBuff[CON_PROTOCOL_HND]        =CON_EXT_PROTOCOL_HND;
//    ucBuff[CON_PROTOCOL_HLi]        =CON_EXT_PROTOCOL_HLi;
//    ucBuff[CON_PROTOCOL_KL]         =CON_EXT_PROTOCOL_KL;
//    ucBuff[CON_PROTOCOL_LD]         =CON_EXT_PROTOCOL_LD;
//    ucBuff[CON_PROTOCOL_SKD]        =CON_EXT_PROTOCOL_SKD;
//    ucBuff[CON_PROTOCOL_JSLY]       =CON_EXT_PROTOCOL_JSLY;
//    ucBuff[CON_PROTOCOL_HND645]     =CON_EXT_PROTOCOL_HND645;
//    ucBuff[CON_PROTOCOL_DFDZ]       =CON_EXT_PROTOCOL_DFDZ;
//    ucBuff[CON_PROTOCOL_JQ]         =CON_EXT_PROTOCOL_JQ;
//    ucBuff[CON_PROTOCOL_DL64507]    =CON_EXT_PROTOCOL_DL64507;
//    ucBuff[CON_PROTOCOL_HB_PT]      =CON_EXT_PROTOCOL_HB_PT;
//    ucBuff[CON_PROTOCOL_HB_JM]      =CON_EXT_PROTOCOL_HB_JM;
////azh   Flash_SetMainMemory(ucBuff,80,CON_DFPAGE_HEAD_RESERVE1,0);
////  Flash_GetMainMemory(CON_DFPAGE_HEAD_RESERVE1,0,80,ucBuff2);
//    if(memcmp(ucBuff,ucBuff2,80)!=0)
//    {//�쳣��дһ�Σ����治�ٴ���
////azh       Flash_SetMainMemory(ucBuff,80,CON_DFPAGE_HEAD_RESERVE1,0);
//    }
//    return RET_SUCCESS;
//}

unsigned char RunPara_GetMeterProtocolInfo(unsigned char *pucInfoBuff)
{
    unsigned char ucBuff[80];
    memset(pucInfoBuff,0x00,80);
//azh   Flash_GetMainMemory(CON_DFPAGE_HEAD_RESERVE1,0,80,pucInfoBuff);
    if(pucInfoBuff[72]==0x55 && pucInfoBuff[73]==0xAA && pucInfoBuff[74]==0x33 && pucInfoBuff[75]==0xCC
        && pucInfoBuff[76]==0x5A && pucInfoBuff[77]==0xA5 && pucInfoBuff[78]==0x3C && pucInfoBuff[79]==0xC3)
    {//��Ч
        ;       //ֱ�ӷ���
    }
    else
    {//��Ч
        memset(pucInfoBuff,0x00,80);
        pucInfoBuff[72]=0x55;               //8�ֽڱ�־
        pucInfoBuff[73]=0xAA;
        pucInfoBuff[74]=0x33;
        pucInfoBuff[75]=0xCC;
        pucInfoBuff[76]=0x5A;
        pucInfoBuff[77]=0xA5;
        pucInfoBuff[78]=0x3C;
        pucInfoBuff[79]=0xC3;
        pucInfoBuff[CON_PROTOCOL_DL645]     =CON_EXT_PROTOCOL_DL645;
        pucInfoBuff[CON_PROTOCOL_SAMPLE]    =CON_EXT_PROTOCOL_SAMPLE;
        pucInfoBuff[CON_PROTOCOL_ABB_R]     =CON_EXT_PROTOCOL_ABB_R;
        pucInfoBuff[CON_PROTOCOL_LANDD]     =CON_EXT_PROTOCOL_LANDD;
        pucInfoBuff[CON_PROTOCOL_LANDB]     =CON_EXT_PROTOCOL_LANDB;
//      pucInfoBuff[CON_PROTOCOL_GENIUS]    =CON_EXT_PROTOCOL_GENIUS;
        pucInfoBuff[CON_PROTOCOL_WS]        =CON_EXT_PROTOCOL_WS;
        pucInfoBuff[CON_PROTOCOL_AS1_DL645]=CON_EXT_PROTOCOL_AS1_DL645;
        pucInfoBuff[CON_PROTOCOL_AS2_DL645]=CON_EXT_PROTOCOL_AS2_DL645;
        pucInfoBuff[CON_PROTOCOL_HND]       =CON_EXT_PROTOCOL_HND;
        pucInfoBuff[CON_PROTOCOL_HLi]       =CON_EXT_PROTOCOL_HLi;
        pucInfoBuff[CON_PROTOCOL_KL]        =CON_EXT_PROTOCOL_KL;
        pucInfoBuff[CON_PROTOCOL_LD]        =CON_EXT_PROTOCOL_LD;
        pucInfoBuff[CON_PROTOCOL_SKD]       =CON_EXT_PROTOCOL_SKD;
        pucInfoBuff[CON_PROTOCOL_JSLY]      =CON_EXT_PROTOCOL_JSLY;
        pucInfoBuff[CON_PROTOCOL_HND645]    =CON_EXT_PROTOCOL_HND645;
        pucInfoBuff[CON_PROTOCOL_DFDZ]      =CON_EXT_PROTOCOL_DFDZ;
        pucInfoBuff[CON_PROTOCOL_JQ]        =CON_EXT_PROTOCOL_JQ;
        pucInfoBuff[CON_PROTOCOL_DL64507]   =CON_EXT_PROTOCOL_DL64507;
        pucInfoBuff[CON_PROTOCOL_HB_JM]     =CON_EXT_PROTOCOL_HB_JM;
        pucInfoBuff[CON_PROTOCOL_HB_PT]     =CON_EXT_PROTOCOL_HB_PT;
//azh       Flash_SetMainMemory(pucInfoBuff,80,CON_DFPAGE_HEAD_RESERVE1,0);
//azh       Flash_GetMainMemory(CON_DFPAGE_HEAD_RESERVE1,0,80,ucBuff);
        if(memcmp(pucInfoBuff,ucBuff,80)!=0)
        {//�쳣��дһ�Σ����治�ٴ���
//azh           Flash_SetMainMemory(pucInfoBuff,80,CON_DFPAGE_HEAD_RESERVE1,0);
        }
    }
    return RET_SUCCESS;
}

unsigned char RunPara_GetExternMeterProtocol(unsigned char MeterProtocol)
{//����ⲿ��ͨѶ��Լ
    unsigned char ucRet=CON_EXT_PROTOCOL_INVALID;
    unsigned char ucBuff[80];

    RunPara_GetMeterProtocolInfo(ucBuff);
    if(MeterProtocol<72)
        ucRet=ucBuff[MeterProtocol];
    return ucRet;

/*
    switch(MeterProtocol)
    {
    case CON_PROTOCOL_DL645:
        ucRet=CON_EXT_PROTOCOL_DL645;
        break;
    case    CON_PROTOCOL_HLi:
        ucRet=CON_EXT_PROTOCOL_HLi;
        break;
    case    CON_PROTOCOL_KL:
        ucRet=CON_EXT_PROTOCOL_KL;
        break;
    case    CON_PROTOCOL_WS:
        ucRet=CON_EXT_PROTOCOL_WS;
        break;
    case    CON_PROTOCOL_LD:
        ucRet=CON_EXT_PROTOCOL_LD;
        break;
    case    CON_PROTOCOL_SKD:
        ucRet=CON_EXT_PROTOCOL_SKD;
        break;
    case    CON_PROTOCOL_JSLY:
        ucRet=CON_EXT_PROTOCOL_JSLY;
        break;
    case    CON_PROTOCOL_HND645:
        ucRet=CON_EXT_PROTOCOL_HND645;
        break;
    case    CON_PROTOCOL_DFDZ:
        ucRet=CON_EXT_PROTOCOL_DFDZ;
        break;
    case    CON_PROTOCOL_JQ:
        ucRet=CON_EXT_PROTOCOL_JQ;
        break;
    default:
        break;
    }
*/
}
unsigned char RunPara_GetInnerMeterProtocol(unsigned char MeterProtocol)
{//����ڲ���ͨѶ��Լ
    unsigned char ucRet=CON_PROTOCOL_INVALID;
    unsigned char i;
    unsigned char ucBuff[80];

    RunPara_GetMeterProtocolInfo(ucBuff);
    for(i=0;i<72;i++)
    {
        if(ucBuff[i]==MeterProtocol)
        {
            ucRet=i;
            break;
        }
    }
    return ucRet;
/*
    switch(MeterProtocol)
    {
    case CON_EXT_PROTOCOL_DL645:
        ucRet=CON_PROTOCOL_DL645;
        break;
    case    CON_EXT_PROTOCOL_HLi:
        ucRet=CON_PROTOCOL_HLi;
        break;
    case    CON_EXT_PROTOCOL_KL:
        ucRet=CON_PROTOCOL_KL;
        break;
    case    CON_EXT_PROTOCOL_WS:
        ucRet=CON_PROTOCOL_WS;
        break;
    case    CON_EXT_PROTOCOL_LD:
        ucRet=CON_PROTOCOL_LD;
        break;
    case    CON_EXT_PROTOCOL_SKD:
        ucRet=CON_PROTOCOL_SKD;
        break;
    case    CON_EXT_PROTOCOL_JSLY:
        ucRet=CON_PROTOCOL_JSLY;
        break;
    case    CON_EXT_PROTOCOL_HND645:
        ucRet=CON_PROTOCOL_HND645;
        break;
    case    CON_EXT_PROTOCOL_DFDZ:
        ucRet=CON_PROTOCOL_DFDZ;
        break;
    case    CON_EXT_PROTOCOL_JQ:
        ucRet=CON_PROTOCOL_JQ;
        break;
    default:
        break;
    }
*/
}
unsigned char Inner_RunPara_GetDefaultMeterCommPara(STMETERPARA *pstMeterPara,unsigned char ucInnerMeterProtocol)
{
    unsigned char ucRet=RET_SUCCESS;
    switch(ucInnerMeterProtocol)
    {
    case CON_PROTOCOL_DL645:
    case CON_PROTOCOL_AS1_DL645:
    case CON_PROTOCOL_AS2_DL645:
        pstMeterPara->ulBaudRate=1200;
        pstMeterPara->ucDataBit=8;
        pstMeterPara->ucEPN=UART_EVEN_PARITY;//CON_UART_EPN_EVEN;
        pstMeterPara->ucStopBit=UART_1_STOP_BIT;//CON_UART_STPB_1;
        break;
    default:
        pstMeterPara->ulBaudRate=1200;
        pstMeterPara->ucDataBit=8;
        pstMeterPara->ucEPN=UART_EVEN_PARITY;//CON_UART_EPN_EVEN;
        pstMeterPara->ucStopBit=UART_1_STOP_BIT;//CON_UART_STPB_1;
        break;
    }
    return ucRet;
}
unsigned char RunPara_GetDefaultMeterCommPara(STMETERPARA *pstMeterPara,unsigned char ucInnerMeterProtocol)
{
      unsigned char ucTmp;//,i;
      ucTmp = Inner_RunPara_GetDefaultMeterCommPara(pstMeterPara,ucInnerMeterProtocol);
      if(ucInnerMeterProtocol == 0xF0)
      {
//      for(i=0;i<5;i++)
//          Call_NOP();
      }

    return ucTmp;

}

unsigned long RunPara_Get_Relay_Boadrate(unsigned char ucBoadInfo)
{
    unsigned long ulBoad=1200;
    switch(ucBoadInfo)
    {
    case 0:
        ulBoad=300;
        break;
    case 1:
        ulBoad=600;
        break;
    case 2:
        ulBoad=1200;
        break;
    case 3:
        ulBoad=2400;
        break;
    case 4:
        ulBoad=4800;
        break;
    case 5:
        ulBoad=7200;
        break;
    case 6:
        ulBoad=9600;
        break;
    case 7:
        ulBoad=19200;
        break;
    }
    return ulBoad;
}

//void RunPara_Get_Relay_CommInfo(unsigned char ucCommInfo,unsigned char *pucEPN,unsigned char *pucDataBit,unsigned char *pucStopBit)
//{
//  unsigned char ucTmp;
//  if((ucCommInfo & 0x08)==0)
//      (*pucEPN)=UART_NO_PARITY;//CON_UART_EPN_OFF;
//  else
//  {
//      if((ucCommInfo & 0x04)==0)
//          (*pucEPN)=UART_EVEN_PARITY;//CON_UART_EPN_EVEN;
//      else
//          (*pucEPN)=UART_ODD_PARITY;//CON_UART_EPN_ODD;
//  }
//  ucTmp=ucCommInfo & 0x03;
//  switch(ucTmp)
//  {
//  case 0:
//      (*pucDataBit)=5;
//      break;
//  case 1:
//      (*pucDataBit)=6;
//      break;
//  case 2:
//      (*pucDataBit)=7;
//      break;
//  default:
//      (*pucDataBit)=8;
//      break;
//  }
//  if((ucCommInfo & 0x10)==0)
//      (*pucStopBit)=UART_1_STOP_BIT;//CON_UART_STPB_1;
//  else
//      (*pucStopBit)=UART_2_STOP_BIT;//CON_UART_STPB_2;
//}
//unsigned char RunPara_GetPulseType_ByDI(unsigned short usDI)
//{
//  unsigned char ucRet=CON_PULSEATTR_INVALID;
//  switch(usDI)
//  {
//  case CON_DI_TYPE_MPDIRPOWTOTAL:
//  case CON_DI_TYPE_MPINSTANTPOW:
//      ucRet=CON_PULSEATTR_DIRPOW;
//      break;
//  case CON_DI_TYPE_MPUNDIRPOWTOTAL:
//      ucRet=CON_PULSEATTR_REVPOW;
//      break;
//  case CON_DI_TYPE_MPDIRVARTOTAL:
//  case CON_DI_TYPE_MPINSTANTVAR:
//      ucRet=CON_PULSEATTR_DIRVAR;
//      break;
//  case CON_DI_TYPE_MPUNDIRVARTOTAL:
//      ucRet=CON_PULSEATTR_REVVAR;
//      break;
//  default:
//      break;
//  }
//  return ucRet;
//}
//unsigned short RunPara_GetCommMode(void)
//{
//  return stTerRunPara.stTerCommPara_Inner.ucRCommMode;
//}

unsigned short RunPara_GetReConnectTime(void)
{

    return stTerRunPara.stTerCommPara_Inner.usReConnectInter*2;

}
unsigned char   RunPara_GetTerCommPara_ReConnectTimes(void)
{
    if(stTerRunPara.stTerCommPara_Inner.ucRCommMode==CON_ONLINE_ALLWAYS)//��������
        return 0xFF;
    else
        return stTerRunPara.stTerCommPara_Inner.ucReConnectTimes;
}

unsigned short  RunPara_GetTerCommPara_StandbyTime(void)
{
    return stTerRunPara.stTerCommPara_Inner.ucStandbyTime*120;      // 1����ת����λΪ0.5��
}

//STBUYBILLREC RunPara_GetBuyBillRec_byBillNo(unsigned char ucBillNo)
//{
//  STBUYBILLREC stBuyBillTmp;
//  SetBuyBillRec(&stBuyBillTmp, 0xFFFFFFFF, 0xFF);
//  if(ucBillNo < CON_BUYBILL_NUM)
//      return stTerRunPara.stSwitchCtrlPara.stBuyBillRec[ucBillNo];
//
//  return stBuyBillTmp;
//}
// unsigned char RunPara_SetBuyBillRec_byBillNo(STBUYBILLREC *pstBuyBillRec,unsigned char ucBillNo)
// {
//  unsigned char ucRet = RET_ERROR;
//  if((pstBuyBillRec != NULL) && (ucBillNo < CON_BUYBILL_NUM))
//  {
//      stTerRunPara.stSwitchCtrlPara.stBuyBillRec[ucBillNo] = *pstBuyBillRec;
//      ucRet = RET_SUCCESS;
//  }
//  return ucRet;
// }
//azh 140313
//������������棬�ѵ���������ݽṹ�������浽EEPROM ���Ŀռ����ƫС ��Ϊ���������ݽṹ
void  Save_MP_ParaEE(unsigned char ucMeterNo, STMETERPARA *pstMeterPara)
{
	unsigned short len;
	
	if(ucMeterNo < CON_METER_NUM)
	{
		len = sizeof(STMETERPARA) * ucMeterNo;
		file_write(F10_METERPARA_ADDR+len, (U8*)pstMeterPara, sizeof(STMETERPARA));
	}	
}
//�ϵ��ʼ����������
void  Init_MP_ParaEE(unsigned char ucClrFlag)
{
	unsigned short len;
	STMETERPARA stMeterPara;
  unsigned char ucMeterNo;
	
	for(ucMeterNo=0; ucMeterNo < CON_METER_NUM; ucMeterNo++)
	{
		len = sizeof(STMETERPARA) * ucMeterNo;
		if(ucClrFlag == CON_PARA_INIT_POW_ON)
		{	
			file_read(F10_METERPARA_ADDR+len, (U8*)&stMeterPara, sizeof(STMETERPARA));
			RunPara_SetMeterPara(ucMeterNo,&stMeterPara);			
		}
		else
		{
			RunPara_InitMeterPara(&stTerRunPara.stMeterPara[ucMeterNo]);
			file_write(F10_METERPARA_ADDR+len, (U8*)&stTerRunPara.stMeterPara[ucMeterNo], sizeof(STMETERPARA));//azh 140919
		}	
		stTerRunPara.stMeterPara[ucMeterNo].ucFeeNum = 4;//140919
	}
		
}
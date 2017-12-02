/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : MC55Comm.c
 Description    :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2011-09-23      azh           the original version
******************************************************************************/
#include "..\inc\global_config.h"

//#include "..\include_public\include_files.h"
//#include "..\Main\Main_includefile.h"
#include "MC55Comm.h"
#include "MC55_MODULE.H"

//超级主站手机号码 用于收到短信后判断是否接收并执行
//主站1电话号码8613857113133
//主站2电话号码8613325818556
//主站2电话号码8613325818556
const char HUACAI_SUPER_SMS_NUM[3][8]={
                                        {0x86, 0x13, 0x85, 0x71, 0x13, 0x13, 0x3f, 0xff},
                                        {0x86, 0x13, 0x32, 0x58, 0x18, 0x55, 0x6f, 0xff},
                                        {0x86, 0x13, 0x58, 0x83, 0x14, 0x99, 0x8f, 0xff},
                                      };

static void         TC35_Clear_UpList(void);
//static unsigned char   TC35_GetUpSendTask(STTC35TASK  *pstTC35Task);
//Gprs数据处理
static unsigned char RComm_Gprs_RecvDataProc(unsigned char *pucRCommBuffer,unsigned short usDataContentLen,unsigned char ucDevNo);
static unsigned char RCommData_To_GprsBuf(STRCOMMRECEDATA *pstGprsDataBuf, unsigned char *pucBuffer,unsigned short usDataContentLen );
static unsigned char GprsBuf_To_ResponseBuf(unsigned char *pResponseBuf,STRCOMMRECEDATA *pstGprsDataBuf, unsigned short *pusRecvDataLen );//移到Gprs数据处理响应缓存
static unsigned char Revise_GprsBuf(STRCOMMRECEDATA *pstGprsDataBuf,unsigned short usDataProcPos);              //修复stGprsDataBuf

//短信数据处理
//static unsigned char TC35_Clear_SMSInfo(void);
//static unsigned char SMS_Clear_UniteInfo(STSMSUNITEINFO *pstSMSUniteInfo);
//static unsigned char ClearSMSBuf(STSMSBUFF *pstSMSBuf);
static unsigned char RComm_SMS_RecvDataProc(unsigned char *pucRCommBuffer,unsigned short usDataContentLen);
//static unsigned char RComm_SMS_UniteProc(unsigned char *pucRCommBuffer,unsigned short usDataContentLen);
static unsigned char RComm_SMSData_Proc(unsigned char *pucSMSBuffer,unsigned short usDataContentLen);
//unsigned char Rcomm_Fill_RecvData(unsigned char ucType , unsigned char* pData , unsigned short usLength , PUN_CHANNEL_ADDR pCommunInfo);
/***********************************************************************************************************/
//extern void DbgCommFillMonInfo(unsigned char *pBuf,unsigned short usLen);

//初始化函数
//void RComm_Initial(unsigned long ulBoadrate)
void RComm_Initial(void)
{
    TC35_Clear_UpList();
    TC35_ClearTask(&stTC35TaskTmp);
//    TC35_Clear_SMSInfo();

//  memset(TC35Data_ResponseBuf,0,CON_TC35COMM_RECV_DATA_LEN);
    memset(&stGprsDataBuf,0,sizeof(STRCOMMRECEDATA));
    usRCommModResetTimer=0xFFFF;
    ucRCommModResetFlag=CON_STATUS_NO;

    Reload_Module_RunState(&stModuleRunState);

    //
    Main_RComm_Set_OverFlux_Logout(0XFF);
}

void TC35_Clear_UpList(void)
{
    unsigned char i=0;
    stTC35UpList.ucList_Tail_P = 0;
    stTC35UpList.ucList_Head_P = 0;
    for(i=0;i<CONTC35UPLISTNUM;i++)
        TC35_ClearTask(&(stTC35UpList.stTC35Task[i]));
}
PSTTC35TASK ATGetDataContentPtr(unsigned char ucOffset)
{
    PSTTC35TASK pContent = NULL;
    unsigned char           ucLen = 0;

    ucLen = ATGetDataListLength();

    if(ucLen == 0 || ucOffset>=ucLen)
        return pContent;

    ucOffset += stTC35UpList.ucList_Tail_P;
    if(ucOffset >= CONTC35UPLISTNUM)
        ucOffset -= CONTC35UPLISTNUM;

    pContent = &(stTC35UpList.stTC35Task[ucOffset]);

    return pContent;
}
unsigned char ATGetDataListLength(void)
{
    unsigned char ucLen = 0;

    if(stTC35UpList.ucList_Head_P >= stTC35UpList.ucList_Tail_P)
        ucLen = stTC35UpList.ucList_Head_P - stTC35UpList.ucList_Tail_P;
    else
        ucLen = stTC35UpList.ucList_Tail_P + CONTC35UPLISTNUM - stTC35UpList.ucList_Head_P;

    return ucLen;
}
unsigned char ATDataListPop(void)
{
    if( stTC35UpList.ucList_Head_P == stTC35UpList.ucList_Tail_P)
    {
        return 0;
    }
    else
    {
        stTC35UpList.ucList_Tail_P++;
        if( stTC35UpList.ucList_Tail_P >= CONTC35UPLISTNUM)
            stTC35UpList.ucList_Tail_P = 0;
        return 1;
    }
}

//监控模块状态，如不上线等进行复位
unsigned char MC55_Comm_Timer(void)
{
    unsigned char ucRet = RET_SUCCESS;
//  STTERMISCPARA stTerMiscPara = RunPara_GetTerMiscPara();

    if(ucRCommModResetFlag!=CON_STATUS_YES)
    {
        if((stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat==CON_STATUS_YES)||(stModuleComParam.ucGPRSServer==1))
        {
            if(usRCommModResetTimer!=0xFFFF)
                usRCommModResetTimer=0xFFFF;
        }
        else//离线
        {
            if( (stTerRunPara.stTerMiscPara.ucIPInfoType[0]==CON_TC35COMMTYPE_INVALID) &&
                (stTerRunPara.stTerMiscPara.ucIPInfoType[1]==CON_TC35COMMTYPE_INVALID) )
            {}
            else//非短信
            {
                if(stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat==CON_STATUS_NO && usRCommModResetTimer>65000)
                    usRCommModResetTimer=6500;////18000;        //持续180秒未登录，将复位手机模块
                if(usRCommModResetTimer<=11 && ucRCommModResetFlag==CON_STATUS_NO)//unit 100ms
                    ucRCommModResetFlag=CON_STATUS_YES;
            }
        }
    }
    if(usRCommModResetTimer==0 && ucRCommModResetFlag!=CON_STATUS_NO)
    {
        ucReset_Mc55_Flag = 1;
        Main_RComm_UpStart();
        MC55_Init();
        usRCommModResetTimer=0xFFFF;
        ucRCommModResetFlag=CON_STATUS_NO;
        stTerRunPara.stTerMiscPara.stRCommRunInfo.ucRCommStat=CON_STATUS_NO;
        stTerRunPara.stTerMiscPara.stRCommRunInfo.ucRCommStatBak1=CON_STATUS_NO;
        stTerRunPara.stTerMiscPara.stRCommRunInfo.ucRCommStatBak2=CON_STATUS_NO;
        stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat=CON_STATUS_NO;
        RunPara_SetTerMiscPara(&stTerRunPara.stTerMiscPara);
//      TerPara_Set(CON_STATUS_YES,(1<<CON_TERPARA_BLOCKNUM_MISCPARA),0);
    }
    return ucRet;
}
unsigned char RComm_FillUpData(unsigned char *pBuf,unsigned short usLen,unsigned char ucMsgSource)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char ucChanNo=CON_RCOMMCHAN_1;
    unsigned char ucGPRSType = RunPara_GetTerCommPara_RCommType();
/*
    if (ucGPRSType == CON_ONLINE_CLIENT)
    {
        ucChanNo=CON_RCOMMCHAN_1;
    }
    else if (ucGPRSType == CON_ONLINE_SERVER)
    {
        ucChanNo=CON_RCOMMCHAN_1;
    }
    else    //混合模式
    {
        if( ucMsgSource == CON_TERMSG_SOUR_RCOMM_UDP )          //主动上送
        {
            ucChanNo=CON_RCOMMCHAN_2;
        }
        else
            ucChanNo=CON_RCOMMCHAN_1;
    }
 */
    if( ucMsgSource != CON_TERMSG_SOUR_RCOMM_SMS )
    {
        if(RunPara_GetTerMisc_DefaultIPType()==CON_RCOMMTYPE_UDP)
        {
            if( ucMsgSource == CON_TERMSG_SOUR_RCOMM_UDP )          //UDP来源
                ucChanNo=CON_RCOMMCHAN_1;
            else
                ucChanNo=CON_RCOMMCHAN_2;
        }
        else
        {//默认第一通道为TCP类型
            if( ucMsgSource == CON_TERMSG_SOUR_RCOMM_UDP )          //UDP来源
                ucChanNo=CON_RCOMMCHAN_2;
            else
                ucChanNo=CON_RCOMMCHAN_1;
        }
    }
    else
    {
        ucChanNo=CON_RCOMMCHAN_3;
    }

    if((stTC35UpList.ucList_Head_P +1 == stTC35UpList.ucList_Tail_P ) ||
       ((stTC35UpList.ucList_Head_P +1 == CONTC35UPLISTNUM) && (!stTC35UpList.ucList_Tail_P) ))
    {
        ucRet = RET_ERROR;                  ////缓冲区满
    }else
    {
        TC35_ClearTask(&(stTC35UpList.stTC35Task[stTC35UpList.ucList_Head_P]));
        stTC35UpList.stTC35Task[stTC35UpList.ucList_Head_P].ucChNo= ucChanNo;
        if( (ucChanNo!=CON_RCOMMCHAN_1) &&(ucChanNo!=CON_RCOMMCHAN_2) &&(ucChanNo!=CON_RCOMMCHAN_3))
            ucRet=RET_ERROR;
        if(ucRet==RET_SUCCESS && usLen < CON_AT_MAX_SEND_PACKAGE)
        {
            stTC35UpList.stTC35Task[stTC35UpList.ucList_Head_P].unCurAddrInfo = stModuleComParam.stCh[ucChanNo];
            stTC35UpList.stTC35Task[stTC35UpList.ucList_Head_P].ucDataLength = usLen;
            memcpy(stTC35UpList.stTC35Task[stTC35UpList.ucList_Head_P].pData ,pBuf,usLen);
            stTC35UpList.ucList_Head_P ++;
            if(stTC35UpList.ucList_Head_P >= CONTC35UPLISTNUM )
                stTC35UpList.ucList_Head_P  = 0;
        }else
            ucRet=RET_ERROR;
    }

    return ucRet;
}
//unsigned char TC35_GetUpData(unsigned char *pucBuffer,unsigned short *usLen )
//{
//    unsigned char ucRet = RET_ERROR;
///*  unsigned char ucTailNo=stTC35UpList.ucList_Tail_P;
//    unsigned char ucHeadNo=stTC35UpList.ucList_Head_P;
//    STTC35TASK stTC35TaskTmp;
//    while(ucTailNo != ucHeadNo  )
//    {
//        TC35_TaskCpy(&stTC35TaskTmp,&(stTC35UpList.stTC35Task[ucTailNo]));
//        if(stTC35TaskTmp.ucReportType == CON_TC35COMM_TYPE_DATA)
//        {
//            memcpy(pucBuffer,stTC35TaskTmp.ucDataArea,stTC35TaskTmp.usDataLen);
//            *usLen = stTC35TaskTmp.usDataLen;
//            stTC35UpList.ucList_Tail_P ++;
//            if(stTC35UpList.ucList_Tail_P >= CONTC35UPLISTNUM)
//                stTC35UpList.ucList_Tail_P = 0;
//            ucRet = RET_SUCCESS;
//            break;
//        }
//        ucTailNo ++;
//        if(ucTailNo >=CONTC35UPLISTNUM)
//            ucTailNo = 0;
//    }*/
//    return ucRet;
//
//}

///////////////////////////////////////////////与Gprs相关的函数/////////////////////////////////////////////
unsigned char RComm_Gprs_RecvDataProc(unsigned char *pucRCommBuffer,unsigned short usDataContentLen,unsigned char ucDevNo)
{
    unsigned char   mem_type;
    unsigned char   ucRet=RET_SUCCESS;
    unsigned char   ucWhileCount=0;
    unsigned short  usRecvDataLen=0,usDataProcPos=0,datalen=0;//usRetPos=0;
    unsigned char  *pucBuffer = NULL, *mem_ptr = NULL;
    unsigned char  ucSource=CON_TERMSG_SOUR_RCOMM_TCP;
//  unsigned char   TC35Data_ResponseBuf[CON_TC35COMM_RECV_DATA_LEN];

//azh

    while(1)
    {
        //mem_ptr = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR0);
        mem_ptr = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR0);
        if(mem_ptr != NULL)
        {
            break;
        }
        else
        {
//          memcpy(gusCurMemErrFunName, "RComm_Gp", 8);
            setmemgetnullid(GET_MEM_ERR0, gusCurMemErrFunName);
        }
        os_dly_wait(2);
    }
        pucBuffer = mem_ptr;//azh 由于pucBuffer指针会改动 所以保留后面还内在块的指针

//  pucBuffer=(unsigned char *)TC35Data_ResponseBuf;
//  memset(TC35Data_ResponseBuf,0,CON_TC35COMM_RECV_DATA_LEN);

    RCommData_To_GprsBuf(&stGprsDataBuf,pucRCommBuffer,usDataContentLen);                       //移到stGprsDataBuf
    GprsBuf_To_ResponseBuf( (unsigned char *)pucBuffer,&stGprsDataBuf,&usRecvDataLen);  //移到数据处理响应缓存

    while(usDataProcPos < usRecvDataLen) //处理数据
    {
        ucRet = Main_Protocol_CheckFrame(pucBuffer,(unsigned short)(usRecvDataLen - usDataProcPos));
        if( ucRet == RET_SUCCESS )
        {
            //if(pucBuffer[12]!=0x00)
                Main_RComm_SetStandBy_Time();
            if(RunPara_GetTerMisc_DefaultIPType()==CON_RCOMMTYPE_UDP)
            {
                if( ucDevNo == CON_RCOMMCHAN_2 )            //第二通道
                    ucSource=CON_TERMSG_SOUR_RCOMM_TCP;
                else
                    ucSource=CON_TERMSG_SOUR_RCOMM_UDP;
            }
            else
            {//默认第一通道为TCP类型
                if( ucDevNo == CON_RCOMMCHAN_2 )            //第二通道
                    ucSource=CON_TERMSG_SOUR_RCOMM_UDP;
                else
                    ucSource=CON_TERMSG_SOUR_RCOMM_TCP;
            }
            datalen = (pucBuffer[1]>>2)+pucBuffer[2]*0x40+8;//120911 国网
            Main_Recv_Proc( pucBuffer, datalen ,ucSource);
//          PIN_LEDR_SET;       //09-12-22
            DATALED_SET;//
            DataProc_Stat_FluxBytes_Inc(datalen);           //接收流量统计
            usDataProcPos += datalen;
            pucBuffer   += datalen;
            //azh debug test 下面的测试用处理
/*
            DataProc_Stat_FluxBytes_Inc(usRecvDataLen);         //接收流量统计
            usDataProcPos += usRecvDataLen;
            pucBuffer += usRecvDataLen;
*/
            ucWhileCount++;
            if(ucWhileCount >=150)
                break;

        }
        else if( (ucRet==RET_HEAD_ERROR)||(ucRet==RET_ERROR) )
        {
            usDataProcPos++;
            pucBuffer++;
        }
        else
            break;
    }
    Revise_GprsBuf(&stGprsDataBuf,usDataProcPos);

    free_mem_block(mem_ptr);//Mem_1536_Byte

    return ucRet;
}
unsigned char Revise_GprsBuf(STRCOMMRECEDATA *pstGprsDataBuf,unsigned short usDataProcPos)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned short usTotal;
    if(pstGprsDataBuf!=NULL)
    {
        usTotal = pstGprsDataBuf->usTail_P+usDataProcPos;
        pstGprsDataBuf->usTail_P = usTotal%CON_TC35COMM_RECV_DATA_LEN;
    }
    else
        ucRet=RET_ERROR;

    return ucRet;
}

//移到Gprs数据处理响应缓存
unsigned char GprsBuf_To_ResponseBuf(unsigned char *pResponseBuf,STRCOMMRECEDATA *pstGprsDataBuf, unsigned short *pusRecvDataLen )
{
    unsigned char   ucRet=RET_SUCCESS;
    unsigned short  usTailPos=0;
    unsigned short  usHeadPos=0;

    if(pstGprsDataBuf!=NULL)
    {
        usHeadPos=pstGprsDataBuf->usHead_P;
        usTailPos=pstGprsDataBuf->usTail_P;

        if((usTailPos>usHeadPos)&&(usTailPos<CON_TC35COMM_RECV_DATA_LEN)&&(usHeadPos<CON_TC35COMM_RECV_DATA_LEN))
        {
            memcpy(pResponseBuf,pstGprsDataBuf->ucDataArea+usTailPos,(CON_TC35COMM_RECV_DATA_LEN-usTailPos));
            memcpy(pResponseBuf+CON_TC35COMM_RECV_DATA_LEN-usTailPos,pstGprsDataBuf->ucDataArea,usHeadPos);
            (*pusRecvDataLen)=CON_TC35COMM_RECV_DATA_LEN-(usTailPos-usHeadPos);
        }else
        {
            if((usTailPos<=usHeadPos)&&(usTailPos<CON_TC35COMM_RECV_DATA_LEN)&&(usHeadPos<CON_TC35COMM_RECV_DATA_LEN))
            {
                memcpy(pResponseBuf,pstGprsDataBuf->ucDataArea+usTailPos,usHeadPos-usTailPos);
                (*pusRecvDataLen)=(usHeadPos-usTailPos);
            }else
            {
                (*pusRecvDataLen)=0;
            }
        }
    }
    else
        ucRet=RET_ERROR;

    return ucRet;
}

//移到Gprs接收缓存
unsigned char RCommData_To_GprsBuf(STRCOMMRECEDATA *pstGprsDataBuf, unsigned char *pucBuffer,unsigned short usDataContentLen )
{
    unsigned char   ucRet=RET_SUCCESS;
    unsigned short  j=0;

    if(pstGprsDataBuf!=NULL)
    {
        for(j=0;j<usDataContentLen;j++)
        {
            pstGprsDataBuf->ucDataArea[pstGprsDataBuf->usHead_P]=pucBuffer[j];
            pstGprsDataBuf->usHead_P++;
            if(pstGprsDataBuf->usHead_P>=CON_TC35COMM_RECV_DATA_LEN)
                pstGprsDataBuf->usHead_P=0;
        }
    }
    else
        ucRet=RET_ERROR;

    return ucRet;
}

///////////////////////////////////////////////与SMS相关的函数/////////////////////////////////////////////
unsigned char RComm_SMS_RecvDataProc(unsigned char *pucRCommBuffer,unsigned short usDataContentLen)
{
    unsigned char   ucRet=RET_SUCCESS;

//南网浙规
    RComm_SMSData_Proc(pucRCommBuffer,usDataContentLen);
//国网
/*
    if(pucRCommBuffer[1]==0x80)//单帧处理
    {
        usLateSmsSeq=pucRCommBuffer[0]+0x100*(pucRCommBuffer[1]&0x0F);
        RComm_SMSData_Proc(pucRCommBuffer,usDataContentLen);
    }
    else//多帧处理
    {
        RComm_SMS_UniteProc( pucRCommBuffer, usDataContentLen);
    }
*/
    return ucRet;
}
unsigned char RComm_SMSData_Proc(unsigned char *pucSMSBuffer,unsigned short usDataContentLen)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char   ucWhileCount=0;
    unsigned short  usDataProcPos=0,datalen=0;//,usRetPos=0;

    while(usDataProcPos<usDataContentLen)
    {
        ucRet = Main_Protocol_CheckFrame(pucSMSBuffer,(unsigned short)(usDataContentLen - usDataProcPos));
        if( ucRet == RET_SUCCESS )
        {
//南网的
//            datalen = pucSMSBuffer[9]+(pucSMSBuffer[10]<<8)+13;
//            Main_Recv_Proc( pucSMSBuffer, datalen ,CON_TERMSG_SOUR_RCOMM_SMS);
//            usDataProcPos += datalen;
//            pucSMSBuffer += datalen;
//国网的
//          Main_Recv_Proc( pucSMSBuffer, (unsigned short)((pucSMSBuffer[1]>>2)+pucSMSBuffer[2]*0x40+8) ,CON_TERMSG_SOUR_RCOMM_SMS);
//            usDataProcPos += (pucSMSBuffer[1]>>2)+pucSMSBuffer[2]*0x40+8;
//            pucSMSBuffer    += (pucSMSBuffer[1]>>2)+pucSMSBuffer[2]*0x40+8;
            datalen = (pucSMSBuffer[1]>>2)+pucSMSBuffer[2]*0x40+8;//120911 国网
            Main_Recv_Proc( pucSMSBuffer, datalen ,CON_TERMSG_SOUR_RCOMM_SMS);
            usDataProcPos += datalen;
            pucSMSBuffer += datalen;

            ucWhileCount++;
            if(ucWhileCount >=150)
                break;
        }
        else if( (ucRet==RET_HEAD_ERROR)||(ucRet==RET_ERROR) )
        {
            usDataProcPos++;
            pucSMSBuffer++;
        }
        else
            break;
    }
    return ucRet;
}
/*
unsigned char RComm_SMS_UniteProc(unsigned char *pucRCommBuffer,unsigned short usDataContentLen)
{
    unsigned char ucRet=RET_SUCCESS;
    unsigned char i=0,j=0,h=0;
    unsigned short usSmsSeq=0,usSmlSeq=0,usSmsLen=0,usSmsSeqDiff=0;

    usSmsSeq=pucRCommBuffer[0]+0x100*(pucRCommBuffer[1]&0x0F);
    usSmlSeq=(pucRCommBuffer[1]&0x70)>>4;

    if(usSmsSeq>=usLateSmsSeq)              //过零时会有问题,此种情况是否可以不考虑
        usSmsSeqDiff=usSmsSeq-usLateSmsSeq;
    else
        usSmsSeqDiff=usLateSmsSeq-usSmsSeq;

    //判是否是新的SmsSeq
    for(i=0;i<CON_SMSUNITE_NUM;i++)
    {
        if(usSmsSeq==stSMSUniteInfo[i].usSmsSeq)
            break;
    }
    if(i>=CON_SMSUNITE_NUM) //是，新增的
    {
        if( (usSmlSeq<CON_SMS_NUM)&&(usDataContentLen-2<CON_SMSBUFF_MAXLEN)&&(usSmsSeqDiff<CON_SMSUNITE_NUM) )
        {
            usLateSmsSeq=usSmlSeq;
            //先移位后填充
            for(j=0;j<CON_SMSUNITE_NUM;j++)
                stSMSUniteInfo[CON_SMSUNITE_NUM-1-j]=stSMSUniteInfo[CON_SMSUNITE_NUM-2-j];

            stSMSUniteInfo[0].stDateTime_Recvstart=GetCurrentTime();
            stSMSUniteInfo[0].usSmsSeq=usSmsSeq;
            stSMSUniteInfo[0].usSmlSeq_RecvNum++;
            if( (pucRCommBuffer[1]&0x80)==0x80 )
                stSMSUniteInfo[0].usSmlSeq_End=usSmlSeq;

            stSMSUniteInfo[0].stSMSBuf[usSmlSeq].usSmlSeq=usSmlSeq;
            stSMSUniteInfo[0].stSMSBuf[usSmlSeq].ucDataLen=usDataContentLen-2;
            memcpy(stSMSUniteInfo[0].stSMSBuf[usSmlSeq].ucBuff,pucRCommBuffer+2,usDataContentLen-2);
        }
    }
    else    //已存在但未处理
    {
        //判是否是重发
        for(j=0;j<CON_SMS_NUM;i++)
        {
            if(stSMSUniteInfo[i].stSMSBuf[j].usSmlSeq==usSmlSeq)
                break;
        }
        if(j>=CON_SMS_NUM)
        {
            if( (usSmlSeq<CON_SMS_NUM)&&(usDataContentLen-2<CON_SMSBUFF_MAXLEN) )
            {
                stSMSUniteInfo[i].usSmlSeq_RecvNum++;
                if( (pucRCommBuffer[1]&0x80)==0x80 )
                    stSMSUniteInfo[i].usSmlSeq_End=usSmlSeq;

                stSMSUniteInfo[i].stSMSBuf[usSmlSeq].usSmlSeq=usSmlSeq;
                stSMSUniteInfo[i].stSMSBuf[usSmlSeq].ucDataLen=usDataContentLen-2;
                memcpy(stSMSUniteInfo[i].stSMSBuf[usSmlSeq].ucBuff,pucRCommBuffer+2,usDataContentLen-2);
            }
        }
        else    //重发帧不管
        {}
    }
    //检查合并
    for(i=0;i<CON_SMSUNITE_NUM;i++)
    {
        if( (stSMSUniteInfo[i].usSmlSeq_End+1==stSMSUniteInfo[i].usSmlSeq_RecvNum) &&
            (stSMSUniteInfo[i].usSmlSeq_End!=0) &&
            (stSMSUniteInfo[i].usSmlSeq_RecvNum!=0) )//合并处理
        {
            for(j=0;j<=stSMSUniteInfo[i].usSmlSeq_End;j++)
            {
                for(h=0;h<stSMSUniteInfo[i].stSMSBuf[j].ucDataLen;h++)
                {
                    ucSMSResponseBuff[usSmsLen++]=stSMSUniteInfo[i].stSMSBuf[j].ucBuff[h];
                }
            }
            //
            RComm_SMSData_Proc(ucSMSResponseBuff,usSmsLen);
            //初始化数据
            SMS_Clear_UniteInfo( &(stSMSUniteInfo[i]) );
        }
    }
    return ucRet;
}
*/
//unsigned char TC35_Clear_SMSInfo(void)
//{
//    unsigned char ucRet=RET_SUCCESS;
//    unsigned char i=0;
//
//    usLateSmsSeq=0; //此处若收到第一条是需要合并短信会有问题
//    memset(ucSMSResponseBuff,0,CON_SMSBUFF_RESULTLEN);
//
//    for(i=0;i<CON_SMSUNITE_NUM;i++)
//        SMS_Clear_UniteInfo( &(stSMSUniteInfo[i]) );
//
//    return ucRet;
//
//}
//unsigned char SMS_Clear_UniteInfo(STSMSUNITEINFO *pstSMSUniteInfo)
//{
//    unsigned char ucRet=RET_SUCCESS;
//    unsigned char i=0;
//
//    if(pstSMSUniteInfo!=NULL)
//    {
//        ClearDateTime( &(pstSMSUniteInfo->stDateTime_Recvstart) );
//        pstSMSUniteInfo->usSmsSeq=0;
//        pstSMSUniteInfo->usSmlSeq_End=0;
//        pstSMSUniteInfo->usSmlSeq_RecvNum=0;
//        for(i=0;i<CON_SMS_NUM;i++)
//            ClearSMSBuf( &(pstSMSUniteInfo->stSMSBuf[i]) );
//    }
//    else
//        ucRet=RET_ERROR;
//
//    return ucRet;
//}
//unsigned char ClearSMSBuf(STSMSBUFF *pstSMSBuf)
//{
//    unsigned char ucRet=RET_SUCCESS;
//
//    if(pstSMSBuf!=NULL)
//        memset(pstSMSBuf,0,sizeof(STSMSBUFF));
//    else
//        ucRet=RET_ERROR;
//
//    return ucRet;
//}

void TC35_ClearTask(PSTTC35TASK pstTC35Task)
{
    if(pstTC35Task != NULL)
    {
        memset((unsigned char *)pstTC35Task,0,sizeof(STTC35TASK));
    }
}
void Reload_SCA_Para(void)
{
    unsigned char ucSCALen=0;//,i;

//	STSTAPHONENO stStaPhoneNo = RunPara_GetStaPhoneNo();
//  STSTAIPADDR stStaIPAddr = RunPara_GetStaIPAddr();

	ucSCALen = Get_PhoneNum_Str(stTerRunPara.stStaPhoneNo.stSMSCenter.ucPhoneNo,8,stModuleComParam.stCommonParam.SCA);

    stModuleComParam.stCommonParam.ucSCALen = ucSCALen;
    stModuleComParam.stCh[2].ucType = CON_RCOMMTYPE_SMS;
    stModuleComParam.stCh[2].unChInfo.stSMSParam.ucLen
        = Get_PhoneNum_Str(stTerRunPara.stStaPhoneNo.stPhoneNo.ucPhoneNo,8,stModuleComParam.stCh[2].unChInfo.stSMSParam.ucCode);

}
void Reload_Chinfo_Para(void)
{
    unsigned char i,j,ucPos= 0;
    unsigned char flag=0;//azh
    STIPADDR stIPAddr;
//    STSTAIPADDR stStaIPAddr;
//  STTERMISCPARA stTerMiscPara=RunPara_GetTerMiscPara();
    unsigned char ucGPRSType = RunPara_GetTerCommPara_RCommType();


    if( (stTerRunPara.stTerMiscPara.ucIPInfoType[0]==CON_TC35COMMTYPE_INVALID) &&
        (stTerRunPara.stTerMiscPara.ucIPInfoType[1]==CON_TC35COMMTYPE_INVALID) )
    {
        //memset((unsigned char *)&(stStaIPAddr.stBaseIP),0,6);           //短信方式，第一通道为无效ip地址
        //memset((unsigned char *)&(stStaIPAddr.stBackIP),0,6);           //短信方式，第一通道为无效ip地址
        flag = 1;
    }
    for(i=0;i<2;i++)
    {
        if(stTerRunPara.stTerMiscPara.ucIPInfoType[i] == CON_TC35COMMTYPE_UDP)
            stModuleComParam.stCh[i].ucType = CON_RCOMMTYPE_UDP;
        else
            stModuleComParam.stCh[i].ucType = CON_RCOMMTYPE_TCP;
        if(ucIPFrist == 0)
        {
            if(i == 0)
                stIPAddr = stTerRunPara.stStaIPAddr.stBaseIP;
            else
                stIPAddr = stTerRunPara.stStaIPAddr.stBackIP;
        }
        else
        {
            if(i == 0)
                stIPAddr = stTerRunPara.stStaIPAddr.stBackIP;
            else
                stIPAddr = stTerRunPara.stStaIPAddr.stBaseIP;
        }
        if(flag)//azh
        {
            memset((unsigned char *)&(stIPAddr),0,6);
        }
        ucPos = 0;
        for(j=0;j<4;j++)
        {
            stModuleComParam.stCh[i].unChInfo.stIPParam.ucIP[ucPos++] = (stIPAddr.ucIPAddr[j]/100)+'0';
            stIPAddr.ucIPAddr[j]%=100;
            stModuleComParam.stCh[i].unChInfo.stIPParam.ucIP[ucPos++] = (stIPAddr.ucIPAddr[j]/10)+'0';
            stModuleComParam.stCh[i].unChInfo.stIPParam.ucIP[ucPos++] = (stIPAddr.ucIPAddr[j]%10)+'0';
            if(j<3)
                stModuleComParam.stCh[i].unChInfo.stIPParam.ucIP[ucPos++] = '.';
        }
        ucPos = 0;
        stModuleComParam.stCh[i].unChInfo.stIPParam.ucPort[ucPos++] = (stIPAddr.usIPPort/10000)+'0';
        stIPAddr.usIPPort %= 10000;
        stModuleComParam.stCh[i].unChInfo.stIPParam.ucPort[ucPos++] = (stIPAddr.usIPPort/1000)+'0';
        stIPAddr.usIPPort %= 1000;
        stModuleComParam.stCh[i].unChInfo.stIPParam.ucPort[ucPos++] = (stIPAddr.usIPPort/100)+'0';
        stIPAddr.usIPPort %= 100;
        stModuleComParam.stCh[i].unChInfo.stIPParam.ucPort[ucPos++] = (stIPAddr.usIPPort/10)+'0';
        stModuleComParam.stCh[i].unChInfo.stIPParam.ucPort[ucPos++]  = (stIPAddr.usIPPort%10)+'0';

    }

    for(i=0;i<CON_MAXAPNLEN;i++)
    {
        //if(stTerRunPara.stStaIPAddr.cAPN[i]==0x00)
        if(stTerRunPara.stStaIPAddr.cAPN[i]==0x00)
            break;
    }
    stModuleComParam.stCommonParam.ucAPNLen = i;
    memcpy(stModuleComParam.stCommonParam.cAPN,stTerRunPara.stStaIPAddr.cAPN,i);
//azh 没有服务器模式
//    if((ucGPRSType == CON_ONLINE_MIX) || (ucGPRSType == CON_ONLINE_SERVER))
//        stModuleComParam.ucGPRSServer = 1;
//    else
        stModuleComParam.ucGPRSServer = 0;
//  LCD_Set_HintSeqno(CON_LCDHINT_STARTLOG);
}
void Reload_PINPUK_Para(void)
{
    //STTERMISCPARA stTerMiscPara=RunPara_GetTerMiscPara();
    if(stTerRunPara.stTerMiscPara.usUsePin == CON_STATUS_YES_2BYTE)
        stModuleComParam.stPINPUK.ucIsPwdValid = 0x01;
    else
        stModuleComParam.stPINPUK.ucIsPwdValid = 0;
    memcpy(stModuleComParam.stPINPUK.ucPinPwd,stTerRunPara.stTerMiscPara.ucPinNew,4);
    memcpy(stModuleComParam.stPINPUK.ucPinPwdOld,stTerRunPara.stTerMiscPara.ucPinOld,4);
    memcpy(stModuleComParam.stPINPUK.ucPukPwd,stTerRunPara.stTerMiscPara.ucPuk,4);
    if( ucPinSetFlag == CON_STATUS_YES)
    {
        memcpy(stTerRunPara.stTerMiscPara.ucPinOld,stTerRunPara.stTerMiscPara.ucPinNew,4);
        RunPara_SetTerMiscPara(&stTerRunPara.stTerMiscPara);

        ucPinSetFlag = CON_STATUS_NO;

//      TerPara_Set(CON_STATUS_YES,(1<<CON_TERPARA_BLOCKNUM_MISCPARA),0);
    }
}
void Reload_PDP_Para(void)
{
    unsigned char i;
    //STTERMISCPARA stTerMiscPara=RunPara_GetTerMiscPara();

    stModuleComParam.stPDPAccount.ucIsAccountValid = stTerRunPara.stTerMiscPara.stRCommPara.stGprsPDPPara.ucPDPUseFlag;
    for(i=0;i<CON_MAX_GPRSPDPUSERNAME_LEN;i++)
    {
        if(stTerRunPara.stTerMiscPara.stRCommPara.stGprsPDPPara.ucPDPUserName[i]==0x00)
            break;
    }
    memcpy(stModuleComParam.stPDPAccount.ucUserName,stTerRunPara.stTerMiscPara.stRCommPara.stGprsPDPPara.ucPDPUserName,i);
    stModuleComParam.stPDPAccount.ucUserNameLen = i;
    for(i=0;i<CON_MAX_GPRAPDPPASSWORE_LEN;i++)
    {
        if(stTerRunPara.stTerMiscPara.stRCommPara.stGprsPDPPara.ucPDPPassWord[i]==0x00)
            break;
    }
    memcpy(stModuleComParam.stPDPAccount.ucPwd,stTerRunPara.stTerMiscPara.stRCommPara.stGprsPDPPara.ucPDPPassWord,i);
    stModuleComParam.stPDPAccount.ucPwdLen = i;
}
void Reload_ReLink_Para(void)
{
//  STTERMISCPARA stTerMiscPara=RunPara_GetTerMiscPara();
    stModuleComParam.usTimerParam = stTerRunPara.stTerMiscPara.stRCommPara.stGprsRelinkTime.usRelinkTime;
}
void Reload_Module_RunState(ST_MODULE_RUN_STATE *pstModule_Run_State)
{
//  STTERMISCPARA stTerMiscPara=RunPara_GetTerMiscPara();
//  STREMOTERUNINFO stRemoteRunInfo = stTerRunPara.stTerMiscPara.stRCommRunInfo;


    if(pstModule_Run_State->bIsGPRSConnected != 0)
    {
        stTerRunPara.stTerMiscPara.stRCommRunInfo.ucCurrChanType = CON_TERMSG_SOUR_RCOMM_TCP;
        stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat=CON_STATUS_YES;
    }else
    {
        stTerRunPara.stTerMiscPara.stRCommRunInfo.ucCurrChanType = CON_TERMSG_SOUR_RCOMM_SMS;
        stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat=CON_STATUS_NO;
    }
    if((pstModule_Run_State->bIsPINLock !=0) ||(pstModule_Run_State->bIsPUKLock != 0))
        stTerRunPara.stTerMiscPara.stRCommRunInfo.ucPinStat  = CON_PIN_LOCK ;
    else
        stTerRunPara.stTerMiscPara.stRCommRunInfo.ucPinStat  = CON_PIN_UNLOCK ;
    if((pstModule_Run_State->bIsSIMExist == 0x01)&&(pstModule_Run_State->bIsExist == 0x01))
    {
        stTerRunPara.stTerMiscPara.stRCommRunInfo.ucRCommStatBak2 = stTerRunPara.stTerMiscPara.stRCommRunInfo.ucRCommStatBak1;
        stTerRunPara.stTerMiscPara.stRCommRunInfo.ucRCommStatBak1 = stTerRunPara.stTerMiscPara.stRCommRunInfo.ucRCommStat;
        stTerRunPara.stTerMiscPara.stRCommRunInfo.ucRCommStat = CON_STATUS_YES;
    }else
    {
        stTerRunPara.stTerMiscPara.stRCommRunInfo.ucRCommStatBak2 = stTerRunPara.stTerMiscPara.stRCommRunInfo.ucRCommStatBak1;
        stTerRunPara.stTerMiscPara.stRCommRunInfo.ucRCommStatBak1 = stTerRunPara.stTerMiscPara.stRCommRunInfo.ucRCommStat;
        stTerRunPara.stTerMiscPara.stRCommRunInfo.ucRCommStat = CON_STATUS_NO;
    }
    if(pstModule_Run_State->bIsRing == 1)
        Main_RComm_RingStart();
    stTerRunPara.stTerMiscPara.stRCommRunInfo.ucSignIntension = pstModule_Run_State->ucSignIntension;
    memcpy(stTerRunPara.stTerMiscPara.stRCommRunInfo.ucModHardVer,(unsigned char *)&(pstModule_Run_State->ucHardWareVer),4);            //硬件版本
    memcpy(stTerRunPara.stTerMiscPara.stRCommRunInfo.ucModSoftVer,(unsigned char *)&(pstModule_Run_State->ucSortWareVer),4);            //软件版本

    stTerRunPara.stTerMiscPara.stRCommRunInfo = stTerRunPara.stTerMiscPara.stRCommRunInfo;
//  RunPara_SetTerMiscPara(&stTerRunPara.stTerMiscPara);
}
//void RComm_PinSetFlag(void)
//{
//    ucPinSetFlag = CON_STATUS_YES;
//}
unsigned long ulTest = 0;
unsigned char Rcomm_Fill_RecvData(unsigned char ucType , unsigned char* pData , unsigned short usLength , PUN_CHANNEL_ADDR pCommunInfo)
{
    unsigned char ucRet = RET_SUCCESS;
    unsigned char   i=0,j=0;
    unsigned char ucChanNo = 0;
    ulTest += usLength;
    if( ucType == CMD_GPRSDATA_AUTO_UP || ucType == CMD_SMSDATA_AUTO_UP )
    {
        if(ucType == CMD_SMSDATA_AUTO_UP)
        {
            //返回通道这里需要查找匹配
            ucChanNo  = stModuleRunState.ucCurCommChannel;
            for(i=0;i<3;i++)
            {
                if( stModuleComParam.stCh[i].ucType == TYPE_COMUNNICATION_SMS )
                {
                    if( stModuleComParam.stCh[i].unChInfo.stSMSParam.ucLen >= pCommunInfo->stSMSParam.ucLen )
                    {
                        if(0 == memcmp( stModuleComParam.stCh[i].unChInfo.stSMSParam.ucCode ,
                                pCommunInfo->stSMSParam.ucCode,
                                pCommunInfo->stSMSParam.ucLen   ) )
                        {
                            ucChanNo = i;
                        }
                        else
                        {
                            for(j=0; j<3; j++)
                            {
                                if(0 == memcmp( HUACAI_SUPER_SMS_NUM[j] ,
                                    pCommunInfo->stSMSParam.ucCode,
                                    pCommunInfo->stSMSParam.ucLen   ) )
                                    {
                                       ucChanNo = i;
                                       break;
                                    }
                             }
                        }
                    }
                }
            }
        }
        else
        {
            for(i=0; i<3; i++)
            {
                if((0 == memcmp( &(stModuleComParam.stCh[i].unChInfo.stIPParam.ucPort) , pCommunInfo->stIPParam.ucPort, 5) ) &&
                  (0 == memcmp( &(stModuleComParam.stCh[i].unChInfo.stIPParam.ucIP) , pCommunInfo->stIPParam.ucIP, 15) ))
                {
                    ucChanNo = i;
                    break;
                }
                else
                    ucChanNo = stModuleRunState.ucCurCommChannel;
            }
        }
    }
    if(ucChanNo == CON_RCOMMCHAN_3)
        RComm_SMS_RecvDataProc(pData, usLength);
    else if((ucChanNo == CON_RCOMMCHAN_1)||(ucChanNo == CON_RCOMMCHAN_2))
        RComm_Gprs_RecvDataProc(pData, usLength,ucChanNo);

//  DbgCommFillMonInfo(pData,usLength);

    return ucRet;
}
void RComm_ResetStart(void)
{
    usRCommModResetTimer=10;//100;              //延时1秒
    ucRCommModResetFlag=CON_STATUS_YES;
}


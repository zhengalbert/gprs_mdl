/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : MC55_AT_MANAGE.c
 Description    : 主函数
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2011-09-23      azh           the original version
******************************************************************************/
#include "..\inc\global_config.h"

#include "MC55_AT_MANAGE.H"
#include "MC55_BufferManage.h"
#include "MC55_TASK_MANAGE.h"
#include "MC55_AT_TASK.H"
#include "MC55_SMS_Manage.h"
#include "MC55_MODULE.H"
#include "MC55Comm.h"
//#include "..\\Main\\Main_includefile.h"
//#include "string.h"
#include "stdio.h"
//#include "..\\Main\\RPara\\Run_Para_Export.h"
//#define DEBUG_TRACE_AT_CMD

#ifdef DEBUG_TRACE_AT_CMD
unsigned char at_cmd_sn=0;
#endif//DEBUG_TRACE_AT_CMD

unsigned char ucTmpStr[500];

//动态域名用于在没有固定IP地址的情况长时间测试 
//如果IP地址地址成:111.111.111.111则会启用zhejianghuacai.3322.org:PORT动态域名
const char HUACAI_DNS[]={"zhejianghuacai.3322.org"};
const   char STR_AT_SET_SISS_DNS_ADDRESS_TCP[]={"at^siss=0,address,socktcp://zhejianghuacai.3322.org:     \r\n"};
const   char HUACAI_DNS_FLAG[]={"111.111.111.111"};
//此外，能否将一些没有使用的命令以及相关处理命令去除
/***********************MC55命令****************************/

const char STR_AT_CMD_RETURN[]={"\r\n"};
const char STR_AT_CMD_RETURN2[]={"\x22\r\n"};           //
const char STR_AT_CMD_RETURN3[]={"\x22,0,0,1\r\n"};           //
const char STR_AT_CMD_AT[]={"AT\r\n"};
const char STR_AT_QUERY_CSQ[]={"AT+CSQ\r\n"};
const char STR_AT_SET_ATE0[]={"ATE0\r\n"};              //关回显
const char STR_AT_SET_FLO[]={"AT+FLO=0\r\n"};           //关流控 telit
const char STR_AT_SET_FRS[]={"AT&K0\r\n"};              //azh telit
const char STR_AT_SET_CNMI[]={"AT+CNMI=2,1,0,0,1\r\n"};     //短信提示
const char STR_AT_SET_CREG[]={"AT+CREG=1\r\n"};             //使能主动URC  CREG状态上送
const char STR_AT_QUERY_CREG[]={"AT+CREG?\r\n"};                //使能主动URC  CREG状态上送
const char STR_AT_SET_CHUP[]={"AT+CHUP\r\n"};               //挂断电话
const char STR_AT_QUERY_CSCA[]={"AT+CSCA?"};                //短信中心号码查询
const char STR_AT_SET_CSCA[]={"AT+CSCA="};                  //短信中心号码设置
const char STR_AT_SET_IPR[]={"AT+IPR=57600\r\n"};               //波特率设置为57600
//const char STR_AT_SET_IPR[]={"AT+IPR=38400\r\n"};             //波特率设置为38400  cfh 10-04-14
const char STR_AT_SET_CFUN[]={"AT+CFUN=0,1\r\n"};
const char STR_AT_SET_ATQ3[]={"AT\\Q3\r\n"};
const char STR_AT_SET_CMEE[]={"AT+CMEE=2\r\n"};
const char STR_AT_QUERY_GMR[]={"AT+GMR\r\n"};                   //查询软件版本信息
//密码相关指令
const   char STR_AT_QUERY_CPIN[]={"at+cpin?\r\n"};
const   char STR_AT_QUERY_SPIC[]={"AT+SPIC?\r\n"};
const   char STR_AT_SET_CLCK1[]={"AT+CLCK=\"SC\",1,\"    \"\r\n"};      //关闭PIN保护
const   char STR_AT_SET_CLCK0[]={"AT+CLCK=\"SC\",0,\"    \"\r\n"};      //开启PIN保护
const   char STR_AT_UNLOCK_PIN[]={"AT+CPIN=\"    \"\r\n"};              //PIN解锁,需要4位PIN码
const   char STR_AT_UNLOCK_PUK[]={"AT+CPIN=\"        \",\"    \"\r\n"}; //PUK解锁,需要8位PUK码,4位新的PIN码
const   char STR_AT_SET_CPWD[]={"AT+CPWD=\"SC\",\"    \",\"    \"\r\n"};//密码变更,需要4位旧PIN码,4位新的PIN码


//短信相关指令
const   char STR_AT_SET_CMGF[]={"AT+CMGF=0\r\n"};                       //设置短信PDU格式
const   char STR_AT_QUERY_CMGL[]={"AT+CMGL=4\r\n"};                     //列出所有短消息
const   char STR_AT_SET_CMGS[]={"AT+CMGS=   \r\n"};                     //发送短消息
const   char STR_AT_SET_CPMS[]={"AT+CPMS=\"MT\",\"MT\",\"MT\"\r\n"};    //短消息存储设置
const   char STR_AT_SET_CMGD[]={"AT+CMGD=  \r\n"};                      //删除短消息
const   char STR_AT_SET_SSMSS[]={"AT^SSMSS=1\r\n"};
const   char STR_AT_SET_SM20[]={"AT^SM20=0\r\n"};
const   char STR_AT_QUERY_CPMS[]={"AT+CPMS?\r\n"};
const   char STR_AT_QUERY_CNMI[]={"AT+CNMI?\r\n"};

//GPRS相关的
const   char STR_AT_SET_CGATT_DETACH[]={"at+cgatt=0\r\n"};
const   char STR_AT_SET_CGATT_TACH[]={"at+cgatt=1\r\n"};

//azh const     char STR_AT_SET_SCFG[]={"AT#SCFG=1,1,700,0,900,2\r\n"};     //配置通道1
//const     char STR_AT_SET_SCFG2[]={"AT#SCFG=2,1,700,0,900,2\r\n"};     //配置通道2
//azh const     char STR_AT_SET_SCFGEXT[]={"AT#SCFGEXT=1,1,0,0,0,1\r\n"};     //配置通道1
//const     char STR_AT_SET_SCFGEXT2[]={"AT#SCFGEXT=2,1,0,0,0,1\r\n"};     //配置通道2
//azh const     char STR_AT_SET_FRWL[]={"AT#FRWL=1,\"198.158.1.1\",\"0.0.0.0\"\r\n"};//允许接入IP   //azh telit

//MC55_GPRS
const   char STR_AT_QUERY_SICS[]={"at^sics?\r\n"};
const   char STR_AT_SET_SICS_GPRS0[]={"at^sics=0,contype,gprs0\r\n"};
const   char STR_AT_SET_SICS_APN[]={"at^sics=0,apn,cmnet\r\n"};
//
const   char STR_AT_SET_SICS_USER[]={"at^sics=0,user,                                \r\n"};
const   char STR_AT_SET_SICS_PASSWD[]={"at^sics=0,passwd,                \r\n"};
const   char STR_AT_SET_SISS_SRVTYPE_SOCKET[]={"at^siss=0,srvtype,socket\r\n"};

const   char STR_AT_SET_SISS_ALPHABETAT[]={"at^siss=1,alphabet,1\r\n"};
const   char STR_AT_SET_SISS_CONID[]={"at^siss=1,conid,0\r\n"};
const   char STR_AT_SET_SISO[]={"at^siso= \r\n"};
const   char STR_AT_SET_SISC[]={"at^sisc= \r\n"};
const   char STR_AT_SET_SGCONF[]={"AT^SGCONF=,8\r\n"};
const   char STR_AT_QUERY_SGCONF[]={"AT^SGCONF?\r\n"};

const   char STR_AT_SET_SISS_ADDRESS_TCP[]={"at^siss=0,address,socktcp://   .   .   .   :     \r\n"};
const   char STR_AT_SET_SISS_ADDRESS_UDP[]={"at^siss=1,address,sockudp://   .   .   .   :     \r\n"};
const   char STR_AT_SET_SISW[]={"AT^SISW= ,    \r\n"};  //写数据通道号后是字节数
const   char STR_AT_SET_SISR[]={"AT^SISR= ,700\r\n"};  //读时把数据全部读出
//MC55_GPRS

//TELIT_GPRS
const   char STR_AT_SET_SICS_APN_T[]={"at+cgdcont=1,\"ip\",\"cmnet\"\r\n"};//telit
const     char STR_AT_SET_SISS_ADDRESS_TCP_T[]={"AT#SD=2,0,     , \"   .   .   .   \"\r\n"};//telit
const     char STR_AT_SET_SISS_ADDRESS_UDP_T[]={"AT#SD=1,1,     , \"   .   .   .   \"\r\n"};//telit
const     char STR_AT_SET_SISW_T[]={"AT#SSEND=1\r\n"};  //写数据{"AT#SSEND=1\r\n"};  //写数据
const     char STR_AT_SET_SISR_T[]={"AT#SRECV=1,700\r\n"};  //读时把数据全部读出
//azh const char STR_AT_SET_SISR[]={"AT#SRECV=1,600\r\n"};  //读时把数据全部读出
const     char STR_AT_SET_CGATT_DESOC[]={"AT#SH=2\r\n"};  //azh telit
const     char STR_AT_SET_CGATT_OFFSERVER[]={"AT#SH=1\r\n"}; //azh telit
const     char STR_AT_SET_GPRS[]={"AT#GPRS=1\r\n"};     //激活链接
//const     char STR_AT_SET_SGACT[]={"AT#SGACT=1,1\r\n"};     //激活链接
//const     char STR_AT_SET_DESGACT[]={"AT#SGACT=1,0\r\n"};
const     char STR_AT_SET_SCFG[]={"AT#SCFG=1,1,1400,0,1200,0\r\n"};     //配置通道1
//const     char STR_AT_SET_SCF-G2[]={"AT#SCFG=2,1,1400,0,1200,0\r\n"};     //配置通道2
const     char STR_AT_SET_SCFGEXT[]={"AT#SCFGEXT=1,0,0,12,0,1\r\n"};     //配置通道1
const     char STR_AT_SET_SCFGEXT2[]={"AT#SCFGEXT=2,0,0,0,0,0\r\n"};     //配置通道2
const     char STR_AT_SET_SSENDEXT[]={"AT#SSENDEXT=1,10"};
const     char STR_AT_SET_FRWL[]={"AT#FRWL=1,\"183.129.224.206\",\"0.0.0.0\"\r\n"};//允许接入IP   //azh telit
const     char STR_AT_SET_SL[]={"AT#SL=1,1,1024,0\r\n"};//打开监听
const     char STR_AT_SET_SL2[]={",0\r\n"};
const     char STR_AT_SET_SA[]={"AT#SA=1,1\r\n"};//允许接人，命令模式//azh telit
//TELIT_GPRS
//模块关电
const   char STR_AT_SET_SMSO[]={"AT^SMSO\r\n"};         //关电指令,
//azh const char STR_AT_SET_SMSO[]={"AT#SHDN\r\n"};         //关电指令,

/*******************************************************/
void trace_at_cmd_sn(unsigned char id)
{
#ifdef DEBUG_TRACE_AT_CMD
    at_cmd_sn = id;  
#endif//DEBUG_TRACE_AT_CMD
}

#ifdef _MC55_DEBUG
//调试信息
const   char STR_DEBUG_DTR1[]={"DTR -> ONLINE_CMD\r\n"};
const   char STR_DEBUG_DTR2[]={"DTR -> UNKNOWN(GSM)\r\n"};
const   char STR_DEBUG_POWER[]={"DTR -> POWERON \r\n"};
const   char STR_DEBUG_DELETEDATA[]={"DELETE CURRENT DATA\r\n"};
const   char STR_DEBUG_GPRSSENDDATA[]={"GPRS SEND DATA\r\n"};
const   char STR_AT_DEBUG_CMDTIMEOUT[]={"\nCMD TIMEOUT\r\n"};
const   char STR_AT_DEBUG_PINERROR[]={"\nPIN ERROR!\r\n"};
const   char STR_AT_DEBUG_PUKERROR[]={"\nPUK ERROR!\r\n"};
const   char STR_AT_DEBUG_POWEROFF[]={"\nPOWER OFF STATE....\r\n"};
const   char STR_AT_DEBUG_POWERON[]={"\nPOWER ON STATE\r\n"};
const   char STR_DEBUG_RECEIVE_TIMEROUT[]={"\n Warning:RECEIVE TIMER OUT!!!!!!!!!\n"};
const   char STR_DEBUG_MBPOWR[]={"\nMain Board Power Off!\n"};
const   char STR_DEBUG_GPRSCMD_STATE[]={"\nGPRS CMD State Change!"};
const   char STR_DEBUG_SMSERROR[]={"\r\nSMS Send Error!Delete...\r\n"};
const   char STR_DEBUG_AT_BUFFER_FILL[]={"Buffer"};
const   char STR_DEBUG_COUNTER_ERROR[]={"Counter ERROR!"};
const   char STR_DEBUG_RECEIVE_COUNTER_ERROR[]={"Receive Counter ERROR!"};
const   char STR_DEBUG_RECEIVE_NOT_DATA[]={"\nReceive not data\n"};
const   char STR_DEBUG_RECEIVE_DATA[]={"\nReceive data\n"};
//azh const   char STR_RESET_MC55[]={"\nreset_mc55\n"};
//MC55_GPRS
const   char STR_RESET_MC55[]={"\nreset_mc52i\n"};
//TELIT_GPRS
const   char STR_RESET_MC55_T[]={"\nreset_telit868\n"};
#endif//_MC55_DEBUG

/************************************************************/

void AT_Init(void)
{
    enuATSourceState = STATE_POWEROFF;
    enuATCurrentState = STATE_POWEROFF;
    enuATNextState  = STATE_POWEROFF;
    stAtManageState.uStateTimerCounter = 0;


    ClearCmdBuffer();
    AT_CMD_Clear();

    stReceiveBuffer.ucRetryCnt = 0;
    stReceiveBuffer.usSilenceCnt = 0;
    stReceiveBuffer.ucIsGetVaild = 0;
    stReceiveBuffer.uTimeOutCount = 0;

    memset(&stAtManageState, 0 , sizeof(ST_AT_MANAGE_STATE) );
    stAtManageState.uNoReceiveTimer = 1200*AT_DLY_60S;//6000;   //20小时
//  ucIsATReceived = 0;
    Init_ModChInfo(&stModParaChInfo);
//  PIN_POWERON_CLR;
}

void AT_Manage(void)
{
    AT_State_Manage();
    AT_Send_Manage();
    AT_Receive_Manage();
}

//超时检测函数，每10ms调用一次 改成100ms
void AT_Timer(void)
{
    if(stReceiveBuffer.usSilenceCnt>0)
        stReceiveBuffer.usSilenceCnt--;

    if(stReceiveBuffer.uTimeOutCount >0 )
        stReceiveBuffer.uTimeOutCount --;

    if(stAtManageState.uStateTimerCounter >0)
        stAtManageState.uStateTimerCounter--;

    if(stAtManageState.uPowerOffTimer > 0)
        stAtManageState.uPowerOffTimer--;

    if(stAtManageState.uReceiveTimer > 0)
        stAtManageState.uReceiveTimer--;

    if(stAtManageState.uConnectIndicateTimes > 0)
        stAtManageState.uConnectIndicateTimes--;

    if(stAtManageState.uNoReceiveTimer > 0)
        stAtManageState.uNoReceiveTimer--;

    if(stAtManageState.usReadDataTimes > 0)
        stAtManageState.usReadDataTimes--;

    if (stAtManageState.uSIMIndicateTimes > 0)
        stAtManageState.uSIMIndicateTimes--;

    if ((stAtManageState.uServertimes > 0)&&(stAtManageState.uServertimes !=0xffff))
        stAtManageState.uServertimes--;

    if(ucLink_Time > 0)
        ucLink_Time --;
}


void AT_State_SetDstState(void)
{
    switch(stModuleComParam.stCh[stModuleRunState.ucCurCommChannel].ucType)
    {

    case TYPE_COMUNNICATION_TCP:
        enuATDstState = STATE_GPRS;
        break;

    case TYPE_COMUNNICATION_UDP:
        enuATDstState = STATE_GPRS;
        break;

    case TYPE_COMUNNICATION_SMS:
    default:
        enuATDstState = STATE_GSM;
        break;
    }
}

/********************************************************************
函数名称:   AT_State_Manage
功能    :   手机模块状态转换管理
入口参数:   /
出口参数:   /
*********************************************************************/
void AT_State_Manage(void)
{
    PSTTC35TASK pstDataContent = NULL;
//  UN_CHANNEL_ADDR unAdrInfo;
    unsigned short  usLen;
//  STTERMISCPARA stTerMiscPara=RunPara_GetTerMiscPara();
//  STREMOTERUNINFO stRemoteRunInfo = stTerRunPara.stTerMiscPara.stRCommRunInfo;

    switch(enuATCurrentState)
    {
    case STATE_POWEROFF:

        if (stAtManageState.uStateTimerCounter > AT_DLY_60S)//azh 原先是10ms计数
            stAtManageState.uStateTimerCounter = AT_DLY_60S;//60*100;

        if( (enuATNextState == STATE_POWEROFF) && (stAtManageState.uStateTimerCounter == 0) )       //目标状态与当前状态相同
        {
            if( stAtManageState.bIsPowerOff == 0)
            {
                stAtManageState.ucPowerOffNum = 0;  //add by cfh 10-10-04
//              Uart_InitChBaud( 0,57600,0,8,0 );//与MC55通信端口初始化
        //      Uart_InitChBaud( 0,38400,1,8,0 );//与MC55通信端口初始化
                //当前处于有电状态
                UART3_init(UART3_COMM_PARA, UART3_BAUD_RATE);//azh 110926

                stModuleRunState.bIsSIMExist = 0;
                stModuleRunState.bIsExist = 0;
                stModuleRunState.bIsGPRSConnected = 0;
                Serverflag=0;
                Clientflag=0;
                stAtManageState.ucReconnectTimes = 0;
                AT_CMD_Clear();

                //              PIN_Write0(PIN_CMD_EMERGOFF,1);

                AT_CMD_Fill(PIN_CMD_DELAY_10MS,AT_DLY_100MS);
                AT_CMD_Fill(PIN_CMD_IGT,1);
                //AT_CMD_Fill(PIN_CMD_DELAY_10MS , 200);
                AT_CMD_Fill(PIN_CMD_DELAY_10MS , AT_DLY_2S);
                AT_CMD_Fill(PIN_CMD_IGT,0);
                //AT_CMD_Fill(PIN_CMD_DELAY_10MS , 20);
                AT_CMD_Fill(PIN_CMD_DELAY_10MS , AT_DLY_200MS);
                //      PIN_Write0(PIN_CMD_EMERGOFF,0);
                PIN_EMERGOFF_CLR;
                //AT_CMD_Fill(PIN_CMD_DELAY,2);
                AT_CMD_Fill(PIN_CMD_DELAY,AT_DLY_2S);


                PIN_EMERGOFF_SET;
                AT_CMD_Fill(PIN_CMD_IGT,1);
                //AT_CMD_Fill(PIN_CMD_DELAY,1);
                AT_CMD_Fill(PIN_CMD_DELAY_10MS,AT_DLY_1S);
                AT_CMD_Fill(PIN_CMD_PWCTK,0);       //模块电源关闭
                AT_CMD_Fill(PIN_CMD_PWCTK,1);       //模块电源开启
                AT_CMD_Fill(PIN_CMD_IGT,0);
                AT_CMD_Fill(PIN_CMD_IGT,1);
                //AT_CMD_Fill(PIN_CMD_DELAY_10MS , 20);
                AT_CMD_Fill(PIN_CMD_DELAY_10MS , AT_DLY_200MS);
                AT_CMD_Fill(PIN_CMD_IGT,0);
                //AT_CMD_Fill(PIN_CMD_DELAY_10MS , 20);
                AT_CMD_Fill(PIN_CMD_DELAY_10MS , AT_DLY_200MS);
                //      PIN_Write0(PIN_CMD_EMERGOFF,0);
                PIN_EMERGOFF_CLR;
                //AT_CMD_Fill(PIN_CMD_DELAY,2);
                AT_CMD_Fill(PIN_CMD_DELAY_10MS , AT_DLY_2S);


                enuATNextState = STATE_POWERON;
                //  IOSET0 = PIN_MASK_LED_POWER;
                NETLED_CLR;
                CLR_LOCAL_STATE(CON_GPRS_LED_STATE);
                stAtManageState.OffRegTimes = 0;

                DebugPrint((unsigned char*)STR_AT_DEBUG_POWEROFF, sizeof(STR_AT_DEBUG_POWEROFF));
            }
            else
            {
                stAtManageState.uStateTimerCounter = CON_POWEROFF_DELAY;
                enuATNextState = STATE_POWEROFF;
                //  IOCLR0 = PIN_MASK_LED_INDICATE;
                NETLED_CLR;
                CLR_LOCAL_STATE(CON_GPRS_LED_STATE);//azh 120309
                //  IOCLR0 = PIN_MASK_LED_POWER;
            }
        }
        break;

    case STATE_POWERON:

        if(enuATNextState == STATE_POWERON)     //目标状态与当前状态相同
        {
            enuATNextState = STATE_PWD_CHK;
            enuATSourceState = STATE_POWERON;
            stModuleRunState.bIsGPRSConnected = 0;
            Serverflag=0;
            Clientflag=0;
            stModuleRunState.bIsExist = 0;
            stModuleRunState.bIsPINLock = 0;
            stModuleRunState.bIsPUKLock = 0;
            stModuleRunState.bIsPWDError = 0;
            stModuleRunState.bIsSIMExist = 0;
            stAtManageState.ucCSQErrorCount = 0;
            DebugPrint((unsigned char*)STR_AT_DEBUG_POWERON, sizeof(STR_AT_DEBUG_POWERON));
            //AT_CMD_Fill(PIN_CMD_DELAY,1);     //重新开启模块
            AT_CMD_Fill(PIN_CMD_DELAY,AT_DLY_1S);       //重新开启模块

////MC55_GPRS
//            AT_CMD_Fill(AT_CMD_SET_AT,0);       //at测试
//            //AT_CMD_Fill(PIN_CMD_DELAY,1);     //重新开启模块
//            AT_CMD_Fill(PIN_CMD_DELAY,AT_DLY_1S);       //重新开启模块
//            AT_CMD_Fill(AT_CMD_SET_IPR, 0);
////MC55_GPRS
//TELIT_GPRS
            AT_CMD_Fill(AT_CMD_SET_AT,AT_DLY_200MS);       //at测试
            //AT_CMD_Fill(PIN_CMD_DELAY,1);     //重新开启模块
            AT_CMD_Fill(PIN_CMD_DELAY,AT_DLY_1S);       //重新开启模块
            AT_CMD_Fill(AT_CMD_SET_IPR,AT_DLY_200MS);
//TELIT_GPRS
            //      PIN_LED2_SET;
//azh 141023
            AT_CMD_Fill(PIN_CMD_DELAY,AT_DLY_1S);
            AT_CMD_Fill(AT_CMD_SET_ATE0,0);         //关闭回显
            AT_CMD_Fill(AT_CMD_QUERY_GMR,0);    //显示模块版本号
            AT_CMD_Fill(PIN_CMD_DELAY,AT_DLY_1S);
        }
        break;

    case STATE_PWD_CHK:

        //  IOCLR0 = PIN_MASK_LED_INDICATE;
        NETLED_CLR;
        CLR_LOCAL_STATE(CON_GPRS_LED_STATE);
        //  IOSET0 = PIN_MASK_LED_POWER;
        //  PIN_LED2_SET;

        if(enuATNextState == STATE_PWD_CHK)
        {
//azh 170420 抄表485或者zigbee通信不检查了。
            set_normal_run_flag(1 << 0); 
            set_normal_run_flag(1 << 1);                    //升级有效标识检查之一GPRS通信正常确认
            //AT_CMD_Fill(PIN_CMD_DELAY,3);     //重新开启模块
            AT_CMD_Fill(PIN_CMD_DELAY,AT_DLY_3S);       //重新开启模块
            AT_CMD_Fill(AT_CMD_QUERY_CPIN,0);   //查询密码锁定状态
//          AT_CMD_Fill(AT_CMD_SET_ATE0,0);         //关闭回显
//          AT_CMD_Fill(AT_CMD_SET_FRS,0);
//          AT_CMD_Fill(AT_CMD_SET_FLO,0);          //关闭流控
            enuATNextState = STATE_INIT;
        }
        break;

    case STATE_LOOP:    //SIM卡未插、密码错误会进入循环状态
        //  IOSET0 = PIN_MASK_LED_POWER;
        //  IOSET0 = PIN_MASK_LED_INDICATE;
        NETLED_CLR;
        CLR_LOCAL_STATE(CON_GPRS_LED_STATE);
        
        if(stModParaChInfo.bIsPWDChanged == 1)
        {
            enuATCurrentState = enuATNextState = STATE_PWD_CHK;
        }
        else
        {
            if( stAtManageState.uStateTimerCounter == 0 && enuATNextState == STATE_LOOP )
            {
                enuATNextState = STATE_INIT;
                AT_CMD_Fill(AT_CMD_QUERY_CPIN,0);   //重新查询SIM卡

                //如果是由于密码错误进入的循环状态，则密码变更后重新启动模块，一般应进入PUK解锁状态
                if(stModParaChInfo.bIsPWDChanged == 1 && stAtManageState.bIsPWDUsed == 1)
                {
                    enuATCurrentState = enuATNextState = STATE_POWERON;
                    stModParaChInfo.bIsPWDChanged = 0;
                }
            }
            AT_Power();                             //关电处理
        }
        break;

    case STATE_INIT:                                //初始化

        NETLED_CLR;
        CLR_LOCAL_STATE(CON_GPRS_LED_STATE);
        
        if(enuATNextState == STATE_INIT)            //目标状态与当前状态相同
        {
            AT_CMD_Clear();
            stAtManageState.ucReconnectTimes = 0;
            stAtManageState.uStateTimerCounter = 0;
            stAtManageState.ucIsDataSending = 0;
            stAtManageState.ucIsDataRecving = 0;
            stAtManageState.usDataReceiveLength = 0;
            stAtManageState.usDataSendLength = 0;
            stAtManageState.ucDataReadDelayTimes = 0;
            //add by 090526
            stAtManageState.ucCMGLErrorCount=0;
            stAtManageState.ucSCPMSErrorCount =0;
            //add by 090526 end
            enuATSourceState = STATE_INIT;
            AT_State_SetDstState();
            AT_CMD_Fill(AT_CMD_SET_AT,0);
            //AT_CMD_Fill(PIN_CMD_DELAY,4);
            AT_CMD_Fill(PIN_CMD_DELAY,AT_DLY_4S);
            AT_CMD_Fill(AT_CMD_SET_ATE0,0);         //关闭回显
//            AT_CMD_Fill(AT_CMD_QUERY_GMR,0);    //显示模块版本号
            //AT_CMD_Fill(PIN_CMD_DELAY,4);
            AT_CMD_Fill(PIN_CMD_DELAY,AT_DLY_4S);
            AT_CMD_Fill(AT_CMD_SET_CMGF,0);         //设置短信模式
//MC55_GPRS
        if(gucGprs_Module_Type == MC55_GPRS_MODULE)
        {              
            AT_CMD_Fill(AT_CMD_SET_SGCONF,0);       //检查网络信号强度和SIM卡情况
            AT_CMD_Fill(AT_CMD_QUERY_SGCONF,0);     //检查网络信号强度和SIM卡情况
        }
//MC55_GPRS
            AT_CMD_Fill(AT_CMD_SET_CREG,0);//检查GSM网络注册情况

            //AT_CMD_Fill(PIN_CMD_DELAY,3);
            AT_CMD_Fill(PIN_CMD_DELAY,AT_DLY_3S);
            AT_CMD_Fill(AT_CMD_SET_CNMI,0);   //设置模块主动上报短信
//TELIT_GPRS            
        if(gucGprs_Module_Type == TELIT_GPRS_MODULE)
        {
            AT_CMD_Fill(AT_CMD_SET_SCFG,0);
            AT_CMD_Fill(AT_CMD_SET_SCFGEXT,0);
		    AT_CMD_Fill(PIN_CMD_DELAY,3);
		}
//TELIT_GPRS
            //AT_CMD_Fill(PIN_CMD_DELAY,2);
            AT_CMD_Fill(PIN_CMD_DELAY,AT_DLY_2S);
            //  AT_CMD_Fill(PIN_CMD_DELAY,30);
            AT_CMD_Fill(AT_CMD_SET_CPMS,0);         //短信存储模式设置
        //  AT_CMD_Fill(PIN_CMD_DELAY,3);
            //AT_CMD_Fill(PIN_CMD_DELAY,10);
            AT_CMD_Fill(PIN_CMD_DELAY,AT_DLY_10S);

            AT_CMD_Fill(AT_CMD_QUERY_CSQ,0);        //检查网络信号强度和SIM卡情况


            enuATNextState = STATE_GSM;
        }
        break;

    case STATE_GSM:
        //      IOCLR0 = PIN_MASK_LED_INDICATE;

        ConnectIndicate();//0.5秒闪烁
        //      ModemLedPower();
        stModuleRunState.bIsGPRSConnected = 0;
        Serverflag=0;
        Clientflag=0;
        if (stAtManageState.uStateTimerCounter > CON_TIMEER_GPRSCMDSTATE)
            stAtManageState.uStateTimerCounter = CON_TIMEER_GPRSCMDSTATE;

        if ((enuATNextState != STATE_GSM) && (enuATNextState != STATE_GPRS))
            enuATNextState = STATE_GSM;

        if((stAtManageState.uStateTimerCounter == 0) && (IsATCmdFinish() == 1)) //定时时间到且所有的命令均已完成
        {
            AT_State_SetDstState();

            if((enuATDstState != STATE_GSM) && (IP_Check(stModuleRunState.ucCurCommChannel) != 0 ))         //目标状态与当前状态不相同
            {
                if(enuATNextState == STATE_GSM)     //未启动初始化命令
                {
                    if( stModuleRunState.ucSignIntension >0 )
                    {   //准备进行数据连接

                        stAtManageState.ucIsDataSending = 0;            //初始化
                        stAtManageState.ucIsDataRecving = 0;
//                      AT_CMD_Fill(AT_CMD_QUERY_CPMS,0);           //检查短信存储情况
//                      AT_CMD_Fill(AT_CMD_QUERY_CNMI,0);

//                      AT_CMD_Fill(AT_CMD_QUERY_CSQ,0);            //检查网络信号强度和SIM卡情况
//                      AT_CMD_Fill(PIN_CMD_DELAY,1);
                        AT_CMD_Fill(AT_CMD_QUERY_CMGL,0);           //列出短信
//MC55_GPRS
                        if(gucGprs_Module_Type == MC55_GPRS_MODULE)
                        { 
                        AT_CMD_Fill(AT_CMD_SET_CGATT_TACH , 0);
                        AT_CMD_Fill(AT_CMD_SET_CGATT_DETACH , 0);
                        //add by 090526
                        AT_CMD_Fill(AT_CMD_SET_CGATT_TACH , 0);
                        //add by 090526 end
                        AT_CMD_Fill(AT_CMD_SET_SICS_GPRS0,0);
                        }
///MC55_GPRS

//TELIT_GPRS
                        if(gucGprs_Module_Type == TELIT_GPRS_MODULE)
                        { 
			   			AT_CMD_Fill(AT_CMD_SET_FLO,0);  //关闭流控
			   		    }
//

                        AT_CMD_Fill(AT_CMD_SET_SICS_APN,0);
                        //AT_CMD_Fill(PIN_CMD_DELAY,1);
                        AT_CMD_Fill(PIN_CMD_DELAY,AT_DLY_1S);

                        if((stModParaChInfo.bIsAccountChanged == 1) &&(stModuleComParam.stPDPAccount.ucIsAccountValid == 1))
                        {
                            AT_CMD_Fill(AT_CMD_SET_SICS_USER,stModuleRunState.ucCurCommChannel);
                            AT_CMD_Fill(AT_CMD_SET_SICS_PASSWD,stModuleRunState.ucCurCommChannel);
                        }
                        stModParaChInfo.bIsAccountChanged = 0;
                        stModParaChInfo.bIsCHInfoChanged = 0;

                        //清除PDP索引清除
                        //                      AT_CMD_Fill(AT_CMD_QUERY_SICS,0);
                        //AT_CMD_Fill(PIN_CMD_DELAY,1);
                        AT_CMD_Fill(PIN_CMD_DELAY,AT_DLY_1S);
//MC55_GPRS
                        if(gucGprs_Module_Type == MC55_GPRS_MODULE)
                        {  
                        AT_CMD_Fill(AT_CMD_SET_SISS_CONID, stModuleRunState.ucCurCommChannel);
                        AT_CMD_Fill(AT_CMD_SET_SISS_ALPHABETAT,stModuleRunState.ucCurCommChannel);
                        }
//MC55_GPRS
                        if(stModuleComParam.stCh[stModuleRunState.ucCurCommChannel].ucType == TYPE_COMUNNICATION_TCP)
                        {
//MC55_GPRS
                            if(gucGprs_Module_Type == MC55_GPRS_MODULE)
                            {                              
                            AT_CMD_Fill(AT_CMD_SET_SISS_SRVTYPE_SOCKET,stModuleRunState.ucCurCommChannel);
                            }
//MC55_GPRS

//TELIT_GPRS
                            if(gucGprs_Module_Type == TELIT_GPRS_MODULE)
                            { 							
							AT_CMD_Fill(AT_CMD_SET_GPRS,stModuleRunState.ucCurCommChannel);
							}
//TELIT_GPRS
                            AT_CMD_Fill(AT_CMD_SET_SISS_ADDRESS_TCP,stModuleRunState.ucCurCommChannel);
//MC55_GPRS
                            if(gucGprs_Module_Type == MC55_GPRS_MODULE)
                            {  
                            AT_CMD_Fill(AT_CMD_SET_SISO,stModuleRunState.ucCurCommChannel);
                            }
//MC55_GPRS
                        }
                        else if (stModuleComParam.stCh[stModuleRunState.ucCurCommChannel].ucType == TYPE_COMUNNICATION_UDP)
                        {
                            AT_CMD_Fill(AT_CMD_SET_SISS_SRVTYPE_SOCKET,stModuleRunState.ucCurCommChannel);
                            AT_CMD_Fill(AT_CMD_SET_SISS_ADDRESS_UDP,stModuleRunState.ucCurCommChannel);
                            AT_CMD_Fill(AT_CMD_SET_SISO,stModuleRunState.ucCurCommChannel);
                        }
                        stAtManageState.ucReconnectTimes++;

                    }
                    else
                    {
                        //滞留在GSM状态则需要查询手机状态
                        AT_CMD_Fill(AT_CMD_SET_AT ,0);
                        //AT_CMD_Fill(PIN_CMD_DELAY,1);
                        AT_CMD_Fill(PIN_CMD_DELAY,AT_DLY_1S);
                        AT_CMD_Fill(AT_CMD_QUERY_CPMS,0);           //检查短信存储情况
                        AT_CMD_Fill(AT_CMD_QUERY_CNMI,0);
                        //AT_CMD_Fill(PIN_CMD_DELAY,1);
                        AT_CMD_Fill(PIN_CMD_DELAY,AT_DLY_1S);
                        AT_CMD_Fill(AT_CMD_QUERY_CSQ,0);
                        AT_CMD_Fill(AT_CMD_SET_ATE0,0);     //关闭回显,以防以外引起的回显开启
                        enuATNextState = STATE_GSM;
                        stAtManageState.uStateTimerCounter = CON_TIMEER_GSMSTATE;
                    }
                }
            }
            else
            {
                stAtManageState.uStateTimerCounter = CON_TIMEER_GSMSTATE;
            }
        }

        else if((IsATCmdFinish() == 1) &&( enuATNextState == STATE_GSM ))   //滞留在GSM状态时可以发送短信数据
        {
        /*          if( stPinSate.bIsRingPulse==1 )             //有新短信：这里是否需要改动，防止脉冲的重复检测
        {
        AT_CMD_Fill(AT_CMD_QUERY_CMGL ,0);
        AT_CMD_Fill(AT_CMD_SET_CHUP ,0);
        stPinSate.bIsRingPulse = 0;
        }               */
            //发送数据处理
            pstDataContent =  ATGetDataContentPtr(0);
            if(pstDataContent != NULL)  //判断是否有数据发送
            {//有数据要发
                if( IP_Check(pstDataContent->ucChNo) == 0 )
                {//是GPRS数据，在GSM状态下发不了，直接删掉
                    ATDataListPop();
                    stModuleRunState.ucCurCommChannel = 0;
                }
                else if((pstDataContent->unCurAddrInfo.ucType == TYPE_COMUNNICATION_SMS))   //有短信数据要发送
                {//为短信数据
                    if(stModuleComParam.stCh[pstDataContent->ucChNo].unChInfo.stSMSParam.ucLen != 0x00)
                    {
                        AT_CMD_Fill(AT_CMD_SET_CMGS,0);     //发送短信
                        stAtManageState.uStateTimerCounter = CON_TIMEER_GSMSTATE;;//有数据发送推迟状态转换
                    }
                    else
                        ATDataListPop();
                }
                else    //需要发送的数据为GPRS数据
                {
                    if( (stModuleRunState.ucCurCommChannel == pstDataContent->ucChNo)&&(Serverflag==0)) //与当前通道相符
                    {
                        //如果目标GPRS通道重连3次仍不成功，丢弃该通道的所有数据，退回通道0
                        if(stAtManageState.ucReconnectTimes >6)
                        {//长时间连不上主站，把数据帧删掉
                            while( (pstDataContent != NULL) &&
                                (pstDataContent->ucChNo == stModuleRunState.ucCurCommChannel))
                            {
                                ATDataListPop();
                                pstDataContent =  ATGetDataContentPtr(0);
                                DebugPrint((unsigned char*)STR_DEBUG_DELETEDATA, sizeof(STR_DEBUG_DELETEDATA));
                            }
                            stAtManageState.ucReconnectTimes = 0;
                            //返回通道0
                            stModuleRunState.ucCurCommChannel = 0;
                        }
                    }
                    else    //需要切换通道
                    {
                        stModuleRunState.ucCurCommChannel = pstDataContent->ucChNo ;
                        stAtManageState.ucReconnectTimes = 0;   //连接次数清除
                        stAtManageState.uStateTimerCounter = 0; //加速状态转换
                    }
                }
            }

            if((IsATCmdFinish() == 1)&&(pstDataContent == NULL))
            {

                AT_CMD_Fill(AT_CMD_SET_AT,0);
                AT_CMD_Fill(AT_CMD_QUERY_CSQ,0);
                //AT_CMD_Fill(PIN_CMD_DELAY,1);
                AT_CMD_Fill(PIN_CMD_DELAY,AT_DLY_1S);
                AT_CMD_Fill(AT_CMD_QUERY_CPMS,0);           //检查短信存储情况
                AT_CMD_Fill(AT_CMD_QUERY_CNMI,0);
                //AT_CMD_Fill(PIN_CMD_DELAY,1);
                AT_CMD_Fill(PIN_CMD_DELAY,AT_DLY_1S);
                AT_CMD_Fill(AT_CMD_QUERY_CMGL,0);
                //              AT_CMD_Fill(AT_CMD_QUERY_SGCONF,0);     //检查网络信号强度和SIM卡情况

            }

            if((stModParaChInfo.bIsPWDChanged == 1) &&(stCMDBuffer.ucATCMD != AT_CMD_SET_SISO))             //密码变更
            {
                AT_CHANGE_PWD();
            }
        }
        AT_Power();     //关电处理

        break;

    case STATE_GPRS:

        //      ModemLedPower();
        //      PIN_LED1_SET;
        ConnectIndicate();//发了登录帧后才常亮 有信号时是0.5s闪烁
//        NETLED_SET;
        stAtManageState.ucPowerOffNum = 0 ;

        pstDataContent =  ATGetDataContentPtr(0);                       //获取要发送的数据指针
        if( (pstDataContent == NULL) &&  (stAtManageState.uStateTimerCounter == 0)&&(stModuleComParam.ucGPRSServer==0) )
        {
            trace_at_cmd_sn(1);
            AT_CMD_Fill(AT_CMD_SET_SISC,stModuleRunState.ucCurCommChannel);                         //复位DTR脚进入命令状态
//            AT_CMD_Fill(AT_CMD_SET_CGATT_DESOC,0);
            enuATNextState = STATE_GSM;
            stAtManageState.uStateTimerCounter = CON_TIMEER_GPRSCMDSTATE;
            stModuleRunState.ucCurCommChannel = 0;
        }
        else if(enuATCurrentState == enuATNextState)                    //TCP、UDP稳定态，发送数据
        {
            if((stAtManageState.usReadDataTimes == 0)
                &&(stAtManageState.ucIsDataSending == 0)
                &&(stCMDBuffer.enuBufferState == STATE_BUFFER_IDLE)
                &&(stAtManageState.GPRSNeedReconnect == 0)&& (stAtManageState.ucIsDataRecving == 0))
            {
                stAtManageState.ucQUERYTimes++;
                if(stAtManageState.ucQUERYTimes>5)//1)
                {
                       AT_CMD_Fill(AT_CMD_QUERY_CPMS,0);            //检查短信存储情况
                       AT_CMD_Fill(AT_CMD_QUERY_CNMI,0);
                       //AT_CMD_Fill(PIN_CMD_DELAY,1);
                       AT_CMD_Fill(PIN_CMD_DELAY,AT_DLY_1S);
                       AT_CMD_Fill(AT_CMD_QUERY_CMGL,0);            //列出短信
                       AT_CMD_Fill(AT_CMD_QUERY_CSQ,0);
                       //                       AT_CMD_Fill(AT_CMD_QUERY_SGCONF,0);     //检查网络信号强度和SIM卡情况

                       stAtManageState.ucQUERYTimes = 0;
                       if(stModParaChInfo.bIsPWDChanged == 1)               //密码变更
                       {
                           AT_CHANGE_PWD();
                       }
                }

                //AT_CMD_Fill(PIN_CMD_DELAY_10MS , 5);
//MC55_GPRS
                if(gucGprs_Module_Type == MC55_GPRS_MODULE)
                {                  
                AT_CMD_Fill(PIN_CMD_DELAY_10MS,AT_DLY_200MS);
                AT_CMD_Fill(AT_CMD_SET_SISR , stModuleRunState.ucCurCommChannel);//
                }
//MC55_GPRS
//azh 141026 加上这一句会出错
//                else if(gucGprs_Module_Type == TELIT_GPRS_MODULE)//azh 141024 这个是新加的后面要测试一下有没有问题
//                {   
//                    AT_CMD_Fill(AT_CMD_SET_SISR , 1);
//                }
/*//azh telit
                if(Clientflag==1)
                {AT_CMD_Fill(AT_CMD_SET_SISR , 1);}//
                else
                {AT_CMD_Fill(AT_CMD_SET_SISR , 0);}
*/
                //AT_CMD_Fill(PIN_CMD_DELAY_10MS , 5);//50MS
                AT_CMD_Fill(PIN_CMD_DELAY_10MS,AT_DLY_200MS);
                stAtManageState.usReadDataTimes = 30*AT_DLY_1S;//100;
            }


            if((pstDataContent != NULL ) && (IsATCmdFinish() == 1))
            {
                /********************* 数据发送 *********************/

                if(stAtManageState.GPRSNeedReconnect == 1)
                    ATDataListPop();                                    //发送后从列表中删除数据

                else if( pstDataContent->ucChNo == stModuleRunState.ucCurCommChannel)
                    //通道匹配直接发送数据
                {
                    stModuleRunState.bIsRing = 0;               //清除电话唤醒标志
                    if(stAtManageState.ucIsDataSending == 0)
                    {
                        if((pstDataContent->ucChNo==1)&&(Clientflag==0))
                        {
                            AT_CMD_Fill(AT_CMD_SET_SISS_ADDRESS_TCP,1);
                            Clientflag = 1;
                        }
                        else
                        {
                            usLen = SCIBufferGetSpareLength(&stAT_TX_BUFFER);
                            if(usLen >= pstDataContent->ucDataLength)
                                //AT_CMD_Fill(PIN_CMD_DELAY_10MS , 6);
                                AT_CMD_Fill(PIN_CMD_DELAY_10MS,AT_DLY_100MS);
                            AT_CMD_Fill(AT_CMD_SET_SISW,pstDataContent->ucChNo);
                            stAtManageState.ucIsDataSending = 1;
                        }
                    }
                }
                
                else//通道不匹配需要切换进入命令态发送
                {
                    if ( IP_Check(pstDataContent->ucChNo) == 0 )
                    {
                        ATDataListPop();
                    }
                    else    if( stModuleComParam.stCh[pstDataContent->ucChNo].ucType == TYPE_COMUNNICATION_TCP ||
                        stModuleComParam.stCh[pstDataContent->ucChNo].ucType == TYPE_COMUNNICATION_UDP)
                    {                                                   //如果通道是TCP/UDP，则断线重登
                        trace_at_cmd_sn(2);
                        AT_CMD_Fill(AT_CMD_SET_SISC, stModuleRunState.ucCurCommChannel);
//                        AT_CMD_Fill(AT_CMD_SET_CGATT_DESOC,0);
                        enuATNextState = STATE_GSM;
                        stAtManageState.uStateTimerCounter = CON_TIMEER_GPRSCMDSTATE;
                        stModuleRunState.ucCurCommChannel = pstDataContent->ucChNo;
                    }

                    else if(stModuleComParam.stCh[pstDataContent->ucChNo].unChInfo.stSMSParam.ucLen != 0x00)
                    {
                        AT_CMD_Fill(AT_CMD_SET_CMGS,0);                 //如果通道是SMS，且有效，则掉线
                        stAtManageState.uStateTimerCounter = AT_DLY_35S;//azh 加长一点时间 CON_TIMEER_GSMSTATE;
                    }
                    else                                                //否则直接丢失数据
                        ATDataListPop();                                //发送后从列表中删除数据
                }
                
            }

            if(stAtManageState.bIsNewSMSENTITY==NEW_SMS_ENTITY)         //有短信收到
            {
                AT_CMD_Fill(AT_CMD_QUERY_CMGL,0);
                stAtManageState.bIsNewSMSENTITY=NEW_SMS_NOT_ENTITY;
            }

            if (  (stAtManageState.uReceiveTimer == 0)  ||              //稳定状态如果一段时间未接收到数据则重新进行IP连接
                (stModParaChInfo.bIsCHInfoChanged == 1) )               //当前通道信息改变，需要断线重连
            {
                trace_at_cmd_sn(3);
                AT_CMD_Fill(AT_CMD_SET_SISC, stModuleRunState.ucCurCommChannel);
//                AT_CMD_Fill(AT_CMD_SET_CGATT_DESOC,0);
                enuATNextState=STATE_GSM;
                stAtManageState.uStateTimerCounter = CON_TIMEER_GPRSCMDSTATE;
                stModuleRunState.ucCurCommChannel = 0;
                stModParaChInfo.bIsCHInfoChanged = 0;
            }
            if((ucClientNeed==1)&&(Clientflag==1)&&(stModuleComParam.ucGPRSServer==1))
            {
                //AT_CMD_Fill(AT_CMD_SET_CGATT_DESOC,0);
                trace_at_cmd_sn(4);
                AT_CMD_Fill(AT_CMD_SET_SISC, stModuleRunState.ucCurCommChannel);
                stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat=CON_STATUS_NO;
                Clientflag=0;
                //azh 全局已经改掉了
                //stTerRunPara.stTerMiscPara.stRCommRunInfo = stTerRunPara.stTerMiscPara.stRCommRunInfo;
                //RunPara_SetTerMiscPara(&stTerRunPara.stTerMiscPara);
            }
            if((ucClientNeed==0)&&(Clientflag==0)&&(stModuleComParam.ucGPRSServer==1))
            {
                AT_CMD_Fill(AT_CMD_SET_SISS_ADDRESS_TCP,1);
                Clientflag=1;
            }
            if ((stAtManageState.ucSISRNeeded == 1 ) && (stAtManageState.ucIsDataSending == 0))
            {
                AT_CMD_Fill(AT_CMD_SET_SISR , stAtManageState.ucSISRParam);
                stAtManageState.usReadDataTimes = CON_DATAREADTIMES;
                stAtManageState.ucSISRNeeded = 0;
            }

            if(  stAtManageState.GPRSNeedReconnect == 1 )
            {
//azh
                trace_at_cmd_sn(5);
                AT_CMD_Fill(AT_CMD_SET_SISC , stModuleRunState.ucCurCommChannel );
//MC55_GPRS
                if(gucGprs_Module_Type == MC55_GPRS_MODULE)
                {                
                AT_CMD_Fill(AT_CMD_SET_CGATT_DETACH,0);
                }
//MC55_GPRS
            }

        }
//azh telit
//      if((stAtManageState.uServertimes==0)&&(Serverflag==1))
//      {
//          AT_CMD_Fill(AT_CMD_SET_CGATT_OFFSERVER, 0 );
//          stAtManageState.uServertimes=0xffff;
//      }
        AT_Power();     //关电处理
        break;

    default:
        enuATSourceState = enuATCurrentState = enuATNextState  = STATE_POWEROFF;

        break;
    }
}

/********************************************************************
函数名称:   AT_Send_Manage
功能    :   AT命令缓冲区发送管理
入口参数:   /
出口参数:
*********************************************************************/
void AT_Send_Manage()
{
    unsigned ucRet = 0;

    //AT命令发送

    switch(stCMDBuffer.enuBufferState)
    {
    case STATE_BUFFER_IDLE: //当前无发送任务

        ucRet = AT_Send_FillBuffer();
        if(ucRet>0) //填写串口缓冲区
        {
    //      memcpy(SIO4_Buffer,(unsigned char *)(0xfffff720),13);
//          SCIBufferWrite(&stAT_TX_BUFFER , stCMDBuffer.ucCMDBuffer , (unsigned short)stCMDBuffer.ucLength);
            //azh 110929
            Gprs_send_frame(stCMDBuffer.ucCMDBuffer , (unsigned short)stCMDBuffer.ucLength);
            DebugPrint(stCMDBuffer.ucCMDBuffer , (unsigned short)stCMDBuffer.ucLength);

            stReceiveBuffer.uTimeOutCount = stCMDBuffer.uTimeOutCount;//azh 参数进来就是100ms的单位啦
            if(stReceiveBuffer.uTimeOutCount == 0)
            {
                stReceiveBuffer.uTimeOutCount = 1;
            }

            stCMDBuffer.ucRetryCnt--;
            stReceiveBuffer.ucRetryCnt = stCMDBuffer.ucRetryCnt;
            stCMDBuffer.enuBufferState = STATE_BUFFER_WAIT;
        }
        break;

    case STATE_BUFFER_SEND:     //重新发送状态
    //  memcpy(SIO4_Buffer,(unsigned char *)(0xfffff720),13);
        //填写串口缓冲区
        //SCIBufferWrite(&stAT_TX_BUFFER , stCMDBuffer.ucCMDBuffer , (unsigned short)stCMDBuffer.ucLength);
        //azh 110929
       if(stCMDBuffer.ucLength > 0)//azh 111017
        {
            Gprs_send_frame(stCMDBuffer.ucCMDBuffer , (unsigned short)stCMDBuffer.ucLength);
            DebugPrint(stCMDBuffer.ucCMDBuffer , (unsigned short)stCMDBuffer.ucLength);
          }

        stReceiveBuffer.uTimeOutCount = stCMDBuffer.uTimeOutCount;//azh 已经是100ms 单位
//        stReceiveBuffer.uTimeOutCount = stCMDBuffer.uTimeOutCount/10;//azh 110928 改成100ms计数
        if(stReceiveBuffer.uTimeOutCount == 0)
        {
            stReceiveBuffer.uTimeOutCount = 1;
        }

        stCMDBuffer.ucRetryCnt--;
        stReceiveBuffer.ucRetryCnt = stCMDBuffer.ucRetryCnt;
        stCMDBuffer.enuBufferState = STATE_BUFFER_WAIT;
        break;

    case STATE_BUFFER_WAIT:
        ucRet = 2;
        break;
    default:
        stCMDBuffer.enuBufferState = STATE_BUFFER_IDLE;
        break;
    }
}


//unsigned short AT_GetStringFromATBuffer(unsigned char* pstr)
//{
//    unsigned short  usLen = 0;
//    //  unsigned short  i = 0;
//    //  unsigned short  counter = 0;
//
//    //  if( enuATCurrentState == STATE_GPRS )
//    //  {
//    /*  if (stCMDBuffer.ucATCMD != AT_CMD_SET_SISO)
//    {
//    usLen = SCIBufferGetDataLength( &stAT_RX_BUFFER );
//    i = stAT_RX_BUFFER.usHeadPtr;
//
//      for(counter=0; counter<=usLen ; counter++)
//      {
//      if (stAT_RX_BUFFER.pBuf[i] ==  '\r' )
//      {
//      if(i >= stAT_RX_BUFFER.usMaxDepth)
//                        i = 0;
//                        i++ ;
//                        if (stAT_RX_BUFFER.pBuf[i++] ==  '\n' )
//                        break;
//                        }
//                        }
//
//                          if ( counter<usLen )
//                          usLen = counter;
//                          else
//                          usLen = SCIBufferGetDataLength( &stAT_RX_BUFFER );
//                          }
//    else */
//    /*
//    usLen = SCIBufferGetDataLength( &stAT_RX_BUFFER );
//
//    if( usLen > CON_AT_MAX_REVEIVE_DATA )
//        usLen = CON_AT_MAX_REVEIVE_DATA;
//
//    SCIBufferRead( &stAT_RX_BUFFER, 0 , pstr , usLen);
//    SCIBufferMoveHeadPtr(&stAT_RX_BUFFER, usLen);
//    //  }
//    */
//    /*  else
//    {
//    while( SCIBufferGetByte(&stAT_RX_BUFFER, &pstr[usLen]) >0)
//    {
//    usLen++;
//    if(usLen >= 2)
//    {
//                if( ( pstr[usLen-2] == '\r' ) &&  ( pstr[usLen-1] == '\n' ))
//                break;
//                }
//                if( usLen >= CON_AT_MAX_REVEIVE_DATA )
//                {
//                break;
//                }
//                }
//                pstr[usLen] = 0x00;
//                }
//    */
//    return usLen;
//}


void AT_Receive_Manage()
{
    unsigned short  usDataLength = 0;
    unsigned char   ucRet = 0;
    unsigned short  usSISRReceiveDataLength = 0;
    unsigned char   i = 0;
    unsigned char   k = 0;
    unsigned char   m = 0;
    unsigned char   uctemp[20];
    //有字符接收
    if(stReceiveBuffer.ucIsGetVaild == 1)
    {
        //获得AT接收缓冲区的有效数据长度
        usDataLength = SCIBufferGetDataLength(&stAT_RX_BUFFER);

        //如果发的命令是AT_CMD_SET_SISR
        if (stCMDBuffer.ucATCMD == AT_CMD_SET_SISR)
        {
            if(usDataLength >= CON_AT_MAX_REVEIVE_PACKAGE)      //changed by cfh 10-01-12
                usDataLength = CON_AT_MAX_REVEIVE_PACKAGE;
            SCIBufferRead(&stAT_RX_BUFFER ,0, stReceiveBuffer.ucBuffer ,usDataLength);
    //      SCIBufferAdjust(stReceiveBuffer.ucBuffer,&usDataLength);//cfh 10-04-14
            if (usDataLength >= 15 )
            {
                for(i=9;i<=18;i++)
                {
                    if((stReceiveBuffer.ucBuffer[i] == 0x2d))
                    {
                        ClearCmdBuffer();
                        AT_CMD_Clear();
                        //DebugPrint((unsigned char*)STR_DEBUG_MBPOWR, sizeof(STR_DEBUG_MBPOWR));
                        trace_at_cmd_sn(6);
                        AT_CMD_Fill(AT_CMD_SET_SISC,stModuleRunState.ucCurCommChannel);                         //复位DTR脚进入命令状态
                        break;
                    }
                    if(stReceiveBuffer.ucBuffer[i] == 0x0d)
                        break;
                }
                if(i <= 18)//azh add 111215
                {
                    k=i-12;
                    if (k==1)
                        usSISRReceiveDataLength = stReceiveBuffer.ucBuffer[12]-0x30;
                    else if (k==2)
                        usSISRReceiveDataLength = (stReceiveBuffer.ucBuffer[12]-0x30)*10+(stReceiveBuffer.ucBuffer[13]-0x30);
                    else if (k==3)
                        usSISRReceiveDataLength = (stReceiveBuffer.ucBuffer[12]-0x30)*100+(stReceiveBuffer.ucBuffer[13]-0x30)*10+(stReceiveBuffer.ucBuffer[14]-0x30);
                    else //if (k==4)        //add by cfh 10-01-14
                        usSISRReceiveDataLength = (stReceiveBuffer.ucBuffer[12]-0x30)*1000+(stReceiveBuffer.ucBuffer[13]-0x30)*100+(stReceiveBuffer.ucBuffer[14]-0x30)*10+(stReceiveBuffer.ucBuffer[15]-0x30);
            //  else    //delete by cfh 10-04-08
                }
                else
                {
                    usSISRReceiveDataLength = 0;
                }


                stAtManageState.usDataReceiveLength = usSISRReceiveDataLength;
                if( usSISRReceiveDataLength == 0)
                {
                    if (usDataLength >= 21)
                        stReceiveBuffer.usSilenceCnt = 0;
                }
                else
                {
                    if (usSISRReceiveDataLength >= 1000)
                        m = 26;
                    else if (usSISRReceiveDataLength >= 100)
                        m = 25;
                    else if (usSISRReceiveDataLength >= 10)
                        m = 24;
                    else
                        m = 23;

                    if ( usDataLength >= usSISRReceiveDataLength + m )
                    {
                        stReceiveBuffer.usSilenceCnt = 0;
                        stAtManageState.usDataReceiveLength = usDataLength;
                        stAtManageState.ucDataReadDelayTimes = 0;


                    }
                    else if (stReceiveBuffer.usSilenceCnt == 0)
                    {
                        stReceiveBuffer.usSilenceCnt = CON_AT_SILENCE_MAX;
                        stAtManageState.ucDataReadDelayTimes++;
//                        ultest1++;
                    }
                }
            }
            else if (stReceiveBuffer.usSilenceCnt == 0)
            {
                stReceiveBuffer.usSilenceCnt = CON_AT_SILENCE_MAX;
                stAtManageState.ucDataReadDelayTimes++;
//                ultest2++;
            }

            if (stAtManageState.ucDataReadDelayTimes > 10)
            {
                stReceiveBuffer.usSilenceCnt = 0;
                stAtManageState.ucDataReadDelayTimes = 0;
                ClearCmdBuffer();
                trace_at_cmd_sn(7);
                AT_CMD_Fill(AT_CMD_SET_SISC,stModuleRunState.ucCurCommChannel);                         //复位DTR脚进入命令状态
                DebugPrint((unsigned char*)STR_DEBUG_RECEIVE_TIMEROUT, sizeof(STR_DEBUG_RECEIVE_TIMEROUT));

            }
            stReceiveBuffer.usProcPtr = 0;
            stReceiveBuffer.usLength = 0;
            if(usDataLength <= CON_AT_MAX_REVEIVE_PACKAGE)      //changed by cfh 10-01-12
                memset(stReceiveBuffer.ucBuffer, 0, usDataLength);
            else
                memset(stReceiveBuffer.ucBuffer, 0, CON_AT_MAX_REVEIVE_PACKAGE);

            CMD_TimeOutProcess();               //防止命令一直超时
        }


        //字符接收超时或者长度超出最大的长度
        if( stReceiveBuffer.usSilenceCnt == 0 || usDataLength >= CON_AT_MAX_REVEIVE_PACKAGE)
        {
            stReceiveBuffer.ucIsGetVaild = 0;
            if(usDataLength < 2)    //至少两个字符
            {
                SCIBufferClearAll( &stAT_RX_BUFFER );
                return;
            }

            //长度过长只取一部分，正常不回发生，这里只是防止异常的出现
            if( usDataLength > CON_AT_MAX_REVEIVE_PACKAGE )
                usDataLength = CON_AT_MAX_REVEIVE_PACKAGE;

            //从串口接收缓冲区拷贝数据
            SCIBufferRead(&stAT_RX_BUFFER ,0, stReceiveBuffer.ucBuffer ,usDataLength);
            SCIBufferMoveHeadPtr(&stAT_RX_BUFFER , usDataLength);
        //  SCIBufferAdjust(stReceiveBuffer.ucBuffer,&usDataLength);//cfh 10-04-14s
            stReceiveBuffer.usLength  = usDataLength;
            stReceiveBuffer.usProcPtr = 0;
            DebugPrint(stReceiveBuffer.ucBuffer, usDataLength);

            if (stCMDBuffer.ucATCMD == AT_CMD_SET_SISR)
            {
                if(usDataLength < 15)
                {
                    memset(uctemp , 0x00, 20);//stReceiveBuffer.ucBuffer
                    memcpy(uctemp,&stReceiveBuffer.ucBuffer,20);
                    if(strstr((char*)uctemp , "ERROR\r\n")!=NULL)
                    {
                        ClearCmdBuffer();
                        AT_CMD_Clear();
                        trace_at_cmd_sn(8);
                        AT_CMD_Fill(AT_CMD_SET_SISC ,stModuleRunState.ucCurCommChannel);
                    }
                    else
                    {    
                        if(gucGprs_Module_Type == MC55_GPRS_MODULE)
                        {                           
                        AT_Receive_Process_SISR(stReceiveBuffer.ucBuffer);
                }
                        else if(gucGprs_Module_Type == TELIT_GPRS_MODULE)
                        {   
                            AT_Receive_Process_SISR_T(stReceiveBuffer.ucBuffer);
                        } 
                    }
                }
                else
                {    
                    if(gucGprs_Module_Type == MC55_GPRS_MODULE)
                    {                           
                    AT_Receive_Process_SISR(stReceiveBuffer.ucBuffer);
            }
                    else if(gucGprs_Module_Type == TELIT_GPRS_MODULE)
                    {   
                        AT_Receive_Process_SISR_T(stReceiveBuffer.ucBuffer);
                    } 
                }
            }
            else
                AT_Receive_Explain();

            stReceiveBuffer.usLength = 0;
            //          memset(stReceiveBuffer.ucBuffer, 0, usDataLength);
            if(usDataLength <= CON_AT_MAX_REVEIVE_PACKAGE)      //changed by cfh 10-01-13
                memset(stReceiveBuffer.ucBuffer, 0, usDataLength);
            else
                memset(stReceiveBuffer.ucBuffer, 0, CON_AT_MAX_REVEIVE_PACKAGE);
        }
    }


    else//未接收到数据
    {
        if(stAtManageState.uNoReceiveTimer == 0)
        {
            enuATCurrentState = enuATNextState = STATE_POWEROFF;
            AT_CMD_Clear();                                 //清除命令队列
            ClearCmdBuffer();
            AT_CMD_Fill(AT_CMD_SET_SMSO,0);                 //为了防止模块突然断电引起损坏，在关电前先对ON/OFF脚操作
            //AT_CMD_Fill(PIN_CMD_DELAY,30);
            AT_CMD_Fill(PIN_CMD_DELAY,AT_DLY_30S);
            AT_CMD_Fill(PIN_CMD_PWCTK,0);                   //模块电源关闭

            stAtManageState.uStateTimerCounter  = 600;//6000;     //
            stAtManageState.uNoReceiveTimer = 1200*600;//1200*6000;
        }

        if(stCMDBuffer.enuBufferState == STATE_BUFFER_WAIT)
            CMD_TimeOutProcess();
        else if (stCMDBuffer.enuBufferState == STATE_BUFFER_IDLE)
        {
            ucRet = IsATCmdFinish();
            if(ucRet>0)
                enuATCurrentState = enuATNextState;
        }
    }
}
/********************************************************************
函数名称:   AT_Receive_Explain
功能    :   AT命令解释函数。在接收到的字符串中\r\n最为判别标志
判别中间字符串类型，以及当前命令的类型，进行相应的处理
入口参数:
出口参数:   ENU_AT_RESULT
为最后一条检测到的有效回应类型
*********************************************************************/
ENU_AT_RESULT AT_Receive_Explain()
{
    unsigned char   *pBuf=NULL,mem_type;
    ENU_AT_RESULT   enuAtResult = AT_RESULT_UNKNOW;
    UN_CHANNEL_ADDR unAdrInfo;

    unsigned char   ucTmp = 0;
//TELIT_GPRS
	  unsigned char* pStr1;
//TELIT_GPRS
//    memset(ucTmpStr , 0x0, 360);//stReceiveBuffer.ucBuffer
//azh 131014
//    pBuf = alloc_comm_mem(MEM_1536_TYPE, &mem_type, GET_MEM_ERR29);//azh 120312
    pBuf = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR29);//azh 120312
    if(pBuf == NULL)
    {
		setmemgetnullid(GET_MEM_ERR0, gusCurMemErrFunName);
        return enuAtResult;
    }

    //141023 azh gprs模块版本判别
    if(stCMDBuffer.ucATCMD == AT_CMD_QUERY_GMR) 
    {
        if(stReceiveBuffer.usLength==25)
        {    
            gucGprs_Module_Type = MC55_GPRS_MODULE;
        }
        else if(stReceiveBuffer.usLength==19)
        {
            gucGprs_Module_Type = TELIT_GPRS_MODULE;    
        }
        else
        {
            gucGprs_Module_Type = TELIT_GPRS_MODULE; 
        }    
    }             
        
    while(GetStringFromStr(pBuf)>0)
    {

        enuAtResult = AT_CHECK_STRING(pBuf);
        switch(enuAtResult)
        {
        case AT_RESULT_OK:
            if(gucGprs_Module_Type == MC55_GPRS_MODULE)
            {             
            AT_Receive_Process_OK();
            }
            else if(gucGprs_Module_Type == TELIT_GPRS_MODULE)
            {  
                AT_Receive_Process_OK_T();
            } 
            break;
        case AT_RESULT_LINK_CLOSED:
                AT_Recvive_LinkClose(pBuf);
                break;
        case AT_RESULT_URC_SISR:
//TELIT_GPRS
            if(gucGprs_Module_Type == TELIT_GPRS_MODULE)
            { 
           pStr1=strstr((char*)&stReceiveBuffer.ucBuffer, "SRING:");
           stAtManageState.ucSISRParam=pStr1[7]-0x31;
            }
//TELIT_GPRS
            if(stModuleRunState.bIsGPRSConnected == 0x01)
            {
                //AT_CMD_Fill(PIN_CMD_DELAY_10MS , 2);
                AT_CMD_Fill(PIN_CMD_DELAY_10MS , AT_DLY_100MS);
                AT_CMD_Fill(AT_CMD_SET_SISR, stModuleRunState.ucCurCommChannel);          // 此处0需要改成当前通道
            }
            else
                ClearCmdBuffer();

            break;
        case AT_RESULT_SERVERLINK:
            AT_CMD_Fill(AT_CMD_SET_SA, 0);
            break;
//TELIT_GPRS
        case AT_RESULT_SISR:
            if(gucGprs_Module_Type == MC55_GPRS_MODULE)
            {                           
           AT_Receive_Process_SISR(stReceiveBuffer.ucBuffer);
            }
            else if(gucGprs_Module_Type == TELIT_GPRS_MODULE)
            {   
                AT_Receive_Process_SISR_T(stReceiveBuffer.ucBuffer);
            } 
          break;
//TELIT_GPRS
        case AT_RESULT_ERROR:
            AT_Receive_Process_ERROR();
            break;
        case AT_RESULT_CSQ:
            stModuleRunState.bIsSIMExist = 1;
            AT_Receive_Process_CSQ(pBuf);
            break;

        case AT_RESULT_URC_SMS:
            AT_CMD_Fill(AT_CMD_QUERY_CMGL, 0);
            break;

        case AT_RESULT_SMS:             //接收到短信
            //短信解码,这里需要去除字符串头部的\r\n
            ucTmp = AT_SMS_Decode(pBuf, stReceiveBuffer.ucBuffer, &(unAdrInfo.stSMSParam));
            if(ucTmp != 0 ) //解析短信成功
            {
                stAtManageState.uNoReceiveTimer = 1200*AT_DLY_60S;//6000;
                //将接收的数据添加到SCI任务队列
                ucLink_Time = AT_DLY_1S;//100;
                Rcomm_Fill_RecvData(CMD_SMSDATA_AUTO_UP, stReceiveBuffer.ucBuffer, ucTmp, &unAdrInfo);
            }
            break;

        case AT_RESULT_NEEDDATA:
            if( stCMDBuffer.ucATCMD == AT_CMD_SET_CMGS )
            {
                //从短信缓冲区拷贝数据到串口缓冲区，发送短信数据
                //SCIBufferWrite(&stAT_TX_BUFFER , stSMSBufer.ucData , stSMSBufer.usLength);
                //azh 110929
                Gprs_send_frame((unsigned char*)&stSMSBufer.ucData, stSMSBufer.usLength);
                DebugPrint((unsigned char*)&stSMSBufer.ucData, stSMSBufer.usLength);
                stSMSBufer.ucIsValid = 0;
            }
                else if(stCMDBuffer.ucATCMD == AT_CMD_SET_SISW)
                {
                    //
                    //AT_Receive_Process_SISW(pBuf);
                    if(gucGprs_Module_Type == MC55_GPRS_MODULE)
                    {            
                    AT_Receive_Process_SISW(pBuf);
                    }
                    else if(gucGprs_Module_Type == TELIT_GPRS_MODULE)
                    {             
                        AT_Receive_Process_SISW_T(pBuf);
                    }
                }
            break;

        case AT_RESULT_URC_SIS:
            AT_Receive_Process_URC_SIS(pBuf);

            break;

        case AT_RESULT_CMGL:
            AT_Receive_Process_CMGL(stReceiveBuffer.ucBuffer);
            break;

        case AT_RESULT_CPMS:
            if( stCMDBuffer.ucATCMD == AT_CMD_QUERY_CPMS)
            {
                AT_Receive_Process_CPMS(stReceiveBuffer.ucBuffer);
            }
            break;

        case AT_RESULT_CNMI:
            if( stCMDBuffer.ucATCMD == AT_CMD_QUERY_CNMI)
            {
                AT_Receive_Process_CNMI(stReceiveBuffer.ucBuffer);
            }
            break;


        case AT_RESULT_PIN:     //PIN锁定
            AT_CMD_Fill(AT_CMD_SET_UNLOCK_PIN,0);
            stModuleRunState.bIsPUKLock = 1;
            stAtManageState.bIsPWDUsed = 1;
            break;

        case AT_RESULT_PUK:     //PUK锁定
            AT_CMD_Fill(AT_CMD_SET_UNLOCK_PUK,0);
            stModuleRunState.bIsPUKLock = 1;
            stAtManageState.bIsPWDUsed = 1;
            break;

        case AT_RESULT_RING:                                            //有电话呼入
            stModuleRunState.bIsRing = 1;
            AT_CMD_Fill(AT_CMD_SET_CHUP,0);
        //  ucIsGetRing = 1;
            break;

        case AT_RESULT_CMS_ERROR:
            if( AT_Receive_Process_CMSERROR() ==1 )
                enuAtResult = AT_RESULT_CMS_ERROR;
            else
                enuAtResult = AT_RESULT_UNKNOW;
            break;

        case AT_RESULT_CREG:
            AT_Receive_Process_CGREG((char*)stReceiveBuffer.ucBuffer);  //未搜索到GPRS网络
            break;

        case AT_RESULT_CGREG:
            AT_Receive_Process_CGREG((char*)stReceiveBuffer.ucBuffer);  //未搜索到GPRS网络
            ClearCmdBuffer();
            break;

        case AT_RESULT_SISW:
            if(gucGprs_Module_Type == MC55_GPRS_MODULE)
            {            
            AT_Receive_Process_SISW(pBuf);
            }
            else if(gucGprs_Module_Type == TELIT_GPRS_MODULE)
            {             
                AT_Receive_Process_SISW_T(pBuf);
            }
            break;

        case AT_RESULT_SYSSTART:
            AT_CMD_Clear();
            ClearCmdBuffer();
            break;

        case AT_RESULT_SHUTDOWN:
            AT_CMD_Clear();
            ClearCmdBuffer();
            stAtManageState.uStateTimerCounter  = AT_DLY_1S;//100;      //
            enuATCurrentState = enuATNextState = STATE_POWEROFF;
            break;

        case AT_RESULT_UNKNOW:
            AT_Receive_Process_UNKNOWN((unsigned char *)pBuf);
            break;
/*
        case AT_RESULT_DATA_SEND:
            ClearCmdBuffer();
            stAtManageState.ucIsDataSending = 0;
            break;
*/
        case AT_RESULT_SMSO:
        case AT_RESULT_CME_ERROR:
        case AT_RESULT_CMGL_ERROR:
            break;

        case AT_RESULT_REVISION:
            if((stCMDBuffer.ucATCMD == AT_CMD_QUERY_GMR ) && (stReceiveBuffer.usLength > 0x05))
            {
                memcpy(&stModuleComParam.stModuleInfor.ucInformation,&stReceiveBuffer.ucBuffer,25);
                stModuleComParam.stModuleInfor.ucInforLength = 25;
            }
            break;
        case AT_RESULT_SGACT:
            if(stCMDBuffer.ucATCMD == AT_CMD_SET_SGACT)
                AT_Receive_Process_SGACT(stReceiveBuffer.ucBuffer);
            break;
        case AT_RESULT_BLANK:
            if( (stCMDBuffer.ucATCMD != AT_CMD_SET_CMGS) &&(stCMDBuffer.ucATCMD != AT_CMD_QUERY_CMGL)
                &&(stCMDBuffer.ucATCMD != AT_CMD_QUERY_CPIN) &&(stCMDBuffer.ucATCMD != AT_CMD_SET_SMSO) )
            {
                enuAtResult = AT_RESULT_UNKNOW;
                if((gucGprs_Module_Type != TELIT_GPRS_MODULE)||(stCMDBuffer.ucATCMD != AT_CMD_SET_SISW))//azh 141103 在模块上线后，拔下会出现很长时间不通信问题
                {    
                    ClearCmdBuffer(); 
                }
            }
            break;

        case AT_RESULT_CPIN:
            stModuleRunState.bIsSIMExist = 1;
            ClearCmdBuffer();
            break;

        case AT_RESULT_CMGS:
            ATDataListPop();
            ClearCmdBuffer();
            break;
//        case AT_RESULT_LINK_CLOSED:
//            AT_Recvive_LinkClose(pBuf);
//            break;
//      case AT_RESULT_SERVERLINK_CLOSED:
//          AT_Recvive_ServerLinkClose(pBuf);
//          break;
        case AT_RESULT_FORMATERROR:
            break;
        default:
            enuAtResult = AT_RESULT_UNKNOW;
            ClearCmdBuffer();
            break;
        }
    }
    //检测到电话呼入,发送挂机命令,待定
    //memset(ucTmpStr , 0x0, 500);
    free_mem_block(pBuf);

    return enuAtResult;
}

/********************************************************************
函数名称:   GetStringFromStr
功能    :   以\r\n作为标志，从字符串缓冲区获取单一字符串
入口参数:
出口参数:
*********************************************************************/

unsigned short GetStringFromStr(unsigned char* pucStr)
{
    unsigned short  usLen = 0;
    //  unsigned char   ucIsMached = 0;
    unsigned short  usdatalength = 0;

    if (stAtManageState.usDataReceiveLength > 1000 )
    {
        stAtManageState.usDataReceiveLength = 0;
        ClearCmdBuffer();
    }
    if (stAtManageState.usDataReceiveLength > 4 )
    {
        usdatalength = stAtManageState.usDataReceiveLength - 4;    //减4代表后面的 ok\r\n
    }
    else if(stAtManageState.usDataReceiveLength == 1)//azh debug
    {
        stAtManageState.usDataReceiveLength = 0;
    }

    //  if(usdatalength > 1000)
    //      NETLED_CLR;

    if(stAtManageState.usDataReceiveLength == 0)
    {
        if (stReceiveBuffer.usLength > stReceiveBuffer.usProcPtr)
        {
            pucStr[usLen++] = stReceiveBuffer.ucBuffer[stReceiveBuffer.usProcPtr];
            stReceiveBuffer.usProcPtr++;
        }
        if (stReceiveBuffer.usLength > stReceiveBuffer.usProcPtr)
        {
            pucStr[usLen++] = stReceiveBuffer.ucBuffer[stReceiveBuffer.usProcPtr];
            stReceiveBuffer.usProcPtr++;
        }

        if(stReceiveBuffer.ucBuffer[stReceiveBuffer.usProcPtr] == '>' )//短信
        {
            pucStr[usLen++] = stReceiveBuffer.ucBuffer[stReceiveBuffer.usProcPtr];
            stReceiveBuffer.usProcPtr++;
        }
        else if (stReceiveBuffer.usLength > stReceiveBuffer.usProcPtr)
        {
            pucStr[usLen++] = stReceiveBuffer.ucBuffer[stReceiveBuffer.usProcPtr];
            stReceiveBuffer.usProcPtr++;

            while(stReceiveBuffer.usLength > stReceiveBuffer.usProcPtr)
            {
                if(stReceiveBuffer.ucBuffer[stReceiveBuffer.usProcPtr] ==  '\r' &&
                    stReceiveBuffer.ucBuffer[stReceiveBuffer.usProcPtr+1] ==  '\n')
                {//字串以\r\n开始，以\r\n结束
                    pucStr[usLen++] = stReceiveBuffer.ucBuffer[stReceiveBuffer.usProcPtr];
                    pucStr[usLen++] = stReceiveBuffer.ucBuffer[stReceiveBuffer.usProcPtr+1];
                    stReceiveBuffer.usProcPtr +=2;
                    //  DebugPrint((unsigned char *)STR_DEBUG_RECEIVE_NOT_DATA,sizeof (STR_DEBUG_RECEIVE_NOT_DATA));
                    break;
                }
                else
                {
                    pucStr[usLen++] = stReceiveBuffer.ucBuffer[stReceiveBuffer.usProcPtr];
                    stReceiveBuffer.usProcPtr++;
                }
                if(usLen>=320)
                    break;
            }
        }
    }
    else if( usdatalength > 0 && usdatalength < 750)
    {
        while ( usdatalength > 0 )
        {
            if(stReceiveBuffer.usProcPtr >= stReceiveBuffer.usLength)
                break;

            pucStr[usLen++] = stReceiveBuffer.ucBuffer[stReceiveBuffer.usProcPtr];
            stReceiveBuffer.usProcPtr++;
            //  if(stReceiveBuffer.usProcPtr >= 750)
            //      break;

            usdatalength--;

            if (usdatalength == 0)
                break;
        }
        DebugPrint((unsigned char *)STR_DEBUG_RECEIVE_DATA,sizeof (STR_DEBUG_RECEIVE_DATA));

    }
    pucStr[usLen+1] = 0x00;

    //  DebugPrint((unsigned char *)STR_DEBUG_RECEIVE_STRING,sizeof (STR_DEBUG_RECEIVE_STRING));
    //  DebugPrint(pucStr,  usLen);

    return usLen;
}

/********************************************************************
函数名称:   AT_CHECK_STRING
功能    :   字符串类型判别
入口参数:
pucSrc  :   待判别字符串指针
出口参数:
字符串类型
*********************************************************************/

ENU_AT_RESULT AT_CHECK_STRING(unsigned char* pucSrc)
{
    ENU_AT_RESULT enuRet = AT_RESULT_UNKNOW;

    //返回字串中查找特征字符
    //  if(strstr((char*)pucSrc , "OK\r\n")!=NULL)
    //      enuRet = AT_RESULT_OK;
    if (strstr((char*)pucSrc , "NO CARRIER")!=NULL)
        enuRet = AT_RESULT_LINK_CLOSED;
    else if (strstr((char*)pucSrc , "ABORTED")!=NULL)
        enuRet = AT_RESULT_SERVERLINK_CLOSED;
    else if(strstr((char*)pucSrc , "#SGACT:")!=NULL)
        enuRet = AT_RESULT_SGACT;
    else if(strstr((char*)pucSrc , "+CSQ")!=NULL)
        enuRet = AT_RESULT_CSQ;
    else if(strstr((char*)pucSrc , "+CREG")!=NULL)
        enuRet = AT_RESULT_CREG;

    else if(strstr((char*)pucSrc , "CMTI")!=NULL)
        enuRet = AT_RESULT_URC_SMS;

    else if(strstr((char*)pucSrc , "ERROR\r\n")!=NULL)
        enuRet = AT_RESULT_ERROR;

    else if(strstr((char*)pucSrc , "+CMS ERROR:")!=NULL)
        enuRet = AT_RESULT_CMS_ERROR;

    else if(strstr((char*)pucSrc , "+CME ERROR:")!=NULL)
        enuRet = AT_RESULT_CME_ERROR;

    else if (strstr((char*)pucSrc  , "^SISR:")!=NULL)
        enuRet = AT_RESULT_URC_SISR;

//  else if ((strstr((char*)pucSrc  , "SRING:")!=NULL)&& (strstr((char*)pucSrc  , ",")!=NULL))
//      enuRet = AT_RESULT_URC_SISR;
//  else if (strstr((char*)pucSrc  , "SRING:")!=NULL)
//      enuRet = AT_RESULT_SERVERLINK;
//  else if (strstr((char*)pucSrc  , "#SRECV:")!=NULL)
//      enuRet = AT_RESULT_SISR;
//TELIT_GPRS
  else if (strstr((char*)pucSrc  , "SRING:")!=NULL)    //zk add
      enuRet = AT_RESULT_URC_SISR;
  else if (strstr((char*)pucSrc  , "#SRECV:")!=NULL)
      enuRet = AT_RESULT_SISR;
//TELIT_GPRS
    else if(strstr((char*)pucSrc , "^SISW:")!=NULL)
        enuRet = AT_RESULT_SISW;

    else if(strstr((char*)pucSrc , "RING")!=NULL)
        enuRet = AT_RESULT_RING;

    else if(strstr((char*)pucSrc , "SIS:")!=NULL)
        enuRet = AT_RESULT_URC_SIS;

    else if(strstr((char*)pucSrc , "+CMGL:")!=NULL)
        enuRet = AT_RESULT_CMGL;

    else if(strstr((char*)pucSrc , "+CPMS:")!=NULL)
        enuRet = AT_RESULT_CPMS;

    else if(strstr((char*)pucSrc , "+CNMI:")!=NULL)
        enuRet = AT_RESULT_CNMI;

    else if(strstr((char*)pucSrc, ">")!=NULL)   //短信
        enuRet = AT_RESULT_NEEDDATA;

    else if(strstr((char*)pucSrc , "0891")!=NULL)
        enuRet = AT_RESULT_SMS;

    else if(strstr((char*)pucSrc , "+CPIN: SIM PIN BLOCKED")!=NULL)
        enuRet = AT_RESULT_PUK;

    else if(strstr((char*)pucSrc , "+CPIN: SIM PUK")!=NULL)
        enuRet = AT_RESULT_PUK;

    else if(strstr((char*)pucSrc , "+CPIN: SIM PIN")!=NULL)
        enuRet = AT_RESULT_PIN;

    else if(strstr((char*)pucSrc , "+CPIN: READY")!=NULL)
        enuRet = AT_RESULT_CPIN;

    else if(strstr((char*)pucSrc , "REVISION")!=NULL)
        enuRet = AT_RESULT_REVISION;

    else if(strstr((char*)pucSrc , "+CGREG:")!=NULL)
        enuRet = AT_RESULT_CGREG;

    else if (strstr((char*)pucSrc , "+^SYSSTART")!=NULL)
        enuRet = AT_RESULT_SYSSTART;

    else if(strstr((char*)pucSrc , "^SHUTDOWN")!=NULL)
        enuRet = AT_RESULT_SHUTDOWN;

    else if (strstr((char*)pucSrc , "^SMSO: MS OFF")!=NULL)
        enuRet = AT_RESULT_SMSO;

    else if (strstr((char*)pucSrc , "+CMGS:")!=NULL)
        enuRet = AT_RESULT_CMGS;

    else if(strstr((char*)pucSrc , "OK\r\n")!=NULL)
        enuRet = AT_RESULT_OK;

    else if(strstr((char*)pucSrc , "\r\n")!=NULL)
        enuRet = AT_RESULT_BLANK;

    else
        enuRet = AT_RESULT_UNKNOW;
    return enuRet;
}


/********************************************************************
函数名称:   AT_Send_FillBuffer
功能    :   从AT命令队列中取出最早的命令，解释成字串填写发送缓冲区
入口参数:
出口参数:
*********************************************************************/

unsigned char AT_Send_FillBuffer()
{
    unsigned short  usATCmd = 0;
    unsigned char   ucTmp = 0;

    ucTmp = AT_CMD_Get(&usATCmd);
    if(ucTmp == 0xff)   //空
        return 0;

    stCMDBuffer.ucATCMD = usATCmd;
    stCMDBuffer.ucParam = stATCmdList.ucCmdParam[ucTmp];

    if((usATCmd == AT_CMD_SET_UNLOCK_PIN) || (usATCmd == AT_CMD_SET_UNLOCK_PUK) ||
        (usATCmd == PIN_CMD_EMERGOFF) || (usATCmd == PIN_CMD_DELAY) || (usATCmd == PIN_CMD_DELAY_10MS))
        stCMDBuffer.ucRetryCnt = 1;         //重发一次
    else
        stCMDBuffer.ucRetryCnt = 3;         //重发三次

    stCMDBuffer.enuBufferState = STATE_BUFFER_SEND;
    stCMDBuffer.uTimeOutCount = CON_AT_TIMEOUT_MAX;

    AT_Send_CMD_EXPLAIN(ucTmp);
    if( stCMDBuffer.ucLength == 0)
        return 0;
    return 1;
}


/********************************************************************
函数名称:   AT_Send_CMD_EXPLAIN
功能    :   将当前命令缓冲区中的有效命令解释为字符串，填写入命令缓
冲区的字符串缓冲区
入口参数:
ucPara  :   命令附加参数，用于短信删除等命令
出口参数:
*********************************************************************/
void AT_Send_CMD_EXPLAIN(unsigned char ucPara)
{
//  STLOCALIP stLocalIP2;
//MC55_GPRS
  PSTTC35TASK pstDataContent = NULL;
    unsigned char i=0;
  unsigned short datalength;
  unsigned char ucDatabuf[6];
//MC55_GPRS	

//TELIT_GPRS
    unsigned char m,n;
//TELIT_GPRS

//    unsigned char m=0,n=0;

    unsigned char ucIP[15];
    unsigned char ucPort[5];
//    stTerRunPara.stLocalIP= RunPara_GetLocalIP();
    memset(ucIP,0,15);
    memset(ucPort,0,5);

    stCMDBuffer.ucLength = 0;

    switch(stCMDBuffer.ucATCMD)
    {
    case AT_CMD_SET_AT:
        stCMDBuffer.ucLength = sizeof(STR_AT_CMD_AT);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_CMD_AT , sizeof(STR_AT_CMD_AT));
        break;
    case AT_CMD_SET_ATE0:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_ATE0);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_ATE0 , sizeof(STR_AT_SET_ATE0));
        break;
    case AT_CMD_SET_FLO:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_FLO);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_FLO , sizeof(STR_AT_SET_FLO));
        break;
//TELIT_GPRS
    case AT_CMD_SET_FRS:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_FRS);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_FRS , sizeof(STR_AT_SET_FRS));
        break;
//TELIT_GPRS
    case AT_CMD_SET_CNMI:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_CNMI);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_CNMI , sizeof(STR_AT_SET_CNMI));
        break;

    case AT_CMD_SET_IPR:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_IPR);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_IPR , sizeof(STR_AT_SET_IPR));
        break;

    case AT_CMD_SET_CREG:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_CREG);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_CREG , sizeof(STR_AT_SET_CREG));
        break;
 /*
    case AT_CMD_QUERY_CREG:
        stCMDBuffer.ucLength = sizeof(STR_AT_QUERY_CREG);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_QUERY_CREG , sizeof(STR_AT_QUERY_CREG));
        stCMDBuffer.uTimeOutCount=AT_DLY_3S;//3*100;
        break;
 */
    case AT_CMD_SET_CHUP:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_CHUP);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_CHUP , sizeof(STR_AT_SET_CHUP));
        break;

    case AT_CMD_QUERY_CSCA:
        stCMDBuffer.ucLength = sizeof(STR_AT_QUERY_CSCA);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_QUERY_CSCA , sizeof(STR_AT_QUERY_CSCA));
        break;

    case AT_CMD_QUERY_CSQ:
        stCMDBuffer.ucLength = sizeof(STR_AT_QUERY_CSQ);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_QUERY_CSQ , sizeof(STR_AT_QUERY_CSQ));
        break;

    case AT_CMD_SET_CFUN:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_CFUN);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_CFUN , sizeof(STR_AT_SET_CFUN));

        stCMDBuffer.uTimeOutCount = AT_DLY_30S;//30*100;

        break;

    case AT_CMD_SET_CMEE:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_CMEE);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_CMEE , sizeof(STR_AT_SET_CMEE));
        break;
//MC55_GPRS
    case AT_CMD_SET_SGCONF:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_SGCONF);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_SGCONF , sizeof(STR_AT_SET_SGCONF));
        break;

    case AT_CMD_QUERY_SGCONF:
        stCMDBuffer.ucLength = sizeof(STR_AT_QUERY_SGCONF);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_QUERY_SGCONF , sizeof(STR_AT_QUERY_SGCONF));
        break;
//MC55_GPRS
    case AT_CMD_SET_CGATT_DETACH:
//      stCMDBuffer.ucLength = sizeof(STR_AT_SET_DESGACT);
//      memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_DESGACT , sizeof(STR_AT_SET_DESGACT));
        //      stCMDBuffer.uTimeOutCount=12*100;   //断开网络命令超时12s

        stCMDBuffer.ucLength = sizeof(STR_AT_SET_CGATT_DETACH);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_CGATT_DETACH , sizeof(STR_AT_SET_CGATT_DETACH));
        //      stCMDBuffer.uTimeOutCount=12*100;   //断开网络命令超时12s
//        stCMDBuffer.uTimeOutCount=60*100;   //断开网络命令超时60s
        stCMDBuffer.uTimeOutCount=AT_DLY_60S;//60*100;  //断开网络命令超时60s
        break;
/* //telit
    case AT_CMD_SET_CGATT_DESOC:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_CGATT_DESOC);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_CGATT_DESOC , sizeof(STR_AT_SET_CGATT_DESOC));
        //      stCMDBuffer.uTimeOutCount=12*100;   //断开网络命令超时12s
        stCMDBuffer.uTimeOutCount=AT_DLY_10S;//10*100;  //断开网络命令超时60s
        break;
    case AT_CMD_SET_CGATT_OFFSERVER:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_CGATT_OFFSERVER);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_CGATT_OFFSERVER , sizeof(STR_AT_SET_CGATT_OFFSERVER));
        //      stCMDBuffer.uTimeOutCount=12*100;   //断开网络命令超时12s
        stCMDBuffer.uTimeOutCount=AT_DLY_10S;//10*100;  //断开网络命令超时60s
        break;
*/
    case AT_CMD_SET_CGATT_TACH:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_CGATT_TACH);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_CGATT_TACH , sizeof(STR_AT_SET_CGATT_TACH));
        //      stCMDBuffer.uTimeOutCount=12*100;   //断开网络命令超时12s
        stCMDBuffer.uTimeOutCount=AT_DLY_60S;//60*100;  //断开网络命令超时
        break;

//TELIT_GPRS
    case AT_CMD_SET_GPRS:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_GPRS);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_GPRS , sizeof(STR_AT_SET_GPRS));
        stCMDBuffer.uTimeOutCount=AT_DLY_3S;//3*100;
        break;
//TELIT_GPRS
/*

    case AT_CMD_SET_SGACT:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_SGACT);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_SGACT , sizeof(STR_AT_SET_SGACT));
        stCMDBuffer.uTimeOutCount=AT_DLY_10S;//10*100;
        break;
    case AT_CMD_SET_DESGACT:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_DESGACT);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_DESGACT , sizeof(STR_AT_SET_DESGACT));
        stCMDBuffer.uTimeOutCount=AT_DLY_5S;//5*100;
        break;

    case AT_CMD_SET_FRWL:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_FRWL);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_FRWL , sizeof(STR_AT_SET_FRWL));
        break;
*/
//TELIT_GPRS
    case AT_CMD_SET_SCFGEXT:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_SCFGEXT);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_SCFGEXT , sizeof(STR_AT_SET_SCFGEXT));
        stCMDBuffer.ucCMDBuffer[11] = stCMDBuffer.ucParam+0x31;
        break;

    case AT_CMD_SET_SCFG:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_SCFG);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_SCFG, sizeof(STR_AT_SET_SCFG));
        stCMDBuffer.ucCMDBuffer[8] = stCMDBuffer.ucParam+0x31;
        break;
//TELIT_GPRS
/*
    case AT_CMD_SET_SL:
//      stCMDBuffer.ucLength = sizeof(STR_AT_SET_SL);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_SL, sizeof(STR_AT_SET_SL));
        n= IPportchange2(stTerRunPara.stLocalIP.stLocalIP.usIPPort,ucPort);
        memcpy(&(stCMDBuffer.ucCMDBuffer[10]), (unsigned char * )&ucPort, n);
        memcpy(&(stCMDBuffer.ucCMDBuffer[10+n]) , (unsigned char*)STR_AT_SET_SL2, sizeof(STR_AT_SET_SL2));
        stCMDBuffer.ucLength = 10+n+4;
        stCMDBuffer.uTimeOutCount = AT_DLY_10S;//10*100;
        break;
    case AT_CMD_SET_SA:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_SA);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_SA, sizeof(STR_AT_SET_SA));
        break;
*/
    case AT_CMD_SET_CSCA:
        /**********暂时不加代码******************/
        break;

    case AT_CMD_QUERY_CPMS:
        stCMDBuffer.ucLength = sizeof(STR_AT_QUERY_CPMS);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_QUERY_CPMS , sizeof(STR_AT_QUERY_CPMS));
        break;

    case AT_CMD_QUERY_CNMI:
        stCMDBuffer.ucLength = sizeof(STR_AT_QUERY_CNMI);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_QUERY_CNMI , sizeof(STR_AT_QUERY_CNMI));
        break;


    case AT_CMD_QUERY_GMR:
        stCMDBuffer.ucLength = sizeof(STR_AT_QUERY_GMR);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_QUERY_GMR , sizeof(STR_AT_QUERY_GMR));
        stCMDBuffer.uTimeOutCount=AT_DLY_3S;//3*100;
        break;

    case AT_CMD_QUERY_CPIN:             //查询密码状态
        stCMDBuffer.ucLength = sizeof(STR_AT_QUERY_CPIN);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_QUERY_CPIN , sizeof(STR_AT_QUERY_CPIN));
        stCMDBuffer.uTimeOutCount = AT_DLY_10S;//10*100;
        break;

    case AT_CMD_SET_UNLOCK_PIN:         //解锁PIN
        stCMDBuffer.ucLength = sizeof(STR_AT_UNLOCK_PIN);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_UNLOCK_PIN , sizeof(STR_AT_UNLOCK_PIN));
        memcpy(&(stCMDBuffer.ucCMDBuffer[9]), stModuleComParam.stPINPUK.ucPinPwd , 4);
        stCMDBuffer.ucRetryCnt = 1;     //无重发
        break;

    case AT_CMD_SET_UNLOCK_PUK:         //解锁PUK
        stCMDBuffer.ucLength = sizeof(STR_AT_UNLOCK_PUK);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_UNLOCK_PUK , sizeof(STR_AT_UNLOCK_PUK));
        memcpy(&(stCMDBuffer.ucCMDBuffer[9]), stModuleComParam.stPINPUK.ucPukPwd , 8);
        memcpy(&(stCMDBuffer.ucCMDBuffer[20]), stModuleComParam.stPINPUK.ucPinPwd , 4);
        stCMDBuffer.ucRetryCnt = 1;     //无重发
        break;

    case AT_CMD_SET_CPWD:           //更改密码
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_CPWD);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_CPWD , sizeof(STR_AT_SET_CPWD));
        memcpy(&(stCMDBuffer.ucCMDBuffer[14]), stModuleComParam.stPINPUK.ucPinPwd , 4);
        memcpy(&(stCMDBuffer.ucCMDBuffer[21]), stModuleComParam.stPINPUK.ucPinPwdOld , 4);
        break;

    case AT_CMD_SET_CLCK0:          //解除密码保护
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_CLCK0);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_CLCK0 , sizeof(STR_AT_SET_CLCK0));
        memcpy(&(stCMDBuffer.ucCMDBuffer[16]), stModuleComParam.stPINPUK.ucPinPwd , 4);
        break;

    case AT_CMD_SET_CLCK1:          //开启密码保护
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_CLCK1);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_CLCK1 , sizeof(STR_AT_SET_CLCK1));
        memcpy(&(stCMDBuffer.ucCMDBuffer[16]), stModuleComParam.stPINPUK.ucPinPwd , 4);
        break;
    case AT_CMD_SET_ATQ3:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_ATQ3);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_ATQ3 , sizeof(STR_AT_SET_ATQ3));
        break;


    case AT_CMD_SET_CMGF:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_CMGF);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_CMGF , sizeof(STR_AT_SET_CMGF));
        break;

    case AT_CMD_SET_CPMS:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_CPMS);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_CPMS , sizeof(STR_AT_SET_CPMS));
        break;

    case AT_CMD_SET_SM20:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_SM20);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_SM20 , sizeof(STR_AT_SET_SM20));
        break;

    case AT_CMD_SET_SSMSS:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_SSMSS);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_SSMSS , sizeof(STR_AT_SET_SSMSS));
        break;
    case AT_CMD_SET_CMGD:       //const char STR_AT_SET_CMGD[]={"AT+CMGD=  \r\n"};
                                /*      memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_CMGD , sizeof(STR_AT_SET_CMGD));
                                if(stCMDBuffer.ucParam > 9)
                                {
                                stCMDBuffer.ucCMDBuffer[9] = '0' + stCMDBuffer.ucParam%10 ;
                                stCMDBuffer.ucParam /= 10;
                                }
                                stCMDBuffer.ucCMDBuffer[8] = '0' + stCMDBuffer.ucParam%10 ;
                                stCMDBuffer.ucLength = sizeof(STR_AT_SET_CMGD);
                                break;
                                stCMDBuffer.ucLength = sizeof(STR_AT_SET_CMGD);
                                memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_CMGD , sizeof(STR_AT_SET_CMGD));
                                ByteToBCDAssiic(&stCMDBuffer.ucCMDBuffer[8], stCMDBuffer.ucParam);
        break;*/
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_CMGD , sizeof(STR_AT_SET_CMGD));
        if(stCMDBuffer.ucParam > 9)
        {
            stCMDBuffer.ucCMDBuffer[9] = '0' + stCMDBuffer.ucParam%10 ;
            stCMDBuffer.ucParam /= 10;
        }
        stCMDBuffer.ucCMDBuffer[8] = '0' + stCMDBuffer.ucParam%10 ;
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_CMGD);
        break;

    case AT_CMD_QUERY_CMGL:
        stCMDBuffer.ucLength = sizeof(STR_AT_QUERY_CMGL);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_QUERY_CMGL , sizeof(STR_AT_QUERY_CMGL));
        stCMDBuffer.uTimeOutCount = AT_DLY_5S;//5*100;
        break;

    case AT_CMD_SET_CMGS:           //发送短信
        if( AT_CMD_Process_SendSMS() ==0 )                  //短消息解释失败则CMGS命令不发送
        {
            stCMDBuffer.ucLength = 0;
            stCMDBuffer.uTimeOutCount = 0;
        }
        else
            stCMDBuffer.uTimeOutCount = CON_SMS_TIMEOUT_MAX;    //时间延长
        break;

    case AT_CMD_SET_SICS_GPRS0:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_SICS_GPRS0);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_SICS_GPRS0 , sizeof(STR_AT_SET_SICS_GPRS0));
        break;

    case AT_CMD_SET_SICS_APN:
        if(gucGprs_Module_Type == MC55_GPRS_MODULE)
        {    
        memcpy(stCMDBuffer.ucCMDBuffer, (unsigned char * )STR_AT_SET_SICS_APN, 14);
        memcpy(&(stCMDBuffer.ucCMDBuffer[14]),(unsigned char * )&(stModuleComParam.stCommonParam.cAPN),stModuleComParam.stCommonParam.ucAPNLen);
        memcpy(&(stCMDBuffer.ucCMDBuffer[14+stModuleComParam.stCommonParam.ucAPNLen]), (unsigned char * )STR_AT_CMD_RETURN ,2);
        stCMDBuffer.ucLength = 14+stModuleComParam.stCommonParam.ucAPNLen+2;
        }
        else if(gucGprs_Module_Type == TELIT_GPRS_MODULE)
        {    
//telit
            memcpy(stCMDBuffer.ucCMDBuffer, (unsigned char * )STR_AT_SET_SICS_APN_T, 19);
        memcpy(&(stCMDBuffer.ucCMDBuffer[19]),(unsigned char * )&(stModuleComParam.stCommonParam.cAPN),stModuleComParam.stCommonParam.ucAPNLen);
        memcpy(&(stCMDBuffer.ucCMDBuffer[19+stModuleComParam.stCommonParam.ucAPNLen]), (unsigned char * )STR_AT_CMD_RETURN2 ,3);
        stCMDBuffer.ucLength = 19+stModuleComParam.stCommonParam.ucAPNLen+3;
        }
        break;
        
//MC55_GPRS
    case AT_CMD_SET_SICS_USER:
        memcpy(stCMDBuffer.ucCMDBuffer, (unsigned char * )STR_AT_SET_SICS_USER, 15);

        if(stModuleComParam.stPDPAccount.ucUserNameLen == 32)
            stModuleComParam.stPDPAccount.ucUserNameLen = 31;

        //      Character_Replace(stModuleComParam.stPDPAccount.ucUserName,stModuleComParam.stPDPAccount.ucUserNameLen);
        memcpy(&(stCMDBuffer.ucCMDBuffer[15]),(unsigned char *)stModuleComParam.stPDPAccount.ucUserName,stModuleComParam.stPDPAccount.ucUserNameLen);
        memcpy(&(stCMDBuffer.ucCMDBuffer[15+stModuleComParam.stPDPAccount.ucUserNameLen]), (unsigned char * )STR_AT_CMD_RETURN ,2);
        stCMDBuffer.ucLength  = 15+stModuleComParam.stPDPAccount.ucUserNameLen+2;
        break;

    case AT_CMD_SET_SICS_PASSWD:
        memcpy(stCMDBuffer.ucCMDBuffer, (unsigned char * )STR_AT_SET_SICS_PASSWD, 17);
        if(stModuleComParam.stPDPAccount.ucPwdLen == 32)
            stModuleComParam.stPDPAccount.ucPwdLen = 31;

        //             Character_Replace(stModuleComParam.stPDPAccount.ucPwd,stModuleComParam.stPDPAccount.ucPwdLen);
        memcpy(&(stCMDBuffer.ucCMDBuffer[17]),(unsigned char *)stModuleComParam.stPDPAccount.ucPwd,stModuleComParam.stPDPAccount.ucPwdLen);
        memcpy(&(stCMDBuffer.ucCMDBuffer[17+stModuleComParam.stPDPAccount.ucPwdLen]), (unsigned char * )STR_AT_CMD_RETURN ,2);
        stCMDBuffer.ucLength  = 17+stModuleComParam.stPDPAccount.ucPwdLen+2;
        break;

    case AT_CMD_SET_SISS_SRVTYPE_SOCKET:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_SISS_SRVTYPE_SOCKET);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_SISS_SRVTYPE_SOCKET , sizeof(STR_AT_SET_SISS_SRVTYPE_SOCKET));
        stCMDBuffer.ucCMDBuffer[8] = stCMDBuffer.ucParam+0x30;
        break;
//
    case AT_CMD_SET_SISS_ADDRESS_TCP:
        if(gucGprs_Module_Type == MC55_GPRS_MODULE)
        {   
//如果IP地址地址成:111.111.111.111则会启用zhejianghuacai.3322.org:PORT动态域名
        if(0 == memcmp((unsigned char * )&(stModuleComParam.stCh[stModuleRunState.ucCurCommChannel].unChInfo.stIPParam.ucIP), HUACAI_DNS_FLAG, 15))
        {
            stCMDBuffer.ucLength = sizeof(STR_AT_SET_SISS_DNS_ADDRESS_TCP);
            memcpy(stCMDBuffer.ucCMDBuffer, (unsigned char *) STR_AT_SET_SISS_DNS_ADDRESS_TCP, sizeof(STR_AT_SET_SISS_DNS_ADDRESS_TCP));
            stCMDBuffer.ucCMDBuffer[8] = stCMDBuffer.ucParam+0x30;
            memcpy(&(stCMDBuffer.ucCMDBuffer[28]), HUACAI_DNS, 23);
            memcpy(&(stCMDBuffer.ucCMDBuffer[52]), (unsigned char * )&(stModuleComParam.stCh[stModuleRunState.ucCurCommChannel].unChInfo.stIPParam.ucPort), 5);
        }
        else
        {
            stCMDBuffer.ucLength = sizeof(STR_AT_SET_SISS_ADDRESS_TCP);
            memcpy(stCMDBuffer.ucCMDBuffer, (unsigned char *) STR_AT_SET_SISS_ADDRESS_TCP, sizeof(STR_AT_SET_SISS_ADDRESS_TCP));
            stCMDBuffer.ucCMDBuffer[8] = stCMDBuffer.ucParam+0x30;
            memcpy(&(stCMDBuffer.ucCMDBuffer[28]), (unsigned char * )&(stModuleComParam.stCh[stModuleRunState.ucCurCommChannel].unChInfo.stIPParam.ucIP), 15);
            memcpy(&(stCMDBuffer.ucCMDBuffer[44]), (unsigned char * )&(stModuleComParam.stCh[stModuleRunState.ucCurCommChannel].unChInfo.stIPParam.ucPort), 5);
        }
        }
        else if(gucGprs_Module_Type == TELIT_GPRS_MODULE)
        { 
    //      stCMDBuffer.ucLength = sizeof(STR_AT_SET_SISS_ADDRESS_TCP_T);
            memcpy(stCMDBuffer.ucCMDBuffer, (unsigned char *) STR_AT_SET_SISS_ADDRESS_TCP_T, 10);
        stCMDBuffer.ucCMDBuffer[6] = stCMDBuffer.ucParam+0x31;
        n= IPportchange(stModuleRunState.ucCurCommChannel,ucPort);
        memcpy(&(stCMDBuffer.ucCMDBuffer[10]), (unsigned char * )&ucPort, n);
        stCMDBuffer.ucCMDBuffer[10+n]=0x2C;
        stCMDBuffer.ucCMDBuffer[10+n+1]=0x22;
        m= IPchange(stModuleRunState.ucCurCommChannel,ucIP);
        memcpy(&(stCMDBuffer.ucCMDBuffer[12+n]), (unsigned char * )&ucIP, m);
        memcpy(&(stCMDBuffer.ucCMDBuffer[12+m+n]), (unsigned char * )STR_AT_CMD_RETURN3 ,sizeof(STR_AT_CMD_RETURN3));
        stCMDBuffer.ucLength=12+m+n+sizeof(STR_AT_CMD_RETURN3);
        stCMDBuffer.uTimeOutCount = AT_DLY_80S;//80*100;
        }
        break;

    case AT_CMD_SET_SISS_ADDRESS_UDP:
        if(gucGprs_Module_Type == MC55_GPRS_MODULE)
        {   
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_SISS_ADDRESS_UDP);
        memcpy(stCMDBuffer.ucCMDBuffer, (unsigned char *) STR_AT_SET_SISS_ADDRESS_UDP, sizeof(STR_AT_SET_SISS_ADDRESS_UDP));
        stCMDBuffer.ucCMDBuffer[8] = stCMDBuffer.ucParam+0x30;
        memcpy(&(stCMDBuffer.ucCMDBuffer[28]), (unsigned char * )&(stModuleComParam.stCh[stModuleRunState.ucCurCommChannel].unChInfo.stIPParam.ucIP), 15);
        memcpy(&(stCMDBuffer.ucCMDBuffer[44]), (unsigned char * )&(stModuleComParam.stCh[stModuleRunState.ucCurCommChannel].unChInfo.stIPParam.ucPort), 5);
        }
        else if(gucGprs_Module_Type == TELIT_GPRS_MODULE)
        {         
    //telit
            memcpy(stCMDBuffer.ucCMDBuffer, (unsigned char *) STR_AT_SET_SISS_ADDRESS_UDP_T, 10);
            stCMDBuffer.ucCMDBuffer[6] = stCMDBuffer.ucParam+0x31;
            n= IPportchange(stModuleRunState.ucCurCommChannel,ucPort);
            memcpy(&(stCMDBuffer.ucCMDBuffer[10]), (unsigned char * )&ucPort, n);
            stCMDBuffer.ucCMDBuffer[10+n]=0x2C;
            stCMDBuffer.ucCMDBuffer[10+n+1]=0x22;
            m= IPchange(stModuleRunState.ucCurCommChannel,ucIP);
            memcpy(&(stCMDBuffer.ucCMDBuffer[12+n]), (unsigned char * )&ucIP, m);
            memcpy(&(stCMDBuffer.ucCMDBuffer[12+m+n]), (unsigned char * )STR_AT_CMD_RETURN3 ,sizeof(STR_AT_CMD_RETURN3));
            stCMDBuffer.ucLength=12+m+n+sizeof(STR_AT_CMD_RETURN3);
            stCMDBuffer.uTimeOutCount = AT_DLY_80S;//80*100;
        }
        break;
//MC55_GPRS
    case AT_CMD_SET_SISS_CONID:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_SISS_CONID);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_SISS_CONID , sizeof(STR_AT_SET_SISS_CONID));
        stCMDBuffer.ucCMDBuffer[8] = stCMDBuffer.ucParam+0x30;

        break;

    case AT_CMD_SET_SISS_ALPHABETAT:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_SISS_ALPHABETAT);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_SISS_ALPHABETAT , sizeof(STR_AT_SET_SISS_ALPHABETAT));
        stCMDBuffer.ucCMDBuffer[8] = stCMDBuffer.ucParam+0x30;
        break;

    case AT_CMD_SET_SISO:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_SISO);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_SISO , sizeof(STR_AT_SET_SISO));
        stCMDBuffer.ucCMDBuffer[8] = stCMDBuffer.ucParam+0x30;
        stCMDBuffer.uTimeOutCount = AT_DLY_30S;//30*100;

        break;

    case AT_CMD_SET_SISC:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_SISC);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_SISC, sizeof(STR_AT_SET_SISC));
        stCMDBuffer.ucCMDBuffer[8] = stCMDBuffer.ucParam+0x30;
        break;
//MC55_GPRS
    case AT_CMD_SET_SISR:
        if(gucGprs_Module_Type == MC55_GPRS_MODULE)
        {   
//MC55_GPRS
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_SISR);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_SISR , sizeof(STR_AT_SET_SISR));
        stCMDBuffer.ucCMDBuffer[8] = stCMDBuffer.ucParam+0x30 ;
//MC55_GPRS    
        }
        else if(gucGprs_Module_Type == TELIT_GPRS_MODULE)
        {  
//TELIT_GPRS
            stCMDBuffer.ucLength = sizeof(STR_AT_SET_SISR_T);
            memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_SISR_T , sizeof(STR_AT_SET_SISR_T));
        stCMDBuffer.ucCMDBuffer[9] = stCMDBuffer.ucParam+0x31 ;
//TELIT_GPRS
        }

        stCMDBuffer.uTimeOutCount = AT_DLY_10S;//10*100;
        break;

    case AT_CMD_SET_SISW:
        if(gucGprs_Module_Type == MC55_GPRS_MODULE)
        {          
//MC55_GPRS
        DATALED_SET;
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_SISW);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_SISW , sizeof(STR_AT_SET_SISW));
        stCMDBuffer.ucCMDBuffer[8] = stCMDBuffer.ucParam+0x30 ;
        pstDataContent =  ATGetDataContentPtr(0);
        for(i=0;i<6;i++)
        {
            ucDatabuf[i] = 0;
        }

        datalength = pstDataContent->ucDataLength;

        sprintf((char*)&ucDatabuf,"%d",datalength);

        if (datalength>=100)
        {
            stCMDBuffer.ucCMDBuffer[10] = ucDatabuf[0];
            stCMDBuffer.ucCMDBuffer[11] = ucDatabuf[1];
            stCMDBuffer.ucCMDBuffer[12] = ucDatabuf[2];
        }
        else if (datalength>=10)
        {
            stCMDBuffer.ucCMDBuffer[10] = ucDatabuf[0];
            stCMDBuffer.ucCMDBuffer[11] = ucDatabuf[1];
        }
        else
        {
            stCMDBuffer.ucCMDBuffer[10] = ucDatabuf[0];

        }
//MC55_GPRS
        }
        else if(gucGprs_Module_Type == TELIT_GPRS_MODULE)
        {  
//telit
            stCMDBuffer.ucLength = sizeof(STR_AT_SET_SISW_T)-2;
            memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_SISW_T , sizeof(STR_AT_SET_SISW_T));
            stCMDBuffer.ucCMDBuffer[9] = stCMDBuffer.ucParam+0x31 ;
            stCMDBuffer.uTimeOutCount = AT_DLY_10S;//10*100;
//TELIT_GPRS
        }
        break;

    case AT_CMD_SET_SMSO:
        stCMDBuffer.ucLength = sizeof(STR_AT_SET_SMSO);
        memcpy(stCMDBuffer.ucCMDBuffer , (unsigned char*)STR_AT_SET_SMSO , sizeof(STR_AT_SET_SMSO));
        stCMDBuffer.uTimeOutCount = AT_DLY_15S;//15*100;

        break;


    case PIN_CMD_DELAY_10MS:

        stCMDBuffer.ucLength = 0;
        stCMDBuffer.ucRetryCnt = 1;     //无重发
        if(stCMDBuffer.ucParam == 0)
            stCMDBuffer.uTimeOutCount = CON_CMD_DELAY_100MS;//CON_CMD_DELAY_10MS;
        else
            stCMDBuffer.uTimeOutCount = CON_CMD_DELAY_100MS * stCMDBuffer.ucParam;//CON_CMD_DELAY_10MS * stCMDBuffer.ucParam;
        break;

    case PIN_CMD_IGT:
        stCMDBuffer.ucLength = 0;
        if(stCMDBuffer.ucParam == 1)
        {
            //  PIN_Write1(PIN_MASK_IGT, 1);
            PIN_IGT_SET;
            stCMDBuffer.uTimeOutCount = AT_DLY_1S;//100;
        }
        else
        {
            //  PIN_Write1(PIN_MASK_IGT,0);
            PIN_IGT_CLR;
            stCMDBuffer.uTimeOutCount = AT_DLY_2S;//200;
        }
        break;

    case PIN_CMD_EMERGOFF:
        stCMDBuffer.ucLength = 0;
        if(stCMDBuffer.ucParam == 1)
        {
            //      PIN_Write0(PIN_MASK_EMERGOFF, 1);
            PIN_EMERGOFF_SET;
            stCMDBuffer.uTimeOutCount = AT_DLY_4S;//400;
        }
        else
        {
            //      PIN_Write0(PIN_MASK_EMERGOFF,0);
            PIN_EMERGOFF_CLR;
            stCMDBuffer.uTimeOutCount = AT_DLY_4S;//400;
        }
        break;

    case PIN_CMD_PWCTK:
        stCMDBuffer.ucLength = 0;
        if(stCMDBuffer.ucParam == 1)
            //      PIN_Write1(PIN_MASK_POWERCTL, 1);
            PIN_POWERON_SET;
        else
            //      PIN_Write1(PIN_MASK_POWERCTL, 0);
            PIN_POWERON_CLR;

        break;

    case PIN_CMD_DELAY:
        stCMDBuffer.ucLength = 0;
        stCMDBuffer.ucRetryCnt = 1;     //无重发
        if(stCMDBuffer.ucParam == 0)
            stCMDBuffer.uTimeOutCount = CON_CMD_DELAY_100MS;//CON_CMD_DELAY;
        else
            stCMDBuffer.uTimeOutCount = CON_CMD_DELAY_100MS * stCMDBuffer.ucParam;//CON_CMD_DELAY * stCMDBuffer.ucParam;
        break;

    default :
        stCMDBuffer.uTimeOutCount = 0;

        break;
    }
}

/********************************************************************
函数名称:   AT_CMD_Process_SendSMS
功能    :
短信发送命令(CMGS)解释函数
入口参数:
出口参数:   解释后的字符串长度
*********************************************************************/
unsigned char AT_CMD_Process_SendSMS(void)
{
    unsigned short usLen = 0;
    unsigned char ucRet = 0;

    PSTTC35TASK pstDataContent;

    pstDataContent =  ATGetDataContentPtr(0);
    if( (pstDataContent != NULL) && (pstDataContent->unCurAddrInfo.ucType == TYPE_COMUNNICATION_SMS ))
    {
        usLen = AT_SMS_Encode(pstDataContent);  //短信编码

        memcpy(stCMDBuffer.ucCMDBuffer , STR_AT_SET_CMGS , sizeof(STR_AT_SET_CMGS));
        //设置TAP长度
        usLen = stSMSBufer.usTPALen % 1000;
        stCMDBuffer.ucCMDBuffer[10] = '0' + usLen%10 ;
        usLen /= 10;

        stCMDBuffer.ucCMDBuffer[9] = '0' + usLen%10 ;
        usLen /= 10;

        stCMDBuffer.ucCMDBuffer[8] = '0' + usLen%10 ;

        stCMDBuffer.ucLength = sizeof(STR_AT_SET_CMGS);

        if(stSMSBufer.usLength == 0 )
            ucRet = 0;
        else
            ucRet = 1;

        //      ATDataListPop();
    }
    return ucRet;
}

/********************************************************************
函数名称:   CMD_TimeOutProcess
功    能:   AT命令超时处理函数
入口参数:
出口参数:
*********************************************************************/
void CMD_TimeOutProcess(void)   //超时
{
  unsigned char ucNum ;
    if( stReceiveBuffer.uTimeOutCount==0 )
    {
        switch(stCMDBuffer.ucATCMD)
        {

        case PIN_CMD_DELAY_10MS:
        case PIN_CMD_DELAY:     //命令延时结束不做处理
        case PIN_CMD_PWCTK:
        case PIN_CMD_IGT:
            ClearCmdBuffer();   //清除发送任务、复位接收状态
            break;


        case AT_CMD_SET_CMGS:
            if(stReceiveBuffer.ucRetryCnt >0)
            {
                stCMDBuffer.enuBufferState = STATE_BUFFER_SEND;
                stAtManageState.uStateTimerCounter = CON_TIMEER_GSMSTATE; //CON_TIMEER_GPRSSTATE;//有数据发送推迟状态转换
            }
            else
            {
                //清除发送任务、复位接收状态
                ClearCmdBuffer();

                stAtManageState.ucTimeOutTimes++;
                stSMSBufer.ucIsValid = 0;
                ATDataListPop();                    // 三次发送不成功，删除数据缓冲区
                DebugPrint((unsigned char*)STR_AT_DEBUG_CMDTIMEOUT, sizeof(STR_AT_DEBUG_CMDTIMEOUT));
            }

            break;

        case AT_CMD_QUERY_CMGL:
            AT_CMD_Clear();
            AT_CMD_Fill(AT_CMD_SET_AT, 0);
            ClearCmdBuffer();   //清除发送任务、复位接收状态

            break;

        case AT_CMD_SET_AT:
            AT_CMD_Clear();
            stModuleRunState.bIsExist = 0;
            //AT_CMD_Resend(STATE_POWERON);
            AT_CMD_Resend(STATE_BUFFER_SEND);//azh 110927
            //          ClearCmdBuffer();
            break;

        case AT_CMD_SET_UNLOCK_PIN:
        case AT_CMD_SET_UNLOCK_PUK:
            ClearCmdBuffer();
            //AT_CMD_Resend(STATE_POWERON);
            AT_CMD_Resend(STATE_BUFFER_SEND);//azh 110927
            break;

        case AT_CMD_SET_SMSO:
            ClearCmdBuffer();
            AT_CMD_Fill(PIN_CMD_IGT,1);
            //AT_CMD_Fill(PIN_CMD_DELAY_10MS , 200);
            AT_CMD_Fill(PIN_CMD_DELAY_10MS , AT_DLY_2S);
            AT_CMD_Fill(PIN_CMD_IGT,0);
            stAtManageState.uStateTimerCounter = AT_DLY_6S;//600;
            break;

        case PIN_CMD_EMERGOFF:
            ClearCmdBuffer();
            enuATCurrentState = enuATNextState = STATE_POWEROFF;
            stAtManageState.uStateTimerCounter = 0;
            break;

        case AT_CMD_SET_CFUN:
            ClearCmdBuffer();
            AT_CMD_Clear();
            AT_CMD_Fill(PIN_CMD_IGT,1);
            //AT_CMD_Fill(PIN_CMD_DELAY_10MS , 200);
            AT_CMD_Fill(PIN_CMD_DELAY_10MS , AT_DLY_2S);
            AT_CMD_Fill(PIN_CMD_IGT,0);
            stAtManageState.uStateTimerCounter = AT_DLY_6S;//600;
            break;

        case AT_CMD_SET_SISO:
            trace_at_cmd_sn(9);
            AT_CMD_Fill(AT_CMD_SET_SISC , stCMDBuffer.ucParam);
//MC55_GPRS
            if(gucGprs_Module_Type == MC55_GPRS_MODULE)
            {
            AT_CMD_Fill(AT_CMD_SET_CGATT_DETACH , 0);
            }
//MC55_GPRS
            if(stAtManageState.ucPowerOffNum<10)
            {
                ucNum = (stAtManageState.ucPowerOffNum+1)*5;
                if(stAtManageState.ucReconnectTimes > ucNum)
                {
                    AT_CMD_Clear();
                    AT_CMD_Fill(AT_CMD_SET_SMSO,0);
                    stAtManageState.ucPowerOffNum++;
                }
            }
            else
                stAtManageState.ucPowerOffNum=5;

            ClearCmdBuffer();

            break;

        case AT_CMD_SET_SISW:

            if (stReceiveBuffer.ucRetryCnt >0)
            {
                //SCIBufferWrite(&stAT_TX_BUFFER , ucDataSendBuffer , stAtManageState.usDataSendLength);
                //azh 110929
                Gprs_send_frame(ucDataSendBuffer, stAtManageState.usDataSendLength);
                stReceiveBuffer.ucRetryCnt--;
                stCMDBuffer.uTimeOutCount = AT_DLY_5S;//500;
                DebugPrint((unsigned char*)STR_DEBUG_AT_BUFFER_FILL, sizeof(STR_DEBUG_AT_BUFFER_FILL));
            }
            else
            {
                AT_CMD_Clear();
                ClearCmdBuffer();            //清除发送任务、复位接收状态
                AT_CMD_Fill(AT_CMD_SET_AT,0);
                stAtManageState.ucIsDataSending =0;
            }

            break;

        case AT_CMD_SET_SISR:
            if (stReceiveBuffer.ucRetryCnt >0)
            {
                stReceiveBuffer.ucRetryCnt--;
                AT_CMD_Clear();
                stCMDBuffer.enuBufferState = STATE_BUFFER_SEND;
                //stAtManageState.usReadDataTimes = CON_DataReadTimes;
                AT_CMD_Fill(AT_CMD_SET_SISR,0);

            }
            else
            {
                AT_CMD_Clear();
                ClearCmdBuffer();               //清除发送任务、复位接收状态
                AT_CMD_Fill(AT_CMD_SET_AT,0);
            }
            break;

        case AT_CMD_QUERY_CPIN:
//          ClearCmdBuffer();               //清除发送任务、复位接收状态  add by cfh.
            AT_CMD_Fill(AT_CMD_SET_AT,0);
//            AT_CMD_Fill(AT_CMD_SET_CFUN,0);
//            enuATCurrentState = enuATNextState = STATE_POWERON;
            break;
        case AT_CMD_SET_SISC:
            ClearCmdBuffer();                   //清除发送任务、复位接收状态
            stAtManageState.GPRSNeedReconnect = 1;
            stAtManageState.uStateTimerCounter = CON_TIMEER_GSMSTATE;
            stAtManageState.ucPinDisconnectedIndicate = 0;
            AT_CMD_Fill(AT_CMD_SET_AT,0);
            enuATCurrentState = enuATNextState = STATE_GSM;
            break;
        default:
            ClearCmdBuffer();                   //清除发送任务、复位接收状态
            AT_CMD_Fill(AT_CMD_SET_AT,0);

            break;

        }
    }
}

/********************************************************************
函数名称:   IsATCmdFinish
功能    :   检查所有的AT命令是否执行完毕
入口参数:
出口参数:   1   : 所有命令结束
0   : 有命令未结束
*********************************************************************/
unsigned char IsATCmdFinish(void)
{
    if((stCMDBuffer.enuBufferState == STATE_BUFFER_IDLE ) &&
        ( AT_CMD_GetLength() == 0 ) )
    {
        return 1;
    }
    else
        return 0;
}

/********************************************************************
函数名称:   IsNoDataInATTXBuffer
功能    :   检查AT缓冲区是否有命令、数据未发送完成。在GPRS状态下切换到
命令模式前使用
入口参数:
出口参数:
1   :   所有的数据发送完成
0   :   有数据待发送
*********************************************************************/
//unsigned char IsNoDataInATBuffer( PST_SCI_BUFFER  pBuffer)
//{
//    //串口发送数据缓冲区为空
//    if( SCIBufferGetDataLength( pBuffer ) == 0)
//    {
//        return 1;
//    }
//    else
//        return 0;
//}

/********************************************************************
函数名称:   AT_Receive_Process_CSQ
功能    :   CSQ命令返回结果处理，记录信号强度
入口参数:
出口参数:
*********************************************************************/
unsigned char AT_Receive_Process_CSQ(unsigned char* pStr)
{
    unsigned char   i = 0;
    unsigned char   ptr1 = 0;
    unsigned char   ptr2 = 0;
    unsigned char   ucData = 0;
//  STTERMISCPARA stTerMiscPara=RunPara_GetTerMiscPara();
//  STREMOTERUNINFO stRemoteRunInfo = stTerRunPara.stTerMiscPara.stRCommRunInfo;
    i = 0;
    ptr1 = 0;
    ptr2 = 0;
    ucData = 0;
    while(i<30)
    {
        if(pStr[i]==':')
            ptr1 = i;
        if(pStr[i]==',')
        {
            ptr2 = i;
            break;
        }
        i++;
    }
    ptr1++;
    while( ptr2 > ptr1)
    {
        ucData *= 10;
        if(pStr[ptr1]>='0' && pStr[ptr1]<='9')
        {
            ucData += pStr[ptr1] - '0';
        }
        ptr1++;
    }

    if(ucData <99)
        stModuleRunState.ucSignIntension = ucData;      //??? 信号强度为99
    else
    {
        stModuleRunState.ucSignIntension = 0;
        if(stModuleRunState.bIsGPRSConnected == 1)
        {
            trace_at_cmd_sn(10);
            AT_CMD_Fill(AT_CMD_SET_SISC,stModuleRunState.ucCurCommChannel);                         //复位DTR脚进入命令状态
        }
    }
    Reload_Module_RunState(&stModuleRunState);

/*//azh
    if(Clientflag==0)
    {
        stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat=CON_STATUS_NO;
        //azh
        //stTerRunPara.stTerMiscPara.stRCommRunInfo = stTerRunPara.stTerMiscPara.stRCommRunInfo;
        //RunPara_SetTerMiscPara(&stTerRunPara.stTerMiscPara);
    }
*/
    return 1;
}
/********************************************************************
函数名称:   AT_Receive_Process_CPMS
功能    :   CPMS命令返回结果处理，记录短信存储空间是否改变
入口参数:
出口参数:
*********************************************************************/

unsigned char AT_Receive_Process_CPMS(unsigned char* pStr)
{
    unsigned char i = 0 ;
    unsigned char j = 0 ;
    unsigned char ptr1[7]={0,0,0,0,0,0,0} ;
    unsigned char ptr2[7]={'M','T','M','T','M','T',0};

    while(i<10)
    {
        if(pStr[i]==':')
        {
            break;
        }
        i++;
    }

    while(i<36)
    {
        if((pStr[i]>='A')&&(pStr[i]<='Z'))
        {
            ptr1[j] = pStr[i];
            j++;
        }
        i++;
    }

    if( 0 != memcmp((char*)&(ptr1) , (char*)&(ptr2) , 6 ))
    {
        AT_CMD_Fill(AT_CMD_SET_CPMS,0);
    }

    return 1;
}
/********************************************************************
函数名称:   AT_Receive_Process_CNMI
功能    :   CMNI命令返回结果处理，记录短信存储空间是否改变
入口参数:
出口参数:
*********************************************************************/
unsigned char AT_Receive_Process_CNMI(unsigned char* pStr)
{
    unsigned char ucRet = 1;
    unsigned char i = 0 ;
    unsigned char j = 0 ;
    unsigned char ptr1[6]={0,0,0,0,0,0} ;
    unsigned char ptr2[6]={'2','1','0','0','1'};

    while(i<10)
    {
        if(pStr[i]==':')
        {
            break;
        }
        i++;
    }

    while(i<20)
    {
        if((pStr[i]>='0')&&(pStr[i]<='9'))
        {
            ptr1[j] = pStr[i];
            j++;
        }
        i++;
    }

    if( 0 != memcmp((char*)&(ptr1) , (char*)&(ptr2) , 5 ))
    {
        AT_CMD_Fill(AT_CMD_SET_CNMI,0);
    }
    return ucRet;
}
/********************************************************************
函数名称:   AT_Receive_Process_OK_T
功能    :   AT命令返回OK后的处理
入口参数:
出口参数:   当前命令无效返回0
*********************************************************************/
//TELIT_GPRS
unsigned char AT_Receive_Process_OK_T(void)
{
    unsigned char   ucRet = 1;
//  STTERMISCPARA stTerMiscPara=RunPara_GetTerMiscPara();
//  STREMOTERUNINFO stRemoteRunInfo = stTerRunPara.stTerMiscPara.stRCommRunInfo;
//  unsigned char   m=0;
//  unsigned char   ucNum ;
//  m = stCMDBuffer.ucParam;
    switch(stCMDBuffer.ucATCMD)
    {

    case AT_CMD_SET_CFUN:
    case AT_CMD_SET_SISO:
        break;
    case AT_CMD_SET_SMSO:
        AT_CMD_Clear();                                 //清除命令队列
        ClearCmdBuffer();
        NETLED_CLR;
        enuATCurrentState =  STATE_POWEROFF;
        enuATNextState = STATE_POWEROFF;
        break;
    case AT_CMD_SET_CMGS:
        ClearCmdBuffer();
        break;
    case AT_CMD_SET_SISW:
        ClearCmdBuffer();
        stAtManageState.ucIsDataSending = 0;
        break;
    case AT_CMD_SET_SA:
        ClearCmdBuffer();
        Serverflag=1;
        stAtManageState.uServertimes=120*AT_DLY_1S;//100;
        break;
    case AT_CMD_SET_CGATT_OFFSERVER:
        ClearCmdBuffer();
        Serverflag=0;
        AT_CMD_Fill(AT_CMD_SET_SL,0);
        break;
    case AT_CMD_QUERY_GMR:
        memcpy(&stModuleComParam.stModuleInfor.ucInformation,&stReceiveBuffer.ucBuffer,9);
        stModuleComParam.stModuleInfor.ucInforLength = 9;
        ClearCmdBuffer();
        break;

    //case AT_CMD_SET_SISC:
    case AT_CMD_SET_CGATT_DESOC:
//  case AT_CMD_SET_CGATT_DETACH:
        if(stModuleComParam.ucGPRSServer==0)
        {
            ClearCmdBuffer();
            enuATCurrentState = enuATNextState = STATE_GSM;
            stAtManageState.uStateTimerCounter = CON_TIMEER_GPRSCMDSTATE;
            Clientflag=0;
//          if(stModuleRunState.bIsGPRSConnected == 1)
//          {
            stModuleRunState.bIsGPRSConnected = 0;
            stModuleRunState.ucCurCommChannel++;
            Reload_Module_RunState(&stModuleRunState);
            stModuleRunState.ucCurCommChannel--;
//          }
//          stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat=CON_STATUS_NO;
        }
        else
        {
            stTerRunPara.stTerMiscPara.stRCommRunInfo.ucGprsLoginStat=CON_STATUS_NO;
            Clientflag=0;
            ClearCmdBuffer();
            stTerRunPara.stTerMiscPara.stRCommRunInfo = stTerRunPara.stTerMiscPara.stRCommRunInfo;
            RunPara_SetTerMiscPara(&stTerRunPara.stTerMiscPara);
        }

        break;

    case AT_CMD_SET_SISS_ADDRESS_TCP:
    case AT_CMD_SET_SISS_ADDRESS_UDP:

        if (enuATCurrentState == STATE_GSM)
        {
            ClearCmdBuffer();
            enuATCurrentState = enuATNextState = STATE_GPRS;
            Clientflag=1;
            stAtManageState.uStateTimerCounter  = CON_TIMEER_GPRSSTATE;
        //    if(stModuleComParam.usTimerParam== 0)
        //      stAtManageState.uReceiveTimer   = 60 * 60000;   //分钟->毫秒
        //    else
        //      stAtManageState.uReceiveTimer   = stModuleComParam.usTimerParam * 60000;    //分钟->毫秒

            if(stModuleComParam.usTimerParam== 0)
                stAtManageState.uReceiveTimer   = 60 * AT_DLY_60S;//60 * 60000; //分钟->毫秒
            else
                stAtManageState.uReceiveTimer   = stModuleComParam.usTimerParam * AT_DLY_60S;//60000;   //分钟->毫秒
            //add by 090526 end
            stAtManageState.ucReconnectTimes    = 0;
            stAtManageState.GPRSNeedReconnect   = 0;


            if(stModuleRunState.bIsGPRSConnected == 0)
            {
                stModuleRunState.bIsGPRSConnected = 1;
                stModuleRunState.ucCurCommChannel++;
             //   SCIFillUpTask(CMD_GET_RUN_STATE , (unsigned char*)&stModuleRunState , 11, NULL);
        //   Rcomm_Fill_RecvData(0x03 , (unsigned char*)&stModuleRunState , 11, NULL);
             Reload_Module_RunState(&stModuleRunState);
                stModuleRunState.ucCurCommChannel--;
            }
            stModuleRunState.bIsGPRSConnected = 1;
         //   SCIFillUpTask(CMD_GET_RUN_STATE , (unsigned char*)&stModuleRunState , 11, NULL);
         //   Rcomm_Fill_RecvData(0x03 , (unsigned char*)&stModuleRunState , 11, NULL);
            Reload_Module_RunState(&stModuleRunState);
            stAtManageState.usReadDataTimes = 10*AT_DLY_1S;//100;

        }
        else if (enuATCurrentState == STATE_GPRS)
        {
            AT_CMD_Clear();
            stAtManageState.GPRSNeedReconnect = 0 ;
            stModuleRunState.bIsGPRSConnected = 1;
            Clientflag=1;
            ClearCmdBuffer();
        }

        break;



    default:
        ucRet = 0;
        ClearCmdBuffer();
        break;
    }

    stModuleRunState.bIsExist = 1;
    return ucRet;
}
//TELIT_GPRS
//MC55_GPRS
unsigned char AT_Receive_Process_OK(void)
{
    unsigned char   ucRet = 1;
//    unsigned char   m=0;
//    unsigned char   ucNum ;
//    m = stCMDBuffer.ucParam;
    switch(stCMDBuffer.ucATCMD)
    {

    case AT_CMD_SET_CFUN:
    case AT_CMD_SET_SMSO:
    case AT_CMD_SET_SISO:
        break;

    case AT_CMD_SET_CMGS:
        ClearCmdBuffer();
        break;

    case AT_CMD_QUERY_GMR:
        ClearCmdBuffer();
        break;

    case AT_CMD_SET_SISC:
        ClearCmdBuffer();
        enuATCurrentState = enuATNextState = STATE_GSM;
        stAtManageState.uStateTimerCounter = CON_TIMEER_GPRSCMDSTATE;
        if(stModuleRunState.bIsGPRSConnected == 1)
        {
            stModuleRunState.bIsGPRSConnected = 0;
			stModuleRunState.ucCurCommChannel++;
			Reload_Module_RunState(&stModuleRunState);
			stModuleRunState.ucCurCommChannel--;
        }


    default:
        ucRet = 0;
        ClearCmdBuffer();
        break;
    }

    stModuleRunState.bIsExist = 1;
    return ucRet;
}
//MC55_GPRS
/********************************************************************
函数名称:   AT_Receive_Process_ERROR
功能    :   命令返回ERROR处理
入口参数:
出口参数:
*********************************************************************/
unsigned char AT_Receive_Process_ERROR(void)
{
    unsigned char  ucNum = 0 ;
    switch(stCMDBuffer.ucATCMD)
    {
    case AT_CMD_QUERY_CPIN:
        stModuleRunState.bIsSIMExist = 0;
    //  LCD_Set_HintSeqno(CON_LCDHINT_CHECKSIM);
        AT_CMD_Clear();         //清除命令
        //      AT_CMD_Fill(AT_CMD_SET_SMSO,0);
        ClearCmdBuffer();
        if (stAtManageState.ucCSQErrorCount < 4 )
        {
            //AT_CMD_Fill(PIN_CMD_DELAY,2);
            AT_CMD_Fill(PIN_CMD_DELAY, AT_DLY_2S);
            AT_CMD_Fill(AT_CMD_QUERY_CPIN,0);
            stAtManageState.ucCSQErrorCount++;

        }
        else
        {
            stAtManageState.ucCSQErrorCount=0;
            AT_CMD_Fill(AT_CMD_SET_SMSO,0);
        }

        break;

    case AT_CMD_QUERY_CSQ:          //CSQ返回错误，可能是SIM卡不在
        stModuleRunState.bIsSIMExist = 0;
        AT_CMD_Clear();         //清除命令
        if (stAtManageState.ucCSQErrorCount < 20 )
        {
            stAtManageState.ucCSQErrorCount++;
            enuATCurrentState = enuATNextState = STATE_LOOP;
            stAtManageState.uStateTimerCounter = CON_TIMEER_GSMSTATE;   //启动状态定时器
        }
        else
        {
            AT_CMD_Fill(AT_CMD_SET_SMSO,0);
        }
        break;

    case AT_CMD_SET_UNLOCK_PUK:     //PUK解锁一次错误则状态停滞

        AT_CMD_Clear();             //清除命令
        enuATCurrentState = enuATNextState = STATE_LOOP;
        stAtManageState.uStateTimerCounter = CON_TIMEER_GSMSTATE;   //启动状态定时器
        DebugPrint((unsigned char*)STR_AT_DEBUG_PUKERROR, sizeof(STR_AT_DEBUG_PUKERROR));
        break;

    case AT_CMD_SET_UNLOCK_PIN:     //PIN解码时重试次数为1
        DebugPrint((unsigned char*)STR_AT_DEBUG_PINERROR, sizeof(STR_AT_DEBUG_PINERROR));
        AT_CMD_Clear();             //清除命令
        break;

    case AT_CMD_SET_CMGS:           //
        stReceiveBuffer.uTimeOutCount = 0;
        stReceiveBuffer.ucRetryCnt  = 0;
        //清除发送任务
        ClearCmdBuffer();
        ATDataListPop();
        DebugPrint((unsigned char*)STR_DEBUG_SMSERROR, sizeof(STR_DEBUG_SMSERROR));
        stSMSBufer.ucIsValid = 0;
        break;

    case AT_CMD_SET_AT:             //"AT"
    case AT_CMD_SET_ATE0:           //"ATE=0"
    case AT_CMD_SET_CMGF:           //"AT+CMGF"
    case AT_CMD_QUERY_CNMI:
    case AT_CMD_SET_SSMSS:
    case AT_CMD_SET_SM20:
        enuATCurrentState = enuATNextState = STATE_POWEROFF;
        break;

    case AT_CMD_QUERY_CMGL:
        AT_CMD_Clear();         //清除命令
        if (stAtManageState.ucCMGLErrorCount < 10)
        {
            stAtManageState.ucCMGLErrorCount++;
            //AT_CMD_Fill(PIN_CMD_DELAY,(unsigned char)(4*stAtManageState.ucCMGLErrorCount));
            AT_CMD_Fill(PIN_CMD_DELAY,(unsigned char)(4*stAtManageState.ucCMGLErrorCount));// /10));
            AT_CMD_Fill(AT_CMD_QUERY_CMGL,0);
        }
        else
        {
            AT_CMD_Fill(AT_CMD_SET_SMSO,0);
        }
        break;

    case AT_CMD_SET_CPMS:
        AT_CMD_Clear();         //清除命令
        if (stAtManageState.ucSCPMSErrorCount < 10)
        {
            stAtManageState.ucSCPMSErrorCount++;
            //AT_CMD_Fill(PIN_CMD_DELAY,(unsigned char)(4*stAtManageState.ucSCPMSErrorCount));
            AT_CMD_Fill(PIN_CMD_DELAY,(unsigned char)(4*stAtManageState.ucSCPMSErrorCount*AT_DLY_1S));
            AT_CMD_Fill(AT_CMD_SET_CPMS,0);
        }
        else
        {
            AT_CMD_Fill(AT_CMD_SET_SMSO,0);
        }
        break;

    case AT_CMD_QUERY_CPMS:
        AT_CMD_Clear();         //清除命令
        if (stAtManageState.ucQCPMSErrorCount < 10)
        {
            stAtManageState.ucQCPMSErrorCount++;
            //AT_CMD_Fill(PIN_CMD_DELAY,(unsigned char)(4*stAtManageState.ucSCPMSErrorCount));
            AT_CMD_Fill(PIN_CMD_DELAY,(unsigned char)(4*stAtManageState.ucSCPMSErrorCount/10));
            AT_CMD_Fill(AT_CMD_QUERY_CPMS,0);
        }
        else
        {
            AT_CMD_Fill(AT_CMD_SET_SMSO,0);
        }
        break;


    case AT_CMD_SET_SISC:
        if(stModuleRunState.bIsGPRSConnected == 1)
        {
            stModuleRunState.bIsGPRSConnected = 0;
            stModuleRunState.ucCurCommChannel++;
            Reload_Module_RunState(&stModuleRunState);
            stModuleRunState.ucCurCommChannel--;
        }

        if(stAtManageState.ucPowerOffNum<10)
        {
            ucNum = (stAtManageState.ucPowerOffNum+1)*5;
            if(stAtManageState.ucReconnectTimes > ucNum)
            {
                AT_CMD_Clear();
                AT_CMD_Fill(AT_CMD_SET_SMSO,0);
                stAtManageState.ucPowerOffNum++;
            }
        }
        else
            stAtManageState.ucPowerOffNum=5;

        enuATCurrentState = enuATNextState =  STATE_GSM;
        stAtManageState.GPRSNeedReconnect = 1;
        stAtManageState.uStateTimerCounter = CON_TIMEER_GSMSTATE;
        stAtManageState.ucPinDisconnectedIndicate = 0;

        ClearCmdBuffer();

        break;

/*
    case AT_CMD_SET_SGACT:
        AT_CMD_Clear();
        AT_CMD_Fill(AT_CMD_SET_DESGACT,0);
        //AT_CMD_Fill(PIN_CMD_DELAY,5);
        AT_CMD_Fill(PIN_CMD_DELAY,AT_DLY_5S);
        if(stAtManageState.ucPowerOffNum<10)
        {
            ucNum = (stAtManageState.ucPowerOffNum+1)*5;
            if(stAtManageState.ucReconnectTimes > ucNum)
            {
                AT_CMD_Clear();
                AT_CMD_Fill(AT_CMD_SET_SMSO,0);
                stAtManageState.ucPowerOffNum++;
            }
        }
        else
            stAtManageState.ucPowerOffNum=5;

        ClearCmdBuffer();

        break;
*/
    case AT_CMD_SET_GPRS:
        AT_CMD_Clear();
//      AT_CMD_Fill(AT_CMD_SET_SISC,stModuleRunState.ucCurCommChannel);
/*      AT_CMD_Fill(AT_CMD_SET_CGATT_DETACH,0);
        AT_CMD_Fill(PIN_CMD_DELAY,5);
        AT_CMD_Fill(AT_CMD_SET_CGATT_TACH,0);
        AT_CMD_Fill(AT_CMD_SET_GPRS,0);
        //复位DTR脚进入命令状态
*/
//MC55_GPRS
        if(gucGprs_Module_Type == MC55_GPRS_MODULE)
        {
        AT_CMD_Fill(AT_CMD_SET_CGATT_DETACH,0);
        }
//MC55_GPRS
        if(stAtManageState.ucPowerOffNum<10)
        {
            ucNum = (stAtManageState.ucPowerOffNum+1)*5;
            if(stAtManageState.ucReconnectTimes > ucNum)
            {
                AT_CMD_Clear();
                AT_CMD_Fill(AT_CMD_SET_SMSO,0);
                stAtManageState.ucPowerOffNum++;
            }
        }
        else
            stAtManageState.ucPowerOffNum=5;

        ClearCmdBuffer();

        break;
    case AT_CMD_SET_SISS_ADDRESS_TCP:
    case AT_CMD_SET_SISS_ADDRESS_UDP:
        if(stAtManageState.ucPowerOffNum<10)
        {
            ucNum = (stAtManageState.ucPowerOffNum+1)*5;
            if(stAtManageState.ucReconnectTimes > ucNum)
            {
                AT_CMD_Clear();
                AT_CMD_Fill(AT_CMD_SET_SMSO,0);
                stAtManageState.ucPowerOffNum++;
            }
        }
        else
        {
            Clientflag=0;
            stAtManageState.ucPowerOffNum=5;
        }
        ClearCmdBuffer();
        break;
    case AT_CMD_SET_CGATT_TACH:
        AT_CMD_Clear();         //清除命令
        if (stAtManageState.ucPowerOffNum < 10)
        {
            stAtManageState.ucPowerOffNum++;

            AT_CMD_Fill(AT_CMD_SET_CGATT_DETACH,0);
        }
        else
        {
            AT_CMD_Fill(AT_CMD_SET_SMSO,0);
        }
        break;
/*//telit
    case AT_CMD_SET_CGATT_DESOC:
        Clientflag=1;
        break;
*/
        //"ATO" 返回数据状态
    case AT_CMD_SET_SISW:
        if((stModuleComParam.ucGPRSServer==0)||(Clientflag==1))
        {

            stAtManageState.uStateTimerCounter = 0;

        }
        stAtManageState.ucIsDataSending = 0 ;
        ATDataListPop();                                //发送后从列表中删除数据
 //       ClearCmdBuffer();
        break;

    case AT_CMD_SET_SMSO:
        ClearCmdBuffer();
        AT_CMD_Fill(PIN_CMD_IGT,1);
        //AT_CMD_Fill(PIN_CMD_DELAY_10MS , 200);
        AT_CMD_Fill(PIN_CMD_DELAY_10MS,AT_DLY_2S);
        AT_CMD_Fill(PIN_CMD_IGT,0);
        stAtManageState.uStateTimerCounter = AT_DLY_6S;//600;
        break;

    case AT_CMD_SET_SISR:
        trace_at_cmd_sn(11);
        AT_CMD_Fill(AT_CMD_SET_SISC,stModuleRunState.ucCurCommChannel);                         //复位DTR脚进入命令状态
        ClearCmdBuffer();
        break;

    case AT_CMD_SET_CHUP:   //挂机
    case AT_CMD_SET_CMGD:   //短信删除
        break;

    default:

        break;
    }

    ClearCmdBuffer();

    return 1;
}

unsigned char AT_Recvive_LinkClose(unsigned char* pStr)
{
    if(stModuleComParam.ucGPRSServer==0)
    {
        AT_CMD_Fill(AT_CMD_SET_CGATT_DESOC,0);
        enuATCurrentState = enuATNextState = STATE_GSM;
        stAtManageState.uStateTimerCounter = CON_TIMEER_GPRSCMDSTATE;
        stModuleRunState.bIsGPRSConnected = 0;
      //  SCIFillUpTask(CMD_GET_RUN_STATE , (unsigned char*)&stModuleRunState , 11, NULL);
    //  Rcomm_Fill_RecvData(0x03 , (unsigned char*)&stModuleRunState , 11, NULL);
        Reload_Module_RunState(&stModuleRunState);
        if(stAtManageState.ucIsDataSending == 1)
            stAtManageState.ucIsDataSending = 0;
    }
    else if((Serverflag==1)&&(Clientflag==0))
    {
        AT_CMD_Fill(AT_CMD_SET_SL,0);
        Serverflag=0;
    }
    return 1;
}
//unsigned char AT_Recvive_ServerLinkClose(unsigned char* pStr)
//{
//
////      AT_CMD_Fill(AT_CMD_SET_CGATT_DESOC,0);
//        enuATCurrentState = enuATNextState = STATE_GSM;
//        stAtManageState.uStateTimerCounter = CON_TIMEER_GPRSCMDSTATE;
//        stModuleRunState.bIsGPRSConnected = 0;
//      //  SCIFillUpTask(CMD_GET_RUN_STATE , (unsigned char*)&stModuleRunState , 11, NULL);
//    //  Rcomm_Fill_RecvData(0x03 , (unsigned char*)&stModuleRunState , 11, NULL);
//        Reload_Module_RunState(&stModuleRunState);
//        if(stAtManageState.ucIsDataSending == 1)
//            stAtManageState.ucIsDataSending = 0;
//
//    return 1;
//}
/********************************************************************
函数名称:   AT_Receive_Process_SISW
功能    :   at^siso命令返回,或者是GPRS状态下发送数据
当前命令为at^siso，出现CONNECT后模块进入数据状态，
则更改相应状态
入口参数:
出口参数:
1   :   当前为AT^SISO   或者没有
0   :   其他命令
*********************************************************************/
//MC55_GPRS
unsigned char AT_Receive_Process_SISW(unsigned char* pStr)
{
    unsigned char k =0;
    unsigned char i =0;
    unsigned char ucsp =0;
    unsigned char ucdv =0;

    PSTTC35TASK pstDataContent = NULL;
    unsigned char ucRet = 0 ;
    if ( (pStr[12] == 0x31) && (pStr[13] == 0x0d) && (pStr[14] == 0x0a) )
    {
        if (enuATCurrentState == STATE_GSM)
        {
            enuATCurrentState = enuATNextState = STATE_GPRS;
            stAtManageState.uStateTimerCounter  = CON_TIMEER_GPRSSTATE;
            if(stModuleComParam.usTimerParam== 0)
                stAtManageState.uReceiveTimer   = 60 * 600;//azh //60 * 60000;  //分钟->毫秒
            else
                stAtManageState.uReceiveTimer   = stModuleComParam.usTimerParam * 600;//azh //stModuleComParam.usTimerParam * 60000;    //分钟->毫秒
            //          stAtManageState.uReceiveTimer       = stModuleComParam.usTimerParam * 6000; //分钟->毫秒
            //                stAtManageState.uNoReceiveTimer = 1200*60000;
            //add by 090526
            if(stModuleComParam.usTimerParam== 0)
                stAtManageState.uReceiveTimer   = 60 * 600;//azh //60 * 60000;  //分钟->毫秒
            else
                stAtManageState.uReceiveTimer   = stModuleComParam.usTimerParam * 600;//azh //stModuleComParam.usTimerParam * 60000;    //分钟->毫秒
            //add by 090526 end
            stAtManageState.ucReconnectTimes    = 0;
            stAtManageState.GPRSNeedReconnect   = 0;

            AT_CMD_Fill(AT_CMD_SET_SISR , stModuleRunState.ucCurCommChannel);

            if(stModuleRunState.bIsGPRSConnected == 0)
            {
                stModuleRunState.bIsGPRSConnected = 1;
                stModuleRunState.ucCurCommChannel++;
                Reload_Module_RunState(&stModuleRunState);
                stModuleRunState.ucCurCommChannel--;
            }
            stModuleRunState.bIsGPRSConnected = 1;
            stAtManageState.usReadDataTimes = CON_DATAREADTIMES;
            ClearCmdBuffer();
        }
        else if (enuATCurrentState == STATE_GPRS)
        {
            stAtManageState.GPRSNeedReconnect = 0 ;
            stModuleRunState.bIsGPRSConnected = 1;
        }
    }
    else
    {
        if (enuATCurrentState == STATE_GPRS)
        {
            if (stCMDBuffer.ucATCMD == AT_CMD_SET_SISW)
            {
                stModuleRunState.bIsGPRSConnected = 1;

                for(i=9;i<=20;i++)
                {
                    //会出现: 0d0a^SISR: 0, 10D0A0D0A^SISW: 0, 36, 360D0A的情况 在响应写的过程中先收到 ^SISR有数据的请求
                    //在GetStringFromStr函数中stReceiveBuffer.usProcPtr这个指针有偏移 所以不用stReceiveBuffer.ucBuffer
                    //if(stReceiveBuffer.ucBuffer[i] == 0x2C)pStr
                    if(pStr[i] == 0x2C)
                        k = i;
                    //if(stReceiveBuffer.ucBuffer[i] == 0x20)
                    if(pStr[i] == 0x20)
                        ucsp= i ;
                    //if(stReceiveBuffer.ucBuffer[i] == 0x0d)
                    if(pStr[i] == 0x0d)
                        break;
                }

                if(i>15)
                {
                    k = k - 12;
                    ucdv= i - ucsp;
                    //if((ucdv >= 5)&&((stReceiveBuffer.ucBuffer[ucsp+1]-0x30)>=6))
                    if((ucdv >= 5)&&((pStr[ucsp+1]-0x30)>=6))
                    {
                        ClearCmdBuffer();
                        trace_at_cmd_sn(12);
                        AT_CMD_Fill(AT_CMD_SET_SISC,stModuleRunState.ucCurCommChannel);
                        return ucRet;
                    }
                }
                else
                {
                    k=i-12;
                }
                if (k==1)
                    stAtManageState.usDataSendLength = pStr[12]-0x30;
                else if (k==2)
                    stAtManageState.usDataSendLength = (pStr[12]-0x30)*10+(pStr[13]-0x30);
                else if (k==3)
                    stAtManageState.usDataSendLength = (pStr[12]-0x30)*100+(pStr[13]-0x30)*10+(pStr[14]-0x30);
                else
                    stAtManageState.usDataSendLength = (pStr[12]-0x30)*1000+(pStr[13]-0x30)*100+(pStr[14]-0x30)*10+(pStr[15]-0x30);

                pstDataContent =  ATGetDataContentPtr(0);
                if (stAtManageState.usDataSendLength < pstDataContent->ucDataLength)
                {
                    //                  stAtManageState.usDataSendLength = pstDataContent->ucDataLength;
                    //AT_CMD_Fill(PIN_CMD_DELAY,5);
                    AT_CMD_Fill(PIN_CMD_DELAY,AT_DLY_5S);
                    stAtManageState.ucSISWBusyCheckTimes++;

                    if(stAtManageState.ucSISWBusyCheckTimes > 10)
                    {
                        trace_at_cmd_sn(13);
                        AT_CMD_Fill(AT_CMD_SET_SISC,stModuleRunState.ucCurCommChannel);
                        stAtManageState.ucIsDataSending = 0;
                    }
                    else
                    {
                        AT_CMD_Fill(AT_CMD_SET_SISW,stModuleRunState.ucCurCommChannel);
                        stAtManageState.ucIsDataSending = 1;
                    }
                    DebugPrint((unsigned char*)STR_DEBUG_COUNTER_ERROR, sizeof(STR_DEBUG_COUNTER_ERROR));
                }
                if ((pstDataContent != NULL)&&(stAtManageState.usDataSendLength != 0))
                {
                    stAtManageState.usDataSendLength = pstDataContent->ucDataLength;
                    //SCIBufferWrite(&stAT_TX_BUFFER, pstDataContent->pData, stAtManageState.usDataSendLength);
                    Gprs_send_frame(pstDataContent->pData, stAtManageState.usDataSendLength);//azh 111107
                    memcpy(&ucDataSendBuffer , pstDataContent->pData , stAtManageState.usDataSendLength);
                    ATDataListPop();
                    stAtManageState.ucIsDataSending = 0;
                    stAtManageState.ucSISWBusyCheckTimes=0;
//                  DebugPrint(ucDataSendBuffer,stAtManageState.usDataSendLength);
                }
            }
        }
        else
        {
            trace_at_cmd_sn(14);
            AT_CMD_Fill(AT_CMD_SET_SISC,stModuleRunState.ucCurCommChannel);
            enuATCurrentState = enuATNextState = STATE_GSM;
            stAtManageState.uStateTimerCounter = CON_TIMEER_GSMSTATE;
            ClearCmdBuffer();
        }
    }

    return ucRet;
}
//MC55_GPRS

//telit
//TELIT_GPRS
unsigned char AT_Receive_Process_SISW_T(unsigned char* pStr)
{
//  unsigned char k =0;
//  unsigned char i =0;
//  unsigned char ucsp =0;
//  unsigned char ucdv =0;
    unsigned char ucRet = 1 ;
    PSTTC35TASK pstDataContent = NULL;
    pstDataContent =  ATGetDataContentPtr(0);
    if (enuATCurrentState == STATE_GPRS)
    {
//      if ((stCMDBuffer.ucATCMD == AT_CMD_SET_SISWTCP)||(stCMDBuffer.ucATCMD == AT_CMD_SET_SISWUDP))
//      {
            stModuleRunState.bIsGPRSConnected = 1;
            pstDataContent =  ATGetDataContentPtr(0);
            if(pstDataContent->ucDataLength>=2000)
            {
                ClearCmdBuffer();
                stAtManageState.ucIsDataSending = 0;
                return ucRet;
            }
            else if (pstDataContent != NULL)//&&(stAtManageState.usDataSendLength != 0))
            {

                stAtManageState.usDataSendLength = (pstDataContent->ucDataLength)*2;
                ASCTOHEX(ucDataSendBuffer , pstDataContent->pData , stAtManageState.usDataSendLength);
//              memcpy(&ucDataSendBuffer , pstDataContent->pData , stAtManageState.usDataSendLength);
                if((stAtManageState.usDataSendLength+1)<CON_DATA_SEND_BUFFER_DEPTH)
                {
                    ucDataSendBuffer[stAtManageState.usDataSendLength] = 0x1A;
                    stAtManageState.usDataSendLength += 1;
                }
                //SCIBufferWrite(&stAT_TX_BUFFER, ucDataSendBuffer, stAtManageState.usDataSendLength);
                //azh 110929
                Gprs_send_frame(ucDataSendBuffer, stAtManageState.usDataSendLength);
                ucLink_Time = AT_DLY_1S;//100;
                ATDataListPop();
                stAtManageState.ucIsDataSending = 0;
                stAtManageState.ucSISWBusyCheckTimes=0;
            }else
                stAtManageState.ucIsDataSending = 0;
//      }
    }
    else if(Clientflag==1)
    {
        stAtManageState.ucIsDataSending = 0;
        AT_CMD_Fill(AT_CMD_SET_CGATT_DESOC,1);
        enuATCurrentState = enuATNextState = STATE_GSM;
        stAtManageState.uStateTimerCounter = CON_TIMEER_GSMSTATE;
        ClearCmdBuffer();
    }

    return ucRet;
}
// TELIT_GPRS
/********************************************************************
函数名称:   AT_Receive_Process_SISR
功能    :

  入口参数:
  出口参数:
  1 :   当前为  或者没有
  0 :   其他命令
  *********************************************************************/
/*  unsigned char AT_Receive_Process_SISR(unsigned char* pStr)
  {
      unsigned char ucRet = 0;
      unsigned char k =0;
      unsigned char i =0;
      unsigned char m = 0;
      UN_CHANNEL_ADDR   unAdrInfo;
      if(stModuleRunState.bIsGPRSConnected == 1)
      {
      //     PIN_LED_R_SET;

//        if ( (pStr[12] == 0x31)&& (pStr[13] == 0x0d) && (pStr[14] == 0x0a) )
//        {
//            if(stAtManageState.ucIsDataSending == 0)
//            {
//                m = pStr[9] -0x30;
//                stAtManageState.usReadDataTimes = CON_DataReadTimes;
//                stAtManageState.usDataReceiveLength = 0;
//                stAtManageState.ucSISRNeeded = 0 ;
//                AT_CMD_Fill(AT_CMD_SET_SISR, m);
//            }
//            else
//            {
//                m = pStr[9] -0x30;
//                stAtManageState.ucSISRNeeded = 1 ;
//                stAtManageState.ucSISRParam = m;
//            }

//        }

          if (stCMDBuffer.ucATCMD == AT_CMD_SET_SISR)
          {
              for(i=10;i<=18;i++)
              {
                  //if(stReceiveBuffer.ucBuffer[i] == 0x2d)
                //  {
                    //  AT_CMD_Clear();
                    //  ClearCmdBuffer();
                    //  AT_CMD_Fill(AT_CMD_SET_SISC,stModuleRunState.ucCurCommChannel);                         //复位DTR脚进入命令状态
                     // break;
                //  }
                  if(stReceiveBuffer.ucBuffer[i] == 0x0d)
                      break;
              }

              k=i-10;
              if (k==1)
                  stAtManageState.usDataReceiveLength = pStr[10]-0x30;
              else if (k==2)
                  stAtManageState.usDataReceiveLength = (pStr[10]-0x30)*10+(pStr[11]-0x30);
              else if (k==3)
                  stAtManageState.usDataReceiveLength = (pStr[10]-0x30)*100+(pStr[11]-0x30)*10+(pStr[12]-0x30);
              else
                  stAtManageState.usDataReceiveLength = (pStr[10]-0x30)*1000+(pStr[11]-0x30)*100+(pStr[12]-0x30)*10+(pStr[13]-0x30);

              m = pStr[8]-0x31;
              unAdrInfo = stModuleComParam.stCh[m].unChInfo;

              if (stAtManageState.usDataReceiveLength != 0)
              {
                  if (stAtManageState.usDataReceiveLength >= 1500)      //如果大于或等于所要读的字节数则重读
                  {
                      //    DebugPrint( (unsigned char *)&(stReceiveBuffer.ucBuffer[i+2]) , (unsigned short) stAtManageState.usDataReceiveLength);
                      Rcomm_Fill_RecvData(CMD_GPRSDATA_AUTO_UP , (unsigned char *)&(stReceiveBuffer.ucBuffer[i+2]), stAtManageState.usDataReceiveLength, &unAdrInfo);
                      AT_CMD_Fill(PIN_CMD_DELAY_10MS , 20);
//                    AT_CMD_Fill(AT_CMD_SET_SISR,m);

                      stAtManageState.usReadDataTimes = CON_DataReadTimes;
                  }
                  else
                  {
                      //    DebugPrint( (unsigned char *)&(stReceiveBuffer.ucBuffer[i+2]) , (unsigned short) stAtManageState.usDataReceiveLength);
                      Rcomm_Fill_RecvData(CMD_GPRSDATA_AUTO_UP , (unsigned char *)&(stReceiveBuffer.ucBuffer[i+2]), stAtManageState.usDataReceiveLength, &unAdrInfo);
                      stAtManageState.usReadDataTimes = CON_DataReadTimes;
                  }
                  //        stAtManageState.uReceiveTimer = stModuleComParam.usTimerParam * 6000;   //分钟->毫秒
                  //add by 090526
                  if(stModuleComParam.usTimerParam== 0)
                      stAtManageState.uReceiveTimer     = 60 * 60000;   //分钟->毫秒
                  else
                      stAtManageState.uReceiveTimer     = stModuleComParam.usTimerParam * 60000;    //分钟->毫秒
                  //add by 090526 end
                  stAtManageState.uNoReceiveTimer = 1200*6000;
              }
              else
              {
                  stAtManageState.uStateTimerCounter = CON_TIMEER_GPRSSTATE;
                  stAtManageState.usDataReceiveLength = 0;
              }
              //复位数据计数器
              stAtManageState.ucIsDataRecving = 0;
              stReceiveBuffer.usProcPtr = stAtManageState.usDataReceiveLength + stReceiveBuffer.usProcPtr+2;
              stAtManageState.uStateTimerCounter = CON_TIMEER_GPRSSTATE;
              stAtManageState.usDataReceiveLength = 0;
              ClearCmdBuffer();
          }

         // else if ( (pStr[12] == 0x32) && (pStr[13] == 0x0d) && (pStr[14] == 0x0a) )
         // {
            //  m = pStr[9]-0x30;
            //  ClearCmdBuffer();
            //  AT_CMD_Clear();
             // AT_CMD_Fill(AT_CMD_SET_SISC,stModuleRunState.ucCurCommChannel);                         //复位DTR脚进入命令状态
              //        DebugPrint((unsigned char*)STR_AT_DEBUG_PINERROR, sizeof(STR_AT_DEBUG_PINERROR));
          //}

      }
      else
          ClearCmdBuffer();
      return ucRet;

}
*/
//azh telit
//TELIT_GPRS
unsigned char AT_Receive_Process_SISR_T(unsigned char* pStr)
{
    unsigned char ucRet = 0;
    unsigned char k =0;
    unsigned char i =0;
    unsigned char m = 0;
//  unsigned char* pStr1;
//  int l=0;
    UN_CHANNEL_ADDR unAdrInfo;
    if(stModuleRunState.bIsGPRSConnected == 1)
    {
       // memset(uctemp , 0x00, 20);//stReceiveBuffer.ucBuffer
      //  memcpy(uctemp,&stReceiveBuffer.ucBuffer,20);
      //  如果发的命令是AT_CMD_SET_SISR
//      pStr1=strstr((char*)&stReceiveBuffer.ucBuffer, "PRECV:");

        for (i=1;i<18;i++)
        {
            if (stReceiveBuffer.ucBuffer[i] == 0x0d)
                break;
        }
        k=i-12;
        if (k==1)
            stAtManageState.usDataReceiveLength  = stReceiveBuffer.ucBuffer[12]-0x30;
        else if (k==2)
            stAtManageState.usDataReceiveLength  = (stReceiveBuffer.ucBuffer[12]-0x30)*10+(stReceiveBuffer.ucBuffer[13]-0x30);
        else if (k==3)
            stAtManageState.usDataReceiveLength  = (stReceiveBuffer.ucBuffer[12]-0x30)*100+(stReceiveBuffer.ucBuffer[13]-0x30)*10+(stReceiveBuffer.ucBuffer[14]-0x30);
        else //if (k==4)        //add by cfh 10-01-14
            stAtManageState.usDataReceiveLength  = (stReceiveBuffer.ucBuffer[12]-0x30)*1000+(stReceiveBuffer.ucBuffer[13]-0x30)*100+(stReceiveBuffer.ucBuffer[14]-0x30)*10+(stReceiveBuffer.ucBuffer[15]-0x30);
        m = stReceiveBuffer.ucBuffer[10] - 0x31;
        if(m==0)
        {
            stAtManageState.uServertimes = 120*AT_DLY_1S;//100;}
        }
        unAdrInfo = stModuleComParam.stCh[m].unChInfo;

        if (stAtManageState.usDataReceiveLength != 0)
        {
            if(stAtManageState.usDataReceiveLength <=600)
                Rcomm_Fill_RecvData(CMD_GPRSDATA_AUTO_UP , (unsigned char *)&(stReceiveBuffer.ucBuffer[i+2]), stAtManageState.usDataReceiveLength, &unAdrInfo);
                ucLink_Time = AT_DLY_1S;//100;
            //SCIFillUpTask(CMD_GPRSDATA_AUTO_UP ,(unsigned char*)&(stReceiveBuffer.ucBuffer[i+2]), stAtManageState.usDataReceiveLength, &unAdrInfo);
//          PIN_LEDDATA_SET;
//            PIN_RCOMM_RXD_SET;//
                if(stAtManageState.usDataReceiveLength ==600)
                {
                      //AT_CMD_Fill(PIN_CMD_DELAY_10MS , 20);
                      AT_CMD_Fill(PIN_CMD_DELAY_10MS,AT_DLY_200MS);
                      AT_CMD_Fill(AT_CMD_SET_SISR, stAtManageState.ucSISRParam);            // 此处0需要改成当前通道
//                    AT_CMD_Fill(AT_CMD_SET_SISR,m);
                }
            stAtManageState.usReadDataTimes = CON_DATAREADTIMES;
            //stAtManageState.usReadDataTimes = 10*100;
            if(stModuleComParam.usTimerParam== 0)
                stAtManageState.uReceiveTimer   = 60 * AT_DLY_60S;//60000;  //分钟->毫秒
            else
                stAtManageState.uReceiveTimer   = stModuleComParam.usTimerParam * AT_DLY_60S;//60000;   //分钟->毫秒
                    //add by 090526 end
            stAtManageState.uNoReceiveTimer = 1200*AT_DLY_60S;//6000;
        }

        else
        {
            stAtManageState.uStateTimerCounter = CON_TIMEER_GPRSSTATE;
            stAtManageState.usDataReceiveLength = 0;
        }
        stAtManageState.ucIsDataRecving = 0;
        stReceiveBuffer.usProcPtr = stAtManageState.usDataReceiveLength + stReceiveBuffer.usProcPtr+2;
        stAtManageState.uStateTimerCounter = CON_TIMEER_GPRSSTATE;
        stAtManageState.usDataReceiveLength = 0;
        ClearCmdBuffer();
    }
    else
            ClearCmdBuffer();
    return ucRet;

}
// TELIT_GPRS
//MC55_GPRS
unsigned char AT_Receive_Process_SISR(unsigned char* pStr)
  {
      unsigned char ucRet = 0;
      unsigned char k =0;
      unsigned char i =0;
      unsigned char m = 0;
//      unsigned char* pStr1;
      UN_CHANNEL_ADDR   unAdrInfo;

      if(stModuleRunState.bIsGPRSConnected == 1)
      {
                    //azh//0x0d0x0a^SISR: 0, 1,0x0d0x0a0x0d0x0aOK
          if ( (pStr[12] == 0x31)&& (pStr[13] == 0x0d) && (pStr[14] == 0x0a) )
          {
              if(stAtManageState.ucIsDataSending == 0)
              {
                  m = pStr[9] -0x30;//通道号
                  stAtManageState.usReadDataTimes = 10*10;//azh unit:100ms//10*100;
                  stAtManageState.usDataReceiveLength = 0;
                  stAtManageState.ucSISRNeeded = 0 ;
                  AT_CMD_Fill(AT_CMD_SET_SISR, m);
              }
              else
              {
                  m = pStr[9] -0x30;
                  stAtManageState.ucSISRNeeded = 1 ;
                  stAtManageState.ucSISRParam = m;
              }

          }

          if (stCMDBuffer.ucATCMD == AT_CMD_SET_SISR)
          {
              for(i=9;i<=18;i++)
              {
                  if(stReceiveBuffer.ucBuffer[i] == 0x2d)
                  {
                      AT_CMD_Clear();
                      ClearCmdBuffer();
                      trace_at_cmd_sn(15);
                      AT_CMD_Fill(AT_CMD_SET_SISC,stModuleRunState.ucCurCommChannel);                           //复位DTR脚进入命令状态
                      break;
                  }
                  if(stReceiveBuffer.ucBuffer[i] == 0x0d)
                      break;
              }
              if(i <= 18)//azh add
              {
                  k=i-12;
                  if (k==1)
                      stAtManageState.usDataReceiveLength = pStr[12]-0x30;
                  else if (k==2)
                      stAtManageState.usDataReceiveLength = (pStr[12]-0x30)*10+(pStr[13]-0x30);
                  else if (k==3)
                      stAtManageState.usDataReceiveLength = (pStr[12]-0x30)*100+(pStr[13]-0x30)*10+(pStr[14]-0x30);
                  else
                      stAtManageState.usDataReceiveLength = (pStr[12]-0x30)*1000+(pStr[13]-0x30)*100+(pStr[14]-0x30)*10+(pStr[15]-0x30);

                  m = pStr[9]-0x30;
                  unAdrInfo = stModuleComParam.stCh[m].unChInfo;
              }
              else
              {
                  stAtManageState.usDataReceiveLength = 0;
              }


              if (stAtManageState.usDataReceiveLength != 0)
              {
      //              PIN_LED_R_SET;
                  if (stAtManageState.usDataReceiveLength >= 700)       //如果大于或等于所要读的字节数则重读
                  {
                      //    DebugPrint( (unsigned char *)&(stReceiveBuffer.ucBuffer[i+2]) , (unsigned short) stAtManageState.usDataReceiveLength);
                      //SCIFillUpTask(CMD_GPRSDATA_AUTO_UP , (unsigned char *)&(stReceiveBuffer.ucBuffer[i+2]), stAtManageState.usDataReceiveLength, &unAdrInfo);
                        Rcomm_Fill_RecvData(CMD_GPRSDATA_AUTO_UP, (unsigned char *)&(stReceiveBuffer.ucBuffer[i+2]), stAtManageState.usDataReceiveLength, &unAdrInfo);
                    //  PIN_LEDDATA_SET;
                      //AT_CMD_Fill(PIN_CMD_DELAY_10MS , 20);
                      AT_CMD_Fill(PIN_CMD_DELAY_10MS , AT_DLY_200MS);
                      AT_CMD_Fill(AT_CMD_SET_SISR,m);

                      stAtManageState.usReadDataTimes = 10*10;//10*100;
                  }
                  else
                  {
                      //    DebugPrint( (unsigned char *)&(stReceiveBuffer.ucBuffer[i+2]) , (unsigned short) stAtManageState.usDataReceiveLength);
                      //SCIFillUpTask(CMD_GPRSDATA_AUTO_UP , (unsigned char *)&(stReceiveBuffer.ucBuffer[i+2]), stAtManageState.usDataReceiveLength, &unAdrInfo);
                      Rcomm_Fill_RecvData(CMD_GPRSDATA_AUTO_UP, (unsigned char *)&(stReceiveBuffer.ucBuffer[i+2]), stAtManageState.usDataReceiveLength, &unAdrInfo);
                    //  PIN_LEDDATA_SET;
                      stAtManageState.usReadDataTimes = 10*10;//10*100;
                  }
                  //        stAtManageState.uReceiveTimer = stModuleComParam.usTimerParam * 6000;   //分钟->毫秒
                  //add by 090526
                  if(stModuleComParam.usTimerParam== 0)
                      stAtManageState.uReceiveTimer     = 60 * 600;//60 * 60000;   //分钟->毫秒
                  else
                      stAtManageState.uReceiveTimer     = stModuleComParam.usTimerParam * 600;//stModuleComParam.usTimerParam * 60000;    //分钟->毫秒
                  //add by 090526 end
                  stAtManageState.uNoReceiveTimer = 1200*600;//1200*6000;
              }
              else
              {
                  stAtManageState.uStateTimerCounter = CON_TIMEER_GPRSSTATE;
                  stAtManageState.usDataReceiveLength = 0;
              }
              //复位数据计数器
              stReceiveBuffer.usProcPtr = stAtManageState.usDataReceiveLength + stReceiveBuffer.usProcPtr+2;
              stAtManageState.uStateTimerCounter = CON_TIMEER_GPRSSTATE;
              stAtManageState.usDataReceiveLength = 0;
              ClearCmdBuffer();
          }

          else if ( (pStr[12] == 0x32) && (pStr[13] == 0x0d) && (pStr[14] == 0x0a) )
          {
              m = pStr[9]-0x30;
              ClearCmdBuffer();
              AT_CMD_Clear();
              trace_at_cmd_sn(16);
              AT_CMD_Fill(AT_CMD_SET_SISC,stModuleRunState.ucCurCommChannel);                           //复位DTR脚进入命令状态
              //        DebugPrint((unsigned char*)STR_AT_DEBUG_PINERROR, sizeof(STR_AT_DEBUG_PINERROR));
          }

      }
      else
          ClearCmdBuffer();

      return ucRet;

}
// MC55_GPRS
unsigned char AT_Receive_Process_SGACT(unsigned char* pStr)
{
    unsigned char ucRet = 0;
    unsigned char k =0;
    unsigned char i =0;
    unsigned char m = 0,n=0;
    //STLOCALIP stLocalIP = RunPara_GetLocalIP();
    for (i=1;i<4;i++)
    {
        if (stReceiveBuffer.ucBuffer[10+i] == 0x2e)
            break;
    }
    if(i==1)
    stTerRunPara.stLocalIP.stLocalIP.ucIPAddr[0] = stReceiveBuffer.ucBuffer[10]-0x30;
    else if(i==2)
    stTerRunPara.stLocalIP.stLocalIP.ucIPAddr[0] = (stReceiveBuffer.ucBuffer[10]-0x30)*10+(stReceiveBuffer.ucBuffer[11]-0x30);
    else
    stTerRunPara.stLocalIP.stLocalIP.ucIPAddr[0] = (stReceiveBuffer.ucBuffer[10]-0x30)*100+(stReceiveBuffer.ucBuffer[11]-0x30)*10+(stReceiveBuffer.ucBuffer[12]-0x30);
    for (m=1;m<4;m++)
    {
        if (stReceiveBuffer.ucBuffer[11+i+m] == 0x2e)
            break;
    }
    if(m==1)
    stTerRunPara.stLocalIP.stLocalIP.ucIPAddr[1] = stReceiveBuffer.ucBuffer[11+i]-0x30;
    else if(m==2)
    stTerRunPara.stLocalIP.stLocalIP.ucIPAddr[1] = (stReceiveBuffer.ucBuffer[11+i]-0x30)*10+(stReceiveBuffer.ucBuffer[12+i]-0x30);
    else
    stTerRunPara.stLocalIP.stLocalIP.ucIPAddr[1] = (stReceiveBuffer.ucBuffer[11+i]-0x30)*100+(stReceiveBuffer.ucBuffer[12+i]-0x30)*10+(stReceiveBuffer.ucBuffer[13+i]-0x30);
    for (n=1;n<4;n++)
    {
        if (stReceiveBuffer.ucBuffer[12+i+m+n] == 0x2e)
            break;
    }
    if(n==1)
    stTerRunPara.stLocalIP.stLocalIP.ucIPAddr[2] = stReceiveBuffer.ucBuffer[12+i+m]-0x30;
    else if(n==2)
    stTerRunPara.stLocalIP.stLocalIP.ucIPAddr[2] = (stReceiveBuffer.ucBuffer[12+i+m]-0x30)*10+(stReceiveBuffer.ucBuffer[13+i+m]-0x30);
    else
    stTerRunPara.stLocalIP.stLocalIP.ucIPAddr[2] = (stReceiveBuffer.ucBuffer[12+i+m]-0x30)*100+(stReceiveBuffer.ucBuffer[13+i+m]-0x30)*10+(stReceiveBuffer.ucBuffer[14+i+m]-0x30);
    for (k=1;k<4;k++)
    {
        if (stReceiveBuffer.ucBuffer[13+i+m+n+k] == 0x0d)
            break;
    }
    if(k==1)
        stTerRunPara.stLocalIP.stLocalIP.ucIPAddr[3] = stReceiveBuffer.ucBuffer[13+i+m+n]-0x30;
    else if(k==2)
        stTerRunPara.stLocalIP.stLocalIP.ucIPAddr[3] = (stReceiveBuffer.ucBuffer[13+i+m+n]-0x30)*10+(stReceiveBuffer.ucBuffer[14+i+m+n]-0x30);
    else
        stTerRunPara.stLocalIP.stLocalIP.ucIPAddr[3] = (stReceiveBuffer.ucBuffer[13+i+m+n]-0x30)*100+(stReceiveBuffer.ucBuffer[14+i+m+n]-0x30)*10+(stReceiveBuffer.ucBuffer[15+i+m+n]-0x30);
//azh
//    RunPara_SetTerLocalIP(&stTerRunPara.stLocalIP);
    ClearCmdBuffer();
    return ucRet;

}

/********************************************************************
函数名称:   AT_Receive_Process_URC_SIS
功能    :   命令返回和主动上送GPRS连接信息^SIS
当前命令为at^siso，出现CONNECT后模块进入数据状态，
则更改相应状态
入口参数:
出口参数:
1   :   当前为AT^SISO   或者没有
0   :   其他命令
*********************************************************************/
unsigned char AT_Receive_Process_URC_SIS(unsigned char* pStr)
{
    unsigned char ucRet = 0 ;
    unsigned char ucNum = 0 ;
    unsigned char m = 0 ;
    m =  pStr[8] - 0x30 ;

    AT_CMD_Clear();
    trace_at_cmd_sn(17);
    AT_CMD_Fill(AT_CMD_SET_SISC,m);
    AT_CMD_Fill(AT_CMD_SET_CGATT_DETACH , 0);   //azh

    if(stAtManageState.ucPowerOffNum<10)
    {
        ucNum = (stAtManageState.ucPowerOffNum+1)*5;
        if(stAtManageState.ucReconnectTimes > ucNum)
                {
            AT_CMD_Clear();
            AT_CMD_Fill(AT_CMD_SET_SMSO,0);
            stAtManageState.ucPowerOffNum++;
                }
    }
    else
        stAtManageState.ucPowerOffNum=5;

    if(stModuleRunState.bIsGPRSConnected == 1)
    {
        stModuleRunState.bIsGPRSConnected = 0;
        stModuleRunState.ucCurCommChannel++;
        Reload_Module_RunState(&stModuleRunState);
        stModuleRunState.ucCurCommChannel--;
    }
    ClearCmdBuffer();

    return ucRet;
}

/********************************************************************
函数名称:   AT_Receive_Process_CMGL
功能    :   列出所有短信及其内容
入口参数:
出口参数:
*********************************************************************/

unsigned char AT_Receive_Process_CMGL(unsigned char* pStr)
{
    unsigned char   ucTmp = 0;
    unsigned char   i = 0;
    unsigned char   m = 0;
    unsigned char   n = 0;
    unsigned char   ucRet = 0;

    //  if(stCMDBuffer.ucATCMD == AT_CMD_QUERY_CMGL)
    //  {
    for(i=0;i<30;i++)
    {
        if(pStr[i] == ':')
            m = n = i;
        if(pStr[i] == ',')
        {
            n = i;
            break;
        }
    }
    ucTmp = 0;
    for( i = m + 1 ; i<n ; i++)
    {
        ucTmp *= 10;
        if(pStr[i]>='0' && pStr[i]<='9')
            ucTmp += pStr[i]-'0';
    }
    if(( ucTmp <= 160) && (ucTmp != 0))
    {

        AT_CMD_Fill(AT_CMD_SET_CMGD, ucTmp);
    }
    ucRet = 1;
    //  }
    //azh 140606 在下面函数增加下面函数 以删除短信 不然AT命令都复位 上不了线 但不知是否影响短信的接收！
    ClearCmdBuffer();

    return ucRet;
}


/********************************************************************
函数名称:   AT_Receive_Process_CGREG
功能    :   AT*CGREG?命令返回结果处理

  入口参数: pStr 命令返回字符串指针
  出口参数:
*********************************************************************/
unsigned char AT_Receive_Process_CGREG(char* pStr)
{
    //  unsigned char   ucLen = 0 ;
    /*  if(strstr((char*)pStr , "+CREG: 0\r\n")!=NULL)
    stModuleRunState.bIsSIMExist = 0;

      else if(strstr((char*)pStr , "+CREG: 1\r\n")!=NULL)
      stModuleRunState.bIsSIMExist = 1;

        else if(strstr((char*)pStr , "+CREG: 2\r\n")!=NULL)
        stModuleRunState.bIsSIMExist = 0;

          else if(strstr((char*)pStr , "+CREG: 5\r\n")!=NULL)
          stModuleRunState.bIsSIMExist = 1;

            else
            stModuleRunState.bIsSIMExist = 0;
    //
    if(stCMDBuffer.ucATCMD != AT_CMD_SET_CFUN )
        ClearCmdBuffer();

    AT_CMD_Fill(AT_CMD_QUERY_CSQ,0);        //检查网络信号强度和SIM卡情况

    return 0;
    */
//  unsigned char   ucLen = 0 ;

/*//azh telit
    if((strstr((char*)pStr , "+CREG: 0,1")!=NULL)||(strstr((char*)pStr , "+CREG: 0,5")!=NULL))
    {
       stModuleRunState.bIsSIMExist = 1;
    }

    else
    {
        stModuleRunState.bIsSIMExist = 0;
    }
*/
    if(stCMDBuffer.ucATCMD != AT_CMD_SET_CFUN )
        ClearCmdBuffer();
    AT_CMD_Fill(AT_CMD_QUERY_CSQ,0);        //检查网络信号强度和SIM卡情况

    return 0;
}

/********************************************************************
函数名称:   AT_Receive_Process_CMSERROR
功能    :   命令返回ERROR处理：
可重发时：设置命令发送缓冲区，重发该命令;
超过重发次数：模块重新上电
入口参数:
出口参数:
*********************************************************************/
unsigned char AT_Receive_Process_CMSERROR()
{
    unsigned char ucRet = 0;
    switch(stCMDBuffer.ucATCMD)
    {
    case AT_CMD_SET_CMGS:
        if(stReceiveBuffer.ucRetryCnt >0)
        {
            stCMDBuffer.enuBufferState = STATE_BUFFER_SEND;
        }
        else//不再重发
        {
            stAtManageState.ucERRORCount ++;
            //AT_CMD_Clear();
            stReceiveBuffer.uTimeOutCount = 0;
            stReceiveBuffer.ucRetryCnt  = 0;
            //清除发送任务
            ClearCmdBuffer();

            ATDataListPop();
            DebugPrint((unsigned char*)STR_DEBUG_SMSERROR, sizeof(STR_DEBUG_SMSERROR));

            stSMSBufer.ucIsValid = 0;
        }
        ucRet = 1;
        break;
    default:
        break;
    }
    return ucRet;
}
/********************************************************************
函数名称:   AT_Receive_Process_CMEERROR
功能    :   模块CMEERROR处理
入口参数:
pstr    :   数据区指针
uslen   :   数据区长度
出口参数:

********************************************************************/
//unsigned char AT_Receive_Process_CMEERROR()
//{
//    AT_CMD_Fill(AT_CMD_SET_AT,0);
//    enuATCurrentState = enuATNextState =  STATE_GSM;
//    stAtManageState.uStateTimerCounter = CON_TIMEER_GSMSTATE;
//    return 1;
//}

/********************************************************************
函数名称:   AT_Receive_Process_UNKNOWN
功能    :   处理ATO命令返回CONNECT 与 OK 之间的缓存数据
入口参数:
pstr    :   数据区指针
uslen   :   数据区长度
出口参数:

********************************************************************/
unsigned char AT_Receive_Process_UNKNOWN(unsigned char* pstr)
{
    unsigned char   ucRet = 0;
    if (( stCMDBuffer.ucATCMD == AT_CMD_SET_SISR) && (enuATCurrentState == STATE_GPRS))
    {
        stAtManageState.usDataReceiveLength = 0;
        stReceiveBuffer.ucIsGetVaild = 0;
        stReceiveBuffer.usProcPtr = 0;
        stReceiveBuffer.usLength = 0;
        stAtManageState.uStateTimerCounter = CON_TIMEER_GPRSSTATE;
    }
    return ucRet;
}

/********************************************************************
函数名称:   AT_CHANGE_PWD
功能    :   口令变更处理
入口参数:
出口参数:
*********************************************************************/
unsigned char AT_CHANGE_PWD(void)
{
    if( stModuleComParam.stPINPUK.ucIsPwdValid == 1 )   //设置密码保护状态
    {
        if(stAtManageState.bIsPWDUsed == 0) //当前无密码保护则设置密码启用
        {
            AT_CMD_Fill(AT_CMD_SET_CLCK1, 0);
        }
        else        //当前有密码保护则密码变更
        {
            AT_CMD_Fill(AT_CMD_SET_CPWD, 0);
        }
    }
    else    //取消密码保护状态
    {
        //if(stAtManageState.bIsPWDUsed == 1)   //当前处于密码保护状态，则下发命令取消密码保护
        AT_CMD_Fill(AT_CMD_SET_CLCK0, 0);
    }
    stModParaChInfo.bIsPWDChanged = 0;
    return 1;
}

/********************************************************************
函数名称:   ClearCmdBuffer
功能    :   清除当前命令缓冲区数据
入口参数:
出口参数:
*********************************************************************/
void ClearCmdBuffer(void)
{
    //清除命令
    stCMDBuffer.ucATCMD = AT_CMD_INVALID;
    stCMDBuffer.enuBufferState = STATE_BUFFER_IDLE;
    stCMDBuffer.ucRetryCnt = 0;
    stCMDBuffer.uTimeOutCount = 0;
    stCMDBuffer.ucLength = 0;
}

/********************************************************************
函数名称:   AT_Power
功能    :   手机模块关电处理
终端掉电后，MB_PWCTL信号无效，清除数据队列，
手机模块在1分钟后掉电（等待终端发送掉电告警）；
入口参数:
出口参数:
*********************************************************************/
void AT_Power(void)
{   //模块关电
        //模块关电
    if(stAtManageState.bIsPowerOff == 1 && stAtManageState.uPowerOffTimer == 0)
    {
        enuATCurrentState = enuATNextState = STATE_POWEROFF;
        AT_CMD_Clear();                                 //清除命令队列
        ClearCmdBuffer();
        AT_CMD_Fill(AT_CMD_SET_SMSO,0);                 //为了防止模块突然断电引起损坏，在关电前先对ON/OFF脚操作
        //AT_CMD_Fill(PIN_CMD_DELAY,30);
        AT_CMD_Fill(PIN_CMD_DELAY,AT_DLY_30S);
        AT_CMD_Fill(PIN_CMD_PWCTK,0);                   //模块电源关闭

        DebugPrint((unsigned char*)STR_DEBUG_MBPOWR, sizeof(STR_DEBUG_MBPOWR));
        stAtManageState.uStateTimerCounter  = AT_DLY_60S;//6000;        //

//      NETLED_CLR;
        //      PIN_LED2_CLR;
    }
        //模块关电
    if(ucReset_Mc55_Flag  == 1)
    {
        ucReset_Mc55_Flag = 0;
        enuATCurrentState = enuATNextState = STATE_POWEROFF;
        AT_CMD_Clear();                                 //清除命令队列
        ClearCmdBuffer();
        AT_CMD_Fill(AT_CMD_SET_SMSO,0);                 //为了防止模块突然断电引起损坏，在关电前先对ON/OFF脚操作
        //AT_CMD_Fill(PIN_CMD_DELAY,30);
        AT_CMD_Fill(PIN_CMD_DELAY,AT_DLY_30S);
        AT_CMD_Fill(PIN_CMD_PWCTK,0);                   //模块电源关闭

        if(gucGprs_Module_Type == MC55_GPRS_MODULE)
        {          
        DebugPrint((unsigned char*)STR_RESET_MC55, sizeof(STR_RESET_MC55));
        }
        else if(gucGprs_Module_Type == TELIT_GPRS_MODULE)
        { 
            DebugPrint((unsigned char*)STR_RESET_MC55_T, sizeof(STR_RESET_MC55_T));   
        }
        stAtManageState.uStateTimerCounter  = AT_DLY_60S;//6000;        //
//      NETLED_CLR;
        //      PIN_LED2_CLR;
    }
}

/********************************************************************
函数名称:   AT_CMD_Resend
功能    :   命令超时重发
入口参数:
出口参数:
*********************************************************************/
void AT_CMD_Resend(ENU_BUFFER_STATE  enuDestState)
{
    //可以重发
    if(stReceiveBuffer.ucRetryCnt >0)
    {
        stCMDBuffer.enuBufferState = STATE_BUFFER_SEND;
    }
    else//不再重发
    {
        DebugPrint((unsigned char*)STR_AT_DEBUG_CMDTIMEOUT, sizeof(STR_AT_DEBUG_CMDTIMEOUT));
        ClearCmdBuffer();
        AT_CMD_Clear();
        stReceiveBuffer.uTimeOutCount = 0;
        stReceiveBuffer.ucRetryCnt  = 0;
        enuATSourceState = enuATCurrentState = enuATNextState  = STATE_POWEROFF;
        AT_CMD_Fill(AT_CMD_SET_SMSO ,0);
        stAtManageState.uStateTimerCounter  = 0;
    }
}

/********************************************************************
函数名称:   AT_StrFromStr_ByChr
功能    :   从源字符串取数据，知道遇到标志字符（但不包括）
最长29个字符
入口参数:
出口参数:
*********************************************************************/
//unsigned char AT_StrFromStr_ByChr(char* pStr, char*pDst, char cIndicate)
//{
//    unsigned char   ucLen = 0;
//
//    while( ( ucLen < 30 ) && ( pStr[ucLen] != 0x00 ) && ( pStr[ucLen] != cIndicate) )
//    {
//        pDst[ucLen] = pStr[ucLen];
//        ucLen++;
//    }
//
//    if( ucLen == 30 )
//        ucLen = 0;
//    pDst[ucLen] = 0x00;
//    return ucLen;
//}
/******************************************************************
登陆网络指示

*******************************************************************/

void ConnectIndicate(void)
{
    if(stModuleRunState.bIsGPRSConnected == 0)//不在线
    {    
        if(stAtManageState.uConnectIndicateTimes==0)
        {
            stAtManageState.uConnectIndicateTimes = CON_CONNECTINDICATE_TIMES;//CON_ConnectIndicateTimes;
            if(LOCAL_STATE_IS_HIG(CON_GPRS_LED_STATE))
            {
                CLR_LOCAL_STATE(CON_GPRS_LED_STATE);
                NETLED_CLR;
            }
            else
            {
                SET_LOCAL_STATE(CON_GPRS_LED_STATE);
                NETLED_SET;
            }    

//            if(0 == PORT1_IS_HIGH(P1_RLED_GPRS))
//            {
//                NETLED_SET;
//            }
//            else
//            {
//                NETLED_CLR;
//            }
        }
    }
    else
    {
        NETLED_SET; 
        SET_LOCAL_STATE(CON_GPRS_LED_STATE);
    }    
}
/**********************************************************************
函数名称IP_Check
返回1表示IP错误，返回0表示正确
***********************************************************************/
unsigned char IP_Check(unsigned char ucChannel)
{
    unsigned char ucRet = 0;
    unsigned char i=0;

    for ( i=0; i<15; i++ )
    {
        if ((stModuleComParam.stCh[ucChannel].unChInfo.stIPParam.ucIP[i] != '0') &&
            (stModuleComParam.stCh[ucChannel].unChInfo.stIPParam.ucIP[i] != 0))
        {
            if ((i != 3) && (i != 7) && (i !=11))
            {
                ucRet =1;
                break;
            }
        }
    }
    return ucRet;
}



unsigned char Init_ModChInfo(STMODPARACHINFO *pstModParaChInfo)
{
    unsigned char ucRet = RET_SUCCESS;
    if(pstModParaChInfo != NULL)
    {
        memset(pstModParaChInfo,0,sizeof(STMODPARACHINFO));
    }
    return ucRet;
}
unsigned char Get_ModChInfo(STMODPARACHINFO *pstModParaChInfo)
{
    unsigned char ucRet = RET_ERROR;
    if(pstModParaChInfo != NULL)
    {
        *pstModParaChInfo = stModParaChInfo;
        ucRet = RET_SUCCESS;
    }
    return ucRet;
}
unsigned char Set_ModChInfo(STMODPARACHINFO *pstModParaChInfo)
{
    unsigned char ucRet = RET_ERROR;
    if(pstModParaChInfo != NULL)
    {
        stModParaChInfo = *pstModParaChInfo;
        ucRet = RET_SUCCESS;
    }
    return ucRet;
}
unsigned char IPchange(unsigned char ucChannel,unsigned char *ucIP)
{
    unsigned char ucRet = 0;
    unsigned char i=0,j=0;
    for ( i=0; i<15; i++ )
    {
        if ((stModuleComParam.stCh[ucChannel].unChInfo.stIPParam.ucIP[i] != '0')||(i == 2) || (i == 6) || (i ==10)||(i ==14))
        {
            ucIP[j]=stModuleComParam.stCh[ucChannel].unChInfo.stIPParam.ucIP[i];
            j++;
        }
        else if ((i != 0)&&(stModuleComParam.stCh[ucChannel].unChInfo.stIPParam.ucIP[i-1] != '0')&&(i != 4) && (i != 8) && (i !=12))
        {
            ucIP[j]=stModuleComParam.stCh[ucChannel].unChInfo.stIPParam.ucIP[i];
            j++;

        }
        else if(stModuleComParam.stCh[ucChannel].unChInfo.stIPParam.ucIP[i+1] != '0')
        {
            ucIP[j]=stModuleComParam.stCh[ucChannel].unChInfo.stIPParam.ucIP[i+1];
            j++;
            i++;
        }

        else
        {
            ucIP[j]=stModuleComParam.stCh[ucChannel].unChInfo.stIPParam.ucIP[i+2];
            j++;
            i+=2;
        }

    }
    ucRet=j;
    return ucRet;
}
unsigned char IPportchange(unsigned char ucChannel,unsigned char *ucPort)
{
    unsigned char ucRet = 0;
    unsigned char i=0,j=0,flag=0;
    for ( i=0; i<5; i++ )
    {
        if (stModuleComParam.stCh[ucChannel].unChInfo.stIPParam.ucPort[i] != '0')
        {
            ucPort[j]=stModuleComParam.stCh[ucChannel].unChInfo.stIPParam.ucPort[i];
            j++;
            flag=1;

        }
        else if (flag != 0)
        {
            ucPort[j]=stModuleComParam.stCh[ucChannel].unChInfo.stIPParam.ucPort[i];
            j++;

        }
        else if(stModuleComParam.stCh[ucChannel].unChInfo.stIPParam.ucPort[i+1] != '0')
        {
            ucPort[j]=stModuleComParam.stCh[ucChannel].unChInfo.stIPParam.ucPort[i+1];
            j++;
            i++;
            flag=1;
        }

        else if(stModuleComParam.stCh[ucChannel].unChInfo.stIPParam.ucPort[i+2] != '0')
        {
            ucPort[j]=stModuleComParam.stCh[ucChannel].unChInfo.stIPParam.ucPort[i+2];
            j++;
            i+=2;
            flag=1;
        }
        else if(stModuleComParam.stCh[ucChannel].unChInfo.stIPParam.ucPort[i+3] != '0')
        {
            ucPort[j]=stModuleComParam.stCh[ucChannel].unChInfo.stIPParam.ucPort[i+3];
            j++;
            i+=3;
            flag=1;
        }
        else
        {
            ucPort[j]=stModuleComParam.stCh[ucChannel].unChInfo.stIPParam.ucPort[i+4];
            j++;
            i+=4;
        }


    }
    ucRet=j;
    return ucRet;
}
//unsigned char IPportchange2(unsigned short usinPort,unsigned char *ucPort)
//{
//    unsigned char ucRet = 0;
//    unsigned char i=0,j=0,flag=0;
//    unsigned char ucinPort[5];
//    unsigned short usVal;
//    usVal=usinPort;
//    for(i=0;i<5;i++)
//    {
//        ucinPort[5-i-1] = ((unsigned char)(usVal % 10))+0x30;
//        usVal/=10;
//    }
//
//    for ( i=0; i<5; i++ )
//    {
//        if (ucinPort[i]!= '0')
//        {
//            ucPort[j]=ucinPort[i];
//            j++;
//            flag=1;
//
//        }
//        else if (flag != 0)
//        {
//            ucPort[j]=ucinPort[i];
//            j++;
//
//        }
//        else if(ucinPort[i+1] != '0')
//        {
//            ucPort[j]=ucinPort[i+1];
//            j++;
//            i++;
//            flag=1;
//        }
//
//        else if(ucinPort[i+2] != '0')
//        {
//            ucPort[j]=ucinPort[i+2];
//            j++;
//            i+=2;
//            flag=1;
//        }
//        else if(ucinPort[i+3] != '0')
//        {
//            ucPort[j]=ucinPort[i+3];
//            j++;
//            i+=3;
//            flag=1;
//        }
//        else
//        {
//            ucPort[j]=ucinPort[i+4];
//            j++;
//            i+=4;
//        }
//
//
//    }
//    ucRet=j;
//    return ucRet;
//}

void ASCTOHEX (unsigned char *Hexdata, unsigned char *Ascdata, unsigned short len)
{
    int i;

    for(i = 0; i < len; i++)
    {
        if((i % 2) == 0)                                //一个分成俩
        {
            Hexdata[i] = Ascdata[i / 2] >> 4;
        }
        else
        {
            Hexdata[i] = Ascdata[i / 2] & 0xf;
        }

        if(Hexdata[i] < 0xa)                            //逻辑上不会存在超过0xf的数
        {
            Hexdata[i] += '0';
        }
        else
        {
            Hexdata[i] += 0x37;
        }
    }
}
unsigned char Get_PhoneNum_Str(unsigned char *pucPhone,unsigned char ucPhoneLen,unsigned char *pstr)
{
    unsigned char m=0,i=0;
    unsigned char ucTemp=0;
    for(i=0;i<ucPhoneLen;i++)
    {
        ucTemp = pucPhone[i];
        if((ucTemp & 0xF0) == 0xF0)
        {
            pstr[m] =0;
            break;
        }
        pstr[m++] = (ucTemp >> 4)+'0';
        if((ucTemp & 0x0F) == 0x0F)
        {
            pstr[m] =0;
            break;
        }
        pstr[m++] = (ucTemp & 0x0F)+'0';
    }
    return m;
}
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
unsigned char Get_Signal_Strenth(void)
{
	unsigned char value=0;
	
    if(stModuleRunState.ucSignIntension > 31)
    {
    	value = 31;
    }
    else
    {
    	value = stModuleRunState.ucSignIntension;
    }		
    if(stModuleRunState.bIsGPRSConnected == 1)//在线
    {
    	value += 0x80;
    }

    return value;
}
#endif

/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_RunPara_Info.c
 Description    : 国网规约对应运行数据
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-08-15    azh     the original version
******************************************************************************/
#include    "..\inc\global_config.h"

//static unsigned char   Get_BitNums(unsigned char ucByte);

unsigned char Main_DI_Para_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,unsigned char *pucRecvBuffer)
{

    unsigned char   ucNum=0,ucNum2=0,ucPos=0,ucNum7=0,ucNum8=0;
    unsigned char ucLen=CON_DI_LEN_CHANGED;
    unsigned char i=0;
    unsigned char   ucRet=RET_SUCCESS;

    ucNum = pucRecvBuffer[0];
    ucNum2 = pucRecvBuffer[8];
//  ucNum3 = pucRecvBuffer[4];
//  ucNum4 = pucRecvBuffer[1];
//  ucNum5 = pucRecvBuffer[20];
//  ucNum6 = pucRecvBuffer[21+ucNum5];
    ucNum7= pucRecvBuffer[20];
    ucNum8 = pucRecvBuffer[21+ucNum7];

    ClearDIInfo( stRetDIInfo );

    if(usDI < CON_PARA_BEG_CODE || usDI > CON_PARA_END_CODE)    //错误
        return ucRet;

    switch(usDI)
    {
    case    CON_PARA_F1_COMMPARA:   //F1：终端通信参数设置
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,6,                    CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F2_RELAYPARA:  //F2：终端中继转发设置
        ucNum = pucRecvBuffer[0] & 0x7F;
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,(unsigned char)(ucNum*2+1),CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F3_STAIPADDR:  //F3:主站IP地址和端口
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,28,                   CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F4_STAPHONENO: //F4：主站电话号码和短信中心号码
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,16,                   CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F5_TERENCRYPT: //F5：终端密码设置
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,3,                    CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F6_TERGROUPADDR:   //F6：终端组地址设置
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,16,                   CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F7_IPADDR: //F7：终端抄表日设置
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,6,                    CON_DI_TYPE_INVALID);
            0,(unsigned char)(24+ucNum7+ucNum8),                    CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F8_ALRCTLWORD: //F8：终端事件记录配置设置
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,8,                    CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F9_CONFIGNUM:      //F9：终端配置数量表
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,16,                   CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F10_METERPARA:     //F10：终端电能表/交流采样装置配置参数
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,(unsigned char)(2+(27*ucNum)),    CON_DI_TYPE_INVALID);
        break;
//  case    CON_PARA_F11_PULSEPARA:     //F11：终端脉冲配置参数
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,(unsigned char)(1+(5*ucNum)),         CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F12_DINPUTPARA:    //F12：终端状态量输入参数
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,2,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F13_ANALOGPARA:    //F13：终端电压/电流模拟量配置参数
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,(unsigned char)(1+(3*ucNum)), CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F14_SUMGPARA:  //F14：终端总加组配置参数
//
//      ucNum=pucRecvBuffer[0];
//      if(ucNum<=8)
//      {
//          ucLen=1;
//          for(i=0;i<ucNum;i++)
//          {
//              ucPos=ucLen+1;
//              if((ucLen+pucRecvBuffer[ucPos]+2)<0x100)
//                  ucLen+=(pucRecvBuffer[ucPos]+2);
//              else
//              {
//                  ucLen=CON_DI_LEN_CHANGED;
//                  break;
//              }
//          }
//      }
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,ucLen, CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F15_POWVALDIFF:    //F15：有功总电能量差动越限事件参数设置
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,(unsigned char)(1+(9*ucNum)), CON_DI_TYPE_INVALID);
//      break;
    case    CON_PARA_F16_PDPINFO:   //F16：虚拟专网用户名、密码
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
        //  0,32,                   CON_DI_TYPE_INVALID);
            0,64,                   CON_DI_TYPE_INVALID);
        break;
//  case    CON_PARA_F17_KEEPPOWER: //F17：终端保安定值
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,2,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F18_POWCTRLTIMEPER:    //F18：终端功控时段
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,12,                   CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F19_POWCTRLCOEF:   //F19：终端时段功控定值浮动系数
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,1,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F20_ENERCTRLCOEF:  //F20：终端月电能量控定值浮动系数
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,1,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F21_PERTIMEFEE:    //F21：终端电能量费率时段和费率数
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,49,                   CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F22_FEEPARA:   //F22：终端电能量费率
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,(unsigned char)(1+(4*ucNum)), CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F23_URGEALARMPARA: //F23：终端催费告警参数
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,3,                    CON_DI_TYPE_INVALID);
//      break;
    case    CON_PARA_F24_COPYINTER: //F24：终端抄表间隔设置
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,1,                    CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F25_MPBASEPARA:        //F25：测量点基本参数
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,11,                   CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F26_MPLIMIT:           //F26：测量点限值参数
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,57,                   CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F27_MPFREEZEPARA:      //F27：测量点数据冻结参数
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
        //  0,(unsigned char)(1+(2*ucNum)),     CON_DI_TYPE_INVALID);
            0,24,                   CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F28_MPPOWRATEPARA:     //F28：测量点功率因数分段限值
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,4,                    CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F29_METERADDR:     //F28：测量点功率因数分段限值
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,12,                   CON_DI_TYPE_INVALID);
        break;
    case CON_PARA_F30_METERCOPYFLAG:
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,1,                    CON_DI_TYPE_INVALID);
        break;
    case CON_PARA_F31_SLAVENODE:
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,(unsigned char)(1+6*ucNum),                   CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F34_PORTPROPERTY:
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,(unsigned char)(1+6*ucNum),                   CON_DI_TYPE_INVALID);
        break;
    case CON_PARA_F35_EMPHAFLAG:
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,(unsigned char)(1+2*ucNum),                   CON_DI_TYPE_INVALID);
        break;

//   case    CON_PARA_F33_SUMGFREEZEPARA:    //F33：总加组数据冻结参数
//       SetDIInfo(stRetDIInfo,
//           CON_DI_HAVE,
//           CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//           CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//           CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//           0,4,                    CON_DI_TYPE_INVALID);
//       break;
    case    CON_PARA_F33_SUMGFREEZEPARA:    //F33：终端抄表运行参数设置

        ucNum=pucRecvBuffer[0];
        if(ucNum<=CON_PORT_NUM)
        {
            ucLen=1;
            for(i=0;i<ucNum;i++)
            {
                ucPos=ucLen+13;
                if( (pucRecvBuffer[ucPos]*4 + ucLen +14) < 0x100 )
                {
                    ucLen+=(pucRecvBuffer[ucPos]*4+14);
                }
                else
                {
                    ucLen=CON_DI_LEN_CHANGED;
                    break;
                }
            }
        }
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,ucLen, CON_DI_TYPE_INVALID);
        break;
    case CON_PARA_F37_LINKPARA:
        ucNum = pucRecvBuffer[6];
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,(unsigned char)(7+4*ucNum), CON_DI_TYPE_INVALID);
        break;
    case CON_PARA_F38_JOB1PARA:
    case CON_PARA_F39_JOB2PARA:
        ucNum = pucRecvBuffer[1];
        ucLen =2;
        for(i=0;i<ucNum;i++)
        {
            ucNum2 = pucRecvBuffer[ucLen+1];
            ucLen = ucLen +2 + ucNum2;
        }
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,ucLen, CON_DI_TYPE_INVALID);
        break;
//  case    CON_PARA_F41_TIMEPERSETTING:    //F41：时段功控定值
//      ucNum=pucRecvBuffer[0]&0x07;
//      ucNum=Get_BitNums(ucNum);
//      ucLen=1;
//      for(i=0;i<ucNum;i++)
//      {
//          ucPos=ucLen;
//          ucNum2=Get_BitNums(pucRecvBuffer[ucPos]);
//          ucLen+=(ucNum2+ucNum2+1);
//      }
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,ucLen,    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F42_RESTCTRL:  //F42：厂休功控参数
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,6,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F43_POWERSLIP: //F43：功率控制的功率计算滑差时间
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,1,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F44_SALESTOP:  //F44：营业报停控参数
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,8,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F45_POWERCTRLTURN: //F45：功控轮次设定
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,1,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F46_MONTHENERCTRL: //F46：月电量控定值
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,4,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F47_BUYENERCTRL:   //F47：购电量控参数
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//      //  0,21,                   CON_DI_TYPE_INVALID);
//          0,17,                   CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F48_ENERCTRLTURN:  //F48：电控轮次设定
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,1,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F49_CTRLALRTIME:   //F49：功控告警时间
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,1,                    CON_DI_TYPE_INVALID);
//      break;
    case    CON_PARA_F57_SOUNDALARM:    //F57：终端声音告警允许∕禁止设置
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,3,                    CON_DI_TYPE_INVALID);
        break;
//  case    CON_PARA_F58_KEEPAUTO:      //F58：终端自动保电参数
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,1,                    CON_DI_TYPE_INVALID);
//      break;
    case    CON_PARA_F59_MTRERRORPARA:  //F59：电能表异常判别阈值设定
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,4,                    CON_DI_TYPE_INVALID);
        break;
//  case    CON_PARA_F60_HARMONPARA:    //F60：谐波限值
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,80,                   CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F61_DCAINPARA: //F61：直流模拟量接入参数
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,1,                    CON_DI_TYPE_INVALID);
//      break;

    case    CON_PARA_F62_COMMTYPE:      //F62 虚拟专网工作方式
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,5,                    CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F63_FLUXLIMIT:     //F63 日通信流量门限设置
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,4,                    CON_DI_TYPE_INVALID);
        break;

    case    CON_PARA_F65_UPDATA1PARA:   //F65：定时发送1类数据任务设置
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,(unsigned char)(9+(4*ucNum2)),    CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F66_UPDATA2PARA:   //F66：定时发送2类数据任务设置
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,(unsigned char)(9+(4*ucNum2)),    CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F67_UPDATA1CTRL:   //F67：定时发送1类数据任务启动/停止设置
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,1,                    CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F68_UPDATA2CTRL:   //F68：定时发送2类数据任务启动/停止设置
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,1,                    CON_DI_TYPE_INVALID);
        break;
//  case    CON_PARA_F73_CAPPARA:   //F73：电容器参数
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,48,                   CON_DI_TYPE_INVALID);       //changed by cfh 10-04-03
//      break;
//  case    CON_PARA_F74_CAPRUNPARA:    //F74：电容器投切运行参数
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,10,                   CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F75_CAPRELAYPARA:  //F75：电容器保护参数
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,16,                   CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F76_CAPCTRL:   //F76：电容器投切控制方式
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,1,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F81_DCARATE:   //F81：直流模拟量输入变比
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,4,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F82_DCALIMIT:  //F82：直流模拟量限值
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,4,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F83_DCAFREESE: //F83：直流模拟量冻结参数
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,1,                    CON_DI_TYPE_INVALID);
//      break;
    case    CON_PARA_F89_ADDR:
    case    CON_PARA_F207_ADDR:
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,4,                    CON_DI_TYPE_INVALID);
        break;
//azh 170916
    case    CON_PARA_F90_WIRELESS:
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,2,                    CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F91_LOCATION:
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,10,                   CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F150_SNSTATE:
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,0xff,                 CON_DI_TYPE_INVALID);//azh 0xff 这个特殊了，实际长度是256+256
        break;

    }
    return ucRet;
}


unsigned char Main_DI_ParaRead_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,unsigned char *pucRecvBuffer)
{
    unsigned char   ucNum=0;///ucNum2=0,ucNum3=0,ucNum4=0,ucPos=0;
//    unsigned char ucLen=CON_DI_LEN_CHANGED;
//    unsigned char i=0;
    unsigned char   ucRet=RET_SUCCESS;

    ucNum = pucRecvBuffer[0];
//  ucNum2 = pucRecvBuffer[8];
//  ucNum3 = pucRecvBuffer[4];
//  ucNum4 = pucRecvBuffer[1];
    ClearDIInfo( stRetDIInfo );

    if(usDI < CON_PARA_BEG_CODE || usDI > CON_PARA_END_CODE)    //错误
        return ucRet;
    SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,0,    CON_DI_TYPE_INVALID);

    switch(usDI)
    {
    case CON_PARA_F10_METERPARA:
        stRetDIInfo->ucCommandLen= (ucNum+1)*2;
        break;
//  case CON_PARA_F11_PULSEPARA:
//  case CON_PARA_F13_ANALOGPARA:
//  case CON_PARA_F14_SUMGPARA:
//  case CON_PARA_F15_POWVALDIFF:
//      stRetDIInfo->ucCommandLen = (ucNum+1);
//      break;
    case CON_PARA_F33_SUMGFREEZEPARA:
    case CON_PARA_F34_PORTPROPERTY:
        stRetDIInfo->ucCommandLen = ucNum + 1;
        break;
    case CON_PARA_F38_JOB1PARA:
    case CON_PARA_F39_JOB2PARA:
        ucNum = pucRecvBuffer[1];
        stRetDIInfo->ucCommandLen = ucNum + 2;
//  case CON_PARA_F35_EMPHAFLAG:
//      stRetDIInfo->ucCommandLen = 1 + 2 * ucNum;
        break;
    }
    return ucRet;

}


unsigned char  Main_DI_Config_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,unsigned char ucNum4)
{
    unsigned char   ucRet=RET_SUCCESS;
    ClearDIInfo( stRetDIInfo );

    if(usDI < CON_CONFIG_BEG_CODE || usDI > CON_CONFIG_END_CODE)    //错误
        return ucRet;

    SetDIInfo(stRetDIInfo,
    CON_DI_HAVE,
    CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
    CON_DI_OPERATE_DISABLE, CON_DI_SETPURVIEW_ADMIN,
    CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
    CON_DI_LEN_CHANGED,0,   CON_DI_TYPE_INVALID);

    switch(usDI)
    {
    case    CON_CONFIG_F1_VERSION:      //F1 终端版本信息
            stRetDIInfo->ucContentLen = 41;
            break;
    case    CON_CONFIG_F2_INTERFACE:    //F2 终端输入、输出及通信端口配置
            stRetDIInfo->ucContentLen = 41;
            break;
    case    CON_CONFIG_F3_OTHER:        //F3 终端其他配置
            stRetDIInfo->ucContentLen = 17;
            break;
    case    CON_CONFIG_F4_PARA:         //F4 终端参数配置（本终端软硬件版本支持的参数F1~F68）
            stRetDIInfo->ucContentLen = 10;
            break;
    case    CON_CONFIG_F5_CTRL:         //F5 终端控制配置（本终端软硬件版本支持的控制命令F1-F38）
            stRetDIInfo->ucContentLen = 6;
            break;
    case    CON_CONFIG_F6_DATA1:        //F6 终端1类数据配置（本终端软硬件版本支持的1类数据F1-F121）
            stRetDIInfo->ucContentLen = 20;
            break;
    case    CON_CONFIG_F7_DATA2:        //F7 终端2类数据配置（本终端软硬件版本支持的2类数据F1=F108）
            stRetDIInfo->ucContentLen = 26;
            break;
    case    CON_CONFIG_F8_ERC:          //F8 终端事件记录配置（本终端软硬件版本支持的事件记录ERC1-ERC33）
            stRetDIInfo->ucContentLen = 6;
            break;
//azh 170920
    case    CON_CONFIG_F9_REMOTEVER:          //F9远程通信模块版本信息
            stRetDIInfo->ucContentLen = 46;
            break;
    case    CON_CONFIG_F10_LOCALVER:          //F10本地通信模块版本信息
            stRetDIInfo->ucContentLen = 15;
            break;
    }

    return ucRet;
}


unsigned char  Main_DI_Login_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,unsigned char ucNum4)
{
    unsigned char   ucRet=RET_SUCCESS;
    ClearDIInfo( stRetDIInfo );

    if(usDI < CON_LOGIN_BEG_CODE || usDI > CON_LOGIN_END_CODE)  //错误
        return ucRet;
    if(usDI == CON_LOGIN_F1_REQUEST)
    {
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_DISABLE, CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,16,   CON_DI_TYPE_INVALID);
    }
    else if(usDI == CON_LOGIN_F2_ANSWER)
    {
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_DISABLE, CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            16,0,   CON_DI_TYPE_INVALID);
    }
    return ucRet;
}


unsigned char  Main_DI_Taskdata_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,unsigned char ucNum4)
{
    unsigned char   ucRet=RET_SUCCESS;
    ClearDIInfo( stRetDIInfo );

    if(usDI < CON_TASKDATA_REQ_BEG_CODE || usDI > CON_TASKDATA_REQ_END_CODE)    //错误
        return ucRet;

    SetDIInfo(stRetDIInfo,
    CON_DI_HAVE,
    CON_DI_TYPE_DATA,       CON_DI_MPTYPE_NORMAL,
    CON_DI_OPERATE_DISABLE, CON_DI_SETPURVIEW_ADMIN,
    CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
    CON_DI_LEN_CHANGED,0,   CON_DI_TYPE_INVALID);

    switch(usDI)
    {
    case    CON_TASKDATA_REQ_F1:        //F1 1类数据
            //stRetDIInfo->ucContentLen = 0;
            stRetDIInfo->ucCommandLen = 0;
            break;
    case    CON_TASKDATA_REQ_F2:        //F2 2类数据
            //stRetDIInfo->ucContentLen = 5;
            stRetDIInfo->ucCommandLen = 5;//azh 121226
            
            break;
    }
    return ucRet;
}



unsigned char  Main_DI_Ctrl_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,unsigned char ucNum4)
{
    unsigned char   ucRet=RET_SUCCESS;
    ClearDIInfo( stRetDIInfo );

    if(usDI < CON_CTRL_BEG_CODE || usDI > CON_CTRL_END_CODE)    //错误
        return ucRet;

    switch(usDI)
    {
//    case    CON_CTRL_F1_YK_OPEN:        //F1 遥控跳闸
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,1,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_CTRL_F2_YK_CLOSE:       //F2 遥控允许合闸
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
//
//    case CON_CTRL_F9_TIMEPER_ON:        //F9时段功控投入
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,2,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F10_RESTCTRL_ON:      //F10厂休功控投入
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F11_SALESTOP_ON:      //F11营业报停功控投入
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F12_POWERLOWER_ON:    //F12当前功率下浮控投入
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,8,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F15_MONTHENER_ON:     //F15月电控投入
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F16_BUYENER_ON:       //F16购电控投入
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F17_TIMEPER_OFF:      //F17时段功控解除
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F18_RESTCTRL_OFF:     //F18厂休功控解除
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F19_SALESTOP_OFF:     //F19营业报停功控解除
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F20_POWERLOWER_OFF:   //F20当前功率下浮控解除
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F23_MONTHENER_OFF:    //F23月电控解除
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F24_BUYENER_OFF:      //F24购电控解除
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F25_KEEPPOWER_ON:     //F25终端保电投入
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,1,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F26_URGEALARM_ON:     //F26催费告警投入
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
    case CON_CTRL_F27_TALK_ON:          //F27允许终端与主站通话
            SetDIInfo(stRetDIInfo,
                CON_DI_HAVE,
                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
                0,0,                    CON_DI_TYPE_INVALID);
            break;
//    case CON_CTRL_F28_REMOVE_ON:        //F28终端剔除投入
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
    case CON_CTRL_F29_UPACTIVE_ON:      //F29允许终端主动上报
            SetDIInfo(stRetDIInfo,
                CON_DI_HAVE,
                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
                0,0,                    CON_DI_TYPE_INVALID);
            break;
//azh 170920
    case CON_CTRL_F30_SETMETERTIME_SWITCH:          //F30终端对电能表对时功能的开启与关闭
            SetDIInfo(stRetDIInfo,
                CON_DI_HAVE,
                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
                0,1,                    CON_DI_TYPE_INVALID);
            break;
    case CON_CTRL_F31_SETTIME:          //F31对时命令
            SetDIInfo(stRetDIInfo,
                CON_DI_HAVE,
                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
                0,6,                    CON_DI_TYPE_INVALID);
            break;
    case CON_CTRL_F32_MESSAGE:          //F32中文信息
            SetDIInfo(stRetDIInfo,
                CON_DI_HAVE,
                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
                0,(unsigned char)(2+ucNum4),    CON_DI_TYPE_INVALID);
            break;
//    case CON_CTRL_F33_KEEPPOWER_OFF:    //F33终端保电解除
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F34_URGEFEE_OFF:      //F34催费告警解除
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
    case CON_CTRL_F35_TALK_OFF:         //F35禁止终端与主站通话
            SetDIInfo(stRetDIInfo,
                CON_DI_HAVE,
                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
                0,0,                    CON_DI_TYPE_INVALID);
            break;
//    case CON_CTRL_F36_REMOVE_OFF:       //F36终端剔除解除
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
    case CON_CTRL_F37_UPACTIVE_OFF:     //F37禁止终端主动上报
    case CON_CTRL_F38_SMS_ACTIVE:       //F38短信激活
    case CON_CTRL_F39_DISCONECT:
            SetDIInfo(stRetDIInfo,
                CON_DI_HAVE,
                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
                0,0,                    CON_DI_TYPE_INVALID);
            break;
//    case CON_CTRL_F41_CAPCTRL_ON:       //F41电容控制投入
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,2,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F42_CAPCTRL_OFF:      //F42电容控制解除
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,2,                    CON_DI_TYPE_INVALID);
//            break;
/*  case  CON_CTRL_F88_TIMEREVISE:      //F88精确对时命令
            SetDIInfo(&stRetDIInfo,
                CON_DI_HAVE,
                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
                0,2,                    CON_DI_TYPE_INVALID);
            break;
*/
    case CON_CTRL_F49:
    case CON_CTRL_F50:
    case CON_CTRL_F51:
            SetDIInfo(stRetDIInfo,
                CON_DI_HAVE,
                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
                0,1,                    CON_DI_TYPE_INVALID);
            break;
//azh 170920
    case CON_CTRL_F149_METERSELF_SWITCH:          //F149电能表通信参数自动维护功能开启与关闭
            SetDIInfo(stRetDIInfo,
                CON_DI_HAVE,
                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
                0,1,                    CON_DI_TYPE_INVALID);
            break;
    case CON_CTRL_F150_RESETSNSTATE_SWITCH:          //F150复位测量点参数变更标志
            SetDIInfo(stRetDIInfo,
                CON_DI_HAVE,
                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
                0,0,                    CON_DI_TYPE_INVALID);
            break;
    }
    return ucRet;
}

unsigned char  Main_DI_ExtCmd_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,unsigned char ucNum2)
{
    unsigned char   ucRet=RET_SUCCESS;
    ClearDIInfo( stRetDIInfo );

    if(usDI < CON_EXTCMD_BEG_CODE || usDI > CON_EXTCMD_END_CODE)    //错误
        return ucRet;

    switch(usDI)
    {
//    case    CON_EXTCMD_F1:          //F1 内存读
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,12,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F2:          //F2 内存写
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,28,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F3:          //F3 DATAFLASH读
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,12,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F4:          //F4 DATAFLASH写
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,28,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F5:          //F5 看门狗测试，让程序进入死循环
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,8,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F6:          //F6 手机模块通讯检测
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F7:          //F7 终端状态检测
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
    case    CON_EXTCMD_F10:         //F10 终端地址10/16进制切换
            SetDIInfo(stRetDIInfo,
                CON_DI_HAVE,
                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
                0,6,                    CON_DI_TYPE_INVALID);
            break;
//    case    CON_EXTCMD_F11:         //F11 中文信息读取
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,5,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F21:     //F21 GR任务读
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F22:     //F22 GR任务配置
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,(unsigned char)(32+2*ucNum2),     CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F23:     //F23 GR任务信息读
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F24:     //F24 GR任务信息保存
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F31:     //F31 脉冲需量清零
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F33:     //F33 脉冲底度设置内容全
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,20,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F40:     //F40 设置内表数据
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,CON_DI_LEN_CHANGED,   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F41:     //F41 读取内表数据
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,6,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F51:     //F51 终端地址读
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F52:     //F52 终端序号读
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F53:     //F53 终端地址行政区号写
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,8,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F54:     //F54 终端序号写
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,12,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F71:     //F71 蜂鸣器控制
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,13,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F72:     //F72 修改时钟
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,6,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F73:     //F73 查询通讯波特率
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,5,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F74:     //F74 修改通讯波特率
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,12,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F75:     //F75 读取液晶轮显显示参数
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F76:     //F76 读取液晶操作显示参数
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,5,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F77:     //F77 读取液晶特殊显示参数
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F78:     //F78 修改液晶轮显显示参数
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,38,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F79:     //F79 修改液晶操作显示参数
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,39,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F80:     //F80 修改液晶特殊显示参数
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,8,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F81:     //F81 启用SIM卡的pin
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,6,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F82:     //F82 修改SIM卡的pin
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,8,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F83:     //F83 用PUK设置SIM卡的pin
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,8,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F84:     //F84 SIM卡的pin相关状态
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F85:     //F85 GSM/GPRS切换
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,5,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F86:     //F86 读功率控制恢复时间长度
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F87:     //F87 写功率控制恢复时间长度
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,5,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F89:     //F89 读功率控制越限确认时间
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F90:     //F90 写功率控制越限确认时间
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,5,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F91:     //F91 读自定义调整参数
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F92:     //F92 写自定义调整参数
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,16,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F93:     //F93 读取终端电压两位小数电流四位小数
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F94:     //F94 任务初始化
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,13,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F95:     //F95 8025时钟矫正
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,8,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F96:     //F96 液晶对比度设置
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,5,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F97:     //F97　设置表计通迅名和密码
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,20,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F98:     //F98　读取表计通迅名和密码
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F99:     //F99 设置GPRS接入的PDP用户名、密码
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,37,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F100:        //F100 读取GPRS接入的PDP用户名、密码
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F101:        //F101 设置GPRS重连间隔时间
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,6,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F102:        //F102 读取GPRS重连间隔时间
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F109:        //F109 设置终端工作模式
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,5,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F110:        //F110 读取终端工作模式
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F111:        //F111 设置CT类型
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,5,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F112:        //F112 读取CT类型
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//
//    case    CON_EXTCMD_F113:        //F113 设置自学习有效标志
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,5,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F114:        //F114 读取自学习有效标志
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F115:        //F115 设置二次侧开路阈值
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,10,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F116:        //F116 读取二次侧开路阈值
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F117:        //F117 设置一次侧短路阈值
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,10,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F118:        //F118 读取一次侧短路阈值
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F119:        //F119 读取阻抗值
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F120:            //F120 设置AES密码
//        SetDIInfo(stRetDIInfo,
//            CON_DI_HAVE,
//            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//            0,37,                   CON_DI_TYPE_INVALID);
//        break;
//    case    CON_EXTCMD_F121:            //F120 读取AES密码
//        SetDIInfo(stRetDIInfo,
//            CON_DI_HAVE,
//            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//            0,4,                    CON_DI_TYPE_INVALID);
//    case    CON_EXTCMD_F122:            //F122设置解除网络断线保电
//        SetDIInfo(stRetDIInfo,
//            CON_DI_HAVE,
//            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//            0,5,                    CON_DI_TYPE_INVALID);
//        break;
//    case    CON_EXTCMD_F123:            //F123读取温度
//        SetDIInfo(stRetDIInfo,
//            CON_DI_HAVE,
//            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//            0,4,                    CON_DI_TYPE_INVALID);
//        break;
//    case    CON_EXTCMD_F124:            //F124 读取校表EEPROM的参数
//        SetDIInfo(stRetDIInfo,
//            CON_DI_HAVE,
//            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//            0,5,                    CON_DI_TYPE_INVALID);
//        break;
//    case    CON_EXTCMD_F125:            //F125 设置校表EEPROM的参数
//        SetDIInfo(stRetDIInfo,
//            CON_DI_HAVE,
//            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//            0,9,                    CON_DI_TYPE_INVALID);
//        break;
//    case    CON_EXTCMD_F126:
//        SetDIInfo(stRetDIInfo,
//            CON_DI_HAVE,
//            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//            0,6,                    CON_DI_TYPE_INVALID);
//        break;

    }
    return ucRet;
}

unsigned char  Main_DI_DataTrans_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,unsigned char *pucBuffer)
{
    unsigned char   ucRet=RET_SUCCESS;
    unsigned char ucNum=0,ucNum1 = 0;

    ClearDIInfo( stRetDIInfo );

    if(usDI < CON_DATATRANS_BEG_CODE || usDI > CON_DATATRANS_END_CODE)
        return ucRet;

    switch(usDI)
    {
    case    CON_DATATRANS_F1:           //F1 数据转发
        ucNum = pucBuffer[4];
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
        //  0,(unsigned char)(5+ucNum), CON_DI_TYPE_INVALID);
            0,(unsigned char)(6+ucNum), CON_DI_TYPE_INVALID);       //转发的长度为2个字节的，需要改.cfh 10-03-31
        break;
    case CON_DATATRANS_F9:
        ucNum = pucBuffer[1];
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,(unsigned char)(2+6*ucNum+6+5),   CON_DI_TYPE_INVALID);
        break;
    case CON_DATATRANS_F10:
    case CON_DATATRANS_F11:
        ucNum = pucBuffer[1];
        ucNum1 = pucBuffer[2+6*ucNum+6+2];
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,(unsigned char)(2+6*ucNum+6+3+ucNum1),    CON_DI_TYPE_INVALID);
        break;

    }
    return ucRet;
}

unsigned char  Main_DI_FileTrans_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI)
{
    unsigned char   ucRet=RET_SUCCESS;
    ClearDIInfo( stRetDIInfo );

    if(usDI < CON_FILETRANS_BEG_CODE || usDI > CON_FILETRANS_END_CODE)
        return ucRet;

    switch(usDI)
    {
    case    CON_FILETRANS_F1:           //F1 文件传输方式1
            SetDIInfo(stRetDIInfo,
                CON_DI_HAVE,
                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
                0,0,                    CON_DI_TYPE_INVALID);
            break;
    case    CON_FILETRANS_F100:         //F100 自定义程序下装
            SetDIInfo(stRetDIInfo,
                CON_DI_HAVE,
                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
                0,0,                    CON_DI_TYPE_INVALID);
            break;
    }
    return ucRet;
}
//unsigned char Get_BitNums(unsigned char ucByte)
//{//获得位置1的个数
//  unsigned char ucNum=0,i=0;
//  for(i=0;i<8;i++)
//  {
//      if((ucByte&0x01)==0x01)
//          ucNum++;
//      ucByte=ucByte>>1;
//  }
//  return ucNum;
//}

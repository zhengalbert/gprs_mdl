/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Main_RunPara_Info.c
 Description    : ������Լ��Ӧ��������
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-08-15    azh     the original version
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

    if(usDI < CON_PARA_BEG_CODE || usDI > CON_PARA_END_CODE)    //����
        return ucRet;

    switch(usDI)
    {
    case    CON_PARA_F1_COMMPARA:   //F1���ն�ͨ�Ų�������
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,6,                    CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F2_RELAYPARA:  //F2���ն��м�ת������
        ucNum = pucRecvBuffer[0] & 0x7F;
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,(unsigned char)(ucNum*2+1),CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F3_STAIPADDR:  //F3:��վIP��ַ�Ͷ˿�
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,28,                   CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F4_STAPHONENO: //F4����վ�绰����Ͷ������ĺ���
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,16,                   CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F5_TERENCRYPT: //F5���ն���������
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,3,                    CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F6_TERGROUPADDR:   //F6���ն����ַ����
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,16,                   CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F7_IPADDR: //F7���ն˳���������
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,6,                    CON_DI_TYPE_INVALID);
            0,(unsigned char)(24+ucNum7+ucNum8),                    CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F8_ALRCTLWORD: //F8���ն��¼���¼��������
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,8,                    CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F9_CONFIGNUM:      //F9���ն�����������
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,16,                   CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F10_METERPARA:     //F10���ն˵��ܱ�/��������װ�����ò���
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,(unsigned char)(2+(27*ucNum)),    CON_DI_TYPE_INVALID);
        break;
//  case    CON_PARA_F11_PULSEPARA:     //F11���ն��������ò���
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,(unsigned char)(1+(5*ucNum)),         CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F12_DINPUTPARA:    //F12���ն�״̬���������
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,2,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F13_ANALOGPARA:    //F13���ն˵�ѹ/����ģ�������ò���
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,(unsigned char)(1+(3*ucNum)), CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F14_SUMGPARA:  //F14���ն��ܼ������ò���
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
//  case    CON_PARA_F15_POWVALDIFF:    //F15���й��ܵ������Խ���¼���������
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,(unsigned char)(1+(9*ucNum)), CON_DI_TYPE_INVALID);
//      break;
    case    CON_PARA_F16_PDPINFO:   //F16������ר���û���������
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
        //  0,32,                   CON_DI_TYPE_INVALID);
            0,64,                   CON_DI_TYPE_INVALID);
        break;
//  case    CON_PARA_F17_KEEPPOWER: //F17���ն˱�����ֵ
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,2,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F18_POWCTRLTIMEPER:    //F18���ն˹���ʱ��
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,12,                   CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F19_POWCTRLCOEF:   //F19���ն�ʱ�ι��ض�ֵ����ϵ��
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,1,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F20_ENERCTRLCOEF:  //F20���ն��µ������ض�ֵ����ϵ��
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,1,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F21_PERTIMEFEE:    //F21���ն˵���������ʱ�κͷ�����
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,49,                   CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F22_FEEPARA:   //F22���ն˵���������
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,(unsigned char)(1+(4*ucNum)), CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F23_URGEALARMPARA: //F23���ն˴߷Ѹ澯����
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,3,                    CON_DI_TYPE_INVALID);
//      break;
    case    CON_PARA_F24_COPYINTER: //F24���ն˳����������
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,1,                    CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F25_MPBASEPARA:        //F25���������������
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,11,                   CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F26_MPLIMIT:           //F26����������ֵ����
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,57,                   CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F27_MPFREEZEPARA:      //F27�����������ݶ������
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
        //  0,(unsigned char)(1+(2*ucNum)),     CON_DI_TYPE_INVALID);
            0,24,                   CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F28_MPPOWRATEPARA:     //F28�������㹦�������ֶ���ֵ
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,4,                    CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F29_METERADDR:     //F28�������㹦�������ֶ���ֵ
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

//   case    CON_PARA_F33_SUMGFREEZEPARA:    //F33���ܼ������ݶ������
//       SetDIInfo(stRetDIInfo,
//           CON_DI_HAVE,
//           CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//           CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//           CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//           0,4,                    CON_DI_TYPE_INVALID);
//       break;
    case    CON_PARA_F33_SUMGFREEZEPARA:    //F33���ն˳������в�������

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
//  case    CON_PARA_F41_TIMEPERSETTING:    //F41��ʱ�ι��ض�ֵ
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
//  case    CON_PARA_F42_RESTCTRL:  //F42�����ݹ��ز���
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,6,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F43_POWERSLIP: //F43�����ʿ��ƵĹ��ʼ��㻬��ʱ��
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,1,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F44_SALESTOP:  //F44��Ӫҵ��ͣ�ز���
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,8,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F45_POWERCTRLTURN: //F45�������ִ��趨
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,1,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F46_MONTHENERCTRL: //F46���µ����ض�ֵ
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,4,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F47_BUYENERCTRL:   //F47���������ز���
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//      //  0,21,                   CON_DI_TYPE_INVALID);
//          0,17,                   CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F48_ENERCTRLTURN:  //F48������ִ��趨
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,1,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F49_CTRLALRTIME:   //F49�����ظ澯ʱ��
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,1,                    CON_DI_TYPE_INVALID);
//      break;
    case    CON_PARA_F57_SOUNDALARM:    //F57���ն������澯�����M��ֹ����
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,3,                    CON_DI_TYPE_INVALID);
        break;
//  case    CON_PARA_F58_KEEPAUTO:      //F58���ն��Զ��������
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,1,                    CON_DI_TYPE_INVALID);
//      break;
    case    CON_PARA_F59_MTRERRORPARA:  //F59�����ܱ��쳣�б���ֵ�趨
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,4,                    CON_DI_TYPE_INVALID);
        break;
//  case    CON_PARA_F60_HARMONPARA:    //F60��г����ֵ
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,80,                   CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F61_DCAINPARA: //F61��ֱ��ģ�����������
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,1,                    CON_DI_TYPE_INVALID);
//      break;

    case    CON_PARA_F62_COMMTYPE:      //F62 ����ר��������ʽ
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,5,                    CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F63_FLUXLIMIT:     //F63 ��ͨ��������������
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,4,                    CON_DI_TYPE_INVALID);
        break;

    case    CON_PARA_F65_UPDATA1PARA:   //F65����ʱ����1��������������
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,(unsigned char)(9+(4*ucNum2)),    CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F66_UPDATA2PARA:   //F66����ʱ����2��������������
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,(unsigned char)(9+(4*ucNum2)),    CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F67_UPDATA1CTRL:   //F67����ʱ����1��������������/ֹͣ����
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,1,                    CON_DI_TYPE_INVALID);
        break;
    case    CON_PARA_F68_UPDATA2CTRL:   //F68����ʱ����2��������������/ֹͣ����
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
            0,1,                    CON_DI_TYPE_INVALID);
        break;
//  case    CON_PARA_F73_CAPPARA:   //F73������������
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,48,                   CON_DI_TYPE_INVALID);       //changed by cfh 10-04-03
//      break;
//  case    CON_PARA_F74_CAPRUNPARA:    //F74��������Ͷ�����в���
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,10,                   CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F75_CAPRELAYPARA:  //F75����������������
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,16,                   CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F76_CAPCTRL:   //F76��������Ͷ�п��Ʒ�ʽ
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,1,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F81_DCARATE:   //F81��ֱ��ģ����������
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,4,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F82_DCALIMIT:  //F82��ֱ��ģ������ֵ
//      SetDIInfo(stRetDIInfo,
//          CON_DI_HAVE,
//          CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//          CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//          CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//          0,4,                    CON_DI_TYPE_INVALID);
//      break;
//  case    CON_PARA_F83_DCAFREESE: //F83��ֱ��ģ�����������
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
            0,0xff,                 CON_DI_TYPE_INVALID);//azh 0xff ��������ˣ�ʵ�ʳ�����256+256
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

    if(usDI < CON_PARA_BEG_CODE || usDI > CON_PARA_END_CODE)    //����
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

    if(usDI < CON_CONFIG_BEG_CODE || usDI > CON_CONFIG_END_CODE)    //����
        return ucRet;

    SetDIInfo(stRetDIInfo,
    CON_DI_HAVE,
    CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
    CON_DI_OPERATE_DISABLE, CON_DI_SETPURVIEW_ADMIN,
    CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
    CON_DI_LEN_CHANGED,0,   CON_DI_TYPE_INVALID);

    switch(usDI)
    {
    case    CON_CONFIG_F1_VERSION:      //F1 �ն˰汾��Ϣ
            stRetDIInfo->ucContentLen = 41;
            break;
    case    CON_CONFIG_F2_INTERFACE:    //F2 �ն����롢�����ͨ�Ŷ˿�����
            stRetDIInfo->ucContentLen = 41;
            break;
    case    CON_CONFIG_F3_OTHER:        //F3 �ն���������
            stRetDIInfo->ucContentLen = 17;
            break;
    case    CON_CONFIG_F4_PARA:         //F4 �ն˲������ã����ն���Ӳ���汾֧�ֵĲ���F1~F68��
            stRetDIInfo->ucContentLen = 10;
            break;
    case    CON_CONFIG_F5_CTRL:         //F5 �ն˿������ã����ն���Ӳ���汾֧�ֵĿ�������F1-F38��
            stRetDIInfo->ucContentLen = 6;
            break;
    case    CON_CONFIG_F6_DATA1:        //F6 �ն�1���������ã����ն���Ӳ���汾֧�ֵ�1������F1-F121��
            stRetDIInfo->ucContentLen = 20;
            break;
    case    CON_CONFIG_F7_DATA2:        //F7 �ն�2���������ã����ն���Ӳ���汾֧�ֵ�2������F1=F108��
            stRetDIInfo->ucContentLen = 26;
            break;
    case    CON_CONFIG_F8_ERC:          //F8 �ն��¼���¼���ã����ն���Ӳ���汾֧�ֵ��¼���¼ERC1-ERC33��
            stRetDIInfo->ucContentLen = 6;
            break;
//azh 170920
    case    CON_CONFIG_F9_REMOTEVER:          //F9Զ��ͨ��ģ��汾��Ϣ
            stRetDIInfo->ucContentLen = 46;
            break;
    case    CON_CONFIG_F10_LOCALVER:          //F10����ͨ��ģ��汾��Ϣ
            stRetDIInfo->ucContentLen = 15;
            break;
    }

    return ucRet;
}


unsigned char  Main_DI_Login_Query_Manage(STDIINFO *stRetDIInfo,unsigned short usDI,unsigned char ucNum4)
{
    unsigned char   ucRet=RET_SUCCESS;
    ClearDIInfo( stRetDIInfo );

    if(usDI < CON_LOGIN_BEG_CODE || usDI > CON_LOGIN_END_CODE)  //����
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

    if(usDI < CON_TASKDATA_REQ_BEG_CODE || usDI > CON_TASKDATA_REQ_END_CODE)    //����
        return ucRet;

    SetDIInfo(stRetDIInfo,
    CON_DI_HAVE,
    CON_DI_TYPE_DATA,       CON_DI_MPTYPE_NORMAL,
    CON_DI_OPERATE_DISABLE, CON_DI_SETPURVIEW_ADMIN,
    CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
    CON_DI_LEN_CHANGED,0,   CON_DI_TYPE_INVALID);

    switch(usDI)
    {
    case    CON_TASKDATA_REQ_F1:        //F1 1������
            //stRetDIInfo->ucContentLen = 0;
            stRetDIInfo->ucCommandLen = 0;
            break;
    case    CON_TASKDATA_REQ_F2:        //F2 2������
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

    if(usDI < CON_CTRL_BEG_CODE || usDI > CON_CTRL_END_CODE)    //����
        return ucRet;

    switch(usDI)
    {
//    case    CON_CTRL_F1_YK_OPEN:        //F1 ң����բ
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,1,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_CTRL_F2_YK_CLOSE:       //F2 ң��������բ
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
//
//    case CON_CTRL_F9_TIMEPER_ON:        //F9ʱ�ι���Ͷ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,2,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F10_RESTCTRL_ON:      //F10���ݹ���Ͷ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F11_SALESTOP_ON:      //F11Ӫҵ��ͣ����Ͷ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F12_POWERLOWER_ON:    //F12��ǰ�����¸���Ͷ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,8,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F15_MONTHENER_ON:     //F15�µ��Ͷ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F16_BUYENER_ON:       //F16�����Ͷ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F17_TIMEPER_OFF:      //F17ʱ�ι��ؽ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F18_RESTCTRL_OFF:     //F18���ݹ��ؽ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F19_SALESTOP_OFF:     //F19Ӫҵ��ͣ���ؽ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F20_POWERLOWER_OFF:   //F20��ǰ�����¸��ؽ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F23_MONTHENER_OFF:    //F23�µ�ؽ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F24_BUYENER_OFF:      //F24����ؽ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F25_KEEPPOWER_ON:     //F25�ն˱���Ͷ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,1,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F26_URGEALARM_ON:     //F26�߷Ѹ澯Ͷ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
    case CON_CTRL_F27_TALK_ON:          //F27�����ն�����վͨ��
            SetDIInfo(stRetDIInfo,
                CON_DI_HAVE,
                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
                0,0,                    CON_DI_TYPE_INVALID);
            break;
//    case CON_CTRL_F28_REMOVE_ON:        //F28�ն��޳�Ͷ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
    case CON_CTRL_F29_UPACTIVE_ON:      //F29�����ն������ϱ�
            SetDIInfo(stRetDIInfo,
                CON_DI_HAVE,
                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
                0,0,                    CON_DI_TYPE_INVALID);
            break;
//azh 170920
    case CON_CTRL_F30_SETMETERTIME_SWITCH:          //F30�ն˶Ե��ܱ���ʱ���ܵĿ�����ر�
            SetDIInfo(stRetDIInfo,
                CON_DI_HAVE,
                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
                0,1,                    CON_DI_TYPE_INVALID);
            break;
    case CON_CTRL_F31_SETTIME:          //F31��ʱ����
            SetDIInfo(stRetDIInfo,
                CON_DI_HAVE,
                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
                0,6,                    CON_DI_TYPE_INVALID);
            break;
    case CON_CTRL_F32_MESSAGE:          //F32������Ϣ
            SetDIInfo(stRetDIInfo,
                CON_DI_HAVE,
                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
                0,(unsigned char)(2+ucNum4),    CON_DI_TYPE_INVALID);
            break;
//    case CON_CTRL_F33_KEEPPOWER_OFF:    //F33�ն˱�����
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F34_URGEFEE_OFF:      //F34�߷Ѹ澯���
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
    case CON_CTRL_F35_TALK_OFF:         //F35��ֹ�ն�����վͨ��
            SetDIInfo(stRetDIInfo,
                CON_DI_HAVE,
                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
                0,0,                    CON_DI_TYPE_INVALID);
            break;
//    case CON_CTRL_F36_REMOVE_OFF:       //F36�ն��޳����
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,0,                    CON_DI_TYPE_INVALID);
//            break;
    case CON_CTRL_F37_UPACTIVE_OFF:     //F37��ֹ�ն������ϱ�
    case CON_CTRL_F38_SMS_ACTIVE:       //F38���ż���
    case CON_CTRL_F39_DISCONECT:
            SetDIInfo(stRetDIInfo,
                CON_DI_HAVE,
                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
                0,0,                    CON_DI_TYPE_INVALID);
            break;
//    case CON_CTRL_F41_CAPCTRL_ON:       //F41���ݿ���Ͷ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,2,                    CON_DI_TYPE_INVALID);
//            break;
//    case CON_CTRL_F42_CAPCTRL_OFF:      //F42���ݿ��ƽ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,2,                    CON_DI_TYPE_INVALID);
//            break;
/*  case  CON_CTRL_F88_TIMEREVISE:      //F88��ȷ��ʱ����
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
    case CON_CTRL_F149_METERSELF_SWITCH:          //F149���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
            SetDIInfo(stRetDIInfo,
                CON_DI_HAVE,
                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
                0,1,                    CON_DI_TYPE_INVALID);
            break;
    case CON_CTRL_F150_RESETSNSTATE_SWITCH:          //F150��λ��������������־
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

    if(usDI < CON_EXTCMD_BEG_CODE || usDI > CON_EXTCMD_END_CODE)    //����
        return ucRet;

    switch(usDI)
    {
//    case    CON_EXTCMD_F1:          //F1 �ڴ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,12,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F2:          //F2 �ڴ�д
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,28,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F3:          //F3 DATAFLASH��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,12,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F4:          //F4 DATAFLASHд
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,28,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F5:          //F5 ���Ź����ԣ��ó��������ѭ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,8,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F6:          //F6 �ֻ�ģ��ͨѶ���
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F7:          //F7 �ն�״̬���
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
    case    CON_EXTCMD_F10:         //F10 �ն˵�ַ10/16�����л�
            SetDIInfo(stRetDIInfo,
                CON_DI_HAVE,
                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
                0,6,                    CON_DI_TYPE_INVALID);
            break;
//    case    CON_EXTCMD_F11:         //F11 ������Ϣ��ȡ
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,5,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F21:     //F21 GR�����
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F22:     //F22 GR��������
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,(unsigned char)(32+2*ucNum2),     CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F23:     //F23 GR������Ϣ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F24:     //F24 GR������Ϣ����
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F31:     //F31 ������������
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F33:     //F33 ����׶���������ȫ
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,20,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F40:     //F40 �����ڱ�����
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,CON_DI_LEN_CHANGED,   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F41:     //F41 ��ȡ�ڱ�����
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,6,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F51:     //F51 �ն˵�ַ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F52:     //F52 �ն���Ŷ�
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F53:     //F53 �ն˵�ַ��������д
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,8,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F54:     //F54 �ն����д
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,12,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F71:     //F71 ����������
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,13,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F72:     //F72 �޸�ʱ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,6,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F73:     //F73 ��ѯͨѶ������
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,5,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F74:     //F74 �޸�ͨѶ������
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,12,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F75:     //F75 ��ȡҺ��������ʾ����
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F76:     //F76 ��ȡҺ��������ʾ����
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,5,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F77:     //F77 ��ȡҺ��������ʾ����
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F78:     //F78 �޸�Һ��������ʾ����
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,38,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F79:     //F79 �޸�Һ��������ʾ����
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,39,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F80:     //F80 �޸�Һ��������ʾ����
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,8,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F81:     //F81 ����SIM����pin
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,6,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F82:     //F82 �޸�SIM����pin
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,8,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F83:     //F83 ��PUK����SIM����pin
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,8,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F84:     //F84 SIM����pin���״̬
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F85:     //F85 GSM/GPRS�л�
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,5,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F86:     //F86 �����ʿ��ƻָ�ʱ�䳤��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F87:     //F87 д���ʿ��ƻָ�ʱ�䳤��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,5,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F89:     //F89 �����ʿ���Խ��ȷ��ʱ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F90:     //F90 д���ʿ���Խ��ȷ��ʱ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,5,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F91:     //F91 ���Զ����������
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F92:     //F92 д�Զ����������
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,16,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F93:     //F93 ��ȡ�ն˵�ѹ��λС��������λС��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F94:     //F94 �����ʼ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,13,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F95:     //F95 8025ʱ�ӽ���
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,8,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F96:     //F96 Һ���Աȶ�����
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,5,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F97:     //F97�����ñ���ͨѸ��������
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,20,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F98:     //F98����ȡ����ͨѸ��������
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F99:     //F99 ����GPRS�����PDP�û���������
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,37,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F100:        //F100 ��ȡGPRS�����PDP�û���������
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F101:        //F101 ����GPRS�������ʱ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,6,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F102:        //F102 ��ȡGPRS�������ʱ��
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F109:        //F109 �����ն˹���ģʽ
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,5,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F110:        //F110 ��ȡ�ն˹���ģʽ
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F111:        //F111 ����CT����
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,5,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F112:        //F112 ��ȡCT����
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//
//    case    CON_EXTCMD_F113:        //F113 ������ѧϰ��Ч��־
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,5,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F114:        //F114 ��ȡ��ѧϰ��Ч��־
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F115:        //F115 ���ö��β࿪·��ֵ
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,10,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F116:        //F116 ��ȡ���β࿪·��ֵ
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F117:        //F117 ����һ�β��·��ֵ
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,10,                   CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F118:        //F118 ��ȡһ�β��·��ֵ
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F119:        //F119 ��ȡ�迹ֵ
//            SetDIInfo(stRetDIInfo,
//                CON_DI_HAVE,
//                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//                0,4,                    CON_DI_TYPE_INVALID);
//            break;
//    case    CON_EXTCMD_F120:            //F120 ����AES����
//        SetDIInfo(stRetDIInfo,
//            CON_DI_HAVE,
//            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//            0,37,                   CON_DI_TYPE_INVALID);
//        break;
//    case    CON_EXTCMD_F121:            //F120 ��ȡAES����
//        SetDIInfo(stRetDIInfo,
//            CON_DI_HAVE,
//            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//            0,4,                    CON_DI_TYPE_INVALID);
//    case    CON_EXTCMD_F122:            //F122���ý��������߱���
//        SetDIInfo(stRetDIInfo,
//            CON_DI_HAVE,
//            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//            0,5,                    CON_DI_TYPE_INVALID);
//        break;
//    case    CON_EXTCMD_F123:            //F123��ȡ�¶�
//        SetDIInfo(stRetDIInfo,
//            CON_DI_HAVE,
//            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//            0,4,                    CON_DI_TYPE_INVALID);
//        break;
//    case    CON_EXTCMD_F124:            //F124 ��ȡУ��EEPROM�Ĳ���
//        SetDIInfo(stRetDIInfo,
//            CON_DI_HAVE,
//            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
//            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
//            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
//            0,5,                    CON_DI_TYPE_INVALID);
//        break;
//    case    CON_EXTCMD_F125:            //F125 ����У��EEPROM�Ĳ���
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
    case    CON_DATATRANS_F1:           //F1 ����ת��
        ucNum = pucBuffer[4];
        SetDIInfo(stRetDIInfo,
            CON_DI_HAVE,
            CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
            CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
            CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
        //  0,(unsigned char)(5+ucNum), CON_DI_TYPE_INVALID);
            0,(unsigned char)(6+ucNum), CON_DI_TYPE_INVALID);       //ת���ĳ���Ϊ2���ֽڵģ���Ҫ��.cfh 10-03-31
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
    case    CON_FILETRANS_F1:           //F1 �ļ����䷽ʽ1
            SetDIInfo(stRetDIInfo,
                CON_DI_HAVE,
                CON_DI_TYPE_PARA,       CON_DI_MPTYPE_TERSELF,
                CON_DI_OPERATE_ENABLE,  CON_DI_SETPURVIEW_ADMIN,
                CON_DI_OPERATE_ENABLE,  CON_DI_OPERATE_DISABLE,
                0,0,                    CON_DI_TYPE_INVALID);
            break;
    case    CON_FILETRANS_F100:         //F100 �Զ��������װ
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
//{//���λ��1�ĸ���
//  unsigned char ucNum=0,i=0;
//  for(i=0;i<8;i++)
//  {
//      if((ucByte&0x01)==0x01)
//          ucNum++;
//      ucByte=ucByte>>1;
//  }
//  return ucNum;
//}
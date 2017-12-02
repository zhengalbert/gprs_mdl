/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : ProtocolParse.h
 Description    : ��վЭ�����
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1��    1.00        2012-02-05    azh     the original version
******************************************************************************/
#ifndef _PROTOCOLPARSE_H_
#define _PROTOCOLPARSE_H_


/************************************************************************/
/* �豸����                                                             */
/************************************************************************/
#define UP_COM_NUM   5//(5)      //����ͨ������

#define  CON_NWUP_TN_LENTH                      1//�����ն˲����㳤��Ϊ1�ֽ� ����2�ֽڴ�ȷ��

#define TRG_OK          (0)     //�ɹ�
#define TRG_WAIT        (1)     //�ȴ�
#define TRG_CHUNLOGO    (2)     //ͨ��δ��¼
#define TRG_CHFAIL      (3)     //ͨѶ�豸����
#define TRG_CMDFAIL     (4)     //�������
#define TRG_END         (5)     //������ֹ

#define M_NORMAL_FRM    (0)     //��վ��������֡
#define M_RSP_FRM       (1)     //��վ����Ӧ��֡
#define M_UNRSP_FRM     (2)     //��վ�쳣Ӧ��֡
#define S_RSP_FRM       (3)     //ģ������Ӧ���ϴ�֡
#define S_UNRSP_FRM     (4)     //ģ���쳣Ӧ���ϴ�֡
#define M_LINE_FRM      (5)     //����ģ��֡
#define S_AUTO_FRM      (6)     //ģ�������ϴ�֡
#define MAX_CNTN_FRM    (6)     //˳��ö������

//azh
//#define CON_TERMSG_TYPE_HEARTCHK    0x06        //�������
//#define CON_TERMSG_TYPE_LOGIN       0x04        //��¼
//#define CON_TERMSG_TYPE_LOGOUT      0x05        //��¼�˳�
//����ģ����Ϣģ��


//�������
#define ECD_OK       (0x00)     //��ȷ���޴���
#define ECD_RNORTN   (0x01)     //�м�����û�з���
#define ECD_SETERR   (0x02)     //�������ݷǷ�
#define ECD_PWERR    (0x03)     //����Ȩ�޲���
#define ECD_NODI     (0x04)     //�޴�������
#define ECD_COMTOV   (0x05)     //����ʱ��ʧЧ
#define ECD_NOOBJ    (0x11)     //Ŀ���ַ������
#define ECD_SNDFAIL  (0x12)     //����ʧ��
#define ECD_FRMOL    (0x13)     //����Ϣ̫֡��
#define ECD_CREATERR (0x14)     //��֡ʧ��
#define ECD_TRANSMIT (0x15)     //͸��ת��

#define ECD_OBJFRM   (0xFE)     //Ŀ��֡
#define ECD_NOFRM    (0xFF)     //����Э��֡


//�ն˺Ͳ�����������ݱ�ʶ����
#define L_V_NUM      (3)    //��
#define L_H_P_NUM    (18)//(49) //��
//���������ݱ�ʶ����
#define L_H_T_NUM    (41) //��


//__EXTERN int8u RdTopFSEQ( void );
__EXTERN int16u SearchDI( int16u uiDI, int8u DIType );
__EXTERN  S8 deal_local_GWframe(U8 *InBuf);
__EXTERN  U8 LocalCom_UpSendData(unsigned char *pBuf,unsigned short usLen,unsigned char ucMsgSource);
__EXTERN  U8 vport_to_com_convert(U8 vport);
#endif//_PROTOCOLPARSE_H_
/******************************************************************************
    End Of File
******************************************************************************/


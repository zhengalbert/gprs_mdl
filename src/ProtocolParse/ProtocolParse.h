/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : ProtocolParse.h
 Description    : 主站协议解析
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/
#ifndef _PROTOCOLPARSE_H_
#define _PROTOCOLPARSE_H_


/************************************************************************/
/* 设备常量                                                             */
/************************************************************************/
#define UP_COM_NUM   5//(5)      //上行通道数量

#define  CON_NWUP_TN_LENTH                      1//南网终端测量点长度为1字节 集抄2字节待确定

#define TRG_OK          (0)     //成功
#define TRG_WAIT        (1)     //等待
#define TRG_CHUNLOGO    (2)     //通信未登录
#define TRG_CHFAIL      (3)     //通讯设备故障
#define TRG_CMDFAIL     (4)     //命令错误；
#define TRG_END         (5)     //触发终止

#define M_NORMAL_FRM    (0)     //主站正常命令帧
#define M_RSP_FRM       (1)     //主站正常应答帧
#define M_UNRSP_FRM     (2)     //主站异常应答帧
#define S_RSP_FRM       (3)     //模块正常应答上传帧
#define S_UNRSP_FRM     (4)     //模块异常应答上传帧
#define M_LINE_FRM      (5)     //级联模块帧
#define S_AUTO_FRM      (6)     //模块主动上传帧
#define MAX_CNTN_FRM    (6)     //顺序枚举最大号

//azh
//#define CON_TERMSG_TYPE_HEARTCHK    0x06        //心跳检测
//#define CON_TERMSG_TYPE_LOGIN       0x04        //登录
//#define CON_TERMSG_TYPE_LOGOUT      0x05        //登录退出
//触发模块信息模块


//错误编码
#define ECD_OK       (0x00)     //正确，无错误
#define ECD_RNORTN   (0x01)     //中继命令没有返回
#define ECD_SETERR   (0x02)     //设置内容非法
#define ECD_PWERR    (0x03)     //密码权限不足
#define ECD_NODI     (0x04)     //无此项数据
#define ECD_COMTOV   (0x05)     //命令时间失效
#define ECD_NOOBJ    (0x11)     //目标地址不存在
#define ECD_SNDFAIL  (0x12)     //发送失败
#define ECD_FRMOL    (0x13)     //短消息帧太长
#define ECD_CREATERR (0x14)     //组帧失败
#define ECD_TRANSMIT (0x15)     //透传转发

#define ECD_OBJFRM   (0xFE)     //目标帧
#define ECD_NOFRM    (0xFF)     //不是协议帧


//终端和测量点参数数据标识集合
#define L_V_NUM      (3)    //列
#define L_H_P_NUM    (18)//(49) //行
//测量点数据标识集合
#define L_H_T_NUM    (41) //行


//__EXTERN int8u RdTopFSEQ( void );
__EXTERN int16u SearchDI( int16u uiDI, int8u DIType );
__EXTERN  S8 deal_local_GWframe(U8 *InBuf);
__EXTERN  U8 LocalCom_UpSendData(unsigned char *pBuf,unsigned short usLen,unsigned char ucMsgSource);
__EXTERN  U8 vport_to_com_convert(U8 vport);
#endif//_PROTOCOLPARSE_H_
/******************************************************************************
    End Of File
******************************************************************************/


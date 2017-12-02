/******************************************************************************
 Copyright (C) 2011  R&D Institute of Reallin Co., Ltd.
 Module         : far infrared mamage function
 File Name      : far_infrared.h
 Description    : LPC17系列芯片硬件IO口模拟串口远红外通信
 Author         :
 Build Date     :
 others         :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、     1.00       2011-07-30     rosoon     the original version
******************************************************************************/

#ifndef __FAR_INFRARED_H
#define __FAR_INFRARED_H

//----------------------------- macro definition ------------------------------
#define FAR_INFRARED_BAUD               1200        //红外口波特率
#define FAR_INFRARED_CARRIER            38000       //红外口发射频频率
#define FAR_INFRARED_BIT_WIDTH          1000000 / FAR_INFRARED_BAUD //远红外通信位宽
#define FAR_INFRARED_RX_BYTE_OVERTIME   100 * T1_MAT_FAR_INFRARED   //字节接收超时匹配值(100位速率)
#define FAR_INFRARED_TX_BYTE_TIMING     50 * T1_MAT_FAR_INFRARED    //字节接收超时匹配值(50位速率)
#define FAR_INFRARED_PARA_ADDR          LAST_EEPROM_ADDR            //外存保存远红外通信参数的地址
//#define
//#define
//#define
//#define
//#define

//----------------------------- type definition -------------------------------
typedef enum
{
    IR_RX,              //0,接收通道
    IR_TX,              //1,发送通道
    IR_MX = 256,        //dat_buf[]容量
    IR_FI = 0,          //dat_buf首字节(FIRST)dat_buf[0]
    IR_LS = IR_MX - 1   //dat_buf尾字节(LAST)dat_buf[255]
}IR_INDEX;
typedef struct
{
    U8 Bit_Cnt;         //数据位计数器
    U8 Bit_Sum;         //数据位偶校验和
    U8 Dat_Len;         //数据字节计数器[数据帧长信息]
    U8 Dat_Tok;         //数据帧令牌信息
    U8 Dat_Buf[IR_MX];  //数据缓冲区
    U16 Dat_Tmp;        //临时数据
    U32 Mat_Val;        //匹配值
}FAR_INFRARED_STRUCT;
//-------------------------- functions declaration ----------------------------
__EXTERN void far_infrared_init(void);

#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不是配合带CPU_GPRS模块
__EXTERN void far_infrared_rx_drv(void);
__EXTERN void far_infrared_rx_timeout(void);
__EXTERN void far_infrared_tx_drv(void);
__EXTERN void far_infrared_tx_timing(void);
__EXTERN void send_far_infrared_data(U8 *buf, U8 cnt);
__EXTERN S8 get_far_infrared_data(U8 *buf);

//--------------------------- variable declaration ----------------------------
__EXTERN FAR_INFRARED_STRUCT Ir_Ctl[2];         //远红外通信结构体
__EXTERN U8 Ir_Rxbyte_Timeout_Cnt[2];           //字节接收超时计数器
__EXTERN U8  Ir_Occupy_State;                   //红外端口占用状态:IR_RX->接收任务占用；IR_TX->发送任务占用；

#endif
//
#endif//__FAR_INFRARED_H

/******************************************************************************
    End Of File
******************************************************************************/

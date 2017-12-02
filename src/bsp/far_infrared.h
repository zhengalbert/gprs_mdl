/******************************************************************************
 Copyright (C) 2011  R&D Institute of Reallin Co., Ltd.
 Module         : far infrared mamage function
 File Name      : far_infrared.h
 Description    : LPC17ϵ��оƬӲ��IO��ģ�⴮��Զ����ͨ��
 Author         :
 Build Date     :
 others         :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��     1.00       2011-07-30     rosoon     the original version
******************************************************************************/

#ifndef __FAR_INFRARED_H
#define __FAR_INFRARED_H

//----------------------------- macro definition ------------------------------
#define FAR_INFRARED_BAUD               1200        //����ڲ�����
#define FAR_INFRARED_CARRIER            38000       //����ڷ���ƵƵ��
#define FAR_INFRARED_BIT_WIDTH          1000000 / FAR_INFRARED_BAUD //Զ����ͨ��λ��
#define FAR_INFRARED_RX_BYTE_OVERTIME   100 * T1_MAT_FAR_INFRARED   //�ֽڽ��ճ�ʱƥ��ֵ(100λ����)
#define FAR_INFRARED_TX_BYTE_TIMING     50 * T1_MAT_FAR_INFRARED    //�ֽڽ��ճ�ʱƥ��ֵ(50λ����)
#define FAR_INFRARED_PARA_ADDR          LAST_EEPROM_ADDR            //��汣��Զ����ͨ�Ų����ĵ�ַ
//#define
//#define
//#define
//#define
//#define

//----------------------------- type definition -------------------------------
typedef enum
{
    IR_RX,              //0,����ͨ��
    IR_TX,              //1,����ͨ��
    IR_MX = 256,        //dat_buf[]����
    IR_FI = 0,          //dat_buf���ֽ�(FIRST)dat_buf[0]
    IR_LS = IR_MX - 1   //dat_bufβ�ֽ�(LAST)dat_buf[255]
}IR_INDEX;
typedef struct
{
    U8 Bit_Cnt;         //����λ������
    U8 Bit_Sum;         //����λżУ���
    U8 Dat_Len;         //�����ֽڼ�����[����֡����Ϣ]
    U8 Dat_Tok;         //����֡������Ϣ
    U8 Dat_Buf[IR_MX];  //���ݻ�����
    U16 Dat_Tmp;        //��ʱ����
    U32 Mat_Val;        //ƥ��ֵ
}FAR_INFRARED_STRUCT;
//-------------------------- functions declaration ----------------------------
__EXTERN void far_infrared_init(void);

#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//������ϴ�CPU_GPRSģ��
__EXTERN void far_infrared_rx_drv(void);
__EXTERN void far_infrared_rx_timeout(void);
__EXTERN void far_infrared_tx_drv(void);
__EXTERN void far_infrared_tx_timing(void);
__EXTERN void send_far_infrared_data(U8 *buf, U8 cnt);
__EXTERN S8 get_far_infrared_data(U8 *buf);

//--------------------------- variable declaration ----------------------------
__EXTERN FAR_INFRARED_STRUCT Ir_Ctl[2];         //Զ����ͨ�Žṹ��
__EXTERN U8 Ir_Rxbyte_Timeout_Cnt[2];           //�ֽڽ��ճ�ʱ������
__EXTERN U8  Ir_Occupy_State;                   //����˿�ռ��״̬:IR_RX->��������ռ�ã�IR_TX->��������ռ�ã�

#endif
//
#endif//__FAR_INFRARED_H

/******************************************************************************
    End Of File
******************************************************************************/

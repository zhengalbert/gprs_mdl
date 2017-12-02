/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : pmu.h
 Description    : LPC17ϵ��оƬӲ����Դ�����ӳ����ͷ�ļ�
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��    2.00        2011-02-25     rosoon
   1��    1.00        2010-09-14     rosoon     the original version
******************************************************************************/

#ifndef __PMU_H
#define __PMU_H

//----------------------------- macro definition ------------------------------
#define __WFI               __wfi                       //˯�ߣ��ȴ��ж�ָ��

#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//��ϴ�CPU_GPRSģ��
#define CLRWDT              SET_PORT0_OUT(P0_WDI);\
                            SET_PORT0_REV(P0_WDI)       //���Ӳ�����Ź���ָ��
#else
#define CLRWDT              SET_PORT1_OUT(P1_WDI);\
                            SET_PORT1_REV(P1_WDI)       //���Ӳ�����Ź���ָ��
#endif                            
#define PWR_ON              0                   //ϵͳ�ϵ��ʶ
#define PWR_DN              (1 << 0)                    //ϵͳ�����ʶ

#define BAT_ON_MXTIME       1800    //unit=100ms ����ʱ�������๤��3����
#define BAT_LOW_GATE_VOLT   2980    //0xBA4:2.4V[2^12*(2.4/3.3)](��Ӧʵ��ֵ4.8V) �������͹������޵�ѹ

//----------------------------- type definition -------------------------------
typedef struct _B_PD_CTRL
{
    U8  last_state;     //�ϴ�״̬
    U8  cmd_sended;       //���ݱ����ϱ�����
    U16  send_time;      //���ݷ����󵹼�ʱ60�� ��ģ���Դ
    U16 dncount;        //����ع�������ʱ

}B_PD_CTRL;
//-------------------------- functions declaration ----------------------------
__EXTERN void check_power_on(U16 cnt);
__EXTERN void check_power_down (void);
__EXTERN void before_pd_pwr_manage(void);
//--------------------------- variable declaration ----------------------------
__EXTERN B_PD_CTRL gstB_Pd_Ctrl;

#endif//__PMU_H

/******************************************************************************
    End Of File
******************************************************************************/

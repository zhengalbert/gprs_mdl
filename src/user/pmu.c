/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : pmu.c
 Description    : LPC11ϵ��оƬ��Դ�����ӳ���
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��    2.00        2011-02-25     rosoon
   1��    1.00        2010-09-14    rosoon     the original version
******************************************************************************/

#include "..\inc\global_config.h"

//-------------------------------- �������� -----------------------------------

//------------------------------ ˽�б������� ---------------------------------

//------------------------------ ˽�к������� ---------------------------------


/******************************************************************************
** ��������: void before_pd_manage (void)
** ��������: �յ���� ��Դ����(���ĸ���ع��� ���೤ʱ��), �����ϵ�ʱ Ҳ��ͨ�����Ź��ⲿ��λ
** ��ڲ���:
**
** �� �� ֵ: ��
** ˵    ��: ����������
******************************************************************************/
void before_pd_pwr_manage (void)
{
#ifndef PD_MANAGE_VER12
    U8 volt_type;   //Ϊ�˼���2.4V��4.8V�������͵�صĲ�ͬ�ж�����
#endif
    if(Power_Down_Flag == PWR_DN)//�ⲿû��
    {
        if(gstB_Pd_Ctrl.last_state == PWR_ON)//��ͣ��
        {
            gstB_Pd_Ctrl.dncount = BAT_ON_MXTIME;//������������ʱ��
            CHARGE_DIS();                       //������
            FAST_CHARGE_OFF();                  //���ر�
            if(stModuleRunState.bIsGPRSConnected == 1)//������ߵĻ� ��д�����ϱ��澯
            {
//azh
//                up_send_pd_cmd();//��д����澯����
                gstB_Pd_Ctrl.send_time = 600; //30��
            }
            else//������
            {
                PIN_POWERON_CLR;//�ڵ���ʱ����������ر�GPRSģ���Դ
                gstB_Pd_Ctrl.send_time =600;//60��
            }
            gstB_Pd_Ctrl.cmd_sended = 1;
            Alarm_Save_Last_TerTime();//�յ��籣��һ��
//
            powdn_save_day_month_data(CON_DAYDATA_TYPE);//900���ֽ�
            powdn_save_day_month_data(CON_MONTHDATA_TYPE);
        }
#ifndef PD_MANAGE_VER12
        volt_type = (U8)(0x01 & (Bat_Stat_Flag >> RATED_VOLT_FLG_BIT)); //��ȡ���������Ϣ,�Ա���������Ӧ����ѡ��
        if(((Ad_Data[BAT_GPRS] <= (BAT_LOW_GATE_VOLT >> volt_type))&&(Ad_Data[BAT_GPRS] != 0)) || (gstB_Pd_Ctrl.dncount == 0))//<=4.8V or �����ع���ʱ��
        {
            disable_irq();              // �ر������ж�
            BAT_OFF();//����ع���ر�
            while(1);//debug
        }
#endif

        if((gstB_Pd_Ctrl.cmd_sended == 1) && (gstB_Pd_Ctrl.send_time == 0))
        {
            disable_irq();              // �ر������ж�
            BAT_OFF();//����ع���ر�
            while(1);//debug
        }
    }
    else//�ⲿ�е�
    {
        if(gstB_Pd_Ctrl.last_state == PWR_DN)//������
        {
            disable_irq();              // �ر������ж�
            BAT_OFF();//����ع���ر�
            while(1);//debug
        }
    }

    gstB_Pd_Ctrl.last_state = Power_Down_Flag;  //
    if(gstB_Pd_Ctrl.dncount > 0)
    {
        gstB_Pd_Ctrl.dncount--;
    }
    if(gstB_Pd_Ctrl.send_time > 0)
    {
        gstB_Pd_Ctrl.send_time--;
    }
}
/******************************************************************************
** ��������: void check_power_on(U16 cnt)
** ��������: ϵͳ�ϵ�:��⹦�ܺ���
** ��ڲ���: cnt �趨�ϵ������Ų������ߵ�ƽ�Ĵ���,ȡֵ1~65535
**
** �� �� ֵ: ��
** ˵    ��: ��
******************************************************************************/
void check_power_on (U16 cnt)
{
    U8  power_ok_cnt;

    power_ok_cnt = 0;

    SET_PORT0_IN(P0_PD_CHK);                    //����p0.29Ϊ�����

    while(1)
    {
        if(PORT0_IS_HIGH(P0_PD_CHK))        //�������ź�����
        {
            if(++power_ok_cnt > cnt)        //����cnt(=200)�μ�⵽�����ź�Ϊ������Ϊ��Դ�Ѿ��ȶ�
            {
                return;
            }
        }
        else
        {
            while(1);
        }
        CLRWDT;                         //���Ӳ�����Ź�
        delay(50);                          //��ʱԼ1.2ms����ʱ�Ĵ�����ʱ����4MHz
    }
}
/******************************************************************************
Function name: void check_power_down (void)
Author       : snap.gao
Description  : �����飬��������ʱ��ÿ����������Ϣ
Input        : None
Return       : None
******************************************************************************/
void check_power_down (void)
{
    static U8 power_fail_cnt = 0;

    SET_PORT0_IN(P0_PD_CHK);                    //����p0.29Ϊ�����

    while(1)
    {
        if(PORT0_IS_HIGH(P0_PD_CHK) == 0)   //�������ź�����
        {
            if(++power_fail_cnt > 2)        //����3�μ�⵽�����ź����ж�����
            {
    //            Power_Down_Flag = POWER_DOWN_FLAG;
    //            os_evt_set(POWER_DOWN_FLAG, System_Manage_Task_ID);
    //            os_evt_set(POWER_DOWN_FLAG, Protocol_Status_Task_ID);
    //            os_evt_set(POWER_DOWN_FLAG, SNTP_Task_ID);
    //            os_evt_set(POWER_DOWN_FLAG, Crawl_Task_ID);
    //            while(os_mbx_send(Mbx_Load_Profiler, NULL, 0) == OS_R_TMO)
    //            {
    //                os_dly_wait(2);
    //            }
    //            while(os_mbx_send(Mbx_Comm_Data, NULL, 0) == OS_R_TMO)
    //            {
    //                os_dly_wait(2);
    //            }
                //azh 110624 �ݲ�����
                //pd_main();//���������ѭ��
                //
                Power_Down_Flag = PWR_DN;
                BAT_ON();//����ع���ر�
#if CODE_VER_TYPE!=GPRS_MODULE_PRJ		//��CPU_GPRSģ��
                PVDD_OFF();  //Zigbee���������Դ
#endif							
                power_fail_cnt = 0;
                break;

//              RTCGPREG0 = COP_PD_1ST_DIS;         //����� ��λ�������ģʽ�� ����һ���־
//              disable_irq();              // �ر������ж�
//              while(1);//debug
            }
        }
        else
        {
            Power_Down_Flag = PWR_ON;
            power_fail_cnt = 0;
//          BAT_OFF();//����ع���ر�
            break;
        }
    }
}

/******************************************************************************
    End Of File
******************************************************************************/

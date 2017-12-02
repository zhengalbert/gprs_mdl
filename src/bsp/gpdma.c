/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : gpdma.c
 Description    : LPC17ϵ��оƬGPDMA�ӳ���
-------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��	  1.01		  2011-07-17    azh             zigbee DMA send query
   1��    1.00        2011-03-03    wwllzz          the original version
******************************************************************************/
#include "..\inc\global_config.h"



/******************************************************************************
����: void Zigbee_DMA_Init(void)
���: ��
����: ��
����: ��ʼ��GPDMA, �ܹ���8ͨ��, 16��
******************************************************************************/
#ifdef ZIGBEE_TEST
void Zigbee_DMA_Init(void)
{
    DMACIntErrClr = DMACIntTCClear = 0x08;
    
    DMACC3DestAddr = (U32)(&U1THR);                     //UART1.Tx
    DMACC3LLI = 0;
    DMACC3Control = 0;
    DMACC3Config &= ~0x01;
}
#endif
/******************************************************************************
����: void GPDMA_Init(void)
���: ��
����: ��
����: ��ʼ��GPDMA, �ܹ���8ͨ��, 16��
SSP0.Tx -> 0[0],  SSP0.Rx-> 1[1],  MAT2.0 -> 12[2]
1), IrDA����FE, ���ð��ս������趨�õ�ƥ�����ݷ��͵�T2MR0, DMA�����ж�ʱ����T2MR0
    �ж�, T2MR0�ж������ٿ�ʼUART3��IrDA��������
2), SSP0ֻʹ�ܽ��ռ����ж�, ���ͺͽ��ռ�������ͬ, ÿ�β���INT�ű��غ���������
    ��״̬-��״̬-R/W..., ��ƥ���ж������250uS��CRDY, ��4000����������DRDY��ʶ
    ��250uS��1S�ĳ�ʱ, �ֱ�û��Ӳ���������ʶ���м�¼. ���һ�η��ͽ���36�ֽ�
    AD��ͨ������ʹ��SSP0
3), UARTÿ��ͨ��ʹ�õ����ڴ���������������, IrDA�Ľ��պͷ���ʹ��ͬһ��DMAͨ��
    �ͻ�����
******************************************************************************/
void GPDMA_Init (void)
{
    PCONP |= (1 << PCGPDMA);                            //enable GPDMA
//    DISABLE_ISR(NVIC_GP_DMA);
    DMACIntErrClr = DMACIntTCClear = 0xff;              //���жϱ�ʶ
    DMACSync = 0;                                       //ʹ��ͬ��, Ĭ�ϵ�Ҳ��
//    DMAReqSel = 0x10;                                   //ͨ��12����MAT2.0����������UART��
	DMAReqSel = 0;

//    DMACC0DestAddr = (U32)(&SSP0DR);
//    DMACC0LLI = 0;
//    DMACC0Control = DMACC0_CONTROL;
//    DMACC0Config = DMACC0_CONFIG;
//
//    DMACC1SrcAddr = (U32)(&SSP0DR);                     //SSP0.Rx
//    DMACC1LLI = 0;
//    DMACC1Control = DMACC1_CONTROL;
//    DMACC1Config = DMACC1_CONFIG;
//
//    DMACC2LLI = 0;
//    DMACC2Control = 0;
//    DMACC2Config = 0;

    DMACC3DestAddr = (U32)(&U1THR);                     //UART1.Tx
    DMACC3LLI = 0;

//    DMACC3Control = 0;
	DMACC3Control = ((1 << 26)|0x80000000);

    DMACC3Config = 0;

//    DMACC4DestAddr = (U32)(&U0THR);                     //UART0.Tx
//    DMACC4LLI = 0;
//    DMACC4Control = 0;
//    DMACC4Config = 0;

    DMACConfig = 0x01;                                  //ʹ��С��ģʽGPDMA
//    ENABLE_ISR(NVIC_GP_DMA, PRIO_GPDMA);
}

#pragma push
#pragma Otime
/******************************************************************************
** ��������: void DMA_IRQHandler(void)
** ��������: DMA������ϼ��������ӳ���
** ��ڲ���: ��
** �� �� ֵ: ��
** ˵    ��: ������15uS�ڴ������
******************************************************************************/
void DMA_IRQHandler (void)
{
    U32 tmp;

    tmp = DMACIntStat & 0xff;
/*
    if(tmp & (1 << 1))                                  //���յ���ָ������, ��SSP0���ݷ������
    {
        DMACIntTCClear = (1 << 1);                      //������ԭ�����жϱ�ʶ
        if((AD_Ctrl_Id & 3) == 1)                       //ǰ��������Ƿ���AD1��
        {
            CS5463_CS_DIS1();                           //AD1Ƭѡ��Ч
            if(AD_Ctrl_Id & (3 << 2))                   //����ͨ����Ҫ��������
            {
                if((AD_Ctrl_Id & (3 << 2)) != (2 << 2)) //�ȴ��Ĳ���AD2, д�����ʶ
                {
                    AD_Channel_Error |= (1 << 0);
                }
                Ad_Rd_Buf[MAX_SSP0_BUF_LEN - 1] = Ad_Wr_Buf[MAX_SSP0_BUF_LEN - 1];
                copy_ssp0_buf(Ad_Wr_Buf, AD_Ctr[AD2].Dat_Buf);
                copy_ssp0_buf((U32 *)AD_Ctr[AD1].Rcv_Buf, Ad_Rd_Buf);  //ȡ���յ����ݸ�������, AD_Ctr[AD2].Dat_buf��������ݻ�δ����
                CS5463_CS_EN2();                        //AD2Ƭѡ��Ч
                startup_ssp0_gpdma();                   //��������
                AD_Ctrl_Id = 2;
                set_ad_send_buf(AD2);                   //��֡
            }
            else                                        //ͨ������������
            {
                AD_Ctrl_Id = 0;
                Ad_Rd_Buf[MAX_SSP0_BUF_LEN - 1] = Ad_Wr_Buf[MAX_SSP0_BUF_LEN - 1];
                copy_ssp0_buf((U32 *)AD_Ctr[AD1].Rcv_Buf, Ad_Rd_Buf);  //ȡ���յ����ݸ�������
            }
            switch(AD_Ctr[AD1].Ad_Sta_Id & AD_STAID_BIT_AD_STATUS2BITMASK)
            {
            case AD_STAID_BIT_AD_STATUS_INIT:           //��ʼ��״̬
                if(AD_STAID_BIT_INIT_SET_WD == AD_STATUS_3BITGET(AD_Ctr[AD1].Ad_Sta_Id, AD_STAID_BIT_INIT_BITNUM))
                {
                    AD_Ctr[AD1].Ad_Sta_Id &= ~(AD_STAID_BIT_AD_STATUS2BITMASK | (1 << AD_STAID_BIT_NORM_FIRST));
                    AD_Ctr[AD1].Ad_Sta_Id |= AD_STAID_BIT_AD_STATUS_NORM;
                    AD_Ctr[AD1].Id_Reg &= ~((1 << ID_REG_BIT_SEND_SECOND) | (1 << ID_REG_BIT_SEND_MACHINE));//û����, ��λ����д
#ifndef TEST_WWLLZZ
                    if(Sys_Share_Reg & (1 << SYS_SHARE_REG_BIT_QR)) //P/Qѡ��
                    {
                        AD_Ctr[AD1].Id_Reg |= (1 << ID_REG_BIT_SEND_QR);
                    }
                    else                                //ѡP
                    {
                        AD_Ctr[AD1].Id_Reg &= ~(1 << ID_REG_BIT_SEND_QR);
                    }
#endif  //TEST_WWLLZZ
                    set_ad_send_buf(AD1);               //׼��������֡����
                    T1MR1 = T1TC + (T1_MAT_252US * 2);
                }
                break;
            case AD_STAID_BIT_AD_STATUS_NORM:           //������ȡ����״̬
#ifndef TEST_WWLLZZ
                ad_rcv_analysis(AD1);                   //����
#else   //TEST_WWLLZZ
                ad_rcv_analysis_AD1();                  //����
#endif  //TEST_WWLLZZ
                break;
            case AD_STAID_BIT_AD_STATUS_CALI:           //У׼״̬
                if(AD_STAID_BIT_CALI_SAVE == AD_STATUS_3BITGET(AD_Ctr[AD1].Ad_Sta_Id, AD_STAID_BIT_CALI_BITNUM))
                {
                    if((AD_Ctr[AD1].Rcv_Buf[0] & (0x80 << 8)) == 0)     //״̬�Ĵ��������DRDYλ, �б�У׼�Ƿ�ɹ�
                    {
                        AD_Ctr[AD1].Ad_Sta_Id |= (1 << AD_STAID_BIT_AD_STATUS_DRDY);
                    }
                    else                                //У׼�������
                    {
                        if(CS5463_IDCOFF == (AD_Ctr[AD1].Rd_Command & 0xff))//�������ݷ���
                        {
                            Calib_Ref32[AD1][0] = ((AD_Ctr[AD1].Rcv_Buf[1] & (~0xff)) | (IDCOFF << 1) | 0x40);
                        }
                        else                            //��ѹ
                        {
                            Calib_Ref32[AD1][1] = ((AD_Ctr[AD1].Rcv_Buf[1] & (~0xff)) | (VDCOFF << 1) | 0x40);
                        }
                    }
                    AD_Ctr[AD1].Ad_Sta_Id = 0;          //�ص��ϵ�ʱ��״̬
                    REV_XOR_INSIDE_BIT(OUTSIDE_IN_INT_GROUP_BIT_REQ_FINISH_AD1);//֪ͨУ׼���
                }
                break;
//            default:                                    //IDLE, ������
//                break;
            }
        }
        else if((AD_Ctrl_Id & 3) == 2)                  //����AD2��
        {
            CS5463_CS_DIS2();                           //AD2Ƭѡ��Ч
            if(AD_Ctrl_Id & (3 << 2))                   //�б�����ͨ���Ƿ���Ҫ��������
            {
                if((AD_Ctrl_Id & (3 << 2)) != (1 << 2)) //�ȴ��Ĳ���AD1, дͨ�������ʶ
                {
                    AD_Channel_Error |= (1 << 1);
                }
                Ad_Rd_Buf[MAX_SSP0_BUF_LEN - 1] = Ad_Wr_Buf[MAX_SSP0_BUF_LEN - 1];
                copy_ssp0_buf(Ad_Wr_Buf, AD_Ctr[AD1].Dat_Buf);
                copy_ssp0_buf((U32 *)AD_Ctr[AD2].Rcv_Buf, Ad_Rd_Buf);  //ȡ���յ����ݸ�������, AD_Ctr[AD2].Dat_buf��������ݻ�δ����
                CS5463_CS_EN1();                        //AD1Ƭѡ��Ч
                startup_ssp0_gpdma();                   //��������
                AD_Ctrl_Id = 1;                         //��ǰ����ͨ��
                set_ad_send_buf(AD1);                   //��֡
            }
            else                                        //ͨ������������
            {
                AD_Ctrl_Id = 0;
                Ad_Rd_Buf[MAX_SSP0_BUF_LEN - 1] = Ad_Wr_Buf[MAX_SSP0_BUF_LEN - 1];
                copy_ssp0_buf((U32 *)AD_Ctr[AD2].Rcv_Buf, Ad_Rd_Buf);  //ȡ���յ����ݸ�������
            }
            switch(AD_Ctr[AD2].Ad_Sta_Id & AD_STAID_BIT_AD_STATUS2BITMASK)
            {
            case AD_STAID_BIT_AD_STATUS_INIT:           //��ʼ��״̬
                if(AD_STAID_BIT_INIT_SET_WD == AD_STATUS_3BITGET(AD_Ctr[AD2].Ad_Sta_Id, AD_STAID_BIT_INIT_BITNUM))
                {
                    AD_Ctr[AD2].Ad_Sta_Id &= ~(AD_STAID_BIT_AD_STATUS2BITMASK | (1 << AD_STAID_BIT_NORM_FIRST));
                    AD_Ctr[AD2].Ad_Sta_Id |= AD_STAID_BIT_AD_STATUS_NORM;
                    AD_Ctr[AD2].Id_Reg &= ~((1 << ID_REG_BIT_SEND_SECOND) | (1 << ID_REG_BIT_SEND_MACHINE));//û����, ��λ����д
#ifndef TEST_WWLLZZ
                    if(Sys_Share_Reg & (1 << SYS_SHARE_REG_BIT_QR)) //P/Qѡ��
                    {
                        AD_Ctr[AD2].Id_Reg |= (1 << ID_REG_BIT_SEND_QR);
                    }
                    else                                //ѡP
                    {
                        AD_Ctr[AD2].Id_Reg &= ~(1 << ID_REG_BIT_SEND_QR);
                    }
#endif  //TEST_WWLLZZ
                    set_ad_send_buf(AD2);               //׼��Ҫ���͵�֡����
                    T1MR2 = T1TC + (T1_MAT_252US * 2);
                }
                break;
            case AD_STAID_BIT_AD_STATUS_NORM:           //������ȡ����״̬
#ifndef TEST_WWLLZZ
                ad_rcv_analysis(AD2);                   //����
#else   //TEST_WWLLZZ
                ad_rcv_analysis_AD2();                  //����
#endif  //TEST_WWLLZZ
                break;
            case AD_STAID_BIT_AD_STATUS_CALI:           //У׼״̬
                if(AD_STAID_BIT_CALI_SAVE == AD_STATUS_3BITGET(AD_Ctr[AD2].Ad_Sta_Id, AD_STAID_BIT_CALI_BITNUM))
                {
                    if((AD_Ctr[AD2].Dat_Buf[0] & (0x80 << 8)) == 0)     //״̬�Ĵ��������DRDYλ, �б�У׼�Ƿ�ɹ�
                    {
                        AD_Ctr[AD2].Ad_Sta_Id |= (1 << AD_STAID_BIT_AD_STATUS_DRDY);
                    }
                    else                                //У׼�������
                    {
                        if(CS5463_IDCOFF == (AD_Ctr[AD2].Rd_Command & 0xff))//�������ݷ���
                        {
                            Calib_Ref32[AD2][0] = ((AD_Ctr[AD2].Rcv_Buf[1] & (~0xff)) | (IDCOFF << 1) | 0x40);
                        }
                        else                            //��ѹ
                        {
                            Calib_Ref32[AD2][1] = ((AD_Ctr[AD2].Rcv_Buf[1] & (~0xff)) | (VDCOFF << 1) | 0x40);
                        }
                    }
                    AD_Ctr[AD2].Ad_Sta_Id = 0;          //�ص��ϵ�ʱ��״��
                    REV_XOR_INSIDE_BIT(OUTSIDE_IN_INT_GROUP_BIT_REQ_FINISH_AD2);//AD2��ʶУ׼���
                }
                break;
            }
        }
        else                                            //ͨ����ʶ����
        {
            AD_Channel_Error |= (1 << 2);
            AD_Ctrl_Id = 0;                             //�����
        }
    }
    if(tmp & (1 << 2))                                  //ƥ�����ݷ������
    {
        DMACIntTCClear = (1 << 2);                      //DMA�ж����첽�ģ����Ը�����
        if(Sync_Send_Status == 1)                       //�����ͬ���ֽڡ�0xfe����ƥ������DMA�������
        {
            record_state_machine(3, SYNC_BYTE_DMA_END);
#ifdef UART3_IRDA_ENABLE
            T2EMR = 0;                                  //�´�ƥ��ʱ����תMAT2.0���ţ���ʼ��ƽ��0��
#else// UART3_IRDA_ENABLE
            T2EMR = 1;                                  //�´�ƥ��ʱ����תMAT2.0���ţ���ʼ��ƽ��1��
#endif// UART3_IRDA_ENABLE
            T2IR = 1;                                   //��T2MR0ƥ���ʶ
            T2MCR = 0x0001;                             //����ƥ��0�ж�
        }
        else if(Sync_Send_Status == 2)                  //�����UART3�û�����DMA�������
        {
            T2MR0 = T2TC + 13200;                       //����1.5���ֽڳ��ȵ�ƥ���������ڼ��UART3�������
            T2IR = 1;                                   //��T2MR0ƥ���ʶ
            T2MCR = 0x0001;                             //����ƥ��0�ж�

            record_state_machine(3, DATA_DMA_END);
            if(State_Machine[3].Send_State == DATA_SEND)
            {
                State_Machine[3].Send_End_Tick_Count = Sys_Tick;
                record_state_machine(3, DATA_SEND_END);
                State_Machine[3].Send_State = DATA_SEND_END;
            }
        }
    }
*/
    DMACIntTCClear = 0xf9u;                             //SSP0�Ľ���, UART0��UART1�ķ����ô������ȥ����
    tmp = DMACIntErrStat;
    if(tmp != 0)                                        //�д���
    {
#ifdef __DEBUG
        __nop();
        while(1);                                       //ԭ�����...
#else   //__DEBUG
        DMACIntErrClr = 0xffu;
#endif  //__DEBUG
    }
}

#pragma pop

/******************************************************************************
����: void startup_ssp0_gpdma(void)
���: ��, ���ȴ��ڻ�������ĩ��, ���28, Ϊ4��������
����: ��
����: ����SSP0��DMA����, �Գ��Ƚ����˴���
******************************************************************************/
/*
void startup_ssp0_gpdma (void)
{
    U8 l;

    l = Ad_Wr_Buf[MAX_SSP0_BUF_LEN - 1] & 0x1c;
    DMACC1DestAddr = (U32)Ad_Rd_Buf;                    //�ȿ���SSP0��DMA1����
    DMACC1Control = l | DMACC1_CONTROL;
    DMACC1Config = DMACC1_CONFIG | 1;
    DMACC0SrcAddr = (U32)Ad_Wr_Buf;
    DMACC0Control = l | DMACC0_CONTROL;
    DMACC0Config = DMACC0_CONFIG | 1;                   //������ʼ����
}
*/
/******************************************************************************
    End Of File
******************************************************************************/

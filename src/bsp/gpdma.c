/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : gpdma.c
 Description    : LPC17系列芯片GPDMA子程序
-------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、	  1.01		  2011-07-17    azh             zigbee DMA send query
   1、    1.00        2011-03-03    wwllzz          the original version
******************************************************************************/
#include "..\inc\global_config.h"



/******************************************************************************
函数: void Zigbee_DMA_Init(void)
入口: 无
出口: 无
概述: 初始化GPDMA, 总共有8通道, 16线
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
函数: void GPDMA_Init(void)
入口: 无
出口: 无
概述: 初始化GPDMA, 总共有8通道, 16线
SSP0.Tx -> 0[0],  SSP0.Rx-> 1[1],  MAT2.0 -> 12[2]
1), IrDA发送FE, 采用按照将事先设定好的匹配数据发送到T2MR0, DMA计数中断时开启T2MR0
    中断, T2MR0中断里面再开始UART3的IrDA发送数据
2), SSP0只使能接收计数中断, 发送和接收计数器相同, 每次捕获到INT脚边沿后启动发送
    读状态-清状态-R/W..., 用匹配中断来监控250uS的CRDY, 用4000计数来监视DRDY标识
    对250uS和1S的超时, 分别没有硬件和软件标识进行记录. 最多一次发送接收36字节
    AD两通道交替使用SSP0
3), UART每个通道使用单独内存块来缓冲接收数据, IrDA的接收和发送使用同一个DMA通道
    和缓冲区
******************************************************************************/
void GPDMA_Init (void)
{
    PCONP |= (1 << PCGPDMA);                            //enable GPDMA
//    DISABLE_ISR(NVIC_GP_DMA);
    DMACIntErrClr = DMACIntTCClear = 0xff;              //清中断标识
    DMACSync = 0;                                       //使能同步, 默认的也是
//    DMAReqSel = 0x10;                                   //通道12用于MAT2.0，其它都是UART用
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

    DMACConfig = 0x01;                                  //使能小端模式GPDMA
//    ENABLE_ISR(NVIC_GP_DMA, PRIO_GPDMA);
}

#pragma push
#pragma Otime
/******************************************************************************
** 函数名称: void DMA_IRQHandler(void)
** 函数功能: DMA计数完毕及错误处理子程序
** 入口参数: 无
** 返 回 值: 无
** 说    明: 必须在15uS内处理完毕
******************************************************************************/
void DMA_IRQHandler (void)
{
    U32 tmp;

    tmp = DMACIntStat & 0xff;
/*
    if(tmp & (1 << 1))                                  //接收到了指定数据, 即SSP0数据发送完毕
    {
        DMACIntTCClear = (1 << 1);                      //先清下原来的中断标识
        if((AD_Ctrl_Id & 3) == 1)                       //前面的数据是发给AD1的
        {
            CS5463_CS_DIS1();                           //AD1片选无效
            if(AD_Ctrl_Id & (3 << 2))                   //另外通道有要发送数据
            {
                if((AD_Ctrl_Id & (3 << 2)) != (2 << 2)) //等待的不是AD2, 写错误标识
                {
                    AD_Channel_Error |= (1 << 0);
                }
                Ad_Rd_Buf[MAX_SSP0_BUF_LEN - 1] = Ad_Wr_Buf[MAX_SSP0_BUF_LEN - 1];
                copy_ssp0_buf(Ad_Wr_Buf, AD_Ctr[AD2].Dat_Buf);
                copy_ssp0_buf((U32 *)AD_Ctr[AD1].Rcv_Buf, Ad_Rd_Buf);  //取出收到数据给接收者, AD_Ctr[AD2].Dat_buf保存的数据还未发送
                CS5463_CS_EN2();                        //AD2片选有效
                startup_ssp0_gpdma();                   //启动发送
                AD_Ctrl_Id = 2;
                set_ad_send_buf(AD2);                   //组帧
            }
            else                                        //通道接下来空闲
            {
                AD_Ctrl_Id = 0;
                Ad_Rd_Buf[MAX_SSP0_BUF_LEN - 1] = Ad_Wr_Buf[MAX_SSP0_BUF_LEN - 1];
                copy_ssp0_buf((U32 *)AD_Ctr[AD1].Rcv_Buf, Ad_Rd_Buf);  //取出收到数据给接收者
            }
            switch(AD_Ctr[AD1].Ad_Sta_Id & AD_STAID_BIT_AD_STATUS2BITMASK)
            {
            case AD_STAID_BIT_AD_STATUS_INIT:           //初始化状态
                if(AD_STAID_BIT_INIT_SET_WD == AD_STATUS_3BITGET(AD_Ctr[AD1].Ad_Sta_Id, AD_STAID_BIT_INIT_BITNUM))
                {
                    AD_Ctr[AD1].Ad_Sta_Id &= ~(AD_STAID_BIT_AD_STATUS2BITMASK | (1 << AD_STAID_BIT_NORM_FIRST));
                    AD_Ctr[AD1].Ad_Sta_Id |= AD_STAID_BIT_AD_STATUS_NORM;
                    AD_Ctr[AD1].Id_Reg &= ~((1 << ID_REG_BIT_SEND_SECOND) | (1 << ID_REG_BIT_SEND_MACHINE));//没有秒, 上位机读写
#ifndef TEST_WWLLZZ
                    if(Sys_Share_Reg & (1 << SYS_SHARE_REG_BIT_QR)) //P/Q选择
                    {
                        AD_Ctr[AD1].Id_Reg |= (1 << ID_REG_BIT_SEND_QR);
                    }
                    else                                //选P
                    {
                        AD_Ctr[AD1].Id_Reg &= ~(1 << ID_REG_BIT_SEND_QR);
                    }
#endif  //TEST_WWLLZZ
                    set_ad_send_buf(AD1);               //准备待发送帧数据
                    T1MR1 = T1TC + (T1_MAT_252US * 2);
                }
                break;
            case AD_STAID_BIT_AD_STATUS_NORM:           //正常读取数据状态
#ifndef TEST_WWLLZZ
                ad_rcv_analysis(AD1);                   //解析
#else   //TEST_WWLLZZ
                ad_rcv_analysis_AD1();                  //解析
#endif  //TEST_WWLLZZ
                break;
            case AD_STAID_BIT_AD_STATUS_CALI:           //校准状态
                if(AD_STAID_BIT_CALI_SAVE == AD_STATUS_3BITGET(AD_Ctr[AD1].Ad_Sta_Id, AD_STAID_BIT_CALI_BITNUM))
                {
                    if((AD_Ctr[AD1].Rcv_Buf[0] & (0x80 << 8)) == 0)     //状态寄存器里面的DRDY位, 判别校准是否成功
                    {
                        AD_Ctr[AD1].Ad_Sta_Id |= (1 << AD_STAID_BIT_AD_STATUS_DRDY);
                    }
                    else                                //校准正常完成
                    {
                        if(CS5463_IDCOFF == (AD_Ctr[AD1].Rd_Command & 0xff))//读出数据放哪
                        {
                            Calib_Ref32[AD1][0] = ((AD_Ctr[AD1].Rcv_Buf[1] & (~0xff)) | (IDCOFF << 1) | 0x40);
                        }
                        else                            //电压
                        {
                            Calib_Ref32[AD1][1] = ((AD_Ctr[AD1].Rcv_Buf[1] & (~0xff)) | (VDCOFF << 1) | 0x40);
                        }
                    }
                    AD_Ctr[AD1].Ad_Sta_Id = 0;          //回到上电时的状态
                    REV_XOR_INSIDE_BIT(OUTSIDE_IN_INT_GROUP_BIT_REQ_FINISH_AD1);//通知校准完成
                }
                break;
//            default:                                    //IDLE, 不处理
//                break;
            }
        }
        else if((AD_Ctrl_Id & 3) == 2)                  //发给AD2的
        {
            CS5463_CS_DIS2();                           //AD2片选无效
            if(AD_Ctrl_Id & (3 << 2))                   //判别另外通道是否有要发送数据
            {
                if((AD_Ctrl_Id & (3 << 2)) != (1 << 2)) //等待的不是AD1, 写通道错误标识
                {
                    AD_Channel_Error |= (1 << 1);
                }
                Ad_Rd_Buf[MAX_SSP0_BUF_LEN - 1] = Ad_Wr_Buf[MAX_SSP0_BUF_LEN - 1];
                copy_ssp0_buf(Ad_Wr_Buf, AD_Ctr[AD1].Dat_Buf);
                copy_ssp0_buf((U32 *)AD_Ctr[AD2].Rcv_Buf, Ad_Rd_Buf);  //取出收到数据给接收者, AD_Ctr[AD2].Dat_buf保存的数据还未发送
                CS5463_CS_EN1();                        //AD1片选有效
                startup_ssp0_gpdma();                   //启动发送
                AD_Ctrl_Id = 1;                         //当前发送通道
                set_ad_send_buf(AD1);                   //组帧
            }
            else                                        //通道接下来空闲
            {
                AD_Ctrl_Id = 0;
                Ad_Rd_Buf[MAX_SSP0_BUF_LEN - 1] = Ad_Wr_Buf[MAX_SSP0_BUF_LEN - 1];
                copy_ssp0_buf((U32 *)AD_Ctr[AD2].Rcv_Buf, Ad_Rd_Buf);  //取出收到数据给接收者
            }
            switch(AD_Ctr[AD2].Ad_Sta_Id & AD_STAID_BIT_AD_STATUS2BITMASK)
            {
            case AD_STAID_BIT_AD_STATUS_INIT:           //初始化状态
                if(AD_STAID_BIT_INIT_SET_WD == AD_STATUS_3BITGET(AD_Ctr[AD2].Ad_Sta_Id, AD_STAID_BIT_INIT_BITNUM))
                {
                    AD_Ctr[AD2].Ad_Sta_Id &= ~(AD_STAID_BIT_AD_STATUS2BITMASK | (1 << AD_STAID_BIT_NORM_FIRST));
                    AD_Ctr[AD2].Ad_Sta_Id |= AD_STAID_BIT_AD_STATUS_NORM;
                    AD_Ctr[AD2].Id_Reg &= ~((1 << ID_REG_BIT_SEND_SECOND) | (1 << ID_REG_BIT_SEND_MACHINE));//没有秒, 上位机读写
#ifndef TEST_WWLLZZ
                    if(Sys_Share_Reg & (1 << SYS_SHARE_REG_BIT_QR)) //P/Q选择
                    {
                        AD_Ctr[AD2].Id_Reg |= (1 << ID_REG_BIT_SEND_QR);
                    }
                    else                                //选P
                    {
                        AD_Ctr[AD2].Id_Reg &= ~(1 << ID_REG_BIT_SEND_QR);
                    }
#endif  //TEST_WWLLZZ
                    set_ad_send_buf(AD2);               //准备要发送的帧数据
                    T1MR2 = T1TC + (T1_MAT_252US * 2);
                }
                break;
            case AD_STAID_BIT_AD_STATUS_NORM:           //正常读取数据状态
#ifndef TEST_WWLLZZ
                ad_rcv_analysis(AD2);                   //解析
#else   //TEST_WWLLZZ
                ad_rcv_analysis_AD2();                  //解析
#endif  //TEST_WWLLZZ
                break;
            case AD_STAID_BIT_AD_STATUS_CALI:           //校准状态
                if(AD_STAID_BIT_CALI_SAVE == AD_STATUS_3BITGET(AD_Ctr[AD2].Ad_Sta_Id, AD_STAID_BIT_CALI_BITNUM))
                {
                    if((AD_Ctr[AD2].Dat_Buf[0] & (0x80 << 8)) == 0)     //状态寄存器里面的DRDY位, 判别校准是否成功
                    {
                        AD_Ctr[AD2].Ad_Sta_Id |= (1 << AD_STAID_BIT_AD_STATUS_DRDY);
                    }
                    else                                //校准正常完成
                    {
                        if(CS5463_IDCOFF == (AD_Ctr[AD2].Rd_Command & 0xff))//读出数据放哪
                        {
                            Calib_Ref32[AD2][0] = ((AD_Ctr[AD2].Rcv_Buf[1] & (~0xff)) | (IDCOFF << 1) | 0x40);
                        }
                        else                            //电压
                        {
                            Calib_Ref32[AD2][1] = ((AD_Ctr[AD2].Rcv_Buf[1] & (~0xff)) | (VDCOFF << 1) | 0x40);
                        }
                    }
                    AD_Ctr[AD2].Ad_Sta_Id = 0;          //回到上电时的状况
                    REV_XOR_INSIDE_BIT(OUTSIDE_IN_INT_GROUP_BIT_REQ_FINISH_AD2);//AD2标识校准完成
                }
                break;
            }
        }
        else                                            //通道标识错误
        {
            AD_Channel_Error |= (1 << 2);
            AD_Ctrl_Id = 0;                             //简单清掉
        }
    }
    if(tmp & (1 << 2))                                  //匹配数据发送完毕
    {
        DMACIntTCClear = (1 << 2);                      //DMA中断是异步的，所以各自清
        if(Sync_Send_Status == 1)                       //如果是同步字节‘0xfe’的匹配数据DMA传输完成
        {
            record_state_machine(3, SYNC_BYTE_DMA_END);
#ifdef UART3_IRDA_ENABLE
            T2EMR = 0;                                  //下次匹配时不翻转MAT2.0引脚，初始电平‘0’
#else// UART3_IRDA_ENABLE
            T2EMR = 1;                                  //下次匹配时不翻转MAT2.0引脚，初始电平‘1’
#endif// UART3_IRDA_ENABLE
            T2IR = 1;                                   //清T2MR0匹配标识
            T2MCR = 0x0001;                             //开启匹配0中断
        }
        else if(Sync_Send_Status == 2)                  //如果是UART3用户数据DMA传输完成
        {
            T2MR0 = T2TC + 13200;                       //产生1.5个字节长度的匹配间隔，用于检查UART3发送完成
            T2IR = 1;                                   //清T2MR0匹配标识
            T2MCR = 0x0001;                             //开启匹配0中断

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
    DMACIntTCClear = 0xf9u;                             //SSP0的接收, UART0和UART1的发送让处理程序去管理
    tmp = DMACIntErrStat;
    if(tmp != 0)                                        //有错误
    {
#ifdef __DEBUG
        __nop();
        while(1);                                       //原因待查...
#else   //__DEBUG
        DMACIntErrClr = 0xffu;
#endif  //__DEBUG
    }
}

#pragma pop

/******************************************************************************
函数: void startup_ssp0_gpdma(void)
入口: 无, 长度存在缓冲区最末字, 最大28, 为4的整数倍
出口: 无
概述: 启动SSP0的DMA传送, 对长度进行了处理
******************************************************************************/
/*
void startup_ssp0_gpdma (void)
{
    U8 l;

    l = Ad_Wr_Buf[MAX_SSP0_BUF_LEN - 1] & 0x1c;
    DMACC1DestAddr = (U32)Ad_Rd_Buf;                    //先开启SSP0的DMA1接收
    DMACC1Control = l | DMACC1_CONTROL;
    DMACC1Config = DMACC1_CONFIG | 1;
    DMACC0SrcAddr = (U32)Ad_Wr_Buf;
    DMACC0Control = l | DMACC0_CONTROL;
    DMACC0Config = DMACC0_CONFIG | 1;                   //真正开始传输
}
*/
/******************************************************************************
    End Of File
******************************************************************************/

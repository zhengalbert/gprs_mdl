/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : sync_data.c
 Description    : 通过zigbee通道同步高压电能表数据
-------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2011-07-17    azh          the original version
******************************************************************************/
#include    "..\inc\global_config.h"

//------------------------------ 私有变量声明 ---------------------------------

//------------------------------ 私有函数声明 -------------------------------

//-------------------------------常量定义-------------------------------------
//同步用当前电量表格 不能超出30项 因为都是安排在一分钟之内同步完成 当前和上1结算日电需量

//对于华采扩展规约的定义
//这里是为了发送组帧方便 程序内部定义 
//DI3=0x99:代表是扩展要特殊处理
//DI2=华采扩展规约的id4
//DI1=0:保留
//DI0=0:当前 DI0=n:上n个结算日 对应扩展规约的di6
#if CODE_VER_TYPE==H_METER_PRJ				//配合高压表
#ifdef ZIGBEE_TEST
const U32 sync_minute_data_tab[SYNC_MINUTE_MAX] =
{
    0x99000000,     //当前日期及时间 此处为扩展数据标识
    //0x07000001 0x00 0x09 0x00, 实际命令帧中的扩展标识
    0x0000ff00,     //(当前)组合有功电能数据块
    0x0001ff00,     //(当前)正向有功电能数据块
    0x0002ff00,     //(当前)反向有功电能数据块
    0x0003ff00,     //(当前)组合无功1电能数据块
    0x0004ff00,     //(当前)组合无功2电能数据块
    0x0005ff00,     //(当前)第一象限无功电能数据块
    0x0006ff00,     //(当前)第二象限无功电能数据块
    0x0007ff00,     //(当前)第三象限无功电能数据块
    0x0008ff00,     //(当前)第四象限无功电能数据块
    0x0009ff00,     //(当前)正向视在电能数据块
    0x000Aff00,     //(当前)反向视在电能数据块
    0x99010000,     //(当前)关联总、A、B、C电能数据块   此数据标识为扩展
    //0x07000001 0x01 0x09 0x00, 实际命令帧中的扩展标识
    0x99020000,     //(当前)A、B、C相电能数据块 此数据标识为扩展
    //0x07000001 0x02 0x09 0x00, 实际命令帧中的扩展标识

//同步用当前最大需量及发生时间
    0x0101ff00,     //(当前)正向有功最大需量及发生时间数据块
    0x0102ff00,     //(当前)反向有功最大需量及发生时间数据块
    0x0103ff00,     //(当前)组合无功1最大需量及发生时间数据块
    0x0104ff00,     //(当前)组合无功2最大需量及发生时间数据块
    0x0105ff00,     //(当前)第一象限无功最大需量及发生时间数据块
    0x0106ff00,     //(当前)第二象限无功最大需量及发生时间数据块
    0x0107ff00,     //(当前)第三象限无功最大需量及发生时间数据块
    0x0108ff00,     //(当前)第四象限无功最大需量及发生时间数据块
    0x0109ff00,     //(当前)正向视在最大需量及发生时间数据块
    0x010Aff00,     //(当前)反向视在最大需量及发生时间数据块
    0x99030000,     //(当前)A相最大需量及发生时间数据块   此数据标识为扩展 
    //0x07000001 0x03 0x09 0x00, 实际命令帧中的扩展标识 
    0x99040000,     //(当前)B相最大需量及发生时间数据块   此数据标识为扩展
    //0x07000001 0x04 0x09 0x00, 实际命令帧中的扩展标识
    0x99050000,     //(当前)C相最大需量及发生时间数据块   此数据标识为扩展
    //0x07000001 0x05 0x09 0x00, 实际命令帧中的扩展标识
};
#endif//ZIGBEE_TEST
#endif//H_METER_PRJ
/******************************************************************************
** 函数名称: void sync_data_init(void)
** 函数功能:
** 入口参数:
** 返 回 值:
**
** 说    明: 上电初始化相关同步用到的控制状态及变量
******************************************************************************/
void sync_data_init (void)
{
    U8 i;

    Dmd_Up_Flg = 0;
    Eng_Up_Flg = 0;

    for(i=0; i<7; i++)
    {
        gMeter_State[i] = 0;//电表状态字
    }
    gucPoweron = 1;//刚上电标志
}
/******************************************************************************
** 函数名称: U8 check_zigbee_broadcase_flag(void)
** 函数功能:
** 入口参数:
** 返 回 值:
**
** 说    明: 检查zigbee是广播命令发出去 则只同步时间其他同步命令不发 以免空中繁忙
******************************************************************************/
#ifdef ZIGBEE_TEST
U8 check_zigbee_broadcase_flag(void)
{
	U8 i;
	
    for(i=0; i<PAR_HVMETER_ZIGBEE_ADR_LEN; i++)
    {
        if(gMatch_Zigbee_Addr[i] != 0xff)//因为上电时的初始化成全0的
        {
            break;
        }
    }
    if(i >= PAR_HVMETER_ZIGBEE_ADR_LEN)
    {
        return 1;//如果是全FF 则允许每秒同步时间 其他同步命令就不发了 以免空中繁忙
    }
    else
    {
    	return 0;
    }	
}
//针对zigbee模块主动或或者被动模式
//azh 171017 电子式高压表GPRS模块，被动上行规约回数据
//zigbee地址和绑定的不一样，而且也不是FF(没绑定前)，也不是zigbee层的广播地址，那么就去默认解析支持被动上行规约
U8 check_zigbee_addr_unmatch(U8 *buf)
{
	U8 i,ret=RET_ERROR;

    if(gucGetZigbeeAddrFlag !=1 )//不是在绑定zigbee MAC地址状态
    {
        for(i=0; i<PAR_HVMETER_ZIGBEE_ADR_LEN; i++)
        {
            if(gMatch_Zigbee_Addr[i] != buf[i])
            {
                ret = RET_SUCCESS;//绑定的zigbee MAC地址和收到的zigbee MAC地址不对
                break;
            }
        }
    }
    return ret;    
}
#endif//ZIGBEE_TEST
/******************************************************************************
** 函数名称: U8 dis_ctrl_sync (void)
** 函数功能:
** 入口参数:
** 返 回 值:
**
** 说    明: 轮显或键显示触发在显示前实时同步数据
******************************************************************************/
#ifdef LCD_VALID
U8 dis_ctrl_sync (void)
{
    if( ((Dis_Ctrl.di & 0xff0000ff) == 0) || ((Dis_Ctrl.di & 0xff0000ff) == 0x01000000)//当前电需量本地会定时同步
        || ((Dis_Ctrl.di & 0xffffff00) == 0x04000500) //电表状态字
        || (Dis_Ctrl.di == 0x04000101) || (Dis_Ctrl.di == 0x04000102))//当前日期时间本地会定时同步
    {
       set_di_sync_finish(0);//不用比数据标识 直接置标志
       return __FALSE;
    }
	if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
	{
    	assemble_sync_send_645frame(Dis_Ctrl.di);//待显示的显示数据标识
    }

    return __TRUE;
}
#endif
/******************************************************************************
** 函数名称: void sync_sec_data(void)
** 函数功能:
** 入口参数:
** 返 回 值:
**
** 说    明: 每秒钟填写秒同步数据帧，并发出去
******************************************************************************/
void sync_sec_data (void)
{
//debug
//  assemble_sync_send_645frame(0x02010100);//A相电压
//  assemble_sync_send_645frame(0x02010200);//B相电压
//  assemble_sync_send_645frame(0x02010300);//C相电压

    assemble_sync_send_645frame(0x040005ff);//电表状态字数据块
//  assemble_sync_send_645frame(0x040001ff);//日期及时间

//  if(gSync_CurEnergy.Sync_Sn >= SYNC_ENG_MAX)
//  {
//      gSync_CurEnergy.Sync_Sn = 0;
//  }
//  assemble_sync_send_645frame(sync_cur_energy_tab[gSync_CurEnergy.Sync_Sn]);//当前某块电量数据块
//  gSync_CurEnergy.Sync_Sn++;
}
/******************************************************************************
** 函数名称: void sync_minute_data(U8 sec)
** 函数功能:
** 入口参数:
** 返 回 值:
**
** 说    明: 每分钟第0秒开始的前SYNC_MINUTE_MAX秒填写分钟同步数据帧，并发出去
******************************************************************************/
void sync_minute_data (U8 sec)
{
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
	//azh 171011 电子式高压表模块，是通过同步命令来绑定的，所以只要校验码对就要去发
//	#ifdef ZIGBEE_TEST
//	if(check_zigbee_broadcase_flag()==1)
//	{
//		return;	//azh 140517
//	}	
//	#endif//ZIGBEE_TEST
	if(sec==1)//在第每分钟的1秒去读下面这个数据项
	{	
    	assemble_sync_send_645frame(0x99000000);//刚上电把时间读回来
    }
#else
    if(sec < SYNC_MINUTE_MAX)
    {
        if(gucPoweron == 1)
        {
            gucPoweron = 0;
            assemble_sync_send_645frame(sync_minute_data_tab[0]);//刚上电把时间读回来
        }
        else
        {        
            assemble_sync_send_645frame(sync_minute_data_tab[sec]);//当前某块时间、电量、需量及发生时间数据块
        }
    }
#endif
/*
    if(gSync_Minute.Sn >= 60)
    {
        gSync_Minute.Sn = 0;
    }
    if(gSync_Minute.Sn < SYNC_MINUTE_MAX)
    {
        assemble_sync_send_645frame(sync_minute_data_tab[gSync_Minute.Sn]);//当前某块时间、电量、需量及发生时间数据块
    }
    gSync_Minute.Sn++;
*/
}
/******************************************************************************
** 函数名称: void sync_hour_data(U8 sec)
** 函数功能:
** 入口参数:
** 返 回 值:
**
** 说    明: 填写小时同步数据帧，并发出去 每小时的第1分钟的30秒后的SYNC_HOUR_MAX秒同步数据
******************************************************************************/
#if CODE_VER_TYPE==H_METER_PRJ				//配合高压表
void sync_hour_data (U8 sec)
{
    if((sec >= 31) && (sec < (30 + SYNC_HOUR_MAX)))//从第30秒开始同步每小时的数据 由于要去掉第一项时间数据从31开始
    {
        assemble_sync_send_645frame(sync_minute_data_tab[sec-30]+1);//上一结算日(di0=1)某块时间、电量、需量及发生时间数据块
    }
}
#endif
/******************************************************************************
** 函数名称: S8 assemble_sync_send_645frame(U32 di)
** 函数功能:
** 入口参数:
** 返 回 值:
**
** 说    明: 根据输入的数据标识组成645命令同步数据帧
******************************************************************************/
S8 assemble_sync_send_645frame (U32 di)
{
    U8  i;
//  U8  adr[6];
    U8  mem_type;
    U8  *p_send;
    U8  *mem_ptr;

/////测试debug 透传命令
//return __FALSE;
#ifdef ZIGBEE_TEST
    if(gucZigbee_Disable_Flag == CON_ZIGBEE_DISABLE)//手动命令禁止zigbee同步
    {
        return __FALSE;
    }    
//azh 171011 带显示的主动搜表功能，电子式高压表GPRS模块只有在配地址的时候才去绑定zigbee模块地址
    #ifdef LCD_VALID
    	if((__FALSE == check_match_valid()) || (CON_SEARCH_RUN_STATE == Dis_Search_Ctrl.state))//如果匹配地址不对 不会发同步帧
    	{
    		return __FALSE;
    	}
    #else//电子式高压表GPRS模块，不带显示，代码移值的是移动式用的GPRS模块
    	if(__FALSE == check_match_valid())//如果匹配地址不对 不会发同步帧
    	{
    		return __FALSE;
    	}
    #endif
#endif
    for(i=0; i<10; i++)
    {
        mem_ptr = alloc_comm_mem(MEM_256_TYPE, &mem_type, GET_MEM_ERR16);
        if(NULL != mem_ptr)
        {
            break;
        }
        os_dly_wait(2);
    }
    if(i >= 10)
    {
    	setmemgetnullid(GET_MEM_ERR0, gusCurMemErrFunName);	 
    	return __FALSE;
    }
    p_send = mem_ptr + DATA_OFFSET;

    i = 0;
    p_send[i++] = 0x68;
    //memcpy(p_send+i, adr, 6);

    //debug!!
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
	#ifdef ZIGBEE_TEST//azh 171111 电子式高压表GPRS模块
    memcpy(p_send+i, gMatch_HVMeter_Addr, 6);
	#else//移动式的内部通信口
	memset(p_send+i, 0xaa, 6);
	#endif//ZIGBEE_TEST

#else
    memcpy(p_send+i, gMatch_HVMeter_Addr, 6);
#endif
    i += 6;

    p_send[i++] = 0x68;
    p_send[i++] = 0x11;
	if((di & 0xff000000) == 0x99000000)
	{
		p_send[i++] = 0x07;//len

	    p_send[i++] = 0x01 + 0x33;//扩展标识
	    p_send[i++] = 0x00 + 0x33;
	    p_send[i++] = 0x00 + 0x33;
	    p_send[i++] = 0x07 + 0x33;
	    p_send[i++] = ((di>>16)&0xff) + 0x33;//扩展项序号
	    p_send[i++] = 0x09 + 0x33;//代表显示终端
	    p_send[i++] = (di&0xff) + 0x33;//0:当前 n:上n结算日
	}
	else
	{
	    p_send[i++] = 0x04;//len
	    p_send[i++] = (di&0xff) + 0x33;
	    p_send[i++] = ((di>>8)&0xff) + 0x33;
	    p_send[i++] = ((di>>16)&0xff) + 0x33;
	    p_send[i++] = ((di>>24)&0xff) + 0x33;
    }
    p_send[i++] = cal_frmcs(p_send, i);
    p_send[i++] = 0x16;

    //debug
    //memset(gAim_Addr, 0xff, 8);
//    gAim_Addr[0] = 0xFA;
//    gAim_Addr[1] = 0x37;
//    gAim_Addr[2] = 0x4E;
//    gAim_Addr[3] = 0x00;
//    gAim_Addr[4] = 0x00;
//    gAim_Addr[5] = 0x6F;
//    gAim_Addr[6] = 0x0D;
//    gAim_Addr[7] = 0x00;

    memcpy(&mem_ptr[ZIGBEE_RET_ADDR_OFFSET], gMatch_Zigbee_Addr, 8);//zigbee待发送目标地址
//    send_protocol_frame(mem_ptr, ZIGBEE_CHL, i, MEM_256_TYPE);
    send_wait_rev_frame(mem_ptr, ZIGBEE_CHL, i, MEM_256_TYPE, CON_TER_SYNFLASH);
    return __TRUE;

}
/******************************************************************************
    End Of File
******************************************************************************/

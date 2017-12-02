/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : search.c
 Description    : 问设备信息 搜索高压表地址 匹配绑定的地址
-------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2、
   1、    1.00        2011-08-17    azh          the original version
******************************************************************************/
#include    "..\inc\global_config.h"

#ifdef ZIGBEE_TEST//azh 171011电子式高压表GPRS模块//CODE_VER_TYPE!=GPRS_MODULE_PRJ		//不是带CPU_GPRS模块
#define HVMETER_ADDR_NO_PROKEY//如果此宏定义 则表明匹配地址不需要编程键 直接按键就可以匹配

extern U16 os_time;                                     //使用系统变量

//------------------------------ 私有变量声明 ---------------------------------
static const U8 identity[] = "T=Diste N=???????????? A=???????????? P=645-07 V=1.0-0000\r\n";
static const U8 ASCII_table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
//------------------------------ 私有函数声明 -------------------------------
//static U8 send_identity_frame(U8 *ptr);
static void add_in_dis_addr_list(U8 *addr);
static void set_search_data(void);
/******************************************************************************
** 函数名称: void add_in_dis_addr_list(U8 *addr)
** 函数功能: 把搜索到的高压表的通信地址及其zigbee模块地址保存到待显示缓冲
** 入口参数: 搜索到的高压表的通信地址及其zigbee模块地址
** 返 回 值: 返回数据域长度
** 说    明: 无
******************************************************************************/
void add_in_dis_addr_list (U8 *addr)
{
    U8 i;

    if(Dis_Search_Ctrl.totnum < CON_SEARCH_CMD_MAX)
    {
	    for(i=0; i<Dis_Search_Ctrl.totnum; i++)
	    {
	        if(memcmp(addr, &Dis_Search_Ctrl.addr[i][0], 14) == 0)//此地址已保存
	        {
	            return;
	        }
	    }
	    addr[14] = addverify(addr, 14);
	    memcpy(&Dis_Search_Ctrl.addr[Dis_Search_Ctrl.totnum][0], addr, CON_SEARCH_ADDR_LEN);
	    Dis_Search_Ctrl.totnum++;
	}
}
/******************************************************************************
** 函数名称: void send_identity_frame (void)
** 函数功能: 发送‘/?’指令的回应帧
** 入口参数: 无
** 返 回 值: 返回数据域长度
** 说    明: 无, 内部尝试分配使用256字节内存块, 没有的话放弃此次发送
******************************************************************************/
void send_identity_frame (void)
{
    U8 buf[6], i;
    U8 len;
    U8 *ptr;

    ptr = alloc_comm_mem(MEM_256_TYPE, &len, GET_MEM_ERR23);
    if(NULL == ptr)
    {
//	    memcpy(gusCurMemErrFunName, "send_ide", 8);
	   	setmemgetnullid(GET_MEM_ERR23, gusCurMemErrFunName); 
        return;                                         //此处该标识调试宏...
    }

    memset(ptr + ZIGBEE_RET_ADDR_OFFSET, 0xff, 8);      //广播地址
    len = sizeof(identity) - 1;
    memcpy(ptr + DATA_OFFSET, identity, len);
//    file_read(PAR_HVMNUM_04000402_00_ADDR, buf, PAR_HVMNUM_04000402_TOT_LEN);
    memcpy(buf, gMatch_HVMeter_Addr, PAR_HVMETER_ADR_LEN);//azh 120316
    for(i = 0; i < 6; i++)
    {
        ptr[DATA_OFFSET + 10 + i * 2] = ASCII_table[buf[5 - i] >> 4];
        ptr[DATA_OFFSET + 11 + i * 2] = ASCII_table[buf[5 - i] & 0x0f];
    }
//    file_read(PAR_HVMADDR_04000401_00_ADDR, buf, PAR_HVMADDR_04000401_TOT_LEN);
    memset(buf, 0, PAR_HVMETER_ADR_LEN);
    memcpy(&buf[0], stTerRunPara.stTerAddr.ucDevAddr, 2);    
    buf[2] = stTerRunPara.stTerRegionAddr.ucRegionAddr[1];
    buf[3] = stTerRunPara.stTerRegionAddr.ucRegionAddr[0];
    for(i = 0; i < 6; i++)
    {
        ptr[DATA_OFFSET + 25 + i * 2] = ASCII_table[buf[5 - i] >> 4];
        ptr[DATA_OFFSET + 26 + i * 2] = ASCII_table[buf[5 - i] & 0x0f];
    }
    send_protocol_frame(ptr, ZIGBEE_CHL, len, MEM_256_TYPE);
}
/******************************************************************************
** 函数名称: U8 analyse_identity_frame(U8 *in_ptr)
** 函数功能: 比对zigbee目标地址 如果是全0则每分钟匹配一次 非0则15分钟检查一次
** 入口参数: 接到指令的缓冲指针
** 返 回 值: 数据帧有没有得到处理，__TRUE已经处理，__FALSE没有处理
** 说    明: "T=Meter N=444444444444 A=111111111111 P=645-07 V=1.0-0000"(按看到的顺序传输)
******************************************************************************/
U8 analyse_identity_frame (U8 *in_ptr)
{
    U8 len, i;
    U8 data[CON_SEARCH_ADDR_LEN];
    U8 *ptr = NULL;

    len = in_ptr[0] + (in_ptr[1]<<8);
    if(len > 128)
    {
        return __FALSE;
    }

    memset(data, 0, CON_SEARCH_ADDR_LEN);
    memcpy(data, "T=Meter", 7);
//azh debug 
//    memcpy(data1, "T=Diste", 7);
//    if((memcmp(data, in_ptr+DATA_OFFSET, 7) == 0)||(memcmp(data1, in_ptr+DATA_OFFSET, 7) == 0))        //回设备信息    
    
    if(memcmp(data, in_ptr+DATA_OFFSET, 7) == 0)        //回设备信息
    {
#ifdef LCD_VALID
        if(CON_SEARCH_RUN_STATE != Dis_Search_Ctrl.state)
        {
            return __FALSE;                             //现在不是本终端在搜表, 其它搜表造成的不理会
        }
//azh 
#endif//LCD_VALID
        memcpy(data, "A=", 2);
        for(i = 2; i < len; i++)
        {
            if(memcmp(data, in_ptr+DATA_OFFSET + i, 2) == 0)
            {
                ptr = in_ptr + DATA_OFFSET + 2 + i;
                break;
            }
        }
        if((i < len) && ((i + 12) < len))               //找到标识且地址完整
        {
            memset(data, 0, 6);
            for(i = 0; i < 12; i++)
            {
                data[i / 2] |= ((asc_to_hex(ptr[11 - i]) & 0x0f) << (4 * (i % 2)));
            }
            memcpy(data + 6, in_ptr + ZIGBEE_RET_ADDR_OFFSET, 8);
            add_in_dis_addr_list(data);
            ptr = alloc_comm_mem(MEM_256_TYPE, &len, GET_MEM_ERR30);   //准备回应OK, 只有这一处使用, 不再使用函数
            if(NULL == ptr)                             //暂时分配不到内存, 该记录
            {
//	    		memcpy(gusCurMemErrFunName, "analyse_", 8);
	   			setmemgetnullid(GET_MEM_ERR30, gusCurMemErrFunName); 
                return __TRUE;
            }
            len = 2;                                    //单播回应OK
            memcpy(ptr + ZIGBEE_RET_ADDR_OFFSET, in_ptr + ZIGBEE_RET_ADDR_OFFSET, 8);
            ptr[DATA_OFFSET] = 'O';
            ptr[DATA_OFFSET + 1] = 'K';
            send_protocol_frame(ptr, ZIGBEE_CHL, len, MEM_256_TYPE);
            return __TRUE;
        }
    }
    else if(len == 2)                                   //读设备信息
    {
        if((in_ptr[DATA_OFFSET] == '/') && (in_ptr[DATA_OFFSET+1] == '?'))
        {
            send_identity_frame();                      //广播应答

            GangMarkTime = os_time;                     //记录接到/?时刻
            if(0 == GangMarkTime)                       //0我们用来表示没接到了 用1来代替
            {
                GangMarkTime = 1;
            }
            return __TRUE;                              //内存块在后续释放
        }
        if(0 != GangMarkTime)                           //之前有接/?到并在等待OK回应中
        {
            if(('O' == in_ptr[DATA_OFFSET]) && ('K' == in_ptr[DATA_OFFSET + 1]))
            {
                GangMarkTime = 0;                       //表示收到OK了
                return __TRUE;
            }
        }
    }

    return __FALSE;
}
/******************************************************************************
** 函数名称: U8 check_match_valid(void)
** 函数功能: 比对zigbee目标地址 如果是全0则每分钟匹配一次 非0则15分钟检查一次
** 入口参数: 只有发现表地址不对 才会去启动搜索 zigbee地址不对 不会启动 可以通过回应帧获取zigbee地址
** 返 回 值:
**
** 说    明:
******************************************************************************/
U8 check_match_valid (void)
{
    U8 i;

    for(i=0; i<PAR_HVMETER_ADR_LEN; i++)
    {
        if(gMatch_HVMeter_Addr[i] != 0)//因为上电时如果读出来不对则会初始化成全0的 并启动搜索表
        {
            break;
        }
    }
    if(i >= PAR_HVMETER_ADR_LEN)
    {
        return  __FALSE;//如果是全0 那就是已经启动搜索表 但还没有确定状态
    }
    if(gMatch_HVMeter_Addr[PAR_HVMETER_ADR_LEN] != addverify(gMatch_HVMeter_Addr, PAR_HVMETER_ADR_LEN))
    {
        get_match_addr();//重新从EEPROM中读取匹配地址 如果不对 还会启动搜索表功能
        return __FALSE;
    }

//    for(i=0; i<PAR_HVMETER_ZIGBEE_ADR_LEN; i++)
//    {
//        if(gMatch_Zigbee_Addr[i] != 0)//因为上电时的初始化成全0的
//        {
//            break;
//        }
//    }
//    if(i >= PAR_HVMETER_ZIGBEE_ADR_LEN)
//    {
//        return  __FALSE;//如果是全0 那就是已经启动搜索表 但还没有确定状态
//    }

    for(i=0; i<PAR_HVMETER_ZIGBEE_ADR_LEN; i++)
    {
        if(gMatch_Zigbee_Addr[i] != 0xff)//因为上电时的初始化成全0的
        {
            break;
        }
    }
    if(i >= PAR_HVMETER_ZIGBEE_ADR_LEN)
    {
        gucGetZigbeeAddrFlag = 1;//如果是全FF 如果前面表地址有效说明要获取回应帧中的zigbee地址
    }
    if(gMatch_Zigbee_Addr[PAR_HVMETER_ZIGBEE_ADR_LEN] != addverify(gMatch_Zigbee_Addr, PAR_HVMETER_ZIGBEE_ADR_LEN))
    {
        get_match_addr();//重新从EEPROM中读取匹配地址 如果不对 还会启动搜索表功能
        //return __FALSE;
        return __TRUE;//如果只是zigbee地址不对 则同步命令还得发 或能通过获取回应帧中的zigbee地址
    }

    return __TRUE;
}
/******************************************************************************
** 函数名称: void send_match_module_frame(void)
** 函数功能: 上电或约15分钟zigbee广播命令查询 地址配对的zigbee模块 配对高压表
** 入口参数:
** 返 回 值:
**
** 说    明:
******************************************************************************/
#ifdef LCD_VALID
void send_match_module_frame (void)
{
    U8  i;
    U8  mem_type;
    U8  *p_send;
    U8  *mem_ptr;

    mem_ptr = alloc_comm_mem(MEM_64_TYPE, &mem_type, GET_MEM_ERR24);
    if(NULL == mem_ptr)
    {
//	    memcpy(gusCurMemErrFunName, "send_mat", 8);
	   	setmemgetnullid(GET_MEM_ERR24, gusCurMemErrFunName); 
        return;
    }

    p_send = mem_ptr + DATA_OFFSET;

    i = 0;
    p_send[i++] = '/';
    p_send[i++] = '?';

    memset(&mem_ptr[ZIGBEE_RET_ADDR_OFFSET], 0xff, 8);//zigbee广播发查询其他设备信息
//    send_protocol_frame(mem_ptr, ZIGBEE_CHL, i, MEM_64_TYPE);
    send_wait_rev_frame(mem_ptr, ZIGBEE_CHL, i, MEM_64_TYPE, CON_TER_SYNFLASH);
}
#endif//LCD_VALID
/******************************************************************************
** 函数名称: void init_search_data(void)
** 函数功能: 上电初始化自动配置表地址相关变量
** 入口参数:
**
**
** 返 回 值:
** 说    明:
******************************************************************************/
void init_search_data (void)
{
#ifdef LCD_VALID//带zigbee模块，但不带显示，所又无法通过按键和显示屏搜高压表
    Dis_Search_Ctrl.state = CON_SEARCH_IDLE_STATE;
    Dis_Search_Ctrl.totnum = 0;
    Dis_Search_Ctrl.dis_sn = 0;
    Dis_Search_Ctrl.pre_key = CON_KEYBIT_NO;
    memset(Dis_Search_Ctrl.addr, 0, (CON_SEARCH_CMD_MAX*CON_SEARCH_ADDR_LEN));
#endif//LCD_VALID

    gucGetZigbeeAddrFlag = 0;

    if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
    {
        get_match_addr();
    }
}
/******************************************************************************
** 函数名称: void get_match_addr(void)
** 函数功能: 从EEPROM读地址 如果不对 则会启动搜索表功能 但不会确定表地址 还要手工配合确认
** 入口参数:
**
**
** 返 回 值:
** 说    明:
******************************************************************************/
void get_match_addr (void)
{
#ifdef ZIGBEE_TEST
    U8 crc,i;
//如果判断高压表地址或zigbee地址校验码不对 启动搜索表号
    file_read(PAR_HVMETER_ADR_ADDR, gMatch_HVMeter_Addr, PAR_HVMETER_ADR_LEN+1);
    crc = addverify(gMatch_HVMeter_Addr, PAR_HVMETER_ADR_LEN);
    if(crc != gMatch_HVMeter_Addr[PAR_HVMETER_ADR_LEN])
    {
        set_search_data();//只有高压表地址不对才会去搜索
        memset(gMatch_HVMeter_Addr, 0, 6);
    }

    file_read(PAR_HVMETER_ZIGBEE_ADR_ADDR, gMatch_Zigbee_Addr, PAR_HVMETER_ZIGBEE_ADR_LEN+1);
    crc = addverify(gMatch_Zigbee_Addr, PAR_HVMETER_ZIGBEE_ADR_LEN);
    #ifdef LCD_VALID
    if(crc != gMatch_Zigbee_Addr[PAR_HVMETER_ZIGBEE_ADR_LEN])
    {

//        set_search_data();
        gucGetZigbeeAddrFlag = 1;
        memset(gMatch_Zigbee_Addr, 0xff, 8);//用于第一帧zigbee广播获取地址
    }
    #else// 如果是电子式高压表只有在配置表地址时去搜索，这个zigbee地址校验码是对的，掉电再上电后还能去搜索
    if(crc == gMatch_Zigbee_Addr[PAR_HVMETER_ZIGBEE_ADR_LEN])
    {
        for(i=0; i<PAR_HVMETER_ZIGBEE_ADR_LEN; i++)
        {
            if(gMatch_Zigbee_Addr[i] != 0xff)//因为上电时的初始化成全0的
            {
                break;
            }
        }
        if(i >= PAR_HVMETER_ZIGBEE_ADR_LEN)
        {
            gucGetZigbeeAddrFlag = 1;//如果是全FF 如果前面表地址有效说明要获取回应帧中的zigbee地址
        }
    }
    #endif//LCD_VALID

#endif    
}
/******************************************************************************
** 函数名称: void set_search_data(void)
** 函数功能: 设置自动搜索表功能相关参数
** 入口参数:
**
**
** 返 回 值:
** 说    明:
******************************************************************************/
void set_search_data (void)
{
#ifdef LCD_VALID////azh 171011只有带显示的才要去初始化
//  if(CON_CPU_NORM_MODE == cpu_run_mode())//有电状态　带系统运行
//    {
        if(Dis_Ctrl.cur_mode == DIS_MODE_MATCH)
        {
            return;//已经启动搜索
        }
        Dis_Ctrl.cur_mode = DIS_MODE_MATCH;

        Dis_Search_Ctrl.state = CON_SEARCH_RUN_STATE;
        Dis_Search_Ctrl.state_dly = DIS_SEARCH_INTERVAL_TIME;
        Dis_Search_Ctrl.pre_key = CON_KEYBIT_NO;
        Dis_Search_Ctrl.cmd_ct = CON_SEARCH_CMD_MAX;
        Dis_Search_Ctrl.totnum = 0;

        Dis_Search_Ctrl.dis_sn = 0;
        memset(Dis_Search_Ctrl.addr, 0, (CON_SEARCH_CMD_MAX*CON_SEARCH_ADDR_LEN));
//    }
#endif//#ifdef LCD_VALID
}
/******************************************************************************
** 函数名称: void judge_search_state(void)
** 函数功能: 判断如果是编程键按下的情况 并且上下翻页键同时按下大于3秒 在100ms里调用
** 入口参数:
**
**
** 返 回 值:
** 说    明:
******************************************************************************/
#ifdef LCD_VALID
void judge_search_state (void)
{
    if(Dis_Ctrl.cur_mode != DIS_MODE_MATCH)
    {
#ifdef HVMETER_ADDR_NO_PROKEY
            if((Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP)) == (CON_KEYBIT_DN | CON_KEYBIT_UP))//同时按住上下翻页键
            {
                Dis_Search_Ctrl.state_dly++;
                if(Dis_Search_Ctrl.state_dly > DIS_SEARCH_JUDGE_TIME)
                {
                    set_search_data();
                }
            }
            else
            {
                Dis_Search_Ctrl.state_dly = 0;
            }
#else
        if(LOCAL_STATE_IS_HIG(CON_LOCAL_STATE_PROG))//编程键标志置起来了
        {
            if((Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP)) == (CON_KEYBIT_DN | CON_KEYBIT_UP))//同时按住上下翻页键
            {
                Dis_Search_Ctrl.state_dly++;
                if(Dis_Search_Ctrl.state_dly > DIS_SEARCH_JUDGE_TIME)
                {
                    set_search_data();
                }
            }
            else
            {
                Dis_Search_Ctrl.state_dly = 0;
            }
        }
        else
        {
            Dis_Search_Ctrl.state_dly = 0;
        }
#endif//HVMETER_ADDR_NO_PROKEY
    }
}
#endif//LCD_VALID
/******************************************************************************
** 函数名称: void store_match_addr(U8 index)
** 函数功能: 保存高压表的通信地址及其zigbee模块MAC地址
**
** 入口参数:
**
**
** 返 回 值:
** 说    明:
******************************************************************************/
void store_match_addr (U8 index)
{
    if(index < Dis_Search_Ctrl.totnum)
    {
        memcpy(gMatch_HVMeter_Addr, &Dis_Search_Ctrl.addr[index][0], PAR_HVMETER_ADR_LEN);
        gMatch_HVMeter_Addr[PAR_HVMETER_ADR_LEN] = addverify(gMatch_HVMeter_Addr, PAR_HVMETER_ADR_LEN);
        file_write(PAR_HVMETER_ADR_ADDR, gMatch_HVMeter_Addr, PAR_HVMETER_ADR_LEN+1);

        memcpy(gMatch_Zigbee_Addr, &Dis_Search_Ctrl.addr[index][PAR_HVMETER_ADR_LEN], PAR_HVMETER_ZIGBEE_ADR_LEN);
        gMatch_Zigbee_Addr[PAR_HVMETER_ZIGBEE_ADR_LEN] = addverify(gMatch_Zigbee_Addr, PAR_HVMETER_ZIGBEE_ADR_LEN);
        file_write(PAR_HVMETER_ZIGBEE_ADR_ADDR, gMatch_Zigbee_Addr, PAR_HVMETER_ZIGBEE_ADR_LEN+1);
    }
}

/******************************************************************************
** 函数名称: void store_zigbee_addr(U8 *addr)
** 函数功能: 地表地址已经有的情况下 保存高压表的zigbee模块MAC地址
**
** 入口参数:
**
**
** 返 回 值:
** 说    明:
******************************************************************************/
void store_zigbee_addr (U8 *addr)
{
    memcpy(gMatch_Zigbee_Addr, addr, PAR_HVMETER_ZIGBEE_ADR_LEN);
    gMatch_Zigbee_Addr[PAR_HVMETER_ZIGBEE_ADR_LEN] = addverify(gMatch_Zigbee_Addr, PAR_HVMETER_ZIGBEE_ADR_LEN);
    file_write(PAR_HVMETER_ZIGBEE_ADR_ADDR, gMatch_Zigbee_Addr, PAR_HVMETER_ZIGBEE_ADR_LEN+1);
}
/******************************************************************************
** 函数名称: void store_hvmeter_addr(U8 *addr)
** 函数功能: 通过通信口设置表地址 
**
** 入口参数:
**
**
** 返 回 值:
** 说    明:
******************************************************************************/
void store_hvmeter_addr (U8 *addr)
{
    memcpy(gMatch_HVMeter_Addr, addr, PAR_HVMETER_ADR_LEN);
    gMatch_HVMeter_Addr[PAR_HVMETER_ADR_LEN] = addverify(gMatch_HVMeter_Addr, PAR_HVMETER_ADR_LEN);
    file_write(PAR_HVMETER_ADR_ADDR, gMatch_HVMeter_Addr, PAR_HVMETER_ADR_LEN+1);
}
/******************************************************************************
** 函数名称: void handle_search_state(void)
** 函数功能: 处理搜索表各个状态的显示问题
**
** 入口参数:
**
**
** 返 回 值:
** 说    明:
******************************************************************************/
#ifdef LCD_VALID
void handle_search_state (void)
{
    switch(Dis_Search_Ctrl.state)
    {
        case CON_SEARCH_RUN_STATE://发命令: /? 状态
        {
            if(Dis_Search_Ctrl.state_dly > DIS_SEARCH_INTERVAL_TIME)
            {
                Dis_Search_Ctrl.state_dly = DIS_SEARCH_INTERVAL_TIME;
            }
            if(Dis_Search_Ctrl.state_dly > 0)
            {
                Dis_Search_Ctrl.state_dly--;
            }
            else if(Dis_Search_Ctrl.state_dly == 0)
            {
                if(Dis_Search_Ctrl.cmd_ct > CON_SEARCH_CMD_MAX)
                {
                    Dis_Search_Ctrl.cmd_ct = CON_SEARCH_CMD_MAX;
                }
                if(Dis_Search_Ctrl.cmd_ct > 0)
                {
                    send_match_module_frame();
                    Dis_Search_Ctrl.state_dly = DIS_SEARCH_INTERVAL_TIME;
                    Dis_Search_Ctrl.cmd_ct--;
                }
                else if(Dis_Search_Ctrl.cmd_ct == 0)
                {
                    Dis_Search_Ctrl.state = CON_SEARCH_DISPLY_STATE;
                    Dis_Search_Ctrl.state_dly = DIS_SEARCH_KEY_TIME;
                    Dis_Search_Ctrl.pre_key = CON_KEYBIT_NO;
                }
            }
            break;
        }
        case CON_SEARCH_DISPLY_STATE://命令发送结束 显示搜索到的表地址 可以按键显示 同时按下就是选择此表号
        {
            if((Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP)) == (CON_KEYBIT_DN | CON_KEYBIT_UP))//同时按下
            {
                if(Dis_Search_Ctrl.state_dly > DIS_SEARCH_KEY_TIME)
                {
                    Dis_Search_Ctrl.state_dly = DIS_SEARCH_KEY_TIME;
                }
                if(Dis_Search_Ctrl.state_dly > 0)
                {
                    Dis_Search_Ctrl.state_dly--;
                }
                else
                {
                    store_match_addr(Dis_Search_Ctrl.dis_sn);//保存高压表的通信地址及其zigbee模块MAC地址
                    Dis_Search_Ctrl.state = CON_SEARCH_FINISH_STATE;//上下按键要同时释放后
                    Dis_Search_Ctrl.state_dly = DIS_SEARCH_DIS_TIME;   
                    //azh 140919
			        RunPara_InitMeterPara(&stTerRunPara.stMeterPara[0]);
                    memcpy(stTerRunPara.stMeterPara[0].stMeterAddr.ucMeterAddr, gMatch_HVMeter_Addr, PAR_HVMETER_ADR_LEN);
                    file_write(F10_METERPARA_ADDR, (U8*)&stTerRunPara.stMeterPara[0], sizeof(STMETERPARA));	//国为是测量点0，所以地址没有偏移		        
                }
            }
            else
            {
                Dis_Search_Ctrl.state_dly = DIS_SEARCH_KEY_TIME;
                if((Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP)) == CON_KEYBIT_DN)
                {
                    Dis_Search_Ctrl.pre_key = CON_KEYBIT_DN;
                }
                else if((Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP)) == CON_KEYBIT_UP)
                {
                    Dis_Search_Ctrl.pre_key = CON_KEYBIT_UP;
                }
                else
                {
                    if(Dis_Search_Ctrl.pre_key == CON_KEYBIT_DN)
                    {
                        Dis_Search_Ctrl.dis_sn++;
                        if(Dis_Search_Ctrl.dis_sn >= Dis_Search_Ctrl.totnum)
                        {
                            Dis_Search_Ctrl.dis_sn = 0;
                        }
                        Dis_Ctrl.flash_flag |= BSN_CHG_SN_FLAG;
                    }
                    else if(Dis_Search_Ctrl.pre_key == CON_KEYBIT_UP)
                    {

                        if(Dis_Search_Ctrl.dis_sn > 0)
                        {
                            Dis_Search_Ctrl.dis_sn--;

                        }
                        else if(Dis_Search_Ctrl.dis_sn == 0)
                        {
                            Dis_Search_Ctrl.dis_sn = Dis_Search_Ctrl.totnum;
                            if(Dis_Search_Ctrl.totnum != 0)
                            {
                                Dis_Search_Ctrl.dis_sn--;
                            }
                        }
                        Dis_Ctrl.flash_flag |= BSN_CHG_SN_FLAG;
                    }
                    Dis_Search_Ctrl.pre_key = CON_KEYBIT_NO;
                }
            }
            break;
        }
        case CON_SEARCH_FINISH_STATE://保存表地址和及其他zigbee地址后 清相关状态 //上下按键要同时释放后
        {
            if((Key_CurState & (CON_KEYBIT_DN | CON_KEYBIT_UP)) != (CON_KEYBIT_DN | CON_KEYBIT_UP))
            {
                if(Dis_Search_Ctrl.state_dly > DIS_SEARCH_DIS_TIME)
                {
                    Dis_Search_Ctrl.state_dly = DIS_SEARCH_DIS_TIME;
                }
                if(Dis_Search_Ctrl.state_dly > 0)
                {
                    Dis_Search_Ctrl.state_dly--;
                }
                else
                {
                    init_search_data();

                    Dis_Ctrl.cur_mode = DIS_MODE_AUTO;
                    Dis_Ctrl.t_count = Dis_Para.dly[DIS_MODE_AUTO];
                    Dis_Ctrl.flash_flag |= MOD_CHG_SN_FLAG;
                    Dis_Ctrl.cur_diret = DN_SN_DIRECT;
                    Dis_Ctrl.pre_key = CON_KEYBIT_NO;
                }
            }
            break;
        }
        case CON_SEARCH_IDLE_STATE:
            break;
        default:
        {
            init_search_data();
            break;
        }
    }
}
#endif//LCD_VALID
#endif//ZIGBEE_TEST
/******************************************************************************
                            End Of File
 *****************************************************************************/

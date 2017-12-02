/******************************************************************************
 Copyright (C) 2010	 R&D Institute of HuaCai Co.,Ltd.
 File Name		: gprs_task.c
 Description	: GPRS模块任务管理
-------------------------------------------------------------------------------
 Modification History:
 <No.>	<version>		<time>		<author>		<contents>
   2、
   1、	  1.00		  2011-09-28	azh			 the original version
******************************************************************************/
#include	"..\inc\global_config.h"

//------------------------------ 私有变量声明 ---------------------------------

//------------------------------ 私有函数声明 -------------------------------

//void deubg_test_eon_flash(void)
//{
//    U8 buf[6],i;
//    U32 ulDataAddr=MXFLASH1_HEAD_ADDR;
//
////            FLS1_CS_EN();
////            FLS1_CS_DIS();
////
////            FLS2_CS_EN();
////            FLS2_CS_DIS();
////
////DF_WP_EN();
////DF_WP_DIS();
//
//   ulDataAddr = MXFLASH1_HEAD_ADDR+0x80000+2;
//   memset(buf,0x11,6);
//   file_read(ulDataAddr,buf,6); 
//   
//   mxflash_sector_erase(ulDataAddr);
//   memset(buf,0x11,6);
//   file_read(ulDataAddr,buf,6); 
//    
////    
//    ulDataAddr = MXFLASH1_HEAD_ADDR+0x80000-2;
//    memset(buf,0x11,6);
//    file_read(ulDataAddr,buf,6); 
//    
//    mxflash_sector_erase(ulDataAddr);
//    memset(buf,0x11,6);
//    file_read(ulDataAddr,buf,6); 
//
//    memset(buf,0x11,6);
//    for(i=0; i<6; i++)
//    {
//        buf[i] = 0xa0+i;
//    }
//    file_write(ulDataAddr,buf,6);
//    memset(buf,0x11,6);
//    file_read(ulDataAddr,buf,6); 
////
//    ulDataAddr = MXFLASH2_HEAD_ADDR+0x10000;
//    memset(buf,0x11,6);
//    file_read(ulDataAddr,buf,6); 
//    
//    mxflash_sector_erase(ulDataAddr);
//    memset(buf,0x11,6);
//    file_read(ulDataAddr,buf,6); 
//
//    memset(buf,0x11,6);
//    for(i=0; i<6; i++)
//    {
//        buf[i] = 0xa0+i;
//    }
//    file_write(ulDataAddr,buf,6);
//    memset(buf,0x11,6);
//    file_read(ulDataAddr,buf,6);     
//}

/******************************************************************************
 Function name:  __task void gprs_task(void)
 Author       :
 Description  : run per 100ms
 Input        :  None
 Return       :  None
 *****************************************************************************/
__task void gprs_task (void)
{
    U8 count=0;

	for(;;)
	{
       sign_task_schedule(GPRS_TASK_PRIO);

		//todo
		Mc55_Run();
        
        if(count > 5)
        {    
            if(gucPoweronGetTimeOver == 0xa5)//时间已经同步到
            {    
                Alarm_CopyMeter_Timer();            
    			Job_ProcData_Timer();					//数据定时上送
    			DataProc_Timer();						//数据处理管理
                Alarm_Manage();
    		}
            count = 0;
//
#if CODE_VER_TYPE==GPRS_MODULE_PRJ		//配合带CPU_GPRS模块
            if(PORT0_IS_HIGH(P0_PD_CHK))           //非掉电工况,闪烁运行灯
            {
                SET_PORT0_REV(P0_RUN_LED);
            }
#else
            if(PORT0_IS_HIGH(P0_PD_CHK))           //非掉电工况,闪烁运行灯
            {
                SET_PORT1_REV(P1_CPU_YGP);
            }
#endif
        }
        count++;        
		
		os_dly_wait(OS_DLY_100MS);
	}
}

/******************************************************************************
							End Of File
 *****************************************************************************/

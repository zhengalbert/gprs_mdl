/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 File Name      : Alarm_Timer_Response.h
 Description    : 定时刷新数据更新到全局数组
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   1、    1.00        2012-02-05    azh     the original version
******************************************************************************/
#ifndef _ALARM_TIMER_RESPONSE_H_
#define _ALARM_TIMER_RESPONSE_H_

__EXTERN STFEERATELIST BYTE_TO_FEERATELIST(unsigned char *pucByte);
__EXTERN unsigned char Is_FeeRateList_Equel(STFEERATELIST *pstFeeRateList1,STFEERATELIST *pstFeeRateList2);

#endif 

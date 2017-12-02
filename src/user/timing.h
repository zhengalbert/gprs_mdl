/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 Module         : timing function head file
 File Name      : timing.h
 Description    :
 Author         :
 Build Date     :
 others         :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2��
   1��     1.00       2009-06-30     azh       create
******************************************************************************/

#ifndef __TIMING_H
#define __TIMING_H
/*----------------------------- macro definition ----------------------------*/
#define     REPORT_CONTENT_BUF      1024

#define     VALID_SECOND_INDEX      (3600 * 24 * 365 * 100ul)

//�û�Ӧ�������궨��:��������ת�沿��
#define DATA_SAVE_TIME              15      //�ڴ�ת���������ʱ��,��λ:����
/*----------------------------- type definition -----------------------------*/


/*-------------------------- functions declaration --------------------------*/
__EXTERN    __task  void    timing_task (void);

//__EXTERN            void    reflash_system_RTC (void);
/*--------------------------- variable declaration --------------------------*/
//__EXTERN    U32         Current_Date;               //BCD��ʽ�ĵ�ǰ���ڣ� YYYY-MM-DD
//__EXTERN    U32         Current_Time;               //BCD��ʽ�ĵ�ǰʱ�䣺WW-HH-MM-SS
//__EXTERN    U32         Second_Index;               //�������������ƣ�

//__EXTERN    U32         Power_Up_Record_Delay;      //���ϵ翪ʼ����¼����¼�������ʱ
__EXTERN    U16         Min_Cnt;                    //���Ӽ�����

#endif//__TIMING_H
/******************************************************************************
                            End Of File
 *****************************************************************************/

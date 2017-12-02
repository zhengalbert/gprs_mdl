/******************************************************************************
 Copyright (C) 2010  R&D Institute of HuaCai Co.,Ltd.
 Module         : task manage function head file
 File Name      : task_manage.h
 Description    :
 Author         :
 Build Date     :
 others         :
 ------------------------------------------------------------------------------
 Modification History:
 <No.>  <version>       <time>      <author>        <contents>
   2¡¢
   1¡¢     1.00       2011-08-18     azh       		create
******************************************************************************/

#ifndef __TASK_MANAGE_H
#define __TASK_MANAGE_H
/*----------------------------- macro definition ----------------------------*/

/*----------------------------- type definition -----------------------------*/
typedef enum
{
    NULL_ERR,
    TASK_RUN_ERR,
    MEM_FREE_ERR,
    MEM_END_ERR,
    MEM_SIZE_ERR,
    REGISTER_ERR
}MANAGE_ERROR;
/*-------------------------- functions declaration --------------------------*/
__EXTERN    __task  void    system_manage_task (void);
__EXTERN    void sign_task_schedule(U8 prio);
/*--------------------------- variable declaration --------------------------*/

#endif//__SEARCH_H
/******************************************************************************
                            End Of File
 *****************************************************************************/

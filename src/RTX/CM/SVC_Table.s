;/*----------------------------------------------------------------------------
; *      RL-ARM - RTX
; *----------------------------------------------------------------------------
; *      Name:    SVC_TABLE.S
; *      Purpose: Pre-defined SVC Table for CORTEX M3
; *      Rev.:    V4.05
; *----------------------------------------------------------------------------
; *      This code is part of the RealView Run-Time Library.
; *      Copyright (c) 2004-2009 KEIL - An ARM Company. All rights reserved.
; *---------------------------------------------------------------------------*/


                AREA    SVC_TABLE, CODE, READONLY

                EXPORT  SVC_Count

SVC_Cnt         EQU    (SVC_End-SVC_Table)/4
SVC_Count       DCD     SVC_Cnt

; Import user SVC functions here.
               IMPORT  __SVC_1			;azh isr_set(U32 channel, U32 prio);
               IMPORT  __SVC_2			;azh isr_clr(U32 channel);
               IMPORT  __SVC_3			;azh power_mode_shift(void);
               IMPORT  __SVC_4			;azh disable_irq(void);
               IMPORT  __SVC_5			;azh enable_irq(void);
;               IMPORT  __SVC_6			;azh enable_user_int(void);
               IMPORT  __SVC_6			;azh U32 operation_at_admin(U8 id);               
               
                EXPORT  SVC_Table
SVC_Table
; Insert user SVC functions here. SVC 0 used by RTL Kernel.
               DCD     __SVC_1                 ; user SVC function
               DCD     __SVC_2                 ; user SVC function
               DCD     __SVC_3                 ; user SVC function 
               DCD     __SVC_4                 ; user SVC function     
               DCD     __SVC_5                 ; user SVC function                               
;               DCD     __SVC_6                 ; user SVC function        
               DCD     __SVC_6                 ; user SVC function  
SVC_End

                END

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

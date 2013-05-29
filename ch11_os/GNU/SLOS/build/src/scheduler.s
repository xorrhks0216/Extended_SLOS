@ ****************************************************************************
@ * Simple Little Operating System - SLOS
@ ****************************************************************************

@ ****************************************************************************
@ *
@ * Module     : scheduler.s
@ * Project	: SLOS
@ * Version	: 0.09
@ * Originator	: Andrew N. Sloss
@ * History    :
@ * 
@ * 16th July 2003 Andrew N. Sloss
@ * - added header information
@ *
@ ****************************************************************************

     .text
     .code 32
     .align 0
     .global PCB_CurrentTask
     .global PCB_PtrCurrentTask
     .global PCB_PtrNextTask
     .global PCB_Table
     .global PCB_TopOfIRQStack
     .global kernelScheduler

kernelScheduler:
/*
@ ---------------------------------------------------
@ Round Robin Scheduler
@ ---------------------------------------------------
*/
CurrentTask:

     LDR     r3,=PCB_CurrentTask
     LDR     r0,[r3]      
     LDR     r1,=PCB_Table
     LDR     r1,[r1,r0,LSL#2]    
     LDR     r2,=PCB_PtrCurrentTask
     STR     r1,[r2]    
/*
@ ** PCB_PtrCurrentTask - updated with the new address
*/
NextTask:
     ADD     r0,r0,#1  
     CMP     r0,#3
     MOVEQ   r0,#0   
     STR     r0,[r3]      
     LDR     r1,=PCB_Table
     LDR     r1,[r1,r0,LSL#2]    
     LDR     r0,=PCB_PtrNextTask
     STR     r1,[r0]     
/*
@ ** PCB_PtrCurrentTask   = current PCB
@ ** PCB_PtrNextTask	  = next PCB
@ ** PCB_CurrentTask	  = new TASK_ID

@ ------------------------------------------------------
@ Context Switch
@ ------------------------------------------------------
*/

handler_contextswitch:
     LDMFD   sp!,{r0-r3,r12,r14}    
     LDR     r13,=PCB_PtrCurrentTask
     LDR     r13,[r13]
     SUB     r13,r13,#60    
     STMIA   r13,{r0-r14}^  
     MRS     r0, SPSR
     STMDB   r13,{r0,r14}    
     LDR     r13,=PCB_PtrNextTask
     LDR     r13,[r13]
     SUB     r13,r13,#60  
     LDMDB   r13,{r0,r14}
     MSR     spsr_cxsf, r0
     LDMIA   r13,{r0-r14}^         
     LDR     r13,=PCB_TopOfIRQStack
     LDR     r13,[r13]
     MOVS    pc,r14
     .end

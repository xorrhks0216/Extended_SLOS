; ****************************************************************************
; * Simple Little Operating System - SLOS
; ****************************************************************************

; ****************************************************************************
; *
; * Module     : scheduler.s
; * Project	: SLOS
; * Version	: 0.09
; * Originator	: Andrew N. Sloss
; * History    :
; * 
; * 16th July 2003 Andrew N. Sloss
; * - added header information
; *
; ****************************************************************************

     IMPORT     PCB_CurrentTask
     IMPORT     PCB_PtrCurrentTask
     IMPORT     PCB_PtrNextTask
     IMPORT     PCB_Table
     IMPORT     PCB_TopOfIRQStack

     EXPORT     kernelScheduler

     AREA SCHEDULER,CODE,READONLY

kernelScheduler

; ---------------------------------------------------
; Round Robin Scheduler
; ---------------------------------------------------

CurrentTask

     LDR     r3,=PCB_CurrentTask
     LDR     r0,[r3]                    ; r0 = Current Task ID
     LDR     r1,=PCB_Table
     LDR     r1,[r1,r0,LSL#2]           ; r1 = address of current task
     LDR     r2,=PCB_PtrCurrentTask
     STR     r1,[r2]                    ; save current structure task
; ** PCB_PtrCurrentTask - updated with the new address
NextTask
     ADD     r0,r0,#1                   ; r0 = Current_Task+1
     CMP     r0,#3
     MOVEQ   r0,#0                      ; if r0==MaxTask then r0=0
     STR     r0,[r3]                    ; save new Task ID
     LDR     r1,=PCB_Table
     LDR     r1,[r1,r0,LSL#2]           ; r1 = new next task PCB address
     LDR     r0,=PCB_PtrNextTask
     STR     r1,[r0]                    ; save current structure task

; ** PCB_PtrCurrentTask   = current PCB
; ** PCB_PtrNextTask	  = next PCB
; ** PCB_CurrentTask	  = new TASK_ID

; ------------------------------------------------------
; Context Switch
; ------------------------------------------------------

handler_contextswitch

     LDMFD   sp!,{r0-r3,r12,r14}         ; restore remaining registers
     LDR     r13,=PCB_PtrCurrentTask
     LDR     r13,[r13]
     SUB     r13,r13,#60                 ; r13->current PCB
     STMIA   r13,{r0-r14}^               ; save User regs
     MRS     r0, SPSR
     STMDB   r13,{r0,r14}                ; save the rest
     LDR     r13,=PCB_PtrNextTask
     LDR     r13,[r13]
     SUB     r13,r13,#60                 ; r13->next PCB
     LDMDB   r13,{r0,r14}
     MSR     spsr_cxsf, r0
     LDMIA   r13,{r0-r14}^               ; load User regs
     LDR     r13,=PCB_TopOfIRQStack
     LDR     r13,[r13]                   ; reset IRQ stack
     MOVS    pc,r14                      ; return to next task
     END

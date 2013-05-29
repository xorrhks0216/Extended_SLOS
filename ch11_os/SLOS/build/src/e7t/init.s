; ****************************************************************************
; * Simple Little Operating System - SLOS
; ****************************************************************************

; ****************************************************************************
; *
; * Module     : init.s
; * OS         : SLOS
; * Version	: 0.09
; * Originator	: Andrew N. Sloss
; *
; * 14 July 2003 Andrew N. Sloss
; * - separated the pcbSetUp routine
; *
; ****************************************************************************

; ------------------------------------------------
; IMPORT/EXPORT key functions
; ------------------------------------------------

     EXPORT     coreInitialize

     IMPORT     PCB_PtrTask2
     IMPORT     C_EntryTask2
     IMPORT     PCB_PtrTask3
     IMPORT     C_EntryTask3
     IMPORT     bringUpInitFIQRegisters
     IMPORT     C_Entry
     IMPORT     PCB_CurrentTask
     IMPORT     pcbSetUp

; ------------------------------------------------
; Define useful constants 
; ------------------------------------------------

IRQ32md   EQU   0x12
SVC32md   EQU   0x13
SYS32md   EQU   0x1f
NoInt     EQU   0xc0

     AREA INITSLOS,CODE,READONLY

; ------------------------------------------------
; Start initializing the core parts 
; ------------------------------------------------

coreInitialize

; ------------------------------------------------
; Initialize low level debug 
; ------------------------------------------------

     BL	     bringUpInitFIQRegisters

; ------------------------------------------------
; Setup stacks for SVC,IRQ,SYS&USER 
; Mode = SVC 
; ------------------------------------------------

     MOV     sp,#0x80000           ; initializing sp to TopOfMemory
     MSR     CPSR_c,#NoInt|SYS32md
     MOV     sp,#0x40000           ; SYS/User Stack = 0x40000
     MSR     CPSR_c,#NoInt|IRQ32md
     MOV     sp,#0x9000            ; IRQ Stack = 0x9000
     MSR     CPSR_c,#NoInt|SVC32md

; ------------------------------------------------
; Setup Task Process Control Block (PCB).
; Mode = SVC
; ------------------------------------------------

     LDR     r0,=C_EntryTask2     ; addr of C_EntryTask2
     LDR     r1,=PCB_PtrTask2
     MOV     r2,#0x10000
     BL      pcbSetUp

     LDR     r0,=C_EntryTask3     ; addr of C_EntryTask3
     LDR     r1,=PCB_PtrTask3
     MOV     r2,#0x20000
     BL      pcbSetUp

; -- set the current ID to TASK1 ...........

     LDR     r0, =PCB_CurrentTask
     MOV     r1, #0
     STR     r1,[r0]              ; first task ID = 0
     LDR     lr,=C_Entry
     MOV     pc,lr                ; call C_Entry
     END

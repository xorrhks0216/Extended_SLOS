@ ****************************************************************************
@ * Simple Little Operating System - SLOS
@ ****************************************************************************

@ ****************************************************************************
@ *
@ * Module     : init.s
@ * OS         : SLOS
@ * Version	: 0.09
@ * Originator	: Andrew N. Sloss
@ *
@ * 14 July 2003 Andrew N. Sloss
@ * - separated the pcbSetUp routine
@ *
@ ****************************************************************************

     .text
     .code 32
     .align 0
     .global coreInitialize
     .extern C_Entry
     .extern C_EntryTask2
     .extern C_EntryTask3
     .extern PCB_PtrTask2
     .extern PCB_PtrTask3
     .extern PCB_CurrentTask
     .extern pcbSetUp

     .extern coreCallSWI

     .equ IRQ32md, 0x12
     .equ SVC32md, 0x13
     .equ SYS32md, 0x1f
     .equ NoInt, 0xc0
     SVC_STACK_START:
 	.word	0xa0300000
     SYS_STACK_START:
 	.word	0xa0400000
     IRQ_STACK_START:
 	.word	0xa0380000

coreInitialize:
     BL	     bringUpInitFIQRegisters

/*
@ ------------------------------------------------
@ Setup stacks for SVC,IRQ,SYS&USER 
@ Mode = SVC 
@ ------------------------------------------------
*/

     ldr sp, SVC_STACK_START
     MSR CPSR_c,#NoInt|SYS32md
     ldr sp, SYS_STACK_START
     MSR CPSR_c,#NoInt|IRQ32md
     ldr sp, IRQ_STACK_START
     MSR CPSR_c,#NoInt|SVC32md

/*
@ ------------------------------------------------
@ Setup Task Process Control Block (PCB).
@ Mode = SVC
@ ------------------------------------------------
*/
     LDR     r0,=C_EntryTask2    
     LDR     r1,=PCB_PtrTask2
     MOV     r2, #0x30000
     BL      pcbSetUp

     LDR     r0,=C_EntryTask3     
     LDR     r1,=PCB_PtrTask3
     MOV     r2, #0x20000
     BL      pcbSetUp

@ -- set the current ID to TASK1 ...........
     LDR     r0, =PCB_CurrentTask
     MOV     r1, #0
     STR     r1,[r0]              
     LDR     lr,=C_Entry
     MOV     pc,lr                
     .end

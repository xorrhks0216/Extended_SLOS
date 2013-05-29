; ****************************************************************************
; * Simple Little Operating System - SLOS
; ****************************************************************************

; ****************************************************************************
; *
; * Module     : pcb.s
; * Project	: SLOS
; * Version	: 0.09
; * Originator	: Andrew N. Sloss
; * History    :
; * 
; * 16th July 2003 Andrew N. Sloss
; * - added header information
; *
; ****************************************************************************

     EXPORT     PCB_CurrentTask
     EXPORT     PCB_Table
     EXPORT     PCB_TopOfIRQStack
     EXPORT     PCB_PtrCurrentTask
     EXPORT     PCB_PtrNextTask
     EXPORT     PCB_PtrTask1
     EXPORT     PCB_BottomTask1
     EXPORT     PCB_PtrTask2
     EXPORT     PCB_BottomTask2
     EXPORT     PCB_PtrTask3
     EXPORT     PCB_BottomTask3
     EXPORT     pcbSetUp

; -- pcbSetup ----------------------------------------------------------------
;
; Description : PCB setup 
;
; Parameters  : r0=<thread entry address>
;               r1=<PCB Address>
;               r2=<stack offset>
; Return      : setup PCB
; Notes       :
;

     AREA PCBSetUp,CODE,READWRITE 
pcbSetUp
     STR     r0,[r1,#-4]          ; PCB[-4]=C_TaskEntry<2>
     STR     r0,[r1,#-64]         ; PCB[-64]=C_TaskEntry<2>
     SUB     r0,sp,r2
     STR     r0,[r1,#-8]          ; PCB[-8]=sp-<stack offset>
     MOV     r0,#0x50             
     STR     r0,[r1,#-68]         ; PCB[-68]=iFt_User
     MOV     pc,lr

     AREA PCBTable,DATA,READWRITE
PCB_Table
     DCD     PCB_PtrTask1
     DCD     PCB_PtrTask2
     DCD     PCB_PtrTask3

     AREA PCBIRQ,DATA,READWRITE
PCB_TopOfIRQStack
     DCD     0x9000               ; real value

     AREA PCBPtrCURRENT,DATA,READWRITE
PCB_PtrCurrentTask
     DCD     0x1

     AREA PCBPtrNEXT,DATA,READWRITE
PCB_PtrNextTask
     DCD     0x1

     AREA PCBActive,DATA,READWRITE
PCB_CurrentTask
     DCD     0x1

     AREA PCB1,DATA,READWRITE
PCB_BottomTask1
     % 68
PCB_PtrTask1

     AREA PCB2,DATA,READWRITE
PCB_BottomTask2
     % 68
PCB_PtrTask2

     AREA PCB3,DATA,READWRITE
PCB_BottomTask3
     % 68
PCB_PtrTask3
     END

; *******************************************************
; * Simple Little Operating System - SLOS
; *******************************************************

; ****************************************************************************
; *
; * Module     : start.s
; * Project	: SLOS
; * Version	: 0.09
; * Originator	: Andrew N. Sloss
; * History    :
; * 
; * 16th July 2003 Andrew N. Sloss
; * - added header information
; *
; ****************************************************************************

; ****************************************************************************
; * IMPORT 
; ****************************************************************************

     IMPORT    coreInitialize
     IMPORT    coreUndefinedHandler
     IMPORT    coreSWIHandler
     IMPORT    corePrefetchAbortHandler
     IMPORT    coreDataAbortHandler
     IMPORT    coreReservedHandler
     IMPORT    coreIRQHandler
     IMPORT    coreFIQHandler
     IMPORT    PCB_Table

     AREA ENTRYSLOS,CODE,READONLY

; ****************************************************************************
; * CODE 
; ****************************************************************************

     ENTRY
 
; ----------------------------------------------------
; table offset .......... 
; 00 - Reset
; 04 - Undefined instructions
; 08 - SWI instructions
; 0C - prefetch abort
; 10 - Data abort
; 14 - Reserved
; 18 - IRQ interrupts
; 1C - FIQ interrupts
; ----------------------------------------------------

     LDR     pc,vectorReset
     LDR     pc,vectorUndefined
     LDR     pc,vectorSWI
     LDR     pc,vectorPrefetchAbort
     LDR     pc,vectorDataAbort
     LDR     pc,vectorReserved
     LDR     pc,vectorIRQ
     LDR     pc,vectorFIQ

; -- Useful address to the PCB tables ------------------ 

ptrPCBTable         DCD  PCB_Table

; -- Kernel Jump table ------------------------------ 

vectorReset         DCD     coreInitialize
vectorUndefined     DCD     coreUndefinedHandler
vectorSWI           DCD     coreSWIHandler
vectorPrefetchAbort DCD     corePrefetchAbortHandler
vectorDataAbort     DCD     coreDataAbortHandler
vectorReserved      DCD     coreReservedHandler
vectorIRQ           DCD     coreIRQHandler
vectorFIQ           DCD     coreFIQHandler

     END


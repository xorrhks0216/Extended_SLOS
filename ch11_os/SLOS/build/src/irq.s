; ****************************************************************************
; * Simple Little Operating System - SLOS
; ****************************************************************************

; ****************************************************************************
; *
; * Module     : irq.s
; * Project	: SLOS
; * Version	: 0.09
; * Originator	: Andrew N. Sloss
; * History    :
; * 
; * 16th July 2003 Andrew N. Sloss
; * - added header information
; *
; ****************************************************************************

     EXPORT     coreIRQHandler
     
     IMPORT     eventsIRQHandler

     AREA IRQSLOS,CODE,READONLY

coreIRQHandler

     B    eventsIRQHandler   ; branch IRQ handler

     END

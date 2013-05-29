; ****************************************************************************
; * Simple Little Operating System - SLOS
; ****************************************************************************

; ****************************************************************************
; *
; * Module     : fiq.s
; * Project	: SLOS
; * Version	: 0.09
; * Originator	: Andrew N. Sloss
; * History    :
; * 
; * 16th July 2003 Andrew N. Sloss
; * - added header information
; *
; ****************************************************************************

     EXPORT	coreFIQHandler

     AREA FIQSLOS,CODE,READONLY

coreFIQHandler
     B    coreFIQHandler      ; infinite loop

     END

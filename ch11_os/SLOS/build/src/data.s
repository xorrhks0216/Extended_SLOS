; ****************************************************************************
; * Simple Little Operating System - SLOS
; ****************************************************************************

; ****************************************************************************
; *
; * Module     : data.s
; * Project	: SLOS
; * Version	: 0.09
; * Originator	: Andrew N. Sloss
; * History    :
; * 
; * 16th July 2003 Andrew N. Sloss
; * - added header information
; *
; ****************************************************************************

     EXPORT	coreDataAbortHandler

     AREA DATAABTSLOS,CODE,READONLY
coreDataAbortHandler

     B    coreDataAbortHandler          ; infinite loop

     END

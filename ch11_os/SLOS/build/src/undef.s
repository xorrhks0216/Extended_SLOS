; ****************************************************************************
; * Simple Little Operating System - SLOS
; ****************************************************************************

; ****************************************************************************
; *
; * Module     : undef.s
; * Project	: SLOS
; * Version	: 0.09
; * Originator	: Andrew N. Sloss
; * History    :
; * 
; * 16th July 2003 Andrew N. Sloss
; * - added header information
; *
; ****************************************************************************

     EXPORT	coreUndefinedHandler

     AREA UNDEFSLOS,CODE,READONLY

coreUndefinedHandler

     B     coreUndefinedHandler    ; infinite loop
     END

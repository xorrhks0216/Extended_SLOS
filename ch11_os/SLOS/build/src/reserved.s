; ****************************************************************************
; * Simple Little Operating System - SLOS
; ****************************************************************************

; ****************************************************************************
; *
; * Module     : reserved.s
; * Project	: SLOS
; * Version	: 0.09
; * Originator	: Andrew N. Sloss
; * History    :
; * 
; * 16th July 2003 Andrew N. Sloss
; * - added header information
; *
; ****************************************************************************

     EXPORT	coreReservedHandler

     AREA RESERVEDSLOS,CODE,READONLY

coreReservedHandler
     B    coreReservedHandler          ; infinite loop

     END

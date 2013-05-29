; ****************************************************************************
; * Simple Little Operating System - SLOS
; ****************************************************************************

; ****************************************************************************
; *
; * Module     : prefetch.s
; * Project	: SLOS
; * Version	: 0.09
; * Originator	: Andrew N. Sloss
; * History    :
; * 
; * 16th July 2003 Andrew N. Sloss
; * - added header information
; *
; **************************************************************************** 

     EXPORT	corePrefetchAbortHandler

     AREA PREFECTABTSLOS,CODE,READONLY

corePrefetchAbortHandler

     B	corePrefetchAbortHandler	; infinite loop
     
     END

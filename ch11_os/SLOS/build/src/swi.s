; ****************************************************************************
; * Simple Little Operating System - SLOS
; ****************************************************************************

; ****************************************************************************
; *
; * Module     : swi.s
; * Project	: SLOS
; * Version	: 0.09
; * Originator	: Andrew N. Sloss
; * History    :
; * 
; * 16th July 2003 Andrew N. Sloss
; * - added header information
; *
; ****************************************************************************

     EXPORT    coreSWIHandler

; ****************************************************************************
; * IMPORT
; ****************************************************************************

     IMPORT     eventsSWIHandler

     AREA SWISLOS, CODE, READONLY

; ****************************************************************************
; * kernel swi handler event
; ****************************************************************************

coreSWIHandler

     STMFD   sp!,{r0-r12,lr}       ; Store registers
     LDR     r10,[lr,#-4]          ; Calculate address of SWI instruction
     BIC     r10,r10,#0xff000000   ; Mask off top 8 bits
     MOV     r1,sp			     ; second parameter to C routine...
                                   ; is pointer to register values.
     MRS     r2,SPSR               ; move the spsr into gp register
     STMFD   sp!,{r2}              ; store spsr onto the stack. 
                                   ; This is only really
                                   ; needed in case of nested SWI's
     BL      swi_jumptable         ; call C routine to handle SWI
     LDMFD   sp!,{r2}              ; restore spsr from stack into r2
     MSR     SPSR_cxsf,r2          ; and restore it into spsr.
     LDMFD   sp!,{r0-r12,pc}^      ; restore registers and return.

; ---------------------------------------------------------------
; SWI Jump Table
; r0 - swi Number
; r1 - point to the r0-r12
; ---------------------------------------------------------------

swi_jumptable

     MOV     r0,r10                ; move SWI number into r0
     B       eventsSWIHandler      ; calling c routine to handle swi call
     END

/*
 *  ____________________________________________________________________
 * 
 *  Copyright (c) 2002, Andrew N. Sloss, Chris Wright and Dominic Symes
 *  All rights reserved.
 *  ____________________________________________________________________
 * 
 *  NON-COMMERCIAL USE License
 *  
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met: 
 *  
 *  1. For NON-COMMERCIAL USE only.
 * 
 *  2. Redistributions of source code must retain the above copyright 
 *     notice, this list of conditions and the following disclaimer. 
 * 
 *  3. Redistributions in binary form must reproduce the above 
 *     copyright notice, this list of conditions and the following 
 *     disclaimer in the documentation and/or other materials provided 
 *     with the distribution. 
 * 
 *  4. All advertising materials mentioning features or use of this 
 *     software must display the following acknowledgement:
 * 
 *     This product includes software developed by Andrew N. Sloss,
 *     Chris Wright and Dominic Symes. 
 * 
 *   THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS ``AS IS'' AND ANY 
 *   EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 *   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
 *   PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE CONTRIBUTORS BE 
 *   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, 
 *   OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 *   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
 *   OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
 *   TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
 *   OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 *   OF SUCH DAMAGE. 
 * 
 *  If you have questions about this license or would like a different
 *  license please email :
 * 
 * 	andrew@sloss.net
 * 
 * 
 */
 
/*****************************************************************************
 * Simple Little Operating System - SLOS 
 *****************************************************************************/

/*****************************************************************************
 *
 * Module       : macros.h
 * Description  : brings in macro headers
 * OS           : SLOS
 * Platform     : e7t
 * History      : 
 *
 * 16th November 2001 Andrew N. Sloss
 * - added a macro header  
 *
 *****************************************************************************/

/* -- Interrupt controller ------------------------------------------------- */

#ifndef SYSCFG 
#define SYSCFG 0x03ff0000
#endif

#define IOPMOD ((volatile unsigned *)(SYSCFG+0x5000))
#define IOPCON ((volatile unsigned *)(SYSCFG+0x5004))

#ifndef IOPDATA
#define IOPDATA (SYSCFG+0x5008)
#endif

#define INTPND ((volatile unsigned *)(SYSCFG+0x4004))
#define INTMSK	((volatile unsigned *)(SYSCFG+0x4008))
#define INT_GLOBAL            (21)

#define INT_SW3_MASK          (1)

#define IO_ENABLE_INT0        (1<<4)
#define IO_ACTIVE_HIGH_INT0   (1<<3)
#define IO_RISING_EDGE_INT0   (1)

#define TMOD   ((volatile unsigned *)(SYSCFG+0x6000))  // timer mode register
#define TDATA0 ((volatile unsigned *)(SYSCFG+0x6004))
#define TCNT0  ((volatile unsigned *)(SYSCFG+0x600c))
 
#define ENABLE_TIMER0    (1)
#define DISABLE_TIMER0   (0)

#define TOGGLE_TIMER0    (1<<1)
#define INIT0_TIMER0     (1<<2)

/* -- General -------------------------------------------------------------- */

#define IRQVector        0x18



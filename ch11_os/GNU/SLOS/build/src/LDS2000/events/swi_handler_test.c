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
 * Module       : handler.c
 * Description  : provides the SWI handlers
 * Originator   : Andrew N. Sloss
 * History      :
 *	
 * November 4th 2001 Andrew N. Sloss
 * - corrected the header
 *
 * January 3rd 2002 Andrew N. Sloss
 * - added the bringup swi for trace and fatal error
 *
 * April 30th 2002 Andrew N. Sloss
 * - cleaned up for Integrator port
 *
 *****************************************************************************/

/*****************************************************************************
 * IMPORT
 *****************************************************************************/

#include "../../core/all.h"

/*****************************************************************************
 * MACROS
 *****************************************************************************/

/* Mode control bits .......................... */

#define SYSTEM 0x1f
#define SVC    0x13
#define USER   0x10

/* IRQ control bits ........................... */

#define IRQoN  0x40
#define IRQoFF 0xc0

/* Low-level FIQ debug ........................ */

void  modifyControlCPSR(unsigned int);
void  bringUpSetR8fiq(unsigned int);
void  bringUpSetR9fiq(unsigned int);
void  bringUpSetR10fiq(unsigned int);

extern unsigned int STATE;

/*****************************************************************************
 * ROUTINES
 *****************************************************************************/

/* -- events_swi_e7t_handler --------------------------------------------------
 *
 * Descriptions : handler the swi calls for the e7t
 * 
 * Parameters   : int swi_number - swi being called
 *              : SwiRegs *r	 - pointer to the SWI registers
 * Return       : CallBack pointer
 * Other        :
 *
 * This function should migrate to be generic
 *
 */
#define TEST1 100

CallBack eventsSWIHandler(int swi_number, SwiRegs *r) 
{
  switch (swi_number) 
  {
  case /* SWI */ SLOS:
    switch (r->r[0])
    {
    /* Test Number: 100
     * Description: test basic swi call
     *
     */
    case TEST1: /* 100: this test basic functionality */
    r->r[0] = 0x1001;
    r->r[1] = 0x2002;
    r->r[2] = 0x3003;
    break;
    }
  break;
  }
return reportOK();
}


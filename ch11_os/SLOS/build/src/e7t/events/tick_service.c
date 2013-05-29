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
 * Module      : tick_service.c
 * Description : Header File
 * Platform    : Evaluator7T
 * History     : 
 *
 * 2000-03-25 Andrew N. Sloss
 * - implemented a tick service
 *
 *****************************************************************************/


/*****************************************************************************
 * IMPORT
 *****************************************************************************/

#include "../headers/macros.h"

#include "../../headers/api_types.h"

/*****************************************************************************
 * MACRO'S
 *****************************************************************************/

/* none ... */
 
/*****************************************************************************
 * GLOBAL
 *****************************************************************************/

UINT countdown;

/*****************************************************************************
 * EXTERN's
 *****************************************************************************/

/* none... */

/*****************************************************************************
 * GLOBALS
 *****************************************************************************/

/* none... */

/*****************************************************************************
 * ROUTINES
 *****************************************************************************/

/* -- eventTickInit -----------------------------------------------------------
 *                                                            
 * Description  : Initialises the counter timer in milliseconds
 *
 * Parameters   : UINT msec - sets periodic timer in milliseconds
 * Return       : none...
 * Notes        : none...
 *                                                                             
 */
 
void eventTickInit (UINT msec)
{
/* ----------------------------------------------------------------------
 *
 * Initalize the Tick hardware on the Samsung part.
 *
 * ----------------------------------------------------------------------
 */

*TMOD = 0;
*INTPND = 0x00000000; 	// Clear pending interrupts .............

/* ----------------------------------------------------------------------
 *
 * Set the countdown value depending on msec.
 *
 * ----------------------------------------------------------------------
 */

  switch (msec)
  {
  case 2: /* fast ... */
  countdown = 0x000ffff0;
  break;
  default: /* slow ... */
  countdown = 0x00effff0;
  break;	
  }
}

/* -- eventTickService --------------------------------------------------------
 *
 * Description : interrupt service routine for timer0 interrupt.
 *
 * Parameters  : none...
 * Return      : none...
 * Notes       : 
 * 
 * timer interrupt everytime the counter reaches 0. To reset
 * the timer TDATA0 has to have a new initialization value.
 * Finally the last act is to unmask the timer interrupt on 
 * the Samsung KS3250C100.
 *
 */ 

void eventTickService(void) 
{ 

/* -- reset timer interrupt... */ 

*INTPND	= 1<<10;
*TDATA0	= countdown; 

/* -- unmask the interrupt source.... */
	
*(volatile int*)INTMSK &= ~((1<<INT_GLOBAL)|(1<<10)|(1<<0));
} 

/* -- eventTickStart ----------------------------------------------------------
 *
 * Description  : switches on the periodic tick event
 *
 * Parameters   : none...
 * Return       : none...
 * Notes        : none...
 *
 */  
 
void eventTickStart(void)
{	
*TDATA0   = countdown; /* load Counter Timer ... */
*TMOD     |= 0x1;      /* enable interval interrupt ... */
  
/* -- unmask the interrupt source.. */ 
  	
*(volatile int*)INTMSK &= ~((1 << INT_GLOBAL) | (1<<10) | (1<<0));     
}


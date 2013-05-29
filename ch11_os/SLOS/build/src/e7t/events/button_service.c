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
 * Module       : button_service.c
 * Description  : services a button interrupt
 * OS           : SLOS 0.09
 * Platform     : e7t
 * History      :
 *
 * December 7th 2001 Andrew N. Sloss
 * - created a button event example
 *
 * Note         :
 *
 * The eventButtonInit() has to be called before a button can be
 * called. This is achieved in the cinit.c 0latform_events%  
 *
 *****************************************************************************/

/*****************************************************************************
 * IMPORTS
 *****************************************************************************/

#include "../../headers/api_types.h"
#include "../../devices/ddf_io.h"
#include "../devices/ddf_types.h"

#include "../devices/led_driver.h"

#include "swis.h"
#include "../../headers/api_device.h"

#include "../headers/macros.h"

extern unsigned int STATE;

/*****************************************************************************
 * STATICS
 *****************************************************************************/

device_treestr *ledNode;
UID ledUID;

/*****************************************************************************
 * MACROS
 *****************************************************************************/

/* none... */

/*****************************************************************************
 * IMPORTS
 *****************************************************************************/

void eventButtonMaskIrq(unsigned irq) 
{
/* -----------------------------------------------------------
 *
 * Turn interrupt source off.
 *
 * -----------------------------------------------------------
 */
*(volatile int*)INTMSK |= (1<<irq);
}

void eventButtonUnmaskIrq(unsigned irq1,unsigned irq2) 
{

/* -----------------------------------------------------------
 *
 * Force global disable OFF. Turn interrupt source on.
 *
 * -----------------------------------------------------------
 */

*(volatile int*)INTMSK &= ~((1<<INT_GLOBAL)|(1<<irq1)|(1<<irq2));   
}

/* -- eventButtonInit ---------------------------------------------------
 *                                                            
 * Description  :  Initialises the button interrupt
 *
 * Parameters   : none...   
 * Return       : none...
 * Notes        : none...
 *                                                                             
 */

void eventButtonInit(void)
{	
/* -----------------------------------------------------------
 *
 * Bitwise OR incase an existing set of Interrupts have been 
 * set by a debug monitor
 *
 * -----------------------------------------------------------
 

eventButtonUnmaskIrq (10,0);	

 * -----------------------------------------------------------
 *
 * Enable INT0 set as Active high and allows Rising Edge.
 * 
 * -----------------------------------------------------------
 

*(volatile unsigned *)IOPCON 	= IO_ENABLE_INT0; 
*(volatile unsigned *)IOPCON	|= IO_ACTIVE_HIGH_INT0; 
*(volatile unsigned *)IOPCON	|= IO_RISING_EDGE_INT0;	

*/

 *(volatile int*)INTMSK &= ~((1 << INT_GLOBAL) | (1<<10) | (1<<0));

 *(volatile unsigned *)IOPCON   |= IO_ENABLE_INT0;       /* enable int0 */
 *(volatile unsigned *)IOPCON   |= IO_ACTIVE_HIGH_INT0;  /* active high */
 *(volatile unsigned *)IOPCON   |= IO_RISING_EDGE_INT0;  /* RISING EDGE */

/* -----------------------------------------------------------
 *
 * Open LED 1 Device
 * 
 * -----------------------------------------------------------
 */

ledNode = eventIODeviceOpen(&ledUID,DEVICE_LED_E7T,GREEN1);

}

/* -- eventButtonPressed ------------------------------------------------------
 *
 * Description  : services button interrupt button by toggling LED1
 *
 * Parameters   : none...
 * Return       : none...
 * Notes        : none...
 *
 */

void eventButtonPressed(void)
{
BYTE LED1;

/* -----------------------------------------------------------
 * 
 * Obtain the current status of LED 1
 *
 * -----------------------------------------------------------
 */

STATE = 1; // turn off interrupts

LED1 = eventIODeviceReadBit (ledNode,ledUID);

  if (LED1==1) 
  {
  /* -------------------------------------------
   *
   * Switch LED1 OFF
   * 
   * -------------------------------------------
   */

  eventIODeviceWriteBit (ledNode,ledUID,0); 
  }
  else 
  {
  /* -------------------------------------------
   *
   * Switch LED1 ON
   * 
   * -------------------------------------------
   */

  eventIODeviceWriteBit (ledNode,ledUID,1);
  }

STATE = 2; /* turn on interrupts */
}

/* -- eventButtonService -----------------------------------------------------
 *
 * Description  : handles the button press interrupt...
 *
 * Parameters   : none...
 * Return       : none...
 * Notes        : none...
 *
 */ 

void eventButtonService(void) 
{

/* --------------------------------------------
 *
 * Reset interrupt source
 *
 * --------------------------------------------
 */

*(volatile unsigned *) INTPND &= INT_SW3_MASK;

/* --------------------------------------------
 *
 * Service Button Interrupt
 *
 * --------------------------------------------
 */

eventButtonPressed();		/* Call toggle LED's... */	
}


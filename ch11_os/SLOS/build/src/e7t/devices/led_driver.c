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
 * Module      : led_driver.c
 * Description : device driver for led on the e7t
 * OS          : SLOS 0.09
 * Platform    : e7t
 * History     :
 *
 * 19th November 2001 Andrew N. Sloss
 * - started working on an example device driver
 *
 * December 2nd 2001 Andrew N. Sloss
 * - added uid generator to the LED driver
 *
 *****************************************************************************/

/*****************************************************************************
 * IMPORT
 *****************************************************************************/

#include "../../devices/ddf_frame.h"
#include "led_driver.h"

/*****************************************************************************
 * MACROS
 *****************************************************************************/

/* -- base register */

#ifndef SYSCFG
#define SYSCFG 0x03ff0000
#endif

/* -- GPIO */

#define IOPMOD      (volatile unsigned *)(SYSCFG + 0x5000)
#define IOPDATA     (volatile unsigned *)(SYSCFG + 0x5008)
#define LEDBANK     *IOPDATA		

/* -- set */

#define LED_4_ON    (LEDBANK=LEDBANK|0x00000010)
#define LED_3_ON    (LEDBANK=LEDBANK|0x00000020)
#define LED_2_ON    (LEDBANK=LEDBANK|0x00000040)
#define LED_1_ON    (LEDBANK=LEDBANK|0x00000080)

/* -- reset */

#define LED_4_OFF   (LEDBANK=LEDBANK&~0x00000010)
#define LED_3_OFF   (LEDBANK=LEDBANK&~0x00000020)
#define LED_2_OFF   (LEDBANK=LEDBANK&~0x00000040)
#define LED_1_OFF   (LEDBANK=LEDBANK&~0x00000080)

/*****************************************************************************
 * DATATYPES
 *****************************************************************************/

typedef struct 
{
unsigned char led[4]; 
unsigned char uid[4];
} internal_ledstr;

/*****************************************************************************
 * STATICS
 *****************************************************************************/

internal_ledstr		diodes;

/* -- led_init ----------------------------------------------------------------
 *
 * Description : initalize the LED device driver internal 
 *               data structures and hardware. Set all the
 *               LED's to be zero.
 * 
 * Parameters  : none...
 * Return      : none...
 * Notes       : none...
 *
 */

void led_init(void) 
{
*IOPMOD  = 0xf0;
*IOPDATA = 0x50;

/* switch off all the LED's ....................... */

LED_4_OFF;
LED_3_OFF;
LED_2_OFF;
LED_1_OFF;

/* initialize internal data structure ............. */

diodes.led [GREEN1] = OFF;
diodes.led [RED]    = OFF;
diodes.led [ORANGE] = OFF;
diodes.led [GREEN2] = OFF;

diodes.uid [GREEN1] = NONE;
diodes.uid [RED]    = NONE;
diodes.uid [ORANGE] = NONE;
diodes.uid [GREEN2] = NONE;

}

/* -- led_open ----------------------------------------------------------------
 *
 * Description : example open on the LED 
 * 
 * Parameters  : unsigned major = 55075 
 *             : unsigned minor = (0=GREEN1,1=RED,2=ORANGE,3=GREEN2)
 * Return      : UID -
 * Notes       : 
 *
 */

UID led_open(unsigned major,unsigned minor) 
{

  if (major==DEVICE_LED_E7T)
  {
    if (minor <4) 
    {
      if (diodes.uid[minor]==NONE)
      {
      diodes.uid[minor] = uid_generate();
      return diodes.uid[minor];      /* unique ID */
      }
      else
      {
      return DEVICE_IN_USE;	
      }
    } 
    else
    {
    return DEVICE_UNKNOWN;	
    }
  }

return DEVICE_NEXT;
}

/* -- led_close ---------------------------------------------------------------
 *
 * Description : example open on the LED 
 * 
 * Parameters  : UID id 
 * Return      : 
 *
 *   DEVICE_SUCCESS - successfully close the device
 *   DEVICE_UNKNOWN - couldn't identify the UID
 *
 * Notes       : 
 *
 */

int led_close(UID id) 
{
int x;

  for (x=0;x<=3;x++)
  {
    if (diodes.uid[x]==id) 
    {
    diodes.uid [x] = NONE;
    return DEVICE_SUCCESS;
    }
  }

return DEVICE_UNKNOWN;
}

/* -- led_write_bit -----------------------------------------------------------
 *
 * Description : write a particular bit to an LED 
 * 
 * Parameters  : UID id = 55075 + unit (0..3)
 *	     	: BYTE led_set - least significant bit is used
 * Return	     : none...
 *
 * Notes       : an example of a led write bit
 *
 */

void led_write_bit(UID id,BYTE led_set) 
{
int x;

  for (x=0;x<=3;x++)
  {
    if (diodes.uid[x]==id) 
    {
      if (diodes.uid[x]!=NONE)
      { 	
      led_set &= 1; /* bitwise AND */
     
      diodes.led [x] = led_set;

        if (led_set==ON) 
        {
          switch (x) 
          {
          case GREEN2 : LED_4_ON; break;
          case RED    : LED_2_ON; break;
          case ORANGE : LED_3_ON; break;
          case GREEN1 : LED_1_ON; break;
          }
        }
        else 
        {
          switch (x) 
          {
          case GREEN2 : LED_4_OFF; break;
          case RED    : LED_2_OFF; break;
          case ORANGE : LED_3_OFF; break;
          case GREEN1 : LED_1_OFF; break;
          }
        }
      }
    }
  }
}

/* -- led_read_bit ------------------------------------------------------------
 *
 * Description : read a particular bit value 
 * 
 * Parameters  : UID id = 55075 + unit (0..3)
 * Return      : value return error if 255
 *
 * Notes       : an example of a led read bit
 */

BYTE led_read_bit(UID id) 
{
int x;

  for (x=0;x<=3;x++)
  {
    if (diodes.uid[x]==id) 
    {
      if (diodes.uid[x]!=NONE)
      { 	     
      return diodes.led [x];
      }
    }
  }

/* 
 * error correction goes here...
 */

return 255;
}


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
#include "../headers/reg.h"

/*****************************************************************************
 * MACROS
 *****************************************************************************/

#define ACCESS_VALUE     0x5AA8
#define LEDPORT    	 0x08000008
#define LEDBANK    	 __REG8(LEDPORT)		

/* -- set */

#define LED_7_ON    (LEDBANK=LEDBANK|0x80)
#define LED_6_ON    (LEDBANK=LEDBANK|0x40)
#define LED_5_ON    (LEDBANK=LEDBANK|0x20)
#define LED_4_ON    (LEDBANK=LEDBANK|0x10)
#define LED_3_ON    (LEDBANK=LEDBANK|0x08)
#define LED_2_ON    (LEDBANK=LEDBANK|0x04)
#define LED_1_ON    (LEDBANK=LEDBANK|0x02)
#define LED_0_ON    (LEDBANK=LEDBANK|0x01)

/* -- reset */

#define LED_7_OFF   (LEDBANK=LEDBANK&~0x80)
#define LED_6_OFF   (LEDBANK=LEDBANK&~0x40)
#define LED_5_OFF   (LEDBANK=LEDBANK&~0x20)
#define LED_4_OFF   (LEDBANK=LEDBANK&~0x10)
#define LED_3_OFF   (LEDBANK=LEDBANK&~0x08)
#define LED_2_OFF   (LEDBANK=LEDBANK&~0x04)
#define LED_1_OFF   (LEDBANK=LEDBANK&~0x02)
#define LED_0_OFF   (LEDBANK=LEDBANK&~0x01)

/*****************************************************************************
 * DATATYPES
 *****************************************************************************/

typedef struct 
{
unsigned char led; 
unsigned char uid;
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
__REG(MEM_CTL_BASE+MSC1) = ACCESS_VALUE;
/* switch off all the LED's ....................... */
LED_7_OFF;
LED_6_OFF;
LED_5_OFF;
LED_4_OFF;
LED_3_OFF;
LED_2_OFF;
LED_1_OFF;
LED_0_OFF;

/* initialize internal data structure ............. */

diodes.led = OFF;
diodes.uid = NONE;

}

/* -- led_open ----------------------------------------------------------------
 *
 * Description : example open on the LED 
 * 
 * Parameters  : unsigned major = 55075 
 *             : unsigned minor = (0=LED0,1=LED1,2=LED2,3=LED3,...,7=LED7)
 * Return      : UID -
 * Notes       : 
 *
 */

UID led_open(unsigned int major,unsigned int minor) 
{

  if (major==DEVICE_LED_LDS2000)
  {
    if (minor < 2) 
    {
      if (diodes.uid==NONE)
      {
      diodes.uid = uid_generate();
      return diodes.uid;      /* unique ID */
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
    if (diodes.uid==id) 
    {
    diodes.uid = NONE;
    return DEVICE_SUCCESS;
    }
return DEVICE_UNKNOWN;
}

/* -- led_write_bit -----------------------------------------------------------
 *
 * Description : write a particular bit to an LED 
 * 
 * Parameters  : UID id = 55075 + unit (0..7)
 *	     	: BYTE led_set - least significant bit is used
 * Return	     : none...
 *
 * Notes       : an example of a led write bit
 *
 */

void led_write_byte(UID id,BYTE led_set) 
{

if ((diodes.uid==id)&&(diodes.uid!=NONE)) 
	LEDBANK=led_set;
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

BYTE led_read_byte(UID id) 
{
    if ((diodes.uid==id)&&(diodes.uid!=NONE)) 
      return diodes.led;
/* 
 * error correction goes here...
 */

return 255;
}


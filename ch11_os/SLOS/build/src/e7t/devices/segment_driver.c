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
 * Module       : segment_driver.c
 * Description  : low level device driver for 7-segment display 
 *                provided on the e7t
 * OS           : SLOS 0.09
 * Platform     : e7t
 * History      :
 *
 * 24th November 2001 Andrew N. Sloss
 * - converted segment driver to the Wright Device Driver Framework.
 *
 *****************************************************************************/

/*****************************************************************************
 * IMPORT
 *****************************************************************************/

#include "../../devices/ddf_frame.h"
#include "segment_driver.h"

#define DEVICE_SEGMENT_E7T  55090

/*****************************************************************************
 * MACROS
 *****************************************************************************/

/* Samsung KS32C50100 settings .... */

#define SYSCFG		0x03ff0000
#define IOPMOD          ((volatile unsigned *)(SYSCFG+0x5000))
#define IOPDATA         ((volatile unsigned *)(SYSCFG+0x5008))
#define	SEG_MASK	(0x1fc00)

/* define segments in terms of IO lines */

#define	SEG_A		(1<<10)
#define	SEG_B		(1<<11)
#define	SEG_C		(1<<12)
#define	SEG_D		(1<<13)
#define	SEG_E		(1<<14)
#define	SEG_F		(1<<16)
#define	SEG_G		(1<<15)

#define	DISP_0		(SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F)
#define	DISP_1		(SEG_B|SEG_C)
#define	DISP_2		(SEG_A|SEG_B|SEG_D|SEG_E|SEG_G)
#define	DISP_3		(SEG_A|SEG_B|SEG_C|SEG_D|SEG_G)
#define	DISP_4		(SEG_B|SEG_C|SEG_F|SEG_G)
#define	DISP_5		(SEG_A|SEG_C|SEG_D|SEG_F|SEG_G)
#define	DISP_6		(SEG_A|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G)
#define	DISP_7		(SEG_A|SEG_B|SEG_C)
#define	DISP_8		(SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G)
#define	DISP_9		(SEG_A|SEG_B|SEG_C|SEG_D|SEG_F|SEG_G)

#define	DISP_A		(SEG_A|SEG_B|SEG_C|SEG_E|SEG_F|SEG_G)
#define	DISP_B		(SEG_C|SEG_D|SEG_E|SEG_F|SEG_G)
#define	DISP_C		(SEG_A|SEG_D|SEG_E|SEG_F)
#define	DISP_D		(SEG_B|SEG_C|SEG_D|SEG_E|SEG_G)
#define	DISP_E		(SEG_A|SEG_D|SEG_E|SEG_F|SEG_G)
#define	DISP_F		(SEG_A|SEG_E|SEG_F|SEG_G)

#define ON		1
#define OFF		0

/*****************************************************************************
 * DATATYPES
 *****************************************************************************/

typedef struct {
	unsigned char seg; 
	int 	      uid;
} internal_segmentstr;

/*****************************************************************************
 * STATICS
 *****************************************************************************/

internal_segmentstr		display;

/* -- numeric_display for the 7 segment display.... */

static unsigned int numeric_display [16] = 
{
	DISP_0,
	DISP_1,
	DISP_2,
	DISP_3,
	DISP_4,
	DISP_5,
	DISP_6,
	DISP_7,
	DISP_8,
	DISP_9,
	DISP_A,
	DISP_B,
	DISP_C,
	DISP_D,
	DISP_E,
	DISP_F
};	

void segment_set (BYTE seg);

/* -- segment_init ------------------------------------------------------------
 *
 * Description  : initalize the segment device driver internal 
 *                data structures and hardware. Set segment's 
 *                to display zero.
 * 
 * Parameters   : none...
 * Return       : none...
 * Other        : none...
 *
 */

void segment_init (void)
{
/* initalize physical device ... */

*IOPMOD 	|= SEG_MASK;
*IOPDATA 	|= SEG_MASK;

/* setup internal structure ... */

display.uid	= NONE;
display.seg 	= 0; 
segment_set 	(0);

}

/* -- segment_open ------------------------------------------------------------
 *
 * Description : open the segment device driver 
 * 
 * Parameters  : unsigned major = DEVICE_SEGMENT_E7T 
 *             : unsigned ignored = anything
 * Return      : if (success) return DEVICE_SEGMENT_E7T 
 *                else if (alreadyopen) return DEVICE_IN_USE
 *                else if (unknown) return DEVICE_NEXT
 * Other       : 
 *
 */

UID segment_open(unsigned major,unsigned ignore) 
{

  if (major==DEVICE_SEGMENT_E7T)
  {
    if (display.uid==NONE)
    {
    display.uid = uid_generate ();
    return display.uid;      /* unique ID */
    }
    else
    {
    return DEVICE_IN_USE;	
    }
  } 

return DEVICE_NEXT;
}

/* -- segment_close -----------------------------------------------------------
 *
 * Description  : close open handle to segment display 
 * 
 * Parameters   : UID id 
 * Return       : 
 *   DEVICE_SUCCESS - successfully close the device
 *   DEVICE_UNKNOWN - couldn't identify the UID
 * Other        : 
 *
 */

int segment_close(UID id) 
{
  if (display.uid==id) 
  {
  display.uid = NONE;
  return DEVICE_SUCCESS;
  }

  return DEVICE_UNKNOWN;
}

/* -- segment_setdisplay ------------------------------------------------------
 * 
 * Description  : sets the display of the segment display
 *
 * Parameters   : int d - binary pattern
 * Return       : none...
 * Notes        : 
 *
 */

void segment_setdisplay(unsigned d)
{
*IOPDATA 	&= ~SEG_MASK;
*IOPDATA 	|= d;	
}

/* -- segment_set -------------------------------------------------------------
 * 
 * Description  : check whether number is in the range of 0x0 and
 *                0xf. If so then it set the segment display. 
 *
 * Parameters   : int seg - then new number to set the segment 
 *                display.
 * Return       : none...
 * Notes        : 
 *
 */

void segment_set(BYTE seg)
{
  if (seg <= 0xf ) 
    segment_setdisplay(numeric_display[seg]);

}

/* -- segment_write_byte ------------------------------------------------------
 *
 * Description  : write a particular bit to an LED 
 * 
 * Parameters   : UID id = 55075 + unit (0..3)
 *              : BYTE led_set - least significant bit is used
 * Return       : none...
 *
 * Other        : an example of a led write bit
 *
 */

void segment_write_byte(UID id,BYTE seg) 
{
  if (display.uid==id) 
  {
  display.seg = seg;
  segment_set (seg);	
  }
}

/* -- segment_read_byte -------------------------------------------------------
 *
 * Description  : read a particular bit value 
 * 
 * Parameters   : UID id = 55075 + unit (0..3)
 * Return       : value return error if 255
 *
 * Other        : an example of a led read bit
 */

BYTE  segment_read_byte(UID id) 
{
  if (display.uid==id) 
  {  
  return display.seg;
  }

/* 
 * error flag goes here...
 */

return 255;
}


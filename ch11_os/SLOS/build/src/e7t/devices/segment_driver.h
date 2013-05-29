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
 * Module      : segment_driver.h
 * Description : low level SEGMENT device driver
 * OS          : SLOS 0.09
 * Platform    : e7t
 * History     : 
 *
 * 25th November 2001 Andrew N. Sloss
 * - Example simple Device driver by Chris Wright's 
 *   specification
 *
 *****************************************************************************/

/* -- segment_init ------------------------------------------------------------
 *
 * Description : initalize the SEGMENT device driver internal 
 *               data structures and hardware. Set the SEGMENT 
 *               to display zero.
 * 
 * Parameters  : none...
 * Return      : none...
 * Notes       : none...
 *
 */

void segment_init(void);

/* -- segment_open ------------------------------------------------------------
 *
 * Description : open the segment display  
 * 
 * Parameters  : unsigned device = DEVICE_SEGMENT_E7T
 *             : unsigned ignored - for this device
 * Return      : UID -
 * Notes       : 
 *
 */

UID segment_open(unsigned device,unsigned ignored);

/* -- segment_close -----------------------------------------------------------
 *
 * Description : example open on the segment 
 * 
 * Parameters  : UID id = DEVICE_SEGMENT_E7T 
 * Return      : 
 *   DEVICE_SUCCESS - successfully close the device
 *   DEVICE_UNKNOWN - couldn't identify the UID
 * Notes       : 
 *
 */

int segment_close(UID id);

/* -- segment_write_byte ------------------------------------------------------
 *
 * Description : write a byte 
 * 
 * Parameters  : UID id
 *             : BYTE segment = 0-9 
 * Return      : none...
 *
 * Notes       : 
 */

void segment_write_byte(UID id,BYTE segment);

/* -- segment_read_byte -------------------------------------------------------
 *
 * Description : read a byte value 
 * 
 * Parameters  : UID id
 * Return      : BYTE value 0-9 
 *
 * Notes       : an example of a led read bit
 */

BYTE segment_read_byte(UID id);


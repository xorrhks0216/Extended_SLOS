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
 * Module      : serial_driver.c
 * Description : low level device driver for the serial device
 *               based on DDF v0.01. 
 * OS          : SLOS 0.09
 * Platform    : e7t
 * History     :
 *
 * 24th November 2001 Andrew N. Sloss
 * - create template to speed up driver development.
 *
 *****************************************************************************/

/*****************************************************************************
 * IMPORT
 *****************************************************************************/

#include "../../devices/ddf_frame.h"
#include "serial_driver.h"

#define DEVICE_SERIAL_E7T  55095

/*****************************************************************************
 * MACROS
 *****************************************************************************/

#define ON		1
#define OFF		0

#define SYSCFG		(0x03ff0000)
#define UART0_BASE	(SYSCFG + 0xD000)
#define UART1_BASE 	(SYSCFG + 0xE000)

/*
 * Serial settings.......................
 */
	
#define	ULCON	0x00
#define	UCON	     0x04
#define	USTAT	0x08
#define	UTXBUF	0x0C
#define	URXBUF	0x10
#define	UBRDIV	0x14

/*
 * Line control register bits............
 */
 
#define	ULCR8bits		(3)
#define	ULCRS1StopBit	(0)
#define	ULCRNoParity	(0)

/*
 * UART Control Register bits............
 */
 
#define 	UCRRxM	(1)
#define 	UCRRxSI	(1 << 2)
#define 	UCRTxM	(1 << 3)
#define 	UCRLPB	(1 << 7)

/*
 * UART Status Register bits
 */
 
#define USROverrun     	(1 << 0)
#define USRParity      	(1 << 1)
#define USRFraming     	(1 << 2)
#define USRBreak       	(1 << 3)
#define USRDTR           (1 << 4)
#define USRRxData      	(1 << 5) 
#define USRTxHoldEmpty 	(1 << 6)
#define USRTxEmpty     	(1 << 7)

 /* default baud rate value */
 
#define BAUD_9600	   (162 << 4)

/* UART registers are on word aligned, D8 */

/* UART primitives */

#define GET_STATUS(p)	(*(volatile unsigned  *)((p) + USTAT))
#define RX_DATA(s)     	((s) & USRRxData)
#define GET_CHAR(p)		(*(volatile unsigned  *)((p) + URXBUF))
#define TX_READY(s)    	((s) & USRTxHoldEmpty)
#define PUT_CHAR(p,c)  	(*(unsigned  *)((p) + UTXBUF) = (unsigned )(c))
		
#define COM1	(1)
#define COM0	(0)

/*****************************************************************************
 * DATATYPES
 *****************************************************************************/

typedef struct 
{
unsigned int baudrate[2]; /* baud rate of the serial port ............ */
unsigned int uid[2];      /* lock[0] = COM1 lock[1] = COM2 ........... */
} internal_serialstr;

/*****************************************************************************
 * STATICS
 *****************************************************************************/

internal_serialstr		node;

/* -- internalSerialE7TSetup --------------------------------------------------
 *
 * Description : internal serial port setup routine
 *
 * Parameters  : unsigned int port - UART address
 *             : unsigned int baud - baud rate
 * Return      : none...
 * Notes       :
 * 
 */

void internalSerialE7TSetup(unsigned int port, unsigned int baud)
{
/* Disable interrupts  */
*(volatile unsigned *) (port + UCON) = 0;

/* Set port for 8 bit, one stop, no parity  */
*(volatile unsigned *) (port + ULCON) = (ULCR8bits);

/* Enable interrupt operation on UART */
*(volatile unsigned *) (port + UCON) = UCRRxM | UCRTxM;

/* Set baud rate  */
*(volatile unsigned *) (port + UBRDIV) = baud;
}

/* -- serial_init -------------------------------------------------------------
 *
 * Description : initalize serial driver.
 * 
 * Parameters  : none...
 * Return      : none...
 * Notes       : none...
 *
 *  Initializes the node data structures
 */

void serial_init(void)
{
/* initalize physical device ... */

/* initialize COM0 ......................... */
internalSerialE7TSetup(UART0_BASE,BAUD_9600);

/* initialize COM1 ......................... */
internalSerialE7TSetup(UART1_BASE,BAUD_9600);

/* setup internal structure ... */

node.baudrate[0] = BAUD_9600;
node.baudrate[1] = BAUD_9600;
node.uid[COM0]   = NONE;
node.uid[COM1]   = NONE;
}

/* -- serial_open -------------------------------------------------------------
 *
 * Description : open the serial device driver
 * 
 * Parameters  : unsigned major - DEVICE_SEGMENT_E7T 
 *             : unsigned com - COM0 | COM1
 * Return      : if (success) return UART0_BASE or UART1_BASE 
 *                  else if (alreadyopen) return DEVICE_IN_USE
 *                  else if (unknown) return DEVICE_NEXT
 * Notes       : 
 *
 */

UID serial_open(unsigned major,unsigned com) 
{
  if (major==DEVICE_SERIAL_E7T)
  {
    if (com==COM0 || com==COM1)
    {
      if (node.uid[com]!=NONE) 
      {
      return DEVICE_IN_USE;
      } 
      else
      {
      node.uid[com] = uid_generate ();	
      return node.uid[com];  
      }
    } 
  }
    
return DEVICE_NEXT;
}

/* -- serial_close ------------------------------------------------------------
 *
 * Description : close serial device driver
 * 
 * Parameters  : UID id 
 * Return      : 
 *   DEVICE_SUCCESS - successfully close the device
 *   DEVICE_UNKNOWN - couldn't identify the UID
 * Notes       : 
 *
 */

int serial_close(UID id) 
{
  if (node.uid[COM0]==id) 
  { 	
  node.uid[COM0] = NONE;
  return DEVICE_SUCCESS;
  }

  if (node.uid[COM1]==id) 
  { 	
  node.uid[COM1] = NONE;
  return DEVICE_SUCCESS;
  }

return DEVICE_UNKNOWN;
}

/* -- serial_write_byte -------------------------------------------------------
 *
 * Description : waits for response from hardware and writes a 
 *               byte to the device 
 * 
 * Parameters  : UART port address
 *             : BYTE data
 * Return      : none...
 *
 * Notes       : an example of a led write bit
 *
 */

void internal_serial_write(unsigned int port,BYTE c)		
{
  while (TX_READY(GET_STATUS(port))==0){;}

PUT_CHAR(port,c);
}

/* -- serial_write_byte -------------------------------------------------------
 *
 * Description : write a byte to a serial device 
 * 
 * Parameters  : UID device
 *             : BYTE data
 * Return      : none...
 *
 * Notes       : an example of a led write bit
 *
 */

void serial_write_byte(UID id,BYTE data) 
{

  if (node.uid[COM0]==id) 
  {
  internal_serial_write (UART0_BASE,data);
  return;
  }

  if (node.uid[COM1]==id) 
  {
  internal_serial_write (UART1_BASE,data);
  return;
  }

}

/* -- internal_serial_read ----------------------------------------------------
 *
 * Description : getkey routine
 *
 * Parameters  : unsigned int port - port address
 * Return      : BYTE - character
 * Notes       : 
 *
 */	
	
BYTE internal_serial_read(unsigned int port)
{

  while ( (RX_DATA(GET_STATUS(port)))==0 ) {;}

return GET_CHAR(port);
}	

/* -- serial_read_byte --------------------------------------------------------
 *
 * Description : read a particular byte value
 * 
 * Parameters  : UID id - COM0 | COM1 
 * Return      : BYTE - read byte otherwise 255
 *
 * Notes       :
 */

BYTE  serial_read_byte(UID id) 
{
  if (node.uid[COM0]==id) 
  {
  return internal_serial_read (UART0_BASE);
  }
 
  if (node.uid[COM1]==id) 
  {
  return internal_serial_read (UART1_BASE);
  }

return 255;
}


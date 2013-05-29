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
 * Platform    : LDS2000
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
#include "../headers/reg.h"


#define DEVICE_SERIAL_LDS2000  55090

/*****************************************************************************
 * MACROS
 *****************************************************************************/

/*
 * Serial settings.......................
 */
 
/* UART registers are on word aligned, D8 */

/* UART primitives */
/*
//#define GET_STATUS(p)	(*(volatile unsigned  *)((p) + USTAT))
#define GET_STATUS(p)	(*(volatile unsigned  *)((p) + FFLSR))
//#define RX_DATA(s)     	((s) & USRRxData)
#define RX_DATA(s)     	((s) & 1)
//#define GET_CHAR(p)		(*(volatile unsigned  *)((p) + URXBUF))
#define GET_CHAR(p)		__REG8((p) + FFRBR))
//#define TX_READY(s)    	((s) & USRTxHoldEmpty)
#define TX_READY(s)    	((s) & 32)
//#define PUT_CHAR(p,c)  	(*(unsigned  *)((p) + UTXBUF) = (unsigned )(c))
#define PUT_CHAR(p,c)  	__REG8((p) + FFTHR)) = (unsigned )(c) & 0xFF)
*/		
#define COM1	(1)
#define COM0	(0)

/*****************************************************************************
 * DATATYPES
 *****************************************************************************/

typedef struct 
{
unsigned int baudrate; /* baud rate of the serial port ............ */
unsigned int uid;      /* lock[0] = COM1 lock[1] = COM2 ........... */
} internal_serialstr;

/*****************************************************************************
 * STATICS
 *****************************************************************************/

internal_serialstr		node;

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

	__REG(GPIO_BASE + GPDR1) &= 0xFFFFFFFB; //set bit 0 PD34(input)
	__REG(GPIO_BASE + GPDR1) |= 0x80; //set bit 1 PD39(output)
		
	__REG(GPIO_BASE + GAFR1_L) |= 0x10; //AF34 low bit set 1 ALT_FN_1
	__REG(GPIO_BASE + GAFR1_L) &= 0xFFFFFFDF;//AF34 high bit set 0 ALT_FN_1
				
	__REG(GPIO_BASE + GAFR1_L) &= 0xFFFFBFFF;//AF39 low bit set 0 ALT_FN_2
	__REG(GPIO_BASE + GAFR1_L) |= 0x8000;   //AF39 high bit set 1 ALT_FN_2

	__REG(CLOCK_MANAGER_BASE + CKEN) |= 0x40;//serial clock enable

	__REG(FFUART_BASE + FFLCR) = 0x3;   //WLS:data length bit

	__REG(FFUART_BASE + FFLCR) &= 0xFFFFFF7F; // DLAB set 0(USE IER)
	__REG(FFUART_BASE + FFIER) |= 0x40; //UART Unit enable

	__REG(FFUART_BASE + FFFCR) |= 0x7;  //USE RESETTF, RESETTRF, TRFIFO

	__REG(FFUART_BASE + FFLCR) |= 0x80; //DLAB set 1(USE DLR)
	__REG(FFUART_BASE + FFDLL) = 0x60;  //divisor = 96(baud rate=9600bps)
	__REG(FFUART_BASE + FFDLH) = 0x00;  //divisor = 96(baud rate=9600bps)

	__REG(FFUART_BASE + FFLCR) &= 0xFFFFFF7F; // DLAB set 0(USE RBR,THR)

	__REG(FFUART_BASE + FFLSR) = 0x0;
/* setup internal structure ... */

node.baudrate = 9600;
node.uid   = NONE;
}

/* -- serial_open -------------------------------------------------------------
 *
 * Description : open the serial device driver
 * 
 * Parameters  : unsigned major - DEVICE_SEGMENT_LDS2000 
 *             : unsigned com - COM0 
 * Return      : if (success) return UID 
 *                  else if (alreadyopen) return DEVICE_IN_USE
 *                  else if (unknown) return DEVICE_NEXT
 * Notes       : 
 *
 */

UID serial_open(unsigned int major,unsigned int com) 
{
  if (major==DEVICE_SERIAL_LDS2000)
  {
    if (com==COM0)
    {
      if (node.uid!=NONE) 
      {
      	return DEVICE_IN_USE;
      } 
      else
      {
      	node.uid = uid_generate ();	
     	return node.uid;  
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
  if (node.uid==id) 
  { 	
  node.uid = NONE;
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
	while((__REG32(port + FFLSR ) & 32 ) == 0);

	__REG32(port + FFTHR ) = ((u32)c & 0xFF);

//	if(c=='\n') internal_serial_write(port, '\r');
   return;
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

  if (node.uid==id) 
  {
  	//internal_serial_write (UART0_BASE,data);
  	internal_serial_write(FFUART_BASE, data);
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

	//wait until LSR bit 1(DR) is 1
	while((__REG32(port + FFLSR ) & 1 ) == 0);
	return __REG8(port + FFRBR);

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
  if (node.uid==id) 
  {
  //return internal_serial_read (UART0_BASE);
  return internal_serial_read (FFUART_BASE);
  }
 
return 255;
}


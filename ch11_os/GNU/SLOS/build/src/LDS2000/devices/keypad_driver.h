/*****************************************************************************

 * Simple Little Operating System - SLOS

 *****************************************************************************/



/*****************************************************************************

 * 

 * Module      : keypad_driver.h

 * Description : low level KEY PAD device driver

 * OS          : SLOS 0.09

 * Platform    : LDS2000

 * History     : 

 *

 * 19th November 2001 Andrew N. Sloss

 * - Example simple Device driver by Chris Wright's 

 *   specification

 *

 *****************************************************************************/

#define DEVICE_KEYPAD_LDS2000   55100


void keypad_init(void);

UID keypad_open(unsigned device,unsigned keypad);

int keypad_close(UID id);

void keypad_write_byte(UID id,BYTE keypad_set);

BYTE keypad_read_byte(UID id);





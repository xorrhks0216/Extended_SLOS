
/*****************************************************************************
 * Simple Little Operating System - SLOS
 *****************************************************************************/

/*****************************************************************************
 *
 * Module      : keypad_driver.c
 * Description : device driver for key pad on the LDS2000
 * OS          : SLOS 0.09
 * Platform    : LDS2000
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
#include "keypad_driver.h"
#include "../headers/reg.h"

/*****************************************************************************
 * MACROS
 *****************************************************************************/

#define ACCESS_VALUE     0x5AA8
#define KEYPADINPORT     0x0800000A
#define KEYPADOUTPORT	0x0800000C
#define KEYPADINBANK       __REG8(KEYPADINPORT)
#define KEYPADOUTBANK	__REG8(KEYPADOUTPORT)                

/*****************************************************************************
 * DATATYPES
 *****************************************************************************/
int tempValue=0;
typedef struct 
{
unsigned char num; 
unsigned char uid;
} internal_keypadstr;

/*****************************************************************************
 * STATICS
 *****************************************************************************/


internal_keypadstr		keypads;

/* -- keypad_init ----------------------------------------------------------------

 */



void keypad_init(void) 
{
//__REG(MEM_CTL_BASE+MSC1) = ACCESS_VALUE;
KEYPADINBANK=0;

keypads.num = 0;

keypads.uid = NONE;
}

/* -- keypad_open ----------------------------------------------------------------

 */



UID keypad_open(unsigned int major,unsigned int minor) 
{
  if (major==DEVICE_KEYPAD_LDS2000)
  {
    if (minor < 2) 
    {
      if (keypads.uid==NONE)
      {
      keypads.uid = uid_generate();
      return keypads.uid;      /* unique ID */
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

/* -- keypad_close ---------------------------------------------------------------

 */

int keypad_close(UID id) 
{
    if (keypads.uid==id) 
    {
    keypads.uid = NONE;
    return DEVICE_SUCCESS;
    }
return DEVICE_UNKNOWN;
}

/* -- keypad_write_byte -----------------------------------------------------------

 */

void keypad_write_byte(UID id,BYTE keypad_set) 
{

if ((keypads.uid==id)&&(keypads.uid!=NONE)) 
	tempValue=keypad_set&0x0F;
}

/* -- keypad_read_byte ------------------------------------------------------------


bit control mode 0 - data mode 1 - control
7	6	5	4	3	2	1	0
mode 0	'7'	'4'	'1'	'5'	'2'	'6'	'3'
mode 1						'#'

 */

BYTE keypad_read_byte(UID id) 
{
    int i;
    BYTE value=0,temp=0;
   if ((keypads.uid==id)&&(keypads.uid!=NONE)){ 
   for(i=0;i<3;i++){
      		keypad_write_byte(id, 1 << i);
      		tempValue = KEYPADOUTBANK&0x0F;
      		if((i==0||i==2)&&(temp&8==1)){
			keypads.num = (1<<7)|i;
			return keypads.num;
	        }
	        else{
			if(i==2)
				value |= temp;
			else
				value |= temp<<(1<<(2-i));
	        }
 	}
    }

	keypads.num=value;

	
        return keypads.num;
/* 
 * error correction goes here...
 */

return 255;
}



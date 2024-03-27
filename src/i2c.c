/*  2024-02-13 23:00  */
/*
yast - yet another slotcar timer
File: i2c -> i2c software interface for linux

Copyright (C)  2015 - 2024  Martin Berentsen


This file is part of yast.

yast is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

yast is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with yast.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include "yast_define.h"
#include "config.h"

// Define constants for MCP23017 register numbers. They'll be explained later.
#define IODIRA 0x00
#define IODIRB 0x01
#define GPIOA 0x12
#define GPIOB 0x13

// Define a constant for output and "low" status:
#define OUTPUT 0x00
#define ALL_OFF 0x00


static const char *i2cdevice = I2CDEVICENAME ;	// Filesystem path to access the I2C bus
uint8_t buffer[2];                       	// Initialize a buffer for two bytes to write to the device
int mcph[2];					// handle to the chips

static uint8_t gpioState[4] = { 0,0,0,0 }; // used to get the output status in memory

void yaSetMCP23017(int ioNumber, int val) {
  // This function sends bytes (received as arguments)
  // to the chips' GPIOA, GPIOB registers.
  /* this is not the fastest way, but works for beginning */
  /* now four times faster */

  if( (mcph[0] <= 0) | (mcph[1] <=0) ) /* This test isn't very good, but better then nothimg */
  {
    printf("mcp header not valid\n");
    return;
  }

  switch(val){
   case YA_HIGH:

    if(ioNumber < 8) {
     gpioState[0] = gpioState[0] | (0x01 << ioNumber); 
     buffer[0] = GPIOA;
     buffer[1] = gpioState[0];
     write(mcph[0], buffer, 2) ; //GPIOA set byte 1
     }
    if((ioNumber >= 8) & ( ioNumber < 16)) {
     gpioState[1] = gpioState[1] | (0x01 << (ioNumber-8)); 
     buffer[0] = GPIOB;
     buffer[1] = gpioState[1];
     write(mcph[0], buffer, 2) ; //GPIOB set byte 2
     }
    if((ioNumber >= 16) & ( ioNumber < 24)) {
     gpioState[2] = gpioState[2] | (0x01 << (ioNumber-16)); 
     buffer[0] = GPIOA;
     buffer[1] = gpioState[2];
     write(mcph[1], buffer, 2) ; //GPIOA set byte 3
     }
    if((ioNumber >= 24) & ( ioNumber < 32)) {
     gpioState[3] = gpioState[3] | (0x01 << (ioNumber-24)); 
     buffer[0] = GPIOB;
     buffer[1] = gpioState[3];
     write(mcph[1], buffer, 2) ; //GPIOB set byte 4
     }
    break;
  
   case YA_LOW:
    if(ioNumber < 8) {
     gpioState[0] = gpioState[0] & ~(0x01 << ioNumber); 
     buffer[0] = GPIOA;
     buffer[1] = gpioState[0];
     write(mcph[0], buffer, 2) ; //GPIOA set byte 1
     }
    if((ioNumber >= 8) & ( ioNumber < 16)) {
     gpioState[1] = gpioState[1] & ~(0x01 << (ioNumber-8)); 
     buffer[0] = GPIOB;
     buffer[1] = gpioState[1];
     write(mcph[0], buffer, 2) ; //GPIOB set byte 2
     }     
    if((ioNumber >= 16) & ( ioNumber < 24)) {
     gpioState[2] = gpioState[2] & ~(0x01 << (ioNumber-16)); 
     buffer[0] = GPIOA;
     buffer[1] = gpioState[2];
     write(mcph[1], buffer, 2) ; //GPIOA set byte 3
     }
    if((ioNumber >= 24) & ( ioNumber < 32)) {
     gpioState[3] = gpioState[3] & ~(0x01 << (ioNumber-24)); 
     buffer[0] = GPIOB;
     buffer[1] = gpioState[3];
     write(mcph[1], buffer, 2) ; //GPIOB set byte 4
     }     
    break;
    
    default:
     break;
}

  // Chip 0
//  buffer[0] = GPIOA;
//  buffer[1] = gpioState[0];
//  write(mcph[0], buffer, 2) ; //GPIOA set byte 1

//  buffer[0] = GPIOB;
//  buffer[1] = gpioState[1];
//  write(mcph[0], buffer, 2) ; //GPIOB set byte 2

  // Chip 1
//  buffer[0] = GPIOA;
//  buffer[1] = gpioState[2];
//  write(mcph[1], buffer, 2) ; //GPIOA set byte 3

//  buffer[0] = GPIOB;
//  buffer[1] = gpioState[3];
//  write(mcph[1], buffer, 2) ; //GPIOB set byte 4
}


/* 
  init of one or two MCP23017 

  returns opened bits as output
  or the negative number of failed device
  
*/

// Chip initialization:
// The system (and kernel in particular) must set up communication
// with the MCP23017 chips over the I2C bus. It's done by opening
// the device file (/dev/i2c-1) and calling the "ioctl" function.

// The chip must also "know" that it's used for providing outputs.
// All the registers used are specified in the chip's datasheet:
// startup here with MCP23017

int yaMCP23017Setup(int devcount)
{
int retval = 0;
int i;

if(devcount > 0)
{
// Initialize 1st MCP23017 with first address:
    mcph[0] = open(i2cdevice, O_RDWR);
    if (mcph[0] > 0) {
        ioctl(mcph[0], I2C_SLAVE, MC23017_ADDR_IC00);
        retval += 16;

  // First, we must set the I/O direction to output.
  // Write 0x00 to all IODIRA and IODIRB registers.

  // Chip 0
  buffer[0] = IODIRA;
  buffer[1] = OUTPUT;
  write(mcph[0], buffer, 2) ;  // set IODIRA to all outputs

  buffer[0] = IODIRB;
  buffer[1] = OUTPUT;
  write(mcph[0], buffer, 2) ;  // set IODIRB to all outputs


    } else {
        printf("MCP23017 #1 open error");
        return -1;
        }
}
    
if(devcount > 1)
{
// Initialize 2nd MCP23017 with second  address:
    mcph[1] = open(i2cdevice, O_RDWR);
    if (mcph[1] > 0) {
        ioctl(mcph[1], I2C_SLAVE, MC23017_ADDR_IC01);
        retval +=16;

  // Chip 1
  buffer[0] = IODIRA;
  buffer[1] = OUTPUT;
  write(mcph[1], buffer, 2) ;  // set IODIRA to all outputs

  buffer[0] = IODIRB;
  buffer[1] = OUTPUT;
  write(mcph[1], buffer, 2) ;  // set IODIRB to all outputs


    } else {
        printf("MCP23017 #2 open error");
        return -2;
        }          
  }

  // Chip 1
  buffer[0] = IODIRA;
  buffer[1] = OUTPUT;
  write(mcph[1], buffer, 2) ;  // set IODIRA to all outputs

  buffer[0] = IODIRB;
  buffer[1] = OUTPUT;
  write(mcph[1], buffer, 2) ;  // set IODIRB to all outputs

  for (i=0;i<4;i++)
    gpioState[i] = 0x00;

  return retval;  

}


int yaMCP23017Release(int devcount)
{
  int i;
  int retval;
  
  for(i=0;i<devcount;i++)
  {
    retval += close(mcph[i]);  
  }

return retval;
}


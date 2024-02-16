/*  2024-02-13 23:00  */
/*
yast - yet another slotcar timer
File: gpio.c -> Software for gpio interfacinfg with lgpio or WiringPi

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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>
#include <math.h>


#include "gpio.h"
#include "yast_define.h"
#include "config.h"

#ifdef WIRINGPI
#include <wiringPi.h>
#endif /* WIRINGPI */

#ifdef LGPIO
#include <lgpio.h>
#endif /* LGPIO */


extern int lghandle;		/* handle for lgpio lib */


/* uses delay or lguSleep, depending on gpio - lib used */
void yaSleep(double sleepsecs)
{
        #ifdef WIRINGPI
        int msecs = (int) lround(sleepsecs * 1000.0);
        delay(msecs);  /* wait for sound ready, can be done better */
        #endif
        #ifdef LGPIO
        lguSleep(sleepsecs);
        #endif		
}


/* digital write for a gpio_number to the given level */
/* LG_HIGH or HIGH is required */
int yaDigitalWrite(int gpio_num, int level)
{
        #ifdef WIRINGPI
        digitalWrite(gpio_num,level); 
        #endif /* WIRINGPI */

        #ifdef LGPIO
        int lgret;
        lgret = lgGpioClaimOutput(lghandle, 0 /*LFLAGS*/ , gpio_num, level);
        if (lgret < 0) 
        {
 	        printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
		yaGpioGetMode(lghandle,gpio_num); 
		return 1; }
        #endif /* LGPIO */
        return 0;
}


/*  returns as printf the state of the given GPIO value */
int DisplaylgMode(int val)
{
   printf(" GPIO Mode Meaning for %0x h:\n",val);

   if(val > (1<<19)) {
      printf("ERROR: unknown value\n");
      return -1;
   }

   if((val & 1) == 1)       printf("  Kernel: In use by the kernel\n");
   if((val & 2) == 2)       printf("  Kernel: Output\n");
   if((val & 4) == 4)       printf("  Kernel: Active low\n");
   if((val & 8) == 8)       printf("  Kernel: Open drain\n");
   if((val & 16) == 16)     printf("  Kernel: Open source\n");
   if((val & 32) == 32)     printf("  Kernel: Pull up set\n");
   if((val & 64) == 64)     printf("  Kernel: Pull down set\n");
   if((val & 128) == 128)   printf("  Kernel: Pull off set\n");
   if((val & 256) == 256)   printf("  LG: Input\n");
   if((val & 512) == 512)   printf("  LG: Output\n");
   if((val & 1024) == 1024) printf("  LG: Alert\n");
   if((val & 2048) == 2048) printf("  LG: Group\n");
   if((val & 4096) == 4096) printf("  LG: ---\n");
   if((val & 8192) == 8192) printf("  LG: ---\n");
   if((val & (1<<14)) == (1<<14)) printf("  LG: ---\n");
   if((val & (1<<15)) == (1<<15)) printf("  LG: ---\n");
   if((val & (1<<16)) == (1<<16)) printf("  Kernel: Input\n");
   if((val & (1<<17)) == (1<<17)) printf("  Kernel: Rising edge alert\n");
   if((val & (1<<18)) == (1<<18)) printf("  Kernel: Falling edge alert\n");
   if((val & (1<<19)) == (1<<19)) printf("  Kernel: Realtime clock alert\n");

   return 0;
}

# ifdef LGPIO

/*  returns as printf the state of the requested GPIO */
int yaGpioGetMode(int handle, int gpio)
{
   int gpio_mode;
   gpio_mode = lgGpioGetMode(handle,gpio);
   printf("GPIO %d Mod: %0xh\n",gpio,gpio_mode);

   DisplaylgMode(gpio_mode);

   return 0;
}

#endif /* LGPIO */
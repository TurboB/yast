/*  2018-12-23 21:00  */
/*
    yast - yet another slotcar timer
	File: display.c -> contains some screen routines of the project

    Copyright (C)  2016,2017,2018 Martin Berentsen


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
#include <stdarg.h>		/* ANSI-C stdarg.h */

#include "display.h"
#include "yast_define.h"
#include "config.h"

#ifdef OFFLINE
#include "curses.h"  	/* taken out of the ncurses5-dev package, win32 as pdcurses */
#endif
#ifndef OFFLINE
#include <curses.h>  	/* taken out of the ncurses5-dev package, win32 as pdcurses */
#endif

extern struct CONFIG config;	/* announce the config memory*/

/************************************************************************
  Function printd
  input identical to printf
  output to stderr only exists, if debug flag is set > Level 1
  ************************************************************************/
#ifdef _ANSI_STDARG_H_
void printd(const char *args,...)
{
  va_list ap;
  va_start(ap,args);
  if(config.debuglevel > 1) {
	fprintf(stderr,"DEBUG:");		/* Mark them in stderr */
    vfprintf (stderr,args,ap);
  }
  va_end (ap);
}
#endif /* _ANSI_STDARG_H_ */


/************************************************************************
  Function printmessage()
  input identical to printf
  output only goes to the 48 char area inside the yast race screen
  CUR_MESSAGE_LENGTH = 48
  output to stderr only exists, if debug flag is set > Level 0
************************************************************************/
#ifdef _ANSI_STDARG_H_
void printmessage(const char *args,...)
{
char messagebuffer[256];     /* critical, because can be overwritten*/

	int count = 0;
	va_list ap;
	va_start(ap,args);
	count = vsprintf(messagebuffer,args,ap);
	if(count > CUR_MESSAGE_LENGTH+10) {   											/* Emergency solution to prevent a buffer overrun */
		fprintf(stderr,"printmessage: string to long -%s-", messagebuffer );
		move(CUR_MESSAGE_LINE_Y,CUR_MESSAGE_LINE_X);  /* (y,x) */
		printw("[%.50s]",messagebuffer);  /*  50 chars maximum */
		return; 
	}
	va_end (ap);

  if(config.debuglevel > 0) {
		fprintf(stderr,"MESSAGE:");		/* Mark them in stderr */
		fprintf(stderr,"%s\n",messagebuffer);  /* an LF is added to messages and errors */
	}
	
	if(count < CUR_MESSAGE_LENGTH)  /* fill up with blank */
	{
		strcat(messagebuffer, "                                                ");  /* add 48 spaces */
	}	
	
	move(CUR_MESSAGE_LINE_Y,CUR_MESSAGE_LINE_X);  /* (y,x) */
	printw("[%.50s]",messagebuffer);  /*  50 chars maximum */
}
#endif /* _ANSI_STDARG_H_ */


/************************************************************************
Clear all results/last times on the right side of the screen
input: nout number of tracks in use
************************************************************************/
void ClearResults(int nout)
{
  int i,j;
  for(i=0; i<nout;i++) {  /* TRACKLIM */   
    for(j=1;j<=FONTHEIGHT;j++) {
      move(j+(i*FONTHEIGHT),CUR_PLACE_X - 2);
      printw("              ");
    }
  }
}
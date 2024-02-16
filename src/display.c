/*  2024-02-10 23:00  */
/*
    yast - yet another slotcar timer
	File: display.c -> contains some screen routines of the project

    Copyright (C)  2016 - 2024 Martin Berentsen


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

// #ifdef OFFLINE
// #include "curses.h"  	/* taken out of the ncurses5-dev package, win32 as pdcurses */
// #endif /* OFFLINE */
// #ifndef OFFLINE
#include <curses.h>  	/* taken out of the ncurses5-dev package, win32 as pdcurses */
// #endif /* OFFLINE */

extern struct CONFIG config;	/* announce the config memory*/

extern unsigned long overallfastest[TRACKLIM]; /* this must be done better later */
extern unsigned long lapcount[TRACKLIM];		
extern unsigned long laptime[TRACKLIM][LAPLIM]; 
extern char *trackdriversname[TRACKLIM];	


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

/* -------------------------------------------
ncurses printout overall timing for all known tracks
input:  nout Number of used tracks
---------------------------------------------- */
void view_overall(int nout)
{
	int i;
	for(i=0;i<nout /*TRACKLIM */ ;i++)
	{
		move(FONTHEIGHT+(i*FONTHEIGHT),CURSES_OVERALLFASTEST_LINE_X );
		printw("Record: %.3f     ",overallfastest[i]/1000.0);
	}
}

/* -------------------------------------------
ncurses printout the last three track times 
input:  nout Number of used tracks
---------------------------------------------- */
void view_last(int nout)
{
	int i,j;
	for(i=0;i<nout /*TRACKLIM */ ;i++)
	{
	switch(lapcount[i]) {
	
	case 0:
		break;
	case 1:
		break;
	case 2:
		for(j=1;j<=1;j++) {
			move((i*FONTHEIGHT)+j+1, CUR_PLACE_X );
			printw("%d: %.3f    ",lapcount[i]-j,laptime[i][(lapcount[i]-j-1)] / 1000.0);
		}		
		break;
	case 3:
		for(j=1;j<=2;j++) {
			move((i*FONTHEIGHT)+j+1, CUR_PLACE_X );
			printw("%d: %.3f    ",lapcount[i]-j,laptime[i][(lapcount[i]-j-1)] / 1000.0);
		}
		break;
	default:
		for(j=1;j<=3;j++) {
			move((i*FONTHEIGHT)+j+1, CUR_PLACE_X );
			printw("%d: %.3f    ",lapcount[i]-j,laptime[i][(lapcount[i]-j-1)] / 1000.0);
		}
		break;		
	}
	
	} /* for(i) */	
}

/* -------------------------------------------
ncurses clear the last three track times 
input:  nout Number of track to clear
---------------------------------------------- */
void clear_last(int nout)
{
	int j;

	if( (nout >= 0) && (nout <=3) ) {

		for(j=1;j<=3;j++) {
			move((nout*FONTHEIGHT)+j+1, CUR_PLACE_X );
			printw("          ");
		}
	}

}
/* ---------------------------------------------
printout driver names with respect to the tracks
 - max. 13 chars, please check in config at start
input:  nout Number of used tracks
------------------------------------------------ */
void view_drivernames(int nout)
{
	int i;
	for(i=0;i<nout /*TRACKLIM */ ;i++)
	{
/*	if( (trackdriversname[i] != NULL) && (strlen(trackdriversname[i]) < 14) ){ */
	if(trackdriversname[i] != NULL) {
		move(FONTHEIGHT+(i*FONTHEIGHT),CURSES_DRIVERSNAME_LINE_X );
		printw("Driver: %s",trackdriversname[i]);
		}
	}
}

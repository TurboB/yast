/*  2024-03-29 14:00  */
/*
yast - yet another slotcar timer
File: yast.c -> main c source

Copyright (C)  2015 - 2024 Martin Berentsen

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

/* #define VERSION "0.2.0" */ 			/*  out of use since 0.2.0, now in Makefile */
/* #define OFFLINE 1 */				/*  run without Pi, but define in makefile for Win32 */
/* #define MCP23017 */				/*  Add the MCP23017 hardware to the project */
/* #define SN3218 */				/*  Add the SN3218 hardware to the project */
/* #define DISPLAYTEST 1 */			/*  Make some test on the beginning (DEBUG ONLY) */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>
#include <sys/time.h>
#include <locale.h>

// #ifdef OFFLINE
// #include "curses.h"  	/* taken out of the ncurses5-dev package, win32 as pdcurses */
// #endif

// #ifndef OFFLINE

#ifdef  WIRINGPI
#include <wiringPi.h>  	/* header from wiringPi package */
#endif

#ifdef LGPIO
#include <lgpio.h>	/* header for lgpio lib package */
#endif

#include <curses.h>  	/* taken out of the ncurses5-dev or ncursesw5-dev package, win32 as pdcurses */
#include <signal.h>

// #endif /* OFFLINE */

#ifdef MCP23008
#include <mcp23008.h>  	/* header from wiringPi package */
#endif
#ifdef MCP23017
#include <mcp23017.h>  	/* header from wiringPi package */
#endif
#ifdef MCP23017EASY
#include <mcp23017.h>  	/* header from wiringPi package */
#endif
#ifdef SN3218
#include <sn3218.h>   	/* header from wiringPi package */
#endif

/* include yast own files */

#include "yast_define.h"
#include "trafficLight.h"
#include "font_3x.h"
#include "config.h"
#include "files.h"
#include "display.h"
#include "sound.h"
#include "gpio.h"
#include "i2c.h"


#define RESETALLTRACKS          \
for(j=0;j<TRACKLIM;j++) {       \
	for(i=0; i < LAPLIM;i++)  { \
		laptime[j][i] = 0 ;     \
	}                           \
	lapcount[j] = 0;            \
	fasttime[j] = MAXLAPTIME;   \
	lasttime[j] = 0;            \
	firsttime[j] = 0;           \
	secondtime[j] = 0;          \
	finishtime[j] = 0;          \
	overalltime[j]= 0;          \
	finishlap[j] = LAPLIM;      \
	reactiontime[j] = 0;        \
	carsin = 0;                 \
}

struct timeval t1_light;  /* Time variable for time measuring, please think about ntp running in parallel */

struct CONFIG config;  /* set up the space for run time configuration */

/* make it global to support ISR routines */
unsigned long lapcount[TRACKLIM];				/*  lap counter for every track */
unsigned long laptime[TRACKLIM][LAPLIM];		/*  Array for the time im ms for finished laps */
unsigned long fasttime[TRACKLIM];				/* store here the fastest lap of a race or training */
unsigned long lasttime[TRACKLIM];				/* store here the last lap; */
unsigned long finishtime[TRACKLIM];				/* complete race time from start to last track event or race end */
unsigned long overalltime[TRACKLIM];			/* latest known overall time in race */
unsigned long overallfastest[TRACKLIM];			/* storage of the fastest track time over all races and trainings */
unsigned long reactiontime[TRACKLIM];			/* starting reaction time is stored here */
char *trackdriversname[TRACKLIM];				/* drivers name on this track */
int position[TRACKLIM];					/* position in race */
unsigned long starttime = 0;					/* holds the rough ms time from race startup to now */
int screenupdate = 0;							/* is there something to do for the display redraw */
unsigned long panictime = 0;					/* holds the exact point time, where the PANIC button was pressed */
												/* if zero, no panic, if set to time, panic button was pressed */
unsigned long overallpanictime = 0;				/* store all panic times added here */
unsigned int numberofpanics = 0;				/* view the number of panic button pressed while race at the end */
int panictimestop = 0;							/* =1 prepare for panic, =2 holds the time while panic, with =0 the time moves on */

unsigned long ISR_Signal_Falgs = 0;				/* bring out some messages out of the ISR */

unsigned long firsttime[TRACKLIM];  			/* track start time stamp (for a single round ) */
unsigned long secondtime[TRACKLIM]; 			/* track end time stamp  (for a single round )*/

unsigned char trackcurrent = 0;					/*  Track supply on (1) and off (0) (just a marker) */
unsigned long supplyofftime[TRACKLIM] = {0,0,0,0};		/*  Track supply on (0) or off > 0 in timestamp */
unsigned char timingactive[TRACKLIM] = {0,0,0,0};			/*  Timing on (1) and off (0) */
int stop = 0;									/* used to exit the core, it's the only way */
short snd_buffer[SND_NUMBER_OF_TONES] [SND_BUFFER_SIZE];	/* define default the tone arrays */
unsigned int snd_buffer_len[SND_NUMBER_OF_TONES]; 			/* define length inside tones */
struct tm *timeinfo;	/* timeinfo and rawtime used for RTC view and file output */
time_t rawtime;			/* timeinfo and rawtime used for RTC view and file output */

#ifdef LGPIO
int lghandle;		/* handle for lgpio lib */
int lgret;		/* int return var for lib functions */
lgChipInfo_t cInfo;	/* A pointer to a lgChipInfo_t object  */
static int userdata_1=1; /* dummy user data for ISR */
static int userdata_2=2;
static int userdata_3=3;
static int userdata_4=4;
#endif


/* ---------------------------------------------------------------------- */
/*   return time im ms as long */
/* ---------------------------------------------------------------------- */
unsigned long GetTime(void){
	gettimeofday(&t1_light, NULL);
	return (unsigned long) t1_light.tv_sec  *  (unsigned long) 1000  + (unsigned long) ( t1_light.tv_usec / 1000) ;
}

/* ----------------------------------------------------------------------
The functions will be called when the interrupt triggers.
------------------------------------------------------------------------*/

#ifdef WIRINGPI
void Track_1_ISR(void) {
#endif /* WIRINGPI */

#ifdef LGPIO
void Track_1_ISR(int e, lgGpioAlert_p evt, void *data)
{
//   int i;
//   int userdata = *(int*)data;
//   for (i=0; i<e; i++)
//   {
//      printf("u=%d t=%"PRIu64" c=%d g=%d l=%d f=%d (%d of %d)\n",
//         userdata, evt[i].report.timestamp, evt[i].report.chip,
//         evt[i].report.gpio, evt[i].report.level,
//         evt[i].report.flags, i+1, e);
//   }
#endif /* LGPIO */

	if(timingactive[0]){
		secondtime[0] = GetTime();
		if(firsttime[0] > 0) {
			if( secondtime[0] - firsttime[0] >= config.minlaptime ) {  /*  minimum time check */
				laptime[0][lapcount[0]++] = secondtime[0] - firsttime[0];
				lasttime[0] = laptime[0][lapcount[0]-1];
				if(fasttime[0] > lasttime[0]) {
					fasttime[0] = lasttime[0];
					if(overallfastest[0] > fasttime[0])
						overallfastest[0] = fasttime[0];
				}
				firsttime[0] = secondtime[0];
				overalltime[0] = secondtime[0] - starttime;
			} else {
				ISR_Signal_Falgs = ISR_Signal_Falgs | TRACKEVENT_1_TOO_FAST;
			}
		} else {
			firsttime[0] = secondtime[0];
			reactiontime[0] = firsttime[0] - starttime;
		}
		if(lapcount[0] >= LAPLIM ) { /*  now check for lap counter limits */
			ISR_Signal_Falgs = ISR_Signal_Falgs | LAP_LIMIT_TRACK_1;
			lapcount[0] = 0;
		}
	ISR_Signal_Falgs = ISR_Signal_Falgs | TRACKEVENT_1;
	}
	screenupdate = 1;
}


#ifdef WIRINGPI
void Track_2_ISR(void) {
#endif /* WIRINGPI */

#ifdef LGPIO
void Track_2_ISR(int e, lgGpioAlert_p evt, void *data)
{
//   int i;
//   int userdata = *(int*)data;
//   for (i=0; i<e; i++)
//   {
//      printf("u=%d t=%"PRIu64" c=%d g=%d l=%d f=%d (%d of %d)\n",
//         userdata, evt[i].report.timestamp, evt[i].report.chip,
//         evt[i].report.gpio, evt[i].report.level,
//         evt[i].report.flags, i+1, e);
//   }
#endif /* LGPIO */

	if(timingactive[1]){
		secondtime[1] = GetTime();
		if(firsttime[1] > 0) {
			if( secondtime[1] - firsttime[1] >= config.minlaptime ) {  /*  minimum time check */
				laptime[1][lapcount[1]++] = secondtime[1] - firsttime[1];
				lasttime[1] = laptime[1][lapcount[1]-1];
				if(fasttime[1] > lasttime[1]){
					fasttime[1] = lasttime[1];
					if(overallfastest[1] > fasttime[1])
						overallfastest[1] = fasttime[1];
				}
				firsttime[1] = secondtime[1];
				overalltime[1] = secondtime[1] - starttime;
			} else {
				ISR_Signal_Falgs = ISR_Signal_Falgs | TRACKEVENT_2_TOO_FAST;
			}
		} else {
			firsttime[1] = secondtime[1];
			reactiontime[1] = firsttime[1] - starttime;
		}
		if(lapcount[1] >= LAPLIM ) {
			ISR_Signal_Falgs = ISR_Signal_Falgs | LAP_LIMIT_TRACK_2;
			lapcount[1] = 0;
		}
	ISR_Signal_Falgs = ISR_Signal_Falgs | TRACKEVENT_2;
	}
	screenupdate = 1;
}

#ifdef WIRINGPI
void Track_3_ISR(void) {
#endif /* WIRINGPI */

#ifdef LGPIO
void Track_3_ISR(int e, lgGpioAlert_p evt, void *data)
{
//   int i;
//   int userdata = *(int*)data;
//   for (i=0; i<e; i++)
//   {
//      printf("u=%d t=%"PRIu64" c=%d g=%d l=%d f=%d (%d of %d)\n",
//         userdata, evt[i].report.timestamp, evt[i].report.chip,
//         evt[i].report.gpio, evt[i].report.level,
//         evt[i].report.flags, i+1, e);
//   }
#endif /* LGPIO */

	if(timingactive[2]){
		secondtime[2] = GetTime();
		if(firsttime[2] > 0) {
			if( secondtime[2] - firsttime[2] >= config.minlaptime ) {  /*  minimum time check */
				laptime[2][lapcount[2]++] = secondtime[2] - firsttime[2];
				lasttime[2] = laptime[2][lapcount[2]-1];
				if(fasttime[2] > lasttime[2]){
					fasttime[2] = lasttime[2];
					if(overallfastest[2] > fasttime[2])
						overallfastest[2] = fasttime[2];
				}
				firsttime[2] = secondtime[2];
				overalltime[2] = secondtime[2] - starttime;
			} else {
				ISR_Signal_Falgs = ISR_Signal_Falgs | TRACKEVENT_3_TOO_FAST;
			}
		} else {
			firsttime[2] = secondtime[2];
			reactiontime[2] = firsttime[2] - starttime;
		}
		if(lapcount[2] >= LAPLIM ) {
			ISR_Signal_Falgs = ISR_Signal_Falgs | LAP_LIMIT_TRACK_3;
			lapcount[2] = 0;
		}
	ISR_Signal_Falgs = ISR_Signal_Falgs | TRACKEVENT_3;
	}
	screenupdate = 1;
}


#ifdef WIRINGPI
void Track_4_ISR(void) {
#endif /* WIRINGPI */

#ifdef LGPIO
void Track_4_ISR(int e, lgGpioAlert_p evt, void *data)
{
//   int i;
//   int userdata = *(int*)data;
//   for (i=0; i<e; i++)
//   {
//      printf("u=%d t=%"PRIu64" c=%d g=%d l=%d f=%d (%d of %d)\n",
//         userdata, evt[i].report.timestamp, evt[i].report.chip,
//         evt[i].report.gpio, evt[i].report.level,
//         evt[i].report.flags, i+1, e);
//   }
#endif /* LGPIO */

	if(timingactive[3]){
		secondtime[3] = GetTime();
		if(firsttime[3] > 0) {
			if( secondtime[3] - firsttime[3] >= config.minlaptime ) {  /*  minimum time check */
				laptime[3][lapcount[3]++] = secondtime[3] - firsttime[3];
				lasttime[3] = laptime[3][lapcount[3]-1];
				if(fasttime[3] > lasttime[3]){
					fasttime[3] = lasttime[3];
					if(overallfastest[3] > fasttime[3])
						overallfastest[3] = fasttime[3];
				}
				firsttime[3] = secondtime[3];
				overalltime[3] = secondtime[3] - starttime;
			} else {
				ISR_Signal_Falgs = ISR_Signal_Falgs | TRACKEVENT_4_TOO_FAST;
			}
		} else  {
			firsttime[3] = secondtime[3];
			reactiontime[3] = firsttime[3] - starttime;
		}
		if(lapcount[3] >= LAPLIM ) {
			ISR_Signal_Falgs = ISR_Signal_Falgs | LAP_LIMIT_TRACK_4;
			lapcount[3] = 0;
		}
	ISR_Signal_Falgs = ISR_Signal_Falgs | TRACKEVENT_4;
	}
	screenupdate = 1;
}

/* ----------------------------------------------------------------------
The functions will be called when the interrupt triggers.
These are only hardware check dummy's
------------------------------------------------------------------------*/

#ifdef WIRINGPI
void Track_1_ISR_DUMMY(void) {
#endif /* WIRINGPI */

#ifdef LGPIO
void Track_1_ISR_DUMMY(int e, lgGpioAlert_p evt, void *data)
{
//   int i;
//   int userdata = *(int*)data;
//   for (i=0; i<e; i++)
//   {
//      printf("u=%d t=%"PRIu64" c=%d g=%d l=%d f=%d (%d of %d)\n",
//         userdata, evt[i].report.timestamp, evt[i].report.chip,
//         evt[i].report.gpio, evt[i].report.level,
//         evt[i].report.flags, i+1, e);
//   }
#endif /* LGPIO */

	printf("ISR detected on Track 1\n");
}

#ifdef WIRINGPI
void Track_2_ISR_DUMMY(void) {
#endif /* WIRINGPI */

#ifdef LGPIO
void Track_2_ISR_DUMMY(int e, lgGpioAlert_p evt, void *data)
{
//   int i;
//   int userdata = *(int*)data;
//   for (i=0; i<e; i++)
//   {
//      printf("u=%d t=%"PRIu64" c=%d g=%d l=%d f=%d (%d of %d)\n",
//         userdata, evt[i].report.timestamp, evt[i].report.chip,
//         evt[i].report.gpio, evt[i].report.level,
//         evt[i].report.flags, i+1, e);
//   }
#endif /* LGPIO */

	printf("ISR detected on Track 2\n");
}

#ifdef WIRINGPI
void Track_3_ISR_DUMMY(void) {
#endif /* WIRINGPI */

#ifdef LGPIO
void Track_3_ISR_DUMMY(int e, lgGpioAlert_p evt, void *data)
{
//   int i;
//   int userdata = *(int*)data;
//   for (i=0; i<e; i++)
//   {
//      printf("u=%d t=%"PRIu64" c=%d g=%d l=%d f=%d (%d of %d)\n",
//         userdata, evt[i].report.timestamp, evt[i].report.chip,
//         evt[i].report.gpio, evt[i].report.level,
//         evt[i].report.flags, i+1, e);
//   }
#endif /* LGPIO */

	printf("ISR detected on Track 3\n");
}

#ifdef WIRINGPI
void Track_4_ISR_DUMMY(void) {
#endif /* WIRINGPI */

#ifdef LGPIO
void Track_4_ISR_DUMMY(int e, lgGpioAlert_p evt, void *data)
{
//   int i;
//   int userdata = *(int*)data;
//   for (i=0; i<e; i++)
//   {
//      printf("u=%d t=%"PRIu64" c=%d g=%d l=%d f=%d (%d of %d)\n",
//         userdata, evt[i].report.timestamp, evt[i].report.chip,
//         evt[i].report.gpio, evt[i].report.level,
//         evt[i].report.flags, i+1, e);
//   }
#endif /* LGPIO */

	printf("ISR detected on Track 4\n");
}

/* from here we start with the question: Who is first ?  */
/* first criterion  : number of laps finished            */
/* second criterion : who was first with this last lap   */
/* third criterion  : sort by reverse timing order       */

/* ----------------------------------------------------------------------
returns the time in seconds of the track, normally after race finished
Track number starts with 0
------------------------------------------------------------------------*/
float do_evaluation_time(int track, int endlap){
	int j;
	long times = 0;
	for(j=0;j<endlap;j++) {
		times += laptime[track][j];
	}
	return (float)times / 1000.0 ;  /*     */
}

/* ----------------------------------------------------------------------
returns the state of the track as position, normally after race finished
Track number starts with 0
------------------------------------------------------------------------*/
int do_evaluation_place(int track){
	int position = TRACKLIM;  /* please check for more than 4 */
	int i;
	for(i=0;i<TRACKLIM;i++) {
		if(lapcount[track] > lapcount[i]) {
			position --;
		}
	}
	return position;  /*     */
}

/* ----------------------------------------------------------------------
returns the state of all tracks in display
------------------------------------------------------------------------*/
int view_evaluation(int nout)
{
	int i,j,k;
/*	int position[TRACKLIM];  // now global */
	unsigned long behind[TRACKLIM] = { 0,0,0,0 };

	for(i=0;i<TRACKLIM;i++) {
		position[i] = do_evaluation_place(i);
	}

	for(j=0;j<TRACKLIM;j++) {
		for(i=0;i<TRACKLIM;i++) {
			for(k=0;k<TRACKLIM;k++) {
				if(position[k] == position[i])
				if(overalltime[k] > overalltime[i]) {
					position[i]--;
					behind[k] = overalltime[k] - overalltime[i];
				}
			}
			for(k=0;k<TRACKLIM;k++) {
				if(position[k] == position[i])
				if(overalltime[k] > overalltime[i]) {
					position[i]--;
					behind[k] = overalltime[k] - overalltime[i];
				}
			}
			for(k=0;k<TRACKLIM;k++) {
				if(position[k] == position[i])
				if(overalltime[k] > overalltime[i]) {
					position[i]--;
					behind[k] = overalltime[k] - overalltime[i];
				}
			}
			for(k=0;k<TRACKLIM;k++) {
				if(position[k] == position[i])
				if(overalltime[k] > overalltime[i]) {
					position[i]--;
					behind[k] = overalltime[k] - overalltime[i];
				}
			}
		}
	}

	for(i=0;i<nout /*TRACKLIM*/ ;i++) {

#ifdef FONT3X5
		move(DOTSHIFT+2+(i*FONTHEIGHT),CUR_PLACE_X );
#endif
#ifdef FONT3X3
		move(DOTSHIFT+3+(i*FONTHEIGHT),CUR_PLACE_X );
#endif

		if(position[i] <= nout) {		/* view only places smaller than used tracks */
			if(lapcount[i] > 0 ) 
				printw("%.1d  ",position[i]);
			else
				printw("*");
		}			

#ifdef FONT3X5
		move(DOTSHIFT+2+(i*FONTHEIGHT),CUR_PLACE_X + 2 );
#endif
#ifdef FONT3X3
		move(DOTSHIFT+3+(i*FONTHEIGHT),CUR_PLACE_X + 2 );
#endif
		if(position[i] == 1) {
			printw("leader      ");
		} else {
			if(behind[i] > 0)
				printw("%.3f s   ",(float)behind[i]/1000.0);
			else
				printw("         ");
		}
	}
	return 0;
}

/* ----------------------------------------------------------------------
printout the FuntionKeys informations on console
------------------------------------------------------------------------*/
void printFuntionKeys(void)
{
	printf("Function Keys:\n");
	printf("F01 - reset track 1 parameter\n");
	printf("F02 - reset track 2 parameter\n");
	printf("F03 - reset track 3 parameter\n");
	printf("F04 - reset track 4 parameter\n");
	printf("F09 - red flag\n");
	printf("F10 - yellow flag\n");
	printf("F11 - green flag (free driving, training)\n");
	printf("F12 - prepare a race with specified number of rounds or time\n");
	printf("END - exit program\n");
	printf("E - end a race\n");
	printf("g - goon after PANIC button\n");
	printf("p - PANIC button\n");
	printf("W - write timing data file\n");
	printf("Shift F01 - reset track 1 fastest lap\n");
	printf("Shift F02 - reset track 2 fastest lap\n");
	printf("Shift F03 - reset track 3 fastest lap\n");
	printf("Shift F04 - reset track 4 fastest lap\n");
	printf("ALT 1 - simulate track 1 pulse\n");
	printf("ALT 2 - simulate track 2 pulse\n");
	printf("ALT 3 - simulate track 3 pulse\n");
	printf("ALT 4 - simulate track 4 pulse\n");
}

/* ----------------------------------------------------------------------
printout the Copyright information on console
------------------------------------------------------------------------*/
void printcopyright(void)
{
	printf("YAST - Yet Another Slotcar Timer\n");
	printf("Copyright (C)  2015-2024  Martin Berentsen\n\n\n");
	printf("This program is free software: you can redistribute it and/or modify\n");
	printf("it under the terms of the GNU General Public License as published by\n");
	printf("the Free Software Foundation, either version 3 of the License, or\n");
	printf("(at your option) any later version. \n\n");
	printf("This program is distributed in the hope that it will be useful,\n");
	printf("but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
	printf("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
	printf("GNU General Public License for more details.\n\n");
	printf("You should have received a copy of the GNU General Public License\n");
	printf("along with this program.  If not, see <http://www.gnu.org/licenses/>.\n\n\n");
}


/******************************************************************
- signal handler for exit
- done by SIGTERM
*******************************************************************/
void signal_term(int why)
{
	int i;
	printf("yast: SIGTERM\n");
	for(i=0;i<TRACKLIM;i++)
		timingactive[i] = 0; 		/* ignore al timing events from now */
	stop = 1;  				/* exit from everything with correct closing*/
}

/******************************************************************
- signal handler for exit
- done by SIGINT
*******************************************************************/
void signal_int(int why)
{
	int i;
	printf("yast: SIGINT\n");
	for(i=0;i<TRACKLIM;i++)
		timingactive[i] = 0; 		/* ignore all timing events from now */
	stop = 1;  				/* exit from everything with correct closing*/
}

/* -----------------------------------------------
int store_environment(char *filename)
store overall fastest timing for all known tracks in file
-------------------------------------------------- */
int store_environment(char *filename)
{
	int i_ret = -1;
	int j;
	FILE *storage_fd;
	if( (storage_fd = fopen(filename,"w")) != NULL )
	{
		printf("creating %s storage file\n",filename);
		for(j=0;j<TRACKLIM;j++) {
			fprintf(storage_fd,"%ld\n",overallfastest[j]);
		}
		fclose(storage_fd);
		i_ret = 0;
	}
	else {
		fprintf(stderr,"Environment storage file %s can not be written\n",filename);
	}

	return i_ret;
}

/* -----------------------------------------------
int readback_environment(char *filename, int minlaptime)
read back overall fastest timing for all known tracks in file
-------------------------------------------------- */
int readback_environment(char *filename, int minlaptime)
{
	int i_ret = -1;
	int j;
	FILE *storage_fd;
	if( (storage_fd = fopen(filename,"r")) != NULL )
	{
		for(j=0;j<TRACKLIM;j++) {
			fscanf(storage_fd,"%ld\n",&overallfastest[j]);
			if(overallfastest[j] > MAXLAPTIME)
				overallfastest[j] = MAXLAPTIME;

			if(overallfastest[j] < minlaptime) {
				fprintf(stderr,"Environment readback: Set back to MAXLAPTIME of track %d, because %ld ms smaller than %d ms minlaptime\n",j+1,overallfastest[j], minlaptime);
				overallfastest[j] = MAXLAPTIME;
			}

		}

		fclose(storage_fd);
		i_ret = 0;
	}
	else {
		fprintf(stderr,"Environment storage file %s can not be examined\n",filename);
	}

	return i_ret;
}

/* ----------------------------------------------------------------------
int find_free_datafilename(char *filename,char *filepath)
returns the next free file number for data file
------------------------------------------------------------------------*/
int find_free_datafilename(char *filename,char *filepath)
{
	int retval = -1;
	int namecount = 0;     		/* takes the counter to the next free file name place holder; if written, search a new one */
	char *filenamespace = NULL;
	FILE *reslog_fd;			/* File descriptor for result file */


	printd("searching for next free data file started\n");
	filenamespace = (char *) malloc(DATESPACELENGTH);

	if( filenamespace != NULL)
	{

		if(PathExists(filepath) == 0)
		{

			if((strlen(filename)+strlen(filepath)) < (DATESPACELENGTH-10))
			{
				while(namecount < MAXNAMECOUNT)
				{
					sprintf(filenamespace,"%s%s_%d%d%d%d.txt",filepath,filename,(namecount/1000)%10,(namecount/100)%10,(namecount/10)%10,namecount%10);
					if( (reslog_fd = fopen(filenamespace,"r")) != NULL )
					{
						fclose(reslog_fd);
						namecount++;
					}
					else /* does not exists */
					{
						printd("next free file number is %d\n",namecount);
						retval = namecount;
						break;
					}
				}
				free(filenamespace);
			}
			else
			{
				fprintf(stderr,"find_free_datafilename: filename length error\n");
				/* exit(0); */
			}
		}
		else
		{
			fprintf(stderr,"find_free_datafilename: data file path %s not found \n",filepath);
			retval = -1;
		}

	}
	else
	{
		fprintf(stderr,"find_free_datafilename: malloc filenamespace error\n");
		/* exit(0); */
	}

	return retval;
}

/* ----------------------------------------------------------------------
char *generate_datafilename(char *filepath, char *filename, int namecount)
data file name generation
------------------------------------------------------------------------*/
char *generate_datafilename(char *filepath, char *filename, int namecount)
{
	return NULL;
}

/* ----------------------------------------------------------------------
char *free_datafilename(char *datafilename)
free data file name
------------------------------------------------------------------------*/
char *free_datafilename(char *datafilename)
{
	return NULL;
}

/* ----------------------------------------------------------------------
int write_datafilename(char *filename, char *filepath, int namecount, unsigned int endlap, unsigned int endtime)
write data file into the given slot number with name and path
------------------------------------------------------------------------*/
int write_datafilename(char *filename, char *filepath, int namecount, unsigned int endlap, unsigned int endtime)
{
	int retval = -1;
//	struct tm *info;
//	time_t rawtime;
	char *datespace = NULL;
	FILE *reslog_fd = NULL;		/* File descriptor for result file */
	int i,j;					/* scratch */
	long maxlap = 0;

	time( &rawtime );
	timeinfo = localtime( &rawtime );  	/* get time zone information out of the environment */

	if( (config.resultfile_format < 0) | (config.resultfile_format > 1) ) {
		fprintf(stderr,"write_datafilename: unknown file format :%d\n", config.resultfile_format);
		return retval;
	}

	/* first calculate lines to print = lapes to go ;) , required for CSV */
	maxlap = 0;
	for(i=0;i<TRACKLIM;i++){
		if(lapcount[i] > maxlap)
			maxlap = lapcount[i];
			}

	if(endlap > 0)
		if(maxlap > endlap) maxlap = endlap;


	datespace = (char *) malloc(DATESPACELENGTH);
		
	if( datespace != NULL)
	{

		if(namecount >= 0)
		{
			sprintf(datespace,"%s%s_%d%d%d%d.txt",filepath,filename,(namecount/1000)%10,(namecount/100)%10,(namecount/10)%10,namecount%10);
		} /* namecount >= 0*/

		if(namecount == -1)
		{
			sprintf(datespace,"%s",filename);

		} /* namecount == -1 */
	
		if(namecount < -1)
			return retval;

		if( (reslog_fd = fopen(datespace,"w")) != NULL )
		{
			printmessage("creating result file \"%s\" in format %d",datespace,config.resultfile_format);

				/* Storage file format 0 = ASCII CLASSIC    */
				/* Storage file format 1  = ASCII CSV EXCE1 */

#define FILEFORMATLEADER          		\
	switch(config.resultfile_format) {	\
	case 0: 				\
		fprintf(reslog_fd,"; ");	\
		break;				\
	case 1: 				\
		fprintf(reslog_fd,"# ");	\
		break;				\
	default:				\
		break;				\
	}

				FILEFORMATLEADER
				fprintf(reslog_fd,"Track result file from yast, VERSION %s\n",VERSION);
				strftime(datespace,DATESPACELENGTH,"%d.%m.%Y %T",timeinfo);          /*  DevC seems to have here a problem with this */

				FILEFORMATLEADER
				fprintf(reslog_fd,"Written on %s\n",datespace);
/*
				FILEFORMATLEADER
				fprintf(reslog_fd,"Race over %d laps with %d tracks\n",endlap,config.numberoftracks);
*/
/*				for(j=0;j<TRACKLIM;j++)
				{
				}
*/
				if(endlap > 0) {
					FILEFORMATLEADER
					fprintf(reslog_fd,"Race over %d laps with %d tracks\n",endlap,config.numberoftracks);
					for(j=0;j<TRACKLIM;j++)
					{
						if( overalltime[j] > 0) {
							FILEFORMATLEADER
							fprintf(reslog_fd,"Track %d finished %ld laps within: %.3f seconds\n", j+1, lapcount[j] ,overalltime[j] / 1000.0);
						}
					}
				}
				if(endtime > 0) {
					FILEFORMATLEADER
					fprintf(reslog_fd,"Race over %d seconds ( %dh:%.02dm:%.02ds ) with %d tracks\n",endtime,(int)floorf((float)endtime / 3600.0),((int)(floorf(endtime/60.0)))%60,((int)(floorf(endtime)))%60,config.numberoftracks);
					for(j=0;j<TRACKLIM;j++) {
						if( lapcount[j] > 0) {
							if( overalltime[j] > 0) {
								FILEFORMATLEADER
								fprintf(reslog_fd,"Track %d finished %ld laps within: %.3f seconds\n", j+1, lapcount[j] ,overalltime[j] / 1000.0);
							}
							else { /* is this really required ? */
								FILEFORMATLEADER
								fprintf(reslog_fd,"Track %d finished %ld laps \n", j+1, lapcount[j]);
							}
						}
					}
				}

				if(numberofpanics > 0)
				{
					FILEFORMATLEADER
					fprintf(reslog_fd,"PANIC button was %d times pushed\n",numberofpanics);
					FILEFORMATLEADER
					fprintf(reslog_fd,"And there were %.3f seconds in PANIC\n",overallpanictime / 1000.0);
				}
				else
				{
					FILEFORMATLEADER
					fprintf(reslog_fd,"There was no PANIC button pushed\n");
					FILEFORMATLEADER
					fprintf(reslog_fd,"And there was no time for PANIC\n");
				}

				FILEFORMATLEADER
				fprintf(reslog_fd,"Minimum lap time is configured as %ld ms\n",config.minlaptime);

//				for(j=0;j<config.numberoftracks;j++)
//				{
//					if(trackdriversname[j] != NULL) {
//						FILEFORMATLEADER
//						fprintf(reslog_fd,"Driver %d: %s\n",j+1,trackdriversname[j]);
//					}
//				}

				for(j=0;j<config.numberoftracks;j++)
				{
					if(trackdriversname[j] != NULL) {
						FILEFORMATLEADER
						fprintf(reslog_fd,"Track %d Driver: %s\n",j+1,trackdriversname[j]);
					}
					if(reactiontime[j] > 0) {
						if(endtime > 0) { /* only at time race required */
							FILEFORMATLEADER
							fprintf(reslog_fd,"Track %d REMAIN: \n",j+1);
							}
						FILEFORMATLEADER
						fprintf(reslog_fd,"Track %d Reaction time: %.3f seconds\n",j+1,(float)reactiontime[j] / 1000.0);
					}

				}

				for(j=0;j<config.numberoftracks;j++)
				{
					FILEFORMATLEADER
						fprintf(reslog_fd,"Track %d Result (place): %d\n",j+1,position[j]);				
				}

				for(j=0;j<config.numberoftracks;j++)
				{
					FILEFORMATLEADER
						fprintf(reslog_fd,"Track %d fastest lap: %.3f\n",j+1,fasttime[j] / 1000.0);				
				}

				/* Storage file format 0  = ASCII CLASSIC */
				if(config.resultfile_format == 0) {

				fprintf(reslog_fd,"; sort order by track lapnumber time \n");

					for(j=0;j<TRACKLIM;j++)
					{
						for (i=0;i<(int)lapcount[j];i++) {   /* casting is possible, because  max(lapcount) <LAPLIM */
								fprintf(reslog_fd,"%d %d %.3f\n",j+1,i+1,(float)laptime[j][i] / 1000.0);
						}
					}
				}

				/* Storage file format 1  = ASCII CSV EXCE1 */
				if(config.resultfile_format == 1) {

					FILEFORMATLEADER
					fprintf(reslog_fd,"Sort order by lapnumber; track time 1; track time 2; .......\n");
					for (i=0;i<maxlap;i++) {   /* casting is possible, because  max(lapcount) <LAPLIM */
							fprintf(reslog_fd,"%d",i+1);
							for(j=0;j<config.numberoftracks;j++)
								fprintf(reslog_fd,";%.3f",(float)laptime[j][i] / 1000.0);
							fprintf(reslog_fd,"\n");
						}

				}

			fclose(reslog_fd);
			retval = 0;
		}
		else
		{
			fprintf(stderr,"write_datafilename: Timing data file can not be written\n");
			free(datespace);
		}

		}
		else
		{
			fprintf(stderr,"write_datafilename: malloc datespace error\n");
		}

	return retval;
}

/* ----------------------------------------------------------------------
set_trackcurrent(int onoff, int active) will switch the current and set a marker
onoff is positive logic 1= on, 0= off
active is also 1 or 0
If it's already on or off it stays on or off
SCR must be open -> move and printw must be possible
------------------------------------------------------------------------*/
int set_trackcurrent(int onoff, int active)
{
	if( (config.trackcurrentoutput[0] >=0) && (active == 1) )
	{

		switch(onoff)
		{

			case 1:
			if(trackcurrent == 1)
				break;
			trackcurrent = 1;
			move(CUR_POWER_LINE_Y,CUR_POWER_LINE_X);  /* (y,x) */
			printw("On");

			#ifdef WIRINGPI
			digitalWrite(config.trackcurrentoutput[0],HIGH);
			#endif /* WIRINGPI */

			#ifdef LGPIO
			lgret = lgGpioWrite(lghandle, config.trackcurrentoutput[0], LG_HIGH);
			if (lgret < 0) 
			{
				printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
				yaGpioGetMode(lghandle,config.trackcurrentoutput[0]); 
				return 1; }
			#endif /* LGPIO */

			#ifdef MCP23017
			digitalWrite(MCP23017_pinBase + MCP23017_BACKSIDE,MCP23017_ON);		/* set alive LED to ON */
			#endif /* MCP23017 */

			#ifdef I2C
			yaSetMCP23017(MCP23017_BACKSIDE, YA_HIGH);  /* Set trackpower LED high */
			#endif /* I2C */

			if(config.soundactive == 1) { /* make some noise, it track power is switched off */
#ifdef ALSA_SOUND
			SND_play(1);
#endif /* ALSA_SOUND */
			}
			break;
			default:
			if(trackcurrent == 0)
				break;
			trackcurrent = 0;
			move(CUR_POWER_LINE_Y,CUR_POWER_LINE_X);  /* (y,x) */
			printw("--");

			#ifdef WIRINGPI
			digitalWrite(config.trackcurrentoutput[0],LOW);
			#endif /* WIRINGPI */

			#ifdef LGPIO
			lgret = lgGpioWrite(lghandle, config.trackcurrentoutput[0], LG_LOW);
			if (lgret < 0) 
			{
				printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
				yaGpioGetMode(lghandle,config.trackcurrentoutput[0]); 
				return 1; }
			#endif /* LGPIO */
			
			#ifdef MCP23017
			digitalWrite(MCP23017_pinBase + MCP23017_BACKSIDE,MCP23017_OFF);		/* set alive LED to OFF */
			#endif /* MCP23017 */

			#ifdef I2C
			yaSetMCP23017(MCP23017_BACKSIDE, YA_LOW); /* Set trackpower LED low */
			#endif /* I2C */

			if(config.soundactive == 1) { /* make some noise, it track power is switched off */
#ifdef ALSA_SOUND
			SND_play(2);
#endif /* ALSA_SOUND */
			}
			break;
		}
	}

	if( (config.trackcurrentoutput[1] >=0) && (active == 1) )
	{

		switch(onoff)
		{
			case 1:
			#ifdef WIRINGPI
			digitalWrite(config.trackcurrentoutput[1],HIGH);
			#endif /* WIRINGPI */
	
			#ifdef LGPIO
			lgret = lgGpioWrite(lghandle, config.trackcurrentoutput[1], LG_HIGH);
			if (lgret < 0) 
			{
				printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
				yaGpioGetMode(lghandle,config.trackcurrentoutput[1]); 
				return 1; }
			#endif /* LGPIO */
				
			break;

			default:
			#ifdef WIRINGPI
			digitalWrite(config.trackcurrentoutput[1],LOW);
			#endif /* WIRINGPI */

			#ifdef LGPIO
			lgret = lgGpioWrite(lghandle, config.trackcurrentoutput[1], LG_LOW);
			if (lgret < 0) 
			{
				printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
				yaGpioGetMode(lghandle,config.trackcurrentoutput[1]); 
				return 1; }
			#endif /* LGPIO */
			
			break;
		}
	}

	if( (config.trackcurrentoutput[2] >=0) && (active == 1) )
	{

		switch(onoff)
		{
			case 1:
			#ifdef WIRINGPI
			digitalWrite(config.trackcurrentoutput[2],HIGH);
			#endif /* WIRINGPI */

			#ifdef LGPIO
			lgret = lgGpioWrite(lghandle, config.trackcurrentoutput[2], LG_HIGH);
			if (lgret < 0) 
			{
				printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
				yaGpioGetMode(lghandle,config.trackcurrentoutput[2]); 
				return 1; }
			#endif /* LGPIO */
			
			break;

			default:
			#ifdef WIRINGPI
			digitalWrite(config.trackcurrentoutput[2],LOW);
			#endif /* WIRINGPI */

			#ifdef LGPIO
			lgret = lgGpioWrite(lghandle, config.trackcurrentoutput[2], LG_LOW);
			if (lgret < 0) 
			{
				printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
				yaGpioGetMode(lghandle,config.trackcurrentoutput[2]); 
				return 1; }
			#endif /* LGPIO */
						
			
			break;
		}
	}

	if( (config.trackcurrentoutput[3] >=0) && (active == 1) )
	{

		switch(onoff)
		{
			case 1:
			#ifdef WIRINGPI
			digitalWrite(config.trackcurrentoutput[3],HIGH);
			#endif /* WIRINGPI */
			
			#ifdef LGPIO
			lgret = lgGpioWrite(lghandle, config.trackcurrentoutput[3], LG_HIGH);
			if (lgret < 0) 
			{
				printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
				yaGpioGetMode(lghandle,config.trackcurrentoutput[3]); 
				return 1; }
			#endif /* LGPIO */
						
			break;

			default:
			#ifdef WIRINGPI
			digitalWrite(config.trackcurrentoutput[3],LOW);
			#endif /* WIRINGPI */
			
			#ifdef LGPIO
			lgret = lgGpioWrite(lghandle, config.trackcurrentoutput[3], LG_LOW);
			if (lgret < 0) 
			{
				printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
				yaGpioGetMode(lghandle,config.trackcurrentoutput[3]); 
				return 1; }
			#endif /* LGPIO */
						
			break;
		}
	}
	return 0;
} /* set_trackcurrent  */

/* ----------------------------------------------------------------------
MAIN Program goes from this point
------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	unsigned int endlap = 50;   	/* last lap of a race */
	unsigned int endtime = 0;	/* last second of the race*/
	int gostate = G_WAIT;         	/* state var, 0 is waiting, 1, 2, 3, 4, 5, 6, is startlight, 7 is run, 8 is ready, starting up with doing nothing ;) */
	unsigned long acttime = 0;	/* var to hold the actual time */
	int datafilenamecount = 0;     	/* takes the counter to the next free file name place holder; if written, search a new one */
	int racedatawrittentofile = 1;	/* Is the race data already written to a file, to prevent double writing */
	unsigned long finishlap[TRACKLIM];    		/*  lap counter for every track finish round count while checkered flag */
	int carsin = 0;					/* number of cars, which have finished */
	int hardwarecheck = 0;				/* hardware check flag, formerly yast_check */
	int failstartpoweroff = 0;			/* if a false start occurs a yellow flag and power off appears */
	unsigned long timetogo; 	/* time to go in seconds */
	unsigned long lapstogo; 	/* laps to go in seconds */
	int timetogoheight = 0; 	/* height addition of shown time/lap to go, min TrafficLight, max Trafficlight + Trafficlightheight */
	int display_y, display_x; 	/* size of the screen ASCII display */
	int random_light_startup_time = 6 * STARTLIGHTDELAY; /* value of the random starttime */

	int i,j;			/* scratch */
	int inchr;  			/* input char buffer for curses */

	/* first set up the default for the configuration structure */

	config.minlaptime = MINLAPTIME;   	/* minimum time between two track events */
	config.numberoftracks = 4;		/* number of tracks used for calculating and race mode, min 1 max 8 */
	config.outfile = 0;			/* a logfile has to be written 0=none, 1 = counted, 2 = forced */
	config.rtc_view = 0;			/* do not view RTC by default */
	config.trackcurrentoutput[0] = -1;	/* default output GPIO port for track current switching Track 1 */
	config.trackcurrentoutput[1] = -1;	/* default output GPIO port for track current switching Track 2 */
	config.trackcurrentoutput[2] = -1;	/* default output GPIO port for track current switching Teack 3 */
	config.trackcurrentoutput[3] = -1;	/* default output GPIO port for track current switching Track 4 */
	config.trackpoweractive = 0;		/* track power can be activated/deactivated */
	config.debuglevel = 0;			/* set debug level, default = 0 */
	config.random_light_startup = 1;	/* if 1, random start time at Christmas tree , 0 else */
	config.panicdelaytime = PANICDELAYTIME;  /* panic delay for timing end */
	config.timeraceenddelaytime = TIMERACEENDDELAYTIME; /* race delay for timing end */
	config.trackcolor[0] = COLOR_RED;	/* predefining the track colors */
	config.trackcolor[1] = COLOR_GREEN;
	config.trackcolor[2] = COLOR_BLUE;
	config.trackcolor[3] = COLOR_YELLOW;
	config.soundactive = 0;			/* predefine the sound already active =1 or not =0; */

	#ifdef WIRINGPI
	config.trackinputpin[0] = PIN_IN_TRACK_1;	/* set the wiringPi input pin to track number */
	config.trackinputpin[1] = PIN_IN_TRACK_2;
	config.trackinputpin[2] = PIN_IN_TRACK_3;
	config.trackinputpin[3] = PIN_IN_TRACK_4;
	config.trackinputevent[0] = PIN_IN_EVENT_1;	/* set the wiringPi input pin event type */
	config.trackinputevent[1] = PIN_IN_EVENT_2;
	config.trackinputevent[2] = PIN_IN_EVENT_3;
	config.trackinputevent[3] = PIN_IN_EVENT_4;
	config.trackinputpud[0] = PIN_IN_PUD_1;		/* set the wiringPi input pin pull up/down */
	config.trackinputpud[1] = PIN_IN_PUD_2;
	config.trackinputpud[2] = PIN_IN_PUD_3;
	config.trackinputpud[3] = PIN_IN_PUD_4;
	#endif /* WIRINGPI */

	#ifdef LGPIO
	config.trackinputpin[0] = PIN_IN_TRACK_1;	/* set the wiringPi input pin to track number */
	config.trackinputpin[1] = PIN_IN_TRACK_2;
	config.trackinputpin[2] = PIN_IN_TRACK_3;
	config.trackinputpin[3] = PIN_IN_TRACK_4;
	config.trackinputevent[0] = PIN_IN_EVENT_1;	/* set the wiringPi input pin event type */
	config.trackinputevent[1] = PIN_IN_EVENT_2;
	config.trackinputevent[2] = PIN_IN_EVENT_3;
	config.trackinputevent[3] = PIN_IN_EVENT_4;
	config.trackinputpud[0] = PIN_IN_PUD_1;		/* set the wiringPi input pin pull up/down */
	config.trackinputpud[1] = PIN_IN_PUD_2;
	config.trackinputpud[2] = PIN_IN_PUD_3;
	config.trackinputpud[3] = PIN_IN_PUD_4;
	#endif /* LGPIO */
	

	trackdriversname[0] = NULL; 			/* "Martin B."; , later on max 13 chars */
	trackdriversname[1] = NULL;
	trackdriversname[2] = NULL;
	trackdriversname[3] = NULL;

	/* set locale, required for unicode */
#ifdef UNICODE
	setlocale(LC_CTYPE, "");  /* only required for unicode and *NOT* for CP437 */
#endif /* UNICODE */

	/* copy the strings into struct config malloc space */

	if((config.resultfile_name = (char *)malloc(strlen(RESULTFILENAME)+1)) != NULL)
	{
		strcpy(config.resultfile_name,RESULTFILENAME);
	}
	else
	{
		fprintf(stderr,"main: malloc resultfile_name error\n");
		exit(-1);
	}

	if((config.resultfile_path = (char *)malloc(strlen(RESULTFILEPATH)+1)) != NULL)
	{
		strcpy(config.resultfile_path,RESULTFILEPATH);
	}
	else
	{
		fprintf(stderr,"main: malloc resultfile_path error\n");
		exit(-1);
	}

	if((config.storagefile_name = (char *)malloc(strlen(STORAGEFILENAME)+1)) != NULL)
	{
		strcpy(config.storagefile_name,STORAGEFILENAME);
	}
	else
	{
		fprintf(stderr,"main: malloc storagefile_path error\n");
		exit(-1);
	}

	config.resultfile_format = 0; /* 0 = default old format, 1 = exce1 ascii input format */

	/* read the configuration file  */
	/* taken from /etc and local, /etc is more recent */

	if( read_yastrc( (char *)"/etc/"CONFFILENAME ) == -1)
		read_yastrc( (char *)CONFFILENAME );

	/* parse the command line */

	for( ; ; ) {

		if ( argc >= 2 && strcmp( argv[1], "-a" ) == 0 ) {
			++argv; --argc;
			config.trackpoweractive = 1;
			continue;
		}
		if ( argc >= 2 && strcmp( argv[1], "-c" ) == 0 ) {
			++argv; --argc;
			hardwarecheck = 1;
			continue;
		}
		if ( argc >= 2 && strcmp( argv[1], "-D" ) == 0 ) {
			++argv; --argc;
			if( (argc < 2) && argv[1] == NULL) {                          /* just for secure */
				fprintf(stderr,"no DEBUG LEVEL given\n");
				exit(1);
			}
			config.debuglevel = atoi(argv[1]);
			++argv; --argc;					/*  move to the next */
			if((config.debuglevel < 0) || (config.debuglevel > MAXDEBUGLEVEL)) {
				fprintf(stderr,"wrong DEBUG LEVEL given: %d, must be between 0 and %d \n",config.debuglevel,MAXDEBUGLEVEL);
				exit(1);
			}
			continue;
		}
		if ( argc >= 2 && strcmp( argv[1], "-d" ) == 0 ) {
			++argv; --argc;
			if( (argc < 2) && argv[1] == NULL) {                          /* just for secure */
				fprintf(stderr,"no delay time given\n");
				exit(1);
			}
			config.minlaptime = atoi(argv[1]);
			++argv; --argc;					/*  move to the next */
			if((config.minlaptime < 10) || (config.minlaptime > MAXDEADDELAY)) {
				fprintf(stderr,"wrong minimum lap time given: %ld, must be between 10 and %d mseconds\n",config.minlaptime,MAXDEADDELAY);
				exit(1);
			}
			continue;
		}
		if ( argc >= 2 && strcmp( argv[1], "-f" ) == 0 ) {
			++argv; --argc;
			failstartpoweroff = 1;
			continue;
		}
		if ( argc >= 2 && strcmp( argv[1], "-l" ) == 0 ) {
			++argv; --argc;
			if( (argc < 2) && argv[1] == NULL) {                          /* just for secure */
				fprintf(stderr,"no lap number given\n");
				exit(1);

			}
			endlap = atoi(argv[1]);
			++argv; --argc;
			if((endlap < 1) || (endlap > LAPLIM)) {
				fprintf(stderr,"wrong endlap number given: %d, must be between 1 and %d laps\n",endlap,LAPLIM );
				exit(1);
			}
			continue;
		}
		if ( argc >= 2 && strcmp( argv[1], "-o" ) == 0 ) {
			++argv; --argc;
			if(config.outfile > 0) {
				fprintf(stderr,"dont't mix -n and -o option\n");
				exit(1);
			}
			else 
				config.outfile = 1; /* counted logfile */
			continue;
		}

		if ( argc >= 2 && strcmp( argv[1], "-r" ) == 0 ) {
			++argv; --argc;
			config.rtc_view = 1;
			continue;
		}

		if ( argc >= 2 && strcmp( argv[1], "-s" ) == 0 ) {
			++argv; --argc;
			config.soundactive = 1;
			continue;
		}
		if ( argc >= 2 && strcmp( argv[1], "-t" ) == 0 ) {
			++argv; --argc;
			if( (argc < 2) && argv[1] == NULL) {                          /* just for secure */
				fprintf(stderr,"no time given\n");
				exit(1);

			}
			endtime = atoi(argv[1]);
			endlap = 0;		/* race on time needs no last lap number */
			++argv; --argc;
			if((endtime < 10) || (endtime > TIMELIM)) {
				fprintf(stderr,"wrong endtime given: %d, must be between 10 and %d seconds\n",endtime,TIMELIM );
				exit(1);
			}
			continue;
		}
		if ( argc >= 2 && strcmp( argv[1], "-u" ) == 0 ) {
			++argv; --argc;
			if( (argc < 2) && argv[1] == NULL) {                          /* just for secure */
				fprintf(stderr,"no number of used tracks given\n");
				exit(1);
			}
			config.numberoftracks = atoi(argv[1]);
			++argv; --argc;
			if(( config.numberoftracks < 1) || (config.numberoftracks > TRACKLIM)) {
				fprintf(stderr,"wrong number of used tracks given: %d, must be between 1 and %d\n",config.numberoftracks,TRACKLIM );
				exit(1);
			}
			continue;
		}
		if ( argc >= 2 && strcmp( argv[1], "-N1" ) == 0 ) {
			++argv; --argc;
			if( (argc < 2) && argv[1] == NULL) {                          /* just for secure */
				fprintf(stderr,"no Drivers Name for Track 1 given\n");
				exit(1);
			}
			trackdriversname[0] = malloc(strlen(argv[1])+1);
			strncpy(trackdriversname[0],argv[1], strlen(argv[1]) );
			if(strlen(argv[1]) > MAXDRIVERSNAMELENGTH )
				trackdriversname[0][MAXDRIVERSNAMELENGTH] = 0;
			else
				trackdriversname[0][strlen(argv[1])] = 0;
			++argv; --argc;
			continue;
		}
		if ( argc >= 2 && strcmp( argv[1], "-N2" ) == 0 ) {
			++argv; --argc;
			if( (argc < 2) && argv[1] == NULL) {                          /* just for secure */
				fprintf(stderr,"no Drivers Name for Track 2 given\n");
				exit(1);
			}
			trackdriversname[1] = malloc(strlen(argv[1])+1);
			strncpy(trackdriversname[1],argv[1], strlen(argv[1]) );
			if(strlen(argv[1]) > MAXDRIVERSNAMELENGTH )
				trackdriversname[1][MAXDRIVERSNAMELENGTH] = 0;
			else
				trackdriversname[1][strlen(argv[1])] = 0;
			++argv; --argc;
			continue;
		}
		if ( argc >= 2 && strcmp( argv[1], "-N3" ) == 0 ) {
			++argv; --argc;
			if( (argc < 2) && argv[1] == NULL) {                          /* just for secure */
				fprintf(stderr,"no Drivers Name for Track 3 given\n");
				exit(1);
			}
			trackdriversname[2] = malloc(strlen(argv[1])+1);
			strncpy(trackdriversname[2],argv[1], strlen(argv[1]) );
			if(strlen(argv[1]) > MAXDRIVERSNAMELENGTH )
				trackdriversname[2][MAXDRIVERSNAMELENGTH] = 0;
			else
				trackdriversname[2][strlen(argv[1])] = 0;
			++argv; --argc;
			continue;
		}
		if ( argc >= 2 && strcmp( argv[1], "-N4" ) == 0 ) {
			++argv; --argc;
			if( (argc < 2) && argv[1] == NULL) {                          /* just for secure */
				fprintf(stderr,"no Drivers Name for Track 4 given\n");
				exit(1);
			}
			trackdriversname[3] = malloc(strlen(argv[1])+1);
			strncpy(trackdriversname[3],argv[1], strlen(argv[1]) );
			if(strlen(argv[1]) > MAXDRIVERSNAMELENGTH )
				trackdriversname[3][MAXDRIVERSNAMELENGTH] = 0;
			else
				trackdriversname[3][strlen(argv[1])] = 0;
			++argv; --argc;
			continue;
		}
		if ( argc >= 2 && strcmp( argv[1], "-n" ) == 0 ) {
			++argv; --argc;
			if( (argc < 2) && argv[1] == NULL) {                          /* just for secure */
				fprintf(stderr,"no timing filename given\n");
				exit(1);
			}

//			config.resultfile_name
//			strncpy(trackdriversname[3],argv[1], strlen(argv[1]) );
//			if(strlen(argv[1]) > MAXDRIVERSNAMELENGTH )
//				trackdriversname[3][MAXDRIVERSNAMELENGTH] = 0;
//			else
//				trackdriversname[3][strlen(argv[1])] = 0;
			if(config.outfile > 0) {
				fprintf(stderr,"dont't mix -n and -o option\n");
				exit(1);
			}
			else { 
				config.outfile = 2;  /* set forced output file */
				if((config.resultfile_name = (char *)malloc(strlen(argv[1])+1)) != NULL) {
					strcpy(config.resultfile_name,argv[1]);
				}
				else
				{
					fprintf(stderr,"config: malloc resultfile_name error\n");
					exit(-1);
				}
			}
			++argv; --argc;
			continue;
			
		}

		if ( argc >= 2 && strcmp( argv[1], "-h" ) == 0 ) {
			printcopyright();
			printf("usage: yast [option(s)]\n");
			printf("Version: %s, compiled at: %s %s\n",VERSION,__TIME__,__DATE__);
			printf("options are:\n");
			printf("  -a   : activate track power switching\n");
			printf("  -c   : hardware check loop, endless, sound possible (quit with CTRL-C)\n");
			printf("  -D n : set DEBUG level to n, default = 0, max = %d\n",MAXDEBUGLEVEL);
			printf("  -d n : n is the minimum laptime in ms between two track events (dead time)\n");
			printf("  -f   : activate false start detection\n");
			printf("  -h   : this little help, exit\n");
			printf("  -l n : n is number of laps to run in race\n");
			printf("  -Nn s: n is number of track, s is drivers name\n");
			printf("  -n s : s is filename, filename counter isn't active, file will be overwritten\n");
			printf("  -o   : write a logfile of the track timimg\n");
			printf("  -r   : view real time clock if no race is ongoing\n");
#ifdef ALSA_SOUND
			printf("  -s   : activate sound\n");
#else /* ALSA_SOUND*/
			printf("  -s   : dummy, no sound function compiled in\n");
#endif /* ALSA_SOUND*/
			printf("  -t n : n is number of seconds to run in race\n");
			printf("  -u n : n is number of used tracks, min = 1, max = %d\n",TRACKLIM);
			printf("  -v   : printout Copyright and Version information, exit\n");
			printf("  -vv  : printout Version, compile and config information, exit\n");
			printFuntionKeys();
			exit(0);
			continue;
		}
		if ( argc >= 2 && strcmp( argv[1], "-v" ) == 0 ) {
			printcopyright();
			printf("yast Version: %s\ncompiled at %s on %s\n",VERSION,__TIME__,__DATE__);
			if(RaspberryPiVersion() != 0)
				printf("no Pi hardware descriptor found\n");
			#ifdef LGPIO
			lgret = lguVersion();
			printf("lg Version: %0x (%d.%d.%d) \n", lgret, lgret&0x0000ff, (lgret>>8)&0x00ff, lgret>>16);
			#endif /* LGPIO */
			if(LinuxVersion() != 0)
				printf("no Linux version descriptor found\n");
			exit(0);
			continue;
		}
		if ( argc >= 2 && strcmp( argv[1], "-vv" ) == 0 ) {
			++argv; --argc;
			printf("------------------------------------------------------------------\n");
			printf("-        -vv additional information output from here             -\n");
			printf("------------------------------------------------------------------\n");
			printf("Version : %s\nCompiled at : %s %s\n",VERSION,__TIME__,__DATE__);
			printf("Compile options :\n");
#ifdef WIRINGPI
			printf(" #define WIRINGPI\n");
#endif /* WIRINGPI */

#ifdef LGPIO
			printf(" #define LGPIO\n");
#endif /* LGPIO */

#ifdef ALSA_SOUND
			printf(" #define ALSA_SOUND\n");
#endif /* ALSA_SOUND */

#ifdef MCP23008
			printf(" #define MCP23008\n");
#endif /* MCP23008 */
#ifdef MCP23017
			printf(" #define MCP23017\n");
#endif /* MCP23017 */
#ifdef MCP23017EASY
			printf(" #define MCP23017EASY\n");
#endif /* MCP23017EASY */
#ifdef SN3218
			printf(" #define SN3218\n");
#endif /* SN3218 */
#ifdef OFFLINE
			printf(" #define OFFLINE\n");
#endif /* OFFLINE */
			printf(" Reserved lap memory is : %d laps x %d tracks x %ld Bytes = %.2f kBytes\n",LAPLIM,TRACKLIM,sizeof(long),(float)(LAPLIM*TRACKLIM*sizeof(long))/1024.0);
			printf("Config options :\n");
			printf(" Number of tracks : %d\n",config.numberoftracks);
			printf(" Minimum lap time : %ld ms\n",config.minlaptime);
			printf(" Input Pins configured to (GPIO): ");
			for(i=0;i<TRACKLIM;i++) {
				printf("[Tr%.2d = In%.2d]",i,config.trackinputpin[i]);
			}
			printf("\n");
			printf(" Pull Up : ");
			for(i=0;i<TRACKLIM;i++) {
				printf("[Tr%.2d = Pu%.2d]",i,config.trackinputpud[i]);
			}
			printf("\n");
			printf(" Input Event : ");
			for(i=0;i<TRACKLIM;i++) {
				printf("[Tr%.2d = Ev%.2d]",i,config.trackinputevent[i]);
			}
			printf("\n");

			printf(" Output Pins configured to (GPIO): ");
			for(i=0;i<TRACKLIM;i++) {
				printf("[Tr%.2d = out %.2d]",i,config.trackcurrentoutput[i]);
			}
			printf("\n");

			printf(" Race lap counter : %d laps\n",endlap);
			printf(" Race time counter : %d seconds\n",endtime);
			printf(" Timing file writing is : ");
			switch(config.outfile) {
			case 0:
				printf("off\n");
				break;
			case 1:
				printf("on\n");
				break;
			case 2:
				printf("forced\n");
				printf(" filename will be overwritten by command line to \"%s\"\n",config.resultfile_name);
				break;
			default:
				printf("unknown\n");
				break;
			}
				
			printf(" Writing timing output to path : \"%s\"\n",config.resultfile_path);
			printf(" Writing timing output to file : \"%s\"\n",config.resultfile_name);
			printf(" Writing timing output in format : %d\n",config.resultfile_format);
			printf(" Writing storage data to file : \"%s\"\n",config.storagefile_name);

			printf(" Track power 1 output port (GPIO): %d\n",config.trackcurrentoutput[0]);
			printf(" Track power 2 output port (GPIO): %d\n",config.trackcurrentoutput[1]);
			printf(" Track power 3 output port (GPIO): %d\n",config.trackcurrentoutput[2]);
			printf(" Track power 4 output port (GPIO): %d\n",config.trackcurrentoutput[3]);

			printf(" Track power switching is : ");
				if(config.trackpoweractive == 1)
					printf("active\n");
				else
					printf("passive\n");

			printf(" Driver's names:");
			for(i=0;i<TRACKLIM ;i++) {
			if(trackdriversname[i] != NULL) 
				printf(" Track %d - \"%s\"",i+1,trackdriversname[i]);
			else
				printf(" Track %d - none",i+1);
				
			}
			printf("\n");
			printf(" PANICDELAYTIME : %d ms\n",config.panicdelaytime );
			printf(" TIMERACEENDDELAYTIME : %d ms\n",config.timeraceenddelaytime );

			printf(" Sound is : ");
				if(config.soundactive == 1)
					printf("on\n");
				else
					printf("off\n");

			printf(" RTC (Real Time Clock viewing) is : ");
			if(config.rtc_view > 0)
				printf("on\n");
			else
				printf("off\n");
			
			printf("------------------------------------------------------------------\n");
			printf("!-!-!-!-! now standard startup without main loop follows !-!-!-!-!\n");
			printf("------------------------------------------------------------------\n");
			stop = 1; 		/* don't do the main loop */
			continue;
		}

		break;
	}

	/******************************************************************************************/

	printcopyright();
	printf("YAST information....\n");
	printf("Version: %s compiled at: %s %s\n",VERSION,__DATE__,__TIME__);
	if(RaspberryPiVersion() != 0)
		printf("no Pi hardware descriptor found\n");
	if(LinuxVersion() != 0)
		printf("no Linux version descriptor found\n");
	printf("Reserved lap memory is: %d laps x %d tracks x %ld Bytes = %.2f kBytes\n",LAPLIM,TRACKLIM,sizeof(long),(float)(LAPLIM*TRACKLIM*sizeof(long))/1024.0);
	printf("Race lap counter set to %d laps\n",endlap);
	printf("Race time counter set to %d seconds\n",endtime);
	printf("Minimum lap time set to %ld ms\n",config.minlaptime);
	if(config.outfile > 0)
		printf("writing timing output to file\n");

	/* WiringPi version here */

	#ifdef WIRINGPI
	printf("wiringPi Setup....\n");

	if(CheckLock(GPIOLOCKFILENAME) > 0)
	{
		fprintf(stderr,"Old lockfile active found, exiting....\n");
		exit(0);
	}
	if(CreateLock(GPIOLOCKFILENAME, "yast" /* argv[0] */ ) == -1)
		exit(0);

	wiringPiSetup();
	pinMode (config.trackinputpin[0], INPUT);
	pinMode (config.trackinputpin[1], INPUT);
	pinMode (config.trackinputpin[2], INPUT);
	pinMode (config.trackinputpin[3], INPUT);

	printf("Setting four GPIO's to input....\n");

	pullUpDnControl(config.trackinputpin[0],config.trackinputpud[0]);
	pullUpDnControl(config.trackinputpin[1],config.trackinputpud[1]);
	pullUpDnControl(config.trackinputpin[2],config.trackinputpud[2]);
	pullUpDnControl(config.trackinputpin[3],config.trackinputpud[3]);

	printf("Input Port  Track_1: %d Track_2: %d Track_3: %d Track_4: %d\n",config.trackinputpin[0], config.trackinputpin[1], config.trackinputpin[2], config.trackinputpin[3]);
	printf("Input State Track_1: %d Track_2: %d Track_3: %d Track_4: %d\n",digitalRead(config.trackinputpin[0]), digitalRead(config.trackinputpin[1]), digitalRead(config.trackinputpin[2]), digitalRead(config.trackinputpin[3]));
	printf("Output Port Track_1: %d Track_2: %d Track_3: %d Track_4: %d\n",config.trackcurrentoutput[0], config.trackcurrentoutput[1], config.trackcurrentoutput[2], config.trackcurrentoutput[3]);

	/* implement the IRS for all tracks  */
	if(hardwarecheck == 0) {
		wiringPiISR(config.trackinputpin[0], config.trackinputevent[0], &Track_1_ISR);
		wiringPiISR(config.trackinputpin[1], config.trackinputevent[1], &Track_2_ISR);
		wiringPiISR(config.trackinputpin[2], config.trackinputevent[2], &Track_3_ISR);
		wiringPiISR(config.trackinputpin[3], config.trackinputevent[3], &Track_4_ISR);
	}
	else {
		wiringPiISR(config.trackinputpin[0], config.trackinputevent[0], &Track_1_ISR_DUMMY);
		wiringPiISR(config.trackinputpin[1], config.trackinputevent[1], &Track_2_ISR_DUMMY);
		wiringPiISR(config.trackinputpin[2], config.trackinputevent[2], &Track_3_ISR_DUMMY);
		wiringPiISR(config.trackinputpin[3], config.trackinputevent[3], &Track_4_ISR_DUMMY);
	}
	printf("Setting up GPIO's for output lines ....\n");

	if( config.trackcurrentoutput[0] >=0)
	{
		for(i=0;i<TRACKLIM;i++)
		{
			if(config.trackcurrentoutput[0] == config.trackinputpin[i])
			{
				printf("TrackCurrentOutput 1 is already an input (%d)\n",config.trackcurrentoutput[0]);
				exit(-1);
			}
		}

		pinMode(config.trackcurrentoutput[0], OUTPUT);
		printf("TrackCurrentOutput 1 set to GPIO Pin %d\n",config.trackcurrentoutput[0]);

		if(config.trackpoweractive == 1)
			printf("Track power switching activated!\n");

	}

	if( config.trackcurrentoutput[1] >=0)
	{
		for(i=0;i<TRACKLIM;i++)
		{
			if(config.trackcurrentoutput[1] == config.trackinputpin[i])
			{
				printf("TrackCurrentOutput 2 is already an input (%d)\n",config.trackcurrentoutput[1]);
				exit(-1);
			}
		}

		pinMode(config.trackcurrentoutput[1], OUTPUT);
		printf("TrackCurrentOutput 2 set to GPIO Pin %d\n",config.trackcurrentoutput[1]);

	}

	if( config.trackcurrentoutput[2] >=0)
	{
		for(i=0;i<TRACKLIM;i++)
		{
			if(config.trackcurrentoutput[2] == config.trackinputpin[i])
			{
				printf("TrackCurrentOutput 3 is already an input (%d)\n",config.trackcurrentoutput[2]);
				exit(-1);
			}
		}

		pinMode(config.trackcurrentoutput[2], OUTPUT);
		printf("TrackCurrentOutput 3 set to GPIO Pin %d\n",config.trackcurrentoutput[2]);

	}

	if( config.trackcurrentoutput[3] >=0)
	{
		for(i=0;i<TRACKLIM;i++)
		{
			if(config.trackcurrentoutput[3] == config.trackinputpin[i])
			{
				printf("TrackCurrentOutput 4 is already an input (%d)\n",config.trackcurrentoutput[3]);
				exit(-1);
			}
		}

		pinMode(config.trackcurrentoutput[3], OUTPUT);
		printf("TrackCurrentOutput 4 set to GPIO Pin %d\n",config.trackcurrentoutput[3]);

	}

	if(config.trackpoweractive == 0) {
	/* set timing overhed to zero, because it makes no sense */
		config.panicdelaytime = 0;  /* panic delay for timing end to zero */
		config.timeraceenddelaytime = 0; /* race delay for timing end to zero */
		printf("Track power not activated, panicdelaytime and timeraceenddelaytime set to zero\n");

	}

	#endif  /*  WIRINGPI */

	/* lgpio version here */

	#ifdef LGPIO
	printf(" lgpio Setup....\n");

	if(CheckLock(GPIOLOCKFILENAME) > 0)
	{
		fprintf(stderr,"Old lockfile active found, exiting....\n");
		exit(0);
	}
	if(CreateLock(GPIOLOCKFILENAME, "yast" /* argv[0] */ ) == -1)
		exit(0);

	// wiringPiSetup();
	lghandle = lgGpiochipOpen(0);
	if (lghandle < 0) { printf("ERROR: %s (%d)\n", lguErrorText(lghandle), lghandle); return 1; }
	else printf("lgGpiochipOpen done\n");
	
	lgret = lguVersion();
	printf("lg Version: %0x (%d.%d.%d) \n", lgret, lgret&0x0000ff, (lgret>>8)&0x00ff, lgret>>16);

	lgret = lgGpioGetChipInfo(lghandle, &cInfo);

	if (lgret == LG_OKAY)
	{
	printf("ChipInfo: lines=%d name=%s label=%s\n", cInfo.lines, cInfo.name, cInfo.label);
	}
		else
		{
		printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
		return 1;
	}

	printf("Input Port  Track_1: %d Track_2: %d Track_3: %d Track_4: %d\n",config.trackinputpin[0], config.trackinputpin[1], config.trackinputpin[2], config.trackinputpin[3]);
	printf("Input State Track_1: %d Track_2: %d Track_3: %d Track_4: %d\n",lgGpioRead(lghandle,config.trackinputpin[0]), lgGpioRead(lghandle,config.trackinputpin[1]), lgGpioRead(lghandle,config.trackinputpin[2]), lgGpioRead(lghandle,config.trackinputpin[3]));
	printf("Output Current Port Track_1: %d Track_2: %d Track_3: %d Track_4: %d\n",config.trackcurrentoutput[0], config.trackcurrentoutput[1], config.trackcurrentoutput[2], config.trackcurrentoutput[3]);

	/* implement the IRS for all tracks  */
	if(hardwarecheck == 0) {

		lgret = lgGpioSetAlertsFunc(lghandle, config.trackinputpin[0],  Track_1_ISR, &userdata_1);
		if (lgret < 0) 
		{
			printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
			yaGpioGetMode(lghandle,config.trackinputpin[0]); 
			return 1; }

		lgret = lgGpioSetAlertsFunc(lghandle, config.trackinputpin[1],  Track_2_ISR, &userdata_2);
		if (lgret < 0) 
		{
			printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
			yaGpioGetMode(lghandle,config.trackinputpin[1]); 
			return 1; }

		lgret = lgGpioSetAlertsFunc(lghandle, config.trackinputpin[2],  Track_3_ISR, &userdata_3);
		if (lgret < 0) 
		{
			printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
			yaGpioGetMode(lghandle,config.trackinputpin[2]); 
			return 1; }

		lgret = lgGpioSetAlertsFunc(lghandle, config.trackinputpin[3],  Track_4_ISR, &userdata_4);
		if (lgret < 0) 
		{
			printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
			yaGpioGetMode(lghandle,config.trackinputpin[3]); 
			return 1; }

	}
	else {

		lgret = lgGpioSetAlertsFunc(lghandle, config.trackinputpin[0],  Track_1_ISR_DUMMY, &userdata_1);
		if (lgret < 0) 
		{
			printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
			yaGpioGetMode(lghandle,config.trackinputpin[0]); 
			return 1; }

		lgret = lgGpioSetAlertsFunc(lghandle, config.trackinputpin[1],  Track_2_ISR_DUMMY, &userdata_2);
		if (lgret < 0) 
		{
			printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
			yaGpioGetMode(lghandle,config.trackinputpin[1]); 
			return 1; }

		lgret = lgGpioSetAlertsFunc(lghandle, config.trackinputpin[2],  Track_3_ISR_DUMMY, &userdata_3);
		if (lgret < 0) 
		{
			printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
			yaGpioGetMode(lghandle,config.trackinputpin[2]); 
			return 1; }

		lgret = lgGpioSetAlertsFunc(lghandle, config.trackinputpin[3],  Track_4_ISR_DUMMY, &userdata_4);
		if (lgret < 0) 
		{
			printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
			yaGpioGetMode(lghandle,config.trackinputpin[3]); 
			return 1; }

	}

	lgret = lgGpioClaimAlert(lghandle, config.trackinputpud[0], config.trackinputevent[0], config.trackinputpin[0], -1); 
	if (lgret < 0) 
	{
		printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
		yaGpioGetMode(lghandle,config.trackinputpin[0]); 
		return 1; }

	lgret = lgGpioClaimAlert(lghandle, config.trackinputpud[1], config.trackinputevent[1], config.trackinputpin[1], -1); 
	if (lgret < 0) 
	{
		printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
		yaGpioGetMode(lghandle,config.trackinputpin[1]); 
		return 1; }

	lgret = lgGpioClaimAlert(lghandle, config.trackinputpud[2], config.trackinputevent[2], config.trackinputpin[2], -1); 
	if (lgret < 0) 
	{
		printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
		yaGpioGetMode(lghandle,config.trackinputpin[2]); 
		return 1; }

	lgret = lgGpioClaimAlert(lghandle, config.trackinputpud[3], config.trackinputevent[3], config.trackinputpin[3], -1); 
	if (lgret < 0) 
	{
		printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
		yaGpioGetMode(lghandle,config.trackinputpin[3]); 
		return 1; }

	printf("Setting up GPIO's for output lines ....\n");

	if( config.trackcurrentoutput[0] >=0)
	{
		for(i=0;i<TRACKLIM;i++)
		{
			if(config.trackcurrentoutput[0] == config.trackinputpin[i])
			{
				printf("TrackCurrentOutput 1 is already an input (%d)\n",config.trackcurrentoutput[0]);
				exit(-1);
			}
		}
//		#ifdef WIRINGPI
//		pinMode(config.trackcurrentoutput[0], OUTPUT);
//		#endif /* WIRINGPI */
	
//		#ifdef LGPIO
		lgret = lgGpioClaimOutput(lghandle, 0 /*LFLAGS*/ , config.trackcurrentoutput[0], LG_LOW);
		if (lgret < 0) 
		{
			printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
			yaGpioGetMode(lghandle,config.trackcurrentoutput[0]); 
			return 1; }
//		#endif /* LGPIO */
	
		printf("TrackCurrentOutput 1 set to GPIO Pin %d\n",config.trackcurrentoutput[0]);

		if(config.trackpoweractive == 1)
			printf("Track power switching activated!\n");

	}

	if( config.trackcurrentoutput[1] >=0)
	{
		for(i=0;i<TRACKLIM;i++)
		{
			if(config.trackcurrentoutput[1] == config.trackinputpin[i])
			{
				printf("TrackCurrentOutput 2 is already an input (%d)\n",config.trackcurrentoutput[1]);
				exit(-1);
			}
		}

//		#ifdef WIRINGPI
//		pinMode(config.trackcurrentoutput[1], OUTPUT);
//		#endif /* WIRINGPI */

//		#ifdef LGPIO
		lgret = lgGpioClaimOutput(lghandle, 0 /*LFLAGS*/ , config.trackcurrentoutput[1], LG_LOW);
		if (lgret < 0) 
		{
			printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
			yaGpioGetMode(lghandle,config.trackcurrentoutput[1]); 
			return 1; }
//		#endif /* LGPIO */

		printf("TrackCurrentOutput 2 set to GPIO Pin %d\n",config.trackcurrentoutput[1]);

	}

	if( config.trackcurrentoutput[2] >=0)
	{
		for(i=0;i<TRACKLIM;i++)
		{
			if(config.trackcurrentoutput[2] == config.trackinputpin[i])
			{
				printf("TrackCurrentOutput 3 is already an input (%d)\n",config.trackcurrentoutput[2]);
				exit(-1);
			}
		}

//		#ifdef WIRINGPI
//		pinMode(config.trackcurrentoutput[2], OUTPUT);
//		#endif /* WIRINGPI */

//		#ifdef LGPIO
		lgret = lgGpioClaimOutput(lghandle, 0 /*LFLAGS*/ , config.trackcurrentoutput[2], LG_LOW);
		if (lgret < 0) 
		{
			printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
			yaGpioGetMode(lghandle,config.trackcurrentoutput[2]); 
			return 1; }
//		#endif /* LGPIO */
		
		printf("TrackCurrentOutput 3 set to GPIO Pin %d\n",config.trackcurrentoutput[2]);

	}

	if( config.trackcurrentoutput[3] >=0)
	{
		for(i=0;i<TRACKLIM;i++)
		{
			if(config.trackcurrentoutput[3] == config.trackinputpin[i])
			{
				printf("TrackCurrentOutput 4 is already an input (%d)\n",config.trackcurrentoutput[3]);
				exit(-1);
			}
		}

//		#ifdef WIRINGPI
//		pinMode(config.trackcurrentoutput[3], OUTPUT);
//		#endif /* WIRINGPI */
		
//		#ifdef LGPIO
		lgret = lgGpioClaimOutput(lghandle, 0 /*LFLAGS*/ , config.trackcurrentoutput[3], LG_LOW);
		if (lgret < 0) 
		{
			printf("ERROR: %s (%d)\n", lguErrorText(lgret), lgret);
			yaGpioGetMode(lghandle,config.trackcurrentoutput[3]); 
			return 1; }
//		#endif /* LGPIO */

		printf("TrackCurrentOutput 4 set to GPIO Pin %d\n",config.trackcurrentoutput[3]);

	}

	if(config.trackpoweractive == 0) {
	/* set timing overhed to zero, because it makes no sense */
		config.panicdelaytime = 0;  /* panic delay for timing end to zero */
		config.timeraceenddelaytime = 0; /* race delay for timing end to zero */
		printf("Track power not activated, panicdelaytime and timeraceenddelaytime set to zero\n");

	}

	#endif  /* LGPIO  */


	/* start traffic light hardware */

	#ifdef I2C
	printf("Starting traffic light hardware ....\n");
	printf("Setting up MCP23017 GPIO's ....\n");
	if( yaMCP23017Setup(NumberOfMCP23017) != 32)   /* place togeter lgpio andWiringpi (later), can return <0 for errors, and bits as usable, only lgpio later */
	{
		printf(" i2c MCP23017 error\n");
		printf(" -> Is the i2c interface already in action ?\n");
		return 1;				/* exit here required to prevent confusion */
	}
	else printf(" i2c setup with device %s is o.k.\n",I2CDEVICENAME);
	#endif /* I2C */

	#ifdef MCP23017
	printf("Starting traffic light hardware ....\n");
	printf("Setting up MCP23017 GPIO's ....\n");

	
	mcp23017Setup(MCP23017_pinBase + 0  , MC23017_ADDR_IC00);  	/* first MCP23017 with 0x00 address */
	mcp23017Setup(MCP23017_pinBase + 16 , MC23017_ADDR_IC01);	/* second MCP23017 with 0x01 address */

	for (i = 0 ; i <= 31 ; i++)					/* setup for 20 LED's inside and one on the backside */
		pinMode(MCP23017_pinBase + i, OUTPUT) ;

	for (i = 0 ; i <= 31 ; i++)					/* setup for 20 LED's inside and one on the backside */
		digitalWrite(MCP23017_pinBase + i,MCP23017_OFF);

	#endif  /* #ifdef MCP23017 */

	#ifdef MCP23017EASY
	printf("Starting traffic light hardware ....\n");
	printf("Setting up MCP23017EASY GPIO's ....\n");

	mcp23017Setup(MCP23017_pinBase + 0  , MC23008_ADDR_IC00);  	/* first MCP23017 with 0x00 address */

	for (i = 0 ; i <= 7 ; i++)					/* setup for 8 LED's inside */
		pinMode(MCP23017_pinBase + i, OUTPUT) ;

	for (i = 0 ; i <= 7 ; i++)					/* setup for 8 LED's inside */
		digitalWrite(MCP23017_pinBase + i,MCP23017_OFF);

	#endif /* #ifdef MCP23017EASY */

	#ifdef MCP23008
	printf("Starting traffic light hardware ....\n");
	printf("Setting up MCP23008 GPIO's ....\n");

	mcp23008Setup(MCP23017_pinBase + 0  , MC23008_ADDR_IC00);  	/* first MCP23008 with 0x00 address */

	for (i = 0 ; i <= 7 ; i++)					/* setup for 8 LED's inside */
		pinMode(MCP23017_pinBase + i, OUTPUT) ;

	for (i = 0 ; i <= 7 ; i++)					/* setup for 8 LED's inside */
		digitalWrite(MCP23017_pinBase + i,MCP23017_OFF);

	#endif /* #ifdef MCP23008 */

	#ifdef SN3218
	printf("Starting traffic light hardware ....\n");
	printf("Setting up SN3218 ....\n");

	sn3218Setup(MCP23017_pinBase);

	for (i = 0 ; i <= 17 ; i++)					/* setup for 18 LED's connected back to ZERO */
		analogWrite(MCP23017_pinBase + i, SN3218_OFF);

	#endif

	/* checking the path at the beginning to prevent surprises */
	if(PathExists(config.resultfile_path) == 0)
	{
		printf("result data file path [%s] found\n",config.resultfile_path);
	}
	else
	{
		fprintf(stderr,"result data file path [%s] not found\n",config.resultfile_path);
		exit(0);
	}

			if(config.soundactive == 1) { /* make some noise, it track power is switched off */
#ifdef ALSA_SOUND
			printf("Setting up ALSA sound system for some noise\n");
			SND_setup();
#endif /* ALSA_SOUND */
			}


	/* installing the signal event handler for correct exit */

	signal(SIGTERM,signal_term);
	signal(SIGINT,signal_int);

	/* If the hardware check flag is set to 1, do an endless loop */
	if(hardwarecheck == 1) {


	/*
		must be done for all tracks and libs
	*/

		if( (config.trackcurrentoutput[0] >=0) && (config.trackpoweractive == 1) )
		{
			yaDigitalWrite(config.trackcurrentoutput[0],LG_HIGH); /* switching power on */

			printf("Track 1 Power on\n");
		}

		if( (config.trackcurrentoutput[1] >=0) && (config.trackpoweractive == 1) )
		{
			yaDigitalWrite(config.trackcurrentoutput[1],LG_HIGH); /* switching power on */

			printf("Track 2 Power on\n");
		}

		if( (config.trackcurrentoutput[2] >=0) && (config.trackpoweractive == 1) )
		{
			yaDigitalWrite(config.trackcurrentoutput[2],LG_HIGH); /* switching power on */

			printf("Track 3 Power on\n");
		}

		if( (config.trackcurrentoutput[3] >=0) && (config.trackpoweractive == 1) )
		{
			yaDigitalWrite(config.trackcurrentoutput[3],LG_HIGH); /* switching power on */

			printf("Track 4 Power on\n");
		}

//		#ifdef WIRINGPI
//		delay(500);  /* delay between power and sound */
//		#endif
//		#ifdef LGPIO
//		lguSleep(0.5);
//		#endif

		yaSleep(0.5);

		if(config.soundactive == 1){
			printf("HEY,  - Check this sound - if compiled in.....\n");
			for(j=0;j < SND_NUMBER_OF_TONES ; j++) {
			printf(" sound %d ",j+1);
			switch(j) {
			case 0:
				printf("\"power off\"");
			break;
			case 1:
				printf("\"power on\"");
			break;
			case 2:
				printf("\"valid timing\"");
			break;
			case 3:
				printf("\"non valid timing\"");
			break;
			case 4:
				printf("\"timing end after race time\"");
			break;
			default:
				printf("\"unknown\"");
			break;
			}
			printf("\n");
#ifdef ALSA_SOUND
			SND_play(j+1);
#endif /* ALSA_SOUND */
//			#ifdef WIRINGPI
//			delay(750);  /* delay between sounds */
//			#endif
//			#ifdef LGPIO
//			lguSleep(0.75);
//			#endif
			
			yaSleep(1.00);


			}
			printf("sound done\n");

		}

		printf("starting (nearly) endless hardware check modus,\nend with CTRL-C\nand wait......\n");

		while( stop == 0) { /* the hardware test core starts here */

	#ifdef I2C
	yaSetMCP23017(MCP23017_BACKSIDE, YA_HIGH);  /* Set trackpower LED high */
	#endif /* I2C */

	#ifdef WIRINGPI

	#ifdef MCP23017

	for(j=0;j<32;j++) {
			digitalWrite(MCP23017_pinBase + j ,MCP23017_ON);
			delay(50);
			digitalWrite(MCP23017_pinBase + j ,MCP23017_OFF);			
			delay(50);
			digitalWrite(MCP23017_pinBase + j ,MCP23017_ON);
			delay(150);
			digitalWrite(MCP23017_pinBase + j ,MCP23017_OFF);			
			delay(20);
			if (stop != 0) /* just for a quicker exit */
				break;
			}

			digitalWrite(MCP23017_pinBase + MCP23017_BACKSIDE,MCP23017_ON);	/* set alive LED to 1 */
			digitalWrite(MCP23017_pinBase + MCP23017_RED_1,MCP23017_ON);
			digitalWrite(MCP23017_pinBase + MCP23017_RED_4,MCP23017_ON);
			digitalWrite(MCP23017_pinBase + MCP23017_RED_6,MCP23017_ON);
			digitalWrite(MCP23017_pinBase + MCP23017_RED_8,MCP23017_ON);
			digitalWrite(MCP23017_pinBase + MCP23017_RED_9,MCP23017_ON);
			digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_2,MCP23017_ON);
			digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_4,MCP23017_ON);
			digitalWrite(MCP23017_pinBase + MCP23017_GREEN_3,MCP23017_ON);

			delay(1250*2);

			digitalWrite(MCP23017_pinBase + MCP23017_BACKSIDE,MCP23017_OFF);/* set alive LED to 0 */
			digitalWrite(MCP23017_pinBase + MCP23017_RED_1,MCP23017_OFF);
			digitalWrite(MCP23017_pinBase + MCP23017_RED_4,MCP23017_OFF);
			digitalWrite(MCP23017_pinBase + MCP23017_RED_6,MCP23017_OFF);
 			digitalWrite(MCP23017_pinBase + MCP23017_RED_8,MCP23017_OFF);
			digitalWrite(MCP23017_pinBase + MCP23017_RED_9,MCP23017_OFF);
			digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_2,MCP23017_OFF);
			digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_4,MCP23017_OFF);
			digitalWrite(MCP23017_pinBase + MCP23017_GREEN_3,MCP23017_OFF);

			delay(20*2);

		/******************************************************************************************/
		/* switch starting traffic light off here */

		for (i = 0 ; i <= 31 ; i++)						/* clear all LEDs */
			digitalWrite(MCP23017_pinBase + i,MCP23017_OFF);
	#endif /* if MCP23017 */

	#ifdef MCP23008

	for(j=0;j<8;j++) {
			digitalWrite(MCP23017_pinBase + j ,MCP23017_ON);
			delay(150);
			digitalWrite(MCP23017_pinBase + j ,MCP23017_OFF);			
			delay(150);
			digitalWrite(MCP23017_pinBase + j ,MCP23017_ON);
			delay(550);
			digitalWrite(MCP23017_pinBase + j ,MCP23017_OFF);			
			delay(20);
			if (stop != 0) /* just for a quicker exit */
				break;
			}

		/******************************************************************************************/
		/* switch starting traffic light off here */

		for (i = 0 ; i <= 8 ; i++)						/* clear all LEDs */
			digitalWrite(MCP23017_pinBase + i,MCP23017_OFF);
	#endif /* if MCP23008 */

		/******************************************************************************************/

	#endif /* WIRINGPI */

		yaSleep(0.5);

		}

		printf("CTRL-C exit from yast hardware check.....\n");

		if( (config.trackcurrentoutput[0] >=0) && (config.trackpoweractive == 1) )
		{
			yaDigitalWrite(config.trackcurrentoutput[0],LG_LOW); /* switching power off */

			printf("Track 1 Power off\n");
		}

		if( (config.trackcurrentoutput[1] >=0) && (config.trackpoweractive == 1) )
		{
			yaDigitalWrite(config.trackcurrentoutput[1],LG_LOW); /* switching power off */

			printf("Track 2 Power off\n");
		}

		if( (config.trackcurrentoutput[2] >=0) && (config.trackpoweractive == 1) )
		{
			yaDigitalWrite(config.trackcurrentoutput[2],LG_LOW); /* switching power off */

			printf("Track 3 Power off\n");
		}

		if( (config.trackcurrentoutput[3] >=0) && (config.trackpoweractive == 1) )
		{
			yaDigitalWrite(config.trackcurrentoutput[3],LG_LOW); /* switching power off */

			printf("Track 4 Power off\n");
		}

	#ifdef I2C
	yaSetMCP23017(MCP23017_BACKSIDE, YA_LOW);  /* Set trackpower LED low */
	#endif /* I2C */

		exit(0);

	}

	/* Reset all variables inside Array at the beginning*/
	printf("starting yast environment and display.....\n");

	RESETALLTRACKS

	for(j=0;j<TRACKLIM;j++) {
		overallfastest[j] = MAXLAPTIME;  		/* beware empty */
	}
	readback_environment(config.storagefile_name, config.minlaptime);

	srandom(GetTime());  /* set a new random sequence */

	/* set up the screen, from here no longer printf()'s */
//	exit(0);

	initscr();
	nodelay(stdscr, TRUE);
	noecho();
	cbreak();
	nonl();
	keypad(stdscr, TRUE);
	curs_set(0);

	if (has_colors())
	{
		start_color();
		if ( (COLORS >= 8) && (COLOR_PAIRS >= 16) ) {
			init_pair(COLOR_PAIR_BASIC  , COLOR_WHITE         , COLOR_BLACK); /* BASIC WHITE */
			init_pair(COLOR_PAIR_TRACK_1, config.trackcolor[0], COLOR_BLACK); /* Track 1 */
			init_pair(COLOR_PAIR_TRACK_2, config.trackcolor[1], COLOR_BLACK); /* Track 2 */
			init_pair(COLOR_PAIR_TRACK_3, config.trackcolor[2], COLOR_BLACK); /* Track 3 */
			init_pair(COLOR_PAIR_TRACK_4, config.trackcolor[3], COLOR_BLACK); /* Track 4 */
			init_pair(COLOR_PAIR_TRACK_5, COLOR_MAGENTA       , COLOR_BLACK); /* Track 5 */
			init_pair(COLOR_PAIR_TRACK_6, COLOR_CYAN          , COLOR_BLACK); /* Track 6 */
			init_pair(COLOR_PAIR_TRACK_7, COLOR_RED           , COLOR_BLACK); /* Track 7 */
			init_pair(COLOR_PAIR_TRACK_8, COLOR_BLUE          , COLOR_BLACK); /* Track 8 */
			init_pair(COLOR_PAIR_RED    , COLOR_RED           , COLOR_BLACK); /* BASIC RED */
			init_pair(COLOR_PAIR_GREEN  , COLOR_GREEN         , COLOR_BLACK); /* BASIC GREEN */
			init_pair(COLOR_PAIR_YELLOW , COLOR_YELLOW        , COLOR_BLACK); /* BASIC YELLOW */
			init_pair(COLOR_PAIR_BLUE   , COLOR_BLUE          , COLOR_BLACK); /* BASIC BLUE */
			bkgd(COLOR_PAIR(COLOR_PAIR_BASIC));
		}
		else {  /* no colors found */
		endwin();
		printf("ncurses fails, COLORS=%d (>=8), COLOR_PAIRS=%d (>=16)\n",COLORS,COLOR_PAIRS); 
		exit(0);
		}
	}
	else {  /* no colors found */
		endwin();
		printf("ncurses fails, has no colors\n"); 
		exit(0);
	}

	/*  Do something here before startup the main screen */

	getmaxyx(stdscr, display_y, display_x);

	if(stop == 1) {
		endwin();
		printf("ncurses Window Size is %d x %d chars.\n",display_x,display_y);
		printf("ncurses has COLORS=%d and COLOR_PAIRS=%d.\n",COLORS,COLOR_PAIRS); 
		printf("exiting while option -vv ..................\n");
		exit(0);			/* don't go into main loop, because -vv */
		}

	/* automatic height control for time / lap to go */

	if ((display_y - 3 - CURSES_TOGO_LINE_Y) > 3)
		timetogoheight = 3;
	else
		timetogoheight = display_y - 3 - CURSES_TOGO_LINE_Y;

	if ((display_y - 3 - CURSES_TOGO_LINE_Y) < 0)
		timetogoheight = 0;

	if (display_x < 80) {
		fprintf(stderr,"please use a display wide with minimum 80 chars\n");
		fprintf(stderr,"you are using %d chars wide\n",display_x);
		/* exit(0); */
	}

	if (display_y < 36) {
		fprintf(stderr,"please use a display height with minimum 36 chars\n");
		fprintf(stderr,"you are using %d chars height\n",display_y);
		/* exit(0); */
	}

	#ifdef DISPLAYTEST

	nodelay(stdscr, FALSE);
	clear();

	mvprintw(0,0,"Window Size: %d x %d\n",display_x,display_y);

	TrafficLight(G_GREEN);

	move(2,0);  /* (y,x) */
	for (i=0;i<256;i++) {
		printw("%c",i);
	}

	for(i=0;i<=9;i++)
		mvprintnum3x5(8,i*5,i);

	for(i=0;i<=9;i++)
		mvprintnum3x5r(14,i*5,i);

	for(i=0;i<=9;i++)
		mvprintnum3x3(20,i*5,i);

	getch();
	nodelay(stdscr, TRUE);

	#endif /* DISPLAYTEST */

	printd("searching for next free filename.....\n");
	datafilenamecount = find_free_datafilename(config.resultfile_name,config.resultfile_path);
	printd("returned file is: %d\n",datafilenamecount);

	if ( datafilenamecount == -1 ) {
		fprintf(stderr,"There is no free data file slot inside the %d filespace, please clear first......\n",MAXNAMECOUNT);
	}
	printd("next free filespace is known as: %d\n",datafilenamecount);

	/* now I start up the main screen */

	clear();

	mvprintw(0,0,"track  lap          last lap time [s]        best lap time [s]      last / place");
	/*	mvprintw(0,0,"track  lap          last lap time [s]        best lap time [s]      place/behind"); */
	/*	mvprintw(0,0,"tra    lap          last lap time [s]        best lap time [s]      place/behind"); */ /* Just another idea */
	/*	mvprintw(1,0,"ck"); */

	if(endlap > 0)
		printmessage("Race over %d laps with %d tracks",endlap,config.numberoftracks);
	if(endtime > 0)
		printmessage("Race over %d seconds (%dh:%.02dm:%02ds) with %d tracks",endtime,(int)floorf((float)endtime / 3600.0),((int)(floorf(endtime/60.0)))%60,((int)(floorf(endtime)))%60,config.numberoftracks);

	for(i=0;i<TRACKLIM;i++)
		timingactive[i] = 1;                /* start looking for track events from now*/
	screenupdate = 1; 				/* do the first draw */

	set_trackcurrent(1,config.trackpoweractive);  /* start up the track current at the beginning */

	starttime = GetTime();

	while( stop == 0) { /* the real core starts here */

		#ifdef WIRINGPI
		delay(10);  /* minimize the load from 1.0 down to less 0.1 */
		#endif /* WIRINGPI */
		#ifdef LGPIO
		lguSleep(0.01);
		#endif /* LGPIO */

		/* mvprintw(CUR_STATE_Y,CUR_STATE_X,"State:%.2d",gostate); */
		mvprintw(CUR_STATE_Y,CUR_STATE_X,"S:%.2d",gostate);

		/* Print out the time */
		acttime = GetTime() - starttime;
		mvprintw(CUR_CLOCK_Y,CUR_CLOCK_X,"%.2ld:%.2ld:%.2ld:%.3ld",(acttime/(1000*60*60))%100,(acttime/(1000*60))%60,(acttime/1000)%60,acttime%1000);

		/* make some times a fully screen painting update -> required for RTC, nice to have else */
		/* was %1000, now %250 for slow pi1, because the 1000 is just 60 times a second possible */
		if( (acttime%250) == 0)   /* fit some times by random, for my Pi approx some times a minute */
		{
			screenupdate = 1;
			/* SND_play(4);  make some noise for testing */
		}

		/* calculate and view time to go, but only while race */
		/* will be overwritten with checkered, green, yellow or red flag   */
		if( (gostate == G_RACE) && (endtime > 0) ){
			if(panictime == 0)
				timetogo = (unsigned long)endtime - (unsigned long)(acttime/1000) ;
			print_timetogo(timetogo,timetogoheight);
		} /* gostate == G_RACE */

		/* redraw the result display */
		if(screenupdate == 1){

			/* calculate lap to go, but only while race and at screen update */
			/* will be overwritten with checkered, green, yellow or red flag   */
			if( (gostate == G_RACE) && (endlap > 0) ) {
				lapstogo = 0;
				for(i=0;i<TRACKLIM;i++){
					if(lapcount[i] > lapstogo)
						lapstogo = lapcount[i];
				}
				if(lapstogo > endlap) lapstogo = endlap;
				print_laptogo((unsigned long)endlap - lapstogo,timetogoheight);
			} /* gostate == G_RACE */

			/* bring everything to display */
			for (i=0;i< config.numberoftracks /* TRACKLIM */ ;i++) {
				print_lap(i,lapcount[i]);

				if(lapcount[i] > 0)
					print_last(i,lasttime[i]);
				else
					print_last(i,reactiontime[i]);

				print_fastes(i,fasttime[i]);
			}

			view_last(config.numberoftracks);
			view_evaluation(config.numberoftracks);

			view_overall(config.numberoftracks);
			view_drivernames(config.numberoftracks);

			
			if(ISR_Signal_Falgs != 0) {
				printd("ISR message found - 0x%lx -\n",ISR_Signal_Falgs);

				if ( (ISR_Signal_Falgs & TRACKEVENT_1_TOO_FAST) == TRACKEVENT_1_TOO_FAST) {
					printmessage("Trackevent Track 1 too fast (%ld ms)",secondtime[0] - firsttime[0]);
					ISR_Signal_Falgs = ISR_Signal_Falgs & ~(TRACKEVENT_1_TOO_FAST);
					ISR_Signal_Falgs = ISR_Signal_Falgs & ~(TRACKEVENT_1);
					if(config.soundactive == 1) {
					#ifdef ALSA_SOUND
					SND_play(4);  /* not valid */
					#endif /* ALSA_SOUND */
					}

				}

				if ( (ISR_Signal_Falgs & LAP_LIMIT_TRACK_1) == LAP_LIMIT_TRACK_1) {
					printmessage("LAP LIMIT on Track 1");
					ISR_Signal_Falgs = ISR_Signal_Falgs & ~(LAP_LIMIT_TRACK_1);
				}

				if ( (ISR_Signal_Falgs & TRACKEVENT_2_TOO_FAST) == TRACKEVENT_2_TOO_FAST) {
					printmessage("Trackevent Track 2 too fast (%ld ms)",secondtime[1] - firsttime[1]);
					ISR_Signal_Falgs = ISR_Signal_Falgs & ~(TRACKEVENT_2_TOO_FAST);
					ISR_Signal_Falgs = ISR_Signal_Falgs & ~(TRACKEVENT_2);
					if(config.soundactive == 1) {
					#ifdef ALSA_SOUND
					SND_play(4);  /* not valid */
					#endif /* ALSA_SOUND */
					}
				}

				if ( (ISR_Signal_Falgs & LAP_LIMIT_TRACK_2) == LAP_LIMIT_TRACK_2) {
					printmessage("LAP LIMIT on Track 2");
					ISR_Signal_Falgs = ISR_Signal_Falgs & ~(LAP_LIMIT_TRACK_2);
				}

				if ( (ISR_Signal_Falgs & TRACKEVENT_3_TOO_FAST) == TRACKEVENT_3_TOO_FAST) {
					printmessage("Trackevent Track 3 too fast (%ld ms)",secondtime[2] - firsttime[2]);
					ISR_Signal_Falgs = ISR_Signal_Falgs & ~(TRACKEVENT_3_TOO_FAST);
					ISR_Signal_Falgs = ISR_Signal_Falgs & ~(TRACKEVENT_3);
					if(config.soundactive == 1) {
					#ifdef ALSA_SOUND
					SND_play(4);  /* not valid */
					#endif /* ALSA_SOUND */
					}
				}

				if ( (ISR_Signal_Falgs & LAP_LIMIT_TRACK_3) == LAP_LIMIT_TRACK_3) {
					printmessage("LAP LIMIT on Track 3");
					ISR_Signal_Falgs = ISR_Signal_Falgs & ~(LAP_LIMIT_TRACK_3);
				}

				if ( (ISR_Signal_Falgs & TRACKEVENT_4_TOO_FAST) == TRACKEVENT_4_TOO_FAST) {
					printmessage("Trackevent Track 4 too fast (%ld ms)",secondtime[3] - firsttime[3]);
					ISR_Signal_Falgs = ISR_Signal_Falgs & ~(TRACKEVENT_4_TOO_FAST);
					ISR_Signal_Falgs = ISR_Signal_Falgs & ~(TRACKEVENT_4);
					if(config.soundactive == 1) {
					#ifdef ALSA_SOUND
					SND_play(4);  /* not valid */
					#endif /* ALSA_SOUND */
					}
				}

				if ( (ISR_Signal_Falgs & LAP_LIMIT_TRACK_4) == LAP_LIMIT_TRACK_4) {
					printmessage("LAP LIMIT on Track 4");
					ISR_Signal_Falgs = ISR_Signal_Falgs & ~(LAP_LIMIT_TRACK_4);
				}

				if ( (ISR_Signal_Falgs & TRACKEVENT_1) == TRACKEVENT_1) {
				ISR_Signal_Falgs = ISR_Signal_Falgs & ~(TRACKEVENT_1);
				if(config.soundactive == 1) {
					#ifdef ALSA_SOUND
					SND_play(3);  /* valid */
					#endif /* ALSA_SOUND */
					}
				}

				if ( (ISR_Signal_Falgs & TRACKEVENT_2) == TRACKEVENT_2) {
				ISR_Signal_Falgs = ISR_Signal_Falgs & ~(TRACKEVENT_2);
				if(config.soundactive == 1) {
					#ifdef ALSA_SOUND
					SND_play(3);
					#endif /* ALSA_SOUND */
					}
				}

				if ( (ISR_Signal_Falgs & TRACKEVENT_3) == TRACKEVENT_3) {
				ISR_Signal_Falgs = ISR_Signal_Falgs & ~(TRACKEVENT_3);
				if(config.soundactive == 1) {
					#ifdef ALSA_SOUND
					SND_play(3);
					#endif /* ALSA_SOUND */
					}
				}

				if ( (ISR_Signal_Falgs & TRACKEVENT_4) == TRACKEVENT_4) {
				ISR_Signal_Falgs = ISR_Signal_Falgs & ~(TRACKEVENT_4);
				if(config.soundactive == 1) {
					#ifdef ALSA_SOUND
					SND_play(3);
					#endif /* ALSA_SOUND */
					}
				}
			}

			/* view next free slot for filename (4 digits) or force or error or.... */
			switch(config.outfile) {
			case 0:
				mvprintw(CURSES_FILE_Y,CURSES_FILE_X,"F:none");
				break;
			case 1:	
				if(datafilenamecount >= 0)
					mvprintw(CURSES_FILE_Y,CURSES_FILE_X,"F:%.4d",datafilenamecount);
				else
					mvprintw(CURSES_FILE_Y,CURSES_FILE_X,"F:ERR!");
				break;
			case 2:
				mvprintw(CURSES_FILE_Y,CURSES_FILE_X,"Force ");
				break;
			
			default:
				mvprintw(CURSES_FILE_Y,CURSES_FILE_X,"unknown");
				break;
			}
			
			if ((config.rtc_view == 1) && ( (gostate == G_WAIT) || (gostate == G_GREEN_FLAG) || (gostate == G_RED_FLAG) || (gostate == G_YELLOW_FLAG) ) )
			{
				time( &rawtime );
				timeinfo = localtime( &rawtime );  	/* get time zone information out of the environment */
				print_realtime(timeinfo->tm_hour, timeinfo->tm_min, timetogoheight);
			}

			screenupdate = 0;  /* I don't remember the reason for this */
		}

		/* state - engine */

		if((gostate > G_WAIT) && (gostate <= G_GREEN)) {

			if(failstartpoweroff == 1 )
			{
				/* This should be a fail start mode */
				if((reactiontime[0] > 0 ) && (timingactive[0]))
				{
					for(i=0;i<TRACKLIM;i++)
						timingactive[i] = 0;
					set_trackcurrent(0,config.trackpoweractive);
					gostate = G_WAIT;
					TrafficLight(G_YELLOW_FLAG);
					TrafficLightHard(G_YELLOW_FLAG);
					printmessage("Fail start on Track 1");
				}
				if((reactiontime[1] > 0) && (timingactive[1]))
				{
					for(i=0;i<TRACKLIM;i++)
						timingactive[i] = 0;
					set_trackcurrent(0,config.trackpoweractive);
					gostate = G_WAIT;
					TrafficLight(G_YELLOW_FLAG);
					TrafficLightHard(G_YELLOW_FLAG);
					printmessage("Fail start on Track 2");
				}
				if((reactiontime[2] > 0) && (timingactive[2]))
				{
					for(i=0;i<TRACKLIM;i++)
						timingactive[i] = 0;
					set_trackcurrent(0,config.trackpoweractive);
					gostate = G_WAIT;
					TrafficLight(G_YELLOW_FLAG);
					TrafficLightHard(G_YELLOW_FLAG);
					printmessage("Fail start on Track 3");
				}
				if((reactiontime[3] > 0) && (timingactive[3]))
				{
					for(i=0;i<TRACKLIM;i++)
						timingactive[i] = 0;
					set_trackcurrent(0,config.trackpoweractive);
					gostate = G_WAIT;
					TrafficLight(G_YELLOW_FLAG);
					TrafficLightHard(G_YELLOW_FLAG);
					printmessage("Fail start on Track 4");
				}
			}

			/* switching G_STATE forward */

			switch(gostate) {
				case G_RED_0:
				if((starttime) < GetTime()) {
					printmessage(" G_RED_0 [.....]");

					TrafficLightMidErase(timetogoheight);
					TrafficLight(gostate);
					TrafficLightHard(gostate);

					set_trackcurrent(1,config.trackpoweractive);  /* Set current on */

					/* prepare here for a new race */
					ClearResults(config.numberoftracks);  		/* clear something of the screen */

					if(config.random_light_startup == 1) {
						random_light_startup_time = 6*STARTLIGHTDELAY + (int)round(random()*STARTLIGHTRANDOM/(RAND_MAX*1.0));  /* 1..3 seconds */
						printd("the random_light_delay_time value in ms -%d-\n",random_light_startup_time);
					}
					else
					random_light_startup_time = 6*STARTLIGHTDELAY;
					gostate ++;
				}
				break;
				case G_RED_1:
				if((starttime + 1*STARTLIGHTDELAY) < GetTime()) {
					printmessage(" G_RED_1 [*....]");
					TrafficLight(gostate);
					TrafficLightHard(gostate);
					gostate ++;
				}
				break;
				case G_RED_2:
				if((starttime + 2*STARTLIGHTDELAY) < GetTime()) {
					printmessage(" G_RED_2 [**...]");
					TrafficLight(gostate);
					TrafficLightHard(gostate);
					gostate ++;
				}
				break;
				case G_RED_3:
				if((starttime + 3*STARTLIGHTDELAY) < GetTime()) {
					printmessage(" G_RED_3 [***..]");
					TrafficLight(gostate);
					TrafficLightHard(gostate);
					gostate ++;
				}
				break;
				case G_RED_4:
				if((starttime + 4*STARTLIGHTDELAY) < GetTime()) {
					printmessage(" G_RED_4 [****.]");
					TrafficLight(gostate);
					TrafficLightHard(gostate);
					gostate ++;
				}
				break;
				case G_RED_5:
				if((starttime + 5*STARTLIGHTDELAY) < GetTime()) {
					printmessage(" G_RED_5 [*****]");
					TrafficLight(gostate);
					TrafficLightHard(gostate);
					gostate ++;
					gostate ++;	/* just in for test reasons, because blank is opsolete */
				}
				break;
				case G_BLANK:
				if((starttime + 6*STARTLIGHTDELAY) < GetTime()) {
					printmessage(" G_BLANK [.....]");
					TrafficLight(gostate);
					TrafficLightHard(gostate);
					gostate ++;
				}
				break;
				case G_GREEN:
				if((starttime + random_light_startup_time) < GetTime()) {
					printmessage(" G_GREEN [*****]");

					RESETALLTRACKS

					overallpanictime = 0;		/* reset the complete panic vars */
					panictime = 0;
					numberofpanics = 0;

					for(i=0;i<TRACKLIM;i++)
						timingactive[i] = 1; 	/* start looking for track events from now*/
					screenupdate = 1; 		/* and bring the differences to the outer world */
					TrafficLight(gostate);
					TrafficLightHard(gostate);
					gostate ++;
					starttime = GetTime();		/* Race from now */
					printmessage(" --- RACE STARTED ---");
					TrafficLightMidErase(timetogoheight);
					racedatawrittentofile = 0;	/* make sure to write next time */
				}
				break;
				case G_WAIT:	/* do nothing, but do not exit */
				break;
				default:
				fprintf(stderr,"STATE Engine: state NOT defined (%d)...exiting...",gostate);

				exit(1);
				break;
			}
		}

		if(gostate == G_RACE)  {
			/* RACE end with endlap */
			if((endlap > 0) && ( (lapcount[0] >= endlap ) || ( lapcount[1] >= endlap) || ( lapcount[2] >= endlap) || ( lapcount[3] >= endlap) )) {
				if( /* (panictime > 0) && */ (panictimestop >= 1))  		/*  PANIC Button with timeshift should not stop a race */
				{
					printmessage("RACE END delayed by PANIC BUTTON");
				}
				else
				{
					printmessage(" ---- RACE END on ENDLAP ---- ");

					// set_trackcurrent(0);

					gostate ++; 			/* goto state G_ready */
					TrafficLightMidErase(timetogoheight);
					TrafficLight(gostate);
					TrafficLightHard(gostate);

					for(i=0;i<TRACKLIM;i++)
						finishlap[i] = lapcount[i];  /* store the first car in lap in finishlap[];  */
				}
			}

			if((endtime > 0) &&  (((GetTime() - starttime) / 1000) > (endtime-1))) {
				/* RACE end with endtime */
				if( /* (panictime > 0) && */ (panictimestop >= 1))  		/*  PANIC Button with timeshift should not stop a race */
				{
					printmessage("RACE END delayed by PANIC BUTTON");
				}
				else
				{
					printmessage(" ---- RACE END on ENDTIME ----");

					/* Track power off, but timing not -> do later */
					set_trackcurrent(0,config.trackpoweractive);

					gostate ++; 			/* goto state G_checkered */
					TrafficLightMidErase(timetogoheight);
					TrafficLight(gostate);
					TrafficLightHard(gostate);
//					for(i=0;i<TRACKLIM;i++)
//					{
//						finishlap[i] = lapcount[i];         /* store the first car in lap in finishlap[];  */
//						timingactive[i] = 0;                /* stop looking for this track */
//					}
//					carsin = TRACKLIM;
				}
				screenupdate = 1; 				/* and bring the differences to the outer world */
			}

			if(panictimestop == 1) {
				if( (panictime + config.panicdelaytime) < (GetTime() - starttime) ){
					for(i=0;i<TRACKLIM;i++)
						timingactive[i] = 0;                /* ignore all track events from now */
					/* set now the real panic time */
					panictime = GetTime() - starttime;  	/* remember the panic time, later on starttime must be moved to starttime + panictime */
					panictimestop = 2;  /* mark panic as real stopped */
					printmessage("timing delayed by PANIC BUTTON (No.: %d)",numberofpanics);
				}
			}
		}

		if( (gostate == G_CHECKERED_FLAG) && (endlap > 0)  ) {
			if( (lapcount[0] >= endlap) || (lapcount[0] > finishlap[0]) || (reactiontime[0] == 0) ) {
#ifdef FONT3X3
				move(DOTSHIFT - 1 + (0*FONTHEIGHT),CUR_PLACE_X);
#endif
#ifdef FONT3X5
				move(DOTSHIFT + 3 + (0*FONTHEIGHT),CUR_PLACE_X);
#endif
				printw("- finished -");
				if(timingactive[0] == 1)
				{
					finishtime[0] = overalltime[0];  /*  GetTime(); */
					timingactive[0] = 0;                /* stop looking for this track */
					carsin ++;
				}
			}
			if(config.numberoftracks > 1) {
				if( (lapcount[1] >= endlap) || (lapcount[1] > finishlap[1])  || (reactiontime[1] == 0)) {
#ifdef FONT3X3
				move(DOTSHIFT - 1 + (0*FONTHEIGHT),CUR_PLACE_X);
#endif
#ifdef FONT3X5
				move(DOTSHIFT + 3 + (0*FONTHEIGHT),CUR_PLACE_X);
#endif
					printw("- finished -");
					if(timingactive[1] == 1)
					{
						finishtime[1] = overalltime[1];  /* GetTime(); */
						timingactive[1] = 0;                /* stop looking for this track */
						carsin ++;
					}
				}
			}
			if(config.numberoftracks > 2) {
				if( (lapcount[2] >= endlap) || (lapcount[2] > finishlap[2])  || (reactiontime[2] == 0)) {
#ifdef FONT3X3
				move(DOTSHIFT - 1 +(0*FONTHEIGHT),CUR_PLACE_X);
#endif
#ifdef FONT3X5
				move(DOTSHIFT + 3 +(0*FONTHEIGHT),CUR_PLACE_X);
#endif
					printw("- finished -");
					if(timingactive[2] == 1)
					{
						finishtime[2] = overalltime[2];  /* GetTime(); */
						timingactive[2] = 0;                /* stop looking for this track */
						carsin ++;
					}
				}
			}
			if(config.numberoftracks > 3) {
				if( (lapcount[3] >= endlap) || (lapcount[3] > finishlap[3])  || (reactiontime[3] == 0)) {
#ifdef FONT3X3
				move(DOTSHIFT - 1 + (0*FONTHEIGHT),CUR_PLACE_X);
#endif
#ifdef FONT3X5
				move(DOTSHIFT + 3 + (0*FONTHEIGHT),CUR_PLACE_X);
#endif
					printw("- finished -");
					if(timingactive[3] == 1)
					{
						finishtime[3] = overalltime[3];     /* GetTime(); */
						timingactive[3] = 0;                /* stop looking for this track */
						carsin ++;
					}
				}
			}
		}

		if( (gostate == G_CHECKERED_FLAG) && (endtime > 0) ) {

			if( (((GetTime() - starttime - config.timeraceenddelaytime) / 1000) > (endtime-1)) ) {
				printmessage(" ---- TIME RACE TIMING END ----");

				for(i=0; i<TRACKLIM; i++)
				{
					finishlap[i] = lapcount[i];         /* store the first car in lap in finishlap[];  */
					finishtime[i] = overalltime[i];
					timingactive[i] = 0;                /* stop looking for this track */
				}

				for(i=0;i<config.numberoftracks /* TRACKLIM */ ;i++)
				{
#ifdef FONT3X3
					move(DOTSHIFT - 1 + (i*FONTHEIGHT),CUR_PLACE_X);
#endif
#ifdef FONT3X5
					move(DOTSHIFT + 3 + (i*FONTHEIGHT),CUR_PLACE_X);
#endif
					printw("- finished -");
				}

				if( config.soundactive == 1 ) {
					#ifdef ALSA_SOUND
					SND_play(5); /* make some noise */
					#endif /* ALSA_SOUND */
				}

				carsin = TRACKLIM;

			}
			screenupdate = 1; 				/* and bring the differences to the outer world */
		}

		if((gostate == G_CHECKERED_FLAG) && ( carsin >=  config.numberoftracks )) {
			printmessage(" -!-!- ALL CARS IN -!-!-");
			gostate = G_WAIT; 			/* goto state G_ready */

			set_trackcurrent(0,config.trackpoweractive);

			view_evaluation(config.numberoftracks);

			switch(config.outfile) {
			case 1:
				if(write_datafilename(config.resultfile_name, config.resultfile_path, datafilenamecount, endlap, endtime) == 0 )
				{
					printmessage("Timing data file %.04d writing done",datafilenamecount);
					racedatawrittentofile = 1; /* mark written */
				}
				else
				{
					printmessage("file writing gone wrong");
					fprintf(stderr,"file writing gone wrong -%s-%s-\n",config.resultfile_path,config.resultfile_name);
				}

				datafilenamecount = find_free_datafilename(config.resultfile_name,config.resultfile_path);

				if ( datafilenamecount == -1 )
				{
					fprintf(stderr,"There is no free data file slot inside the %d filespace, please clear first......",MAXNAMECOUNT);
				}
				break;
			case 2:				
				if(write_datafilename(config.resultfile_name, "", -1 , endlap, endtime) == 0 )
				{
					printmessage("Timing data file writing done");
					racedatawrittentofile = 1; /* mark written */
				}
				break;
			default:
				break;	
			}
			
			screenupdate = 1;  /* and bring the differences to the outer world */
		}

		/* and from now the keyboard */
		/* IS THIS REALLY REQUIRED IN LIVE MODUS */
		inchr = getch();
		if(inchr)
		switch(inchr){
			case 27:	/* do something special on ALT key on ncurses */
			inchr = getch();
			if(inchr)
			switch(inchr){
				case 49:
				printmessage("Simulate Track 1 event");
				
				#ifdef WIRINGPI
				Track_1_ISR();  /* Simulate Track Pulse */
				#endif /* WIRINGPI */
				
				#ifdef LGPIO
				Track_1_ISR(0, 0, NULL);  /* Simulate Track Pulse */
				#endif /* LGPIO */
				
				break;

				case 50:
				printmessage("Simulate Track 2 event");

				#ifdef WIRINGPI
				Track_2_ISR();  /* Simulate Track Pulse */
				#endif /* WIRINGPI */

				#ifdef LGPIO
				Track_2_ISR(0, 0, NULL);  /* Simulate Track Pulse */
				#endif /* LGPIO */

				break;

				case 51:
				printmessage("Simulate Track 3 event");

				#ifdef WIRINGPI
				Track_3_ISR();  /* Simulate Track Pulse */
				#endif /* WIRINGPI */

				#ifdef LGPIO
				Track_3_ISR(0, 0, NULL);  /* Simulate Track Pulse */
				#endif /* LGPIO */

				break;

				case 52:
				printmessage("Simulate Track 4 event");

				#ifdef WIRINGPI
				Track_4_ISR();  /* Simulate Track Pulse */
				#endif /* WIRINGPI */

				#ifdef LGPIO
				Track_4_ISR(0, 0, NULL);  /* Simulate Track Pulse */
				#endif /* LGPIO */

				break;

				default:
				break;
			}
			break;

			/* OLD STUFF for development under PD CURSES on Windows */
//			#ifdef OFFLINE		/* ALT_X on PD curses only */

//			case ALT_1:
//			printmessage("Simulate Track 1 event");
//			Track_1_ISR();  /* Simulate Track Pulse */
//			break;

//			case ALT_2:
//			printmessage("Simulate Track 2 event");
//			Track_2_ISR();  /* Simulate Track Pulse */
//			break;

//			case ALT_3:
//			printmessage("Simulate Track 3 event");
//			Track_3_ISR();  /* Simulate Track Pulse */
//			break;

//			case ALT_4:
//			printmessage("Simulate Track 4 event");
//			Track_4_ISR();  /* Simulate Track Pulse */
//			break;

//			#endif /* OFFLINE */

			case KEY_F(1):
			if(gostate != G_RACE) {
				for(i=0; i < LAPLIM;i++){ 		/* reset all times */
					laptime[0][i] = 0 ;
				}
				lapcount[0] = 0;
				fasttime[0] = MAXLAPTIME;
				lasttime[0] = 0;
				firsttime[0] = 0;
				secondtime[0] = 0;
				finishtime[0] = 0;
				overalltime[0]= 0;
				reactiontime[0] = 0;
			}
			clear_last(0);
			screenupdate = 1; 				/* and bring the differences to the outer world */
			printmessage("RESET Lapcounter Track 1");
			break;

			case KEY_F(2):
			if(gostate != G_RACE) {
				for(i=0; i < LAPLIM;i++){   	/* reset all times */
					laptime[1][i] = 0 ;
				}
				lapcount[1] = 0;
				fasttime[1] = MAXLAPTIME;
				lasttime[1] = 0;
				firsttime[1] = 0;
				secondtime[1] = 0;
				finishtime[1] = 0;
				overalltime[1]= 0;
				reactiontime[1] = 0;
			}
			clear_last(1);
			screenupdate = 1; 				/* and bring the differences to the outer world */
			printmessage("RESET Lapcounter Track 2");
			break;

			case KEY_F(3):
			if(gostate != G_RACE) {
				for(i=0; i < LAPLIM;i++){ 	/* reset all times */
					laptime[2][i] = 0 ;
				}
				lapcount[2] = 0;
				fasttime[2] = MAXLAPTIME;
				lasttime[2] = 0;
				firsttime[2] = 0;
				secondtime[2] = 0;
				finishtime[2] = 0;
				overalltime[2]= 0;
				reactiontime[2] = 0;

			}
			clear_last(2);
			screenupdate = 1; 				/* and bring the differences to the outer world */
			printmessage("RESET Lapcounter Track 3");
			break;

			case KEY_F(4):
			if(gostate != G_RACE) {
				for(i=0; i < LAPLIM;i++){ 	/* reset all times */
					laptime[3][i] = 0 ;
				}
				lapcount[3] = 0;
				fasttime[3] = MAXLAPTIME;
				lasttime[3] = 0;
				firsttime[3] = 0;
				secondtime[3] = 0;
				finishtime[3] = 0;
				overalltime[3]= 0;
				reactiontime[3] = 0;
			}
			clear_last(3);			
			screenupdate = 1; 				/* and bring the differences to the outer world */
			printmessage("RESET Lapcounter Track 4");
			break;

			/* future use in here */
			case KEY_F(5):
			break;
			case KEY_F(6):
			break;
			case KEY_F(7):
			break;
			case KEY_F(8):
			break;

			case KEY_F(9):
			gostate = G_RED_FLAG;  				/* show red flag */
			for(i=0;i<TRACKLIM;i++)
				timingactive[i] = 0;      		/* ignore al timing events from now */
			TrafficLightMidErase(timetogoheight);
			TrafficLight(gostate);
			TrafficLightHard(gostate);
			set_trackcurrent(0,config.trackpoweractive);
			printmessage("RED FLAG");
			screenupdate = 1;  /* and bring the differences to the outer world */
			break;

			case KEY_F(10):
			gostate = G_YELLOW_FLAG;  			/* show yellow flag */
			TrafficLightMidErase(timetogoheight);
			TrafficLight(gostate);
			TrafficLightHard(gostate);
			for(i=0;i<TRACKLIM;i++)
				timingactive[i] = 1;            /* activate timing again */
			set_trackcurrent(1,config.trackpoweractive);
			printmessage("YELLOW FLAG");
			screenupdate = 1;  /* and bring the differences to the outer world */
			break;

			case 'p':							/* PANIC button pressed here, don't change the go_state */
			if(gostate == G_RACE) {
				if(panictimestop == 0) {
					panictimestop = 1;  /* mark as stop first step */
					TrafficLight(G_YELLOW_FLAG);				/* show yellow flag */
					TrafficLightHard(G_YELLOW_FLAG);
					TrafficLightMidErase(timetogoheight);
					numberofpanics++;						/* add one more panic */
					panictimestop = 1;
					set_trackcurrent(0,config.trackpoweractive);  /* stop current */
					panictime = GetTime() - starttime;  	/* remember the panic time, later on starttime muts be moved to starttime + panictime */
					printmessage("PANIC BUTTON pressed (No.: %d)",numberofpanics);

//				if(panictime == 0){
//					TrafficLight(G_YELLOW_FLAG);				/* show yellow flag */
//					TrafficLightHard(G_YELLOW_FLAG);
//					TrafficLightMidErase(timetogoheight);
//					for(i=0;i<TRACKLIM;i++)
//						timingactive[i] = 0;                /* ignore all track events from now */
//					panictime = GetTime() - starttime;  	/* remember the panic time, later on starttime muts be moved to starttime + panictime */
//					numberofpanics++;						/* add one more panic */
//					panictimestop = 1;
//					set_trackcurrent(0,trackpoweractive);
//					printmessage("PANIC BUTTON (No.: %d)",numberofpanics);
				}
				else {
					printmessage("PANIC BUTTON already pressed");
				}
			}
			screenupdate = 1;
			break;

			case 'g':
			if(gostate == G_RACE) {
				/* go on after panic  */
				if(panictimestop == 2) {   /* panic delay time is over after Panicstop is from 1 to 2  */
//					if(panictime > 0){
					acttime = GetTime() - starttime;
					overallpanictime += acttime - panictime;
//					if(panictimestop == 1) {
					starttime = starttime + (acttime - panictime); /* in case of timestop shift the start time */
					for(i=0;i<config.numberoftracks;i++)
						firsttime[i] +=	(acttime - panictime); /* correct all lap times */
//						panictimestop = 0;
//					}
					panictimestop = 0;
					panictime = 0;				/* set panic time as indicator for panic back to zero */
					TrafficLight(G_GREEN_FLAG);			/* show green flag */
					TrafficLightHard(G_GREEN_FLAG);
					TrafficLightMidErase(timetogoheight);
					for(i=0;i<TRACKLIM;i++)
						timingactive[i] = 1;                /* start looking for track events from now*/
					set_trackcurrent(1,config.trackpoweractive);
					printmessage("PANIC END (No.: %d)",numberofpanics);
//					}
//					else {
//					printmessage("pst=2, pt=0, makes no sense");
//					}
				}
				else {
					printmessage("PANIC BUTTON not pressed");
				}
			}
			screenupdate = 1;
			break;

			case KEY_F(11):   /* do not modify any state */
			gostate = G_GREEN_FLAG;  			/* show green flag */
			TrafficLightMidErase(timetogoheight);
			TrafficLight(G_GREEN_FLAG);
			TrafficLightHard(gostate);
			for(i=0;i<TRACKLIM;i++)
				timingactive[i] = 1;                /* start looking for track events from now*/
			set_trackcurrent(1,config.trackpoweractive);
			printmessage("FREE TRAINING");
			screenupdate = 1;  /* and bring the differences to the outer world */
			break;

			case KEY_F(12):		/* Starting count down......\n"); */
			if(gostate != G_RACE) {
				gostate = G_RED_0;  			/* start the traffic light engine */
	 			RESETALLTRACKS
				panictimestop = 0;  /* reset panic button */
				starttime = GetTime();
				if( failstartpoweroff == 0) {
					for(i=0;i<TRACKLIM;i++)
						timingactive[i] = 0;
				}
				else {
					for(i=0;i<TRACKLIM;i++)
						timingactive[i] = 1;

				}
				screenupdate = 1;  /* and bring the differences to the outer world */

				set_trackcurrent(1,config.trackpoweractive);
				printmessage("START YOUR ENGINE");
			}
			else
				printmessage("Race already started");
			break;

			case KEY_END:
				stop = 1;  				/* exit from everything */
			break;

			case SHIFT_KEY_F(1):
			if(gostate != G_RACE) {
				printmessage("Reset fastest lap time Track 1");
				fasttime[0] = MAXLAPTIME;
				overallfastest[0] = MAXLAPTIME;
				screenupdate = 1; 				/* and bring the differences to the outer world */
			}
			break;

			case SHIFT_KEY_F(2):
			if(gostate != G_RACE) {
				printmessage("Reset fastest lap time Track 2");
				fasttime[1] = MAXLAPTIME;
				overallfastest[1] = MAXLAPTIME;
				screenupdate = 1; 				/* and bring the differences to the outer world */
			}
			break;

			case SHIFT_KEY_F(3):
			if(gostate != G_RACE) {
				printmessage("Reset fastest lap time Track 3");
				fasttime[2] = MAXLAPTIME;
				overallfastest[2] = MAXLAPTIME;
				screenupdate = 1; 				/* and bring the differences to the outer world */
			}
			break;

			case SHIFT_KEY_F(4):
			if(gostate != G_RACE) {
				printmessage("Reset fastest lap time Track 4");
				fasttime[3] = MAXLAPTIME;
				overallfastest[3] = MAXLAPTIME;
				screenupdate = 1; 				/* and bring the differences to the outer world */
			}
			break;

			case 'W':	/* write data file */

			if( (config.outfile > 0) && (gostate != G_RACE) )
			{

				for(i=0;i<TRACKLIM;i++)
					timingactive[i] = 0;  /* ignore all timing events from now */

		switch(config.outfile) 
		{
		case 1:

			if(write_datafilename(config.resultfile_name, config.resultfile_path, datafilenamecount, endlap, endtime) == 0 )
			{
				printmessage("Timing data file %.4d writing done",datafilenamecount);
			}
			else
			{
				printmessage("file writing gone wrong");
				fprintf(stderr,"file writing gone wrong -%s-%s-\n",config.resultfile_path,config.resultfile_name);
			}
			
			datafilenamecount = find_free_datafilename(config.resultfile_name,config.resultfile_path);

			if ( datafilenamecount == -1 )
			{
				fprintf(stderr,"There is no free data file slot inside the %d filespace, please clear first......",MAXNAMECOUNT);
			}
				/* printmessage("Timing Data file written");  */
			break;
		case 2:
			if(write_datafilename(config.resultfile_name, "", -1 , endlap, endtime) == 0) 
			{
				printmessage("writing timing data file: %s",config.resultfile_name);
			}
			else
			{
				printmessage("file writing gone wrong");
				fprintf(stderr,"file writing gone wrong -%s-\n",config.resultfile_name);
			}			
			break;
		default:
			break;		
		}
		
			for(i=0;i<TRACKLIM;i++)
				timingactive[i] = 1;  /* and switch on timing events again */

			} else
			{
				printmessage("Nothing to write");
			}
			screenupdate = 1; 			/* and bring the differences to the outer world */
			break;

			case 'E':	/* EOR - end of race (forced) */
			if((gostate == G_RACE) || ((gostate == G_CHECKERED_FLAG)&&(endlap > 0)) ){
				printmessage("Race end forced");
				gostate = G_CHECKERED_FLAG; 			/* goto state G_ready */
				TrafficLightMidErase(timetogoheight);
				TrafficLight(gostate);
				TrafficLightHard(gostate);

				for(i=0;i<TRACKLIM;i++)
				{
					finishlap[i] = lapcount[i];  /* store the first car in lap in finishlap[];  */
					finishtime[i] = overalltime[i];  /*  GetTime(); */
					timingactive[i] = 0;                /* stop looking for this track */
				}

				set_trackcurrent(0,config.trackpoweractive);
				carsin = TRACKLIM;
			}
			else {
				printmessage("Forced race end only while race");
			}
			break;

			case ERR:	/* no new message in display */
			break;

			default:
			#ifdef CURSES_KEYCHECK
			if(inchr == 27)
				printmessage(" getch() = %d %d",inchr,getch());/
			else
				printmessage(" getch() = %d",inchr);

			#endif
			break;
		}

	 	refresh();     /* This makes no differences in time or result, so why */

	}


	/******************************************************************************************/
	/* switch off track current here */

	set_trackcurrent(0,config.trackpoweractive);

	/******************************************************************************************/
	/* switch starting traffic light off here */

	#ifdef SN3218

	for (i = 0 ; i <= 17 ; i++)						/* clear 18 LED's connected back to ZERO */
		analogWrite(MCP23017_pinBase + i, SN3218_OFF;

	#endif

	#ifdef MCP23017
	for (i = 0 ; i <= 31 ; i++)						/* clear all LEDs */
		digitalWrite(MCP23017_pinBase + i,MCP23017_OFF);
	#endif

	endwin();

	#ifdef I2C
	for (i = 0 ; i <= 31 ; i++)						/* clear all LEDs */
		yaSetMCP23017(i, YA_LOW); 

	yaSleep(0.25);

	if ( yaMCP23017Release(NumberOfMCP23017) == 0 )
		printf("i2c %s free again\n",I2CDEVICENAME);
	#endif /* I2C */

	if(config.soundactive == 1) {
	#ifdef ALSA_SOUND

//		#ifdef WIRINGPI
//		delay(450);  /* wait for sound ready, can be done better */
//		#endif
//		#ifdef LGPIO
//		lguSleep(0.45);
//		#endif

		yaSleep(0.45);

		SND_close();
	#endif /* ALSA_SOUND */
	}

	/******************************************************************************************/

	for(i=0;i<TRACKLIM;i++)
		timingactive[i] = 0;  /* ignore all timing events from now */
	endwin();

	/******************************************************************************************/
	/* print out the lap times from the last race */

	if( (config.outfile > 0) && (racedatawrittentofile == 0) )
	{
		switch(config.outfile) 
		{
		case 1:
			printf("writing timing data file: %s%s_%d%d%d%d.txt\n",config.resultfile_path,config.resultfile_name,(datafilenamecount/1000)%10,(datafilenamecount/100)%10,(datafilenamecount/10)%10,datafilenamecount%10);
			/* no error check required, because if wrong, then wrong at this time */
			write_datafilename(config.resultfile_name, config.resultfile_path, datafilenamecount, endlap, endtime); 
			break;
		case 2:
			printf("writing timing data file: %s\n",config.resultfile_name);
			/* no error check required, because if wrong, then wrong at this time */
			write_datafilename(config.resultfile_name, "", -1 , endlap, endtime); 
			break;
		default:
			break;		
		}
	}

	store_environment(config.storagefile_name);  /*  store overall lap times in a file */


	#ifdef LGPIO	
	lgret = lgGpiochipClose(lghandle);
	if(lgret < 0)
		printf("lg close failed\n");			
	#endif /* LGPIO */

//	#ifndef OFFLINE
		DeleteLock(GPIOLOCKFILENAME);
//	#endif /* OFFLINE */


	return 0;

}

/******************************************************************************************/

/*  2022-01-31 14:00  */
/*
    yast - yet another slotcar timer
	File: config.h -> all run time settings of the project

    Copyright (C)  2015-2022 Martin Berentsen


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

#ifndef _CONFIG_H_
	#define _CONFIG_H_

	#define RC_MAXLINELENGTH  1024

	struct CONFIG {
		unsigned long minlaptime;  		/* minimum time between two track events */
		int numberoftracks;			/* number of tracks used for calculating and race mode, min 1 max 8 */
		int outfile;				/* a data storage file with timings has to be written or not */
		int rtc_view;				/* real time clock is shown while no race is ongoing */
		int debuglevel;				/* set up the debug level */
		int trackpoweractive;			/* track power can be activated/deactivated */
		int panicdelaytime;			/* panic delay for timing end */
		int timeraceenddelaytime;		/* race delay for timing end */
		int trackcolor[TRACKLIM];   		/* the color of all tracks from 1 to max */
		int trackinputpin[TRACKLIM];		/* set the wiringPi input pin to track number */
		int trackinputevent[TRACKLIM];		/* set the wiringPi input pin event type */
		int trackinputpud[TRACKLIM];		/* set the wiringPi input pin pull up/down */
		int trackcurrentoutput[TRACKLIM];	/* configure aWiringPi GPIO pin for switching power */
		int random_light_startup;		/* if 1, random start time at christmas tree , 0 else */
		int soundactive;			/* Is the sound already active =1 or not =0; */
		char *resultfile_name;			/* name pointer for result files */
		char *resultfile_path;			/* name pointer for result files path */
		char resultfile_format;			/* format of the data storage file*/
		char *storagefile_name;			/* name of the overall storage file */

	};

	int read_yastrc(char *filename);

#endif

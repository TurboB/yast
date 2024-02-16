/*  2024-02-10 20:00  */
/*
    yast - yet another slotcar timer
	File: config.c -> all run time settings of the project

    Copyright (C)  2015-2024 Martin Berentsen


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

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <curses.h>
#include <stdlib.h>

#ifdef WIRINGPI
#include <wiringPi.h>
#endif

#ifdef LGPIO
#include <lgpio.h>
#endif

#include "yast_define.h"
#include "config.h"

extern struct CONFIG config;	/* announce the config memory*/

/* #define PARSERC 1 */  /* make the work of parser visible for debug */

/* ----------------------------------------
converts a string to uppercase
------------------------------------------*/
void strtoupper(char *string)
{
	while(*string)
	{
		*string = toupper(*string);
		string  = string+1;
  	} 
}

/* --------------------------------------------------------------------------
CONFIG read_yastrc(char *filename)
translate the config file into the program vars
Parser did not stop at unknown TOKEN
input: filename
return: scruct CONFIG
------------------------------------------------------------------------*/
int read_yastrc(char *filename)
{
	FILE *fp;					/* filepointer to rc-File */
	char str_rc[RC_MAXLINELENGTH];		/* incoming rc-String */
	char str_copy[RC_MAXLINELENGTH];	/* copy of rc-Strings */
	char *token;				/* isolated word */
	int start;    				/* number of char where the search string starts */
	int len;					/* length of string under work */
	int icount=0;				/* count of known token */
	int excount = 0;			/* eventcounter for choice check */

	if((fp =fopen(filename,"r")) != NULL)
	{
		#ifdef PARSERC
		printf("parsing RC-File %s \n",filename);
		#endif /* PARSERC */

		while (!feof(fp) && icount != -1)
		{
			if(!fgets(str_rc,RC_MAXLINELENGTH,fp))			/* take complete line */
				continue;

			#ifdef PARSERC
			printf("Line to Parse: -%s-\n",str_rc);
			#endif /* PARSERC */

			if(str_rc[0] != '#')
			{
				strcpy(str_copy,str_rc);                  	/* guard string */

				#ifdef PARSERC
				printf("String to Parse: -%s-\n",str_rc);
				#endif /* PARSERC */

				token = strtok(str_copy," \t\n");         	/* isolate first part */
				if(!token)
					continue;

				strtoupper(token);  						/* change to uppercase */

				#ifdef PARSERC
				printf("Token to Parse: -%s-\n",token);
				#endif /* PARSERC */

			/* Parsing Track1Color */
				if(strcmp(token,"TRACK1COLOR:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: TRACK1COLOR: -%s-\n",token);
					#endif
					
					excount = 0;
					if(strcmp(token,"RED") == 0) {
						config.trackcolor[0] = COLOR_RED;
						excount = 1;
						}
					if(strcmp(token,"GREEN") == 0) {
						config.trackcolor[0] = COLOR_GREEN;
						excount = 1;
						}
					if(strcmp(token,"BLUE") == 0) {
						config.trackcolor[0] = COLOR_BLUE;
						excount = 1;
						}
					if(strcmp(token,"YELLOW") == 0) {
						config.trackcolor[0] = COLOR_YELLOW;
						excount = 1;
						}
					if(strcmp(token,"CYAN") == 0) {
						config.trackcolor[0] = COLOR_CYAN;
						excount = 1;
						}
					if(strcmp(token,"MAGENTA") == 0) {
						config.trackcolor[0] = COLOR_MAGENTA;
						excount = 1;
						}
					if(strcmp(token,"WHITE") == 0) {
						config.trackcolor[0] = COLOR_WHITE;
						excount = 1;
						}
					if(excount > 0)
						icount ++;	
					else
					   fprintf(stderr,"read_yastrc: no match found to -%s-\n",token);
				}

			/* Parsing Track2Color */
				if(strcmp(token,"TRACK2COLOR:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: TRACK2COLOR: -%s-\n",token);
					#endif
					
					excount = 0;
					if(strcmp(token,"RED") == 0) {
						config.trackcolor[1] = COLOR_RED;
						excount = 1;
						}
					if(strcmp(token,"GREEN") == 0) {
						config.trackcolor[1] = COLOR_GREEN;
						excount = 1;
						}
					if(strcmp(token,"BLUE") == 0) {
						config.trackcolor[1] = COLOR_BLUE;
						excount = 1;
						}
					if(strcmp(token,"YELLOW") == 0) {
						config.trackcolor[1] = COLOR_YELLOW;
						excount = 1;
						}
					if(strcmp(token,"CYAN") == 0) {
						config.trackcolor[1] = COLOR_CYAN;
						excount = 1;
						}
					if(strcmp(token,"MAGENTA") == 0) {
						config.trackcolor[1] = COLOR_MAGENTA;
						excount = 1;
						}
					if(strcmp(token,"WHITE") == 0) {
						config.trackcolor[1] = COLOR_WHITE;
						excount = 1;
						}
					if(excount > 0)
						icount ++;	
					else
					   fprintf(stderr,"read_yastrc: no match found to -%s-\n",token);
				}

			/* Parsing Track3Color */
				if(strcmp(token,"TRACK3COLOR:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: TRACK3COLOR: -%s-\n",token);
					#endif
					
					excount = 0;
					if(strcmp(token,"RED") == 0) {
						config.trackcolor[2] = COLOR_RED;
						excount = 1;
						}
					if(strcmp(token,"GREEN") == 0) {
						config.trackcolor[2] = COLOR_GREEN;
						excount = 1;
						}
					if(strcmp(token,"BLUE") == 0) {
						config.trackcolor[2] = COLOR_BLUE;
						excount = 1;
						}
					if(strcmp(token,"YELLOW") == 0) {
						config.trackcolor[2] = COLOR_YELLOW;
						excount = 1;
						}
					if(strcmp(token,"CYAN") == 0) {
						config.trackcolor[2] = COLOR_CYAN;
						excount = 1;
						}
					if(strcmp(token,"MAGENTA") == 0) {
						config.trackcolor[2] = COLOR_MAGENTA;
						excount = 1;
						}
					if(strcmp(token,"WHITE") == 0) {
						config.trackcolor[2] = COLOR_WHITE;
						excount = 1;
						}
					if(excount > 0)
						icount ++;	
					else
					   fprintf(stderr,"read_yastrc: no match found to -%s-\n",token);
				}

			/* Parsing Track4Color */
				if(strcmp(token,"TRACK4COLOR:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: TRACK4COLOR: -%s-\n",token);
					#endif
					
					excount = 0;
					if(strcmp(token,"RED") == 0) {
						config.trackcolor[3] = COLOR_RED;
						excount = 1;
						}
					if(strcmp(token,"GREEN") == 0) {
						config.trackcolor[3] = COLOR_GREEN;
						excount = 1;
						}
					if(strcmp(token,"BLUE") == 0) {
						config.trackcolor[3] = COLOR_BLUE;
						excount = 1;
						}
					if(strcmp(token,"YELLOW") == 0) {
						config.trackcolor[3] = COLOR_YELLOW;
						excount = 1;
						}
					if(strcmp(token,"CYAN") == 0) {
						config.trackcolor[3] = COLOR_CYAN;
						excount = 1;
						}
					if(strcmp(token,"MAGENTA") == 0) {
						config.trackcolor[3] = COLOR_MAGENTA;
						excount = 1;
						}
					if(strcmp(token,"WHITE") == 0) {
						config.trackcolor[3] = COLOR_WHITE;
						excount = 1;
						}
					if(excount > 0)
						icount ++;	
					else
					   fprintf(stderr,"read_yastrc: no match found to -%s-\n",token);
				}

			/* Parsing Track1INPUT */
				if(strcmp(token,"TRACK1INPUT:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: TRACK1INPUT: -%s-\n",token);
					#endif

					config.trackinputpin[0] = atoi(token);
					if((config.trackinputpin[0] >= MIN_GPIO_INPUTPINS) && (config.trackinputpin[0]<= MAX_GPIO_INPUTPINS))
						icount ++;
					else {
						fprintf(stderr, "read_yastrc: Wrong track input pin given %d\n",config.trackinputpin[0]);
						exit(0);
						}
				}		

			/* Parsing Track2INPUT */
				if(strcmp(token,"TRACK2INPUT:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: TRACK2INPUT: -%s-\n",token);
					#endif

					config.trackinputpin[1] = atoi(token);
					if((config.trackinputpin[1] >= MIN_GPIO_INPUTPINS) && (config.trackinputpin[1]<= MAX_GPIO_INPUTPINS))
						icount ++;
					else {
						fprintf(stderr, "read_yastrc: Wrong track input pin given %d\n",config.trackinputpin[1]);
						exit(0);
						}
				}		

			/* Parsing Track3INPUT */
				if(strcmp(token,"TRACK3INPUT:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: TRACK3INPUT: -%s-\n",token);
					#endif

					config.trackinputpin[2] = atoi(token);
					if((config.trackinputpin[2] >= MIN_GPIO_INPUTPINS) && (config.trackinputpin[2]<= MAX_GPIO_INPUTPINS))
						icount ++;
					else {
						fprintf(stderr, "read_yastrc: Wrong track input pin given %d\n",config.trackinputpin[2]);
						exit(0);
						}
				}		

			/* Parsing Track4INPUT */
				if(strcmp(token,"TRACK4INPUT:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: TRACK4INPUT: -%s-\n",token);
					#endif

					config.trackinputpin[3] = atoi(token);
					if((config.trackinputpin[3] >= MIN_GPIO_INPUTPINS) && (config.trackinputpin[3]<= MAX_GPIO_INPUTPINS))
						icount ++;
					else {
						fprintf(stderr, "read_yastrc: Wrong track input pin given %d\n",config.trackinputpin[3]);
						exit(0);
						}
				}		

			/* Parsing track1inputevent */
				if(strcmp(token,"TRACK1INPUTEVENT:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: TRACK1INPUTEVENT: -%s-\n",token);
					#endif
					
					excount = 0;
					if(strcmp(token,"INT_EDGE_RISING") == 0) {

						#ifdef WIRINGPI
						config.trackinputevent[0] = INT_EDGE_RISING;
						#endif /* WIRINGPI */

						#ifdef LGPIO
						config.trackinputevent[0] = LG_RISING_EDGE;
						#endif /* LGPIO */
						
						excount = 1;
						}
					if(strcmp(token,"INT_EDGE_FALLING") == 0) {

						#ifdef WIRINGPI
						config.trackinputevent[0] = INT_EDGE_FALLING;
						#endif /* WIRINGPI */

						#ifdef LGPIO
						config.trackinputevent[0] = LG_FALLING_EDGE;
						#endif /* LGPIO */

						excount = 1;
						}
					if(excount > 0)
						icount ++;	
					else
					   fprintf(stderr, "read_yastrc: no match found to -%s-\n",token);
				}

			/* Parsing track2inputevent */
				if(strcmp(token,"TRACK2INPUTEVENT:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: TRACK2INPUTEVENT: -%s-\n",token);
					#endif
					
					excount = 0;
					if(strcmp(token,"INT_EDGE_RISING") == 0) {

						#ifdef WIRINGPI
						config.trackinputevent[1] = INT_EDGE_RISING;
						#endif /* WIRINGPI */
						
						#ifdef WIRINGPI
						config.trackinputevent[1] = INT_EDGE_RISING;
						#endif /* WIRINGPI */

						excount = 1;
						}
					if(strcmp(token,"INT_EDGE_FALLING") == 0) {
						#ifdef WIRINGPI
						config.trackinputevent[1] = INT_EDGE_FALLING;
						#endif /* WIRINGPI */

						#ifdef LGPIO
						config.trackinputevent[1] = LG_FALLING_EDGE;
						#endif /* LGPIO */

						excount = 1;
						}
					if(excount > 0)
						icount ++;	
					else
					   fprintf(stderr, "read_yastrc: no match found to -%s-\n",token);
				}

			/* Parsing track3inputevent */
				if(strcmp(token,"TRACK3INPUTEVENT:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: TRACK3INPUTEVENT: -%s-\n",token);
					#endif
					
					excount = 0;
					if(strcmp(token,"INT_EDGE_RISING") == 0) {
						#ifdef WIRINGPI 
						config.trackinputevent[2] = INT_EDGE_RISING;
						#endif /* WIRINGPI */

						#ifdef LGPIO
						config.trackinputevent[2] = LG_FALLING_EDGE;
						#endif /* LGPIO */

						excount = 1;
						}
					if(strcmp(token,"INT_EDGE_FALLING") == 0) {
						#ifdef WIRINGPI
						config.trackinputevent[2] = INT_EDGE_FALLING;
						#endif /* WIRINGPI */

						#ifdef LGPIO
						config.trackinputevent[2] = LG_FALLING_EDGE;
						#endif /* LGPIO */

						excount = 1;
						}
					if(excount > 0)
						icount ++;	
					else
					   fprintf(stderr, "read_yastrc: no match found to -%s-\n",token);
				}

			/* Parsing track4inputevent */
				if(strcmp(token,"TRACK4INPUTEVENT:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: TRACK4INPUTEVENT: -%s-\n",token);
					#endif
					
					excount = 0;
					if(strcmp(token,"INT_EDGE_RISING") == 0) {

						#ifdef WIRINGPI
						config.trackinputevent[3] = INT_EDGE_RISING;
						#endif /* WIRINGPI */

						#ifdef LGPIO
						config.trackinputevent[3] = LG_FALLING_EDGE;
						#endif /* LGPIO */

						excount = 1;
						}
					if(strcmp(token,"INT_EDGE_FALLING") == 0) {

						#ifdef WIRINGPI
						config.trackinputevent[3] = INT_EDGE_FALLING;
						#endif /* WIRINGPI */

						#ifdef LGPIO
						config.trackinputevent[3] = LG_FALLING_EDGE;
						#endif /* LGPIO */

						excount = 1;
						}
					if(excount > 0)
						icount ++;	
					else
					   fprintf(stderr, "read_yastrc: no match found to -%s-\n",token);
				}

			/* Parsing track1inputPUD */
				if(strcmp(token,"TRACK1INPUTPUD:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: TRACK1INPUTEVENT: -%s-\n",token);
					#endif
					
					excount = 0;
					if(strcmp(token,"PUD_OFF") == 0) {
						#ifdef WIRINGPI
						config.trackinputpud[0] = PUD_OFF;
						#endif /* WIRINGPI */

						#ifdef LGPIO
						config.trackinputpud[0] = LG_SET_PULL_NONE;
						#endif /* LGPIO */
						
						excount = 1;
						}
					if(strcmp(token,"PUD_DOWN") == 0) {
						#ifdef WIRINGPI
						config.trackinputpud[0] = PUD_DOWN;
						#endif /* WIRINGPI */

						#ifdef LGPIO
						config.trackinputpud[0] = LG_SET_PULL_DOWN;
						#endif /* LGPIO */
												
						excount = 1;
						}
					if(strcmp(token,"PUD_UP") == 0) {
						#ifdef WIRINGPI
						config.trackinputpud[0] = PUD_UP;
						#endif /* WIRINGPI */

						#ifdef LGPIO
						config.trackinputpud[0] = LG_SET_PULL_UP;
						#endif /* LGPIO */
												
						excount = 1;
						}
					if(excount > 0)
						icount ++;	
					else
					   fprintf(stderr, "read_yastrc: no match found to -%s-\n",token);
				}

			/* Parsing track2inputPUD */
				if(strcmp(token,"TRACK2INPUTPUD:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: TRACK2INPUTEVENT: -%s-\n",token);
					#endif
					
					excount = 0;
					if(strcmp(token,"PUD_OFF") == 0) {
						#ifdef WIRINGPI
						config.trackinputpud[1] = PUD_OFF;
						#endif /* WIRINGPI */

						#ifdef LGPIO
						config.trackinputpud[1] = LG_SET_PULL_NONE;
						#endif /* LGPIO */
						
						excount = 1;
						}
					if(strcmp(token,"PUD_DOWN") == 0) {
						#ifdef WIRINGPI
						config.trackinputpud[1] = PUD_DOWN;
						#endif /* WIRINGPI */

						#ifdef LGPIO
						config.trackinputpud[1] = LG_SET_PULL_DOWN;
						#endif /* LGPIO */
						
						excount = 1;
						}
					if(strcmp(token,"PUD_UP") == 0) {
						#ifdef WIRINGPI
						config.trackinputpud[1] = PUD_UP;
						#endif /* WIRINGPI */

						#ifdef LGPIO
						config.trackinputpud[1] = LG_SET_PULL_UP;
						#endif /* LGPIO */
						
						excount = 1;
						}
					if(excount > 0)
						icount ++;	
					else
					   fprintf(stderr, "read_yastrc: no match found to -%s-\n",token);
				}

			/* Parsing track3inputPUD */
				if(strcmp(token,"TRACK3INPUTPUD:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: TRACK3INPUTEVENT: -%s-\n",token);
					#endif
					
					excount = 0;
					if(strcmp(token,"PUD_OFF") == 0) {
						#ifdef WIRINGPI
						config.trackinputpud[2] = PUD_OFF;
						#endif /* WIRINGPI */
						
						#ifdef LGPIO
						config.trackinputpud[2] = LG_SET_PULL_NONE;
						#endif /* LGPIO */
						
						excount = 1;
						}
					if(strcmp(token,"PUD_DOWN") == 0) {
						#ifdef WIRINGPI
						config.trackinputpud[2] = PUD_DOWN;
						#endif /* WIRINGPI */
						
						#ifdef LGPIO
						config.trackinputpud[2] = LG_SET_PULL_DOWN;
						#endif /* LGPIO */
						
						excount = 1;
						}
					if(strcmp(token,"PUD_UP") == 0) {
						#ifdef WIRINGPI
						config.trackinputpud[2] = PUD_UP;
						#endif /* WIRINGPI */
						
						#ifdef LGPIO
						config.trackinputpud[2] = LG_SET_PULL_UP;
						#endif /* LGPIO */
						
						excount = 1;
						}
					if(excount > 0)
						icount ++;	
					else
					   fprintf(stderr, "read_yastrc: no match found to -%s-\n",token);
				}

			/* Parsing track4inputPUD */
				if(strcmp(token,"TRACK4INPUTPUD:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: TRACK4INPUTEVENT: -%s-\n",token);
					#endif
					
					excount = 0;
					if(strcmp(token,"PUD_OFF") == 0) {
						#ifdef WIRINGPI
						config.trackinputpud[3] = PUD_OFF;
						#endif /* WIRINGPI */

						#ifdef LGPIO
						config.trackinputpud[3] = LG_SET_PULL_NONE;
						#endif /* LGPIO */						
						
						excount = 1;
						}
					if(strcmp(token,"PUD_DOWN") == 0) {
						#ifdef WIRINGPI
						config.trackinputpud[3] = PUD_DOWN;
						#endif /* WIRINGPI */

						#ifdef LGPIO
						config.trackinputpud[3] = LG_SET_PULL_DOWN;
						#endif /* LGPIO */						
						
						excount = 1;
						}
					if(strcmp(token,"PUD_UP") == 0) {
						#ifdef WIRINGPI
						config.trackinputpud[3] = PUD_UP;
						#endif /* WIRINGPI */

						#ifdef LGPIO
						config.trackinputpud[3] = LG_SET_PULL_UP;
						#endif /* LGPIO */						
						
						excount = 1;
						}
					if(excount > 0)
						icount ++;	
					else
					   fprintf(stderr, "read_yastrc: no match found to -%s-\n",token);
				}

			/* Parsing NumberOfTracks */
				if(strcmp(token,"NUMBEROFTRACKS:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: TRACKSINUSE: -%s-\n",token);
					#endif

					config.numberoftracks = atoi(token);
					if((config.numberoftracks > 0) && (config.numberoftracks<= TRACKLIM))
						icount ++;
					else {
						fprintf(stderr, "read_yastrc: Wrong number of tracks given %d\n, must be between 1 and %d",config.numberoftracks,TRACKLIM);
						exit(0);
						}
				}

			/* Parsing RESULTFILEFORMAT */
				if(strcmp(token,"RESULTFILEFORMAT:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: RESULTFILEFORMAT: -%s-\n",token);
					#endif

					config.resultfile_format = atoi(token);
					if((config.resultfile_format >= 0) && (config.resultfile_format <= 1))
						icount ++;
					else {
						fprintf(stderr, "read_yastrc: Wrong RESULTFILEFORMAT given %d\n",config.resultfile_format);
						exit(0);
						}
				}		

				/* Parsing MinimumLapTime */
				if(strcmp(token,"MINIMUMLAPTIME:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: MINIMUMLAPTIME: -%s-\n",token);
					#endif

					config.minlaptime = (unsigned long) atoi(token);
					if((config.minlaptime > 10) && (config.minlaptime<= MAXDEADDELAY))
						icount ++;
					else {
          				fprintf(stderr, "read_yastrc: wrong minimum lap time given: %ld, must be between 10 and %d mseconds\n",config.minlaptime,MAXDEADDELAY);
						exit(0);
						}
				}		

			/* Parsing TrackCurrentOutput 1 */
				if(strcmp(token,"TRACKCURRENTOUTPUT1:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: TRACKCURRENTOUTPUT1: -%s-\n",token);
					#endif

					config.trackcurrentoutput[0] = atoi(token);
					if((config.trackcurrentoutput[0] >= -1) && (config.trackcurrentoutput[0] <= MAX_GPIO_OUTPUTPINS))
						icount ++;
					else {
						fprintf(stderr, "read_yastrc: Wrong TrackCurrentOutput1 pin given %d\n",config.trackcurrentoutput[0]);
						exit(0);
						}
				}		

			/* Parsing TrackCurrentOutput 2 */
				if(strcmp(token,"TRACKCURRENTOUTPUT2:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: TRACKCURRENTOUTPUT2: -%s-\n",token);
					#endif

					config.trackcurrentoutput[1] = atoi(token);
					if((config.trackcurrentoutput[1] >= -1) && (config.trackcurrentoutput[1] <= MAX_GPIO_OUTPUTPINS))
						icount ++;
					else {
						fprintf(stderr, "read_yastrc: Wrong TrackCurrentOutput2 pin given %d\n",config.trackcurrentoutput[1]);
						exit(0);
						}
				}		

			/* Parsing TrackCurrentOutput 3 */
				if(strcmp(token,"TRACKCURRENTOUTPUT3:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: TRACKCURRENTOUTPUT3: -%s-\n",token);
					#endif

					config.trackcurrentoutput[2] = atoi(token);
					if((config.trackcurrentoutput[2] >= -1) && (config.trackcurrentoutput[2] <= MAX_GPIO_OUTPUTPINS))
						icount ++;
					else {
						fprintf(stderr, "read_yastrc: Wrong TrackCurrentOutput3 pin given %d\n",config.trackcurrentoutput[2]);
						exit(0);
						}
				}		

			/* Parsing TrackCurrentOutput 4 */
				if(strcmp(token,"TRACKCURRENTOUTPUT4:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: TRACKCURRENTOUTPUT4: -%s-\n",token);
					#endif

					config.trackcurrentoutput[3] = atoi(token);
					if((config.trackcurrentoutput[3] >= -1) && (config.trackcurrentoutput[3] <= MAX_GPIO_OUTPUTPINS))
						icount ++;
					else {
						fprintf(stderr, "read_yastrc: Wrong TrackCurrentOutput4 pin given %d\n",config.trackcurrentoutput[3]);
						exit(0);
						}
				}		

			/* Parsing TrackPOWERActive */
				if(strcmp(token,"TRACKPOWERACTIVE:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: TRACKPOWERACTIVE: -%s-\n",token);
					#endif

					config.trackpoweractive = atoi(token);
					if((config.trackpoweractive == 0) || (config.trackpoweractive == 1))
						icount ++;
					else {
						fprintf(stderr, "read_yastrc: Wrong trackpoweractive level (0 or 1) given %d\n",config.trackpoweractive);
						exit(0);
						}
				}		

			/* Parsing SoundActive */
				if(strcmp(token,"SOUNDACTIVE:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: SOUNDACTIVE: -%s-\n",token);
					#endif

					config.soundactive = atoi(token);
					if((config.soundactive == 0) || (config.soundactive == 1))
						icount ++;
					else {
						fprintf(stderr, "read_yastrc: Wrong soundactive level (0 or 1) given %d\n",config.soundactive);
						exit(0);
						}
				}		

			/* Parsing RTCActive */
				if(strcmp(token,"RTCACTIVE:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: RTCACTIVE: -%s-\n",token);
					#endif

					config.rtc_view = atoi(token);
					if((config.rtc_view == 0) || (config.rtc_view == 1))
						icount ++;
					else {
						fprintf(stderr, "read_yastrc: Wrong rtcactive level (0 or 1) given %d\n",config.rtc_view);
						exit(0);
						}
				}		
				
			/* Parsing ResultFileName */
				if(strcmp(token,"RESULTFILENAME:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: RESULTFILENAME: -%s-\n",token);
					#endif

					free(config.resultfile_name);  /* cancel the old one */

					if((config.resultfile_name = (char *)malloc(strlen(token)+1)) != NULL) 
						{
							strcpy(config.resultfile_name,token);
						}
						else
						{
							fprintf(stderr,"config: malloc resultfile_name error\n");
							exit(-1);
						}
						
						if(strlen(config.resultfile_name) <=2 )
						{
							fprintf(stderr,"config: please set up a ResultFileName: with three or more letters (given: %s))\n",config.resultfile_name);
							exit(-1);
						}
				}		

			/* Parsing ResultFilePath */
				if(strcmp(token,"RESULTFILEPATH:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: RESULTFILEPATH: -%s-\n",token);
					#endif

					free(config.resultfile_path);  /* cancel the old one */

					if((config.resultfile_path = (char *)malloc(strlen(token)+1)) != NULL) 
						{
							strcpy(config.resultfile_path,token);
						}
						else
						{
							fprintf(stderr,"config: malloc resultfile_path error\n");
							exit(-1);
						}
						
						if(strlen(config.resultfile_path) >= 2 )
						{
							char *tmp;
							tmp = strrchr(config.resultfile_path,'/');
							
							if(tmp == NULL)
							{
								fprintf(stderr,"config: please set up a ResultFilePath: with a / at the end (given: %s)\n",config.resultfile_path);
								exit(0);
							}
							else
							if(strlen(tmp) > 1)
							{
								fprintf(stderr,"config: please set up a ResultFilePath: with a / at the end (given: %s)\n",config.resultfile_path);
								exit(0);
							}
						}
						else
						{
								fprintf(stderr,"config: please set up a ResultFilePath: with a / and more than two chars (given: %s)\n",config.resultfile_path);
								exit(0);
							
						}
				}		

			/* Parsing 	NO_RANDOM_LIGHT_STARTUP */
				if(strcmp(token,"NO_RANDOM_LIGHT_STARTUP") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: NO_RANDOM_LIGHT_STARTUP -%s-\n",token);
					#endif

					config.random_light_startup = 0;
				}		
			
			/* Parsing PANICDELAYTIME */
				if(strcmp(token,"PANICDELAYTIME:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: PANICDELAYTIME: -%s-\n",token);
					#endif

					config.panicdelaytime = atoi(token);
					if((config.panicdelaytime >= 0) && (config.panicdelaytime <= MAXPANICDELAYTIME))
						icount ++;
					else {
						fprintf(stderr, "read_yastrc: PANICDELAYTIME given%d\n",config.panicdelaytime);
						exit(0);
						}
				}		

				/* Parsing TIMERACEENDDELAYTIME */
				if(strcmp(token,"TIMERACEENDDELAYTIME:") == 0)
				{
					start = strstr(str_copy,token) - str_copy;	/* start of token inside the string */
					len = strlen(token);						/* length of tokens */
					memmove(str_copy,&str_rc[len+start],strlen(str_rc));
					token = strtok(str_copy," \t\n");        	/* isolate second part */

					#ifdef PARSERC
					printf("Token found: TIMERACEENDDELAYTIME: -%s-\n",token);
					#endif

					config.timeraceenddelaytime = atoi(token);
					if((config.timeraceenddelaytime >= 0) && (config.timeraceenddelaytime <= MAXTIMERACEENDDELAYTIME))
						icount ++;
					else {
						fprintf(stderr, "read_yastrc: TIMERACEENDDELAYTIME given%d\n",config.timeraceenddelaytime);
						exit(0);
						}
				}		

			}
		}
		fclose (fp);
		#ifdef PARSERC
		printf("Token found: %d\n",icount);
		printf("End of parsing\n");
		#endif /* PARSERC */
	}
        else {
		#ifdef PARSERC
        	fprintf(stderr,"read_yastrc: rc-file %s not found\n",filename);
        	#endif
        	icount = -1;
        }
	return icount;
}


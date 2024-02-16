/*  2024-02-10 23:00  */
/*
yast - yet another slotcar timer
File: TrafficLight.h -> Software display based Traffic Light / Start Light

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

#include <curses.h>

#ifdef WIRINGPI
#include <wiringPi.h>
#endif /* WIRINGPI */

#ifdef LGPIO
#include <lgpio.h>
#endif /* LGPIO */

#include "trafficLight.h"
#include "yast_define.h"
#include "display.h"
#include "i2c.h"

/* TrafficLight() shows the Traffic Light on the main screen */
void TrafficLight(int state)
{
	switch(state) {
		case G_RED_0:
		case G_BLANK:
		attrset(COLOR_PAIR(COLOR_PAIR_RED));
		#ifdef TRAFFIC_BIG
		move(CURSES_TRAFFICLIGHT_LINE_Y,20);
		printw("  ...      ...      ...      ...      ...  ");
		move(CURSES_TRAFFICLIGHT_LINE_Y+1,20);
		printw(" .   .    .   .    .   .    .   .    .   . ");
		move(CURSES_TRAFFICLIGHT_LINE_Y+2,20);
		printw(".     .  .     .  .     .  .     .  .     .");
		move(CURSES_TRAFFICLIGHT_LINE_Y+3,20);
		printw(".     .  .     .  .     .  .     .  .     .");
		move(CURSES_TRAFFICLIGHT_LINE_Y+4,20);
		printw(" .   .    .   .    .   .    .   .    .   . ");
		move(CURSES_TRAFFICLIGHT_LINE_Y+5,20);
		printw("  ...      ...      ...      ...      ...  ");
		#endif
		#ifdef TRAFFIC_SMALL
		move(CURSES_TRAFFICLIGHT_LINE_Y,0);
		printw("..........       ..........        ..........        ..........       ..........");
		move(CURSES_TRAFFICLIGHT_LINE_Y+1,0);
		printw(".        .       .        .        .        .        .        .       .        .");
		move(CURSES_TRAFFICLIGHT_LINE_Y+2,0);
		printw("..........       ..........        ..........        ..........       ..........");
		#endif
		attrset(COLOR_PAIR(COLOR_PAIR_BASIC));
		break;
		case G_RED_1:
		attrset(COLOR_PAIR(COLOR_PAIR_RED));
		#ifdef TRAFFIC_BIG
		move(CURSES_TRAFFICLIGHT_LINE_Y,20);
		printw("  ***      ...      ...      ...      ...  ");   /* do later on with addch(ACS_CKBOARD); */
		move(CURSES_TRAFFICLIGHT_LINE_Y+1,20);
		printw(" *****    .   .    .   .    .   .    .   . ");
		move(CURSES_TRAFFICLIGHT_LINE_Y+2,20);
		printw("*******  .     .  .     .  .     .  .     .");
		move(CURSES_TRAFFICLIGHT_LINE_Y+3,20);
		printw("*******  .     .  .     .  .     .  .     .");
		move(CURSES_TRAFFICLIGHT_LINE_Y+4,20);
		printw(" *****    .   .    .   .    .   .    .   . ");
		move(CURSES_TRAFFICLIGHT_LINE_Y+5,20);
		printw("  ***      ...      ...      ...      ...  ");
		#endif
		#ifdef TRAFFIC_SMALL
		move(CURSES_TRAFFICLIGHT_LINE_Y,0);
		printw("XXXXXXXXXX       ..........        ..........        ..........       ..........");
		move(CURSES_TRAFFICLIGHT_LINE_Y+1,0);
		printw("XXXXXXXXXX       .        .        .        .        .        .       .        .");
		move(CURSES_TRAFFICLIGHT_LINE_Y+2,0);
		printw("XXXXXXXXXX       ..........        ..........        ..........       ..........");
		#endif
		attrset(COLOR_PAIR(COLOR_PAIR_BASIC));
		break;
		case G_RED_2:
		attrset(COLOR_PAIR(COLOR_PAIR_RED));
		#ifdef TRAFFIC_BIG
		move(CURSES_TRAFFICLIGHT_LINE_Y,20);
		printw("  ***      ***      ...      ...      ...  ");   /* do later on with addch(ACS_CKBOARD); */
		move(CURSES_TRAFFICLIGHT_LINE_Y+1,20);
		printw(" *****    *****    .   .    .   .    .   . ");
		move(CURSES_TRAFFICLIGHT_LINE_Y+2,20);
		printw("*******  *******  .     .  .     .  .     .");
		move(CURSES_TRAFFICLIGHT_LINE_Y+3,20);
		printw("*******  *******  .     .  .     .  .     .");
		move(CURSES_TRAFFICLIGHT_LINE_Y+4,20);
		printw(" *****    *****    .   .    .   .    .   . ");
		move(CURSES_TRAFFICLIGHT_LINE_Y+5,20);
		printw("  ***      ***      ...      ...      ...  ");
		#endif
		#ifdef TRAFFIC_SMALL
		move(CURSES_TRAFFICLIGHT_LINE_Y,0);
		printw("XXXXXXXXXX       XXXXXXXXXX        ..........        ..........       ..........");
		move(CURSES_TRAFFICLIGHT_LINE_Y+1,0);
		printw("XXXXXXXXXX       XXXXXXXXXX        .        .        .        .       .        .");
		move(CURSES_TRAFFICLIGHT_LINE_Y+2,0);
		printw("XXXXXXXXXX       XXXXXXXXXX        ..........        ..........       ..........");
		#endif
		attrset(COLOR_PAIR(COLOR_PAIR_BASIC));
		break;
		case G_RED_3:
		attrset(COLOR_PAIR(COLOR_PAIR_RED));
		#ifdef TRAFFIC_BIG
		move(CURSES_TRAFFICLIGHT_LINE_Y,20);
		printw("  ***      ***      ***      ...      ...  ");   /* do later on with addch(ACS_CKBOARD); */
		move(CURSES_TRAFFICLIGHT_LINE_Y+1,20);
		printw(" *****    *****    *****    .   .    .   . ");
		move(CURSES_TRAFFICLIGHT_LINE_Y+2,20);
		printw("*******  *******  *******  .     .  .     .");
		move(CURSES_TRAFFICLIGHT_LINE_Y+3,20);
		printw("*******  *******  *******  .     .  .     .");
		move(CURSES_TRAFFICLIGHT_LINE_Y+4,20);
		printw(" *****    *****    *****    .   .    .   . ");
		move(CURSES_TRAFFICLIGHT_LINE_Y+5,20);
		printw("  ***      ***      ***      ...      ...  ");
		#endif
		#ifdef TRAFFIC_SMALL
		move(CURSES_TRAFFICLIGHT_LINE_Y,0);
		printw("XXXXXXXXXX       XXXXXXXXXX        XXXXXXXXXX        ..........       ..........");
		move(CURSES_TRAFFICLIGHT_LINE_Y+1,0);
		printw("XXXXXXXXXX       XXXXXXXXXX        XXXXXXXXXX        .        .       .        .");
		move(CURSES_TRAFFICLIGHT_LINE_Y+2,0);
		printw("XXXXXXXXXX       XXXXXXXXXX        XXXXXXXXXX        ..........       ..........");
		#endif
		attrset(COLOR_PAIR(COLOR_PAIR_BASIC));
		break;
		case G_RED_4:
		attrset(COLOR_PAIR(COLOR_PAIR_RED));
		#ifdef TRAFFIC_BIG
		move(CURSES_TRAFFICLIGHT_LINE_Y,20);
		printw("  ***      ***      ***      ***      ...  ");   /* do later on with addch(ACS_CKBOARD); */
		move(CURSES_TRAFFICLIGHT_LINE_Y+1,20);
		printw(" *****    *****    *****    *****    .   . ");
		move(CURSES_TRAFFICLIGHT_LINE_Y+2,20);
		printw("*******  *******  *******  *******  .     .");
		move(CURSES_TRAFFICLIGHT_LINE_Y+3,20);
		printw("*******  *******  *******  *******  .     .");
		move(CURSES_TRAFFICLIGHT_LINE_Y+4,20);
		printw(" *****    *****    *****    *****    .   . ");
		move(CURSES_TRAFFICLIGHT_LINE_Y+5,20);
		printw("  ***      ***      ***      ***      ...  ");
		#endif
		#ifdef TRAFFIC_SMALL
		move(CURSES_TRAFFICLIGHT_LINE_Y,0);
		printw("XXXXXXXXXX       XXXXXXXXXX        XXXXXXXXXX        XXXXXXXXXX       ..........");
		move(CURSES_TRAFFICLIGHT_LINE_Y+1,0);
		printw("XXXXXXXXXX       XXXXXXXXXX        XXXXXXXXXX        XXXXXXXXXX       .        .");
		move(CURSES_TRAFFICLIGHT_LINE_Y+2,0);
		printw("XXXXXXXXXX       XXXXXXXXXX        XXXXXXXXXX        XXXXXXXXXX       ..........");
		#endif
		attrset(COLOR_PAIR(COLOR_PAIR_BASIC));
		break;
		case G_RED_5:
		case G_RED_FLAG:
		attrset(COLOR_PAIR(COLOR_PAIR_RED));
		#ifdef TRAFFIC_BIG
		move(CURSES_TRAFFICLIGHT_LINE_Y,20);
		printw("  ***      ***      ***      ***      ***  ");   /* do later on with addch(ACS_CKBOARD); */
		move(CURSES_TRAFFICLIGHT_LINE_Y+1,20);
		printw(" *****    *****    *****    *****    ***** ");
		move(CURSES_TRAFFICLIGHT_LINE_Y+2,20);
		printw("*******  *******  *******  *******  *******");
		move(CURSES_TRAFFICLIGHT_LINE_Y+3,20);
		printw("*******  *******  *******  *******  *******");
		move(CURSES_TRAFFICLIGHT_LINE_Y+4,20);
		printw(" *****    *****    *****    *****    ***** ");
		move(CURSES_TRAFFICLIGHT_LINE_Y+5,20);
		printw("  ***      ***      ***      ***      ***  ");
		#endif
		#ifdef TRAFFIC_SMALL
		move(CURSES_TRAFFICLIGHT_LINE_Y,0);
		printw("XXXXXXXXXX       XXXXXXXXXX        XXXXXXXXXX        XXXXXXXXXX       XXXXXXXXXX");
		move(CURSES_TRAFFICLIGHT_LINE_Y+1,0);
		printw("XXXXXXXXXX       XXXXXXXXXX        XXXXXXXXXX        XXXXXXXXXX       XXXXXXXXXX");
		move(CURSES_TRAFFICLIGHT_LINE_Y+2,0);
		printw("XXXXXXXXXX       XXXXXXXXXX        XXXXXXXXXX        XXXXXXXXXX       XXXXXXXXXX");
		#endif
		attrset(COLOR_PAIR(COLOR_PAIR_BASIC));
		break;
		case G_GREEN:
		case G_GREEN_FLAG:
		attrset(COLOR_PAIR(COLOR_PAIR_GREEN));
		#ifdef TRAFFIC_BIG
		move(CURSES_TRAFFICLIGHT_LINE_Y,20);
		printw("  ***      ***      ***      ***      ***  ");   /* do later on with addch(ACS_CKBOARD); */
		move(CURSES_TRAFFICLIGHT_LINE_Y+1,20);
		printw(" *****    *****    *****    *****    ***** ");
		move(CURSES_TRAFFICLIGHT_LINE_Y+2,20);
		printw("*******  *******  *******  *******  *******");
		move(CURSES_TRAFFICLIGHT_LINE_Y+3,20);
		printw("*******  *******  *******  *******  *******");
		move(CURSES_TRAFFICLIGHT_LINE_Y+4,20);
		printw(" *****    *****    *****    *****    ***** ");
		move(CURSES_TRAFFICLIGHT_LINE_Y+5,20);
		printw("  ***      ***      ***      ***      ***  ");
		#endif
		#ifdef TRAFFIC_SMALL
		move(CURSES_TRAFFICLIGHT_LINE_Y,0);
		printw("XXXXXXXXXX       XXXXXXXXXX        XXXXXXXXXX        XXXXXXXXXX       XXXXXXXXXX");
		move(CURSES_TRAFFICLIGHT_LINE_Y+1,0);
		printw("XXXXXXXXXX       XXXXXXXXXX        XXXXXXXXXX        XXXXXXXXXX       XXXXXXXXXX");
		move(CURSES_TRAFFICLIGHT_LINE_Y+2,0);
		printw("XXXXXXXXXX       XXXXXXXXXX        XXXXXXXXXX        XXXXXXXXXX       XXXXXXXXXX");
		#endif
		attrset(COLOR_PAIR(COLOR_PAIR_BASIC));
		break;
		case G_YELLOW_FLAG:
		attrset(COLOR_PAIR(COLOR_PAIR_YELLOW));
		#ifdef TRAFFIC_BIG
		move(CURSES_TRAFFICLIGHT_LINE_Y,20);
		printw("  ***      ***      ***      ***      ***  ");   /* do later on with addch(ACS_CKBOARD); */
		move(CURSES_TRAFFICLIGHT_LINE_Y+1,20);
		printw(" *****    *****    *****    *****    ***** ");
		move(CURSES_TRAFFICLIGHT_LINE_Y+2,20);
		printw("*******  *******  *******  *******  *******");
		move(CURSES_TRAFFICLIGHT_LINE_Y+3,20);
		printw("*******  *******  *******  *******  *******");
		move(CURSES_TRAFFICLIGHT_LINE_Y+4,20);
		printw(" *****    *****    *****    *****    ***** ");
		move(CURSES_TRAFFICLIGHT_LINE_Y+5,20);
		printw("  ***      ***      ***      ***      ***  ");
		#endif
		#ifdef TRAFFIC_SMALL
		move(CURSES_TRAFFICLIGHT_LINE_Y,0);
		printw("XXXXXXXXXX       XXXXXXXXXX        XXXXXXXXXX        XXXXXXXXXX       XXXXXXXXXX");
		move(CURSES_TRAFFICLIGHT_LINE_Y+1,0);
		printw("XXXXXXXXXX       XXXXXXXXXX        XXXXXXXXXX        XXXXXXXXXX       XXXXXXXXXX");
		move(CURSES_TRAFFICLIGHT_LINE_Y+2,0);
		printw("XXXXXXXXXX       XXXXXXXXXX        XXXXXXXXXX        XXXXXXXXXX       XXXXXXXXXX");
		#endif
		attrset(COLOR_PAIR(COLOR_PAIR_BASIC));
		break;
		case G_BLUE_FLAG:
		attrset(COLOR_PAIR(COLOR_PAIR_BLUE));
		#ifdef TRAFFIC_BIG
		move(CURSES_TRAFFICLIGHT_LINE_Y,20);
		printw("  ***      ***      ***      ***      ***  ");   /* do later on with addch(ACS_CKBOARD); */
		move(CURSES_TRAFFICLIGHT_LINE_Y+1,20);
		printw(" *****    *****    *****    *****    ***** ");
		move(CURSES_TRAFFICLIGHT_LINE_Y+2,20);
		printw("*******  *******  *******  *******  *******");
		move(CURSES_TRAFFICLIGHT_LINE_Y+3,20);
		printw("*******  *******  *******  *******  *******");
		move(CURSES_TRAFFICLIGHT_LINE_Y+4,20);
		printw(" *****    *****    *****    *****    ***** ");
		move(CURSES_TRAFFICLIGHT_LINE_Y+5,20);
		printw("  ***      ***      ***      ***      ***  ");
		#endif
		#ifdef TRAFFIC_SMALL
		move(CURSES_TRAFFICLIGHT_LINE_Y,0);
		printw("XXXXXXXXXX       XXXXXXXXXX        XXXXXXXXXX        XXXXXXXXXX       XXXXXXXXXX");
		move(CURSES_TRAFFICLIGHT_LINE_Y+1,0);
		printw("XXXXXXXXXX       XXXXXXXXXX        XXXXXXXXXX        XXXXXXXXXX       XXXXXXXXXX");
		move(CURSES_TRAFFICLIGHT_LINE_Y+2,0);
		printw("XXXXXXXXXX       XXXXXXXXXX        XXXXXXXXXX        XXXXXXXXXX       XXXXXXXXXX");
		#endif
		attrset(COLOR_PAIR(COLOR_PAIR_BASIC));
		break;
		case G_CHECKERED_FLAG:
		attrset(COLOR_PAIR(COLOR_PAIR_BASIC));
		#ifdef TRAFFIC_BIG
		move(CURSES_TRAFFICLIGHT_LINE_Y,20);
		printw("    *****     *****     *****     *****    ");   /* do later on with addch(ACS_CKBOARD); */
		move(CURSES_TRAFFICLIGHT_LINE_Y+1,20);
		printw("    *****     *****     *****     *****    ");   /* do later on with addch(ACS_CKBOARD); */
		move(CURSES_TRAFFICLIGHT_LINE_Y+2,20);
		printw("         *****     *****     *****         ");
		move(CURSES_TRAFFICLIGHT_LINE_Y+3,20);
		printw("         *****     *****     *****         ");
		move(CURSES_TRAFFICLIGHT_LINE_Y+4,20);
		printw("    *****     *****     *****     *****    ");
		move(CURSES_TRAFFICLIGHT_LINE_Y+5,20);
		printw("    *****     *****     *****     *****    ");
		#endif
		#ifdef TRAFFIC_SMALL
		move(CURSES_TRAFFICLIGHT_LINE_Y,0);
		printw("XXXXXXXXXX          XXXXXXXXXX          XXXXXXXXXX          XXXXXXXXXX          ");
		move(CURSES_TRAFFICLIGHT_LINE_Y+1,0);
		printw("          XXXXXXXXXX          XXXXXXXXXX          XXXXXXXXXX          XXXXXXXXXX");
		move(CURSES_TRAFFICLIGHT_LINE_Y+2,0);
		printw("XXXXXXXXXX          XXXXXXXXXX          XXXXXXXXXX          XXXXXXXXXX          ");
		#endif
		attrset(COLOR_PAIR(COLOR_PAIR_BASIC));
		break;
		default:
		move(CUR_MESSAGE_LINE_Y,CUR_MESSAGE_LINE_X);  /* (y,x) */
		printw("TrafficLight: state NOT defined : %d\n          ",state);
		break;
	}

}

void TrafficLightMidErase(int lower)
{
		#ifdef TRAFFIC_BIG
		if((lower < 0) || (lower > 5))
		#endif
		#ifdef TRAFFIC_SMALL
		if((lower < 0) || (lower > 3))
		#endif
			return;
		move(CURSES_TOGO_LINE_Y+0+lower,10);
		printw("                                                            ");
		move(CURSES_TOGO_LINE_Y+1+lower,10);
		printw("                                                            ");
		move(CURSES_TOGO_LINE_Y+2+lower,10);
		printw("                                                            ");
}

/* TrafficLightHard() shows the Traffic Light external defined panel */
void TrafficLightHard(int state)
{
	#ifdef SN3218
	int i;				/* scratch */
	#endif

	switch(state) {

		case G_RED_0:
		case G_BLANK:
		#ifdef I2C
		yaSetMCP23017(MCP23017_RED_1,YA_LOW);
		yaSetMCP23017(MCP23017_RED_2,YA_LOW);
		yaSetMCP23017(MCP23017_RED_3,YA_LOW);
		yaSetMCP23017(MCP23017_RED_4,YA_LOW);
		yaSetMCP23017(MCP23017_RED_5,YA_LOW);
		yaSetMCP23017(MCP23017_RED_6,YA_LOW);
		yaSetMCP23017(MCP23017_RED_7,YA_LOW);
		yaSetMCP23017(MCP23017_RED_8,YA_LOW);
		yaSetMCP23017(MCP23017_RED_9,YA_LOW);
		yaSetMCP23017(MCP23017_RED_10,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_1,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_2,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_3,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_4,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_5,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_1,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_2,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_3,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_4,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_5,YA_LOW);
		#endif /* I2C */
		#ifdef SN3218
		for (i = 0 ; i < 8 ; i++)						/* clear 8 LED's connected back to ZERO */
			analogWrite(MCP23017_pinBase + i, SN3218_OFF);
		#endif
		#ifdef MCP23017
		/* clear all frontside - LEDs */
		digitalWrite(MCP23017_pinBase + MCP23017_RED_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_6,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_7,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_8,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_9,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_10,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_5,MCP23017_OFF);
		#endif
		#if defined(MCP23017EASY) || defined(MCP23008)
		/* clear all frontside - LEDs */
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_GREEN,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_YELLOW,MCP23017_OFF);
		#endif
		break;
		case G_RED_1:
		#ifdef I2C
		yaSetMCP23017(MCP23017_RED_1,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_2,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_3,YA_LOW);
		yaSetMCP23017(MCP23017_RED_4,YA_LOW);
		yaSetMCP23017(MCP23017_RED_5,YA_LOW);
		yaSetMCP23017(MCP23017_RED_6,YA_LOW);
		yaSetMCP23017(MCP23017_RED_7,YA_LOW);
		yaSetMCP23017(MCP23017_RED_8,YA_LOW);
		yaSetMCP23017(MCP23017_RED_9,YA_LOW);
		yaSetMCP23017(MCP23017_RED_10,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_1,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_2,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_3,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_4,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_5,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_1,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_2,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_3,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_4,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_5,YA_LOW);
		#endif /* I2C */
		#ifdef SN3218
		analogWrite(MCP23017_pinBase + 0, SN3218_ON);
		analogWrite(MCP23017_pinBase + 1, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 2, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 3, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 4, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 5, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 6, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 7, SN3218_OFF);
		#endif
		#ifdef MCP23017
		digitalWrite(MCP23017_pinBase + MCP23017_RED_1,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_2,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_6,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_7,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_8,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_9,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_10,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_5,MCP23017_OFF);
		#endif
		#if defined(MCP23017EASY) || defined(MCP23008)
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_1,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_GREEN,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_YELLOW,MCP23017_OFF);
		#endif
		break;
		case G_RED_2:
		#ifdef I2C
		yaSetMCP23017(MCP23017_RED_1,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_2,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_3,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_4,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_5,YA_LOW);
		yaSetMCP23017(MCP23017_RED_6,YA_LOW);
		yaSetMCP23017(MCP23017_RED_7,YA_LOW);
		yaSetMCP23017(MCP23017_RED_8,YA_LOW);
		yaSetMCP23017(MCP23017_RED_9,YA_LOW);
		yaSetMCP23017(MCP23017_RED_10,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_1,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_2,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_3,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_4,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_5,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_1,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_2,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_3,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_4,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_5,YA_LOW);
		#endif /* I2C */
		#ifdef SN3218
		analogWrite(MCP23017_pinBase + 0, SN3218_ON);
		analogWrite(MCP23017_pinBase + 1, SN3218_ON);
		analogWrite(MCP23017_pinBase + 2, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 3, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 4, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 5, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 6, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 7, SN3218_OFF);
		#endif
		#ifdef MCP23017
		digitalWrite(MCP23017_pinBase + MCP23017_RED_1,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_2,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_3,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_4,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_6,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_7,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_8,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_9,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_10,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_5,MCP23017_OFF);
		#endif
		#if defined(MCP23017EASY) || defined(MCP23008)
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_1,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_2,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_GREEN,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_YELLOW,MCP23017_OFF);
		#endif
		break;
		case G_RED_3:
		#ifdef I2C
		yaSetMCP23017(MCP23017_RED_1,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_2,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_3,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_4,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_5,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_6,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_7,YA_LOW);
		yaSetMCP23017(MCP23017_RED_8,YA_LOW);
		yaSetMCP23017(MCP23017_RED_9,YA_LOW);
		yaSetMCP23017(MCP23017_RED_10,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_1,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_2,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_3,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_4,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_5,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_1,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_2,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_3,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_4,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_5,YA_LOW);
		#endif /* I2C */
		#ifdef SN3218
		analogWrite(MCP23017_pinBase + 0, SN3218_ON);
		analogWrite(MCP23017_pinBase + 1, SN3218_ON);
		analogWrite(MCP23017_pinBase + 2, SN3218_ON);
		analogWrite(MCP23017_pinBase + 3, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 4, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 5, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 6, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 7, SN3218_OFF);
		#endif
		#ifdef MCP23017
		digitalWrite(MCP23017_pinBase + MCP23017_RED_1,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_2,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_3,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_4,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_5,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_6,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_7,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_8,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_9,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_10,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_5,MCP23017_OFF);
		#endif
		#if defined(MCP23017EASY) || defined(MCP23008)
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_1,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_2,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_3,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_GREEN,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_YELLOW,MCP23017_OFF);
		#endif
		break;
		case G_RED_4:
		#ifdef I2C
		yaSetMCP23017(MCP23017_RED_1,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_2,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_3,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_4,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_5,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_6,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_7,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_8,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_9,YA_LOW);
		yaSetMCP23017(MCP23017_RED_10,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_1,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_2,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_3,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_4,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_5,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_1,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_2,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_3,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_4,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_5,YA_LOW);
		#endif /* I2C */
		#ifdef SN3218
		analogWrite(MCP23017_pinBase + 0, SN3218_ON);
		analogWrite(MCP23017_pinBase + 1, SN3218_ON);
		analogWrite(MCP23017_pinBase + 2, SN3218_ON);
		analogWrite(MCP23017_pinBase + 3, SN3218_ON);
		analogWrite(MCP23017_pinBase + 4, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 5, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 6, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 7, SN3218_OFF);
		#endif
		#ifdef MCP23017
		digitalWrite(MCP23017_pinBase + MCP23017_RED_1,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_2,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_3,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_4,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_5,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_6,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_7,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_8,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_9,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_10,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_5,MCP23017_OFF);
		#endif
		#if defined(MCP23017EASY) || defined(MCP23008)
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_1,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_2,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_3,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_4,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_GREEN,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_YELLOW,MCP23017_OFF);
		#endif		
		break;
		case G_RED_5:
		case G_RED_FLAG:
		#ifdef I2C
		yaSetMCP23017(MCP23017_RED_1,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_2,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_3,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_4,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_5,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_6,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_7,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_8,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_9,YA_HIGH);
		yaSetMCP23017(MCP23017_RED_10,YA_HIGH);
		yaSetMCP23017(MCP23017_GREEN_1,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_2,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_3,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_4,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_5,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_1,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_2,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_3,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_4,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_5,YA_LOW);
		#endif /* I2C */
		#ifdef SN3218
		analogWrite(MCP23017_pinBase + 0, SN3218_ON);
		analogWrite(MCP23017_pinBase + 1, SN3218_ON);
		analogWrite(MCP23017_pinBase + 2, SN3218_ON);
		analogWrite(MCP23017_pinBase + 3, SN3218_ON);
		analogWrite(MCP23017_pinBase + 4, SN3218_ON);
		analogWrite(MCP23017_pinBase + 5, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 6, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 7, SN3218_OFF);
		#endif
		#ifdef MCP23017
		digitalWrite(MCP23017_pinBase + MCP23017_RED_1,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_2,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_3,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_4,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_5,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_6,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_7,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_8,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_9,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_10,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_5,MCP23017_OFF);
		#endif
		#if defined(MCP23017EASY) || defined(MCP23008)
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_1,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_2,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_3,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_4,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_5,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017E_GREEN,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_YELLOW,MCP23017_OFF);
		#endif
		break;
		case G_GREEN:
		case G_GREEN_FLAG:
		#ifdef I2C
		yaSetMCP23017(MCP23017_RED_1,YA_LOW);
		yaSetMCP23017(MCP23017_RED_2,YA_LOW);
		yaSetMCP23017(MCP23017_RED_3,YA_LOW);
		yaSetMCP23017(MCP23017_RED_4,YA_LOW);
		yaSetMCP23017(MCP23017_RED_5,YA_LOW);
		yaSetMCP23017(MCP23017_RED_6,YA_LOW);
		yaSetMCP23017(MCP23017_RED_7,YA_LOW);
		yaSetMCP23017(MCP23017_RED_8,YA_LOW);
		yaSetMCP23017(MCP23017_RED_9,YA_LOW);
		yaSetMCP23017(MCP23017_RED_10,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_1,YA_HIGH);
		yaSetMCP23017(MCP23017_GREEN_2,YA_HIGH);
		yaSetMCP23017(MCP23017_GREEN_3,YA_HIGH);
		yaSetMCP23017(MCP23017_GREEN_4,YA_HIGH);
		yaSetMCP23017(MCP23017_GREEN_5,YA_HIGH);
		yaSetMCP23017(MCP23017_YELLOW_1,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_2,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_3,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_4,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_5,YA_LOW);
		#endif /* I2C */
		#ifdef SN3218
		analogWrite(MCP23017_pinBase + 0, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 1, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 2, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 3, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 4, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 5, SN3218_ON);
		analogWrite(MCP23017_pinBase + 6, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 7, SN3218_OFF);
		#endif
		#ifdef MCP23017
		digitalWrite(MCP23017_pinBase + MCP23017_RED_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_6,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_7,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_8,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_9,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_10,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_1,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_2,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_3,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_4,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_5,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_5,MCP23017_OFF);
		#endif
		#if defined(MCP23017EASY) || defined(MCP23008)
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_GREEN,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017E_YELLOW,MCP23017_OFF);
		#endif
		break;
		case G_YELLOW_FLAG:
		#ifdef I2C
		yaSetMCP23017(MCP23017_RED_1,YA_LOW);
		yaSetMCP23017(MCP23017_RED_2,YA_LOW);
		yaSetMCP23017(MCP23017_RED_3,YA_LOW);
		yaSetMCP23017(MCP23017_RED_4,YA_LOW);
		yaSetMCP23017(MCP23017_RED_5,YA_LOW);
		yaSetMCP23017(MCP23017_RED_6,YA_LOW);
		yaSetMCP23017(MCP23017_RED_7,YA_LOW);
		yaSetMCP23017(MCP23017_RED_8,YA_LOW);
		yaSetMCP23017(MCP23017_RED_9,YA_LOW);
		yaSetMCP23017(MCP23017_RED_10,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_1,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_2,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_3,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_4,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_5,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_1,YA_HIGH);
		yaSetMCP23017(MCP23017_YELLOW_2,YA_HIGH);
		yaSetMCP23017(MCP23017_YELLOW_3,YA_HIGH);
		yaSetMCP23017(MCP23017_YELLOW_4,YA_HIGH);
		yaSetMCP23017(MCP23017_YELLOW_5,YA_HIGH);
		#endif /* I2C */
		#ifdef SN3218
		analogWrite(MCP23017_pinBase + 0, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 1, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 2, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 3, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 4, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 5, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 6, SN3218_ON);
		analogWrite(MCP23017_pinBase + 7, SN3218_OFF);
		#endif
		#ifdef MCP23017
		digitalWrite(MCP23017_pinBase + MCP23017_RED_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_6,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_7,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_8,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_9,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_10,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_1,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_2,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_3,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_4,MCP23017_ON);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_5,MCP23017_ON);
		#endif
		#if defined(MCP23017EASY) || defined(MCP23008)
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_GREEN,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_YELLOW,MCP23017_ON);
		#endif
		break;
		case G_BLUE_FLAG:
		#ifdef I2C
		yaSetMCP23017(MCP23017_RED_1,YA_LOW);
		yaSetMCP23017(MCP23017_RED_2,YA_LOW);
		yaSetMCP23017(MCP23017_RED_3,YA_LOW);
		yaSetMCP23017(MCP23017_RED_4,YA_LOW);
		yaSetMCP23017(MCP23017_RED_5,YA_LOW);
		yaSetMCP23017(MCP23017_RED_6,YA_LOW);
		yaSetMCP23017(MCP23017_RED_7,YA_LOW);
		yaSetMCP23017(MCP23017_RED_8,YA_LOW);
		yaSetMCP23017(MCP23017_RED_9,YA_LOW);
		yaSetMCP23017(MCP23017_RED_10,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_1,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_2,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_3,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_4,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_5,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_1,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_2,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_3,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_4,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_5,YA_LOW);
		#endif /* I2C */
		#ifdef SN3218
		analogWrite(MCP23017_pinBase + 0, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 1, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 2, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 3, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 4, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 5, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 6, SN3218_OFF);
		analogWrite(MCP23017_pinBase + 7, SN3218_ON);
		#endif
		#ifdef MCP23017
		digitalWrite(MCP23017_pinBase + MCP23017_RED_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_6,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_7,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_8,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_9,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_10,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_5,MCP23017_OFF);
		#endif

		#if defined(MCP23017EASY) || defined(MCP23008)
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_GREEN,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_YELLOW,MCP23017_OFF);
		#endif
		break;
		case G_CHECKERED_FLAG:
		#ifdef I2C
		yaSetMCP23017(MCP23017_RED_1,YA_LOW);
		yaSetMCP23017(MCP23017_RED_2,YA_LOW);
		yaSetMCP23017(MCP23017_RED_3,YA_LOW);
		yaSetMCP23017(MCP23017_RED_4,YA_LOW);
		yaSetMCP23017(MCP23017_RED_5,YA_LOW);
		yaSetMCP23017(MCP23017_RED_6,YA_LOW);
		yaSetMCP23017(MCP23017_RED_7,YA_LOW);
		yaSetMCP23017(MCP23017_RED_8,YA_LOW);
		yaSetMCP23017(MCP23017_RED_9,YA_LOW);
		yaSetMCP23017(MCP23017_RED_10,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_1,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_2,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_3,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_4,YA_LOW);
		yaSetMCP23017(MCP23017_GREEN_5,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_1,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_2,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_3,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_4,YA_LOW);
		yaSetMCP23017(MCP23017_YELLOW_5,YA_LOW);
		#endif /* I2C */
		#ifdef SN3218
		for (i = 0 ; i < 8 ; i++)						/* clear 8 LED's connected back to ZERO */
			analogWrite(MCP23017_pinBase + i, SN3218_OFF);
		#endif
		#ifdef MCP23017
		/* clear all front-LEDs */
		digitalWrite(MCP23017_pinBase + MCP23017_RED_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_6,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_7,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_8,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_9,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_RED_10,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_GREEN_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017_YELLOW_5,MCP23017_OFF);
		#endif
		#if defined(MCP23017EASY) || defined(MCP23008)
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_1,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_2,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_3,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_4,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_RED_5,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_GREEN,MCP23017_OFF);
		digitalWrite(MCP23017_pinBase + MCP23017E_YELLOW,MCP23017_OFF);
		#endif
		break;
		default:
		printmessage("TrafficLight: state NOT defined : %d\n",state);
		break;
	}
}

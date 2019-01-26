/*  2017-01-17 19:10  */
/* 
    yast - yet another slotcar timer
	File: font_3x.h -> ASCII font with ncurses, 3x3 and 3x5, based on codepage CP437
	
    Copyright (C)  2015,2016,2017 Martin Berentsen


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

/* #define BASICCHARSETDEMO 1 */ 
 
int mvprintnum3x3(int y, int x, int number );
int mvprintnum3x5r(int y, int x, int number );
int mvprintnum3x5(int y, int x, int number );
int mvprintnum3x(int y, int x, int number );
void print_lap(int track, unsigned long ul_LapCount);
void print_last(int track,unsigned long ul_LapLast);
void print_fastes(int track,unsigned long ul_LapFast);
int print_laptogo(unsigned long laptogo, int lower);
int print_timetogo(unsigned long timetogo,int lower);
int print_realtime(int hour,int minutes,int lower);
#ifdef BASICCHARSETDEMO
 void font3xout(void);
#endif
 

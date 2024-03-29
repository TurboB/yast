/*  2024-03-29 14:00  */
/*
    yast - yet another slotcar timer
	File: font_3x.h -> ASCII font with ncurses, 3x3 and 3x5, based on codepage CP437

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

/*  meaning of codepage CP437 used chars, also in unicode
	0xdb = 219 = full  => 0x2588
	0xdc = 220 = lower => 0x2584
	0xdd = 221 = left  => 0x258c
	0xde = 222 = right => 0x2590
	0xdf = 223 = upper => 0x2580
	0x20 =  32 = blanc => 0x0020
*/

/* #include <wchar.h> */
#include <curses.h>
/* #include <ncurses.h> */
/* #include <wctype.h> */
#include <math.h>

#include "font_3x.h"
#include "yast_define.h"

/*  print out a number between 0 and 9  */
/*  at the position of x,y with a size  */
/*  of 3x3, insert point is top-left	*/
/*  small SRM version */
int mvprintnum3x3(int y, int x, int number ) {
 	int iret = 0;
 		switch(number) {
 			case 0:
#ifdef UNICODE
 				mvprintw(y  ,x,"\u2588\u2580\u2588");
 				mvprintw(y+1,x,"\u2588\x20\u2588");
 				mvprintw(y+2,x,"\u2588\u2584\u2588");
#else /* UNICODE */
 				mvprintw(y  ,x,"\xdb\xdf\xdb");
 				mvprintw(y+1,x,"\xdb\x20\xdb");
 				mvprintw(y+2,x,"\xdb\xdc\xdb");
#endif /* UNICODE */
			 	break;
			case 1:
#ifdef UNICODE
 				mvprintw(y  ,x,"\u2584\u2588\x20");
 				mvprintw(y+1,x,"\x20\u2588\x20");
 				mvprintw(y+2,x,"\u2584\u2588\u2584");
#else /* UNICODE */
 				mvprintw(y  ,x,"\xdc\xdb\x20");
 				mvprintw(y+1,x,"\x20\xdb\x20");
 				mvprintw(y+2,x,"\xdc\xdb\xdc");
#endif /* UNICODE */
 			 	break;
			case 2:
#ifdef UNICODE
 				mvprintw(y  ,x,"\u2580\u2580\u2588");
 				mvprintw(y+1,x,"\u2588\u2580\u2580");
 				mvprintw(y+2,x,"\u2588\u2584\u2584");
#else /* UNICODE */
 				mvprintw(y  ,x,"\xdf\xdf\xdb");
 				mvprintw(y+1,x,"\xdb\xdf\xdf");
 				mvprintw(y+2,x,"\xdb\xdc\xdc");
#endif /* UNICODE */
			 	break;
			case 3:
#ifdef UNICODE
 				mvprintw(y  ,x,"\u2580\u2580\u2588");
 				mvprintw(y+1,x,"\x20\u2580\u2588");
 				mvprintw(y+2,x,"\u2584\u2584\u2588");
#else /* UNICODE */
 				mvprintw(y  ,x,"\xdf\xdf\xdb");
 				mvprintw(y+1,x,"\x20\xdf\xdb");
 				mvprintw(y+2,x,"\xdc\xdc\xdb");
#endif /* UNICODE */
			 	break;
			case 4:
#ifdef UNICODE
 				mvprintw(y  ,x,"\u2584\x20\u2588");
 				mvprintw(y+1,x,"\u2588\u2584\u2588");
 				mvprintw(y+2,x,"\x20\x20\u2588");
#else /* UNICODE */
 				mvprintw(y  ,x,"\xdc\x20\xdb");
 				mvprintw(y+1,x,"\xdb\xdc\xdb");
 				mvprintw(y+2,x,"\x20\x20\xdb");
#endif /* UNICODE */
			 	break;
			case 5:
#ifdef UNICODE
 				mvprintw(y  ,x,"\u2588\u2580\u2580");
 				mvprintw(y+1,x,"\u2580\u2580\u2588");
 				mvprintw(y+2,x,"\u2584\u2584\u2588");
#else /* UNICODE */
 				mvprintw(y  ,x,"\xdb\xdf\xdf");
 				mvprintw(y+1,x,"\xdf\xdf\xdb");
 				mvprintw(y+2,x,"\xdc\xdc\xdb");
#endif /* UNICODE */
			 	break;
			case 6:
#ifdef UNICODE
 				mvprintw(y  ,x,"\u2588\u2580\u2580");
 				mvprintw(y+1,x,"\u2588\u2580\u2588");
 				mvprintw(y+2,x,"\u2588\u2584\u2588");
#else /* UNICODE */
 				mvprintw(y  ,x,"\xdb\xdf\xdf");
 				mvprintw(y+1,x,"\xdb\xdf\xdb");
 				mvprintw(y+2,x,"\xdb\xdc\xdb");
#endif /* UNICODE */
			 	break;
			case 7:
#ifdef UNICODE
 				mvprintw(y  ,x,"\u2580\u2580\u2588");
 				mvprintw(y+1,x,"\x20\x20\u2588");
 				mvprintw(y+2,x,"\x20\x20\u2588");
#else /* UNICODE */
 				mvprintw(y  ,x,"\xdf\xdf\xdb");
 				mvprintw(y+1,x,"\x20\x20\xdb");
 				mvprintw(y+2,x,"\x20\x20\xdb");
#endif /* UNICODE */
			 	break;
			case 8:
#ifdef UNICODE
 				mvprintw(y  ,x,"\u2588\u2580\u2588");
 				mvprintw(y+1,x,"\u2588\u2580\u2588");
 				mvprintw(y+2,x,"\u2588\u2584\u2588");
#else /* UNICODE */
 				mvprintw(y  ,x,"\xdb\xdf\xdb");
 				mvprintw(y+1,x,"\xdb\xdf\xdb");
 				mvprintw(y+2,x,"\xdb\xdc\xdb");
#endif /* UNICODE */
				break;
			case 9:
#ifdef UNICODE
 				mvprintw(y  ,x,"\u2588\u2580\u2588");
 				mvprintw(y+1,x,"\u2580\u2580\u2588");
 				mvprintw(y+2,x,"\u2584\u2584\u2588");
#else /* UNICODE */
 				mvprintw(y  ,x,"\xdb\xdf\xdb");
 				mvprintw(y+1,x,"\xdf\xdf\xdb");
 				mvprintw(y+2,x,"\xdc\xdc\xdb");
#endif /* UNICODE */
				break;
			default:
				iret = -1;
				break;
 		}
 		return iret;
 }

/*  print out a number between 0 and 9  */
/*  at the position of x,y with a size  */
/*  of 3x5, insert point is top-left	*/
/*  rustic version */
int mvprintnum3x5r(int y, int x, int number ) {
 	int iret = 0;
 		switch(number) {
 			case 0:
 				mvprintw(y  ,x,"\xdb\xdb\xdb");
 				mvprintw(y+1,x,"\xdb\x20\xdb");
 				mvprintw(y+2,x,"\xdb\x20\xdb");
 				mvprintw(y+3,x,"\xdb\x20\xdb");
 				mvprintw(y+4,x,"\xdb\xdb\xdb");
			 	break;
			case 1:
 				mvprintw(y  ,x,"\x20\xdb\x20");
 				mvprintw(y+1,x,"\xdb\xdb\x20");
 				mvprintw(y+2,x,"\x20\xdb\x20");
 				mvprintw(y+3,x,"\x20\xdb\x20");
 				mvprintw(y+4,x,"\xdb\xdb\xdb");
 			 	break;
			case 2:
 				mvprintw(y  ,x,"\xdb\xdb\xdb");
 				mvprintw(y+1,x,"\x20\x20\xdb");
 				mvprintw(y+2,x,"\x20\xdb\x20");
 				mvprintw(y+3,x,"\xdb\x20\x20");
 				mvprintw(y+4,x,"\xdb\xdb\xdb");
			 	break;
			case 3:
 				mvprintw(y  ,x,"\xdb\xdb\xdb");
 				mvprintw(y+1,x,"\x20\x20\xdb");
 				mvprintw(y+2,x,"\x20\xdb\xdb");
 				mvprintw(y+3,x,"\x20\x20\xdb");
 				mvprintw(y+4,x,"\xdb\xdb\xdb");
			 	break;
			case 4:
 				mvprintw(y  ,x,"\xdb\x20\x20");
 				mvprintw(y+1,x,"\xdb\x20\xdb");
 				mvprintw(y+2,x,"\xdb\xdb\xdb");
 				mvprintw(y+3,x,"\x20\x20\xdb");
 				mvprintw(y+4,x,"\x20\x20\xdb");
			 	break;
			case 5:
 				mvprintw(y  ,x,"\xdb\xdb\xdb");
 				mvprintw(y+1,x,"\xdb\x20\x20");
 				mvprintw(y+2,x,"\xdb\xdb\xdb");
 				mvprintw(y+3,x,"\x20\x20\xdb");
 				mvprintw(y+4,x,"\xdb\xdb\xdb");
			 	break;
			case 6:
 				mvprintw(y  ,x,"\xdb\xdb\xdb");
 				mvprintw(y+1,x,"\xdb\x20\x20");
 				mvprintw(y+2,x,"\xdb\xdb\xdb");
 				mvprintw(y+3,x,"\xdb\x20\xdb");
 				mvprintw(y+4,x,"\xdb\xdb\xdb");
			 	break;
			case 7:
 				mvprintw(y  ,x,"\xdb\xdb\xdb");
 				mvprintw(y+1,x,"\x20\x20\xdb");
 				mvprintw(y+2,x,"\x20\xdb\x20");
 				mvprintw(y+3,x,"\x20\xdb\x20");
 				mvprintw(y+4,x,"\x20\xdb\x20");
			 	break;
			case 8:
 				mvprintw(y  ,x,"\xdb\xdb\xdb");
 				mvprintw(y+1,x,"\xdb\x20\xdb");
 				mvprintw(y+2,x,"\xdb\xdb\xdb");
 				mvprintw(y+3,x,"\xdb\x20\xdb");
 				mvprintw(y+4,x,"\xdb\xdb\xdb");
				break;
			case 9:
 				mvprintw(y  ,x,"\xdb\xdb\xdb");
 				mvprintw(y+1,x,"\xdb\x20\xdb");
 				mvprintw(y+2,x,"\xdb\xdb\xdb");
 				mvprintw(y+3,x,"\x20\x20\xdb");
 				mvprintw(y+4,x,"\xdb\xdb\xdb");
				 break;
			default:
				iret = -1;
				break;
 		}
 		return iret;
 }

/*  print out a number between 0 and 9  */
/*  at the position of x,y with a size  */
/*  of 3x5, insert point is top-left	*/
/*  smooth version */
int mvprintnum3x5(int y, int x, int number ) {
 	int iret = 0;
 		switch(number) {
 			case 0:

#ifdef UNICODE
 				mvprintw(y  ,x,"\u2588\u2588\u2588");
 				mvprintw(y+1,x,"\u258c\x20\u2590");
 				mvprintw(y+2,x,"\u258c\x20\u2590");
 				mvprintw(y+3,x,"\u258c\x20\u2590");
 				mvprintw(y+4,x,"\u2588\u2588\u2588");
#else /* UNICODE */
 				mvprintw(y  ,x,"\xdb\xdb\xdb");
 				mvprintw(y+1,x,"\xdd\x20\xde");
 				mvprintw(y+2,x,"\xdd\x20\xde");
 				mvprintw(y+3,x,"\xdd\x20\xde");
 				mvprintw(y+4,x,"\xdb\xdb\xdb");
#endif /* UNICODE */
			 	break;
			case 1:
#ifdef UNICODE
 				mvprintw(y  ,x,"\x20\u2588\x20");
 				mvprintw(y+1,x,"\u2588\u2588\x20");
 				mvprintw(y+2,x,"\x20\u2588\x20");
 				mvprintw(y+3,x,"\x20\u2588\x20");
 				mvprintw(y+4,x,"\u2588\u2588\u2588");
#else /* UNICODE */
 				mvprintw(y  ,x,"\x20\xdb\x20");
 				mvprintw(y+1,x,"\xdb\xdb\x20");
 				mvprintw(y+2,x,"\x20\xdb\x20");
 				mvprintw(y+3,x,"\x20\xdb\x20");
 				mvprintw(y+4,x,"\xdb\xdb\xdb");
#endif /* UNICODE */
 			 	break;
			case 2:
#ifdef UNICODE
 				mvprintw(y  ,x,"\u2588\u2588\u2588");
 				mvprintw(y+1,x,"\x20\x20\u2590");
 				mvprintw(y+2,x,"\u2588\u2588\u2588");
 				mvprintw(y+3,x,"\u258c\x20\x20");
 				mvprintw(y+4,x,"\u2588\u2588\u2588");
#else /* UNICODE */
 				mvprintw(y  ,x,"\xdb\xdb\xdb");
 				mvprintw(y+1,x,"\x20\x20\xde");
 				mvprintw(y+2,x,"\xdb\xdb\xdb");
 				mvprintw(y+3,x,"\xdd\x20\x20");
 				mvprintw(y+4,x,"\xdb\xdb\xdb");
#endif /* UNICODE */
			 	break;
			case 3:
#ifdef UNICODE
 				mvprintw(y  ,x,"\u2588\u2588\u2588");
 				mvprintw(y+1,x,"\x20\x20\u2590");
 				mvprintw(y+2,x,"\x20\u2588\u2588");
 				mvprintw(y+3,x,"\x20\x20\u2590");
 				mvprintw(y+4,x,"\u2588\u2588\u2588");
#else /* UNICODE */
 				mvprintw(y  ,x,"\xdb\xdb\xdb");
 				mvprintw(y+1,x,"\x20\x20\xde");
 				mvprintw(y+2,x,"\x20\xdb\xdb");
 				mvprintw(y+3,x,"\x20\x20\xde");
 				mvprintw(y+4,x,"\xdb\xdb\xdb");
#endif /* UNICODE */
			 	break;
			case 4:
#ifdef UNICODE
 				mvprintw(y  ,x,"\u258c\x20\x20");
 				mvprintw(y+1,x,"\u258c\x20\u258c");
 				mvprintw(y+2,x,"\u2588\u2588\u2588");
 				mvprintw(y+3,x,"\x20\x20\u258c");
 				mvprintw(y+4,x,"\x20\x20\u258c");
#else /* UNICODE */
 				mvprintw(y  ,x,"\xdd\x20\x20");
 				mvprintw(y+1,x,"\xdd\x20\xdd");
 				mvprintw(y+2,x,"\xdb\xdb\xdb");
 				mvprintw(y+3,x,"\x20\x20\xdd");
 				mvprintw(y+4,x,"\x20\x20\xdd");
#endif /* UNICODE */
			 	break;
			case 5:
#ifdef UNICODE
 				mvprintw(y  ,x,"\u2588\u2588\u2588");
 				mvprintw(y+1,x,"\u258c\x20\x20");
 				mvprintw(y+2,x,"\u2588\u2588\u2588");
 				mvprintw(y+3,x,"\x20\x20\u2590");
 				mvprintw(y+4,x,"\u2588\u2588\u2588");
#else /* UNICODE */
 				mvprintw(y  ,x,"\xdb\xdb\xdb");
 				mvprintw(y+1,x,"\xdd\x20\x20");
 				mvprintw(y+2,x,"\xdb\xdb\xdb");
 				mvprintw(y+3,x,"\x20\x20\xde");
 				mvprintw(y+4,x,"\xdb\xdb\xdb");
#endif /* UNICODE */
			 	break;
			case 6:
#ifdef UNICODE
 				mvprintw(y  ,x,"\u2588\u2588\u2588");
 				mvprintw(y+1,x,"\u258c\x20\x20");
 				mvprintw(y+2,x,"\u2588\u2588\u2588");
 				mvprintw(y+3,x,"\u258c\x20\u2590");
 				mvprintw(y+4,x,"\u2588\u2588\u2588");
#else /* UNICODE */
 				mvprintw(y  ,x,"\xdb\xdb\xdb");
 				mvprintw(y+1,x,"\xdd\x20\x20");
 				mvprintw(y+2,x,"\xdb\xdb\xdb");
 				mvprintw(y+3,x,"\xdd\x20\xde");
 				mvprintw(y+4,x,"\xdb\xdb\xdb");
#endif /* UNICODE */
			 	break;
			case 7:
#ifdef UNICODE
 				mvprintw(y  ,x,"\u2588\u2588\u2588");
 				mvprintw(y+1,x,"\x20\x20\u2588");
 				mvprintw(y+2,x,"\x20\u2590\u258c");
 				mvprintw(y+3,x,"\x20\u2588\x20");
 				mvprintw(y+4,x,"\x20\u2588\x20");
#else /* UNICODE */
 				mvprintw(y  ,x,"\xdb\xdb\xdb");
 				mvprintw(y+1,x,"\x20\x20\xdb");
 				mvprintw(y+2,x,"\x20\xde\xdd");
 				mvprintw(y+3,x,"\x20\xdb\x20");
 				mvprintw(y+4,x,"\x20\xdb\x20");
#endif /* UNICODE */
			 	break;
			case 8:
#ifdef UNICODE
 				mvprintw(y  ,x,"\u2588\u2588\u2588");
 				mvprintw(y+1,x,"\u258c\x20\u2590");
 				mvprintw(y+2,x,"\u2588\u2588\u2588");
 				mvprintw(y+3,x,"\u258c\x20\u2590");
 				mvprintw(y+4,x,"\u2588\u2588\u2588");
#else /* UNICODE */
 				mvprintw(y  ,x,"\xdb\xdb\xdb");
 				mvprintw(y+1,x,"\xdd\x20\xde");
 				mvprintw(y+2,x,"\xdb\xdb\xdb");
 				mvprintw(y+3,x,"\xdd\x20\xde");
 				mvprintw(y+4,x,"\xdb\xdb\xdb");
#endif /* UNICODE */
				break;
			case 9:
#ifdef UNICODE
 				mvprintw(y  ,x,"\u2588\u2588\u2588");
 				mvprintw(y+1,x,"\u258c\x20\u2590");
 				mvprintw(y+2,x,"\u2588\u2588\u2588");
 				mvprintw(y+3,x,"\x20\x20\u2590");
 				mvprintw(y+4,x,"\u2588\u2588\u2588");
#else /* UNICODE */
 				mvprintw(y  ,x,"\xdb\xdb\xdb");
 				mvprintw(y+1,x,"\xdd\x20\xde");
 				mvprintw(y+2,x,"\xdb\xdb\xdb");
 				mvprintw(y+3,x,"\x20\x20\xde");
 				mvprintw(y+4,x,"\xdb\xdb\xdb");
#endif /* UNICODE */
				break;
			default:
				iret = -1;
				break;
 		}
 		return iret;
 }

/*  this is a dummy mvprint for all 3x fonts */
 int mvprintnum3x(int y, int x, int number ) {
 	int iret = 0;

#ifdef FONT3X3
	iret = mvprintnum3x3(y, x, number );
#endif
#ifdef FONT3X5
	iret = mvprintnum3x5(y, x, number );
#endif

  return iret;
}

	/* insert point 6,1 (historic) */
	/* max.: 9999 laps   */
void print_lap(int track, unsigned long ul_LapCount){

	if(ul_LapCount > 9999)
		ul_LapCount = 9999;

	mvprintw(CURSES_TRACKRESULT_LINE_Y+1+(track*FONTHEIGHT),0,"%d",track+1);  /* print the track number */

	attrset(COLOR_PAIR(track+1));

	if( ((ul_LapCount)/1000%10)> 0)
		mvprintw(CURSES_TRACKRESULT_LINE_Y+2+(track*FONTHEIGHT), 1,"%ld",(ul_LapCount)/1000%10);         /*  print the first digit small */
	mvprintnum3x(CURSES_TRACKRESULT_LINE_Y+(track*FONTHEIGHT), CURSES_3x_LAPNUM_X+1,(int) (ul_LapCount)/100%10 );
	mvprintnum3x(CURSES_TRACKRESULT_LINE_Y+(track*FONTHEIGHT), CURSES_3x_LAPNUM_X+5,(int) (ul_LapCount)/10%10  );
	mvprintnum3x(CURSES_TRACKRESULT_LINE_Y+(track*FONTHEIGHT), CURSES_3x_LAPNUM_X+9,(int) (ul_LapCount)%10     );
	attrset(COLOR_PAIR(COLOR_PAIR_BASIC));
}
	/* insert point 6,12+1 (historic) */
	/* max.: 99.999 sec.   */
void print_last(int track,unsigned long ul_LapLast){

	if(ul_LapLast > 99999)
		ul_LapLast = 99999;

	attrset(COLOR_PAIR(track+1));
	mvprintnum3x(CURSES_TRACKRESULT_LINE_Y+(track*FONTHEIGHT), CURSES_3x_LAPLAST_X+5,(int) (ul_LapLast)/10000%10);
	mvprintnum3x(CURSES_TRACKRESULT_LINE_Y+(track*FONTHEIGHT), CURSES_3x_LAPLAST_X+9,(int) (ul_LapLast)/1000%10 );

	mvaddch(CURSES_TRACKRESULT_LINE_Y+DOTSHIFT+(track*FONTHEIGHT),CURSES_3x_LAPLAST_X+13,0xdb); /* decimal point */

	mvprintnum3x(CURSES_TRACKRESULT_LINE_Y+(track*FONTHEIGHT), CURSES_3x_LAPLAST_X+15,(int) (ul_LapLast)/100%10);
	mvprintnum3x(CURSES_TRACKRESULT_LINE_Y+(track*FONTHEIGHT), CURSES_3x_LAPLAST_X+19,(int) (ul_LapLast)/10%10 );
	mvprintnum3x(CURSES_TRACKRESULT_LINE_Y+(track*FONTHEIGHT), CURSES_3x_LAPLAST_X+23,(int) (ul_LapLast)%10    );
	attrset(COLOR_PAIR(COLOR_PAIR_BASIC));
}

	/* insert point 6,37+1 (historic) */
	/* max.: 99.999 sec.   */
void print_fastes(int track,unsigned long ul_LapFast){

	if(ul_LapFast > 99999)
		ul_LapFast = 99999;

	attrset(COLOR_PAIR(track+1));
	mvprintnum3x(CURSES_TRACKRESULT_LINE_Y+(track*FONTHEIGHT), CURSES_3x_LAPFAST_X+5,(int) (ul_LapFast)/10000%10);
	mvprintnum3x(CURSES_TRACKRESULT_LINE_Y+(track*FONTHEIGHT), CURSES_3x_LAPFAST_X+9,(int) (ul_LapFast)/1000%10 );

	mvaddch(CURSES_TRACKRESULT_LINE_Y+DOTSHIFT+(track*FONTHEIGHT),CURSES_3x_LAPFAST_X+13,0xdb); /* decimal point */

	mvprintnum3x(CURSES_TRACKRESULT_LINE_Y+(track*FONTHEIGHT), CURSES_3x_LAPFAST_X+15,(int) (ul_LapFast)/100%10);
	mvprintnum3x(CURSES_TRACKRESULT_LINE_Y+(track*FONTHEIGHT), CURSES_3x_LAPFAST_X+19,(int) (ul_LapFast)/10%10 );
	mvprintnum3x(CURSES_TRACKRESULT_LINE_Y+(track*FONTHEIGHT), CURSES_3x_LAPFAST_X+23,(int) (ul_LapFast)%10);
	attrset(COLOR_PAIR(COLOR_PAIR_BASIC));
}

/* ----------------------------------------------------------------------
int print_laptogo(unsigned long laptogo)
------------------------------------------------------------------------*/
int print_laptogo(unsigned long laptogo, int lower)
{
	if((lower < 0) || (lower > 3))
		return -1;

	mvprintnum3x3(CURSES_TOGO_LINE_Y+lower,CURSES_TOGO_LINE_X+ 2,(int) (laptogo)/1000%10 );
	mvprintnum3x3(CURSES_TOGO_LINE_Y+lower,CURSES_TOGO_LINE_X+ 6,(int) (laptogo)/100%10  );
	mvprintnum3x3(CURSES_TOGO_LINE_Y+lower,CURSES_TOGO_LINE_X+10,(int) (laptogo)/10%10  );
	mvprintnum3x3(CURSES_TOGO_LINE_Y+lower,CURSES_TOGO_LINE_X+14,(int) (laptogo)%10      );

	attrset(COLOR_PAIR(COLOR_PAIR_BASIC));
	return 0;
}

/* ----------------------------------------------------------------------
int print_timetogo(unsigned long laptogo)
------------------------------------------------------------------------*/
int print_timetogo(unsigned long timetogo,int lower)
{
        int togo;

	if((lower < 0) || (lower > 3))
		return -1;

	togo = (int)floorf((float)timetogo / 3600.0);
	mvprintnum3x3(CURSES_TOGO_LINE_Y+lower,CURSES_TOGO_LINE_X- 3,(int) (togo)/10%10  );
	mvprintnum3x3(CURSES_TOGO_LINE_Y+lower,CURSES_TOGO_LINE_X+ 1,(int) (togo)%10     );

	mvaddch(CURSES_TOGO_LINE_Y+0+lower,CURSES_TOGO_LINE_X+ 5,0xdc);
	mvaddch(CURSES_TOGO_LINE_Y+2+lower,CURSES_TOGO_LINE_X+ 5,0xdf);

	togo = (int)(floorf(timetogo/60.0))%60;
	mvprintnum3x3(CURSES_TOGO_LINE_Y+lower,CURSES_TOGO_LINE_X+ 7,(int) (togo)/10%10  );
	mvprintnum3x3(CURSES_TOGO_LINE_Y+lower,CURSES_TOGO_LINE_X+11,(int) (togo)%10     );

	mvaddch(CURSES_TOGO_LINE_Y+0+lower,CURSES_TOGO_LINE_X+ 15,0xdc);
	mvaddch(CURSES_TOGO_LINE_Y+2+lower,CURSES_TOGO_LINE_X+ 15,0xdf);

	togo = (int)(floorf(timetogo))%60;
	mvprintnum3x3(CURSES_TOGO_LINE_Y+lower,CURSES_TOGO_LINE_X+17,(int) (togo)/10%10  );
	mvprintnum3x3(CURSES_TOGO_LINE_Y+lower,CURSES_TOGO_LINE_X+21,(int) (togo)%10     );

	attrset(COLOR_PAIR(COLOR_PAIR_BASIC));
	return 0;
}

/* ----------------------------------------------------------------------
int print_realtime(int hour,int minutes,int lower)
------------------------------------------------------------------------*/
int print_realtime(int hour,int minutes,int lower)
{

	if((hour < 0) || (hour > 23))
		return -1;
	
	if((minutes < 0) || (minutes > 59))
		return -2;

	mvprintnum3x3(CURSES_TOGO_LINE_Y+lower,CURSES_TOGO_LINE_X+ 2,(int) (hour)/10%10  );
	mvprintnum3x3(CURSES_TOGO_LINE_Y+lower,CURSES_TOGO_LINE_X+ 6,(int) (hour)%10     );

	mvaddch(CURSES_TOGO_LINE_Y+0+lower,CURSES_TOGO_LINE_X+ 10,0xdc);
	mvaddch(CURSES_TOGO_LINE_Y+2+lower,CURSES_TOGO_LINE_X+ 10,0xdf);

	mvprintnum3x3(CURSES_TOGO_LINE_Y+lower,CURSES_TOGO_LINE_X+ 12,(int) (minutes)/10%10  );
	mvprintnum3x3(CURSES_TOGO_LINE_Y+lower,CURSES_TOGO_LINE_X+ 16,(int) (minutes)%10     );

	return 0;
}


 #ifdef BASICCHARSETDEMO
 void font3xout(void) {
	move(1,0);  /* (y,x) */
    printw(" %c%c%c | %c%c%c | %c%c%c | %c%c%c | %c%c%c",BLOCK_DOWN,BLOCK_WHOLE,BLOCK_SPACE,   BLOCK_UP,BLOCK_UP,BLOCK_WHOLE,      BLOCK_UP,BLOCK_UP, BLOCK_WHOLE,     BLOCK_DOWN,BLOCK_SPACE,BLOCK_WHOLE,   BLOCK_WHOLE,BLOCK_UP,BLOCK_UP);
	move(2,0);  /* (y,x) */
    printw(" %c%c%c | %c%c%c | %c%c%c | %c%c%c | %c%c%c",BLOCK_SPACE,BLOCK_WHOLE,BLOCK_SPACE,  BLOCK_WHOLE, BLOCK_UP, BLOCK_UP,    BLOCK_SPACE,BLOCK_UP,BLOCK_WHOLE,   BLOCK_WHOLE,BLOCK_DOWN,BLOCK_WHOLE,   BLOCK_UP,BLOCK_UP,BLOCK_WHOLE);
	move(3,0);  /* (y,x) */
    printw(" %c%c%c | %c%c%c | %c%c%c | %c%c%c | %c%c%c",BLOCK_DOWN,BLOCK_WHOLE,BLOCK_DOWN,    BLOCK_WHOLE,BLOCK_DOWN,BLOCK_DOWN,  BLOCK_DOWN,BLOCK_DOWN,BLOCK_WHOLE,  BLOCK_SPACE,BLOCK_SPACE,BLOCK_WHOLE,  BLOCK_DOWN,BLOCK_DOWN,BLOCK_WHOLE);

    move(5,0);  /* (y,x) */
    printw(" %c%c%c | %c%c%c | %c%c%c | %c%c%c | %c%c%c",BLOCK_WHOLE,BLOCK_UP,BLOCK_UP,       BLOCK_UP,BLOCK_UP,BLOCK_WHOLE,         BLOCK_WHOLE,BLOCK_UP, BLOCK_WHOLE,   BLOCK_WHOLE,BLOCK_UP,BLOCK_WHOLE,   BLOCK_WHOLE,BLOCK_UP,BLOCK_WHOLE);
	move(6,0);  /* (y,x) */
    printw(" %c%c%c | %c%c%c | %c%c%c | %c%c%c | %c%c%c",BLOCK_WHOLE,BLOCK_UP,BLOCK_WHOLE,    BLOCK_SPACE, BLOCK_SPACE,BLOCK_WHOLE,  BLOCK_WHOLE,BLOCK_UP,BLOCK_WHOLE,    BLOCK_UP,BLOCK_UP,BLOCK_WHOLE,      BLOCK_WHOLE,BLOCK_SPACE,BLOCK_WHOLE);
	move(7,0);  /* (y,x) */
    printw(" %c%c%c | %c%c%c | %c%c%c | %c%c%c | %c%c%c",BLOCK_WHOLE,BLOCK_DOWN,BLOCK_WHOLE,  BLOCK_SPACE,BLOCK_SPACE,BLOCK_WHOLE,   BLOCK_WHOLE,BLOCK_DOWN,BLOCK_WHOLE,  BLOCK_DOWN,BLOCK_DOWN,BLOCK_WHOLE,  BLOCK_WHOLE,BLOCK_DOWN,BLOCK_WHOLE);
}
 #endif


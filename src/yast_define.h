/*  2018-11-21 20:00  */
/* 
    yast - yet another slotcar timer
    File: yast_define.h -> all compile time definitions of the project

    Copyright (C)  2015,2016,2017,2018 Martin Berentsen


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
 
/*  Software related definitions */
#define LAPLIM 9999        /* lap counter limit std: 0 to 9999, must be smaller than INT_MAX */
#define TIMELIM 21600       /* time limit set to 6h in seconds */
/* #define TIMELIM 43200 */      /* time limit set to 12h in seconds */
/* #define TIMELIM 86400 */      /* time limit set to 24h in seconds */
#define TRACKLIM 4          /* maximum number of supported tracks */
#define MAXLAPTIME 99999    /* maximun lap time in ms */
#define MINLAPTIME  1500    /* minimum lap time in ms with respect to the pulse delay */
#define MAXDEADDELAY 10000  /* maximum time between two single track events */
#define STARTLIGHTDELAY 1000 /* delay between one and another step alt start light */
#define STARTLIGHTRANDOM  2000.0 /* random part of the start light delay end */
#define PANICDELAYTIME 800 /* delay between panic button and time measurement stop */
#define MAXPANICDELAYTIME 2500 /* delay between panic button and time measurement stop (MAX) */
#define TIMERACEENDDELAYTIME 800 /* delay time between end of time race track current and time measurement stop */
#define MAXTIMERACEENDDELAYTIME 2500 /* delay time between end of time race track current and time measurement stop (MAX) */

/* FILENAMES */
#define RESULTFILENAME 	"timing"		/* ".txt" will be added automatically */
#define RESULTFILEPATH 	"result/"   	/* path to result file, "/" must be at the end */
#define STORAGEFILENAME "yastdata.txt"	/* storage of the runtime data */    
#define CONFFILENAME 	"yast.conf"		/* configuration file name */
#define GPIOLOCKFILENAME "/var/lock/pigpio"	 /* file name to lock the Raspberry Pi GPIO connecten over WiringPi */

#define DATESPACELENGTH 120				/* length of the filename */
#define MAXNAMECOUNT	2500
#define MAXDRIVERSNAMELENGTH 16			/* maximum string length of the drivers name in display */

/* DEBUGING DEFINES */

#define MAXDEBUGLEVEL 2 /* defines the maximum debug level*/
		/*  0  is no debug, default */
		/*  1  printout all display messages also to stderr */
		/*  2  add some more verbose output to stderr */

/* GO State defines */
#define G_WAIT  0      	   /* wait or stop */
#define G_RED_0 1	   /* no red light  */
#define G_RED_1 2          /* one red light */
#define G_RED_2 3	   /* two red light */
#define G_RED_3 4	   /* three red light */
#define G_RED_4 5	   /* four red light */
#define G_RED_5 6	   /* five red light */
#define G_BLANK 7	   /* no red light again */
#define G_GREEN 8          /* switch to green */
#define G_RACE  9	   /* race is runnung */
#define G_CHECKERED_FLAG 10    /* Race end */
#define G_ALL_IN         11    /* All cars are at the complete round trip of the race */
#define G_GREEN_FLAG     20    /* free training, timing on */
#define G_YELLOW_FLAG    21    /* fail start */
#define G_RED_FLAG       22    /* end of race, timing off, power supply off */
#define G_BLUE_FLAG      23    /* unused */

#define TRACKEVENT_1			0x00000001L
#define TRACKEVENT_2			0x00000002L
#define TRACKEVENT_3			0x00000004L
#define TRACKEVENT_4			0x00000008L
#define TRACKEVENT_5			0x00000010L
#define TRACKEVENT_6			0x00000020L
#define TRACKEVENT_7			0x00000040L
#define TRACKEVENT_8			0x00000080L
#define LAP_LIMIT_TRACK_1		0x00000100L
#define LAP_LIMIT_TRACK_2		0x00000200L
#define LAP_LIMIT_TRACK_3		0x00000400L
#define LAP_LIMIT_TRACK_4		0x00000800L
#define LAP_LIMIT_TRACK_5		0x00001000L
#define LAP_LIMIT_TRACK_6		0x00002000L
#define LAP_LIMIT_TRACK_7		0x00004000L
#define LAP_LIMIT_TRACK_8		0x00008000L
#define TRACKEVENT_1_TOO_FAST	0x00010000L
#define TRACKEVENT_2_TOO_FAST	0x00020000L
#define TRACKEVENT_3_TOO_FAST	0x00040000L
#define TRACKEVENT_4_TOO_FAST	0x00080000L
#define TRACKEVENT_5_TOO_FAST	0x00100000L
#define TRACKEVENT_6_TOO_FAST	0x00200000L
#define TRACKEVENT_7_TOO_FAST	0x00400000L
#define TRACKEVENT_8_TOO_FAST	0x00800000L

/*  Hardware related definitions, wiringPi related */

#ifndef OFFLINE

#define PIN_IN_TRACK_1 0  		/* Hardware definition for Track light barrier input */
#define PIN_IN_EVENT_1 INT_EDGE_RISING
#define PIN_IN_PUD_1 	PUD_OFF
#define PIN_IN_TRACK_2 1
#define PIN_IN_EVENT_2 INT_EDGE_RISING
#define PIN_IN_PUD_2 	PUD_OFF
#define PIN_IN_TRACK_3 2
#define PIN_IN_EVENT_3 INT_EDGE_RISING
#define PIN_IN_PUD_3 	PUD_OFF		/* PUD_DOWN for unused */
#define PIN_IN_TRACK_4 3
#define PIN_IN_EVENT_4 INT_EDGE_RISING
#define PIN_IN_PUD_4 	PUD_OFF

#endif

#define MAXWIRINGPIINPUTPINS 20  	/* All GPIO Pins of the RaspberryPi by number */


/* I2C related defines */

/* Layout for the MCP23017 traffic light
 YELLOW (01) (02) (03) (04) (05)
 GREEN  (01) (02) (03) (04) (05)
 RED    (01) (03) (05) (07) (09)
 RED    (02) (04) (06) (08) (10)
*/

#define MCP23017_pinBase 	100		/* Where to find linear in WiringPi */
#define MC23017_ADDR_IC00	0x22 		/* Where to find in reality on I2C */
#define MC23017_ADDR_IC01	0x21 
#define MC23008_ADDR_IC00	0x20		/* single chip MCP23008 and MCP23017EASY */
#define MCP23017_ON		 1
#define MCP23017_OFF		 0
#define MCP23017_RED_1		13		/*  A0 GPB 5 */
#define MCP23017_RED_2		12		/*  A0 GPB 4 */
#define MCP23017_RED_3		10		/*  A0 GPB 2 */
#define MCP23017_RED_4		11		/*  A0 GPB 3 */
#define MCP23017_RED_5		 6		/*  A0 GPA 6 */
#define MCP23017_RED_6		 7		/*  A0 GPA 7 */
#define MCP23017_RED_7		 2		/*  A0 GPA 2 */
#define MCP23017_RED_8		 3		/*  A0 GPA 3 */
#define MCP23017_RED_9		29		/*  A1 GPB 5 */
#define MCP23017_RED_10		28		/*  A1 GPB 4 */
#define MCP23017_GREEN_1	14		/*  A0 GPB 6 */
#define MCP23017_GREEN_2	 8		/*  A0 GPB 0 */
#define MCP23017_GREEN_3	 5		/*  A0 GPA 5 */
#define MCP23017_GREEN_4	 1		/*  A0 GPA 1 */
#define MCP23017_GREEN_5	30		/*  A1 GPB 6 */
#define MCP23017_YELLOW_1	15    		/*  A0 GPB 7 */
#define MCP23017_YELLOW_2	 9		/*  A0 GPB 1 */
#define MCP23017_YELLOW_3	 4		/*  A0 GPA 4 */
#define MCP23017_YELLOW_4	 0		/*  A0 GPA 0 */
#define MCP23017_YELLOW_5	31		/*  A1 GPB 7 */
#define MCP23017_BACKSIDE	27		/*  A1 GPB 3 */

/* easy MCP23071 definitions */
/* Layout for the MCP23017 easy traffic light
 YELLOW (--) (--) (--) (--) (--)
 GREEN  (--) (--) (--) (--) (--)
 RED    (01) (02) (03) (04) (05)
 RED    (01) (02) (03) (04) (05)
*/

#define MCP23017E_RED_1		0
#define MCP23017E_RED_2		1
#define MCP23017E_RED_3		2
#define MCP23017E_RED_4		3
#define MCP23017E_RED_5		4
#define MCP23017E_YELLOW	5
#define MCP23017E_GREEN		6

/* Layout for ticker inside traffic light
 (54) (44) (34) (24) (14)
 (53) (43) (33) (23) (13)
 (52) (42) (32) (22) (12)
 (51) (41) (31) (21) (11)
*/

#define LL_11 MCP23017_RED_10	
#define LL_12 MCP23017_RED_9
#define LL_13 MCP23017_GREEN_5
#define LL_14 MCP23017_YELLOW_5
#define LL_21 MCP23017_RED_8
#define LL_22 MCP23017_RED_7
#define LL_23 MCP23017_GREEN_4
#define LL_24 MCP23017_YELLOW_4
#define LL_31 MCP23017_RED_6
#define LL_32 MCP23017_RED_5
#define LL_33 MCP23017_GREEN_3
#define LL_34 MCP23017_YELLOW_3
#define LL_41 MCP23017_RED_4
#define LL_42 MCP23017_RED_3
#define LL_43 MCP23017_GREEN_2
#define LL_44 MCP23017_YELLOW_2
#define LL_51 MCP23017_RED_2
#define LL_52 MCP23017_RED_1
#define LL_53 MCP23017_GREEN_1
#define LL_54 MCP23017_YELLOW_1

#define SN3218_pinBase 100
#define SN3218_ON	1 
#define SN3218_OFF	0
#define SN3218_RED_1	0
#define SN3218_RED_2	1
#define SN3218_RED_3	2
#define SN3218_RED_4	3
#define SN3218_RED_5	4
#define SN3218_GREEN	5
#define SN3218_YELLOW	6
#define SN3218_BLUE	7

/*  Display related definitions */

#define CURSES_TRACKRESULT_LINE_X 0
#define CURSES_TRACKRESULT_LINE_Y 2
#define CUR_MESSAGE_LINE_X 05		/* messages and errors */
#define CUR_STATE_X 0
#define CUR_CLOCK_X 68
#define CURSES_TRAFFICLIGHT_LINE_X 20
#define CURSES_TOGO_LINE_X 30
#define CUR_PLACE_X 66
#define CURSES_OVERALLFASTEST_LINE_X 43
#define CURSES_3x_LAP_X 43
#define CURSES_3x_LAPNUM_X 2
#define CURSES_3x_LAPLAST_X 13
#define CURSES_3x_LAPFAST_X 38
#define CURSES_FILE_X 58  		/* shortest to time is 60 */
#define CUR_POWER_LINE_X 65
#define CURSES_DRIVERSNAME_LINE_X 18

#define CUR_MESSAGE_LENGTH 50	/* maximum legth of an error or message in display, fill up to erase */

/* #define FONT3X3  */  /* now defined in makefile */
/* #define FONT3X5  */

/* from here all heights with dependencies */
#ifdef FONT3X3 
#define FONTHEIGHT 5   /* 5 if 3x3, 7 if 3x5 */
#endif

#ifdef FONT3X5 
#define FONTHEIGHT 7   /* 5 if 3x3, 7 if 3x5 */
#endif

#define DOTSHIFT (FONTHEIGHT - 3)   /* 2 if 3x3, 4 if 3x5 */
#define CUR_MESSAGE_LINE_Y (FONTHEIGHT * 4 + 1)
#define CUR_CLOCK_Y  (FONTHEIGHT * 4 + 1)
#define CURSES_FILE_Y  (FONTHEIGHT * 4 + 1)
#define CUR_STATE_Y   (FONTHEIGHT * 4 + 1)
#define CURSES_TRAFFICLIGHT_LINE_Y  (FONTHEIGHT * 4 + 2)
#define CUR_POWER_LINE_Y  (FONTHEIGHT * 4 + 1)
#define CURSES_TOGO_LINE_Y  CURSES_TRAFFICLIGHT_LINE_Y  /* + 3 not required since height control */

/*  ASCII related things */
/*  meaning of codepage CP437 used chars */

#define BLOCK_SPACE 	32   /* 0x20 */
#define BLOCK_WHOLE 	219  /* 0xdb */
#define BLOCK_DOWN 	220  /* 0xdc */
#define BLOCK_LEFT 	221  /* 0xdd */
#define BLOCK_RIGHT 	222  /* 0xde */
#define BLOCK_UP 	223  /* 0xdf */

/* curses extension */

#define SHIFT_KEY_F(n) 	(276 + (n))

/* ncurses colors definitions */

#define COLOR_PAIR_BASIC   0
#define COLOR_PAIR_TRACK_1 1
#define COLOR_PAIR_TRACK_2 2
#define COLOR_PAIR_TRACK_3 3
#define COLOR_PAIR_TRACK_4 4
#define COLOR_PAIR_TRACK_5 5
#define COLOR_PAIR_TRACK_6 6
#define COLOR_PAIR_TRACK_7 7
#define COLOR_PAIR_TRACK_8 8
#define COLOR_PAIR_RED     9
#define COLOR_PAIR_GREEN   10
#define COLOR_PAIR_YELLOW  11
#define COLOR_PAIR_BLUE    12

/* sound defines */

#define SND_NUMBER_OF_TONES 		5	/* number of different sounds */
#define SND_SAMPLE_RATE			44100 	/* 48000 also possible as default rate Raspberry Pi */
#define SND_S16_CORRECTION		16000.0	/* correction from -1.0 and +1.0 to 16 bit signed */
#define SND_PERIOD_SIZE			8192	/* sound period size */
#define SND_FRAME_SIZE			2		/* sound frame size */
#define SND_BUFFER_SIZE			SND_PERIOD_SIZE * SND_FRAME_SIZE  /* size of the buffer	in defined size S16 */

/* end of yast_define.h */

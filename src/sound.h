/*  2017-01-17 13:00  */
/*
    yast - yet another slotcar timer
	File: sound.h -> include file for sound.c

    Copyright (C)  2016,2017 Martin Berentsen


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

#ifndef _SOUND_H_
	#define _SOUND_H_

int SND_setup(void);
int SND_play(int soundnumber);
int SND_close(void);
	
#endif




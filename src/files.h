/*  2024-02-06 14:00  */
/*
    yast - yet another slotcar timer
	File: files.h -> include file for files.c

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

#ifndef _FILES_H_
	#define _FILES_H_
	
	int PathExists(char *sPath);
	int FileExists(char *sPath, char *sFile);
	int CreateLock(char *name, char *progname);
	int CheckLock(char *name);
	int DeleteLock(char *name);
	int RaspberryPiVersion(void);
	int LinuxVersion(void);
		
#endif




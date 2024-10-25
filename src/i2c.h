/*  2024-10-25 23:00  */
/*
yast - yet another slotcar timer
File: i2c.h -> i2c interface for linux

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

#ifndef _I2C_H_
        #define _I2C_H_


#endif

// void yaSetMCP23017(int ioNumber, int val);
int yaSetMCP23017(int ioNumber, int val);
int yaMCP23017Setup(int devcount);
int yaMCP23017Release(int devcount);



/*  2017-01-17 20:22  */
/* 
yast - yet another slotcar timer
File: TrafficLight.h -> Software display based Traffic Light / Start Light

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

#undef TRAFFIC_BIG
#define TRAFFIC_SMALL

void TrafficLight(int state);
void TrafficLightMidErase(int lower);
void TrafficLightHard(int state);

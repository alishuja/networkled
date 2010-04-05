/***********************************************************************
 * Copyright 2010 Ali Shuja Siddiqui.
 * 
 * This file is a part of networkled.
 * Networkled is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Networkled is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
************************************************************************/

#ifndef TRAFFIC_PARSER_H
#define TRAFFIC_PARSER_H

#include <fstream>
#include <iostream>
#include <string.h>
#include <list>
#include <string>
#include <algorithm>
using namespace std;
char * strip_whitespaces(char *);
list<string> get_interface_list();
list<string> get_traffic(string);

#endif

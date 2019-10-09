/**********
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
**********/
/*
    A C++ program to parse DJI's ".txt" log files (recorded by the "DJI Go 4" app).
    Version 2019-02-08

    Copyright (c) 2019 Live Networks, Inc.  All rights reserved.
    For the latest version of this program (and more information), visit http://djilogs.live555.com

    A database that maps integer values into a textual description.
    Implementation.
*/

#include "InterpretationTable.hh"
#include <string.h>

InterpretationTable::InterpretationTable(char const* defaultResultString)
: fDefaultResultString(strdup(defaultResultString == NULL ? "" : defaultResultString)) {
}

InterpretationTable::~InterpretationTable() {
  delete[] fDefaultResultString;
}

void InterpretationTable::add(u_int32_t intValue, char const* strValue) {
  fUMap[intValue] = strValue;
}

char const* InterpretationTable::lookup(u_int32_t intValue) {
  char const* mapLookupResult = fUMap[intValue];
  if (mapLookupResult == NULL) return fDefaultResultString;
  return mapLookupResult;
}

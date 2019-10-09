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

    Parsing CUSTOM records within DJI ".txt" files.
    Implementation.
*/

#include "RecordAndDetailsParser.hh"

void RecordAndDetailsParser::parseRecord_CUSTOM(u_int8_t const*& ptr, u_int8_t const* limit) {
  // unknown (2 bytes):
  ptr += 2;

  // CUSTOM.hSpeed: 4 bytes little-endian float:
  note4ByteFloatField("CUSTOM.hSpeed", ptr, limit);

  // CUSTOM.distance: 4 bytes little-endian float:
  note4ByteFloatField("CUSTOM.distance", ptr, limit);

  // CUSTOM.updateTime: 8 bytes little-endian, multiple of 0.001 seconds, in Unix time format:
  note8ByteTimestampField("CUSTOM.updateTime", ptr, limit, 1/*is in ms*/);
}

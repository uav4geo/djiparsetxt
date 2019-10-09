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

    Parsing CENTER_BATTERY records within DJI ".txt" files.
    Implementation.
*/

#include "RecordAndDetailsParser.hh"

void RecordAndDetailsParser::parseRecord_CENTER_BATTERY(u_int8_t const*& ptr, u_int8_t const* limit) {
  // CENTER_BATTERY.relativeCapacity: 1 byte unsigned:
  noteByteField("CENTER_BATTERY.relativeCapacity", ptr, limit);

  // CENTER_BATTERY.currentPV: 2 bytes unsigned little-endian; multiple of 0.001 volts; convert to volts:
  noteUnsigned2ByteField("CENTER_BATTERY.currentPV", ptr, limit, 1000.0);

  // CENTER_BATTERY.currentCapacity: 2 bytes unsigned little-endian:
  noteUnsigned2ByteField("CENTER_BATTERY.currentCapacity", ptr, limit);

  // CENTER_BATTERY.fullCapacity: 2 bytes unsigned little-endian:
  noteUnsigned2ByteField("CENTER_BATTERY.fullCapacity", ptr, limit);

  // CENTER_BATTERY.life: 1 byte unsigned:
  noteByteField("CENTER_BATTERY.life", ptr, limit);

  // CENTER_BATTERY.loopNum: 2 bytes unsigned little-endian:
  noteUnsigned2ByteField("CENTER_BATTERY.loopNum", ptr, limit);

  // CENTER_BATTERY.errorType: 4 bytes unsigned little-endian:
  note4ByteField("CENTER_BATTERY.errorType", ptr, limit);

  // CENTER_BATTERY.current: 2 bytes unsigned little-endian, multiple of 0.001 amps; convert to amps:
  noteUnsigned2ByteField("CENTER_BATTERY.current", ptr, limit, 1000.0);
  // CHECK VALUE! #####

  // CENTER_BATTERY.voltageCell(1 through 6): 2 bytes unsigned little-endian, multiple of 0.001 volts; convert to volts:
  noteUnsigned2ByteField("CENTER_BATTERY.voltageCell1", ptr, limit, 1000.0);
  noteUnsigned2ByteField("CENTER_BATTERY.voltageCell2", ptr, limit, 1000.0);
  noteUnsigned2ByteField("CENTER_BATTERY.voltageCell3", ptr, limit, 1000.0);
  noteUnsigned2ByteField("CENTER_BATTERY.voltageCell4", ptr, limit, 1000.0);
  noteUnsigned2ByteField("CENTER_BATTERY.voltageCell5", ptr, limit, 1000.0);
  noteUnsigned2ByteField("CENTER_BATTERY.voltageCell6", ptr, limit, 1000.0);

  // CENTER_BATTERY.serialNo: 2 bytes unsigned little-endian:
  noteUnsigned2ByteField("CENTER_BATTERY.serialNo", ptr, limit);

  // CENTER_BATTERY.productDate: 2 bytes little-endian: 7 bits (years since 1980) + 4 bits (month) + 5 bits (day of month):
  note2ByteDateField("CENTER_BATTERY.productDate", ptr, limit);

  // CENTER_BATTERY.temperature: 2 bytes unsigned little-endian, multiple of 0.01 C; convert to C:
  noteUnsigned2ByteField("CENTER_BATTERY.temperature", ptr, limit, 100.0);
  // CHECK VALUE #####

  // CENTER_BATTERY.connStatus.RAW: 1 byte unsigned:
  noteByteField("CENTER_BATTERY.connStatus.RAW", ptr, limit);
}

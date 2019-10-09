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

    Parsing HOME records within DJI ".txt" files.
    Implementation.
*/

#include "RecordAndDetailsParser.hh"

void RecordAndDetailsParser::parseRecord_HOME(u_int8_t const*& ptr, u_int8_t const* limit) {
  // HOME.longitude: 8 bytes little-endian double, in radians; convert to degrees:
  note8ByteLatitudeOrLongitudeFieldInRadians("HOME.longitude", ptr, limit);

  // HOME.latitude: 8 bytes little-endian double, in radians; convert to degrees:
  note8ByteLatitudeOrLongitudeFieldInRadians("HOME.latitude", ptr, limit);

  // HOME.height: 4 bytes little-endian float, multiple of 0.1 meters; convert to meters:
  note4ByteFloatField("HOME.height", ptr, limit, 10.0);

  // HOME.hasGoHome (1 bit) + HOME.goHomeStatus (3 bits) + HOME.isDynamicHomePointEnabled (1 bit) + HOME.aircraftHeadDirection (1 bit) + HOME.goHomeMode (1 bit) + HOME.isHomeRecord (1 bit):
  u_int8_t byte = getByte(ptr, limit);
  enterSubByteField("HOME.hasGoHome", byte, 0x80);
  enterSubByteField("HOME.goHomeStatus", byte, 0x70);
  enterSubByteField("HOME.isDynamicHomePointEnabled", byte, 0x08);
  enterSubByteField("HOME.aircraftHeadDirection", byte, 0x04);
  enterSubByteField("HOME.goHomeMode", byte, 0x02);
  enterSubByteField("HOME.isHomeRecord", byte, 0x01);

  // HOME.iocMode.RAW (3 bits) + HOME.isIOCEnabled (1 bit) + HOME.isBeginnerMode (1 bit) + HOME.isCompassCeleing (1 bit) + HOME.compassCeleStatus (2 bits):
  byte = getByte(ptr, limit);
  enterSubByteField("HOME.iocMode.RAW", byte, 0xE0);
  enterSubByteField("HOME.isIOCEnabled", byte, 0x10);
  enterSubByteField("HOME.isBeginnerMode", byte, 0x08);
  enterSubByteField("HOME.isCompassCeleing", byte, 0x04);
  enterSubByteField("HOME.compassCeleStatus", byte, 0x03);

  // HOME.goHomeHeight: 2 bytes little-endian unsigned, meters:
  noteUnsigned2ByteField("HOME.goHomeHeight", ptr, limit);

  // HOME.courseLockAngle: 2 bytes little-endian signed, multiple of 0.1 degrees, convert to degrees:
  noteSigned2ByteField("HOME.courseLockAngle", ptr, limit, 10.0);

  // HOME.dataRecorderStatus: 1 byte unsigned:
  noteByteField("HOME.dataRecorderStatus", ptr, limit);

  // HOME.dataRecorderRemainCapacity: 1 byte unsigned:
  noteByteField("HOME.dataRecorderRemainCapacity", ptr, limit);

  // HOME.dataRecorderRemainTime: 2 bytes little-endian unsigned:
  noteUnsigned2ByteField("HOME.dataRecorderRemainTime", ptr, limit);

  // HOME.dataRecorderFileIndex: 2 bytes little-endian unsigned:
  noteUnsigned2ByteField("HOME.dataRecorderFileIndex", ptr, limit);
}

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

    Parsing RC records within DJI ".txt" files.
    Implementation.
*/

#include "RecordAndDetailsParser.hh"

void RecordAndDetailsParser::parseRecord_RC(u_int8_t const*& ptr, u_int8_t const* limit) {
  // RC.aileron: 2 bytes little-endian, signed: subtract 1024, and divide by 0.066(?):
  noteSigned2ByteField("RC.aileron", ptr, limit, 0.066, 1024);

  // RC.elevator: 2 bytes little-endian, signed: subtract 1024, and divide by 0.066(?):
  noteSigned2ByteField("RC.elevator", ptr, limit, 0.066, 1024);

  // RC.throttle: 2 bytes little-endian, signed: subtract 1024, and divide by 0.066(?):
  noteSigned2ByteField("RC.throttle", ptr, limit, 0.066, 1024);

  // RC.rudder: 2 bytes little-endian, signed: subtract 1024, and divide by 0.066(?):
  noteSigned2ByteField("RC.rudder", ptr, limit, 0.066, 1024);

  // RC.gimbal: 2 bytes little-endian, signed: subtract 1024, and divide by 0.066(?):
  noteSigned2ByteField("RC.gimbal", ptr, limit, 0.066, 1024);

  // unknown(2 bits) + RC.wheelOffset(5 bits) + unknown(1 bit):
  u_int8_t byte = getByte(ptr, limit);
  enterSubByteField("RC.wheelOffset", byte, 0x3E);

  // unknown(2 bits) + RC.mode(2 bits) + RC.goHome(1 bit) + unknown(3 bits):
  byte = getByte(ptr, limit);
  enterSubByteField("RC.mode", byte, 0x30);
  enterSubByteField("RC.goHome", byte, 0x08);

  // 8 bits (Boolean flags); from high to low:
  //  RC.record
  //  RC.shutter
  //  RC.playback
  //  RC.custom1
  //  RC.custom2
  //  unknown(3 bits)
  byte = getByte(ptr, limit);
  enterSubByteField("RC.record", byte, 0x80);
  enterSubByteField("RC.shutter", byte, 0x40);
  enterSubByteField("RC.playback", byte, 0x20);
  enterSubByteField("RC.custom1", byte, 0x10);
  enterSubByteField("RC.custom2", byte, 0x08);
}

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

    Parsing GIMBAL records within DJI ".txt" files.
    Implementation.
*/

#include "RecordAndDetailsParser.hh"

void RecordAndDetailsParser::parseRecord_GIMBAL(u_int8_t const*& ptr, u_int8_t const* limit) {
  // GIMBAL.pitch: 2 bytes signed little-endian, multiple of 0.1 degrees; convert to degrees:
  noteSigned2ByteField("GIMBAL.pitch", ptr, limit, 10.0);

  // GIMBAL.roll: 2 bytes signed little-endian, multiple of 0.1 degrees; convert to degrees:
  noteSigned2ByteField("GIMBAL.roll", ptr, limit, 10.0);

  // GIMBAL.yaw: 2 bytes signed little-endian, multiple of 0.1 degrees; convert to degrees:
  noteSigned2ByteField("GIMBAL.yaw", ptr, limit, 10.0);

  // GIMBAL.mode.RAW(2 bits) + unknown(6 bits):
  u_int8_t byte = getByte(ptr, limit);
  enterSubByteField("GIMBAL.mode.RAW", byte, 0xC0);

  // GIMBAL.rollAdjust: 1 byte signed, multiple of 0.1:
  noteByteField("GIMBAL.rollAdjust", ptr, limit, 10.0, 1/*isSigned*/);

  // GIMBAL.yawAngle: 2 bytes signed little-endian, multiple of 0.1 degrees; convert to degrees:
  noteSigned2ByteField("GIMBAL.yawAngle", ptr, limit, 10.0);

  // 8 bits (Boolean flags); from high to low:
  //  unknown
  //  GIMBAL.isStuck
  //  unknown
  //  GIMBAL.autoCalibrationResult
  //  GIMBAL.isAutoCalibration
  //  GIMBAL.isYawInLimit
  //  GIMBAL.isRollInLimit
  //  GIMBAL.isPitchInLimit
  byte = getByte(ptr, limit);
  enterSubByteField("GIMBAL.isStuck", byte, 0x40);
  enterSubByteField("GIMBAL.autoCalibrationResult", byte, 0x10);
  enterSubByteField("GIMBAL.isAutoCalibration", byte, 0x08);
  enterSubByteField("GIMBAL.isYawInLimit", byte, 0x04);
  enterSubByteField("GIMBAL.isRollInLimit", byte, 0x02);
  enterSubByteField("GIMBAL.isPitchInLimit", byte, 0x01);

  // GIMBAL.isSingleClick (1 bit) + GIMBAL.isTripleClick (1 bit) + GIMBAL.isDoubleClick (1 bit) + unknown (1 bit) + GIMBAL.version (4 bits):
  byte = getByte(ptr, limit);
  enterSubByteField("GIMBAL.isSingleClick", byte, 0x80);
  enterSubByteField("GIMBAL.isTripleClick", byte, 0x40);
  enterSubByteField("GIMBAL.isDoubleClick", byte, 0x20);
  enterSubByteField("GIMBAL.version", byte, 0x0F);
}

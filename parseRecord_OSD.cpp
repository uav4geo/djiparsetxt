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

    Parsing OSD records within DJI ".txt" files.
    Implementation.
*/

#include "RecordAndDetailsParser.hh"

void RecordAndDetailsParser::parseRecord_OSD(u_int8_t const*& ptr, u_int8_t const* limit) {
  // OSD.longitude: 8 bytes little-endian double, in radians; convert to degrees:
  note8ByteLatitudeOrLongitudeFieldInRadians("OSD.longitude", ptr, limit);

  // OSD.latitude: 8 bytes little-endian double, in radians; convert to degrees:
  note8ByteLatitudeOrLongitudeFieldInRadians("OSD.latitude", ptr, limit);

  // OSD.height: 2 bytes signed(?) little-endian, multiple of 0.1 meters; convert to meters:
  noteSigned2ByteField("OSD.height", ptr, limit, 10.0);
  
  // OSD.xSpeed: 2 bytes signed little-endian, multiple of 0.1 m/s; convert to m/s:
  noteSigned2ByteField("OSD.xSpeed", ptr, limit, 10.0);

  // OSD.ySpeed: 2 bytes signed little-endian, multiple of 0.1 m/s; convert to m/s:
  noteSigned2ByteField("OSD.ySpeed", ptr, limit, 10.0);

  // OSD.zSpeed: 2 bytes signed little-endian, multiple of 0.1 m/s; convert to m/s:
  noteSigned2ByteField("OSD.zSpeed", ptr, limit, 10.0);

  // OSD.pitch: 2 bytes signed little-endian, multiple of 0.1 degrees; convert to degrees:
  noteSigned2ByteField("OSD.pitch", ptr, limit, 10.0);

  // OSD.roll: 2 bytes signed little-endian, multiple of 0.1 degrees; convert to degrees:
  noteSigned2ByteField("OSD.roll", ptr, limit, 10.0);

  // OSD.yaw: 2 bytes signed little-endian, multiple of 0.1 degrees; convert to degrees:
  noteSigned2ByteField("OSD.yaw", ptr, limit, 10.0);

  // OSD.rcState(1 bit) + OSD.flycState.RAW (7 bits):
  u_int8_t byte = getByte(ptr, limit);
  enterSubByteField("OSD.rcState", byte, 0x80);
  enterSubByteField("OSD.flycState.RAW", byte, 0x7F);

  // OSD.flycCommand.RAW: 1 byte unsigned:
  noteByteField("OSD.flycCommand.RAW", ptr, limit);

  // OSD.goHomeStatus.RAW(3 bits) + OSD.isSwaveWork(1 bit) + OSD.isMotorUp(1 bit) + OSD.groundOrSky.RAW(2 bits) + OSD.canIOCWork(1 bit)
  byte = getByte(ptr, limit);
  enterSubByteField("OSD.goHomeStatus.RAW", byte, 0xE0);
  enterSubByteField("OSD.isSwaveWork", byte, 0x10);
  enterSubByteField("OSD.isMotorUp", byte, 0x08);
  enterSubByteField("OSD.groundOrSky.RAW", byte, 0x06); // only the high bit is used?
  enterSubByteField("OSD.canIOCWork", byte, 0x01);

  // unknown(1 bit) + OSD.modeChannel(2 bits) + OSD.isImuPreheated(1 bit) + unknown(1 bit) + OSD.voltageWarning(2 bits) + OSD.isVisionUsed(1 bit)
  byte = getByte(ptr, limit);
  enterSubByteField("OSD.modeChannel", byte, 0x60);
  enterSubByteField("OSD.isImuPreheated", byte, 0x10);
  enterSubByteField("OSD.voltageWarning", byte, 0x06);
  enterSubByteField("OSD.isVisionUsed", byte, 0x01);

  // OSD.batteryType.RAW(2 bits) + OSD.gpsLevel(4 bits) + OSD.waveError(1 bit) + OSD.compassError(1 bit)
  byte = getByte(ptr, limit);
  enterSubByteField("OSD.batteryType.RAW", byte, 0xC0);
  enterSubByteField("OSD.gpsLevel", byte, 0x3C);
  enterSubByteField("OSD.waveError", byte, 0x02);
  enterSubByteField("OSD.compassError", byte, 0x01);

  // 8 bits (Boolean flags); from high to low:
  //  OSD.isAcceletorOverRange (sic)
  //  OSD.isVibrating
  //  OSD.isBarometerDeadInAir
  //  OSD.isMotorBlocked
  //  OSD.isNotEnoughForce
  //  OSD.isPropellerCatapult
  //  OSD.isGoHomeHeightModified
  //  OSD.isOutOfLimit
  byte = getByte(ptr, limit);
  enterSubByteField("OSD.isAcceletorOverRange", byte, 0x80);
  enterSubByteField("OSD.isVibrating", byte, 0x40);
  enterSubByteField("OSD.isBarometerDeadInAir", byte, 0x20);
  enterSubByteField("OSD.isMotorBlocked", byte, 0x10);
  enterSubByteField("OSD.isNotEnoughForce", byte, 0x08);
  enterSubByteField("OSD.isPropellerCatapult", byte, 0x04);
  enterSubByteField("OSD.isGoHomeHeightModified", byte, 0x02);
  enterSubByteField("OSD.isOutOfLimit", byte, 0x01);

  // OSD.gpsNum: 1 byte unsigned:
  noteByteField("OSD.gpsNum", ptr, limit);

  // OSD.flightAction.RAW: 1 byte unsigned:
  noteByteField("OSD.flightAction.RAW", ptr, limit);

  // OSD.motorStartFailedCause.RAW: 1 byte unsigned:
  noteByteField("OSD.motorStartFailedCause.RAW", ptr, limit);

  // unknown (3 bits) + OSD.waypointLimitMode (1 bit) + OSD.nonGPSCause.RAW (4 bits):
  byte = getByte(ptr, limit);
  enterSubByteField("OSD.waypointLimitMode", byte, 0x10);
  enterSubByteField("OSD.nonGPSCause.RAW", byte, 0x0F);

  // OSD.battery: 1 byte unsigned:
  noteByteField("OSD.battery", ptr, limit);

  // OSD.sWaveHeight: 1 byte unsigned, multiple of 0.1 meters; convert to meters:
  noteByteField("OSD.sWaveHeight", ptr, limit, 10.0);

  // OSD.flyTime: 2 bytes unsigned little-endian, multiple of 0.1 seconds; convert to seconds:
  noteUnsigned2ByteField("OSD.flyTime", ptr, limit, 10.0);

  // OSD.motorRevolution: 1 byte unsigned:
  noteByteField("OSD.motorRevolution", ptr, limit);

  // unknown (2 bytes):
  ptr += 2;

  // OSD.flycVersion: 1 byte unsigned:
  noteByteField("OSD.flycVersion", ptr, limit);

  // OSD.droneType.RAW: 1 byte unsigned:
  noteByteField("OSD.droneType.RAW", ptr, limit);

  // OSD.imuInitFailReason.RAW: 1 byte unsigned:
  noteByteField("OSD.imuInitFailReason.RAW", ptr, limit);

  // The following fields are not present in some versions of .txt files:
  if (limit - ptr > 3) {
    // OSD.motorFailReason.RAW: 1 byte unsigned:
    noteByteField("OSD.motorFailReason.RAW", ptr, limit);

    // unknown (1 byte):
    ++ptr;

    // OSD.ctrlDevice.RAW: 1 byte unsigned:
    noteByteField("OSD.ctrlDevice.RAW", ptr, limit);

    // unknown (1 byte):
    ++ptr;
  }
}

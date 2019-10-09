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

    Parsing SMART_BATTERY records within DJI ".txt" files.
    Implementation.
*/

#include "RecordAndDetailsParser.hh"

void RecordAndDetailsParser::parseRecord_SMART_BATTERY(u_int8_t const*& ptr, u_int8_t const* limit) {
  // SMART_BATTERY.usefulTime: 2 bytes little-endian unsigned:
  noteUnsigned2ByteField("SMART_BATTERY.usefulTime", ptr, limit);

  // SMART_BATTERY.goHomeTime: 2 bytes little-endian unsigned:
  noteUnsigned2ByteField("SMART_BATTERY.goHomeTime", ptr, limit);

  // SMART_BATTERY.landTime: 2 bytes little-endian unsigned:
  noteUnsigned2ByteField("SMART_BATTERY.landTime", ptr, limit);

  // SMART_BATTERY.goHomeBattery: 2 bytes little-endian unsigned:
  noteUnsigned2ByteField("SMART_BATTERY.goHomeBattery", ptr, limit);

  // SMART_BATTERY.landBattery: 2 bytes little-endian unsigned:
  noteUnsigned2ByteField("SMART_BATTERY.landBattery", ptr, limit);

  // SMART_BATTERY.safeFlyRadius: 4 bytes little-endian unsigned:
  note4ByteField("SMART_BATTERY.safeFlyRadius", ptr, limit);

  // SMART_BATTERY.volumeConsume: 4 bytes little-endian float:
  note4ByteFloatField("SMART_BATTERY.volumeConsume", ptr, limit);

  // SMART_BATTERY.status.RAW: 4 bytes little-endian unsigned:
  note4ByteField("SMART_BATTERY.status.RAW", ptr, limit);

  // SMART_BATTERY.goHomeStatus.RAW: 1 byte unsigned:
  noteByteField("SMART_BATTERY.goHomeStatus.RAW", ptr, limit);

  // SMART_BATTERY.goHomeCountdown: 1 byte unsigned:
  noteByteField("SMART_BATTERY.goHomeCountdown", ptr, limit);

  // SMART_BATTERY.voltage: 2 bytes little-endian unsigned; multiple of 0.001 volts; convert to volts:
  noteUnsigned2ByteField("SMART_BATTERY.voltage", ptr, limit, 1000.0);

  // SMART_BATTERY.battery: 1 byte unsigned:
  noteByteField("SMART_BATTERY.battery", ptr, limit);

  // SMART_BATTERY.lowWarningGoHome (1 bit) + SMART_BATTERY.lowWarning (7 bits):
  u_int8_t byte = getByte(ptr, limit);
  enterSubByteField("SMART_BATTERY.lowWarningGoHome", byte, 0x80);
  enterSubByteField("SMART_BATTERY.lowWarning", byte, 0x7F);

  // SMART_BATTERY.seriousLowWarningLanding (1 bit) + SMART_BATTERY.seriousLowWarning (7 bits):
  byte = getByte(ptr, limit);
  enterSubByteField("SMART_BATTERY.seriousLowWarningLanding", byte, 0x80);
  enterSubByteField("SMART_BATTERY.seriousLowWarning", byte, 0x7F);

  // SMART_BATTERY.voltagePercent: 1 byte unsigned:
  noteByteField("SMART_BATTERY.voltagePercent", ptr, limit);
}

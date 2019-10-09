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

    Parsing RECOVER records within DJI ".txt" files.
    Implementation.
*/

#include "RecordAndDetailsParser.hh"

void RecordAndDetailsParser::parseRecord_RECOVER(u_int8_t const*& ptr, u_int8_t const* limit) {
  // RECOVER.droneType.RAW: 1 byte unsigned:
  noteByteField("RECOVER.droneType.RAW", ptr, limit);

  // RECOVER.appType.RAW: 1 byte unsigned:
  noteByteField("RECOVER.appType.RAW", ptr, limit);

  // RECOVER.appVersion: 3 bytes;
  note3ByteVersionField("RECOVER.appVersion", ptr, limit);

  // RECOVER.aircraftSn: string (length 10):
  noteStringField("RECOVER.aircraftSn", ptr, 10, limit);

  // RECOVER.aircraftName: string (length 24):
  noteStringField("RECOVER.aircraftName", ptr, 24, limit);

  // unknown (8 bytes)
  ptr += 8;

  // RECOVER.activeTimestamp: 8 bytes little-endian, in Unix time format:
  extern u_int32_t fileVersionNumber;
  if ((fileVersionNumber&0x0000FF00) < 0x00000800) {
    note8ByteTimestampField("RECOVER.activeTimestamp", ptr, limit);
  } else {
    // This timestamp is formatted differently (how?) in newer versions of the .txt file. #####
    if ((fileVersionNumber&0x0000FF00) > 0x00000800) {
      ptr += 8; // skip over 8-byte timestamp
    } else {
      ptr += 6+8; // skip over 6 bytes (unknown) + 8-byte timestamp
    }
  }

  // RECOVER.cameraSn: string (length 10):
  noteStringField("RECOVER.cameraSn", ptr, 10, limit);

  // RECOVER.rcSn: string (length 10):
  noteStringField("RECOVER.rcSn", ptr, 10, limit);

  // RECOVER.batterySn: string (length 10):
  noteStringField("RECOVER.batterySn", ptr, 10, limit);
}

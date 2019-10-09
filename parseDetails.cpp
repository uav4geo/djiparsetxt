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

    Parsing the 'details' section in DJI ".txt" files.
    Implementation.
*/

#include "RecordAndDetailsParser.hh"

void RecordAndDetailsParser::parseDetailsArea(u_int8_t const*& ptr, u_int8_t const* limit) {
  // DETAILS.cityPart: string (length 20):
  noteStringField("DETAILS.cityPart", ptr, 20, limit);

  // DETAILS.street: string (length 20):
  noteStringField("DETAILS.street", ptr, 20, limit);

  // DETAILS.city: string (length 20):
  noteStringField("DETAILS.city", ptr, 20, limit);

  // DETAILS.area: string (length 20):
  noteStringField("DETAILS.area", ptr, 20, limit);

  // DETAILS.isFavorite: 1 byte unsigned:
  noteByteField("DETAILS.isFavorite", ptr, limit);

  // DETAILS.isNew: 1 byte unsigned:
  noteByteField("DETAILS.isNew", ptr, limit);

  // DETAILS.needsUpload: 1 byte unsigned:
  noteByteField("DETAILS.needUpload", ptr, limit);

  // DETAILS.recordLineCount: 4 bytes little-endian unsigned::
  note4ByteField("DETAILS.recordLineCount", ptr, limit);

  // unknown (4 bytes):
  ptr += 4;

  // DETAILS.timestamp: 8 bytes little-endian, multiple of 0.001 seconds, in Unix time format:
  note8ByteTimestampField("DETAILS.timestamp", ptr, limit, 1/*is in ms*/);

  // DETAILS.longitude: 8 bytes little-endian double, in degrees:
  note8ByteDoubleField("DETAILS.longitude", ptr, limit);

  // DETAILS.latitude: 8 bytes little-endian double, in degrees:
  note8ByteDoubleField("DETAILS.latitude", ptr, limit);

  // DETAILS.totalDistance: 4 bytes little-endian float:
  note4ByteFloatField("DETAILS.totalDistance", ptr, limit);

  // DETAILS.totalTime: 4 bytes little-endian unsigned, multiple of 0.001m; convert to meters:
  note4ByteField("DETAILS.totalTime", ptr, limit, 1000.0);

  // DETAILS.maxHeight: 4 bytes little-endian float:
  note4ByteFloatField("DETAILS.maxHeight", ptr, limit);

  // DETAILS.maxHorizontalSpeed: 4 bytes little-endian float:
  note4ByteFloatField("DETAILS.maxHorizontalSpeed", ptr, limit);

  // DETAILS.maxVerticalSpeed: 4 bytes little-endian float:                                         
  note4ByteFloatField("DETAILS.maxVerticalSpeed", ptr, limit);

  // DETAILS.photoNum: 4 bytes little-endian unsigned:
  note4ByteField("DETAILS.photoNum", ptr, limit);

  // DETAILS.videoTime: 4 bytes little-endian unsigned:
  note4ByteField("DETAILS.videoTime", ptr, limit);

  // The format from here on depends upon the file version:
  extern u_int32_t fileVersionNumber;
  if ((fileVersionNumber&0x0000FF00) < 0x00000600) {
    // unknown (124 bytes)
    ptr += 124;

    // DETAILS.aircraftSnBytes: string (length 10):
    noteStringField("DETAILS.aircraftSn", ptr, 10, limit);

    // unknown (1 byte)
    ++ptr;

    // DETAILS.aircraftName: string (length 25):
    noteStringField("DETAILS.aircraftName", ptr, 25, limit);

    // unknown (7 bytes)
    ptr += 7;

    // DETAILS.activeTimestamp: 8 bytes little-endian, in Unix time format:
    note8ByteTimestampField("DETAILS.activeTimestamp", ptr, limit);

    // DETAILS.cameraSn: string (length 10):
    noteStringField("DETAILS.cameraSn", ptr, 10, limit);

    // DETAILS.rcSn: string (length 10):
    noteStringField("DETAILS.rcSn", ptr, 10, limit);

    // DETAILS.batterySn: string (length 10):
    noteStringField("DETAILS.batterySn", ptr, 10, limit);
  } else {
    // Where is "DETAILS.activeTimestamp", and how is it formatted? #####
    // unknown (137 bytes)
    ptr += 137;

    // DETAILS.aircraftName: string (length 32):
    noteStringField("DETAILS.aircraftName", ptr, 32, limit);

    // DETAILS.aircraftSnBytes: string (length 16):
    noteStringField("DETAILS.aircraftSn", ptr, 16, limit);

    // DETAILS.cameraSn: string (length 16):
    noteStringField("DETAILS.cameraSn", ptr, 16, limit);

    // DETAILS.rcSn: string (length 16):
    noteStringField("DETAILS.rcSn", ptr, 16, limit);

    // DETAILS.batterySn: string (length 16):
    noteStringField("DETAILS.batterySn", ptr, 16, limit);
  }

  // DETAILS.appType.RAW: 1 byte unsigned:
  noteByteField("DETAILS.appType.RAW", ptr, limit);

  // DETAILS.appVersion: 3 bytes:
  note3ByteVersionField("DETAILS.appVersion", ptr, limit);
}

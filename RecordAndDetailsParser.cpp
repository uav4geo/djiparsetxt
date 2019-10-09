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

    Parsing records (and the 'details' section) within DJI ".txt" files.
    Implementation.
*/

#include "RecordAndDetailsParser.hh"
#include <stdio.h>

DJITxtParser* DJITxtParser::createNew() {
  return new RecordAndDetailsParser;
}

////////// RecordTypeStat implementation //////////

RecordTypeStat::RecordTypeStat()
  : count(0), minLength(~0), maxLength(0) {
}

////////// RecordAndDetailsParser implementation //////////

RecordAndDetailsParser::RecordAndDetailsParser()
  : fNumRecords(0), fMaxNumRecordsForOneType(0), fFieldDatabase(new FieldDatabase) {
#ifdef DEBUG_RECORD_PARSING
  // Initialize "fRecordTypeName":
  for (unsigned i = 0; i < 256; ++i) {
    fRecordTypeName[i] = NULL;
  }
  fRecordTypeName[0x01] = "OSD";
  fRecordTypeName[0x02] = "HOME";
  fRecordTypeName[0x03] = "GIMBAL";
  fRecordTypeName[0x04] = "RC";
  fRecordTypeName[0x05] = "CUSTOM";
  fRecordTypeName[0x06] = "DEFORM";
  fRecordTypeName[0x07] = "CENTER_BATTERY";
  fRecordTypeName[0x08] = "SMART_BATTERY";
  fRecordTypeName[0x09] = "APP_TIP";
  fRecordTypeName[0x0A] = "APP_WARN";
  fRecordTypeName[0x0B] = "RC_GPS";
  fRecordTypeName[0x0C] = "RC_DEBUG";
  fRecordTypeName[0x0D] = "RECOVER";
  fRecordTypeName[0x0E] = "APP_GPS";
  fRecordTypeName[0x0F] = "FIRMWARE";
  fRecordTypeName[0x10] = "OFDM_DEBUG";
  fRecordTypeName[0x11] = "VISION_GROUP";
  fRecordTypeName[0x12] = "VISION_WARN";
  fRecordTypeName[0x13] = "MC_PARAM";
  fRecordTypeName[0x14] = "APP_OPERATION";
  // What is record type 0x16? #####
  fRecordTypeName[0x18] = "APP_SER_WARN";
  // What is record type 0x19? #####
  // What is record type 0x1a? #####
  // What is record type 0x1e? #####
  // What is record type 0x28? #####
  fRecordTypeName[0x39] = "JPEG";
  fRecordTypeName[0xFE] = "OTHER";
#endif
}

RecordAndDetailsParser::~RecordAndDetailsParser() {
  delete fFieldDatabase;
}

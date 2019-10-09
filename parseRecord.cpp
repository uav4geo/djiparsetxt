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

    Parsing records within DJI ".txt" files.
    Implementation.
*/

#include "RecordAndDetailsParser.hh"

#include <stdio.h>
#include <string.h>

#define RECORD_TYPE_OSD 0x01
#define RECORD_TYPE_HOME 0x02
#define RECORD_TYPE_GIMBAL 0x03
#define RECORD_TYPE_RC 0x04
#define RECORD_TYPE_CUSTOM 0x05
#define RECORD_TYPE_DEFORM 0x06
#define RECORD_TYPE_CENTER_BATTERY 0x07
#define RECORD_TYPE_SMART_BATTERY 0x08
#define RECORD_TYPE_APP_TIP 0x09
#define RECORD_TYPE_APP_WARN 0x0A
#define RECORD_TYPE_RC_GPS 0x0B
#define RECORD_TYPE_RC_DEBUG 0x0C
#define RECORD_TYPE_RECOVER 0x0D
#define RECORD_TYPE_APP_GPS 0x0E
#define RECORD_TYPE_FIRMWARE 0x0F
#define RECORD_TYPE_OFDM_DEBUG 0x10
#define RECORD_TYPE_VISION_GROUP 0x11
#define RECORD_TYPE_VISION_WARN 0x12
#define RECORD_TYPE_MC_PARAM 0x13
#define RECORD_TYPE_APP_OPERATION 0x14
// What is record type 0x16? #####
#define RECORD_TYPE_APP_SER_WARN 0x18
// What is record type 0x19? #####
// What is record type 0x1a? #####
// What is record type 0x1e? #####
// What is record type 0x28? #####
#define RECORD_TYPE_JPEG 0x39
#define RECORD_TYPE_OTHER 0xFE

#define JPEG_SOI_BYTE 0xD8

int RecordAndDetailsParser::parseRecord(u_int8_t const*& ptr, u_int8_t const* limit, int isScrambled) {
  // Attempt to parse a record; Returns 1 iff it succeeds.
  try {
    // The first two bytes are the 'record type' and the 'record length':
    u_int8_t recordType = getByte(ptr, limit);
    u_int8_t recordLength = getByte(ptr, limit);

    // Record statistics about the record type and length:
    ++fNumRecords;
    RecordTypeStat& stat = fRecordTypeStats[recordType]; // alias
    ++stat.count;
    if (stat.count > fMaxNumRecordsForOneType) fMaxNumRecordsForOneType = stat.count;
    if (recordLength < stat.minLength) stat.minLength = recordLength;
    if (recordLength > stat.maxLength) stat.maxLength = recordLength;
#ifdef DEBUG_RECORD_PARSING
    char const* recordTypeName = fRecordTypeName[recordType];
    if (recordTypeName == NULL) recordTypeName = "???";
    fprintf(stderr, "[%d]\trecordType %d[%s], recordLength %d\n", fRecordTypeStats[RECORD_TYPE_OSD].count, recordType, recordTypeName, recordLength);
#endif

    if (recordType == RECORD_TYPE_JPEG) {
      // This record contains one or more JPEG images, and needs to be handled especially.
      // (In particular, the 'recordLength' seems to be irrelevant in this case)
      return parseRecord_JPEG(ptr, limit);
    } else if (recordType == 0xFF && recordLength == JPEG_SOI_BYTE) {
      // Some old log formats start JPEG images this way.  Back up 4 bytes; handle them the same way:
      ptr -=4;
      return parseRecord_JPEG(ptr, limit);
    }

    // Check the record length, and whether there's a 0xFF byte at the end:
    if (ptr + recordLength + 1 > limit) throw END_OF_DATA;
    if (ptr[recordLength] != 0xFF) {
      fprintf(stderr, "'End of record' byte not seen\n");
      return 0;
    }
    u_int8_t const* recordStart = ptr;
    u_int8_t const* recordLimit = ptr + recordLength; // position of the 0xFF 'End of record' byte
    ptr += recordLength + 1; // advance to the next record, if any

    u_int8_t unscrambledRecord[recordLength-1]; // used only if "isScrambled"
    if (isScrambled) {
      // We need to unscramble the record data before we can parse it.

      // The next byte (along with the 'record type') is an index into the 'scramble table':
      u_int8_t keyIndexLowByte = getByte(recordStart, limit);
      u_int16_t scrambleTableIndex = ((recordType-1)<<8)|keyIndexLowByte;
      --recordLength;

      if (scrambleTableIndex >= 0x1000) {
	// Our current 'scramble table' is not large enough to handle this record type. #####
	fprintf(stderr, "WARNING: for record type 0x%02x", recordType);
#ifdef DEBUG_RECORD_PARSING
	fprintf(stderr, "[%s]", recordTypeName);
#endif
	fprintf(stderr, ", scrambleTableIndex 0x%x is too large (>0x1000) for our current 'scramble table'; we can't unscramble this data!\n", scrambleTableIndex);
      } else {
	// Normal case: We know how to unscramble this record's data:
	extern u_int8_t const scrambleTable[0x1000][8];
	u_int8_t const* scrambleBytes = scrambleTable[scrambleTableIndex]; // an array of 8 bytes

	// Unscramble each byte in the record by XORing it with the 'scrambleBytes':
	for (unsigned i = 0; i < recordLength; ++i) {
	  unscrambledRecord[i] = recordStart[i] ^ scrambleBytes[i%8];
	}
	recordStart = unscrambledRecord;
	recordLimit = unscrambledRecord + recordLength;
      }
    }

    switch (recordType) {
      case RECORD_TYPE_OSD: {
	// Because an 'OSD' record effectively starts a new row of data, output a row of data
	// before we parse it (except for the very first 'OSD' record, where we output
	// the column labels instead):
	outputOneRow(fRecordTypeStats[RECORD_TYPE_OSD].count == 1);
	parseRecord_OSD(recordStart, recordLimit);
	break;
      }
      case RECORD_TYPE_HOME: {
	parseRecord_HOME(recordStart, recordLimit);
	break;
      }
      case RECORD_TYPE_GIMBAL: {
	parseRecord_GIMBAL(recordStart, recordLimit);
	break;
      }
      case RECORD_TYPE_RC: {
	parseRecord_RC(recordStart, recordLimit);
	break;
      }
      case RECORD_TYPE_CUSTOM: {
	parseRecord_CUSTOM(recordStart, recordLimit);
	break;
      }
      case RECORD_TYPE_DEFORM: {
	parseRecord_DEFORM(recordStart, recordLimit);
	break;
      }
      case RECORD_TYPE_CENTER_BATTERY: {
	parseRecord_CENTER_BATTERY(recordStart, recordLimit);
	break;
      }
      case RECORD_TYPE_SMART_BATTERY: {
	parseRecord_SMART_BATTERY(recordStart, recordLimit);
	break;
      }
      case RECORD_TYPE_APP_TIP: {
  	parseRecord_APP_TIP(recordStart, recordLimit);
	break;
      }
      case RECORD_TYPE_APP_WARN: {
	parseRecord_APP_WARN(recordStart, recordLimit);
	break;
      }
      case RECORD_TYPE_RC_GPS: {
	parseRecordUnknownFormat("RC_GPS", recordStart, recordLimit);
	break;
      }
      case RECORD_TYPE_RC_DEBUG: {
	parseRecordUnknownFormat("RC_DEBUG", recordStart, recordLimit);
	break;
      }
      case RECORD_TYPE_RECOVER: {
	parseRecord_RECOVER(recordStart, recordLimit);
	break;
      }
      case RECORD_TYPE_APP_GPS: {
	parseRecord_APP_GPS(recordStart, recordLimit);
	break;
      }
      case RECORD_TYPE_FIRMWARE: {
	parseRecord_FIRMWARE(recordStart, recordLimit);
	break;
      }
      case RECORD_TYPE_OFDM_DEBUG: {
	parseRecordUnknownFormat("OFDM_DEBUG", recordStart, recordLimit);
	break;
      }
      case RECORD_TYPE_VISION_GROUP: {
	parseRecordUnknownFormat("VISION_GROUP", recordStart, recordLimit);
	break;
      }
      case RECORD_TYPE_VISION_WARN: {
	parseRecordUnknownFormat("VISION_WARN", recordStart, recordLimit);
	break;
      }
      case RECORD_TYPE_MC_PARAM: {
	parseRecordUnknownFormat("MC_PARAM", recordStart, recordLimit);
	break;
      }
      case RECORD_TYPE_APP_OPERATION: {
	parseRecordUnknownFormat("APP_OPERATION", recordStart, recordLimit);
	break;
      }
      case RECORD_TYPE_APP_SER_WARN: {
	parseRecordUnknownFormat("APP_SER_WARN", recordStart, recordLimit);
	break;
      }
      default: {
#ifdef DEBUG_RECORD_PARSING
	char const* recordTypeName = fRecordTypeName[recordType];
	if (recordTypeName == NULL) {
	  fprintf(stderr, "Unknown record type 0x%02x\n", recordType);
	} else {
	  fprintf(stderr, "Unhandled record type 0x%02x [%s]\n", recordType, recordTypeName);
	}
#else
	fprintf(stderr, "Unhandled record type 0x%02x\n", recordType);
#endif
      }
    }
  } catch (int /*e*/) {
    fprintf(stderr, "Unexpected error in parsing\n");
    return 0;
  }

  return 1;
}

void RecordAndDetailsParser::summarizeRecordParsing() {
#ifdef DEBUG_RECORD_PARSING
  fprintf(stderr, "%d records parsed; max num records for one type: %d\n", fNumRecords, fMaxNumRecordsForOneType);
  unsigned maxRecordTypeFieldLen = 0;
  for (unsigned i = 0; i < 256; ++i) {
    char const* recordTypeName = fRecordTypeName[i];
    unsigned recordTypeNameLen = recordTypeName == NULL ? 3 : strlen(recordTypeName);
    unsigned iLog10 = i<10 ? 0 : i<100 ? 1 : 2;
    unsigned recordTypeFieldLen = iLog10 + 2 + recordTypeNameLen + 2;
    if (recordTypeFieldLen > maxRecordTypeFieldLen) maxRecordTypeFieldLen = recordTypeFieldLen;
  }
  unsigned maxNumTabs = maxRecordTypeFieldLen/8 + 1;

  for (unsigned i = 0; i < 255; ++i) {
    if (fRecordTypeStats[i].count > 0) {
      char const* recordTypeName = fRecordTypeName[i];
      if (recordTypeName == NULL) recordTypeName = "???";
      unsigned iLog10 = i<10 ? 0 : i<100 ? 1 : 2;
      unsigned recordTypeFieldLen = iLog10 + 2 + strlen(recordTypeName) + 2;
      unsigned numTabs = maxNumTabs - recordTypeFieldLen/8; // >0

      fprintf(stderr, "%d[%s]:", i, recordTypeName);
      for (unsigned j = 0; j < numTabs; ++j) fprintf(stderr, "\t");
      fprintf(stderr, "%d\t", fRecordTypeStats[i].count);
      if (fRecordTypeStats[i].minLength == fRecordTypeStats[i].maxLength) {
	fprintf(stderr, "length:\t\t%d\n", fRecordTypeStats[i].minLength);
      } else {
	fprintf(stderr, "lengths:\t%d-%d\n", fRecordTypeStats[i].minLength, fRecordTypeStats[i].maxLength);
      }
    }
  }
#endif
}

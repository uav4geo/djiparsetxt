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
    Header File.
*/

#ifndef _RECORD_AND_DETAILS_PARSER_HH
#define _RECORD_AND_DETAILS_PARSER_HH

#ifndef _DJI_TXT_PARSER_HH
#include "DJITxtParser.hh"
#endif

#ifndef _FIELD_DATABASE_HH
#include "FieldDatabase.hh"
#endif

class RecordTypeStat {
public:
  RecordTypeStat();

public:
  unsigned count, minLength, maxLength;
};

class RecordAndDetailsParser: public DJITxtParser {
public:
  RecordAndDetailsParser();
  virtual ~RecordAndDetailsParser();

  int parseJPEGRecord(u_int8_t const*& ptr, u_int8_t const* limit);

private: // redefined virtual functions:
  virtual void parseDetailsArea(u_int8_t const*& ptr, u_int8_t const* limit);
  virtual int parseRecord(u_int8_t const*& ptr, u_int8_t const* limit, int isScrambled);
  virtual void summarizeRecordParsing();
  virtual void outputOneRow(int outputColumnLabels);

private:
  // Routines for parsing specific types of record:
  void parseRecord_OSD(u_int8_t const*& ptr, u_int8_t const* limit);
  void parseRecord_HOME(u_int8_t const*& ptr, u_int8_t const* limit);
  void parseRecord_GIMBAL(u_int8_t const*& ptr, u_int8_t const* limit);
  void parseRecord_RC(u_int8_t const*& ptr, u_int8_t const* limit);
  void parseRecord_CUSTOM(u_int8_t const*& ptr, u_int8_t const* limit);
  void parseRecord_DEFORM(u_int8_t const*& ptr, u_int8_t const* limit);
  void parseRecord_CENTER_BATTERY(u_int8_t const*& ptr, u_int8_t const* limit);
  void parseRecord_SMART_BATTERY(u_int8_t const*& ptr, u_int8_t const* limit);
  void parseRecord_APP_TIP(u_int8_t const*& ptr, u_int8_t const* limit);
  void parseRecord_APP_WARN(u_int8_t const*& ptr, u_int8_t const* limit);
  void parseRecord_RECOVER(u_int8_t const*& ptr, u_int8_t const* limit);
  void parseRecord_APP_GPS(u_int8_t const*& ptr, u_int8_t const* limit);
  void parseRecord_FIRMWARE(u_int8_t const*& ptr, u_int8_t const* limit);
  int parseRecord_JPEG(u_int8_t const*& ptr, u_int8_t const* limit);
  void parseRecordUnknownFormat(char const* recordTypeName, u_int8_t const*& ptr, u_int8_t const* limit);

  // Routines for parsing various types of fields within a record:
  void enterSubByteField(char const* label, u_int8_t byte, u_int8_t mask);
  void noteByteField(char const* label, u_int8_t const*& ptr, u_int8_t const* limit,
		     float divisor = 0.0, int isSigned = 0);
      // if "divisor" is not 0.0, divide the value by it, and store it as a float instead
  void note2ByteField(char const* label, u_int8_t const*& ptr, u_int8_t const* limit,
		      float divisor = 0.0, int16_t offset = 0, int isSigned = 0);
      // first subtract "offset" from the value
      // then, if "divisor" is not 0.0, divide the value by it, and store it as a float instead
  void noteSigned2ByteField(char const* label, u_int8_t const*& ptr, u_int8_t const* limit,
			    float divisor = 0.0, int16_t offset = 0) {
    note2ByteField(label, ptr, limit, divisor, offset, 1);
  }
  void noteUnsigned2ByteField(char const* label, u_int8_t const*& ptr, u_int8_t const* limit,
			      float divisor = 0.0, int16_t offset = 0) {
    note2ByteField(label, ptr, limit, divisor, offset, 0);
  }
  void note4ByteField(char const* label, u_int8_t const*& ptr, u_int8_t const* limit,
		      float divisor = 0.0, int isSigned = 0);
      // if "divisor" is not 0.0, divide the value by it, and store it as a float instead
  void note4ByteFloatField(char const* label, u_int8_t const*& ptr, u_int8_t const* limit,
			   float divisor = 0.0);
      // if "divisor" is not 0.0, divide the value by it, before storing
  void note8ByteDoubleField(char const* label, u_int8_t const*& ptr, u_int8_t const* limit);
  void note8ByteLatitudeOrLongitudeFieldInRadians(char const* label, u_int8_t const*& ptr, u_int8_t const* limit);
  void note8ByteLatitudeOrLongitudeFieldInDegrees(char const* label, u_int8_t const*& ptr, u_int8_t const* limit);
  void note2ByteDateField(char const* label, u_int8_t const*& ptr, u_int8_t const* limit);
  void note8ByteTimestampField(char const* label, u_int8_t const*& ptr, u_int8_t const* limit,
			       int isInMilliseconds = 0);
  void noteStringField(char const* label, u_int8_t const*& ptr, unsigned stringLength, u_int8_t const* limit);
  void note3ByteVersionField(char const* label, u_int8_t const*& ptr, u_int8_t const* limit);

private:
  unsigned fNumRecords;
  RecordTypeStat fRecordTypeStats[256];
  char const* fRecordTypeName[256];
  unsigned fMaxNumRecordsForOneType;

  FieldDatabase* fFieldDatabase;
};

#endif

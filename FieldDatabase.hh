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

    A database that stores values of fields.  (Each field is indexed by a 'label' (column name).)
    Header File.
*/

#ifndef _FIELD_DATABASE_HH
#define _FIELD_DATABASE_HH

#ifndef _INTERPRETATION_TABLE_HH
#include "InterpretationTable.hh"
#endif

// How each field value is represented:
enum FieldType {
     IntegerByteUnsigned,
     IntegerByteSigned,
     Integer2ByteUnsigned,
     Integer2ByteSigned,
     Date2Byte,
     Integer4ByteUnsigned,
     Integer4ByteSigned,
     Version4Byte,
     Float,
     Double,
     Timestamp8ByteInSeconds,
     Timestamp8ByteInMilliseconds,
     String
   };

class FieldValue {
public:
  FieldValue(FieldType type, u_int8_t val);
  FieldValue(FieldType type, u_int16_t val);
  FieldValue(FieldType type, u_int32_t val);
  FieldValue(FieldType type, u_int64_t val);
  FieldValue(FieldType type, float val);
  FieldValue(FieldType type, double val);
  FieldValue(FieldType type, char const* str);

  virtual ~FieldValue();

private:
  friend class FieldDatabase;
  FieldType fType;
  union {
    u_int8_t fByte;
    u_int16_t fBytes2;
    u_int32_t fBytes4;
    u_int64_t fBytes8;
    float fFloat;
    double fDouble;
    char const* fStr;
  };
};

class FieldDatabase {
public:
  FieldDatabase();
  virtual ~FieldDatabase();

public:
  // Routines for entering field values into the database:
  void addByteField(char const* label, u_int8_t value, int isSigned);
  void add2ByteField(char const* label, u_int16_t value, int isSigned);
  void add2ByteDateField(char const* label, u_int16_t value);
  void add4ByteField(char const* label, u_int32_t value, int isSigned);
  void add4ByteVersionField(char const* label, u_int32_t value);
  void addFloatField(char const* label, float value);
  void addDoubleField(char const* label, double value);
  void add8ByteTimestampField(char const* label, u_int64_t value, int isInMilliseconds);
  void addStringField(char const* label, char const* str);

  // Routines for outputting field values (to 'stdout'):
  void outputField(char const* label, unsigned numFractionalDigits = 0);
  void outputFieldAsBoolean(char const* label);
  void outputFieldInterpreted(char const* label, char const* interpretedLabel);

private:
  void addFieldValue(char const* label, FieldValue* fieldValue);
  FieldValue const* lookupFieldValue(char const* label); // returns NULL if not found

  void initializeInterpretationTables(); // called by our constructor
  InterpretationTable*
  newInterpretationTable(char const* interpretedLabel,
			 char const* defaultResultString); // called by the above

private:
  // Implement the database using an 'unordered map' - i.e., a hash table:
  std::unordered_map<char const*, FieldValue*> fUMap;

  // We also use an 'unordered map' to look up "InterpretationTable"s:
  std::unordered_map<char const*, InterpretationTable*> fInterpretationTableMap;
};

#endif

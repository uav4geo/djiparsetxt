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
    Implementation.
*/

#include "FieldDatabase.hh"
#include <string.h>

////////// FieldValue: implementation //////////

FieldValue::FieldValue(FieldType type, u_int8_t val) : fType(type), fByte(val) {}
FieldValue::FieldValue(FieldType type, u_int16_t val) : fType(type), fBytes2(val) {}
FieldValue::FieldValue(FieldType type, u_int32_t val) : fType(type), fBytes4(val) {}
FieldValue::FieldValue(FieldType type, u_int64_t val) : fType(type), fBytes8(val) {}
FieldValue::FieldValue(FieldType type, float val) : fType(type), fFloat(val) {}
FieldValue::FieldValue(FieldType type, double val) : fType(type), fDouble(val) {}
FieldValue::FieldValue(FieldType type, char const* str) : fType(type) {
  fStr = strdup(str);
}

FieldValue::~FieldValue() {
  if (fType == String) delete[] fStr;
}


////////// FieldDatabase: implementation //////////

FieldDatabase::FieldDatabase() {
  initializeInterpretationTables();
}

FieldDatabase::~FieldDatabase() {
  // Iterate through each of our 'unordered map's, deleting its contents:
  // "fUMap":
  {
    std::unordered_map<char const*, FieldValue*>:: iterator itr;
    for (itr = fUMap.begin(); itr != fUMap.end(); itr++) {
      delete itr->second;
    }
  }
  // "fInterpretationTableMap":
  {
    std::unordered_map<char const*, InterpretationTable*>:: iterator itr;
    for (itr = fInterpretationTableMap.begin(); itr != fInterpretationTableMap.end(); itr++) {
      delete itr->second;
    }
  }
}

void FieldDatabase::addByteField(char const* label, u_int8_t value, int isSigned) {
  addFieldValue(label, isSigned
		? new FieldValue(IntegerByteSigned, value)
		: new FieldValue(IntegerByteUnsigned, value));
}

void FieldDatabase::add2ByteField(char const* label, u_int16_t value, int isSigned) {
  addFieldValue(label, isSigned
		? new FieldValue(Integer2ByteSigned, value)
		: new FieldValue(Integer2ByteUnsigned, value));
}

void FieldDatabase::add2ByteDateField(char const* label, u_int16_t value) {
  addFieldValue(label, new FieldValue(Date2Byte, value));
}

void FieldDatabase::add4ByteField(char const* label, u_int32_t value, int isSigned) {
  addFieldValue(label, isSigned
		? new FieldValue(Integer4ByteSigned, value)
		: new FieldValue(Integer4ByteUnsigned, value));
}

void FieldDatabase::add4ByteVersionField(char const* label, u_int32_t value) {
  addFieldValue(label, new FieldValue(Version4Byte, value));
}

void FieldDatabase::addFloatField(char const* label, float value) {
  addFieldValue(label, new FieldValue(Float, value));
}

void FieldDatabase::addDoubleField(char const* label, double value) {
  addFieldValue(label, new FieldValue(Double, value));
}

void FieldDatabase::add8ByteTimestampField(char const* label, u_int64_t value, int isInMilliseconds) {
  addFieldValue(label, isInMilliseconds
		? new FieldValue(Timestamp8ByteInMilliseconds, value)
		: new FieldValue(Timestamp8ByteInSeconds, value));
}

void FieldDatabase::addStringField(char const* label, char const* str) {
    addFieldValue(label, new FieldValue(String, str));
}

void FieldDatabase::addFieldValue(char const* label, FieldValue* fieldValue) {
  // First, delete any existing "FieldValue" for this label (the assignment below doesn't do that):
  delete fUMap[label];

  // Then, store the new "FieldValue":
  fUMap[label] = fieldValue;
}

FieldValue const* FieldDatabase::lookupFieldValue(char const* label) {
  return fUMap[label];
}

InterpretationTable* FieldDatabase
::newInterpretationTable(char const* interpretedLabel, char const* defaultResultString) {
  InterpretationTable* it = new InterpretationTable(defaultResultString);
  fInterpretationTableMap[interpretedLabel] = it;

  return it;
}

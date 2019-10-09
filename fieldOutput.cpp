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

    Outputting a single field of data from the table.
    Implementation.
*/

#include "FieldDatabase.hh"
#include <stdio.h>
#include <time.h>

void FieldDatabase::outputField(char const* label, unsigned numFractionalDigits) {
  FieldValue const* fieldValue = lookupFieldValue(label);
  if (fieldValue == NULL) return; // output nothing for a nonexistent field

  int timeIsInMilliseconds = 0; // by default

  switch (fieldValue->fType) {
    case IntegerByteUnsigned: {
      printf("%u", fieldValue->fByte);
      break;
    }
    case IntegerByteSigned: {
      printf("%d", (int8_t)(fieldValue->fByte));
      break;
    }
    case Integer2ByteUnsigned: {
      printf("%u", fieldValue->fBytes2);
      break;
    }
    case Integer2ByteSigned: {
      printf("%d", (int16_t)(fieldValue->fBytes2));
      break;
    }
    case Date2Byte: {
      // Interpret the two bytes as: 7 bits (years since 1980) + 4 bits (month) + 5 bits (day of month):
      printf("%u/%02u/%02u",
	     ((fieldValue->fBytes2&0xFE00)>>9) + 1980,
	     (fieldValue->fBytes2&0x01E0)>>5,
	     (fieldValue->fBytes2&0x001F));

      break;
    }
    case Integer4ByteUnsigned: {
      printf("%u", fieldValue->fBytes4);
      break;
    }
    case Integer4ByteSigned: {
      printf("%d", (int32_t)(fieldValue->fBytes4));
      break;
    }
    case Version4Byte: {
      // Use the first 3 bytes (big-endian) as version numbers:
      u_int32_t v = fieldValue->fBytes4;
      printf("%u.%u.%u", (v>>24)&0xFF, (v>>16)&0xFF, (v>>8)&0xFF);
      break;
    }
    case Float: {
      printf("%.*f", numFractionalDigits, fieldValue->fFloat);
      break;
    }
    case Double: {
      printf("%.*f", numFractionalDigits, fieldValue->fDouble);
      break;
    }
    case Timestamp8ByteInMilliseconds: {
      timeIsInMilliseconds = 1;
      // fall through to:
    }
    case Timestamp8ByteInSeconds: {
      u_int64_t time = fieldValue->fBytes8;
      u_int64_t timeInSeconds;
      unsigned milliseconds;
      
      if (timeIsInMilliseconds) {
	timeInSeconds = time/1000;
	milliseconds = time%1000;
      } else {
	timeInSeconds = time;
	milliseconds = 0;
      }

      struct tm* convertedTime = gmtime((time_t*)&timeInSeconds);
      if (convertedTime == NULL) {
	fprintf(stderr, "outputField(8-byte timestamp): gmtime(%llu) failed!\n", timeInSeconds);
	return;
      }
      printf("%u/%02u/%02u %02u:%02u:%02u",
	     convertedTime->tm_year + 1900, convertedTime->tm_mon + 1, convertedTime->tm_mday,
	     convertedTime->tm_hour, convertedTime->tm_min, convertedTime->tm_sec);
      if (timeIsInMilliseconds) {
	printf(".%03u", milliseconds);
      }
      break;
    }
    case String: {
      printf("%s", fieldValue->fStr);
      break;
    }
  }
}

void FieldDatabase::outputFieldAsBoolean(char const* label) {
  FieldValue const* fieldValue = lookupFieldValue(label);
  if (fieldValue == NULL) return; // output nothing for a nonexistent field

  int booleanValue = 0;

  switch (fieldValue->fType) {
    case IntegerByteUnsigned:
    case IntegerByteSigned: {
      booleanValue = fieldValue->fByte != 0;
      break;
    }
    case Integer2ByteUnsigned:
    case Integer2ByteSigned: {
      booleanValue = fieldValue->fBytes2 != 0;
      break;
    }
    case Integer4ByteUnsigned:
    case Integer4ByteSigned: {
      booleanValue = fieldValue->fBytes4 != 0;
      break;
    }
    default: {
      return; // print nothing if the field cannot be interpreted as Boolean
    }
  }

  printf(booleanValue ? "True" : "False");
}

void FieldDatabase::outputFieldInterpreted(char const* label, char const* interpretedLabel) {
  // First, check whether we currently have a value for the field "label":
  FieldValue const* fieldValue = lookupFieldValue(label);
  if (fieldValue == NULL) return; // output nothing for a nonexistent field

  // Next, check its type.  It needs to be an unsigned integer type <= 4 bytes long:
  u_int32_t intValue;
  switch (fieldValue->fType) {
    case IntegerByteUnsigned: {
      intValue = (u_int32_t)(fieldValue->fByte);
      break;
    }
    case Integer2ByteUnsigned: {
      intValue = (u_int32_t)(fieldValue->fBytes2);
      break;
    }
    case Integer4ByteUnsigned: {
      intValue = fieldValue->fBytes4;;
      break;
    }
    default: {
      return; // bad type
    }
  }
  
  // Now, look up an InterpretationTable for "interpretedLabel":
  InterpretationTable* interpretationTable = fInterpretationTableMap[interpretedLabel];
  if (interpretationTable == NULL) return;

  // And use this to look up (and print) a string 'interpretation' of our integer value:
  printf("%s", interpretationTable->lookup(intValue));
}

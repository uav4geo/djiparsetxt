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

    Parsing individual fields within records within DJI ".txt" files.
    Implementation.
*/

#include "RecordAndDetailsParser.hh"

void RecordAndDetailsParser
::enterSubByteField(char const* label, u_int8_t byte, u_int8_t mask) {
  // First, use "mask" to select the bits that we want, and move them to the low-order part of the byte:
  byte &= mask;
  while (mask != 0x00 && (mask&0x01) == 0) {
    byte >>= 1;
    mask >>= 1;
  }

  fFieldDatabase->addByteField(label, byte, 0);
}

void RecordAndDetailsParser
::noteByteField(char const* label, u_int8_t const*& ptr, u_int8_t const* limit,
		float divisor, int isSigned) {
  u_int8_t byte = getByte(ptr, limit);

  if (divisor != 0.0) {
    // Divide by this, and store the resulting value as a 'float' instead:
    float value = isSigned ? (int8_t)byte/divisor : byte/divisor;
    fFieldDatabase->addFloatField(label, value);
  } else {
    // Normal case:
    fFieldDatabase->addByteField(label, byte, isSigned);
  }
}

void RecordAndDetailsParser
::note2ByteField(char const* label, u_int8_t const*& ptr, u_int8_t const* limit,
		 float divisor, int16_t offset, int isSigned)  {
  u_int16_t bytes = get2BytesLE(ptr, limit);
  bytes -= offset;

  if (divisor != 0.0) {
    // Divide by this, and store the resulting value as a 'float' instead:
    float value = isSigned ? (int16_t)bytes/divisor : bytes/divisor;
    fFieldDatabase->addFloatField(label, value);
  } else {
    // Normal case:
    fFieldDatabase->add2ByteField(label, bytes, isSigned);
  }
}

void RecordAndDetailsParser
::note4ByteField(char const* label, u_int8_t const*& ptr, u_int8_t const* limit,
		 float divisor, int isSigned) {
  u_int32_t bytes = getWord32LE(ptr, limit);

  if (divisor != 0.0) {
    // Divide by this, and store the resulting value as a 'float' instead:
    float value = isSigned ? (int32_t)bytes/divisor : bytes/divisor;
    fFieldDatabase->addFloatField(label, value);
  } else {
    // Normal case:
    fFieldDatabase->add4ByteField(label, bytes, isSigned);
  }
}

void RecordAndDetailsParser
::note4ByteFloatField(char const* label, u_int8_t const*& ptr, u_int8_t const* limit,
		      float divisor) {
  u_int32_t bytes = getWord32LE(ptr, limit);
  float value = *(float*)&bytes;
  if (divisor != 0.0) value /= divisor;
  fFieldDatabase->addFloatField(label, value);
}

void RecordAndDetailsParser
::note8ByteDoubleField(char const* label, u_int8_t const*& ptr, u_int8_t const* limit) {
  u_int64_t bytes = getWord64LE(ptr, limit);
  fFieldDatabase->addDoubleField(label, *(double*)&bytes);
}

#ifndef PI
#define PI 3.14159265359
#endif

void RecordAndDetailsParser
::note8ByteLatitudeOrLongitudeFieldInRadians(char const* label, u_int8_t const*& ptr, u_int8_t const* limit) {
  u_int64_t bytes = getWord64LE(ptr, limit);
  double* value = (double*)&bytes;
  
  // This value is in radians.  Convert it to degrees before storing it as a 'double':
  fFieldDatabase->addDoubleField(label, (*value)*(180/PI));
}

void RecordAndDetailsParser
::note8ByteLatitudeOrLongitudeFieldInDegrees(char const* label, u_int8_t const*& ptr, u_int8_t const* limit) {
  u_int64_t bytes = getWord64LE(ptr, limit);
  double* value = (double*)&bytes;
  
  fFieldDatabase->addDoubleField(label, *value);
}

void RecordAndDetailsParser
::note2ByteDateField(char const* label, u_int8_t const*& ptr, u_int8_t const* limit) {
  u_int16_t bytes = get2BytesLE(ptr, limit);
  fFieldDatabase->add2ByteDateField(label, bytes);
}

void RecordAndDetailsParser
::note8ByteTimestampField(char const* label, u_int8_t const*& ptr, u_int8_t const* limit,
			  int isInMilliseconds) {
  u_int64_t value = getWord64LE(ptr, limit);
  fFieldDatabase->add8ByteTimestampField(label, value, isInMilliseconds);
}

void RecordAndDetailsParser
::noteStringField(char const* label, u_int8_t const*& ptr, unsigned stringLength, u_int8_t const* limit) {
  if (limit != NULL && ptr > limit - stringLength) throw END_OF_DATA;

  char str[stringLength+1];
  unsigned i;
  for (i = 0; i < stringLength; ++i) str[i] = *ptr++;
  str[i] = '\0';

  fFieldDatabase->addStringField(label, str);
}

void RecordAndDetailsParser
::note3ByteVersionField(char const* label, u_int8_t const*& ptr, u_int8_t const* limit) {
  u_int8_t v1 = getByte(ptr, limit);
  u_int8_t v2 = getByte(ptr, limit);
  u_int8_t v3 = getByte(ptr, limit);

  // Pack these three bytes into a 4-byte value (big-endian), and store this:
  u_int32_t value = (v1<<24)|(v2<<16)|(v3<<8);
  fFieldDatabase->add4ByteVersionField(label, value);
}

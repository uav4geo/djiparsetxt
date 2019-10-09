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

    General parsing routines, and parser base class.
    Implementation.
*/

#include "DJITxtParser.hh"
#include <stdio.h>

u_int8_t getByte(u_int8_t const*& ptr, u_int8_t const* limit) {
  if (limit != NULL && ptr > limit-1) throw END_OF_DATA;
  return *ptr++;
}

u_int16_t get2BytesBE(u_int8_t const*& ptr, u_int8_t const* limit) {
  if (limit != NULL && ptr > limit-2) throw END_OF_DATA;
  u_int8_t byte1 = *ptr++;
  u_int8_t byte2 = *ptr++;
  return (byte1<<8)|byte2;
}

u_int16_t get2BytesLE(u_int8_t const*& ptr, u_int8_t const* limit) {
  if (limit != NULL && ptr > limit-2) throw END_OF_DATA;
  u_int8_t byte1 = *ptr++;
  u_int8_t byte2 = *ptr++;
  return (byte2<<8)|byte1;
}

unsigned getWord32BE(u_int8_t const*& ptr, u_int8_t const* limit) {
  if (limit != NULL && ptr > limit-4) throw END_OF_DATA;
  unsigned result = (ptr[0]<<24)|(ptr[1]<<16)|(ptr[2]<<8)|ptr[3];
  ptr += 4;
  return result;
}

unsigned getWord32LE(u_int8_t const*& ptr, u_int8_t const* limit) {
  if (limit != NULL && ptr > limit-4) throw END_OF_DATA;
  unsigned result = (ptr[3]<<24)|(ptr[2]<<16)|(ptr[1]<<8)|ptr[0];
  ptr += 4;
  return result;
}

u_int64_t getWord64LE(u_int8_t const*& ptr, u_int8_t const* limit) {
  u_int64_t resultLow = getWord32LE(ptr, limit);
  u_int64_t resultHigh = getWord32LE(ptr, limit);
  return (resultHigh<<32)|resultLow;
}

void printString(char const* label, u_int8_t const*& ptr, unsigned stringLength, u_int8_t const* limit) {
  if (limit != NULL && ptr > limit - stringLength) throw END_OF_DATA;

  char str[stringLength+1];
  unsigned i;
  for (i = 0; i < stringLength; ++i) str[i] = *ptr++;
  str[i] = '\0';

  if (label == NULL) {
    fprintf(stderr, "%s\n", str);
  } else {
    fprintf(stderr, "%s: %s\n", label, str);
  }
}

void printHex(char const* label, u_int8_t const*& ptr, u_int8_t const* limit) {
  if (limit == NULL) return;
  if (label != NULL) fprintf(stderr, "%s: ", label);
  for (u_int8_t const* p = ptr; p < limit; ++p) fprintf(stderr, ":%02x", *p);
  fprintf(stderr, "\n");
}


////////// DJITxtParser implementation //////////

DJITxtParser::DJITxtParser() {
}

DJITxtParser::~DJITxtParser() {
}

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
    Header File.
*/

#ifndef _DJI_TXT_PARSER_HH
#define _DJI_TXT_PARSER_HH

#include <sys/types.h>
#include <stdlib.h>

// Uncomment the following line (then: make clean; make) to generate more debugging output:
//#define DEBUG_RECORD_PARSING 1

// In each of these routines, "limit" is 1 byte past the end of the usable data.
u_int8_t getByte(u_int8_t const*& ptr, u_int8_t const* limit = NULL); 
u_int16_t get2BytesBE(u_int8_t const*& ptr, u_int8_t const* limit = NULL);
u_int16_t get2BytesLE(u_int8_t const*& ptr, u_int8_t const* limit = NULL);
unsigned getWord32BE(u_int8_t const*& ptr, u_int8_t const* limit = NULL);
unsigned getWord32LE(u_int8_t const*& ptr, u_int8_t const* limit = NULL);
u_int64_t getWord64LE(u_int8_t const*& ptr, u_int8_t const* limit = NULL);

// The following routines are used for debugging:
void printString(char const* label, u_int8_t const*& ptr, unsigned stringLength, u_int8_t const* limit = NULL);
void printHex(char const* label, u_int8_t const*& ptr, u_int8_t const* limit);

#define END_OF_DATA 1 // exception thrown if parsing unexpectedly reaches the end of the buffer

class DJITxtParser {
public:
  static DJITxtParser* createNew();

protected:
  DJITxtParser(); // called only by "createNew()"

public:
  virtual ~DJITxtParser();

  virtual void parseDetailsArea(u_int8_t const*& ptr, u_int8_t const* limit) = 0;
  virtual int parseRecord(u_int8_t const*& ptr, u_int8_t const* limit, int isScrambled) = 0;
  virtual void outputOneRow(int outputColumnLabels = 0) = 0;
  virtual void summarizeRecordParsing() = 0;
};

#endif

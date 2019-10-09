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
*/

#include "DJITxtParser.hh"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>

#define OLD_HEADER_SIZE 12
#define NEW_HEADER_SIZE 100
#define MIN_RECORD_SIZE 3 // type+0-length+FF

u_int32_t fileVersionNumber;

int main(int argc, char** argv) {
  fprintf(stderr, "\"%s\", version 2019-02-08. Copyright (c) 2019 Live Networks, Inc. All rights reserved.\n", argv[0]);
  fprintf(stderr, "For the latest version of this program (and more information), visit http://djilogs.live555.com\n");

  int fileNamePos = 1;
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <txtFileName>\n", argv[0]);
    return 1;
  }
  char const* fileName = argv[fileNamePos];

  FILE* fid = fopen(fileName, "rb");
  if (fid == NULL) {
    fprintf(stderr, "Failed to open \"%s\"\n", fileName);
    return 1;
  }

  // Figure out the file's size
  struct stat sb;
  u_int64_t fileSize;
  if (stat(fileName, &sb) == 0) {
    fileSize = sb.st_size;
  } else {
    fprintf(stderr, "Failed to get file size\n");
    return 1;
  }

  // Check the file size:
  if (fileSize < OLD_HEADER_SIZE) {
    fprintf(stderr, "Bad file size: %lld bytes\n", fileSize);
    return 1;
  }

  // Map the file into memory:
  u_int8_t* const mappedFile
    = (u_int8_t*)mmap(0, fileSize, PROT_READ, MAP_SHARED, fileno(fid), 0);
  if (mappedFile == MAP_FAILED) {
    extern int errno;
    fprintf(stderr, "mmap() call failed: %s\n", strerror(errno));
    return 1;
  }

  // Get/check the first 8 bytes (little-endian) of the file; it's the size of the header+record area:
  u_int8_t const* ptr = mappedFile;
  u_int64_t headerPlusRecordAreaSize = getWord64LE(ptr);

  // The next 4 bytes are the file version number (apparently big-endian):
  fileVersionNumber = getWord32BE(ptr);
  fprintf(stderr, "File version number: 0x%08x\n", fileVersionNumber);


  // Old versions of the file have a smaller header, and are not scrambled:
  unsigned headerSize;
  int isScrambled;
  if ((fileVersionNumber&0x0000FF00) < 0x00000600) {
    headerSize = OLD_HEADER_SIZE;
    isScrambled = 0;
  } else {
    headerSize = NEW_HEADER_SIZE;
    isScrambled = 1;
  }

  unsigned const minFileSize = headerSize + MIN_RECORD_SIZE;
  if (headerPlusRecordAreaSize < minFileSize || headerPlusRecordAreaSize > fileSize) {
    fprintf(stderr, "Bad 'header+record-area' size: %lld (0x%llx); file size is %lld\n", headerPlusRecordAreaSize, headerPlusRecordAreaSize, fileSize);
    return 1;
  }

  // Create a parser:
  DJITxtParser* parser = DJITxtParser::createNew();

  // Begin by parsing the 'DETAILS' area (the data after the header+record area):
  u_int8_t* const detailsArea = &mappedFile[headerPlusRecordAreaSize];
  u_int8_t* const endOfDetailsArea = &mappedFile[fileSize];
  ptr = detailsArea;
  parser->parseDetailsArea(ptr, endOfDetailsArea);

  // Then, parse all of the records in the file:
  u_int8_t* const recordArea = &mappedFile[headerSize];
  u_int8_t* const endOfRecordArea = detailsArea;

  ptr = recordArea;
  while (ptr < endOfRecordArea) {
#ifdef DEBUG_RECORD_PARSING
    u_int64_t curFilePosition = ptr - mappedFile;
    fprintf(stderr, "@0x%08llx: ", curFilePosition);
#endif
    if (!parser->parseRecord(ptr, endOfRecordArea, isScrambled)) break;
  }
  if (ptr < endOfRecordArea) {
    u_int64_t curFilePosition = ptr - mappedFile;
    fprintf(stderr, "Premature end of record parsing at file position %lld (0x%08llx)\n", curFilePosition, curFilePosition);
  }
  parser->outputOneRow(); // the final row of data
  parser->summarizeRecordParsing();

  delete parser;

  fprintf(stderr, "Done writing CSV.\n");
  return 0;
}

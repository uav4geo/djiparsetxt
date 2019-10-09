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

    Parsing JPEG records within DJI ".txt" files.
    Implementation.
*/

#include "RecordAndDetailsParser.hh"

#include <stdio.h>
#include <string.h>

#define JPEG_SOI_BYTE 0xD8
#define JPEG_SOI ((0xFF<<8)|JPEG_SOI_BYTE)
#define JPEG_EOI 0xFFD9

int outputJPGFiles = 1;

static char outputJPGFilenamePrefix[] = "embeddedImage";

static FILE* openOutputJPGFile() {
  static unsigned jpgFileNumber = 0;
  char outputFileName[strlen(outputJPGFilenamePrefix) + 100];
  sprintf(outputFileName, "embedded%d.jpg", ++jpgFileNumber);
  FILE* outputFid = fopen(outputFileName, "wb");
  if (outputFid == NULL) {
    fprintf(stderr, "Failed to open output JPG file \"%s\"\n", outputFileName);
  } else {
    fprintf(stderr, "\tOutputting embedded JPEG image to the file \"%s\"\n", outputFileName);

    // Begin by writing the initial JPEG SOI code:
    fputc(JPEG_SOI>>8, outputFid); fputc(JPEG_SOI, outputFid);
  }

  return outputFid;
}

int RecordAndDetailsParser::parseRecord_JPEG(u_int8_t const*& ptr, u_int8_t const* limit) {
  // Skip the first two bytes (both zero) in the record:
  (void)get2BytesBE(ptr, limit);

  if (ptr == limit) return 1; // We're done; there are no JPEG images in the record

  // The next two bytes should be the JPEG 'start of image' code:
  if (get2BytesBE(ptr, limit) != JPEG_SOI) {
    // Unknown contents. Skip all bytes up to and including the next 0xFF (or until "limit"):
    while (ptr < limit && *ptr++ != 0xFF) {}
    return 1;
  }
  
  // The JPEG data is all following data, up to (and including) the next JPEG 'end of image' code,
  // that's not then immediately followed by a JPEG 'start of image' code:
  FILE* outputFid;
  if (outputJPGFiles) {
    outputFid = openOutputJPGFile();
    if (outputFid == NULL) return 0;
  }
    
  while (1) {
    u_int16_t next2Bytes = get2BytesBE(ptr, limit);
    if (next2Bytes == JPEG_EOI) {
      if (outputJPGFiles) {
	// We've finished writing the JPG file:
	fputc(JPEG_EOI>>8, outputFid); fputc(JPEG_EOI, outputFid);
	fclose(outputFid);
      }

      // Look for an immediately following JPEG 'start of image' code (if there's more data left):
      if (ptr == limit) return 1; // we're done
      next2Bytes = get2BytesBE(ptr, limit);
      if (next2Bytes == JPEG_SOI) {
	if (outputJPGFiles) {
	  outputFid = openOutputJPGFile();
	  if (outputFid == NULL) return 0;
	}
      } else {
	ptr -= 2;
	return 1;
      }
    } else {
      // Output the first byte, then continue:
      if (outputJPGFiles) {
	u_int8_t firstByte = next2Bytes >> 8;
	fputc(firstByte, outputFid);
      }
      --ptr;
    }
  }

  return 0; // never reached
}

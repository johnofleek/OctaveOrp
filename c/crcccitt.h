/*
 * Library: libcrc
 * File:    include/checksum.h
 * Author:  Lammert Bies
 *
 * This file is licensed under the MIT License as stated below
 *
 * Copyright (c) 1999-2018 Lammert Bies
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Description
 * -----------
 * The headerfile include/checksum.h contains the definitions and prototypes
 * for routines that can be used to calculate several kinds of checksums.
 */

#ifndef DEF_LIBCRC_CHECKSUM_H
#define DEF_LIBCRC_CHECKSUM_H

#ifdef __cplusplus
extern "C" {
#endif

// #include <stddef.h>
// #include <stdint.h>
#include "orp.h"

#define CCITT_TABLE_SIZE	256 //256

/*
 * #define CRC_POLY_xxxx
 *
 * The constants of the form CRC_POLY_xxxx define the polynomials for some well
 * known CRC calculations.
 */

#define		CRC_POLY_16		0xA001
#define		CRC_POLY_32		0xEDB88320ul
#define		CRC_POLY_64		0x42F0E1EBA9EA3693ull
#define		CRC_POLY_CCITT		0x1021
#define		CRC_POLY_DNP		0xA6BC
#define		CRC_POLY_KERMIT		0x8408
#define		CRC_POLY_SICK		0x8005

/*
 * #define CRC_START_xxxx
 *
 * The constants of the form CRC_START_xxxx define the values that are used for
 * initialization of a CRC value for common used calculation methods.
 */

#define		CRC_START_8		0x00
#define		CRC_START_16		0x0000
#define		CRC_START_MODBUS	0xFFFF
#define		CRC_START_XMODEM	0x0000
#define		CRC_START_CCITT_1D0F	0x1D0F
#define		CRC_START_CCITT_FFFF	0xFFFF
#define		CRC_START_KERMIT	0x0000
#define		CRC_START_SICK		0x0000
#define		CRC_START_DNP		0x0000
#define		CRC_START_32		0xFFFFFFFFul
#define		CRC_START_64_ECMA	0x0000000000000000ull
#define		CRC_START_64_WE		0xFFFFFFFFFFFFFFFFull

/*
 * Prototype list of global functions
 */

/// crcccitt ffff calculator
///
/// Takes a payload string and returns CRC value
/// 
/// @param  const unsigned char * input payload
/// @param size_t payload size in bytes
/// @returns uint16_t representation of the calculated CRC
uint16_t		crc_ccitt_ffff(     const unsigned char *input_str, size_t num_bytes       );

/// crcccitt init or constructor
///
/// Must be called before using the calculator
/// 
/// @param uint16_t * app_crc_tabccitt is a pointer to the app CRC table resource
/// @param size_t app_crc_tabccitt_size is the sizeof the app CRC table
/// @returns 0 if ok -1 if the table is not the correct size
int8_t crcccitt_crcccitt(uint16_t * app_crc_tabccitt, size_t app_crc_tabccitt_size);

/*
 * Global CRC lookup tables
 */

extern const uint32_t	crc_tab32[];
extern const uint64_t	crc_tab64[];

#ifdef __cplusplus
}
#endif

#endif  // DEF_LIBCRC_CHECKSUM_H

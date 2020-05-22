/*
Copyright <2020> <J Thompson>
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/ 

#ifndef ORP_OUTPUT_H
#define ORP_OUTPUT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "orp.h"


// Output registration call back handler prototype
// this is called when the Octave Edge device response over ORP

// example Notification from Octave Edge
// c@01T1585927713.843660,Pval/od/value,D456.000000
// 0x7E,0x63,0x40,0x30,0x31,0x54,0x31,0x35,0x38,0x35,0x39,0x32,0x37,0x37,0x31,0x33,0x2E,0x38,0x34,0x33,0x36,0x36,0x30,0x2C,0x50,0x76,0x61,0x6C,0x2F,0x6F,0x64,0x2F,0x76,0x61,0x6C,0x75,0x65,0x2C,0x44,0x34,0x35,0x36,0x2E,0x30,0x30,0x30,0x30,0x30,0x30,0x53,0x65,0x7E




// Creates remote resource in the Octave edge device
// ----------------------------------------------------------------------------
/// Register a numeric output in Octave Remote
/// 
/// @param keyString Null terminated string that contains the remote path
/// @returns A value of orpProtocol_txBuffer_size or more means that the output was truncated
/// @see orp_protocol()
// ----------------------------------------------------------------------------
int16_t orp_output_create_num(char *keyString);



// ----------------------------------------------------------------------------
/// Registers a callback inside the Octave edge device
///
/// When data arrives on the flow the octave edge device triggers the callback 
/// registered with orp_protocol()  
/// callback flow is -- Octave Edge device (WP77xx) --ORP--> Microcontroller 
/// 
/// @param keyString Null terminated string that contains the remote path
/// @returns A value of orpProtocol_txBuffer_size or more means that the output was truncated
/// @see orp_protocol()
// ----------------------------------------------------------------------------
int16_t orp_output_register_num(char *keyString);

// Creates remote resource in the Octave edge device
// ----------------------------------------------------------------------------
/// Register a JSON output in Octave Remote
/// 
/// @param keyString Null terminated string that contains the remote path
/// @returns A value of orpProtocol_txBuffer_size or more means that the output was truncated
/// @see orp_protocol()
// ----------------------------------------------------------------------------
int16_t orp_output_create_json(char *keyString);



// ----------------------------------------------------------------------------
/// Registers a callback inside the Octave edge device
///
/// When data arrives on the flow the octave edge device triggers the callback 
/// registered with orp_protocol()  
/// callback flow is -- Octave Edge device (WP77xx) --ORP--> Microcontroller 
/// 
/// @param keyString Null terminated string that contains the remote path
/// @returns A value of orpProtocol_txBuffer_size or more means that the output was truncated
/// @see orp_protocol()
// ----------------------------------------------------------------------------
int16_t orp_output_register_json(char *keyString);


// Creates remote resource in the Octave edge device
// ----------------------------------------------------------------------------
/// Register a String output in Octave Remote
/// 
/// @param keyString Null terminated string that contains the remote path
/// @returns A value of orpProtocol_txBuffer_size or more means that the output was truncated
/// @see orp_protocol()
// ----------------------------------------------------------------------------
int16_t orp_output_register_string(char *keyString);


// ----------------------------------------------------------------------------
/// Registers a callback inside the Octave edge device
///
/// When data arrives on the flow the octave edge device triggers the callback 
/// registered with orp_protocol()  
/// callback flow is -- Octave Edge device (WP77xx) --ORP--> Microcontroller 
/// 
/// @param keyString Null terminated string that contains the remote path
/// @returns A value of orpProtocol_txBuffer_size or more means that the output was truncated
/// @see orp_protocol()
// ----------------------------------------------------------------------------
int16_t orp_output_create_string(char *keyString);

#ifdef __cplusplus
}
#endif

#endif // ORP_OUTPUT_H
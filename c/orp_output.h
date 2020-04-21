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
int16_t orp_output_create_num(char *keyString);

// Registers a callback inside the Octave edge
// so that the local handler gets called when something changes on the edge device
int16_t orp_output_registerCallback_num(char *keyString);

#ifdef __cplusplus
}
#endif

#endif // ORP_OUTPUT_H
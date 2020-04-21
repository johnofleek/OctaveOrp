/*
Copyright <2020> <J Thompson>
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/ 

#ifndef ORP_H
#define ORP_H

#define DEBUG

#include "debug.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

#include "crcccitt.h"
#include "hdlc.h"
#include "orp_protocol.h"
#include "orp_input.h"
#include "orp_output.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
    *Notes*

    Due to the handshake method and lack of message context over ORP
    The remote orp is going to have to respond busy if already waitng for
     a response from the Octave Edge. This applies to
     1. Octave inputs
     2. Octave registration - this may need to block while the process completes
        or maybe return with a request to call again or maybe we attach a timer
        to drive a statemachine


    We need to be careful with Octave ORP remote stream registration
    The current 2020-03 versions of Octave Edge remote have volatile
     registration. This means that if the Octave Edge Device resets
     but the remote does not then the remote will need to
     re-register 
*/

#ifdef __cplusplus
}
#endif

#endif // ORP_H

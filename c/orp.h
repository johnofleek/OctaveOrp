#define DEBUG

#include "debug.h"

#include <stdint.h>
#include <stdbool.h>

#include "hdlc.h"
#include "orp_protocol.h"
#include "orp_input.h"
#include "orp_output.h"



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


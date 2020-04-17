#ifndef ORP_OUTPUT_H
#define ORP_OUTPUT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "orp.h"
// orp_input.h
// #include <stdint.h>
// #include <stdbool.h>

// Output registration call back handler prototype
// this is called when the Octave Edge device response over ORP

// I think this needs changing to (index , responseCode) 
typedef void (*orp_register_outputResponse_cbf)(char * );

// As the data is stored in the struct - just pass the index back
typedef void (*orp_output_unsolicitedData_cbf)(uint8_t idx);

// This struct is created by the used app and passed via registration request
typedef struct
{
	// the key is really the Octave remote ORP sub path
	char *keyString;

    // this gets populated by the Octave cloud - we need to know the maxsize
    char *valueString;
    uint8_t valueString_maxsize;	

	// gets called when the registration is complete
	orp_register_outputResponse_cbf userApp_OctaveRegistrationResponse_cbh;	
    
    // is called whenever Octave sends data to remote
    // maybe it should 
    orp_output_unsolicitedData_cbf  userApp_OctaveUnsolicitedResponse_cbh;
	
    // true when output was registered with Octave
    bool orp_output_registered;
} orp_output_struct;

// The input registration request
uint8_t orp_output_s_register(orp_output_struct *inputKeys, int8_t numberOfInputs);

#ifdef __cplusplus
}
#endif

#endif // ORP_OUTPUT_H
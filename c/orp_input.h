// orp_input.h
#include <stdint.h>
#include <stdbool.h>



// Input registration call back handler
// this is called when the Octave Edge device response over ORP
typedef void (*orp_register_inputResponse_cbf)(char *);

// This struct is created by the used app and passed via registration request
typedef struct
{
	// the key is really the remote sub path
	char *keyString;	
	// gets called when the registration is complete
	orp_register_inputResponse_cbf userApp_OctaveResponse_cbh;	
	bool orp_input_registered;
} orp_input_struct;

// The input registration request
uint8_t orp_input_registerJson(orp_input_struct inputKey_struct);

// The Octave input data send Remote device --> Octave Edge device
uint8_t orp_input_sendJson( char * keyString, char * valueString);
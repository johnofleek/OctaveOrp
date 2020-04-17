#ifndef ORP_INPUT_H
#define ORP_INPUT_H

#ifdef __cplusplus
extern "C" {
#endif

// #include <stdint.h>
// #include <stdbool.h>



// Input registration call back handler
// this is called when the Octave Edge device response over ORP
typedef void (*orp_register_inputResponse_cbf)(const uint8_t *buffer, uint16_t bufferLength);

// This struct is created by the used app and passed via registration request
typedef struct
{
	// the key is really the remote sub path
	char *keyString;	
	// gets called when the registration is complete
	orp_register_inputResponse_cbf userApp_OctaveResponse_cbh;	
	bool isRegistered;
	orp_responseStatusEnum currentResponseStatus; // status of request response (ack)
} orp_input_struct;

// The input registration request
int8_t orp_input_registerJson(orp_input_struct inputKey_struct);

// The Octave input data send Remote device --> Octave Edge device
int8_t orp_input_sendJson( orp_input_struct inputKey_struct, char * valueString);

#ifdef __cplusplus
}
#endif

#endif // ORP_INPUT_H
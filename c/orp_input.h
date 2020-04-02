// orp_input.h
#include <stdint.h>

typedef void (*orp_register_inputResponse_cbf)(char *);

typedef struct
{
	// the key is really the remote sub path
	char *keyString;	
	// gets called when the registration is complete
	orp_register_inputResponse_cbf userApp_OctaveResponse_cbh;	
} orp_input_struct;

uint8_t orp_input_s_register(orp_input_struct *inputKeys, int8_t numberOfInputs);


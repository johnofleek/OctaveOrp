// orp_input.h
#include <stdint.h>

typedef void (*octave_response_cbf)(char *);

typedef struct
{
	char *keyString;
	octave_response_cbf userApp_OctaveResponse_cbh;
} octaveInput_struct;

uint8_t octave_register_inputs(octaveInput_struct *inputKeys, int8_t numberOfInputs);


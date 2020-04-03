// orp.c
#include "orp_input.h"

uint8_t orp_input_s_register(orp_input_struct *inputKeys, int8_t numberOfInputs)
{
	uint8_t loop = 0;

	// reset the registered flag
	for(loop = 0; loop < numberOfInputs; loop++)
	{
		inputKeys[loop].orp_input_registered = false;
	}
	// register the Octave inputs
	// then exit
	return(0);
}


uint8_t orp_input_send(orp_input_struct *inputKeys, int8_t inputIndex, char * valueString)
{
	return(0);
}
#include "orp_output.h"

uint8_t orp_output_s_register(orp_output_struct *outputKeys, int8_t numberOfInputs)
{
	uint8_t loop = 0;

	// reset the registered flag
	for(loop = 0; loop < numberOfInputs; loop++)
	{
		outputKeys[loop].orp_output_registered = false;
	}
	// register the Octave outputs
	// then exit
	return(0);
}



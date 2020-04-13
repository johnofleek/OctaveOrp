// orp
#include "orp.h"

// debug only
#ifdef DEBUG
	#include <stdio.h>
#endif
// hdlc




uint8_t orp_input_registerJson(orp_input_struct inputKey_struct)
{
	int16_t retVal = 0;

	// reset the registered flag
	inputKey_struct.orp_input_registered = false;

	// register the Octave inputs
	retVal = orp_protocol_createResource(
		SBR_PKT_RQST_INPUT_CREATE,
		SBR_DATA_TYPE_JSON,
		inputKey_struct.keyString,
		""
	); 

	// register the callback handler
	// TODO:


	// then exit
	return(retVal);
}


uint8_t orp_input_sendJson( char * keyString, char * valueString)
{
	orp_protocol_pushValue(SBR_DATA_TYPE_NUMERIC, keyString , valueString );
	return(0);
}
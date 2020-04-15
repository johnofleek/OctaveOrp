// orp
#include "orp.h"

// debug only
#ifdef DEBUG
	#include <stdio.h>
#endif
// hdlc




int8_t orp_input_registerJson(orp_input_struct inputKey_struct)
{
	int8_t retVal = 0;

	// reset the registered flag
	inputKey_struct.isRegistered = false;
	inputKey_struct.currentResponseStatus = WAITING_RESPONSE;

	// register the Octave inputs
	retVal = orp_protocol_createResource(
		SBR_PKT_RQST_INPUT_CREATE,
		SBR_DATA_TYPE_JSON,
		inputKey_struct.keyString,
		"",
		inputKey_struct.userApp_OctaveResponse_cbh // register the callback handler
	); 

	return(retVal);
}


int8_t orp_input_sendJson(
	orp_input_struct inputKey_struct,
	char * valueString)
{
	orp_protocol_pushValue(
		SBR_DATA_TYPE_NUMERIC,
		inputKey_struct.keyString,
		valueString,
		inputKey_struct.userApp_OctaveResponse_cbh // register the callback handler
	);
	return(0);
}
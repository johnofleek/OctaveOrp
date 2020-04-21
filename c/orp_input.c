// orp
#include "orp.h"

// debug only
#ifdef DEBUG
	#include <stdio.h>
#endif
// hdlc




int8_t orp_input_registerJson
(
	char * keyString
)
{
	int8_t retVal = 0;

	// register the Octave inputs
	retVal = orp_protocol_createResource(
		SBR_PKT_RQST_INPUT_CREATE,
		SBR_DATA_TYPE_JSON,
		keyString,
		""
	); 

	return(retVal);
}


int8_t orp_input_sendJson
(
	char * keyString,
	char * valueString
)
{
	orp_protocol_pushValue(
		SBR_DATA_TYPE_JSON,
		keyString,
		valueString
		
	);
	return(0);
}
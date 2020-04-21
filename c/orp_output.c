#include "orp.h"

// Creates remote resource in the Octave edge device
int16_t orp_output_create_num(char *keyString)
{

	// register the Octave output
	// then exit

    int16_t retVal;
	
	retVal = orp_protocol_createResource(
		SBR_PKT_RQST_OUTPUT_CREATE,
		SBR_DATA_TYPE_NUMERIC,
		keyString,
		""
	);
	return(retVal);
}

// Registers a callback inside the Octave edge
// so that the local handler gets called when something changes on the edge device
int16_t orp_output_registerCallback_num(char *keyString)
{
	int16_t retVal;

	retVal = orp_protocol_addpushHandler(
  		SBR_DATA_TYPE_NUMERIC,
  		keyString
	);
	return(retVal);
}



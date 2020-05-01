/*
Copyright <2020> <J Thompson>
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/ 


// orp
#include "orp.h"

// debug only
#ifdef DEBUG
	#include <stdio.h>
#endif
// hdlc


// ----------------------------------------------------------------------------
// JSON
// ----------------------------------------------------------------------------

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



int8_t orp_input_registerNumeric
(
	char * keyString
)
{
	int8_t retVal = 0;

	// register the Octave inputs
	retVal = orp_protocol_createResource(
		SBR_PKT_RQST_INPUT_CREATE,
		SBR_DATA_TYPE_NUMERIC,
		keyString,
		""
	); 

	return(retVal);
}



int8_t orp_input_sendNumeric
(
	char * keyString,
	char * valueString
)
{
	orp_protocol_pushValue(
		SBR_DATA_TYPE_NUMERIC,
		keyString,
		valueString
		
	);
	return(0);
}
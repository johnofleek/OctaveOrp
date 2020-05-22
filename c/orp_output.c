/*
Copyright <2020> <J Thompson>
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/ 

#include "orp.h"

// Creates remote numeric resource in the Octave edge device
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
int16_t orp_output_register_num(char *keyString)
{
	int16_t retVal;

	retVal = orp_protocol_addpushHandler(
  		SBR_DATA_TYPE_NUMERIC,
  		keyString
	);
	return(retVal);
}


// SBR_DATA_TYPE_JSON
// Creates remote resource in the Octave edge device
int16_t orp_output_create_json(char *keyString)
{

	// register the Octave output
	// then exit

    int16_t retVal;
	
	retVal = orp_protocol_createResource(
		SBR_PKT_RQST_OUTPUT_CREATE,
		SBR_DATA_TYPE_JSON,
		keyString,
		""
	);
	return(retVal);
}

// Registers a callback inside the Octave edge
// so that the local handler gets called when something changes on the edge device
int16_t orp_output_register_json(char *keyString)
{
	int16_t retVal;

	retVal = orp_protocol_addpushHandler(
  		SBR_DATA_TYPE_JSON,
  		keyString
	);
	return(retVal);
}

// SBR_DATA_TYPE_STRING
// Creates remote resource in the Octave edge device
int16_t orp_output_create_string(char *keyString)
{

	// register the Octave output
	// then exit

    int16_t retVal;
	
	retVal = orp_protocol_createResource(
		SBR_PKT_RQST_OUTPUT_CREATE,
		SBR_DATA_TYPE_STRING,
		keyString,
		""
	);
	return(retVal);
}

// Registers a callback inside the Octave edge
// so that the local handler gets called when something changes on the edge device
int16_t orp_output_register_string(char *keyString)
{
	int16_t retVal;

	retVal = orp_protocol_addpushHandler(
  		SBR_DATA_TYPE_STRING,
  		keyString
	);
	return(retVal);
}
/*
Copyright <2020> <J Thompson>
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/ 

#ifndef ORP_INPUT_H
#define ORP_INPUT_H

#ifdef __cplusplus
extern "C" {
#endif




// The input registration request
// ----------------------------------------------------------------------------
/// Register a JSON input in Octave Remote
/// 
/// @param keyString Null terminated string that contains the remote path
/// @returns A value of orpProtocol_txBuffer_size or more means that the output was truncated
/// @see orp_protocol()
// ----------------------------------------------------------------------------
int8_t orp_input_registerJson(char *keyString);

// The Octave input data send Remote device --> Octave Edge device
// ----------------------------------------------------------------------------
/// Sends a JSON string to Octave Remote input
/// 
/// @param keyString Null terminated string that contains the remote path
/// @param valueString Null terminated string that represents a JSON value (see Octave docs)
/// @returns A value of orpProtocol_txBuffer_size or more means that the output was truncated
/// @see orp_protocol()
// ----------------------------------------------------------------------------
int8_t orp_input_sendJson( char *keyString , char * valueString);


// The input registration request
// ----------------------------------------------------------------------------
/// Register a numeric input in Octave Remote
/// 
/// @param keyString Null terminated string that contains the remote path
/// @returns A value of orpProtocol_txBuffer_size or more means that the output was truncated
/// @see orp_protocol()
// ----------------------------------------------------------------------------
int8_t orp_input_sendNumeric ( char * keyString, char * valueString);

// The Octave input data send Remote device --> Octave Edge device
// ----------------------------------------------------------------------------
/// Sends a numeric value to Octave Remote input
/// 
/// @param keyString Null terminated string that contains the remote path
/// @param valueString Null terminated string that represents a numeric value (see Octave docs)
/// @returns A value of orpProtocol_txBuffer_size or more means that the output was truncated
/// @see orp_protocol()
// ----------------------------------------------------------------------------
int8_t orp_input_registerNumeric ( char * keyString);

#ifdef __cplusplus
}
#endif

#endif // ORP_INPUT_H
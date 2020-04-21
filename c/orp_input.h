#ifndef ORP_INPUT_H
#define ORP_INPUT_H

#ifdef __cplusplus
extern "C" {
#endif




// The input registration request
int8_t orp_input_registerJson(char *keyString);

// The Octave input data send Remote device --> Octave Edge device
int8_t orp_input_sendJson( char *keyString , char * valueString);

#ifdef __cplusplus
}
#endif

#endif // ORP_INPUT_H
/*
 * atmel.c
 *
 * Created: 01-Apr-20 12:53:23 PM
 * Author : john
 */ 

//#include <avr/io.h>
#include <stdint.h>


// orp.h

typedef void (*octave_response_cbf)(char *);

typedef struct
{
	char *keyString;
	octave_response_cbf userApp_OctaveResponse_cbh;
} octaveInput_struct;

uint8_t octave_register_inputs(octaveInput_struct *inputKeys, int8_t numberOfInputs);


// orp.c
uint8_t octave_register_inputs(octaveInput_struct *inputKeys, int8_t numberOfInputs)
{
	// register the Octave inputs
	// then exit
	return(0);
}



// The c app

// This is the real user application
// #include "orp.h"

// in the app create an array of structs
#define NUMBER_OF_INPUTS 5



void testApi(void)
{
	int8_t numberOfInputs = 2;
	
	octaveInput_struct octaveInputs_struct[NUMBER_OF_INPUTS];
	octave_register_inputs( octaveInputs_struct, numberOfInputs);	
}




int main(void)
{
	testApi();
	
    /* Replace with your application code */
    while (1) 
    {
    }
}


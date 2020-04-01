#include <stdint.h>
#include <stdio.h>  // just for printf debug
#include "orp.h"





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

int main()
{
    printf("Done\r\n");
}
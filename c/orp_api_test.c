#include <stdint.h>
#include <stdio.h>  // just for printf debug
#include "orp.h"





// The c app

// This is the real user application
// #include "orp.h"



// ----------------------------------------------------------------------------
// test the orp initialisation functionality
#define HDLC_FRAMEBUFFER_LENGTH 64
uint8_t HDLC_frameBuffer[HDLC_FRAMEBUFFER_LENGTH];
uint16_t HDLC_frameBuffer_length = HDLC_FRAMEBUFFER_LENGTH;
uint16_t HDLC_maxFrameLength = HDLC_FRAMEBUFFER_LENGTH - 16;


uint8_t serial_sendTxByte(uint8_t txByte)
{
	return(0);
}

int16_t serial_rxByte(void)
{
	return('?');
}

void test_orp_init_api(void)
{
	octave_init(
		serial_sendTxByte, //! bind the UART send function
		serial_rxByte,   //! bind the UART get function
		HDLC_frameBuffer,        //! user passes in a buffer
		HDLC_frameBuffer_length,       //! user passes in buffer length 
		HDLC_maxFrameLength            //! user passes in max frame length (TBD)
	);

}

// ----------------------------------------------------------------------------
// test the orp input system
// dummy callback handlers
void adc_json_cbh(char *response)
{

}

void outputsState_json_cbh(char *response)
{

}

// in the app create an array of structs
#define NUMBER_OF_INPUTS 2

void test_orp_input_api(void)
{
	int8_t numberOfInputs = NUMBER_OF_INPUTS;
	
	// ! This will be a (static )global in a real user app
	orp_input_struct orpInputs_struct[NUMBER_OF_INPUTS+1];

	orpInputs_struct[0].keyString = "temperature/json";
	orpInputs_struct[0].userApp_OctaveResponse_cbh = adc_json_cbh;

	orpInputs_struct[1].keyString = "adcarray/json";
	orpInputs_struct[1].userApp_OctaveResponse_cbh = outputsState_json_cbh;

	orp_input_s_register( orpInputs_struct, numberOfInputs);	

	//  wait for input registration to complete TBD

	orp_input_send(orpInputs_struct, 0, "\"jsonKey_test\":\"26.8\"");
	orp_input_send(orpInputs_struct, 0, "\"adcArray\":[100,101,102,103,104,105,106,107]"); // not certain this will work
	
}



// ----------------------------------------------------------------------------
// Test the Octave outputs --> Remote micro

#define NUMBER_OF_OUTPUTS 2
#define OUTPUT_VALUE_STRING_SIZE	32

// Output registration call back handler
// this is called when the Octave Edge device response over ORP
void orp_register_outputResponse_cbh(char *stringRes)
{

}

// Output data callback event handler
void orp_output_unsolicitedData_cbh(uint8_t idx)
{

}

void test_orp_output_api(void)
{
	int8_t numberOfOu = NUMBER_OF_OUTPUTS;
	char output1Value[OUTPUT_VALUE_STRING_SIZE];
		
	// ! This will be a (static )global in a real user app
	orp_output_struct orp_output_structs[NUMBER_OF_OUTPUTS + 1];

	orp_output_structs[0].keyString="motorspeed/json"; // need to check this is valid
	orp_output_structs[0].userApp_OctaveRegistrationResponse_cbh = orp_register_outputResponse_cbh;
	orp_output_structs[0].userApp_OctaveUnsolicitedResponse_cbh = orp_output_unsolicitedData_cbh;
}


// ----------------------------------------------------------------------------
// run the tests
int main()
{
	printf("Start api test\r\n");
	test_orp_init_api();
	test_orp_input_api();
	test_orp_output_api();
    printf("Done api test\r\n");
}
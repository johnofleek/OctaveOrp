#include <stdint.h>
#include <stdio.h>  // just for printf debug
#include "orp.h"





// The c app

// This is the real user application
// #include "orp.h"

// in the app create an array of structs
#define NUMBER_OF_INPUTS 5


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


void test_orp_input_api(void)
{
	int8_t numberOfInputs = 2;
	
	orp_input_struct octaveInputs_struct[NUMBER_OF_INPUTS];

	octaveInputs_struct[0].keyString = "adc/json";
	octaveInputs_struct[0].userApp_OctaveResponse_cbh = adc_json_cbh;

	octaveInputs_struct[1].keyString = "opstate/json";
	octaveInputs_struct[1].userApp_OctaveResponse_cbh = outputsState_json_cbh;

	orp_input_s_register( octaveInputs_struct, numberOfInputs);	

	//  wait for input registration to complete TBD

	orp_input_send(octaveInputs_struct, 0, "\"jsonKey_test\":\"26.8\"");
	orp_input_send(octaveInputs_struct, 0, "\"adcArray\":[100,101,102,103,104,105,106,107]"); // not certain this will work
	
}

// ----------------------------------------------------------------------------
// run the tests
int main()
{
	test_orp_init_api();
	test_orp_input_api();
    printf("Done\r\n");
}
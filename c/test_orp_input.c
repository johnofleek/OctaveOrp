

#include <stdint.h>
#include <stdio.h>  // just for printf debug
#include <string.h>

// Pltaform specific
#include <unistd.h>


// project specific
#include "orp.h"
#include "hdlc.h"


// The c app

// This is the real user application
// #include "orp.h"



// ----------------------------------------------------------------------------
// test the orp initialisation functionality

// Created rx resource for HDLC receiver 
// this also also holds the orp protocol call back payload data 
#define HDLC_RX_BUFFER_LENGTH 1024
static uint8_t hdlc_rxBuffer[HDLC_RX_BUFFER_LENGTH];

// ----------------------------------------------------------------------------
// HDLC setup
static void serial_txByte(uint8_t txByte)
{
	putchar(txByte);
}

// bool hdlcframeDecoded_flag = false;

static void hdlcDecoded_cbh(const uint8_t *framebuffer, uint16_t framelength)
{
	uint8_t ctr = 0;

	printf("RX RAW ORP <");
	for(ctr = 0; ctr < framelength; ctr++)
	{
		printf("%c", framebuffer[ctr]);
	}
	printf(">\r\n");

	// hdlcframeDecoded_flag = true;
}


// ----------------------------------------------------------------------------
// Orp protocol setup

#define ORP_PROTOCOL_TX_BUFFER_LENGTH 1024
static uint8_t orpProtocol_txBuffer[ORP_PROTOCOL_TX_BUFFER_LENGTH];

static void init_hdlc(void)
{

	hdlc_hdlc 
	(    
  		serial_txByte,              // encoder - bind send a byte to UART
  		hdlcDecoded_cbh, 			// decoder - called with payload from decoded HDLC rx frame  
  		hdlc_rxBuffer,              // decoder - working buffer
  		HDLC_RX_BUFFER_LENGTH      	// decoder - length of working buffer
	);

}



static void init_orp_protocol(void)
{
	orp_protocol(orpProtocol_txBuffer, ORP_PROTOCOL_TX_BUFFER_LENGTH);
}

// ----------------------------------------------------------------------------
// Test orp_input 
static void adc_json_cbh(char *response)
{
	printf("jsonCbh <%s>", response);
}


// in the app create an array of structs
#define NUMBER_OF_INPUTS 2


static orp_input_struct orpInput_struct_temperature;


// plaform specific sleep
void sleep100ms(void)
{
	usleep(100000);
}

static void test_orp_input(void)
{
	orpInput_struct_temperature.keyString = "temperature/json";
	orpInput_struct_temperature.userApp_OctaveResponse_cbh = adc_json_cbh;

	orp_protocol_wakeup(serial_txByte, sleep100ms);

	orp_input_registerJson( orpInput_struct_temperature);	

	//  wait for input registration to complete TBD

	orp_input_sendJson(orpInput_struct_temperature.keyString, "\"jsonKey\":\"26.8\"");
	
}


// ----------------------------------------------------------------------------
// run the tests
int main()
{
	printf("\n\rorp_input - Start test\r\n");

	init_hdlc();
	

	test_orp_input();

    printf("\n\rDone orp_input test\r\n");
}


#include <stdint.h>
#include <stdio.h>  // just for printf debug
#include <string.h>

// Pltaform specific
#include <unistd.h>


// project specific
#include "orp.h"


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
	// putchar(txByte);
	TRACE(("0x%X,",txByte));
}

// bool hdlcframeDecoded_flag = false;

static void hdlcDecoded_cbh(const uint8_t *framebuffer, uint16_t framelength)
{
	uint8_t ctr = 0;

	TRACE(("RX RAW ORP <"));
	for(ctr = 0; ctr < framelength; ctr++)
	{
		printf("%c", framebuffer[ctr]);
	}
	TRACE((">\r\n"));

	// hdlcframeDecoded_flag = true;
}


// ----------------------------------------------------------------------------
// Orp protocol setup

#define ORP_PROTOCOL_TX_BUFFER_LENGTH 1024
static uint8_t orpProtocol_txBuffer[ORP_PROTOCOL_TX_BUFFER_LENGTH];




static void init_orp_protocol(void)
{

	orp_protocol(
        orpProtocol_txBuffer,       // encoder input payload 
        ORP_PROTOCOL_TX_BUFFER_LENGTH,       // encoder input payload size

       	serial_txByte,         		// encoder - bind TX a byte to UART
                                            
        hdlcDecoded_cbh, 			// decoder - frame decoded callback with payload 
        hdlc_rxBuffer,            	// decoder - working buffer
        HDLC_RX_BUFFER_LENGTH       // decoder - length of working buffer
	);


	// note the serial_rxByte() needs handling by the app feeding it data from the serial port

}

// ----------------------------------------------------------------------------
// Test orp_input 
static bool hdlcframeDecoded_flag = false;

void adc_json_cbh(const uint8_t *buffer, uint16_t bufferLength)
{
	TRACE(("\n\rjsonCbh <%c%c>\n\r", buffer[0],buffer[1]));
	hdlcframeDecoded_flag = true;
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

	orp_protocol_wakeup( sleep100ms);
	TRACE(("\n\r"));

	orp_input_registerJson( orpInput_struct_temperature);	
	TRACE(("\n\r"));
	//  wait for input registration to complete TBD

	orp_input_sendJson(orpInput_struct_temperature, "\"jsonKey\":\"26.8\"");
	TRACE(("\n\r"));

	// example orp request responses
	// p@01 -> ~p@01° ~ -> 0x7E,0x70,0x40,0x30,0x31,0xF8,0xFF,0x7E
	// i@01 -> ~i@01/~  -> 0x7E,0x69,0x40,0x30,0x31,0x10,0x2F,0x7E

	// feed in a test response
	uint8_t rxBytes[256] = {0x7E,0x70,0x40,0x30,0x31,0xF8,0xFF,0x7E};
	uint8_t ctr = 0;
	

	hdlc_frameEncode(rxBytes,strlen(rxBytes));
	TRACE(("\n\r"));

	while(hdlcframeDecoded_flag == false)
	{
		//gets(rxBytes); // dodgy
		
		printf("IN %s\n\r", rxBytes);

		for(ctr = 0; ctr < strlen(rxBytes); ctr++)
		{
			orp_protocol_processHdlcRx(rxBytes[ctr]);
		}
		
		// wait for decoded callback to fire
	}
	
}


// ----------------------------------------------------------------------------
// run the tests
int main()
{
	TRACE(("\n\rorp_input - Start test\r\n"));

	
	
	init_orp_protocol();

	test_orp_input();

    TRACE(("\n\rDone orp_input test\r\n"));
}
/*
Copyright <2020> <J Thompson>
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/ 

#include <stdint.h>
#include <stdio.h>  // just for printf debug
#include <string.h>

// Pltaform specific
#include <unistd.h>


// project specific
#include "orp.h"


// The c app
// This is an example user application




// ----------------------------------------------------------------------------
// test the orp initialisation functionality


// ----------------------------------------------------------------------------
// Orp protocol setup

static void serial_txByte(uint8_t txByte)
{
	putchar(txByte);
	TRACE((" 0x%X,",txByte));
}


// Created rx resource for HDLC receiver 
// this also also holds the orp protocol call back payload data 
#define HDLC_RX_BUFFER_LENGTH 1024
static uint8_t hdlc_rxBuffer[HDLC_RX_BUFFER_LENGTH];

#define ORP_PROTOCOL_TX_BUFFER_LENGTH 1024
static char orpProtocol_txBuffer[ORP_PROTOCOL_TX_BUFFER_LENGTH];




static void app_requestResponse_cbh(const uint8_t *buffer, uint16_t bufferLength)
{
	TRACE(("\n\rRequest response Cbh <%c%c%c%c> len %d\n\r\n\r", buffer[0],buffer[1],buffer[2],buffer[3], bufferLength));
}



static void app_notification_cbh(const uint8_t *buffer, uint16_t bufferLength)
{
	TRACE(("\n\rNotification Cbh <%c%c%c%c> len %d\n\r\n\r", buffer[0],buffer[1],buffer[2],buffer[3], bufferLength));
	
	uint16_t ctr;

	// decode()
	for (ctr = 0; ctr < bufferLength; ctr++)
	{
		if((ctr==3) || (buffer[ctr] == ','))
		{
			ctr++;
			switch(buffer[ctr])
			{
				case 'P':
					TRACE(("Path "));
					break;
				case 'T':
					TRACE(("Timestamp "));
					break;
				case 'D':
					TRACE(("Data "));
					break;
				default:
					TRACE(("Unknown "));
					break;

			}
			ctr++;
			for(; ctr < bufferLength ; ctr++ )
			{
				TRACE(("%c",buffer[ctr]));
				if((buffer[ctr +1]) == ',')
				{
					TRACE(("\n"));
					break;
				}
			}
		}
	}
}

static void init_orp_protocol(void)
{

	orp_protocol(
        orpProtocol_txBuffer,       // encoder input payload 
        ORP_PROTOCOL_TX_BUFFER_LENGTH,       // encoder input payload size

       	serial_txByte,         		// encoder - bind TX a byte to UART                                      

        hdlc_rxBuffer,            	// decoder - working buffer
        HDLC_RX_BUFFER_LENGTH,      // decoder - length of working buffer

		app_requestResponse_cbh,	// decoder request responses from Octave 
		app_notification_cbh		// decoder notifications from Octave
	);


	// Note 
	// The serial_rxByte() needs handling by the app feeding it data from the
	// serial port
	// usually this will be an UART interrupt handler
	// in this case we are testing by using predefined data 

}

// ----------------------------------------------------------------------------
// Test orp_input 





// plaform specific sleep
void sleep100ms(void)
{
	usleep(100000);
}

// Warning - This only works if the checksum doesn't containn '\0' which could happen
static void simulateRxData(uint8_t * rxdata)
{
	uint8_t ctr = 0;
	
	TRACE(("\n\rRX raw "));
	for(ctr = 0; ctr < strlen((char *) rxdata); ctr++)
	{
		TRACE(("%c", rxdata[ctr]));
		orp_protocol_processHdlcRx(rxdata[ctr]);
		if((ctr > 0) && (rxdata[ctr]) == '~')
		{
			TRACE(("\n\r"));
			return;
		}
	}
		
		// wait for decoded callback to fire
}

	// example orp request responses
	// push to resource response
	// p@01 -> ~p@01° ~ --> 0x7E,0x70,0x40,0x30,0x31,0xF8,0xFF,0x7E
	// create resource response
	// i@01 -> ~i@01/~  --> 0x7E,0x69,0x40,0x30,0x31,0x10,0x2F,0x7E

	// c@01 -> ~c@01xä~ --> 0x7E,0x63,0x40,0x30,0x31,0x78,0x84,0x7E
	// c@01T1585927713.843660,Pval/od/value,D456.000000
	// b@01 -> ~b@010~  --> 0x7E,0x62,0x40,0x30,0x31,0xE,0x30,0x7E

	// AddPushHandler request response
	// h@01 --> 0x7E,0x68,0x40,0x30,0x31,0x66,0x9B,0x7E

	// push handler - notification from Octave
	// "c@01T1585927713.843660,Pval/od/value,D456.000000" ->
	// ~c@01T1585927713.843660,Pval/od/value,D456.000000Se~ -->
	// 0x7E,0x63,0x40,0x30,0x31,0x54,0x31,0x35,0x38,0x35,0x39,0x32,0x37,0x37,0x31,0x33,0x2E,0x38,0x34,0x33,0x36,0x36,0x30,0x2C,0x50,0x76,0x61,0x6C,0x2F,0x6F,0x64,0x2F,0x76,0x61,0x6C,0x75,0x65,0x2C,0x44,0x34,0x35,0x36,0x2E,0x30,0x30,0x30,0x30,0x30,0x30,0x53,0x65,0x7E
	// feed in a test response


static void test_orp_input(void)
{
	char keyString[] = "temperature/json";
	
	orp_protocol_wakeup( sleep100ms);

	TRACE(("\n\rorp_input_registerJson\n\r"));
	orp_input_registerJson( keyString);	
	uint8_t createResourceResponse[] = { 0x7E,0x69,0x40,0x30,0x31,0x10,0x2F,0x7E};
	simulateRxData(createResourceResponse);	

	TRACE(("orp_input_sendJson\n\r"));
	orp_input_sendJson(keyString, "\"jsonKey\":\"26.8\"");
	uint8_t pushToResourceResponse[] = { 0x7E,0x70,0x40,0x30,0x31,0xF8,0xFF,0x7E};
	simulateRxData(pushToResourceResponse);


	TRACE(("orp_output_create_num test\n\r"));
	char opKeyString[] = "motorspd/num";
	orp_output_create_num(opKeyString);
	uint8_t orp_output_create_num_ResourceResponse[] =  { 0x7E,0x6F,0x40,0x30,0x31,0x37,0xB6,0x7E};
	simulateRxData(orp_output_create_num_ResourceResponse);

	TRACE(("orp_output_registerCallback_num test\n\r"));
	orp_output_registerCallback_num(opKeyString);
	uint8_t orp_output_registerCallback_num_ResourceResponse[] = { 0x7E,0x68,0x40,0x30,0x31,0x66,0x9B,0x7E};
	simulateRxData(orp_output_registerCallback_num_ResourceResponse);

	TRACE(("\n\rNotification test - Orp Output \n\r"));
	uint8_t notificationTest[] = { 0x7E,0x63,0x40,0x30,0x31,0x54,0x31,0x35,0x38,0x35,0x39,0x32,0x37,0x37,0x31,0x33,0x2E,0x38,0x34,0x33,0x36,0x36,0x30,0x2C,0x50,0x76,0x61,0x6C,0x2F,0x6F,0x64,0x2F,0x76,0x61,0x6C,0x75,0x65,0x2C,0x44,0x34,0x35,0x36,0x2E,0x30,0x30,0x30,0x30,0x30,0x30,0x53,0x65,0x7E, 0};
	simulateRxData(notificationTest);

	// Just to build some test data - this is not a normal thing to do
	//char txBytes[256] = "o@01";
	// hdlc_frameEncode((uint8_t *) txBytes,strlen( txBytes));

	
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


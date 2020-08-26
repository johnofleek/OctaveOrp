/*
Copyright <2020> <J THOMPSON>
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 */

/*
 * Added support for SYNC packets
 * The edge device will send sync packets following a reset
 * This enables the example to detect if the ORP edge device resets 
 * and the app can then restart the statemachine so that ORP edge 
 * datapoints are recreated
 * Remember that ORP Edge datapoints are volatile (I think they all are except for virtual/config)
 */

#include <crcccitt.h>
#include <debug.h>
#include <hdlc.h>
#include <orp.h>
#include <orp_input.h>
#include <orp_output.h>
#include <orp_protocol.h>




#include <SimpleTimer.h>  // polled timer slightly better than delay()

#define TIME_CALL_MS 1000
SimpleTimer firstTimer(TIME_CALL_MS);

SimpleTimer secondTimer((uint64_t)TIME_CALL_MS * (uint64_t)(5*60)); // measurement data orp rate


// ----------------------------------------------------------------------------
// called by HDLC layer to create a 100ms delay
// ----------------------------------------------------------------------------
// plaform specific sleep
// if using a different micro you need to changes this
// At some point in time this needs modifying because it blocks
void sleep100ms(void)
{
  delay(100); 
}



// static int tempPin = 1;        //the analog pin the TMP36's Vout (sense) pin is connected to

// ----------------------------------------------------------------------------
// Arduino serial port interface code 
// ----------------------------------------------------------------------------

/* Function to send out one 8bit character */
static void serial_txByte(uint8_t txByte)
{
    Serial2.print((char)txByte);
    Serial.print((char) txByte);
}

/*
SerialEvent occurs whenever a new data comes in the hardware serial port 2
RX.  
This routine is run between each time loop() runs, so using delay inside 
loop can delay response.  Multiple bytes of data may be available.

Damn I hadn't realised that this won't run if stuck in loop()
*/
void serialEvent2() 
{
    while (Serial2.available()) 
    {
        // get the new byte:
        int rxByte = Serial2.read();
          
        Serial.print((char) rxByte);
        
        // Pass all incoming data to hdlc char receiver
        // when a valid hdlc frame has been decoded an app callback function
        //  is called
        orp_protocol_processHdlcRx(rxByte);
    }
}

// ----------------------------------------------------------------------------
// orp protocol setup
// ----------------------------------------------------------------------------

static bool orp_requestAck  = false; // 
static bool orp_rebooted = true;

// process Octave request acks
static void app_requestResponse_cbh(const uint8_t *buffer, uint16_t bufferLength)
{
  char packetType = (char)buffer[0];

  // TRACE(("\n\rjsonCbh <%c%c%c%c> len %d\n\r", buffer[0],buffer[1],buffer[2],buffer[3], bufferLength));
  Serial.println();
  Serial.print("Response cb ");
  Serial.print("TYPE(");
  Serial.print(packetType);       
  Serial.print(") "); 

  Serial.print(" STATUS(");
  Serial.print(buffer[1] - 64);  // Status code
  Serial.print(") "); 
  
  Serial.print((char)buffer[2]);  // context defines use of fields - see the ORP manual
  Serial.print((char)buffer[3]);
  
  Serial.print(" Len(");
  Serial.print(bufferLength); 
  Serial.println(") ");   

  if(packetType == SBR_PKT_RESP_SYNC_RQST)
  {
    // the WP has rebooted
    // we need some code here to restart the state machine
    orp_rebooted = true;
  }
  else
  {
    orp_requestAck = true; // very crude fix this later // this indicates some sort of response has occcured
  }
}

// process Octave notifications
static void app_notification_cbh(const uint8_t *buffer, uint16_t bufferLength)
{
  char packetType = (char)buffer[0];
  // TRACE(("\n\rNotification Cbh <%c%c%c%c> len %d\n\r\n\r", buffer[0],buffer[1],buffer[2],buffer[3], bufferLength));
  Serial.println();
  Serial.print("Notify cb   ");

  Serial.print("TYPE(");
  Serial.print(packetType); // Message type
  
  Serial.print(") STATUS(");
  Serial.print(buffer[1] - 64);  // Status code
  Serial.print(") "); 
  Serial.print((char)buffer[2]); 
  Serial.println((char)buffer[3]);  
 
  
  uint16_t ctr;
  uint16_t currentDataTypePos = 0;

  // decode()
  for (ctr = 0; ctr < bufferLength; ctr++)
  {
    if((ctr==3) || (buffer[ctr] == ','))
    {
      ctr++;
      currentDataTypePos = ctr;
      switch(buffer[currentDataTypePos])
      {
        case 'P':
          Serial.print(("Path "));
          break;
        case 'T':
          Serial.print(("Timestamp "));
          break;
        case 'D':
          Serial.print(("Data "));
          break;
        default:
          Serial.print(("Unknown "));
          break;

      }
      ctr++;
      for(; ctr < bufferLength ; ctr++ )
      {
        // TRACE(("%c",buffer[ctr]));
        Serial.print(( char) buffer[ctr]);
        // Put the message onto the Arduino LCD
        if((ctr - (currentDataTypePos+1)) < 80)
        {
          if((buffer[currentDataTypePos] == 'P') || (buffer[currentDataTypePos] == 'D'))
          {
          }
        }
        if(( (char) buffer[ctr +1]) == ',')
        {
          Serial.println("");
          break;
        }
      }
    }
  }
  Serial.println("");
}


#define ORP_PROTOCOL_TX_BUFFER_LENGTH 128
static char orpProtocol_txBuffer[ORP_PROTOCOL_TX_BUFFER_LENGTH];

#define HDLC_RX_BUFFER_LENGTH 256
static uint8_t hdlc_rxBuffer[HDLC_RX_BUFFER_LENGTH];

static uint16_t     crc_tabccitt[CCITT_TABLE_SIZE];   // size is fixed by crc calculator


// This version passes in the crc table 
static void init_orp_protocol(void)
{
  orp_protocol(
    orpProtocol_txBuffer,           // encoder input payload 
    ORP_PROTOCOL_TX_BUFFER_LENGTH,  // encoder input payload size

    serial_txByte,                  // encoder - bind TX a byte to UART                                      

    hdlc_rxBuffer,                  // decoder - working buffer
    HDLC_RX_BUFFER_LENGTH,          // decoder - length of working buffer

    crc_tabccitt,                   // size is fixed by crc calculator
    CCITT_TABLE_SIZE,     

    app_requestResponse_cbh,        // decoder request responses from Octave 
    app_notification_cbh            // decoder notifications from Octave
  );
  // note the serial_rxByte() needs handling by the app feeding it data from the serial port
}

// 12 bit
static char valAsString[] =  "[0000,0001,0002,0003,0004,0005,0006,0007,0008] ";

static void adcsAsJsonString(void)
{
  snprintf(valAsString, sizeof(valAsString),        // need to add check
          "{\"ad\":[%u,%u,%u,%u,%u,%u,%u,%u]}",
          analogRead(0), 
          analogRead(1), 
          analogRead(2), 
          analogRead(3), 
          analogRead(4), 
          analogRead(5), 
          analogRead(6), 
          analogRead(7) 
  );

  Serial.println(valAsString);
}



// ----------------------------------------------------------------------------
// Standard arduino setup
// ----------------------------------------------------------------------------

void setup() { 
  // Initialize serial port to 9600 baud
  Serial2.begin(9600);
  Serial.begin(115200);
  Serial.println("Setup Start");
  
  pinMode(16, OUTPUT); // Serial port TX to output
  pinMode(17, INPUT);
  digitalWrite(17, LOW); // disable internal pullup

  init_orp_protocol();

  adcsAsJsonString();
 
  Serial.println("setup done");
}




static char ipKeyString[] = "adcs";
static char opKeyString[] = "motorspd/num";
static char opKeyString2[] = "txt_str";

void sendAdcValues(void)
{
  Serial.println("Send data to json input");
  adcsAsJsonString();
  orp_protocol_wakeup( sleep100ms);
  orp_input_sendJson(ipKeyString, valAsString);
}

// ----------------------------------------------------------------------------
// core state machine loop
// ----------------------------------------------------------------------------



void loop() 
{
  static uint8_t state = 0;

  if(orp_rebooted) // flag set by SYNC message received
  {
    state = 0;
    orp_rebooted = false;
    orp_requestAck = false;
  }
  
  if (firstTimer.isReady())
  {       
    firstTimer.reset();
    
    switch(state)
    {
      case 0:
        if(orp_requestAck)
        {
          orp_requestAck = false; 
          state = 1;           
        }
        else
        {
          Serial.println("Create json input in the Octave edge");
          orp_protocol_wakeup( sleep100ms);     
          orp_input_registerJson(ipKeyString);        
        }

        break;
      case 1:
        // Create a number resource in the Octave edge
        if(orp_requestAck)
        {
          orp_requestAck = false; 
          state = 2;          
        } 
        else
        {
          Serial.println("Create a numnber resource in the Octave edge ");
          orp_protocol_wakeup( sleep100ms);  
          orp_output_create_num(opKeyString); orp_requestAck = false; 
        }
        break;
      case 2:
          // Activate a callback from Octave edge output to micro
          if(orp_requestAck)
          {
            orp_requestAck = false;
            state = 3;       
          } 
          else
          {
            Serial.println("Activate a number callback from Octave edge output to micro");
            orp_protocol_wakeup( sleep100ms);
            orp_output_register_num(opKeyString);  orp_requestAck = false;
          }
          break;
       case 3:
        // Create a number resource in the Octave edge
        if(orp_requestAck)
        {
          orp_requestAck = false; 
          state = 4;          
        } 
        else
        {
          Serial.println("Create a string resource in the Octave edge ");
          orp_protocol_wakeup( sleep100ms);  
          orp_output_create_string(opKeyString2); orp_requestAck = false; 
        }
        break;
      case 4:
          // Activate a callback from Octave edge output to micro
          if(orp_requestAck)
          {
            orp_requestAck = false;
            sendAdcValues();  // workaround potential long delay before first vals are sent 
            state = 5;       
          } 
          else
          {
            Serial.println("Activate a string callback from Octave edge output to micro");
            orp_protocol_wakeup( sleep100ms);
            orp_output_register_string(opKeyString2);  orp_requestAck = false;
          }
          break;
      case 5:
        if(secondTimer.isReady()) // send data to server
        {
          secondTimer.reset();
          sendAdcValues();
        }
        break;
      default:
        state = 0;  
    }
  }
  else
  {

  }
}

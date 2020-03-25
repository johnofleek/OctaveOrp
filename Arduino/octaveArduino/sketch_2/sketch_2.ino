/* 
 *  For spec on ORP
 *  https://docs.octave.dev/references/edge/octave_resource_protocol/#orp-api
 */



#include <avr/pgmspace.h>
#include <math.h>

#include <Arduhdlc.h>
#include <orp.h>
#include <SimpleTimer.h>  // polled timer slightly better than delay()

#define TIME_CALL_MS 1000
SimpleTimer firstTimer(TIME_CALL_MS);

SimpleTimer secondTimer((uint64_t)TIME_CALL_MS * (uint64_t)(30)); // measurement data orp rate

#define MAX_HDLC_FRAME_LENGTH   128
#define MAX_ORP_RX_FRAME_LENGTH 128
#define MAX_ORP_TX_FRAME_LENGTH 128

/* Function to send out byte/char */
static void send_character(uint8_t data);

/* Function to handle a valid HDLC frame */
static void hdlc_frame_handler(const uint8_t *data, uint16_t length);

/* Initialize Arduhdlc library with three parameters.
1. Character send function, to send out HDLC frame one byte at a time.
2. HDLC frame handler function for received frame.
3. Length of the longest frame used, to allocate buffer in memory */
Arduhdlc hdlc(&send_character, &hdlc_frame_handler, MAX_HDLC_FRAME_LENGTH);

static char orp_responseFrame[MAX_ORP_RX_FRAME_LENGTH] = ""; 
static char orp_rxPayload[MAX_ORP_RX_FRAME_LENGTH] = "";
static char orp_txPayload[MAX_ORP_TX_FRAME_LENGTH] = "";

orp orpO(orp_txPayload, sizeof(orp_txPayload));

/* Function to send out one 8bit character */
static void send_character(uint8_t data) {
    Serial2.print((char)data);
}




/* Frame handler function. What to do with received data? */
static void hdlc_frame_handler(const uint8_t *data, uint16_t length) {
    // Do something with data that is in framebuffer
    Serial.println("Frame");

    if (
        ( data[0] == SBR_PKT_RESP_INPUT_CREATE  ) ||
        ( data[0] == SBR_PKT_RESP_OUTPUT_CREATE ) ||
        ( data[0] == SBR_PKT_RESP_HANDLER_ADD   ) ||
        ( data[0] == SBR_PKT_RESP_HANDLER_REMOVE)
    )
    {
      memcpy(orp_responseFrame, data , sizeof(orp_responseFrame));
    }
    else
    {
      memcpy(orp_rxPayload, data , sizeof(orp_rxPayload));
    }

    // debug the raw frame data
    for(uint16_t i = 0; i < length; i++)
    {
        Serial.print((char)orp_responseFrame[i]);
        Serial.print("[");
        Serial.print(orp_responseFrame[i],HEX);
        Serial.print("] ");
    }  
    Serial.println(" ");
    
}


static void debugPayload(void)
{
  Serial.print("Payload "); Serial.print(orp_txPayload); Serial.print(" Len ");Serial.println(strlen(orp_txPayload));
}

static void wakeup(void)
{
    send_character('~');
    delay(100);
    send_character('~');
    delay(100);
    send_character('~');
}

// better object method is needed than this
static void octave_registerInput(void)
{
    int16_t retVal;
    retVal = orpO.createResource(SBR_PKT_RQST_INPUT_CREATE, SBR_DATA_TYPE_NUMERIC, "val/adc1", "num"  );
    Serial.print("orp encode "); Serial.println(retVal);
    debugPayload();
    wakeup(); 
    hdlc.frameEncode((uint8_t *) orp_txPayload, strlen(orp_txPayload));
}

// better object method is needed than this
static void octave_registerOutput(void)
{
    int16_t retVal;
    retVal = orpO.createResource(SBR_PKT_RQST_OUTPUT_CREATE, SBR_DATA_TYPE_NUMERIC, "val/od", "num"  );
    Serial.print("orp encode "); Serial.println(retVal);
    debugPayload();
    wakeup(); 
    hdlc.frameEncode((uint8_t *) orp_txPayload, strlen(orp_txPayload));
}


static void octave_registerOutputCallback(void)
{
    int16_t retVal;
    retVal = orpO.addpushHandler(SBR_DATA_TYPE_NUMERIC, "val/od");
    Serial.print("orp encode "); Serial.println(retVal);
    debugPayload();
    wakeup(); 
    hdlc.frameEncode((uint8_t *) orp_txPayload, strlen(orp_txPayload));
}

static int tempPin = 1;        //the analog pin the TMP36's Vout (sense) pin is connected to
static char valAsString[32] = ""; 

// read the sensor and send the data to Octave
static void octave_sendInput(void)
{
    int16_t retVal;
    
    // read the sensor
    uint16_t adc_1 = analogRead(tempPin);   
    
    // converting that reading to voltage, which is based off the reference voltage
    // float voltage = tempReading * aref_voltage;
    // voltage /= 1024.0; 
    
    // print out the voltage
    Serial.print("ADC "); Serial.print(tempPin); Serial.print(" ");
    Serial.println(adc_1);

    snprintf(valAsString, sizeof(valAsString),        // need to add check
            "%u",
            adc_1 
    );
        
    // send the sensor data to octave via orp
    retVal = orpO.pushValue(SBR_DATA_TYPE_NUMERIC, "val/adc1" , valAsString );
    Serial.print("orp encode "); Serial.println(retVal);
    debugPayload();
    wakeup();
    hdlc.frameEncode((uint8_t *) orp_txPayload, strlen(orp_txPayload));
}



/*
SerialEvent occurs whenever a new data comes in the
hardware serial RX.  This routine is run between each
time loop() runs, so using delay inside loop can delay
response.  Multiple bytes of data may be available.
Damn I hadn't realised that this won't run if stuck in loop
*/
void serialEvent2() {
    while (Serial2.available()) 
    {
        // get the new byte:
        int inChar = Serial2.read();
        // Pass all incoming data to hdlc char receiver
        hdlc.frameDecode_char((uint8_t )inChar);
    }
}


/*
 * standard arduino stuff
 */

void setup() { 
  pinMode(16, OUTPUT); // Serial port TX to output
  pinMode(17, INPUT);
  digitalWrite(17, LOW); // disable internal pullup

  // need to work out what happens by default with the adc pins
  //pinMode(A1, INPUT);
  //digitalWrite(A1, INPUT_PULLUP);
  //digitalWrite(A1, LOW);  
  
  // initialize serial port to 9600 baud
  Serial2.begin(9600);
  Serial.begin(115200);
  Serial.println("Init");

  // To reduce noise - connect the 3V3 source to AREF - gives 0 to 3V3 span
  analogReference(EXTERNAL);
  
  Serial.println("Init done");
}

// core state machine
void loop() 
{
  static uint8_t state = 0;

  // process orp data responses
  
  if      (orp_rxPayload[0] == SBR_PKT_NTFY_HANDLER_CALL)   // very basic response check - needs work
  {
    Serial.println("Octave data Received");
    for(uint16_t i = 4; i < 10; i++)
    {
        Serial.print((char)orp_rxPayload[i]);
    }
    Serial.println("");
    orp_rxPayload[0] = '\0'; 
  }

  // process orp responses
  if      (orp_responseFrame[0] == SBR_PKT_RESP_INPUT_CREATE)   // very basic response check - needs work
  {
    state = 1;
    orp_responseFrame[0] = '\0';    // clear frame data 
    octave_sendInput(); // try and send first data set
  }
  else if (orp_responseFrame[0] == SBR_PKT_RESP_OUTPUT_CREATE)   // very basic response check - needs work
  {
    state = 2;
    orp_responseFrame[0] = '\0';    // clear frame data   
  }
  else if (orp_responseFrame[0] == SBR_PKT_RESP_HANDLER_ADD)   // very basic response check - needs work
  {
    state = 3;
    orp_responseFrame[0] = '\0';    // clear frame data   
  }

  if (firstTimer.isReady())
  {
    firstTimer.reset();
    
    switch(state)
    {
      case 0:
        octave_registerInput();
        break;
      case 1:
        octave_registerOutput();
        break;
      case 2:
        octave_registerOutputCallback();
        break;
      case 3:
        if(secondTimer.isReady()) // send data to server
        {
          secondTimer.reset();
          octave_sendInput();
        }
        break;
      default:
        state = 0;  
    }
  }
}

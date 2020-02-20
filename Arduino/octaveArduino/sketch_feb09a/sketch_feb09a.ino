/*
 * Working a bit better but I'm having some issues with my level shifter
 * Hence the commented out frame waiting code
 */


#include <Arduhdlc.h>

#include <orp.h>

#define MAX_HDLC_FRAME_LENGTH 64
/* Function to send out byte/char */
void send_character(uint8_t data);

/* Function to handle a valid HDLC frame */
void hdlc_frame_handler(const uint8_t *data, uint16_t length);

/* Initialize Arduhdlc library with three parameters.
1. Character send function, to send out HDLC frame one byte at a time.
2. HDLC frame handler function for received frame.
3. Length of the longest frame used, to allocate buffer in memory */
Arduhdlc hdlc(&send_character, &hdlc_frame_handler, MAX_HDLC_FRAME_LENGTH);

char payload[128] = "hello";

orp orpO(payload, sizeof(payload));

/* Function to send out one 8bit character */
void send_character(uint8_t data) {
    Serial2.print((char)data);
}

// quick bodge to handle response frame
bool respFrame = false;

/* Frame handler function. What to do with received data? */
void hdlc_frame_handler(const uint8_t *data, uint16_t length) {
    // Do something with data that is in framebuffer
    Serial.println("Frame");
    for(uint16_t i = 0; i < length; i++)
    {
        Serial.print((char)data[i]);
        Serial.print("[");
        Serial.print(data[i],HEX);
        Serial.print("] ");
    }  
    Serial.println(" ");
    respFrame = true;
}


void debugPayload(void)
{
  Serial.print("Payload "); Serial.print(payload); Serial.print(" Len ");Serial.println(strlen(payload));
}

void wakeup(void)
{
    send_character('~');
    delay(100);
    send_character('~');
    delay(100);
    send_character('~');
}

void setup() { 
    pinMode(16, OUTPUT); // Serial port TX to output
    pinMode(17, INPUT);
    digitalWrite(17, LOW); // disable internal pullup
    
    // initialize serial port to 9600 baud
    Serial2.begin(9600);
    Serial.begin(115200);
    Serial.println("Init");
    // checkSend();
    

    int16_t retVal;
    // while(respFrame == false)
    {
      retVal = orpO.createResource(SBR_PKT_RQST_INPUT_CREATE, SBR_DATA_TYPE_NUMERIC, "val/te", "num"  );
      Serial.println(retVal);
      debugPayload();
      wakeup(); 
      hdlc.frameEncode((uint8_t *) payload, strlen(payload));
      delay(500);
    }
    respFrame = false;
  
    Serial.println("Init done");
}


void loop() {
    delay(10000);
    // while(respFrame == false)
    {
      orpO.pushValue(SBR_DATA_TYPE_NUMERIC, "val/te" , "25.2" );
      debugPayload();
      wakeup();
      hdlc.frameEncode((uint8_t *) payload, strlen(payload));
      delay(2000);
    }
    respFrame = false;
}



/*
SerialEvent occurs whenever a new data comes in the
hardware serial RX.  This routine is run between each
time loop() runs, so using delay inside loop can delay
response.  Multiple bytes of data may be available.
*/
void serialEvent2() {
    //Serial.println("Sta");
    while (Serial2.available()) {
        // get the new byte:
        int inChar = Serial2.read();
        // Pass all incoming data to hdlc char receiver
        //Serial.print((char)inChar);
        //Serial.print(" ");
        //Serial.print(inChar,HEX);
        //Serial.print(", ");
        hdlc.frameDecode_char((uint8_t )inChar);
    }
    //Serial.println("End");
    // Serial.print(" [" );
    //Serial.print(hdlc.debug() );
    // Serial.print("] " );
}

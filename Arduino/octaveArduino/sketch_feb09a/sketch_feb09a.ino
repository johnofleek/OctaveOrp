#include <Arduhdlc.h>
#include <checksum.h>
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

static char payload[128];

/* Function to send out one 8bit character */
void send_character(uint8_t data) {
    Serial2.print((char)data);
}



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
}

void checkSend(void)
{
  char data[] = "ABCD";
  
  hdlc.frameEncode((uint8_t *)data, strlen(data));
}



int16_t orp_createResource(char packetType, char dataType, const char *path, const char * units )
{
    int16_t retval = -1;
    retval = snprintf(payload,sizeof(payload),
            "%c%c01P%s/value,U%s",
            packetType,
            dataType,
            path,
            units 
        );
    Serial.println(sizeof(payload));
    
    Serial.println(payload);
    hdlc.frameEncode((uint8_t *) payload, strlen(payload));
    
    /* Response 'i' || 'o' , status, <2bytes ignore>
    status == @ is OK */
    return (retval);
}

int16_t orp_pushValue(uint8_t dataType, const char *path, const char * data )
{
    int16_t retval = -1;
    char packetType = SBR_PKT_RQST_PUSH;
    
    retval = snprintf(payload,sizeof(payload),
            "%c%c01P%s/value,D%s",
            packetType,
            dataType,
            path,
            data 
        );
    Serial.println(sizeof(payload));
    Serial.println(payload);
    hdlc.frameEncode((uint8_t *) payload, strlen(payload));
    
    /* Response 'i' || 'o' , status, <2bytes ignore>
    status == @ is OK */
    return (retval);
}


void setup() {
    pinMode(16,OUTPUT); // Serial port TX to output
    // initialize serial port to 9600 baud
    Serial2.begin(9600);
    Serial.begin(115200);
    Serial.print("init");
    // checkSend();
    orp_createResource(SBR_PKT_RQST_INPUT_CREATE, SBR_DATA_TYPE_NUMERIC, "val/te", "num"  );
    Serial.println("done");
}


void loop() {
  delay(5000);
  orp_pushValue( SBR_DATA_TYPE_NUMERIC, "val/te" , "25.2" );
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

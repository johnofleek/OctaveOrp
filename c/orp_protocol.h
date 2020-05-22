/*
Copyright <2020> <J Thompson>
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/ 

#ifndef ORP_PROTOCOL_H
#define ORP_PROTOCOL_H

#ifdef __cplusplus
extern "C" {
#endif

/*
* https://docs.octave.dev/references/edge/octave_resource_protocol/#the-octave-resource-protocol
* https://docs.octave.dev/docs/octave-resource-protocol#section-response-structure
*/


// 0<packet type[1]>1<status[1]>2<segment[2]>4<contents[variable]>
#define ORP_PACKET_TYPE_FIELD           0
#define ORP_PACKET_STATUS_FIELD         1
#define ORP_PACKET_SEGMENT_START_FIELD  2
#define ORP_PACKET_CONTENTS_START_FIELD 4

//
// Packet type field - byte 0
//


#define SBR_PKT_RQST_INPUT_CREATE    'I'   // type[1] d_type[1] pad[2] path[] units[]
#define SBR_PKT_RESP_INPUT_CREATE    'i'   // type[1] status[1] pad[2]

#define SBR_PKT_RQST_OUTPUT_CREATE   'O'   // type[1] d_type[1] pad[2] path[] units[]
#define SBR_PKT_RESP_OUTPUT_CREATE   'o'   // type[1] status[1] pad[2]

#define SBR_PKT_RQST_DELETE          'D'   // type[1] pad[1]    pad[2] path[]
#define SBR_PKT_RESP_DELETE          'd'   // type[1] status[1] pad[2]

#define SBR_PKT_RQST_HANDLER_ADD     'H'   // type[1] pad[1]    pad[2] path[] 
#define SBR_PKT_RESP_HANDLER_ADD     'h'   // type[1] status[1] pad[2]

#define SBR_PKT_RQST_HANDLER_REMOVE  'K'   // type[1] pad[1]    pad[2] path[] 
#define SBR_PKT_RESP_HANDLER_REMOVE  'k'   // type[1] status[1] pad[2]

#define SBR_PKT_RQST_PUSH            'P'   // type[1] d_type[1] pad[2] time[] path[] data[]
#define SBR_PKT_RESP_PUSH            'p'   // type[1] status[1] pad[2]

#define SBR_PKT_RQST_GET             'G'   // type[1] pad[1]    pad[2] path[]
#define SBR_PKT_RESP_GET             'g'   // type[1] status[1] pad[2] time[] data[]

#define SBR_PKT_RQST_EXAMPLE_SET     'E'   // type[1] d_type[1] pad[2] path[] data[]
#define SBR_PKT_RESP_EXAMPLE_SET     'e'   // type[1] status[1] pad[2]

#define SBR_PKT_RQST_SENSOR_CREATE   'S'   // type[1] d_type[1] pad[2] path[] units[]
#define SBR_PKT_RESP_SENSOR_CREATE   's'   // type[1] status[1] pad[2]

#define SBR_PKT_RQST_SENSOR_REMOVE   'R'   // type[1] pad[1]    pad[2] path[]
#define SBR_PKT_RESP_SENSOR_REMOVE   'r'   // type[1] status[1] pad[2]

#define SBR_PKT_NTFY_HANDLER_CALL    'c'   // type[1] d_type[1] pad[2] time[] path[] data[]
#define SBR_PKT_RESP_HANDLER_CALL    'C'   // type[1] status[1] pad[2]

#define SBR_PKT_NTFY_SENSOR_CALL     'b'   // type[1] pad[1]    pad[2] path[]
#define SBR_PKT_RESP_SENSOR_CALL     'B'   // type[1] status[1] pad[2]

#define SBR_PKT_RESP_UNKNOWN_RQST    '?'   // type[1] status[1] pad[2]


//
// Data type field - byte 1
//


#define SBR_DATA_TYPE_TRIGGER        'T'   // trigger - no data
#define SBR_DATA_TYPE_BOOLEAN        'B'   // Boolean - 1 byte:  't' | 'f'
#define SBR_DATA_TYPE_NUMERIC        'N'   // numeric - null-terminated ASCII string, representing double
#define SBR_DATA_TYPE_STRING         'S'   // string  - null-terminated ASCII string
#define SBR_DATA_TYPE_JSON           'J'   // JSON    - null-terminated ASCII string, representing JSON
#define SBR_DATA_TYPE_UNDEF          ' '   // not specified

#define OK                           '@'    // weird encoding status value = -1 * (<status byte> - 0x40)  ==== 0


//
//
// 
//
//
typedef enum 
 {
    STATUS_OK,
    NOT_FOUND,
    NOT_POSSIBLE,
    OUT_OF_RANGE,
    NO_MEMORY,
    NOT_PERMITTED,
    FAULT,
    COMM_ERROR,
    TIMEOUT,
    OVERFLOW,
    UNDERFLOW,
    WOULD_BLOCK,
    DEADLOCK,
    FORMAT_ERROR,
    DUPLICATE,
    BAD_PARAMETER,
    CLOSED,
    BUSY,
    UNSUPPORTED,
    IO_ERROR,
    NOT_IMPLEMENTED,
    UNAVAILABLE,
    TERMINATED,
    WAITING_RESPONSE = -100,
    UN_INITIALISED = -101
 }orp_responseStatusEnum;

//
// Variable length field identifiers
//
#define SBR_FIELD_ID_PATH            'P'
#define SBR_FIELD_ID_TIME            'T'
#define SBR_FIELD_ID_UNITS           'U'
#define SBR_FIELD_ID_DATA            'D'

// Variable length field separator
#define SBR_VARLENGTH_SEPARATOR      ','


//  public:

// prototypes for callback functions
typedef void (* orp_hdlc_tx_cb) (uint8_t);   // to send data to the physical interface (UART ...)
typedef void (* orp_delay100ms_cb) (void);  // used for wakeup function


/*
    Current thinking on RX data handling.

    We have two types of unsolicited data from the Octave Edge device
    1. Notification Packets - these contain payload data sent by the Octave edge
      1.1 context is set by the Path 'P' in the example below
      1.2 and example PushHandler packet "c@01T1585927713.843660,Pval/od/value,D456.000000"
      1.3 I think one callback handler will be ok - it pumps incoming key/value pairs to the app
      1.4 Defined as SBR_PKT_NTFY_xxxxx above. For example SBR_PKT_NTFY_HANDLER_CALL and SBR_PKT_NTFY_SENSOR_CALL
    2. Request response Packets 
      2.1 these contain command ack responses from Octave edge
      2.2 there is an problem here there is no ack context available - therefore only one Request can be sent at once
      2.3 I think one callback handler will be ok - we need to save the context (path) when the original call is made
      2.4 We need to lock the sending interface whilst waiting for a response
      2.5 Defined as SBR_PKT_RESP_xxxxx above


    Thinking is we have a orp protocol app which intercepts the incoming packets and
    splits them into two types.
    Next question is - what do they call ?

    At the moment plan is to have 2 application callback functions
    1. appRequestIn_cbf
    2. appNotificationIn_cbf

    Then some helper functions to decode the cb payload
    Initially I was thinking of mapping outputs to individual callbacks but i think 
    it's better of the user app does this by looking at the key (path)

    packet structure:
    <packet type[1]><status[1]><segment[2]><contents[variable]>
    
    Fields:
    packet type : Identifies response, 1-byte
    status      : Identifies status of response. 1-byte
    segment     : Count allowing a responses to span multiple packets. 2-bytes
    contents    : Packet dependent, variable length

*/
// Use the following as a prototype for all app response callbacks 
typedef void (*orp_protocol_genericRequestResponse_cb)
(
  const uint8_t *buffer,
  uint16_t bufferLength
);

// Use the following as a prototype for all app notification callbacks 
typedef void (*orp_protocol_genericNotification_cb)
(
  const uint8_t *buffer,
  uint16_t bufferLength
);

// pass in buffer references 
void orp_protocol(
    char * app_orp_txBuffer,            // encoder input payload 
    size_t app_orp_txBuffer_size,       // encoder input payload size

    orp_hdlc_tx_cb tx_serial_cb,        // encoder - bind TX a byte to UART
                                        // decoder - no need to bind rx as the HDLC layer has a direct write input 

    uint8_t *hdlc_rxBuffer,             // decoder - working buffer
    uint16_t hdlc_rxBuffer_size,        // decoder - length of working buffer

    uint16_t * app_crc_tabccitt,        // is a pointer to the app CRC table resource 
    size_t app_crc_tabccitt_size,       // is the sizeof the app CRC table

    orp_protocol_genericRequestResponse_cb  appRequestIn_cbf, // called by rx decoder when response arrives
    orp_protocol_genericNotification_cb     appNotificationIn_cbf // called by rx decoder when response arrives
);


// *   Apps sends RAW HDLC data from serial stream in via this function one byte at a time
void orp_protocol_processHdlcRx(uint8_t data);

void orp_protocol_wakeup( orp_delay100ms_cb);


int16_t orp_protocol_pushValue(
  uint8_t dataType,
  const char *path,
  const char * data
);

int16_t orp_protocol_createResource(
  char packetType,
  char dataType,
  const char *path,
  const char * units
);

int16_t orp_protocol_addpushHandler(
  uint8_t dataType,
  const char *path
);




// todo:
// The serial cycle is one at a time - send and ack.
// But with possible unsolicited events as well
// ideally we need a way to set a flag whilst waiting for an request response ack
// or maybe the app can decide if it wants to ignore the request response (ack)
// 
// We need to implement some Notification and RequestResponse data decode helper functions 
 

#ifdef __cplusplus
}
#endif

#endif // ORP_PROTOCOL_H
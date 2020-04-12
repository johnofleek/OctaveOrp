/*
* https://docs.octave.dev/references/edge/octave_resource_protocol/#the-octave-resource-protocol
*/


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
typedef void (* txChar_cb) (uint8_t);
typedef void (* delay100ms_cb) (void);

// pass in buffer references 
void orp_protocol(char * sendingPayload, size_t sendingPayloadMaxSize);

int16_t orp_protocol_wakeup(txChar_cb, delay100ms_cb);

int16_t orp_protocol_pushValue( uint8_t dataType, const char *path, const char * data );

int16_t orp_protocol_createResource( char packetType, char dataType, const char *path, const char * units );

int16_t orp_protocol_addpushHandler(uint8_t dataType, const char *path);




// todo:
// The serial cycle is one at a time - send and ack.
// But with possible unsolicited events as well
// ideally we need a way to set a flag whilst waiting for an command ack

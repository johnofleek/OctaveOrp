/*
    Functions that support Octave Resource Protocol ORP 
*/

#ifdef ARDUINO
//    #include <Arduhdlc.h>
#endif

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include "orp.h"

// local globals for app resources - idea is to minimise these
static char * orpProtocol_txPayload = "";
static size_t orpProtocol_txPayloadMaxSize = 0;

static void orp_protocol_nullRequestResponse_cb(const uint8_t *buffer, uint16_t bufferLength)    // do nothing if unlinked calls
{
    TRACE(("unbound orp_protocol_cb %c%c  len %d", buffer[0], buffer[1], bufferLength));
}

// called in response to an orp request to Octave -- its an ack handler
static orp_protocol_genericRequestResponse_cb orp_currentRequestResponse_cbf = orp_protocol_nullRequestResponse_cb;

// called when a notification arrives from Octave
static orp_protocol_genericRequestResponse_cb orp_currentNotification_cbf = orp_protocol_nullRequestResponse_cb;


// static hdlc_decoder_callback_type hdlc_decoded_cbh = NULL; // processes locally hdlc to orp - needs to be a function
// static orp

/*
 * this  is called when a hdlc packet is decoded
 * we have to ORP decode the contents of the packet 
 * then pass on anything as required 
 * 
 * <packet type[1]><status[1]><segment[2]><contents[variable]>
 * Fields:
 * packet type : Identifies response, 1-byte
 * status      : Identifies status of response. 1-byte
 * segment     : Count allowing a responses to span multiple packets. 2-bytes
 * contents    : Packet dependent, variable length
*/
static void hdlc_decoded_cbh(const uint8_t *buffer, uint16_t bufferLength)
{
    // Decode which type of packet
    switch(buffer[ORP_PACKET_TYPE_FIELD])
    {
        case SBR_PKT_NTFY_HANDLER_CALL:
            orp_currentNotification_cbf(buffer, bufferLength );
            break;
        case SBR_PKT_NTFY_SENSOR_CALL:
            orp_currentNotification_cbf(buffer, bufferLength );
            break;
            // If we get this far we should just get SBR_PKT_RQST_xxxxxx
        default:
            orp_currentRequestResponse_cbf(buffer, bufferLength );
            break;
    } 

}



/*
*   Initialise orp and hdlc
*/
void orp_protocol(
    char * app_orp_inPayload,           // encoder input payload 
    size_t app_orp_inPayloadSize,       // encoder input payload size

    orp_hdlc_tx_cb tx_serial_cb,        // encoder - bind TX a byte to UART
                                        // decoder - no need to bind rx as the HDLC layer has a direct write input 

    // hdlc_decoder_callback_type hdlc_decoded_callback, // decoder - frame decoded callback with payload ----- don't need this ??? !!!
    uint8_t *hdlc_rx_buffer,            // decoder - working buffer
    uint16_t hdlc_rx_bufferSize         // decoder - length of working buffer
)
{
    orpProtocol_txPayload = app_orp_inPayload;              // used to contain the orp payload which is sent via HDLC
    orpProtocol_txPayloadMaxSize = app_orp_inPayloadSize;
    // Bind the user "app notification callback" handler - this to handle unsolicited messages from Octave

    // Bind the user "app Request_response" callback handler - this is to handle uplink acks (response)
    hdlc_hdlc (    
        tx_serial_cb,                        // encoder - binds - sends a byte to UART
        hdlc_decoded_cbh,                   // decoder - frame decoded callback with payload 
        hdlc_rx_buffer,                     // decoder - working buffer
        hdlc_rx_bufferSize                  // decoder - length of working buffer
    );
}

/*
*   Apps sends RAW HDLC data from serial stream in via this function one at a time
*/
void orp_protocol_processHdlcRx(uint8_t data)
{
    hdlc_frameDecode_char(data);
}


void orp_protocol_wakeup( orp_delay100ms_cb delay100ms) // I'm hoping this will be optimised out 
{
    hdlc_wakeup(delay100ms);  
}



int16_t orp_protocol_createResource(
  char packetType,
  char dataType,
  const char *path,
  const char * units,
  orp_protocol_genericRequestResponse_cb callbackFunction // called by rx decoder when response arrives
)
{
    int16_t retval = 0;

    orp_currentRequestResponse_cbf = callbackFunction; // this becomes the app requestReponse ack handler

    // check for no units option
    if(units[0] == '\0')
    {
        retval = snprintf(
                orpProtocol_txPayload, orpProtocol_txPayloadMaxSize, "%c%c01P%s/value",
                packetType,
                dataType,
                path
        );
    }
    else
    {
        retval = snprintf(orpProtocol_txPayload, orpProtocol_txPayloadMaxSize, "%c%c01P%s/value,U%s",
            packetType,
            dataType,
            path,
            units 
        );
    }


    // hdlc encode and send serial port 
    hdlc_frameEncode((uint8_t *) orpProtocol_txPayload, strlen(orpProtocol_txPayload));

 

    /* ORP Response 'i' || 'o' , status, <2bytes ignore>
    status == @ is OK */
    return (retval);
}

int16_t orp_protocol_pushValue(
   uint8_t dataType,
   const char *path,
   const char * data,
   orp_protocol_genericRequestResponse_cb callbackFunction
)
{
    int16_t retval = 0;
    char packetType = SBR_PKT_RQST_PUSH;

    orp_currentRequestResponse_cbf = callbackFunction; // this becomes the app requestReponse ack handler
    
    retval = snprintf(orpProtocol_txPayload, orpProtocol_txPayloadMaxSize,
            "%c%c01P%s/value,D%s",
            packetType,
            dataType,
            path,
            data 
        );
    
        // hdlc encode and send serial port 
    hdlc_frameEncode((uint8_t *) orpProtocol_txPayload, strlen(orpProtocol_txPayload));
    /* ORP Response 'i' || 'o' , status, <2bytes ignore>
    status == @ is OK */
    return (retval);
}

int16_t orp_protocol_addpushHandler(
  uint8_t dataType,
  const char *path,
  orp_protocol_genericRequestResponse_cb callbackFunction // called by rx decoder when response arrives
)
{
    int16_t retval = 0;
    char packetType = SBR_PKT_RQST_HANDLER_ADD;
    
    orp_currentRequestResponse_cbf = callbackFunction; // this becomes the app requestReponse ack handler

    retval = snprintf(orpProtocol_txPayload, orpProtocol_txPayloadMaxSize,
            "%c%c01P%s/value",
            packetType,
            dataType,
            path 
        );
    
    /* ORP Response 'i' || 'o' , status, <2bytes ignore>
    status == @ is OK */
    return (retval);
}




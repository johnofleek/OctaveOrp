/*
Copyright <2020> <J Thompson>
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/ 

/*
    Functions that support Octave Resource Protocol ORP 
*/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#include "orp.h"

// local globals for app resources - idea is to minimise these
static char * orpProtocol_txBuffer = "";
static size_t orpProtocol_txBuffer_size = 0;


static void orp_protocol_nullRequestResponse_cb(const uint8_t *buffer, uint16_t bufferLength )    // do nothing if unlinked calls
{
    TRACE(("unbound orp_protocol_cb %c%c  len %d", buffer[0], buffer[1], bufferLength));
}

static void orp_protocol_nullNotification_cb(const uint8_t *buffer, uint16_t bufferLength)
{
    TRACE(("unbound orp_protocol_cb %c%c  len %d", buffer[0], buffer[1], bufferLength));
}

// called in response to an orp request to Octave -- its an ack handler
static orp_protocol_genericRequestResponse_cb orp_currentRequestResponse_cbf = orp_protocol_nullRequestResponse_cb;

// called when a notification arrives from Octave
static orp_protocol_genericNotification_cb orp_currentNotification_cbf = orp_protocol_nullNotification_cb;


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
)
{
    orpProtocol_txBuffer = app_orp_txBuffer;              // used to contain the orp payload which is sent via HDLC
    orpProtocol_txBuffer_size = app_orp_txBuffer_size;

    // Bind the user "app notification callback" handler - this to handle unsolicited messages from Octave
    orp_currentNotification_cbf = appNotificationIn_cbf;

    // Bind the user "app Request_response" callback handler - this is to handle uplink acks (response)
    orp_currentRequestResponse_cbf = appRequestIn_cbf;

    crcccitt_crcccitt( app_crc_tabccitt,  app_crc_tabccitt_size);

    hdlc_hdlc (    
        tx_serial_cb,                        // encoder - binds - sends a byte to UART
        hdlc_decoded_cbh,                   // decoder - frame decoded callback with payload 
        hdlc_rxBuffer,                     // decoder - working buffer
        hdlc_rxBuffer_size                  // decoder - length of working buffer
    );
}

/// @param uint16_t * app_crc_tabccitt is a pointer to the app CRC table resource
/// @param size_t app_crc_tabccitt_size is the sizeof the app CRC table

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
  const char * units
)
{
    int16_t retval = 0;

    // check for no units option
    if(units[0] == '\0')
    {
        retval = snprintf(
                orpProtocol_txBuffer, orpProtocol_txBuffer_size, "%c%c01P%s/value",
                packetType,
                dataType,
                path
        );
    }
    else
    {
        retval = snprintf(orpProtocol_txBuffer, orpProtocol_txBuffer_size, "%c%c01P%s/value,U%s",
            packetType,
            dataType,
            path,
            units 
        );
    }


    // hdlc encode and send serial port 
    hdlc_frameEncode((uint8_t *) orpProtocol_txBuffer, strlen(orpProtocol_txBuffer));

 

    /* ORP Response 'i' || 'o' , status, <2bytes ignore>
    status == @ is OK */
    return (retval);
}

int16_t orp_protocol_pushValue(
   uint8_t dataType,
   const char *path,
   const char * data
)
{
    int16_t retval = 0;
    char packetType = SBR_PKT_RQST_PUSH;

    retval = snprintf(orpProtocol_txBuffer, orpProtocol_txBuffer_size,
            "%c%c01P%s/value,D%s",
            packetType,
            dataType,
            path,
            data 
        );
    
    // hdlc encode and send serial port 
    hdlc_frameEncode((uint8_t *) orpProtocol_txBuffer, strlen(orpProtocol_txBuffer));
    /* ORP Response 'i' || 'o' , status, <2bytes ignore>
    status == @ is OK */
    return (retval);
}

int16_t orp_protocol_addpushHandler(
  uint8_t dataType,
  const char *path
)
{
    int16_t retval = 0;
    char packetType = SBR_PKT_RQST_HANDLER_ADD;
    
    retval = snprintf(orpProtocol_txBuffer, orpProtocol_txBuffer_size,
            "%c%c01P%s/value",
            packetType,
            dataType,
            path 
        );
    
    // hdlc encode and send serial port 
    hdlc_frameEncode((uint8_t *) orpProtocol_txBuffer, strlen(orpProtocol_txBuffer));

    /* ORP Response 'i' || 'o' , status, <2bytes ignore>
    status == @ is OK */
    return (retval);
}




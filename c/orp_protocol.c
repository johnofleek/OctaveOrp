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

#include "orp_protocol.h"
#include "hdlc.h"

char * txPayload = NULL;
size_t txPayloadMaxSize = 0;

void orp_protocol(char * payloadIn, size_t maxPayloadSizeIn)
{
    txPayload = payloadIn;
    txPayloadMaxSize = maxPayloadSizeIn;
}

int16_t orp_protocol_wakeup(txChar_cb txChar, delay100ms_cb delay100ms)
{
    int16_t retval = 0;

    txChar('~');
    delay100ms();

    txChar('~');
    delay100ms();

    txChar('~');
    delay100ms();

    return (retval);
}

int16_t orp_protocol_createResource( char packetType, char dataType, const char *path, const char * units )
{
    int16_t retval = 0;
    
    // check for no units option
    if(units[0] == '\0')
    {
        retval = snprintf(
                txPayload, txPayloadMaxSize, "%c%c01P%s/value",
                packetType,
                dataType,
                path
        );
    }
    else
    {
        retval = snprintf(txPayload, txPayloadMaxSize, "%c%c01P%s/value,U%s",
            packetType,
            dataType,
            path,
            units 
        );
    }
    
    // hdlc encode and send serial port 
    hdlc_frameEncode((uint8_t *) txPayload, strlen(txPayload));
    
    /* ORP Response 'i' || 'o' , status, <2bytes ignore>
    status == @ is OK */
    return (retval);
}

int16_t orp_protocol_pushValue(uint8_t dataType, const char *path, const char * data )
{
    int16_t retval = 0;
    char packetType = SBR_PKT_RQST_PUSH;
    
    retval = snprintf(txPayload, txPayloadMaxSize,
            "%c%c01P%s/value,D%s",
            packetType,
            dataType,
            path,
            data 
        );
    
        // hdlc encode and send serial port 
    hdlc_frameEncode((uint8_t *) txPayload, strlen(txPayload));
    /* ORP Response 'i' || 'o' , status, <2bytes ignore>
    status == @ is OK */
    return (retval);
}


int16_t orp_protocol_addpushHandler(uint8_t dataType, const char *path)
{
    int16_t retval = 0;
    char packetType = SBR_PKT_RQST_HANDLER_ADD;
    
    retval = snprintf(txPayload, txPayloadMaxSize,
            "%c%c01P%s/value",
            packetType,
            dataType,
            path 
        );
    
    /* ORP Response 'i' || 'o' , status, <2bytes ignore>
    status == @ is OK */
    return (retval);
}




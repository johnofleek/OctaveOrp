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
#include <orp.h> 

#include "Arduino.h"

orp::orp(char * payloadIn, size_t payloadSizeIn)
{
    payload = payloadIn;
    payloadSize = payloadSizeIn;
    Serial.println(payload);
}

int16_t orp::wakeup(void)
{
    int16_t retval = 0;
    strncpy(payload, "", payloadSize);
    return (retval);
}

int16_t orp::createResource( char packetType, char dataType, const char *path, const char * units )
{
    int16_t retval = -1;
    
    retval = snprintf(payload, payloadSize,
            "%c%c01P%s/value,U%s",
            packetType,
            dataType,
            path,
            units 
        );
    Serial.println(payload);
    
    /* ORP Response 'i' || 'o' , status, <2bytes ignore>
    status == @ is OK */
    return (retval);
}

int16_t orp::pushValue(uint8_t dataType, const char *path, const char * data )
{
    int16_t retval = -1;
    char packetType = SBR_PKT_RQST_PUSH;
    
    retval = snprintf(payload, payloadSize,
            "%c%c01P%s/value,D%s",
            packetType,
            dataType,
            path,
            data 
        );
    
    /* ORP Response 'i' || 'o' , status, <2bytes ignore>
    status == @ is OK */
    return (retval);
}

 
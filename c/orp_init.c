// The micro Octave remote C API 
#include "orp_init.h"


uint8_t octave_init(
    serial_sendTxByte_uf * serial_sendTxByte, //! bind the UART send function
    serial_getRxByte_cbf * serial_rxByte,   //! bind the UART get function
    const uint8_t *HDLC_frameBuffer,        //! user passes in a buffer
    uint16_t HDLC_frameBuffer_length,       //! user passes in buffer length 
    uint16_t HDLC_maxFrameLength            //! user passes in max frame length (TBD)
)
{
    return(0);
}

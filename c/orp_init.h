/**
 * @file orp_init.h
 * @author John
 * @date 2020-03-02
 * @brief init functions for orp micro
 *
 */


#include <stdint.h>
#include <errno.h> // or add your own version

//! Functions which the user app provides and init binds to this code 
//! The orp system uses this function to send bytes to the physical UART
typedef uint8_t (serial_sendTxByte_uf)(uint8_t);

//! The orp system uses this callback function to get bytes from the physical UART
//! returns - negative numbers indicates errors
typedef int16_t (serial_getRxByte_cbf)(void);



uint8_t octave_init(
    serial_sendTxByte_uf * serial_sendTxByte, //! bind the UART send function
    serial_getRxByte_cbf * serial_rxByte,   //! bind the UART get function
    const uint8_t *HDLC_frameBuffer,        //! user passes in a buffer
    uint16_t HDLC_frameBuffer_length,       //! user passes in buffer length 
    uint16_t HDLC_maxFrameLength            //! user passes in max frame length (TBD)
);


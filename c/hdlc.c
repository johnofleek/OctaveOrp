
#include <stdio.h>  // just for printf debug
#include "orp.h"
#include "crcccitt.h"

/* HDLC Asynchronous framing */
/* The frame boundary octet is 01111110, (7E in hexadecimal notation) */
#define FRAME_BOUNDARY_OCTET 0x7E

/* A "control escape octet", has the bit sequence '01111101', (7D hexadecimal) */
#define CONTROL_ESCAPE_OCTET 0x7D

/* If either of these two octets appears in the transmitted data, an escape octet is sent, */
/* followed by the original data octet with bit 5 inverted */
#define INVERT_OCTET 0x20

/* The frame check sequence (FCS) is a 16-bit CRC-CCITT */
/* AVR Libc CRC function is _crc_ccitt_update() */
/* Corresponding CRC function in Qt (www.qt.io) is qChecksum() */
#define CRC16_CCITT_INIT_VAL 0xFFFF

/* 16bit low and high bytes copier */
#define low(x)    ((x) & 0xFF)
#define high(x)   (((x)>>8) & 0xFF)

//  private:
/* User must define a function, that sends a 8bit char over the chosen interface -
   usart, spi, i2c etc. */
static txChar_cb sendchar_function;

/* User must define a function, that will process the valid received frame */
/* This function can act like a command router/dispatcher */
static hdlc_decoder_callback_type decoded_callback;
// static void sendchar_function(uint8_t data);


static bool escape_character;
static uint8_t * receive_frame_buffer;
static uint8_t frame_position;

// 16bit CRC sum for _crc_ccitt_update
static uint16_t frame_checksum;
static uint16_t max_frame_length;


// public interface

void hdlc_hdlc ( 
    txChar_cb put_char,
    hdlc_decoder_callback_type hdlc_decoded_callback, 
    uint8_t *rx_frameBuffer,
    uint16_t rx_frameLength
) 
{
    sendchar_function = put_char;

    decoded_callback = hdlc_decoded_callback;
    frame_position = 0;
	max_frame_length = rx_frameLength;
	receive_frame_buffer = rx_frameBuffer; 
    frame_checksum = CRC16_CCITT_INIT_VAL;
    escape_character = false;
}


/*  Function to find valid HDLC frame from incoming data - 
    A byte at a time*/
void hdlc_frameDecode_char(uint8_t data)
{
    /* FRAME FLAG */
    if(data == FRAME_BOUNDARY_OCTET)
    {         
        // Check to see if this really is a FRAME_BOUNDARY_OCTET
        if(escape_character == true)
        {
            escape_character = false;
        }         
        else if((frame_position >= 2))  /* If a valid frame is detected */
                                        // checksum here // (msb << 8 ) | (lsb & 0xff)
        {
            frame_checksum = crc_ccitt_ffff(receive_frame_buffer, (frame_position - 2)) ;
        
            uint16_t rxChecksum = receive_frame_buffer[frame_position-2] << 8 ;
            rxChecksum = rxChecksum | (receive_frame_buffer[frame_position-1] & 0xff);

            printf("Check %X RX %X\r\n", frame_checksum, rxChecksum);
            if(rxChecksum == frame_checksum )
            {
                // Call the user defined function and pass decoded payloadframe to it 
                decoded_callback(receive_frame_buffer,(frame_position-2));
            }

        }
        // reset frame 
        frame_position = 0;
        frame_checksum = CRC16_CCITT_INIT_VAL;
        return;
    }

    if(escape_character)
    {
        escape_character = false;
        data ^= INVERT_OCTET;
    }
    else if(data == CONTROL_ESCAPE_OCTET)
    {
        escape_character = true;
        return;
    }

    receive_frame_buffer[frame_position] = data;



    frame_position++;
    // restart if the frame got too long
    if(frame_position == max_frame_length)
    {
        frame_position = 0;
        frame_checksum = CRC16_CCITT_INIT_VAL;
    }
}



/* Wrap given data in HDLC frame and send it out byte at a time*/
void hdlc_frameEncode(const uint8_t *framebuffer, uint8_t frame_length)
{
    uint8_t data ;
    uint16_t fcs = CRC16_CCITT_INIT_VAL;

    sendchar_function((uint8_t)FRAME_BOUNDARY_OCTET);
    
    fcs = crc_ccitt_ffff(framebuffer, (frame_length));

    while(frame_length)
    {
        data = *framebuffer++;

        if((data == CONTROL_ESCAPE_OCTET) || (data == FRAME_BOUNDARY_OCTET))
        {
            sendchar_function((uint8_t)CONTROL_ESCAPE_OCTET);
            data ^= INVERT_OCTET;
        }
        sendchar_function((uint8_t)data);
        frame_length--;
    }
    
    data = high(fcs);
    if((data == CONTROL_ESCAPE_OCTET) || (data == FRAME_BOUNDARY_OCTET))
    {
        sendchar_function(CONTROL_ESCAPE_OCTET);
        data ^= INVERT_OCTET;
    }
    sendchar_function(data);
    
    data = low(fcs);
    if((data == CONTROL_ESCAPE_OCTET) || (data == FRAME_BOUNDARY_OCTET))
    {
        sendchar_function((uint8_t)CONTROL_ESCAPE_OCTET);
        data ^= (uint8_t)INVERT_OCTET;
    }
    sendchar_function((uint8_t)data);   

    sendchar_function(FRAME_BOUNDARY_OCTET);
}


void hdlc_wakeup( hdlc_delay100ms_cb delay100ms)
{
    int16_t retval = 0;

    sendchar_function('~');
    delay100ms();

    sendchar_function('~');
    delay100ms();

    sendchar_function('~');
    delay100ms();
}

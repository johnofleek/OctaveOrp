
#include "checksum.h"
#include "hdlc.h"
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
static sendchar_type sendchar_function;

/* User must define a function, that will process the valid received frame */
/* This function can act like a command router/dispatcher */
static frame_handler_type frame_handler;
static void sendchar(uint8_t data);

static bool escape_character;
static uint8_t * receive_frame_buffer;
static uint8_t frame_position;

// 16bit CRC sum for _crc_ccitt_update
static uint16_t frame_checksum;
static uint16_t max_frame_length;

void hdlc_hdlc ( 
    sendchar_type put_char,
    frame_handler_type hdlc_command_router,
    uint8_t *rx_frameBuffer,
    uint16_t rx_frameLength
) 
{
    sendchar_function = put_char;

    frame_handler = frame_handler;
    frame_position = 0;
	max_frame_length = rx_frameLength;
	receive_frame_buffer = rx_frameBuffer; 
    frame_checksum = CRC16_CCITT_INIT_VAL;
    escape_character = false;
}

/* Function to send a byte throug USART, I2C, SPI etc.*/
void hdlc_sendchar(uint8_t data)
{
    sendchar_function(data);
}

/*  Function to find valid HDLC frame from incoming data - 
    A byte at a time*/
void hdlc_frameDecode_char(uint8_t data)
{
    /* FRAME FLAG */
    if(data == FRAME_BOUNDARY_OCTET)
    {
        frame_checksum = crc_ccitt_ffff(receive_frame_buffer, (frame_position - 2)) ;
        
        uint16_t rxChecksum = receive_frame_buffer[frame_position-2] << 8 ;
        rxChecksum = rxChecksum | (receive_frame_buffer[frame_position-1] & 0xff);
        
        if(escape_character == true)
        {
            escape_character = false;
        }         
        else if((frame_position >= 2))  /* If a valid frame is detected */
                                        // checksum here // (msb << 8 ) | (lsb & 0xff)
        {
            /* Call the user defined function and pass frame to it */
            frame_handler(receive_frame_buffer,(uint8_t)(frame_position-2));
        }
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

    if(frame_position-2 >= 0) 
    {
        // ???
    }

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
    uint16_t fcs2 = 0;

    sendchar((uint8_t)FRAME_BOUNDARY_OCTET);
    
    fcs = crc_ccitt_ffff(framebuffer, (frame_length));

    while(frame_length)
    {
        data = *framebuffer++;
        // fcs = _crc_ccitt_update(fcs, data);
        if((data == CONTROL_ESCAPE_OCTET) || (data == FRAME_BOUNDARY_OCTET))
        {
            sendchar((uint8_t)CONTROL_ESCAPE_OCTET);
            data ^= INVERT_OCTET;
        }
        sendchar((uint8_t)data);
        frame_length--;
    }
    
    fcs2 = fcs; // work around compiler issue with 16->8 bit   
    data = high(fcs2);
    if((data == CONTROL_ESCAPE_OCTET) || (data == FRAME_BOUNDARY_OCTET))
    {
        sendchar(CONTROL_ESCAPE_OCTET);
        data ^= INVERT_OCTET;
    }
    sendchar(data);
    
    fcs2 = fcs;  
    data = low(fcs2);
    if((data == CONTROL_ESCAPE_OCTET) || (data == FRAME_BOUNDARY_OCTET))
    {
        sendchar((uint8_t)CONTROL_ESCAPE_OCTET);
        data ^= (uint8_t)INVERT_OCTET;
    }
    sendchar((uint8_t)data);   

    sendchar(FRAME_BOUNDARY_OCTET);
}



#ifndef hdlc_h
#define hdlc_h


// #include "orp.h"

typedef void (* txChar_cb) (uint8_t);
typedef void (* hdlc_decoder_callback_type)(const uint8_t *framebuffer, uint16_t framelength);


//  public:
// note there is no rx_getchar() as the host sends in bytes via hdlc_frameDecode_char 

// app provides a delay function for wakeup function
typedef void (* hdlc_delay100ms_cb) (void);


void hdlc_hdlc (    
  txChar_cb put_char,                 // encoder - bind send a byte to UART
  hdlc_decoder_callback_type hdlc_decoded_callback, // decoder - frame decoded callback with payload 
  uint8_t *rx_frame_buffer,               // decoder - working buffer
  uint16_t max_rxFrame_length             // decoder - length of working buffer
);

/*
  Raw serial HDLC encoded bytes are sent into this function
  If a frame is decoded the hdlc_decoded_callback() is called with the
  decoded payload 
*/
void hdlc_frameDecode_char(uint8_t data);

/*
  This takes a payload_buffer payload in
  HDLC encodes it
  Then sends the HDLC encoded data using put_char()
*/
void hdlc_frameEncode(const uint8_t *payload_buffer, uint8_t payload_length);

void hdlc_wakeup( hdlc_delay100ms_cb delay100ms);




#endif // hdlc_h
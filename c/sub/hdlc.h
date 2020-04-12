#ifndef hdlc_h
#define hdlc_h

#include <stdint.h>
#include <stdbool.h>


typedef void (* sendchar_type) (uint8_t);
typedef void (* frame_handler_type)(const uint8_t *framebuffer, uint16_t framelength);


//  public:
void hdlc_hdlc (    
  sendchar_type put_char,
  frame_handler_type hdlc_command_router,
  uint8_t *rx_frameBuffer,
  uint16_t rx_frameLength
);
void hdlc_frameDecode_char(uint8_t get_char);
void hdlc_frameEncode(const uint8_t *framebuffer, uint8_t frame_length);


#endif
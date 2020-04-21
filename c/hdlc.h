/*
Copyright <2020> <J Thompson>
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/ 

#ifndef HDLC_H
#define HDLC_H

#ifdef __cplusplus
extern "C" {
#endif


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

/*
 * This needs some work as most system will block during the 100ms delay 
 */
void hdlc_wakeup( hdlc_delay100ms_cb delay100ms);

#ifdef __cplusplus
}
#endif

#endif // HDLC_H
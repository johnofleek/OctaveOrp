# Purpose of this document - currently PHASE 1

*PHASE 1*  
To define a C API which enables easy integration of the Octave system into basic microcontroller projects

*PHASE 2*   
In addition describe how the C API wraps the ORP (Octave Resource Protocol) wrt to the remote microcontroller



# Goals
* Code written in as generic as possible C  
* Targets microcontrollers only
* Doesn't require an OS
* The API should be simple to use  
* The final code should be portable to other microcontrollers
* limited set of host microcontroller software dependencies

# Description
End users will include the code in their project and use the API to 
* Send data to Octave
* Receive unsolicited data from Octave 

The host microcontroller will provide certain interfaces for example
 non-blocking UART interfaces - full info TBD

# Limitations
It is proposed at this stage to implement a subset of the ORP
 protocol supporting only 
* Octave inputs 
* Octave outputs

# Terms
| Term                           | Description |
|:------------------------------ |:--- |
| Remote                         | this is the microcontroller |
| Octave Edge                    | this the WP7702 or whatever Octave device is in the field |
| Octave Cloud                   | this is Sierra's cloud compute system | 
| Octave_RemoteMicro_api         | the api that is defined in this document |
| Octave_RemoteMicro_code        | the code which implements the api and the functionality |
| Octave resource protocol (ORP) | the serial protocol between the Octave Edge Device and the Remote Device |


---


# C API design philosophy

Data sets sent over ORP will be implemented as KEY / VALUE pair strings.

Values and Keys will be truncated at a size defined by the users C storage. The
 user will need to handle any errors caused by truncation.
 
At ORP level the KEY will be the ORP data path and the VALUE will be the ORP payload.



## KEYs
The same KEY formats should work for both input and output transactions. My thinking
 is that if a consistent KEY format is used it will be easy to post process the data
 in the Octave cloud, Octave edge or the microcontroller.

A suggestion is that a meaningful KEY name is used which describes both the function and the datatype. 
The remote key will form part of the Octave device path 
 
A suggestion is to embed the datatype in the KEY path   
```
str  
num    
bool  
```

A remote KEY example  
```
adc1/num
```  


Results in an Octave device path   
```
/remote/adc1/num/value
```

## Octave output data

Data received from Octave is restricted by this project to individual KEY / VALUE
 pairs. This removes the need to decode incoming data in a difficult to parse
 in C format like JSON.

 
### Octave output VALUE 
Note that strings have been used to represent all types this is to simplify the
 interface. They can be reformated locally by the microcontroller code as required.

Number examples - expect a float in Octave
```
"101"  
"23.27"    
```

Strings examples
```
"powerOff"
"motorStarted"
```
Bool examples 
```
"true"  
"false"  
```


 
## Octave input VALUE - JSON payload
Values sent to Octave are encoded by the users remote application. It's
 proposed to only use simple JSON strings
 as the are easy to encode in C using prebuilt functions like snprintf(). 
 
 By using [JSON](https://www.json.org/json-en.html) the value encoding used in JSON
 can be used as JSON data is supported by the Octave edge system. 

### Example octave JSON input value
For example the following JSON encoded data could be sent to the Octave edge  
```
{“temperature”:123.2,“humidity”:70,“pressure”:9997,“powerOn”:true}
or 
{"ADC1":998,"ADC2":102}
or 
{"input1":"on"}
```


## Octave ORP interaction - remote initiated
This section describes the ORP interface over the serial port.
In particular the interaction between this projects microcontroller code and
 the Octave edge remote service

The basic Octave ORP cycle is
1. Remote sends a message to Octave Edge
2. Octave edge responds with an ack / nack message

It is proposed to handle this cycle by implementing the following in the
 Octave_RemoteMicro_code

1. The users remote application calls a Octave_RemoteMicro_Api function which sends
  an ORP message to the Octave Edge device to the serial port and then returns
1. The Octave system then processes the ORP message
1. After the Octave system has finished processing the ORP message it sends 
  a response message to the Octave_RemoteMicro_code code via the serial port
1. The Octave_RemoteMicro_code decodes the response message
1. The Octave_RemoteMicro_code calls the appropriate users callback* function which
  was registered during initialisation of the Octave_RemoteMicro_Api

## Octave interaction - Octave initiated
1. Octave Edge receives an Octave message from the remote device
1. Octave Edge sends the message to the remote device via the serial port
1. The Octave_RemoteMicro_code decodes the ORP message
1. The Octave_RemoteMicro_code executes any registered callbacks which match the ORP message


---

# C API

DO NOT TAKE ANY NOTICE OF THIS SECTION ITS STILL BEING WORKED ON

// Low level platform dependencies
/// serial port from Octave to Micro

The target micro provides these functions to the micro Octave remote C code 
serial_rxByte(uint8_t data);
serial_rxByte(uint8_t data),



// The micro Octave remote C API
octave_init(
    &serial_txByte(uint8_t),
    &serial_rxByte(uint8_t data),
    const uint8_t *HDLC_frameBuffer,
    uint16_t HDLC_frameBuffer_length), 
    uint16_t HDLC_maxFrameLength,      // maybe hide this? 
    
)


## Register inputs
```
// orp.h

typedef void (*octave_response_cbf)(char *);

typedef struct
{
	char *keyString;
	octave_response_cbf userApp_OctaveResponse_cbh;
} octaveInput_struct;

uint8_t octave_register_inputs(octaveInput_struct *inputKeys, int8_t numberOfInputs);


// orp.c
uint8_t octave_register_inputs(octaveInput_struct *inputKeys, int8_t numberOfInputs)
{
	// register the Octave inputs
	// then exit
	return(0);
}

```

```
// Test the API with a basic c app

// This is the real user application
// #include "orp.h"

// in the app create an array of structs
#define NUMBER_OF_INPUTS 5



void testApi(void)
{
	int8_t numberOfInputs = 2;
	
	octaveInput_struct octaveInputs_struct[NUMBER_OF_INPUTS];
	octave_register_inputs( octaveInputs_struct, numberOfInputs);	
}
```



--- 
# Notes
Is this blocking or none blocking
What about failures?
 


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

The host microcontroller will provide certain interfaces for example non-blocking UART interfaces - full info TBD

# Limitations
It is proposed at this stage to implement a subset of the ORP protocol supporting only 
* Octave inputs 
* Octave outputs

# API
Api definition

// Outputs are output from Octave
octaveResponse  octave_registerOutput(char * key);
octaveResponse  octave_registerOutput_cbh( define the cbh)

callback
void ___ (char * key, char * value)

octaveResponse  octave_registerInput(char * key, char * value);



I this blocking or none blocking
What about failures?
 


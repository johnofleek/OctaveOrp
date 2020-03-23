# Purpose of this document

To define a C code API for a C code implementation which enables easy integration of the Octave system into basic microcontroller projects 

# Goals

The API should be simple to use
The final code should be as portable as possible

# Description

The final C code will provide a simple method of transacting data with the Octave system.

It will acheive this by embedding the Octave Resource Protocol (ORP) prototol. 

End users will include the code in their project and use the API to 
* Send data to Octave
* Receive unsolicted data from Octave 

# Limitations
It is proposed at this stage to implement a subset of the ORP protocol

# API

// Outputs are output from Octave
octaveResponse  octave_registerOutput(char * key);
octaveResponse  octave_registerOutput_cbh( define the cbh)

callback
void ___ (char * key, char * value)

octaveResponse  octave_registerInput(char * key, char * value);



I this blocking or none blocking
What about failures?
 


// The orp.h - header bit
typedef void (*octave_response_cbf)(char *);

typedef struct 
{
    char *keyString;
    octave_response_cbf userApp_OctaveResponse_cbh
} octaveInput_struct;

uint8_t octave_register_inputs(int8_t *inputKeys, int8_t numberOfInputs);

// orp.c

uint8_t octave_register_inputs(int8_t *inputKeys, int8_t numberOfInputs)
{
  return(0);
}


// The app.c
#define NUMBER_OF_INPUTS 5

void setup() {
  // put your setup code here, to run once:
  // in the app create an array of structs 

  struct octaveInputs_struct[NUMBER_OF_INPUTS];
  
  octave_register_inputs(octaveInput_struct octaveInputs_struct, int8_t numberOfInputs);
}

void loop() {
  // put your main code here, to run repeatedly:

}

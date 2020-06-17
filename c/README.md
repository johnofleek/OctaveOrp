# Octave remote C code project

The C code has been written to be hopefully
1. As portable as possible
2. Target simple microcontrollers
3. RAM resources are passed into the code where possible (no local mallocs or static data)

It is being developed and tested on an Atmel Arduino Mega2560.  
A third party is successfully using the c code with a 16 bit Microchip PIC.   

If you have any ideas or feedback please get in touch


# Current status of Examples
*OctaveOrp/c/AppExamples/ArduinoAvr/PC_mingw32/test_orp_input.c*  
Builds and runs for mingw32 


*OctaveOrp/c/AppExamples/ArduinoAvr/octaveAdcDemoSketch/octaveAdcDemoSketch.ino*  

Builds and runs - is able to send and receive data to/from Octave. 
Sends 8 live ADC values to Octave
Receives one datavalue from Octave


Tested with :  
* Arduino mega2560 +  
* mangOH yellow / Octave edge 2.1.3  / Octave edge 2.1.4

Not yet tested with Octave edge 3.0.0


*OctaveOrp/c/AppExamples/ArduinoAvr/octaveDemoSketch/octaveDemoSketch.ino*  

Builds and runs - is able to send and receive data to/from Octave. 
Sends dummy value to Octave
Receives one datavalue from Octave

Tested with :  
* Arduino mega2560 +  
* mangOH yellow / Octave edge 2.1.3  


## Hardware
This is a list of the hardware used to test the example applications.  

* Arduino Mega 2560
* mangOH yellow WP7702(octave_2.1.3_wp77xx Legato version 19.07 Modem firmware version R12)
* TMP36 analogue output temperature sensor (horrible electrically but works if carefully integrated)
* SEEED Grove - Temperature Sensor V1.2 attached to Arduino ADC 0

## Useful websites
For the cloud data  
https://octave.sierrawireless.io/  
Login to the account where the particular mangOH yellow has been registered  

For the temperature sensor  
http://wiki.seeedstudio.com/Grove-Temperature_Sensor_V1.2/  

For the Arduino mega 2560  
https://www.arduino.cc/en/Guide/ArduinoMega2560  

For the mangOH yellow  
https://mangoh.io/mangoh-yellow  


# Visual Studio Code

## Configuration  
https://code.visualstudio.com/docs/cpp/config-mingw

## Markdown viewer
CTRl SHIFT V

## Hint on making a symlink in powershell
From 
https://docs.microsoft.com/en-us/powershell/module/microsoft.powershell.management/new-item?view=powershell-6


Used this to symlink core c code into arduino or other target project

```
New-Item -ItemType SymbolicLink -Path "Link" -Target "Target"
```
Replace the "Link" portion with the path to the symbolic link you want 
 to create (including the file name and its extension for files)

Replace the "Target" portion with the path (relative or absolute) that
 the new link refers to.

```
New-Item -ItemType SymbolicLink -Path "Link" -Target .
```

Source:  
C:\Users\john\Documents\Arduino\octaveArduinoGH2\c  

Destination:  
C:\Users\john\Documents\Arduino\libraries  



```
cmd /c mklink
Creates a symbolic link.

MKLINK [[/D] | [/H] | [/J]] Link Target

        /D      Creates a directory symbolic link.  Default is a file
                symbolic link.
        /H      Creates a hard link instead of a symbolic link.
        /J      Creates a Directory Junction.
        Link    Specifies the new symbolic link name.
        Target  Specifies the path (relative or absolute) that the new link
                refers to.
```

In my case
```
cmd /c mklink /d .\c C:\Users\john\Documents\Arduino\octaveArduinoGH2\c
```

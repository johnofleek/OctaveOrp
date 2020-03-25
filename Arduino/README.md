# Arduino ORP demo
The first Arduino application reads temperature from a thermistor and sends the temperature value to the Octave cloud.  

The second one reads ADC1 - during testing I attached an TMP36 micro power temperature sensor - transmit the ADC1 value over Octave and then in an Octave cloud action calculate the temperature. There is some info in the docs about the issues with the TMP36 sensor - basically it's a high impedance device and can't be used alone with leads.  

## Hardware
This is a list of the hardware used to test the application.  

* Arduino Mega 2560
* mangOH yellow WP7702 with Octave build Version 2.1.0 (octave_2.1.3_wp77xx Legato version 19.07 Modem firmware version R12)
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



## Arduino software
The Sketch is called sketch_feb09a.ino  

The git repo also includes libraries that are needed to build the application  
* ArduOrp
* SimpleTimer-1.0.0

## Github

Current Github location is  
https://github.com/johnofleek/OctaveOrpPythonDemo

This git contains this Arduino project



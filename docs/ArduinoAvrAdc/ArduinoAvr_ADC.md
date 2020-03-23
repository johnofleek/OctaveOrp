# AVR mega 2560 ADC

A review of issues observed when connecting a mega2560 ADC to a TMP36 temperature sensor

## Datasheets
https://ww1.microchip.com/downloads/en/devicedoc/atmel-2549-8-bit-avr-microcontroller-atmega640-1280-1281-2560-2561_datasheet.pdf

https://www.analog.com/media/en/technical-documentation/data-sheets/TMP35_36_37.pdf

## TMP36

TMP36 design is based on the LM34 which was introduced in 1986.
Using a 100mm cable we observed instability in the output voltage. Initially we thought his was caused by the AVR ADC sampling capacitor but in fact the instability is due to the design of the TMP36. 


* Output impedance is relativily high
* OP load current range is only 0-50uA 
* Stable only with a load capacitance < 10nF 
* Cable lengths > a few 10s of mm cause the LM36 output to be unstable (see scope trace)

Scope trace:
![supposed to be scope trace here](ArduinoAvr_ADC1_LM36.png) "TMP36 100mm cable"

## Conclusion  
The TMP36 needs an analog buffer in order to be able to drive wiring > 100mm

Alternatives
* Thermistor 
   * non-linear response but easily rectifed in software
   * http://www.vishay.com/docs/29056/ntcs0603e3t.pdf
   * lower impedance 5V / 2K = 2.5mA
   * very low cost  
   * industrial versions available   
* PT100 
   * Small change in OP requires more sophisticated ADC
   * industrial versions available
* Digital temperature IC such as 
   * such as http://www.ti.com/sensors/temperature-sensors/digital/products.html
   * Advantage of a very low impendance interface
   * no additional cct needed
   * I2C, UART and 1-wire industrial versions available

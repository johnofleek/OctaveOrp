 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 IN THE SOFTWARE.


# Octave ORP 
The purpose of the repository is to provide resources for remote
 devices connected to Sierra Wireless Octave ORP enabled hardware such as
* mangOH yellow
* FX30S WP7702
* WP7702

The *C version* was developed to support limited resource remote microcontrollers  
  devices such as Microchip AVR and PIC.   

The *Python* remote device folders are present for 2 reasons - 
please note that it is not currently being maintained.
It's probable that Sierra Wireless will update their version over time.
1. Python 2.7 to provided an easy way to access the source 
1. Python 3.x was need for a demo and is based on the 2.7 version

The *JavaScript* folder ./edgeJavaScript (at the time of writing) contains
 some Octave edge device JavaScript.
This may continue if it proves useful. This is executed on the Sierra Wireless edge
 device not the remote microcontroller / device.
 
Anyone interested in the development, support, use ... of this open
 source project please get in contact.

# Octave ORP - C
This is a work in progress. This project targets microcontrollers
 with limited resources.  

The original intention was to complete this
 in C++ but a wider requirement to target microchip PIC (no C++ support)
 has lead to the development of the C project.  

The C project is now working with Arduino AVR mega and PIC microcontrollers.  

Have a look at the C folder for more information and examples of Arduino
 and mingw builds.
 
There is some api documentation in the docs/doxy folder
 

# Octave Orp - Python version
The purpose of this app is to demonstrate and simulate an external 
 computer sending sensor data via a serial port (UART) to the Octave 
 system.  
 
The Python 2.7 code was taken from Sierra Wireless Octave ORP resources.  
The Python 3.x code is based on the 2.7 version but runs on Python 2.7.  

An attempt was made to port the code to micro Python but it was  
 found that a complete rewrite would be needed.

The app was tested on a Windows 10 PC and used a mangOH Yellow / WP7702 
 board as the edge device
 
Python demo runs on serial port connected remote machine

## Python demos
Is emulateSystem.py

```
python emulateSystem.py
```
 
or 

```
emSysPybv.py
```

# Physical / Hardware connections
See the docs folder


# Install 

## Clone this repo

```
git clone https://github.com/johnofleek/OctaveOrpPythonDemo
```

## Python3 version
This is working on a PC - it was developed to investigate the possibility of deploying on micropython. So far this isn't working due to data fragmentation issues


## (Python 2 version) Install Python 2.7
```
https://www.python.org/download/releases/2.7/
```

## Install app dependencies
```
pip install -r requirements.txt
```


## Edit run.py 
Work out which serial port the USB adapter is and edit this line
 (COM8) to suit
```
DEV = os.getenv('DEV', 'COM8')
```

## Run the app from the remote computers (PC)command line

```
python run.py
```

Or what ever version of app you are using

The current app creates the sensor in the Octave system and sends
 one value



## View the running application data flow

The External Python test app is now transacting serial data with
the Octave system 

On the remote computer command line (PC running Python app) the data looks like this

```
C:\Users\john\Documents\Octave\PythonORPclientDemo2\OctaveOrpPythonDemo>python run.py
2019-12-20 16:47:52,440 SbSerial: creating sensor at sensors/set1
2019-12-20 16:47:52,440 SbSerial: sending frame, awaiting s
2019-12-20 16:47:52,525 SbSerial: recv frame: {'status': 'OK', 'responseType': 's'}
2019-12-20 16:47:52,957 SbSerial: checking message {'status': 'OK', 'responseType': 's'}
2019-12-20 16:47:53,158 SbSerial: preparing to send: push json sensors/set1/value {"DissolvedOxygen": 78, "Debris": true, "temperature": 123.2, "flowOut": 11.8, "flowIn": 12.5, "waterLevelLow": false, "Turbidity": 45, "waterLevelHigh": true}
argc: 18
2019-12-20 16:47:53,397 SbSerial: recv frame: {'status': 'OK', 'responseType': 'p'}
loop
```


At the Octave edge (View in Octave web UI https://octave.sierrawireless.io/device/resources )  - note the doesn't work that well in MS edge browser
```
Devices - remote/sensors/set1/value ...
```
 

At the Cloud (View in Octave web UI)
```
{
  "creationDate": 1576860654490,
  "creatorId": "i000000000000000000000001",
  "elems": {
    "remote": {
      "sensors": {
        "set1": {
          "Debris": true,
          "Oxygen": 78,
          "Turbid": 45,
          "flowIn": 12.5,
          "flowOut": 11.8,
          "temperature": 123.2,
          "waterLevelHigh": true,
          "waterLevelLow": false
        }
      }
    }
  },
  "generatedDate": 1576860651477,
  "hash": null,
  "id": "e5dfcfbee9ddc52dee6fxxxxx",
  "lastEditDate": null,
  "location": null,
  "metadata": null,
  "path": "/xxx/devices/mangohyellow/set_1",
  "streamId": "s5dfcf77f0dcf212031feaxxx",
  "tags": {
  }
}
```


# Further Info


## Octave register device

### Config mangOH yellow
Set the switches and connect the internal antenna

* Connect external LTE antenna to MAIN connector  
* Enable external LTE antenna  SW403-5 to ON
* MAIN power switch ON


For Windows host machines Load Sierra driver for WP7702 from https://source.sierrawireless.com/

Power MangOH yellow via USB socket in narrow side of the board

Over AT command port use ati command to retrieve FSN and IMEI

Go to the octave 
https://octave.sierrawireless.io/

Add the device

ssh into the mangOH
After a while attempt a data connection - this seems to speed up the
 provisioning process

```
cm data connect
``` 

This most likely will fail but the Octave web UI probably now indicates
 a provisioned device

## useful commands
SIM ID  
```
at+ccid
+CCID: 89332401000010876791,89033240817620069918070003435626
```

IMEI  
```
at+cgsn
352653090215245
```

Serial number == FSN 
IMEI == IMEI 
```
ati
Manufacturer: Sierra Wireless, Incorporated
Model: WP7702
Revision: SWI9X06Y_02.32.02.00 c2e98c jenkins 2019/08/30 07:28:21
IMEI: 352653090215245
IMEI SV:  4
FSN: 4L936370140910
+GCAP: +CGSM,+DS
```


## config the octave device from octave backend
set the uart to ORP
https://octave.sierrawireless.io/device/services/remote/UART1



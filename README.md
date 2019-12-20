# OctaveOrpPythonDemo
The purpose of this app is to demonstrate and simulate an external 
 computer sending sensor data via a serial port (UART) to the Octave 
 system

The app was tested on a Windows 10 PC and used a mangOH Yellow / WP7702 
 board as the edge device
 
Python demo runs on serial port connected remote machine
 
 
# Install 

## Clone this repo

```
git clone https://github.com/johnofleek/OctaveOrpPythonDemo
```

## Install Python 2.7
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

The current app creates the sensor in the Octave system and sends
 one value



## View the running application data flow

The External Python test app is now transacting serial data with
the Octave (remote?) app.

https://docs.octave.dev/references/edge/octave_resource_protocol/#source-code-for-the-demo-application-for-southbound-resource-protocol


On the remote computer command line (PC running Python app)
```
2019-12-19 15:25:15,980 SbSerial: preparing to send: push num system/cpu/value 0.0 
```
 

At the Octave edge (View in Octave web UI)
```
Devices - remote/system/cpu/value 0.0 
```
 

At the Cloud (View in Octave web UI)
```
{
  "creationDate": 1576769712182,
  "creatorId": "i000000000000000000000001",
  "elems": {
    "remote": {
      "system": {
        "cpu": 0
      }
    }
  },
  "generatedDate": 1576769712179,
  "hash": null,
  "id": "e5dfb98b06079f3b7d6caaa94",
  "lastEditDate": null,
  "location": null,
  "metadata": null,
  "path": "/linkwave_technologies/devices/macdui_1/cpu",
  "streamId": "s5dfb987c742183f467ab0832",
  "tags": {
  }
}
```


# Further Info

## Octave register device (not reqd EPAS I've done it)

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






## Stock product
```
ati8
Legato Ver: 19.07.0_c8105fef6769b17540226029a5d23d44
Yocto Ver:  SWI9X06Y_02.32.02.00 2019-10-08_01:04:54
OS Ver: Linux version 3.18.140 (oe-user@oe-host) (gcc version 7.3.0 (GCC) ) #1 PREEMPT Tue Oct 8 00:58:07 UTC 2019
LK Ver: SWI9X06Y_02.22.12.00
RootFS Ver: SWI9X06Y_02.32.02.00 2019-10-08_01:04:54
UserFS Ver: unknown
MCU Ver: 002.013
```

```
root@swi-mdm9x28-wp:~# cm info
Device:                        WP7702
IMEI:                          352653090215245
IMEISV:                        4
FSN:                           4L936370140910
Firmware Version:              SWI9X06Y_02.32.02.00 c2e98c jenkins 2019/08/30 07:28:21
Bootloader Version:            SWI9X06Y_02.32.02.00 c2e98c jenkins 2019/08/30 07:28:21
MCU Version:                   002.013
PRI Part Number (PN):          9909138
PRI Revision:                  001.001
Carrier PRI Name:              SIERRA
Carrier PRI Revision:          001.027_000
SKU:                           1104405
Last Reset Cause:              Power Down
Resets Count:                  Expected: 9      Unexpected: 0
```



## actual image used for demo

Why? - because the default doesn't support ORP 

Image is 
```
module-2.1.0.wp77xx.spk
```

Programming use windows powershell with this 

```
ls 

Mode                LastWriteTime         Length Name
----                -------------         ------ ----
-a----       28/02/2019     11:09         664056 fdt2.exe
-a----       28/02/2019     10:08        1583608 GobiApi.dll
-a----       19/12/2019     14:57       64543870 module-2.1.0.wp77xx.spk
```
and program mangOH like this

```
./fdt2.exe module-2.1.0.wp77xx.spk
```

The ORP app causes a Legato restart the first time ever - there is a delay while the backend syncs - after that it works


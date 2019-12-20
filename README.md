# OctaveOrpPythonDemo
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
Work out which serial port the USB adapter is and edit this line (COM8) to suit
```
DEV = os.getenv('DEV', 'COM8')
```

## run the app from the command line

```
python run.py
```

The current app creates the sensor in the Octave system and sends one value



## the running app

The External Python test app is now transacting serial data with the Octave (remote?) app.

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
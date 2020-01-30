# emulateSystem.py 
#  https://docs.octave.dev/references/edge/octave_resource_protocol/#octave_rppy
# This is a crude emulation of the data that an external micro transacts with OctaveRP
#   There is micro --> Octave data and Octave <-- micro data
#   The data payloads are in JSON format - this may be tricky for the micro to handle
## note on data types - the following types are supported
'''
'trig'  ## not sure
'bool'  ## not sure if this is JSON true / false or Python True / False 
'num'   ## Float or Integer - watch out for rounding issues if using python maths 
           e.g.
             0.1 + 0.2
             0.30000000000000004
           this is not a fault it's just a consequence of using binary to represent decimal
           to avoid this it might be safer to send data as strings or json strings
'str'   ## strings
'json'  ## strings in json format
'''

import random

random.seed()

from json import dumps
# import logging
# from platform import platform
# from psutil import cpu_percent, virtual_memory
try:
    from serial import Serial
    import os
    #DEV = os.getenv('DEV', '/dev/ttyS0')
    DEV = os.getenv('DEV', 'COM11')
    print("using serial")
except:
    from serialM import Serial
    DEV = "aPort"
    print("using pybv serial")
    
from time import sleep

import gc
from octave_rp import OctaveRP 
from octave_rp import Output
from octave_rp import Input
from octave_rp import Sensor
 
 

# Create serial object and use it to create SbSerial connection
s = Serial(DEV)
orp = OctaveRP(s)

# Global data for simplicity
# note python dict has bool True/False JSON true/False

measurementData = {
    "temperature": 123.2,
    "waterLevelHigh": True,
    "waterLevelLow": False,
    "Turbidity": 45,
    "DissolvedOxygen": 78,
    "Debris": True,
    "flowIn": 12.5,
    "flowOut": 11.8
}

settingsData = {
    "Fog_roller_OFF":300,
    "Fog_roller_ON":300,
    "Recirculation_OFF":90,
    "Recirculation_ON":10,
    "Service_pump_OFF":2550,
    "Service_pump_ON":90,
    "Solenoid_valve_OFF":2550,
    "Solenoid_valve_ON":120,
    "cleaningCycles":3,
    "unitOFF":2,
    "unitON":7
}

# Send measurement data from the micro to Octave
def json_measurement_handler():
    # return dictionary converted to JSON
    return dumps(measurementData)
#    
def json_settings_handler():
    # return dictionary converted to JSON
    return dumps(settingsData)
    

# Create sensor objects with SbSerial connection, callback, type, resource path, and optionally unit of measure
## this example no sensors
## sensors = [
##    Sensor(orp, json_measurement_handler, 'json', 'sensors/uplinkMeasured'),
##    Sensor(orp, json_settings_handler, 'json', 'sensors/controlsettingsUp')
## ]


#[sensor.create_sensor() for sensor in sensors]

## this callback handler is called when Octave cloud sends some data that matches 'sensors/controlsettingsdownj'
def print_output(data):
    print (data)

# mimic the settings dictionary but setting values from the cloud
## Register callback handler with Octave
output = Output(orp, print_output, 'json', 'sensors/controlsettingsdownj')
output.create_output()



'''
measurementData = {
    "temperature": 123.2,
    "waterLevelHigh": True,
    "waterLevelLow": False,
    "Turbidity": 45,
    "DissolvedOxygen": 78,
    "Debris": True,
    "flowIn": 12.5,
    "flowOut": 11.8
    "Fog_roller_OFF":300,
    "Fog_roller_ON":300,
    "Recirculation_OFF":90,
    "Recirculation_ON":10,
    "Service_pump_OFF":2550,
    "Service_pump_ON":90,
    "Solenoid_valve_OFF":2550,
    "Solenoid_valve_ON":120,
    "cleaningCycles":3,
    "unitOFF":2,
    "unitON":7
}
'''

up={}   # dictionary
        # note it seems paths must be alpha chars only

# create the Octave inputs
up["temperature"]    = Input(orp, 'num' , 'sense/temperature', 'number') # we might want to simplify the text
up["waterLevelHigh"] = Input(orp, 'bool', 'sense/waterLevelHigh','bool')
up["waterLevelLow"]  = Input(orp, 'bool', 'sense/waterLevelLow','bool')
up["Turbidity"]      = Input(orp, 'num' , 'sense/Turbidity','number')
up["DissolvedOxygen"]= Input(orp, 'num' , 'sense/DissolvedOxygen','number')
up["Debris"]         = Input(orp, 'bool', 'sense/Debris','bool')
up["flowIn"]         = Input(orp, 'num' , 'sense/flowIn','number')
up["flowOut"]        = Input(orp, 'num' , 'sense/flowOut','number')

for key in up:
    print("Creating", key)
    up[key].create_input()

turbidity = 55
DissolvedOxygen = 22


# send values to Octave
up["temperature"].send(42.2) 
up["waterLevelHigh"].send(True)
up["waterLevelLow"].send(True)
up["Turbidity"].send(turbidity)
up["DissolvedOxygen"].send(DissolvedOxygen)
up["Debris"].send(True)   
up["flowIn"].send(11.2)   
up["flowOut"].send(23)     



# Run Forever
while True:
    try:
        sleep(5)
        turbidity = random.randrange(20,50,1)
        DissolvedOxygen = random.randrange(40,60,1)
       
        up["Turbidity"].send(turbidity)
        up["DissolvedOxygen"].send(DissolvedOxygen)
        
    except KeyboardInterrupt:
        exit(0)
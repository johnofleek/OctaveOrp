# emulateSystem.py 
#  https://docs.octave.dev/references/edge/octave_resource_protocol/#octave_rppy
# This is a crude emulation of the data that an external micro transacts with OctaveRP
#   There is micro --> Octave data and Octave <-- micro data
#   The data payloads are in JSON format - this may be tricky for the micro to handle


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
gc.collect()

from octave_rp import OctaveRP 
gc.collect()
from octave_rp import Output
gc.collect()
from octave_rp import Input
gc.collect()
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
    

# Create sensor objects with SbSerial connection, callback, type,
# resource path, and optionally unit of measure
sensors = [
    Sensor(orp, json_measurement_handler, 'json', 'sensors/uplinkMeasured'),
    Sensor(orp, json_settings_handler, 'json', 'sensors/controlsettingsUp')
]

[sensor.create_sensor() for sensor in sensors]

def print_output(data):
    print (data)

# mimic the settings dictionary but setting values from the cloud
output = Output(orp, print_output, 'json', 'sensors/controlsettingsdownj')
output.create_output()

#push_type_input = Input(orp, 'num', 'push_type_input', 'number')
#push_type_input.create_input()

#push_counter = 0
#push_type_input.send(push_counter)

# Run Forever
while True:
    try:
        sleep(1)
        
        #push_type_input.send(push_counter)
        #push_counter += 1
        
    except KeyboardInterrupt:
        exit(0)
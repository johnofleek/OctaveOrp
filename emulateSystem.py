# sample.py 
# from https://docs.octave.dev/references/edge/octave_resource_protocol/#octave_rppy

import os
from json import dumps
import logging
from platform import platform
from psutil import cpu_percent, virtual_memory
from serial import Serial
from time import sleep
from octave_rp import OctaveRP, Output, Input, Sensor


#DEV = os.getenv('DEV', '/dev/ttyS0')
DEV = os.getenv('DEV', 'COM11') 

# Create serial object and use it to create SbSerial connection
s = Serial(DEV)
orp = OctaveRP(s)

#
def json_measurement_handler():
    # note python dict has bool True/False JSON true/False
    dictPayload = {
        "temperature": 123.2,
        "waterLevelHigh": True,
        "waterLevelLow": False,
        "Turbidity": 45,
        "DissolvedOxygen": 78,
        "Debris": True,
        "flowIn": 12.5,
        "flowOut": 11.8
    }
    # return dictionary convert to JSON
    return dumps(dictPayload)
#    
def json_settings_handler():
    # note python dict has bool True/False JSON true/False
    dictPayload = {
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
    return dumps(dictPayload)
    

# Create sensor objects with SbSerial connection, callback, type,
# resource path, and optionally unit of measure
sensors = [
    Sensor(orp, json_measurement_handler, 'json', 'sensors/uplinkMeasured'),
    Sensor(orp, json_settings_handler, 'json', 'sensors/controlsettingsUp')
]

[sensor.create_sensor() for sensor in sensors]

def print_output(data):
    print data

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
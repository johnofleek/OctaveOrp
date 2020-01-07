import os
from json import dumps
import logging
from platform import platform
from psutil import cpu_percent, virtual_memory
from serial import Serial
from time import sleep
from sb_serial import Sensor, SbSerial

# Change the serial port to suit the machine that this running on 
# and the OS
#DEV = os.getenv('DEV', '/dev/ttyS0')
#DEV = os.getenv('DEV', '/dev/tty.SLAB_USBtoUART')
DEV = os.getenv('DEV', 'COM11')  

# Create serial object and use it to create SbSerial connection
s = Serial(DEV)
sbs = SbSerial(s)



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
    # return dictionary convert to JSON
    return dumps(dictPayload)

# Create sensor objects with SbSerial connection, callback, type,
# resource path, and optionally unit of measure


sensors = [
    Sensor(sbs, json_measurement_handler, 'json', 'sensors/uplinkMeasured'),
    Sensor(sbs, json_settings_handler, 'json', 'sensors/controlSettings')
]

[sensor.create_sensor() for sensor in sensors]

# Run Forever
while True:
    try:
        sleep(10)
        print("loop")
    except KeyboardInterrupt:
        exit(0)
        
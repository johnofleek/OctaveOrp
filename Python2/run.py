import os
from json import dumps
import logging
from platform import platform
# from psutil import cpu_percent, virtual_memory
from serial import Serial
from time import sleep
from sb_serial import Sensor, SbSerial

# Change the serial port to suit the machine that this running on 
# and the OS
#DEV = os.getenv('DEV', '/dev/ttyS0')
#DEV = os.getenv('DEV', '/dev/tty.SLAB_USBtoUART')
DEV = os.getenv('DEV', 'COM1')  

# Create serial object and use it to create SbSerial connection
s = Serial(DEV)
sbs = SbSerial(s)

# Handles request for platform resource
def string_platform_handler():
    return platform()

# Handles request for JSON resource
def json_vmem_handler():
    mem = virtual_memory()
    return dumps({ 'total': mem.total, 'available': mem.available })


def json_set1_handler():
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

# Create sensor objects with SbSerial connection, callback, type,
# resource path, and optionally unit of measure
'''
sensors = [
    Sensor(sbs, string_platform_handler, 'string', 'system/platform', 'alphabet'),
    Sensor(sbs, cpu_percent, 'num', 'system/cpu', 'percent'),
    Sensor(sbs, json_vmem_handler, 'json', 'system/mem')
]
'''

sensors = [
    Sensor(sbs, json_set1_handler, 'json', 'sensors/set1')
]

[sensor.create_sensor() for sensor in sensors]

# Run Forever
while True:
    try:
        sleep(10)
        print("loop")
    except KeyboardInterrupt:
        exit(0)
        
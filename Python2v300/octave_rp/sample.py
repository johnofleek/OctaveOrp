import os
from json import dumps
import logging
from platform import platform
from psutil import cpu_percent, virtual_memory
from serial import Serial
from time import sleep
from octave_rp import OctaveRP, Output, Input, Sensor

DEV = os.getenv('DEV', '/dev/ttyS0')

# Create serial object and use it to create SbSerial connection
s = Serial(DEV)
orp = OctaveRP(s)

# Handles request for platform resource
def string_platform_handler():
    return platform()

# Handles request for JSON resource
def json_vmem_handler():
    mem = virtual_memory()
    return dumps({ 'total': mem.total, 'available': mem.available, 'active': mem.active })

# Create sensor objects with SbSerial connection, callback, type,
# resource path, and optionally unit of measure
sensors = [
    Sensor(orp, string_platform_handler, 'string', 'system/platform', 'alphabet'),
    Sensor(orp, cpu_percent, 'num', 'system/cpu', 'percent'),
    Sensor(orp, json_vmem_handler, 'json', 'system/mem')
]

[sensor.create_sensor() for sensor in sensors]

def print_output(data):
    print '*' * 20
    print '* %s' % data
    print '*' * 20

output = Output(orp, print_output, 'string', 'external_status/lcd1', 'alphabet')
output.create_output()

push_type_input = Input(orp, 'num', 'push_type_input', 'number')
push_type_input.create_input()

push_counter = 0
push_type_input.send(push_counter)

# Run Forever
while True:
    try:
        sleep(1)
        push_type_input.send(push_counter)
        push_counter += 1
    except KeyboardInterrupt:
        exit(0)
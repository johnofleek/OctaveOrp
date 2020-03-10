import os
from json import dumps
import logging
from platform import platform

from serial import Serial
from time import sleep
from sb_serial import Sensor, SbSerial

from octave_rp import OctaveRP 
from octave_rp import Output
from octave_rp import Input
from octave_rp import Sensor

DEV = os.getenv('DEV', 'COM1')  


# Create serial object and use it to create SbSerial connection
s = Serial(DEV)
orp = OctaveRP(s)



# Handles request for platform resource
def string_platform_handler():
    return platform()


up={}   # dictionary

up["waterLevelHigh"] = Input(orp, 'bool', 'val/wl','bool')
up["temp"] = Input(orp, 'num', 'val/tp','num')

for key in up:
    print("Creating", key)
    up[key].create_input()

up["waterLevelHigh"].send(True)
up["temp"].send(23.7)
    
# Run Forever
while True:
    try:
        sleep(10)
        print("loop")
    except KeyboardInterrupt:
        exit(0)
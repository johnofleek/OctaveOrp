# simpleData.py 
#  https://docs.octave.dev/references/edge/octave_resource_protocol/#octave_rppy
# This is a crude emulation of the data that an external micro transacts with OctaveRP
#   There is micro --> Octave data and Octave <-- micro data
#   The data payloads are in JSON format - this may be tricky for the micro to handle
## note on data types - the following types are supported
'''
'trig'  ## not sure
'bool'  ## Python True / False 
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
    DEV = os.getenv('DEV', 'COM13')
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




downPath = "dwn/"
Recirculation_OFF_str = "rof"

## this callback handler is called when Octave cloud sends some data that matches 'sensors/controlsettingsdownj'
def orp_push_cbh(data):
    print ("orp_push_cbh", data)

# mimic the settings dictionary but setting values from the cloud
## Register callback handler with Octave
octaveOp = Output(orp, orp_push_cbh, 'str', ('txtstr'))
octaveOp.create_output()



'''
## this callback handler is called when Octave cloud sends some data that matches 'sensors/controlsettingsdownj'
def Recirculation_ON_cbh(data):
    print ("Recirculation_ON_cbh", data)

# mimic the settings dictionary but setting values from the cloud
## Register callback handler with Octave
Recirculation_ON_octave = Output(orp, Recirculation_ON_cbh, 'num', (downPath + Recirculation_ON_str))
Recirculation_ON_octave.create_output()
'''


# Run Forever
while True:
    try:
        sleep(0.1)
        
    except KeyboardInterrupt:
        exit(0)
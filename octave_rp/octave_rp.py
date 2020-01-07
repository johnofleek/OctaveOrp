# octave_rp.py

from sbr_Protocol_v1 import * 
from serial import Serial
from simple_hdlc import HDLC
from time import sleep
import logging

logging.basicConfig(format='%(asctime)s %(name)s: %(message)s')
logger = logging.getLogger('OctaveRP')
logger.setLevel(logging.DEBUG)

class OctaveRP(HDLC):

    def __init__(self, serial, retry_timeout=0.5):
        HDLC.__init__(self, serial)
        self.serial = serial
        self.messages = []
        self.sensor_handlers = {}
        self.output_handlers = {}
        self.retry_timeout = retry_timeout
        self.startReader(onFrame=self.rcv_frame)

    def _wakeup(self):
        self.serial.write("~")
        sleep(0.1)
        self.serial.write("~")
        sleep(0.1)
        self.serial.write("~")

    def execute_wait_confirm(self, frame, resp):
        self.clear()
        self._wakeup()
        confirmed = False
        while confirmed == False:
            logger.debug("sending frame, awaiting %s" % resp)
            self.sendFrame(frame)
            sleep(self.retry_timeout)
            for m in self.messages:
                logger.debug('checking message %s' % m)
                if m['responseType'] == resp:
                    confirmed = True

    def push_value(self, path, datatype=None, value=None):
        if value is None:
            value = self.sensor_handlers[path][1]()
            datatype = self.sensor_handlers[path][0]
        cmd = 'push %s %s %s' % (datatype, '%s/value' % path, value)
        logger.debug('preparing to send: %s' % cmd)
        frame = encode_request(cmd)
        self._wakeup()
        self.sendFrame(frame)

    def create_input(self, dataType, path, unit):
        if unit:
            frame = encode_request('create input %s %s/value %s' % (dataType, path, unit))
        else:
            frame = encode_request('create input %s %s' % (dataType, path))
        logger.debug('creating input at %s' % path)
        self.execute_wait_confirm(frame, SBR_PKT_RESP_INPUT_CREATE)

    def create_sensor(self, dataType, path, unit, handler):
        if unit:
            frame = encode_request('create sensor %s %s %s' % (dataType, path, unit))
        else:
            frame = encode_request('create sensor %s %s' % (dataType, path))
        logger.debug('creating sensor at %s' % path)
        self.execute_wait_confirm(frame, SBR_PKT_RESP_SENSOR_CREATE)
        self.sensor_handlers[path] = (dataType, handler)
        self.push_value(path)

    def create_output(self, dataType, path, unit, handler):
        self.output_handlers[path] = handler

        if unit:
            frame = encode_request('create output %s %s %s' % (dataType, path, unit))
        else:
            frame = encode_request('create output %s %s' % (dataType, path))

        # create output
        logger.debug('creating output at %s' % path)
        self.execute_wait_confirm(frame, SBR_PKT_RESP_OUTPUT_CREATE)

        # register handler
        frame = encode_request('add handler %s' % path)
        self.execute_wait_confirm(frame, SBR_PKT_RESP_HANDLER_ADD)

    def clear(self):
        self.messages = []

    def rcv_frame(self, data):
        message = decode_response(data)
        logger.debug('recv frame: %s' % message)
        if message['responseType'] == SBR_PKT_NTFY_SENSOR_CALL:
            if message['path'] in self.sensor_handlers.keys():
                self.push_value(message['path'])
        elif message['responseType'] == SBR_PKT_NTFY_HANDLER_CALL:
            logger.debug('new data at %s' % message['path'])
            if message['path'] in self.output_handlers.keys():
                self.output_handlers[message['path']](message['data'])
        else:
            self.messages.append(message)

class Sensor(object):

    def __init__(self, orp, handler, datatype, path, unit=None):
        self.datatype = datatype
        self.path = path
        self.unit = unit
        self.orp = orp
        self.handler = handler
        self.declared = False

    def create_sensor(self):
        self.orp.create_sensor(self.datatype, self.path, self.unit, self.handler)
        self.declared = True

class Output(object):

    def __init__(self, orp, handler, datatype, path, unit=None):
        self.datatype = datatype
        self.path = path
        self.unit = unit
        self.orp = orp
        self.handler = handler
        self.declared = False

    def create_output(self):
        self.orp.create_output(self.datatype, self.path, self.unit, self.handler)
        self.declared = True

class Input(object):

    def __init__(self, orp, datatype, path, unit=None):
        self.datatype = datatype
        self.path = path
        self.unit = unit
        self.orp = orp
        self.declared = False

    def create_input(self):
        self.orp.create_input(self.datatype, self.path, self.unit)

    def send(self, value):
        self.orp.push_value(self.path, self.datatype, value)
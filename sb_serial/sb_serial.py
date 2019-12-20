from sbr_Protocol_v1 import *
from serial import Serial
from simple_hdlc import HDLC
from time import sleep
import logging

logging.basicConfig(format='%(asctime)s %(name)s: %(message)s')
logger = logging.getLogger('SbSerial')
logger.setLevel(logging.DEBUG)

class SbSerial(HDLC):

    def __init__(self, serial, retry_timeout=0.5):
        HDLC.__init__(self, serial)
        self.serial = serial
        self.messages = []
        self.sensor_handlers = {}
        self.output_handlers = {}
        self.retry_timeout = retry_timeout
        self.startReader(onFrame=self.rcv_frame)

    def execute_wait_confirm(self, frame, resp):
        self.clear()
        self.serial.write("~~~~~~~~~~~~")
        confirmed = False
        while confirmed == False:
            logger.debug("sending frame, awaiting %s" % resp)
            self.sendFrame(frame)
            sleep(self.retry_timeout)
            for m in self.messages:
                logger.debug('checking message %s' % m)
                if m['responseType'] == resp:
                    confirmed = True

    def push_value(self, path):
        value = self.sensor_handlers[path][1]()
        cmd = 'push %s %s %s' % (self.sensor_handlers[path][0],
            '%s/value' % path,
            self.sensor_handlers[path][1]())
        logger.debug('preparing to send: %s' % cmd)
        frame = encode_request(cmd)
        self.sendFrame(frame)
        #self.execute_wait_confirm(frame, SBR_PKT_RESP_PUSH)

    def create_sensor(self, dataType, path, unit, handler):
        if unit:
            frame = encode_request('create sensor %s %s %s' % (dataType, path, unit))
        else:
            frame = encode_request('create sensor %s %s' % (dataType, path))
        logger.debug('creating sensor at %s' % path)
        self.execute_wait_confirm(frame, SBR_PKT_RESP_SENSOR_CREATE)
        self.sensor_handlers[path] = (dataType, handler)
        sleep(0.2)
        self.push_value(path)

    def clear(self):
        self.messages = []

    def rcv_frame(self, data):
        message = decode_response(data)
        logger.debug('recv frame: %s' % message)
        if message['responseType'] == SBR_PKT_NTFY_SENSOR_CALL:
            if message['path'] in self.sensor_handlers.keys():
                self.push_value(message['path'])
        elif message['responseType'] == SBR_PKT_NTFY_SENSOR_CALL:
            logger.debug('new data at %s' % message['path'])
        else:
            self.messages.append(message)

class Sensor(object):

    def __init__(self, sbs, handler, dataType, path, unit=None):
        self.dataType = dataType
        self.path = path
        self.unit = unit
        self.sbs = sbs
        self.handler = handler
        self.declared = False

    def create_sensor(self):
        self.sbs.create_sensor(self.dataType, self.path, self.unit, self.handler)
        self.declared = True
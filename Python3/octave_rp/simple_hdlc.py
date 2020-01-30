#!/usr/bin/python
# coding: utf8

__version__ = '0.2'


import struct
import time
import gc

threadingType = "none"
try:
    from threading import Thread
    threadingType = "threading"
    print("using Normal Thread")
except:                         # Micropython
    import _thread
    threadingType = "_thread"
    print("using _thread")
    
print("import CRCCCITT")
from PyCRC.CRCCCITT import CRCCCITT

print("import binascii")
import binascii


try:
    import logging
    print("using proper logging")
except:                 # oh no its micropython
    class Logger:
        DEBUG = 10
        def isEnabledFor(self, _):
            return False
        def debug(self, msg, *args):
            print('debug',msg,args)
            
        def warning(self, msg, *args):
            print('warning',msg,args)
            
        def info(self, msg, *args):
            print('info',msg,args)
            
        def getLogger(self, name):
            return Logger()            
    logging = Logger()
    print("using John's logging")

        
logger = logging.getLogger(__name__)
logFile = logging.getLogger('file')
#logFile.basicConfig(format='%(asctime)s %(message)s', level=logging.DEBUG, filename='serial.log')
logFile.debug("AppStart");

logger.setLevel(logging.DEBUG)
logger.debug("simple_hdlc")

# initialise the crc table once    
crcccitt = CRCCCITT("FFFF")

def calcCRC(data):
    # print("calcCRC(data) in ", data)
    # crc = CRCCCITT("FFFF").calculate(data) # original inits crc every time
    crc = crcccitt.calculate(data)
    # print ("calcCRC -> crc ", crc )
    b = bytearray(struct.pack(">H", crc))
    return b

class Frame(object):
    STATE_READ = 0x01
    STATE_ESCAPE = 0x02

    def __init__(self):
        self.finished = False
        self.error = False
        self.state = self.STATE_READ
        self.data = bytearray()
        self.crc = bytearray()
        self.reader = None

    def __len__(self):
        return len(self.data)

    def addByte(self, b):
        if b == 0x7D:
            self.state = self.STATE_ESCAPE
        elif self.state == self.STATE_ESCAPE:
            self.state = self.STATE_READ
            b = b ^ 0x20
            self.data.append(b)
        else:
            self.data.append(b)

    def finish(self):
        self.crc = self.data[-2:]
        self.data = self.data[:-2]
        self.finished = True

    def checkCRC(self):
        # print("simple_hdlc - Frame - checkCRC()", self.data)
        res = bool(self.crc == calcCRC(bytes(self.data))) ## changed to this due to Python 3
        if not res:
            c1 = str(self.crc)
            c2 = str(calcCRC(self.data))
            logger.warning("invalid crc %s != %s", c1.encode("hex"), c2.encode("hex"))
            self.error = True
        return res

    def toString(self):
        # return str(self.data)
        return (self.data.decode("utf-8"))


class HDLC(object):
    def __init__(self, serial):
        self.serial = serial
        self.current_frame = None
        self.last_frame = None
        self.frame_callback = None
        self.error_callback = None
        self.running = False

    @classmethod
    def toBytes(cls, data):
        # return bytearray(data)
        return (data.encode())
        
    def sendFrame(self, data):
        bs = self._encode(self.toBytes(data))
        logger.info("Sending Frame: %s", bs)
        res = self.serial.write(bs)
        # logger.info("Send %s bytes", res)
    
    def _onFrame(self, frame):
        self.last_frame = frame
        s = self.last_frame.toString()
        #logger.info("Received Frame: " +  s)
        if self.frame_callback is not None:
            self.frame_callback(s)

    def _onError(self, frame):
        self.last_frame = frame
        s = self.last_frame.toString()
        logger.warning("Frame Error: %s", s.encode("hex"))
        if self.error_callback is not None:
            self.error_callback(s)

    def _readBytes(self, size):
        while size > 0:
            b = self.serial.read(1)
            #if b < 1:
            #    return False
            res = self._readByte(b[0])
            if res:
                return True

    def _readByte(self, b):
        assert 0 <= b <= 255
        if b == 0x7E:
            # Start or End
            if not self.current_frame or len(self.current_frame) < 1:
                # Start
                self.current_frame = Frame()
            else:
                # End
                self.current_frame.finish()
                self.current_frame.checkCRC()
        elif self.current_frame is None:
            # Ignore before Start
            return False
        elif not self.current_frame.finished:
            self.current_frame.addByte(b)
        else:
            # Ignore Bytes
            pass

        # Validate and return
        if self.current_frame.finished and not self.current_frame.error:
            # Success
            self._onFrame(self.current_frame)
            self.current_frame = None
            return True
        elif self.current_frame.finished:
            # Error
            self._onError(self.current_frame)
            self.current_frame = None
            return True
        return False

    def readFrame(self, timeout=5):
        timer = time.time() + timeout
        while time.time() < timer:
            i = self.serial.in_waiting
            if i < 1:
                time.sleep(0.0001)
                continue

            res = self._readBytes(i)

            if res:
                # Validate and return
                if not self.last_frame.error:
                    # Success
                    s = self.last_frame.toString()
                    return s
                elif self.last_frame.finished:
                    # Error
                    raise ValueError("Invalid Frame (CRC FAIL)")
        raise RuntimeError("readFrame timeout")

    @classmethod
    def _encode(cls, bs):
        # print("_encode()",  bs)
        data = bytearray()
        data.append(0x7E)
        crc = calcCRC(bs)
        bs += crc
        for byte in bs:
            if byte == 0x7E or byte == 0x7D:
                data.append(0x7D)
                data.append(byte ^ 0x20)
            else:
                data.append(byte)
        data.append(0x7E)
        return bytes(data)

    def _receiveLoop(self):
        while self.running:
            i = self.serial.in_waiting
            if i < 1:
                time.sleep(0.001)
                continue
            res = self._readBytes(i)

    def startReader(self, onFrame, onError=None):
        self.frame_callback = onFrame
        self.error_callback = onError
           
        if self.running:
                raise RuntimeError("reader already running")
                
        if( threadingType == "threading" ):
            self.reader = Thread(target=self._receiveLoop)
            self.reader.setDaemon(True) 
            self.running = True
            self.reader.start()
        else:
            #try:
           
            _thread.start_new_thread( self._receiveLoop, ())
            #except:
            #    print ("Error: startReader new thread failed")
           

    def stopReader(self):
        if( threadingType == "threading" ):
            self.reader.join() 
        else:
            _thread.exit()
            
        self.running = False
        self.reader = None
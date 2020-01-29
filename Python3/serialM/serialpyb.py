#
# limited pyserial emulation for micropython
#
'''
The physical pins of the UART busses are:

UART(4) is on XA: (TX, RX) = (X1, X2) = (PA0, PA1)
UART(1) is on XB: (TX, RX) = (X9, X10) = (PB6, PB7)
UART(6) is on YA: (TX, RX) = (Y1, Y2) = (PC6, PC7)
UART(3) is on YB: (TX, RX) = (Y9, Y10) = (PB10, PB11)
UART(2) is on: (TX, RX) = (X3, X4) = (PA2, PA3)
'''

from pyb import UART


class Serial:

    def __init__(self, name, port=1, baudrate=9600, timeout=None, **kwargs):
        self.port = port
        self.baudrate = baudrate
        self.timeout = -1 if timeout is None else timeout * 1000
        self.uart = UART(self.port)
        self.open()

    def open(self):
        self.uart.init( self.baudrate)

    def close(self):
        self.uart.deinit()

    # Return the number of bytes in the receive buffer
    def inWaiting(self):
        return self.uart.any()

    def write(self, data):
        return self.uart.write(data)

    def read(self, size=1):
        return self.uart.read(size) 
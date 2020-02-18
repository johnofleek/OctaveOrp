
from CRCCCITT import CRCCCITT

crcccitt = CRCCCITT("FFFF")

data = "SF675"

crc = crcccitt.calculate(data)

print (crc)
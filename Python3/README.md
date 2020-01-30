# simpleData.py
Is a remote device emulator written in Python  
Has been tested in Windows 10 Python 3

[Remote] <--> [WP7702 Octave Edge] <--> [Octave cloud]

Sends data from remote->Octave cloud
```
sense/temperature
sense/waterLevelHigh
sense/waterLevelLow
sense/Turbidity
sense/DissolvedOxygen
sense/Debris
sense/flowIn
sense/flowOut
```


Sends data from Octave cloud--> remote
```
dwn/rof
dwn/ron
```

At the Octave cloud web gui look for Resources - remote

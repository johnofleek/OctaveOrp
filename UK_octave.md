2020-03-15
There seems to be an issue in the UK with the networks
See - https://forum.sierrawireless.com/t/octave-uk-radio-registration-problem/19746

Sometimes times on boot the module fails to network register cycling between

at+creg?
+CREG: 0,3

OK
at+creg?
+CREG: 0,2

OK
Which I think means
image

When returning +CREG: 0,3 I also see

root@swi-mdm9x28-wp:~# cm radio
Power:                         ON
Current Network Operator:      Sierra Wireless
Current RAT:                   Module not registered on network, RAT not available
Status:                        Not registered but currently searching for a new operator (LE_MRC_REG_SEARCHING)
Signal:                        Strong signal strength (4)
PS:                            Packet Switched Not registered (LE_MRC_REG_NONE)


My theory is this an NBIOT issue if I

```
at!entercnd="A710"
OK
AT!SELACQ?
!SELACQ:
LTE
LTE-M1
LTE-NB1
GSM

OK
AT!SELCIOT?
Current Operating Mode: 0x2

Supported Operating Modes:
LTE WB:  0x01
LTE M1:  0x02
LTE NB1: 0x04
OK
AT!SELCIOT=2
OK
AT!SELACQ=GSM,LTE-M1
OK
```

Wait a while

```
at+creg?
+CREG: 0,5
```

OK
and

```
root@swi-mdm9x28-wp:~# cm radio
Power:                         ON
Current Network Operator:      Sierra Wireless
Current RAT:                   GSM network (LE_MRC_RAT_GSM)
Status:                        Registered to a roaming network (LE_MRC_REG_ROAMING)
Signal:                        Very strong signal strength (5)
PS:                            Packet Switched Registered to a roaming network (LE_MRC_REG_ROAMING)
```


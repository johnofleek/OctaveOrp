# Images

## Stock product
```
ati8
Legato Ver: 19.07.0_c8105fef6769b17540226029a5d23d44
Yocto Ver:  SWI9X06Y_02.32.02.00 2019-10-08_01:04:54
OS Ver: Linux version 3.18.140 (oe-user@oe-host) (gcc version 7.3.0 (GCC) ) #1 PREEMPT Tue Oct 8 00:58:07 UTC 2019
LK Ver: SWI9X06Y_02.22.12.00
RootFS Ver: SWI9X06Y_02.32.02.00 2019-10-08_01:04:54
UserFS Ver: unknown
MCU Ver: 002.013
```

```
root@swi-mdm9x28-wp:~# cm info
Device:                        WP7702
IMEI:                          352653090215245
IMEISV:                        4
FSN:                           4L936370140910
Firmware Version:              SWI9X06Y_02.32.02.00 c2e98c jenkins 2019/08/30 07:28:21
Bootloader Version:            SWI9X06Y_02.32.02.00 c2e98c jenkins 2019/08/30 07:28:21
MCU Version:                   002.013
PRI Part Number (PN):          9909138
PRI Revision:                  001.001
Carrier PRI Name:              SIERRA
Carrier PRI Revision:          001.027_000
SKU:                           1104405
Last Reset Cause:              Power Down
Resets Count:                  Expected: 9      Unexpected: 0
```



## Actual image used for early demo

Image is 
```
module-2.1.0.wp77xx.spk
```

Programming use windows powershell with this 

```
ls 

Mode                LastWriteTime         Length Name
----                -------------         ------ ----
-a----       28/02/2019     11:09         664056 fdt2.exe
-a----       28/02/2019     10:08        1583608 GobiApi.dll
-a----       19/12/2019     14:57       64543870 module-2.1.0.wp77xx.spk
```
and program mangOH like this

```
./fdt2.exe module-2.1.0.wp77xx.spk
```

### Device after flashing 

```
ati8
Legato Ver: OTVMODULE2.1.0_03288fcd1015098878f8419aa0b052c6
Yocto Ver:  SWI9X06Y_02.32.02.00 2019-08-30_11:05:02
OS Ver: Linux version 3.18.140 (oe-user@oe-host) (gcc version 7.3.0 (GCC) ) #1 PREEMPT Fri Aug 30 10:01:23 UTC 2019
LK Ver: SWI9X06Y_02.32.02.00
RootFS Ver: SWI9X06Y_02.32.02.00 2019-08-30_11:05:02
UserFS Ver: unknown
MCU Ver: 002.013

```


The ORP app causes a Legato restart the first time ever - there is a delay while the backend syncs - after that it works

# Link to the original app source

https://docs.octave.dev/references/edge/octave_resource_protocol/#source-code-for-the-demo-application-for-southbound-resource-protocol

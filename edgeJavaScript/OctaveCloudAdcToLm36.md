

## Cloud Action info
Source stream  "/iinnovate/devices/john_test_yellow/ad_c"  

Cloud action name  "temp_ch1"  

## Cloud Action JavaScript
```
function(event) {

  var adc1 = event.elems.remote.adcs.ad[1];; // event.value;

  var aref_voltage = 5;  // changed to 5V to simplyfy the hardware construction

  var voltage = adc1 * aref_voltage;
  voltage = voltage / 1024.0;

  // LM36 device 
  var s = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset

  return {

        // Create this path in the cloud streams manually - the elements appear automatically
        // Resulting Event(s) must have `elems` with Object type
        // The following appears in the Octave Cloud Streams as path /iinnovate/devices/john_test_yellow/lm36 data {"t1":-3.271484375}
        "/iinnovate/devices/john_test_yellow/lm36": [{"elems": {"t1": s}}] 
  }
}
```

## Input event format

This is the format of the event object in *function(event)* above
  
```
"elems": {
  "remote": {
    "adcs": {
      "ad": [
        227,
        136,
        386,
        370,
        362,
        401,
        398,
        405
      ]
    }
  }
}
```
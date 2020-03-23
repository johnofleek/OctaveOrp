function(event){
  var adc1 = event.value;
  
  var s = adc1;

  return {
    "vr://value/adc1":[s]
  }
}

// arduino_adc_remote_to_cloud
// this edge function calculates the temperate from the arduino ADC count
function(event){
  var adc1 = event.value;
  
  var s ; //= adc1;

  var aref_voltage = 3.3;

  var voltage = adc1 * aref_voltage;
  voltage = voltage / 1024.0;

  // LM36 device 
  s = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset

  return {
    "vr://temperature1":[s]
  }
}



// lm36_adc1_temperature
// same as above but in the cloud

This is an example event
{
  "creationDate": 1584794553065,
  "creatorId": "i000000000000000000000001",
  "elems": {
    "remote": {
      "val": {
        "adc1": 134
      }
    }
  },
  "generatedDate": 1584794551707,
  "hash": null,
  "id": "e5e760bb905d223ef2fb5cb33",
  "lastEditDate": 1584794553065,
  "lastEditorId": "i000000000000000000000001",
  "location": null,
  "metadata": {
  },
  "path": "/iinnovate/devices/john_test_yellow/adc_1",
  "streamId": "s5e74fe204cbfca0efa705ff6",
  "tags": {
  }
}

function(event){
  var adc1 = event.value;
  
  var s ; //= adc1;

  var aref_voltage = 3.3;

  var voltage = adc1 * aref_voltage;
  voltage = voltage / 1024.0;

  // LM36 device 
  s = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset


  return {
        // "/my_company/devices/device1/weather_data_at_location": [{"elems": {"weather": result}}],
        // note that you have to create this path in the cloud streams manually
        "/iinnovate/devices/john_test_yellow/lm36_adc1_temp": [{"elems": {"temperature1": s}}]
    }
  }
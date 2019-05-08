# Smart home

Smart home prototype for Arduino Day 2019 demo.


## Getting Started

These instructions will get you knowing the first steps to use or import this application.

### Prerequisites

* Create a Matr account
* Look for SmartHome app in Matr Hub. Fork the project SmartHome

### Create the channels

#### Channel home

#### Channel command

### Testing the app with mosquitto

### Example of publication with mosquitto client

Channel home server: a2sq3y7mdrjtom.iot.us-east-1.amazonaws.com:8883

MQTT topic: 3e873641

JSON format

{"bulb1State":0,"bulb2State":1,"bulb3State":1,"temp":30,"hum":60,"lux":60}

```
mosquitto_pub -h a2sq3y7mdrjtom.iot.us-east-1.amazonaws.com -p 8883 -t 3e873641 --cert testDevice.certificate.pem --key testDevice.private-key.txt --cafile rootCA.pem -m '{"bulb1State":"true","bulb2State":"true","bulb3State":"true","temp":30,"hum":60,"lux":60}' -d

```

### Example of suscription with mosquitto client

Channel command server: a2sq3y7mdrjtom.iot.us-east-1.amazonaws.com:8883

To subscribe or publish in this channel, use the following MQTT topic: a152175c

mosquitto_sub -h a2sq3y7mdrjtom.iot.us-east-1.amazonaws.com -p 8883 -t a152175c --cert testDevice.certificate.pem --key testDevice.private-key.txt --cafile rootCA.pem -d

## Acknowledgments

* This app was inspired in My Light Control app developed by Matr team
* Matr Async documentation 

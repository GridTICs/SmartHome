# Smart home

Smart home prototype for Arduino Day 2019 demo.


## Getting Started

These instructions will get you knowing the first steps to use or import this application.

### Prerequisites

* Create a Matr account [in the following page](https://platform.matrproject.com/sign-up)
* Look for SmartHome app in Matr Hub. Fork the project SmartHome

### Architecture

NodeMCU -- publish sensor status --> channel home (temperature, humidity, light intensity,etc) 
SmartHome app ---> suscribed to (hear) channel home ---> update APP status
SmartHome --publish commands to --> channel command  (LED on/off)
NodeMCU --> suscribed to (hear) channel command


### Create the MQTT channels

You can follow [these steps](http://matrproject.com/docs/eng/async-channels-eng/) to create two async channels: home and command.

#### Channel home

#### Channel command

### Testing the app with mosquitto

Si quieres probar la aplicación sin tener aún tu hardware terminado, puedes usar mosquitto client para simular tu dispositivo. Mosquitto client es una aplicación que se comporta como un cliente MQTT.

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

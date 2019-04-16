/*
 * NodeMCU Secure MQTT.
 * This code is a simple adaptation of the original.
 * https://gist.github.com/eLement87/133cddc5bd0472daf5cb35a20bfd811e
 * 
 */
#include <Arduino.h>
#include <FS.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <time.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include "credentials.h"

#define DEBUG 0
#define DEBUG_RED 0

// Insert your FQDN of your MQTT Broker
#define MQTT_SERVER "a2sq3y7mdrjtom.iot.us-east-1.amazonaws.com"
const char* mqtt_server = MQTT_SERVER;

// WiFi Credentials
const char* ssid = AP_5;
const char* password = AP_5_PASS;

// Fingerprint of the broker CA
// openssl x509 -in  mqttserver.crt -sha1 -noout -fingerprint
const char* fingerprint = MY_FINGERPRINT;
                           
// Topic
char* outTopic = "3e873641";
char* inTopic = "a152175c";
String clientName;

char jsonStr[200];

long lastMsg = 0;
long lastReconnectAttempt = 0;

int test_para = 5000;
unsigned long startMills;

WiFiClientSecure wifiClient;
PubSubClient client(mqtt_server, 8883, wifiClient);

Adafruit_SHT31 sht31 = Adafruit_SHT31();
int ledPinRed = 14;     // LED connected to D5 but It is GPIO14
int ledPinWhite = 12;   // D6 - GPIO12 
int ledPinYellow = 13;  // D7 - GPIO13
int ledPinGreen = 15;   // D8 - GPIO15

String redLedState = "false";
String yellowLedState = "false";
String greenLedState = "false";

boolean booleanRedLedState = false;
boolean booleanYellowLedState = false;
boolean booleanGreenLedState = false;

int analogPin = analogRead(A0);   // potentiometer connected to analog pin 10
int potentiometerValue = 0;       // variable to store the read value

float illuminance = 0;
float temperature = 0;
float humidity = 0;
int BH1750_address = 0x23;        // i2c Addresse light sensor
byte buff[2];

void verifytls() {
  // Use WiFiClientSecure class to create TLS connection
  Serial.print("connecting to ");
  Serial.println(mqtt_server);
  if (!wifiClient.connect(mqtt_server, 8883)) {
    Serial.println("connection failed");
    return;
  }

  if (wifiClient.verify(fingerprint, mqtt_server)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
  }
}
// Load Certificates
void loadcerts() {
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }
  // Load client certificate file from SPIFFS
  File cert = SPIFFS.open("/arduino.certificate.pem", "r"); //replace esp.der with your uploaded file name
  if (!cert) {
    Serial.println("Failed to open cert file");
  } else {
    Serial.println("Success to open cert file");
  }
 
  delay(1000);
  // Set client certificate
  if (wifiClient.loadCertificate(cert)) {
    Serial.println("cert loaded");
  } else {
    Serial.println("cert not loaded");
  }
 // Load client private key file from SPIFFS
  File private_key = SPIFFS.open("/arduino.private-key.txt", "r"); //replace espkey.der with your uploaded file name
  if (!private_key) {
    Serial.println("Failed to open private cert file");
  } else {
    Serial.println("Success to open private cert file");
  }
  
  delay(1000);
  // Set client private key
  if (wifiClient.loadPrivateKey(private_key)) {
    Serial.println("private key loaded");
  } else {
    Serial.println("private key not loaded");
  }
  // Load CA file from SPIFFS
  File ca = SPIFFS.open("/rootCA.pem", "r"); //replace ca.der with your uploaded file name
  if (!ca) {
    Serial.println("Failed to open ca ");
  } else {
    Serial.println("Success to open ca");
  }
  delay(1000);
  // Set server CA file
  if(wifiClient.loadCACert(ca)) {
    Serial.println("ca loaded");
  } else {
    Serial.println("ca failed");
  }
}

void getTime() {
  // Synchronize time useing SNTP. This is necessary to verify that
  // the TLS certificates offered by the server are currently valid.
  Serial.print("Setting time using SNTP");
  configTime(8 * 3600, 0, "de.pool.ntp.org");
  time_t now = time(nullptr);
  while (now < 1000) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

boolean reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect((char*) clientName.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(outTopic, jsonStr);
      // ... and resubscribe
      client.subscribe(inTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  return client.connected();
}

void wifi_connect() {
  if (WiFi.status() != WL_CONNECTED) {    // WIFI
    Serial.println();
    Serial.print("===> WIFI ---> Connecting to ");
    Serial.println(ssid);
    delay(10);
    WiFi.begin(ssid, password);
    int Attempt = 0;
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(". ");
      Serial.print(Attempt);
      delay(5000);
      Attempt++;
      if (Attempt == 150) {
        Serial.println();
        Serial.println("-----> Could not connect to WIFI");
        ESP.restart();
        delay(200);
      }
    }
    
    Serial.println();
    Serial.print("===> WiFi connected");
    Serial.print(" ------> IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);                                         // will pause until serial console opens
  }
  
  startMills = millis();
  wifi_connect();
  delay(500);
  getTime();
  delay(500);
  loadcerts();
  delay(200);
  clientName += "esp8266-";
  uint8_t mac[6];
  WiFi.macAddress(mac);
  clientName += macToStr(mac);
  clientName += "-";
  clientName += String(micros() & 0xff, 16);
  Wire.begin();
  BH1750_Init(BH1750_address);
  if (DEBUG) {
    Serial.println("SHT31 test");  
  }
    
  if (!sht31.begin(0x44)) {   // i2c Addresse sht31 sensor. If pin ADR is in LOW i2c address = 0x44. And pin ADR is in HIGH alternate i2c address = 0x45.
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
    
  pinMode(ledPinRed, OUTPUT);   // sets the pin as output
  pinMode(ledPinGreen, OUTPUT);
  pinMode(ledPinYellow, OUTPUT);
  pinMode(ledPinWhite, OUTPUT);
  client.setCallback(callback);
}

void loop() {
  getData();
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
    reconnect();
    }
    
    client.loop();
    long now = millis();
    if (now - lastMsg > 2500) {
      lastMsg = now;
      if (client.connected()) {
        String json = buildJson(temperature,humidity,illuminance);
        json.toCharArray(jsonStr,200);
        boolean pubresult = client.publish(outTopic,jsonStr);
        Serial.print("attempt to send ");
        Serial.println(jsonStr);
        Serial.print("to ");
        Serial.println(outTopic);
        if (pubresult) {
          Serial.println("successfully sent");
        } else {
          Serial.println("unsuccessfully sent");
        }
      }
    }
  } else {
    wifi_connect();
  }
}

void sendmqttMsg(char* topictosend, String payload) {
  if (client.connected()) {
    Serial.print("Sending payload: ");
    Serial.print(payload);
    unsigned int msg_length = payload.length();
    Serial.print(" length: ");
    Serial.println(msg_length);
    byte* p = (byte*)malloc(msg_length);
    memcpy(p, (char*) payload.c_str(), msg_length);
    if ( client.publish(topictosend, p, msg_length)) {
      Serial.println("Publish ok");
      free(p);
      //return 1;
    } else {
      Serial.println("Publish failed");
      free(p);
      //return 0;
    }
  }
}

String macToStr(const uint8_t* mac) {
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  
  return result;
}

void callback(char* topic, byte* payload, unsigned int length) {
  const char* toggleRed    = "{\"command\":\"TOGGLE-RELAY\",\"relayIdx\":1}";
  const char* toggleYellow = "{\"command\":\"TOGGLE-RELAY\",\"relayIdx\":2}";
  const char* toggleGreen  = "{\"command\":\"TOGGLE-RELAY\",\"relayIdx\":3}";
  char charPayload[39];
  int ledNumber;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    charPayload[i] = (char)payload[i];
    Serial.print((char)payload[i]);
  }
  
  const char*  stringPayload = charPayload;
  Serial.println(stringPayload);
  // Switch on the LED
  if (!strcmp (stringPayload, toggleRed)) {
    ledNumber = 1;
    ledsState (ledNumber);
    Serial.println("Led Rojo");
  }
  
  if (!strcmp (stringPayload, toggleYellow )) {
    ledNumber = 2;
    ledsState (ledNumber);
    Serial.println("Led Amarillo");
  }
  
  if (!strcmp (stringPayload, toggleGreen )) {
    ledNumber = 3;
    ledsState (ledNumber);
    Serial.println("Led Verde");
  }
}

void ledsState (int ledToggle) {
  if (ledToggle == 1) {
    if (booleanRedLedState == true) {
      redLedState = "false";
      booleanRedLedState = false;
      digitalWrite(ledPinRed, LOW);
    } else {
      redLedState = "true";
      booleanRedLedState = true;
      digitalWrite(ledPinRed, HIGH);
    }
  }
  
  if (ledToggle == 2) {
    if (booleanYellowLedState == true) {
      yellowLedState = "false";
      booleanYellowLedState = false;
      digitalWrite(ledPinYellow, LOW);
    } else {
      yellowLedState = "true";
      booleanYellowLedState = true;
      digitalWrite(ledPinYellow, HIGH);
    }
  }
  
  if (ledToggle == 3) {
    if (booleanGreenLedState == true) {
      greenLedState = "false";
      booleanGreenLedState = false;
      digitalWrite(ledPinGreen, LOW);
    } else {
      greenLedState = "true";
      booleanGreenLedState = true;
      digitalWrite(ledPinGreen, HIGH);
    }
  }
}

String buildJson(float temperatura, float humedad, float luminosidad) {
  String data = "{";
  data+="\"bulb1State\":";
  data+=redLedState;
  data+=",";
  data+="\"bulb2State\":";
  data+=yellowLedState;
  data+=",";
  data+="\"bulb3State\":";
  data+=greenLedState;
  data+=",";
  data+="\"temp\":";
  data+=temperatura;
  data+=",";
  data+="\"hum\":";
  data+=humedad;
  data+=",";
  data+="\"lux\":";
  data+=luminosidad;
  data+="}";
  return data;
}

void BH1750_Init(int address) {
  Wire.beginTransmission(address);
  Wire.write(0x10); // 1 [lux] aufloesung
  Wire.endTransmission();
}

byte BH1750_Read(int address) {
  byte i = 0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  while(Wire.available()) {
    buff[i] = Wire.read(); 
    i++;
  }
  
  Wire.endTransmission();  
  return i;
}

void getData() {
  temperature = sht31.readTemperature();
  humidity = sht31.readHumidity();
  if (!isnan(temperature)) {  // check if 'is not a number'
  } else { 
    Serial.println("Failed to read temperature");
    temperature = -1;
  }
  
  if (!isnan(humidity)) {    // check if 'is not a number'
  } else { 
    Serial.println("Failed to read humidity");
    humidity = -1;
  }
  
  potentiometerValue = analogRead(analogPin);            // read the input pin
  analogWrite(ledPinWhite, potentiometerValue / 4);      // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
  if(BH1750_Read(BH1750_address)==2) {
    illuminance = ((buff[0] << 8) | buff[1] ) / 1.2;
    if (illuminance < 0) {
      Serial.print("> 65535");
    }
  }
}

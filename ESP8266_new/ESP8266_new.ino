#define THINGER_SERIAL_DEBUG

#include <ThingerESP8266.h>
#include "arduino_secrets.h"
#include <ESP8266WiFi.h>
#include <ThingerWifi.h>
#include "DHT.h"

#define DHTPIN D2 
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
#define LEDPIN D1 

#define TRIGGER_PIN  D4 
#define ECHO_PIN     D3 

ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(LEDPIN, OUTPUT);


  thing.add_wifi(SSID, SSID_PASSWORD);


  thing["led"] << digitalPin(LED_BUILTIN);

  thing["led2"] << digitalPin(LEDPIN);


  thing["millis"] >> outputValue(millis());

  dht.begin();

  thing["Temperature_C"] >> [](pson & out) {
    out = dht.readTemperature();
};

thing["Temperature_F"] >> [](pson & out) {
    out = dht.readTemperature(true);
};

thing["Humidity"] >> [](pson & out) {
    out = dht.readHumidity();
};

thing["distance"] >> [] (pson& out){
      double duration, distance;
      digitalWrite(TRIGGER_PIN, LOW);  
      delayMicroseconds(2); 
      digitalWrite(TRIGGER_PIN, HIGH); 
      delayMicroseconds(10); 
      digitalWrite(TRIGGER_PIN, LOW); 
      duration = pulseIn(ECHO_PIN, HIGH); 
      distance = (duration/2); 
      out = distance;
  };


 }

void loop() {
  thing.handle();
}

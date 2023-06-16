#pragma once

#include "Config.h"
#include "State.h"
#include <ESP8266WiFi.h>
#include <aREST.h>
#include <ESP8266HTTPClient.h>
#include "Clock.h"

int deserializeTargetTemperature(String targetTemperature) {
  state.deserializeTargetTemperature(targetTemperature);
  return 1;
}
int deserializeIsOn(String isOn) {
  state.deserializeIsOn(isOn);
  return 1;
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

class RestClient {
public:
  RestClient() : server_{WiFiServer(80)} {}

  void setup() {
    Serial.println("Init Rest Client: ");

    rest_.function("set_target_emp", deserializeTargetTemperature);
    rest_.function("setIsOn", deserializeIsOn);
    Serial.println("Init WIFI... ");
    
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(config.ssid.c_str());
    WiFi.begin((char *)config.ssid.c_str(), (char *)config.pwd.c_str());
      
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
    }
    Serial.print("Connected.");
    server_.begin();
    printWifiStatus();

    timer_.interval();
  }

  void process() {
    
    if(timer_.hasPassed(1000)){
      timer_.interval();
    
      WiFiClient client = server_.available();
      rest_.handle(client);
      
      HTTPClient http;
      http.begin(client, config.remoteIP, 8080, "/log");
      
      int responseCode = http.POST(state.to_json().c_str());
      
      if (responseCode != 200){
        Serial. print(state.to_json().c_str());
        Serial.print(responseCode);
        Serial.println();
      }
    }
  }

private:
  WiFiServer server_;
  aREST rest_;
  Clock timer_;
};

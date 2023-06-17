#pragma once

#include "Config.h"
#include "State.h"
#include <ESP8266WiFi.h>
#include <aREST.h>
#include <ESP8266HTTPClient.h>
#include "Clock.h"

int deserialize_target_temperature(String targetTemperature) {
  state.deserialize_target_temperature(targetTemperature);
  return 1;
}
int deserialize_activated(String activated) {
  state.deserialize_activated(activated);
  return 1;
}

int deserialize_override_pid(String override_pid) {
  state.deserialize_override_pid(override_pid);
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

    rest_.function("set_target_temp", deserialize_target_temperature);
    rest_.function("set_activated", deserialize_activated);
    rest_.function("set_override_pid", deserialize_override_pid);

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
    WiFiClient client = server_.available();
    rest_.handle(client);
      
    if(timer_.hasPassed(1000)){
      timer_.interval();
    
      HTTPClient http;
      http.begin(client, config.remoteIP, 8080, "/log");
      
      int responseCode = http.POST(state.to_json().c_str());
      
      if (responseCode != 200){
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

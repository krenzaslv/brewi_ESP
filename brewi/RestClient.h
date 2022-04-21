#pragma once

#include "Config.h"
#include "State.h"
#include <ESP8266WiFi.h>
#include <aREST.h>

int deserializeConfig(String overridePID){
  state.deserialize(overridePID);
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

class RestClient{
public:
  RestClient(): server_{WiFiServer(80)}{
  }

  void setup(){
    Serial.println("Init Rest Client: ");
    rest_.variable("temperature", &state.temperature); 
    rest_.variable("targetTemperature", &state.targetTemperature); 
    rest_.variable("isHeating", &state.isHeating);
    rest_.variable("pidGain", &state.pidGain); 
    rest_.variable("pidD", &state.pidD); 
    rest_.variable("pidI", &state.pidI); 
    rest_.variable("time", &state.time); 
    rest_.variable("pidP", &state.pidP); 
    rest_.variable("dutyCycles", &state.dutyCycles); 
    rest_.function("state", deserializeConfig); 
    Serial.println("Init WIFI... ");

    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
      Serial.println("WiFi shield not present");
      // don't continue:
      while (true);
    }
    while ( status_ != WL_CONNECTED) {
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(config.ssid.c_str());
      status_ = WiFi.begin((char *) config.ssid.c_str(), (char *) config.pwd.c_str());
      delay(10000);
    }
    server_.begin();
    printWifiStatus();
  }

  void process(){
    WiFiClient client = server_.available();
    rest_.handle(client);
  }

private:
  WiFiServer server_;
  aREST rest_;

  int status_ = WL_IDLE_STATUS;
};

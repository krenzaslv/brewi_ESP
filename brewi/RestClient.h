#pragma once

#include "Config.h"
#include "State.h"
#include <WiFi.h>
#include <aREST.h>

int setOverridePID(String overridePID){
  state.setOverridePIDFromJson(overridePID);
  return 1;
}

int setHeatingElementOn(String heatingElementOn){
  state.setHeatingElementOnFromJson(heatingElementOn);
  return 1;
}

int setKP(String json){
  state.setKPFromJson(json);
  return 1;
}

int setTargetTemperature(String json){
  state.setTargetTemperature(json);
  return 1;
}

int setKI(String json){
  state.setKIFromJson(json);
  return 1;
}

int setKD(String json){
  state.setKDFromJson(json);
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
    rest_.variable("temperature", &state.temperature); 
    rest_.variable("isHeating", &state.isHeating);
    rest_.variable("pidGain", &state.pidGain); 
    rest_.variable("pidD", &state.pidD); 
    rest_.variable("pidI", &state.pidI); 
    rest_.variable("pidP", &state.pidP); 
    rest_.variable("dutyCycles", &state.dutyCycles); 
    rest_.function("setOveridePID", setOverridePID); 
    rest_.function("setHeatinElementOn", setHeatingElementOn); 
    rest_.function("setKI", setKI); 
    rest_.function("setKP", setKP); 
    rest_.function("setKP", setKP); 
    rest_.function("setKP", setTargetTemperature); 

    while ( status_ != WL_CONNECTED) {
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

  int status_ = 0;
  float temperature = -127.0;
} restClient;

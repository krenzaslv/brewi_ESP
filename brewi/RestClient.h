#pragma once

#include "Config.h"
#include "State.h"
#include <WiFi.h>
#include <aREST.h>
#include <ArduinoHttpClient.h>
#include <exception>

class RestClient {
public:
  RestClient()
    : server_{ WiFiServer(80) }, client_{ server_.available() }, http_{ client_, config.remoteIP, 8080 } {}

  void process() {
    try {
      //POST state to server
      http_.post("/log", "application/json", state.to_json().c_str());
      int responseCode = http_.responseStatusCode();
      if (responseCode != 200) {
        Serial.print("Response code: ");
        Serial.print(responseCode);
      }else{
        //Process control command
        String responseBody = http_.responseBody();
        if (responseBody != "{}") state.from_json(responseBody);
      }
    } catch (String e){
      Serial.print("Couldn't send/receive to server with exeption: ");
      Serial.println(e);
    }
  }


  void setup() {
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
  }


private:
  WiFiClient client_;
  HttpClient http_;

  WiFiServer server_;

  void printWifiStatus() {
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
  }
};

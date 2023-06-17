#pragma once

#include "Config.h"
#include "State.h"
#include <ESP8266WiFi.h>
#include <aREST.h>
#include <ESP8266HTTPClient.h>
#include "Chrono.h"

class RestClient {
  public:
    RestClient(int messageInterval) : messageInterval_{messageInterval}, server_{WiFiServer(80)} {}

    void process() {
      WiFiClient client = server_.available();
      
      if(timer_.hasPassed(messageInterval_)){
        timer_.restart();

        HTTPClient http;

        //POST state to server
        http.begin(client, config.remoteIP, 8080, "/log");
        int responseCode = http.POST(state.to_json().c_str());
        if (responseCode != 200){
          Serial.print(responseCode);
          Serial.println();
        }else{
          try{
            state.from_json(client.responseBody());
          }
        }

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
    int messageInterval_;

    WiFiServer server_;
    Chrono timer_;

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


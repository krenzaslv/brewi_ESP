#pragma once

#include <ArduinoJson.h>

struct State{
  int deserialize(String json){
    try{
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, json);
    k_p = doc["k_p"];
    temperature = doc["temperature"];
    k_i = doc["k_i"];
    k_d = doc["k_d"];
    targetTemperature = doc["targetTemperature"];
    scaleMax = doc["scaleMax"];    
    dutyCycleDuration= doc["maxDutyCycles"]; 
    return 1;
    }catch(const std::exception& e){
      Serial.print(e.what());
      return 0;
    }
  }

  
  bool overridePID = false; 
  bool heatingElementOn = false;
  bool isHeating = false;
  float temperature = 22.0;
  float targetTemperature = 22.0;
  float pidGain = 0;
  float pidD = 0;
  float pidI = 0;
  float pidP = 0;
  float k_p = 1;
  float k_i = 0.01;
  float k_d = 1;
  float dutyCycles = 0;
  int scaleMax = 3;
  int dutyCycleDuration = 5; //seconds
  float time = 0; //seconds
} state;

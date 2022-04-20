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
    scaleMin = doc["scaleMin"];    
    scaleMax = doc["scaleMax"];    
    max_duty_cycle= doc["maxDutyCycles"]; 
    return 1;
    }catch(const std::exception& e){
      Serial.print(e.what());
      return 0;
    }
  }

  
  bool overridePID = false; 
  bool heatingElementOn = false;
  float temperature = 22.0;
  float targetTemperature = 22.0;
  bool isHeating = false;
  float pidGain = 0;
  float pidD = 0;
  float pidI = 0;
  float pidP = 0;
  float k_p = 1;
  float k_i = 1;
  float k_d = 1;
  int dutyCycles = 0;
  int scaleMin = 500;
  int scaleMax = 500;
  int max_duty_cycle = 20; //seconds
} state;

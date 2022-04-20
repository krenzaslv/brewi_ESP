#pragma once

#include <ArduinoJson.h>

struct State{

  void setOverridePIDFromJson(String json){
    DynamicJsonDocument doc = deserialize(json);
    temperature = doc["temperature"];
  }

  void setKPFromJson(String json){
    DynamicJsonDocument doc = deserialize(json);
    k_p = doc["k_p"];
  }

  void setKIFromJson(String json){
    DynamicJsonDocument doc = deserialize(json);
    k_i = doc["k_i"];
  }

  void setKDFromJson(String json){
    DynamicJsonDocument doc = deserialize(json);
    k_d = doc["k_d"];
  }

  void setTargetTemperature(String json){
    DynamicJsonDocument doc = deserialize(json);
    targetTemperature = doc["targetTemperature"];
  }


  void setHeatingElementOnFromJson(String json){
    DynamicJsonDocument doc = deserialize(json);
    temperature = doc["temperature"];    
  }

  DynamicJsonDocument deserialize(String json){
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, json);
    return doc;
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
} state;

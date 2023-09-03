#pragma once

#include <ArduinoJson.h>
#include <string>

struct State {

  bool is_heating = false;
  bool is_activated = false;
  bool override_pid = false;

  float temperature = 22.0;
  float temperatureAvg = 22.0;
  float temperatureExp= 22.0;
  float temperatureKalman = 22.0;

  float target_temperature = 57.0;
  float pid_gain = 0;
  float pd_gain = 0;
  float pi_gain = 0;
  float pp_gain = 0;
  float pp_gain_scaled = 0;
  float pd_gain_scaled = 0;
  float pi_gain_scaled = 0;
  
  float k_p = 0.7;
  float t_i = 50;
  float t_d = 20;

  float duty_cycle = 0;
  int pidWindowLenght = 10*1000; 


  std::string to_json() const{
    DynamicJsonDocument json(1024);
    json["is_heating"] = is_heating;
    json["is_activated"] = is_activated;
    json["override_pid"] = override_pid;

    json["temperature"] = temperature;
    json["temperatureAvg"] = temperatureAvg;
    json["temperatureExp"] = temperatureExp;
    json["temperatureKalman"] = temperatureKalman;
    json["target_temperature"] = target_temperature;

    json["pid_gain"] = pid_gain;
    json["pd_gain"] = pd_gain;
    json["pi_gain"] = pi_gain;
    json["pp_gain"] = pp_gain;
    json["pp_gain_scaled"] = pp_gain_scaled;
    json["pd_gain_scaled"] = pd_gain_scaled;
    json["pi_gain_scaled"] = pi_gain_scaled;
    json["duty_cycle"] = duty_cycle;


    json["k_p"] = k_p;
    json["t_i"] = t_d;
    json["t_d"] = t_d;

    char output[1024];
    serializeJson(json, output);
    return std::string(output);
  }


  void from_json(String json) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, json);
    k_p  = doc["k_p"];
    t_i = doc["t_i"];
    t_d = doc["t_d"];
    override_pid = doc["override_pid"];
    target_temperature = doc["target_temperature"];
    is_activated = doc["is_activated"];
  }
} state;

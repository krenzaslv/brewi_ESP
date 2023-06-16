#pragma once

#include <ArduinoJson.h>
#include <string>

struct State {
  int deserializeTargetTemperature(String temp) {
    Serial.print("Setting targetTemperature to:");
    Serial.print(temp.toFloat());
    target_temperature = temp.toFloat();
    return 1;
  }
  int deserializeIsOn(String on) {
    is_on = on.toInt();
    return 1;
  }

  std::string to_json() const{
    DynamicJsonDocument json(1024);
    json["is_heating"] = is_heating;
    json["is_on"] = is_on;
    json["override_pid"] = override_pid;

    json["temperature"] = temperature;
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

  bool is_heating = false;
  bool is_on = false;
  bool override_pid = false;

  float temperature = 22.0;
  float target_temperature = 30.0;
  float pid_gain = 0;
  float pd_gain = 0;
  float pi_gain = 0;
  float pp_gain = 0;
  float pp_gain_scaled = 0;
  float pd_gain_scaled = 0;
  float pi_gain_scaled = 0;

  
  float k_p = 25;
  float t_i = 3;
  float t_d = 2;
  float duty_cycle = 0;
  int scale_max = 100;
  int duty_cycle_duration = 60; // seconds
  float time = 0;             // minutes
} state;

#pragma once

#include "State.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMPERATURE_BUS 4

class TemperatureSensor{

public:
  TemperatureSensor(int nMesurements = 3): nMesurements_{nMesurements}, oneWire_{TEMPERATURE_BUS}, sensors_(&oneWire_){
  }

  void setup(){
   // sensors_.setResolution(10);
    sensors_.begin();
  }

  void process(){
    float avgTemperature = 0;
    //Average sensor measurements
    for(size_t i = 0; i<nMesurements_; ++i){
      sensors_.requestTemperatures(); 
      avgTemperature += sensors_.getTempCByIndex(0);
    }
    state.temperature = avgTemperature/nMesurements_;
  }

private:
  int nMesurements_;
  OneWire oneWire_;
  DallasTemperature sensors_;
};

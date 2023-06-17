#pragma once

#include "HeatingElement.h"
#include "State.h"
#include <DallasTemperature.h>
#include <OneWire.h>
#include "Chrono.h"

#define TEMPERATURE_BUS 4

template<int N>
class CircularTemperatureBuffer{
  public:
    void add(float temperature){
      temperatures[currentI%N] = temperature;
      ++currentI;
      ++bufferSize;
      if(currentI > 1000) currentI = currentI % N; //Prevent buffer overflow
      if(bufferSize > 1000) bufferSize = N + 1; //Prevent buffer overflow
    }

    float getAvg(){
      float res = 0;
      int n = bufferSize > N ? N : bufferSize;
      for(size_t i=0; i< n; ++i){
        res += temperatures[i];  
      }
      return res/n;
    }

  private:
    size_t bufferSize = 0;
    size_t currentI = 0;
    float temperatures[N];
};


template<int N, int M> ///Running average over last N measurements with M measurements per iteration
class TemperatureSensor {

  public:
    TemperatureSensor() : oneWire_{HEATING_BUS}, sensors_(&oneWire_) {}

    void setup() {
      // sensors_.setResolution(10);
      sensors_.begin();
    }

    void process() {

      float avgTemperature = 0;
      // Average sensor measurements
      for (size_t i = 0; i < M; ++i) {
        sensors_.requestTemperatures();
        avgTemperature += sensors_.getTempCByIndex(0);
        delay(5);
      }
      avgTemperature/=nMesurements_;
      float currentTmp = buffer_.getAvg();
      if(avgTemperature > 1.2*currentTmp || avgTemperature < 0.8*currentTmp) return;
      buffer_.add(avgTemperature);
      state.temperature = buffer_.getAvg();
    }

  private:
    int nMesurements_;
    OneWire oneWire_;
    DallasTemperature sensors_;
    CircularTemperatureBuffer<N> buffer_;
};

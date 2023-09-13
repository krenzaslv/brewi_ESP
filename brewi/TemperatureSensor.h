#pragma once

#include "HeatingElement.h"
#include "State.h"
#include <DallasTemperature.h>
#include <OneWire.h>

#define TEMPERATURE_BUS D2

template<int N>
class MovingAverageFilter{
  public:
    float add(float temperature){
      temperatures[currentI%N] = temperature;
      ++currentI;
      ++bufferSize;
      if(currentI > 1000) currentI = currentI % N; //Prevent buffer overflow
      if(bufferSize > 1000) bufferSize = N + 1; //Prevent buffer overflow
      
      return getAvg();
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

class ExponentialFilter{
  public:

    void setup(float temp){
      y_n_ = temp;
    }

    float next(float temp){
      y_n_ = w*temp+ (1-w)*y_n_;
      return y_n_;
  }

  private:
    float y_n_ = 0;

    //Exp filter params
    float w = 0.5;
};

class KalmanFilter{
  public:

    void setup(float temp){
      x_n_ = temp;
      x_nm1_ = temp;
    }

    float next(float temp, float dt){
      //State update
      x_n_ = x_n_ + K*(temp - x_n_); //State update
      x_n_dot_ = (x_n_ - x_nm1_)*dt; //FD estimate of state velocity 
      x_nm1_ = x_n_;
      p = (1-K)*p; // Covariance update
      K = p/(p+r); // Kalman gain

      // State predict
      x_n_ = x_n_ + dt*x_n_dot_;
      p = p + dt*dt*p_v;
      p_v = p_v + q;
      return x_n_;
    }

  private:
    float x_n_dot_ = 0;
    float x_n_ = 0;
    float x_nm1_ = 0;

    float K = 0.5;
    float r = 0.5; // Measurement noise
    float p = 0.5; // Variance estimate
    float p_v = 0.5; // Velocity variance estimate 
    float q = 0.5; // Process noise 
};



template<int N, int M> ///Running average over last N measurements with M measurements per iteration
class TemperatureSensor {

  public:
    TemperatureSensor() : oneWire_{TEMPERATURE_BUS}, sensors_(&oneWire_) {
    }

    void setup() {
      
      sensors_.begin();
      //sensors_.getAddress(deviceAddress_, 0);
      //sensors_.setResolution(deviceAddress_, 12);
      sensors_.setResolution(12);
      sensors_.requestTemperatures();
      float temp =  sensors_.getTempCByIndex(0);
      expFilter_.setup(temp);
      kalmanFilter_.setup(temp);
      Serial.println("setup");
      Serial.println(temp);

      clock_.restart();
    }

    void process() {
      float avgTemperature = 0;
      // Average sensor measurements
      for (size_t i = 0; i < M; ++i) {
          sensors_.requestTemperatures();
      //    avgTemperature += sensors_.getTempC(deviceAddress_);
          float temp = sensors_.getTempCByIndex(0);
          if(temp == DEVICE_DISCONNECTED_C){
            Serial.println("No device connected");
          }
          avgTemperature +=  sensors_.getTempCByIndex(0);
        if(i == 0) state.temperature = avgTemperature;
        if (M>1) delay(10);
      }
      avgTemperature/=(float) M;
      
      //float currentTmp = movingAvgFilter_.getAvg();
      // if(avgTemperature > 1.5*currentTmp || avgTemperature < 0.5*currentTmp) return;

      state.temperatureAvg = movingAvgFilter_.add(avgTemperature);
      state.temperatureExp = expFilter_.next(avgTemperature);
      state.temperatureKalman = kalmanFilter_.next(avgTemperature,((float) clock_.elapsed())/1000);
      Serial.print("temperature: ");
      Serial.println(state.temperature);
      Serial.print("temperature avg:");
      Serial.println(state.temperatureAvg);
      clock_.restart();
    }

  private:

    Chrono clock_;
    OneWire oneWire_;
    DallasTemperature sensors_;
    DeviceAddress deviceAddress_;

    MovingAverageFilter<N> movingAvgFilter_;
    ExponentialFilter expFilter_;
    KalmanFilter kalmanFilter_;
};

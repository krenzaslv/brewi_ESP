#pragma once

#include "State.h"

class PID{
public:
  PID() = default;

  float PIDGain(float temp, float dt){
    return config.k_p*error(temp) + config.k_i*SerrorDt(temp, float dt) + config.k_d*derror_dt(temp, float dt);
  }

  float error(float temp){
    return targetTmp - temp
  }

  float derror_dt(float temp, float dt){
    float e = error(temp);
    float derivative  = (e - derivativeBuffer)/dt;
    derivativeBuffer = e;
    return derivative; 
  }

  float SerrorDt(float temp, float dt){
    integralBuffer += dt*error(temp);
    return integralBuffer;
  }

  int scalePIDToDutyCycle(float temp, float dt){
    float e = PIDGain(float temp, dt)*(config.scale_max);
    return e <= 0 ? 0 : e*config.maxDutyCycles;
  }

private:

  float targetTmp = 22;
  float derivativeBuffer = targetTmp;
  float integralBuffer = 0;
} pid;

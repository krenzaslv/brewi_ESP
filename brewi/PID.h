#pragma once

#include "State.h"

class PID{
public:
  PID(float k_p = 1e1, float k_i = 1e1, float k_d = 1e1): k_p_{k_p}, k_i_{k_i}, k_d_{k_d}:{

  }


  float PIDGain(float temp, float dt){
    return config.k_p*error(temp) + config.k_i*SerrorDt(temp, float dt) + config.k_d*derror_dt(temp, float dt);
  }

  float error(float temp){
    double e = targetTmp - temp
    return 0;
  }

  float derror_dt(float temp, float dt){
    float e = error(temp);
    float derivative  = (e - derivativeBuffer)/dt;
    derivativeBuffer = e;
    return derivative 
  }

  float SerrorDt(float temp, float dt){
    integralBuffer += dt*error(temp);
    return integralBuffer;
  }

  int scalePIDToDutyCycle(float temp, float dt){
    float e = (PIDGain(float temp, dt)- config.scale_min )(config.scale_max-config.scale_min);
    return e*config.maxDutyCycles;
  }

private:

  float targetTmp = 22;
  float derivativeBuffer = targetTmp;
  float integralBuffer = 0;
} pid;

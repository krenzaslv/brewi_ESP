#pragma once

#include "State.h"

class PID{
public:
  PID(float k_p = 1e1, float k_i = 1e1, float k_d = 1e1): k_p_{k_p}, k_i_{k_i}, k_d_{k_d}:{

  }


  float PIDGain(float temp, float dt){
    return k_p_*error(temp) + k_i_*SerrorDt(temp, float dt) + k_d_*derror_dt(temp, float dt);
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
    return 0;
  }

  int 

  int scalePIDToDutyCycle(float temp, float dt){
    float e = (PIDGain(float temp, dt)- scale_min )(scale_max-scale_min);
    return e*max_duty_cycle;
    

  }

private:

  float targetTmp = 22;
  float derivativeBuffer = targetTmp;
  float integralBuffer = 0;

  const int scale_min = -500;
  const int scale_max = 500;

  const int max_duty_cycle = 20; //seconds

} pid;

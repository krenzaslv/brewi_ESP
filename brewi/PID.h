#pragma once

#include "State.h"
#include <algorithm>

class PID {
public:
  PID() = default;

  void setup(){

  }

  void reset(){
    integralBuffer = 0;
    temperatureBuffer = state.temperatureAvg;
  }

  //dt in seconds
  void process(float dt) {
    state.pid_gain = scaletoWindowLenght(pidGain(dt));
  }

  float pidGain(float dt) {
    return state.k_p * error() + scaletoWindowLenght(SerrorDt(dt)) + state.k_p*state.t_d * derror_dt(dt);
  }

  float error() {
    float e = state.target_temperature - state.temperatureAvg;
    state.pp_gain = e;
    state.pp_gain_scaled = state.k_p * e;
    return e;
  }

  float derror_dt(float dt) {
    float derivative = -(state.temperatureAvg- temperatureBuffer) / dt;
    temperatureBuffer = state.temperatureAvg;
    state.pd_gain = derivative;
    state.pd_gain_scaled = state.k_p*state.t_d * derivative;
    return derivative;
  }

  float SerrorDt(float dt) {
    integralBuffer += state.k_p/state.t_i* dt * error() ;
    state.pi_gain = integralBuffer;
    state.pi_gain_scaled = integralBuffer;
    return integralBuffer;
  }


private:
  float temperatureBuffer = 0;
  float integralBuffer = 0;

  float scaletoWindowLenght(float gain) {
    return std::min(std::max(gain, 0.0f), (float)(state.pidWindowLenght/(float)1e3));
  }
};

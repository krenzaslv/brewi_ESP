#pragma once

#include "State.h"

class PID {
public:
  PID() = default;

  void process(float dt) {
    state.pid_gain = pidGain(dt);
    state.duty_cycle = scalePIDToDutyCycle(state.pid_gain);
  }

  float pidGain(float dt) {
    return state.k_p * (error() + 1/state.t_i * SerrorDt(dt) +
           state.t_d * derror_dt(dt));
  }

  float error() {
    float e = state.target_temperature - state.temperature;
    state.pp_gain_scaled = state.k_p * e;
    return e;
  }

  float derror_dt(float dt) {
    float e = error();
    float derivative = (e - derivativeBuffer) / dt;
    derivativeBuffer = e;
    state.pd_gain_scaled = state.k_p*state.t_d * derivative;
    return derivative;
  }

  float SerrorDt(float dt) {
    integralBuffer += dt * error() ;
    integralBuffer = std::abs(error()) > 1.5 ? 0 : integralBuffer;
    state.pi_gain_scaled = state.k_p/state.t_i * integralBuffer;
    return integralBuffer;
   
  }

  float scalePIDToDutyCycle(float gain) {
    float e = gain / (state.scale_max);
    e = e > 1 ? 1 : e;
    return gain <= 0 ? 0 : e;
  }

private:
  float derivativeBuffer = state.target_temperature;
  float integralBuffer = 0;
};

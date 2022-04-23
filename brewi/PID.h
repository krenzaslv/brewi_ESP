#pragma once

#include "State.h"

class PID {
public:
  PID() = default;

  void process(float dt) {
    state.pidGain = PIDGain(dt);
    state.dutyCycles = scalePIDToDutyCycle(state.pidGain);
  }

  float PIDGain(float dt) {
    return state.k_p * error() + state.k_i * SerrorDt(dt) +
           state.k_d * derror_dt(dt);
  }

  float error() {
    float e = state.targetTemperature - state.temperature;
    state.pidP = e;
    state.pidP_scaled = state.k_p * e;
    return e;
  }

  float derror_dt(float dt) {
    float e = error();
    float derivative = (e - derivativeBuffer) / dt;
    derivativeBuffer = e;
    state.pidD = derivative;
    state.pidD_scaled = state.k_d * state.pidD;
    return derivative;
  }

  float SerrorDt(float dt) {
    integralBuffer += dt * error();
    integralBuffer = integralBuffer > 10 ? 10 : integralBuffer;
    integralBuffer = integralBuffer < -10 ? -10 : integralBuffer;
    state.pidI = integralBuffer;
    state.pidI_scaled = state.k_i * state.pidI;
    return integralBuffer;
  }

  float scalePIDToDutyCycle(float gain) {
    float e = gain / (state.scaleMax);
    e = e > 1 ? 1 : e;
    return gain <= 0 ? 0 : e;
  }

private:
  float derivativeBuffer = state.targetTemperature;
  float integralBuffer = 0;
};

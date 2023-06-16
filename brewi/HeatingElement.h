#pragma once

#include "State.h"
#include "Clock.h"

#define HEATING_BUS 12

class HeatingElement{
public:
  HeatingElement(){
    timer_.interval();
  }

  void setup(){
    pinMode(HEATING_BUS, OUTPUT);   
  }
  
  //Returns wheter current cycle is finished
  bool process(){
    if(timer_.dt_interval() < state.duty_cycle_duration){
      processDutyCycle();
      return true;
    }
    else{
      timer_.interval();
      return false;
    }
  }

private:
  Clock timer_;

  void processDutyCycle(){
    if(state.override_pid){
      //TODO
    }
    else if (!state.override_pid){
      if(timer_.dt_interval() < state.duty_cycle_duration*state.duty_cycle && !state.is_heating){
        activateHeatingElement(true);
        state.is_heating = true;
      }
      else if(timer_.dt_interval() > state.duty_cycle_duration*state.duty_cycle && state.is_heating){
        activateHeatingElement(false);
        state.is_heating = false;
      }
    }
  }

  void activateHeatingElement(bool on){
    Serial.print("Set Heating Element to:  ");
    Serial.println(on);
    
    if(on){
      digitalWrite(HEATING_BUS, HIGH);
    }
    else{
      digitalWrite(HEATING_BUS, LOW);
    }
    
  }
};

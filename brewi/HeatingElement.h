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
    if(timer_.dt_interval() < state.dutyCycleDuration){
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
    if(state.overridePID && (state.heatingElementOn != state.isHeating)){
      activateHeatingElement(state.heatingElementOn);
      state.isHeating - state.heatingElementOn;
      timer_.interval();
    }else if (!state.overridePID){
      if(timer_.dt_interval() < state.dutyCycleDuration*state.dutyCycles && !state.isHeating){
        activateHeatingElement(true);
        state.isHeating = true;
      }
      else if(timer_.dt_interval() > state.dutyCycleDuration*state.dutyCycles && state.isHeating){
        activateHeatingElement(false);
        state.isHeating = false;
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

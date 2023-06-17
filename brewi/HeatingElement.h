#pragma once

#include "State.h"
#include "Chrono.h"

#define HEATING_BUS 12

class HeatingElement{

  public:
    HeatingElement() = default;

    void setup(){
      pinMode(HEATING_BUS, OUTPUT); 
    }

    //Returns wheter current cycle is finished
    void process(){
      if(timer_.hasPassed(state.pidWindowLenght)) timer_.restart();

      processDutyCycle();
    }

  private:

    Chrono timer_;

    void processDutyCycle(){
      bool on = false;

      if(state.override_pid) on = state.is_activated;
      else if(!timer_.hasPassed(state.pidWindowLenght*state.duty_cycle) && !state.is_heating) on = true;

      activateHeatingElement(on);
      state.is_heating = on;
    }

    void activateHeatingElement(bool on){
      Serial.print("Set Heating Element to:  ");
      Serial.println(on);

      if(on) digitalWrite(HEATING_BUS, HIGH);
      else digitalWrite(HEATING_BUS, LOW);

    }
};

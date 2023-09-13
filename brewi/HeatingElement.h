#pragma once

#include "State.h"
#include "Chrono.h"

#define HEATING_BUS D3

class HeatingElement{

  public:
    HeatingElement() = default;

    void setup(){
      pinMode(HEATING_BUS, OUTPUT); 
    }

    //Returns wheter current cycle is finished
    void process(){
      if(timer_.elapsed() > state.pidWindowLenght) {
        timer_.restart();
        Serial.print("10 seconds passed with nIterations: ");
        Serial.println(counter);
        counter = 0;
      }
      ++counter;

      processDutyCycle();
    }

  private:

    Chrono timer_;
    int counter = 0;

    void processDutyCycle(){
      /*
      Serial.println(state.is_activated);
      if(state.is_activated){
        activateHeatingElement(false);
        state.is_activated = false;
        delay(100);
      }else{
        activateHeatingElement(true);
        delay(100);
        state.is_activated = true;
      }
      */
      
      bool on = false;
      
      if(state.override_pid) {
        on = state.is_activated;
      } else if(timer_.elapsed() < int(state.pid_gain*1e3) && state.is_activated) {
        on = true;
      }

      activateHeatingElement(on);
      state.is_heating = on;
      
    }

    void activateHeatingElement(bool on){
      if(on) {
        digitalWrite(HEATING_BUS, HIGH);
      } else {
        digitalWrite(HEATING_BUS, LOW);
      }

    }
};

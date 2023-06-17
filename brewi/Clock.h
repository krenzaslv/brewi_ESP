#pragma once

#include <Chrono.h>

class Clock{
public:

  Clock():secondsChrono_{Chrono::MILLIS}{}

  void interval(){
    secondsChrono_.restart();
  }

  float dt_interval(){
    return secondsChrono_.elapsed()/1e3;
  }

  bool hasPassed(float time){
    return secondsChrono_.hasPassed(time)/1e3;
  }

private:

  Chrono secondsChrono_;
};

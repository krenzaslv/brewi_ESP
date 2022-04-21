#pragma once

#include <Chrono.h>

//TODO not needed anynore thanks to Chrono.h
class Clock{
public:

  Clock():secondsChrono_{Chrono::SECONDS}{}

  void interval(){
    secondsChrono_.restart();
  }

  float dt_interval(){
    return secondsChrono_.elapsed();
  }

  bool hasPassed(float time){
    return secondsChrono_.hasPassed(time);
  }

private:

  Chrono secondsChrono_;
};

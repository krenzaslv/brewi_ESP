#include "State.h"
#include "RestClient.h"
#include "TemperatureSensor.h"
#include "HeatingElement.h"
#include "Clock.h"
#include "PID.h"

RestClient restClient;
TemperatureSensor temperatureSensor;
PID pidController;
Clock timer;
Clock overallTimer;
Clock temperatureUpdateTimer;

HeatingElement heatingElement;

void setup(void){
  Serial.begin(115200);
  restClient.setup();
  temperatureSensor.setup();
  heatingElement.setup();
  timer.interval();
  overallTimer.interval();
  temperatureUpdateTimer.interval();
}

void loop(void){
  //TODO would be nicer with a more functional approach that doesn't modify the state
  temperatureSensor.process();
  pidController.process(timer.dt_interval());
  while(heatingElement.process()){
    restClient.process();
    
    if(temperatureUpdateTimer.dt_interval()>10){
          temperatureSensor.process();
          temperatureUpdateTimer.interval();
    }
    
    //delay(500);
    state.time = overallTimer.dt_interval();
  }
  timer.interval();
  state.time = overallTimer.dt_interval();
}

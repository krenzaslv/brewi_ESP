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

void setup(void) {
  Serial.begin(115200);
  restClient.setup();
  temperatureSensor.setup();
  heatingElement.setup();
  timer.interval();
  overallTimer.interval();
  temperatureUpdateTimer.interval();
}

void loop(void) {
  temperatureSensor.process();
  if (state.is_on) {
    while (heatingElement.process()) {
      restClient.process();
      temperatureSensor.process();
      state.time = overallTimer.dt_interval() / 60;
    } 
    pidController.process(timer.dt_interval());
    timer.interval();
    state.time = overallTimer.dt_interval() / 60;
  }
  
  restClient.process();
}

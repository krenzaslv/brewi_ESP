#include "State.h"
#include "RestClient.h"
#include "TemperatureSensor.h"
#include "HeatingElement.h"
#include "Chrono.h"
#include "PID.h"

const float controlInterval = 100; // ms
const float measurementInterval = 1*1e3; // ms 
const float messageInterval  = 1*1e3; // ms 
const float pidWindowLenght = 10*1e3; // ms

TemperatureSensor<6,1> temperatureSensor; //Running avg over 6 observations 1 at a time 
HeatingElement heatingElement;
RestClient restClient(messageInterval);
PID pidController;
Chrono measurementClock;
Chrono controlClock;


bool currently_activated = false;

void setup(void) {
    Serial.begin(115200);
    state.pidWindowLenght = pidWindowLenght;
    restClient.setup();
    temperatureSensor.setup();
    heatingElement.setup();
    pidController.setup();
 
}

void loop(void) {
    if(controlClock.hasPassed(controlInterval)){
      controlLoop();
    }
    restClient.process();
}

void controlLoop(){

    //Measure every n seconds
    if (measurementClock.hasPassed(measurementInterval)){
        measurementClock.restart();

        //Reset PID if newly activated
        if (state.is_activated && !currently_activated){
            currently_activated = true;
            pidController.reset();
        }

        temperatureSensor.process();
        pidController.process(controlInterval/1000.0);
    }
    
    if (state.is_activated) {
        heatingElement.process();
    }
}

#include "State.h"
#include "RestClient.h"
#include "TemperatureSensor.h"
#include "HeatingElement.h"
#include "Chrono.h"
#include "PID.h"

const int controlInterval = 100; // ms
const int measurementInterval = 5*1000; // ms 
const int messageInterval  = 10*1000; // ms 
const int pidWindowLenght = 10*1000; // ms

TemperatureSensor<2,1> temperatureSensor; //Running avg over 6 observations 1 at a time 
HeatingElement heatingElement;
RestClient restClient(messageInterval);
PID pidController;

Chrono measurementClock;
Chrono controlClock;
Chrono messagingClock;

float setTemp = 0;

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
    
    heatingElement.process(); //Execute controller as fast as possible

    if (measurementClock.elapsed() > measurementInterval){
        float dt = measurementClock.elapsed();
        measurementClock.restart();
        //Reset PID if newly activated
        if (state.is_activated != !currently_activated || state.override_pid || setTemp != state.target_temperature){
            currently_activated = state.is_activated;
            pidController.reset();
        }

        temperatureSensor.process();
        dt += (float) measurementClock.elapsed();
        pidController.process(dt/1000.0);
    }

    if(controlClock.elapsed() > controlInterval){
        messagingClock.restart();
        restClient.process();
    }

}


#include "State.h"
#include "RestClient.h"
#include "TemperatureSensor.h"
#include "HeatingElement.h"
#include "Chrono.h"
#include "PID.h"

const int controlInterval = 100;           // ms
const int measurementInterval = 1 * 1000;  // ms
const int messageInterval = 5 * 1000;      // ms
const int pidWindowLenght = 10 * 1000;     // ms


TemperatureSensor<5, 1> temperatureSensor;  //Running avg over 6 observations 1 at a time
HeatingElement heatingElement;
RestClient restClient;
PID pidController;

Chrono measurementClock;
Chrono controlClock;
Chrono messagingClock;

float setTemp = 0;

bool currently_activated = false;

TaskHandle_t messagingTask;
TaskHandle_t controlTask;

void setup(void) {
  Serial.begin(115200);
  state.pidWindowLenght = pidWindowLenght;
  temperatureSensor.setup();
  heatingElement.setup();
  pidController.setup();
  restClient.setup();

  
  //Run intensive rest communication on seperate thread to not mess with control loop
  xTaskCreatePinnedToCore(
    messagingLoop,
    "messagingLoop",
    100000,          // Stack size in words 
    NULL,           // Task input parameter 
    1,              // Priority of the task 
    &messagingTask, // Task handle. 
    0);             // Core where the task should run 
  delay(500);
  
  
  //Run intensive rest communication on seperate thread to not mess with control loop
  xTaskCreatePinnedToCore(
    controlLoop,
    "controlLoop",
    10000,        // Stack size in words 
    NULL,         // Task input parameter 
    1,            // Priority of the task 
    &controlTask, // Task handle. 
    1);           // Core where the task should run 
  delay(500);
  
}

void controlLoop(void* param) {
  while (true) {
    if (controlClock.elapsed() > controlInterval) {
      controlClock.restart();
      heatingElement.process();  //Execute controller as fast as possible
    }

    if (measurementClock.elapsed() > measurementInterval) {
      float dt = measurementClock.elapsed();
      measurementClock.restart();
      //Reset PID if newly activated
      if (state.is_activated != !currently_activated || state.override_pid || setTemp != state.target_temperature) {
        currently_activated = state.is_activated;
        setTemp = state.target_temperature;
        pidController.reset();
      }

      temperatureSensor.process();
      dt += (float)measurementClock.elapsed();
      pidController.process(dt / 1000.0);
    }
  }
}

void messagingLoop(void* param) {
  while (true) {
    if (messagingClock.elapsed() > messageInterval) {
      Serial.println("Messaging...");
      messagingClock.restart();
      restClient.process();
    }
  }
}

void loop(void) {
}

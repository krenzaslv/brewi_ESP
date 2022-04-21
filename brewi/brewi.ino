#include <OneWire.h>
#include <DallasTemperature.h>
#include "State.h"
#include "RestClient.h"

#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
RestClient restClient;

void setup(void){
  Serial.begin(115200);
  sensors.begin();
  restClient.setup();
}

void loop(void){
  sensors.requestTemperatures(); // Send the command to get temperatures
  state.temperature = sensors.getTempCByIndex(0);

  restClient.process();

  delay(500);
}

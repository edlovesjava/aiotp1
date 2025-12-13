/*
 * Custom library implementation
 * 
 * Place your custom library implementation files (.cpp) here
 */

#include "../include/custom_lib.h"

void exampleFunction() {
  Serial.println("Example function called");
}

// Sensor class implementation
Sensor::Sensor(int sensorPin) {
  pin = sensorPin;
}

void Sensor::init() {
  pinMode(pin, INPUT);
  Serial.print("Sensor initialized on pin ");
  Serial.println(pin);
}

int Sensor::read() {
  return analogRead(pin);
}

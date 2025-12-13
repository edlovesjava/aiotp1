/*
 * Custom library header files
 * 
 * Place your custom library header files (.h) here
 * These files should declare classes, functions, and constants
 * used by your Arduino project.
 */

#ifndef CUSTOM_LIB_H
#define CUSTOM_LIB_H

#include <Arduino.h>

// Example function declaration
void exampleFunction();

// Example class declaration
class Sensor {
  private:
    int pin;
    
  public:
    Sensor(int sensorPin);
    void init();
    int read();
};

#endif

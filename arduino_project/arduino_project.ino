/*
 * AIoT Project 1 - Main Arduino Sketch
 * 
 * This is the main entry point for your Arduino project.
 * Add your setup code in setup() and your main logic in loop().
 */

#include <Arduino.h>

// Pin definitions
#define LED_PIN 13

// Global variables
unsigned long previousMillis = 0;
const long interval = 1000;  // Blink interval in milliseconds

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Initialize LED pin
  pinMode(LED_PIN, OUTPUT);
  
  Serial.println("AIoT Project 1 - Starting...");
}

void loop() {
  // Simple blink example using millis() (non-blocking)
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Toggle LED
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    
    // Print status to serial monitor
    Serial.print("LED toggled at: ");
    Serial.println(currentMillis);
  }
  
  // Add your main code here
}

/*
 * Example: Blink LED
 * 
 * Simple example that demonstrates basic Arduino functionality
 * This example shows how to blink the built-in LED
 */

#define LED_PIN 13

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Blink Example Started");
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  Serial.println("LED ON");
  delay(1000);
  
  digitalWrite(LED_PIN, LOW);
  Serial.println("LED OFF");
  delay(1000);
}

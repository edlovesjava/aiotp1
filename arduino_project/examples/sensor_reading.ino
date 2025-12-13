/*
 * Example: Sensor Reading
 * 
 * This example demonstrates how to read analog sensor values
 * and send them via serial communication
 */

#define SENSOR_PIN A0
#define READ_INTERVAL 500  // milliseconds

unsigned long lastReadTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT);
  Serial.println("Sensor Reading Example Started");
}

void loop() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastReadTime >= READ_INTERVAL) {
    lastReadTime = currentTime;
    
    int sensorValue = analogRead(SENSOR_PIN);
    float voltage = sensorValue * (5.0 / 1023.0);
    
    Serial.print("Sensor Value: ");
    Serial.print(sensorValue);
    Serial.print(" - Voltage: ");
    Serial.print(voltage);
    Serial.println("V");
  }
}

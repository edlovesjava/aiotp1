/*
 * Example: DHT Temperature and Humidity Sensor
 *
 * This example demonstrates how to read temperature and humidity
 * values from a DHT11 or DHT22 sensor using non-blocking code.
 *
 * Hardware Required:
 * - Arduino board (Uno, Mega, Nano, etc.)
 * - DHT11 or DHT22 sensor
 * - 10K ohm pull-up resistor (some modules have this built-in)
 *
 * Wiring:
 * - DHT Pin 1 (VCC) -> Arduino 5V (or 3.3V for DHT22)
 * - DHT Pin 2 (Data) -> Arduino Digital Pin 2 + 10K resistor to VCC
 * - DHT Pin 3 (NC) -> Not connected
 * - DHT Pin 4 (GND) -> Arduino GND
 *
 * Library Required:
 * Install "DHT sensor library" by Adafruit via Library Manager
 * Also install "Adafruit Unified Sensor" dependency
 */

#include <DHT.h>

// Configuration
#define DHT_PIN 2              // Digital pin connected to the DHT sensor
#define DHT_TYPE DHT11         // DHT11 or DHT22
#define READ_INTERVAL 2000     // DHT sensors need at least 2 seconds between reads

// Global variables
DHT dht(DHT_PIN, DHT_TYPE);
unsigned long lastReadTime = 0;
bool sensorReady = false;

// Store last valid readings for error recovery
float lastValidTemp = 0.0;
float lastValidHumidity = 0.0;

void setup() {
  Serial.begin(9600);
  Serial.println("DHT Temperature and Humidity Sensor Example");
  Serial.println("-------------------------------------------");

  dht.begin();

  // Wait for sensor to stabilize
  delay(1000);
  Serial.println("Sensor initialized. Starting readings...\n");
}

void loop() {
  unsigned long currentTime = millis();

  // Non-blocking read using millis()
  if (currentTime - lastReadTime >= READ_INTERVAL) {
    lastReadTime = currentTime;
    readAndDisplaySensorData();
  }

  // Other non-blocking tasks can be added here
  // The main loop continues running without blocking
}

void readAndDisplaySensorData() {
  // Reading temperature or humidity takes about 250 milliseconds
  // Sensor readings may also be up to 2 seconds old
  float humidity = dht.readHumidity();
  float tempC = dht.readTemperature();        // Celsius
  float tempF = dht.readTemperature(true);    // Fahrenheit

  // Check if any reads failed
  if (isnan(humidity) || isnan(tempC) || isnan(tempF)) {
    Serial.println("Error: Failed to read from DHT sensor!");
    Serial.println("Check wiring and pull-up resistor.");
    return;
  }

  // Store valid readings
  lastValidTemp = tempC;
  lastValidHumidity = humidity;

  // Calculate heat index (feels like temperature)
  float heatIndexC = dht.computeHeatIndex(tempC, humidity, false);
  float heatIndexF = dht.computeHeatIndex(tempF, humidity);

  // Display readings
  Serial.println("=== Sensor Reading ===");
  Serial.print("Humidity: ");
  Serial.print(humidity, 1);
  Serial.println("%");

  Serial.print("Temperature: ");
  Serial.print(tempC, 1);
  Serial.print("°C / ");
  Serial.print(tempF, 1);
  Serial.println("°F");

  Serial.print("Heat Index: ");
  Serial.print(heatIndexC, 1);
  Serial.print("°C / ");
  Serial.print(heatIndexF, 1);
  Serial.println("°F");

  // Comfort level indicator
  Serial.print("Comfort: ");
  Serial.println(getComfortLevel(tempC, humidity));

  Serial.println();
}

// Determine comfort level based on temperature and humidity
const char* getComfortLevel(float tempC, float humidity) {
  // Comfortable range: 20-26°C and 30-60% humidity
  bool tempOk = (tempC >= 20.0 && tempC <= 26.0);
  bool humidityOk = (humidity >= 30.0 && humidity <= 60.0);

  if (tempOk && humidityOk) {
    return "Comfortable";
  } else if (tempC < 18.0) {
    return "Too Cold";
  } else if (tempC > 28.0) {
    return "Too Hot";
  } else if (humidity < 30.0) {
    return "Too Dry";
  } else if (humidity > 70.0) {
    return "Too Humid";
  } else {
    return "Acceptable";
  }
}

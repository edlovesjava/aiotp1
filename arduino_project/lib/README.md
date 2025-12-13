# Library Directory

This directory is for external Arduino libraries that are not available through the Arduino Library Manager or require custom modifications.

## Adding External Libraries

### Method 1: Manual Installation
1. Download the library (usually a .zip file)
2. Extract it to this `lib/` directory
3. Restart Arduino IDE
4. Include the library in your sketch:
   ```cpp
   #include <LibraryName.h>
   ```

### Method 2: Arduino Library Manager (Recommended)
1. Open Arduino IDE
2. Go to Sketch → Include Library → Manage Libraries
3. Search for the library you need
4. Click Install

## Common Libraries for IoT Projects

Here are some useful libraries for AIoT projects:

### Sensors
- **DHT sensor library** - For DHT11/DHT22 temperature and humidity sensors
- **Adafruit Sensor** - Unified sensor library
- **BMP280** - Pressure and temperature sensor

### Communication
- **WiFi** - Built-in WiFi support for ESP32/ESP8266
- **ESP8266WiFi** - WiFi for ESP8266
- **PubSubClient** - MQTT client for IoT messaging
- **ArduinoJson** - JSON parsing and generation

### Display
- **Adafruit GFX** - Graphics library
- **Adafruit SSD1306** - OLED display driver
- **LiquidCrystal** - LCD display control

### Storage
- **SD** - SD card reading/writing
- **EEPROM** - EEPROM memory access

## Library Structure

A typical Arduino library contains:
```
LibraryName/
├── LibraryName.h      # Header file
├── LibraryName.cpp    # Implementation
├── keywords.txt       # Syntax highlighting
├── library.properties # Library metadata
└── examples/          # Example sketches
```

## Custom Library Development

If you're creating your own library:
1. Create a folder with your library name
2. Add header (.h) and implementation (.cpp) files
3. Follow Arduino library specifications
4. See: https://www.arduino.cc/en/Hacking/LibraryTutorial

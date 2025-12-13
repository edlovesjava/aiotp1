# Test Directory

This directory is reserved for unit tests and test fixtures for your Arduino project.

## Testing Arduino Code

### Option 1: Arduino IDE Serial Monitor Testing
- Upload your code
- Open Serial Monitor
- Verify expected output

### Option 2: Using AUnit (Arduino Unit Testing)
Install the AUnit library for unit testing:
1. Open Arduino IDE
2. Go to Sketch → Include Library → Manage Libraries
3. Search for "AUnit"
4. Install it

Example test structure:
```cpp
#include <AUnit.h>

test(sensor_reading) {
  int value = analogRead(A0);
  assertTrue(value >= 0 && value <= 1023);
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  aunit::TestRunner::run();
}
```

### Option 3: Using PlatformIO
PlatformIO provides better testing support. To migrate to PlatformIO:
1. Install PlatformIO
2. Create a new project from this code
3. Add tests in the `test/` directory

## Test Files

Place your test files here with descriptive names:
- `test_sensors.cpp`
- `test_communication.cpp`
- `test_utilities.cpp`

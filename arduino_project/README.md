# Arduino Project

This is a starter Arduino project for AIoT Project 1 with an organized directory structure for better code management.

## Project Structure

```
arduino_project/
├── arduino_project.ino    # Main Arduino sketch
├── include/               # Header files (.h)
│   └── custom_lib.h
├── src/                   # Implementation files (.cpp)
│   └── custom_lib.cpp
├── lib/                   # External libraries
├── examples/              # Example sketches
│   ├── blink_example.ino
│   ├── sensor_reading.ino
│   └── dht_temperature_humidity.ino
└── test/                  # Test files
```

## Getting Started

### Prerequisites

- Arduino IDE (version 1.8.x or later) or Arduino CLI
- USB cable to connect your Arduino board
- Arduino board (e.g., Arduino Uno, Mega, Nano)

### Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/edlovesjava/aiotp1.git
   cd aiotp1
   ```

2. Open the Arduino IDE

3. Open the main sketch:
   - File → Open → Navigate to `arduino_project/arduino_project.ino`

4. Select your Arduino board:
   - Tools → Board → Select your board (e.g., Arduino Uno)

5. Select the correct port:
   - Tools → Port → Select the port your Arduino is connected to

6. Upload the sketch:
   - Click the Upload button (→) or press Ctrl+U

## Usage

### Main Sketch

The main sketch (`arduino_project.ino`) includes a simple LED blink example using non-blocking code with `millis()`. This is a good starting point for your project.

### Adding Custom Libraries

1. Place header files (`.h`) in the `include/` directory
2. Place implementation files (`.cpp`) in the `src/` directory
3. Include your headers in the main sketch:
   ```cpp
   #include "include/custom_lib.h"
   ```

### Examples

The `examples/` directory contains sample sketches:
- **blink_example.ino**: Basic LED blink using delay()
- **sensor_reading.ino**: Reading analog sensor values
- **dht_temperature_humidity.ino**: Temperature and humidity monitoring with DHT11/DHT22 sensors (requires DHT sensor library)

To use an example:
1. Open the example file in Arduino IDE
2. Upload to your board

### Adding External Libraries

Place third-party libraries in the `lib/` directory or install them via Arduino IDE:
- Sketch → Include Library → Manage Libraries

## Serial Monitor

The project uses Serial communication at 9600 baud. To view output:
1. Upload the sketch
2. Open Serial Monitor: Tools → Serial Monitor
3. Set baud rate to 9600

## Customization

### Pin Definitions

Modify pin definitions at the top of the sketch:
```cpp
#define LED_PIN 13
#define SENSOR_PIN A0
```

### Adding Sensors/Actuators

1. Define pins for your components
2. Initialize them in `setup()`
3. Add your logic in `loop()`

## Best Practices

- Use `millis()` instead of `delay()` for non-blocking code
- Add comments to explain your code
- Organize reusable code into functions or classes
- Use meaningful variable and function names
- Keep `setup()` for initialization only
- Put your main logic in `loop()`

## Troubleshooting

### Upload Issues
- Check that the correct board and port are selected
- Ensure the USB cable is properly connected
- Try pressing the reset button on the Arduino before uploading

### Serial Monitor Issues
- Verify baud rate matches the code (9600)
- Check that the correct port is selected

## Resources

- [Arduino Official Documentation](https://www.arduino.cc/reference/en/)
- [Arduino Language Reference](https://www.arduino.cc/reference/en/)
- [Arduino Forum](https://forum.arduino.cc/)

## License

This project is open source and available for educational purposes.

## Contributing

Feel free to contribute by:
1. Forking the repository
2. Creating a feature branch
3. Submitting a pull request

# AIoT Project 1

An organized Arduino starter project for AI + IoT development.

## Overview

This repository provides a well-structured starting point for Arduino-based AIoT (Artificial Intelligence of Things) projects. It includes a clean project structure, example code, and comprehensive documentation to help you quickly start developing Arduino applications.

## Features

- 📁 **Organized Directory Structure** - Clean separation of concerns with dedicated folders for source code, libraries, examples, and tests
- 🚀 **Ready-to-Use Examples** - Multiple example sketches demonstrating common Arduino patterns
- 📚 **Documentation** - Comprehensive README files explaining project structure and usage
- 🛠️ **Best Practices** - Non-blocking code examples using millis() instead of delay()
- 🧪 **Test Directory** - Placeholder for unit tests and testing documentation

## Quick Start

1. **Clone the repository:**
   ```bash
   git clone https://github.com/edlovesjava/aiotp1.git
   cd aiotp1
   ```

2. **Open in Arduino IDE:**
   - Navigate to `arduino_project/arduino_project.ino`
   - Open the file in Arduino IDE

3. **Configure your board:**
   - Tools → Board → Select your Arduino board
   - Tools → Port → Select your port

4. **Upload:**
   - Click the Upload button (→)

See the [Arduino Project README](arduino_project/README.md) for detailed instructions.

## Project Structure

```
aiotp1/
├── arduino_project/           # Main Arduino project
│   ├── arduino_project.ino    # Main sketch
│   ├── include/               # Header files
│   ├── src/                   # Implementation files
│   ├── lib/                   # External libraries
│   ├── examples/              # Example sketches
│   ├── test/                  # Test files
│   └── README.md              # Project documentation
├── .gitignore                 # Git ignore rules
└── README.md                  # This file
```

## What's Included

### Main Sketch
- Non-blocking LED blink example using millis()
- Serial communication setup
- Well-commented code structure

### Examples
- **blink_example.ino** - Simple LED blink using delay()
- **sensor_reading.ino** - Analog sensor reading with serial output
- **dht_temperature_humidity.ino** - DHT11/DHT22 temperature and humidity monitoring with comfort level indicator

### Custom Library Template
- Header and implementation file examples
- Sample Sensor class demonstrating OOP in Arduino

### Documentation
- Detailed setup instructions
- Best practices guide
- Troubleshooting tips
- Resource links

## Requirements

- Arduino IDE 1.8.x or later (or Arduino CLI)
- Arduino board (Uno, Mega, Nano, ESP32, ESP8266, etc.)
- USB cable

## Use Cases

This starter project is ideal for:
- 🌡️ Environmental monitoring systems
- 🏠 Home automation projects
- 📊 Data logging applications
- 🤖 Robotics projects
- 🔌 IoT sensor networks
- 🧠 AI-enabled edge devices

## Contributing

Contributions are welcome! Feel free to:
- Report bugs
- Suggest new features
- Submit pull requests
- Improve documentation

## Resources

- [Arduino Documentation](https://www.arduino.cc/reference/en/)
- [Arduino Forum](https://forum.arduino.cc/)
- [Arduino Project Hub](https://create.arduino.cc/projecthub)

## License

Open source and available for educational purposes.

---

**Happy Making! 🔧**

# aiotp1

ESP32 nodes talking over ESP-NOW: an ESP32-S3 voice controller (wake word,
short command vocabulary, SSD1306 OLED) and actuator nodes (LED first, relay
later). The product spec in `docs/product-spec.md` is the source of truth for
scope, epics, and constraints; read it before starting any issue.

Heritage: iotmesh / MeshSwarm. We keep the PlatformIO layout, the serial
command console, the OLED status pattern, and versioned key-value state. We
do not use painlessMesh; the transport is ESP-NOW.

## Layout

```
CLAUDE.md                     this file
barney.toml                   coding-agent configuration
docs/product-spec.md          product spec, epics, constraints
.github/workflows/ci.yml      runs the Verification commands below
firmware/
  platformio.ini              envs: voice_s3, actuator_led, native
  include/pins_*.h            one pin header per board (the only place pins live)
  lib/<Name>/                 project libraries, one directory each
  nodes/<node>/main.cpp       one directory per node, selected by build_src_filter
  test/test_<name>/           Unity tests, run on the native env
```

## Verification

Run from the repo root. Both must pass before a PR is opened.

    pio run -d firmware
    pio test -d firmware -e native

CI (`.github/workflows/ci.yml`) runs exactly these two commands. Never change
CI or the verification commands to make them pass; fix the code.

## Conventions

- Stack: PlatformIO, Arduino framework for ESP32 (arduino-esp32 3.x), C++17.
  Libraries: Adafruit SSD1306, Adafruit GFX, ArduinoJson. Host tests use
  PlatformIO `native` with Unity.
- The build is `-Wall -Wextra` clean. Treat new warnings as failures.
- No blocking `delay()` longer than 10 ms in any loop. Use `millis()` timers
  and state machines; periodic work (OLED refresh, heartbeat) runs at a
  fixed rate without stalling `loop()`.
- Pins are defined only in `firmware/include/pins_<board>.h`. Never hardcode
  a GPIO number anywhere else. Unconfirmed pins are marked "assumed, confirm".
- Pure-logic code (message codec, command parser, state store, ring buffer)
  lives in `firmware/lib/<Name>/` with no Arduino dependency, and every such
  module gets a native test in `firmware/test/test_<name>/`. Hardware-touching
  code is thin and lives in the node `main.cpp` or a `*_hw.cpp` file.
- Serial console commands reuse iotmesh names where they overlap (`status`,
  `peers`, `send`, `set`, `get`, `reboot`, `help`).
- Serial at 115200 baud. Log lines are single-line `key=value` pairs where
  practical so they are easy to grep.
- Small, focused commits. One issue per PR. Do not touch unrelated files.

## Adding a node

1. Create `firmware/nodes/<node_name>/main.cpp` with `setup()` and `loop()`.
2. Add `firmware/include/pins_<board>.h` if the board is new.
3. Add `[env:<node_name>]` to `firmware/platformio.ini` with `platform`,
   `framework = arduino`, `board`, and `build_src_filter = +<<node_name>/>`.
4. Add the env to `default_envs` so `pio run -d firmware` builds it.

## Adding a library

1. Create `firmware/lib/<Name>/` with `<Name>.h` and `<Name>.cpp` (add a
   `library.json` if it needs dependencies).
2. Keep it free of `Arduino.h` when it is pure logic, so it compiles on
   `native`.
3. Add `firmware/test/test_<name>/test_main.cpp` with Unity tests and run
   `pio test -d firmware -e native`.

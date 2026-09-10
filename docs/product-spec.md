# aiotp1 product spec: ESP32-S3 voice node over ESP-NOW

Status: **draft v1**. This repo is the **target** for the autonomous coding
agent in [barney-bedrock](https://github.com/edlovesjava/barney-bedrock).
The agent works from GitHub issues; this document is the source those issues
are cut from. Keep it current, the agent reads it.

## 1. What we are building

A small family of ESP32 nodes that talk over **ESP-NOW** (no WiFi
association, no router). One node type is a **voice controller**: an
ESP32-S3 with a microphone, a wake word, a short command vocabulary, and a
128x64 OLED that shows status. Saying "<wake word>, lights on" makes the
controller broadcast a command that an **actuator node** (LED, relay, later
anything) executes.

Heritage: [iotmesh](https://github.com/edlovesjava/iotmesh) and its
[MeshSwarm](https://github.com/edlovesjava/MeshSwarm) library. We keep the
PlatformIO layout, the serial command console, the OLED status pattern, and
the versioned key-value state idea. We do **not** use painlessMesh: the
transport here is ESP-NOW.

## 2. Hardware (to be confirmed by Ed)

| Item | Assumed part | Notes |
|---|---|---|
| Controller MCU | ESP32-S3 DevKitC-1, N16R8 (16 MB flash, 8 MB octal PSRAM) | PSRAM is required by ESP-SR wake word models |
| Microphone | INMP441 I2S MEMS | pins TBD, defaults in `platformio.ini` |
| Display | SSD1306 128x64 I2C | address 0x3C |
| Actuator MCU | any ESP32 (classic or S3 or C3) | ESP-NOW works across all |
| Actuator output | onboard LED first, relay later | |

Pin assignments live in one header per board so the agent can change them in
one place.

## 3. Software stack

- PlatformIO, Arduino framework for ESP32 (arduino-esp32 3.x, which bundles
  ESP-SR for wake word and command recognition on S3).
- Libraries: Adafruit SSD1306, Adafruit GFX, ArduinoJson.
- Host unit tests: PlatformIO `native` environment with Unity, for all
  pure-logic code (message codec, command parser, state store).
- CI: GitHub Actions runs `pio run` for every environment and `pio test -e
  native` on every PR. This is what the agent uses to verify its work.

## 4. Repository layout (target state)

```
aiotp1/
  CLAUDE.md               conventions + ## Verification (the agent reads this)
  docs/product-spec.md    this file
  firmware/
    platformio.ini        envs: voice_s3, actuator_led, native
    include/
      pins_voice_s3.h
      pins_actuator.h
    lib/
      NowLink/            ESP-NOW transport: peer table, message codec, ack
      NowState/           versioned key-value state over NowLink (from MeshSwarm ideas)
      Commands/           text command grammar -> command struct (pure logic)
    nodes/
      voice_s3/main.cpp
      actuator_led/main.cpp
    test/
      test_codec/
      test_commands/
      test_state/
  .github/workflows/ci.yml
```

## 5. Epics and issues

Each numbered item below is meant to become one GitHub issue labelled
`agent`. Acceptance criteria are written so the agent can check them by
running commands. Order matters; each builds on the last.

### Epic A: Skeleton and CI

1. **PlatformIO skeleton + CI.** `firmware/platformio.ini` with `voice_s3`,
   `actuator_led`, `native` environments; blink `main.cpp` for the two board
   envs; one passing Unity test in `native`; `.github/workflows/ci.yml` runs
   both. AC: `pio run` and `pio test -e native` pass in CI.
2. **CLAUDE.md with Verification section.** Conventions from iotmesh adapted
   here, plus the exact verify commands. AC: file exists, commands match CI.

### Epic B: Display and console

3. **OLED status screen.** Boot splash, then a status page: node name, MAC,
   peer count, last command, uptime. Refresh 1 Hz without blocking the main
   loop. AC: compiles for `voice_s3`; display code isolated in
   `lib/Display/`.
4. **Serial command console.** `status`, `peers`, `send <cmd>`, `reboot`,
   `help`. Same command names as iotmesh where they overlap. AC: parser in
   `lib/Console/` with native tests for tokenising and dispatch.

### Epic C: ESP-NOW transport

5. **Message codec.** Fixed-header binary message (magic, version, type,
   sequence, origin MAC, payload len, CRC) with encode/decode in pure C++.
   AC: native tests cover round trip, truncation, bad CRC, max payload.
6. **NowLink transport.** Init ESP-NOW, broadcast discovery, maintain a peer
   table with last-seen, send unicast with ack and one retry, receive
   callback dispatch. AC: compiles for both board envs; peer table logic has
   native tests; `peers` console command lists peers.
7. **NowState.** Versioned key-value store synchronised over NowLink using
   the iotmesh rule (higher version wins, tie to lower origin id), with
   `watchState(key, cb)`. AC: native tests for conflict resolution; `set` and
   `get` console commands.

### Epic D: Audio and wake word

8. **I2S microphone capture.** 16 kHz mono PCM from INMP441 into a ring
   buffer; `mic` console command prints RMS level. AC: compiles on
   `voice_s3`; ring buffer has native tests.
9. **Wake word.** ESP-SR WakeNet on the S3 with a built-in wake word (the
   exact word depends on the installed model set, "Hi ESP" is the safe
   default). OLED shows a listening indicator when triggered. AC: compiles
   with PSRAM flags; wake event logged to serial and shown on OLED.

### Epic E: Voice commands to actuators

10. **Command grammar.** Pure-logic parser mapping recognised phrases
    ("lights on", "lights off", "all off") to a command struct with target
    and action. AC: native tests; used by both the console `send` command
    and the voice path.
11. **Command recognition.** ESP-SR MultiNet with a configured phrase list
    matching the grammar; after wake word, listen for one command, dispatch
    it through NowLink, show result on OLED. AC: compiles; phrase list and
    grammar are generated from one source file.
12. **Actuator LED node.** Receives commands over NowLink, drives the LED,
    acks, publishes its state via NowState. AC: compiles for `actuator_led`;
    handler has native tests.

### Epic F: Later

- Relay actuator, multiple named targets ("kitchen lights").
- Gateway node bridging NowState to the iotmesh server API.
- OTA over ESP-NOW.

## 6. Constraints the agent must respect

- Never change CI to make it pass. Fix the code.
- No blocking `delay()` in main loops beyond 10 ms; use `millis()` timers.
- Every pure-logic module gets a native test. Hardware-touching code is thin
  and lives in the node `main.cpp` or a `*_hw.cpp` file.
- Pins only in `include/pins_*.h`.
- Keep the build warning-free with `-Wall`.

## 7. Open questions for Ed

1. Confirm the board (N16R8 or other) and microphone part. Pin numbers.
2. Wake word preference, if any, from the ESP-SR model list.
3. Is the first actuator the onboard LED, or do you have a relay board ready?
4. Should the actuator node be a classic ESP32 from the iotmesh fleet, to
   prove cross-chip ESP-NOW early?

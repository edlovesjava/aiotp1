// actuator_led node: classic ESP32 actuator. Skeleton: non-blocking blink
// plus a once-per-second serial heartbeat with uptime.

#include <Arduino.h>

#include "pins_actuator.h"

namespace {

constexpr uint32_t kBlinkIntervalMs = 500;
constexpr uint32_t kHeartbeatIntervalMs = 1000;

uint32_t lastBlinkMs = 0;
uint32_t lastHeartbeatMs = 0;
bool ledOn = false;

}  // namespace

void setup() {
    Serial.begin(115200);
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);
    Serial.println("aiotp1 actuator_led boot");
}

void loop() {
    const uint32_t now = millis();

    if (now - lastBlinkMs >= kBlinkIntervalMs) {
        lastBlinkMs = now;
        ledOn = !ledOn;
        digitalWrite(PIN_LED, ledOn ? HIGH : LOW);
    }

    if (now - lastHeartbeatMs >= kHeartbeatIntervalMs) {
        lastHeartbeatMs = now;
        Serial.printf("heartbeat node=actuator_led uptime_s=%lu\n",
                      static_cast<unsigned long>(now / 1000));
    }
}
